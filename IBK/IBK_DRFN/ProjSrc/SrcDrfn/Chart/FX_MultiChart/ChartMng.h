#if !defined(AFX_CHARTMNG_H__FD135639_7D01_492D_BFC5_AF011DA6BD62__INCLUDED_)
#define AFX_CHARTMNG_H__FD135639_7D01_492D_BFC5_AF011DA6BD62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChartMng.h : header file
//
#include <afxtempl.h>
#include "./control/XListCtrl.h"

#ifndef __HANHWA_CHART_REPLAY_INFO_H
	#include "../chart_common/ReplayChartInfoDef.h"
#endif

//<<20100308_JS.Kim �ַθ�
#include "../COMMONTR/OAQ69901_io.h"
//>>

//#include "MFECToolTip.h"

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


class CCodeDataFromInputBar;
class CFloatingHogaDlg;
class CChartMng : public CWnd
{

//20080914 �̹��� >>
public:	

	void SaveChartContainerToImg();

	void PrintChartContainer();

	void Paint(CDC * pDC, BOOL bPrint=FALSE);
	
	BOOL GetDefaultOption(CString& szPaperSize, CString& szWay, CString& szOrientation);

	BOOL SetDefaultOption(CString szPaperSize, CString szWay, CString szOrientation);

	BOOL SetDefaultPrinter(CString printer, CString driver, CString port);

//20080914 �̹��� <<

public:
	BOOL CheckDataTypeForReplay();
	//>>>>>>>------------------------------------------------------------------------
	//Author	:	Sang Yun Nam
	//Date		:	2005.11.24
	//Reason	:   �ܺ����ٷ� ���� �ʱ�ȭ ��ư�� ������� TR�� ����ȸ �ϴµ�..
	//				�̶� �߰�����ϰ�� ���ο� ��Ʈ�� �߰��Ǵ� ��찡 �߻��Ͽ� 
	//				�̸� �÷��׷� �����ϱ� ���� �߰��Ѵ�.
private:
	BOOL m_bInitFromExtToolBar;
public:
	void SetBInitFromExtToolBar(BOOL bInit)	{ m_bInitFromExtToolBar = bInit;}
	BOOL GetBInitFromExtToolBar()			{ return m_bInitFromExtToolBar;	}
	//<<<<<<<<<----------------------------------------------------------------------

//Start ------------------------------------------------------------------------
	//���� �ڵ� �Է¶��� ��Ŀ���� ������ �ϱ� ���� �Ʒ��� ���� ������� �߰��Ѵ�.
	// m_bNoExistLoadCnts �� ȭ���� ó�� �ε�� ��� 
	// ����� ���Ͽ��� ���� ������ ���� ���� ��� 
	//
	//Author	:	Sang Yun Nam
	//Date		:	2005.11.15
private:
	BOOL m_bNoExistLoadCnts ; // ���� �ڵ忡 ��Ŀ���� �������� �߰��� ����
public:
	BOOL GetBNoExistLoadCnts()				{ return m_bNoExistLoadCnts	   ; }
	void SetBNoExistLoadCnts(BOOL bNoExist) { m_bNoExistLoadCnts = bNoExist; }
//End ----------------------------------------------------------------------
public :
	//BOOL MarketTRSend_By_OCXSignal(CString strCode, UINT nID); //���� ��ǥ TR ���� ����..
	BOOL MarketTRSend_By_OCXSignal(int nID, const LPCTSTR pcszPacketNames, int nCount, const BOOL bIsNew); //���� ��ǥ TR ���� �غ�
	BOOL RequestChart_MarketData(CString strTRCode, CString strJipyoName,  CChartItem* pSelChartItem, int nDataRqst, BOOL bIsNew); //���� ��ǥ TR send
//	BOOL RequestChart_MarketDataMore(CChartItem* pSelChartItem); //���� ��ȸ


	BOOL m_bMarketJipyoAdding; //��ǥ�ǿ��� ������ǥ �׸��� ����Ŭ�� �Ͽ� ���� ���..TRUE�� ���� 

