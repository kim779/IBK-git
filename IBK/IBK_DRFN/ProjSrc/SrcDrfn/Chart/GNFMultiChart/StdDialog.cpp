/****************************************************************************/
/* Project   : Woori Futures Fx Margin System                               */
/* File Name : StdDialog.cpp                                                */
/* Function  : ��Ƽ ��Ʈ ȭ�� ���̾�α�                                    */
/* Comment   : DRFN Chart�� �ҽ��� �޾� ����� �������̽��� ����            */
/* Copyright : 2008 DRFN                                                    */
/*             Modify Korea Sorimachi                                       */
/****************************************************************************/
#include "./define/ChartMsgDef.h"
#include "ChartItem.h"

#include "stdafx.h"
#include "MultiChart.h"
#include "IO.h"
#include "direct.h"
#include "StdDialog.h"
#include "MiniBarDlg.h"
#include "TabCodeSearch.h"
#include "ChartListDlg.h"
#include "FloatingHogaDlg.h"

#include "../COMMONTR/ChartRealConverter_GNF.h"
#include "DataWindowDlg.h"
#include "CodeDataFromInputBar.h"
#include "./define/ReserveShareName.h"
#include "./define/MessageDefine.h"
#include "./define/DragNDropDefine.h"
#include "./include/UChar.h"

#include "../chart_common/StatusMsgDef.h"
#include "../../inc/IMainInfoMng.h"
#include "../../inc/IAUPlatformDispMng.h"
#include "../../inc/IKSLibMng.h"
#include "../../inc/ExCommonHeader.h"
#include "../../inc/RealUpdate.h"
#include "../../inc/IAUDataMonitorMng.h"			// UUID_IAUDataMonitorManager : �ݹ��Լ� �޴µ� ���
#include "DlgGlass.h"								// ������ ���� - ojtaso (20080519
#include "../CommonTR/EU_p0602.h"	//�ֽ�
#include "../Include_Chart/Dll_Load/IMetaTable.h"	// (2008/9/13 - Seung-Won, Bae) for Multi-Language
#include "../../inc/RealUpdate.h"
#include "../CommonTR/SM_CheJanStruct.h"			// ü�� - �ֹ������� ���


//	�߰�... �ֶ󸮽� ȯ�濡�� ������ ������ �Ѵ�. �� �����̰� �����
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")		// htonl, ntohl


#define	WMU_GET_KEY_INFO_ARRAY		WM_USER + 2253	// �ش� AUP���� �ԷµǾ� �ִ� Key���� ����� �����´�. LPARAM�� CStringArray *

#define	WIDTH_SIDEBAR		3

//<<< 20091215 by kmlee for �ֹ�����
const UINT RMSG_CHANGE_CHARTACCOUNT = ::RegisterWindowMessage(_T("RMSG_CHANGE_CHARTACCOUNT"));
const UINT RMSG_GET_CHARTACCOUNT = ::RegisterWindowMessage(_T("RMSG_GET_CHARTACCOUNT"));
const UINT RMSG_ACCOUNT_INIT = ::RegisterWindowMessage(_T("RMSG_ACCOUNT_INIT"));
//>>>


const UINT RMSG_GET_SCREENTYPE = ::RegisterWindowMessage(_T("RMSG_GET_SCREENTYPE"));
const UINT RMSG_GET_MARKETTYPE = ::RegisterWindowMessage(_T("RMSG_GET_MARKETTYPE"));
const UINT RMSG_GET_LINKINFODATA = ::RegisterWindowMessage(_T("RMSG_GET_LINKINFODATA"));
const UINT RMSG_GET_SCREENGB = ::RegisterWindowMessage(_T("RMSG_GET_SCREENGB"));

const UINT RMSG_USERINFO = ::RegisterWindowMessage(_T("RMSG_USERINFO"));

#define	USERINFO_GET	0x01
#define	USERINFO_SET	0x02

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CStdDialog dialog
#define ID_LEFTBAR			IDD_LEFTBAR
#define ID_INPUTBAR			IDD_INPUTBAR
#define ID_MINIBAR			IDD_MINIBAR
#define ID_EXTINPUTBAR		IDD_EXTINPUTBAR
#define ID_SPLITTER			7000
#define ID_CHART			8000
#define ID_TOOLBAR			IDD_DLG_TOOLBAR

// shared Info Name.. gm0604 2005.8.24
#define MULTICHARTOPEN		"MultiChartOpen"
#define MULTICHARTCLOSE		"MultiChartClose"
#define FROMSTCENTER		"FromSTCenter"

#define DEFAULT_CHARTDATA_COUNT_INT		400
#define DEFAULT_CHARTDATA_COUNT_CHAR	"400"

// --> [Edit]  ������ 2008/08/06		( ���� ���̵���� ���̸� ���δ�. )
//#define WIDTH_SIDEBAR_EXTEND		170
#define WIDTH_SIDEBAR_EXTEND		165
// <-- [Edit]  ������ 2008/08/06
#define WIDTH_SIDEBAR_COLLAPSE		10


extern HINSTANCE g_hInstance;

CString Parser(LPCTSTR lpInputData, int *pNShifter ) 
{
	if(!pNShifter) return "";
	CString strInputData;
	int nPosStart = *pNShifter;
	int nPosEnd = 0;
	strInputData = lpInputData;
	nPosEnd = strInputData.Find(',',nPosStart);
	if(nPosEnd==-1)
	{
		nPosEnd = strInputData.GetLength();
		if(nPosStart>=nPosEnd)
		{
			return "";
		}		
	}
	*pNShifter = nPosEnd + 1;
	return strInputData.Mid(nPosStart,nPosEnd-nPosStart);
}

// CMultiChartWnd
#define HEIGHTOFINPUTBAR	(24)
#define WIDTHOFSPLITTER		(3)
#define WIDTH_TOOLBAR		(32)

const UINT RMSG_GETMAINWND	= ::RegisterWindowMessage(_T("RMSG_GETMAINWND"));

#define Memset(x) memset(x, 0x00, sizeof(x))

CString SetComma(CString strNumber,BOOL bShowSign=FALSE)
{
	CString szRet=_T("");
	CString szTmp = strNumber;
	szTmp.TrimLeft();
	szTmp.TrimRight();

	char	ch = NULL;
	int nLen = strlen(szTmp);
	for(int i=0;  i<nLen ; i++) {
		ch = szTmp[i];
		if(ch >= '0' && ch <= '9'||ch == '.' || ch == '-' || ch== '+') 
			szRet += ch;
	}
	szTmp = szRet;

	if(szTmp.IsEmpty())	return "" ;
	if(szTmp[0]=='+' ||szTmp[0]=='-') {
		ch = szTmp[0];
		szTmp = szTmp.Mid(1);
	}

	szTmp.MakeReverse();
	nLen = szTmp.GetLength();	
	szRet.Empty();
	BOOL bIsFloat = FALSE;
	if(szTmp.Find('.')!=-1)
	{
		bIsFloat = TRUE;
	}
	int j = 0;
 	for(i=0;   i<nLen;   i++) 
	{
		if(szTmp[i]=='.') 	bIsFloat = FALSE;
		else
		{
			if(!bIsFloat)
			{
				if(j!=0 && !(j%3))	szRet += ',';
				j++;
			}
		}
		szRet += szTmp[i];
	}
	if(ch=='-') szRet += ch;
	if(bShowSign&&ch=='+'&&szRet!="0")
	{
		szRet += ch;
	}
	szRet.MakeReverse();
	return szRet;
}

int		   CStdDialog::m_static_nChartCnt4ImgList = 0;

CImageList CStdDialog::m_ImgList_W15H15; //�� 15�� ���� 15�� ��ư �̹�������Ʈ
CImageList CStdDialog::m_ImgList_W20H20; //�� 20�� ���� 19�� ��ư �̹�������Ʈ
CImageList CStdDialog::m_ImgList_W20H20_Tool; //�� 20�� ���� 19�� ��ư �̹�������Ʈ
CImageList CStdDialog::m_ImgList_W46H20; //�� 46�� ���� 19�� ��ư �̹�������Ʈ	
CImageList CStdDialog::m_ImgList_W50H20; //�� 50�� ���� 19�� ��ư �̹�������Ʈ
CImageList CStdDialog::m_ImgList_W151H21; //�� 151�� ���� 21�� ��ư �̹�������Ʈ
CImageList CStdDialog::m_ImgList_W28H22; //�� 28�� ���� 22�� ��ư �̹�������Ʈ
CImageList CStdDialog::m_ImgList_W17H20; //�� 17�� ���� 20�� ��ư �̹�������Ʈ
CImageList CStdDialog::m_ImgList_W43H20; //�� 43�� ���� 20�� ��ư �̹�������Ʈ
CImageList CStdDialog::m_ImgList_W58H20; //�� 58�� ���� 20�� ��ư �̹�������Ʈ
CImageList CStdDialog::m_ImgList_W72H20; //�� 72�� ���� 20�� ��ư �̹�������Ʈ
CImageList CStdDialog::m_ImgList_W46H20_Gwan; //�� 46�� ���� 20�� ��ư �̹�������Ʈ
CImageList CStdDialog::m_ImgList_W8H25;  //�� 8�� ���� 25�� ��ư �̹�������Ʈ

CMapStringToString	CStdDialog::m_mapCodeType;

extern CMultiChartApp theApp;
/****************************************************************************/
/* Function : ������														*/
/* Paramter :																*/
/* Return	:																*/
/* Author	: By DRFN														*/
/* Comments	: FX Margin �ϸ鼭 �ҽ� ���� (�ѱ��Ҹ�ġ �����)				*/
/****************************************************************************/
CStdDialog::CStdDialog(CWnd* pParent /*=NULL*/)
	: CExOCXDLL(CStdDialog::IDD, pParent),
	m_nDrdsInfoCnt(0)
{
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	m_nRequestPairTickState = E_RPTS_STOP;
	m_bOrgAddMode = FALSE;

	EnableAutomation();
	//{{AFX_DATA_INIT(CStdDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLeftBarDlg		= NULL;
	m_pExtInputBarDlg	= NULL;
	m_pInputBarDlg		= NULL;
	m_nLeftBarType		 = 0;
	m_pData				= NULL;
	m_bSmallScreenMode	= FALSE;
	m_nTempLeftBarType	= 0;
	m_pMsgBar			= NULL;
	m_bEnable			= TRUE;
	m_bFirstGShared		= TRUE;
	m_bShowExtInputBarDlg = FALSE;
	m_bShowInputBarDlg	= TRUE;
	m_strMapID			= "33100";
	m_nWidthOfLeftBar	= 0;
	m_nSpliteID			= 0;
	m_nTabID			= 0;
	m_nChartType		= SINGLE_CHART;
	m_nImportedMapID	= 0;
	m_nWidthOfDlg		= 0;
	m_nHeightOfDlg		= 0;
//	m_hViewInMain		= NULL;
	m_lWidthOriginal	= 0;
	m_lHeightOriginal	= 0;
	m_bHideChart		= FALSE;
	m_bInitResize		= FALSE;

	m_hSocket			= NULL;
	m_hMainFrame		= NULL;

	m_bShowDataListWnd	= FALSE;			// ������ �����츦 ������ ����
	m_hReplayChartLib	= NULL;				// ����  Lib Handle

	m_nMarketType		= CMasterTypeInfo::STOCK_CHART;

	m_bActiveDataWindow = false;
	m_pDataWindow		= NULL;
	m_pRqWndFromTab		= NULL;

	m_pWndReplayLib		= NULL;				//���� ���� Wnd Pointer


	memset(&m_ChartCfgInfo, 0x00, sizeof(CHART_CONFIG_INFO));

	// (2008/11/2 - Seung-Won, Bae) Set Default for Imaginary Candle and Gap Revision.
	m_ChartCfgInfo.bImaginaryCandle	= true;		// TRUE �� ���
	m_ChartCfgInfo.bUseGapRevision	= false;	// TRUE �� ������
	
	m_bNeedRqstTRForReplay	= FALSE;
	m_bReplayMode			= FALSE;

	m_strConfig				= "";
	m_strRootPath			= "";

	m_TRComm				 = NULL;
	m_bSystemChart			= FALSE;
	m_bSimpleChart			= FALSE;
	m_bMultiItemChart		= FALSE;
	m_hCursor				= NULL;
	m_cUM					= 'E';
	m_strDollar				= "";
	m_pDlgToolbar			= NULL;
	m_bOneClickHide			= FALSE;
	m_bShowToolBar			= FALSE;
	m_nMarketTrCode			= atoi(QUERY_STRMARKET);

	m_nExtendState			= 0;		// 20061212 ������ ����, grid column Ȯ�� btn �߰�
	m_cMUM					= 'P';
	m_bPreMarket			= FALSE;	// ���� ������ ó�� by 20070226 by LYH
	m_cUT = '1';						// ƽ������ ���� ��� Ŭ���� ���� ó��
	m_strRecvCode			= "";
	m_strSvcID				= "CP986100";// 2007.05.02 by LYH ���� ��� ���� ���� ID �߰�

	m_pDrdsLib				= NULL;
	m_pDlgGlass				= NULL;		// ������ ���� - ojtaso (20080519)

	g_pPtUtilMng = NULL;

	m_bMigyulcheDraw		= TRUE;

// --> [Edit]  ������ 2008/10/29
//@Sol100618	m_pCommMsg				= NULL;

	m_nCodeInput			= TRUE;
	m_strHwndKey			= "";
	m_strScreenKey			= "";
// <-- [Edit]  ������ 2008/10/29

	m_hWinixWnd				= NULL;
}


void CStdDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStdDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_BTN_EXT_TOOL, m_btnExtTool);
	DDX_Control(pDX, IDC_SIDEBAR, m_btnSideBar);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStdDialog, CDialog)
	//{{AFX_MSG_MAP(CStdDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_EXT_TOOL, OnBtnExtTool)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_SPLITTER_MOVED, OnWmSplitterMoved )	
//	ON_MESSAGE( WM_GET_BROD, OnGetBroad )
//@Solomon_GNF	ON_MESSAGE( WM_GET_BROD, OnGetBroad )
	ON_REGISTERED_MESSAGE(RMS_ANALYSIS_FUNC		, OnByPass_ExportToolOption ) //2005. 07. 28 added by sy,nam	
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_REAL		, OnReplayDrds )
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_FAIL		, OnReplayFail )
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_CHANGE_DATE	, OnReplayChangeDate)
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_END			, OnReplayEnd)			//���� ������.
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_TOTAL_SEC		, OnReplaySetTotalSec)	//06.01.10
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_INC_SEC		, OnReplayEachSec)		//06.01.10

	ON_REGISTERED_MESSAGE(RMSG_DRAGSTART, OnDragStart)			// Added to drag and drop operation
	ON_REGISTERED_MESSAGE(RMSG_DRAGEND, OnDragEnd)				// ������ Date : 2005/2/23

	ON_MESSAGE(UMSG_FORMINITAFTER_FROMFORM, OnFormInitAfter)
	ON_MESSAGE(UMSG_FORMGETPAGESAVE, OnFormGetPageSave)
	ON_MESSAGE(UMSG_FORMSETPAGESAVE, OnFormSetPageSave)
	ON_REGISTERED_MESSAGE(RMSG_CHANGE_SKINCOLOR, OnChangeSkinColor)
	ON_REGISTERED_MESSAGE(RMSG_GET_SKINCOLOR, OnGetSkinColor)
	ON_REGISTERED_MESSAGE(RMSG_USERINFO, OnUserInfo)
	ON_REGISTERED_MESSAGE(RMSG_ACCOUNT_INIT, OnAccountInit)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CStdDialog, CExOCXDLL)
	//{{AFX_DISPATCH_MAP(CStdDialog)
	DISP_FUNCTION(CStdDialog, "SetOpenMapData", SetOpenMapData, VT_EMPTY, VTS_BSTR VTS_I2)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IStdDialog to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {506C0A8C-CBC2-4d27-BD67-D4531AB6F5A8}
static const IID IID_IStdDialog =
{ 0x506c0a8c, 0xcbc2, 0x4d27, { 0xbd, 0x67, 0xd4, 0x53, 0x1a, 0xb6, 0xf5, 0xa8 } };

BEGIN_INTERFACE_MAP(CStdDialog, CExOCXDLL)
	INTERFACE_PART(CStdDialog, IID_IStdDialog, Dispatch)
END_INTERFACE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CStdDialog message handlers
//-----------------------------------------------------------------------------
// Author		: ojtaso 	Date : 2005/11/18
// Return		: void
// Paramter
//		LPCTSTR lpSharedName : ����Ű
//		LPCTSTR lpGSharedData : ������
//		BOOL bWithStart : Start���� ����
// Comments		: �������� ����Ǹ� ȣ��Ǵ� �Լ�
//				  ����Ű�� ���� ����� �������� �����Ѵ�.
//				  �ʿ信 ���� CExOCXDLL::OnGSharedDataChange ȣ��� ����� �� �ִ�.
//-----------------------------------------------------------------------------
void CStdDialog::OnGSharedDataChange(LPCTSTR lpSharedName, LPCTSTR lpGSharedData, BOOL bWithStart)
{
	CExOCXDLL::OnGSharedDataChange(lpSharedName, lpGSharedData, bWithStart);
	CString strSharedName = lpSharedName;
	CString strGSharedData = lpGSharedData;
	strGSharedData.TrimRight();
	strSharedName.TrimRight(';');
	if(strGSharedData.IsEmpty()) return;
	/* solomon
	if(strSharedName=="�ð�-�׸���")
	{
		m_ChartMng.SetTimeMark(lpGSharedData, TRUE);
		return;
	}
	*/
}

//-----------------------------------------------------------------------------
// Author		: ojtaso 	Date : 2005/11/18
// Return		: void
// Paramter
//		long dwMainDllObj : MainDll �������̽� (������ ������� �ʴ´�.)
//		long dwBaseDesk : CBaseDesk2 ������ (����ũ�� ������ ��� �ִ�.)
//		long dwRscMng : ���ҽ��Ŵ��� ������ (������ ������� �ʴ´�.)
//		CStringList* pAllProperties : DLL.ocx�� ��� �Ӽ��� ��� �ִ� ������ (���ڿ� �Ľ����� �ʱ������� �����Ѵ�.)
// Comments		: CStdDialog�� �ʿ��� ��� ������ �ʱ�ȭ �Ѵ�.
//				  ���� �ʱ�ȭ �۾��� CExOCXDLL���� �ϰ� �׿� �ٸ� �۾��� �ʿ��ϴٸ� ���⼭ �����Ѵ�.
//				  �ݵ�� CExOCXDLL::Platform_Initialize�� ȣ���Ͽ��� �Ѵ�.
//-----------------------------------------------------------------------------
void CStdDialog::Platform_Initialize(long dwMainDllObj, long dwBaseDesk, long dwRscMng, CStringList* pAllProperties)
{
	CExOCXDLL::Platform_Initialize(dwMainDllObj, dwBaseDesk, dwRscMng, pAllProperties);
}

//-----------------------------------------------------------------------------
// Author		: ojtaso 	Date : 2005/11/18
// Return		: �޼��� ó�� ����
// Paramter
//		UINT message : DLL.ocx���� ���� ������޼���
//		WPARAM wParam : WPARAM
//		LPARAM lParam : LPARAM
//		int &nContinue : nContinue 0 : �޽��� ó����. ���ϵ� ������ DLL.ocx�������� �����ϱ� ����.
//								   1 : ó���� �޽��� ����
//								   2 : �޽��� ó����. ���ϰ� �ǹ� ����. DLL.ocx�������� �޽��� ���ó��.
// Comments		: CWnd::WindowProc�� ȣ���̿ܿ� DLL.ocx���� ó���ϴ� �޼����� �޴´�.
//				  �ݵ�� CExOCXDLL::Platform_WindowProc ȣ���Ͽ��� �Ѵ�.
//-----------------------------------------------------------------------------
LRESULT CStdDialog::Platform_WindowProc(UINT message, WPARAM wParam, LPARAM lParam, int &nContinue)
{	
	LRESULT lResult = CExOCXDLL::Platform_WindowProc(message, wParam, lParam, nContinue);

	return lResult;
}

//-----------------------------------------------------------------------------
// Author		: ojtaso 	Date : 2005/11/18
// Return		: �޼��� ó�� ����
// Paramter
//		MSG* pMsg : �޼��� ����ü
// Comments		: CWnd::PreTranslateMessage�� ȣ���̿ܿ� DLL.ocx���� ó���ϴ� �޼����� �޴´�.
//				  �ݵ�� CExOCXDLL::Platform_PreTranslateMessage ȣ���Ͽ��� �Ѵ�.
//-----------------------------------------------------------------------------
LRESULT CStdDialog::Platform_PreTranslateMessage(MSG* pMsg)
{
	LRESULT lResult = CExOCXDLL::Platform_PreTranslateMessage(pMsg);

	return 0L;
}

//-----------------------------------------------------------------------------
// Author		: ojtaso 	Date : 2005/11/18
// Return		: �޼��� ó�� ����
// Paramter
//		UINT nType : ũ�⺯�� ����
//		int cx : 
//		int cy :
// Comments		: DLL.ocx���� OnSize �޼��� ó���� ȣ���Ѵ�.
//				  CStdDialog�� ȭ��ũ�� ó���� ���⼭ �����Ѵ�.
//				  �ʿ信 ���� CExOCXDLL::Platform_OnSize ȣ��� ����� �� �ִ�.
//-----------------------------------------------------------------------------
void CStdDialog::Platform_OnSize(UINT nType, int cx, int cy)
{
	CExOCXDLL::Platform_OnSize(nType, cx, cy);
}

/****************************************************************************/
/* Function : ��Ƽ ��Ʈ ȭ�� ���̾˷α� �ʱ�ȭ								*/
/* Paramter :																*/
/* Return	:																*/
/* Author	: By DRFN														*/
/* Comments	: FX Margin �ϸ鼭 �ҽ� ���� (�ѱ��Ҹ�ġ �����)				*/
/****************************************************************************/
BOOL CStdDialog::OnInitDialog() 
{
	CExOCXDLL::OnInitDialog();

	AfxEnableControlContainer();
	
	m_strHwndKey.Format("%d", (long)GetSafeHwnd());

	SetInitInfo();												// User ID �� ������� �ʱ�ȭ

	InitImageList();											// �ֿ켱���� ImageList �ε�

	SetBaseInterface();											// PCTR �� �⺻ Interface ����

	m_nChartType = MULTI_CHART;

	CString strScreenType, strMarketType;

	if (m_nScreenID == atoi("8041"))	//@Solomon	8041
	{
		m_nMarketType = CMasterTypeInfo::STOCK_CHART;
		m_bSmallScreenMode = FALSE;
		m_nChartType = MULTI_CHART;
		m_bMultiItemChart = TRUE;
	}
/*	else if (m_nScreenID == atoi(""))	//@Solomon	���ֺ�ƽ ��Ʈ �䱸��.
	{
		m_nMarketType = CMasterTypeInfo::STOCK_CHART;
		m_nChartType = SINGLE_CHART;
		m_bSmallScreenMode = FALSE;
		m_bMultiItemChart = TRUE;
	}
*/
#ifdef _WRITELOG
	CreateLogFile();											// Log - ojtaso (20080107)
#endif

#ifdef _WRITELOG_SISE
	CreateLogFile();											// Log - ojtaso (20080107)
#endif

	IPartnerSvrManager* pPtSvrMng = (IPartnerSvrManager*)AfxGetPctrInterface(UUID_IPartnerSvrManager);
	if (pPtSvrMng)
		g_pPtUtilMng = (IPartnerUtilManager*)pPtSvrMng->GetInerInterface(LUID_IPartnerUtilManager);

	// ��Ʈ �� �ε�
	FormLoad();

	if(m_nHeightOfDlg && m_nWidthOfDlg)							// ?
		SetWindowPos(&CWnd::wndTop, 0, 0, m_nWidthOfDlg, m_nHeightOfDlg, SWP_SHOWWINDOW);			

	GetOrderInfo();

	if (m_pDrdsLib == NULL)
	{
		//@SolomonGNF IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager, 3);
		IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
		if (pDrdsMng)
			m_pDrdsLib = pDrdsMng->AdviseDrdsLib();
	}

	if (m_pDrdsLib)
	{
//@SolomonGNF 		m_pDrdsLib->DrdsAdvise(0, "TIME", "", "01", m_hWnd);
	}

	SetTimer(TIMER_EVENTID_FORMLOAD, 1, NULL);

	return TRUE;
}

void CStdDialog::ChangeStockMode(int type)
{
	if(m_nChartType == MINI_MULTI_CHART || m_nChartType == MINI_SINGLE_CHART)
	{
		ChangeMiniStockMode(type);
		return;
	}
	CString strTemp;
	//���� Lib UnLoad
	UnLoad_ReplayChart_Lib();	

	m_nMarketType = type;

	//int nPreChartGubun = 
//	if(m_pLeftBarDlg)
//	{
//		m_pLeftBarDlg->SetMarketType(m_nMarketType);				
//		m_pLeftBarDlg->MoveWindow(GetRect(ID_LEFTBAR,TRUE));
//	}
	//------------------------------------------------------------------------------

	if(m_pInputBarDlg)
	{
		m_pInputBarDlg->SetMarketType(m_nMarketType);	

		m_pInputBarDlg->MoveWindow(GetRect(ID_INPUTBAR,TRUE));
		//m_pInputBarDlg->SetMarKetToolType(m_nMarketType);//2005. 05. 16 - chart gubun ����A��

		//2005. 06. 22 ���� �ڵ带 ���� ���� ���� �����Ѵ�.
		BOOL bRequestAPData = FALSE;
		if(m_pInputBarDlg->GetSafeHwnd())
		{
//@��������(zz)
//			m_pInputBarDlg->InitBaseInfo(NULL, bRequestAPData);
//@��������
		}
	}

	//------------------------------------------------------------------------------
	// 2005. 05. 16 chart �����ڸ� �ѱ��.
	//------------------------------------------------------------------------------
	if(m_pExtInputBarDlg)
	{
		m_pExtInputBarDlg->SetMarketType(m_nMarketType);
		m_pExtInputBarDlg->MoveWindow(GetRect(ID_EXTINPUTBAR,TRUE));
	}
	//------------------------------------------------------------------------------
	// 2005. 05. 16 chart �����ڸ� �ѱ��.
	//------------------------------------------------------------------------------	
	m_ChartMng.SetMarketType(m_nMarketType);
//@��������
//	m_pInputBarDlg->SetMarKetToolType(m_nMarketType);//2005. 05. 16 - chart gubun ����
//@��������
//@�����߰�
	m_pInputBarDlg->ChangeMarket(m_nMarketType, FALSE, TRUE);
//@�����߰�
	//------------------------------------------------------------------------------
	// 2005. 09. 01 Tab�� ������ ���õ� �׸��� ���δ�.
	// ���� �ٲ��� �ʴ´�.
	//------------------------------------------------------------------------------
	//m_pLeftBarDlg->SetConfig();

//@��������
//	if(m_hCursor == NULL)
//	{
//		if(m_pLeftBarDlg)
//		{
//			if(m_pLeftBarDlg->m_nMarketType != m_nMarketType)
//			{
//				m_pLeftBarDlg->SetMarketType(m_nMarketType);				
//				m_pLeftBarDlg->MoveWindow(GetRect(ID_LEFTBAR,TRUE));
//				if(m_pLeftBarDlg->m_tabctrl.GetCurSel() == 0)
//					m_pLeftBarDlg->m_tabctrl.SetCurSel(0);
//			}
//		}
//	}
//@��������
	//------------------------------------------------------------------------------


//	if( m_ChartMng.GetpChartItemSelected() != NULL)
//		m_ChartMng.GetpChartItemSelected()->LoadTickMinDayInfo(m_strConfig);

	//SetLeftBarType(m_nLeftBarType);
	//SetShowExtInputBarDlg(m_bShowExtInputBarDlg);
}

