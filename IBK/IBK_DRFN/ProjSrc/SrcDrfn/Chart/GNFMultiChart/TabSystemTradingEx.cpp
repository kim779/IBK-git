//-->090902-solomon : alzioyes ST������ @����.
// �ַθ��� ��� @�� ����ó���Ǵ� ���Ͽ� ����� �Ǹ� �ȵǱ� ������ 
// '@'�� '^'���� ������.
//-->090902-solomon : alzioyes ST������ @����.


// TabSystemTradingEx.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LeftBarDlg.h"
#include <mmsystem.h>

#include "./define/ChartMsgDef.h"
#include "../Common_ST/STControllerDef.h"
#include "../Common_ST/STControllerDef2.h"
#include "../Common_ST/STPathDefine.h"

#include "TabSystemTradingEx.h"
#include "StdDialog.h"
#include "../Include_ST/STStructDef.h"		// ChartSTIndexAddin�� ���� �������� �� ���࿩�θ� üũ�ϰ��� ���.
#include "../../inc/ICompanyMng.h"
#include "../Common_ST/STConfigEnv.h"
#include "../../inc/IMainInfoMng.h"
#include ".\dlldlg.h"

#include "../CommonTr/KB_f0201.h"
#include "../CommonTr/KB_01230.h"

#define		IDC_OUTLOOKBAR			1000
#define		IDC_STListCtrl			1001
#define		Y_GAP					2

const UINT RMSG_OB_ITEMCLICK	= ::RegisterWindowMessage("RMSG_OB_ITEMCLICK");
const UINT RMSG_OB_ITEMDBLCLK	= ::RegisterWindowMessage("RMSG_OB_ITEMDBLCLK");
const UINT RMSG_RUN_MULTIST		= ::RegisterWindowMessage("RMSG_RUN_MULTIST");
const UINT RMSG_INDEXADDIN_NOTIFY_RAWDATA = ::RegisterWindowMessage("RMSG_INDEXADDIN_NOTIFY_RAWDATA");
const UINT RMSG_INDEXADDIN_NOTIFY_BUYSELL = ::RegisterWindowMessage("RMSG_INDEXADDIN_NOTIFY_BUYSELL");
const UINT RMSG_INDEXADDIN_NOTIFY_SIGNAL  = ::RegisterWindowMessage("RMSG_INDEXADDIN_NOTIFY_SIGNAL");
const UINT RMSG_DESTROY_CONFIRMWND = ::RegisterWindowMessage("RMSG_DESTROY_CONFIRMWND");
const UINT RMSG_GET_LINKINFODATA = ::RegisterWindowMessage(_T("RMSG_GET_LINKINFODATA"));
const UINT RMSG_GET_SCREENVIEWHANDLE = ::RegisterWindowMessage(_T("RMSG_GET_SCREENVIEWHANDLE"));
const UINT RMSG_REQUEST_HOGADATA = ::RegisterWindowMessage(_T("RMSG_REQUEST_HOGADATA"));
const UINT RMSG_ISSTRUNSTATE = ::RegisterWindowMessage(_T("RMSG_ISSTRUNSTATE"));

extern "C" __declspec(dllexport) LONG SCW_IGfxOutBarCtrl(int nType, long dwValue, long dwReserved1);

// CTabSystemTradingEx ��ȭ �����Դϴ�.

//IMPLEMENT_DYNAMIC(CTabSystemTradingEx, CTabDlg)
CTabSystemTradingEx::CTabSystemTradingEx(CWnd* pParent /*=NULL*/)
	: CTabDlg(CTabSystemTradingEx::IDD, pParent)
{
	m_bShowOption = FALSE;
	m_pDlgOption = NULL;
	m_nHeightOfSearch=80;
	m_pSTLoadMnger = NULL;

	m_pIAUDataMonitorManager = NULL;
	for(int i=0; i<2; i++)	m_dwIAUDataMonitorManager[i] = NULL;
	m_pWndOutBar = NULL;

	m_hSTConfigureWizardInst = NULL;

	m_xSTAnalysisInput.m_Info.nFolderIndx = -1;		// �ƹ� ���õ� ���� ���� (-1)
	m_xSTAnalysisInput.m_Info.nItemIndx = -1;	// �ƹ� ���õ� ���� ���� (-1)
	m_xSTAnalysisInput.m_InfoForAnalysis.nFolderIndx = -1;		// �ƹ� ���õ� ���� ���� (-1)
	m_xSTAnalysisInput.m_InfoForAnalysis.nItemIndx = -1;	// �ƹ� ���õ� ���� ���� (-1)
	m_bEmptyWizard = FALSE;
	m_bSTBtn_Click = FALSE;

	m_mapConfirmWnd.RemoveAll();
}

CTabSystemTradingEx::~CTabSystemTradingEx()
{
	for(int i=0; i<2; i++)
		if(m_dwIAUDataMonitorManager[i])	m_pIAUDataMonitorManager->UnAdvise(m_dwIAUDataMonitorManager[i]);

	if(m_pDlgOption && m_pDlgOption->GetSafeHwnd())
		m_pDlgOption->DestroyWindow();
	SafeDelete(m_pDlgOption);
	m_ilSTIcon.DeleteImageList();
	m_ilW151H21.DeleteImageList();
	m_ImageListIcon.DeleteImageList();
	m_ImageOutbarIcon.DeleteImageList();
}

void CTabSystemTradingEx::DoDataExchange(CDataExchange* pDX)
{
	CTabDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STWIZARDA, m_BtnStwizardA);
	DDX_Control(pDX, IDC_STOPTIMA, m_BtnStoptimaA);
	DDX_Control(pDX, IDC_STDELETE, m_BtnStDelete);
	DDX_Control(pDX, IDC_STANAL, m_BtnStanal);
	DDX_Control(pDX, IDC_STSHOWOPTION, m_BtnOption);
}


BEGIN_MESSAGE_MAP(CTabSystemTradingEx, CTabDlg)
	//{{AFX_MSG_MAP(CTabSystemTradingEx)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_STWIZARDA, OnBtnStwizardA)
	ON_BN_CLICKED(IDC_STOPTIMA, OnBtnStoptimaA)
	ON_BN_CLICKED(IDC_STDELETE, OnBtnStDelete)
	ON_BN_CLICKED(IDC_STANAL, OnBtnStanal)
	//}}AFX_MSG_MAP
//	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDC_STCHKDIV, OnBnClickedStchkdiv)
	ON_REGISTERED_MESSAGE(RMSG_OB_ITEMCLICK		,OnRmsgObItemClk)
	ON_REGISTERED_MESSAGE(RMSG_OB_ITEMDBLCLK	,OnRmsgObItemDBClk)
	ON_REGISTERED_MESSAGE(RMSG_RUN_MULTIST		,OnRmsgMultiST)
	ON_REGISTERED_MESSAGE(RMSG_INDEXADDIN_NOTIFY_RAWDATA,OnRmsgIndexAddinNotifyRawData)
	ON_REGISTERED_MESSAGE(RMSG_INDEXADDIN_NOTIFY_BUYSELL, OnRmsgIndexAddinNotifyBuySell)
	ON_REGISTERED_MESSAGE(RMSG_INDEXADDIN_NOTIFY_SIGNAL , OnRmsgIndexAddinNotifySignal)
	ON_REGISTERED_MESSAGE(RMSG_DESTROY_CONFIRMWND, OnRmsgDestroyConfirmWnd)
	ON_REGISTERED_MESSAGE(RMSG_ISSTRUNSTATE, OnRmsgIsSTRunState)
	ON_BN_CLICKED(IDC_STSHOWOPTION, OnStshowoption)
END_MESSAGE_MAP()


// CTabSystemTradingEx �޽��� ó�����Դϴ�.
void CTabSystemTradingEx::OnSize(UINT nType, int cx, int cy) 
{
	CTabDlg::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
//	CRect rc2;
//	CWnd* pWnd = GetDlgItem(IDC_STFRAME2);
//	if(pWnd && pWnd->GetSafeHwnd())
//	{
//		pWnd->GetWindowRect(rc2);
//		rc2.bottom = cy -GAP_TABDLG;
//	}

	RecalcLayout();
}

void CTabSystemTradingEx::RecalcLayout()
{
	if(!m_pWndOutBar) return;

	//RecalcLayoutCtrl(m_wndOutBar);
	CWnd* pWndOutBar = m_pWndOutBar->GetCoutBarCtrl();
	if(!pWndOutBar)return;

	RecalcLayoutCtrl(*pWndOutBar);
	RecalcLayoutCtrl(m_BtnStwizardA);
	RecalcLayoutCtrl(m_BtnStoptimaA);
	RecalcLayoutCtrl(m_BtnStDelete);
	RecalcLayoutCtrl(m_BtnStanal);
	RecalcLayoutCtrl(m_BtnOption);
}

CRect CTabSystemTradingEx::GetRectOfCtrl(INT nID,BOOL bInit/*=FALSE*/)
{
	CRect rect, rc1;
	GetClientRect(rect);
	int nWidth = rect.Width();
	rect.DeflateRect(2,4,0,0);

	switch(nID)
	{	case IDC_OUTLOOKBAR:
			{
				CRect rc2;
				GetDlgItem(IDC_STFRAME2)->GetWindowRect(rc2);
				ScreenToClient(rc2);
				rc2.bottom = rect.bottom-Y_GAP;
				GetDlgItem(IDC_STFRAME2)->MoveWindow(rc2);
			}
			rect.top = rect.top + (HEIGHTOFBUTTON*2) +(Y_GAP)*3;
			//rect.bottom = rect.bottom - GAP_TABDLG;


			GetDlgItem(IDC_STFRAME1)->GetWindowRect(rc1);
//			rect.right-=2; // KHD : ��Ų ���� 
			if(m_bShowOption)
			{
				rect.top = rect.top + rc1.Height();
			}
//			rect.left += 1;	// KHD : ��Ų���� �ƿ���� ��ġ����

			if(m_pDlgOption)
			{
				int cx=rect.Width(), cy=rc1.Height();
				m_pDlgOption->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOACTIVATE|SWP_NOMOVE);
			}
			break;

//		case IDC_STWIZARDA:
//			rect.bottom = rect.top + HEIGHTOFBUTTON;
//			rect.right = rect.left + 58 +15;
//			break;
//		case IDC_STOPTIMA:
//			rect.bottom = rect.top + HEIGHTOFBUTTON;
//			rect.left = rect.left + 58 + GAP_TABDLG +15;
//			rect.right = rect.left + 58 +15;
//			break;
//		case IDC_STANAL:
//			rect.top = rect.top + HEIGHTOFBUTTON * 1 + Y_GAP * 1;
//			rect.bottom = rect.top + HEIGHTOFBUTTON;
//			rect.left = rect.left + 58 + GAP_TABDLG -15;
//			rect.right = rect.left + 58 +15;
//			break;
		case IDC_STWIZARDA:
			rect.bottom = rect.top + HEIGHTOFBUTTON;
			rect.right = rect.left + 58;
			break;
		case IDC_STOPTIMA:
			rect.bottom = rect.top + HEIGHTOFBUTTON;
			rect.left = rect.left + 58 + 2;
			rect.right = rect.left + 58;
			break;
		case IDC_STDELETE:
			rect.bottom = rect.top + HEIGHTOFBUTTON;
			rect.left = rect.left + 58 + 2 + 58 + 2;
			rect.right = rect.left + 43;
			break;
		case IDC_STANAL:
			rect.top = rect.top + HEIGHTOFBUTTON * 1 + Y_GAP * 1;
			rect.bottom = rect.top + HEIGHTOFBUTTON;
			rect.left = rect.left + 58 + 2 + 23;
			rect.right = rect.left + 58;
			break;
		case IDC_STSHOWOPTION:
			rect.top = rect.top + HEIGHTOFBUTTON * 1 + Y_GAP * 1;
			rect.bottom = rect.top + HEIGHTOFBUTTON;
			rect.left = rect.left + 58 + 2 + 23 + 58 + 2;
			rect.right = rect.left + 20;
			break;

		default:
			break;
	}
	return rect;
}

