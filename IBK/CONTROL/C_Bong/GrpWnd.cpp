// GrpWnd.cpp : implementation file
//

#include "stdafx.h"
#include "C_Bong.h"
#include "GrpWnd.h"
#include "memDC.h"
#include "Grp_Data.h"
#include "Grp_Base.h"
#include "Grp_Juga.h"
#include "Grp_Vol.h"
#include "Grp_MA.h"
#include "ToolTip.h"
#include "CrossLine.h"
#include "CtrlWnd.h"
#include "MainWnd.h"

#include "../../h/axiscgrp.h"
#include <math.h>
#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ECN realtime Symbol
#define	ER_HNAM	"722"	// �����
#define	ER_CURR	"723"	// ���簡
#define	ER_DIFF	"724"	// ���ϴ��
#define	ER_MDGA	"725"	// �ŵ�ȣ��
#define	ER_MSGA	"726"	// �ż�ȣ��
#define	ER_GVOL	"727"	// �ŷ���
#define	ER_GAMT	"728"	// �ŷ����(��)
#define	ER_SIGA	"729"	// �ð�
#define	ER_KOGA	"730"	// ��
#define	ER_JEGA	"731"	// ����
#define	ER_CVOL	"732"	// ü��ŷ���(ü�����)
#define	ER_UDYL	"733"	// �����
#define	ER_CTIM	"734"	// ü��ð�
#define	ER_CCOL	"735"	// ü�ᱸ��
#define	ER_HTIM	"740"	// ȣ���ð�
/////////////////////////////////////////////////////////////////////////////
// CGrpWnd

#define	TIMER_PID	10000
#define	TIMER_PGAP	250
#define	TIMER_ID	10001
#define	TIMER_GAP	1000

CGrpWnd::CGrpWnd(CWnd *pView, CWnd *pParent, struct _param *pInfo)
{
	m_pView = pView;
	m_pParent = pParent;
	m_pToolTip = new CToolTip(m_pView, this);
	m_pCrossLine = NULL;
	
	m_RgnCnt = m_DataCnt = m_GrpCnt = 0;
	for (int ii = 0; ii < maxCnt; ii++)
	{
		m_pRgnInfo[ii] = NULL;
		m_pDataInfo[ii] = NULL;
		m_pGrpInfo[ii] = NULL;
	}

	m_pMinMaxChk = NULL;
	m_MinMaxCnt = 0;

	m_bVol = m_bOutLine = false;
	m_jChart = 0;
	m_dKind = CDK_BONG;
	m_dIndex = CDI_DAY;
	m_dCount = 100;
	m_dDay = 100;
	m_dUnit = CDU_JONGMOK;

	m_pivot = -100;
	m_PMACnt = m_VMACnt = 0;
	m_PMA[0] = m_PMA[1] = m_PMA[2] = m_PMA[3] = 0;
	m_VMA[0] = m_VMA[1] = m_VMA[2] = m_VMA[3] = 0;
	m_digit = 0;
	m_power = 1;
	m_fPoint = pInfo->point;
	m_fName.Format("%s", pInfo->fonts);
	m_tRGB = GetPalette(pInfo->tRGB);
	m_pRGB = GetPalette(pInfo->pRGB);
	m_fRGB = GetPalette(BGIndex);

	m_TickView = CDT_LEFT;
	m_ObjRect = pInfo->rect;
	ParseParam((char*)pInfo->options.operator LPCTSTR());	

	m_pFont = NULL;
	m_ltColor = m_tRGB;
	m_rtColor = m_tRGB;
	m_btColor = m_tRGB;

	m_PosData = _T("");
	m_timerID = 0;

	m_pApp = (CC_BongApp *)AfxGetApp();
}

CGrpWnd::~CGrpWnd()
{
}


BEGIN_MESSAGE_MAP(CGrpWnd, CWnd)
	//{{AFX_MSG_MAP(CGrpWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(CM_GRP, OnGrp)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGrpWnd message handlers

LONG CGrpWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:	// set data from server
		m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_Clear, 0), 0);
		return DispatchData(wParam, lParam);
		break;
	case DLL_INB:	// get data from out
		return RequestHead();
		break;
	case DLL_ALERTx:	// realtime data		
		RealTimeData((char *)lParam);
		break;
	case DLL_SETPAL:
		m_fRGB = GetPalette(BGIndex);
		Invalidate();
		break;
	case DLL_SETFONT:
		{
			m_fPoint = HIWORD(wParam);
			CString fName = (char *)lParam;
			if (!fName.IsEmpty())
				m_fName.Format("%s", fName);

			m_pFont = m_pApp->GetFont(m_pView, m_fPoint, m_fName);

			Resize();
			Invalidate();
		}
		break;
	}

	return ret;
}

