// XCaption.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "XCaption.h"
#include "axMisc.h"
#include "axMsg.hxx"
#include "../dll/sm/MenuXP.h"

#include <algorithm>
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//using std::_cpp_max;  //vc2019 �ּ�
//using std::_cpp_min;  //vc2019 �ּ�

#define	GAP	2
#define INFOSTRING	 "[�� ����������:1544-5550]"

class CStaticCmdUI : public CCmdUI      
{
public: 
	virtual void SetText(LPCTSTR lpszText);
};

void CStaticCmdUI::SetText(LPCTSTR lpszText)
{
	CXCaption* pCaption = (CXCaption*)m_pOther;
	ASSERT(pCaption != NULL);

	pCaption->SetText(lpszText);
}

/////////////////////////////////////////////////////////////////////////////
// CXCaption

CXCaption::CXCaption()
{
	m_cyBottomBorder= m_cyTopBorder = 4;	// l&r default to 6; t&b was 1
	m_title		= "IBK";		// must have some text for CalcFixedLayout
	m_activeCap	= TRUE;
	m_syscom =  m_func = 0;
	m_ToolTip	= NULL;
	m_mouseOver	= false;
	m_single	= false;
	m_sdiMode	= false;
	m_bIconic	= false;
	m_pinON		= false;
	m_bMax		= false;
	m_mainframe	= NULL;

	//m_hideflag	= SHOW_ALL;
	m_hideflag = HIDE_HELP;

	m_hoverI	= -1;
	m_downI		= -1;
	m_action	= -1;
	m_vsN		= 0;

	for (int ii = 0; ii < MBMP_CNT; ii++)
		m_arRc.Add(CRect(0, 0, 0, 0));

	m_gradientDraw = TRUE;

	LoadBitmap();

	Initialize();
}

CXCaption::~CXCaption()
{
	m_arRc.RemoveAll();

	Release();
}


BEGIN_MESSAGE_MAP(CXCaption, CControlBar)
	//{{AFX_MSG_MAP(CXCaption)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXCaption message handlers

int CXCaption::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CXCaption::OnDestroy() 
{
	if (m_ToolTip)
	{
		if (m_ToolTip->m_hWnd)
		{
			m_ToolTip->Activate(FALSE);
			m_ToolTip->DelTool(this);
#ifndef DF_USE_CPLUS17
			m_ToolTip->RelayEvent(nullptr);
#endif
		}
#ifndef DF_USE_CPLUS17
		delete m_ToolTip;
#endif
	}

	CControlBar::OnDestroy();
}

void CXCaption::LoadBitmap()
{
	m_capAct = Axis::GetSkinBitmap("CAP_ACT");
	m_capSideAct = Axis::GetBitmap("SFRAME_SIDE");

	m_capInact = Axis::GetBitmap("XN_FRAME_MIDDLE");	
	m_capSideInact = Axis::GetBitmap("XN_FRAME_SIDE");

	if (m_capAct)
	{
		m_gradientDraw = FALSE;
		BITMAP bm;
		m_capAct->GetBitmap(&bm);
		m_szCaptionBitmap.cx = bm.bmWidth;
		m_szCaptionBitmap.cy = bm.bmHeight;
	}
	else {
		m_gradientDraw = TRUE;
		m_szCaptionBitmap = CSize(0, 0);
	}
}

void CXChildCaption::LoadBitmap()
{
	m_capAct = Axis::GetSkinBitmap("CAP_ACT");
	m_capSideAct = Axis::GetSkinBitmap("CHILD_SFRAME_SIDE");

	m_capInact = Axis::GetBitmap("XN_FRAME_MIDDLE");	
	m_capSideInact = Axis::GetBitmap("XN_FRAME_SIDE");

	if (m_capAct)
	{
		m_gradientDraw = FALSE;
		BITMAP bm;
		m_capAct->GetBitmap(&bm);
		m_szCaptionBitmap.cx = bm.bmWidth;
		m_szCaptionBitmap.cy = bm.bmHeight;
	}
	else {
		m_gradientDraw = TRUE;
		m_szCaptionBitmap = CSize(0, 0);
	}
}


void CXCaption::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_downI = -1;
	const int nItem = GetSelectItem(point);
	if (nItem == -1)
	{
		m_bNcLDown = true;
		
		GetParent()->SetFocus();

		/**
		if (!m_bIconic)
			GetParent()->ModifyStyle(NULL, WS_CAPTION);
		**/
		GetParent()->SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		if (!m_bIconic)
			GetParent()->ModifyStyle(WS_CAPTION, NULL);
		return;
	}
	else
	{
		CClientDC	dc(this);
		CRect	rc = m_arRc.GetAt(nItem);

		OnLButtonDown(&dc, rc, nItem);
	}
	CControlBar::OnLButtonDown(nFlags, point);
}


void CXCaption::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bNcLDown)
	{
		GetParent()->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOMOVE|SWP_DRAWFRAME);
		m_bNcLDown = false;
	}
	if (m_downI != -1)
	{
		ReleaseCapture();
		const int nItem = GetSelectItem(point);
		if (nItem == m_downI)
		{ 
			m_action = nItem;
			m_mainframe->PostMessage(m_nMsg, MAKEWPARAM(axCAPTION, (int) m_action), m_key);
		}
		else	
			m_action = -1;

		CClientDC	dc(this);
		CRect		rc;
		if (m_bMax && IDX_RESTORE == m_downI)
			rc = m_arRc.GetAt(IDX_MAX);
		else	
			rc = m_arRc.GetAt(m_downI);
		DrawBitmap(&dc, m_downI, rc);
	}

	m_action = -1;
	m_downI = -1;
	CControlBar::OnLButtonUp(nFlags, point);
}

