// Grp_Base.cpp: implementation of the CGrp_Base class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C_Bong.h"
#include "Grp_Base.h"
#include "Grp_Data.h"
#include "GrpWnd.h"

#include <FLOAT.H>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	VOLUNIT	1000000
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define	gapTICK	2

CGrp_Base::CGrp_Base(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo)
{
	m_pView = pView;
	m_pGrpWnd = pGrpWnd;

	struct _gInfo *pGInfo = (struct _gInfo*)pInfo;

	m_gKind = pGInfo->gKind;
	m_rKey = pGInfo->rKey;
	m_dKey = pGInfo->dKey;
	m_dKind = pGInfo->dKind;
	m_dIndex = pGInfo->dIndex;

	m_tRGB = pGInfo->tRGB;
	m_pRGB = pGInfo->pRGB;
	m_clrUp = m_pApp->GetColor(pView, CLR_UP);
	m_clrDn = m_pApp->GetColor(pView, CLR_DOWN);

	m_DrawRect = m_pGrpWnd->m_pRgnInfo[m_rKey]->gpRect;

	m_Max = DBL_MIN;
	m_Min = DBL_MAX;

	m_pApp = (CC_BongApp *)AfxGetApp();
}

CGrp_Base::~CGrp_Base()
{

}

void CGrp_Base::Resize()
{
	m_DrawRect = m_pGrpWnd->m_pRgnInfo[m_rKey]->gpRect;
	m_DrawRect.top += m_pGrpWnd->m_pRgnInfo[m_rKey]->tkHeight + 3;

	if (m_DrawRect.Width() > 50)
	{
		m_DrawRect.right = m_DrawRect.right - 5;
	}
}

