#if !defined(AFX_STRATEGYWND_H__44B9B7B2_99DB_4966_95EC_AC22B1A93F42__INCLUDED_)
#define AFX_STRATEGYWND_H__44B9B7B2_99DB_4966_95EC_AC22B1A93F42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StrategyWnd.h : header file
//
//#include <AFXMT.H>
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CStrategyWnd window
#ifdef _DEBUG
#define _SETLOG
#endif

#include "../Common_ST/IStrategyItem.h"
#include "../Common_ST/STControllerDef.h"
#include "../Common_ST/STControllerDef2.h"
#include "../Common_ST/SDataItem.h"

#include "LogicMaker2.h"
#include "../Common_ST/DealDataItem.h"

class CForcedLiquidationInfo
{
public:
	CForcedLiquidationInfo();

public:
	int				m_nExitIndex;
	long			m_lDealValue;
	CIDataItem*		m_pDataItem;

public:
	void SetFLInfo(int nExitIndex, long lDealValue, CIDataItem* pDataItem);
};

class CFeeCal;
class CLogicItem;
class CStrategyWnd : public CWnd, public CIStrategyItem
{
// Construction
public:
	virtual ~CStrategyWnd();

	// 20060705 �Լ����� ���� : HWND hSocket,/*long pDBMgr,*/
	//int Create(CWnd* pParentWnd,HWND hSocket,/*long pDBMgr,*/ long lKey, CStringArray &rStArrayPath);
	virtual int Create(CWnd* pParentWnd, long lKey, CStringArray &rStArrayPath, long pChartItem);

	virtual BOOL DestoryWindow();
	virtual HWND GetSafeHwnd( ) const;
	// atcion command
	virtual BOOL LoadSD(LPCTSTR lpStrategyDllPath,LPCTSTR lpInputCode,long lDataCnt,LPCTSTR lpEndDate, LPCTSTR lpTimeTypeNum,LPCTSTR lpTypeDealOption);	
	virtual BOOL LoadST(LPCTSTR lpStrategyPath,LPCTSTR lpInputCode,long lDataCnt,LPCTSTR lpEndDate);
	virtual BOOL Run(LPCTSTR lpOption);
	virtual BOOL Stop();
	virtual BOOL Pause();
	virtual long Update(long lPos, BOOL bIsUpdateData);		//@060922����Real���ü���
	virtual CString GetTime();
	virtual long GetCount();
	virtual CString GetInputCode();
	virtual CString GetEndDate();
	virtual long GetDataCount(long lIndex);
	virtual CString GetTime(long lIndex);
	virtual long GetPtrDrawInfo(long lIndex);
	virtual BOOL CalculateStatisticalInfo(long lStartPos, long lEndPos);
	virtual long GetCntStrategyResult();
	virtual CStrategyResult* GetStrategyResult(long lIndex);
	virtual double GetStatisticalInfo(long lPos);
	virtual LPCTSTR GetErrorMessage();
	virtual BOOL HasAPData();
	virtual OPERATTIONSTATUS GetOperationStatus();
	virtual CString GetInputs(long lIndexST, long lIndexInput);
	virtual void GetCurrentPositonNProfit(long& rLCurrentDealPosition, double &rDCurrentProfitRate);
	virtual CString GetName(long lIndex);
	virtual CString GetPath(long lIndex);
	virtual CString GetProfileInfo(LPCTSTR lpAppName,LPCTSTR lpKeyName);
	virtual BOOL SetUpperLowerPrice(double dUpperPrice,double dLowerPrice);
	virtual BOOL SetVolumeForCalculatation(double dVol);// to calculate statical info..
	virtual double GetVolumeInDealData();

