#if !defined(AFX_WNDASSISTINDEXMANAGER_H__A82F4907_B365_480A_AA4C_C75327E251DA__INCLUDED_)
#define AFX_WNDASSISTINDEXMANAGER_H__A82F4907_B365_480A_AA4C_C75327E251DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndAssistIndexManager.h : header file
//
#include "./Include_AddIn/I139092/_IChartOCX.h"						// for IChartOCX
#include "../Common_ST/DataItem.h"
#include "../Common_ST/SDataItem.h"
#include "../Include_ST/ISTControllerD.h"
#include "../Common_ST/STControllerDef.h"
#include "../Common_ST/STControllerDef2.h"
#include "../Common_ST/DealDataItem.h"
#include "../Include_ST/STOrderDef.h"//�ֹ�����
#include "../Common_ST/StrategyLoader.h"
#include "../Common_ST/STConfigEnv.h"

//#include "../Common_ST/IStrategyItem.h"								// [WID_060913_A1]
#include "../../inc/IAuTrCommMng.h"
#include "../Include_ST/STStructDef.h"
#include <AFXMT.H>

/////////////////////////////////////////////////////////////////////////////
// CWndAssistIndexManager window

interface IPacket;
interface ITrComm;

class CDealDataItem;
class CIndexItem;
class CWndAssistIndexManager : public CWnd
{
// Construction
public:
	CWndAssistIndexManager();
	
	BOOL UpdateRawData();
//	BOOL UpdateRawDataEx(int nOffIndex, long lCntUpdate);
	BOOL SetRawData();

	// assist index /////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	void AddGraph(LPCTSTR lpszParamCombine, CList<IPacket*, IPacket*>* pListIPacket);
	BOOL RemoveIndexItem(LPCTSTR lpszParamCombine);
	/////////////////////////////////////////////////////////////////////////////////////////////

	// ST Index  ////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	BOOL AddST(LPCTSTR lpST, LPCTSTR lpSTPath);
	void Draw(CDC *p_pDC);
	void DeleteSTInfo();
	BOOL OnLButtonDblClk( UINT nFlags, CPoint &point);
	/////////////////////////////////////////////////////////////////////////////////////////////

	// ST Order  ////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	ST_ORDERSIGN	m_stOrderSign;
	ITrComm*				m_pITrComm;

	CMap<UINT, UINT, STORDER_INDATA*, STORDER_INDATA*> m_mapOrderData;
	CMap<UINT, UINT, PROCEED_ORDER*, PROCEED_ORDER*> m_mapProcOrderData;

	void OrderSign(STORDER_INDATA* lpOrderData);
	void MakeQueryPrice(STORDER_INDATA* lpOrderData);
	void QueryPrice(STORDER_INDATA* lpOrderData, UINT nOrderIndex);
	BOOL SendOrder(STORDER_INDATA* pOrderData, PROCEED_ORDER* pProceedOrderData);
	BOOL SendOrderStock(STORDER_INDATA* pOrderData, float fOrderPrice);
	BOOL SendOrderFuture(STORDER_INDATA* pOrderData, float fOrderPrice);

	void MakeQueryOrderQty(STORDER_INDATA* lpOrderData, PROCEED_ORDER* pProceedOrderData);

