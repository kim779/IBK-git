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

#include "../COMMONTR/ChartRealConverter.h"
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
#include "../CommonTR/SM_fx112.h"					// FX
#include "../CommonTR/SM_CheJanStruct.h"			// ü�� - �ֹ������� ���
#include "../Include_Chart/Dll_Load/IMetaTable.h"	// (2008/9/13 - Seung-Won, Bae) for Multi-Language
#include "../../inc/RealUpdate.h"
#include "../../inc/IAUDataMonitorMng.h"	// UUID_IAUDataMonitorManager : �ݹ��Լ� �޴µ� ���

#include "../../inc/AZUtil_Ctrl.h"			// OSüũ��ƾ. IsSystemChart()���� ���
#include "TabSystemTradingEx.h"				// �ý���Ʈ���̵� ���� ����/�ҷ�����

#include "../../inc/RMsgUserInfo.h"

#ifdef __MULTI_LANG__
	#include "../Include_Chart/Dll_Load/IMetaTable.h"	//@Solomon-MultiLang:091117
#endif

//<<20100308_JS.Kim �ַθ�
/*
#include "../../../Common/CommStructDef.h"
// FX��Ʈ �ֹ� ���� - onlyojt (20091204)
#include "../../../TRDll/KTB_CheJanStruct.h"
//<<< 20091211 by kmlee �ֹ�����
#include "../../../TrDll/TrHeader/signalStruct.h"
#include "../../../common/KHresource.h"
#include "../Common_ST/STConfigEnv.h"
__declspec(dllimport) CString KSLink_GetAccInfo (CString strAccount, int nType);
__declspec(dllimport) long KSLink_GetEnvInfo (int nType);
//>>>
*/
#define CONCLUSION_YES 0   //ü��
#define CONCLUSION_NO  1   //��ü��

#define ORDERTYPE_MARKET		0 //���尡
#define ORDERTYPE_ENTRY			1 //������
#define ORDERTYPE_CHANGE		2 //����
#define ORDERTYPE_CANCEL		3 //���
#define ORDERTYPE_STOP			4 //Stop/Limit ����
#define ORDERTYPE_LIMIT			5 //Stop/Limit ���
#define ORDERTYPE_CLEAR			6 //�ֹ�û��
#define ORDERTYPE_CON_STOP		7 //��û�� Stop/Limit ����
#define ORDERTYPE_CON_LIMIT		8 //��û�� Stop/Limit ���

#define ORDERTYPE_STOP_ONLY		9	 //Stop ���� ��ü��
#define ORDERTYPE_LIMIT_ONLY	10	//Limit ���� ��ü��
#define ORDERTYPE_CON_STOP_ONLY	11	//Stop ���� ��û��
#define ORDERTYPE_CON_LIMIT_ONLY 12	//Limit ���� ��û��
#define ORDERTYPE_CROSS_ENTRY	 13	//�������� 

#define ORDERTYPE_ENTRY2		14 //������(�ż�/�ŵ� ���ÿ� ������ �ֹ�â�� ���� ���)
//>>

//	�߰�... �ֶ󸮽� ȯ�濡�� ������ ������ �Ѵ�. �� �����̰� �����
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")		// htonl, ntohl


#define	WMU_GET_KEY_INFO_ARRAY		WM_USER + 2253	// �ش� AUP���� �ԷµǾ� �ִ� Key���� ����� �����´�. LPARAM�� CStringArray *

#define	WIDTH_SIDEBAR		3

const UINT RMSG_GET_SCREENTYPE = ::RegisterWindowMessage(_T("RMSG_GET_SCREENTYPE"));
const UINT RMSG_GET_MARKETTYPE = ::RegisterWindowMessage(_T("RMSG_GET_MARKETTYPE"));
const UINT RMSG_GET_LINKINFODATA = ::RegisterWindowMessage(_T("RMSG_GET_LINKINFODATA"));

//<<< 20091215 by kmlee for �ֹ�����
const UINT RMSG_CHANGE_CHARTACCOUNT = ::RegisterWindowMessage(_T("RMSG_CHANGE_CHARTACCOUNT"));
const UINT RMSG_GET_CHARTACCOUNT = ::RegisterWindowMessage(_T("RMSG_GET_CHARTACCOUNT"));
const UINT RMSG_ACCOUNT_INIT = ::RegisterWindowMessage(_T("RMSG_ACCOUNT_INIT"));
//>>>

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

#define WIDTH_SIDEBAR_EXTEND		170
#define WIDTH_SIDEBAR_COLLAPSE		10
#define	REAL_FX_SISE				0xAB

//<<< 20091230 by kmlee  for �ֹ�����
#define		ORD_TYPE_MARKETORDER		0		// ���尡
#define		ORD_TYPE_ORDER				1		// ������
#define		ORD_TYPE_MODIFY				2		// ����
#define		ORD_TYPE_CANCEL				3		// ���
#define		ORD_TYPE_STOPLIMIT_APLY1	4		// stop/limit ���� (��ü��)
#define		ORD_TYPE_STOPLIMIT_CANCEL1	5		// stop/limit ��� (��ü��)
#define		ORD_TYPE_CLEAR				6		// û��
#define		ORD_TYPE_STOPLIMIT_APLY2	7		// stop/limit ���� (��û��)
#define		ORD_TYPE_STOPLIMIT_CANCEL2	8		// stop/limit ��� (��û��)
#define		ORD_TYPE_STOP_APPLY3		9		// Stop ����(��ü��)
#define		ORD_TYPE_LIMIT_APPLY3		10		// Limit ����(��ü��)
#define		ORD_TYPE_STOP_APPLY4		11		// Stop ����(��û��)
#define		ORD_TYPE_LIMIT_APPLY4		12		// Limit ����(��û��)
#define		ORD_TYPE_ORDER_REVERSE		13		// ��������
//>>>

extern HINSTANCE g_hInstance;
//<<< 20091214 by kmlee for �ֹ�����
extern void dll_GetUnitInfo( CString strCode , CString strMarket, CString& strPrecision, CString& strTickSize );
//

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
// Static CimageList ============================================================>
	int		   CStdDialog::m_static_nChartCnt4ImgList = 0;

	CImageList CStdDialog::m_ImgList_W15H15;				//�� 15�� ���� 15�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W20H20;				//�� 20�� ���� 19�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W20H20_Tool;			//�� 20�� ���� 19�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W46H20;				//�� 46�� ���� 19�� ��ư �̹�������Ʈ	
	CImageList CStdDialog::m_ImgList_W50H20;				//�� 50�� ���� 19�� ��ư �̹�������Ʈ

	CImageList CStdDialog::m_ImgList_W151H21;				//�� 151�� ���� 21�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W28H22;				//�� 28�� ���� 22�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W17H20;				//�� 17�� ���� 20�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W43H20;				//�� 43�� ���� 20�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W58H20;				//�� 58�� ���� 20�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W72H20;				//�� 72�� ���� 20�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W46H20_Gwan;			//�� 46�� ���� 20�� ��ư �̹�������Ʈ
	CImageList CStdDialog::m_ImgList_W8H25;					//�� 8�� ���� 25�� ��ư �̹�������Ʈ
// <==============================================================================	

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
	m_nLeftBarType		= 0;
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
	m_hViewInMain		= NULL;
	m_lWidthOriginal	= 0;
	m_lHeightOriginal	= 0;
	m_bHideChart		= FALSE;
	m_bInitResize		= FALSE;

	m_hSocket			= NULL;
	m_hMainFrame		= NULL;

	m_bShowDataListWnd	= FALSE;			// ������ �����츦 ������ ����
	m_hReplayChartLib	= NULL;				// ����  Lib Handle

	m_nMarketType		= CMasterTypeInfo::FX_CHART;

	m_bActiveDataWindow = false;
	m_pDataWindow		= NULL;
	m_pRqWndFromTab		= NULL;

	m_pWndReplayLib		= NULL;				//���� ���� Wnd Pointer


	memset(&m_ChartCfgInfo, 0x00, sizeof(CHART_CONFIG_INFO));
	
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

	m_nExtendState			= 0;				// 20061212 ������ ����, grid column Ȯ�� btn �߰�
	m_cMUM					= 'P';
	m_bPreMarket			= FALSE;			// ���� ������ ó�� by 20070226 by LYH
	m_cUT = '1';								// ƽ������ ���� ��� Ŭ���� ���� ó��
	m_strRecvCode			= "";
	m_strSvcID				= "CP986100";		// 2007.05.02 by LYH ���� ��� ���� ���� ID �߰�

	m_pDrdsLib				= NULL;				// �ǽð�..
	m_pDlgGlass				= NULL;				// ������ ���� - ojtaso (20080519)

	g_pDebugMng				= NULL;
	g_pPtUtilMng			= NULL;

	::memset(&m_stTrInfo2, 0x00, sizeof(m_stTrInfo2));

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
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_SPLITTER_MOVED, OnWmSplitterMoved )	
	//ON_MESSAGE( WM_GET_BROD, OnGetBroad )					//20100308_JS.Kim �ַθ�
	ON_REGISTERED_MESSAGE(RMS_ANALYSIS_FUNC			, OnByPass_ExportToolOption ) //2005. 07. 28 added by sy,nam	
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_REAL			, OnReplayDrds )
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_FAIL			, OnReplayFail )
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
	//<<< 20091215 by kmlee
	ON_REGISTERED_MESSAGE(RMSG_CHANGE_CHARTACCOUNT, OnChangeAccount)
	//>>>
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

/*
	Function : ��Ƽ ��Ʈ ȭ�� ���̾˷α� �ʱ�ȭ								
	Paramter :																
	Return	:																
	Author	: By DRFN														
	Comments	: FX Margin �ϸ鼭 �ҽ� ���� (�ѱ��Ҹ�ġ �����)				

	ok..
*/
BOOL CStdDialog::OnInitDialog() 
{
	CExOCXDLL::OnInitDialog ();

	AfxEnableControlContainer();

	m_strHwndKey.Format("%d", (long)GetSafeHwnd());

	SetInitInfo ();						// User ID �� ������� �ʱ�ȭ

	InitImageList ();					// �ֿ켱���� ImageList �ε�

	SetBaseInterface ();				// PCTR �� �⺻ Interface ����

	m_nChartType = MULTI_CHART;

	CString strScreenType, strMarketType;

	if (m_nScreenID == atoi("9041"))	//@Solomon	FX MultiChart
	{
		m_nMarketType = CMasterTypeInfo::FX_CHART;
		m_bSmallScreenMode = FALSE;
		m_nChartType = MULTI_CHART;
		m_bMultiItemChart = TRUE;
	}
	else if (m_nScreenID == atoi("9043"))	//@Solomon	FX MiniChart
	{
		m_nMarketType = CMasterTypeInfo::FX_CHART;
		m_bSmallScreenMode = FALSE;
		m_nChartType = MINI_MULTI_CHART;
		m_bMultiItemChart = TRUE;
	}


#ifdef _WRITELOG
	CreateLogFile();		// Log - ojtaso (20080107)
#endif

	IPartnerSvrManager* pPtSvrMng = (IPartnerSvrManager*)AfxGetPctrInterface(UUID_IPartnerSvrManager);
	if (pPtSvrMng)
		g_pPtUtilMng = (IPartnerUtilManager*)pPtSvrMng->GetInerInterface(LUID_IPartnerUtilManager);

	FormLoad();				// ��Ʈ �� �ε�, ȭ�鿡 �ʿ��� ���� ��� �����Ͽ� �ø���..
	
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

/*
	ok
*/
void CStdDialog::ChangeStockMode (int type)
{
	//<< �ַθ�. FX ���� ��Ʈ�̹Ƿ� ���� ���� �ʿ����
	return;
	//>>

	if(m_nChartType == MINI_MULTI_CHART || m_nChartType == MINI_SINGLE_CHART)
	{
		ChangeMiniStockMode(type);
		return;
	}

	CString strTemp;
	UnLoad_ReplayChart_Lib();		//���� Lib UnLoad
	
	m_nMarketType =  type;

	//------------------------------------------------------------------------------
	if (m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
	{
		m_pInputBarDlg->SetMarketType (m_nMarketType);
		m_pInputBarDlg->MoveWindow (GetRect (ID_INPUTBAR,TRUE));
		//2005. 06. 22 ���� �ڵ带 ���� ���� ���� �����Ѵ�.
		BOOL bRequestAPData = FALSE;
	}

	//------------------------------------------------------------------------------
	// 2005. 05. 16 chart �����ڸ� �ѱ��.
	//------------------------------------------------------------------------------
	if (m_pExtInputBarDlg)
	{
		m_pExtInputBarDlg->SetMarketType (m_nMarketType);
		m_pExtInputBarDlg->MoveWindow (GetRect(ID_EXTINPUTBAR,TRUE));
	}
	//------------------------------------------------------------------------------
	// 2005. 05. 16 chart �����ڸ� �ѱ��.
	//------------------------------------------------------------------------------	
	m_ChartMng.SetMarketType (m_nMarketType);
	m_pInputBarDlg->ChangeMarket (m_nMarketType, FALSE, TRUE);
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

	m_strConfig = m_strUserDir + "\\" + m_strScreenNo + "_" + ENVINFOFILE;

	if(m_pInputBarDlg)
	{
		m_pInputBarDlg->m_strCfgFile = m_strConfig;

		m_pInputBarDlg->SetMarketType(m_nMarketType);	
		m_pInputBarDlg->MoveWindow(GetRect(ID_INPUTBAR,TRUE));
	}


	//------------------------------------------------------------------------------
	// 2005. 05. 16 chart �����ڸ� �ѱ��.
	//------------------------------------------------------------------------------	
	m_ChartMng.SetMarketType(m_nMarketType);
	m_pInputBarDlg->ChangeMarket(m_nMarketType, FALSE, TRUE);
}

/*
	Function : ��Ƽ ��Ʈ �� �ε�	
	Paramter : ?					
	Return	:					
	Author	: By DRFN			
	Comments	: FX Margin �ϸ鼭 �ҽ� ���� (�ѱ��Ҹ�ġ �����)
	Date     : 2008/07/10 
*/
void CStdDialog::FormLoad()
{
	m_pMsgBar = new CMessageBar;								// �޼����� ����
	m_pMsgBar->Create (CMessageBar::IDD, this);

	m_TRComm = (ITrComm2*)GetTrComm ();							// TR �� DRDS �ʱ�ȭ

	InitDrds ();												// ��� Ű �ʱ�ȭ

	m_FavoriteMng.SetPath (m_strUserDir);						// ����� ȯ�� path������
	
	if (_access (m_strUserDir, 0) == -1)						// User Directory ����
	{															// �������� : IO.h, Direct.h
		 int retval = _mkdir(m_strUserDir);
	}  

	// �������� ���	
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

		if(m_nWidthOfLeftBar)
		{
			::GetPrivateProfileString("Frame","ShowLeftBar","1",szTemp,sizeof(szTemp),m_strConfig);
			m_nLeftBarType = atoi(szTemp);
		}
		else
		{
			m_nWidthOfLeftBar = WIDTH_SIDEBAR_COLLAPSE;
		}
		::GetPrivateProfileString("Frame","ShowExtInputBarDlg","1",szTemp,sizeof(szTemp),m_strConfig);
		m_bShowExtInputBarDlg = atoi(szTemp);

		::GetPrivateProfileString("Frame","ShowToolBar", "1", szTemp, sizeof(szTemp),m_strConfig);
		m_bShowToolBar = atoi(szTemp);

//		::GetPrivateProfileString("Frame","ShowMigyulche", "1", szTemp, sizeof(szTemp),m_strConfig);
//		m_bMigyulcheDraw = atoi(szTemp);
	}
	else if (m_nChartType == SINGLE_CHART)
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

	// ���ø��� ����
	m_xSplitter1.CreateInWnd(this, ID_SPLITTER, GetRect(ID_SPLITTER));

	// ��Ʈ�Ŵ��� ����
	m_ChartMng.SetMarketType(m_nMarketType);
	m_ChartMng.m_strRootPath = m_strRootPath;
	BOOL bRetValue = m_ChartMng.Create(GetRect(ID_CHART),this, m_strUserDir, m_szImageDir,ID_CHART, (long)((CMChartInterface*)this));
	if (!bRetValue) return;
	m_ChartMng.SetBaseDesk ((long)1);            // ����ũ�� ���������� ���� ������ ���������� �⺻���� ����

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

	// ��Ų����
	//<<20100308_JS.Kim �ַθ�
	//IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 2);
	//if(pKSLibMng)
	//	m_clrBackground = DEFAULT_BKGRND_COLOR;		// �⺻��
	//else
	//	m_clrBackground = DEFAULT_BKGRND_COLOR;		// �⺻��
	m_clrBackground = DEFAULT_BKGRND_COLOR;		// �⺻��
	//>>
	int nTheme = 0;//AfxGetMainWnd()->SendMessage(WMU_GET_THEME);	
	OnChangeSkinColor(nTheme, 0);
//	ChangeSkinColor();
	Invalidate();
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

	if( m_pLeftBarDlg )
		m_pLeftBarDlg->Invalidate();
	m_ChartMng.Invalidate();
	m_xSplitter1.Invalidate();
	m_pInputBarDlg->ShowWindow(SW_HIDE);
	m_pExtInputBarDlg->Invalidate();
	m_pInputBarDlg->ShowWindow(SW_SHOW);


	if(m_ChartMng.m_pFloatingDlg != NULL)				
		m_ChartMng.m_pFloatingDlg->Invalidate();

	
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
		if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd()&&!bInit)
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
		break;
	case ID_INPUTBAR:
	case ID_MINIBAR:
		if(!m_bShowInputBarDlg)	
		{
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
		//<<< 20091223 by kmlee - ������ ����
		rect.top -= 2;
		//>>>
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
	case ID_TOOLBAR:
		if(m_pDlgToolbar->GetSafeHwnd())
		{
			rect.left = rect.right;
			rect.right = rect.right + WIDTH_TOOLBAR;
		}
		//<<< 20091223 by kmlee - ������ ����
		rect.top -= 2;
		//>>>
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
		}
		//<<20100308_JS.Kim �ַθ�
		/*
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
		//>>
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
		}
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
	CString strTemp;

	m_pPTManager = NULL;

#ifdef _WRITELOG
	WriteToStringLog("������ ��ġ ����::Begin", _T("CStdDialog"), 10);
#endif
	
	if(!m_bSmallScreenMode)		// DRDS ����
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
	

#ifdef _WRITELOG
	WriteToStringLog("������ ��ġ ����::End", _T("CStdDialog"), 10);
#endif
	
#ifdef _WRITELOG
	WriteToStringLog("��Ʈ�Ŵ��� ����::Begin", _T("CStdDialog"), 10);
#endif

	if(m_ChartMng.GetSafeHwnd())
		m_ChartMng.DestroyWindow();

#ifdef _WRITELOG
	WriteToStringLog("��Ʈ�Ŵ��� ����::End", _T("CStdDialog"), 10);
#endif
	
#ifdef _WRITELOG
	WriteToStringLog("���м� ����::Begin", _T("CStdDialog"), 10);
#endif

	if(m_xSplitter1.GetSafeHwnd())
	{
		m_xSplitter1.DetachAllCtrls();
		m_xSplitter1.DestroyWindow();
	}

#ifdef _WRITELOG
	WriteToStringLog("���м� ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	WriteToStringLog("�������� ����::Begin", _T("CStdDialog"), 10);
#endif

	if(m_pLeftBarDlg)
	{
		m_pLeftBarDlg->DestroyWindow();
		delete m_pLeftBarDlg;
		m_pLeftBarDlg = NULL;
	}

#ifdef _WRITELOG
	WriteToStringLog("�������� ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	WriteToStringLog("��ǲ�� ����::Begin", _T("CStdDialog"), 10);
#endif

	if(m_pInputBarDlg)
	{
		m_pInputBarDlg->DestroyWindow();
		delete m_pInputBarDlg;
		m_pInputBarDlg = NULL;
	}

#ifdef _WRITELOG
	WriteToStringLog("��ǲ�� ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	WriteToStringLog("��ǲ��2 ����::Begin", _T("CStdDialog"), 10);
#endif

	if(m_pExtInputBarDlg)
	{
		m_pExtInputBarDlg->DestroyWindow();
		delete m_pExtInputBarDlg;
		m_pExtInputBarDlg = NULL;
	}

#ifdef _WRITELOG
	WriteToStringLog("��ǲ��2 ����::End", _T("CStdDialog"), 10);
#endif
	
	
#ifdef _WRITELOG
	WriteToStringLog("�޼����� ����::Begin", _T("CStdDialog"), 10);
#endif

	if(m_pMsgBar)
	{
		m_pMsgBar->DestroyWindow();
		delete m_pMsgBar;
		m_pMsgBar = NULL;
	}	

#ifdef _WRITELOG
	WriteToStringLog("�޼����� ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	WriteToStringLog("���� ���̺귯�� ����::Begin", _T("CStdDialog"), 10);
#endif

	UnLoad_ReplayChart_Lib();	

#ifdef _WRITELOG
	WriteToStringLog("���� ���̺귯�� ����::End", _T("CStdDialog"), 10);
#endif

	POSITION pos = NULL;	

#ifdef _WRITELOG
	WriteToStringLog("DRDS ����::Begin", _T("CStdDialog"), 10);
#endif
	
	//<<20100308_JS.Kim �ַθ�
	//if ( m_pDrdsLib )
	//	m_pDrdsLib->DrdsUnadviseAll(m_hWnd);	// �ڵ鿡 ��ϵǾ��ִ� ������ ���� ������Ų��
	//AllCodeUnAdvise ();							// Real ���� ����..
	//>>

	
#ifdef _WRITELOG
	WriteToStringLog("DRDS ����::End", _T("CStdDialog"), 10);
#endif
	

#ifdef _WRITELOG
	WriteToStringLog("����Ÿ ������ ����::Begin", _T("CStdDialog"), 10);
#endif
	
	if(m_pDataWindow)
	{
		m_pDataWindow->DestroyWindow();
		delete m_pDataWindow;
		m_pDataWindow = NULL;
	}
	
#ifdef _WRITELOG
	WriteToStringLog("����Ÿ ������ ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	WriteToStringLog("���� ����::Begin", _T("CStdDialog"), 10);
#endif
	
	if(m_pDlgToolbar)
	{
		m_pDlgToolbar->DestroyWindow();
		delete m_pDlgToolbar;
		m_pDlgToolbar = NULL;
	}
	
#ifdef _WRITELOG
	WriteToStringLog("���� ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	WriteToStringLog("�̹�������Ʈ ����::Begin", _T("CStdDialog"), 10);
#endif
	
	DeleteAllImageList();
	
#ifdef _WRITELOG
	WriteToStringLog("�̹�������Ʈ ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	WriteToStringLog("Ŀ�� ����::Begin", _T("CStdDialog"), 10);
#endif
	
	if(m_hCursor)
		::DeleteObject(m_hCursor);
	
#ifdef _WRITELOG
	WriteToStringLog("Ŀ�� ����::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	CloseLogFile();
#endif
	
	if(m_pDlgGlass) // ������ ���� - ojtaso (20080519)
	{
		delete m_pDlgGlass;
		m_pDlgGlass = NULL;
	}

	m_memoryDC.DeleteDC();
	m_bufferBitmap.DeleteObject();

	//<<< 20100113 by kmlee ü���ܰ� ���̺귯�� ����..
	//<<20100308_JS.Kim �ַθ�
	//IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 2);
	//if (pKSLibMng)
	//	pKSLibMng->Lib_KSLink_SetGridCheJanLibFree (this);
	//>>
	//>>>

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

CString CStdDialog::MakeChartData (STDR_FID* pStdrFid, int& nCount, int dwTRDataLen, int nMarketType, int nType, double dPrePrice, int nUnitType/*=WON_TYPE*/)
{
//<<20100308_JS.Kim �ַθ�
/*
#ifdef _WRITELOG
	WriteToStringLog("�����κ��� ���� Data�� ��Ʈ Data�� ����::Begin", m_strRecvCode, m_strRecvCode.GetLength());
#endif

	if(nCount <= 0)
		return "";

	char* szChartData = NULL;
	CString strChartData;
	int nBufferLength = 0;
	if(nMarketType == FX_CHART)
	{
		SM_fx112_OutRec2 eu0602OutRec2;
		nBufferLength = sizeof(SM_fx112_OutRec2) * nCount + 1;
		szChartData = strChartData.GetBuffer(nBufferLength);
		CString strData;
		double dRate = 0.;
		for(int i = 0; i < nCount; ++i)
		{
			if(nUnitType == PERCENT_TYPE)
			{
				memcpy(eu0602OutRec2.date, pStdrFid->vvRpValueList[nCount - i][0], sizeof(eu0602OutRec2.date));
				strData.Format("%10.2f", atof(pStdrFid->vvRpValueList[nCount - i][1]));
				memcpy(eu0602OutRec2.open, strData, sizeof(eu0602OutRec2.open));
				strData.Format("%10.2f", atof(pStdrFid->vvRpValueList[nCount - i][2]));
				memcpy(eu0602OutRec2.high, strData, sizeof(eu0602OutRec2.high));
				strData.Format("%10.2f", atof(pStdrFid->vvRpValueList[nCount - i][3]));
				memcpy(eu0602OutRec2.low, strData, sizeof(eu0602OutRec2.low));
				strData.Format("%10.2f", atof(pStdrFid->vvRpValueList[nCount - i][4]));
				memcpy(eu0602OutRec2.close, strData, sizeof(eu0602OutRec2.close));
				memcpy(eu0602OutRec2.volume, pStdrFid->vvRpValueList[nCount - i][5], sizeof(eu0602OutRec2.volume));
				memcpy(eu0602OutRec2.rights, pStdrFid->vvRpValueList[nCount - i][6], sizeof(eu0602OutRec2.rights));
			}
			else if(nUnitType == DOLLAR_TYPE)
			{
				memcpy(eu0602OutRec2.date, pStdrFid->vvRpValueList[nCount - i][0], sizeof(eu0602OutRec2.date));
				memcpy(eu0602OutRec2.open, pStdrFid->vvRpValueList[nCount - i][1], sizeof(eu0602OutRec2.open));
				memcpy(eu0602OutRec2.high, pStdrFid->vvRpValueList[nCount - i][2], sizeof(eu0602OutRec2.high));
				memcpy(eu0602OutRec2.low, pStdrFid->vvRpValueList[nCount - i][3], sizeof(eu0602OutRec2.low));
				memcpy(eu0602OutRec2.close, pStdrFid->vvRpValueList[nCount - i][4], sizeof(eu0602OutRec2.close));
				memcpy(eu0602OutRec2.volume, pStdrFid->vvRpValueList[nCount - i][5], sizeof(eu0602OutRec2.volume));
				memcpy(eu0602OutRec2.rights, pStdrFid->vvRpValueList[nCount - i][6], sizeof(eu0602OutRec2.rights));
			}
			else	// nUnitType == WON_TYPE
			{
				memcpy(eu0602OutRec2.date, pStdrFid->vvRpValueList[nCount - i][0], sizeof(eu0602OutRec2.date));
				memcpy(eu0602OutRec2.open, pStdrFid->vvRpValueList[nCount - i][1], sizeof(eu0602OutRec2.open));
				memcpy(eu0602OutRec2.high, pStdrFid->vvRpValueList[nCount - i][2], sizeof(eu0602OutRec2.high));
				memcpy(eu0602OutRec2.low, pStdrFid->vvRpValueList[nCount - i][3], sizeof(eu0602OutRec2.low));
				memcpy(eu0602OutRec2.close, pStdrFid->vvRpValueList[nCount - i][4], sizeof(eu0602OutRec2.close));
				memcpy(eu0602OutRec2.volume, pStdrFid->vvRpValueList[nCount - i][5], sizeof(eu0602OutRec2.volume));
				memcpy(eu0602OutRec2.rights, pStdrFid->vvRpValueList[nCount - i][6], sizeof(eu0602OutRec2.rights));
			}

			memcpy(szChartData, (LPCSTR)&eu0602OutRec2, sizeof(SM_fx112_OutRec2));
			szChartData += sizeof(SM_fx112_OutRec2);
		}
	}

	strChartData.ReleaseBuffer();
	return strChartData;
	*/
	return "";
	//>>
}

