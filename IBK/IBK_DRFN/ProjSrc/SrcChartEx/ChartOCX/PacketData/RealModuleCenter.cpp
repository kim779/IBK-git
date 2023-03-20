// RealModuleCenter.cpp: implementation of the CRealModuleCenter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RealModuleCenter.h"

#include <assert.h>

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/IChartCtrl.h"					// for IChartCtrl

#include "PacketBase.h"
#include "Conversion.h"
#include "DataMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CRealModuleCenter

CRealModuleCenter::CRealModuleCenter( const CPacketType::REALTYPE eRealType, IChartCtrl *p_pIChartCtrl, CTypedPtrList<CObList, CPacket*>* pPacketList) :
	m_realDataModule( p_pIChartCtrl->GetSafeHwnd())
{
	// (2009/11/4 - Seung-Won, Bae) Backup to receive NMTR Data.
	m_pIChartCtrl = p_pIChartCtrl;

	// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
	m_nChartModeSetting = p_pIChartCtrl->GetChartMode();

	// (2009/10/19 - Seung-Won, Bae) Real Data Processing Interface.
	m_pOpenPacket	= NULL;
	m_pHighPacket	= NULL;
	m_pLowPacket	= NULL;
	m_pClosePacket	= NULL;
	m_pVolumePacket	= NULL;
	m_bOnClockedData = FALSE;

	// (2009/10/19 - Seung-Won, Bae) comment.
	//	Base Packet�� ����ó�� - PacketList ���� ������ �Ǵ� AddPacket�� ���Ͽ� CPacketListImplementation::m_PacketList��
	//		Packet�� �߰��Ǵ� ��� C0_DATE_TIME Packet (�ڷ�����)�� ���Ͽ� ����Ѵ�.
	//	�׿ܿ��� ������ CPacketList::SetBasePacketName()�� ���� ���� �����ޱ⵵ �Ѵ�. (�Ⱦ��ϵ�.)
	m_pRealBasePacket = NULL;

	// (2009/10/20 - Seung-Won, Bae) Comment. �к� ó�� Logic Class�� �����Ѵ�.
	m_pTimeRealDataModule = NULL;

	// (2009/10/21 - Seung-Won, Bae) Comment. �ǽð� Data�� Packet Type�� �����Ѵ�.
	m_strRealTimeType = "HHMMSS";

	// (2009/10/22 - Seung-Won, Bae) Comment. X Scale Manager���� �ð� Merge�� �����ϵ��� �к� �ø� ���θ� �����Ѵ�.
	m_eRealType = eRealType;

	// 20080925 JS.Kim	���� �ٸ� �ð����� Scale ó��
	m_nOrgBaseTimeDiff = 0;
	m_nBaseTimeDiff = 0;

	// (2009/11/1 - Seung-Won, Bae) Support Last Real Time.
	m_nClock = -1;
	m_nLastClockTime = -1;
	m_nLastRealTime = -1;

	InitializeHelpMessage();
}

CRealModuleCenter::~CRealModuleCenter()
{
	delete m_pTimeRealDataModule;
}


// (2009/10/19 - Seung-Won, Bae) comment.
//	Base Packet�� ����ó�� - PacketList ���� ������ �Ǵ� AddPacket�� ���Ͽ� CPacketListImplementation::m_PacketList��
//		Packet�� �߰��Ǵ� ��� C0_DATE_TIME Packet (�ڷ�����)�� ���Ͽ� ����Ѵ�.
//	�׿ܿ��� ������ CPacketList::SetBasePacketName()�� ���� ���� �����ޱ⵵ �Ѵ�. (�Ⱦ��ϵ�.)
void CRealModuleCenter::SetRealBasePacket( CPacket* pPacket)
{
	m_pRealBasePacket = pPacket;
}


// (2009/10/19 - Seung-Won, Bae) Comment. ������ �ǽð� ���ŵ� DTI.
//		��ü Data���⿡�� Update�� �� Ȯ�ν� �̿��Ѵ�.
//		�ڷ����ڰ� �ƴ� ������Ŷ�� ���� �ǽð��� ������ ��츸 �̿�ȴ�.
void CRealModuleCenter::SetUpdateOrAppendIndex( const int nUpdateOrAppendIndex)
{
	m_nUpdateOrAppendIndex = nUpdateOrAppendIndex;
}
int CRealModuleCenter::GetUpdateOrAppendIndex( void) const
{
	return m_nUpdateOrAppendIndex;
}


// (2009/10/19 - Seung-Won, Bae) Comment. ������ Real�� �ݿ��� DTI Range?
//		��ǥ ���� CMainChartFormulateImplementation::GetDataPoint()���� ��ȸ�Ѵ�.
//			(������ range�� �̿���� ������ start�θ� �̿�Ǵ� ������ �Ǵܵȴ�.)
//		UM=B�� ������ �ƴ� ���� �߰� Data�� �����ϴ� ��쿡 �߰��� ��ǥ ������ ���Ͽ� �����Ѵ�.
void CRealModuleCenter::SetUpdateRangeOffset(const int nStart, const int nEnd)
{
	m_ptUpdateRange.Offset(nStart, nEnd);
}
// �ǽð� ���ſ� ���� ���� ����ؾߵ� data ������ �����Ѵ�.
void CRealModuleCenter::SetPacketMinMaxAndUpdateRage(CPacket* pPacket)
{
	assert(pPacket != NULL);

	int nDataCount = pPacket->GetCount();
	// data�� ���� ���
	if(nDataCount <= 0)
		m_ptUpdateRange = CPoint(-1, -1);
	// data �� 1��
	else if(nDataCount == 1)
		m_ptUpdateRange = CPoint(0, 0);
	// data �� 2�� �̻�
	else
		m_ptUpdateRange = CPoint(nDataCount - 2, nDataCount - 1);
}
CPoint CRealModuleCenter::GetUpdateRange( void) const
{
	return m_ptUpdateRange;
}


