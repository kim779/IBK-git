// ChartObjectBaseData.h: interface for the CChartObjectBaseData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTOBJECTBASEDATA_H__1F9E6948_1AAE_448F_9EC9_784200EF8076__INCLUDED_)
#define AFX_CHARTOBJECTBASEDATA_H__1F9E6948_1AAE_448F_9EC9_784200EF8076__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "../Include_Chart/BaseData.h"
#include "../Include_Chart/ScaleBaseData.h"

class CPacket;
class CPacketList;
class CIndicatorList;
class CIndicatorInfo;
class CMainChartFormulate;
class CMainBlockImp;
class CXScaleManager;
class CPacketListManager;	// �������� ���� : �������� - ojtaso (20070111)
interface IToolBarManager;
interface IChartCtrl;

// CMainBlock�� ������ �ִ� data
class CMainAttributes
{
public:
	CMainAttributes& operator=(const CMainAttributes& data);

public:
	CMainAttributes();

// Interfaces
protected:
	bool						m_bDesignMode;			// design & user mode
	CString						m_strOCXPath;			// OCX ���
	CMainBlockImp *				m_pMainBlockImpl;
	IChartCtrl *				m_pIChartCtrl;			// ocx interface
	CPacketList *				m_pPacketList;			// ocx���� ���� packetList
	CIndicatorList *			m_pIndicatorList;		// ocx���� ���� ��ǥ �Ӽ�.
	CMainChartFormulate	*		m_pMainChartFormulate;	// ����
	CXScaleManager*				m_pxScaleManager;		// ocx���� ���� X-Scale Manager
	CPacketListManager*			m_pPacketListManager;	// �������� ���� : �������� - ojtaso (20070111)
	IToolBarManager*			m_pToolBarManager;
// Set
public:
	void	SetMainBlockImpl(		CMainBlockImp *pMainBlockImpl);
	void	SetIChartCtrl(			IChartCtrl* p_pICharetCtrl);
	void	SetDesignMode(			const bool designMode);
	void	SetOCXPath(				const CString& strOCXPath);
	void	SetPacketList(			CPacketList* pPacketList);
	void	SetIndicatorList(		CIndicatorList* pIndicatorList);
	void	SetMainChartFormulate(	CMainChartFormulate	*p_pMainChartFormulate);
	void	SetXScaleManager(		CXScaleManager* pxScaleManager);
	void	SetPacketListManager(	CPacketListManager* pPacketListManager);	// �������� ���� : �������� - ojtaso (20070111)
	void	SetToolBarManager(		IToolBarManager* pToolBarManager);

// Get
public:
	CMainBlockImp *				GetMainBlockImpl( void) const;
	IChartCtrl *				GetIChartCtrl( void) const;
	CWnd *						GetParentWnd( void) const;
	HWND						GetParent_Handle( void) const;
	bool						IsDesignMode( void) const;
	CString						GetOCXPath( void) const;
	CPacketList *				GetPacketList( void) const;
	CIndicatorList *			GetIndicatorList( void) const;
	CMainChartFormulate *		GetMainChartFormulate( void) const;
	CXScaleManager*				GetXScaleManager() const;
	CPacketListManager*			GetPacketListManager() const;	// �������� ���� : �������� - ojtaso (20070111)
	IToolBarManager*			GetToolBarManager() const;

// Other
public:
	void Invalidate();
	void SendMessageToParent(const CBlockBehaviorData& blockBehaviorData);
	void SendMessageToParent(const CString& strGraphName, const bool bIsAddMargin);
	void SendMessageToParent_TRData(const CString& strGraphName, const CString& strPacketNames);
	void SendMessageToParent_TRData(const CString& strGraphName, const CString& strPacketNames, const int nRequestTRCount, const bool bIsNew);
	void SendMessageToParent_CalculateIndicatorInTRData(CIndicatorInfo* pIndicatorInfo, const CList<CPacket*, CPacket*>& packetList);

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
public:
	CChartInfo::CHART_MODE	GetChartMode( void) const;
};

