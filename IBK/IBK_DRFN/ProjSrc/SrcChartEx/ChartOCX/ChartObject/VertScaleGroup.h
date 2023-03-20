// VertScaleGroup.h: interface for the CVertScaleGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERTSCALEGROUP_H__D483D8C3_9A0B_4A61_AEE3_C8B9F50F1E8B__INCLUDED_)
#define AFX_VERTSCALEGROUP_H__D483D8C3_9A0B_4A61_AEE3_C8B9F50F1E8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _TEST
#ifdef _TEST
class CGraphImp;
class CSubGraph;
class CBlockImp;
class CGraphSearchInfo;
class CMainBlockBaseData;
class CMainChartFormulate;
class CVertScaleGroupList;
class CSubGraphViewDataList;
class CSelectedSubGraphDeque;

#include <afxtempl.h>
#include "../Include_Chart/BaseData.h"
#endif

class CVertScaleGroup : public CObject 
{
#ifdef _TEST
public:
	CVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList);	// (2006/11/26 - Seung-Won, Bae) Support Empty VScaleGroup
	CVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, char *p_szVertScaleGroup);
	// RQ�� �˻� : �������� - ojtaso (20070118)
	CVertScaleGroup(LPVOID pPacketRQ, CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, char *p_szVertScaleGroup);
	CVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, const CVerticalScale& vertScale);

// (2007/2/16 - Seung-Won, Bae) Protect the redestructing on Destructor.
protected:
	virtual ~CVertScaleGroup();
	BOOL	m_bOnDestructor;

// Delete
public:
	// Graph
	BOOL	DeleteAllIndicator( const char *p_szIndicatorName);

// Search
public:
	// Graph
	BOOL	FindGraph( CGraphSearchInfo *p_pGraphSearchInfo) const;
	// (2008/9/15 - Seung-Won, Bae) RQ Search
	BOOL	FindGraph( LPVOID pPacketRQ, CGraphSearchInfo *p_pGraphSearchInfo) const;

	// Sub Graph
	CSubGraph *	GetSubGraph( const int nGraphIndex, const int nSubGraphIndex) const;

	// (2007/2/20 - Seung-Won, Bae) Support Indicator Hiding
	//		It can not be replaced with AddChartBlock(), because of Condition Reset.
	void	ShowIndicator( const char *p_szIndicatorName, BOOL p_bShow, BOOL p_bRecalculate);

public:
	static CVertScaleGroup* MakeVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList); // (2006/11/26 - Seung-Won, Bae) Support Empty VScaleGroup
	static CVertScaleGroup* MakeVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, char *p_szVertScaleGroup);
	// RQ�� �˻� : �������� - ojtaso (20070118)
	static CVertScaleGroup* MakeVertScaleGroup(LPVOID pPacketRQ, CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, char *p_szVertScaleGroup);
	static CVertScaleGroup* MakeVertScaleGroup(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroupList* pVertScaleGroupList, const CVerticalScale& vertScale);
	static void Delete(CVertScaleGroup* pVertScaleGroup);
	CGraphImp* FindBaseGraphForDrawingVertScaleData() const;

private:
	// CGraphImp�� �����.
	CGraphImp* MakeNewGraph( char *p_szGraphData);
	// RQ�� ������ �ִ� Graph ���� : �������� - ojtaso (20061224)
	// CPacketRQ*�� ���� : �������� - ojtaso (20070109)
	CGraphImp* MakeNewGraph(LPVOID pPacketRQ, char *p_szGraphData);	
	bool DeleteEmptyGroup();
	// graph�� ���Ͽ� packet�� �����Ѵ�.
	void DeleteGraph(CGraphImp* pDeleteGraph, CGraphImp* pSourceGraph);
	// packet�� �������� �ʰ� graph�� �����Ѵ�.
	void DeleteGraphNoDeletePacket(CGraphImp* pGraph);
	// �� graph�� �ٲ۴�.
	void ChangeGraph(CGraphImp* pSourceGraph, CGraphImp* pTargetGraph);
