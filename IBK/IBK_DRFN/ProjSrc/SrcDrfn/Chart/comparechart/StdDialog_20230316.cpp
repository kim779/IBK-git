// StdDialog.cpp : implementation file
//
#include "./define/ChartMsgDef.h"
#include "ChartItem.h"

#include "stdafx.h"
#include "CompareChart.h"
#include "IO.h"
#include "direct.h"
#include "StdDialog.h"
//#include "TabCodeSearch.h"
//#include "ChartListDlg.h"
//#include "FloatingHogaDlg.h"

#include "../COMMONTR/ChartRealConverter.h"
#include "DataWindowDlg.h"
#include "CodeDataFromInputBar.h"
#include "./define/ReserveShareName.h"
#include "./define/MessageDefine.h"
#include "./define/DragNDropDefine.h"
//#include "../../../COMMON_BASE/LocalUtil.h"
//#include "../../COMMON_BASE/drds/DRDSDEF.h"
#include "./include/UChar.h"

#include "../chart_common/StatusMsgDef.h"
#include "../../inc/IMainInfoMng.h"
#include "../../inc/IAUPlatformDispMng.h"
#include "../../inc/ExCommonHeader.h"
#include "../../inc/RealUpdate.h"
#include "../../inc/IAUDataMonitorMng.h"	// UUID_IAUDataMonitorManager : �ݹ��Լ� �޴µ� ���
#include "../../inc/IKSLibMng.h"

#include "../../inc/RMsgUserInfo.h"

//#include "../../../../../include/CommDefine.h"

#include "../CommonTR/KXS3.h"	// �ֽ�
#include "../CommonTR/IXIJ.h"	// ����
#include "../CommonTR/FFC0.h"	// ����
#include "../CommonTR/OOC0.h"	// �ɼ�
#include "../CommonTR/JFC0.h"	// �ֽļ���
#include "../CommonTR/JOC0.h"	// �ֽĿɼ�

//#ifdef __MULTI_LANG__
	#include "../Include_Chart/Dll_Load/IMetaTable.h"	//@Solomon-MultiLang:091117
//#endif

//#include "../../../../../CommonSrc/CtlColor.h"

const UINT RMSG_GET_SCREENTYPE = ::RegisterWindowMessage(_T("RMSG_GET_SCREENTYPE"));
const UINT RMSG_GET_TEMPDATA = ::RegisterWindowMessage(_T("RMSG_GET_TEMPDATA"));
const UINT RMSG_GET_LINKINFODATA = ::RegisterWindowMessage(_T("RMSG_GET_LINKINFODATA"));
const UINT RMSG_GET_ITEMNAMEFORMITEMCODE = ::RegisterWindowMessage(_T("RMSG_GET_ITEMNAMEFORMITEMCODE"));
const UINT RMSG_GET_MARKETTYPEFORMITEMCODE = ::RegisterWindowMessage(_T("RMSG_GET_MARKETTYPEFORMITEMCODE"));
const UINT RMSG_GETINTERFACE = ::RegisterWindowMessage(_T("RMSG_GETINTERFACE"));

#define	WMU_GET_KEY_INFO_ARRAY		WM_USER + 2253	// �ش� AUP���� �ԷµǾ� �ִ� Key���� ����� �����´�. LPARAM�� CStringArray *

#define	WIDTH_SIDEBAR		8
//#include "../../inc/RegisterWindowMessage.h"

//const UINT RMSG_SETFOCUSTOCODE = ::RegisterWindowMessage(_T("RMSG_SETFOCUSTOCODE"));
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// hmc skin table id
#define COLORTBL_BKGRND			2			// ��������
#define COLORTBL_TAB_SEL_TEXT	12			// �� Ȱ�� �ؽ�Ʈ
#define COLORTBL_TAB_NOR_TEXT	14			// �� ��Ȱ�� �ؽ�Ʈ
#define COLORTBL_TAB_BKGRND		10			// �� �ٵ� ���
#define COLORTBL_TAB_SEL_GDSTT	11			// �� Ȱ�� ��� �׶��̼� ����
#define COLORTBL_TAB_SEL_GDEND	261			// �� Ȱ�� ��� �׶��̼� ��
#define COLORTBL_TAB_NOR_GDSTT	13			// �� ��Ȱ�� ��� �׶��̼� ����
#define COLORTBL_TAB_NOR_GDEND	67			// �� ��Ȱ�� ��� �׶��̼� ��

/////////////////////////////////////////////////////////////////////////////
// CStdDialog dialog
#define ID_LEFTBAR			IDD_LEFTBAR
#define ID_INPUTBAR			IDD_INPUTBAR
#define ID_MINIBAR			IDD_MINIBAR
#define ID_EXTINPUTBAR		IDD_EXTINPUTBAR
#define ID_SPLITTER			7000
#define ID_CHART			8000
#define ID_TOOLBAR			IDD_DLG_TOOLBAR

// shared Info Name.. gm0604 2005.8.24
#define COMPARECHARTOPEN	"CompareChartOpen"
#define COMPARECHARTCLOSE	"CompareChartClose"
#define FROMSTCENTER	"FromSTCenter"

#define DEFAULT_CHARTDATA_COUNT_INT		400
#define DEFAULT_CHARTDATA_COUNT_CHAR	"400"

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

/////////////////////////////////////////////////////////////////////////////
// CCompareChartWnd
#define HEIGHTOFINPUTBAR	(26)
//#define HEIGHTOFINPUTBAR	(20)
//#define WIDTHOFLEFTBAR		(200)
//#define WIDTHOFSPLITTER		(4)
#define WIDTHOFSPLITTER		(5)
#define WIDTH_TOOLBAR		(36)

const UINT RMSG_GETMAINWND	= ::RegisterWindowMessage(_T("RMSG_GETMAINWND"));

#define Memset(x) memset(x, 0x00, sizeof(x))

CString SetComma(CString strNumber,BOOL bShowSign=FALSE)
{
	CString szRet=_T("");
	CString szTmp = strNumber;
	szTmp.TrimLeft();
	szTmp.TrimRight();

	int i = 0;
	char	ch = NULL;
	int nLen = strlen(szTmp);
	for( i=0;  i<nLen ; i++) {
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
// Static CimageList ============================================================>
	int		   CStdDialog::m_static_nChartCnt4ImgList = 0;

	CImageList CStdDialog::m_ImgList_W15H15; //�� 15�� ���� 15�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W20H20; //�� 20�� ���� 19�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W31H20; //�� 31�� ���� 20�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W20H20_Tool; //�� 20�� ���� 19�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W46H20; //�� 46�� ���� 19�� ��ư �̹�������Ʈ	
	CImageList CStdDialog::m_ImgList_W50H20; //�� 50�� ���� 19�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W151H21; //�� 151�� ���� 21�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W28H22; //�� 28�� ���� 22�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W17H20; //�� 17�� ���� 20�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W43H20; //�� 43�� ���� 20�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W58H20; //�� 58�� ���� 20�� ��ư �̹�������Ʈ
//	CImageList CStdDialog::m_ImgList_W72H20; //�� 72�� ���� 20�� ��ư �̹�������Ʈ
//	CImageList CStdDialog::m_ImgList_W46H20_Gwan; //�� 46�� ���� 20�� ��ư �̹�������Ʈ
//	CImageList CStdDialog::m_ImgList_W8H25;  //�� 8�� ���� 25�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W60H20; //�� 60�� ���� 20�� ��ư �̹�������Ʈ
// <==============================================================================	

extern CCompareChartApp theApp;

CStdDialog::CStdDialog(CWnd* pParent /*=NULL*/)
	: CExOCXDLL(CStdDialog::IDD, pParent),
	m_nDrdsInfoCnt(0),
	m_pPTManager(NULL)
{
	EnableAutomation();
	//{{AFX_DATA_INIT(CStdDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLeftBarDlg = NULL;
	m_pExtInputBarDlg = NULL;
	m_pInputBarDlg = NULL;
//Han Hwa	m_pDrds = NULL;
	m_nLeftBarType = 0;
	m_pData = NULL;
	m_bSmallScreenMode = FALSE;
	m_nTempLeftBarType = 0;
	m_bEnable = TRUE;
	m_bFirstGShared = TRUE;
	m_bShowExtInputBarDlg = FALSE;
	m_bShowInputBarDlg = TRUE;
	m_strMapID = "33100";
	m_nWidthOfLeftBar = 0;
	m_nSpliteID = 0;
	m_nTabID = 0;
	m_nChartType = SINGLE_CHART;
	m_nImportedMapID = 0;
	m_nWidthOfDlg = 0;
	m_nHeightOfDlg = 0;
	m_hViewInMain = NULL;
	m_hKSDllView = NULL;
	m_lWidthOriginal = 0;
	m_lHeightOriginal = 0;
	m_bHideChart = FALSE;
	m_bInitResize = FALSE;

	m_hSocket		= NULL;
	//m_pDBMgr		= NULL;
	m_hMainFrame	= NULL;
//	m_lDBMgr		= NULL;

	m_bShowDataListWnd = FALSE; //2005. 04. 25 - ������ �����츦 ������ ����

	m_nMarketType   = CMasterTypeInfo::STOCK_CHART;

	

	m_pRqWndFromTab = NULL;

	memset(&m_ChartCfgInfo, 0x00, sizeof(CHART_CONFIG_INFO));
	
	m_strConfig = "";
	m_strRootPath = "";

	m_TRComm = NULL;

	m_bSystemChart = FALSE;
	m_bSimpleChart = FALSE;
	m_bMultiItemChart = FALSE;
	m_hCursor = NULL;
	m_cUM = 'E';
	m_strDollar = "";
	m_bOneClickHide = FALSE;

//	m_nMarketTrCode = atoi(QUERY_STRMARKET);

	//20061212 ������ ����
	//grid column Ȯ�� btn �߰�
	m_nExtendState = 0;
	//20061212 ���� end
	//20070131 by ������
	//{������ǥ ������ȸ ���� ����
	m_cMUM = 'P';
	//} 

	//20070226 by LYH
	//{���� ������ ó��
	m_bPreMarket = FALSE;
	//}}
	//{{ƽ������ ���� ��� Ŭ���� ���� ó��
	m_cUT = '1';
	//}}

	m_strRecvCode = "";
	//2007.05.02 by LYH ���� ��� ���� ���� ID �߰�
//	m_strSvcID = "CP986100";

//	m_nScreenType = Screen_Compchart;

	m_pDrdsLib = NULL;
	m_pShareInputData = new CShareInputData;
	m_bActiveDataWindow = false;
	m_pDataWindow = NULL;

	m_bSizeTestMode = FALSE;
	g_pDebugMng = NULL;
	g_pPtUtilMng = NULL;

	::memset(&m_stTrInfo2, 0x00, sizeof(m_stTrInfo2));

	m_pGwanMng = NULL;
	m_bOnDestroy = FALSE;
}


void CStdDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStdDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_BTN_EXT_TOOL, m_btnExtTool);
//	DDX_Control(pDX, IDC_SIDEBAR, m_btnSideBar);
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
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_SPLITTER_MOVED, OnWmSplitterMoved )	
//	ON_MESSAGE( WM_GET_BROD, OnGetBroad )
	//ON_REGISTERED_MESSAGE(RMSG_SETFOCUSTOCODE	, OnSetFocusToCode)
	ON_REGISTERED_MESSAGE(RMS_ANALYSIS_FUNC		, OnByPass_ExportToolOption ) //2005. 07. 28 added by sy,nam	
	//-----------------------------------------------------------------------------
	// Author		: {{{ ������ }}} ojtaso 	Date : 2005/2/23
	// Reason		: Added to drag and drop operation.
	//-----------------------------------------------------------------------------
	// BEGIN
	ON_REGISTERED_MESSAGE(RMSG_DRAGSTART, OnDragStart)
	ON_REGISTERED_MESSAGE(RMSG_DRAGEND, OnDragEnd)
	ON_REGISTERED_MESSAGE(RMSG_CHANGE_SKINCOLOR, OnChangeSkinColor)
	ON_REGISTERED_MESSAGE(RMSG_GET_SKINCOLOR, OnGetSkinColor)
	ON_REGISTERED_MESSAGE(RMSG_USERINFO, OnUserInfo)
	// END
//	ON_MESSAGE(WMU_DND_DATA, OnDropData)
END_MESSAGE_MAP()
//@��������	ON_MESSAGE( WM_DRDS_DATA, OnDrdsData )
//@Solomon090819 	afx_msg long OnGetBroad(WPARAM wParam, LPARAM lParam);

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
	if(strSharedName=="�ð�-�׸���")
	{
		m_ChartMng.SetTimeMark(lpGSharedData, TRUE);
		return;
	}
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

#define	titleCC			0x01		// SetWindowText(text)

BOOL CStdDialog::OnInitDialog() 
{
	CExOCXDLL::OnInitDialog();

	AfxEnableControlContainer();	//solomon-090825

	// User ID �� ������� �ʱ�ȭ
	SetInitInfo();

	// �ֿ켱���� ImageList �ε�
	InitImageList();

	if(m_nHeightOfDlg && m_nWidthOfDlg)
	{
		SetWindowPos(&CWnd::wndTop, 0, 0, m_nWidthOfDlg, m_nHeightOfDlg, SWP_SHOWWINDOW);			
	}

	// PCTR �� �⺻ Interface ����
	SetBaseInterface();
	g_pDebugMng = (IDebugManager*)AfxGetPctrInterface(UUID_IDebugManager);
	g_pDebugMng = (IDebugManager*)AfxGetPctrInterface(UUID_IDebugManager);
	{
		IPartnerSvrManager* pPtSvrMng = (IPartnerSvrManager*)AfxGetPctrInterface(UUID_IPartnerSvrManager);
		if(pPtSvrMng)
		{
			g_pPtUtilMng = (IPartnerUtilManager*)pPtSvrMng->GetInerInterface(LUID_IPartnerUtilManager);
		}
	}

	if(m_nScreenType==CStdDialog::Screen_Compchart)
		g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "���պ���Ʈ");
	else if(m_nScreenType==CStdDialog::Screen_ForeignChart)
		g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "����Ʈ");
	else if(m_nScreenType==CStdDialog::Screen_fluctuation)
		g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "����Ʈ");

// 	m_pGwanMng = (IWinixGwanManager*)GetParent()->SendMessage(RMSG_GETINTERFACE, 0, 0);
// 	if(m_pGwanMng)
// 	{
// 		CString szKey;
// 		szKey.Format("%08X\t%08X\t", m_hPlatform, GetParent()->GetSafeHwnd());
// 		m_pGwanMng->Advise(szKey);
// 	}

	m_nChartType = MULTI_CHART;
//	CString strScreenInfoPath = m_strRootPath +  "\\data\\ChartScreenInfo.dat";
	CString strInfo, strInfo2;
//	::GetPrivateProfileString(m_strMapFileName, "��Ʈ����", "����", strInfo.GetBuffer(MAX_PATH), MAX_PATH, strScreenInfoPath);
//	::GetPrivateProfileString(m_strMapFileName, "���屸��", "�ֽ�", strInfo2.GetBuffer(MAX_PATH), MAX_PATH, strScreenInfoPath);
	
	strInfo = "����";	// ����, �ֽ�����, ��������, �����ɼ�����, �̴�����1, �̴�����2, �̴��ֽ�, �̴Ͼ���, �̴ϼ���
	strInfo2 = "�ֽ�";	// �ֽ�, ����, �����ɼ�

	if(strInfo == "����")
	{
		m_nMarketType =  CMasterTypeInfo::STOCK_CHART;
		m_bSmallScreenMode = FALSE;
		m_nChartType = MULTI_CHART;
		m_bMultiItemChart = TRUE;
	}

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	CreateLogFile();
#endif

	m_ChartMng.m_pShareInputData = m_pShareInputData;
	FileDelete();
	
	// �������Ʈ�̸�...
	if(m_nScreenType==Screen_fluctuation)
	{
		m_FnHandle.m_szDataFile = m_strUserDir + "\\" + m_strScreenNo + "_" + ENVINFOFLUCTFILE;
		_DoCheckDaultDataFile(m_strScreenNo, m_FnHandle.m_szDataFile);

		if(atoi(m_strScreenNo)==nScreenID_0424) _DoCheckRecentFuture(m_FnHandle.m_szDataFile);

		m_stDlgCompData.m_pFn = &m_FnHandle;
		m_FnHandle.DoCheckFirst();
		m_FnHandle.LoadData((long)&m_stDlgCompData);		
		m_pShareInputData->m_nBase = m_stDlgCompData.m_nBase;
		m_pShareInputData->m_DataInputBar.m_nUnitType = PERCENT_TYPE;

		switch(m_pShareInputData->m_nBase)
		{
			case 0:
			default:
				m_pShareInputData->m_DataInputBar.m_cChartShape = NUJUK_OVERLAPP_COMPARE_CHARTA;
				m_pShareInputData->m_DataInputBar.m_cDataGubun = '1';
				break;
			case 1:
				m_pShareInputData->m_DataInputBar.m_cChartShape = NUJUK_OVERLAPP_COMPARE_CHARTB;
				m_pShareInputData->m_DataInputBar.m_cDataGubun = '3';
				break;
			case 2:
				m_pShareInputData->m_DataInputBar.m_cChartShape = NUJUK_OVERLAPP_COMPARE_CHARTC;
				m_pShareInputData->m_DataInputBar.m_cDataGubun = '4';
				break;
		}
		// ����ÿ� Clear����� �Ѵ�.

		m_bSizeTestMode = AfxGetApp()->GetProfileInt("STBasic", "bSizeTestMode", 0);
	}

	FormLoad();
	//ChangeSkinColor();
	OnChangeSkinColor(0, 0);

	//����Ʈ �� �������Ʈ���� ��ƮOCX �޴� ����
	//if(m_nScreenType == Screen_fluctuation)
	{
		if(m_ChartMng.m_hWnd) m_ChartMng.GetpChartItemSelected()->SetBAddGraphMenuShow(FALSE);
		//SetTimer(2, 1, NULL);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStdDialog::ChangeStockMode(int type)
{
	if(m_nChartType == MINI_MULTI_CHART || m_nChartType == MINI_SINGLE_CHART)
	{
		ChangeMiniStockMode(type);
		return;
	}
	CString strTemp;

	m_nMarketType =  type;

	if(m_pInputBarDlg)
	{
		m_pInputBarDlg->SetMarketType(m_nMarketType);	
		m_pInputBarDlg->ChangeMarket(m_nMarketType, FALSE, TRUE);

		m_pInputBarDlg->MoveWindow(GetRect(ID_INPUTBAR,TRUE));
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

	m_ChartMng.m_bLastEnvUpdated = FALSE;
	//====================================================
	m_strConfig = m_strUserDir + "\\" + m_strScreenNo + "_" + ENVINFOFILE;

	if(m_pInputBarDlg)
	{
		m_pInputBarDlg->m_strCfgFile = m_strConfig;

		m_pInputBarDlg->SetMarketType(m_nMarketType);	
		m_pInputBarDlg->MoveWindow(GetRect(ID_INPUTBAR,TRUE));

//@��������
//		m_pInputBarDlg->SetMarKetToolType(m_nMarketType);//2005. 05. 16 - chart gubun ����
//@��������
//@�����߰�
	m_pInputBarDlg->ChangeMarket(m_nMarketType, FALSE, TRUE);
//@�����߰�

		//2005. 06. 22 ���� �ڵ带 ���� ���� ���� �����Ѵ�.
		BOOL bRequestAPData = FALSE;
		if(m_pInputBarDlg->GetSafeHwnd())
		{
			m_pInputBarDlg->InitBaseInfo(NULL, bRequestAPData);
		}
	}


	//------------------------------------------------------------------------------
	// 2005. 05. 16 chart �����ڸ� �ѱ��.
	//------------------------------------------------------------------------------	
	m_ChartMng.SetMarketType(m_nMarketType);
	if( m_ChartMng.GetpChartItemSelected() != NULL)
		m_ChartMng.GetpChartItemSelected()->LoadTickMinDayInfo(m_strConfig);

//@��������
//	//{{2007.07.23 by LYH
//	if(m_pInputBarDlg)
//		m_pInputBarDlg->SetRecentCode();
//	//}}
//@��������
//@�����߰�
	if(m_pInputBarDlg)
		m_pInputBarDlg->SetLastHistoryCode();
//@�����߰�
	PostMessage(UMSG_EDITCODE_INIT, 0L, 0L);
}

void CStdDialog::FormLoad()
{
	// �޼����� ����
	// TR �� DRDS �ʱ�ȭ
	m_TRComm = (ITrComm2*)GetTrComm();
	InitDrds();

	// User Directory ����
	// �������� : IO.h, Direct.h
	if (_access(m_strUserDir, 0) == -1)
	{	
		 int retval = _mkdir(m_strUserDir);
	}  

	// �������� ���
	m_strConfig = m_strUserDir + "\\" + m_strScreenNo + "_" + ENVINFOFILE;

	char szTemp[64];
	if(m_nChartType == MULTI_CHART)
	{
		m_nWidthOfLeftBar = 167;

		if(m_nWidthOfLeftBar)
		{
			::GetPrivateProfileString("Frame","ShowLeftBar","1",szTemp,sizeof(szTemp),m_strConfig);
			m_nLeftBarType = atoi(szTemp);
		}
		else
		{
			m_nWidthOfLeftBar = 10;
		}
	}
	else
	{
		m_nWidthOfLeftBar = 167;
		m_nLeftBarType = 0;
	}

		// ��Ų����
//	IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager);
//	if(pKSLibMng)
//		m_clrBackground = pKSLibMng->Lib_KSLink_GetColor(1);	// ����ȭ�� ���
//	elseelse
		m_clrBackground = CLR_INPUTBAR_BKGRND;		// �⺻��

	switch(m_nChartType)
	{
	case MULTI_CHART:
	case SINGLE_CHART:
		{
//			m_btnSideBar.SetBorderColor(RGB(157,157,161));
//			m_btnSideBar.SetBackGroundColor(RGB(254,254,156), RGB(254,254,156));
			// ��ܹ� ����
			m_pInputBarDlg = NULL;
			m_pInputBarDlg = new CInputBarDlg();
			if(m_pInputBarDlg)
			{
				m_pInputBarDlg->SetBkColor(m_clrBackground);
				m_pInputBarDlg->m_nScreenType = m_nScreenType;
				m_pInputBarDlg->m_pShareInputData = m_pShareInputData;
				m_pInputBarDlg->SetMarketType(m_nMarketType);				
				m_pInputBarDlg->SetpChartMng(&m_ChartMng);
				m_pInputBarDlg->Create(IDD_INPUTBAR,this);
				m_ChartMng.SetpInputBarDlg(m_pInputBarDlg);
				m_pInputBarDlg->MoveWindow(GetRect(ID_INPUTBAR,TRUE));
				//m_pInputBarDlg->ShowWindow(SW_SHOW);
				m_pInputBarDlg->ChangeMarket(m_nMarketType, FALSE, TRUE);
				m_pInputBarDlg->SetAllPosition(m_nChartType);
			}
			// ���̵�� ���� ����			
			// ������ ����
			m_pLeftBarDlg = NULL;
			if(m_nScreenType != Screen_fluctuation)
			{
				m_pLeftBarDlg = new CLeftBarDlg();
				if(m_pLeftBarDlg)
				{
					m_pLeftBarDlg->SetBkColor(m_clrBackground);
					m_pInputBarDlg->m_pShareInputData = m_pShareInputData;
					
					m_pLeftBarDlg->SetpChartMng(&m_ChartMng);
					m_pLeftBarDlg->Create(IDD_LEFTBAR,this);
					m_pLeftBarDlg->MoveWindow(GetRect(ID_LEFTBAR,TRUE));
					//JS.Kim_20101005   ȭ�� ������ ���� ����
					m_pLeftBarDlg->ModifyStyle(0, WS_CLIPSIBLINGS|WS_CLIPCHILDREN);
					m_ChartMng.SetpLeftBarDlg(m_pLeftBarDlg);
				}
			}
		}
	}

	if(m_pInputBarDlg)	SetPermanentHandle(m_pInputBarDlg);
	if(m_pLeftBarDlg)	SetPermanentHandle(m_pLeftBarDlg);

	// ������ܹ� ����
	m_pExtInputBarDlg = NULL;
	m_pExtInputBarDlg = new CExtInputBarDlg;
	if(m_pExtInputBarDlg)
	{
		m_pExtInputBarDlg->SetBkColor(m_clrBackground);
		m_pExtInputBarDlg->m_nMarketType = m_nMarketType;
		m_pExtInputBarDlg->m_pShareInputData = m_pShareInputData;

		m_pExtInputBarDlg->SetpChartMng(&m_ChartMng);
		if(m_pLeftBarDlg) m_pExtInputBarDlg->SetpLeftBar(m_pLeftBarDlg);
		m_pExtInputBarDlg->Create(IDD_EXTINPUTBAR,this);
		m_ChartMng.SetpExtInputBarDlg(m_pExtInputBarDlg);	
		m_pExtInputBarDlg->SetMarketType(m_nMarketType);
		m_pExtInputBarDlg->MoveWindow(GetRect(ID_EXTINPUTBAR,TRUE));
		//m_pExtInputBarDlg->ShowWindow(SW_SHOW);
//--> Global�ϰ� ����
//		if(m_pInputBarDlg->GetType() == DAY_DATA_CHART || 
//			m_pInputBarDlg->GetType() == WEEK_DATA_CHART || 
//			m_pInputBarDlg->GetType() == MONTH_DATA_CHART)
		if(m_pShareInputData->m_DataInputBar.m_chType == DAY_DATA_CHART || 
			m_pShareInputData->m_DataInputBar.m_chType == WEEK_DATA_CHART || 
			m_pShareInputData->m_DataInputBar.m_chType == MONTH_DATA_CHART)
//<--
		EnableQueryOpt(FALSE);
	}

	// ���ø��� ����
	m_xSplitter1.CreateInWnd(this, ID_SPLITTER, GetRect(ID_SPLITTER));

	// ��Ʈ�Ŵ��� ����
	m_ChartMng.SetMarketType(m_nMarketType);
	BOOL bRetValue = m_ChartMng.Create(GetRect(ID_CHART),this, m_strUserDir, m_szImageDir,ID_CHART, (long)((CMChartInterface*)this));
	if(!bRetValue) return;
	m_ChartMng.m_strRootPath = m_strRootPath;
	m_ChartMng.SetBaseDesk((long)1);            // ����ũ�� ���������� ���� ������ ���������� �⺻���� ����
	//JS.Kim_20101005   ȭ�� ������ ���� ����
	m_ChartMng.ModifyStyle(0, WS_CLIPSIBLINGS|WS_CLIPCHILDREN);

	if((m_nChartType != MINI_MULTI_CHART && m_nChartType != MINI_SINGLE_CHART) && m_pLeftBarDlg)
		m_pLeftBarDlg->SetConfig();

	if( m_ChartMng.GetpChartItemSelected() != NULL)
		m_ChartMng.GetpChartItemSelected()->LoadTickMinDayInfo(m_strConfig);
#ifdef _DEBUG
	else
	{
		AfxMessageBox("FormLoad : m_ChartMng.GetpChartItemSelected() is NULL !!");
		return;
	}
#endif 

	// (����)��ܹ� Ÿ�� �� Visible ����
	SetLeftBarType(m_nLeftBarType);
	SetShowExtInputBarDlg(m_bShowExtInputBarDlg);

	BOOL bRequestAPData = TRUE;

	// �����ڵ� ��Ʈ�� ����
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
	{
		m_pInputBarDlg->InitBaseInfo(NULL, bRequestAPData);
	}

//@080526-002	if(m_pInputBarDlg)	m_pInputBarDlg->SetLastHistoryCode();
	if(m_nScreenType == Screen_fluctuation)
		PostMessage(UMSG_EDITCODE_INIT, 0L, 0L);
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

	if(m_pLeftBarDlg) m_pLeftBarDlg->Invalidate();
	m_ChartMng.Invalidate();
	m_xSplitter1.Invalidate();
	m_pInputBarDlg->Invalidate();
	m_pExtInputBarDlg->Invalidate();

    return 1L;
}

CRect CStdDialog::GetRect(UINT nID, BOOL bInit /*=FALSE*/)
{
	CRect rect;
	GetClientRect(rect);
	rect.DeflateRect(BOUND_GAP, BOUND_GAP, BOUND_GAP, BOUND_GAP);

	if(!m_nLeftBarType)		rect.left = WIDTH_SIDEBAR;
	if(!m_nLeftBarType)		rect.left = 1;

	switch(nID)
	{
//	case IDC_SIDEBAR:
//		{
//			if(!m_nLeftBarType)
//			{
//				rect.left = 0;
//				rect.right = WIDTH_SIDEBAR;
//			}
//			else
//			{
//				rect.SetRectEmpty();
//			}
//			return rect;
//		}
//		break;
	case ID_LEFTBAR:
		if(!m_nLeftBarType)	
		{
			rect.SetRectEmpty();		
			return rect;
		}
		if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd()&&!bInit)
		{
			CRect rectLeftBar;
			m_pLeftBarDlg->GetWindowRect(rectLeftBar);
			ScreenToClient(rectLeftBar);
			if(m_nLeftBarType==1)		rect.right = rectLeftBar.right;
		}
		else	// initial
		{
			if(m_nLeftBarType==1)		rect.right = rect.left + m_nWidthOfLeftBar;
		}
		break;
	case ID_INPUTBAR:		
		if(!m_bShowInputBarDlg)	
		{
//			rect.left = WIDTH_SIDEBAR;
			rect.bottom = rect.top;
			return rect;
		}

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
				}
			}
			else
			{			
				if(m_nLeftBarType==1)		rect.left = rect.left + m_nWidthOfLeftBar + WIDTHOFSPLITTER;
			}	
			if(m_bShowExtInputBarDlg)
				rect.bottom = rect.top + HEIGHTOFINPUTBAR + 1;
			else
				rect.bottom = rect.top + HEIGHTOFINPUTBAR;
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
			}
		}
		else
		{			
			if(m_nLeftBarType==1)		rect.left = rect.left + m_nWidthOfLeftBar + WIDTHOFSPLITTER;
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
		}
		else
		{
			if(m_nLeftBarType==1)
			{
				rect.left = rect.left + m_nWidthOfLeftBar;
				rect.right = rect.left + WIDTHOFSPLITTER;
			}
		}
		break;
	case ID_CHART:
		if(m_nScreenType==Screen_fluctuation)
		{
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
				}
			}
			return rect;
		}
		
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
			if(m_nLeftBarType==1) rect.left = rectChart.left;
		}
		else
		{
			if(m_pLeftBarDlg)
			{
				if(m_nLeftBarType==1)
				{
					rect.left = rect.left + (m_nWidthOfLeftBar + WIDTHOFSPLITTER);
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
			}
		}	
		break;
	default:
		break;
	}
	return rect;
}


