// fxLineStyleCombo.cpp : implementation file
//

#include "stdafx.h"
#include "fxLineStyleCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLOR_WHITE	RGB(255, 255, 255)
#define COLOR_DISABLE	RGB(212, 208, 201)
#define COLOR_PAL_BG	PALETTERGB(27, 79, 129);
#define COLOR_PAL_FG	PALETTERGB(255, 145, 2);

#define	maxSTYLE	4
int	lineStyle[maxSTYLE] = {
	PS_SOLID,
	PS_DASH,
	PS_DOT,
	PS_DASHDOT,
};
/////////////////////////////////////////////////////////////////////////////
// CfxLineStyleCombo

CfxLineStyleCombo::CfxLineStyleCombo()
{
	loadPen();
	m_pFont = NULL;
	m_pParent = NULL;
	m_bEnable = TRUE;
}

CfxLineStyleCombo::~CfxLineStyleCombo()
{
	removePen();
}


BEGIN_MESSAGE_MAP(CfxLineStyleCombo, CComboBox)
	//{{AFX_MSG_MAP(CfxLineStyleCombo)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfxLineStyleCombo message handlers

void CfxLineStyleCombo::PreSubclassWindow() 
{
	for (int ii = 0; ii < m_pens.GetSize(); ii++)
		AddString(_T(""));
	
	SetCurSel(0);
	CComboBox::PreSubclassWindow();
}

void CfxLineStyleCombo::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (m_pens.GetSize() <= 0)
		return;

	CDC	dc;
	CRect	rect;
	int	y;
	CPen*	ppen;

	dc.Attach(lpDrawItemStruct->hDC);
	rect.CopyRect(&lpDrawItemStruct->rcItem);
	dc.FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));

	rect.DeflateRect(2, 2);
	ppen = dc.SelectObject((CPen *)m_pens.GetAt(lpDrawItemStruct->itemID));
	y = rect.top + rect.Height()/2;
	dc.MoveTo(rect.left, y);
	dc.LineTo(rect.right, y);
	dc.SelectObject(ppen);

	rect.InflateRect(2, 2);
	dc.DrawEdge(rect, BDR_SUNKENINNER, BF_RECT);
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) && (lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		rect.DeflateRect(1, 1);
		dc.DrawFocusRect(rect);
	}

	dc.Detach();
}

int CfxLineStyleCombo::GetStyle()
{
	int	index = GetCurSel();
	if (index == CB_ERR || index >= m_pens.GetSize())
		return PS_SOLID;

	return lineStyle[index];
}

void CfxLineStyleCombo::SetStyle(int style)
{
	for (int ii = 0; ii < maxSTYLE; ii++)
	{
		if (lineStyle[ii] == style)
		{
			SetCurSel(ii);
			return;
		}
	}

	SetCurSel(0);
}

void CfxLineStyleCombo::loadPen()
{
	CPen*	pen;
	
	m_pens.RemoveAll();
	for (int ii = 0; ii < maxSTYLE; ii++)
	{
		pen = new CPen;
		pen->CreatePen(lineStyle[ii], 1, RGB(0,0,0));
		m_pens.Add(pen);
	}
}

void CfxLineStyleCombo::removePen()
{
	CPen*	pen;

	for (int ii = 0; ii < maxSTYLE; ii++)
	{
		pen = (CPen *)m_pens.GetAt(ii);
		pen->DeleteObject();
		delete pen;
	}
	m_pens.RemoveAll();
}


void CfxLineStyleCombo::OnDestroy() 
{
	CWnd::OnDestroy();
}

BOOL CfxLineStyleCombo::Create(int dwStyle, CRect rect, CWnd* parent, UINT nID)
{
	//dwStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP;
	m_pParent = parent;
	return CComboBox::Create(dwStyle, rect, parent, nID);
}

void CfxLineStyleCombo::SetFont(CFont* pFont, bool redraw)
{
	m_pFont = pFont;
	CComboBox::SetFont(pFont, FALSE);

	if (redraw) RedrawWindow();
}

void CfxLineStyleCombo::OnPaint() 
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

void CfxLineStyleCombo::SetPalette(COLORREF clrFg, COLORREF clrBg)
{
	m_cbRGB = clrFg;
	m_csRGB = clrBg;
	Invalidate();
}

int CfxLineStyleCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

BOOL CfxLineStyleCombo::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CComboBox::OnEraseBkgnd(pDC);
}

void CfxLineStyleCombo::OnEnable(BOOL bEnable) 
{
	CComboBox::OnEnable(bEnable);
	m_bEnable = bEnable;
	Invalidate();
}
