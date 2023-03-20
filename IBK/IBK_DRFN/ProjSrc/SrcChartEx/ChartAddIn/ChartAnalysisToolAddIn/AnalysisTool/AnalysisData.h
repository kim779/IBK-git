// AnalysisData.h: interface for the CAnalysisData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANALYSISDATA_H__73E71018_E550_4315_B600_ED567CB93E72__INCLUDED_)
#define AFX_ANALYSISDATA_H__73E71018_E550_4315_B600_ED567CB93E72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/CoordinateCalculatorData.h"
#include "../Include_Analysis/BlockRegion.h"
#include "../Include_Analysis/ElementEnvironment.h"

#include "Coordinate.h"
#include "ElementTree.h"
#include "UndoRedoManager.h"

class CDisplayAttributes;
class CCoordinateCalculatorData;

// ----------------------------------------------------------------------------
// �м��������� ���Ǵ� �⺻ data
class CAnalysisData
{
public:
	CAnalysisData( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);
	~CAnalysisData();

	void SetParent(CWnd* pWnd);
	void SetMainBlock(IChartManager* pMainBlock, IChartOCX *p_pIChartOCX);
	void SetCurrentToolType(const CAnalysis::TOOLTYPE eToolType);

	CWnd* GetParent() const;
	IChartManager* GetMainBlock() const;
	IPacketManager* GetPacketManager() const;
	IChartOCX* GetChartOCX() const;

	CAnalysis::TOOLTYPE GetCurrentToolType() const;
	const CElementTree& GetElementTree() const;
	CElementTree& GetElementTree();

	CString GetOCXPath() const;
	CString GetUserPath() const;
	
	void RegisterElementToOrderManager(long lElementAdress);
	void UnRegisterElementToOrderManager(long lElementAdress);
	CDisplayAttributes GetDisplayAttributes() const;
	bool GetVertScaleRegion(CRect& rcLeft,CRect& rcRight);
	bool GetHorzScaleRegion(CRect& rcTop,CRect& rcBottom);
	bool GetVertScaleType(const CPoint& point, bool& bIsLog, bool& bIsInvert) const;
	CCoordinateCalculatorData GetCoordinateCalculatorData(const CPoint& point) const;
	CBlockIndex GetBlockIndexInPoint(const CPoint& point) const;
	CBlockIndex GetBlockIndexInElement(long lElementAddress) const;
	bool PointInGraphRegion(const CPoint& point) const;

	// ocx�� �޽��� ������.
	void SendMessageToParent(const CCursorShape::CHARTOBJECT cursorShape);
	void InvalidateAndSendMessageToParent(const CCursorShape::CHARTOBJECT cursorShape);
	void Invalidate();
	// element �׸���.
	void DrawElement(CDC* pDC, CElement* pSelectedElement);
	CElement *CopyElement(CElement* pSelectedElement);

	// block �߰��� elementTree�� �߰��ϱ�.
	bool InsertElementTree(const CBlockIndex& insertBlockIndex);
	
	// UnDo / Redo 
	const CUndoRedoManager& GetUndoRedoManager() const;
	CUndoRedoManager& GetUndoRedoManager();
	BOOL MoveElement(CElement* pSelectedElement, CString strElementData_Prev = "");

private:
	CWnd* m_pParent;	// ocx �ڵ�.
	CAnalysis::TOOLTYPE m_eCurrentToolType; // ���� ���ǰ� �ִ� Tool Type.
	CElementTree m_elementTree;				// tool�� ���� ������ ��� element.
	CUndoRedoManager m_UndoRedoManager;		// Undo / Redo

public:
	static CString		m_strAddInItemName;
protected:
	IPacketManager  *m_pIDefaultPacketManager;
	IChartManager	*m_pIChartManager;
	IChartOCX		*m_pIChartOCX;


// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
public:
	HWND	GetOcxHwnd( void) const
			{	return m_pIChartOCX ? m_pIChartOCX->GetOcxHwnd() : NULL;	}

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
public:
	CChartInfo::CHART_MODE	GetChartMode( void) const
							{	return ( CChartInfo::CHART_MODE)( m_pIChartOCX ? m_pIChartOCX->GetChartMode() : CChartInfo::HTS);	}

// 20081007 JS.Kim	����ڽð��� ����
protected:
	IPacketListManager *	m_pIPacketListManager;
public:
	IPacket *	GetDatePacket( const char *p_szRQ) const;
};

class CDrawingRegions
{
public:
	CDrawingRegions& operator=(const CDrawingRegions& drawingRegions);

public:
	CDrawingRegions();

	void Initialize();

	// list�� �׸� ������ �����Ѵ�.
	void SetDrawingRegions(IChartManager* pMainBlock, const CPoint& point);
	void SetDrawingRegions(IChartManager* pMainBlock, const int columnIndex);

	CBlockRegionList GetBlockRegionList() const;
	CBlockRegionList& GetBlockRegionList();
	CBlockRegion GetBlockRegion(const int nIndex) const;
	CGraphPartRegion GetGraphPartRegion(const int nIndex) const;
	CGraphPartRegion GetCurrentGraphPartRegion() const;

