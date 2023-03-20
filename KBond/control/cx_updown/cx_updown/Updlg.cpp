// Updlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "cx_updown.h"
#include "Updlg.h"
#include "afxdialogex.h"


// CUpdlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUpdlg, CDialogEx)

CUpdlg::CUpdlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpdlg::IDD, pParent)
{
	
}

CUpdlg::~CUpdlg()
{
}

void CUpdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UP, m_uplist);
}


BEGIN_MESSAGE_MAP(CUpdlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_FILE, &CUpdlg::OnBnClickedBtnFile)
	ON_BN_CLICKED(IDC_BTN_UP, &CUpdlg::OnBnClickedBtnUp)
//	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CUpdlg::OnItemdblclickListUp)
ON_NOTIFY(NM_DBLCLK, IDC_LIST_UP, &CUpdlg::OnDblclkListUp)
END_MESSAGE_MAP()


// CUpdlg �޽��� ó�����Դϴ�.


BOOL CUpdlg::OnInitDialog()
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
	lvc.cx       = m_uplist.GetStringWidth(lvc.pszText) + rc.Width();
	lvc.iSubItem = -1;
	m_uplist.InsertColumn(0, &lvc);
	m_uplist.SetExtendedStyle(m_uplist.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);


	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL CUpdlg::addItem(CString items, int item, int subi)
{
	LV_ITEM lvi;

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = item;
	lvi.iSubItem = subi;
	lvi.pszText  = (LPTSTR)items.operator LPCTSTR();

	if (!subi)
		return m_uplist.InsertItem(&lvi);
	return m_uplist.SetItem(&lvi);
}

void CUpdlg::GetListData(CStringArray& parr)
{
	int icnt = m_uplist.GetItemCount();
	for(int ii = 0 ; ii < icnt ; ii++)
	{
		if(m_uplist.GetCheck(ii))
		{
			parr.Add(m_uplist.GetItemText(ii, 0));
		}
	}

}

void CUpdlg::OnBnClickedBtnFile()
{
	CString strpath;
	CFileDialog	dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST, "All Files (*.*)|*.*||");
	if (dlg.DoModal() == IDOK)
	{
		int iItem = m_uplist.GetItemCount();
		strpath = dlg.GetPathName();
		addItem(strpath, iItem, 0);
		m_uplist.SetCheck(iItem);
	}
}


void CUpdlg::OnBnClickedBtnUp()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CUpdlg::OnDblclkListUp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	

	int nItem = -1;
	int nSubItem = -1;
	if(pNMItemActivate)
	{
		nItem = pNMItemActivate->iItem;
		nSubItem = pNMItemActivate->iSubItem;
		m_uplist.DeleteItem(nItem);
		*pResult = 0;
	}
}
