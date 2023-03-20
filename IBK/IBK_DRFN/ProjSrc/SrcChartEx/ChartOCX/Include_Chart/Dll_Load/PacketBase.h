// Packetbase.h: interface for the CPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETBASE_H__DDFE472D_2D8F_11D5_81A5_0050FC28B229__INCLUDED_)
#define AFX_PACKETBASE_H__DDFE472D_2D8F_11D5_81A5_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "../BlockBaseData.h"

// (2006/9/5 - Seung-Won, Bae) for Import in Extension DLL
#ifndef AFX_EXT_CLASS_PACKETDATA
	#ifdef _PACKETDATA_EXPORT								// in PacketList.dll
		#define AFX_EXT_CLASS_PACKETDATA	AFX_CLASS_EXPORT
//	#elif defined _DD_NOT_IMPORT							// in Lib or Regular used in PacketList.
//		#define AFX_EXT_CLASS_DD			
	#else													// in Other, To Link
		#define AFX_EXT_CLASS_PACKETDATA	AFX_CLASS_IMPORT
	#endif
#endif

class CPacketList;
class CPacketImplementation;
class AFX_EXT_CLASS_PACKETDATA CPacket : public CObject  
{
public:	
	CPacket(CPacketList * pParentList,
			CString strName, 
			CString strFieldName, 
			CString strType,
			CString szExpressionType,
			int nDataLength, 
			bool bUseReal,
			int nStartPeriod = 0,
			CString strRealFieldName = "");

	virtual ~CPacket();

	//nami 9.5
	bool RestoreMax();
	bool RestoreMin();
	bool RestoreMinMax();
//	double GetMin()	const;
//	double GetMax() const;
	
	bool SetMax(const double nMax);
	bool SetMin(const double nMin);
	bool SetMinMax(const double nMin, const double nMax);
	void SetBaseData(const CString &strType, 
					 const CString &strExpressionType,
					 const int nStartPeriod, 
					 const bool bIsAutoMinMax = true);

