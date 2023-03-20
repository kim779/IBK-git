// fontSizeCombo.cpp : implementation file
//

#include "stdafx.h"
#include "fontSizeCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLOR_WHITE	RGB(255, 255, 255)
#define COLOR_DISABLE	RGB(212, 208, 201)
#define COLOR_PAL_BG	PALETTERGB(27, 79, 129);
#define COLOR_PAL_FG	PALETTERGB(255, 145, 2);
/////////////////////////////////////////////////////////////////////////////
// CfontSizeCombo

CfontSizeCombo::CfontSizeCombo()
{
	m_pFont = NULL;
	m_pParent = NULL;
	m_bEnable = TRUE;
}

CfontSizeCombo::~CfontSizeCombo()
{
}


BEGIN_MESSAGE_MAP(CfontSizeCombo, CComboBox)
	//{{AFX_MSG_MAP(CfontSizeCombo)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfontSizeCombo message handlers

int CfontSizeCombo::AddSize(int point, LONG height)
{
	CString	str;
	int	value, entry, count;

	if (height == 0)
	{
		CDC*	dc = GetDC();
		height = -MulDiv(point, dc->GetDeviceCaps(LOGPIXELSY), 72);
		ReleaseDC(dc);
	}

	str.Format("%d", point);
	count = GetCount();

	// we use positive height values for non-truetype fonts, negitive for true type
	if (height > 0)
	{
		for (entry = 0; entry < count; entry++)
		{
			value = (int)(height - GetHeight(entry));
			if (!value)
				return CB_ERR;
			if (value < 0)
				break;
		}
	}
	else
	{
		for (entry = 0; entry < count; entry++)
		{
			value = (int)(height - GetHeight(entry));
			if (!value)
				return CB_ERR;
			if (value > 0)
				break;
		}
	}

	if (entry == count)
		entry = -1;
	entry = InsertString(entry, str);
	if (entry != CB_ERR)
		SetItemData(entry, (DWORD)height);

	return entry;
}

LONG CfontSizeCombo::GetHeight(int sel)
{
	if (sel == -1)
		sel = GetCurSel();

	if (sel == -1)
	{
		CString	text;

		GetWindowText(text);
		sel = FindString(-1, text);
		if (sel == CB_ERR)
		{
			int	point = atoi(text);

			if (point)
			{
				CDC*	dc = GetDC();
				point = -MulDiv(point, dc->GetDeviceCaps(LOGPIXELSY), 72);
				ReleaseDC(dc);
				return point;
			}
			else
				sel = 0;
		}
	}

	return (LONG) GetItemData(sel);
}


void CfontSizeCombo::OnDestroy() 
{
	CWnd::OnDestroy();
}

BOOL CfontSizeCombo::Create(int dwStyle, CRect rect, CWnd* parent, UINT nID)
{
	//dwStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP;
	m_pParent = parent;
	return CComboBox::Create(dwStyle, rect, parent, nID);
}

void CfontSizeCombo::SetFont(CFont* pFont, bool redraw)
{
	m_pFont = pFont;
	CComboBox::SetFont(pFont, FALSE);

	if (redraw) RedrawWindow();
}

void CfontSizeCombo::OnPaint() 
{
	CPaintDC	dc(this);
	
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

void CfontSizeCombo::SetPalette(COLORREF clrFg, COLORREF clrBg)
{
	m_cbRGB = clrFg;
	m_csRGB = clrBg;
	Invalidate();
}

int CfontSizeCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

BOOL CfontSizeCombo::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CComboBox::OnEraseBkgnd(pDC);
}

void CfontSizeCombo::OnEnable(BOOL bEnable) 
{
	CComboBox::OnEnable(bEnable);
	m_bEnable = bEnable;
	Invalidate();
}
