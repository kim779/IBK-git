// DlgSTExOption.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgSTExOption.h"
#include "TabSystemTradingEx.h"
#include "StdDialog.h"

// CDlgSTExOption ��ȭ �����Դϴ�.

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// IMPLEMENT_DYNAMIC(CDlgSTExOption, CDialog)
CDlgSTExOption::CDlgSTExOption(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSTExOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSTExOption)
	//}}AFX_DATA_INIT

	m_pWndTrader = NULL;
	m_hTradeInst = NULL;
	m_bIsApplyBtn = FALSE;
}

CDlgSTExOption::~CDlgSTExOption()
{
}

void CDlgSTExOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSTExOption)
	DDX_Control(pDX, IDC_COMBO_REP, m_CtlComboRep);
	DDX_Control(pDX, IDC_COMBO_DIV, m_CtlComboDiv);
	DDX_Control(pDX, IDC_LIST_DAY, m_CtlList);
	DDX_Control(pDX, IDC_STAPPLY, m_BtnSTApply);
	DDX_Control(pDX, IDC_STSHOWOPTION, m_BtnOption);
	DDX_Control(pDX, IDC_STREPORT, m_BtnSTReport);
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CDlgSTExOption, CDialog)
	//{{AFX_MSG_MAP(CDlgSTExOption)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_STREPORT, OnBnClickedStreport)
	ON_BN_CLICKED(IDC_STSHOWOPTION, OnBnClickedStshowoption)
	ON_COMMAND(IDC_STAPPLY, OnBnClickedStapply)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgSTExOption �޽��� ó�����Դϴ�.

BOOL CDlgSTExOption::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitColumn(&m_CtlList);

	m_CtlComboDiv.SetCurSel(0);
	m_CtlComboRep.SetCurSel(0);

	CTabSystemTradingEx* pParentDlg = (CTabSystemTradingEx*)GetParent();

	m_BtnSTApply.FP_vSetImage( pParentDlg->m_pIL_W58H20, 0, 2, TRUE, "����", FALSE);//KHD : 2006.11.16
	m_BtnSTApply.FP_vSetToolTipText("����");
	//m_BtnOption.FP_vSetImage( pParentDlg->m_pIL_W20H20, 44, 46);KHD :��� ����

	m_BtnSTReport.FP_vSetImage(pParentDlg->m_pIL_W58H20, 0, 2, TRUE, "����", FALSE);
	m_BtnSTReport.FP_vSetToolTipText("����");

	m_CtlList.GetWindowRect(m_rcList);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


// ���Ҽ�
int CDlgSTExOption::GetDivCount()
{
	int nTmp = 0;
	int nSel = m_CtlComboDiv.GetCurSel();
	if(nSel<0) return nTmp;


	CString szTmp;
	m_CtlComboDiv.GetLBText(nSel, szTmp);

	nTmp = atoi(szTmp);
	return nTmp;
}

// ��ø��
double CDlgSTExOption::GetRepRate()
{
	int nSel = m_CtlComboRep.GetCurSel();
	if(nSel<0) return 0.0f;

	CString szTmp;
	m_CtlComboRep.GetLBText(nSel, szTmp);

	double rTmp = 0.00f;
	rTmp = atof(szTmp);
	return rTmp;
}

void CDlgSTExOption::OnBnClickedStshowoption()
{
	CTabSystemTradingEx* pParentDlg = (CTabSystemTradingEx*)GetParent();
	pParentDlg->HideStOptimaA();
}


#include ".\dlldlg.h"
#include ".\dlgstexoption.h"