void CStdDialog::ChangeMiniStockMode(int type)
{
	m_ChartMng.SaveChart(m_strConfig);
	//===========================================================================
	AllCodeUnAdvise(); //Real ���� ����..
	//===========================================================================
	
	m_ChartMng.SendMessage(UMSG_RESETOCX);
	m_ChartMng.SendMessage(UMSG_SETMATRIX,0,0);

	m_nMarketType =  type;

//	m_ChartMng.m_bLastEnvUpdated = FALSE;
	//====================================================
#ifdef	_LASTINFO_SAVE
	m_strConfig = m_strUserDir + "\\" + m_strHwndKey + "_" + m_strScreenNo + "_" + ENVINFOFILE;
#else
	m_strConfig = m_strUserDir + "\\" + m_strScreenNo + "_" + ENVINFOFILE;
#endif

	if(m_pInputBarDlg)
	{
		m_pInputBarDlg->m_strCfgFile = m_strConfig;

		m_pInputBarDlg->SetMarketType(m_nMarketType);	
		m_pInputBarDlg->MoveWindow(GetRect(ID_INPUTBAR,TRUE));

//@��������
//		m_pInputBarDlg->SetMarKetToolType(m_nMarketType);//2005. 05. 16 - chart gubun ����
//@��������

//@��������
//		//2005. 06. 22 ���� �ڵ带 ���� ���� ���� �����Ѵ�.
//		BOOL bRequestAPData = FALSE;
//		if(m_pInputBarDlg->GetSafeHwnd())
//		{
//			m_pInputBarDlg->InitBaseInfo(NULL, bRequestAPData);
//		}
//@��������
	}


	//------------------------------------------------------------------------------
	// 2005. 05. 16 chart �����ڸ� �ѱ��.
	//------------------------------------------------------------------------------	
	m_ChartMng.SetMarketType(m_nMarketType);
//@�����߰�
	m_pInputBarDlg->ChangeMarket(m_nMarketType, FALSE, TRUE);
//@�����߰�
//@��������
//	if( m_ChartMng.GetpChartItemSelected() != NULL)
//		m_ChartMng.GetpChartItemSelected()->LoadTickMinDayInfo(m_strConfig);
//@��������

//@��������
//	//{{2007.07.23 by LYH
//	if(m_pInputBarDlg)
//		m_pInputBarDlg->SetRecentCode();
//	//}}
//@��������
//	PostMessage(UMSG_EDITCODE_INIT, 0L, 0L);
}
/****************************************************************************/
/* Function : ��Ƽ ��Ʈ �� �ε�												*/
/* Paramter : ?																*/
/* Return	:																*/
/* Author	: By DRFN														*/
/* Comments	: FX Margin �ϸ鼭 �ҽ� ���� (�ѱ��Ҹ�ġ �����)				*/
/* Date     : 2008/07/10                                                    */
/****************************************************************************/
void CStdDialog::FormLoad()
{
	m_pMsgBar = new CMessageBar;								// �޼����� ����
	m_pMsgBar->Create(CMessageBar::IDD,this);

// --> [Edit]  ������ 2008/10/29
//@Sol100618	m_pCommMsg = new CRequestMsg;								// �����ȸ �޼����� ����
//@Sol100618	m_pCommMsg->Create(CRequestMsg::IDD,this);
// <-- [Edit]  ������ 2008/10/29

	m_TRComm = (ITrComm2*)GetTrComm();							// TR �� DRDS �ʱ�ȭ

	InitDrds();													// ��� Ű �ʱ�ȭ

	m_FavoriteMng.SetPath(m_strUserDir);						// ����� ȯ�� path������
	
	if (_access(m_strUserDir, 0) == -1)							// User Directory ����
		 int retval = _mkdir(m_strUserDir);
/*
#ifdef	_LASTINFO_SAVE
	m_strConfig = m_strUserDir + "\\" + m_strHwndKey + "_" + m_strScreenNo + "_" + ENVINFOFILE;

	CString strTemp = "", strSaveKey = "";
	CString strName = m_strConfig;
	CString strScreenNo = m_strScreenNo;

	if ( m_strScreenKey.IsEmpty() )
	{
		strName.Replace(strScreenNo, "Che_" + strScreenNo);
		strName.Replace(".dat", ".mct");
		FileCopyAndSaveAndDelete(DEFAULT_HWND_COPY);
	}
	else
	{
		strName.Replace(strScreenNo, "Che_" + strScreenNo);
		strName.Replace(".dat", ".mct");
		FileCopyAndSaveAndDelete(HWND_LAST_COPY);
	}
#else																// �������� ���
	m_strConfig = m_strUserDir + "\\" + m_strScreenNo + "_" + ENVINFOFILE;
#endif
*/
	m_strConfig = m_strUserDir + "\\" + m_strScreenNo + "_" + ENVINFOFILE;

	char szTemp[64];
	if (m_nChartType == MULTI_CHART)
	{
		m_nWidthOfLeftBar = WIDTH_SIDEBAR_EXTEND;

		if (m_nWidthOfLeftBar)
		{
			::GetPrivateProfileString("Frame","ShowLeftBar","1",szTemp,sizeof(szTemp),m_strConfig);
			m_nLeftBarType = atoi(szTemp);
		}
		else
			m_nWidthOfLeftBar = WIDTH_SIDEBAR_COLLAPSE;

		::GetPrivateProfileString("Frame","ShowExtInputBarDlg","1",szTemp,sizeof(szTemp),m_strConfig);
		m_bShowExtInputBarDlg = atoi(szTemp);

		::GetPrivateProfileString("Frame","ShowToolBar", "1", szTemp, sizeof(szTemp),m_strConfig);
		m_bShowToolBar = atoi(szTemp);

		::GetPrivateProfileString("Frame","ShowMigyulche", "1", szTemp, sizeof(szTemp),m_strConfig);
		m_bMigyulcheDraw = atoi(szTemp);

	}
	else if(m_nChartType == SINGLE_CHART)
	{
		m_nWidthOfLeftBar = WIDTH_SIDEBAR_EXTEND;

		if(m_nWidthOfLeftBar)
		{
			::GetPrivateProfileString("Frame","ShowLeftBar","0",szTemp,sizeof(szTemp),m_strConfig);
			m_nLeftBarType = atoi(szTemp);

			::GetPrivateProfileString("Frame","ShowToolBar", "1", szTemp, sizeof(szTemp),m_strConfig);
			m_bShowToolBar = atoi(szTemp);
		}
		else
		{
			m_nWidthOfLeftBar = WIDTH_SIDEBAR_COLLAPSE;
		}
		m_bShowExtInputBarDlg = 0;
	}
	else
	{
		m_nWidthOfLeftBar = WIDTH_SIDEBAR_EXTEND;
		m_nLeftBarType = 0;
		m_bShowExtInputBarDlg = 0;

		::GetPrivateProfileString("Frame","ShowToolBar", "1", szTemp, sizeof(szTemp),m_strConfig);
		m_bShowToolBar = atoi(szTemp);
	}

	switch(m_nChartType)
	{
	case MINI_MULTI_CHART:
	case MINI_SINGLE_CHART:
		{
			m_pInputBarDlg = new CMiniBarDlg;
			if(m_pInputBarDlg)
			{
				m_pInputBarDlg->SetMarketType(m_nMarketType);
				m_pInputBarDlg->SetpChartMng(&m_ChartMng);
				m_pInputBarDlg->SetBaseDesk((long)m_pBaseDesk);
				m_pInputBarDlg->Create(IDD_MINIBAR,this);
				m_ChartMng.SetpInputBarDlg(m_pInputBarDlg);
				m_pInputBarDlg->MoveWindow(GetRect(IDD_MINIBAR,TRUE));
				m_pInputBarDlg->ShowWindow(SW_SHOW);
				m_pInputBarDlg->ChangeMarket(m_nMarketType, FALSE, TRUE);

				//�м����� ����
				m_pDlgToolbar = new CDlgToolbar(this);
				if(m_pDlgToolbar)
				{
					m_pDlgToolbar->SetChartMng(&m_ChartMng);
					m_pDlgToolbar->Create(IDD_DLG_TOOLBAR,this);
					m_pDlgToolbar->MoveWindow(GetRect(ID_TOOLBAR,TRUE));
					m_pDlgToolbar->ShowWindow(SW_SHOW);

					m_btnExtTool.FP_vSetImage(&m_ImgList_W8H25, 2, 3);
					m_btnExtTool.FP_vSetToolTipText("�������");
					m_btnExtTool.ShowWindow(SW_SHOW);
				}
			}
		}
		break;
	case MULTI_CHART:
	case SINGLE_CHART:
		{
			m_btnSideBar.SetBorderColor(RGB(157,157,161));
			m_btnSideBar.SetBackGroundColor(GetBkgrndColor(), GetBkgrndColor());

			// ��ܹ� ����
			m_pInputBarDlg = NULL;
			m_pInputBarDlg = new CInputBarDlg();
			if(m_pInputBarDlg)
			{
				if(m_bMultiItemChart == FALSE)
					m_pInputBarDlg->SetReplayChartMode(FALSE);

				m_pInputBarDlg->SetMarketType(m_nMarketType);				
				m_pInputBarDlg->SetpChartMng(&m_ChartMng);
				m_pInputBarDlg->Create(IDD_INPUTBAR,this);
				m_ChartMng.SetpInputBarDlg(m_pInputBarDlg);
				m_pInputBarDlg->MoveWindow(GetRect(ID_INPUTBAR,TRUE));
				m_pInputBarDlg->ShowWindow(SW_SHOW);
				m_pInputBarDlg->ChangeMarket(m_nMarketType, FALSE, TRUE);

				m_pInputBarDlg->SetAllPosition(m_nChartType);

				//�м����� ����
				m_pDlgToolbar = new CDlgToolbar(this);
				if(m_pDlgToolbar)
				{
					m_pDlgToolbar->SetChartMng(&m_ChartMng);
					m_pDlgToolbar->Create(IDD_DLG_TOOLBAR,this);
					m_pDlgToolbar->MoveWindow(GetRect(ID_TOOLBAR,TRUE));
					m_pDlgToolbar->ShowWindow(SW_SHOW);

					m_btnExtTool.FP_vSetImage(&m_ImgList_W8H25, 2, 3);
					m_btnExtTool.FP_vSetToolTipText("�������");
					m_btnExtTool.ShowWindow(SW_SHOW);
				}
			}

			// ���̵�� ���� ����
			
			// ������ ����
			m_pLeftBarDlg = NULL;
			m_pLeftBarDlg = new CLeftBarDlg();
			if(m_pLeftBarDlg)
			{
				m_pLeftBarDlg->SetMarketType(m_nMarketType);				
				m_pLeftBarDlg->SetStdDialog(this);
				m_pLeftBarDlg->SetpChartMng(&m_ChartMng);
				m_pLeftBarDlg->SetpFavoriteThingsMng(&m_FavoriteMng);
				m_pLeftBarDlg->Create(IDD_LEFTBAR,this);
				m_btnSideBar.SetSideBar(m_pLeftBarDlg);
				m_pLeftBarDlg->MoveWindow(GetRect(ID_LEFTBAR,TRUE));
				m_pLeftBarDlg->ShowWindow(SW_SHOW);
				m_ChartMng.SetpLeftBarDlg(m_pLeftBarDlg);
			}
		}

		if(m_pInputBarDlg)
			GetIndicatorNameListIntoComboBox(m_pInputBarDlg->m_cmbChartType, "Ư����ǥ");
	}

	if(m_pInputBarDlg)
		SetPermanentHandle(m_pInputBarDlg);
	SetPermanentHandle(m_pLeftBarDlg);

	// ������ܹ� ����
	m_pExtInputBarDlg = NULL;

// --> [Edit]  ������ 2008/11/09
	if ( m_nChartType != MULTI_SPREAD_CHART )
	{
		m_pExtInputBarDlg = new CExtInputBarDlg;
		if(m_pExtInputBarDlg)
		{
			m_pExtInputBarDlg->m_nMarketType = m_nMarketType;				

			m_pExtInputBarDlg->SetpChartMng(&m_ChartMng);
			m_pExtInputBarDlg->SetpLeftBar(m_pLeftBarDlg);
			m_pExtInputBarDlg->Create(IDD_EXTINPUTBAR,this);
			m_ChartMng.SetpExtInputBarDlg(m_pExtInputBarDlg);	
			m_pExtInputBarDlg->SetMarketType(m_nMarketType);
			m_pExtInputBarDlg->MoveWindow(GetRect(ID_EXTINPUTBAR,TRUE));
			m_pExtInputBarDlg->ShowWindow(SW_SHOW);
			if(m_pInputBarDlg->GetType() == DAY_DATA_CHART || 
				m_pInputBarDlg->GetType() == WEEK_DATA_CHART || 
				m_pInputBarDlg->GetType() == MONTH_DATA_CHART)
			EnableQueryOpt(FALSE);
		}
	}
// <-- [Edit]  ������ 2008/11/09

	// ���ø��� ����
	m_xSplitter1.CreateInWnd(this, ID_SPLITTER, GetRect(ID_SPLITTER));

	// ��Ʈ�Ŵ��� ����
	m_ChartMng.SetMarketType(m_nMarketType);
// --> [Edit]  ������ 2008/08/26	( Root�н��� ���� �˷��ش�. )
	m_ChartMng.m_strRootPath = m_strRootPath;
// <-- [Edit]  ������ 2008/08/26
	BOOL bRetValue = m_ChartMng.Create(GetRect(ID_CHART),this, m_strUserDir, m_szImageDir,ID_CHART, (long)((CMChartInterface*)this));
	if(!bRetValue) return;
	m_ChartMng.SetBaseDesk((long)1);            // ����ũ�� ���������� ���� ������ ���������� �⺻���� ����

	if((m_nChartType != MINI_MULTI_CHART && m_nChartType != MINI_SINGLE_CHART) && m_pLeftBarDlg)
		m_pLeftBarDlg->SetConfig();

	if( m_ChartMng.GetpChartItemSelected() != NULL)
		m_ChartMng.GetpChartItemSelected()->LoadTickMinDayInfo(m_strConfig);
#ifdef _DEBUG
	else
	{
		AfxMessageBox("FormLoad : m_ChartMng.GetpChartItemSelected() is NULL !!");
//		return;
	}
#endif 

	// (����)��ܹ� Ÿ�� �� Visible ����
	SetLeftBarType(m_nLeftBarType);
	SetShowExtInputBarDlg(m_bShowExtInputBarDlg);

	BOOL bRequestAPData = TRUE;

	// �����ڵ� ��Ʈ�� ����
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->InitBaseInfo(NULL, bRequestAPData);

	// ��Ų����
//@SolomonGNF IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 3);
//@SolomonGNF 	if(pKSLibMng)
//@SolomonGNF 		m_clrBackground = pKSLibMng->Lib_KSLink_GetColor(1);	// ����ȭ�� ���
//@SolomonGNF 	else
//@SolomonGNF 		m_clrBackground = DEFAULT_BKGRND_COLOR;		// �⺻��

	m_clrBackground = DEFAULT_BKGRND_COLOR;		// �⺻��

	// 	// ��Ų����
	int nTheme = 0;//AfxGetMainWnd()->SendMessage(WMU_GET_THEME);	
	OnChangeSkinColor(nTheme, 0);
//	ChangeSkinColor();
	Invalidate();

	//�޷�/�� ������ ��û
//20080714
//	RequestDollarData();
}

LONG CStdDialog::OnWmSplitterMoved( UINT wParam, LONG lParam )
{

	if(ID_SPLITTER == lParam)
	{		
		if(m_nLeftBarType == 1)		
		{
			m_nWidthOfLeftBar += wParam;
		}
		else if(m_nLeftBarType == 2)	
		{
			m_nWidthOfLeftBar -= wParam;
		}
	}

	if(	(m_bShowDataListWnd				) &&
		(m_pDataWindow != NULL			) &&
		(m_pDataWindow->GetSafeHwnd()	) )
	{
		if(m_ChartMng.GetSafeHwnd())
		{	
			CRect rect;
			m_ChartMng.GetWindowRect(rect);	
			ScreenToClient(rect);
			m_ViewRect.left = rect.left;
			m_pDataWindow->MoveWindow(m_ViewRect);	
		}
	}

	m_pLeftBarDlg->Invalidate();
	m_ChartMng.Invalidate();
	m_xSplitter1.Invalidate();
	//m_pInputBarDlg->Invalidate();
	m_pInputBarDlg->ShowWindow(SW_HIDE);
	m_pExtInputBarDlg->Invalidate();
	m_pInputBarDlg->ShowWindow(SW_SHOW);


	if(m_ChartMng.m_pFloatingDlg != NULL)				
		m_ChartMng.m_pFloatingDlg->Invalidate();

//	if(m_pInputBarDlg)
//		m_pInputBarDlg->SendMessage(UMSG_EDIT_FOCUS);
	
    return 1L;
}

CRect CStdDialog::GetRect(UINT nID, BOOL bInit /*=FALSE*/)
{
	CRect rect;
	GetClientRect(rect);

	if(m_bShowToolBar)
		rect.right -= WIDTH_TOOLBAR;

	if(!m_nLeftBarType && m_nChartType != MINI_MULTI_CHART && m_nChartType != MINI_SINGLE_CHART)
		rect.left = WIDTH_SIDEBAR;

	switch(nID)
	{
	case IDC_SIDEBAR:
		{
			if(!m_nLeftBarType && m_nChartType != MINI_MULTI_CHART && m_nChartType != MINI_SINGLE_CHART)
			{
				rect.left = 0;
				rect.right = WIDTH_SIDEBAR;
			}
			else
			{
				rect.SetRectEmpty();
			}
			return rect;
		}
		break;
	case ID_LEFTBAR:
		if(!m_nLeftBarType)	
		{
			rect.SetRectEmpty();		
			return rect;
		}
		if(m_pLeftBarDlg->GetSafeHwnd()&&!bInit)
		{
			CRect rectLeftBar;
			m_pLeftBarDlg->GetWindowRect(rectLeftBar);
			ScreenToClient(rectLeftBar);
			if(m_nLeftBarType==1)		rect.right = rectLeftBar.right;
			else if(m_nLeftBarType==2)	rect.left = rect.right - rectLeftBar.Width();
		}
		else	// initial
		{
			if(m_nLeftBarType==1)		rect.right = rect.left + m_nWidthOfLeftBar;
			else if(m_nLeftBarType==2)	rect.left = rect.right - m_nWidthOfLeftBar;
		}		
//		if(m_pLeftBarDlg->GetSafeHwnd()&&!bInit)
//		{
//			CRect rectLeftBar;
//			m_pLeftBarDlg->GetWindowRect(rectLeftBar);
//			ScreenToClient(rectLeftBar);
//			if(m_nLeftBarType==1)
//			{
//				rect.left = WIDTH_SIDEBAR;
//				rect.right = rectLeftBar.right;
//			}
//			else if(m_nLeftBarType==2)
//			{
//				rect.right -= WIDTH_SIDEBAR;
//				rect.left = rect.right - rectLeftBar.Width();
//			}
//		}
//		else	// initial
//		{
//			if(m_nLeftBarType==1)
//			{
//				rect.left = WIDTH_SIDEBAR;
//				rect.right = rect.left + m_nWidthOfLeftBar;
//			}
//			else if(m_nLeftBarType==2)
//			{
//				rect.right -= WIDTH_SIDEBAR;
//				rect.left = rect.right - m_nWidthOfLeftBar;
//			}
//		}		
		break;
	case ID_INPUTBAR:
	case ID_MINIBAR:
		if(!m_bShowInputBarDlg)	
		{
//			rect.left = WIDTH_SIDEBAR;
			rect.bottom = rect.top;
			return rect;
		}
		if(m_pInputBarDlg && m_pInputBarDlg->IsMiniBar())
		{
			rect.bottom = rect.top + HEIGHTOFINPUTBAR;
			if(m_nLeftBarType==1)		rect.left = rect.left + m_nWidthOfLeftBar + WIDTHOFSPLITTER;
			else if(m_nLeftBarType==2)	rect.right = rect.right - (m_nWidthOfLeftBar + WIDTHOFSPLITTER);			

			if(m_pDlgToolbar)
				rect.right = rect.right - 8;
		}
		else
		{
			if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd()&&!bInit)
			{
				CRect rectInputBar;
				CRect rectLeftBar;
				if(m_pLeftBarDlg)
				{
					m_pLeftBarDlg->GetWindowRect(rectLeftBar);
				}
				
				if(m_pInputBarDlg)
				{
					m_pInputBarDlg->GetWindowRect(rectInputBar);
					ScreenToClient(rectInputBar);
					if(m_nLeftBarType==1)		rect.left = rectInputBar.left;
					else if(m_nLeftBarType==2)	rect.right = rect.right - rectLeftBar.Width() - WIDTHOFSPLITTER;
				}
			}
			else
			{			
				if(m_nLeftBarType==1)		rect.left = rect.left + m_nWidthOfLeftBar + WIDTHOFSPLITTER;
				else if(m_nLeftBarType==2)	rect.right = rect.right - (m_nWidthOfLeftBar + WIDTHOFSPLITTER);			
			}	
			if(m_bShowExtInputBarDlg)
				rect.bottom = rect.top + HEIGHTOFINPUTBAR + 1;
			else
				rect.bottom = rect.top + HEIGHTOFINPUTBAR;

			if(m_pDlgToolbar)
				rect.right = rect.right - 8;
		}
		break;
	case ID_EXTINPUTBAR:
		if(!m_bShowExtInputBarDlg)	
		{
			rect.bottom = rect.top;
			return rect;
		}
		if(m_pExtInputBarDlg->GetSafeHwnd()&&!bInit)
		{
			CRect rectInputBar;
			CRect rectLeftBar;
			if(m_pLeftBarDlg)
			{
				m_pLeftBarDlg->GetWindowRect(rectLeftBar);
			}		

			if(m_pInputBarDlg)
			{
				m_pInputBarDlg->GetWindowRect(rectInputBar);
				ScreenToClient(rectInputBar);
				if(m_nLeftBarType==1)		rect.left = rectInputBar.left;
				else if(m_nLeftBarType==2)	rect.right = rect.right - rectLeftBar.Width() - WIDTHOFSPLITTER;
			}
		}
		else
		{			
			if(m_nLeftBarType==1)		rect.left = rect.left + m_nWidthOfLeftBar + WIDTHOFSPLITTER;
			else if(m_nLeftBarType==2)	rect.right = rect.right - (m_nWidthOfLeftBar +WIDTHOFSPLITTER);			
		}		
		rect.top = rect.top + HEIGHTOFINPUTBAR + 1;
		rect.bottom = rect.top + HEIGHTOFINPUTBAR - 1;
		break;
	case ID_SPLITTER:
		if(!m_nLeftBarType||!m_pLeftBarDlg)	
		{
			rect.SetRectEmpty();		
			return rect;
		}
		if(m_xSplitter1.GetSafeHwnd()&&!bInit)
		{
			CRect rectSplitter1;
			CRect rectLeftBar;
			m_pLeftBarDlg->GetWindowRect(rectLeftBar);
			m_xSplitter1.GetWindowRect(rectSplitter1);
			ScreenToClient(rectSplitter1);
			if(m_nLeftBarType==1)
			{
				rect.left = rectSplitter1.left;
				rect.right = rectSplitter1.right;			
			}
			else if(m_nLeftBarType==2)
			{
				rect.right = rect.right - rectLeftBar.Width();
				rect.left = rect.right - WIDTHOFSPLITTER;							
			}
		}
		else
		{
			if(m_nLeftBarType==1)
			{
				rect.left = rect.left + m_nWidthOfLeftBar;
				rect.right = rect.left + WIDTHOFSPLITTER;
			}
			else if(m_nLeftBarType==2)
			{
				rect.right = rect.right - m_nWidthOfLeftBar;
				rect.left = rect.right - WIDTHOFSPLITTER;				
			}
		}
		break;
	case ID_CHART:
		if(m_ChartMng.GetSafeHwnd()&&!bInit)
		{
			CRect rectChart;
			CRect rectLeftBar;
			m_ChartMng.GetWindowRect(rectChart);			
			if(m_pLeftBarDlg)
			{
				m_pLeftBarDlg->GetWindowRect(rectLeftBar);
			}
			
			ScreenToClient(rectChart);
			if(m_nLeftBarType==1)		rect.left = rectChart.left;
			else if(m_nLeftBarType==2)	rect.right = rect.right - rectLeftBar.Width() - 5;
		}
		else
		{
			if(m_pLeftBarDlg)
			{
				if(m_nLeftBarType==1)
				{
					rect.left = rect.left + (m_nWidthOfLeftBar + WIDTHOFSPLITTER);
				}
				else if(m_nLeftBarType==2)
				{
					rect.right = rect.right - (m_nWidthOfLeftBar + WIDTHOFSPLITTER);
				}			
			}
		}				
		if(m_bShowExtInputBarDlg)
		{
			rect.top = rect.top + HEIGHTOFINPUTBAR*2;
		}
		else
		{
			if(m_bShowInputBarDlg)
			{
				if(m_pInputBarDlg && m_pInputBarDlg->IsMiniBar())
				{
					rect.top = rect.top + HEIGHTOFINPUTBAR;
				}
				else
				{
					rect.top = rect.top + HEIGHTOFINPUTBAR;
				}
			}
		}

		if(m_bShowDataListWnd)
		{
			m_ViewRect.left		= 0;
			m_ViewRect.top		= 0;
			m_ViewRect.right	= 0;
			m_ViewRect.bottom	= 0;

			m_ViewRect.left		= rect.left;		//ChartMng
			m_ViewRect.right	= rect.right;	//ChartMng
			m_ViewRect.bottom	= rect.bottom;		
			m_ViewRect.top		= rect.bottom = rect.bottom - 90;
			if(m_ChartMng.GetSafeHwnd())
			{
				m_ChartMng.MoveWindow(rect);

			//	m_ChartMng.Invalidate();
			//	if(m_ChartMng.m_pFloatingDlg)
			//	m_ChartMng.m_pFloatingDlg->SendMessage(WM_LBUTTONUP, 0, 0);
			}

			if(	(m_pDataWindow != NULL			) &&
				(m_pDataWindow->GetSafeHwnd()	) 
			)
			{
				m_pDataWindow->MoveWindow(m_ViewRect);
			}
		}
		else
		{
			m_ViewRect.left		= 0;
			m_ViewRect.top		= 0;
			m_ViewRect.right	= 0;
			m_ViewRect.bottom	= 0;
			
			if(m_ChartMng.GetSafeHwnd())
			{
				CRect rectChartMng;
				m_ChartMng.GetWindowRect(rectChartMng);
				ScreenToClient(rectChartMng);
				rectChartMng.bottom = rect.bottom;
				m_ChartMng.MoveWindow(rectChartMng);

				//m_ChartMng.Invalidate();
				//if(m_ChartMng.m_pFloatingDlg)
				//	m_ChartMng.m_pFloatingDlg->SendMessage(WM_LBUTTONUP, 0, 0);
			}
		}

		
	
		break;
	case ID_TOOLBAR:
		if(m_pDlgToolbar->GetSafeHwnd())
		{
			//if(m_nLeftBarType==1)
			//{
				rect.left = rect.right;
				rect.right = rect.right + WIDTH_TOOLBAR;
			//}
		}
		break;
	default:
		break;
	}
	return rect;
}


void CStdDialog::Resize()
{	
	CRect rect;	
	CRect rectNew;	
	
	if(m_pLeftBarDlg)
	{
		if(m_bHideChart)
		{
			CRect rect;
			GetClientRect(rect);
			rect.DeflateRect(BOUND_GAP,BOUND_GAP,BOUND_GAP,BOUND_GAP);
			m_pLeftBarDlg->MoveWindow(rect);
			m_nWidthOfLeftBar = rect.Width();
		}
		else
		{
			m_pLeftBarDlg->GetWindowRect(rect);
			ScreenToClient(rect);
			rectNew = GetRect(ID_LEFTBAR,m_bInitResize);
			if(rect!=rectNew)
			{
				m_pLeftBarDlg->MoveWindow(rectNew);
			}

			if(m_btnSideBar.GetSafeHwnd())
			{
				m_btnSideBar.GetWindowRect(rect);
				ScreenToClient(rect);
				rectNew = GetRect(IDC_SIDEBAR, m_bInitResize);
				if(rect!=rectNew)
				{
					m_btnSideBar.MoveWindow(rectNew);
				}
			}
		}

		//m_pLeftBarDlg->Invalidate();
	}

	if(m_xSplitter1.m_hWnd != NULL )
	{
		m_xSplitter1.GetWindowRect(rect);
		ScreenToClient(rect);
		rectNew = GetRect(ID_SPLITTER,m_bInitResize);
		if(rect!=rectNew)	
		{		
			m_xSplitter1.MoveWindow(rectNew);
			//m_xSplitter1.ShowWindow(SW_SHOW);
		}
	}

	if(m_pInputBarDlg)
	{
		m_pInputBarDlg->GetWindowRect(rect);
		ScreenToClient(rect);
		rectNew = GetRect(ID_INPUTBAR,m_bInitResize);
		if(rect!=rectNew)	
		{
			m_pInputBarDlg->MoveWindow(rectNew);
			//m_pInputBarDlg->ShowWindow(SW_SHOW);
		}

/*@sini
		if(m_pInputBarDlg->m_ctrlCodeInput.GetSafeHwnd())
		{
			m_pInputBarDlg->m_ctrlCodeInput.SetWindowPos(NULL, 
				m_pInputBarDlg->m_rectCmb.left, m_pInputBarDlg->m_rectCmb.top,
				m_pInputBarDlg->m_rectCmb.Width(), m_pInputBarDlg->m_rectCmb.Height(),
				SWP_NOZORDER);

			if(m_nChartType != MINI_MULTI_CHART && m_nChartType != MINI_SINGLE_CHART)
			{
				if(m_pInputBarDlg->m_hKSJongName && IsWindow(m_pInputBarDlg->m_hKSJongName))
				{
					CRect rcJN = m_pInputBarDlg->m_rectJongName;
					CWnd::FromHandle(m_pInputBarDlg->m_hKSJongName)->SetWindowPos(NULL, 
						rcJN.left, rcJN.top, rcJN.Width(), rcJN.Height(),
						SWP_NOZORDER);
				}
			}
		}
*/
		//m_pInputBarDlg->Invalidate();
	}
	
	if(m_pExtInputBarDlg)
	{
		m_pExtInputBarDlg->GetWindowRect(rect);
		ScreenToClient(rect);
		rectNew = GetRect(ID_EXTINPUTBAR,m_bInitResize);
		if(rect!=rectNew)	
		{
			m_pExtInputBarDlg->MoveWindow(rectNew);
			//m_pExtInputBarDlg->ShowWindow(SW_SHOW);
		}

		//m_pExtInputBarDlg->Invalidate();
	}

	if(m_ChartMng.GetSafeHwnd())
	{
		m_ChartMng.GetWindowRect(rect);	
		ScreenToClient(rect);
		rectNew = GetRect(ID_CHART,m_bInitResize);
		if(rect!=rectNew)	
		{	
			m_ChartMng.MoveWindow(rectNew);
		}
	}

	if(m_pDlgToolbar)
	{
		m_pDlgToolbar->GetWindowRect(rect);
		ScreenToClient(rect);
		rectNew = GetRect(ID_TOOLBAR,m_bInitResize);
		if(rect!=rectNew)	
		{
			m_pDlgToolbar->MoveWindow(rectNew);

			rectNew.right = rectNew.left;
			rectNew.top += 2;
			rectNew.left = rectNew.right - 8;
			rectNew.bottom = rectNew.top + 20;
			m_btnExtTool.MoveWindow(rectNew);
		}
		m_btnExtTool.Invalidate();
	}

	if(m_bOneClickHide)
	{		
		//20080914 �̹��� >>
		//��Ŭ�� ����� ���� �� ������¡�� ����� ��Ʈ�ѵ��� �ٽ� ���̴� ���׶����� ����
		//false�� �����Ͽ� �ٽ� ��� ���̰� �ȴ�.
		//false�� m_bOneClickHide�� ���� ����
		//SetOneClickHide(FALSE);
		//m_pDlgToolbar->SetToggleButton(50066, 0);
		SetOneClickHide(m_bOneClickHide);
		m_pDlgToolbar->SetToggleButton(50066, m_bOneClickHide);
		//20080914 �̹��� <<
	}
	m_bInitResize = TRUE;
}

