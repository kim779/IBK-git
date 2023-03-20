#if !defined(AFX_FXSTATIC_H__B7DCD616_60DA_4578_8F1F_F3D5CD30A1BA__INCLUDED_)
#define AFX_FXSTATIC_H__B7DCD616_60DA_4578_8F1F_F3D5CD30A1BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fxStatic.h : header file
//

#include "afxmt.h"

/////////////////////////////////////////////////////////////////////////////
// CfxStatic window

class AFX_EXT_CLASS CfxStatic : public CStatic
{
// Construction
public:
	CfxStatic(CFont* pFont = NULL);

// Attributes
protected:
	DWORD	m_style;
	BYTE	m_alignment;
	CString	m_text;
	DWORD	m_attr;
	int	m_float;
	CWnd*	m_parent;
	bool	m_flash;

	CFont*	m_pFont;

	COLORREF	m_orgBACK;
	COLORREF	m_clrBORDER;
	COLORREF	m_clrBACK, m_clrBACK2;
	COLORREF	m_clrFORE;
	COLORREF	m_upCOL;
	COLORREF	m_dnCOL;
	COLORREF	m_eqCOL;
	CBrush		m_brBACK, m_brBACK2;
	CBrush		m_brBORDER;
	
	CDC		m_memDC;
	CBitmap m_memBuf;
	CBitmap*m_pOldBuf;


	CCriticalSection m_sync;	// by NMH
// Operations
public:
	BOOL	Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	void	Init(DWORD style, DWORD attr, CString text = _T(""), CString images = _T(""),
					COLORREF clrBACK = GetSysColor(COLOR_3DFACE),
					COLORREF clrBACK2= GetSysColor(COLOR_3DFACE),
					COLORREF clrFORE = GetSysColor(COLOR_BTNTEXT),
					BYTE alignment = 0 /*alCENTER*/, int float_size = 0);
	void	SetStyle(DWORD style, bool redraw = true);
	void	SetText(CString text, bool redraw = true);
	CString	GetText();
	
	void	SetBitmap(CString path, bool redraw = true);
	void	SetBkColor(COLORREF clrBACK, bool redraw = true);
	void	SetBk2Color(COLORREF clrBACK, bool redraw = true);
	void	SetFgColor(COLORREF clrFORE, bool redraw = true);
	void	SetBorderColor(COLORREF clrBORDER, bool redraw = true);
	void	SetConditionColor(COLORREF upCOL, COLORREF dnCOL, COLORREF eqCOL);
	void	SetUpColor(COLORREF color)	{ m_upCOL = color; }
	void	SetDnColor(COLORREF color)	{ m_dnCOL = color; }
	void	SetEqColor(COLORREF color)	{ m_eqCOL = color; }
	COLORREF GetUpColor()			{ return m_upCOL; }
	COLORREF GetDnColor()			{ return m_dnCOL; }
	COLORREF GetEqColor()			{ return m_eqCOL; }
	
	COLORREF GetBkColor();
	COLORREF GetBk2Color();
	COLORREF GetFgColor();
	COLORREF GetBorderColor();
	void	SetFont(CFont* pFont, bool redraw = true);
	void	SetAlignment(BYTE alignment, bool redraw = true);
	void	ModifyAttribute(DWORD attrRM, DWORD attrADD);
	void	Blink(COLORREF color, int interval = 500);
	void	SetFloat(int float_size, bool redraw = true);
	int	GetFloat();

protected:
	CString	makeData(CString text);
	LRESULT	messageToParent(int nMessage, CString text = _T(""));
	CString	commaModify(CString string);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CfxStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CfxStatic)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();	
	afx_msg void OnClicked();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FXSTATIC_H__B7DCD616_60DA_4578_8F1F_F3D5CD30A1BA__INCLUDED_)