void CStdDialog::Resize()
{
	//������϶��� ó��
	if(m_nScreenType==Screen_fluctuation)
	{
		if(m_pLeftBarDlg) m_pLeftBarDlg->ShowWindow(SW_HIDE);
		if(m_xSplitter1.GetSafeHwnd()) m_xSplitter1.ShowWindow(SW_HIDE);
		if(m_pInputBarDlg)
		{
			if(m_bSizeTestMode)		m_pInputBarDlg->ShowWindow(SW_SHOW);
			else m_pInputBarDlg->ShowWindow(SW_HIDE);
		}
		if(m_pExtInputBarDlg) m_pExtInputBarDlg->ShowWindow(SW_HIDE);

		if(m_ChartMng.GetSafeHwnd())
		{
			CRect rect, rectNew;	

			m_ChartMng.GetWindowRect(rect);
			ScreenToClient(rect);
			rectNew = GetRect(ID_CHART,m_bInitResize);
			if(rect!=rectNew)
			{
				if(m_bSizeTestMode) rectNew.top = 25;
				m_ChartMng.MoveWindow(rectNew);
			}
		}
		return;
	}

	// ������ƴ� ����Ʈ�϶�.
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
				//<<JS.Kim_20101005   ȭ�� ������ ���� ����
				// �ֽ��� �ڵ��޺� ũ�� ����
				CWnd* pWnd = (CWnd*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
				if(pWnd)
					pWnd->SendMessage(USMG_PLEASE_RESIZE);
				//>>
			}
			//<<JS.Kim_20101005   ȭ�� ������ ���� ����
			//// �ֽ��� �ڵ��޺� ũ�� ����
			//CWnd* pWnd = (CWnd*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
			//if(pWnd)
			//	pWnd->SendMessage(USMG_PLEASE_RESIZE);
			//>>
		}
	}

	if(m_xSplitter1.m_hWnd != NULL )
	{
		m_xSplitter1.GetWindowRect(rect);
		ScreenToClient(rect);
		rectNew = GetRect(ID_SPLITTER,m_bInitResize);
		if(rect!=rectNew)	
		{		
			m_xSplitter1.MoveWindow(rectNew);
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

//-->080520-001 CodeInput����
//		if(m_pInputBarDlg->m_ctrlCodeInput.GetSafeHwnd())
//		{
//			m_pInputBarDlg->m_ctrlCodeInput.SetWindowPos(&CWnd::wndTop, 
//				m_pInputBarDlg->m_rectCmb.left, m_pInputBarDlg->m_rectCmb.top,
//				m_pInputBarDlg->m_rectCmb.Width(), m_pInputBarDlg->m_rectCmb.Height(),
//				SWP_NOZORDER);
//		}
//<--
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
		rectNew.right -= 3;
		rectNew.bottom -= 3;
//		if(rect!=rectNew)	
		{	
			m_ChartMng.MoveWindow(rectNew);
		}
	}

	if(m_bOneClickHide)
	{
		SetOneClickHide(FALSE);
	}
	m_bInitResize = TRUE;
}


void CStdDialog::OnDestroy() 
{
	m_bOnDestroy = TRUE;
// 	if(m_pGwanMng)
// 	{
// 		m_pGwanMng->UnAdvise();
// 		//m_pGwanMng = NULL;
// 		//m_pGwandwRet = 0;
// 	}

	m_pPTManager = NULL;
	
	CString strTemp;
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("������ ��ġ ����::Begin", _T("CStdDialog"), 10);
#endif
	// DRDS ����
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
//			strTemp.Format("%d",m_bShowExtInputBarDlg);
//			::WritePrivateProfileString("Frame","ShowExtInputBarDlg",strTemp,m_strConfig);
		}
	}
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
	
	//===========================================================================
	POSITION pos = NULL;	
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("DRDS ����::Begin", _T("CStdDialog"), 10);
#endif
	AllCodeUnAdvise(); //Real ���� ����..
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("DRDS ����::End", _T("CStdDialog"), 10);
#endif	//===========================================================================
	

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

	if(m_pDataWindow)
	{
		m_pDataWindow->DestroyWindow();
		delete m_pDataWindow;
		m_pDataWindow = NULL;
	}
	
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	CloseLogFile();
#endif

	if(m_pShareInputData) delete m_pShareInputData; m_pShareInputData=NULL;

	// �������Ʈ�̸�...
	if(m_nScreenType==Screen_fluctuation)
	{
		m_FnHandle.SaveData((long)&m_stDlgCompData);
		m_FnHandle.ClearAll((long)&m_stDlgCompData);
	}

	CExOCXDLL::OnDestroy();
}

CString CStdDialog::MakeChartData(LPVOID aTRData, int& nCount, int dwTRDataLen, int nMarketType, int nType, double dPrePrice, int nUnitType/*=WON_TYPE*/)
{
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("�����κ��� ���� Data�� ��Ʈ Data�� ����::Begin", m_strRecvCode, m_strRecvCode.GetLength());
#endif

	//20070226 by LYH
	//{���� ������ ó��
//	m_bPreMarket = FALSE;
	//}}
	if(nCount <= 0)
		return "";

	char* szChartData = NULL;
	CString strChartData;
	int nBufferLength = 0;
	if(nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
		KB_p0602_OutRec2_COMP KB0602OutRec2;
		nBufferLength = sizeof(KB_p0602_OutRec1_EX) + sizeof(KB_p0602_OutRec2_COMP) * nCount + 1;
		szChartData = strChartData.GetBuffer(nBufferLength);

		memcpy(szChartData, (char*)aTRData, sizeof(KB_p0602_OutRec1_EX));
		szChartData += sizeof(KB_p0602_OutRec1_EX);

		CString strData;
		KB_p0602_OutRec2* pChartData = NULL;

		if(nUnitType == PERCENT_TYPE)
		{

			char* szTemp = new char[10 + 1];

			for(int i = 0; i < nCount; ++i)
			{			
				pChartData = (KB_p0602_OutRec2*)((char*)aTRData + sizeof(KB_p0602_OutRec1_EX) + (sizeof(KB_p0602_OutRec2) * i));
				
				memcpy(KB0602OutRec2.date, pChartData->date, sizeof(KB0602OutRec2.date));

				::memset(szTemp, 0x00, 10 + 1);
				::memcpy(szTemp, pChartData->open, sizeof(pChartData->open));
				strData.Format("%10.2f", atof(szTemp));
				memcpy(KB0602OutRec2.open, strData, sizeof(KB0602OutRec2.open));

				::memset(szTemp, 0x00, 10 + 1);
				::memcpy(szTemp, pChartData->high, sizeof(pChartData->high));
				strData.Format("%10.2f", atof(szTemp));
				memcpy(KB0602OutRec2.high, strData, sizeof(KB0602OutRec2.high));

				::memset(szTemp, 0x00, 10 + 1);
				::memcpy(szTemp, pChartData->low, sizeof(pChartData->low));
				strData.Format("%10.2f", atof(szTemp));
				memcpy(KB0602OutRec2.low, strData, sizeof(KB0602OutRec2.low));

				::memset(szTemp, 0x00, 10 + 1);
				::memcpy(szTemp, pChartData->close, sizeof(pChartData->close));
				strData.Format("%10.2f", atof(szTemp));
				memcpy(KB0602OutRec2.close, strData, sizeof(KB0602OutRec2.close));

				//memcpy(KB0602OutRec2.volume, pStdrFid->vvRpValueList[nCount - i][5], sizeof(KB0602OutRec2.volume));
				//memcpy(KB0602OutRec2.rights, pStdrFid->vvRpValueList[nCount - i][6], sizeof(KB0602OutRec2.rights));
				memcpy(szChartData, (LPCSTR)&KB0602OutRec2, sizeof(KB_p0602_OutRec2_COMP));
				szChartData += sizeof(KB_p0602_OutRec2_COMP);
			}

			delete [] szTemp;
		}
		else if(nUnitType == DOLLAR_TYPE)
		{
			for(int i = 0; i < nCount; ++i)
			{
				pChartData = (KB_p0602_OutRec2*)((char*)aTRData + sizeof(KB_p0602_OutRec1_EX) + (sizeof(KB_p0602_OutRec2) * i));

				memcpy(KB0602OutRec2.date, pChartData->date, sizeof(KB0602OutRec2.date));
				memcpy(KB0602OutRec2.open, pChartData->open, sizeof(KB0602OutRec2.open));
				memcpy(KB0602OutRec2.high, pChartData->high, sizeof(KB0602OutRec2.high));
				memcpy(KB0602OutRec2.low, pChartData->low, sizeof(KB0602OutRec2.low));
				memcpy(KB0602OutRec2.close, pChartData->close, sizeof(KB0602OutRec2.close));
				//memcpy(KB0602OutRec2.volume, pStdrFid->vvRpValueList[nCount - i][5], sizeof(KB0602OutRec2.volume));
				//memcpy(KB0602OutRec2.rights, pStdrFid->vvRpValueList[nCount - i][6], sizeof(KB0602OutRec2.rights));
				memcpy(szChartData, (LPCSTR)&KB0602OutRec2, sizeof(KB_p0602_OutRec2_COMP));
				szChartData += sizeof(KB_p0602_OutRec2_COMP);
			}
		}
		else	// nUnitType == WON_TYPE
		{
			for(int i = 0; i < nCount; ++i)
			{
				pChartData = (KB_p0602_OutRec2*)((char*)aTRData + sizeof(KB_p0602_OutRec1_EX) + (sizeof(KB_p0602_OutRec2) * i));

				memcpy(KB0602OutRec2.date, pChartData->date, sizeof(KB0602OutRec2.date));
				memcpy(KB0602OutRec2.open, pChartData->open, sizeof(KB0602OutRec2.open));
				memcpy(KB0602OutRec2.high, pChartData->high, sizeof(KB0602OutRec2.high));
				memcpy(KB0602OutRec2.low, pChartData->low, sizeof(KB0602OutRec2.low));
				memcpy(KB0602OutRec2.close, pChartData->close, sizeof(KB0602OutRec2.close));
				//memcpy(KB0602OutRec2.volume, pStdrFid->vvRpValueList[nCount - i][5], sizeof(KB0602OutRec2.volume));
				//memcpy(KB0602OutRec2.rights, pStdrFid->vvRpValueList[nCount - i][6], sizeof(KB0602OutRec2.rights));

				memcpy(szChartData, (LPCSTR)&KB0602OutRec2, sizeof(KB_p0602_OutRec2_COMP));
				szChartData += sizeof(KB_p0602_OutRec2_COMP);
			}

		}
	}
	else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		KB_p0603_OutRec2_COMP KB0603OutRec2;
		nBufferLength = sizeof(KB_p0603_OutRec1_EX) + sizeof(KB_p0603_OutRec2_COMP) * nCount + 1;
		szChartData = strChartData.GetBuffer(nBufferLength);

		memcpy(szChartData, (char*)aTRData, sizeof(KB_p0603_OutRec1_EX));
		szChartData += sizeof(KB_p0603_OutRec1_EX);

		KB_p0603_OutRec2* pChartData = NULL;

		for(int i = 0; i < nCount; ++i)
		{
			pChartData = (KB_p0603_OutRec2*)((char*)aTRData + sizeof(KB_p0603_OutRec1_EX) + (sizeof(KB_p0603_OutRec2) * i));

			memcpy(KB0603OutRec2.date, pChartData->date, sizeof(KB0603OutRec2.date));
			memcpy(KB0603OutRec2.open, pChartData->open, sizeof(KB0603OutRec2.open));
			memcpy(KB0603OutRec2.high, pChartData->high, sizeof(KB0603OutRec2.high));
			memcpy(KB0603OutRec2.low, pChartData->low, sizeof(KB0603OutRec2.low));
			memcpy(KB0603OutRec2.close, pChartData->close, sizeof(KB0603OutRec2.close));
			//@�񱳿��� ����memcpy(KB0603OutRec2.volume, pStdrFid->vvRpValueList[nCount - i][5], sizeof(KB0603OutRec2.volume));

			memcpy(szChartData, (LPCSTR)&KB0603OutRec2, sizeof(KB_p0603_OutRec2_COMP));
			szChartData += sizeof(KB_p0603_OutRec2_COMP);
		}
	}
	else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
	{
		KB_p0604_OutRec2_COMP KB0604OutRec2;
		nBufferLength = sizeof(KB_p0604_OutRec1_EX) + sizeof(KB_p0604_OutRec2_COMP) * nCount + 1;
		szChartData = strChartData.GetBuffer(nBufferLength);

		memcpy(szChartData, (char*)aTRData, sizeof(KB_p0604_OutRec1_EX));
		szChartData += sizeof(KB_p0604_OutRec1_EX);

		KB_p0604_OutRec2* pChartData = NULL;

		for(int i = 0; i < nCount; ++i)
		{
			pChartData = (KB_p0604_OutRec2*)((char*)aTRData + sizeof(KB_p0604_OutRec1_EX) + (sizeof(KB_p0604_OutRec2) * i));

			memcpy(KB0604OutRec2.date, pChartData->date, sizeof(KB0604OutRec2.date));
			memcpy(KB0604OutRec2.open, pChartData->open, sizeof(KB0604OutRec2.open));
			memcpy(KB0604OutRec2.high, pChartData->high, sizeof(KB0604OutRec2.high));
			memcpy(KB0604OutRec2.low, pChartData->low, sizeof(KB0604OutRec2.low));
			memcpy(KB0604OutRec2.close, pChartData->close, sizeof(KB0604OutRec2.close));
			//@�񱳿��� ����memcpy(KB0604OutRec2.volume, pStdrFid->vvRpValueList[nCount - i][5], sizeof(KB0604OutRec2.volume));

			memcpy(szChartData, (LPCSTR)&KB0604OutRec2, sizeof(KB_p0604_OutRec2_COMP));
			szChartData += sizeof(KB_p0604_OutRec2_COMP);
		}
	}
	else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
		KB_d1013_OutRec2_COMP KB1013OutRec2;
		nBufferLength = sizeof(KB_d1013_OutRec1_EX) + sizeof(KB_d1013_OutRec2_COMP) * nCount + 1;
		szChartData = strChartData.GetBuffer(nBufferLength);

		memcpy(szChartData, (char*)aTRData, sizeof(KB_d1013_OutRec1_EX));
		szChartData += sizeof(KB_d1013_OutRec1_EX);

		KB_d1013_OutRec2* pChartData = NULL;
		CString strData;
		if(nUnitType == PERCENT_TYPE)
		{
			char* szTemp = new char[10 + 1];
			for(int i = 0; i < nCount; ++i)
			{			
				pChartData = (KB_d1013_OutRec2*)((char*)aTRData + sizeof(KB_d1013_OutRec1_EX) + (sizeof(KB_d1013_OutRec2) * i));

				memcpy(KB1013OutRec2.date, pChartData->date, sizeof(KB1013OutRec2.date));

				::memset(szTemp, 0x00, 10 + 1);
				::memcpy(szTemp, pChartData->open, sizeof(pChartData->open));
				strData.Format("%10.2f", atof(szTemp));
				memcpy(KB1013OutRec2.open, strData, sizeof(KB1013OutRec2.open));

				::memset(szTemp, 0x00, 10 + 1);
				::memcpy(szTemp, pChartData->high, sizeof(pChartData->high));
				strData.Format("%10.2f", atof(szTemp));
				memcpy(KB1013OutRec2.high, strData, sizeof(KB1013OutRec2.high));

				::memset(szTemp, 0x00, 10 + 1);
				::memcpy(szTemp, pChartData->low, sizeof(pChartData->low));
				strData.Format("%10.2f", atof(szTemp));
				memcpy(KB1013OutRec2.low, strData, sizeof(KB1013OutRec2.low));

				::memset(szTemp, 0x00, 10 + 1);
				::memcpy(szTemp, pChartData->close, sizeof(pChartData->close));
				strData.Format("%10.2f", atof(szTemp));
				memcpy(KB1013OutRec2.close, strData, sizeof(KB1013OutRec2.close));

				memcpy(szChartData, (LPCSTR)&KB1013OutRec2, sizeof(KB_d1013_OutRec2_COMP));
				szChartData += sizeof(KB_d1013_OutRec2_COMP);
			}
			delete [] szTemp;
		}
		else if(nUnitType == DOLLAR_TYPE)
		{
			for(int i = 0; i < nCount; ++i)
			{
				pChartData = (KB_d1013_OutRec2*)((char*)aTRData + sizeof(KB_d1013_OutRec1_EX) + (sizeof(KB_d1013_OutRec2) * i));
				memcpy(KB1013OutRec2.date, pChartData->date, sizeof(KB1013OutRec2.date));
				memcpy(KB1013OutRec2.open, pChartData->open, sizeof(KB1013OutRec2.open));
				memcpy(KB1013OutRec2.high, pChartData->high, sizeof(KB1013OutRec2.high));
				memcpy(KB1013OutRec2.low, pChartData->low, sizeof(KB1013OutRec2.low));
				memcpy(KB1013OutRec2.close, pChartData->close, sizeof(KB1013OutRec2.close));

				memcpy(szChartData, (LPCSTR)&KB1013OutRec2, sizeof(KB_d1013_OutRec2_COMP));
				szChartData += sizeof(KB_d1013_OutRec2_COMP);
			}
		}
		else	// nUnitType == WON_TYPE
		{
			for(int i = 0; i < nCount; ++i)
			{
				pChartData = (KB_d1013_OutRec2*)((char*)aTRData + sizeof(KB_d1013_OutRec1_EX) + (sizeof(KB_d1013_OutRec2) * i));
				memcpy(KB1013OutRec2.date, pChartData->date, sizeof(KB1013OutRec2.date));
				memcpy(KB1013OutRec2.open, pChartData->open, sizeof(KB1013OutRec2.open));
				memcpy(KB1013OutRec2.high, pChartData->high, sizeof(KB1013OutRec2.high));
				memcpy(KB1013OutRec2.low, pChartData->low, sizeof(KB1013OutRec2.low));
				memcpy(KB1013OutRec2.close, pChartData->close, sizeof(KB1013OutRec2.close));

				memcpy(szChartData, (LPCSTR)&KB1013OutRec2, sizeof(KB_d1013_OutRec2_COMP));
				szChartData += sizeof(KB_d1013_OutRec2_COMP);
			}
		}
	}
	else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		KB_p0606_OutRec2_COMP KB0606OutRec2;
		nBufferLength = sizeof(KB_p0606_OutRec1_EX) + sizeof(KB_p0606_OutRec2_COMP) * nCount + 1;
		szChartData = strChartData.GetBuffer(nBufferLength);

		memcpy(szChartData, (char*)aTRData, sizeof(KB_p0606_OutRec1_EX));
		szChartData += sizeof(KB_p0606_OutRec1_EX);

		KB_p0606_OutRec2* pChartData = NULL;
		for(int i = 0; i < nCount; ++i)
		{
			pChartData = (KB_p0606_OutRec2*)((char*)aTRData + sizeof(KB_p0606_OutRec1_EX) +(sizeof(KB_p0606_OutRec2) * i));

			memcpy(KB0606OutRec2.date, pChartData->date, sizeof(KB0606OutRec2.date));
			memcpy(KB0606OutRec2.open, pChartData->open, sizeof(KB0606OutRec2.open));
			memcpy(KB0606OutRec2.high, pChartData->high, sizeof(KB0606OutRec2.high));
			memcpy(KB0606OutRec2.low, pChartData->low, sizeof(KB0606OutRec2.low));
			memcpy(KB0606OutRec2.close, pChartData->close, sizeof(KB0606OutRec2.close));
//			memcpy(KB0606OutRec2.volume, pStdrFid->vvRpValueList[nCount - i][5], sizeof(KB0606OutRec2.volume));

			memcpy(szChartData, (LPCSTR)&KB0606OutRec2, sizeof(KB_p0606_OutRec2_COMP));
			szChartData += sizeof(KB_p0606_OutRec2_COMP);
		}
	}
	strChartData.ReleaseBuffer();

//	if( atoi(strHigh) < atoi(strClose) )
//		m_bPreMarket = TRUE;

	return strChartData;

//	CString strHigh, strClose, strOut, strOut2;
//	for(int i = 0; i < nCount; ++i)
//	{
//		//��¥
//		strData = m_TRComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 8, &nIdx, 0, TRUE);
//		// Intra_Ticks �߰� - ojtaso (20071227)
//		if(nType == TICK_DATA_CHART)
//		{
////			strData.Format("%6.6s  ", strData);
//
//			strData.Format("%2.2s", strData.Right(2));
//			CString strTime = m_TRComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 8, &nIdx, 0, TRUE);
//			if(atoi(strTime.Mid(4,6)) != 0)
//			{
//				int nHour = atoi(strTime.Left(2));
//				int nMin  = atoi(strTime.Mid(2,2));
//				int nSec  = atoi(strTime.Mid(4,2));
///*
//				if(nSec>=60)
//				{
//					nMin += 1;
//					nSec = 0;
//				}
//
//				if(nMin>=60)
//				{
//					nHour += 1;
//					nMin = 0;
//				}*/
//
//				strTime.Format("%02d%02d%02d", nHour, nMin, nSec);
//			}
//			strData += strTime.Left(6);
//
//		}
//		else if(nType == MIN_DATA_CHART)
//		{
//			strData.Format("%4.4s", strData.Mid(4, 4));
//			CString strTime = m_TRComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 8, &nIdx, 0, TRUE);
//			if(atoi(strTime.Mid(4,6)) != 0)
//			{
//				int nHour = atoi(strTime.Mid(0,2));
//				int nMin  = atoi(strTime.Mid(2,2)) + 1;
//				if(nMin>=60)
//				{
//					nHour += 1;
//					nMin = 0;
//				}
//				strTime.Format("%02d%02d", nHour, nMin);
//			}
//			strData += strTime.Mid(0,4);
//		}
//		strOut += strData;
//
//		//if(nType == TICK_DATA_CHART)
//		//{
//		//	//�ð�, ��, ����, ����
//		//	strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
//		//	if(nUnitType == PERCENT_TYPE && dPrePrice != 0)
//		//	{
//		//		double rate = (atof(strData)/dPrePrice - 1) * 100.0;
//		//		strData.Format("%10.2f", rate);
//		//	}
//		//	else if(nUnitType == DOLLAR_TYPE && dDollar != 0)
//		//	{
//		//		rate = atof(strData)/dDollar;
//		//		strData.Format("%10.2f", rate);
//		//	}
//
//		//	strOut += strData;
//		//	strOut += strData;
//		//	strOut += strData;
//		//	strOut += strData;
//		//}
//		//else
//		//{
//		if(nMarketType == FOREIGN_CHART && nType == TICK_DATA_CHART)
//		{
//			//�ð�,��,����,����
//			strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
//			strOut += strData;
//			strOut += strData;
//			strOut += strData;
//			strOut += strData;
//
//			if(i==0)
//			{
//				strHigh = strData;
//				strClose = strData;
//			}
//			strOut += "          ";
//
////			strData = m_TRComm->ApplyByteOrder(nVolType, (char *)aTRData+nIdx, 10, &nIdx);
////			strOut += strData;
//		}
//		else
//		{
//			if(nUnitType == PERCENT_TYPE && dPrePrice != 0)
//			{
//				//�ð�
//				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
//				rate = (atof(strData)/dPrePrice - 1) * 100.0;
//				strData.Format("%10.2f", rate);
//				strOut += strData;
//
//				//��
//				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
//				if(i==0)
//					strHigh = strData;
//
//				rate = (atof(strData)/dPrePrice - 1) * 100.0;
//				strData.Format("%10.2f", rate);
//				strOut += strData;
//
//				//����
//				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
//				rate = (atof(strData)/dPrePrice - 1) * 100.0;
//				strData.Format("%10.2f", rate);
//				strOut += strData;
//
//				//����
//				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
//				if(i==0)
//					strClose = strData;
//
//				rate = (atof(strData)/dPrePrice - 1) * 100.0;
//				strData.Format("%10.2f", rate);
//				strOut += strData;
//			}
//			else if(nUnitType == DOLLAR_TYPE && dDollar != 0)
//			{
//				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
//				rate = atof(strData)/dDollar;
//				strData.Format("%10.2f", rate);
//				strOut += strData;
//
//				//��
//				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
//				if(i==0)
//					strHigh = strData;
//
//				rate = atof(strData)/dDollar;
//				strData.Format("%10.2f", rate);
//				strOut += strData;
//
//				//����
//				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
//				rate = atof(strData)/dDollar;
//				strData.Format("%10.2f", rate);
//				strOut += strData;
//
//				//����
//				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
//				if(i==0)
//					strClose = strData;
//
//				rate = atof(strData)/dDollar;
//				strData.Format("%10.2f", rate);
//				strOut += strData;
//			}
//			else
//			{
//				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
//				strOut += strData;
//
//				//��
//				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
//				if(i==0)
//					strHigh = strData;
//
//				strOut += strData;
//
//				//����
//				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
//				strOut += strData;
//
//				//����
//				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
//				if(i==0)
//					strClose = strData;
//				strOut += strData;
//			}
//			//}
//			//�ŷ���
//			strData = m_TRComm->ApplyByteOrder(nVolType, (char *)aTRData+nIdx, 10, &nIdx);
//			strOut += strData;
//		}
//		//�Ǹ���
//		if(nMarketType == CMasterTypeInfo::STOCK_CHART)
//		{
//			if(nType == DAY_DATA_CHART)
//			{
//				strData = m_TRComm->ApplyByteOrder(C_DT_BYTE, (char *)aTRData+nIdx, 3, &nIdx);
//				nRights = atoi(strData);
//				if(nRights > 0)
//				{
//					switch(nRights)
//					{
//					case 10:
//					case 20:
//					case 30:
//					case 40:
//					case 50:
//					case 60:
//					case 70:
//						strData.Format("1%d", nRights);
//						strOut += strData;
//						break;
//					case 61:
//						strOut += "260";
//						break;
//					case 71:
//						strOut += "270";
//						break;
//					case 81:
//						strOut += "280";
//						break;
//					case 91:
//						strOut += "110";
//						break;
//					default:
//						strOut += "   ";
//						break;
//					}
//				}
//				else
//					strOut += "   ";
//				
//			}
//			else
//			{
//				strOut += "   ";
//			}
//		}
//		strOut2 = strOut + strOut2;
//	}
//	//20070226 by LYH
//	//{���� ������ ó��
//	if( atoi(strHigh) < atoi(strClose) )
//		m_bPreMarket = TRUE;
//	//}}

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("�����κ��� ���� Data�� ��Ʈ Data�� ����::End", m_strRecvCode, m_strRecvCode.GetLength());
#endif

