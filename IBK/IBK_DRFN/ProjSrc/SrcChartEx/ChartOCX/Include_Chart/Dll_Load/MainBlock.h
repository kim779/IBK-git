// MainBlock.h: interface for the CMainBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINBLOCK_H__405226D5_F3E1_4335_BC7A_76FA68A1E0A7__INCLUDED_)
#define AFX_MAINBLOCK_H__405226D5_F3E1_4335_BC7A_76FA68A1E0A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "../BlockBaseData.h"

class CGraphRegionColor;
class CPacketList;
class CPacket;
class CIndicatorList;
class CBlock;
class CSubGraph;
class CScaleColor;
class CSelectedSubGraphDeque;
class CViewData;
class CCoordinateCalculatorData;
class CSignalData;
class CGraphColor;
class CSubGraphData;
class CXScaleManager;
class CPacketListManager;	// �������� ���� : �������� - ojtaso (20070111)
class _ICHARTCTRL_CTRL;
class CHorizontalScale;
interface IToolBarManager;
interface IChartCtrl;

class CMainBlock
{
public:
	virtual ~CMainBlock()	{};

public:
	virtual CBlockIndex FindBlockIndex(const CPoint& point) const	= 0;
	// block ã��.
	virtual CBlock* GetBlock( const CBlockIndex &blockIndex) const	= 0;
	// block�� ����
	virtual bool DeleteBlock(const CBlockIndex& blockIndex, const bool bAutoHorzScaleDelete = true)	= 0;
	virtual bool DeleteAllBlock()	= 0;
	// �� �ִ�ȭ �߰� - ojtaso (20080519)
	virtual void		SetMaximizedBlock( const CBlockIndex& maxBlockIndex) = 0;
	virtual CBlockIndex	GetMaximizedBlock( void) const = 0;
	// graph ����
	virtual bool DeleteGraph(const CString& strGraphName)	= 0;
	virtual BOOL DeleteAllIndicator( const CString& strIndicatorName)	= 0;
	virtual bool DeleteGraphWithSubGraphTitle( const CBlockIndex &blockIndex, const CString& strTitleName, const bool bIsOnlyNoTRData = true)	= 0;
	virtual BOOL RemoveAllGraphOfRQ( LPVOID lpPacketRQ) = 0;	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	// RQ ���� �׷��� ���� �޼ҵ� �߰� - ojtaso (20080604)
	virtual bool DeleteGraphWithSubGraphTitleRQ(LPCTSTR lpszRQ, const CBlockIndex &blockIndex, const CString& strTitleName, const bool bIsOnlyNoTRData = true)	= 0;


//	bool DeleteGraph_PacketNames(const CString& strGraphPacketNames)	= 0;
	// graph �߰�
	virtual BOOL InsertGraph(const CString& graphData, const CGraphBaseData::ObjectChange blockType, const bool doesMaintainOldScale)	= 0;
	// RQ�� �˻� : �������� - ojtaso (20070118)
	virtual BOOL InsertGraph(LPVOID pPacketRQ, const CString& graphData, const CGraphBaseData::ObjectChange blockType, const bool doesMaintainOldScale)	= 0;
	virtual BOOL InsertGraphInBlock(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const CString& graphData, 
		const bool doesInsertInNewBlock = false, const bool doesInsertInNewVertScaleGroup = false, const bool doesMaintainOldScale = true)	= 0;
	// RQ�� ������ �ִ� Graph ���� : �������� - ojtaso (20061224)
	// CPacketRQ*�� ���� : �������� - ojtaso (20070109)
	virtual BOOL InsertGraphInBlock(LPVOID pPacketRQ, const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const CString& graphData, 
		const bool doesInsertInNewBlock = false, const bool doesInsertInNewVertScaleGroup = false, const bool doesMaintainOldScale = true)	= 0;
//	bool InsertGraph(const CPoint& insertPosition, const CString& graphData, const CGraphBaseData::ObjectChange blockType, const bool doesMaintainOldScale)	= 0;
	// vertScaleGroup �ش� block�� �̵�
	virtual bool MoveAllVertScaleGroupsToHeadBlock(const bool bDoesMaintainOldScale = true)	= 0;
	virtual bool MoveAllVertScaleGroupsToEachRowBlock(const CBlockIndex& targetBlockIndex, const bool bDoesMaintainOldScale = true)	= 0;
	virtual bool MoveAllVertScaleGroups(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex)	= 0;
//	bool MoveVertScaleGroup(const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const CBlockIndex& targetBlockIndex)	= 0;
	// vertScaleGroup�� join
	virtual bool JoinAllVertScaleGroups(const CBlockIndex& blockIndex, const bool bIsJoin = true)	= 0;
	// graph �̵�.
	virtual bool MoveAllGraphs(const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const CBlockIndex& targetBlockIndex, const int nTargetGroupIndex = -1)	= 0;
	virtual bool MoveGraph(const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const int nSourceGraphIndex, const CBlockIndex& targetBlockIndex, const int nTargetGroupIndex = -1)	= 0;
	virtual bool MoveGraph2(const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const int nSourceGraphIndex, const CBlockIndex& targetBlockIndex, const int nTargetGroupIndex = -1, const BOOL bIsAddTail = TRUE)	= 0;
	// graph ����.
	// -> GraphTitle�� target ������ �����Ѵ�.
//	bool ChangeGraph(const CBlockIndex& blockIndex, const CString& targetGraphTitle, const CString& sourceGraphData)	= 0;
//	bool ChangeGraph(const CString& targetGraphName, const CString& sourceGraphData)	= 0;
//	bool ChangeGraph(const CString& targetGraphName, const CIndicatorConfigurationChangedData& changedData)	= 0;
	// graph GraphName/subGraphName �� ����. (�ش� graph�� ��� ����, subGraphCount�� 1���� ����.)
//	bool ChangeAllGraphNameAndSubGraphName(const CString& targetGraphName, 
//		const CString& sourceGraphName, const CString& sourceSubGraphName)	= 0;
	// graph GraphName/subGraphName �� ����. -> cfg�� �Լ����� �ٲ�� ��� �����
//	bool ChangeAllGraphNameAndSubGraphNames(const CString& targetGraphName, const CString& sourceGraphName, const bool bIsSubGraphCheck = true)	= 0;
//	bool ChangeGraphNameAndSubGraphNames(const CString& targetGraphName, const CString& sourceGraphName, const bool bIsSubGraphCheck = true)	= 0;
//	bool ChangeGraphNameAndSubGraphNames(const CBlockIndex& blockIndex, const CString& targetGraphName, const CString& sourceGraphName, const bool bIsSubGraphCheck = true)	= 0;
	// Change Input Packet Names and Calculate again.
	virtual BOOL	ChangeIndicatorPacketNames( const char *p_szIndicatorName, const int p_nSubGraphIndex, const char *p_szPacketNames)	= 0;
	// graph Data ����.
//	bool ChangeGraphData(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, 
//		const CString& sourceGraphName, const CString& sourcePacketNames, const CString& sourceSubGraphTitle, 
//		const CGraphBaseData::GRAPHTYPE& sourceType, const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, 
//		const int nPenThickness1, const int nPenThickness2, const int nPenThickness3)	= 0;
	virtual bool ChangeGraphData(const CString& targetGraphName, const int nSubGraphIndex, const CString& sourcePacketNames, const CString& sourceSubGraphTitle, 
		const CGraphBaseData::GRAPHTYPE& sourceType, const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, 
		const int nPenThickness1, const int nPenThickness2, const int nPenThickness3)	= 0;
	// GraphDataDrawingData ����.
	virtual bool ChangeGraphDataDrawingData(const CString& p_strIndicatorName, const bool bLowHigh, const bool bLowCur, const bool bHighCur, const bool bPrevCur, const int nCurData)	= 0;
	// GraphDataDraw ���� �߰� - ojtaso (20080609)
	virtual bool ShowGraphDataDrawingData(const CString& strRQ, const CString& p_strIndicatorName, const bool bShow, const int nShowType) = 0;
	// ViewMinMaxDrawingData ����.
//	virtual bool ChangeViewMinMaxDrawingData(const CString& targetGraphName, 
//		const bool bIsMinDrawing, const bool bIsMaxDrawing, const bool bIsSignDrawing, const int nTextColorType)	= 0;
	// graph type ����.
	virtual bool ChangeGraphType(const CString& targetGraphName, const CGraphBaseData::GRAPHTYPE& sourceType, 
		const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, const CString& sourcePacketNames)	= 0;
	// subGraph data ����.
	virtual bool ChangeSubGraphData(const CString& targetGraphName, const CString& targetSubGraphName, const CSubGraphData& sourceSubGraphData)	= 0;
	// subGraph title ����.
	virtual bool ChangeSubGraphTitle(const CString& orgSubGraphTitle, const CString& changeSubGraphTitle)	= 0;
	virtual bool ChangeSubGraphTitle(const CBlockIndex& blockIndex, const int nScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const CString& changeSubGraphTitle)	= 0;
	virtual bool ChangeSubGraphTitle(const CString& strGraphName, const int nSubGraphIndex, const CString& changeSubGraphTitle)	= 0;
	virtual bool ChangeSubGraphTitleFromIndicator(const CString& strGraphName, const int nSubGraphIndex)	= 0;
	// subGraph color ����.
	virtual bool ChangeGraphColor1(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const COLORREF& newGraphColor1)	= 0;
	// �ش� block�� graph���� ����.
//	bool ModifyGraphDataInBlock(const CString& graphsData, const CBlockIndex& blockIndex, const bool doesMaintainOldScale = true)	= 0;
	// �ش� graph�� indicatorInfo ���� ����.
	// -> gaphData(subGraph����)�� IndicatorInfo�� ����.
	//    (��ǥ���� ���õ� ������ ����� ��� ��ǥ��� ����)
//	bool ChangeAllGraphDataFromIndicatorInfo(const bool bIsCalculate)	= 0;
	virtual bool ChangeGraphDataFromIndicatorInfo(const CString& strGraphName, const bool bIsCalculate)	= 0;
	// -> IndicatorInfo�� gaphData(subGraph����)�� ����.
	virtual bool ChangeAllIndicatorInfoFromGraphData()	= 0;
	virtual BOOL ChangeAllGraphDataFromIndicatorInfo( BOOL p_bWithCalculate)	= 0;
	virtual bool ChangeIndicatorInfoFromGraphData(const CString& strGraphName)	= 0;
	// -> �ش� graph�� Map Default IndicatorInfo�� ����
	virtual bool ChangeAllMapDefaultIndicatorInfo( void)	= 0;
//	bool ChangeMapDefaultIndicatorInfo(const CString& strGraphName, const bool bIsAutoMake = true)	= 0;

