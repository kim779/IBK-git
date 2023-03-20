// RealModuleCenter.h: interface for the CRealModuleCenter class.
//
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (2009/10/19 - Seung-Won, Bae) �ǽð� �⺻ ��Ģ ������
//	1. �ؿܼ���(WorldOn) ����/�ð� Data�� ���� : FX�� HTS�� ��� ������ ���ڿ� �ð����� ���ϵ�.
//		��ȸ (��/��/��)							: ���� ������
//		��ȸ (��/ƽ)							: �ŷ��� ����/�ð�
//		�ǽð�									: ���� ������ / �ŷ��� �ð�
//		�����/���� �ð� (MARKETTIME)			: �ŷ��� �ð� (���ھ���)
//		����ð� (DATATIME)						: �ŷ��� ����/�ð�
//		�������� (MARKETDAY)					: ���� ������
//		24�ð� �ü��� N�к� �������� (BASETIME)	: ����¿����� ���ȵ� (����۽ð��� �⺻�� �� ��)
//		����� CLOCK �ð�						: ����(��ī��) ����/�ð�
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


















////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//	1. �ǽð� ó�� �⺻ ��Ģ
//		: �ǽð��� Update�� Add ���δ� ��ȸ��(�Ǵ� �ǽð� ���ŵ�) ������ �ð������� ������ Interval�� �������� ó���Ѵ�.
//		1) ��/��/������ ������ ���� ���Ͽ� ������ ������ ���� ����Ÿ�� ��� ������ ���� Update�Ѵ�
//		2) �к��� ��� ������ ���� ���Ͽ� ������ ������ ���� ����Ÿ�� ��� ������ ���� Update�Ѵ�.
//			���� ��ȸ����Ÿ�� YYYY �� ���� ���� ���� �Ǵ��Ͽ� 12��->1�� �� ���� �̷��� �����Ѵ�
//		
//		=> ���� �� ��Ģ�� ������ �ʰ� ���ڰ� �ٲ�� ����,�̷� ������� Append�ϰ� ���� ����
//			_REAL_UPDATE_LOGIC_1  �� Define �Ͽ� ����Ѵ�.
//		
//	2. ����� ó��
//		: ����� �ð谡 �߻���Ű�� �ð���, ��ȸ������ �����ð�(������ ���ŵ� �ڷ᳡�ð�)�� ��������, ������ �ð� Data�� �߻���Ų��.
//		1) ���� �ü��� �߻��ϴ� ���� ���� �ð� ����Ÿ�� ������ ���� �ü��� �ð� ����Ÿ�� Update�Ѵ�.
//			=> ���� �ü� �ð��� PC���� Tick���� Count �ϴ� �ð����� ������ ���� �� �����Ƿ� �ü� �ð����� �����.
//		2) ����� ����ϴ� �� Ÿ�� : CScaleBaseData::HORZ_TIME_ONE, CScaleBaseData::HORZ_TIME_TWO, CScaleBaseData::HORZ_TIME_SECOND 
//		3) �����,�帶�� �ð��� ����ð��� �������� �� ��� Tick �ð��� ������ �ƴϸ� ����� �߻���Ű�� �ʴ´�.
//		4) Tick �ð��� ���� �������� �ð����� 30�� ������ �������Ÿ�� �ǽð����� ������.
//		5) �������� ����ð� ���ְ� �ü��� ���� �� �� ���� ����� ó�� ���Ѵ�.
//			=> ������� ������ �ٰ� �ð��� �����Ƿ� �߻���ų ���� ����.
//			=> TR ����Ÿ�� ������ ����Ÿ�� �������� ������� �߻���Ű��, 
//				�������� �ƴ� �� ��ȸ�� �� ���� ���� ����Ÿ�� ����� �߻��� �� �����Ƿ� �ȵȴ�
//
//	3. ������ ó��
//		1) �����ð��� ������ �Ѱ� ��Ʋ�� �����ִ� ��� �������ڸ� �������ڷ� �� ������, �������ڸ� �������ڷ� �� ������ �Ǵ��Ͽ� ó��
//			=> �Ǵܹ�� : �������� �����ִ� �������ڿ� ����ð��� ���Ͽ� ����Ѵ�
//				- ���� �ð��� �帶���ð� ������ ��� ( ���� ������ )
//						������ == ������ : �������� ���(CPacketType::DAY_DOWN)
//						������ > ������ : �������� ���(CPacketType::DAY_UP)	=> CME
//				- ���� �ð��� �帶���ð� ������ ��� ( ���� ������ )
//						������ == ������ : �������� ���(CPacketType::DAY_UP)	=> CME
//						������ < ������ : �������� ���(CPacketType::DAY_DOWN)
//		2) ���� �ü��� ���� ���ڴ� �ŷ��ҽð����ڰ� �ƴ϶� �������ڰ� �´�
//				- ��/��/������ ��� �� ���ڸ� �״�� ����Ͽ� �� ����
//				- �к��� ��� �������̸鼭 �������� �������� ����� ��� �ü��� ���ڿ��� -1 �� �Ͽ� ó���ϰ�,
//							  �������̸鼭 �������� ���ϳ��� ����� ��� �ü��� ���ڿ��� +1 �� �Ͽ� ó���Ѵ�.
//		3) ����� �����ÿ��� �ŷ��� �ð��� �ƴ϶� �������ڷ� ����Ÿ�� �����ؾ� �Ѵ�
//				- �������̸鼭 �������� �������� ����� ��� Tick���� ���� ���糯�ڿ��� +1 �� �Ͽ� �ǽð����� ������,
//				  �������̸鼭 �������� ���ϳ��� ����� ��� Tick���� ���� ���糯�ڿ��� -1 �� �Ͽ� �ǽð����� ������.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REALMODULECENTER_H__36233F7E_0FC6_4D5A_9F9F_698C7F318819__INCLUDED_)
#define AFX_REALMODULECENTER_H__36233F7E_0FC6_4D5A_9F9F_698C7F318819__INCLUDED_

