// MainBlock.h: interface for the CMainBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINBLOCK_H__405226D5_F3E1_4335_BC7A_76FA68A1E0A7__INCLUDED_)
#define AFX_MAINBLOCK_H__405226D5_F3E1_4335_BC7A_76FA68A1E0A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "BlockBaseData.h"

class CCommonInterface;
class CGraphRegionColor;
class CMainBlockImplementation;
class CPacketList;
class CPacket;
class CIndicatorList;
class CBlock;
class CSubGraph;
class CScaleColor;
class CSelectedSubGraphDeque;
class CViewData;
class CCoordinateCalculatorData;
class CIndicatorConfigurationChangedData;
class CBlockRegionList;
class CGraphColor;
class CSubGraphData;
class CSubGraphNameAndColorList;


class AFX_EXT_CLASS CMainBlock
{
public:
	CMainBlock();
	CMainBlock(CCommonInterface* pParentIF);
	virtual ~CMainBlock();

	// block�� ����
	bool DeleteBlock(const CPoint& point, const bool bAutoHorzScaleDelete = true);
	bool DeleteBlock(const CBlockIndex& blockIndex, const bool bAutoHorzScaleDelete = true);
	bool DeleteAllBlock();
	bool DeleteBlockHavingNoGraphs();
	// graph ����
	bool DeleteGraph(const CString& strFunctionName);
	bool DeleteGraph_TitleName(const CString& strTitleName, const bool bIsOnlyNoTRData = true);
	bool DeleteGraph_TitleName(const CBlockIndex& blockIndex, const CString& strTitleName, const bool bIsOnlyNoTRData = true);
	bool DeleteGraph_PacketNames(const CString& strGraphPacketNames);
	// graph �߰�
	bool InsertGraph(const CString& graphData, const CGraphBaseData::ObjectChange blockType, const bool doesMaintainOldScale);
	bool InsertGraph(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const CString& graphData, 
		const bool doesInsertInNewBlock = false, const bool doesInsertInNewVertScaleGroup = false, const bool doesMaintainOldScale = true);
	bool InsertGraph(const CPoint& insertPosition, const CString& graphData, const CGraphBaseData::ObjectChange blockType, const bool doesMaintainOldScale);
	// vertScaleGroup �ش� block�� �̵�
	bool MoveAllVertScaleGroups(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex);
	bool MoveVertScaleGroup(const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const CBlockIndex& targetBlockIndex);
	// vertScaleGroup�� join
	bool JoinAllVertScaleGroups(const CBlockIndex& blockIndex, const bool bIsJoin = true);
	// graph �̵�.
	bool MoveAllGraphs(const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const CBlockIndex& targetBlockIndex, const int nTargetGroupIndex = -1);
	bool MoveGraph(const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const int nSourceGraphIndex, const CBlockIndex& targetBlockIndex, const int nTargetGroupIndex = -1);
	// graph ����.
	// -> GraphTitle�� target ������ �����Ѵ�.
	bool ChangeGraph(const CBlockIndex& blockIndex, const CString& targetGraphTitle, const CString& sourceGraphData);
	bool ChangeGraph(const CString& targetFunctionName, const CString& sourceGraphData);
	bool ChangeGraph(const CString& targetFunctionName, const CIndicatorConfigurationChangedData& changedData);
	// graph functionName/subGraphName �� ����. (�ش� graph�� ��� ����, subGraphCount�� 1���� ����.)
	bool ChangeAllGraphFunctionNameAndSubGraphName(const CString& targetFunctionName, 
		const CString& sourceFunctionName, const CString& sourceSubGraphName);
	// graph functionName/subGraphName �� ����. -> cfg�� �Լ����� �ٲ�� ��� �����
	bool ChangeFunctionNameAndSubGraphNames(const CString& targetFunctionName, const CString& sourceFunctionName, const bool bIsSubGraphCheck = true);
	// graph packetName ����.
	bool ChangePacketNames(const CString& targetFunctionName, const CString& sourcePacketNames);
	// graph Data ����.
	bool ChangeGraphData(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, 
		const CString& sourceFunctionName, const CString& sourcePacketNames, const CString& sourceSubGraphTitle, 
		const CGraphBaseData::GRAPHTYPE& sourceType, const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, 
		const int nPenThickness1, const int nPenThickness2, const int nPenThickness3);
	bool ChangeGraphData(const CString& targetFunctionName, const int nSubGraphIndex, const CString& sourcePacketNames, const CString& sourceSubGraphTitle, 
		const CGraphBaseData::GRAPHTYPE& sourceType, const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, 
		const int nPenThickness1, const int nPenThickness2, const int nPenThickness3);
	// GraphDataDrawingData ����.
	bool ChangeGraphDataDrawingData(const CString& targetFunctionName, 
		const bool bLowHigh, const bool bLowCur, const bool bHighCur, const bool bPrevCur, const bool bPrevCurRate);
	// graph type ����.
	bool ChangeGraphType(const CString& targetFunctionName, const CGraphBaseData::GRAPHTYPE& sourceType, 
		const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, const CString& sourcePacketNames);
	bool ChangeGraphType(const CString& targetFunctionName, const int nSubGraphIndex, const CGraphBaseData::GRAPHTYPE eGraphType, const CGraphStyle& graphStyle, const CGraphDrawStyle& graphDrawStyle);
	bool ChangeGraphType(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, 
		const CGraphBaseData::GRAPHTYPE eGraphType, const CGraphStyle& graphStyle, const CGraphDrawStyle& graphDrawStyle);
	// subGraph data ����.
	bool ChangeSubGraphData(const CString& targetFunctionName, const CString& targetSubGraphName, const CSubGraphData& sourceSubGraphData);
	// subGraph ���� ���� ����.
	// -> nSubGraphIndex : �ٲ� subGraph index (-1 ���� �����. -1 = ��� subGraph�� �ش�)
	bool ChangeSubGraphPenThickness(const CString& targetFunctionName, const int nSubGraphIndex, const int nPenThickness1, const int nPenThickness2, const int nPenThickness3);
	// subGraph title ����.
	bool ChangeSubGraphTitle(const CString& orgSubGraphTitle, const CString& changeSubGraphTitle);
	bool ChangeSubGraphTitle(const CBlockIndex& blockIndex, const CString& orgSubGraphTitle, const CString& changeSubGraphTitle);
	bool ChangeSubGraphTitle(const CBlockIndex& blockIndex, const int nScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const CString& changeSubGraphTitle);
	bool ChangeSubGraphTitle(const CString& strFunctionName, const int nSubGraphIndex, const CString& changeSubGraphTitle);
	// subGraph color ����.
	bool ChangeGraphColor(const CString& strFunctionName, const CSubGraphNameAndColorList& graphNameAndColorList);
	bool ChangeGraphColor(const CString& strFunctionName, const int nSubGraphIndex, const CGraphColor& graphColor);
	bool ChangeGraphColor1(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const COLORREF& newGraphColor1);
	// �ش� block�� graph���� ����.
	bool ModifyGraphDataInBlock(const CString& graphsData, const CBlockIndex& blockIndex, const bool doesMaintainOldScale = true);
	// �ش� graph�� indicatorInfo ���� ����.
	// -> gaphData(subGraph����)�� IndicatorInfo�� ����.
	//    (��ǥ���� ���õ� ������ ����� ��� ��ǥ��� ����)
	bool ChangeAllGraphDataFromIndicatorInfo(const bool bIsCalculate);
	bool ChangeGraphDataFromIndicatorInfo(const CString& strFunctionName, const bool bIsCalculate);
	// -> IndicatorInfo�� gaphData(subGraph����)�� ����.
	bool ChangeAllIndicatorInfoFromGraphData();
	bool ChangeIndicatorInfoFromGraphData(const CString& strFunctionName);


