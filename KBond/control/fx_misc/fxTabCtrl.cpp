#include "stdafx.h"
#include "fxTabCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	padding		3
#define	edge		20

CfxTabCtrl::CfxTabCtrl()
{
	m_pImageList = NULL;
	m_clrBack = ::GetSysColor(COLOR_3DFACE);
	m_clrUnSel = lighter(GetSysColor(COLOR_3DFACE), 1.3f);
	m_clrSel = darker(GetSysColor(COLOR_3DFACE), 0.9f);
	m_clrFontUnSel = RGB(100, 100, 100);
	m_clrFontSel = RGB(0, 0, 0);
	m_bBorder = false;
	m_bTriangle = false;

	m_bImgBtn = false;
	m_pFont = NULL;
}

CfxTabCtrl::~CfxTabCtrl()
{
	if (m_pImageList)
	{
		m_pImageList->DeleteImageList();
		delete m_pImageList;
	}
}


BEGIN_MESSAGE_MAP(CfxTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CfxTabCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CfxTabCtrl::Create(CRect rect, CWnd* pParentWnd, UINT nID)
{
	DWORD dwStyle = WS_CHILD | WS_VISIBLE;
	m_parent = pParentWnd;
	return CTabCtrl::Create(dwStyle, rect, pParentWnd, nID);
}


BOOL CfxTabCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	CRect rClient, rTab, rTotalTab, rBkgnd;
	COLORREF clrBack;
	int nTab, nTabHeight = 0;

	CTabCtrl::OnEraseBkgnd(pDC);

	GetClientRect(rClient);

	nTab = GetItemCount();
	rTotalTab.SetRectEmpty();

	while (nTab--)
	{
		GetItemRect(nTab, rTab);
		rTotalTab.UnionRect(rTab, rTotalTab);
	}

	nTabHeight = rTotalTab.Height();
	clrBack = m_clrBack;
	
	rBkgnd = rClient;
	rBkgnd.bottom = rBkgnd.top + (nTabHeight + 2);
	pDC->SetBkColor(clrBack);
	pDC->ExtTextOut(rBkgnd.left, rBkgnd.top, ETO_CLIPPED | ETO_OPAQUE, rBkgnd, "", NULL);

	return TRUE;
}