CString CGrp_Base::GetDisplayPosHead(CPoint pt)
{
	CString	dispStr = _T("");

	if (m_DrawRect.left > pt.x || m_DrawRect.right <= pt.x)
		return dispStr;

	if (!m_pGrpWnd->m_pDataInfo[m_dKey] || 
		m_pGrpWnd->m_pDataInfo[m_dKey]->GetDataCount() <= 0)
		return dispStr;

	int	dispPos = m_pGrpWnd->m_dispPos;
	int	dispEnd = m_pGrpWnd->m_dispEnd;
	int	dispDay = m_pGrpWnd->m_dispDay;

	int	xPosition = pt.x - m_DrawRect.left;
	double	szOneDay = double(m_DrawRect.Width()) / double(dispDay);
	int	dataPos = int(double(xPosition) / szOneDay);
	dataPos += dispPos;

	CString	stmp, stmp2, sSign, sSign2;
	struct _cgBong	*gBong = 
		 (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(dataPos);
	if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
		dispStr.Format("�ð�\t%02d:%02d:%02d", gBong->index.time.hh, 
		gBong->index.time.mm, gBong->index.time.ss);
	else
		dispStr.Format("����\t%04d/%02d/%02d", gBong->index.date.yy, 
		gBong->index.date.mm, gBong->index.date.dd);

	return dispStr;
}

CString CGrp_Base::GetExcelHead(int idx)
{
	CString	dispStr = _T("");
	CString	stmp;
	struct _cgBong	*gBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(idx);

	if (idx < 0)
	{
		if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
			dispStr = "�ð�";
		else
			dispStr = "����";
	}
	else
	{
		if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
			dispStr.Format("%02d:%02d:%02d", gBong->index.time.hh, 
			gBong->index.time.mm, gBong->index.time.ss);
		else
			dispStr.Format("%04d/%02d/%02d", gBong->index.date.yy, 
			gBong->index.date.mm, gBong->index.date.dd);
	}

	return dispStr;
}

void CGrp_Base::DrawTickHorizon(CDC* pDC, double Max, double Min, int tickinfo)
{
	int	dispCount;
	double	dispValue[64];
	CRect	tickRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[tickinfo].tkRect;
	tickRC.top = m_DrawRect.top;

	int	tickHeight = tickRC.Height();
	double	ValueHeight = Max - Min;
	if (ValueHeight <= 0)	ValueHeight = 1;

	double	div;
	if (ValueHeight < 5)			div = 1;
	else if (ValueHeight < 50)		div = 5;
	else if (ValueHeight < 100)		div = 10;
	else if (ValueHeight < 500)		div = 50;
	else if (ValueHeight < 1000)		div = 100;
	else if (ValueHeight < 5000)		div = 500;
	else if (ValueHeight < 10000)		div = 1000;
	else if (ValueHeight < 50000)		div = 5000;
	else if (ValueHeight < 100000)		div = 10000;
	else if (ValueHeight < 500000)		div = 50000;
	else if (ValueHeight < 1000000)		div = 100000;
	else if (ValueHeight < 5000000)		div = 500000;
	else if (ValueHeight < 10000000)	div = 1000000;
	else if (ValueHeight < 50000000)	div = 5000000;
	else if (ValueHeight < 100000000)	div = 10000000;
	else if (ValueHeight < 500000000)	div = 50000000;
	else if (ValueHeight < 1000000000)	div = 100000000;
	else					div = 500000000;

	int dispTICK = GetTickCount(tickinfo);
	if (dispTICK <= 0)
		return;

	double	drawMinValue= 0.0;

	if (!calculateDisplay(div, Max, Min, drawMinValue, dispTICK, dispValue, dispCount))
	{
		return;
	}

	int		sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF	sColor = pDC->SetTextColor(m_pGrpWnd->m_ltColor);
	int		style = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[tickinfo].lstyle;
	int		width = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[tickinfo].lwidth;
	COLORREF	color = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[tickinfo].lcolor;

	CRect	textRC;
	CFont	*sFont = pDC->SelectObject(m_pGrpWnd->m_pFont);
	CPen	*cPen = m_pApp->GetPen(m_pView, style, width, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	double	dValue;
	CString	dispStr;
	CPoint	pointS, pointE;
	CRect	tkSaveRC = tickRC;

	CSize	size;
	int	maxdigit = 7;
	char	*digitStr[10] = {"2,000,000,000", "200,000,000", "20,000,000", 
		"2,000,000", "200,000", "20,000", "2,000", "200", "20", "2"};
	for (int ii = 9; ii >= 0; ii--)
	{
		size = pDC->GetOutputTextExtent(digitStr[ii]);
		if (size.cx >= tkSaveRC.Width())
			break;
		else
			maxdigit = strlen(digitStr[ii]);
	}

	CString	sUnit = _T("");
	if ((m_gKind == CGK_VOL || m_gKind == CGK_VMA) && m_Max > VOLUNIT)
		sUnit = "x1000";

	size = pDC->GetOutputTextExtent(" ");
	int	nGap = size.cx;

	FormatVariableComma(dispStr, Max, m_pGrpWnd->m_digit, maxdigit);
	size = pDC->GetOutputTextExtent(dispStr);
	int	PivotY = tickRC.left + gapTICK + size.cx + nGap*2;
	if (tickinfo == ctkLEFT)
		PivotY = tickRC.right - gapTICK;

	for (int ii = 0; ii < dispCount; ii++)		// ���ݺ����� (DOT)
	{
		pointS.x = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkLEFT].tkRect.right+1;
		dValue = double(double(tickHeight) * double(Max - dispValue[ii]));
		dValue /= double(ValueHeight);

		pointS.y = tickRC.top + int(dValue);

		if (!tickRC.PtInRect(CPoint(pointS.x -1, pointS.y)))
		{
			if (tickRC.bottom < pointS.y)
				continue;
		}

		pointE.x = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].tkRect.left;
		pointE.y = pointS.y;
		if (pointS.y > m_DrawRect.top && pointS.y < m_DrawRect.bottom)
		{
			pDC->MoveTo(pointS);
			pDC->LineTo(pointE);
		}

		FormatVariableComma(dispStr, dispValue[ii], m_pGrpWnd->m_digit, maxdigit);
		if (ii == 0 && !sUnit.IsEmpty())
			dispStr = sUnit;

		size = pDC->GetOutputTextExtent(dispStr);

		textRC.SetRect(tickRC.left, pointS.y - size.cy/2,
			PivotY, pointS.y + size.cy/2);

		if (textRC.bottom > tkSaveRC.bottom)
		{
			textRC.OffsetRect(0, tkSaveRC.bottom - textRC.bottom);
			tkSaveRC.bottom -= size.cy + 1;
		}
		if (textRC.top < tkSaveRC.top)
		{
			textRC.OffsetRect(0, tkSaveRC.top - textRC.top);
			tkSaveRC.top += size.cy + 1;
		}
		if (textRC.top < tickRC.top)
		{
			textRC.OffsetRect(0, tickRC.top - textRC.top);
		}

		textRC.right -= nGap;
		pDC->DrawText(dispStr, textRC, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
	}

	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

struct _dispInfo {
	char	dispStr[10];
	CRect	dispRC;
	CPoint	dispPt;
};
enum { dispNONE, dispONEHOUR, dispTHREEHOUR, dispONEDAY, dispTHREEDAY, dispSIXDAY};
void CGrp_Base::DrawTickBottom(CDC *pDC)
{
	if (!m_pGrpWnd->m_pDataInfo[m_dKey])
		return;

	CRect	orgRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->gpRect;
	orgRC.right = m_pGrpWnd->m_ObjRect.right;
	CRect	tickRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].tkRect;
	CRect	bottomRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].tkRect;
	tickRC.right = m_DrawRect.right;
	tickRC.left = m_DrawRect.left;
	int tickWidth = tickRC.Width();
	int tickHeight = tickRC.Height();

	int		sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF	sColor = pDC->SetTextColor(m_pGrpWnd->m_btColor);
	int		style = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lstyle;
	int		width = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lwidth;
	COLORREF	color = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lcolor;

	CFont	*boldFont = m_pApp->GetFont(m_pView, m_pGrpWnd->m_fPoint, m_pGrpWnd->m_fName, FW_BOLD);
	CFont	*sFont = pDC->SelectObject(boldFont);
	CPen	*cPen = m_pApp->GetPen(m_pView, style, width, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	CArray <_dispInfo, _dispInfo> arDispInfo;
	_dispInfo	dispInfo;
	arDispInfo.RemoveAll();

	CPoint	pointDAY;
	CString	dispStr;
	CRect	drawRC;
	CRect	saveRC(0,0,0,0);
	int	svYear = 0, svMonth = 0, svDay = 0;
	int	svHH = 0, svMM = 0;
	int	savePos = -20;
	int	gap = 0;
	pointDAY.y = tickRC.top + int(tickHeight / 5);

	int	dispPos = m_pGrpWnd->m_dispPos;
	int	dispEnd = m_pGrpWnd->m_dispEnd;
	int	dispDay = m_pGrpWnd->m_dispDay;
	int	endIdx = dispEnd - dispPos - 1;
	struct _cgBong	*cgBong;

	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		cgBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);
		pointDAY.x = tickRC.left;
		pointDAY.x += int((tickWidth * ii) / dispDay);
		pointDAY.x -= 1;	// �׷����� ����
		pointDAY.y = tickRC.top + int(tickHeight / 5);

		switch(m_dIndex)
		{
		case CDI_MON:			// �⵵
		case CDI_WEEK:			// 3�� 
			if (svYear == cgBong->index.date.yy)
				continue;
			dispStr.Format("%04d", cgBong->index.date.yy);
			svYear = cgBong->index.date.yy;
			break;
		case CDI_MIN:	case CDI_TICK:			// �ð�
			if (svHH == cgBong->index.time.hh)
				continue;

			gap = max(pointDAY.x - savePos, gap);
			savePos = pointDAY.x;
			svHH = cgBong->index.time.hh;

			if (ii != 0 && cgBong->index.date.dd == svDay)
				continue;
			
			if (ii == 0)
				dispStr.Format("%02d:%02d", cgBong->index.time.hh,
					cgBong->index.time.mm);
			else
				dispStr.Format("%02d/%02d", 
					cgBong->index.date.mm, cgBong->index.date.dd);

			svDay = cgBong->index.date.dd;

			break;
		case CDI_DAY:			// ��
		default:
			if (svYear == cgBong->index.date.yy && svMonth == cgBong->index.date.mm)
				continue;

			svMonth  = cgBong->index.date.mm;
			gap = max(pointDAY.x - savePos, gap);
			savePos = pointDAY.x;

			if (svYear == cgBong->index.date.yy)
				continue;

			dispStr.Format("%04d/%02d", cgBong->index.date.yy, cgBong->index.date.mm);
			svYear = cgBong->index.date.yy;
			
			break;
		}

		drawRC = DisplayRect(pDC, ii, dispStr, pointDAY);
		drawRC.left -= 4;
		drawRC.right += 4;

		dispInfo.dispRC = drawRC;
		memset(&dispInfo.dispStr, 0x00, sizeof(dispInfo.dispStr));
		memcpy(&dispInfo.dispStr, dispStr, dispStr.GetLength());
		dispInfo.dispPt = pointDAY;
		arDispInfo.Add(dispInfo);
	}
		
	int view = dispONEDAY;
	if (m_dIndex == CDI_DAY)
	{
		if (gap < 13)
			view = dispNONE;
		else if (gap >= 13 && gap < 20)
			view = dispSIXDAY;
		else if (gap >= 20 && gap < 30)
			view = dispTHREEDAY;
	}
	else if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
	{
		if (gap > 55)
			view = dispONEHOUR;
		else if (gap >= 15 && gap < 55)
			view = dispTHREEHOUR;
		else
			view = dispNONE;
	}
	else
		view = dispNONE;

	bool bText = true;
	CRect unionRC;
	CRect cmpRC;

	saveRC.SetRectEmpty();
	int arSize = arDispInfo.GetSize();
	for (int ii = 0; ii < arSize; ii++)
	{
		bText = true;
		dispInfo = arDispInfo.GetAt(ii);
		// ���ʰ�
		if ((m_dIndex != CDI_MIN && m_dIndex != CDI_TICK) && ii == 0 && arSize > 1)
		{
			cmpRC = arDispInfo.GetAt(1).dispRC;
			unionRC = cmpRC & dispInfo.dispRC;
			if (!unionRC.IsRectEmpty())
				bText = false;
		}

		if (dispInfo.dispPt.x > tickRC.left && dispInfo.dispPt.x < tickRC.right)
		{
			pDC->MoveTo(dispInfo.dispPt.x, orgRC.top);
			pDC->LineTo(dispInfo.dispPt.x, orgRC.bottom);
		}
		
		if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
		{
			unionRC = saveRC & dispInfo.dispRC;
			if (!unionRC.IsRectEmpty())
				continue;
			saveRC = dispInfo.dispRC;
		}
		pDC->MoveTo(dispInfo.dispPt.x, m_DrawRect.bottom);
		pDC->LineTo(dispInfo.dispPt.x, m_DrawRect.bottom+6);
		
		if (bText)
		{
			if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
			{
				if (view != dispNONE && ii != 0)
					pDC->SelectObject(&boldFont);
				else
					pDC->SelectObject(m_pGrpWnd->m_pFont);
				
			}
			if (dispInfo.dispRC.right < orgRC.right)
				pDC->DrawText(dispInfo.dispStr, &dispInfo.dispRC, DT_SINGLELINE|DT_TOP|DT_CENTER);
		}
	}

	pDC->SelectObject(m_pGrpWnd->m_pFont);
	if (view > 0)
	{
		int jj = 0;
		bool bDraw = true;
		saveRC.SetRectEmpty();
		
		for (int ii = 0; ii <= endIdx; ii++)
		{
			cgBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);
			
			switch (m_dIndex)
			{
			case CDI_DAY:
				if (svYear == cgBong->index.date.yy 
					&& svMonth == cgBong->index.date.mm)
					continue;
				switch (view)
				{
				case dispTHREEDAY:
					if (cgBong->index.date.mm % 3 != 1)
						continue;
					break;
				case dispSIXDAY:
					if (cgBong->index.date.mm % 6 != 1)
						continue;
					break;
				}
				dispStr.Format("%02d", cgBong->index.date.mm);
				svYear = cgBong->index.date.yy;
				svMonth  = cgBong->index.date.mm;
				break;
			case CDI_MIN:	case CDI_TICK:
				if (svHH == cgBong->index.time.hh && ii != endIdx)
					continue;
				switch (view)
				{
				case dispTHREEHOUR:
					if (ii != endIdx && cgBong->index.time.hh % 12 != 0)
					{
						svHH = cgBong->index.time.hh;
						continue;
					}
					break;
				}
				
				if (ii != endIdx)
					dispStr.Format("%02d", cgBong->index.time.hh);
				else
					dispStr.Format("%02d:%02d", 
						cgBong->index.time.hh, cgBong->index.time.mm);

				svHH = cgBong->index.time.hh;
				break;
			}

			// �׷����� ����
			pointDAY.x = tickRC.left;
			pointDAY.x += int((tickWidth * ii) / dispDay);
			pointDAY.x -= 1;
			pointDAY.y = tickRC.top + int(tickHeight / 5);

			drawRC = DisplayRect(pDC, ii, dispStr, pointDAY);
			unionRC = saveRC & drawRC;
			if (!unionRC.IsRectEmpty())
				continue;
			
			bDraw = true;
			for (jj = 0; jj < arDispInfo.GetSize(); jj++)
			{
				if (arDispInfo.GetAt(jj).dispPt.x == pointDAY.x)
				{
					bDraw = false;
					break;
				}
			}
			for (jj = 0; jj < arDispInfo.GetSize(); jj++)
			{
				cmpRC = arDispInfo.GetAt(jj).dispRC;
				unionRC = cmpRC & drawRC;
				if (!unionRC.IsRectEmpty())
				{
					bDraw = false;
					break;
				}
			}

			if (bDraw)
			{
				int Pixgap = drawRC.right - m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].tkRect.right;
				if (Pixgap > 0)	drawRC.OffsetRect(Pixgap*(-1), 0);
				for (jj = 0; jj < arDispInfo.GetSize(); jj++)
				{
					cmpRC = arDispInfo.GetAt(jj).dispRC;
					unionRC = cmpRC & drawRC;
					if (!unionRC.IsRectEmpty())
					{
						bDraw = false;
						break;
					}
				}
				pDC->MoveTo(pointDAY.x, orgRC.top);
				pDC->LineTo(pointDAY.x, orgRC.bottom);
				pDC->MoveTo(pointDAY.x, orgRC.bottom);
				pDC->LineTo(pointDAY.x, orgRC.bottom+3);

				if (bDraw)
				{
					pDC->DrawText(dispStr, &drawRC, DT_SINGLELINE|DT_TOP|DT_CENTER);
					saveRC = drawRC;
				}
			}
		}
	}

	arDispInfo.RemoveAll();
	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

