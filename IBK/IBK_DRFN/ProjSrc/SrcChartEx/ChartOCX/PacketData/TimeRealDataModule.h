// TimeRealDataModule.h: interface for the CTimeRealDataModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEREALDATAMODULE_H__AA27934F_EA58_40D3_960A_440C19829896__INCLUDED_)
#define AFX_TIMEREALDATAMODULE_H__AA27934F_EA58_40D3_960A_440C19829896__INCLUDED_

#include <afxtempl.h>			// for CTypedPtrMap

#include "../Include_Chart/BlockBaseEnum.h"
#include "../Include_Chart/TimeTypeConvert.h"		// for CTimeTypeConvert

#include "PacketBaseData.h"							// for CMarketTimeData

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDateData;
class CPacket;

class CBaseData
{
public:
	enum RESULT
	{
		Error = 0,
		TodayFirstReal,
		Add,
		Update,
		AddNMTime,		// 2008.08.25 JS.Kim ����� ó��
		UpdateNMTime,
		UpdateCloseTime
	};

	enum MARKET_STATUS
	{
		Close		= 0,
		Trading		= 1,
		NoDefine	= 2
	};
};


// (2009/10/28 - Seung-Won, Bae) Real �ð� ó�� Logic Class�� �ֻ��� Base
class CTimeRealDataModuleBase
{
public:
	// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	CTimeRealDataModuleBase( CMarketTimeData &p_marketTimeData, const CString& strPacketType, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType);
	virtual ~CTimeRealDataModuleBase();


// (2009/10/21 - Seung-Won, Bae) Comment. �ڷ����� Packet�� Data Type�� �����Ѵ�. (ex : "YYYYMMDDHHMMSS")
protected:
	CTimeTypeConvert	m_ttcTrPacket;


// (2009/11/19 - Seung-Won, Bae) ��ȸ�� ������ Data�� �ǽð� ���� ����� �������� ���Ѵ�.
//	�� �������� �������� �ǽð� ���Ž� ������ Data�� Update�� ������ Append�� ������ �����ȴ�.
//		Ư�� Append�ÿ��� ���� ������ ������Ű�� Update���� ó���ȴ�.
//		���� ������ ���� Update�ϱ� ���� �ǽð� �������� �ð������� ���ϴ� ���̴�.
//	�� ������ ������ Data������ ���� ���۱��ؽ÷� �⺻ ������ �����ϰ�, ������ Data�� �ǽð� Data�� ��ó�� ���ԵǴ� �������� �˻��Ѵ�.
//	������ Data ��ü�� ���� ���, ������ �������� �ʰ�, ù �ǽð��� ���� ��, ���� ���� ���� ������� ���Ѵ�.
//		���� �̷��� ������ �������� ���� ��Ȳ������ ����� ������ �� ������,
//		ù �Ǻ� ���Žô� ������ Append�� ó���� ������ �����Ѵ�.
public:
	virtual BOOL	SetTimeRange_TRPart( const __int64 &p_nServerTime, __int64 nTailTime)
	{
		// (2009/11/13 - Seung-Won, Bae) �߰��� ����� RollBack ��Ű��.
		//	�߰��� ����� ���� ��ŭ Ȯ���ϸ鼭, ������ ��� ������ ������Ű�鼭 �����ؾ��� Data ������ ���Ѵ�.
		//  �� �������(m_bCloseEmptyCandleWhenEnd:TRUE)�� ù���� �Ǻ��� ������Ű�Ƿ�,
		//		��� Data ������ -1���� ���۵ǵ��� �Ѵ�.
		m_nClockedDataCount = 0;
		return TRUE;
	}


// (2009/10/22 - Seung-Won, Bae) Comment. ���ŵ� �ǽð��� �ø�/���� ���� ���Ͽ� �� Logic Class�� ���� Packet�� �ݿ� ó���Ѵ�.
public:
	virtual int		SetRealData( CPacket* pPacket, const __int64 &nRealTime)					{	return -1;	}


// (2009/10/21 - Seung-Won, Bae) Comment. ���� �ǽð��� �����ϴ� ���� ���� ����/�� �ð����� �����Ѵ�. (Packet Type���̴�.)
public:
	virtual __int64		GetTimeRangeStartWithPacketType( void) const							{	return 0;		}


// ����� ���� �ð��帧�� �����Ѵ�. (����ð��� ���� ���� �߰��Ǵ��� ���θ� Return�Ѵ�.)
protected:
	int				m_nClockedDataCount;
	int				m_nClockedDataCountBackup;
public:
	virtual bool	GetRealData_CloseTime( CPacket* pPacket, __int64 &p_nRealDataTime)			{	return false;	}
	void			PushNMTRState( void)	{	m_nClockedDataCountBackup = m_nClockedDataCount;	m_nClockedDataCount = 0;	}
	void			PopNMTRState( void)		{	m_nClockedDataCount = m_nClockedDataCountBackup;	}


// 20080925 JS.Kim	���� �ٸ� �ð����� Scale ó��
protected:
	__int64		m_nRealTimeDiff;
	int			m_nNewRangeStartTime;
public:
	void		OnInitTR( const __int64 &p_nRealTimeDiff, const int &p_nNewRangeStartTime, const CString &p_szMarketDayInfo);
	void		SetRealTimeDiff( const __int64 &p_nRealTimeDiff)	{	m_nRealTimeDiff = p_nRealTimeDiff;	}


// (2009/11/4 - Seung-Won, Bae) Calculate Market Date.
protected:
	CMarketTimeData &	m_marketTimeData; // ����۽ð�,�帶���ð�
public:
	void				GetMarketDate_DateTime( __int64 &p_nData) const;


// (2009/10/27 - Seung-Won, Bae) for Real Data Type.
protected:
	CChartInfo::CHART_MODE	m_nChartModeSetting;
public:
	void					SetChartMode( CChartInfo::CHART_MODE p_nChartModeSetting)			{	m_nChartModeSetting = p_nChartModeSetting;		}


// (2009/11/4 - Seung-Won, Bae) Empty Candle Closing Time Type.
protected:
	BOOL	m_bCloseEmptyCandleWhenEnd;

// (2009/11/12 - Seung-Won, Bae) for NMTR Step Width
public:
	virtual __int64		GetTimeIntervalWithSecond( void)		{	return 0;	}

// (2009/11/16 - Seung-Won, Bae) Check for Market Days
protected:
	CMapPtrToPtr		m_mapAddedMarketDays;
	CMapPtrToPtr		m_mapRemovedMarketDays;
public:
	BOOL				IsMarketDay( int p_nDate);

	// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	CScaleBaseData::HORZSCALEDRAWERTYPE		m_eDateUnitType; // "UD" : ��,��,��,��,ƽ... ����
	// 2011.01.21 by SYS <<
};


// (2009/10/28 - Seung-Won, Bae) Tick ó�� Logic Class
class CTickRealDataModule : public CTimeRealDataModuleBase
{
public:
	// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	CTickRealDataModule( CMarketTimeData &p_marketTimeData, const CString& strPacketType, const int nTimeInterval, const int nAddTimeInterval, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType);
	virtual ~CTickRealDataModule();

// (2009/10/21 - Seung-Won, Bae) Comment. Nƽ ���� ���� ƽ�� �����Ѵ�. (UT��)
protected:
	__int64		m_nTimeInterval;	// �а���

// (2009/10/20 - Seung-Won, Bae) Nƽ�� ���� ���������� �� Tick�� ���� (�����޽��� UTEC�� ���� ������)
protected:
	int		m_nAddTimeInterval; // Tick������ ���

// (2009/10/22 - Seung-Won, Bae) Comment. ���ŵ� �ǽð��� �ø�/���� ���� ���Ͽ� �� Logic Class�� ���� Packet�� �ݿ� ó���Ѵ�.
protected:
	virtual int		SetRealData( CPacket* pPacket, const __int64 &nRealTime);

// (2009/11/12 - Seung-Won, Bae) for NMTR Step Width
protected:
	virtual __int64		GetTimeIntervalWithSecond( void)		{	return 1;	}
};