// (2009/10/19 - Seung-Won, Bae) Comment. Real Data�� �����ϴ� Logic Class�̴�.
//		���� RDC�θ� �̿�ǰ� ����. ER�� ������ RDC = 100 / ER �� ������ �����Ǵ� ����� ���̹Ƿ� RDC�� ������ �ٶ����ϴ�.
// �����޽����� RDC �������� �����ϴ� Interface
bool CRealModuleCenter::SetRealDataModuleAndData( const bool bIsRemove, const CString& strModuleHelpMsg, const bool bIsExchangeRate_Percent)
{
	// �⺻ ���� ������ �߰�
	if( !bIsRemove) return m_realDataModule.ChangeRealDataModuleAndData(strModuleHelpMsg);

	m_realDataModule.SetRealDataModuleType( CRealDataModule::RATE_TYPE, strModuleHelpMsg);
	// ȯ�� ���� ����.
	if( bIsExchangeRate_Percent) m_realDataModule.ChangeExchangeRateData_Percent();
	return true;
}


// (2009/10/20 - Seung-Won, Bae) Nƽ�� ���� ���������� �� Tick�� ���� (�����޽��� UTEC�� ���� ������)
void CRealModuleCenter::SetAddTimeInterval(const int nData)
{
	//sy 2005.07.28. nƽ�� ��� ������ ���� ƽ��.
	if( nData < 0)	m_nAddTimeInterval = 0;
	else			m_nAddTimeInterval = nData;
}


// (2009/10/19 - Seung-Won, Bae) Comment. UD �����޽����� (�ֱⰪ)�� �����Ѵ�.
void CRealModuleCenter::SetDateUnitType( const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType)
{
	m_eDateUnitType = eDateUnitType;
}
CScaleBaseData::HORZSCALEDRAWERTYPE CRealModuleCenter::GetDateUnitType( void) const
{
	return m_eDateUnitType;
}


// (2009/10/20 - Seung-Won, Bae) UT ������ 1���� �����ϱ� ���� UD�� �����̴�.
void CRealModuleCenter::SetTimeInterval(const int nTimeInterval)
{
	//>> 20091214_JS.Kim  ����.   ���׼���.  �����޼����� UT�� 0�� ��� TimeRange ���� ���ѷ����� ������ ���� ����
	if( nTimeInterval <= 0 )
		m_nTimeInterval = 1;
	else
	//<<
		m_nTimeInterval = nTimeInterval;
}
int CRealModuleCenter::GetTimeInterval( void) const
{
	return m_nTimeInterval;
}
__int64 CRealModuleCenter::GetTimeIntervalWithSecond( void)
{
	return m_pTimeRealDataModule->GetTimeIntervalWithSecond();
}


