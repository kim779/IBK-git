// PacketList.h: interface for the CPacketList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETLIST_H__67D8B2C1_2E6F_11D5_81A5_0050FC28B229__INCLUDED_)
#define AFX_PACKETLIST_H__67D8B2C1_2E6F_11D5_81A5_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "../BlockBaseData.h"

#include "PacketBase.h"

interface IChartCtrl;
class CPacketListImplementation;
class AFX_EXT_CLASS CPacketList  
{
public:	
	CPacketList( const CPacketType::REALTYPE eRealType, IChartCtrl *p_pIChartCtrl);
	CPacketList( CString strPacketOption, const CPacketType::REALTYPE eRealType, IChartCtrl *p_pIChartCtrl);
	virtual ~CPacketList();

	void SetVersion(double dVersion);
	void SetPacketLengthCount(int nLength);
	bool SetData(const CString szPacketName, int nIndex, double dValue, bool IsByReal);

	CPacketType::REALTYPE GetRealType() const;

	// (2004.06.18, ��¿�) Data Byte���� Long���� �����Ѵ�.
	void SetPacketData(const CString& szPacketName, long pData, long lCount);

	bool SetPacketData(const CString& strPacketName, const CString& strPacketType, 
		const char* pPacketData, const int nCount);
	void AddPacket(const CString szPacketOption);
	bool RemovePacket(const CString szPacketName);

	// (2009/10/19 - Seung-Won, Bae) Comment. ������ �ǽð� ���ŵ� DTI.
	//		��ü Data���⿡�� Update�� �� Ȯ�ν� �̿��Ѵ�.
	int		GetLastestRealIndex();

	//	�ð� ���� ����
	int GetTimeInterval();
	__int64 GetTimeIntervalWithSecond( void);
	// (2006/7/11 - Seung-Won, Bae) Retrived the Next First Time of Data.
	__int64	GetStartTimeInTimeRangeOfLastData( void);
	int	GetEndTimeInTimeRangeOfLastData( void);
	//  TR ���� ����
	int GetTotalTRIndex() const;
	//  ��ü TR data count
	int GetTotalTRCount() const;
	//	�κ� TR data count
	int GetTRCount(const int nTRIndex) const;
	//  TR �߻��� count
	int GetRequestTRCount(const int nCurTRCount) const;
	//	����Ÿ�� ���� �� ����Ÿ�� ������ ����
	int GetMaxCount();
	//�ش� packet�� count
	int GetCount(const CString strPacketName);
	//	Packet�� ������ �ִ� ����Ÿ������ ���� ���� ����Ÿ ������ �����Ѵ�
	//	strPacketList : Packet�� list
	// (2004.06.25, ��¿�) Packet ���� �����ڸ� ���޹ް� �Ѵ�. (�⺻���� ������ �����Ǿ��� ',')
	int GetMaxCount(CString strPacketNameList, const CString& strCompartName = ',');
	//	���� Packet���� Minimun & Maximun Value�� Join�Ѵ�
	//	nStart : ���۵���Ÿ ��ġ
	//	nEnd : ������Ÿ ��ġ
	//	strPacketNameList : Join�� Packet�� List(packet1,packet2,Packet3,...)
	//	dMin : Minimum
	//	dMax : Maximun
	int GetMinMax(int nStart, int nEnd, CString strPacketNameList, double& nMin, double& nMax);
	bool GetAllPacketsMinMax(const int nStart, const int nEnd, double& dMin, double& dMax);

	CList<double, double> * GetnumericDataList(CPacket * pPacket);
	CList<CString, CString> * GetstrDataList(CPacket * pPacket);

	//nami 8.29
	CString GetPacketType(CString strPacketName, const bool bIsFullType = true);
	CString GetPacketType(CPacket * pPacket, const bool bIsFullType = true);

	CString GetStrDataFromIndex(CString strFunction, int nIndex);
	double GetDataFromIndex(CString strFunction, int nIndex);

	double GetCurrentDate();
	//	�ð�(��¥) Ÿ�� ����
	CScaleBaseData::HORZSCALEDRAWERTYPE GetDateUnitType();
	//	data updateŸ�� ���� (NEW, INSERT, APPEND)
	CPacketDataInfo::DATAINSERTTYPE GetDataInsertType();
	// 04.16.2002  -> ����� �����
	CList<CString, CString>* GetRealKeyName();
	CPacketType::PRICETYPE GetPriceType();	

