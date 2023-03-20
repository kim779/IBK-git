// GraphDrawerImplementation.cpp: implementation of the CGraphDrawerImplementation class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphDrawerImplementation.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "SelectSubGraph.h"
#include "DrawSingleton.h"
#include "GraphTitleDrawer.h"
#include "GraphDataDrawer.h"
#include "SignalDraw.h"
#include "GripDraw.h"
#include "PacketPointerList.h"
#include "PacketBase.h"
#include "XScaleManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
bool CGraphDrawerImplementation::DrawGraph(CDC* pDC, const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData, const CString& strRQ, CXScaleManager* pxScaleManager)
{

	if(drawingGraphData.GetSubGraphPacketList() == NULL)
		return false;

	CDraw* pDraw = CDrawSingleton::Find( subGraphData.GetType(), subGraphData.GetStyle(), drawingGraphData);
	if(pDraw == NULL) 
		return false;

	// (2006/10/11 - Seung-Won, Bae) Use Clipping
	CRect rctNoFrameRegion = drawingGraphData.GetGraphRegions().GetNoFrameRegion();

	// X-Scale ���� ���� ��� (yiilyoul@magicn.com - 20070125)
//	pxScaleManager->GetGraphXArea(rctNoFrameRegion.left, rctNoFrameRegion.right);

	CRgn rgn;
	rgn.CreateRectRgn( rctNoFrameRegion.left, rctNoFrameRegion.top, rctNoFrameRegion.right, rctNoFrameRegion.bottom);
	pDC->SelectClipRgn( &rgn);

	// graph �׸���
	const CList< CPacket *, CPacket *> &PacketList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	CDisplayAttributes realDisplayAttributes = drawingGraphData.GetDisplayAttributes();
	int dataRealEnd = GetDisplayDataEnd(subGraphData.GetType(), realDisplayAttributes.GetDataEndInBlock(), PacketList);
	if( 0 <= realDisplayAttributes.GetDataStartInBlock() && 0 <= dataRealEnd) 
	{
		// graph style �� ���� displyaAttribute �� ����ȴ�.
		GetDisplayAttributes(subGraphData, dataRealEnd, realDisplayAttributes);

		CRect realDrawingRegion = drawingGraphData.GetGraphRegions().GetDrawingRegion( subGraphData.GetType(), subGraphData.GetStyle());
		BOOL bResult2 =pDraw->DrawGraph(pDC, realDrawingRegion, realDisplayAttributes, drawingGraphData, subGraphData, strRQ, pxScaleManager);
		if(bResult2 == false)
			bResult2 = pDraw->DrawGraph(pDC, realDrawingRegion, realDisplayAttributes, drawingGraphData, subGraphData);
 
		pDC->SelectClipRgn(NULL);
		if( !bResult2) return true;

		// (2009/2/19 - Seung-Won, Bae) Do not draw on caclulation-thread.
		if( pxScaleManager->GetThreadFlag() == THREAD_DRAWING)
			DrawGraphDatas(pDC, realDrawingRegion, realDisplayAttributes, drawingGraphData, subGraphData);
	}

	return true;
}

// xScaleManager/�������� ���� - xScaleManager - ojtaso (20070404)
void CGraphDrawerImplementation::DrawSignal(CDC* pDC, const CDrawingGraphData& drawingGraphData, const CList<double, double>* pSignalData, CSignalData &p_signalData, const CString& strRQ, CXScaleManager* pxScaleManager)
{
	if(pSignalData == NULL || pSignalData->GetCount() <= 0)
		return;

	if(!IsAction(drawingGraphData.GetSubGraphPacketList()))
		return;

	CRect rctNoFrameRegion = drawingGraphData.GetGraphRegions().GetNoFrameRegion();
	CRgn rgn;
	rgn.CreateRectRgn( rctNoFrameRegion.left, rctNoFrameRegion.top, rctNoFrameRegion.right, rctNoFrameRegion.bottom);
	pDC->SelectClipRgn( &rgn);

	CDisplayAttributes displayAttributes = drawingGraphData.GetDisplayAttributes();
	CPacket* pPacket = drawingGraphData.GetSubGraphPacketList()->GetHeadPacket();
	CSignalDraw().DrawSignal(pDC, drawingGraphData, displayAttributes, pPacket, pSignalData, p_signalData, strRQ, pxScaleManager, drawingGraphData.GetColumnIndex());

	pDC->SelectClipRgn( NULL);
}

