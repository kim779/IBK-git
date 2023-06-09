#include "stdafx.h"
#include "testctrl.h"
#include "Ctrl.h"

CExSlider::CExSlider()
{
}

CExSlider::~CExSlider()
{
}

BEGIN_MESSAGE_MAP(CExSlider, CSliderCtrl)
	//{{AFX_MSG_MAP(CExSlider)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////
// ScrollBar
/////////////////////////////////////////////////////////////

CExScrollBar::CExScrollBar()
{
}

CExScrollBar::~CExScrollBar()
{
}


BEGIN_MESSAGE_MAP(CExScrollBar, CScrollBar)
	//{{AFX_MSG_MAP(CExScrollBar)
	ON_WM_HSCROLL_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExScrollBar::Initialize(SCROLLINFO sInfo)
{
	SetScrollInfo(&sInfo);
	if ((int)sInfo.nPage >= sInfo.nMax)
		EnableScrollBar(ESB_DISABLE_BOTH);
	else
		EnableScrollBar(ESB_ENABLE_BOTH);
}

void CExScrollBar::HScroll(UINT nSBCode, UINT nPos)
{
	const	int	pos = GetScrollPos();
	SCROLLINFO sInfo;

	GetScrollInfo(&sInfo, SIF_ALL);
	switch (nSBCode)
	{
	case SB_LINEDOWN:
	case SB_LINEUP:
		GetOwner()->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_ChgScrPos, nSBCode), 0);
		break;
	case SB_PAGEDOWN:
		if ((nPos = getPos(SB_PAGEDOWN, sInfo.nPage)) >= 0)
			SetScrollPos(nPos);
		if (pos != GetScrollPos())
			GetOwner()->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_ChgScrPos, nSBCode), GetScrollPos());
		break;
	case SB_PAGEUP:
		if ((nPos = getPos(SB_PAGEUP)) >= 0)
			SetScrollPos(nPos);
		if (pos != GetScrollPos())
			GetOwner()->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_ChgScrPos, nSBCode), GetScrollPos());
		break;
	case SB_THUMBTRACK:
		SetScrollPos(nPos);
		if (pos != GetScrollPos())
			GetOwner()->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_ChgScrPos, nSBCode), GetScrollPos());
		break;
	default:
		break;
	}
}

int CExScrollBar::getPos(int type, int gap)
{
	const	int cx = GetSystemMetrics(SM_CXHSCROLL);
	int	nPos = -1;
	CPoint	pt;
	CRect scrollRC;

	GetCursorPos(&pt);
	GetOwner()->ScreenToClient(&pt);
	GetClientRect(scrollRC);
	ClientToScreen(scrollRC);

	GetOwner()->ScreenToClient(scrollRC);
	scrollRC.left += cx;
	scrollRC.right -= cx;
	if (scrollRC.PtInRect(pt))
	{
		const	int	xpos = pt.x - scrollRC.left;

		nPos = (int)(100. * xpos / (double)scrollRC.Width());
		switch (type)
		{
		case SB_PAGEDOWN:
			nPos = nPos - gap + 2;
			break;
		case SB_PAGEUP:
			break;
		}
	}
	return nPos;
}