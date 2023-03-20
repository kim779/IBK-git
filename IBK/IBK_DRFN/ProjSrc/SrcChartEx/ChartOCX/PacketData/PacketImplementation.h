// PacketImplementation.h: interface for the CPacketImplementation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETIMPLEMENTATION_H__E24ABB33_A837_49DE_AA47_48DF59595FFE__INCLUDED_)
#define AFX_PACKETIMPLEMENTATION_H__E24ABB33_A837_49DE_AA47_48DF59595FFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxtempl.h>
#include "BlockBaseData.h"
#include "PacketBaseData.h"		// CItemData
#include "PacketBase.h"

class CPacket;
class CPacketList;
class CPacketImplementation  
{
public:	
	CPacketImplementation(CPacket* pPacket,
			CPacketList * pParentList,
			CString strName, 
			CString strFieldName, 
			CString strType,
			CString szExpressionType,
			int nDataLength, 
			bool bUseReal,
			int nStartPeriod = 0,
			CString strRealFieldName = "");
	virtual ~CPacketImplementation();

	//////////Data Min/Max ���� �޼ҵ�////////////////////
	bool RestoreMax();
	bool RestoreMin();
	bool RestoreMinMax();

	bool SetMax(const double nMax);
	bool SetMin(const double nMin);
	bool SetMinMax(const double nMin, const double nMax);
	int SetMinMax(CPacketDataInfo::DATAINSERTTYPE nInsertType = CPacketDataInfo::NEW, CPoint ptDataRange = CPoint(0, 0));
	// (2009/1/17 - Seung-Won, Bae) Add without-Zero option.
	int GetMinMax(int nStart, int nEnd, double& nMin, double& nMax, BOOL p_bWithoutZero = FALSE);
//	double GetMin() const;
//	double GetMax() const;
	///////////////////////////////////////////////////////

	
	void SetBaseData(const CString& strType, const CString& strExpressionType, const int nStartPeriod, const bool bIsAutoMinMax = true);
	void SetYesOrNoBaseData(const bool bBaseData);
	void SetAcceptFromServer(const bool bAcceptFromServer);

	bool IsTRData() const;
	bool IsAcceptFromServer() const;
	bool IsZeroBasePacket() const;
	bool IsChangeRealData() const;
	//	Packet�� ���� �� data�� ����
	int GetLength();
	//	data�� ���� return
	int GetCount() const;
	int GetDataCount();
	//  ��ü ����Ÿ�� ���� ����� ����
	int GetMaxCount();
	//	CPacket�� FieldName�� ��´� (�� �����ν� ������ �̸�)
	CString GetFieldName();
	//	CPacket�� Name�� ��´� (������ �̸� or ����ڰ� ������ �̸�)
	CString GetName();
	//nami 9.30
	CPacketList* GetPacketList();
	CPacket* GetDatePacket();
	CPacketType::COMMODITY_BOND_NAME GetCommodityBondName() const;
	void SetCommodityBondName(const CPacketType::COMMODITY_BOND_NAME eCommodityBondName);
	// fail : -1  success : index of data
	// ����Ʈ �����ð� ���� - ojtaso (20080811)
	int GetIndexFromData(double nData, CString strType = _T(""));
	// return : true -> ���� ���� ���
	//			false -> ������ ����ų� ���� ���� ���� ���
	bool GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData) const;
	bool GetData(const int nIndex, double& dData) const;
	// fail : ERRORVALUE   success : data of index
	double GetData(int nIndex);
	CString GetStrData(int nIndex);
	// ���ſ� �̷� ������ �ð������� return�Ѵ�
	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰� <<
	//int GetDataPastandFuture(int nIndex);
	double GetDataPastandFuture(int nIndex);
	bool GetDataPastandFuture(const int nIndex, double& dData);
	int GetTimeInterval();
	int GetDateUnitType();
	CString GetRealKeyName();
	CString GetRealFieldName();
	bool GetUseReal();
	bool IsUseStandardValue( const int p_nType) const;		// 0:Base,	1:Upper Base,	2:Lower Base
	double GetStandardValue( const int p_nType);			// 0:Base,	1:Upper Base,	2:Lower Base
	int GetStartPeriod();
	void SetRealDataGap( const double &p_dDataGap);
	bool GetRealDataGap( double& dDataGap) const;
	//nami 9.29
	CString GetType(const bool bIsFullType = true);
	double GetTypeFormatDouble(const bool bIsFullType = true);
	const CList<double, double>* GetnumericDataList() const;
	CList<double, double>* GetnumericDataList();
	CList<CString, CString>* GetstrDataList();
	

	//	CPacket�� �ǽð� �޴� Packet�� ��� �ǽð� data Field Name�� �����Ѵ�
	void SetRealFieldName(CString strRealFieldName, CString strRealKeyName);
	void SetBoundLimit(double nUpperBound, double nLowerBound);
	void SetStandardValue( const char *p_szValue);
	void SetStartPeriod(int nStartPeriod);
	void SetName(CString strName);
	void SetType(const CString& strType);
	void SetType(const CString& strFullType, const CString& strExpressionType);
	void SetLength(int nLength);

	//	CPacket���� data�� �־��� data�� �ٲ۴� list�� data���� ��� copy�ȴ�
	void ChangeData(CList<double, double>& lsnewData);
	void ChangeData(CList<CString, CString>& lsnewData);

	//	String ���·� ���� data�� data list�� insert (head�� �ش�)
	void InsertData(CString strData);	
	//	�ּҷ� ���� data��  �о double������ data list�� insert (head�� �ش�)
	void InsertData(long pData, int nDataLength);
	//	�ּҷ� ���� data��  �о double������ data list�� append (tail�� �ش�)
	void AppendData(long pData, int nDataLength);
	//	String ���·� ���� data�� double������ �ٲپ data list�� append (tail�� �ش�)
	void AppendData(const CString& strData);
	void AppendData(double nData, int nIndex = -1);
	//	List ���·� ���� data�� data list�� insert (head�� �ش�)
	void AppendData(CList<double, double>& lsnewData);	
	void AppendData(CList<CString, CString>& lsnewData);

	void UpdateData(const double nData);
	void UpdateData(const double nData, const int nIndex, const bool bIsCurrentData = true);
	void UpdateData(const CString& strData);
	void UpdateData(const CString& strData, const int nIndex, const bool bIsCurrentData = true);