	// object�� �⺻���� set. -- block, graph, scale, data Set.
	// block�� ������ ��� ���� ����� --> ó������ block�� ���� ���
	virtual bool SetBlockEnvironment(CDC* pDC, const CRect& allBlockRegion, const CBlockBaseData::BLOCKTYPE blockType, const CBlockBaseData::BLOCKCOMPART blockCompart, 
			const CBlockIndex& blockCount, const int graphRegionFrameThickness, const int interBlockHalfGap, const CGraphRegionColor& graphRegionColor)	= 0;
	virtual bool SetAllBlockRegion(const CRect& allBlockRegion)	= 0;
	virtual void SetBlockCount(const CBlockIndex& blockCount)	= 0;
	virtual void SetGraphTitleDraw(const bool bIsCondition, const bool bIsData)	= 0;
	virtual void SetBackgroundColor(const COLORREF& color)	= 0;
	virtual void SetGraphRegionColor(const CGraphRegionColor& graphRegionColor)	= 0;
	virtual void SetEtcGraphColor(const COLORREF& graphColor1, const COLORREF& graphColor2, const COLORREF& graphColor3, const COLORREF& graphColor4, const COLORREF& graphColor5, const COLORREF& graphColor6)	= 0;

	// block�� ����(����block + gap + ���� ����)�� ���ϴ� ������ ���� 
	// --> block�� �̹� ������� ��
	virtual void SetBlocksPosition(const CString& blocksPosition)	= 0;
	// �ٸ� block size���� 2���� block ����
	virtual void SetDoubleSizeBlockNumber(const CBlockIndex& doubleSizeBlockIndex)	= 0;
	// Tool�� ���õ� ���� ���� (block�̵�, block����)