	// object�� �⺻���� set. -- block, graph, scale, data Set.

	// block�� ������ ��� ���� ����� --> ó������ block�� ���� ���
	bool SetBlockEnvironment(CDC* pDC, const CRect& allBlockRegion, const CBlockBaseData::BLOCKTYPE blockType, const CBlockBaseData::BLOCKCOMPART blockCompart, 
			const CBlockIndex& blockCount, const int graphRegionFrameThickness, const int interBlockHalfGap, const CGraphRegionColor& graphRegionColor);
	bool SetAllBlockRegion(const CRect& allBlockRegion, const bool bInitialize = false);
	bool SetBlockType(const CBlockBaseData::BLOCKTYPE blockType, const CBlockIndex& blockCount);
	void SetBlockCompart(const CBlockBaseData::BLOCKCOMPART blockCompart);
	void SetDrawingOrderType(const CDrawingOption::DRAWINGORDER eDrawingOrderType);
	void SetBlockCount(const CBlockIndex& blockCount);
	void SetGraphRegionFrameThickness(const int graphRegionFrameThickness);
	void SetInterBlockHalfGap(const int interBlockHalfGap);
	void SetGraphTitleDraw(const bool bIsCondition, const bool bIsData);
	void SetBackgroundColor(const COLORREF& color);
	void SetGraphRegionColor(const CGraphRegionColor& graphRegionColor);
	void SetEtcGraphColor(const COLORREF& graphColor1, const COLORREF& graphColor2, const COLORREF& graphColor3, const COLORREF& graphColor4, const COLORREF& graphColor5, const COLORREF& graphColor6);