void CfxTabCtrl::LoadImg(CString path)
{
	m_bImgBtn = true;
	m_hBmp[L_STAB] = (HBITMAP)::LoadImage (NULL, path + "L_SGRAYTAB.BMP",
		IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_hBmp[C_STAB] = (HBITMAP)::LoadImage (NULL, path + "SGRAYTAB.BMP",
		IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_hBmp[R_STAB] = (HBITMAP)::LoadImage (NULL, path + "R_SGRAYTAB.BMP",
		IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_hBmp[L_TAB] = (HBITMAP)::LoadImage (NULL, path + "L_GRAYTAB.BMP",
		IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_hBmp[C_TAB] = (HBITMAP)::LoadImage (NULL, path + "GRAYTAB.BMP",
		IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_hBmp[R_TAB] = (HBITMAP)::LoadImage (NULL, path + "R_GRAYTAB.BMP",
		IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_hBmp[TAB_BG] = (HBITMAP)::LoadImage (NULL, path + "B_GRAYTAB.BMP",
		IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
}

void CfxTabCtrl::OnPaint() 
{
	CPaintDC xdc(this);
	CMemDC dc(&xdc);

	CRect rRc;
	GetClientRect(rRc);

	if(m_bImgBtn)
	{
		drawImgBG(&dc, rRc);
		
		DRAWITEMSTRUCT dis;
		dis.CtlType = ODT_TAB;
		dis.CtlID = GetDlgCtrlID();
		dis.hwndItem = GetSafeHwnd();
		dis.hDC = dc.GetSafeHdc();
		dis.itemAction = ODA_DRAWENTIRE;

		int nTab = GetItemCount();
		int nSel = GetCurSel();

		if (!nTab)
			return;

		while (nTab--)
		{
			if (nTab != nSel)
			{
				dis.itemID = nTab;
				dis.itemState = 0;
				VERIFY(GetItemRect(nTab, &dis.rcItem));
				dis.rcItem.top = 0;
				drawImgBtn(&dc, &dis);
			}
		}
		// now selected tab
		dis.itemID = nSel;
		dis.itemState = ODS_SELECTED;
		VERIFY(GetItemRect(nSel, &dis.rcItem));
		dis.rcItem.top = 0;
		drawImgBtn(&dc, &dis);
		

		CRect rPage, sRect;
		sRect = dis.rcItem;
		
	
		//main
		GetClientRect(&dis.rcItem);
		rPage = dis.rcItem;
		AdjustRect(FALSE, rPage);
		dis.rcItem.top = rPage.top - 2;
		dc.FillSolidRect(&dis.rcItem, RGB(250, 250, 250));
	}
	else
	{
		CBrush brush;
		brush.CreateSolidBrush(m_clrBack);
		dc.FillRect(&rRc, &brush);
		brush.DeleteObject();

		DRAWITEMSTRUCT dis;
		dis.CtlType = ODT_TAB;
		dis.CtlID = GetDlgCtrlID();
		dis.hwndItem = GetSafeHwnd();
		dis.hDC = dc.GetSafeHdc();
		dis.itemAction = ODA_DRAWENTIRE;

	
		CRect rClient, rPage;

		// paint the tabs first and then the borders
		int nTab = GetItemCount();
		int nSel = GetCurSel();

		if (!nTab)
			return;

		while (nTab--)
		{
			if (nTab != nSel)
			{
				dis.itemID = nTab;
				dis.itemState = 0;
				VERIFY(GetItemRect(nTab, &dis.rcItem));
				dis.rcItem.bottom -= 2;
				drawFlatItem(&dis);
			}
		}

		// now selected tab
		CRect sRect;
		dis.itemID = nSel;
		dis.itemState = ODS_SELECTED;
		VERIFY(GetItemRect(nSel, &dis.rcItem));
		dis.rcItem.top = dis.rcItem.top -2;
		dis.rcItem.bottom -= 2;
		sRect = dis.rcItem;
		drawFlatItem(&dis);

		// main
		GetClientRect(&dis.rcItem);
		rPage = dis.rcItem;
		AdjustRect(FALSE, rPage);
		dis.rcItem.top = rPage.top - 2;
		drawMainBorder(&dis, sRect);
	}
}

void CfxTabCtrl::drawImgBG(CMemDC* pDC, CRect rRc)
{
	CDC  ImgMemDC;
	CBitmap Bitmap, *pOldBitmap;
	BITMAP bm;

	ImgMemDC.CreateCompatibleDC(pDC);
	Bitmap.Attach(m_hBmp[TAB_BG]);
	Bitmap.GetObject(sizeof(bm), &bm);

	pOldBitmap = ImgMemDC.SelectObject(&Bitmap);
	pDC->SetStretchBltMode(COLORONCOLOR);
	pDC->StretchBlt(rRc.left, rRc.top, rRc.Width(), bm.bmHeight, &ImgMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	ImgMemDC.SelectObject(pOldBitmap);
	Bitmap.Detach();
	Bitmap.DeleteObject();
}

void CfxTabCtrl::drawImgBtn(CMemDC* pDC, LPDRAWITEMSTRUCT lpdis)
{
	CDC  ImgMemDC;
	CBitmap BitmapL, BitmapC, BitmapR, *pOldBitmap;
	BITMAP bmL, bmC, bmR;
	CRect rRc = lpdis->rcItem;

	if(lpdis->itemState == ODS_SELECTED)
	{
		BitmapL.Attach(m_hBmp[L_STAB]);
		BitmapC.Attach(m_hBmp[C_STAB]);
		BitmapR.Attach(m_hBmp[R_STAB]);
	}
	else
	{
		BitmapL.Attach(m_hBmp[L_TAB]);
		BitmapC.Attach(m_hBmp[C_TAB]);
		BitmapR.Attach(m_hBmp[R_TAB]);
	}
	ImgMemDC.CreateCompatibleDC(pDC);
	//BitmapL.Attach(m_hBmp[TAB_BG]);
	pDC->SetStretchBltMode(COLORONCOLOR);

	BitmapL.GetObject(sizeof(bmL), &bmL);
	BitmapC.GetObject(sizeof(bmC), &bmC);
	BitmapR.GetObject(sizeof(bmR), &bmR);

	pOldBitmap = ImgMemDC.SelectObject(&BitmapL);
	pDC->StretchBlt(rRc.left, rRc.top, bmL.bmWidth, bmL.bmHeight, &ImgMemDC, 0, 0, bmL.bmWidth, bmL.bmHeight, SRCCOPY);

	ImgMemDC.SelectObject(&BitmapC);
	pDC->StretchBlt(rRc.left + bmL.bmWidth, rRc.top, rRc.Width() - bmL.bmWidth - bmR.bmWidth, bmC.bmHeight, &ImgMemDC, 0, 0, bmC.bmWidth, bmC.bmHeight, SRCCOPY);

	ImgMemDC.SelectObject(&BitmapR);
	pDC->StretchBlt(rRc.right - bmR.bmWidth, rRc.top, bmR.bmWidth, bmR.bmHeight, &ImgMemDC, 0, 0, bmR.bmWidth, bmR.bmHeight, SRCCOPY);


	CFont	font, *pOldFont;
	// set font
	LOGFONT lf;
	if (m_pFont)
		m_pFont->GetLogFont(&lf);
	else
	{
		if (GetParent()->GetFont())
			GetParent()->GetFont()->GetLogFont(&lf);
		else
		{
			lf = getLogFont(9, NULL);
			lf.lfHeight = -12;
		}
	}

	lf.lfWeight = (GetCurSel() == (int)lpdis->itemID ? FW_SEMIBOLD : FW_NORMAL);
	font.CreateFontIndirect(&lf);
	pOldFont = (CFont*)pDC->SelectObject(&font);

	TC_ITEM     tci;
	CString sTemp;
	tci.mask        = TCIF_TEXT | TCIF_IMAGE;
	tci.pszText     = sTemp.GetBuffer(100);
	tci.cchTextMax  = 99;
	GetItem(lpdis->itemID, &tci);
	sTemp.ReleaseBuffer();
	pDC->SetBkMode( TRANSPARENT );
	rRc.top +=4;
	
	rRc.left-=3;
	pDC->DrawText(sTemp, rRc, DT_NOPREFIX | DT_CENTER);

	ImgMemDC.SelectObject(pOldBitmap);
	BitmapL.Detach();
	BitmapL.DeleteObject();
	BitmapC.Detach();
	BitmapC.DeleteObject();
	BitmapR.Detach();
	BitmapR.DeleteObject();

	pDC->SelectObject(pOldFont);
	font.DeleteObject();
}

void CfxTabCtrl::drawFlatItem(LPDRAWITEMSTRUCT lpdis)
{
	TC_ITEM     tci;
	CDC* pDC = CDC::FromHandle(lpdis->hDC);

	BOOL bSelected = (lpdis->itemID == (UINT)GetCurSel());
	
	CRect rItem(lpdis->rcItem);
	if (lpdis->rcItem.left < 0)
		return;		
	
	rItem.bottom += 2;
	rItem.right -= 1;
	
	COLORREF crFrom = getMainColor(bSelected);
	COLORREF crShadow = darker(crFrom, 0.75f);
	COLORREF crRight = darker(RGB(255, 255, 255), 0.88f);
	
	CBrush brush, tbrush, *pBrush;
	CPen pen, spen, upen, rpen, tpen, *pPen;
	brush.CreateSolidBrush(crFrom);
	tbrush.CreateSolidBrush(RGB(255, 0, 0));
	pen.CreatePen(PS_SOLID, 1, crFrom);
	spen.CreatePen(PS_SOLID, 1, crShadow);
	upen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	rpen.CreatePen(PS_SOLID, 1, crRight);
	tpen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	pBrush = pDC->SelectObject(&brush);
	pPen = pDC->SelectObject(&pen);

	int tGap = 5;
	pDC->SetPolyFillMode(ALTERNATE);

	// tab 모양
	
	CPoint pt[5];
	pt[0].x = rItem.left-1;
	pt[0].y = rItem.top;
	pt[1].x = rItem.left-1;
	pt[1].y = rItem.bottom;
	pt[2].x = rItem.right-2;
	pt[2].y = rItem.bottom;
	pt[3].x = rItem.right-2;
	pt[3].y = rItem.top+tGap;
	pt[4].x = rItem.right-tGap-2;
	pt[4].y = rItem.top;
	pDC->Polygon(pt, 5);

  	// tab 윗부분 line
	pDC->SelectObject(&spen);
	pDC->MoveTo(rItem.left-1, rItem.bottom+1);
	pDC->LineTo(rItem.left-1, rItem.top);
	pDC->LineTo(rItem.right-tGap-2, rItem.top);
	pDC->LineTo(rItem.right-2, rItem.top+tGap);
	pDC->LineTo(rItem.right-2, rItem.bottom+1);

	// 입체
	if (false && bSelected)
	{
		pDC->SelectObject(&rpen);
		pDC->MoveTo(rItem.right-3, rItem.top+tGap+1);
		pDC->LineTo(rItem.right-3, rItem.bottom+1);
		
		pDC->SelectObject(&upen);
		pDC->MoveTo(rItem.right-tGap-2, rItem.top+1);
		pDC->LineTo(rItem.right-2, rItem.top+tGap+1);
		pDC->MoveTo(rItem.right-tGap-3, rItem.top+1);
		pDC->LineTo(rItem.right-2, rItem.top+tGap+2);
	}
	
	// 강조
	if (bSelected && m_bTriangle)
	{
		pDC->SelectObject(&tpen);
		pDC->SelectObject(&tbrush);
		CPoint tpt[4];
		tpt[0].x = rItem.right-tGap-1;
		tpt[0].y = rItem.top;
		tpt[3] = tpt[0];
		tpt[1].x = rItem.right-2;
		tpt[1].y = rItem.top;
		tpt[2].x = rItem.right-2;
		tpt[2].y = rItem.top+tGap;
		pDC->Polygon(tpt, 4);
	}
	
	pDC->SelectObject(pBrush);
	pDC->SelectObject(pPen);
	pen.DeleteObject();
	spen.DeleteObject();
	upen.DeleteObject();
	rpen.DeleteObject();
	tpen.DeleteObject();
	brush.DeleteObject();
	tbrush.DeleteObject();

	// text & icon
	rItem.left += padding;
	rItem.top += padding + (bSelected ? 1 : 0);

	pDC->SetBkMode(TRANSPARENT);

	CString sTemp;
	tci.mask        = TCIF_TEXT | TCIF_IMAGE;
	tci.pszText     = sTemp.GetBuffer(100);
	tci.cchTextMax  = 99;
	GetItem(lpdis->itemID, &tci);
	sTemp.ReleaseBuffer();

	if (m_pImageList) 
	{
		IMAGEINFO info;
		CRect ImageRect;
		CRect rect = lpdis->rcItem;
		rect.left += pDC->GetTextExtent(_T(" ")).cx;

		m_pImageList->GetImageInfo(tci.iImage, &info);
		ImageRect = info.rcImage;
		int nYpos = rect.top + 2;

		m_pImageList->Draw(pDC, tci.iImage, CPoint(rect.left, nYpos), ILD_TRANSPARENT);
		rItem.left += 16 + padding;
	}

	CFont	font, *pOldFont;
	// set font
	LOGFONT lf;
	if (m_pFont)
		m_pFont->GetLogFont(&lf);
	else
	{
		if (GetParent()->GetFont())
			GetParent()->GetFont()->GetLogFont(&lf);
		else
		{
			lf = getLogFont(9, NULL);
			lf.lfHeight = -12;
		}
	}

	lf.lfWeight = (GetCurSel() == (int)lpdis->itemID ? FW_SEMIBOLD : FW_NORMAL);
	font.CreateFontIndirect(&lf);
	pOldFont = (CFont*)pDC->SelectObject(&font);
	
	// text
	rItem.right -= padding;
	formatText(sTemp, pDC, rItem.Width());

	pDC->SetTextColor(getTabTextColor(bSelected));
	pDC->DrawText(sTemp, rItem, DT_NOPREFIX | DT_CENTER);
	
	pDC->SelectObject(pOldFont);
	font.DeleteObject();
	ReleaseDC(pDC);
}

COLORREF CfxTabCtrl::getMainColor(BOOL bSelected)
{
	if (bSelected)
		return m_clrSel;
	else
		return m_clrUnSel;
	
	return ::GetSysColor(COLOR_3DFACE);
}

COLORREF CfxTabCtrl::getTabColor(BOOL bSelected)
{
	if (!bSelected)
		return (m_clrBack == -1) ? ::GetSysColor(COLOR_3DFACE) : m_clrBack;
	return ::GetSysColor(COLOR_3DFACE);
}

COLORREF CfxTabCtrl::getTabTextColor(BOOL bSelected)
{
	if (bSelected)
		return m_clrFontSel;
	else
		return m_clrFontUnSel;
}

COLORREF CfxTabCtrl::darker(COLORREF crBase, float fFactor)
{
	ASSERT (fFactor < 1.0f && fFactor > 0.0f);

	fFactor = min(fFactor, 1.0f);
	fFactor = max(fFactor, 0.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedShadow, bBlueShadow, bGreenShadow;

	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);

	bRedShadow = (BYTE)(bRed * fFactor);
	bBlueShadow = (BYTE)(bBlue * fFactor);
	bGreenShadow = (BYTE)(bGreen * fFactor);

	return RGB(bRedShadow, bGreenShadow, bBlueShadow);
}

COLORREF CfxTabCtrl::lighter(COLORREF crBase, float fFactor) 
{
	ASSERT (fFactor > 1.0f);

	fFactor = max(fFactor, 1.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedHilite, bBlueHilite, bGreenHilite;

	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);

	bRedHilite = (BYTE)min((int)(bRed * fFactor), 255);
	bBlueHilite = (BYTE)min((int)(bBlue * fFactor), 255);
	bGreenHilite = (BYTE)min((int)(bGreen * fFactor), 255);

	return RGB(bRedHilite, bGreenHilite, bBlueHilite);
}

CSize CfxTabCtrl::formatText(CString& sText, CDC* pDC, int nWidth)
{
	CRect rect(0, 0, nWidth, 20);
	UINT uFlags = DT_CALCRECT|DT_SINGLELINE;//|DT_MODIFYSTRING|DT_END_ELLIPSIS;

	::DrawText(pDC->GetSafeHdc(), sText.GetBuffer(sText.GetLength() + 4), -1, rect, uFlags);
	sText.ReleaseBuffer();

	return pDC->GetTextExtent(sText);
}

void CfxTabCtrl::drawMainBorder(LPDRAWITEMSTRUCT lpdis, CRect sRect)
{
	CRect rBorder(lpdis->rcItem);
	CDC* pDC = CDC::FromHandle(lpdis->hDC); 
	rBorder.DeflateRect(1, 1);
	CBrush brush;
	COLORREF clrBack = getMainColor(true);
	brush.CreateSolidBrush(clrBack);
	pDC->FillRect(&rBorder, &brush);
	brush.DeleteObject();

	if (m_bBorder)
	{
		COLORREF crFrom = m_clrSel;
		COLORREF crShadow = darker(crFrom, 0.75f);
		CPen mpen, spen, *pPen;
		mpen.CreatePen(PS_SOLID, 1, crFrom);
		spen.CreatePen(PS_SOLID, 1, crShadow);
		pPen = pDC->SelectObject(&spen);
		
		pDC->MoveTo(rBorder.left, rBorder.top);
		pDC->LineTo(rBorder.left, rBorder.bottom);
		pDC->LineTo(rBorder.right, rBorder.bottom);
		pDC->LineTo(rBorder.right, rBorder.top);
		pDC->LineTo(rBorder.left, rBorder.top);
		
		pDC->SelectObject(&mpen);
		pDC->MoveTo(sRect.left, rBorder.top);
		pDC->LineTo(sRect.right-3, rBorder.top);
		
		pDC->SelectObject(pPen);
		spen.DeleteObject();
		mpen.DeleteObject();
	}
}

void CfxTabCtrl::SetTabImage(UINT bmpID)
{
	m_pImageList = new CImageList();
	m_pImageList->Create(bmpID, 16, 1, RGB(255, 0, 255)); 
	SetImageList(m_pImageList); 
}


void CfxTabCtrl::SetColor(COLORREF clrUnSel, COLORREF clrFontUnSel, 
			  COLORREF clrSel, COLORREF clrFontSel)
{
	m_clrUnSel = clrUnSel;
	m_clrSel = clrSel;
	m_clrFontSel = clrFontSel;
	m_clrFontUnSel = clrFontUnSel;
	if (GetSafeHwnd())
		Invalidate();
}

void CfxTabCtrl::SetBkColor(COLORREF clrBack)
{
	m_clrBack = clrBack;
	if (GetSafeHwnd())
		Invalidate();
}

int CfxTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	ModifyStyle(0, TCS_OWNERDRAWFIXED);
	
	return 0;
}


void CfxTabCtrl::SetFont(CFont* pFont, bool redraw)
{
	m_pFont = pFont;
	CTabCtrl::SetFont(pFont, FALSE);

	if (redraw)	Invalidate();
}


void CfxTabCtrl::SetViewBorder(bool bBorder, bool redraw)
{
	m_bBorder = bBorder;
	if (redraw)
		Invalidate();
}

LOGFONT CfxTabCtrl::getLogFont(int fsize, DWORD weight)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = fsize * 10; 
	
	strcpy(lf.lfFaceName, "굴림체");
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	lf.lfWeight = weight;
	lf.lfItalic = FALSE;
	return lf;
}