void CXCaption::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	WINDOWPLACEMENT	pl;
	GetParent()->GetWindowPlacement(&pl);

	const int nItem = GetSelectItem(point);
	OnLButtonDblClk(nItem);

	CControlBar::OnLButtonDblClk(nFlags, point);
}

LRESULT CXCaption::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_mouseOver	= false;
	m_hoverI	= -1;
	return 0;
}

void CXCaption::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd*			wndUnderMouse = NULL;
	const CWnd*			wndActive = this;
	TRACKMOUSEEVENT		csTME{};

	CControlBar::OnMouseMove(nFlags, point);

	const CPoint	lpoint = point;
	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);
	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if (!m_mouseOver)
		{
			m_mouseOver = true;

			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);
		}

		HoverItem(lpoint);
	}
	else 
		m_mouseOver = false;
}

void CXCaption::OnPaint()
{
	CPaintDC	dc(this);

	DrawTitle(&dc);

	CDC		memDC;
	CRect		cRc, iRc, tRc, clipRc;
	CRgn		clipRgn;
	int		round{}, x{}, y{}, width{}, height{};

	GetClientRect(cRc);
	tRc.CopyRect(&cRc);

	round	= cRc.Height()/2;
	x	= cRc.left;
	y	= cRc.top;
	width	= cRc.Width();
	height	= cRc.Height();

#if _MARK_
	int	mainkey = 0;
	if (!m_activeCap && m_mainframe)
		mainkey = m_mainframe->SendMessage(WM_AXIS, MAKEWPARAM(axGetActiveKey, 0));
#endif 

	CAxDrawHelper painter(&memDC);
	
	if (memDC.CreateCompatibleDC(&dc))
	{
		CBitmap	bitmap;
		bitmap.CreateCompatibleBitmap(&dc, cRc.Width(), cRc.Height());
		CBitmap* pbmp = (CBitmap *) memDC.SelectObject(&bitmap);

		if (m_sdiMode)
			iRc = m_arRc.GetAt(IDX_PIN);
		else
		{
			if (m_single)
				iRc = m_arRc.GetAt(IDX_PIN);
			else
				iRc = m_arRc.GetAt(m_bIconic ? IDX_MIN : IDX_SINGLE);

			if (iRc.Width() == 0)
				iRc = m_arRc.GetAt(IDX_MIN);
		}

		if (m_gradientDraw)
		{
			if (m_activeCap)
			{
				painter.DrawGradient(cRc, m_aVSColor, m_aVEColor);

				clipRc.CopyRect(cRc);
				clipRc.DeflateRect(2, 2);
				clipRgn.CreateRoundRectRgn(clipRc.left, clipRc.top, clipRc.right, clipRc.bottom, round, round);
				memDC.SelectClipRgn(&clipRgn, RGN_COPY);

				x	= clipRc.left;
				y	= clipRc.top;
				width	= iRc.left + iRc.Height()/2 - clipRc.left;
				height	= clipRc.Height();

				painter.DrawGradient(CRect(clipRc.left, clipRc.top, width, height), m_aHSColor, m_aHEColor, true);
				
				memDC.SelectClipRgn(NULL);
				clipRgn.DeleteObject();
			}
			else
			{			
	#if _MARK_TITLE
				if (mainkey == m_key)
					painter.DrawGradient(cRc, m_mVSColor, m_mVEColor);
				else
					painter.DrawGradient(cRc, m_iVSColor, m_iVEColor);
	#else
				painter.DrawGradient(cRc, m_iVSColor, m_iVEColor);
	#endif
				clipRc.CopyRect(cRc);
				clipRc.DeflateRect(2, 2);
				clipRgn.CreateRoundRectRgn(clipRc.left, clipRc.top, clipRc.right, clipRc.bottom, round, round);
				memDC.SelectClipRgn(&clipRgn, RGN_COPY);

				x	= clipRc.left;
				y	= clipRc.top;
				width	= iRc.left + iRc.Height()/2 - clipRc.left;
				height	= clipRc.Height();
	#if _MARK_TITLE
				if (mainkey == m_key)
					painter.DrawGradient(CRect(clipRc.left, clipRc.top, width, height), m_mHSColor, m_mHEColor, true);
				else
					painter.DrawGradient(CRect(clipRc.left, clipRc.top, width, height), m_iHSColor, m_iHEColor, true);
	#else
					painter.DrawGradient(CRect(clipRc.left, clipRc.top, width, height), m_iHSColor, m_iHEColor, true);
	#endif			
				memDC.SelectClipRgn(NULL);
				clipRgn.DeleteObject();
			}
		}
		else //** bitmap draw
		{
			if (m_capAct) {
				CDC tempDC;
				tempDC.CreateCompatibleDC(&memDC);
				CBitmap* oldBitmap = tempDC.SelectObject(m_activeCap ? m_capAct: m_capInact);

				const UINT margin = 0;

				iRc = CRect(margin, margin, cRc.Width() - margin * 2, cRc.Height() - margin * 2);
				memDC.StretchBlt(iRc.left, iRc.top, iRc.Width(), iRc.Height(), 
					&tempDC, 0, 0, m_szCaptionBitmap.cx, m_szCaptionBitmap.cy, SRCCOPY);

				tempDC.SelectObject(oldBitmap);
			}
		}

// draw icon
		iRc = m_arRc.GetAt(IDX_ICON);
		if (iRc.IsRectEmpty())
		{
			SetRegion();
			iRc = m_arRc.GetAt(IDX_ICON);
		}
		DrawBitmap(&memDC, IDX_ICON, iRc);
		//DrawIconEx(memDC.m_hDC, iRc.left, iRc.top, ResourceHelper()->m_hIcon,
		//	iRc.Width(), iRc.Height(), 0, NULL, DI_NORMAL|DI_COMPAT);

// draw system button
		iRc = m_arRc.GetAt(IDX_CLOSE);	
		DrawBitmap(&memDC, IDX_CLOSE, iRc);

		iRc = m_arRc.GetAt(IDX_MAX);	
		DrawBitmap(&memDC, m_bMax ? IDX_RESTORE : IDX_MAX, iRc);

		iRc = m_arRc.GetAt(IDX_MIN);	
		DrawBitmap(&memDC, m_bIconic ? IDX_RESTORE : IDX_MIN, iRc);

// draw user button
		if (!m_bIconic)
		{
			iRc = m_arRc.GetAt(IDX_HELP);	
			DrawBitmap(&memDC, IDX_HELP, iRc);
			iRc = m_arRc.GetAt(IDX_COPY);	
			DrawBitmap(&memDC, IDX_COPY, iRc);

			if (!(m_hideflag & HIDE_FONT))
			{
				iRc = m_arRc.GetAt(IDX_FONT);
				DrawBitmap(&memDC, IDX_FONT, iRc);
			}

			iRc = m_arRc.GetAt(IDX_GROUP);	
			DrawBitmap(&memDC, IDX_GROUP, iRc);

			if (m_sdiMode)
			{
				iRc = m_arRc.GetAt(IDX_PIN);
				DrawBitmap(&memDC, IDX_PIN, iRc);
			}
			else
			{
				iRc = m_arRc.GetAt(IDX_SINGLE);
				DrawBitmap(&memDC, IDX_SINGLE, iRc);
				if (m_single)
				{
					iRc = m_arRc.GetAt(IDX_PIN);
					DrawBitmap(&memDC, IDX_PIN, iRc);
				}
			}
		}

// draw text
		tRc.left  = m_arRc.GetAt(IDX_ICON).right + GAP;
		if (m_sdiMode)
			tRc.right = m_arRc.GetAt(IDX_PIN).left - GAP;
		else
		{
			if (m_single)
				tRc.right = m_arRc.GetAt(IDX_PIN).left - GAP;
			else
				tRc.right = m_arRc.GetAt(m_bIconic ? IDX_MIN : IDX_SINGLE).left - GAP;
		}

		CFont*	font = memDC.SelectObject(ResourceHelper()->GetFont());

		if (m_activeCap)
			memDC.SetTextColor(RGB(255, 255, 255));//::GetSysColor(COLOR_CAPTIONTEXT));
		else	
		{
#if _MARK_TEXT
			if (mainkey == m_key)
				memDC.SetTextColor(RGB(255, 255, 0));//::GetSysColor(COLOR_INACTIVECAPTIONTEXT));
			else
				memDC.SetTextColor(Axis::GetColor(35));//RGB(192, 192, 192));//::GetSysColor(COLOR_INACTIVECAPTIONTEXT));
#else
			memDC.SetTextColor(Axis::GetColor(35));//RGB(192, 192, 192));//::GetSysColor(COLOR_INACTIVECAPTIONTEXT));
#endif
		}
	
		memDC.SetBkMode(TRANSPARENT);
		if (tRc.right < tRc.left)
		{
			CRect	rect;
			rect = m_arRc.GetAt(IDX_MIN);
			tRc.right = rect.left;
		}
		//��Ÿ üũ���� �ʰ� ����.(12/04/25)
		if (Axis::devMode)
		{
			CRect r;
			GetParent()->GetWindowRect(r);
			memDC.DrawText(Format("%s (%d x %d)", m_title, r.Width(), r.Height()), tRc, 
				DT_VCENTER | DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS);
		}
		else
		{
			memDC.DrawText(m_title, tRc, DT_VCENTER|DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);
		}

		memDC.SelectObject(font);

		dc.BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pbmp);

		memDC.DeleteDC();
		bitmap.DeleteObject();
	}
}


