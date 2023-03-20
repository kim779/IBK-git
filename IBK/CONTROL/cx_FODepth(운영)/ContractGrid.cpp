// ContractGrid.cpp : implementation file
//

#include "stdafx.h"
#include "CX_FODepth.h"
#include "ContractGrid.h"
#include "ContractScroll.h"
#include "ContractWnd.h"
#include "../../h/memDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContractGrid

CContractGrid::CContractGrid(CWnd *pWizard, CWnd *pParent)
{
	m_pWizard = pWizard;
	m_pParent = pParent;
	m_pApp = (CCX_FODepthApp*)AfxGetApp();

	m_pScroll = nullptr;

	m_rcData.SetRectEmpty();
	m_SCRpos = m_SCRday = m_SCRend = m_SCRtotal = 0;
	m_iDispCnt = 10;
	m_iHalfLine = 0;
	m_pFont = nullptr;

	m_showTitle = FALSE;
	m_iHalfInfo = 0;

	m_pColor[clr_UpIndex] = m_pColor[clr_NormalIndex] = m_pColor[clr_DnIndex] = RGB(0,0,0);
	m_colorUp = m_colorDown = m_colorNormal = RGB(0, 0, 0);
	m_colorLine = m_colorGrid1st = m_colorGrid2st = RGB(0, 0, 0);

}

CContractGrid::~CContractGrid()
{
}


BEGIN_MESSAGE_MAP(CContractGrid, CWnd)
	//{{AFX_MSG_MAP(CContractGrid)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SCRL, OnVScroll)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CContractGrid message handlers
long CContractGrid::OnVScroll(WPARAM wParam, LPARAM lParam)
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

int CContractGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_pScroll == NULL)	// invalidate()���� �������� ��Ÿ���Ƿ�  Parent�� ���� �����츦 �ش�.
	{
		m_pScroll = std::make_unique<CContractScroll>();
		m_pScroll->Create(WS_CHILD|WS_VISIBLE|SBS_VERT|SBS_BOTTOMALIGN | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), GetParent(), 0);
		m_pScroll->SetOwner(this);
		m_pScroll->EnableWindow(FALSE);
	}

	return 0;
}

void CContractGrid::OnDestroy() 
{
	while (!m_match.IsEmpty())
		m_match.RemoveHead().reset();

	if (m_pScroll)
		m_pScroll.reset();

	CWnd::OnDestroy();
}

void CContractGrid::OnPaint() 
{
#ifdef _DEBUG
	CPaintDC dc(this); // device context for painting

	DrawData(&dc);
#else
	CPaintDC dc(this); // device context for painting

	xxx::CMemDC	mdc(&dc);
	DrawData(&mdc);
#endif
}

void CContractGrid::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);	

	Resize();
}

void CContractGrid::Resize()
{
	CRect	rc;
	GetClientRect(&rc);

	m_rcData = rc;
	if (m_iHalfInfo == 0)
		m_iHalfLine = (int)(m_rcData.Width() * (50 / 100.0));
	else
		m_iHalfLine = m_iHalfInfo;
	
	Invalidate();
}

void CContractGrid::SetFontx(CFont *pFont)
{
	m_pFont = pFont;
	Resize();
}

void CContractGrid::InsertData(const char* price, const char* contract, int cp, int cc)
{
	while (((CContractWnd*)m_pParent)->GetMax() < m_match.GetCount())
		m_match.RemoveTail().reset();

	m_match.AddHead(std::make_shared<CMatch>(price, contract, cp, cc));

	AutoUpdate();
}

void CContractGrid::AddData(const char* price, const char* contract, int cp, int cc)
{
	m_match.AddTail(std::make_shared<CMatch>(price, contract, cp, cc));
}

void CContractGrid::AutoScroll()
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

void CContractGrid::UpdateScroll(int pos, int end, int day, int total)
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

void CContractGrid::Clear()
{
	while (!m_match.IsEmpty())
		m_match.RemoveHead().reset();

	AutoScroll();
	Invalidate();
}