	void SetYesOrNoBaseData(const bool bBaseData);
	void SetAcceptFromServer(const bool bAcceptFromServer);
	bool IsTRData() const;
	bool IsAcceptFromServer() const;
	// data �⺻�� "0" �� packet
	bool IsZeroBasePacket() const;
	bool IsChangeRealData() const;
	//	Packet�� ���� �� data�� ����
	int GetLength();
	//	data�� ���� return
	virtual int GetCount() const;
	//  ���� ȭ�鿡 ���̴� data ����
	int GetDataCount();
	//  ��ü ����Ÿ�� ���� ����� ����
	int GetMaxCount();
	//	CPacket�� FieldName�� ��´� (�� �����ν� ������ �̸�)
	CString GetFieldName();
	//	CPacket�� Name�� ��´� (������ �̸� or ����ڰ� ������ �̸�)
	CString GetName();
	// 20100324_JS.Kim �ַθ�.FX	�ð����� ���� ��Ŷ�� �ð�����ó�� ����ϱ� ���� ó��
	CString GetSubName();
	void	SetSubName(CString strSubName);
	//nami 9.30
	CPacketList* GetPacketList();
	CPacket* GetDatePacket();
	CPacketType::COMMODITY_BOND_NAME GetCommodityBondName() const;
	void SetCommodityBondName(const CPacketType::COMMODITY_BOND_NAME eCommodityBondName);
	// fail : -1  success : index of data
	int GetIndexFromData(double nData);
	// ����Ʈ �����ð� ���� - ojtaso (20080811)
	int GetIndexFromData(double nData, CString strType);
	// return : true -> ���� ���� ���
	//			false -> ������ ����ų� ���� ���� ���� ���
	bool GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData) const;
	bool GetData(const int nIndex, double& dData) const;
	// fail : ERRORVALUE   success : data of index
	double GetData(int nIndex);
	CString GetStrData(int nIndex);
	// ���ſ� �̷� ������ �ð������� return�Ѵ�
	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//int GetDataPastandFuture(int nIndex);
	double GetDataPastandFuture(int nIndex);
	// 2011.01.06 by SYS <<
	bool GetDataPastandFuture(const int nIndex, double& dData);
	int GetTimeInterval();
	int GetDateUnitType();
	CString GetRealKeyName();
	CString GetRealFieldName();
	bool GetUseReal();
	bool IsUseStandardValue( const int p_nType = 0) const;		// 0:Base,	1:Upper Base,	2:Lower Base
	double GetStandardValue( const int p_nType = 0);			// 0:Base,	1:Upper Base,	2:Lower Base
	int GetStartPeriod();
	void SetRealDataGap( const double &p_dDataGap);
	bool GetRealDataGap(double& dDataGap) const;
	//nami 9.29
	CString GetType(const bool bIsFullType = true);
	double GetTypeFormatDouble(const bool bIsFullType = true);
	//	CPacket�� data list�� �����Ѵ�
	const CList<double, double>* GetnumericDataList() const;
	CList<double, double>* GetnumericDataList();
	CList<CString, CString>* GetstrDataList();
	//	data�� Minimum & Maximum���� ������
	//	nStart : data�� ���� ��ġ
	//	nEnd   : data�� �� ��ġ
	//	dMin   : ������ Minimum �� 
	//	dMax   : ������ Maximum ��
	//	return value : success = 0 or 1
	//	-2 : type error
	//	-1 : error
	//	 0 : nEnd - nStart = Data Count -> success
	//	 1 : success
	// (2009/1/17 - Seung-Won, Bae) Add without-Zero option.
	int GetMinMax(int nStart, int nEnd, double& nMin, double& nMax, BOOL p_bWithoutZero = FALSE);

	// data�� Minimun & Maximum ���� ���ؼ� ������
	// parameter 
	//   nInsertType : NEW, INSERT, APPEND
	//	 ptDataRange : �߰��� ����Ÿ�� ó����ġ, ����ġ (default -1, -1));
	// return
	//	-2 : type error
	//	-1 : error
	//	 none zero : success
	int SetMinMax(CPacketDataInfo::DATAINSERTTYPE nInsertType = CPacketDataInfo::NEW, CPoint ptDataRange = CPoint(0, 0));
	//	CPacket�� �ǽð� �޴� Packet�� ��� �ǽð� data Field Name�� �����Ѵ�
	void SetRealFieldName(CString strRealFieldName, CString strRealKeyName);
	void SetBoundLimit(double nUpperBound, double nLowerBound);
	void SetStandardValue( const char *p_szValue);
	void SetStartPeriod(int nStartPeriod);
	void SetName(CString strName);
	void SetType(const CString& strType);
	void SetType(const CString& strFullType, const CString& strExpressionType);
	void SetLength(int nLength);

	// (2007/2/7 - Seung-Won, Bae) for Exception Value
	void	SetExceptionValue( const double &p_dExceptionValue);
	double	GetExceptionValue( void);

	// (2007/2/22 - Seung-Won, Bae) for Continue Code
	void SetContinueCode( const double &p_dContinueCode);
	double GetContinueCode( void);

	//	CPacket���� data�� �־��� data�� �ٲ۴� list�� data���� ��� copy�ȴ�
	void ChangeData(CList<double, double>& lsnewData);
	void ChangeData(CList<CString, CString>& lsnewData);

	//	String ���·� ���� data�� data list�� insert (head�� �ش�)
	void InsertData(CString strData);	
	//	�ּҷ� ���� data��  �о int������ data list�� insert (head�� �ش�)
	void InsertData(long pData, int nDataLength);
	//	�ּҷ� ���� data��  �о int������ data list�� append (tail�� �ش�)
	void AppendData(long pData, int nDataLength);
	//	String ���·� ���� data�� int������ �ٲپ data list�� append (tail�� �ش�)
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
	bool RemoveBaseData( int p_nCount);
	int RemoveFictionData(const double dReallastDate, const bool bDate = false);
	// CPacket���� data�� clear��
	virtual int ClearData(const bool bClearIncludingTR = false);	
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
	CPacketImplementation* m_pPacketImplementation;