// �ǽð� Data�� �߰� �Ǵ� Update ó���Ѵ�.
// Ư�� ���� �ŷ���, �ŷ������ �޴� ���, �߰��� ���� ���� ���� ���Ͽ� ������ �߰��ǵ��� �Ѵ�.
bool CRealModuleCenter::SetAppendOrUpdateData_DataInDate(CPacket* pPacket, const int nBaseCount, const double& dData, double& dPrevData)
{
	if( !pPacket) return false;

	// Base count > Data count -> Data append
	if( 0 < nBaseCount && pPacket->GetCount() < nBaseCount)
	{
		dPrevData = dData;
		// real data �� �����ϴ� ��� -> "�⺻�ŷ���", "�ŷ����"
		if( pPacket->IsChangeRealData())
		{
			dPrevData = 0.0;
			pPacket->GetRealDataGap( dPrevData);
		}
		else
		{
			CList<double, double>* pDataList = pPacket->GetnumericDataList();
			if( pDataList != NULL && pDataList->GetTailPosition() != NULL)
				dPrevData = pDataList->GetTail();
		}

		if( SetAppendData(pPacket, nBaseCount, dData, dPrevData)) return true;
	}

	// Base count <= Data count -> Data update
	if( m_nUpdateOrAppendIndex < -1 || pPacket->GetCount() < nBaseCount)
		return false;

	pPacket->UpdateData( dData, m_nUpdateOrAppendIndex);
	SetPacketMinMaxAndUpdateRage(pPacket);
	return true;
}
// �ǽð� Data �߰���, Base Packet�� Data Count�� ���� ������ �κ��� ���� ������ Append�� �ڿ� Append ó���� �Ѵ�.
bool CRealModuleCenter::SetAppendData( CPacket* pPacket, const int nBaseCount, const double& dData, const double& dPrevData)
{
	if( !pPacket) return false;

	int nCount = pPacket->GetCount();
	if( nBaseCount <= 0 || nBaseCount <= nCount) return false;

	// Base(�ڷ����� or ����)�� ������ ���� ����. Base������������=����data
	for( int nIndex = ( nCount < 0 ? 0 : nCount); nIndex < ( nBaseCount - 1); nIndex++)
		pPacket->AppendData( dPrevData);

	pPacket->AppendData(dData);
	SetPacketMinMaxAndUpdateRage(pPacket);
	return true;
}
// ���� ������ Update�� ó���Ѵ�. (���� ������ ���ų� ���� ��츸 ó���Ѵ�.)
bool CRealModuleCenter::SetUpdateData_Compare(CPacket* pPacket, const int nBaseCount, const double& dData, const bool bLowCheck)
{
	if( pPacket->GetCount() < nBaseCount) return false;

	double dTailData = pPacket->GetnumericDataList()->GetTail();
	// ����(bLowCheck : true), ��(bLowCheck : false) �� ��
	if( bLowCheck && (dTailData <= dData) || !bLowCheck && (dTailData >= dData))
		return false;

	pPacket->UpdateData(dData);
	SetPacketMinMaxAndUpdateRage(pPacket);
	return true;
}
// �ǽð� Data�� ó���ϴ� Main Routine. (����/�ڷ�����/������Ŷ/��Ÿ �� ���еǾ� ó���ȴ�.)
bool CRealModuleCenter::SetRealData(CPacket* pPacket, const double dData, const int nOption, int &p_nRemovePacketDataCount)
{
	if(pPacket == NULL) return false;

	// 0. nOption�� ���� ���� Update(1), �Ǵ� Append(2) ó���Ѵ�.
	if( 0 != nOption)
	{
		//update
		if(			nOption == 1)	pPacket->UpdateData(dData);
		//append
		else if(	nOption == 2)	pPacket->AppendData(dData);
		//min, max setting �ϱ�
		SetPacketMinMaxAndUpdateRage(pPacket);
		return true;
	}

	// 1. �� ������ ���� ���Ѵ�. (RDC�� ���� �������� ���Ѵ�.)
	CString strPacketName = pPacket->GetName();
	double dRealData = m_realDataModule.GetRealData( strPacketName, dData);

	// 2. ���� Packet�� ó�� ~ <����(���簡)> �� ���� ������ ���� ��,��,�� �� �ٲ�� �ȴ�.
	//<< 20100324_JS.Kim �ַθ�.FX	�ð����� ���� ��Ŷ�� �ð�����ó�� ����ϱ� ���� ó��
	//if( pPacket->GetName() == _MTEXT( C0_CLOSE))
	if( strPacketName == _MTEXT( C0_CLOSE) || strPacketName == "_BuyClose_" || strPacketName == "_SellClose_")
	//>>
	{
		// ��/��/�� ���ó��
		double dPrevData = dRealData;
		if( m_pRealBasePacket) SetAppendOrUpdateData_DataInDate( pPacket, m_pRealBasePacket->GetCount(), dRealData, dPrevData);
		// ��/��/���� ���� �����Ͽ� ����ÿ��� ���� ������ ����Ͽ� SetAppendOrUpdateData_DataInDate()�� ������� �ʴ´�.
		int nCloseCount = pPacket->GetCount();
		if( 0 < nCloseCount)
		{
			if( m_pOpenPacket) if( m_pOpenPacket->GetnumericDataList())
				SetAppendData( m_pOpenPacket, nCloseCount, dRealData, dPrevData);

			if( m_pHighPacket)
				if( m_pHighPacket->GetnumericDataList())
					if( !SetAppendData( m_pHighPacket, nCloseCount, dRealData, dPrevData))
						SetUpdateData_Compare( m_pHighPacket, nCloseCount, dRealData, false);

			if( m_pLowPacket)
				if( m_pLowPacket->GetnumericDataList())
					if( !SetAppendData( m_pLowPacket, nCloseCount, dRealData, dPrevData))
						SetUpdateData_Compare( m_pLowPacket, nCloseCount, dRealData, true);
		}
		
		return true;
	}

	// 3. �ڷ������� ó��
	// "�ڷ�����"�� ���Ե� packet : "�ڷ�����", "MP_�ڷ�����"
	if( 0 <= strPacketName.Find( _MTEXT( C0_DATE_TIME)))
	{
		__int64 nRealData = ( __int64)dRealData;

		// (2009/12/11 - Seung-Won, Bae) Check Special Time
		BOOL bSpecialTime = g_iMetaTable.IsSpecialTime( int( nRealData % 1000000));

		// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
		if( nRealData < 1000000)
		{
			if( nRealData + 10000 < m_nLastRealTime % 1000000 && !bSpecialTime)
				m_nLastRealTime = CDataConversion::CalcTimeSum( m_nLastRealTime, 1000000);
			nRealData += m_nLastRealTime - ( m_nLastRealTime % 1000000);
		}

		if( !m_bOnClockedData && !bSpecialTime) SetLastRealTime( nRealData);

		TRACE( "\r\nRealTime : %I64d", nRealData);

		//>>20100520_JS.Kim.		������� �����ϸ鼭 ���� �ִ� ������ ����
		////@Solomon:100114-JunokLee
		//int nDeleteCount = -1;
		//if(m_pTimeRealDataModule) m_pTimeRealDataModule->SetRealData( pPacket, nRealData);
		//if( nDeleteCount < 0) false;
		if(!m_pTimeRealDataModule) return false;

		int nDeleteCount = m_pTimeRealDataModule->SetRealData( pPacket, nRealData);
		if( nDeleteCount < 0) return false;
		//<<

		SetPacketMinMaxAndUpdateRage( pPacket);

		if( 0 < nDeleteCount)
		{
			if( m_pOpenPacket)		m_pOpenPacket->RemoveBaseData(		nDeleteCount);
			if( m_pHighPacket)		m_pHighPacket->RemoveBaseData(		nDeleteCount);
			if( m_pLowPacket)		m_pLowPacket->RemoveBaseData(		nDeleteCount);
			if( m_pClosePacket)		m_pClosePacket->RemoveBaseData(		nDeleteCount);
			if( m_pVolumePacket)	m_pVolumePacket->RemoveBaseData(	nDeleteCount);

			p_nRemovePacketDataCount += nDeleteCount;
		}
		return true;
	}

	// 4. ���� Packet�� ó��
	//		���ĵ� ��ġ�� �˻��Ͽ� Update �Ǵ� Insert�� Index�� �˻��ϰ�(m_nUpdateOrAppendIndex) ó���Ѵ�.
	if( pPacket == m_pRealBasePacket)
	{
		m_nUpdateOrAppendIndex = -1;
		int nIndex = 0;
		CList<double, double>* pDataList = pPacket->GetnumericDataList();
		if( !pDataList) return false;

		POSITION pos = pDataList->GetHeadPosition();
		while(pos != NULL)
		{
			double dOrgData = pDataList->GetNext(pos);
			if( dRealData == dOrgData) //Update
			{
				m_nUpdateOrAppendIndex = nIndex;
				return true;
			}
			else if( dRealData < dOrgData) //Insert
			{
				m_nUpdateOrAppendIndex = nIndex;
				pPacket->AppendData( dRealData, m_nUpdateOrAppendIndex);
				return true;
			}
			nIndex++;
		}

		//AddTail
		pPacket->AppendData(dRealData);
		return false;
	}

	// 5. �Ϲ� Packet�� ó��
	if( m_pRealBasePacket)
	{
		double dPrevData = 0;
		SetAppendOrUpdateData_DataInDate( pPacket, m_pRealBasePacket->GetCount(), dRealData, dPrevData);
	}
	else
	{
		if( m_nUpdateOrAppendIndex >= -1)
		{
			// (2009/10/20 - Seung-Won, Bae) m_nUpdateOrAppendIndex�� ���� Packet�� �ִ� ��쿡�� �̿�Ǵ� ������ �м��ȴ�.
			//	���� ��Ŷ�� ��� �̿���� �ʴ� ��� -1������ �������ٵ�, -1���� ū���� �����ϱ�?
			ASSERT( m_nUpdateOrAppendIndex == -1);

			pPacket->AppendData( dRealData, m_nUpdateOrAppendIndex);
		}
		else
		{
			// (2009/10/20 - Seung-Won, Bae) m_nUpdateOrAppendIndex�� ���� Packet�� �ִ� ��쿡�� �̿�Ǵ� ������ �м��ȴ�.
			//	���� ��Ŷ�� ��� �̿���� �ʴ� ��� -1������ �������ٵ�, -1���� ���� ���� ����?
			ASSERT( FALSE);

			pPacket->UpdateData( dRealData, m_nUpdateOrAppendIndex);
		}
		SetPacketMinMaxAndUpdateRage(pPacket);
	}
	return true;
}
// ����� ���� �ð��帧�� �����Ѵ�. (����ð��� ���� ���� �߰��Ǵ��� ���θ� Return�Ѵ�.)
bool CRealModuleCenter::GetRealData_CloseTime( CPacket* pPacket, const __int64 &p_nClock, __int64 &p_nRealDataTime)
{
	m_nClock = p_nClock;
	if( m_nLastClockTime < 0) m_nLastClockTime = m_nClock;
	if( m_nLastRealTime < 0) return false;
	if( m_nClock == m_nLastRealTime) return false;
//	20121017 �̻�� ����׸�����(5���� �ð����� ���̿�)�� �ǽð� �����͸� ���� �޴� ������ �������� ---->>>>
//	if( m_nClock % 5) return false;
//	---------<<<<<<

	//2012-1012 by alzioyes.
	if( !m_pIChartCtrl->GetNormalizedMinuteTimeRule()) return false;

	m_nLastRealTime = CDataConversion::CalcTimeSum( m_nLastRealTime, m_nClock - m_nLastClockTime, TRUE);
	m_nLastClockTime = m_nClock;

	// Caution! the Clock must be faster than real time not to omit the empty candle.
	//	So the empty candle will be closed before 5 sec of real time.
	p_nRealDataTime = CDataConversion::CalcTimeSum( m_nLastRealTime, 5 - ( m_nLastRealTime % 5));

	TRACE( "CLOCK:%I64d\r\n", p_nRealDataTime);

	if( !m_pTimeRealDataModule->GetRealData_CloseTime( pPacket, p_nRealDataTime)) return false;

	if( CChartInfo::HTS == m_nChartModeSetting || CChartInfo::KRX_CME == m_nChartModeSetting)
		p_nRealDataTime %= 1000000;
	// (2009/12/2 - Seung-Won, Bae) for 31 Hour.
	if( CChartInfo::KRX_CME == m_nChartModeSetting && p_nRealDataTime < 70000)
		p_nRealDataTime += 240000;
		
	return true;
}