CRect CGrp_Base::DisplayRect(CDC *pDC, int idx, CString dispStr, CPoint dispPt)
{
	CRect dispRC = CRect(0, 0, 0, 0);
	CSize size = pDC->GetOutputTextExtent(dispStr);
	if (idx == 0)
		dispRC.SetRect(dispPt.x, dispPt.y+2, 
				dispPt.x + size.cx, dispPt.y + size.cy);
	else
		dispRC.SetRect(dispPt.x - size.cx/2, dispPt.y+2, 
				dispPt.x + size.cx/2+1, dispPt.y + size.cy+2);

	return dispRC;
}

int CGrp_Base::DrawTickIndex(CDC* pDC, CString dispText, COLORREF tColor, bool bColor, bool bIndex, CPoint pt)
{
	if (dispText.IsEmpty())
		return 0;

	int sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF sColor;
	if (bColor)	sColor = pDC->SetTextColor(tColor);
	else		sColor = pDC->SetTextColor(m_tRGB);
	CFont *sFont = pDC->SelectObject(m_pGrpWnd->m_pFont);

	CString	realSizeStr = dispText;
	CSize	size = pDC->GetOutputTextExtent(realSizeStr);
	CSize	size2 = pDC->GetOutputTextExtent("D");
	size.cx = size.cx + size2.cx/2;
	size2.cx = size2.cx + size2.cx/2;

	if (bIndex)
	{
		if (pt.x < m_DrawRect.left)
			pt.x = m_DrawRect.left;
		else if ((pt.x + size.cx) > m_DrawRect.right)
			pt.x = m_DrawRect.right - size.cx;
		if (pt.y < m_DrawRect.top)
			pt.y = m_DrawRect.top;
		else if ((pt.y + size.cy) > m_DrawRect.bottom)
			pt.y = m_DrawRect.bottom - size.cy;

		CRect	dispRC;
		dispRC.SetRect(pt.x, pt.y, pt.x + size.cx, pt.y + size.cy);
		pDC->DrawText(dispText, &dispRC, DT_SINGLELINE|DT_TOP|DT_LEFT);

		pDC->SelectObject(sFont);
		pDC->SetTextColor(sColor);
		pDC->SetBkMode(sMode);

		return pt.x + size.cx;
	}

	struct _MinMaxChk	*pMinMaxChk = m_pGrpWnd->GetMinMaxChk(m_rKey);
	if (pMinMaxChk->tickx == 0)
	{
		pt.x = m_pGrpWnd->m_pRgnInfo[m_rKey]->gpRect.left + 5;
		pt.y = m_pGrpWnd->m_pRgnInfo[m_rKey]->gpRect.top + 3;
	}
	else
	{
		if (pMinMaxChk->tickx + size.cx > m_DrawRect.right)
		{
			pt.x = m_pGrpWnd->m_pRgnInfo[m_rKey]->gpRect.left + 5;
			pt.y = pMinMaxChk->ticky + size.cy;
		}
		else
		{
			pt.x = pMinMaxChk->tickx;
			pt.y = pMinMaxChk->ticky;
		}
	}

	if (pt.y + size.cy > m_DrawRect.bottom)
	{
		pDC->SelectObject(sFont);
		pDC->SetTextColor(sColor);
		pDC->SetBkMode(sMode);

		return 0;
	}

	CRect	dispRC;
	CRect	legendRC;
	dispRC.SetRect(pt.x, pt.y, pt.x + size.cx, pt.y + size.cy);
	dispRC.NormalizeRect();
	pDC->DrawText(dispText, &dispRC, DT_SINGLELINE|DT_TOP|DT_LEFT);
	legendRC = dispRC;

	pDC->SelectObject(sFont);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);

	pMinMaxChk->tickx = pt.x + size.cx;
	pMinMaxChk->ticky = pt.y;

	return pt.x + size.cx;
}