LRESULT CXCaption::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		{
			if (!m_ToolTip)
				break;
			MSG	msg{};
			POINT	point;
			
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);

			msg.hwnd = m_hWnd;
			msg.message = message;
			msg.wParam = wParam;
			msg.lParam = lParam;
			msg.pt = point;
		
			m_ToolTip->RelayEvent(&msg);
		}
		break;
	default:
		break;
	}	
	return CControlBar::WindowProc(message, wParam, lParam);
}

LRESULT CXCaption::OnSizeParent(WPARAM, LPARAM lParam)
{
	const AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;
	return CControlBar::OnSizeParent(0, lParam);
}

void CXCaption::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CStaticCmdUI state;
	state.m_pOther = this;
	state.m_nIndexMax = 1;		// there's only one thing to update
	state.m_nID = AFX_IDW_STATUS_BAR;

	// allow the statusbar itself to have update handlers
	if (CWnd::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
		return;

	// allow target (owner) to handle the remaining updates
	state.DoUpdate(pTarget, FALSE);
}

CSize CXCaption::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	// The Y value is the sum of the calculated height from DrawText,
	// plus the top and bottom border.

	return OnCalcFixedLayout(bStretch, bHorz);
}

void CXCaption::SetText(LPCTSTR lpszNew)
{
	ASSERT(lpszNew && AfxIsValidString(lpszNew));
	if (lpszNew == nullptr)
		return;

	if (m_title != lpszNew)
	{
		m_title = lpszNew;
		Invalidate();
	}
}