// (2009/10/20 - Seung-Won, Bae) Comment. ���� �޽����� ������ ���ڰ��� �����Ѵ�.
// ������ ���������� ���ڷ� �ʱ�ȭ�Ѵ�.
//		�����޽��� DATE ��.
//		�����޽��� DATATIME ��.
//		�����޽��� DATATIME ������ ���� �ŷ��� ���ڸ� �������� ������� ������.
//				�������� �����ð� 0�ø� �������� �ٲ��, �ŷ��� �ð����� ���߿� ����ȴ�.
//				�����ð��� ���������� �������� ��/��/������ ���� ������ �ȴ�.
//"DATE=YYYYMMDD@" ���� "YYYYMMDD"�� ���´�.  -> ���� ���� ��¥�� �޴´�.
void CRealModuleCenter::SetCurrentDate(const CString& strData)
{
//	CString strOldDate = m_dateData.GetDate();
	
	if( strData.IsEmpty())	m_dateData.SetDate();	// PC ��¥�� ����.
	else					m_dateData.SetDate(strData);

// (2009/10/20 - Seung-Won, Bae) It will be done on CRealModuleCenter::InitializeTRPart()
// ��¥�� �ٲ� ��� -> real ������ �ٽ� ���Ѵ�.
//	if( strOldDate != m_dateData.GetDate())
//		SetRealTimeModuleData();
}