	// block�� ����(����block + gap + ���� ����)�� ���ϴ� ������ ���� 
	// --> block�� �̹� ������� ��
	void SetBlocksPosition(const CString& blocksPosition);
	// �ٸ� block size���� 2���� block ����
	void SetDoubleSizeBlockNumber(const CBlockIndex& doubleSizeBlockIndex);
	// Tool�� ���õ� ���� ���� (block�̵�, block����)

	// block�� Graph���� �����Ѵ�.
	void BuildGraphsAndScales(const CString& data);

	// scale�� data ����
	void SetScaleEnvironment(const CScaleBaseData::SCALEGRIDTYPE gridType, const COLORREF& horzGridColor, const COLORREF& vertGridColor, const COLORREF& textColor);
	void SetBaseScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType);
	void SetAllScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType);
	void SetAllHorzScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType);
	void SetAllVertScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType);
	void SetScaleColor(const COLORREF& horzGridColor, const COLORREF& vertGridColor, const COLORREF& textColor);
	bool SetHorzScaleData(const CBlockIndex& blockIndex, const CString& strScaleData);
	bool SetVertScaleData(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const CString& strScaleData);
	void SetHorzScaleCompart(const CBlockIndex& blockIndex, const CString& dateCompart, const CString& timeCompart, const CScaleBaseData::TEXTCOMPARTTYPE eTextCompart = CScaleBaseData::HORZ_TEXT_COMPART);
	bool SetHorzScalePosition(const CBlockIndex& blockIndex, const CScaleBaseData::HORZSCALEPOSITION ePosition);
	bool SetHorzScalePosition(const int nBlockRowIndex, const CScaleBaseData::HORZSCALEPOSITION ePosition);
	void SetAllHorzScalePosition(const CScaleBaseData::HORZSCALEPOSITION ePosition);
	bool SetVertScalePosition(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const CScaleBaseData::VERTSCALEPOSITION ePosition);
	void SetAllVertScalePosition(const CScaleBaseData::VERTSCALEPOSITION ePosition);
	void SetOnlyPriceVertScale(CDC* pDC, const bool bIsOnlyPrice, const bool bIsCalVertMinMax = true);
	void SetDisplayDataVertScale(CDC* pDC, const bool bIsDisplayData, const bool bIsCalVertMinMax = true);

	// ȭ�鿡 ���� data�� ����.
	void SetDataRegionInBlock(const int start, const int end);
	// ȭ���� ���� design mode���� ����.
	void SetDesignMode(const bool designMode = true);
	// ȭ�鿡 �߰��� �� �ִ� �� block ��
	void SetMaxBlocksCountInView(const int maxBlockCount);
	// messagebox ���� ����
	void SetMessageBoxShow(const bool bIsShow);
	// graph background gap ����
	void SetGraphBackgroundGap(const int nGap_x, const int nGap_y);

	// ocx �ּ�
	void SetParentInterface(CCommonInterface* pParentIF);
	// packet�� ���� ����
	void SetPacketList(CPacketList* pPacketList);
	// prject ���
	void SetProjectPath(const CString& projectPath);
	// �Ӽ��� ���� ����
	void SetIndicatorList(CIndicatorList* pIndicatorList);
	// graph ������ �ش� graph�� indicatorInfo�� �����Ѵ�.
	void SetAutoDeleteIndicatorInfo(const bool bIsAutoDelete);
	
	// graph�� indicatorInfo �ʱ�ȭ
	void Initialize_IndicatorInfo(const bool bIsNULL);
	// graph(subGraph��)�� packet���� �ʱ�ȭ(bIsCalculate = true �ٽ� �����)
	void Initialize_AllPackets(const bool bIsCalculate);
	bool Initialize_AllPackets(const CString& strFunctionName, const bool bIsCalculate);
	bool Initialize_Packet(const CString& strPacketName, const bool bIsCalculate);
	// ��ǥ�� ����Ѵ�.
	// ����ؾ� �ϴ� ��� : TR, �ǽð�, ���ǰ�, �Լ���, packetname, ��ũ��, ����
	void CalculateAllGraphs(const CCalculateBaseData::CALCULATETYPE eCalType = CCalculateBaseData::TRDATA_CAL);
	bool CalculateGraph(const CString& strFunctionName, const CCalculateBaseData::CALCULATETYPE eCalType = CCalculateBaseData::TRDATA_CAL);
	bool CalculateGraph(const CString& strFunctionName, const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsClearPacketData);


	// block�� ���� ���� �ٲٱ�.
	bool SetBlockTypeNotChangeBlockData(const CBlockBaseData::BLOCKTYPE blockType, const CBlockIndex& blockCount);

	// subGraph�� ���̱� ���� setting.
	void SetAllGraphsHiding(const bool bIsHiding = true);
	void SetAllGraphsHiding(const CBlockIndex& blockIndex, const bool bIsHiding = true);
	bool SetSubGraphHiding(const CBlockIndex& blockIndex, const CString& subGraphTitle, const bool bIsHiding = true);
	bool SetSubGraphHiding(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const bool bIsHiding = true);
	// subGraph title ����� ���� setting.
	void SetAllGraphsTitleHiding(const bool bIsAllHiding = true);
	// block�� ���� scale�� log/invert type setting.
	bool SetVertScaleLogAndInvertType(const CBlockIndex& blockIndex, const bool bIsLogType, const bool bIsInvertType);
	bool SetVertScaleLogAndInvertType(const CString& strFunctionName, const bool bIsLogType, const bool bIsInvertType);
	// block�� ���� scale�� log type setting.
	bool SetAllVertScaleLogType(const bool bIsLogType);
	bool SetVertScaleLogType(const CBlockIndex& blockIndex, const bool bIsLogType);
	bool SetVertScaleLogType(const CString& strFunctionName, const bool bIsLogType);
	// block�� ���� scale�� invert type setting.
	bool SetVertScaleInvertType(const CBlockIndex& blockIndex, const bool bIsInvertType);
	bool SetVertScaleInvertType(const CString& strFunctionName, const bool bIsInvertType);


	// block�� ���� data ��������
	// ���� main block�� data�� �ִ����� �����´�.
	bool IsEmpty() const;
	// ���� ȭ�鿡 ���� Mode�� �����´�.
	bool GetDesignMode() const;
	// region
	CRect GetAllBlockRegion() const;
	CRect GetBlockRegion(const CBlockIndex& blockIndex) const;
	CRect GetNoFrameBlockRegion(const CBlockIndex& blockIndex) const;
	CRect GetGraphRegionSumInColumn(const int blockColumnIndex = 0) const;
	CRect GetGraphRegionSumInColumn(const CPoint& point) const;
	CRect GetNoFrameGraphRegion(const CString& strFunctionName) const;
	CRect GetNoFrameGraphRegion(const CBlockIndex& blockIndex) const;	
	CRect GetNoFrameGraphRegion(const CPoint& point) const;
	CRect GetDrawingGraphRegion(const CString& strFunctionName) const;
	CRect GetDrawingGraphRegion(const CBlockIndex& blockIndex) const;
	CRect GetDrawingGraphRegion(const CPoint& point) const;
	bool GetBlockRegionList(const int blockColumnIndex, CBlockRegionList& blockRegionList) const;
	bool GetBlockRegionList(const CPoint& point, CBlockRegionList& blockRegionList) const;
	// type
	CBlockBaseData::BLOCKTYPE GetBlockType() const;
	CBlockBaseData::BLOCKCOMPART GetBlockCompartType() const;
	CDrawingOption::DRAWINGORDER& GetDrawingOrderType();
	// block�� ����
	int GetRowCount() const;
	int GetColumnCount() const;
	// graph�� ����
	int GetTotalGraphCount() const;
	int GetGraphCount(const CBlockIndex& blockIndex) const;
	int GetGraphCount(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex) const;
	// ȭ�鿡 ���̴� data ����/��
	void GetDisplayDataIndex(int& nDataStartInBlock, int& nDataEndInBlock) const;
	// graph ���� �׵θ� �β�
	int GetGraphRegionFrameThickness() const;
	// block ���� gap/2
	int GetInterBlockHalfGap() const;
	// �� ȭ�鿡 ���� �ִ� block ����
	int GetMaxBlocksCountInView() const;
	// block�� ���� ��������
	CString GetBlocksPosition() const;
	// ��� block�� data
	CString GetAllBlocksData() const;
	// ������ �ִ� graph functionName
	CString GetGraphFunctionName(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex = 0, const int nGraphIndex = 0) const;
	CString GetAllGraphFunctionName(const CString& strCompart = ',') const;
	bool GetAllGraphFunctionName(CList<CString, CString>& strFunctionNameList) const;
	// �ش� graph �� �ִ� block�� ��� ��ǥ�� �Լ��� �������� (�Լ��� ������ ";" �� �Ѵ�.)
	CString GetGraphsFunctionName_InBlock(const CString& strFunctionName) const;
	// 2�� ũ�� block
	CBlockIndex GetDoubleSizeBlockNumber() const;
	// �ش� graph functionName�� �ִ� block index
	CBlockIndex GetBlockIndex(const CString& strFunctionName) const;
	// block data�� ���� block index
	CBlockIndex GetBlockIndex_SameBlockData(const CString& graphData, const bool doesMaintainOldScale = true) const;

	
	// vert scale type
	bool GetVertScaleLogAndInvertType(const CBlockIndex& blockIndex, bool& bIsLog, bool& bIsInvert) const;
	bool GetVertScaleLogAndInvertType(const CString& strFunctionName, bool& bIsLog, bool& bIsInvert) const;
	// scale grid type
	CScaleBaseData::SCALEGRIDTYPE GetBaseScaleGridType() const;
	// scale position
	CScaleBaseData::VERTSCALEPOSITION GetVertScalePosition(const CBlockIndex& blockIndex = CBlockIndex(0, 0), const int nVertScaleGroupIndex = 0) const;
	// scale ��
	CScaleColor& GetScaleColor();
	void GetScaleColor(COLORREF& horzGridColor, COLORREF& vertGridColor, COLORREF& textColor) const;
	// scale data
	CString GetHorzScaleData(const CBlockIndex& blockIndex) const;
	// graph region�� ���� ���� �����´�.
	COLORREF GetBackgroundColor() const;
	CGraphRegionColor GetGraphRegionColor() const;
	void GetGraphRegionColor(COLORREF& backgroundColor, COLORREF& lineColor, COLORREF& frameColor) const;
	// graph ��
	bool GetGraphColor(const CString& strFunctionName, CSubGraphNameAndColorList& graphNameAndColorList) const;
	bool GetGraphColor(const CString& strFunctionName, const int nSubGraphIndex, CGraphColor& graphColor) const;
	CGraphColor* GetGraphColor(const CString& strFunctionName, const int nSubGraphIndex = 0) const;
	COLORREF GetGraphColor1(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex) const;
	void GetEtcGraphColor(COLORREF& graphColor1, COLORREF& graphColor2, COLORREF& graphColor3, COLORREF& graphColor4, COLORREF& graphColor5, COLORREF& graphColor6) const;
	// graph packetName
	CString GetGraphPacketName(const CString& strFunctionName) const;
	CString GetGraphPacketName_TitleName(const CString& strSubGraphTitle) const;
	CString GetGraphPacketName_TitleName(const CBlockIndex& blockIndex, const CString& strSubGraphTitle) const;
	// packet
	bool GetGraphPacketList(const CString& strFunctionName, CList<CPacket*, CPacket*>& packetList) const;
	// graph type
	bool GetGraphType(const CString& strFunctionName, const int nSubGraphIndex, CGraphBaseData::GRAPHTYPE& eGraphType, CGraphStyle& graphStyle, CGraphDrawStyle& graphDrawStyle) const;
	// graph style
	bool GetGraphStyle(const CString& strFunctionName, const int nSubGraphIndex, CGraphStyle& graphStyle, CGraphDrawStyle& graphDrawStyle) const;
	// subGraphData
	CSubGraphData* GetSubGraphData(const CString& strFunctionName, const CString& strSubGraphName) const;
	CSubGraphData* GetSubGraphData(const CString& strFunctionName, const int nSubGraphIndex) const;
	// subGraph names (bIsViewable = true : ���� ���̰� �ִ� subGraph)
	CString GetSubGraphNames(const CString& strFunctionName, const bool bIsViewable = true) const;
	// subGraph�� title
	bool GetAllSubGraphTitle(const CString& strFunctionName, CList<CString, CString>& titleList) const;
	CString GetSubGraphTitle(const CString& strFunctionName, const int nSubGraphIndex = 0) const;
	// GraphDataDrawingData
	bool GetGraphDataDrawingData(const CString& strFunctionName, 
		bool& bLowHigh, bool& bLowCur, bool& bHighCur, bool& bPrevCur, bool& bPrevCurRate);
	// ocx �ּ�
	CWnd* GetParentWnd() const;
	// packet dll �ּ�
	CPacketList* GetPacketList() const;
	// �Ӽ�dll �ּ�
	CIndicatorList* GetIndicatorList() const;
	// project ���
	CString GetProjectPath() const;

	// �ش� column�� graph ��
	int GetGraphRegionWidthOfBlocksColumn(CDC* pDC, const int blockColumnIndex = 0) const;
	// view data
	bool GetViewDataListInAllBlock(CViewData& viewData) const;
	// data min/max
	bool GetDisplayDataMinMax(const CString& functionName, double& dMin, double& dMax) const;
	bool GetDisplayDataMinMax(const CString& functionName, const CBlockIndex& blockIndex, double& dMin, double& dMax) const;
	bool GetDisplayDataMinMax(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, double& dMin, double& dMax) const;
	// AnalysisTool���� ��ǥ ����� ���� data.
	void GetCoordinateCalculatorData(const CPoint& point, CCoordinateCalculatorData& calData) const;
	void GetCoordinateCalculatorData(const CBlockIndex& blockIndex, CCoordinateCalculatorData& calData) const;


	// �ش� ��ǥ�� �ִ����� ����
	bool IsExistenceGraph(const CString& functionName) const;
	bool IsExistenceGraph(const CString& functionName, const CGraphBaseData::GRAPHTYPE eGraphType) const;
	bool IsExistenceGraph(const CString& functionName, const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex) const;
	bool IsExistenceGraphInBlock(const CString& functionName, const CBlockIndex& blockIndex) const;
	bool IsExistenceGraphInBlock(const CString& functionName, const CPoint& point) const;

	// �ش� graph Type�� �����ϴ��� üũ
	bool IsViewableGraphType(const CGraphBaseData::GRAPHTYPE eGraphType) const;


	// block�� �׸���
	void DrawBlocks(CDC* pDC);
	void DrawBlocks(CDC* pDC, const CDrawingOption::DRAWINGTYPE eDrawingType, const bool bIsCalDataForDrawing);

	// �ش� block ã��&��ġ ����.
	CBlockIndex FindCompartLine(const CPoint& point) const;
	bool MoveBlocksPosition(const CPoint& point, const CBlockIndex& compartLineIndex);

	// block region�� ã�´�.
	CBlockIndex FindBlockMovingRegion(const CPoint& point) const;
	bool MoveBlock(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex);
	CBlockIndex FindBlockDeletingRegion(const CPoint& point) const;

	// block ã��.
	CBlockIndex FindBlockNumber(const CPoint& point) const;
	CBlock* FindBlock(const CPoint& point) const;

	// subGraph �� ã��.
	CSubGraph* FindSubGraphTitle(CDC* pDC, const CPoint& point, CRect& titleRegion) const;
	bool FindSubGraph(const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque) const;
	bool FindSubGraph(const CPoint& point, CBlockIndex& blockIndex, CString& strSubGraphData, int& nSubGraphIndex, int& nDataIndex) const;

private:
	CMainBlockImplementation* m_pMainBlockImpl;
};

///////////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_MAINBLOCK_H__405226D5_F3E1_4335_BC7A_76FA68A1E0A7__INCLUDED_)