	virtual	HRESULT ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
	virtual	HRESULT ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData) {return S_FALSE;}
	virtual HRESULT StatusChange(int nState) {return S_FALSE;}

	//< CTAupTrAgent�� ���ŵ� ����Ÿ�� �Լ� �ݷ� �޾ƿ��� ���� Interface
	class CMyITrCommSite : public ITrCommSite
	{
	public:
		STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
		{
			METHOD_PROLOGUE(CWndAssistIndexManager, MyITrCommSite)
			return pThis->ReceiveData(dwKey, szTR, szMsgCode, szMessage, aTRData, dwTRDateLen);
		}

		STDMETHOD(ReceiveRealData) (WORD wFID, LPCSTR szMainKey, void* pRealData)
		{
			METHOD_PROLOGUE(CWndAssistIndexManager, MyITrCommSite)
			return pThis->ReceiveRealData(wFID, szMainKey, pRealData);
		}

		STDMETHOD(StatusChange) (int nState)
		{
			MYMETHOD_PROLOGUE(CWndAssistIndexManager, MyITrCommSite)
			return pThis->StatusChange(nState);
		}

		STDMETHOD(ReceiveMessage) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage)
		{
			MYMETHOD_PROLOGUE(CWndAssistIndexManager, MyITrCommSite)
			return S_OK;
		}
	} m_xMyITrCommSite;
	friend class CMyITrCommSite;
	/////////////////////////////////////////////////////////////////////////////////////////////

private:
	// assist index /////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	CIndexItem *CreateIndexItem(LPCTSTR lpszParamCombine,CList<IPacket*, IPacket*>* pListIPacket,BOOL &rBHasPriceInput);
	CMap<CString, LPCSTR, CIndexItem*, CIndexItem*> m_mapIndexItem;
	CMapStringToString m_MapIndexNoPriceInput;
	CStringArray m_arrayPacket;
	CString m_strPath;
	CString m_strBasicalExecutionOption;
	long	m_lCntOfCurrentPacketData;
	CString m_strTimeType;

	CString m_strCloseScaleType;
	CString m_strOpenScaleType;
	CString m_strHighScaleType;
	CString m_strLowScaleType;
	CString m_strVolumeScaleType;
	// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
	CString m_strExtScaleType;
	CString m_szPW;//���
	/////////////////////////////////////////////////////////////////////////////////////////////

	// ST Index  ////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	int GetXPositoin(double dDataOffset, double dCandleWidth, CRect rctGraphRegion,int &nLeftBorder,int &nRightBorder);
	long MakeDealShape(double dArrawWidth,CRgn &rShape,long lShape, long lSellBuy);
	void LoadEnvDealPos(CDealDataItem *pDataItem);
	void LoadEnvBullBear(CIDataItem *pDataItem);
	CString MakePacketOption(LPCTSTR lpPacketName,long lPacketSize,LPCTSTR lpPacketType, BOOL bUseInReal);
	CString MakeGraphOption(LPCTSTR lpChartType,LPCTSTR lpChartName, CStringArray& rArrayPacketList, LPCTSTR lpChartTitle, long lChartStyle =0, long lLineStyle =0 ,
									COLORREF clrGrp1 = RGB(218,46,46), COLORREF clrGrp2 = RGB(218,46,46),
									COLORREF clrGrp3 = RGB(46,46,218), COLORREF clrGrp4 = RGB(46,46,218),
									COLORREF clrGrp5 = RGB(218,46,46), COLORREF clrGrp6 = RGB(218,46,46),
									BOOL bUpFilled = 0, BOOL bDownFilled = 1,
									long lUpLineWidth = 1, long lDownLineWidth = 1, BOOL bHideGrp = 0);
	BOOL CopyPlotDataToPacket(IPacket *pIPacketDataItem, CIDataItem *pDataItem, LPCTSTR lpPacketName, long lSize, BOOL bIsInRealTime);
	CStringList m_listPacket;
	CStringList m_listGraph;
	CIndexItem* m_pSTItem;


	CArray<double,double> m_arrayDealDrawPosition;
	CArray<CString,LPCTSTR> m_arrayDealName;
	CArray<CString,LPCTSTR> m_arrayDealVol;
	CArray<double,double> m_arrayDealPosition;
	CArray<double,double> m_arrayBullBearPosition;

	//���� ���� ���� Array : KHD 
	CArray<double,double> m_arrayDivideDealDrawPosition;
	CArray<double,double> m_arrayDivideDealPosition;
	CArray<CString,LPCTSTR> m_arrayDivideDealName;
	////////////////////////////////////

	COLORREF	m_clrSell;
	COLORREF	m_clrBuy;
	COLORREF	m_clrExitLong;
	COLORREF	m_clrExitShort;
	COLORREF	m_clrBkBull;
	COLORREF	m_clrBkBear;
	CBrush		m_brushSell;
	CBrush		m_brushBuy;
	CBrush		m_brushExitLong;
	CBrush		m_brushExitShort;
	long		m_lShapeSignalSell;
	long		m_lShapeSignalBuy;
	long		m_lShapeSignalExitLong;
	long		m_lShapeSignalExitShort;
	BOOL		m_bShowName;
	BOOL		m_bShowQty;
	int m_nStartIndex;
	int m_nEndIndex;
	double m_dCandleWidth;
	CRect m_rctGraphRegion;
	CRect m_rctBlockRegion;
	CPen		m_penNormal;
	CString		m_strBullName;
	CString		m_strBearName;
	CRect m_rectTitle;	

	CRgn m_rgnBuy;
	CRgn m_rgnSell;
	CRgn m_rgnExitLong;
	CRgn m_rgnExitShort;
	long m_lHeightBuy;
	long m_lHeightSell;
	long m_lHeightExitLong;
	long m_lHeightExitShort;
	
	/////////////////////////////////////////////////////////////////////////////////////////////

	// ST Index Cfg ////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	CIndexItem* m_pSTItemCfg;

	/////////////////////////////////////////////////////////////////////////////////////////////
	long m_pStieCustomAddin;