void CTabSystemTradingEx::OnDestroy()
{
	UnLoadSTList();

	if(m_hSTConfigureWizardInst == NULL) return;
	typedef	LONG (FnSCW_IGfxOutBarCtrl)(int nType, long dwValue, long dwReserved1);

	FnSCW_IGfxOutBarCtrl* pFunc = (FnSCW_IGfxOutBarCtrl*)GetProcAddress(m_hSTConfigureWizardInst ,_T("SCW_IGfxOutBarCtrl"));
	if(pFunc == NULL) return ;

	if(m_pWndOutBar) {
		m_pWndOutBar->GetCoutBarCtrl()->DestroyWindow();
		pFunc(1, (long)m_pWndOutBar, 0);
		m_pWndOutBar = NULL;
	}

	FreeLibrary(m_hSTConfigureWizardInst);
	m_hSTConfigureWizardInst = NULL;

	CConfirmWnd* pConfirmWnd = NULL;
	long rValue = 0;
	POSITION pos = m_mapConfirmWnd.GetStartPosition();
	while(pos)
	{
		m_mapConfirmWnd.GetNextAssoc(pos, pConfirmWnd, rValue);
		if(pConfirmWnd && pConfirmWnd->GetSafeHwnd())
			pConfirmWnd->DestroyWindow();

		delete pConfirmWnd;
		pConfirmWnd = NULL;
	}
	m_mapConfirmWnd.RemoveAll();
}

BOOL CTabSystemTradingEx::OnInitDialog() 
{
	CTabDlg::OnInitDialog();	

	IMainInfoManager0002* pMng = (IMainInfoManager0002*)AfxGetPctrInterface(UUID_IMainInfoManager);
	if (pMng)
	{
		m_szRootDir = pMng->GetRootDir();
		m_szDataDir = pMng->GetDataDir();
		m_szEncUserDir = ((CStdDialog*)m_pMultiChartWnd)->m_strUserDir;
		m_nUserType = pMng->GetHtsMode();	//"0":HTS  "1":FO

		CString szDLLPath = m_szRootDir + "\\dev";
		::SetEnvironmentVariable("SYSTEMDLLPATH", szDLLPath);
	}

	MakeSTDefaultDirectory();
	//m_ilSTIcon.Create(IDB_IL_STICON, 16, 0, RGB(0,128,128));

	GetDlgItem(IDC_STFRAME1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STFRAME2)->ShowWindow(SW_HIDE);

	InitControl();
	LoadOutlookBar();
	LoadInfo();	
	HideStOptimaA();
	RecalcLayout();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



// ������ ������ ����Ʈ�� ��������(TRUE), ����Ʈ���� ����·� ��������(FALSE)�� ���� �����ۼ� ����
void CTabSystemTradingEx::RunWizard(BOOL bSelectedDefault)
{
	if(CanSTModule()==FALSE) return;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//HINSTANCE hOld = AfxGetResourceHandle();
	//AfxSetResourceHandle(g_hInstance);

	CString szDllName;
	szDllName.Format("%s%s", m_szRootDir, "\\dev\\HWStrategyConfigureWizard");	// DLL�� �ִ´�.
	CTabSystemTradingEx* pParentDlg = (CTabSystemTradingEx*)GetParent();

	m_bEmptyWizard = bSelectedDefault;
	CDllDlg dlg(szDllName, "�����ۼ�(Wizard)", &m_xSTRunManager, NULL);
	dlg.m_strImageDir = m_pParentDlg->m_szImageDir;
	// m_xSTAnalysisInput.m_Info;
	dlg.DoModal();
	m_bEmptyWizard = FALSE;

	if(dlg.m_hTradeInst) FreeLibrary(dlg.m_hTradeInst);
}

void CTabSystemTradingEx::OnBtnStwizardA()
{	
	RunWizard(TRUE);
}

//KHD : ����ȭ UI ���� 
void CTabSystemTradingEx::OnBtnStoptimaA()
{
	CMenu menu;
	menu.CreatePopupMenu();

	long lPosAll = 0;
	long lSize = 0;
	//KHD : Popup Menu �߰� , ���� ID �ο� 
	menu.AppendMenu(MF_STRING,(UINT)10001,"��������ȭ");
	menu.AppendMenu(MF_STRING,(UINT)10002,"��������ȭ");
	menu.AppendMenu(MF_STRING,(UINT)10003,"�ð�����ȭ");

//#ifdef _DEBUG
//	menu.AppendMenu(MF_STRING,(UINT)10004,"����(#AROONSTRATEGY)����");
//	menu.AppendMenu(MF_STRING,(UINT)10005,"����(#�̰ݵ�)����");
//	menu.AppendMenu(MF_STRING,(UINT)10006,"����Clear");
//	menu.AppendMenu(MF_STRING,(UINT)10007,"�ֹ������׽�Ʈ...");
//#endif

	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	CRect rect;
	GetDlgItem(IDC_STOPTIMA)->GetWindowRect(rect);
	int nRetValue = menu.TrackPopupMenu(nFlagsForMenu, rect.left, rect.bottom, this);
	if(!nRetValue) return;
	menu.DestroyMenu();

	int nOptimizer = 0; // �ð�, ����, ��ǥ������ Flag ����
	CString stTitle;
	//1001:��ǥ����ȭ,1002:��������ȭ,1003:�ð�����ȭ
	switch(nRetValue) {
		case 10001:
			stTitle = "��������ȭ";
			nOptimizer = 1001;
			break;
		case 10002:
			stTitle = "��������ȭ";
			nOptimizer = 1002;
			break;
		case 10003:
			stTitle = "�ð�����ȭ";
			nOptimizer = 1003;
			break;
	/*	case 10004:
			stTitle = "#AROONSTRATEGY";
			Run_STDLL((LPCSTR)stTitle);
			return;
		case 10005:
			stTitle = "#�̰ݵ�";
			Run_STDLL((LPCSTR)stTitle);
			return;
		case 10006:
			STClear();
			return;
		case 10007:
			TestJumun();*/
			return;
	}

	if(m_xSTAnalysisInput.GetCount()==0)
	{
		AfxMessageBox("������ȸ �� �����մϴ�.");
		return;
	}

	if(!CheckOcx())
	{
		AfxMessageBox("����ȭ���� �ʿ��� OCX(TeeChart7.ocx, MSChrt20.ocx)����� �ȵǾ��ֽ��ϴ�.");
		return;
	}

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//HINSTANCE hOld = AfxGetResourceHandle();
	//AfxSetResourceHandle(g_hInstance);

	CString szDllName;
	szDllName.Format("%s%s", m_szRootDir, "\\dev\\IndexOptimizer");	// DLL�� �ִ´�.
	CDllDlg dlg(szDllName, stTitle, &m_xSTInterface, NULL);
	dlg.m_strImageDir = m_pParentDlg->m_szImageDir;
	dlg.m_nOptimizerDiv = nOptimizer;
	dlg.DoModal();
	//if(dlg.m_hTradeInst) FreeLibrary(dlg.m_hTradeInst);//DllDlg���� ��ü������ ������..OnDestroy�ÿ�..

	// AfxSetResourceHandle(hOld);

}


void CTabSystemTradingEx::ShowHideStOptimaA(BOOL bChange, BOOL bShopOption, int nOffset)
{
	if(bChange)	m_bShowOption = bShopOption;
	else m_bShowOption = !m_bShowOption;

	CRect rc1, rc2;
	GetDlgItem(IDC_STFRAME1)->GetWindowRect(rc1);
	GetDlgItem(IDC_STFRAME2)->GetWindowRect(rc2);
	rc2.left+=nOffset; rc2.right+=nOffset;

	rc1.right = rc2.right;
	if(m_bShowOption)
	{
		rc2.top = rc1.bottom;
		if(m_pDlgOption) m_pDlgOption->ShowWindow(SW_SHOW);
	}
	else 
	{
		rc2.top = rc1.top;
		if(m_pDlgOption)
		{
			m_pDlgOption->ShowWindow(SW_HIDE);
		}
	}
	ScreenToClient(rc1);
	ScreenToClient(rc2);
	GetDlgItem(IDC_STFRAME2)->MoveWindow(rc2);
	CWnd* pWndOutBar = m_pWndOutBar ? m_pWndOutBar->GetCoutBarCtrl() : NULL;
	if(pWndOutBar && pWndOutBar->GetSafeHwnd())
		pWndOutBar->MoveWindow(rc2);
	if(m_pDlgOption==NULL)
	{
		m_pDlgOption = new CDlgSTExOption;
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		//HINSTANCE hOld = AfxGetResourceHandle();
		//AfxSetResourceHandle(g_hInstance);
		if(m_pDlgOption->Create(CDlgSTExOption::IDD, this)==FALSE)
		{
			SafeDelete(m_pDlgOption);
		}
		else
		{
			m_pDlgOption->MoveWindow(rc1);
		}
		//KHD : ���� : ������ ExDlg�� Ŭ���Ǵ� ���� ���� : 2006.11.16
		m_pDlgOption->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STFRAME1)->ShowWindow(SW_HIDE);
		//AfxSetResourceHandle(hOld);
		
	}

	RecalcLayout();
}

// ���� üũ�� ���� ��쿡�� �ϴ��� ������ �����ְ�,
//             �����ʾ��� ��쿡�� ������ ���� ���� ���·� ������ ������ó�� ó���Ѵ�.
void CTabSystemTradingEx::OnBtnStanal()
{
	CChartItem* pCItem = ((CChartMng*)m_pChartMng)->GetpChartItemSelected();
	if(pCItem) {
		pCItem->InvokeAddInSTIndex("ChartSTIndexAddIn.dll", 3, (long)&pCItem);
	}
	UINT nChk = IsDlgButtonChecked(IDC_STCHKDIV);
	if(nChk==0)
	{
		BOOL bSel = IsSTSelected();
		if(!bSel)
		{
			AfxMessageBox("������ ������ �������ֽñ� �ٶ��ϴ�");
			return;
		}

		if(m_pDlgOption) m_pDlgOption->OpenRefort();//PostMessage(WM_COMMAND, IDC_STREPORT, 0);
		//if(m_pDlgOption) m_pDlgOption->OnBnClickedStapply();
	}
	else
	{
		ShowHideStOptimaA(TRUE, TRUE);		// bChange=TRUE, bShowOption=TRUE
		// m_BtnStanal.ShowWindow(SW_HIDE);
		BOOL bSel = IsSTSelected();
		if(!bSel)
		{
			AfxMessageBox("������ ������ �������ֽñ� �ٶ��ϴ�");
			return;
		}

		if(m_pDlgOption) m_pDlgOption->OpenRefort();//PostMessage(WM_COMMAND, IDC_STREPORT, 0);
	}
}

void CTabSystemTradingEx::HideStOptimaA()
{
	ShowHideStOptimaA(TRUE, FALSE);		// bChange=TRUE, bShowOption=FALSE
}


void CTabSystemTradingEx::LoadInfo()
{
	int nID = UUID_IAUDataMonitorManager; 
	//@SolomonGNF m_pIAUDataMonitorManager = (IAUDataMonitorManager*)AfxGetPctrInterface(nID, 3);
	m_pIAUDataMonitorManager = (IAUDataMonitorManager*)AfxGetPctrInterface(nID);
	CString szKey = "STLIST_MODIFIED"; 
	m_dwIAUDataMonitorManager[0] = m_pIAUDataMonitorManager->Advise(szKey, (long)this, (long)NULL, DataMonitorCallbackProc);

	szKey = "MultiChart_ReceiveData"; 
	m_dwIAUDataMonitorManager[1] = m_pIAUDataMonitorManager->Advise(szKey, (long)this, (long)NULL, DataMonitorCallbackProc);
	
	//�����Ϸ��� �̹� ���� �ϰ� �ִٸ�...... ��.. �ٽ� ��ƼíƮ�� ����ٸ� �ڵ鰪�� �����Ѵ�.
	CWnd* pWndCompiler = FindWindow(NULL, _T("SystemTrading"));
	if(pWndCompiler)
	{
		HWND h_pctr = NULL;
		//@SolomonGNF h_pctr = AfxGetPctrMainHandle(3);
		h_pctr = AfxGetPctrMainHandle();
		CString szPctr;
		szPctr.Format("%08X",h_pctr);
		COPYDATASTRUCT copyData;
		copyData.dwData = 2000;
		copyData.cbData = ::lstrlen(szPctr) + sizeof(DWORD);
		copyData.lpData = (void*)(LPCSTR)szPctr;

		pWndCompiler->SendMessage(WM_COPYDATA, 0,(LPARAM)&copyData);
	}
	
}


