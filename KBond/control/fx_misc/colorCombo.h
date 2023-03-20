#if !defined(AFX_COLORCOMBO_H__F777AB9D_42AF_4C3F_B282_518AD19ABE5F__INCLUDED_)
#define AFX_COLORCOMBO_H__F777AB9D_42AF_4C3F_B282_518AD19ABE5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// colorCombo.h : header file
//

#include <afxtempl.h>

#define	userColor	0x02000000

/////////////////////////////////////////////////////////////////////////////
// CcolorCombo window

class CcolorCombo : public CComboBox
{
// Construction
public:
	CcolorCombo();

// Attributes
public:
	CString	m_path;
	COLORREF m_color;
	CArray	<COLORREF, COLORREF>	m_colors;

	CFont		*m_pFont;
	CWnd		*m_pParent;

	COLORREF	m_cbRGB;
	COLORREF	m_csRGB;
	
	BOOL		m_bEnable;
// Operations
public:
	COLORREF GetColor();
	void	SetColor(COLORREF clr);
	void	SetPalette(CString path);
	BOOL	Create(int dwStyle, CRect rect, CWnd* parent, UINT nID);
	void	SetFont(CFont* pFont, bool redraw = true);
	void	SetPalette(COLORREF clrFg, COLORREF clrBg);
	void	SetParent(CWnd * parent) {m_pParent = parent;}

protected:
	bool	loadColor();
	bool	parseString(CString string, BYTE& rr, BYTE& gg, BYTE& bb);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CcolorCombo)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CcolorCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CcolorCombo)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG
	afx_msg BOOL OnSelChange();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORCOMBO_H__F777AB9D_42AF_4C3F_B282_518AD19ABE5F__INCLUDED_)
