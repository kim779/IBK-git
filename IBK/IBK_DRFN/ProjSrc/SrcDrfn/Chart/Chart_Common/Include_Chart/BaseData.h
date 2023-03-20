// BaseData.h: interface for the BaseData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEDATA_H__8BA084DD_9B9D_4FBF_8C93_FC178312B669__INCLUDED_)
#define AFX_BASEDATA_H__8BA084DD_9B9D_4FBF_8C93_FC178312B669__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ScaleBaseData.h"
#include "GraphData.h"

class CPacket;
class CSubGraphPacketList;
class CIndicatorList;

// ȭ�鿡 ���� data Setting.
class CDisplayAttributes
{
public:
	CDisplayAttributes& operator=(const CDisplayAttributes& data);
	bool operator==(const CDisplayAttributes& data);

public:
	CDisplayAttributes();
	CDisplayAttributes(const int start, const int end);

	// ȭ�鿡 ���� data Setting.
	void SetDataStartInBlock(const int start);
	void SetDataEndInBlock(const int end);

	int GetDataStartInBlock() const;
	int GetDataEndInBlock() const;
	int GetDataCountInBlock() const;

private:
	int m_nDataStartInBlock;	// block���� ������ ó�� data
	int m_nDataEndInBlock;		// block���� ������ ������ data
};


// scale region�� ȯ��
class CScaleRegionAttributes
{
public:
	CScaleRegionAttributes();

	void SetMinHorzScaleRegionHeight(const int minHorzScaleRegionHeight);
	void SetMinVertScaleRegionWidth(const int minVertScaleRegionWidth);
	void SetMaxVertScaleRegionWidth(const int maxVertScaleRegionWidth);

	int GetMinHorzScaleRegionHeight() const;
	int GetMinVertScaleRegionWidth() const;
	int GetMaxVertScaleRegionWidth() const;

private:
	int m_nMinHorzScaleRegionHeight; // ���� scale�� �ּ� ����(y�� ����)
	int m_nMinVertScaleRegionWidth;  // ���� scale�� �ּ� ��(x�� ����)
	int m_nMaxVertScaleRegionWidth;  // ���� scale�� �ִ� ��(x�� ����)
};


// --------------------------------------------------------------------------------------
// graph �� ���õ� ������ �����ϴ� class
class CDataBarRegion
{
public:
	CDataBarRegion();
	CDataBarRegion(const bool bIsTop, const bool bIsBottom);

	void SetTop(const bool bIsTop);
	void SetBottom(const bool bIsBottom);

	CRect GetDrawingRegion_Center(const CRect& region, const int nHight) const;
	CRect GetDrawingRegion_Top(const CRect& region, const int nHight) const;
	CRect GetDrawingRegion_Bottom(const CRect& region, const int nHight) const;

	bool IsTopDataBar() const;
	bool IsBottomDataBar() const;

private:
	bool m_bIsTop;	// graph type : DataBar_Type �̰� graph style : Top �� ���.
	bool m_bIsBottom;// graph type : DataBar_Type �̰� graph style : Bottom �� ���.

	int GetDataBarHight(const bool bIsDataBar, const nMaxHight, const int nMinHight) const;
};

class CGraphRegions
{
public:
	enum REGIONPOSITION
	{
		CENTER = 0,
		TOP,
		BOTTOM,
	};

public:
	CGraphRegions();
	CGraphRegions(const CRect& noFrameRegion, const int nFontSize, const bool bIsTitle, const bool bIsText, 
		const bool bIsVertScale, const bool bIsTopDataBar, const bool bIsBottomDataBar);

	void SetNoFrameRegion(const CRect& region);
	void SetFontSize(const int nSize);
	void OffsetFontSize(const int nSize);
	void SetTitle(const bool bIsTitle);
	void SetText(const bool bIsText);
	void SetVertScale(const bool bIsVertScale);
	void SetTopDataBar(const bool bIsTopDataBar);
	void SetBottomDataBar(const bool bIsBottomDataBar);