LONG CGrpWnd::OnGrp(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;

	switch (LOWORD(wParam))
	{
	case GRP_Control:
		switch (HIWORD(wParam))
		{
		case GetScrData:
			{
				CString	tmpS;
				tmpS.Format("%d\t%d\t%d\t%d", m_totalDay, m_dispPos, m_dispEnd, m_dispDay);
				memcpy((char *)lParam, tmpS, tmpS.GetLength());
				return tmpS.GetLength();
			}
			break;
		case ChgDispPos:	// change pos, LOWORD(lParam) : dispPos, HIWORD(lParam) : dispEnd
			m_dispPos = LOWORD(lParam);
			m_dispEnd = HIWORD(lParam);
			RecalculateMinMax();
			Invalidate();
			break;
		case ChgDispScale:	// change scale, lParam:scale
			if (ChangeDisplayScale(lParam))
			{
				RecalculateMinMax();
				Invalidate();
			}
			break;
		case ChgDisplay:	// change display, lParam:display cnt
			if (ChangeDisplay(lParam))
			{
				RecalculateMinMax();
				Invalidate();
				return TRUE;
			}
			break;
		case autoScroll:
			break;
		case ChgShift:
			return ChangeDisplayShift(lParam);
		case ExcelData:
			ExcelProc();
			break;
		}
		break;
	}

	return ret;
}

int CGrpWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pToolTip->Create();
	m_pToolTip->ShowWindow(SW_HIDE);

	m_pFont = m_pApp->GetFont(m_pView, m_fPoint, m_fName);
	
	return 0;
}

void CGrpWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMemDC	mdc(&dc);
	DrawGraph(&mdc);
}

void CGrpWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (cx == 0 || cy == 0)
		return;

	Resize();
}

void CGrpWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TIMER_PID)
	{
		KillTimer(nIDEvent);
		if (IsPtInGrp())
		{
			CPoint	pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			CString str = GetTipStr(pt);

			if (m_PosData == str)
				MouseMove(true, pt);
		}
	}

	if (nIDEvent == TIMER_ID)
	{	
		if (!IsPtInGrp())
		{
			KillTimer(nIDEvent);
			m_pToolTip->ShowWindow(SW_HIDE);
			m_pCrossLine->HideCrossLine();
			m_timerID = 0;
		}
	}
	
	CWnd::OnTimer(nIDEvent);
}

void CGrpWnd::OnDestroy() 
{
	if (m_timerID > 0)
		KillTimer(m_timerID);

	if (m_pToolTip)
	{
		m_pToolTip->DestroyWindow();
		delete m_pToolTip;
		m_pToolTip = NULL;
	}

	RemoveComponent();

	CWnd::OnDestroy();
}

void CGrpWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CString	str = GetTipStr(point);
	if (m_PosData != str)
	{
		m_pToolTip->ShowWindow(SW_HIDE);
		m_PosData.Format("%s", str);
		KillTimer(TIMER_PID);
		SetTimer(TIMER_PID, TIMER_PGAP, NULL);
	}

	MouseMove(false, point, true);
	
	CWnd::OnMouseMove(nFlags, point);
}

int CGrpWnd::DispatchData(WPARAM wParam, LPARAM lParam)
{
	//int	dlen = HIWORD(wParam);
	//char	*pData = (char *)lParam;

	struct _extTHx* exTH = (struct _extTHx*)lParam;
	const int dlen = exTH->size;
	char* pData = exTH->data;

	RemoveComponent();

	if (dlen < sz_CDJUGA)
	{
		Invalidate();
		return dlen;
	}

	if (!AssignRegion())
	{
		Invalidate();
		return 0;
	}

	struct _cdJuga	*pcdJuga = (struct _cdJuga *)pData;
	int	dCnt = CGrp_Data::CharToInt(pcdJuga->nData, sizeof(pcdJuga->nData));
	if (dCnt <= 0)
	{
		Invalidate();
		return sz_CDJUGA;
	}

	m_digit = CGrp_Data::CharToInt(pcdJuga->digit, sizeof(pcdJuga->digit));
	m_power = pow(10.0, m_digit);
	m_totalDay = dCnt;
	int mgap = CGrp_Data::CharToInt(pcdJuga->mgap, sizeof(pcdJuga->mgap));
	if (mgap >= 900)
		mgap = mgap - 900;
	else
		mgap = mgap * 60;
	
	int	dPos = sz_CDJUGA;
	m_pDataInfo[m_DataCnt] = new CGrp_Data(this, m_dKind, 0, m_dIndex);
	m_pDataInfo[m_DataCnt]->SetDate(pcdJuga->pday);

	int	szFrame = m_pDataInfo[m_DataCnt]->AttachGraphData(&pData[dPos], m_totalDay, mgap);
	dPos += szFrame * m_totalDay;
	m_DataCnt++;

	if (m_dDay == 0 || m_dDay > m_totalDay)
		m_dispDay = m_totalDay;
	else
		m_dispDay = m_dDay;
	m_dispPos = m_totalDay - m_dispDay;
	m_dispEnd = m_dispPos + m_dispDay;

	m_pCrossLine = new CCrossLine(m_pView, this, m_pRGB, RGB(180,77,77));

	AssignGraph();
	ReviseTick();
	Resize();
	Invalidate();

	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_Enable, TRUE), 0);
	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_UpdateScr, 0), 0);

	return 0;
}

