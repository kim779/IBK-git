// TriDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CX_POP.h"
#include "TriDlg.h"
#include "afxdialogex.h"


// CTriDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTriDlg, CDialogEx)

CTriDlg::CTriDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTriDlg::IDD, pParent)
{

}

CTriDlg::~CTriDlg()
{
}

void CTriDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTriDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CTriDlg �޽��� ó�����Դϴ�.


void CTriDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(rc, RGB(249,249,249));
	
	CPen	pen;
	pen.CreatePen(PS_SOLID, 1, RGB(183,187,209));
	CPen	*pOldPen = dc.SelectObject(&pen);

	dc.MoveTo(rc.left, rc.bottom);
	dc.LineTo(rc.right/2, rc.top);

	dc.MoveTo(rc.right/2, rc.top);
	dc.LineTo(rc.right, rc.bottom);

	dc.SelectObject(pOldPen);
}


BOOL CTriDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	/*
	LONG style = GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE);
	style |= WS_EX_LAYERED;
	int iresult = SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, style);
	::SetLayeredWindowAttributes(GetSafeHwnd(), 0,128, LWA_ALPHA);
	*/
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