#if _MARK_TITLE
COLORREF CXCaption::GetMColor(COLORREF color1, COLORREF color2)
{
	int	r = (GetRValue(color1) + GetRValue(color2))/2; 
	int	g = (GetGValue(color1) + GetGValue(color2))/2;
	int	b = (GetBValue(color1) + GetBValue(color2))/2;
	r -= 10, r = abs(r);
	g -= 10, g = abs(g);
	b -= 10, b = abs(b);
	return RGB(r, g, b);
}
#endif

void CXCaption::ChangeSkin()
{
	m_aVSColor = ResourceHelper()->GetColor(ACTTITLE_VSBK);
	m_aVEColor = ResourceHelper()->GetColor(ACTTITLE_VEBK);
	m_aHSColor = ResourceHelper()->GetColor(ACTTITLE_HSBK);
	m_aHEColor = ResourceHelper()->GetColor(ACTTITLE_HEBK);

	m_iVSColor = ResourceHelper()->GetColor(INACTTITLE_VSBK);
	m_iVEColor = ResourceHelper()->GetColor(INACTTITLE_VEBK);
	m_iHSColor = ResourceHelper()->GetColor(INACTTITLE_HSBK);
	m_iHEColor = ResourceHelper()->GetColor(INACTTITLE_HEBK);
#if _MARK_TITLE
	m_mVSColor = GetMColor(m_aVSColor, m_iVSColor);
	m_mVEColor = GetMColor(m_aVEColor, m_iVEColor);
	m_mHSColor = GetMColor(m_aHSColor, m_iHSColor);
	m_mHEColor = GetMColor(m_aHEColor, m_iHEColor);
#endif

	LoadBitmap();
}

void CXCaption::ChangeVS(int vsN)
{
	m_vsN = vsN;
	Invalidate(FALSE);
}

int CXCaption::GetLuminosity(COLORREF color) const
{
	const int HlsMax = 240;	// This is what Display Properties uses
	const int RgbMax = 255;	// max r/g/b value is 255

	const int r = GetRValue(color);
	const int g = GetGValue(color);
	const int b = GetBValue(color);
	const int rgbMax = max(max(r, g), b); //vc2019 _MAX -> max
	const int rgbMin = min(min(r, g), b);   //vc2019 _MIN -> min
	return (((rgbMax + rgbMin) * HlsMax) + RgbMax ) / (2 * RgbMax);
}

int CXCaption::GetSysCommand(CPoint point)
{
	CRect	sRc, cRc;

	GetClientRect(cRc);
 	const int	width = cRc.Height() - GAP*2;
	
	sRc.SetRect(cRc.right - (width+GAP), cRc.top + GAP, cRc.right - GAP, cRc.bottom - GAP);
	for (int ii = 3; ii > 0; ii--)
	{
		if (sRc.PtInRect(point))
			return ii;
		sRc.OffsetRect(-(width+GAP), 0);
	}

	sRc.SetRect(GAP, GAP, GAP + width, GAP + width);
	if (sRc.PtInRect(point))
	{
		// getsystemmenu display
		return -1;
	}
	return 0;
}

void CXCaption::SetToolTipText(CString text, BOOL bActivate)
{
	if (text.IsEmpty())	return;

	InitToolTip();
	if (m_ToolTip->GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip->AddTool(this, text, rectBtn, 1);
		 m_ToolTip->SetDelayTime(100);
	}

	m_ToolTip->UpdateTipText(text, this, 1);
	m_ToolTip->Activate(bActivate);
	//int delaytime = m_ToolTip->GetDelayTime(TTDT_AUTOPOP);
}

void CXCaption::InitToolTip()
{
	if (!m_ToolTip)
	{
#ifdef DF_USE_CPLUS17
		m_ToolTip = std::make_unique<CToolTipCtrl>();
#else
		m_ToolTip = new CToolTipCtrl();
#endif
		m_ToolTip->Create(this, TTS_ALWAYSTIP);
		m_ToolTip->Activate(TRUE);
	}
}

void CXCaption::HoverItem(CPoint point)
{
	const int nItem = GetSelectItem(point);
	if (nItem == m_hoverI)	return;
	m_hoverI = nItem;

	CString	tipS = GetItemTipText(nItem);

	if (tipS.IsEmpty())	return;
	SetToolTipText(tipS);
	
	CRect	rc;
	if (m_bMax && IDX_RESTORE == nItem)
		rc = m_arRc.GetAt(IDX_MAX);
	else	rc = m_arRc.GetAt(nItem);
	m_ToolTip->SetToolRect(this, 1, rc);
}

void CXCaption::SetFunc(int index, bool on)
{
	if (index >= FUNCNO)	return;
//	m_funcONOFF[index] = on;
}

void CXCaption::SetGroup(int group)
{
	m_groupN = group;
	//	redraw group
	Invalidate();
}

