// TimeRealDataModule.cpp: implementation of the CTimeRealDataModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TimeRealDataModule.h"

#include "../Include_Chart/DLL_Load/PacketBase.h"			// for CPacket
#include "../Include_Chart/DLL_Load/IMetaTable.h"			// for g_iMetaTable

#include "Conversion.h"
#include "PacketBaseData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// class CTimeRealDataModuleBase

// (2009/10/28 - Seung-Won, Bae) Real �ð� ó�� Logic Class�� �ֻ��� Base
// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
CTimeRealDataModuleBase::CTimeRealDataModuleBase( CMarketTimeData &p_marketTimeData, const CString& strPacketType, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType)
	: m_marketTimeData( p_marketTimeData)
{
	// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	m_eDateUnitType = eDateUnitType;

	// (2009/10/21 - Seung-Won, Bae) Comment. �ڷ����� Packet�� Data Type�� �����Ѵ�. (ex : "YYYYMMDDHHMMSS")
	m_ttcTrPacket.SetPacketType( strPacketType);
	
	// ����� ���� �ð��帧�� �����Ѵ�. (����ð��� ���� ���� �߰��Ǵ��� ���θ� Return�Ѵ�.)
	m_nClockedDataCount = 0;

	// 20080925 JS.Kim	���� �ٸ� �ð����� Scale ó��
	m_nRealTimeDiff = 0;
	m_nNewRangeStartTime = 0;

	// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
	m_nChartModeSetting = CChartInfo::HTS;

	// (2009/11/4 - Seung-Won, Bae) Empty Candle Closing Time Type.
	m_bCloseEmptyCandleWhenEnd = TRUE;
}
CTimeRealDataModuleBase::~CTimeRealDataModuleBase()
{
}

void CTimeRealDataModuleBase::OnInitTR( const __int64 &p_nRealTimeDiff, const int &p_nNewRangeStartTime, const CString &p_szMarketDayInfo)
{
	m_nRealTimeDiff = p_nRealTimeDiff;
	m_nNewRangeStartTime = p_nNewRangeStartTime;

	// (2009/11/16 - Seung-Won, Bae) Check for Market Days
	m_mapAddedMarketDays.RemoveAll();
	m_mapRemovedMarketDays.RemoveAll();
	if( !p_szMarketDayInfo.IsEmpty())
	{
		CString strBuffer;
		char *szBuffer = strBuffer.GetBufferSetLength( p_szMarketDayInfo.GetLength());
		strcpy( szBuffer, p_szMarketDayInfo);
		char *szNext = szBuffer;

		do
		{
			szNext = strchr( szNext, ',');
			if( szNext) *szNext = '\0';
			long lDay = atol( szBuffer);
			if( lDay < 0) m_mapRemovedMarketDays.SetAt( ( void *)( -lDay), NULL);
			if( 0 < lDay) m_mapAddedMarketDays.SetAt( ( void *)lDay, NULL);
			if( szNext)
			{
				szNext++;
				szBuffer = szNext;
			}
		} while( szNext);
	}
}

// (2009/11/4 - Seung-Won, Bae) Calculate Market Date.
void CTimeRealDataModuleBase::GetMarketDate_DateTime( __int64 &p_nData) const
{
	if( !m_marketTimeData.IsReverseTime()) return;

	// X ScaleManager�� �ٸ� �ð����� Scale ó���� ���Ͽ� �����/�����ð��� ��ȸ�Ѵ�.
	int nMarketStartTime = -1, nMarketEndTime = -1;
	if( !m_marketTimeData.GetMarketTime( nMarketStartTime, nMarketEndTime)) return;

	//<<20100511_JS.Kim �ַθ�. �ַθ��� ����Ÿ �ð��� �ŷ��� �ð� ������ �ƴ� �ѱ������̴�
	// ���� �ڵ��� �ϸ� �Ϻ����� �ǽð� ������ ���� ���ڷ� ���� �ϳ� �����...
	//if( CChartInfo::WORLD_ON == m_nChartModeSetting && nMarketStartTime <= p_nData % 1000000)
	if( CChartInfo::WORLD_ON == m_nChartModeSetting && nMarketStartTime > p_nData % 1000000)
		p_nData = CDataConversion::CalcTimeSum( p_nData, -1000000);
	else if( ( CChartInfo::FX == m_nChartModeSetting || CChartInfo::KRX_CME == m_nChartModeSetting) && p_nData % 1000000 < nMarketStartTime)
		p_nData = CDataConversion::CalcTimeSum( p_nData, -1000000);
}