//	void UpdateData(CList<double, double>& lsnewData, CPoint ptUpdateRange);
	void UpdateData(CList<CString, CString>& lsnewData, CPoint ptUpdateRange);


	bool RemoveData(int nStart, int nEnd);
	int	 RemoveBaseData(const double nCount);
	int RemoveFictionData(const double dReallastDate, const bool bDate = false);
	// CPacket���� data�� clear��
	int ClearData(const bool bClearIncludingTR = false);
	void Initialize(const bool bClearIncludingTR);

public:
	//sy 2005.07.11. -> �ӽ� ���� ������ �ִ� list
	void AddTailTempList(const double& dData);
	void RemoveAllTempList();

	bool SetTempListData(const int nIndex, const double& dData, const bool bIsAutoTail = true);
	bool GetTempListData(const int nIndex, double& dData) const;
	bool GetHeadTempListData(double& dData) const;
	bool GetTailTempListData(double& dData) const;
	CList<double, double>* GetTempList() const;

private:
	CPacket* m_pPacket;

	//////////Data Min/Max ����//////////
	double m_dMaxBound;
	double m_dMinBound;	
	bool m_bUseBound;
	//////////////////////////////////////

	bool m_bBaseData;
	bool m_bAcceptFromServer; //sy 2005.11.08.
	int m_nStartPeriod;
	int m_nLength;

	// (2007/1/14 - Seung-Won, Bae) Support Upper and Lower Base Data.
	CItemData m_standardValue;
	CItemData m_upperBaseValue;
	CItemData m_lowerBaseValue;

	bool m_bUseReal;
	//�Ӽ����������� ������ Ÿ�� "*0.01"��.
	CString m_Type;
	//scale, title type
	//nami 8.29
	CString m_ExpressionType;
	//sy 2003.3.19. 
	// -> ����data�� �ǽð� ������ ���� ���� ������("�⺻�ŷ���", "�����ŷ���")
	CItemData m_realDataGap;
	//sy 2005.08.05. �屸��(�ֽ�/����/����/�ɼ�)
	CPacketType::COMMODITY_BOND_NAME m_eCommodityBondName;

	CString m_FieldName;
	CString m_Name;
	CString m_RealFieldName;
	CString m_RealKeyName;
	CList <double, double> m_lnData;
	CList <CString, CString> m_lstrData;
	CPacketList * m_pParentList;

	//sy 2005.07.11. -> �ӽ� ���� ������ �ִ� list
	CList<double, double> m_dTempDataList;

	// 20080908_JS.Kim	8����/32���� ǥ�� ����
	int		m_nBase;
	int		m_nDecLen;		// ä�ǰ���ǥ�ÿ��� ' ���� ������ ����
	int		m_nDecLen2;		// ä�ǰ���ǥ�ÿ��� . ���� ������ ����

private:
	// data�� Minimun & Maximum ���� ���ؼ� ������
	// SetMinMax���� datatiype�� add�� insert�� ��쿡 ȣ��ȴ�
