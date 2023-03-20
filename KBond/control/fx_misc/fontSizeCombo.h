#if !defined(AFX_FONTSIZECOMBO_H__5E6E675B_45BE_4B2A_8D26_97117CDE7F8B__INCLUDED_)
#define AFX_FONTSIZECOMBO_H__5E6E675B_45BE_4B2A_8D26_97117CDE7F8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fontSizeCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CfontSizeCombo window

class CfontSizeCombo : public CComboBox
{
// Construction
public:
	CfontSizeCombo();

// Attributes
public:
	CFont		*m_pFont;
	CWnd		*m_pParent;

	COLORREF	m_cbRGB;
	COLORREF	m_csRGB;
	
	BOOL		m_bEnable;
// Operations
public:
	int	AddSize(int point, LONG height);
	LONG	GetHeight(int sel = -1);
	BOOL		Create(int dwStyle, CRect rect, CWnd* parent, UINT nID);
	void		SetFont(CFont* pFont, bool redraw = true);
	void		SetPalette(COLORREF clrFg, COLORREF clrBg);
	void		SetParent(CWnd * parent) {m_pParent = parent;}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfontSizeCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CfontSizeCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CfontSizeCombo)
		// NOTE - the ClassWizard will add and remove member functions here.
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

#endif // !defined(AFX_FONTSIZECOMBO_H__5E6E675B_45BE_4B2A_8D26_97117CDE7F8B__INCLUDED_)