// (2009/10/28 - Seung-Won, Bae) Date(��/��) ó�� Logic Class
class CDateRealDataModule : public CTimeRealDataModuleBase
{
public:
	// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	CDateRealDataModule( CMarketTimeData &p_marketTimeData, const CString& strPacketType, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType, BOOL p_bMonthChart = FALSE);
	virtual ~CDateRealDataModule();

// (2009/10/22 - Seung-Won, Bae) Comment. ���ŵ� �ǽð��� �ø�/���� ���� ���Ͽ� �� Logic Class�� ���� Packet�� �ݿ� ó���Ѵ�.
protected:
	virtual int		SetRealData( CPacket* pPacket, const __int64 &nRealTime);

// (2009/11/22 - Seung-Won, Bae) ��/������ ����� ���� Update ����� �ʿ����. (��ȸ ������ �ʿ����.)
//	����, ��/�� ������Ŀ� ���� ������ ��/�� ��, �Ǵ� ���� ��/�� ������ ���Ͽ� ���� �����͸� �߻���Ų��.
//	�̶� �Ϻ��� ��� �������� Ȯ���Ͽ��� �Ѵ�.
protected:
	virtual bool	GetRealData_CloseTime( CPacket* pPacket, __int64 &p_nRealDataTime);

protected:
	BOOL	m_bMonthChart;

// (2009/11/12 - Seung-Won, Bae) for NMTR Step Width
protected:
	virtual __int64		GetTimeIntervalWithSecond( void);
};


// (2009/10/28 - Seung-Won, Bae) Week(��) ó�� Logic Class
class CWeekRealDataModule : public CTimeRealDataModuleBase
{
public:
	// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	CWeekRealDataModule( CMarketTimeData &p_marketTimeData, const CString& strPacketType, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType);
	virtual ~CWeekRealDataModule();

// (2009/11/19 - Seung-Won, Bae) ��ȸ�� ������ Data�� �ǽð� ���� ����� �������� ���Ѵ�.
//	�� �������� �������� �ǽð� ���Ž� ������ Data�� Update�� ������ Append�� ������ �����ȴ�.
//		Ư�� Append�ÿ��� ���� ������ ������Ű�� Update���� ó���ȴ�.
//		���� ������ ���� Update�ϱ� ���� �ǽð� �������� �ð������� ���ϴ� ���̴�.
//	�� ������ ������ Data������ ���� ���۱��ؽ÷� �⺻ ������ �����ϰ�, ������ Data�� �ǽð� Data�� ��ó�� ���ԵǴ� �������� �˻��Ѵ�.
//	������ Data ��ü�� ���� ���, ������ �������� �ʰ�, ù �ǽð��� ���� ��, ���� ���� ���� ������� ���Ѵ�.
//		���� �̷��� ������ �������� ���� ��Ȳ������ ����� ������ �� ������,
//		ù �Ǻ� ���Žô� ������ Append�� ó���� ������ �����Ѵ�.
protected:
	virtual BOOL	SetTimeRange_TRPart( const __int64 &p_nServerTime, __int64 nTailTime);

// (2009/10/22 - Seung-Won, Bae) Comment. ���ŵ� �ǽð��� �ø�/���� ���� ���Ͽ� �� Logic Class�� ���� Packet�� �ݿ� ó���Ѵ�.
protected:
	virtual int		SetRealData( CPacket* pPacket, const __int64 &nRealTime);

// (2009/10/29 - Seung-Won, Bae) Get the last monday date.
protected:
	int		GetLastMondayDate( __int64 p_nTime);

// ����� ���� �ð��帧�� �����Ѵ�. (����ð��� ���� ���� �߰��Ǵ��� ���θ� Return�Ѵ�.)
protected:
	virtual bool	GetRealData_CloseTime( CPacket* pPacket, __int64 &p_nRealDataTime);

// (2009/11/12 - Seung-Won, Bae) for NMTR Step Width
protected:
	virtual __int64		GetTimeIntervalWithSecond( void);
};


