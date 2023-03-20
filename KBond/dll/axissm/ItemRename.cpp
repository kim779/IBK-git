// ItemRename.cpp : implementation file
//

#include "stdafx.h"
#include "ItemRename.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemRename dialog


CItemRename::CItemRename(CWnd* pParent /*=NULL*/)
	: CDialog(CItemRename::IDD, pParent)
{
	//{{AFX_DATA_INIT(CItemRename)
	m_edit = _T("");
	//}}AFX_DATA_INIT
}


void CItemRename::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemRename)
	DDX_Text(pDX, IDC_ITEMNAME, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemRename, CDialog)
	//{{AFX_MSG_MAP(CItemRename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemRename message handlers

BOOL CItemRename::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	((CEdit *)GetDlgItem(IDC_ITEMNAME))->SetFocus();
	((CEdit *)GetDlgItem(IDC_ITEMNAME))->SetSel(0, -1, FALSE);
	((CEdit *)GetDlgItem(IDC_ITEMNAME))->SetLimitText(8);
	
	return TRUE;
}

void CItemRename::SetName(CString name)
{
	m_edit = name;
}

void CItemRename::OnOK() 
{
	UpdateData();

	if (m_edit.IsEmpty())
	{
		::MessageBox(m_hWnd, "���̸��� �Է��Ͻʽÿ�.", "Ȯ��", MB_ICONSTOP);
		return;
	}
	
	CDialog::OnOK();
}