	BOOL m_bAddingMarketData;// ������ǥ�߰�
	BOOL m_bNeedForceCode;	 // ������ �� ���� ��� ������ �����ڵ带 �����Ͽ� ������.
	void SetReplayRealTitle(CString strCode, CString strSpeed, TICK_DATA* pData);
	void SetReplayChangedDate(CString strCode, CString strBojoMsg); //���� ������ ó���ϴ� ���� ��¥�� �ٲ��� ChartOCX�� �˷��ش�.

private:
	BOOL m_bReplaychartMode;
public:
	BOOL GetBReplayChartMode() { return m_bReplaychartMode; }
	void SetBReplayChartMode(BOOL bReplayMode);// { m_bReplaychartMode = bReplayMode;	}
	void SetBTypeLink(BOOL bTypeSync)		   { m_bTypeLink = bTypeSync;			}	// �ð� �������� ����.
	
private:
	// Start =====================================================================================
	// ƽ/��/��/��/��/��/�� �� ������ �Ǵ� ��Ʈ�� �� Cwnd* �� �����Ѵ�.
	BOOL	m_bExistSyncChart; // ������ �� ����� ��ǥ ������ ���� �ϴ��� ����.
public:
	CWnd* m_pTickCfgChart;
	CWnd* m_pSecCfgChart;
	CWnd* m_pMinCfgChart;
	CWnd* m_pDayCfgChart;
	CWnd* m_pHourCfgChart;
	CWnd* m_pWeekCfgChart;
	CWnd* m_pMonthCfgChart;
	CWnd* m_pYearCfgChart;
	

	BOOL GetbExistSyncChart()			{ return m_bExistSyncChart	;}
	BOOL SetbExistSyncChart(BOOL bExist){ m_bExistSyncChart = bExist;}


	CChartItem* GetpChartCfgWnd(char chType);
	void SetpChartCfgWnd(char chType, CWnd* pWnd);
	
	// ���񺯰� �� �ֱ⺯���...
	void Synchro_jipyo();
	
	// End =======================================================================================

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


	//2005. 08. 25 add by sy, nam ===================================================
	//�ϳ��� ��Ʈ�� ���ϰ�� ȣ��â�� ���δ�.
	CRect*			m_pRectFloatingHoga;
	CFloatingHogaDlg *m_pFloatingDlg;
	void CreateFloatingHogaWnd(BOOL bForce = FALSE);
	void DestroyFloatingHogaWnd();
	//===============================================================================

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
//	bool	GetShowCurrentPriceToOutside	(bool& bShow				);	// ��Ʈ�� ���簡 ǥ��



	void OnChartEnvSetupOK();
	void SetAllChartCfgSync();		//ȯ�浿��ȭ ����
	//===============================================================================




	//================================================================================
	// ���� ��Ƽ��Ʈ ���� ����� ��� ����������  Mode���� ����ȴ�.	
	// 2005. 08. 11
	//CHANGE_CHART			0		//��ȯ���
	//ADD_NEW_CHART			1		//�߰����
	//REPLAY_CHART			2		//������
	//OVERLAPP_CHART		3		//��ø���
	void SetMode(int nMode) {m_nMode = nMode;}
	int  GetMode()			{return m_nMode; } //2005. 08. 11
	//================================================================================


	//2005. 08.02  ��� ��Ʈ�� ���� ���� -----------------------------
	void OnAllCodeUnAdvise();

	//2005. 08.02 ��� ��Ʈ OCX�� ���� ����Ʈ Get --------------------
	void GetChartAllCode(CStringArray &arrCode);

	//2005. 06.24  DBMgr Set -----------------------------
	//CDrMLib_CDBMgr*	m_pDBMgr;
	//void				SetpDBMgr(CDrMLib_CDBMgr* pDBMgr){ m_pDBMgr = pDBMgr;}	
	//CDrMLib_CDBMgr*	GetpDBMgr()				{return m_pDBMgr;	 }
	//--------------------------------------------------------------------------