void CXCaption::SetMisc(CWnd* mainframe, UINT nMsg, int key, int fontsize, bool sdi)
{
	m_mainframe	= mainframe;
	m_nMsg		= nMsg;
	m_key		= key;
	m_fontSize	= fontsize;
	m_sdiMode	= sdi;

	ChangeSkin();
	SetRegion();

	FocusNotify();
}

void CXCaption::SetSDI()
{
	m_sdiMode = true;
	m_single  = false;
	SetRegion();
	Invalidate(FALSE);
}

int CXCaption::ChangeFont()
{
#ifdef DF_USE_CPLUS17
	std::unique_ptr<CMenuXP> menu = std::make_unique<CMenuXP>();
#else
	CMenuXP *menu = new CMenuXP;
#endif
	CString	str;

	menu->CreatePopupMenu();
	const int	base = 25000;
	
	menu->AppendMenuX(MF_STRING, base + INITMAPSIZE, "�⺻ũ��");
	menu->AppendSeparator();
	for (int ii = 8; ii < 13; ii++)
	{
		str.Format("%d", ii);
		switch(ii)
		{
		case 8 :
			str = "(" + str +") "+" �����۰�";
			break;
		case 9 :
			str = "(" + str +") "+" �⺻ũ��";
			break;
		case 10 :
			str = "(" + str +")"+" ����ũ��";
			break;
		case 11 :
			str =  "(" + str +")"+" ���� �� ũ��";
			break;
		case 12 :
			str =  "(" + str +")"+" ���� ũ��";
			break;

		}
		menu->AppendMenuX(MF_STRING, base + ii, str);
		if (ii == m_fontSize)
			menu->CheckMenuItem(base + ii, MF_CHECKED);
	}
	

	const UINT	nFlags = TPM_RETURNCMD|TPM_LEFTALIGN|TPM_RIGHTBUTTON;
	const CRect	rect   = m_arRc.GetAt(IDX_FONT);
	CPoint	point  = CPoint(rect.left, rect.bottom);
	ClientToScreen(&point);
	const int	comm = menu->TrackPopupMenu(nFlags, point.x, point.y + GAP, this);
	if (comm > base)
	{
		if (comm - base != INITMAPSIZE)
			m_fontSize = comm - base;
	}
#ifndef DF_USE_CPLUS17
	delete menu;
#endif
	return comm - base;
}

int CXCaption::ChangeGroup()
{
#ifdef DF_USE_CPLUS17
	std::unique_ptr<CMenuXP> menu = std::make_unique<CMenuXP>();
#else
	CMenuXP *menu = new CMenuXP;
#endif
	CString	str;

	menu->CreatePopupMenu();
	const int base = 25000;
	/*
	for (int ii = 0; ii <= 5; ii++)
	{
		if (ii)
			str.Format("%d", ii);
		else	str = _T("N");
		menu->AppendMenuX(MF_STRING, base + ii, str);
		if (ii == m_groupN)
			menu->CheckMenuItem(base + ii, MF_CHECKED);
	}
	menu->AppendMenuX(MF_STRING, base + ALL_GROUP, _T("A"));
	if (ALL_GROUP == m_groupN)
		menu->CheckMenuItem(base + ALL_GROUP, MF_CHECKED);
	*/
	
	for (int ii = 0; ii <= 10; ii++)
	{
		if (ii)
			str.Format("%d", ii);
		else	str = _T("���� ����");
		switch(ii)
		{
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 5 :
		case 6 :
		case 7 :
		case 8 :
		case 9 :
		case 10 :
			str = "���� ���� �׷� " + str +"";
			break;
		}
		menu->AppendMenuX(MF_STRING, base + ii, str);
		if (ii == m_groupN)
			menu->CheckMenuItem(base + ii, MF_CHECKED);
	}
	menu->AppendMenuX(MF_STRING, base + ALL_GROUP, _T("��� �׷� ����"));
	if (ALL_GROUP == m_groupN)
		menu->CheckMenuItem(base + ALL_GROUP, MF_CHECKED);
	

	const UINT	nFlags = TPM_RETURNCMD|TPM_LEFTALIGN|TPM_RIGHTBUTTON;
	const CRect	rect   = m_arRc.GetAt(IDX_GROUP);
	CPoint	point  = CPoint(rect.left, rect.bottom);
	ClientToScreen(&point);
	const int	comm = menu->TrackPopupMenu(nFlags, point.x, point.y + GAP, this);
#ifndef DF_USE_CPLUS17
	delete menu;
#endif
	return comm - base;
}

int CXCaption::DrawBitmap(CDC* pDC, int index, CRect bRC, bool bDown)
{
	if (bRC.IsRectEmpty())	return 0;
	return DrawBitmapByMask(pDC, index, bRC, bDown);
	
	CDC	memDC;
	if (!memDC.CreateCompatibleDC(pDC))
		return 0;

	BITMAP	bm{};
	CBitmap* bitmap	= GetButtonImage(index);
	CBitmap* pbitmap= memDC.SelectObject(bitmap);
	bitmap->GetBitmap(&bm);
	
	const int	bx = bRC.Width();
	const int	by = bRC.Height();
	int	sx = bDown ? bx : 0;
	int	iCnt{};

	switch (index)
	{
	case IDX_GROUP:
		iCnt = bm.bmWidth/(2 * UB_WIDTH);
		if (m_groupN == ALL_GROUP)
			sx = bDown ? iCnt * UB_WIDTH + (iCnt-1) * UB_WIDTH : (iCnt-1) * UB_WIDTH;
		else	
			sx = bDown ? iCnt * UB_WIDTH + m_groupN * UB_WIDTH : m_groupN*UB_WIDTH;
		break;
	case IDX_SINGLE:
		if (m_single)
			sx = bDown ? UB_WIDTH*3 : UB_WIDTH*2;
		break;
	case IDX_PIN:
		break;
	}

	if (pbitmap == (CBitmap *) 0)
	{
		memDC.DeleteDC();
		return 0 ;
	}

	CDebug::Out("sx =   %d", sx);
	pDC->BitBlt(bRC.left, bRC.top, bx, by, &memDC, sx, 0, SRCCOPY);
	memDC.SelectObject(pbitmap);
	memDC.DeleteDC();

	return bx;
}