//aTRData : Recieve Data, nIdx:Start Index, nCount: count, dwTRDataLen: Data Length, nMarketType: Stock, Upjong, Future Type, nType : Tick/Min...
CString CStdDialog::MakeChartData(LPVOID aTRData, int nIdx, int& nCount, int dwTRDataLen, int nMarketType, int nType, double dPrePrice, int nUnitType/*=WON_TYPE*/)
{
//<<20100308_JS.Kim �ַθ�
	/*
#ifdef _WRITELOG
	WriteToStringLog("�����κ��� ���� Data�� ��Ʈ Data�� ����::Begin", m_strRecvCode, m_strRecvCode.GetLength());
#endif

	m_bPreMarket = FALSE;

	if(nCount < 0)
		nCount = 400;

	CString strData, strOut, strOut2;
	CString strHigh, strClose;

	double rate;
	int nPriceType = C_DT_ULONG, nVolType = C_DT_ULONG, nDotPos = 0;

	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
	int nLength=0, nExp=0;

	double dDollar = atof(m_strDollar);
	
	int nRights = 0;
	for(int i=0; i<nCount; i++)
	{
		if(nIdx>=dwTRDataLen)
		{
			nCount = i;
			break;
		}

		strOut = "";
		//��¥
		strData = m_TRComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 8, &nIdx, 0, TRUE);
		// Intra_Ticks �߰� - ojtaso (20071227)
		if(nType == TICK_DATA_CHART)
		{
			strData.Format("%2.2s", strData.Right(2));
			CString strTime = m_TRComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 8, &nIdx, 0, TRUE);
			if(atoi(strTime.Mid(4,6)) != 0)
			{
				int nHour = atoi(strTime.Left(2));
				int nMin  = atoi(strTime.Mid(2,2));
				int nSec  = atoi(strTime.Mid(4,2));

				strTime.Format("%02d%02d%02d", nHour, nMin, nSec);
			}
			strData += strTime.Left(6);

		}
		else if(nType == MIN_DATA_CHART)
		{
			strData.Format("%4.4s", strData.Mid(4, 4));
			CString strTime = m_TRComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 8, &nIdx, 0, TRUE);
			if(atoi(strTime.Mid(4,6)) != 0)
			{
				int nHour = atoi(strTime.Mid(0,2));
				int nMin  = atoi(strTime.Mid(2,2)) + 1;
				if(nMin>=60)
				{
					nHour += 1;
					nMin = 0;
				}
				strTime.Format("%02d%02d", nHour, nMin);
			}
			strData += strTime.Mid(0,4);
		}
		strOut += strData;

		if(nMarketType == FOREIGN_CHART && nType == TICK_DATA_CHART)
		{
			//�ð�,��,����,����
			strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
			strOut += strData;
			strOut += strData;
			strOut += strData;
			strOut += strData;

			if(i==0)
			{
				strHigh = strData;
				strClose = strData;
			}
			strOut += "          ";
		}
		else
		{
			if(nUnitType == PERCENT_TYPE && dPrePrice != 0)
			{
				//�ð�
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				rate = (atof(strData)/dPrePrice - 1) * 100.0;
				strData.Format("%10.2f", rate);
				strOut += strData;

				//��
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				if(i==0)
					strHigh = strData;

				rate = (atof(strData)/dPrePrice - 1) * 100.0;
				strData.Format("%10.2f", rate);
				strOut += strData;

				//����
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				rate = (atof(strData)/dPrePrice - 1) * 100.0;
				strData.Format("%10.2f", rate);
				strOut += strData;

				//����
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				if(i==0)
					strClose = strData;

				rate = (atof(strData)/dPrePrice - 1) * 100.0;
				strData.Format("%10.2f", rate);
				strOut += strData;
			}
			else if(nUnitType == DOLLAR_TYPE && dDollar != 0)
			{
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				rate = atof(strData)/dDollar;
				strData.Format("%10.2f", rate);
				strOut += strData;

				//��
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				if(i==0)
					strHigh = strData;

				rate = atof(strData)/dDollar;
				strData.Format("%10.2f", rate);
				strOut += strData;

				//����
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				rate = atof(strData)/dDollar;
				strData.Format("%10.2f", rate);
				strOut += strData;

				//����
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				if(i==0)
					strClose = strData;

				rate = atof(strData)/dDollar;
				strData.Format("%10.2f", rate);
				strOut += strData;
			}
			else
			{
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				strOut += strData;

				//��
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				if(i==0)
					strHigh = strData;

				strOut += strData;

				//����
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				strOut += strData;

				//����
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				if(i==0)
					strClose = strData;
				strOut += strData;
			}
			//}
			//�ŷ���
			strData = m_TRComm->ApplyByteOrder(nVolType, (char *)aTRData+nIdx, 10, &nIdx);
			strOut += strData;
		}
		//�Ǹ���
		if(nMarketType == FX_CHART)
		{
			if(nType == DAY_DATA_CHART)
			{
				strData = m_TRComm->ApplyByteOrder(C_DT_BYTE, (char *)aTRData+nIdx, 3, &nIdx);
				nRights = atoi(strData);
				if(nRights > 0)
				{
					switch(nRights)
					{
					case 10:
					case 20:
					case 30:
					case 40:
					case 50:
					case 60:
					case 70:
						strData.Format("1%d", nRights);
						strOut += strData;
						break;
					case 61:
						strOut += "260";
						break;
					case 71:
						strOut += "270";
						break;
					case 81:
						strOut += "280";
						break;
					case 91:
						strOut += "110";
						break;
					default:
						strOut += "   ";
						break;
					}
				}
				else
					strOut += "   ";
				
			}
			else
			{
				strOut += "   ";
			}
		}
		strOut2 = strOut + strOut2;
	}
	//20070226 by LYH
	//{���� ������ ó��
	if( atoi(strHigh) < atoi(strClose) )
		m_bPreMarket = TRUE;
	//}}

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("�����κ��� ���� Data�� ��Ʈ Data�� ����::End", m_strRecvCode, m_strRecvCode.GetLength());
#endif

	return strOut2;
	*/
	return "";
	//>>
}