//	bool CopyGraphData(CGraphImp* pTargetGraph, const CString& sourceGraphData);

	// �̹� �����Ǿ� �ִ� graph�� data�� setting.
	void ModifyGraph( CGraphImp* pGraph, char *p_szGraphData);

	// ���� ���̴� ȭ�鿡���� min, max�� ���Ѵ�. ==> y���� ��
	CDisplayDataMinMax GetDisplayDataMinMaxInAll() const;
	bool GetDisplayDataMinMaxInOnlyPrice(CDisplayDataMinMax& displayDataMinMax) const;
	void GetDisplayDataMinMax(CGraphImp* pGraph, CDisplayDataMinMax& displayDataMinMax) const;

	// draw �ϱ� ���� �� ����
	void SetDrawingVertScaleData(CDrawingVertScaleData* pDrawingVertScaleData);
	// ���� scale�� data �ʱ�ȭ
	void InitializeDrawingVertScaleData(CDrawingVertScaleData* pDrawingVertScaleData);

	// scale�� �׸����� ����
	bool IsVertScaleDrawing() const;
	// <������Ʈ> ���� scale ���� ����
	bool DoesOnlyPriceVertScale() const;
	
private:
	CMainBlockBaseData* m_pMainBlockBaseData;
	CVertScaleGroupList* m_pVertScaleGroupList; // ���� class �ּ�
	CVerticalScale m_vertScale;	// ���� scale
	CDisplayDataMinMax m_displayDataMinMax;	// ȭ���� Min/Max
	CTypedPtrList<CObList, CGraphImp*> m_GraphList; // CGraphImp���� list��..
	// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
	CGraphImp*	m_lpSelectedGraph;

public:
	// graph�� ����&����
	void MakeGraphs( char *p_szVertScaleGroup);
	// RQ�� �˻� : �������� - ojtaso (20070118)
	void MakeGraphs(LPVOID pPacketRQ,  char *p_szVertScaleGroup);
	bool MakeGraph( char *p_szGraphData, const bool bIsAddTail = true);
	// RQ�� ������ �ִ� Graph ���� : �������� - ojtaso (20061224)
	// CPacketRQ*�� ���� : �������� - ojtaso (20070109)
	bool MakeGraph(LPVOID pPacketRQ, char *p_szGraphData, const bool bIsAddTail = true);
	bool DeleteGraphWithNotify( CGraphImp* pGraph, const bool bIsOnlyNoTRData = true);
	bool DeleteGraph(CGraphImp* pGraph, const bool bIsOnlyNoTRData = true);
	void DeleteAll();
	void AddGraph(CGraphImp* pGraph, const bool bIsAddTail = true);
	bool RemoveAt(CGraphImp* pGraph, const bool bIsDeleteBlock);

	bool MoveAllGraphs(CVertScaleGroupList& groupList);
	bool MoveAllGraphs(CVertScaleGroup* pSourceGroup);
	// graph ����
//	bool ChangeGraph(const CString& targetGraphTitle, const CString& sourceGraphData);
	// graph GraphName/subGraphName �� ����. (�ش� graph�� ��� ����, subGraphCount�� 1���� ����.)
//	bool ChangeAllGraphNameAndSubGraphName(const CString& targetGraphName, 
//		const CString& sourceGraphName, const CString& sourceSubGraphName);
	// graph GraphName/subGraphName �� ����. -> cfg�� �Լ����� �ٲ�� ��� �����
//	bool ChangeAllGraphNameAndSubGraphNames(const CString& targetGraphName, const CString& sourceGraphName, const bool bIsSubGraphCheck);
	// graph type ����.
	bool ChangeGraphType(const CString& targetGraphName, const CGraphBaseData::GRAPHTYPE& sourceType, 
		const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, const CString& sourcePacketNames);
	// subGraph data ����.
	bool ChangeSubGraphData(const CString& targetGraphName, const CString& targetSubGraphName, const CSubGraphData& sourceSubGraphData);
	// subGraph ���� ���� ����.
	bool ChangeSubGraphPenThickness(const CString& targetGraphName, const int nSubGraphIndex, const CPenThickness& penThickness);
	// subGraph title ����.
	bool ChangeSubGraphTitle(const CString& orgSubGraphTitle, const CString& changeSubGraphTitle);
	// subGraph �� ����.
	bool ChangeAllHeadSubGraphColor(const CList<CGraphColor, CGraphColor&>& graphColorList, POSITION& colorPos) const;
	// �̹� �����Ǿ� �ִ� graph�� data�� setting.
	void ModifyGraphDataInVertScaleGroup( char *p_szVertScaleGroup, const bool doesMaintainOldScale);
	// �ش� graph�� indicatorInfo ���� ����.
	// -> gaphData(subGraph����)�� IndicatorInfo�� ����.
	//    (��ǥ���� ���õ� ������ ����� ��� ��ǥ��� ����)