bool CGraphDrawerImplementation::DrawGrip(CDC* pDC, const int index, const CSelectSubGraphData& selectSubGraphData)
{
	return CGripDraw().DrawGrip(pDC, index, selectSubGraphData);
}

// ----------------------------------------------------------------------------
int CGraphDrawerImplementation::PointInSubGraph(const CPoint& point, const CSelectSubGraphData& selectSubGraphData, CIndicatorList* pIndicatorList)
{
	CSelectSubGraph selectSubGraph;
	return selectSubGraph.PointInSubGraph(point, &selectSubGraphData, pIndicatorList);
}

// vntsorl_20110609:[A00000259] ���ݴ��� ���� ���簡 ǥ�� ����
bool CGraphDrawerImplementation::GetTextInRightScaleRegionDrawer(const CTextInRightScaleRegionDrawer& textData, CString& strCurPrice, CString& strDiffPrice, CString& strPipPrice, CString& strRiseFallRate) const
{
	return CDataInRightScaleRegionDrawer().GetText(textData, strCurPrice, strDiffPrice, strPipPrice, strRiseFallRate);
}


// private ====================================================================
bool CGraphDrawerImplementation::IsAction(CSubGraphPacketList* m_pSubGraphPacketList) const
{
	return (m_pSubGraphPacketList != NULL && m_pSubGraphPacketList->GetCount() > 0);
}

// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
bool CGraphDrawerImplementation::DrawGraphTitle(CDC* pDC, const CDrawingGraphData& drawingGraphData, 
		const CSubGraphData& subGraphData, int& nTitleEndPosition, int& nTitleEndLine)
{
	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	return CGraphTitleDrawer().DrawGraphTitle(pDC, drawingGraphData, subGraphData, nTitleEndPosition, nTitleEndLine);
}

void CGraphDrawerImplementation::DrawGraphDatas(CDC* pDC, const CRect& realDrawingRegion, const CDisplayAttributes& realDisplayAttributes, 
		const CDrawingGraphData& drawingGraphData, const CSubGraphData& subGraphData)
{
	CGraphDataDrawer().DrawGraphDatas(pDC, realDrawingRegion, realDisplayAttributes, drawingGraphData, subGraphData);
}

// ----------------------------------------------------------------------------
int CGraphDrawerImplementation::GetDisplayDataEnd(const CGraphBaseData::GRAPHTYPE& eGraphType, 
		const int nOrgEnd, const CList<CPacket*, CPacket*>& PacketList) const
{
	if(eGraphType == CGraphBaseData::Special_Type || eGraphType == CGraphBaseData::FuOp_Type)
		return nOrgEnd;

	int nMaxDataCount = 0;
	POSITION pos = PacketList.GetHeadPosition();
	while(pos != NULL){
		CPacket* pPacket = PacketList.GetNext(pos);
		if(pPacket == NULL)
			return -1;

		int nDataCount = 0;
		if(pPacket->GetType() == _MTEXT( C6_CHARACTER))
			nDataCount = pPacket->GetstrDataList()->GetCount();
		else
			nDataCount = pPacket->GetnumericDataList()->GetCount();

		// "���Ź�" �� ���
// (2006.12.06) Seunghwan Ahn
		if( g_iMetaTable.IsVolumeForSale(pPacket->GetName()) && nDataCount >= 2)
// (2006.12.06) Seunghwan Ahn
//		if(pPacket->GetName() == _MTEXT( C2_OVERHANGING_SUPPLY) && nDataCount >= 2)
			return (nDataCount - 2);

		if(nDataCount > nMaxDataCount)
			nMaxDataCount = nDataCount;
	}

	int nDataEndIndex = nMaxDataCount -1;
	return (nDataEndIndex < nOrgEnd ? nDataEndIndex : nOrgEnd);
}

// graph style �� ���� displyaAttribute �� ����ȴ�.
bool CGraphDrawerImplementation::GetDisplayAttributes(const CSubGraphData& subGraphData, const int nDataEnd, CDisplayAttributes& displayAttributes) const
{
	if(subGraphData.GetType() != CGraphBaseData::Bar_Type)
		return false;

	// <Bar_Type>�� <FixedRateCombBar : ��������������> �� ��츸 RealEnd ������ �����Ѵ�.
	if(subGraphData.GetStyle().GetGraphBarStyle() != CGraphBaseData::FixedRateCombBar)
		return false;

	displayAttributes.SetDataEndInBlock(nDataEnd);
	return true;
}

// -----------------------------------------------------------------------------------------
