// TInfo1.cpp : implementation file
//

#include "stdafx.h"
#include "TInfo1.h"
#include "SimpleSplitter.h"
#include "Pane.h"
#include "resource.h"
#include "smdefine.h"
#include "nbutton.h"
#include "../../h/axisvar.h"
#include "../../axis/axmsg.hxx"
#include "MenuXP.h"

#define	WGAP		2
#define MINPANE		20

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTInfo1

CTInfo1::CTInfo1(bool (*callback)(int, WPARAM, LPARAM))
{
	m_axiscall	= callback;
	m_height	= TINFO_HEIGHT;
	m_base		= NULL;
	//**m_updn		= NULL;
	m_deal		= NULL;
	m_config	= NULL;
	m_bBase		= TRUE;

	m_bmpBackground = Axis::GetBitmap("ticker_bg");
	if (!m_bmpBackground)
		m_bmpBackground = Axis::GetBitmap(IDB_INFOBACK);
}

CTInfo1::~CTInfo1()
{
	XMSG();
#ifndef DF_USESTL_TINFO1
	if (m_base)	delete m_base;
	if (m_deal)	delete m_deal;
	if (m_config)	delete m_config;
	for (POSITION pos = m_arPane.GetHeadPosition(); pos; )
		delete m_arPane.GetNext(pos);
#else
	m_arPane.clear();
#endif
}


BEGIN_MESSAGE_MAP(CTInfo1, CDialogBar)
	//{{AFX_MSG_MAP(CTInfo1)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XMSG, OnXMSG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTInfo1 message handlers

void CTInfo1::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CCmdUI	state;

	state.m_pOther = this;
	state.m_nIndexMax = 1;
	state.m_nID = AFX_IDW_STATUS_BAR;

	if (CControlBar::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
		return;
	state.DoUpdate(pTarget, FALSE);
}

CSize CTInfo1::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CRect	rect;
	rect.SetRectEmpty();
	CalcInsideRect(rect, bHorz);

	CRect	client;
	GetClientRect(client);

	CSize	size;
	CRect	fRc;

	size.cy = m_height;
	size.cy += (rect.Height());

	if (IsFloating())
	{
		GetDockingFrame()->GetParent()->GetWindowRect(fRc);
		size.cx = fRc.Width();
	}
	else
	{
		GetDockingFrame()->GetWindowRect(fRc);
		size.cx = fRc.Width();
	}

	return size;
}

void CTInfo1::OnDestroy() 
{
	XMSG();
#ifdef DF_USESTL_TINFO1
	CString	value, tmps;
	CRect	paneRc;
	CArray <int, int > ary;

	for (int ii = 0; ii < (int)m_arPane.size(); ii++)
	{
		m_base->GetPaneRect(ii, paneRc);
		ary.Add(paneRc.Width());
	}

	for (int ii = 0; ii < ary.GetSize(); ii++)
	{
		if (!ii)
			value.Format("%d", ary.GetAt(ii));
		else
		{
			tmps.Format(", %d", ary.GetAt(ii));
			value += tmps;
		}
	}

	CProfile profile(pkAxisTicker);
	profile.Write(szGeneral, "BAR00SIZE", value);

	CDialogBar::OnDestroy();
#else
	CString	value, tmps;
	CRect	paneRc;
	CArray <int, int > ary;

	for (int ii = 0; ii < m_arPane.GetCount(); ii++)
	{
		m_base->GetPaneRect(ii, paneRc);
		ary.Add(paneRc.Width());
	}

	for (int ii = 0; ii < ary.GetSize(); ii++)
	{
		if (!ii)
			value.Format("%d", ary.GetAt(ii));
		else
		{
			tmps.Format(", %d", ary.GetAt(ii));
			value += tmps;
		}
	}

	CProfile profile(pkAxisTicker);
	profile.Write(szGeneral, "BAR00SIZE", value);

	CDialogBar::OnDestroy();
#endif
	
}

