#if !defined(AFX_CHARTMNG_H__FD135639_7D01_492D_BFC5_AF011DA6BD62__INCLUDED_)
#define AFX_CHARTMNG_H__FD135639_7D01_492D_BFC5_AF011DA6BD62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChartMng.h : header file
//
#include <afxtempl.h>
#include "./control/XListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CChartMng window
#define CNTOFMAXROW			(20)
#define CNTOFMAXCOL			(20)
#define	RESIZEBOTH			(0)
#define	RESIZEROW			(1)
#define	RESIZECOL			(2)

#define HEIGHTOFCAPTION		(18)
#define GAPOFCHARTS			(1)


#include "ChartItem.h"

// �������� ��Ʈ ���� : �������� - ojtaso (20070122)
#define UWM_SELECTEDCHART			( WM_USER + 0x2D46)
// ���������� Total Count�� ��Ƽ��Ʈ�� �˷��� : �������� - ojtaso (20070704)
#define UWM_MULTITOTALCOUNT			( WM_USER + 0x2D47)

class CCodeDataFromInputBar;
class CFloatingHogaDlg;
class CChartMng : public CWnd
{
public:
	CShareInputData	*m_pShareInputData;

private:
	BOOL m_bInitFromExtToolBar;
public:
	void SetBInitFromExtToolBar(BOOL bInit)	{ m_bInitFromExtToolBar = bInit;}
	BOOL GetBInitFromExtToolBar()			{ return m_bInitFromExtToolBar;	}

private:
	BOOL m_bNoExistLoadCnts ; // ���� �ڵ忡 ��Ŀ���� �������� �߰��� ����
public:
	BOOL GetBNoExistLoadCnts()				{ return m_bNoExistLoadCnts	   ; }
	void SetBNoExistLoadCnts(BOOL bNoExist) { m_bNoExistLoadCnts = bNoExist; }

public :
	BOOL m_bNeedForceCode;	 // ������ �� ���� ��� ������ �����ڵ带 �����Ͽ� ������.

private:
	//==========================================================================================
	// ȯ�浿��ȭ ��۷� �����Ǿ� �ִ� ���
	// �űԷ� �����͸� ��ȸ�Ҷ�..������ �ð������� ������
	// ��Ʈ�� ã�Ƽ� �ش� ������ �������� ����ȭ �Ѵ�.
	//==========================================================================================

	BOOL	m_bEmptyDataChart;
	BOOL	m_bEmptyChartForSinkCfg;	
public:
	BOOL GetbEmptyDataChart()			{ return m_bEmptyDataChart	;}
	BOOL SetbEmptyDataChart(BOOL bEmpty){ m_bEmptyDataChart = bEmpty;}
	void ApplySyncEnv_OnlyOne()	;
	//==========================================================================================
public:
	//===========================================================================================
	// 05. 09. 13 ����� �ڵ� ����� ��Ʈ �ε�� ���� ����� �׸��� 0�� 0���� ���� 
	// ������ ������ �������� ��ü�ϱ� ���ؼ�
	// Static���� ���� ���� Count�� ��� 1��° �� �� �����Ѵ�.
	static int		m_nStatic_RecvCntShareCode;
	static CString  m_strStatic_ShareCode;
	//===========================================================================================
	static CString  m_strCodeForMulti;


	//===========================================================================================
	// 05. 09. 12 �Ǻ� ��� �ϳ��� ������� �Ѵ�.
	BOOL UpdateShowingFacked_JustOneChart(BOOL bNormalizedMinuteTimeRule,BOOL bDontRequestData);
	//===========================================================================================

	//2005. 08. 25 add by sy, nam ===================================================
	void GetStartEndDateAtChartMng(double& dStartDate, double& dEndDate);
	//===============================================================================

	enum	{STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, ELW_CHART, FOREIGN_CHART, COMMODITY_CHART};	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
	//===============================================================================
	//��Ʈ ȯ�� ����
	void	SetTimeShareOnlyOne				(BOOL bUseage				);	//�ð�����
	void	SetEnableBoundMarkOnlyOne		(BOOL bUseage				);	//������ǥ��
	void	SetShowExRightOnlyOne			(BOOL bUseage				);	//�� ǥ��
	void	SetPriceLog						(BOOL bLog					);	//�α� ǥ��
	void	SetAutoSaveToOutside			(const bool bAutoSave		);	// ��Ʈ����� �ڵ� ���� ����	
//	void	SetShowCurrentPriceToOutside	(const bool bShow			);	// ��Ʈ�� ���簡 ǥ��
	