	//KHD : �ð�����ȭ ���� ����
	virtual int GetTimeBong(){return m_nBong;}
	virtual void SetTimeBong(int nBong){m_nBong = nBong; }
	//KHD : ��������ȭ ����
	virtual void SetValOptimizer(LONG str){
		m_pstValOptimazer =str;
	}
	virtual LONG GetValOptimizer(){
		return (LONG)m_pstValOptimazer;
	}
	virtual CString GetSendValString(){
		return m_stSendValue;
	}
	virtual void SetSendValString(CString stSend){
		m_stSendValue = stSend;
	}
	// atcion command
	virtual void OnSTSetRealDataST();
	virtual void	SetChartDivideInfo(int nDivideState, int nUnitPos);	// ��Ʈ �ð����� ����
	
public:
	BOOL IsRunning();
	void RemoveAllArray();
	CStrategyResult *CreateStrategyResultData();
	void CalaulateStrategyResultData();
	LRESULT OnSTDataManagerMessageSD( WPARAM wParam, LPARAM lParam);
	LRESULT OnSTDataManagerMessageST( WPARAM wParam, LPARAM lParam);
	void OnSTSetAPDataST();
	void Init();
	void ExitAtTodayByTimeSelected(BOOL bStartTimer = TRUE);

// Attributes
private:
	CLogicMaker2 m_logicMaker2;
	CString m_strTimeTypeNum;
	int	 m_nLastSize;
	// For Action..
	double m_dUpperPrice;
	double m_dLowerPrice;
	double m_dVolForOrder;
	OPERATTIONSTATUS m_Status;
	ST_DRAWINFO_EX m_DrawInfo;
	CPtrArray m_ptrArrayPlot;
	CPtrArray m_ptrArraySDataItem;
	CPtrArray m_ptrArrayHistoryDataItem;

	CLogicItem* m_pLogicRootItem;
	int m_nCurrentRunningLogic;
	long m_pChartInterface;
	CString m_strStrategyPath;
	CISDataItem* m_pDateItem;
	CDealDataItem m_dealDataItem;
	BOOL m_bHasAPData;
	BOOL m_bIsRunning;
	long m_lKey;
	CString m_strInputCode;
	long m_lDataCnt;
	CString m_strEndDate;
	CString m_strCurrentTime;
	CString m_strError;

	// For SD Load
	CString m_strTypeSell;
	CString m_strTypeBuy;
	double m_dSellOption;
	double m_dBuyOption;

	// For Order Setting
	long m_lFirstOrderType;// 0:Buy, 1:Sell, 2:All
	BOOL m_bUseProfitReverseRate;
	double m_dProfitReverseRate;
	BOOL m_bEnableContinuty;
	long m_lMaxContinuty;
	double m_dMinimumEquity;
	double m_dMinimumFuture;
	double m_dFeeForEntry;
	double m_dFeeForExit;
	long m_lFeeUnit;
	double m_dSlippageForEntry;
	double m_dSlippageForExit;
	long m_lSlippageUnit;


	CFeeCal *m_pFeeCal;
	CFeeCal *m_pSlippageCal;

	enum ORDER_TIME	
	{
		ATCompletingCandle = 0,
		AtEventingSignal = 1
	};
	ORDER_TIME m_OrderTimeOfForcedExit;

	// For Calculation Of Realtime Exchange
	long m_lCurrentDealPosition;	// deal status
	double m_dCurrentProfitRate;	// rate status
	double m_dCurrentProfitRateOld;
	double m_dVolumeBought;
	double m_dAmountBought;
	double m_dAmountAccmulated;
	double m_dVolumeSold;
	double m_dAmountSold;
	
	// for Calculation of Statistical infomation..
	CArray<CStrategyResult*,CStrategyResult*>	m_arrayStrategyResult;
	double m_dSumLoss;
	double m_dMaxProfit;
	double m_dMaxLoss;
	double m_dSumAmountExit;
	double m_dSumProfit;
	long m_lCntProfit;
	long m_lCntLoss;
	long m_lCntProfitStraight;
	long m_lCntLossStraight;
	long m_lSumProfitStraight;
	long m_lSumLossStraight;
	long m_lSumCandleProfit;
	long m_lSumCandleLoss;
	long m_lCntCandleAccess;
	long m_lAllCandleInCalculation;

	CStringArray m_strArrayPath; //0: system, 1:User

	enum ST_TYPE {
		NONE,
		ST_ST = 1,
		ST_SD
	};
	ST_TYPE m_strategyType;
	
