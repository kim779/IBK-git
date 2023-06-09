// GridWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_ContractX.h"
#include "GridWnd.h"
#include "ctrl.h"
#include "ControlWnd.h"
#include "../../h/memDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridWnd

CGridWnd::CGridWnd(CWnd *pWizard, CWnd *pParent)
{
	m_pApp = (CCX_ContractXApp*)AfxGetApp();

	m_pWizard = pWizard;
	m_pParent = pParent;
	m_pScroll = nullptr;

	m_rcData.SetRectEmpty();
	
	m_SCRpos = m_SCRday = m_SCRend = m_SCRtotal = 0;
	m_iDispCnt = 10;
	m_iHalfLine = 0;
	m_pFont = nullptr;

	m_showTitle = FALSE;
	m_col1 = m_col2 = 50;

	m_colorUp = m_colorDown = m_colorNormal = m_colorLine = 0;

}

CGridWnd::~CGridWnd()
{
}


BEGIN_MESSAGE_MAP(CGridWnd, CWnd)
	//{{AFX_MSG_MAP(CGridWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SCRL, OnVScroll)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGridWnd message handlers
long CGridWnd::OnVScroll(WPARAM wParam, LPARAM lParam)
{
	int	pos = 0;

	switch (LOWORD(wParam))
	{
	case SCRL_VCHANGE:
		switch ((int)HIWORD(wParam))
		{
		case SB_LINEUP:
			if (m_SCRpos < 1)
				break;

			m_SCRpos--;
			m_SCRend--;

			UpdateScroll(m_SCRpos, m_SCRend, m_SCRday, m_SCRtotal);
			Invalidate();

			break;
		case SB_LINEDOWN:
			if (m_SCRend >= m_SCRtotal)
				break;

			m_SCRpos++;
			m_SCRend++;

			UpdateScroll(m_SCRpos, m_SCRend, m_SCRday, m_SCRtotal);
			Invalidate();

			break;
		case SB_PAGEUP:
			if (m_SCRpos < 1)
				break;

			m_SCRpos -= m_iDispCnt - 1;
			m_SCRend -= m_iDispCnt - 1;

			if (m_SCRpos < 0)
			{
				m_SCRend -= m_SCRpos;
				m_SCRpos = 0;
			}

			UpdateScroll(m_SCRpos, m_SCRend, m_SCRday, m_SCRtotal);
			Invalidate();

			break;
		case SB_PAGEDOWN:
			if (m_SCRend >= m_SCRtotal)
				break;

			m_SCRpos += m_iDispCnt - 1;
			m_SCRend += m_iDispCnt - 1;

			if (m_SCRend > m_SCRtotal)
			{
				m_SCRpos -= m_SCRend - m_SCRtotal;
				m_SCRend = m_SCRtotal;
			}

			UpdateScroll(m_SCRpos, m_SCRend, m_SCRday, m_SCRtotal);
			Invalidate();

			break;
		case SB_THUMBTRACK:
			pos = (int)lParam;
			if (pos >= 0)
			{
				int scrPos = (int)((pos) * (m_SCRtotal) / 100); 
				int scrEnd = m_SCRday + scrPos;
				if (scrEnd > m_SCRtotal)
				{
					const	int	gap = scrEnd - m_SCRtotal;
					scrPos -= gap;
					scrEnd -= gap;
				}
				m_SCRpos = scrPos;
				m_SCRend = scrEnd;

				UpdateScroll(m_SCRpos, m_SCRend, m_SCRday, m_SCRtotal);
				Invalidate();
			}
			break;
		}
		break;
	}
	
	return 0;
}

int CGridWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_pScroll == NULL)	// invalidate()에서 깜빡임이 나타나므로  Parent를 상위 윈도우를 준다.
	{
		m_pScroll = std::make_unique<CExScrollBar>();
		m_pScroll->Create(WS_CHILD|WS_VISIBLE|SBS_VERT|SBS_BOTTOMALIGN | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), GetParent(), 0);
		m_pScroll->SetOwner(this);
		m_pScroll->EnableWindow(FALSE);
	}

	return 0;
}

void CGridWnd::OnDestroy() 
{
	while (!m_match.IsEmpty())
		m_match.RemoveHead().reset();

	if (m_pScroll)
		m_pScroll.reset();

	CWnd::OnDestroy();
}

void CGridWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	xxx::CMemDC	mdc(&dc);

	DrawData(&mdc);
}

void CGridWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);	

	Resize();
}

void CGridWnd::Resize()
{
	CRect	rc;

	GetClientRect(&rc);
	m_rcData = rc;
	Invalidate();
}

void CGridWnd::SetFontx(CFont *pFont)
{
	m_pFont = pFont;
	Resize();
}

void CGridWnd::InsertData(const char* price, const char* volume)
{
	while (((CControlWnd*)m_pParent)->GetMax() < m_match.GetCount())
		m_match.RemoveTail().reset();

	m_match.AddHead(std::make_shared<CMatch>(price, volume));

	AutoUpdate();
}

void CGridWnd::AddData(const char* price, const char* volume)
{
	m_match.AddTail(std::make_shared<CMatch>(price, volume));
}

void CGridWnd::AutoScroll()
{
	m_SCRtotal = GetCount();

	if (m_SCRtotal <= 0)
	{
		m_SCRpos = m_SCRday = m_SCRend = 0;
	}

	if (m_SCRday < m_iDispCnt)
	{
		m_SCRend = m_SCRday = m_SCRtotal;
	}

	UpdateScroll(m_SCRpos, m_SCRend, m_SCRday, m_SCRtotal);
}