	bool	GetImaginaryCandleToOutside		(bool& bImaginaryCandle		);	// ���������.
	bool	GetShowHighLowBoundToOutside	(bool& bShow				);	// ��/���Ѱ� ǥ��
	bool	GetShowExRightDividendToOutside	(bool& bShow				);	// �Ǹ���/���� ǥ��
	bool	GetTimeLinkageToOutside			(bool& bTimeLinkage			);	// �ð����� ����
	bool	GetAutoSaveToOutside			(bool& bAutoSave			);	// ��Ʈ����� �ڵ� ���� ����


	void SetMode(int nMode) {m_nMode = nMode;}
	int  GetMode()			{return m_nMode; } //2005. 08. 11
	//================================================================================


	//2005. 08.02  ��� ��Ʈ�� ���� ���� -----------------------------
	void OnAllCodeUnAdvise();

	//2005. 08.02 ��� ��Ʈ OCX�� ���� ����Ʈ Get --------------------
	void GetChartAllCode(CStringArray &arrCode);

	//2005. 06.03 CStdDialog�� �θ�Ŭ���� ���� -----------------------------
	long m_lMltchrtDlgParentClsPointer; 
	void SetParentCompareChartClassPointer(long clsPointer);
	//--------------------------------------------------------------------------

	//2005. 05.24 public���� ����.
	CChartItem* GetpChartItemSelected();	

	//��Ʈ ���� �ֽ�, �����ɼ�, ���� ��Ʈ 
	int		m_nMarketType;
	void	SetMarketType(int nMarketType);
	//--------------------------------------------------------------------------

	CString m_strRootPath;
	void GetIndicatorNameListIntoComboBox(CComboBox& rComboBox, LPCTSTR p_strGroup);

	CChartMng();
	void SetBaseDesk(long lKey);
    BOOL Create(const RECT& rect, CWnd *pParentWnd, LPCTSTR lpUserPath, LPCTSTR m_szImagePath, UINT nID, long clsPointer);
	void SetpInputBarDlg(CWnd *pWnd) { m_pInputBarDlg = pWnd;}
	void SetpExtInputBarDlg(CWnd *pWnd) { m_pExtInputBarDlg = pWnd;}
	void SetpLeftBarDlg(CWnd *pWnd) { m_pLeftBarDlg = pWnd;}
	void GetIndicatorNameListIntoListCtrl(CXListCtrl& rxlistCtrl, LPCTSTR p_strGroup);
	void GetIndicatorNameListIntoTreeCtrl(CTreeCtrl& rxtreeCtrl);
	void GetIndicatorNameListIntoArray(CStringArray *pStrArray, LPCTSTR p_strGroup);
	long GetCols() { return m_lCols;}
	long GetRows() { return m_lRows;}
	long GetCntOfChart() { return (m_lCols*m_lRows); }	
	CChartItem* GetChartItem(int row, int col) { return m_pChartItems[row][col]; }	
	CRect GetRectOfChart(int nRowIndex,int nColIndex,int nTitle = 0); // nTitle 0:Chart, 1:Title, 2:Chart+Title
	
	
	//���콺�� ���ý� �߻��ϴ� �̺�Ʈ �� ���ؼ��� ó���Ϸ��� ������.
	BOOL m_bLBDnFromChart;  //��Ʈ �����ۿ��� LButtonDown�� 
	void SendInitChartData(BOOL bNotCodeInputInit = TRUE, BOOL bFromTRData = FALSE); 
	//====================================================================================================

	void SetRealJPriceData(void *pRealData);
	void SetRealUPriceData(char *pData, int nSize);
	void SetRealFPriceData(char *pData, int nSize);
	void SetRealOPriceData(char *pData, int nSize); //�ɼ�		-- �ű� �߰� 05.06.21 by sy,nam
	void SetRealHPriceData(char *pData, int nSize); //�ؿ�����  -- �ű� �߰� 05.06.21 by sy,nam
	