void CXCaption::OnSize(UINT nType, int cx, int cy) 
{
	SetRegion();
	CControlBar::OnSize(nType, cx, cy);
}

void CXCaption::SetTitle(CString title)
{
	m_title = title;
	GetParent()->SetWindowText(title);
	Invalidate(FALSE);
}

bool CXCaption::changePin()
{
	m_pinON = !m_pinON;

	CClientDC	dc(this);
	DrawBitmap(&dc, IDX_PIN, m_arRc.GetAt(IDX_PIN));
	return m_pinON;
}

void CXCaption::HideButton(DWORD flag)
{
	//m_hideflag = flag;

	//** temporary
	m_hideflag |= flag;//HIDE_HELP;

	SetRegion();
	Invalidate();
}

void CXCaption::ShowButton(DWORD flag)
{
	//m_hideflag = flag;

	//** temporary
	if (m_hideflag & flag)
	m_hideflag ^= flag;

	SetRegion();
	Invalidate();
}

void CXCaption::OnRButtonDown(UINT nFlags, CPoint point) 
{
	GetParent()->SendMessage(WM_AXIS, MAKEWPARAM(axLINKEDIT, 1));
	CControlBar::OnRButtonDown(nFlags, point);
}

CBitmap* CXCaption::GetButtonImage(int index)
{
	const char* const buttonNames[11] = 
	{
		"PIN", "����ȭ��", "GROUP", "FONT", "COPY", "HELP", "�ּ�ȭ", "�ִ�ȭ", "â�ݱ�", "â����", "ICON"
	};

	if (index == 10)
		return Axis::GetBitmap(IDB_IBK);

	if (index >= 0 && index < 11)
	{
		if (m_activeCap)
			return Axis::GetBitmap(buttonNames[index]);
		else
			return Axis::GetBitmap(Format("XN_%s", buttonNames[index]));
	}
	else
		return NULL;
}


/************************************************************************/
/* CXChildCaption                                                       */
/************************************************************************/
CXChildCaption::CXChildCaption()
: CXCaption()
{

}

CXChildCaption::~CXChildCaption()
{
}

void CXChildCaption::Initialize()
{

}

void CXChildCaption::Release()
{
	
}

int CXChildCaption::DrawBitmapByMask(CDC* pDC, int index, CRect bRC, bool bDown, int maskcolor)
{
	///**
	BITMAP bm{};
	CBitmap* bitmap{};
	int iCnt{}, bx{}, by{}, sx{};

	bx = bRC.Width();
	by = bRC.Height();

	sx = bDown ? bx: 0;

	bitmap = GetButtonImage(index);
	if (bitmap && bitmap->m_hObject)
		bitmap->GetBitmap(&bm);
	else
		return bx;

	const int drawIndex = 0;
	CSize size(UB_WIDTH, UB_WIDTH);
	
	switch (index)
	{
	case IDX_GROUP:
		iCnt = bm.bmWidth/(2*UB_WIDTH);
		if (m_groupN == ALL_GROUP)
			sx = bDown ? iCnt*UB_WIDTH + (iCnt-1)*UB_WIDTH : (iCnt-1)*UB_WIDTH;
		else	
			sx = bDown ? iCnt*UB_WIDTH + m_groupN*UB_WIDTH : m_groupN*UB_WIDTH;
		break;
	case IDX_SINGLE:
		if (m_single)
			sx = bDown ? UB_WIDTH*3 : UB_WIDTH*2;
		break;
	case IDX_PIN:
		if (m_pinON)
			sx = bDown ? bm.bmWidth/2 + UB_WIDTH : bm.bmWidth/2;
		else	
			sx = bDown ? UB_WIDTH : 0;
		break;
	case IDX_ICON:
		bitmap = ResourceHelper()->GetSiteIcon();
		bRC.left--;
		bRC.top--;
		size.cx++;
		size.cy++;
		break;
	}

	if (index <= IDX_ICON)
		bRC.top += 1;

	CAxDrawHelper painter(pDC);
	painter.DrawIndexedBitmap(bRC.left, bRC.top, bitmap, maskcolor, size.cx, size.cy, sx);

	return bx;
}