//	int ResetMinMax(int nStart, int nEnd, double& nMi, double& nMa);
//	void ResetMinMax(const double &dValue);

	int GetIndexOfFuture(const double dData);
	int GetIndexOfPast(const double dData);
	int GetIndexInCaseTypeIsnotBun(const double dData);
	int GetIndexInCaseTypeIsnotBun_CurData(const double dData);
	// ����Ʈ �����ð� ���� - ojtaso (20080811)
	int GetIndexInCaseTypeIsBun(double dData, CString strType = _T(""));
	// ����Ʈ �����ð� ���� - ojtaso (20080811)
	int GetIndexInCaseTypeIsBun_CurData(const double dData, CString strType = _T(""));
	void GetComparisionDatas(const double dData, int& HeadNum, int& tailNum, int& dataNum);

	bool GetCurOrPrevIndexFromData_Date(const double& dData, double& dCurOrPrevIndexData, int& nCurOrPrevIndex) const;

	// Find Index
	POSITION FindPosition(const int nIndex) const;
	POSITION FindStrPosition(const int nIndex) const;

	int RemoveDateData(const double dReallastDate);

	bool GetRealData_UpdateData(const double& dData, double& dRealData);
	double GetRealData_AppendData(const double& dData);

	bool IsDataInPacketType(const double& dData) const;
	bool IsDataInPacketType_Date(const double& dData) const;
	bool IsDataInPacketType_Time(const double& dData) const;
	int GetTypePartData(const double& dOrgData, const CString& strType) const;

// (2006/12/20 - Seung-Won, Bae) Support Binary TR Data Receiving
public:
	void	AppendTrData( const char *p_szData);
	void	AppendTrData( const double dData);
	void	InsertTrData( const char *p_szData);
	void	InsertTrData( const double dData);

// (2007/2/7 - Seung-Won, Bae) for Exception Value
protected:
	double	m_dExceptionValue;
public:
	void SetExceptionValue( const double &p_dExceptionValue);
	double GetExceptionValue( void) const	{	return m_dExceptionValue;	}

// (2007/2/22 - Seung-Won, Bae) for Continue Code
protected:
	double m_dContinueCode;
public:
	void SetContinueCode( const double &p_dContinueCode);
	double GetContinueCode( void) const	{	return m_dContinueCode;	}

// (2007/2/21 - Seung-Won, Bae) Support Not-Auto Scale Type.
protected:
	BOOL	m_bAutoScaleType;
	CString	m_strScaleType;
public:
	void	SetAutoScaleType( BOOL p_bAutoScaleType)	{	m_bAutoScaleType = p_bAutoScaleType;	}
	BOOL	IsAutoScaleType( void)						{	return m_bAutoScaleType;				}
	void	SetScaleType( const CString &p_strScaleType)	{	m_strScaleType = p_strScaleType;	}
	CString	GetScaleType( void)								{	return m_strScaleType;				}
	double	GetScaleTypeFormatDouble( void);

	// ���簡 ����� - ojtaso (20080602)
	void	OffsetPacketData(double dData);

	// 20080908 ������	8����/32���� ǥ�� ����
	void	SetBaseNumber(int nBase)	{	m_nBase = nBase;	}
	int		GetBaseNumber()				{	return m_nBase;		}
	void	SetDecLen(int nDecLen, int nDecLen2)	{	m_nDecLen = nDecLen;	m_nDecLen2 = nDecLen2;	}
	int		GetDecLen(int &nDecLen, int &nDecLen2)	{	nDecLen = m_nDecLen;	nDecLen2 = m_nDecLen2;	return m_nDecLen;	}

// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
//		Do not support Full Range Min/Max for collision with Pre-Min/Max.
protected:
	BOOL	m_bWithoutZero_Pre;
	double	m_dContinueCode_Pre;
	double	m_dExceptionValue_Pre;
	int		m_nStartPeriod_Pre;
	int		m_nStartIndex_Pre;
	int		m_nEndIndex_Pre;
	double	m_dPreMaxValue;
	double	m_dPreMinValue;
	double	m_dPreMaxValueWithoutEnd;
	double	m_dPreMinValueWithoutEnd;
public:
	void	ClearMinMax( void);
	void	UpdateMinMax( double p_dData, int p_nIndex);

// (2009/5/21 - Seung-Won, Bae) Shift the data of packet to future.
public:
	BOOL	ShiftData( long p_szShiftCount);

// 20081007_JS.Kim	����ڽð��� ����
public:
	void	CalcPacketTimeDiff(int nTimeDiff);
	double	CalcTimeDiff(double dData, int nTimeDiff);
	double	CalcTimeDiff(double dData);
	double	CalcBackTimeDiff(double dData);		// ���� �ð���� �ǵ����� �Լ�

// 20100324_JS.Kim �ַθ�.FX	�ð����� ���� ��Ŷ�� �ð�����ó�� ����ϱ� ���� ó��
public:
	CString	m_strSubName;

	CString GetSubName()						{	return m_strSubName;				};
	void	SetSubName(CString strSubName)		{	m_strSubName = strSubName;	};
};

#endif // !defined(AFX_PACKETIMPLEMENTATION_H__E24ABB33_A837_49DE_AA47_48DF59595FFE__INCLUDED_)
