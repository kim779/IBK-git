#pragma once
// InfoPopup.h : header file
//
#include "jinfo.h"
#include "editX.h"

/////////////////////////////////////////////////////////////////////////////
// CInfoPopup window

class CInfoPopup : public CWnd
{
// Construction
public:
	CInfoPopup();

// Attributes
public:
	CWnd	*m_pWizard;
	CString m_sKey;
	Cjinfo	m_jinfo;
	std::unique_ptr<CEditX> m_pEdit;
	bool	m_bShow;

protected:
	bool	m_fDone, m_bDrag;
	CPoint	m_firstPnt;
	CRect	m_rc;
	RECT	m_RECT;
	int	m_nSelected;
	CString m_sUserID, m_sRoot;

	CFont	*m_pFont;

	bool	m_ujongsise;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoPopup)
	//}}AFX_VIRTUAL

// Implementation
public:
	CString addComma(CString data);
	CString LoadStringTb(UINT nIdx);
	CString getDiffSym(char chDiff);
	CFont*	getAxFont(CString fName, int point, int style);
	CPen*	getAxPen(COLORREF clr, int width, int style);
	CBrush*	getAxBrush(COLORREF clr);
	CString	SetComma(CString src);
	void	setPathInfo(CString sRoot, CString sID);
	void	loadMemo(CString sKey);
	void	saveMemo(CString sKey);
	void	drawData(CDC *pDC);
	void	drawCaption(CDC *pDC);
	void	drawIndicator(CDC* dc, CRect rect);
	void	setJinfo(Cjinfo jinfo, bool ujongsise);
	virtual ~CInfoPopup();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInfoPopup)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};