void CStdDialog::OnDestroy() 
{	
	if ( m_pDrdsLib )
	{
		// [Edit] 2009.05.27 ������
		// ���� ȭ���� ��� ���¿��� ȭ���� ���� �� TIME �ǽð��� �����ϴ� ��찡 �߻��Ͽ�
		// �Ʒ��� DrdsUnadviseAll()���� �� ���� �����ϰ� ����
		//m_pDrdsLib->DrdsUnadvise(STOCK_30SEC_PID, "TIME", "", "01", m_hWnd);
//@SolomonGNF 		m_pDrdsLib->DrdsUnadvise(MIGYULJE_PID, "A", "", "MIGYULCHE", m_hWnd);
	}

	CString strTemp;
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("������ ��ġ ����::Begin", _T("CStdDialog"), 10);
#endif
	strTemp.Format("%d",m_bMigyulcheDraw);
	::WritePrivateProfileString("Frame","ShowMigyulche", strTemp,m_strConfig);

	CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
	if( pChartItem) if( pChartItem->GetCheHogaChange() != 1 )
		SaveChart(FALSE);

// <-- [Edit]  ������ 2008/10/30
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("������ ��ġ ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("��Ʈ�Ŵ��� ����::Begin", _T("CStdDialog"), 10);
#endif
	if(m_ChartMng.GetSafeHwnd())
		m_ChartMng.DestroyWindow();
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("��Ʈ�Ŵ��� ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("���м� ����::Begin", _T("CStdDialog"), 10);
#endif
	if(m_xSplitter1.GetSafeHwnd())
	{
		m_xSplitter1.DetachAllCtrls();
		m_xSplitter1.DestroyWindow();
	}
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("���м� ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("�������� ����::Begin", _T("CStdDialog"), 10);
#endif
	if(m_pLeftBarDlg)
	{
		m_pLeftBarDlg->DestroyWindow();
		delete m_pLeftBarDlg;
		m_pLeftBarDlg = NULL;
	}
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("�������� ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("��ǲ�� ����::Begin", _T("CStdDialog"), 10);
#endif
	if(m_pInputBarDlg)
	{
		m_pInputBarDlg->DestroyWindow();
		delete m_pInputBarDlg;
		m_pInputBarDlg = NULL;
	}
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("��ǲ�� ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("��ǲ��2 ����::Begin", _T("CStdDialog"), 10);
#endif
	if(m_pExtInputBarDlg)
	{
		m_pExtInputBarDlg->DestroyWindow();
		delete m_pExtInputBarDlg;
		m_pExtInputBarDlg = NULL;
	}
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("��ǲ��2 ����::End", _T("CStdDialog"), 10);
#endif
	
	

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("�޼����� ����::Begin", _T("CStdDialog"), 10);
#endif
	if(m_pMsgBar)
	{
		m_pMsgBar->DestroyWindow();
		delete m_pMsgBar;
		m_pMsgBar = NULL;
	}	
// --> [Edit]  ������ 2008/10/29
//@Sol100618-->
// 	if(m_pCommMsg)
// 	{
// 		m_pCommMsg->DestroyWindow();
// 		delete m_pCommMsg;
// 		m_pCommMsg = NULL;
// 	}
//@Sol100618 <--

// <-- [Edit]  ������ 2008/10/29
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("�޼����� ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("���� ���̺귯�� ����::Begin", _T("CStdDialog"), 10);
#endif
	//���� Lib UnLoad
	UnLoad_ReplayChart_Lib();	
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("���� ���̺귯�� ����::End", _T("CStdDialog"), 10);
#endif

	//===========================================================================
	POSITION pos = NULL;	
//	CRealChartData *pRealChart = NULL;
//	while(m_lstRegRealChart.GetHeadPosition())
//	{
//		pRealChart = NULL;		
//		pRealChart = (CRealChartData*)m_lstRegRealChart.GetAt(pos);
//		if(pRealChart)
//		{	delete pRealChart;
//			pRealChart = NULL;
//			m_lstRegRealChart.RemoveAt(pos);							
//		}		
//	}

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("DRDS ����::Begin", _T("CStdDialog"), 10);
#endif
// --> [Edit]  ������ 2008/11/18	( �������� ���� )
// --> [Edit]  ������ 2008/11/23	( �ڵ鿡 ��ϵǾ��ִ� ������ ���� ������Ų��. )
	if ( m_pDrdsLib )
//@SolomonGNF 		m_pDrdsLib->DrdsUnadviseAll(m_hWnd);
// <-- [Edit]  ������ 2008/11/23

	AllCodeUnAdvise( FALSE ); //Real ���� ����..
// <-- [Edit]  ������ 2008/11/18
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("DRDS ����::End", _T("CStdDialog"), 10);
#endif	//===========================================================================
	

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("����Ÿ ������ ����::Begin", _T("CStdDialog"), 10);
#endif
	if(m_pDataWindow)
	{
		m_pDataWindow->DestroyWindow();
		delete m_pDataWindow;
		m_pDataWindow = NULL;
	}
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("����Ÿ ������ ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("���� ����::Begin", _T("CStdDialog"), 10);
#endif
	if(m_pDlgToolbar)
	{
		m_pDlgToolbar->DestroyWindow();
		delete m_pDlgToolbar;
		m_pDlgToolbar = NULL;
	}
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("���� ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("�̹�������Ʈ ����::Begin", _T("CStdDialog"), 10);
#endif
	//ImageList���� �����Ѵ�. 05.09.01
	DeleteAllImageList();
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("�̹�������Ʈ ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("Ŀ�� ����::Begin", _T("CStdDialog"), 10);
#endif
	if(m_hCursor)
		::DeleteObject(m_hCursor);
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("Ŀ�� ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	CloseLogFile();
#endif

#ifdef _WRITELOG_SISE
	// Log - ojtaso (20080107)
	CloseLogFile();
#endif

	// ������ ���� - ojtaso (20080519)
	if(m_pDlgGlass) 
	{
		delete m_pDlgGlass;
		m_pDlgGlass = NULL;
	}

	m_memoryDC.DeleteDC();
	m_bufferBitmap.DeleteObject();

// --> [Edit]  ������ 2008/12/17
#ifdef	_LASTINFO_SAVE
//	LastInfoSave();
	if ( !m_strSaveKey.IsEmpty() )
		FileCopyAndSaveAndDelete(LAST_HWND_COPY);	//	����ߴ� ���� �����

	FileCopyAndSaveAndDelete(HWND_DEFAULT_COPY);
	FileCopyAndSaveAndDelete(DELETE_FILE);	//	����ߴ� ���� �����
#endif
// <-- [Edit]  ������ 2008/12/17

	CExOCXDLL::OnDestroy();
}

// --> [Edit]  ������ 2008/07/25		( �����͸� �ѱ涧 ���Ͱ� �ƴ� �׳� Char�� �ѱ� )
HRESULT CStdDialog::ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	{
		CString szDebug;
		szDebug.Format("TR[%s](%s)(%s) Len=%d\n", szTR, szMsgCode, szMessage, dwTRDateLen);
		OutputDebugString(szDebug);
	}

	CWnd *pWndRequested = NULL;
	CString strTRCode = szTR;
	strTRCode.Remove(' ');

	BOOL bSendGuideLine = FALSE;
	CString strMsgCode = szMsgCode;
	if (strMsgCode.GetLength() <= 0)
	{
		EnableWindowAll(TRUE);	
	}
	//@Solomon:091123SM068	-->
	else if (strMsgCode.CompareNoCase(QUERY_STRGUIDELINE) == 0)
	{
		bSendGuideLine = TRUE;
	}
	//@Solomon:091123SM068	<--
	else
	{
		if (strMsgCode.GetLength() > 0 || dwTRDateLen == 0)
		{
			if (!m_bEnable)
				EnableWindowAll(TRUE);
		}

		return 1L;
	}

	STTRINFO trInfo;
	long lCnt = m_arryQuery.GetSize();
	CString sTemp;
	for (long lPos=0; lPos<lCnt; lPos++)
	{
		trInfo = m_arryQuery.GetAt(lPos);
		if (trInfo.m_strTrCode==strTRCode)
		{
			pWndRequested = trInfo.m_pWndRequested;
			//@Solomon:091123SM068	-->
			if (bSendGuideLine)
			{
				stReceiveFromServer stR;
				stR.m_strTRCode = szMsgCode;
				stR.m_pBuffer = (LPVOID)(LPCTSTR)aTRData; 
				stR.m_dwLength = dwTRDateLen;
				if (((CChartMng*)pWndRequested) == ((CChartMng*)&m_ChartMng))
				{
					CChartItem* pItem = m_ChartMng.GetpChartItemSelected();
					if (pItem)
						pItem->SendMessageToInvoke(INVOKEMSG_RECEIVEDATA, (long)&stR, sizeof(stReceiveFromServer));
				}

				//OutputDebugString("ReceiveData End\n");
				return 0L;
			}
			//@Solomon:091123SM068	<--

			m_arryQuery.RemoveAt(lPos);

			lCnt = m_arryQuery.GetSize();
			break;
		}
	}

	if (strTRCode == QUERY_STRFOFOREIGN_HOGA)
	{
		if (pWndRequested->GetSafeHwnd())
		{
			stReceiveFromServer stR;
			stR.m_strTRCode = strTRCode;
			stR.m_pBuffer = aTRData;
			stR.m_dwLength = dwTRDateLen;
			pWndRequested->SendMessage(UMSG_RECEIVEFROMSERVER,0,(long)&stR);
		}

		return 0L;
	}

	//<<20100308_JS.Kim �ַθ�
	if (strTRCode == QUERY_STRFOFOREIGN_JAN)		// �ܰ� ����
	{
		CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrOpenPosition;
		
		FX_CHE_STRUCT* pCheSt = NULL;
		int nIdxData = 0;
		while(nIdxData < dwTRDateLen)
		{
			pCheSt = (FX_CHE_STRUCT *)((char *)aTRData+nIdxData);
			arrOpenPosition.Add(pCheSt);

			nIdxData += sizeof(FX_CHE_STRUCT);
		}

		CChartItem* pChartItem = (CChartItem *)pWndRequested;
		if(pChartItem)
		{
			pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 1, (long)&arrOpenPosition);
			//pChartItem->InvokeAddIn("ChartKoreaSorimachiAddIn.dll:USER_COMMENT", 1, (long)&arrOpenPosition);
		}
		return 0L;
	}
	else if (strTRCode == QUERY_STRFOFOREIGN_CHE2)	// ü�᳻��
	{
		CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrPendingOrder;
		
		FX_CHE_STRUCT* pCheSt = NULL;
		int nIdxData = 0;
		while(nIdxData < dwTRDateLen)
		{
			pCheSt = (FX_CHE_STRUCT *)((char *)aTRData+nIdxData);
			arrPendingOrder.Add(pCheSt);
			nIdxData += sizeof(FX_CHE_STRUCT);
		}

		CChartItem* pChartItem = (CChartItem *)pWndRequested;
		if(pChartItem)
		{
			// ü�᳻���� ����/û�� ǥ�� �ʿ� ������.
			pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 6, (long)&arrPendingOrder);
		}
		return 0L;
	}
	//>>


	if (pWndRequested)
	{
		//2007.04.25 by LYH ���� �ڵ�(���� CodeCombo �ڵ�� ���ؼ� �ٸ� ��� ����ȸ)
		if (m_strRecvCode == "" || m_pInputBarDlg->m_pCodeInput->GetCode() == "" || m_ChartMng.IsInLoop() || m_pInputBarDlg->m_pCodeInput->GetCode() == m_strRecvCode)
		{
			if (pWndRequested->GetSafeHwnd())
			{
				ConvertKoscomData(strTRCode, (char*)aTRData, dwTRDateLen);
				stReceiveFromServer stR;
				stR.m_strTRCode = strTRCode;
				stR.m_pBuffer = (LPVOID)(LPCTSTR)m_strOut;
				stR.m_dwLength = m_strOut.GetLength();

				pWndRequested->SendMessage(UMSG_RECEIVEFROMSERVER,0,(long)&stR);
			}
			m_strRecvCode = "";
		}
		else
			m_pInputBarDlg->SetKeyCodeChange(NID_CODEINPUT, (LPARAM)(LPCTSTR)m_pInputBarDlg->m_pCodeInput->GetCode());
	}
	else
	{
		long lCnt = m_arryQueryInvoke.GetSize();
		for(long lPos=0;lPos<lCnt;lPos++)
		{
			trInfo = m_arryQueryInvoke.GetAt(lPos);
			if(trInfo.m_strTrCode==szTR)
			{
				pWndRequested = trInfo.m_pWndRequested;
				m_arryQueryInvoke.RemoveAt(lPos);
				break;
			}
		}

		if (pWndRequested)
		{
			ConvertKoscomData(szTR, (char*)aTRData, dwTRDateLen);
			stReceiveFromServer stR;
			stR.m_strTRCode = szTR;
			stR.m_pBuffer = (LPVOID)(LPCTSTR)m_strOut;
			stR.m_dwLength = m_strOut.GetLength();

			if (dwTRDateLen>=12)
			{
				CChartItem *pWndChartItem = NULL;
				char szWndChartItem[18];
				memcpy(szWndChartItem,aTRData,12);
				szWndChartItem[12] = NULL;
				pWndChartItem = (CChartItem *)atol(szWndChartItem);
				if (pWndChartItem->GetSafeHwnd())
					pWndChartItem->SendMessageToInvoke(INVOKEMSG_RECEIVEDATA,(long)&stR, sizeof(stReceiveFromServer));					
			}
		}
	}

	
	BOOL bRet = m_ChartMng.IsToBeReceivedMore();
	if (!m_arryQuery.GetSize()&&!bRet)
		EnableWindowAll();

	SendMessage2StatusBar(MSG_SEARCH_END, (LPTSTR)(LPCTSTR)szMessage);

	return S_OK;
}

HRESULT	CStdDialog::ReceiveMessage(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage)
{
	CString szTmp;
	szTmp.Format("[%s]%s\n", szMsgCode, szMessage);
	
	if(strcmp(szMsgCode, "e")==0)		// Ÿ�Ӿƿ�
	{
		AfxMessageBox(szMessage);
		EnableWindowAll(TRUE);	//@���� �ӽ��ڵ���. �� ��ƾ�� ���������� �����ٶ�.
	}
	else if(strcmp(szMsgCode, "99999")==0)	// �ش� �ڷᰡ �����ϴ�.
	{
		EnableWindowAll(TRUE);	//@���� �ӽ��ڵ���. �� ��ƾ�� ���������� �����ٶ�.
		
		AfxMessageBox("�ش� �ڷᰡ �����ϴ�.");
		m_ChartMng.ReplaceOldCode();
	}
	else
		EnableWindowAll(TRUE);	//@���� �ӽ��ڵ���. �� ��ƾ�� ���������� �����ٶ�.

	return S_OK;
}

BOOL CStdDialog::GetStatusEnable()
{
	return m_bEnable;
}

void CStdDialog::EnableWindowAll(BOOL bEnable /*= TRUE*/)
{
	m_bEnable = bEnable;
	if(bEnable)
	{
		if(m_pLeftBarDlg->GetSafeHwnd())
		{
			m_pLeftBarDlg->EnableWindow();
		}
		if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		{
			m_pInputBarDlg->EnableWindow();
		}
		if(m_pExtInputBarDlg->GetSafeHwnd())
		{
			if(m_hReplayChartLib == NULL)
				m_pExtInputBarDlg->EnableWindow();
			else
				m_pExtInputBarDlg->EnableWindow(FALSE);
		}
		if(m_ChartMng.GetSafeHwnd())
		{
			m_ChartMng.EnableWindow();
		}
		
		KillTimer(1);
// --> [Edit]  ������ 2008/10/29
//@Sol100618		m_pCommMsg->HideMessage();
// <-- [Edit]  ������ 2008/10/29
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
	else
	{
		if(m_pLeftBarDlg->GetSafeHwnd())
		{
			m_pLeftBarDlg->EnableWindow(FALSE);
		}
		if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())	
		{
			m_pInputBarDlg->EnableWindow(FALSE);
		}

		if(m_pExtInputBarDlg->GetSafeHwnd())	
		{
			m_pExtInputBarDlg->EnableWindow(FALSE);
		}

		if(m_ChartMng.GetSafeHwnd())
		{
			m_ChartMng.EnableWindow(FALSE);
		}

		CWnd *pFocus = GetFocus();
		CWnd *pParent = GetParent();
		if (pParent->IsChild(pFocus))
		{
// --> [Edit]  ������ 2008/10/29
			CString		strTemp = "��ȸ���Դϴ�...";
//@Sol100618			m_pCommMsg->ShowMessage((LPCTSTR)strTemp,(CWnd*)this);
// <-- [Edit]  ������ 2008/10/29
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		}

// --> [Edit]  ������ 2008/07/23	( �ӽ� : Ÿ�Ӿƿ� �ϴ� ���� )
//		AfxMessageBox("SetTimer(1)");
//		SetTimer(1,20000,NULL); // 20sec
// <-- [Edit]  ������ 2008/07/23
	}
}

// �׽�Ʈ����̸� �������� ���� ������ ���� �������� ���� �����ͷ� ó���Ѵ�.
int CStdDialog::Send2Server(LPCTSTR lpTrCode,LPCTSTR lpData,int nSize,BOOL bWait, int nArrayIndex)
{
// 	{
// 		CString szDebug;
// 		szDebug.Format("->SVR TR[%s]Len=%d [%-*.*s]\n", lpTrCode, nSize, nSize, nSize, lpData);
// 		OutputDebugString(szDebug);
// 	}
// 
// 	int nTestMode = AfxGetApp()->GetProfileInt("TESTMODE", "HW", 0);

	SendMessage2StatusBar(MSG_GOING);	
	EnableWindowAll(FALSE);	

	if (m_TRComm)
	{
		if (CString(lpTrCode) == QUERY_STRFOFOREIGN_HOGA)
		{
			m_TRComm->SetBaseData("SCREENCODE", m_strScreenNo);
			m_TRComm->SetBaseData("PtManager", m_szPTManager);
			m_TRComm->SetTR(lpTrCode);
			m_TRComm->Send2Server((LPVOID)lpData, nSize, TRKEY_STRFOFOREIGN_HOGA);
		}
		else if(CString(lpTrCode) == QUERY_STRFOFOREIGN_JAN || CString(lpTrCode) == QUERY_STRFOFOREIGN_CHE2)
		{
			m_TRComm->SetBaseData("SCREENCODE", m_strScreenNo);
			m_TRComm->SetTR(lpTrCode);
			m_TRComm->Send2Server((LPVOID)lpData, nSize, TRKEY_FID_END);
		}
		// ����TR
		else
		{
			// (2008/9/14 - Seung-Won, Bae) for Pair Tick
			if (GetRequestPairTickState() != E_RPTS_PAIR)
			{
				m_TRComm->SetTR(lpTrCode);
				m_TRComm->SetBaseData("PtManager", m_szPTManager);
				m_TRComm->SetBaseData("SCREENCODE", m_strScreenNo);

				return m_TRComm->Send2Server((LPVOID)lpData, nSize, TRKEY_FID_END);
			}
			else
				return 0;
		}

		return 0;
	}
	else
		return 0;
}

void CStdDialog::SetShowExtInputBarDlg(BOOL bShowExtInputBarDlg)
{
	m_bShowExtInputBarDlg = bShowExtInputBarDlg;
	if(m_pInputBarDlg)
	{
		m_pInputBarDlg->SetShowExtInputBarDlg(bShowExtInputBarDlg);
	}	
	Resize();
}

void CStdDialog::SetLeftBarType(UINT nLeftBarType)
{
	m_nLeftBarType = nLeftBarType;
	CRect rect;	
	CRect chartMngRect;

	if(!m_pInputBarDlg||!m_pExtInputBarDlg||!m_pLeftBarDlg||!m_ChartMng.GetSafeHwnd()) return;
	rect = GetRect(ID_INPUTBAR,TRUE);
	m_pInputBarDlg->MoveWindow(rect);
	m_pInputBarDlg->ShowWindow(SW_SHOW);
	m_pInputBarDlg->SetLeftBarType(m_nLeftBarType);
	
	if(m_ChartMng.GetSafeHwnd())
	{
		m_ChartMng.GetWindowRect(chartMngRect);
		ScreenToClient(chartMngRect);
		CRect rectNew = GetRect(ID_CHART,TRUE);
		if(chartMngRect!=rectNew)	
		{
			chartMngRect.left	= rectNew.left;		//ChartMng
			chartMngRect.right	= rectNew.right;	//ChartMng
			chartMngRect.bottom	= rectNew.bottom;		
			chartMngRect.top	= rectNew.top;				
		}

		m_ChartMng.MoveWindow(chartMngRect);
	}
	
	rect = GetRect(ID_EXTINPUTBAR,TRUE);
	m_pExtInputBarDlg->MoveWindow(rect);
	m_pExtInputBarDlg->ShowWindow(SW_SHOW);

	rect = GetRect(ID_LEFTBAR,TRUE);
	if(rect.IsRectNull())
	{
		m_pLeftBarDlg->ShowWindow(SW_HIDE);
		m_pLeftBarDlg->ModifyStyle(WS_CHILD, WS_POPUP|WS_DLGFRAME|WS_CLIPSIBLINGS);
		m_pLeftBarDlg->SetParent(this);

		if(m_btnSideBar.GetSafeHwnd())
		{
			m_btnSideBar.MoveWindow(GetRect(IDC_SIDEBAR, TRUE));
			m_btnSideBar.ShowWindow(SW_SHOW);
			m_btnSideBar.Invalidate();
		}
	}
	else
	{
		m_pLeftBarDlg->ModifyStyle(WS_POPUP|WS_DLGFRAME|WS_CLIPSIBLINGS, WS_CHILD);
		m_pLeftBarDlg->SetParent(this);
		
		m_pLeftBarDlg->MoveWindow(rect);
		m_pLeftBarDlg->ShowWindow(SW_SHOW);

		if(m_btnSideBar.GetSafeHwnd())
		{
			m_btnSideBar.MoveWindow(CRect(0,0,0,0));
			m_btnSideBar.ShowWindow(SW_HIDE);
			m_btnSideBar.Invalidate();
		}
	}

	rect = GetRect(ID_SPLITTER,TRUE);
	if(rect.IsRectNull())
	{
		m_xSplitter1.ShowWindow(SW_HIDE);
	}
	else
	{
		m_xSplitter1.MoveWindow(rect);
		m_xSplitter1.ShowWindow(SW_SHOW);
	}	
	switch(m_nLeftBarType)
	{
	case 0:
		break;
	case 1:
		m_xSplitter1.DetachAllCtrls();
		m_xSplitter1.AttachAsLeftCtrl(m_pLeftBarDlg);
		m_xSplitter1.AttachAsRightCtrl(ID_CHART);
		m_xSplitter1.AttachAsRightCtrl(m_pInputBarDlg);
		m_xSplitter1.AttachAsRightCtrl(m_pExtInputBarDlg);	
		m_xSplitter1.RecalcLayout();
		break;
	case 2:
		m_xSplitter1.DetachAllCtrls();
		m_xSplitter1.AttachAsLeftCtrl(ID_CHART);
		m_xSplitter1.AttachAsLeftCtrl(m_pInputBarDlg);
		m_xSplitter1.AttachAsLeftCtrl(m_pExtInputBarDlg);
		m_xSplitter1.AttachAsRightCtrl(m_pLeftBarDlg);	
		if(	(m_bShowDataListWnd				) &&
			(m_pDataWindow != NULL			) &&
			(m_pDataWindow->GetSafeHwnd()	) 
		)
		{
			m_xSplitter1.AttachAsLeftCtrl(m_pDataWindow);
		}
		m_xSplitter1.RecalcLayout();
		break;
	}

	m_pLeftBarDlg->Invalidate();
	m_xSplitter1.Invalidate();
}

LRESULT CStdDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case UMSG_SEND2SERVER:
		{
			stSend2Server *pSt = (stSend2Server *)lParam;

			if ((CString(pSt->m_strTRCode) != QUERY_STRFOFOREIGN) &&		//	�ؿܼ���
				(CString(pSt->m_strTRCode) != QUERY_STRFOFOREIGN_HOGA) &&	//	ȣ��â��ȸ
				(CString(pSt->m_strTRCode) != QUERY_STRFOFOREIGN_HOGA2) &&	//	�ż�/�ŵ� ȣ����ȸ
				(CString(pSt->m_strTRCode) != QUERY_STRFOFOREIGN_CHE2)		&&		// ü��
				(CString(pSt->m_strTRCode) != QUERY_STRFOFOREIGN_JAN) )		// �ܰ�
				break;

			if ((CString(pSt->m_strTRCode) != QUERY_STRFOFOREIGN))
				memcpy(&m_st2Server, pSt, sizeof(stSend2Server));				

			int lCnt = m_arryQuery.GetSize();

			STTRINFO trInfo, trCheckInfo;
			trInfo.m_pWndRequested = pSt->m_pWnd;
			trInfo.m_strTrCode = pSt->m_strTRCode;

			for (long lPos=0;lPos<lCnt;lPos++)
				trCheckInfo = m_arryQuery.GetAt(lPos);
			
			if (pSt->m_nType==0)
			{
				trInfo.m_nType = 0;
				m_arryQuery.Add(trInfo);
			}
			else
			{
				trInfo.m_nType = pSt->m_nType;
				m_arryQueryInvoke.Add(trInfo);
			}

			Send2Server(pSt->m_strTRCode,(char*)pSt->m_pBuffer,pSt->m_lLength, (BOOL)wParam);
		}
		break;
	case UMSG_SENDSHAREDCODE:
		{
			if (!m_bEnable)
				break;

// --> [Edit]  ������ 2008/11/07	( ������ȸ ���̸� ������ ���� ���͵� ��������. )
			if (m_ChartMng.IsInLoop())
				break;
// <-- [Edit]  ������ 2008/11/07

			// ������Ʈ���� ���� ����ó��
			if (m_pInputBarDlg)
			{
				STINPUTCODEDATA stInputCodeData;
				stInputCodeData.m_strCode = CString((LPCTSTR)wParam);
				stInputCodeData.m_strTime = "";
				stInputCodeData.m_nDefault = MODE_DEFAULT;
				stInputCodeData.m_nMode = CHANGE_ITEM;

				if (m_nCodeInput)
					m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, MODE_DEFAULT, (LPARAM)&stInputCodeData);

// --> [Edit]  ������ 2008/10/28
				//	���� RQ�� ���õǾ� �ִ� �ð����� Index ���ϱ�
				CChartItem* pSelectedChartItem = m_ChartMng.GetpChartItemSelected();
				if (pSelectedChartItem)
				{
					// lParam �� 1�̸� Get
					long lResult = pSelectedChartItem->SendMessage(UDM_CHANGE_TIMEDIFF, 0, 1);
					// lResult => 1 Based �ð��� Index, -1 : ���� �ð����� ����

					SetTabTimeIndex(lResult);
					ChangeTimeSet(lResult);
				}
// <-- [Edit]  ������ 2008/10/28
			}
		}		
		break;
	case UMSG_SENDSYNCCODE:
		break;
	case UMSG_SENDSHARETIME:
		m_ChartMng.SetSharedTime((LPCTSTR)wParam);
		break;
	case UMSG_SENDTOMSGBOX:
		m_pMsgBar->ShowMessage((LPCTSTR)wParam,(CWnd*)lParam);
		break;
	case UMSG_EDITCODE_INIT:
		if (m_pInputBarDlg)
			m_pInputBarDlg->SetCodeInit();
//		break;
	case UMSG_UPDATECODE:
		{
			//�ڽ��� ������� ����(������)
			if(m_pInputBarDlg)
			{
				switch(lParam) {
				case 1:
					if(m_pInputBarDlg->m_nMarketType != CMasterTypeInfo::STOCK_CHART)
						return 1L;
					break;
				case 8:
				case 12:
					if(m_pInputBarDlg->m_nMarketType != CMasterTypeInfo::FUTOPT_CHART)
						return 1L;
					break;
				case 3:
						return 1L;
				case 100:
					return 1L;
				default:
					return 1L;
				}

				// �����ڵ�� RQ�� ã�� ���� : �������� - ojtaso (20070705)
				if(!m_ChartMng.FindSelectChartByItemCode((LPCTSTR)wParam))
				{
					m_pInputBarDlg->SetMode(0);
					// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
					CString strCodeInputCode = m_pInputBarDlg->GetTypedItemCode();
					CString strSetCode = (LPCTSTR)wParam;
					if(strSetCode.GetLength() && strCodeInputCode != strSetCode)
					{
						m_pInputBarDlg->SetShareCodeChanged(TRUE);
						// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
						m_pInputBarDlg->m_pCodeInput->SetCode(CStdDialog::GetRealItemCode( ( LPCTSTR)wParam));
						m_pInputBarDlg->SetShareCodeChanged(FALSE);
					}
				}
			}
		}
		break;
	case UMSG_DROPCODE:
		{
			//�ڽ��� ������� ����(������)
			if(m_nChartType == MULTI_CHART && m_pInputBarDlg)
			{
				m_pInputBarDlg->SetCodeDragAndDrop(CString((LPCTSTR)wParam), lParam);
								STINPUTCODEDATA stInputCodeData;
				stInputCodeData.m_strCode = CString((LPCTSTR)wParam);
				stInputCodeData.m_strTime = "";
				stInputCodeData.m_nDefault = MODE_DEFAULT;
				stInputCodeData.m_nMode = CHANGE_ITEM;
				m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, MODE_DEFAULT, (LPARAM)&stInputCodeData);
			}
			//�ڽ��� ������� ����(������)
			if(m_nChartType == SINGLE_CHART)
			{
				if(m_pInputBarDlg)
				{
					switch(lParam) {
					case 1:
						if(m_pInputBarDlg->m_nMarketType != CMasterTypeInfo::STOCK_CHART)
							return 1L;
						break;
					case 8:
					case 12:
						if(m_pInputBarDlg->m_nMarketType != CMasterTypeInfo::FUTOPT_CHART)
							return 1L;
						break;
					case 3:
							return 1L;
					case 100:
						return 1L;
					default:
						return 1L;
					}

					STINPUTCODEDATA stInputCodeData;
					stInputCodeData.m_strCode = CString((LPCTSTR)wParam);
					stInputCodeData.m_strTime = "";
					stInputCodeData.m_nDefault = MODE_DEFAULT;
					stInputCodeData.m_nMode = CHANGE_ITEM;
					m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, MODE_DEFAULT, (LPARAM)&stInputCodeData);
				}
			}
		}
		break;
	case UMSG_GETSTATUSENABLE:
		{
			return GetStatusEnable();
		}
		break;
	case UMSG_CHANGEMODEBYINPUTBAR:
		{
			int nMode = wParam;
			if(m_pInputBarDlg)
				m_pInputBarDlg->SetMode(nMode);
		}
		break;
	case UMSG_SETHIGHLIGHTINFO:
		{
			if(m_pExtInputBarDlg)
			{
				m_pExtInputBarDlg->SendMessage(UMSG_SETHIGHLIGHTINFO,wParam,lParam);
			}			
		}
		break;
	case USMG_SHOW_DATAWND:
		{
			m_bShowDataListWnd = (BOOL)lParam;
			Resize();
			CRect ViewRect(0,0,0,0);
			ViewRect = GetRect(ID_CHART);
			ShowDataListView(m_bShowDataListWnd, ViewRect);
		}
		break;
	case USMG_PLEASE_RESIZE:			break;
	case UMSG_SETMATRIX:				break;
	case UMSG_FORCE_SHOW_HOGAWND:		break;
	case WM_DRDS_DATA:					break;
	case UMSG_SENDTR_HOGA_DATA:
		{
			CString* pStrCode, sCode;
			pStrCode = (CString*)wParam;
			sCode.Format("%s", pStrCode->GetBuffer(pStrCode->GetLength()));
			Request_StockHogaData(sCode);
		}
		break;
	case UMSG_DISCONNECT_HOGA_REAL:
		Request_Disconnect_StockRealHogaData(_T(""));
		break;
	case UMSG_GET_CHARTMODE:
		{
			// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
			if(m_pInputBarDlg)
			{
				if(wParam == 1 && !m_pInputBarDlg->GetReplayChartMode())
					return -1;

				return m_pInputBarDlg->m_nCurrentMode;
			}
		}
		break;
	case UMSG_GET_REPLAY_STARTTIME:
		{
			if(m_pInputBarDlg)
				return m_pInputBarDlg->GetReplayStartTime();
		}
		break;
	case UMSG_GET_REPLAY_DAYBEFORE:
		{
			if(m_pInputBarDlg)
				return m_pInputBarDlg->GetReplayDayBefore();
		}
		break;
	case UMSG_STOP_JIPYOCYCLE:
		{
			if(m_pLeftBarDlg)
			{
				m_pLeftBarDlg->SendMessage(message, 0, 0);
			}			
		}
		break;
	case UMSG_SHOW_HOGADLG:
		if(m_ChartMng)
			m_ChartMng.m_pFloatingDlg->Invalidate();
		break;
	case UMSG_CHECK_BE_COMPLETE_QUERYDATA:
		{
			STTRINFO trCheckInfo;
			
			CString strTRCode;
			strTRCode.Format("%s",(LPTSTR)(LPCTSTR)wParam);

			int lCnt = m_arryQuery.GetSize();
			for(long lPos=0;lPos<lCnt;lPos++)
			{
				trCheckInfo = m_arryQuery.GetAt(lPos);
				if(trCheckInfo.m_strTrCode == strTRCode)
				{
#ifdef _DEBUG
					CString strTemp;
					strTemp.Format("Not Send 2 TR No. : %s", (LPTSTR)(LPCTSTR)strTRCode);
					OutputDebugString(strTemp);
#endif
					return 0l;
				}				
			}
			return 1l;
		}
		break;
	case UMSG_GETJONGMOKLINK:
		{
			if(m_pExtInputBarDlg)
				return m_pExtInputBarDlg->SendMessage(UMSG_GETJONGMOKLINK);
		}
		break;
	case UMSG_DESTROY_FLOATINGHOGA:
		{
			if( m_ChartMng.m_pFloatingDlg == NULL ) return 0;
			
			if(lParam)
				m_ChartMng.m_pFloatingDlg->ShowWindow(SW_SHOW);
			else
				m_ChartMng.m_pFloatingDlg->ShowWindow(SW_HIDE);
		}
		break;
	case UMSG_EXTINPUT_CHANGEDATE:
		{
			if(m_pInputBarDlg)
				m_pInputBarDlg->SendMessage(UMSG_EXTINPUT_CHANGEDATE, wParam, lParam);
		}
		break;
	case UMSG_ONECLICK_HIDE:
		{
			SetOneClickHide(lParam);
		}
		break;
	case UMSG_RESETCHART:
		{
			if(m_pDlgToolbar)
			{
				m_pDlgToolbar->ResetAllButton();
				m_bShowToolBar = TRUE;
				if(m_bShowDataListWnd)
				{
					m_bShowDataListWnd = FALSE;
					Resize();
					CRect ViewRect(0,0,0,0);
					ViewRect = GetRect(ID_CHART);
					ShowDataListView(m_bShowDataListWnd, ViewRect);
				}
			}
		}
		break;
		//2007.01.17 add by LYH  ���ٹ�ư ����ȭ
	case  UMSG_SYNC_TOOLBAR:
		if(m_pDlgToolbar)
		{
			if(lParam == 50000)
				m_pDlgToolbar->ResetAllButton();
			else
				m_pDlgToolbar->SetToggleButton(wParam, lParam);
		}
		break;
	case WMU_GET_KEY_INFO_ARRAY:
		if(m_pInputBarDlg)
		{
			//m_pInputBarDlg->m_ctrlCodeInput.OnGetKeyInfoArray(wParam, lParam);
		}
		break;
	case UMSG_CREATESCREEN_LINK:
		if(m_pInputBarDlg)
		{
// --> [Edit]  ������ 2008/10/30	( ȣ�����̺��� �ֹ�ȭ�� ������ ���� �޽��� )
//			::SendMessage(m_hViewInMain, RMSG_GET_LINKINFODATA, wParam, lParam);
// <-- [Edit]  ������ 2008/10/30
		}
		break;

	case UMSG_CHART_TICKCHART:
		{
			//	���ӿ����� ��������� ȣ����Ʈ�� ƽ��Ʈ�ϰ�� ��ȸ�� ���� �ʴ´�. 
			if ( ((CInputBarDlg*)m_pInputBarDlg)->GetCheHogaType() == 1 )
				return 0;

			if ( ((CInputBarDlg*)m_pInputBarDlg)->GetType() == TICK_DATA_CHART )
				return 0;

			return 1;
		}		
		break;
		
	}
	return CExOCXDLL::WindowProc(message, wParam, lParam);
}
/****************************************************************************/
/* Function : ��� Ű �ʱ�ȭ												*/
/* Paramter : ?																*/
/* Return	:																*/
/* Author	: By DRFN														*/
/* Comments	: FX Margin �ϸ鼭 �ҽ� ���� (�ѱ��Ҹ�ġ �����)				*/
/****************************************************************************/
void CStdDialog::InitDrds()
{
	m_nDrdsInfoCnt = 1;
	
	m_DrdsInfo[0].DataName = "FOFR";								// �ؿܼ���
	m_DrdsInfo[0].FieldName = "*";
	m_DrdsInfo[0].KeyName = "";
	m_DrdsInfo[0].nID = 0;
	m_DrdsInfo[0].bAdvise = FALSE;
}

BOOL CStdDialog::MakeDrdsConnectAdviseFromCode(int nI, BOOL bAddMod)
{	
	char* DataName = (LPSTR)(LPCTSTR)m_DrdsInfo [nI].DataName;
	char* FieldName = (LPSTR)(LPCTSTR)m_DrdsInfo [nI].FieldName;

	// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
	CString KeyName = GetRealItemCode (m_DrdsInfo[nI].KeyName);

	UINT	nID = m_DrdsInfo[nI].nID;
	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);

	if(!pDrdsMng) return FALSE;

	// �ǽð� ���̺귯�� �ε�
	if (m_pDrdsLib == NULL)		m_pDrdsLib = pDrdsMng->AdviseDrdsLib();
	if (!m_pDrdsLib) return FALSE;

	/*
	CString strType = "1";

	ST_CODEDATA oneCode;

	oneCode.m_strCode = m_DrdsInfo[nI].KeyName;
	
	if (m_pInputBarDlg->m_pDataManager)
		oneCode.m_bType = GetMarketTypeFromCode(oneCode.m_strCode);
	else
		oneCode.m_bType = atoi(strType);
	
	if(nI == 0)
		m_ListStockRealCodes.AddTail(oneCode);

	if (m_RealTrCounter.AddTr (KeyName) == 1)
	{
		m_pDrdsLib->DrdsInit (m_hWnd);
		m_pDrdsLib->DrdsAdvise (nID, DataName, FieldName, ( char *)( const char *)KeyName);
		m_DrdsInfo[nI].bAdvise = TRUE;
	}
	*/
	CString strType("1");
	ST_CODEDATA oneCode;
	int nType;
	oneCode.m_strCode = m_DrdsInfo[nI].KeyName;
	if(m_pInputBarDlg->m_pDataManager)
		oneCode.m_bType = GetMarketTypeFromCode(oneCode.m_strCode, nType);// m_pDataManager->GetCodeInfo("", 2, oneCode.m_strCode);
	else
		oneCode.m_bType = atoi(strType);

	// �������� ������ : �������� - ojtaso (20070208)
//	if(bAddMod)
	if(m_RealTrCounter.AddTr(oneCode.m_strCode) == 1)
	{
		//@Solomon-->
		//m_pDrdsLib->DrdsInit(m_hWnd);
		//m_pDrdsLib->DrdsAdvise(nID, DataName, FieldName, KeyName);
		if(m_pPTManager) m_pPTManager->RequestMChartReal('A', DataName, KeyName);
		//@Solomon<--
		m_DrdsInfo[nI].bAdvise = TRUE;
	}


	return TRUE;
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
void CStdDialog::AddRealCodes()
{
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
void CStdDialog::AddRealCodeList(LPCTSTR lpszCode)
{
	ST_CODEDATA oneCode;
	oneCode.m_strCode = lpszCode;

	int nType;
	int nMarketType = GetMarketTypeFromCode(lpszCode, nType);
	BOOL bRet = FALSE;
	if(nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
		m_ListStockRealCodes.AddTail(oneCode);
	}
	else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		m_ListUpjongRealCodes.AddTail(oneCode);
	}
	else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
	{
		m_ListFutOptRealCodes.AddTail(oneCode);
	}
	else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
		m_ListStockFutOptRealCodes.AddTail(oneCode);
	}
	else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		m_ListForeignRealCodes.AddTail(oneCode);
	}
	m_RealTrCounter.AddTr(lpszCode);
}