void CXChildCaption::OnLButtonDown(CDC* dc, CRect& rc, int nItem)
{
	switch (nItem)
	{
	case IDX_ICON:
		{
			/*
			CMenu*	menu;
			CRect	rc = m_arRc.GetAt(IDX_ICON);
			menu = GetParent()->GetSystemMenu(FALSE);
			int cmd = menu->TrackPopupMenu(TPM_RETURNCMD|TPM_LEFTALIGN|TPM_RIGHTBUTTON,
						rc.left, rc.bottom + GAP, this);
			TRACE("system menu command = [%d]\n", cmd);
			// CWnd::OnSysCommand() reference
			*/
		}
		break;
	default:
		SetCapture();
		m_downI = nItem;
		if (m_bMax && IDX_RESTORE == nItem)
			rc = m_arRc.GetAt(IDX_MAX);
		else	
			rc = m_arRc.GetAt(nItem);
		DrawBitmap(dc, nItem, rc, true);
		break;
	}	
}

void CXChildCaption::OnLButtonDblClk(int nItem)
{
	
	if (nItem != -1)
	{
		m_action = nItem;
		m_mainframe->PostMessage(m_nMsg, MAKEWPARAM(axCAPTION, (int) m_action), m_key);
	}
	else
	{
		m_action = -1;
		if (m_single)
			m_mainframe->PostMessage(m_nMsg, MAKEWPARAM(axCAPTION, m_bMax ? IDX_RESTORE:IDX_MAX), m_key);
		else
		{
			if (m_bMax)
				m_mainframe->PostMessage(m_nMsg, MAKEWPARAM(axCAPTION, IDX_RESTORE), m_key);
			else
			{
				if (m_bIconic)
				{
					m_bIconic = false;
					m_mainframe->PostMessage(m_nMsg, MAKEWPARAM(axCAPTION, IDX_RESTORE), m_key);
				}
				else	m_mainframe->PostMessage(m_nMsg, MAKEWPARAM(axCAPTION, IDX_MAX), m_key);
			}
		}
	}
	
}

void CXChildCaption::DrawTitle(CDC* dc)
{
}

CSize CXChildCaption::OnCalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CSize size;
	size.cx = 32767;
	size.cy = XCAP_HEIGHT;

	return size;
}

bool CXChildCaption::ActivateApp(BOOL bActive)
{
	if (m_activeCap != bActive)
	{
		m_activeCap = bActive;
		if (GetSafeHwnd())
			Invalidate();
		
		if (m_activeCap && m_mainframe)
		{
			if (GetParent()->GetStyle() & WS_VISIBLE)
			{
				m_mainframe->SendMessage(WM_AXIS, MAKEWPARAM(axFOCUS, m_key), 0);
			}
		}
		return true;
	}	

	return false;
}

CBitmap* CXChildCaption::GetButtonImage(int index)
{
	const char* const buttonNames[11] = 
	{
		"PIN", "����ȭ��", "GROUP", "FONT", "COPY", "HELP", "�ּ�ȭ_T", "�ִ�ȭ_T", "â�ݱ�_T", "â����_T", "ICON"
	};

	if (index == 10)
		return Axis::GetBitmap(IDB_IBK);

	if (index >= 0 && index < 11)
	{
		if (m_activeCap)
			return Axis::GetSkinBitmap(buttonNames[index]);
		else
			return Axis::GetBitmap(Format("XN_%s", buttonNames[index]));
	}
	else
		return NULL;
}

CString CXChildCaption::GetItemTipText(int nItem)
{
	CString tipS = "";
	switch (nItem)
	{
	case IDX_PIN:	tipS = m_pinON  ? _T("��������") : _T("��������");	break;
	case IDX_SINGLE:tipS = m_single  ? _T("����") : _T("��������");	break;
	case IDX_GROUP:	tipS = m_bIconic ? _T("") : _T("��������׷�");	break;
	case IDX_COPY:	tipS = m_bIconic ? _T("") : _T("ȭ�麹��");	break;
	case IDX_HELP:	tipS = m_bIconic ? _T("") : _T("����");	break;
	case IDX_FONT:	tipS = m_bIconic ? _T("") : _T("��Ʈ����");	break;
	case IDX_MIN:	tipS =  _T("�ּ�ȭ");	break;
	case IDX_MAX:	m_bMax ? tipS = _T("����ũ���"):tipS = _T("�ִ�ȭ");	break;
	case IDX_CLOSE:	tipS = _T("�ݱ�");	break;
	case IDX_RESTORE:tipS = _T("����ũ���");
	default:	break;
	}

	return tipS;
}

void CXChildCaption::FocusNotify()
{
	if (m_activeCap && m_mainframe)
		m_mainframe->SendMessage(WM_AXIS, MAKEWPARAM(axFOCUS, m_key), 0);
}