#include "PacketListModule.h"
#include "TimeRealDataModule.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IChartCtrl;
class CPacket;
class CRealModuleCenter
{
// 0. Constructor / Destructor
public:
	CRealModuleCenter( const CPacketType::REALTYPE eRealType, IChartCtrl *p_pIChartCtrl, CTypedPtrList< CObList, CPacket*> * pPacketList = NULL);
	virtual ~CRealModuleCenter();


// (2009/10/19 - Seung-Won, Bae) comment.
//	Base Packet�� ����ó�� - PacketList ���� ������ �Ǵ� AddPacket�� ���Ͽ� CPacketListImplementation::m_PacketList��
//		Packet�� �߰��Ǵ� ��� C0_DATE_TIME Packet (�ڷ�����)�� ���Ͽ� ����Ѵ�.
//	�׿ܿ��� ������ CPacketList::SetBasePacketName()�� ���� ���� �����ޱ⵵ �Ѵ�. (�Ⱦ��ϵ�.)
protected:
	CPacket *	m_pRealBasePacket; // real ���� packet
public:
	void		SetRealBasePacket( CPacket* pPacket);


// (2009/10/19 - Seung-Won, Bae) Comment. ������ �ǽð� ���ŵ� DTI.
//		��ü Data���⿡�� Update�� �� Ȯ�ν� �̿��Ѵ�.
//		�ڷ����ڰ� �ƴ� ������Ŷ�� ���� �ǽð��� ������ ��츸 �̿�ȴ�. (�װ͵� ��ġ�ε�)
//		�׿��� ���� -1������ ������ ���� Update������ �̿�ȴ�.
protected:
	int		m_nUpdateOrAppendIndex;		
public:
	void	SetUpdateOrAppendIndex( const int nUpdateOrAppendIndex);
	int		GetUpdateOrAppendIndex( void) const;


// (2009/10/19 - Seung-Won, Bae) Comment. ������ Real�� �ݿ��� DTI Range?
//		��ǥ ���� CMainChartFormulateImplementation::GetDataPoint()���� ��ȸ�Ѵ�.
//			(������ range�� �̿���� ������ start�θ� �̿�Ǵ� ������ �Ǵܵȴ�.)
protected:
	CPoint	m_ptUpdateRange; // real �� �ݿ��� ���� (x:start, y:end)
public:
	void	SetUpdateRangeOffset( const int nStart, const int nEnd);
	void	SetPacketMinMaxAndUpdateRage(CPacket* pPacket);
	CPoint	GetUpdateRange( void) const;


// (2009/10/19 - Seung-Won, Bae) Comment. Real Data�� �����ϴ� Logic Class�̴�.
//		���� RDC�θ� �̿�ǰ� ����. ER�� ������ RDC = 100 / ER �� ������ �����Ǵ� ����� ���̹Ƿ� RDC�� ������ �ٶ����ϴ�.
protected:
	CRealDataModule		m_realDataModule;
public:
						// �����޽����� RDC �������� �����ϴ� Interface
	bool				SetRealDataModuleAndData( const bool bIsRemove, const CString& strModuleHelpMsg, const bool bIsExchangeRate_Percent);


// (2009/10/20 - Seung-Won, Bae) Nƽ�� ���� ���������� �� Tick�� ���� (�����޽��� UTEC�� ���� ������)
protected:
	int		m_nAddTimeInterval; // Tick������ ���
public:
	void	SetAddTimeInterval( const int nData);


// (2009/10/19 - Seung-Won, Bae) Comment. UD �����޽����� (�ֱⰪ)�� �����Ѵ�.
protected:
	CScaleBaseData::HORZSCALEDRAWERTYPE		m_eDateUnitType; // "UD" : ��,��,��,��,ƽ... ����
public:
	void									SetDateUnitType( const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType);
	CScaleBaseData::HORZSCALEDRAWERTYPE		GetDateUnitType( void) const;


// (2009/10/20 - Seung-Won, Bae) UT ������ 1���� �����ϱ� ���� UD�� �����̴�.
protected:
	int		m_nTimeInterval;	// "UT" : ���ϳ��� ����
public:
	void	SetTimeInterval( const int nTimeInterval);
	int		GetTimeInterval( void) const;
	__int64 GetTimeIntervalWithSecond( void);


// (2009/10/19 - Seung-Won, Bae) Real Data Processing Interface.
public:
	CPacket *	m_pOpenPacket;
	CPacket *	m_pHighPacket;
	CPacket *	m_pLowPacket;
	CPacket *	m_pClosePacket;
	CPacket *	m_pVolumePacket;
public:
			// �ǽð� Data�� �߰� �Ǵ� Update ó���Ѵ�.
			// Ư�� ���� �ŷ���, �ŷ������ �޴� ���, �߰��� ���� ���� ���� ���Ͽ� ������ �߰��ǵ��� �Ѵ�.
	bool	SetAppendOrUpdateData_DataInDate( CPacket* pPacket, const int nBaseCount, const double& dData, double& dPrevData);
			// �ǽð� Data �߰���, Base Packet�� Data Count�� ���� ������ �κ��� ���� ������ Append�� �ڿ� Append ó���� �Ѵ�.
	bool	SetAppendData( CPacket* pPacket, const int nBaseCount, const double& dData, const double& dPrevData);
			// ���� ������ Update�� ó���Ѵ�. (���� ������ ���ų� ���� ��츸 ó���Ѵ�.)
	bool	SetUpdateData_Compare( CPacket* pPacket, const int nBaseCount, const double& dData, const bool bLowCheck);
			// �ǽð� Data�� ó���ϴ� Main Routine. (����/�ڷ�����/������Ŷ/��Ÿ �� ���еǾ� ó���ȴ�.)
	bool	SetRealData( CPacket* pPacket, const double dData, const int nOption, int &p_nRemovePacketDataCount);
			// ����� ���� �ð��帧�� �����Ѵ�. (����ð��� ���� ���� �߰��Ǵ��� ���θ� Return�Ѵ�.)
	bool	GetRealData_CloseTime( CPacket* pPacket, const __int64 &p_nClock, __int64 &p_nRealDataTime);


// (2009/10/20 - Seung-Won, Bae) Comment. ���� �޽����� ������ ���ڰ��� �����Ѵ�.
//		�ǽð� ���Ž� �ǽð� Data�� ���ڰ��� �����ϱ����Ͽ� ���ȴ�.
//		�ϰ� �ֱ��� ������ ������ ����� ó���� ��� ����?
protected:
	CDateData	m_dateData;	// ���� ��¥ (����ó���� PC��¥�� �ƴ� ���� ��¥�� �Ǵ���)
public:
				// ������ ���������� ���ڷ� �ʱ�ȭ�Ѵ�.
				//		�����޽��� DATE ��.
				//		�����޽��� DATATIME ��.
				//		�����޽��� DATATIME ������ ���� �ŷ��� ���ڸ� �������� ������� ������.
				//				�������� �����ð� 0�ø� �������� �ٲ��, �ŷ��� �ð����� ���߿� ����ȴ�.
				//				�����ð��� ���������� �������� ��/��/������ ���� ������ �ȴ�.
	void		SetCurrentDate(const CString& strData);


// (2009/10/20 - Seung-Won, Bae) MARKETTIME �����޽����� ������ �����, �����ð��� �����Ѵ�.
protected:
	CMarketTimeData		m_marketTimeData; // ����۽ð�,�帶���ð�
public:
						// MARKETTIME �����޽����� ���ŵ� �����/������ �����Ѵ�.
	void				SetMarketTime( const CString& strData);
						// X ScaleManager�� �ٸ� �ð����� Scale ó���� ���Ͽ� �����/�����ð��� ��ȸ�Ѵ�.
	bool				GetMarketTime(int& nMarketStartTime, int& nMarketEndTime) const;
						// (2009/11/3 - Seung-Won, Bae) Make Market Date to Real Date of WorldOn Real Data.
	void				GetRealData_RealDateTime( double &p_dData) const;


// (2009/10/20 - Seung-Won, Bae) Comment. �к� ó�� Logic Class�� �����Ѵ�.
protected:
	CTimeRealDataModuleBase *	m_pTimeRealDataModule;
public:
							// �к� ó�� Logic Type�� ���Ѵ�.
	CPacketType::REALTYPE	GetRealType( void) const;
							// �к��� �̷�/���� ������ �߷��ϱ� ���Ͽ� �������� ���� ���۽ð��� ��ȸ�Ѵ�.
	__int64					GetStartTimeInTimeRangeOfLastData( void);


// (2009/10/21 - Seung-Won, Bae) Comment. ���� �ٸ� �ð��븦 ���ϱ� ���� ��������,
//		���� �ŷ����� ����/�ð��� �����Ѵ�. (��ȸ�����ΰ�?)
protected:
	int			m_nBasisTime;
public:
	void		SetCurrDateTime(const CString& strData);	// TR ��ȸ ����+�ð� ( "YYYYMMDDHHMMSS" )
	__int64		GetCurrDateTime() const;					// TR ��ȸ ����+�ð� ( "YYYYMMDDHHMMSS" )


// (2009/10/21 - Seung-Won, Bae) Comment. 1������ ���� 0�ø� �Ѿ�鼭 �����ϴ����� ��Ÿ���� Type�� �����Ѵ�.
protected:
	CPacketType::MARKETDAYTYPE	m_nMarketDayType;
public:
	CPacketType::MARKETDAYTYPE	GetMarketDayType();


// (2009/10/21 - Seung-Won, Bae) Comment. �ǽð� Data�� Packet Type�� �����Ѵ�.
protected:
	CString		m_strRealTimeType;	//�ǽð�����Ÿ�� ����+�ð��� Data Type(default�� HHMMSS)
public:
	void		SetRealTimeType(const CString& strData);	//�ǽð�����Ÿ ��Ŷ�� ���� ������ ����
	CString		GetRealTimeType( void)						{	return m_strRealTimeType;	}


// (2009/10/21 - Seung-Won, Bae) Comment. �ǽð� ó���� ���� ȯ�氪�� �ʱ�ȭ�Ѵ�.
public:
			// �����޽��� ���� �� �ʱ�ȭ
	void	InitializeHelpMessage( void);
			// �����޽��� �� TR Data ������ �ʱ�ȭ
	void	InitializeTRPart( CPacket *p_pOpenPacket, CPacket *p_pHighPacket, CPacket *p_pLowPacket, CPacket *p_pClosePacket, CPacket *p_pVolumePacket);


// (2009/10/22 - Seung-Won, Bae) Comment. X Scale Manager���� �ð� Merge�� �����ϵ��� �к� �ø� ���θ� �����Ѵ�.
protected:
	CPacketType::REALTYPE	m_eRealType;

// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
protected:
	CChartInfo::CHART_MODE	m_nChartModeSetting;
public:
	CChartInfo::CHART_MODE	GetChartMode( void) const		{	return m_nChartModeSetting;		}
							// (2009/12/2 - Seung-Won, Bae) Notify to other modules.
	void					ChangeChartMode( int p_nChartMode)	{	m_nChartModeSetting = ( CChartInfo::CHART_MODE)p_nChartMode;	}


// (2009/11/1 - Seung-Won, Bae) for Normalized Candle data.
public:
	double		GetOrgDataBeforeRate( const char *p_szPacketName, const double p_dRatedData);


// 20080925 JS.Kim	���� �ٸ� �ð����� Scale ó��
protected:
	int			m_nOrgBaseTimeDiff;
	int			m_nBaseTimeDiff;
public:
	void		SetTimeDiffInfo( const int &p_nOrgBaseTimeDiff, const int &p_nBaseTimeDiff);


// (2009/11/1 - Seung-Won, Bae) Support Last Real Time.
protected:
	__int64		m_nClock;
	__int64		m_nLastClockTime;
	__int64		m_nLastRealTime;
protected:
	void		SetLastRealTime( const __int64 &p_nRealTime);

// (2009/11/4 - Seung-Won, Bae) Backup to receive NMTR Data.
protected:
	IChartCtrl *	m_pIChartCtrl;
protected:
	BOOL		m_bOnClockedData;
	__int64		m_nLastRealTimeBackup;
public:
	void		PushNMTRState( __int64 p_nRealDataTime);
	void		PopNMTRState( void);
				// (2009/11/12 - Seung-Won, Bae) Get NMTR Info
	BOOL		GetLastClockAndRealTime( __int64 &p_nLastClockTime, __int64 &p_nLastRealTime);

// (2009/11/16 - Seung-Won, Bae) Base Time of N-Minite
protected:
	int			m_nNewRangeStartTime;
public:
	void		SetNewRangeStartTime( const CString &p_strTime);

// (2009/11/16 - Seung-Won, Bae) Manage Market Days
protected:
	CString		m_strMarketDayInfo;
public:
	void		SetMarketDayInfo( const CString &p_strMarketDayInfo)	{	m_strMarketDayInfo = p_strMarketDayInfo;	}
};

#endif // !defined(AFX_REALMODULECENTER_H__36233F7E_0FC6_4D5A_9F9F_698C7F318819__INCLUDED_)