	bool IsTitle() const;
	bool IsText() const;
	CDataBarRegion& GetDataBarRegion();
	CRect GetFrameRegion(const int frameThickness) const;
	CRect GetNoFrameRegion() const;
	CRect GetNoFrameRegionAndMargin() const;
	// title ����
	CRect GetTitleRegion() const;
	// title�� textHeight�� ������ ������ ������ ����
	CRect GetDrawingRegion(const CGraphRegions::REGIONPOSITION position = CGraphRegions::CENTER) const;
	CRect GetDrawingRegion(const CGraphBaseData::GRAPHTYPE eGraphType, const CGraphStyle& graphStyle) const;

private:
	CRect m_noFrameRegion;	// graph ����(frame�� ������ ����)
	int m_nFontSize;		// font�� size
	bool m_bIsTitle;		// graph title ���翩��
	bool m_bIsText;			// �ְ�/�������� ���� ���� ����
	bool m_bIsVertScale;	// vert scale �� ����
	CDataBarRegion m_dataBar;

	CRect GetRealDrawingRegion() const;
	CRect GetNoTitleRegion(const CRect& region) const;
	CRect GetNoTextCenterRegion(const CRect& region) const;
	CRect GetRegionInDataBar(const CRect& region) const;
};


// graph, scale������ �����ϴ� class
class CDrawingRegion
{
public:
	void SetGraphRegions(const CGraphRegions& graphRegions);
	void SetHorzScaleRegion(const CRect& topRegion, const CRect& bottomRegion);
	void SetVertScaleRegion(const CRect& leftRegion, const CRect& rightRegion);
	void OffsetDrawingRegion(const int nFontSize);

	CGraphRegions& GetGraphRegions();
	CGraphRegions GetGraphRegions() const;
	CRect GetGraphFrameRegion(const int graphRegionFrameThickness) const;
	CHorzScaleRegion GetHorzScaleRegion() const;
	CVertScaleRegion GetVertScaleRegion() const;

private:
	CGraphRegions m_graphRegions; // graph ����
	CHorzScaleRegion m_horzScaleRegion;	// ���� scale ����
	CVertScaleRegion m_vertScaleRegion;	// ���� scale ����
};

// --------------------------------------------------------------------------------------
// ȭ�鿡 ���̴� data�� min, max.
class CDisplayDataMinMax
{
public:
	CDisplayDataMinMax& operator=(const CDisplayDataMinMax& displayDataMinMax);

public:
	CDisplayDataMinMax();
	CDisplayDataMinMax(const double& dMin, const double& dMax);

	void SetDisplayData(const double& dMin, const double& dMax);
	void SetDisplayMin(const double& dMin);
	void SetDisplayMax(const double& dMax);
	void Inflate(const CDisplayDataMinMax& displayDataMinMax);
	void Inflate(const double& dMinGap, const double& dMaxGap);

	double& GetDisplayMin();
	double& GetDisplayMax();
	double GetDisplayMin() const;
	double GetDisplayMax() const;
	double GetCount() const;

	double GetMarginDisplayMin() const;
	double GetMarginDisplayMax() const;

	bool IsAllZero() const;
	bool IsEqual() const;
	bool IsCloseToMaxValue(const double& dCur, const bool bIsInvert = false) const;

private:
	double m_dMin;	// block���� ������ data�� �ּҰ�
	double m_dMax;	// block���� ������ data�� �ִ밪
};


// --------------------------------------------------------------------------------------
// graph�� �׸��� ���� data
class CDrawingGraphData
{
public:
	// (2004.04.01, ��¿�) Default Indicator List�� Static���� �����Ͽ�, �ݺ� Loading�� �����Ѵ�.
	//		Static���� CIndicatorList�� Pointer�� �������� �ʴ´�. (CIndicatorList�� Static�� �ƴϴ�.)
	//		CIndicatorInfo�� �������� �ʰ� CIndicatorList�� ������ �� �ֵ��� �����Ѵ�.
	CDrawingGraphData(CIndicatorList* pIndicatorList, 
		const CDrawingOption::DRAWINGORDER eDrawingOrderType, 
		const CGraphRegions& graphRegions, const CRect& scaleTextRegion,
		const CDisplayDataMinMax& displayDataHorzMinMax, const CString& horzScalePacketName, 
		const CEtcGraphColor& etcGraphColor, const CDisplayAttributes& displayAttributes, 
		const bool bIsVertScaleLogType, const bool bIsVertScaleInvertType, 
		const bool bIsConditionTitleDraw, const bool bIsDataTitleDraw);

