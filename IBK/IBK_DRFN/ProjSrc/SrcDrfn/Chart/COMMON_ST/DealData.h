// DealData.h: interface for the CDealData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALDATA_H__91F3027D_1166_48C9_9BBA_67ABF195A063__INCLUDED_)
#define AFX_DEALDATA_H__91F3027D_1166_48C9_9BBA_67ABF195A063__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDealData  
{
public:
	CDealData();	
	virtual ~CDealData();
	CDealData(const CDealData& rhs);
	
	CDealData operator= (const CDealData& rhs );	// ASSIGN
	CDealData operator& (const CDealData& rhs );	// AND
	CDealData operator| (const CDealData& rhs );	// OR

	void Clear();

	int			m_nIndex;
	long		m_lDealValue;
	long		m_lType;
	double		m_dPrice;
	double		m_dContract;
	CString		m_strDealName;
	CStringList m_strListEntryNameForExit;
};

class CDealTraceBase
{
public:
	CDealTraceBase();	
	virtual ~CDealTraceBase();

public:
	BOOL	IsNonePosition();						// ��ȣ ����
	BOOL	IsNonePosition(long lDealValue);		// ��ȣ ����
	BOOL	IsBuy(long lDealValue);					// �ż� ��ȣ
	BOOL	IsExitShort(long lDealValue);			// �ŵ�û�� ��ȣ
	BOOL	IsSell(long lDealValue);				// �ŵ� ��ȣ
	BOOL	IsExitLong(long lDealValue);			// �ż�û�� ��ȣ
//	BOOL	IsForcedLiquidation(long lDealValue);	// ����û�� ��ȣ

	BOOL	IsClearStateOfRaw();					// �ʱ�ȭ ����(Raw)�ΰ�?
	BOOL	IsClearStateOfCurr();					// �ʱ�ȭ ����(Curr)�ΰ�?
	BOOL	IsClearStateOfAll();					// �ʱ�ȭ ����(All)�ΰ�?

	BOOL	IsSameDealData(const CDealData* pDealData);

	int		GetRawIndex();
	long	GetRawDealValue();
	long	GetRawType();
	CString	GetRawDealName();
	double	GetRawPrice();
	double	GetRawContract();
	CStringList*	GetRawEntryNameForExit();

	int		GetCurrIndex();
	long	GetCurrDealValue();
	long	GetCurrType();
	CString	GetCurrDealName();
	double	GetCurrPrice();
	double	GetCurrContract();
	CStringList*	GetCurrEntryNameForExit();

	int		GetOrdrIndex();
	long	GetOrdrDealValue();
	long	GetOrdrType();
	CString	GetOrdrDealName();
	double	GetOrdrPrice();
	double	GetOrdrContract();
	CStringList*	GetOrdrEntryNameForExit();

//	void	InitRepeatCount();
//	void	SetForcedLiquidation(int nIndex, long lDealValue, BOOL bRealState = FALSE);

	void	InitDealData();
	void	SetRepeatInfo(const int nUseRepeatEntry = 0, const int nRepeatEntryCount = 1);
//	BOOL	IncreaseRepeatCount();
	void	SetTimingInfo(int nTiming_Order = 1, int nTiming_ForcedLiquidation = 1);
	int		GetTimingOrder();
	int		GetTimingForcedLiquidation();

	BOOL	Set(const CDealData* pDealData);
	void	Copy();
	void	Next();

	CDealData	m_DealDataRaw;						// �ǽð� Raw DealData
	CDealData	m_DealDataCurr;						// �ֹ� �� ����û�� DealData (���Ǹ�����)
	CDealData	m_DealDataOrdr;						// �ֹ� �� ����û�� DealData (���ϼ���)

public:
	virtual BOOL	IsEntryPosition();
	virtual BOOL	IsExitPosition();

protected:
	int		m_nUseRepeatEntry;						// �ߺ����� ��뿩��
	int		m_nRepeatEntryCount;					// �ߺ����� Ƚ��
//	int		m_nRepeatCount;							// �ߺ����� Trace

//	BOOL	LessThanRepeatEntryCount();

	int		m_nTiming_Order;						// �ֹ����� (0 : ���Ǹ�����, 1 : ���ϼ���)
	int		m_nTiming_ForcedLiquidation;			// ����û����� (0 : ���Ǹ�����, 1 : ���ϼ���)
};

class CBuyDealTrace : public CDealTraceBase
{
public:
	CBuyDealTrace();	
	virtual ~CBuyDealTrace();

public:
	BOOL	IsEntryPosition(long lDealValue);
	BOOL	IsExitPosition(long lDealValue);
	BOOL	IsForcedLiquidation(long lDealType);
	void	SetForcedLiquidation(int nIndex, long lDealValue, BOOL bRealState = FALSE);

	void	InitRepeatCount();
	BOOL	IncreaseRepeatCount();
	BOOL	LessThanRepeatEntryCount();

	BOOL	Init(CDealData* pDealData);
	BOOL	Set(const CDealData* pDealData, BOOL bRealState = FALSE);
	void	Next(BOOL bRealState = FALSE);

private:
	int		m_nRepeatCount;							// �ߺ����� Trace

public:
	virtual BOOL	IsEntryPosition();
	virtual BOOL	IsExitPosition();
};

class CSellDealTrace : public CDealTraceBase
{
public:
	CSellDealTrace();	
	virtual ~CSellDealTrace();

public:
	BOOL	IsEntryPosition(long lDealValue);
	BOOL	IsExitPosition(long lDealValue);
	BOOL	IsForcedLiquidation(long lDealType);
	void	SetForcedLiquidation(int nIndex, long lDealValue, BOOL bRealState = FALSE);

	void	InitRepeatCount();
	BOOL	IncreaseRepeatCount();
	BOOL	LessThanRepeatEntryCount();

	BOOL	Init(CDealData* pDealData);
	BOOL	Set(const CDealData* pDealData, BOOL bRealState = FALSE);
	void	Next(BOOL bRealState = FALSE);

private:
	int		m_nRepeatCount;							// �ߺ����� Trace

public:
	virtual BOOL	IsEntryPosition();
	virtual BOOL	IsExitPosition();
};

#endif // !defined(AFX_DEALDATA_H__91F3027D_1166_48C9_9BBA_67ABF195A063__INCLUDED_)
