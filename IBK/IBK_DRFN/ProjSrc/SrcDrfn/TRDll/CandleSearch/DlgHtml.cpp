// DlgHtml.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgHtml.h"


// CDlgHtml ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgHtml, CDialog)
CDlgHtml::CDlgHtml(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHtml::IDD, pParent)
{
	m_pHtmlView = NULL;
	m_strURL = _T("");
}

CDlgHtml::~CDlgHtml()
{
	if(m_pHtmlView)
	{
		delete m_pHtmlView;
		m_pHtmlView = NULL;
	}
}
BOOL CDlgHtml::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_pHtmlView = new CHelpHtmlCtrl();

	CRect rect;
	GetDlgItem(IDC_STATIC_TEXT)->GetWindowRect(rect);		
	ScreenToClient(&rect);		
	m_pHtmlView->CreateWnd(rect, this, IDC_STATIC_TEXT);

	ShowHtml();	
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CDlgHtml::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgHtml, CDialog)
END_MESSAGE_MAP()


void CDlgHtml::ShowHtml()
{
	m_pHtmlView->Navigate(m_strURL);
}

// CDlgHtml �޽��� ó�����Դϴ�.