public:
	IChartManager *m_pIChartManager;
	IChartOCX *m_pIChartOCX;
	IPacketManager *m_pIPacketManager;
	IGraphInfoManager *	m_pIGraphInfoManager;

	IPacket *m_pIPacketTime;
	IPacket *m_pIPacketOpen;
	IPacket *m_pIPacketHigh;
	IPacket *m_pIPacketLow;
	IPacket *m_pIPacketClose;
	IPacket *m_pIPacketVolume;
	// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
	IPacket *m_pIPacketExt;

	CSDataItemEx m_SDataItemDate;
	CSTDataItem m_DataItemOpen;
	CSTDataItem m_DataItemHigh;
	CSTDataItem m_DataItemLow;
	CSTDataItem m_DataItemClose;
	CSTDataItem m_DataItemVolume;
	// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
	CSTDataItem m_DataItemExt;

// Attributes
public:
	//KHD : 
	CArray<ST_DATAINDEX,ST_DATAINDEX> m_StDataIndex;
	CArray<CRect,CRect>m_arrayRectPeriod;
	BOOL m_bIsDragging;
	long		m_nSelectedRectIndex;
	BOOL		m_bIsContinuedByLeftOutside;
	BOOL		m_bIsContinuedByRightOutside;

	CBrush		m_brushPeriodFrame;
	COLORREF	m_clrPeriod;
	COLORREF	m_clrPeriodSelected;

	BOOL	m_bSetDataFirst;
	BOOL GetDataOffset(long& rLLeftIndex, long& rLRightIndex, long lLeftXPosition, long lRightXPosition
		, long lStartIndex, double dCandleWidth, CRect rctGraphRegion);
	void SetpChartModule(IChartManager *pIChartManager, IChartOCX *pIChartOCX, IPacketManager *pIPacketManager);
	void SetPeriod(long p_lData);
	void SetOnlyDealSignal(long p_lData);
	void ClearData();
	BOOL SetCopyData();
	BOOL CheckRawData();
	BOOL SetHelpMessage();

	//�ֹ�//
	void MakeOrderData_FutureOption(CStringArray& arrOrder, STORDER_INDATA* pOrderData, float fOrderPrice);
	void MakeOrderData_Qty_Future(CStringArray& arrOrder, STORDER_INDATA* pOrderData, PROCEED_ORDER* pProceedOrderData);
	void MakeOrderData_Qty_Stock(CStringArray& arrOrder, STORDER_INDATA* pOrderData, PROCEED_ORDER* pProceedOrderData);
	void MakeOrderData_Stock(CStringArray& arrOrder, STORDER_INDATA* pOrderData, float fOrderPrice);
	//void SetHandleUpdatePeriod(long p_lData) { m_hUpdatePeriod = (HWND)p_lData;}
	//END
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndAssistIndexManager)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, IChartManager *p_pIChartManager, IChartOCX *p_pIChartOCX, IPacketManager *p_pIPacketManager,IGraphInfoManager *	m_pIGraphInfoManager);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWndAssistIndexManager();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWndAssistIndexManager)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	LRESULT OnSTDataManagerMessage( WPARAM wParam, LPARAM lParam);
	LRESULT OnSTController( WPARAM wParam, LPARAM lParam);
	LRESULT OnUpdateSTSfg( WPARAM wParam, LPARAM lParam);		
	DECLARE_MESSAGE_MAP()

