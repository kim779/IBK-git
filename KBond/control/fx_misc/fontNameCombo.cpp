// fontNameCombo.cpp : implementation file
//

#include "stdafx.h"
#include "fontNameCombo.h"
#include <afxdlgs.h>
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define dxBITMAP        16
#define dyBITMAP        12

#define COLOR_WHITE	RGB(255, 255, 255)
#define COLOR_DISABLE	RGB(212, 208, 201)
#define COLOR_PAL_BG	PALETTERGB(27, 79, 129);
#define COLOR_PAL_FG	PALETTERGB(255, 145, 2);
/////////////////////////////////////////////////////////////////////////////
// CfontNameCombo

CfontNameCombo::CfontNameCombo()
{
	HINSTANCE hinst = AfxGetResourceHandle();
	AfxSetResourceHandle(::GetModuleHandle("fx_misc.dll"));
	m_loaded = m_bitmap.LoadBitmap(IDB_TRUETYPE);
	initMaskFromBitmap(&m_bitmap, &m_mask);
	AfxSetResourceHandle(hinst);

	m_pFont = NULL;
	m_pParent = NULL;
	m_bEnable = TRUE;
}

CfontNameCombo::~CfontNameCombo()
{
	if (m_loaded)
	{
		m_bitmap.DeleteObject();
		m_mask.DeleteObject();
	}
}


BEGIN_MESSAGE_MAP(CfontNameCombo, CComboBox)
	//{{AFX_MSG_MAP(CfontNameCombo)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfontNameCombo message handlers
void CfontNameCombo::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	ASSERT(lpDIS->CtlType == ODT_COMBOBOX);

	if (lpDIS->itemID == -1)
		return;

	int	x, y;
	CDC*	dc   = CDC::FromHandle(lpDIS->hDC);
	fontITEM* fI = (fontITEM *)lpDIS->itemData;
	LOGFONT*  lf = &fI->lf;
	COLORREF  crBk, crText;
	TEXTMETRIC tm;

	crBk   = dc->SetBkColor(GetSysColor(lpDIS->itemState & ODS_SELECTED ? COLOR_HIGHLIGHT : COLOR_WINDOW));
	crText = dc->SetTextColor(GetSysColor(lpDIS->itemState & ODS_SELECTED ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));

	dc->GetTextMetrics(&tm);
	x = LOWORD(GetDialogBaseUnits()) / 4;
	y = (lpDIS->rcItem.bottom + lpDIS->rcItem.top - tm.tmHeight) / 2;

	dc->ExtTextOut(lpDIS->rcItem.left + dxBITMAP + x, y, ETO_CLIPPED | ETO_OPAQUE,
			&lpDIS->rcItem, (LPCTSTR)lf->lfFaceName, lstrlen((LPCTSTR) lf->lfFaceName), NULL);

	dc->SetTextColor(crText);
	dc->SetBkColor(crBk);

	if (fI->dwType & TRUETYPE_FONTTYPE)
	{
		int	dy = ((lpDIS->rcItem.bottom - lpDIS->rcItem.top) - dxBITMAP) / 2;
		drawMaskedBitmap(dc, &m_bitmap, &m_mask, x, lpDIS->rcItem.top+dy, dxBITMAP, dyBITMAP);
	}

	if (lpDIS->itemState & ODS_FOCUS)
		dc->DrawFocusRect(&lpDIS->rcItem);
}

void CfontNameCombo::DeleteItem(LPDELETEITEMSTRUCT lpDIS) 
{
	if (lpDIS->itemID == -1)
		return;

	ASSERT(lpDIS->CtlType == ODT_COMBOBOX);

	fontITEM* fI = GetFontItem(lpDIS->itemID);
	ASSERT(fI);
	delete fI;
}

void CfontNameCombo::OnDropdown() 
{
	CRect	rect;
	int	width;

	GetWindowRect(&rect);
	width = (int)((float)rect.Width() * 1.6);
	SetDroppedWidth(width);
}

int CfontNameCombo::AddFont(LOGFONT* lf, DWORD type)
{
	int	entry;
	fontITEM* item = NULL;

	if (FindString(-1, (LPCTSTR)lf->lfFaceName) != CB_ERR)
		return CB_ERR;

	item = new fontITEM;
	item->lf     = *lf;
	item->dwType = type;

	entry = AddString((LPCTSTR)item->lf.lfFaceName);
	if (entry == CB_ERR)
		delete item;
	else
		SetItemData(entry, (DWORD)item);

	return entry;

}

CString CfontNameCombo::GetCurrentName()
{
	CString str;

	GetWindowText(str);
	return str;
}

fontITEM* CfontNameCombo::GetFontItem(int sel)
{
	if (sel == -1)
		sel = GetCurSel();

	if (sel == -1)
	{
		CString str;

		GetWindowText(str);
		sel = FindString(-1, str);
		if (sel == CB_ERR)
			sel = 0;
	}

	ASSERT(GetItemData(sel));
	return (fontITEM *)GetItemData(sel);
}

DWORD CfontNameCombo::GetFontType(int sel)
{
	return GetFontItem(sel)->dwType;
}

#define DSx     0x00660046L
#define DSna    0x00220326L

