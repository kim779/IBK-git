// GraphDrawer.cpp: implementation of the CGraphDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphDrawer.h"

#include "GraphDrawerImplementation.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// (2009/1/7 - Seung-Won, Bae) Support graph title drawing routine.
bool CGraphDrawer::DrawGraphTitle(CDC* pDC,	// CDC
			const CDrawingGraphData& drawingGraphData,
            const CSubGraphData& subGraphData,  // subgraph data
			int& titlePosition,
			int& titleLine)				// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
{
	CGraphDrawerImplementation graphDrawerImplementation;
	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	return graphDrawerImplementation.DrawGraphTitle( pDC, drawingGraphData, subGraphData, titlePosition, titleLine);
}

// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
bool CGraphDrawer::DrawGraph(CDC* pDC,	// CDC
			const CDrawingGraphData& drawingGraphData,
            const CSubGraphData& subGraphData,  // subgraph data
			const CString& strRQ,
			CXScaleManager* pxScaleManager) 
{
	CGraphDrawerImplementation graphDrawerImplementation;
	return graphDrawerImplementation.DrawGraph(pDC, drawingGraphData, subGraphData, strRQ, pxScaleManager);
}
// xScaleManager/�������� ���� - xScaleManager - ojtaso (20070404)
void CGraphDrawer::DrawSignal(CDC* pDC, 
			const CDrawingGraphData& drawingGraphData,
			const CList<double, double>* pSignalData,
			CSignalData &p_signalData,
			const CString& strRQ,
			CXScaleManager* pxScaleManager)
{
	CGraphDrawerImplementation graphDrawerImplementation;
	graphDrawerImplementation.DrawSignal(pDC, drawingGraphData, pSignalData, p_signalData, strRQ, pxScaleManager);
}

bool CGraphDrawer::DrawGrip(CDC* pDC, const int index, const CSelectSubGraphData& selectSubGraphData)
{
	CGraphDrawerImplementation graphDrawerImplementation;
	return graphDrawerImplementation.DrawGrip(pDC, index, selectSubGraphData);
}

int CGraphDrawer::PointInSubGraph(const CPoint& point, const CSelectSubGraphData& selectSubGraphData, CIndicatorList* pIndicatorList)
{
	CGraphDrawerImplementation graphDrawerImplementation;
	return graphDrawerImplementation.PointInSubGraph(point, selectSubGraphData, pIndicatorList);
}

// vntsorl_20110609:[A00000259] ���ݴ��� ���� ���簡 ǥ�� ����
bool CGraphDrawer::GetTextInRightScaleRegionDrawer(const CTextInRightScaleRegionDrawer& textData, CString& strCurPrice, CString& strDiffPrice, CString& strPipPrice, CString& strRiseFallRate) const
{
	CGraphDrawerImplementation graphDrawerImplementation;
	return graphDrawerImplementation.GetTextInRightScaleRegionDrawer(textData, strCurPrice, strDiffPrice, strPipPrice, strRiseFallRate);
}