//	return strOut2;
}

//@solomon 
//aTRData : Recieve Data, nIdx:Start Index, nCount: count, dwTRDataLen: Data Length, nMarketType: Stock, Upjong, Future Type, nType : Tick/Min...
CString CStdDialog::MakeChartData(LPVOID aTRData, int nIdx, int& nCount, int dwTRDataLen, int nMarketType, int nType, double dPrePrice, int nUnitType/*=WON_TYPE*/)
{
//@Solomon-->
	return "";

// 	#ifdef _WRITELOG
// 		// Log - ojtaso (20080107)
// 		WriteToStringLog("�����κ��� ���� Data�� ��Ʈ Data�� ����::Begin", m_strRecvCode, m_strRecvCode.GetLength());
// 	#endif
// 
// 		//20070226 by LYH
// 		//{���� ������ ó��
// 		m_bPreMarket = FALSE;
// 		//}}
// 		if(nCount < 0)
// 			nCount = 400;
// 		//������
// 		CString strData, strOut, strOut2;
// 
// 		//20070226 by LYH
// 		//{������ǥ ������ȸ ���� ����
// 		CString strHigh, strClose;
// 		//}}
// 
// 		double rate;
// 		int nPriceType = C_DT_ULONG, nVolType = C_DT_ULONG, nDotPos = 0;
// 		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
// 		int nLength=0, nExp=0;
// 		BYTE nCodeGubun = atoi(m_pInputBarDlg->m_pDataManager->GetCodeInfo("", 2, m_strRecvCode));
// 
// 		double dDollar = atof(m_strDollar);
// 		switch(nMarketType)
// 		{
// 		case FUTOPT_CHART:
// 	//		nPriceType = C_DT_SHORT;
// 			nDotPos = 2;
// 			if(nType == TICK_DATA_CHART)
// 				nVolType = C_DT_USHORT;
// 			break;
// 		case UPJONG_CHART:
// 			nDotPos = 2;
// 			//		if(nType == TICK_DATA_CHART)
// 			//			nVolType = C_DT_USHORT;
// 			break;
// 		case FOREIGN_CHART:
// 			nDotPos = 2;
// 			break;
// 		}
// 
// 		int nRights = 0;
// 		for(int i=0; i<nCount; i++)
// 		{
// 			if(nIdx>=dwTRDataLen)
// 			{
// 				nCount = i;
// 				//strCount.Format("%d", i);
// 				break;
// 			}
// 
// 			strOut = "";
// 			//��¥
// 			strData = m_TRComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 8, &nIdx, 0, TRUE);
// 			// Intra_Ticks �߰� - ojtaso (20071227)
// 			if(nType == TICK_DATA_CHART)
// 			{
// 	//			strData.Format("%6.6s  ", strData);
// 
// 				strData.Format("%2.2s", strData.Right(2));
// 				CString strTime = m_TRComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 8, &nIdx, 0, TRUE);
// 				if(atoi(strTime.Mid(4,6)) != 0)
// 				{
// 					int nHour = atoi(strTime.Left(2));
// 					int nMin  = atoi(strTime.Mid(2,2));
// 					int nSec  = atoi(strTime.Mid(4,2));
// 
// 					strTime.Format("%02d%02d%02d", nHour, nMin, nSec);
// 				}
// 				strData += strTime.Left(6);
// 
// 			}
// 			else if(nType == MIN_DATA_CHART)
// 			{
// 				strData.Format("%4.4s", strData.Mid(4, 4));
// 				CString strTime = m_TRComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 8, &nIdx, 0, TRUE);
// 				if(atoi(strTime.Mid(4,6)) != 0)
// 				{
// 					int nHour = atoi(strTime.Mid(0,2));
// 					int nMin  = atoi(strTime.Mid(2,2)) + 1;
// 					if(nMin>=60)
// 					{
// 						nHour += 1;
// 						nMin = 0;
// 					}
// 					strTime.Format("%02d%02d", nHour, nMin);
// 				}
// 				strData += strTime.Mid(0,4);
// 			}
// 			strOut += strData;
// 
// 			if(nMarketType == FOREIGN_CHART && nType == TICK_DATA_CHART)
// 			{
// 				//�ð�,��,����,����
// 				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
// 				strOut += strData;
// 				strOut += strData;
// 				strOut += strData;
// 				strOut += strData;
// 
// 				if(i==0)
// 				{
// 					strHigh = strData;
// 					strClose = strData;
// 				}
// 				strOut += "          ";
// 
// 			}
// 			else
// 			{
// 				if(nUnitType == PERCENT_TYPE && dPrePrice != 0)
// 				{
// 					//�ð�
// 					strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
// 					rate = (atof(strData)/dPrePrice - 1) * 100.0;
// 					strData.Format("%10.2f", rate);
// 					strOut += strData;
// 
// 					//��
// 					strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
// 					if(i==0)
// 						strHigh = strData;
// 
// 					rate = (atof(strData)/dPrePrice - 1) * 100.0;
// 					strData.Format("%10.2f", rate);
// 					strOut += strData;
// 
// 					//����
// 					strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
// 					rate = (atof(strData)/dPrePrice - 1) * 100.0;
// 					strData.Format("%10.2f", rate);
// 					strOut += strData;
// 
// 					//����
// 					strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
// 					if(i==0)
// 						strClose = strData;
// 
// 					rate = (atof(strData)/dPrePrice - 1) * 100.0;
// 					strData.Format("%10.2f", rate);
// 					strOut += strData;
// 				}
// 				else if(nUnitType == DOLLAR_TYPE && dDollar != 0)
// 				{
// 					strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
// 					rate = atof(strData)/dDollar;
// 					strData.Format("%10.2f", rate);
// 					strOut += strData;
// 
// 					//��
// 					strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
// 					if(i==0)
// 						strHigh = strData;
// 
// 					rate = atof(strData)/dDollar;
// 					strData.Format("%10.2f", rate);
// 					strOut += strData;
// 
// 					//����
// 					strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
// 					rate = atof(strData)/dDollar;
// 					strData.Format("%10.2f", rate);
// 					strOut += strData;
// 
// 					//����
// 					strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
// 					if(i==0)
// 						strClose = strData;
// 
// 					rate = atof(strData)/dDollar;
// 					strData.Format("%10.2f", rate);
// 					strOut += strData;
// 				}
// 				else
// 				{
// 					strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
// 					strOut += strData;
// 
// 					//��
// 					strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
// 					if(i==0)
// 						strHigh = strData;
// 
// 					strOut += strData;
// 
// 					//����
// 					strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
// 					strOut += strData;
// 
// 					//����
// 					strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
// 					if(i==0)
// 						strClose = strData;
// 					strOut += strData;
// 				}
// 				//}
// 				//�ŷ���
// 				strData = m_TRComm->ApplyByteOrder(nVolType, (char *)aTRData+nIdx, 10, &nIdx);
// 				strOut += strData;
// 			}
// 			//�Ǹ���
// 			if(nMarketType == CMasterTypeInfo::STOCK_CHART)
// 			{
// 				if(nType == DAY_DATA_CHART)
// 				{
// 					strData = m_TRComm->ApplyByteOrder(C_DT_BYTE, (char *)aTRData+nIdx, 3, &nIdx);
// 					nRights = atoi(strData);
// 					if(nRights > 0)
// 					{
// 						switch(nRights)
// 						{
// 						case 10:
// 						case 20:
// 						case 30:
// 						case 40:
// 						case 50:
// 						case 60:
// 						case 70:
// 							strData.Format("1%d", nRights);
// 							strOut += strData;
// 							break;
// 						case 61:
// 							strOut += "260";
// 							break;
// 						case 71:
// 							strOut += "270";
// 							break;
// 						case 81:
// 							strOut += "280";
// 							break;
// 						case 91:
// 							strOut += "110";
// 							break;
// 						default:
// 							strOut += "   ";
// 							break;
// 						}
// 					}
// 					else
// 						strOut += "   ";
// 					
// 				}
// 				else
// 				{
// 					strOut += "   ";
// 				}
// 			}
// 			strOut2 = strOut + strOut2;
// 		}
// 		//20070226 by LYH
// 		//{���� ������ ó��
// 		if( atoi(strHigh) < atoi(strClose) )
// 			m_bPreMarket = TRUE;
// 		//}}
// 
// 	#ifdef _WRITELOG
// 		// Log - ojtaso (20080107)
// 		WriteToStringLog("�����κ��� ���� Data�� ��Ʈ Data�� ����::End", m_strRecvCode, m_strRecvCode.GetLength());
// 	#endif
// 
// 		return strOut2;

//@Solomon<--

}

// Function name	: CStdDialog::RecieveData
// Author			: Sang-Yun, Nam
// Description	    : TR Data�� �޴´�.
// Return type		: HRESULT
// Argument         : HWND		hTrComm		--	TRComm Handle
// Argument         : LPCSTR	szTR		--	TR No.
// Argument         : LPCSTR	szMsgCode	--	Result Code
// Argument         : LPCSTR	szMessage	--	Result Message
// Argument         : LPVOID	aTRData		--	��ȸ ����Ÿ Pointer
// Argument         : long		dwTRDateLen	--	��ȸ ����Ÿ ����
void CStdDialog::ConvertKoscomData(LPCSTR szTR, LPVOID	aTRData, long dwTRDateLen, LPCSTR szCode)
{
	CString strTRCode = szTR;
	strTRCode.Remove(' ');
	if(strTRCode == QUERY_STRJONGMOK)
	{
		KB_p0602_OutRec1_EX* pOut1 = (KB_p0602_OutRec1_EX*)aTRData;

		// �����޼���
		CString strBojoMsg = CString(pOut1->bojomsg, sizeof(pOut1->bojomsg));
		//strBojoMsg.Format("0396%-396s", pOut1->bojomsg);
		
		GetSCDDataFromBosomsg(szCode, strBojoMsg, szTR);
	}
	else if(strTRCode == QUERY_STRFUTOPT)
	{
		KB_p0604_OutRec1_EX* pOut1 = (KB_p0604_OutRec1_EX*)aTRData;
		
		// �����޼���
		CString strBojoMsg = CString(pOut1->bojomsg, sizeof(pOut1->bojomsg));
		//strBojoMsg.Format("0396%-396s", pOut1->bojomsg);
		
		GetSCDDataFromBosomsg(szCode, strBojoMsg, szTR);
	}
	else if(strTRCode == QUERY_STRSTOCKFUTOPT)
	{
		KB_d1013_OutRec1_EX* pOut1 = (KB_d1013_OutRec1_EX*)aTRData;

		// �����޼���
		CString strBojoMsg = CString(pOut1->bojomsg, sizeof(pOut1->bojomsg));
		//strBojoMsg.Format("0396%-396s", pOut1->bojomsg);
		
		GetSCDDataFromBosomsg(szCode, strBojoMsg, szTR);
	}
	else if(strTRCode == QUERY_STRUPJONG)
	{
		KB_p0603_OutRec1_EX* pOut1 = (KB_p0603_OutRec1_EX*)aTRData;

		// �����޼���
		CString strBojoMsg = CString(pOut1->bojomsg, sizeof(pOut1->bojomsg));
		//strBojoMsg.Format("0396%-396s", pOut1->bojomsg);
		
		GetSCDDataFromBosomsg(szCode, strBojoMsg, szTR);
	}
	else if(strTRCode == QUERY_STRFOREIGN)
	{
		KB_p0606_OutRec1_EX* pOut1 = (KB_p0606_OutRec1_EX*)aTRData;

		// �����޼���
		CString strBojoMsg = CString(pOut1->bojomsg, sizeof(pOut1->bojomsg));
		//strBojoMsg.Format("0396%-396s", pOut1->bojomsg);
		
		GetSCDDataFromBosomsg(szCode, strBojoMsg, szTR);
	}
}

// 	void CStdDialog::ConvertKoscomData(LPCSTR szTR, LPVOID	aTRData, long dwTRDateLen, LPCSTR szCode)
// 	{
// 		TRACE("dwTRDateLen=%d\n", dwTRDateLen);
// 
// 		CString strTRCode = szTR;
// 		strTRCode.Remove(' ');
// 		if(strTRCode == QUERY_STRJONGMOK)
// 		{
// 			KB_p0602_OutRec1_EX* pOut1 = (KB_p0602_OutRec1_EX*)aTRData;
// 			m_strOut = "";
// 
// 			CString strOut;
// 			// �����
// 			char* szTemp = new char[400 + 1];
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->name, sizeof(pOut1->name));
// 			strOut += szTemp;
// 
// 			// ���簡
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->price, sizeof(pOut1->price));
// 			strOut += szTemp;
// 			
// 			// �����ȣ
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->sign, sizeof(pOut1->sign));
// 			strOut += szTemp;
// 
// 			// ���
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->change, sizeof(pOut1->change));
// 			strOut += szTemp;
// 
// 			// �����
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->chrate, sizeof(pOut1->chrate));
// 			strOut += szTemp;
// 
// 			// ���ذ�
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->preprice, sizeof(pOut1->preprice));
// 			strOut += szTemp;
// 
// 			m_stTrInfo2.m_dPrevPrice = atof(szTemp);
// 
// 			// ����Ű
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->nkey, sizeof(pOut1->nkey));
// 			strOut += szTemp;
// 
// 			// Chart Count
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->fcnt, sizeof(pOut1->fcnt));
// 			strOut += szTemp;
// 
// 			m_stTrInfo2.m_nChartCount = atoi(szTemp);
// 
// 			// �����޼���
// 			CString strBojoMsg;
// 			strBojoMsg.Format("0396%-396s", pOut1->bojomsg);
// 			strOut += strBojoMsg;
// 			
// 			delete [] szTemp;
// 
// 			//@080728-001 ��ȸ������ ��������.
// 			GetSCDDataFromBosomsg(szCode, strBojoMsg, szTR);
// 
// 			m_strOut = strOut;// + strChartData;
// 
// 	#ifdef _WRITELOG
// 			// Log - ojtaso (20080107)
// 			WriteToStringLog("�ֽ����� �����κ��� ����_���簡::End", strCode, strCode.GetLength());
// 	#endif
// 		}
// 		else if(strTRCode == QUERY_STRFUTOPT)
// 		{
// 	//@�����߰�
// 			KB_p0604_OutRec1* pOut1 = (KB_p0604_OutRec1*)aTRData;
// 			m_strOut = "";
// 
// 			CString strOut;
// 			// �����
// 			char* szTemp = new char[400 + 1];
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->name, sizeof(pOut1->name));
// 			strOut += szTemp;
// 			// ���簡
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->price, sizeof(pOut1->price));
// 			strOut += szTemp;
// 			// �����ȣ
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->sign, sizeof(pOut1->sign));
// 			strOut += szTemp;
// 			// ���
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->change, sizeof(pOut1->change));
// 			strOut += szTemp;
// 			// �����
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->chrate, sizeof(pOut1->chrate));
// 			strOut += szTemp;
// 			// ���ذ�
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->preprice, sizeof(pOut1->preprice));
// 			strOut += szTemp;
// 
// 			m_stTrInfo2.m_dPrevPrice = atof(szTemp);
// 			// ����Ű
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->nkey, sizeof(pOut1->nkey));
// 			strOut += szTemp;
// 			// Chart Count
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->fcnt, sizeof(pOut1->fcnt));
// 			strOut += szTemp;
// 
// 			m_stTrInfo2.m_nChartCount = atoi(szTemp);
// 			// �����޼���
// 			CString strBojoMsg;
// 			strBojoMsg.Format("0396%-396s", pOut1->bojomsg);
// 			strOut += strBojoMsg;
// 
// 			delete [] szTemp;
// 
// 			//@080728-001 ��ȸ������ ��������.
// 			GetSCDDataFromBosomsg(szCode, strBojoMsg, szTR);
// 			
// 			m_strOut = strOut;// + strChartData;
// 		}
// 		else if(strTRCode == QUERY_STRSTOCKFUTOPT)
// 		{
// 	//@�����߰�
// 			KB_d1013_OutRec1* pOut1 = (KB_d1013_OutRec1*)aTRData;
// 			m_strOut = "";
// 
// 			CString strOut;
// 			// �����
// 			char* szTemp = new char[400 + 1];
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->name, sizeof(pOut1->name));
// 			strOut += szTemp;
// 			// ���簡
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->price, sizeof(pOut1->price));
// 			strOut += szTemp;
// 			// �����ȣ
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->sign, sizeof(pOut1->sign));
// 			strOut += szTemp;
// 			// ���
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->change, sizeof(pOut1->change));
// 			strOut += szTemp;
// 			// �����
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->chrate, sizeof(pOut1->chrate));
// 			strOut += szTemp;
// 			// ���ذ�
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->preprice, sizeof(pOut1->preprice));
// 			strOut += szTemp;
// 
// 			m_stTrInfo2.m_dPrevPrice = atof(szTemp);
// 			// ����Ű
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->nkey, sizeof(pOut1->nkey));
// 			strOut += szTemp;
// 			// Chart Count
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->fcnt, sizeof(pOut1->fcnt));
// 			strOut += szTemp;
// 
// 			m_stTrInfo2.m_nChartCount = atoi(szTemp);
// 			// �����޼���
// 			CString strBojoMsg;
// 			strBojoMsg.Format("0396%-396s", pOut1->bojomsg);
// 			strOut += strBojoMsg;
// 
// 			delete [] szTemp;
// 
// 			//@080728-001 ��ȸ������ ��������.
// 			GetSCDDataFromBosomsg(szCode, strBojoMsg, szTR);
// 			
// 			m_strOut = strOut;// + strChartData;
// 		}
// 		else if(strTRCode == QUERY_STRUPJONG)
// 		{
// 	//@�����߰�
// 			KB_p0603_OutRec1* pOut1 = (KB_p0603_OutRec1*)aTRData;
// 			m_strOut = "";
// 
// 			CString strOut;
// 			// �����
// 			char* szTemp = new char[400 + 1];
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->name, sizeof(pOut1->name));
// 			strOut += szTemp;
// 			// ���簡
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->price, sizeof(pOut1->price));
// 			strOut += szTemp;
// 			// �����ȣ
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->sign, sizeof(pOut1->sign));
// 			strOut += szTemp;
// 			// ���
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->change, sizeof(pOut1->change));
// 			strOut += szTemp;
// 			// �����
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->chrate, sizeof(pOut1->chrate));
// 			strOut += szTemp;
// 			// ���ذ�
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->preprice, sizeof(pOut1->preprice));
// 			strOut += szTemp;
// 
// 			m_stTrInfo2.m_dPrevPrice = atof(szTemp);
// 			// ����Ű
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->nkey, sizeof(pOut1->nkey));
// 			strOut += szTemp;
// 			// Chart Count
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->fcnt, sizeof(pOut1->fcnt));
// 			strOut += szTemp;
// 
// 			m_stTrInfo2.m_nChartCount = atoi(szTemp);
// 			// �����޼���
// 			CString strBojoMsg;
// 			strBojoMsg.Format("0396%-396s", pOut1->bojomsg);
// 			strOut += strBojoMsg;
// 			
// 			delete [] szTemp;
// 
// 			//@080728-001 ��ȸ������ ��������.
// 			GetSCDDataFromBosomsg(szCode, strBojoMsg, szTR);
// 
// 			m_strOut = strOut;// + strChartData;
// 		}
// 		else if(strTRCode == QUERY_STRFOREIGN)
// 		{
// 			KB_p0606_OutRec1* pOut1 = (KB_p0606_OutRec1*)aTRData;
// 			m_strOut = "";
// 
// 			CString strOut;
// 			// �����
// 			char* szTemp = new char[400 + 1];
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->name, sizeof(pOut1->name));
// 			strOut += szTemp;
// 			// ���簡
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->price, sizeof(pOut1->price));
// 			strOut += szTemp;
// 			// �����ȣ
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->sign, sizeof(pOut1->sign));
// 			strOut += szTemp;
// 			// ���
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->change, sizeof(pOut1->change));
// 			strOut += szTemp;
// 			// �����
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->chrate, sizeof(pOut1->chrate));
// 			strOut += szTemp;
// 			// ���ذ�
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->preprice, sizeof(pOut1->preprice));
// 			strOut += szTemp;
// 
// 			m_stTrInfo2.m_dPrevPrice = atof(szTemp);
// 			// ����Ű
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->nkey, sizeof(pOut1->nkey));
// 			strOut += szTemp;
// 			// Chart Count
// 			::memset(szTemp, 0x00, 400 + 1);
// 			::memcpy(szTemp, pOut1->fcnt, sizeof(pOut1->fcnt));
// 			strOut += szTemp;
// 
// 			m_stTrInfo2.m_nChartCount = atoi(szTemp);
// 			// �����޼���
// 			CString strBojoMsg;
// 			strBojoMsg.Format("0396%-396s", pOut1->bojomsg);
// 			strOut += strBojoMsg;
// 			
// 			delete [] szTemp;
// 
// 			//@080728-001 ��ȸ������ ��������.
// 			GetSCDDataFromBosomsg(szCode, strBojoMsg, szTR);
// 
// 			m_strOut = strOut;// + strChartData;
// 		}
// 	}

HRESULT CStdDialog::DemoReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	CWnd *pWndRequested = NULL;
	CString strTRCode = szTR;
	strTRCode.Remove(' ');

	STTRINFO trInfo;
	long lCnt = m_arryQuery.GetSize();
	CString sTemp;

	for(long lPos=0;lPos<lCnt;lPos++)
	{
		trInfo = m_arryQuery.GetAt(lPos);
		if(trInfo.m_strTrCode==strTRCode)
		{

			pWndRequested = trInfo.m_pWndRequested;
			m_arryQuery.RemoveAt(lPos);

			lCnt = m_arryQuery.GetSize();
			break;
		}
	}


	//============================================================================



	if(strTRCode=="HWS09501")
	{	
		if(m_pRqWndFromTab)
		{			
			m_pRqWndFromTab->SendMessage(UMSG_RECEIVE_TR09501, 0, (long)aTRData);			
			SendMessage2StatusBar(MSG_SEARCH_END, (LPTSTR)(LPCTSTR)szMessage);
		}
		else
			SendMessage2StatusBar(MSG_SEARCH_FAIL, (LPTSTR)(LPCTSTR)szMessage);

		EnableWindowAll(TRUE);	

		return 0L;
	}

	if(pWndRequested)
	{
//@Solomon-->
// 		if(strTRCode==QUERY_STR_AVGMP)
// 		{
// 			CStringList strlistSetAll;
// 			char szWndChartItem[18];
// 			CChartItem *pWndChartItem = NULL;
// 			HWND hWndChartItem = NULL;
// 			D10418_O st10418o;
// 			memset(&st10418o, 0x00, sizeof(D10418_O));
// 			memcpy(&st10418o,aTRData,dwTRDateLen);
// 			st10418o.chNull = NULL;
// 			MEMCPYWITHNULL(szWndChartItem,st10418o.sBuffer);
// 			hWndChartItem = (HWND)atol(szWndChartItem);
// 
// 			pWndChartItem = (CChartItem *)CWnd::FromHandle(hWndChartItem);
// 			if(pWndChartItem->GetSafeHwnd())
// 			{
// 				strlistSetAll.AddTail(GetAccountInfo());
// 				strlistSetAll.AddTail((char*)&st10418o.sShcode[0]);
// 				pWndChartItem->InvokeAddIn("ChartGuideLine.dll",INVOKEMSG_SETACCOUNTNQUOTECODE,(long)&strlistSetAll);
// 			}
// 
// 			EnableWindowAll(TRUE);
//		}	
//		else
//@Solomon<--
		{
			if(pWndRequested->GetSafeHwnd())
			{
				stReceiveFromServer stR;
				stR.m_strTRCode = strTRCode;
				stR.m_pBuffer = aTRData;
				stR.m_dwLength = dwTRDateLen;
				//				if(dwTRDateLen < 30)
				//				{
				//#ifdef _DEBUG
				//				//	CString sTemp = CString(szTR) + "��ȸ����";
				//				//	AfxMessageBox(sTemp);
				//#endif
				//					SendMessage2StatusBar(MSG_SEARCH_FAIL, (LPTSTR)(LPCTSTR)szMessage);
				//					EnableWindowAll();
				//					return S_FALSE;
				//				}

				//������� �������� �ʾƼ� �̺κ��� �߰��� 05.06.24========================
				if(stR.m_strTRCode == QUERY_STRUPJONG)
				{
					//char szTemp[64];
					S35100_O* pstOutput = (S35100_O*)stR.m_pBuffer;
					if(m_ChartMng.GetSafeHwnd())
					{
						//CString sTemp = m_pInputBarDlg->GetCurCodeNameFromControl();
						//sTemp = m_ChartMng.GetCurSelChartItemJMName();
						//memcpy(pstOutput->name, (LPTSTR)(LPCTSTR)sTemp, sTemp.GetLength());
					}
				}


				//========================================================================	
				//				CString sTemp;
				//				sTemp.Format("Query %s", (char*)(stR.m_pBuffer));
				//				OutputDebugString(sTemp);
				pWndRequested->SendMessage(UMSG_RECEIVEFROMSERVER,0,(long)&stR);


			}
		}
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
		if(pWndRequested)
		{
			stReceiveFromServer stR;	

			stR.m_strTRCode = szTR;
			stR.m_pBuffer = aTRData;
			stR.m_dwLength = dwTRDateLen;
			if(dwTRDateLen>=12)
			{
				CChartItem *pWndChartItem = NULL;
				char szWndChartItem[18];
				memcpy(szWndChartItem,aTRData,12);
				szWndChartItem[12] = NULL;
				pWndChartItem = (CChartItem *)atol(szWndChartItem);
				if(pWndChartItem->GetSafeHwnd())
				{
					pWndChartItem->SendMessageToInvoke(INVOKEMSG_RECEIVEDATA,(long)&stR, sizeof(stReceiveFromServer));					
				}
			}
		}
	}


	BOOL bRet = m_ChartMng.IsToBeReceivedMore();
	if(!m_arryQuery.GetSize()&&!bRet)
	{
		EnableWindowAll();
	}	

	SendMessage2StatusBar(MSG_SEARCH_END, (LPTSTR)(LPCTSTR)szMessage);
	//EnableWindowAll(TRUE);
	return S_OK;
}


