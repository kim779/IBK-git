// PacketListImplementation.h: interface for the CPacketListImplementation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETLISTIMPLEMENTATION_H__356283A1_90B9_409D_992E_3A81BB4F1E5C__INCLUDED_)
#define AFX_PACKETLISTIMPLEMENTATION_H__356283A1_90B9_409D_992E_3A81BB4F1E5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"
#include "PacketBase.h"
#include "RealModuleCenter.h"
#include "TimeRealDataModule.h"
#include "PacketListModule.h"
#include "PacketBaseData.h"

interface IChartCtrl;
class CPacket;
class CPacketList;
class CPacketListImplementation  
{
public:	
	CPacketListImplementation(CPacketList* pPacketList, const CPacketType::REALTYPE eRealType, IChartCtrl *p_pIChartCtrl);
	CPacketListImplementation(CPacketList* pPacketList, CString strPacketOption, const CPacketType::REALTYPE eRealType, IChartCtrl *p_pIChartCtrl);
	virtual ~CPacketListImplementation();

	void SetVersion(double dVersion);
	void SetPacketLengthCount(int nLength);
	bool SetData(const CString szPacketName, int nIndex, double dValue, bool IsByReal);

	CPacketType::REALTYPE GetRealType() const;

	// (2004.06.18, ��¿�) Data Byte���� Long���� �����Ѵ�.
	void SetPacketData(const CString& szPacketNames, long pData, long lCount);

	bool SetPacketData(const CString& strPacketName, const CString& strPacketType, 
		const char* pPacketData, const int nCount);
	CString AddPacket(const CString szPacketOption);
	bool RemovePacket(const CString szPacketName);
	int GetLastestRealIndex();

	//	�ð� ���� ����
	int GetTimeInterval();
	__int64 GetTimeIntervalWithSecond( void);
	// (2006/7/11 - Seung-Won, Bae) Retrived the Next First Time of Data.
	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰� <<
	//int GetStartTimeInTimeRangeOfLastData( void);
	__int64	GetStartTimeInTimeRangeOfLastData( void);
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
	int GetMaxCount(CString strPacketNameList, const CString& strCompartName);
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
	CList<CString, CString>* GetRealKeyName()	{return &m_lstRealKeyName;}
	CPacketType::PRICETYPE GetPriceType()	{ return m_nPriceType;}

	bool UseRealData()	{return m_bUseRealData;}
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
	CPacket * GetBaseData(const CString& strPacketName, const bool bCheckCurrentPacket = true);
	bool GetPacketList(const CString& strPacketNames, const CString& strCompart, CList<CPacket*, CPacket*>& packetList) const;
	bool GetPacketList(const CString& strPacketNames, const CString& strCompart, const bool bIsOnlyBaseTRData, CTypedPtrList<CObList, CPacket*>& packetList) const;
	CTypedPtrList<CObList, CPacket*>& GetTRPacketList();
//	CTypedPtrList<CObList, CPacket*>& GetIndicatorPacketList();
	CString GetTRPacketNames(const CString& strCompart) const;

	//sy 2004.07.26.
	void SetUseRealData(bool bUseRealData)	{m_bUseRealData = bUseRealData;}
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
	//	���ο� data list�� �� Packet�� �����. Packet�� �̸��� data list�� �Բ� �־����� ���
	CPacket * SetBaseData(const CString& szName,
						  const CString& szType,
						  const CString& szExpressionType,
						  CList<double, double>& lsData,
						  const int nStartPeriod = 0, 
						  CPacketDataInfo::DATAINSERTTYPE nInsertType = CPacketDataInfo::NEW,
						  CPoint ptUpdateRange = CPoint(-1,-1));
	CPacket * SetBaseData(const CString& szName,
					      const CString& szType,
						  const CString& szExpressionType,
						  CList<CString, CString>& lsData,
						  const int nStartPeriod = 0,
						  CPacketDataInfo::DATAINSERTTYPE nInsertType = CPacketDataInfo::NEW,
						  CPoint ptUpdateRange = CPoint(-1,-1));
*/

	//	�����ؾ� �� �⺻ ����Ÿ Packet�׸��� �޾Ƽ� Packet List�� �����Ѵ�
//	void SetOption(CString strPacketOption);

	bool RemoveFictionData();
	bool ClearIndicatorData(const CString& strName);
	//  Packet List���� ������ �ִ� ��� Packet�� data list�� clear�Ѵ�
	void Initialize_Packet(const bool bClearIncludingTR = true);
	int ClearDataAll(const bool bClearIncludingTR = true);
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
	int SetHelpMessage( const char *p_szHelpMessage, const bool bIsAllRemove);
	bool SetHelpMessageRealDataRate(const CString& strHelpMessage, const bool bIsRemove);

 private:
	void DeleteData(CTypedPtrList<CObList, CPacket*>* lstPacket);