// usr/src/AupMain/AupFrm.cpp���� ��ƾ������
//#include "../../../src/Shared/inc/AupAgentEventDef.h"
#define WMU_GET_AUP_SIZE	WM_USER + 2243			// Aup�� �⺻ Size�� View�� ��û�Ѵ�.
#define WMU_SET_TRADING_INSTANCE WM_USER + 2248		// Tradingȭ�鿡 Dll�� �����Ѵ�.
//#include "../../../src/Shared/inc/ISTitleBar.h"
#define		SIZE_TITLE_BAR			23			    ///> Ÿ��Ʋ�� ����.
typedef	CWnd * (CreateTradingAupEx)(UINT nDldresId, CWnd* pParent, void* pInterface);

BOOL CTabSystemTradingEx::OpenAUP()
{
	CString szDllName = "HWStrategyAnalysis";
	CWnd *pTrade = LoadTradeAup(szDllName, this);
	if(pTrade == NULL)
	{
		// Error ó��
		return FALSE;
	}
	else
	{
		//((CAUPExMainView *)m_pInfoAgent->m_pActiveView)->SetTradingWnd(pTrade);
		CPoint size(800,600);
		pTrade->SendMessage(WMU_GET_AUP_SIZE,(WPARAM)&size,0);

		CRect frameRect;
		CRect viewRect;
		GetWindowRect(&frameRect);
		GetClientRect(&viewRect);

		int nTheme = nTheme = GetSystemMetrics(SM_CYCAPTION);

		frameRect.right = frameRect.right - 
			GetSystemMetrics(SM_CXEDGE) - 
			GetSystemMetrics(SM_CXDLGFRAME) -
			GetSystemMetrics(SM_CYBORDER);

		frameRect.bottom = frameRect.bottom - GetSystemMetrics(SM_CYEDGE) -
			GetSystemMetrics(SM_CYDLGFRAME) - nTheme -		
			GetSystemMetrics(SM_CYBORDER) + SIZE_TITLE_BAR;

		int iNewWidth = frameRect.Width() - viewRect.Width() + size.x;
		int iNewHeight = frameRect.Height() - viewRect.Height() + size.y;

		SetWindowPos(NULL,0,0,iNewWidth,iNewHeight,SWP_NOMOVE);	
	}
	return TRUE;
}


// usr/src/AupMain/MainFrm.cpp���� ��ƾ������
// #include "../../../src/TradingAupDll/TradingAupDef.h



CWnd *CTabSystemTradingEx::LoadTradeAup(CString strFile, CWnd *pParent)
{
	CString strTradingName = strFile + ".dll";

	HINSTANCE hTradeInst = LoadLibrary(strTradingName);
	if(hTradeInst == NULL)
	{
		CString strError;
		strError.Format(_T("%s File Load Faile"),strTradingName);
		MessageBox(strError);
		return NULL;
	}

	CreateTradingAupEx *pCreateTrading = (CreateTradingAupEx *)GetProcAddress(hTradeInst,_T("CREATE_TRADING_EX"));
	if(pCreateTrading == NULL)
	{
		MessageBox("Ưȭȭ�� �ε� ����");
		FreeLibrary(hTradeInst);
		return NULL;
	}

	CWnd *pTrade = pCreateTrading(0,pParent, &m_xSTInterface);
	

	//////////////////////////////////////////////////////////////////////////
		
	if(pTrade != NULL)
	{
		pParent->SendMessage(WMU_SET_TRADING_INSTANCE,(WPARAM)hTradeInst,0);
	}
	else
	{
		FreeLibrary(hTradeInst);
	}
	return pTrade;						
}




// ------------------------------------------------------------------------------------------
//				CSTAnalysisInput
// ------------------------------------------------------------------------------------------


// ID�� ������
void* CTabSystemTradingEx::CSTInterface::GetInterface(int nID)
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STInterface)

	void* pInterface = NULL;
	if(nID==UUSTID_ISTAnalysisInput)
		return &pThis->m_xSTAnalysisInput;

	return pInterface;
}

LONG CTabSystemTradingEx::CSTInterface::GetOutBarColor(COLORREF& clrOLBkgrnd, COLORREF& clrOLOutLine)
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STInterface)

	clrOLBkgrnd = ((CStdDialog*)pThis->m_pMultiChartWnd)->m_clrOLBkgrnd;
	clrOLOutLine = ((CStdDialog*)pThis->m_pMultiChartWnd)->m_clrOLOutLine;
	return (LONG)1L;
}


// ------------------------------------------------------------------------------------------
//				CSTAnalysisInput
// ------------------------------------------------------------------------------------------

CTabSystemTradingEx::CSTAnalysisInput::CSTAnalysisInput()
{
	m_bUseUserData = FALSE;
	m_pInterfaceUserData = NULL;
}

BOOL CTabSystemTradingEx::CSTAnalysisInput::IsUseUserData()
{
	return m_bUseUserData;
}

void CTabSystemTradingEx::CSTAnalysisInput::SetUserData(BOOL bUse, ISTUserData* pInterfaceUserData)
{
	m_pInterfaceUserData = NULL;
	m_bUseUserData = bUse;
	if(bUse) 	m_pInterfaceUserData = pInterfaceUserData;
}

ISTUserData* CTabSystemTradingEx::CSTAnalysisInput::GetUserDataInterface()
{
	return m_pInterfaceUserData;
}


// ID�� ������
// ���� ��������. �������� ��������... 06.07.21
COleVariant CTabSystemTradingEx::CSTAnalysisInput::GetData(int nID)
{
	COleVariant a;
	return a;
}

// 
// 0: �����ڵ� , GetCode(0)
CString CTabSystemTradingEx::CSTAnalysisInput::GetCode()
{
//	return "000660";

	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STAnalysisInput)

//	CStdDialog* pMainDlg = (CStdDialog*)pThis->m_pStdDialog;
	//return pMainDlg->m_strInputCode;

	CString szCode = ((CChartMng*)pThis->m_pChartMng)->GetpChartItemSelected()->GetDataCode();
	return szCode;
}

// CInputBarDlg	*m_pInputBarDlg;
// �����ڵ��, GetCode(1)
#include "../../inc/IMasterDataMng.h"	// ���������������� ������ؼ� ȣ��, Code�� CodeName��ȸ.
										// �� �κ��� Chart���� �̹� �����Ͱ� �����Ƿ� �������� ��ƾ���� ���濹��.
#include "../../inc/IMainExInfoMng.h"				// for IMainExInfoManager2

CString CTabSystemTradingEx::CSTAnalysisInput::GetCodeName()
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STAnalysisInput)

	CString szCode = GetCode();
	//@SolomonGNF IMasterDataManager080331* pMasterMng = (IMasterDataManager080331*)AfxGetPctrInterface(UUID_IMasterDataManager, 3);
	IMasterDataManagerLast* pMasterMng = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	CString szCodeName;

//	if(szCode.GetLength() && pMasterMng) szCodeName = pMasterMng->GetCodeInfo("JMASTER", 0, szCode);
	CStdDialog* pStdDialog = (CStdDialog*)pThis->m_pMultiChartWnd;
	//if(szCode.GetLength())
	//	szCodeName = pStdDialog->GetItemNameFromItemCode(szCode);
	szCodeName = szCode;

	return szCodeName;
}

#define		USE_INPUTBARDLG()  CStdDialog* pMainDlg = (CStdDialog*)pThis->m_pMultiChartWnd;	\
		CInputBarDlg* pInputBarDlg = (CInputBarDlg*)((CChartMng*)pThis->m_pChartMng)->GetpInputBarDlg();

// �����ڵ� ����
// InputBarDlg.h�� ���ǵ� �� ���� : enum	{STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, FOREIGN_CHART};
int CTabSystemTradingEx::CSTAnalysisInput::GetCodeType()
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STAnalysisInput)

	//CStdDialog* pMainDlg = (CStdDialog*)pThis->m_pMultiChartWnd;
	//CInputBarDlg* pInputBarDlg = (CInputBarDlg*)((CChartMng*)pThis->m_pChartMng)->GetpInputBarDlg();
	USE_INPUTBARDLG();

	int nCodeTypeID[] = {0,2,1};
	int nType = nCodeTypeID[pInputBarDlg->m_nMarketType];

	return nType;
}

int CTabSystemTradingEx::CSTAnalysisInput::GetUnitPos()
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STAnalysisInput)

	USE_INPUTBARDLG();
	int nType = pInputBarDlg->GetUnitPos();
	return nType;
}

// ������, GetCode(3)
BOOL CTabSystemTradingEx::CSTAnalysisInput::GetSTInfo(STMNG_ITEMINFO* pArg)
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STAnalysisInput)

	//// ���õ� ������� (���� ������ & STType ����)
	//CString szFolderPath;
	//szFolderPath = m_xSTAnalysisInput.m_Info.szSTPath;
	//szFolderPath += "\\";
	//szFolderPath += m_xSTAnalysisInput.m_Info.szSTName;

	//// ���õ� ���� ��ü��� (".ST"�� ����)
	//STLISTITEMDATA	dataItem;
	//dataItem.szSTPath = szFolderPath;
	//dataItem.bUserFolder = m_xSTAnalysisInput.m_Info.nSTType;

	pArg->nSTType	= m_InfoForAnalysis.nSTType	;
	pArg->szSTName	= m_InfoForAnalysis.szSTName;
	pArg->szSTPath	= m_InfoForAnalysis.szSTPath;
	pArg->szSTRData = m_InfoForAnalysis.szSTRData;

	// 06.07.24�� ����� �׽�Ʈ�� MACD_ ������ �Ѱ��ش�.
	return TRUE;
}


// ����, GetCode(4)
int CTabSystemTradingEx::CSTAnalysisInput::GetCount()
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STAnalysisInput)

	CStdDialog* pMainDlg = (CStdDialog*)pThis->m_pMultiChartWnd;
	//int nTmp = ((CChartMng*)pThis->m_pChartMng)->GetpChartItemSelected()->m_nSaveAllCount;
	int nTmp = ((CChartMng*)pThis->m_pChartMng)->GetpInputBarDlg()->GetDlgItemInt(IDC_EDITCNTOFDATALL);

	return nTmp;

}


// ������, GetCode(5)
CString CTabSystemTradingEx::CSTAnalysisInput::GetEndDay()
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STAnalysisInput)

	CStdDialog* pMainDlg = (CStdDialog*)pThis->m_pMultiChartWnd;
	CString szTmp = ((CChartMng*)pThis->m_pChartMng)->GetpChartItemSelected()->GetEndDate();
	return szTmp;
}


BOOL CTabSystemTradingEx::CSTAnalysisInput::IsDivide()
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STAnalysisInput)
	return pThis->IsDlgButtonChecked(IDC_STCHKDIV);
}

// ���Ҽ�, GetCode(6)
int CTabSystemTradingEx::CSTAnalysisInput::GetDivideCnt()
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STAnalysisInput)
	BOOL bIsDive = IsDivide();
	if(!bIsDive)
		return 1; 
		
	if(pThis->m_pDlgOption) return pThis->m_pDlgOption->GetDivCount();

	return 0;
}


// ���� ����, GetCode(7)
// ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5)
int CTabSystemTradingEx::CSTAnalysisInput::GetDivideState()
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STAnalysisInput)

	USE_INPUTBARDLG();
	int nType = pInputBarDlg->GetType() - '0';

	return nType;
}


// ��ø��, GetCode(8)
double CTabSystemTradingEx::CSTAnalysisInput::GetDeivideRate()
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STAnalysisInput)
	if(pThis->m_pDlgOption) return pThis->m_pDlgOption->GetRepRate();

	return 0;
}

// ��Ʈ���� ���´�.
// nID 0(ChartOcx), 1(CListCtrl)
LONG CTabSystemTradingEx::CSTAnalysisInput::GetControl(int nID)
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STAnalysisInput)

	if(nID==0)
	{
		CChartItem* pCItem = ((CChartMng*)pThis->m_pChartMng)->GetpChartItemSelected();
		pThis->m_STExChartItem.m_pChartItem = pCItem;
		//return (LONG)pCItem;
		return (long)&pThis->m_STExChartItem;
	}
	else if(nID==1)
	{
		if(pThis->m_pDlgOption==0) return NULL;

		return (LONG)(&pThis->m_pDlgOption->m_CtlList);
	}
	else if(nID==2)//CTabSystemTradingEx�ڵ鸦 ��ȯ�Ѵ�.: �����м��ε� :KHD
	{
		return (LONG)pThis->GetSafeHwnd();
	}


	return NULL;
}