// ���������� �����ϱ� ���� ����� �߰��Ѵ�.
// �˻���. WID_060913_A1
public:
	void DestroyStrategyItem();
	// ��Ʈ �ð����� ���� ����
	void	SetChartDivideInfo(int nDivideState, int nUnitPos);	// ��Ʈ �ð����� ����
	int		m_nDivideState;										// ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5)
	int		m_nUnitPos;											// 1(0),5(1),10(2),15(3)

	int LoadST(LPCSTR szSTFile, long pChartInterface, LPCSTR szCode, int nCount, LPCSTR szEndDate, CStringArray &m_stArrayPath,CString szSTName);
	// LoadST : 0�ΰ��� �������� ������ �����϶���,
	//			���̳ʽ����� ����, �÷����� ����

	CStrategyLoader* m_pstrategyLoader;
	CIStrategyItem *m_pStrategyItem;
	CString			m_szMSTName;			// ���� �������� ����������
	CStringArray	m_stArrayPath;
	BOOL			m_bSinHo;//ó���϶� ��ȸ������ ������� �ϹǷ�...
	CIndexItem* m_pIndexItem;
	CSTConfigEnv m_STConfigInfo;
	HWND	m_hSysTabEx;		//@@@@@06.11.20 HWND
	////////////////////Debug/////////////////////
	void WriteDebug(CString szDebug)
	{
		SYSTEMTIME sTime;
		GetSystemTime(&sTime);


		FILE *fp = fopen("C:\\Logs\\����.txt", "a+t");

		if(fp != NULL) 
		{
			fprintf(fp, "[%02d:%02d:%02d]", sTime.wHour+9, sTime.wMinute, sTime.wSecond);
			fprintf(fp, "[%s]\n", szDebug);
			fclose(fp);
		}

	}
	/////////����û�� ���� Project: KHD ////////////////////
	CString m_stCode;
	//������ ���� /////////////
	BOOL m_bChkSon;
	double m_ValSon;//%��...
	BOOL m_bComOperatorSon;
	CString m_stLogoSon;
	COLORREF m_ColorSon;

	//��ǥ ���� ���� /////////
	BOOL m_bChkSooik;
	double m_ValSooik;
	BOOL m_bComOperatorSooik;
	CString m_stLogoSooik;
	COLORREF m_ColorSooik;

	// �ְ���� �϶� ����
	BOOL m_bChkGoga;
	double m_ValGoga;
	BOOL m_bComOperatorGoga;
	CString m_stLogoGoga;
	COLORREF m_ColorGoga;

	////����������� �϶� ����///
	//BOOL m_bChkJuga;
	//double m_ValJuga;
	//BOOL m_bComOperatorJuga;
	//CString m_stLogoJuga;
	//COLORREF m_ColorJuga;

	// ��ǥ������û��
	BOOL m_bChkTarget;
	double m_ValTarget;
	CString m_stLogoTarget;
	COLORREF m_ColorTarget;

	// ���ͺ���û��
	BOOL m_bChkBenefit;
	double m_ValBenefit;
	CString m_stLogoBenefit;
	COLORREF m_ColorBenefit;

	//�ּҰ��� ��ȭ
	BOOL m_bChkMin;
	int m_nBong;
	double m_ValMin;
	BOOL m_bComOperatorMin;

	//����û�� ����
	BOOL m_bChkToday;
	BOOL m_bTime;
	CString m_stTodayTime;

	//��뼳��////////////////////////
	int m_ApplyPosition;
	void SetRunSetUpExit(CString stPath);
	CString GetOptionLogo(int nSel);

