// Drawer.h: interface for the CDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWER_H__95DCC1FE_A971_4570_B22B_E9472DA769D5__INCLUDED_)
#define AFX_DRAWER_H__95DCC1FE_A971_4570_B22B_E9472DA769D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"

class CDrawingRegions;
class CRegion;
class CPoints;

class CDrawer  
{
public:
	CDrawer();

	void SetGripBrushColor(const COLORREF& color);

	void DrawRectangle(CDC* pDC, const CRect& region);

// (2004.12.09, ��¿�) Public���� �����Ͽ� �ܺ� ȣ�� Routine���� �̸� �̿��� �� �ֵ��� �Ѵ�.
public:
	enum INTERSECTION_RESULT
	{
		NO_RESULT = -1,
		FALSE_RESULT, // ���� ����.
		TRUE_RESULT	  // ���� ����.
	};

	enum TEXTPOSITION
	{
		LEFT = 0,
		RIGHT,
		TOP,
		BOTTOM,
		// (2004.12.09, ��¿�) �밢�� ������ TEXT ������ �����Ѵ�.
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT
	};

protected:
	void SelectClipRegion(CDC* pDC, const CRect& rect);
	void SelectClipRegion(CDC* pDC);
	void SelectNewDrawingMode(CDC* pDC, const CRect& rect, const int nDrawMode);
	void SelectOrgDrawingMode(CDC* pDC);
	void SelectNewTextMode(CDC* pDC, const COLORREF& textColor);
	void SelectOrgTextMode(CDC* pDC);

	void DrawPolygonLine(CDC* pDC, const LPPOINT pPoint, const int& nCount);
	void DrawLine(CDC* pDC, const CPoint& moveToPt, const CPoint& lineToPt);
	void DrawLineExt(CDC* pDC, const CPoint& moveToPt, const CPoint& lineToPt,BOOL bLeft,BOOL bRight);
	void DrawFrameRect(CDC* pDC, const CRect& region);
	void DrawFrameRect(CDC* pDC, const CPoint& moveToPt, const CPoint& lineToPt);
	void DrawRect(CDC* pDC, const CRect& region, const COLORREF& color, const bool bIsFill = true);
	void DrawRectangle(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor, const bool bIsFill = true);
	void DrawTextOut(CDC* pDC, const TEXTPOSITION eTextPosition, const CString& data, const CPoint& point, const CSize& maxSize = CSize(0, 0));
	void DrawTextOut(CDC* pDC, const CString& data, const CRect& region, const COLORREF& textColor);
	void DrawTextOut(CDC* pDC, const CString& data, const CRect& region);
	void DrawTextOut_Path(CDC* pDC, const CString& data, const CPoint& point);
	void DrawStyleLine(CDC* pDC,  const CPoint& moveToPt, const CPoint& lineToPt, const UINT nStryle);	// 2014.09.24 �ڼ��� : 0010862: [7111] �Ｚ���� ��Ʈ ������ [�ϸ��ġ] ��� IBK��Ʈ���� �����ϰ� �߰� ��û

	// grip ��.
	COLORREF GetGripBrushColor() const;
	// grip ����.
	CRect GetGripRegion(const CPoint& centerPoint) const;
	// text ����.
	CRect GetTextRegion(const CSize& textSize, const CPoint& orgPoint = CPoint(0, 0), const bool bLeft = true, const bool bTop = true) const;

	// ������ ���ϱ�.
	CPoint GetLineEndPoint(const CPoint& line1_pt1, const CPoint& line1_pt2, const CPoint& line2_pt1, const CPoint& line2_pt2) const;
	bool Intersection(const CPoint& line1_pt1, const CPoint& line1_pt2, const CPoint& line2_pt1, const CPoint& line2_pt2, CPoint& point) const;
	
	// ���� �� ���� ���ϱ�.
	double GetLineAngle(const CPoint& point1, const CPoint& point2) const;
	double GetLineIntercept(const double& dAngle, const CPoint& point) const;

	// ���� point�� index�� ��������.
	int GetCurrentDataIndex(const CRegion& region, const CPoint& point) const;
	// �׸� x��ǥ ��������.
	int GetDrawingPoint_X(const CRegion& region, const int nIndex, const bool bIsCenter = true) const;

private:
	// ������ ����. X = a
	bool IsVerticalLine(const CPoint& point1, const CPoint& point2) const;
	// ���� ����. Y = a
	bool IsHorizontalLine(const CPoint& point1, const CPoint& point2) const;

	// ���� ���ϱ�.
	// -- ����
	CDrawer::INTERSECTION_RESULT IsIntersectionPoint_InHorzLine(const CPoint& line1_pt1, const CPoint& line1_pt2, 
		const CPoint& line2_pt1, const CPoint& line2_pt2, CPoint& point) const;
	CDrawer::INTERSECTION_RESULT GetIntersectionPoint_InHorzLine1(const CPoint& line1_pt1, const CPoint& line1_pt2, 
		const CPoint& line2_pt1, const CPoint& line2_pt2, CPoint& point) const;
	// -- ������
	CDrawer::INTERSECTION_RESULT IsIntersectionPoint_InVertLine(const CPoint& line1_pt1, const CPoint& line1_pt2, 
		const CPoint& line2_pt1, const CPoint& line2_pt2, CPoint& point) const;
	CDrawer::INTERSECTION_RESULT GetIntersectionPoint_InVertLine1(const CPoint& line1_pt1, const CPoint& line1_pt2, 
		const CPoint& line2_pt1, const CPoint& line2_pt2, CPoint& point) const;
	// -- �缱
	bool GetIntersectionPoint_Slant(const CPoint& line1_pt1, const CPoint& line1_pt2, 
		const CPoint& line2_pt1, const CPoint& line2_pt2, CPoint& point) const;
	bool Result(const CDrawer::INTERSECTION_RESULT result) const;

	bool Intersection(double x1, double y1, double x2, double y2,
		double bx1, double by1, double bx2, double by2, double &ix, double &iy) const;
	bool IsSame(double x1, double y1, double x2, double y2) const;

	// text ��ġ
	CPoint GetTextPosition(const CDrawer::TEXTPOSITION eTextPosition, const CPoint& orgPoint, const CSize& textSize, const CSize& maxSize = CSize(0, 0)) const;

private:
	const int nGripSize;
	COLORREF m_gripBrushColor;

	int m_nOrgDrawMode;
	int m_nOrgBkMode;
	COLORREF m_orgTextColor;
};


#endif // !defined(AFX_DRAWER_H__95DCC1FE_A971_4570_B22B_E9472DA769D5__INCLUDED_)
