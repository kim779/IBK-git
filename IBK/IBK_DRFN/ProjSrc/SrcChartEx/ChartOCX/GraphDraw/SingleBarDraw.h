#if !defined(AFX_SINGLEBARDRAW_H__2EBE73B1_8773_481F_B8EA_B5F39F03D4F1__INCLUDED_)
#define AFX_SINGLEBARDRAW_H__2EBE73B1_8773_481F_B8EA_B5F39F03D4F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SingleBarDraw.h : header file
//
#include "BarDraw.h"

/////////////////////////////////////////////////////////////////////////////
// CSingleBarDraw window
//class CIndicatorInfo;
class CXScaleManager;

class CSingleBarDraw : public CBarDraw
{
// Construction
public:
	CSingleBarDraw();
	virtual ~CSingleBarDraw();

	virtual bool DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData);

	virtual bool DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData,
			const CString& strRQ,
			CXScaleManager* pxScaleManager);

protected:
	void GetMarginMinMax(const double dMin, const double dMax, double& dRealMin, double& dRealMax);
	bool DrawVerticalBar_General(CDC* pDC, CPacket* pPacket, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle,
		const CRect& graphRegion, const CGraphColor& graphColor, const int nBarWidth, const double dRealWidth, 
		const int nStart, const int nEnd, const double& dViewMax, const double& dViewMin, 
		const CPenThickness& penThickness, const bool bLog, const bool bInvert);
    bool DrawVerticalBar_General(CDC* pDC, CPacket* pPacket, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle,
		const CRect& graphRegion, const CGraphColor& graphColor, const int nBarWidth, const double dRealWidth, 
		const int nStart, const int nEnd, const double& dViewMax, const double& dViewMin, 
		const CPenThickness& penThickness, const bool bLog, const bool bInvert, 
		const CString& strRQ, CXScaleManager* pxScaleManager, CSubGraphData& subGraphData, int p_nColumn, CIndicatorInfo* pIndicatorInfo);
	bool DrawVerticalBar_General(CDC* pDC, CPacket* pPacket, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle,
		const CRect& graphRegion, const int nBarWidth, const double dRealWidth, int nStart, int nEnd, 
		double dRealMax, double dRealMin, const bool bLog, const bool bInvert, 
		const CPenThickness& penThickness, const COLORREF& penColor, const COLORREF& brushColor, const bool bIsFill);
	bool DrawVerticalBar_General(CDC* pDC, CPacket* pPacket, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle,
		const CRect& graphRegion, const int nBarWidth, const double dRealWidth, int nStart, int nEnd, 
		double dRealMax, double dRealMin, const bool bLog, const bool bInvert, 
		const CPenThickness& penThickness, const COLORREF& penColor, const COLORREF& brushColor, const bool bIsFill,
		const CString& strRQ, CXScaleManager* pxScaleManager, CSubGraphData& subGraphData, int p_nColumn);

		
	bool DrawVerticalBar_VolumeChart_HighLow(CDC* pDC, CPacket* pVolumePacket, 
		const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, const CRect& graphRegion, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth, const int nStart, const int nEnd, const double& dViewMax, const double& dViewMin, 
		const CPenThickness& penThickness, const bool bLog, const bool bInvert);

	bool DrawVerticalBar_VolumeChart_HighLow(CDC* pDC, CPacket* pVolumePacket, 
		const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, const CRect& graphRegion, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth, const int nStart, const int nEnd, const double& dViewMax, const double& dViewMin, 
		const CPenThickness& penThickness, const bool bLog, const bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, CSubGraphData& subGraphData, int p_nColumn);

	// (2004.04.01, ��¿�) Default Indicator List�� Static���� �����Ͽ�, �ݺ� Loading�� �����Ѵ�.
	//		Static���� CIndicatorList�� Pointer�� �������� �ʴ´�. (CIndicatorList�� Static�� �ƴϴ�.)
	//		CIndicatorInfo�� �������� �ʰ� CIndicatorList�� ������ �� �ֵ��� �Ѵ�.
	bool DrawVerticalBar_VolumeChart_Price(CDC* pDC, CPacket* pVolumePacket, CPacket* pOpenPacket, CPacket* pClosePacket, 
		CIndicatorInfo* pIndicatorInfo, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, const CRect& graphRegion, 
		const CGraphColor& graphColor, CGraphColor* pPriceChartColor, const int nBarWidth, const double dRealWidth, const int nStart, const int nEnd, 
		const double& dViewMax, const double& dViewMin, const CPenThickness& penThickness, const bool bLog, const bool bInvert, CIndicatorList *pIndicatorList);
	bool DrawVerticalBar_VolumeChart_Price(CDC* pDC, CPacket* pVolumePacket, CPacket* pOpenPacket, CPacket* pClosePacket, 
		CIndicatorInfo* pIndicatorInfo, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, const CRect& graphRegion, 
		const CGraphColor& graphColor, CGraphColor* pPriceChartColor, const int nBarWidth, const double dRealWidth, const int nStart, const int nEnd, 
		const double& dViewMax, const double& dViewMin, const CPenThickness& penThickness, const bool bLog, const bool bInvert, CIndicatorList *pIndicatorList,
		const CString& strRQ, CXScaleManager* pxScaleManager, CSubGraphData& subGraphData, int p_nColumn, CMainBlock *pMainBlock);
	bool DrawVerticalBar_VolumeChart_PrevPrice(CDC* pDC, CPacket* pVolumePacket, CPacket* pClosePacket, 
		CIndicatorInfo* pIndicatorInfo, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, const CRect& graphRegion, 
		const CGraphColor& graphColor, CGraphColor* pPriceChartColor, const int nBarWidth, const double dRealWidth, const int nStart, const int nEnd, 
		const double& dViewMax, const double& dViewMin, const CPenThickness& penThickness, const bool bLog, const bool bInvert, CIndicatorList *pIndicatorList);
	bool DrawVerticalBar_VolumeChart_PrevPrice(CDC* pDC, CPacket* pVolumePacket, CPacket* pClosePacket, 
		CIndicatorInfo* pIndicatorInfo, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, const CRect& graphRegion, 
		const CGraphColor& graphColor, CGraphColor* pPriceChartColor, const int nBarWidth, const double dRealWidth, const int nStart, const int nEnd, 
		const double& dViewMax, const double& dViewMin, const CPenThickness& penThickness, const bool bLog, const bool bInvert, CIndicatorList *pIndicatorList,
		const CString& strRQ, CXScaleManager* pxScaleManager, CSubGraphData& subGraphData, int p_nColumn);
	bool DrawVerticalBar_VolumeChart(CDC* pDC, CPacket* pVolumePacket, CPacket* pOpenPacket, CPacket* pClosePacket, 
		CIndicatorInfo* pIndicatorInfo, const int nType, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, 
		const CRect& graphRegion, const CGraphColor& graphColor, CGraphColor* pPriceChartColor, 
		const int nBarWidth, const double dRealWidth, const int nStart, const int nEnd, const double& dViewMax, 
		const double& dViewMin, const CPenThickness& penThickness, const bool bLog, const bool bInvert, CIndicatorList *pIndicatorList);
	bool DrawVerticalBar_VolumeChart(CDC* pDC, CPacket* pVolumePacket, CPacket* pOpenPacket, CPacket* pClosePacket, 
		CIndicatorInfo* pIndicatorInfo, const int nType, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, 
		const CRect& graphRegion, const CGraphColor& graphColor, CGraphColor* pPriceChartColor, 
		const int nBarWidth, const double dRealWidth, const int nStart, const int nEnd, const double& dViewMax, 
		const double& dViewMin, const CPenThickness& penThickness, const bool bLog, const bool bInvert, CIndicatorList *pIndicatorList,
		const CString& strRQ, CXScaleManager* pxScaleManager, const CSubGraphData& subGraphData, int p_nColumn, CMainBlock *pMainBlock); //��Ʈ ���� ���� CSubGraphData�߰�
	bool DrawVerticalBar(CDC* pDC, const CRect& graphRegion, const CString& GraphName, 
		const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, CGraphColor* pPriceChartColor, 
		const int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, CIndicatorInfo* pIndicatorInfo, 
		const CPenThickness& penThickness, bool bLog, bool bInvert, CIndicatorList *pIndicatorList);
	bool DrawVerticalBar(CDC* pDC, const CRect& graphRegion, const CString& GraphName, 
		const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, CGraphColor* pPriceChartColor, 
		const int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, CIndicatorInfo* pIndicatorInfo, 
		const CPenThickness& penThickness, bool bLog, bool bInvert, CIndicatorList *pIndicatorList,
		const CString& strRQ, CXScaleManager* pxScaleManager, const CSubGraphData& subGraphData, int p_nColumn, CMainBlock *pMainBlock); //��Ʈ ���� ����

	void DrawVerticalBar(CDC* pDC, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, const CRect& barRegion, const int nBarWidth);
	void DrawVerticalBar_BarStyle(CDC* pDC, const CRect& barRegion);
	void DrawVerticalBar_DotBarStyle(CDC* pDC, const CRect& barRegion, const int nBarWidth);
	void DrawVerticalBar_HorzLineBarStyle(CDC* pDC, const CRect& barRegion);
	bool DrawHorizontalBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert);
	// Singbar Graph ���� �񱳱��� �߰� - ojtaso (20071121)
	bool DrawUpDownBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, const CGraphPaintStyle& paintStyle, 
		 const int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		 const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
		 CIndicatorInfo* pIndicatorInfo, const CPenThickness& penThickness, bool bLog, bool bInvert);
	//XScale����
	// Singbar Graph ���� �񱳱��� �߰� - ojtaso (20071121)
	// ���ؼ� �β�, ��� ���� - ojtaso (20071213)
	bool DrawUpDownBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, const CGraphPaintStyle& paintStyle, 
		 const int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		 const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
		 CIndicatorInfo* pIndicatorInfo, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, const CPenThickness& penThickness, const CGraphBaseLineStyle& baseLineStyle, bool bLog, bool bInvert,
		 const CString& strRQ, CXScaleManager* pxScaleManager, CSubGraphData& subGraphData, int p_nColumn);
	//}}
	bool DrawCombBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert);
	bool DrawFixedRateCombBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert);
	bool DrawFixedRateCombBar_Bar(CDC* pDC, CPacket* pPacket, const CRect& graphRegion, 
		const CGraphBaseData::BARDRAWSTYLE eDrawStyle, const int nRealStart, const int nRealEnd, const double& dRealWidth, const int nBarWidth, 
		const double& dViewMin, const double& dViewMax, const bool bLog, const bool bInvert, const CPoint& standardPt, 
		const COLORREF& plusColor, const COLORREF& minusColor, const int nLineThickness, const bool bIsFill, const bool bIsTop);
	void DataTextDraw(CDC* pDC, const double& dData, const CRect& barRegion, const CSize& textSize, const bool bIsTop);
	void DrawDotBar(CDC* pDC, POINT startPt, int nBarWidth);
	bool DrawDotBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		int nBarWidth, double dRealWidth, const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert);
	//XScale����
	bool DrawDotBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		int nBarWidth, double dRealWidth, const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn);
	//}}
	bool DrawUpDownHighLowBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
		CIndicatorInfo* pIndicatorInfo, const CPenThickness& penThickness, bool bLog, bool bInvert);
	void InvertBar(const CRect& graphRegion, CRect& rect);
	bool DrawBoxBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
		CIndicatorInfo* pIndicatorInfo, const CPenThickness& penThickness, bool bLog, bool bInvert);
	bool DrawForwardBoxBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth,	const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const int nCnd, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert);
	bool DrawBackwardBoxBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth,	const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const int nCnd, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert);
	//XScale����
	bool DrawBoxBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
		CIndicatorInfo* pIndicatorInfo, const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn);
	bool DrawForwardBoxBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth,	const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const int nCnd, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn);
	bool DrawBackwardBoxBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const int nBarWidth, const double dRealWidth,	const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const int nCnd, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn);
	//}}
	bool DrawLeftRightBar_Base(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		int nBarWidth, double dRealWidth, const int nRealStart, const int nRealEnd, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, const bool bIsFixedRate);
	bool DrawLeftRightBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		int nBarWidth, double dRealWidth, const int nRealStart, const int nRealEnd, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness);
	bool DrawFixedRateLeftRightBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		int nBarWidth, double dRealWidth, const int nRealStart, const int nRealEnd, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness);
	bool DrawLeftRightCombBar(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		int nBarWidth, double dRealWidth, const int nRealStart, const int nRealEnd, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness);
	bool DrawGroupBar(CDC* pDC, const const CRect& graphRegion, const	CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, const int nBarWidth, const double dRealWidth, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, CIndicatorInfo* pIndicatorInfo,
		const CPenThickness& penThickness, bool bLog, bool bInvert);
	bool DrawRatioBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert);
	// ���Ź� ȣ������
	bool DrawTextHorizontalBar(CDC* pDC, const CRect& graphRegion, 
		const CString& strSubGraphName, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nStartIndex, const int nEndIndex, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, const CIndicatorInfo* pIndicatorInfo, const bool bLog,
		const bool bInvert,const CPacket& pPacket);
	// ���Ź� ȣ������
	void DrawBar_TextHorizontalBar(CDC* pDC, const CGraphDrawStyle& drawStyle, 
		const CRect& graphRegion, const int nBarWidth, const bool bInvert, CPoint& dataPt);
	// ���Ź� ȣ������
