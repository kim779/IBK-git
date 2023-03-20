#if !defined(AFX_LVEDIT_H__18636D59_169E_4D7F_BD1C_EA2C450B6CA2__INCLUDED_)
#define AFX_LVEDIT_H__18636D59_169E_4D7F_BD1C_EA2C450B6CA2__INCLUDED_

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	List Control�� Sub Item�� �����ϴ� Edit Control�̴�.
///////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LVEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLVEdit window

class CLVEdit : public CEdit
{
// Construction
public:
	CLVEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLVEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLVEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLVEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	������ ������ �ݿ��� List Control�� Item�� SubItem Index�� �����Ѵ�.
//
//	����	:	Parent���� Edit Control�� ������ �� �����Ѵ�. (Friend Class)
///////////////////////////////////////////////////////////////////////////////
protected:
	int m_nItem;
	int m_nSubItem;
friend class CEditableListCtrl;

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.07
//
//	����	:	Ctrl+Arrow Key�� ���Ͽ� ���� Sub Item�� ������ �õ��ϴ� Routine�� �����Ѵ�.
//
//	����	:	(2003.01.08) �̵� ���� ���θ� Ȯ���ϱ� ���Ͽ� BOOL�� Return�Ѵ�.
///////////////////////////////////////////////////////////////////////////////
protected:
	BOOL	TryEditSubItem( int p_nItem, int p_nSubItem);

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.01.08
//
//	����	:	Ctrl Key�� ���� ���¸� ��Ÿ���� Flag�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
protected:
	BOOL	m_bCtrl;

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
protected:
	BOOL		m_bUpdateOnlyEnter;

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.02.21
//
//	����	:	UpdateOnlyEnter Option�� �����ϱ� ���Ͽ�, Update Routine�� ������ �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
protected:
	void		UpdateToListCtrl( void);

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.02.21
//
//	����	:	Parent�� List Control�� Pointer�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
protected:
	CEditableListCtrl *	m_pEditableListCtrl;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LVEDIT_H__18636D59_169E_4D7F_BD1C_EA2C450B6CA2__INCLUDED_)