class CBlockAttributes
{
public:
	CBlockAttributes& operator=(const CBlockAttributes& data);

public:
	CBlockAttributes();

	void SetBlockType(const CBlockBaseData::BLOCKTYPE eBlockType);
	void SetBlockCompartType(const CBlockBaseData::BLOCKCOMPART eBlockCompartType);
	void SetMaxBlocksCountInView(const int maxBlocksCountInView);

	CBlockBaseData::BLOCKTYPE GetBlockType() const;
	CBlockBaseData::BLOCKCOMPART GetBlockCompartType() const;
	int GetMaxBlocksCountInView() const;

private:
	// block���� ��� �⺻�� �Ǵ� data
	CBlockBaseData::BLOCKTYPE m_eBlockType;	// block�� Type
	CBlockBaseData::BLOCKCOMPART m_eBlockCompartType;	// block�� ���� Type
	int m_nMaxBlocksCountInView;	// ȭ�鿡 �߰��� �� �ִ� �� block ��
};

class CGraphAttributes
{
public:
	CGraphAttributes& operator=(const CGraphAttributes& data);

public:
	CGraphAttributes();

	void SetAllGraphsTitleDrawHiding(const bool bIsAllTitleDrawHiding);
	void SetGraphTitleDraw(const bool bIsCondition, const bool bIsData);
	void SetAutoDeleteIndicatorInfo(const bool bIsAutoDeleteIndicatorInfo);
	void SetGraphBackgroundGap(const int nGapX, const int nGapY);
	void SetGraphRegionFrameThickness(const int nThickness);
	void SetGraphRegionColor(const CGraphRegionColor& graphRegionColor);
	void SetEtcGraphColor(const CEtcGraphColor& etcGraphColor);

	bool DoesAllTitleDrawHiding() const;
	bool DoesConditionTitleDraw() const;
	bool DoesDataTitleDraw() const;
	bool DoesAutoDeleteIndicatorInfo() const;
	int GetGraphBackgroundGapX() const;
	int GetGraphBackgroundGapY() const;
	int GetGraphRegionFrameThickness() const;
	CGraphRegionColor GetGraphRegionColor() const;
	CEtcGraphColor GetEtcGraphColor() const;

private:
	bool m_bIsAllTitleDrawHiding;	// graph title�� ��� ����
	bool m_bIsConditionTitleDraw;	// graph title�� ���ǰ� ǥ��
	bool m_bIsDataTitleDraw;		// graph title�� data ǥ��
	bool m_bIsAutoDeleteIndicatorInfo;	// graph�� �ִ� indicatorInfo�� graph ������ �ڵ����� �������� ����
	int m_nGraphBackgroundGapX;		// graph ������ ����(����)
	int m_nGraphBackgroundGapY;		// graph ������ ����(����)
	int m_nGraphRegionFrameThickness;	// graph region �׵θ��� ����
	CGraphRegionColor m_graphRegionColor;	// graph region color
	CEtcGraphColor m_etcGraphColor;	// ��Ÿ graph ��
};

class CScaleAttributes
{
public:
	CScaleAttributes& operator=(const CScaleAttributes& data);

public:
	CScaleAttributes();
	
	void SetHorzScaleMessageBoxShow(const bool bHorzScaleMessageBoxShow);
	void SetOnlyPriceVertScale(const bool bIsOnlyPrice);
	void SetScaleRegionAttributes(const CScaleRegionAttributes& scaleRegionAttributes);
	void SetMinHorzScaleRegionHeight(const int minHorzScaleRegionHeight);
	void SetMinVertScaleRegionWidth(const int minVertScaleRegionWidth);
	void SetMaxVertScaleRegionWidth(const int maxVertScaleRegionWidth);
	void SetScaleColor(const CScaleColor& scaleColor);

