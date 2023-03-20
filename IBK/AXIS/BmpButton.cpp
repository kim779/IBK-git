// BmpButton.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "BmpButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TM_TIPS		100
/////////////////////////////////////////////////////////////////////////////
// CBmpButton

CBmpButton::CBmpButton(CFont* pFont/*=NULL*/)
{
	m_pFont = pFont;

	m_capture    = false;

	m_bTracking = FALSE;
	m_bHover    = FALSE;
	m_bCheck    = false;
	m_bChecked  = false;
	
	m_bFocus    = false;
	m_bDrawFocus = false;
	m_pTipCtrl  = NULL;
	m_clText    = RGB(0,0,0);

	m_bStretch  = true;

	m_bmp = NULL;
	m_statusCount = 3;
}

CBmpButton::~CBmpButton()
{
	ReleaseBitmap();
}

BEGIN_MESSAGE_MAP(CBmpButton, CWnd)
	//{{AFX_MSG_MAP(CBmpButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_DRAWITEM()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(BM_SETCHECK, OnBmSetCheck)
	ON_MESSAGE(BM_GETCHECK, OnBmGetCheck)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBmpButton message handlers
BOOL CBmpButton::Create(LPCTSTR caption, DWORD style, const RECT& rect, CWnd* parent, UINT nID, bool bCheck /*= FALSE*/, bool bStretch /*= FALSE*/)
{
	const DWORD dwStyle = WS_CHILD | WS_VISIBLE | style;
	m_bCheck = bCheck;
	//m_bStretch = bStretch;
	m_bStretch = true;
	const BOOL result = CWnd::Create(NULL, caption, dwStyle, rect, parent, nID);

	m_nID = nID;

	return result;
}

int CBmpButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetCursor(LoadCursor(AfxGetApp()->m_hInstance, IDC_CROSS));//Axis::handCursor);
	CreateTooltip();

	return 0;
}

void CBmpButton::OnDestroy() 
{
	if (m_pTipCtrl)
	{
		 m_pTipCtrl = NULL;
	}

	CWnd::OnDestroy();
}

void CBmpButton::SetTextColor(COLORREF clText)
{
	m_clText = clText;
	RedrawWindow();
}

void CBmpButton::SetImgBitmap(const char* fileName, int statusCount)
{
	if (statusCount < 1 || statusCount > 3)
		return;

	ReleaseBitmap();

	CString file(fileName);
	CString path;

	file.MakeUpper();
	if (file.Find(".BMP") < 0)
		path.Format("%s\\image\\%s.bmp", Axis::home, fileName);
	else
		path = file;

	m_bmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), path, 
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (m_bmp)
	{
		m_bImageSelfLoaded = TRUE;
		m_statusCount = statusCount;
		m_bmpSize = BitmapSize(m_bmp);
		if (::IsWindow(m_hWnd))
			FitImageSize();
	}
	else
	{
		TRACE("Load Failed Bitmap NAME = %s\n", fileName);
	}
}

void CBmpButton::SetImgBitmap(CBitmap* bmp, int statusCount)
{
	if (statusCount < 1 || statusCount > 3)
		return;

	ReleaseBitmap();

	m_bmp = (HBITMAP)bmp->m_hObject;

	m_statusCount = statusCount;
	m_bmpSize = BitmapSize(m_bmp);
	if (::IsWindow(m_hWnd))
		FitImageSize();
}

void CBmpButton::SetFont(CFont* pFont, BOOL redraw)
{
	m_pFont = pFont;
	if (redraw) Invalidate();
}

void CBmpButton::SetCheck(bool bCheck)
{
	m_bChecked = bCheck; 
	Invalidate();
}

void CBmpButton::SetDrawFocus(bool bDraw)
{
	m_bDrawFocus = bDraw;	Invalidate();
}

void CBmpButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc, rcFocus;
	CString txt;
	GetClientRect(&rc);
	rcFocus = rc;
	rcFocus.DeflateRect(2, 2);

	dc.SetBkMode(TRANSPARENT);

	if (m_bmp)
	{
		CDC		memDC;
		CBitmap* pOldBitmap{}, * pBitmap{};

		pBitmap = CBitmap::FromHandle(m_bmp);
		int bmpOffset = 0;

		if (m_bCheck && m_bChecked)
			bmpOffset = GetBitmapOffset(BMBS_DOWN);
		else if (m_capture && m_bHover)
			bmpOffset = GetBitmapOffset(BMBS_DOWN);
		else if (m_bHover)
			bmpOffset = GetBitmapOffset(BMBS_HOVER);

		memDC.CreateCompatibleDC(&dc);	
		if (memDC.m_hDC != NULL) 
			pOldBitmap = (CBitmap*)memDC.SelectObject(pBitmap);

		dc.BitBlt(0, 0, m_bmpSize.cx, m_bmpSize.cy, &memDC, bmpOffset, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
	else
		dc.DrawFrameControl(rc, DFC_BUTTON, DFCS_BUTTONPUSH | (m_capture && m_bHover ? DFCS_PUSHED:NULL));
	
	CGdiObject* pOldFont{};
	if (m_pFont != NULL) pOldFont = dc.SelectObject(m_pFont);
	else pOldFont = dc.SelectStockObject(SYSTEM_FONT);

	/**

	GetWindowText(txt); 
	
	if (m_hBitmap && !m_bStretch)
	{
		rc.right = BitmapSize(m_hBitmap).cx;
		rc.bottom = BitmapSize(m_hBitmap).cy;
	}

	if (m_capture && m_bHover)
		rc.OffsetRect(1, 1);
	
	if (IsWindowEnabled())
	{
		dc.SetTextColor(m_clText);
	}
	else
	{
		dc.SetTextColor(RGB(255, 255, 255));
		rc.OffsetRect(1, 1);
		dc.DrawText(txt, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		rc.OffsetRect(-1, -1);
		dc.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	}
	dc.DrawText(txt, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	if (m_bDrawFocus && m_bFocus)
		dc.DrawFocusRect(rcFocus);

	dc.SelectObject(pOldFont);
	*/
}

void CBmpButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	if (m_capture)
		ReleaseCapture();
	SetCapture();
	m_capture = true;
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}

void CBmpButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (true || !m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CBmpButton::OnMouseHover(WPARAM wparam, LPARAM lparam)   //vc2019 void->LRESULT
{
	m_bHover = TRUE; 
	Invalidate();
	return 0;
}


LRESULT CBmpButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover=FALSE;

	Invalidate(); 
	return 0;
}

void CBmpButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_capture)
		ReleaseCapture();

	SetCapture();
	m_capture = true;
	Invalidate();
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CBmpButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_capture)
	{
		CRect rc;
		m_capture = false;
		ReleaseCapture();
		GetClientRect(&rc);
		if (rc.PtInRect(point))
		{
			const DWORD style = GetStyle();
			const BOOL autoRadio = (style & BS_AUTORADIOBUTTON) && (style & BS_PUSHLIKE);
			if (autoRadio)
			{
				if (!m_bCheck)
					m_bCheck = TRUE;
				
				m_bChecked = TRUE;

				CWnd* child = GetParent()->GetWindow(GW_CHILD);
				
				char className[MAX_PATH];
				::GetClassName(m_hWnd, className, MAX_PATH);
				while (child)
				{
					char buffer[MAX_PATH];
					GetClassName(child->m_hWnd, buffer, MAX_PATH);

					if (child->m_hWnd != m_hWnd && strcmp(className, buffer) == 0)
					{
						CString tmp;
						child->GetWindowText(tmp);
						//MessageBox(tmp);
						tmp.TrimLeft(); tmp.TrimRight();
						if (tmp == "")
						{
							child = child->GetWindow(GW_HWNDNEXT);
							continue;
						}
						//GetWindowText(child->m_hWnd, (LPTSTR) &tmp, 255);
						child->SendMessage(BM_SETCHECK, 0);
					}
					
					child = child->GetWindow(GW_HWNDNEXT);
				}
			}
			else
			{
				if (m_bCheck)
					m_bCheck = FALSE;
			}

			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
		}
		Invalidate();
	}
	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CBmpButton::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CWnd::OnEraseBkgnd(pDC);
}

