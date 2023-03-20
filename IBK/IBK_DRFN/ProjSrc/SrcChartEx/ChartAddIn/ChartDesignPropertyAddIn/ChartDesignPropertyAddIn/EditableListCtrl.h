#if !defined(AFX_EDITABLELISTCTRL_H__CAFE41FA_3D51_47C3_8E14_3027688AF62A__INCLUDED_)
#define AFX_EDITABLELISTCTRL_H__CAFE41FA_3D51_47C3_8E14_3027688AF62A__INCLUDED_

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	Editing�� ������ List Control Class�̴�.
//
//	����	:	1. �� List Control�� �ݵ�� InsertColumn�ÿ� �ڽ��� Memeber�� ����Ͽ���,
//					Column�� Edit���ΰ� �����ȴ�.
///////////////////////////////////////////////////////////////////////////////

#include <afxcmn.h>	// for CListCtrl


///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.31
//
//	����	:	Parent Window���� �����Ϸ���� �˸��� Notification Message�� Structure�� �����Ѵ�.
//
//	����	:	(2003.02.02) Notification�� ���� ������ Data�� �����ϵ��� �Ѵ�.
///////////////////////////////////////////////////////////////////////////////
const UINT RMSG_ELC_CELLEDITED = ::RegisterWindowMessage(_T("RMSG_EDITABLELISTCTRL_CELLEDITED"));
typedef struct tagNMEDITABLELISTCTRL
{
	int				m_nItemIndex;
	int				m_nColumnIndex;
	const char *	m_szNewData;
	const char *	m_szOldData;
} NMEDITABLELISTCTRL, FAR *LPNMEDITABLELISTCTRL;


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditableListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditableListCtrl window

class CLVEdit;
class CEditableListCtrl : public CListCtrl
{
// Construction
public:
	CEditableListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditableListCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditableListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditableListCtrl)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	Ư�� Point�� ������ Item�� �����ϴ� Routine�� ������ �����Ѵ�.
//
//	����	:	1. BeginLabelEdit()�� LDoubleClick()���� �Ѵ� Sub Item�� ������ �� �ֵ��� �Ѵ�.
//				2. ���� �ٸ� Routine������ Index�� ȣ���� �� �ֵ��� Index�� Interface�� �����Ѵ�.
//
//	����	:	���� Control ���� Routine Override�� �� �ֵ��� Virtual�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
public:
	void			EditSubItem( CPoint point);
	virtual BOOL	EditSubItem( int p_nItem, int p_nSubItem);

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	���� ������ Column�� Array�� ����Ѵ�.
//
//	����	:	(2003.01.29) Byte Array�� BOOL Array�� Ȱ���Ͽ�����, ������ ���� Flag���� Ȱ���Ͽ� 
//					ReadOnly, CopyOnly, Editable�� �з��Ͽ� ó������Ѵ�.
//					0	: ReadOnly
//					1	: CopyOnly
//					2	: Editable
///////////////////////////////////////////////////////////////////////////////
public:
	CByteArray	m_bEditableColumn;

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	���� ������ Column�� Column �߰��ø��� ��Ͻ�Ų��.
//
//	����	:	(2003.01.08) �⺻������ ���������ϵ��� �����Ѵ�.
//				(2003.01.29) Byte Array�� BOOL Array�� Ȱ���Ͽ�����, ������ ���� Flag���� Ȱ���Ͽ� 
//					ReadOnly, CopyOnly, Editable�� �з��Ͽ� ó������Ѵ�.
///////////////////////////////////////////////////////////////////////////////
public:
	int		InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1)
			{	m_bEditableColumn.Add( 2);	return CListCtrl::InsertColumn( nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);	}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	Key Input���� ������ �����ϴ� Routine�� �߰��Ѵ�.
///////////////////////////////////////////////////////////////////////////////
protected:
	void	StartEditSubItem( void);

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.08
//
//	����	:	Label �������� Edit Control�� �����Ѵ�.
//
//	����	:	1. Library�� Header�ν� �߰� Header File�� �ʿ� ������ Pointer�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
protected:
	CLVEdit *	m_pLabelEdit;

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
public:
	void		SetUpdateOnlyEnter( BOOL p_bUpdateOnlyEnter);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITABLELISTCTRL_H__CAFE41FA_3D51_47C3_8E14_3027688AF62A__INCLUDED_)
