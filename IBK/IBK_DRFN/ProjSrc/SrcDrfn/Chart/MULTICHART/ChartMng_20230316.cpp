// ChartMng.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "ChartMng.h"
#include "StdDialog.h"
#include "ChartListDlg.h"
#include "CodeDataFromInputBar.h"
#include "FloatingHogaDlg.h"
#include "./define/ChartMsgDef.h"
#include "./define/DragNDropDefine.h"
#include "./define/MessageDefine.h"
#include "../COMMONTR/ChartRealConverter.h"
#include "../Chart_Common/BCMenu.h"
#include "../chart_common/IRealReceiver.h"
#include "../CommonTR/Data_common.h"
#include "../../inc/IMainInfoMng.h"

// #ifdef __MULTI_LANG__ //@Solomon-100225
	#include "../Include_Chart/Dll_Load/IMetaTable.h"	//@Solomon-MultiLang:091117
// #endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT  RMSG_MAGNIFYING_GLASS = ::RegisterWindowMessage("RMSG_MAGNIFYING_GLASS"); //05.09.30
//const UINT RMSG_CHECKAVGPRICE		=   ::RegisterWindowMessage("RMSG_CHECKAVGPRICE");

//const UINT RMSG_TEST_TR = ::RegisterWindowMessage("RMSG_TEST_TR");

// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
const UINT  RMSG_GETCURRENTMODE = ::RegisterWindowMessage("RMSG_GETCURRENTMODE");

// (2005/2/17 - Ssyng-Won, Bae) Term Analysis Infomation Notify I (Drag Start Position in Time Serial)
//		LParam : Time Value (long) of Mouse Position
#define UWM_DRAGSTART				( WM_USER + 0x2D3C)
// (2005/2/17 - Ssyng-Won, Bae) Term Analysis Infomation Notify II (Drag End Position in Time Serial)
//		LParam : Time Value (long) of Mouse Position
#define UWM_DRAGEND					( WM_USER + 0x2D3D)

#define UWM_ZOOMVALUE			( WM_USER + 0x2D36)
#define UWM_FIRECODESHARE		( WM_USER + 0x2D38)
// (2004.10.08, ��¿�) ���ڼ� �̿���� ��ġ��ȸâ �̿뿩�� ����� Multi-Chart���� �˷�,
//		�ٸ� ������Ʈ���� �� ������ ����ǵ��� �Ѵ�.
//		�̶�, WParam�� ���� ������ ������ ��ġ��ȸâ �̿뿩�� �������̴�.
#define UWM_USEINQUIRY_CROSSLINE	( WM_USER + 0x2D39)

// (2004.10.26, ��¿�) ��Ʈ�� ȣ�� ���� (ȣ��-��Ʈ)�� �߻����� �˸��� Message�̴�.
//		�̶� LParam�� ������ ���� Data String�� Pointer (const char *)�� ���޵ȴ�.
#define UWM_FIREASKINGPRICESHARE	( WM_USER + 0x2D3A)
// (2004.11.04, ��¿�) ��Ʈ�� Global ���� Data �߻��� DLL Container���� �˸��� Message�̴�.
//		�̶� LParam���� "������;����Data"�� ���·� Data�� ���޵ȴ�.
//		�⺻������ Chart�� �ð�����, �Ϲ�ȣ������, Ư��ȣ������(ȭ�麰���ѺҰ�)�� �߻���Ų��.
#define UWM_FIREGSHAREDATA			( WM_USER + 0x2D3B)

// (2005/10/25 - Sun-Young, Kim) ��Ʈ ��ü�ʱ�ȭ
#define UWM_ORGRETURN				( WM_USER + 0x2D40)

// �������� ��Ʈ ���� : �������� - ojtaso (20070122)
#define UWM_SELECTEDCHART			( WM_USER + 0x2D46)
// ���������� Total Count�� ��Ƽ��Ʈ�� �˷��� : �������� - ojtaso (20070704)
#define UWM_MULTITOTALCOUNT			( WM_USER + 0x2D47)

// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
// ADD PSH 20101117 - 20100730 ���μ� �ֱ⺰ ���� ���� �߰� >>
#define UWM_USECYCLESAVE		( WM_USER + 0x2D50)
// END ADD

#define _CHARTMSG_EVENTMESSAGE	"CHARTMSG_EVENTMESSAGE"
const UINT RMSG_CHARTMSG_EVENTMESSAGE	= ::RegisterWindowMessage( _CHARTMSG_EVENTMESSAGE);


#define EACH_CHARTJOB_START()	\
		{	CChartItem *pChartItem = NULL;\
			for(int nRowIndex=0;nRowIndex<m_lRows;nRowIndex++){\
				for(int nColIndex=0;nColIndex<m_lCols;nColIndex++){\
					pChartItem = m_pChartItems[nRowIndex][nColIndex];\
					if(pChartItem){
#define EACH_CHARTJOB_END()		\
		}	}	}	}

#define EACH_CHART_MAKENULL() m_pChartItems[nRowIndex][nColIndex] = NULL;

#define MEMCOPYINTOBUFFER(lpBuf,lpData,lDataSize, lOffset)		memcpy(&lpBuf[lOffset],lpData,lDataSize); lOffset+=lDataSize;


// �޴� �߰�
#define ID_PLM_ADD_MENU					41500		// �޴� ���̵� ����
#define ID_PLM_ADD_MENU_STARTANALTOOL	42000		// �м����� �޴� ���̵� Begin
#define ID_PLM_ADD_MENU_ENDANALTOOL		42199		// �м����� �޴� ���̵� End
#define ID_PLM_ADD_MENU_BEGINMAIN		41700		// ���� �޴� ���̵� Begin
#define ID_PLM_ADD_MENU_ENDMAIN			41719		// ���� �޴� ���̵� End
#define ID_PLM_ADD_MENU_BEGINMAINCONN	41720		// ���ο���ȭ�� �޴� ���̵� Begin
#define ID_PLM_ADD_MENU_ENDMAINCONN		42699		// ���ο���ȭ�� �޴� ���̵� End
#define ID_PLM_ADD_MENU_END				42699		// �޴� ���̵� ��

#define ID_TOOLMENU_BASE				500
#define ID_MAINMENU_BASE				4310		// 4310 ~ 4320
#define ID_MAINMENU_BASE_END			4320		// 4310 ~ 4320
#define ID_MAINCONNMENU_BASE			33000		// 33000 ~ 34000
#define ID_MAINCONNMENU_BASE_END		34000		// 33000 ~ 34000
/////////////////////////////////////////////////////////////////////////////
// CChartMng

int		 CChartMng::m_nStatic_RecvCntShareCode = 0;
CString  CChartMng::m_strStatic_ShareCode = _T(""); //��Ʈ �ε�� ������ ���� ������ �ѹ��� �����ϱ� ���� 
CString  CChartMng::m_strCodeForMulti = _T(""); //��Ʈ �ε�� ������ ���� ������ �ѹ��� �����ϱ� ���� 


CChartMng::CChartMng()
{
	m_bNeedForceCode    = TRUE;
	m_bAddingMarketData = FALSE;
	m_bLastEnvUpdated = FALSE;
	m_lColSelectedInLoop = 0;
	m_lRowSelectedInLoop = 0;
	m_bIsInLoop = FALSE;
	m_lTypeInLoop = LOOPTYPE_NULL/*NULL*/;
	
	m_pChartItemInLoop = NULL;
	m_nRowIndexInLoop = 0;
	m_nColIndexInLoop = 0;

	m_bUpdatingRevisedValue = FALSE;
	m_bHideScale = FALSE;
	m_bHideInnerToolBar = FALSE;
	m_bUpdatedInChartExtended = FALSE;
	m_bUpdatingShowingFackedCandle = FALSE;
	m_bTypeLink = FALSE;
	m_bJongmokLink = FALSE;
	m_bClicked = FALSE;
	m_bDragNDrop = FALSE;
	m_lKey = NULL;
	m_pInputBarDlg = NULL;
	m_pExtInputBarDlg = NULL;
	m_pLeftBarDlg = NULL;
	m_bChartExtended = FALSE;

	//m_clStartSelected =	RGB(255,241,206);//RGB(242,242,242); //RGB(236,241,255);
	m_clStartSelected =	RGB(234, 214, 142);
	m_clStartUnselected = RGB(238,238,238);//RGB(213,213,213);	//RGB(225,227,233);
	m_nMode = CHANGE_ITEM;
	m_nOldMode = -1;
	m_lCols = 0;
	m_lRows = 0;
	m_lColSelected = 0;
	m_lRowSelected = 0;
	m_lColDragFrom = -1;
	m_lRowDragFrom = -1;
	m_lDragStart = 0;
	m_lDragEnd = 0;
	::memset(m_pChartItems,0x00,sizeof(m_pChartItems));	

	m_lMltchrtDlgParentClsPointer = 0;

	m_bIsSystemTradingMode = FALSE;

	m_pFloatingDlg = NULL;
	m_pRectFloatingHoga = NULL;

	m_bLBDnFromChart = FALSE;

	m_bEmptyDataChart		= FALSE;
	m_bEmptyChartForSinkCfg	= FALSE;


	//=======================================================================
	// 2005.10.12
	// ƽ/��/��/��/��/�� �� ��ǥ ����
	//=======================================================================
	m_pTickCfgChart		= NULL;
	m_pSecCfgChart		= NULL;
	m_pMinCfgChart		= NULL;
	m_pDayCfgChart		= NULL;
	m_pHourCfgChart		= NULL;
	m_pWeekCfgChart		= NULL;
	m_pMonthCfgChart	= NULL;
	m_pYearCfgChart		= NULL;

	m_bExistSyncChart	= FALSE;
	//=======================================================================
	
	m_bReplaychartMode  = FALSE;

	m_bMarketJipyoAdding = FALSE;

	m_bNoExistLoadCnts = FALSE;

	m_bInitFromExtToolBar = FALSE; //05.11.24 �ʱ�ȭ ������ ���� �ƴϴ�.

	m_strCodeForLoadChart = "";

	// �������� �߰� : �������� - ojtaso (20070118)
	m_bAddMode = FALSE;
	m_bChangeTypeMode = FALSE;

	// ��Ƽ��Ʈ�� ���ÿ��� : �������� - ojtaso (20070525)
	m_bSelChangeChart = FALSE;
	m_bAddModeFromTab = FALSE;

	// ��Ʈ �ʱ�ȭ���� : �������� - ojtaso (20070621)
	m_bResetChartAll = FALSE;
	m_bOnEventMessage = FALSE;
	
	m_nSTRunState = ST_STNONE;		// �ý���Ʈ���̵� ����������� �ʱ�ȭ
	m_nChartLayout = 0;
	m_nDWMChartIndex = 0;

	m_bFutPrevChart = FALSE;
	m_nHeightOfCaption = 18;

	// (2008/8/10 - Seung-Won, Bae) Support UpdateIndex in Maxized Chart Item.
	m_bUpdateIndexAfterExtended = FALSE;

	m_nAllDayViewCount = -1;

	m_bNoInsertChart = FALSE;

	m_bSaveAnalTool = FALSE;

// #ifdef __MULTI_LANG__ //@Solomon-100225
	m_hOcxWnd = NULL;		//@Solomon-MultiLang:091117
// #endif

	m_nLeftbarReplayState = 0;
}

CChartMng::~CChartMng()
{
}


BEGIN_MESSAGE_MAP(CChartMng, CWnd)
	//{{AFX_MSG_MAP(CChartMng)	
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE( RMSG_CHARTMSG_EVENTMESSAGE,	OnEventMessage)
	ON_REGISTERED_MESSAGE( RMSG_GET_USERID,	OnGetUserID)
	ON_REGISTERED_MESSAGE( RMSG_MAGNIFYING_GLASS, OnMagnifyingGlass)
	ON_REGISTERED_MESSAGE( RMSG_SETQUERYOPT, OnSetQueryOpy)
	ON_REGISTERED_MESSAGE( RMSG_CHANGEVERTSCALETYPE, OnChangeVertScaleType)
	ON_REGISTERED_MESSAGE( RMSG_ADD_PLATFORM_MENU, OnAddPlatformMenu)
	ON_REGISTERED_MESSAGE( RMSG_GETINFO, OnRmgGetInfo)
	ON_REGISTERED_MESSAGE( RMSG_SETINFO, OnRmgSetInfo)
// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
	ON_REGISTERED_MESSAGE( RMSG_GETCURRENTMODE, OnGetCurrentMode)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChartMng message handlers

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	Input	:	lKey	����ũ����� ���� class�� Pointer
//	����	:	����ũ Class�� �����ϰ� Chart���� Setting�Ѵ�.
///////////////////////////////////////////////////////////////////////////////
void CChartMng::SetBaseDesk(long lKey)
{
	BOOL bNoToolBar = FALSE;
	if(m_pExtInputBarDlg == NULL)
	{
#ifdef _DEBUG
		AfxMessageBox("CChartMng::SetBaseDesk m_pExtInputBarDlg ERRROR!");
#endif
		return;
		//VERIFY(m_pExtInputBarDlg);
	}
	m_lKey = lKey;
	VERIFY(m_pExtInputBarDlg);
	BOOL bTimeShareUseage = m_pExtInputBarDlg->SendMessage(UMSG_GETTIMESHARSYSEAGE);
	EACH_CHARTJOB_START()
	//pChartItem->SetInit((long)this->GetSafeHwnd(), m_lKey,bNoToolBar);
	pChartItem->SetInit((long)this->GetSafeHwnd(), m_lKey, m_strRootPath, bNoToolBar);
	if(m_lMltchrtDlgParentClsPointer > 0)
		pChartItem->SetOutsideCommonInterface(m_lMltchrtDlgParentClsPointer);
	pChartItem->EnableReceivingTimeShare(bTimeShareUseage);
	EACH_CHARTJOB_END()
}

BOOL CChartMng::Create(const RECT& rect, CWnd *pParentWnd, LPCTSTR lpUserPath, LPCTSTR lpImagePath, UINT nID, long clsPointer)
{
	m_strUserPath = lpUserPath;
	m_strImagePath = lpImagePath;
	m_nID = nID;
	m_lMltchrtDlgParentClsPointer = clsPointer;
//	if(((CStdDialog*)pParentWnd)->m_nChartType==1)
//	{
//		m_lHeightOfCaption = 0;
//	}

	
	//BOOL bResult = CWnd::Create(NULL,"Chart Mng",WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE|WS_BORDER,rect,pParentWnd,nID);
	BOOL bResult = CWnd::Create(NULL,"Chart Mng",WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE,rect,pParentWnd,nID);
	if(bResult==FALSE)
	{
		AfxMessageBox("ChartMng Create Failed.");
		return bResult;
	}
	//==============================================================================================
	//2005. 06. 23 ���� ���ҵ� ��Ʈ�� ���� 1 * 1 ������ ��� Floating Hoga â�� �����Ѵ�.
	//==============================================================================================
	CreateFloatingHogaWnd();
	//==============================================================================================

	if(((CStdDialog*)GetParent())->m_nChartType < MINI_MULTI_CHART || ((CStdDialog*)GetParent())->m_nScreenID == nScreenID_6301 || ((CStdDialog*)GetParent())->m_nScreenID == nScreenID_6401)
		m_nHeightOfCaption = 18;
	else
		m_nHeightOfCaption = 0;


	// �м��� ���� ���� - ojtaso (20080723)
	if(((CStdDialog*)GetParent())->m_nChartType==SINGLE_CHART)
	{
		m_bSaveJipyoOneFile = m_pInputBarDlg->SendMessage(UMSG_GETSAVEJIPYOONEFILE);
		//m_bSaveAnalTool = !m_bSaveJipyoOneFile;
		m_bSaveAnalTool = m_bSaveJipyoOneFile;
	}
	else if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
	{
		m_bSaveJipyoOneFile = m_pExtInputBarDlg->SendMessage(UMSG_GETSAVEJIPYOONEFILE);
		m_bSaveAnalTool = m_pExtInputBarDlg->SendMessage(UMSG_GETSAVEJIPYOONEFILE, 100);
	}
	
	return bResult;
}


int CChartMng::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	

//	OleInitialize(NULL);

	// TODO: Add your specialized creation code here
	LOGFONT lfont;
	memset(&lfont, 0, sizeof(LOGFONT));       // Zero out the structure.
	lfont.lfWidth = 0; 
	lfont.lfEscapement = 0; 
	lfont.lfOrientation = 0; 
	lfont.lfWeight = FW_NORMAL; 
	lfont.lfItalic = 0; 
	lfont.lfUnderline = 0; 
	lfont.lfStrikeOut = 0; 
	lfont.lfCharSet = DEFAULT_CHARSET; 
	lfont.lfOutPrecision = OUT_DEFAULT_PRECIS; 
	lfont.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	lfont.lfQuality = DEFAULT_QUALITY;
	lfont.lfPitchAndFamily = FF_ROMAN; 
	strcpy(lfont.lfFaceName, "����ü");
	lfont.lfHeight = -12;
	m_ftForTitle.CreateFontIndirect(&lfont);

	lfont.lfWeight	= 500;
	m_ftNormal.CreateFontIndirect(&lfont);
	Rearrange(1,RESIZEBOTH);

//	OleUninitialize();
	SetChartSelected(CPoint(0,0));	

// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CChartMng::OnCreate: Middle\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}

	((CInputBarDlg*)m_pInputBarDlg)->CreatedChartMng();

	m_pStdDialog = (CStdDialog *)(GetParent()->SendMessage(RMSG_GET_STDDLG_PTR));

	return 0;
}

void CChartMng::OnDestroy() 
{
#ifdef _WRITELOG
	((CStdDialog*)GetParent())->WriteToStringLog("CChartMng::OnDestroy::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif

	//Floating Hoga Destroy......
	DestroyFloatingHogaWnd();

#ifdef _DEBUG
		EACH_CHARTJOB_START()
			pChartItem->InvokeAddInSTIndex("ChartSTIndexAddIn.dll",2,0);
		EACH_CHARTJOB_END()		
#endif
	if(m_bIsSystemTradingMode)// clear System strading info. 2005.10.10
	{
		EACH_CHARTJOB_START()
			pChartItem->ClearPtrST();
		EACH_CHARTJOB_END()		
	}

	BOOL bSavedFromExtInputBarGlg = FALSE;
	CString sJipyoFileName1, sJipyoSaveFileName;

	if(m_bReplaychartMode)
		goto ReplayMode;

	
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
	{
		bSavedFromExtInputBarGlg = m_pExtInputBarDlg->SendMessage(UMSG_GETSAVED); //����� ������ ������ ���� 
		if(bSavedFromExtInputBarGlg)//��� ����� Ǭ��.
		{
			//SaveChart(((CStdDialog*)GetParent())->m_strConfig);
			SaveChart(m_pStdDialog->m_strConfig);
		}
	}

	//if(!((CStdDialog*)GetParent())->IsSimpleChart())
	{
		//sJipyoFileName1 = ((CStdDialog*)GetParent())->m_strScreenNo;
		//if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
		//	sJipyoFileName1 += "_Stock";
		//else if(m_nMarketType == FUTOPT_CHART)
		//	sJipyoFileName1 += "_FutOpt";
		//else if(m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
		//	sJipyoFileName1 += "_Upjong";
		//else if(m_nMarketType == FOREIGN_CHART)
		//	sJipyoFileName1 += "_Foreign";	
		//else if(m_nMarketType == ELW_CHART)
		//	sJipyoFileName1 += "_ELW";	

#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("m_pSecCfgChart::SaveandOpenFromFile::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif
		if(m_pSecCfgChart)
		{		
			if(::IsWindow(m_pSecCfgChart->GetSafeHwnd()))
			{
//				if( ( ((CChartItem*)m_pSecCfgChart)->GetType() == SEC10_DATA_CHART ) ||
//					( ((CChartItem*)m_pSecCfgChart)->GetType() == SEC10_DATA_CHART ) )
				if( ( ((CChartItem*)m_pSecCfgChart)->GetType() == SEC_DATA_CHART ))
				{
					//if(m_bSaveJipyoOneFile)
					//	sJipyoSaveFileName = sJipyoFileName1 + "_" + JIPYOSYNC_FILE;
					//else
					//	sJipyoSaveFileName = sJipyoFileName1 + "_Sec_" + JIPYOSYNC_FILE	;
					sJipyoSaveFileName = ((CChartItem*)m_pSecCfgChart)->GetOpenAndSaveFileName();
					((CChartItem*)m_pSecCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
				}
			}
		}
#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("m_pSecCfgChart::SaveandOpenFromFile::End", _T("��Ƽ��Ʈ ����"), 13);
		((CStdDialog*)GetParent())->WriteToStringLog("m_pTickCfgChart::SaveandOpenFromFile::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif
		if(m_pTickCfgChart)
		{		
			if(::IsWindow(m_pTickCfgChart->GetSafeHwnd()))
			{
				if( ((CChartItem*)m_pTickCfgChart)->GetType() == TICK_DATA_CHART ) 
				{
					//if(m_bSaveJipyoOneFile)
					//	sJipyoSaveFileName = sJipyoFileName1 + "_" + JIPYOSYNC_FILE;
					//else
					//	sJipyoSaveFileName = sJipyoFileName1 + "_Tick_" + JIPYOSYNC_FILE	;
					sJipyoSaveFileName = ((CChartItem*)m_pTickCfgChart)->GetOpenAndSaveFileName();
					((CChartItem*)m_pTickCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
				}
			}
		}
#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("m_pTickCfgChart::SaveandOpenFromFile::End", _T("��Ƽ��Ʈ ����"), 13);
		((CStdDialog*)GetParent())->WriteToStringLog("m_pMinCfgChart::SaveandOpenFromFile::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif

		if(m_pMinCfgChart)
		{		
			if(::IsWindow(m_pMinCfgChart->GetSafeHwnd()))
			{
				if( ((CChartItem*)m_pMinCfgChart)->GetType() == MIN_DATA_CHART ) 
				{
					//if(m_bSaveJipyoOneFile)
					//	sJipyoSaveFileName = sJipyoFileName1 + "_" + JIPYOSYNC_FILE;
					//else
					//	sJipyoSaveFileName = sJipyoFileName1 + "_Min_" + JIPYOSYNC_FILE	;
					sJipyoSaveFileName = ((CChartItem*)m_pMinCfgChart)->GetOpenAndSaveFileName();
					((CChartItem*)m_pMinCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
				}
			}
		}
#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("m_pMinCfgChart::SaveandOpenFromFile::End", _T("��Ƽ��Ʈ ����"), 13);
		((CStdDialog*)GetParent())->WriteToStringLog("m_pDayCfgChart::SaveandOpenFromFile::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif

		if(m_pDayCfgChart)
		{				
			if(::IsWindow(m_pDayCfgChart->GetSafeHwnd()))
			{
				if( ((CChartItem*)m_pDayCfgChart)->GetType() == DAY_DATA_CHART ) 
				{
					//if(m_bSaveJipyoOneFile)
					//	sJipyoSaveFileName = sJipyoFileName1 + "_" + JIPYOSYNC_FILE;
					//else
					//	sJipyoSaveFileName = sJipyoFileName1 + "_Day_" + JIPYOSYNC_FILE	;
					sJipyoSaveFileName = ((CChartItem*)m_pDayCfgChart)->GetOpenAndSaveFileName();
					((CChartItem*)m_pDayCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
				}
			}
		}
#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("m_pDayCfgChart::SaveandOpenFromFile::End", _T("��Ƽ��Ʈ ����"), 13);
		((CStdDialog*)GetParent())->WriteToStringLog("m_pHourCfgChart::SaveandOpenFromFile::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif

		if(m_pHourCfgChart)
		{				
			if(::IsWindow(m_pHourCfgChart->GetSafeHwnd()))
			{
				if( ((CChartItem*)m_pHourCfgChart)->GetType() == HOUR_DATA_CHART ) 
				{
					//if(m_bSaveJipyoOneFile)
					//	sJipyoSaveFileName = sJipyoFileName1 + "_" + JIPYOSYNC_FILE;
					//else
					//	sJipyoSaveFileName = sJipyoFileName1 + "_Hour_" + JIPYOSYNC_FILE	;
					sJipyoSaveFileName = ((CChartItem*)m_pHourCfgChart)->GetOpenAndSaveFileName();
					((CChartItem*)m_pHourCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
				}
			}
		}
#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("m_pHourCfgChart::SaveandOpenFromFile::End", _T("��Ƽ��Ʈ ����"), 13);
		((CStdDialog*)GetParent())->WriteToStringLog("m_pWeekCfgChart::SaveandOpenFromFile::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif

		if(m_pWeekCfgChart)
		{		
			if(::IsWindow(m_pWeekCfgChart->GetSafeHwnd()))
			{
				if( ((CChartItem*)m_pWeekCfgChart)->GetType() == WEEK_DATA_CHART ) 
				{
				//	if(m_bSaveJipyoOneFile)
				//		sJipyoSaveFileName = sJipyoFileName1 + "_" + JIPYOSYNC_FILE;
				//	else
				//		sJipyoSaveFileName = sJipyoFileName1 + "_Week_" + JIPYOSYNC_FILE	;
					sJipyoSaveFileName = ((CChartItem*)m_pWeekCfgChart)->GetOpenAndSaveFileName();
					((CChartItem*)m_pWeekCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
				}
			}
		}
#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("m_pWeekCfgChart::SaveandOpenFromFile::End", _T("��Ƽ��Ʈ ����"), 13);
		((CStdDialog*)GetParent())->WriteToStringLog("m_pMonthCfgChart::SaveandOpenFromFile::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif

		if(m_pMonthCfgChart)
		{		
			if(::IsWindow(m_pMonthCfgChart->GetSafeHwnd()))
			{
				if( ((CChartItem*)m_pMonthCfgChart)->GetType() == MONTH_DATA_CHART ) 
				{
					//if(m_bSaveJipyoOneFile)
					//	sJipyoSaveFileName = sJipyoFileName1 + "_" + JIPYOSYNC_FILE;
					//else
					//	sJipyoSaveFileName = sJipyoFileName1 + "_Month_" + JIPYOSYNC_FILE	;
					sJipyoSaveFileName = ((CChartItem*)m_pMonthCfgChart)->GetOpenAndSaveFileName();
					((CChartItem*)m_pMonthCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
				}
			}
		}
#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("m_pMonthCfgChart::SaveandOpenFromFile::End", _T("��Ƽ��Ʈ ����"), 13);
		((CStdDialog*)GetParent())->WriteToStringLog("m_pYearCfgChart::SaveandOpenFromFile::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif

		if(m_pYearCfgChart)
		{		
			if(::IsWindow(m_pYearCfgChart->GetSafeHwnd()))
			{
				if( ((CChartItem*)m_pYearCfgChart)->GetType() == YEAR_DATA_CHART ) 
				{
					//if(m_bSaveJipyoOneFile)
					//	sJipyoSaveFileName = sJipyoFileName1 + "_" + JIPYOSYNC_FILE;
					//else
					//	sJipyoSaveFileName = sJipyoFileName1 + "_Year_" + JIPYOSYNC_FILE	;
					sJipyoSaveFileName = ((CChartItem*)m_pYearCfgChart)->GetOpenAndSaveFileName();
					((CChartItem*)m_pYearCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
				}
			}
		}
	}
#ifdef _WRITELOG
	((CStdDialog*)GetParent())->WriteToStringLog("m_pYearCfgChart::SaveandOpenFromFile::End", _T("��Ƽ��Ʈ ����"), 13);
#endif

ReplayMode:

	m_ftForTitle.DeleteObject();		
	m_ftNormal.DeleteObject();
	EACH_CHARTJOB_START()
		pChartItem->DestroyWindow();
		delete pChartItem;
		EACH_CHART_MAKENULL()
	EACH_CHARTJOB_END()

	m_lCols = 0;
	m_lRows = 0;

	// �м����� �޴��߰�
	m_arrayAnalIdx.RemoveAll();
	m_arrayAdditionIdx.RemoveAll();
	m_arrayUserIdx.RemoveAll();
	m_mapToolName.RemoveAll();

#ifdef _WRITELOG
	((CStdDialog*)GetParent())->WriteToStringLog("CChartMng::OnDestroy::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
	
	CWnd::OnDestroy();
}

void CChartMng::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

//	OutputDebugString("CChartMng Size");

	// TODO: Add your message handler code here
	if(GetSafeHwnd())
	{
		MoveCharts();
	}
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	Input	:	nRowIndex, nColIndex  ��Ʈ�� ��ġ
//				nTitle 0=chart 1=title �׿�=Chart+Title
//	����	:	��Ʈ�� ��ƮŸ��Ʋ�� ��ġ�� �����ش�.
///////////////////////////////////////////////////////////////////////////////
CRect CChartMng::GetRectOfChart(int nRowIndex,int nColIndex, int nTitle /*=0*/)
{
	CRect rectOfChart = CRect(0,0,0,0);
	CRect rectClient;
	GetClientRect(rectClient);
	rectClient.DeflateRect(1,1,1,1);
	BOOL bIsSystemTradingMode = FALSE;
	if(m_pChartItems[nRowIndex][nColIndex])// added by gm0604 2005.10.10
	{
		bIsSystemTradingMode = m_pChartItems[nRowIndex][nColIndex]->GetPtrST();
	}	
	if(!m_bChartExtended)
	{
		if(!m_lCols||!m_lRows)	return rectOfChart;
		int nWidthOfChart =  (rectClient.Width()  - (m_lCols)*GAPOFCHARTS)/m_lCols;
		int nHeightOfChart = (rectClient.Height() - (m_lRows)*GAPOFCHARTS)/m_lRows;

		if(m_nChartLayout == 0)
		{
			if(!nColIndex)
			{
				rectOfChart.left = rectClient.left;
			}
			else
			{
				rectOfChart.left = 1 + nWidthOfChart*nColIndex + nColIndex*GAPOFCHARTS;
			}
			if(nColIndex==(m_lCols-1))
			{
				rectOfChart.right = rectClient.right;
			}
			else
			{
				rectOfChart.right = rectOfChart.left + nWidthOfChart;
			}
			if(!nRowIndex)
			{
				rectOfChart.top = rectClient.top;
			}
			else
			{
				rectOfChart.top = 1 + nHeightOfChart*nRowIndex + nRowIndex*GAPOFCHARTS;
			}
			if(nRowIndex==(m_lRows-1))
			{
				rectOfChart.bottom = rectClient.bottom;
			}
			else
			{
				rectOfChart.bottom = rectOfChart.top + nHeightOfChart;
			}		
		}
		else if(m_nChartLayout == 1)
		{
			if(!nColIndex)
			{
				rectOfChart.left = rectClient.left;
			}
			else
			{
				rectOfChart.left = 1 + nWidthOfChart*nColIndex + nColIndex*GAPOFCHARTS;
			}
			if(nColIndex==(m_lCols-1))
			{
				rectOfChart.right = rectClient.right;
			}
			else
			{
				rectOfChart.right = rectOfChart.left + nWidthOfChart;
			}

			if(!nRowIndex)
			{
				rectOfChart.top = rectClient.top;
			}
			else
			{
				rectOfChart.top = 1 + nHeightOfChart*nRowIndex + nRowIndex*GAPOFCHARTS;
			}
			if(nRowIndex==(m_lRows-1))
			{
				rectOfChart.bottom = rectClient.bottom;

				rectOfChart.left = rectClient.left;
				rectOfChart.right = rectClient.right;
			}
			else
			{
				rectOfChart.bottom = rectOfChart.top + nHeightOfChart;
			}		
		}
		else if(m_nChartLayout == 2)
		{
			if(!nRowIndex && !nColIndex)
			{
				rectOfChart.left = rectClient.left;
				rectOfChart.right = rectClient.right;
			}
			else if(nRowIndex == 1 && !nColIndex)
			{
				rectOfChart.left = 1 + nWidthOfChart + GAPOFCHARTS;
				rectOfChart.right = rectOfChart.left + nWidthOfChart;
			}
			else
			{
				rectOfChart.left = rectClient.left;
				rectOfChart.right = rectOfChart.left + nWidthOfChart;
			}

			if(!nRowIndex && !nColIndex)
			{
				rectOfChart.top = rectClient.top;
			}
			else
			{
				rectOfChart.top = 1 + nHeightOfChart + GAPOFCHARTS;
			}
		
			rectOfChart.bottom = rectOfChart.top + nHeightOfChart;
		}
	}
	else
	{
		if(m_pChartItems[nRowIndex][nColIndex] == GetpChartItemSelected())
		{
			rectOfChart = rectClient;
		}
		else
		{
			return CRect(0,0,0,0);
		}
	}
	if(nTitle==1) // Title
	{
//		rectOfChart.bottom = rectOfChart.top + m_lHeightOfCaption;
		if(bIsSystemTradingMode)
		{
			rectOfChart.bottom = rectOfChart.top + m_nHeightOfCaption*2;
		}
		else
		{
			rectOfChart.bottom = rectOfChart.top + m_nHeightOfCaption;
		}
	}
	else if(nTitle==0) // Chart
	{
//		rectOfChart.top += m_lHeightOfCaption;
//		if(!((CStdDialog*)GetParent())->IsSimpleChart())
		{
			if(bIsSystemTradingMode)
			{
				rectOfChart.top += m_nHeightOfCaption*2;
			}
			else
			{
				rectOfChart.top += m_nHeightOfCaption;
			}
		}

	}
	return rectOfChart;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	Input	:	szNewPos  x, y ��Ʈ�� ��ġ
//	����	:	Ư�� ��Ʈ�� Active�Ѵ�.
///////////////////////////////////////////////////////////////////////////////
void CChartMng::SetChartSelected(CPoint szNewPos, BOOL bInvalidateEvenIfPosisequal /*=FALSE*/, BOOL bQueryData/*=FALSE*/)
{
	//��ǥ �������⸦ �����. ===================================================================
	GetParent()->PostMessage(UMSG_STOP_JIPYOCYCLE, 0, 0);
	//===========================================================================================
	
	CPoint ptOutput(m_lColSelected,m_lRowSelected);
	if(szNewPos.y>=m_lRows||szNewPos.x>=m_lCols
		||((szNewPos.x ==m_lColSelected)&&(szNewPos.y==m_lRowSelected))) 
	{
		if(!bInvalidateEvenIfPosisequal)
		{
			return;
		}
	}
	ptOutput.x = m_lColSelected;
	ptOutput.y = m_lRowSelected;
	m_lRowSelected = szNewPos.y;
	m_lColSelected = szNewPos.x;
	InvalidateRect(GetRectOfChart(ptOutput.y,ptOutput.x,TRUE));
	InvalidateRect(GetRectOfChart(m_lRowSelected,m_lColSelected,TRUE));
	//2007.01.29 by LYH
	if(m_bLastEnvUpdated)
	{
		//@Solomon100218��
		BOOL bDWMchart = ((CStdDialog*)GetParent())->m_nScreenID == nScreenID_6301;
		if(bDWMchart)
			m_pChartItems[0][0]->SetFocus();
		//>>[A00000476]��Ʈ �����޴� ���� ���� ��, Ű���� ����Ű ���� �̻�
		//4���� ��, ���񿬵������� �ϸ�, ���� �޴����� ���콺�� ���� ������ �ϸ� 4���� ��Ʈ�� ������ ����� �Ŀ�, 
		//Focus�� ��Ʈ�� �������� Ű���� ����Ű�� ������ ZoomValue�� �����.
		//else
		//	m_pChartItems[m_lRowSelected][m_lColSelected]->SetFocus();
		//<<
	}
	//}}

	//===============================================================================
//	::SendMessage( GetParent()->GetParent()->GetSafeHwnd(), RMSG_SAVE_FOCUS_CONTROL, 0, 0);
	//===============================================================================

	int nMode = ((CInputBarDlg*)m_pInputBarDlg)->GetCurrentMode();
	if(nMode == CHANGE_ITEM || nMode == REPLAY_ITEM)
	{
		if(bQueryData)
			SendInitChartData(TRUE, TRUE);
		else
			SendInitChartData();
	}
	else
	{
		SendInitChartData();
	}
	
	return;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	Input	:	szNewPos  x, y ��Ʈ�� ��ġ
//	����	:	��ȸ�� ��� Data�� ���ٷ� ������.
///////////////////////////////////////////////////////////////////////////////
void CChartMng::SendInitChartData(BOOL bNotCodeInputInit/*= TRUE*/, BOOL bFromTRData /*= FALSE */)
{
	stChartInitData _stData;
	double dStartDate = 0, dEndDate = 0;
	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();
	if(pChartItem == NULL) return;

	_stData.m_lTotalCnt = pChartItem->GetAllCnt();
	pChartItem->GetStartEndDateOnView(&dStartDate,&dEndDate);
	_stData.m_lStartDate = (long)dStartDate;
	_stData.m_lEndDate = (long)dEndDate;
	if(pChartItem->GetBWholeView())
		_stData.m_lZoomValue = _stData.m_lTotalCnt;
	else	
	{
		if(m_nAllDayViewCount > 0 && !((CExtInputBarDlg*)GetpExtInputBarDlg())->GetQueryOpt())
		{
			pChartItem->SetNew_OnePageDataCount(m_nAllDayViewCount);
			_stData.m_lZoomValue = m_nAllDayViewCount;

			m_nAllDayViewCount = -1;
		}
		else
		{	//���õ� ũ��� ��.
			//>>0001399: ��Ʈ�� �׸��� ���������� �׸� �Ŀ� �ϴ� ������Ʈ���� ������ŭ ���������� �ٽ� �׸��°� �����ϴ�.
			//_stData.m_lZoomValue = pChartItem->GetNOnePageDataCount();			
			long nViewCount;
			((CInputBarDlg*)m_pInputBarDlg)->GetViewCount(nViewCount);
			_stData.m_lZoomValue = nViewCount;
			pChartItem->SetNew_OnePageDataCount(nViewCount);
			//<<0001399
		}
	}
	// ��Ƽ��Ʈ�� ���ÿ��� : �������� - ojtaso (20070525)
	ST_SELCHART* lpSelChart;
	if(pChartItem->GetSelChart(NULL, lpSelChart))
	{
		_stData.m_strCode = lpSelChart->m_strItemCode;
		if(lpSelChart->m_strItemCode.GetLength() && lpSelChart->m_lCount > 0)
			_stData.m_lTotalCnt = lpSelChart->m_lCount;
	}
	else
		_stData.m_strCode = pChartItem->GetDataCode();
	_stData.m_chType = pChartItem->GetType();
	
	_stData.m_strTick	= pChartItem->GetTickNumber();
	_stData.m_strSec	= pChartItem->GetSecNumber();
	_stData.m_strMin	= pChartItem->GetMinNumber();	
	_stData.m_strDay	= pChartItem->GetDayNumber();
	_stData.m_strHour	= pChartItem->GetHourNumber();

	//_stData.m_bRateValue = pChartItem->GetUseRateValue();
	_stData.m_nChartNumericValue = pChartItem->GetChart_NumericValueType();
	_stData.m_bFackedCandle = pChartItem->GetBNormalizedMinuteTimeRule();

	//=============================================================
	//2005.05.24 add by sy, nam
	//���� ���õ� Row,col�� �ѱ��.
	_stData.m_nSelCol = m_lColSelected;
	_stData.m_nSelRow = m_lRowSelected;
	//=============================================================


	//=============================================================
	//2005.08.22 add by sy, nam
	_stData.m_bRevised	= pChartItem->m_CfgInfo.bRevisedValue;
	_stData.m_bLog		= pChartItem->m_CfgInfo.bShowLog;	

	//=============================================================
	//2005.08.23 add by sy, nam
	_stData.m_strChartLineType	= pChartItem->m_strChartLineType;
	
	//=============================================================
	//2005.08.25 add by sy, nam
	//������ ���� ����
	_stData.m_bStartDateState	= pChartItem->m_CfgInfo.bStartDate;

	//============================================================
	//2005. 09. 05 ��ȭ ��
	if(bNotCodeInputInit == FALSE) // bNotCodeInputInit�� False�� ���� ���� �� �ѹ� �Ҹ���.
	{
		_stData.m_strCode = "NOCHANGE";
	}
	//==============================================================

//	if(((CStdDialog*)GetParent())->GetpMainBaseInfo()) // gm0604 added ..
//	{
		//============================================================
		//2005. 09. 05 ��ȭ ��
		long lFromTRdata = 0;
		if(bFromTRData)
			lFromTRdata  = 1;
		//============================================================

		if(m_pInputBarDlg)
		{
			m_pInputBarDlg->SendMessage(RMSG_SETINITIALCHARTDATA, 0, 0);
			m_pInputBarDlg->SendMessage(UMSG_SETINITIALCHARTDATA,lFromTRdata,(LPARAM)&_stData);
			m_pInputBarDlg->SendMessage(RMSG_SETINITIALCHARTDATA, 0, 1);
		}
//	}
	
	//=================================================================================
	// 2005. 05. 24 add by sy, nam
	// %, \, $ ǥ�ð� ������ CInputBarDlg���� CExtInputBarDlg�� �Ű����鼭...�߰��Ѵ�.
	//=================================================================================
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
	{
		m_pExtInputBarDlg->SendMessage(UMSG_SETINITIALCHARTDATA,0,(LPARAM)&_stData);
	}	
	//=================================================================================

	if(m_pLeftBarDlg)
	{
		m_pLeftBarDlg->SendMessage(UMSG_SETINITIALCHARTDATA,0,(LPARAM)&_stData);
	}	

	//((CStdDialog*)GetParent())->SetItemCodeName(_stData.m_strCode);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	Input	:	szNewPos  x, y ��Ʈ�� ��ġ
//	����	:	��Ʈ�� ���� ��Ʈ, ��ǥ List�� �����´�.
///////////////////////////////////////////////////////////////////////////////
CString CChartMng::GetIndicatorNameList(LPCTSTR p_strGroup)
{
	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();
	if(pChartItem == NULL) return _T("");

	return GetpChartItemSelected()->GetIndicatorNameList(p_strGroup);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	Input	:	p_strGroup  ��ǥ �׷��
//				pStrArray   �����
//	����	:	��Ʈ�� ���� ��Ʈ, ��ǥ List�� �����´�.
///////////////////////////////////////////////////////////////////////////////
void CChartMng::GetIndicatorNameListIntoArray(CStringArray *pStrArray, LPCTSTR p_strGroup)
{
	pStrArray->RemoveAll();
	if(p_strGroup[0]==NULL || p_strGroup == "��ü��ǥ")
	{
		CString strList = GetIndicatorNameList("");
		CString strALine;
		CString strALineTitle;
		CString strAItem;
		int nPos = 0;
		int nLength = 0;

		CStringList listIndi;
//@Solomon-MultiLang:091117 -->
//		pStrArray->Add(_LTEXT2(C2_PRICE_MA));
//		pStrArray->Add(_LTEXT2(C2_VOLUME_CHART));
//		pStrArray->Add(_LTEXT2(C2_VOLUME_MA));
//@Solomon-MultiLang:091117 <--

		if(strList.GetLength())
		{
			while(1)
			{
				nPos = strList.Find(':');
				if(nPos==-1) break;		
				if(nPos==0) 
				{
					nPos = strList.Find('\n');
					strList.Delete(0,nPos+1);
					continue;
				}			
				strALineTitle = strList.Left(nPos);
//@Solomon-MultiLang:091117 -->
// #ifdef __MULTI_LANG__ //@Solomon-100225
// 				if(strALineTitle==_MTEXT(C7_BASE)
// 					||strALineTitle==_MTEXT(C7_MARKET)
// 					||strALineTitle==_MTEXT(C7_INVESTOR)
// 					||strALineTitle==_MTEXT(C7_SPECIAL))
// #else
// 				if(strALineTitle=="�⺻��ǥ"
// 					||strALineTitle=="������ǥ"
// 					||strALineTitle=="�����ڵ���"
// 					||strALineTitle=="Ư����ǥ")
// #endif
//@Solomon-MultiLang:091117 <--
// 				{
// 					nPos = strList.Find('\n');
// 					strList.Delete(0,nPos+1);
// 					continue;
// 				}

				strList.Delete(0,nPos+1);
				nPos = strList.Find('\n');
				strALine = strList.Left(nPos-1);		
				strList.Delete(0,nPos+1);

				while(1)
				{
					nLength = strALine.GetLength();
					if(!nLength) break;
					nPos = strALine.Find(',');
					if(nPos==-1) 
					{
						strAItem = strALine;
						strALine.Empty();
					}
					else
					{
						strAItem = strALine.Left(nPos);
						strALine.Delete(0,nPos+1);
					}

					listIndi.AddTail(_MTOL2(strAItem));
// #ifdef __MULTI_LANG__ //@Solomon-100225
// 					pStrArray->Add(_MTOL2(strAItem));
// #else
// 					pStrArray->Add(strAItem);
// #endif
				}
			}		
		}	
		
		POSITION endpos = listIndi.GetTailPosition();
		for(int i=1; i<listIndi.GetCount(); i++)
		{
			bool bChanged = false;
			POSITION pos2;
			POSITION pos1 = listIndi.GetHeadPosition();
			
			// Loop through list
			while ((pos2=pos1) != endpos)
			{
				CString str1 = listIndi.GetNext(pos1);
				CString str2 = listIndi.GetAt(pos1);
				if (strcmp(str1, str2) > 0)
				{
					// If str2 is greater than str1 exchange strings
					listIndi.SetAt(pos2, str2);
					listIndi.SetAt(pos1, str1);
					bChanged = true;
				}
			}
			if (!bChanged) // If nothing changed in this pass
				break; // sorting is finished
			listIndi.GetPrev(endpos); // Decrease endpos for next pass
		}

		char cAscii;
		int nAscii = 0;
		CString str;
		POSITION pos = listIndi.GetHeadPosition();
		while( pos)
		{
			str = listIndi.GetNext(pos);
			cAscii = str[0];
			// Ư����ȣ or ����
			if((cAscii>=33 && cAscii<=47)			// Ư������
				|| (cAscii>=58 && cAscii<=64) 
				|| (cAscii>=91 && cAscii<=96) 
				|| (cAscii>=123 && cAscii<=126)
				|| (cAscii>=48 && cAscii<=57))		// ����
				pStrArray->Add( str);
		}

		pos = listIndi.GetHeadPosition();
		while( pos)
		{
			str = listIndi.GetNext(pos);
			cAscii = str[0];
			// �ѱ�
			if(!((cAscii>=33 && cAscii<=47)			// Ư������
				|| (cAscii>=58 && cAscii<=64) 
				|| (cAscii>=91 && cAscii<=96) 
				|| (cAscii>=123 && cAscii<=126)
				|| (cAscii>=48 && cAscii<=57)		// ����
				|| (cAscii>=65 && cAscii<=90)		// ����
				|| (cAscii>=97 && cAscii<=122)))
				pStrArray->Add( str);		
		}

		pos = listIndi.GetHeadPosition();
		while( pos)
		{
			str = listIndi.GetNext(pos);
			cAscii = str[0];
			// ����
			if((cAscii>=65 && cAscii<=90)		// ����
				|| (cAscii>=97 && cAscii<=122))
				pStrArray->Add( str);		
		}
	}
	else
	{
		CString strGroup = p_strGroup;
// #ifdef __MULTI_LANG__ //@Solomon-100225
		if(strGroup=="Ư����ǥ")
		{
			pStrArray->Add(_LTEXT5(C5_CANDLE_CHART));
			pStrArray->Add(_LTEXT5(C5_BAR_CHART));
			pStrArray->Add(_LTEXT5(C5_LINE_CHART));
			pStrArray->Add(_LTEXT5(C5_FLOW_CHART));
			//pStrArray->Add("����Ʈ");
			//pStrArray->Add("������Ʈ");
			//pStrArray->Add("�Ｑ��ȯ��");
			pStrArray->Add(_LTEXT5(C5_CANDLE_VOLUME_CHART));
			pStrArray->Add(_LTEXT5(C5_EQUI_VOLUME_CHART));
			pStrArray->Add(_LTEXT2(C2_RAINBOW_CHART));
			pStrArray->Add(_LTEXT2(C2_OVERHANGING_SUPPLY));
			pStrArray->Add(_LTEXT2(C2_OBVIOUSLY_BALANCE_CHART));
		}
// #else
// 		if(strGroup=="Ư����ǥ")
// 		{
// 			pStrArray->Add("ĵ����Ʈ");
// 			pStrArray->Add("����Ʈ");
// 			pStrArray->Add("����Ʈ");
// 			pStrArray->Add("FLOW��Ʈ");
// 			//pStrArray->Add("����Ʈ");
// 			//pStrArray->Add("������Ʈ");
// 			//pStrArray->Add("�Ｑ��ȯ��");
// 			pStrArray->Add("ĵ�麼����Ʈ");
// 			pStrArray->Add("��ť������Ʈ");
// 		}
// #endif

		CString strALine = GetIndicatorNameList(p_strGroup);
		CString strAItem;
		int nPos = 0;
		int nLength = 0;

		if(strALine.GetLength())
		{
			while(1)
			{
				nLength = strALine.GetLength();
				if(!nLength) break;
				nPos = strALine.Find(',');
				if(nPos==-1) 
				{
					strAItem = strALine;
					strALine.Empty();
				}
				else
				{
					strAItem = strALine.Left(nPos);
					strALine.Delete(0,nPos+1);
				}

				// (2009/12/16 - Seung-Won, Bae) for MultiLanguage
				if(strAItem=="������Ʈ")
				{
					continue;
				}

// #ifdef __MULTI_LANG__ //@Solomon-100225
				pStrArray->Add(_MTOL2(strAItem));
// #else
// 				pStrArray->Add(strAItem);
// #endif
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	Input	:	p_strGroup  ��ǥ �׷��
//				rxlistCtrl  ������� �� ListCtrl
//	����	:	��Ʈ�� ���� ��Ʈ, ��ǥ List�� �����´�.
///////////////////////////////////////////////////////////////////////////////
void CChartMng::GetIndicatorNameListIntoListCtrl(CXListCtrl& rxlistCtrl, LPCTSTR p_strGroup)
{
	if(p_strGroup[0]!=NULL)
	{
		CString strALine = GetIndicatorNameList(p_strGroup);
		CString strAItem;
		int nPos = 0;
		int nItem = 0;
		int nLength = 0;
		rxlistCtrl.DeleteAllItems();
		if(!strcmp(p_strGroup,"Ư����ǥ"))
		{
			rxlistCtrl.InsertItem(0, "");
			rxlistCtrl.SetItemText(0, 0, "ĵ����Ʈ",CLR_FONT_DEFAULT,CRGRID);

			rxlistCtrl.InsertItem(1, "");
			rxlistCtrl.SetItemText(1, 0, "����Ʈ");

			rxlistCtrl.InsertItem(2, "");
			rxlistCtrl.SetItemText(2, 0, "����Ʈ",CLR_FONT_DEFAULT,CRGRID);

			//rxlistCtrl.InsertItem(3, "");
			//rxlistCtrl.SetItemText(3, 0, "����Ʈ");

			//rxlistCtrl.InsertItem(4, "");
			//rxlistCtrl.SetItemText(4, 0, "������Ʈ");

			//rxlistCtrl.InsertItem(4, "");
			//rxlistCtrl.SetItemText(4, 0, "�Ｑ��ȯ��",CLR_FONT_DEFAULT,CRGRID);

			//rxlistCtrl.InsertItem(6, "");
			//rxlistCtrl.SetItemText(6, 0, "��ť������Ʈ");
			
			//rxlistCtrl.InsertItem(7, "");
			//rxlistCtrl.SetItemText(7, 0, "ĵ�麼����Ʈ",CLR_FONT_DEFAULT,CRGRID);
			
			nItem = 3;
		}

		if(strALine.GetLength())
		{
			while(1)
			{
				nLength = strALine.GetLength();
				if(!nLength) break;
				nPos = strALine.Find(',');
				if(nPos==-1) 
				{
					strAItem = strALine;
					strALine.Empty();
				}
				else
				{
					strAItem = strALine.Left(nPos);
					strALine.Delete(0,nPos+1);
				}
				//if(strAItem=="P&F")
				//{
				//	strAItem = "P&&F";
				//}
				rxlistCtrl.InsertItem(nItem, "");
				if(nItem%2) // odd
				{
					rxlistCtrl.SetItemText(nItem, 0, strAItem);
				}
				else		// even
				{
					rxlistCtrl.SetItemText(nItem, 0, strAItem,CLR_FONT_DEFAULT,CRGRID);					
				}				
				nItem++;
			}
		}
	}
	else
	{
		CString strList = GetIndicatorNameList("");
		CString strALine;
		CString strALineTitle;
		CString strAItem;
		int nPos = 0;
		int nItem = 0;
		int nLength = 0;
		rxlistCtrl.DeleteAllItems();
		rxlistCtrl.InsertItem(nItem, "");
		rxlistCtrl.SetItemText(nItem, 0, "�ŷ�����Ʈ",CLR_FONT_DEFAULT,CRGRID);
		nItem++;

		if(strList.GetLength())
		{
			while(1)
			{
				nPos = strList.Find(':');
				if(nPos==-1) break;		
				if(nPos==0) 
				{
					nPos = strList.Find('\n');
					strList.Delete(0,nPos+1);
					continue;
				}			
				strALineTitle = strList.Left(nPos);
				if(strALineTitle=="�⺻��ǥ"
					||strALineTitle=="������ǥ"
					||strALineTitle=="�����ڵ���"
					||strALineTitle=="Ư����ǥ")
				{
					nPos = strList.Find('\n');
					strList.Delete(0,nPos+1);
					continue;
				}

				strList.Delete(0,nPos+1);
				nPos = strList.Find('\n');
				strALine = strList.Left(nPos-1);		
				strList.Delete(0,nPos+1);
				
				while(1)
				{
					nLength = strALine.GetLength();
					if(!nLength) break;
					nPos = strALine.Find(',');
					if(nPos==-1) 
					{
						strAItem = strALine;
						strALine.Empty();
					}
					else
					{
						strAItem = strALine.Left(nPos);
						strALine.Delete(0,nPos+1);
					}
					rxlistCtrl.InsertItem(nItem, "");
					if(nItem%2) // odd
					{
						rxlistCtrl.SetItemText(nItem, 0, strAItem);
					}
					else		// even
					{
						rxlistCtrl.SetItemText(nItem, 0, strAItem,CLR_FONT_DEFAULT,CRGRID);					
					}				
					nItem++;
				}
			}		
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	Input	:	
//				WPARAM	: MESSAGE
//				LPARAM	: MSG Structure Address
//	����	:	��Ʈ���� ���� Message ���� ó���Ѵ�.
///////////////////////////////////////////////////////////////////////////////
LRESULT CChartMng::OnEventMessage( WPARAM wParam, LPARAM lParam)
{	
	MSG *pMsgFromChart = (MSG *)lParam;
	//TRACE1("OnEventMessage %d\n", pMsgFromChart->message);
	switch(pMsgFromChart->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
//	case WM_SETFOCUS:	// ���� drag&grop�� �ش� ��Ʈ�� focus�� ������ ��..	
		{	
			m_bOnEventMessage = TRUE;
			CDRChartOcx *pChart = (CDRChartOcx *)GetDlgItem(wParam);		
			if(pChart)
			{				
				EACH_CHARTJOB_START()
					if(pChart==pChartItem)
					{
						//CString strTemp;
						//strTemp.Format("SETFOCUS %d::%d\r\n", nColIndex, nRowIndex);
						//OutputDebugString(strTemp);
						//if( pMsgFromChart->message == WM_LBUTTONDOWN )
						//2007.01.17 add by LYH  ���ٹ�ư ����ȭ(���� ���� ����)
						int m_lColSelectedOld = m_lColSelected;
						int m_lRowSelectedOld = m_lRowSelected;
						//}}

						SetChartSelected(CPoint(nColIndex,nRowIndex));

						//2007.01.17 add by LYH  ���ٹ�ư ����ȭ(������ ���´�)
						if(m_lColSelectedOld!=nColIndex || m_lRowSelectedOld != nRowIndex)
						{
							// ��Ƽ��Ʈ�� ���ÿ��� : �������� - ojtaso (20070525)
							SetSelChangeChart(TRUE);

							GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 0, 50000);
							GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50031, pChartItem->m_bDrawAutoTrendLine );
							GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50032, pChartItem->m_bDrawAutoPattern );
							GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50033, pChartItem->m_bDrawCandlePattern );
							GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50026, pChartItem->m_bDrawNews );

							if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
							{
								m_pExtInputBarDlg->SendMessage(UMSG_SYNC_TOOLBAR, CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL, 0);
							}
							pChartItem->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::T_ALL_TOOL_TRUE_QUERY, 0);

							//>> [QC No]090804HMC082
							SetSelChangeChart(FALSE);
							//<< [QC No]090804HMC082
						}
						//}}

						char cType = pChartItem->GetType();
						if(cType == MIN_DATA_CHART || cType == SEC_DATA_CHART || cType == TICK_DATA_CHART)
							((CStdDialog*)GetParent())->EnableQueryOpt(TRUE);
						else
							((CStdDialog*)GetParent())->EnableQueryOpt(FALSE);

						break;
					}	
				EACH_CHARTJOB_END()	

//>> [QC No]090728HMC040
				UpdateSyncIndicator();
//<< [QC No]090728HMC040
			}	
			m_bOnEventMessage = FALSE;
		}
		break;
	case WM_KEYUP:
		{
			if(pMsgFromChart->wParam==VK_TAB)
			{
				SHORT wShiftKeyDown = ::GetAsyncKeyState(VK_SHIFT);
				CDRChartOcx *pChart = (CDRChartOcx *)GetDlgItem(wParam);
				if(pChart)
				{
					int nColIndexCurrent = 0;
					int nRowIndexCurrent = 0;
					EACH_CHARTJOB_START()
					if(pChart==pChartItem)
					{
						nColIndexCurrent = nColIndex;
						nRowIndexCurrent = nRowIndex;
					}
					EACH_CHARTJOB_END()
					if(wShiftKeyDown)
					{
						if(nColIndexCurrent>0)
						{
							nColIndexCurrent--;
						}
						else if(nRowIndexCurrent>0)
						{
							nRowIndexCurrent--;
							nColIndexCurrent = GetCols() - 1;
						}
						else
						{
							nColIndexCurrent = GetCols() - 1;
							nRowIndexCurrent = GetRows() - 1;
						}
					}
					else
					{
						if((nColIndexCurrent+1)<GetCols())
						{
							nColIndexCurrent++;
						}
						else if((nRowIndexCurrent+1)<GetRows())
						{
							nRowIndexCurrent++;
							nColIndexCurrent = 0;
						}
						else
						{
							nColIndexCurrent = 0;
							nRowIndexCurrent = 0;
						}
					}
					SetChartSelected(CPoint(nColIndexCurrent,nRowIndexCurrent));
				}
			}
		}
		break;
	case UWM_ZOOMVALUE:
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem == NULL) 
				break;

			CDRChartOcx *pChart = (CDRChartOcx *)GetDlgItem(wParam);		
			if(GetpChartItemSelected()==pChart&&m_bLastEnvUpdated)
			{
				//R��ư���� �߻��ϴ� ��ü �ʱ�ȭ ��� �ǹ� 05.12.05 �����븮 ��û=================================
				if(pMsgFromChart->wParam == 0 && GetpChartItemSelected()->GetViewCountFlag() == FALSE) //20090922 SJ_KIM ViewCount
				//=============================================================================
 				if(m_pInputBarDlg)
				{
//@Test1
					//���� ���콺�� ���� Zoom����� �̴ϸ� �����ϰ� ��.
					//@100306 if(((CStdDialog*)GetParent())->m_nChartType>=MINI_MULTI_CHART && ((CStdDialog*)GetParent())->m_nChartType<=MINI_SINGLE2_CHART)
					if(((CStdDialog*)GetParent())->m_nScreenID>=367 && ((CStdDialog*)GetParent())->m_nScreenID<=371)
					{
							m_pInputBarDlg->SendMessage(UMSG_SETZOOMVALUE,pMsgFromChart->lParam);
							((CInputBarDlg*)m_pInputBarDlg)->SetViewCount(pMsgFromChart->lParam);
					}
//@Test2
// 					long nViewCount;
// 					((CInputBarDlg*)m_pInputBarDlg)->GetViewCount(nViewCount);
// 
// 					if(pChartItem->IsReqCntIsMore())
// 					{
// 						if(nViewCount<=pMsgFromChart->lParam)
// 						{
// 							m_pInputBarDlg->SendMessage(UMSG_SETZOOMVALUE,pMsgFromChart->lParam);
// 							((CInputBarDlg*)m_pInputBarDlg)->SetViewCount(pMsgFromChart->lParam);
// 						}
// 						else
// 						{
// 							m_pInputBarDlg->SendMessage(UMSG_SETZOOMVALUE, nViewCount);
// 							((CInputBarDlg*)m_pInputBarDlg)->SetViewCount( nViewCount);
// 						}
// 					}
				}
			}
		}		
		break;
	case UWM_USEINQUIRY_CROSSLINE:
		{
			CChartItem *pChart = (CChartItem *)GetDlgItem(wParam);
			EACH_CHARTJOB_START()
				if(pChartItem!=pChart)
				{
					pChartItem->SetNUseInquiryWithCrossline(pMsgFromChart->wParam);
				}				
			EACH_CHARTJOB_END()
		}
		break;
	case UWM_FIREGSHAREDATA: // Global ������ ������ �Ƿ�... �Ƚ������.
		{
			CString strCodeShare = (LPCTSTR)pMsgFromChart->lParam;			
			int nSize = strCodeShare.GetLength();
			int nPos = strCodeShare.Find(';');
			if(nPos<1) return 1L;
			CString strCodeHead = strCodeShare.Left(nPos);
			CString strCodeTail = strCodeShare.Right(nSize-(nPos+1));
			if(strCodeHead=="�ð�-��Ʈ")
			{
				CChartItem *pChart = (CChartItem *)GetDlgItem(wParam);
				SetTimeMark((LPCTSTR)strCodeTail,TRUE);
			}
			else
			{
				STSHAREDDATA stData;
				stData.m_szKey = (LPCTSTR)strCodeHead;
				stData.m_szValue = (LPCTSTR)strCodeTail;
				if(m_pLeftBarDlg)
				{
					m_pLeftBarDlg->SendMessage(UMSG_SETCHARTHOGA,NULL,(LPARAM)&stData);
				}	
			}

		}
		break;
	case UWM_DRAGSTART:
		{
			m_lDragStart = pMsgFromChart->lParam;
		}
		break;
	case UWM_DRAGEND:
		{
			m_lDragEnd = pMsgFromChart->lParam;
			CChartItem *pChart = (CChartItem *)GetDlgItem(wParam);
			if(pChart->GetDataTRCode()==QUERY_STRJONGMOK)
			{
				SetSendJongData(pChart->GetDataCode(),pChart->GetType(),pChart->GetTickMinDayNumber());
			}			
		}
		break;
	case UWM_ORGRETURN:
		{
			// ���õ� ��Ʈ�� ���� ��û�� ������ default ���� ����
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem == NULL) 
				return 0;

			if( ((CStdDialog*)GetParent())->m_nChartType >= MINI_MULTI_CHART)
				pChartItem->SetCntForRequest(MINICNTATONCE);
			else
				pChartItem->SetCntForRequest(DEFAULTCNTATONCE);
//			pChartItem->SetSaveAllCount(0);

			((CStdDialog*)GetParent())->ResetChartData();

			if(!m_bReplaychartMode) //06.02.03  ���� �� �ʱ�ȭ�� ���� TR �����ϴ� ���� ���´�.
			{
				pChartItem->m_bDrawAutoTrendLine = 0;
				pChartItem->m_bDrawAutoPattern = 0;
				pChartItem->m_bDrawCandlePattern = 0;
				pChartItem->m_bDrawNews = 0;
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50031, pChartItem->m_bDrawAutoTrendLine );
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50032, pChartItem->m_bDrawAutoPattern );
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50033, pChartItem->m_bDrawCandlePattern );
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50026, pChartItem->m_bDrawNews );

				m_bHideScale = FALSE;
				ShowScale();
				m_bHideInnerToolBar = FALSE;
				ShowInnerToolbar();
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50040, pChartItem->m_bDrawCandlePattern );
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50041, pChartItem->m_bDrawNews );
				//�ʱ�ȭ ��Ű���� ���� ����.
				if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
					m_pExtInputBarDlg->SendMessage(UMSG_CHART_INIT_FROMOCX);

				//�ʱ�ȭ �� �ٽ� TR������
				if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
					m_pInputBarDlg->SendMessage(UMSG_CHART_INIT_FROMOCX);		

				pChartItem->InvokeAddIn("ChartPatternAddin.dll",0,200);
				pChartItem->InvokeAddInSTIndex("ChartSTIndexAddIn.dll",200,0);	// �ý���Ʈ���̵� ��ü�ʱ�ȭ
			}		
		}
		break;
		// �������� ��Ʈ ���� : �������� - ojtaso (20070122)
	case UWM_SELECTEDCHART:
		{
			if(pMsgFromChart->lParam)
			{
				// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
				CChartItem* pSelItem = GetpChartItemSelected();
				if(pSelItem)
				{
					LPCTSTR pSelRQ = (LPCTSTR)pMsgFromChart->lParam;
					ST_SELCHART* pSelChart = NULL;

					// ������ RQ�� �������� �ʵ��� ���� - ojtaso (20080509)
					if(pSelItem->GetSelChart(pSelRQ, pSelChart, pMsgFromChart->wParam))
					{
						if(!pMsgFromChart->wParam)	// ��Ʈ ����
						{
							((CStdDialog*)GetParent())->OnDeleteChartItem(pSelChart->m_strItemCode);
							pSelItem->OnDeleteChartItem(pSelRQ, pSelChart);

//>> [QC No]090728HMC040
							UpdateSyncIndicator();
//<< [QC No]090728HMC040
						}
						else						// ���õ�
						{
							((CStdDialog*)GetParent())->OnSelectChartItem(pSelChart);
							pSelItem->OnSelectChartItem(pSelChart);
						}
					}
				}

			}
		}
		break;
		// ���������� Total Count�� ��Ƽ��Ʈ�� �˷��� : �������� - ojtaso (20070704)
	case UWM_MULTITOTALCOUNT:
		if(m_pInputBarDlg)
		{
			CDRChartOcx *pChart = (CDRChartOcx *)GetDlgItem(wParam);		
			if(GetpChartItemSelected()==pChart&&m_bLastEnvUpdated)
			{
				((CInputBarDlg*)m_pInputBarDlg)->SetMultiTotalCount(pMsgFromChart->lParam);
			}
		}
		break;
// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
	// ADD PSH 20101117 - 20100730 ���μ� �ֱ⺰ ���� ���� �߰� >>
	case UWM_USECYCLESAVE:
		{
			BOOL bUseCycleSave = pMsgFromChart->lParam;
			CStdDialog* pStdDialog = ((CStdDialog*)GetParent());
			if(NULL != pStdDialog)
			{
				pStdDialog->SetUseCycleSave(bUseCycleSave);
			}
			// END CHG
		}
		break;
	}
	//2007.01.17 add by LYH  ���ٹ�ư ����ȭ
	if(pMsgFromChart->message == RMS_ANALYSIS_FUNC)
	{
		//���ٿ� ���� �޽��� ����
		if(pMsgFromChart->wParam == CToolOptionInfo::TOOLOPTION::T_DRAG_X_ZOOM 
			|| pMsgFromChart->wParam == CToolOptionInfo::TOOLOPTION::T_WHOLEVIEW
			|| pMsgFromChart->wParam == CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION)
			return 0;
		//end

		if(pMsgFromChart->wParam == CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL
			|| pMsgFromChart->wParam == CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_LEFT
			|| pMsgFromChart->wParam == CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_RIGHT)
		{
			if( m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd() )
				m_pExtInputBarDlg->SendMessage(UMSG_SYNC_TOOLBAR,pMsgFromChart->wParam, pMsgFromChart->lParam);
			return 0;
		}
		CChartItem* pChartItem = NULL;
		pChartItem = GetpChartItemSelected();
		if(pChartItem == NULL) 
			return 0;

		CDRChartOcx *pChart = (CDRChartOcx *)GetDlgItem(wParam);		
		if(pChartItem==pChart)
		{
			int nID = (int)pMsgFromChart->wParam;
			CString strData;
			strData.Format("%d | %d",nID , pMsgFromChart->lParam);
			//AfxMessageBox(strData);
			GetParent()->SendMessage(UMSG_SYNC_TOOLBAR,pMsgFromChart->wParam, pMsgFromChart->lParam);				
		}
	}		
	//end
	return 0;
}

LRESULT CChartMng::OnGetUserID( WPARAM wParam, LPARAM lParam)
{
	//@Solomon-091106 ��Ʈ������ �ʹ� ���� �����Ǿ� Chart������ ������ ó���ϱ�����.
	//OCX���� �̰��� �������� ó���� �ϰ� �ִ� ����.
	static CString szID;
	//szID.Format("%s%s", ((CStdDialog*)GetParent())->m_strUserID, "\\Chart");
	szID.Format("%s%s", m_pStdDialog->m_strUserID, "\\Chart");
	return (long)(LPCSTR)szID;

	//������ƾ  (long)(LPCSTR)((CStdDialog*)GetParent())->m_strUserID;
}

LRESULT CChartMng::OnMagnifyingGlass( WPARAM wParam, LPARAM lParam)
{
	((CStdDialog*)GetParent())->OnMagnifyingglass((BOOL)wParam);
	return 0L;
}

void CChartMng::SetTimeMark(LPCTSTR p_szMarkTime, BOOL p_bWithScroll)
{
	EACH_CHARTJOB_START()
		pChartItem->SetTimeMark(p_szMarkTime,p_bWithScroll);
	EACH_CHARTJOB_END()	
}


void CChartMng::OnPaint() 
{
	CRect clientRc;
	GetClientRect(&clientRc);
	CPaintDC dc(this);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, clientRc.Width(), clientRc.Height());
	CBitmap * obmp = memDC.SelectObject(&bmp);

	CDC * pDC = &memDC;

	//	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect,RGB(128,135,145));
	rect.DeflateRect(1,1,1,1);
	pDC->FillSolidRect(rect, RGB(237,243,249));	//RGB(222,222,222));

	if(((CStdDialog*)GetParent())->m_nChartType < MINI_MULTI_CHART || ((CStdDialog*)GetParent())->m_nScreenID == nScreenID_6301 || ((CStdDialog*)GetParent())->m_nScreenID == nScreenID_6401)
	{
		EACH_CHARTJOB_START()
			PaintTitle(nRowIndex,nColIndex,pChartItem,pDC);			
		//		pChartItem->Invalidate();
		EACH_CHARTJOB_END()
	}

		dc.BitBlt(clientRc.left, clientRc.top, clientRc.Width(), clientRc.Height(), &memDC, 0,0, SRCCOPY);

	memDC.SelectObject(obmp);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	Input	:	
//	����	:	��Ʈ�� Title���� Draw�Ѵ�.
///////////////////////////////////////////////////////////////////////////////
void CChartMng::PaintTitle(int nRowIndex,int nColIndex,CChartItem* pChartItem,CDC *pDC)
{
	CRect rect;
	rect = GetRectOfChart(nRowIndex,nColIndex,TRUE);
	pChartItem->SetButton(rect);	
	if(rect.IsRectEmpty()) return;	
	COLORREF Start, End;

	CChartItem* pChartItem2 = NULL;
	pChartItem2 = GetpChartItemSelected();
	if(pChartItem2 == NULL) 
		return;

	//{{KJS_20100917. 1X1�� ���� Ÿ��Ʋ ������ �ٲ��� ����� ��û
	//if(pChartItem==GetpChartItemSelected())
	if(pChartItem==GetpChartItemSelected() && (m_lCols > 1 || m_lRows > 1) )
	//}}
	{
		Start = m_clStartSelected;
		End   = m_clEndSelected;
	}
	else
	{
		Start = m_clStartUnselected;
		End   = m_clEndUnselected;
	}

	pDC->FillSolidRect( &rect,  Start);
	CRect backup = rect;
//		
//	int r,g,b;//�� 3���� ������ ���� ����..
//	float rStep, gStep, bStep;//�� 3���� ���� ������ ������ ����..
//	float fStep;//�ִ� ���� ���̷� ����� �ѹ��� �̵��� �ɼ�..
//
//	//�� ���� ���̸� ���Ѵ�..
//	r = (GetRValue(End) - GetRValue(Start));
//	g = (GetGValue(End) - GetGValue(Start));
//	b = (GetBValue(End) - GetBValue(Start));
//
//	//���� ū���̰� ���°��� ���Ѵ�..
//	int Max = max( abs(r), max(abs(g), abs(b)) );
//
//	//�׸� �簢���� ���̰� ���°� ��ŭ.. ������ ���̸� ���Ѵ�..
//	fStep = (float)rect.Height()/(float)Max;     // ��-�� Gradation 
//
//	//�� ���� �ܰ� ���̰�..
//	rStep = r/(float)Max;
//	gStep = g/(float)Max;
//	bStep = b/(float)Max;
//
//	//r,g,b�� ���� ������ �缳��...
//	r = GetRValue(Start);
//	g = GetGValue(Start);
//	b = GetBValue(Start);
//
//	//������ ���ؾ� �ϴ� Ƚ����ŭ.. �ݺ� �׸���.
//	for(int iOnBand=0 ; iOnBand <Max; iOnBand++)  // ��-�� Gradation 
//	{
//		::SetRect( &rect, rect.left, int(fStep*iOnBand) + backup.top ,
//		    rect.right, backup.bottom );  
//		pDC->FillSolidRect( &rect,  RGB( r + rStep*iOnBand, g + gStep*iOnBand, b + bStep*iOnBand ) );
//	}	
//	pChartItem->DrawTitle(backup, pDC,&m_ftForTitle);

	//{{KJS_20100917. 1X1�� ���� Ÿ��Ʋ ������ �ٲ��� ����� ��û
	//if(pChartItem==GetpChartItemSelected())
	if(pChartItem==GetpChartItemSelected() && (m_lCols > 1 || m_lRows > 1) )
	//}}
	{
		if(pChartItem->GetBReplayChartMode())
		{

			//	OutputDebugString("Paint Tilte" + pChartItem->GetDataCode());
			pChartItem->DrawReplayChartTitle(backup, pDC,&m_ftForTitle,&m_ftNormal,m_clStartSelected);
		}
		else
			pChartItem->DrawTitle(backup, pDC,&m_ftForTitle,&m_ftNormal,m_clStartSelected);			
	}
	else
	{
		if(pChartItem->GetBReplayChartMode())
		{
			//	OutputDebugString("Paint Tilte" + pChartItem->GetDataCode());
			pChartItem->DrawReplayChartTitle(backup, pDC,&m_ftForTitle,&m_ftNormal,m_clStartUnselected);
		}
		else
			pChartItem->DrawTitle(backup, pDC,&m_ftNormal,&m_ftNormal,m_clStartUnselected);
	}
}


void CChartMng::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	EACH_CHARTJOB_START()
	rect = GetRectOfChart(nRowIndex,nColIndex,TRUE);
	if(rect.PtInRect(point))
	{
		SetChartSelected(CPoint(nColIndex,nRowIndex));
		ExtendChart(!m_bChartExtended);

		// ȣ�� â Show ---------------------------------------------------
		// 2005. 04. 28 by Nam
		//-----------------------------------------------------------------
		//HWND hParent = GetParent()->GetSafeHwnd();		
		//::SendMessage(hParent, UMSG_FORCE_SHOW_HOGAWND, m_bChartExtended, 0 );		
		//-----------------------------------------------------------------
		break;

	}
	EACH_CHARTJOB_END()		

	


	CWnd::OnLButtonDblClk(nFlags, point);
}

void CChartMng::MoveCharts(int nRowIndexOutside/*= -1*/,int nColIndexOutside/* = -1*/)
{
	if(nRowIndexOutside==-1&&nColIndexOutside==-1)
	{
		EACH_CHARTJOB_START()
		CRect rect = GetRectOfChart(nRowIndex,nColIndex);
		//2005. 06. 07 =======================================================================
		//������ �����Ǵ� ChartOCX(chartItem)���� Drds�� ��û ������ ClassPointer�� �Ѱ��ش�.
		//added by sang yun, nam
		if(m_lMltchrtDlgParentClsPointer > 0)
		{
			pChartItem->SetOutsideCommonInterface(m_lMltchrtDlgParentClsPointer); 
			pChartItem->m_CfgInfo.bShowLog = pChartItem->GetLogScale(_T("")) ? true : false;
			//pChartItem->SetRootPath(m_strRootPath);
		}
		//====================================================================================
		pChartItem->MoveWindow(rect);	
		EACH_CHARTJOB_END()

	//	if(m_pFloatingDlg && m_pFloatingDlg->GetSafeHwnd())
	//		m_pFloatingDlg->SetWindowPos(	&CWnd::wndTopMost,
	//										m_pRectFloatingHoga->left,
	//										m_pRectFloatingHoga->top,
	//										m_pRectFloatingHoga->Width(),
	//										m_pRectFloatingHoga->Height(),
	//										SWP_NOMOVE | SWP_NOSIZE |SWP_SHOWWINDOW);
	}
	else// gm0604 added 2005.10.10
	{
		CRect rect = GetRectOfChart(nRowIndexOutside,nColIndexOutside);
		CChartItem *pChartItem = m_pChartItems[nRowIndexOutside][nColIndexOutside];
		//2005. 06. 07 =======================================================================
		//������ �����Ǵ� ChartOCX(chartItem)���� Drds�� ��û ������ ClassPointer�� �Ѱ��ش�.
		//added by sang yun, nam
		if(m_lMltchrtDlgParentClsPointer > 0)
		{
			pChartItem->SetOutsideCommonInterface(m_lMltchrtDlgParentClsPointer); 
			pChartItem->m_CfgInfo.bShowLog = pChartItem->GetLogScale(_T("")) ? true : false;
			//pChartItem->SetRootPath(m_strRootPath);
		}
		//====================================================================================
		pChartItem->MoveWindow(rect);	

	}
	
	Invalidate(TRUE);
}


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	Input	:	lCnt ��ȭ��
//				nType	RESIZEBOTH = ��,�� ���� ����
//						RESIZECOL  = �� ����
//						RESIZEROW  = �� ����
//	����	:	��Ʈ�� ���� ������Ű�ų� ���ҽ�Ų��.
///////////////////////////////////////////////////////////////////////////////
BOOL CChartMng::Rearrange(long lCnt, int nType)
{
	CChartItem *pChartItem = NULL;
	m_bChartExtended = FALSE;
	long lColsForFuture = m_lCols;
	long lRowsForFuture = m_lRows;
	CString strEndDateFromInputBar;
	if(nType == RESIZEBOTH)
	{
		lColsForFuture += lCnt;
		lRowsForFuture += lCnt;
	}
	else if(nType == RESIZECOL)
	{
		lColsForFuture += lCnt;
	}
	else if(nType == RESIZEROW)
	{
		lRowsForFuture += lCnt;
	}
	if(lCnt>0)
	{
		if(lColsForFuture>CNTOFMAXCOL)	return FALSE;
		if(lRowsForFuture>CNTOFMAXROW)	return FALSE;
		for(int nRowIndex=0;nRowIndex<lRowsForFuture;nRowIndex++)
		{
			for(int nColIndex=0;nColIndex<lColsForFuture;nColIndex++)
			{
				if(m_nChartLayout && nRowIndex && nColIndex)
					break;

				pChartItem = NULL;
				pChartItem = m_pChartItems[nRowIndex][nColIndex];
				if(pChartItem == NULL)
				{
					pChartItem = new CChartItem;
					m_nID += 10;
					if( ((CStdDialog*)GetParent())->m_nChartType >= MINI_MULTI_CHART)
					{
						pChartItem->Create(this,GetRectOfChart(nRowIndex,nColIndex),m_nID,nRowIndex,nColIndex, m_strImagePath, TRUE);
						pChartItem->SetSmallChartMode(2);
					}
					else
					{
						if(!pChartItem->Create(this,GetRectOfChart(nRowIndex,nColIndex),m_nID,nRowIndex,nColIndex, m_strImagePath, TRUE))
						{
							delete pChartItem;
							return FALSE;
						}
					}					
//					else if(((CStdDialog*)GetParent())->GetMapID()=="10010")
//					{
//						pChartItem->Create(this,GetRectOfChart(nRowIndex,nColIndex),m_nID,nRowIndex,nColIndex, "");
//						pChartItem->SetSmallChartMode(1);
//					}
					
					pChartItem->ShowInnerToolBar(FALSE);
					
					//================================================================
					// 2005. 07. 28 add by sy, nam
					// InputBar���� Extension Bar�� 
					//================================================================
					if(!m_pExtInputBarDlg)
					{
						//MessageBox("m_pExtInputBarDlg ERROR!");
					}
					else
					{
						long lEndDate = m_pExtInputBarDlg->SendMessage(UMSG_GETENDDATEFROMINPUTBAR);
						//================================================================
						if(lEndDate)
						{
							strEndDateFromInputBar.Format("%d",lEndDate);
							pChartItem->SetEndDateRequested(strEndDateFromInputBar);
							pChartItem->SetEndDate(strEndDateFromInputBar); //05.09.06 added by sy, nam
						}
					}
					if(m_bTypeLink) //��������
					{							
						char chType = m_pInputBarDlg ? (char)m_pInputBarDlg->SendMessage(UMSG_GETTYPEFROMINPUTBAR) : '0';
						long lNumber = m_pInputBarDlg ? m_pInputBarDlg->SendMessage(UMSG_GETTYPENUMFROMINPUTBAR) : 0;
						pChartItem->SetType(chType);
						pChartItem->SetTickMinDayNumber(lNumber);
					}
					if(m_lKey)
					{
						pChartItem->SetInit((long)this->GetSafeHwnd(), m_lKey, m_strRootPath, FALSE);
						if(m_pChartItems[0][0])
						{
							pChartItem->SetNUseInquiryWithCrossline(m_pChartItems[0][0]->GetNUseInquiryWithCrossline());							
						}
					}	
					if(!m_pExtInputBarDlg)
					{
						//MessageBox("m_pExtInputBarDlg ERROR!");
						return FALSE;
					}
					

					BOOL bRevised = m_pExtInputBarDlg->SendMessage(UMSG_GETREVISED);
					BOOL bLogPrice	= m_pInputBarDlg ? m_pInputBarDlg->SendMessage(UMSG_GETLOG) : FALSE;		// �αװ�    ��뿩��
					
					INT  nAnalysisType = m_pExtInputBarDlg->SendMessage(UMSG_GETDRAGMODETYPE);
					
					pChartItem->SetUseRevisedValue			(bRevised);
					pChartItem->SetUseLogValue				(bLogPrice);
					
					pChartItem->EnableReceivingTimeShare	(pChartItem->m_CfgInfo.bRecvTimeShare	);
					pChartItem->EnableBoundMark				(pChartItem->m_CfgInfo.bShowHLBar		);

					pChartItem->SetBNormalizedMinuteTimeRule(pChartItem->m_CfgInfo.bRealCandle ? FALSE : TRUE);

					pChartItem->SetDragModeType				(nAnalysisType);

					m_pChartItems[nRowIndex][nColIndex] = pChartItem;
					SetChartSelected(CPoint(nColIndex,nRowIndex));					
				}
			}			
		}
		m_lCols = lColsForFuture;
		m_lRows = lRowsForFuture;
	}
	else if(lCnt<0)
	{
		if(lColsForFuture<1)	return FALSE;
		if(lRowsForFuture<1)	return FALSE;
		// delete Col First..
		for(int nRowIndex=(m_lRows-1);nRowIndex>=0;nRowIndex--)
		{
			for(int nColIndex=(m_lCols-1);nColIndex>=(lColsForFuture);nColIndex--)
			{
				pChartItem = m_pChartItems[nRowIndex][nColIndex];
				if(pChartItem)
				{					
					pChartItem->DestroyWindow();
					delete pChartItem;
					pChartItem = NULL;
					m_pChartItems[nRowIndex][nColIndex] = pChartItem;
					if(nColIndex<=m_lColSelected)
					{
						m_lColSelected = nColIndex - 1;
					}
				}
			}			
		}
		m_lCols = lColsForFuture;
		
		// delete Row Second..
		for(int nColIndex=(m_lCols-1);nColIndex>=0;nColIndex--)
		{
			for(int nRowIndex=(m_lRows-1);nRowIndex>=(lRowsForFuture);nRowIndex--)
			{
				pChartItem = m_pChartItems[nRowIndex][nColIndex];
				if(pChartItem)
				{
					pChartItem->DestroyWindow();
					delete pChartItem;
					pChartItem = NULL;
					m_pChartItems[nRowIndex][nColIndex] = pChartItem;
					if(nRowIndex<=m_lRowSelected)
					{
						m_lRowSelected = nRowIndex - 1;
					}
				}
			}			
		}
		m_lRows = lRowsForFuture;
	}
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
	{
		m_pExtInputBarDlg->SendMessage(UMSG_SETMATRIX,m_lRows,m_lCols);
	}
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��Ʈ�� ��, ���� ���� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
BOOL CChartMng::SetMatrix(long lRows,long lCols)
{
	long lColSelectedOld = m_lColSelected;
	long lRowSelectedOld = m_lRowSelected;

	long lColsDif = lCols - m_lCols;
	long lRowsDif = lRows - m_lRows;
	if(lColsDif==lRowsDif)
	{
		Rearrange(lColsDif,RESIZEBOTH);
	}
	else
	{
		if(lRowsDif>lColsDif)
		{
			Rearrange(lColsDif,RESIZECOL);
			Rearrange(lRowsDif,RESIZEROW);
		}
		else
		{
			Rearrange(lRowsDif,RESIZEROW);
			Rearrange(lColsDif,RESIZECOL);			
		}
	}
	MoveCharts();
	if(lColSelectedOld>=m_lCols||lRowSelectedOld>=m_lRows)
	{
		SetChartSelected(CPoint(0,0),TRUE);
	}

/*	// ȣ�� â Show ---------------------------------------------------
	// 2005. 04. 28 by Nam
	//-----------------------------------------------------------------
	if(lRows == 1 && lCols == 1)
	{
		HWND hParent = GetParent()->GetSafeHwnd();		
		::SendMessage(hParent, UMSG_FORCE_SHOW_HOGAWND, 1, 0 );		
	}
	//-----------------------------------------------------------------
*/
	return TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	���õ� ��Ʈ�� Pointer�� ������.
///////////////////////////////////////////////////////////////////////////////
CChartItem* CChartMng::GetpChartItemSelected()
{
	if(m_lRowSelected < 0) return NULL;
	if(m_lColSelected < 0) return NULL;

	if(m_pChartItems[m_lRowSelected][m_lColSelected]->GetSafeHwnd())
		return m_pChartItems[m_lRowSelected][m_lColSelected];
	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��Ʈ�� ���� ������Ų��.
///////////////////////////////////////////////////////////////////////////////
BOOL CChartMng::InsertChart()
{
	BOOL betValue = FALSE;
	if(m_lRows*m_lCols<20)
	{
		if(m_lRows<m_lCols)
		{
			betValue = Rearrange(1,RESIZEROW);
		}
		else
		{
			betValue = Rearrange(1,RESIZECOL);
		}
		MoveCharts();
	}
	return betValue;
}


void CChartMng::OnLButtonDown(UINT nFlags, CPoint point) 
{
	((CInputBarDlg*)m_pInputBarDlg)->m_pCodeInput->SetFocus();
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	EACH_CHARTJOB_START()
		rect = GetRectOfChart(nRowIndex,nColIndex,TRUE);
		if(rect.PtInRect(point))
		{
			pChartItem->SetOutsideCommonInterface(m_lMltchrtDlgParentClsPointer);
			//2007.01.18 by LYH ToolBar ������ ���´�. ���� ���� ����
			int m_lColSelectedOld = m_lColSelected;
			int m_lRowSelectedOld = m_lRowSelected;
			//}}

			SetChartSelected(CPoint(nColIndex,nRowIndex));

			//2007.01.18 by LYH ToolBar ������ ���´�.
			if(m_lColSelectedOld!=nColIndex || m_lRowSelectedOld != nRowIndex)
			{
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 0, 50000);
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50031, pChartItem->m_bDrawAutoTrendLine );
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50032, pChartItem->m_bDrawAutoPattern );
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50033, pChartItem->m_bDrawCandlePattern );
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50026, pChartItem->m_bDrawNews );

				if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
					m_pExtInputBarDlg->SendMessage(UMSG_SYNC_TOOLBAR, CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL, 0);
				pChartItem->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::T_ALL_TOOL_TRUE_QUERY, 0);
			}
			break;
		}
	EACH_CHARTJOB_END()	
	m_bClicked = TRUE;
	CWnd::OnLButtonDown(nFlags, point);
}

void CChartMng::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bClicked)
	{
		m_bClicked = FALSE;
		ReleaseCapture();
		if(m_bDragNDrop)	// �巡�� ���̾�����
		{
			//CChartItem* pLastChartItem = NULL;

			CRect rect;
			EACH_CHARTJOB_START()
				rect = GetRectOfChart(nRowIndex,nColIndex,2);
				if(rect.PtInRect(point))
				{
					ReplaceChart(m_lRowDragFrom,m_lColDragFrom, nRowIndex, nColIndex);
					SetChartSelected(CPoint(nColIndex,nRowIndex),TRUE);
					//2005. 07. 18  �巡�� ���� ������ ���õ� ChartItem�� �����Ѵ�.==========
					//�׽�Ʈ �� ���� �ʿ�..  
					//pLastChartItem = pChartItem; 
					//=======================================================================
					break;
				}
			EACH_CHARTJOB_END()
		}
		m_bDragNDrop = FALSE;
	}		
	CWnd::OnLButtonUp(nFlags, point);
}

void CChartMng::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bClicked)
	{
		// �巡������ ǥ��
		if(!m_bDragNDrop)
		{
			m_lColDragFrom = m_lColSelected;
			m_lRowDragFrom = m_lRowSelected;
			SetCapture();
		}
		m_bDragNDrop = TRUE;
		CPoint ptDrag(point);
		ClientToScreen(&ptDrag);
		GetParent()->GetParent()->GetParent()->SendMessage(RMSG_DRAGSTART, (WPARAM)&ptDrag, (LPARAM)m_hWnd);
	}	

	CWnd::OnMouseMove(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��Ʈ�� �ʱⰪ���� �ٲ۴�.
//////////////////////////////////////////////////////////////////////////////
long CChartMng::ResetChart()
{
	if(m_bJongmokLink)
	{
		EACH_CHARTJOB_START()
			pChartItem->ResetChart();
		EACH_CHARTJOB_END()
	}
	else
	{
		CChartItem* pChartItem = NULL;
		pChartItem = GetpChartItemSelected();
		if(pChartItem == NULL) 
			return 1L;

		GetpChartItemSelected()->ResetChart();
	}
	return 1L;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��Ʈ�� �ʱ�ȭ�Ѵ�.
//////////////////////////////////////////////////////////////////////////////
void CChartMng::ResetOCX()
{
	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();
	if(pChartItem == NULL) 
		return;

	GetpChartItemSelected()->ResetOCX();
}

void CChartMng::InitInLoop()
{	
	m_bDontRequestDataInLoop = NULL;
	m_bUseRevisedInLoop = NULL;
	m_bNormalizedMinuteTimeRuleInLoop = NULL;
	//m_bUseRateValueInLoop = NULL;
	m_nChartMngs_ChartDataNumericValue = WON_TYPE;
	m_pChartItemInLoop = NULL;

	m_strChartSelectedInLoop.Empty();
	m_strCodeInLoop.Empty();
	m_strEndDateInLoop.Empty();
	m_chTypeInLoop = NULL;
	m_lNumberInLoop = NULL;
	m_lZoomValueInLoop = NULL;
	m_dEndDateInLoop = NULL;
	m_lTypeInLoop = LOOPTYPE_NULL/*0*/;
}

BOOL CChartMng::PostInLoop()
{
	SetChartSelected(CPoint(m_lColSelectedInLoop,m_lRowSelectedInLoop));
//@solomon	��/�� SingleChart ���ἱ�� ��ȸ ���� �Ǵ� ���󶧹��� ����.
// 	if(m_bJongmokLink)
// 	{
// 		if(m_pInputBarDlg)
// 		{
// 			CString strCode = ((CInputBarDlg*)m_pInputBarDlg)->m_pCodeInput->GetCode();
// 			((CInputBarDlg*)m_pInputBarDlg)->m_pCodeInput->SetCode(strCode);
// 		}
// 	}

	if(m_lTypeInLoop==LOOPTYPE_UPDATE_BY_EXTEND/*5*/)
	{
		m_bUpdatedInChartExtended = FALSE;

		// (2008/8/10 - Seung-Won, Bae) Support UpdateIndex in Maxized Chart Item.
		if( m_bUpdateIndexAfterExtended)
		{
			PostMessage( UMSG_UPDATEINDEX, 0);
			m_bUpdateIndexAfterExtended = FALSE;
		}
	}

	return TRUE;
}

BOOL CChartMng::TailInLoop()
{
	if(!m_bIsInLoop) 
	{
		if(((CStdDialog*)GetParent())->m_nScreenID == nScreenID_6401)
		{
			SendPrevDateData();
			return FALSE;				
		}

		CChartItem *pChartItem = NULL;
		for(int nRowIndex=0;nRowIndex<m_lRows;nRowIndex++)
		{  
			for(int nColIndex=0;nColIndex<m_lCols;nColIndex++)
			{  
				pChartItem = m_pChartItems[nRowIndex][nColIndex];  
				if(pChartItem && pChartItem->IsEmpty())
				{
					pChartItem->ResetOCX();
				}
			}
		}

		return FALSE;
	}
	if(!HeadInLoop())
	{
		PostInLoop();
	}
	return TRUE;
}

BOOL CChartMng::HeadInLoop()
{
	if (!m_bIsInLoop)
	{	
		m_lColSelectedInLoop = m_lColSelected;
		m_lRowSelectedInLoop = m_lRowSelected;
		m_nRowIndexInLoop = 0;
		m_nColIndexInLoop = 0;
		m_bIsInLoop = TRUE;
	}
	else
	{	
		if (m_pChartItemInLoop != NULL) 
		{
			switch (m_lTypeInLoop)
			{
			case LOOPTYPE_LOAD/*1*/: // Load >>> m_strChartSelectedInLoop : ���ϸ�.	
				
				m_pChartItemInLoop->LoadChartSetting(m_strChartSelectedInLoop);
				if (m_pInputBarDlg)
				{
					if (m_pChartItemInLoop->GetBWholeView())
						m_pInputBarDlg->SendMessage(UMSG_SETZOOMVALUE,m_pChartItemInLoop->GetAllCnt());
					else
					{
						//m_pInputBarDlg->SendMessage(UMSG_SETZOOMVALUE,m_pChartItemInLoop->GetNOnePageDataCount());
						long lZoomValue = m_pInputBarDlg ? m_pInputBarDlg->SendMessage(UMSG_GETZOOMVALUE,m_pChartItemInLoop->GetType()) : 0;	
						if (lZoomValue > m_pChartItemInLoop->GetAllCnt())
							m_pChartItemInLoop->SetNew_OnePageDataCount(m_pChartItemInLoop->GetAllCnt());
						else
							m_pChartItemInLoop->SetNew_OnePageDataCount(lZoomValue);
						//2007.02.06 by LYH �ҷ����� �� �������� veiwcount �� �´� ���� ����
						//((CInputBarDlg*)m_pInputBarDlg)->SetViewCount(m_pChartItemInLoop->GetNOnePageDataCount());
						//}}
					}

					if (((CStdDialog*)GetParent())->m_nScreenID == nScreenID_6401)
					{
						m_bIsInLoop = FALSE;
						SendPrevDateData();
						return FALSE;				
					}
				}
				break;
			case LOOPTYPE_FIXDATE/*2*/:// fixdate
				m_pChartItemInLoop->ZoomToWithRightFix((short)m_lZoomValueInLoop,TRUE);
				//@solomon	100104SM003
				m_pChartItemInLoop->SetNew_OnePageDataCount(m_lZoomValueInLoop);
				break;
			case LOOPTYPE_UPDATE/*4*/:
				if (m_pChartItemInLoop!=m_pChartItemSelectedInLoop)
				{
					if (m_pChartItemInLoop->GetAllCnt() &&
						m_pChartItemInLoop->GetCntOfItemData() &&
						m_pChartItemInLoop->GetType()==m_chTypeInLoop) 
					{
						m_pChartItemInLoop->SaveandOpenFromFile("temp.tmp","",TRUE,FALSE);
						m_pChartItemInLoop->InvalidateType();
					}				
				}
				break;
			default:
				break;
			}
		}
		m_nColIndexInLoop++;
		if (m_nColIndexInLoop>=m_lCols)
		{
			m_nColIndexInLoop = 0;
			m_nRowIndexInLoop++;
			if (m_nRowIndexInLoop>=m_lRows)			
			{
				m_bIsInLoop = FALSE;

				if (m_lTypeInLoop == LOOPTYPE_LOAD && !m_bLastEnvUpdated)
					SetFocusToJMEdit_At_LoadEnd();

				return FALSE;
			}
		}
	}
	m_pChartItemInLoop = m_pChartItems[m_nRowIndexInLoop][m_nColIndexInLoop];
	if (!m_pChartItemInLoop)
	{	
		if (!HeadInLoop())
			PostInLoop();

		return TRUE;
	}
	SetChartSelected(CPoint(m_nColIndexInLoop,m_nRowIndexInLoop), FALSE, TRUE);

	//05. 09. 07 ==============================================================
	// ���Ͽ��� �о���� ������ ���� ��� ������ ������ 
	// �ڵ带 ��Ʈ�� �����Ű�� ���� LOOPTYPE_LOAD�ÿ���  ����� 
	// �޾Ƽ� FALSE�� ��� Return��Ų��.
	// sy, nam 
	BOOL bResultOfLoopTypeLoad = FALSE; 
	//=========================================================================

	CChartItem* pSelItem = NULL;
	switch (m_lTypeInLoop)
	{
	case LOOPTYPE_LOAD/*1*/:
		{
			m_nStatic_RecvCntShareCode++;
			m_nDWMChartIndex++;
		
		//	BOOL bDWMchart = ((CStdDialog*)GetParent())->m_nScreenID == nScreenID_6301;  KHD : �ε��ϵ��� ���� 

//			if (bDWMchart && m_nDWMChartIndex > 1)
//			{
//				GoNextChart();
//				break;
//			}

			bResultOfLoopTypeLoad = m_pChartItemInLoop->LoadChartInfo(m_strChartSelectedInLoop);
			if (bResultOfLoopTypeLoad == FALSE) 
			{
		//		if (bDWMchart)//�ε��ϵ��� ���� 
		//		{
		//			GoNextChart();
		//			return FALSE;				
		//		}
				
				PostInLoop();
				m_bIsInLoop = FALSE;
				m_bLastEnvUpdated = TRUE;
				return FALSE;				
			}
		}
		break;
	case LOOPTYPE_FIXDATE/*2*/:
		{
			if (m_pChartItemInLoop->GetType() != DAY_DATA_CHART	&&
				m_pChartItemInLoop->GetType() != WEEK_DATA_CHART	&&
				m_pChartItemInLoop->GetType() != MONTH_DATA_CHART	&&
				m_pChartItemInLoop->GetType() != YEAR_DATA_CHART
//				m_pChartItemInLoop->GetType() != SEC10_DATA_CHART	&& //05.09.28
//				m_pChartItemInLoop->GetType() != SEC30_DATA_CHART //05.09.28
				)
			{
				if (!HeadInLoop())	PostInLoop();
				return TRUE;
			}

			if (m_pChartItemInLoop->ScrollToDataOfEnd(m_dEndDateInLoop))
			{
				if (!HeadInLoop())	PostInLoop();
				return TRUE;
			}
			m_pChartItemInLoop->SetEndDateRequested((LPCTSTR)m_strEndDateInLoop);
			m_pChartItemInLoop->SetEndDate((LPCTSTR)m_strEndDateInLoop); //05.09.06 added by sy, nam
		}
		break;
	case LOOPTYPE_REVISED/*3*/:
		pSelItem = GetpChartItemSelected();
		if (pSelItem == NULL) 
			break;

		pSelItem->SetUseRevisedValue(m_bUseRevisedInLoop);
		if (m_bDontRequestDataInLoop) 
		{
			if (!HeadInLoop())	PostInLoop();
			return TRUE;
		}
		break;
	case LOOPTYPE_UPDATE/*4*/:
		{
			//@Solomon:091116SM044 ��ü�ʱ�ȭ�� �� ȭ�鿡���� ��ǥ����ȭ�� �ȵǴ� �������.
			//if (!m_pChartItemInLoop->GetAllCnt() ||
			if (!m_pChartItemInLoop->GetCntOfItemData() ||
				m_pChartItemInLoop==m_pChartItemSelectedInLoop ||
				m_pChartItemInLoop->GetType()!=m_chTypeInLoop)
			{
				if (!HeadInLoop())	PostInLoop();
				return TRUE;
			}

			m_pChartItemInLoop->ClearDataExceptPrimaryData();
			
			if (m_pChartItemInLoop->GetChart_NumericValueType() != m_nChartMngs_ChartDataNumericValue)	
				m_pChartItemInLoop->SetChartNumericValue(m_nChartMngs_ChartDataNumericValue);

			if (m_pChartItemInLoop->GetBNormalizedMinuteTimeRule() != m_bNormalizedMinuteTimeRuleInLoop)
				m_pChartItemInLoop->SetBNormalizedMinuteTimeRule(m_bNormalizedMinuteTimeRuleInLoop );

			CString strTRCode, strCode;
			for (long lPos = 0;lPos<(m_lCntInLoop-1);lPos++)
			{
				strCode = m_strArrayCodeInLoop.GetAt(lPos);
				strTRCode = m_strArrayTRCodeInLoop.GetAt(lPos);
				m_pChartItemInLoop->SetCodeForQuery(strCode, strTRCode, FALSE);
			}
		}
		break;
	case LOOPTYPE_UPDATE_BY_EXTEND/*5*/:
		m_bNeedToUpdateInLoop = FALSE;
		if (m_pChartItemInLoop->GetUseRevisedValue() != m_bUseRevisedInLoop)
		{
			m_pChartItemInLoop->SetUseRevisedValue(m_bUseRevisedInLoop);
			m_bNeedToUpdateInLoop = TRUE;
		}

		if (m_pChartItemInLoop->GetBNormalizedMinuteTimeRule() != m_bNormalizedMinuteTimeRuleInLoop)
		{
			m_pChartItemInLoop->SetBNormalizedMinuteTimeRule(m_bNormalizedMinuteTimeRuleInLoop);
			m_bNeedToUpdateInLoop = TRUE;
		}

		if (m_pChartItemInLoop->GetDataCode()!=m_strCodeInLoop && m_bJongmokLink)
		{
			CString strQry = QUERY_STRJONGMOK;
			int nRealMarketType;
			int nMarketType = ((CStdDialog*)GetParent())->GetMarketTypeFromCode(m_strCodeInLoop, nRealMarketType);

			switch (nMarketType)
			{
			case CMasterTypeInfo::STOCK_CHART:			strQry = QUERY_STRJONGMOK;		break;
			case CMasterTypeInfo::UPJONG_CHART:			strQry = QUERY_STRUPJONG;		break;
			case CMasterTypeInfo::FUTOPT_CHART:			strQry = QUERY_STRFUTOPT;		break;
			case CMasterTypeInfo::STOCKFUTOPT_CHART:	strQry = QUERY_STRSTOCKFUTOPT;	break;
			case CMasterTypeInfo::FOREIGN_CHART:		strQry = QUERY_STRFOREIGN;		break;
			case CMasterTypeInfo::FUOPT_JIPYO_CHART:	strQry = QUERY_STRFOJIPYO;		break;
			}

			m_pChartItemInLoop->SetCodeForQuery(m_strCodeInLoop, strQry, TRUE);
			m_pChartItemInLoop->SetEndDateRequested(m_strEndDateInLoop);
			m_pChartItemInLoop->SetEndDate(m_strEndDateInLoop); //05.09.06 added by sy, nam
			m_bNeedToUpdateInLoop = TRUE;
		}

		if ((m_pChartItemInLoop->GetType()!=m_chTypeInLoop ||
			m_lTickMinNumberInLoop!=atol(m_pChartItemInLoop->GetTickMinDayNumber())))
		{
			if (m_bTypeLink)
			{
				m_pChartItemInLoop->SetType(m_chTypeInLoop);
				m_pChartItemInLoop->SetTickMinDayNumber(m_lTickMinNumberInLoop);
				m_bNeedToUpdateInLoop = TRUE;
			}
		}

		if (!m_bNeedToUpdateInLoop)
		{
			if (!HeadInLoop())	PostInLoop();
			return TRUE;				
		}
		break;
	case LOOPTYPE_RATE_VALUE/*6*/:// RateValue
		pSelItem = GetpChartItemSelected();
		if (pSelItem == NULL)
			break;

		//2005. 06. 22
		pSelItem->SetChartNumericValue(m_nChartMngs_ChartDataNumericValue);
		if (m_bDontRequestDataInLoop)
		{
			if (!HeadInLoop())	PostInLoop();
			return TRUE;
		}
		break;
	case LOOPTYPE_FAKE_CANDLE/*7*/:// ShowFakedCandle
		pSelItem = GetpChartItemSelected();
		if (pSelItem == NULL)
			break;

		pSelItem->SetBNormalizedMinuteTimeRule(m_bNormalizedMinuteTimeRuleInLoop);
		if (m_bDontRequestDataInLoop)
		{
			if (!HeadInLoop())	PostInLoop();
			return TRUE;
		}
		break;
	default:
		pSelItem = GetpChartItemSelected();
		if (pSelItem == NULL) 
			break;

		if (!m_strCodeInLoop.IsEmpty())
		{
			// ���񿬵��� �ٸ� ����Ÿ���� ��� Ÿ���� �����Ŵ
			if (((CStdDialog*)GetParent())->m_nChartType == MULTI_CHART && m_bJongmokLink)
			{
				int nRealMarketType;
				int nMarketType = ((CStdDialog*)GetParent())->GetMarketTypeFromCode(m_strCodeInLoop, nRealMarketType);
				if (m_nMarketType != nMarketType)
				{
					m_nMarketType = nMarketType;
					((CStdDialog*)GetParent())->ChangeMarket(m_nMarketType);
				}
			}

			BOOL bSetQry = TRUE;
			CString strQry = QUERY_STRJONGMOK;
			int nLen = m_strCodeInLoop.GetLength();
			switch (m_nMarketType)
			{
			case CMasterTypeInfo::STOCK_CHART:
				if (nLen == CODE_STOCK_LENGTH)		strQry = QUERY_STRJONGMOK;	break;
			case CMasterTypeInfo::UPJONG_CHART:
				if (nLen == UPJONG_LENGTH)			strQry = QUERY_STRUPJONG;	break;
			case CMasterTypeInfo::FUTOPT_CHART:
			case CMasterTypeInfo::COMMODITY_CHART:
				if ((nLen == FUTOPT_LENGTH) ||
					(nLen == KQOPT_LENGTH12)||
					(nLen == KQFUT_LENGTH ) ||
					(nLen == KQOPT_LENGTH))			strQry = QUERY_STRFUTOPT;	break;
			case CMasterTypeInfo::STOCKFUTOPT_CHART:
				if ((nLen == FUTOPT_LENGTH) ||
					(nLen == KQOPT_LENGTH12)||
					(nLen == KQFUT_LENGTH)	||
					(nLen == KQOPT_LENGTH))			strQry = QUERY_STRSTOCKFUTOPT;	break;
			case CMasterTypeInfo::FOREIGN_CHART:	strQry = QUERY_STRFOREIGN;	break;
			case CMasterTypeInfo::FUOPT_JIPYO_CHART:strQry = QUERY_STRFOJIPYO;	break;
			default:		bSetQry = FALSE;		break;
			}

			if (bSetQry)	pSelItem->SetCodeForQuery(m_strCodeInLoop, strQry, TRUE);
		}

		if (!m_strEndDateInLoop.IsEmpty())
			pSelItem->SetEndDate(m_strEndDateInLoop);

		int nRealMarketType;
		int nMarketType = ((CStdDialog*)GetParent())->GetMarketTypeFromCode(pSelItem->GetDataCode(), nRealMarketType);
		if (nMarketType == CMasterTypeInfo::FOREIGN_CHART || m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
		{
			if (m_chTypeInLoop == MIN_DATA_CHART || pSelItem->GetType() == MIN_DATA_CHART)
				pSelItem->ResetOCX();
		}

		if (m_chTypeInLoop!=NULL)
		{
			if( (nMarketType == CMasterTypeInfo::UPJONG_CHART && m_chTypeInLoop == SEC_DATA_CHART) ||
				(nMarketType == CMasterTypeInfo::FOREIGN_CHART && (m_chTypeInLoop == MIN_DATA_CHART || m_chTypeInLoop == SEC_DATA_CHART)))
				pSelItem->SetType(DAY_DATA_CHART);
			else
				pSelItem->SetType(m_chTypeInLoop);
		}
		if (m_lNumberInLoop!=NULL)			pSelItem->SetTickMinDayNumber(m_lNumberInLoop);
		break;
	}

	if (!RequestChartData())
	{
		if (!HeadInLoop())	PostInLoop();
	}

	return TRUE;
}


BOOL CChartMng::InputCodeFromInputBar(CCodeDataFromInputBar* pCodeData,BOOL bForcedMode0 /*=FALSE*/)
{
// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CChartMng::InputCodeFromInputBar:Entry\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}

	if (((CStdDialog*)GetParent())->m_nChartType==SINGLE_CHART)
		m_bJongmokLink = TRUE;

	m_bNeedForceCode = FALSE; // ������ ���Ա� ������ InputBar�ʿ��� ������ �ڵ带 ���� �ʿ䰡 ���ٰ� �����Ѵ�.
	 
	if (!m_bIsSystemTradingMode)
	{
		//nOpenedOwner  = 0 : �޴�/���ٿ��� ����
		//nOpenedOwner  = 1 : ���콺 ������ ��ư���� ���� (�����ڵ� ����)
		//nOpenedOwner  = 2 : ��ũ��Ʈ,dll���� ȭ�� ��û (OpenMapData ����)		
		//UINT nOpenedOwner = g_pViewInMain->SendMessage(RMSG_GETOPENEDOWNER);
//		UINT nOpenedOwner = ::SendMessage(((CStdDialog*)GetParent())->m_hViewInMain,RMSG_GETOPENEDOWNER,0,0);
		if (!m_bLastEnvUpdated)
		{
			m_strStatic_ShareCode = pCodeData->m_strCode; //Static���� �� �ѹ� �����Ѵ�.

			BOOL bResult = FALSE;
			//bResult = LoadChart(((CStdDialog*)GetParent())->m_strConfig);
			bResult = LoadChart(m_pStdDialog->m_strConfig);

			if (!bResult) //�ε��� ������ ���� ��� �ε尡 �����ٴ� �÷��׸� �ݵ�� ���ش�.
			{
				m_bLastEnvUpdated = TRUE;
				SetBNoExistLoadCnts(TRUE); // �ε� �� �� ���� ����.
			}
			m_bLastEnvUpdated = TRUE;

			m_strStatic_ShareCode = _T("");

			if (bResult)		//�ε� �����ϸ� �ٷ� 
				return TRUE;	//Return �� ������.
		}
		else 
		{
			m_bLastEnvUpdated = TRUE;
		}
	}

	CChartItem* pSelItem = GetpChartItemSelected();
	//{{ 2007.07.06 by LYH Ư����Ʈ�� ��� ���񺯰� ���� �ٲ�
	if (pSelItem)
	{
		if (pSelItem->GetOnRunningOneChart())
			((CInputBarDlg*)m_pInputBarDlg)->SetMode(CHANGE_ITEM);
	}
	//}}

	CString strEndDate;
	CString strCode;
	char chType = NULL;
	strEndDate.Format("%d",pCodeData->m_lEndTime);
	strCode = pCodeData->m_strCode;
	if (m_bJongmokLink&&!m_bChartExtended)
	{
		if (!GetBInitFromExtToolBar()) //�ʱ�ȭ ���°� �ƴҶ��� �����Ų�� . 05.11.24
		{
			if (m_nMode == ADD_NEW_CHART &&!bForcedMode0)
			{
				if (!SelectEmptyPos())	
				{
					if (!InsertChart())
					{
						m_strMultiCodes.Empty();
						return FALSE;
					}
					SelectEmptyPos();
				}
			}	
		}

		SetBInitFromExtToolBar(FALSE); //�ʱ�ȭ�̴� �ƴϴ� �⺻���� False�� ���ش�.05.11.24

		if(m_bIsInLoop == FALSE)
		{
			InitInLoop();
			m_strEndDateInLoop = strEndDate;
			m_strCodeInLoop = pCodeData->m_strCode;
			m_lTypeInLoop = LOOPTYPE_NULL/*0*/;
			HeadInLoop();

		}
		
		pSelItem = GetpChartItemSelected();
		//2005. 05. 19 add by sy, nam ===================================
		//��Ʈ�� ��ø��尡 �ƴ� ��� ChartItem�� ��ø���� �κ��� �ʱ�ȭ 
	
		if (!pSelItem)
			return FALSE;
		if (m_nMode != OVERLAPP_ITEM)
			pSelItem->Init_OverLapped();
		else
			pSelItem->Add_OverLappedCnt();
		//===============================================================
	}
	else
	{	
		if (!GetBInitFromExtToolBar()) //�ʱ�ȭ ���°� �ƴҶ��� �����Ų�� . 05.11.24
		{
			m_nMode = ((CInputBarDlg*)m_pInputBarDlg)->GetCurrentMode();
//>> [QC No]090731HMC070
//			if(m_nMode == ADD_NEW_CHART)
			if(m_nMode == ADD_NEW_CHART && !m_bNoInsertChart)
//<< [QC No]090731HMC070
			{
				if (!SelectEmptyPos())	
				{
					if (!InsertChart())
					{
						m_strMultiCodes.Empty();
						return FALSE;
					}
					SelectEmptyPos();
				}
			}
		}
		
		SetBInitFromExtToolBar(FALSE); //�ʱ�ȭ�̴� �ƴϴ� �⺻���� False�� ���ش�.05.11.24

		pSelItem = GetpChartItemSelected();
		//2005. 05. 19 add by sy, nam ===================================
		//��Ʈ�� ��ø��尡 �ƴ� ��� ChartItem�� ��ø���� �κ��� �ʱ�ȭ 
		if (pSelItem == NULL)
			return FALSE;

		if (m_nMode != OVERLAPP_ITEM)
			pSelItem->Init_OverLapped();
		else
			pSelItem->Add_OverLappedCnt();
		//===============================================================

//@Solomon: TRCode���ϱ� ����.-->
//Before
// 		CString strQry = QUERY_STRJONGMOK;
// 		BOOL bSetQry = TRUE;
// 		int nLen = strCode.GetLength();
// 		switch (m_nMarketType)
// 		{
// 		case CMasterTypeInfo::STOCK_CHART:
// 			if (nLen == CODE_STOCK_LENGTH)		strQry = QUERY_STRJONGMOK;	break;
// 		case CMasterTypeInfo::UPJONG_CHART:
// 			if (nLen == UPJONG_LENGTH)			strQry = QUERY_STRUPJONG;	break;
// 		case CMasterTypeInfo::FUTOPT_CHART:
// 			if ((nLen == FUTOPT_LENGTH) ||
// 				(nLen == KQOPT_LENGTH12)||
// 				(nLen == KQFUT_LENGTH ) ||
// 				(nLen == KQOPT_LENGTH))			strQry = QUERY_STRFUTOPT;	break;
// 		case CMasterTypeInfo::STOCKFUTOPT_CHART:
// 			if ((nLen == FUTOPT_LENGTH) ||
// 				(nLen == KQOPT_LENGTH12)||
// 				(nLen == KQFUT_LENGTH)	||
// 				(nLen == KQOPT_LENGTH))			strQry = QUERY_STRSTOCKFUTOPT;	break;
// 		case CMasterTypeInfo::FOREIGN_CHART:	strQry = QUERY_STRFOREIGN;	break;
// 		case CMasterTypeInfo::FUOPT_JIPYO_CHART:strQry = QUERY_STRFOJIPYO;	break;
// 		default:
// #ifdef _DEBUG
// 			AfxMessageBox("InputCodeFromInputBar == ��Ʈ ���а��� ����");
// #endif
// 			bSetQry = FALSE;
// 			break;
// 		}
//		if (bSetQry)	pSelItem->SetCodeForQuery(pCodeData->m_strCode, strQry, TRUE);

//After

//@Solomon: TRCode���ϱ� ����.<--
		CString strQry = QUERY_STRJONGMOK;

		int nCodeType;
		int nMarketType = ((CStdDialog*)GetParent())->GetMarketTypeFromCode(pCodeData->m_strCode, nCodeType);
		if(g_pPtUtilMng)
		{
			CString szTRCode;
			g_pPtUtilMng->GetTRCodeFromCode(nMarketType, "", strQry);
		}

		pSelItem->SetCodeForQuery(pCodeData->m_strCode, strQry, TRUE);

		//pSelItem->SetEndDateRequested(strEndDate);
		pSelItem->SetEndDate(strEndDate);

		// gm0604 added For SystemTrading 2005.7.21 Start
		if (pCodeData->m_chType)
		{			
			pSelItem->SetType(pCodeData->m_chType);

			//edited by sy, nam 05. 08. 29 =============================================
			CString strTypeNum;
			strTypeNum.Format("%03d", pCodeData->m_lTypeNum);

			//pSelItem->SetTickNumber(strTypeNum);
			//pSelItem->SetMinNumber(strTypeNum);
			if (pCodeData->m_lTypeNum)
				pSelItem->SetTickMinDayNumber(pCodeData->m_lTypeNum);
			//==========================================================================
		}

		if (pCodeData->m_lDataCnt)
			pSelItem->SetCntForRequest(pCodeData->m_lDataCnt);

		if (pCodeData->m_lPtrST)
			pSelItem->SetPtrST(pCodeData->m_lPtrST,pCodeData->m_strSTName);			
		// gm0604 added For SystemTrading 2005.7.21 End

		//========================================================================================
		// 2005. 09. 23
		ApplySyncEnv_OnlyOne();
		//========================================================================================

		//========================================================================================
		// 2005. 09. 23
		m_bChangeTypeMode = FALSE;
// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
		//Synchro_jipyo();
		LPCTSTR lpszCodeForRequest = (LPCTSTR)pCodeData->m_strCode;
		Synchro_jipyo(m_bChangeTypeMode, lpszCodeForRequest);
		//========================================================================================

#ifdef _WRITELOG
		// Log - ojtaso (20080107)
		((CStdDialog*)GetParent())->WriteToStringLog(_T("��Ʈ���� �����ϴ� Data ����"), pCodeData->m_strCode, pCodeData->m_strCode.GetLength());
#endif
//>> [QC No]090731HMC070
		RequestChartData(bForcedMode0 ? TRUE : FALSE);
//<< [QC No]090731HMC070
	}

	return TRUE;
}

BOOL CChartMng::SetType(char chType, long lNumber)
{
	// ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5)/��(6)
	if(m_bTypeLink&&!m_bChartExtended)
	{
		InitInLoop();
		m_chTypeInLoop = chType;
		m_lNumberInLoop = lNumber;
		HeadInLoop();
	}
	else
	{
		if (!GetpChartItemSelected())
			return FALSE;

		GetpChartItemSelected()->SetType(chType);
		
		// 2005. 05. 20 -- �� ��Ʈ�� ������ �ѱ��.
		if (chType == TICK_DATA_CHART || chType == SEC_DATA_CHART || chType == MIN_DATA_CHART || 
			chType == DAY_DATA_CHART || chType == HOUR_DATA_CHART)
		{
			GetpChartItemSelected()->SetTickMinDayNumber(lNumber);
		}

		if (!((CStdDialog*)GetParent())->IsMultiItemChart())
			m_bChangeTypeMode = TRUE;

		Synchro_jipyo();

		// �������� �߰� : �������� - ojtaso (20070118)
		RequestChartData(TRUE);
	}
	return TRUE;
}

#undef OBJECT_SELECT
#include "../chart_common/BlockBaseEnum.h"

LRESULT CChartMng::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(((CStdDialog*)GetParent())->IsChartHide())
	{
		switch(message)
		{
		// �����ڵ� �Է½� ��ȸó��
		case UMSG_INPUTCODETOCHARTMNG:
			{
				STINPUTCODEDATA* pStInputCodeData = (STINPUTCODEDATA*)lParam;
				CString strCode = pStInputCodeData->m_strCode;
				UINT nDefault = pStInputCodeData->m_nDefault;
				UINT nMode = pStInputCodeData->m_nMode;
				nMode = ((CInputBarDlg*)m_pInputBarDlg)->GetCurrentMode(nDefault, nMode);
				GetParent()->SendMessage(UMSG_SENDSHAREDCODE, (WPARAM)(LPCTSTR)strCode);
			}
			break;
		case UMSG_GETJCODECHECKEDFROMCHART:// keep going...			
			break;
		case UMSG_GETINC:
			return 0L; // return 0(send mode)..
			break;
		default:
			return 1L; // don't do antying..
			break;
		}
	}
	CString sTemp;
	switch(message)
	{
	case WM_COMMAND:
		{
			int nMenuID = (int)wParam;
			if(nMenuID >= ID_PLM_ADD_MENU && nMenuID <= ID_PLM_ADD_MENU_END)
			{
				if(nMenuID >= ID_PLM_ADD_MENU_STARTANALTOOL && nMenuID <= ID_PLM_ADD_MENU_ENDANALTOOL)
				{
					//int nToolBtnID = nMenuID - ID_PLM_ADD_MENU + ID_TOOLMENU_BASE;
					int nToolBtnID = nMenuID - ID_PLM_ADD_MENU_STARTANALTOOL;
					SetDlgToolBtnToggle(nToolBtnID, TRUE);
				}
				else
					DoSiteMenuCall(nMenuID);
				return 1L;
			}

			else
			{
				int nBnClicked = HIWORD(wParam);
				int nBtnID = LOWORD(wParam);
				if(lParam && nBnClicked == BN_CLICKED)
					return CWnd::WindowProc(message, wParam, lParam);
			}
		}
		break;
	// �����ڵ� �Է½� ��ȸó��
	case UMSG_INPUTCODETOCHARTMNG:
		{
			STINPUTCODEDATA* pStInputCodeData = (STINPUTCODEDATA*)lParam;
			CString strCode = pStInputCodeData->m_strCode;
			UINT nDefault = pStInputCodeData->m_nDefault;
			UINT nMode = pStInputCodeData->m_nMode;
			nMode = ((CInputBarDlg*)m_pInputBarDlg)->GetCurrentMode(nDefault, nMode);
			
			if(((CStdDialog*)GetParent())->IsMultiItemChart())
			{
				SetMode(nMode);
				
				if(nMode == CHANGE_ITEM || nMode == REPLAY_ITEM)
				{
					m_bAddModeFromTab = FALSE;
				}
				else if(nMode == NEW_ITEM)
				{
					m_bAddModeFromTab = TRUE;
				}
			}

			STINPUTCODEDATA2	stInputCodeData2;
			stInputCodeData2.m_strCode = pStInputCodeData->m_strCode;
			stInputCodeData2.m_bDontPutInShared = FALSE;
			if(m_bAddModeFromTab)
				stInputCodeData2.m_bRequestWithoutHesitation = TRUE;
			else
				stInputCodeData2.m_bRequestWithoutHesitation = FALSE;
			
			CChartItem* pInitChartItem = GetpChartItemSelected();
			if(pInitChartItem && !m_bOnEventMessage && pInitChartItem->m_bNowInitialState)
			{
				pInitChartItem->m_bNowInitialState = FALSE;
				stInputCodeData2.m_bRequestWithoutHesitation = TRUE;
			}

			if(m_pInputBarDlg)
				m_pInputBarDlg->SendMessage(UMSG_INPUTCODETOINPUTBAR,(WPARAM)&stInputCodeData2);			
			if(pStInputCodeData->m_strTime != "")
			{
				m_strTimeMarkReserved = pStInputCodeData->m_strTime;
			}	
//			if(wParam != NULL)
//			{
//				m_nMode = m_nOldMode;
//				m_nOldMode = -1;
//			}
		}
		break;
	case UMSG_INPUTCODEFROMINPUTBAR:
		{
//			m_bLastEnvUpdated = TRUE;
			InputCodeFromInputBar((CCodeDataFromInputBar *)wParam);

			//100222 History & Share
			//	20130518 �̻�� : MiniTickBarDlg �� �ֱ⺯������ ��ȸ�� ���� ��������� ������� ����.
			if (lParam == 0)
			{
				CCodeDataFromInputBar* pCodeData = (CCodeDataFromInputBar *)wParam;
				CStdDialog* pStdDialog = ((CStdDialog*)GetParent());
				if(pStdDialog)
				{
					pStdDialog->DoShareCode(pCodeData->m_strCode);
					if(g_pPtUtilMng)
							g_pPtUtilMng->AddToHistory(pStdDialog->m_hPlatform, pCodeData->m_strCode);
				}

			}
		}
		break;
	case UMSG_RECV_MARKET_TR:
		{
			//���� ��ǥ TR ����.
			CString sChatID;
			sChatID.Format("%8.8s", (char*)lParam);
			UINT nWndID = atoi(sChatID);
			CChartItem* pRcvChartItem = NULL;
			EACH_CHARTJOB_START()
				if(pChartItem->GetDlgCtrlID() == nWndID)
				{
					pRcvChartItem = pChartItem;
					goto ExitLoop3;
				}
			EACH_CHARTJOB_END();
ExitLoop3:
			//pRcvChartItem = GetpChartItemSelected();
			if(pRcvChartItem == NULL) return FALSE;

			if(!((long)wParam))
			{
				pRcvChartItem->SetNext(FALSE);

				//==========================================================
				for(int i = 0; i < pRcvChartItem->m_arrayMarketTRData.GetSize();i++)
				{
					CChartItemData* pChartItemData = NULL;
					pChartItemData = pRcvChartItem->m_arrayMarketTRData.GetAt(i);
					if(pChartItemData)
						pChartItemData->SetBNextMarketData(FALSE);
				}
				//==========================================================


				return TRUE;
			}


			pRcvChartItem->ReceiveMarketChartData((char*)lParam, (long)wParam);	


			//================================================================================
			//*/
			if(m_bLastEnvUpdated == FALSE) //�ε尡 �� �� ������ ���.
			{
				SetFocusToJMEdit_At_LoadEnd();
			}

			//================================================================================
		}
		break;
	case UMSG_RECEIVEFROMSERVER:
		{
// 			{
// 				CString szDebug;
// 				szDebug.Format("[DRCS][LOAD](%8d) CChartMng::WindowProc:UMSG_RECEIVEFROMSERVER\n", __LINE__);
// 				OutputDebugString(szDebug);
// 			}

			stReceiveFromServer* pStR = (stReceiveFromServer*)lParam;

			if(!GetpChartItemSelected()) return FALSE;
			if(!pStR->m_dwLength) 
			{
				GetpChartItemSelected()->SetNext(FALSE);

				//==========================================================
				for(int i = 0; i < GetpChartItemSelected()->m_arrayMarketTRData.GetSize();i++)
				{
					CChartItemData* pChartItemData = NULL;
					pChartItemData = GetpChartItemSelected()->m_arrayMarketTRData.GetAt(i);
					if(pChartItemData)
						pChartItemData->SetBNextMarketData(FALSE);
				}
				//==========================================================

				return TRUE;
			}
			CChartItem* pSelChartItem = GetpChartItemSelected();
			// �������� �ҷ����� : �������� - ojtaso (20070514)
			long lRetValue = 0;
			if(pSelChartItem->IsLoadingRQ())
			{
				pSelChartItem->ReceiveLoadingChartData( pStR->m_pBuffer, pStR->m_dwLength);			
				pSelChartItem->SetBEnableIndicatorCalculation(TRUE);
				return CWnd::WindowProc(message, wParam, lParam);
			}
			else
			{
//#ifdef __MULTI_LANG__ //@Solomon-100225				
				pSelChartItem->SetAvgUnitPrice(0.);
				pSelChartItem->SetBEnableIndicatorCalculation(FALSE);
				lRetValue = pSelChartItem->ReceiveInitalChartData( pStR->m_pBuffer, pStR->m_dwLength);
				
//				if(m_pLeftBarDlg)
//					m_pLeftBarDlg->SendMessage(RMSG_CHECKAVGPRICE, 0, (LPARAM)(LPTSTR)(LPCTSTR)pSelChartItem->GetSelectedItemCode());
			}

			//2007.03.22 by LYH ������ ��ü ���� ȭ�� ���� ������ viewCount �����ϱ� ���� ��Ʈ �׸����� ��
			pSelChartItem->SetBEnableIndicatorCalculation(TRUE);

			// �̹� �� �Լ����� ��Ʈ�� �׸���. =======================================================
			long lZoomValue = m_pInputBarDlg ? m_pInputBarDlg->SendMessage(UMSG_GETZOOMVALUE,pSelChartItem->GetType()) : 0;	
				
// 			if(lZoomValue)
// 			{
// 				if((lZoomValue>pSelChartItem->GetAllCnt()&&pSelChartItem->GetAllCnt()))
// 				{
// 					pSelChartItem->SetNew_OnePageDataCount(pSelChartItem->GetAllCnt());
// 				}
// 				else
// 				{
// 					pSelChartItem->SetNew_OnePageDataCount(lZoomValue);
// 				}				
// 			}
// 			else
// 			{
// 				pSelChartItem->SetNew_OnePageDataCount(pSelChartItem->GetAllCnt());
// 			}

			
			
			char Type = pSelChartItem->GetType();
			if(m_strTimeMarkReserved!=""
					&&(	pSelChartItem->GetType()!= WEEK_DATA_CHART
					&&	pSelChartItem->GetType()!= MONTH_DATA_CHART
					&&	pSelChartItem->GetType()!= DAY_DATA_CHART
					&&	pSelChartItem->GetType()!= YEAR_DATA_CHART ))
			{
				pSelChartItem->SetTimeMark(m_strTimeMarkReserved,TRUE);				
			}
			m_strTimeMarkReserved.Empty();

			
			

			if(m_bHideScale)
			{
				pSelChartItem->HideScale(m_bHideScale);
			}		
			if(m_bHideInnerToolBar)
			{
				pSelChartItem->HideInnerToolBar(m_bHideInnerToolBar);
			}
			switch(lRetValue)
			{
			case 1:
				{				
//					RequestAVGForGuideLine(TRUE);										
					SendInitChartData(TRUE, TRUE/*bFromTRData*/);
					if(!SetJCodesNext())
					{
						if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd())
						{
							BOOL bPlayState = FALSE;
							m_pLeftBarDlg->SendMessage(UMSG_IS_PLAYSTATE, (WPARAM)&bPlayState);

							if(bPlayState)
								m_pLeftBarDlg->SendMessage(UMSG_RUN_AUTOTIMER);
						}

						TailInLoop();	
//@100303-A032--> ���񵿱�ȭ�̰� ������ø�� �� Mode�� �ٲ�� ���������� �ȵǴ� ��� ����. ����������.
//						if(m_nOldMode >= 0)
//						{
//							m_nMode = m_nOldMode;
//							m_nOldMode = -1;
//						}
//@100303-A032<--
						if(m_nMode == ADD_NEW_CHART && m_pInputBarDlg)
							m_pInputBarDlg->SendMessage(UMSG_EDIT_FOCUS);
						//if(!m_bLastEnvUpdated)
						//	m_bLastEnvUpdated = TRUE; //ó�� ���� �ε尡 �������� ����
					}
					else
					{
						if(GetBReplayChartMode() && m_pInputBarDlg)
						{
							// 2005. 10. 18
							// ���� ���¿��� �޾ƾ� �� �����͸� �� �޾��� ��� Play��ư Ȱ��ȭ �Ѵ�.
							m_pInputBarDlg->SendMessage(UMSG_ENABLE_PLAYBTN);
						}
					}				
				}
				break;
			case 2:				
				if(!RequestChartData())
				{					
					TailInLoop();
				}
				break;
			default:				
				TailInLoop();
				break;
			}
			
			
			// gm0604 added. Start
			((CStdDialog*)GetParent())->SetChartBySystemTrading(FALSE);
			// gm0604 added. End
			CChartItem* pChartItem = GetpChartItemSelected();
			if(pChartItem)
			{
				pChartItem->UpdateSystemTradingIndexByTab();
			}			

			//05.09.08
			if(m_pInputBarDlg)
				m_pInputBarDlg->SendMessage(UMSG_END_REQUEST_CHARTDATA);
			
			if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
				m_pExtInputBarDlg->SendMessage(UMSG_END_REQUEST_CHARTDATA);

			if(m_bExistSyncChart)
			{
				m_bExistSyncChart = FALSE;		
			}
			//���� �ڵ忡 ��Ŀ���� ������  �Ѵ�.
			if(GetBNoExistLoadCnts() && m_bLastEnvUpdated && m_pInputBarDlg)
			{
				m_pInputBarDlg->SendMessage(UMSG_EDIT_FOCUS);
				SetBNoExistLoadCnts(FALSE); //������ �ٽ� �� �Ҹ��� �ϱ� ���� False�� ��
			}
		}
		break;
	case UMSG_CHANGEMODE:
		{
			int nMode = lParam;
			// �������� �߰� UI : �������� - ojtaso (20070522)
			if(((CStdDialog*)GetParent())->IsMultiItemChart())
			{
				if(nMode == NEW_ITEM || nMode == OVERLAPP_ITEM)
				{//KHD : ��ø�϶� ������ ����ȭ�ȵǰ� ���� 
					if(nMode == OVERLAPP_ITEM)
					{
						m_bJongmokLink = FALSE;
						m_pExtInputBarDlg->SendMessage(UMSG_SETJONGMOKLINK, FALSE);//���� ����ȭ �ȵǰ� ���� 
					}
					SetAddMode(TRUE);
//					nMode = CHANGE_CHART;
				}
				else if(nMode == CHANGE_ITEM || nMode == NEW_CHART || nMode == REPLAY_ITEM)
				{
					SetAddMode(FALSE);
//					nMode = CHANGE_CHART;
				}
			}
			else
				SetAddMode(FALSE);

			m_nOldMode = GetMode();
			SetMode(nMode);
		}
		break;
	case UMSG_SHOWINDICATORCONFIGDLG:
		if(!GetpChartItemSelected()) return FALSE;
//>> [QC No]090729HMC046
		if(!GetpChartItemSelected()->m_bReceivedDataChart) return FALSE;
//<< [QC No]090729HMC046
		if(wParam == 1)
			GetpChartItemSelected()->ShowIndicatorConfigDlgWithName("���� �̵����");
		else if(wParam == 2)
			GetpChartItemSelected()->ShowIndicatorConfigDlgWithName("�ŷ��� �̵����");
		else
			GetpChartItemSelected()->ShowIndicatorConfigDlg();
		break;
	case UMSG_SETTYPE:
		{
			char chType = wParam;
			long lNumber = lParam;
			SetType( chType, lNumber);
		}
		break;
	case UMSG_SETZOOMVALUE:
		{
			if(!GetpChartItemSelected()) return FALSE;
			GetpChartItemSelected()->SetNew_OnePageDataCount(wParam);
			if(m_pInputBarDlg)
				m_pInputBarDlg->SendMessage(UMSG_SETZOOMVALUE,wParam);
		}
		break;
	case UMSG_SETCNTS:
		{
			if(!GetpChartItemSelected()) return FALSE;
			GetpChartItemSelected()->SetCntForRequest(wParam);
			// �������� �߰� : �������� - ojtaso (20070118)
			RequestChartData(TRUE);
			GetpChartItemSelected()->SetSaveAllCount(wParam);
		}
		break;
	case UMSG_SETMATRIX:
		{			
			SetMatrix(wParam,lParam);
		}
		break;
	case UMSG_REQUESTMORE:
		{
			CChartItem* pSelChartItem = NULL;
			pSelChartItem = GetpChartItemSelected();
			if(!pSelChartItem) return FALSE;
			CString strNext;
			strNext.Format("%s", pSelChartItem->GetNextKey());
			strNext.TrimLeft();
			if(strNext.GetLength()<1)
			{
				 return FALSE;
			}
			pSelChartItem->SetNext(TRUE);
			pSelChartItem->ClearData();
			// �������� �߰� : �������� - ojtaso (20070118)
			RequestChartData(TRUE);
		}
		break;
	case UMSG_RESETCHART:
		{
			ResetChartAll();
		}
		break;
	case UMSG_RESETOCX:
		{
			//===========================================================
			//===========================================================
			//if(m_pExtInputBarDlg)
			//	m_pExtInputBarDlg->SendMessage(UMSG_RESETOCX, 0, 0);
			//===========================================================

			SetMatrix(1,1);
			ResetOCX();
		}
		break;
	case UMSG_APPLYFAVORITE:
		{
			CString strItem = (LPCTSTR)lParam;
			CString strTabName;
			CString strSubItemName;
			int nPos = strItem.Find(']');
			strTabName = strItem.Mid(1,nPos-1);
			strSubItemName = strItem.Right(strItem.GetLength()-nPos-2);
			if(strTabName==TAB_CHART)
			{
				if(!GetpChartItemSelected()) return FALSE;
				GetpChartItemSelected()->AddChartBlock(strSubItemName);
			}			
		}
		break;
	case UMSG_GETNAMEINDICATOR:
		{
			GetIndicatorNameListIntoArray((CStringArray *)lParam, (LPCTSTR)wParam);
		}
		break;
	case UMSG_HASGRAPH:
		{
			if(!GetpChartItemSelected()) return FALSE;
			return GetpChartItemSelected()->HasGraph((LPCTSTR)wParam);
		}
		break;
	case UMSG_DEL_CHART_BLOCK:
		{
			if(!GetpChartItemSelected()) return FALSE;
			return GetpChartItemSelected()->RemoveChart((LPCTSTR)wParam);
		}
		break;
	case UMSG_ADDCHARTBLOCK:
		{
			CChartItem* pSelChartItem = NULL;
			CString strMessage;

			//@Solomon:100226(alzioyes)���콺��ġ�� CChartItem�� ã�Ƽ� ó���Ѵ�.
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			pSelChartItem = (CChartItem*)ChildWindowFromPoint(pt);

			if(pSelChartItem==NULL || pSelChartItem->IsKindOf(RUNTIME_CLASS(CChartItem))==FALSE)
			{
				pSelChartItem = GetpChartItemSelected();
			}

			if(!pSelChartItem) return FALSE;

			int nFrom = (int)lParam;


			//05.05.30 add by sy, nam =========================================
			//Input Bar�� Combo�� ���õ� ���� ��ȭ ��Ų��. 
			if( nFrom == FROM_TABCHART && m_pInputBarDlg && (m_pInputBarDlg->GetSafeHwnd()))
				m_pInputBarDlg->SendMessage(UMSG_CHANGE_CHARTVALUE, wParam, lParam);

		
			//2007.01.26 by LYH ��ǥ ������ ���Ͽ��� �е��� ����
			//CString strIndicondFile = m_strRootPath + "\\data\\indicond.cfg";
			CString strIndicondFile = m_strRootPath + "\\Icss\\indicond.cfg";	//@Solomon
			CString strGroup;
			CString strJipyo = (LPCTSTR)wParam;
			//0023674: [7111] ������Ʈ ȭ�� - '�ŷ����' ��ǥ �� �ֱ� ��Ʈ���� �߰� ��û �� �˸� ���� ��û
			if(strJipyo == "�ŷ����")
			{
				if(pSelChartItem->GetDataTRCode() == QUERY_STRFUTOPT || pSelChartItem->GetDataTRCode() == QUERY_STRSTOCKFUTOPT)
				{
					MessageBox("�ŷ������ �Ʒ� ��Ʈ�� �߰� �����մϴ�.\n �ֽ� : ��,��,��,�� / ���� : ��,��,��", "IBK����", MB_OK | MB_ICONINFORMATION);
					return 0;
				}
				if(pSelChartItem->GetType() == TICK_DATA_CHART || pSelChartItem->GetType() == SEC_DATA_CHART || (pSelChartItem->GetType() == MIN_DATA_CHART && m_nMarketType != CMasterTypeInfo::STOCK_CHART))
				{
					MessageBox("�ŷ������ �Ʒ� ��Ʈ�� �߰� �����մϴ�.\n �ֽ� : ��,��,��,�� / ���� : ��,��,��", "IBK����", MB_OK | MB_ICONINFORMATION);
					return 0;
				}
			}
// #ifdef __MULTI_LANG__ //@Solomon-100225
			CString strJipyo2 = g_iMetaTable.GetMetaTextDirectly(IMetaTable::INDICATOR_NAME, IMetaTable::KOREAN, strJipyo);
			::GetPrivateProfileString(strJipyo2, "groupName", "", strGroup.GetBuffer(MAX_PATH), MAX_PATH, strIndicondFile);
// #else
// 			::GetPrivateProfileString(strJipyo, "groupName", "", strGroup.GetBuffer(MAX_PATH), MAX_PATH, strIndicondFile);
// #endif

			//if(nFrom == FROM_TABJIPYO_MARKET || nFrom == FROM_EXTINPUTBAR_MARKET)
// #ifdef __MULTI_LANG__ //@Solomon-100225
			if(strGroup == "_Market_")
// #else
// 			if(strGroup == "������ǥ")
// #endif
			{
				m_bMarketJipyoAdding = TRUE;
				//>>0000550: 0503_����������Ʈ�� ������ǥ�� �̰������� ��ǥ�� ���ֱ� �̿��� ��� �ֱ⿡���� ��ȸ�ǵ��� ������û
				if( pSelChartItem->GetType() != DAY_DATA_CHART )
				if( strJipyo != "�̰�������" && pSelChartItem->GetType() != DAY_DATA_CHART )
				{
					AfxMessageBox("������ǥ�� �ϰ��϶��� ����˴ϴ�");
					return 0;
				}

				// ������ �� ��Ÿ��ǥ �޽���
				if (strJipyo == "�ð��Ѿ�" || strJipyo == "�ܱ��� ������" || strJipyo == "�ܱ��� ������" || strJipyo == "�ܱ��μ��ż�" || 
					strJipyo == "������ż���" || strJipyo == "������ż�����" || strJipyo == "�ܱ���+������ż�" || strJipyo == "�ܱ���+������ż�����")
				{
					if(pSelChartItem->GetDataTRCode() != QUERY_STRJONGMOK)
					{
						strMessage.Format("%s ��ǥ�� �ֽĿ����� ��ȸ�� �����մϴ�", strJipyo);						
						AfxMessageBox(strMessage);
						return FALSE;
					}
				}
				else if(strJipyo == "�̰�������")
				{
					if(pSelChartItem->GetDataTRCode() != QUERY_STRFUTOPT/* && pSelChartItem->GetDataTRCode() != QUERY_STRCOMMODITY*/)	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
					{
						strMessage.Format("%s ��ǥ�� ���������� ��ȸ�� �����մϴ�", strJipyo);						
						AfxMessageBox(strMessage);
						return FALSE;
					}
				}

			}
			//>> �������񿡼��� Ư����ǥ ��ȯ ���� - vntsorl(20090516)
// #ifdef __MULTI_LANG__ //@Solomon-100225
			else if(strJipyo2 == "_Special_")
// #else
// 			else if(strGroup == "Ư����ǥ")
// #endif
			{
				if(pSelChartItem->GetRQCount() > 1)
				{
					CString strMessage;
					strMessage.Format("Ư����ǥ�� �������񿡼��� �����մϴ�");      
					AfxMessageBox(strMessage);
					return FALSE;
				}
			}
			//<<�������񿡼��� Ư����ǥ ��ȯ ���� - vntsorl(20090516)
			else
			{
				if (strJipyo == "ADL" || strJipyo == "����ֺ���(ADR)")
				{
					if(pSelChartItem->GetDataTRCode() != QUERY_STRUPJONG)
					{
						strMessage.Format("%s ��ǥ�� ���� �ϰ��϶��� ��ȸ�� �����մϴ�", strJipyo);						
						AfxMessageBox(strMessage);
						return FALSE;
					}
				}
				else if (strJipyo == "�ſ��ܰ���" || strJipyo == "�ſ������" || strJipyo == "�ſ��ܰ����" || strJipyo == "�ſ��ܰ�����" )
				{
					if (pSelChartItem->GetDataTRCode() != QUERY_STRJONGMOK || pSelChartItem->GetType() != DAY_DATA_CHART )
					{
						strMessage.Format("%s ��ǥ�� �ֽ� �ϰ��϶��� ��ȸ�� �����մϴ�", strJipyo);						
						AfxMessageBox(strMessage);
						return FALSE;
					}
				}
			}
			
			if(strGroup != "_Special_" && strGroup != "_Price_")
			{
				int nBlockCount = 0;
				if(!((CStdDialog*)GetParent())->IsAvailableBlockCount(FALSE, nBlockCount))
				{
					CString strMessage;
					strMessage.Format("�ִ� %d�� �������� �߰� �����մϴ�.", nBlockCount);
					AfxMessageBox(strMessage);
				}
			}

			//ȯ�浿��ȭ ���� �������� Ȯ��		05.09.26	
			if( m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd() && m_pExtInputBarDlg->SendMessage(UMSG_GETENVSINK) == 0 ) //����ȭ ���� �ƴ�.
			{
				if(nFrom == FROM_INPUTBAR || nFrom == FROM_TABCHART)
				{	
					if(!pSelChartItem->GetDataCode().GetLength()) 
					{
						if(m_bMarketJipyoAdding)
							m_bMarketJipyoAdding = FALSE;	

						return 0l;
					}

					CString sValue = (LPCTSTR)wParam;
					pSelChartItem->SetChartLineType(sValue);
				}				
				
				pSelChartItem->AddChartBlock((LPCTSTR)wParam);

				//@100613-alzioyes. ReCalculateAllGraphs �Ź��� �ٽ� �׸������ϱ� ���� ��ƾ.
				//if(strJipyo.CompareNoCase("ĵ����Ʈ")==0)		pSelChartItem->ReCalculateAllGraphs();
			}
			else
			{
				CChartItem *pChartItem = NULL;
				for(int nRowIndex=0;nRowIndex<m_lRows;nRowIndex++)
				{  
					for(int nColIndex=0;nColIndex<m_lCols;nColIndex++)
					{  
						pChartItem = m_pChartItems[nRowIndex][nColIndex];  
						if(pChartItem)
						{
							char chBaseType		= pSelChartItem->GetType();  //���� ��Ʈ�� ��ȸ �ð����� Get
							char chInLoopChartType	= pChartItem->GetType(); //�� ��Ʈ�� ��ȸ �ð����� Get
							if(lstrlen(&chInLoopChartType) == 0)				  //������ ��ȸ Ÿ���� ������ 	
								chInLoopChartType	= pChartItem->GetTypeRequested();//��û�ϴ� Ÿ���� Get

							if(lstrlen(&chInLoopChartType) == 0) continue; //�׷��� �������°� ������ Return

							if( chBaseType == chInLoopChartType ) //���� ��Ʈ�� ã�� ��� 
							{			
								if(pChartItem->m_bReceivedDataChart != TRUE)  //��ȸ Ÿ���� ������ �����Ͱ� ���� ��� �����Ѵ�.
									continue;

								if(nFrom == FROM_INPUTBAR || nFrom == FROM_TABCHART)
								{	
									if(!pChartItem->GetDataCode().GetLength()) continue;

									CString sValue = (LPCTSTR)wParam;
									pChartItem->SetChartLineType(sValue);
								}
								pChartItem->AddChartBlock((LPCTSTR)wParam);
							}
						}
					}
				}			
			}

			if(m_bMarketJipyoAdding)
				m_bMarketJipyoAdding = FALSE;			

			return TRUE;

		}
		break;
	case UMSG_SETJONGMOKLINK:
		{
			m_bJongmokLink = wParam;
			if(lParam==999) return 0L;

			if(m_bJongmokLink)
			{
				if(!GetpChartItemSelected()) return FALSE;

				if(((CStdDialog*)GetParent())->m_nScreenID == nScreenID_6301)
					SetChartLayout(m_nChartLayout);

				CCodeDataFromInputBar CodeData;
				CodeData.m_strCode = GetpChartItemSelected()->GetDataCode();
				CodeData.m_lEndTime = atol(GetpChartItemSelected()->GetEndDateRequested());
				InputCodeFromInputBar(&CodeData,TRUE);	
				
				m_nOldMode = -1;
			}
		}
		break;
	case UMSG_SETTYPELINK:
		{
			m_bTypeLink = wParam;
			if(m_bTypeLink)
			{
				if(!GetpChartItemSelected()) return FALSE;
				char chType = GetpChartItemSelected()->GetType();
				long lTickMinNumber = atol(GetpChartItemSelected()->GetTickMinDayNumber());
				if(chType == TICK_DATA_CHART || chType== MIN_DATA_CHART || chType == DAY_DATA_CHART || chType == SEC_DATA_CHART)
				{
					this->SendMessage(UMSG_SETTYPE,(WPARAM)chType,(LPARAM)lTickMinNumber);				
				}
				else
				{
					this->SendMessage(UMSG_SETTYPE,(WPARAM)chType);
				}
			}
		}
		break;
	case UMSG_LOADCHART:
		{
			CPoint* pPt = (CPoint*)lParam;
			LoadChart(pPt);
		}
		break;
	case UMSG_SAVECHART:
		{
			CPoint* pPt = (CPoint*)lParam;
			SaveChart(pPt);
		}
		break;
	case UMSG_LOADSAVECHART:
		{
			CPoint* pPt = (CPoint*)lParam;
			LoadSaveChart(pPt);
		}
		break;
	case UMSG_ADDUPJONGDATA:
		{
			CString strCode;
			strCode.Format("%d",wParam);
			AddUpjongData(strCode);
		}
		break;
	case UMSG_FIXDATE:
		{
			wParam = GetpChartItemSelected()->GetNOnePageDataCount();

			InitInLoop();			
			m_strEndDateInLoop = (LPCTSTR)lParam;
			m_lZoomValueInLoop = wParam;
			m_dEndDateInLoop = atof(m_strEndDateInLoop);
			m_lTypeInLoop = LOOPTYPE_FIXDATE/*2*/;// fixdate
			HeadInLoop();
		}
		break;
	case UMSG_SHOWSCALE:
		{
			m_bHideScale = wParam;
			ShowScale();
		}
		break;
	case UMSG_SHOWINNERTOOLBAR: 
		{
			m_bHideInnerToolBar = wParam;
			ShowInnerToolbar();
		}
	case UMSG_UPDATEINDEX:
		{
			UpdateIndex();
		}
		break;
	case UMSG_SETONECHART:
		{
			ReplaceChart(0,0,m_lRowSelected,m_lColSelected);
			SetMatrix(1,1);

			//@�����߰�
			{
				CChartItem* pInitChartItem = GetpChartItemSelected();
				if(pInitChartItem)
					pInitChartItem->m_bNowInitialState = TRUE;
			}
			//@�����߰�
		}
		break;
	case UMSG_UPDATEREVISED:
		{
			BOOL bUseRevised = wParam;
			BOOL bDontRequestData = lParam;

			UpdateRevisedValue(bUseRevised,bDontRequestData);
		}
		break;

	case USMG_NOTIFY_REVISED:
		{
			BOOL bUseRevised = wParam;
			if(m_pInputBarDlg)
				m_pInputBarDlg->SendMessage(USMG_NOTIFY_REVISED, bUseRevised);
		}
		break;
	case UMSG_MULTICODES:
		{
			// �������� �߰� UI : �������� - ojtaso (20070522)
			if(wParam == 2)
			{
				m_nOldMode = m_nMode;
				m_nMode = ADD_NEW_CHART;
				m_bAddModeFromTab = FALSE;
			}
			else
			{
				m_bAddModeFromTab = (BOOL)wParam;
			}

			m_strMultiCodes = (LPCTSTR)lParam;
			if(!GetAddGraphMode())
				SetChartSelected(CPoint(0,0));
			SetMultiCodes((LPCTSTR)lParam);
		}
		break;
	case UMSG_SHOWENVIRDLG:
		{
			CChartItem* pChartItem = GetpChartItemSelected();
			if (!pChartItem)
				return FALSE;

			pChartItem->RunEnvironmentDlg(1);
			//GetpChartItemSelected()->ShowEnvironmentDlg(); //��������.
		}
		break;
	case UMSG_GETINC:
		{
			return m_nMode;
		}
		break;
	case UMSG_RATEVALUE:
		{
			int  nNumericChartValue = wParam;
			BOOL bDontRequestData = lParam;
			UpdateNumericChartValue(nNumericChartValue, bDontRequestData);
		}
		break;
	case UMSG_SETTIMESHARSYSEAGE:
		{
			SetTimeShareAll(wParam);
		}
		break;
	case UMSG_SETUPPERLOWPRICEMARKUSEAGE:
		{
			SetEnableBoundMarkAll(wParam);
		}
		break;
	case UMSG_SETSHOWLOCK:
		{
			BOOL bShowLock = wParam;
			EACH_CHARTJOB_START()
				if(pChartItem->GetType() == DAY_DATA_CHART)
				{
					pChartItem->ShowLock(bShowLock);
				}
				else
				{
					pChartItem->ShowLock(FALSE);
				}
			EACH_CHARTJOB_END()			
		}
		break;
//@��������
//	case UMSG_SENDJCODECHECKED:
//		{
//			LPCTSTR lpCode = (LPCTSTR)lParam;
//			CString strCode;
//			strCode.Format("%s ", lpCode);
//			BOOL bCheck = wParam;
//			EACH_CHARTJOB_START()
//				if(pChartItem->GetDataCode()==strCode)
//				{
//					pChartItem->SetCheck(bCheck);
//				}
//			EACH_CHARTJOB_END()						
//		}
//		break;
//	case UMSG_GETJCODECHECKEDFROMCHART:// Check�ϴ� �����ڵ带 �����´�.
//		{
//			CStringList *plistStrJongmokCodeChecked = (CStringList *)wParam;
//			CStringList *plistStrJongmokCodeHighlighted = (CStringList *)lParam;;
//			CString strJongmokCode;
//			EACH_CHARTJOB_START()
//				if(pChartItem->GetDataTRCode()==QUERY_STRJONGMOK)
//				{
//					strJongmokCode = pChartItem->GetDataCode();
//					if(strJongmokCode.GetLength()&&!plistStrJongmokCodeHighlighted->Find(strJongmokCode))
//					{
//						plistStrJongmokCodeHighlighted->AddTail(strJongmokCode);
//						if(pChartItem->GetCheck()&&!plistStrJongmokCodeChecked->Find(strJongmokCode))
//						{
//							plistStrJongmokCodeChecked->AddTail(strJongmokCode);
//						}
//					}
//				}
//			EACH_CHARTJOB_END()
//		}
//		break;
//@��������
	case UMSG_GETJONGMOKCODE:
		{
			CChartItem* pChartItem = GetpChartItemSelected();
			if(!pChartItem) return FALSE;

			if(pChartItem->GetDataTRCode()==QUERY_STRJONGMOK)
			{
				CString *pStr = (CString *)wParam;
				pStr->Format("%s",pChartItem->GetDataCode());				
			}
		}
		break;
	case UMSG_GETCNTOFCHART:
		{
			return GetCntOfChart();
		}
		break;
//>> ������ - vntsorl(20091008)
	case UMSG_SETNORMALIZEDMINUTETIMERULE:
		{
			BOOL bShowFackedCandle = wParam; //0. �Ǻ�  1. �Ǻ� + ���
			BOOL bDontRequestData = lParam;
			
			//
			GetpChartItemSelected()->m_CfgInfo.bRealCandle = wParam > 0 ? false : true;
			UpdateShowingFackedCandle(bShowFackedCandle,bDontRequestData);
		}
		break;
// 	case UMSG_SET_NORMALIZEDMINUTETIMERULE_AND_GAPREVISION:
// 		{
// 			BOOL bImaginaryCandle	= wParam;	// 0. �Ǻ�  1. �Ǻ� + ���
// 			BOOL bUseGapRevision	= lParam;	// ������
// 			GetpChartItemSelected()->SetImaginaryCandleAndGapRevision( bImaginaryCandle, bUseGapRevision);
// 		}
// 		break;
//<< ������ - vntsorl(20091008)
	case UMSG_CHARTODRTRACE:
		{
			if(!GetpChartItemSelected()) return FALSE;
			long lFlag = (long)wParam;
			{
//				GetpChartItemSelected()->LoadAddIn("OrderTrace.dll");
//				GetpChartItemSelected()->InvokeAddIn("OrderTrace.dll",0, ((CStdDialog*)GetParent())->m_lDBMgr);
			}
		}
		break;
	case UMSG_ANALYSISPATTERN:
		{
			CChartItem* pSelChartItem = GetpChartItemSelected();
			if(!pSelChartItem) return FALSE;
			long lFlag = (long)wParam;
			pSelChartItem->InvokeAddInPattern("ChartPatternAddin.dll",NULL,lFlag);
		}
		break;
	case UMSG_APPLY_ANALYZE_PERIOD_ADDIN: //2005.12.02 �̿�
		{
			if(!GetpChartItemSelected()) return FALSE;			
			

			//GetpChartItemSelected()->InvokeAddIn("AnalyzePeriodAddin.dll",NULL, (long)m_pDBMgr);
			//GetParent()->SendMessage(RMS_ANALYSIS_FUNC, 1001, 1);

		}
		break;
	case UMSG_CHARTGUIDELINE:
		{
			//if(m_nMarketType != CMasterTypeInfo::STOCK_CHART)	
			//{
			//	AfxMessageBox("�ֽ� ��Ʈ������ ���� �����մϴ�");
			//	return 0;
			//}
			CChartItem* pSelChartItem = NULL;
			pSelChartItem = GetpChartItemSelected();
			if(!pSelChartItem) return FALSE;


			if( pSelChartItem->GetChart_NumericValueType() != WON_TYPE)
			{
				AfxMessageBox("�� ���� �϶��� ���ؼ��� ���� �����մϴ�");
				return FALSE;
			}

//@Solomon:091123SM068
//			pSelChartItem->InvokeAddIn("ChartGuideLineAddin.dll:GUIDE_LINE",101,(long)pSelChartItem->GetSafeHwnd());

			
			CStringList strlistSetAll;
			strlistSetAll.AddTail(((CStdDialog*)GetParent())->GetAccountInfo());
			pSelChartItem->InvokeAddIn("ChartGuideLineAddin.dll:GUIDE_LINE",NULL,(long)&strlistSetAll);
/*
			if(strlistSetAll.GetCount())
			{
				CString strbChanged = strlistSetAll.GetHead();
				if(strbChanged=="1")
				{
					RequestAVGForGuideLine(FALSE);
				}
			}
*/
		}
		break;
	case UMSG_PERIOD_SUMMARY:
		{
			CChartItem* pSelChartItem = GetpChartItemSelected();
			if(!pSelChartItem) return FALSE;

			pSelChartItem->InvokeAddInPeriod("ChartPeriodSummaryAddIn.dll:PERIOD_SUMMARY",lParam,0);
		}
		break;
	case UMSG_SETDRAGMODETYPE:
		{
			BOOL bType = wParam;
			EACH_CHARTJOB_START()
				pChartItem->SetDragModeType(bType);
			EACH_CHARTJOB_END()				
		}
		break;
	case UMSG_SETORDEREDINFO:
		{
			EACH_CHARTJOB_START()
				pChartItem->SendMessageToInvoke(INVOKEMSG_SHOWORDEREDINFO,NULL,NULL);
			EACH_CHARTJOB_END()			
		}
		break;		
	case UMSG_SEND2SERVERFROMINVOKE:
		{
			stSend2Server *pSt = (stSend2Server *)lParam;
			if(pSt->m_nType>0)
			{
				GetParent()->SendMessage(UMSG_SEND2SERVER,wParam,lParam);			
			}
			else
			{
				MessageBox("SEND2SERVERFROMINVOKE Error!");
			}			
		}
		break;
	case UMSG_GETCODEFROMINVOKE:
		{
			CChartItem *pChartItem = (CChartItem *)wParam;
			CString* pStrCode = (CString*)lParam;
			if(pChartItem->GetSafeHwnd())
			{
				pStrCode->Format("%s",pChartItem->GetDataCode());
			}
		}
		break;
	case UMSG_GETMAINHANDLEFROMINVOKE:
		{
		//	CWnd** ppMainFrame = (CWnd**)wParam;
		//	*ppMainFrame = ((IMainDllObMain*)g_pMainDll)->m_pMainFrame;
		}
		break;
	case UMSG_GET_TICKMINDAY_VALUE:
		{
			//�ʱ�ȭ�ÿ��� ����Ʈ�� �׻� 1�̹Ƿ� ������ �����ص� �ɵ���.
			if(m_bResetChartAll) return 1;

			int nType = lParam;			
			sTemp = GetpChartItemSelected()->GetTickMinDayNumber(nType > -1 ? nType : -1);

			//2005. 06. 24 ... �޷� ��Ʈ�� �翷�� ��ư���� �����ϵ��� �Ѵ� ===================
			if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
				m_pExtInputBarDlg->SendMessage(message, wParam, lParam);
			//================================================================================
			return atoi(sTemp);
		}
		break;
	case UMSG_USE_LOG:
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
			{
				long lValue = (long)wParam;				
				//pChartItem->SetAllLogScale(lValue > 0 ? TRUE : FALSE);
				pChartItem->SetLogScale("", (lValue > 0 ? TRUE : FALSE));
				pChartItem->m_CfgInfo.bShowLog = (lValue > 0 ? true : false); //2005. 08. 22 
				pChartItem->Invalidate();
			}
		}
		break;
	case UMSG_GET_TIME_FROM_DATETIMEPICKER:
		{
			long lEndDate = 0;
			if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())			
				lEndDate =m_pExtInputBarDlg->SendMessage(UMSG_GET_TIME_FROM_DATETIMEPICKER, 0, lParam);
			
			return lEndDate;
		}
		break;
	case UMSG_MOVE_CHART_VIEWRANGE:
		{
			//��Ʈ�� ���� �ִ� ������ �����Ѵ�.
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
			{				
				pChartItem->SetStartEndDateOfView((double)wParam, (double)lParam);
			}
		}
		break;
	case UMSG_DISCONNECT_ALLREAL: //��Ʈ Real ����.
		{
			OnAllCodeUnAdvise();
		}
		break;
	case UMSG_SETLOG: //05.08.23
		{
			if(m_pInputBarDlg)
				m_pInputBarDlg->SendMessage(message, wParam, lParam);
		}
		break;
	case UMS_SETCHART_DATEINFO:
		{
			//��Ʈ�� ��¥ ������ �����Ѵ�
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
			{
				CString sDate;
				sDate.Format("%d", lParam); //��¥ ����
				
				if(wParam == 1) // ������ �����̸� 1, ������ �����̸� 0;
				{
//					pChartItem->SetEndDateRequested(sDate);
					pChartItem->SetEndDate(sDate);
					pChartItem->SetStartDateFlag(FALSE);
				}
				else if(wParam == 0)
				{					
					pChartItem->SetStartDate(sDate)  ;
					pChartItem->SetStartDateFlag(TRUE);
				}
			}
		}
		break;
	case USMG_PLEASE_RESIZE:
		{		
			if(m_pFloatingDlg != NULL)
			{
				BOOL bCanRequest = wParam ? FALSE : TRUE;
				if(bCanRequest) // Ȯ��� �����̸�
				{
					if(m_pInputBarDlg)
					{
						CString strCode = GetpChartItemSelected()->GetDataCode();
						if(strCode.GetLength() != CODE_STOCK_LENGTH)
						{
							m_pFloatingDlg->m_bIconState = 1;
							return 1L;
						}
					}
				}

				m_pRectFloatingHoga = NULL;
				m_pRectFloatingHoga = (CRect*)lParam;			
				if(m_pRectFloatingHoga)
				{
					ScreenToClient(m_pRectFloatingHoga);					
					m_pFloatingDlg->MoveWindow(m_pRectFloatingHoga, TRUE);
				}

				//=====================================================================================
				// ȣ�� ������ ���� �� �̵�.
				//=====================================================================================
				MoveFloatingHogaDlg();
				//=====================================================================================

				if(bCanRequest) // Ȯ��� �����̸�
				{
					//ȣ���� ��û �Ѵ�.
					((CStdDialog*)GetParent())->Request_StockHogaData();
				}
			}
		}
		break;
	case UMSG_SIGNAL_CLICK:
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
			{
				pChartItem->OpenSignalMapScr();
			}
		}
		break;
	case UMSG_STOP_APPLY_EXPTOOOL:
		{
			EACH_CHARTJOB_START()
				if(GetpChartItemSelected() != pChartItem)
				{
					pChartItem->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::T_TOOL_SELECTION, 1);
					pChartItem->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::T_TOOL_SELECTION, 1);
				}
			EACH_CHARTJOB_END()
		}
		break;
	case UMSG_FIREGSHAREDATA:
		{
			CString strCodeShare = (LPCTSTR)wParam;			
			if(strCodeShare=="�ð�_��Ʈ")
			{

				CString strTimeInfo = (LPCTSTR)lParam;
				SetTimeMark((LPCTSTR)strTimeInfo,TRUE);
			}
//			else
//			{
//				STSHAREDDATA stData;
//				stData.m_szKey = (LPCTSTR)strCodeHead;
//				stData.m_szValue = (LPCTSTR)strCodeTail;
//				if(m_pLeftBarDlg)
//				{
//					m_pLeftBarDlg->SendMessage(UMSG_SETCHARTHOGA,NULL,(LPARAM)&stData);
//				}	
//			}	
		}
		break;
	case UMSG_ADDSYSTEMTRADINGINDEX:
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
			{
				pChartItem->SetSystemTradingIndexByTab((CStringArray*)wParam);
			}
		}
		break;
	case UMSG_CLEAR_MAREKT_TR:
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
			{
				pChartItem->RemoveAllMarketData();
			}
		}
		break;	
	//11.17�� �߰� =======================================================
	//��Ʈ ����� ����ڰ� ���� �Է��� �� ������ ������ �����ϱ� ���� �߰��ߴ�.
	case UMSG_GETZOOMVALUE:
		{
			if(m_pInputBarDlg && ::IsWindow(m_pInputBarDlg->GetSafeHwnd()))
			{
				return m_pInputBarDlg->SendMessage(UMSG_GETZOOMVALUE, wParam, lParam);
			}
		}
		break;
	case UMSG_EXT_TOOL_RESTORE:
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
			{
				if( pChartItem->GetDataCode().GetLength() )
				{	
					//pChartItem->InitChartItem_FromExtToolBar();
					return 1;
				}

				return 0;
			}
		}
		break;
	case UMSG_CHART_PRINT:
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
				pChartItem->PrintChart();
		}
		break;
	case UMSG_CHART_EXCEL:
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
				pChartItem->SetExcelData();
		}
		break;
	case UMSG_SAVEJIPYOONEFILE:
		{
			// �м��� ���� ���� - ojtaso (20080723)
			if(lParam == 100)
				m_bSaveAnalTool = wParam;
			else
			{
				m_bSaveJipyoOneFile = wParam;
				m_pTickCfgChart		= NULL;
				m_pSecCfgChart		= NULL;
				m_pMinCfgChart		= NULL;
				m_pDayCfgChart		= NULL;
				m_pHourCfgChart		= NULL;
				m_pWeekCfgChart		= NULL;
				m_pMonthCfgChart	= NULL;
				m_pYearCfgChart		= NULL;
			}
		}
		break;
	case UMSG_DWMM_CHART:
		{
//>> [QC No]090729HMC049
//			LoadDWMMChart(lParam);
			int nResult = GetParent()->SendMessage(UMSG_GET_CHARTMODE, 1);
			if(nResult == REPLAY_ITEM && ((CInputBarDlg*)m_pInputBarDlg)->GetReplayChartMode())
			{
				AfxMessageBox("�����忡���� ���ֿ��� ��Ʈ�� �������� �ʽ��ϴ�.");
			}
			else
				LoadDWMMChart(lParam);
//>> [QC No]090729HMC049
		}
		break;
	case UMSG_REQUEST_FORQUERYOPT:
		{
			if(!GetpChartItemSelected()) return FALSE;

			SaveViewCount();

			CCodeDataFromInputBar CodeData;
			CodeData.m_strCode = GetpChartItemSelected()->GetDataCode();
			CodeData.m_lEndTime = atol(GetpChartItemSelected()->GetEndDateRequested());
			//>> [QC No]090731HMC070
			m_bNoInsertChart = TRUE;
			InputCodeFromInputBar(&CodeData,TRUE);				
			m_bNoInsertChart = FALSE;
			//<< [QC No]090731HMC070
		}
		break;
	case UMSG_GETQUERYOPT:
		{
			return ((CExtInputBarDlg*)GetpExtInputBarDlg())->GetQueryOpt();
		}
		break;
	case UMSG_STANDBY_TR_CALL:
		{
			((CStdDialog*)GetParent())->AddStandByTrCall( (LPCTSTR)lParam );
		}
		break;	
	case UMSG_RELAY_POSSSIBLE_CHECK:
		{
			return GetRelayPossible();
		}
		break;
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

void CChartMng::GetJongmokCodes(CStringList *plistStrJongmokCode, CHighlightChartInfoList* plistHighlightChartInfo)
{
	stHighlightChartInfo stChartInfo;
	CString strJongmokCode;
	EACH_CHARTJOB_START()
		if(pChartItem->GetDataTRCode()==QUERY_STRJONGMOK)
		{
			strJongmokCode = pChartItem->GetDataCode();
			if(strJongmokCode.GetLength()&&!plistStrJongmokCode->Find(strJongmokCode))
			{
				plistStrJongmokCode->AddTail(strJongmokCode);
				stChartInfo.m_strJongmokCode = strJongmokCode;
				stChartInfo.m_pChartItem = pChartItem;				
				plistHighlightChartInfo->AddTail(stChartInfo);
			}
		}
	EACH_CHARTJOB_END()
}

void CChartMng::SetTimeShareAll(BOOL bUseage)
{
	EACH_CHARTJOB_START()
		pChartItem->EnableReceivingTimeShare(bUseage);
	EACH_CHARTJOB_END()
}

void CChartMng::SetEnableBoundMarkAll(BOOL bUseage)
{
	EACH_CHARTJOB_START()
		pChartItem->EnableBoundMark(bUseage);
	EACH_CHARTJOB_END()
}

BOOL CChartMng::UpdateIndex()
{
	 //ȣ�� ��û�� ���ϵ��� �Ѵ�.
	if(m_pInputBarDlg)
		m_pInputBarDlg->SendMessage(UMSG_ENABLE_SENDHOGATR, FALSE, 0);


	if(!GetpChartItemSelected()) return FALSE;

	if(GetpChartItemSelected()->GetPrimaryChartItemData() == NULL) return FALSE;
	
	// (2008/8/10 - Seung-Won, Bae) Support UpdateIndex in Maxized Chart Item.
	if( m_bChartExtended)
	{
		m_bUpdatedInChartExtended = TRUE;
		m_bUpdateIndexAfterExtended = TRUE;
		return TRUE;
	}

	InitInLoop();
	m_strArrayCodeInLoop.RemoveAll();
	m_strArrayTRCodeInLoop.RemoveAll();
	


	m_pChartItemSelectedInLoop = GetpChartItemSelected();
	m_lCntInLoop = m_pChartItemSelectedInLoop->GetCntOfItemData();
	m_chTypeInLoop = m_pChartItemSelectedInLoop->GetType();

	//m_bUseRateValueInLoop = m_pChartItemSelectedInLoop->GetUseRateValue();
	m_nChartMngs_ChartDataNumericValue = m_pChartItemSelectedInLoop->GetChart_NumericValueType();

	m_bNormalizedMinuteTimeRuleInLoop = m_pChartItemSelectedInLoop->GetBNormalizedMinuteTimeRule();
	for(long lPos = 1;lPos<m_lCntInLoop;lPos++)
	{
		m_strArrayCodeInLoop.Add(m_pChartItemSelectedInLoop->GetDataCode(lPos));
		m_strArrayTRCodeInLoop.Add(m_pChartItemSelectedInLoop->GetDataTRCode(lPos));
	}			
	m_pChartItemSelectedInLoop->SaveandOpenFromFile("","temp.tmp",FALSE,TRUE);
	m_lTypeInLoop = LOOPTYPE_UPDATE/*4*/;//update
	HeadInLoop();

	//ȣ�� ��û�� �ϵ��� �Ѵ�.
	if(m_pInputBarDlg)
		m_pInputBarDlg->SendMessage(UMSG_ENABLE_SENDHOGATR, TRUE, 0);
	

//	CChartItem *pChartItemSelected = GetpChartItemSelected();
//	CStringArray strArrayCode;
//	CStringArray strArrayTRCode;
//	long lCnt = pChartItemSelected->GetCntOfItemData();
//	for(long lPos = 1;lPos<lCnt;lPos++)
//	{
//		strArrayCode.Add(pChartItemSelected->GetDataCode(lPos));
//		strArrayTRCode.Add(pChartItemSelected->GetDataTRCode(lPos));
//	}			
//	pChartItemSelected->SaveandOpenFromFile("","temp.tmp",FALSE,TRUE);
//	long lColSelected = m_lColSelected;
//	long lRowSelected = m_lRowSelected;
//	EACH_CHARTJOB_START()
//		SetChartSelected(CPoint(nColIndex,nRowIndex));
//		if(!pChartItem->GetAllCnt()) continue;
//		if(pChartItem==pChartItemSelected) continue;
//		pChartItem->ClearDataExceptPrimaryData();
//		for(lPos = 0;lPos<(lCnt-1);lPos++)
//		{
//			pChartItem->SetCodeForQuery(strArrayCode.GetAt(lPos),atoi(strArrayTRCode.GetAt(lPos)),FALSE);
//		}			
//		RequestChartData();
//		pChartItem->SaveandOpenFromFile("temp.tmp","",TRUE,FALSE);
//	EACH_CHARTJOB_END()
//	SetChartSelected(CPoint(lColSelected,lRowSelected));

	
	return TRUE;
}

BOOL CChartMng::UpdateShowingFacked_JustOneChart(BOOL bNormalizedMinuteTimeRule,BOOL bDontRequestData)
{
	if(!GetpChartItemSelected()) return FALSE;

	GetpChartItemSelected()->m_CfgInfo.bRealCandle = bNormalizedMinuteTimeRule ? false : true; //05.11.25
	GetpChartItemSelected()->SetBNormalizedMinuteTimeRule(bNormalizedMinuteTimeRule);
	if(!bDontRequestData) 
		return RequestChartData(TRUE);	

	return TRUE;
}


BOOL CChartMng::UpdateShowingFackedCandle(BOOL bNormalizedMinuteTimeRule,BOOL bDontRequestData)
{
	m_bUpdatingShowingFackedCandle = TRUE;
	if(m_bChartExtended)
	{
		if(!GetpChartItemSelected()) return FALSE;
		GetpChartItemSelected()->SetBNormalizedMinuteTimeRule(bNormalizedMinuteTimeRule);
		if(!bDontRequestData) RequestChartData(TRUE);
		m_bUpdatedInChartExtended = TRUE;
	}
	else
	{
		InitInLoop();
		m_bNormalizedMinuteTimeRuleInLoop = bNormalizedMinuteTimeRule;
		m_bDontRequestDataInLoop = bDontRequestData;
		m_lTypeInLoop = LOOPTYPE_FAKE_CANDLE/*7*/; // ShowFackecCandle
		HeadInLoop();

//		long lColSelected = m_lColSelected;
//		long lRowSelected = m_lRowSelected;
//		EACH_CHARTJOB_START()
//			SetChartSelected(CPoint(nColIndex,nRowIndex));
//			GetpChartItemSelected()->SetUseRevisedValue(bUseRevised);
//			if(!bDontRequestData) RequestChartData();
//		EACH_CHARTJOB_END()
//		SetChartSelected(CPoint(lColSelected,lRowSelected));
	}
	m_bUpdatingShowingFackedCandle = FALSE;
	return TRUE;
}

BOOL CChartMng::UpdateRevisedValue(BOOL bUseRevised,BOOL bDontRequestData)
{
	m_bUpdatingRevisedValue = TRUE;

	//2005. 08. 22 ���� �ְ� ���õ� ��Ʈ �ϳ��� �����Ѵ�. ====================================
	if(!GetpChartItemSelected()) return FALSE;
	//�����ְ����� ���� ���� �� ���� ����
	if(GetpChartItemSelected()->GetUseRevisedValue() == bUseRevised)
		return TRUE;

	GetpChartItemSelected()->SetUseRevisedValue(bUseRevised);
	if(!bDontRequestData)
		RequestChartData();
	
	if(m_bChartExtended)
		m_bUpdatedInChartExtended = TRUE;	
	//========================================================================================

/*	if(m_bChartExtended)
	{
		if(!GetpChartItemSelected()) return FALSE;
		GetpChartItemSelected()->SetUseRevisedValue(bUseRevised);
		if(!bDontRequestData) RequestChartData();
		m_bUpdatedInChartExtended = TRUE;
	}
	else
	{
		InitInLoop();
		m_bUseRevisedInLoop = bUseRevised;
		m_bDontRequestDataInLoop = bDontRequestData;
		m_lTypeInLoop = LOOPTYPE_REVISED; //3 RevisedValue
		HeadInLoop();

//		long lColSelected = m_lColSelected;
//		long lRowSelected = m_lRowSelected;
//		EACH_CHARTJOB_START()
//			SetChartSelected(CPoint(nColIndex,nRowIndex));
//			GetpChartItemSelected()->SetUseRevisedValue(bUseRevised);
//			if(!bDontRequestData) RequestChartData();
//		EACH_CHARTJOB_END()
//		SetChartSelected(CPoint(lColSelected,lRowSelected));
	}
*/
	m_bUpdatingRevisedValue = FALSE;
	return TRUE;
}


//DEL BOOL CChartMng::UpdateRateValue(BOOL bUseRateValue,BOOL bDontRequestData)
//DEL {
//DEL 	if(!GetpChartItemSelected()) return FALSE;	
//DEL 	GetpChartItemSelected()->SetUseRateValue(bUseRateValue);
//DEL 	if(!bDontRequestData) RequestChartData();
//DEL 	return TRUE;
//DEL }

//05.06. 22 add by sy, nam
//������ ��� ����� �ƴ� ���°� 3��(\, $, %)�� �Ǿ� �� �߰��� �Լ� 
BOOL CChartMng::UpdateNumericChartValue(int nNumericChartValue, BOOL bDontRequestData)
{
	if(!GetpChartItemSelected()) return FALSE;	
	GetpChartItemSelected()->SetChartNumericValue(nNumericChartValue);
	// �������� �߰� : �������� - ojtaso (20070118)
	if(!bDontRequestData) RequestChartData(TRUE);
	return TRUE;
}

BOOL CChartMng::ShowScale()
{
	if(m_bHideScale)
	{
//		double dStartDate = 0, dEndDate = 0;
//		GetpChartItemSelected()->GetStartEndDateOnView(&dStartDate,&dEndDate);
//		char chType = GetpChartItemSelected()->GetType();
		EACH_CHARTJOB_START()
			if(pChartItem->GetDataCode()!="")
			{
				pChartItem->HideScale(m_bHideScale);
//				if(pChartItem->GetType()==chType)
//				{
//					pChartItem->SetStartEndDateOfView(dStartDate,dEndDate);						
//				}
			}
		EACH_CHARTJOB_END()			
	}
	else
	{
		EACH_CHARTJOB_START()
			if(!pChartItem->GetDataCode().IsEmpty())
			{
				pChartItem->HideScale(m_bHideScale);
			}
		EACH_CHARTJOB_END()			
	}
	return TRUE;
}

BOOL CChartMng::ShowInnerToolbar()
{
	if(m_bHideInnerToolBar)
	{
		EACH_CHARTJOB_START()
			if(pChartItem->GetDataCode()!="")
			{
				pChartItem-> HideInnerToolBar(m_bHideInnerToolBar);
			}
		EACH_CHARTJOB_END()			
	}
	else
	{
		EACH_CHARTJOB_START()
			if(!pChartItem->GetDataCode().IsEmpty())
			{
				pChartItem->HideInnerToolBar(m_bHideInnerToolBar);
			}
		EACH_CHARTJOB_END()			
	}
	return TRUE;
}
/*
#define TICK_DATA_CHART		'0'
#define MIN_DATA_CHART		'1'
#define DAY_DATA_CHART		'2'
#define WEEK_DATA_CHART		'3'
#define MONTH_DATA_CHART	'4'
#define YEAR_DATA_CHART		'5'
*/

BOOL CChartMng::AddUpjongData(CString strCode)
{
	if(!GetpChartItemSelected()) return FALSE;
	if(!GetpChartItemSelected()->GetCntOfItemData()||GetpChartItemSelected()->GetOnRunningOneChart()) 
	{
		return FALSE;
	}
	// ƽ(0)/��(1)/��(2)/��(3)/��(4)
	if(	  GetpChartItemSelected()->GetType() == TICK_DATA_CHART
		||GetpChartItemSelected()->GetType() == MIN_DATA_CHART
		) 
	{
		GetParent()->SendMessage(UMSG_SENDTOMSGBOX,(WPARAM)&ERRORMSG2);
		return FALSE;
	}
//==================================================================================
// long�� TRCode�� ������� �����Ƿ� �ּ�ó���Ѵ�. 2005.05.06 
//==================================================================================
	if(GetpChartItemSelected()->HasChartItemData(strCode, QUERY_STRUPJONG, FALSE))
	{
		GetpChartItemSelected()->RemoveChartItemData(strCode,QUERY_STRUPJONG);
	}
	else
	{
		//2005.05.06 by sy,nam
		GetpChartItemSelected()->SetCodeForQuery(strCode, QUERY_STRUPJONG, FALSE);
		RequestChartData();
	}
//==================================================================================
	return TRUE;
}


// OCX�κ��� ���� ��ǥ TR�� ������ �� ��Ȳ�� �Ǹ�
// �� �Լ��� Ÿ���� �Ѵ�.
//BOOL CChartMng::MarketTRSend_By_OCXSignal(CString strCode, UINT nID, int nCount, BOOL bIsNew)
BOOL CChartMng::MarketTRSend_By_OCXSignal(	int nID,						//ChartOCX WindowID
											const LPCTSTR pcszPacketNames,	//�ش� ������ǥ ��Ŷ ��
											int nCount,						//��û�� ����
											const BOOL bIsNew				//TRUE�� �ű���ȸ, FALSE�� ������ȸ
											)
{
	CString strCode;
	strCode.Format("%s", pcszPacketNames);
	
	if(!strCode.GetLength()) 
	{
#ifdef _DEBUG
		AfxMessageBox("MarketTRSend_By_OCXSignal \n �߰��� ������ǥ ��Ŷ���� ����.");
#endif 
		return FALSE;
	}

	CChartItem* pSelChartItem = NULL;
	EACH_CHARTJOB_START()
		if( pChartItem->GetDlgCtrlID() == nID)
		{
			pSelChartItem = pChartItem;
			goto ExitLoop3; 
		}
	EACH_CHARTJOB_END()

ExitLoop3:

	if(pSelChartItem == NULL) return FALSE;

	if(!pSelChartItem->GetCntOfItemData()||pSelChartItem->GetOnRunningOneChart()) 
	{
#ifdef _DEBUG
		AfxMessageBox("MarketTRSend_By_OCXSignal \n ��Ʈ �����Ͱ� ���ų� TR�� �ް� �ִ� ���̴�");
#endif 
		return FALSE;
	}

	//���� ��ǥ �߰��� ��Ʈ�� �ε�ǰ� �ִ� �����̸�
	//��û�� Ÿ���� �˻��ϰ�
	//�̹� �ε尡 ���� �����̸� ���� ��Ʈ�� ������ Ÿ���� üũ�Ѵ�.
	//05. 10. 27
	if(pSelChartItem->GetType() != NULL)
	{
		//>>0000550: 0503_����������Ʈ�� ������ǥ�� �̰������� ��ǥ�� ���ֱ� �̿��� ��� �ֱ⿡���� ��ȸ�ǵ��� ������û
		if(	pSelChartItem->GetType() != DAY_DATA_CHART )
		//if(	strCode.Find(";_SuspenseContract_;")<0 && pSelChartItem->GetType() != DAY_DATA_CHART )
		{
#ifdef _DEBUG
		AfxMessageBox("MarketTRSend_By_OCXSignal \n ������ǥ�� �ϰ����� ����ȴ� #1");
#endif 

			return FALSE;
		}			
	}
	else
	{
		if(	pSelChartItem->GetTypeRequested() != DAY_DATA_CHART )
		{
#ifdef _DEBUG
		AfxMessageBox("MarketTRSend_By_OCXSignal \n ������ǥ�� �ϰ����� ����ȴ� #2");
#endif 
			return FALSE;
		}
	}
//	if(pSelChartItem->GetDataTRCode() != QUERY_STRJONGMOK)
//		return FALSE;

	//if(strCode.Find("�̰���") >= 0)
	if(strCode.Find(";_SuspenseContract_;")>0)
	{
		if(pSelChartItem->GetDataTRCode() != QUERY_STRFUTOPT/* && pSelChartItem->GetDataTRCode() != QUERY_STRCOMMODITY*/)		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
			return FALSE;
	}
	else if (strCode.Find(";_ADL_;") > 0 || strCode.Find(";_ADR_;") > 0)
	{
		if(pSelChartItem->GetDataTRCode() != QUERY_STRUPJONG)
			return FALSE;
	}
	else
	{
		if(pSelChartItem->GetDataTRCode() != QUERY_STRJONGMOK)
			return FALSE;
	}

	CString strTR = QUERY_STRMARKET;
	//=================================================================
	//�ߺ��ؼ� ���� ������ǥ �׸��� TR�� ���� ��� ���ֹ�����.
	//2005.11.01
	//=================================================================
	pSelChartItem->RemoveChartItem_OneData(strCode,strTR);
	//=================================================================

	if(!strCode.GetLength()) 
	 	return FALSE;
	
	pSelChartItem->GetChartItemMarketDataNew(strTR, strCode);		
	
	RequestChart_MarketData(strTR, strCode, pSelChartItem, nCount, bIsNew);
	

	return TRUE;
}

BOOL CChartMng::AddMarketData(CString strCode, BOOL bFromTree)
{

/*	CChartItem* pSelChartItem = NULL;
	pSelChartItem = GetpChartItemSelected();

	if(pSelChartItem == NULL) return FALSE;
	
	
	if(!pSelChartItem->GetCntOfItemData()||pSelChartItem->GetOnRunningOneChart()) 
	{
		return FALSE;
	}
	//���� ��ǥ �߰��� ��Ʈ�� �ε�ǰ� �ִ� �����̸�
	//��û�� Ÿ���� �˻��ϰ�
	//�̹� �ε尡 ���� �����̸� ���� ��Ʈ�� ������ Ÿ���� üũ�Ѵ�.
	//05. 10. 27
	if(pSelChartItem->GetType() != NULL)
	{
		if(	pSelChartItem->GetType() != DAY_DATA_CHART )
		{
			return FALSE;
		}			
	}
	else
	{
		if(	pSelChartItem->GetTypeRequested() != DAY_DATA_CHART )
		{
			return FALSE;
		}
	}

	if(strCode.Find("����") < 0)  //���� ���� ��ǥ�� �ƴ϶�� 
	{
		if(pSelChartItem->HasChartItemData(strCode,QUERY_STRMARKET,FALSE))
		{
			pSelChartItem->RemoveChartItemData(strCode,QUERY_STRMARKET);
		}
		else
		{
//			//=================================================================
//			//�ߺ��ؼ� ���� ������ǥ �׸��� TR�� ���� ��� ���ֹ�����.
//			//2005.11.01
//			//=================================================================
//			pSelChartItem->RemoveChartItem_AllData(strCode,QUERY_STRMARKET);
//			//=================================================================


			if(!strCode.GetLength()) 
				return FALSE;

			pSelChartItem->SetCodeForQuery(strCode,QUERY_STRMARKET,FALSE);
//			if(m_bLastEnvUpdated) //�ε尡 ������ �� ��û�Ѵ�.
//			{
//#ifdef _DEBUG
//		CString sTemp;
//		sTemp.Format("Query ���� %s", strCode);
//		OutputDebugString(sTemp);
//#endif
//				
//			}
//
//			if(m_bMarketJipyoAdding)
//			{
				RequestChartData();
//			}
		}
	}
	else
	{		
		if(pSelChartItem->HasChartItemData(strCode,QUERY_STRMARKET_NEWS,FALSE))
		{
			pSelChartItem->RemoveChartItemData(strCode,QUERY_STRMARKET_NEWS);
		}
		else
		{
			pSelChartItem->SetCodeForQuery(strCode,QUERY_STRMARKET_NEWS,FALSE);
//			if(m_bLastEnvUpdated) //�ε尡 ������ �� ��û�Ѵ�.
//			{
//#ifdef _DEBUG
//		CString sTemp;
//		sTemp.Format("Query ���� %s", strCode);
//		OutputDebugString(sTemp);
//#endif
//				//RequestChartData();
//			}
//
//			if(m_bMarketJipyoAdding)
//			{
				RequestChartData();
//			}
		}
	}
//==================================================================================
*/
	return TRUE;
}




BOOL CChartMng::ReplaceChart(long lRow1,long lCol1, long lRow2, long lCol2)
{
	if(!m_pChartItems[lRow1][lCol1]||!m_pChartItems[lRow2][lCol2]) 
		return FALSE;

	CChartItem *pChartItemSelected = m_pChartItems[lRow1][lCol1];
	m_pChartItems[lRow1][lCol1] = m_pChartItems[lRow2][lCol2];
	m_pChartItems[lRow1][lCol1]->SetRowNCol(lRow1,lCol1);
	m_pChartItems[lRow2][lCol2] = pChartItemSelected;	
	m_pChartItems[lRow2][lCol2]->SetRowNCol(lRow2,lCol2);
	InvalidateRect(GetRectOfChart(lRow1,lCol1,1));
	m_pChartItems[lRow1][lCol1]->MoveWindow(GetRectOfChart(lRow1,lCol1));
	InvalidateRect(GetRectOfChart(lRow2,lCol2,1));
	m_pChartItems[lRow2][lCol2]->MoveWindow(GetRectOfChart(lRow2,lCol2));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	����ִ� ��Ʈ�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartMng::SelectEmptyPos()
{
	long nRowIndexOld = -1;
	long nColIndexOld = -1;
	EACH_CHARTJOB_START()
		if(pChartItem->IsEmpty())
		{
			SetChartSelected(CPoint(nColIndex,nRowIndex));
			return TRUE;
			break;
		}
	EACH_CHARTJOB_END()
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��Ʈ�� �ִ�ȭ �Ѵ�.
//////////////////////////////////////////////////////////////////////////////
void CChartMng::ExtendChart(BOOL bExtend/*= TRUE*/)
{
	m_bChartExtended = bExtend;
	MoveCharts();
	Invalidate();

	
	// ȣ�� â Show ---------------------------------------------------
	// 2005. 04. 28 by Nam
	//-----------------------------------------------------------------
	//HWND hParent = GetParent()->GetSafeHwnd();		
	//::SendMessage(hParent, UMSG_FORCE_SHOW_HOGAWND, m_bChartExtended, 0 );		
	//-----------------------------------------------------------------
	
	// to update alot things which didn't do due to extended mode..
	if(!m_bChartExtended&&(m_bJongmokLink||m_bTypeLink||m_bUpdatedInChartExtended))
	{
		if(!GetpChartItemSelected()) return ;
		InitInLoop();		
		m_bNeedToUpdateInLoop = FALSE;
		m_bUseRevisedInLoop = GetpChartItemSelected()->GetUseRevisedValue();
		m_bNormalizedMinuteTimeRuleInLoop = GetpChartItemSelected()->GetBNormalizedMinuteTimeRule();
		m_strCodeInLoop = GetpChartItemSelected()->GetDataCode();
		m_strEndDateInLoop = GetpChartItemSelected()->GetEndDate();
		m_chTypeInLoop = GetpChartItemSelected()->GetType();
		m_lTickMinNumberInLoop = atol(GetpChartItemSelected()->GetTickMinDayNumber());
		m_lTypeInLoop = LOOPTYPE_UPDATE_BY_EXTEND/*5*/; //updateByExtend
		HeadInLoop();	
	}
}

BOOL CChartMng::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nCode == CN_COMMAND)
	{
		long nCmdID = nID%10;
		UINT nChartID = nID - nID%10;
		EACH_CHARTJOB_START()
			if(pChartItem->GetID()==nChartID)
			{
				switch(nCmdID)
				{
				case 1:	// Maxmize
					SetChartSelected(CPoint(nColIndex,nRowIndex));
					ExtendChart(!m_bChartExtended);
					break;
				case 2:	// Initial
					//100204-alzioyes �ʱ�ȭ�϶��� "ȣ"�� ������ �ʰ� ó��.
					// ���̴� �� CStdDialog.WindowProc.switch (UMSG_SENDTR_HOGA_DATA)����.
					if(m_pFloatingDlg && m_pFloatingDlg->IsWindowVisible())
						m_pFloatingDlg->ShowWindow(SW_HIDE);

					SetChartSelected(CPoint(nColIndex,nRowIndex));
					ResetOCX();	
					if(m_pInputBarDlg)
					{
						CString szCode;
						m_pInputBarDlg->SendMessage(UMSG_SETTEXT_CODE_INPUTBAR, 9999, (LPARAM)(LPCSTR)szCode);
					}
						
					//===================================================================
					// ƽ �� �� �� �� �� ���� ���� ��Ʈ��  �̳��� ��� NULL�� ���ش�.
					// �� �� �ָ� ���� ��Ʈ ���°� ��������.
					// 05.11.24
					//===================================================================
					switch(pChartItem->GetType())
					{			
						case TICK_DATA_CHART:
							m_pTickCfgChart = NULL;
							break;
						case MIN_DATA_CHART:
							m_pMinCfgChart = NULL;
							break;
						case DAY_DATA_CHART:
							m_pDayCfgChart = NULL;
							break;
						case HOUR_DATA_CHART:
							m_pHourCfgChart = NULL;
							break;
						case WEEK_DATA_CHART:
							m_pWeekCfgChart = NULL;
							break;
						case MONTH_DATA_CHART:
							m_pMonthCfgChart = NULL;
							break;
						case YEAR_DATA_CHART:
							m_pYearCfgChart = NULL;
							break;
						case SEC_DATA_CHART:			
							m_pSecCfgChart = NULL;
							break;
//						case SEC10_DATA_CHART:			
//						case SEC30_DATA_CHART:
//							m_pSecCfgChart = NULL;
//							break;
					}					
					//===================================================================

					//@�����߰�
					{
						CChartItem* pInitChartItem = GetpChartItemSelected();
						if(pInitChartItem)
							pInitChartItem->m_bNowInitialState = TRUE;
					}
					//@�����߰�

					//CDRChartOcx::ResetChartCfgWith(140);
					//2005. 07. 28 ==========================
					// ���� ������ 1 * 1 �Ǵ� �ϳ��� â�� ���� ��� 
					// ȣ��â ���� Clear & Init
					break;
				case 3:	
					SetChartSelected(CPoint(nColIndex,nRowIndex));		
					SetTimer(1, 100, NULL);										
					break;
//@��������
//				case 4:	// Check
//					SetChartSelected(CPoint(nColIndex,nRowIndex));	
//					EACH_CHARTJOB_START()
//						if(!GetpChartItemSelected()) break;
//						if(GetpChartItemSelected()!=pChartItem&&GetpChartItemSelected()->GetDataCode()==pChartItem->GetDataCode())
//						{
//							pChartItem->SetCheck(GetpChartItemSelected()->GetCheck());
//						}
//					EACH_CHARTJOB_END()					
//					m_pLeftBarDlg->SendMessage(UMSG_SENDJCODECHECKED,GetpChartItemSelected()->GetCheck(),(LPARAM)(LPCTSTR)GetpChartItemSelected()->GetDataCode());
//					break;
//@��������
				default:
					break;				
				}
			}
		EACH_CHARTJOB_END()
	}
	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void  CChartMng::UnhighlightJCode(CChartItem *pChartItemIn)
{
	CString strJongmokCode = pChartItemIn->GetDataCode();
	EACH_CHARTJOB_START()
		if(pChartItemIn!=pChartItem)
		{
			if(pChartItem->GetDataCode()==strJongmokCode)
			{
				return;
			}
		}
	EACH_CHARTJOB_END()					
	if(m_pLeftBarDlg)
	{
		m_pLeftBarDlg->SendMessage(UMSG_SENDJCODSYNHIGHLIGHTED,(WPARAM)(LPCTSTR)strJongmokCode);	
	}	
}
///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��������ÿ� (�����ڵ�,�ð�)�� ���ٷ� ������.
//////////////////////////////////////////////////////////////////////////////
void  CChartMng::SetMultiCodes(CString strMultiCodes)
{
	if(m_bJongmokLink)
	{
		m_bJongmokLink = 0;
		if( m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd() )
			m_pExtInputBarDlg->SendMessage(UMSG_SETJONGMOKLINK,(WPARAM)m_bJongmokLink);
	}

	m_strMultiCodes = strMultiCodes;
	CString strAData;
	CString strJCode;
	CString strTimeMark;
	int nPos = m_strMultiCodes.Find(';',0);	
	strAData = m_strMultiCodes.Left(nPos);
	int nPos1 = strAData.Find(',',0);	
	strJCode = strAData.Left(nPos1);
	strAData.Delete(0,nPos1+1);
	strAData.Remove(':');
	m_strTimeMarkReserved = strAData;
	m_strMultiCodes.Delete(0,nPos+1);

//@�����߰�
	((CStdDialog*)GetParent())->ChangeCode(strJCode);
//@�����߰�
//@��������
//	if(m_pInputBarDlg)
//	{
//		STINPUTCODEDATA2	stInputCodeData2;
//		stInputCodeData2.m_strCode = strJCode;
//		//if(stInputCodeData2.m_strCode.GetLength()==CODE_STOCK_LENGTH2)
//		//	stInputCodeData2.m_strCode.Format("%s ", strJCode);
//		stInputCodeData2.m_bDontPutInShared = TRUE;
//		stInputCodeData2.m_bRequestWithoutHesitation = TRUE;
//		m_pInputBarDlg->SendMessage(UMSG_INPUTCODETOINPUTBAR,(WPARAM)&stInputCodeData2);
//	}
//@��������
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	����������ȸ�� ������������ ��ȸ�Ѵ�.
//////////////////////////////////////////////////////////////////////////////
BOOL   CChartMng::SetJCodesNext()
{
	if(!m_strMultiCodes.IsEmpty())
	{
		if(m_nMode == CHANGE_CHART)
		{
			// �������� �߰� UI : �������� - ojtaso (20070522)
			if(!m_bAddModeFromTab && !SelectNextPos())
			{
				m_strMultiCodes.Empty();
				return FALSE;
			}
		}
		SetMultiCodes(m_strMultiCodes);
		return TRUE;
	}	

	// �������� �߰� UI : �������� - ojtaso (20070522)
	if(m_bAddModeFromTab)
	{
		CChartItem* pChartItem = NULL;
		pChartItem = GetpChartItemSelected();
		if(pChartItem)
			pChartItem->SetResetTotalCountWhileDraw(TRUE);

		m_bAddModeFromTab = FALSE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	������ ��Ʈ�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartMng::SelectNextPos()
{
	long lColSelected = m_lColSelected;
	long lRowSelected = m_lRowSelected;

	if((m_lColSelected+1)>=m_lCols)
	{
		if((m_lRowSelected+1)>=m_lRows)
		{
			return FALSE;
		}
		else
		{
			lColSelected = 0;
			lRowSelected++;			
		}
	}
	else
	{
		lColSelected++;
	}
	SetChartSelected(CPoint(lColSelected,lRowSelected));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��Ʈ������ ��ȸ�Ѵ�.
//////////////////////////////////////////////////////////////////////////////
// �������� �߰� : �������� - ojtaso (20070118)
BOOL CChartMng::RequestChartData(BOOL bChangeType/* = FALSE*/)
{
// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CChartMng::RequestChartData:Begin\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}

	m_bChangeTypeMode = bChangeType;

	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();
	if(!pChartItem) return FALSE;
	// �������� �ҷ����� : �������� - ojtaso (20070514)
	CString strPacket = pChartItem->IsLoadingRQ() ? pChartItem->GetLoadedQueryData() : pChartItem->GetQueryData(bChangeType);

//	OutputDebugString("Send" + strPacket);

	if(strPacket=="")	
	{
// 		{
// 			CString szDebug;
// 			szDebug.Format("[DRCS][LOAD](%8d) CChartMng::RequestChartData:End\n", __LINE__);
// 			OutputDebugString(szDebug);
// 		}
		return FALSE;
	}

	stSend2Server st;
	st.m_nType = 0;
	st.m_pWnd = this;
	st.m_pBuffer = (void*)(LPCTSTR)strPacket;
	st.m_lLength= strPacket.GetLength();
	st.m_strTRCode = GetpChartItemSelected()->GetTrCodeForQuery();

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	((CStdDialog*)GetParent())->WriteToStringLog(_T("�������� ���� Data ����"), pChartItem->GetSelectedItemCode(), pChartItem->GetSelectedItemCode().GetLength());
#endif
//	CRect rect;
//	GetWindowRect(rect);
//	InvalidateRect(rect);
	GetParent()->SendMessage(UMSG_SEND2SERVER,0,(long)&st);	

	if(strPacket=="")	
	{
// 		{
// 			CString szDebug;
// 			szDebug.Format("[DRCS][LOAD](%8d) CChartMng::RequestChartData:End\n", __LINE__);
// 			OutputDebugString(szDebug);
// 		}
		return FALSE;
	}

	return TRUE;
}

BOOL CChartMng::RequestChart_MarketData(CString		strTRCode,
										CString		strJipyoName,
										CChartItem* pSelChartItem,
										int			nDataRqst,
										BOOL		bIsNew
										)
{
	if(pSelChartItem == NULL) return FALSE;

	CString strPacket = pSelChartItem->GetMarketQueryData(strJipyoName, nDataRqst, bIsNew);
	if(strPacket=="")	
	{
		return FALSE;
	}
	
	stSend2Server st;
	st.m_nType			= 0;
	st.m_pWnd			= this;
	st.m_pBuffer		= (void*)(LPCTSTR)strPacket;
	st.m_lLength		= strPacket.GetLength();
	st.m_strTRCode		= strTRCode;

	GetParent()->SendMessage(UMSG_SEND2SERVER,0,(long)&st);	

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��Ʈ���� �ǽð� ������ ó���Ѵ�.
//////////////////////////////////////////////////////////////////////////////
void CChartMng::SetRealJPriceData(void *pRealData)
{
	STREALTYPEDATA sndData;

#ifdef _WRITELOG
	((CStdDialog*)GetParent())->WriteToStringLog("CChartMng::SetRealJPriceData::Begin", _T("��Ƽ��Ʈ �ü�"), 13);
#endif
	CRealData_Common* pRealData_Common = (CRealData_Common*)pRealData;

// 	if (pRealData_Common->m_strPrice.IsEmpty()==TRUE ||
// 		pRealData_Common->m_strValue.IsEmpty()==TRUE)
// 		return;

	//[SMalzio]110105 ��ǰ���� ������Ʈ������ �Ҽ��� ó���ϵ��� ����.
	//20110124_alzioyes:FO��Ʈ(FUOPT_JIPYO_CHART) �����߰�.
	if(	pRealData_Common->m_nMarketType == CMasterTypeInfo::UPJONG_CHART 
		|| pRealData_Common->m_nMarketType == CMasterTypeInfo::FUTOPT_CHART
		|| pRealData_Common->m_nMarketType == CMasterTypeInfo::FOREIGN_CHART 
		|| pRealData_Common->m_nMarketType == CMasterTypeInfo::FUOPT_JIPYO_CHART )
	{
		
		double dData = atof(pRealData_Common->m_strPrice)/100.;
		pRealData_Common->m_strPrice.Format("%.2f", dData);
		
		dData = atof(pRealData_Common->m_strOpen)/100.;
		pRealData_Common->m_strOpen.Format("%.2f", dData);
		
		dData = atof(pRealData_Common->m_strHigh)/100.;
		pRealData_Common->m_strHigh.Format("%.2f", dData);
		
		dData = atof(pRealData_Common->m_strLow)/100.;
		pRealData_Common->m_strLow.Format("%.2f", dData);
	}
	else if(pRealData_Common->m_nMarketType == CMasterTypeInfo::COMMODITY_CHART)
	{
		//>>[A00000545] ��ǰ���� �ڸ����� ���� �ٸ��Ƿ� ���� ó��.
		if(m_szCommodityUnit.GetLength()==0)
		{
			char aBuff[255+1]={0,};
			IMainInfoManager* pManager = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
			CString szCFG;
			szCFG.Format("%s\\CodeItem.cfg", pManager->GetDataDir());

			::GetPrivateProfileString("CHART_UNIT", "COMMODITY_UNIT", "185;186;187;", aBuff, 255, szCFG );
			m_szCommodityUnit = aBuff;
		}
		double nOption = 100.0;
		CString strShortCode = pRealData_Common->m_strCode.Left(3);
		strShortCode += ";";
		//if(strShortCode.Compare("185")==0 || strShortCode.Compare("186")==0)
		//	nOption = 1.0;
		if(m_szCommodityUnit.Find(strShortCode) != -1) nOption = 1.0;
		//<<[A00000545]

		if(nOption==1.0)
		{
			long dData = atof(pRealData_Common->m_strPrice)/nOption;
			pRealData_Common->m_strPrice.Format("%ld", dData);
			
			dData = atof(pRealData_Common->m_strOpen)/nOption;
			pRealData_Common->m_strOpen.Format("%ld", dData);
			
			dData = atof(pRealData_Common->m_strHigh)/nOption;
			pRealData_Common->m_strHigh.Format("%ld", dData);
			
			dData = atof(pRealData_Common->m_strLow)/nOption;
			pRealData_Common->m_strLow.Format("%ld", dData);
		}
		else
		{
			double dData = atof(pRealData_Common->m_strPrice)/nOption;
			pRealData_Common->m_strPrice.Format("%.2f", dData);
			
			dData = atof(pRealData_Common->m_strOpen)/nOption;
			pRealData_Common->m_strOpen.Format("%.2f", dData);
			
			dData = atof(pRealData_Common->m_strHigh)/nOption;
			pRealData_Common->m_strHigh.Format("%.2f", dData);
			
			dData = atof(pRealData_Common->m_strLow)/nOption;
			pRealData_Common->m_strLow.Format("%.2f", dData);
		}
	}

	EACH_CHARTJOB_START()
		if(/*pChartItem->GetDataCode()==szTemp0&&*/pChartItem->IsOnDrdsAdvise())			
		{
			int nRealMarketType;
			//CString strItemCode = pChartItem->GetDataCode();
			CString strItemCode = pRealData_Common->m_strCode;
//OutputDebugString(strItemCode);
			int nMarketType = ((CStdDialog*)GetParent())->GetMarketTypeFromCode(strItemCode, nRealMarketType);
			if(nMarketType == CMasterTypeInfo::STOCK_CHART)
				pChartItem->SetRealPriceData( pRealData, STOCK_REAL);
			else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
				pChartItem->SetRealPriceData( pRealData, UPJONG_REAL);
			else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
				pChartItem->SetRealPriceData( pRealData, FUTURE_REAL);
			else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
				pChartItem->SetRealPriceData( pRealData, SFUTURE_REAL);
			else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
				pChartItem->SetRealPriceData( pRealData, UPJONG_REAL);
			//20110124_alzioyes:FO��Ʈ(FUOPT_JIPYO_CHART) �����߰�.
			else if(nMarketType == CMasterTypeInfo::FUOPT_JIPYO_CHART)
				pChartItem->SetRealPriceData( pRealData, FO_REAL);
			else if(nMarketType == CMasterTypeInfo::COMMODITY_CHART)
				pChartItem->SetRealPriceData( pRealData, COMMODITY_REAL);
			else
				pChartItem->SetRealPriceData( pRealData, STOCK_REAL);

			sndData.nDataType = nMarketType;
			sndData.szCode = (LPSTR)(LPCSTR)pRealData_Common->m_strCode;
			sndData.oPrice = (LPSTR)(LPCSTR)pRealData_Common->m_strOpen;
			sndData.hPrice = (LPSTR)(LPCSTR)pRealData_Common->m_strHigh;
			sndData.lPrice = (LPSTR)(LPCSTR)pRealData_Common->m_strLow;
			sndData.cPrice = (LPSTR)(LPCSTR)pRealData_Common->m_strPrice;
			sndData.sign = (LPSTR)(LPCSTR)pRealData_Common->m_strSign;
			pChartItem->InvokeAddIn("ChartGuideLineAddIn.dll:GUIDE_LINE", INVOKEMSG_RECEIVEREALDATA, (long)&sndData);
			pChartItem->InvokeAddIn("ChartGuideLineAddIn.dll:LOWHIGH_MARKER", INVOKEMSG_RECEIVEREALDATA, (long)&sndData);
		}
	EACH_CHARTJOB_END()

#ifdef _WRITELOG
	((CStdDialog*)GetParent())->WriteToStringLog("CChartMng::SetRealJPriceData::End", _T("��Ƽ��Ʈ �ü�"), 13);
#endif
}

void CChartMng::SetRealYPriceData(void *pRealData)
{
	int nRealMarketType;
	EACH_CHARTJOB_START()
		if(pChartItem->IsOnDrdsAdvise())			
		{
			int nMarketType = ((CStdDialog*)GetParent())->GetMarketTypeFromCode(pChartItem->GetDataCode(), nRealMarketType);
			if(nMarketType == CMasterTypeInfo::STOCK_CHART)
				pChartItem->SetRealYPriceData( pRealData, STOCK_REAL);
			else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
				pChartItem->SetRealYPriceData( pRealData, UPJONG_REAL);
			else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
				pChartItem->SetRealYPriceData( pRealData, FUTURE_REAL);
			else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
				pChartItem->SetRealYPriceData( pRealData, FUTURE_REAL);
			else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
				pChartItem->SetRealYPriceData( pRealData, UPJONG_REAL);
			else
				pChartItem->SetRealYPriceData( pRealData, STOCK_REAL);
		}
	EACH_CHARTJOB_END()
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	�ش������� �ǽð��� ���ϴ��� Ȯ���Ѵ�.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartMng::IsAdvisingCodeOf(LPCTSTR lpCode)
{
	EACH_CHARTJOB_START()
		if(pChartItem->GetDataCode()==lpCode)
		{
			return TRUE;
		}		
	EACH_CHARTJOB_END()
	return FALSE;
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
void CChartMng::AddRealCodeList(LPCTSTR lpszCode)
{
	((CStdDialog*)GetParent())->AddRealCodeList(lpszCode);
}

// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
void CChartMng::OnChartMngCodeAdvise(CChartItem *pItem, BOOL bAddMode)
{
	((CStdDialog*)GetParent())->OnCodeAdvise(pItem->GetDataCode(), bAddMode);
}

// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
void CChartMng::OnChartMngCodeUnAdvise(CChartItem *pItem, BOOL bAddMode)
{
	EACH_CHARTJOB_START()
		if(pChartItem!=pItem)			
		{
			//if(pItem->GetDataCode()==pChartItem->GetDataCode())
			//{
			//	return;
			//}	
			
			if(pChartItem->GetDataCode()==pItem->GetOldChartCode())
			{
				return;
			}	
		}
	EACH_CHARTJOB_END()
	//((CStdDialog*)GetParent())->OnCodeUnAdvise(pItem->GetDataCode());

	//05. 09. 07 =================================================================================
	//���� �ڵ带 �������� �ʰ� �־ ���� ��Ű�� ��ƾ �߰�
	CString strOldCode = pItem->GetOldChartCode(); strOldCode.TrimLeft(); strOldCode.TrimRight();
	if(strOldCode.GetLength())
		((CStdDialog*)GetParent())->OnCodeUnAdvise(strOldCode, bAddMode);
	//============================================================================================
}

void CChartMng::ChartItemUnAdvise(CChartItem *pItem, BOOL bAddMode)
{
	//05. 09. 07 =================================================================================
	//���� �ڵ带 �������� �ʰ� �־ ���� ��Ű�� ��ƾ �߰�
	CString strOldCode = pItem->GetOldChartCode(); strOldCode.TrimLeft(); strOldCode.TrimRight();
	if(strOldCode.GetLength())
		((CStdDialog*)GetParent())->OnCodeUnAdvise(strOldCode, bAddMode);
	//============================================================================================
}

//////////////////////////////////////////////////////////////////////////////
//	�۾���	:	sy, nam
//	����	:	2005. 08.02
//	����	:	��� ��Ʈ�� Real�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////////////
void CChartMng::OnAllCodeUnAdvise()
{
	EACH_CHARTJOB_START()
		if(pChartItem!=NULL)			
		{
			if(pChartItem->GetDataCode())
			{
				((CStdDialog*)GetParent())->OnCodeUnAdvise(pChartItem->GetDataCode(), pChartItem->IsAddMode());
			}			
		}
	EACH_CHARTJOB_END()
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	����� ��Ʈ�� �ҷ��´�.
//////////////////////////////////////////////////////////////////////////////
void CChartMng::LoadChart(CPoint *pPt)
{
	CString strSelected;
	CStringArray arrayFileTitle;
	CStringArray arrayFilePath;
	int nCnt = 0;
	CString strIndexFile = m_strUserPath + "\\Saved\\ChartIndex.dat";
	CString strIndex, strFileTitle, strFilePath;
	char szKey[30];

	sprintf(szKey,"%s","CHART LIST");
	long lAllCnt = ::GetPrivateProfileInt(szKey, "COUNT", -1, strIndexFile);

	for(int i=0;i < lAllCnt; i++)
	{
		strIndex.Format("%02d", i);
		::GetPrivateProfileString(szKey, strIndex, "", strFileTitle.GetBuffer(MAX_PATH), MAX_PATH, strIndexFile);	
		strFileTitle.ReleaseBuffer();

		if (strFileTitle.GetLength() == 0)
			continue;
		strFilePath = m_strUserPath +"\\"+ strFileTitle +".mct";
		arrayFileTitle.Add(strFileTitle);
		arrayFilePath.Add(strFilePath);
	}

	if(!lAllCnt)
	{
		GetParent()->SendMessage(UMSG_SENDTOMSGBOX,(WPARAM)&ERRORMSG7);
		return;
	}
	CMenu menu;
	menu.CreatePopupMenu();

	long lPosAll = 0;
	long lSize = 0;
	CString strIndicator;
	for(long lIndex = 0;lIndex<lAllCnt;lIndex++)
	{
		menu.AppendMenu(MF_STRING,(UINT)1000+lIndex,arrayFileTitle[lIndex]);
	}

	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	int nRetValue = menu.TrackPopupMenu(nFlagsForMenu, pPt->x, pPt->y, this);
	if(!nRetValue) return;

	if(nRetValue-1000<lAllCnt)
	{
		strSelected = arrayFilePath[nRetValue-1000];
	}	

	menu.DestroyMenu();
	LoadChart(strSelected);
}

BOOL CChartMng::LoadChart(CString strLoadFile)
{
// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CChartMng::LoadChart:Begin strLoadFile(%s)\n", __LINE__, strLoadFile);
// 		OutputDebugString(szDebug);
// 	}

	int nCols = 0;
	int nRows = 0;
	char szTemp[16];
	CString strTemp;
	::GetPrivateProfileString("FrameInfo","COLS","",szTemp,sizeof(szTemp),strLoadFile);
	nCols = atoi(szTemp);
	::GetPrivateProfileString("FrameInfo","ROWS","",szTemp,sizeof(szTemp),strLoadFile);
	nRows = atoi(szTemp);
	if(((CStdDialog*)GetParent())->m_nChartType==SINGLE_CHART)
	{
		if(nCols > 1 || nRows > 1) 
		{
			nCols = 1;
			nRows = 1;
		}
	}

	SetMatrix(nRows,nCols);
	if( m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd() )
		m_pExtInputBarDlg->SendMessage(UMSG_CLEARLINK);	


	//���� �� ���� ȭ���� 1�ϰ��...������ ���� ������ ����..
	//�� �ܴ� ���� ������ �����Ѵ�.=============================
	//if(nCols * nRows != 1) 
	//{
	//	m_strStatic_ShareCode = _T("");
	//	m_strCodeForLoadChart = _T("");
	//}
	//=============================================================

//	InitInLoop();
//	m_strChartSelectedInLoop = strLoadFile;
//	m_lTypeInLoop = LOOPTYPE_LOAD/*1*/;//Load	
//	HeadInLoop();


//======================================================================
	BOOL bRsylt = FALSE;

	InitInLoop();
	m_strChartSelectedInLoop = strLoadFile;
	m_lTypeInLoop = LOOPTYPE_LOAD/*1*/;//Load
//khd : ���ֿ� ��Ʈ ����ǵ��� ...���� 
//	if(((CStdDialog*)GetParent())->m_nScreenID == nScreenID_6301)
//	{
//		m_lTypeInLoop = LOOPTYPE_NULL;
//		((CStdDialog*)GetParent())->SetTimer(2,1,NULL);
//	}
	bRsylt = HeadInLoop();

// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CChartMng::LoadChart:End\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}

	if(m_lTypeInLoop == LOOPTYPE_LOAD)
		return bRsylt;

//======================================================================
	return TRUE;

}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	���ٹ�(Gsynmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	����	:	2004.06.04
//	����	:	��Ʈ������ �����Ѵ�.
//////////////////////////////////////////////////////////////////////////////
void CChartMng::SaveChart(CPoint *pPt)
{
	CChartListDlg dlg;
	CString strUserPath = m_strUserPath;
	//dlg.SetUserPath(strUserPath);
	dlg.SetUserPath(strUserPath, (LPCSTR)((CStdDialog*)GetParent())->GetScreenNo());
	dlg.m_nInfoType = 1;
	//dlg.m_pPtTopLeftOfDlg = pPt;
	dlg.m_nMarketType = m_nMarketType;
	dlg.m_pChartMng = this;
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.m_strChartSelected.IsEmpty()) return;		
		SaveChart(dlg.m_strChartSelected);
	}	
}

void CChartMng::SaveChart(CString strSaveFile)
{
#ifdef _WRITELOG
	((CStdDialog*)GetParent())->WriteToStringLog("CChartMng::SaveChart::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif
	CString strTemp;
	
//2005. 08. 27======================================================================
	if(m_pInputBarDlg)
	{
		strTemp.Format("%d",m_pInputBarDlg->SendMessage(UMSG_GETREVISED));
		::WritePrivateProfileString("InputBar","Revised",strTemp,strSaveFile);	

		strTemp.Format("%d",m_pInputBarDlg->SendMessage(UMSG_GETLOG));
		::WritePrivateProfileString("InputBar","Log",strTemp,strSaveFile);	
	}
//=====================================================================================	

	strTemp.Format("%d",m_lCols);	
	::WritePrivateProfileString("FrameInfo","COLS",strTemp,strSaveFile);

	strTemp.Format("%d",m_lRows);	
	::WritePrivateProfileString("FrameInfo","ROWS",strTemp,strSaveFile);

	EACH_CHARTJOB_START()		
		pChartItem->SaveChartInfo(strSaveFile);
	EACH_CHARTJOB_END()
#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("CChartMng::SaveChart::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
}

///////////////////////////////////////////////////////////////////////////////
//	����	:	2006.09.26
//	����	:	����� ��Ʈ�� �ҷ����� ����� ��Ʈ�� �ε��Ѵ�
//////////////////////////////////////////////////////////////////////////////
void CChartMng::LoadSaveChart(CPoint *pPt)
{
	//����Ʈ �ε� ��� ����(�ε���) by LYH 2006.09.28
	CString strSelected;
	CStringArray arrayFileTitle;
	CStringArray arrayFilePath;
	int nCnt = 0;
	CString strIndexFile;
	CString strScreenNum = ((CStdDialog*)GetParent())->GetScreenNo();
	strIndexFile.Format("%s\\Saved\\%s\\ChartIndex.dat", m_strUserPath , (LPCSTR)strScreenNum);
	CString strIndex, strFileTitle, strFilePath;
	char szKey[30];

	sprintf(szKey,"%s","CHART LIST");
	long lAllCnt = ::GetPrivateProfileInt(szKey, "COUNT", -1, strIndexFile);

	for(int i=0;i < lAllCnt; i++)
	{
		strIndex.Format("%02d", i);
		::GetPrivateProfileString(szKey, strIndex, "", strFileTitle.GetBuffer(MAX_PATH), MAX_PATH, strIndexFile);	
		strFileTitle.ReleaseBuffer();

		if (strFileTitle.GetLength() == 0)
			continue;
		strFilePath = m_strUserPath +"\\"+ strFileTitle +".mct";
		arrayFileTitle.Add(strFileTitle);
		arrayFilePath.Add(strFilePath);
	}

//	if(!lAllCnt)
//	{
//		GetParent()->SendMessage(UMSG_SENDTOMSGBOX,(WPARAM)&ERRORMSG7);
//		return;
//	}

	UINT nFlags = NULL;
	BOOL bLoadSaveCode = TRUE;
	CMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenu(MF_UNCHECKED,(UINT)101, "��� �� ����");
	menu.SetDefaultItem((UINT)101);
	if(((CStdDialog*)GetParent())->m_nChartType!=SINGLE_CHART)
	{
		if(m_pInputBarDlg)
		{
			bLoadSaveCode = m_pInputBarDlg->SendMessage(UMSG_GETLOADSAVECODE);
			if (bLoadSaveCode)
			{
				nFlags = MF_CHECKED;
			}
			else
			{
				nFlags = MF_UNCHECKED;
			}
			menu.AppendMenu(nFlags,(UINT)102,"�������� ȣ��");
		}
	}

// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
	int nSaveJipyoPerCodePeriod = ::GetCommonDataInfo(GetSafeHwnd(), COMDATA_SAVE_JIPYO_PER_CODE_PERIOD, 0);
	nFlags = MF_UNCHECKED;
	if (nSaveJipyoPerCodePeriod)		nFlags = MF_CHECKED;

//	issue note -> 0022192: [7111]������ �ֱ⺰�� ���� ������� �ʴ� ���� ����
//	if (strScreenNum == "7111")			nFlags |= MF_DISABLED;

	menu.AppendMenu(nFlags, 103, _T("�ֱ⺰ ����"));

	menu.AppendMenu(MF_SEPARATOR, (UINT)104, "");

	long lPosAll = 0;
	long lSize = 0;
	CString strIndicator;
	for(long lIndex = 0;lIndex<lAllCnt;lIndex++)
	{
		menu.AppendMenu(MF_STRING,(UINT)1000+lIndex,arrayFileTitle[lIndex]);
	}

	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	int nRetValue = menu.TrackPopupMenu(nFlagsForMenu, pPt->x, pPt->y, this);
	if(!nRetValue) return;

	menu.DestroyMenu();
	if(101 == nRetValue) // ����
	{
		SaveChart(pPt);
	}
	else if(102 == nRetValue) //����
	{
		m_pInputBarDlg->SendMessage(UMSG_SETLOADSAVECODE);
	}
	else if(103 == nRetValue) //�ֱ⺰�� ��ǥ ����
	{
		BOOL bSaveJipyoPerCodePeriod = ::GetCommonDataInfo(GetSafeHwnd(), COMDATA_SAVE_JIPYO_PER_CODE_PERIOD, 0);
		::SetCommonDataInfo(GetSafeHwnd(), COMDATA_SAVE_JIPYO_PER_CODE_PERIOD, !bSaveJipyoPerCodePeriod);
	}
	else if(nRetValue-1000<lAllCnt)	// �ҷ�����
	{
		if (theStdParent->m_nChartType == SINGLE_CHART && theStdParent->m_nMarketType == CMasterTypeInfo::STOCK_CHART)
			((CInputBarDlg*)m_pInputBarDlg)->m_bChkLoadSaveCode = TRUE;

		strSelected = arrayFilePath[nRetValue-1000];
		_DoMoveToSaveDir((LPCSTR)strSelected);


		//{{ 2007.02.05 by LYH �ҷ����� ����
		//LoadChart(strSelected);
		m_strCodeForLoadChart = GetpChartItemSelected()->GetDataCode();

		//@Solomon-A002:alzioyes. �ʱ�ȭ������ ����� ���.
		{
			ResetChartAll(FALSE);

			//StdDialog���� ��Ʈ0500 �ʱ�ȭ �߰�
			GetParent()->SendMessage(UMSG_RESETCHART);

			//@Solomon-A002:�������� �ε��϶��� ������ �ٲٰ�, �׷��� ������ �״��.
			if(bLoadSaveCode)
			{
				_DoSavedData_To_LastCode(strSelected);
			}
		}

		::WritePrivateProfileString("IMSI","LoadSaveCode", "1", strSelected);
		LoadChart(strSelected);
		m_strCodeForLoadChart = "";
		EACH_CHARTJOB_START()
			//@[����=A002] 100224
			//pChartItem->m_bLoadRQ = TRUE;
			pChartItem->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::T_ALL_TOOL_RESET_STATE, 0);
		EACH_CHARTJOB_END()
		//}}

		if (theStdParent->m_nChartType == SINGLE_CHART && theStdParent->m_nMarketType == CMasterTypeInfo::STOCK_CHART)
			((CInputBarDlg*)m_pInputBarDlg)->m_bChkLoadSaveCode = FALSE;
	}	
}

#include "../../inc/AZUtilFileMng.h"
void CChartMng::_DoMoveToSaveDir(LPCSTR szSelInfo)
{
	CString szSaveKey(szSelInfo);
	int nPos = szSaveKey.ReverseFind('\\');
	if(nPos<0) return;
	szSaveKey = szSaveKey.Mid(nPos+1);
	nPos = szSaveKey.ReverseFind('.');
	if(nPos<0) return;
	szSaveKey = szSaveKey.Left(nPos);

	CString strUserPathOrg = m_strUserPath;
	//CString strUserPath = m_strUserPath;
	//strUserPath += "\\Saved";
	CString strUserPath;
	strUserPath.Format("%s\\Saved\\%s", m_strUserPath, ((CStdDialog*)GetParent())->GetScreenNo());

	CString szExt;

	szExt.Format("%s.mct", szSaveKey);
	CAZUtil_FileManager::DoDirFileCopy(strUserPath, strUserPathOrg, szExt, FALSE);

	szExt.Format("%s_*.cht", szSaveKey);
	CAZUtil_FileManager::DoDirFileCopy(strUserPath, strUserPathOrg, szExt, FALSE);
	
	szExt.Format("IndiCond_%s_*.cht", szSaveKey);
	CAZUtil_FileManager::DoDirFileCopy(strUserPath, strUserPathOrg, szExt, FALSE);
}

BOOL CChartMng::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(pMsg->message) 
	{
		case WM_KEYDOWN:
		{
			UINT nChar = (UINT)pMsg->wParam;
			switch(nChar)
			{
				case VK_ESCAPE:
				case VK_RETURN:
				{
					return FALSE;
					break;					
				}
			}
			break;
		}
		default:
		{
			break;
		}
	}		
	//if(pMsg->wParam == VK_HOME)
	//	return FALSE;
	if(::IsWindow(m_hWnd))
		return GetParent()->PreTranslateMessage(pMsg);
	else
		return CWnd::PreTranslateMessage(pMsg);
}

void CChartMng::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bInRectOfChart = FALSE;
	CRect rect;
	EACH_CHARTJOB_START()
		rect = GetRectOfChart(nRowIndex,nColIndex,TRUE);
		if(rect.PtInRect(point))
		{
			SetChartSelected(CPoint(nColIndex,nRowIndex));
			bInRectOfChart = TRUE;
			break;
		}
	EACH_CHARTJOB_END()	
	
	if(!GetpChartItemSelected()) return ;
	if(bInRectOfChart&&GetpChartItemSelected()->GetDataTRCode()==QUERY_STRJONGMOK)
	{
		STSHAREDDATA stInfo;
		stInfo.m_szKey = "�ڵ�-����2";
		stInfo.m_szValue = GetpChartItemSelected()->GetDataCode();
		stInfo.m_szValue.TrimRight(';');
		//((IMainDllObMain*)g_pMainDll)->m_pMainFrame->SendMessage(RMSG_CREATEOPENSHAREDATA,0, (LPARAM)&stInfo);		
		CWnd::OnRButtonUp(nFlags, point);
	}
}

//@solomon 20090925 ����ȣ, ���Լ��� ȣ���ϴµ��� ����
void CChartMng::RequestAVGForGuideLine(BOOL bSelectedOnly)
{
	return;

// 	if(!GetpChartItemSelected()) return ;
// 
// 	CUIntArray AryAvgForChartGuideLine;
// 	AryAvgForChartGuideLine.SetSize(4);
// 	GetpChartItemSelected()->InvokeAddIn("ChartGuideLineAddin.dll:GUIDE_LINE",INVOKEMSG_GETAVGINFO,(long)&AryAvgForChartGuideLine);
// 	stSend2Server st;
// 	CString strPacket;
// 	if(bSelectedOnly)
// 	{
// 		strPacket = GetpChartItemSelected()->GetQueryDataForGuideLine(&AryAvgForChartGuideLine);
// 		if(strPacket=="")	return;	
// 		st.m_nType = 0;
// 		st.m_pWnd = GetpChartItemSelected();
// 		st.m_pBuffer = (void*)(LPCTSTR)strPacket;
// 		st.m_lLength= strPacket.GetLength();
// 		st.m_strTRCode = QUERY_STR_AVGMP;
// 		GetParent()->SendMessage(UMSG_SEND2SERVER,0,(long)&st);
// 	}
// 	else
// 	{
// 		EACH_CHARTJOB_START()
// 			strPacket = pChartItem->GetQueryDataForGuideLine(&AryAvgForChartGuideLine);
// 			if(strPacket=="")	continue;	
// 			st.m_nType = 0;
// 			st.m_pWnd = pChartItem;
// 			st.m_pBuffer = (void*)(LPCTSTR)strPacket;
// 			st.m_lLength= strPacket.GetLength();
// 			st.m_strTRCode = QUERY_STR_AVGMP;
// 			GetParent()->SendMessage(UMSG_SEND2SERVER,0,(long)&st);
// 		EACH_CHARTJOB_END()	
// 	}
}

void CChartMng::SetSendJongData(LPCTSTR lpQuoteCode,char chType, LPCTSTR lpMinNTickNumber)
{
/*	const int nMaxSendResultCnt = 100;
	CString strMapNo = "60000";

	STOPENMAPINFO data;
	char szHandle[9] = {0, };
	::sprintf(szHandle, "%08x", GetParent()->GetSafeHwnd());
	::memcpy(data.m_aOpenWnd, szHandle, sizeof(data.m_aOpenWnd));

	::memset(data.m_aMap, 0x00, OMID_MAPNAME);
	::strcpy(data.m_aMap, strMapNo);  // 5�ڸ�

	data.m_nOpenType = OMI_MAINMAP;

	
	CString strCodeData;	
	if(!m_lDragStart||!m_lDragEnd) 
	{
		return;
	}
	else if(m_lDragStart<m_lDragEnd) 
	{
		strCodeData.Format("%6s,%c,%s,%08d,%08d,",lpQuoteCode,chType, lpMinNTickNumber, m_lDragStart,m_lDragEnd);
	}
	else
	{
		strCodeData.Format("%6s,%c,%s,%08d,%08d,",lpQuoteCode,chType, lpMinNTickNumber, m_lDragEnd,m_lDragStart);
	}
//	CString strTest;
//	strTest = "\n[gm0604]";
//	strTest += strCodeData;
//	strTest += "\n";
//	::OutputDebugString(strTest);

	
	::memset(data.m_aOpenInfo, 0x00, OMID_OPENDATA);
	::strcpy(data.m_aOpenInfo, strCodeData);

	data.m_nInfoLen = strCodeData.GetLength();
	GetParent()->GetParent()->GetParent()->GetParent()->SendMessage(RMSG_OPENMAPWITHDATA, 0, (LPARAM)&data);
*/	
}


void CChartMng::GetIndicatorNameListIntoTreeCtrl(CTreeCtrl& rxtreeCtrl)
{
	//-----------------------------------------------------------------
	// Han Hwa ��ǥ������ �°� �����Ѵ�.
	//-----------------------------------------------------------------
	/*char* szIndexGroupName[] = { 
									"�⺻��ǥ",
									"�̵����",
									"�ְ���ǥ",
									"��Ÿ��ǥ",
									"÷����ǥ",
									"�߼���ǥ",
									"�ŷ�����ǥ",
									"��������ǥ",
									"ź�¼���ǥ"};
	//-----------------------------------------------------------------*/
	char* szIndexGroupName[] = { 
									"�⺻��ǥ",
									"������ǥ",
									"�߼���ǥ",
									"��������ǥ",
									"�ŷ�����ǥ",
									"2���Ļ���ǥ",
									"��Ÿ��ǥ",
									"������ǥ",
									""
	};

	long lAllCnt = sizeof(szIndexGroupName)/sizeof(char*);

//#ifdef _DEBUG
	//=============================================================
	// 05.10.24
	// �ֽĸ� ���� ��ǥ �߰� 
	//=============================================================
	if(m_nMarketType != CMasterTypeInfo::STOCK_CHART)
		lAllCnt -= 1;
	//=============================================================
//#endif

	rxtreeCtrl.DeleteAllItems();
	CStringArray strArrayIndicator;	
	long lSize = 0;
	HTREEITEM hRootItem = NULL;
	for(long lIndex = 0;lIndex<lAllCnt;lIndex++)
	{
		hRootItem = rxtreeCtrl.InsertItem(szIndexGroupName[lIndex]);
		GetIndicatorNameListIntoArray(&strArrayIndicator, szIndexGroupName[lIndex]);
		lSize = strArrayIndicator.GetSize();
		for(long lPos=0;lPos<lSize;lPos++)
		{
			rxtreeCtrl.InsertItem(strArrayIndicator.GetAt(lPos),hRootItem);
		}		
	}

//	//====================================================================
//	// ���� ��ǥ �׽�Ʈ �ϱ� ���� ���Ƿ� �ִ´�.
//	// ���߿� �����.
//#ifdef _DEBUG	
//	rxtreeCtrl.InsertItem("�ݸ�",hRootItem);
//	rxtreeCtrl.InsertItem("��������",hRootItem);
//#endif
//	//====================================================================
}



///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	Sang Yun, Nam
//	E-Mail	:	bbodongyi@hotmail.com
//	����	:	2005.04.25
//	Input	:	p_strGroup  ��ǥ �׷��
//				������� �� ComboBOx
//	����	:	��Ʈ�� ���� ��Ʈ, ��ǥ ����� �����´�.
///////////////////////////////////////////////////////////////////////////////
void CChartMng::GetIndicatorNameListIntoComboBox(CComboBox& rComboBox, LPCTSTR p_strGroup)
{
	if(p_strGroup[0]!=NULL)
	{
		CString strALine = GetIndicatorNameList(p_strGroup);
		CString strAItem;
		int nPos = 0;
		int nItem = 0;
		int nLength = 0;
		rComboBox.Clear();
			//rxlistCtrl.DeleteAllItems();
		if(!strcmp(p_strGroup,"Ư����ǥ"))
		{
//			OutputDebugString("���� �ִ´�.");



			rComboBox.AddString("ĵ����Ʈ");
			rComboBox.AddString("����Ʈ");
			rComboBox.AddString("����Ʈ");
			//rComboBox.AddString("����Ʈ");
			//rComboBox.AddString("������Ʈ");
			//rComboBox.AddString("�Ｑ��ȯ��");
			//rComboBox.AddString("��ť������Ʈ");
			//rComboBox.AddString("ĵ�麼����Ʈ");
			//rComboBox.AddString("P&F��Ʈ");
			
			/*
			rxlistCtrl.InsertItem(0, "");
			rxlistCtrl.SetItemText(0, 0, "ĵ����Ʈ",CLR_FONT_DEFAULT,CRGRID);
			rxlistCtrl.InsertItem(1, "");
			rxlistCtrl.SetItemText(1, 0, "����Ʈ");
			rxlistCtrl.InsertItem(2, "");
			rxlistCtrl.SetItemText(2, 0, "����Ʈ",CLR_FONT_DEFAULT,CRGRID);
			*/
			nItem = 3;
		}

		if(strALine.GetLength())
		{
			while(1)
			{
				nLength = strALine.GetLength();
				if(!nLength) break;
				nPos = strALine.Find(',');
				if(nPos==-1) 
				{
					strAItem = strALine;
					strALine.Empty();
				}
				else
				{
					strAItem = strALine.Left(nPos);
					strALine.Delete(0,nPos+1);
				}

				if(strAItem=="P&&F")
				{
					strAItem = "P&F";
				}
				rComboBox.AddString(strAItem);

				/*
				if(strAItem=="P&F")
				{
					strAItem = "P&&F";
				}
				rComboBox.AddString(strAItem);
				
				rxlistCtrl.InsertItem(nItem, "");
				if(nItem%2) // odd
				{					
					rxlistCtrl.SetItemText(nItem, 0, strAItem);
				}
				else		// even
				{
					rxlistCtrl.SetItemText(nItem, 0, strAItem,CLR_FONT_DEFAULT,CRGRID);					
				}*/				
				nItem++;
			}
		}
	}
	else
	{
		CString strList = GetIndicatorNameList("");
		CString strALine;
		CString strALineTitle;
		CString strAItem;
		int nPos = 0;
		int nItem = 0;
		int nLength = 0;

		rComboBox.Clear();
		rComboBox.AddString("�ŷ�����Ʈ");
//		rxlistCtrl.DeleteAllItems();
//		rxlistCtrl.InsertItem(nItem, "");
//		rxlistCtrl.SetItemText(nItem, 0, "�ŷ�����Ʈ",CLR_FONT_DEFAULT,CRGRID);
		nItem++;

		if(strList.GetLength())
		{
			while(1)
			{
				nPos = strList.Find(':');
				if(nPos==-1) break;		
				if(nPos==0) 
				{
					nPos = strList.Find('\n');
					strList.Delete(0,nPos+1);
					continue;
				}			
				strALineTitle = strList.Left(nPos);
				if(strALineTitle=="�⺻��ǥ"
					||strALineTitle=="������ǥ"
					||strALineTitle=="�����ڵ���"
					||strALineTitle=="Ư����ǥ")
				{
					nPos = strList.Find('\n');
					strList.Delete(0,nPos+1);
					continue;
				}

				strList.Delete(0,nPos+1);
				nPos = strList.Find('\n');
				strALine = strList.Left(nPos-1);		
				strList.Delete(0,nPos+1);
				
				while(1)
				{
					nLength = strALine.GetLength();
					if(!nLength) break;
					nPos = strALine.Find(',');
					if(nPos==-1) 
					{
						strAItem = strALine;
						strALine.Empty();
					}
					else
					{
						strAItem = strALine.Left(nPos);
						strALine.Delete(0,nPos+1);
					}
					rComboBox.AddString("�ŷ�����Ʈ");
//					rxlistCtrl.InsertItem(nItem, "");
//					if(nItem%2) // odd
//					{
//						rxlistCtrl.SetItemText(nItem, 0, strAItem);
//					}
//					else		// even
//					{
//						rxlistCtrl.SetItemText(nItem, 0, strAItem,CLR_FONT_DEFAULT,CRGRID);					
//					}				
					nItem++;
				}
			}		
		}
	}

	rComboBox.SetCurSel(0);
}

void CChartMng::SetParentMultiChartClassPointer(long clsPointer)
{
	m_lMltchrtDlgParentClsPointer = clsPointer;
}


// gm0604 added 2005.7.21 Start To Receive System Trading Message
BOOL CChartMng::OnSTDataManagerMessaggeAll()
{
	EACH_CHARTJOB_START()
		pChartItem->ReceiveFromSTDataManager();
	EACH_CHARTJOB_END()
	return TRUE;
}
// gm0604 added 2005.7.21 End To Receive System Trading Message


void CChartMng::GetChartAllCode(CStringArray &arrCode)
{
	CString strCode, csCode, csName;
	BOOL bCheck1, bCheck2;


	arrCode.RemoveAll();
	
	EACH_CHARTJOB_START()
		if(pChartItem)
		{
			strCode = _T("");
			strCode = pChartItem->GetDataCode();
			strCode.TrimLeft();		strCode.TrimRight();
			
			// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
			if(m_nMarketType == CMasterTypeInfo::FUTOPT_CHART)//@�������� || m_nMarketType == COMMODITY_CHART)
			{
				bCheck1 = bCheck2 = TRUE;

//���߿� �߰�				
//				// ����
//				m_pDBMgr->GetCodeName(CODE_FUTURE, strCode, csName);
//				if(!csName.GetLength())		bCheck1 = FALSE;					
//
//				//�ɼ�
//				m_pDBMgr->GetCodeName(CODE_OPTION, strCode, csName);
//				if(!csName.GetLength())		bCheck2 = FALSE;

				if(!bCheck1 && !bCheck2)
				{
					strCode = _T("");
					pChartItem->m_strReplayTime = _T("���Ⱑ ���� �ʴ� �����Դϴ�");					
					pChartItem->SetReplayChart_RealData("���Ⱑ ���� �ʴ� �����Դϴ�");
				}
			}

			if( strCode.GetLength())
				arrCode.Add(strCode);
		}
	EACH_CHARTJOB_END()
}

//===================================================================================
// ȯ�� ����
//===================================================================================
void CChartMng::SetTimeShareOnlyOne(BOOL bUseage)
{
	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();	
	if(pChartItem)
		pChartItem->m_CfgInfo.bRecvTimeShare = bUseage ? true : false;

	GetpChartItemSelected()->EnableReceivingTimeShare(bUseage);
}

void CChartMng::SetEnableBoundMarkOnlyOne(BOOL bUseage)
{	
	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();	
	if(pChartItem)
		pChartItem->m_CfgInfo.bShowHLBar = bUseage  ? true : false ;

	GetpChartItemSelected()->EnableBoundMark(bUseage);	
}

void CChartMng::SetShowExRightOnlyOne(BOOL bUseage)
{
	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();
	if(pChartItem->GetType() == DAY_DATA_CHART)
	{
		pChartItem->ShowLock(bUseage);
	}
	else
	{
		pChartItem->ShowLock(FALSE);
	}	
}

void CChartMng::SetPriceLog(BOOL bLog)	//�α� ǥ��
{
	//SendMessage Input Bar
	if(m_pInputBarDlg)
	{
		// �� �Լ����� ��� ���ٸ� ������ �� CChartMng�� UMSG_USE_LOG �� �ٽ� ������.
		m_pInputBarDlg->SendMessage(UMSG_USE_LOG, (long)bLog);
	}
}

void CChartMng::OnChartEnvSetupOK()
{
}
//===================================================================================

void CChartMng::SetAllChartCfgSync()
{
	CHART_CONFIG_INFO *pInfo = NULL;
	pInfo = GetpChartItemSelected()->GetChartConfigInfo();

	EACH_CHARTJOB_START()
		pChartItem->SetChartConfigInfo(*pInfo);
	EACH_CHARTJOB_END()
}

bool CChartMng::GetImaginaryCandleToOutside(bool& bImaginaryCandle)// ���������.
{
	CHART_CONFIG_INFO *pInfo = NULL;
	pInfo = GetpChartItemSelected()->GetChartConfigInfo();
	if(pInfo == NULL) return false;
	bImaginaryCandle = pInfo->bRealCandle;
	return true;
}
bool CChartMng::GetShowHighLowBoundToOutside(bool& bShow)			// ��/���Ѱ� ǥ��
{
	CHART_CONFIG_INFO *pInfo = NULL;
	pInfo = GetpChartItemSelected()->GetChartConfigInfo();
	if(pInfo == NULL) return false;
	bShow = pInfo->bShowHLBar;
	return true;
}

bool CChartMng::GetShowExRightDividendToOutside(bool& bShow)		// �Ǹ���/���� ǥ��
{
	CHART_CONFIG_INFO *pInfo = NULL;
	pInfo = GetpChartItemSelected()->GetChartConfigInfo();
	if(pInfo == NULL) return false;
	bShow = pInfo->bShowRight;
	return true;
}

bool CChartMng::GetTimeLinkageToOutside(bool& bTimeLinkage)		// �ð����� ����
{
	CHART_CONFIG_INFO *pInfo = NULL;
	pInfo = GetpChartItemSelected()->GetChartConfigInfo();
	if(pInfo == NULL) return false;
	bTimeLinkage = pInfo->bRecvTimeShare;
	return true;
}

bool CChartMng::GetAutoSaveToOutside(bool& bAutoSave)				// ��Ʈ����� �ڵ� ���� ����
{
	CHART_CONFIG_INFO *pInfo = NULL;	
	pInfo = GetpChartItemSelected()->GetChartConfigInfo();
	if(pInfo == NULL) return false;
	bAutoSave = pInfo->bAutoSave;
	return true;
}
/*
bool CChartMng::GetShowCurrentPriceToOutside(bool& bShow)
{
	CHART_CONFIG_INFO *pInfo = NULL;	
	pInfo = GetpChartItemSelected()->GetChartConfigInfo();
	if(pInfo == NULL) return false;
	bShow = pInfo->bShowCurPrice;
	return true;
}*/


//void CChartMng::SetShowCurrentPriceToOutside(const bool bShow)
//{
//	CChartItem* pChartItem = NULL;
//	pChartItem = GetpChartItemSelected();
//	pChartItem->SetShowCurrentPriceToOutside(bShow);
//}

void CChartMng::SetAutoSaveToOutside(const bool bAutoSave)
{
	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();
	pChartItem->SetAutoSaveToOutside(bAutoSave);
}

void CChartMng::CreateFloatingHogaWnd(BOOL bForce /* = FALSE */)
{
	if( ((CStdDialog*)GetParent())->m_nChartType >= MINI_MULTI_CHART) 
		return;
	if(m_nMarketType != CMasterTypeInfo::STOCK_CHART) return;
	
	//==============================================================================================
	//2005. 06. 23 ���� ���ҵ� ��Ʈ�� ���� 1 * 1 ������ ��� Floating Hoga â�� �����Ѵ�.
	//==============================================================================================
	DestroyFloatingHogaWnd();

	m_pFloatingDlg = NULL;
	m_pFloatingDlg = new CFloatingHogaDlg;	
	if(m_pFloatingDlg == NULL) return;

	m_pFloatingDlg->m_pMainDlg = this;
	m_pFloatingDlg->Create(IDD_DLG_FLOATING, this);	
	m_pFloatingDlg->SetParent(this);

	CRect rectChart;
	GetWindowRect(rectChart);
	ScreenToClient(rectChart);

	m_pFloatingDlg->SetWindowPos(&CWnd::wndTop, rectChart.left + 3,rectChart.top + m_nHeightOfCaption+2 + 16,0,0, SWP_NOSIZE |SWP_SHOWWINDOW);
}

void CChartMng::DestroyFloatingHogaWnd()
{	
#ifdef _WRITELOG
	((CStdDialog*)GetParent())->WriteToStringLog("CChartMng::DestroyFloatingHogaWnd::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif

	if(m_pFloatingDlg != NULL)
	{
		if(m_pFloatingDlg->GetSafeHwnd())
		{

			// Real ���� 
			if(m_pFloatingDlg->m_strOldCode.GetLength() == CODE_STOCK_LENGTH)
				((CStdDialog*)GetParent())->Request_Disconnect_StockRealHogaData(m_pFloatingDlg->m_strOldCode);

			if(m_pFloatingDlg->m_strCode.GetLength() == CODE_STOCK_LENGTH)
				((CStdDialog*)GetParent())->Request_Disconnect_StockRealHogaData(m_pFloatingDlg->m_strCode);			

			CRect floatRect;
			floatRect.SetRectEmpty();
			m_pFloatingDlg->GetWindowRect(floatRect);
			ScreenToClient(floatRect);
			
			m_pFloatingDlg->DestroyWindow();
			
			delete m_pFloatingDlg;
			m_pFloatingDlg = NULL;
		}
	}

#ifdef _WRITELOG
	((CStdDialog*)GetParent())->WriteToStringLog("CChartMng::DestroyFloatingHogaWnd::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
}

void CChartMng::MoveFloatingHogaDlg()
{
	if(m_pFloatingDlg == NULL) 
	{
		return;
	}
	if(m_pFloatingDlg->GetSafeHwnd())
	{
		CRect rectChart, rectFloat;
		/*CChartItem* pSelChartItem = GetpChartItemSelected();
		if(pSelChartItem == NULL) 
		{
			m_pFloatingDlg->Invalidate();
			return;
		}
		pSelChartItem->GetWindowRect(rectChart);
		*/
		GetWindowRect(rectChart);

		ScreenToClient(rectChart);

		if(m_pRectFloatingHoga == NULL)
		{
			m_pFloatingDlg->Invalidate();
			return;
		}
		if(m_pRectFloatingHoga->left > m_pRectFloatingHoga->right)
		{
			m_pFloatingDlg->Invalidate();
			return;
		}
		if(m_pRectFloatingHoga->top  > m_pRectFloatingHoga->bottom) 
		{
			m_pFloatingDlg->Invalidate();
			return;
		}

		m_pFloatingDlg->GetWindowRect(m_pRectFloatingHoga);
		ScreenToClient(m_pRectFloatingHoga);

		//======================================================================
		int nWidth	= m_pRectFloatingHoga->Width();
		int nHeight = m_pRectFloatingHoga->Height();

//		///////////////////////////////////////// gm0604 changed 2005.10.10
//		BOOL bIsSystemTradingMode = FALSE;
//		if(m_pChartItems[0][0])
//		{
//			bIsSystemTradingMode = m_pChartItems[0][0]->GetPtrST();
//		}	
//		if(bIsSystemTradingMode)
//		{
//			m_pRectFloatingHoga->top = rectChart.top + HEIGHTOFCAPTION*2+2;
//		}
//		else
//		{
//		m_pRectFloatingHoga->top = rectChart.top + m_nHeightOfCaption+2 + 16;
//		}				
		/////////////////////////////////////////

//		m_pRectFloatingHoga->top = rectChart.top + m_nHeightOfCaption+2 + 16;
//		m_pRectFloatingHoga->left = rectChart.left + 3;
		m_pRectFloatingHoga->right = m_pRectFloatingHoga->left + nWidth;
		m_pRectFloatingHoga->bottom = m_pRectFloatingHoga->top + nHeight;
		//======================================================================	
		
	
		m_pFloatingDlg->MoveWindow(m_pRectFloatingHoga);
		m_pFloatingDlg->SetWindowPos(&CWnd::wndTop, m_pRectFloatingHoga->left,m_pRectFloatingHoga->top, nWidth,nHeight, SWP_NOMOVE | SWP_NOSIZE |SWP_SHOWWINDOW);
		//m_pFloatingDlg->SendMessage(WM_SIZE, 0, MAKELPARAM(nWidth, nHeight));

		//::InvalidateRect(m_pFloatingDlg->GetSafeHwnd(), (CONST RECT*)m_pRectFloatingHoga, TRUE);
		//m_pFloatingDlg->Invalidate();
		//m_pFloatingDlg->UpdateWindow();

//		OutputDebugString("Move ChartInval");
	}
}

CString CChartMng::GetCurSelChartItemJMName()
{
	if(!GetpChartItemSelected()) return _T("");
	return GetpChartItemSelected()->GetKorName();
}


void CChartMng::GetStartEndDateAtChartMng(double& dStartDate, double& dEndDate)
{
	if(!GetpChartItemSelected())
	{
		dStartDate = dEndDate = 0;
		return;
	}

	GetpChartItemSelected()->GetStartEndDateOnView(&dStartDate,&dEndDate);

}

void CChartMng::ApplySyncEnv_OnlyOne()
{	
	//ȯ�浿��ȭ ���� �������� Ȯ��
	if( m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd() )
		if( m_pExtInputBarDlg->SendMessage(UMSG_GETENVSINK) == 0 ) return;	

	// 1 * 1 �̸� Return;
	if( m_lCols * m_lRows <= 1) return;

	CChartItem* pEmptyChart = NULL	; //ȯ�浿��ȭ�� ����� ��Ʈ
	pEmptyChart = GetpChartItemSelected();
	if(pEmptyChart == NULL) return;

	//����ִ� ������ ���� üũ
	if(pEmptyChart->m_bReceivedDataChart) return;
	



	m_bEmptyChartForSinkCfg = TRUE;
	
	// ������ �Ǵ� ��Ʈ�� ã�´�.
	// ������Ʈ�� ����� ��ȸ �ð�����(ƽ/��/��/��/��/��)�� 
	// �����ϸ� ������ OK.
	// �ٸ��� ȯ�浿��ȭ ����.
	CChartItem* pBaseChart	= NULL	;	
	EACH_CHARTJOB_START()
		char chBaseType		= pChartItem->GetType();  //���� ��Ʈ�� ��ȸ �ð����� Get
		char chEmptyType	= pEmptyChart->GetType(); //�� ��Ʈ�� ��ȸ �ð����� Get
		if(lstrlen(&chEmptyType) == 0)				  //������ ��ȸ Ÿ���� ������ 	
			chEmptyType	= pEmptyChart->GetTypeRequested();//��û�ϴ� Ÿ���� Get

		if(lstrlen(&chEmptyType) == 0) return; //�׷��� �������°� ������ Return

		if( chBaseType == chEmptyType ) //���� ��Ʈ�� ã�� ��� 
		{			
			if(pChartItem->m_bReceivedDataChart != TRUE)  //��ȸ Ÿ���� ������ �����Ͱ� ���� ��� �����Ѵ�.
				continue;

			pBaseChart = pChartItem;	//���� ��Ʈ Pointer�� ���� 
			goto Finded;			  	//���� For���̶�..goto���� �Ἥ ���� ������.
		}
	EACH_CHARTJOB_END()

Finded:

	if(pBaseChart)
	{
		//TR��ȸ�� �ʿ��� ��� - ������ǥ �϶�.
		//m_strArrayCodeInLoop.RemoveAll();    //TR��ȸ�� �ʿ��� �����ڵ� ���� 
		//m_strArrayTRCodeInLoop.RemoveAll();  //TR��ȸ�μ� ����ȭ�� �ʿ��� TR���� ���� 

		//ȣ�� ��û�� ���ϵ��� �Ѵ�.
		if(m_pInputBarDlg)
			m_pInputBarDlg->SendMessage(UMSG_ENABLE_SENDHOGATR, FALSE, 0);

		//������ �Ǵ� ��Ʈ�� ������ �����Ѵ�.
		pBaseChart->SaveandOpenFromFile("","temp.tmp",FALSE,TRUE);

		int	 nlCntForSyncEnv						= pBaseChart->GetCntOfItemData();				//
		char chTypeForSyncEnv						= pBaseChart->GetType();						//ƽ����		
		BOOL bNormalizedMinuteTimeRuleForSyncEnv	= pBaseChart->GetBNormalizedMinuteTimeRule();	//�����
		int  nChartMngs_ChartDataNumericValue		= pBaseChart->GetChart_NumericValueType();		//\, $, %

		//chart TR Data�� ������ �׸���� �ִ� ���..������ �����Ѵ�.
		// lPos = 1 ���� �����ϴ� ������ 
		// 0��°���� ��Ʈ ������ �̰�
		// 1��° ���� ����ȭ �� TR�� ��ȸ�� �ʿ��� ������ǥ�� �ش��ϴ� �������� 
		// ����ֱ� �����̴�.
		CStringArray strArrayCode, strArrayTRCode;
		for(long lPos = 1;lPos<nlCntForSyncEnv;lPos++)
		{
			strArrayCode.Add(pBaseChart->GetDataCode(lPos));
			strArrayTRCode.Add(pBaseChart->GetDataTRCode(lPos));
		}

		pEmptyChart->ClearDataExceptPrimaryData();
		
		
		if(pEmptyChart->GetChart_NumericValueType()!=nChartMngs_ChartDataNumericValue)	
		{
			pEmptyChart->SetChartNumericValue(nChartMngs_ChartDataNumericValue);
		}		

		if(pEmptyChart->GetBNormalizedMinuteTimeRule()!=bNormalizedMinuteTimeRuleForSyncEnv)
		{
			pEmptyChart->SetBNormalizedMinuteTimeRule(bNormalizedMinuteTimeRuleForSyncEnv);
		}

		CString  strTrCode;
		for(int lPos = 0;lPos<(nlCntForSyncEnv-1);lPos++)
		{
			//lTrCode = atoi(strArrayTRCode.GetAt(lPos));
			strTrCode = strArrayTRCode.GetAt(lPos);
			pEmptyChart->SetCodeForQuery(strArrayCode.GetAt(lPos),strTrCode,FALSE);
		}	
	}	
}

void CChartMng::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		KillTimer(1);
		PostMessage(UMSG_SIGNAL_CLICK);
	}
	CWnd::OnTimer(nIDEvent);
}

void CChartMng::SetSystemTradingMode(BOOL bEnable, LPCTSTR lpMessage/*=NULL*/)
{
	m_bIsSystemTradingMode = bEnable;
	if(!bEnable)
	{
		ASSERT(lpMessage);
		long lPtrFree = atol(lpMessage);
		BOOL bIsFree = FALSE;
		EACH_CHARTJOB_START()
			if(pChartItem->GetPtrST()==lPtrFree)
			{
				bIsFree = TRUE;
				break;
			}
		EACH_CHARTJOB_END()
		if(bIsFree)
		{
			EACH_CHARTJOB_START()
				pChartItem->ResetOCX();
			EACH_CHARTJOB_END()
			MoveCharts();
		}

	}
}


void CChartMng::SetpChartCfgWnd(char chType, CWnd* pWnd)
{
	switch(chType)
	{
//		case	SEC10_DATA_CHART:	
//		case	SEC30_DATA_CHART:	m_pSecCfgChart	= pWnd; break;
		case	SEC_DATA_CHART	:	m_pSecCfgChart	= pWnd; break;
		case	TICK_DATA_CHART	:	m_pTickCfgChart = pWnd; break;			
		case	MIN_DATA_CHART	:	m_pMinCfgChart	= pWnd; break;	
		case	DAY_DATA_CHART	:	m_pDayCfgChart	= pWnd; break;					
		case	HOUR_DATA_CHART	:	m_pHourCfgChart = pWnd; break;
		case	WEEK_DATA_CHART	:	m_pWeekCfgChart = pWnd; break;
		case	MONTH_DATA_CHART:	m_pMonthCfgChart= pWnd; break;
		case	YEAR_DATA_CHART	:	m_pYearCfgChart = pWnd; break;
	}
}

CChartItem* CChartMng::GetpChartCfgWnd(char chType)
{
	switch(chType)
	{
//		case	SEC10_DATA_CHART:	
//		case	SEC30_DATA_CHART:	return (CChartItem*)m_pSecCfgChart	; break;
		case	SEC_DATA_CHART	:	return (CChartItem*)m_pSecCfgChart	; break;
		case	TICK_DATA_CHART	:	return (CChartItem*)m_pTickCfgChart ; break;			
		case	MIN_DATA_CHART	:	return (CChartItem*)m_pMinCfgChart	; break;	
		case	DAY_DATA_CHART	:	return (CChartItem*)m_pDayCfgChart	; break;					
		case	HOUR_DATA_CHART	:	return (CChartItem*)m_pHourCfgChart	; break;
		case	WEEK_DATA_CHART	:	return (CChartItem*)m_pWeekCfgChart ; break;
		case	MONTH_DATA_CHART:	return (CChartItem*)m_pMonthCfgChart; break;
		case	YEAR_DATA_CHART	:	return (CChartItem*)m_pYearCfgChart ; break;
		default					:	return NULL;
	}

	return NULL;
}


// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
//20110502 �̹��� >>
//void CChartMng::Synchro_jipyo()
void CChartMng::Synchro_jipyo(BOOL bChangePeriod/*=TRUE*/, LPCTSTR lpszCodeForRequest/*=NULL*/)
//20110502 �̹��� <<
{
	if(m_bSaveJipyoOneFile)
		return;

	// �߰� ����϶� : �������� - ojtaso (20070209)
//	if(!m_bChangeTypeMode || m_bAddMode )
//		return;

	m_bExistSyncChart = FALSE;

	//ȯ�浿��ȭ ���� �������� Ȯ��
	//ȯ�� ����ȭ ���¸� ���� ���Ѵ�.
	if( m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd() )
		if( m_pExtInputBarDlg->SendMessage(UMSG_GETENVSINK) == 1 ) return;	

	CChartItem* pSyncChart = NULL	; //��ǥ����ȭ�� ����� ��Ʈ
	pSyncChart = GetpChartItemSelected();
	if(pSyncChart == NULL) return;

	
	// ������ �Ǵ� ��Ʈ�� ã�´�.
	// ������Ʈ�� ����� ��ȸ �ð�����(ƽ/��/��/��/��/��)�� 
	// �����ϸ� ������ OK.
	// �ٸ��� ȯ�浿��ȭ ����.
	
	char chOriType		= '\0';
	char chRqstType		= '\0';

	chOriType = pSyncChart->GetType(); //��Ʈ�� ���� ��ȸ�� �ð����� Get	
	chRqstType	= pSyncChart->GetTypeRequested();//��û�ϴ� Ÿ���� Get

	if(lstrlen(&chRqstType) == 0) return; //�׷��� �������°� ������ Return

	// �м��� ���� ���� - ojtaso (20080728)
	BOOL bSaveAnalTool = GetSaveAnalTool();
	if(!bSaveAnalTool && chOriType == chRqstType) return; //���� Ÿ���̸� �����ϰų� �ε����� �ʴ´�.
	
//	if(chOriType == NULL)  chOriType = chRqstType;

	CChartItem* pSaveChart	= NULL	;
	CChartItem* pLoadChart	= NULL	;


// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
	if (chOriType == chRqstType)  bChangePeriod = FALSE;//1��,2�е��� �����ֱ⸸ ����� ���� �ֱⰡ ������� ���� ������ �����Ѵ�.
	BOOL bSaveJipyoPerCodePeriod = ::GetCommonDataInfo(GetSafeHwnd(), COMDATA_SAVE_JIPYO_PER_CODE_PERIOD, 0);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//������ ��Ʈ�� �׷��� ���¿��� ���񺯰��̳� 1��, 2�� ���� �����ֱⰡ ����� ���
	//����ִ� ��Ʈocx�� ó�� ��������� ���� �Ʒ��� ����� ������ ������ ��� �� ������ �ε���� �Ѵ�.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	int nRQCount = pSyncChart->GetCountRQ();
	if (!bChangePeriod && nRQCount > 0)
		pLoadChart = GetpChartCfgWnd(chRqstType);
	else if (nRQCount > 0)//�� ��Ʈ��Ʈ�ѿ� ��ȸ�ϴ� ��찡 �ƴϹǷ� �Ѿ�� �����ڵ�� ������� �ʾƵ� �ȴ�.
		lpszCodeForRequest = NULL;
//	pLoadChart = GetpChartCfgWnd(chRqstType	);

	if(pSyncChart)
	{
		//TR��ȸ�� �ʿ��� ��� - ������ǥ �϶�.
		//m_strArrayCodeInLoop.RemoveAll();    //TR��ȸ�� �ʿ��� �����ڵ� ���� 
		//m_strArrayTRCodeInLoop.RemoveAll();  //TR��ȸ�μ� ����ȭ�� �ʿ��� TR���� ���� 
		if(pLoadChart->GetSafeHwnd())
		{
			m_bExistSyncChart = TRUE;
		}


		//ȣ�� ��û�� ���ϵ��� �Ѵ�.
		if(m_pInputBarDlg)
			m_pInputBarDlg->SendMessage(UMSG_ENABLE_SENDHOGATR, FALSE, 0);	

		//��ȸ ���� ��Ʈ�� ������ �����Ѵ�.
		if(pSyncChart->HasGraph("������Ʈ"))
		{			
			CString sSaveFileName = pSyncChart->GetOpenAndSaveFileName();
			pSyncChart->SaveandOpenFromFile("", (LPCTSTR)sSaveFileName, FALSE, TRUE);

			if(bSaveAnalTool)
			{
				sSaveFileName = pSyncChart->GetOpenAndSaveFileName(TRUE);
				pSyncChart->SaveandOpenFromAnalToolFile("", (LPCTSTR)sSaveFileName, FALSE, TRUE);
			}

// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
			if (bChangePeriod && bSaveJipyoPerCodePeriod && nRQCount > 0)
			{
				CString strSaveFile		= pSyncChart->GetOpenAndSaveFileName4PeriodRQ(pSyncChart->GetType());
				CString strSaveFilePath = GetUserpath() + "\\" + strSaveFile;

				pSyncChart->SaveChartInfo4PeriodRQ(strSaveFilePath);
			}
		}

		int	 nlCntForSyncEnv	= pSyncChart->GetCntOfItemData();

		//chart TR Data�� ������ �׸���� �ִ� ���..������ �����Ѵ�.
		// lPos = 1 ���� �����ϴ� ������ 
		// 0��°���� ��Ʈ ������ �̰�
		// 1��° ���� ����ȭ �� TR�� ��ȸ�� �ʿ��� ������ǥ�� �ش��ϴ� �������� 
		// ����ֱ� �����̴�.
		CStringArray strArrayCode, strArrayTRCode;
		for(long lPos = 1;lPos<nlCntForSyncEnv;lPos++)
		{
			strArrayCode.Add(pSyncChart->GetDataCode(lPos));
			strArrayTRCode.Add(pSyncChart->GetDataTRCode(lPos));
		}

		pSyncChart->ClearDataExceptPrimaryData();
		long lTrCode = 0;
		

		for(int lPos = 0;lPos<(nlCntForSyncEnv-1);lPos++)
		{
			lTrCode = atoi(strArrayTRCode.GetAt(lPos));
			pSyncChart->SetCodeForQuery(strArrayCode.GetAt(lPos),strArrayTRCode.GetAt(lPos),FALSE);
		}	
	}
	
	if(pLoadChart == NULL)
	{
		//=======================================================================================
		CString sJipyoFileName, sJipyoFileName1, sJipyoSaveFileName;
		//100530:����ȭ�� �ٸ��� ����/�б�
		//sJipyoFileName1 = ((CStdDialog*)GetParent())->GetScreenNo();
		((CStdDialog*)GetParent())->GetFileSaveKey(sJipyoFileName1);
		if(m_bSaveJipyoOneFile)
		{
			sJipyoSaveFileName = sJipyoFileName1 + "_"	;
		}
		else
		{
			switch(chRqstType)
			{	
				case	SEC_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Sec_"	; break;
				case	TICK_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Tick_"	; break;			
				case	MIN_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Min_"	; break;	
				case	DAY_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Day_"	; break;					
				case	HOUR_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Hour_"	; break;
				case	WEEK_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Week_"	; break;
				case	MONTH_DATA_CHART:	sJipyoSaveFileName = sJipyoFileName1 + "_Month_"	; break;
				case	YEAR_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Year_"	; break;
				default					:	return ;
			}
		}

		sJipyoSaveFileName += JIPYOSYNC_FILE;

		CFileFind ff;
		CString sTempPath;
		sTempPath = m_strUserPath + "\\" + sJipyoSaveFileName;
		BOOL bResult = ff.FindFile(sTempPath);

// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
		//20110502 �̹��� >>
		BOOL bResultRQ = FALSE;

		if (bSaveJipyoPerCodePeriod)
		{
			CString strSaveFile		= pSyncChart->GetOpenAndSaveFileName4PeriodRQ(chRqstType);
			CString strSaveFilePath = GetUserpath() + "\\" + strSaveFile;			
			bResultRQ = ff.FindFile(strSaveFilePath);
		}

		if (!bResult || !bResultRQ)
		{
			if (bSaveJipyoPerCodePeriod)
				pSyncChart->ResetChart();
		}
		else
		{
			m_bExistSyncChart = TRUE; //������ �ִ� ���� �� ���� ���� �� �ֵ��� flag���Ѵ�.

			if(bSaveJipyoPerCodePeriod)
			{
				CString strSaveFile		= pSyncChart->GetOpenAndSaveFileName4PeriodRQ(chRqstType);
				CString strSaveFilePath = GetUserpath() + "\\" + strSaveFile;

				pSyncChart->LoadChartInfo(strSaveFilePath, TRUE, TRUE, TRUE, lpszCodeForRequest);	

				pSyncChart->SetLoadChartInfo(TRUE);
			}
		}
		//=======================================================================================
	}
	
}


void CChartMng::SetBReplayChartMode(BOOL bReplayMode)
{
	m_bReplaychartMode = bReplayMode;	

	EACH_CHARTJOB_START()
		pChartItem->SetBReplayChartMode(bReplayMode);
	EACH_CHARTJOB_END()
}


void CChartMng::SetReplayRealTitle(CString strCode, CString strSpeed, TICK_DATA* pData)
{

	int nLen = 0;
	double dPoint = 1.0;
	CString strFormat = _T("%.lf");
	if(m_nMarketType == CMasterTypeInfo::STOCK_CHART) 
		nLen = CODE_STOCK_LENGTH;
	else if(m_nMarketType == CMasterTypeInfo::FUTOPT_CHART)
	{
		nLen = FUTOPT_LENGTH;
		dPoint = 0.01;
		strFormat = _T("%.2lf");
	}
	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//@��������	else if(m_nMarketType == COMMODITY_CHART) nLen = COMMODITY_LENGTH;
	else return;

	CString strCnts, strTime, strCurPrice, strVolume, strPrePrice;
	CString strChartCode;
	CRect rectChart;
	CString strRQ(_T("DEFAULT"));

	EACH_CHARTJOB_START()
		strChartCode = pChartItem->GetDataCode();
		if(strChartCode.GetLength()==nLen&&strChartCode==strCode)
		{
			IRealReceiver *pIRealReceiver = ( IRealReceiver *)pChartItem->GetAddInCustomInterface( "ChartCommonAddIn.dll:DIRECT_REAL");
			
			strTime.Format("%6.6s", pData->chDate);
			strCurPrice.Format("%10.10s", pData->chClosingPrice);
			strVolume.Format("%10.10s", pData->chVolume);
			
			strRQ = pChartItem->GetSelectedRQ();

			// ���� ���ؼ� �ǽð� ���� ǥ�� - ojtaso (20090210)
			BOOL bAppend = FALSE;
// #ifdef __MULTI_LANG__ //@Solomon-100225
			pIRealReceiver->SetRealDataRQ(strRQ, "_DateTime_",		atof(strTime), bAppend);
			pIRealReceiver->SetRealDataRQ(strRQ, "_Close_",			atof(strCurPrice), bAppend);
			pIRealReceiver->SetRealDataRQ(strRQ, "_Volume_",	atof(strVolume), bAppend);
// #else
// 			pIRealReceiver->SetRealDataRQ(strRQ, "�ڷ�����",		atof(strTime), bAppend);
// 			pIRealReceiver->SetRealDataRQ(strRQ, "����",			atof(strCurPrice), bAppend);
// 			pIRealReceiver->SetRealDataRQ(strRQ, "�⺻�ŷ���",	atof(strVolume), bAppend);
// #endif

			strTime.Format("%.2s:%.2s:%.2s", pData->chDate, pData->chDate + 2, pData->chDate + 4);
			strCnts.Format("%s %s %s ", 
							pChartItem->GetKorName(),	//�����							 
							pChartItem->GetReplay_CurDate(), //���೯¥
							strTime //����ð�							
							);

			CString strRePlayPrice;
			strRePlayPrice.Format(strFormat, atof( strCurPrice ) * dPoint );
		
			pChartItem->SetReplayChart_RealData(strCnts, strRePlayPrice, strVolume);

			BOOL ret2 = pIRealReceiver->SetRealPacketEndRQ(strRQ);
			pIRealReceiver->Release();


/*
			//=============================================================================
			// ��Ʈ�� Real ������.
			//=============================================================================		
			DrdsData r_DrdsData;
			memset(&r_DrdsData, 0x20, sizeof(DrdsData));	
			
			if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
			{
				r_DrdsData.nID = 100;
				memset(r_DrdsData.DataName, 0x00,  32);
				memcpy(r_DrdsData.DataName, "58", 2);

				memset(r_DrdsData.KeyName, 0x00, 32);
				memcpy(r_DrdsData.KeyName, (LPTSTR)(LPCTSTR)strChartCode, strChartCode.GetLength());

				//=======================================================================================
				//�ڷ����� 
				CString sTime;
				sTime.Format("%6.6s", pData->chDate);
				r_DrdsData.Len = 11;
				memset(r_DrdsData.FieldName, 0x00,  32);
				memcpy(r_DrdsData.FieldName, "2", 1);
				memcpy(r_DrdsData.Data, (LPTSTR)(LPCTSTR)sTime, 11 );
				pChartItem->SendMessage(WM_DRDS_DATA, sizeof(r_DrdsData), (LPARAM)(const char*)&(r_DrdsData));				

				//=======================================================================================
				//���� 
				CString sPrice;
				sPrice.Format("%10.10s", pData->chClosingPrice);
				r_DrdsData.Len = 12;
				memset(r_DrdsData.FieldName, 0x00,  32);
				memcpy(r_DrdsData.FieldName,"6", 1);
				memcpy(r_DrdsData.Data, (LPTSTR)(LPCTSTR)sPrice, 12 );
				pChartItem->SendMessage(WM_DRDS_DATA, sizeof(r_DrdsData), (LPARAM)(const char*)&(r_DrdsData));				

				//=======================================================================================
				//�ŷ��� 
				CString sVolume;
				sVolume.Format("%10.10s", pData->chVolume);
				r_DrdsData.Len = 13;
				memset(r_DrdsData.FieldName, 0x00,  32);
				memcpy(r_DrdsData.FieldName,"10", 2);
				memcpy(r_DrdsData.Data, (LPTSTR)(LPCTSTR)sVolume, 13 );
				pChartItem->SendMessage(WM_DRDS_DATA, sizeof(r_DrdsData), (LPARAM)(const char*)&(r_DrdsData));

			}
			else if(m_nMarketType == FUTOPT_CHART)
			{
				r_DrdsData.nID = 100;
				memset(r_DrdsData.DataName, 0x00,  32);
				
				CString csName = _T("");

				m_pDBMgr->GetCodeName(CODE_FUTURE, strChartCode, csName);				

				if(csName.GetLength())
					memcpy(r_DrdsData.DataName, "65", 2); //����
				else				
					memcpy(r_DrdsData.DataName, "67", 2); //�ɼ�

				memset(r_DrdsData.KeyName, 0x00, 32);
				memcpy(r_DrdsData.KeyName, (LPTSTR)(LPCTSTR)strChartCode, strChartCode.GetLength());

				//=======================================================================================
				//�ڷ����� 
				CString sTime;
				sTime.Format("%6.6s", pData->chDate);
				r_DrdsData.Len = 11;
				memset(r_DrdsData.FieldName, 0x00,  32);
				memcpy(r_DrdsData.FieldName, "2", 1);
				memcpy(r_DrdsData.Data, (LPTSTR)(LPCTSTR)sTime, 11 );
				pChartItem->SendMessage(WM_DRDS_DATA, sizeof(r_DrdsData), (LPARAM)(const char*)&(r_DrdsData));				

				//=======================================================================================
				//���� 
				CString sPrice;
				sPrice.Format("%10.10s", pData->chClosingPrice);
				r_DrdsData.Len = 12;
				memset(r_DrdsData.FieldName, 0x00,  32);
				memcpy(r_DrdsData.FieldName,"3", 1);
				memcpy(r_DrdsData.Data, (LPTSTR)(LPCTSTR)sPrice, 12 );
				pChartItem->SendMessage(WM_DRDS_DATA, sizeof(r_DrdsData), (LPARAM)(const char*)&(r_DrdsData));				

				//=======================================================================================
				//�ŷ��� 
				CString sVolume;
				sVolume.Format("%10.10s", pData->chVolume);
				r_DrdsData.Len = 13;
				memset(r_DrdsData.FieldName, 0x00,  32);
				memcpy(r_DrdsData.FieldName,"10", 2);
				memcpy(r_DrdsData.Data, (LPTSTR)(LPCTSTR)sVolume, 13 );
				pChartItem->SendMessage(WM_DRDS_DATA, sizeof(r_DrdsData), (LPARAM)(const char*)&(r_DrdsData));
			}
		
			pChartItem->SendMessage(WM_DRDS_DATA_PACKET_END, 0, 0 );
		*/	
			//=============================================================================
		}
	EACH_CHARTJOB_END()
}

//================================================================================
// �������� �� ��¥�� �ٲ������ ��Ʈ�� �˷��ش�.
// 2005. 12. 08 �߰� 
//================================================================================
void CChartMng::SetReplayChangedDate(CString strCode, CString strBojoMsg)
{
	CString strChartCode;
	EACH_CHARTJOB_START()
		strChartCode = pChartItem->GetDataCode();
		if(strChartCode.GetLength()&&strChartCode==strCode)
		{
//			OutputDebugString("���� ��¥�� �ٲ����");
//			OutputDebugString("���� " + strBojoMsg);
			pChartItem->InitString_ForReplay(); //06.01.12 added by sy, nam

			pChartItem->SetReplay_PrePrice(strBojoMsg);

			pChartItem->SetReplay_CurDate(strBojoMsg);
			pChartItem->CDRChartOcx::SetPacketHelpMessage((LPCTSTR)strBojoMsg, false);
		}
	EACH_CHARTJOB_END()
}

//================================================================================
// ��Ʈ�� �ε�� �ʿ��� TR�������� ��� ��
//================================================================================
void CChartMng::SetFocusToJMEdit_At_LoadEnd()
{		
	
	if(!m_bLastEnvUpdated)
	{
		CChartItemData * pItemData = NULL;
		int nDataSize = 0;
		int nCntComplete = 0;
		int nCntComplete2 = 0;
		EACH_CHARTJOB_START()								
			nDataSize = 0; nCntComplete = 0;
			nDataSize = pChartItem->m_arrayMarketTRData.GetSize();							
			for(int i = 0; i < nDataSize; i++)
			{
				pItemData = NULL;
				pItemData = pChartItem->m_arrayMarketTRData.GetAt(i);
				if(pItemData == NULL) continue;

				if( pItemData->IsFilled()) 
					nCntComplete++;								
			}
		
			if(nCntComplete == nDataSize) //�Ϸᰡ �� �Ǿ��� ���....
			{
				nCntComplete2++;
				//m_pInputBarDlg->SendMessage(UMSG_EDIT_FOCUS);
				//m_bLastEnvUpdated = TRUE;
			}
		EACH_CHARTJOB_END()

		if(nCntComplete2 == m_lCols * m_lRows)
		{
			if(!m_bIsInLoop) //�ε尡 �� ������ ���� Focus�� ���� �Ѵ�.
			{
				m_bLastEnvUpdated = TRUE;
				if(m_pInputBarDlg)
					m_pInputBarDlg->SendMessage(UMSG_EDIT_FOCUS);		
			}
		}
	}
}


BOOL CChartMng::CheckDataTypeForReplay()
{
	EACH_CHARTJOB_START()
		if(pChartItem->GetChart_NumericValueType() != WON_TYPE)
			return FALSE;
	EACH_CHARTJOB_END()

	return TRUE;
}

void CChartMng::SetEndData(LPCSTR szMessage)
{
	GetParent()->SendMessage(UMSG_SENDTOMSGBOX,(WPARAM)szMessage,(LPARAM)GetpChartItemSelected());
//	InitInLoop();
}

//-----------------------------------------------------------------------------
// Author		: Young-Hee, Lee	Date :2006/10/2
// Reason		: ���ֿ��� ��Ʈ
//-----------------------------------------------------------------------------
BOOL CChartMng::LoadDWMMChart(BOOL bLoad)
{
	CString strCode;
	//chType: ƽ(0)/��(1)/��(2)/��(3)/��(4)
	CChartItem* pChartItem;
	strCode = GetpChartItemSelected()->GetDataCode();
	if( m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd() )
		m_pExtInputBarDlg->SendMessage(UMSG_CLEARLINK);	

	//2007.02.06 by LYH ������Ʈ�� ��� ���ֿ��� ���� ��� �ٽ� �ϳ���Ʈ�� �ǵ��� ����
	if(((CStdDialog*)GetParent())->m_nChartType==SINGLE_CHART)
	{
		if(m_lRows != 1 && m_lCols != 1)
			bLoad = FALSE;
	}
	//}}
	if(bLoad)
	{
		//{{2007.06.25 by LYH �����߰� ����� ��� ������� ����
		int nResult = GetParent()->SendMessage(UMSG_GET_CHARTMODE, 1);
		if(nResult != CHANGE_CHART &&  nResult != ADD_NEW_CHART)
			((CInputBarDlg*)m_pInputBarDlg)->SetMode(CHANGE_CHART);
		//}}

		SetMatrix(2,2);
		if( m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd() )
			m_pExtInputBarDlg->SendMessage(UMSG_SETJONGMOKLINK, TRUE);	
		m_bJongmokLink = TRUE;

		if(m_pChartItems[0][0]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[0][0];
			pChartItem->SetType(DAY_DATA_CHART);
		}

		if(m_pChartItems[0][1]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[0][1];
			pChartItem->SetType(WEEK_DATA_CHART);
		}

		if(m_pChartItems[1][0]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[1][0];
			pChartItem->SetType(MONTH_DATA_CHART);
		}

		if(m_pChartItems[1][1]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[1][1];
			pChartItem->SetType(MIN_DATA_CHART);
		}
	}
	else
	{
		SetMatrix(1,1);

		if(m_pChartItems[0][0]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[0][0];
			pChartItem->SetType(DAY_DATA_CHART);
		}
	}

	CString strEndDate;
	char chType = NULL;
	strEndDate.Format("%d",GetpChartItemSelected()->GetEndDate());

	SetBInitFromExtToolBar(FALSE); //�ʱ�ȭ�̴� �ƴϴ� �⺻���� False�� ���ش�.05.11.24

	InitInLoop();
	m_strEndDateInLoop = strEndDate;
	m_strCodeInLoop = strCode;
	m_lTypeInLoop = LOOPTYPE_NULL/*0*/;

	HeadInLoop();
	if(!GetpChartItemSelected()) return FALSE;
	GetpChartItemSelected()->Init_OverLapped();

	return TRUE;

}

//-----------------------------------------------------------------------------
// Author		: Young-Hee, Lee	Date :2006/12/6
// Reason		: ��Ʈ ��ü�ʱ�ȭ
//-----------------------------------------------------------------------------
void CChartMng::ResetChartAll(BOOL bTrRetry)
{
	((CStdDialog*)GetParent())->AllCodeUnAdvise();
	
	//{1*1 ��Ʈ�� �ʱ�ȭ ��û
	ReplaceChart(0,0,m_lRowSelected,m_lColSelected);
	SetMatrix(1,1);
	//}
	if(m_pInputBarDlg)
		((CInputBarDlg*)m_pInputBarDlg)->SetMode(CHANGE_ITEM);

	m_bHideScale = FALSE;
	m_bHideInnerToolBar = FALSE;

	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();
	if(pChartItem == NULL || !pChartItem->m_bReceivedDataChart) 
		return;
	pChartItem->ResetChart();

	// ���õ� ��Ʈ�� ���� ��û�� ������ default ���� ����
	if( ((CStdDialog*)GetParent())->m_nChartType >= MINI_MULTI_CHART) 
		pChartItem->SetCntForRequest(MINICNTATONCE);
	else
		pChartItem->SetCntForRequest(DEFAULTCNTATONCE);

	pChartItem->SetSaveAllCount(0);

	pChartItem->SetChartNumericValue(WON_TYPE);

	//2007.01.17 add by LYH  ���ٹ�ư ����ȭ(���� ���� ����)
	pChartItem->m_bDrawAutoTrendLine = 0;
	pChartItem->m_bDrawAutoPattern = 0;
	pChartItem->m_bDrawCandlePattern = 0;
	pChartItem->m_bDrawNews = 0;

	m_bHideScale = FALSE;
	ShowScale();
	m_bHideInnerToolBar = FALSE;
	ShowInnerToolbar();
	//}}

	//�ʱ�ȭ ��Ű���� ���� ����.
	if( m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd() )
		m_pExtInputBarDlg->SendMessage(UMSG_CHART_INIT_FROMOCX, 1);


	//////////////////////////////////////////////////////////
	//�ʱ�ȭ�� ��� ���� ���� by LYH 2007.01.06
	CString sAllFileName = m_strUserPath + "\\" + "*" +((CStdDialog*)GetParent())->GetScreenNo()+ "_*";
	CFileFind ff;
	CString sFileName;
	BOOL bFindNextFile = FALSE;
	if(ff.FindFile(sAllFileName,0))
	{
		while(1)
		{
			bFindNextFile = ff.FindNextFile();
			sFileName = ff.GetFilePath();
			::DeleteFile(sFileName);
			if(!bFindNextFile)	break;
		}
	}

	pChartItem->InvokeAddIn("ChartGuideLineAddIn.dll:GUIDE_LINE",200,(long)pChartItem->GetSafeHwnd());
	pChartItem->InvokeAddIn("ChartPatternAddin.dll",0,200);
	pChartItem->InvokeAddInSTIndex("ChartSTIndexAddIn.dll",200,0);	// �ý���Ʈ���̵� ��ü�ʱ�ȭ

	m_pTickCfgChart		= NULL;
	m_pSecCfgChart		= NULL;
	m_pMinCfgChart		= NULL;
	m_pDayCfgChart		= NULL;
	m_pHourCfgChart		= NULL;
	m_pWeekCfgChart		= NULL;
	m_pMonthCfgChart	= NULL;
	m_pYearCfgChart		= NULL;

	m_bExistSyncChart	= FALSE;

	//////////////////////////////////////////////////////////

	// ��Ʈ �ʱ�ȭ���� : �������� - ojtaso (20070621)
	m_bResetChartAll = TRUE;

	//�ʱ�ȭ �� �ٽ� TR������
	if(bTrRetry)	//@Solomon-A002 TR������ �ʿ����� ���� ���.
	{
		if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		{
			if(((CStdDialog*)GetParent())->m_nScreenID == nScreenID_6204) //20090910 SJ_KIM �̴�TICK��Ʈ �ʱ�ȭ�� ����ȸ �����ʵ��� ����
			{
				SendMessage(RMSG_SETQUERYOPT, 0);
				m_pInputBarDlg->SendMessage(UMSG_CHART_INIT_FROMOCX, 2);						
			}
			else
				m_pInputBarDlg->SendMessage(UMSG_CHART_INIT_FROMOCX, 1);
		}
	}

	// ��Ʈ �ʱ�ȭ���� : �������� - ojtaso (20070621)
	m_bResetChartAll = FALSE;

	SetBInitFromExtToolBar(FALSE);	
}

BOOL CChartMng::RevisedPriceFlag()// �����ְ�.
{
	CChartItem* pSelItem = GetpChartItemSelected();

	if(pSelItem)
	{
		return pSelItem->GetUseRevisedValue();
	}
	return FALSE;
}

void CChartMng::SetMarketType(int nMarketType)
{
	m_nMarketType = nMarketType;

	if(m_pFloatingDlg)
	{
		if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
			m_pFloatingDlg->ShowWindow(SW_SHOW);
		else
			m_pFloatingDlg->ShowWindow(SW_HIDE);
	}
} 

// ���ο� ��Ʈ �߰� : �������� - ojtaso (20070108) 
void CChartMng::SetAddMode(BOOL bAddMode)
{
	m_bAddMode = bAddMode;
}

// ����� ��� : �������� - ojtaso (20080213)
CString CChartMng::GetJongMokNameByCode(LPCSTR lpszItemCode, LPCTSTR lpszTRCode/* = NULL*/)
{
	return ((CStdDialog*)GetParent())->GetJongMokNameByCode(lpszItemCode, lpszTRCode);
}

// ��Ƽ��Ʈ�� ���ÿ��� : �������� - ojtaso (20070525)
void CChartMng::SetSelChangeChart(BOOL bSelChangeChart)
{ 
	m_bSelChangeChart = bSelChangeChart; 
}

// �����ڵ�� RQ�� ã�� ���� : �������� - ojtaso (20070705)
BOOL CChartMng::FindSelectChartByItemCode(LPCTSTR lpszItemCode, BOOL bSelect/* = TRUE*/)
{
	CChartItem* lpSelItem = GetpChartItemSelected();
	if(lpSelItem)
		return lpSelItem->FindSelectChartByItemCode(lpszItemCode, bSelect);
	else 
		return FALSE;
}

void CChartMng::ReplaceOldCode()
{
	CChartItem* lpSelItem = GetpChartItemSelected();
	if(lpSelItem)
		lpSelItem->ResetRQ();
	//�ʱ�ȭ �� �ٽ� TR������
	CString strRetryCode, strCurrentCode;
	strRetryCode = ((CInputBarDlg*)m_pInputBarDlg)->m_strOldCode;			
	strCurrentCode = ((CInputBarDlg*)m_pInputBarDlg)->GetCurrentCode();
	if(strCurrentCode.IsEmpty() || strRetryCode.IsEmpty() || strRetryCode == strCurrentCode)
		return;

	CString strCodeInputCode = ((CInputBarDlg*)m_pInputBarDlg)->m_pCodeInput->GetCode();
	if(strRetryCode.GetLength() && strCodeInputCode != strRetryCode)
	{
		((CInputBarDlg*)m_pInputBarDlg)->SetShareCodeChanged(TRUE);
		((CInputBarDlg*)m_pInputBarDlg)->m_pCodeInput->SetCode(strRetryCode);
	}

	((CInputBarDlg*)m_pInputBarDlg)->m_strCodeFromOutside = strRetryCode;
	((CInputBarDlg*)m_pInputBarDlg)->EnableWindow(FALSE);
		
	CCodeDataFromInputBar CodeData;
	CodeData.m_strCode = strRetryCode;
	CodeData.m_lEndTime = ((CInputBarDlg*)m_pInputBarDlg)->GetEndDate();
	CodeData.m_chType = ((CInputBarDlg*)m_pInputBarDlg)->GetType();
	CodeData.m_lTypeNum = ((CInputBarDlg*)m_pInputBarDlg)->GetTypeNumber();
	if(GetSafeHwnd()) // 2005. 04. 22 by Nam
		SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);

	((CInputBarDlg*)m_pInputBarDlg)->EnableWindow(TRUE);
	
//	CString strCodeReplace = ((CInputBarDlg*)m_pInputBarDlg)->m_strOldCode;
//	if(strCodeReplace.GetLength() == 0)
//	{
//		ResetOCX();
//		return;
//	}
//
//	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
//		m_pInputBarDlg->SendMessage(UMSG_CHART_INIT_FROMOCX, 1);
//
//	int nMarketType = ((CStdDialog*)GetParent())->GetMarketTypeFromCode(strCodeReplace);
//	CString strQueryType;
//	if(nMarketType == CMasterTypeInfo::STOCK_CHART)
//		strQueryType = QUERY_STRJONGMOK;
//	else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
//		strQueryType = QUERY_STRUPJONG;
//	else if(nMarketType == FUTOPT_CHART)
//		strQueryType = QUERY_STRFUTOPT;
//	else if(nMarketType == STOCKFUTOPT_CHART)
//		strQueryType = QUERY_STRSTOCKFUTOPT;
//	else if(nMarketType == FOREIGN_CHART)
//		strQueryType = QUERY_STRFOREIGN;
//
//	CChartItem* pChartItem = GetpChartItemSelected();
//	if(pChartItem)
//		pChartItem->SetCodeForQuery(strCodeReplace, strQueryType, TRUE);
//
//	RequestChartData(TRUE);
}

void CChartMng::MakePlatformMenu(HMENU hMenu)
{
	CMenu menu;
	menu.Attach(hMenu);

	CMenu menuTool, menuAnal, menuAddition, menuUser;
	menuTool.CreatePopupMenu();
	menuAnal.CreatePopupMenu();
	menuAddition.CreatePopupMenu();
	menuUser.CreatePopupMenu();

	int nToolID;
	CString strToolName;
	UINT nFlagsChild;
	int nCheckState;
	int nIndx = 0; 
	for(int nIndx = 0; nIndx < m_arrayAnalIdx.GetSize(); ++nIndx)
	{
		nToolID = m_arrayAnalIdx.GetAt(nIndx);
		nCheckState = m_arrayAnalCheck.GetAt(nIndx);
		if( nCheckState )
		{
			nFlagsChild = MF_STRING|MF_CHECKED|MF_BYCOMMAND;
		}
		else
		{
			nFlagsChild = MF_STRING|MF_UNCHECKED|MF_BYCOMMAND;
		}
		if(m_mapToolName.Lookup(nToolID, strToolName))
			menuAnal.AppendMenu(nFlagsChild,(UINT)ID_PLM_ADD_MENU_STARTANALTOOL+nToolID, strToolName);
	}
	for(nIndx = 0; nIndx < m_arrayAdditionIdx.GetSize(); ++nIndx)
	{
		nToolID = m_arrayAdditionIdx.GetAt(nIndx);
		nCheckState = m_arrayAdditionCheck.GetAt(nIndx);
		if( nCheckState )
		{
			nFlagsChild = MF_STRING|MF_CHECKED|MF_BYCOMMAND;
		}
		else
		{
			nFlagsChild = MF_STRING|MF_UNCHECKED|MF_BYCOMMAND;
		}
		if(m_mapToolName.Lookup(nToolID, strToolName))
			menuAddition.AppendMenu(nFlagsChild,(UINT)ID_PLM_ADD_MENU_STARTANALTOOL+nToolID, strToolName);
	}

	LPCSTR szSpecail = "���ؼ�";
	UINT   nSpecailID;
	for(nIndx = 0; nIndx < m_arrayUserIdx.GetSize(); ++nIndx)
	{
		nToolID = m_arrayUserIdx.GetAt(nIndx);
		nCheckState = m_arrayUserCheck.GetAt(nIndx);
		if( nCheckState )
		{
			nFlagsChild = MF_STRING|MF_CHECKED|MF_BYCOMMAND;
		}
		else
		{
			nFlagsChild = MF_STRING|MF_UNCHECKED|MF_BYCOMMAND;
		}
		if(m_mapToolName.Lookup(nToolID, strToolName))
		{
			if(strToolName.Compare(szSpecail)==0)
			{
				nSpecailID = ID_PLM_ADD_MENU_STARTANALTOOL+nToolID;
			}
			menuUser.AppendMenu(nFlagsChild,(UINT)ID_PLM_ADD_MENU_STARTANALTOOL+nToolID, strToolName);
		}
	}
	
	menuTool.AppendMenu(MF_POPUP, (UINT)menuAnal.m_hMenu, "�м�����");
	menuTool.AppendMenu(MF_POPUP, (UINT)menuAddition.m_hMenu, "�ΰ����");
	menuTool.AppendMenu(MF_POPUP, (UINT)menuUser.m_hMenu, "����ڵ���");

	CString str;
	int count = menu.GetMenuItemCount();
	int i = 0;
	for ( i = 0; i < count; i++)
	{
		if(menu.GetMenuString(i, str, MF_BYPOSITION) && (strcmp(str, "��ǥ�߰�") == 0))
			break;
	}

	menu.InsertMenu(i+1, MF_BYPOSITION|MF_SEPARATOR, (UINT)0, "");
	menu.InsertMenu(i+2, MF_BYPOSITION|MF_POPUP, (UINT)menuTool.m_hMenu, "�� ����");
	menu.InsertMenu(i+2, MF_BYPOSITION, nSpecailID, szSpecail);

	menuAnal.Detach();
	menuAddition.Detach();
	menuUser.Detach();
	menuTool.Detach();
	menu.Detach();
}

BOOL CChartMng::GetDlgToolBtnToggle(CArray<int, int>& arAnal, CArray<int, int>& arAnalCheck, CArray<int, int>& arAddition, CArray<int, int>& arAdditionCheck, CArray<int, int>& arUser, CArray<int, int>& arUserCheck, CMap<int, int, CString, CString>& mapToolName)
{
	arAnal.RemoveAll();
	arAnalCheck.RemoveAll();
	arAddition.RemoveAll();
	arAdditionCheck.RemoveAll();
	arUser.RemoveAll();
	arUserCheck.RemoveAll();
	mapToolName.RemoveAll();

	return ((CStdDialog*)GetParent())->GetDlgToolBtnToggle(arAnal, arAnalCheck, arAddition, arAdditionCheck, arUser, arUserCheck, mapToolName);
}

void CChartMng::SetDlgToolBtnToggle(int nToolBtnID, BOOL bToggle/*=TRUE*/)
{
	((CStdDialog*)GetParent())->SetDlgToolBtnToggle(nToolBtnID, bToggle);
}


void CChartMng::AddMainMenuToPlatformMenu(HMENU hMenu)
{
	if(!hMenu) return;

	//���ڼ� ǥ��
	CExtInputBarDlg* pExtInputBarDlg = ((CExtInputBarDlg*)GetpExtInputBarDlg());
	BOOL bCrossLine = FALSE;
	if(pExtInputBarDlg)  bCrossLine = pExtInputBarDlg->GetChkCrossLine();

	UINT nFlagsChild = MF_STRING|MF_UNCHECKED|MF_BYPOSITION;
	UINT nMenuID = ID_PLM_ADD_MENU;
	int  nMenuPosFix=0, nMenuPos = 10;

	HMENU hChartMenu = hMenu;
	CMenu* pChartMenu = CMenu::FromHandle(hMenu);


	//pChartMenu->InsertMenu(0, MF_BYPOSITION | MF_SEPARATOR);
// 	if(pExtInputBarDlg)
// 	{
// 		if(bCrossLine) nFlagsChild = MF_STRING|MF_CHECKED|MF_BYPOSITION;
// 	}
// 	else
// 	{
// 		nFlagsChild |= MF_GRAYED; //MF_DISABLED;
// 	}
// 	pChartMenu->InsertMenu(0, nFlagsChild, nMenuID+nMenuPosFix, "���ڼ� ǥ��");
	//-->"���ؼ�����"
	nFlagsChild = MF_STRING|MF_UNCHECKED|MF_BYPOSITION;
	pChartMenu->InsertMenu(0, nFlagsChild, nMenuID+(nMenuPosFix+1), "���ؼ�����");
	//<--

	CString szCrossText(_T("���ڼ� ǥ��"));
	if(pExtInputBarDlg)
	{
		if(bCrossLine)
		{
			nFlagsChild = MF_STRING|MF_CHECKED|MF_BYPOSITION;
			szCrossText = _T("���ڼ� �����");
		}
	}
	else
	{
		nFlagsChild |= MF_GRAYED; //MF_DISABLED;
	}
	pChartMenu->InsertMenu(0, nFlagsChild, nMenuID+nMenuPosFix, szCrossText);

	nFlagsChild = MF_STRING|MF_UNCHECKED|MF_BYPOSITION;
	pChartMenu->InsertMenu(0, MF_BYPOSITION | MF_SEPARATOR);
	pChartMenu->InsertMenu(0, nFlagsChild, nMenuID+nMenuPos++, "����ȭ�� ����...");

	{
		CStringList szKeyList;
		IPartnerSvrManager* pPtSvrMng = (IPartnerSvrManager*)AfxGetPctrInterface(UUID_IPartnerSvrManager);
		if(pPtSvrMng)
		{
			IPartnerUtilManager* pPtUtilMng = (IPartnerUtilManager*)pPtSvrMng->GetInerInterface(LUID_IPartnerUtilManager);
			if(pPtUtilMng)
			{
				CString szScreen;
				szScreen.Format("%04d", theStdParent->m_nScreenID);
				pPtUtilMng->GetLinkScreenInfo(szScreen, szKeyList);
			}
		}

		if(szKeyList.GetCount()>0)
			pChartMenu->InsertMenu(0, MF_BYPOSITION | MF_SEPARATOR);

		CString rMenuTxt;
		for(POSITION pos=szKeyList.GetTailPosition(); pos; )
		{
			rMenuTxt = szKeyList.GetPrev(pos);
			pChartMenu->InsertMenu(0, nFlagsChild, nMenuID+nMenuPos, rMenuTxt.Mid(8));
			nMenuPos++;			
		}
	}
}

void CChartMng::DoSiteMenuCall(int _nMenuID)
{
	UINT nMenuID = _nMenuID - ID_PLM_ADD_MENU;
	if(nMenuID<10)
	{
		if(nMenuID==0)
		{
			CExtInputBarDlg* pExtInputBarDlg = ((CExtInputBarDlg*)GetpExtInputBarDlg());
			if(pExtInputBarDlg && pExtInputBarDlg->GetSafeHwnd())
			{
				BOOL bCrossLine = pExtInputBarDlg->GetChkCrossLine();
				pExtInputBarDlg->PostMessage(RMSG_CHANGE_CROSSLINE, 1, (LPARAM)!bCrossLine);
				pExtInputBarDlg->PostMessage(WM_COMMAND, IDC_BT_SUB2, 0);
			}
		}
		else if(nMenuID==1)
		{
			//CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION
			//AfxMessageBox("���ؼ�");
			SendMessage(UMSG_CHARTGUIDELINE, NULL, FALSE);
		}
	}
	else
	{
		nMenuID -= 10;
		IPartnerSvrManager* pPtSvrMng = (IPartnerSvrManager*)AfxGetPctrInterface(UUID_IPartnerSvrManager);
		if(pPtSvrMng)
		{
			IPartnerUtilManager* pPtUtilMng = (IPartnerUtilManager*)pPtSvrMng->GetInerInterface(LUID_IPartnerUtilManager);
			if(pPtUtilMng)
			{
				CString szScreen;
				szScreen.Format("%04d", theStdParent->m_nScreenID);

				pPtUtilMng->GetLinkScreenOpen(theStdParent->m_hPlatform, szScreen, nMenuID, FALSE);
			}
		}
	}
}

int CChartMng::GetPositionInMenu(CString strMenu, CMenu* pMenu)
{
	int i=-1;
	int nMenuCount = pMenu->GetMenuItemCount();
	CString strTemp;
	for(i=0;i<nMenuCount;i++)
	{
		pMenu->GetMenuString(i, strTemp, MF_BYPOSITION);
		if(strTemp=="") continue;
		if(!strTemp.Compare(strMenu)) return i;
	}
	return -1;
}

LRESULT CChartMng::OnAddPlatformMenu( WPARAM wParam, LPARAM lParam)
{
	HMENU hMenu = (HMENU)lParam;
	AddMainMenuToPlatformMenu(hMenu);
	if(GetDlgToolBtnToggle(m_arrayAnalIdx, m_arrayAnalCheck, m_arrayAdditionIdx, m_arrayAdditionCheck, m_arrayUserIdx, m_arrayUserCheck, m_mapToolName))
		MakePlatformMenu(hMenu);

	return 0;
}

// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
LRESULT CChartMng::OnGetCurrentMode( WPARAM wParam, LPARAM lParam)
{
	if( m_pInputBarDlg)
		return (LRESULT)((CInputBarDlg*)m_pInputBarDlg)->GetCurrentMode();

	return (LRESULT)CHANGE_ITEM;
}

int CChartMng::IsAvailableForReplay()
{
	int nReason = 0;
	int nChartCount = 0;
	EACH_CHARTJOB_START()
		nChartCount++;
		if(nReason = pChartItem->IsAvailableForReplay())
			break;
	EACH_CHARTJOB_END()	

	if(nChartCount > 1)
		nReason = 2;	// Ref Count �ʰ�
	return nReason;
}

LRESULT CChartMng::OnSetQueryOpy( WPARAM wParam, LPARAM lParam)
{
	((CExtInputBarDlg*)GetpExtInputBarDlg())->SetQueryOpt((int)wParam);
	((CInputBarDlg*)GetpInputBarDlg())->SetQueryOpt((int)wParam);
	return 0l;
}

LRESULT CChartMng::OnChangeVertScaleType( WPARAM wParam, LPARAM lParam)
{
	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();

//#ifdef __MULTI_LANG__ //@Solomon-100225 
	pChartItem->ChangeVertScaleMinMaxType(0, 0, 0, (short)wParam, -1, -1);

	return 0l;
}

void CChartMng::SetChartLayout(int nLayout)
{
	m_nChartLayout = nLayout;
	LoadDWMChart();
}

BOOL CChartMng::LoadDWMChart()
{
	CChartItem* pChartItem;
	if(m_nChartLayout == 1)
	{
		if(m_pChartItems[0][0]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[0][0];
			pChartItem->SetType(WEEK_DATA_CHART);
		}
		
		if(m_pChartItems[0][1]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[0][1];
			pChartItem->SetType(MONTH_DATA_CHART);
		}
		
		if(m_pChartItems[1][0]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[1][0];
			pChartItem->SetType(DAY_DATA_CHART);
		}
	}
	else if(m_nChartLayout == 2)
	{
		if(m_pChartItems[0][0]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[0][0];
			pChartItem->SetType(DAY_DATA_CHART);
		}
		
		if(m_pChartItems[0][1]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[0][1];
			pChartItem->SetType(WEEK_DATA_CHART);
		}
		
		if(m_pChartItems[1][0]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[1][0];
			pChartItem->SetType(MONTH_DATA_CHART);
		}
	}

	return TRUE;
}

void CChartMng::GoNextChart()
{
	if(!m_strCodeInLoop.IsEmpty())
		return;

	switch(m_nDWMChartIndex)
	{
	case 2:
		{
			m_pChartItems[0][1]->ResetOCX(FALSE);
			m_pChartItems[0][1]->CopyData(m_pChartItems[0][0]);
		}
	case 3:
		{
			m_pChartItems[1][0]->ResetOCX(FALSE);
			m_pChartItems[1][0]->CopyData(m_pChartItems[0][0]);
		}
	}

	m_strCodeInLoop = m_pChartItems[0][0]->GetDataCode();
}

void CChartMng::SendPrevDateData()
{
	if(m_bFutPrevChart)
	{
		m_bFutPrevChart = FALSE;
		return;
	}

	if(!m_strCodeInLoop.IsEmpty())
		return;
/*
	CTime tmCur = CTime::GetCurrentTime();

	time_t tm1 = tmCur.GetTime();
	tm1 -= 86400; // (24 * 60 * 60)

	CTime tmPrev(tm1);
	
	CString strDate;

	strDate = tmPrev.Format("%Y%m%d");
*/

	CString strDate;

	CStdDialog* pDlg = (CStdDialog*)GetParent();
	if (pDlg && pDlg->m_saWorkDate.GetSize() > 1)
		strDate = pDlg->m_saWorkDate.GetAt(1);

	m_nOldMode = -1;
	SetMode(OVERLAPP_ITEM);
	SetAddMode(TRUE);
	m_pChartItems[0][0]->ClearData();
	m_pChartItems[0][0]->SetEndDateRequested(strDate);

	m_strCodeInLoop = m_pChartItems[0][0]->GetDataCode();
	m_pChartItems[0][0]->SetCntForRequest(500);

	m_bFutPrevChart = TRUE;
	int nQryOpt = ((CExtInputBarDlg*)GetpExtInputBarDlg())->GetQueryOpt();
	m_pChartItems[0][0]->SetQueryOpt(2);
	RequestChartData();
	m_pChartItems[0][0]->SetQueryOpt(nQryOpt);

	if (pDlg && pDlg->m_saWorkDate.GetSize() > 0)
		strDate = pDlg->m_saWorkDate.GetAt(0);

	m_pChartItems[0][0]->SetEndDateRequested(strDate);
}

void CChartMng::SetInLoop(BOOL bLoop)
{ 
	m_bIsInLoop = bLoop; 

	if(bLoop)
	{
		m_lColSelectedInLoop = m_lColSelected;
		m_lRowSelectedInLoop = m_lRowSelected;
		m_nRowIndexInLoop = 0;
		m_nColIndexInLoop = 0;
	}
}

void CChartMng::ShowAvgLine(CString strCode, CString strPrice)
{
	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();

	if(!pChartItem)
		return;

	if(strCode.IsEmpty())
		strCode = pChartItem->GetSelectedItemCode();
	else if(pChartItem->GetSelectedItemCode() != strCode)
		return;

//#ifdef __MULTI_LANG__ //@Solomon-100225
	pChartItem->SetAvgUnitPrice(atof(strPrice));
	pChartItem->Invalidate();
}

void CChartMng::GetCurStatusInfo(char& cType, CString& strNumber, CString& strDate, CString& strTime, long& lCount)
{
	CChartItem* pChartItem = GetpChartItemSelected();
	if(!pChartItem)
		return;

	cType = pChartItem->GetType();
	strNumber = pChartItem->GetTickMinDayNumber();
	strDate = pChartItem->GetEndDate();
	strTime = pChartItem->GetData("�ڷ�����", -1);
	lCount = pChartItem->GetCntForRequest();
}

long CChartMng::GetPacketCount(LPCTSTR lpszPacketName)
{
	CChartItem* pChartItem = GetpChartItemSelected();
	if(!pChartItem)
		return 0;

//#ifdef __MULTI_LANG__ //@Solomon-100225
	return pChartItem->GetPacketCount(lpszPacketName);

}

short CChartMng::GetPacketIndex(LPCTSTR lpszPacketName, double dData)
{
	CChartItem* pChartItem = GetpChartItemSelected();
	if(!pChartItem)
		return 0;

	return pChartItem->GetPacketIndex(lpszPacketName, dData);
}

short CChartMng::GetPacketNearIndex(LPCTSTR lpszPacketName, double dData)
{
	CChartItem* pChartItem = GetpChartItemSelected();
	if(!pChartItem)
		return 0;

//#ifdef __MULTI_LANG__ //@Solomon-100225
//	return 0;
//#else
	return pChartItem->GetPacketNearIndex(lpszPacketName, dData);
//#endif
}

CString CChartMng::GetPacketData(LPCTSTR lpszPacketName, short nDataIndex)
{
	CChartItem* pChartItem = GetpChartItemSelected();
	if(!pChartItem)
		return "";

	return pChartItem->GetPacketData2(lpszPacketName, nDataIndex, FALSE);
}

void CChartMng::AddConcPacketGraph()
{
	CChartItem* pChartItem = GetpChartItemSelected();
	if(!pChartItem)
		return;

	if(!pChartItem->HasGraph("�ŵ�ü��"))
		pChartItem->AddGraph(0,0,0,"�ŵ�ü��",FALSE,FALSE,FALSE);

	if(!pChartItem->HasGraph("�ż�ü��"))
		pChartItem->AddGraph(0,0,0,"�ż�ü��",FALSE,FALSE,FALSE);
}

void CChartMng::AddConcPacketData(LPCTSTR lpszConcTimePacketData, int nConcCountLen,
								  LPCTSTR lpszSellCountPacketData, int nSellCountLen,
								  LPCTSTR lpszSellPricePacketData, int nSellPriceLen,
								  LPCTSTR lpszBuyCountPacketData, int nBuyCountLen,
								  LPCTSTR lpszBuyPricePacketData, int nBuyPriceLen)
{
	CChartItem* pChartItem = GetpChartItemSelected();
	if(!pChartItem)
		return;

//	pChartItem->AddPacket("1;ü���Ͻ�;;8;�� 1;��;\r\n");
	pChartItem->AddPacket("1;�ŵ��Ǽ�;;5;�� 1;��;\r\n");
	pChartItem->AddPacket("1;�ŵ��ܰ�;;9;�� 1;��;\r\n");
	pChartItem->AddPacket("1;�ż��Ǽ�;;5;�� 1;��;\r\n");
	pChartItem->AddPacket("1;�ż��ܰ�;;9;�� 1;��;\r\n");

//	pChartItem->SetPacketData2("ü���Ͻ�", "ü���Ͻ�:8|@", lpszConcTimePacketData, nConcCountLen, TRUE);
	pChartItem->SetPacketData2("�ŵ��Ǽ�", "�ŵ��Ǽ�:5|@", lpszSellCountPacketData, nSellCountLen, TRUE);
	pChartItem->SetPacketData2("�ŵ��ܰ�", "�ŵ��ܰ�:9|@", lpszSellPricePacketData, nSellPriceLen, TRUE);
	pChartItem->SetPacketData2("�ż��Ǽ�", "�ż��Ǽ�:5|@", lpszBuyCountPacketData, nBuyCountLen, TRUE);
	pChartItem->SetPacketData2("�ż��ܰ�", "�ż��ܰ�:9|@", lpszBuyPricePacketData, nBuyPriceLen, TRUE);
}

void CChartMng::RemoveConcPacketData()
{
	CChartItem* pChartItem = GetpChartItemSelected();
	if(!pChartItem)
		return;

	pChartItem->RemoveGraph(0, 0, "�ŵ�ü��");
	pChartItem->RemoveGraph(0, 0, "�ż�ü��");

//	pChartItem->RemovePacket("ü���Ͻ�");
	pChartItem->RemovePacket("�ŵ��Ǽ�");
	pChartItem->RemovePacket("�ŵ��ܰ�");
	pChartItem->RemovePacket("�ż��Ǽ�");
	pChartItem->RemovePacket("�ż��ܰ�");
}

//@Solomon100225:�ַθ󿡼��� ������.
// 	void CChartMng::AddPopupMenuItem(CMenu* pPopupMeun, CPoint* ptPoint)
// 	{
// 		CChartItem* pChartItem = GetpChartItemSelected();
// 		if(!pChartItem)
// 			return;
// 
// 	#ifndef __MULTI_LANG__ //@Solomon-100225
// 		pChartItem->AddPopupMenuItem((long)pPopupMeun, (long)ptPoint);
// 	#endif
// 	}
// 
// 	void CChartMng::ActionPopupMenuItem(UINT nMenuID)
// 	{
// 		CChartItem* pChartItem = GetpChartItemSelected();
// 		if(!pChartItem)
// 			return;
// 
// 	#ifndef __MULTI_LANG__ //@Solomon-100225
// 		pChartItem->ActionPopupMenuItem(nMenuID);
// 	#endif
// 	}

BOOL CChartMng::GetSendShareFlag()
{
	return m_bJongmokLink || m_bTypeLink || !m_strMultiCodes.IsEmpty();
}

BOOL CChartMng::GetJongmokLink()
{
	return m_bJongmokLink;
}

void CChartMng::SaveViewCount()
{
	if(!((CExtInputBarDlg*)GetpExtInputBarDlg())->GetQueryOpt())
		return;

	m_nAllDayViewCount = GetpChartItemSelected()->GetNOnePageDataCount();
}

CString CChartMng::GetOldCode()
{
	CChartItem* pChartItem = GetpChartItemSelected();
	if(!pChartItem)
		return "";

	return pChartItem->GetOldChartCode();
}

//>> [QC No]090728HMC040
void CChartMng::UpdateSyncIndicator()
{
	int nChartCount = 0;
	EACH_CHARTJOB_START()
		nChartCount++;
	EACH_CHARTJOB_END()	

	CChartItem* pSelectedChartItem = GetpChartItemSelected();
	if(nChartCount > 1 && pSelectedChartItem->GetRQCount() == 1)
		((CExtInputBarDlg*)GetpExtInputBarDlg())->m_chkUpdateIndex.EnableWindow(TRUE);
	else
		((CExtInputBarDlg*)GetpExtInputBarDlg())->m_chkUpdateIndex.EnableWindow(FALSE);
}
//<< [QC No]090728HMC040

void CChartMng::ReceiveRealHogaData(DWORD dwKey, LPCTSTR pszCode, long dwData)
{
	if (m_pFloatingDlg)
		m_pFloatingDlg->SetRealHogaData(dwKey, pszCode, dwData);
}

//@Solomon-A002
void CChartMng::_DoSavedData_To_LastCode(LPCSTR strFileName)
{
	//@Solomon-A002
	return;

	CStdDialog* stdDialog = (CStdDialog*)GetParent();
	if(!stdDialog) return;

	char szTemp[32];
	memset(szTemp, 0x00, sizeof(szTemp));
	::GetPrivateProfileString("0000", "Code00", "", szTemp, sizeof(szTemp), strFileName);

	CString rString=szTemp;
	int nMarketType, nStep=0;
	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	nMarketType = pMasterDataManager->GetMarketTypeFromCode(rString);

	stdDialog->GetFirstCode(rString, nMarketType, nStep);

	::WritePrivateProfileString("0000", "Code00", rString, strFileName);
}

BOOL CChartMng::GetSaveAnalTool()
{
	//return m_bSaveAnalTool;
	return TRUE;
}

//20110210_alzioyes:[A00000221]��Ʈ��ũ�ѱ�ɰ� ���񵹷����� ���ÿ� �ȵǵ���.
LRESULT CChartMng::OnRmgGetInfo(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case 1: //�������
			{
				long dType = (long)lParam;
				switch(dType)
				{
					case 0: //ExtInputBar�� ��� �������
						return ((CExtInputBarDlg*)GetpExtInputBarDlg())->GetScrollState();

					case 1: //CTabCodeSearch������ �������.
						return m_nLeftbarReplayState;
				}
			}
			break;
	}
	return 0L;
}

LRESULT CChartMng::OnRmgSetInfo(WPARAM wParam, LPARAM lParam)
{
	if(wParam==1)
	{
		m_nLeftbarReplayState = (int)lParam;
		return 0L;
	}

	return 0L;
}

BOOL CChartMng::GetRelayPossible()
{
	int nChartItemCnt = m_lCols * m_lRows;
	if( nChartItemCnt != 4 )
		return FALSE;

	char chType[4] = {'2', '3', '4', '1' }; // ��(2)/��(3)/��(4)/��(1)
	int nIndexType = 0;

	EACH_CHARTJOB_START()
		if( chType[ nIndexType ] !=	pChartItem->GetType() )
			return FALSE;
		nIndexType++;		
	EACH_CHARTJOB_END()

	return TRUE;
}