// �����Ͱ��� ����.
#include "../../inc/ISTAddinManager.h" // �ý���Ʈ���̵� ���õ� ������ �������̽�
#include ".\tabsystemtradingex.h"
// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
int CTabSystemTradingEx::CSTAnalysisInput::GetChartData(LONG& dwDataTime, 
													LONG& dwDataOpen, LONG& dwDataHigh, LONG& dwDataLow, LONG& dwDataClose,
													LONG& dwDataVolume, LONG& dwDataExt)
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STAnalysisInput)

	CChartItem* pCItem = ((CChartMng*)pThis->m_pChartMng)->GetpChartItemSelected();
	//@SolomonGNF IMainExInfoManager2* pMng = (IMainExInfoManager2*)AfxGetPctrInterface(UUID_IMainExInfoManager2, 3);
	IMainExInfoManager2* pMng = (IMainExInfoManager2*)AfxGetPctrInterface(UUID_IMainExInfoManager2);
	if(!pMng) return 0;

	LONG rValue = pMng->GetUserExData((LONG)pCItem);
	TRACE("key=%ld, rValue=%ld\n", pCItem, rValue);

	ISTAddinData* pAddinDataMng = (ISTAddinData*)rValue;
	if(pAddinDataMng)
	{
		// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
		//@SolomonGNF return pAddinDataMng->GetChartData(dwDataTime, dwDataOpen, dwDataHigh, dwDataLow, dwDataClose, dwDataVolume/*, dwDataExt*/);
		return pAddinDataMng->GetChartData(dwDataTime, dwDataOpen, dwDataHigh, dwDataLow, dwDataClose, dwDataVolume, dwDataExt);
	}
	return 0;	// �����͸� ���� ���� ���.
}

// 20081016 �ڽ·� - 8���� ��ȯó��
CString CTabSystemTradingEx::CSTAnalysisInput::GetPacketType()
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STAnalysisInput)
	CChartItem* pCItem = ((CChartMng*)pThis->m_pChartMng)->GetpChartItemSelected();
	if(pCItem)
	{
		return pCItem->GetCodePrecisionFormat(GetCode());
	}
	
	// Default 10����
	CString strPacketType;
	strPacketType.Format("�� %0.*f", 0, 1.);
	return strPacketType;
}

BOOL CTabSystemTradingEx::CSTAnalysisInput::Is30SecUnit()
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STAnalysisInput)
	USE_INPUTBARDLG();

	if(pInputBarDlg && pInputBarDlg->GetTypeNumber() == 999)
		return TRUE;

	return FALSE;
}

// ������ �����Ѵ�.
#include "../../inc/AZUtil_Ctrl.h"
int CTabSystemTradingEx::CSTRunManager::RunST(BOOL bImsiRun, CString &szRunFile, LONG _pInData)
{
	STMNG_ITEMINFO* pInData = (STMNG_ITEMINFO*)_pInData;
	int nRet = 0;
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STRunManager)

		if(pThis->m_pDlgOption) 
		{
			CString szSTName;
			{
//				// ���� ���õ� ���� ���� ����
//				pThis->m_xSTAnalysisInput.m_Info.nSTType		= pInData->nSTType;
//				pThis->m_xSTAnalysisInput.m_Info.szSTName		= pInData->szSTName;
//				pThis->m_xSTAnalysisInput.m_Info.szSTPath		= pInData->szSTPath;
//				pThis->m_xSTAnalysisInput.m_Info.szSTRData		= pInData->szSTRData;	
//				pThis->m_xSTAnalysisInput.m_Info.nFolderIndx	= pInData->nFolderIndx;
//				pThis->m_xSTAnalysisInput.m_Info.nItemIndx		= pInData->nItemIndx;

				{
					CString szMsg;
					if(pThis->IsAllowJumunBefore(szRunFile, szMsg)==FALSE)
						AfxMessageBox(szMsg);
				}

				// �����ۼ�ȭ�鿡�� ���� ���õ� ������ �ý������� �ƿ���ٿ� �����Ѵ�.
				if(pThis->m_pSTLoadMnger)
					pThis->m_pSTLoadMnger->SetSelectedItem(pInData);
			}

			//�������� CDlgSTExOption���� ó���ϴ� ��ƾ�� ���������� �����ϴ� ��ƾ���� �����ϸ鼭
			// RunMultiST();���� ������.
			// 1�� ����, 2�� ������.
			//1.
			// pThis->m_pDlgOption->PostMessage(WM_COMMAND, IDC_STAPPLY, 0);
			// 2.
			pThis->m_szImsiRunSTFile = "";
			if(bImsiRun) pThis->m_szImsiRunSTFile = szRunFile;

			pThis->PostMessage(RMSG_RUN_MULTIST, 0, 0 ); // RunMultiST();
		}

		return nRet;
}

LONG CTabSystemTradingEx::CSTRunManager::GetSelectedSTInfo()
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STRunManager)

	if(pThis->m_bEmptyWizard) return NULL;

	return (LONG)&pThis->m_xSTAnalysisInput.m_Info;
}

LONG CTabSystemTradingEx::CSTRunManager::GetViewHandle()
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STRunManager)

	//@Solomon
	//return (LONG)((CStdDialog*)pThis->m_pMultiChartWnd)->m_hViewInMain;
	return (LONG)((CStdDialog*)pThis->m_pMultiChartWnd)->GetSafeHwnd();
}

LONG CTabSystemTradingEx::CSTRunManager::GetOutBarColor(COLORREF& clrOLBkgrnd, COLORREF& clrOLOutLine)
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STRunManager)

	clrOLBkgrnd = ((CStdDialog*)pThis->m_pMultiChartWnd)->m_clrOLBkgrnd;
	clrOLOutLine = ((CStdDialog*)pThis->m_pMultiChartWnd)->m_clrOLOutLine;
	return (LONG)1L;
}

void CTabSystemTradingEx::InitControl()
{
//	m_BtnStwizardA.FP_vSetImage(m_pIL_W72H20, 0, 2, TRUE, "�����ۼ�", FALSE);
//	m_BtnStwizardA.FP_vSetToolTipText("�����ۼ�");
//
//	m_BtnStoptimaA.FP_vSetImage(m_pIL_W72H20, 0, 2, TRUE, "����ȭ", FALSE);
//	m_BtnStoptimaA.FP_vSetToolTipText("����ȭ");
//
//	m_BtnStanal.FP_vSetImage(m_pIL_W72H20, 0, 2, TRUE, "�����м�", FALSE);
//	m_BtnStanal.FP_vSetToolTipText("����ȸ");
	m_BtnStwizardA.FP_vSetImage(m_pIL_W58H20, 0, 2, TRUE, "�����ۼ�", FALSE);
	m_BtnStwizardA.FP_vSetToolTipText("�����ۼ�");

	m_BtnStoptimaA.FP_vSetImage(m_pIL_W58H20, 0, 2, TRUE, "����ȭ", FALSE);
	m_BtnStoptimaA.FP_vSetToolTipText("����ȭ");

	m_BtnStDelete.FP_vSetImage(m_pIL_W43H20, 0, 2, TRUE, "����", FALSE);
	m_BtnStDelete.FP_vSetToolTipText("����");

	m_BtnStanal.FP_vSetImage(m_pIL_W58H20, 0, 2, TRUE, "�����м�", FALSE);
	m_BtnStanal.FP_vSetToolTipText("����ȸ");
	//KHD 
	m_BtnOption.FP_vSetImage(m_pIL_W20H20, 48, 50,TRUE,"",FALSE);
}