	int GetRowNumber(const CPoint& point) const;
	int GetCount() const;
	CRect GetFullGraphRegion(const int nIndex) const;
	CRect GetFullGraphRegion(const CPoint& point) const;
	CRect GetCurrentFullGraphRegion() const;
	CRect GetUnmixedGraphRegion(const int nIndex) const;
	CRect GetUnmixedGraphRegion(const CPoint& point) const;
	CRect GetCurrentUnmixedGraphRegion() const;

	bool PointInDrawingRegionsInColumn(const CPoint& point) const;
	bool PointInCurrentRegion(const CPoint& point) const;

private:
	CBlockRegionList m_blockRegionList;

	// ������ �ٽ� �������� ����
	bool DoseGetDrawingRegionAgain(const CPoint& point);

	// �׸��� ���� setting. -> point �̿� : ���� ���Ǵ� row ��ȣ ����.
	void SetDrawingRegions_Column(IChartManager* pMainBlock, const CPoint& point);
	void SetDrawingRegions_Row(IChartManager* pMainBlock, const CPoint& point);
	// �׸��� ���� setting. -> column number �̿� : ���� ���Ǵ� row ����.
	void SetDrawingRegions_Column(IChartManager* pMainBlock, const int columnIndex);
	void SetDrawingRegions_Row(IChartManager* pMainBlock, const int columnIndex);
};

// ----------------------------------------------------------------------------
// ȭ�鿡 ���� data Setting.
class CDisplayAttributes
{
public:
	CDisplayAttributes& operator=(const CDisplayAttributes& displayAttributes);

public:
	CDisplayAttributes();
	CDisplayAttributes(const int start, const int end);

	// ȭ�鿡 ���� data Setting.
	void SetDataStartInBlock(const int start);
	void SetDataEndInBlock(const int end);

	int GetDataStartInBlock() const;
	int GetDataEndInBlock() const;
	int GetDataCountInBlock() const;
	bool IsAllValueBiggerThanZero() const;

private:
	int m_nDataStartInBlock;	// block���� ������ ó�� data
	int m_nDataEndInBlock;		// block���� ������ ������ data
};

// ����� ���� ����
class CRegion
{
public:
	CRegion& operator=(const CRegion& data);

public:
	CRegion();
	CRegion(const CRegion& region);
	CRegion(const CRect& rect, const CDisplayAttributes& displayAttributes);

	void SetRect(const CRect& rect);
	void SetDisplayAttributes(const CDisplayAttributes& displayAttributes);

	CRect GetRect() const;
	CDisplayAttributes GetDisplayAttributes() const;
	bool DoesIndexInDisplayAttributes(const int nIndex) const;

private:
	CRect m_rect;
	CDisplayAttributes m_displayAttributes;
};

// �׸��� ���� �ʿ��� data
class CDisplayData
{
public:
	CDisplayData& operator=(const CDisplayData& data);

public:
	CDisplayData();
	CDisplayData(const CAnalysis::COORDINATETYPE eCoordinateType, 
		const CGraphPartRegion& graphPartRegion, const CDisplayAttributes& displayAttributes);

	void SetCoordinateType(const CAnalysis::COORDINATETYPE eCoordinateType);
	void SetDisplayAttributes(const CDisplayAttributes& displayAttributes);
	void SetGraphPartRegion(const CGraphPartRegion& graphPartRegion);

	bool IsRelativePointType() const;
	CAnalysis::COORDINATETYPE GetCoordinateType() const;
	CDisplayAttributes GetDisplayAttributes() const;
	CGraphPartRegion GetGraphPartRegion() const;
	CRect GetDisplayRegion() const;
	CRect GetCoordinateCalRegion() const;

private:
	CAnalysis::COORDINATETYPE m_eCoordinateType;
	CDisplayAttributes m_displayAttributes;
	CGraphPartRegion m_graphPartRegion;
};


// ----------------------------------------------------------------------------
// element�� modify type�� ã�� ���� �ʿ��� data.
class CExtractingDataOfModifyType
{
public:
	CExtractingDataOfModifyType& operator=(const CExtractingDataOfModifyType& extractingData);

public:
	CExtractingDataOfModifyType( IChartManager *p_pIChartManager);
	CExtractingDataOfModifyType( CDrawingRegions* pDrawingRegions, const CPoint& point, IChartManager *p_pIChartManager);
	
	void SetCoordinateCalculatorData(const CCoordinateCalculatorData& calData);
	void SetRowSetIndex(const int nRowSetIndex);

	// �⺻ data
	int GetRowSetIndex() const;
	CDrawingRegions* GetDrawingRegions() const;
	CPoint GetMousePoint() const;
	CCoordinateCalculatorData GetCoordinateCalculatorData() const;
	CCoordinateCalculatorData& GetCoordinateCalculatorData();
	CDisplayAttributes GetDisplayAttributes() const;

	// �������� ����
	int GetDrawingRegionCount() const;
	// �ش� ���� ��������
	CGraphPartRegion GetGraphPartRegion(const int nIndex) const;
	// ���� mousePoint�� �ִ� ���� ��������
	CRect GetFullGraphRegionInMousePoint() const;