	void SetDisplayDataVertMinMax(const CDisplayDataMinMax& displayDataVertMinMax);
	void SetFunctionName(const CString& functionName);
	void SetPacketNames(const CString& packetNames);
	void SetTitleDraw(const bool bIsConditionTitleDraw, const bool bIsDataTitleDraw);
	void SetConditionTitleDraw(const bool bIsConditionTitleDraw);
	void SetDataTitleDraw(const bool bIsDataTitleDraw);
	void SetGraphDataDrawingData(const CGraphDataDrawingData& graphDataDrawingData);
	void SetPriceChartColor(CGraphColor* pGraphColor);
	void SetIndicatorInfo(CIndicatorInfo* pIndiInfo);
	void SetSubGraphPacketList(CSubGraphPacketList* pSubGraphPacketList);

	CDrawingOption::DRAWINGORDER GetDrawingOrderType() const;
	bool DoesVertScaleLogType() const;
	bool DoesVertScaleInvertType() const;
	const CGraphRegions& GetGraphRegions() const;
	CRect GetScaleTextRegion() const;
	CDisplayDataMinMax GetDisplayDataHorzMinMax() const;
	CDisplayDataMinMax GetDisplayDataVertMinMax() const;
	CString GetFunctionName() const;
	CString GetPacketName() const;
	CString GetHorzScalePacketName() const;
	bool DoesConditionTitleDraw() const;
	bool DoesDataTitleDraw() const;
	CGraphDataDrawingData GetGraphDataDrawingData() const;
	CDisplayAttributes GetDisplayAttributes() const;
	CEtcGraphColor GetEtcGraphColor() const;
	CGraphColor* GetPriceChartColor() const;
	CIndicatorList* GetIndicatorList() const;
	CIndicatorInfo* GetIndicatorInfo() const;
	CSubGraphPacketList* GetSubGraphPacketList() const;

private:
	CDrawingOption::DRAWINGORDER m_eDrawingOrderType;
	CGraphRegions m_graphRegions;
	CRect m_scaleTextRegion;
	CDisplayDataMinMax m_displayDataHorzMinMax;
	CDisplayDataMinMax m_displayDataVertMinMax;
	bool m_bIsVertScaleLogType;
	bool m_bIsVertScaleInvertType;
	CString m_strFunctionName;
	CString m_strPacketNames;
	CString m_strHorzScalePacketName;
	bool m_bIsConditionTitleDraw;
	bool m_bIsDataTitleDraw;
	CGraphDataDrawingData m_drawingData;	// Graph�� data ǥ��
	CDisplayAttributes m_displayAttributes;
	CEtcGraphColor m_etcGraphColor;
	CGraphColor* m_pPriceChartColor;
	CIndicatorInfo* m_pIndiInfo;
	CSubGraphPacketList* m_pSubGraphPacketList;
	// (2004.04.01, ��¿�) Default Indicator List�� Static���� �����Ͽ�, �ݺ� Loading�� �����Ѵ�.
	//		Static���� CIndicatorList�� Pointer�� �������� �ʴ´�. (CIndicatorList�� Static�� �ƴϴ�.)
	//		CIndicatorInfo�� �������� �ʰ� CIndicatorList�� ������ �� �ֵ��� �����Ѵ�.
	CIndicatorList* m_pIndicatorList;
};


// ���� scale�� �׸��� ���� data
class CDrawingHorzScaleData
{
public:
	CDrawingHorzScaleData(const CRect& drawingGraphRegion, const CGraphBaseData::GRAPHTYPE& eHeadGraphType, 
		const CGraphStyle& headGraphStyle, const CGraphRegionColor& graphRegionColor, 
		const CScaleColor& scaleColor, const CDisplayAttributes& displayAttributes, 
		const CDisplayDataMinMax& displayDataMinMax, CPacket* pPacket);

	CRect GetDrawingGraphRegion() const;
	CGraphBaseData::GRAPHTYPE GetHeadGraphType() const;
	CGraphStyle GetHeadGraphStyle() const;
	CScaleBaseData::HORZSCALEDRAWERTYPE GetScaleDrawerType() const;
	CScaleColor GetScaleColor() const;
	CGraphRegionColor GetGraphRegionColor() const;
	COLORREF GetCompartLineColor() const;
	CDisplayAttributes GetDisplayAttributes() const;
	CDisplayDataMinMax GetDisplayDataMinMax() const;
	CPacket* GetPacket() const;