// (2009/10/28 - Seung-Won, Bae) Comment. �ð����� Logic Class�� Base
class CTimeRealDataModule : public CTimeRealDataModuleBase
{
public:
	// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	CTimeRealDataModule( const CDateData& dateData, CMarketTimeData &p_marketTimeData, const CString& strPacketType, int nTimeInterval, const CString& strMarketStartTime, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType);
	virtual ~CTimeRealDataModule();


// (2009/10/22 - Seung-Won, Bae) Comment. �к� �ø� ���θ� �����Ѵ�.
protected:
	virtual CPacketType::REALTYPE	GetRealType( void) const = 0;


// (2009/10/21 - Seung-Won, Bae) Comment. N ��/��/�� ���� ���� ���� �����Ѵ�. (UT��)
protected:
	__int64		m_nTimeInterval;
// (2009/10/21 - Seung-Won, Bae) Comment. ����� �ð��� HHMMSS�� �����Ѵ�. (�ǽð� ���۽ð� Ȯ�ο�)
protected:
	int			m_nMarketStartTime;
	CString		m_strMarketStartTime;	// ����۽ð�(HHMMSS Ÿ������ �����Ѵ�.)


// (2009/10/21 - Seung-Won, Bae) Comment. ���� �ǽð��� �����ϴ� ���� ���� ����/�� �ð����� �����Ѵ�. (Packet Type���̴�.)
protected:
	__int64				m_nTimeRangeStart;		// update �� ����(����)	~ Floor Type �������� ���������θ� ���δ�.
	__int64				m_nTimeRangeEnd;		// update �� ����(��)	~ GetNextIntervalWithoutOffTime()�� ���� ������, Update/Appen ��Ȳ���� �ĺ��ϴ� ���� �� Ceil Type ������������ ���δ�.
protected:
	virtual __int64		GetTimeRangeStartWithPacketType( void) const;


// (2009/11/19 - Seung-Won, Bae) ��ȸ�� ������ Data�� �ǽð� ���� ����� �������� ���Ѵ�.
//	�� �������� �������� �ǽð� ���Ž� ������ Data�� Update�� ������ Append�� ������ �����ȴ�.
//		Ư�� Append�ÿ��� ���� ������ ������Ű�� Update���� ó���ȴ�.
//		���� ������ ���� Update�ϱ� ���� �ǽð� �������� �ð������� ���ϴ� ���̴�.
//	�� ������ ������ Data������ ���� ���۱��ؽ÷� �⺻ ������ �����ϰ�, ������ Data�� �ǽð� Data�� ��ó�� ���ԵǴ� �������� �˻��Ѵ�.
//	������ Data ��ü�� ���� ���, ������ �������� �ʰ�, ù �ǽð��� ���� ��, ���� ���� ���� ������� ���Ѵ�.
//		���� �̷��� ������ �������� ���� ��Ȳ������ ����� ������ �� ������,
//		ù �Ǻ� ���Žô� ������ Append�� ó���� ������ �����Ѵ�.
protected:
	virtual BOOL	SetTimeRange_TRPart( const __int64 &p_nServerTime, __int64 nTailTime);
					// (2009/10/22 - Seung-Won, Bae) �ǽð��� ���� �����ϴ� �ð����� ������ nEndTime�̰�, �ǽð� ���� nCurTime�� ���, Append ��Ȳ���� Ȯ���ϴ� Logic.
					//		�� ó�� Logic Class ���� �ٸ��� ó���ȴ�.
	virtual bool	IsTimeAdd( const __int64 &nCurTime, const __int64 &nEndTime) const = 0;


// (2009/10/22 - Seung-Won, Bae) Comment. ���ŵ� �ǽð��� �ø�/���� ���� ���Ͽ� �� Logic Class�� ���� Packet�� �ݿ� ó���Ѵ�.
protected:
					// (2009/11/20 - Seung-Won, Bae) ����! ��ȸ�����Ͱ� ���� ��� m_nTimeRangeStart, m_nTimeRangeEnd ���� -1�̴�.
					//		����� Ȯ���� �ʿ��ϴ�.
	virtual int		SetRealData( CPacket* pPacket, const __int64 &nRealTime);
protected:
						// (2009/10/22 - Seung-Won, Bae) �ǽð� �ð��� �ø�/���������� �� Packet���� �����ϰ� �űԺ� ������ �������� �ٽ� �����Ѵ�.
						//		Logic Class�� ���� Update/Append Ȯ�ο� �ð�, Update�� �ð�, �������� �ð��� �޸��Ͽ�,
						//		SetRealData_AppendOrUpdate()�� ���� ó���ȴ�.
	virtual int			SetRealData_AppendOrUpdate( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nFloorTime, const __int64 &nCeilTime) = 0;
						// (2009/10/22 - Seung-Won, Bae) nCheckTime�� �������� Update/Append ���θ� �����ϰ�,
						//		�� Data�δ� nUpdateTime�� �ݿ��Ѵ�. �������ÿ��� nCloseTime���� �����Ѵ�.
	int					SetRealData_AppendOrUpdate2( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nCheckTime, const __int64 &nUpdateTime);
						// (2009/10/27 - Seung-Won, Bae) �к� ������ ���Žð��� ��ȸ�Ѵ�.
	virtual __int64		GetCloseTime( const __int64 &p_nTimeRangeStart, const __int64 &p_nTimeRangeEnd) const	= 0;


protected:
	const CDateData& m_dateData; // ���� ��¥

// (2009/10/27 - Seung-Won, Bae) the Flag for real after special code like market end.
protected:
	BOOL	m_bMarketOn;

// ����� ���� �ð��帧�� �����Ѵ�. (����ð��� ���� ���� �߰��Ǵ��� ���θ� Return�Ѵ�.)
protected:
	virtual bool	GetRealData_CloseTime( CPacket* pPacket, __int64 &p_nRealDataTime);
	virtual bool	GetRealData_CloseTime_AppendOrUpdate( CPacket* pPacket, const __int64 &nFloorTime, const __int64 &nCeilTime, __int64 &p_nRealDataTime) = 0;
	bool			GetRealData_CloseTime_AppendOrUpdate2( CPacket* pPacket, const __int64 &nCheckTime, const __int64 &nUpdateTime, __int64 &p_nRealDataTime);
	BOOL			GetNextIntervalWithoutOffTime( __int64 &p_nTimeRangeStart, __int64 &p_nTimeRangeEnd, BOOL p_bWithoutOffTime = TRUE);
	void			GetPrevIntervalWithoutOffTime( __int64 &p_nTimeRangeStart, __int64 &p_nTimeRangeEnd, BOOL p_bWithoutOffTime = TRUE);
					// (2009/11/21 - Seung-Won, Bae) ������ ������ �������� ���θ� Ȯ���Ѵ�.
	BOOL			IsOnMarketTimeRange( const __int64 &p_nTimeRangeStart, const __int64 &p_nTimeRangeEnd, const int &nStartTime, const int &nEndTime, const int &nMarketStartTime, const int &nMarketEndTime);
	BOOL			IsOnMarketDay( __int64 p_nDateCheckTime);

// (2009/11/12 - Seung-Won, Bae) for NMTR Step Width
protected:
	virtual __int64		GetTimeIntervalWithSecond( void);
};