	//2005. 06.03 CStdDialog�� �θ�Ŭ���� ���� -----------------------------
	long m_lMltchrtDlgParentClsPointer; 
	void SetParentMultiChartClassPointer(long clsPointer);
	//--------------------------------------------------------------------------

	//2005. 05.24 public���� ����.
	CChartItem* GetpChartItemSelected();	

	//2005.05.16 add by Nam ----------------------------------------------------
	//��Ʈ ���� �ֽ�, �����ɼ�, ���� ��Ʈ 
	int		m_nMarketType;
	void	SetMarketType(int nMarketType);
	void	ReplaceOldCode();
	BOOL	m_bOnEventMessage;
	//--------------------------------------------------------------------------

	CString m_strRootPath;
	// 2005. 04. 25 -- Sang Yun, Nam ==== Get ��ǥ, ��Ʈ ����Ʈ 
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
	
	
	
	//2005. 08. 26 BOOL bLbuttonDown  �߰� 
	//���콺�� ���ý� �߻��ϴ� �̺�Ʈ �� ���ؼ��� ó���Ϸ��� ������.
	// sy, nam
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
	//void MoveCharts();
	void MoveCharts(int nRowIndexOutside= -1,int nColIndexOutside = -1); // parameter is added to move one chart gm0604 2005.10.10
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

	// gm0604 added to Update System Trading Data (2005.7.21)
	BOOL OnSTDataManagerMessaggeAll();
	// gm0604 added 2005/10/06
	void SetSystemTradingMode(BOOL bEnable, LPCTSTR lpMessage = NULL);		// gm0604 added 2005/8/23

	void SetTimeMark(LPCTSTR p_szMarkTime, BOOL p_bWithScroll);

	//�������� �ҷ����� �� �ڵ� ������. ���� ���� �ڵ� �����ϰ� �� �ڵ�� ��ü
	CString  m_strCodeForLoadChart;
	long ResetChart();
	void ResetOCX();
private:
	// gm0604 added to Update System Trading Data (2005.10.06)
	BOOL m_bIsSystemTradingMode;
	// gm0604 added to Update System Trading Data (2005.10.06)
	void GetJongmokCodes(CStringList *plistStrJongmokCode,CHighlightChartInfoList* plistHighlightChartInfo);
	void SendJongmokCodesToLeftBar();
	BOOL ReplaceChart(long lRow1,long lCol1, long lRow2, long lCol2);
	CString GetIndicatorNameList(LPCTSTR p_strGroup);
	BOOL SetMatrix(long lRows,long lCols);
//	long ResetChart();
//	void ResetOCX();
	BOOL SetType(char chType, long lNumber);
	
	BOOL InputCodeFromInputBar(CCodeDataFromInputBar* pCodeData,BOOL bForcedMode0 = FALSE);
	BOOL AddMarketData(CString strCode, BOOL bFromTree = FALSE);
	BOOL ShowScale();
	BOOL ShowInnerToolbar();
	BOOL UpdateRevisedValue(BOOL bUseRevised,BOOL bDontRequestData);

	//05.06.22 add by sy, nam
	BOOL UpdateNumericChartValue(int nNumericChartValue, BOOL bDontRequestData);

	BOOL UpdateShowingFackedCandle(BOOL bShowFackedCandle, BOOL bDontRequestData);
	BOOL UpdateIndex();
	BOOL SetJCodesNext();
	void PaintTitle(int nRowIndex,int nColIndex,CChartItem* pChartItem,CDC *pDC);
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
	BOOL m_bIndexLink;				//20100212_JS.Kim Ȯ�� ���¿��� ��ǥ����ȭ ó��
	CString m_strUserPath;
	CString m_strImagePath;
	BOOL m_bNextQuery;
	BOOL m_bHideScale;
	BOOL m_bHideInnerToolBar;
	BOOL m_bOrderLink;				// �ֹ�����

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
	BOOL m_bSaveJipyoOneFile;

	// �м��� ���� ���� - ojtaso (20080723)
	BOOL m_bSaveAnalTool;