	// element�� ã���� �˻� ����
	bool IsFindElement() const;
	// mouse point�� element�� �ִ� region�� ���ϴ��� ���� (== rowSet Region)
	bool IsPointInRowSetRegion() const;

private:
	int m_nRowSetIndex;
	CDrawingRegions* m_pDrawingRegions;
	CPoint m_mousePoint;
	CCoordinateCalculatorData m_calData;

	CBlockRegion GetBlockRegion(const int nIndex) const;
	CRect GetFullGraphRegion(const int nIndex) const;
};


// modify type�� ���� element�� �����ϱ� ���� �ʿ��� data
class CElementModifyData
{
public:
	CElementModifyData();
	CElementModifyData( const CElementModifyData& modifyData);
	~CElementModifyData();

public:
	CElementModifyData& operator=(const CElementModifyData& modifyData);

	void SetModifyType(const CToolEnumData::ModifyType modifyType);
	void SetPacket_X(IPacket* pPacket);
	void SetPoint(const CCoordinate& point);
	void SetIChartManager( IChartManager* pIChartManager);	// XScale : ���� Interface�� ��� - vntsorl(20070327)

	CToolEnumData::ModifyType GetModifyType() const;
	int GetPacket_XIndex(double dXData) const;
	double GetPacket_XValue(int nXIndex) const;
	IPacket* GetPacket_X() const;
	CCoordinate GetPoint() const;
	CCoordinate GetModifyPoint(const CCoordinate& orgPoint) const;
	IChartManager* GetIChartManager() const;				// XScale : ���� Interface�� ��� - vntsorl(20070327)
	CBlockIndex& GetBlockIndex();
	CBlockIndex GetBlockIndex() const;
	bool SetBlockIndex(const CPoint& point);
	bool SetBlockIndex(const CBlockIndex& blockIndex);

private:
	CToolEnumData::ModifyType m_modifyType;
	IPacket* m_pIPacket_x;
	CCoordinate m_modifyPoint;
	IChartManager* m_pIChartManager;						// XScale : ���� Interface�� ��� - vntsorl(20070327)
	CBlockIndex m_blockIndex;

	CCoordinate GetModifyPoint_Center(const CCoordinate& orgPoint) const;
};


// ----------------------------------------------------------------------------
// ���õ� element�� �׸��� ���� �ʿ��� data
class CSelectedElementDrawingData
{
public:
	CSelectedElementDrawingData& operator=(const CSelectedElementDrawingData& selectedDrawingData);

public:
	CSelectedElementDrawingData(const CToolEnumData::ModifyType eModifyType, 
		CDrawingRegions* pDrawingRegions, const CCoordinateCalculatorData& calData, const CPoints& points);

	CToolEnumData::ModifyType GetModifyType() const;
	CDrawingRegions* GetDrawingRegions();
	CDrawingRegions* GetDrawingRegions() const;
	const CCoordinateCalculatorData& GetCoordinateCalculatorData() const;
	CPoint GetPoint() const;
	CCoordinate GetCoordinate() const;

	CPoint GetSelectedDrawPoint(const CAnalysis::COORDINATETYPE& eType, const bool bIsAutoPrice, const CCoordinate& orgPoint) const;
	CRect GetCurrentRegion() const;
	CGraphPartRegion GetCurrentGraphPartRegion() const;
	CRegion GetRegion() const;
	CDisplayAttributes GetDisplayAttributes() const;
	CVertScaleType GetVertScaleType() const;
private:
	CToolEnumData::ModifyType m_eModifyType;
	CDrawingRegions* m_pDrawingRegions;
	CCoordinateCalculatorData m_calData;
	CPoint m_point;
	CCoordinate m_coordinate;

	CPoint GetSelectedDrawPoint_Data(const bool bIsAutoPrice, const CCoordinate& orgPoint) const;
	CPoint GetSelectedDrawPoint_Data_AutoPrice(const CCoordinate& orgPoint) const;
	CPoint GetSelectedDrawPoint_Data_NoAutoPrice(const CCoordinate& orgPoint) const;
	CPoint GetDrawingPoint(const bool bIsAutoPrice, const CCoordinate& orgPoint) const;
	CPoint GetSelectedDrawPoint_Pixel(const CCoordinate& orgPoint) const;
};


// ----------------------------------------------------------------------------
// ��ü�� ��ǥ ��������
class CElementPointList
{
public:
	CElementPointList& operator=(const CElementPointList& elementPointList);

public:
	CElementPointList();

	void AddTail(CCoordinate* coordinate);
	void RemoveAll();

	void SetVertDataType(const double& dVertDataType);

	double GetVertDataType() const;
	int GetCount() const;
	CCoordinate* GetAt(const int nIndex);
	CCoordinate* GetAt(const int nIndex) const;

private:
	double m_dVertDataType;
	CDequePointerList<CCoordinate> m_pointList;
};

#endif // !defined(AFX_ANALYSISDATA_H__73E71018_E550_4315_B600_ED567CB93E72__INCLUDED_)