	BOOL SelectNextPos();
	BOOL SelectEmptyPos();
	BOOL InsertChart();
	void MoveCharts();
	//void MoveCharts(int nRowIndexOutside= -1,int nColIndexOutside = -1); // parameter is added to move one chart gm0604 2005.10.10
	//void SetChartSelected(CPoint szNewPos,BOOL bInvalidateEvenIfPosisequal =FALSE);
	void SetChartSelected(CPoint szNewPos,BOOL bInvalidateEvenIfPosisequal =FALSE, BOOL bQueryData=FALSE);
	BOOL IsAdvisingCodeOf(LPCTSTR lpCode);
	void SetMultiCodes(CString strMultiCodes);	
	// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
	void OnChartMngCodeAdvise(CChartItem *pItem, BOOL bAddMod);
	// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
	void OnChartMngCodeUnAdvise(CChartItem *pItem, BOOL bAddMod);
	CWnd *GetpInputBarDlg() { return m_pInputBarDlg;}
	CWnd *GetpExtInputBarDlg() { return m_pExtInputBarDlg;}
	void SetSharedTime(LPCTSTR lpSharedTime) { m_strSharedTime = lpSharedTime;}
	CString GetSharedTime() { return m_strSharedTime;}
	CWnd *GetpLeftBarDlg() { return m_pLeftBarDlg;}
	void UnhighlightJCode(CChartItem *pChartItemIn);
	void SetEndData(LPCSTR szMessage);
	BOOL	IsToBeReceivedMore() 
	{
		if(m_strMultiCodes.IsEmpty())
			return FALSE;
		return TRUE;
	}

	void SetTimeMark(LPCTSTR p_szMarkTime, BOOL p_bWithScroll);

	//�������� �ҷ����� �� �ڵ� ������. ���� ���� �ڵ� �����ϰ� �� �ڵ�� ��ü
	CString  m_strCodeForLoadChart;
private:
	void GetJongmokCodes(CStringList *plistStrJongmokCode,CHighlightChartInfoList* plistHighlightChartInfo);
	void SendJongmokCodesToLeftBar();
	BOOL ReplaceChart(long lRow1,long lCol1, long lRow2, long lCol2);
	CString GetIndicatorNameList(LPCTSTR p_strGroup);
	BOOL SetMatrix(long lRows,long lCols);
	long ResetChart();
	void ResetOCX();
	BOOL SetType(char chType, long lNumber);

	//@080520-004
	void SetType_NoRequest(char chType, long lNumber);

	CCodeDataFromInputBar* m_pCodeData;
	BOOL InputCodeFromInputBar(CCodeDataFromInputBar* pCodeData,BOOL bForcedMode0 = FALSE);
	BOOL AddMarketData(CString strCode, BOOL bFromTree = FALSE);
	BOOL AddUpjongData(CString strCode);
	BOOL ShowScale();
	BOOL ShowInnerToolbar();
	BOOL UpdateRevisedValue(BOOL bUseRevised,BOOL bDontRequestData);

	//05.06.22 add by sy, nam
	BOOL UpdateNumericChartValue(int nNumericChartValue, BOOL bDontRequestData);

	BOOL UpdateShowingFackedCandle(BOOL bShowFackedCandle, BOOL bDontRequestData);
	BOOL UpdateIndex();
	BOOL SetJCodesNext();
	//void PaintTitle(int nRowIndex,int nColIndex,CChartItem* pChartItem,CDC *pDC);
	void PaintTitle(int nRowIndex,int nColIndex,CChartItem* pChartItem,CPaintDC *pDC);
	BOOL Rearrange(long lCnt,int nType);
	
	void ExtendChart(BOOL bExtend = TRUE);
	void LoadChart(CPoint *pPt);
	void SaveChart(CPoint *pPt);
	void LoadSaveChart(CPoint *pPt);
public:
	// �������� �߰� : �������� - ojtaso (20070118)
	BOOL RequestChartData(BOOL bChangeType = FALSE);
	// �������� �ҷ����� : �������� - ojtaso (20070514)
	BOOL RequestLoadedChartData();

	BOOL LoadChart(CString strLoadFile); // 05.09.07 ��Ʈ �ε�� ������ ������ ������ ó���ϱ� ���� Return���� void->BOOL �� �Ѵ�.
	void SaveChart(CString strSaveFile);

