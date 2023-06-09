// CustMenu.h: interface for the CMenuXP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUXP_H__8FC060B7_2454_11D5_99BD_5254AB339987__INCLUDED_)
#define AFX_CUSTMENU_H__8FC060B7_2454_11D5_99BD_5254AB339987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////////////////////////////////////////////////////////////////////////////////
//	CMenuXP is a class derived from CMenu, using ownerdraw technology.
//	I named it MenuXP because I once expected it to be like the menus
//	in OfficeXP and WindowsXP, but I failed to accomplish it. The big
//	difficulty is I cannot convert the 3D border of the menu into flat.
//	So I giveup. And I hope it still usefull to you.
//	
//	I construt the class from scribble, but some of the drawing code is copy from the class
//	CCoolMenuManager. I also use a class named CBCGKeyHelper from BCGControlBar to show
//	accelerator key text.
//
//	Usage:
//	Seee Example
//
//	Features:
//	1. Menu with icons, like in office 97
//	2. A sidebar in any level of the popup menu
//	3. Support button style menuitem, it appear in some drawing toolbars of office suite
//	4. All colors and font and size can be customized
//
//	Use it as you like. bug report and improvement is welcome.
//	
//	Created:	10/24/2001
//	Author:		Yao Zhifeng			yzf_nuaa@sina.com
//	
////////////////////////////////////////////////////////////////////////////////////////////
//The ownerdraw data
class CMenuXPItem
{
public:
	DWORD		m_dwMagicNum;		// A magic number to distingush our data
	DWORD		m_dwID;			// Menu ID
	bool		m_bSeparator;		// Separator
	bool		m_bSideBar;		// A gradient sidebar
	bool		m_bButtonOnly;		// Button only style item
	CString		m_strText;		// Menu item text
	HICON		m_hIcon;		// Menu icon
	int		m_nSize;		// Height of the item(Width of the sidebar if m_bSideBar is true)

public:
	CMenuXPItem() 
	{
		m_dwMagicNum	= 0x0505a0a0;
		m_dwID		= 0;
		m_bSeparator	= false;
		m_bSideBar	= false;
		m_bButtonOnly	= false;
		m_hIcon		= NULL;
		m_nSize		= 16;
	};

	virtual ~CMenuXPItem()
	{
		// icon handle은 하나만 생성해서 상위에서 삭제 하는걸로.
		//if (m_hIcon)
		//	::DestroyIcon(m_hIcon);
	}

	BOOL	IsMyData(void) { return m_dwMagicNum == 0x0505a0a0; };
};

///////////////////////////////////////////////////////////////////////////////////////////////
//	For convenient, derive some class from CMenuXPItem, 
//	and do the initialization in constructor
class CMenuXPText : public CMenuXPItem	//Normal item with text and an optional icon
{
public:
	CMenuXPText(DWORD dwID, LPCTSTR strText, HICON hIcon = NULL) : CMenuXPItem()
	{
		m_dwID		= dwID;
		m_strText	= strText;
		m_hIcon		= hIcon;
	}
};

class CMenuXPSeparator : public CMenuXPItem //A separator item
{
public:
	CMenuXPSeparator() : CMenuXPItem()
	{
		m_bSeparator = true;
	}
};

class CMenuXPSideBar : public CMenuXPItem //A gradient sidebar
{
public:
	CMenuXPSideBar(int nSize = 32, LPCTSTR strText = NULL, HICON hIcon=NULL, DWORD dwID=0) : CMenuXPItem()
	{
		m_dwID		= dwID;
		m_bSideBar	= true;
		m_strText	= strText;
		m_hIcon		= hIcon;
		m_nSize		= nSize;
		m_dwID		= dwID;
	}
};

class CMenuXPButton : public CMenuXPItem // A button only item
{
public:
	CMenuXPButton(DWORD dwID, HICON hIcon = NULL) : CMenuXPItem()
	{
		m_dwID		= dwID;
		m_bButtonOnly	= true;
		m_hIcon		= hIcon;
	}
};

////////////////////////////////////////////////////////////////////////////////////////
// Class CMenuXP, an ownerdraw menu
class AFX_EXT_CLASS CMenuXP : public CMenu  
{
	DECLARE_DYNAMIC(CMenuXP)

public:
	CMenuXP();
	virtual ~CMenuXP();

	//Menu style(Default: STYLE_OFFICE)
	typedef	enum 
	{
		STYLE_OFFICE,		// Draw a float button around the icon
		STYLE_STARTMENU,	// show selected bar below the icon
		STYLE_XP		// use different color for the icon area
	} MENUSTYLE;