	// block�� Graph���� �����Ѵ�.
	// �׷��� ������ ��� ���� : xScaleManager - ojtaso (20070701)
	virtual BOOL BuildGraphsAndScales(const CString& data, BOOL p_bWithIndicatorInfoUpdate = TRUE)	= 0;

	// scale�� data ����
	virtual void SetAllHorzScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType)	= 0;
	virtual void SetAllVertScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType)	= 0;
	virtual void SetScaleColor(const COLORREF& horzGridColor, const COLORREF& vertGridColor, const COLORREF& textColor)	= 0;
	virtual bool SetHorzScaleData(const CBlockIndex& blockIndex, const CString& strScaleData)	= 0;
	// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
	//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
	virtual void SetHorzScalePosition( CHorizontalScale &p_horizontalScale, const CScaleBaseData::HORZSCALEPOSITION ePosition) = 0;
	virtual bool SetVertScaleData(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const CString& strScaleData)	= 0;
	// ȣ������, ���ذ� ��� - ojtaso (20070917)
	virtual void SetVertScaleType(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nScaleType, const double dTypeValue) = 0;
	virtual void SetHorzScaleCompart(const CBlockIndex& blockIndex, const CString& dateCompart, const CString& timeCompart, const CScaleBaseData::TEXTCOMPARTTYPE eTextCompart = CScaleBaseData::HORZ_TEXT_COMPART)	= 0;
	virtual bool SetHorzScalePosition(const CBlockIndex& blockIndex, const CScaleBaseData::HORZSCALEPOSITION ePosition)	= 0;
	virtual void SetAllHorzScalePosition(const CScaleBaseData::HORZSCALEPOSITION ePosition)	= 0;
	virtual void SetBaseHorzScalePosition()	= 0;
	virtual bool SetVertScalePosition(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const CScaleBaseData::VERTSCALEPOSITION ePosition)	= 0;
	virtual void SetAllVertScalePosition(const CScaleBaseData::VERTSCALEPOSITION ePosition)	= 0;
	virtual void SetOnlyPriceVertScale(CDC* pDC, const bool bIsOnlyPrice)	= 0;
	// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
	virtual BOOL SetDrawBaseLine(BOOL bDrawBaseLine) = 0;
	virtual bool GetDrawBaseLine() = 0;

	// ȭ���� ���� design mode���� ����.
	virtual void SetDesignMode(const bool designMode = true)	= 0;
	// ȭ�鿡 �߰��� �� �ִ� �� block ��
	virtual void SetMaxBlocksCountInView(const int maxBlockCount)	= 0;
	// messagebox ���� ����
	virtual void SetMessageBoxShow(const bool bIsShow)	= 0;
	// graph background gap ����
	virtual void SetGraphBackgroundGap(const int nGap_x, const int nGap_y)	= 0;

	// ocx �ּ�
	virtual void			SetIChartCtrl( IChartCtrl* p_pICharetCtrl)	= 0;
	virtual IChartCtrl *	GetIChartCtrl( void)	= 0;

	// packet�� ���� ����
	virtual void SetPacketList( CPacketList* pPacketList, const char *p_szRQ)	= 0;
	// OCX ���
	virtual void SetOCXPath(const CString& strOCXPath)	= 0;
	// �Ӽ��� ���� ����
	virtual void SetIndicatorList(CIndicatorList* pIndicatorList)	= 0;
	// X-Scale Manager
	virtual void SetXScaleManager(CXScaleManager* pxScaleManager)	= 0;
	// �������� ���� : �������� - ojtaso (20070111)
	virtual void SetPacketListManager(CPacketListManager* pPacketListManager)	= 0;
	virtual void SetToolBarManager(IToolBarManager* pToolManager)	= 0;

	// graph ������ �ش� graph�� indicatorInfo�� �����Ѵ�.
	virtual void SetAutoDeleteIndicatorInfo(const bool bIsAutoDelete)	= 0;
	
	// graph�� indicatorInfo �ʱ�ȭ
	virtual void Initialize_IndicatorInfo(const bool bIsNULL)	= 0;
	// graph(subGraph��)�� packet���� �ʱ�ȭ(bIsCalculate = true �ٽ� �����)
	virtual bool Initialize_AllPackets(const CString& strGraphName, const bool bIsCalculate)	= 0;
	virtual bool Initialize_Packet(const CString& strPacketName, const bool bIsCalculate)	= 0;
	// ��ǥ�� ����Ѵ�.
	// ����ؾ� �ϴ� ��� : TR, �ǽð�, ���ǰ�, �Լ���, packetname, ��ũ��, ����
	virtual void CalculateAllGraphs(const CCalculateBaseData::CALCULATETYPE eCalType = CCalculateBaseData::TRDATA_CAL)	= 0;
	virtual bool CalculateAllGraphs(const CList<CString, CString>& packetNameList, const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsHeadGraph)	= 0;
	virtual bool CalculateGraph(const CString& strGraphName, const CCalculateBaseData::CALCULATETYPE eCalType = CCalculateBaseData::TRDATA_CAL, const bool bIsClearPacketData = false)	= 0;

	// block�� ���� ���� �ٲٱ�.