	int m_nBong; //KHD : �ð�����ȭ�� �� ���� ���� ��
	LONG m_pstValOptimazer;//KHD :��������ȭ����...
	CString m_stSendValue;//KHD : ��������ȭ����...
	// For Action..
	// ������ �� ���� ���� 
	double m_dMedoRate;//�ż�û��
	double m_dMesuRate;//�ż�����
	long   m_dqty;//����
	BOOL    m_bncbTax;// FALSE : % TRUE:Point
	//��������
	double m_dMesuSlip;//SlipigyBuy
	double m_dMedoSlip;//SlipigySell
	BOOL   m_bSlipTex;//cbSplipigy FALSE : % TRUE : Point

	UINT	m_nTimerID;

public:
// ��Ʈ �ð����� ���� ����
	int		m_nDivideState;										// ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5)
	int		m_nUnitPos;											// 1(0),5(1),10(2),15(3)


// �������� �������
	void	InitConfiguration();								// �������� �ʱ�ȭ
	void	LoadConfiguration();								// �������� ����
	void	ReadConfiguration(CString szPath);					// �������� ����
	void	ReadConfig_Signal(CString szPath);					// ��ȣ(Setup) ���� ����
	void	ReadConfig_ForcedLiquidation(CString szPath);		// ����û��(Forced Liquidation) ���� ����
	void	ReadConfig_Setup(CString szPath);					// ���(Setup) ���� ����
	void	ReadConfig_Order(CString szPath);					// �ֹ�(Order) ���� ����

	// ����û�� ���ú��� �� �Լ�
	int		m_nTiming_ForcedLiquidation;	// ����û�� ���� (Real�� ��츸 ���)
	BOOL	m_bIsFirstSignal;				// ���� ��ȣ����?

	//������
	BOOL	m_bUseLossRate;
	double	m_dLossRate;
	UINT	m_uLossRate_PerOrWon;			// ������ % or ��

	//��ǥ����
	BOOL	m_bUseTargetBenefit;
	double	m_dTargetBenefit;
	UINT	m_uTargetBenefit_PerOrWon;		// ��ǥ���� % or ��

	//���� ����� �϶�
	BOOL	m_bUseHPriceCPBear;
	double	m_dHPriceCPBear;
	UINT	m_uHPriceCPBear_PerOrWon;		// ������϶� % or ��

	//���� ������� ���
	BOOL	m_bUseLPriceCPBull;
	double	m_dLPriceCPBull;
	UINT	m_uLPriceCPBull_PerOrWon;		// ��������� % or ��

	//��ǥ������
	BOOL	m_bUseTargetValuePreserve;
	double	m_dTargetValuePreserve;				// ��ǥ��
	UINT	m_uTargetValuePreserve_PerOrWon;		// ��ǥ������ % or ��

	//���ͺ���
	BOOL	m_bUseBenefitPreserve;
	double	m_dBenefitPreserveUp;				// �����
	double	m_dBenefitPreserveDn;				// �϶���
	UINT	m_uBenefitPreserveUp_PerOrWon;		// ���ͺ���(�����) % or ��
	UINT	m_uBenefitPreserveDn_PerOrWon;		// ���ͺ���(�϶���) % or ��

	//�ּҰ��ݺ�ȭ
	BOOL	m_bUseMiniPrice;
	int		m_nBongCount;
	double	m_dMiniPrice;
	UINT	m_uMiniPrice_PerOrWon;			// �ּҰ��� % or ��

	//����û�� ����
	BOOL	m_bUseDayLiquidation;
	CString	m_szTimeDayLiquidation;
	UINT	m_uDayLiquidation_PerOrWon;		// ����û�� % or ��

	// ������ȣ���� �迭
	CArray<double, double> m_arrayDealPosition;
	CArray<CString, LPCTSTR> m_arrayDealName;
	CArray<double, double> m_arrayDealDrawPosition;
	CDealData	m_TempDealData;
	CDealData	m_TempCondDealData;
	CDealData	m_TempForcedDealData;

	//////////////////////////////////////////////////////////////////////////
	// vntsorl �߰�
	BOOL			m_bBuyDealTraceInit;		// ���� �ż�/�ż�û�� DealTrace ����
	BOOL			m_bSellDealTraceInit;		// ���� �ŵ�/�ŵ�û�� DealTrace ����
	CBuyDealTrace	m_buyDealTrace;				// �ż�/�ż�û�� DealTrace ��ü
	CSellDealTrace	m_sellDealTrace;			// �ŵ�/�ŵ�û�� DealTrace ��ü

