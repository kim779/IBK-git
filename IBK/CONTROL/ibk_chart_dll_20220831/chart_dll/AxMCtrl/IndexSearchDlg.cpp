// IndexSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axmctrl.h"
#include "IndexSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexSearchDlg dialog


CIndexSearchDlg::CIndexSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIndexSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndexSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// �θ� Ʈ���� �����͸� �����Ѵ�.
	m_pParentTree = (CAxMTreeCtrl*)pParent;
}


void CIndexSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndexSearchDlg)
	DDX_Control(pDX, IDC_LIST_RESULT, m_ResultList);
	DDX_Control(pDX, IDC_EDIT_INDEX, m_IndexEdit);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_SearchButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndexSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CIndexSearchDlg)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RESULT, OnDblclkListResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexSearchDlg message handlers

void CIndexSearchDlg::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	// "ã��" ��ư�� ������ Ʈ������ �˻��ϰ��� �ϴ� ���ڿ��� ������
	// ��ǥ ����� �޾ƿ´�.
	SearchIndexName();
}


void CIndexSearchDlg::SearchIndexName() 
{
	// Ʈ������ �˻��ϰ��� �ϴ� ���ڿ��� ������
	// ��ǥ ����� �޾ƿ´�.
	CString			 strIndexName;

	m_IndexEdit.GetWindowText(strIndexName);
	m_TextMapItemList.RemoveAll();
	m_pParentTree->GetTreeItemTextList(&m_TextMapItemList, strIndexName);

	// ��ǥ ����� ������ ����Ʈ ��Ʈ�ѿ� ����ϴ� �۾��� �����Ѵ�.	
	// �켱 ����Ʈ ��Ʈ�Ѹ� �ʱ�ȭ�Ѵ�.
	m_ResultList.DeleteAllItems();

	// ����Ʈ ��Ʈ�ѿ� �������� �߰��Ѵ�.
	POSITION	position;
	CString		strFoundIndex;
	HTREEITEM	hItem;
	
	position = m_TextMapItemList.GetStartPosition();
	while (NULL != position)
	{
		m_TextMapItemList.GetNextAssoc(position, strFoundIndex, (void *&)hItem);
		m_ResultList.InsertItem(0, (LPCSTR)strFoundIndex);
	}
}



BOOL CIndexSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// ����Ʈ ��Ʈ�ѿ� �÷��� �߰��Ѵ�.
	m_ResultList.InsertColumn(0, "�˻��� ��ǥ", LVCFMT_CENTER, 200);

	m_TextMapItemList.RemoveAll();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///void CIndexSearchDlg::OnItemdblclickListResult(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
//	// TODO: Add your control notification handler code here
//	AfxMessageBox("OK");
//	
//	*pResult = 0;
//}

void CIndexSearchDlg::OnDblclkListResult(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//AfxMessageBox("OK");
	NM_LISTVIEW *pNMListVIew = (NM_LISTVIEW*)pNMHDR;
	int			nSelected = pNMListVIew->iItem;
	CString		strSelectedIndex;
	HTREEITEM	hItem;

	// ���õ� �������� �ؽ�Ʈ�� �˾Ƴ���.
	if (nSelected != -1)
	{
		strSelectedIndex = m_ResultList.GetItemText(nSelected, 0);
		m_TextMapItemList.Lookup((LPCSTR)strSelectedIndex, (void *&)hItem);

		// �θ� Ʈ���� ������ �ڵ��� �Ѱ� Ʈ������ �ش� ��ǥ�� select�Ѵ�.
		m_pParentTree->SelectTreeItem(hItem);

		// ���� �� �ִ� �� ���̾�α� �ڽ��� ���ش�.
		EndDialog(IDOK);
	}
	
	*pResult = 0;
}

void CIndexSearchDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CIndexSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	// ����Ű �Է½� ���̾�α� �ڽ��� �������� �ʵ��� �Ѵ�.
	//CDialog::OnOK();
}

BOOL CIndexSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	// ����Ʈ �ڽ��� ����Ű�� ġ�� ��ǥ �˻��ϵ��� �Ѵ�.
	if (m_IndexEdit.GetSafeHwnd() == pMsg->hwnd)
	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		{
			SearchIndexName();
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
