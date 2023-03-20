// GraphDrawerImplementation.h: interface for the CGraphDrawerImplementation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHDRAWERIMPLEMENTATION_H__1DC145CE_2791_4412_A204_202283D5C472__INCLUDED_)
#define AFX_GRAPHDRAWERIMPLEMENTATION_H__1DC145CE_2791_4412_A204_202283D5C472__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "BlockBaseData.h"

class CSubGraphData;
class CDrawingGraphData;
class CSelectSubGraphData;
class CDisplayAttributes;
class CEtcGraphColor;
class CPacket;
class CIndicatorList;
class CSubGraphPacketList;
class CTextInRightScaleRegionDrawer;
class CXScaleManager;
class CSignalData;

class CGraphDrawerImplementation  
{
public:
	// (2009/1/7 - Seung-Won, Bae) Support graph title drawing routine.
	bool DrawGraphTitle( CDC* pDC, const CDrawingGraphData& drawingGraphData, 
		const CSubGraphData& subGraphData, int& nTitleEndPosition, int& nTitleEndLine);
	// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
	bool CGraphDrawerImplementation::DrawGraph(CDC* pDC, const CDrawingGraphData& drawingGraphData,
				const CSubGraphData& subGraphData, const CString& strRQ, CXScaleManager* pxScaleManager);

	// xScaleManager/�������� ���� - xScaleManager - ojtaso (20070404)
	void DrawSignal(CDC* pDC, const CDrawingGraphData& drawingGraphData, const CList<double, double>* pSignalData, CSignalData &p_signalData, const CString& strRQ, CXScaleManager* pxScaleManager);

	bool DrawGrip(CDC* pDC, const int index, const CSelectSubGraphData& selectSubGraphData);

	int PointInSubGraph(const CPoint& point, const CSelectSubGraphData& selectSubGraphData, CIndicatorList* pIndicatorList);

// vntsorl_20110609:[A00000259] ���ݴ��� ���� ���簡 ǥ�� ����
	bool GetTextInRightScaleRegionDrawer(const CTextInRightScaleRegionDrawer& textData, CString& strCurPrice, CString& strDiffPrice, CString& strPipPrice, CString& strRiseFallRate) const;

private:
	bool IsAction(CSubGraphPacketList* m_pSubGraphPacketList) const;

	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	void DrawGraphDatas(CDC* pDC, const CRect& realDrawingRegion, const CDisplayAttributes& realDisplayAttributes, 
		const CDrawingGraphData& drawingGraphData, const CSubGraphData& subGraphData);

	int GetDisplayDataEnd(const CGraphBaseData::GRAPHTYPE& eGraphType, 
		const int nOrgEnd, const CList<CPacket*, CPacket*>& PacketList) const;
	bool GetDisplayAttributes(const CSubGraphData& subGraphData, const int nDataEnd, CDisplayAttributes& displayAttributes) const;
};

#endif // !defined(AFX_GRAPHDRAWERIMPLEMENTATION_H__1DC145CE_2791_4412_A204_202283D5C472__INCLUDED_)