//	void DrawText_TextHorizontalBar(CDC* pDC, const CRect& graphRegion, const CPoint& dataPt, const CString& strData);
	void DrawText_TextHorizontalBar(CDC* pDC, const CRect& graphRegion,
		const CPoint& dataPt, const CString& strData,short nWidth);
	// ���Ź� ȣ������
	bool GetDrawingEnvironment_TextHorizontalBar(const CList<CPacket*, CPacket*>& packetList, 
		CList<double, double>*& pDataList, int& nDataCount, 
		double& dHorzViewMin, double& dHorzViewMax, int& nCurrentIndex, int& nHighestIndex) const;
	bool DrawTextGroupBar(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		int nBarWidth, const double dRealWidth, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert);

	void GetMinMaxAndStandard_LeftRightBar(CPacket* pPacket, const bool bIsFixedRate, double& dStandard, double& dMin, double& dMax);
	void GetMinMax_LeftRightBar(const double& dStandard, double& dMin, double& dMax);
	void GetMinMax_FixedRateLeftRightBar(const double& dStandard, double& dMin, double& dMax);

	// Singbar Graph ���� �񱳱��� �߰� - ojtaso (20071121)
	double GetUpDownBarPrevValue(CPacket*& pPacket, POSITION& pos, CList<double, double>*& pList, const int& nStart);
	// Singbar Graph ���� �񱳱��� �߰� - ojtaso (20071121)
	void DrawUpDownBarGraphFill(CDC* pDC, const double& dLeftValue, const double& dRightValue, const CGraphColor& graphColor,
								CBrush& upBrush, CPen& upPen, CBrush& downBrush, CPen& downPen, CBrush& bohapBrush, CPen& bohapPen);