// (2006/12/20 - Seung-Won, Bae) Support Binary TR Data Receiving
public:
	void	AppendTrData( const char *p_szData);
	void	AppendTrData( const double dData);
	void	InsertTrData( const char *p_szData);
	void	InsertTrData( const double dData);

// (2007/2/21 - Seung-Won, Bae) Support Not-Auto Scale Type.
public:
	void	SetAutoScaleType( BOOL p_bAutoScaleType);
	BOOL	IsAutoScaleType( void);
	void	SetScaleType( const CString &p_strScaleType);
	CString	GetScaleType( void);
	double	GetScaleTypeFormatDouble( void);
	// ���簡 ����� - ojtaso (20080602)
	void OffsetPacketData(double dData);

// 20080908 ������	8����/32���� ǥ�� ����
public:
	void	SetBaseNumber(int nBase);
	int		GetBaseNumber();
	void	SetDecLen(int nDecLen, int nDecLen2);
	int		GetDecLen(int &nDecLen, int &nDecLen2);

// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
//		Do not support Full Range Min/Max for collision with Pre-Min/Max.
public:
	void	ClearMinMax( void);

// (2009/5/21 - Seung-Won, Bae) Shift the data of packet to future.
public:
	BOOL	ShiftData( long p_szShiftCount);

// 20081007 JS.Kim	����ڽð��� ����
public:
	void	CalcPacketTimeDiff(int nTimeDiff);
	double	CalcTimeDiff(double dData);
	double	CalcTimeDiff(double dData, int nTimeDiff);
	double	CalcBackTimeDiff(double dData);		// ���� �ð���� �ǵ����� �Լ�
};

class AFX_EXT_CLASS_PACKETDATA CBracket {
public :
	CBracket( CString szItem, int nIndex, int nDate);
	~CBracket();

	int				GetIndex()	const	{	return m_nIndex;	}
	int				GetDate()	const	{	return m_nDate;		}
	const char *	GetItem()	const	{	return m_szItem;	}

private :
	int		m_nIndex;
	int		m_nDate;
	CString	m_szItem;
};

class AFX_EXT_CLASS_PACKETDATA CMPItem {
public :
	CMPItem();
	~CMPItem();

	int		GetPriceX100()	const { return m_nPriceX100;	}
	DWORD	GetVolume()		const { return m_dwVolume;		}
	CList<CBracket*, CBracket*>* GetBracketList()	{ return &m_listBracket; }

	void	SetPrice(	int		nPriceX100)	{	m_nPriceX100	=	nPriceX100;		}
	void	AddVolume(	DWORD	dwVolume)	{	m_dwVolume		+=	dwVolume;		}

// (2004.01.19, ��¿�, �м�) ���ο� CBracket Object�� ���� �� Data�� �����Ѵ�.
public:
	void	AddBracket( CString& strAlpa, int nIndex, int nDate);

// (2004.01.20, ��¿�, �м�) Price�� �� ����� �ǹǷ� Int�� �����Ѵ�.
// (2004.01.26, ��¿�) Volume�� Max Volume ��� ���ϴ� ��찡 �־� double�� �̿����� �ʴ´�.
private :
	int		m_nPriceX100;
	DWORD	m_dwVolume;
	CList< CBracket *, CBracket *> m_listBracket;
};

class AFX_EXT_CLASS_PACKETDATA CMProfile {
public :
	CMProfile();
	~CMProfile();

	int			GetDate() const					{	return m_nDate;	}
	CList<CMPItem*, CMPItem*>* GetMPItemList()	{	return &m_listMPItem; }
	CMPItem*	GetMPItem(const int nIndex);
	int			AddMPItem(CMPItem *item);
	int			GetTotalBracketCount();
	int			GetMaxVolumeIndex();
	virtual int	GetCount() const;

