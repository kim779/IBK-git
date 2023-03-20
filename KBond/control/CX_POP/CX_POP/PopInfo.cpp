// PopInfo.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CX_POP.h"
#include "PopInfo.h"
#include "afxdialogex.h"


// CPopInfo ��ȭ �����Դϴ�.

#define	TM_SLIDE	100
#define	TMI_SLIDE	100
#define	SLIDEGAP	10
#define	GAPX		4

IMPLEMENT_DYNAMIC(CPopInfo, CDialogEx)

CPopInfo::CPopInfo(CString home,CWnd* pParent /*=NULL*/)
	: CDialogEx(CPopInfo::IDD, pParent)
{
	m_home  = home;
	m_table = NULL;
	m_pwnd = pParent;
}

CPopInfo::~CPopInfo()
{
}

void CPopInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPopInfo, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CPopInfo::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPopInfo::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPopInfo �޽��� ó�����Դϴ�.

void CPopInfo::Init()
{
	int	x, y, gapX = 0, gapY = 0, GAP = 10, rowN = 2;

	if (!m_table)	
	{
		m_table = new CMTable(m_home, this, m_pwnd);

		if (!m_table->Create(NULL, "����", 
			WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, (int) m_table))
		{
			delete m_table;
			m_table = NULL;
			return;
		}
	}

	
	if(m_table->m_data.IsEmpty())
		m_table->SetTitle("���̻�");

	CRect	wRc, cRc, bRc;
	m_table->GetWindowRect(wRc);
	m_pwnd->GetWindowRect(cRc);

	gapX += wRc.Width();
	gapY += wRc.Height();

	x = GetSystemMetrics(SM_CXSCREEN) - gapX - GAPX;
	y = GetSystemMetrics(SM_CYSCREEN) - gapY - GAPX;

	m_width  = gapX;
	m_height = gapY;
	SetWindowPos(NULL, cRc.left, cRc.bottom, m_height, m_height, SWP_HIDEWINDOW);
}

void CPopInfo::ChangeData()
{
	if (m_table)	
		m_table->ChangeData();
}

void CPopInfo::ShowSlide(CRect baseRC)
{
	CRect rc;
	
	m_pwnd->GetWindowRect(&rc);

	int iposition = m_pwnd->SendMessage(WM_USER + 999, 999, 0);
	switch(iposition)
	{
		case 0:  //left
			SetWindowPos(&wndTop, rc.left, rc.bottom + 10, m_width, m_height, SWP_NOACTIVATE|SWP_SHOWWINDOW);
			break;
		case 1:
			rc.OffsetRect(-(m_width/2) , 0);
			SetWindowPos(&wndTop, rc.left, rc.bottom + 10, m_width, m_height, SWP_NOACTIVATE|SWP_SHOWWINDOW);
			break;
		case 2:
			SetWindowPos(&wndTop, rc.left - m_width + rc.Width(), rc.bottom + 10, m_width, m_height, SWP_NOACTIVATE|SWP_SHOWWINDOW);
			break;

	}
	
	
	//MoveWindow(rc.left, rc.bottom + 1, rc.left + m_width + 10, rc.bottom + 1 + m_height);
}

void CPopInfo::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_table)
	{
		if (m_table->m_hWnd && ::IsWindow(m_table->m_hWnd))
			m_table->DestroyWindow();
		delete m_table;
	}
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CPopInfo::HideSlide()
{
	SetTimer(TM_SLIDE, TMI_SLIDE, NULL);
	//ShowWindow(SW_HIDE);
}

void CPopInfo::SetData(CString dat, int kind, int pos)
{
	int	gapX = 0, gapY = 0, GAP = 10, rowN = 2;
	m_data = dat;
	m_table->SetTitle((LPSTR)(LPCTSTR)m_data);

	CRect	wRc, cRc, bRc;
	m_table->GetWindowRect(wRc);
	m_pwnd->GetWindowRect(cRc);

	gapX += wRc.Width();
	gapY += wRc.Height();
	m_width = gapX;
	m_height = gapY;
	SetWindowPos(NULL, cRc.left, cRc.bottom + 1, m_width, m_height, SWP_HIDEWINDOW);
}

void CPopInfo::Slide()
{
	
}

void CPopInfo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch (nIDEvent)
	{
		case TM_SLIDE:	
		
			break;
		default:		
			break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CPopInfo::StopSlide()
{
	
}

void CPopInfo::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	CDialogEx::OnOK();
}


void CPopInfo::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnCancel();
}


BOOL CPopInfo::OnInitDialog()
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