void CStdDialog::RemoveDrdsFromCode(LIST_CODEDATA *pListRealCodes, CString strCode/*=""*/)
{
	POSITION pos = NULL;
	int nCount = pListRealCodes->GetCount();
	if(strCode.GetLength() > 0)
	{
		for(int nIndx = 0; nIndx < nCount; ++nIndx)
		{
			pos = pListRealCodes->FindIndex(nIndx);
			if(strCode == pListRealCodes->GetAt(pos).m_strCode)
			{
				if(m_pDrdsLib && pos)
				{
//@SolomonGNF 					m_pDrdsLib->DrdsUnadvise(0, "", "", (LPSTR)(LPCSTR)strCode, m_hWnd);
					pListRealCodes->RemoveAt(pos);
				}
				break;
			}
		}
	}
	else
	{
		for(int nIndx = 0; nIndx < nCount; ++nIndx)
		{
			pos = pListRealCodes->FindIndex(nIndx);
			if(m_pDrdsLib && pos)
			{
//@SolomonGNF 				m_pDrdsLib->DrdsUnadvise(0, "", "", (LPSTR)(LPCSTR)pListRealCodes->GetAt(pos).m_strCode, m_hWnd);
				pListRealCodes->RemoveAt(pos);
			}
		}
		pListRealCodes->RemoveAll();
	}
}

BOOL CStdDialog::MakeDrdsUnConnectUnAdviseFromCode(int nI, BOOL bAddMod)
{
	if(!m_pDrdsLib) return FALSE;

	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
	if(!pDrdsMng) return FALSE;

	if(m_pDrdsLib == NULL)
		return TRUE;

	char* DataName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].DataName;
	char* FieldName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].FieldName;
	// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
	CString KeyName = GetRealItemCode( m_DrdsInfo[nI].KeyName);
	UINT nID = m_DrdsInfo[nI].nID;
/*
	if(m_RealTrCounter.RemoveTr( KeyName) == 0)
	{
		//<<20100316_JS.Kim �ַθ�
		//m_pDrdsLib->DrdsUnadvise(nID, DataName, FieldName, ( char *)( const char *)KeyName);
		//>>
		m_DrdsInfo[nI].bAdvise = FALSE;
	}

	//{{2007.01.02 by LYH ������
	if(nI == 0)
		RemoveRealCodes(&m_ListStockRealCodes, m_DrdsInfo[nI].KeyName);
	//}}
*/
	if(m_RealTrCounter.RemoveTr(m_DrdsInfo[nI].KeyName) == 0)
	{
		//@Solomon
		//m_pDrdsLib->DrdsUnadvise(nID, DataName, FieldName, KeyName);
		if(m_pPTManager) m_pPTManager->RequestMChartReal('U', DataName, KeyName);
		m_DrdsInfo[nI].bAdvise = FALSE;
	}

	return TRUE;
}

// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
void CStdDialog::OnCodeUnAdvise(CString strCode, BOOL bAddMode)
{
	// ���� �ڵ尡 ����Ʈ�� ������ ����
	// �������� ������ : �������� - ojtaso (20070208)
	int nType;
	int nMarketType = GetMarketTypeFromCode(strCode, nType);
	if(nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
// --> [Edit]  ������ 2008/10/29
//		m_DrdsInfo[0].KeyName = strCode;
//		MakeDrdsUnConnectUnAdviseFromCode(0, bAddMode);								
		CChartItem*	pChartItem = m_ChartMng.GetpChartItemSelected();
		if ( pChartItem )
		{
			if ( m_ChartMng.GetpChartItemSelected()->GetCheHogaChange() == 0 )
			{
				m_DrdsInfo[0].KeyName = strCode;
				MakeDrdsUnConnectUnAdviseFromCode(0, bAddMode);								
			}
			else
			{
				m_DrdsInfo[1].KeyName = strCode;
				MakeDrdsUnConnectUnAdviseFromCode(1, bAddMode);								
			}
		}
		else
		{
			m_DrdsInfo[0].KeyName = strCode;
			MakeDrdsUnConnectUnAdviseFromCode(0, bAddMode);								
			m_DrdsInfo[1].KeyName = strCode;
			MakeDrdsUnConnectUnAdviseFromCode(1, bAddMode);								
		}

		// �������� ������ : �������� - ojtaso (20070208)
// <-- [Edit]  ������ 2008/10/29
	}
	else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		m_DrdsInfo[1].KeyName = strCode;
		MakeDrdsUnConnectUnAdviseFromCode(1, bAddMode);
		// �������� ������ : �������� - ojtaso (20070208)
	}
	else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
	{
		if(strCode[0]=='1'||strCode[0]=='4')
		{
			m_DrdsInfo[2].KeyName = strCode;
			MakeDrdsUnConnectUnAdviseFromCode(2, bAddMode);								
			// �������� ������ : �������� - ojtaso (20070208)
		}
		else
		{
			m_DrdsInfo[3].KeyName = strCode;
			MakeDrdsUnConnectUnAdviseFromCode(3, bAddMode);								
			// �������� ������ : �������� - ojtaso (20070208)
		}
	}
	else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
		MakeDrdsUnConnectUnAdviseFromCode(4, bAddMode);								
	}
	else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		m_DrdsInfo[6].KeyName = strCode;
		MakeDrdsUnConnectUnAdviseFromCode(6, bAddMode);								
		// �������� ������ : �������� - ojtaso (20070208)
	}
}

// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
void CStdDialog::OnCodeAdvise(CString strCode, BOOL bAddMode)
{
	// Drds ���� 
	// ���� �ڵ尡 ����Ʈ�� ������ �߰�
	// �������� ������ : �������� - ojtaso (20070208)
	int nType;
	int nMarketType = GetMarketTypeFromCode(strCode, nType);
	if(nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
// --> [Edit]  ������ 2008/10/29
//		m_DrdsInfo[0].KeyName = strCode;
//		MakeDrdsConnectAdviseFromCode(0, bAddMode);
		if ( m_ChartMng.GetpChartItemSelected()->GetCheHogaChange() == 0 )
		{
			m_DrdsInfo[0].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(0, bAddMode);
		}
		else
		{
			m_DrdsInfo[1].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(1, bAddMode);
		}
// <-- [Edit]  ������ 2008/10/29
	}
	else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		m_DrdsInfo[1].KeyName = strCode;
		MakeDrdsConnectAdviseFromCode(1, bAddMode);
	}
	else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
	{
		if(strCode[0]=='1'||strCode[0]=='4')
		{
			m_DrdsInfo[2].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(2, bAddMode);
		}
		else
		{
			m_DrdsInfo[3].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(3, bAddMode);
		}
	}
	else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
		MakeDrdsConnectAdviseFromCode(4, bAddMode);
	}
	else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		m_DrdsInfo[6].KeyName = strCode;
		MakeDrdsConnectAdviseFromCode(6, bAddMode);
	}

}

BOOL CStdDialog::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)  
	{		
		UINT nChar = (UINT)pMsg->wParam;
		switch(nChar)
		{
		case VK_RETURN :
		case VK_ESCAPE :
			{
				return FALSE;
				break;					
			}
		}
	}
	else if(pMsg->message == WM_SYSKEYDOWN)
	{
		if( ::GetKeyState(VK_MENU) & 0x8000)
			return FALSE;
	}
	else if(pMsg->message == WM_LBUTTONDOWN ||
			pMsg->message == WM_LBUTTONUP ||
			pMsg->message == WM_LBUTTONDBLCLK||
			pMsg->message == WM_RBUTTONDOWN ||
			pMsg->message == WM_RBUTTONUP ||
			pMsg->message == WM_RBUTTONDBLCLK ||
			pMsg->message == WM_MOUSEMOVE)
	{
		if(m_pDlgGlass && m_pDlgGlass->IsWindowVisible())
		{
			ScreenToClient( &pMsg->pt);

			CRect rcGlass;
			m_pDlgGlass->GetWindowRect( &rcGlass);
			ScreenToClient( &rcGlass);
			if(!rcGlass.PtInRect(pMsg->pt))
				return TRUE;
		}
	}

	return CExOCXDLL::PreTranslateMessage(pMsg);
}

void CStdDialog::OnSize(UINT nType, int cx, int cy) 
{
	CExOCXDLL::OnSize(nType, cx, cy);
	
	if(GetSafeHwnd())
	{
		Resize();			
	}	
}

void CStdDialog::SetSmallView(BOOL bSmall /*= TRUE*/)
{
	if(bSmall)
	{
		if(m_pInputBarDlg)
			m_pInputBarDlg->HideLeftBarBtn(SW_HIDE);
		m_nTempLeftBarType = GetLeftBarType();
		SetLeftBarType(0);
		m_bSmallScreenMode = TRUE;
	}
	else
	{
		if(m_pInputBarDlg)
			m_pInputBarDlg->HideLeftBarBtn(SW_SHOW);
		SetLeftBarType(m_nTempLeftBarType);
		m_bSmallScreenMode = FALSE;
	}
}


void CStdDialog::InputCodeFromOutSide(CString strPacketFromOutSide)
{
	if(GetLeftBarType()==0)
	{
		SetLeftBarType(1);
	}
	m_pLeftBarDlg->SetPacketFromOutSide(strPacketFromOutSide);
}

void CStdDialog::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TIMER_EVENTID_FORMLOAD)
	{
		KillTimer(nIDEvent);
		_DoFirstRequest();
	}
	else
		KillTimer(nIDEvent);

	CDialog::OnTimer(nIDEvent);
}

HRESULT CStdDialog::RecieveError(int nError, LPCSTR szMessage)
{
	MessageBox(szMessage);
	return S_OK;
}

CString CStdDialog::GetMapID()
{
	return m_strMapID;
}


BOOL CStdDialog::IsChartHide()
{
	return m_bHideChart;
}

BOOL CStdDialog::HideChart()
{
	m_bHideChart=!m_bHideChart;
	
	CString strSize;
	CRect rect;
	CRect rectOriginal;
	if(m_bHideChart)
	{
		GetClientRect(rectOriginal);
		m_lWidthOriginal = rectOriginal.Width()+1;
		m_lHeightOriginal = rectOriginal.Height()+1;
		m_pLeftBarDlg->GetClientRect(rect);
		strSize.Format("%d,%d,",rect.Width(),rect.Height());
	}
	else
	{
		if(!m_lWidthOriginal||!m_lHeightOriginal)
		{
			m_lWidthOriginal = m_nWidthOfDlg;
			m_lHeightOriginal = m_nHeightOfDlg;
		}
		strSize.Format("%d,%d,",m_lWidthOriginal,m_lHeightOriginal);
	}	
	CWnd* pWnd = GetParent();	
	if (pWnd)// && m_pBaseDesk)
	{
		//ȭ�鿡 Data�� �����ش�.
		CString  csData;
		csData.Format("Dim szText, szValue\n" "szText=\"%s\"\n" "szValue=\"%s\"\n", "LOCAL_RESIZE", strSize);
	}
	return m_bHideChart;
}

void	CStdDialog::SetJongListPrev()
{
	CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
	if(pTabCodeSearch)
	{
		pTabCodeSearch->OnBtprev();
	}	
}

void	CStdDialog::SetJongListNext()
{
	CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
	if(pTabCodeSearch)
	{
		pTabCodeSearch->OnBtnext();
	}	
}

CString CStdDialog::GetAccountInfo() 
{ 
	return m_strAccountInfo;
}

BOOL CStdDialog::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(!m_bEnable)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

/****************************************************************************/
/* Function : �ʱ� ���� �ʱ�ȭ												*/
/* Paramter :																*/
/* Return	:																*/
/* Author	: By DRFN														*/
/* Comments	: FX Margin �ϸ鼭 �ҽ� ���� (�ѱ��Ҹ�ġ �����)				*/
/****************************************************************************/
void CStdDialog::SetInitInfo()
{
	int nID = 21;

	IGateManager* pGateMng;
	AfxGetIGateManager(pGateMng);
	if(pGateMng == NULL)
		return;

	IMainInfoManager* pManager = (IMainInfoManager*)pGateMng->GetInterface(nID);

	m_strUserID = pManager->GetEncUserID();					// ����� ID ��� ����
	if(m_strUserID.GetLength() == 0)
	{
		AfxMessageBox("UserID�� ���� �� �����ϴ�.");
	}

	m_strRootPath = pManager->GetRootDir();					// Root Path��� ����
	if(m_strRootPath.GetLength()==0)
	{
		AfxMessageBox("RootPath�� ���� �� �����ϴ�.");
	}

	m_strUserDir.Format("%s%s", pManager->GetEncUserDir(), "\\Chart");

// --> [Edit]  ������ 2008/10/14	( �̹��� ��� ���� )
//	m_szImageDir = m_strRootPath + "\\system\\Image\\Chart";		// �̹��� Path ����
	m_szImageDir = m_strRootPath + "\\Icss\\Image\\drchart";		// �̹��� Path ����
// <-- [Edit]  ������ 2008/10/14

	// (2008/9/13 - Seung-Won, Bae) for Multi-Language
	g_iMetaTable.m_strDataFolderPath.Format(_T("%s\\"), pManager->GetDataDir());
	g_iMetaTable.LoadMetaTable();
}

//#============================== TR ���� Test ==========================
//* ȣ����ȸ
#define TR_HOGA_FUTURE "HWS10010" 
void CStdDialog::SendTR_TEST(CString strRecvCode)
{	
	strRecvCode = "10156000";
	if(m_TRComm)
	{
		m_TRComm->SetTR(TR_HOGA_FUTURE);
		m_TRComm->Send2Server((LPVOID)(LPCSTR)strRecvCode, strRecvCode.GetLength(), FALSE);
	}
}
//#============================== TR ���� Test ==========================



void CStdDialog::GetIndicatorNameListIntoComboBox(CComboBox& rComboBox,LPCTSTR p_strGroup)
{
#ifdef _DEBUG
	VERIFY(m_pLeftBarDlg);
	VERIFY(m_pLeftBarDlg->GetSafeHwnd());
	VERIFY(rComboBox.GetSafeHwnd());
#endif
	if(m_pLeftBarDlg)
	{
		if(m_pLeftBarDlg->GetSafeHwnd())
		{
			m_pLeftBarDlg->GetIndicatorNameListIntoComboBox(rComboBox, p_strGroup);
		}		
	}	
}

#include <atlconv.h>
void CStdDialog::ShowDataListView(BOOL bShowDataListView, CRect ViewRect)
{
	if(bShowDataListView)
	{	
		//Ȥ�� Window�� �ִٸ� �����.
		DestroyDataWindow();		

		if(m_pDataWindow != NULL)				
			m_pDataWindow = NULL;	

		if(m_pDataWindow == NULL)
		{
			m_pDataWindow = new CDataWindowDlg();
			m_pDataWindow->CreateDataWindow();
			m_pDataWindow->SetParent((CStdDialog*)this);
			m_pDataWindow->MoveWindow(&m_ViewRect);
			m_pDataWindow->ShowWindow(SW_SHOW);
		}
		m_bActiveDataWindow = true;
	}
	else
	{
		DestroyDataWindow();
	}
}

void CStdDialog::SetRealInfo(
									void* pDrds		,   // Drds�������ε� NULL�� ����.
									HWND hWnd		,   // Chart OCX�� ������ �ڵ�
									UINT id			, 	// CPacket* 
									LPSTR DataName	,	// ex: "S31"
									LPSTR FieldName	, 	// ex: "time"
									LPSTR KeyName	)	// ex: "000660"
{	
	if(!CString(DataName).GetLength()) return;
	if(!CString(FieldName).GetLength()) return;
	if(!CString(FieldName).GetLength()) return;
}

void CStdDialog::UnSetRealInfo(
									void* pDrds		,   // Drds�������ε� NULL�� ����.
									HWND hWnd		,   // Chart OCX�� ������ �ڵ�
									UINT id			, 	// CPacket* 
									LPSTR DataName	,	// ex: "S31"
									LPSTR FieldName	, 	// ex: "time"
									LPSTR KeyName	)	// ex: "000660"
{
}


void CStdDialog::SendStockReal(char* pData)
{
}

void CStdDialog::SendUpjongReal(char* pData)
{
}

void CStdDialog::SendFutureReal(char* pData)
{
}

void CStdDialog::SendOptionReal(char* pData)
{
}


static int ConvertToChartSign(char* pBuff)
{	
	CString sTemp;
	sTemp.Format("%1.1s", pBuff);
	int nSign = 0;
	nSign = atoi((LPTSTR)(LPCTSTR)sTemp);

	int nResult = 0;
	switch(nSign)
	{
		case 1:		//����
			nResult =  7; break;
		case 2:		//���
			nResult =  6; break;
		case 3:		//����
			nResult =  5; break;
		case 4:		//����
			nResult =  3; break;
		case 5:		//�϶�
			nResult =  4; break;
	}
	char buff[1];
	itoa(nResult, buff, 10);
	memcpy(pBuff, buff, 1);	

	return 0;
}

void CStdDialog::RequestDollarData()
{
	CString strFidInfo("0                                                                               ");
	CString strTrInfo;
	strTrInfo.Format("%-10s", "FX@KRW");
	CString strRecordInfo("1               000;");
	CString strOutput;
	strOutput = strFidInfo + strTrInfo + strRecordInfo;

	stSend2Server st;
	st.m_nType = 0;
	st.m_pWnd = (CWnd*)this;
	st.m_pBuffer = (void*)(LPCTSTR)strOutput;
	st.m_lLength= strOutput.GetLength();
	st.m_strTRCode = QUERY_STRDOLLAR;	
	m_TRComm->SetTR(QUERY_STRDOLLAR);
	m_TRComm->SetBaseData("PtManager", m_szPTManager);
	m_TRComm->SetBaseData("SCREENCODE", m_strScreenNo);
	m_TRComm->Send2Server(st.m_pBuffer, st.m_lLength, FALSE);	
}

void CStdDialog::Request_StockHogaData(CString strRecvCode)
{
	if ((m_ChartMng.m_pFloatingDlg == NULL) ||
		(m_ChartMng.m_pFloatingDlg->GetSafeHwnd() == NULL) ||
		(m_ChartMng.m_pFloatingDlg->m_bIconState) ||			//��� ���¸� TR��ȸ ���Ѵ�.
		(m_pInputBarDlg == NULL) ||
		(m_pInputBarDlg->GetSafeHwnd() == NULL))
	{
		return;
	}

	CString strCode;
	if (strRecvCode.GetLength() == CODE_STOCK_LENGTH)
		strCode = strRecvCode;
	else
		strCode = m_pInputBarDlg->GetTypedItemCode();

	if (!strCode.GetLength())	// ȣ��â Data Clear & Real ����
	{
		Request_Disconnect_StockRealHogaData(m_pInputBarDlg->m_strOldCode);
		return;
	}

	if (m_ChartMng.m_pFloatingDlg != NULL)
	{
		if (m_ChartMng.m_pFloatingDlg->GetSafeHwnd())
		{	
			if (m_pInputBarDlg->m_strOldCode != strCode)
				Request_Disconnect_StockRealHogaData(m_pInputBarDlg->m_strOldCode);

			strCode = strCode.Mid(3);
			strCode = CStdDialog::GetRealItemCode( strCode, TRUE);
			if (strCode.GetLength() < SIMBOL_CODE_MIN_LENGTH)
				return;

			char stJMCode[HCQ01112_IN_SZ];
			memset(&stJMCode, 0x00, HCQ01112_IN_SZ);			
			memcpy(stJMCode, strCode, strCode.GetLength());		

			CString strOutput;
			strOutput.Format("%-*s", HCQ01112_IN_SZ, strCode);

			stSend2Server st;
			st.m_nType = 0;
			st.m_pWnd = (CWnd*)m_ChartMng.m_pFloatingDlg;
			st.m_pBuffer = (void*)(LPCTSTR)strOutput;
			st.m_lLength= HCQ01112_IN_SZ;
			st.m_strTRCode = QUERY_STRFOFOREIGN_HOGA;
			SendMessage(UMSG_SEND2SERVER,0,(long)&st);

			Request_StockRealHogaData(strCode);
		}
	}
}

void CStdDialog::Request_StockRealHogaData(CString strCode)
{
	// �ֽ� Real Hoga ��û ====================================================================
	if(strCode.GetLength())
	{
		//if(m_pDBMgr)
		{				
			strCode.TrimLeft();
			strCode.TrimRight();
			m_DrdsInfo[2].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(2, FALSE);		// �������� ������ : �������� - ojtaso (20070208)
		}
	}
}

void CStdDialog::Request_Disconnect_StockRealHogaData(CString strCode)
{
#ifdef _WRITELOG
	WriteToStringLog("CStdDialog::Request_Disconnect_StockRealHogaData::Begin", _T("��Ƽ��Ʈ ����"), 13);
#endif
	if(m_ChartMng.m_pFloatingDlg != NULL)
	{
		if(m_ChartMng.m_pFloatingDlg->GetSafeHwnd())
		{	
			// Real ���� ====================================================================
			if(strCode.GetLength())
			{
				m_ChartMng.m_pFloatingDlg->m_strCode.TrimLeft();
				m_ChartMng.m_pFloatingDlg->m_strCode.TrimRight();
				m_DrdsInfo[2].KeyName = m_ChartMng.m_pFloatingDlg->m_strCode;
				MakeDrdsUnConnectUnAdviseFromCode(2, FALSE);		// �������� ������ : �������� - ojtaso (20070208)
			}
			// Real ���� ====================================================================
		}
	}
#ifdef _WRITELOG
	WriteToStringLog("CStdDialog::Request_Disconnect_StockRealHogaData::End", _T("��Ƽ��Ʈ ����"), 13);
#endif
}

void CStdDialog::CreateDataWindow()
{
	if(m_pDataWindow == NULL)
	{
		m_pDataWindow = new CDataWindowDlg();
		m_pDataWindow->CreateDataWindow();
	}
	m_bActiveDataWindow = true;
}

void CStdDialog::DestroyDataWindow()
{
	if(m_pDataWindow)
	{
		
		m_pDataWindow->DestroyWindow();
		delete m_pDataWindow;
	}
	m_pDataWindow = NULL;
	m_bActiveDataWindow = false;

}

bool CStdDialog::IsActiveDataWindow()
{
	return m_bActiveDataWindow;
}

void CStdDialog::SetDataWindowData(CList<CString, CString&>* pListData)
{
	m_pDataWindow->SetDataWindowData(pListData);
}

BOOL CStdDialog::ChangedChartItem()
{
	return FALSE;	
}

/// gm0604 Start 
void CStdDialog::InitChartBySystemTrading(LPCTSTR lpMessage)
{
	CString strMessage = lpMessage;
	CString strInfo;
	CString strTimeType;
	CString strDataCount;
	long lDataCount = 0;

	int nRow = 0;
	int nCol = 0;
	CString strInputCode;
	CString strEndDate;
	CString strPtrST;
	CString strName;


	// col, row, inputcode, enddate, timetype, datacnt,....
	int nShifter = 0;
	m_strInputCode = Parser(strMessage,&nShifter);
	m_strEndDate = Parser(strMessage,&nShifter);

	while(TRUE)
	{
		strTimeType = Parser(strMessage,&nShifter);
		strDataCount = Parser(strMessage,&nShifter);
		strPtrST = Parser(strMessage,&nShifter);
		strName = Parser(strMessage,&nShifter);
		if(strTimeType!=""&&strDataCount!=""&&strPtrST!=""&&strName!="")
		{
			m_stringListTimeType.AddTail(strTimeType);
			m_longListDataCount.AddTail(atol(strDataCount));
			m_ptrListSystemTrading.AddTail(atol(strPtrST));
			m_stringListSTName.AddTail(strName);
		}
		else
		{
			break;
		}
	}		
	m_ChartMng.SetSystemTradingMode(TRUE);
	SetChartBySystemTrading(TRUE);
}


void CStdDialog::SetChartBySystemTrading(BOOL bIsFirst)
{
	if(m_stringListTimeType.GetCount()==0)
	{		
		m_ChartMng.Invalidate();
		return;
	}

	if(!m_ChartMng.SelectEmptyPos())	
	{
		if(!m_ChartMng.InsertChart())
		{
			return ;
		}
		m_ChartMng.SelectEmptyPos();		
	}
	
	CString strTimeType = m_stringListTimeType.GetHead();
	m_stringListTimeType.RemoveHead();
	long lDataCount = m_longListDataCount.GetHead();
	m_longListDataCount.RemoveHead();
	long lPtrST = m_ptrListSystemTrading.GetHead();
	m_ptrListSystemTrading.RemoveHead();
	CString strSTName = m_stringListSTName.GetHead();
	m_stringListSTName.RemoveHead();


	// ƽ(0)/��(1)/��(2)/��(3)/��(4)
	CString strNum;
	CString strType;
	char chTimeType = NULL;
	int nLength = strTimeType.GetLength();
	if(nLength==2)
	{
		if(strTimeType=="��")
		{
			chTimeType = MONTH_DATA_CHART;
		}
		else if(strTimeType=="��")
		{
			chTimeType = WEEK_DATA_CHART;
		}
		else if(strTimeType=="��")
		{
			chTimeType = DAY_DATA_CHART;
		}
	}
	else
	{
		strType = strTimeType.Right(2);
		strNum = strTimeType.Left(nLength-2);
		if(strType=="��")
		{
			chTimeType = MIN_DATA_CHART;
		}
		else if(strType=="ƽ")
		{
			chTimeType = TICK_DATA_CHART;
		}
	}
	CCodeDataFromInputBar CodeData;
	CodeData.m_strCode = m_strInputCode;
	CodeData.m_lEndTime = atol(m_strEndDate);
	CodeData.m_chType = chTimeType;
	CodeData.m_lTypeNum = atol(strNum);
	CodeData.m_lDataCnt = lDataCount;
	CodeData.m_lPtrST = lPtrST;
	CodeData.m_strSTName = strSTName;
	m_ChartMng.SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);
	m_ChartMng.MoveCharts();
}

long CStdDialog::ChangeSharedData(CString sShareName, BYTE* pData, int nLen, int nStock/*=0*/)
{
	if(sShareName==FROMSTCENTER)
	{
		CString strData = (LPCTSTR)pData;
		if(strData=="#") return 0;
		CString strMessage = strData.Mid(3);
		int nCode = atol(strData.Left(3));

		switch(nCode)
		{
		case 2:	// Update SystemTrading Info
			{
				m_ChartMng.OnSTDataManagerMessaggeAll();
			}
			break;
		case 3:// Close MultiChart
			{
				m_ChartMng.SetSystemTradingMode(FALSE,strMessage);
			}
			break;
		}
	}
	else if(sShareName=="�����ڵ�")
	{
		AfxMessageBox("�����ڵ� ����");
	}
	return 0;
}
/// gm0604 end

void CStdDialog::Send2HanHwa_09501(void* pData, int nLen, CWnd* pRequestWnd, CString strTRCode)
{
	EnableWindowAll(FALSE);	

	SendMessage2StatusBar(MSG_GOING);
	m_pRqWndFromTab = NULL;
	m_pRqWndFromTab = pRequestWnd;
}

#undef OBJECT_SELECT
#include "../chart_common/BlockBaseEnum.h"


LONG CStdDialog::OnByPass_ExportToolOption(UINT wParam, LPARAM lParam)
{	
	if(wParam == CToolOptionInfo::T_RESTORE
		|| wParam == CToolOptionInfo::T_DATALIST_REPORT
		|| wParam == CToolOptionInfo::T_DELETE_INORDER
		|| wParam == CToolOptionInfo::T_DELETE_ALL
		|| wParam == CToolOptionInfo::T_PRICE_YSCALE_SETUP
		|| wParam == CToolOptionInfo::T_UNDO_WORK
		|| wParam == CToolOptionInfo::T_REDO_WORK)
	{
		CWnd* pWnd = (CWnd*)m_ChartMng.GetpChartItemSelected();
		if(pWnd)
		{
			//========================================================================
			// �ʱ�ȭ ��ư�� ������ ��� �ش� ��Ʈ�� ������ ���� ���� 
			// �ʱ�ȭ �޼����� OCX�� ������ �ʴ´�.
			// 05.11.24
			//========================================================================
			if(wParam == CToolOptionInfo::T_RESTORE)
			{
				if(m_ChartMng.GetSafeHwnd())
					if( !m_ChartMng.SendMessage(UMSG_EXT_TOOL_RESTORE)) //������ ���� ���� 
						return 1;										//�ʱ�ȭ�� ��Ű�� �ʴ´�.
			}
			//========================================================================


			pWnd->SendMessage(RMS_ANALYSIS_FUNC, wParam, 1);	
		}
		return 0L;
	}

	for(int nRowIndex=(m_ChartMng.GetRows()-1);nRowIndex>=0;nRowIndex--)
	{
		for(int nColIndex=(m_ChartMng.GetCols()-1);nColIndex>=0;nColIndex--)
		{
			CWnd* pWnd = (CWnd*)m_ChartMng.GetChartItem(nRowIndex, nColIndex);
			if(pWnd)
			{
				//========================================================================
				// �ܺ����� �� �����Ǹ鼭... 
				// �����ڵ��� ��Ŀ���� �������� ������ ���ܼ� 
				// lParam�� 9999�� ������
				// ���� �Է�â�� ��Ŀ���� ������ �Ѵ�. 05.11.15
				//========================================================================
				if(lParam == 9999)
					if(m_pInputBarDlg)
					{
						m_pInputBarDlg->SendMessage(UMSG_EDIT_FOCUS);
						return 1;
					}
				//========================================================================

				//========================================================================
				// �ʱ�ȭ ��ư�� ������ ��� �ش� ��Ʈ�� ������ ���� ���� 
				// �ʱ�ȭ �޼����� OCX�� ������ �ʴ´�.
				// 05.11.24
				//========================================================================
				if(wParam == CToolOptionInfo::T_RESTORE)
				{
					if(m_ChartMng.GetSafeHwnd())
						if( !m_ChartMng.SendMessage(UMSG_EXT_TOOL_RESTORE)) //������ ���� ���� 
							return 1;										//�ʱ�ȭ�� ��Ű�� �ʴ´�.
				}
				//========================================================================


				pWnd->SendMessage(RMS_ANALYSIS_FUNC, wParam, lParam);	
			}
		}
	}
	return 0L;
}