void CTabSystemTradingEx::LoadOutlookBar()
{
	CRect rc2;
	GetDlgItem(IDC_STFRAME2)->GetWindowRect(rc2);
	ScreenToClient(rc2);
//	rc2.top += 3;

	//DWORD dwf = fDragItems | fEditGroups | fEditItems | fRemoveGroups | fRemoveItems | fAddGroups|fAnimation;
	DWORD dwf = fDragItems | fEditGroups | fEditItems | fRemoveGroups | fRemoveItems | fAddGroups | fAnimation | fSelHighlight;

	//W151_H21===========================================================================
	BOOL bOK = FALSE;
//	CBitmap bitmap;
//	CString strBitmapFile;	
//	//CString szImageDir = (LPCSTR)AfxGetPctrData(14);	// 14 : Image Directory
//	CString szImageDir = m_pParentDlg->m_strRootPath + "\\Image\\";
//	CString szImageChartDir = szImageDir + "\\dev";
//	if ( !m_ilW151H21.m_hImageList)
//	{		
//		if ( !m_ilW151H21.Create( 151, 21, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
//		{
//			AfxMessageBox ( "Can not Find W151_H21.bmp file" );
//		}
//		strBitmapFile.Format("%s\\%s", szImageChartDir,  "W151_H21.bmp");		
//		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
//			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
//		m_ilW151H21.Add(&bitmap, RGB(255, 0, 0));
//		bitmap.DeleteObject();
//	}
//
//	//icon image===========================================================================
//	CString szILPath = szImageChartDir + "\\treeIcon.bmp";
//	bitmap.Attach((HBITMAP)LoadImage(NULL, (LPSTR)(LPCTSTR)szILPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
//	m_ilSTIcon.Create(9, 18, ILC_MASK | ILC_COLORDDB, 1, 1);
//	m_ilSTIcon.Add(&bitmap, RGB(255, 0, 255));
//	bitmap.DeleteObject();
	CBitmap bitmap;
	CString strImageDir;
	strImageDir.Format("%s\\folder.bmp", m_pParentDlg->m_szImageDir);
	bitmap.Attach((HBITMAP)LoadImage(NULL, (LPSTR)(LPCTSTR)strImageDir, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
	m_ImageListIcon.Create(18, 18, ILC_MASK | ILC_COLORDDB, 1, 1);
	m_ImageListIcon.Add(&bitmap, RGB(255, 0, 255));
	bitmap.DeleteObject();

	//20061213 ������ ���� 
	//LeftBar Ʈ������� �̹��� ����
//@�������� 
	strImageDir = "";
	strImageDir.Format("%s\\outbar_icon.bmp", m_pParentDlg->m_szImageDir);
	bitmap.Attach((HBITMAP)LoadImage(NULL, (LPSTR)(LPCTSTR)strImageDir, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
	m_ImageOutbarIcon.Create(20, 20, ILC_MASK | ILC_COLORDDB, 1, 1);
	m_ImageOutbarIcon.Add(&bitmap, RGB(255, 0, 255));
	bitmap.DeleteObject();

	typedef	LONG (FnSCW_IGfxOutBarCtrl)(int nType, long dwValue, long dwReserved1);
	if(!m_hSTConfigureWizardInst)
	{
		CString szDllName;
		szDllName.Format("%s%s", m_szRootDir, "\\dev\\HWStrategyConfigureWizard.dll");
		m_hSTConfigureWizardInst  = LoadLibrary(szDllName);
	}
	if(!m_hSTConfigureWizardInst) return;


	FnSCW_IGfxOutBarCtrl* pFunc = (FnSCW_IGfxOutBarCtrl*)GetProcAddress(m_hSTConfigureWizardInst,_T("SCW_IGfxOutBarCtrl"));
	if(pFunc == NULL) return ;

	m_pWndOutBar = (IGfxOutBarCtrl*)pFunc(0, 0, 0);

	m_pWndOutBar->CreateOutBar(WS_CHILD|WS_VISIBLE, rc2, this, IDC_OUTLOOKBAR, dwf);
	m_pWndOutBar->Setcr3dFace(RGB(191,211,233));
	m_pWndOutBar->Setcr3dUnFace((COLORREF)m_pMultiChartWnd->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0));
	//m_wndOutBar.cr3dUnFace = RGB(239,242,247);
	m_pWndOutBar->GetCoutBarCtrl()->SetOwner(this);
	//  m_listSTList.Create(WS_CHILD|WS_VSCROLL|LBS_NOINTEGRALHEIGHT|LBS_NOTIFY|LBS_SORT, CRect(0,0,0,0), &m_wndOutBar, IDC_STLIST);

	int  nfSmallIcon = 1 << 0;
//	m_pWndOutBar->SetFolderImageList(&m_ilW151H21, 0, 2, 3);
//	m_pWndOutBar->SetImageList(&m_ilSTIcon, nfSmallIcon);
	m_pWndOutBar->SetFolderImageList(&m_ImageOutbarIcon, 0, 1, 3);
	m_pWndOutBar->SetImageList(&m_ImageListIcon, nfSmallIcon);

	//m_wndOutBar.AddFolderBar("���������", &m_listSTList);
	//m_wndOutBar.SetSelFolder(0);
	m_arSTPath.RemoveAll();
	LoadSTList();
}

// �ӽ��۾�
//���� �����ε��ϴ� �κ��� ���Ŀ� �����ؾ� ��.

void CTabSystemTradingEx::LoadSTList()
{
//	CString szDataDir = (LPCSTR)AfxGetPctrData(12);	// 12 : Data Directory
//	CString szUserDir = (LPCSTR)AfxGetPctrData(17);	// 17 : User Directory

	CString szSTData_STPath = m_szDataDir + "\\" + PATH_STDATA_ST;
	CString szSTUser_STPath = m_szEncUserDir + "\\" + PATH_STDATA_ST;


	{
		typedef	LONG (FPSCW_GetInterfaceOfStrategy)(LONG dwType/*=0*/);
		if(!m_hSTConfigureWizardInst)
		{
			CString szDllName;
			szDllName.Format("%s%s", m_szRootDir, "\\dev\\HWStrategyConfigureWizard.dll");
			m_hSTConfigureWizardInst  = LoadLibrary(szDllName);
		}
		if(!m_hSTConfigureWizardInst) return;

		FPSCW_GetInterfaceOfStrategy* pFunc = (FPSCW_GetInterfaceOfStrategy*)GetProcAddress(m_hSTConfigureWizardInst,_T("SCW_GetInterfaceOfStrategy"));
		if(pFunc == NULL) return ;


		LONG dwType = 0;	//CSTLoadManager*
		m_pSTLoadMnger = (ISTLoadManager2*)pFunc(dwType);
		if(m_pSTLoadMnger)
		{
			m_arSTPath.Add(szSTData_STPath);
			m_arSTPath.Add(szSTUser_STPath);

//			m_pSTLoadMnger->Init(TRUE, "*.ST", &m_ilSTIcon);
			m_pSTLoadMnger->Init(TRUE, "*.ST", &m_ImageListIcon);
			m_pSTLoadMnger->SetUseBoldFont(FALSE);
			int p_LoadOption = 0;
			m_pSTLoadMnger->LoadST2(m_pWndOutBar->GetCoutBarCtrl(), &m_arSTPath, 0, 0, p_LoadOption);
			// m_wndOutBar.MoveWindow(GetRect(IDC_OUTLOOKBAR));
		}
	}
}

void CTabSystemTradingEx::UnLoadSTList()
{
	if(!m_pSTLoadMnger) return;

	typedef	void (FPSCW_DeleteInterfaceOfStrategy)(LONG dwKey, LONG dwType);
	if(m_hSTConfigureWizardInst == NULL) return;

	FPSCW_DeleteInterfaceOfStrategy* pFunc = (FPSCW_DeleteInterfaceOfStrategy*)GetProcAddress(m_hSTConfigureWizardInst,_T("SCW_DeleteInterfaceOfStrategy"));
	if(pFunc == NULL) return ;

	LONG dwType = 0;	//CSTLoadManager*
	if(m_pSTLoadMnger)
	{
		m_pSTLoadMnger->UnloadST();
		pFunc((LONG)m_pSTLoadMnger, dwType);
	}	
}

void CTabSystemTradingEx::AddSTFile(CListBox& rSTList, CString szPath)
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(szPath);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		rSTList.AddString(finder.GetFileTitle());
	}
}

// ~�ӽ��۾�

HBRUSH CTabSystemTradingEx::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CTabDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
	default:
		{
			pDC->SetBkColor(m_crBk);
			return m_brBkColor;
		}		
		break;
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

//void CTabSystemTradingEx::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
//{
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
//	
//}

void CTabSystemTradingEx::OnBnClickedStchkdiv()
{
	if(!m_pDlgOption) return;

	UINT nChk = IsDlgButtonChecked(IDC_STCHKDIV);
	if(m_pDlgOption)
	{
		m_pDlgOption->m_BtnSTApply.EnableWindow((BOOL)nChk);
		if(nChk)
		{	
			m_bSTBtn_Click = FALSE;
			if(m_bShowOption) return;//KHD
			m_BtnOption.FP_vSetImage(m_pIL_W20H20, 44, 46,TRUE,"",FALSE);//KHD
			ShowHideStOptimaA(TRUE, TRUE);		// bChange=TRUE, bShowOption=TRUE
		}
		else
		{
			m_bSTBtn_Click = TRUE;
			if(!m_bShowOption) return;//KHD
			m_BtnOption.FP_vSetImage(m_pIL_W20H20, 48, 50,TRUE,"",FALSE);//KHD
			ShowHideStOptimaA(TRUE, FALSE);		// bChange=TRUE, bShowOption=TRUE
		}
		m_pDlgOption->m_BtnSTApply.Invalidate();
		m_BtnOption.Invalidate();
	}
}


void CTabSystemTradingEx::DataMonitorCallbackProc(LPCSTR szKey, long pThisPoint, long dwKey, long dwSubKey, long dwNotifyData)
{
	CTabSystemTradingEx* pThis = (CTabSystemTradingEx*)pThisPoint;
	CString strKey = szKey;

	if(strKey == "STLIST_MODIFIED") 
	{
		// [NOTE] : �ý���Ʈ���̵��� ��������Ʈ�� ����Ǿ��� ���� ó���� �Ѵ�.
		if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG)
		{
			LPCSTR szSTName = (LPCSTR)dwNotifyData;
			switch(dwSubKey)
			{
			case Basic_STChange_Delete:
				pThis->m_STBasicTree.DeleteSTItem("���������", szSTName);	break;
			case Basic_STChange_Add:
				pThis->m_STBasicTree.AddSTItem("���������", szSTName);		break;
			case Ext_STChange_SDDelete:
				pThis->m_STTree.DeleteSTItem("����ڴ�������", szSTName);	break;
			case Ext_STChange_SDAdd:
				pThis->m_STTree.AddSTItem("����ڴ�������", szSTName);		break;
			case Ext_STChange_Delete:
				pThis->m_STTree.DeleteSTItem("�������������", szSTName);	break;
			case Ext_STChange_Add:
				pThis->m_STTree.AddSTItem("�������������", szSTName);		break;
			}
		}
		else	// ���� ������ƾ�� �״��
		{
			LPCSTR szSTName = (LPCSTR)dwNotifyData;
			pThis->m_pSTLoadMnger->UpdateST(dwSubKey, szSTName);

		}

	}
	//	else if(strKey == "MultiChart_ReceiveData")
	//	{
	//		// dwSubKey�� ��Ƽ��Ʈ�� ����Ʈ��.
	//		// ���纸�� �ִ� ��Ƽ��Ʈ���� �����Ѵ�.
	//		long pMainDlg = (long)pThis->m_pMultiChartWnd;
	//		if(pMainDlg==dwSubKey)
	//		{
	//			pThis->STClear();
	//			pThis->SetTimer(ID_RUNST, 300, NULL);
	//			//pThis->PostMessage(RMSG_RUN_MULTIST, 0, 0 ); // RunMultiST();
	//		}
	//	}
}

void CTabSystemTradingEx::MakeSTDefaultDirectory()
{
	//CString szDir = (LPCSTR)AfxGetPctrData(17);	// 17 : User���丮
//@��������
//	CString szDir = m_pParentDlg->m_strRootPath + "\\User\\";
//	CString szPath;
//@��������
//@�����߰�
	CString szDir = m_szEncUserDir;
	CString szPath;
//@�����߰�

	//�߰����� : ��������⿡�� SD, SDD�� �����ϰ���� ���ٳ��� ������ �߰���.
	const int nFolderCnt=9;
	char *aSubDir[nFolderCnt] = {"STData", "STData\\ST", "STData\\ST\\����ڴ�������", "STData\\ST\\�������������"
		,"STData\\Temp", "STData\\STBasic", "STData\\STBasic\\���������"
		, "STData\\SD", "STData\\SDD"};
/*
	const int nFolderCnt=5;
	char *aSubDir[nFolderCnt] = {"STData", "STData\\SD", "STData\\ST", "STData\\ST\\�������������","STData\\Temp"};
*/
	for(int i=0; i<nFolderCnt; i++) 
	{
		szPath = szDir + "\\" + aSubDir[i];
		::CreateDirectory(szPath, NULL);
	}
}

// -----------------------------------------------------------------------
// ��� : ����ǰ� �ִ� ������ �������� ��������â�� ����
//		  ����ǰ� �ִ� ������ ������ ���������� �����.
//		  ChartSTIndex.dll ������ ��ü������ ó����.
// -----------------------------------------------------------------------
LONG CTabSystemTradingEx::OnRmsgObItemClk(WPARAM wParam, LPARAM lParam)
{
	STMNG_ITEMINFO* pArg = (STMNG_ITEMINFO*)lParam;

	m_xSTAnalysisInput.m_InfoForAnalysis.nSTType		= pArg->nSTType;
	m_xSTAnalysisInput.m_InfoForAnalysis.szSTName		= pArg->szSTName;
	m_xSTAnalysisInput.m_InfoForAnalysis.szSTPath		= pArg->szSTPath;
	m_xSTAnalysisInput.m_InfoForAnalysis.szSTRData		= pArg->szSTRData;	
	m_xSTAnalysisInput.m_InfoForAnalysis.nFolderIndx	= pArg->nFolderIndx;
	m_xSTAnalysisInput.m_InfoForAnalysis.nItemIndx		= pArg->nItemIndx;	


	return 0;
}

BOOL CTabSystemTradingEx::IsSTRun(LPCSTR p_szSTPath, LPCSTR p_szSTName, int p_nSTType)
{
	stAddinSTRunInfo InData;
	InData.bIsRun = FALSE;
	InData.szRunSTFile = p_szSTPath;
	InData.szSTFile = p_szSTPath;
	InData.szSTName = p_szSTName;

	if(Version_stAddinSTRunInfo==070213)
	{
		InData.nSTType = p_nSTType;
	}

	CChartItem* pCItem = ((CChartMng*)m_pChartMng)->GetpChartItemSelected();
	if(pCItem) {
		pCItem->InvokeAddInSTIndex("ChartSTIndexAddIn.dll", 11, (long)&InData);
	}

	return InData.bIsRun;
}