void CGrp_Base::FormatVariableComma(CString &dispTxt, double dVal, int point, int maxDigit)
{
	if (m_gKind == CGK_VOL)
		point = 0;

	CString	edit1, edit2;
	double dotL = 0, dotR = 0;

	bool bSign = false;
	if (dVal < 0)
	{
		bSign = true;
		dVal = dVal * (-1);
	}
	switch (point)
	{
	case 1:
		dotL = int(dVal / 10.0);
		dotR = int(int(dVal) % 10);
		break;
	case 2:
		dotL = int(dVal / 100.0);
		dotR = int(int(dVal) % 100);
		break;
	case 3:
		dotL = int(dVal / 1000.0);
		dotR = int(int(dVal) % 1000);
		break;
	case 4:
		dotL = int(dVal / 10000.0);
		dotR = int(int(dVal) % 10000);
		break;
	case 5:
		dotL = int(dVal / 100000.0);
		dotR = int(int(dVal) % 100000);
		break;
	case 6:
		dotL = int(dVal / 1000000.0);
		dotR = int(int(dVal) % 1000000);
		break;
	default:
		dotL = dVal;
		break;
	}

	if ((m_gKind == CGK_VOL || m_gKind == CGK_VMA) && m_Max > VOLUNIT)
		dotL /= 1000.;

	dispTxt = _T("");
	edit1.Format("%.f", dotL);
	edit1.MakeReverse();
	for (int ii = 0; ii < edit1.GetLength(); ii++)
	{
		if (ii && !(ii % 3))
			edit2 += ',';
		edit2 += edit1[ii];
	}
	if (bSign) edit2 += "-";
	edit2.MakeReverse();

	edit1= _T("");
	switch (point)
	{
	case 1:
		edit1.Format(".%0d", int(dotR+0.5));
		break;
	case 2:
		edit1.Format(".%02d", int(dotR+0.5));
		break;
	case 3:
		edit1.Format(".%03d", int(dotR+0.5));
		break;
	case 4:
		edit1.Format(".%04d", int(dotR+0.5));
		break;
	case 5:
		edit1.Format(".%05d", int(dotR+0.5));
		break;
	case 6:
		edit1.Format(".%06d", int(dotR+0.5));
		break;
		break;
	default:
		break;
	}
/*
	int trimComma = edit2.GetLength() + edit1.GetLength() - maxDigit;
	if (trimComma > 0)
	{
		int trim = 0;
		for (int ii = 0; ii < edit2.GetLength(); ii++)
		{
			if (edit2[ii] == ',' && trim < trimComma)
			{
				trim++;
				continue;
			}
			dispTxt += edit2[ii];
		}
		dispTxt += edit1;
	}
	else
	{
*/		dispTxt = edit2 + edit1;
//	}
}