	bool UseRealData();
	bool GetUseReal();
	// 04.17.2002  -> ������� ��������� üũ
	bool FindKey(CString strKey);

	int GetEndDate(CPacket* pDatePacket = NULL);
	int GetStartDate();
	//	data update�� ������ ����
	CPoint GetUpdateRange();
	//	�������� �޾ƾ� �� ��Ŷ���� �� ���ڵ��� ���̸� �����Ѵ�
	int GetPacketDataLength(int nIndex);
	//	Pakcet Name or Packet Field Name or Packet list Name ���� ���� �������� Packet�� ã�Ƽ� �����Ѵ�
	CPacket * GetBaseData(CString strFunction, const bool bCheckCurrentPacket = true);	
	bool GetPacketList(const CString& strPacketNames, const CString& strCompart, CList<CPacket*, CPacket*>& packetList) const;
	CTypedPtrList<CObList, CPacket*>& GetTRPacketList();
//	CTypedPtrList<CObList, CPacket*>& GetIndicatorPacketList();
	CString GetTRPacketNames(const CString& strCompart = ";") const;

	//sy 2004.07.26.
	void SetUseRealData(bool bUseRealData);
	//	data update�� �������� setting
	void SetHoldingPacketSize(const int nSize);
	void SetBasePacketName(const CString &szPacketName);
	//	data updateŸ�� setting  (NEW, INSERT, APPEND)
	void SetDataInsertType(CPacketDataInfo::DATAINSERTTYPE nDataInsertType);

	//	Server���� ������ data�� �޾Ƽ� �����Ѵ�. ���� ����Ÿ�� �ּҸ� �޾ƿ��� ����
	int SetBaseData(long pData, int nCount, int nType);	
	//	Server���� ������ data�� �޾Ƽ� �����Ѵ�. ���� ����Ÿ�� string���� �޾ƿ��� ����
	int SetBaseData(CString szData, int nCount, int nType);
/*
	CPacket* SetBaseData(const CString& szName, const CString& szType, const CString& szExpressionType, 
					     CList<double, double>& lsData, 
						 const int nStartPeriod, CPacketDataInfo::DATAINSERTTYPE nInsertType, 
						 CPoint ptUpdateRange);

	CPacket* SetBaseData(const CString& szName,
						 const CString& szType,
						 const CString& szExpressionType,
						 CList<CString, CString>& lsData, 
						 const int nStartPeriod,
						 CPacketDataInfo::DATAINSERTTYPE nInsertType, 
						 CPoint ptUpdateRange);
*/

	//	�����ؾ� �� �⺻ ����Ÿ Packet�׸��� �޾Ƽ� Packet List�� �����Ѵ�
//	void SetOption(CString strPacketOption);

	bool RemoveFictionData();
	bool ClearIndicatorData(const CString& strName);
	//  Packet List���� ������ �ִ� ��� Packet�� data list�� clear�Ѵ�
	//  bClear�� false�� ��ǥ�����͸� ����
	//  true�� �⺻�������ϰ� ��ǥ������ ��� ����
	int ClearDataAll(const bool bClearAllData = true);
	//  Packet Name�� �ִ� Packet�� data list�� clear�Ѵ�
	int ClearData(const CString& strPacketNames, const CString& strCompartName = ',', const bool bClearIncludingTR = true);
	bool ClearAllIndicatorData(const bool bOnlyData);
	bool DeleteOrClearPacket(const CString& strPacketName, const bool bIsDelete = true, const bool bIsOnlyNoTRData = true);
	bool DeleteOrClearPacket(CPacket* pPacket, const bool bIsDelete = true, const bool bIsOnlyNoTRData = true);

	//sy 2003.3.19.
	void SetItemData(const CString& strCode, const CString& strName);
	void GetItemData(CString& strCode, CString& strName) const;
	CString GetItemCode() const;
	CString GetItemName() const;

	//sy 2004.07.16.
	int GetRemovePacketCountInRealData() const;

	//���� packet�� �屸��.
	CPacketType::COMMODITY_BOND_NAME GetCommodityBondNameInBase() const;