//	bool ChangeAllGraphDataFromIndicatorInfo(const bool bIsCalculate);
	// -> IndicatorInfo�� gaphData(subGraph����)�� ����.
	bool ChangeAllIndicatorInfoFromGraphData();
	BOOL	ChangeAllGraphDataFromIndicatorInfo( BOOL p_bWithCalculate);
	// -> �ش� graph�� Map Default IndicatorInfo�� ����
	bool ChangeAllMapDefaultIndicatorInfo( CIndicatorList* pIndicatorList);


	// ���� class �ּ�
	void SetVertScaleGroupList(CVertScaleGroupList* pVertScaleGroupList);
	void SetMainBlockBaseData(CMainBlockBaseData* pMainBlockBaseData);
	// ���� scale �� �ٲٱ�.
	void SetVerticalScale(const CVerticalScale& vertScale);
	// subGraph�� ���̱� setting.
	void SetAllGraphsHiding(const bool bIsHiding);
	bool SetSubGraphHiding(const CString& subGraphTitle, const bool bIsHiding);
	bool SetSubGraphHiding(const CString& strGraphName, const int nSubGraphIndex, const bool bIsHiding);
	// graph�� indicatorinfo�� �ʱ�ȭ(NULL)
	void Initialize_IndicatorInfo(const bool bIsNULL);
	// graph(subGraph��)�� packet���� �ʱ�ȭ
	void Initialize_AllPackets(const bool bIsCalculate);
	bool Initialize_Packet(const CString& strPacketName, const bool bIsCalculate);

	// ��ǥ�� ����Ѵ�.
	void CalculateAllGraphs(const CCalculateBaseData::CALCULATETYPE eCalType);
	bool CalculateAllGraphs(const CList<CString, CString>& packetNameList, const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsHeadGraph);
	// vert scale Min/Max �� ����Ѵ�.
	void CalculateDisplayDataMinMax();

	// ���� class �ּ�
	CMainChartFormulate* GetMainChartFormulate() const;
	CBlockImp* GetBlock() const;
	CVertScaleGroupList* GetVertScaleGroupList() const;
	// ���� scale ��
	CVerticalScale& GetVertScale();
	const CVerticalScale& GetVertScale() const;
	// ��� VertScaleGroup�� ��� data
	void GetVertScaleGroupAllData( CStringList &p_slVertScaleGroupData) const;
	// ������ �ִ� ��� graph GraphName
	bool GetAllIndicatorName(CList<CString, CString>& strIndicatorNameList) const;
	bool GetAllGraphName(CList<CString, CString>& strGraphNameList) const;

	// graph
	int GetGraphCount() const;
	CString GetGraphsGraphName() const;
	// graph ��
	// ��� block�� �ִ� ��� graph�� head subGraph color ��������.
	bool GetAllHeadSubGraphColor(CList<CGraphColor, CGraphColor&>& graphColorList) const;
	COLORREF GetGraphColor1(const int nGraphIndex, const int nSubGraphIndex) const;
	// subGraph title region
	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	CRect GetSubGraphTitleRegion(CDC* pDC, const CSubGraph* pSubGraph, const CRect& graphRegion, int& preTitleEndPosition, int& nPreLineCount) const;

	// graph�� ���� scale�� ���� ���Ѵ�.
	// (2008/6/13 - Seung-Won, Bae) for Support BuySell Overhanging Supply.
	void	GetLeftVertScaleRegionWidth(	CDC* pDC, int &p_nLeftWidth,	BOOL p_bIsDualScale) const;
	void	GetRightVertScaleRegionWidth(	CDC* pDC, int &p_nRightWidth,	BOOL p_bIsDualScale) const;
	// �ش� RQ�� �۾� : �������� - ojtaso (20070420)
	void GetSubGraphViewDataList(LPCTSTR lpszRQ, CSubGraphViewDataList& subGraphViewDataList) const;
	// ���� ���̴� ȭ�鿡���� min, max�� ���Ѵ�. ==> y���� ��
	CDisplayDataMinMax GetDisplayDataMinMax() const;


	// graph title name�� ��� empty���� ���ϱ�
	bool IsViewableGraphTitleEmpty() const;
	// �̹� �����ϴ� graph���� üũ
	bool IsExistenceGraph( const CString &strGraphName) const;
	// RQ�� ������ �ִ� Graph üũ : �������� - ojtaso (20061224)
	bool IsExistenceGraph(LPCTSTR lpszRQ, const CString &strGraphName) const;
	// CPacketRQ*�� Graph üũ : �������� - ojtaso (20070109)
	bool IsExistenceGraph(LPVOID pPacketRQ, const CString &strGraphName) const;
	bool IsExistenceIndicator( const CString &strIndicatorName) const;
	// RQ�� ������ �ִ� Graph üũ : �������� - ojtaso (20061224)
	bool IsExistenceIndicator(LPCTSTR lpszRQ, const CString &strIndicatorName) const;
	// �ش� graph Type�� �����ϴ��� üũ
	bool IsViewableGraphType(const CGraphBaseData::GRAPHTYPE eGraphType) const;
	// �ش� graph style�� �����ϴ��� üũ
	bool IsViewableGraphStyle(const CGraphBaseData::GRAPHTYPE eGraphType, const int nGraphStyle) const;
	// �ش� graph draw style�� �����ϴ��� üũ
	bool IsViewableGraphDrawStyle(const CGraphBaseData::GRAPHTYPE eGraphType, const int nGraphStyle, const int nDrawStyle) const;
	// ��� graph �� hide ���� üũ
	bool DoesAreHidingAllGraphs() const;
	// exceptionGraph �� �����ϰ� ���� indicatorInfo�� ����ϴ��� ����
	bool DoesUseEqualIndicatorInfo(const CIndicatorInfo* pIndiInfo, const CGraphImp* pExceptionGraph) const;

	// ���� scale �׸���.
	void DrawVertScale(CDC* pDC, CDrawingVertScaleData* pDrawingVertScaleData, const CDrawingRegion& drawingRegion);
	// graph �׸���
	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	void DrawGraphs(CDC* pDC, CDrawingGraphData* pDrawingGraphData, int& preGraphTitleEndPosition, int& nEndLimit, int& nGraphTitleEndLine);
	// (2009/10/9 - Seung-Won, Bae) for Thread Draw.
	static UINT DrawGraphAndSignal( LPVOID pParam);
	
	// graph ã��
	CGraphImp* FindGraph(const CString& strGraphName) const;
	// RQ�� ������ �ִ� Graph �˻� : �������� - ojtaso (20061224)
	CGraphImp* FindGraph(LPCTSTR lpszRQ, const CString& strGraphName) const;
	CGraphImp* GetGraph(const int nIndex) const;
	CGraphImp* FindGraph(const CString& strTitleName, int& nSubGraphIndex) const;
	CGraphImp* FindIndicator( const CString& strIndicatorName) const;
	// RQ�� ������ �ִ� Graph �˻� : �������� - ojtaso (20061224)
	CGraphImp* FindIndicator(LPCTSTR lpszRQ, const CString& strIndicatorName) const;

	// subGraph Title�� �̿��Ͽ� subGraphã��.
	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	CSubGraph* FindSubGraphTitle(CDC* pDC, const CRect& allTitleRegion, const CPoint& point, 
		CRect& titleRegion, int& preTitleEndPosition, int& nPreLineCount) const;
	// subGraph ã��.
	bool FindSubGraph(const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque) const;