/*
	����Ÿ�� ���� �޾Ƽ� ����. ok

	LPCSTR szTR : TR �ڵ�
	char* pData	: TR ��� buffer
	long dwTRDateLen : TR ��� buffer length

	m_strOut�� ������ ���� ��ȯ�� ����Ÿ�� ����ǰ� �ȴ�.
	íƮcount[5];�����޽�������[4];�����޽���[296];íƮ����Ÿ-count ��ŭ

*/
void CStdDialog::ConvertKTBFXData (LPCSTR szTR, char* pData, long dwTRDateLen)
{
	TRACE("dwTRDateLen=%d\n", dwTRDateLen);

	if (pData == NULL || dwTRDateLen <= 0)
		return;

	CString strTRCode = szTR;
	strTRCode.Remove (' ');

	if (strTRCode == QUERY_STRFX)		// TR = x56000#
	{
		CString		strData			= "";
		CString		strTemp			= "";
		CString		strPipLowest	= "";

		m_strOut = "";
/*
		//	�������� ������ ���Ѵ�.
		int	nDataCnt = (dwTRDateLen - EU_X56000_OUT1_SZ - EU_X56000_OUT_KEY_SZ) / EU_X56000_OUT2_SZ;

		//	���� �����Ͱ� ���ڵ��� ���̺��� ������� �����̹Ƿ� return
		if (nDataCnt <= 0)		
		{
			//<<< 20091109 by kmlee ����Ÿ�� ��ȸ���� �ʾ� �޽��� ���
			AfxMessageBox ("����Ÿ�� �����ϴ�!! �ٽ� ��ȸ�Ͻʽÿ�!!");
			//>>>
			return;
		}

		EU_x56000_OutRec1		singOutput;	
		EU_x56000_OutRec2		output;
		CString					strOut;
		int						nPipLowest(0);

		//	�̱۵����Ϳ��� �Ҽ��� �����͸� �޾Ƽ� ó���ϴºκ��� �ʿ��Ұ� ����... ��.��
		//	( �Ҽ��� �ڸ����� ���������Ͽ��� ���� => TR�� �޾Ƽ� ��� )
		memset (&singOutput, 0x00,  EU_X56000_OUT1_SZ);
		memcpy (&singOutput, pData, EU_X56000_OUT1_SZ);	//	KeyValue�� ������ ������ ©�󳽴�.

		CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
		char chTypeRequested = m_ChartMng.GetpChartItemSelected()->m_lpSelChart ? 
							m_ChartMng.GetpChartItemSelected()->m_lpSelChart->m_cType : pChartItem->GetTypeRequested();

		CString strBojoyMsg = "";
		strTemp.Format("%05d", nDataCnt);
		strOut += strTemp;

		//	�Ҽ��� �ڸ���
		nPipLowest = ntohl (singOutput.nPipLowest);
		strPipLowest.Format("%.*f", nPipLowest, pow(10 , nPipLowest*(-1)));

		nPipLowest += 1;

		if (chTypeRequested == DAY_DATA_CHART)
		{
			strBojoyMsg.Format ("UM=E@UD=1@RDATET=YYYYMMDD@TDC=%s:�ð�,|%s:��,|%s:����,|%s:����,|1:�⺻�ŷ���,|1:������,@RDC=%s:%d:�ð�,|%s:%d:��,|%s:%d:����,|%s:%d:����,|1:�⺻�ŷ���,|1:������,@REALTIMET=YYYYMMDDHHMMSS@", 
								strPipLowest, strPipLowest, strPipLowest, strPipLowest, 
								strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest);
		}
		else if (chTypeRequested == WEEK_DATA_CHART)
		{
			strBojoyMsg.Format ("UM=E@UD=2@RDATET=YYYYMMDD@TDC=%s:�ð�,|%s:��,|%s:����,|%s:����,|1:�⺻�ŷ���,|1:������,@RDC=%s:%d:�ð�,|%s:%d:��,|%s:%d:����,|%s:%d:����,|1:�⺻�ŷ���,|1:������,@REALTIMET=YYYYMMDDHHMMSS@", 
								strPipLowest, strPipLowest, strPipLowest, strPipLowest, 
								strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest);
		}
		else if (chTypeRequested == MONTH_DATA_CHART)
		{
			strBojoyMsg.Format ("UM=E@UD=3@RDATET=YYYYMM@TDC=%s:�ð�,|%s:��,|%s:����,|%s:����,|1:�⺻�ŷ���,|1:������,@RDC=%s:%d:�ð�,|%s:%d:��,|%s:%d:����,|%s:%d:����,|1:�⺻�ŷ���,|1:������,@REALTIMET=YYYYMMDDHHMMSS@", 
								strPipLowest, strPipLowest, strPipLowest, strPipLowest, 
								strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest);
		}
		else if (chTypeRequested == MIN_DATA_CHART)
		{
			CString strTerm = pChartItem->GetTickMinDayNumberRequested ();
			strBojoyMsg.Format ("UM=E@UD=4@UT=%d@RDATET=MMDDHHMM@TDC=%s:�ð�,|%s:��,|%s:����,|%s:����,|1:�⺻�ŷ���,|1:������,@RDC=%s:%d:�ð�,|%s:%d:��,|%s:%d:����,|%s:%d:����,|1:�⺻�ŷ���,|1:������,@REALTIMET=YYYYMMDDHHMMSS@", atoi(strTerm),
								strPipLowest, strPipLowest, strPipLowest, strPipLowest, 
								strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest);
		}
		else if (chTypeRequested == TICK_DATA_CHART)
		{
			strBojoyMsg.Format ("UM=E@UD=5@RDATET=HHMMSS@TDC=%s:�ð�,|%s:��,|%s:����,|%s:����,|1:�⺻�ŷ���,|1:������,@RDC=%s:%d:�ð�,|%s:%d:��,|%s:%d:����,|%s:%d:����,|1:�⺻�ŷ���,|1:������,@REALTIMET=YYYYMMDDHHMMSS@", 
								strPipLowest, strPipLowest, strPipLowest, strPipLowest, 
								strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest);
		}
		else if (chTypeRequested == HOUR_DATA_CHART)
		{
			//	���ϰ�� �� ������������ �����ش޶�� ��û ( ��¿�����Կ�û...  144�� => 0224�� �����ؼ� �޶���� )
			CString strTerm = pChartItem->GetTickMinDayNumberRequested ();
			long	nTime = atol(strTerm) * 100;
			strTerm.Format ("%02d00", nTime);

			strBojoyMsg.Format ("UM=E@UD=4@UT=%d@RDATET=MMDDHHMM@TDC=%s:�ð�,|%s:��,|%s:����,|%s:����,|1:�⺻�ŷ���,|1:������,@RDC=%s:%d:�ð�,|%s:%d:��,|%s:%d:����,|%s:%d:����,|1:�⺻�ŷ���,|1:������,@REALTIMET=YYYYMMDDHHMMSS@", 
								nTime, strPipLowest, strPipLowest, strPipLowest, strPipLowest, 
								strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest);
		}
		else
		{
			return;
		}

		nPipLowest -= 1;

		CString strBojoMsg;
		strBojoMsg.Format("0296%-296s", strBojoyMsg);
		strOut += strBojoMsg;

		// (2008/9/14 - Seung-Won, Bae) for Pair Tick
		char *szPrice = NULL;
		CString strItemCode;
		
		if (m_ChartMng.GetpChartItemSelected ()) 
			if (m_ChartMng.GetpChartItemSelected ()->m_lpSelChart)
				strItemCode = m_ChartMng.GetpChartItemSelected ()->m_lpSelChart->m_strItemCode;

		BOOL bAskItem = FALSE;
		if (!strItemCode.IsEmpty ()) 
			if (strItemCode.GetAt (0) == '|') 
				if (strItemCode.GetAt (1) == 'S') 
					bAskItem = TRUE;

		int nTemp (0);
		char cTemp [128];
 		double	fTemp = pow (10, nPipLowest);


		for (int i = nDataCnt - 1; 0 <= i; i--)
		{
			memset (&output, 0x00, EU_X56000_OUT2_SZ);
			memcpy (&output,							//	KeyValue�� ������ ������ ©�󳽴�.
				pData + ( i * EU_X56000_OUT2_SZ ) + EU_X56000_OUT1_SZ + EU_X56000_OUT_KEY_SZ, 
				EU_X56000_OUT2_SZ);	

			memset(cTemp, 0x00, 128);

			//<<< date & time ó�� ==========================================
			if (chTypeRequested == TICK_DATA_CHART)			// HHMMSS
			{
				memcpy (cTemp, &output.szDate [8], 6);
				strTemp = cTemp;
				strTemp.Remove ('.');
				strData.Format ("%6s00", strTemp);
				strOut += strData;
			}
			else if (chTypeRequested == MIN_DATA_CHART)		// MMDDHHMM
			{
				memcpy (cTemp, &output.szDate [4], 8 );
				strTemp = cTemp;
				strTemp.Remove ('.');
				strData.Format ("%8s", strTemp);
				strOut += strData;
			}
			else if (chTypeRequested == HOUR_DATA_CHART)	// MMDDHHMM
			{
				memcpy (cTemp, &output.szDate [4], 8);
				strTemp = cTemp;
				strTemp.Remove ('.');
				strData.Format ("%8s", strTemp);
				strOut += strData;
			}
			else if (chTypeRequested == MONTH_DATA_CHART)	// YYYYMM
			{
				memcpy (cTemp, output.szDate, 6);
				strTemp = cTemp;
				strTemp.Remove ('.');
				strData.Format ("%6s01", strTemp);
				strOut += strData;
			}
			else											// YYYYMMDD
			{
				memcpy (cTemp, output.szDate, 8);
				strTemp = cTemp;
				strTemp.Remove ('.');
				strData.Format ("%8s", strTemp);
				strOut += strData;
			}
			//>>> date & time ó�� ==========================================

			//<<< ����Ÿ ó�� ===============================================
			if (chTypeRequested == TICK_DATA_CHART)
			{
				// (2008/9/14 - Seung-Won, Bae) for Pair Tick
				if (bAskItem)	szPrice = output.szAsk;
				else			szPrice = output.szBid;

				//	�ð�
				memset (cTemp, 0x00, 128);
				memcpy (cTemp, szPrice, sizeof (output.szBid));
				strTemp.Format ("%.0f", fTemp * atof (cTemp));
				strData.Format ("%10s", strTemp);
				strOut += strData;

				//	��
				memset (cTemp, 0x00, 128);
				memcpy (cTemp, szPrice, sizeof (output.szBid));
				strTemp.Format ("%.0f", fTemp * atof (cTemp));
				strData.Format ("%10s", strTemp);
				strOut += strData;

				//	����
				memset (cTemp, 0x00, 128);
				memcpy (cTemp, szPrice, sizeof (output.szBid));
				strTemp.Format ("%.0f", fTemp * atof (cTemp));
				strData.Format ("%10s", strTemp);
				strOut += strData;

				//	����
				memset (cTemp, 0x00, 128);
				memcpy (cTemp, szPrice, sizeof (output.szBid));
				strTemp.Format ("%.0f", fTemp * atof (cTemp));
				strData.Format ("%10s", strTemp);
				strOut += strData;

				//	ü�ᷮ
				strTemp.Format ("%.0f", fTemp * 0);
				strData.Format ("%10s", strTemp);
				strOut += strData;
				
				//	�Ǹ��� ( �ϴ� �츮 ���������Ϳ��� ���µ�.. ���⿡�� ��Ʈ �Ѱ��ִ� �����Ϳ��� �ʿ���... ���߿� �����ؾ��Ұ� ���� )
				strOut += "0";
			}
			else
			{
				//	�ð�
				memset (cTemp, 0x00, 128);
				memcpy (cTemp, output.szBidOpen, sizeof (output.szBidOpen));
				strTemp.Format ("%.0f", fTemp * atof (cTemp));
				strData.Format ("%10s", strTemp);
				strOut += strData;

				//	��
				memset (cTemp, 0x00, 128);
				memcpy (cTemp, output.szBidHigh, sizeof (output.szBidHigh));
				strTemp.Format ("%.0f", fTemp * atof (cTemp));
				strData.Format ("%10s", strTemp);
				strOut += strData;

				//	����
				memset (cTemp, 0x00, 128);
				memcpy (cTemp, output.szBidLow, sizeof (output.szBidLow));
				strTemp.Format ("%.0f", fTemp * atof (cTemp));
				strData.Format ("%10s", strTemp);
				strOut += strData;

				//	����
				memset (cTemp, 0x00, 128);
				memcpy (cTemp, output.szBidClose, sizeof (output.szBidClose));
				strTemp.Format ("%.0f", fTemp * atof (cTemp));
				strData.Format ("%10s", strTemp);
				strOut += strData;
				
				//	ü�ᷮ
				strTemp.Format ("%.0f", fTemp * 0);
				strData.Format ("%10s", strTemp);
				strOut += strData;
	
				//	�Ǹ��� ( �ϴ� �츮 ���������Ϳ��� ���µ�.. ���⿡�� ��Ʈ �Ѱ��ִ� �����Ϳ��� �ʿ���... ���߿� �����ؾ��Ұ� ���� )
				strOut += "0";
			}
			//>>> ����Ÿ ó�� ===============================================
		
		} // end of for
		m_strOut = strOut;

#ifdef _WRITELOG
		WriteToStringLog("�ֽ����� �����κ��� ����_���簡::End", strCode, strCode.GetLength());
#endif
		*/
	}

}

HRESULT CStdDialog::DemoReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
/*
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
	else if(strTRCode == QUERY_STRMARKET)
	{								

		m_ChartMng.SendMessage( UMSG_RECV_MARKET_TR, (long)dwTRDateLen, (long)aTRData);

		SendMessage2StatusBar(MSG_SEARCH_END, (LPTSTR)(LPCTSTR)szMessage);

		EnableWindowAll(TRUE);	

		return 0L;
	}

	if(pWndRequested)
	{
		if(strTRCode==QUERY_STR_AVGMP)
		{
			CStringList strlistSetAll;
			char szWndChartItem[18];
			CChartItem *pWndChartItem = NULL;
			HWND hWndChartItem = NULL;
			D10418_O st10418o;
			memset(&st10418o, 0x00, sizeof(D10418_O));
			memcpy(&st10418o,aTRData,dwTRDateLen);
			st10418o.chNull = NULL;
			MEMCPYWITHNULL(szWndChartItem,st10418o.sBuffer);
			hWndChartItem = (HWND)atol(szWndChartItem);

			pWndChartItem = (CChartItem *)CWnd::FromHandle(hWndChartItem);
			if(pWndChartItem->GetSafeHwnd())
			{
				strlistSetAll.AddTail(GetAccountInfo());
				strlistSetAll.AddTail((char*)&st10418o.sShcode[0]);
				pWndChartItem->InvokeAddIn("FX_ChartGuideLineAddin.dll",INVOKEMSG_SETACCOUNTNQUOTECODE,(long)&strlistSetAll);
			}

			EnableWindowAll(TRUE);

		}	
		else
		{
			if(pWndRequested->GetSafeHwnd())
			{
				stReceiveFromServer stR;
				stR.m_strTRCode = strTRCode;
				stR.m_pBuffer = aTRData;
				stR.m_dwLength = dwTRDateLen;
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
*/
	return S_OK;
}


/*
	TR����� �޾Ƽ� ó���ϴ� �Լ� - ok
*/
//<<20100308_JS.Kim �ַθ�
/*
HRESULT CStdDialog::ReceiveData (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, 
								 LPCSTR szMessage, char* pData, long dwTRDateLen)
{
	int nRequestPairTickState = GetRequestPairTickState ();

	CWnd		*pWndRequested	= NULL;
	CString		strTRCode		= szTR;
	strTRCode.Remove (' ');

	CString strMsgCode = szMsgCode;

	if (strMsgCode.GetLength () <= 0)	// There is no error
	{
		EnableWindowAll(TRUE);	
	}
	else								// error
	{
		if (strMsgCode.GetLength () > 0 || dwTRDateLen == 0)
		{
			if(!m_bEnable)
				EnableWindowAll(TRUE);	
		}
		return 1L;
	}

	if (strcmp(szTR, TR3930) == 0)
	{
		if (m_pDrdsLib == NULL)		MakeDrdsConnectAdviseFromCode (0, 0);
		EnableWindowAll (TRUE);
		m_ChartMng.SetOrderData (pData);
		m_pDrdsLib->DrdsAdvise (CHEGEUL_PID, "@", "", "EXECUTED");
		m_pDrdsLib->DrdsAdvise (MICHEGEUL_PID, "A", "", "PENDING");
	}
	else	// #560000
	{
		STTRINFO trInfo;
		long lCnt = m_arryQuery.GetSize ();
		CString sTemp;
		
		for (long lPos = 0; lPos < lCnt; lPos++)
		{
			trInfo = m_arryQuery.GetAt (lPos);
			if (trInfo.m_strTrCode == strTRCode)
			{
				pWndRequested = trInfo.m_pWndRequested;
				m_arryQuery.RemoveAt (lPos);
				lCnt = m_arryQuery.GetSize ();
				break;
			}
		}

		if (pWndRequested)
		{
			// 2007.04.25 by LYH ���� �ڵ�(���� CodeCombo �ڵ�� ���ؼ� �ٸ� ��� ����ȸ)
			if (m_strRecvCode == "" || m_pInputBarDlg->m_ctrlCodeInput.GetDataText () == "" || 
				m_ChartMng.IsInLoop () || m_pInputBarDlg->m_ctrlCodeInput.GetDataText () == m_strRecvCode)
			{
				if (pWndRequested->GetSafeHwnd ())
				{
	#ifdef _DEBUG
					CTime tm = CTime::GetCurrentTime ();
					CString strTime;
					strTime.Format ("\n\n### ReceiveData���� - %d:%d:%d\n", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
					AfxTrace (strTime);
	#endif
					// TR (x56000) ����� �޾Ƽ� ����Ÿ ��ȯ.
					ConvertKTBFXData (strTRCode, pData, dwTRDateLen);

					stReceiveFromServer stR;
					stR.m_strTRCode = strTRCode;
					stR.m_pBuffer = (LPVOID)(LPCTSTR)m_strOut;
					stR.m_dwLength = m_strOut.GetLength();



	#ifdef _DEBUG
					tm = CTime::GetCurrentTime ();
					strTime.Format ("\n\n### ConvertKTBFXData���� - %d:%d:%d\n", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
					AfxTrace (strTime);
	#endif
					//<<< 20091221 by kmlee
					UpdateCheJanOption ();
					//>>>
					pWndRequested->SendMessage (UMSG_RECEIVEFROMSERVER,0,(long)&stR);

	#ifdef _DEBUG
					tm = CTime::GetCurrentTime ();
					strTime.Format ("\n\n### RECEIVEFROMSERVER �޼��� ó���� - %d:%d:%d\n", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
					AfxTrace (strTime);
	#endif
				}
				m_strRecvCode = "";
			}
			else
			{
				// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
				m_pInputBarDlg->SetKeyCodeChange (NID_CODEINPUT, (LPARAM)(LPCTSTR)m_pInputBarDlg->GetTypedItemCode());
			}
		}
		
		
		BOOL bRet = m_ChartMng.IsToBeReceivedMore ();

		if (!m_arryQuery.GetSize () && !bRet)
		{
			EnableWindowAll ();
		}	
	}
	
	// ******************************************************************
	SendMessage2StatusBar (MSG_SEARCH_END, (LPTSTR)(LPCTSTR)szMessage);
	// ******************************************************************

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	switch (nRequestPairTickState)
	{
		case E_RPTS_START:	RequestPairTick (m_strPairTickCode);
							ReceiveData (dwKey,  szTR,  szMsgCode, szMessage, pData, dwTRDateLen);
							break;
		case E_RPTS_PAIR:	EndRequestPairTick();
							m_ChartMng.SetAddModeFromTab (m_bOrgAddMode);
							break;
		case E_RPTS_STOP:	EndRequestPairTick ();
							break;
	}
	
	return S_OK;
}
// <-- [Edit]  ������ 2008/07/25
*/
// --> [Edit]  ������ 2008/07/25		( �����͸� �ѱ涧 ���Ͱ� �ƴ� �׳� Char�� �ѱ� )
HRESULT CStdDialog::ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	if( strcmp(szTR, QUERY_STRFX) && strcmp(szTR, QUERY_STRFX_CHE) && strcmp(szTR, QUERY_STRFX_JAN) && strcmp(szTR, QUERY_STRFX_CHE2) )		
		return 0L;

	int nRequestPairTickState = GetRequestPairTickState ();

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
	//<<20100308_JS.Kim �ַθ�
	/*
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
	*/
	/*
	if (strTRCode == QUERY_STRFX_CHE || strTRCode == QUERY_STRFX_JAN)
	{
		CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrOpenPosition;
		CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrPendingOrder;
		
		FX_CHE_STRUCT* pCheSt = NULL;
		int nIdxData = 0;
		while(nIdxData < dwTRDateLen)
		{
			pCheSt = (FX_CHE_STRUCT *)((char *)aTRData+nIdxData);

			if( memcmp(pCheSt->szKeyType, "CH01", 4) == 0 )
				arrOpenPosition.Add(pCheSt);
			else
				arrPendingOrder.Add(pCheSt);
			nIdxData += sizeof(FX_CHE_STRUCT);
		}

		CChartItem* pChartItem = (CChartItem *)pWndRequested;
		if(pChartItem)
		{
			pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 1, (long)&arrOpenPosition);
			pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 2, (long)&arrPendingOrder);
		}
		return 0L;
	}
	*/
	if (strTRCode == QUERY_STRFX_JAN)		// �ܰ� ����
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
	else if (strTRCode == QUERY_STRFX_CHE || strTRCode == QUERY_STRFX_CHE2)	// ��ü�᳻��/ü�᳻��
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
			if( strTRCode == QUERY_STRFX_CHE ) // ��ü�᳻���� OrderLine �� ������.
				pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 2, (long)&arrPendingOrder);
			else								// ü�᳻���� ����/û�� ǥ�� �ʿ� ������.
				pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 6, (long)&arrPendingOrder);
				//pChartItem->InvokeAddIn("ChartKoreaSorimachiAddIn.dll:USER_COMMENT", 2, (long)&arrPendingOrder);
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

				//<<< 20091221 by kmlee
				UpdateCheJanOption ();
				//>>>

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

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	switch (nRequestPairTickState)
	{
		case E_RPTS_START:	RequestPairTick (m_strPairTickCode);
							ReceiveData (dwKey,  szTR,  szMsgCode, szMessage, aTRData, dwTRDateLen);
							break;
		case E_RPTS_PAIR:	EndRequestPairTick();
							m_ChartMng.SetAddModeFromTab (m_bOrgAddMode);
							break;
		case E_RPTS_STOP:	EndRequestPairTick ();
							break;
	}

	return S_OK;
}
//>>

HRESULT	CStdDialog::ReceiveMessage(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage)
{
	CString szTmp;
	szTmp.Format("[%s]%s\n", szMsgCode, szMessage);
	
	//<<20100308_JS.Kim �ַθ�
	/*
	if(strcmp(szMsgCode, "80001")==0)		// Ÿ�Ӿƿ�
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
	*/
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
	//>>
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
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		}
	}
}