#include "../Include_ST/STOrderDef_GNF.h"
LONG CTabSystemTradingEx::OnRmsgObItemDBClk(WPARAM wParam, LPARAM lParam)
{
	STMNG_ITEMINFO* pArg = (STMNG_ITEMINFO*)lParam;

	m_xSTAnalysisInput.m_Info.nSTType		= pArg->nSTType;
	m_xSTAnalysisInput.m_Info.szSTName		= pArg->szSTName;
	m_xSTAnalysisInput.m_Info.szSTPath		= pArg->szSTPath;
	m_xSTAnalysisInput.m_Info.szSTRData		= pArg->szSTRData;	
	m_xSTAnalysisInput.m_Info.nFolderIndx	= pArg->nFolderIndx;
	m_xSTAnalysisInput.m_Info.nItemIndx		= pArg->nItemIndx;	

	m_xSTAnalysisInput.m_InfoForAnalysis.nSTType		= pArg->nSTType;
	m_xSTAnalysisInput.m_InfoForAnalysis.szSTName		= pArg->szSTName;
	m_xSTAnalysisInput.m_InfoForAnalysis.szSTPath		= pArg->szSTPath;
	m_xSTAnalysisInput.m_InfoForAnalysis.szSTRData		= pArg->szSTRData;	
	m_xSTAnalysisInput.m_InfoForAnalysis.nFolderIndx	= pArg->nFolderIndx;
	m_xSTAnalysisInput.m_InfoForAnalysis.nItemIndx		= pArg->nItemIndx;	
	
#ifndef _DEBUG
	if(m_xSTAnalysisInput.GetCount()==0) return 0L;	// ��ȸ�� �����Ͱ� ���� ���� �ƹ�ó������ �ʴ´�.
#endif

	 //m_xSTRunManager.RunST(pArg->szSTName);
	 //PostMessage(WM_COMMAND, IDC_STWIZARDA, 0);
	CString szPath;
	szPath.Format("%s\\%s.ST", pArg->szSTPath, pArg->szSTName);

	int nRunSTType = STRun_Type_ST;

	// �������ǥ ��Ÿ�� �϶��� ����â�� ����� �ʰ� �ٷ� ������ �����Ѵ�.
	// @1 : ��ǥ, @2,@3,@7 : ����, @4, @5 : �����༼, @8 : ����
	//if(pArg->szSTName.Find("@1")>0)
	if(pArg->szSTName.Find("^1")>0)	//090902-solomon @->^
	{
		nRunSTType = STRun_Type_Index;
		return RunIndexSD(nRunSTType, pArg);
	}
	//else if(m_xSTAnalysisInput.m_Info.szSTRData.CompareNoCase("����ڰ����༼")==0)
	//else if(pArg->szSTName.Find("@4")>0	|| pArg->szSTName.Find("@5")>0)
	else if(pArg->szSTName.Find("^4")>0	|| pArg->szSTName.Find("^5")>0)	//090902-solomon @->^
		//		|| pArg->szSTRData.Compare(SZSTKEY_USERINDEXN)==0)
	{
		nRunSTType = STRun_Type_BullBear;
		return RunIndexSD(nRunSTType, pArg);
	}
	//else if(pArg->szSTName.Find("@0")==0) return 0L;	// �ƹ�ó���� ���� �ʴ´�.
	else if(pArg->szSTName.Find("^0")==0) return 0L;	//090902-solomon @->^

	CString szSTPath = pArg->szSTPath + "\\" + pArg->szSTName + ".ST";
	char aSDName[MAX_PATH]={0,};
	if(::GetPrivateProfileString("StrategyList", "PosName0", pArg->szSTName, aSDName, MAX_PATH, szSTPath)==0)
		return 1L;

	if(IsSTRun(szPath, aSDName, nRunSTType))
	{
		// �������� ���� ����.
		//CChartItem* pCItem = ((CChartMng*)m_pChartMng)->GetpChartItemSelected();
		//if(pCItem)
		//	pCItem->InvokeAddInSTIndex("ChartSTIndexAddIn.dll", 7, NULL);
		CString szTmp;
		m_xSTRunManager.RunST(FALSE, szTmp, (LONG)pArg);
		//m_szRunSTPath = _T("");
		return 0L;
	}

	// ���� ����â�� �ٿ��.
	RunWizard(FALSE);



/*------------------------------------------------------------------------------- 
�׽�Ʈ���̹Ƿ� �������� ���ƿ�^^
------------------------------------------------------------------------------- 
{
		typedef struct 
		{
			CString szSTFile;
			long	pChartInterface;
			CString szCode;
			int		nCount;
			CString szEndData;
			CStringArray stArrayPath;
		} stAddinSTRun;

		stAddinSTRun InData;
		InData.szSTFile = pArg->szSTPath + "\\" + pArg->szSTName + ".ST";
		InData.pChartInterface = (long)&m_xSTInterface;
		InData.szCode = m_xSTAnalysisInput.GetCode();
		InData.nCount = m_xSTAnalysisInput.GetCount();
		InData.szEndData = m_xSTAnalysisInput.GetEndDay();

#ifdef _DEBUG
		CString strSystemPath = m_pParentDlg->m_strRootPath +  "\\" + STSD_D;
		CString strUserPath = m_pParentDlg->m_strRootPath + "\\" + STUSER_D;
#else
		CString strSystemPath = m_pParentDlg->m_strRootPath + "\\" + STSD;
		CString strUserPath = m_pParentDlg->m_strRootPath + "\\" + STUSER;
#endif
		InData.stArrayPath.Add(strSystemPath);//Data
		InData.stArrayPath.Add(strUserPath);//User

		CChartItem* pCItem = ((CChartMng*)m_pChartMng)->GetpChartItemSelected();
		if(pCItem)
			pCItem->InvokeAddInSTIndex("ChartSTIndexAddIn.dll", 10, (long)&InData);

	}
------------------------------------------------------------------------------- */
	return 0L;
}

BOOL CTabSystemTradingEx::IsSTSelected()
{
	// ���õ� ������ �ִ��� üũ�Ѵ�.
	// -1 �̸� ���õ��� ���� ����.
//	int nFolderIndx	= m_xSTAnalysisInput.m_Info.nFolderIndx;
//	int nItemIndx	= m_xSTAnalysisInput.m_Info.nItemIndx;	
	int nFolderIndx	= m_xSTAnalysisInput.m_InfoForAnalysis.nFolderIndx;
	int nItemIndx	= m_xSTAnalysisInput.m_InfoForAnalysis.nItemIndx;	

	if(nFolderIndx>=0 && nItemIndx>=0)	return TRUE;

	return FALSE;
}

BOOL CTabSystemTradingEx::Run_STDLL(LPCSTR szDll)
{
	CString strText = szDll;
//@��������
//#ifdef _DEBUG
//	CString strUserPath = m_pParentDlg->m_strRootPath +"\\" + STSYSTEM_D;
//#else
//	CString strUserPath = m_pParentDlg->m_strRootPath +"\\" + STSYSTEM;
//#endif
//@��������
//@�����߰�
	CString strUserPath = m_pParentDlg->m_strRootPath +"\\" + PATH_DATA_STDATA_STSYSTEM;
//@�����߰�

	CStringArray strArrayName;
	CString strFilePathName = strUserPath + "\\" + strText + ".SD";
	CString strFileName = strText;	
	strArrayName.Add(strFileName);
	strArrayName.Add(strFilePathName);

	if(m_pChartMng->GetSafeHwnd())
		m_pChartMng->SendMessage(UMSG_ADDSYSTEMTRADINGINDEX,(WPARAM)&strArrayName);	

	return TRUE;
}

// ���� ������ ǥ�õ� �����ŸŽ�ȣ �� ��������Ʈ�� Clear�Ѵ�.
void CTabSystemTradingEx::STClear()
{
	m_xSTAnalysisInput.m_Info.nSTType		= -1;
	m_xSTAnalysisInput.m_Info.szSTName		= "";
	m_xSTAnalysisInput.m_Info.szSTPath		= "";
	m_xSTAnalysisInput.m_Info.szSTRData		= "";	
	m_xSTAnalysisInput.m_Info.nFolderIndx	= -1;
	m_xSTAnalysisInput.m_Info.nItemIndx		= -1;	

	CChartItem* pCItem = ((CChartMng*)m_pChartMng)->GetpChartItemSelected();
	if(pCItem)
	{
	//	pCItem->SetSTRunSTPath("");
		pCItem->InvokeAddInSTIndex("ChartSTIndexAddIn.dll", 2, (long)this);// solomon
		pCItem->InvokeAddInSTIndex("ChartSTIndexAddIn.dll", 7, (long)this);
	}
}

BOOL CTabSystemTradingEx::CanSTModule()
{
	return TRUE;

	if(m_xSTAnalysisInput.GetCodeType()==2)
	{
		AfxMessageBox("���������� ������� �ʽ��ϴ�");
		return FALSE;		// 0(�ֽ�), 1(��/��), 2(����), ������ ���ؼ��� ó������ �ʴ´�.
	}

	return TRUE;
}

//@��������
//#include "../../inc/ICustomMng.h"
//@��������
#include "../../inc/ISystemUtilMng.h"
void CTabSystemTradingEx::TestJumun()
{
/*
	ICumstomManager* pCMng = (ICumstomManager*)AfxGetPctrInterface(UUID_ICumstomManager);
	if(!pCMng) return;

	ICumstomSite_Info* pCSInfo = (ICumstomSite_Info*)pCMng->GetInterface(UUID_ICumstomSite_Info, NULL);
	if(!pCSInfo) return;

	pCSInfo->Initialize(this);

	LPCSTR szCode = m_xSTAnalysisInput.GetCode();
	pCSInfo->GetHoga(szCode);
*/


/*------------------------------------------------------------------------------- 
�׽�Ʈ���̹Ƿ� �������� ���ƿ�^^
------------------------------------------------------------------------------- 

	{
	STORDER_INDATA inData;
	memset(&inData, 0, SZSTORDER_INDATA);

	inData.nMarket = m_xSTAnalysisInput.GetCodeType();			// 0:�ֽ�, 1:����/�ɼ�, 2:����
	inData.nOrderType = 0;										// 0:�ý���Ʈ���̵� �ż�����, 1:�ż�����, 2:�ŵ�û��, 3:�ŵ�����
	sprintf(inData.aAcctNo, "123-12-123456");
	sprintf(inData.aAcctName, "������");
	sprintf(inData.aCode, m_xSTAnalysisInput.GetCode());			// �����ڵ�
	sprintf(inData.aCodeName, m_xSTAnalysisInput.GetCodeName());	// �����ڵ��
	inData.nOrderGubun = 1;						// �ֹ�����
	sprintf(inData.aOrderAmt, "10000");			// �ֹ�����
	//sprintf(inData.aOrderPrice, "100000");		// �ֹ�����
	inData.nOrderPriceType = 0;

	CStrategyOrderLoader orderLoader;
	int nRet = orderLoader.STOrder_DoModal((long)&inData);
	TRACE("STOder Result = %d\n", nRet);
	}

------------------------------------------------------------------------------- */
}

LONG CTabSystemTradingEx::OnRmsgMultiST(WPARAM wp, LPARAM lp)
{
	RunMultiST();
	return 0L;
}

LONG CTabSystemTradingEx::OnRmsgIndexAddinNotifyRawData(WPARAM wp, LPARAM lp)
{
	int nType = (int)lp;
	STMNG_ITEMINFO* pInData = (STMNG_ITEMINFO*)wp;

	switch(nType) {
	case 0:
		{
			if(pInData)
			{
				m_xSTAnalysisInput.m_Info.nSTType		= pInData->nSTType;
				m_xSTAnalysisInput.m_Info.szSTName		= pInData->szSTName;
				m_xSTAnalysisInput.m_Info.szSTPath		= pInData->szSTPath;
				m_xSTAnalysisInput.m_Info.szSTRData		= pInData->szSTRData;	
				m_xSTAnalysisInput.m_Info.nFolderIndx	= 0;
				m_xSTAnalysisInput.m_Info.nItemIndx		= 0;

				delete pInData;
				pInData = NULL;
			}
			else
			{
				m_xSTAnalysisInput.m_Info.nSTType		= -1;
				m_xSTAnalysisInput.m_Info.szSTName		= "";
				m_xSTAnalysisInput.m_Info.szSTPath		= "";
				m_xSTAnalysisInput.m_Info.szSTRData		= "";	
				m_xSTAnalysisInput.m_Info.nFolderIndx	= -1;
				m_xSTAnalysisInput.m_Info.nItemIndx		= -1;	
			}
			OnRmsgMultiST(wp, lp);
		}
		break;
	case 1:
			m_pDlgOption->SetInitDlg();//Clear�϶�..
		break;
	}
	return 0L;
}