void CStdDialog::SendMessage2StatusBar(int nMsgType, const char* pMsg, int nNum /*=0*/)
{
//100618-alzioyes: �� ��ƾ�� ���ϴ� �ž�?
// 	CString strMessage;
// 	switch(nMsgType)
// 	{
// 		case	MSG_GOING:
// 			strMessage.Format("ó����...");
// 			break;
// 		default:
// 			if( lstrlen(pMsg) )
// 				strMessage.Format("%s", pMsg);
// 			break;
// 	}
}


void CStdDialog::Send2HanHwa_IFCond(void* pData, int nLen, CWnd* pRequestWnd, CString strTRCode)
{
	EnableWindowAll(FALSE);	

	SendMessage2StatusBar(MSG_GOING);
	m_pRqWndFromTab = NULL;
	m_pRqWndFromTab = pRequestWnd;
}


BOOL CStdDialog::Load_ReplayChart_Lib()
{	
	//100618-alzioyes : �ؿܴ� ���� �����ݾ�.
	return FALSE;

	BOOL bResult  = FALSE;

	CString strFileRoot;
	strFileRoot = m_strRootPath + "\\dev\\GNF_ReplayChart.dll";

	m_hReplayChartLib = NULL;
	m_hReplayChartLib = LoadLibrary(strFileRoot);
	
	if (m_hReplayChartLib)
	{
		_fnReplayDll_Load			= (_DLL_fnReplayDll_Load		)::GetProcAddress(m_hReplayChartLib, "fnReplayDll_Load"			);
		_fnReplayDll_UnLoad			= (_DLL_fnReplayDll_Unload		)::GetProcAddress(m_hReplayChartLib, "fnReplayDll_Unload"		);
		_fnReplayDll_SetInitInfo	= (_DLL_fnReplayDll_SetInitInfo	)::GetProcAddress(m_hReplayChartLib, "fnReplayDll_SetInitInfo"	);
		_fnReplayDll_Run			= (_DLL_fnReplayDll_Run			)::GetProcAddress(m_hReplayChartLib, "fnReplayDll_Run"			);				
		_fnReplayDll_ChangeSpeed	= (_DLL_fnReplayDll_ChangeSpeed	)::GetProcAddress(m_hReplayChartLib, "fnReplayDll_ChangeSpeed"	);

		m_pWndReplayLib = _fnReplayDll_Load(GetSafeHwnd());
		bResult = TRUE;
	}
	else
	{	
#ifdef _DEBUG
		AfxMessageBox("\n# GNF_ReplayChart.dll�� �����ϴ�.");
#endif
		m_pWndReplayLib = NULL;
		bResult = FALSE;
	}

	return bResult;
}

BOOL CStdDialog::UnLoad_ReplayChart_Lib()
{		
	if(m_hReplayChartLib != NULL)
	{
		if(m_pWndReplayLib)
			_fnReplayDll_UnLoad(m_pWndReplayLib);
		
		if(FreeLibrary(m_hReplayChartLib))
			m_hReplayChartLib = NULL;

		return TRUE;
	}

	return FALSE;
}

BOOL CStdDialog::RunReplay(int nRun, REPLAY_INFO* pReplayInfo) 
{	
	if(m_pWndReplayLib && m_hReplayChartLib)
	{
		switch(nRun)
		{			
			case PLAY:
				{						
					m_pExtInputBarDlg->EnableWindow(FALSE);
				
					if(pReplayInfo != NULL)
					{
						pReplayInfo->hCallHwnd		= this->m_hWnd;
						pReplayInfo->hRecvMainFrame = m_hMainFrame;
						m_strSpeed.Format("%d",		pReplayInfo->nSpeed);
						m_ChartMng.GetChartAllCode(pReplayInfo->arrStrCode);
						if( pReplayInfo->arrStrCode.GetSize() <= 0)
						{
							AfxMessageBox("���⸦ ������ ������ �����ϴ�.\n�����ɼ� ��Ʈ���� ���Ⱑ ������ ������\n���������ɼ� �����Դϴ�.");
							return FALSE;
						}	
						
						_fnReplayDll_SetInitInfo(m_pWndReplayLib, (void*)pReplayInfo);
						return TRUE;
					}				
				}
				break;
			case STOP:
				{
					m_strSpeed = "1";
					SetBNeedRqstTRForReplay(TRUE);	//Stop ��ư�� ������ ����.. �÷��̸� ��� �ѹ��� �����ϰ� 
													//���ȴٰ� ���� 
				}
				break;
		}

		_fnReplayDll_Run(m_pWndReplayLib, nRun);

		return TRUE;
	}
	return FALSE;
}

void CStdDialog::ChangeSpeed(int nSpeed)
{
	m_strSpeed.Format("%d",					  nSpeed);
	_fnReplayDll_ChangeSpeed(m_pWndReplayLib, nSpeed);
}

LPCTSTR CStdDialog::GetMainPath()
{
	return m_strRootPath;
}
LPCTSTR CStdDialog::GetUserID()
{
	return m_strUserID;
}

// ���������.
// (2008/11/2 - Seung-Won, Bae) for Gap Revision.
bool CStdDialog::SetImaginaryCandleAndGapRevisionToOutside( const bool bImaginaryCandle, const bool bUseGapRevision)
{ 
// (2008/11/2 - Seung-Won, Bae) Do not support sync for all ChartItem.
//	m_ChartCfgInfo.bImaginaryCandle	= bImaginaryCandle; // TRUE �� ���
//	m_ChartCfgInfo.bUseGapRevision	= bUseGapRevision;	// TRUE �� ������
//	m_pExtInputBarDlg->SendMessage( UMSG_SETNORMALIZEDMINUTETIMERULE, bImaginaryCandle, bUseGapRevision);	

	// (2008/11/2 - Seung-Won, Bae) Set Minute data for Normalized Time or Gap Revision.
	m_ChartMng.SendMessage( UMSG_SET_NORMALIZEDMINUTETIMERULE_AND_GAPREVISION, bImaginaryCandle, bUseGapRevision);

	return TRUE;
}	

// ��/���Ѱ� ǥ��
bool CStdDialog::SetShowHighLowBoundToOutside(const bool bShow)	
{ 
	m_ChartCfgInfo.bShowHLBar		= bShow				;
	m_ChartMng.SetEnableBoundMarkOnlyOne(bShow);
	return TRUE;
}	

// �Ǹ���/���� ǥ��
bool CStdDialog::SetShowExRightDividendToOutside(const bool bShow)
{ 
	m_ChartCfgInfo.bShowRight		= bShow;
	
	if(m_ChartMng.GetpChartItemSelected()->GetType()=='2')
	{
		m_ChartMng.SetShowExRightOnlyOne(bShow);
	}
	else
	{
		m_ChartMng.SetShowExRightOnlyOne(FALSE);
	}
	
	return TRUE;
}	

// �ð����� ����
bool CStdDialog::SetTimeLinkageToOutside(const bool bTimeLinkage)
{ 
	m_ChartCfgInfo.bRecvTimeShare	= bTimeLinkage;		
	m_ChartMng.SetTimeShareOnlyOne(bTimeLinkage);
	return TRUE;
}	

// ��Ʈ����� �ڵ� ���� ����
bool CStdDialog::SetAutoSaveToOutside(const bool bAutoSave)
{	
	m_ChartCfgInfo.bAutoSave		= bAutoSave;
	m_ChartMng.SetAutoSaveToOutside(bAutoSave);
	return TRUE;
}	

// ������Ʈ log ����.
bool CStdDialog::SetPriceLog(const bool bLog)
{
	m_ChartCfgInfo.bShowLog		= bLog;
	m_ChartMng.SetPriceLog(bLog);
	return TRUE;
}

bool CStdDialog::OnChartEnvSetupOk()
{
	m_ChartMng.OnChartEnvSetupOK();
	return TRUE;
}


bool CStdDialog::GetImaginaryCandleToOutside(bool& bImaginaryCandle		)	// ���������.
{
	return m_ChartMng.GetImaginaryCandleToOutside(bImaginaryCandle);
}
// (2008/11/2 - Seung-Won, Bae) for Gap Revision.
bool CStdDialog::GetUseGapRevisionToOutside(bool& bUseGapRevision)	// ������
{
	return m_ChartMng.GetUseGapRevisionToOutside(bUseGapRevision);
}

bool CStdDialog::GetShowHighLowBoundToOutside	(bool& bShow				)	// ��/���Ѱ� ǥ��
{
	return m_ChartMng.GetShowHighLowBoundToOutside(bShow);
}

bool CStdDialog::GetShowExRightDividendToOutside	(bool& bShow				)	// �Ǹ���/���� ǥ��
{
	return m_ChartMng.GetShowExRightDividendToOutside(bShow);
}

bool CStdDialog::GetTimeLinkageToOutside			(bool& 	bTimeLinkage		)	// �ð����� ����
{
	return m_ChartMng.GetTimeLinkageToOutside(bTimeLinkage);
}

bool CStdDialog::GetAutoSaveToOutside			(bool& bAutoSave			)	// ��Ʈ����� �ڵ� ���� ����
{
	return m_ChartMng.GetAutoSaveToOutside(bAutoSave);
}

void CStdDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);
}

void CStdDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rcExtTool;
	dc.GetClipBox(rcExtTool);
	dc.FillSolidRect(&rcExtTool, GetBkgrndColor());
	
	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.bottom = rcClient.top + 2;
	dc.FillSolidRect(rcClient, RGB(255,255,255));

	// ������ ���� - ojtaso (20080519)
//	if(m_pDlgGlass && m_pDlgGlass->IsWindowVisible())
//	{
//		CRect rcBounds;
//		GetWindowRect(rcBounds);
//		ScreenToClient(rcBounds);
//		
//		m_memoryDC.CreateCompatibleDC(&dc);
//		CreateBufferBitmap(&dc, rcBounds);
//		CBitmap* pOldbm = m_memoryDC.SelectObject(&m_bufferBitmap);
//		
//		CRect rcMain = rcBounds;
//		dc.StretchBlt(rcMain.left, rcMain.top, rcMain.Width(), rcMain.Height(), &m_memoryDC, 
//			0, 0, rcMain.Width(), rcMain.Height(), SRCCOPY) ;
//		
//		m_pDlgGlass->OnDraw(&m_memoryDC, m_bufferBitmap);
//		
//		m_memoryDC.SelectObject(pOldbm) ;
//		m_memoryDC.DeleteDC();
//	}
}
/****************************************************************************/
/* Function : ��Ʈ �̹��� �ʱ�ȭ											*/
/* Paramter :																*/
/* Return	:																*/
/* Author	: By DRFN														*/
/* Comments	: FX Margin �ϸ鼭 �ҽ� ���� (�ѱ��Ҹ�ġ �����)				*/
/****************************************************************************/
void CStdDialog::InitImageList()
{
	BOOL bOK = FALSE;											// ��ư �̹��� �ε� �÷���

	CBitmap		bitmap;
	CString		strBitmapFile;	
	
	m_static_nChartCnt4ImgList++;								// �ε�� ��Ʈ ����


	if ( !m_ImgList_W15H15.m_hImageList)						// Width 15, Height 15 �̹��� ���� �ε�
	{
		if ( !m_ImgList_W15H15.Create( 15, 14, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W15_H15.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W15_H15.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W15H15.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}
	
	if ( !m_ImgList_W20H20.m_hImageList)						// Width 20, Height 20 �̹��� ���� �ε�
	{
		if ( !m_ImgList_W20H20.Create( 20, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W20_H20.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W20_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W20H20.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	if ( !m_ImgList_W20H20_Tool.m_hImageList)					// Width 15, Height 15 �̹��� ���� �ε�(TOOL)
	{
		if ( !m_ImgList_W20H20_Tool.Create( 19, 19, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W20_H20_Tool.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W20_H20_Tool.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W20H20_Tool.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	if ( !m_ImgList_W46H20.m_hImageList)						// Width 46, Height 20 �̹��� ���� �ε�
	{
		if ( !m_ImgList_W46H20.Create( 46, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W46_H20.bmp file" );
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W46_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));	
		m_ImgList_W46H20.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();						

	}
	
	if ( !m_ImgList_W50H20.m_hImageList)						// Width 50, Height 20 �̹��� ���� �ε�
	{
		if ( !m_ImgList_W50H20.Create( 50, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W50_H20.bmp file" );
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W50_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));	

		m_ImgList_W50H20.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	if ( !m_ImgList_W151H21.m_hImageList)						// Width 151, Height 21 �̹��� ���� �ε�
	{		
		if ( !m_ImgList_W151H21.Create( 151, 21, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W151_H21.bmp file" );
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W151_H21.bmp");		
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W151H21.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	if ( !m_ImgList_W28H22.m_hImageList)						// Width 28, Height 24 �̹��� ���� �ε�
	{
		if ( !m_ImgList_W28H22.Create( 28, 22, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W28_H22.bmp file" );
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W28_H22.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));	
		m_ImgList_W28H22.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();

	}

	if ( !m_ImgList_W17H20.m_hImageList)						// Width 17, Height 20 �̹��� ���� �ε�
	{
		if ( !m_ImgList_W17H20.Create( 17, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W17_H20.bmp file" );
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W17_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));	
		m_ImgList_W17H20.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();

	}

	if ( !m_ImgList_W43H20.m_hImageList)						// Width 43, Height 20 �̹��� ���� �ε�
	{
		if ( !m_ImgList_W43H20.Create( 43, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W43_H19.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W43_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W43H20.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	if ( !m_ImgList_W58H20.m_hImageList)						// Width 58, Height 20 �̹��� ���� �ε�
	{
		if ( !m_ImgList_W58H20.Create( 58, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W58_H19.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W58_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W58H20.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	if ( !m_ImgList_W72H20.m_hImageList)						// Width 72, Height 19 �̹��� ���� �ε�
	{
		if ( !m_ImgList_W72H20.Create( 72, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W72_H20.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W72_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W72H20.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}
	
	if ( !m_ImgList_W46H20_Gwan.m_hImageList)					// Width 46, Height 20 �̹��� ���� �ε�
	{
		if ( !m_ImgList_W46H20_Gwan.Create( 46, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W46_H20_Gwan.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W46_H20_Gwan.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W46H20_Gwan.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	if ( !m_ImgList_W8H25.m_hImageList)							// Width 8, Height 25 �̹��� ���� �ε�
	{
		if ( !m_ImgList_W8H25.Create( 8, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W8_H25.bmp file" );
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W8_H25.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));	
		m_ImgList_W8H25.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
		
	}
}

void CStdDialog::DeleteAllImageList()
{
	// �ε�� ��Ʈ ����
	m_static_nChartCnt4ImgList--;
	
	if ( m_static_nChartCnt4ImgList == 0)
	{
		m_ImgList_W15H15.DeleteImageList();

		m_ImgList_W20H20.DeleteImageList();
		m_ImgList_W20H20_Tool.DeleteImageList();
		m_ImgList_W46H20.DeleteImageList();
		m_ImgList_W50H20.DeleteImageList();

		m_ImgList_W151H21.DeleteImageList();
		m_ImgList_W28H22.DeleteImageList();
		m_ImgList_W17H20.DeleteImageList();
		m_ImgList_W43H20.DeleteImageList();	
		m_ImgList_W58H20.DeleteImageList();	
		m_ImgList_W72H20.DeleteImageList();	
		m_ImgList_W46H20_Gwan.DeleteImageList();
		m_ImgList_W8H25.DeleteImageList();
	}
}

void CStdDialog::GetStartEndDateAtStdDialog(double& dStartDate, double& dEndDate)
{
	if(m_ChartMng.GetSafeHwnd())
		m_ChartMng.GetStartEndDateAtChartMng(dStartDate, dEndDate);	
}


LRESULT CStdDialog::OnDropData(WPARAM wp, LPARAM lp)
{	
	return 0;
}

void CStdDialog::SendHogaNCodeToSpeedJumun(CString strCode, CString strPrice)
{
	m_xGSharedCli.SetData("S51210-ȣ��;", strPrice);
}

void CStdDialog::AllCodeUnAdvise(BOOL bFlag /* = TRUE */)
{
	if ( bFlag )
	{
		for(int i = 0; i < m_nDrdsInfoCnt; ++i)
			MakeDrdsUnConnectUnAdviseFromCode(i, FALSE);
	}

//@SolomonGNF 	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager, 3);
	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
	if(!pDrdsMng) return;

	if(m_pDrdsLib)
	{
		RemoveDrdsFromCode(&m_ListStockRealCodes);
		RemoveDrdsFromCode(&m_ListUpjongRealCodes);
		RemoveDrdsFromCode(&m_ListFutOptRealCodes);
		RemoveDrdsFromCode(&m_ListStockFutOptRealCodes);
		RemoveDrdsFromCode(&m_ListForeignRealCodes);

		m_pDrdsLib->DrdsClose("");

		pDrdsMng->UnadviseDrdsLib(m_pDrdsLib);
		m_pDrdsLib = NULL;
	}

	m_RealTrCounter.RemoveAll();
}

void CStdDialog::Recv_ReplayRealData(void *pData, int nLen)
{
}

LONG CStdDialog::OnReplayDrds(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != CMasterTypeInfo::STOCK_CHART) && (m_nMarketType != CMasterTypeInfo::FUTOPT_CHART))
		return 1L;

	CString strCode, strSpeed;
	strCode.Format("%s", (LPCTSTR)wp);

	m_ChartMng.SetReplayRealTitle(strCode, m_strSpeed, (TICK_DATA*)lp);
	return 1;
}

LONG CStdDialog::OnReplayFail(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != CMasterTypeInfo::STOCK_CHART) && (m_nMarketType != CMasterTypeInfo::FUTOPT_CHART))
		return 1L;
	
	if(m_pInputBarDlg)
		m_pInputBarDlg->ReplayFail();

	return 1;
}

LONG CStdDialog::OnReplayChangeDate(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != CMasterTypeInfo::STOCK_CHART) && (m_nMarketType != CMasterTypeInfo::FUTOPT_CHART))
		return 1L;

	CString strCode, strBojoMsg;
	strCode.Format("%s", (LPCTSTR)wp);
	strBojoMsg.Format("%s", (LPCTSTR)lp);
	
	m_ChartMng.SetReplayChangedDate(strCode, strBojoMsg);

	return 1;
}

BOOL CStdDialog::GetTimeSyncFlag()
{
	return m_pExtInputBarDlg->GetTimeSyncFlag();
}

void CStdDialog::SetTimeSyncFlag(BOOL bCheck)
{
	SetBReplayMode(bCheck);

	m_ChartMng.SetBTypeLink(bCheck);
	m_ChartMng.SetBReplayChartMode(bCheck); 
	m_pExtInputBarDlg->SetTimeSyncFlag(bCheck);
	m_pExtInputBarDlg->EnableWindow(!bCheck);
}

void CStdDialog::RqstTRForReplay()
{
	CStringArray arrStrCode;
	m_ChartMng.GetChartAllCode(arrStrCode);

	int nSize = arrStrCode.GetSize();
	if(nSize <= 0) return;

	

	CString strMultiCodes = _T("");

	for(int i = 0; i < nSize; i++) 
	{
		strMultiCodes += arrStrCode.GetAt(i);
		strMultiCodes += ",;";
	}	
	
	m_ChartMng.SendMessage(UMSG_MULTICODES, NULL, (LPARAM)(LPCTSTR)strMultiCodes);	
}

// ������ǥ���� TR Data��û
void CStdDialog::RequestTRPacket(const short nID, const LPCTSTR pcszPacketNames, const short nCount, const bool bIsNew)
{		
	m_ChartMng.MarketTRSend_By_OCXSignal(nID, pcszPacketNames, nCount, bIsNew ? TRUE : FALSE);
}

bool CStdDialog::IsLoad_MarketChart() const
{ 
	if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
		return true;

	return false;
}

void CStdDialog::SetBReplayMode(BOOL bReplay)
{ 
	m_bReplayMode = bReplay; 	
	m_pExtInputBarDlg->EnableWindow(!bReplay);
}

BOOL CStdDialog::CheckDataTypeForReplay()
{
	return m_ChartMng.CheckDataTypeForReplay();
}

LONG CStdDialog::OnReplayEnd(WPARAM wp, LPARAM lp)
{
	if ((m_nMarketType != CMasterTypeInfo::STOCK_CHART) && (m_nMarketType != CMasterTypeInfo::FUTOPT_CHART))
		return 1L;

	return 1;
}

LONG CStdDialog::OnReplaySetTotalSec(WPARAM wp, LPARAM lp)
{
	if ((m_nMarketType != CMasterTypeInfo::STOCK_CHART) && (m_nMarketType != CMasterTypeInfo::FUTOPT_CHART))
		return 1L;
	
	if(m_pInputBarDlg)
		m_pInputBarDlg->SetReplayTotalSec(wp);

	return 1;
}

LONG CStdDialog::OnReplayEachSec(WPARAM wp, LPARAM lp)
{
	if ((m_nMarketType != CMasterTypeInfo::STOCK_CHART) && (m_nMarketType != CMasterTypeInfo::FUTOPT_CHART))
		return 1L;

	CString sTime((LPTSTR)(LPCTSTR)wp);

	if(m_pInputBarDlg)
		m_pInputBarDlg->SetReplayEachSec(sTime);

	return 1;
}


void CStdDialog::SetOpenMapData(LPCTSTR szOpenData, short nDataLen) 
{
	if(m_pInputBarDlg)
	{
		// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
		CString strCodeInputCode = m_pInputBarDlg->GetTypedItemCode();
		CString strSetCode = (LPCTSTR)szOpenData;
		if(strSetCode.GetLength() && strCodeInputCode != strSetCode)
		{
			m_pInputBarDlg->SetShareCodeChanged(TRUE);
			// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
			m_pInputBarDlg->m_pCodeInput->SetCode(CStdDialog::GetRealItemCode( ( LPCTSTR)szOpenData));
			m_pInputBarDlg->SetShareCodeChanged(FALSE);
		}
	}
}

//@�����߰�
void CStdDialog::ChangeCode(LPCTSTR szCode, int nDefault/* = MODE_DEFAULT*/, int nMode/* = CHANGE_ITEM*/) 
{
	// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
	CString strSBCode( szCode);
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	if( m_pInputBarDlg) if( m_pInputBarDlg->GetSafeHwnd())
		strSBCode = m_pInputBarDlg->GetTypedItemCode( szCode, GetRequestPairTickState());

	int nType;
	ChangeStockMode(GetMarketTypeFromCode(szCode, nType));

	STINPUTCODEDATA stInputCodeData;
	stInputCodeData.m_strCode = strSBCode;
	stInputCodeData.m_strTime = "";
	stInputCodeData.m_nDefault = nDefault;
	stInputCodeData.m_nMode = nMode;
	// �������� �߰� UI : �������� - ojtaso (20070522)
	m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, nDefault, (LPARAM)&stInputCodeData);

}

//@�����߰�
void CStdDialog::ChangeTimeSet(int nIndex, BOOL	bSetFlag /* = FALSE */) 
{
	m_ChartMng.SetTimeGab(nIndex, bSetFlag);
}

void CStdDialog::ChangeMarket(UINT nMarketType, BOOL bUpdateCbMarket/* = TRUE*/ , BOOL bRequestToServer/* = FALSE*/) 
{
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->ChangeMarket(nMarketType, bUpdateCbMarket, bRequestToServer);
}

void CStdDialog::EnableQueryOpt(BOOL bEnable)
{
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
		m_pExtInputBarDlg->EnableQueryOpt(bEnable);
}
//@�����߰�

//-----------------------------------------------------------------------------
// Author		: {{{ ������ }}} ojtaso 	Date : 2005/2/23
// Reason		: Added to drag and drop operation.
// modify		: {{{ �̿��� }}} Date : 2006/7/7
// Reason		: ���� ��� �Űܿ�
//-----------------------------------------------------------------------------
// BEGIN
LRESULT CStdDialog::OnDragEnd(WPARAM wParam, LPARAM lParam)
{
	CDragNDrop* pDnD = (CDragNDrop*)lParam;	
	if(strcmp(pDnD->GetKey(), "��ǥ-�巡��") == 0)
	{
		if(!pDnD)
			return 0L;

		HWND hWnd = ::WindowFromPoint(*pDnD->GetCursorPoint());
		HWND hStart = pDnD->GetHWndStart();

		if(m_hCursor)
		{
			::DeleteObject(m_hCursor);
			m_hCursor = NULL;
		}

		if(::IsWindow(hWnd) && hWnd != hStart && !::IsChild(hWnd, hStart))
		{
			if(m_ChartMng.GetSafeHwnd()) // 2005. 04. 22 by Nam
				m_ChartMng.SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)pDnD->GetValue());
		}

		if(pDnD->IsDelete())
			delete pDnD;
	}
	else
	{
		if(!pDnD)
			return 0L;

		HWND hWnd = ::WindowFromPoint(*pDnD->GetCursorPoint());
		HWND hStart = pDnD->GetHWndStart();

		if(m_hCursor)
		{
			::DeleteObject(m_hCursor);
			m_hCursor = NULL;
		}

		if(::IsWindow(hWnd) && hWnd != hStart && !::IsChild(hWnd, hStart))
		{
			CString strParam = pDnD->GetValue();
			int nPosCode = strParam.Find(";");
			CString strCode = strParam.Left(nPosCode);
			strCode.Remove(';');
			CString strOption = strParam.Mid(nPosCode + 1, 1);
			if(strOption == "L")
				ChangeCode(strCode);
			else if(strOption == "R")
			{
				CStringList listViewType;
				if(m_pInputBarDlg)
				{
					m_pInputBarDlg->GetChartViewType(listViewType);

					CMenu menu;
					menu.CreatePopupMenu();

					int nIndx = 0;
					POSITION pos = listViewType.GetHeadPosition();
					while(pos)
					{
						menu.AppendMenu(MF_STRING, (UINT)1000 + nIndx, listViewType.GetNext(pos));
						nIndx++;
					}

					UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
					int nXPos = (*pDnD->GetCursorPoint()).x;
					int nYPos = (*pDnD->GetCursorPoint()).y;
					int nSelectMode = menu.TrackPopupMenu(nFlagsForMenu, nXPos, nYPos, this);
					menu.DestroyMenu();
					if(nSelectMode <= 0)
					{
						
						if(pDnD->IsDelete())
						delete pDnD;

						return 0L;
					}

					ChangeCode(strCode, MODE_USER, nSelectMode - 1000);
				}				
			}
		}

		if(pDnD->IsDelete())
			delete pDnD;
	}
	return 0L;
}

LRESULT CStdDialog::OnDragStart(WPARAM wParam, LPARAM lParam)
{
	CPoint* pPoint = (CPoint*)wParam;

	HWND hWnd = ::WindowFromPoint((POINT)(*pPoint));
	HWND hStart = (HWND)lParam;

	::SetFocus(hWnd);

	if(hWnd != hStart)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		//HINSTANCE hInstResourceClient = AfxGetResourceHandle();
		//AfxSetResourceHandle(g_hInstance);

		m_hCursor = AfxGetApp()->LoadCursor(IDC_HANDCUR);
		//AfxSetResourceHandle(hInstResourceClient);
		::SetCursor(m_hCursor);
	}

	return 0L;
}

//003 {{ ȣ��TR_AddDataKey		
void CStdDialog::AddDataKey_Hoga()
{
//@SolomonGNF -->
// 	m_TRComm->AddDataKey(15007);	//���ذ�
// 	m_TRComm->AddDataKey(15313);	//����ü�ᰡ��
// 	m_TRComm->AddDataKey(15315);	//����ü�����	//-> ����ŷ������� �ӽù���
// 	//}}
// 
// 	m_TRComm->AddDataKey(DI_NAME);	//�����
// 	m_TRComm->AddDataKey(DI_PRICE);	//���簡
// 	//m_TRComm->AddDataKey(DI_SIGN);	//�����ȣ
// 	m_TRComm->AddDataKey(15317);	//�����ȣ
// 	m_TRComm->AddDataKey(15004);	//�����
// 	m_TRComm->AddDataKey(15472);	//�����
// 	m_TRComm->AddDataKey(DI_VOLUME);	//�����ŷ���	//-> �ŷ������� �ӽù���
// 	m_TRComm->AddDataKey(DI_AMOUNT);	//�����ŷ����	//-> �ŷ�������� �ӽù���
// 	//m_TRComm->AddDataKey(14659);	//ȣ���ܷ����ؽð�
// 	
// 	// �ŵ� 8 + 40 + 
// 	int nIdx;
// 	int nDataKey;
// 
// 	nDataKey = DI_OFFER_HO1;		// �ŵ�ȣ��1
// 	for(nIdx=0; nIdx < 5; nIdx++)
// 	{
// 		m_TRComm->AddDataKey(nDataKey+nIdx);		
// 	}
// 
// 	nDataKey = DI_OFFER_REM1;		// �ŵ�ȣ���ܷ�1
// 	for(nIdx=0; nIdx < 5; nIdx++)
// 	{
// 		m_TRComm->AddDataKey(nDataKey+nIdx);		
// 	}
// 
// 	nDataKey = DI_BID_HO1;		// �ż�ȣ��1
// 	for(nIdx=0; nIdx < 5; nIdx++)
// 	{
// 		m_TRComm->AddDataKey(nDataKey+nIdx);		
// 	}
// 
// 	nDataKey = DI_BID_REM1;		// �ż�ȣ���ܷ�1
// 	for(nIdx=0; nIdx < 5; nIdx++)
// 	{
// 		m_TRComm->AddDataKey(nDataKey+nIdx);		
// 	}
// 
// 	//{{ 2007.06.27 by LYH ���ܷ��� 10ȣ���ܷ����� ����
// 	//m_TRComm->AddDataKey(DI_OFFER_TOTREM);	//�Ѹŵ��ܷ�
// 	//m_TRComm->AddDataKey(DI_BID_TOTREM);	//�Ѹż��ܷ�
// 	m_TRComm->AddDataKey(14565);	//10ȣ���ŵ��ܷ�
// 	m_TRComm->AddDataKey(14567);	//10ȣ���ż��ܷ�
// 	//}}
// 	m_TRComm->AddDataKey(DI_OFFER_OUT_TOT);	//�ð��ܸŵ��ܷ�
// 	m_TRComm->AddDataKey(DI_BID_OUT_TOT);	//�ð��� �ż��ܷ�
// 
// 	m_TRComm->AddDataKey(16117);	//���Ѱ�
// 	m_TRComm->AddDataKey(16118);	//���Ѱ�
// 	//{{ 2007.06.27 by LYH �ð���� �������� ����
// 	m_TRComm->AddDataKey(15326);	//�ð�->������ -> ���������� ��� 
// 	//}}
// 	m_TRComm->AddDataKey(15010);	//��
// 	m_TRComm->AddDataKey(15011);	//����
// 
// 	m_TRComm->AddDataKey(3007);		//���ϰŷ���  //-> ���ϰŷ���_������������ �����ͷ� �ӽû��
// 	m_TRComm->AddDataKey(16143);	//����kospi����kosdaq �ֽļ�
//@SolomonGNF <--
}

//003 }}

//008 {{
void CStdDialog::ChangeUpjongMode(UINT type)
{
	if(m_pInputBarDlg)
		m_pInputBarDlg->InitUpjongCodeCombo(type);
}
//008 }}

// END

//Real process
HRESULT CStdDialog::ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData)
{
	return 1L;
}

void CStdDialog::ConvertHogaData(LPVOID	aTRData, long dwTRDateLen)
{
//@SolomonGNF -->
// 	STDR_FID* pStdrFid_Rec1 = (STDR_FID*)aTRData;
// 	m_strOut = "";
// 
// 	CString strOut;
// 	// ����Ű
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][0];
// 	// ���ذ�
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][1];
// 	// ����ü�ᰡ��
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][2];
// 	// ����ü�����
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][3];
// 	// �����
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][4];
// 	// ���簡
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][5];
// 	// �����ȣ
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][6];
// 	// �����
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][7];
// 	// �����
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][8];
// 	// �����ŷ���
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][9];
// 	// �����ŷ����
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][10];
// 	// �ŵ�ȣ��1
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][11];
// 	// �ŵ�ȣ��2
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][12];
// 	// �ŵ�ȣ��3
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][13];
// 	// �ŵ�ȣ��4
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][14];
// 	// �ŵ�ȣ��5
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][15];
// 	// �ŵ��ܷ�1
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][16];
// 	// �ŵ��ܷ�2
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][17];
// 	// �ŵ��ܷ�3
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][18];
// 	// �ŵ��ܷ�4
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][19];
// 	// �ŵ��ܷ�5
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][20];
// 	// �ż�ȣ��1
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][21];
// 	// �ż�ȣ��2
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][22];
// 	// �ż�ȣ��3
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][23];
// 	// �ż�ȣ��4
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][24];
// 	// �ż�ȣ��5
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][25];
// 	// �ż��ܷ�1
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][26];
// 	// �ż��ܷ�2
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][27];
// 	// �ż��ܷ�3
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][28];
// 	// �ż��ܷ�4
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][29];
// 	// �ż��ܷ�5
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][30];
// 	// �Ѹŵ��ܷ�
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][31];
// 	// �Ѹż��ܷ�
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][32];
// 	// �ð��ܸŵ��ܷ�
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][33];
// 	// �ð��� �ż��ܷ�
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][34];
// 	// ���Ѱ�
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][35];
// 	// ���Ѱ�
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][36];
// 	// ��������
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][37];
// 	// ��
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][38];
// 	// ����
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][39];
// 	// ���ϰŷ���
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][40];
// 	// ����kospi����kosdaq �ֽļ�
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][41];
// 	//strData = "          ";
// 	m_strOut = strOut;

