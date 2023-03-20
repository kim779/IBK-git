// SubGraphList.h: interface for the CSubGraphList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUBGRAPHLIST_H__2255E3BA_4922_4512_9B33_39A02570ABCA__INCLUDED_)
#define AFX_SUBGRAPHLIST_H__2255E3BA_4922_4512_9B33_39A02570ABCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "../Include_Chart/BlockBaseData.h"

class CGraphImp;
class CPacket;
class CSubGraph;
class CGraphColor;
class CPenThickness;
class CSubGraphData;
class CGraphSearchInfo;
class CDrawingGraphData;
class CDisplayDataMinMax;
class CMainBlockBaseData;
class CGraphCalculateData;
class CSubGraphPacketList;
class CSubGraphViewDataList;
class CSelectedSubGraphDeque;
class CVertScaleMaxWidthCalculatorData;
class CVertDisplayDataMinMaxInputData;
class CSubGraphList  
{
public:
	CSubGraphList();
	virtual ~CSubGraphList();

// Search
public:
	// Graph
	CSubGraph *	GetSubGraph( int p_nIndex);
	BOOL		FindGraph( CGraphSearchInfo *p_pGraphSearchInfo) const;








	// subGraph�� ����&����
	void MakeSubGraphs( char *p_szSubGraphsData, const char *p_szIndicatorName);
	// Ÿ��Ʋ�� ����� ǥ�� : �������� - ojtaso (20070322)
	void MakeSubGraphs( char *p_szSubGraphsData, const char *p_szIndicatorName, LPCTSTR lpszItemName);
	void Delete(CSubGraph* pSubGraph);
	void DeleteAll();
	void DeleteAllPackets(const bool bIsOnlyNoTRData = true);
	void ClearAllPackets(const bool bIsOnlyNoTRData = true);
	// subGraphPacketList �ʱ�ȭ
	void Initialize_AllPackets();

	// �θ� �ּ�
	void SetMainBlockBaseData(CMainBlockBaseData* pMainBlockBaseData);
	void SetGraph(CGraphImp* pGraph);

	// �̹� ������� SubGraph�� data�� setting.
//	void SetSubGraphs(const CString& strData);

	// subGraph�� ���̱� setting.
	bool SetAllHiding(const bool bIsHiding);
	bool SetHiding(const CString& targetTitle, const bool bIsHiding);
	bool SetHiding(const int nIndex, const bool bIsHiding);
	// subGraph data setting.
	bool SetData(const int nIndex, const CString& changeTitle, const CGraphBaseData::GRAPHTYPE& sourceType, 
		const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, const CPenThickness& penThickness);
	// subGraph name setting.
	bool SetSubGraphName(const int nIndex, const CString& strSubGraphName);
	// subGraph title setting.
	bool SetTitle(const CString& orgTitle, const CString& changeTitle);
	bool SetTitle(const int nIndex, const CString& changeTitle);
	// subGraph type ����.
	bool SetType(const int nIndex, const CGraphBaseData::GRAPHTYPE& sourceType, 
		const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle);
	// subGraph ���� ���� ����.
	bool SetAllSubGraphPenThickness(const CPenThickness& penThickness);
	bool SetPenThickness(const int nIndex, const CPenThickness& penThickness);
	// subGraph �� ����.
	bool SetSubGraphColor(const int nIndex, const CGraphColor& graphColor);

	// ��� subGraphData ����.
	bool ChangeAllSubGraphData(const CList<CSubGraphData*, CSubGraphData*>& sourceSubGraphDataList, const bool bIsAllChange = true);
	bool ChangeSubGraphData(const CString& targetSubGraphName, const CSubGraphData& sourceSubGraphData);