	// 2011.01.12 by SYS >> ����Ʈ YYYY(��) �߰� �� �м��� ���� ��� �߰�
	BOOL m_bShareAnalTool;
	// 2011.01.12 by SYS

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

	// �м����� �޴��߰�
	CArray<int, int>	m_arrayAnalIdx;			//�м���������Ʈ �ε���
	CArray<int, int>	m_arrayAnalCheck;		//�м���������Ʈ üũ����
	CArray<int, int>	m_arrayAdditionIdx;		//�ΰ���ɸ���Ʈ �ε���
	CArray<int, int>	m_arrayAdditionCheck;	//�ΰ���ɸ���Ʈ üũ����
	CArray<int, int>	m_arrayUserIdx;			//����ڼ�������Ʈ �ε���
	CArray<int, int>	m_arrayUserCheck;		//����ڼ�������Ʈ üũ����
	CMap<int, int, CString, CString>	m_mapToolName;			//�м���������Ʈ �̸�

	void MakePlatformMenu(HMENU hMenu);
	BOOL GetDlgToolBtnToggle(CArray<int, int>& arAnal, CArray<int, int>& arAnalCheck, CArray<int, int>& arAddition, CArray<int, int>& arAdditionCheck, CArray<int, int>& arUser, CArray<int, int>& arUserCheck, CMap<int, int, CString, CString>& mapToolName);
	void SetDlgToolBtnToggle(int nToolBtnID, BOOL bToggle = TRUE);

	// ���� �޴��߰�
	void AddMainMenuToPlatformMenu(HMENU hMenu);
	int GetPositionInMenu(CString strMenu, CMenu* pMenu);

// Operations
public:
	BOOL GetSaveJipyoOneFile(){return m_bSaveJipyoOneFile;};
	
	// �м��� ���� ���� - ojtaso (20080723)
	BOOL GetSaveAnalTool(){return m_bSaveAnalTool;};

	// 2011.01.12 by SYS >> ����Ʈ YYYY(��) �߰� �� �м��� ���� ��� �߰�
	BOOL GetShareAnalTool(){return m_bShareAnalTool;};
	// 2011.01.12 by SYS

	BOOL GetExistSyncChart(){return m_bExistSyncChart;};

	// ����� ��� : �������� - ojtaso (20080213)
	CString		GetJongMokNameByCode(LPCSTR lpszItemCode, LPCTSTR lpszTRCode = NULL);
	// �����ڵ�� RQ�� ã�� ���� : �������� - ojtaso (20070705)
	BOOL		FindSelectChartByItemCode(LPCTSTR lpszItemCode, BOOL bSelect = TRUE);

	BOOL		IsAbleToAdd() { return (!m_bChangeTypeMode || m_bResetChartAll); }
// --> [Edit]  �̵��� 2008/09/11
// �ֹ� ���� 
	CMapStringToOb	m_mapOrder;
	BOOL		SetOrderData(char *pData);
	BOOL		GetOrderLink() { return m_bOrderLink; };

	BOOL		SetRealOrderInfo(ORDER_INFO* pOrderInfo, int nRealType);

	CString		GetCurrentCode();
// <-- [Edit]  �̵��� 2008/09/11
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
	LRESULT OnMagnifyingGlass( WPARAM wParam, LPARAM lParam);
	LRESULT OnAddPlatformMenu( WPARAM wParam, LPARAM lParam);
//	LRESULT OnCheckPlatformMenu( WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
		
	CUIntArray	m_menuMain, menuChart;

// (2008/9/14 - Seung-Won, Bae) for Pair Tick
public:
	void	RequestChartPairTickData( const char *p_szCode, BOOL p_bWithRequest = TRUE);

	//>> ����ó�� - vntsorl (20090106)
	BOOL	m_bInitChartItem;
	//<< ����ó�� - vntsorl (20090106)

	//<<20100308_JS.Kim �ַθ�
	CString		Format32String(CString strPrice, int scale, int outdec, int indec, UINT uMask);
	//>>
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTMNG_H__FD135639_7D01_492D_BFC5_AF011DA6BD62__INCLUDED_)