// (2009/11/16 - Seung-Won, Bae) Check for Market Days
BOOL CTimeRealDataModuleBase::IsMarketDay( int p_nDate)
{
	tm tmMarket = { 0, 0, 0, 0, 0, 0, 0, 0, -1};	//[A00000554]alzioyes:20110530 �ð����� �ʱⰪ����.
	tmMarket.tm_mday = int( p_nDate % 100);
	int nMonth = int( p_nDate / 100);
	tmMarket.tm_mon = nMonth % 100 - 1;
	tmMarket.tm_year = nMonth / 100 - 1900;
	mktime( &tmMarket);
	void *pTemp = NULL;
	if( 0 == tmMarket.tm_wday || 6 == tmMarket.tm_wday)
	{
		if( !m_mapAddedMarketDays.Lookup( ( void *)p_nDate, pTemp)) return FALSE;
	}
	else
	{
		if( m_mapRemovedMarketDays.Lookup( ( void *)p_nDate, pTemp)) return FALSE;
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// class CTickRealDataModule

// (2009/10/28 - Seung-Won, Bae) Tick ó�� Logic Class
// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
CTickRealDataModule::CTickRealDataModule( CMarketTimeData &p_marketTimeData, const CString& strPacketType, const int nTimeInterval, const int nAddTimeInterval, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType)
	: CTimeRealDataModuleBase( p_marketTimeData, strPacketType, eDateUnitType)
{
	// (2009/10/21 - Seung-Won, Bae) Comment. Nƽ ���� ���� ƽ�� �����Ѵ�. (UT��)
	m_nTimeInterval = nTimeInterval;	// �а���
	if( m_nTimeInterval < 1) m_nTimeInterval = 1;

	// (2009/10/20 - Seung-Won, Bae) Nƽ�� ���� ���������� �� Tick�� ���� (�����޽��� UTEC�� ���� ������)
	m_nAddTimeInterval = nAddTimeInterval;
}
CTickRealDataModule::~CTickRealDataModule()
{
}

// (2009/10/22 - Seung-Won, Bae) Comment. ���ŵ� �ǽð��� �ø�/���� ���� ���Ͽ� �� Logic Class�� ���� Packet�� �ݿ� ó���Ѵ�.
int CTickRealDataModule::SetRealData( CPacket* pPacket, const __int64 &nRealTime)
{
	// 3.3.3. Nƽ�� ���� Counter�� ������Ų��.
	//	������ ���� �� ƽ�� ������ m_nTimeInterval�� ���� ������ ���ο� ���� ������� �Ѵ�.
	m_nAddTimeInterval++;
	if( m_nTimeInterval < m_nAddTimeInterval)
		m_nAddTimeInterval = 1;

	// (2009/11/3 - Seung-Won, Bae) User Time.
	__int64 nUserTime = nRealTime;
	if( 0 != m_nRealTimeDiff) nUserTime = CDataConversion::CalcTimeSum( nUserTime, m_nRealTimeDiff);

	if( m_nTimeInterval <= 1 || m_nAddTimeInterval <= 1)
			pPacket->AppendData( ( double)m_ttcTrPacket.GetPacketTypeTime( nUserTime));
	else	pPacket->UpdateData( ( double)m_ttcTrPacket.GetPacketTypeTime( nUserTime));

	return 0;
}


//////////////////////////////////////////////////////////////////////
// class CDateRealDataModule

// (2009/10/28 - Seung-Won, Bae) Date(��/��) ó�� Logic Class
// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
CDateRealDataModule::CDateRealDataModule( CMarketTimeData &p_marketTimeData, const CString& strPacketType, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType, BOOL p_bMonthChart)
	: CTimeRealDataModuleBase( p_marketTimeData, strPacketType, eDateUnitType)
{
	m_bMonthChart = p_bMonthChart;
}
CDateRealDataModule::~CDateRealDataModule()
{
}

// (2009/10/22 - Seung-Won, Bae) Comment. ���ŵ� �ǽð��� �ø�/���� ���� ���Ͽ� �� Logic Class�� ���� Packet�� �ݿ� ó���Ѵ�.
int CDateRealDataModule::SetRealData( CPacket* pPacket, const __int64 &nRealTime)
{
	if( !pPacket) return -1;
	const CList< double, double> *pDateList = pPacket->GetnumericDataList();
	if( !pDateList) return -1;

	// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	//__int64 nRealDate = nRealTime;
	__int64 nRealDate = 0;
	if (m_ttcTrPacket.GetPacketType() == _T("YYYYMMDDHHMMSS"))
	{
		if (m_bMonthChart)
			nRealDate = nRealTime / 100000000;	// ������ ���
		else
			nRealDate = nRealTime / 1000000;	// �ϰ��� ���.
	}
	else
		nRealDate = nRealTime;
	// 2011.01.21 by SYS <<

	// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
	if( CChartInfo::WORLD_ON == m_nChartModeSetting || CChartInfo::FX == m_nChartModeSetting)
		GetMarketDate_DateTime( nRealDate);

	nRealDate = m_ttcTrPacket.GetPacketTypeTime( nRealDate);

	// ����� ���� �ð��帧�� �����Ѵ�. (����ð��� ���� ���� �߰��Ǵ��� ���θ� Return�Ѵ�.)
	int nDeleteCount = 0;
	int nEndDate = -1;
	POSITION psnDate = pDateList->GetTailPosition();
	while( 0 < m_nClockedDataCount - nDeleteCount && psnDate)
	{
		nEndDate = ( int)pDateList->GetPrev( psnDate);
		if( nEndDate < nRealDate) break;
		nDeleteCount++;
	}
	if( 0 < nDeleteCount) 
		pPacket->RemoveBaseData( nDeleteCount);
	m_nClockedDataCount = 0;

	// 3.5. �ϰ� ó��
	//		m_dateData�� �������� ���ڿ� ��ȸ�� ������ ���ڰ� �ٸ� ��츸 Append ó�� (Update�� ���ǹ�)
	//		N���� ��쵵 ���� �к��� ����Ʈ�� �ø����� ó���Ͽ� ��� ���� ���ڸ� ǥ���Ѵ�.
	//			�ٸ� 24�ð� ������ ��츸 �����ȴ�.
	nEndDate = -1;
	if( 0 < pPacket->GetCount()) 
		nEndDate = ( int)pPacket->GetnumericDataList()->GetTail();

	// ������ TR�� �ް� �ǽð��� ó�� ���� ��� (TR data�� ���� ��¥�� ���� ���)
	if( nEndDate < nRealDate) 
		pPacket->AppendData( ( double)nRealDate);

	return nDeleteCount;
}

// (2009/11/22 - Seung-Won, Bae) ��/������ ����� ���� Update ����� �ʿ����. (��ȸ ������ �ʿ����.)
//	����, ��/�� ������Ŀ� ���� ������ ��/�� ��, �Ǵ� ���� ��/�� ������ ���Ͽ� ���� �����͸� �߻���Ų��.
//	�̶� �Ϻ��� ��� �������� Ȯ���Ͽ��� �Ѵ�.
bool CDateRealDataModule::GetRealData_CloseTime( CPacket* pPacket, __int64 &p_nRealDataTime)
{
	// 1. Get the Last Data
	if( pPacket->GetCount() <= 0) 
		return false;

	__int64 nEndDate = ( __int64)pPacket->GetnumericDataList()->GetTail();

	// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	//nEndDate = m_ttcTrPacket.GetFullTypeTime( nEndDate);
	if (m_ttcTrPacket.GetPacketType() == "YYYYMMDDHHMMSS")
	{
		if (m_bMonthChart)
			nEndDate = nEndDate * 100000000;	// ������ ���
		else
			nEndDate = nEndDate * 1000000;		// �ϰ��� ���.
	}
	else
		nEndDate = m_ttcTrPacket.GetFullTypeTime(nEndDate);
	// 2011.01.21 by SYS <<

	// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
	__int64 nRealDateTime = p_nRealDataTime;
	GetMarketDate_DateTime( nRealDateTime);

	__int64 nRealDate = nRealDateTime / 1000000;
	if( m_bMonthChart)
	{
		if( m_bCloseEmptyCandleWhenEnd)
			nEndDate = CDataConversion::CalcTimeSum( nEndDate, 100000000);
		if( nRealDate / 100 <= nEndDate / 100000000) 
			return false;
	}
	else
	{
		do
		{
			nEndDate = CDataConversion::CalcTimeSum( nEndDate, 1000000);
		}
		while( !IsMarketDay( int( nEndDate / 1000000)));

		if( !m_bCloseEmptyCandleWhenEnd)
			nEndDate = CDataConversion::CalcTimeSum( nEndDate, -1000000);

		if( nRealDate <= nEndDate / 1000000) 
			return false;
	}

	if( CChartInfo::WORLD_ON == m_nChartModeSetting) 
		p_nRealDataTime = nRealDateTime;
//<<�ַθ�... �ؿܼ��� �Ϻ� ��ȸ�� �׾ ���� 
	//==> �Ͽ��ٰ� ������. �� ���� ������ ��� ������ �ȵȴٰ� ��. �׾��� ������ ����Ÿ ������ ������(��¿�����)
	if( m_bCloseEmptyCandleWhenEnd)
//	else if( m_bCloseEmptyCandleWhenEnd)
//>>
		p_nRealDataTime = CDataConversion::CalcTimeSum( p_nRealDataTime, -1000000);

	m_nClockedDataCount++;

	return true;
}

// (2009/11/12 - Seung-Won, Bae) for NMTR Step Width
__int64 CDateRealDataModule::GetTimeIntervalWithSecond( void)
{
	if( m_bMonthChart) return __int64( 60. * 60. * 24. * 30.);
	return __int64( 60. * 60. * 24.);
}

//////////////////////////////////////////////////////////////////////
// class CWeekRealDataModule

// (2009/10/28 - Seung-Won, Bae) Week(��) ó�� Logic Class
// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
CWeekRealDataModule::CWeekRealDataModule( CMarketTimeData &p_marketTimeData, const CString& strPacketType, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType)
	: CTimeRealDataModuleBase( p_marketTimeData, strPacketType, eDateUnitType)
{
}
CWeekRealDataModule::~CWeekRealDataModule()
{
}


// (2009/11/19 - Seung-Won, Bae) ��ȸ�� ������ Data�� �ǽð� ���� ����� �������� ���Ѵ�.
//	�� �������� �������� �ǽð� ���Ž� ������ Data�� Update�� ������ Append�� ������ �����ȴ�.
//		Ư�� Append�ÿ��� ���� ������ ������Ű�� Update���� ó���ȴ�.
//		���� ������ ���� Update�ϱ� ���� �ǽð� �������� �ð������� ���ϴ� ���̴�.
//	�� ������ ������ Data������ ���� ���۱��ؽ÷� �⺻ ������ �����ϰ�, ������ Data�� �ǽð� Data�� ��ó�� ���ԵǴ� �������� �˻��Ѵ�.
//	������ Data ��ü�� ���� ���, ������ �������� �ʰ�, ù �ǽð��� ���� ��, ���� ���� ���� ������� ���Ѵ�.
//		���� �̷��� ������ �������� ���� ��Ȳ������ ����� ������ �� ������,
//		ù �Ǻ� ���Žô� ������ Append�� ó���� ������ �����Ѵ�.
BOOL CWeekRealDataModule::SetTimeRange_TRPart( const __int64 &p_nServerTime, __int64 nTailTime)
{
	// (2009/11/13 - Seung-Won, Bae) �߰��� ����� RollBack ��Ű��.
	//	�߰��� ����� ���� ��ŭ Ȯ���ϸ鼭, ������ ��� ������ ������Ű�鼭 �����ؾ��� Data ������ ���Ѵ�.
	//  �� �������(m_bCloseEmptyCandleWhenEnd:TRUE)�� ù���� �Ǻ��� ������Ű�Ƿ�,
	//		��� Data ������ -1���� ���۵ǵ��� �Ѵ�.
	if( m_bCloseEmptyCandleWhenEnd && 0 <= nTailTime)	m_nClockedDataCount = -1;
	else												m_nClockedDataCount = 0;
	return TRUE;
}


// (2009/11/23 - Seung-Won, Bae) �ֺ��� �ǽð� ������ ���ŵ� �ǽð� �����Ϳ� �߰��� ������ �����Ͱ� ����
//		���� �������� ���� �������� ���θ� Ȯ���Ͽ� Append/Update ���θ� �����Ѵ�.
//	�̶�, Append�� ��� �߰��� ������ �����Ͱ� ���� ������ �������ڷ� Update�� �����Ų��.
int CWeekRealDataModule::SetRealData( CPacket* pPacket, const __int64 &nRealTime)
{
	if( !pPacket) return -1;
	const CList< double, double> *pDateList = pPacket->GetnumericDataList();
	if( !pDateList) return -1;

	// for Date Check.
	// 2011.01.24 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	//__int64 nDateOfReal = nRealTime;
	__int64 nDateOfReal = 0;
	if (m_ttcTrPacket.GetPacketType() == _T("YYYYMMDDHHMMSS"))
		nDateOfReal = nRealTime / 1000000;
	else
		nDateOfReal = nRealTime;
	// 2011.01.24 by SYS <<

	GetMarketDate_DateTime( nDateOfReal);
	__int64 nLastMondayDateOfReal = GetLastMondayDate( nDateOfReal);

	// ����� ���� �ð��帧�� �����Ѵ�. (����ð��� ���� ���� �߰��Ǵ��� ���θ� Return�Ѵ�.)
	int nDeleteCount = 0;
	__int64 nEndDate = -1;
	POSITION psnDate = pDateList->GetTailPosition();
	while( 0 < m_nClockedDataCount - nDeleteCount && psnDate)
	{
		nEndDate = ( __int64)pDateList->GetPrev( psnDate);
		if( GetLastMondayDate( m_ttcTrPacket.GetFullTypeTime( nEndDate)) < nLastMondayDateOfReal) 
			break;
		nDeleteCount++;
	}
	if( 0 < nDeleteCount) pPacket->RemoveBaseData( nDeleteCount);
	m_nClockedDataCount = ( m_bCloseEmptyCandleWhenEnd ? -1 : 0);

	if( pPacket->GetCount() <= 0)
		pPacket->AppendData( ( double)m_ttcTrPacket.GetPacketTypeTime( nDateOfReal));
	else
	{
		nEndDate = ( __int64)pPacket->GetnumericDataList()->GetTail();
		nEndDate = GetLastMondayDate( m_ttcTrPacket.GetFullTypeTime( nEndDate));
		if( nLastMondayDateOfReal <= nEndDate)
			pPacket->UpdateData( ( double)m_ttcTrPacket.GetPacketTypeTime( nDateOfReal));
		else
		{
			nEndDate *= 1000000;
			__int64 nLastMarketDay = -1;

			// (2009/11/23 - Seung-Won, Bae) WorldOn close with first monday.
			if( CChartInfo::WORLD_ON == m_nChartModeSetting)
				nLastMarketDay = nEndDate;
			else
			{
				nEndDate = CDataConversion::CalcTimeSum( nEndDate, 6000000);
				for( int i = 0; i < 7; i++)
				{
					if( IsMarketDay( int( nEndDate / 1000000)))
					{
						nLastMarketDay = nEndDate;
						break;
					}
					nEndDate = CDataConversion::CalcTimeSum( nEndDate, -1000000);
				}
			}


			if( 0 < nLastMarketDay)
				pPacket->UpdateData( ( double)m_ttcTrPacket.GetPacketTypeTime( nLastMarketDay));
			pPacket->AppendData( ( double)m_ttcTrPacket.GetPacketTypeTime( nDateOfReal));
		}
	}

	return nDeleteCount;
}

// (2009/10/29 - Seung-Won, Bae) Get the last monday date.
int CWeekRealDataModule::GetLastMondayDate( __int64 p_nTime)
{
	tm tmSum;
	tmSum.tm_isdst = -1;			//[A00000554]alzioyes:20110530 �ð����� �ʱⰪ����.

	tmSum.tm_sec = 0;
	tmSum.tm_min = 0;
	tmSum.tm_hour = 0;
	p_nTime /= 1000000;

	tmSum.tm_mday = int( p_nTime % 100);
	p_nTime /= 100;
	tmSum.tm_mon = int( p_nTime % 100) - 1;
	p_nTime /= 100;
	tmSum.tm_year = int( p_nTime) - 1900;

	tmSum.tm_wday = 0;
	tmSum.tm_yday = 0;
	//tmSum.tm_isdst = 0;			//[A00000554]alzioyes:20110530 �ð����� �ʱⰪ����.
	mktime( &tmSum);

	if( 1 != tmSum.tm_wday)
	{
		if( tmSum.tm_wday < 1) tmSum.tm_wday += 7;
		tmSum.tm_mday += 1 - tmSum.tm_wday;
		mktime( &tmSum);
	}

	int nDate = tmSum.tm_year + 1900;
	nDate *= 100;
	nDate += tmSum.tm_mon + 1;
	nDate *= 100;
	nDate += tmSum.tm_mday;
	return nDate;
}

// (2009/11/22 - Seung-Won, Bae) �ֺ��� ����� ���� Update ����� �ʿ��ϴ�.
//		�׷���, ��Ȯ�ϰ� �ְ� �����ڿ� Update���� ������ ���߿� ��� Update�ϴ� �������� ����� �ȴ�.
//			(������������ ��ܷ� �ǽð��� ������ ��� ��)
//	�� ��������� ���, ������ ������ ���� �������� ù ������ ������ �Ѿ������ Ȯ���ϸ�,
//		���� �ð��� ������ ���ؽ÷� �����Ѵ�. (�� �ð����� ������ �������� �ְ� �ٲ�� �ݺ� �۾��� �߻����� �ʴ´�.)
//	�� ��������� ���, ������ ������ ���� ����-�������� ������ ������ �Ѿ������ Ȯ���ϸ�,
//		�ָ� ������ �������ڸ� �� ���� ���ؽ÷� �����Ѵ�. (��Ȯ�� �ְ� �������� �۾����� �ݺ� �۾��� �߻����� �ʴ´�.)
//		Ư�� ù����� ��� (������� -1), ����-�����ְ� �ƴ� ������ ���������� ������ ������ ������ �Ѿ������ Ȯ���Ѵ�.
bool CWeekRealDataModule::GetRealData_CloseTime( CPacket* pPacket, __int64 &p_nRealDataTime)
{
	int nMarketStartTime = -1, nMarketEndTime = -1;
	if( !m_marketTimeData.GetMarketTime( nMarketStartTime, nMarketEndTime)) return false;

	if( pPacket->GetCount() <= 0) 
		return false;
	
	__int64 nEndDate = ( __int64)pPacket->GetnumericDataList()->GetTail();

	// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	//	__int64 nTempDate = GetLastMondayDate( m_ttcTrPacket.GetFullTypeTime( nEndDate));
	//	nEndDate = nTempDate * 1000000 + nMarketStartTime;
	if (m_ttcTrPacket.GetPacketType() == "YYYYMMDDHHMMSS")
		nEndDate = nEndDate * 1000000;
	else
		nEndDate = m_ttcTrPacket.GetFullTypeTime(nEndDate);
	nEndDate = nEndDate + nMarketStartTime;
	// 2011.01.21 by SYS <<

	//<<20100511_JS.Kim �ַθ�. �ַθ��� ����Ÿ �ð��� �ŷ��� �ð� ������ �ƴ� �ѱ������̴�
	//			��ð��� ������ ��� �� ������ �Ϸ縦 ���ִ���????
	//if( CChartInfo::WORLD_ON == m_nChartModeSetting && m_marketTimeData.IsReverseTime())
	//	nEndDate = CDataConversion::CalcTimeSum( nEndDate, -1000000);
	//>>

	if( nEndDate < 0) return false;

	if( m_bCloseEmptyCandleWhenEnd)
	{
		if( 0 <= m_nClockedDataCount)
			nEndDate = CDataConversion::CalcTimeSum( nEndDate, 7000000);
		nEndDate = CDataConversion::CalcTimeSum( nEndDate, 7000000);

		if( p_nRealDataTime <= nEndDate) 
			return false;

		__int64 nLastMarketDay = -1;
		if( CChartInfo::WORLD_ON == m_nChartModeSetting)
		{
			nLastMarketDay = CDataConversion::CalcTimeSum( nEndDate, -7000000);
			GetMarketDate_DateTime( nLastMarketDay);
		}
		else
		{
			for( int i = 0; i < 7; i++)
			{
				nEndDate = CDataConversion::CalcTimeSum( nEndDate, -1000000);
				if( IsMarketDay( int( nEndDate / 1000000)))
				{
					nLastMarketDay = nEndDate;
					break;
				}
			}
		}
		if( nLastMarketDay < 0) return false;
		p_nRealDataTime = nLastMarketDay;
	}
	else
	{
		nEndDate = CDataConversion::CalcTimeSum( nEndDate, 7000000);
		__int64 nLastMarketDay = -1;
		for( int i = 1; i < 7; i++)
		{
			if( IsMarketDay( int( nEndDate / 1000000)))
			{
				nLastMarketDay = nEndDate;
				break;
			}
			nEndDate = CDataConversion::CalcTimeSum( nEndDate, 1000000);
		}
		if( nLastMarketDay < 0) return false;
		nLastMarketDay = CDataConversion::CalcTimeSum( nLastMarketDay, -1000000);

		if( p_nRealDataTime / 1000000 <= nLastMarketDay / 1000000) return false;
	}

	m_nClockedDataCount++;
	return true;
}

// (2009/11/12 - Seung-Won, Bae) for NMTR Step Width
__int64 CWeekRealDataModule::GetTimeIntervalWithSecond( void)
{
	return __int64( 60. * 60. * 24. * 7.);
}


//////////////////////////////////////////////////////////////////////
// class CTimeRealDataModule

// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
CTimeRealDataModule::CTimeRealDataModule( const CDateData& dateData, CMarketTimeData &p_marketTimeData, const CString& strPacketType, int nTimeInterval, const CString& strMarketStartTime, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType)
	:	CTimeRealDataModuleBase( p_marketTimeData, strPacketType, eDateUnitType),
		m_dateData(dateData),
		m_nTimeRangeStart( 0),
		m_nTimeRangeEnd( 0)
{
	// (2009/10/21 - Seung-Won, Bae) Comment. N ��/��/�� ���� ���� ���� �����Ѵ�. (UT��)
	if( nTimeInterval < 1) 
		nTimeInterval = 1;

	// 2011.01.18 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	//m_nTimeInterval = m_ttcTrPacket.GetFullTypeTime( nTimeInterval);
	if (m_ttcTrPacket.GetPacketType() == "YYYYMMDDHHMMSS")
		m_nTimeInterval = nTimeInterval * 100;
	else
		m_nTimeInterval = m_ttcTrPacket.GetFullTypeTime( nTimeInterval);
	// 2011.01.18 by SYS <<


	// (2009/10/21 - Seung-Won, Bae) Comment. ����� �ð��� HHMMSS�� �����Ѵ�. (�ǽð� ���۽ð� Ȯ�ο�)
	m_strMarketStartTime = strMarketStartTime;
	m_nMarketStartTime = atoi( m_strMarketStartTime);

	// (2009/10/27 - Seung-Won, Bae) the Flag for real after special code like market end.
	m_bMarketOn = TRUE;
}

CTimeRealDataModule::~CTimeRealDataModule()
{
}


// (2009/10/21 - Seung-Won, Bae) Comment. ���� �ǽð��� �����ϴ� ���� ���� ����/�� �ð����� �����Ѵ�. (Packet Type���̴�.)
__int64 CTimeRealDataModule::GetTimeRangeStartWithPacketType( void) const
{
	return m_ttcTrPacket.GetPacketTypeTime( m_nTimeRangeStart);
}


// (2009/11/19 - Seung-Won, Bae) ��ȸ�� ������ Data�� �ǽð� ���� ����� �������� ���Ѵ�.
//	�� �������� �������� �ǽð� ���Ž� ������ Data�� Update�� ������ Append�� ������ �����ȴ�.
//		Ư�� Append�ÿ��� ���� ������ ������Ű�� Update���� ó���ȴ�.
//		���� ������ ���� Update�ϱ� ���� �ǽð� �������� �ð������� ���ϴ� ���̴�.
//	�� ������ ������ Data������ ���� ���۱��ؽ÷� �⺻ ������ �����ϰ�, ������ Data�� �ǽð� Data�� ��ó�� ���ԵǴ� �������� �˻��Ѵ�.
//	������ Data ��ü�� ���� ���, ������ �������� �ʰ�, ù �ǽð��� ���� ��, ���� ���� ���� ������� ���Ѵ�.
//		���� �̷��� ������ �������� ���� ��Ȳ������ ����� ������ �� ������,
//		ù �Ǻ� ���Žô� ������ Append�� ó���� ������ �����Ѵ�.
BOOL CTimeRealDataModule::SetTimeRange_TRPart( const __int64 &p_nServerTime, __int64 nTailTime)
{
	if( nTailTime < 0) return FALSE;

	// (2009/11/13 - Seung-Won, Bae) �߰��� ����� RollBack ��Ű��.
	//	�߰��� ����� ���� ��ŭ Ȯ���ϸ鼭, ������ ��� ������ ������Ű�鼭 �����ؾ��� Data ������ ���Ѵ�.
	//  �� �������(m_bCloseEmptyCandleWhenEnd:TRUE)�� ù���� �Ǻ��� ������Ű�Ƿ�,
	//		��� Data ������ -1���� ���۵ǵ��� �Ѵ�.
	m_nClockedDataCount = ( m_bCloseEmptyCandleWhenEnd ? -1 : 0);

	// 1. Make nTailTime to FullTime with Server Time.

	// 2011.01.11 by SYS >> ����Ʈ YYYY(��) �߰�
	// �к����� ������ ������ ó���� �Ǿ� ����
	//
// 	__int64 nFullTailTime = nTailTime;
// 	if( !m_ttcTrPacket.IsYearInPacketType())
// 	{
// 		// 1.1.1 Make User Locale Time of Server Time.
// 		__int64 nUserTimeOfServer = p_nServerTime;
// 		if( 0 != m_nRealTimeDiff) nUserTimeOfServer = CDataConversion::CalcTimeSum( nUserTimeOfServer, m_nRealTimeDiff);
// 		// 1.1.2 Make nTailTime to FullTime. (FullTime = FullTyped_DataTime + ServerTime - PacketTyped_ServerTime)
// 		__int64 nRotationTime = m_ttcTrPacket.GetRotationFullTime();
// 		nFullTailTime = m_ttcTrPacket.GetFullTypeTime( nFullTailTime);
// 		nFullTailTime += p_nServerTime - p_nServerTime % nRotationTime;
// 		// 1.1.3 Make Standard Time.
// 		if( 0 != m_nRealTimeDiff) nFullTailTime = CDataConversion::CalcTimeSum( nFullTailTime, -m_nRealTimeDiff);
// 		// 1.3 Tail Time can not be future time. (but can be future with time interval)
// 		__int64 nServerTime = CDataConversion::CalcTimeSum( p_nServerTime, m_nTimeInterval);
// 		while( nServerTime < nFullTailTime)
// 			nFullTailTime = CDataConversion::CalcTimeSum( nFullTailTime, -nRotationTime);
// 	}

	__int64 nFullTailTime = m_ttcTrPacket.GetFullTypeTime( nTailTime);

//	{
//		CString szDebug;
//		szDebug.Format("A p_nServerTime(%I64d) nFullTailTime(%I64d) nTailTime(%I64d) m_nRealTimeDiff(%I64d) m_nNewRangeStartTime(%I64d)", 
//				p_nServerTime, nFullTailTime, nTailTime, m_nRealTimeDiff, m_nNewRangeStartTime);
//		//OutputDebugString(szDebug);
//		g_DRDebuger.DRLog_Message(szDebug);
//	}
	if( !m_ttcTrPacket.IsYearInPacketType())
	{
//		g_DRDebuger.DRLog_Message("B IsYearInPacketType is FALSE.");
		// 1.1.1 Make User Locale Time of Server Time.
		__int64 nUserTimeOfServer = p_nServerTime;
		if( 0 != m_nRealTimeDiff) nUserTimeOfServer = CDataConversion::CalcTimeSum( nUserTimeOfServer, m_nRealTimeDiff);
		// 1.1.2 Make nTailTime to FullTime. (FullTime = FullTyped_DataTime + ServerTime - PacketTyped_ServerTime)
		__int64 nRotationTime = m_ttcTrPacket.GetRotationFullTime();
		nFullTailTime += p_nServerTime - p_nServerTime % nRotationTime;
		// 1.1.3 Make Standard Time.
		if( 0 != m_nRealTimeDiff) nFullTailTime = CDataConversion::CalcTimeSum( nFullTailTime, -m_nRealTimeDiff);
		// 1.3 Tail Time can not be future time. (but can be future with time interval)
		__int64 nServerTime = CDataConversion::CalcTimeSum( p_nServerTime, m_nTimeInterval);
		while( nServerTime < nFullTailTime)
			nFullTailTime = CDataConversion::CalcTimeSum( nFullTailTime, -nRotationTime);
	}
	// 2011.01.11 by SYS <<


	// 2. Get the first Range with Range Start Time.
	// 2.1 Get FullTime of Range Start Time.
	if(p_nServerTime == 0)
		return FALSE;

	__int64 nRangeStartTime = p_nServerTime - ( p_nServerTime % 1000000) + m_nNewRangeStartTime;
	// 2.2 Range Start Time must be after tail time.
	while( nFullTailTime <= nRangeStartTime)
		nRangeStartTime = CDataConversion::CalcTimeSum( nRangeStartTime, -1000000);

	// 3. Get the Range for Tail Time.
	m_nTimeRangeStart = nRangeStartTime;
	m_nTimeRangeEnd = CDataConversion::CalcTimeSum( m_nTimeRangeStart, m_nTimeInterval);
//	{
//		CString szDebug;
//		szDebug.Format("C before: m_nTimeRangeEnd(%I64d) m_nTimeRangeStart(%I64d) m_nTimeInterval(%I64d) nFullTailTime(%I64d)\n", 
//					m_nTimeRangeEnd, m_nTimeRangeStart, m_nTimeInterval, nFullTailTime);
//		//OutputDebugString(szDebug);
//		g_DRDebuger.DRLog_Message(szDebug);
//	}

	while( IsTimeAdd( nFullTailTime, m_nTimeRangeEnd))
		if( !GetNextIntervalWithoutOffTime( m_nTimeRangeStart, m_nTimeRangeEnd, FALSE))
			break;
//	{
//		CString szDebug;
//		szDebug.Format("D after : m_nTimeRangeEnd(%I64d) m_nTimeRangeStart(%I64d) m_nTimeInterval(%I64d)\n", m_nTimeRangeEnd, m_nTimeRangeStart, m_nTimeInterval);
//		//OutputDebugString(szDebug);
//		g_DRDebuger.DRLog_Message(szDebug);
//	}

	// 4. for CEIL2 and FLOOR2 (Bound data is not included in range)
	if( GetRealType() == CPacketType::CEILING2 && m_nTimeRangeStart == nFullTailTime)
		GetPrevIntervalWithoutOffTime( m_nTimeRangeStart, m_nTimeRangeEnd, TRUE);

//	{
//		CString szDebug;
//		szDebug.Format("E m_nTimeRangeStart(%I64d) m_nTimeRangeEnd(%I64d)\n", m_nTimeRangeStart, m_nTimeRangeEnd);
//		g_DRDebuger.DRLog_Message(szDebug);
//	}

	return TRUE;
}

// (2009/10/22 - Seung-Won, Bae) Comment. ���ŵ� �ǽð��� �ø�/���� ���� ���Ͽ� �� Logic Class�� ���� Packet�� �ݿ� ó���Ѵ�.
// (2009/11/20 - Seung-Won, Bae) ����! ��ȸ�����Ͱ� ���� ��� m_nTimeRangeStart, m_nTimeRangeEnd ���� -1�̴�.
//		����� Ȯ���� �ʿ��ϴ�.
int CTimeRealDataModule::SetRealData( CPacket* pPacket, const __int64 &nRealTime)
{
	if( m_ttcTrPacket.GetPacketType().IsEmpty()) return -1;

	if( !m_bMarketOn) return 0;
	// (2009/10/27 - Seung-Won, Bae) the Flag for real after special code like market end.
	int nRealHMS = int( nRealTime % 1000000);
	if( g_iMetaTable.IsSpecialTime( nRealHMS))
	{
		if( 0 < m_nTimeRangeEnd)
		{
			// (2009/11/3 - Seung-Won, Bae) User Time.
			__int64 nUserTime = GetCloseTime( m_nTimeRangeStart, m_nTimeRangeEnd);
			if( 0 != m_nRealTimeDiff) nUserTime = CDataConversion::CalcTimeSum( nUserTime, m_nRealTimeDiff);

			m_bMarketOn = FALSE;
			pPacket->UpdateData( ( double)m_ttcTrPacket.GetPacketTypeTime( nUserTime));
		}
		return 0;
	}



	// 1. ���ŵ� �ǽð����� �д��� �ø�/�������� ���Ѵ�. (�ô���-����Ʈ�� �������� �ʴ´�.)

	// 2011.01.18 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	//__int64 nMod = nRealTime % m_ttcTrPacket.GetMinimumUnitTime();

	__int64	nMinimumUnitTime = m_ttcTrPacket.GetMinimumUnitTime();
	if (m_ttcTrPacket.GetPacketType() == _T("YYYYMMDDHHMMSS"))
		nMinimumUnitTime = 100;	
	__int64 nMod = nRealTime % nMinimumUnitTime;
	// 2011.01.18 by SYS <<
	
	__int64 nFloorTime = nRealTime - nMod;
	__int64 nCeilTime = nRealTime;
	// 01�� �̻��� ��� ������ �ø�.
	// 09:00:00 �ΰ�� �������� �����1�ʸ� �������� �༭ 09:00:00�� ��츸 1�� �ø�(�Ｚ����)
	if( nRealHMS == m_nMarketStartTime
		|| ( 0 == nRealHMS % 100 && GetRealType() == CPacketType::CEILING2)
		|| 0 != nMod)
	{
		// 2011.01.18 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
		//nCeilTime = CDataConversion::CalcTimeSum( nCeilTime, m_ttcTrPacket.GetMinimumUnitTime());
		nCeilTime = CDataConversion::CalcTimeSum( nCeilTime, nMinimumUnitTime);
		// 2011.01.18 by SYS <<
	}
	nCeilTime = nCeilTime - nMod;

	// 2. ���ŵ� �ǽð��� �ø�/���� ���� ������ �� Logic Class�� �°� Packet�� �ݿ� ó���Ѵ�.
	return SetRealData_AppendOrUpdate( pPacket, nRealTime, nFloorTime, nCeilTime);
}
// (2009/10/22 - Seung-Won, Bae) nCheckTime�� �������� Update/Append ���θ� �����ϰ�,
//		�� Data�δ� nUpdateTime�� �ݿ��Ѵ�. �������ÿ��� nCloseTime���� �����Ѵ�.
int CTimeRealDataModule::SetRealData_AppendOrUpdate2( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nCheckTime, const __int64 &nUpdateTime)
{
	// (2009/11/13 - Seung-Won, Bae) �߰��� ����� RollBack ��Ű��.
	//	�߰��� ����� ���� ��ŭ Ȯ���ϸ鼭, ������ ��� ������ ������Ű�鼭 �����ؾ��� Data ������ ���Ѵ�.
	//  �� �������(m_bCloseEmptyCandleWhenEnd:TRUE)�� ù���� �Ǻ��� ������Ű�Ƿ�,
	//		��� Data ������ -1���� ���۵ǵ��� �Ѵ�.
	int nDeleteCount = 0;
	if( 0 < m_nTimeRangeEnd) while( 0 < m_nClockedDataCount - nDeleteCount && !IsTimeAdd( nCheckTime, m_nTimeRangeEnd))
	{
		GetPrevIntervalWithoutOffTime( m_nTimeRangeStart, m_nTimeRangeEnd);
		nDeleteCount++;
	}
	if( 0 < nDeleteCount) pPacket->RemoveBaseData( nDeleteCount);
	m_nClockedDataCount = ( m_bCloseEmptyCandleWhenEnd ? -1 : 0);

	CBaseData::RESULT eResult = CBaseData::Add;
	if( 0 < m_nTimeRangeEnd)
		if( !IsTimeAdd( nCheckTime, m_nTimeRangeEnd)) eResult = CBaseData::Update;

//	{
//		CString szDebug;
//		szDebug.Format("[SMalzio] realchk : IsTimeAdd (%d) nCheckTime(%I64d) m_nTimeRangeEnd(%I64d)\n", (nCheckTime > m_nTimeRangeEnd), nCheckTime, m_nTimeRangeEnd);
//		OutputDebugString(szDebug);
//	}

	if(eResult == CBaseData::Update)
	{
		// (2009/11/3 - Seung-Won, Bae) User Time.
		__int64 nUserTime = nUpdateTime;
		if( 0 != m_nRealTimeDiff) nUserTime = CDataConversion::CalcTimeSum( nUserTime, m_nRealTimeDiff);

		pPacket->UpdateData( ( double)m_ttcTrPacket.GetPacketTypeTime( nUserTime));
		return nDeleteCount;
	}

	if( 0 < m_nTimeRangeEnd && eResult == CBaseData::Add)
	{
		// (2009/11/3 - Seung-Won, Bae) User Time.
		__int64 nUserTime = GetCloseTime( m_nTimeRangeStart, m_nTimeRangeEnd);
		if( 0 != m_nRealTimeDiff) nUserTime = CDataConversion::CalcTimeSum( nUserTime, m_nRealTimeDiff);

		pPacket->UpdateData( ( double)m_ttcTrPacket.GetPacketTypeTime( nUserTime));
	}

	// (2009/11/3 - Seung-Won, Bae) User Time.
	__int64 nUserTime = nUpdateTime;
	if( 0 != m_nRealTimeDiff) nUserTime = CDataConversion::CalcTimeSum( nUserTime, m_nRealTimeDiff);

	// with CBaseData::TodayFirstReal
	__int64 nAppendTime = m_ttcTrPacket.GetPacketTypeTime( nUserTime);
	pPacket->AppendData( ( double)nAppendTime);

	// (2009/11/19 - Seung-Won, Bae) ��ȸ�� ������ Data�� �ǽð� ���� ����� �������� ���Ѵ�.
	//	�� �������� �������� �ǽð� ���Ž� ������ Data�� Update�� ������ Append�� ������ �����ȴ�.
	//		Ư�� Append�ÿ��� ���� ������ ������Ű�� Update���� ó���ȴ�.
	//		���� ������ ���� Update�ϱ� ���� �ǽð� �������� �ð������� ���ϴ� ���̴�.
	//	�� ������ ������ Data������ ���� ���۱��ؽ÷� �⺻ ������ �����ϰ�, ������ Data�� �ǽð� Data�� ��ó�� ���ԵǴ� �������� �˻��Ѵ�.
	//	������ Data ��ü�� ���� ���, ������ �������� �ʰ�, ù �ǽð��� ���� ��, ���� ���� ���� ������� ���Ѵ�.
	//		���� �̷��� ������ �������� ���� ��Ȳ������ ����� ������ �� ������,
	//		ù �Ǻ� ���Žô� ������ Append�� ó���� ������ �����Ѵ�.
	if( m_nTimeRangeEnd <= 0)
		SetTimeRange_TRPart( nRealTime, nAppendTime);

//	CString strTemp;
//	strTemp.Format("\n DRFN => nCheckTime = %I64d, m_nTimeRangeStart = %I64d, m_nTimeRangeEnd = %I64d", nCheckTime, m_nTimeRangeStart, m_nTimeRangeEnd);
//	OutputDebugString(strTemp);

	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	if(m_ttcTrPacket.GetPacketType() == "YYYYMMDDHHMMSS"  && m_nTimeRangeEnd<20000000000000)
	{
//		{
//			CString szDebug;
//			szDebug.Format("[SMalzio]m_nTimeRangeEnd(%I64d) nCheckTime(%I64d)\n", m_nTimeRangeEnd, nCheckTime);
//			OutputDebugString(szDebug);
//		}

		m_nTimeRangeEnd = nCheckTime;
	}
	// 2011.01.06 by SYS <<

	while( IsTimeAdd( nCheckTime, m_nTimeRangeEnd))
	{
		GetNextIntervalWithoutOffTime( m_nTimeRangeStart, m_nTimeRangeEnd, FALSE);
	}

	return nDeleteCount;
}


// ����� ���� �ð��帧�� �����Ѵ�. (����ð��� ���� ���� �߰��Ǵ��� ���θ� Return�Ѵ�.)
bool CTimeRealDataModule::GetRealData_CloseTime( CPacket* pPacket, __int64 &p_nRealDataTime)
{
	if( m_ttcTrPacket.GetPacketType().IsEmpty() || m_nTimeRangeEnd <= 0 || !m_bMarketOn) 
		return false;

	// 1. ���ŵ� �ǽð����� �д��� �ø�/�������� ���Ѵ�. (�ô���-����Ʈ�� �������� �ʴ´�.)


	// 2011.01.18 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	//__int64 nMod = p_nRealDataTime % m_ttcTrPacket.GetMinimumUnitTime();
	__int64	nMinimumUnitTime = m_ttcTrPacket.GetMinimumUnitTime();
	if (m_ttcTrPacket.GetPacketType() == _T("YYYYMMDDHHMMSS"))
		nMinimumUnitTime = 100;	
	__int64 nMod = p_nRealDataTime % nMinimumUnitTime;
	// 2011.01.18 by SYS <<

	__int64 nFloorTime = p_nRealDataTime - nMod;
	__int64 nCeilTime = p_nRealDataTime;
	// 01�� �̻��� ��� ������ �ø�.
	// 09:00:00 �ΰ�� �������� �����1�ʸ� �������� �༭ 09:00:00�� ��츸 1�� �ø�(�Ｚ����)
	if( p_nRealDataTime % 1000000 == m_nMarketStartTime
		|| ( 0 == p_nRealDataTime % 100 && GetRealType() == CPacketType::CEILING2)
		|| 0 != nMod)
	{
		// 2011.01.18 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
		//nCeilTime = CDataConversion::CalcTimeSum( nCeilTime, m_ttcTrPacket.GetMinimumUnitTime());
		nCeilTime = CDataConversion::CalcTimeSum( nCeilTime, nMinimumUnitTime);
		// 2011.01.18 by SYS <<
	}
	nCeilTime = nCeilTime - nMod;

	// 2. ���ŵ� �ǽð��� �ø�/���� ���� ������ �� Logic Class�� �°� Packet�� �ݿ� ó���Ѵ�.
	if( !GetRealData_CloseTime_AppendOrUpdate( pPacket, nFloorTime, nCeilTime, p_nRealDataTime))
		return false;

	// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
	if( CChartInfo::WORLD_ON == m_nChartModeSetting)
		GetMarketDate_DateTime( p_nRealDataTime);

	m_nClockedDataCount++;
	return true;
}
bool CTimeRealDataModule::GetRealData_CloseTime_AppendOrUpdate2( CPacket* pPacket, const __int64 &nCheckTime, const __int64 &nUpdateTime, __int64 &p_nRealDataTime)
{
	if( nCheckTime <= 0 || nUpdateTime <= 0) return  false;

	// (2009/11/13 - Seung-Won, Bae) ��� ó����, CLOCK�� �� ����� �ð��� �Ǿ�����,
	//		������ ���� �������� ���̴� ���̴�.
	//	�� �������(m_bCloseEmptyCandleWhenEnd:FALSE)�� ��� ���� ������� ������ ������� ���ϰ�,
	//														(��Ȯ���� ���� ��� ������ �����ߴ���)
	//		��� ���, ���� ������ �������� ���� �ð� Data�� ���Ͽ� �����ϸ�,
	//		�������� �Ǻ� ó�� Logic�� ���� ���� ������ �ڵ����� �����ǰ�,
	//		�ű� ������ ���� �����Ǹ鼭 ����ǥ�õ� �̷�����.
	//  �� �������(m_bCloseEmptyCandleWhenEnd:TRUE)�� ��� ���� ������� ������ ���� ������ ������� ���ϰ�,
	//			(ù ������ �Ǻ��� �����̴�. �׷��� ��� ������ -1�� ��쿡�� ���� ������ �ƴ� �� ������ ���Ѵ�.)
	//		��� ���, �� ���� ������� ������ ���� ������ �������� ���� �ð� Data�� ���Ͽ� �����ϸ�,
	//		�������� �Ǻ� ó�� Logic�� ���� ���� ������ �ڵ����� �����ǰ�,
	//		�ű� ������ ���� �����Ǹ鼭 ����ǥ�õ� �̷�����.
	// (2009/11/18 - Seung-Won, Bae) �񿵾��ð��� ����
	//		1. �帶���ð�
	//		2. �񿵾���
	//	�Ѵ� GetNextIntervalWithoutOffTime()�� GetPrevIntervalWithoutOffTime()���� �ڵ����� �ǳ� �پ�� �Ѵ�.
	//	�׷��� ����� RollBack ó���� ��Ȯ�ϰ� ó���ȴ�.
	//	����! �� ��������� ��� �񿵾��ð��� ���(���� ������ ������ �ð��� ���� ���� ���) ������ó������ �ʵ��� �Ѵ�.
	__int64 nNewTimeRangeStart = m_nTimeRangeStart;
	__int64 nNewTimeRangeEnd = m_nTimeRangeEnd;

	// ��/�� ���� ��Ŀ� ���� ������ų ������ ���ϰ�, ������ų �������� Ȯ���Ѵ�.
	if( m_bCloseEmptyCandleWhenEnd)
	{
		// �ĸ��� ����� CLOCK �ð��� ���� ������ ������ �Ѿ����� ���Ѵ�. �� ù ����� �� �������� �������Ѿ� �Ѵ�.
		if( 0 <= m_nClockedDataCount) GetNextIntervalWithoutOffTime( nNewTimeRangeStart, nNewTimeRangeEnd);
		// ��� ������ ������ �Ѿ���� Ȯ���Ѵ�. �Ѿ�߸� ������ �ϴ��� �Ѵ�.
		if( !IsTimeAdd( nCheckTime, nNewTimeRangeEnd))	return false;
	}
	else
	{
		// �� ��������� ���� ������ ������ �����Ͽ������� ���Ͽ� ��������Ų��.
		GetNextIntervalWithoutOffTime( nNewTimeRangeStart, nNewTimeRangeEnd);
		if( !IsTimeAdd( nCheckTime, nNewTimeRangeStart)) return false;
	}

	// ������Ű�� ���� �ð����� ���Ѵ�.
	__int64 nUpdateSec = CDataConversion::GetSecond( nUpdateTime, TRUE);
	__int64 nCloseSec = CDataConversion::GetSecond( GetCloseTime( nNewTimeRangeStart, nNewTimeRangeEnd), TRUE);
	if( nUpdateSec < nCloseSec && !m_ttcTrPacket.IsYearInPacketType())
		nUpdateSec += CDataConversion::GetSecond( m_ttcTrPacket.GetRotationFullTime());
	__int64 nDiff = nCloseSec - nUpdateSec;

	if( nDiff < -60 * 60 * 24) return false;

	p_nRealDataTime = CDataConversion::CalcTimeSum( p_nRealDataTime, nDiff, TRUE);
	return true;
}

BOOL CTimeRealDataModule::GetNextIntervalWithoutOffTime( __int64 &p_nTimeRangeStart, __int64 &p_nTimeRangeEnd, BOOL p_bWithoutOffTime)
{
	// X ScaleManager�� �ٸ� �ð����� Scale ó���� ���Ͽ� �����/�����ð��� ��ȸ�Ѵ�.
	BOOL bReturn = TRUE;
	int nMarketStartTime = -1, nMarketEndTime = -1;
	if( !m_marketTimeData.GetMarketTime( nMarketStartTime, nMarketEndTime))
		bReturn = FALSE;

	int nStartTime = -1, nEndTime = -1;
	__int64 nIntervalSec = GetTimeIntervalWithSecond();
	__int64 nEnd = ( 24 * 60 * 60 * 7) / nIntervalSec;
	for( __int64 i = 0; i <= nEnd; i++)
	{
		p_nTimeRangeStart = p_nTimeRangeEnd;
		p_nTimeRangeEnd = CDataConversion::CalcTimeSum( p_nTimeRangeEnd, m_nTimeInterval);

		// (2009/11/16 - Seung-Won, Bae) �ش� �ǽð� ������ m_nNewRangeStartTime�� �������� �����Ѵ�.
		nStartTime = int( p_nTimeRangeStart % 1000000);
		nEndTime = int( p_nTimeRangeEnd % 1000000);
		if( nEndTime < nStartTime)
		{
			if( m_nNewRangeStartTime < nEndTime || nStartTime < m_nNewRangeStartTime)
			{
				p_nTimeRangeEnd -= nEndTime;
				nEndTime = m_nNewRangeStartTime;
				p_nTimeRangeEnd += nEndTime;
			}
		}
		else
		{
			if( nStartTime < m_nNewRangeStartTime && m_nNewRangeStartTime < nEndTime)
			{
				p_nTimeRangeEnd -= nEndTime;
				nEndTime = m_nNewRangeStartTime;
				p_nTimeRangeEnd += nEndTime;
			}
		}

		if( !p_bWithoutOffTime) return TRUE;

		if( IsOnMarketTimeRange( p_nTimeRangeStart, p_nTimeRangeEnd, nStartTime, nEndTime, nMarketStartTime, nMarketEndTime)) return TRUE;
	}
	return bReturn; 
}

void CTimeRealDataModule::GetPrevIntervalWithoutOffTime( __int64 &p_nTimeRangeStart, __int64 &p_nTimeRangeEnd, BOOL p_bWithoutOffTime)
{
	// X ScaleManager�� �ٸ� �ð����� Scale ó���� ���Ͽ� �����/�����ð��� ��ȸ�Ѵ�.
	int nMarketStartTime = -1, nMarketEndTime = -1;
	if( !m_marketTimeData.GetMarketTime( nMarketStartTime, nMarketEndTime)) return;

	int nStartTime = -1, nEndTime = -1;
	__int64 nIntervalSec = GetTimeIntervalWithSecond();
	for( int i = 0; i <= ( 24 * 60 * 60 * 7) / nIntervalSec; i++)
	{
		p_nTimeRangeEnd = p_nTimeRangeStart;
		p_nTimeRangeStart = CDataConversion::CalcTimeSum( p_nTimeRangeStart, -m_nTimeInterval);

		// (2009/11/16 - Seung-Won, Bae) �ش� �ǽð� ������ m_nNewRangeStartTime�� �������� �����Ѵ�.
		nStartTime = int( p_nTimeRangeStart % 1000000);
		nEndTime = int( p_nTimeRangeEnd % 1000000);
		if( nEndTime == m_nNewRangeStartTime)
		{
			__int64 nMod = ( 24 * 60 * 60) % nIntervalSec;
			if( 0 != nMod)
			{
				p_nTimeRangeStart = CDataConversion::CalcTimeSum( p_nTimeRangeStart, nIntervalSec - nMod, TRUE);
				nStartTime = int( p_nTimeRangeStart % 1000000);
			}
		}

		if( p_bWithoutOffTime) return;

		if( IsOnMarketTimeRange( p_nTimeRangeStart, p_nTimeRangeEnd, nStartTime, nEndTime, nMarketStartTime, nMarketEndTime)) return;
	}
}


// (2009/11/21 - Seung-Won, Bae) ������ ������ �������� ���θ� Ȯ���Ѵ�.
//	1. ���� ���δ� �⺻������ ����� �ð��� �帶�� �ð��� ���������Ѵ�.
//		�ǽð� �ð� ������ �Ϻΰ� ����� �ð��� �帶�� �ð��� ���ʿ� ������ '����'���� �����Ѵ�.
//	2. ���� �ð���� ������ ���ο� ���� ���� ���ΰ� �����ȴ�.
//	3. ������ �ƴ� �ð���� �⺻������ '����'�� �ƴϳ�, FX�� ��� ���ϰ� ������ �������̸� ��� '����'���� �����Ѵ�.
BOOL CTimeRealDataModule::IsOnMarketTimeRange( const __int64 &p_nTimeRangeStart, const __int64 &p_nTimeRangeEnd, const int &nStartTime, const int &nEndTime, const int &nMarketStartTime, const int &nMarketEndTime)
{
	//	1. ���� ���δ� �⺻������ ����� �ð��� �帶�� �ð��� ���������Ѵ�.
	//		�ǽð� �ð� ������ �Ϻΰ� ����� �ð��� �帶�� �ð��� ���ʿ� ������ '����'���� �����Ѵ�.
	//	2. ���� �ð���� ������ ���ο� ���� ���� ���ΰ� �����ȴ�.
	__int64 nDateCheckTime = -1;
	if( m_marketTimeData.IsReverseTime())
	{
		if( nStartTime < nMarketEndTime || nMarketStartTime <= nStartTime)
		{
			if( IsOnMarketDay( p_nTimeRangeStart)) return TRUE;
		}
		if( nEndTime <= nMarketEndTime || nMarketStartTime < nEndTime) 
		{
			if( IsOnMarketDay( p_nTimeRangeEnd)) return TRUE;
		}
		return FALSE;
	}
	else
	{
		if( nMarketStartTime <= nStartTime && nStartTime < nMarketEndTime)
		{
			if( IsOnMarketDay( p_nTimeRangeStart)) return TRUE;
		}
		if( nMarketStartTime < nEndTime && nEndTime <= nMarketEndTime)
		{
			if( IsOnMarketDay( p_nTimeRangeEnd)) return TRUE;
		}
		return FALSE;
	}

	//	3. �׸��� ������ �ƴ� ���� �Ͽ���, FX�� ��� ���ϰ� ������ �������̸� ���������� ����ȴ�.
	if( CChartInfo::FX == m_nChartModeSetting)
	{
		__int64 nMarketDate = p_nTimeRangeStart;
		GetMarketDate_DateTime( nMarketDate);
		if( !IsMarketDay( int( nMarketDate / 1000000))) return FALSE;
		if( !IsMarketDay( int( CDataConversion::CalcTimeSum( nMarketDate, 1000000) / 1000000))) return FALSE;
		return TRUE;
	}

	return FALSE;
}
BOOL CTimeRealDataModule::IsOnMarketDay( __int64 p_nDateCheckTime)
{
	GetMarketDate_DateTime( p_nDateCheckTime);
	return IsMarketDay( int( p_nDateCheckTime/ 1000000));
}


// (2009/11/12 - Seung-Won, Bae) for NMTR Step Width
__int64	CTimeRealDataModule::GetTimeIntervalWithSecond( void)
{
	return CDataConversion::GetSecond( m_nTimeInterval);
}


///////////////////////////////////////////////////////////////////////////////
// class CFloorTimeRealDataModule : 09:01:00 ~ 09:01:59 -> 09:01

// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
CFloorTimeRealDataModule::CFloorTimeRealDataModule( const CDateData& dateData, CMarketTimeData &p_marketTimeData, const CString& strPacketType, const int nTimeInterval, const CString& strMarketStartTime, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType):
	CTimeRealDataModule( dateData, p_marketTimeData, strPacketType, nTimeInterval, strMarketStartTime, eDateUnitType)
{
}

// (2009/10/22 - Seung-Won, Bae) �ǽð��� ���� �����ϴ� �ð����� ������ nEndTime�̰�, �ǽð� ���� nCurTime�� ���, Append ��Ȳ���� Ȯ���ϴ� Logic.
//		�� ó�� Logic Class ���� �ٸ��� ó���ȴ�.
// ������ time�� update ������ ����
bool CFloorTimeRealDataModule::IsTimeAdd( const __int64 &nCurTime, const __int64 &nEndTime) const
{
	if(nEndTime<0 || nCurTime<0)
	{
		return false;
	}

	return (nCurTime >= nEndTime);
}


// (2009/10/22 - Seung-Won, Bae) �ǽð� �ð��� �ø�/���������� �� Packet���� �����ϰ� �űԺ� ������ �������� �ٽ� �����Ѵ�.
//		Logic Class�� ���� Update/Append Ȯ�ο� �ð�, Update�� �ð�, �������� �ð��� �޸��Ͽ�,
//		SetRealData_AppendOrUpdate()�� ���� ó���ȴ�.
int CFloorTimeRealDataModule::SetRealData_AppendOrUpdate( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nFloorTime, const __int64 &nCeilTime)
{
	return CTimeRealDataModule::SetRealData_AppendOrUpdate2( pPacket, nRealTime, nFloorTime, nFloorTime);
}


// ����� ���� �ð��帧�� �����Ѵ�. (����ð��� ���� ���� �߰��Ǵ��� ���θ� Return�Ѵ�.)
bool CFloorTimeRealDataModule::GetRealData_CloseTime_AppendOrUpdate( CPacket* pPacket, const __int64 &nFloorTime, const __int64 &nCeilTime, __int64 &p_nRealDataTime)
{
	return GetRealData_CloseTime_AppendOrUpdate2( pPacket, nFloorTime, nFloorTime, p_nRealDataTime);
}


///////////////////////////////////////////////////////////////////////////////
// class CCeilingTimeRealDataModule : 09:00:01 ~ 09:01:00 -> 09:01 (�޸���)

// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
CCeilingTimeRealDataModule::CCeilingTimeRealDataModule( const CDateData& dateData, CMarketTimeData &p_marketTimeData, const CString& strPacketType, const int nTimeInterval, const CString& strMarketStartTime, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType):
	CTimeRealDataModule( dateData, p_marketTimeData, strPacketType, nTimeInterval, strMarketStartTime, eDateUnitType)
{
}


// (2009/10/22 - Seung-Won, Bae) �ǽð��� ���� �����ϴ� �ð����� ������ nEndTime�̰�, �ǽð� ���� nCurTime�� ���, Append ��Ȳ���� Ȯ���ϴ� Logic.
//		�� ó�� Logic Class ���� �ٸ��� ó���ȴ�.
// ������ time�� update ������ ����
bool CCeilingTimeRealDataModule::IsTimeAdd( const __int64 &nCurTime, const __int64 &nEndTime) const
{
	//100618-alzioyes : ���ѷ��� ������ �Ϳ����� ����ó����ƾ �߰�.
	if(nEndTime<0 || nCurTime<0)
	{
		return false;
	}

	return nCurTime > nEndTime;
}


// (2009/10/22 - Seung-Won, Bae) �ǽð� �ð��� �ø�/���������� �� Packet���� �����ϰ� �űԺ� ������ �������� �ٽ� �����Ѵ�.
//		Logic Class�� ���� Update/Append Ȯ�ο� �ð�, Update�� �ð�, �������� �ð��� �޸��Ͽ�,
//		SetRealData_AppendOrUpdate()�� ���� ó���ȴ�.
int CCeilingTimeRealDataModule::SetRealData_AppendOrUpdate( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nFloorTime, const __int64 &nCeilTime)
{
	return CTimeRealDataModule::SetRealData_AppendOrUpdate2( pPacket, nRealTime, nCeilTime, nCeilTime);
}


// ����� ���� �ð��帧�� �����Ѵ�. (����ð��� ���� ���� �߰��Ǵ��� ���θ� Return�Ѵ�.)
bool CCeilingTimeRealDataModule::GetRealData_CloseTime_AppendOrUpdate( CPacket* pPacket, const __int64 &nFloorTime, const __int64 &nCeilTime, __int64 &p_nRealDataTime)
{
	return GetRealData_CloseTime_AppendOrUpdate2( pPacket, nCeilTime, nCeilTime, p_nRealDataTime);
}


// (2009/6/15 - Seung-Won, Bae) Koscom is new ceiling type.
///////////////////////////////////////////////////////////////////////////////
// class CCeiling2TimeRealDataModule : 09:00:00 ~ 09:00:59 -> 09:01 : �ø� (KOSCOM)

// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
CCeiling2TimeRealDataModule::CCeiling2TimeRealDataModule( const CDateData& dateData, CMarketTimeData &p_marketTimeData, const CString& strPacketType, const int nTimeInterval, const CString& strMarketStartTime, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType):
	CCeilingTimeRealDataModule( dateData, p_marketTimeData, strPacketType, nTimeInterval, strMarketStartTime, eDateUnitType)
{
}

// (2009/10/22 - Seung-Won, Bae) �ǽð��� ���� �����ϴ� �ð����� ������ nEndTime�̰�, �ǽð� ���� nCurTime�� ���, Append ��Ȳ���� Ȯ���ϴ� Logic.
//		�� ó�� Logic Class ���� �ٸ��� ó���ȴ�.
// ������ time�� update ������ ����
bool CCeiling2TimeRealDataModule::IsTimeAdd( const __int64 &nCurTime, const __int64 &nEndTime) const
{
	//100618-alzioyes : ���ѷ��� ������ �Ϳ����� ����ó����ƾ �߰�.
	if(nEndTime<0 || nCurTime<0)
	{
		return false;
	}

	return nCurTime >= nEndTime;
}


// (2009/10/22 - Seung-Won, Bae) �ǽð� �ð��� �ø�/���������� �� Packet���� �����ϰ� �űԺ� ������ �������� �ٽ� �����Ѵ�.
//		Logic Class�� ���� Update/Append Ȯ�ο� �ð�, Update�� �ð�, �������� �ð��� �޸��Ͽ�,
//		SetRealData_AppendOrUpdate()�� ���� ó���ȴ�.
int CCeiling2TimeRealDataModule::SetRealData_AppendOrUpdate( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nFloorTime, const __int64 &nCeilTime)
{
	return CTimeRealDataModule::SetRealData_AppendOrUpdate2( pPacket, nRealTime, nFloorTime, nCeilTime);
}


// ����� ���� �ð��帧�� �����Ѵ�. (����ð��� ���� ���� �߰��Ǵ��� ���θ� Return�Ѵ�.)
bool CCeiling2TimeRealDataModule::GetRealData_CloseTime_AppendOrUpdate( CPacket* pPacket, const __int64 &nFloorTime, const __int64 &nCeilTime, __int64 &p_nRealDataTime)
{
	return GetRealData_CloseTime_AppendOrUpdate2( pPacket, nFloorTime, nCeilTime, p_nRealDataTime);
}