void CGrpWnd::RealTimeData(CString sRTM)
{
	CString str = sRTM;
	CStringList list;
	int find = 0, index;
	while (TRUE)
	{
		index = str.Find('\n', find);
		if (index < 0)
			break;

		list.AddHead(str.Mid(find, index - find));
		find = index + 1;
	}

	if (find < str.GetLength() - 1)
		list.AddHead(str);

	bool	bShift = false;
	CString	sPrice, sVolume;
	for (POSITION pos = list.GetHeadPosition(); pos; )
	{
		str = list.GetNext(pos);
		if (Alert(str))
			bShift = true;
	}

	bool	bRedraw = false;
	for (int ii = 0; ii < m_GrpCnt; ii++)
	{
		bool bRet = m_pGrpInfo[ii]->IsChangeMinMax(bShift);
		if (bRet)	bRedraw = true;
	}

	if (bRedraw || bShift)
	{
		Invalidate();
	}
	else
	{
		int width = int(double(m_ObjRect.Width())*5 / m_dispDay);
		CRect	RC = CRect(m_ObjRect.right - width, m_ObjRect.top, 
			m_ObjRect.right, m_ObjRect.bottom);

		InvalidateRect(&RC);
	}
}

bool CGrpWnd::Alert(CString sRTM)
{
	CString	rtm = sRTM;
	CMapStringToString	rtmStore;
	CString	val, symbol;
	int	index;
	for (; !rtm.IsEmpty(); )
	{
		index = rtm.Find('\t');
		if (index < 0)
			break;
		symbol = rtm.Left(index++);
		rtm = rtm.Mid(index);

		index = rtm.Find('\t');
		if (index < 0)
			index = rtm.GetLength();
		val = rtm.Left(index++);

		rtmStore.SetAt(symbol, val);

		if (rtm.GetLength() <= index)
			break;
		rtm = rtm.Mid(index);
	}

	m_siga = m_koga = m_jega = m_jgga = _T("");
	m_gvol = m_ctim = _T("");

	CString	sVal;

	if (rtmStore.Lookup(R_CURR, sVal))
	{
		double dVal = atof(sVal) * m_power;
		if (dVal < 0)	dVal *= (-1);
		m_jgga.Format("%.f", dVal);
	}
	if (rtmStore.Lookup(R_CTIM, sVal))
	{
		m_ctim = sVal;
	}

	if (m_dIndex == CDI_DAY)
	{
		if (rtmStore.Lookup(R_SIGA, sVal))
		{
			double dVal = atof(sVal) * m_power;
			if (dVal < 0)	dVal *= (-1);
			m_siga.Format("%.f", dVal);
		}
		if (rtmStore.Lookup(R_KOGA, sVal))
		{
			double dVal = atof(sVal) * m_power;
			if (dVal < 0)	dVal *= (-1);
			m_koga.Format("%.f", dVal);
		}
		if (rtmStore.Lookup(R_JEGA, sVal))
		{
			double dVal = atof(sVal) * m_power;
			if (dVal < 0)	dVal *= (-1);
			m_jega.Format("%.f", dVal);
		}
		if (rtmStore.Lookup(R_GVOL, sVal))
		{
			m_gvol = sVal;
		}
	}
	else
	{
		if (rtmStore.Lookup(R_CVOL, sVal))
			m_gvol.Format("%.f", fabs(atof(sVal)));
	}

	rtmStore.RemoveAll();

	if (m_jgga.IsEmpty() || m_ctim.IsEmpty())
		return false;

	bool	bShift = false;
	for (int ii = 0; ii < m_DataCnt; ii++)
	{
		bool bRet = m_pDataInfo[ii]->UpdateRTM();
		if (bRet)	bShift = true;
	}

	return bShift;
}

bool CGrpWnd::AssignRegion()
{
	if (!m_bVol && !m_jChart)
		return false;

	if (m_jChart)
	{
		m_pRgnInfo[m_RgnCnt] = new _RgnInfo;
		m_RgnCnt++;
	}
	if (m_bVol && !m_bSameRegion)
	{
		m_pRgnInfo[m_RgnCnt] = new _RgnInfo;
		m_RgnCnt++;
	}

	for (int ii = 0; ii < m_RgnCnt; ii++)
	{
		m_pRgnInfo[ii]->tick[ctkLEFT].bDraw = (m_TickView & CDT_LEFT)? true:false;
		m_pRgnInfo[ii]->tick[ctkRIGHT].bDraw = (m_TickView & CDT_RIGHT)? true:false;
		m_pRgnInfo[ii]->tick[ctkLEFT].lcolor = RGB(208, 208, 208);
		m_pRgnInfo[ii]->tick[ctkLEFT].lstyle = PS_SOLID;
		m_pRgnInfo[ii]->tick[ctkLEFT].lwidth = 1;
		m_pRgnInfo[ii]->tick[ctkRIGHT].lcolor = RGB(208, 208, 208);
		m_pRgnInfo[ii]->tick[ctkRIGHT].lstyle = PS_SOLID;
		m_pRgnInfo[ii]->tick[ctkRIGHT].lwidth = 1;
		m_pRgnInfo[ii]->tick[ctkBOTTOM].lcolor = RGB(208, 208, 208);
		m_pRgnInfo[ii]->tick[ctkBOTTOM].lstyle = PS_SOLID;
		m_pRgnInfo[ii]->tick[ctkBOTTOM].lwidth = 1;
	}

	return true;
}