	// ���ο� ��Ʈ �߰� : �������� - ojtaso (20070108) 
	void SetAddMode(BOOL bAddMode);
	BOOL GetAddGraphMode() { return (m_bAddMode && !m_bSelChangeChart) || m_bAddModeFromTab; }
	BOOL GetAddBlockMode() { return m_bAddMode || m_bAddModeFromTab; }
	// m_bAddMode : UMSG_CHANGEMODE �� ���� ���õǸ� CHANGE_ITEM�� ��� False
	// m_bSelChangeChart : ��ø�� ���� �ɼ� False (CHANGE_ITEM�� �ǵ��� �� ���ΰ� �ǹ�)

	BOOL GetAddModeFromTab() { return m_bAddModeFromTab; }
	void SetAddModeFromTab(BOOL bAddModeFromTab) { m_bAddModeFromTab = bAddModeFromTab; }

	// �������� �ҷ����� : �������� - ojtaso (20070514)
	void AddRealCodeList(LPCTSTR lpszCode);
	void AddRealCodes();

	// ��Ƽ��Ʈ�� ���ÿ��� : �������� - ojtaso (20070525)
	void SetSelChangeChart(BOOL bSelChangeChart);

private:
	void SetTimeShareAll(BOOL bUseage);
	void SetEnableBoundMarkAll(BOOL bUseage);
	void RequestAVGForGuideLine(BOOL bSelectedOnly);
	void SetSendJongData(LPCTSTR lpQuoteCode,char chType, LPCTSTR lpMinNTickNumber);


// Attributes

public:
	//��Ƽ��Ʈ�� �����Ҷ� ���� ���������� ����Ǿ��� ȯ�������� ����ؼ� �ʱ� ȯ�漳�����ִ� �÷��׿���
	//������ �����ִ� ��ǥ������簡 �ֱ� �Ⱓ���� ����Ǿ��ٰ� �� ���� �ҷ��� �����ϴ°���
	BOOL	m_bLastEnvUpdated;
	BOOL	m_bChangeTypeMode;			// ���ο� ��Ʈ �߰� : �������� - ojtaso (20070108) 
	// m_bChangeTypeMode : CHANGE_ITEM�� �ǵ��� �� ���ΰ� �ǹ�

private:
	long	m_lInitialZoomValue;
	BOOL	m_bUpdatingRevisedValue;
	BOOL	m_bUpdatedInChartExtended;
	BOOL	m_bUpdatingShowingFackedCandle;
	long	m_lKey;				// SetBySetBaseDesk
	CWnd    *m_pInputBarDlg;
	CWnd	*m_pExtInputBarDlg;
	CWnd	*m_pLeftBarDlg;
	UINT	m_nID;	// ID For Chart Component
	COLORREF m_clStartSelected;
	COLORREF m_clEndSelected;
	COLORREF m_clStartUnselected;
	COLORREF m_clEndUnselected;
	BOOL	 m_bChartExtended;
	CFont	m_ftForTitle;
	CFont	m_ftNormal;
	int		m_nMode;	// CHANGE_CHART(0): ����, ADD_NEW_CHART(1):�߰�, OVERLAP_CHART(2):��ø
	int		m_nOldMode;
	/// Chart Matrix Variable & Func Start
	//CChartItem* GetpChartItemSelected();
	CChartItem* m_pChartItems[CNTOFMAXROW][CNTOFMAXCOL];
	long m_lCols;
	long m_lRows;
	long m_lColSelected;
	long m_lRowSelected;
	long m_lColDragFrom;
	long m_lRowDragFrom;
	CString m_strMultiCodes;
	CString m_strSharedTime;
	BOOL m_bClicked;
	BOOL m_bDragNDrop;
	BOOL m_bJongmokLink;			// ���񿬵�
	BOOL m_bTypeLink;				// ��������
	CString m_strUserPath;
	CString m_strImagePath;
	BOOL m_bNextQuery;
	BOOL m_bHideScale;
	BOOL m_bHideInnerToolBar;

	CString m_strTimeMarkReserved;	// TimeMark Setting Resevered
	