// (2009/1/9 - Seung-Won, Bae) for Vertical Bar Draw
protected:
	void DrawVerticalBar_ReallocCache( CXScaleManager* pxScaleManager, CCalcDataForDraw *&pCalcData, const CGraphBaseData::BARDRAWSTYLE eBarDrawStyle, int nDataCount, int nDTI_Start, int nDTI_End, int p_nColumn, const char *p_szRQ);
	inline BOOL DrawVerticalBar_AddBar( int nLastInsert, CCalcDataForDraw* pCalcData, int &nIndex_High, int &nIndex_Low, int &nIndex_Bohap, const CRect &rect);
	inline void DrawVerticalBar_RemoveOnSamePixel(	LPINT lpPolyCount, int &nCount, LPPOINT lpPoint, int &nIndex, const CRect &rect);
	inline void DrawVerticalBar_AddBarOnPixel( BOOL bLine, LPINT lpPolyCount, DWORD *lpdwcntPoly, int &nCount, LPPOINT lpPoint, int &nIndex, const CRect &rect);
	inline void DrawVerticalBar_NotOverlapDataSetting( CCalcDataForDraw* pCalcData, int nLastInsert, int nDTI, bool &IsOverlap, int nIndex_High=0, int nIndex_Low=0,
													int nIndex_Bohap=0, int nGraphWidth=0, int nTotalDTI=0);
	void DrawVerticalBar_DrawPolygon( CCalcDataForDraw* pCalcData, CDC *pDC, const CGraphColor& graphColor, CBrush *brushArray, CPen *penArray);
	void DrawVerticalBar_DrawPolygon( CDC *pDC, int nFlag, int nCount, LPPOINT lpPoint, LPINT lpPolyCount, DWORD *lpdwcntPoly);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SINGLEBARDRAW_H__2EBE73B1_8773_481F_B8EA_B5F39F03D4F1__INCLUDED_)