	bool DoesHorzScaleMessageBoxShow() const;
	bool DoesOnlyPriceVertScale() const;
	CScaleRegionAttributes GetScaleRegionAttributes() const;
	CScaleRegionAttributes& GetScaleRegionAttributes();
	CScaleColor GetScaleColor() const;
	CScaleColor& GetScaleColor();
	int GetMinHorzScaleRegionHeight() const;
	int GetMinVertScaleRegionWidth() const;
	int GetMaxVertScaleRegionWidth() const;

private:
	bool m_bIsHorzScaleMessageBoxShow; // �ϴ� scale �� ���������� ���� message box load.
	bool m_bIsOnlyPriceVertScale;	// "������Ʈ" �� scale�� �� ǥ��
	CScaleRegionAttributes m_scaleRegionAttributes; // scale ������ �⺻��
	CScaleColor m_scaleColor;		// scale ��

// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
//	CScaleBaseData::SCALEGRIDTYPE m_TYPEGridType;	// grid type
//	void SetBaseScaleGridType(const CScaleBaseData::SCALEGRIDTYPE& gridType);
//	CScaleBaseData::SCALEGRIDTYPE GetBaseScaleGridType() const;
};

class CViewAttributes
{
public:
	CViewAttributes& operator=(const CViewAttributes& data);

public:
	CViewAttributes();

	void SetDataRegionInBlock(const int start, const int end);
	void SetDrawingOrderType(const CDrawingOption::DRAWINGORDER eDrawingOrderType);
	void SetMessageBoxShow(const bool bIsShow);
	void SetBackgroundColor(const COLORREF& color);

	CDisplayAttributes GetDisplayAttributes() const;
	CDrawingOption::DRAWINGORDER GetDrawingOrderType() const;
	CDrawingOption::DRAWINGORDER& GetDrawingOrderType();
	bool DoesMessageBoxShow() const;
	COLORREF GetBackgroundColor() const;

private:
	CDisplayAttributes m_displayAttributes; // ȭ�鿡 ���̴� data����
	CDrawingOption::DRAWINGORDER m_eDrawingOrderType;	// �׸��� ��ġ
	bool m_bIsMessageBoxShow;		// messageBox ����
	COLORREF m_backgroundColor;		// ������
};

class CMainBlockBaseData
{
public:
	CMainBlockBaseData();
	CMainBlockBaseData& operator=(const CMainBlockBaseData& data);

public:
	enum POINTPOSITION
	{
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};

	void SetMinHorzScaleRegionHeight(const int minHorzScaleRegionHeight);
	void SetMinVertScaleRegionWidth(const int minVertScaleRegionWidth);
	void SetMaxVertScaleRegionWidth(const int maxVertScaleRegionWidth);

	CMainAttributes GetMainAttributes() const;
	CMainAttributes& GetMainAttributes();
	CBlockAttributes GetBlockAttributes() const;
	CBlockAttributes& GetBlockAttributes();
	CGraphAttributes GetGraphAttributes() const;
	CGraphAttributes& GetGraphAttributes();
	CScaleAttributes GetScaleAttributes() const;
	CScaleAttributes& GetScaleAttributes();
	CViewAttributes GetViewAttributes() const;
	CViewAttributes& GetViewAttributes();

	CWnd* GetParentWnd() const;
	HWND GetParent_Handle() const;
	CPacketList* GetPacketList() const;
	CIndicatorList* GetIndicatorList() const;
	CXScaleManager* GetXScaleManager() const;	
	CPacketListManager* GetPacketListManager() const;	// �������� ���� : �������� - ojtaso (20070111)
	IToolBarManager* GetToolBarManager() const;
	bool DoesAutoDeleteIndicatorInfo() const;	
	int GetGraphRegionFrameThickness() const;
	int GetMinHorzScaleRegionHeight() const;
	int GetMinVertScaleRegionWidth() const;
	int GetMaxVertScaleRegionWidth() const;

