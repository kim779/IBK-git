// Downdlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "cx_updown.h"
#include "Downdlg.h"
#include "afxdialogex.h"


// CDowndlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDowndlg, CDialogEx)

CDowndlg::CDowndlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDowndlg::IDD, pParent)
{

}

CDowndlg::~CDowndlg()
{
}

void CDowndlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DOWN, m_Dnlist);
}


BEGIN_MESSAGE_MAP(CDowndlg, CDialogEx)
END_MESSAGE_MAP()


// CDowndlg �޽��� ó�����Դϴ�.


BOOL CDowndlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CRect rc;
	GetClientRect(&rc);
	CString columns;
	columns = _T("File");

	LV_COLUMN lvc;

	lvc.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt      = LVCFMT_LEFT;
	lvc.pszText  = (LPTSTR)columns.operator LPCTSTR();
	lvc.cx       = m_Dnlist.GetStringWidth(lvc.pszText) + rc.Width();
	lvc.iSubItem = -1;
	m_Dnlist.InsertColumn(0, &lvc);
	m_Dnlist.SetExtendedStyle(m_Dnlist.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);


	//test
	addItem(_T("axisinfo"), 0, 0);
	addItem(_T("axismenu"), 1, 0);
	addItem(_T("axismenu"), 1, 0);
	addItem(_T("axismenu"), 1, 0);


	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

BOOL CDowndlg::addItem(CString items, int item, int subi)
{
	LV_ITEM lvi;

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = item;
	lvi.iSubItem = subi;
	lvi.pszText  = (LPTSTR)items.operator LPCTSTR();

	if (!subi)
		return m_Dnlist.InsertItem(&lvi);

	return m_Dnlist.SetItem(&lvi);
}