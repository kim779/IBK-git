#pragma once

#include <afxtempl.h>
// CfxTab
#include "LBSpinButtonCtrl.h"
#include "fxImgButton.h"

struct _item
{
	CRect	rcX;
	bool	hover;
	bool	select;
};

struct menuGroup
{
	CString	m_symbol;
	CString m_str;
};

class AFX_EXT_CLASS CfxTab : public CTabCtrl
{
	
public:
	CfxTab();
	virtual ~CfxTab();

	CWnd		*m_pParent;
	CWnd		*m_pWizard;
	CFont		*m_pFont;
	COLORREF	m_bkColor, m_frColor, m_selectedbColor, m_selectedfColor, m_backColor;
	CBitmap		*m_bmpSelL, *m_bmpSelM, *m_bmpSelR, *m_bmpL, *m_bmpM, *m_bmpR, *m_bmpSpaceL, *m_bmpSpaceM, *m_bmpSpaceR;

	DWORD		m_style;
	HBITMAP		m_pics[9];
	bool		m_selfLoad;
	int		m_textUpDown;
	BOOL		m_bOnce;

	CRect			m_rcDefaultSpin;
	CLBSpinButtonCtrl	*m_pSpin;
	CWnd			*m_pOrgSpin;
	CRect			m_rcX;
	CfxImgButton*		m_pBtnSetup;
	HBITMAP			m_hBitmapSetup, m_hBitmapSetup_dn, m_hBitmapSetup_en;
	CBitmap			*m_bmpSetup, *m_bmpSetup_dn, *m_bmpSetup_en;
	int			m_bmSetupW, m_bmOverW;
	bool			m_bShowSetup;
	bool			m_bOver;
	CString			m_strImgDir;
	int			m_nHover;
	bool			m_bShowDelete;
	CString			m_sRoot;
	CString			m_sMapName, m_sUser;
	bool			m_bFirstLoad;
	COLORREF		m_clrBtnBack;
	bool			m_bShowMenu;
	
	CArray < struct menuGroup, struct menuGroup> m_SrcGroup;
public:
	BOOL	Create(CRect rect, CWnd* parent, UINT nID);
	void	SetWizard(CWnd * pWizard);
	void	SetFont(CFont* pFont, bool redraw = true);
	void	SetStyle(DWORD style, bool redraw);
	void	SetColor(COLORREF fcolor, COLORREF bcolor, COLORREF sfcolor, COLORREF sbcolor, COLORREF backColor);
	void	SetImage(HBITMAP hselectL, HBITMAP hSelectM, HBITMAP hSelectR, HBITMAP hL, HBITMAP hM, HBITMAP hR, HBITMAP hSpaceL, HBITMAP hSpaceM, HBITMAP hSpaceR);
	void	SetImage(CString imageDir, CString selectL, CString selectM, CString selectR, CString left, CString middle, CString right, CString spaceL, CString spaceM, CString spaceR);
	void	LoadTabImage(CString rootDir, bool bTopTab = true);
	void	resizeSpin();
	COLORREF GetColor(int iColor);
	void	SetSpinColor();
	void	ShowTabSetup(bool bShow = TRUE, COLORREF clrBtnBack = -1);
	void	ShowDelete(bool bShow = true);
	void	SetMapName(CString sName) { m_sMapName = sName;}
	CString GetCurList();
	void	ShowMenu(bool bShow = true);
	int		GetTabWidth();
protected:
	LOGFONT getLogFont(int fsize, DWORD weight);
	void	OnDraw(CDC *pDC);
	void	drawNormal(CDC *pDC);
	void	drawImage(CDC* pDC);
	void	draw2Rect(CDC *dc, CRect rc, COLORREF color1, COLORREF color2);
	void	DrawItem(LPDRAWITEMSTRUCT lpdis);
	LOGFONT setFont(int fsize, DWORD weight);
	void	draw3dRoundRect(CDC *dc, CRect rc, int align, int depth, COLORREF borderColor);
	int	bmpCopy(CDC *dc, CDC *memDC, int bmpidx, int posX, int posTo = -1, int height = -1);
	int	getBmpWidth(int bmpidx);
	void	ClearImage();
	CBitmap* CreateBmp( LPCTSTR sBMPFile);
	void	LoadBtn();
	void	SetBtnVisible();
	void	DrawXMark(CDC * dc);
	void	PopSetup();
	CString	GetSource();
	CString GetSelect();
	CString	GetDefine();
	void	SaveSelect();
	void	LoadItems();	
	CString Parser(CString &srcstr, CString substr = "\t");
	CString GetSymToStr(CString symbol);
	CString GetStrToSym(CString str);
	CString	Variant(int comm, CString data);
	void	Init();
	int		GetMaxlen();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg BOOL OnSelchange ( NMHDR * pNotifyStruct, LRESULT* result );
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetup();
};