//@SolomonGNF <--
}
// END


// ������ �����ϰų� ����� ��� �����͸� ���� �ް� �ȴ�. �̶� �ʱ�ȭó���� ������ϴ� ��ƾ���ִ�.
// �ý���Ʈ���̵��� ������ ����ǰ� ���� �� �̷��� ���濡 ���� ó���� ����� �Ѵ�.
void CStdDialog::NofityNewDataReceive()
{
	// �ý��������� ����ǰ� ���� ��� ������ Ŭ�����Ѵ�.
	// m_ChartMng.GetpChartItemSelected()->InvokeAddIn("ChartSTIndexAddIn.dll", 7, (long)this);

	// ��������Ʈ�� ����Ǿ��ٴ� ������ �˸���.
	//int nID = UUID_IAUDataMonitorManager;
	//IAUDataMonitorManager* pMng = (IAUDataMonitorManager*)AfxGetPctrInterface(nID);
	//pMng->NotifyData("MultiChart_ReceiveData", (long)this, NULL);
}

void CStdDialog::SetOneClickHide(BOOL bOneClickHide)
{
	m_bOneClickHide = bOneClickHide;
	CRect rect;	
	CRect chartMngRect;

	if(!m_pInputBarDlg||!m_pExtInputBarDlg||!m_pLeftBarDlg||!m_ChartMng.GetSafeHwnd()) return;
	rect = GetRect(ID_INPUTBAR,TRUE);
	m_pInputBarDlg->MoveWindow(rect);
	m_pInputBarDlg->SetLeftBarType(m_nLeftBarType);

	rect = GetRect(ID_EXTINPUTBAR,TRUE);
	m_pExtInputBarDlg->MoveWindow(rect);

	if(m_bOneClickHide)
	{		
		m_pInputBarDlg->ShowWindow(SW_HIDE);
		m_pExtInputBarDlg->ShowWindow(SW_HIDE);
		m_pLeftBarDlg->ShowWindow(SW_HIDE);
		m_xSplitter1.ShowWindow(SW_HIDE);
		if(m_ChartMng.GetSafeHwnd())
		{
			m_ChartMng.GetWindowRect(chartMngRect);
			ScreenToClient(chartMngRect);
			CRect rectNew = GetRect(ID_CHART,TRUE);
			rect = GetRect(ID_LEFTBAR,TRUE);
			chartMngRect.left	= rect.left;		//ChartMng
			chartMngRect.right	= rectNew.right;	//ChartMng
			chartMngRect.bottom	= rectNew.bottom;		
			chartMngRect.top	= rect.top;				

			m_ChartMng.MoveWindow(chartMngRect);
		}
	}
	else
	{
		m_pInputBarDlg->ShowWindow(SW_SHOW);
		m_pExtInputBarDlg->ShowWindow(SW_SHOW);

		rect = GetRect(ID_LEFTBAR,TRUE);
		if(rect.IsRectNull())
		{
			m_pLeftBarDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pLeftBarDlg->MoveWindow(rect);
			m_pLeftBarDlg->ShowWindow(SW_SHOW);
		}

		rect = GetRect(ID_SPLITTER,TRUE);
		if(rect.IsRectNull())
		{
			m_xSplitter1.ShowWindow(SW_HIDE);
		}
		else
		{
			m_xSplitter1.MoveWindow(rect);
			m_xSplitter1.ShowWindow(SW_SHOW);
		}	

		if(m_ChartMng.GetSafeHwnd())
		{
			m_ChartMng.GetWindowRect(chartMngRect);
			ScreenToClient(chartMngRect);
			CRect rectNew = GetRect(ID_CHART,TRUE);
			if(chartMngRect!=rectNew)	
			{
				chartMngRect.left	= rectNew.left;		//ChartMng
				chartMngRect.right	= rectNew.right;	//ChartMng
				chartMngRect.bottom	= rectNew.bottom;		
				chartMngRect.top	= rectNew.top;				
			}

			m_ChartMng.MoveWindow(chartMngRect);
		}
	}
}

BOOL	CStdDialog::GetRevisedPriceFlag( void)	//ocx �����ְ���뿩�� �޾ư���
{
	return m_ChartMng.RevisedPriceFlag();
}
BOOL	CStdDialog::SetRevisedPriceFlag( BOOL p_bRevisedFlag)	//ocx �����ְ���뿩�� ����
{
	m_ChartMng.SendMessage(UMSG_UPDATEREVISED, p_bRevisedFlag);
	return TRUE;
}
BOOL	CStdDialog::OnIndicatorAddOrRemoved( const char *p_szGraphName,	const BOOL p_bAdded)
{
	return FALSE;
}
BOOL	CStdDialog::OnUserZoomChanged( const int p_nDataCountInView)
{
//	if(m_pInputBarDlg) // 2005. 04. 22 by Nam
//		m_pInputBarDlg->SetViewCount(p_nDataCountInView);

	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->SendMessage(UMSG_SETZOOMVALUE, p_nDataCountInView);

	return FALSE;
}
BOOL	CStdDialog::OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn)
{
	return FALSE;
}

void	CStdDialog::GridColumnExtend()
{
	
	CRect rect;
	GetClientRect(rect);
	rect.DeflateRect(BOUND_GAP,BOUND_GAP,BOUND_GAP,BOUND_GAP);

	CRect rectSplitter1;
	CRect rectLeftBar;
	m_pLeftBarDlg->GetWindowRect(rectLeftBar);
	m_xSplitter1.GetWindowRect(rectSplitter1);
	ScreenToClient(rectSplitter1);	

	if (m_nExtendState == 0)
	{
		rect.left = 245;
		rect.right = 250;    

		m_nWidthOfLeftBar += 75;
		m_nExtendState = 1;
	}
	else
	{
		rect.left = 162;
		rect.right = 167; 

		m_nWidthOfLeftBar -= 75;
		m_nExtendState = 0;
	}
	
	m_xSplitter1.MoveWindow(rect);
	m_xSplitter1.RecalcLayout();
	m_pLeftBarDlg->Invalidate();
	m_ChartMng.Invalidate();
	m_xSplitter1.Invalidate();

	if(m_pInputBarDlg)
		m_pInputBarDlg->Invalidate();
	m_pExtInputBarDlg->Invalidate();

	if(	(m_bShowDataListWnd				) &&
		(m_pDataWindow != NULL			) &&
		(m_pDataWindow->GetSafeHwnd()	) 
	)
	{
		if(m_ChartMng.GetSafeHwnd())
		{	

			CRect rect2;
			m_ChartMng.GetWindowRect(rect2);	
			ScreenToClient(rect2);
			m_ViewRect.left = rect2.left;
			m_pDataWindow->MoveWindow(m_ViewRect);	
		}
	}

	if(m_ChartMng.m_pFloatingDlg != NULL)				
		m_ChartMng.m_pFloatingDlg->Invalidate();

	return;
}

BOOL CStdDialog::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

//��Ʈ ����� ���� ���
void CStdDialog::AddRealCodes(CString strTrCode, LIST_CODEDATA *pListRealCodes) 
{
//@��������
//	m_TRComm->ResetRealTR(strTrCode);
//@��������
	if(pListRealCodes->GetCount() <= 0)
		return;

//@��������
//	m_TRComm->SetTRCondition(strTrCode, strTrCode);
//@��������
	ST_CODEDATA oneCode;
	int nOffset = 0;
	char cCode[6+1]={0,}; 
	int nIndex = 0;
	UINT nCodeLength=0;
	CByteArray bArr;
	for(POSITION pos=pListRealCodes->GetHeadPosition(); pos; )
	{
		oneCode = pListRealCodes->GetNext(pos);
		if(oneCode.m_bType == 79 || oneCode.m_bType == 80 || oneCode.m_bType == 81 || oneCode.m_bType == 82 || oneCode.m_bType == 83 || oneCode.m_bType == 84 || oneCode.m_bType == 85 || oneCode.m_bType == 86)
		{
			nCodeLength = 16;
		}
		else
		{
			nCodeLength = oneCode.m_strCode.GetLength();
			if(nCodeLength != CODE_STOCK_LENGTH && nCodeLength != FUTOPT_LENGTH && nCodeLength != (UPJONG_LENGTH-1))
				continue;
		}

		if(nIndex == 0)
		{
//@SolomonGNF -->
// 			if(oneCode.m_bType == 2 || oneCode.m_bType == 4)
// 				m_TRComm->AddMainKey(DI_CODE, oneCode.m_strCode, C_DT_USHORT, 2);
// 			else
// 				m_TRComm->AddMainKey(DI_CODE, oneCode.m_strCode, C_DT_STRING, nCodeLength);
// 			
// 			m_TRComm->AddOptionKey(1, NULL, 5, 0, 0);
// 			m_TRComm->SetRealUpdateType(2);			//  RT_UPDATE	2	// ������Ʈ Ÿ��
// 			//{{2007.06.26 by LYH �ֽĸ��� ���(����ü�� �������� ����)
// 			if(strTrCode == "20000")
// 			{
// 				m_TRComm->AddDataKey(15313);//���簡
// 				m_TRComm->AddDataKey(15009);//�ð�
// 				m_TRComm->AddDataKey(15010);//��
// 				m_TRComm->AddDataKey(15011);//����
// 				m_TRComm->AddDataKey(15317);//�����ȣ
// 				m_TRComm->AddDataKey(15316);//���Ϻ�
// 				m_TRComm->AddDataKey(15326);//�����
// 				m_TRComm->AddDataKey(15315);//�ŷ���
// 				m_TRComm->AddDataKey(DI_TIME);//ü��ð�
// 			}
// 			else
// 				//}}
// 			{
// 				m_TRComm->AddDataKey(15001);//���簡
// 				m_TRComm->AddDataKey(15009);//�ð�
// 				m_TRComm->AddDataKey(15010);//��
// 				m_TRComm->AddDataKey(15011);//����
// 				m_TRComm->AddDataKey(15006);//�����ȣ
// 				m_TRComm->AddDataKey(15472);//���Ϻ�
// 				m_TRComm->AddDataKey(15004);//�����
// 				m_TRComm->AddDataKey(15015);//�ŷ���
// 				m_TRComm->AddDataKey(DI_TIME);//ü��ð�
// 			}
//@SolomonGNF <--
			nIndex++;
		}
		else
		{
		}
		bArr.Add(oneCode.m_bType);
	}
}

//��Ʈ ����� ���� ���
void CStdDialog::RemoveRealCodes(LIST_CODEDATA *pListRealCodes, CString strKey) 
{
	ST_CODEDATA oneCode;
	for(POSITION pos=pListRealCodes->GetHeadPosition(); pos; )
	{
		oneCode = pListRealCodes->GetAt(pos);

		{
			if(strKey == oneCode.m_strCode)
			{
				pListRealCodes->RemoveAt(pos);
				break;
			}
		}
		pListRealCodes->GetNext(pos);
	}
}

int CStdDialog::GetWidthOfLeftBar()
{
	return m_nWidthOfLeftBar;
}

// �������� ���� ���� : �������� - ojtaso (20070209)
void CStdDialog::OnDeleteChartItem(LPCSTR lpszItemCode)
{
	OnCodeUnAdvise(lpszItemCode, TRUE);

	int nType;
	int nMarketType = GetMarketTypeFromCode(lpszItemCode, nType);
	if(nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
		RemoveRealCodes(&m_ListStockRealCodes, lpszItemCode);
		AddRealCodes("20000", &m_ListStockRealCodes);
	}
	else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		RemoveRealCodes(&m_ListUpjongRealCodes, lpszItemCode);
		AddRealCodes("20001", &m_ListUpjongRealCodes);
	}
	else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
	{
		RemoveRealCodes(&m_ListFutOptRealCodes, lpszItemCode);
		AddRealCodes("20002", &m_ListFutOptRealCodes);
	}
	else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
		RemoveRealCodes(&m_ListStockFutOptRealCodes, lpszItemCode);
		AddRealCodes("20002", &m_ListStockFutOptRealCodes);
	}
	else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		RemoveRealCodes(&m_ListForeignRealCodes, lpszItemCode);
		AddRealCodes("20003", &m_ListForeignRealCodes);
	}
}

// ��Ʈ �ʱ�ȭ�� ��� ���� ���� : �������� - ojtaso (20070621)
void CStdDialog::ResetChartAll()
{
	if(!m_ListStockRealCodes.IsEmpty())
	{
		m_ListStockRealCodes.RemoveAll();
		AddRealCodes("20000", &m_ListStockRealCodes);
	}
	if(!m_ListUpjongRealCodes.IsEmpty())
	{
		m_ListUpjongRealCodes.RemoveAll();
		AddRealCodes("20001", &m_ListUpjongRealCodes);
	}
	if(!m_ListFutOptRealCodes.IsEmpty())
	{
		m_ListFutOptRealCodes.RemoveAll();
		AddRealCodes("20002", &m_ListFutOptRealCodes);
	}
	if(!m_ListStockFutOptRealCodes.IsEmpty())
	{
		m_ListStockFutOptRealCodes.RemoveAll();
		AddRealCodes("20002", &m_ListStockFutOptRealCodes);
	}
	if(!m_ListForeignRealCodes.IsEmpty())
	{
		m_ListForeignRealCodes.RemoveAll();
		AddRealCodes("20003", &m_ListForeignRealCodes);
	}
}

// ����� ��� : �������� - ojtaso (20080213)
CString CStdDialog::GetJongMokNameByCode(LPCSTR lpszItemCode, LPCTSTR lpszTRCode/* = NULL*/)
{
	if(!m_pInputBarDlg)
		return _T("");

	return m_pInputBarDlg->GetJongMokNameByCode(lpszItemCode, lpszTRCode);
}

// �������� ���� : �������� - ojtaso (20080215)
void CStdDialog::OnSelectChartItem(ST_SELCHART* pSelChart)
{
	m_pInputBarDlg->InitChartMulitItem(pSelChart);
// --> [Edit]  ������ 2008/12/04	( �ֱⱸ���� �������ش�. )
	m_pExtInputBarDlg->m_chType = m_pInputBarDlg->GetType();
// <-- [Edit]  ������ 2008/12/04
	m_pExtInputBarDlg->InitChartMulitItem(pSelChart);
}

// ���� ����� ���õ� ������ ������ : �������� - ojtaso (20070223)
BOOL CStdDialog::IsSameItemCodeName(LPCTSTR lpszItemName)
{
	return !::lstrcmp(lpszItemName, m_pInputBarDlg->GetCurCodeNameFromControl());
}

// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
USHORT CStdDialog::GetDotPosition(CString& strItemCode)
{
	if(strItemCode.GetAt(0) == _T('2') || strItemCode.GetAt(0) == _T('3'))
		return 1;

	USHORT nDotPos = 2;

	if(strItemCode.GetAt(1) == _T('7') || strItemCode.GetAt(1) == _T('U') || strItemCode.GetAt(1) == _T('Y') || strItemCode.GetAt(1) == _T('E'))
		nDotPos = 1;
	else if(strItemCode.GetAt(1) == _T('8') || strItemCode.GetAt(1) == _T('G'))
		nDotPos = 0;

	return nDotPos;
}

// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)
void CStdDialog::EnableRequestMoreButton(BOOL bEnable)
{
	if(m_pInputBarDlg)
		m_pInputBarDlg->EnableRequestMoreButton(bEnable);
}

#ifdef _WRITELOG
// Log - ojtaso (20080107)
void CStdDialog::WriteToStringLog( const char *p_szSection, const char *p_szData, int p_nDataLen)
{
	if( m_hLogFile == HFILE_ERROR) return;

	// 1. ���� Logging�ϴ� �ð��� Ȯ���Ѵ�.
	SYSTEMTIME sTime;
	GetLocalTime(&sTime);

	// 4. �־��� Section Value�� Ȯ���Ͽ� Section String�� �����Ѵ�.
	CString strBuffer;
	if( p_szSection && *p_szSection != '\0')
	{
		CString strCurTime;
		// (2005/9/1 - Seung-Won, Bae) Line Skip for Only Section.
		strCurTime.Format("\r\n\r\n\r\n[%02d:%02d:%02d:%03d  ", sTime.wHour, sTime.wMinute, sTime.wSecond, 
			sTime.wMilliseconds);
		strBuffer = strCurTime + p_szSection + "]\r\n";
		_hwrite( m_hLogFile, ( LPCTSTR)strBuffer, strBuffer.GetLength());
	}
	// (2005/9/2 - Seung-Won, Bae) in No Section Log, Insert Time in front of Line
	else
	{
		CString strCurTime;
		strCurTime.Format("%02d:%02d:%02d:%03d\t", sTime.wHour, sTime.wMinute, sTime.wSecond, 
			sTime.wMilliseconds);
		_hwrite( m_hLogFile, strCurTime, strCurTime.GetLength());
	}

	// 5. Memory�� Dump�Ѵ�.
	_hwrite( m_hLogFile, p_szData, p_nDataLen);
}

// Log - ojtaso (20080107)
void CStdDialog::CreateLogFile()
{
	// 0. Parameter ���� �����ϱ� ���Ͽ�, ���� Variable�� �̿��Ѵ�.
	const char *szLogFolderPath	 = "C:\\Logs";
	// 1. ���� Logging�ϴ� �ð��� Ȯ���Ѵ�.
	SYSTEMTIME sTime;
	GetLocalTime(&sTime);

	// 2.3 Log File Name�� �����Ѵ�.
	CString strFileName;
	strFileName.Format( "%s\\MultiChart%s_%04d%02d%02d.log", szLogFolderPath, m_strScreenNo, sTime.wYear, sTime.wMonth, sTime.wDay);
	// 3. szLogFolderPath�� ������ �ʿ䰡 ������ �̸� ó���Ѵ�.
	::CreateDirectory( szLogFolderPath, NULL);

	// 3. Log File�� Open�Ѵ�.
	m_hLogFile = _lopen( ( LPCTSTR)strFileName, OF_READWRITE);
	if( m_hLogFile != HFILE_ERROR)
	{
		// 3.1 ���� File�� ������ File ������ �̵��Ѵ�.
		if( _llseek( m_hLogFile, 0L, FILE_END) == HFILE_ERROR) return;
	}
	// 3.2 ���� File�� ������ ������ �����Ѵ�.
	else m_hLogFile = _lcreat( ( LPCTSTR)strFileName, 0);
}

// Log - ojtaso (20080107)
void CStdDialog::CloseLogFile()
{
	// 6. Dump File�� �ݴ´�.
	if( m_hLogFile != HFILE_ERROR) _lclose( m_hLogFile);
}
#endif


#ifdef _WRITELOG_SISE
// Log - ojtaso (20080107)
void CStdDialog::WriteToStringLog( const char *p_szSection, const char *p_szData, int p_nDataLen)
{
	if( m_hLogFile == HFILE_ERROR) return;

	// 1. ���� Logging�ϴ� �ð��� Ȯ���Ѵ�.
	SYSTEMTIME sTime;
	GetLocalTime(&sTime);

	// 4. �־��� Section Value�� Ȯ���Ͽ� Section String�� �����Ѵ�.
	CString strBuffer;
	if( p_szSection && *p_szSection != '\0')
	{
		CString strCurTime;
		// (2005/9/1 - Seung-Won, Bae) Line Skip for Only Section.
		strCurTime.Format("\r\n[%02d:%02d:%02d:%03d  ", sTime.wHour, sTime.wMinute, sTime.wSecond, 
			sTime.wMilliseconds);
		strBuffer = strCurTime + p_szSection + "]\r\n";
		_hwrite( m_hLogFile, ( LPCTSTR)strBuffer, strBuffer.GetLength());
	}
	// (2005/9/2 - Seung-Won, Bae) in No Section Log, Insert Time in front of Line
	else
	{
		CString strCurTime;
		strCurTime.Format("%02d:%02d:%02d:%03d\t", sTime.wHour, sTime.wMinute, sTime.wSecond, 
			sTime.wMilliseconds);
		_hwrite( m_hLogFile, strCurTime, strCurTime.GetLength());
	}

	// 5. Memory�� Dump�Ѵ�.
	_hwrite( m_hLogFile, p_szData, p_nDataLen);
}

// Log - ojtaso (20080107)
void CStdDialog::CreateLogFile()
{
	// 0. Parameter ���� �����ϱ� ���Ͽ�, ���� Variable�� �̿��Ѵ�.
	const char *szLogFolderPath	 = "C:\\Logs";
	// 1. ���� Logging�ϴ� �ð��� Ȯ���Ѵ�.
	SYSTEMTIME sTime;
	GetLocalTime(&sTime);

	// 2.3 Log File Name�� �����Ѵ�.
	CString strFileName;
	strFileName.Format( "%s\\MultiChart%s_%04d%02d%02d_%02d%02d%02d.log", szLogFolderPath, m_strScreenNo, sTime.wYear, sTime.wMonth, sTime.wDay, sTime.wHour, sTime.wMinute, sTime.wSecond);
	// 3. szLogFolderPath�� ������ �ʿ䰡 ������ �̸� ó���Ѵ�.
	::CreateDirectory( szLogFolderPath, NULL);

	// 3. Log File�� Open�Ѵ�.
	m_hLogFile = _lopen( ( LPCTSTR)strFileName, OF_READWRITE);
	if( m_hLogFile != HFILE_ERROR)
	{
		// 3.1 ���� File�� ������ File ������ �̵��Ѵ�.
		if( _llseek( m_hLogFile, 0L, FILE_END) == HFILE_ERROR) return;
	}
	// 3.2 ���� File�� ������ ������ �����Ѵ�.
	else m_hLogFile = _lcreat( ( LPCTSTR)strFileName, 0);

	WriteToStringLog("�α� ��� ���� =========================================================================", _T("CStdDialog"), 10);
}

// Log - ojtaso (20080107)
void CStdDialog::CloseLogFile()
{
	WriteToStringLog("�α� ��� ���� =========================================================================", _T("CStdDialog"), 10);

	// 6. Dump File�� �ݴ´�.
	if( m_hLogFile != HFILE_ERROR) _lclose( m_hLogFile);
}
#endif

void CStdDialog::OnBtnExtTool() 
{
	m_bShowToolBar = !m_bShowToolBar;

	if(m_bShowToolBar)
	{
		m_btnExtTool.FP_vSetImage( &m_ImgList_W8H25, 2, 3);
		m_btnExtTool.FP_vSetToolTipText("�������");
	}
	else
	{
		m_btnExtTool.FP_vSetImage( &m_ImgList_W8H25, 0, 1);
		m_btnExtTool.FP_vSetToolTipText("����Ȯ��");
	}
	m_btnExtTool.Invalidate();
	Resize();
}

//@Solomon_GNF-->
/*
long CStdDialog::OnGetBroad(WPARAM wParam, LPARAM lParam)
{
	if(!m_pDrdsLib || !m_ChartMng)
		return 0;

	int pRealType = -1;
	char* pRealStruct = NULL;
	m_pDrdsLib->GetRealStructData(wParam, lParam, pRealStruct, pRealType);

	if(pRealType == STOCK_SISE_PID)
	{
		CRealData_Common realData_Common;
		CChartRealConverter::Convert_FX_STOCK_SISE(pRealStruct, realData_Common);
		m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common, pRealType);
		return 1L;
	}
	else if(pRealType == STOCK_30SEC_PID)
	{
	CRealData_Common realData_Common;
	CChartRealConverter::Convert_FX_STOCK_30SEC(pRealStruct, realData_Common);
	m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common, pRealType);
	return 1L;
	}
	else if(pRealType == STOCK_HOGA0_PID)	
	{
		CRealData_Common realData_Common;
		CChartRealConverter::Convert_FX_STOCK_HOGA(pRealStruct, realData_Common);
		m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common, pRealType);
		return 1L;
	}
	else if(pRealType == MIGYULJE_PID)
	{
		CRealData_Common realData_Common;
		CChartRealConverter::Convert_ORDERINFO(pRealStruct, realData_Common);
		m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common, pRealType);
		return 1L;
	}
	
	return 0L;
}
*/
//@Solomon_GNF<--

// ������ ���� - ojtaso (20080519)
void CStdDialog::OnMagnifyingglass(BOOL bShow)
{
	if(bShow)
	{
		if(!m_pDlgGlass)	
		{
			m_pDlgGlass = new CDlgGlass(&m_ChartMng);
			
			AFX_MANAGE_STATE(AfxGetStaticModuleState());
			//HINSTANCE hInstResourceClient = AfxGetResourceHandle();
			//AfxSetResourceHandle(g_hInstance);

			m_pDlgGlass->Create(IDD_MAGNIFYINGGLASS, &m_ChartMng);
	
			//AfxSetResourceHandle(hInstResourceClient);
		}
		//hide�� ���ѳ��� ���� �ٽ� ��Ʈ���� ����� ���� ������.
		//hide ������ ���� ������ �ִ´�.
		else 
			m_pDlgGlass->MakeBitmap();
		
		m_pDlgGlass->ShowWindow(SW_SHOW);
		m_pDlgGlass->BringWindowToTop();
	}
	else
	{
		if(m_pDlgGlass)
			m_pDlgGlass->ShowWindow(SW_HIDE);

		m_pDlgToolbar->SetToggleButton(50073, 0);
		SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
	}
}

void CStdDialog::CreateBufferBitmap(CDC* pdc, const CRect& rcBounds)
{
	if (m_bufferBitmap.GetSafeHandle() == NULL)
	{
		m_bufferBitmap.CreateCompatibleBitmap(pdc, rcBounds.Width(), rcBounds.Height());
	}
	else
	{
		CSize bitmapSize = m_bufferBitmap.GetBitmapDimension();
		CSize boundsSize = rcBounds.Size();
		if (bitmapSize.cx < boundsSize.cx || bitmapSize.cy < boundsSize.cy)
		{
			m_bufferBitmap.DeleteObject();
			m_bufferBitmap.CreateCompatibleBitmap(pdc, boundsSize.cx, boundsSize.cy);
		}
	}
}

void CStdDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_pDlgGlass && m_pDlgGlass->IsWindowVisible())
	{
		m_pDlgGlass->SetWindowPos( NULL, point.x, point.y, 0, 0, 
				SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
	}
	CExOCXDLL::OnMouseMove(nFlags, point);
}

LRESULT CStdDialog::OnFormInitAfter(WPARAM wParam, LPARAM lParam)
{
	if(m_pInputBarDlg)
		m_pInputBarDlg->SetLastHistoryCode();
	PostMessage(UMSG_EDITCODE_INIT);
	
	return 0L;
}

BOOL CStdDialog::GetDlgToolBtnToggle(CArray<int, int>& arAnal, CArray<int, int>& arAnalCheck, CArray<int, int>& arAddition, CArray<int, int>& arAdditionCheck, CArray<int, int>& arUser, CArray<int, int>& arUserCheck, CMap<int, int, CString, CString>& mapToolName)
{
	if(m_pDlgToolbar && m_pDlgToolbar->GetSafeHwnd())
	{
		m_pDlgToolbar->GetIndexArray(0, arUser, arUserCheck);
		m_pDlgToolbar->GetIndexArray(1, arAnal, arAnalCheck);
		m_pDlgToolbar->GetIndexArray(2, arAddition, arAdditionCheck);
		m_pDlgToolbar->GetToolNameMap(mapToolName);
	}

	return TRUE;
}

void CStdDialog::SetDlgToolBtnToggle(int nToolBtnID, BOOL bToggle)
{
	m_pDlgToolbar->SetToggleBtnFromOutside(nToolBtnID);
}

#define getPALETTE 0x01 
#define winix_GetColor(nIdx)	(COLORREF)::SendMessage(m_hPlatform, WM_USER, MAKEWPARAM(getPALETTE, 0), nIdx);

LRESULT CStdDialog::OnChangeSkinColor(WPARAM wParam, LPARAM lParam)
{
	m_clrTabBkgrnd = m_clrBackground;
	m_clrOLBkgrnd = winix_GetColor(9);
	m_clrOLOutLine = winix_GetColor(10);
	
	g_pPtUtilMng->GetCurrentPalette();
	
	ChangeSkinColor();

//@SolomonGNF 	IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 3);
//@SolomonGNF 	if(!pKSLibMng) return 0;

//@SolomonGNF 	m_clrBackground = pKSLibMng->Lib_KSLink_GetColor(1);	// ����ȭ�� ���
//@SolomonGNF 	ChangeSkinColor();
	return 0;
}

COLORREF CStdDialog::GetBkgrndColor()
{
	return winix_GetColor(clridx_BackGround);
}

void CStdDialog::ChangeSkinColor()
{
	if((m_nChartType != MINI_MULTI_CHART && m_nChartType != MINI_SINGLE_CHART) && m_pLeftBarDlg)
		m_pLeftBarDlg->ChangeSkinColor();

	m_pInputBarDlg->ChangeSkinColor();
	m_pExtInputBarDlg->ChangeSkinColor();
	m_xSplitter1.Invalidate();
	m_pDlgToolbar->Invalidate();

	m_pInputBarDlg->ShowWindow(SW_HIDE);
	m_pInputBarDlg->ShowWindow(SW_SHOW);
}

LRESULT CStdDialog::OnGetSkinColor(WPARAM wParam, LPARAM lParam)
{
	int nColorType = (int)wParam;
	if(nColorType == 0)				// ����
		return (LRESULT)GetBkgrndColor();
	else if(nColorType == 1)		// Tab
	{
		STTabColor* pTabColor = (STTabColor*)lParam;
		if(pTabColor)
		{
			pTabColor->clrTabSelText = winix_GetColor(1);
			pTabColor->clrTabNorText = winix_GetColor(2);
			pTabColor->clrTabBkgrnd  = winix_GetColor(clridx_BackGround);
			
			g_pPtUtilMng->GetTabColor("", 0, pTabColor->clrTabSelGDStt, pTabColor->clrTabSelGDEnd);
			g_pPtUtilMng->GetTabColor("", 1, pTabColor->clrTabNorGDStt, pTabColor->clrTabNorGDEnd);
			
			return TRUE;
		}
		return FALSE;
	}
	else if(nColorType == 2)
	{
		STOutBarColor* pOLColor = (STOutBarColor*)lParam;
		if(pOLColor)
		{
			pOLColor->clrOLBkgrnd = m_clrOLBkgrnd;
			pOLColor->clrOLOutLine = m_clrOLOutLine;
			return TRUE;
		}
		return FALSE;
	}
	
	return DEFAULT_BKGRND_COLOR;
}