// 09:01:00 ~ 09:01:59 -> 09:01 : ����
class CFloorTimeRealDataModule: public CTimeRealDataModule  
{
public:
	// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	CFloorTimeRealDataModule( const CDateData& dateData, CMarketTimeData &p_marketTimeData, const CString& strPacketType, const int nTimeInterval, const CString& strMarketStartTime, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType);


// (2009/10/22 - Seung-Won, Bae) Comment. �к� �ø� ���θ� �����Ѵ�.
protected:
									//����. 09:01:00 ~ 09:01:59 -> 09:01
	virtual CPacketType::REALTYPE	GetRealType( void) const	{	return CPacketType::FLOOR;	}


protected:
					// (2009/10/22 - Seung-Won, Bae) �ǽð��� ���� �����ϴ� �ð����� ������ nEndTime�̰�, �ǽð� ���� nCurTime�� ���, Append ��Ȳ���� Ȯ���ϴ� Logic.
					//		�� ó�� Logic Class ���� �ٸ��� ó���ȴ�.
	virtual bool	IsTimeAdd( const __int64 &nCurTime, const __int64 &nEndTime) const;


protected:
						// (2009/10/22 - Seung-Won, Bae) �ǽð� �ð��� �ø�/���������� �� Packet���� �����ϰ� �űԺ� ������ �������� �ٽ� �����Ѵ�.
						//		Logic Class�� ���� Update/Append Ȯ�ο� �ð�, Update�� �ð�, �������� �ð��� �޸��Ͽ�,
						//		SetRealData_AppendOrUpdate()�� ���� ó���ȴ�.
	virtual int			SetRealData_AppendOrUpdate( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nFloorTime, const __int64 &nCeilTime);
						// (2009/10/27 - Seung-Won, Bae) �к� ������ ���Žð��� ��ȸ�Ѵ�.
	virtual __int64		GetCloseTime( const __int64 &p_nTimeRangeStart, const __int64 &p_nTimeRangeEnd) const	{	return	p_nTimeRangeStart;	}


// ����� ���� �ð��帧�� �����Ѵ�. (����ð��� ���� ���� �߰��Ǵ��� ���θ� Return�Ѵ�.)
protected:
	virtual bool	GetRealData_CloseTime_AppendOrUpdate( CPacket* pPacket, const __int64 &nFloorTime, const __int64 &nCeilTime, __int64 &p_nRealDataTime);
};