// (2009/10/20 - Seung-Won, Bae) MARKETTIME �����޽����� ������ �����, �����ð��� �����Ѵ�.
//sy 2005.01.25. ����۽ð��� �����ð� �ޱ�
//"MARKETTIME=����۽ð�|�帶���ð�@" ���� "����۽ð�|�帶���ð�" �� ���´�.
void CRealModuleCenter::SetMarketTime(const CString& strData)
{
	CString strEndTime = strData;
	if(strEndTime.Find("|") < 0)
		strEndTime += "|";

	CString strStartTime = CDataConversion::GetStringData(strEndTime, "|");
	if(!strStartTime.IsEmpty())
	{
		int nTime = atoi(strStartTime);
		if(nTime > 0)
			m_marketTimeData.SetMarketStartTime(nTime, true);
	}

	if(!strEndTime.IsEmpty())
	{
		int nTime = atoi(strEndTime);
		if(nTime > 0)
			m_marketTimeData.SetMarketEndTime(nTime, true);
	}
}
// X ScaleManager�� �ٸ� �ð����� Scale ó���� ���Ͽ� �����/�����ð��� ��ȸ�Ѵ�.
bool CRealModuleCenter::GetMarketTime(int& nMarketStartTime, int& nMarketEndTime) const
{
	return m_marketTimeData.GetMarketTime(nMarketStartTime, nMarketEndTime);
}


// (2009/10/20 - Seung-Won, Bae) Comment. �к� ó�� Logic Class�� �����Ѵ�.
// �к� ó�� Logic Type�� ���Ѵ�.
CPacketType::REALTYPE CRealModuleCenter::GetRealType( void) const
{
	return m_eRealType;
}
// �к��� �̷�/���� ������ �߷��ϱ� ���Ͽ� �������� ���� ���۽ð��� ��ȸ�Ѵ�.
__int64	CRealModuleCenter::GetStartTimeInTimeRangeOfLastData( void)
{
	if( !m_pTimeRealDataModule) return -1;
	return m_pTimeRealDataModule->GetTimeRangeStartWithPacketType();
}


// (2009/10/21 - Seung-Won, Bae) Comment. ���� �ٸ� �ð��븦 ���ϱ� ���� ��������,
//		���� �ŷ����� ����/�ð��� �����Ѵ�. (��ȸ�����ΰ�?)
// ���� ����+�ð� ( "YYYYMMDDHHMMSS" )
//  �� �ð��� �������� �ü� ����Ÿ�� ���� ������ ó���Ѵ�.
void CRealModuleCenter::SetCurrDateTime(const CString& strData)
{
	if( strData.IsEmpty() )	return;

	if( strData.GetLength() > 8 )
		m_nBasisTime = atoi(strData.Mid(8));

//	{
//		CString szDebug;
//		szDebug.Format("CRealModuleCenter::SetCurrDateTime(%s)", strData);
//		g_DRDebuger.DRLog_Message(szDebug);
//	}

	SetCurrentDate(strData.Left(8));

	//>>20100222_JS.Kim		==>������Ʈ�� ��� CRealModuleCenter::InitializeTRPart()�� Call�� �ȵǹǷ� ���⼭ ���ش�.
	m_nLastRealTime = -1;
	if( 0 <= m_nBasisTime) SetLastRealTime( (__int64)m_dateData.GetFullDate() * 1000000 + (__int64)m_nBasisTime);
	else
	{
		SYSTEMTIME st;
		GetLocalTime( &st);
		__int64 nLocalTime = (__int64)m_dateData.GetFullDate() * 1000000 + st.wHour * 10000 + st.wMinute * 100 + st.wSecond;
		if( CChartInfo::WORLD_ON == m_nChartModeSetting)
			nLocalTime = CDataConversion::CalcTimeSum( nLocalTime, ( m_nOrgBaseTimeDiff - 9) * 10000);
		SetLastRealTime( nLocalTime);
	}
	//<<
// 	if( strData.GetLength() > 8 )
// 	{
// 		// vntsorl:[0000684] �ؿܼ��� ��Ʈ���� 200�к����� ��ȸ�� 19�� ���Ŀ� �������� �����Ǵ� ����
// 		m_nBasisTime = atoi(strData.Mid(8));
// 		
// 		if((m_eRealType == CPacketType::CEILING || m_eRealType == CPacketType::CEILING2) && 
// 			m_nTimeInterval > 1)
// 		{
// 			int nTimeDiff = (m_nTimeInterval / 100) * 10000 + (m_nTimeInterval % 100) * 100 - 1;
// 			m_nBasisTime -= nTimeDiff;
// 			if( m_nBasisTime < 0)
// 				m_nBasisTime = 0;
// 		}
// 	}
// 	
// 	SetCurrentDate(strData.Left(8));

}
// 20080925 JS.Kim	���� �ٸ� �ð����� Scale ó��
__int64 CRealModuleCenter::GetCurrDateTime() const
{
	if( m_nBasisTime < 0) return -1;

	return (__int64)m_dateData.GetFullDate() * 1000000 + (__int64)m_nBasisTime;
}


