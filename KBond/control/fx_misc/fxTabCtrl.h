#if !defined(AFX_FXTABCTRL_H__389048FA_B576_4AAE_8E50_FEBA1D236F53__INCLUDED_)
#define AFX_FXTABCTRL_H__389048FA_B576_4AAE_8E50_FEBA1D236F53__INCLUDED_
#include "memdc.h"
#if _MSC_VER > 1000
#pragma once
#endif

#define L_STAB 0
#define C_STAB 1
#define R_STAB 2
#define L_TAB 3
#define C_TAB 4
#define R_TAB 5
#define TAB_BG 6

class AFX_EXT_CLASS CfxTabCtrl : public CTabCtrl
{
public:
	CfxTabCtrl();
	virtual ~CfxTabCtrl();

	//{{AFX_VIRTUAL(CfxTabCtrl)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CfxTabCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL	Create(CRect rect, CWnd* pParentWnd, UINT nID);
	void	SetBkColor(COLORREF clrBack);
	void	SetColor(COLORREF clrunBack, COLORREF clrun, COLORREF clrSel, COLORREF clrFontSel);
	void	SetTabImage(UINT bmpID);
	void	SetFont(CFont* pFont, bool redraw = true);
	void	SetSelectFont(LOGFONT* logfont, bool redraw = true);
	void	SetViewBorder(bool bBorder, bool redraw = true);
protected:
	LOGFONT getLogFont(int fsize, DWORD weight);
private:
	void	drawMainBorder(LPDRAWITEMSTRUCT lpdis, CRect sRect);
	void	drawFlatItem(LPDRAWITEMSTRUCT lpdis);

	COLORREF getTabColor(BOOL bSelected = FALSE);
	COLORREF getMainColor(BOOL bSelected = FALSE);
	COLORREF getTabTextColor(BOOL bSelected = FALSE);

	COLORREF darker(COLORREF crBase, float fFactor);
	COLORREF lighter(COLORREF crBase, float fFactor);
	CSize	formatText(CString& sText, CDC* pDC, int nWidth);


private:
	CFont		*m_pFont;
	COLORREF	m_clrBack;
	COLORREF	m_clrUnSel;
	COLORREF	m_clrSel;
	COLORREF	m_clrFontUnSel;
	COLORREF	m_clrFontSel;
	CImageList*	m_pImageList;
	CWnd*		m_parent;
	bool		m_bBorder;	
	bool		m_bTriangle;
public:
	void LoadImg(CString path);

	void drawImgBG(CMemDC* pDC, CRect rRc);
	void drawImgBtn(CMemDC* pDC, LPDRAWITEMSTRUCT lpdis);
public:
	bool		m_bImgBtn;
	HBITMAP		m_hBmp[7];
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_EXTABCTRL_H__389048FA_B576_4AAE_8E50_FEBA1D236F53__INCLUDED_)