// �Ҹ���ġ �÷������� Ŀ�� ���� �޽����� ������.
// CommApi���� Form Wnd�� �ִ� BeginWaitCursor�� �⵿�ϰ� �Ѵ�.
#define WM_BEGIN_WAIT_CURSOR	WM_USER + 610
#define WM_END_WAIT_CURSOR		WM_USER + 611
HRESULT	CStdDialog::StatusChange(int nState)
{
	if(nState==WM_BEGIN_WAIT_CURSOR)	GetParent()->SendMessage(WM_USER+105, 0, 0);
	else if(nState==WM_END_WAIT_CURSOR)	GetParent()->SendMessage(WM_USER+106, 0, 0);
	
	return S_OK;
}

HRESULT CStdDialog::ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	CWnd *pWndRequested = NULL;
	CString strTRCode = szTR;
	if(strTRCode.IsEmpty()) return 0;

//@solomon ���� 20091019 ����ȣ
// 	//@080714-001 �е������� ��ȸTR�� �����䱸�� ���� �����ϸ� ������.
// 	strTRCode.Replace("p0612", "p0602");
// 	strTRCode.Replace("p0613", "p0603");
// 	strTRCode.Replace("p0614", "p0604");
// 	strTRCode.Replace("p0615", "p0605");


//	strTRCode.Remove(' ');

	CString strMsgCode = szMsgCode;
//	if(strMsgCode.GetLength() >0) return S_OK;
	if(strMsgCode.GetLength() <= 0)
	{
		EnableWindowAll(TRUE);	
	}
	//@Solomon:091123SM068	-->
	else if (strMsgCode.CompareNoCase(QUERY_STRGUIDELINE) == 0)
	{
		return 0;
	}
	//@Solomon:091123SM068	<--
	else
	{
		if(strMsgCode.GetLength() > 0 || dwTRDateLen == 0)
		{
			if(!m_bEnable)
				EnableWindowAll(TRUE);	
		}
		
		return 1L;
	}

	STTRINFO trInfo;
	long lCnt = m_arryQuery.GetSize();
	CString sTemp;
	
	for(long lPos=0;lPos<lCnt;lPos++)
	{
		trInfo = m_arryQuery.GetAt(lPos);
		if(trInfo.m_strTrCode==strTRCode)
		{
			
			pWndRequested = trInfo.m_pWndRequested;
			m_arryQuery.RemoveAt(lPos);

			lCnt = m_arryQuery.GetSize();
			break;
		}
	}
	
	if(!m_stTrInfo2.m_pWndRequested && pWndRequested ) //tour2k
	{
		m_stTrInfo2.m_pWndRequested = pWndRequested;
		LPCSTR szCode = (LPCSTR)pWndRequested->SendMessage(RMSG_SELCHARTCODE);
		ConvertKoscomData(strTRCode, aTRData, dwTRDateLen, szCode);
		if(m_stTrInfo2.m_nChartCount)
			return 0L;
	}
	else if(!pWndRequested)
	{
		pWndRequested = m_stTrInfo2.m_pWndRequested;
	}

	if(pWndRequested)
	{
		//2007.04.25 by LYH ���� �ڵ�(���� CodeCombo �ڵ�� ���ؼ� �ٸ� ��� ����ȸ)
//080520-001 CodeInput����		if(m_strRecvCode == "" || m_pInputBarDlg->m_ctrlCodeInput.GetDataText() == "" || m_ChartMng.IsInLoop() || m_pInputBarDlg->m_ctrlCodeInput.GetDataText() == m_strRecvCode)
		if(m_strRecvCode == "" || m_ChartMng.IsInLoop())
		{
			if(pWndRequested->GetSafeHwnd())
			{
				int nUnitType = WON_TYPE;
				char chDataType = DAY_DATA_CHART;
				if(m_pShareInputData)
				{
					nUnitType = m_pShareInputData->m_DataInputBar.m_nUnitType;
					chDataType = m_pShareInputData->m_DataInputBar.m_chType;
				}

				int nMarket = CMasterTypeInfo::STOCK_CHART;
				
				if(strTRCode == QUERY_STRJONGMOK)
				{
					nMarket = CMasterTypeInfo::STOCK_CHART;
				}
				else if(strTRCode == QUERY_STRUPJONG)
				{
					nMarket = CMasterTypeInfo::UPJONG_CHART;
				}
				else if(strTRCode == QUERY_STRFUTOPT)
				{
					nMarket = CMasterTypeInfo::FUTOPT_CHART;
				}
				else if(strTRCode == QUERY_STRFOREIGN)
				{
					nMarket = CMasterTypeInfo::FOREIGN_CHART;
				}
				if(dwTRDateLen!=0)
				{
					KB_p0602_OutRec1_EX* pOut1 = (KB_p0602_OutRec1_EX*)aTRData;
					char* szTemp = new char[50];
					// ���ذ�
					::memset(szTemp, 0x00, 50);
					::memcpy(szTemp, pOut1->preprice, sizeof(pOut1->preprice));
					double dPrevPrice = atof(szTemp);

					// Chart Count
					int nChartCount = 0;
					::memset(szTemp, 0x00, 50);
					if(strTRCode == QUERY_STRFOREIGN)
					{
						KB_p0606_OutRec1_EX* pOut2 = (KB_p0606_OutRec1_EX*)aTRData;
						::memcpy(szTemp, pOut2->fcnt, sizeof(pOut2->fcnt));
						nChartCount = atoi(szTemp);
					}
					else if(strTRCode == QUERY_STRFUTOPT)
					{
						KB_p0604_OutRec1_EX* pOut2 = (KB_p0604_OutRec1_EX*)aTRData;
						::memcpy(szTemp, pOut2->fcnt, sizeof(pOut2->fcnt));
						nChartCount = atoi(szTemp);
					}
					else
					{
						::memcpy(szTemp, pOut1->fcnt, sizeof(pOut1->fcnt));
						nChartCount = atoi(szTemp);
					}

					CString strChartData = MakeChartData(aTRData, nChartCount, dwTRDateLen, nMarket, chDataType, dPrevPrice, nUnitType);
					m_strOut = strChartData;
					stReceiveFromServer stR;
					stR.m_strTRCode = strTRCode;
					stR.m_pBuffer = (LPVOID)(LPCTSTR)m_strOut;
					stR.m_dwLength = m_strOut.GetLength();

// 					stReceiveFromServer stR;
// 					stR.m_strTRCode = strTRCode;
// 					stR.m_pBuffer = (LPVOID)(LPCTSTR)aTRData; 
// 					stR.m_dwLength = dwTRDateLen; //m_strOut.GetLength();

					pWndRequested->SendMessage(UMSG_RECEIVEFROMSERVER,0,(long)&stR);

					::memset(&m_stTrInfo2, 0x00, sizeof(m_stTrInfo2));
				}
				else
				{
					AfxMessageBox("��ȸ ����Ÿ 0");
				}

			}
			m_strRecvCode = "";
		}
		else
		{
//080520-001 CodeInput����			m_pInputBarDlg->SetKeyCodeChange(NID_CODEINPUT, (LPARAM)(LPCTSTR)m_pInputBarDlg->m_ctrlCodeInput.GetDataText());
		}
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
		if(pWndRequested)
		{
			stReceiveFromServer stR;	
			
			stR.m_strTRCode = szTR;
			stR.m_pBuffer = aTRData;
			stR.m_dwLength = dwTRDateLen;
			if(dwTRDateLen>=12)
			{
				CChartItem *pWndChartItem = NULL;
				char szWndChartItem[18];
				memcpy(szWndChartItem,aTRData,12);
				szWndChartItem[12] = NULL;
				pWndChartItem = (CChartItem *)atol(szWndChartItem);
				if(pWndChartItem->GetSafeHwnd())
				{
					pWndChartItem->SendMessageToInvoke(INVOKEMSG_RECEIVEDATA,(long)&stR, sizeof(stReceiveFromServer));					
				}
			}
		}
	}

	
	BOOL bRet = m_ChartMng.IsToBeReceivedMore();
	if(!m_arryQuery.GetSize()&&!bRet)
	{
		EnableWindowAll();		
	}	

	//@080522-001 ��/������.
	m_pInputBarDlg->ConfirmLineType();m_pInputBarDlg->SendNextTR();

	SendMessage2StatusBar(MSG_SEARCH_END, (LPTSTR)(LPCTSTR)szMessage);
	//EnableWindowAll(TRUE);
	return S_OK;
}

HRESULT	CStdDialog::ReceiveMessage(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage)
{
	CString szTmp;
	szTmp.Format("[%s]%s\n", szMsgCode, szMessage);

	if(strcmp(szMsgCode, "80001")==0)		// Ÿ�Ӿƿ�
	{
		AfxMessageBox(szMessage);
		EnableWindowAll(TRUE);

		m_pInputBarDlg->CurrentTrError();
		m_pInputBarDlg->SendNextTR();
	}
	else if(strcmp(szMsgCode, "99999")==0)	// �ش� �ڷᰡ �����ϴ�.
	{
		EnableWindowAll(TRUE);

		//m_ChartMng.ResetOCX();
		AfxMessageBox("�ش� �ڷᰡ �����ϴ�.");

		m_pInputBarDlg->CurrentTrError();
		m_pInputBarDlg->SendNextTR();
	}
	else
	{
		TRACE("%s", szTmp);
		m_pInputBarDlg->CurrentTrError();
		m_pInputBarDlg->SendNextTR();
	}
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
			m_pExtInputBarDlg->EnableWindow();
		}
		if(m_ChartMng.GetSafeHwnd())
		{
			m_ChartMng.EnableWindow();
		}
		EnableWindow(TRUE);
		KillTimer(1);
//		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
	else
	{EnableWindow(FALSE);
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

	//	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

		SetTimer(1,10000,NULL); // 20sec
	}
}

//����: ���ϵ����ʹ� ���������� ������ �׷��� p0602 ��ſ� p0612�� ����. �������� ����������.
void CStdDialog::DoCheckMin(int nType, LPCTSTR lpData, CString& szReqTr)
{
	if(nType==2)
	{
		KB_p0602_InRec1* pInRec1 = (KB_p0602_InRec1*)&lpData[80];
		if(pInRec1->nowData[0]=='0' && pInRec1->button[0]=='1') szReqTr="d1010";
		return;
	}
	if(nType==3)
	{
		KB_p0603_InRec1* pInRec1 = (KB_p0603_InRec1*)&lpData[80];
		if(pInRec1->nowData[0]=='0' && pInRec1->Sbtngb[0]=='1') szReqTr="d1011";
		return;
	}
	if(nType==4)
	{
		KB_p0604_InRec1* pInRec1 = (KB_p0604_InRec1*)&lpData[80];
		if(pInRec1->nowData[0]=='0' && pInRec1->Sbtngb[0]=='1') szReqTr="d1012";
		return;
	}
	if(nType==5)
	{
		KB_d1013_InRec1* pInRec1 = (KB_d1013_InRec1*)&lpData[80];
		if(pInRec1->nowData[0]=='0' && pInRec1->Sbtngb[0]=='1') szReqTr="d1013";
		return;
	}
	if(nType==6)
	{
		KB_p0606_InRec1* pInRec1 = (KB_p0606_InRec1*)&lpData[80];
		if(pInRec1->nowData[0]=='0' && pInRec1->button[0]=='1') szReqTr="d1014";
		return;
	}
}

// �׽�Ʈ����̸� �������� ���� ������ ���� �������� ���� �����ͷ� ó���Ѵ�.
int CStdDialog::Send2Server(LPCTSTR lpTrCode,LPCTSTR lpData,int nSize,BOOL bWait, int nArrayIndex)
{
	SendMessage2StatusBar(MSG_GOING);	
	EnableWindowAll(FALSE);	

	if(m_TRComm)
	{
		CString szReqTr = lpTrCode;
		if(strcmp(lpTrCode		, "d1010")==0)	DoCheckMin(2, lpData, szReqTr);
		else if(strcmp(lpTrCode	, "d1011")==0)	DoCheckMin(3, lpData, szReqTr);
		else if(strcmp(lpTrCode	, "d1012")==0)	DoCheckMin(4, lpData, szReqTr);
		else if(strcmp(lpTrCode	, "d1013")==0)	DoCheckMin(5, lpData, szReqTr);
		else if(strcmp(lpTrCode	, "d1014")==0)	DoCheckMin(6, lpData, szReqTr);

		::memset(&m_stTrInfo2, 0x00, sizeof(m_stTrInfo2));
		
		m_TRComm->SetTR(szReqTr);
		//ȭ���ȣ�� �����Ѵ�.
		m_TRComm->SetBaseData("PtManager", m_szPTManager);
		m_TRComm->SetBaseData("SCREENCODE", m_strScreenNo);

		return m_TRComm->Send2Server((LPVOID)lpData, nSize, TRKEY_FID_END);
	}
	else
		return 0;
}

void CStdDialog::SetShowExtInputBarDlg(BOOL bShowExtInputBarDlg)
{
	m_bShowExtInputBarDlg = bShowExtInputBarDlg;
//	if(m_pInputBarDlg)
//	{
//		m_pInputBarDlg->SetShowExtInputBarDlg(bShowExtInputBarDlg);
//	}	
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
	m_pLeftBarDlg->SetParent(this);
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
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case UMSG_SEND2SERVER:
		{
			BOOL bWait = wParam;
			stSend2Server *pSt = (stSend2Server *)lParam;

			if( (CString(pSt->m_strTRCode) != QUERY_STRJONGMOK) && //�ֽ�		��Ʈ ����Ÿ
				(CString(pSt->m_strTRCode) != QUERY_STRFUTOPT) && //�����ɼ� ��Ʈ ����Ÿ
				(CString(pSt->m_strTRCode) != QUERY_STRSTOCKFUTOPT) && //�ֽļ����ɼ� ��Ʈ ����Ÿ
				(CString(pSt->m_strTRCode) != QUERY_STRUPJONG) && //����		��Ʈ ����Ÿ
				(CString(pSt->m_strTRCode) != QUERY_STRFOREIGN)//�ؿ����� ��Ʈ ����Ÿ
				//(CString(pSt->m_strTRCode) != QUERY_STRTHEME_GROUP) && //�׸� �׷� ����Ÿ
				//(CString(pSt->m_strTRCode) != QUERY_STRTHEME_ITEM) && //�׸� �׷쳻 ����  ����Ÿ
				//(CString(pSt->m_strTRCode) != QUERY_STOCK_HOGA)		 && //�ֽ� ȣ��
				//(CString(pSt->m_strTRCode) != QUERY_STR_AVGMP)       //���� ��ȸ 
				)
				break;
			if( CString(pSt->m_strTRCode) == QUERY_STRFUTOPT )
			{	
//				S36100_I* p36100 = (S36100_I*)pSt->m_pBuffer;
//				CString sCode;
//				sCode.Format("%*.*s", sizeof(p36100->jmcode), sizeof(p36100->jmcode), p36100->jmcode);
//				sCode.TrimLeft(); sCode.TrimRight();
//				DWORD dw = m_IDrMLib_CDBMgr.GetCodeType(sCode);
//				switch(dw)
//				{ 
//					case CODE_QFUTURE	:			
//					case CODE_QFUTURES	: //�ڽ��� ����				
//					case CODE_QOPTION	:						
//					case CODE_QOPTIONS	: //�ڽ��� �ɼ� 
//						pSt->m_strTRCode = QUERY_STRKQFUTOPT;//�ڽ��ڼ����ɼ� ��Ʈ����Ÿ ��ȸ						
//						break;
//					case CODE_JOPTION	: //�ֽ� �ɼ�
//						pSt->m_strTRCode = QUERY_STRSTOCKOPT;//�ֽĿɼ� ��Ʈ����Ÿ ��ȸ						
//						break;	
//					case CODE_FSTAR		: //��Ÿ ���� 
//					case CODE_FSSPREAD	: //��Ÿ ���� ��������
//						pSt->m_strTRCode = QUERY_STR_FSTAR;
//						break;
//				}		
			}

//			if( ( pSt->m_strTRCode == QUERY_STRMARKET_10450) ||	
//				( pSt->m_strTRCode == QUERY_STRMARKET_10451) ||
//				( pSt->m_strTRCode == QUERY_STRMARKET_10452)
//				)

			int lCnt = m_arryQuery.GetSize();

			STTRINFO trInfo, trCheckInfo;
			trInfo.m_pWndRequested = pSt->m_pWnd;
			trInfo.m_strTrCode = pSt->m_strTRCode;

			for(long lPos=0;lPos<lCnt;lPos++)
			{
				trCheckInfo = m_arryQuery.GetAt(lPos);
			}			
			
			if(pSt->m_nType==0)
			{				
				trInfo.m_nType = 0;
				m_arryQuery.Add(trInfo);
			}
			else
			{
				trInfo.m_nType = pSt->m_nType;
				m_arryQueryInvoke.Add(trInfo);
			}

			if( CString(pSt->m_strTRCode) == QUERY_STRFUTOPT)
			{
				if(((char*)pSt->m_pBuffer)[0] == '2' || ((char*)pSt->m_pBuffer)[0] == '3')
				{
//					pSt->m_strTRCode = QUERY_STROPTION;
					Send2Server(pSt->m_strTRCode,(char*)pSt->m_pBuffer,pSt->m_lLength, bWait, m_arryQueryInvoke.GetSize());
				}
				else
					Send2Server(pSt->m_strTRCode,(char*)pSt->m_pBuffer,pSt->m_lLength, bWait, m_arryQueryInvoke.GetSize());
			}
//			else if(CString(pSt->m_strTRCode) == QUERY_STRCOMMODITY)		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//			{
////				if(((char*)pSt->m_pBuffer)[0] == '2' || ((char*)pSt->m_pBuffer)[0] == '3')
////				{
////					pSt->m_strTRCode = QUERY_STROPTION;
////					Send2Server(pSt->m_strTRCode,(char*)pSt->m_pBuffer,pSt->m_lLength, bWait, m_arryQueryInvoke.GetSize());
////				}
////				else
//				Send2Server(pSt->m_strTRCode,(char*)pSt->m_pBuffer,pSt->m_lLength, bWait, m_arryQueryInvoke.GetSize());
//			}
			else
				Send2Server(pSt->m_strTRCode,(char*)pSt->m_pBuffer,pSt->m_lLength, bWait);
		}
		break;
	case UMSG_SENDSHAREDCODE:
		{
//--> ���� ��ƾ�� ���� LeftBar���� ������ ��ƾ���� �����Ѵ�.
//			SendToChartMngCode((LPCTSTR)wParam);
			CString szTmp;
			LPCSTR szCode = (LPCSTR)wParam;
			
			szTmp.Format("%s|%s", GetItemNameFromItemCode(szCode), szCode);
			m_pLeftBarDlg->SendMessage(UMSG_ADDITEM, 0, (LPARAM)(LPCSTR)&szTmp);
//<--
		}		
		break;
	case UMSG_SENDSYNCCODE:
		{
			//m_xGSharedCli.SetData("�ڵ�-����ȭ",(LPCTSTR)wParam);
		}		
		break;
	case UMSG_SENDSHARETIME:
		{
			m_ChartMng.SetSharedTime((LPCTSTR)wParam);
		}		
		break;
	case UMSG_EDITCODE_INIT:
		{
			if(m_pInputBarDlg)
				m_pInputBarDlg->SetCodeInit();
		}
		break;
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
//@��������
//					if(m_pInputBarDlg->m_nMarketType != CMasterTypeInfo::ELW_CHART)
						return 1L;
//					break;
//@��������
				case 100:
					return 1L;
//@��������
////				case 13:	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//				case 14:
//				case 15:
//				case 16:
////				case 17:
////				case 18:
////				case 19:
//				case 21:
//				case 61:
//				case 62:
//					if(m_pInputBarDlg->m_nMarketType != COMMODITY_CHART)
						return 1L;
//					break;
//@��������
				default:
					return 1L;
				}

				//STINPUTCODEDATA stInputCodeData;
				//stInputCodeData.m_strCode = CString((LPCTSTR)wParam);
				//stInputCodeData.m_strTime = "";
				//m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG,NULL,(LPARAM)&stInputCodeData);

				// �����ڵ�� RQ�� ã�� ���� : �������� - ojtaso (20070705)
				if(!m_ChartMng.FindSelectChartByItemCode((LPCTSTR)wParam))
				{
					m_pInputBarDlg->SetMode(0);
//080520-001 CodeInput����					m_pInputBarDlg->SetShareCodeChanged(TRUE);
//080520-001 CodeInput����					m_pInputBarDlg->m_ctrlCodeInput.SetDataText((LPCTSTR)wParam);
				}
			}

			//ChangedGlobalData("�ڵ�-�巡��;",strCode);
			//m_xGSharedCli.SetData("�ڵ�-����2;",(LPCTSTR)lParam);
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
//@��������
					case 3:
//						if(m_pInputBarDlg->m_nMarketType != ELW_CHART)
							return 1L;
//						break;
//@��������
					case 100:
						return 1L;
//@��������
////					case 13:	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//					case 14:
//					case 15:
//					case 16:
////					case 17:
////					case 18:
////					case 19:
//					case 21:
//					case 61:
//					case 62:
//						if(m_pInputBarDlg->m_nMarketType != COMMODITY_CHART)
							return 1L;
//						break;
//@��������
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
			//ChangedGlobalData("�ڵ�-�巡��;",strCode);
			//m_xGSharedCli.SetData("�ڵ�-����2;",(LPCTSTR)lParam);
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
			//m_bShowDataListWnd = (BOOL)lParam;
			m_bShowDataListWnd = !m_bShowDataListWnd;
			Resize();
			CRect ViewRect(0,0,0,0);
			ViewRect = GetRect(ID_CHART);
			ShowDataListView(m_bShowDataListWnd, ViewRect);
		}
		break;
	case USMG_PLEASE_RESIZE:
		{
			//OutputDebugString("Floating ����Ƴ�1!");			
		}
		break;	
	case UMSG_SETMATRIX:
		{
	
		}
		break;

// 	case UMSG_SENDTR_HOGA_DATA:
// 		{		
// 			CString* pStrCode, sCode;
// 			pStrCode = (CString*)wParam;
// 			sCode.Format("%s", pStrCode->GetBuffer(pStrCode->GetLength()));
// 			Request_StockHogaData(sCode);
// 		}
// 		break;
// 	case UMSG_DISCONNECT_HOGA_REAL:
// 		{
// 			Request_Disconnect_StockRealHogaData(_T(""));
// 		}
// 		break;
	case UMSG_GET_CHARTMODE:
		{
			// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
			if(m_pInputBarDlg)
			{
				return m_pInputBarDlg->m_nCurrentMode;
			}
		}
		break;
	//<<<< End ======================================================================
	case UMSG_STOP_JIPYOCYCLE:
		{
			if(m_pLeftBarDlg)
			{
				m_pLeftBarDlg->SendMessage(message, 0, 0);
			}			
		}
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
		//2007.01.17 add by LYH  ���ٹ�ư ����ȭ
	case WMU_GET_KEY_INFO_ARRAY:
		if(m_pInputBarDlg)
		{
			//m_pInputBarDlg->m_ctrlCodeInput.OnGetKeyInfoArray(wParam, lParam);
		}
		break;
	case UMSG_RESETCHART:
		{
			if( m_bOnDestroy) break;
			if(m_ChartMng.GetSafeHwnd())
				m_ChartMng.SendMessage(UMSG_RESETCHART, 0, 0);
		}
		break;
	case UMSG_INIT_COMPARECHART:
		{
			//m_arryRecvTRData.RemoveAll();
			//if(m_pCompToolBarDlg->GetSafeHwnd())	m_pCompToolBarDlg->Init_Controls();
			if( m_bOnDestroy) break;
			if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
				m_pInputBarDlg->Init_Controls();

			if(m_ChartMng.GetSafeHwnd())
				m_ChartMng.SendMessage(UMSG_RESETOCX, 0, 0);				
		}
		break;
	case UMSG_INPUTCODETOINPUTBAR:
		{
			GetInputBarInput();

			if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
				return m_pInputBarDlg->SendMessage(UMSG_INPUTCODETOINPUTBAR, wParam, lParam);

			return 0;
		}
	case UMSG_INITIAL_BTNCLICK:
		InitialBtnClick();
		return 0;

//	case UMSG_RE_REQUEST_TRDATA:
//		GetRequestList(&m_szRCodeList, &m_szRCodeNameList);
//		SendMessage(UMSG_RESETCHART);
//		SetTimer(3, 10, NULL);
//		return 0L;
	}
	return CExOCXDLL::WindowProc(message, wParam, lParam);
}

void CStdDialog::InitDrds()
{
	m_nDrdsInfoCnt = MAXPACKET;//7;
	
	m_DrdsInfo[0].DataName = NAME_KXS3;	// �ֽ� (KXS3)
	m_DrdsInfo[0].FieldName = "*";
	m_DrdsInfo[0].KeyName = "";
	m_DrdsInfo[0].nID = 0;
	m_DrdsInfo[0].bAdvise = FALSE;

	m_DrdsInfo[1].DataName = NAME_IXIJ;	// ���� (IXIJ)
	m_DrdsInfo[1].FieldName = "*";
	m_DrdsInfo[1].KeyName = "";
	m_DrdsInfo[1].nID = 0;
	m_DrdsInfo[1].bAdvise = FALSE;

	m_DrdsInfo[2].DataName = NAME_FFC0;	// ���� (FFC0)
	m_DrdsInfo[2].FieldName = "*";
	m_DrdsInfo[2].KeyName = "";
	m_DrdsInfo[2].nID = 0;
	m_DrdsInfo[2].bAdvise = FALSE;

	m_DrdsInfo[3].DataName = NAME_OOC0;	// �ɼ� (OOC0)
	m_DrdsInfo[3].FieldName = "*";
	m_DrdsInfo[3].KeyName = "";
	m_DrdsInfo[3].nID = 0;
	m_DrdsInfo[3].bAdvise = FALSE;

	m_DrdsInfo[4].DataName = NAME_JFC0;	// �ֽļ��� (JFC0)
	m_DrdsInfo[4].FieldName = "*";
	m_DrdsInfo[4].KeyName = "";
	m_DrdsInfo[4].nID = 0;
	m_DrdsInfo[4].bAdvise = FALSE;

	m_DrdsInfo[5].DataName = NAME_JOC0;	// �ֽĿɼ� (JOC0)
	m_DrdsInfo[5].FieldName = "*";
	m_DrdsInfo[5].KeyName = "";
	m_DrdsInfo[5].nID = 0;
	m_DrdsInfo[5].bAdvise = FALSE;

	m_DrdsInfo[6].DataName = "";	// �ؿ� ()
	m_DrdsInfo[6].FieldName = "*";
	m_DrdsInfo[6].KeyName = "";
	m_DrdsInfo[6].nID = 0;
	m_DrdsInfo[6].bAdvise = FALSE;

	//if(GetDrdsLib() != NULL)
	//	GetDrdsLib()->DrdsInit(m_hWnd);

}

BOOL CStdDialog::MakeDrdsConnectAdviseFromCode(int nI, BOOL bAddMod)
{	
	char* DataName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].DataName;
	char* FieldName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].FieldName;
	char* KeyName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].KeyName;

	UINT	nID = m_DrdsInfo[nI].nID;
	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
	if(!pDrdsMng) return FALSE;

	if(m_pDrdsLib == NULL)
		m_pDrdsLib = pDrdsMng->AdviseDrdsLib();

	if(!m_pDrdsLib) return FALSE;