void CGridWnd::UpdateScroll(int pos, int end, int day, int total)
{
	SCROLLINFO sInfo;
	sInfo.cbSize = sizeof(SCROLLINFO);
	sInfo.fMask = SIF_ALL|SIF_DISABLENOSCROLL;     
	sInfo.nMin = 0;
	sInfo.nMax = 100;

	if ((total) == 0)
	{
		sInfo.nPos = 100;
		sInfo.nPage = 110;
	}
	else
	{
		sInfo.nPos = (end == total)? 100:(pos*100) / (total);
		sInfo.nPage = (day*100) / (total);
	}

	if ((int)sInfo.nPage == sInfo.nMax)
		sInfo.nPage = sInfo.nMax+1;

	if (m_pScroll)
		m_pScroll->Initialize(sInfo);
	m_pScroll->Invalidate();
}

void CGridWnd::Clear()
{
	while (!m_match.IsEmpty())
		m_match.RemoveHead().reset();

	AutoScroll();
	Invalidate();
}

void CGridWnd::SetTextColor(COLORREF upColor, COLORREF downColor, COLORREF normalColor, COLORREF lineColor)
{
	m_colorUp = upColor;
	m_colorDown = downColor;
	m_colorNormal = normalColor;
	m_colorLine = lineColor;
}

void CGridWnd::DrawData(CDC *pDC)
{
	CRect	rect, cRc, rcFill, rcTemp;
	const	COLORREF clr = RGB(227, 227, 227);

	rect = m_rcData;
	m_iHalfLine = (int)(m_rcData.Width() * (m_col1 / 100.0));
	if (m_pFont == NULL)
		return;

	CFont*	pOldFont = pDC->SelectObject(m_pFont);
	CPen*	pPen = m_pApp->GetPen(m_pWizard, m_colorLine, 1);
	CPen*	pPenWhite = m_pApp->GetPen(m_pWizard, RGB(255, 255, 255), 1);
	CPen*	pOldPen = pDC->SelectObject(pPen);
	
	pDC->FillSolidRect(&rect, RGB(255, 255, 255));

	CString sPrice, sVolume;
	const	double	height = double(rect.Height()) / double(m_iDispCnt + (m_showTitle ? 1 : 0));
	COLORREF textColor = 0;
	POSITION pos{};
	std::shared_ptr<CMatch>	match = nullptr;

	for (int ii = 0, row = 0; ii < m_SCRday; ii++)
	{
		row = ii + (m_showTitle ? 1 : 0);
		rcFill.SetRect(0, (int)(row * height), rect.Width() - 1, (int)((row + 1) * height));
		
		if (ii % 2 == 0)
			pDC->FillSolidRect(rcFill, RGB(245, 245, 245));
		else
			pDC->FillSolidRect(rcFill, RGB(255, 255, 255));

		pos = m_match.FindIndex(ii + m_SCRpos);
		if (pos)
		{
			match = m_match.GetAt(pos);
			sPrice = match->price;
			sVolume = match->volume;
		}
		else 
		{
			sPrice = sVolume = "";
		}

		cRc.SetRect(0, (int)(row * height)+2, m_iHalfLine - 3, (int)((row + 1) * height));


		textColor = m_colorNormal;
		if (!sPrice.IsEmpty())
		{
			switch (sPrice[0])
			{
			case '+': case '1':
				textColor = m_colorUp;
				sPrice.Delete(0, 1);
				break;
			case '-': case '4':
				textColor = m_colorDown;
				sPrice.Delete(0, 1);
				break;
			}
		}
		pDC->SetTextColor(textColor);
		pDC->DrawText(sPrice, cRc, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

		textColor = m_colorNormal;
		if (!sVolume.IsEmpty())
		{
			switch (sVolume[0])
			{
			case '+': case '1':
				textColor = m_colorUp;
				sVolume.Delete(0, 1);
				break;
			case '-': case '4':
				textColor = m_colorDown;
				sVolume.Delete(0, 1);
				break;
			}
		}
		pDC->SetTextColor(textColor);
		cRc.SetRect(cRc.right, cRc.top, rect.Width() - 3, cRc.bottom);
		pDC->DrawText(sVolume, cRc, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
	}

	if (m_showTitle)
	{
		pDC->SelectObject(pPenWhite);
		pDC->MoveTo(rect.right, rect.top);
		pDC->LineTo(rect.left, rect.top);
		pDC->LineTo(rect.left, rect.bottom-1);
		pDC->LineTo(rect.right, rect.bottom-1);

		rcTemp.SetRect(rect.left, rect.top, rect.right, rect.top + (int)height);
		pDC->FillSolidRect(rcTemp, clr);
		pDC->SelectObject(pPen);
		
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->DrawText("체결가", CRect(rcTemp.left, rcTemp.top, rcTemp.left + m_iHalfLine, rcTemp.bottom), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		pDC->DrawText("체결량", CRect(rcTemp.left + m_iHalfLine, rcTemp.top, rcTemp.right, rcTemp.bottom), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		
		pDC->MoveTo(rcTemp.left, rcTemp.bottom);
		pDC->LineTo(rcTemp.right, rcTemp.bottom);
	}

	pDC->SelectObject(pPen);
	rect.bottom--;
	pDC->MoveTo(m_iHalfLine - 1, rect.top);
	pDC->LineTo(m_iHalfLine - 1, rect.bottom);
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.left, rect.bottom);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->LineTo(rect.right, rect.top);
	pDC->LineTo(rect.left, rect.top);

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}


BOOL CGridWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;//CWnd::OnEraseBkgnd(pDC);
}

void CGridWnd::SetColInfo(int col1, int col2)
{
	m_col1 = col1;
	m_col2 = col2;
}

void CGridWnd::AutoUpdate()
{
	Invalidate();
	AutoScroll();
}