// (2009/10/21 - Seung-Won, Bae) Comment. 1������ ���� 0�ø� �Ѿ�鼭 �����ϴ����� ��Ÿ���� Type�� �����Ѵ�.
CPacketType::MARKETDAYTYPE CRealModuleCenter::GetMarketDayType()
{
	return m_nMarketDayType;
}


// (2009/10/21 - Seung-Won, Bae) Comment. �ǽð� Data�� Packet Type�� �����Ѵ�.
// �ǽð� Packet �� �ð��� ���� type ( "HHMMSS", "YYYYMMDDHHMMSS" �� )
//"REALTIMET=��ŶŸ��@" ���� "��ŶŸ��" �� ���´�.
void CRealModuleCenter::SetRealTimeType(const CString& strData)
{
	m_strRealTimeType = strData;
}


// (2009/10/21 - Seung-Won, Bae) Comment. �ǽð� ó���� ���� ȯ�氪�� �ʱ�ȭ�Ѵ�.
// �����޽��� ���� �� �ʱ�ȭ
void CRealModuleCenter::InitializeHelpMessage()
{
	// (2009/11/16 - Seung-Won, Bae) Base Time of N-Minite
	m_nNewRangeStartTime = 0;

	//0000603: FX�� 240 �к����� ���� ���¿��� ���� �ü��� ������ �Ʒ��� ���� ���� �ϳ� �� ������ �ʰ� �������� ������Ÿ�� �߰� �Ǵ� ����
	if(CChartInfo::FX == m_nChartModeSetting)
		m_nNewRangeStartTime = 50000;	//5�÷� ����.

	// (2009/10/19 - Seung-Won, Bae) Comment. ������ �ǽð� ���ŵ� DTI
	m_nUpdateOrAppendIndex = -1;

	// (2009/10/19 - Seung-Won, Bae) Comment. ������ Real�� �ݿ��� DTI Range?
	//		��ǥ ���� CMainChartFormulateImplementation::GetDataPoint()���� ��ȸ�Ѵ�.
	//			(������ range�� �̿���� ������ start�θ� �̿�Ǵ� ������ �Ǵܵȴ�.)
	m_ptUpdateRange = CPoint(-1, -1);

	// (2009/10/19 - Seung-Won, Bae) Comment. Real Data�� �����ϴ� Logic Class�̴�.
	//		���� RDC�θ� �̿�ǰ� ����. ER�� ������ RDC = 100 / ER �� ������ �����Ǵ� ����� ���̹Ƿ� RDC�� ������ �ٶ����ϴ�.
	m_realDataModule.SetRealDataModuleType( CRealDataModule::BASE_TYPE);

	// (2009/10/20 - Seung-Won, Bae) Nƽ�� ���� ���������� �� Tick�� ���� (�����޽��� UTEC�� ���� ������)
	m_nAddTimeInterval = 0;

	// (2009/10/19 - Seung-Won, Bae) Comment. UD �����޽����� (�ֱⰪ)�� �����Ѵ�.
	// ���� Ÿ���� �ϴ��� �ְ��� �����Ѵ�
	m_eDateUnitType = CScaleBaseData::HORZ_DAILY;

	// (2009/10/20 - Seung-Won, Bae) UT ������ 1���� �����ϱ� ���� UD�� �����̴�.
	m_nTimeInterval = 1;

	// (2009/10/20 - Seung-Won, Bae) Comment. ���� �޽����� ������ ���ڰ��� �����Ѵ�.
	//		�켱 PC �������� �ʱ�ȭ�Ѵ�.
	m_dateData.SetDate();

	// (2009/10/21 - Seung-Won, Bae) Comment. ���� �ٸ� �ð��븦 ���ϱ� ���� ��������,
	//		���� �ŷ����� ����/�ð��� �����Ѵ�. (��ȸ�����ΰ�?)
	m_nBasisTime = -1;

	// (2009/10/21 - Seung-Won, Bae) Comment. 1������ ���� 0�ø� �Ѿ�鼭 �����ϴ����� ��Ÿ���� Type�� �����Ѵ�.
	m_nMarketDayType = CPacketType::TODAY_ONLY;	//20081029 JS.Kim

	// (2009/11/16 - Seung-Won, Bae) Check for Market Days
	m_strMarketDayInfo.Empty();
}
// �����޽��� �� TR Data ������ �ʱ�ȭ
void CRealModuleCenter::InitializeTRPart( CPacket *p_pOpenPacket, CPacket *p_pHighPacket, CPacket *p_pLowPacket, CPacket *p_pClosePacket, CPacket *p_pVolumePacket)
{
	// (2009/11/1 - Seung-Won, Bae) Support Last Real Time.
	m_nLastRealTime = -1;
	if( 0 <= m_nBasisTime) SetLastRealTime( (__int64)m_dateData.GetFullDate() * 1000000 + (__int64)m_nBasisTime);
	else
	{
		SYSTEMTIME st;
		GetLocalTime( &st);
		//>>20100221_JS.Kim	PC���ڷ� ó������ ���� �����޼����� ���� DATE�� �������.
		//__int64 nLocalTime = st.wYear * 10000000000 + st.wMonth * 100000000 + st.wDay * 1000000 + st.wHour * 10000 + st.wMinute * 100 + st.wSecond;
		__int64 nLocalTime = (__int64)m_dateData.GetFullDate() * 1000000 + st.wHour * 10000 + st.wMinute * 100 + st.wSecond;
		//<<
		if( CChartInfo::WORLD_ON == m_nChartModeSetting)
			nLocalTime = CDataConversion::CalcTimeSum( nLocalTime, ( m_nOrgBaseTimeDiff - 9) * 10000);
		SetLastRealTime( nLocalTime);
	}

	// (2009/10/21 - Seung-Won, Bae) Comment. 1������ ���� 0�ø� �Ѿ�鼭 �����ϴ����� ��Ÿ���� Type�� �����Ѵ�.
	int nMarketStartTime = -1, nMarketEndTime = -1;
	if( m_marketTimeData.GetMarketTime( nMarketStartTime, nMarketEndTime))
		if( nMarketEndTime <= nMarketStartTime)
			m_nMarketDayType = CPacketType::WITH_NEXT_DAY;

	// (2009/11/23 - Seung-Won, Bae) for HTS default value.
	if( CChartInfo::HTS == m_nChartModeSetting && 0 == m_nNewRangeStartTime)
		m_nNewRangeStartTime = nMarketStartTime;

	// (2009/10/19 - Seung-Won, Bae) Real Data Processing Interface.
	m_pOpenPacket	= p_pOpenPacket;
	m_pHighPacket	= p_pHighPacket;
	m_pLowPacket	= p_pLowPacket;
	m_pClosePacket	= p_pClosePacket;
	m_pVolumePacket	= p_pVolumePacket;

	if( !m_pRealBasePacket) return;

	// (2009/10/21 - Seung-Won, Bae) Comment. �ڷ����� PacketType�� �����޾� �ش� Format�� �´� Date�� ������ �����Ѵ�.
	m_dateData.SetPacketType( m_pRealBasePacket->GetType());

	if( m_pRealBasePacket->GetName().Find( _MTEXT( C0_DATE_TIME)) < 0) return;

	//sy 2005.01.28. -> ����۽ð��� �����޽����� ���� �޴´�.
	CString strMarketStartTime;
	if( !m_marketTimeData.GetMarketStartTime( strMarketStartTime)) strMarketStartTime.Empty();

	if( m_pTimeRealDataModule) delete m_pTimeRealDataModule;

	// 2011.02.18 by SYS >> ����Ʈ YYYY(��) ���� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
// 	switch( m_eDateUnitType)
// 	{
// 		// �ϰ�/����
// 		case CScaleBaseData::HORZ_MONTHLY:
// 		case CScaleBaseData::HORZ_DAILY:	m_pTimeRealDataModule = new CDateRealDataModule( m_marketTimeData, m_pRealBasePacket->GetType(), CScaleBaseData::HORZ_MONTHLY == m_eDateUnitType);
// 											break;
// 		// �ְ�
// 		case CScaleBaseData::HORZ_WEEKLEY:	m_pTimeRealDataModule = new CWeekRealDataModule( m_marketTimeData, m_pRealBasePacket->GetType());
// 											break;
// 		// �ð�(��)
// 		case CScaleBaseData::HORZ_TIME_TWO:
// 		case CScaleBaseData::HORZ_TIME_ONE:	
// 			switch( m_eRealType)
// 			{
// 				// ����. 09:01:00 ~ 09:01:59 -> 09:01
// 				case CPacketType::FLOOR:	m_pTimeRealDataModule = new CFloorTimeRealDataModule( m_dateData, m_marketTimeData, m_pRealBasePacket->GetType(), m_nTimeInterval, strMarketStartTime);
// 											break;
// 				// �ø�. 09:00:01 ~ 09:01:00 -> �޸���
// 				default:
// 				case CPacketType::CEILING:	m_pTimeRealDataModule = new CCeilingTimeRealDataModule( m_dateData, m_marketTimeData, m_pRealBasePacket->GetType(), m_nTimeInterval, strMarketStartTime);
// 											break;
// 				// �ø�. 09:00:00 ~ 09:00:59 -> 09:01 (KOSCOM)	// (2009/6/15 - Seung-Won, Bae) Koscom is new ceiling type.
// 				case CPacketType::CEILING2:	m_pTimeRealDataModule = new CCeiling2TimeRealDataModule( m_dateData, m_marketTimeData, m_pRealBasePacket->GetType(), m_nTimeInterval, strMarketStartTime);
// 											break;
// 			}
// 			break;
// 		// ƽ
// 		default:
// 		case CScaleBaseData::HORZ_TICK:		m_pTimeRealDataModule = new CTickRealDataModule( m_marketTimeData, m_pRealBasePacket->GetType(), m_nTimeInterval, m_nAddTimeInterval);
// 											break;
// 	}
	
	switch( m_eDateUnitType)
	{
		// �ϰ�/����
		case CScaleBaseData::HORZ_MONTHLY:
		case CScaleBaseData::HORZ_DAILY:	m_pTimeRealDataModule = new CDateRealDataModule( m_marketTimeData, m_pRealBasePacket->GetType(), m_eDateUnitType, CScaleBaseData::HORZ_MONTHLY == m_eDateUnitType);
											break;
		// �ְ�
		case CScaleBaseData::HORZ_WEEKLEY:	m_pTimeRealDataModule = new CWeekRealDataModule( m_marketTimeData, m_pRealBasePacket->GetType(), m_eDateUnitType);
											break;
		// �ð�(��)
		case CScaleBaseData::HORZ_TIME_TWO:
		case CScaleBaseData::HORZ_TIME_ONE:	
			switch( m_eRealType)
			{
				// ����. 09:01:00 ~ 09:01:59 -> 09:01
				case CPacketType::FLOOR:	m_pTimeRealDataModule = new CFloorTimeRealDataModule( m_dateData, m_marketTimeData, m_pRealBasePacket->GetType(), m_nTimeInterval, strMarketStartTime, m_eDateUnitType);
											break;
				// �ø�. 09:00:01 ~ 09:01:00 -> �޸���
				default:
				case CPacketType::CEILING:	m_pTimeRealDataModule = new CCeilingTimeRealDataModule( m_dateData, m_marketTimeData, m_pRealBasePacket->GetType(), m_nTimeInterval, strMarketStartTime, m_eDateUnitType);
											break;
				// �ø�. 09:00:00 ~ 09:00:59 -> 09:01 (KOSCOM)	// (2009/6/15 - Seung-Won, Bae) Koscom is new ceiling type.
				case CPacketType::CEILING2:	m_pTimeRealDataModule = new CCeiling2TimeRealDataModule( m_dateData, m_marketTimeData, m_pRealBasePacket->GetType(), m_nTimeInterval, strMarketStartTime, m_eDateUnitType);
											break;
			}
			break;
		// ƽ
		default:
		case CScaleBaseData::HORZ_TICK:		m_pTimeRealDataModule = new CTickRealDataModule( m_marketTimeData, m_pRealBasePacket->GetType(), m_nTimeInterval, m_nAddTimeInterval, m_eDateUnitType);
											break;
	}
	// 2011.02.18 by SYS <<

	// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
	m_pTimeRealDataModule->SetChartMode( m_nChartModeSetting);

	m_pTimeRealDataModule->OnInitTR( ( m_nBaseTimeDiff - m_nOrgBaseTimeDiff) * 10000, m_nNewRangeStartTime, m_strMarketDayInfo);

	double dTailData = -1;
	CList< double, double>* pDataList = m_pRealBasePacket->GetnumericDataList();
	if( pDataList) if( 0 < pDataList->GetCount())
		m_pTimeRealDataModule->SetTimeRange_TRPart( m_nLastRealTime, ( __int64)pDataList->GetTail());
}