void CXChildCaption::SetRegion()
{
	CRect	cRc, iRc;
	CPoint	pt;
	BITMAP	bm{};

	GetClientRect(cRc);
	CBitmap* bmp = GetButtonImage(IDX_CLOSE);
	if (!bmp || !bmp->m_hObject)
		return;

	bmp->GetBitmap(&bm);

	pt = CPoint(cRc.right - bm.bmWidth/2, (cRc.Height() - bm.bmHeight)/2);
	iRc.SetRect(pt.x, pt.y, pt.x + bm.bmWidth/2, pt.y + bm.bmHeight);
	iRc.OffsetRect(-GAP, 0);


	for (int ii = IDX_CLOSE; ii >= IDX_PIN; ii--)
	{
		switch (ii)
		{
		case IDX_PIN:
			/*if (m_sdiMode)
				iRc = m_arRc.GetAt(IDX_SINGLE);
			else if (!m_single)
				iRc.SetRectEmpty();*/
			if (!m_sdiMode)
			{
				if (!m_single)
					iRc.SetRectEmpty();
			}
			m_arRc.SetAt(IDX_PIN, iRc);	break;
			break;
		case IDX_SINGLE:
			if (m_hideflag & HIDE_SINGLE)	
			{
				m_arRc.SetAt(IDX_SINGLE, CRect(0, 0, 0, 0));
				continue;
			}
			if (m_sdiMode)	continue;
			m_arRc.SetAt(IDX_SINGLE, iRc);
			iRc.OffsetRect(-1, 0);		
			break;
		case IDX_GROUP:
			if (m_hideflag & HIDE_GROUP)	
			{
				m_arRc.SetAt(IDX_GROUP, CRect(0, 0, 0, 0));
				continue;
			}
			m_arRc.SetAt(IDX_GROUP, iRc);
			iRc.OffsetRect(-1, 0);		
			break;
		case IDX_COPY:
			if (m_hideflag & HIDE_COPY)	
			{
				m_arRc.SetAt(IDX_COPY, CRect(0, 0, 0, 0));
				continue;
			}
			m_arRc.SetAt(IDX_COPY, iRc);
			iRc.OffsetRect(-1, 0);		
			break;
		case IDX_HELP:
			if (m_hideflag & HIDE_HELP)	
			{
				m_arRc.SetAt(IDX_HELP, CRect(0, 0, 0, 0));
				continue;
			}
			m_arRc.SetAt(IDX_HELP, iRc);
			iRc.OffsetRect(-1, 0);		
			break;
		case IDX_FONT:
			if (m_hideflag & HIDE_FONT)
			{
				m_arRc.SetAt(IDX_FONT, CRect(0, 0, 0, 0));
				continue;
			}
			m_arRc.SetAt(IDX_FONT, iRc);
			iRc.OffsetRect(-1, 0);		
			break;
		case IDX_MIN:
			m_arRc.SetAt(IDX_MIN, iRc);
			m_arRc.SetAt(IDX_RESTORE, iRc);
			iRc.OffsetRect(-GAP*2, 0);	break;
		case IDX_MAX:
			if (m_hideflag & HIDE_MAX)	continue;
			m_arRc.SetAt(IDX_MAX, iRc);
			iRc.OffsetRect(-1, 0);		break;
		case IDX_CLOSE:
			m_arRc.SetAt(IDX_CLOSE, iRc);
			iRc.OffsetRect(-1, 0);		break;
		default:				continue;
		}
		iRc.OffsetRect(-bm.bmWidth/2, 0);

	}
	
	const int	cx = GetSystemMetrics(SM_CXSMICON);
	const int	cy = GetSystemMetrics(SM_CYSMICON);
	pt = CPoint((cRc.Height() - cy)/2 + GAP, (cRc.Height() - cy)/2);
	iRc.SetRect(pt.x, pt.y, pt.x + cx, pt.y + cy);
	m_arRc.SetAt(IDX_ICON, iRc);
}

int CXChildCaption::GetSelectItem(CPoint point)
{
	CRect	bRc;

	for (int ii = 0; ii < m_arRc.GetSize(); ii++)
	{
		bRc = m_arRc.GetAt(ii);
		if (!bRc.PtInRect(point))
			continue;
		//if (!m_bIconic)	return ii;

		switch (ii)
		{
		case IDX_SINGLE:
			if (m_sdiMode)	return IDX_PIN;
			return ii;
		case IDX_MAX:
			if (m_bMax)	return IDX_RESTORE;
		case IDX_PIN:
		case IDX_ICON:
		case IDX_CLOSE:	return ii;
		case IDX_MIN:	return m_bIconic ? IDX_RESTORE : IDX_MIN;
		case IDX_GROUP:	case IDX_COPY:
		case IDX_HELP:	case IDX_FONT:	return m_bIconic ? -1 : ii;
		}
	}
	return -1;
}

void CXChildCaption::DrawFrame(CDC* dc, CRect rc)
{
	CBitmap* bmp{};

	if (IsActive())
		bmp = m_capAct;
	else
		bmp = m_capInact;

	if (!bmp) {
		return;
	}

	rc -= rc.TopLeft();
	rc += CPoint(1, 1);
	
	BITMAP bm;
	bmp->GetBitmap(&bm);

	CDC mdc;
	mdc.CreateCompatibleDC(dc);
	CBitmap* oldBmp = mdc.SelectObject(bmp);

	const int frameWidth = 3;

	dc->StretchBlt(rc.left, rc.top, frameWidth, bm.bmHeight, &mdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	dc->StretchBlt(rc.right - frameWidth - 2, rc.top, frameWidth, bm.bmHeight, &mdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	if (m_capSideAct)
	{
		mdc.SelectObject(m_activeCap ? m_capSideAct : m_capSideInact);
		BITMAP bm;
		m_capSideAct->GetBitmap(&bm);
		
		dc->BitBlt(rc.left, rc.top, bm.bmWidth, bm.bmHeight, &mdc, 0, 0, SRCCOPY);
		dc->StretchBlt(rc.right - frameWidth - 2, rc.top, frameWidth, bm.bmHeight, &mdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	}

	mdc.SelectObject(oldBmp);
}

int CXCaption::DefaultFont()
{
	/*
	CString	str;

	int	base = 25000;
	int	comm = base + INITMAPSIZE;
	
	if (comm > base)
	{
		if (comm - base != INITMAPSIZE)
			m_fontSize = comm - base;
	}
	*/
	
	return INITMAPSIZE;
}
