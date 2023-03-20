// NotToggleButton.cpp : implementation file
//

#include "stdafx.h"
#include "NotToggleButton.h"
#include "ToolBarDlg.h"
#include "Definition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNotToggleButton

CNotToggleButton::CNotToggleButton()
{
	// (2006/11/16 - Seung-Won, Bae) Support Auto Repeat!
	m_nTimerID = 0;
}

CNotToggleButton::~CNotToggleButton()
{
}


BEGIN_MESSAGE_MAP(CNotToggleButton, CButtonEx)
	//{{AFX_MSG_MAP(CNotToggleButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotToggleButton message handlers


/*-----------------------------------------------------------------------------------------
 - Function    :  OnLButtonDown
 - Created at  :  2004-11-09   16:07
 - Author      :  ������
 - Description :  LButtonDown�� ���� �������ִ� ��ư�� ������ ��Ŵ.
 -----------------------------------------------------------------------------------------*/
void CNotToggleButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// ��ư ���� �̹��� �׸�.
	m_bState = true;
	m_pParent->ReleaseOtherButtonUI(m_enumToolOption, m_strParam);

	// (2006/11/16 - Seung-Won, Bae) Support Auto Repeat!
	if( m_nTimerID) KillTimer( m_nTimerID);
	m_nTimerID = 1;
	SetTimer( m_nTimerID, 1000, NULL);

	CButtonEx::OnLButtonDown(nFlags, point);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnLButtonUp
 - Created at  :  2004-11-09   16:07
 - Author      :  ������
 - Description :  �ش������� ����� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CNotToggleButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// (2006/11/25 - Seung-Won, Bae) Call Base for Do Modal.
	CButtonEx::OnLButtonUp(nFlags, point);
	
	// ��ư �� ���� ���� �̹��� �׸���.(�θ�Ŭ���� DrawItem����)
	m_bState = false;
	Invalidate();

	// (2006/11/16 - Seung-Won, Bae) Support Auto Repeat!
	if( 0 < m_nTimerID)
	{
		KillTimer( m_nTimerID);
		if( m_nTimerID == 1) m_pParent->SetToolFunction(m_enumToolOption, this);
		m_nTimerID = 0;
	}
}



/*-----------------------------------------------------------------------------------------
 - Function    :  OnMouseMove
 - Created at  :  2004-11-09   16:25
 - Author      :  ������
 - Description :  ���콺 �����϶�... ������ �ִ� CButtonExŬ������ MOUSEMOVE�ڵ鷯 ����ȣ��.
 -----------------------------------------------------------------------------------------*/
void CNotToggleButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CButtonEx::OnMouseMove(nFlags, point);
}

// (2006/11/16 - Seung-Won, Bae) Support Auto Repeat!
void CNotToggleButton::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	// (2006/11/16 - Seung-Won, Bae) Support Auto Repeat!
	if( nIDEvent == 1)
	{
		KillTimer( nIDEvent);

		// (2006/11/16 - Seung-Won, Bae) Support Auto Repeat!
		m_nTimerID = 2;
		SetTimer( m_nTimerID, 200, NULL);
	}
	m_pParent->SetToolFunction( m_enumToolOption, this);
	
	CButtonEx::OnTimer(nIDEvent);
}