// �����м� �ε�...
void CDlgSTExOption::OpenRefort()
{
	OnBnClickedStreport();
}
void CDlgSTExOption::OnBnClickedStreport()
{
	CTabSystemTradingEx* pParentDlg = (CTabSystemTradingEx*)GetParent();

	if(pParentDlg->m_xSTAnalysisInput.m_Info.szSTRData.CompareNoCase("�������ǥ")==0 ||
		pParentDlg->m_xSTAnalysisInput.m_Info.szSTRData.Find("�����༼")>=0)
	{
		AfxMessageBox("�������ǥ,����ڰ����༼�� �����м��� �Ұ��մϴ�.");
		return;
	}

	if(pParentDlg->m_xSTAnalysisInput.GetCount()==0)
	{
		AfxMessageBox("������ȸ �� �����մϴ�.");
		return;
	}

	if(pParentDlg->CanSTModule()==FALSE)	return;

	BOOL bSel = pParentDlg->IsSTSelected();
	if(!bSel)
	{
		AfxMessageBox("������ ������ �������ֽñ� �ٶ��ϴ�");
		return;
	}

	// ��������������� TeeChart�� MSChart����� Ȯ���Ѵ�.
	if(!pParentDlg->CheckOcx())
	{
		AfxMessageBox("����ȭ���� �ʿ��� OCX(TeeChart7.ocx, MSChrt20.ocx)����� �ȵǾ��ֽ��ϴ�.");
		return;
	}

	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	CString szDllName;
	szDllName.Format("%s%s", pParentDlg->m_szRootDir, "\\dev\\HWStrategyAnalysis");	// DLL�� �ִ´�.
	CDllDlg dlg(szDllName, "��������", &pParentDlg->m_xSTInterface, hOld,this);	
	dlg.DoModal();
	
	AfxSetResourceHandle(hOld);
}


// ---------------------------------------------------------------------------------
// void CDlgSTExOption::OnBnClickedStapply()
// �����м��� �ε��ϰ� �����м����� ��Ʈ�� ������ �����ϵ��� ȣ���մϴ�.
// Start : 06.07.28 alzio
// ---------------------------------------------------------------------------------
// �����м��� �ִ� ��ƾ�� ���ϴ� ��ƾ�̴�. ��������â���� ���������� ���� �� �Ҹ��� �κ��̴�.
// �� �κ��� ���ϴ� ��ƾ�� ChartSTIndexAddin���� ������ �����ϴ� ��ƾ���� �����ϸ鼭
// ������ ������� �ʴ´�. 06.09.25
typedef	CWnd* (FPSA_ApplyChart)(LONG dwChartOcx, CListCtrl* pDivList, LONG dwInfo, LONG dwReserved1, LONG dwReserved2);
void CDlgSTExOption::OnBnClickedStapply()
{
	CTabSystemTradingEx* pParentDlg = (CTabSystemTradingEx*)GetParent();
	if(pParentDlg->m_xSTAnalysisInput.m_Info.szSTRData.CompareNoCase("�������ǥ")==0 ||
		pParentDlg->m_xSTAnalysisInput.m_Info.szSTRData.Find("�����༼")>=0)
	{
		AfxMessageBox("�������ǥ,����ڰ����༼�� �����м��� �Ұ��մϴ�.");
		return;
	}

	if(pParentDlg->m_xSTAnalysisInput.GetCount()==0)
	{
		AfxMessageBox("������ȸ �� �����մϴ�.");
		return;
	}

	if(pParentDlg->CanSTModule()==FALSE) return;

	m_bIsApplyBtn = TRUE;
	LPCSTR strTradingName = "HWStrategyAnalysis.dll";
	CString szDllName;
	szDllName.Format("%s%s", pParentDlg->m_szRootDir, "\\Dev\\HWStrategyAnalysis");	// DLL�� �ִ´�.

	if(m_hTradeInst!=NULL)
	{
		FreeLibrary(m_hTradeInst);
		m_hTradeInst = NULL;
	}
	m_hTradeInst = LoadLibrary(szDllName);
	if(m_hTradeInst == NULL)
	{
		#ifdef _DEBUG
			CString strError;
			strError.Format(_T("%s File Load Faile"),strTradingName);
			MessageBox(strError);
		#endif
		
	}

	FPSA_ApplyChart* pFunc = (FPSA_ApplyChart*)GetProcAddress(m_hTradeInst,_T("SA_ApplyChart_FX"));
	if(pFunc == NULL)
	{
		#ifdef _DEBUG
			CString strError;
			strError.Format(_T("%s[%s] Load Error!"), strTradingName, "SA_ApplyChart");
			MessageBox(strError);
		#endif
		return ;
	}

	LONG pChartOcx = NULL;
	LONG dwReserved1 = 0, dwReserved2 = 0;

	CChartItem* pCItem = ((CChartMng*)pParentDlg->m_pChartMng)->GetpChartItemSelected();
	pChartOcx = (LONG)(pCItem);

	//if(!m_pWndTrader) :KHD : ��ø�� ����� �� �� �ۿ� ���� �ȵǼ� �ּ�ó��..
		m_pWndTrader = pFunc(pChartOcx, &m_CtlList, (LONG)&pParentDlg->m_xSTAnalysisInput.m_Info, (LONG)&pParentDlg->m_xSTInterface, dwReserved2);
//	FreeLibrary(hTradeInst);
}