	bool GetPacketTypeInInfo(const CString& strInfo, CString& strName, CString& strType, int& nLength) const;
	int GetPacketDataLength(int nIndex, CTypedPtrList<CObList, CPacket*>* lstPacket);
	int GetPacketDataLength(const CString &szPacketName);
	CPacket* GetRealPacket(const CString& strRealFieldName, const CString& strRealKeyName);
	CPacket* GetBaseDataInCurrentInCompart(const CString& strPacketName, const CString& strCompart) const;
	CPacket* GetBaseDataInCurrent(const CString& strPacketName, const bool bIsOnlyTRData = false) const;
	CPacket* GetPacketInTRData(const CString& strPacketName, const bool bIsOnlyBaseTRData) const;
	CPacket* GetPacket(const CString& strPacketName, const CTypedPtrList<CObList, CPacket*>& packetList) const;
	CPacket* GetBaseDataInNew(const CString& strPacketName);

	//code�� �屸��.
	CPacketType::COMMODITY_BOND_NAME GetCommodityBondNameInCode() const;

	int SetBaseDataNewPacket(long pData, int nCount);
	int SetBaseData(long pData, int nCount, CTypedPtrList<CObList, CPacket*>* lstPacket);
	void SetPacketData(long pData, long lCount, const int nDataCount, const bool bIsBasePacket, CTypedPtrList<CObList, CPacket*>& packetList);
	int SetBaseData(long pData, const int nbozolen, const int nRowCount, const bool bIsBasePacket, CTypedPtrList<CObList, CPacket*>& packetList);
	bool SetErrorMessage(const CString& strMessage);
	void SetBoundLimit(const CString& strData);
	void SetBoundLimit(double nUpperBound, double nLowerBound, const CString &strPacketName);

	void SetDrawStartIndex(const CString& strInfo);
	void SetItemData(const CString &strData);
	void SetCurrentDate();
	void SetLength(CPacket * pPacket);
	void SetPrevDatas(const CString& strData);
	void SetPrevData(CString& strData);

	// (2007/2/7 - Seung-Won, Bae) for Exception Value
	void	SetExceptionValues( const CString &strData);
	// (2007/2/22 - Seung-Won, Bae) for Continue Code
	void SetContinueCodes( const CString &strData);

	void SetCommodityBondName(const CString& strData);

	int ResetTimeType(CString strTimeType);
	//	�����޼��� ���� setting
	void SetHelpMessageInitialize( const char *p_szHelpMessage);
	int SetHelpMessage(const CString& strInfo);
	int SetDataInsertTypeInfo(const CString& strData/*, const bool bIsBasePacket*/);
	void Initialize_PacketData();
	void Initialize_DataInsertType();
	void Initialize_HelpMessage();
	void SetDateUnitType(const CString& strData);
	// 02.19.2002  ��Ŷ ���̸� �� �����Ѵ�
	bool ResetPacketInfo(const CString& strPacketInfo);
	bool SetPacketInfo(const CString& strPacketInfo);
	// 03.11.2002  �� ��Ŷ�� �����Ѵ�(�����޼����� ���ؼ�)
	bool SetNewPacket(const CString& strPacketInfo);

//	CPacket * AddIndicatorData(CString strFuntion, CString strCondition);
	
	// (2006/6/5 - Seung-Won, Bae) return Deleted Data Count
	int	RemovePacketList(int nStart, int nEnd, int nType);

	bool DeletePacketList(CTypedPtrList<CObList, CPacket*>* lstPacket);
	// 02.19.2002  �Ϻ� ��Ŷ�� clear�Ѵ� (����Ÿ ���̰� 0�� �ƴ� ��Ŷ)
	bool PacketClearPart();

	bool GetAllPacketsMinMax(CTypedPtrList<CObList, CPacket*>& packetList, const int nStart, const int nEnd, double& dMin, double& dMax);
	bool FindPosition(CPacket* pPacket, CTypedPtrList<CObList, CPacket*>*& pPacketList, POSITION& pos);

	bool SetPacketData(CPacket* pPacket, const char* pPacketData, const int nCount);

	void AppendAllDataInPacket(CPacket* pPacket, char* szData, const bool bClearData, 
			const int nAllPacketLength, const int nRowCount, int& nDataTotalIndex);
	void InsertAllDataInPacket(CPacket* pPacket, char* szData, const int nAllPacketLength, 
			const int nRowCount, int& nDataTotalIndex);

	void Initialize_BaseDataBefore(const bool bIsBasePacket);
	void Initialize_BaseDataAfter(const bool bIsBasePacket, const int nRowCount, CTypedPtrList<CObList, CPacket*>* pPacketList);
	void SetPecketBaseData(CTypedPtrList<CObList, CPacket*>* pPacketList);

private:
	CPacketList* m_pPacketList;
	double m_dVersion;
	int m_nHoldingPacketSize;
	int	m_nPacketLengthCount;
	int m_nMaxDataCount;
	double m_dCurrentDate;

