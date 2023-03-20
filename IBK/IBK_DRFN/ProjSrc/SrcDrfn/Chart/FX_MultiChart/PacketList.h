// PacketList.h: interface for the CPacketList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETLIST_H__67D8B2C1_2E6F_11D5_81A5_0050FC28B229__INCLUDED_)
#define AFX_PACKETLIST_H__67D8B2C1_2E6F_11D5_81A5_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "BlockBaseData.h"
#include "PacketBase.h"

class CPacket;
class CPacketListImplementation;
class AFX_EXT_CLASS CPacketList  
{
public:	
	CPacketList(const CPacketType::REALTYPE eRealType);
	CPacketList(CString strPacketOption, const CPacketType::REALTYPE eRealType);
	virtual ~CPacketList();

	void SetVersion(double dVersion);
	void SetRealType(const CPacketType::REALTYPE eRealType);
	void SetPacketLengthCount(int nLength);
	bool SetData(const CString szPacketName, int nIndex, double dValue, bool IsByReal);

	// (2004.06.18, ��¿�) Data Byte���� Long���� �����Ѵ�.
	void SetPacketData(const CString szPacketName, long pData, long lCount);

	bool SetPacketData(const CString& strPacketName, const CString& strPacketType, 
		const char* pPacketData, const int nCount);
	void AddPacket(const CString szPacketOption);
	bool RemovePacket(const CString szPacketName);
	int GetLastestRealIndex();

	//	�ð� ���� ����
	int GetTimeInterval();
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
	CRealUpdateMode::MODE GetRealUpdateMode();

	bool UseRealData();
	bool GetUseReal();
	// 04.17.2002  -> ������� ��������� üũ
	bool FindKey(CString strKey);
	bool IsFullMinMax();

	int GetAvailablePacketCount();
	int GetFundTypeStartandEndPoint(CPoint& ptStart, CPoint& ptEnd);
	int GetEndDate(CPacket* pDatePacket = NULL);
	int GetStartDate();
	//	data update�� ������ ����
	CPoint GetUpdateRange();
	//	�������� �޾ƾ� �� ��Ŷ���� �� ���ڵ��� ���̸� �����Ѵ�
	int GetPacketDataLength(int nIndex);
	//	Pakcet Name or Packet Field Name or Packet list Name ���� ���� �������� Packet�� ã�Ƽ� �����Ѵ�
	CPacket * GetBaseData(CString strFunction, const bool bCheckCurrentPacket = true);	
	CTypedPtrList<CObList, CPacket*>& GetTRPacketList();
	CTypedPtrList<CObList, CPacket*>& GetIndicatorPacketList();

	//sy 2004.07.26.
	bool SetRealOrgData(UINT wParam, LPARAM lParam, bool& bRemovePacketData);
	//sy 2004.07.26.
	bool SetRealData(const CString &strPacketName, const double nData, 
		const int nOption, const bool bCheckDataLength, bool& bRemovePacketData);
	void SetFullMinMax(bool bFullMinMax);
	void SetUseRealData(bool bUseRealData);
	//	data update�� �������� setting
	void SetUpdateTerminate();
	void SetHoldingPacketSize(const int nSize);
	void SetBasePacketName(const CString &szPacketName);
	//	data update�� ������ ����
	void SetUpdateRange(CPoint ptRange);
	bool SetRealDataFieldNames(const CString& strFieldNames);
	//	data updateŸ�� setting  (NEW, INSERT, APPEND)
	void SetDataInsertType(CPacketDataInfo::DATAINSERTTYPE nDataInsertType);

	//	Server���� ������ data�� �޾Ƽ� �����Ѵ�. ���� ����Ÿ�� �ּҸ� �޾ƿ��� ����
	int SetBaseData(long pData, int nCount, int nType);	
	//	Server���� ������ data�� �޾Ƽ� �����Ѵ�. ���� ����Ÿ�� string���� �޾ƿ��� ����
	int SetBaseData(CString szData, int nCount, int nType);
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

	//	�����ؾ� �� �⺻ ����Ÿ Packet�׸��� �޾Ƽ� Packet List�� �����Ѵ�
	void SetOption(CString strPacketOption);

	bool RemoveFictionData();
	bool ClearIndicatorData(const CString& strName);
	//  Packet List���� ������ �ִ� ��� Packet�� data list�� clear�Ѵ�
	//  bClear�� false�� ��ǥ�����͸� ����
	//  true�� �⺻�������ϰ� ��ǥ������ ��� ����
	int ClearDataAll(const bool bClearAllData = true);
	//  Packet Name�� �ִ� Packet�� data list�� clear�Ѵ�
	int ClearData(const CString& strPacketNames, const CString& strCompartName = ',');
	bool ClearAllIndicatorData(const bool bOnlyData);
	bool DeleteOrClearPacket(CPacket* pPacket, const bool bIsDelete = true, const bool bIsOnlyNoTRData = true);

	//sy 2003.3.19.
	void SetItemData(const CString& strCode, const CString& strName);
	void GetItemData(CString& strCode, CString& strName) const;
	CString GetItemCode() const;
	CString GetItemName() const;

	//sy 2004.07.16.
	int GetRemovePacketCountInRealData() const;

	CPacketType::COMMODITY_BOND_NAME GetCommodityBondName() const;

private:
	CPacketListImplementation* m_pPacketListImple;

// (2004.09.14, ��¿�) ���� Message Item�� ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	BOOL GetHelpMessageItem( const char *p_szMsgKey, CString &p_strMsgValue);
};

#endif // !defined(AFX_PACKETLIST_H__67D8B2C1_2E6F_11D5_81A5_0050FC28B229__INCLUDED_)