bool CGrpWnd::AssignGraph()
{
	if (!m_bVol && !m_jChart)
		return false;

	int	rKey = 0;
	struct _gInfo	gInfo;
	if (m_jChart)
	{
		gInfo.dIndex = m_dIndex;
		gInfo.dKey = 0;
		gInfo.dKind = m_dKind;
		gInfo.gKind = m_jChart;
		gInfo.rKey = rKey;
		gInfo.pRGB = RGB(255, 255, 255);
		gInfo.tRGB = RGB(10, 10, 10);

		m_pGrpInfo[m_GrpCnt] = new CGrp_Juga(m_pView, this, (char *)&gInfo);
		m_GrpCnt++;

		if (m_PMACnt)
		{
			gInfo.dIndex = m_dIndex;
			gInfo.dKey = 0;
			gInfo.dKind = m_dKind;
			gInfo.gKind = CGK_PMA;
			gInfo.rKey = rKey;
			gInfo.pRGB = RGB(255, 255, 255);
			gInfo.tRGB = RGB(0, 0, 255);

			m_pGrpInfo[m_GrpCnt] = new CGrp_MA(m_pView, this, (char *)&gInfo);
			m_GrpCnt++;
		}

		rKey++;
	}

	if (m_bSameRegion)	rKey = 0;

	if (m_bVol)
	{
		gInfo.dIndex = m_dIndex;
		gInfo.dKey = 0;
		gInfo.dKind = m_dKind;
		gInfo.gKind = CGK_VOL;
		gInfo.rKey = rKey;
		gInfo.pRGB = RGB(255, 255, 255);
		gInfo.tRGB = RGB(0, 0, 0);

		m_pGrpInfo[m_GrpCnt] = new CGrp_Vol(m_pView, this, (char *)&gInfo);
		m_GrpCnt++;

		if (m_VMACnt)
		{
			gInfo.dIndex = m_dIndex;
			gInfo.dKey = 0;
			gInfo.dKind = m_dKind;
			gInfo.gKind = CGK_VMA;
			gInfo.rKey = rKey;
			gInfo.pRGB = RGB(255, 255, 255);
			gInfo.tRGB = RGB(0, 0, 255);

			m_pGrpInfo[m_GrpCnt] = new CGrp_MA(m_pView, this, (char *)&gInfo);
			m_GrpCnt++;
		}

		rKey++;
	}

	return true;
}

void CGrpWnd::DrawGraph(CDC *pDC)
{
	m_cs.Lock();

	DrawEmpty(pDC, true);
	if (m_GrpCnt == 0)
	{
		DrawEmpty(pDC, false);
		m_cs.Unlock();
		return;
	}

	for (int ii = 0; ii < m_MinMaxCnt; ii++)
	{
		m_pMinMaxChk[ii].Max = DBL_MIN;
		m_pMinMaxChk[ii].Min = DBL_MAX;
		m_pMinMaxChk[ii].tickx = 0;
		m_pMinMaxChk[ii].ticky = 0;
	}

	int	gKind;
	double	dMin, dMax;
	struct _MinMaxChk	*pMinMaxChk;
	class CGrp_Base	*pBase;
	if (m_bSameRegion)
	{
		for (int ii = 0; ii < m_GrpCnt; ii++)
		{
			pBase = m_pGrpInfo[ii];
			pMinMaxChk = GetMinMaxChk(pBase->GetRgnKey());
			if (pMinMaxChk && pMinMaxChk->coMinMax)
			{
				if (pBase->CalculateMinMax())
				{
					gKind = pBase->GetGrpKind();
					pBase->GetMinMaxValue(dMin, dMax);
					if (gKind != CGK_VOL && gKind != CGK_VMA &&
						dMin != DBL_MAX && dMax != DBL_MIN)
					{
						if (pMinMaxChk->Max < dMax)
							pMinMaxChk->Max = dMax;
						if (pMinMaxChk->Min > dMin)
							pMinMaxChk->Min = dMin;
					}
				}
			}
		}
	}
	else
	{
		for (int ii = 0; ii < m_GrpCnt; ii++)
		{
			pBase = m_pGrpInfo[ii];
			pMinMaxChk = GetMinMaxChk(pBase->GetRgnKey());
			if (pMinMaxChk && pMinMaxChk->coMinMax)
			{
				if (pBase->CalculateMinMax())
				{
					pBase->GetMinMaxValue(dMin, dMax);
					if (dMin != DBL_MAX && dMax != DBL_MIN)
					{
						if (pMinMaxChk->Max < dMax)
							pMinMaxChk->Max = dMax;
						if (pMinMaxChk->Min > dMin)
							pMinMaxChk->Min = dMin;
					}
				}
			}
		}
	}

	if (m_bSameRegion)
	{
		for (int ii = m_GrpCnt - 1; ii >= 0; ii--)
			m_pGrpInfo[ii]->DrawGraph(pDC);
	}
	else
	{
		for (int ii = 0; ii < m_GrpCnt; ii++)
			m_pGrpInfo[ii]->DrawGraph(pDC);
	}
	DrawEmpty(pDC, false);
	m_pCrossLine->DrawCrossLine(pDC);

	m_cs.Unlock();
}