//	virtual bool SetBlockTypeNotChangeBlockData(const CBlockBaseData::BLOCKTYPE blockType, const CBlockIndex& blockCount)	= 0;

	// subGraph�� ���̱� ���� setting.
	virtual void SetAllGraphsHiding(const CBlockIndex& blockIndex, const bool bIsHiding = true)	= 0;
	virtual bool SetSubGraphHiding(const CString& strGraphName, const int nSubGraphIndex, const bool bIsHiding = true)	= 0;
	virtual bool SetSubGraphHiding(const CBlockIndex& blockIndex, const CString& subGraphTitle, const bool bIsHiding = true)	= 0;
	virtual bool SetSubGraphHiding(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const bool bIsHiding = true)	= 0;
	// subGraph title ����� ���� setting.
	virtual void SetAllGraphsTitleHiding(const bool bIsAllHiding = true)	= 0;
	// block�� ���� scale�� log/invert type setting.
	virtual bool SetVertScaleLogAndInvertType(const CString& strGraphName, const bool bIsLogType, const bool bIsInvertType)	= 0;
	// block�� ���� scale�� log type setting.
	virtual bool SetAllVertScaleLogType(const bool bIsLogType)	= 0;
	virtual bool SetVertScaleLogType(const CBlockIndex& blockIndex, const bool bIsLogType)	= 0;
	virtual bool SetVertScaleLogType(const CString& strGraphName, const bool bIsLogType)	= 0;
	// block�� ���� scale�� invert type setting.
	virtual bool SetAllVertScaleInvertType(const CBlockIndex& blockIndex, const bool bIsInvertType)	= 0;


	// block�� ���� data ��������
	// ���� main block�� data�� �ִ����� �����´�.
	virtual bool IsEmpty() const	= 0;
	// ���� ȭ�鿡 ���� Mode�� �����´�.
	virtual bool GetDesignMode() const	= 0;
	// region
	virtual CRect GetAllBlockRegion() const	= 0;
	virtual CRect GetBlockRegion(const CBlockIndex& blockIndex) const	= 0;
	virtual CRect GetGraphRegionSumInColumn(const CPoint& point) const	= 0;
	virtual CRect GetNoFrameGraphRegion(const CBlockIndex& blockIndex) const	= 0;	
	virtual CRect GetNoFrameGraphRegion(const CPoint& point) const	= 0;
	// Graph �и�/���� - ojtaso (20070731)
	virtual CRect GetNoFrameGraphRegion(const CPoint& point, int& nInsertCol, int& nInsertRow) const	= 0;
	virtual CRect GetDrawingGraphRegion(const CBlockIndex& blockIndex) const	= 0;
	// type
	virtual CBlockBaseData::BLOCKTYPE GetBlockType() const	= 0;
	virtual CBlockBaseData::BLOCKCOMPART GetBlockCompartType() const	= 0;
	// block�� ����
	virtual int GetRowCount() const	= 0;
	virtual int GetRowCountInColumn( int p_nColumnIndex) const	= 0;
	virtual int GetColumnCount() const	= 0;
	// graph�� ����
	// ȭ�鿡 ���̴� data ����/��
	virtual void GetDisplayDataIndex(int& nDataStartInBlock, int& nDataEndInBlock) const	= 0;
	// graph ���� �׵θ� �β�
	virtual int GetGraphRegionFrameThickness() const	= 0;
	// block ���� gap/2
	virtual int GetInterBlockHalfGap() const	= 0;
	// �� ȭ�鿡 ���� �ִ� block ����
	virtual int GetMaxBlocksCountInView() const	= 0;
	// block�� ���� ��������
	virtual CString GetBlocksPosition() const	= 0;
	// ��� block�� data
	virtual void GetAllBlocksData( CString &p_strAllBlockData) const	= 0;
	// ������ �ִ� graph GraphName
	virtual CString GetIndicatorName(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex = 0, const int nGraphIndex = 0) const	= 0;
	virtual bool GetAllIndicatorName(CList<CString, CString>& strIndicatorNameList) const	= 0;
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	virtual bool GetAllIndicatorAndGraphNameWithRQ(CList<CString, CString>& strIndicatorNameList, CList<CString, CString>& strGraphNameList, const char *p_szRQ) = 0;
	virtual bool GetAllGraphName(CList<CString, CString>& strGraphNameList) const	= 0;
	// �ش� graph �� �ִ� block�� ��� ��ǥ�� �Լ��� �������� (�Լ��� ������ "	= 0;" �� �Ѵ�.)
	// 2�� ũ�� block
	virtual CBlockIndex GetDoubleSizeBlockNumber() const	= 0;
	// �ش� graph GraphName�� �ִ� block index
	// block data�� ���� block index
