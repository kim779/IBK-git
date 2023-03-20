// ScaleDrawerImplementation.h: interface for the CScaleDrawerImplementation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALEDRAWERIMPLEMENTATION_H__8238BC9A_9759_4C56_A122_78BF69713A3A__INCLUDED_)
#define AFX_SCALEDRAWERIMPLEMENTATION_H__8238BC9A_9759_4C56_A122_78BF69713A3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"

class CDrawingHorzScaleData;
class CDrawingVertScaleData;
class CHorizontalScale;
class CHorzScaleRegion;
class CVerticalScale;
class CVertScaleRegion;
class CScaleCompart;
class CScaleDrawer;
class CScaleDrawingRegion;
class CXScaleManager;

class CScaleDrawerImplementation  
{
// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
protected:
	HWND	m_hOcxWnd;

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE	m_eChartMode;

public:
	CScaleDrawerImplementation( HWND p_hOcxWnd, CChartInfo::CHART_MODE	p_eChartMode)
		{	m_hOcxWnd = p_hOcxWnd;	m_eChartMode = p_eChartMode;	}
	virtual ~CScaleDrawerImplementation();

	// ���� scale �׸���.
	void DrawHorzTextOrGrid(CDC* pDC, 
				const CDrawingHorzScaleData& drawingHorzScaleData,
				const CHorzScaleRegion& horzScaleRegion, // ���� scale�� ����
				const CHorizontalScale& horzScale,		 // ���� scale�� ���� ��
				CXScaleManager* pxScaleManager,
				const char *p_szRQ);

	// ���� scale �׸���.
	void DrawVertTextOrGrid(CDC* pDC, 
				const CDrawingVertScaleData& drawingVertScaleData,
				const CVertScaleRegion& horzScaleRegion, // ���� scale�� ����
				const CVerticalScale& horzScale);		 // ���� scale�� ���� ��

private:
	// ���� scale text�� �׸���.
	void DrawHorzTextOrGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, 
		const CRect& scaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager,
				const char *p_szRQ);
	void DrawHorzTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, 
		const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager,
				const char *p_szRQ);
	void DrawHorzGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, 
		const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, const char *p_szRQ);

	// ���� scale text�� �׸���.
	void DrawVertTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, 
		const CScaleDrawingRegion& scaleDrawingRegion, const CVerticalScale& vertScale);
	void DrawVertTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, 
		const CRect& scaleRegion, const CVerticalScale& vertScale);
	void DrawVertTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, 
		const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale);
};

#endif // !defined(AFX_SCALEDRAWERIMPLEMENTATION_H__8238BC9A_9759_4C56_A122_78BF69713A3A__INCLUDED_)