/*
void CTabSystemTradingEx::DirectRunST(LPCSTR szSTFile)
{
	STClear();	// ���ҵ� ó���� ������ �����Ѵ�.

	stAddinSTRun InData;

	m_szImsiRunSTFile = "";
	InData.szSTFile = szSTFile;
	InData.pChartInterface = (long)&m_xSTInterface;
	InData.szCode = m_xSTAnalysisInput.GetCode();
	InData.szCode = InData.szCode.Right(7);
	InData.nCount = m_xSTAnalysisInput.GetCount();
	InData.szEndData = m_xSTAnalysisInput.GetEndDay();

//@��������
//#ifdef _DEBUG
//	CString strSystemPath = m_pParentDlg->m_strRootPath +  "\\" + STSD_D;
//	CString strUserPath = m_pParentDlg->m_strRootPath + "\\" + STUSER_D;
//#else
//	CString strSystemPath = m_pParentDlg->m_strRootPath + "\\" + STSD;
//	CString strUserPath = m_pParentDlg->m_strRootPath + "\\" + STUSER;
//#endif
//@��������
//@�����߰�
	CString strSystemPath = m_pParentDlg->m_strRootPath + "\\" + STSD;
	//CString strUserPath = m_pParentDlg->m_strRootPath + "\\" + STUSER;
	CString strUserPath = m_szEncUserDir+ "\\" + STFOLDER_SD;
//@�����߰�
	InData.stArrayPath.Add(strSystemPath);//Data
	InData.stArrayPath.Add(strUserPath);//User

	sprintf(InData.szCodeName, "%s", m_xSTAnalysisInput.GetCodeName());
	InData.nCodeType = m_xSTAnalysisInput.GetCodeType();

	CChartItem* pCItem = ((CChartMng*)m_pChartMng)->GetpChartItemSelected();
	if(pCItem)
		pCItem->InvokeAddIn("ChartSTIndexAddIn.dll", 10, (long)&InData);
}
*/
void CTabSystemTradingEx::DirectRunST(LPCSTR szSTFile)
{
	STClear();	// ���ҵ� ó���� ������ �����Ѵ�.
	
	CString szSTName(szSTFile);
	int nPos = szSTName.Find('.');
	szSTName.Delete(nPos,szSTName.GetLength()-nPos);
	while(1)
	{
		nPos = szSTName.Find('\\');
		if(nPos!=-1)
		{
			szSTName.Delete(0,nPos+1);
		}
		else
		{
			break;
		}	
	}
	
	CString szSTPath(szSTFile);
	nPos = szSTPath.ReverseFind('\\');
	if(nPos!=-1)
	{
		szSTPath = szSTPath.Left(nPos);
	}
	
	m_xSTAnalysisInput.m_Info.szSTPath = szSTPath;
	m_xSTAnalysisInput.m_Info.szSTName = szSTName;
	
	stAddinSTRun InData;
	
	m_szImsiRunSTFile = "";
	InData.szSTFile = szSTFile;
	InData.pChartInterface = (long)&m_xSTInterface;
	InData.szCode = m_xSTAnalysisInput.GetCode();
	InData.nCount = m_xSTAnalysisInput.GetCount();
	InData.szEndData = m_xSTAnalysisInput.GetEndDay();
	InData.szSTName = m_xSTAnalysisInput.m_Info.szSTName;
	InData.wndInfo.hSystemTabEx = GetSafeHwnd();
	
//#ifdef _DEBUG
//	CString strSystemPath = m_szDataDir + "\\STData\\SDD";
//	CString strUserPath = m_szEncUserDir + "\\STData\\SDD";
//#else
	CString strSystemPath = m_szDataDir + "\\STData\\SD";
	CString strUserPath = m_szEncUserDir + "\\STData\\SD";
//#endif
	
	InData.stArrayPath.Add(strSystemPath);//Data
	InData.stArrayPath.Add(strUserPath);//User
	
	sprintf(InData.szCodeName, "%s", m_xSTAnalysisInput.GetCodeName());
	InData.nCodeType = m_xSTAnalysisInput.GetCodeType();
	
	CChartItem* pCItem = ((CChartMng*)m_pChartMng)->GetpChartItemSelected();
	if(pCItem)
	{
//		pCItem->SetSTRunSTPath(InData.szSTFile);
		pCItem->InvokeAddInSTIndex("ChartSTIndexAddIn.dll", 10, (long)&InData);
	}
}

// ������������
void CTabSystemTradingEx::RunMultiST()
{
	stAddinSTRun InData;
	if(m_szImsiRunSTFile.GetLength())
	{
		InData.szSTFile = m_szImsiRunSTFile;
		m_szImsiRunSTFile = "";
	}
	else
	{
		InData.szSTFile = m_xSTAnalysisInput.m_Info.szSTPath + "\\" + m_xSTAnalysisInput.m_Info.szSTName + ".ST";
	}

	InData.pChartInterface = (long)&m_xSTInterface;
	InData.szCode = m_xSTAnalysisInput.GetCode();
	InData.szCode = InData.szCode.Right(7);
	InData.nCount = m_xSTAnalysisInput.GetCount();
	InData.szEndData = m_xSTAnalysisInput.GetEndDay();
//	InData.szSTName  = m_xSTAnalysisInput.m_Info.szSTName;//KHD :2006.11.16

	CString strSTName = InData.szSTFile;
	int nPosExt = strSTName.ReverseFind('.');
	int nPosFileTitle = strSTName.ReverseFind(('\\'));
	strSTName = strSTName.Left(nPosExt);
	strSTName = strSTName.Mid(nPosFileTitle+1);
	InData.szSTName = strSTName;
	{
		int nFind = InData.szSTName.Find("@");
		if(nFind>0) InData.szSTName = InData.szSTName.Left(nFind);
	}
//@��������
//#ifdef _DEBUG
//	CString strSystemPath = m_pParentDlg->m_strRootPath +  "\\" + STSD_D;
//	CString strUserPath = m_pParentDlg->m_strRootPath + "\\" + STUSER_D;
//#else
//	CString strSystemPath = m_pParentDlg->m_strRootPath + "\\" + STSD;
//	CString strUserPath = m_pParentDlg->m_strRootPath + "\\" + STUSER;
//#endif
//@��������
//@�����߰�
//	CString strSystemPath = m_pParentDlg->m_strRootPath + "\\" + PATH_DATA_STDATA_SD;
//	CString strUserPath = m_pParentDlg->m_strRootPath + "\\" + PATH_USER_STDATA_SD;
	CString strSystemPath = m_szDataDir + "\\STData\\SD";
	CString strUserPath = m_szEncUserDir + "\\STData\\SD";
//@�����߰�
	InData.stArrayPath.Add(strSystemPath);//Data
	InData.stArrayPath.Add(strUserPath);//User

	InData.wndInfo.hSystemTabEx = GetSafeHwnd();		//@@@@@06.11.20 HWND
	sprintf(InData.szCodeName, "%s", m_xSTAnalysisInput.GetCodeName());
	InData.nCodeType = m_xSTAnalysisInput.GetCodeType();
	CChartItem* pCItem = ((CChartMng*)m_pChartMng)->GetpChartItemSelected();
	if(pCItem)
	{
		pCItem->InvokeAddInSTIndex("ChartSTIndexAddIn.dll", 10, (long)&InData);
		if(m_pDlgOption && m_pDlgOption->m_bIsApplyBtn) 
			m_pDlgOption->OnBnClickedStapply();
	}

//	COPY_STMNG_ITEMINFO(m_LastRunInfo, m_xSTAnalysisInput.m_Info);
}


#include "../../inc/ISystemUtilMng.h"
/*
void CTabSystemTradingEx::CheckRegister(LPCSTR szProgID, LPCSTR szFileName)
{
	CLSID clsid;
	ISystemUtilManager* pMng = (ISystemUtilManager*)AfxGetPctrInterface(UUID_ISystemUtilManager);
	if(pMng)
	{
		if(pMng->IsRegistered(szProgID, clsid)==FALSE)
		{
			CString szOcxName;
			szOcxName.Format("%s\\Bin\\%s", m_pParentDlg->m_strRootPath, szFileName);

			pMng->DLLRegister(szOcxName);
		}
	}
}
*/


BOOL CTabSystemTradingEx::CheckRegister(LPCSTR p_szProgID, LPCSTR p_szFileName)
{
	CLSID clsid;
	//@SolomonGNF ISystemUtilManager2* pMng = (ISystemUtilManager2*)AfxGetPctrInterface(UUID_ISystemUtilManager, 3);
	ISystemUtilManager2* pMng = (ISystemUtilManager2*)AfxGetPctrInterface(UUID_ISystemUtilManager);
	if(pMng)
	{

		CString szOcxName;
		szOcxName.Format("%s\\dev\\%s", m_pParentDlg->m_strRootPath, p_szFileName);

		if(pMng->IsRegisteredEx(p_szProgID, clsid, p_szFileName)==FALSE)
		{

			pMng->DLLRegister(szOcxName);
		}
		return pMng->IsRegistered(p_szProgID, clsid);
	}
	return TRUE;
}

BOOL CTabSystemTradingEx::CheckOcx()
{
	// MSChart�� TeeChart OCX�� ����Ѵ�. �����м� �� ����ȭ�� ���ؼ�.
	CString szProgID; 
	szProgID = "MSChart20Lib.MSChart.2";			if(!CheckRegister(szProgID, "Mschrt20.ocx")) return FALSE;
	szProgID = "TeeChart.ChartPageNavigator.7";		if(!CheckRegister(szProgID, "TeeChart7.ocx")) return FALSE;
	return TRUE;
}

//���� 
void CTabSystemTradingEx::OnStshowoption()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(m_bSTBtn_Click)//FALSE : Hide , TRUE:Show
	{
		m_bSTBtn_Click = !m_bSTBtn_Click;
		m_BtnOption.FP_vSetImage(m_pIL_W20H20, 44, 46,TRUE,"",FALSE);//KHD
		ShowHideStOptimaA(TRUE, TRUE);
	}
	else
	{
		m_bSTBtn_Click = !m_bSTBtn_Click;
		m_BtnOption.FP_vSetImage(m_pIL_W20H20, 48, 50,TRUE,"",FALSE);//KHD ���� ȭ��ǥ
		HideStOptimaA();
	}
	m_BtnOption.Invalidate();

}

//*****************************************************************************
// ��  ��  ��  : void CTabSystemTradingEx::RunIndexSD(LPCSTR p_szSDName) 
// ��      ��  : ��ǥ�� �����ϴ� ��ƾ
//				 ������ CTabSystemTrading���� �����ߴ� ����� ���׷��̵� ��.
//				 ��ǥ�� ST���� "PosName0" ù��°�͸��� ó���Ѵ�.
// ��      ��  : 
// ȣ��  �Լ�  : 
// ��  ��  ��  : ���ؿ�     (��)Ʃ����ó��
// Date        : 2007-01-22 ���� 10:48:47
//*****************************************************************************
LRESULT CTabSystemTradingEx::RunIndexSD(int p_nSTRunType, STMNG_ITEMINFO* p_pArg) 
{
	//  ����(ST)�� �ٷ� �����ϴ� ��� 
	//	CString szTmp;
	//	m_xSTRunManager.RunST(FALSE, szTmp, (LONG)pArg);
	//	return 0L;

	CString szTmp;

	CString strSystemPath = m_pParentDlg->m_strRootPath + "\\" + PATH_DATA_STDATA_SD;
	CString strUserPath = m_pParentDlg->m_strRootPath + "\\" + PATH_USER_STDATA_SD;


	CString strSDPath;
	if(p_pArg->nSTType==0)	// ��������(0), ����� ����(1)
		strSDPath = m_pParentDlg->m_strRootPath + "\\" + PATH_DATA_STDATA_SD;
	else
		strSDPath = m_szEncUserDir + "\\STData\\SD";

	CString szSTPath = p_pArg->szSTPath + "\\" + p_pArg->szSTName + ".ST"; 
	char aSDName[MAX_PATH]={0,};
	if(::GetPrivateProfileString("StrategyList", "PosName0", "", aSDName, MAX_PATH, szSTPath)==0)
		return 1L;

	CString strText = aSDName;
	CString strFileName = strText;
	CString strFilePathName = strSDPath + "\\" + strText + ".SD";
	//CString strFilePathName = strSDPath + "\\" + p_pArg->szSTName + ".SD";

	CStringArray strArrayName;
	strArrayName.Add(strFileName);
	strArrayName.Add(strFilePathName);

	// �Ʒ��� ��ƾ�� SD�� �ٷ� �����ϴ� ���
	if(p_nSTRunType==STRun_Type_Index)
	{
		if(m_pChartMng->GetSafeHwnd())
			m_pChartMng->SendMessage(UMSG_ADDSYSTEMTRADINGINDEX,(WPARAM)&strArrayName);

		return 1L;
	}
	// 200 : �����༼
	else if(p_nSTRunType==STRun_Type_BullBear)
	{
		if(IsSTRun(strFilePathName, strText, p_nSTRunType))
		{
		}
		else
		{
			LONG dwOutput=0;
			CStringList szInputList;

			// MakeInput
			{
				szTmp.Format("STNAME=%s", p_pArg->szSTName);	// ������
				szInputList.AddTail(szTmp);

				szTmp.Format("SFGPATH=%s\\%s", strSDPath, p_pArg->szSTName);	// SFG���, ���������� @�������� �߶�� ��.
				szInputList.AddTail(szTmp);

				szTmp.Format("STPATH=%s\\%s%s", p_pArg->szSTPath, p_pArg->szSTName, ".ST");	// ����(ST)���
				szInputList.AddTail(szTmp);
			}
			int nRet = DoDlgSysVarOption(&szInputList, dwOutput);
			if(nRet==IDCANCEL) return 1L;
		}

		if(m_pChartMng->GetSafeHwnd())
			m_pChartMng->SendMessage(UMSG_ADDSYSTEMTRADINGINDEX,(WPARAM)&strArrayName);

//		COPY_STMNG_ITEMINFO(m_LastRunInfo, m_xSTAnalysisInput.m_Info);
		return 1L;
	}

	return 1L;
}

