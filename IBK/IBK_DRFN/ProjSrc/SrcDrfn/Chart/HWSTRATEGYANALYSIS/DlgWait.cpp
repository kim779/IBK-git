// DlgWait.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgWait.h"
#include ".\dlgwait.h"

#include "Resource.h"

#define CRBACKGROUND	RGB(242, 246, 251)

// CDlgWait ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgWait, CDialog)
CDlgWait::CDlgWait(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWait::IDD, pParent)
{
	m_crBk = CRBACKGROUND;
	m_brBkColor.CreateSolidBrush(m_crBk);
}

CDlgWait::~CDlgWait()
{
	m_brBkColor.DeleteObject();
}

void CDlgWait::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STSTITLE, m_ExStaticTitle);
}


BEGIN_MESSAGE_MAP(CDlgWait, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgWait �޽��� ó�����Դϴ�.

HBRUSH CDlgWait::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		pDC->SetBkColor(m_crBk);
		return m_brBkColor;		
		break;
	}

	return hbr;
}
