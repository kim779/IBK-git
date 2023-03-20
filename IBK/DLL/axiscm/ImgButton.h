#if !defined(AFX_IMGBUTTON_H__032EEAD3_14AB_41A0_8F1A_FCCF75777444__INCLUDED_)
#define AFX_IMGBUTTON_H__032EEAD3_14AB_41A0_8F1A_FCCF75777444__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImgButton.h : header file
//
#include "AxCommon.h"
/////////////////////////////////////////////////////////////////////////////
// CImgButton window

class AXIS_EXT_CLASS CImgButton : public CWnd
{
// Construction
public:
	CImgButton(CFont* pFont = NULL);

// Attributes
public:	
	DWORD		m_dwState{};
	bool		m_capture, m_bChecked, m_bCheck, m_bStretch, m_bFocus, m_bDrawFocus	;
	BOOL		m_bHover, m_bTracking;
	HBITMAP		m_hBitmap, m_hBitmap_dn, m_hBitmap_hv;

protected:
	CFont		*m_pFont;
	std::unique_ptr<CToolTipCtrl> m_pTipCtrl;

private:
	COLORREF	m_clText;

// Operations
public:
	BOOL	Create(LPCTSTR caption, DWORD style, const RECT& rect, CWnd* parent, UINT nID, bool bCheck = FALSE, bool bStretch = FALSE);

	void	SetTextColor(COLORREF clText);
	void	SetImgBitmap(HBITMAP hBitmap, HBITMAP hBitmap_dn, HBITMAP hBitmap_hv);
	void	SetFont(CFont* pFont, BOOL bRedraw = true);
	void	SetCheck(bool bCheck = true);
	void	SetDrawFocus(bool bDraw = true);

	bool	IsChecked();

protected:
	CString	Parser(CString &srcstr, CString substr);
	CSize	BitmapSize(HBITMAP hBitmap);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgButton)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void CreateTooltip();
	void FitImageSize();
	virtual ~CImgButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImgButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnBmSetCheck(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBmGetCheck(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGBUTTON_H__032EEAD3_14AB_41A0_8F1A_FCCF75777444__INCLUDED_)