//	CBlockIndex GetBlockIndex_SameBlockData(const CString& graphData, const bool doesMaintainOldScale = true) const	= 0;

	
	// vert scale type
	virtual CScaleBaseData::SCALEGRIDTYPE	GetFirstBlockHGridLineType( void)	= 0;
	virtual CScaleBaseData::SCALEGRIDTYPE	GetFirstBlockVGridLineType( void)	= 0;
	// scale position
	virtual CScaleBaseData::VERTSCALEPOSITION GetVertScalePosition(const CBlockIndex& blockIndex = CBlockIndex(0, 0), const int nVertScaleGroupIndex = 0) const	= 0;
	virtual CScaleBaseData::HORZSCALEPOSITION GetHorzScalePosition(const CBlockIndex& blockIndex = CBlockIndex(0, 0)) const	= 0;
	// scale ��
	virtual CScaleColor& GetScaleColor()	= 0;
	virtual void GetScaleColor(COLORREF& horzGridColor, COLORREF& vertGridColor, COLORREF& textColor) const	= 0;
	// scale data
	virtual CString GetHorzScaleData(const CBlockIndex& blockIndex) const	= 0;
	// ��ġ��ȸâ�� �ڷ����� Format, X Scale �������� ���� ó�� - ojtaso (20070827)
	virtual void GetHorzScaleCompart( const CBlockIndex& blockIndex, CString& strCompartDate, CString& strCompartTime) = 0;
	// graph region�� ���� ���� �����´�.
	virtual COLORREF GetBackgroundColor() const	= 0;
	virtual CGraphRegionColor GetGraphRegionColor() const	= 0;
	virtual void GetGraphRegionColor(COLORREF& backgroundColor, COLORREF& lineColor, COLORREF& frameColor) const	= 0;
	// graph ��
	virtual CGraphColor* GetGraphColor(const CString& strGraphName, const int nSubGraphIndex = 0) const	= 0;
	virtual void GetEtcGraphColor(COLORREF& graphColor1, COLORREF& graphColor2, COLORREF& graphColor3, COLORREF& graphColor4, COLORREF& graphColor5, COLORREF& graphColor6) const	= 0;
	// �̷����� �������� �׸������� : ojtaso (20070724)
	virtual BOOL GetDrawFutureScale() const = 0;
	virtual void SetDrawFutureScale(const BOOL bDrawFutureScale) = 0;
	// ��ƮDLL���� ���������� ������� : �������� - ojtaso (20070904)
	virtual void SetMultiItemMode(const BOOL bMultiItem) = 0;
	virtual BOOL GetMultiItemMode() const = 0;
	// graph packetName
	virtual CString GetGraphPacketName(const CString& strGraphName) const	= 0;
	virtual CString GetGraphPacketName_TitleName(const CString& strSubGraphTitle) const	= 0;
	virtual CString GetGraphPacketName_TitleName(const CBlockIndex& blockIndex, const CString& strSubGraphTitle) const	= 0;
	// packet
	// graph type
	virtual bool GetGraphType(const CString& strGraphName, const int nSubGraphIndex, CGraphBaseData::GRAPHTYPE& eGraphType, CGraphStyle& graphStyle, CGraphDrawStyle& graphDrawStyle) const	= 0;
	// graph style
	// subGraphData
	virtual CSubGraphData* GetSubGraphData(const CString& strGraphName, const CString& strSubGraphName) const	= 0;
	// subGraph names (bIsViewable = true : ���� ���̰� �ִ� subGraph)
	virtual CString GetSubGraphNames(const CString& strGraphName, const bool bIsViewable = true) const	= 0;
	// subGraph�� title
	virtual bool GetAllSubGraphTitle(const CString& strGraphName, CList<CString, CString>& titleList) const	= 0;
	// ViewMinMaxDrawingData ����.