void CTInfo1::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC	 memDC;
	CRect	 cRc;

	GetClientRect(cRc);
	if (memDC.CreateCompatibleDC(&dc))
	{
		CBitmap	bitmap;
		bitmap.CreateCompatibleBitmap(&dc, cRc.Width(), cRc.Height());
		CBitmap* oldbuffBitmap = memDC.SelectObject(&bitmap);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			drawBackground(&memDC);
			DrawGripper(&memDC);
			
			dc.BitBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
			memDC.SelectObject(oldbuffBitmap);
		}
		memDC.DeleteDC();
	}

}

void CTInfo1::OnSize(UINT nType, int cx, int cy) 
{
	CDialogBar::OnSize(nType, cx, cy);
	ResizeBase();

}

void CTInfo1::drawBackground(CDC* pDC)
{
	CDC	 memDC;
	CRect	 cRc;

	GetClientRect(cRc);
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap* oldbuffBitmap = memDC.SelectObject(m_bmpBackground);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			BITMAP	 bm;
			m_bmpBackground->GetBitmap(&bm);
			pDC->StretchBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), 
					&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			int diff_x{}, diff_y{}, diff_button{};
			diff_x = 2;
			diff_y = 2;
			diff_button = 59;
			pDC->MoveTo(0+diff_x,0+diff_y); pDC->LineTo(cRc.right-1-diff_x-diff_button, 0+diff_y); pDC->LineTo(cRc.right-1-diff_x-diff_button, cRc.bottom-1-diff_y);
			pDC->LineTo(0+diff_x, cRc.bottom-1-diff_y); pDC->LineTo(0+diff_x,0+diff_y);
			memDC.SelectObject(oldbuffBitmap);
		}
		memDC.DeleteDC();
	}
}

void CTInfo1::DrawGripper(CDC* pDC)
{
	const CRect	gRc = GetGRect();

	pDC->FillSolidRect(gRc.left, gRc.top + 1, 1, gRc.Height() - 2, GRIP_LT);
	pDC->FillSolidRect(gRc.left + 1, gRc.top, 1, 1, GRIP_LT);
	pDC->FillSolidRect(gRc.left + 2, gRc.top + 1, 1, gRc.Height() - 2, GRIP_RB);
	pDC->FillSolidRect(gRc.left + 1, gRc.bottom - 1, 1, 1, GRIP_RB);

}

void CTInfo1::Config(int configN, COLORREF bkColor)
{
#ifdef DF_USESTL_TINFO1
	if (m_base || !(this && GetSafeHwnd()))
		return;

	CArray	< int, int > ary;
	m_base = std::make_unique<CSimpleSplitter>(configN, SSP_HORZ, MINPANE, SPLITWIDTH);
	m_base->Create(this, m_bmpBackground);
	GetPaneSize(configN, ary);

	std::unique_ptr<int[]> size = std::make_unique<int[]>(configN);

	for (int ii = 0; ii < configN; ii++)
		size.get()[ii] = ary.GetAt(ii);
	m_base->SetPaneSizes(size.get());
	ary.RemoveAll();

	for (int ii = 0; ii < configN; ii++)
	{
		auto pane = m_arPane.emplace_back(std::make_shared<CPane>(m_axiscall, m_bmpBackground));
		if (!m_base->CreatePane(ii, pane.get(), 0, 0/*WS_EX_CLIENTEDGE*/, NULL))
			continue;

		pane.get()->SetPaneInfo(ii);
	}

	createButton();
	ResizeBase();
#else
	if (m_base || !(this && GetSafeHwnd()))
		return;

	CArray	< int, int > ary;
	m_base = new CSimpleSplitter(configN, SSP_HORZ, MINPANE, SPLITWIDTH);
	m_base->Create(this, m_bmpBackground);
	GetPaneSize(configN, ary);

	int* size = new int[configN];
	for (int ii = 0; ii < configN; ii++)
		size[ii] = ary.GetAt(ii);
	m_base->SetPaneSizes(size);
	ary.RemoveAll();

	for (int ii = 0; ii < configN; ii++)
	{
		CPane* pane = new CPane(m_axiscall, m_bmpBackground);
		if (!m_base->CreatePane(ii, pane, 0, 0/*WS_EX_CLIENTEDGE*/, NULL))
		{
			delete pane;
			continue;
		}
		pane->SetPaneInfo(ii);
		m_arPane.AddTail(pane);
	}

	createButton();
	delete[]size;
	ResizeBase();
#endif
}

