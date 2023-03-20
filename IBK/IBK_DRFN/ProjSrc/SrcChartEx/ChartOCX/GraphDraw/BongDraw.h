// BongDraw.h: interface for the CBongDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BONGDRAW_H__8208750A_D1E4_4534_BA08_CA9BCA8BC476__INCLUDED_)
#define AFX_BONGDRAW_H__8208750A_D1E4_4534_BA08_CA9BCA8BC476__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Draw.h"

class CIndicatorInfo;
class CXScaleManager;
class CCalcDataForDraw;

class CBongDraw : public CDraw  
{
public:
	CBongDraw();
	virtual ~CBongDraw();

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

	// (2004.04.01, ��¿�) Default Indicator List�� Static���� �����Ͽ�, �ݺ� Loading�� �����Ѵ�.
	//		Static���� CIndicatorList�� Pointer�� �������� �ʴ´�. (CIndicatorList�� Static�� �ƴϴ�.)
	//		CIndicatorInfo�� �������� �ʰ� CIndicatorList�� ������ �� �ֵ��� �Ѵ�.
//	int GetBohapTypeInPriceChart(CIndicatorInfo* pIndicatorInfo, CIndicatorList *pIndicatorList) const;

private:
	enum ENUM_BRUSH
	{
		UP_BRUSH,
		DOWN_BRUSH,

		BRUSH_COUNT
	};
	
	enum ENUM_PEN
	{
		STANDARD_PEN,
		UP_PEN,
		DOWN_PEN,
		BOHAP_PEN,

		PEN_COUNT
	};

private:
// (2006.11.03 �Ƚ�ȯ) Flow��Ʈ �߰�
	int		nPtCount;
	CPoint* pFlowPt;
// (2006.11.03 �Ƚ�ȯ) Flow��Ʈ �߰�
	bool DrawCandlestickBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType, 
			const double& dViewMax, const double& dViewMin,	const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw);
	// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
	bool DrawCandlestickBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType, 
			const double& dViewMax, const double& dViewMin,	const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw,
			const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn,
			const CObArray& arrInfo);
	// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
	bool DrawEquiVolumeBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType, 
			const double& dViewMax, const double& dViewMin,	const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw,
			const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn,
			const CObArray& arrInfo);
// (2006.10.30 �Ƚ�ȯ) ����Ʈ(��'��'��)�߰�
//	bool DrawHighLowCloseBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor,
	bool DrawOpenHighLowCloseBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor,
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType,
			const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw);
	// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
	bool DrawOpenHighLowCloseBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor,
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType,
			const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw,
			const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn,
			const CObArray& arrInfo);
	bool DrawNewHighLowCloseBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor,
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType,
			const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw);
	// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
	bool DrawNewHighLowCloseBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor,
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType,
			const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw,
			const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn,
			const CObArray& arrInfo);
// (2006.10.30 �Ƚ�ȯ) ����Ʈ(��'��'��)�߰�
	bool DrawAntennaBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType, 
			const double& dViewMax, const double& dViewMin,	const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw);
	// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
	bool DrawAntennaBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType, 
			const double& dViewMax, const double& dViewMin,	const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw,
			const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn,
			const CObArray& arrInfo);
	bool DrawSolidBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
			const double dRealWidth, const int nRealStart, const int nRealEnd, 
			const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw, BOOL p_bWithBaseText,
			bool bBaseLine);	// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
// (2006.11.03 �Ƚ�ȯ) Flow��Ʈ �߰�
	// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
	bool DrawSolidBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
			const double dRealWidth, const int nRealStart, const int nRealEnd, 
			const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList, 
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw,BOOL p_bWithBaseText, bool bBaseLine, // ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
			const CString& strRQ, CXScaleManager* pxScaleManager, CSubGraphData& subGraphData, int p_nColumn,
			const CObArray& arrInfo);
	bool DrawFlowBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor,
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType,
			const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw);
// (2006.11.03 �Ƚ�ȯ) Flow��Ʈ �߰�
	// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
	bool DrawFlowBong(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor,
			const double dRealWidth, const int nRealStart, const int nRealEnd, const int nBohapType,
			const double& dViewMax, const double& dViewMin, const CList<CPacket*, CPacket*>& packetList,
			const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw,
			const CString& strRQ, CXScaleManager* pxScaleManager, int p_Column,
			const CObArray& arrInfo);
//	void DrawComparisonLine(CDC *pDC, const CRect *graphRegion, const double dRealWidth, const double dRealMin, const double dRealMax, const double& dViewMin, const double& dViewMax, CPacket* pClosePacket);

	void SelectGdiObject(CDC* pDC, const int nBohapType, const double& dOpen, const double& dClose, 
		const double& dPreClose, const CGraphColor& graphColor, CBrush* pBrush, CPen* pPen);
	void SelectGdiObject_HighLow(CDC* pDC, const bool bFill, const int nBrushIndex, 
		const int nPenIndex, CBrush* pBrush, CPen* pPen);
	void SelectGdiObject_Bohap(CDC* pDC, const int nBohapType, const double& dClose, 
		const double& dPreClose, CPen* pPen);
	void Draw_SolidBong(CDC* pDC, const CPoint& curPt, const int &nBase, const double& dRealWidth, CPen* pPen);
	void Draw_SolidBong(CDC* pDC, const CPoint& prePt, const CPoint& curPt, const int &nBase, CPen* pPen);

	void InitGdiObject(CBrush* pBrush, CPen* pPen, const CGraphColor& graphColor, 
		const double& dWidth, const CPenThickness& penThickness, const bool bIsDrawingThickness);
	void DestroyGdiObject(CDC* pDC, CBrush* pBrush, CPen* pPen, CBrush* pOldBrush, CPen* pOldPen);

	bool GetPacketData_Antenna(const CList<CPacket*, CPacket*>& packetList, const int nDataIndex, 
		CList<double, double>*& pPoleHighList, CList<double, double>*& pPoleLowList, 
		CList<double, double>*& pData1List, CList<double, double>*& pData2List, 
		POSITION& poleHighPos, POSITION& poleLowPos, POSITION& data1Pos, POSITION& data2Pos) const;
	bool GetPacketData(CPacket* pPacket, const int nDataIndex, CList<double, double>*& pDataList, POSITION& dataPos) const;
	CPoint GetVerticalPoint_Antenna(const int nMaxPos_x, const int nGap, const CRect& region, const int nIndex, 
		const double& dWidth, const double& dViewMin, const double& dViewMax, const double& dData) const;

	inline void DrawSolidBong_Append( int curX, int curY, int *pnCount, DWORD *lpdwcntPoly, int *pnIdxPoint, LPPOINT lpPoint, BOOL p_bWithNew = FALSE);

public:
	static int GetBohapType(CIndicatorInfo* pIndicatorInfo);
};

#endif // !defined(AFX_BONGDRAW_H__8208750A_D1E4_4534_BA08_CA9BCA8BC476__INCLUDED_)