public:
	// �������� �������
	void	ReadConfiguration(CString stPath);					// ������ ������ ������ ����
	void	ReadConfig_ForcedLiquidation(CString szPath);		// ����û��(Forced Liquidation) ������ ����
	void	ReadConfig_Setup(CString szPath);					// ���(Setup) ������ ����
	void	ReadConfig_Order(CString szPath);					// �ֹ�(Order) ������ ����
	CString m_szSTName;
	// �������� �ʱ�ȭ
	void	InitStrategySignalInfo(CDealDataItem* pDealDataItem);

	BOOL	IsBuy(long lDealType);						// �ż� ��뿩��
	BOOL	IsExitShort(long lDealType);				// �ŵ�û�� ��뿩��
	BOOL	IsSell(long lDealType);						// �ŵ� ��뿩��
	BOOL	IsExitLong(long lDealType);					// �ż�û�� ��뿩��
	BOOL	IsBuyOrExitShort(long lDealType);			// �ż� / �ŵ�û�� ��뿩��
	BOOL	IsSellOrExitLong(long lDealType);			// �ŵ� / �ż�û�� ��뿩��

	BOOL	IsLossRate(long lDealType);					// ������ ��뿩��
	BOOL	IsTargetBenefit(long lDealType);			// ��ǥ���� ��뿩��
	BOOL	IsHPriceCPBear(long lDealType);				// �ְ�����϶� ��뿩��
	//BOOL	IsLPriceCPBull(long lDealType);				// ��������� ��뿩��
	BOOL	IsTargetValuePreserve(long lDealType);		// ��ǥ������û�� ��뿩��
	BOOL	IsBenefitPreserve(long lDealType);			// ���ͺ���û�� ��뿩��
	BOOL	IsMiniPrice(long lDealType);				// �ּҰ��� ��뿩��
	BOOL	IsDayLiquidation(long lDealType);			// ����û�� ��뿩��

	// ��ȣ���� Draw
	void	DrawSignal_BuyOrExitShort(CDC* pDC, long lDealType, int nXPos, int nYPos, CString strDealName);		// �ż� / �ŵ�û�� ��ȣ �׸���
	void	DrawSignal_SellOrExitLong(CDC* pDC, long lDealType, int nXPos, int nYPos, CString strDealName);		// �ŵ� / �ż�û�� ��ȣ �׸���
	void	DrawSignal_ForcedLiquidation(CDC* pDC, int nXPos, int nYPos, CString strLogoName, COLORREF clrLogo);	// ����û�� ��ȣ �׸���

	//////////////////////////////////////////////////////////////////////////
	// vntsorl �߰�
	BOOL	IsEntryPosition(long lDealType);			// ���� ������
	BOOL	IsExitPosition(long lDealType);				// û�� ������
	BOOL	IsForcedLiquidation(long lDealType);		// ����û�� ������ (�ż�/�ŵ�)
	BOOL	IsBuyForcedLiquidation(long lDealType);		// �ż����Կ� ���� ����û�� ������
	BOOL	IsSellForcedLiquidation(long lDealType);	// �ŵ����Կ� ���� ����û�� ������
	BOOL	IsBuyLossRate(long lDealType);
	BOOL	IsBuyTargetBenefit(long lDealType);
	BOOL	IsBuyHPriceCPBear(long lDealType);
	BOOL	IsBuyTargetValuePreserve(long lDealType);
	BOOL	IsBuyBenefitPreserve(long lDealType);
	BOOL	IsBuyMiniPrice(long lDealType);
	BOOL	IsBuyDayLiquidation(long lDealType);
	BOOL	IsSellLossRate(long lDealType);
	BOOL	IsSellTargetBenefit(long lDealType);
	BOOL	IsSellHPriceCPBear(long lDealType);
	BOOL	IsSellTargetValuePreserve(long lDealType);
	BOOL	IsSellBenefitPreserve(long lDealType);
	BOOL	IsSellMiniPrice(long lDealType);
	BOOL	IsSellDayLiquidation(long lDealType);
	void	DrawSignal_Buy(CDC* pDC, int nXPos, int nYPos, CString strDealName);
	void	DrawSignal_ExitShort(CDC* pDC, int nXPos, int nYPos, CString strDealName);
	void	DrawSignal_Sell(CDC* pDC, int nXPos, int nYPos, CString strDealName);
	void	DrawSignal_ExitLong(CDC* pDC, int nXPos, int nYPos, CString strDealName);

	void	DrawSignal_EntryPosition(CDC* pDC, long lDealType, int nXPos, int nYPos, CString strDealName);
	void	DrawSignal_ExitPosition(CDC* pDC, long lDealType, int nXPos, int nYPos, CString strDealName);
	void	DrawSignal_BuyForcedLiquidation(CDC* pDC, long lDealType, int nXPos, int nYPos);
	void	DrawSignal_SellForcedLiquidation(CDC* pDC, long lDealType, int nXPos, int nYPos);


	//////////////////////////////////////////////////////////////////////////
	
	// ����û�� ���ú���
	int		m_nTiming_ForcedLiquidation;	// ����û�� ���� (Real�� ��츸 ���)

	// ��� ���ú���
	int		m_nApplyPosition;				// ��� ������

	// �ֹ� ���ú���
	int		m_nTiming_Order;				// �ֹ����� (Real�� ��츸 ���)
	BOOL	m_bUseEntrySound;				// ��ȣ�߻���(���Խ�ȣ) ��뿩��
	CString	m_szEntrySound;					// ��ȣ�߻���(���Խ�ȣ)
	BOOL	m_bUseExitSound;				// ��ȣ�߻���(û���ȣ) ��뿩��
	CString	m_szExitSound;					// ��ȣ�߻���(û���ȣ)
	int		m_nOrderIndex;					// �ֹ��߻��� �ε��� ����

// 2008.04.04 by LYH >> �Ѻ����� �ֹ� �ѹ� ������ �ٽ� �ֹ� �ȳ����� ���� ����(���Ǹ���, �ż�/�ŵ����� �ٸ���)
	BOOL	IsCheckPosition(CDealData* pDealData);
	int		m_nOrderType;					// �ֹ��߻��� �ε��� ����
// 2008.04.04 by LYH <<
	CCriticalSection m_CriticalSection;

	//�ֹ����� ����ü
	STORDER_INDATA* m_pstOrderData;
	int m_nJinValue,m_nChungValue;
	BOOL m_bJinMarket, m_bChungMarket;
	void SetSiteCUstomAddin(LONG Addin);
	// ��������
	void STBackup(LPCSTR szMSTName);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);

	// ��ǥ,�����༼�� ���࿩�θ� �����Ѵ�.
	BOOL STItem_IsRun(LPCSTR p_szRunSTFile, LPCSTR p_szSTName);

	CString	m_strRQ;		// ���ǰ� �ִ� RQ :�������� - ojtaso (20070307)
	void	SetRQ(LPCTSTR lpszRQ) {m_strRQ = lpszRQ; }
	LPCTSTR GetRQ() { return m_strRQ; };
};

// --------------------------------------------------
// �۾�ID [WID_060913_A1]
// 

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDASSISTINDEXMANAGER_H__A82F4907_B365_480A_AA4C_C75327E251DA__INCLUDED_)