/*
	CString strType("1");
	ST_CODEDATA oneCode;
	if(nI == 1)//�����̸�
	{
		if(m_DrdsInfo[nI].KeyName.GetLength() == UPJONG_LENGTH)
		{
			oneCode.m_bType = atoi(m_DrdsInfo[nI].KeyName.Left(1));
			oneCode.m_strCode = m_DrdsInfo[nI].KeyName;
		}
	}
	else
	{
		oneCode.m_strCode = m_DrdsInfo[nI].KeyName;
		if(m_pInputBarDlg->m_pDataManager)
			strType = m_pInputBarDlg->m_pDataManager->GetCodeInfo("", 2, oneCode.m_strCode);
		oneCode.m_bType = atoi(strType);
	}
*/
	
	// �������� ������ : �������� - ojtaso (20070208)
	//if(bAddMod)	m_RealTrCounter.AddTr(oneCode.m_strCode);

	m_pDrdsLib->DrdsInit(m_hWnd);
	m_pDrdsLib->DrdsAdvise(nID, DataName, FieldName, KeyName);

	//@Solomon
	if(m_pPTManager) m_pPTManager->RequestMChartReal('A', DataName, KeyName);

	m_DrdsInfo[nI].bAdvise = TRUE;

	return TRUE;
}

// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
/*
BOOL CStdDialog::MakeDrdsConnectAdviseFromCode(int nI, BOOL bAddMod)
{	
	char* DataName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].DataName;
	char* FieldName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].FieldName;
	char* KeyName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].KeyName;

	UINT	nID = m_DrdsInfo[nI].nID;

	//IDrdsLib* m_pDrdsLib = GetDrdsLib();
	//m_pDrdsLib->DrdsConnect(DataName);
	//m_pDrdsLib->DrdsAdvise(nID, DataName,  FieldName, KeyName);

	m_DrdsInfo[nI].bAdvise = TRUE;
	
	//{{2007.01.02 by LYH ������
	CString strType = "1";
	ST_CODEDATA oneCode;
	if(nI == 1)//�����̸�
	{
		if(m_DrdsInfo[nI].KeyName.GetLength() == UPJONG_LENGTH)
		{
			oneCode.m_bType = atoi(m_DrdsInfo[nI].KeyName.Left(1));
			oneCode.m_strCode = m_DrdsInfo[nI].KeyName.Mid(1);
		}
	}
	else
	{
		oneCode.m_strCode = m_DrdsInfo[nI].KeyName;
		if(m_pInputBarDlg->m_pDataManager)
			strType = m_pInputBarDlg->m_pDataManager->GetCodeInfo("", 2, oneCode.m_strCode);
		oneCode.m_bType = atoi(strType);
	}
	
	if(nI == 0)
		m_ListStockRealCodes.AddTail(oneCode);
	else if(nI == 1)
		m_ListUpjongRealCodes.AddTail(oneCode);
	else if(nI == 2 || nI == 3)
		m_ListFutOptRealCodes.AddTail(oneCode);
	else if(nI == 5)
		m_ListForeignRealCodes.AddTail(oneCode);

	// �������� ������ : �������� - ojtaso (20070208)
//@��������	if(bAddMod)
//@��������		m_RealTrCounter.AddTr(oneCode.m_strCode);

	AddRealCodes("20000", &m_ListStockRealCodes);
	AddRealCodes("20001", &m_ListUpjongRealCodes);
	AddRealCodes("20002", &m_ListFutOptRealCodes);
	AddRealCodes("20003", &m_ListForeignRealCodes);
	//}}
	return TRUE;
}
*/

// �������� �ҷ����� : �������� - ojtaso (20070514)
void CStdDialog::AddRealCodes()
{
//@��������
//	AddRealCodes("20000", &m_ListStockRealCodes);
//	AddRealCodes("20001", &m_ListUpjongRealCodes);
//	AddRealCodes("20002", &m_ListFutOptRealCodes);
//	AddRealCodes("20003", &m_ListForeignRealCodes);
//@��������
//@�����߰�
//@�����߰�
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
void CStdDialog::AddRealCodeList(LPCTSTR lpszCode)
{
/*
	int nLength = ::lstrlen(lpszCode);
	CString strType;

	ST_CODEDATA oneCode;
	oneCode.m_strCode = lpszCode;

	strType = m_pInputBarDlg->m_pDataManager->GetCodeInfo("", 2, lpszCode);
	if(strType == "79"|| strType == "80" || strType == "81" || strType == "82" || strType == "83" || strType == "84" || strType == "85" || strType == "86")
	{
		oneCode.m_bType = atoi(strType);
		m_ListForeignRealCodes.AddTail(oneCode);
	}
	else
	{
		switch(nLength)
		{
		case CODE_STOCK_LENGTH:
//			if(m_pInputBarDlg->m_pDataManager)
//				strType = m_pInputBarDlg->m_pDataManager->GetCodeInfo("", 2, oneCode.m_strCode);
			oneCode.m_bType = atoi(strType);

//			m_ListStockRealCodes.AddTail(oneCode);
			break;
		case UPJONG_LENGTH:
			oneCode.m_strCode = lpszCode;
			oneCode.m_bType = atoi(oneCode.m_strCode.Left(1));
			oneCode.m_strCode = oneCode.m_strCode.Mid(1);

//			m_ListUpjongRealCodes.AddTail(oneCode);
			break;
		case FUTOPT_LENGTH:
//			if(m_pInputBarDlg->m_pDataManager)
//				strType = m_pInputBarDlg->m_pDataManager->GetCodeInfo("", 2, oneCode.m_strCode);
			oneCode.m_bType = atoi(strType);
			m_ListFutOptRealCodes.AddTail(oneCode);
			break;
		}
	}
*/

	//m_RealTrCounter.AddTr(lpszCode);
}

// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
/*
BOOL CStdDialog::MakeDrdsUnConnectUnAdviseFromCode(int nI, BOOL bAddMod)
{
	//IDrdsLib* m_pDrdsLib = GetDrdsLib();
	//if(m_pDrdsLib) {
		if(!m_DrdsInfo[nI].bAdvise)
			return TRUE;

		char* DataName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].DataName;
		char* FieldName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].FieldName;
		char* KeyName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].KeyName;
		UINT	nID = m_DrdsInfo[nI].nID;
		
//		m_pDrdsLib->DrdsUnadvise(nID,
//			DataName, 
//			FieldName, 
//			KeyName);
//		m_pDrdsLib->DrdsClose(DataName);		
		
		m_DrdsInfo[nI].bAdvise = FALSE;
	//}

	//{{2007.01.02 by LYH ������
	if(nI == 0)
		RemoveRealCodes(&m_ListStockRealCodes, m_DrdsInfo[nI].KeyName);
	else if(nI == 1)
		RemoveRealCodes(&m_ListUpjongRealCodes, m_DrdsInfo[nI].KeyName);
	else if(nI == 2 || nI == 3)
		RemoveRealCodes(&m_ListFutOptRealCodes, m_DrdsInfo[nI].KeyName);
	else if(nI == 5)
		RemoveRealCodes(&m_ListForeignRealCodes, m_DrdsInfo[nI].KeyName);
	//}}

//@��������
//	// �������� ������ : �������� - ojtaso (20070208)
//	if(bAddMod)
//	{
//		if(!m_RealTrCounter.RemoveTr(m_DrdsInfo[nI].KeyName))
//			m_TRComm->ResetRealTR(m_DrdsInfo[nI].KeyName);
//	}
//@��������

	return TRUE;
}
*/

BOOL CStdDialog::MakeDrdsUnConnectUnAdviseFromCode(int nI, BOOL bAddMod)
{
	if(!m_pDrdsLib) return FALSE;

	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
	if(!pDrdsMng) return FALSE;

	if(!m_DrdsInfo[nI].bAdvise && m_pDrdsLib == NULL)
		return TRUE;

	char* DataName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].DataName;
	char* FieldName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].FieldName;
	char* KeyName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].KeyName;
	UINT nID = m_DrdsInfo[nI].nID;

	m_pDrdsLib->DrdsUnadvise(nID, DataName, FieldName, KeyName);

	//@Solomon
	if(m_pPTManager) m_pPTManager->RequestMChartReal('U', DataName, KeyName);

	pDrdsMng->UnadviseDrdsLib(m_pDrdsLib);
	m_pDrdsLib = NULL;
	m_DrdsInfo[nI].bAdvise = FALSE;

	// �������� ������ : �������� - ojtaso (20070208)
	//if(bAddMod)		m_RealTrCounter.RemoveTr(m_DrdsInfo[nI].KeyName);

	return TRUE;
}

// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
// void CStdDialog::OnCodeUnAdvise(CString strCode, BOOL bAddMode)
// {
// 	// ���� �ڵ尡 ����Ʈ�� ������ ����
// 	// �������� ������ : �������� - ojtaso (20070208)
// 	CString strType = m_pInputBarDlg->m_pDataManager->GetCodeInfo("", 2, strCode);
// 	if(strType == "79"|| strType == "80" || strType == "81" || strType == "82" || strType == "83" || strType == "84" || strType == "85" || strType == "86")
// 	{
// 		m_DrdsInfo[5].KeyName = strCode;
// 		MakeDrdsUnConnectUnAdviseFromCode(5, bAddMode);								
// 		// �������� ������ : �������� - ojtaso (20070208)
// 	}
// 	else if(strCode.GetLength()==CODE_STOCK_LENGTH)
// 	{
// 		m_DrdsInfo[0].KeyName = strCode;
// 		MakeDrdsUnConnectUnAdviseFromCode(0, bAddMode);								
// 		// �������� ������ : �������� - ojtaso (20070208)
// 	}
// 	else if(strCode.GetLength()==UPJONG_LENGTH)
// 	{
// 		m_DrdsInfo[1].KeyName = strCode;
// 		MakeDrdsUnConnectUnAdviseFromCode(1, bAddMode);
// 		// �������� ������ : �������� - ojtaso (20070208)
// 	}
// 	else if(strCode.GetLength()==FUTOPT_LENGTH)
// 	{
// 		if(strCode[0]=='1'||strCode[0]=='4')
// 		{
// 			m_DrdsInfo[2].KeyName = strCode;
// 			MakeDrdsUnConnectUnAdviseFromCode(2, bAddMode);								
// 			// �������� ������ : �������� - ojtaso (20070208)
// 		}
// 		else
// 		{
// 			m_DrdsInfo[3].KeyName = strCode;
// 			MakeDrdsUnConnectUnAdviseFromCode(3, bAddMode);								
// 			// �������� ������ : �������� - ojtaso (20070208)
// 		}
// 	}
// }

void CStdDialog::OnCodeUnAdvise(CString strCode, BOOL bAddMode)
{
	// ���� �ڵ尡 ����Ʈ�� ������ ����
	// �������� ������ : �������� - ojtaso (20070208)
	//CString strType = m_pInputBarDlg->m_pDataManager->GetCodeInfo("", 2, strCode);
	int nMarketType = m_pInputBarDlg->m_pDataManager->GetMarketTypeFromCode(strCode);
	//if(strType == "79"|| strType == "80" || strType == "81" || strType == "82" || strType == "83" || strType == "84" || strType == "85" || strType == "86")
	if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
		m_DrdsInfo[5].KeyName = strCode;
		MakeDrdsUnConnectUnAdviseFromCode(5, bAddMode);								
		// �������� ������ : �������� - ojtaso (20070208)
	}
	//else if(strCode.GetLength()==CODE_STOCK_LENGTH)
	else if(nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
		m_DrdsInfo[0].KeyName = strCode;
		MakeDrdsUnConnectUnAdviseFromCode(0, bAddMode);								
		// �������� ������ : �������� - ojtaso (20070208)
	}
	//else if(strCode.GetLength()==UPJONG_LENGTH)
	else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		m_DrdsInfo[1].KeyName = strCode;
		MakeDrdsUnConnectUnAdviseFromCode(1, bAddMode);
		// �������� ������ : �������� - ojtaso (20070208)
	}
	//else if(strCode.GetLength()==FUTOPT_LENGTH)
	else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
	{
		// ���ἱ���� ��� �ֱٿ����� ����
		if(strCode.CompareNoCase(DEF_CONTINUE_FUT_CODE) == 0 || strCode.CompareNoCase(DEF_CONTINUE_MINI_FUT_CODE) == 0 )
			IsContinueFutCode(strCode);

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
}

// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
// void CStdDialog::OnCodeAdvise(CString strCode, BOOL bAddMode)
// {
// 	// Drds ���� 
// 	// ���� �ڵ尡 ����Ʈ�� ������ �߰�
// 	// �������� ������ : �������� - ojtaso (20070208)
// 	CString strType = m_pInputBarDlg->m_pDataManager->GetCodeInfo("", 2, strCode);
// 	if(strType == "79"|| strType == "80" || strType == "81" || strType == "82" || strType == "83" || strType == "84" || strType == "85" || strType == "86")
// 	{
// 		m_DrdsInfo[5].KeyName = strCode;
// 		MakeDrdsConnectAdviseFromCode(5, bAddMode);
// 	}
// 	else if(strCode.GetLength()==CODE_STOCK_LENGTH)
// 	{
// 		m_DrdsInfo[0].KeyName = strCode;
// 		MakeDrdsConnectAdviseFromCode(0, bAddMode);
// 	}
// 	else if(strCode.GetLength()==UPJONG_LENGTH)
// 	{
// 		m_DrdsInfo[1].KeyName = strCode;
// 		MakeDrdsConnectAdviseFromCode(1, bAddMode);
// 	}
// 	else if(strCode.GetLength()==FUTOPT_LENGTH)
// 	{
// 		if(strCode[0]=='1'||strCode[0]=='4')
// 		{
// 			m_DrdsInfo[2].KeyName = strCode;
// 			MakeDrdsConnectAdviseFromCode(2, bAddMode);
// 		}
// 		else
// 		{
// 			m_DrdsInfo[3].KeyName = strCode;
// 			MakeDrdsConnectAdviseFromCode(3, bAddMode);
// 		}
// 	}
// }

void CStdDialog::OnCodeAdvise(CString strCode, BOOL bAddMode)
{
	// Drds ���� 
	// ���� �ڵ尡 ����Ʈ�� ������ �߰�
	// �������� ������ : �������� - ojtaso (20070208)
	//CString strType = m_pInputBarDlg->m_pDataManager->GetCodeInfo("", 2, strCode);
	int nMarketType = m_pInputBarDlg->m_pDataManager->GetMarketTypeFromCode(strCode);
	//if(strType == "79"|| strType == "80" || strType == "81" || strType == "82" || strType == "83" || strType == "84" || strType == "85" || strType == "86")
	if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
		m_DrdsInfo[5].KeyName = strCode;
		MakeDrdsConnectAdviseFromCode(5, bAddMode);
	}
	//else if(strCode.GetLength()==CODE_STOCK_LENGTH)
	else if(nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
		m_DrdsInfo[0].KeyName = strCode;
		MakeDrdsConnectAdviseFromCode(0, bAddMode);
	}
	//else if(strCode.GetLength()==UPJONG_LENGTH)
	else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		m_DrdsInfo[1].KeyName = strCode;
		MakeDrdsConnectAdviseFromCode(1, bAddMode);
	}
	//else if(strCode.GetLength()==FUTOPT_LENGTH)
	else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
	{
		// ���ἱ���� ��� �ֱٿ����� ����
		if(strCode.CompareNoCase(DEF_CONTINUE_FUT_CODE) == 0 || strCode.CompareNoCase(DEF_CONTINUE_MINI_FUT_CODE) == 0 )
			IsContinueFutCode(strCode);

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
}

BOOL CStdDialog::IsContinueFutCode(CString& strCode)
{
	CStringArray arCode, arName;	
	if(strCode.GetLength()<8) return FALSE;
	
	CString szTmpCode = strCode.Mid(3);
	if(szTmpCode.Compare("99999") != 0) return FALSE;
	
	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);		
	if(pMasterDataManager)
	{
		CString strFOCode;
		BOOL bRet = pMasterDataManager->GetRecentCode(strCode, strFOCode, 10);
		strCode = strFOCode;
		return bRet;
	}
	
	return FALSE;
}

/* @��������
LONG CStdDialog::OnDrdsData(UINT wParam, LPARAM lParam)
{
	LPCSTR pData = (LPSTR)lParam; //(LPCSTR)lParam;
	int len = (int)wParam;
	if(len > 3)   
	{
		DrdsData* pmyDData = (DrdsData*)pData;		
		if(memcmp(pmyDData->Name, "S31", 3) == 0)			// �������簡
		{
			//if( SZ_PriceData > pmyDData->Len )
			//{
			//	AfxMessageBox("�������簡 �̻� Data Recv");
			//	return 0L;			
			//}
			if(pmyDData->Id == 200)
			{
				//m_pLeftBarDlg->SetRealPriceData(pmyDData->Data, pmyDData->Len);
			}
			else
			{
				//m_ChartMng.SetRealJPriceData(pmyDData->Data, pmyDData->Len);
				if(	(m_ChartMng.m_pFloatingDlg != NULL)		&&
					m_ChartMng.m_pFloatingDlg->GetSafeHwnd()	
				)	
				{
					m_ChartMng.m_pFloatingDlg->SetMarkRealCurPrice(pmyDData->Data, pmyDData->Len);
				}
			}
			return 1L;
		}
		else if(memcmp(pmyDData->Name, "JS0", 3) == 0)			// �������簡
		{
			//if( SZ_JS0_OUT > pmyDData->Len )
			//{
			//	AfxMessageBox("�������簡 �̻� Data Recv");
			//	return 0L;			
			//}
//			if(pmyDData->Key)
//			m_pLeftBarDlg->SetRealPriceData(pmyDData->Data, pmyDData->Len);
			m_ChartMng.SetRealUPriceData(pmyDData->Data, pmyDData->Len);
			return 1L;
		}
		else if(memcmp(pmyDData->Name, "SC0", 3) == 0)			// �������簡
		{
			//if( SZ_SC0_OUT > pmyDData->Len )
			//{
			//	AfxMessageBox("�������簡 �̻� Data Recv");
			//	return 0L;			
			//}
//			if(pmyDData->Key)
//			m_pLeftBarDlg->SetRealPriceData(pmyDData->Data, pmyDData->Len);
			m_ChartMng.SetRealFPriceData(pmyDData->Data, pmyDData->Len);
			return 1L;
		}
		else if(memcmp(pmyDData->Name, "OC0", 3) == 0)			// �������簡
		{
			//if( SZ_OC0_OUT > pmyDData->Len )
			//{
			//	AfxMessageBox("�ɼ����簡 �̻� Data Recv");
			//	return 0L;			
			//}
//			if(pmyDData->Key)
//			m_pLeftBarDlg->SetRealPriceData(pmyDData->Data, pmyDData->Len);
			m_ChartMng.SetRealOPriceData(pmyDData->Data, pmyDData->Len);
			return 1L;
		}
		else if(memcmp(pmyDData->Name, "X11", 3) == 0)			// �ֽ�ȣ��
		{
			//CString sCode;
			//sCode.Format("%7.7s", RealData.shcode); sCode.TrimLeft(); sCode.TrimRight();
			//if( sCode.CompareNoCase( m_ChartMng.m_pFloatingDlg->m_strCode) == 0)
			//{
//				m_ChartMng.m_pFloatingDlg->SetRealHogaData(pmyDData->Data, pmyDData->Len);
			//}
		}


	}
	//CRealData *pData = (CRealData *)lParam;
//	CDrMLib_CRealData RealData((void*)lParam);
//	CString strData, strTmp;	
//	if( RealData.wRecordID == PID_JONGCHEGYUL)			// �������簡
//	{	
//		STPriceData RealData;
//		//CDataUtil::ConvertToDRStruct(pData, (char *)&RealData);		
//		//int nValue = ConvertToChartSign((char*)RealData.sign);
//		CDrMLib_CDataUtil::ConvertToDRStruct((void*)lParam, (char *)&RealData);
//		
//		
//		m_ChartMng.SetRealJPriceData((char *)&RealData, sizeof(RealData));		
//		if(	(m_ChartMng.m_pFloatingDlg != NULL)		&&
//			m_ChartMng.m_pFloatingDlg->GetSafeHwnd()	
//		)	
//		{
//			m_ChartMng.m_pFloatingDlg->SetMarkRealCurPrice((char *)&RealData, sizeof(RealData));
//		}
//		return 1L;
//	}
//	else if( RealData.wRecordID == PID_JISUSISE)			// �������簡
//	{		
//		st_JS0 RealData;
//		//CDataUtil::ConvertToDRStruct(pData, (char *)&RealData);	
//		CDrMLib_CDataUtil::ConvertToDRStruct((void*)lParam, (char *)&RealData);
//
//		m_ChartMng.SetRealUPriceData((char *)&RealData, sizeof(RealData));
//		return 1L;
//	}
//	else if(	( RealData.wRecordID == PID_FUTURESISE	)	||	// �������簡
//				( RealData.wRecordID == PID_QFUTURESISE	)	||	// KOSDAQ50 ���� ���簡	
//				(RealData.wRecordID == PID_FSTARSISE	)		// ��Ÿ���� ����
//	)
//	{	
//		st_SC0 RealData;
//		//CDataUtil::ConvertToDRStruct(pData, (char *)&RealData);	
//		CDrMLib_CDataUtil::ConvertToDRStruct((void*)lParam, (char *)&RealData);
//		m_ChartMng.SetRealFPriceData((char *)&RealData, sizeof(RealData));
//		return 1L;
//	}
//	else if(	(RealData.wRecordID == PID_OPTIONSISE	)	||	// �ɼ� ���簡
//				(RealData.wRecordID == PID_QOPTIONSISE	)	||	// �ڽ��� �ɼ�
//				(RealData.wRecordID == PID_JOPTIONSISE	)		// �ֽ�   �ɼ�
//				
//	)
//	{	
//		st_OC0 RealData;
//		//CDataUtil::ConvertToDRStruct(pData, (char *)&RealData);	
//		CDrMLib_CDataUtil::ConvertToDRStruct((void*)lParam, (char *)&RealData);
//		m_ChartMng.SetRealOPriceData((char *)&RealData, sizeof(RealData));
//		return 1L;
//	}
//	else if( RealData.wRecordID == PID_JONGHOGA_10)
//	{
//		STHogaData RealData;
//		CDrMLib_CDataUtil::ConvertToDRStruct((void*)lParam, (char *)&RealData);
//		if(m_ChartMng.m_pFloatingDlg != NULL)
//		{
//			if(m_ChartMng.m_pFloatingDlg->GetSafeHwnd())
//			{
//				CString sCode;
//				sCode.Format("%7.7s", RealData.shcode); sCode.TrimLeft(); sCode.TrimRight();
//				if( sCode.CompareNoCase( m_ChartMng.m_pFloatingDlg->m_strCode) == 0)
//				{
//					m_ChartMng.m_pFloatingDlg->SetRealHogaData((char *)&RealData, sizeof(RealData));
//				}
//			}
//		}
//	}

/* === G.Zone =======================================
	LPCSTR pData = (LPSTR)lParam; //(LPCSTR)lParam;
	int len = (int)wParam;
	if(len > 3)   
	{
		DrdsData* pmyDData = (DrdsData*)pData;		
		if(memcmp(pmyDData->Name, "S31", 3) == 0)			// �������簡
		{
			if( SZ_PriceData > pmyDData->Len )
			{
				AfxMessageBox("�������簡 �̻� Data Recv");
				return 0L;			
			}
//			m_pLeftBarDlg->SetRealPriceData(pmyDData->Data, pmyDData->Len);
			m_ChartMng.SetRealJPriceData(pmyDData->Data, pmyDData->Len);
			return 1L;
		}
		else if(memcmp(pmyDData->Name, "JS0", 3) == 0)			// �������簡
		{
			if( SZ_JS0_OUT > pmyDData->Len )
			{
				AfxMessageBox("�������簡 �̻� Data Recv");
				return 0L;			
			}
//			if(pmyDData->Key)
//			m_pLeftBarDlg->SetRealPriceData(pmyDData->Data, pmyDData->Len);
			m_ChartMng.SetRealUPriceData(pmyDData->Data, pmyDData->Len);
			return 1L;
		}
		else if(memcmp(pmyDData->Name, "SC2", 3) == 0)			// �������簡
		{
			if( SZ_SC2_OUT > pmyDData->Len )
			{
				AfxMessageBox("�������簡 �̻� Data Recv");
				return 0L;			
			}
//			if(pmyDData->Key)
//			m_pLeftBarDlg->SetRealPriceData(pmyDData->Data, pmyDData->Len);
			m_ChartMng.SetRealFPriceData(pmyDData->Data, pmyDData->Len);
			return 1L;
		}


	}
	return 1L;
}
*/