//	bool GetViewMinMaxDrawingData(const CString& strGraphName, bool& bIsMinDrawing, bool& bIsMaxDrawing, bool& bIsSignDrawing, bool& nTextColorType)	= 0;

	// packet dll �ּ�
	virtual CPacketList* GetPacketList() const	= 0;
	// �Ӽ�dll �ּ�
	virtual CIndicatorList* GetIndicatorList() const	= 0;
	// X-Scale Manager
	virtual CXScaleManager* GetXScaleManager() const	= 0;
	// �������� ���� : �������� - ojtaso (20070111)
	virtual CPacketListManager* GetPacketListManager() const	= 0;
	// IToolBarManager
	virtual IToolBarManager* GetToolBarManager() const	= 0;

	// OCX ���
	virtual CString GetOCXPath() const	= 0;
	// �ش� RQ�� �۾� : �������� - ojtaso (20070420)
	virtual bool GetViewDataListInAllBlock(LPCTSTR lpszRQ, CViewData& viewData) const	= 0;
	// data min/max
	virtual bool GetDisplayDataMinMax(const CString& GraphName, double& dMin, double& dMax) const	= 0;

	// �ش� ��ǥ�� �ִ����� ����
	virtual BOOL IsExistenceGraph( const CString &strGraphName) const	= 0;
	virtual BOOL IsExistenceGraphInBlock( const CString& GraphName, const CBlockIndex& blockIndex) const	= 0;
	virtual BOOL IsExistenceIndicator( const CString &strIndicatorName) const	= 0;
	// �������� �˻� : �������� - ojtaso (20070502)
	virtual BOOL IsExistenceIndicator(LPVOID pPacketRQ, const CString &strIndicatorName) const	= 0;
	virtual BOOL IsExistenceIndicatorInBlock( const CString &strIndicatorName, const CBlockIndex& blockIndex) const	= 0;
	// RQ�� ��ǥ�˻� : �������� - ojtaso (20070109)
	virtual BOOL IsExistenceIndicatorInBlock(LPCTSTR lpszRQ, const CString &strIndicatorName, const CBlockIndex& blockIndex) const	= 0;
	virtual BOOL IsExistenceIndicatorInBlock( const CString &strIndicatorName, const CPoint& point) const	= 0;

	// block�� �׸���
	virtual void CalculateDataForDrawing( CDC* pDC) = 0;
	virtual void DrawBlocks(CDC* pDC, const CDrawingOption::DRAWINGTYPE eDrawingType, const bool bIsCalDataForDrawing)	= 0;
	virtual void ChangeDrawStyle(int nDrawStyle) = 0;
	// �ش� block ã��&��ġ ����.
	virtual CBlockIndex FindCompartLine(const CPoint& point) const	= 0;
	virtual bool MoveBlocksPosition(const CPoint& point, const CBlockIndex& compartLineIndex)	= 0;

	// block region�� ã�´�.
	virtual CBlockIndex FindBlockMovingRegion(const CPoint& point) const	= 0;
	virtual bool MoveBlock(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex)	= 0;
	virtual CBlockIndex FindBlockDeletingRegion(const CPoint& point) const	= 0;
	// �� �ִ�ȭ �߰� - ojtaso (20080519)
	virtual CBlockIndex FindBlockSizingRegion(const CPoint& point) const	= 0;

	// subGraph �� ã��.
	virtual CSubGraph *	FindSubGraphTitle(CDC* pDC, const CPoint& point, CRect& titleRegion) const	= 0;
	virtual bool		FindSubGraph(const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque) const	= 0;
	virtual CString		FindGraph( const CPoint &p_pt) const	= 0;
	// (2006/6/25 - Seung-Won, Bae) Get Index of Graph
	virtual BOOL	GetIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG)	= 0;

	// ( 2006 / 09 / 26 - Sang-Woo, Cho )
	// ������Ʈ���� �ְ�, ������ Text�� ���̰� �� ������ �� �������� ���� ���� �����Ѵ�.
	virtual BOOL	GetShowPriceMinMaxText( void )	= 0;					
	virtual void	SetShowPriceMinMaxText( BOOL bShowPriceMinMaxText )	= 0;

	// (2006/10/13 - Seung-Won, Bae) Manage Block Move & Delete Tool Flag for Position of Graph Drawing
	virtual void	SetBlockMoveDeleteFlag( BOOL p_bBlockMoveDelete)	= 0;

	// (2006/11/12 - Seung-Won, Bae) Support VScale Min/Max Type Change for All VSCale
	virtual void	SetAllVScaleMinMaxType( CScaleBaseData::VERTMINMAXTYPE p_eVScaleMinMaxType, CDC* pDC)	= 0;
	// (2006/11/14 - Seung-Won, Bae) Support VScale Min/Max Type Release in TR
	virtual void	ReleaseAllVScaleSettingInFixedMinMax( CDC *pDC)	= 0;

	// VertScaleGroup�� ���� ó�� Graph�� Ÿ�� : �������� - ojtaso (20070504)
	virtual CScaleBaseData::HORZSCALEDRAWERTYPE GetMasterValueType(const int nRow, const int nCol)	= 0;
	virtual LPCTSTR GetMasterValueRQ(const int nRow, const int nCol)	= 0;
	// 20081007 JS.Kim	����ڽð��� ����
	virtual CString		GetCurrentRQ( void) = 0;

