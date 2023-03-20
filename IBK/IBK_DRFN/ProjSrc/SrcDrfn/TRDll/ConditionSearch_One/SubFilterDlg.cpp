// FilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SubFilterDlg.h"
#include "MainFrmWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubFilterDlg dialog

CSubFilterDlg::CSubFilterDlg(CWnd* pParent, CString strType, 
							 CList <SubFilterInfo, SubFilterInfo&>	*plistSubFilterInfo, CWnd* pMainWnd)
							 : CDialog(CSubFilterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubFilterDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_strType = strType;
	m_pwndParent = pParent;
	m_plistSubFilterInfo = plistSubFilterInfo;
	m_pMainWnd = pMainWnd;
}


void CSubFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubFilterDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSubFilterDlg, CDialog)
//{{AFX_MSG_MAP(CSubFilterDlg)
ON_WM_CREATE()
ON_WM_SIZE()
ON_WM_PAINT()
ON_WM_CTLCOLOR()
ON_WM_DESTROY()
//}}AFX_MSG_MAP

ON_BN_CLICKED(IDC_BTN_MODIFY, OnBtnModify)
ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)

ON_MESSAGE(WM_NOTIFY_GRID, OnNotify_Grid)

END_MESSAGE_MAP()

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CSubFilterDlg message handlers

BOOL CSubFilterDlg::Create(CWnd* pParentWnd, UINT nID) 
{
	return CDialog::Create(IDD, pParentWnd);
}

int CSubFilterDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
		
	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pMainWnd;
	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	m_brushStatic.CreateSolidBrush(COLOR_DLG_CANVAS);
	
	m_btnFilterGroup.Create(m_strType + " ����", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_GROUPIN);
	m_btnFilterGroup.SetFont(&m_fontDefault);
	
	m_gridSet.CreateGrid(WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), this, IDC_GRID_SET);
	
	m_comboTime.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
      CRect(0, 0, 0, 0), this, IDC_COMBO_TIME);
	m_comboTime.SetFont(&m_fontDefault);
	
	m_ctrlTime.Create(_T("����"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlTime.SetFont(&m_fontDefault);
	
	m_editVal.Create(WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), this, IDC_EDIT_VAL);
	m_editVal.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	m_editVal.SetFont(&m_fontDefault);
	
	m_ctrlVal.Create(_T("õ�� �̻�"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlVal.SetFont(&m_fontDefault);
	
	m_btnModify.Create(_T("�߰�/����"), CRect(0, 0, 0, 0), this, IDC_BTN_MODIFY);
	pMainWnd->SetBtnImg(&m_btnModify, 4);
	
	m_btnDelete.Create(_T("����"), CRect(0, 0, 0, 0), this, IDC_BTN_DELETE);
	pMainWnd->SetBtnImg(&m_btnDelete, 4);
	
	m_btnOk.Create(_T("Ȯ��"), CRect(0, 0, 0, 0), this, IDC_BTN_OK);
	pMainWnd->SetBtnImg(&m_btnOk, 4);
	
	m_btnCancel.Create(_T("���"), CRect(0, 0, 0, 0), this, IDC_BTN_CANCEL);
	pMainWnd->SetBtnImg(&m_btnCancel, 4);
	
	m_ctrlInfo.Create(_T("����"), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlInfo.SetFont(&m_fontDefault);
	
	AfxSetResourceHandle(hInstSave);

	return 0;
}

void CSubFilterDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_fontDefault.DeleteObject();
	m_brushStatic.DeleteObject();
}

BOOL CSubFilterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect reClient;
	GetWindowRect(reClient);
	
	reClient.bottom = reClient.top + 250;
	reClient.right = reClient.left + 335;
	MoveWindow(&reClient);
	
	SetInit();
	SetShow(m_strType, m_plistSubFilterInfo);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSubFilterDlg::SetInit()
{
	CString strTime;
	for (int nIndex = 0; nIndex < 6; nIndex++)
	{
		strTime.Format("%2d ��", nIndex + 10);
		m_comboTime.AddString(strTime);
	}
	m_comboTime.SetCurSel(0);
}

void CSubFilterDlg::SetShow(CString strType, CList <SubFilterInfo, SubFilterInfo&>	*plistSubFilterInfo)
{
	if (strType == "VOL")
	{
		m_btnFilterGroup.SetWindowText("�ŷ��� ����");
		m_ctrlVal.SetWindowText("õ�� �̻�");
		m_ctrlInfo.SetWindowText("�˻� ������ �ŷ��� ���� ������ �����մϴ�.");
		
		SetWindowText("�ŷ��� ���� ����");
	}
	else if (strType == "PRICE")
	{
		m_btnFilterGroup.SetWindowText("�ŷ���� ����");
		m_ctrlVal.SetWindowText("�鸸�� �̻�");
		m_ctrlInfo.SetWindowText("�˻� ������ �ŷ���� ���� ������ �����մϴ�.");
		
		SetWindowText("�ŷ���� ���� ����");
	}
	
	m_comboTime.SetCurSel(0);
	m_editVal.SetWindowText("");
	
	//m_plistSubFilterInfo = plistSubFilterInfo;
	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pMainWnd;
	m_gridSet.HeaderSetup(strType, pMainWnd->GetAxColor(74));
	
	POSITION pos;
	SubFilterInfo	stSubFilterInfo;
	for (int nIndex = 0; nIndex < plistSubFilterInfo->GetCount(); nIndex++)
	{
		pos = plistSubFilterInfo->FindIndex(nIndex);
		stSubFilterInfo = plistSubFilterInfo->GetAt(pos);
		
		m_gridSet.SetAddRow(stSubFilterInfo.nTime, stSubFilterInfo.nData, FALSE);
	}
	m_gridSet.RedrawAll();
	
	ShowWindow(SW_SHOW);
}

void CSubFilterDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CRect reClient, reBottom, reLeft;
	GetClientRect(reClient);
	
	reClient.top += 5;
	reClient.bottom -= 5;
	reClient.left += 5;
	reClient.right -= 5;
	m_btnFilterGroup.MoveWindow(&reClient);
	
	reClient.top += 20;
	reClient.bottom -= 5;
	reClient.left += 5;
	reClient.right -= 5;
	
	reBottom = reClient;
	
	reBottom.top = reBottom.bottom - 20;
	m_ctrlInfo.MoveWindow(&reBottom);
	
	reBottom.bottom  = reBottom.top - 5; 
	reBottom.top = reClient.top;
	
	reLeft = reBottom;
	
	reLeft.right = reLeft.left + 180;
	m_gridSet.MoveWindow(&reLeft);
	
	reLeft.left = reLeft.right + 5;
	reLeft.right = reBottom.right;
	OnSize_Set(reLeft);
}

void CSubFilterDlg::OnSize_Set(CRect reClient)
{
	CRect reTop, reTemp, reStatic;
	
	reTop = reClient;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.right = reTemp.left + 55;
	reTemp.bottom += 100;
	m_comboTime.MoveWindow(&reTemp);
	reTemp.bottom -= 100;
	
	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 60;
	
	reStatic = reTemp;
	reStatic.top += 3;
	reStatic.bottom -= 2;
	m_ctrlTime.MoveWindow(&reStatic);
	
	reTop.top = reTop.bottom + 5;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.right = reTemp.left + 55;
	m_editVal.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 3;
	reTemp.right = reClient.right;
	reStatic = reTemp;
	reStatic.top += 3;
	reStatic.bottom -= 2;
	m_ctrlVal.MoveWindow(&reStatic);
	
	reTop.top = reTop.bottom + 70;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.right = reTemp.left + 60;
	m_btnModify.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 60;
	m_btnDelete.MoveWindow(&reTemp);
	
	reTop.top = reTop.bottom + 3;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.right = reTemp.left + 60;
	m_btnOk.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 60;
	m_btnCancel.MoveWindow(&reTemp);
}


void CSubFilterDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pMainWnd;

	CRect reClient;
	GetClientRect(reClient);
	//dc.FillSolidRect(reClient, COLOR_DLG_CANVAS);
	dc.FillSolidRect(reClient, pMainWnd->GetAxColor(66));
}