	void SetScaleDrawerType(const CScaleBaseData::HORZSCALEDRAWERTYPE& drawType);

private:
	CRect m_drawingGraphRegion;
	CGraphBaseData::GRAPHTYPE m_eHeadGraphType;
	CGraphStyle m_headGraphStyle;
	CScaleBaseData::HORZSCALEDRAWERTYPE m_TYPEScaleDrawer; // scale draw type
	CScaleColor m_scaleColor;				// scale color
	CGraphRegionColor m_graphRegionColor;	// graph ������
	CDisplayAttributes m_displayAttributes;	// block�� ���� ��ġ
	CDisplayDataMinMax m_displayDataMinMax;	// ����(x)�� min, max
	CPacket* m_pPacket;
};


class CIndicatorInfo;

// ���� scale�� �׸��� ���� data
class CDrawingVertScaleData
{
public:
	CDrawingVertScaleData(const CRect& noFrameGraphRegion, const CRect& drawingGraphRegion, 
		const CGraphRegionColor& graphRegionColor, const CScaleColor& scalrColor, 
		const bool bIsVertScaleInvertType, const bool bIsVertScaleLogType, const CDisplayAttributes& displayAttributes);

	CRect GetNoFrameGraphRegion() const;
	CRect GetDrawingGraphRegion() const;
	bool GetVertScaleInvertType() const;
	bool GetVertScaleLogType() const;
	CString GetSubGraphNames() const;
	CString GetPacketNames() const;
	CString GetFunctionName() const;
	CScaleBaseData::VERTSCALEDRAWERTYPE GetScaleDrawerType() const;
	CScaleColor GetScaleColor() const;
	CGraphRegionColor GetGraphRegionColor() const;
	COLORREF GetCompartLineColor() const;
	CDisplayAttributes GetDisplayAttributes() const;
	CDisplayDataMinMax GetDisplayDataMinMax() const;
	CIndicatorInfo* GetIndicatorInfo() const;
	CPacket* GetPacket() const;

	void SetSubGraphNames(const CString& subGraphNames);
	void SetPacketNames(const CString& packetNames);
	void SetFunctionName(const CString& functionName);
	void SetScaleDrawerType(const CScaleBaseData::VERTSCALEDRAWERTYPE& drawType);
	void SetDisplayDataMinMax(const CDisplayDataMinMax& displayDataMinMax);
	void SetIndicatorInfo(CIndicatorInfo* pIndicatorInfo);
	void SetPacket(CPacket* pPacket);

private:
	CRect m_noFrameGraphRegion;
	CRect m_drawingGraphRegion;
	bool m_bIsVertScaleInvertType;
	CString m_strSubGraphNames;
	CString m_strPacketNames;
	CString m_strFunctionName;
	CScaleBaseData::VERTSCALEDRAWERTYPE m_TYPEScaleDrawer; // scale draw type
	CScaleColor m_scaleColor;	// scale color
	CGraphRegionColor m_graphRegionColor;	// graph ������
	CDisplayAttributes m_displayAttributes;	  // block�� ���� ��ġ
	CDisplayDataMinMax m_displayDataMinMax;	  // ����(y)�� min, max
	CIndicatorInfo* m_pIndicatorInfo;
	CPacket* m_pPacket;
};


// --------------------------------------------------------------------------------------
// ������ ������ ������ �׸� text�� ���� ����
class CTextInRightScaleRegionDrawer
{
public:
	CTextInRightScaleRegionDrawer(const CString& strFunctionName = "", CSubGraphPacketList* pSubGraphPacketList = NULL, 
		const CRect& region = CRect(0, 0, 0, 0), const int nCurDataType = 0, const int nDataEnd = 0);

	void SetFunctionName(const CString& strFunctionName);
	void SetSubGraphPacketList(CSubGraphPacketList* pSubGraphPacketList);
	void SetRegion(const CRect& region);
	void SetCurDataType(const int nCurDataType);
	void SetDataEnd(const int nDataEnd);

	CString GetFunctionName() const;
	CSubGraphPacketList* GetSubGraphPacketList() const;
	CRect GetRegion() const;
	int GetCurDataType() const;
	int GetDataEnd() const;

	bool IsTextDrawing() const;

private:
	CString m_strFunctionName;
	CSubGraphPacketList* m_pSubGraphPacketList;
	CRect m_region;
	int m_nCurDataType;
	int m_nDataEnd;
};

#endif // !defined(AFX_BASEDATA_H__8BA084DD_9B9D_4FBF_8C93_FC178312B669__INCLUDED_)