void CGrpWnd::DrawEmpty(CDC *pDC, bool bInit)
{
	if (bInit)
	{
		CBrush	*pBrush = m_pApp->GetBrush(m_pView, m_fRGB);
		CBrush	*tBrush =  m_pApp->GetBrush(m_pView, m_pRGB);
		CBrush	*fBrush =  m_pApp->GetBrush(m_pView, RGB(128, 128, 128));

		pDC->FillRect(&m_ObjRect, pBrush);
		pDC->FrameRect(&m_ObjRect, fBrush);
		pDC->FillRect(&m_GrpRect, tBrush);
	}
	else
	{
		CBrush	*fBrush =  m_pApp->GetBrush(m_pView, RGB(128,128,128));
		pDC->FrameRect(&m_GrpRect, fBrush);

		if (m_RgnCnt > 1)
		{
			CPen	*cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, RGB(128,128,128));
			CPen	*sPen = pDC->SelectObject(cPen);
			for (int ii = 1; ii < m_RgnCnt; ii++)
			{
				pDC->MoveTo(m_pRgnInfo[ii]->gpRect.left, m_pRgnInfo[ii]->gpRect.top);
				pDC->LineTo(m_pRgnInfo[ii]->gpRect.right, m_pRgnInfo[ii]->gpRect.top);
			}
			pDC->SelectObject(sPen);
		}
	}
}

void CGrpWnd::Resize()
{
	GetClientRect(&m_ObjRect);

	if (m_RgnCnt == 0 || m_GrpCnt == 0)
		return;

	CDC *pDC = GetDC();
	CFont *pOldFont = pDC->SelectObject(m_pFont);
	CSize	sz = pDC->GetOutputTextExtent("99,999,999");
	
	double	*pHeight = new double[m_RgnCnt];
	switch (m_RgnCnt)
	{
	case 1:
		pHeight[0] = 1.0;
		break;
	case 2:
		pHeight[0] = 0.7;
		pHeight[1] = 0.3;
		break;
	case 3:
		pHeight[0] = 0.4;
		pHeight[1] = 0.3;
		pHeight[2] = 0.3;
		break;
	}

	CRect	bottomRC = m_ObjRect;
	bottomRC.top = bottomRC.bottom - sz.cy - 5;
	m_GrpRect = m_ObjRect;
	m_GrpRect.top += 3;
	m_GrpRect.bottom = bottomRC.top;
	if ((m_TickView & CDT_LEFT) && (m_TickView & CDT_RIGHT))
	{
		m_GrpRect.left = m_ObjRect.left + sz.cx;
		m_GrpRect.right = m_ObjRect.right - sz.cx;
	}
	else if (m_TickView & CDT_LEFT)
	{
		m_GrpRect.left = m_ObjRect.left + sz.cx;
		m_GrpRect.right = m_ObjRect.right - 5;
	}
	else if (m_TickView & CDT_RIGHT)
	{
		m_GrpRect.left = m_ObjRect.left + 5;
		m_GrpRect.right = m_ObjRect.right - sz.cx;
	}
	else
	{
		m_GrpRect.left = m_ObjRect.left + 5;
		m_GrpRect.right = m_ObjRect.right - 5;
	}

	for (int ii = 0; ii < m_RgnCnt; ii++)
			m_pRgnInfo[ii]->tkHeight = sz.cy;

	bottomRC.left = m_GrpRect.left;
	bottomRC.right = m_GrpRect.right;

	int	top = m_GrpRect.top;
	for (int ii = 0; ii < m_RgnCnt; ii++)
	{
		m_pRgnInfo[ii]->tick[ctkLEFT].tkRect = 
			CRect(0, top, m_GrpRect.left, top + int(m_GrpRect.Height()*pHeight[ii]));

		m_pRgnInfo[ii]->tick[ctkRIGHT].tkRect = 
			CRect(m_GrpRect.right, top, 
			m_ObjRect.right, top + int(m_GrpRect.Height()*pHeight[ii]));

		m_pRgnInfo[ii]->tick[ctkBOTTOM].tkRect = bottomRC;
		m_pRgnInfo[ii]->gpRect.top = m_pRgnInfo[ii]->tick[ctkLEFT].tkRect.top;
		m_pRgnInfo[ii]->gpRect.bottom = m_pRgnInfo[ii]->tick[ctkLEFT].tkRect.bottom;
		m_pRgnInfo[ii]->gpRect.left = m_pRgnInfo[ii]->tick[ctkLEFT].tkRect.right;
		m_pRgnInfo[ii]->gpRect.right = m_pRgnInfo[ii]->tick[ctkRIGHT].tkRect.left;

		top = top + int(m_GrpRect.Height()*pHeight[ii]);
	}

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
	delete pHeight;

	for (int ii = 0; ii < m_GrpCnt; ii++)
		m_pGrpInfo[ii]->Resize();
	m_pCrossLine->Resize(m_GrpRect);
}