	BOOL			m_bBuyOrderStartSignal;		// �ż�/�ż�û�� �ֹ����۽�ȣ ó������(����)
	BOOL			m_bSellOrderStartSignal;	// �ŵ�/�ŵ�û�� �ֹ����۽�ȣ ó������(����)

	void	InitBuyDealTrace(BOOL bRealState = FALSE, int nInterval = 0);
	void	InitSellDealTrace(BOOL bRealState = FALSE, int nInterval = 0);
	void	ExecuteBuyDealTrace(CDealData* pDealData, BOOL bRealState = FALSE, int nInterval = 0);
	void	ExecuteSellDealTrace(CDealData* pDealData, BOOL bRealState = FALSE, int nInterval = 0);
	void	ExecuteDealTrace();
	void	ExecuteDealTraceReal(CDealData* pDealData, int& nTotalCount, int& nInterval);
	BOOL	ExecuteDealTraceExceptional(CDealData* pDealData, int& nTotalCount, int& nInterval);
	BOOL	ExecuteDealTraceUsual(CDealData* pDealData, int& nTotalCount, int& nInterval);

	void	ConfirmBuyForcedLiquidation(BOOL bRealState = FALSE, int nInterval = 0);
	void	ConfirmSellForcedLiquidation(BOOL bRealState = FALSE, int nInterval = 0);

	void	BuyForcedLiquidation_LossRate(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);				// �����ŷ� ���� ����û��
	void	BuyForcedLiquidation_TargetBenefit(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);		// ��ǥ�������� ���� ����û��
	void	BuyForcedLiquidation_HPriceCPBear(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);			// ������϶����� ���� ����û��
	void	BuyForcedLiquidation_TargetValuePreserve(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);	// ��ǥ�������� ���� ����û��
	void	BuyForcedLiquidation_BenefitPreserve(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);		// ���ͺ����� ���� ����û�� 
	void	BuyForcedLiquidation_MiniPrice(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);			// �ּҰ������� ���� ����û��
	void	BuyForcedLiquidation_DayLiquidation(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);		// ����û������ ���� ����û��
	void	BuyForcedLiquidation(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE, int nInterval = 0);

	void	SellForcedLiquidation_LossRate(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);			// �����ŷ� ���� ����û��
	void	SellForcedLiquidation_TargetBenefit(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);		// ��ǥ�������� ���� ����û��
	void	SellForcedLiquidation_HPriceCPBear(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);		// ������϶����� ���� ����û��
	void	SellForcedLiquidation_TargetValuePreserve(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);	// ��ǥ�������� ���� ����û��
	void	SellForcedLiquidation_BenefitPreserve(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);		// ���ͺ����� ���� ����û�� 
	void	SellForcedLiquidation_MiniPrice(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);			// �ּҰ������� ���� ����û��
	void	SellForcedLiquidation_DayLiquidation(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);		// ����û������ ���� ����û��
	void	SellForcedLiquidation(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE, int nInterval = 0);
	//////////////////////////////////////////////////////////////////////////

	void	InitStrategySignalInfo();																// �������� �ʱ�ȭ
	BOOL	IsFirstLiquidation(CDealData* pEntryDealData, BOOL& bExist_ForcedLiquidation);			// û���ȣ�� ��ó���ΰ�?
	BOOL	IsLastEntry(CDealData* pEntryDealData, BOOL& bExist_ForcedLiquidation, POSITION& posLast);	// ���Խ�ȣ�� �������ΰ�?
	BOOL	IsLastEntry_ForReal(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation);
	void	BuyOrExitShort(CDealData* pEntryDealData, BOOL& bExist_ForcedLiquidation);				// �ż� / �ŵ�û��
	void	SellOrExitLong(CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation);				// �ŵ� / �ż�û��
	void	SellOrExitLong_ForReal(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation);
	void	Confirm_ForcedLiquidation(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation);			// ����û�� �ɼ�Ȯ��
	void	Confirm_ForcedLiquidation_ForReal(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation);	// ����û�� �ɼ�Ȯ��

