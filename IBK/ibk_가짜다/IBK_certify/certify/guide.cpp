// guide.cpp : implementation file
//

#include "stdafx.h"
#include "Certify.h"
#include "guide.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuide dialog

CGuide::CGuide(int type, CWnd* pParent /*=NULL*/)
	: CDialog(CGuide::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGuide)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_type = type;
}


void CGuide::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGuide)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGuide, CDialog)
	//{{AFX_MSG_MAP(CGuide)
	ON_BN_CLICKED(IDC_ISSUE, OnIssue)
	ON_BN_CLICKED(IDC_OTHER, OnOther)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuide message handlers

BOOL CGuide::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	switch (m_type)
	{
	case typeSUSPEND:
		GetDlgItem(IDC_TYPE)->SetWindowText("����Ͻ� �������� ������ �������Դϴ�.");			break;
	case typeEXPIRE:
		GetDlgItem(IDC_TYPE)->SetWindowText("����Ͻ� �������� ��ȿ�Ⱓ�� ����� �������Դϴ�.");	break;
	case typeREVOKE:
	default:
		GetDlgItem(IDC_TYPE)->SetWindowText("����Ͻ� �������� ������ �������Դϴ�.");			break;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGuide::OnIssue() 
{
	EndDialog(IDC_ISSUE);	
}

void CGuide::OnOther() 
{
	EndDialog(IDC_OTHER);	
}