// 09:00:01 ~ 09:01:00 -> 09:01 : �ø�. �޸���
class CCeilingTimeRealDataModule: public CTimeRealDataModule    
{
public:
	// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	CCeilingTimeRealDataModule( const CDateData& dateData, CMarketTimeData &p_marketTimeData, const CString& strPacketType, const int nTimeInterval, const CString& strMarketStartTime, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType);


// (2009/10/22 - Seung-Won, Bae) Comment. �к� �ø� ���θ� �����Ѵ�.
protected:
									// �ø�. 09:00:01 ~ 09:01:00 -> 09:01
	virtual CPacketType::REALTYPE	GetRealType( void) const	{	return CPacketType::CEILING;	}


protected:
					// (2009/10/22 - Seung-Won, Bae) �ǽð��� ���� �����ϴ� �ð����� ������ nEndTime�̰�, �ǽð� ���� nCurTime�� ���, Append ��Ȳ���� Ȯ���ϴ� Logic.
					//		�� ó�� Logic Class ���� �ٸ��� ó���ȴ�.
	virtual bool	IsTimeAdd( const __int64 &nCurTime, const __int64 &nEndTime) const;


protected:
						// (2009/10/22 - Seung-Won, Bae) �ǽð� �ð��� �ø�/���������� �� Packet���� �����ϰ� �űԺ� ������ �������� �ٽ� �����Ѵ�.
						//		Logic Class�� ���� Update/Append Ȯ�ο� �ð�, Update�� �ð�, �������� �ð��� �޸��Ͽ�,
						//		SetRealData_AppendOrUpdate()�� ���� ó���ȴ�.
	virtual int			SetRealData_AppendOrUpdate( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nFloorTime, const __int64 &nCeilTime);
						// (2009/10/27 - Seung-Won, Bae) �к� ������ ���Žð��� ��ȸ�Ѵ�.
	virtual __int64		GetCloseTime( const __int64 &p_nTimeRangeStart, const __int64 &p_nTimeRangeEnd) const	{	return	p_nTimeRangeEnd;	}


// ����� ���� �ð��帧�� �����Ѵ�. (����ð��� ���� ���� �߰��Ǵ��� ���θ� Return�Ѵ�.)
protected:
	virtual bool	GetRealData_CloseTime_AppendOrUpdate( CPacket* pPacket, const __int64 &nFloorTime, const __int64 &nCeilTime, __int64 &p_nRealDataTime);
};