LRESULT CStdDialog::OnFormGetPageSave(WPARAM wParam, LPARAM lParam)
{
	CString strKey = (LPCSTR)lParam;
	strKey.TrimRight();
	m_strSaveKey.Format(_T("%s"), strKey);

	// �������� ���
//	m_strConfig = m_strUserDir + "\\" + m_strSaveKey + m_strScreenNo + "_" + ENVINFOFILE;
	//20080915 �̹��� �ּ�ó�� >>
	//m_strConfig = m_strUserDir + "\\" + m_strScreenNo + m_strSaveKey +  ENVINFOFILE;
	//20080915 �̹��� <<

	// Save ó��
//	SaveChart();

// --> [Edit]  ������ 2008/12/19
#ifdef	_LASTINFO_SAVE
//	FileCopyAndSaveAndDelete(LAST_HWND_COPY);
#endif
// <-- [Edit]  ������ 2008/12/19

	return 0L;
}

LRESULT CStdDialog::OnFormSetPageSave(WPARAM wParam, LPARAM lParam)
{
	CString strKey = (LPCSTR)lParam;
	strKey.TrimRight();
	m_strSaveKey.Format(_T("%s"), strKey);

	// �������� ���
//	m_strConfig = m_strUserDir + "\\" + m_strSaveKey + m_strScreenNo + "_" + ENVINFOFILE;
	//20080915 �̹��� �ּ�ó��>>
	//m_strConfig = m_strUserDir + "\\" + m_strScreenNo + m_strSaveKey +  ENVINFOFILE;
	//20080915 �̹��� <<
//	m_strSaveKey.Empty();
	return 0L;
}

CString CStdDialog::GetLinkInfoData()
{
	CString strCode;
//	if(m_hViewInMain)
//		::SendMessage(m_hViewInMain, RMSG_GET_LINKINFODATA, (WPARAM)0, (LPARAM)&strCode);

	return strCode;
}

void CStdDialog::SaveChart(BOOL bType)
{
	CString strTemp;
	if(!m_bSmallScreenMode)
	{
		CRect rect;
		if(m_pLeftBarDlg)
		{
			if(m_nWidthOfLeftBar>0)
			{
				strTemp.Format("%d",m_nWidthOfLeftBar);
				::WritePrivateProfileString("Frame","WidthOfLeftBar",strTemp,m_strConfig);
			}
			strTemp.Format("%d",GetLeftBarType());
			::WritePrivateProfileString("Frame","ShowLeftBar",strTemp,m_strConfig);
			strTemp.Format("%d",m_bShowExtInputBarDlg);
			::WritePrivateProfileString("Frame","ShowExtInputBarDlg",strTemp,m_strConfig);
			strTemp.Format("%d",m_bShowToolBar);
			::WritePrivateProfileString("Frame","ShowToolBar",strTemp,m_strConfig);
		}
	}
	else
	{
		strTemp.Format("%d",m_bShowToolBar);
		::WritePrivateProfileString("Frame","ShowToolBar",strTemp,m_strConfig);
	}

	if ( bType )
	{
		m_ChartMng.SaveChart(m_strConfig);
		if(m_nChartType != MINI_MULTI_CHART && m_nChartType != MINI_SINGLE_CHART)
			m_pLeftBarDlg->SetConfig(FALSE);
		m_pInputBarDlg->SaveInputBar();
		if(m_nChartType == MULTI_CHART)
			m_pExtInputBarDlg->SaveExtInputBar();
	}
}

// --> [Edit]  �̵��� 2008/09/11
// 
void CStdDialog::GetOrderInfo()
{
	//20091103 SJ_KIM KTB
	return;


// 	IKSLibManager* pKSLibManager = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 3);
// 
// 	CString	m_strUserID		= pKSLibManager->Lib_KSLink_GetLoginInfo("USER_ID");		/*ID(17)		: ����� IB_No */
// 
// 	OAQ69901_INREC Inrec;	
// 	memset(&Inrec, 0x00, sizeof(Inrec));
// 	memcpy(Inrec.userid, (LPSTR)(LPCTSTR)m_strUserID, m_strUserID.GetLength());
// 
// 	Send2Server(TROAQ69901, (char*)&Inrec, sizeof(Inrec), FALSE, 0);
}
// <-- [Edit]  �̵��� 2008/09/11

// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
CString	CStdDialog::GetRealItemCode( const char *p_szItemCode, BOOL	bOrgCode /* = FALSE */)
{
	if (!p_szItemCode) return "";
	if (!*p_szItemCode) return "";
	CString strItemCode(p_szItemCode);
	if (strItemCode.GetAt(0) == '|') strItemCode = strItemCode.Mid( 3);

	int nFind = strItemCode.Find('.');
	if (nFind != -1 && bOrgCode)
	{
		CString strName, strMarket, strLimitSize, strActCode;
		BOOL	bCalcInfo(0);
//		short	nPrecision;
		strItemCode = strItemCode.Left(nFind);
//@SolomonGNF	IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager, 3);
//@SolomonGNF	IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager);
//@SolomonGNF	if( pDataManager->GetGItemInfo( strItemCode, strName, strMarket, strLimitSize, bCalcInfo, nPrecision, strActCode) )
//@SolomonGNF		strItemCode = strActCode;
	}

	return strItemCode;
}

// (2008/9/14 - Seung-Won, Bae) for Pair Tick
void CStdDialog::StartRequestPairTick( const char *p_strPairTickCode, int p_nRequestPairTickState)
{
	// (2008/11/8 - Seung-Won, Bae) for Sync Drawing within Pair Tick.
	m_ChartMng.GetpChartItemSelected()->ClearChartData( 0);

	m_nRequestPairTickState = p_nRequestPairTickState;
	m_strPairTickCode = "|B_" + GetRealItemCode( p_strPairTickCode);
	m_ChartMng.GetpChartItemSelected()->SetCodeForQuery( m_strPairTickCode, QUERY_STRFOFOREIGN_HOGA2, TRUE);

	// (2008/9/24 - Seung-Won, Bae) delete C2_OBVIOUSLY_BALANCE_CHART in Tick
	CChartItem *pChartItem = m_ChartMng.GetpChartItemSelected();
	if( pChartItem)
	{
		pChartItem->ResetChart();
		pChartItem->RemoveChart( "�ŷ�����Ʈ");
		pChartItem->RemoveChart( "���� �̵����");
		pChartItem->ChangePriceGraphDrawingData( TRUE, TRUE, TRUE, FALSE, 3, 0);
	}
}
void CStdDialog::EndRequestPairTick( void)
{
	m_nRequestPairTickState = E_RPTS_STOP;
}
void CStdDialog::RequestPairTick( const char *p_strPairTickCode, int p_nRequestPairTickState)
{
	m_nRequestPairTickState = p_nRequestPairTickState;
	m_bOrgAddMode = m_ChartMng.GetAddModeFromTab();
	m_ChartMng.SetAddModeFromTab( TRUE);
	m_ChartMng.RequestChartPairTickData( p_strPairTickCode, p_nRequestPairTickState != E_PRTS_ROTATE_PAIR);
}
void CStdDialog::ClearPairTick( const char *p_strRQ)
{
// --> [Edit]  ������ 2008/11/03	( ü��/ȣ���ϰ�� TR�ڵ带 �������ش�. �Ƹ� ���⼭�� ü�������θ� �귯�� �Ұ� ����. )
//	if ( m_ChartMng.GetpChartItemSelected()->m_nCheHoga == 1 )
//		m_ChartMng.GetpChartItemSelected()->SetCodeForQuery( m_strPairTickCode, QUERY_STRFOFOREIGN_HOGA2, TRUE);
//	else
//		m_ChartMng.GetpChartItemSelected()->SetCodeForQuery( m_strPairTickCode, QUERY_STRFOFOREIGN, TRUE);
// <-- [Edit]  ������ 2008/11/03

	EndRequestPairTick();
	CString strPairRQ( p_strRQ);
	if( strPairRQ.Right( 5) != "_PAIR") strPairRQ = strPairRQ + "_PAIR";
	m_ChartMng.GetpChartItemSelected()->RemoveAllGraphOfRQ( strPairRQ);
}
int CStdDialog::GetRequestPairTickState( void)
{
	return m_nRequestPairTickState;
}
void CStdDialog::SelectMainRQ( void)
{
	CString strMainRQ = m_ChartMng.GetpChartItemSelected()->GetSelectedRQ();
	if( strMainRQ.Right( 5) == "_PAIR")
	{
		strMainRQ = strMainRQ.Left( strMainRQ.GetLength() - 5);
		m_ChartMng.GetpChartItemSelected()->SetCurrentRQ( strMainRQ);
	}
}
void CStdDialog::SetRequestPairTickState( int p_nRequestPairTickState)
{
	m_nRequestPairTickState = p_nRequestPairTickState;
}


//	���� �����Ϳ��� OffSet ������ �̿��Ͽ� ������ ���� �����ϴ� �Լ�
long CStdDialog::DataSeparation(CString strData, long nMask, long nTickSize, int nLog, int nPrecision) 
{
	int nVpoint = MAX_VPOINT;
	if(strData.GetLength() == 10 )//&& nLog == 1)
		nVpoint = MAX_VPOINT_PRICE;

	CString strInt = "", strFloat = "";
	long nResult(0);

	strData.TrimLeft();		strData.TrimRight();
	if ( strData.IsEmpty() )
		return 99999999;

	strFloat = strData.Right(nVpoint);
	strInt = strData.Left(strData.GetLength() - nVpoint );

//	strData.Format("%0.*f", nPrecision, atof(strData) / pow(10 , MAX_VPOINT) );

//	strData.Format("%s.%s", strInt, strFloat);
	if ( nMask <= 0 )
		nMask = 1;

	if ( nLog == 8 || nLog == 32 )
 	{
 		strData.Format("%s.%s", strInt, strFloat );
 		nResult = (long)(atof(strData) * (float)(nMask))/nTickSize;
 	}
	else
	{
/*
				if(strData == "") return 0;		
				if(strData.Left(1).CompareNoCase("0") == 0)
				{
					strData = strData.Mid(1, strData.GetLength() - 5);
				}
				return atof(strData);*/
		
		strData.Format("%0.*f", nPrecision, atof(strData) / pow(10 , nVpoint) );
//		nResult = (long)(atof(strData) * nMask);
		nResult = pow(10, nPrecision);
		double dValue = atof(strData) * (double)nResult;
		dValue += dValue * DBL_EPSILON;
		nResult = (long)(dValue);
		
//		if(nMask == 1)		
//		{
//			strFloat.Format("%f", atof(strData));
//			nResult = (long)(atof(strFloat));
//		}
//		else
//		{
////			m_nTickSize = 1;
//			strFloat.Format("%f", atof(strData));
//			float fGab = pow(10 , nPrecision*(-1))*5;
//			nResult = (long)((atof(strData) * (float)(nMask))+fGab);
//		}
	}

	return nResult;
}


//*******************************************************************/
/*! Function Name : SetFXMasterData
/*! Function      : FX �����ڵ带 ����Ʈ�� �����Ѵ�. 
/*! Param         : 
/*! Return        : void 
/*! Create        : ������ , 2008/08/06
/*! Comment       : 
//******************************************************************/
CString CStdDialog::GetTimeIndexInfo(int nIndex, int nType)
{
// --> [Edit]  ������ 2008/10/13
	CStringArray arrCode, arrName;

	CString strFileName = m_strRootPath + "\\Icss\\timesetup.ini";

	CString	strTemp, strSection, strKey, strData;
	CString	strTitle, strGmt, strSummerTime, strCityName;

	char	szItem[MAX_PATH];
	int nCount;

	strSection = "TIME";
	nCount = GetPrivateProfileInt (strSection, "COUNT", 0, strFileName); 

	if ( nCount < nIndex )
		return "";

	strKey.Format("%d", nIndex);
	memset(szItem, 0, MAX_PATH);
	GetPrivateProfileString(strSection, strKey, "", szItem, MAX_PATH, strFileName); 

	strTemp = szItem;
	int nCnt = strTemp.Find(",");
	strTitle= strTemp.Left(nCnt);	
	strTemp = strTemp.Mid(nCnt+1);	
	nCnt	= strTemp.Find(",");
	strGmt	= strTemp.Left(nCnt);

	// (2008/11/6 - Seung-Won, Bae) Check Index.
	if( strTemp.GetLength() < nCnt + 1) 
	{
		if ( nIndex == -1 && (nType != 1 || nType != 2) )
			return "�����ð���";
		
		return "";
		
	}

	strSummerTime	= strTemp.Mid(nCnt+1);	
	
	// ���ø� ����Ѵ�.
	nCnt = strTitle.Find('(');	
	strCityName = strTitle.Left(nCnt);	

	switch(nType)
	{
		case 1 :		//	�ð���
			{
				if ( strSummerTime == "1" )
					strData.Format( "%d", atoi(strGmt) + 1 );
				else
					strData.Format( "%d", atoi(strGmt) );
			}
			break;
		case 2 :		//	����Ÿ�� ����
			strData = strData;
			break;
		default :		//	Ÿ��Ʋ
			{
				if ( nIndex == -1 && (nType != 1 || nType != 2) )
					return "�����ð���";
				else
					strData = strTitle;
			}
			break;
	}

	return strData;
}

//*******************************************************************/
/*! Function Name : TbondPrcLToF
/*! Function      : �̱�ä ǥ�ú�ȯ TB Price long -> float decimal
/*! Param         : double inprice, 
/*!                 int scale, 
/*!                 int outdec
/*! Return        : long  
/*! Create        : �Ȱ�ȯ , 2006/09/14
/*! Comment       : 
//******************************************************************/
CString CStdDialog::TbondPrcLToF( CString inprice, int scale, int outdec, int ValidPoint, BOOL bFlag)
{
	double	dTemp(0.0);

	CString strReturn = "";
	CString strFloat = "", strInt = "";

	//	�Ǽ��κ� ©�󳻱�	(�� : 102'31.5 => 315 )
	strFloat = inprice.Right(outdec);
	//	�����κ� ©�󳻱�	(�� : 102'31.5 => 102 )
	strInt = inprice.Left(inprice.GetLength() - outdec);
	
	if ( scale == 8 )
	{
		dTemp = atof(strFloat);
		strReturn.Format("%f", (double)atoi(strInt) + (dTemp / scale));

//		strReturn.Remove('.');
	}
	else if ( scale == 32 )
	{
//		CString strTemp = "";
//		strTemp.Format("0.%s", strFloat);

		dTemp = atof(strFloat);
		if ( outdec == 4 )
			strReturn.Format("%f", (double)atoi(strInt) + (dTemp / scale / 100.0 ));
// --> [Edit]  ������ 2009/09/03
// �̱�ä 30�⹰ ƽ������ ���濡 ���� �߰�
		else if ( outdec == 2 )
			strReturn.Format("%f", (double)atoi(strInt) + (dTemp / scale ));
// <-- [Edit]  ������ 2009/09/03
		else
			strReturn.Format("%f", (double)atoi(strInt) + (dTemp / scale / 10.0 ));

//		strReturn.Remove('.');
	}
	else
	{
		if ( bFlag )
		{
			dTemp = (double)(atol(inprice) / (double)pow(10 , ValidPoint));
			strReturn.Format("%f", dTemp);
		}
		else
			strReturn = inprice;

		
		return strReturn;
	}
	
	return strReturn;
}

long CStdDialog::DataSeparation2(CString strData, long nMask, long nTickSize, int nLog, int nPrecision) 
{
	CString strInt = "", strFloat = "";
	long nResult(0);

	strData.TrimLeft();		strData.TrimRight();
	if ( strData.IsEmpty() )
		return 99999999;

	if ( nMask <= 0 )
		nMask = 1;

	if ( nLog == 8 )
	{
		nResult = (long)((atof(strData) * (float)(nMask))/nTickSize);
	}
	else if ( nLog == 32 )
	{
		nResult = (long)((atof(strData) * (float)(nMask))/nTickSize);
	}
	else
	{
//		strData.Format("%0.*f", nPrecision, atof(strData) / pow(10 , MAX_VPOINT) );
//		nResult = (long)((atof(strData) * pow(10, nPrecision)));
		nResult = atol(strData);
	}

	return nResult;
}

void CStdDialog::ConvertKoscomData(LPCSTR szTR, char* pData, long dwTRDateLen)
{
	CString strTRCode = szTR;
	strTRCode.Remove(' ');
	if (strTRCode != QUERY_STRFOFOREIGN)
		return;

	m_strOut = "";
	if (pData == NULL)
		return;

	EU_p0602_OutRec1	singOutput;
	memcpy(&singOutput, pData, sizeof(EU_p0602_OutRec1));

	CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
	ST_SELCHART *pSelChart = pChartItem->m_lpSelChart;

	if (!pSelChart)
		pChartItem->GetSelChart(pChartItem->GetSelectedRQ(), pSelChart);

	CString strTerm = pChartItem->GetTickMinDayNumberRequested();
	char chTypeRequested = pChartItem->GetTypeRequested();
	if (pSelChart)
	{
		pSelChart->m_strKey = CString(singOutput.nkey, sizeof(singOutput.nkey));
		pSelChart->m_nOrgTimeIndex = atoi("22");
		if (chTypeRequested != pSelChart->m_cType)
			chTypeRequested = pSelChart->m_cType;

		CString strCodeType;
		char cRet = singOutput.codetype[0];
		strCodeType.Format(_T("%c"), cRet);
		if (cRet >= '0' && cRet <= '9')
		{
			int nType = atoi(strCodeType) + 1;
			strCodeType.Format(_T("%d"), nType);
		}
		SetCodeType(pSelChart->m_strItemCode, strCodeType);
	}

	ST_CHARTITEM_REALDATA* pChartItemRealData = pChartItem->GetChartItemRealData();
	if (pChartItemRealData)
	{
		//	��ȣ
		CString	strSign = "", strChange = "", strDrate = "", strOpen = "", strHigh = "";
		CString	strLow = "", strPrice = "", strName = "", strStartDate= "", strEndDate = "";
		strSign = CString(singOutput.sign, sizeof(singOutput.sign));
		strChange = CString(singOutput.change, sizeof(singOutput.change));	//	���ϴ��
		strDrate = CString(singOutput.chrate, sizeof(singOutput.chrate));	//	�����
		strOpen = CString(singOutput.open, sizeof(singOutput.open));		//	�ð�
		strHigh = CString(singOutput.high, sizeof(singOutput.high));		//	��
		strLow = CString(singOutput.low, sizeof(singOutput.low));			//	����
		strPrice = CString(singOutput.price, sizeof(singOutput.price));		//	����
		strName = CString(singOutput.name, sizeof(singOutput.name));
		strName.TrimRight();
		strStartDate = CString(singOutput.startDate, sizeof(singOutput.startDate));
		strEndDate = CString(singOutput.endDate, sizeof(singOutput.endDate));

		//	��ȣ
		//if (!strSign.IsEmpty())		pChartItemRealData->m_chSign = strSign.GetAt(0);
		//else						pChartItemRealData->m_chSign = ' ';
		strSign.TrimLeft();		strSign.TrimRight();
		if( strSign.IsEmpty() )
		{
			char cSign = strChange.GetAt(0);
			switch(cSign)
			{
			case ' ': strSign = "3"; break;
			case '+': strSign = "2"; break;
			case '-': strSign = "5"; break;
			default : strSign = "3"; break;
			}
		}
		pChartItemRealData->m_chSign = strSign.GetAt(0);


//		pChartItemRealData->m_strTime = strCodeTime;	//	�ŷ��� ����ð�
		pChartItemRealData->m_strChange = strChange;	//	���ϴ��
		pChartItemRealData->m_strChrate = strDrate;		//	�����
		pChartItemRealData->m_strOpen = strOpen;		//	�ð�
		pChartItemRealData->m_strHigh = strHigh;		//	��
		pChartItemRealData->m_strLow = strLow;			//	����
		pChartItemRealData->m_strPrice = strPrice;		//	����
		pChartItemRealData->m_strItemName = strName;	//	ǰ���
		pChartItemRealData->m_strTimeIndex = "22";

		pChartItemRealData->m_nRealType = STOCK_SISE_PID;

		pChartItemRealData->m_nStartDate = atol(strStartDate);
		pChartItemRealData->m_nEndDate = atol(strEndDate);
	}

	m_strOut.Format(_T("%s"), (LPTSTR)(char*)(pData+sizeof(EU_p0602_OutRec1)));
}

/*
void CStdDialog::ConvertKoscomData(LPCSTR szTR, char* pData, long dwTRDateLen)
{
	TRACE("dwTRDateLen=%d\n", dwTRDateLen);

	CString strTRCode = szTR;
	strTRCode.Remove(' ');

	if(strTRCode == QUERY_STRFOFOREIGN_HOGA2)
	{
		m_strOut = "";
		int nDataCnt(0);

		if ( pData == NULL )
		{
			m_strOut = "";
			nDataCnt = 0;
		}

		CString		strData = "", strTemp = "", strPipLowest = "", strOut = "";
		char		szData[1024];

		int nTemp(0);
		char cTemp[128];

		HCQ03011_OUT_KEY		singOutput;
		HCQ03011_OUT			output;
		o56501_OUT_HEADER		sto56051_Out_Head;	
		memset(&sto56051_Out_Head, 0x00, o56501_OUT_SZ);
		memcpy(&sto56051_Out_Head, pData + HCQ03011_OUT_KEY_SZ, o56501_OUT_SZ);

		memset(cTemp, 0x00, 128);
		memcpy(cTemp, sto56051_Out_Head.count, sizeof(sto56051_Out_Head.count));
		nDataCnt = atoi(cTemp);

//		nDataCnt = (dwTRDateLen - HCQ03011_OUT_KEY_SZ) / HCQ03011_OUT_SZ;		//	�������� ������ ���Ѵ�.

		if ( dwTRDateLen < HCQ03011_OUT_KEY_SZ )
		{
			m_strOut = "";
			nDataCnt = 0;
		}

		if ( nDataCnt <= 0 || dwTRDateLen <= 0 )			//	���� �����Ͱ� ���ڵ��� ���̺��� ������� �����̹Ƿ� return
		{
			m_strOut = "";
			nDataCnt = 0;
		}

		memset(&singOutput, 0x00, HCQ03011_OUT_KEY_SZ);
		if ( dwTRDateLen >= HCQ03011_OUT_KEY_SZ )
			memcpy(&singOutput, pData, HCQ03011_OUT_KEY_SZ);	//	KeyValue�� ������ ������ ©�󳽴�.

		memset(szData, 0x00, sizeof(szData));
		memset(&output, 0x00, HCQ03011_OUT_SZ);

		if ( nDataCnt > 0 )
		{
			memcpy(szData, pData + HCQ03011_OUT_KEY_SZ, HCQ03011_OUT_SZ);	//	KeyValue�� ������ ������ ©�󳽴�.
			memcpy(&output, pData + HCQ03011_OUT_KEY_SZ, HCQ03011_OUT_SZ);	//	KeyValue�� ������ ������ ©�󳽴�.
		}

		strData = szData;

		CString			strTickSize = "1";
		CString			strTickValue, strPriceInfo;

		int				nLog(1);
		int				nLogDispSize(0);
		int				nPrecision(0);
		int				ValidPoint(0);
		int				nMask(1);
		long			nTickSize(1);

		// (2008/9/14 - Seung-Won, Bae) for Pair Tick
		char *szPrice = NULL;
		CString strItemCode;
		CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
		if( pChartItem ) if( pChartItem->m_lpSelChart)
			strItemCode = pChartItem->m_lpSelChart->m_strItemCode;
		BOOL bAskItem = FALSE;
		if( !strItemCode.IsEmpty()) if( strItemCode.GetAt( 0) == '|') if( strItemCode.GetAt( 1) == 'S') bAskItem = TRUE;


		//	�ؽ�Ʈ Ű��
		ST_SELCHART *pSelChart = pChartItem->m_lpSelChart;

		if ( !pSelChart )
			pChartItem->GetSelChart(pChartItem->GetSelectedRQ(), pSelChart);

		if ( pSelChart )
			pSelChart->m_strKey = CString(sto56051_Out_Head.keyvalue, sizeof(sto56051_Out_Head.keyvalue));

		pChartItem->m_strNextKey = CString(sto56051_Out_Head.keyvalue, sizeof(sto56051_Out_Head.keyvalue));
		char chTypeRequested = pChartItem->GetTypeRequested();

		strItemCode.TrimLeft();		strItemCode.TrimRight();
		//@SolomonGNF IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager, 3);
		IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager);
		//@SolomonGNF if ( !pDataManager->GetGJongmokInfo( CStdDialog::GetRealItemCode( ( LPCTSTR)strItemCode, TRUE), nLog, nLogDispSize, nPrecision, strTickSize, strTickValue, strPriceInfo ) )
		if(TRUE)
		{
			nLog			= 1;
			nLogDispSize	= 0;
			nPrecision	= 0;

			ValidPoint	= 0;
			nMask			= 0;

			nTickSize		= 1;
		}
		else
		{
			strTickSize.TrimLeft(); strTickSize.TrimRight();
			CString strTmp = strTickSize.Right(1);

			while( strTmp == "0" )
			{
				strTickSize = strTickSize.Left(strTickSize.GetLength()-1);
				strTmp = strTickSize.Right(1);
			};

			nMask = strTickSize.GetLength() - 2;

			if ( nMask < 0 )
				nMask = 0;

			nMask = pow(10, nMask);
			ValidPoint = nPrecision;

			nTickSize = (long)(atof(strTickSize)*nMask);

		}

		CString	strTickSize2 = "";
		int nPoint(0);
		if ( nLog == 1 )	//	10�����ϰ�� ���� ������� �¿��޶�� ��û������ 8,32 �������� �������ش�.
		{
			nPoint = nPrecision;
			strTickSize.Format("%0.*f", nPoint, 1 / pow(10, nPoint));
			strTickSize2.Format("%0.*f", nPoint, 1 / pow(10, nPoint - 1));		//	��ƽ�� ������... 
		}
		else if ( nLog == 8 )
		{
			//	��Ʈ���� 100�� ���ؼ� �޶�� ��... 8�����ϰ��...
			strTickSize2.Format("%0.8f", atof(strTickSize) * 100 );
		}
		else if ( nLog == 32 )
		{
			strTickSize2.Format("%0.8f", atof(strTickSize)  );
		}		


		for(int i = nDataCnt - 1; i >= 0 ; i--)
		{
			memset(&output, 0x00, o56501_OUT_SZ);
			memcpy(&output, pData + ( i * HCQ03011_OUT_SZ + o56501_OUT_SZ ) + HCQ03011_OUT_KEY_SZ, HCQ03011_OUT_SZ);	//	KeyValue�� ������ ������ ©�󳽴�.

			if ( chTypeRequested == TICK_DATA_CHART )
			{
				//	�ð�
				memset(cTemp, 0x00, 128);
				memcpy(cTemp, output.date + 6, 8 );
				strTemp = cTemp;
				strTemp.Remove('.');
				strData.Format("%8s", strTemp);
				strOut += strData;
			}


			if ( chTypeRequested == TICK_DATA_CHART )		//	ƽ��Ʈ������ �ð������� ���������� ����
			{
				if ( !bAskItem )
				{
					//	�ż�ȣ��
					memset(cTemp, 0x00, 128);
					memcpy(cTemp, output.mshoga_d, sizeof(output.mshoga_d));
					strTemp = cTemp;
					strTemp.Format("%d", DataSeparation(strTemp, nMask, nTickSize, nLog, nPrecision));
					strData.Format("%10s", strTemp);

					strOut += strData;
					strOut += strData;
					strOut += strData;
					strOut += strData;
				}
				else
				{
					//	�ŵ�ȣ��
					memset(cTemp, 0x00, 128);
					memcpy(cTemp, output.mdhoga_d, sizeof(output.mdhoga_d));
					strTemp = cTemp;
					strTemp.Format("%d", DataSeparation(strTemp, nMask, nTickSize, nLog, nPrecision));
					strData.Format("%10s", strTemp);

					strOut += strData;
					strOut += strData;
					strOut += strData;
					strOut += strData;
				}

			}

			
			if ( chTypeRequested == TICK_DATA_CHART )
			{
				if ( !bAskItem )
				{
					//	�ż��ܷ�
					memset(cTemp, 0x00, 128);
					memcpy(cTemp, output.mshorem, sizeof(output.mshorem));
					strTemp.Format("%.0f", atof(cTemp));
					strData.Format("%10s", strTemp);
					strOut += strData;
				}
				else
				{
					//	�ŵ��ܷ�
					memset(cTemp, 0x00, 128);
					memcpy(cTemp, output.mdhorem, sizeof(output.mdhorem));
					strTemp.Format("%.0f", atof(cTemp));
					strData.Format("%10s", strTemp);
					strOut += strData;
				}
			}

			//	�Ǹ��� ( �ϴ� �츮 ���������Ϳ��� ���µ�.. ���⿡�� ��Ʈ �Ѱ��ִ� �����Ϳ��� �ʿ���... ���߿� �����ؾ��Ұ� ���� )
//			strOut += "0";
		}

		//  ���� ����Ǵ� ǰ�񿡼��� ���������� �Ҽ��� �ڸ����� -1�� �����ش޶�� ��û
//		if ( nLog != 1 )
			ValidPoint = -1;

		CString strBojoyMsg = "";

		//	���ذ�
		CString strPrePrice = "";
		strPrePrice = CString(singOutput.jsprice, sizeof(singOutput.jsprice));
		strPrePrice = TbondPrcLToF(strPrePrice, nLog, nLogDispSize, ValidPoint, TRUE);
//		strPrePrice.Format("%ld", DataSeparation2(strPrePrice, nMask, nTickSize, nLog, nLogDispSize));
		strPrePrice.TrimLeft();
		//	�ð���
		CString strTimeIndex = "", strTimeName = "";
		//20091117 SJ_KIM 01�� ���� ���� ����.
	//	strTimeName = strTemp = CString(singOutput.timeindex, sizeof(singOutput.timeindex));
		strTimeName = strTemp = "01";
		strTimeIndex.Format("%s", GetTimeIndexInfo(atoi(strTemp), 1));
		//	�����
		CString	strMarketStart = "";
		strMarketStart = CString(singOutput.estime, sizeof(singOutput.estime));
		//	�帶��
		CString	strMarketEnd = "";
		strMarketEnd = CString(singOutput.eetime, sizeof(singOutput.eetime));
		//	ǰ���� ����ð�
		CString	strCodeTime = "";
		strCodeTime = CString(singOutput.date, sizeof(singOutput.date) + sizeof(singOutput.time));
		//	������ �߰�
		CString	strBDay = "";
		strBDay = CString(singOutput.bday, sizeof(singOutput.bday));

		CString strName = "";
		ST_CHARTITEM_REALDATA* pChartItemRealData = pChartItem->GetChartItemRealData();
		if( pChartItemRealData )
		{
			//	��ȣ
			CString	strSign = "";
			strSign = CString(singOutput.sign, sizeof(singOutput.sign));
			//	���ϴ��
			CString	strChange = "";
			strChange = CString(singOutput.change, sizeof(singOutput.change));
			strChange = TbondPrcLToF(strChange, nLog, nLogDispSize, ValidPoint, FALSE);
			strChange.Format("%ld", DataSeparation2(strChange, nMask, nTickSize, nLog, nLogDispSize));
			strChange.TrimLeft();
			//	�����
			CString	strDrate = "";
			strDrate = CString(singOutput.drate, sizeof(singOutput.drate));
			strDrate.TrimLeft();
			//	�ð�
			CString	strOpen = "";
			strOpen = CString(singOutput.open, sizeof(singOutput.open));
			strOpen = TbondPrcLToF(strOpen, nLog, nLogDispSize, ValidPoint, FALSE);
			strOpen.Format("%ld", DataSeparation2(strOpen, nMask, nTickSize, nLog, nLogDispSize));
			strOpen.TrimLeft();
			//	��
			CString	strHigh = "";
			strHigh = CString(singOutput.high, sizeof(singOutput.high));
			strHigh = TbondPrcLToF(strHigh, nLog, nLogDispSize, ValidPoint, FALSE);
			strHigh.Format("%ld", DataSeparation2(strHigh, nMask, nTickSize, nLog, nLogDispSize));
			strHigh.TrimLeft();
			//	����
			CString	strLow = "";
			strLow = CString(singOutput.low, sizeof(singOutput.low));
			strLow = TbondPrcLToF(strLow, nLog, nLogDispSize, ValidPoint, FALSE);
			strLow.Format("%ld", DataSeparation2(strLow, nMask, nTickSize, nLog, nLogDispSize));
			strLow.TrimLeft();
			//	����
			CString	strPrice = "";
			strPrice = CString(singOutput.price, sizeof(singOutput.price));
			strPrice = TbondPrcLToF(strPrice, nLog, nLogDispSize, ValidPoint, FALSE);
			strPrice.Format("%ld", DataSeparation2(strPrice, nMask, nTickSize, nLog, nLogDispSize));
			strPrice.TrimLeft();

			strName = CString(singOutput.jmname, sizeof(singOutput.jmname));
			strName.TrimLeft();	strName.TrimRight();

			//	��ȣ
			if ( !strSign.IsEmpty() )
				pChartItemRealData->m_chSign = strSign.GetAt(0);
			else
				pChartItemRealData->m_chSign = ' ';
			//	�ð�
//			int nLen = strCodeTime.GetLength();
//			if ( nLen >= 6 )
//				pChartItemRealData->m_strTime.Format("%s:%s:%s", strCodeTime.Mid(nLen-6, 2), strCodeTime.Mid(nLen-4, 2), strCodeTime.Mid(nLen-2, 2));
// --> [Edit]  ������ 2008/11/01
			//	�ϴ� �ŷ��� �ð����� �����ش�.. ü��ð��� TR �����ؾ���
			pChartItemRealData->m_strTime = strCodeTime;		//	�ŷ��� ����ð�
//			pChartItemRealData->m_strTime = strCheTime;			//	ü��ð�
// <-- [Edit]  ������ 2008/11/01
//			pChartItemRealData->m_strTimeGab = strTimeIndex;
			//	���ϴ��
			pChartItemRealData->m_strChange = strChange;
			//	�����
			pChartItemRealData->m_strChrate = strDrate;
			//	�ð�
			pChartItemRealData->m_strOpen = strOpen;
			//	��
			pChartItemRealData->m_strHigh = strHigh;
			//	����
			pChartItemRealData->m_strLow = strLow;
			//	����
			pChartItemRealData->m_strPrice = strPrice;
			//	ǰ���
			pChartItemRealData->m_strItemName = strName;
		//	pChartItemRealData->m_strItemName = strName = pDataManager->GetCodeName( CStdDialog::GetRealItemCode( ( LPCTSTR)strItemCode ) );

//			long lResult(0);
//			CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
//			if(pTabCodeSearch)
//			{
//				lResult = pTabCodeSearch->GetCurListIndex();
//			}
//
//			if (lResult > 0 )
//			{
//////				long lResult = pChartItem->SendMessage(UDM_CHANGE_TIMEDIFF, 0, 1);
//////				if ( lResult < 1 )
//////				{
//////					SetTabTimeIndex(atol(strTimeName));
//////				}
////				strTimeName.TrimLeft();
////				pChartItemRealData->m_strTimeName = GetTimeIndexInfo(lResult, 0);
////				SetTabTimeIndex(lResult);
////				ChangeTimeSet(lResult);
//				pChartItemRealData->m_strTimeIndex = strTimeName;
//////				m_ChartMng.m_strTimeSet = strTimeName;
//			}
////			else
			{
//				long lResult = pChartItem->SendMessage(UDM_CHANGE_TIMEDIFF, 0, 1);
//				if ( lResult < 1 )
//				{
//					SetTabTimeIndex(atol(m_ChartMng.m_strTimeSet));
//				}
//				pChartItemRealData->m_strTimeName = GetTimeIndexInfo(atoi(m_ChartMng.m_strTimeSet), 0);
				pChartItemRealData->m_strTimeIndex = strTimeName;
			}

			pChartItemRealData->m_nRealType = 0; //@SolomonGNF STOCK_HOGA0_PID;

			//	ó���� ������ �ð��� ���� ��¥�� �����Ѵ�. 
			CString strDateToday;
			CTime t = CTime::GetCurrentTime();
			strDateToday = t.Format("%Y%m%d");

			pChartItemRealData->m_nStartDate = atol(strDateToday);
			pChartItemRealData->m_nEndDate = atol(strDateToday);

		}
		else
		{
#ifdef _DEBUG
			AfxMessageBox("�̸��� �����°� ����??");
#endif
		}

		CString strNext = "E";
		BOOL bNext = pChartItem->GetNext();
		if ( bNext )
			strNext = "A";
		else
			strNext = "E";

		if ( chTypeRequested == TICK_DATA_CHART )
		{
			if ( bNext )
			{
				strBojoyMsg.Format("UM=%s@UD=5@RDATET=DDHHMMSS@TDC=%s:�ð�,|%s:��,|%s:����,|%s:����,|1:�⺻�ŷ���,@RDC=%s:%d:�ð�,|%s:%d:��,|%s:%d:����,|%s:%d:����,|1:�⺻�ŷ���,@REALTIMET=HHMMSS@TIMEDIFF=%s@@MARKETTIME=%s|%s@DATATIME=%s@CODEANDNAME=%s:%s@MARKETDAY=%s@RESETUSEREAL=%d@HBPRICE=%s@", 
									strNext, strTickSize, strTickSize, strTickSize, strTickSize, 
									strTickSize, ValidPoint, strTickSize, ValidPoint, strTickSize, ValidPoint, strTickSize, ValidPoint,
									strTimeIndex, strMarketStart, strMarketEnd, strCodeTime, strItemCode, strName, strBDay, GetRealCheck(), strTickSize2);
			}
			else
			{
				strBojoyMsg.Format("UM=%s@UD=5@RDATET=DDHHMMSS@TDC=%s:�ð�,|%s:��,|%s:����,|%s:����,|1:�⺻�ŷ���,@RDC=%s:%d:�ð�,|%s:%d:��,|%s:%d:����,|%s:%d:����,|1:�⺻�ŷ���,@REALTIMET=HHMMSS@TIMEDIFF=%s@PREVPRICE=����:%s@MARKETTIME=%s|%s@DATATIME=%s@CODEANDNAME=%s:%s@MARKETDAY=%s@RESETUSEREAL=%d@HBPRICE=%s@", 
								strNext, strTickSize, strTickSize, strTickSize, strTickSize, 
								strTickSize, ValidPoint, strTickSize, ValidPoint, strTickSize, ValidPoint, strTickSize, ValidPoint,
								strTimeIndex, strPrePrice, strMarketStart, strMarketEnd, strCodeTime, strItemCode, strName, strBDay, GetRealCheck(), strTickSize2);
			}
//			strBojoyMsg = "UM=E@UD=5@RDATET=DDHHMMSS@REALTIMET=YYYYMMDDHHMMSS@";
		}
		else
		{
			return;
		}

//		int n = strBojoyMsg.GetLength();

		CString strBojoMsg;
		strBojoMsg.Format("0496%-496s", strBojoyMsg);
		strTemp.Format("%05d", nDataCnt);

//		strBojoyMsg = strTemp + strBojoMsg;

		m_strOut = strTemp + strBojoMsg + strOut;


#ifdef _WRITELOG
		// Log - ojtaso (20080107)
		WriteToStringLog("�ؿ�ǰ�� ȣ�������� �������� ����::End", strCode, strCode.GetLength());
#endif


	}
	else if(strTRCode == QUERY_STOCK_HOGA)
	{
		if ( pData == NULL )
		{
			return;
		}

		if ( dwTRDateLen < HCQ01112_OUT_SZ )
		{
			return;
		}

		char		szData[1024];
		memset(szData, 0x00, sizeof(szData));
		memcpy(szData, pData, HCQ01112_OUT_SZ);	//	KeyValue�� ������ ������ ©�󳽴�.

		m_strOut = szData;

		int n = m_strOut.GetLength();

#ifdef _WRITELOG
		// Log - ojtaso (20080107)
		WriteToStringLog("�ؿ�ǰ�� ȣ�������� �������� ����::End", strCode, strCode.GetLength());
#endif


	}
}
*/