void CfontNameCombo::drawMaskedBitmap(CDC* dc, CBitmap* pbmp, CBitmap* pbmpMask, int x, int y, int cx, int cy)
{
	CDC	memDC;
	CBitmap* pBMP;
	COLORREF pBK   = dc->SetBkColor(RGB(255, 255, 255));
	COLORREF pTEXT = dc->SetTextColor(RGB(0, 0, 0));

	memDC.CreateCompatibleDC(dc);
	pBMP = memDC.SelectObject(pbmp);
	dc->BitBlt(x, y, cx, cy, &memDC, 0, 0, DSx);
	memDC.SelectObject(pbmpMask);
	dc->BitBlt(x, y, cx, cy, &memDC, 0, 0, DSna);
	memDC.SelectObject(pbmp);
	dc->BitBlt(x, y, cx, cy, &memDC, 0, 0, DSx);
	memDC.SelectObject(pBMP);

	dc->SetBkColor(pBK);
	dc->SetTextColor(pTEXT);
}

void CfontNameCombo::initMaskFromBitmap(CBitmap* pbmp, CBitmap* pbmpMask)
{
	BITMAP	bm;
	CDC	destDC, srcDC;
	CBitmap	*pDEST, *pSRC;
	COLORREF pBK;

	pbmp->GetObject(sizeof(BITMAP), &bm);
	pbmpMask->CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	destDC.CreateCompatibleDC(NULL);
	srcDC.CreateCompatibleDC(NULL);
	pDEST = destDC.SelectObject(pbmpMask);
	pSRC  = srcDC.SelectObject(pbmp);
	pBK   = srcDC.SetBkColor(srcDC.GetPixel(0, 0));
	destDC.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &srcDC, 0, 0, NOTSRCCOPY);
	srcDC.SetBkColor(pBK);

	destDC.SelectObject(pDEST);
	srcDC.SelectObject(pSRC);
}

void CfontNameCombo::OnDestroy() 
{
	CWnd::OnDestroy();
}

BOOL CfontNameCombo::Create(int dwStyle, CRect rect, CWnd* parent, UINT nID)
{
	//dwStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP;
	m_pParent = parent;
	return CComboBox::Create(dwStyle, rect, parent, nID);
}

void CfontNameCombo::SetFont(CFont* pFont, bool redraw)
{
	m_pFont = pFont;
	CComboBox::SetFont(pFont, FALSE);

	if (redraw) RedrawWindow();
}

void CfontNameCombo::OnPaint() 
{
	CPaintDC	dc(this); // device context for painting
	
	CDC		*pDC = &dc;
	
	COLORREF bg, fg;
	if (m_pParent != NULL)
	{
		bg = m_cbRGB;
		fg = m_csRGB;
	}
	else
	{
		bg = COLOR_PAL_BG;
		fg = COLOR_PAL_FG;
	}
 
	CRect rc, rcBtn, tRc;
	CPoint	pts[3];
	GetClientRect(&rc);
	if (m_bEnable == TRUE)
		pDC->FillSolidRect(rc, COLOR_WHITE);
	else
		pDC->FillSolidRect(rc, COLOR_DISABLE);
	pDC->Draw3dRect(rc, RGB(170, 170, 170), COLOR_WHITE);
	rcBtn = rc;
	rcBtn.DeflateRect(0, 3, 2, 3);
	rcBtn.left = rcBtn.right - rcBtn.Height();
	tRc = rcBtn;
	tRc.DeflateRect(1, 0);
	
	CPen *open, pen(PS_SOLID, 1, bg);
	CBrush *obr, brush(bg);
	open = pDC->SelectObject(&pen);
	obr = pDC->SelectObject(&brush);
	pDC->FillRect(tRc, &brush);

	pDC->MoveTo(rcBtn.left, rcBtn.top+1);
	pDC->LineTo(rcBtn.left, rcBtn.bottom-1);
	pDC->MoveTo(rcBtn.right-1, rcBtn.top+1);
	pDC->LineTo(rcBtn.right-1, rcBtn.bottom-1);

	CPen pen1(PS_SOLID, 1, fg);
	CBrush brush1(fg);
	pDC->SelectObject(&pen1);
	pDC->SelectObject(&brush1);

	tRc = rcBtn;
	tRc.DeflateRect(3, tRc.Height()*2/5);
	tRc.OffsetRect(1, 0);
	tRc.right = tRc.left + (tRc.Width()/2)*2-1;
	tRc.bottom = tRc.top + tRc.Width()/2;
	pts[0].y = tRc.bottom;
	pts[1].y = tRc.top;
	pts[2].y = tRc.top;
	pts[0].x = tRc.left + (tRc.Width()/2);
	pts[1].x = tRc.right-1;
	pts[2].x = tRc.left;
	pDC->Polygon(pts, 3);

	CString str = "";
	CFont *ofont;
	ofont = pDC->SelectObject(m_pFont);
	int index = this->GetCurSel();
	if (index != -1)
		this->GetLBText(index, str);
	rc.right = rcBtn.left;
	rc.DeflateRect(2, 1, 1, 1);
	if (m_bEnable == TRUE)
		pDC->FillSolidRect(rc, COLOR_WHITE);
	else
		pDC->FillSolidRect(rc, COLOR_DISABLE);

	pDC->DrawText(str, rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	pDC->SelectObject(ofont);
	pDC->SelectObject(open);
	pDC->SelectObject(obr);
}

void CfontNameCombo::SetPalette(COLORREF clrFg, COLORREF clrBg)
{
	m_cbRGB = clrFg;
	m_csRGB = clrBg;
	Invalidate();
}

int CfontNameCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (m_pFont == NULL)
	{
		m_pFont = (CFont*) new CFont;
		m_pFont->CreatePointFont(90, "±¼¸²");
	}

	SetFont(m_pFont);
	return 0;
}

BOOL CfontNameCombo::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CComboBox::OnEraseBkgnd(pDC);
}

void CfontNameCombo::OnEnable(BOOL bEnable) 
{
	CComboBox::OnEnable(bEnable);
	m_bEnable = bEnable;
	Invalidate();
	// TODO: Add your message handler code here
	
}
