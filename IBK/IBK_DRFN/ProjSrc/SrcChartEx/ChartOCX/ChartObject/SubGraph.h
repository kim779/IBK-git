// SubGraph.h: interface for the CSubGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUBGRAPH_H__8DD94F29_A8A7_407D_AA56_4A4E1D3D4B62__INCLUDED_)
#define AFX_SUBGRAPH_H__8DD94F29_A8A7_407D_AA56_4A4E1D3D4B62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/GraphData.h"
#include "../Include_Chart/PacketPointerList.h"
#include "../Include_Chart/ScaleBaseData.h"			// for CDisplayDataMinMax

class CMainBlockBaseData;
class CBlockImp;
class CVertScaleGroupList;
class CVertScaleGroup;
class CGraphImp;
class CSubGraphList;
class CSubGraphViewDataList;
class CSelectSubGraphData;
class CSubGraphViewData;
class CDrawingGraphData;
class CVertScaleMaxWidthCalculatorData;
class CVertDisplayDataMinMaxInputData;

class CSubGraph : public CObject
{
public:
	// graph�� ����& ����
	static CSubGraph* MakeSubGraph(CMainBlockBaseData* pMainBlockBaseData, CSubGraphList* pSubGraphList, const CSubGraphData& subGraphData);
	// Singbar Graph ���� �񱳱��� �߰� - ojtaso (20071121)
	// ���ؼ� �β�, ��� ���� - ojtaso (20071213)
	static CSubGraph* MakeSubGraph(CMainBlockBaseData* pMainBlockBaseData, CSubGraphList* pSubGraphList, const CGraphBaseData::GRAPHTYPE type, const CGraphStyle& style, const CGraphDrawStyle& drawStyle, const CString& name, const CString& title, const bool hiding, const CGraphColor& color, const CPenThickness& penThickness, const CGraphPaintStyle& paintStyle, const CGraphBaseLineStyle& baseLineStyle);
	static void Delete(CSubGraph* pSubGraph);

public:
	CSubGraph(CMainBlockBaseData* pMainBlockBaseData, CSubGraphList* pSubGraphList, const CSubGraphData& subGraphData);
	// Singbar Graph ���� �񱳱��� �߰� - ojtaso (20071121)
	CSubGraph(CMainBlockBaseData* pMainBlockBaseData, CSubGraphList* pSubGraphList, const CGraphBaseData::GRAPHTYPE type, const CGraphStyle& style, const CGraphDrawStyle& drawStyle, const CString& name, const CString& title, const bool hiding, const CGraphColor& color, const CPenThickness& penThickness, const CGraphPaintStyle& paintStyle, const CGraphBaseLineStyle& baseLineStyle);

	void Delete();
	void DeleteAllPackets(const bool bIsOnlyNoTRData);
	void ClearAllPackets(const bool bIsOnlyNoTRData);

	void SetSubGraphList(CSubGraphList* pSubGraphList);
	void SetSubGraphData(const CSubGraphData& sourceSubGraphData, const bool bIsAllChange);
	bool SetHiding( const bool bIsHiding);
	// (2007/2/28 - Seung-Won, Bae) Support Graph Hiding
	BOOL	GetHiding( void) const;

	// �θ� �ּ�
	void SetMainBlockBaseData(CMainBlockBaseData* pMainBlockBaseData);
	CGraphImp* GetGraph() const;
	CSubGraphList* GetSubGraphList() const;

	// subGraphs���� data
	const CSubGraphData& GetSubGraphData() const;
	CSubGraphData& GetSubGraphData();
	CSubGraphData* GetSubGraphData_PointerType();
	CSubGraphPacketList& GetSubGraphPacketList();
	void GetSubGraphStringData( CString &p_strSubGraphData) const;
	// subGraph title region
	CRect GetSubGraphTitleRegion(CDC* pDC, const CRect& allTitleRegion, int& preTitleEndPosition) const;
	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	CRect GetSubGraphTitleRegion(CDC* pDC, const CRect& allTitleRegion, int& preTitleEndPosition, int& nPreLineCount) const;
	// view data setting
	void GetSubGraphViewDataList(const CString& GraphName, const CString& packetName, CSubGraphViewDataList& subGraphViewDataList) const;
	// packet�� ���õ� ����
	bool GetHeadPacketAndDataType(CPacket*& pPacket, double& dDataType) const;
	bool GetSubGraphPacketList(CList<CPacket*, CPacket*>& packetList) const;