	//�����޽��� (�������� �޴� �׸����� �� �׸񺰷� ���� �� �ִ�)
	bool SetHelpMessage(const CString& strHelpMessage, const bool bIsAllRemove);
	bool SetHelpMessageRealDataRate(const CString& strHelpMessage, const bool bIsRemove);

private:
	CPacketListImplementation* m_pPacketListImple;

// (2004.09.14, ��¿�) ���� Message Item�� ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	BOOL						GetHelpMessageItem( const char *p_szMsgKey, CString &p_strMsgValue);
	// (2005/9/29 - Seung-Won, Bae) Get Help Message List
	const CMapStringToString *	GetHelpMessageMap( void);

// (2006/12/20 - Seung-Won, Bae) Support Binary TR Data Receiving
public:
	BOOL	SetBinTrHelpMsg( const char *p_szHelpMessage);
	BOOL	SetBinTrDoubleData( const char *p_szPacketName, long p_nCount, double FAR* p_pdData);
	BOOL	SetBinTrStringData( const char *p_szPacketName, long p_nCount, const char **p_pszData);
	BOOL	SetBinTrEnd( void); 

	// �ڽ��� RQ : �������� - ojtaso (20070117)
	void	SetRQ(LPCTSTR lpszRQ);
	LPCTSTR GetRQ();

	// ���簡 ����� - ojtaso (20080602)
	void	OffsetPacketData(double dData, CString strPacketName = _T(""));

// (2008/2/4 - Seung-Won, Bae) for Other Meta of MultiLanguage
public:
	CString		GetPacketOption( void);

// (2009/5/21 - Seung-Won, Bae) Shift the data of packet to future.
public:
	BOOL	ShiftPacket( LPCTSTR p_szPacketNames, long p_szShiftCount);


// 20080925 JS.Kim	���� �ٸ� �ð����� Scale ó��
public:
	CString		GetBaseTimeName();
	void		SetBaseTimeName(CString strBaseTimeName);
	int			GetBaseTimeDifference();
	void		SetBaseTimeDifference(int nBaseTimeDiff);
	int			GetCalcDataTimeDifference();
	void		SetBaseTime(CString strBaseTimeName, int nBaseTimeDiff);

	int			GetOrgBaseTimeDifference();
	void		SetOrgBaseTime();

	__int64		GetCurrDateTime() const;
	bool		GetMarketTime(int& nMarketStartTime, int& nMarketEndTime) const;

	void		CalcPacketTimeDiff(int nBaseTimeDiff);

//	CPacket*	GetDatePacket();

	// 20081024 JS.Kim	������� ocx �� AddIn ���� �ֱ����� ������� �ð��� �Ǻ��ð����� ȯ���Ѵ�.
	bool		CalcNMTimeToRealTime(CPacket *pPacket, const double& dTime, double& dRealTime, double& dCloseData);
	// 20081029 JS.Kim	������ ó�� ������ ���Ѵ�.
	CPacketType::MARKETDAYTYPE GetMarketDayType();

// (2009/10/19 - Seung-Won, Bae) Real Data Processing Interface.
public:
	bool	SetRealData( const char *p_szPacketName, const double dData, int &p_nRemovePacketDataCount, const int nOption = 0);
	bool	GetRealData_CloseTime( const __int64 &p_nClock, __int64 &p_nRealDataTime);
	void	PushNMTRState( const __int64 &p_nRealDataTime);
	void	PopNMTRState( void);
			// (2009/11/12 - Seung-Won, Bae) Get NMTR Info
	BOOL	GetLastClockAndRealTime( __int64 &p_nLastClockTime, __int64 &p_nLastRealTime);

// (2009/10/21 - Seung-Won, Bae) Comment. �ǽð� Data�� Packet Type�� �����Ѵ�.
public:
	CString		GetRealTimeType( void);						//�ǽð�����Ÿ ��Ŷ�� ���� ������ ����

// (2009/11/1 - Seung-Won, Bae) for Normalized Candle data.
public:
	double		GetOrgDataBeforeRate( const char *p_szPacketName, const double p_dRatedData);

// (2009/11/3 - Seung-Won, Bae) Make Market Date to Real Date of WorldOn Real Data.
public:
	void		GetRealData_RealDateTime( double &p_dData);

// (2009/12/2 - Seung-Won, Bae) Notify to other modules.
public:
	void		ChangeChartMode( int p_nChartMode);
};

#endif // !defined(AFX_PACKETLIST_H__67D8B2C1_2E6F_11D5_81A5_0050FC28B229__INCLUDED_)