	// subGraphs ��ΰ� �������� üũ
	bool AreHidingAllSubGraphs() const;
	// subGraph title name�� ��� empty���� üũ
	bool IsViewableSubGraphTitleEmpty() const;
	// subGraph���� �����ϴ��� ����
	bool IsExistenceSubGrape(const CString& subGraphName) const;
	// �ش� graph type�� �����ϴ��� üũ
	bool IsViewableGraphType(const CGraphBaseData::GRAPHTYPE eGraphType) const;
	bool IsViewableGraphType(const CGraphBaseData::GRAPHTYPE eGraphType, const int nSubGraphIndex) const;
	// �ش� graph style�� �����ϴ��� üũ
	bool IsViewableGraphStyle(const CGraphBaseData::GRAPHTYPE eGraphType, const int nGraphStyle) const;
	bool IsViewableGraphStyle(const CGraphBaseData::GRAPHTYPE eGraphType, const int nGraphStyle, const int nSubGraphIndex) const;
	// �ش� graph draw style�� �����ϴ��� üũ
	bool IsViewableGraphDrawStyle(const CGraphBaseData::GRAPHTYPE eGraphType, const int nGraphStyle, const int nGraphDrawStyle) const;
	// ���� packet�� �����ϴ��� Ȯ���Ѵ�.
	bool IsExistenceSamePacketInGraph(CGraphImp* pTargetGraph) const;
	bool IsExistenceSamePacket(CPacket* pPacket) const;


	// �θ� �ּ�
	CGraphImp* GetGraph() const;
	int GetSubGraphCount() const;
	// ��� subGraphData
	bool GetAllSubGraphDataList(CList<CSubGraphData*, CSubGraphData*>& subGraphDataList) const;
	CSubGraphData* GetSubGraphData(const CString& strSubGraphName) const;
	CSubGraphData* GetSubGraphData(const int nSubGraphIndex) const;
	// ȭ�鿡 ���̴� subGraph�� ����
	int GetViewableSubGraphCount() const;
	// subGraphs name.
	CString GetSubGraphNames() const;
	CString GetSubGraphName(const int nIndex = 0) const;
	CString GetViewableSubGraphNames() const;
	// subGraphs title.
	bool GetAllSubGraphTitle(CList<CString, CString>& titleList) const;
	CString GetSubGraphTitle(const int nIndex = 0) const;
	CString GetViewableSubGraphTitles() const;
	// subGraph title�� ���� subGraph index.
	int GetSameTitleSubgraphIndex(const CString& subGraphTitle) const;
	// subGraphs���� data
	void GetSubGraphsData( CStringList &p_slSubGraphData) const;
	// subGraph type, style
	bool GetSubGraphType(const int nIndex, CGraphBaseData::GRAPHTYPE& eGraphType, CGraphStyle& graphStyle, CGraphDrawStyle& graphDrawStyle) const;
	CGraphBaseData::GRAPHTYPE GetSubGraphType(const int nIndex = 0) const;
	CGraphStyle GetSubGraphStyle(const int nIndex = 0) const;
	CGraphDrawStyle GetSubGraphDrawStyle(const int nIndex = 0) const;
	// graph ��
	bool GetSubGraphsColor(CList<CGraphColor, CGraphColor&>& graphColorList) const;
	CGraphColor* GetGraphColor(const int nSubGraphIndex) const;
	COLORREF GetGraphColor1(const int nSubGraphIndex) const;
	// subGraph title region
	CRect GetSubGraphTitleRegion(CDC* pDC, const CSubGraph* pSubGraph, const CRect& graphRegion, int& preTitleEndPosition) const;
	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	CRect GetSubGraphTitleRegion(CDC* pDC, const CSubGraph* pSubGraph, const CRect& graphRegion, int& preTitleEndPosition, int& nPreLineCount) const;
	// view data setting
	void GetSubGraphViewDataList(const CString& GraphName, const CString& packetName, CSubGraphViewDataList& subGraphViewDataList) const;
	// packet�� ���õ� ����
	bool GetAllSubGraphCalculateData(CGraphCalculateData& graphCalculateData) const;
	bool GetSubGraphPacketList(CList<CPacket*, CPacket*>& packetList) const;
	CSubGraphPacketList* GetSubGraphPacketList(const int nSubGraphIndex = 0) const;
	CPacket* GetHeadPacket() const;
	bool GetHeadPacketAndDataType(CPacket*& pPacket, double& dDataType) const;
	