// (2006/12/12 - Seung-Won, Bae) Support Indicator Calculation Enabling Flag
public:
	virtual void	EnableIndicatorCalculation( BOOL p_bEnableIndicatorCalculation)	= 0;
	virtual BOOL	IsEnableIndicatorCalculation( void) const	= 0;

// (2007/1/20 - Seung-Won, Bae) Search Indicator
public:
	virtual BOOL	FindFirstIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, CString &p_strGraphName, const char *p_szRQ)	= 0;
	virtual BOOL	FindNextIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, CString &p_strGraphName, const char *p_szRQ)	= 0;

// (2007/2/20 - Seung-Won, Bae) Support Indicator Hiding
//		It can not be replaced with AddChartBlock(), because of Condition Reset.
public:
	virtual void	ShowIndicator( const char *p_szIndicatorName, BOOL p_bShow, BOOL p_bRecalculate)	= 0;

// (2007/2/22 - Seung-Won, Bae) Support Base Text Option
public:
	virtual void	ShowBaseText( BOOL p_bShow)	= 0;

// (2007/2/22 - Seung-Won, Bae) Support Graph Color Changing with Graph Title
public:
	virtual BOOL	ChangeGraphColor( const char *p_szGraphTitle, const char *p_szColors)	= 0;

// (2007/3/28 - Seung-Won, Bae) Retrieve Packet Name of SubGraph
public:
	virtual CString		GetSubGraphPacketName( const char *p_szGraphName, const char *p_szSubGraphName, const int p_nPacketIndex)	= 0;

