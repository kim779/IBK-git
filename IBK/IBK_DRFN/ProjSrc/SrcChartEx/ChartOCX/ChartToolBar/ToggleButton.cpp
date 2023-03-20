// ToggleButton.cpp : implementation file
//

#include "stdafx.h"
#include "ToggleButton.h"
#include "ToolBarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToggleButton

CToggleButton::CToggleButton()
{
}

CToggleButton::~CToggleButton()
{
}


BEGIN_MESSAGE_MAP(CToggleButton, CButtonEx)
	//{{AFX_MSG_MAP(CToggleButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToggleButton message handlers


/*-----------------------------------------------------------------------------------------
 - Function    :  OnLButtonDown
 - Created at  :  2004-11-09   16:02
 - Author      :  ������
 - Description :  ��ư�� ������ �ٸ� ��ư�� ������ ��Ų��.
 -----------------------------------------------------------------------------------------*/
void CToggleButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// ��ư�� �������� �ٸ� ������ ����ư�� Ǭ��.
	if(!m_bState)
		m_pParent->ReleaseOtherButtonUI(m_enumToolOption, m_strParam);

	// ��ư ���� ����. (�� ��������� ���� ��ư���� �̹����� �׷�����. �θ�Ŭ���� DrawItem����)
	m_bState = !m_bState;

	// (2009/10/9 - Seung-Won, Bae) Do not action on ButtonUp. It has some delay.
	if(m_bState)	// ����۵� ���� -> ��۵� ����
		m_pParent->SetToolFunction(m_enumToolOption, this); // �ش� ���� ����� �����Ѵ�.
	else	// ��۵� ���� -> ����۵� ����		
		m_pParent->ReleaseToolFunction(m_enumToolOption, this);// �ش� ���� ��� ������ ��ҽ�Ų��.

	Invalidate();

	CButtonEx::OnLButtonDown(nFlags, point);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  OnMouseMove
 - Created at  :  2004-11-09   16:25
 - Author      :  ������
 - Description :  ���콺 �����϶�... ������ �ִ� CButtonExŬ������ MOUSEMOVE�ڵ鷯 ����ȣ��.
 -----------------------------------------------------------------------------------------*/
void CToggleButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CButtonEx::OnMouseMove(nFlags, point);
}