CSize CBmpButton::BitmapSize(HBITMAP hBitmap)
{
	if (!hBitmap)
		return CSize(0, 0);

	BITMAP BM;
	GetObject(hBitmap, sizeof(BITMAP), &BM);

	return CSize((int)BM.bmWidth / m_statusCount, (int)BM.bmHeight);
}

void CBmpButton::OnEnable(BOOL bEnable) 
{
	CWnd::OnEnable(bEnable);
	
	Invalidate();	
}

bool CBmpButton::IsChecked()
{
	return m_bChecked;
}

LRESULT CBmpButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	MSG msg;
	msg.hwnd = m_hWnd;
	msg.message = message;
	msg.wParam = wParam;
	msg.lParam = lParam;

	if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
	{
		if (m_pTipCtrl)
			m_pTipCtrl->RelayEvent(&msg);
	}
	else if (message == WM_KEYUP && (wParam == VK_RETURN || wParam == VK_SPACE) && m_bDrawFocus)
	{
		if (m_bCheck)
			m_bChecked = !m_bChecked;
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

CString CBmpButton::Parser(CString &srcstr, CString substr)
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


void CBmpButton::FitImageSize()
{
	if (m_bmp)
	{
		SetWindowPos(NULL, 0, 0, m_bmpSize.cx, m_bmpSize.cy, SWP_NOZORDER|SWP_NOMOVE);
	}
}

void CBmpButton::CreateTooltip()
{
	CString txt;
	GetWindowText(txt);

	if (txt.Find("\n") == -1)
		return;

	SetWindowText(Parser(txt, "\n"));
	
	CRect rc;
	GetClientRect(&rc);
	//m_pTipCtrl = new CToolTipCtrl;
	m_pTipCtrl = std::make_unique< CToolTipCtrl>();
	m_pTipCtrl->Create(this);
	m_pTipCtrl->AddTool(this, txt, &rc, GetDlgCtrlID());
	m_pTipCtrl->Activate(TRUE);
}

void CBmpButton::PreSubclassWindow() 
{
	if (!m_pTipCtrl)
		CreateTooltip();	
	CWnd::PreSubclassWindow();
}

void CBmpButton::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	m_bFocus = true;
	if (m_bDrawFocus)	Invalidate();
}

void CBmpButton::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
	m_bFocus = false;	
	if (m_bDrawFocus)	Invalidate();
}

LRESULT CBmpButton::OnBmGetCheck(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)m_bChecked;
}

LRESULT CBmpButton::OnBmSetCheck(WPARAM wParam, LPARAM lParam)
{
	m_bChecked = wParam != 0;
	if (wParam && !m_bCheck)
		m_bCheck = TRUE;
	Invalidate();
	return 0;
}

BOOL CBmpButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	//2013.01.18 �����
	//��õ�޴� ��ư ��Ȱ��ȭ
	if(m_nID != 12345)//IDC_BTN_LOGO
	{
		SetCursor(Axis::handCursor);	
	}

	return TRUE;
}

void CBmpButton::ReleaseBitmap()
{
	if (m_bImageSelfLoaded && m_bmp)
		DeleteObject(m_bmp);

	m_bmp = NULL;
	m_statusCount = 3;
	m_bmpSize = CSize(0, 0);
	m_bImageSelfLoaded = FALSE;
}

int CBmpButton::GetBitmapOffset(int status)
{
	switch (m_statusCount) 
	{
	case 2:
		if (status == BMBS_DOWN)
			return m_bmpSize.cx;
		else if (status == BMBS_HOVER)
			return 0;
	case 3:
		if (status == BMBS_DOWN)
			return m_bmpSize.cx * 2;
		else if (status == BMBS_HOVER)
			return m_bmpSize.cx;
	}

	return 0;
}

BOOL CBmpButton::IsEnable()
{
	return IsWindowEnabled();
}

void CBmpButton::SetEnable(BOOL enable)
{
	EnableWindow(enable);
}

void CBmpButton::SetCheckButton(BOOL check)
{
	m_bCheck = check != 0;
}

BOOL CBmpButton::IsCheckButton()
{
	return m_bCheck;
}