	//Below is the functions to build the menu
	BOOL	AddSideBar(CMenuXPSideBar *pItem);
	BOOL	AppendODMenu(UINT nFlags, CMenuXPItem *pItem, ACCEL *pAccel=0);
	BOOL	AppendSeparator(void);	
	BOOL	AppendODPopup(UINT nFlags, CMenuXP *pPopup, CMenuXPItem *pItem);
	void	Break(void);	// change a column(the next item added will be in a new column)
	void	BreakBar(void);	// change a column with a break line(same as Break, except that a break line is drawn between two columns)
	//	add define function
	BOOL	InsertMenuX(UINT nPosition, UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL, HICON hIcon = NULL);
	BOOL	AppendMenuX(UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL, HICON hIcon = NULL);

protected:
	CFont		m_fontMenu;
	CPen		m_penBorder;
	COLORREF	m_clrBackGround;	// Background color
	COLORREF	m_clrSelectedBar;	// selected bar color
	COLORREF	m_clrText;		// Text color
	COLORREF	m_clrSelectedText;	// selected text color
	COLORREF	m_clrDisabledText;	// disabled text color

	COLORREF	m_clrSideBarStart;	// Start color of the gradient sidebar
	COLORREF	m_clrSideBarEnd;	// end color of the gradient sidebar
	COLORREF	m_clrIconArea;		// Background color of the button(icon) area
	COLORREF	m_clrBorder;		// Background color of the button(icon) area

	BOOL		m_bBreak;		// if true, next item inserted into the menu will be added with the sytle MF_MENUBREAK
	BOOL		m_bBreakBar;		// if true, next item inserted into the menu will be added with the sytle MF_MENUBARBREAK

	HBITMAP		m_hBitmap;		// Background bitmap
	CDC		m_memDC;		// Memory dc holding the background bitmap

	MENUSTYLE	m_Style;		// menu style(currently support office or startmenu style)
public:	//User these functions to change the default attribute of the menu
	void	SetBackColor(COLORREF clr)		{ m_clrBackGround = clr; }
	void	SetSelectedBarColor(COLORREF clr)	{ m_clrSelectedBar = clr; }
	void	SetTextColor(COLORREF clr)		{ m_clrText = clr; }
	void	SetSelectedTextColor(COLORREF clr)	{ m_clrSelectedText = clr; }
	void	SetDisabledTextColor(COLORREF clr)	{ m_clrDisabledText = clr; }
	void	SetSideBarStartColor(COLORREF clr)	{ m_clrSideBarStart = clr; }
	void	SetSideBarEndColor(COLORREF clr)	{ m_clrSideBarEnd = clr; }
	void	SetIconAreaColor(COLORREF clr)		{ m_clrIconArea = clr; }
	void	SetBorderColor(COLORREF clr)		{ m_clrBorder = clr; }
	void	SetBackBitmap(HBITMAP hBmp);

	void	SetMenuStyle(MENUSTYLE style)		{ m_Style = style; }
	BOOL	SetMenuFont(LOGFONT lgfont);

	BOOL	TrackPopupMenu(UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect=NULL);

	//Find the popupmenu from a menuitem id, you may not need it
	CMenuXP	*FindSubMenuFromID(DWORD dwID);

public:
	void	Clear(bool bSubMenu = true);	//Clean all memory and handles
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
	static	LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);

protected:
	virtual void DrawBackGround(CDC *pDC, CRect rect, BOOL bSelected, BOOL bDisabled);
	virtual void DrawButton(CDC *pDC, CRect rect, BOOL bSelected, BOOL bDisabled, BOOL bChecked);
	virtual void DrawIcon(CDC *pDC, CRect rect, HICON hIcon, BOOL bSelected, BOOL bDisabled, BOOL bChecked);
	virtual void DrawSideBar(CDC *pDC, CRect rect, HICON hIcon, CString strText);
	virtual void DrawText(CDC *pDC, CRect rect, CString strText, BOOL bSelected, BOOL bDisabled, BOOL bBold);
	virtual void DrawCheckMark(CDC *pDC, CRect rect, BOOL bSelected);
	virtual void DrawMenuText(CDC& dc, CRect rc, CString text, COLORREF color);
	virtual void DrawIconArea(CDC *pDC, CRect rect, BOOL bSelected, BOOL bDisabled, BOOL bChecked);
	virtual void DrawSeparator(CDC* pDC, CRect rect);


	//helpers 
	HBITMAP		CreateGradientBMP(HDC hDC,COLORREF cl1,COLORREF cl2,int nWidth,int nHeight,int nDir,int nNumColors);
	void		DrawEmbossed(CDC *pDC, HICON hIcon, CRect rect, BOOL bColor = FALSE, BOOL bShadow = FALSE);
	void		FillRect(CDC *pDC, const CRect& rc, COLORREF color);
};

#endif // !defined(AFX_MENUXP_H__8FC060B7_2454_11D5_99BD_5254AB339987__INCLUDED_)
