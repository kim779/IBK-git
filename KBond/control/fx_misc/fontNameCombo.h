#if !defined(AFX_FONTNAMECOMBO_H__5C904EA1_B49F_4F37_8D66_E2F6CD2CF818__INCLUDED_)
#define AFX_FONTNAMECOMBO_H__5C904EA1_B49F_4F37_8D66_E2F6CD2CF818__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fontNameCombo.h : header file
//

struct fontITEM
{
	DWORD	dwType;
	LOGFONT	lf;
};

/////////////////////////////////////////////////////////////////////////////
// CfontNameCombo window

class CfontNameCombo : public CComboBox
{
// Construction
public:
	CfontNameCombo();

// Attributes
public:
	CBitmap	m_mask;
	CBitmap	m_bitmap;
	BOOL	m_loaded;

	CFont		*m_pFont;
	CWnd		*m_pParent;

	COLORREF	m_cbRGB;
	COLORREF	m_csRGB;
	
	BOOL		m_bEnable;
// Operations
public:
	int	AddFont(LOGFONT* lf, DWORD type);
	CString	GetCurrentName();

	fontITEM* GetFontItem(int sel = -1);
	DWORD	GetFontType(int sel = -1);

	BOOL		Create(int dwStyle, CRect rect, CWnd* parent, UINT nID);
	void		SetFont(CFont* pFont, bool redraw = true);
	void		SetPalette(COLORREF clrFg, COLORREF clrBg);
	void		SetParent(CWnd * parent) {m_pParent = parent;}

protected:
	void	drawMaskedBitmap(CDC* dc, CBitmap* pbmp, CBitmap* pbmpMask, int x, int y, int cx, int cy);
	void	initMaskFromBitmap(CBitmap* pbmp, CBitmap* pbmpMask);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfontNameCombo)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDIS);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CfontNameCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CfontNameCombo)
	afx_msg void OnDropdown();
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

#endif // !defined(AFX_FONTNAMECOMBO_H__5C904EA1_B49F_4F37_8D66_E2F6CD2CF818__INCLUDED_)