int CGrp_Base::GetTickCount(int dir)
{
	int	dispTICK = 5;
	int	tkHeight;

	CRect	RC;
	switch (dir)
	{
	case ctkLEFT:
		RC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkLEFT].tkRect;
		RC.top = m_DrawRect.top;
		break;
	case ctkRIGHT:
		RC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].tkRect;
		RC.top = m_DrawRect.top;
		break;
	case ctkBOTTOM:
		RC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].tkRect;
		break;
	}

	tkHeight = RC.Height();

	dispTICK = int(tkHeight / (m_pGrpWnd->m_pRgnInfo[m_rKey]->tkHeight * 1.5));

	if (dispTICK > 10)
		dispTICK = 10;

	return dispTICK;
}

bool CGrp_Base::calculateDisplay(double div, double Max, double Min, double& drawMinValue, 
		int dispTICK, double* dispValue, int& dispCount)
{
	int	index;
	CString	temp;
	double	mok = Min / div;
	temp.Format("%f", mok);
	index = temp.Find('.'); 
	mok = atof(temp.Left(index));
	drawMinValue = mok * div;
	if (drawMinValue < Min)		
		drawMinValue += div;
	int	drawNum = int ((Max - drawMinValue) / div);
	if (drawNum >= dispTICK)
	{
		if (drawNum <= dispTICK*2)
			div *= 2;
		else if (drawNum <= dispTICK*4)
			div *= 4;
		else if (drawNum <= dispTICK*5)
			div *= 5;
		else
			div *= 6;

		mok = Min / div;
		temp.Format("%f", mok);
		index = temp.Find('.'); 
		mok = atof(temp.Left(index));
		drawMinValue = mok * div;
		if (drawMinValue < Min)		
			drawMinValue += div;
	}
	dispCount = 0;
	dispValue[dispCount] = drawMinValue;
	double dVal = 0.0;

	while(1)
	{
		if (dispValue[dispCount] > Max + 0.01)
			break;

		dispValue[dispCount+1] = dispValue[dispCount] + div;
		dispCount++;
	}
	if (dispCount <= 0)
	{
		if (Max == Min)
		{
			dispValue[0] = Max;
			dispCount = 1;
		}
		else
			return false;
	}
	return true;
}

void CGrp_Base::GetMinMaxValue(double& dMin, double& dMax)
{
	dMin = m_Min;
	dMax = m_Max;
}
