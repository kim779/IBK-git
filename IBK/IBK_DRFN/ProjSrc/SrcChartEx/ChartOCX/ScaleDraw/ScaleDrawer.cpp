// ScaleDrawer.cpp: implementation of the CScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScaleDrawer.h"

#include "ScaleDrawerImplementation.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CScaleDrawer

CScaleDrawer::CScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode) :
	m_pScaleDrawerImpl(NULL)
{
	m_pScaleDrawerImpl = new CScaleDrawerImplementation( p_hOcxWnd, p_eChartMode);
}

CScaleDrawer::~CScaleDrawer()
{
	delete m_pScaleDrawerImpl;
}

// public ===========================================================================
// scale �׸���.
void CScaleDrawer::DrawHorzTextOrGrid(CDC* pDC, 
				const CDrawingHorzScaleData& drawingHorzScaleData,
				const CHorzScaleRegion& horzScaleRegion, // ���� scale�� ����
				const CHorizontalScale& horzScale,		 // ���� scale�� ���� ��
				CXScaleManager* pxScaleManager,
				const char *p_szRQ)
{
	m_pScaleDrawerImpl->DrawHorzTextOrGrid( pDC, drawingHorzScaleData, horzScaleRegion, horzScale, pxScaleManager, p_szRQ);
}

void CScaleDrawer::DrawVertTextOrGrid(CDC* pDC, 
				const CDrawingVertScaleData& drawingVertScaleData,
				const CVertScaleRegion& vertScaleRegion, // ���� scale�� ����
				const CVerticalScale& vertScale)		 // ���� scale�� ���� ��
{
	m_pScaleDrawerImpl->DrawVertTextOrGrid(pDC, drawingVertScaleData, vertScaleRegion, vertScale);
}