	//sy 2005.11.14. -> TR ��ȸ�� data ����
	CTRDataCountList m_trDataCountList;
	

	// 04.17.2002  -> ����� �����(���ɿ� ����� ���� �����)
	CList<CString, CString> m_lstRealKeyName;
	CTypedPtrList<CObList, CPacket*> m_PacketList;
	CTypedPtrList<CObList, CPacket*> m_IndicatorDataList;
	CTypedPtrList<CObList, CPacket*> m_AdditionList;

	//sy 2005.09.30.
	CRealModuleCenter m_realCenter;

private:
	// �����޽����� �޼ҵ�� ������ data
	CPacketDataInfo::DATAINSERTTYPE m_nDataInsertType;
	CPacketType::PRICETYPE m_nPriceType;
	bool m_bUseRealData;
	//sy 2003.3.19. -> �����ڵ�� �����
	CItem m_itemData;

	// �ڽ��� RQ : �������� - ojtaso (20070117)
	LPCTSTR	m_lpszRQ;
	
private:
	bool SetPacketData_HelpMsg(const CString& szPacketName, long pData, long lCount, 
			int& nDataCount, bool& bIsBasePacket, CTypedPtrList<CObList, CPacket*>& packetList);
	bool SetPacketData(CTypedPtrList<CObList, CPacket*>& packetList, char* szData, const int nRowCount);
	int GetRowCount(long pData) const;
	void GetHelpMessageData(long pData, int& nDataCount, CString& strHelpMsgData) const;

	void SetMaxDataCount();
	void AddTRDataCount(const int nCount);


// (2004.09.14, ��¿�) ���� Message Item�� ���������� �����Ͽ� ��ȸ �̿��� �� �ֵ����Ѵ�.
protected:
	CMapStringToString 		m_mapHelpMessage;
public:
	BOOL GetHelpMessageItem( const char *p_szMsgKey, CString &p_strMsgValue);
	// (2005/9/29 - Seung-Won, Bae) Get Help Message List
	const CMapStringToString *	GetHelpMessageMap( void)	{	return &m_mapHelpMessage;	}

// (2006/12/20 - Seung-Won, Bae) Support Binary TR Data Receiving
protected:
	long									m_lBinTrRowCount;
	BOOL									m_bBinTrIsBasePacket;
	CTypedPtrList< CObList, CPacket *> *	m_pBinTrPacketList;
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

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;

// (2008/2/4 - Seung-Won, Bae) for Other Meta of MultiLanguage
protected:
	CString		m_strPacketOption;
public:
	CString		GetPacketOption( void)		{	return m_strPacketOption;	}

// (2009/1/29 - Seung-Won, Bae) for 92000 TEST
//protected:
//	int			m_nI;
//	long		m_lTime;

// (2009/5/21 - Seung-Won, Bae) Shift the data of packet to future.
public:
	BOOL	ShiftPacket( LPCTSTR p_szPacketNames, long p_szShiftCount);

// 20080925 JS.Kim	���� �ٸ� �ð����� Scale ó��
protected:
	int			m_nOrgBaseTimeDiff;

	CString		m_strBaseTimeName;
	int			m_nBaseTimeDiff;
public:
	void		SetOrgBaseTimeDifference(const CString& strData);
	int			GetOrgBaseTimeDifference();

	void		SetBaseTimeName(CString strBaseTimeName);
	CString		GetBaseTimeName();

	void		SetBaseTimeDifference(const int nTimeDiff);
	void		SetBaseTimeDifference(const CString& strData);
	int			GetBaseTimeDifference();			// ����ڰ� ������ ���� �ð� ���ϱ�
	int			GetCalcDataTimeDifference();		// Packet�� Data ���� �� ����� ����ڽð�-�����ð� ����� �� ���ϱ�

	__int64		GetCurrDateTime() const;
	bool		GetMarketTime(int& nMarketStartTime, int& nMarketEndTime) const;
	
	void		CalcPacketTimeDiff(int nBaseTimeDiff);

	CPacket*	GetDatePacket();
	void		CalcMarketTime();

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

// 20100324_JS.Kim �ַθ�.FX	�ð����� ���� ��Ŷ�� �ð�����ó�� ����ϱ� ���� ó��
public:
	void		SetRedefinePacketName(const CString& strData);
	CPacket*	GetRedefinedPacket(const CString& strPacketName, const CTypedPtrList<CObList, CPacket*>& packetList) const;

	// (2011/12/09 - Junok, Lee) add function to get default chart bozo message value.
	BOOL GetDefaultBozoValue(int p_nChartMode, LPCSTR p_szKey, CString &p_rValue);

	CDRDebugerLoader *m_pDRDebuger;
	BOOL _GetDRDebuger();	
};

#endif // !defined(AFX_PACKETLISTIMPLEMENTATION_H__356283A1_90B9_409D_992E_3A81BB4F1E5C__INCLUDED_)