BOOL CStdDialog::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)  
	{		
//		if(pMsg->wParam == VK_F4 && (::GetKeyState(VK_CONTROL) & 0x8000))
//		{
//			pMsg->wParam = VK_ESCAPE;
//			return GetParent()->PreTranslateMessage(pMsg);
//		}
//		if(pMsg->wParam == VK_F4 && (::GetKeyState(VK_CONTROL) & 0x8000))
//		{
//			pMsg->wParam = VK_ESCAPE;
//			return AfxGetMainWnd()->PreTranslateMessage(pMsg);
//		}
//		else
//			return FALSE;
		UINT nChar = (UINT)pMsg->wParam;
		switch(nChar)
		{
		case VK_RETURN :
		case VK_ESCAPE :
		case VK_TAB:
		case VK_HOME:
		case VK_ADD:
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
	
	//if(::IsWindow(m_hWnd))
	//	return GetParent()->PreTranslateMessage(pMsg);
	//else
		return CExOCXDLL::PreTranslateMessage(pMsg);
}

//void CStdDialog::OnGSharedDataChange(LPCTSTR lpSharedName,LPCTSTR lpGSharedData, BOOL bWithStart)
//{
//	
//	CString strSharedName = lpSharedName;
//	CString strGSharedData = lpGSharedData;
//	strGSharedData.TrimRight();
//	strSharedName.TrimRight(';');
//	if(strGSharedData.IsEmpty()) return;
//	if(strSharedName=="�ð�-�׸���")
//	{
//		CString strTemp = m_ChartMng.GetSharedTime();
//		if(!strTemp.IsEmpty())
//		{
//			m_ChartMng.SetSharedTime(lpGSharedData);
//			return;
//		}		
//	}
//	if(strSharedName==NPRSN_ACCTALL)
//	{
//		m_strAccountInfo = strGSharedData;
//		if(m_pLeftBarDlg)
//		{
//			CWnd *pWndCodeSearch = m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
//			pWndCodeSearch->SendMessage(UMSG_SETACCOUNTINFO,(WPARAM)(LPCTSTR)m_strAccountInfo);;
//		}		
//	}
//
//	if(!m_bEnable&&!m_bFirstGShared) return;
//
//	if(strSharedName=="�ڵ�-����2"||strSharedName=="�ڵ�-����ȭ")
//	{
//		if(m_pInputBarDlg)
//		{
//			STINPUTCODEDATA2	stInputCodeData2;
//			stInputCodeData2.m_strCode = lpGSharedData;
//			stInputCodeData2.m_bDontPutInShared = TRUE;
//			stInputCodeData2.m_bRequestWithoutHesitation = FALSE;
//			m_pInputBarDlg->SendMessage(UMSG_INPUTCODETOINPUTBAR,(WPARAM)&stInputCodeData2);
//		}		
//	}
//	else if(strSharedName=="�ڵ�-�巡��")
//	{
//		STINPUTCODEDATA stInputCodeData;
//		int nPos = strGSharedData.Find(';');
//		if(nPos==-1)
//		{
//			stInputCodeData.m_strCode = strGSharedData;
//		}
//		else
//		{
//			stInputCodeData.m_strCode = strGSharedData.Left(nPos);
//			strGSharedData.Delete(0,nPos+1);
//			strGSharedData.TrimRight(';');
//			stInputCodeData.m_strTime = strGSharedData;
//		}
//		m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG,NULL,(LPARAM)&stInputCodeData);
////		
////		m_pInputBarDlg->SendMessage(UMSG_INPUTCODETOINPUTBAR,NULL,(LPARAM)(LPCTSTR)strGSharedData);
//	}
//	else if(strSharedName=="�ڵ�-�巡�׸���Ʈ")
//	{
//		m_ChartMng.SetMultiCodes(lpGSharedData);
//	}
//	else if(strSharedName==NPRSN_NOTICHART)
//	{
//		m_ChartMng.SendMessage(UMSG_SETORDEREDINFO);		
//	}	
//	else 
//	{
//		STSHAREDDATA stData;
//		stData.m_szKey = (LPCTSTR)strSharedName;
//		stData.m_szValue = (LPCTSTR)lpGSharedData;		
//		if(m_pLeftBarDlg)
//		{
//			m_pLeftBarDlg->SendMessage(UMSG_SETCHARTHOGA,NULL,(LPARAM)&stData);
//		}
//	}
//	m_bFirstGShared = FALSE;
//}

void CStdDialog::OnSize(UINT nType, int cx, int cy) 
{
	CExOCXDLL::OnSize(nType, cx, cy);
	
/*	if(m_hCompareChartBitmap)
	{
		DeleteObject(m_hCompareChartBitmap);
		m_hCompareChartBitmap = NULL;
	}
*/
	
	if(GetSafeHwnd())
	{
		//SetWindowPos(&CWnd::wndTop,0,0,cx,cy,SWP_SHOWWINDOW);	
		Resize();

		// for Invalidate Floating Hoga Dialog - SeoJeong - 2005/08/26
		//if(m_ChartMng.m_pFloatingDlg && m_ChartMng.m_pFloatingDlg->GetSafeHwnd())
		//	PostMessage(UMSG_SHOW_HOGADLG, 0, 0);
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
	// TODO: Add your message handler code here and/or call default
	KillTimer(nIDEvent);

	if(nIDEvent==1)	
	{
		m_arryQuery.RemoveAll();
		EnableWindowAll();
		SendMessage2StatusBar(MSG_SEARCH_FAIL);
	}
	else if(nIDEvent==2)
	{
		SetOneClickHide(TRUE);
	}
//	else if(nIDEvent==3)
//	{
//		RequestNext();
//	}
	
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
//		m_pBaseDesk->InvokeEvent(pWnd->m_hWnd, _T("SendDataFromDll"), csData);
	}
	return m_bHideChart;
}

void	CStdDialog::SetJongListPrev()
{
//	CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
//	if(pTabCodeSearch)
//	{
//		pTabCodeSearch->OnBtprev();
//	}	
}

void	CStdDialog::SetJongListNext()
{
//	CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
//	if(pTabCodeSearch)
//	{
//		pTabCodeSearch->OnBtnext();
//	}	
}

CString CStdDialog::GetAccountInfo() 
{ 
//	return m_strAccountInfo;
	return "";
}

BOOL CStdDialog::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bEnable)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		return TRUE;
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CStdDialog::SetInitInfo()
{
	int nID = 21;
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	if(pGateMng == NULL)
		return;

	IMainInfoManager* pManager = (IMainInfoManager*)pGateMng->GetInterface(nID);

	// UserID
	m_strUserID = pManager->GetEncUserID();
/*
	if(m_strUserID.GetLength() == 0)
	{
		AfxMessageBox("UserID�� ���� �� �����ϴ�.");
	}
*/

	// Root Path
	m_strRootPath = pManager->GetRootDir();
	m_szDataDir = pManager->GetDataDir();
	if(m_strRootPath.GetLength()==0)
	{
		AfxMessageBox("RootPath�� ���� �� �����ϴ�.");
	}

	// User Path 
	//@Solomon:User�� Chart������ �⺻����� ���丮 ����.
	m_strUserDir.Format("%s\\Chart", pManager->GetEncUserDir());

	// Image Path
	//solomon-090825
	//m_szImageDir = m_strRootPath + "\\Image\\DrChart";
	m_szImageDir = m_strRootPath + "\\Icss\\Image\\drchart";

//#ifdef __MULTI_LANG__
	//@Solomon-MultiLang:091117
	g_iMetaTable.m_strDataFolderPath.Format(_T("%s\\"), pManager->GetDataDir());
	g_iMetaTable.LoadMetaTable();
//#endif

}

//#============================== TR ���� Test ==========================
//* ȣ����ȸ
// #define TR_HOGA_FUTURE "HWS10010" 
// void CStdDialog::SendTR_TEST(CString strRecvCode)
// {	
// 	strRecvCode = "10156000";
// 	if(m_TRComm)
// 	{
// 		m_TRComm->SetTR(TR_HOGA_FUTURE);
// 
// 		m_TRComm->Send2Server((LPVOID)(LPCSTR)strRecvCode, strRecvCode.GetLength(), TRKEY_FID_END);
// 	}
// }
//#============================== TR ���� Test ==========================


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


//	CRealChartData *pRealChart = new CRealChartData;
//
//	pRealChart->m_hOCXHwnd = hWnd;	
//	
//	memset(&(pRealChart->m_DrdsData), 0x00, sizeof(DrdsData));
//
//			 pRealChart->m_DrdsData.nID					= id;
//	memcpy(&(pRealChart->m_DrdsData.DataName	),	DataName	, lstrlen(DataName	));		
//	memcpy(&(pRealChart->m_DrdsData.FieldName	),	FieldName	, lstrlen(FieldName	));
//	memcpy(&(pRealChart->m_DrdsData.KeyName		),	KeyName		, lstrlen(KeyName	));			
//	
//	m_lstRegRealChart.AddTail(pRealChart);
//
//#ifdef _DEBUG
//	CString strRecvPacket;
//	strRecvPacket.Format("%s.%s.%s",DataName, FieldName, KeyName);
//	
//	OutputDebugString("Real ���ۿ�û�� ������" + strRecvPacket);
//#endif

}

void CStdDialog::UnSetRealInfo(
									void* pDrds		,   // Drds�������ε� NULL�� ����.
									HWND hWnd		,   // Chart OCX�� ������ �ڵ�
									UINT id			, 	// CPacket* 
									LPSTR DataName	,	// ex: "S31"
									LPSTR FieldName	, 	// ex: "time"
									LPSTR KeyName	)	// ex: "000660"
{
//	CString strRecvPacket;
//	strRecvPacket.Format("%s.%s.%s",DataName, FieldName, KeyName);	
//	POSITION pos = NULL;
//	pos = m_lstRegRealChart.GetHeadPosition();
//	CRealChartData *pRealChart = NULL;
//	while(pos)
//	{
//		pRealChart = NULL;		
//		pRealChart = (CRealChartData*)m_lstRegRealChart.GetAt(pos);
//		if(pRealChart->m_hOCXHwnd == hWnd)
//		{	
//			if(pRealChart->m_DrdsData.nID == id)
//			{
//				delete pRealChart;
//				pRealChart = NULL;
//				m_lstRegRealChart.RemoveAt(pos);			
//				break;
//			}
//		}
//		m_lstRegRealChart.GetNext(pos);
//	}
//	
//	OutputDebugString("Real ���� ��û�� ������" + strRecvPacket);
}


void CStdDialog::SendStockReal(char* pData)
{
/*	STPriceData* Op = (STPriceData*) pData;

	CString strCode, strMemCode, strMemPacket;
	strCode.Format("%*.*s", sizeof(Op->shcode), sizeof(Op->shcode), Op->shcode);
	strCode.Remove(' ');
	
	POSITION pos = m_lstRegRealChart.GetHeadPosition();
	CRealChartData *pRealChart = NULL;
	BOOL bSendRealComplete = FALSE;

	while(pos)
	{		
		pRealChart = NULL;		
		pRealChart = (CRealChartData*)m_lstRegRealChart.GetAt(pos);
		strMemCode	.Format("%s", pRealChart->m_DrdsData.KeyName	);
		strMemPacket.Format("%s", pRealChart->m_DrdsData.DataName	);

		memset(pRealChart->m_DrdsData.Data, 0x00, MAX_DRDS_PACKET_LEN);

		if( (strMemCode == strCode	) && 
			(strMemPacket == "S31"	)
		)
		{
			if( lstrcmp( pRealChart->m_DrdsData.FieldName, "time") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->time) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->time, sizeof(Op->time) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));				
				//SendMessage	( WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));				
				OutputDebugString("..........1");
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "price") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->price) + 3;
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->price, sizeof(Op->price));
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));
				//SendMessage	( WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));
				OutputDebugString("..........2");
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "open") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->open) + 3;
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->open, sizeof(Op->open));
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));
				//SendMessage	( WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));
				OutputDebugString("..........3");
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "high") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->high) + 3;
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->high, sizeof(Op->high));
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));
				//SendMessage	( WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));
				OutputDebugString("..........4");
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "low") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->low) + 3;
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->low, sizeof(Op->low));
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(long)&(pRealChart->m_DrdsData));
				//SendMessage	( WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));
				bSendRealComplete = TRUE;
				OutputDebugString("..........5");
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "volume") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->volume) + 3;
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->volume, sizeof(Op->volume));
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(long)&(pRealChart->m_DrdsData));
				//SendMessage	( WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));
				bSendRealComplete = TRUE;
				OutputDebugString("..........6");
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "chgrate") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->chgrate) + 3;
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->chgrate, sizeof(Op->chgrate));
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(long)&(pRealChart->m_DrdsData));
				//SendMessage	( WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));
				bSendRealComplete = TRUE;
				OutputDebugString("..........7");
			}

			else
			{
				bSendRealComplete = FALSE;
#ifdef _DEBUG
				CString sTemp;
				sTemp.Format("�ֽ� : Real ó���� �ȵǴ� �׸� :%32.32s", pRealChart->m_DrdsData.FieldName);
				AfxMessageBox(sTemp);
#endif
			}

		}					

		m_lstRegRealChart.GetNext(pos);
	}

	if(bSendRealComplete)
		::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA_PACKET_END, 0, 0 );
*/
}

void CStdDialog::SendUpjongReal(char* pData)
{
/*	st_JS0* Op = (st_JS0*) pData;

	CString strCode, strMemCode, strMemPacket;
	strCode.Format("%*.*s", sizeof(Op->upcode), sizeof(Op->upcode), Op->upcode);
	strCode.Remove(' ');
	
	POSITION pos = m_lstRegRealChart.GetHeadPosition();
	CRealChartData *pRealChart = NULL;
	BOOL bSendRealComplete = FALSE;
	
	while(pos)
	{
		pRealChart = NULL;		
		pRealChart = (CRealChartData*)m_lstRegRealChart.GetAt(pos);
		strMemCode	.Format("%s", pRealChart->m_DrdsData.KeyName	);
		strMemPacket.Format("%s", pRealChart->m_DrdsData.DataName	);

		memset(pRealChart->m_DrdsData.Data, 0x00, MAX_DRDS_PACKET_LEN);

		if( (strMemCode == strCode	) && 
			(strMemPacket == "JS0"	)
		)
		{        

			if( lstrcmp( pRealChart->m_DrdsData.FieldName, "time") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->time) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->time, sizeof(Op->time) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));							
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "jisu") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->jisu) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->jisu, sizeof(Op->jisu) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "sign") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->sign) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->sign, sizeof(Op->sign) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "btime") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->btime) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->btime, sizeof(Op->btime) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}  
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "bdate") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->bdate) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->bdate, sizeof(Op->bdate) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}  
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "volume") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->volume) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->volume, sizeof(Op->volume) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}    
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "cvolume") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->cvolume) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->cvolume, sizeof(Op->cvolume) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "rate") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->rate) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->rate, sizeof(Op->rate) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "change") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->change) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->change, sizeof(Op->change) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}
			else
			{
				bSendRealComplete = FALSE;
#ifdef _DEBUG

				CString sTemp;
				sTemp.Format("���� : Real ó���� �ȵǴ� �׸� :%32.32s", pRealChart->m_DrdsData.FieldName);
				AfxMessageBox(sTemp);
#endif
			}

		}	
		m_lstRegRealChart.GetNext(pos);
	}

	if(bSendRealComplete)
		::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA_PACKET_END, 0, 0 );
*/
}

void CStdDialog::SendFutureReal(char* pData)
{
/*	st_SC0* Op = (st_SC0*) pData;

	CString strCode, strMemCode, strMemPacket;
	strCode.Format("%*.*s", sizeof(Op->jmcode), sizeof(Op->jmcode), Op->jmcode);
	strCode.Remove(' ');
	
	POSITION pos = m_lstRegRealChart.GetHeadPosition();
	CRealChartData *pRealChart = NULL;
	BOOL bSendRealComplete = FALSE;
	while(pos)
	{
		pRealChart = NULL;		
		pRealChart = (CRealChartData*)m_lstRegRealChart.GetAt(pos);
		strMemCode	.Format("%s", pRealChart->m_DrdsData.KeyName	);
		strMemPacket.Format("%s", pRealChart->m_DrdsData.DataName	);

		memset(pRealChart->m_DrdsData.Data, 0x00, MAX_DRDS_PACKET_LEN);

		if( (strMemCode == strCode	) && 
			( (strMemPacket == "SC2") || (strMemPacket == "SC0") )
		)
		{        

			if( lstrcmp( pRealChart->m_DrdsData.FieldName, "time") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->time) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->time, sizeof(Op->time) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));							
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "price") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->price) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->price, sizeof(Op->price) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "sign") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->sign) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->sign, sizeof(Op->sign) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "high") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->high) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->high, sizeof(Op->high) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}  			
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "open") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->open) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->open, sizeof(Op->open) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			} 
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "low") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->low) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->low, sizeof(Op->low) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "cvolume") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->cvolume) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->cvolume, sizeof(Op->cvolume) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "rate") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->chgrate) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->chgrate, sizeof(Op->chgrate) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "change") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->change) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->change, sizeof(Op->change) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}
			else
			{
				bSendRealComplete = FALSE;
#ifdef _DEBUG
				CString sTemp;
				sTemp.Format("KOSPI ����  : Real ó���� �ȵǴ� �׸� :%32.32s", pRealChart->m_DrdsData.FieldName);
				AfxMessageBox(sTemp);
#endif
			}

		}	
		m_lstRegRealChart.GetNext(pos);
	}

	if(bSendRealComplete)
		::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA_PACKET_END, 0, 0 );
*/	
}

void CStdDialog::SendOptionReal(char* pData)
{
/*
	st_OC0* Op = (st_OC0*) pData;

	CString strCode, strMemCode, strMemPacket;
	strCode.Format("%*.*s", sizeof(Op->jmcode), sizeof(Op->jmcode), Op->jmcode);
	strCode.Remove(' ');
	
	POSITION pos = m_lstRegRealChart.GetHeadPosition();
	CRealChartData *pRealChart = NULL;
	BOOL bSendRealComplete = FALSE;
	while(pos)
	{
		pRealChart = NULL;		
		pRealChart = (CRealChartData*)m_lstRegRealChart.GetAt(pos);
		strMemCode	.Format("%s", pRealChart->m_DrdsData.KeyName	);
		strMemPacket.Format("%s", pRealChart->m_DrdsData.DataName	);

		memset(pRealChart->m_DrdsData.Data, 0x00, MAX_DRDS_PACKET_LEN);

		if( (strMemCode == strCode	) && 
			( (strMemPacket == "OC2") || (strMemPacket == "OC0") )
		)
		{        

			if( lstrcmp( pRealChart->m_DrdsData.FieldName, "time") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->time) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->time, sizeof(Op->time) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));							
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "price") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->price) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->price, sizeof(Op->price) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "sign") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->sign) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->sign, sizeof(Op->sign) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "high") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->high) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->high, sizeof(Op->high) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}  
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "open") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->open) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->open, sizeof(Op->open) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			} 
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "low") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->low) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->low, sizeof(Op->low) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "volume") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->cvolume) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->cvolume, sizeof(Op->cvolume) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "rate") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->chgrate) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->chgrate, sizeof(Op->chgrate) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}
			else if( lstrcmp( pRealChart->m_DrdsData.FieldName, "change") == 0 )
			{	
				pRealChart->m_DrdsData.Len = sizeof(Op->change) + 3;		
				memset(pRealChart->m_DrdsData.Data, 0x00,  sizeof(pRealChart->m_DrdsData.Data));
				memcpy(pRealChart->m_DrdsData.Data, Op->change, sizeof(Op->change) );
				::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA, sizeof(pRealChart->m_DrdsData), (LPARAM)(const char*)&(pRealChart->m_DrdsData));								
				bSendRealComplete = TRUE;
			}
			else
			{
				bSendRealComplete = FALSE;
#ifdef _DEBUG
				CString sTemp;
				sTemp.Format("KOSPI �ɼ� : Real ó���� �ȵǴ� �׸� :%32.32s", pRealChart->m_DrdsData.FieldName);
				AfxMessageBox(sTemp);
#endif
			}

			m_lstRegRealChart.GetNext(pos);
			OutputDebugString("Chart ocx send option Real");
		}
	}
	if(bSendRealComplete)
		::SendMessage( pRealChart->m_hOCXHwnd, WM_DRDS_DATA_PACKET_END,  0, 0);	
*/
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




//void CStdDialog::OnChartMouseMove(UINT	nFlags, CPoint	pt)
//{
//	if(m_bShowDataListWnd)
//	{
//		if(m_ChartMng.GetSafeHwnd())
//		{
//			if(m_ChartMng.GetpChartItemSelected() != NULL)
//			{
//				int nIndex = m_ChartMng.GetpChartItemSelected()->GetDataIndexInDataListViewer(pt.x, pt.y);
//				//CString sTemp;
//				//sTemp.Format("GetDataIndex... %d", nIndex);
//				//OutputDebugString(sTemp);
//				_EnSureVisible(m_pDataListView, nIndex);
//			}
//		}
//	}
//}

//@Solomon-->
//void CStdDialog::Request_StockHogaData(CString strRecvCode)
//{
//	if(m_ChartMng.m_pFloatingDlg == NULL) return;
//	if(m_ChartMng.m_pFloatingDlg->GetSafeHwnd() == NULL ) return;
//	if(m_ChartMng.m_pFloatingDlg->m_bIconState) return; //��� ���¸� TR��ȸ ���Ѵ�.
//
//	if(m_pInputBarDlg == NULL) return;
//	if(m_pInputBarDlg->GetSafeHwnd() == NULL) return;
//
//	CString strCode;
//	if(strRecvCode.GetLength() == CODE_STOCK_LENGTH)
//		strCode = strRecvCode;
//	else
//		strCode = m_pInputBarDlg->m_ctrlCodeInput.GetDataText();
//
//	if(!strCode.GetLength())
//	{
//		//ȣ��â Data Clear & Real ���� ===================================
//		
//		Request_Disconnect_StockRealHogaData(m_pInputBarDlg->m_strOldCode);
//		return;
//		//================================================================
//	}
//
//	if(m_ChartMng.m_pFloatingDlg != NULL)
//	{
//		if(m_ChartMng.m_pFloatingDlg->GetSafeHwnd())
//		{	
//			// Real ���� ====================================================================
//			if(m_pInputBarDlg->m_strOldCode != strCode)
//				Request_Disconnect_StockRealHogaData(m_pInputBarDlg->m_strOldCode);
//			// Real ���� ====================================================================
//
//			if(strCode.GetLength() != CODE_STOCK_LENGTH)
//				return;
//
//			char stJMCode[7];
//			memset(&stJMCode,0x00,sizeof(stJMCode));			
//			memcpy(stJMCode, strCode, strCode.GetLength());		
//			
////@�����߰�
//			CString strFidInfo("0                                                                               ");
//			CString strTrInfo;
//			strTrInfo.Format("%s20001", strCode);
//			CString strRecordInfo("1               000060220221001002003004005006008122126130134138124128132136140162166170174178164168172176180202204207209061062222070075010092;");
//			CString strOutput;
//			strOutput = strFidInfo + strTrInfo + strRecordInfo;
////@�����߰�
////@��������
//			stSend2Server st;
//			st.m_nType = 0;
//			st.m_pWnd = (CWnd*)m_ChartMng.m_pFloatingDlg;
//			st.m_pBuffer = (void*)(LPCTSTR)strOutput;
//			st.m_lLength= strOutput.GetLength();
//			st.m_strTRCode = QUERY_STOCK_HOGA;	
//			SendMessage(UMSG_SEND2SERVER,0,(long)&st);
////@�����߰�
//			//2005. 07. 15
//			Request_StockRealHogaData(strCode);
//		}
//	}
//}
//@Solomon<--

void CStdDialog::Request_StockRealHogaData(CString strCode)
{
	// �ֽ� Real Hoga ��û ====================================================================
	if(strCode.GetLength())
	{
		//if(m_pDBMgr)
		{				
			strCode.TrimLeft();
			strCode.TrimRight();
			m_DrdsInfo[4].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(4, FALSE);		// �������� ������ : �������� - ojtaso (20070208)
		}
	}
	

	// �ֽ� Real Hoga ��û ====================================================================
//	if(strCode.GetLength())
//	{
//		//if(m_pDBMgr)
//		{				
//			strCode.TrimLeft();
//			strCode.TrimRight();
//
//			CString strRealKey;
//			strRealKey.Format("%d,%6s", PID_JONGHOGA_10, strCode);
//			m_IDrMLib_CDBMgr.AddRealUpdate(strRealKey, m_hWnd);
//		}				
//	}
	// �ֽ� Real Hoga ��û  ====================================================================
}

void CStdDialog::Request_Disconnect_StockRealHogaData(CString strCode)
{
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
	

//	if(!bIsFirst)
//	{
//		m_ChartMng.SelectNextPos();
//	}
//	else
//	{
//		m_ChartMng.SetChartSelected(CPoint(0,0));
//	}

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
	return 0;
}
/// gm0604 end

void CStdDialog::Send2HanHwa_09501(void* pData, int nLen, CWnd* pRequestWnd, CString strTRCode)
{
	EnableWindowAll(FALSE);	

	SendMessage2StatusBar(MSG_GOING);
	m_pRqWndFromTab = NULL;
	m_pRqWndFromTab = pRequestWnd;
	//m_TRComm.Send2HanWha(pData, nLen);
}

#undef OBJECT_SELECT
#include "../chart_common/BlockBaseEnum.h"


LONG CStdDialog::OnByPass_ExportToolOption(UINT wParam, LPARAM lParam)
{	
	//if(wParam == CToolOptionInfo::T_RESTORE || wParam == CToolOptionInfo::T_SYMBOL_BULET)
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

	//CWnd* pWnd = (CWnd*)m_ChartMng.GetpChartItemSelected();
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
//				if(wParam != CToolOptionInfo::T_TOOL_SELECTION)
//				{
//					if(m_ChartMng.GetSafeHwnd())
//						m_ChartMng.SendMessage(UMSG_STOP_APPLY_EXPTOOOL);
//				}
			}
		}
	}
	return 0L;
}

void CStdDialog::SendMessage2StatusBar(int nMsgType, const char* pMsg, int nNum /*=0*/)
{
	CString strMessage;
	switch(nMsgType)
	{
		case	MSG_GOING:
			strMessage.Format("ó����...");
			break;
		default:
			if( lstrlen(pMsg) )
				strMessage.Format("%s", pMsg);
			break;
	}
//	CWnd *pFrameWnd = GetParentFrame();
//	pFrameWnd->SendMessage(WMU_SETSTATUSMSG, 0, (LPARAM)(LPCSTR)strMessage);

/*	switch(nMsgType)
	{
	case	MSG_GOING:
		strMessage.Format(" [00000] ó�����Դϴ�...��� ��ٷ� �ֽʽÿ�...");	
		break;
	case	MSG_SEARCH_END:
		strMessage.Format(" [00000] ��ȸ�� �Ϸ�Ǿ����ϴ�.");	
		break;
	case	MSG_SEARCH_FAIL:
		{
			if( lstrlen(pMsg) )
				strMessage.Format("%s", pMsg);
			else
				strMessage.Format(" [99999] ��ȸ����! �ٽ� �õ��Ͻʽÿ�");
		}
		
		break;
	case	MSG_NO_ACCT:
		strMessage.Format(" [99999] ���¹�ȣ�� ������ �ֹ� �� ���°��� ��ȸ�� �Ͻ� �� �����ϴ�");	
		break;
	case	MSG_NO_VOLUME:
		strMessage.Format("�ֹ� ������ �Է��� �ֽʽÿ�.");	
		break;
	case	MSG_NO_CODE:
		strMessage.Format("�����ڵ带 �Է��� �ֽʽÿ�.");
		break;
	case	MSG_FALSE_CODE:
		strMessage.Format("�����ڵ尡 �����ϴ�.");
		break;
	case	MSG_NO_PASS:
		strMessage.Format("��й�ȣ�� �Է��� �ֽʽÿ�.");
		break;
	case	MSG_FALSE_PASS:
		strMessage.Format("��й�ȣ�� �߸� �Է��ϼ̽��ϴ�.");
		break;
	case	MSG_TRUE_PASS:
		strMessage.Format("��й�ȣ�� Ȯ�εǾ����ϴ�.");
		break;
	case	MSG_NOTENOUGH_100CASH:
		strMessage.Format("�ֹ����� ������ �����մϴ�. �ֹ����� �Ǵ� �ݾ��� Ȯ���Ͻʽÿ�.");
		break;
	case	MSG_NOTENOUGH_PERCENT:
		strMessage.Format("���������� �۰ų� ������ �����ϴ�.");
		break;
	// �ֹ����� �޽��� --------------------------------------------------------->
	case	MSG_JUMUN_OK:
		strMessage.Format(" [00000] [�ֹ�No.%d](��)�� ���� ó���Ǿ����ϴ�.", nNum);
		break;
	case	MSG_CORRECT_OK:
		strMessage.Format(" [00000] �ֹ��� ���� Ȯ�εǾ����ϴ�.");	
		break;
	case	MSG_CANCEL_OK:
		strMessage.Format(" [00000] �ֹ��� ��� Ȯ�εǾ����ϴ�.");
		break;
	case	MSG_MAESU_CHE:
		strMessage.Format(" [00000] �ֹ���(%d��) �ż� ü��Ǿ����ϴ�.", nNum);
		break;
	case	MSG_MAEDO_CHE:
		strMessage.Format(" [00000] �ֹ���(%d��) �ŵ� ü��Ǿ����ϴ�.", nNum);
		break;
	case	MSG_REJECT:
		strMessage.Format(" [00000] �ֹ��� ���� �źεǾ����ϴ�. ����� �̿��Ͽ��ֽʽÿ�.");
		break;
	case	MSG_REJECT_OK:
		strMessage.Format(" [00000] �ֹ��� �ź� Ȯ�εǾ����ϴ�.");
		break;
//	case	MSG_CHK_INPUTVOL:
//	case	MSG_CHK_JANGO:
//		strMessage.Format(" [00000] %s", m_szMessage.Mid(7));
//		break;
//	case	MSG_JUMUN_RELATE:
//		strMessage.Format(" [00000] %s", m_szMessage);
//		break;
	//-----------------------------------------------------------------------------
	// Author		: Dae-Sung, Byun	Date :2005/4/27
	// Reason		: ��ȭ ���� ���� �߰�
	//-----------------------------------------------------------------------------
	//case MSG_TEMP_MSG:
	//	strMessage.Format(" [00000] %s", m_szMessage);
	//	break;	
	// <------------------------------------------------------------------------
	default	:
		break;
	}
*/
	
}


