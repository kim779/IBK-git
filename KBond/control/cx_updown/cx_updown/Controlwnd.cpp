// Controlwnd.cpp : implementation file
//

#include "stdafx.h"
#include "cx_updown.h"
#include "Controlwnd.h"

#include "UserSaveDlg.h"

#include "../../../h/axisfire.h"
#include "../../../h/axisvar.h"
#include "../../../axis/axMsg.hxx"

#define	WM_MSG	WM_USER+9000
#define	WM_CHILD	WM_USER+1

// CControlwnd

IMPLEMENT_DYNAMIC(CControlwnd, CWnd)

CControlwnd::CControlwnd()
{
	m_pParent = NULL;
	m_bCheck = false;
	m_bChecked = false;
	m_bDisable = false;
	m_capture = false;
	m_hBitmap = NULL;
	m_hBitmap_dn = NULL;
	m_hBitmap_hv = NULL;
	m_bTracking = FALSE;
	m_bHover = FALSE;
	
}

CControlwnd::~CControlwnd()
{

}


BEGIN_MESSAGE_MAP(CControlwnd, CWnd)
	ON_MESSAGE(WM_USER, OnUSER)
	ON_MESSAGE(WM_MSG, OnMSG)
	ON_MESSAGE(WM_CHILD, OnCHILD)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

LONG CControlwnd::OnUSER(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

LONG CControlwnd::OnMSG(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

LONG CControlwnd::OnCHILD(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

void CControlwnd::sendTR(CString trC, CString dat)
{
	if (!m_pParent)	
		return;

	int	len = L_userTH + dat.GetLength();
	char*	sndB = new char[len];
	struct _userTH* udat = (struct _userTH*) sndB;

	ZeroMemory(sndB, sizeof(sndB));
	memcpy(udat->trc, trC, trC.GetLength());
	udat->key = '0';	
	udat->stat = US_PASS;
	CopyMemory(&sndB[L_userTH], dat, dat.GetLength());
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, dat.GetLength()), (LPARAM)sndB);
	delete [] sndB;
}
// CControlwnd message handlers




void CControlwnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//if (m_capture)
	{
		CRect rc;
		m_capture = false;
		ReleaseCapture();
		GetClientRect(&rc);
		if (rc.PtInRect(point))
		{
			CUserSaveDlg Updlg(this);
			Updlg.DoModal();
		}
	}
	CWnd::OnLButtonUp(nFlags, point);
}


void CControlwnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	/*
	CRect rc;
	CString txt;
	GetClientRect(&rc);
	dc.FillSolidRect(rc, GetIndexColor(m_Param.pRGB));

	dc.SetBkMode(TRANSPARENT);

	if (m_hBitmap)
	{
		CDC		memDC;
		CBitmap		*pOldBitmap, *pBitmap;

		if (m_bCheck && m_bChecked && m_hBitmap_dn)
		{
			pBitmap = CBitmap::FromHandle(m_hBitmap_dn);
		}
		else if (m_capture && m_bHover && m_hBitmap_dn)
			pBitmap = CBitmap::FromHandle(m_hBitmap_dn);
		else if (m_bHover && m_hBitmap_hv)
			pBitmap = CBitmap::FromHandle(m_hBitmap_hv);
		else
			pBitmap = CBitmap::FromHandle(m_hBitmap);

		BITMAP stBitmapInfo ;
		pBitmap->GetBitmap(&stBitmapInfo);

		memDC.CreateCompatibleDC(&dc);	
		if (memDC.m_hDC != NULL) 
			pOldBitmap = (CBitmap*)memDC.SelectObject(pBitmap);

	//	::TransparentBlt(dc.m_hDC, 0, 0, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, stBitmapInfo.bmWidth, stBitmapInfo.bmHeight, 
	//		RGB(255, 0, 255));
		::StretchBlt(dc.m_hDC,rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC,0, 0, stBitmapInfo.bmWidth, stBitmapInfo.bmHeight, SRCCOPY);
			
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}

	if (!m_hBitmap)
		dc.DrawFrameControl(rc, DFC_BUTTON, DFCS_BUTTONPUSH | (m_capture && m_bHover?DFCS_PUSHED:NULL));
	*/
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	
}


void CControlwnd::SetButtonImages()
{
	CFileFind	finder;
	m_sImagePath.MakeLower();
	m_sImagePath.Replace(".bmp", "");

	m_sImagePath.Replace(".bmp", "");
	if (finder.FindFile(m_sImagePath + ".bmp"))
		m_hBitmap = getBitmap(m_sImagePath + ".bmp")->operator HBITMAP();
	else
		m_hBitmap = NULL;

	if (finder.FindFile(m_sImagePath + "_dn.bmp"))
			m_hBitmap_dn = getBitmap(m_sImagePath + "_dn.bmp")->operator HBITMAP();
	else
		m_hBitmap_dn = m_hBitmap;

	if (finder.FindFile(m_sImagePath + "_en.bmp"))
		m_hBitmap_hv = getBitmap(m_sImagePath + "_en.bmp")->operator HBITMAP();
	else
		m_hBitmap_hv = m_hBitmap;
}

int CControlwnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	SetButtonImages();
	return 0;
}

void CControlwnd::SetParam(_param *pParam)
{
	m_Param.key   = pParam->key;
	m_Param.name  = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect  = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB  = pParam->tRGB;
	m_Param.pRGB  = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	CString tmp = m_Param.options;
	m_sImagePath = OptionParser(tmp, "/i");

	
	m_sRoot = Variant(homeCC, "");
	
	int	offs = m_sImagePath.ReverseFind('\\');
	if (offs == -1)
		m_sImagePath = m_sRoot + "\\image\\" + m_sImagePath;
	else
		m_sImagePath = m_sRoot + "\\image\\" + m_sImagePath.Mid(offs + 1);
}


CString CControlwnd::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}

CString CControlwnd::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

CString CControlwnd::OptionParser(CString sOption, CString sKey)
{
	CString tmp = sOption;
	Parser(tmp, sKey);
	tmp = Parser(tmp, "/");
	return tmp;
}

CBitmap* CControlwnd::getBitmap(CString path)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

COLORREF CControlwnd::GetIndexColor(int index)
{
	return m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

void CControlwnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_capture)
		ReleaseCapture();
	SetCapture();
	m_capture = true;
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}

LRESULT CControlwnd::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	m_bHover=TRUE;
	Invalidate();
	return 0;
}

LRESULT CControlwnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover=FALSE;
	Invalidate();
	return 0;
}


void CControlwnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	
	CWnd::OnMouseMove(nFlags, point);
}


void CControlwnd::OnFinalRelease()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CWnd::OnFinalRelease();
}