	CBlockBaseData::BLOCKTYPE GetBlockType() const;
	CBlockBaseData::BLOCKCOMPART GetBlockCompartType() const;
	int GetMaxBlocksCountInView() const;
	CGraphRegionColor GetGraphRegionColor() const;
	CDisplayAttributes GetDisplayAttributes() const;

	void Invalidate();
	void SendMessageToParent(const CBlockBehaviorData& blockBehaviorData);
	void SendMessageToParent(const CString& strGraphName, const bool bIsAddMargin);
	void SendMessageToParent_TRData(const CString& strGraphName, const CString& strPacketNames);
	void SendMessageToParent_TRData(const CString& strGraphName, const CString& strPacketNames, const int nRequestTRCount, const bool bIsNew);
	void SendMessageToParent_CalculateIndicatorInTRData(CIndicatorInfo* pIndicatorInfo, const CList<CPacket*, CPacket*>& packetList);

private:	
	CMainAttributes m_mainAttributes;
	CBlockAttributes m_blockAttributes;
	CGraphAttributes m_graphAttributes;
	CScaleAttributes m_scaleAttributes;
	CViewAttributes m_viewAttributes; // ȭ�鿡 ���� ����

// (2006/12/12 - Seung-Won, Bae) Support Indicator Calculation Enabling Flag
protected:
	BOOL	m_bEnableIndicatorCalculation;
public:
	void	EnableIndicatorCalculation( BOOL p_bEnableIndicatorCalculation)		{	m_bEnableIndicatorCalculation = p_bEnableIndicatorCalculation;	}
	BOOL	IsEnableIndicatorCalculation( void) const							{	return m_bEnableIndicatorCalculation;							}

// (2007/2/22 - Seung-Won, Bae) Support Base Text Option
protected:
	BOOL	m_bWithBaseText;
public:
	BOOL	GetWithBaseText( void) const				{	return m_bWithBaseText;	}
	void	SetWithBaseText( BOOL p_bWithBaseText)		{	m_bWithBaseText = p_bWithBaseText;	}

// (2007/6/28 - Seung-Won, Bae) Manage MessageBox Info
protected:
	BOOL	m_bMessageBoxUsed;
public:
	BOOL	IsMessageBoxUsed( void)							{	return m_bMessageBoxUsed;	}
	void	SetMessageBoxUsed( BOOL p_bMessageBoxUsed)		{	m_bMessageBoxUsed = p_bMessageBoxUsed;	}
public:
	BOOL	DoesMessageBoxShow( void) const;
};


// block�� ���� data
class CBlockData
{
public:
	CBlockData();

	void SetDoubleSize(const bool bIsDoubleSize);
	void SetVertScaleLogType(const bool bIsVertScaleLogType);
	void SetHorzScale(const CHorizontalScale& horzScale);
	void SetDisplayDataHorzMinMax(const CDisplayDataMinMax& displayDataHorzMinMax);
	// XScale ���� ���� - ojtaso (20080429)
	void SetHorizontalScaleHeight(const int nHeight);

	bool GetDoubleSize() const;
	bool GetVertScaleLogType() const;
	bool& GetVertScaleLogType();
	CHorizontalScale GetHorzScale() const;
	CHorizontalScale& GetHorzScale();
	CDisplayDataMinMax GetDisplayDataHorzMinMax() const;
	CDisplayDataMinMax& GetDisplayDataHorzMinMax();
	// XScale ���� ���� - ojtaso (20080429)
	int GetHorizontalScaleHeight() const;

private:
	CHorizontalScale m_horzScale; // ���� scale
	CDisplayDataMinMax m_displayDataHorzMinMax; // ���� scale min/max
	bool m_bIsVertScaleLogType;	// ���� scale type�� log���� ����
	bool m_bIsDoubleSize;		// ���� size�� 2������ ����
};

#endif // !defined(AFX_CHARTOBJECTBASEDATA_H__1F9E6948_1AAE_448F_9EC9_784200EF8076__INCLUDED_)
