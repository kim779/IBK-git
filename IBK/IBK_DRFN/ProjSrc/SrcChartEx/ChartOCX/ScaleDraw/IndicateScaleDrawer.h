// IndicateScaleDrawer.h: interface for the CIndicateScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATESCALEDRAWER_H__A88925E6_00A8_4E77_AABF_0720EDAB9496__INCLUDED_)
#define AFX_INDICATESCALEDRAWER_H__A88925E6_00A8_4E77_AABF_0720EDAB9496__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VertScaleDrawer.h"
#include "PriceScaleDrawer.h"

class CDrawingVertScaleData;
class CScaleDrawingRegion;
class CDisplayDataMinMax;
class CIndicatorInfo;
class CPacket;

class CIndicateScaleDrawer : public CPriceScaleDrawer
{
public:
	CIndicateScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

public:
	virtual void DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale);
	virtual void DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale);

private:
	void DrawTextOrGrid_BaseLine(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CRect& scaleRegion, const CVerticalScale& vertScale, 
		const bool bIsLeft, CList<double, double>* pBaseLineList);
	void DrawTextOrGrid_Value(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CRect& scaleRegion, const CVerticalScale& vertScale);

	// ���ؼ� ���� �����´�.
	CList<double, double>* GetBaseLineList(CIndicatorInfo* pIndicatorInfo);
	// packet data type ��������
	CString GetPacketDataType(CPacket* pPacket, const CDisplayDataMinMax& displayDataMinMax) const;
	// type�� ���� �� ����
	double GetChangeData(const CString& strPacketType, const double& orgData) const;
	// y ��ǥ �����´�.
	int GetVertScaleAbsolutePosition_Y(const int absolute_Org, const int absolute_L, 
			const double& relative_Org, const double& relative_L, const double& relative_y) const;
	
	// draw �� �������� üũ
	bool IsPointInRegion(const CRect& region, const int y) const;
};

#endif // !defined(AFX_INDICATESCALEDRAWER_H__A88925E6_00A8_4E77_AABF_0720EDAB9496__INCLUDED_)