HBRUSH CSubFilterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pMainWnd;

		pDC->SetBkMode(TRANSPARENT);
		hbr = pMainWnd->GetAxBrush(pMainWnd->GetAxColor(66));
	}
	
	return hbr;
}

void CSubFilterDlg::OnBtnModify()
{
	int nIndex = m_comboTime.GetCurSel();
	
	CString strData;
	m_editVal.GetWindowText(strData);
	
	m_gridSet.SetModifyRow(nIndex + 10, atoi(strData));
}

void CSubFilterDlg::OnBtnDelete()
{
	m_gridSet.SetDeleteRow(m_gridSet.m_nLastSelRow);
}

void CSubFilterDlg::OnBtnOk()
{
	m_plistSubFilterInfo->RemoveAll();
	
	SubFilterInfo	stSubFilterInfo;
	for (int nRow = 0; nRow < m_gridSet.GetNumberRows(); nRow++)
	{
		m_gridSet.GetRowData(nRow, stSubFilterInfo.nTime, stSubFilterInfo.nData);
		m_plistSubFilterInfo->AddTail(stSubFilterInfo);
	}
	
	OnOK();
}

void CSubFilterDlg::OnBtnCancel()
{
	OnCancel();
}

LRESULT CSubFilterDlg::OnNotify_Grid(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NM_ITEM_CLK)
	{
		int nRow = (int)lParam;
		int nData1, nData2;
		m_gridSet.GetRowData(nRow, nData1, nData2);
		
		m_comboTime.SetCurSel(nData1 - 10);
		CString strTemp;
		strTemp.Format("%d", nData2);
		m_editVal.SetWindowText(strTemp);
	}
	
	return TRUE;
}