int CTabSystemTradingEx::DoDlgSysVarOption(CStringList* p_dwInput, LONG dwOutput)
{
	int nRet = IDCANCEL;
	CString szDllName = "HWStrategyConfigureWizard.dll";	// DLL�� �ִ´�.
	HINSTANCE hLib = ::LoadLibrary(szDllName);
	if(hLib)
	{
		typedef	int (LIB_DoModal_DlgSysVarOption)(CStringList* p_dwInput, LONG dwOutput);
		LIB_DoModal_DlgSysVarOption* pFunc = (LIB_DoModal_DlgSysVarOption*)GetProcAddress(hLib,_T("DoModal_DlgSysVarOption"));
		if(pFunc)
		{
			nRet = pFunc(p_dwInput, dwOutput);
		}
		::FreeLibrary(hLib);
	}
	return nRet;
}

void CTabSystemTradingEx::ChangeSkinColor(COLORREF clrBkgrnd)
{
	m_crBk = (COLORREF)m_pMultiChartWnd->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
	m_brBkColor.DeleteObject();
	m_brBkColor.CreateSolidBrush(m_crBk);

	STOutBarColor OLColor;
	if(m_pMultiChartWnd->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)2, (LPARAM)&OLColor))
	{
		if(m_pWndOutBar)
		{
			m_pWndOutBar->Setcr3dFace(OLColor.clrOLOutLine);
			m_pWndOutBar->Setcr3dUnFace(OLColor.clrOLBkgrnd);
		}
	}
	
	Invalidate();
}

LONG CTabSystemTradingEx::OnRmsgIndexAddinNotifyBuySell(WPARAM wp, LPARAM lp)
{
	STORDER_INDATA* lpOrderData = (STORDER_INDATA*)wp;
	CSTConfigEnv* lpConfigEnv = (CSTConfigEnv*)lp;

	if(gCURRENT_COMPANY_ID==nPCTRComID_EUGENE)
	{
		PlayOrderSound(	lpConfigEnv->m_nOrderType,
						lpConfigEnv->m_InfoJumun.m_bSound_Buy_Use,
						lpConfigEnv->m_InfoJumun.m_szSound_Buy_File,
						lpConfigEnv->m_InfoJumun.m_bSound_Sell_Use,
						lpConfigEnv->m_InfoJumun.m_szSound_Sell_File );


		CString strCode, strPrice, strOrdertype, strAccount;

		strCode = lpConfigEnv->m_szCode;
		strCode = strCode.Right(7);

		strPrice = "";

		// 0:�ý���Ʈ���̵� �ż�����, 1:�ż�����, 2:�ŵ�û��, 3:�ŵ�����
		if (lpConfigEnv->m_nOrderType == 0)
		{
			strOrdertype = "1";
		}
		else if (lpConfigEnv->m_nOrderType == 1)
		{
			strOrdertype = "1";
		}
		else if (lpConfigEnv->m_nOrderType == 2)
		{
			strOrdertype = "0";
		}
		else if (lpConfigEnv->m_nOrderType == 3)
		{
			strOrdertype = "0";
		}

		strAccount = lpOrderData->aAcctNo;

// --> [Edit]  �̵��� 2008/09/25
// 
//		char *pData = new char[100];
//		memset(pData, 0x00, 100);
//		sprintf(pData, "%s,%s,%s,%s", strCode, strPrice, strOrdertype, strAccount);
//		CStdDialog* pMainDlg = (CStdDialog*)m_pMultiChartWnd;
//		::SendMessage(pMainDlg->m_hViewInMain, RMSG_GET_LINKINFODATA, 2, (LPARAM)pData);
//		delete []pData;
// <-- [Edit]  �̵��� 2008/09/25

		CString strOut;
		if (strOrdertype == "0")
		{
			strOut.Format("���¹�ȣ : [%s] \n�����ڵ� : [%s] \n�ֹ����� : [���尡] \n�ֹ����� : [%s]\n�ý���Ʈ���̵� �ֹ��� \n���Ŀ� �ݿ��� �����Դϴ�.",
						  strAccount, strCode, "�ŵ�");
		}
		else
		{
			strOut.Format("���¹�ȣ : [%s] \n�����ڵ� : [%s] \n�ֹ����� : [���尡] \n�ֹ����� : [%s]\n�ý���Ʈ���̵� �ֹ��� \n���Ŀ� �ݿ��� �����Դϴ�.",
						  strAccount, strCode, "�ż�");
		}

		AfxMessageBox(strOut);


	}

	return 0L;
}

LONG CTabSystemTradingEx::OnRmsgIndexAddinNotifySignal(WPARAM wp, LPARAM lp)
{
	CSTConfigEnv* pEnv = (CSTConfigEnv*)lp;

	if(gCURRENT_COMPANY_ID==nPCTRComID_EUGENE)
	{
		PlayOrderSound(	pEnv->m_nOrderType,
						pEnv->m_InfoJumun.m_bSound_Buy_Use,
						pEnv->m_InfoJumun.m_szSound_Buy_File,
						pEnv->m_InfoJumun.m_bSound_Sell_Use,
						pEnv->m_InfoJumun.m_szSound_Sell_File );
	}
	return 0L;
}

#define		OFFSET_Jumun_X		20
#define		OFFSET_Jumun_Y		22
#define		OFFSET_Jumun_MAX	(100+OFFSET_Jumun_X*8)
void CTabSystemTradingEx::CallJunmunDlg(CSTOrderRData_EG *pInData)
{
	if(!IsAllowJumun(pInData, FALSE))
	{
		return;
	}

	int nUserType = 0;
	//@SolomonGNF IMainInfoManager* pMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager, 3);
	IMainInfoManager* pMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
	if(pMng) nUserType = atoi(pMng->GetLoginInfo("ISHTS")); //"1":HTS  "0":FRONT(����)
	if(nUserType!=1)
	{
		TRACE("����(HTS)������ �ֹ��� �����մϴ�.\n");
		return;
	}

	CConfirmWnd* pConfirmWnd = new CConfirmWnd;
	pConfirmWnd->m_InData.Copy(pInData);
	pConfirmWnd->m_pSTDlgConfirmSendJumun = &m_xSTDlgConfirmSendJumun;
	m_mapConfirmWnd.SetAt(pConfirmWnd, (long)pConfirmWnd);
	if(!pConfirmWnd->OpenWindow())
	{
		delete pConfirmWnd;
	}
}

// 0:�ý���Ʈ���̵� �ż�����, 1:�ż�����, 2:�ŵ�û��, 3:�ŵ�����
// VALEXITSHORT : �ŵ��� ���� û��.
// VALEXITLONG �ż��� ���� û��.
// VALENTRYSELL 
void CTabSystemTradingEx::PlayOrderSound(int nOrderType, BOOL bBuySound, LPCSTR szBuyFile, BOOL bSellSound, LPCSTR szSellFile)
{
	if( (nOrderType&FORCEDEXIT) || (nOrderType&VALEXITLONG) || (nOrderType&VALEXITSHORT) )
	{
		if(bSellSound) sndPlaySound (szSellFile, SND_ASYNC);
	}
	else if( (nOrderType & VALENTRYBUY) || (nOrderType & VALENTRYSELL) )
	{
		if(bBuySound) sndPlaySound (szBuyFile, SND_ASYNC);
	}
}

//*****************************************************************************
// ��  ��  ��  : BOOL CTabSystemTradingEx::IsAllowJumun(CSTOrderRData_EG *pInData, BOOL bShowMsg)
// ��      ��  : �ֹ����ɿ��θ� üũ�Ѵ�.
//				 �ֹ��� �ȵǴ� ��� : 
//					- ����
//					- ������
// ��      ��  : bShowMsg : TRUE�̸� �����޽����� �����ش�.
// ȣ��  �Լ�  : 
// ��  ��  ��  : ���ؿ�     (��)Ʃ����ó��
// Date        : 2007-01-20 ���� 7:57:15
//*****************************************************************************
BOOL CTabSystemTradingEx::IsAllowJumun(CSTOrderRData_EG *pInData, BOOL bShowMsg)
{
	if(AfxGetApp()->GetProfileInt("STBAsic", "AllowJumun", 0)==1) 
	{
		return TRUE;
	}

	// 1. HTS�� �ֹ�����  "1":HTS  "0":FRONT(����)
	if(m_nUserType==0) return FALSE;

	//2. ��ȸ������ üũ, �α��ν� ��������üũ�ߴ���, ��ȸ������ �����ߴ��� Ȯ���Ѵ�.
//	if(m_bSignCheckLogin==false) return FALSE;

	//3. ���������� üũ
	{
		// InputBarDlg.h
		// enum	{STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, MS30_CHART, FOREIGN_CHART, ELW_CHART};
		CStdDialog* pMainDlg = (CStdDialog*)m_pMultiChartWnd;
		CInputBarDlg* pInputBarDlg = (CInputBarDlg*)((CChartMng*)m_pChartMng)->GetpInputBarDlg();

		// �ֽ���Ʈ�� �����ɼ� ��Ʈ�� �ƴϸ� �ֹ�ó���� ���� �ʴ´�.
		if (pInputBarDlg->m_nMarketType != CMasterTypeInfo::STOCK_CHART &&
			pInputBarDlg->m_nMarketType != CMasterTypeInfo::FUTOPT_CHART)
			return FALSE;

		//int		m_nDataAddMode; //2 : ������ ���ý�
//		if(pInputBarDlg->m_nDataAddMode==2) return FALSE;
	}

	return TRUE;
}

// ������ �����ϱ� ���� üũ�Ѵ�.
// ELW�� �ֹ��� �ȵǵ��� üũ�� �ʿ��ؼ� ���� üũ�Ѵ�.
BOOL CTabSystemTradingEx::IsAllowJumunBefore(LPCSTR szSTFile, CString& szMsg)
{
// 	int nCodeType = m_xSTAnalysisInput.GetCodeType();
// 	if(nCodeType<0 || nCodeType==2)
// 	{
// 		int nJMethod = ::GetPrivateProfileInt("RunJumun", "JMethod", 0, szSTFile);
// 		if(nJMethod==1)
// 		{
// 			switch(nCodeType)
// 			{
// 			case 2:
// 				szMsg = "�������������� �ֹ��� �Ұ��մϴ�.";
// 				break;
// 			case -1:
// 				szMsg = "MS30������ �ֹ��� �Ұ��մϴ�.";
// 				break;
// 			case -2:
// 				szMsg = "�ؿ����������� �ֹ��� �Ұ��մϴ�.";
// 				break;
// 			case -3:
// 				szMsg = "ELW������ �ֹ��� �Ұ��մϴ�.";
// 				break;
// 			default:
// 				szMsg = "���� ������ ���񿡼��� �ֹ��� �Ұ��մϴ�.";
// 				break;
// 			}
// 			return FALSE;
// 		}
// 	}

	return TRUE;
}

CWnd* CTabSystemTradingEx::CSTDlgConfirmSendJumun::GetParent()
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STDlgConfirmSendJumun)

	return (CWnd*)pThis;
}

HWND CTabSystemTradingEx::CSTDlgConfirmSendJumun::GetHwndScreenView()
{
	MYMETHOD_PROLOGUE(CTabSystemTradingEx, STDlgConfirmSendJumun)

	AfxMessageBox("[GetHwndScreenView]Solomon�����ʿ�"); //@Solomon-091015

	return ((CStdDialog*)pThis->m_pMultiChartWnd)->GetSafeHwnd();
}

LONG CTabSystemTradingEx::OnRmsgDestroyConfirmWnd( WPARAM wp, LPARAM lp)
{
	CConfirmWnd* pConfirmWnd = (CConfirmWnd*)wp;
	if(pConfirmWnd)
	{
		long rValue = 0;
		if(m_mapConfirmWnd.Lookup(pConfirmWnd, rValue))
		{
			if(pConfirmWnd->GetSafeHwnd())
				pConfirmWnd->DestroyWindow();

			m_mapConfirmWnd.RemoveKey(pConfirmWnd);

			delete pConfirmWnd;
			pConfirmWnd = NULL;

		}
	}

	return S_OK;
}

LONG CTabSystemTradingEx::OnRmsgIsSTRunState( WPARAM wp, LPARAM lp)
{
	LPCTSTR lpszSTPath = (LPCTSTR)wp;
	LPCTSTR lpszSTName = (LPCTSTR)lp;
	return (LONG)IsSTRun(lpszSTPath, lpszSTName, STRun_Type_ST);
}

void CTabSystemTradingEx::OnBtnStDelete()
{
	STClear();
}