	BOOL	IsBuy(long lDealType);						// �ż� ��뿩��
	BOOL	IsExitShort(long lDealType);				// �ŵ�û�� ��뿩��
	BOOL	IsSell(long lDealType);						// �ŵ� ��뿩��
	BOOL	IsExitLong(long lDealType);					// �ż�û�� ��뿩��
	BOOL	IsBuyOrExitShort(long lDealType);			// �ż� / �ŵ�û�� ��뿩��
	BOOL	IsSellOrExitLong(long lDealType);			// �ŵ� / �ż�û�� ��뿩��

	BOOL	IsLossRate();								// ������ ��뿩��
	BOOL	IsTargetBenefit();							// ��ǥ���� ��뿩��
	BOOL	IsHPriceCPBear();							// ������϶� ��뿩��
	BOOL	IsLPriceCPBull();							// ��������� ��뿩��
	BOOL	IsBenefitPreserve();						// ���ͺ��� ��뿩��
	BOOL	IsTargetValuePreserve();					// ��ǥ������ ��뿩��
	BOOL	IsMiniPrice();								// �ּҰ��� ��뿩��
	BOOL	IsDayLiquidation();							// ����û�� ��뿩��
	BOOL	IsForcedLiquidation(long lDealValue);		//����û�� ����

	void	ForcedLiquidation_ForLossRate(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo);				// �����ŷ� ���� ����û��
	void	ForcedLiquidation_ForTargetBenefit(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo);		// ��ǥ�������� ���� ����û��
	void	ForcedLiquidation_ForHPriceCPBear(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo);			// ������϶����� ���� ����û��
//	void	ForcedLiquidation_ForLPriceCPBull(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo);			// ������������� ���� ����û��
	void	ForcedLiquidation_ForTargetValuePreserve(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo);	// ��ǥ�������� ���� ����û��
	void	ForcedLiquidation_ForBenefitPreserve(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo);		// ���ͺ����� ���� ����û�� 
	void	ForcedLiquidation_ForMiniPrice(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo);			// �ּҰ������� ���� ����û��
	void	ForcedLiquidation_ForDayLiquidation(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo);		// ����û������ ���� ����û��
	void	ForcedLiquidation(CDealData* pExitDealData, CForcedLiquidationInfo& FLInfo);
	void	ForcedLiquidation_ForReal(CDealData* pExitDealData, CForcedLiquidationInfo& FLInfo);

	void	ResizeSignalInfoArray(int nInterval, int nTotalSignalCnt);		// ��ȣ���� ���� Array ������ ���� (�ǽð� ������Ʈ�� �߻�)
	double	ApplySlipAndTex(double dMulData, BOOL bType,double nNum);

	// ��� ���ú��� �� �Լ�
	int		m_nUseRepeatEntry;				// �ߺ����� ����
	int		m_nRepeatEntryCount;			// �ߺ����� ���Ƚ��
	int		m_nRepeatCount;					// �ߺ����� Ƚ������

	// �ֹ� ���ú��� �� �Լ�
	int		m_nOrderAmount;					// �ֹ�����
	int		m_nTiming_Order;				// �ֹ����� (Real�� ��츸 ���)
	int		m_nOrderStartSignal;			// �ֹ����۽�ȣ
	BOOL	m_bCheckPrev_EntryPosition;		// ������ ��ȣ DealValue
	BOOL	m_bSetComplete_OrderInfo;		// �ֹ����� ���ÿϷ� (���� ������ �ֹ���ȣ �߻��� ���� �ֹ����� ���� ����)
	BOOL	m_bCanRemove_OrderInfo;			// �ֹ����� ���Ű��� (���� ������ �ֹ���ȣ �߻��� �ֹ����� ���� ���� ����)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStrategyWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	CStrategyWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStrategyWnd)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	LRESULT OnSTDataManagerMessage( WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
		
//	CCriticalSection m_CriticalSection;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRATEGYWND_H__44B9B7B2_99DB_4966_95EC_AC22B1A93F42__INCLUDED_)