// (2009/6/15 - Seung-Won, Bae) Koscom is new ceiling type.
// 09:00:00 ~ 09:00:59 -> 09:01 : �ø� (KOSCOM)
class CCeiling2TimeRealDataModule: public CCeilingTimeRealDataModule    
{
public:
	// 2011.01.21 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	CCeiling2TimeRealDataModule( const CDateData& dateData, CMarketTimeData &p_marketTimeData, const CString& strPacketType, const int nTimeInterval, const CString& strMarketStartTime, const CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType);


// (2009/10/22 - Seung-Won, Bae) Comment. �к� �ø� ���θ� �����Ѵ�.
protected:
									// �ø�. 09:00:00 ~ 09:00:59 -> 09:01
	virtual CPacketType::REALTYPE	GetRealType( void) const	{	return CPacketType::CEILING2;	}


protected:
					// (2009/10/22 - Seung-Won, Bae) �ǽð��� ���� �����ϴ� �ð����� ������ nEndTime�̰�, �ǽð� ���� nCurTime�� ���, Append ��Ȳ���� Ȯ���ϴ� Logic.
					//		�� ó�� Logic Class ���� �ٸ��� ó���ȴ�.
	virtual bool	IsTimeAdd( const __int64 &nCurTime, const __int64 &nEndTime) const;


protected:
						// (2009/10/22 - Seung-Won, Bae) �ǽð� �ð��� �ø�/���������� �� Packet���� �����ϰ� �űԺ� ������ �������� �ٽ� �����Ѵ�.
						//		Logic Class�� ���� Update/Append Ȯ�ο� �ð�, Update�� �ð�, �������� �ð��� �޸��Ͽ�,
						//		SetRealData_AppendOrUpdate()�� ���� ó���ȴ�.
	virtual int			SetRealData_AppendOrUpdate( CPacket* pPacket, const __int64 &nRealTime, const __int64 &nFloorTime, const __int64 &nCeilTime);
						// (2009/10/27 - Seung-Won, Bae) �к� ������ ���Žð��� ��ȸ�Ѵ�.
	virtual __int64		GetCloseTime( const __int64 &p_nTimeRangeStart, const __int64 &p_nTimeRangeEnd) const	{	return	p_nTimeRangeEnd;	}


// ����� ���� �ð��帧�� �����Ѵ�. (����ð��� ���� ���� �߰��Ǵ��� ���θ� Return�Ѵ�.)
protected:
	virtual bool	GetRealData_CloseTime_AppendOrUpdate( CPacket* pPacket, const __int64 &nFloorTime, const __int64 &nCeilTime, __int64 &p_nRealDataTime);
};

#endif // !defined(AFX_TIMEREALDATAMODULE_H__AA27934F_EA58_40D3_960A_440C19829896__INCLUDED_)
