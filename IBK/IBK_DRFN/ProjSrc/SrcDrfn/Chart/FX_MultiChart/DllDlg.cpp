// DllDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MultiChart.h"
#include "DllDlg.h"
#include ".\dlldlg.h"

const UINT RMSG_GETDLLSIZE = ::RegisterWindowMessage("RMSG_GETDLLSIZE");
const UINT RMSG_SETDLLSIZE = ::RegisterWindowMessage("RMSG_SETDLLSIZE");

// CDllDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDllDlg, CDialog)
CDllDlg::CDllDlg(LPCSTR szDllName, LPCSTR szTitle, void* pSTInterface, HINSTANCE hOldRsc, CWnd* pParent /*=NULL*/)
	: CDialog(CDllDlg::IDD, pParent)
{
	//m_szDllName = "HWStrategyAnalysis";
	m_szDllName = szDllName;
	m_szTitle = szTitle;
	m_pSTInterface = pSTInterface;
	m_hOldRsc = hOldRsc;
	m_hTradeInst = NULL;
	m_nOptimizerDiv = 0;

	m_pWndTrade = NULL;

}

CDllDlg::~CDllDlg()
{
}

void CDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDllDlg, CDialog)
	//{{AFX_MSG_MAP(CDlgAnalysis)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_SETDLLSIZE,   OnSetDllSize)
END_MESSAGE_MAP()


// CDllDlg �޽��� ó�����Դϴ�.

void CDllDlg::OnOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialog::OnOK();
}

BOOL CDllDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	AfxSetResourceHandle(m_hOldRsc);

	if(m_szTitle.GetLength()) SetWindowText(m_szTitle);

	BOOL bRet = OpenAUP();


	if(bRet==FALSE) OnCancel();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


// usr/src/AupMain/AupFrm.cpp���� ��ƾ������
//#include "../../../src/Shared/inc/AupAgentEventDef.h"
//#define WMU_GET_AUP_SIZE	WM_USER + 2243			// Aup�� �⺻ Size�� View�� ��û�Ѵ�.
#define WMU_SET_TRADING_INSTANCE WM_USER + 2248		// Tradingȭ�鿡 Dll�� �����Ѵ�.

//#include "../../../src/Shared/inc/ISTitleBar.h"
#define		SIZE_TITLE_BAR			23			    ///> Ÿ��Ʋ�� ����.

BOOL CDllDlg::OpenAUP()
{
	if(!m_pWndTrade)
		m_pWndTrade = LoadTradeAup(m_szDllName, this);

	//CWnd *pTrade = LoadTradeAup(m_szDllName, GetParentFrame());
	if(m_pWndTrade == NULL)
	{
		// Error ó��
		return FALSE;
	}
	else
	{   
		//((CAUPExMainView *)m_pInfoAgent->m_pActiveView)->SetTradingWnd(pTrade);
		CPoint size;
		size.x = 800;
		size.y = 600;
		//pTrade->SendMessage(WMU_GET_AUP_SIZE,(WPARAM)&size,0);
		m_pWndTrade->SendMessage(RMSG_GETDLLSIZE,(WPARAM)&size,0);

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
typedef	CWnd * (CreateTradingAup)(UINT nDldresId, CWnd* pParent);
typedef	CWnd * (CreateTradingAupEx)(UINT nDldresId, CWnd* pParent, void* pInterface);
typedef	BOOL (SA_ApplyChart)(LONG dwChartOcx, CListCtrl* pDivList, LONG dwKey);

CWnd *CDllDlg::LoadTradeAup(CString strFile, CWnd *pParent)
{
	CString strTradingName = strFile + ".dll";

	HINSTANCE hTradeInst = LoadLibrary(strTradingName);
	m_hTradeInst = hTradeInst;

	if(hTradeInst == NULL)
	{
		CString strError;
		strError.Format(_T("%s File Load Faile"),strTradingName);
		MessageBox(strError);
		return NULL;
	}

	CreateTradingAup *pCreateTrading = NULL;
	CreateTradingAupEx *pCreateTradingEx = NULL;
	CWnd *pTrade = NULL;

	if(m_pSTInterface)
	{
		pCreateTradingEx = (CreateTradingAupEx *)GetProcAddress(hTradeInst,_T("CREATE_TRADING_FX"));
		if(pCreateTradingEx == NULL)
		{
			MessageBox("Ưȭȭ�� �ε� ����");
			FreeLibrary(hTradeInst);
			return NULL;
		}

		pTrade = pCreateTradingEx(m_nOptimizerDiv,pParent, m_pSTInterface);
		if(pTrade != NULL)
		{
			pParent->SendMessage(WMU_SET_TRADING_INSTANCE,(WPARAM)hTradeInst,0);
		}
		else
		{
			FreeLibrary(hTradeInst);
		}
	}	
	else
	{
		pCreateTrading = (CreateTradingAup *)GetProcAddress(hTradeInst,_T("CREATE_TRADING"));
		if(pCreateTrading == NULL)
		{
			MessageBox("Ưȭȭ�� �ε� ����");
			FreeLibrary(hTradeInst);
			return NULL;
		}

		pTrade = pCreateTrading(m_nOptimizerDiv,pParent);
		if(pTrade != NULL)
		{
			pParent->SendMessage(WMU_SET_TRADING_INSTANCE,(WPARAM)hTradeInst,0);
		}
		else
		{
			FreeLibrary(hTradeInst);
		}
	}
	//if(pTrade)
	//	pTrade->DestroyWindow();
	return pTrade;						
}

LONG CDllDlg::OnSetDllSize(WPARAM wParam, LPARAM lParam)
{
	CPoint *pSize = (CPoint*)wParam;

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

	int iNewWidth = frameRect.Width() - viewRect.Width() + pSize->x;
	int iNewHeight = frameRect.Height() - viewRect.Height() + pSize->y;

	SetWindowPos(NULL,0,0,iNewWidth,iNewHeight,SWP_NOMOVE);

	return 0L;
}

typedef	void (DeleteTradingAupEx)(CWnd* pWndTrade);
void CDllDlg::OnDestroy()
{
	if(m_hTradeInst && m_pWndTrade) 
	{
		DeleteTradingAupEx *pDeleteTradingEx = NULL;
		pDeleteTradingEx = (DeleteTradingAupEx *)GetProcAddress(m_hTradeInst,_T("DESTROY_TRADING"));
		pDeleteTradingEx(m_pWndTrade);
		//if(m_szDllName=="HWStrategyAnalysis")
		//{
			FreeLibrary(m_hTradeInst);//KHD : dll�� �ȳ������� ���� ���� : 2006.11.17
			m_hTradeInst = NULL;
		//}
		m_pWndTrade = NULL;
	}

	CDialog::OnDestroy();
}