/*****/
// c : use control
// r : request count
// d : display count - 0:all
// t : draw tick 
// j : juga chart
// v : volume chart
// o : outline
// i : data index - 1:ƽ, 2:��, 3:��, 4:��, 5:��
// u : data unit - 1:����, 2:����, 3:����, 4:����, 5:ECN
// m : ma
// g : vm
// �ŷ�������|2|00001002|32|p
// �ѿ�����ǥ��|3|$bool|False|s
/*****/
void CGrpWnd::ParseParam(char *param)
{
	m_sParam.Format("%s", param);
	CString	sParam = m_sParam;

	CString	sTmp, sToken;
	int		nPos = 0;

	// ��� ����
	sToken = "/d";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + sToken.GetLength());
		m_dDay = atoi(sTmp);
	}

	// request data count
	sToken = "/r";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + sToken.GetLength());
		m_dCount = atoi(sTmp);
	}

	// tick
	sToken = "/t";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + sToken.GetLength());
		m_TickView = atoi(sTmp);
	}

	// ����Ʈ
	sToken = "/j";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + sToken.GetLength());
		int type = atoi(sTmp);
		switch (type)
		{
		case CGK_LINE:	m_jChart = CGK_LINE;	break;
		case CGK_BONG:	m_jChart = CGK_BONG;	break;
		case CGK_BAR:	m_jChart = CGK_BAR;	break;
		}
	}

	// �ŷ�����Ʈ
	sToken = "/v";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)	m_bVol = true;
	else		m_bVol = false;

	// out line
	sToken = "/t";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)	m_bOutLine = true;
	else		m_bOutLine = false;

	// �ŷ�������
	sToken = "/p";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + sToken.GetLength());
		m_VolColor = atoi(sTmp);
	}
	else
	{
		m_VolColor = -1;
	}

	// �ѿ�����ǥ��
	sToken = "/s";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)	m_bSameRegion = true;
	else		m_bSameRegion = false;

	// data index
	sToken = "/i";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + 2);
		int index = atoi(sTmp);
		switch (index)
		{
		case CDI_TICK:	m_dIndex = CDI_TICK;	break;
		case CDI_MIN:	m_dIndex = CDI_MIN;	break;
		case CDI_DAY:	m_dIndex = CDI_DAY;	break;
		case CDI_WEEK:	m_dIndex = CDI_WEEK;	break;
		case CDI_MON:	m_dIndex = CDI_MON;	break;
		}
	}
	
	// data unit
	sToken = "/u";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + 2);
		int unit = atoi(sTmp);

		switch (unit)
		{
		case CDU_JONGMOK:	m_dUnit = CDU_JONGMOK;	break;
		case CDU_UPJONG:	m_dUnit = CDU_UPJONG;	break;
		case CDU_FUTURE:	m_dUnit = CDU_FUTURE;	break;
		case CDU_OPTION:	m_dUnit = CDU_OPTION;	break;
		case CDU_ECN:		m_dUnit = CDU_ECN;	break;
		default:		m_dUnit = 0;		break;
		}
	}

	if (!m_bSameRegion)
	{
		for (int ii = 0; ii < 4; ii++)
		{
			sToken.Format("/m%d", ii+1);
			nPos = sParam.Find(sToken, 0);
			if (nPos < 0)	
				continue;

			sTmp= sParam.Mid(nPos + 3);
			int mVal = atoi(sTmp);

			if (mVal > 0)
				m_PMA[m_PMACnt++] = mVal;

			sToken.Format("/g%d", ii+1);
			nPos = sParam.Find(sToken, 0);
			if (nPos < 0)	
				continue;

			sTmp= sParam.Mid(nPos + 3);
			mVal = atoi(sTmp);

			if (mVal > 0)
				m_VMA[m_VMACnt++] = mVal;
		}
	}
}

int CGrpWnd::RequestHead()
{
	m_sHeader.Format("%02d%02d%02d%06d0000000000000000", m_dUnit, m_dIndex, m_dKind, m_dCount);

	return long(m_sHeader.operator LPCTSTR());
}