//	bool FindSubGraph(const CPoint& point, CString& strSubGraphData, int& nSubGraphIndex, int& nDataIndex) const;

	// (2006/6/25 - Seung-Won, Bae) Search Index of Graph
	BOOL	GetIndexOfGraph( const char *p_szGraphName, int &p_nG);

	// Change Input Packet Names and Calculate again.
	BOOL ChangeIndicatorPacketNames( const char *p_szIndicatorName, const int p_nSubGraphIndex, const char *p_szPacketNames);

	// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
	BOOL IsScaleTextDrawing();
	// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
	void SetSelectedGraph(CGraphImp*& lpSelectedGraph);

	// ��ǥ ������ - ojtaso (20071023)
	bool	GetVertScaleInvertType() const;
	// ��ǥ ������ - ojtaso (20071023)
	void	SetVertScaleInvertType(const bool bIsVertScaleInvertType);

// (2007/1/14 - Seung-Won, Bae) Get the Base Packet for Default Scale Min/Max
protected:
	CGraphImp *	m_pBaseGraph;
	CPacket *	m_pBasePacket;

// (2009/10/15 - Seung-Won, Bae) Retrieve the selected graph.
public:
	CGraphImp *		GetSelectedGraph( void)		{	return m_lpSelectedGraph;	}

#endif

};

#endif // !defined(AFX_VERTSCALEGROUP_H__D483D8C3_9A0B_4A61_AEE3_C8B9F50F1E8B__INCLUDED_)