BOOL CStdDialog::GetRealCheck()
{
	return m_pExtInputBarDlg->GetRealUpdateCheck();
}

void CStdDialog::SetTabTimeIndex(int nIndex)
{
	CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
	if(pTabCodeSearch)
	{
		pTabCodeSearch->SetCurListIndex( nIndex );
	}	
}

int CStdDialog::GetSummerTimeInfo(int nStart, int nEnd) 
{
	//	��ī�� �ð��� nStart 1 nEnd 7�� �Է��ؾ� �Ѵٰ� �ؼ�... �ϴ� �������� �ڴ´�. 
	nStart	= 1;
	nEnd	= 7;

	CTime	tToday = CTime::GetCurrentTime();
	CTime	tStart, tEnd;
	CString strStart, strEnd;

	tToday.GetCurrentTime();
	int nYear = tToday.GetYear();

	//	����Ÿ�� ���� ��¥
	switch(nStart)
	{
// --> [Edit]  ������ 2007/2/5 
// 	case 1 :	//	4�� ù��° �Ͽ����� ���Ѵ�.
// 		{
// 			CTime	t(tToday.GetYear(), 4, 1, 1, 0, 0);
// 			int nWeek = 8 - t.GetDayOfWeek();
// 
// 			CTimeSpan cts = CTimeSpan(nWeek, 0, 0, 0);     // 1��
// 			tStart = t + cts;
// 		}
// 		break;
	case 1:		//	3�� ��°�� �Ͽ����� ���Ѵ�.
		{
			
			//	( ����ó��... 3������ ������� Ŭ��� 1���� 3���� ��¥�� ���Ѵ�)
			CTime	t(nYear, 3, 1+7, 1, 0, 0);
			int nWeek = 8 - t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek, 0, 0, 0);     // 1��
			tStart = t + cts;
		}
		break;

// <-- [Edit]  ������ 2007/2/5 
	case 2 :	//	3�� ������ �Ͽ����� ���Ѵ�.
		{
			CTime	t(tToday.GetYear(), 3, 31, 1, 0, 0);
			int nWeek = t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek-1, 0, 0, 0);     // 1��
			tStart = t - cts;
		}
		break;
	case 3 :	//	4�� ������ �Ͽ����� ���Ѵ�.
		{
			CTime	t(tToday.GetYear(), 4, 30, 1, 0, 0);
			int nWeek = t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek-1, 0, 0, 0);     // 1��
			tStart = t - cts;
		}
		break;
	case 4 :	//	10�� ��°�� �Ͽ����� ���Ѵ�. 
		{
			//	( ����ó��... 10������ ������� ������� 1���� 10���� ��¥�� ���Ѵ�)
			if ( tToday.GetMonth() < 10 )
			{
				nYear -= 1;
			}
			CTime	t(nYear, 10, 1+7, 1, 0, 0);
			int nWeek = 8 - t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek, 0, 0, 0);     // 1��
			tStart = t + cts;
		}
		break;
	case 5 :	//	4�� 1���� ���Ѵ�.
		{
			CTime	t(tToday.GetYear(), 4, 1, 1, 0, 0);
			tStart = t;
		}
		break;
	case 6 :	//	5�� 1���� ���Ѵ�.
		{
			CTime	t(tToday.GetYear(), 5, 1, 1, 0, 0);
			tStart = t;
		}
		break;
	case 7 :	//	10�� �������� �Ͽ����� ���Ѵ�.
		{
			//	( ����ó��... 10������ ������� ������� 1���� 10���� ��¥�� ���Ѵ�)
 			if ( tToday.GetMonth() < 10 )
 			{
 				nYear -= 1;
 			}
			CTime	t(nYear, 10, 31, 1, 0, 0);
			int nWeek = t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek-1, 0, 0, 0);     // 1��
			tStart = t - cts;	
		}
		break;
	case 8 :	//	10�� ù��° �Ͽ����� ���Ѵ�.
		{
			CTime	t(tToday.GetYear(), 10, 1, 1, 0, 0);
			int nWeek = 8 - t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek, 0, 0, 0);     // 1��
			tStart = t + cts;
		}
		break;
	default :
		{
			return 0;
		}
		break;
	}

	//	����Ÿ�� ������ ��¥
	switch(nEnd)
	{
	case 1 :	//	10�� ������ �Ͽ����� ���Ѵ�.
		{
			CTime	t(tToday.GetYear(), 10, 31, 1, 0, 0);
			int nWeek = t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek-1, 0, 0, 0);     // 1��
			tEnd = t - cts;
		}
		break;
	case 2 :	//	2�� ��°�� �Ͽ����� ���Ѵ�.
		{
			//	( ����ó��... 2������ ������� Ŭ��� 1���� 2���� ��¥�� ���Ѵ�)
			nYear += 1;
			CTime	t(nYear, 2, 1+7, 1, 0, 0);
			int nWeek = 8 - t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek, 0, 0, 0);     // 1��
			tEnd = t + cts;
		}
		break;
	case 3 :	//	3�� ��°�� �Ͽ����� ���Ѵ�.
		{
			//	( ����ó��... 3������ ������� Ŭ��� 1���� 3���� ��¥�� ���Ѵ�)
				nYear += 1;
			CTime	t(nYear, 3, 1+7, 1, 0, 0);
			int nWeek = 8 - t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek, 0, 0, 0);     // 1��
			tEnd = t + cts;
		}
		break;
	case 4 :	//	10�� 1���� ���Ѵ�. 
		{
			CTime	t(tToday.GetYear(), 10, 1, 1, 0, 0);
			tEnd = t;
		}
		break;
	case 5 :	//	3�� �������� �Ͽ����� ���Ѵ�.
		{
			//	( ����ó��... 3������ ������� Ŭ��� 1���� 3���� ��¥�� ���Ѵ�)
			nYear += 1;
			CTime	t(nYear, 3, 31, 1, 0, 0);
			int nWeek = t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek-1, 0, 0, 0);     // 1��
			tEnd = t - cts;
		}
		break;
	case 6 :	//	3�� ��°�� �Ͽ����� ���Ѵ�.
		{
			//	( ����ó��... 3������ ������� Ŭ��� 1���� 3���� ��¥�� ���Ѵ�)

			nYear += 1;
			CTime	t(nYear, 3, 1+7+7, 1, 0, 0);
			int nWeek = 8 - t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek, 0, 0, 0);     // 1��
			tEnd = t + cts;
		}
		break;
// --> [Edit]  ������ 2007/2/5 	
	case 7 :	//	11�� ù°�� �Ͽ����� ���Ѵ�.
		{
			CTime	t(tToday.GetYear(), 11, 1, 1, 0, 0);
			int nWeek = 8 - t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek, 0, 0, 0);     // 1��
			tEnd = t + cts;
		}
		break;
// <-- [Edit]  ������ 2007/2/5 

	default :
		{
			return 0;
		}
		break;
	}

//	strStart.Format("%d�� %d�� %d��", tStart.GetYear(), tStart.GetMonth(), tStart.GetDay());
//	strEnd.Format("%d�� %d�� %d��", tEnd.GetYear(), tEnd.GetMonth(), tEnd.GetDay());
//	ASSERT (tStart < tEnd);

	if ( tToday > tStart && tToday < tEnd )
		return 1;
	else
		return 0;

}

CString CStdDialog::Gl_Format32String(long nTBValue, int scale, int outdec, int indec, UINT uMask) 
{

	CString strValue = "", strInt = "", strMid = "", strFloat = "", strResult = "";
	long	nValue(0);

	if ( scale < 0 || outdec < 0 || indec < 0 )
	{
		strValue.Format("%d", nTBValue);
		return strValue;
	}

	//	���� 0�ϰ�쿡�� Format�� ���� ������� �ʰ� �����Ѵ�.
	if ( nTBValue == 0 )
	{
		CString strZero = "00000000000000000000";
		if ( scale == 1 )
			return "0";
		else if ( scale == 8 || scale == 32 )
		{
			if ( indec == 0 )
				strValue.Format("0'%s", strZero.Left(outdec));
			else
				strValue.Format("0'%s.%s", strZero.Left(outdec - indec), strZero.Left(indec) );
		}
	}

	int nMask = uMask;
	if( nMask == 0 )
		nMask = 1;

	BOOL	bFlag = FALSE;
	if ( nTBValue < 0 )
	{
		nTBValue = abs(nTBValue);
		bFlag = TRUE;
	}

	double dValue = (double)nTBValue/(double)nMask;

	nValue = FtoTbondPrcL( dValue, scale, outdec );

//	if ( scale == 1 )		//	10�����ϰ��
//	{
//		strValue.Format("%0.*f", indec, dValue);
//		return strValue;
//	}

//	nTBValue = nTBValue/nMask;

	//	32�����ϰ�� ����ó��
	if ( scale == 32 || scale == 8 ) 
	{

		int nMidSize = 2;
		strValue.Format("%d", nValue);

		if ( strValue.GetLength() < outdec )
		{
			int nCnt = strValue.GetLength();
			for ( int i = 0 ; i < outdec - nCnt; i++ )
				strValue = "0" + strValue;
		}
			
		//	�Ǽ��κ� ©�󳻱�	(�� : 102'31.5 => 5 )
		strFloat = strValue.Right(indec);
		//	�߰��κ� ©�󳻱�	(�� : 102'31.5 => 31 )
		strMid = strValue.Mid(strValue.GetLength() - outdec, outdec - indec);
		//	�����κ� ©�󳻱�	(�� : 102'31.5 => 102 )
		strInt = strValue.Left(strValue.GetLength() - outdec);

		//	�����κ��� ������� 0���� ��ü�Ѵ�.
		if ( strInt.IsEmpty() )
			strInt = "0";

		//	�Ǽ��κ��� ������� . ���ϴ� ǥ�� ���Ѵ�.
		if ( strFloat.IsEmpty() )
			strValue.Format("%s'%s", strInt, strMid);
		else
			strValue.Format("%s'%s.%s", strInt, strMid, strFloat);

//		return strValue;
	}
	else	//10
	{
		strValue.Format("%d", nTBValue);

		if ( strValue.GetLength() < indec/*outdec*/ )
		{
			int nCnt = strValue.GetLength();
			for ( int i = 0 ; i < indec/*outdec*/ - nCnt; i++ )
				strValue = "0" + strValue;
		}
			
		//	�Ǽ��κ� ©�󳻱�	(�� : 102'31.5 => 5 )
		strFloat = strValue.Right(indec);
		//	�����κ� ©�󳻱�	(�� : 102'31.5 => 102 )
		strInt = strValue.Left(strValue.GetLength() - indec/*outdec*/);

		//	�����κ��� ������� 0���� ��ü�Ѵ�.
		if ( strInt.IsEmpty() )
			strInt = "0";
		//	�Ǽ��κ��� ������� . ���ϴ� ǥ�� ���Ѵ�.
		if ( strFloat.IsEmpty() )
			strValue.Format("%s", strInt);
		else
			strValue.Format("%s.%s", strInt, strFloat/*strMid*/);

//		return strValue;
	}

//	strValue.Format("%d", nTBValue);
	if ( bFlag )
		strResult.Format("-%s", strValue);
	else
		strResult.Format("%s", strValue);

	return strResult;
}

long CStdDialog::FtoTbondPrcL( double inprice, int scale, int outdec )
{
	int		i_intnum;
	char	s_float[10], s_temp[20];
	
	if( scale == 8 )
	{
		i_intnum = (int)floor( inprice );
		sprintf( s_float, "%3.1f", (inprice-i_intnum)*scale );
	}
	else if ( scale == 32 )
	{
		i_intnum = (int)floor( inprice );
		if ( outdec == 4 )
			sprintf( s_float, "%06.1f", (inprice-i_intnum)*scale*100.0 );
// --> [Edit]  ������ 2009/09/03
// �̱�ä 30�⹰ ƽ������ ���濡 ���� �߰�
		else if ( outdec == 2 )
			sprintf( s_float, "%04.1f", (inprice-i_intnum)*scale );
// <-- [Edit]  ������ 2009/09/03
		else
			sprintf( s_float, "%05.1f", (inprice-i_intnum)*scale*10.0 );
	}
	else
	{
		i_intnum = (int)floor( inprice );
		sprintf( s_float, "%05.1f", (inprice-i_intnum)*scale*10.0 );
	}

	
	sprintf( s_temp, "%d%.*s", i_intnum, outdec, s_float );
	return atol( s_temp );
}


void CStdDialog::LastInfoSave()
{
#ifdef	_LASTINFO_SAVE
//	// ������ ���������ϰ�� �ٸ� ȭ���ȣ�� �����ϱ����ؼ� �߰�
//	CString strScreenNo = m_strScreenNo;
//
//	long	nHwnd(0);
//	CString strTempName = "";
//	CString strOrgName = "";
//	CString strTemp = m_strConfig;
//	CString strUserPath = "";
//
//	int pos = m_strConfig.ReverseFind('\\'); 
//	if(pos > -1)
//		strUserPath = m_strConfig.Left(pos); 
//	else
//		strUserPath = m_strUserDir;
//
//	strTemp.Replace(strScreenNo, "Che_" + strScreenNo);
//	strTemp.Replace(".dat", "");
//	strOrgName = strTemp;
//	nHwnd = (long)GetSafeHwnd();
//
////	int pos = strExePath.ReverseFind('\\'); 
////	if(pos > -1)
////	{
////		strExePath = strExePath.Left(pos); 
////	}
//
//	strTempName.Format("%s%s.tmp", strTemp, m_strHwndKey);
//	strOrgName.Format("%s%s.mct", strTemp, m_strSaveKey);
//
//	if ( !m_strSaveKey.IsEmpty() )
//	{
//		CFileFind ff;
//		BOOL bResult = ff.FindFile(strTempName);
//		if(bResult)
//		{
//			::CopyFile(strTempName, strOrgName, TRUE);
//		}
//	}
//
//	::DeleteFile(strTempName);
	
#endif
}

void CStdDialog::FileCopyAndSaveAndDelete(int nType)
{
#ifdef	_LASTINFO_SAVE
	CString strUserPath = "";

	if ( m_strConfig.IsEmpty() )
		return; 

	int pos = m_strConfig.ReverseFind('\\'); 
	if(pos > -1)
		strUserPath = m_strConfig.Left(pos); 
	else
		return;

	if ( nType == DELETE_FILE )		//	����ߴ� ���� �����
	{
// [Edit]  ������ 2009/08/26
//		CString sAllFileName = strUserPath + "\\" + "*" + m_strHwndKey + "*";
		CString sAllFileName = strUserPath + "\\" + "*" + m_strHwndKey + "*" + m_strScreenNo + "*";
		CFileFind ff;
		CString sFileName;
		CString sNewFileName;
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
	}
	else if ( nType == HWND_DEFAULT_COPY )	//	�ڵ�Ű ���� => Default ���� ����
	{
// [Edit]  ������ 2009/08/26
//		CString sAllFileName = strUserPath + "\\" + "*" + m_strHwndKey + "*";
		CString sAllFileName = strUserPath + "\\" + "*" + m_strHwndKey + "*" + m_strScreenNo + "*";
		CFileFind ff;
		CString sFileName;
		CString sNewFileName;
		BOOL bFindNextFile = FALSE;
		if(ff.FindFile(sAllFileName,0))
		{
			while(1)
			{
				bFindNextFile = ff.FindNextFile();
				sNewFileName = sFileName = ff.GetFilePath();

				sNewFileName.Replace(m_strHwndKey, "DEFAULT");
				::CopyFile(sFileName, sNewFileName, FALSE);
				if(!bFindNextFile)	break;
			}
		}
	}
	else if ( nType == DEFAULT_HWND_COPY )	//	Default ���� ���� => �ڵ�Ű ����
	{
// [Edit]  ������ 2009/08/26
//		CString sAllFileName = strUserPath + "\\" + "*" + "DEFAULT" + "*";
		CString sAllFileName = strUserPath + "\\" + "*" + "DEFAULT" + "*" + m_strScreenNo + "*";
		CFileFind ff;
		CString sFileName;
		CString sNewFileName;
		BOOL bFindNextFile = FALSE;
		if(ff.FindFile(sAllFileName,0))
		{
			while(1)
			{
				bFindNextFile = ff.FindNextFile();
				sNewFileName = sFileName = ff.GetFilePath();

				sNewFileName.Replace("DEFAULT", m_strHwndKey);
				::CopyFile(sFileName, sNewFileName, FALSE);
				if(!bFindNextFile)	break;
			}
		}
	}
	else if ( nType == LAST_HWND_COPY )	//	������ ����ɶ� ȭ�� Ű���� ��������ش�. 
	{
// [Edit]  ������ 2009/08/26
//		CString sAllFileName = strUserPath + "\\" + "*" + m_strHwndKey + "*";
		CString sAllFileName = strUserPath + "\\" + "*" + m_strHwndKey + "*" + m_strScreenNo + "*";
		CFileFind ff;
		CString sFileName;
		CString sNewFileName;
		BOOL bFindNextFile = FALSE;
		if(ff.FindFile(sAllFileName,0))
		{
			while(1)
			{
				bFindNextFile = ff.FindNextFile();
				sNewFileName = sFileName = ff.GetFilePath();

				sNewFileName.Replace(m_strHwndKey, m_strSaveKey);
				::CopyFile(sFileName, sNewFileName, FALSE);
				if(!bFindNextFile)	break;
			}
		}
	}
	else if ( nType == HWND_LAST_COPY )	//	����ȭ���� �ڵ鰪�� �߰��� ���Ϸ� ī���Ѵ�.
	{
// [Edit]  ������ 2009/08/26
//		CString sAllFileName = strUserPath + "\\" + "*" + m_strScreenKey + "*";
		CString sAllFileName = strUserPath + "\\" + "*" + m_strScreenKey + "*" + m_strScreenNo + "*";
		CFileFind ff;
		CString sFileName;
		CString sNewFileName;
		BOOL bFindNextFile = FALSE;
		if(ff.FindFile(sAllFileName,0))
		{
			while(1)
			{
				bFindNextFile = ff.FindNextFile();
				sNewFileName = sFileName = ff.GetFilePath();

				sNewFileName.Replace(m_strScreenKey, m_strHwndKey);
				::CopyFile(sFileName, sNewFileName, FALSE);
				::DeleteFile(sFileName);
				if(!bFindNextFile)	break;
			}
		}
	}

#endif
}

LRESULT CStdDialog::OnUserInfo(WPARAM wp, LPARAM lp)
{
	BYTE bt = LOBYTE(LOWORD(wp));
	switch (bt)
	{
	case USERINFO_SET:
		{
			WORD dwKey = HIWORD(wp);
			if (dwKey==0)
			{
				IPartnerManager* pPtManager = (IPartnerManager*)lp;
				DoSetPartnerSite(pPtManager);
			}
		}
		break;
	}
	
	return 0L;
}

void CStdDialog::_DoFirstRequest()
{
	int nStep;
	CString rCode;

	GetFirstCode(rCode, CMasterTypeInfo::FO_FOREIGN_CHART, nStep);

	if(rCode.GetLength()>0)
		SendMessage(UMSG_SENDSHAREDCODE, (WPARAM)(LPCSTR)rCode, (LPARAM)m_nMarketType);

	return;
}

BOOL CStdDialog::GetFirstCode(CString &rString, int nMarketType, int &nSetp)
{
	BOOL bRet = TRUE;
	rString.Empty();
	nSetp=0;

	char szTemp[32];
	memset(szTemp, 0x00, sizeof(szTemp));
	::GetPrivateProfileString("0000", "Code00", "", szTemp, sizeof(szTemp), m_strConfig);

	rString.Format(_T("%s"), szTemp);

	int nCodeType;
	if(rString.IsEmpty()==FALSE)
	{
		nMarketType = GetMarketTypeFromCode(rString, nCodeType);
		if (nMarketType != CMasterTypeInfo::FO_FOREIGN_CHART)
			rString = "";
	}

	CStringArray arrCode, arrName;
	CString rCode;

//	if (nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
/*
		//Step 1. ��ȸ�ϵ��� ���õ� ����ã��
		//Step 2. �ֱ���ȸ����.
		pMasterDataManager->GetRecentCodeNameArray(nMarketType, arrCode, arrName);
		if (arrCode.GetSize()>0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 2;
			rString = rCode;
		}
		else		//Step 3. ����Ʈ �ڵ�� ��ȸ.
		{
			CStringArray arCode, arName, arEngName;
			CStringArray *parCode = &arCode, *parName = &arName, *parEngName = &arEngName;
			pMasterDataManager->GetFutureJongMst(parCode, parName, parEngName);
			if(arCode.GetSize()>0)
			{
				rCode = arCode.GetAt(0);
				nSetp = 3;
				rString = rCode;
			}
			else
			{
				rString = "";
				bRet = FALSE;
			}
		}
*/
		IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
		pMasterDataManager->GetRecentCodeNameArray(nMarketType, arrCode, arrName);
		if(arrCode.GetSize()>0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 2;
			rString = rCode;
		};
	}

	return bRet;
}

CString CStdDialog::GetCodeType(CString strCode)
{
	CString strType = "";
	m_mapCodeType.Lookup(strCode, strType);

	return strType;
}



// ��Ʈ �ֹ� ���� - onlyojt (20091204)
void CStdDialog::SetOrderLineInfo(BOOL bSet)
{
	if(bSet)
	{
		// ����
		CString strCodeInputCode = m_pInputBarDlg->m_pCodeInput->GetCode();
		if (strCodeInputCode.IsEmpty ())
			return;

		// ���¹�ȣ
		CString strAccount = "", strAccName, strAccPW = "";
		m_pExtInputBarDlg->GetAccountInfo(strAccount, strAccName, strAccPW);
		if (strAccount.IsEmpty ())
			return;


		m_ChartMng.GetpChartItemSelected()->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 5, 0);

		//�ŵ�/�ż� ����	'0' : �ŵ�, '1' : �ż�
		CString strOrderType = ((CInputBarDlg*)m_pInputBarDlg)->GetOrderType();

		FX_CHE_RQ_INPUT Input;
		memset(&Input, ' ', sizeof(Input));
		memcpy(Input.szAccNo, (LPCTSTR)strAccount, strAccount.GetLength());
		memcpy(Input.szAccNoPW, (LPCTSTR)strAccPW, strAccPW.GetLength());
		memcpy(Input.szItemCode, (LPCTSTR)strCodeInputCode, strCodeInputCode.GetLength());
		if( strOrderType.GetLength() > 0 )
			Input.szMMGB[0] = strOrderType.GetAt(0);

		stSend2Server st;
		st.m_nType = 0;
		st.m_pWnd = m_ChartMng.GetpChartItemSelected();
		st.m_pBuffer = (void*)(LPCTSTR)&Input;
		st.m_lLength= sizeof(Input);
		//// ��ü����ȸ
		//Input.szChGB[0] = '2';
		//st.m_strTRCode = QUERY_STRFX_CHE;	
		//SendMessage(UMSG_SEND2SERVER,0,(long)&st);

		//<<�ؿܼ����� �ܰ���ȸ ���ϰ� ����ġ�� ü�᳻������ ��ȸ�ϱ�� ��
		//// �ܰ���ȸ
		//st.m_strTRCode = QUERY_STRFOFOREIGN_JAN;	
		//SendMessage(UMSG_SEND2SERVER,0,(long)&st);
		//>>

		// ü����ȸ
		Input.szChGB[0] = '1';
		st.m_strTRCode = QUERY_STRFOFOREIGN_CHE2;	
		SendMessage(UMSG_SEND2SERVER,0,(long)&st);
	}
	else
	{
		CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();;
		if(pChartItem)
		{
			pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 5, 0);
			//pChartItem->InvokeAddIn("ChartKoreaSorimachiAddIn.dll:USER_COMMENT", 5, 0);
		}
	}
}


//<<< 22091215 by kmlee for �ֹ�����
LRESULT CStdDialog::OnChangeAccount(WPARAM wParam, LPARAM lParam)
{
	UpdateCheJanOption ();
	return 0;
}

void CStdDialog::UpdateCheJanOption()
{
	if (m_pExtInputBarDlg->GetChkOrder ())
	{
		// ���� ����Ʈ ����..
		SetOrderLineInfo (FALSE);
		// FX��Ʈ �ֹ� ���� - onlyojt (20091204)
		SetOrderLineInfo (TRUE);
	}
}
//>>>
LRESULT CStdDialog::OnAccountInit(WPARAM wp, LPARAM lp)
{
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->m_hWnd)
		m_pExtInputBarDlg->InitAccountCtrl(wp, lp);

	return 0L;
}