void CGrpWnd::RemoveComponent()
{
	m_cs.Lock();

	if (m_RgnCnt)
	{
		for (int ii = 0; ii < m_RgnCnt; ii++)
		{
			if (m_pRgnInfo[ii])
			{
				delete m_pRgnInfo[ii];
				m_pRgnInfo[ii] = NULL;
			}
		}
	}

	if (m_DataCnt)
	{
		for (int ii = 0; ii < m_DataCnt; ii++)
		{
			if (m_pDataInfo[ii])
			{
				delete m_pDataInfo[ii];
				m_pDataInfo[ii] = NULL;
			}
		}
	}

	if (m_GrpCnt)
	{
		for (int ii = 0; ii < m_GrpCnt; ii++)
		{
			if (m_pGrpInfo[ii])
			{
				delete m_pGrpInfo[ii];
				m_pGrpInfo[ii] = NULL;
			}
		}
	}
	m_RgnCnt = 0;
	m_GrpCnt = 0;
	m_DataCnt = 0;

	if (m_pMinMaxChk)
	{
		delete m_pMinMaxChk;
		m_pMinMaxChk = NULL;
	}
	m_MinMaxCnt = 0;

	if (m_pCrossLine)
	{
		delete m_pCrossLine;
		m_pCrossLine = NULL;
	}

	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_Enable, FALSE), 0);

	m_cs.Unlock();
}