void CTInfo1::ReConfig(int configN)
{
#ifdef DF_USESTL_TINFO1
	CArray	< int, int > ary;
	if (m_base)
	{
		CRect	paneRc;
		for (int ii = 0; ii < configN; ii++)
		{
			if (ii < (int)m_arPane.size())
				m_base->GetPaneRect(ii, paneRc);
			ary.Add(paneRc.Width());
		}

		m_base.reset();
		m_arPane.clear();
	}
#else
	CArray	< int, int > ary;
	if (m_base)
	{
		CRect	paneRc;
		for (int ii = 0; ii < configN; ii++)
		{
			if (ii < m_arPane.GetCount())
				m_base->GetPaneRect(ii, paneRc);
			ary.Add(paneRc.Width());
		}

		delete m_base;

		for (POSITION pos = m_arPane.GetHeadPosition(); pos;)
			delete m_arPane.GetNext(pos);

		m_arPane.RemoveAll();
		m_base = NULL;
	}
#endif

#ifdef DF_USESTL_TINFO1
	if (!(this && GetSafeHwnd()))	return;
	m_base = std::make_unique<CSimpleSplitter>(configN, SSP_HORZ, MINPANE, SPLITWIDTH);
	m_base->Create(this, m_bmpBackground);

	std::unique_ptr<int[]> size = std::make_unique<int[]>(configN);
	for (int ii = 0; ii < configN; ii++)
		size.get()[ii] = ary.GetAt(ii);
	m_base->SetPaneSizes(size.get());
	ary.RemoveAll();

	for (int ii = 0; ii < configN; ii++)
	{
		auto pane = m_arPane.emplace_back(std::make_shared<CPane>(m_axiscall, m_bmpBackground));
		if (!m_base->CreatePane(ii, pane.get(), 0, 0/*WS_EX_CLIENTEDGE*/, NULL))
			continue;
		pane->SetPaneInfo(ii);
	}

	ResizeBase();
#else
	if (!(this && GetSafeHwnd()))	return;
	m_base = new CSimpleSplitter(configN, SSP_HORZ, MINPANE, SPLITWIDTH);
	m_base->Create(this, m_bmpBackground);

	int* size = new int[configN];
	for (int ii = 0; ii < configN; ii++)
		size[ii] = ary.GetAt(ii);
	m_base->SetPaneSizes(size);
	ary.RemoveAll();

	for (int ii = 0; ii < configN; ii++)
	{
		CPane* pane = new CPane(m_axiscall, m_bmpBackground);
		if (!m_base->CreatePane(ii, pane, 0, 0/*WS_EX_CLIENTEDGE*/, NULL))
		{
			delete pane;
			continue;
		}
		pane->SetPaneInfo(ii);
		m_arPane.AddTail(pane);
	}

	delete[]size;
	ResizeBase();
#endif
}

const int btnWidth = 28;
const int btnHeight = 18;
const int leftMargin = 3;
const int rightMargin = 3;
const int btnGap = 1;

void CTInfo1::ResizeBase()
{
	if (m_base && m_config && m_deal)
	{
		CRect rc;
		GetClientRect(rc);

		m_base->MoveWindow(leftMargin, 3, rc.right - (btnWidth * 2 + rightMargin + btnGap) - rightMargin - 2, 
			rc.bottom - 6);

		rc.left = rc.right - (btnWidth * 2 + rightMargin + btnGap);

		rc.top = rc.Height() / 2 - btnHeight / 2;
		rc.bottom = rc.top + btnHeight;
		rc.right = rc.left + btnWidth;

		m_config->MoveWindow(rc);
		rc.OffsetRect(btnWidth + btnGap, 0);
		m_deal->MoveWindow(rc);
	}
	
}