	CMPItem*	AddHead();
	CMPItem*	AddTail();
	CMPItem*	InsertBefore(int nIndex);
	CMPItem*	InsertAfter(int nIndex);
	void		SetDate(const int nDate)	{	m_nDate = nDate;	}

private:
	int m_nDate;
	CList<CMPItem*, CMPItem*> m_listMPItem;

// (2004.01.26, ��¿�) Max Volume�� Max Bracket Count�� CMPItem�� 1�� �̻��� ��츦 �����Ͽ�
//		Max�� Index���� �������� �ʰ�, �� ���� ��������Ѵ�.
public:
	int			GetMaxBracketCount( void);
	DWORD		GetMaxVolume( void);

// (2004.01.19, ��¿�) ���� MP Chart�� ���� ���� ũ�⸦ ���Ѵ�. (���� ��ǥ���� �����̴�.)
//		(�̰������� GetPriceScaleX100()�� �����ϰ� ������ PriceUnitX100�� ���� ���ϴ� ����� �������� �ʴ´�.)
public:
	BOOL	GetPriceMinMaxX100( int &nMinX100, int &nMaxX100);

// (2004.01.27, ��¿�) ��ϵ� CMPItem�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	BOOL	IsEmpty( void)	{	return m_listMPItem.IsEmpty();	}

// (2004.02.03, ��¿�) ���� ������ Last Bracket Mark�� ��µǵ��� �Ѵ�.
//		MP Chart���� ��½� �̿��� ���� ������ �����Ѵ�.
protected:
	int		m_nLastPriceX100;
public:
	int		GetLastPriceX100( void) const		{	return m_nLastPriceX100;				}
	void	SetLastPriceX100( int p_nLastPriceX100)	{	m_nLastPriceX100 = p_nLastPriceX100;	}
};

class AFX_EXT_CLASS_PACKETDATA CMPPacket : public CPacket  
{
public :
	CMPPacket(CPacketList* pParentList, CString szName, CString szFieldName, 
			CString szType, CString szExpType, int nLength, bool bUseReal, 
			int nStartPeriod = 0, CString szRealFieldName = "");

	CMPPacket(CPacketList *pPacketList, CString szName);
	virtual ~CMPPacket();
	
	virtual int	GetCount() const;
	virtual int	ClearData(const bool bClearIncludingTR = false);

	int			GetMaxHitting();
	int			GetMaxMPItemCount();
	int			GetTotalMPItemCount();
	int			AddMarketProfile(CMProfile *mp);
	CMProfile*	AddTail();
	CMProfile*	GetMarketProfile(const int nIndex);
	CList< CMProfile*, CMProfile*>* GetMarketProfileList() { return &m_listMarketProfile; }

private :
	CList<CMProfile*, CMProfile*> m_listMarketProfile;

	void		RemoveAll();

// (2004.01.26, ��¿�) MProfile�� GetMaxVolume()�� �̿��Ͽ� ó���Ѵ�.
public:
	DWORD 		GetMaxVolume( void);

// (2004.01.19, ��¿�) Drawing �ÿ� ������ �� �ֵ��� ���� ���� �����Ѵ�.
protected:
	int		m_nPriceUnitX100;
public:
	void	SetPriceUnitX100( int p_nPriceUnitX100)	{	m_nPriceUnitX100 = p_nPriceUnitX100;	}
	int		GetPriceUnitX100( void) const			{	return m_nPriceUnitX100;	}

// (2004.01.19, ��¿�) Drawing �ÿ� Y Scale�� �����࿡ �����ϴ� ������ �����Ѵ�.
//		(�� ���� ���� ������ ���Ѱ����� Scale������ Max�� �������� ������ ��ġ�� ��ȯ�ȴ�.)
public:
	BOOL	GetPriceScaleX100( int &nMinX100, int &nMaxX100);

// (2004.01.20, ��¿�) Drawing�ÿ� X Scale�� Bracket ���� ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	int		GetMaxBracketCount( void) const;
};

#endif // !defined(AFX_PACKETBASE_H__DDFE472D_2D8F_11D5_81A5_0050FC28B229__INCLUDED_)