// �׽�Ʈ����̸� �������� ���� ������ ���� �������� ���� �����ͷ� ó���Ѵ�.
int CStdDialog::Send2Server(LPCTSTR lpTrCode,LPCTSTR lpData,int nSize,BOOL bWait, int nArrayIndex)
{
	int nTestMode = AfxGetApp()->GetProfileInt("TESTMODE", "HW", 0);

	SendMessage2StatusBar(MSG_GOING);	

	//<<< 20090904 by kmlee for test...
	// EnableWindowAll(FALSE);	
	//>>>
	
	if(m_TRComm)
	{
		if(CString(lpTrCode) == QUERY_STRMARKET)
		{
			p0607InBlock* pMarket = (p0607InBlock*)lpData;

			CString strMarketName, strNum;
			strMarketName.Format("%s", pMarket->pkName);
			if(strMarketName.Find("��������ŷ�����") >= 0)
			{
				strNum.Format("%d", m_pInputBarDlg->GetAllCount());
				memcpy(pMarket->num,strNum,sizeof(pMarket->num));
				memcpy(pMarket->nxtGb ,"0",sizeof(pMarket->nxtGb));
				memcpy(pMarket->nxtKey ,"              ",sizeof(pMarket->nxtKey));
			}
		}


		if(CString(lpTrCode) == QUERY_STRFX_JAN || CString(lpTrCode) == QUERY_STRFX_CHE || CString(lpTrCode) == QUERY_STRFX_CHE2)
		{
			m_TRComm->SetBaseData("SCREENCODE", m_strScreenNo);
			m_TRComm->SetTR(lpTrCode);
			m_TRComm->Send2Server((LPVOID)lpData, nSize, TRKEY_FID_END);
		}
		// ����TR
		else
		{
			// (2008/9/14 - Seung-Won, Bae) for Pair Tick
			if( GetRequestPairTickState() != E_RPTS_PAIR)
			{
				m_TRComm->SetTR(lpTrCode);
				m_TRComm->SetBaseData("PtManager", m_szPTManager);
				m_TRComm->SetBaseData("SCREENCODE", m_strScreenNo);

	#ifdef _DEBUG
				CTime tm = CTime::GetCurrentTime();
				CString strTime;
				strTime.Format("\n\n### Send2Server - %d:%d:%d\n", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
				AfxTrace(strTime);
	#endif
				return m_TRComm->Send2Server((LPVOID)lpData, nSize, TRKEY_FID_END);	
			}
//��������������������			else return 0;
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
	switch(message)
	{
	case UMSG_SEND2SERVER:
		{
			BOOL bWait = wParam;
			stSend2Server *pSt = (stSend2Server *)lParam;

			if( (CString(pSt->m_strTRCode) != QUERY_STRFX)			&&		//
				(CString(pSt->m_strTRCode) != QUERY_STRFX_CHE)		&&		// ��ü��
				(CString(pSt->m_strTRCode) != QUERY_STRFX_CHE2)		&&		// ü��
				(CString(pSt->m_strTRCode) != QUERY_STRFX_JAN)		&&		// �ܰ�
				(CString(pSt->m_strTRCode) != QUERY_STR_AVGMP)			    // ���� ��ȸ 
				)
				break;			

			if(pSt->m_strTRCode != QUERY_STRFX)
				memcpy(&m_st2Server, pSt, sizeof(stSend2Server));				

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
			if(m_pInputBarDlg)
			{
#ifdef _DEBUG
				CTime tm = CTime::GetCurrentTime();
				CString strTime;
				strTime.Format("\n\n### SENDSHAREDCODE���� - %d:%d:%d\n", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
				AfxTrace(strTime);
#endif

				STINPUTCODEDATA stInputCodeData;
				stInputCodeData.m_strCode = CString((LPCTSTR)wParam);
				stInputCodeData.m_strTime = "";
				stInputCodeData.m_nDefault = MODE_DEFAULT;
				stInputCodeData.m_nMode = CHANGE_ITEM;
				m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, MODE_DEFAULT, (LPARAM)&stInputCodeData);
				//	�����޺����� �ڵ尡 ����Ǿ������ ���� �����õ� �������ش�.
				if( m_pLeftBarDlg )
				{
					CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
					if(pTabCodeSearch)
					{
						pTabCodeSearch->SetCurListIndex(stInputCodeData.m_strCode);
					}
				}

#ifdef _DEBUG
				tm = CTime::GetCurrentTime();
				strTime.Format("\n\n### INPUTCODETOCHARTMNG �޼��� ó���� - %d:%d:%d\n", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
				AfxTrace(strTime);
#endif
			}
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
	case UMSG_SENDTOMSGBOX:
		{
			m_pMsgBar->ShowMessage((LPCTSTR)wParam,(CWnd*)lParam);
		}
		break;
	case UMSG_EDITCODE_INIT:
		{
			if(m_pInputBarDlg)
				m_pInputBarDlg->SetCodeInit ();
		}
//		break;
	case UMSG_UPDATECODE:
		{
			//�ڽ��� ������� ����(������)
			if(m_pInputBarDlg)
			{
				switch(lParam) {
				case 1:
					if(m_pInputBarDlg->m_nMarketType != CMasterTypeInfo::FX_CHART)
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
						//<<20100308_JS.Kim �ַθ�
						//m_pInputBarDlg->m_ctrlCodeInput.SetDataText( CStdDialog::GetRealItemCode( ( LPCTSTR)wParam));
						m_pInputBarDlg->m_pCodeInput->SetCode(CStdDialog::GetRealItemCode( ( LPCTSTR)wParam));
						m_pInputBarDlg->SetShareCodeChanged(FALSE);
						//>>

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
						if(m_pInputBarDlg->m_nMarketType != FX_CHART)
							return 1L;
						break;
					case 8:
					case 12:
						if(m_pInputBarDlg->m_nMarketType != FUTOPT_CHART)
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
	case USMG_PLEASE_RESIZE:
		{
		}
		break;	
	case UMSG_SETMATRIX:
		{
		}
		break;
	case UMSG_FORCE_SHOW_HOGAWND:
		{
		}
		break;
	case WM_DRDS_DATA:
		{
		}
		break;
	case UMSG_SENDTR_HOGA_DATA:
		{		
			CString* pStrCode, sCode;
			pStrCode = (CString*)wParam;
			sCode.Format("%s", pStrCode->GetBuffer(pStrCode->GetLength()));
			Request_StockHogaData(sCode);
		}
		break;
	case UMSG_DISCONNECT_HOGA_REAL:
		{
			Request_Disconnect_StockRealHogaData(_T(""));
		}
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
		{
			// for Invalidate Floating Hoga Dialog - SeoJeong - 2005/08/26
			if(m_ChartMng)
				m_ChartMng.m_pFloatingDlg->Invalidate();
			break;
		}
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
			//::SendMessage(m_hViewInMain, RMSG_GET_LINKINFODATA, wParam, lParam);
			CString strItemCode = "";
			int	nOrderType = 2;
			if (!m_ChartMng.GetpChartItemSelected ()) 
				break;
			
			if (m_ChartMng.GetpChartItemSelected()->m_lpSelChart)
				strItemCode = GetRealItemCode(m_ChartMng.GetpChartItemSelected()->m_lpSelChart->m_strItemCode);
			else
				strItemCode = GetRealItemCode(m_ChartMng.GetpChartItemSelected()->GetDataCode());
			nOrderType = m_ChartMng.GetpChartItemSelected()->m_strOrderType == "0" ? 1 : 2;

			if( strItemCode.IsEmpty() )		break;

			CString strAccNo, strAccName, strAccPW;
			m_pExtInputBarDlg->GetAccountInfo(strAccNo, strAccName, strAccPW);
			ConfirmMap(strAccNo, strAccName, strAccPW,
						strItemCode, "", "1", "", "", "", nOrderType, FALSE);

// <-- [Edit]  ������ 2008/10/30
		}
		break;

	// FX��Ʈ �ֹ� ���� - onlyojt (20091204)
	//<<js.kim
	/*
	case UM_GETBROAD:
		{
			if(wParam != 4)		// wParam�� 4�ΰ͸�..
				return 0;
			
			FX_CHE_STRUCT* pOrderInfomation = (FX_CHE_STRUCT*)lParam;
			if(!pOrderInfomation)
				return 0;
			
			CString strCodeInputCode = m_pInputBarDlg->m_ctrlCodeInput.GetDataText();

			if(strCodeInputCode.IsEmpty())
				return 0;

			CString strAccount = "";
			if (m_hViewInMain)
				::SendMessage (m_hViewInMain, RMSG_GET_CHARTACCOUNT, 0L, (LPARAM)&strAccount);
			
			CString strItemCode (pOrderInfomation->SHCODE,	sizeof(pOrderInfomation->SHCODE));
			CString szAccNo (pOrderInfomation->szAccNo,		sizeof(pOrderInfomation->szAccNo));

			strItemCode.TrimRight ();
			strCodeInputCode.TrimRight ();
			strAccount.TrimRight ();
			szAccNo.TrimRight ();
			

			if (strCodeInputCode == strItemCode && strAccount == szAccNo)
			{
				CString strKeyType(pOrderInfomation->szKeyType, sizeof(pOrderInfomation->szKeyType));

				CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
				if(pChartItem)
				{
					if(strKeyType == "CH01" || strKeyType == "CLSL")		// ü��
						pChartItem->InvokeAddIn("ChartCommonAddIn.dll:ORDER_LINE", 3, (long)pOrderInfomation);
					else if(strKeyType == "OR01" )	// ��ü��
						pChartItem->InvokeAddIn("ChartCommonAddIn.dll:ORDER_LINE", 4, (long)pOrderInfomation);
				}
			}
		}
		break;
	// FX��Ʈ �ֹ� ���� - onlyojt (20091204)
	case UM_RESET_CHEJANINFO:
		{
			IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 2);
			if(!pKSLibMng)
				return 0;
			
			CString strCodeInputCode = m_pInputBarDlg->m_ctrlCodeInput.GetDataText();
			if(strCodeInputCode.IsEmpty())
				return 0;

			// ���¹�ȣ
			CString strAccount = "";
			if (m_hViewInMain)
				::SendMessage (m_hViewInMain, RMSG_GET_CHARTACCOUNT, 0L, (LPARAM)&strAccount);

			if (strAccount.IsEmpty ())
				return 0;

			CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrOpenPosition;
			CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrPendingOrder;

			strAccount.TrimRight ();
			strCodeInputCode.TrimRight ();

			pKSLibMng->Lib_KSLink_GetOrderLineInfo(strAccount, strCodeInputCode, (CObArray*)&arrOpenPosition, (CObArray*)&arrPendingOrder, this);

			CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
			if(pChartItem)
			{
				pChartItem->InvokeAddIn("ChartCommonAddIn.dll:ORDER_LINE", 1, (long)&arrOpenPosition);
				pChartItem->InvokeAddIn("ChartCommonAddIn.dll:ORDER_LINE", 2, (long)&arrPendingOrder);
			}
		}
		break;
		*/

	}
	return CExOCXDLL::WindowProc(message, wParam, lParam);
}

/*
	Function : ��� Ű �ʱ�ȭ
	Paramter :
	Return	:
	Author	: By DRFN
	Comments	: FX Margin �ϸ鼭 �ҽ� ���� (�ѱ��Ҹ�ġ �����)

	ok
*/
void CStdDialog::InitDrds()
{
	m_nDrdsInfoCnt = 1;

	m_DrdsInfo[0].DataName = "FX";								// FX
	m_DrdsInfo[0].FieldName = "*";
	m_DrdsInfo[0].KeyName = "";
	m_DrdsInfo[0].nID = FX_SISE_PID;
	m_DrdsInfo[0].bAdvise = FALSE;

	/*
	m_DrdsInfo[0].DataName = "a";								// �ֽ� (KXS3)
	m_DrdsInfo[0].FieldName = "*";
	m_DrdsInfo[0].KeyName = "";
	m_DrdsInfo[0].nID = STOCK_SISE_PID;
	m_DrdsInfo[0].bAdvise = FALSE;

	m_DrdsInfo[1].DataName = "JS0";								// ���� (IXIJ)
	m_DrdsInfo[1].FieldName = "*";
	m_DrdsInfo[1].KeyName = "";
	m_DrdsInfo[1].nID = UPJONG_SISE_PID;
	m_DrdsInfo[1].bAdvise = FALSE;

	m_DrdsInfo[2].DataName = "SC0";								// ���� (FFC0)
	m_DrdsInfo[2].FieldName = "*";
	m_DrdsInfo[2].KeyName = "";
	m_DrdsInfo[2].nID = FUTURE_SISE_PID;
	m_DrdsInfo[2].bAdvise = FALSE;

	m_DrdsInfo[3].DataName = "OC0";								// �ɼ� (OOC0)
	m_DrdsInfo[3].FieldName = "*";
	m_DrdsInfo[3].KeyName = "";
	m_DrdsInfo[3].nID = OPTION_SISE_PID;
	m_DrdsInfo[3].bAdvise = FALSE;

	m_DrdsInfo[4].DataName = "X11";								// ȣ���ܷ�
	m_DrdsInfo[4].FieldName = "*";
	m_DrdsInfo[4].KeyName = "";
	m_DrdsInfo[4].nID = 3;										// ��104;
	m_DrdsInfo[4].bAdvise = FALSE;

	m_DrdsInfo[5].DataName = "ELW";
	m_DrdsInfo[5].FieldName = "*";
	m_DrdsInfo[5].KeyName = "";
	m_DrdsInfo[5].nID = 105;
	m_DrdsInfo[5].bAdvise = FALSE;

	m_DrdsInfo[6].DataName = "FOR";								// �ؿ� (HWFJ)
	m_DrdsInfo[6].FieldName = "*";
	m_DrdsInfo[6].KeyName = "";
	m_DrdsInfo[6].nID = FOREIGN_SISE_PID;
	m_DrdsInfo[6].bAdvise = FALSE;
	*/
}

/*
	�ǽð� Advise�ϴ� �Լ�..
*/
BOOL CStdDialog::MakeDrdsConnectAdviseFromCode (int nI, BOOL bAddMod)
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
		// vntsorl_20110520:[A00000485] ������/���� ó������ ����
		if( m_RealTrCounter.FindCode(KeyName) == 1)
		{
			//@Solomon-->
			//m_pDrdsLib->DrdsInit(m_hWnd);
			//m_pDrdsLib->DrdsAdvise(nID, DataName, FieldName, KeyName);
			if(m_pPTManager) m_pPTManager->RequestMChartReal('A', DataName, KeyName);
			//@Solomon<--
			m_DrdsInfo[nI].bAdvise = TRUE;
		}
	}


	return TRUE;
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
void CStdDialog::AddRealCodes()
{
}

// �������� �ҷ����� : �������� - ojtaso (20070514)
void CStdDialog::AddRealCodeList (LPCTSTR lpszCode)
{
	/*
	ST_CODEDATA oneCode;
	oneCode.m_strCode = lpszCode;

	int nMarketType = GetMarketTypeFromCode (lpszCode);
	BOOL bRet = FALSE;

	if(nMarketType == FX_CHART)
	{
		m_ListStockRealCodes.AddTail(oneCode);
	}
	*/

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
					//@Solomon
					m_pDrdsLib->DrdsUnadvise(0, "", "", (LPSTR)(LPCSTR)strCode);
					if(m_pPTManager) m_pPTManager->RequestMChartReal('U', "", strCode);
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
				//@Solomon
				m_pDrdsLib->DrdsUnadvise(0, "", "", (LPSTR)(LPCSTR)pListRealCodes->GetAt(pos).m_strCode);
				if(m_pPTManager) m_pPTManager->RequestMChartReal('U', "", pListRealCodes->GetAt(pos).m_strCode);
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
		// vntsorl_20110520:[A00000485] ������/���� ó������ ����
		if( m_RealTrCounter.FindCode(KeyName) == 0)
		{
			//@Solomon
			//m_pDrdsLib->DrdsUnadvise(nID, DataName, FieldName, KeyName);
			if(m_pPTManager) m_pPTManager->RequestMChartReal('U', DataName, KeyName);
			m_DrdsInfo[nI].bAdvise = FALSE;
		}
	}

	return TRUE;
}

// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
void CStdDialog::OnCodeUnAdvise(CString strCode, BOOL bAddMode)
{
	// ���� �ڵ尡 ����Ʈ�� ������ ����
	// �������� ������ : �������� - ojtaso (20070208)
	int nMarketType = GetMarketTypeFromCode(strCode);
	if(nMarketType == FX_CHART)
	{
		m_DrdsInfo[0].KeyName = strCode;
		MakeDrdsUnConnectUnAdviseFromCode(0, bAddMode);								
		// �������� ������ : �������� - ojtaso (20070208)
	}
}

// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
void CStdDialog::OnCodeAdvise(CString strCode, BOOL bAddMode)
{
	// Drds ���� - ���� �ڵ尡 ����Ʈ�� ������ �߰�
	// �������� ������ : �������� - ojtaso (20070208)
	int nMarketType = GetMarketTypeFromCode(strCode);

	if(nMarketType == FX_CHART)
	{
		m_DrdsInfo[0].KeyName = strCode;
		MakeDrdsConnectAdviseFromCode(0, bAddMode);
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
	if( m_pLeftBarDlg )	m_pLeftBarDlg->SetPacketFromOutSide(strPacketFromOutSide);
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
	CRect rect(0, 0, 0, 0);
	CRect rectOriginal;
	if(m_bHideChart)
	{
		GetClientRect(rectOriginal);
		m_lWidthOriginal = rectOriginal.Width()+1;
		m_lHeightOriginal = rectOriginal.Height()+1;
		if( m_pLeftBarDlg )		m_pLeftBarDlg->GetClientRect(rect);
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
	if( m_pLeftBarDlg )
	{
		CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
		if(pTabCodeSearch)
		{
			pTabCodeSearch->OnBtprev();
		}	
	}
}

void	CStdDialog::SetJongListNext()
{
	if( m_pLeftBarDlg )
	{
		CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
		if(pTabCodeSearch)
		{
			pTabCodeSearch->OnBtnext();
		}	
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

/*
	Function : �ʱ� ���� �ʱ�ȭ	
	Paramter :					
	Return	:					
	Author	: By DRFN			
	Comments	: FX Margin �ϸ鼭 �ҽ� ���� (�ѱ��Ҹ�ġ �����)
*/
void CStdDialog::SetInitInfo ()
{
	int nID = 21;
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
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

	// User Path 
	m_strUserDir.Format("%s%s", pManager->GetEncUserDir(), "\\Chart");
	if (_access(m_strUserDir, 0) == -1)
		::mkdir(m_strUserDir);

	_DoVersionCheck_ChartDef();
	_DoCheckDaultDataFile();

	// Image Path
	//solomon-090825
	m_szImageDir = m_strRootPath + "\\Icss\\Image\\drchart";

#ifdef __MULTI_LANG__
	//@Solomon-MultiLang:091117
	g_iMetaTable.m_strDataFolderPath.Format(_T("%s\\"), pManager->GetDataDir());
	g_iMetaTable.LoadMetaTable();
#endif
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
	m_TRComm->SetBaseData("SCREENCODE", m_strScreenNo);
	m_TRComm->Send2Server(st.m_pBuffer, st.m_lLength, FALSE);	
}

void CStdDialog::Request_StockHogaData(CString strRecvCode)
{
	if(m_ChartMng.m_pFloatingDlg == NULL) return;
	if(m_ChartMng.m_pFloatingDlg->GetSafeHwnd() == NULL ) return;
	if(m_ChartMng.m_pFloatingDlg->m_bIconState) return; //��� ���¸� TR��ȸ ���Ѵ�.

	if(m_pInputBarDlg == NULL) return;
	if(m_pInputBarDlg->GetSafeHwnd() == NULL) return;

	CString strCode;

	if(strRecvCode.GetLength() == CODE_STOCK_LENGTH)
		strCode = strRecvCode;
	else
		// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
		strCode = m_pInputBarDlg->GetTypedItemCode();

	CString	strKmlee;
	strKmlee.Format ("Request_StockHogaData - %s", strCode);
	AfxMessageBox (strKmlee);

	if(!strCode.GetLength())
	{
		//ȣ��â Data Clear & Real ���� ===================================
		Request_Disconnect_StockRealHogaData(m_pInputBarDlg->m_strOldCode);
		return;
	}

	strKmlee.Format ("Request_StockHogaData2 - %s", strCode);
	AfxMessageBox (strKmlee);

	if(m_ChartMng.m_pFloatingDlg != NULL)
	{
		if(m_ChartMng.m_pFloatingDlg->GetSafeHwnd())
		{	
			// Real ���� ====================================================================
			if(m_pInputBarDlg->m_strOldCode != strCode)
				Request_Disconnect_StockRealHogaData(m_pInputBarDlg->m_strOldCode);
			// Real ���� ====================================================================

			if(strCode.GetLength() != CODE_STOCK_LENGTH)
				return;

			char stJMCode[7];
			memset(&stJMCode,0x00,sizeof(stJMCode));			
			memcpy(stJMCode, strCode, strCode.GetLength());		
			
			CString strFidInfo("0                                                                               ");
			CString strTrInfo;
			strTrInfo.Format("%s20001", strCode);
			CString strRecordInfo("1               000060220221001002003004005006008122126130134138124128132136140162166170174178164168172176180202204207209061062222070075010092;");
			CString strOutput;
			strOutput = strFidInfo + strTrInfo + strRecordInfo;

			stSend2Server st;
			st.m_nType = 0;
			st.m_pWnd = (CWnd*)m_ChartMng.m_pFloatingDlg;
			st.m_pBuffer = (void*)(LPCTSTR)strOutput;
			st.m_lLength= strOutput.GetLength();
			st.m_strTRCode = QUERY_STOCK_HOGA;	
			SendMessage(UMSG_SEND2SERVER,0,(long)&st);

			//2005. 07. 15
			Request_StockRealHogaData(strCode);
		}
	}
}

/*
	�ʿ��� �κ����� ������ ����... by kmlee 20090921
*/
void CStdDialog::Request_StockRealHogaData(CString strCode)
{
	// �ֽ� Real Hoga ��û 
	if(strCode.GetLength())
	{
		strCode.TrimLeft();
		strCode.TrimRight();
		m_DrdsInfo[4].KeyName = strCode;
		// �������� ������ : �������� - ojtaso (20070208)
		MakeDrdsConnectAdviseFromCode(4, FALSE);		
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
				m_DrdsInfo[4].KeyName = m_ChartMng.m_pFloatingDlg->m_strCode;
				// �������� ������ : �������� - ojtaso (20070208)
				MakeDrdsUnConnectUnAdviseFromCode(4, FALSE);		
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
	BOOL bResult  = FALSE;

	CString strFileRoot;
	strFileRoot = m_strRootPath + "\\Dev\\ReplayChart.dll";

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
		AfxMessageBox("\n# ReplayChart.dll�� �����ϴ�.");
#endif
		m_pWndReplayLib = NULL;
		bResult = FALSE;
	}

	return bResult;
}

/*
	ok
*/
BOOL CStdDialog::UnLoad_ReplayChart_Lib ()
{		
	if (m_hReplayChartLib != NULL)
	{
		if (m_pWndReplayLib)
			_fnReplayDll_UnLoad (m_pWndReplayLib);
		
		if (FreeLibrary (m_hReplayChartLib))
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

void CStdDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rcExtTool;
	dc.GetClipBox(rcExtTool);
	dc.FillSolidRect(&rcExtTool, GetBkgrndColor());
	
	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.bottom = rcClient.top + 2;
	dc.FillSolidRect(rcClient, GetBkgrndColor());
}

/*
	Function : ��Ʈ �̹��� �ʱ�ȭ
	Paramter :					
	Return	:					
	Author	: By DRFN			
	Comments	: FX Margin �ϸ鼭 �ҽ� ���� (�ѱ��Ҹ�ġ �����)
*/
void CStdDialog::InitImageList ()
{
	BOOL bOK = FALSE;											// ��ư �̹��� �ε� �÷���

	CBitmap		bitmap;
	CString		strBitmapFile;	
	
	m_static_nChartCnt4ImgList++;								// �ε�� ��Ʈ ����


	if (!m_ImgList_W15H15.m_hImageList)							// Width 15, Height 15 �̹��� ���� �ε�
	{
		if (!m_ImgList_W15H15.Create (15, 14, ILC_MASK | ILC_COLORDDB, 1, 1))
		{
			AfxMessageBox ("Can not Find W15_H15.bmp file");			
		}
		strBitmapFile.Format ("%s\\%s", m_szImageDir, "W15_H15.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage (NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
										LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W15H15.Add (&bitmap, RGB (255, 0, 255));
		bitmap.DeleteObject ();
	}
	
	if (!m_ImgList_W20H20.m_hImageList)						// Width 20, Height 20 �̹��� ���� �ε�
	{
		if (!m_ImgList_W20H20.Create (20, 20, ILC_MASK | ILC_COLORDDB, 1, 1))
		{
			AfxMessageBox ("Can not Find W20_H20.bmp file");
		}

		strBitmapFile.Format ("%s\\%s", m_szImageDir,  "W20_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
									LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W20H20.Add (&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject ();
	}

	if (!m_ImgList_W20H20_Tool.m_hImageList)				// Width 15, Height 15 �̹��� ���� �ε�(TOOL)
	{
		//<<< 20091111 by kmlee
		//if (!m_ImgList_W20H20_Tool.Create (19, 19, ILC_MASK | ILC_COLORDDB, 1, 1))
		if (!m_ImgList_W20H20_Tool.Create (20, 20, ILC_MASK | ILC_COLORDDB, 1, 1))
		//>>>
		{
			AfxMessageBox ("Can not Find W20_H20_Tool.bmp file");			
		}
		strBitmapFile.Format ("%s\\%s", m_szImageDir,  "W20_H20_Tool.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
									LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W20H20_Tool.Add (&bitmap, RGB (255, 0, 255));
		bitmap.DeleteObject ();
	}

	if (!m_ImgList_W46H20.m_hImageList)						// Width 46, Height 20 �̹��� ���� �ε�
	{
		if (!m_ImgList_W46H20.Create (46, 20, ILC_MASK | ILC_COLORDDB, 1, 1))
		{
			AfxMessageBox ("Can not Find W46_H20.bmp file");
		}
		strBitmapFile.Format ("%s\\%s", m_szImageDir,  "W46_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
									LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));	
		m_ImgList_W46H20.Add (&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject ();						
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

	if ( !m_ImgList_W28H22.m_hImageList)						// Width 28, Height 22 �̹��� ���� �ε�
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

/*
*/
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

void CStdDialog::AllCodeUnAdvise()
{
	for(int i = 0; i < m_nDrdsInfoCnt; ++i)
		MakeDrdsUnConnectUnAdviseFromCode (i, FALSE);

	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
	if(!pDrdsMng) return;

	if(m_pDrdsLib)
	{
		RemoveDrdsFromCode (&m_ListStockRealCodes);
		m_pDrdsLib->DrdsClose ("");
		pDrdsMng->UnadviseDrdsLib (m_pDrdsLib);
		m_pDrdsLib = NULL;
	}

	m_RealTrCounter.RemoveAll();
}

void CStdDialog::Recv_ReplayRealData(void *pData, int nLen)
{
}

LONG CStdDialog::OnReplayDrds(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != FX_CHART) && (m_nMarketType != FUTOPT_CHART))
		return 1L;

	CString strCode, strSpeed;
	strCode.Format("%s", (LPCTSTR)wp);

	m_ChartMng.SetReplayRealTitle(strCode, m_strSpeed, (TICK_DATA*)lp);
	return 1;
}

LONG CStdDialog::OnReplayFail(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != FX_CHART) && (m_nMarketType != FUTOPT_CHART))
		return 1L;
	
	if(m_pInputBarDlg)
		m_pInputBarDlg->ReplayFail();

	return 1;
}

LONG CStdDialog::OnReplayChangeDate(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != FX_CHART) && (m_nMarketType != FUTOPT_CHART))
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
	if(m_nMarketType == FX_CHART)
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
	if( (m_nMarketType != FX_CHART) && (m_nMarketType != FUTOPT_CHART))
		return 1L;

	return 1;
}

LONG CStdDialog::OnReplaySetTotalSec(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != FX_CHART) && (m_nMarketType != FUTOPT_CHART))
		return 1L;
	
	if(m_pInputBarDlg)
		m_pInputBarDlg->SetReplayTotalSec(wp);

	return 1;
}

LONG CStdDialog::OnReplayEachSec(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != FX_CHART) && (m_nMarketType != FUTOPT_CHART))
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
			//<<20100308_JS.Kim �ַθ�
			//m_pInputBarDlg->m_ctrlCodeInput.SetDataText( CStdDialog::GetRealItemCode( ( LPCTSTR)szOpenData));
			m_pInputBarDlg->m_pCodeInput->SetCode(CStdDialog::GetRealItemCode( ( LPCTSTR)szOpenData));
			m_pInputBarDlg->SetShareCodeChanged(FALSE);
			//>>
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

	//<< �ַθ�. FX ���� ��Ʈ�̹Ƿ� ���� ���� �ʿ����
	//ChangeStockMode( GetMarketTypeFromCode( szCode));
	//>>

	STINPUTCODEDATA stInputCodeData;
	stInputCodeData.m_strCode = strSBCode;
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
		HINSTANCE hInstResourceClient = AfxGetResourceHandle();
		AfxSetResourceHandle(g_hInstance);

		m_hCursor = AfxGetApp()->LoadCursor(IDC_HANDCUR);
		AfxSetResourceHandle(hInstResourceClient);
		::SetCursor(m_hCursor);
	}

	return 0L;
}

// ������ �����ϰų� ����� ��� �����͸� ���� �ް� �ȴ�. �̶� �ʱ�ȭó���� ������ϴ� ��ƾ���ִ�.
// �ý���Ʈ���̵��� ������ ����ǰ� ���� �� �̷��� ���濡 ���� ó���� ����� �Ѵ�.
void CStdDialog::NofityNewDataReceive()
{
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
	CRect rectLeftBar(0, 0, 0, 0);
	if( m_pLeftBarDlg )
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
	if( m_pLeftBarDlg )		m_pLeftBarDlg->Invalidate();
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
	if(pListRealCodes->GetCount() <= 0)
		return;

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
			//<<20100308_JS.Kim �ַθ�
			/*
			if(oneCode.m_bType == 2 || oneCode.m_bType == 4)
				m_TRComm->AddMainKey(DI_CODE, oneCode.m_strCode, C_DT_USHORT, 2);
			else
				m_TRComm->AddMainKey(DI_CODE, oneCode.m_strCode, C_DT_STRING, nCodeLength);
			
			m_TRComm->AddOptionKey(1, NULL, 5, 0, 0);
			m_TRComm->SetRealUpdateType(2);			//  RT_UPDATE	2	// ������Ʈ Ÿ��
			//{{2007.06.26 by LYH �ֽĸ��� ���(����ü�� �������� ����)
			if(strTrCode == "20000")
			{
				m_TRComm->AddDataKey(15313);//���簡
				m_TRComm->AddDataKey(15009);//�ð�
				m_TRComm->AddDataKey(15010);//��
				m_TRComm->AddDataKey(15011);//����
				m_TRComm->AddDataKey(15317);//�����ȣ
				m_TRComm->AddDataKey(15316);//���Ϻ�
				m_TRComm->AddDataKey(15326);//�����
				m_TRComm->AddDataKey(15315);//�ŷ���
				m_TRComm->AddDataKey(DI_TIME);//ü��ð�
			}
			else
				//}}
			{
				m_TRComm->AddDataKey(15001);//���簡
				m_TRComm->AddDataKey(15009);//�ð�
				m_TRComm->AddDataKey(15010);//��
				m_TRComm->AddDataKey(15011);//����
				m_TRComm->AddDataKey(15006);//�����ȣ
				m_TRComm->AddDataKey(15472);//���Ϻ�
				m_TRComm->AddDataKey(15004);//�����
				m_TRComm->AddDataKey(15015);//�ŷ���
				m_TRComm->AddDataKey(DI_TIME);//ü��ð�
			}
			*/
			//>>
			nIndex++;
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

		if(strKey == oneCode.m_strCode)
		{
			pListRealCodes->RemoveAt(pos);
			break;
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

	int nMarketType = GetMarketTypeFromCode(lpszItemCode);

	if(nMarketType == FX_CHART)
	{
		RemoveRealCodes(&m_ListStockRealCodes, lpszItemCode);
		AddRealCodes("20000", &m_ListStockRealCodes);
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

/*
*/
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


/*
	�ǽð� ����Ÿ ���� ok
*/
//<<20100308_JS.Kim �ַθ�
/*
long CStdDialog::OnGetBroad(WPARAM wParam, LPARAM lParam)
{
	REALDATA_st* lpSbData = (REALDATA_st*)lParam;
	LPREALDATA lpRealData = NULL;

	switch( lpSbData->chRealType )
	{
	case REAL_FX_SISE:
		{
			if ( !m_ChartMng )
				return 0L;
			BYTE *tmpBuff = NULL;
			tmpBuff = new BYTE [lpSbData->nSize + 1];
			memset (tmpBuff, 0x00, lpSbData->nSize + 1);
			memcpy (tmpBuff, lpSbData->pDataBuf, lpSbData->nSize);
			m_ChartMng.SetRealJPriceData ((LPVOID)tmpBuff);
			delete []tmpBuff;
			return 1L;
		}
		break;
	}
	return 0L;
}
*/

/*
	ok
*/
int CStdDialog::GetMarketTypeFromCode (CString strCode) 
{
	//<<20100308_JS.Kim �ַθ�
	int nType;
	return GetMarketTypeFromCode(strCode, nType);
	/*
	IMasterDataManager080331* pDataManager = (IMasterDataManager080331*)AfxGetPctrInterface(UUID_IMasterDataManager, 2);

	BYTE ucMarketType = pDataManager->GetMarketTypeFromCode (strCode);

	BOOL bRet = FALSE;
	int nLength = strCode.GetLength();

	if(		ucMarketType == 1	||		// ���� ����
			ucMarketType == 2	||		// �ڽ��� ����
			ucMarketType == 3	||		// ��3����(OTCBB)
			ucMarketType == 4	||		// HIGHYIELD �ݵ�(��������)
			ucMarketType == 5	||		// �ֽĿ���Ʈ ELW
			ucMarketType == 6 )			// KDR
			return FX_CHART;
	else if( ucMarketType == 26 ||		// �峻����
			 ucMarketType == 27 ||		// KOSPI 200 ����
			 ucMarketType == 28 ||		// KOSPI 100 ����
			 ucMarketType == 33 ||		// �ڽ��� ����
			 ucMarketType == 34 ||		// �ڽ���50 ����
			 ucMarketType == 36 ||		// �ڽ���STAR ����
			 ucMarketType == 59 ||		// ��Ÿ��������
			 ucMarketType == 60	||		// KRX
			 ucMarketType == 61	||		// KRX ��������
			 ucMarketType == 62	)		// �����������
		return UPJONG_CHART;
	else if( ucMarketType == 21	||		// KOFEX ��������
			 ucMarketType == 24 ||		// ����
			 ucMarketType == 25 ||		// �ɼ�
			 ucMarketType == 65 )		// �������弱��
		return FUTOPT_CHART;
	else if( ucMarketType == 64 ||		// �ֽļ���
			 ucMarketType == 38 )		// �ֽĿɼ�
		return STOCKFUTOPT_CHART;
	else if( ucMarketType == 42	||		// �̱�����
			 ucMarketType == 43	||		// �̱�����(�̱�����)
			 ucMarketType == 44	||		// �̱����� �������(ADR)
			 ucMarketType == 45	||		// ��ǰ����(������)
			 ucMarketType == 46	||		// CME����
			 ucMarketType == 47	||		// �������� �������(GDR)
			 ucMarketType == 48	||		// ȯ��
			 ucMarketType == 49	||		// �ݸ�
			 ucMarketType == 50	||		// �����ݸ�
			 ucMarketType == 51	||		// �ֿ䱹����ä
			 ucMarketType == 52	||		// �����ݸ�
			 ucMarketType == 53	||		// �ݵ�ü
			 ucMarketType == 54	||		// �����ֿ�����
			 ucMarketType == 55	||		// ECN
			 ucMarketType == 56	)		// �����ְ�����(������)
		return FOREIGN_CHART;
	return FX_CHART;
	*/
	//>>
}

// ������ ���� - ojtaso (20080519)
void CStdDialog::OnMagnifyingglass(BOOL bShow)
{
	if(bShow)
	{
		if(!m_pDlgGlass)	
		{
			m_pDlgGlass = new CDlgGlass(&m_ChartMng);
			
			HINSTANCE hInstResourceClient = AfxGetResourceHandle();
			AfxSetResourceHandle(g_hInstance);

			m_pDlgGlass->Create(IDD_MAGNIFYINGGLASS, &m_ChartMng);
	
			AfxSetResourceHandle(hInstResourceClient);
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

/*
	Form�� load �Ǹ�.. ���� View������ ������ �ִ�..
*/
LRESULT CStdDialog::OnFormInitAfter(WPARAM wParam, LPARAM lParam)
{
	if (m_pInputBarDlg)
		m_pInputBarDlg->SetLastHistoryCode ();

	PostMessage (UMSG_EDITCODE_INIT);
	
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
	//<<20100308_JS.Kim �ַθ�
	/*
	IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 2);
	if(!pKSLibMng) return 0;

	m_clrBackground = pKSLibMng->Lib_KSLink_GetColor(1);	// ����ȭ�� ���
	ChangeSkinColor();
	*/
	m_clrTabBkgrnd = m_clrBackground;
	m_clrOLBkgrnd = winix_GetColor(9);
	m_clrOLOutLine = winix_GetColor(10);
	
	g_pPtUtilMng->GetCurrentPalette();
	
	ChangeSkinColor();
	//>>

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
	SaveChart();
	return 0L;
}

LRESULT CStdDialog::OnFormSetPageSave(WPARAM wParam, LPARAM lParam)
{
	CString strKey = (LPCSTR)lParam;
	strKey.TrimRight();
	return 0L;
}

CString CStdDialog::GetLinkInfoData()
{
	CString strCode;
	if(m_hViewInMain)
		::SendMessage(m_hViewInMain, RMSG_GET_LINKINFODATA, (WPARAM)0, (LPARAM)&strCode);

	return strCode;
}

void CStdDialog::SaveChart()
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

	m_ChartMng.SaveChart(m_strConfig);
	if(m_nChartType != MINI_MULTI_CHART && m_nChartType != MINI_SINGLE_CHART)
		if( m_pLeftBarDlg )		m_pLeftBarDlg->SetConfig(FALSE);
	m_pInputBarDlg->SaveInputBar();
	if(m_nChartType == MULTI_CHART)
		m_pExtInputBarDlg->SaveExtInputBar();
}

/*
	ok
*/
void CStdDialog::GetOrderInfo()
{
	//<<20100308_JS.Kim �ַθ�
	/*
	IKSLibManager* pKSLibManager = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 2);

	CString	m_strIBNo		= pKSLibManager->Lib_KSLink_GetLoginInfo("IB_NO");			// ȸ���� IB_NO(3)		: ȸ���� IB_No
	CString m_strTradeType	= "00";														// ���� ���� - �ŷ�����(00 : 10��, 01:1��)
	CString	m_strUserID		= pKSLibManager->Lib_KSLink_GetLoginInfo("USER_ID");		// ID(17)		: ����� IB_No

	EU_T3930_INREC Inrec;	

	memset(&Inrec, 0x00, sizeof(Inrec));
	memcpy(Inrec.szMemberNo, (LPSTR)(LPCTSTR)m_strIBNo, m_strIBNo.GetLength());
	memcpy(Inrec.szTradeType, (LPSTR)(LPCTSTR)m_strTradeType, m_strTradeType.GetLength());
	memcpy(Inrec.szCustNo, (LPSTR)(LPCTSTR)m_strUserID, m_strUserID.GetLength());

	Send2Server(TR3930, (char*)&Inrec, sizeof(Inrec), FALSE, 0);
	*/
	//>>
}

// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
CString	CStdDialog::GetRealItemCode( const char *p_szItemCode)
{
	if( !p_szItemCode) return "";
	if( !*p_szItemCode) return "";
	CString strItemCode( p_szItemCode);
	if( strItemCode.GetAt( 0) == '|') strItemCode = strItemCode.Mid( 3);
	return strItemCode;
}

// (2008/9/14 - Seung-Won, Bae) for Pair Tick
void CStdDialog::StartRequestPairTick( const char *p_strPairTickCode, int p_nRequestPairTickState)
{

	m_nRequestPairTickState = p_nRequestPairTickState;
	m_strPairTickCode = "|B_" + GetRealItemCode( p_strPairTickCode);
	m_ChartMng.GetpChartItemSelected()->SetCodeForQuery( m_strPairTickCode, QUERY_STRFX, TRUE);

	// (2008/9/24 - Seung-Won, Bae) delete C2_OBVIOUSLY_BALANCE_CHART in Tick
	m_ChartMng.GetpChartItemSelected()->RemoveChart( "�ϸ����ǥ");

	//>> (2008/12/22 - Seung-Lyong, Park) �ż�/�� �����͸� �����ϴ� ���� ���, �ż�/���� ���ÿ� ���̵��� ����
	m_ChartMng.GetpChartItemSelected()->LockWindowUpdate();
	//<< (2008/12/22 - Seung-Lyong, Park) �ż�/�� �����͸� �����ϴ� ���� ���, �ż�/���� ���ÿ� ���̵��� ����
}
void CStdDialog::EndRequestPairTick( void)
{
	m_nRequestPairTickState = E_RPTS_STOP;
	//>> (2008/12/22 - Seung-Lyong, Park) �ż�/�� �����͸� �����ϴ� ���� ���, �ż�/���� ���ÿ� ���̵��� ����
	if(m_ChartMng.GetpChartItemSelected() && m_ChartMng.GetpChartItemSelected()->GetSafeHwnd())
		m_ChartMng.GetpChartItemSelected()->UnlockWindowUpdate();
	//<< (2008/12/22 - Seung-Lyong, Park) �ż�/�� �����͸� �����ϴ� ���� ���, �ż�/���� ���ÿ� ���̵��� ����
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

CString CStdDialog::GetOrderType(){
	return m_pInputBarDlg->GetOrderType();
}

// FX��Ʈ �ֹ� ���� - onlyojt (20091204)
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
		strAccount.TrimRight();
		if (strAccount.IsEmpty ())
		{
			MessageBox("���¹�ȣ�� �Է��� �ֽʽÿ�", "��Ʈ�ֹ�");
			return;
		}
		if( strAccPW.GetLength() < 4 )
		{
			MessageBox("��й�ȣ�� �Է��� �ֽʽÿ�", "��Ʈ�ֹ�");
			m_pExtInputBarDlg->m_edAccPW.SetFocus();
			return;
		}


		m_ChartMng.GetpChartItemSelected()->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 5, 0);
		//m_ChartMng.GetpChartItemSelected()->InvokeAddIn("ChartKoreaSorimachiAddIn.dll:USER_COMMENT", 5, 0);

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
		// ��ü����ȸ
		Input.szChGB[0] = '2';
		st.m_strTRCode = QUERY_STRFX_CHE;	
		SendMessage(UMSG_SEND2SERVER,0,(long)&st);
		// �ܰ���ȸ
		st.m_strTRCode = QUERY_STRFX_JAN;	
		SendMessage(UMSG_SEND2SERVER,0,(long)&st);

		// ü����ȸ
		Input.szChGB[0] = '1';
		st.m_strTRCode = QUERY_STRFX_CHE2;	
		SendMessage(UMSG_SEND2SERVER,0,(long)&st);

		//CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrOpenPosition;
		//CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrPendingOrder;
		//pKSLibMng->Lib_KSLink_GetOrderLineInfo(strAccount, strCodeInputCode, (CObArray*)&arrOpenPosition, (CObArray*)&arrPendingOrder, this);

		//<<20100308_JS.Kim �ַθ�
		/*
		IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface (UUID_IKSLibManager, 2);
		if(!pKSLibMng)
			return;
		
		// ����
		CString strCodeInputCode = m_pInputBarDlg->m_ctrlCodeInput.GetDataText ();
		if (strCodeInputCode.IsEmpty ())
			return;

		// ���¹�ȣ
		CString strAccount = "";
		if (m_hViewInMain)
			::SendMessage (m_hViewInMain, RMSG_GET_CHARTACCOUNT, 0L, (LPARAM)&strAccount);

		if (strAccount.IsEmpty ())
			return;
		
		CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrOpenPosition;
		CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrPendingOrder;
		pKSLibMng->Lib_KSLink_GetOrderLineInfo(strAccount, strCodeInputCode, (CObArray*)&arrOpenPosition, (CObArray*)&arrPendingOrder, this);
		
		CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
		if(pChartItem)
		{
			pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 1, (long)&arrOpenPosition);
			pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 2, (long)&arrPendingOrder);
		}
		*/
		////////////////////>>
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


// FX��Ʈ �ֹ� ���� - onlyojt (20091204)
// �ż�/�ŵ�/����/��� -- �ֹ�����[nOrderType]
/*
	nOrderType
		[0] = ���尡 �ֹ�
		[1] = ������ �ֹ�
		[2] = ����
		[3] = ���
		[4] = stop/limit ���� (��ü��)
		[5] = stop/limit ��� (��ü��)
		[6] = û��
		[7] = stop/limit ���� (��û��)
		[8] = stop/limit ��� (��û��)
		[9] = Stop ���� ��ü��
		[10] = Limit ���� ��ü��
		[11] = Stop ���� ��û��
		[12] = Limit ���� ��û��
		[13] = �������� �ֹ�

	szOrgOrderID = �ű��ֹ��� ��쿣 ���� ����, �������� ���ֹ���ȣ�� �ִ�.
*/
BOOL CStdDialog::OnOrderInfomationChanged(const double dPirce, const int Qty, const int nOrderType, LPCTSTR szOrgOrderID,

							LPCTSTR	szCustItem1,			/*  [ȸ��ó���׸� 1] or �ŵ�/�ż� ����(0:�ŵ�,1:�ż�)	*/
							LPCTSTR	szCustItem2,			/*  [ȸ��ó���׸� 2]  �ֹ���ȣ							*/
							LPCTSTR	szCustItem3				/*  [ȸ��ó���׸� 3]									*/
						)
{
	if (!m_ChartMng.GetpChartItemSelected ()) 
		return FALSE;


	CString strItemCode = "";
	CString strAccNo = "", strAccName = "", strAccPW = "", strFDM = "", strOrderType = "���尡", strPipLowest = "";
	CString strPrice = "", strQty = "", strStopPrice = "";
	int	nMMGB = 2;
	int nDecPos(0);

	m_pExtInputBarDlg->GetAccountInfo(strAccNo, strAccName, strAccPW);

	strAccNo.TrimRight();
	if (strAccNo.IsEmpty ())
	{
		MessageBox("���¹�ȣ�� �Է��� �ֽʽÿ�", "��Ʈ�ֹ�");
		return FALSE;
	}

	if( strAccPW.GetLength() < 4 )
	{
		MessageBox("��й�ȣ�� �Է��� �ֽʽÿ�", "��Ʈ�ֹ�");
		m_pExtInputBarDlg->m_edAccPW.SetFocus();
		return FALSE;
	}

	if (m_ChartMng.GetpChartItemSelected()->m_lpSelChart)
		strItemCode = m_ChartMng.GetpChartItemSelected()->m_lpSelChart->m_strItemCode;
	else
		strItemCode = m_ChartMng.GetpChartItemSelected()->GetDataCode();
	
	ST_CHARTITEM_REALDATA* pChartItemRealData = m_ChartMng.GetpChartItemSelected()->GetChartItemRealData(strItemCode);
	if( pChartItemRealData )
	{
		strPipLowest = pChartItemRealData->m_strPipLowest;
		if ( strPipLowest.Find('.') >= 0 )
			nDecPos = strPipLowest.GetLength() - strPipLowest.Find(".") - 1;
	}

	strItemCode = GetRealItemCode(strItemCode);
	nMMGB = m_ChartMng.GetpChartItemSelected()->m_strOrderType == "0" ? 0 : 1;

	if( strItemCode.IsEmpty() )		return FALSE;

/*
	if( strItemCode.GetLength() > 2 )
	{
		if( strItemCode.Right(2) == ".I" )
		{
			strFDM = "IBFX";
			strItemCode = strItemCode.Left(strItemCode.GetLength()-2);
		}
		else if( strItemCode.Right(2) == ".H" )
		{
			strFDM = "HMWR";
			strItemCode = strItemCode.Left(strItemCode.GetLength()-2);
		}
		strItemCode.Replace('/', '-');
	}
*/
	strItemCode.Replace('/', '-');
	
	strQty.Format("%d", Qty);

	switch( nOrderType )
	{
	case ORDERTYPE_MARKET:
		strOrderType = "���尡";
		if( szCustItem1 )		nMMGB = atoi(szCustItem1);
		break;
	case ORDERTYPE_ENTRY:			// ������
	case ORDERTYPE_CROSS_ENTRY:		// ��������
		strOrderType = "������";
		if( szCustItem1 )		nMMGB = atoi(szCustItem1);
		if( nDecPos > 0 )	strPrice.Format("%.*f", nDecPos, dPirce);
		else				strPrice.Format("%f", dPirce);
		break;
	case ORDERTYPE_ENTRY2:			// double click (�������� �ŵ�/�ż� ���� �ֹ�ȭ��)
		strOrderType = "������";
		if( nDecPos > 0 )	strPrice.Format("%.*f", nDecPos, dPirce);
		else				strPrice.Format("%f", dPirce);
		break;

	case ORDERTYPE_STOP:		
	case ORDERTYPE_CON_STOP:		//��û�� Stop/Limit ����
		strOrderType = "Stop";
		nMMGB = 6;
		if( nDecPos > 0 )	strStopPrice.Format("%.*f", nDecPos, dPirce);
		else				strStopPrice.Format("%f", dPirce);
		break;
	case ORDERTYPE_CHANGE:
		nMMGB = 2;
		strOrderType = "������";
		if( nDecPos > 0 )	strPrice.Format("%.*f", nDecPos, dPirce);
		else				strPrice.Format("%f", dPirce);
		break;
	case ORDERTYPE_STOP_ONLY:		//Stop ���� ��ü��
	case ORDERTYPE_LIMIT_ONLY:		//Limit ���� ��ü��
		nMMGB = 2;
		strOrderType = "Stop";
		if( nDecPos > 0 )	strStopPrice.Format("%.*f", nDecPos, dPirce);
		else				strStopPrice.Format("%f", dPirce);
		strPrice = strStopPrice;
		break;
	case ORDERTYPE_CANCEL:
	case ORDERTYPE_LIMIT:			//Stop/Limit ���
		nMMGB = 3;
		break;
	case ORDERTYPE_CLEAR:		//�ֹ�û��
		nMMGB = 5;
		break;

	case ORDERTYPE_CON_LIMIT :		//��û�� Stop/Limit ���
		nMMGB = 6;
		break;
	}

	CString strCustItem2 = szCustItem2;
	strCustItem2.TrimLeft();		strCustItem2.TrimRight();
	ConfirmMap(	strAccNo,		//	CString strAccount		: ���¹�ȣ
				strAccName,		//  CString strAccountName	: ���¸�
				strAccPW,		//	CString strPassword		: ��й�ȣ
				strItemCode,	//	CString strCodeName		: ����
				strFDM,			//	CString strFDM 			: FDM 
				strOrderType,	//	CString strOrderType	: �ֹ�����(����/���尡 ��...)
				strQty,			//	CString strQuantity		: ����
				strPrice,		//	CString strPrice		: ����
				strCustItem2,	//	CString strJmNum		: �ֹ���ȣ
				strStopPrice,	//	CString strStopPrice	:
				nMMGB,			//	int nOrderType			: (0-�ŵ� 1-�ż� 2-���� 3-��� 4-T.STOP 5-û�� 6-STOP/LIMIT)
				nOrderType == ORDERTYPE_ENTRY2 ? 1 : 0 );	// int nMapType : �ֹ�ȭ�� ����


//<<20100308_JS.Kim �ַθ�
	/*
	CString strMsg;

	CString		strItem1 = szCustItem1;
	CString		strItem2 = szCustItem2;
	CString		strItem3 = szCustItem3;

	//<<< for test...
	strMsg.Format("kmlee : OnOrderInfomationChanged %f, %d, %d, %s", dPirce, Qty, nOrderType, szOrgOrderID);
	// AfxMessageBox(strMsg);
	OutputDebugString (strMsg);
	//>>>

	CString		strOrderNo = szOrgOrderID;
	FX_CHE_STRUCT	*pInfo = NULL;					// �ֹ� ���� ã��..

	// �����ڵ�
	CString strCodeInputCode = m_pInputBarDlg->m_ctrlCodeInput.GetDataText();
	strCodeInputCode.TrimRight ();
	

	CString strCode = CStdDialog::GetRealItemCode(m_pInputBarDlg->GetCurrentCode ());
	CString	strCodeName = m_pInputBarDlg->GetJongMokNameByCode (strCode);
	
	CString strAccount = "";
	if (m_hViewInMain)
		::SendMessage (m_hViewInMain, RMSG_GET_CHARTACCOUNT, 0L, (LPARAM)&strAccount);
	strAccount.TrimRight ();

	if (strCodeInputCode.IsEmpty () || strAccount.IsEmpty ())
	{
		OutputDebugString ("���¹�ȣ�� ���ų�, �����ڵ尡 �����ϴ�.!!");
		return FALSE;

	}

//************************************************************
// [ST]ü��/��ü�� ����Ʈ���� ����Ÿ �˻��ϱ�
//************************************************************
	IKSLibManager* pKSLibMng = NULL;
	CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrOpenPosition;
	CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrPendingOrder;

	// �ű��ֹ��� �ƴϸ�.. ���� ü��/
	if (nOrderType != ORD_TYPE_MARKETORDER && nOrderType != ORD_TYPE_ORDER && nOrderType != ORD_TYPE_ORDER_REVERSE)
	{
		pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 2);
		if (pKSLibMng)
		{
			pKSLibMng->Lib_KSLink_GetOrderLineInfo(strAccount, strCodeInputCode, (CObArray*)&arrOpenPosition, (CObArray*)&arrPendingOrder, this);
		}
		else
		{
			OutputDebugString ("pKSLibMng == NULL!!");
			return FALSE;
		}

	}

	if (nOrderType == ORD_TYPE_MODIFY ||						// ����
		nOrderType == ORD_TYPE_CANCEL ||						// ���
		nOrderType == ORD_TYPE_STOPLIMIT_APLY1 ||				// stop/limit ���� (��ü��)
		nOrderType == ORD_TYPE_STOPLIMIT_CANCEL1 ||				// stop/limit ��� (��ü��)
		nOrderType == ORD_TYPE_STOP_APPLY3 ||					// stop ���� (��ü��)
		nOrderType == ORD_TYPE_LIMIT_APPLY3)					// limit ���� (��ü��)
	{
		// ��ü�᳻������ ����Ÿ ã��..
		for (int i = 0; i < arrPendingOrder.GetSize (); i++)
		{
			FX_CHE_STRUCT	*pInfoTemp = NULL;
			pInfoTemp = (FX_CHE_STRUCT *)arrPendingOrder.GetAt (i);

			if (pInfoTemp)
			{
				if (!memcmp (szCustItem1, pInfoTemp->szCustItem1, sizeof (pInfoTemp->szCustItem1)) &&
					!memcmp (szCustItem2, pInfoTemp->szCustItem2, sizeof (pInfoTemp->szCustItem2)) &&
					!memcmp (szCustItem3, pInfoTemp->szCustItem3, sizeof (pInfoTemp->szCustItem3)))
				{
					pInfo = pInfoTemp;
					break;
				}
			}
		}
	}
	else if (nOrderType == ORD_TYPE_CLEAR ||					// û��
			 nOrderType == ORD_TYPE_STOPLIMIT_APLY2 ||			// stop/limit ���� (��û��)
			 nOrderType == ORD_TYPE_STOPLIMIT_CANCEL2 ||		// stop/limit ��� (��û��)
			 nOrderType == ORD_TYPE_STOP_APPLY4 ||				// stop ���� (��û��)
			 nOrderType == ORD_TYPE_LIMIT_APPLY4 )				// limit ���� (��û��)
	{
		// ü�᳻������ ����Ÿ ã��..
		for (int i = 0; i < arrOpenPosition.GetSize (); i++)
		{
			FX_CHE_STRUCT	*pInfoTemp = NULL;
			pInfoTemp = (FX_CHE_STRUCT *)arrOpenPosition.GetAt (i);
			if (pInfoTemp)
			{
				if (!memcmp (szCustItem1, pInfoTemp->szCustItem1, sizeof (pInfoTemp->szCustItem1)) &&
				    !memcmp (szCustItem2, pInfoTemp->szCustItem2, sizeof (pInfoTemp->szCustItem2)) &&
				    !memcmp (szCustItem3, pInfoTemp->szCustItem3, sizeof (pInfoTemp->szCustItem3)))
				{
					pInfo = pInfoTemp;
					break;
				}
			}
		}
	
	}

	if( !pInfo && nOrderType != ORD_TYPE_MARKETORDER && nOrderType != ORD_TYPE_ORDER && nOrderType != ORD_TYPE_ORDER_REVERSE)
	{
		AfxMessageBox("�ֹ������� �ҷ��� �� �����ϴ�. ��ü�� �� ��û�� ������ Ȯ���Ͽ� �ֽñ� �ٶ��ϴ�.");
		return 0;
	}

	//<<< for test...
	if (pInfo)
	{
		strMsg.Format ("�ֹ�Ÿ��[%d]", nOrderType); 
		OutputDebugString (strMsg);
	}
	//>>>
//************************************************************
// [END]ü��/��ü�� ����Ʈ���� ����Ÿ �˻��ϱ�
//************************************************************
			
	// �ֹ�ȭ�� �ε�
	//<<< 20091212 by kmlee
	HWND		hSignalWnd = (HWND)KSLink_GetEnvInfo (2);

	if (hSignalWnd == NULL)
		return 0;

	// main���� ������ �ֹ����� �ڷᱸ��..
	SIGNALORDERSINFO	signalOrdersInfo;
	memset (&signalOrdersInfo, 0, sizeof (SIGNALORDERSINFO));

	// ���� ���
	CString strPrecision, strTickSize;
	dll_GetUnitInfo (strCode, "", strPrecision, strTickSize);
	strPrecision.Format ("%%.%df", atoi (strPrecision));

	signalOrdersInfo.hDLLWnd = (long)GetSafeHwnd ();			// ȣ���� ��Ʈ�� �ڵ�

	strcpy (signalOrdersInfo.szIndexNo,		"20");
	strcpy (signalOrdersInfo.szWindName,	"FXíƮ");		 
	strcpy (signalOrdersInfo.szSignalgb,	"��"); 
	strcpy (signalOrdersInfo.szPopgb,		"2");
	strcpy (signalOrdersInfo.szActionGb,	"1");

	strcpy (signalOrdersInfo.szAcctno,		strAccount);
	strcpy (signalOrdersInfo.szCode,		strCode);

	// ���尡[0], ������[1], ����[2], ���[3], stop/limit ���� (��ü��)[4]
	// stop/limit ��� (��ü��)[5], û��[6], stop/limit ���� (��û��)[7], 
	// stop/limit ��� (��û��) [8]
	switch (nOrderType)
	{
	case ORD_TYPE_MARKETORDER :							// �űԽ��尡
		
		strcpy (signalOrdersInfo.szMemegb, "0");
		strcpy (signalOrdersInfo.szJanGogb, "0");		// �ż�
		if (dPirce == 0)
			strcpy (signalOrdersInfo.szOrdgb, "2");		// ���尡
		else
			strcpy (signalOrdersInfo.szOrdgb, "0");		// ������
		break;						

	case ORD_TYPE_ORDER :								// �ű�������

		strcpy (signalOrdersInfo.szMemegb, "0");
		strcpy (signalOrdersInfo.szJanGogb, "0");		// �ż�
		if (dPirce == 0)
			strcpy (signalOrdersInfo.szOrdgb, "2");		// ���尡
		else
			strcpy (signalOrdersInfo.szOrdgb, "0");		// ������
		break;

	case ORD_TYPE_ORDER_REVERSE:
		strcpy (signalOrdersInfo.szMemegb, "0");
		strcpy (signalOrdersInfo.szJanGogb, "0");		// �ż�
		strcpy (signalOrdersInfo.szOrdgb, "1");			// ��������

		break;

	case ORD_TYPE_MODIFY :										// ����

		strcpy (signalOrdersInfo.szMemegb,	"4");
		strcpy (signalOrdersInfo.szOrdgb,	"1");				// ����
		if (memcmp (pInfo->szSide, "079", 3) == 0)
			strcpy (signalOrdersInfo.szJanGogb,	"0");			// �ż� ����
		else
			strcpy (signalOrdersInfo.szJanGogb,	"1");			// �ŵ� ����
		strcpy (signalOrdersInfo.szMsg, szOrgOrderID);			// ���ֹ���ȣ
		break;

	case ORD_TYPE_CANCEL :										// ���

		strcpy (signalOrdersInfo.szMemegb,	"4");
		strcpy (signalOrdersInfo.szOrdgb,	"0");				// ���
		if (memcmp (pInfo->szSide, "079", 3) == 0)
			strcpy (signalOrdersInfo.szJanGogb,	"0");			// �ż� ���
		else
			strcpy (signalOrdersInfo.szJanGogb,	"1");			// �ŵ� ���
		strcpy (signalOrdersInfo.szMsg, szOrgOrderID);			// ���ֹ���ȣ
		break;

	case ORD_TYPE_STOPLIMIT_APLY1 :								// stop/limit ���� (��ü��)
	case ORD_TYPE_STOPLIMIT_APLY2 :								// stop/limit ���� (��û��)
	case ORD_TYPE_STOP_APPLY3:
	case ORD_TYPE_LIMIT_APPLY3:
	case ORD_TYPE_STOP_APPLY4:
	case ORD_TYPE_LIMIT_APPLY4:

		strcpy (signalOrdersInfo.szMemegb,	"3");
		strcpy (signalOrdersInfo.szOrdgb, "0");					// ���� (�߰�, �ű�)
		if (memcmp (pInfo->szKeyType, "OR01", 4) == 0)
		{
			strcpy (signalOrdersInfo.szJanGogb,	"0");			// ��ü��
			strcpy (signalOrdersInfo.szMsg, szOrgOrderID);		// ���ֹ���ȣ
		}
		else
		{
			strcpy (signalOrdersInfo.szJanGogb,	"1");			// ��û��
			memcpy (signalOrdersInfo.szMsg, pInfo->szFXCMPosID, sizeof (pInfo->szFXCMPosID));	// ü���ȣ
		}
		if (memcmp (pInfo->szSide, "079", 3) == 0)
			strcpy (signalOrdersInfo.szSygb,	"0");			// �ż� ���
		else
			strcpy (signalOrdersInfo.szSygb,	"1");			// �ŵ� ���

		break;

	case ORD_TYPE_STOPLIMIT_CANCEL1 :							// stop/limit ��� (��ü��)
	case ORD_TYPE_STOPLIMIT_CANCEL2 :							// stop/limit ��� (��û��)

		strcpy (signalOrdersInfo.szMemegb,	"3");
		strcpy (signalOrdersInfo.szOrdgb,	"2");				// ���
		if (memcmp (pInfo->szKeyType, "OR01", 4) == 0)
		{
			strcpy (signalOrdersInfo.szJanGogb,	"0");			// ��ü��
			strcpy (signalOrdersInfo.szMsg, szOrgOrderID);		// ���ֹ���ȣ
		}
		else
		{
			strcpy (signalOrdersInfo.szJanGogb,	"1");			// ��û��
			memcpy (signalOrdersInfo.szMsg, pInfo->szFXCMPosID, sizeof (pInfo->szFXCMPosID));	// ü���ȣ
		}
		break;

	case ORD_TYPE_CLEAR :										// û��

		strcpy (signalOrdersInfo.szMemegb,	"2");
		strcpy (signalOrdersInfo.szJanGogb,	"1");				// û���� ���� �׻� '0'

		if (memcmp (pInfo->szSide, "079", 3) == 0)
			strcpy (signalOrdersInfo.szOrdgb,	"0");			// �ż� û��
		else
			strcpy (signalOrdersInfo.szOrdgb,	"1");			// �ŵ� û��
		memcpy (signalOrdersInfo.szMsg, pInfo->szFXCMPosID, sizeof (pInfo->szFXCMPosID));	// ü���ȣ
		break;

	}

	// �ֹ���ȣ Trim..
	CString szTempOrdNo = signalOrdersInfo.szMsg;
	szTempOrdNo.TrimRight ();
	strcpy (signalOrdersInfo.szMsg, szTempOrdNo.operator LPCTSTR  ());

	sprintf (signalOrdersInfo.szOrdqty, "%d", Qty);
	// �ֹ�����
	if( nOrderType == ORD_TYPE_MARKETORDER || nOrderType == ORD_TYPE_ORDER || nOrderType == ORD_TYPE_ORDER_REVERSE || 
		nOrderType == ORD_TYPE_MODIFY || nOrderType == ORD_TYPE_CANCEL || nOrderType == ORD_TYPE_CLEAR)
	{
		// ���尡, ������, ����, ���, û���϶��� szOrdprc�� �̵������� �־��ش�
		sprintf (signalOrdersInfo.szOrdprc,	strPrecision, dPirce);
	}
	else if( nOrderType == ORD_TYPE_MODIFY || nOrderType == ORD_TYPE_STOPLIMIT_CANCEL1 || 
			nOrderType == ORD_TYPE_STOPLIMIT_APLY2 || nOrderType == ORD_TYPE_STOPLIMIT_CANCEL2 ||
			nOrderType == ORD_TYPE_STOPLIMIT_APLY1 || nOrderType == ORD_TYPE_STOPLIMIT_CANCEL1 )
	{
		// Stop/Limit ����/����϶��� �̵������� ������� �ֹ�ȯ���� �־��־����
		memcpy (signalOrdersInfo.szOrdprc,	pInfo->szRate,  sizeof (pInfo->szRate));

		memcpy (signalOrdersInfo.szOrdStop,		pInfo->szStop,  sizeof (pInfo->szStop));		// stop
		memcpy (signalOrdersInfo.szOrdLimit,	pInfo->szLimit, sizeof (pInfo->szLimit));		// limit
	}
	else if( nOrderType == ORD_TYPE_STOP_APPLY3 || nOrderType == ORD_TYPE_STOP_APPLY4 )
	{
		// Stop �����϶��� �ֹ�ȯ�� �Է�
		memcpy (signalOrdersInfo.szOrdprc,	pInfo->szRate,  sizeof (pInfo->szRate));
		sprintf (signalOrdersInfo.szOrdStop,	strPrecision, dPirce);
	}
	else if( nOrderType == ORD_TYPE_LIMIT_APPLY3 || nOrderType == ORD_TYPE_LIMIT_APPLY4 )
	{
		// Stop �����϶��� �ֹ�ȯ�� �Է�
		memcpy (signalOrdersInfo.szOrdprc,	pInfo->szRate,  sizeof (pInfo->szRate));
		sprintf (signalOrdersInfo.szOrdLimit,	strPrecision, dPirce);
	}

// 	if (nOrderType != ORD_TYPE_MARKETORDER && nOrderType != ORD_TYPE_ORDER)
// 	{
// 		memcpy (signalOrdersInfo.szOrdStop,		pInfo->szStop,  sizeof (pInfo->szStop));		// stop
// 		memcpy (signalOrdersInfo.szOrdLimit,	pInfo->szLimit, sizeof (pInfo->szLimit));		// limit
// 		sprintf (signalOrdersInfo.szOrdTSPip, "%ld", pInfo->nTrailingStop);						// Trailing Stop Pip
// 	}

	strcpy (signalOrdersInfo.szExecRangeGb,	"0");			// �ֹ���ȿ�Ⱓ(ü�����, AtBest : 0, Range : 1)
	strcpy (signalOrdersInfo.szExecRange,	"0");			// �ֹ���ȿ�Ⱓ(ü����� Spin��)

	::SendMessage(hSignalWnd, UM_SM_SIGNALORDER_MSG, (WPARAM)0L, (long)&signalOrdersInfo);
*/
//>>
	return TRUE;
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

//<<< 20100107 by kmlee
BOOL CStdDialog::DoOrderFromSTSignal (WPARAM wp, LPARAM lp)
{
//<<20100308_JS.Kim �ַθ�  
	/*
	STORDER_INDATA2* lpOrderData = (STORDER_INDATA2*)wp;
	CSTConfigEnv* lpConfigEnv = (CSTConfigEnv*)lp;

	if (lpOrderData == NULL || lpConfigEnv == NULL )
		return 0;

	HWND		hSignalWnd = (HWND)KSLink_GetEnvInfo (2);

	if (hSignalWnd == NULL)
		return 0;
	
	CString strCode = lpConfigEnv->m_szCode;
	strCode.Right (7);

	CString	strCodeName = m_pInputBarDlg->GetJongMokNameByCode (strCode);
	CString strAccount = lpOrderData->aAcctNo;

	SIGNALORDERSINFO	signalOrdersInfo;
	memset (&signalOrdersInfo, 0, sizeof (SIGNALORDERSINFO));


	signalOrdersInfo.hDLLWnd = (long)GetSafeHwnd ();			// ȣ���� ��Ʈ�� �ڵ�

	strcpy (signalOrdersInfo.szIndexNo,		"20");
	strcpy (signalOrdersInfo.szWindName,	"FXíƮ");		 
	strcpy (signalOrdersInfo.szSignalgb,	"��"); 
	strcpy (signalOrdersInfo.szPopgb,		"2");
	strcpy (signalOrdersInfo.szActionGb,	"1");

	strcpy (signalOrdersInfo.szAcctno,		strAccount);
	strcpy (signalOrdersInfo.szCode,		strCode);

	switch (lpConfigEnv->m_nOrderType)
	{
	case 0 :		// �ż� �ű�
		strcpy (signalOrdersInfo.szMemegb, "0");
		strcpy (signalOrdersInfo.szJanGogb, "0");		// �ż�
		strcpy (signalOrdersInfo.szOrdgb, "2");			// ���尡
		break;

	case 1 :		// �ż� û��
		strcpy (signalOrdersInfo.szMemegb,	"2");
		strcpy (signalOrdersInfo.szJanGogb,	"1");		// û���� ���� �׻� '0'
		strcpy (signalOrdersInfo.szOrdgb,	"0");		// �ż� û��
		break;

	case 2 :		// �ŵ� û��
		strcpy (signalOrdersInfo.szMemegb,	"2");
		strcpy (signalOrdersInfo.szJanGogb,	"1");		// û���� ���� �׻� '0'
		strcpy (signalOrdersInfo.szOrdgb,	"1");		// �ż� û��
		break;

	case 3 :		// �ŵ� �ű�
		strcpy (signalOrdersInfo.szMemegb, "1");
		strcpy (signalOrdersInfo.szJanGogb, "1");		// �ż�
		strcpy (signalOrdersInfo.szOrdgb, "2");			// ���尡
		break;
	}

	sprintf (signalOrdersInfo.szOrdqty, "%d", atoi (lpOrderData->aOrderAmt));
	strcpy (signalOrdersInfo.szOrdprc,	"0");			// ���尡 ����


	strcpy (signalOrdersInfo.szExecRangeGb,	"0");			// �ֹ���ȿ�Ⱓ(ü�����, AtBest : 0, Range : 1)
	strcpy (signalOrdersInfo.szExecRange,	"0");			// �ֹ���ȿ�Ⱓ(ü����� Spin��)

	::SendMessage(hSignalWnd, UM_SM_SIGNALORDER_MSG, (WPARAM)0L, (long)&signalOrdersInfo);
	//>>>
	*/
	//>>
	return FALSE;
}
//>>>

//<<20100308_JS.Kim �ַθ�
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
	if (strTRCode != QUERY_STRFX)
		return;

	m_strOut = "";
	if (pData == NULL)
		return;

	SM_fx112_OutRec1	singOutput;
	memcpy(&singOutput, pData, sizeof(SM_fx112_OutRec1));

	CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
	ST_SELCHART *pSelChart = pChartItem->m_lpSelChart;

	if (!pSelChart)
		pChartItem->GetSelChart(pChartItem->GetSelectedRQ(), pSelChart);

	CString strTerm = pChartItem->GetTickMinDayNumberRequested();
	char chTypeRequested = pChartItem->GetTypeRequested();
	bool	bSellChart = false;
	if (pSelChart)
	{
		pSelChart->m_strKey = CString(singOutput.nkey, sizeof(singOutput.nkey));
		//<<20100308_JS.Kim �ַθ�
		//pSelChart->m_nOrgTimeIndex = atoi("22");
		//>>
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

		if( singOutput.mgubun[0] == '2' && pSelChart->m_strItemCode.Left(3) == "|S_" )	// �ŵ�
			bSellChart = true;
	}
/*
//JS.Kim
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
		strStartDate = CString(singOutput.startDate, sizeof(singOutput.startDate));
		strEndDate = CString(singOutput.endDate, sizeof(singOutput.endDate));

		//	��ȣ
		if (!strSign.IsEmpty())		pChartItemRealData->m_chSign = strSign.GetAt(0);
		else						pChartItemRealData->m_chSign = ' ';

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
	m_strOut.Format(_T("%s"), (LPTSTR)(char*)(pData+sizeof(SM_fx112_OutRec1)));
*/
	SM_fx112_CHART* pChart = (SM_fx112_CHART *)((char*)pData + sizeof(SM_fx112_OutRec1));
	char pCnt[12];
	sprintf(pCnt, "%*.*s", sizeof(pChart->fcnt), sizeof(pChart->fcnt), pChart->fcnt);
	int nCount = atoi(pCnt);

	if( E_RPTS_PAIR == GetRequestPairTickState() || bSellChart )
	{
		/*
		int nLen = (sizeof(SM_fx112_OutRec1) + sizeof(SM_fx112_CHART) - 1);
		if( nLen > dwTRDateLen )	return;
		int nLen2 = nCount * sizeof(SM_fx112_OutRec2);
		if( nLen + nLen2 > dwTRDateLen )	return;
		nLen2 = min(nLen2, dwTRDateLen-nLen-nLen2);
		m_strOut.Format(_T("%*.*s%*.*s"), 
							nLen, nLen, (LPTSTR)(char*)(pData), 
							nLen2, nLen2, (LPTSTR)((char*)pData + nLen + nLen2));
		*/
		/*
		int nLen = sizeof(SM_fx112_OutRec1);
		if( nLen > dwTRDateLen )	return;
		int nLen2 = sizeof(SM_fx112_CHART) - 1 + nCount * sizeof(SM_fx112_OutRec2);
		if( nLen + nLen2 > dwTRDateLen )	return;
		nLen2 = min(nLen2, dwTRDateLen-nLen-nLen2);

		m_strOut.Format(_T("%*.*s%*.*s"), 
							nLen, nLen, (LPTSTR)(char*)(pData), 
							nLen2, nLen2, (LPTSTR)((char*)pData + nLen + nLen2));
		*/
		int nLen = (sizeof(SM_fx112_OutRec1) + sizeof(SM_fx112_CHART) - 1) + nCount * sizeof(SM_fx112_OutRec2);
		if( nLen > dwTRDateLen )	return;

		int nLen2 = (sizeof(SM_fx112_CHART) - 1) + nCount * sizeof(SM_fx112_OutRec2);
		nLen2 = min(nLen2, dwTRDateLen);


		m_strOut.Format(_T("%*.*s%*.*s"), sizeof(SM_fx112_OutRec1), sizeof(SM_fx112_OutRec1), (LPTSTR)(char*)(pData),
							nLen2, nLen2, (LPTSTR)((char*)pData + nLen));
	}
	else
	{
		int nLen = (sizeof(SM_fx112_OutRec1) + sizeof(SM_fx112_CHART) - 1) + nCount * sizeof(SM_fx112_OutRec2);
		nLen = min(nLen, dwTRDateLen);
		m_strOut.Format(_T("%*.*s"), nLen, nLen, (LPTSTR)(char*)(pData));
	}

}

void CStdDialog::DoShareCode(LPCSTR _szCode)
{
	//@100304 ����(���̳�)�� ������ ������ ������� �Ѵ�.
	//	if(m_nMatrix>=1) return;

	if(!m_bNotifyChangeCode) return;
	if(g_pPtUtilMng)
	{
		//<<20100312_JS.Kim FX 
		/*
		//@Solomon	���������� ��� ���� ���� ����.	-->
		if (m_pInputBarDlg && m_pInputBarDlg->m_pFOHisBtn)
		{
			CString strCode;
			strCode.Format(_T("%s"), _szCode);
			int nCodeType = m_pInputBarDlg->m_pFOHisBtn->GetCodeType(strCode);
			switch (nCodeType)
			{
			case futureType:	// ����
			case spreadType:	// ��������
			case callType:		// �ݿɼ�
			case putType:		// ǲ�ɼ�
			case sfutureType:	// �ֽļ���
			case koptionType:	// �ֽĿɼ�
				return;
			}
		}
		*/
		//@Solomon	���������� ��� ���� ���� ����.	<--
		g_pPtUtilMng->ShareCode(m_hPlatform, _szCode);
	}
}


//---------------------------------------------------------------------------
// ù ��ȸ�� ���� ��
// step-1. �ڵ���ȸ�ϵ��� ������ ������ �ִ��� üũ.
// step-2. 1���� �ش��ϴ� ������ ������ �ֱ� ��ȸ�������� ��ȸ.
// step-3. 2������ ��ȸ�� �ȵɰ�� -> ����Ʈ �ڵ�� ��ȸ.
//			�ֽ�:�ַθ��������� ��ȸ.
//			����:001 �ڽ����������� ��ȸ.
//			����:�ֱ� �����ڵ�� ��ȸ.
//			�ؿ�:
BOOL CStdDialog::GetFirstCode(CString &rString, int nMarketType, int &nSetp)
{
	BOOL bRet = TRUE;
	if (IsChartScreen("OldFuture") == TRUE)
		return FALSE;

	rString.Empty();
	nSetp=0;

//@Solomon	DominoData ����. -->
	BOOL bDomino = FALSE;
	int nCount = m_mapDominoCode.GetCount();
	if ((m_nMatrix == 0) && nCount > 0)
		bDomino = TRUE;

	char szTemp[32];
	memset(szTemp, 0x00, sizeof(szTemp));
	::GetPrivateProfileString("0000", "Code00", "", szTemp, sizeof(szTemp), m_strConfig);

	rString.Format(_T("%s"), szTemp);
	// FO������Ʈ������ �����丮�� ��������.
	if(GetScreenNo()=="0515") return TRUE;

// 	else
// 	{
// 		m_pInputBarDlg->m_bChkLoadSaveCode = FALSE;
// 		rString = m_mapDominoCode.;
// 		m_strDominoCode = _T("");
// 		bDomino = TRUE;
// 	}
//@Solomon	DominoData ����. <--

	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);

	if (rString.GetLength() > 0)
	{
		if (nMarketType < 0)
		{
			nMarketType = pMasterDataManager->GetMarketTypeFromCode(rString);
			if (nMarketType == CMasterTypeInfo::NONE_CHART)
			{
				nMarketType = m_nMarketType;
				rString = "";
			}
			else
				m_nMarketType = nMarketType;
		}
	}

	if (nMarketType < 0)
		nMarketType = m_nMarketType;

	CStringArray arrCode, arrName;
	CString rCode;

	switch (nMarketType)
	{
	case CMasterTypeInfo::STOCK_CHART:
		{
			//if(m_nMatrix>=1)
			{
				CString szInitKey;
				szInitKey.Format("OnLoadHistory_Init_%d", m_nMatrix);
				if(m_mapDominoCode.Lookup(szInitKey, rCode)==TRUE)
					rString = rCode;
				if(rCode.GetLength()>0)
					break;
			}
			if(m_nMatrix>=1)
			{
				rString.Empty();
				break;
			}

// 			if (bDomino && m_mapDominoCode.Lookup(_T("1301"), rCode))
// 			{
// 				nSetp = 0;
// 				rString = rCode;
// 				break;
// 			}

			//Step 1.
			IMainExInfoManagerLast* pMainExMng = (IMainExInfoManagerLast*)AfxGetPctrInterface(UUID_IMainExInfoManager);
			if(pMainExMng)
			{
				BOOL bSaved = (BOOL)pMainExMng->GetUserStringData("SET_STOCKCODE", rCode);
				if(rCode.GetLength()>0)
				{
					nSetp = 1;
					rString = rCode;
					break;
				}
			}

			//Step 2.
			//GetRecentFutureCode()
			pMasterDataManager->GetRecentCodeNameArray(nMarketType, arrCode, arrName);
			if(arrCode.GetSize()>0)
			{
				rCode = arrCode.GetAt(0);
				nSetp = 2;
				rString = rCode;
				break;
			}

			//Step 3.
			rCode = _T("007800");	//�ַθ���������.
			nSetp = 3;
			rString = rCode;
			break;
		}
		break;

	case CMasterTypeInfo::UPJONG_CHART:
		{
			CString szInitKey;
			szInitKey.Format("OnLoadHistory_Init_%d", m_nMatrix);
			if(m_mapDominoCode.Lookup(szInitKey, rCode)==TRUE)
				rString = rCode;
			if(rCode.GetLength()>0)
				break;
		}
		if(m_nMatrix>=1)
		{
			rString.Empty();
			break;
		}

// 		if (bDomino && m_mapDominoCode.Lookup(_T("20301"), rCode))
// 		{
// 			nSetp = 0;
// 			rString = rCode;
// 			break;
// 		}
		//Step 1. ��ȸ�ϵ��� ���õ� ����ã��
		
		//Step 2. �ֱ���ȸ����.
		pMasterDataManager->GetRecentCodeNameArray(nMarketType, arrCode, arrName);
		if(arrCode.GetSize()>0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 2;
			rString = rCode;
			break;
		}
		
		//Step 3. ����Ʈ �ڵ�� ��ȸ.
		rCode = _T("001");	//�ŷ��� ����
		nSetp = 3;
		rString = rCode;
		break;

	case CMasterTypeInfo::FUTOPT_CHART:
		{
			CString szInitKey;
			szInitKey.Format("OnLoadHistory_Init_%d", m_nMatrix);
			if(m_mapDominoCode.Lookup(szInitKey, rCode)==TRUE)
				rString = rCode;
			if(rCode.GetLength()>0)
				break;
		}
		if(m_nMatrix>=1)
		{
			rString.Empty();
			break;
		}
// 		if (bDomino && m_mapDominoCode.Lookup(_T("30301"), rCode))
// 		{
// 			nSetp = 0;
// 			rString = rCode;
// 			break;
// 		}
		//Step 1. ��ȸ�ϵ��� ���õ� ����ã��
		
		//Step 2. �ֱ���ȸ����.
		pMasterDataManager->GetRecentCodeNameArray(nMarketType, arrCode, arrName);
		if(arrCode.GetSize()>0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 2;
			rString = rCode;
			break;
		}
		
		//Step 3. ����Ʈ �ڵ�� ��ȸ.
		{
			CStringArray arCode, arName, arEngName;
			CStringArray *parCode = &arCode, *parName = &arName, *parEngName = &arEngName;
			pMasterDataManager->GetFutureJongMst(parCode, parName, parEngName);
			if(arCode.GetSize()>0)
			{
				rCode = arCode.GetAt(0);
				nSetp = 3;
				rString = rCode;
				break;
			}
		}

		rString = "";
		bRet = FALSE;
		break;
		
	case CMasterTypeInfo::STOCKFUTOPT_CHART:
		if (bDomino && m_mapDominoCode.Lookup(_T("30301"), rCode))
		{
			nSetp = 0;
			rString = rCode;
			break;
		}
		//Step 1. ��ȸ�ϵ��� ���õ� ����ã��
		
		//Step 2. �ֱ���ȸ����.
		pMasterDataManager->GetRecentCodeNameArray(nMarketType, arrCode, arrName);
		if(arrCode.GetSize()>0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 2;
			rString = rCode;
			break;
		}

		rString = "";
		bRet = FALSE;
		break;
		
	case CMasterTypeInfo::COMMODITY_CHART:
		//100208-alziyes. ��ǰ���� dominoȮ���� ��.
		if (bDomino && m_mapDominoCode.Lookup(_T("cfcod"), rCode))
		{
			nSetp = 0;
			rString = rCode;
			break;
		}
		//Step 1. ��ȸ�ϵ��� ���õ� ����ã��
		
		//Step 2. �ֱ���ȸ����.
		pMasterDataManager->GetRecentCodeNameArray(nMarketType, arrCode, arrName);
		if(arrCode.GetSize()>0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 2;
			rString = rCode;
			break;
		}
		
		pMasterDataManager->GetMasterDataInUpDetails("COMMODITYMASTER", arrCode, arrName, "NONE");

		if (arrCode.GetSize() > 0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 3;
			rString = rCode;
			break;
		}

		//Step 3. ����Ʈ �ڵ�� ��ȸ.
		rString = "";
		bRet = FALSE;
		break;
		
	case CMasterTypeInfo::FOREIGN_CHART:
		if (bDomino && m_mapDominoCode.Lookup(_T("50301"), rCode))
		{
			nSetp = 0;
			rString = rCode;
			break;
		}
		//Step 2. �ֱ���ȸ����.
		pMasterDataManager->GetRecentCodeNameArray(nMarketType, arrCode, arrName);
		if(arrCode.GetSize()>0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 2;
			rString = rCode;
			break;
		}
		
		//Step 3. ����Ʈ �ڵ�� ��ȸ.
		{
			rString = "DJI@DJC";
			nSetp = 3;
			break;
		}
		break;
	case CMasterTypeInfo::FUOPT_JIPYO_CHART:
		if (bDomino && m_mapDominoCode.Lookup(_T("109301"), rCode))
		{
			nSetp = 0;
			rString = rCode;
			break;
		}
		//Step 2. �ֱ���ȸ����.
		pMasterDataManager->GetRecentCodeNameArray(nMarketType, arrCode, arrName);
		if(arrCode.GetSize()>0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 2;
			rString = rCode;
			break;
		}

		//Step 3. ����Ʈ �ڵ�� ��ȸ.
		rCode = _T("PGKOSPIXPT");	
		nSetp = 3;
		rString = rCode;
		break;
	default:	bRet = FALSE;	break;
	}

	m_mapDominoCode.RemoveAll();

	return bRet;
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
//	GetFirstCode(rCode, m_nMarketType, nStep);

// 	//@solomon	�̴ϸ�Ƽ��Ʈ�ΰ�� ���������� ���.
// 	if (m_nChartType == MINI_SINGLE2_CHART)
// 		return ;

//@100303 ��Ʈ���� ȭ�鿡���� ó��.
//	if (m_nMatrix != 0)		return;

	GetFirstCode(rCode, -1, nStep);

	WPARAM wp = (WPARAM)(LPCSTR)rCode;
	LPARAM lp = m_nMarketType;
	SendMessage(UMSG_SENDSHAREDCODE, wp, lp);

//	if(GetFirstCode(rCode, nStep))
//		m_pInputBarDlg->m_strCodeFromOutside  = rCode;
//	m_pInputBarDlg->SetCodeInit();
	return;


// 	switch(m_nMarketType)
// 	{
// 		case STOCK_CHART:
// 			//if(nStep==1)	pMainExMng->SetUserStringData("SET_STOCKCODE", "");
// 			
// 			m_pInputBarDlg->m_pCodeInput->SetCode(rCode);
// 			m_pInputBarDlg->DoSelchangeCodeInput();
// 			return;
// 
// 		case UPJONG_CHART:
// 			m_pInputBarDlg->m_pCodeInput->SetCode(rCode);
// 			m_pInputBarDlg->DoSelchangeCodeInput();
// 			return;
// 
// 		case FUTOPT_CHART:
// 		case STOCKFUTOPT_CHART:
// 			m_pInputBarDlg->m_pCodeInput->SetCode(rCode);
// 			m_pInputBarDlg->DoSelchangeCodeInput();
// 			return;
// 
// 		case FOREIGN_CHART:
// 			break;
// 		default: break;
// 	}
}

BOOL CStdDialog::IsChartScreen(LPCSTR szKey)
{
	//m_nMarketType = CMasterTypeInfo::FUOPT_JIPYO_CHART;
	if(strcmp(szKey, "FUTOPT_CHART")==0)
	{
		if (GetScreenNo().Compare("0515")==0) //|| GetScreenNo().Compare("0516")==0)
			return TRUE;
	}

	if(strcmp(szKey, "OldFuture")==0)
	{
		if(GetScreenNo().Compare("0516")==0)
			return TRUE;
	}
// 		else if(strcmp(szKey, "FOChart")==0)
// 		{
// 			if(m_nMarketType == CMasterTypeInfo::FUOPT_JIPYO_CHART)
// 				return TRUE;
// 	//		if(GetScreenNo().Compare("0515")==0)
// 	//			return TRUE;
// 		}
	return FALSE;
}

#include "../../inc/AZUtilFileMng.h"
void CStdDialog::_DoVersionCheck_ChartDef()
{
	//100204-alzioyes. �������� ��ü������ �����ϱ� ���� �����.
	//return;// �� ����� �ʿ��� �������� ����Ѵ�.

	CString szChartCfg, szUserCfg;
	szChartCfg.Format("%s\\Icss\\ChartDef.cfg", m_strRootPath);
	szUserCfg.Format("%s\\ChartDef.cfg", m_strUserDir);

	char aDataVer[20]={0,}, aUserVer[20]={0,};
	::GetPrivateProfileString("Ver", "Current", "000000", aDataVer, 20, szChartCfg);
	::GetPrivateProfileString("Ver", "Current", "000000", aUserVer, 20, szUserCfg);

	if(strcmp(aDataVer, aUserVer)==0) return;

	DoClearUserChartData();
	::WritePrivateProfileString("Ver", "Current", aDataVer, szUserCfg);
}

//alzioyes ������� ��Ʈ���� ��������
void CStdDialog::DoClearUserChartData()
{
//--> Method 1.
	CString strUserPath(m_strUserDir);
	CString strExt;

	LPTSTR arExt[] = {	"*.mct", "*.cht", "*IndiCond*.*", "*ChartInfo*.dat", "*AnalTool*.*", 
		"*JipyoSync*.dat"};
	
	int nCount = sizeof(arExt)/sizeof(LPTSTR);
	for(int i=0; i<nCount; i++)
	{
		strExt = arExt[i];
		CAZUtil_FileManager::DoDirFileRemove(strUserPath, strExt, FALSE);
	}
//<--

//--> Method 2.
// 	CString szChartCfg;
// 	szChartCfg.Format("%s\\Hanaro\\ChartDef.cfg", m_strRootPath);
// 
// 	int nCount = ::GetPrivateProfileInt("ClearListA", "Count", 0, szChartCfg);
// 
// 	CString strUserPath(m_strUserDir);
// 	CString strExt;
// 	char aExt[100+1], aKey[4];
// 	for(int i=0; i<nCount; i++)
// 	{
// 		sprintf(aKey, "%d", i);
// 		if(::GetPrivateProfileString("ClearListA", aKey, "", aExt, 100, szChartCfg)>0)
// 		{
// 			strExt = aExt;
// 			CAZUtil_FileManager::DoDirFileRemove(strUserPath, strExt, FALSE);
// 		}
// 	}
//<--
	
//--> Method 3.
// 	CString strUserPath(m_strUserDir);
// 	CAZUtil_FileManager::DoDirFileRemove(strUserPath, "*.*", FALSE);
// 	
// 	CString szChartCfg, szUserCfg;
// 	szChartCfg.Format("%s\\Hanaro\\ChartDef.cfg", m_strRootPath);
// 	szUserCfg.Format("%s\\ChartDef.cfg", m_strUserDir);
// 	::CopyFile(szChartCfg, szUserCfg, FALSE);
//<--
}

void CStdDialog::_DoCheckDaultDataFile()
{
	//if(m_nScreenID==nScreenID_6301)
	{
		CString szScreenNo = GetScreenNo();
		CString szUserFile;
		szUserFile.Format("%s\\%s%s", m_strUserDir, szScreenNo, "_ChartInfo.dat");

//	�ӽ÷�ƾ ����. 
// 		if(szScreenNo.CompareNoCase("0515")==0)
//		{
// 			CString szChartCfg;
// 			szChartCfg.Format("%s\\Hanaro\\ChartDef.cfg", m_strRootPath);
// 	
// 			int nVer = GetPrivateProfileInt(szScreenNo, "SvrVer", 0, szChartCfg);
// 			if(nVer==0)
// 			{
// 				::DeleteFile(szUserFile);
// 			}
// 			::WritePrivateProfileString(szScreenNo, "svrVer", "1", szChartCfg);
// 		}
 		CFileFind file;
 		if(file.FindFile(szUserFile)==FALSE)
 		{
 			_DoCopyDefaultToUser(GetScreenNo());
  		}
	}
}

void CStdDialog::_DoCopyDefaultToUser(LPCSTR szSect)
{
	CString szChartCfg;
	szChartCfg.Format("%s\\Icss\\ChartDef.cfg", m_strRootPath);

	int nCnt = ::GetPrivateProfileInt(szSect, "Count", 0, szChartCfg);
	
	char aKey[4], aBuff[MAX_PATH];
	CString szFullName, szFrom, szTo, szDefFile, szUserFile;
	for(int i=0; i<nCnt; i++)
	{
		sprintf(aKey, "%d", i);
		if(::GetPrivateProfileString(szSect, aKey, "", aBuff, MAX_PATH, szChartCfg)>0)
		{			
			szFullName = aBuff;
			//���ϸ��� 32����Ʈ�� �Ѿ�� ��찡 �־ ������.
			AfxExtractSubString(szFrom, szFullName, 0, ';');
			if(szFrom.GetLength() < szFullName.GetLength())
			{
				szTo = szFullName.Mid(szFrom.GetLength()+1);
			}
			else
			{
				szFrom = szFullName;
				szTo = szFrom;
			}
			//szDefFile.Format("%s\\Hanaro\\%s", m_strRootPath, aBuff);
			//szUserFile.Format("%s\\%s", m_strUserDir, aBuff);
			szDefFile.Format("%s\\Icss\\%s", m_strRootPath, szFrom);
			szUserFile.Format("%s\\%s", m_strUserDir, szTo);
			::CopyFile(szDefFile, szUserFile, FALSE);
		}
	}
}

CString CStdDialog::GetScreenNo()
{
	CString strScreenNo;
	strScreenNo.Format("%04d", m_nScreenID);
	return strScreenNo;
}

CString CStdDialog::GetCodeType(CString strCode)
{
	CString strType = "";
	m_mapCodeType.Lookup(strCode, strType);

	return strType;
}

LRESULT CStdDialog::OnAccountInit(WPARAM wp, LPARAM lp)
{
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->m_hWnd)
		m_pExtInputBarDlg->InitAccountCtrl(wp, lp);

	return 0L;
}


//>>