void CStdDialog::Send2HanHwa_IFCond(void* pData, int nLen, CWnd* pRequestWnd, CString strTRCode)
{
	EnableWindowAll(FALSE);	

	SendMessage2StatusBar(MSG_GOING);
	m_pRqWndFromTab = NULL;
	m_pRqWndFromTab = pRequestWnd;

//	m_TRComm->SetTR(TRNO_CONDITION);
	//int nRet = m_TRComm.Send2Server_Buf((LPCSTR)pData, nLen, FALSE, TRHEAD_FIND);	
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
bool CStdDialog::SetImaginaryCandleToOutside(const bool bImaginaryCandle)
{ 
	m_ChartCfgInfo.bRealCandle	= bImaginaryCandle; // TRUE �� ���
	m_ChartMng.UpdateShowingFacked_JustOneChart(bImaginaryCandle, FALSE);

	//���� ȯ�� ����ȭ ��ư�� ��� ���·� �ٲ� ��� ȯ�浿��ȭ On�� ��� �����Ѵ�.
	//{
		m_pExtInputBarDlg->SendMessage(UMSG_SETNORMALIZEDMINUTETIMERULE,bImaginaryCandle);	
		m_ChartMng.SendMessage(UMSG_SETNORMALIZEDMINUTETIMERULE,bImaginaryCandle);	
	//}
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
	//m_ChartMng.SendMessage(UMSG_SETSHOWLOCK,bShow);	
	
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
//	m_ChartMng.OnChartEnvSetupOK();
	return TRUE;
}


bool CStdDialog::GetImaginaryCandleToOutside		(bool& bImaginaryCandle		)	// ���������.
{
	return m_ChartMng.GetImaginaryCandleToOutside(bImaginaryCandle);
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

BOOL CStdDialog::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CStdDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(rcClient);
	dc.FillSolidRect(rcClient, m_clrBackground);
}

void CStdDialog::InitImageList()
{
	// ��ư �̹��� �ε�
	BOOL bOK = FALSE;
	CBitmap bitmap;
	CString strBitmapFile;	
	
	// �ε�� ��Ʈ ����
	m_static_nChartCnt4ImgList++;

//@��������
//	//��ȣ �̹��� ����Ʈ ===========================================================================
//	if ( !m_ImgList_Signal.m_hImageList)
//	{
//		if ( !m_ImgList_Signal.Create( 16, 16, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
//		{
//			AfxMessageBox ( "Can not Find ��ȣ���ȣ.bmp file" );			
//		}		
//
//		// Load ImageList
//		CString strSignalImageFile = m_szImageDir;
//		strBitmapFile = strSignalImageFile + "��ȣ���ȣ.bmp";				
//
//		bOK = bitmap.Attach((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0,
//			LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
//		m_ImgList_Signal.Add(&bitmap, RGB(255,0,0));
//		bitmap.DeleteObject();
//	}
//@��������

	//W15_H15===========================================================================
	if ( !m_ImgList_W15H15.m_hImageList)
	{
		if ( !m_ImgList_W15H15.Create( 15, 15, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W15_H15.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W15_H15.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W15H15.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}
	
	//W20_H20===========================================================================
	if ( !m_ImgList_W20H20.m_hImageList)
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

	//W31_H20===========================================================================
	if ( !m_ImgList_W31H20.m_hImageList)
	{
		if ( !m_ImgList_W31H20.Create( 31, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W31_H20.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W31_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W31H20.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	//W20_H20_Tool===========================================================================
	if ( !m_ImgList_W20H20_Tool.m_hImageList)
	{
		if ( !m_ImgList_W20H20_Tool.Create( 20, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W20_H20_Tool.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W20_H20_Tool.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W20H20_Tool.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	//W46_H20===========================================================================
	if ( !m_ImgList_W46H20.m_hImageList)
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
	
	//W50_H20===========================================================================
	if ( !m_ImgList_W50H20.m_hImageList)
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
	//W151_H21===========================================================================
	if ( !m_ImgList_W151H21.m_hImageList)
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


	//W28_H22===========================================================================
	if ( !m_ImgList_W28H22.m_hImageList)
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

	//W17_H20===========================================================================

	if ( !m_ImgList_W17H20.m_hImageList)
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

	//W43_H20===========================================================================
	if ( !m_ImgList_W43H20.m_hImageList)
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

	//W58_H20===========================================================================
	if ( !m_ImgList_W58H20.m_hImageList)
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

	//W72_H19===========================================================================
//	if ( !m_ImgList_W72H20.m_hImageList)
//	{
//		if ( !m_ImgList_W72H20.Create( 72, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
//		{
//			AfxMessageBox ( "Can not Find W72_H20.bmp file" );			
//		}
//		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W72_H20.bmp");
//		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
//			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
//		m_ImgList_W72H20.Add(&bitmap, RGB(255, 0, 255));
//		bitmap.DeleteObject();
//	}
	
	//W46_H20_Gwan===========================================================================
//	if ( !m_ImgList_W46H20_Gwan.m_hImageList)
//	{
//		if ( !m_ImgList_W46H20_Gwan.Create( 46, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
//		{
//			AfxMessageBox ( "Can not Find W46_H20_Gwan.bmp file" );			
//		}
//		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W46_H20_Gwan.bmp");
//		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
//			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
//		m_ImgList_W46H20_Gwan.Add(&bitmap, RGB(255, 0, 255));
//		bitmap.DeleteObject();
//	}

	//W8_H25===========================================================================
//	if ( !m_ImgList_W8H25.m_hImageList)
//	{
//		if ( !m_ImgList_W8H25.Create( 8, 25, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
//		{
//			AfxMessageBox ( "Can not Find W8_H25.bmp file" );
//		}
//		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W8_H25.bmp");
//		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
//			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));	
//		m_ImgList_W8H25.Add(&bitmap, RGB(255, 0, 255));
//		bitmap.DeleteObject();
//		
//	}

	//W60_H19===========================================================================
	if ( !m_ImgList_W60H20.m_hImageList)
	{		
		if ( !m_ImgList_W60H20.Create( 60, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W60_H20.bmp file" );
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W60_H20.bmp");		
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
								LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W60H20.Add(&bitmap, RGB(255, 0, 255));
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
		m_ImgList_W31H20.DeleteImageList();
		m_ImgList_W20H20_Tool.DeleteImageList();
		m_ImgList_W46H20.DeleteImageList();
		m_ImgList_W50H20.DeleteImageList();
		m_ImgList_W151H21.DeleteImageList();
		m_ImgList_W28H22.DeleteImageList();
		m_ImgList_W17H20.DeleteImageList();
		m_ImgList_W43H20.DeleteImageList();	
		m_ImgList_W58H20.DeleteImageList();	
//		m_ImgList_W72H20.DeleteImageList();	
//		m_ImgList_W46H20_Gwan.DeleteImageList();
//		m_ImgList_W8H25.DeleteImageList();
		m_ImgList_W60H20.DeleteImageList();
	}
}

void CStdDialog::GetStartEndDateAtStdDialog(double& dStartDate, double& dEndDate)
{
	if(m_ChartMng.GetSafeHwnd())
		m_ChartMng.GetStartEndDateAtChartMng(dStartDate, dEndDate);	
}


LRESULT CStdDialog::OnDropData(WPARAM wp, LPARAM lp)
{	
//	CSize szSize;
//	DWORD dwType;
//	HWND hWnd;
//	CString szCode;
//	
//	m_pMainBaseInfo->GetDropInfo(lp,szSize,szCode, dwType,hWnd);
//	
//	BOOL bCanChangeCode = FALSE;
//	switch(m_nMarketType)
//	{
//		case STOCK_CHART:
//			{
//				long lCheck1 = dwType & CODE_JONGMOK	;
//				long lCheck2 = dwType & CODE_KJONGMOK	;
//				long lCheck3 = dwType & CODE_OTCBB		;
//				if(lCheck1 == CODE_JONGMOK  || lCheck2 == CODE_KJONGMOK/* || lCheck3 == CODE_OTCBB*/)
//					bCanChangeCode = TRUE;	
//					break;
//			}
//			break;
//		case UPJONG_CHART:
//			{
//				long lCheck1 = dwType & CODE_UPJONG;
//				long lCheck2 = dwType & CODE_K200UPJONG;
//				long lCheck3 = dwType & CODE_KUPJONG;
//				if(lCheck1 == CODE_UPJONG  || lCheck2 == CODE_K200UPJONG || lCheck3 == CODE_KUPJONG)
//					bCanChangeCode = TRUE;					
//			}
//			break;
//		case FUTOPT_CHART:
//			{
//				long lCheck1 = dwType & CODE_FUTURE;
//				long lCheck2 = dwType & CODE_OPTION;
//				long lCheck3 = dwType & CODE_QFUTURE;
//				long lCheck4 = dwType & CODE_JOPTION;
//				long lCheck5 = dwType & CODE_FSSPREAD;
//				long lCheck6 = dwType & CODE_FSTAR;
//				if(lCheck1 == CODE_FUTURE  || lCheck2 == CODE_OPTION || lCheck3 == CODE_QFUTURE || lCheck4 == CODE_JOPTION || lCheck5 == CODE_FSSPREAD || lCheck6 == CODE_FSTAR)
//					bCanChangeCode = TRUE;				
//			}
//			break;
//		case FOREIGN_CHART:			
//			{
//				long lCheck1 = dwType & CODE_EXCHANGE;
//				long lCheck2 = dwType & CODE_EXCH_AUP;
//				long lCheck3 = dwType & CODE_EXCH_AJONG;
//				long lCheck4 = dwType & CODE_EXCH_DR;
//				if(lCheck1 == CODE_EXCHANGE  || lCheck2 == CODE_EXCH_AUP || lCheck3 == CODE_EXCH_AJONG || lCheck4 == CODE_EXCH_DR)
//					bCanChangeCode = TRUE;					
//			}		
//			break;
//		default:
//			return 0;
//	}
//	
//	if(bCanChangeCode)
//		m_pMainBaseInfo->SetTextToCodeInput(szCode);
//	
	return 0;
}

void CStdDialog::SendHogaNCodeToSpeedJumun(CString strCode, CString strPrice)
{
	m_xGSharedCli.SetData("S51210-ȣ��;", strPrice);
	//if(m_pLeftBarDlg)
	//	m_pLeftBarDlg->SendMessage(UMSG_SETCHARTHOGA,NULL,(LPARAM)&strData);
}

void CStdDialog::AllCodeUnAdvise()
{
	for(int i = 0; i < m_nDrdsInfoCnt; ++i)
		MakeDrdsUnConnectUnAdviseFromCode(i, FALSE);
}


BOOL CStdDialog::GetTimeSyncFlag()
{
	//return m_pExtInputBarDlg->GetTimeSyncFlag();
	return TRUE;
}

void CStdDialog::SetTimeSyncFlag(BOOL bCheck)
{
}

void CStdDialog::SetOpenMapData(LPCTSTR szOpenData, short nDataLen) 
{
	// TODO: Add your dispatch handler code here
//	m_xGSharedCli.SetData("�ڵ�-�ֽ�",(LPCTSTR)szOpenData);

	if(m_pInputBarDlg)
	{
//080520-001 CodeInput����		m_pInputBarDlg->SetShareCodeChanged(TRUE);
//080520-001 CodeInput����		m_pInputBarDlg->m_ctrlCodeInput.SetDataText((LPCTSTR)szOpenData);
	}
//	m_pInputBarDlg->SetKeyCodeChange(NID_CODEINPUT, (LPARAM)(LPCTSTR)szOpenData);
}

//@�����߰�
void CStdDialog::ChangeCode(LPCTSTR szCode, int nDefault/* = MODE_DEFAULT*/, int nMode/* = CHANGE_ITEM*/) 
{
//	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
//		m_pInputBarDlg->m_ctrlCodeInput.SetDataText(szCode);

	STINPUTCODEDATA stInputCodeData;
	stInputCodeData.m_strCode = szCode;
	stInputCodeData.m_strTime = "";
	stInputCodeData.m_nDefault = nDefault;
	stInputCodeData.m_nMode = nMode;
	// �������� �߰� UI : �������� - ojtaso (20070522)
	m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, nDefault, (LPARAM)&stInputCodeData);
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
//@��������
//			STINPUTCODEDATA2	stInputCodeData2;
//			stInputCodeData2.m_strCode = pDnD->GetValue();
//			stInputCodeData2.m_strCode.Remove(';');
//			stInputCodeData2.m_bDontPutInShared = FALSE;
//			stInputCodeData2.m_bRequestWithoutHesitation = FALSE;
//			if(m_pInputBarDlg)
//				m_pInputBarDlg->SendMessage(UMSG_INPUTCODETOINPUTBAR,(WPARAM)&stInputCodeData2);			
//@��������
//@�����߰�
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

//					char szTemp[64];
//					long lPosAll = 0;
//					long lSize = 0;
					
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
//@�����߰�
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
		HINSTANCE hInstResourceClient = AfxGetResourceHandle();
		AfxSetResourceHandle(g_hInstance);

		m_hCursor = AfxGetApp()->LoadCursor(IDC_HANDCUR);
		AfxSetResourceHandle(hInstResourceClient);
		::SetCursor(m_hCursor);
	}

	return 0L;
}

//003 {{ ȣ��TR_AddDataKey		
//void CStdDialog::AddDataKey_Hoga()
//{
// 		m_TRComm->AddDataKey(15007);	//���ذ�
// 		//{{ 2007.06.26 by LYH ȣ��â�� ����ü�� ����
// 		//	m_TRComm->AddDataKey(15176);	//����ü�ᰡ��
// 		//	m_TRComm->AddDataKey(15309);	//����ü�����	//-> ����ŷ������� �ӽù���
// 		m_TRComm->AddDataKey(15313);	//����ü�ᰡ��
// 		m_TRComm->AddDataKey(15315);	//����ü�����	//-> ����ŷ������� �ӽù���
// 		//}}
// 
// 		m_TRComm->AddDataKey(DI_NAME);	//�����
// 		m_TRComm->AddDataKey(DI_PRICE);	//���簡
// 		//m_TRComm->AddDataKey(DI_SIGN);	//�����ȣ
// 		m_TRComm->AddDataKey(15317);	//�����ȣ
// 		m_TRComm->AddDataKey(15004);	//�����
// 		m_TRComm->AddDataKey(15472);	//�����
// 		m_TRComm->AddDataKey(DI_VOLUME);	//�����ŷ���	//-> �ŷ������� �ӽù���
// 		m_TRComm->AddDataKey(DI_AMOUNT);	//�����ŷ����	//-> �ŷ�������� �ӽù���
// 		//m_TRComm->AddDataKey(14659);	//ȣ���ܷ����ؽð�
// 		
// 		// �ŵ� 8 + 40 + 
// 		int nIdx;
// 		int nDataKey;
// 
// 		nDataKey = DI_OFFER_HO1;		// �ŵ�ȣ��1
// 		for(nIdx=0; nIdx < 5; nIdx++)
// 		{
// 			m_TRComm->AddDataKey(nDataKey+nIdx);		
// 		}
// 
// 		nDataKey = DI_OFFER_REM1;		// �ŵ�ȣ���ܷ�1
// 		for(nIdx=0; nIdx < 5; nIdx++)
// 		{
// 			m_TRComm->AddDataKey(nDataKey+nIdx);		
// 		}
// 
// 		nDataKey = DI_BID_HO1;		// �ż�ȣ��1
// 		for(nIdx=0; nIdx < 5; nIdx++)
// 		{
// 			m_TRComm->AddDataKey(nDataKey+nIdx);		
// 		}
// 
// 		nDataKey = DI_BID_REM1;		// �ż�ȣ���ܷ�1
// 		for(nIdx=0; nIdx < 5; nIdx++)
// 		{
// 			m_TRComm->AddDataKey(nDataKey+nIdx);		
// 		}
// 
// 		//{{ 2007.06.27 by LYH ���ܷ��� 10ȣ���ܷ����� ����
// 		//m_TRComm->AddDataKey(DI_OFFER_TOTREM);	//�Ѹŵ��ܷ�
// 		//m_TRComm->AddDataKey(DI_BID_TOTREM);	//�Ѹż��ܷ�
// 		m_TRComm->AddDataKey(14565);	//10ȣ���ŵ��ܷ�
// 		m_TRComm->AddDataKey(14567);	//10ȣ���ż��ܷ�
// 		//}}
// 		m_TRComm->AddDataKey(DI_OFFER_OUT_TOT);	//�ð��ܸŵ��ܷ�
// 		m_TRComm->AddDataKey(DI_BID_OUT_TOT);	//�ð��� �ż��ܷ�
// 
// 		m_TRComm->AddDataKey(16117);	//���Ѱ�
// 		m_TRComm->AddDataKey(16118);	//���Ѱ�
// 		//{{ 2007.06.27 by LYH �ð���� �������� ����
// 		m_TRComm->AddDataKey(15326);	//�ð�->������ -> ���������� ��� 
// 		//}}
// 		m_TRComm->AddDataKey(15010);	//��
// 		m_TRComm->AddDataKey(15011);	//����
// 
// 		m_TRComm->AddDataKey(3007);		//���ϰŷ���  //-> ���ϰŷ���_������������ �����ͷ� �ӽû��
// 		m_TRComm->AddDataKey(16143);	//����kospi����kosdaq �ֽļ�
//}

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
//@��������
//	CRealUpdateData* cReal = (CRealUpdateData*)pRealData;
//	if(cReal->m_nQAIndex == 10051)
//	{
//		//ȣ�� ȭ�� ���� ó��
//		if(	(m_ChartMng.m_pFloatingDlg != NULL)		&&
//			m_ChartMng.m_pFloatingDlg->GetSafeHwnd()	
//			)	
//		{
//			m_ChartMng.m_pFloatingDlg->SetRealHogaData((long)pRealData);
//		}
//	}
//	else
//	{
//		if(wFID == 2 || wFID == 4)
//		{
//			CString szTemp0 = cReal->m_strMainKeyVal;
//			cReal->m_strMainKeyVal.Format("%1d%04s", wFID, szTemp0);
//		}
//		//��Ʈ ���� ó��
//		m_ChartMng.SetRealJPriceData(pRealData);
//	}
//@��������

	//������ ���� ó��/
	//if(m_pLeftBarDlg)
	//	m_pLeftBarDlg->SetRealPriceData(pRealData);
	
	//CRealUpdateData * pData = (CRealUpdateData *)pRealData;
	//bRet = pData->GetDataString(DI_CODE, strData);			//�ڵ�
	//bRet = pData->GetDataString(DI_NAME	, strData);
	//bRet = pData->GetDataString(DI_TIME	, strData);			//ü��ð�
	//bRet = pData->GetDataString(DI_PRICE, strData);			//���簡
	//bRet = pData->GetDataString(DI_CHANGE, strData);		//���
	//bRet = pData->GetDataString(DI_SIGN		, strData);		//�����ȣ
	//bRet = pData->GetDataString(DI_CHGRATE, strData);			//�����
	//bRet = pData->GetDataString(DI_HIGH		, strData);			//��
	//bRet = pData->GetDataString(DI_LOW		, strData);			//����
	//bRet = pData->GetDataString(DI_TRDVOL		, strData);		//ü�ᷮ
	//bRet = pData->GetDataString(DI_VOLUME	, strData);			//�����ŷ���
	//bRet = pData->GetDataString(DI_AMOUNT	, strData);			//�����ŷ����
	//bRet = pData->GetDataString(DI_OPENPRICE	, strData);		//�ð�
	return 1L;
}

// ������ �����ϰų� ����� ��� �����͸� ���� �ް� �ȴ�. �̶� �ʱ�ȭó���� ������ϴ� ��ƾ���ִ�.
// �ý���Ʈ���̵��� ������ ����ǰ� ���� �� �̷��� ���濡 ���� ó���� ����� �Ѵ�.
void CStdDialog::NofityNewDataReceive()
{
	// �ý��������� ����ǰ� ���� ��� ������ Ŭ�����Ѵ�.
	// m_ChartMng.GetpChartItemSelected()->InvokeAddIn("ChartSTIndexAddIn.dll", 7, (long)this);

	// ��������Ʈ�� ����Ǿ��ٴ� ������ �˸���.
	//int nID = UUID_IAUDataMonitorManager;
	//IAUDataMonitorManager* pMng = (IAUDataMonitorManager*)AfxGetPctrInterface(nID);
	//pMng->NotifyData("CompareChart_ReceiveData", (long)this, NULL);
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
	if(m_pInputBarDlg) // 2005. 04. 22 by Nam
		m_pInputBarDlg->SetViewCount(p_nDataCountInView);
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

	return;
}

//��Ʈ ����� ���� ���
void CStdDialog::AddRealCodes(CString strTrCode, LIST_CODEDATA *pListRealCodes) 
{
//	//@��������
//	//	m_TRComm->ResetRealTR(strTrCode);
//	//@��������
//	if(pListRealCodes->GetCount() <= 0)
//		return;
//
//	//@��������
//	//	m_TRComm->SetTRCondition(strTrCode, strTrCode);
//	//@��������
//	ST_CODEDATA oneCode;
//	int nOffset = 0;
//	char cCode[6+1]={0,}; 
//	int nIndex = 0;
//	UINT nCodeLength=0;
//	CByteArray bArr;
//	for(POSITION pos=pListRealCodes->GetHeadPosition(); pos; )
//	{
//		oneCode = pListRealCodes->GetNext(pos);
//		if(oneCode.m_bType == 79 || oneCode.m_bType == 80 || oneCode.m_bType == 81 || oneCode.m_bType == 82 || oneCode.m_bType == 83 || oneCode.m_bType == 84 || oneCode.m_bType == 85 || oneCode.m_bType == 86)
//		{
//			nCodeLength = 16;
//		}
//		else
//		{
//			nCodeLength = oneCode.m_strCode.GetLength();
//			if(nCodeLength != CODE_STOCK_LENGTH && nCodeLength != FUTOPT_LENGTH && nCodeLength != (UPJONG_LENGTH-1))
//				continue;
//		}
//
//		if(nIndex == 0)
//		{
//			if(oneCode.m_bType == 2 || oneCode.m_bType == 4)
//				m_TRComm->AddMainKey(DI_CODE, oneCode.m_strCode, C_DT_USHORT, 2);
//			else
//				m_TRComm->AddMainKey(DI_CODE, oneCode.m_strCode, C_DT_STRING, nCodeLength);
//			
//			m_TRComm->AddOptionKey(1, NULL, 5, 0, 0);
//			m_TRComm->SetRealUpdateType(2);			//  RT_UPDATE	2	// ������Ʈ Ÿ��
//			//{{2007.06.26 by LYH �ֽĸ��� ���(����ü�� �������� ����)
//			if(strTrCode == "20000")
//			{
//				m_TRComm->AddDataKey(15313);//���簡
//				m_TRComm->AddDataKey(15009);//�ð�
//				m_TRComm->AddDataKey(15010);//��
//				m_TRComm->AddDataKey(15011);//����
//				m_TRComm->AddDataKey(15317);//�����ȣ
//				m_TRComm->AddDataKey(15316);//���Ϻ�
//				m_TRComm->AddDataKey(15326);//�����
//				m_TRComm->AddDataKey(15315);//�ŷ���
//				m_TRComm->AddDataKey(DI_TIME);//ü��ð�
//			}
//			else
//				//}}
//			{
//				m_TRComm->AddDataKey(15001);//���簡
//				m_TRComm->AddDataKey(15009);//�ð�
//				m_TRComm->AddDataKey(15010);//��
//				m_TRComm->AddDataKey(15011);//����
//				m_TRComm->AddDataKey(15006);//�����ȣ
//				m_TRComm->AddDataKey(15472);//���Ϻ�
//				m_TRComm->AddDataKey(15004);//�����
//				m_TRComm->AddDataKey(15015);//�ŷ���
//				m_TRComm->AddDataKey(DI_TIME);//ü��ð�
//			}
//
//			nIndex++;
//		}
//		else
//		{
//	//@��������
//	//			if(oneCode.m_bType == 2 || oneCode.m_bType == 4)
//	//				m_TRComm->AddMainKey(DI_CODE, oneCode.m_strCode, C_DT_USHORT, 2, false);
//	//			else
//	//				m_TRComm->AddMainKey(DI_CODE, oneCode.m_strCode, C_DT_STRING, nCodeLength, false);
//	//@��������
//		}
//		bArr.Add(oneCode.m_bType);
//	}
//	//@��������
//	//	m_TRComm->SetForceMidArray(&bArr);
//	//	m_TRComm->SetSvcID("");
//	//	m_TRComm->Send2ServerCondition();
//	//@��������
}

//��Ʈ ����� ���� ���
void CStdDialog::RemoveRealCodes(LIST_CODEDATA *pListRealCodes, CString strKey) 
{
//	ST_CODEDATA oneCode;
//	for(POSITION pos=pListRealCodes->GetHeadPosition(); pos; )
//	{
//		oneCode = pListRealCodes->GetAt(pos);
//
//	//@��������
//	//		if(oneCode.m_bType == 2 || oneCode.m_bType == 4)
//	//		{
//	//			if(atoi(strKey.Left(1)) == oneCode.m_bType)
//	//			{
//	//				strKey = strKey.Mid(1);
//	//				if(strKey == oneCode.m_strCode)
//	//				{
//	//					pListRealCodes->RemoveAt(pos);
//	//					break;
//	//				}
//	//			}
//	//		}
//	//		else
//	//@��������
//		{
//			if(strKey == oneCode.m_strCode)
//			{
//				pListRealCodes->RemoveAt(pos);
//				break;
//			}
//		}
//		pListRealCodes->GetNext(pos);
//	}
}

int CStdDialog::GetWidthOfLeftBar()
{
	return m_nWidthOfLeftBar;
}

// �������� ���� ���� : �������� - ojtaso (20070209)
void CStdDialog::OnDeleteChartItem(LPCSTR lpszItemCode)
{
	//Real����
	OnCodeUnAdvise(lpszItemCode, FALSE);

	//@.080728-001
	OnCodeScdClear(lpszItemCode, TRUE);

//	CString strType = m_pInputBarDlg->m_pDataManager->GetCodeInfo("", 2, lpszItemCode);
//	if(strType == "79"|| strType == "80" || strType == "81" || strType == "82" || strType == "83" || strType == "84" || strType == "85" || strType == "86")
//	{
//		RemoveRealCodes(&m_ListForeignRealCodes, lpszItemCode);
//		AddRealCodes("20003", &m_ListForeignRealCodes);
//	}
//	else
//	{
//		int nLen = ::lstrlen(lpszItemCode);
//		if(nLen == CODE_STOCK_LENGTH)
//		{
//			RemoveRealCodes(&m_ListStockRealCodes, lpszItemCode);
//			AddRealCodes("20000", &m_ListStockRealCodes);
//		}
//		else if(nLen == UPJONG_LENGTH)
//		{
//			RemoveRealCodes(&m_ListUpjongRealCodes, lpszItemCode);
//			AddRealCodes("20001", &m_ListUpjongRealCodes);
//		}
//		else if(nLen == FUTOPT_LENGTH)
//		{
//			RemoveRealCodes(&m_ListFutOptRealCodes, lpszItemCode);
//			AddRealCodes("20002", &m_ListFutOptRealCodes);
//		}
//	}
}

// ��Ʈ �ʱ�ȭ�� ��� ���� ���� : �������� - ojtaso (20070621)
void CStdDialog::ResetChartAll()
{
/*
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
	if(!m_ListForeignRealCodes.IsEmpty())
	{
		m_ListForeignRealCodes.RemoveAll();
		AddRealCodes("20003", &m_ListForeignRealCodes);
	}
*/
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
//	m_pInputBarDlg->InitChartMulitItem(pSelChart);
//	m_pExtInputBarDlg->InitChartMulitItem(pSelChart);
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
	strFileName.Format( "%s\\CompareChart%s_%04d%02d%02d.log", szLogFolderPath, m_strScreenNo, sTime.wYear, sTime.wMonth, sTime.wDay);
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

//-->@Solomon090819 
// 	long CStdDialog::OnGetBroad(WPARAM wParam, LPARAM lParam)
// 	{
// 		if(m_pDrdsLib == NULL)
// 			return 0;
// 	//080529 �׽�Ʈ��. �ϴ� ����.
// 	//return 0L;
// 
// 		BC_ITEM_INFO *pRealItem = (BC_ITEM_INFO*)lParam;
// 
// 		if(strncmp(pRealItem->szBcCode, NAME_KXS3, strlen(NAME_KXS3)) == 0)			// �ֽ����簡
// 		{
// 			CRealData_Common realData_Common;
// 			CChartRealConverter::Convert_KXS3(pRealItem->szBcData, realData_Common);
// 			m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common);
// 			return 1L;
// 		}
// 		else if(strncmp(pRealItem->szBcCode, NAME_IXIJ, strlen(NAME_IXIJ)) == 0)	// �������簡
// 		{	
// 			CRealData_Common realData_Common;
// 			CChartRealConverter::Convert_IXIJ(pRealItem->szBcData, realData_Common);
// 			m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common);
// 			return 1L;
// 		}
// 		else if(strncmp(pRealItem->szBcCode, NAME_FFC0, strlen(NAME_FFC0)) == 0)	// �������簡
// 		{	
// 			CRealData_Common realData_Common;
// 			CChartRealConverter::Convert_FFC0(pRealItem->szBcData, realData_Common);
// 			m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common);
// 			return 1L;
// 		}
// 		else if(strncmp(pRealItem->szBcCode, NAME_OOC0, strlen(NAME_OOC0)) == 0)	// �ɼ����簡
// 		{	
// 			CRealData_Common realData_Common;
// 			CChartRealConverter::Convert_OOC0(pRealItem->szBcData, realData_Common);
// 			m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common);
// 			return 1L;
// 		}
// 		else if(strncmp(pRealItem->szBcCode, NAME_JFC0, strlen(NAME_JFC0)) == 0)	// �ֽļ���
// 		{	
// 			CRealData_Common realData_Common;
// 			CChartRealConverter::Convert_JFC0(pRealItem->szBcData, realData_Common);
// 			m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common);
// 			return 1L;
// 		}
// 		else if(strncmp(pRealItem->szBcCode, NAME_JOC0, strlen(NAME_JOC0)) == 0)	// �ֽĿɼ�
// 		{	
// 			CRealData_Common realData_Common;
// 			CChartRealConverter::Convert_JOC0(pRealItem->szBcData, realData_Common);
// 			m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common);
// 			return 1L;
// 		}
// 	/*
// 		else if(pRealType == FOREIGN_SISE_PID)
// 		{
// 			CRealData_Common realData_Common;
// 			CChartRealConverter::Convert_HWFJ(pRealStruct, realData_Common);
// 			m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common);
// 			return 1L;
// 		}
// 	*/
// 		return 0L;
// 	}
//<--@Solomon090819 

BOOL CStdDialog::RemoveGraph_Title(int nRow, int nCol, CString strCode, CString strJMName, BOOL bIsOnlyNoTRData)
{
	if(m_ChartMng.GetSafeHwnd())
		return m_ChartMng.GetpChartItemSelected()->RemoveGraphTitle(nRow, nCol, strCode, strJMName, bIsOnlyNoTRData);
	return FALSE;
}

//��Ʈ���� �׷����� �����ϴ� ���.
BOOL CStdDialog::RemoveGraph(int nRow, int nCol, CString strCode, CString strJMName)
{
	if(m_ChartMng.GetSafeHwnd())
	{
		//@100125-alzioyes.
		{
			//m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, MODE_DEFAULT, (LPARAM)&stInputCodeData);
			OutputDebugString("CStdDialog::RemoveGraph\n");

		}

		m_ChartMng.FindSelectChartByItemCode(strCode);
		CString strRQ = m_ChartMng.GetpChartItemSelected()->GetSelectedRQ();
//#ifdef __MULTI_LANG__
		BOOL bRet = m_ChartMng.GetpChartItemSelected()->RemoveGraphRQ(strRQ, "_Price_");
//#else
//		BOOL bRet = m_ChartMng.GetpChartItemSelected()->RemoveGraphRQ(strRQ, "����");
//#endif
		

		// "DEFAULT" �϶��� ��ƮOCX���� ���� �̺�Ʈ�� ������ �����Ƿ� ������ �޽����� ������.
// 		BOOL bRemoveMsg = FALSE;
// 		if(strRQ.Compare("DEFAULT")==0)
// 			bRemoveMsg = TRUE;
// 
// 		if(bRemoveMsg)
// 		{
// 			MSG msg;
// 			msg.message = UWM_SELECTEDCHART;
// 			msg.lParam = (LPARAM)(LPCSTR)strRQ;
// 			msg.wParam = 0;	//0:����
// 			m_ChartMng.SendMessage(RMSG_CHARTMSG_EVENTMESSAGE, 0, (LPARAM)&msg);
// 		}

// 		// DEFAULT�� �������Ѵ�.
// 		if(strRQ.Compare("DEFAULT")==0)
// 		{
// 			return FALSE;
// 		}

		return bRet;
	}

	return FALSE;
}

void CStdDialog::SetGraphHide3(CString strCode, CString strName, BOOL bIsHide)
{
	if(m_ChartMng.GetSafeHwnd())
		m_ChartMng.GetpChartItemSelected()->SetGraphHide3(strName, bIsHide);
}

CString CStdDialog::GetCodeNameAndGubun(int nChartType, CString csCode)
{
	CString str		=	_T("");	
	CString csName	=	_T("");

	switch(nChartType)
	{
		case CMasterTypeInfo::UPJONG_CHART:
			{
				if(csCode.GetLength()==UPJONG_LENGTH)
				{
			
					CString strKey;
					strKey = csCode;
					if(m_pDataManager != NULL)
						//csName = m_pDataManager->GetCodeInfo("UMASTER", 0, strKey);
						csName = m_pDataManager->GetCodeName(strKey);
					return csName;
				}
			}
			break;
		case CMasterTypeInfo::FUTOPT_CHART:
			{
				if( (csCode.GetLength() == FUTOPT_LENGTH) ||
					(csCode.GetLength() == KQFUT_LENGTH) ||
					(csCode.GetLength() == KQOPT_LENGTH )
				)
				{
					//csName			= m_pDataManager->GetCodeInfo("FMASTER", 0, csCode);
					csName = m_pDataManager->GetCodeName(csCode);
					
					//if(csName.GetLength()<1)
					//	csName		= m_pDataManager->GetCodeInfo("PMASTER", 0, csCode);
					
					csName.TrimLeft();			
					return csName;
				}
			}
			break;
		case CMasterTypeInfo::STOCK_CHART: 
			{
				if(csCode.GetLength()==CODE_STOCK_LENGTH)
				{
					//csName			= m_pDataManager->GetCodeInfo("JMASTER", 0, csCode);
					csName			= m_pDataManager->GetCodeName(csCode);
					
					//if(csName.GetLength()<1)
					//	csName		= m_pDataManager->GetCodeInfo("KMASTER", 0, csCode);
					
					csName.TrimLeft();
					if(!csName.IsEmpty())
					{
						long lPos = csName.Find('&');
						if(lPos>0)
						{
							csName.Insert(lPos,'&');
						}
						return csName;
					}
					return csName;
				}	
			}
			break;
		case CMasterTypeInfo::FOREIGN_CHART:	
			break;
		default:
	#ifdef _DEBUG
				AfxMessageBox("GetCodeNameAndGubun == ��Ʈ ���а��� ����");
	#endif
			break;
	}

	return "";
}

// ���� �Է����ǰ� �־��� �����ڵ忡 ���� ��ȸ���ۻ���.
BOOL CStdDialog::RequestTRFormCode(LPCSTR szRuestCode, BOOL bNext)
{
	BYTE cType = m_pInputBarDlg->m_pDataManager->GetMarketTypeFromCode(szRuestCode);
	switch(cType)
	{
// 	case CMasterTypeInfo::STOCK_CHART:
// 		ChangeStockMode(cType);
// 		break;
	case CMasterTypeInfo::STOCKFUTOPT_CHART:
	case CMasterTypeInfo::FUTOPT_CHART:
		ChangeStockMode(CMasterTypeInfo::FUTOPT_CHART);
		break;
// 	case CMasterTypeInfo::UPJONG_CHART:
// 		ChangeStockMode(cType);
// 		break;
// 	case CMasterTypeInfo::FOREIGN_CHART:
// 		ChangeStockMode(cType);
// 		break;
// 	case CMasterTypeInfo::FUOPT_JIPYO_CHART:
// 		ChangeStockMode(cType);
// 		break;
	default:
//			if (m_nScreenType == Screen_ForeignChart)
//				ChangeStockMode(FOREIGN_CHART);
		ChangeStockMode(cType);
			break;
	}

	SendToChartMngCode(szRuestCode);

	return TRUE;

// 
// 		//CString strType = m_pInputBarDlg->m_pDataManager->GetCodeInfo("", 2, szRuestCode);
// 		BYTE cType = m_pInputBarDlg->m_pDataManager->GetMarketTypeFromCode(szRuestCode);
// 
// 		CString szQuerTR;
// 	// 	if( strType == "1" ||		// ��������
// 	// 		strType == "2" ||		// �ڽ�������
// 	// 		strType == "3" ||		// ��������
// 	// 		strType == "N" )		// �ֽĿ���Ʈ ELW
// 		if(cType==CMasterTypeInfo::STOCK_CHART)
// 		{
// 			if(m_nMarketType!=STOCK_CHART) ChangeStockMode(STOCK_CHART);
// 		}
// 	//	else if( strType == "B" ||		// KOSPI 200 ����, KOSPI 100 ����, �峻����, KRX, KRX���� 
// 	//		strType == "C" )			// �ڽ��� ����, �ڽ��� 50
// 		else if(cType==CMasterTypeInfo::UPJONG_CHART)
// 		{
// 			if(m_nMarketType!=UPJONG_CHART) ChangeStockMode(UPJONG_CHART);
// 		}
// 	// 	else if( strType == "4" ||		// ����, �������� 
// 	// 		strType == "5" ||		// �ɼ�
// 	// 		strType == "M" )		// ��Ÿ����
// 		else if(cType==CMasterTypeInfo::FUTOPT_CHART)
// 		{
// 			if(m_nMarketType!=FUTOPT_CHART) ChangeStockMode(FUTOPT_CHART);
// 		}
// 		else
// 		{
// 			if(m_nScreenType == Screen_ForeignChart)
// 				ChangeStockMode(FOREIGN_CHART);
// 		}
// 		//SendMessage(UMSG_SENDSHAREDCODE, (WPARAM)(LPCSTR)szRuestCode, (LPARAM)0);
// 		SendToChartMngCode(szRuestCode);
// 		return TRUE;

//	CString szCode = szRuestCode;
//
//	// ������ �����ڵ忡 �´� TR�� ���Ѵ�.
//	IMasterDataManager080331* pMDMng = (IMasterDataManager080331*)AfxGetPctrInterface(UUID_IMasterDataManager080331);
//	CString strType = pMDMng->GetCodeInfo("", 2, szCode);
//
//	long dwInData;
//	CString szInData, szTmp;
//
//	int  nSendPos=0;
//	char szSendBuffer[1024];
//	memset(szSendBuffer, 0x20, sizeof(szSendBuffer));
//
//	CString szQuerTR;
//	if( strType == "1" ||		// ��������
//		strType == "2" ||		// �ڽ�������
//		strType == "3" ||		// ��������
//		strType == "N" )		// �ֽĿ���Ʈ ELW
//	{
//		EU_p0602_InRec1 InData;
//		memset(&InData, 0x20, sizeof(EU_p0602_InRec1));
//		szQuerTR = QUERY_STRJONGMOK;
//
//		memcpy(InData.shcode, szCode, szCode.GetLength());
//
//		m_pInputBarDlg->GetInputCode(5, dwInData, szInData);
//		szTmp.Format("%8.8d", dwInData);
//		memcpy(InData.date, szTmp, szTmp.GetLength());
//
//		m_pInputBarDlg->GetInputCode(2, dwInData, szInData);
//		szTmp.Format("%4.4d", dwInData);
//		memcpy(InData.num, szTmp, szTmp.GetLength());
//
//		m_pInputBarDlg->GetInputCode(1, dwInData, szInData);
//		szTmp.Format("%3.3d", dwInData);
//		memcpy(InData.unit, szTmp, szTmp.GetLength());
//
//		m_pInputBarDlg->GetInputCode(0, dwInData, szInData);
//		InData.button[0] = szInData[0];
//
//		//�����ְ�
//		char chUseRevised = '0';
//		if(m_ChartMng.GetpChartItemSelected()->GetUseRevisedValue())	chUseRevised = '1';
//		InData.modGb[0] = chUseRevised;
//
//		m_pInputBarDlg->GetInputCode(3, dwInData, szInData);
//		InData.nowData[0] = dwInData + '0';
//
//		InData.nflag[0] = '0';
//		// Next��ȸ�̸� InData.nkey ���� ä����� �Ѵ�.
//		if(bNext)
//		{
//			InData.nflag[0] = '1';
//		}
//
//		InData.nDataGubun[0] = '0';
//		InData.Irecordgb[0] = '0';
//
//		char chUseFackedCandle = '1';
//		if(m_ChartMng.GetpChartItemSelected()->GetBNormalizedMinuteTimeRule())
//			chUseFackedCandle = '0'; // '0':����� '1':�Ǻ� 
//		InData.ImgRgb[0] = chUseFackedCandle;
//
//		CString strRecordInfo("0                                                                               ");
//		CString strFidInfo("1               000002003004005009010012013|2               000001002003004005007;");
//		memcpy(&szSendBuffer[nSendPos], strRecordInfo, strRecordInfo.GetLength());
//		nSendPos += strRecordInfo.GetLength();
//
//		memcpy(&szSendBuffer[nSendPos], &InData, sizeof(EU_p0602_InRec1));
//		nSendPos += sizeof(EU_p0602_InRec1);
//
//		memcpy(&szSendBuffer[nSendPos], strFidInfo, strFidInfo.GetLength());
//		nSendPos += strFidInfo.GetLength();
//
//		m_ChartMng.GetpChartItemSelected()->SetCodeForQuery(szRuestCode,QUERY_STRJONGMOK,TRUE);
//		
//	}
//	else if( strType == "B" ||		// KOSPI 200 ����, KOSPI 100 ����, �峻����, KRX, KRX���� 
//		strType == "C" )			// �ڽ��� ����, �ڽ��� 50
//	{
//		//EU_p0603_InRec1 InData;
//		szQuerTR = QUERY_STRUPJONG;
//	}
//	else if( strType == "4" ||		// ����, �������� 
//		strType == "5" ||		// �ɼ�
//		strType == "M" )		// ��Ÿ����
//	{
//		//EU_p0604_InRec1 InData;
//		szQuerTR = QUERY_STRFUTOPT;
//	}
//	else
//		return FALSE;
//
//	if(nSendPos>0)
//	{
//		stSend2Server st;
//		st.m_nType = 0;
//		st.m_pWnd = (CWnd*)&m_ChartMng;
//		st.m_pBuffer = (void*)szSendBuffer;
//		st.m_lLength= nSendPos;
//		st.m_strTRCode = szQuerTR;
//
//		SendMessage(UMSG_SEND2SERVER,0,(long)&st);
//	}
//
//
//	return TRUE;
}

BOOL CStdDialog::GetInputBarInput()
{

	return TRUE;
}

//�������Ʈ�� ����â.
BOOL CStdDialog::SetupChartUtil()
{
	//@Solomon�� Chart��δ� dev.
	CString szDllPath;
	szDllPath.Format("%s\\dev\\%s", m_strRootPath, "ChartUtil.dll");
	HINSTANCE hLib = ::LoadLibrary(szDllPath);
	if(!hLib) return FALSE;

	typedef  LONG  (WINAPI* DLL_UtilDlgCompSetup)(CWnd* pParent, long dwInData, long dwOutData);

	DLL_UtilDlgCompSetup fnDlgSetup = (DLL_UtilDlgCompSetup)::GetProcAddress(hLib, "UtilDlgCompSetup");
	if(!fnDlgSetup)
	{
		::FreeLibrary(hLib);
		return FALSE;
	}

	long dwInData = (long)&m_stDlgCompData;
	long dwOutData=0;
	int nRet = fnDlgSetup(this, dwInData, dwOutData);

	BOOL bRet = FALSE;
	if(nRet==IDOK)
	{
		m_pInputBarDlg->ConfirmDlgSetupData();
		bRet = TRUE;
	}

	::FreeLibrary(hLib);
	return bRet;
}

void CStdDialog::InitialBtnClick()
{
	if(!m_pLeftBarDlg) return;
	
	m_pLeftBarDlg->m_UsrAddListDlg.PostMessage(UMSG_INITIAL_BTNCLICK, 0, 0);
}



void CStdDialog::SendToChartMngCode(LPCSTR szCode)
{
	if(m_pInputBarDlg)
	{
		STINPUTCODEDATA stInputCodeData;
		stInputCodeData.m_strCode = szCode;
		stInputCodeData.m_strTime = "";
		stInputCodeData.m_nDefault = MODE_DEFAULT;
		stInputCodeData.m_nMode = CHANGE_ITEM;
		m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, MODE_DEFAULT, (LPARAM)&stInputCodeData);
	}
}

void CStdDialog::DeleteToLeftBar(LPCSTR strItemCode)
{
	if(m_pLeftBarDlg &&  m_pShareInputData->m_bCanDel)
		m_pLeftBarDlg->m_UsrAddListDlg.DelItem(strItemCode);//UMSG_DELITEM_BTNCLICK, 0, (LPARAM)strItemCode);
}

int CStdDialog::GetRequestList(CStringList* pzRCodeList, CStringList* pszRCodeNameList)
{
	int nCnt = 0;
	if(m_nScreenType==Screen_Compchart || m_nScreenType==Screen_ELWChart || m_nScreenType==Screen_ForeignChart) //����Ʈ�̸�
	{
		nCnt = m_pLeftBarDlg->m_UsrAddListDlg.GetRequestList(*pzRCodeList, *pszRCodeNameList);
		return nCnt;
	}
	else	//�������Ʈ�̸�
	{
		LPCSTR szSelGroup = m_stDlgCompData.m_szSelectGroup;

		for(POSITION pos=m_stDlgCompData.m_GroupList.GetHeadPosition(); pos;)
		{
			CDlgCompGroupData* pGroupData = m_stDlgCompData.m_GroupList.GetNext(pos);
			if(pGroupData->m_szGroupName.Compare(szSelGroup)==0)
			{
				for(POSITION jPos=pGroupData->m_ItemList.GetHeadPosition(); jPos;)
				{
					CDlgCompSubItem* pItem = pGroupData->m_ItemList.GetNext(jPos);
					pzRCodeList->AddTail(pItem->m_szCode);
					pszRCodeNameList->AddTail(pItem->m_szCodeName);
				}
				//nCnt = pzRCodeList->;
			}
		}
		return pzRCodeList->GetCount();
	}
}

//void CStdDialog::RequestNext()
//{
//	int nCnt = m_szRCodeList.GetCount();
//	if(nCnt>0)
//	{
//		CString szCode = m_szRCodeList.RemoveHead();
//		RequestTRFormCode(szCode);
//	}
//}

void CStdDialog::FileDelete()
{
	CString szConfigFile;
	szConfigFile = m_strUserDir + "\\" + m_strScreenNo + "_" + ENVINFOFILE;
	::DeleteFile(szConfigFile);

	szConfigFile = m_strUserDir + "\\" + m_strScreenNo + "_" + "ChartInfo_0000.cht";
	::DeleteFile(szConfigFile);

	szConfigFile = m_strUserDir + "\\IndiCond_" + m_strScreenNo + "_" + "ChartInfo_0000.cht";
	::DeleteFile(szConfigFile);
}

#define getPALETTE 0x01 
#define winix_GetColor(nIdx)	(COLORREF)::SendMessage(m_hPlatform, WM_USER, MAKEWPARAM(getPALETTE, 0), nIdx);

LRESULT CStdDialog::OnChangeSkinColor(WPARAM wParam, LPARAM lParam)
{
	m_clrTabBkgrnd = m_clrBackground;
//	m_clrOLBkgrnd = winix_GetColor(9);
//	m_clrOLOutLine = winix_GetColor(10);
	
	g_pPtUtilMng->GetCurrentPalette();
	
	ChangeSkinColor(m_clrBackground);
	return 0;
}

COLORREF CStdDialog::GetBkgrndColor()
{
	return winix_GetColor(clridx_BackGround);
}

LRESULT CStdDialog::OnGetSkinColor(WPARAM wParam, LPARAM lParam)
{
	int nColorType = (int)wParam;
	if(nColorType == 0)		// ����
		return (LRESULT)GetBkgrndColor();
	else if(nColorType == 1)		// Tab
	{
		STTabColor* pTabColor = (STTabColor*)lParam;
		if(pTabColor)
		{
// 			pTabColor->clrTabSelText = m_clrTabSelText;
// 			pTabColor->clrTabNorText = m_clrTabNorText;
// 			pTabColor->clrTabBkgrnd = m_clrTabBkgrnd;
// 			pTabColor->clrTabSelGDStt = m_clrTabSelGDStt;
// 			pTabColor->clrTabSelGDEnd = m_clrTabSelGDEnd;
// 			pTabColor->clrTabNorGDStt = m_clrTabNorGDStt;
// 			pTabColor->clrTabNorGDEnd = m_clrTabNorGDEnd;

			pTabColor->clrTabSelText = winix_GetColor(1);
			pTabColor->clrTabNorText = winix_GetColor(2);
			pTabColor->clrTabBkgrnd  = winix_GetColor(clridx_BackGround);
			
			g_pPtUtilMng->GetTabColor("", 0, pTabColor->clrTabSelGDStt, pTabColor->clrTabSelGDEnd);
			g_pPtUtilMng->GetTabColor("", 1, pTabColor->clrTabNorGDStt, pTabColor->clrTabNorGDEnd);
			return TRUE;
		}
		return FALSE;
	}
	return CLR_INPUTBAR_BKGRND;
}

void CStdDialog::ChangeSkinColor(COLORREF clrBackground)
{
	if(m_pLeftBarDlg) m_pLeftBarDlg->ChangeSkinColor(clrBackground);
	m_pInputBarDlg->ChangeSkinColor(clrBackground);
	m_pExtInputBarDlg->ChangeSkinColor(clrBackground);
//	m_pInputBarDlg->ShowWindow(SW_HIDE);
//	m_pInputBarDlg->ShowWindow(SW_SHOW);
}

//-->@080728-001 ��ȸ������ ��������.
// ������ ������ Ű���� : SCD(Start Close Data)
void CStdDialog::GetSCDDataFromBosomsg(CString szKey, CString strData, LPCSTR szTR)
{
	int nFind = strData.Find("@SCD=");
	CString strSCD;
	if(szKey.GetLength()>0 && nFind>=0)
	{
		strData = strData.Mid(nFind+5);
		nFind = strData.Find("@");
		if(nFind>0) strData = strData.Left(nFind);

		if(strcmp(szTR, QUERY_STRJONGMOK)==0)
			m_mapSCDate.SetAt(szKey, strData);
		else
		{
			strSCD.Format("%ld", atol(strData));
			m_mapSCDate.SetAt(szKey, strData);
		}
	}
}

void CStdDialog::OnCodeScdClear(CString strCode, BOOL bAddMode)
{
	m_mapSCDate.RemoveKey(strCode);
}

BOOL CStdDialog::GetSCDData(CString szCode, CString &rValue)
{
	szCode.TrimRight();
	return m_mapSCDate.Lookup(szCode, rValue);
}

// ����ÿ� m_mapSCDate.RemoveAll(..)
// �� ���񸶴� ����� ó����ƾ �ʿ�.
//<--

int CStdDialog::GetMarketTypeFromCode(CString strItemCode, int& nCodeType) 
{
	int nRetType;
	nCodeType = -1;
	if(strItemCode.IsEmpty())
		return -1;

	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	if (pMasterDataManager)
	{
		nRetType = pMasterDataManager->GetMarketTypeFromCode(strItemCode);
		nCodeType = pMasterDataManager->GetCodeTypeFromCode(strItemCode);
	}

	return nRetType;
}

CString CStdDialog::GetItemNameFromItemCode(CString strItemCode, BOOL bPreFix)
{
	CString strCodeName = _T("");
	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	if(pMasterDataManager)
	{
		strCodeName = pMasterDataManager->GetCodeName(strItemCode);
		
		if (!bPreFix)
			strCodeName.Replace("&&", "&");
	}
	
	return strCodeName;
}

void CStdDialog::GetELWIndexCode(CString strELWCode, CString& strIndexCode, CString& strIndexName)
{
	if(m_pInputBarDlg)
		m_pInputBarDlg->m_pDataManager->GetELWIndexCode(strELWCode, strIndexCode, strIndexName);
}

BOOL CStdDialog::TrackPopupMenu() 
{
	return ::PostMessage(m_hViewInMain, WM_CONTEXTMENU, 0, 0);
}

LRESULT CStdDialog::OnUserInfo(WPARAM wp, LPARAM lp)
{
	BYTE bt = LOBYTE(LOWORD(wp));
	switch (bt)
	{
	case USERINFO_SET:
		{
			WORD dwKey = HIWORD(wp);
			if(dwKey==0)
			{
				IPartnerManager* pPtManager = (IPartnerManager*)lp;
				DoSetPartnerSite(pPtManager);
			}
		}
		break;
	}

	return 0L;
}

void CStdDialog::_DoCheckDaultDataFile(LPCSTR szScreenNo, LPCSTR szUserFile)
{
// 	{
// 		CString szFilePath;
// 		szFilePath = m_szDataDir + "\\" + m_strScreenNo + "_" + ENVINFOFLUCTFILE;
// 		::CopyFile(szFilePath, m_FnHandle.m_szDataFile, FALSE);
// 	}

	CFileFind file;
	if(file.FindFile(szUserFile)==FALSE)
	{
		_DoCopyDefaultToUser(szScreenNo);
	}
}

void CStdDialog::_DoCopyDefaultToUser(LPCSTR szSect)
{
	CString szChartCfg;
	szChartCfg.Format("%s\\Icss\\ChartDef.cfg", m_strRootPath);
	
	int nCnt = ::GetPrivateProfileInt(szSect, "Count", 0, szChartCfg);
	
	char aKey[4], aBuff[MAX_PATH];
	CString szDefFile, szUserFile;
	for(int i=0; i<nCnt; i++)
	{
		sprintf(aKey, "%d", i);
		if(::GetPrivateProfileString(szSect, aKey, "", aBuff, MAX_PATH, szChartCfg)>0)
		{			
			szDefFile.Format("%s\\Icss\\%s", m_strRootPath, aBuff);
			szUserFile.Format("%s\\%s", m_strUserDir, aBuff);
			::CopyFile(szDefFile, szUserFile, FALSE);
		}
	}
}

void CStdDialog::_DoCheckRecentFuture(LPCSTR szDataFile)
{
	CStringArray arCode, arName, arEngName;
	CStringArray *parCode = &arCode, *parName = &arName, *parEngName = &arEngName;
	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	if(pMasterDataManager)
	{
		pMasterDataManager->GetFutureJongMst(parCode, parName, parEngName);
		if(arCode.GetSize()>0)
		{
			CString rCode;
			rCode = arCode.GetAt(0);
			
			::WritePrivateProfileString("FGroup00", "szCode01", rCode, szDataFile);
			//::WritePrivateProfileString("Fluctuation", "szCode01", rCode, szDataFile);
		}
	}
}

void CStdDialog::GetFirstCode(CString &rString)
{
	if(m_pLeftBarDlg &&  m_pShareInputData->m_bCanDel)
		rString = (char*)m_pLeftBarDlg->m_UsrAddListDlg.SendMessage(RMSG_GETINFO, 10, 0);
}
