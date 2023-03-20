// EditableListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "EditableListCtrl.h"

#include "LVEdit.h"	// for CLVEdit

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditableListCtrl

CEditableListCtrl::CEditableListCtrl()
{

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.08
//
//	����	:	Label �������� Edit Control�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
	m_pLabelEdit = new CLVEdit;
}

CEditableListCtrl::~CEditableListCtrl()
{

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.08
//
//	����	:	Label �������� Edit Control�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
	delete m_pLabelEdit;
}


BEGIN_MESSAGE_MAP(CEditableListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CEditableListCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditableListCtrl message handlers

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	Editable List Control�� �⺻ Style�� �����Ѵ�.
//
//	����	:	(2003.01.08) �⺻ �Ӽ��� LVS_EDITLABELS�� �߰��Ѵ�.
//				(2003.01.08) �⺻ �Ӽ����� �׻� Report Type�� �ǵ��� �Ѵ�.
///////////////////////////////////////////////////////////////////////////////
void CEditableListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	ModifyStyle( LVS_ICON | LVS_SMALLICON | LVS_LIST, LVS_SINGLESEL | LVS_EDITLABELS | LVS_REPORT);
	SetExtendedStyle( LVS_EX_FULLROWSELECT);


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.08
//
//	����	:	Label �������� Edit Control�� �����Ѵ�.
//
//	����	:	������ �ʰ� �����Ͽ� ���´�.
///////////////////////////////////////////////////////////////////////////////
	CRect rctLabelEdit( 0, 0, 0, 0);
	m_pLabelEdit->Create( WS_BORDER | ES_AUTOHSCROLL, rctLabelEdit, this, 107);
	m_pLabelEdit->SetFont( GetFont());

	CListCtrl::PreSubclassWindow();
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	������ Item�� �ٽ� Click�� ��쿡�� Edit Control�� �����Ͽ� �����ش�.
///////////////////////////////////////////////////////////////////////////////
void CEditableListCtrl::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here

	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	EditSubItem( point);

	*pResult = 1;
}
void CEditableListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	EditSubItem( point);

	CListCtrl::OnLButtonDblClk(nFlags, point);
}
void CEditableListCtrl::EditSubItem( CPoint point)
{
	// 1. ���� ���õ� Item�� Index�� ���Ͽ� Selection ���θ� Ȯ���Ѵ�.
	//	(Column Info�� Ȯ���� �� �ֵ��� SubItemHitTest�� �̿��Ѵ�.)
	LVHITTESTINFO lvInfo;
	lvInfo.pt = point;
	SubItemHitTest( &lvInfo);

	// 2. Index�� SubItem ������ �����Ѵ�.
	EditSubItem( lvInfo.iItem, lvInfo.iSubItem);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	Index�� SubItem�� ������ �� �� �ֵ��� Index�� Interface�� �����Ѵ�.
//
//	����	:	(2003.01.08) ����â�� ������ �������̾����� Ȯ���� �� �ֵ��� BOOL�� Return�Ѵ�.
//				(2003.01.08) ����â�� ������ ������ ��쿡�� List Control�� Scoll�Ͽ� �� �� �ֵ��� �Ѵ�.
///////////////////////////////////////////////////////////////////////////////
BOOL CEditableListCtrl::EditSubItem( int p_nItem, int p_nSubItem)
{
	// 1. ������ Column�� ���� �������� Ȯ���ϰ� ó���Ѵ�.
	if( p_nItem < 0 || p_nItem >= GetItemCount() || !m_bEditableColumn[ p_nSubItem]) return FALSE;

	// 2. ���õ� Item�� Click�� ��쿡�� �ش� Column�� ��ġ�� Edit Control�� �����Ѵ�.
	if( !GetItemState( p_nItem, LVIS_SELECTED)) return FALSE;

	// (2003.01.08) ����â�� ������ ������ ��쿡�� List Control�� Scoll�Ͽ� �� �� �ֵ��� �Ѵ�.
	// 2.5.1 Scroll�ÿ� Edit�� �ܿ��� ���� �ʵ��� �켱 Hide��Ų��.
	m_pLabelEdit->ShowWindow( SW_HIDE);
	// 2.5.2 �� SubItem�� ��ġ ������ Ȯ���� �� �ֵ��� ������ Scoll ��Ų��.
	EnsureVisible( p_nItem, FALSE);
	// 2.5.3 �ش� Item�� Sub Item ��ġ�� ���� �����Ѵ�.
	CRect rctSubItem, rctClientRect;
	GetSubItemRect( p_nItem, p_nSubItem, LVIR_LABEL, rctSubItem);
	if( rctSubItem.left < 0) Scroll( CSize( rctSubItem.left, 0));
	else
	{
		// 2.5.4 Edit Control�� ������ Scoll���� ������ ���� ��쿡�� Scroll�Ͽ� �����ش�.
		GetClientRect( rctClientRect);
		if( rctSubItem.right > rctClientRect.right)
		{
			int nScroll = rctSubItem.right - rctClientRect.right;
			if( rctSubItem.left - nScroll < 0) Scroll( CSize( rctSubItem.left, 0));
			else Scroll( CSize( nScroll, 0));
		}
	}

	// 3. �ش� Item�� Sub Item ��ġ�� �ٽ� ���Ѵ�.
	GetSubItemRect( p_nItem, p_nSubItem, LVIR_LABEL, rctSubItem);
	rctSubItem.InflateRect( 0, 2, 0, 1);

	// 4. Edit Control�� Scroll ���� ������ ���� �ʵ��� ũ�⸦ �����Ѵ�.
	GetClientRect( rctClientRect);
	if( rctSubItem.left < 0) rctSubItem.left = 0;
	if( rctSubItem.right > rctClientRect.right) rctSubItem.right = rctClientRect.right;

	// 5. ������ Sub Item�� ��ġ�� Edit Control�� �����Ͽ� �����ش�.
	m_pLabelEdit->m_nItem = p_nItem;
	m_pLabelEdit->m_nSubItem = p_nSubItem;
	m_pLabelEdit->MoveWindow( rctSubItem);
	m_pLabelEdit->ShowWindow( SW_SHOW);

	// 6. ������ �⺻ Data�� �����ش�.
	//	(Ư�� ���� ������ �����ϵ��� �⺻������ ������ �����Ѵ�.)
	m_pLabelEdit->SetWindowText( GetItemText( p_nItem, p_nSubItem));
	m_pLabelEdit->SetFocus();
	m_pLabelEdit->SetSel( 0, -1);

	// 7. Label Edit �۾����̱⶧������ Item Label�� Highlight�� Focus �սǷ� �������� �ʴ´�.
	//	(������ Drawing��Ų��.)
	GetSubItemRect( p_nItem, 0, LVIR_LABEL, rctSubItem);
	InvalidateRect( rctSubItem);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	Enter Key�� F2�� �̿��Ͽ� ���õ� Item�� ������ �� �ֵ����Ѵ�.
//
//	����	:	1. Ư�� ó���� �Ϸ�� ��쿡�� �ٸ� Window�� Message�� ���� ���ϵ��� True�� Return�Ѵ�.
//				2. �ٸ� Class�� �̹� PreTranslateMessage()�� �־����, OnKeyDown()�� ������,
//					PreTranslateMessage()�� �Ҹ����ʴ´�. (���� ��Ȯ��. ������ ó�� ����)
///////////////////////////////////////////////////////////////////////////////
BOOL CEditableListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		StartEditSubItem();
		return TRUE;
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}
void CEditableListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	if( nChar == VK_F2)	StartEditSubItem();

	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	Key Input���� ������ �����ϴ� Routine�� �߰��Ѵ�.
///////////////////////////////////////////////////////////////////////////////
void CEditableListCtrl::StartEditSubItem( void)
{
	int nItem = GetNextItem( -1,  LVNI_SELECTED);
	int nSubItem, nColumnCount = m_bEditableColumn.GetSize();
	for( nSubItem = 0; nSubItem < nColumnCount; nSubItem++) if( m_bEditableColumn[ nSubItem]) break;
	EditSubItem( nItem, nSubItem);
}


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	Scroll �õ��ÿ��� Label Edit Control�� �����.
///////////////////////////////////////////////////////////////////////////////
void CEditableListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	m_pLabelEdit->ShowWindow( SW_HIDE);
	
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}
void CEditableListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	m_pLabelEdit->ShowWindow( SW_HIDE);
	
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}
BOOL CEditableListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	m_pLabelEdit->ShowWindow( SW_HIDE);
	
	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.02.21
//
//	����	:	Enter�� �̿��Ͽ��߸� Update�� �������� ���θ� �����ϴ� Interface�� �����Ѵ�.
//
//	����	:	1. �⺻������ Flase�̸�, Enter�ܿ��� �ܼ��� Kill Focus �������� Update�ȴ�.
//				2. �׷��� �Ͽ���, Esc Key�� �̿��� Kill Focus�� �������� Update���� �ʴ´�.
///////////////////////////////////////////////////////////////////////////////
void CEditableListCtrl::SetUpdateOnlyEnter( BOOL p_bUpdateOnlyEnter)
{
	m_pLabelEdit->m_bUpdateOnlyEnter = p_bUpdateOnlyEnter;
}