CRect CTInfo1::GetGRect()
{
	CRect pRect;
	GetClientRect(pRect);
	pRect.OffsetRect(-pRect.left,-pRect.top);

	pRect.DeflateRect(4, 3, 4, 2);
	pRect.right = pRect.left + 3;
	return pRect;
}

void CTInfo1::GetPaneSize(int configN, CArray <int, int> &ary)
{
	CString	tmpS, value;
	CProfile profile(pkAxisTicker);
	tmpS = profile.GetString(szGeneral, "BAR00SIZE", "10");
	int	total = 0;

	for (int ii = 0; ii < configN; ii++)
	{
		int pos = tmpS.Find(",");
		if (pos == -1)
		{
			pos = atoi(tmpS);
			if (pos < MINPANE)
				pos = MINPANE;
			ary.Add(pos);
			total += pos;
			continue;
		}

		value = tmpS.Left(pos++);
		tmpS = tmpS.Mid(pos);
		pos = atoi(value);
		ary.Add(pos);
		total += pos;
	}
}

void CTInfo1::SetFmx(bool setting, int kind, int key, CString symbol, CString dat)
{
#ifdef DF_USESTL_TINFO1
	for (int ii = 0; ii < (int)m_arPane.size(); ii++)
	{
		auto pane = m_arPane.at(ii);
		pane->SetFmx(setting, kind, key, symbol, dat);
	}
#else
	CPane* pane{};
	for (POSITION pos = m_arPane.GetHeadPosition(); pos; )
	{
		pane = (CPane*)m_arPane.GetNext(pos);
		pane->SetFmx(setting, kind, key, symbol, dat);
	}
#endif
}

void CTInfo1::SetEFmx(bool setting, int kind, int key, CString symbol, CString dat)
{
#ifdef DF_USESTL_TINFO1
	for (int ii = 0; ii < (int)m_arPane.size(); ii++)
	{
		auto pane = m_arPane.at(ii);
		pane->SetEFmx(setting, kind, key, symbol, dat);
	}
#else
	CPane* pane{};
	for (POSITION pos = m_arPane.GetHeadPosition(); pos; )
	{
		pane = (CPane*)m_arPane.GetNext(pos);
		pane->SetEFmx(setting, kind, key, symbol, dat);
	}
#endif

}

void CTInfo1::SetInter(char* dat, bool bRotate)
{
#ifdef DF_USESTL_TINFO1
	for (int ii = 0; ii < (int)m_arPane.size(); ii++)
	{
		auto pane = m_arPane.at(ii);
		pane->SetInter(dat, bRotate);
	}
#else
	CPane* pane{};
	for (POSITION pos = m_arPane.GetHeadPosition(); pos; )
	{
		pane = (CPane*)m_arPane.GetNext(pos);
		pane->SetInter(dat, bRotate);
	}
#endif
	
}

void CTInfo1::UpdatePaneData(int kind, CString dat)
{
#ifdef DF_USESTL_TINFO1
	for (int ii = 0; ii < (int)m_arPane.size(); ii++)
	{
		auto pane = m_arPane.at(ii);
		pane->UpdatePaneData(kind, dat);
	}
#else
	CPane* pane{};
	for (POSITION pos = m_arPane.GetHeadPosition(); pos; )
	{
		pane = (CPane*)m_arPane.GetNext(pos);
		pane->UpdatePaneData(kind, dat);
	}
#endif
	
}

void CTInfo1::ProcessRTS(CString symbol, DWORD* dat)
{
#ifdef DF_USESTL_TINFO1
	for (int ii = 0; ii < (int)m_arPane.size(); ii++)
	{
		auto pane = m_arPane.at(ii);
		pane->ProcessRTS(symbol, dat);
	}
#else
	CPane* pane{};
	for (POSITION pos = m_arPane.GetHeadPosition(); pos; )
	{
		pane = (CPane*)m_arPane.GetNext(pos);
		pane->ProcessRTS(symbol, dat);
	}
#endif
	
}

