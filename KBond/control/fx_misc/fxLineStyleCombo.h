#if !defined(AFX_FXLINESTYLECOMBO_H__E3751C9A_8142_49A5_B1A3_143850272C03__INCLUDED_)
#define AFX_FXLINESTYLECOMBO_H__E3751C9A_8142_49A5_B1A3_143850272C03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fxLineStyleCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CfxLineStyleCombo window

class AFX_EXT_CLASS CfxLineStyleCombo : public CComboBox
{
// Construction
public:
	CfxLineStyleCombo();

public:
	CFont		*m_pFont;
	CWnd		*m_pParent;

	COLORREF	m_cbRGB;
	COLORREF	m_csRGB;
	
	BOOL		m_bEnable;
// Attributes
protected:
	CObArray	m_pens;

// Operations
public:
	int		GetStyle();
	void		SetStyle(int style);
	BOOL		Create(int dwStyle, CRect rect, CWnd* parent, UINT nID);
	void		SetFont(CFont* pFont, bool redraw = true);
	void		SetPalette(COLORREF clrFg, COLORREF clrBg);
	void		SetParent(CWnd * parent) {m_pParent = parent;}


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxLineStyleCombo)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CfxLineStyleCombo();

	// Generated message map functions
protected:
	void	loadPen();
	void	removePen();

	//{{AFX_MSG(CfxLineStyleCombo)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FXLINESTYLECOMBO_H__E3751C9A_8142_49A5_B1A3_143850272C03__INCLUDED_)
