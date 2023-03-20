// DlgModaless.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "axis.h"
#include "DlgModaless.h"
#include "afxdialogex.h"
#define WM_BTNMSG WM_USER + 888
// CDlgModaless ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgModaless, CDialogEx)

CDlgModaless::CDlgModaless(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgModaless::IDD, pParent)
{
	m_pfont = NULL;
}

CDlgModaless::~CDlgModaless()
{
	if(m_pfont != NULL)
		delete m_pfont;
}

void CDlgModaless::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHK_PROGRAM, m_ckProgram);
	DDX_Control(pDX, IDC_CHK_SDI, m_ckSDI);
	DDX_Control(pDX, IDC_BTN_OK, m_btnok);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btncancel);
}


BEGIN_MESSAGE_MAP(CDlgModaless, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDOK, &CDlgModaless::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgModaless::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_OK, &CDlgModaless::OnBnClickedBtnOk)
	ON_MESSAGE(WM_BTNMSG, OnMessage)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgModaless �޽��� ó�����Դϴ�.

LRESULT CDlgModaless::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case 98:
			OnBnClickedCancel();
			break;
		case 99:
			OnBnClickedOk();
			break;
	}
	return 0;
}

BOOL CDlgModaless::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CPoint	point;		
	GetCursorPos(&point);
	CRect rcDlg;
	GetClientRect(&rcDlg);

	MoveWindow(point.x, point.y, rcDlg.Width(), rcDlg.Height());

	m_pBrush = new CBrush();
	m_pBrush->CreateSolidBrush(RGB(42,46,53));

	m_ckProgram.SetBitmaps(IDB_CHECK_ON, RGB(255, 0, 255), IDB_CHECK_OFF, RGB(255, 0, 255));
	m_ckProgram.DrawTransparent();

	m_ckSDI.SetBitmaps(IDB_CHECK_ON, RGB(255, 0, 255), IDB_CHECK_OFF, RGB(255, 0, 255));
	m_ckSDI.DrawTransparent();

	
	for (int ii = 0; ii <= 5; ii++)
	{
		m_ckProgram.SetColor(ii, RGB(0, 0, 0));
		m_ckSDI.SetColor(ii, RGB(0, 0, 0));
	}

	CString	fontName;
	fontName = _T("����");
	m_pfont = new CFont;
	m_pfont->CreateFont(	12,            // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,	           // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				DEFAULT_CHARSET,           // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				fontName); 

	CString	path, path_en, path_dn, imgName; 
	path.Format("%s/%s/loginexBTN.BMP",     m_home, "image");
	path_dn.Format("%s/%s/loginexBTN_DN.BMP",   m_home, "image");
	path_en.Format("%s/%s/loginexBTN.BMP", m_home, "image");

	m_btnok.set_Image(path,path_dn, path_en);
	m_btnok.SetItemFont(m_pfont);
	m_btnok.m_fcolor = RGB(255, 255, 255);
	m_btnok.m_text = _T("Ȯ��");
	m_btnok.m_id = 99;

	m_btncancel.set_Image(path ,path_dn, path_en);
	m_btncancel.m_fcolor = RGB(255, 255, 255);
	m_btncancel.SetItemFont(m_pfont);
	m_btncancel.m_text = _T("���");
	m_btncancel.m_id = 98;
	
	CWinApp* app = AfxGetApp();
	bool bsdi = app->GetProfileInt("Information", "SDI", 0);

	if(bsdi)
		((CButton*)GetDlgItem(IDC_CHK_SDI))->SetCheck(true);
	else
		((CButton*)GetDlgItem(IDC_CHK_SDI))->SetCheck(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL CDlgModaless::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(rc, RGB(42,46,53));
	return true;
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CDlgModaless::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//IDC_CHK_PROGRAM
	//IDC_CHK_SDI

	if(((CButton*)GetDlgItem(IDC_CHK_PROGRAM))->GetCheck())
		m_arr.Add("1");
	else
		m_arr.Add("0");


	m_arr.Add("0");
	/*
	if(((CButton*)GetDlgItem(IDC_CHK_SDI))->GetCheck())
		m_arr.Add("1");
	else
		m_arr.Add("0");
	*/

	CDialogEx::OnOK();
}

void CDlgModaless::GetResultArr(CStringArray& arr)
{
	if(m_arr.GetCount() > 0)
		arr.Copy(m_arr);
	else
		arr.RemoveAll();
}

void CDlgModaless::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_arr.RemoveAll();
	CDialogEx::OnCancel();
}


void CDlgModaless::OnBnClickedBtnOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


HBRUSH CDlgModaless::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_STATIC_PRO:
		case IDC_STATIC_SDI:
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(159, 188, 236));
				hbr = m_pBrush->operator HBRUSH();
			}
			break;	
	}
	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}