// (2007/4/30 - Seung-Won, Bae) Get Data Index of Inverse Line Graph
public:
	virtual int			GetDataIndexOfInverseLine( CPoint p_pt)	= 0;

// (2007/5/23 - Seung-Won, Bae) Support for Only IChartCtrl::SetStartEndIndex in OCX.
//		Use IChartCtrl::SetStartEndIndex() for Invalidation and Notify Zoom Value.
protected:
friend class _ICHARTCTRL_CTRL;
	// ȭ�鿡 ���� data�� ����.
	virtual void	SetDataRegionInBlock2( const int start, const int end)	= 0;

// (2007/7/18 - Seung-Won, Bae) Set Up/Down Space for Spacial Chart.
public:
	virtual BOOL	GetVertScaleSpace( const int p_nR, const int p_nC, const int p_nV, int &p_nUpSpace, int &p_nDownSpace)	= 0;
	virtual BOOL	SetVertScaleSpace( const int p_nR, const int p_nC, const int p_nV, const int p_nUpSpace, const int p_nDownSpace)	= 0;

	// ���� VertScaleGroupCount�� ���� - ojtaso (20080618)
	virtual long	GetVertScaleGroupCount(const CBlockIndex& blockIndex) = 0;
	// �ѹ��� �׷��� �Ӽ� ���� ������ �޽�� �߰� - ojtaso (20080602)
	virtual void	ChangeGraphPropertyAtOnce(LPCTSTR lpszRQ, LPCTSTR lpszTargetTitle, LPCTSTR lpszNewTitle, short nGraphType, short nGraphStyle, short nDrawStyle, LPCTSTR lpszColors, short nThickness1, short nThickness2, short nThickness3) = 0;

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
public:
	virtual HWND	GetParent_Handle( void)								= 0;

// (2009/9/12 - Seung-Won, Bae) for Recalculation of Block Position.
public:
	virtual void	SetHorizontalScaleHeight( short nBlockRow, short nBlockCol, long nHeight) = 0;

// (2009/9/27 - Seung-Won, Bae) Get RQ of Graph.
public:
	virtual const char *	GetRQofGraph( const char *p_szGraphName)	= 0;

	// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
	virtual long	AddHorizontalLine(LPCSTR lpszRQ, LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice) = 0;
	virtual void	DeleteHorizontalLine(LPCSTR lpszRQ, long nIndex) = 0;
	virtual void	DeleteAllHorizontalLine(LPCSTR lpszRQ) = 0;
	virtual void	UpdateHorizontalLine(LPCSTR lpszRQ, long nIndex, LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice) = 0;

	// �ѹ��� �׷��� �Ӽ� ���� �� �޽�� �߰� - (@Solomon-20100120)
	virtual bool	GetGraphPropertyAtOnce(LPCTSTR lpszRQ, LPCTSTR lpszTargetTitle, short &nGraphType, short &nGraphStyle, short &nDrawStyle, COLORREF &crColors, short &nThickness1, short &nThickness2, short &nThickness3) = 0;

//20100518_�ַθ�  �ش� ��ǥ�� SubGraphIndex�� Hide �Ӽ��� Get
	virtual BOOL GetSubGraphHiding( const CString& strGraphName, const int nSubGraphIndex) = 0;

	//	�ð����м�
	virtual CScaleBaseData::SCALEGRIDTYPE GetFirstDailyScaleGridType( CScaleBaseData::SCALEGRIDTIMEGUBUN nType) = 0;
	virtual BOOL GetFirstDailyScaleTime( CScaleBaseData::SCALEGRIDTIMEGUBUN nType) = 0;
	virtual const unsigned short GetFirstBlockGridLineWidth_TimeGubun( int) = 0;

	virtual void SetAllDailyScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType, const CScaleBaseData::SCALEGRIDTIMEGUBUN eTimeType, BOOL p_bCheckTime) = 0;
	virtual void SetAllScaleGridWidth_TimeGubun( CScaleBaseData::SCALEGRIDTIMEGUBUN nType, const unsigned short nGridWidth) = 0;

	virtual const unsigned long GetFirstBlockGridLineColor_TimeGubun( int) = 0;
	virtual void SetAllScaleGridColor_TimeGubun( CScaleBaseData::SCALEGRIDTIMEGUBUN nType, const unsigned long nGridColor) = 0;
};

typedef CMainBlock *(*FN_CreateMainBlock)( void); 

///////////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_MAINBLOCK_H__405226D5_F3E1_4335_BC7A_76FA68A1E0A7__INCLUDED_)