// (2009/11/1 - Seung-Won, Bae) for Normalized Candle data.
double CRealModuleCenter::GetOrgDataBeforeRate( const char *p_szPacketName, const double p_dRatedData)
{
	return m_realDataModule.GetOrgData( p_szPacketName, p_dRatedData);
}


// (2009/11/3 - Seung-Won, Bae) Make Market Date to Real Date of WorldOn Real Data.
void CRealModuleCenter::GetRealData_RealDateTime( double &p_dData) const
{
	// (2009/12/11 - Seung-Won, Bae) for No Date Real Time.
	if( p_dData < 1000000) return;

	if( !m_marketTimeData.IsReverseTime()) return;

	// X ScaleManager�� �ٸ� �ð����� Scale ó���� ���Ͽ� �����/�����ð��� ��ȸ�Ѵ�.
	int nMarketStartTime = -1, nMarketEndTime = -1;
	if( !m_marketTimeData.GetMarketTime( nMarketStartTime, nMarketEndTime)) return;

	if( __int64( p_dData) % 1000000 < nMarketStartTime) return;
	p_dData = ( double)CDataConversion::CalcTimeSum( ( __int64)p_dData, -1000000);
}


// 20080925 JS.Kim	���� �ٸ� �ð����� Scale ó��
void CRealModuleCenter::SetTimeDiffInfo( const int &p_nOrgBaseTimeDiff, const int &p_nBaseTimeDiff)
{
	m_nOrgBaseTimeDiff = p_nOrgBaseTimeDiff;
	m_nBaseTimeDiff	= p_nBaseTimeDiff;
	if( m_pTimeRealDataModule) m_pTimeRealDataModule->SetRealTimeDiff( ( m_nBaseTimeDiff - m_nOrgBaseTimeDiff) * 10000);
}