	BOOL		m_bNeedToUpdateInLoop;
	CChartItem	*m_pChartItemSelectedInLoop;
	CStringArray m_strArrayCodeInLoop;
	CStringArray m_strArrayTRCodeInLoop;
	long	m_lCntInLoop;
	BOOL	m_bDontRequestDataInLoop;
	BOOL	m_bUseRevisedInLoop;
	//BOOL	m_bUseRateValueInLoop;
	int		m_nChartMngs_ChartDataNumericValue;

	BOOL	m_bNormalizedMinuteTimeRuleInLoop;
	long	m_lZoomValueInLoop;
	CString m_strChartSelectedInLoop;
	char	m_chTypeInLoop;
	long	m_lNumberInLoop;
	long	m_lTickMinNumberInLoop;
	CString m_strEndDateInLoop;
	double	m_dEndDateInLoop;
	CString m_strCodeInLoop;

	long m_lColSelectedInLoop;
	long m_lRowSelectedInLoop;
	CChartItem *m_pChartItemInLoop;
	int m_nRowIndexInLoop;
	int m_nColIndexInLoop;
	long m_lTypeInLoop;
	
	enum {	LOOPTYPE_NULL	 = 0,
			LOOPTYPE_LOAD	, //1
			LOOPTYPE_FIXDATE, //2
			LOOPTYPE_REVISED, //3
			LOOPTYPE_UPDATE	, //4
			LOOPTYPE_UPDATE_BY_EXTEND, //5
			LOOPTYPE_RATE_VALUE,//6
			LOOPTYPE_FAKE_CANDLE = 7,
			LOOPTYPE_MULTI_LOAD = 8
	};
	
	BOOL m_bIsInLoop;
	void InitInLoop();
	BOOL HeadInLoop();
	BOOL TailInLoop();
	BOOL PostInLoop();

	long m_lDragStart;
	long m_lDragEnd;
//	BOOL m_bSaveJipyoOneFile;

	//���ֿ��� ��Ʈ
	BOOL LoadDWMMChart(BOOL bLoad);

	//��Ʈ ��ü �ʱ�ȭ
	void ResetChartAll();
	// ���ο� ��Ʈ �߰� : �������� - ojtaso (20070108) 
	BOOL m_bAddMode;

	// ��Ƽ��Ʈ�� ���ÿ��� : �������� - ojtaso (20070525)
	BOOL m_bSelChangeChart;
	BOOL m_bAddModeFromTab;			// Tab�� �߰����� ���� ����
	
	BOOL m_bResetChartAll;			// ��Ʈ �ʱ�ȭ���� : �������� - ojtaso (20070621)

// Operations
public:
	// ����� ��� : �������� - ojtaso (20080213)
	CString		GetJongMokNameByCode(LPCSTR lpszItemCode, LPCTSTR lpszTRCode = NULL);
	// �����ڵ�� RQ�� ã�� ���� : �������� - ojtaso (20070705)
	BOOL		FindSelectChartByItemCode(LPCTSTR lpszItemCode, BOOL bSelect = TRUE);
	
	// RQ Count����
	int GetRQCount();

	BOOL		IsAbleToAdd() { return (!m_bChangeTypeMode || m_bResetChartAll); }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartMng)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:			
	void SetFocusToJMEdit_At_LoadEnd();
	CString GetCurSelChartItemJMName();
	void MoveFloatingHogaDlg();
	virtual ~CChartMng();
	HBITMAP			m_hBitmap;
	BOOL RevisedPriceFlag();// �����ְ�.

	CString GetUserpath(){return m_strUserPath;};
	BOOL IsInLoop() {return m_bIsInLoop;};

	// Generated message map functions
protected:
	//{{AFX_MSG(CChartMng)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point); 		
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	LRESULT OnEventMessage( WPARAM wParam, LPARAM lParam);
	LRESULT OnGetUserID( WPARAM wParam, LPARAM lParam);
	LRESULT OnRmsgSelChartCode(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
		
public:
	BOOL fnMenuLoad(BOOL bOnlyCheck, int nScreenType);

	//-->@.080728-001 ��ȸ������ ��������.
	void OnAllCodeScdClear();	
	void OnChartMngCodeScdClear(CChartItem *pItem, BOOL bAddMode);
	//<--

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTMNG_H__FD135639_7D01_492D_BFC5_AF011DA6BD62__INCLUDED_)