int CGrpWnd::GetPalette(int index)
{
	return m_pView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

BOOL CGrpWnd::IsPtInGrp()
{
	POINT	pt;
	CRect	rect;

	::GetCursorPos(&pt);
	GetWindowRect(rect);

	CWnd* pWnd = CWnd::WindowFromPoint(pt);
	if (pWnd != this)
		return FALSE;

	return (rect.PtInRect(pt));
}

bool CGrpWnd::ChangeDisplayScale(int scale)
{
	scale *=10;
	if (scale > 0)	// dispDay ����, scale ���
	{
		if (m_dispDay >= m_totalDay)
			return false;

		if (m_dispPos == 0)
		{
			scale = (m_totalDay - m_dispDay >= scale)?
						scale:m_totalDay - m_dispDay;
			m_dispDay += scale;
			m_dispEnd += scale;

			return true;
		}
		else if (m_dispPos - abs(scale) < 0)
		{
			scale = m_dispPos;
		}
	}
	else if (scale < 0)	// dispDay ����, scale Ȯ��
	{
		if (m_dispDay <= 20)
			return false;
		if ((m_dispDay - abs(scale)) <= 20)
			scale = 20 - m_dispDay;

		if (scale == 0)
			return false;
	}
	else
		return false;

	m_dispDay += scale;
	m_dispPos -= scale;

	return true;
}

bool CGrpWnd::ChangeDisplay(int dispDay)
{
	m_dispDay = dispDay;
	m_dispPos = m_dispEnd - dispDay;

	if (m_dispPos < 0)
	{
		int gap = 0 - m_dispPos;
		m_dispPos += gap;
		m_dispEnd += gap;
	}

	return true;
}

bool CGrpWnd::ChangeDisplayShift(int shift)
{
	if (shift > 0)	// right scroll
	{
		if (m_dispEnd >= m_totalDay)
			return false;
		if ((m_dispEnd + shift) > m_totalDay)
			shift = m_totalDay - m_dispEnd;
	}
	else if (shift < 0)	// left scroll
	{
		if (m_dispPos <= 0)
			return false;
		if ((m_dispPos - abs(shift)) <  0)
			shift = m_dispPos * (-1);
	}

	if (shift == 0)
		return false;

	m_dispEnd += shift;
	m_dispPos += shift;

	RecalculateMinMax();
	Invalidate();

	return true;
}

void CGrpWnd::ExcelProc()
{
	if (m_GrpCnt <= 0 || !m_pGrpInfo[0] || m_totalDay <= 0)
		return;

	CString	tmpS;
	CString	ExcelS = m_pGrpInfo[0]->GetExcelHead(-1);
	for (int ii = 0; ii < m_GrpCnt; ii++)
	{
		tmpS = m_pGrpInfo[ii]->GetExcelData(-1);
		if (!tmpS.IsEmpty())
			ExcelS += "\t" + tmpS;
	}
	ExcelS += "\n";

	for (int ii = 0; ii < m_totalDay; ii++)
	{
		tmpS = m_pGrpInfo[0]->GetExcelHead(ii);
		ExcelS += tmpS;
		for (int jj = 0; jj < m_GrpCnt; jj++)
		{
			tmpS = m_pGrpInfo[jj]->GetExcelData(ii);
			if (!tmpS.IsEmpty())
				ExcelS += "\t" + tmpS;
		}
		ExcelS += "\n";
	}

	if (ExcelS.IsEmpty())
		return;

	m_pView->SendMessage(WM_USER, MAKEWPARAM(excelDLL, 1), (long)ExcelS.operator LPCTSTR());
}

void CGrpWnd::RecalculateMinMax()
{
	for (int ii = 0; ii < m_GrpCnt; ii++)
		m_pGrpInfo[ii]->CalculateMinMax();
}

void CGrpWnd::ReviseTick()
{
	if (m_pMinMaxChk)
	{
		delete[] m_pMinMaxChk;
		m_pMinMaxChk = NULL;
	}

	m_MinMaxCnt = m_RgnCnt;
	m_pMinMaxChk = new _MinMaxChk[m_MinMaxCnt];
	ZeroMemory(m_pMinMaxChk, sizeof(_MinMaxChk) * m_MinMaxCnt);

	for (int ii = 0; ii < m_RgnCnt; ii++)
	{
		int	grpcnt = 0;
		for (int jj = 0; jj < m_GrpCnt; jj++)
		{			
			if (m_pGrpInfo[jj]->GetRgnKey() == ii)
				grpcnt++;
		}

		m_pMinMaxChk[ii].region = ii;
		if (grpcnt > 1)	m_pMinMaxChk[ii].coMinMax = 1;
	}
}

_MinMaxChk* CGrpWnd::GetMinMaxChk(int rKey)
{
	if (rKey < 0 || rKey >= m_RgnCnt)
		return NULL;
	
	return &m_pMinMaxChk[rKey];
}

void CGrpWnd::TipMouseMove(CPoint point)
{
	ScreenToClient(&point);

	MouseMove(true, point);
}

void CGrpWnd::MouseMove(bool bIsTipEvent, CPoint point, bool bCrossOnly)
{
	if (!m_pToolTip || !m_pCrossLine)
		return;

	if (!m_GrpRect.PtInRect(point))
	{
		m_pToolTip->ShowWindow(SW_HIDE);
		m_pCrossLine->HideCrossLine(point);
		return;
	}

	if (m_GrpCnt < 1)
	{
		m_pCrossLine->HideCrossLine(point);
		m_pToolTip->ShowWindow(SW_HIDE);
		return;
	}

	m_pCrossLine->ShowCrossLine(point);

	if (bCrossOnly)
	{
		if (m_timerID > 0)
			KillTimer(m_timerID);

		m_timerID = TIMER_ID;
		SetTimer(m_timerID, TIMER_GAP, NULL);
		return;
	}

	if (m_PosData.IsEmpty())
	{
		m_pToolTip->ShowWindow(SW_HIDE);
		return;
	}

	if (!bIsTipEvent)
	{
		if (m_pToolTip->IsWindowVisible())
			return;
	}

	CPoint	pt = point;
	ClientToScreen(&pt);
	pt.x += 10;
	pt.y += 10;

	CRect	rect;
	CSize	size;	

	rect = m_pToolTip->SetData(m_PosData);

	size.cx = rect.Width();
	size.cy = rect.Height();

	rect.left = pt.x;
	rect.right = pt.x + size.cx;
	rect.top = pt.y;
	rect.bottom = rect.top + size.cy;

	CRect rcWnd;
	GetWindowRect(rcWnd);

	m_pToolTip->SetWindowPos(&wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE);
	
	if (m_pToolTip->IsWindowVisible())
		m_pToolTip->Invalidate(FALSE);
	else
		m_pToolTip->ShowWindow(SW_SHOWNOACTIVATE);

	if (m_timerID > 0)
		KillTimer(m_timerID);

	m_timerID = TIMER_ID;
	SetTimer(m_timerID, TIMER_GAP, NULL);
}

CString CGrpWnd::GetTipStr(CPoint point)
{
	if (m_GrpCnt < 1)
		return _T("");

	int	rgnIdx = -1;
	int ii = 0;
	for (ii = 0; ii < m_RgnCnt; ii++)
	{
		if (m_pRgnInfo[ii]->gpRect.PtInRect(point))
		{
			rgnIdx = ii;
			break;
		}
	}

	CString stmp;
	CString dispStr = m_pGrpInfo[0]->GetDisplayPosHead(point);
	for (; ii < m_GrpCnt; ii++)
	{
//		if (m_pGrpInfo[ii]->GetRgnKey() == rgnIdx)
//		{
			stmp = m_pGrpInfo[ii]->GetDisplayPosData(point);
			if (!stmp.IsEmpty())
				dispStr = dispStr + "\n" + stmp;
//		}
	}

	return dispStr;
}

void CGrpWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_pParent->SendMessage(WM_RBUTTONDOWN, WPARAM(nFlags), MAKELPARAM(point.x, point.y));	
	CWnd::OnRButtonDown(nFlags, point);
}

void CGrpWnd::StartOrderSignal(CString sPrice)
{
	CString	sTmp;
	sPrice.Remove(',');
	sTmp.Format("%.f", m_power * atof(sPrice));
	m_pivot = atoi(sTmp);
	Invalidate();
}

void CGrpWnd::EndOrderSignal()
{
	m_pivot = -100;
	Invalidate();
}

BOOL CGrpWnd::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_MOUSEWHEEL)
	{
		TRACE("WHEEL!!!!!!!!!!!!!\n");
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CGrpWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_Focus, TRUE), 0);	
	CWnd::OnLButtonDown(nFlags, point);
}