	// ���� scale width ���ϱ�.
	int GetVertScaleRegionWidth(CDC* pDC, CVertScaleMaxWidthCalculatorData* pCalData) const;
	// ���� ���̴� ȭ�鿡���� min, max�� ���Ѵ�. ==> y���� ��
	bool GetDisplayDataMinMax(CVertDisplayDataMinMaxInputData* pInputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange) const;

	// graph �׸���
	// (2009/1/7 - Seung-Won, Bae) Support graph title drawing routine.
	bool DrawSubGraphTitles(CDC* pDC, CDrawingGraphData* pDrawingGraphData, int& graphTitlePosition, int& graphTitleLine, int& nGraphTitleEndLine);
	bool DrawSubGraphs(CDC* pDC, CDrawingGraphData* pDrawingGraphData);

	// subGraph Title�� �̿��Ͽ� subGraphã��.
	CSubGraph* FindSubGraphTitle(CDC* pDC, const CRect& allTitleRegion, const CPoint& point, CRect& titleRegion, int& preGraphTitleEndPosition) const;
	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	CSubGraph* FindSubGraphTitle(CDC* pDC, const CRect& allTitleRegion, const CPoint& point, CRect& titleRegion, int& preGraphTitleEndPosition, int& nPreLineCount) const;
	// Ÿ��Ʋ�� ã�� : �������� - ojtaso (20070322)
	CSubGraph* FindSubGraphTitle(const CString& strSubGraphTitle) const;
	// subGraph ã��.
	CSubGraph* FindSubGraph(const int nIndex) const;
	CSubGraph* FindSubGraph(const CString& strSubGraphName) const;
	bool FindSubGraph(const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque) const;
//	bool FindSubGraph(const CPoint& point, CString& strSubGraphData, int& nSubGraphIndex, int& nDataIndex) const;

	void ChangeSubGraphTitle(LPCTSTR lpszPreName, LPCTSTR lpszNewName);

	// (2009/5/26 - Seung-Won, Bae) Clear SubGraph Drawing Cahe.
	void ClearCalcDataForDraw( void);

private:
	CMainBlockBaseData* m_pMainBlockBaseData;
	CGraphImp* m_pGraph;
	CTypedPtrList<CObList, CSubGraph*> m_SubGraphList; // CSubGraph���� list��..

	bool MakeSubGraph( const char *p_szSubGraphData, const char *p_szIndicatorName);
	// Ÿ��Ʋ�� ����� ǥ�� : �������� - ojtaso (20070322)
	bool MakeSubGraph( const char *p_szSubGraphData, const char *p_szIndicatorName, LPCTSTR lpszItemName);
	// subGraph�� list�� �߰�&�����Ѵ�.
	void Add(CSubGraph* pSubGraph);
	// subGraph�� list�� �ϳ��� ���� ��� graph ����.
	bool DeleteEmptyGraph();
	// ��� subGraph�� ���� �̸� graph�� �����Ѵ�.
	bool DeleteGraphInAllSubGraphHiding();
	bool DeleteGraph();
	void DeleteSubGraphs(const int nStartIndex = 0);
	void DeleteSubGraph(CSubGraph* pSubGraph);

//	int CopySubGraphs(const CString& strData);
//	bool CopySubGraph(CSubGraph* pSubGraph, const CString& strSubGraphData);
	// CSubGraph�� ��� data�� �����Ѵ�.
//	bool SetSubGraphData(CSubGraph* pSubGraph, const CString& strSubGraphData);
	// CDrawingGraphData clear
	// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
	void ClearDrawingGraphData(CDrawingGraphData* pDrawingGraphData, BOOL bDrawConditionEachTitle);

	// list�� subGraph�� �̹� �ִ����� ã�´�. true = �̹� ������.
	bool IsSelectedSubGraphInDeque(CSubGraph* pSubGraph, CSelectedSubGraphDeque& selectedSubGraphDeque) const;
};

#endif // !defined(AFX_SUBGRAPHLIST_H__2255E3BA_4922_4512_9B33_39A02570ABCA__INCLUDED_)
