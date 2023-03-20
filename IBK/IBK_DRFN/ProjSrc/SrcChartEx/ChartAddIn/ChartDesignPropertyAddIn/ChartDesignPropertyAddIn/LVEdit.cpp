// LVEdit.cpp : implementation file
//

#include "stdafx.h"
#include "LVEdit.h"

#include "EditableListCtrl.h"	// for CEditableListCtrl

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLVEdit

CLVEdit::CLVEdit()
{

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.08
//
//	����	:	Ctrl Key�� ���� ���¸� ��Ÿ���� Flag�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
	m_bCtrl = FALSE;

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.02.21
//
//	����	:	Enter�� �̿��Ͽ��߸� Update�� �������� ���θ� ��Ÿ���� Flag�� �����Ѵ�.
//
//	����	:	1. �⺻������ Flase�̸�, Enter�ܿ��� �ܼ��� Kill Focus �������� Update�ȴ�.
//				2. �׷��� �Ͽ���, Esc Key�� �̿��� Kill Focus�� �������� Update���� �ʴ´�.
///////////////////////////////////////////////////////////////////////////////
	m_bUpdateOnlyEnter = FALSE;
}

CLVEdit::~CLVEdit()
{
}


BEGIN_MESSAGE_MAP(CLVEdit, CEdit)
	//{{AFX_MSG_MAP(CLVEdit)
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLVEdit message handlers

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	Enter�� �Է¸� Focus�� ������ �������̴� ���� �ݿ��ϰ� �����Ų��.
//
//	����	:	�̶� ���� Item�� ������ ���� �������� �����Ѵ�.
//
//	����	:	Ư�� ó���� �Ϸ�� ��쿡�� �ٸ� Window�� Message�� ���� ���ϵ��� True�� Return�Ѵ�.
//
//	����	:	(2003.01.08) Label Edit Control�� �Ź� �������� �ʰ�, �ϳ����� �����Ͽ� �����ϵ��� �����Ѵ�.
//				(2003.01.08) ���� Label Edit�� �̵��� Ctrl Key�� ���� ������ Arrow�� �����Ѵ�.
//				(2003.02.21) UpdateOnlyEnter Option�� �����ϱ� ���Ͽ�, Update Routine�� ������ �����Ѵ�.
//				(2003.02.21) ESC�� ���� Kill Focus�ÿ��� Update�� ���� �ʵ��� m_nSubItem�� Ȯ���Ѵ�.
//					m_nSubItem�� -1�̸� ESC�� ���� �����̴�.
///////////////////////////////////////////////////////////////////////////////
BOOL CLVEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	// 0. ���� ESC�� ��쿡�� �����Ų��.
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)		
	{
		// (2003.02.21) ESC�� ���� Kill Focus�ÿ��� Update�� ���� �ʵ��� m_nSubItem�� Ȯ���Ѵ�.
		//	m_nSubItem�� -1�̸� ESC�� ���� �����̴�.
		m_nSubItem = -1;

		ShowWindow( SW_HIDE);
		return TRUE;
	}

	// 0.5 �Է��� �Ϸ�� ��츦 ó���Ѵ�.
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)		
	{
		// 1. ���� �ݿ��Ѵ�.
		//	(2003.02.21) UpdateOnlyEnter Option�� �����ϱ� ���Ͽ�, Update Routine�� ������ �����Ѵ�.
		UpdateToListCtrl();

		// 2. Hidden ��Ų��.
		ShowWindow( SW_HIDE);

		// 3. ���� Item�� ������ �����Ѵ�.
		// 3.1 ���� Item�� �ִ��� Ȯ���Ѵ�.
		int nNextItem = m_nItem + 1;
		if( nNextItem <= m_pEditableListCtrl->GetItemCount())
		{
			// 3.2 ���� Item�� ���ý�Ų��.
			m_pEditableListCtrl->SetItemState( nNextItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			m_pEditableListCtrl->EditSubItem( nNextItem, m_nSubItem);
		}

		return TRUE;
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	Focus�� ������ �������̴� ���� �����ϰ� �׳� �����Ų��.
//
//	����	:	(2003.01.08) Label Edit Control�� �Ź� �������� �ʰ�, �ϳ����� �����Ͽ� �����ϵ��� �����Ѵ�.
//				(2003.02.21) UpdateOnlyEnter Option�� �ƴ϶�� KillFocus�ÿ� Update��Ų��.
///////////////////////////////////////////////////////////////////////////////
void CLVEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	
	// (2003.02.21) UpdateOnlyEnter Option�� �ƴ϶�� KillFocus�ÿ� Update��Ų��.
	if( !m_bUpdateOnlyEnter) UpdateToListCtrl();

	ShowWindow( SW_HIDE);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	Alt+Arrow Key�� �ԷµǸ� ����â�� �̵���Ų��.
//
//	����	:	1. ���� Ctrl Key�� �̿��Ϸ� �Ͽ�����, ������ Flag�� �����Ͽ��� �ϰ�,
//					������ ������ ���� Alt�� ��ȯ�Ͽ���.
//				2. �׷��� Alt�� Message ��ü�� ���޵��� �ʴ� ������ �־� Caret�� ��ġ�� �����Ͽ���.
//
//	����	:	(2003.01.08) Label Edit�� �̵��� Ctrl Key�� ���� ������ Arrow�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
void CLVEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	int i = 0;
	// 0. Ctrl Key�� Ȯ���Ѵ�.
	if( nChar == VK_CONTROL) m_bCtrl = TRUE;

	// 1. Ctrl+Arrow Key�� ��츦 ó���Ѵ�.
	else if( m_bCtrl)
	{
		if( nChar == VK_UP)		if( TryEditSubItem( m_nItem - 1,	m_nSubItem))		return;
		if( nChar == VK_DOWN)	if( TryEditSubItem( m_nItem + 1,	m_nSubItem))		return;

		// 2. Left�� Right�� ���� �Ұ��� Column�� �ǳʶ� �� �ֵ��� �Ѵ�.
		if( nChar == VK_LEFT)
		{
			for(  i = m_nSubItem - 1; i >= 0; i--) if( m_pEditableListCtrl->m_bEditableColumn[ i]) break;
			if( i >= 0) if( TryEditSubItem( m_nItem, i))	return;
		}
		if( nChar == VK_RIGHT)
		{
			int nColumn = m_pEditableListCtrl->m_bEditableColumn.GetSize();
			for( int i = m_nSubItem + 1; i < nColumn; i++) if( m_pEditableListCtrl->m_bEditableColumn[ i]) break;
			if( i < nColumn) if( TryEditSubItem( m_nItem, i))	return;
		}
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CLVEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	// 0. Ctrl Key�� Ȯ���Ѵ�.
	if( nChar == VK_CONTROL) m_bCtrl = FALSE;
	
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	Ctrl+Arrow Key�� ���Ͽ� ���� Sub Item�� ������ �õ��ϴ� Routine�� �����Ѵ�.
//
//	����	:	(2003.01.08) �̵� ���� ���θ� Ȯ���ϱ� ���Ͽ� BOOL�� Return�Ѵ�.
///////////////////////////////////////////////////////////////////////////////
BOOL CLVEdit::TryEditSubItem( int p_nItem, int p_nSubItem)
{
	// 1. �ش� Index�� ���� ���������� Ȯ���Ѵ�.
	if( p_nItem < 0 || p_nItem >= m_pEditableListCtrl->GetItemCount() || p_nSubItem < 0 || p_nSubItem >= m_pEditableListCtrl->m_bEditableColumn.GetSize()) return FALSE;
	if( !m_pEditableListCtrl->m_bEditableColumn[ p_nSubItem]) return FALSE;

	// 2. ���� Item�� ���ý�Ų��.
	m_pEditableListCtrl->SetItemState( p_nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	return m_pEditableListCtrl->EditSubItem( p_nItem, p_nSubItem);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.02.21
//
//	����	:	UpdateOnlyEnter Option�� �����ϱ� ���Ͽ�, Update Routine�� ������ �����Ѵ�.
//
//	����	:	(2003.01.29) Byte Array�� BOOL Array�� Ȱ���Ͽ�����, ������ ���� Flag���� Ȱ���Ͽ� 
//					ReadOnly, CopyOnly, Editable�� �з��Ͽ� ó������Ѵ�.
//					0	: ReadOnly
//					1	: CopyOnly
//					2	: Editable
//				(2003.01.31) Parent Window���� �����Ϸ���� �˸��� Notification�� ó���Ѵ�.
//				(2003.02.02) Notification�� ���� ������ Data�� �����ϵ��� �Ѵ�.
//				(2003.02.21) ESC�� ���� Kill Focus�ÿ��� Update�� ���� �ʵ��� m_nSubItem�� Ȯ���Ѵ�.
//					m_nSubItem�� -1�̸� ESC�� ���� �����̴�.
///////////////////////////////////////////////////////////////////////////////
void CLVEdit::UpdateToListCtrl( void)
{
	// (2003.02.21) ESC�� ���� Kill Focus�ÿ��� Update�� ���� �ʵ��� m_nSubItem�� Ȯ���Ѵ�.
	//	m_nSubItem�� -1�̸� ESC�� ���� �����̴�.
	if( m_nSubItem < 0) return;

	//	(2003.01.29) Byte Array�� BOOL Array�� Ȱ���Ͽ�����, ������ ���� Flag���� Ȱ���Ͽ� 
	//		ReadOnly, CopyOnly, Editable�� �з��Ͽ� ó������Ѵ�.
	if( m_pEditableListCtrl->m_bEditableColumn[ m_nSubItem] == 2)
	{
		// (2003.02.02) Notification�� ���� ������ Data�� �����ϵ��� �Ѵ�.
		CString strOldValue = m_pEditableListCtrl->GetItemText( m_nItem, m_nSubItem);

		CString strNewValue;
		GetWindowText( strNewValue);
		m_pEditableListCtrl->SetItemText( m_nItem, m_nSubItem, strNewValue);

		// (2003.01.31) Parent Window���� �����Ϸ���� �˸��� Notification�� ó���Ѵ�.
		NMEDITABLELISTCTRL nmEditableListCtrl;
		nmEditableListCtrl.m_nItemIndex = m_nItem;
		nmEditableListCtrl.m_nColumnIndex = m_nSubItem;
		nmEditableListCtrl.m_szOldData = strOldValue;
		nmEditableListCtrl.m_szNewData = strNewValue;
		m_pEditableListCtrl->GetParent()->SendMessage( RMSG_ELC_CELLEDITED, m_pEditableListCtrl->GetDlgCtrlID(), ( LPARAM)&nmEditableListCtrl);
	}
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.02.21
//
//	����	:	Parent�� List Control�� Pointer�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
BOOL CLVEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class
	
	m_pEditableListCtrl = ( CEditableListCtrl *)pParentWnd;
	
	return CEdit::Create( dwStyle, rect, pParentWnd, nID);
}