	// point�� subGraph�� ���ϴ����� ����.
	int PointInSubGraph(const CPoint& point) const;

	// ���� packet�� �����ϴ��� Ȯ���Ѵ�.
	bool IsExistenceSamePacketInGraph(CGraphImp* pTargetGraph) const;

	// ���� scale width ���ϱ�.
	int GetVertScaleRegionWidth(CDC* pDC, CVertScaleMaxWidthCalculatorData* pCalData);
	// ���� ���̴� ȭ�鿡���� min, max�� ���Ѵ�. ==> y���� ��
	bool GetDisplayDataMinMax(CVertDisplayDataMinMaxInputData* pInputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);

	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	// (2009/1/7 - Seung-Won, Bae) Support graph title drawing routine.
	bool DrawSubGraphTitle(CDC* pDC, CDrawingGraphData* pDrawingGraphData, int& graphTitlePosition, int& graphTitleLine, const int& nSubGraphIndex);
	// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
	bool DrawSubGraph(CDC* pDC, CDrawingGraphData* pDrawingGraphData);
	void DrawGripedSubGraphTitle(CDC* pDC);
	bool DrawGrip(CDC* pDC, const int nDataIndex);

	void ResetMaxVerticalScaleWidth();

private:
	CMainBlockBaseData* m_pMainBlockBaseData;
	CSubGraphList* m_pSubGraphList;
	CSubGraphData m_subGraphData;
	CSubGraphPacketList m_subGraphPacketList;

	// ��ǥ�� ����Ѵ�.
	void CalculateGraph();

	// min/max �������� ����
	bool IsDisplayDataMinMax() const;
	// view data list�� �߰����� ���� --> ������ �̹� ���� ��쿣 �߰� ����
	bool IsAddTail_SubGraphViewData(const CString& packetName, CSubGraphViewDataList& subGraphViewDataList) const;
	bool IsPointInSubGraph(const CPoint& point) const;

	// subGraph �� select�� ���� data ��������.
	bool GetSelectSubGraphData(CSelectSubGraphData& selectSubGraphData) const;
	// vertScaleGroup���� ������ data
	bool GetSelectSubGraphData_InVertScaleGroupData(CSelectSubGraphData& selectSubGraphData) const;
	// block���� ������ data
	bool GetSelectSubGraphData_InBlock(CSelectSubGraphData& selectSubGraphData) const;
	// ��Ÿ data - DrawAllEnvironment���� ������.
	bool GetSelectSubGraphData_Etc(CSelectSubGraphData& selectSubGraphData) const;

	// subGraphViewData ��������.
	CSubGraphViewData GetSubGraphViewData(const CString& GraphName, const CString& packetName) const;

	// �θ� ���� data ��������.
	CBlockImp* GetBlock() const;
	CVertScaleGroupList* GetVertScaleGroupList() const;
	CVertScaleGroup* GetVertScaleGroup(CGraphImp* pGraph) const;
	
	void SendMessage_SubGraph(const bool bIsMaking);

// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
//		It is initialized in CSubGraph::GetDisplayDataMinMax() for CMainBlockImp::CalculateDataForDrawing().
protected:
	CDisplayDataMinMax	m_displayDataMinMax;

	int		m_nVerticalScaleMaxWidth;
};

#endif // !defined(AFX_SUBGRAPH_H__8DD94F29_A8A7_407D_AA56_4A4E1D3D4B62__INCLUDED_)