void CTInfo1::SettingEticker(int paneID)
{
#ifdef DF_USESTL_TINFO1
	for (int ii = 0; ii < (int)m_arPane.size(); ii++)
	{
		auto pane = m_arPane.at(ii);
		if (pane->GetPaneID() != paneID)
			continue;
		pane->ReSetPaneInfo();
		break;
	}
#else
	CPane* pane{};
	for (POSITION pos = m_arPane.GetHeadPosition(); pos; )
	{
		pane = (CPane*)m_arPane.GetNext(pos);
		if (pane->GetPaneID() != paneID)
			continue;
		pane->ReSetPaneInfo();
		break;
	}
#endif
}

void CTInfo1::createButton()
{
#ifdef DF_USESTL_TINFO1
	if (m_deal)	return;

	const DWORD		dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	m_deal = std::make_unique<CNButton>(CTRL_DEAL);
	if (!m_deal->Create(NULL, "ü��", dwStyle, CRect(0, 0, 0, 0), this, -1))
		m_deal = NULL;
	else
	{
		m_deal->set_ToolTipText("ü�᳻��");
		m_deal->set_Image("TK_DEAL");
	}

	m_config = std::make_unique<CNButton>(CTRL_ENVIRONMENT);
	if (!m_config->Create(NULL, "����", dwStyle, CRect(0, 0, 0, 0), this, -1))
		m_config = NULL;
	else
	{
		m_config->set_ToolTipText("ȯ�漳��");
		m_config->set_Image("TK_SET");
	}
#else
	if (m_deal)	return;

	const DWORD		dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	m_deal = new CNButton(CTRL_DEAL);
	if (!m_deal->Create(NULL, "ü��", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_deal;
		m_deal = NULL;
	}
	else
	{
		m_deal->set_ToolTipText("ü�᳻��");
		m_deal->set_Image("TK_DEAL");
	}

	m_config = new CNButton(CTRL_ENVIRONMENT);
	if (!m_config->Create(NULL, "����", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_config;
		m_config = NULL;
	}
	else
	{
		m_config->set_ToolTipText("ȯ�漳��");
		m_config->set_Image("TK_SET");
	}
#endif
}

LONG CTInfo1::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case CTRL_DEAL:		(*m_axiscall)(AXI_SHOWDEAL, wParam, lParam);	break;
	case CTRL_STOPLOSS:	(*m_axiscall)(AXI_STOPLOSS, wParam, lParam);	break;
	case CTRL_TICKSETUP:	(*m_axiscall)(AXI_TICKSETUP, wParam, lParam);	break;
	case CTRL_REMOTE:	(*m_axiscall)(AXI_HELPCOM, wParam, lParam);	break;
	case CTRL_LOCK:		(*m_axiscall)(AXI_FUNCKEY, wParam, lParam);	break;
	case CTRL_SWITCH:	(*m_axiscall)(AXI_APPSWITCH, wParam, lParam);	break;
	case CTRL_UPDN:		(*m_axiscall)(AXI_SHOWTICKER, 0, 1);	break;
	case CTRL_ENVIRONMENT: (*m_axiscall)(AXI_FUNCKEY, MAKEWPARAM(CTRL_ENVIRONMENT, 0), lParam);
	}
	return 0;
}

void CTInfo1::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_base)
		m_base->DefaultSize();
	CDialogBar::OnLButtonDblClk(nFlags, point);
}

void CTInfo1::change_Palette(CString skinN)
{
	CProfile profile(pkPalette);
	profile.Write(szGeneral, szPalette, (const char*)skinN);
}

CString CTInfo1::GetCurrentSkinName()
{
	return Axis::skinName;
}

void CTInfo1::SetBase(BOOL bBase)
{
	m_bBase = bBase;
	ResizeBase();
}

int CTInfo1::GetHeight()
{
	return m_height;
}

CBitmap* CTInfo1::GetBackImage()
{
	return m_bmpBackground;
}

void CTInfo1::InvalidatePane()
{
	
	for (int ii = 0; ii < (int)m_arPane.size(); ii++)
	{
		auto pane = m_arPane.at(ii);
		pane->Invalidate();
	}
	if (m_deal)
		m_deal->Invalidate();
	if (m_config)
		m_config->Invalidate();

	Invalidate();
}