void CContractGrid::PaletteGrid(COLORREF upColor, COLORREF downColor, COLORREF normalColor, COLORREF lineColor, COLORREF gridColor1st, COLORREF gridColor2st)
{
	m_colorUp = upColor;
	m_colorDown = downColor;
	m_colorNormal = normalColor;
	m_colorLine = lineColor;
	m_colorGrid1st = gridColor1st;
	m_colorGrid2st = gridColor2st;

	m_pColor[clr_UpIndex] = m_colorUp;
	m_pColor[clr_NormalIndex] = m_colorNormal;
	m_pColor[clr_DnIndex] = m_colorDown;
}

void CContractGrid::DrawData(CDC *pDC)
{
	if (m_pFont == nullptr)
		return;

	CRect	rect, cRc, rcFill, rcTemp;
	const	COLORREF clr = RGB(227, 227, 227);
	CFont*	pOldFont = pDC->SelectObject(m_pFont);
	CPen*	pPen = m_pApp->GetPen(m_pWizard, m_colorLine, 1);
	CPen*	pPenWhite = m_pApp->GetPen(m_pWizard, RGB(255, 255, 255), 1);
	CPen*	pOldPen = pDC->SelectObject(pPen);

	rect = m_rcData;
	pDC->FillSolidRect(&rect, RGB(255, 255, 255));

	CString sPrice, sContract;
	const	double	height = double(rect.Height()) / double(m_iDispCnt + (m_showTitle ? 1 : 0));
	POSITION pos = NULL;
	std::shared_ptr<CMatch>	match = nullptr;

	pDC->SetTextColor(RGB(0, 0, 0));
	for (int ii = 0, row = 0; ii < m_SCRday; ii++)
	{
		row = ii + (m_showTitle ? 1 : 0);
		rcFill.SetRect(0, (int)(row * height), rect.Width() - 1, (int)((row + 1) * height));
		
		if (ii % 2 == 0)
			pDC->FillSolidRect(rcFill, m_colorGrid1st);
		else
			pDC->FillSolidRect(rcFill, m_colorGrid2st);

		pos = m_match.FindIndex(ii + m_SCRpos);
		if (pos)
		{
			match = m_match.GetAt(pos);
			sPrice = match->price;
			sContract = match->contract;
		}
		else
		{
			continue;
		}

		cRc.SetRect(0, (int)(row * height)+2, m_iHalfLine - 1, (int)((row + 1) * height));
		
		pDC->SetTextColor(m_pColor[match->cp]);
		pDC->DrawText(sPrice, cRc, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

		pDC->SetTextColor(m_pColor[match->cc]);
		cRc.SetRect(cRc.right, cRc.top, rect.Width() - 1, cRc.bottom);
		pDC->DrawText(sContract, cRc, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
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
		pDC->DrawText("ü�ᰡ", CRect(rcTemp.left, rcTemp.top, rcTemp.left + m_iHalfLine, rcTemp.bottom), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		pDC->DrawText("ü�ᷮ", CRect(rcTemp.left + m_iHalfLine, rcTemp.top, rcTemp.right, rcTemp.bottom), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		
		pDC->MoveTo(rcTemp.left, rcTemp.bottom);
		pDC->LineTo(rcTemp.right, rcTemp.bottom);
	}

	pDC->SelectObject(pPen);
	pDC->MoveTo(m_iHalfLine, rect.top);
	pDC->LineTo(m_iHalfLine, rect.bottom);
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.left, rect.bottom);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->LineTo(rect.right, rect.top);
	pDC->LineTo(rect.left, rect.top);

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}


BOOL CContractGrid::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;//CWnd::OnEraseBkgnd(pDC);
}

void CContractGrid::AutoUpdate()
{
	Invalidate();
	AutoScroll();
}

void CContractGrid::ChangeDisplay(int display)
{
	m_iDispCnt = display;
	AutoScroll();
	Invalidate();
}