//Dlg ��ü�ʱ�ȭ : by KHD : 2007.01.26
void CDlgSTExOption::SetInitDlg()
{
	m_bIsApplyBtn = FALSE;
	m_CtlList.DeleteAllItems();
	m_CtlComboDiv.SetCurSel(0);
	m_CtlComboRep.SetCurSel(0);
}


// ---------------------------------------------------------------------------------
// void CDlgSTExOption::InitColumn(CListCtrl* pListCtrl)
// ����Ʈ��Ʈ���� �ʱ�ȭ�ϴ� ��ƾ�Դϴ�. OnInitDialog���� ȣ���մϴ�.
// Start : 06.07.28 alzio
// ---------------------------------------------------------------------------------
void CDlgSTExOption::InitColumn(CXListCtrl* pListCtrl)
{
	int colwidthsType0[3] = { 14, 70, 70};	// KHD : ��Ų���� sixty-fourths

	TCHAR *	lpszHeadersType0[] = { _T(" "), _T("����"), _T("��"),NULL };
	pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT);

	int total_cx = 0;
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	// add columns
	for (int i = 0; ; i++)
	{
		if (lpszHeadersType0[i] == NULL)
			break;
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = lpszHeadersType0[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = colwidthsType0[i];
		total_cx += lvcolumn.cx;
		pListCtrl->InsertColumn(i, &lvcolumn);
	}
}

HBRUSH CDlgSTExOption::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
	default:
		{
			CTabSystemTradingEx* pParentDlg = (CTabSystemTradingEx*)GetParent();
			pDC->SetBkColor(pParentDlg->GetBkColor());
			return pParentDlg->GetBkColorBrush();
		}		
		break;
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgSTExOption::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_CtlList.GetSafeHwnd())
	{		
		m_CtlList.SetWindowPos(NULL, 0, 0, cx, m_rcList.Height(), SWP_NOACTIVATE|SWP_NOMOVE);
	}
}

typedef	void (FPSA_Delete)(CWnd *pWnd);
void CDlgSTExOption::OnDestroy()
{
	CDialog::OnDestroy();

	if(m_hTradeInst!=NULL)
	{
		if(m_pWndTrader)
		{
			FPSA_Delete* pDeleteFunc = (FPSA_Delete*)GetProcAddress(m_hTradeInst,_T("DESTROY_TRADING"));
			pDeleteFunc(m_pWndTrader);
		}
		FreeLibrary(m_hTradeInst);
	}
	//if(m_pWndTrader)
	//{
	//	LPCSTR strTradingName = "HWStrategyAnalysis.dll";

	//	HINSTANCE hTradeInst = LoadLibrary(strTradingName);

	//	FPSA_Delete* pDeleteFunc = (FPSA_Delete*)GetProcAddress(hTradeInst,_T("DESTROY_TRADING"));
	//	pDeleteFunc(m_pWndTrader);
	//	FreeLibrary(hTradeInst);
	//}
	m_CtlList.DeleteAllItems();
}