// (2009/11/1 - Seung-Won, Bae) Support Last Real Time.
void CRealModuleCenter::SetLastRealTime( const __int64 &p_nRealTime)
{
	m_nLastClockTime = m_nClock;
	m_nLastRealTime = p_nRealTime;
}

// (2009/11/4 - Seung-Won, Bae) Backup to receive NMTR Data.
void CRealModuleCenter::PushNMTRState( __int64 p_nRealDataTime)
{
	m_bOnClockedData = TRUE;
	m_nLastRealTimeBackup = m_nLastRealTime;
	if( CChartInfo::HTS == m_nChartModeSetting || CChartInfo::KRX_CME == m_nChartModeSetting)
	{
		// (2009/12/2 - Seung-Won, Bae) for 31 Hour.
		if( CChartInfo::KRX_CME == m_nChartModeSetting && 240000 <= p_nRealDataTime)
			p_nRealDataTime -= 240000;

		if( m_nLastRealTime % 1000000 < p_nRealDataTime)
			m_nLastRealTime = CDataConversion::CalcTimeSum( m_nLastRealTime, -1000000);
		m_nLastRealTime += p_nRealDataTime - m_nLastRealTime % 1000000;
	}
	else m_nLastRealTime = p_nRealDataTime;
	m_pTimeRealDataModule->PushNMTRState();
}
void CRealModuleCenter::PopNMTRState( void)
{
	m_pTimeRealDataModule->PopNMTRState();
	m_nLastRealTime = m_nLastRealTimeBackup;
	m_bOnClockedData = FALSE;
}

// (2009/11/12 - Seung-Won, Bae) Get NMTR Info
BOOL CRealModuleCenter::GetLastClockAndRealTime( __int64 &p_nLastClockTime, __int64 &p_nLastRealTime)
{
	p_nLastClockTime = m_nLastClockTime;
	p_nLastRealTime = m_nLastRealTime;
	return TRUE;
}

// (2009/11/16 - Seung-Won, Bae) Base Time of N-Minite
void CRealModuleCenter::SetNewRangeStartTime( const CString &p_strTime)
{
	m_nNewRangeStartTime = atoi( p_strTime);
}
