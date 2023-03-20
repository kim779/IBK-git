#pragma once
// AccEdit.h : header file
//

#define	WM_EDITX	(WM_USER + 100)

#define	wpBLANK			1
#define	wpENTER			2
#define	wpSEARCH1		3
#define	wpSEARCH2		4
#define wpSETACCOUNT	5

#include "OXMaskedEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CAccEdit window

class CAccEdit : public COXMaskedEdit
{
// Construction
public:
	CAccEdit();

// Attributes
public:
	inline BOOL GetEditMode() { return m_bEditMode; };
	inline void SetEditMode(BOOL bEditMode) { m_bEditMode = bEditMode; };

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccEdit)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAccEdit();

	// Generated message map functions
protected:
	CString GetClipboard();
	void SetClipboard(CString strData);
	//{{AFX_MSG(CAccEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	void    Update                      (int nSelectionStart=0) ;  // UpdateInsertionPoint is automatic.
	// numberpad�� ����Ű�� ���ڷ� �ν��Ѵ�.
	bool	IsNumber(UINT nChar);	//2012.02.03 KSJ
	// ������ Ư��Ű�϶� TRUE
	bool	IsSpecialKey(UINT nChar);	//2012.02.13 KSJ
public:
	void	SetAccMask();			//2012.02.14 KSJ �ܺο��� ����ũ�����Ҽ��ְ�.

private:
	BOOL m_bEditMode;
	BOOL m_bAllSelected;	
	BOOL m_bEditNumType;		// 2012.02.13 KSJ ������������ ������������.
	BOOL m_bKeyDown;			// 2012.02.15 KSJ Ű���� �ٿ�ƴ��� üũ
};