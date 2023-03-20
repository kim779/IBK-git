// CoordinateCalculator.cpp: implementation of the CCoordinateCalculator class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoordinateCalculator.h"

#include <assert.h>

#include "../Include_Analysis/CoordinateCalculatorData.h"

#include "Coordinate.h"
#include "Conversion.h"
#include "DataMath.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CCoordinateCalculator

// ���� ��ǥ(Absolute) -> ��� ��ǥ(Relative) : ������ ���� ��ǥ
CCoordinate CCoordinateCalculator::AbsoluteToRelative(const CCoordinateCalculatorData& calData, const CPoint& point) const
{
	CCoordinate cdResult;
	IPacket *pIPacket = calData.GetPacket_X();
	if( !IsCalculator( pIPacket)) 
	{
		cdResult = CCoordinate(point);
	}
	else
	{
		// XScale : ���� Interface�� ��� - vntsorl(20070327)
//		int index = CDataConversion::GetPacketDataIndex_Date( calData.GetRegion(), point, 
//					calData.GetDisplayDataStart(), calData.GetDisplayDataEnd());

		//>> (2008/12/26 - Seung-Lyong, Park) �޸𸮸� ����
//		int nIndex = calData.GetIChartManager()->FindViewDataIndex_Pt("DEFAULT", point.x);
		//<< (2008/12/26 - Seung-Lyong, Park) �޸𸮸� ����
		
		double x = 0.0;
//		pIPacket->GetDataPastandFuture(nIndex, x);
		x = GetData_X(calData, point, 0);
		double temp = CDataConversion::AbsoluteToRelative_Y(calData.GetRegion(), point.y, 
					calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType());

		// (2007/5/10 - Seung-Won, Bae) Check No Y Scale Min/Max.
		if( -HUGE_VAL == temp) cdResult = CCoordinate(point);
		else
		{
			double y = calData.ApplyExp(temp);
			// 20081007 JS.Kim	����ڽð��� ����
			cdResult = CCoordinate(x, y, pIPacket->GetBaseTimeDifference());
		}
	}
	if( pIPacket) pIPacket->Release();
	return cdResult;
}

// ��� ��ǥ -> ���� ��ǥ : ����� ��ǥ�� �̿��� �׸������� ��ǥ
CPoint CCoordinateCalculator::RelativeToAbsolute(const CCoordinateCalculatorData& calData, const CCoordinate& point) const
{
	// (2007/5/10 - Seung-Won, Bae) Check No Y Scale Min/Max.
	if( -HUGE_VAL == calData.GetDisplayDataMax_ScaleType() || -HUGE_VAL == calData.GetDisplayDataMin_ScaleType()) return point.GetPixel();

	CPoint ptResult;
	IPacket *pIPacket = calData.GetPacket_X();
	if(!IsCalculator( pIPacket)) 
	{
		ptResult = point.GetPixel();
	}
	else
	{
		int nCenter_x;
		double dX = 0.0;
		dX = point.GetX();
		// 20081007 JS.Kim	����ڽð��� ����<<
		int nPckTimeDiff = pIPacket->GetBaseTimeDifference();
		if( nPckTimeDiff != point.GetTimeDiff() )
			dX = pIPacket->CalcTimeDiff(dX, nPckTimeDiff - point.GetTimeDiff());
		//>>

		if( calData.IsValidAddInInterface())
		{
			int nXPos = calData.m_pIChartManager->GetXFromDouble(calData.GetBlockIndex().GetRow(), calData.GetBlockIndex().GetColumn(), dX, nCenter_x);
			int nIndex = calData.m_pIChartManager->FindViewDataIndex_Pt("DEFAULT", nXPos);
			ptResult = CDataConversion::RelativeToAbsolute(calData.GetRegion(), 
					calData.GetDisplayDataStart(), calData.GetDisplayDataEnd(), 
					calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType(), 
					nIndex,	calData.ApplyLog(point.GetY()));
			ptResult.x = nXPos;
		}
		//<< (2008/12/26 - Seung-Lyong, Park) �޸𸮸� ����
	}
	if( pIPacket) pIPacket->Release();
	return ptResult;
}

// ���� ��ǥ -> ���� ��ǥ : �׸��� ���� ��ǥ
CPoint CCoordinateCalculator::AbsoluteToAbsolute(const CCoordinateCalculatorData& calData, const CPoint& point) const
{
	CPoint ptResult;
	IPacket *pIPacket = calData.GetPacket_X();
	if( !IsCalculator( pIPacket)) ptResult = point;
	else
	{
		CCoordinate relativePt = AbsoluteToRelative(calData, point);
		CPoint drawingPoint = RelativeToAbsolute(calData, relativePt);
		ptResult = drawingPoint;
	}
	if( pIPacket) pIPacket->Release();
	return ptResult;
}

// �������� ����
CCoordinate CCoordinateCalculator::GetGap(const CCoordinateCalculatorData& calData, const CPoint& point1, const CPoint& point2) const
{
//	CCoordinate cdResult;
//	// packet �� ���� ��쿣 �׳� ��ǥ�� �Ѵ�!!
//	IPacket *pIPacket = calData.GetPacket_X();
//	if(!IsCalculator( pIPacket)) cdResult = CCoordinate((double)(point2.x - point1.x), (double)(point2.y - point1.y));
//	else
//	{
//		CCoordinate relativePt1 = AbsoluteToRelative(calData, point1);
//		CCoordinate relativePt2 = AbsoluteToRelative(calData, point2);
//		int nIndex = GetDisplayDataCurrentIndex( pIPacket, (int)relativePt2.GetX()) - GetDisplayDataCurrentIndex( pIPacket, (int)relativePt1.GetX());
//		cdResult = CCoordinate((double)nIndex, (relativePt2.GetY() - relativePt1.GetY()));
//	}
//	if( pIPacket) pIPacket->Release();
//	return cdResult;

	CCoordinate cdResult;
	// packet �� ���� ��쿣 �׳� ��ǥ�� �Ѵ�!!
	IPacket *pIPacket = calData.GetPacket_X();
	// 20081007 JS.Kim	����ڽð��� ����
	if( !IsCalculator( pIPacket)) cdResult = CCoordinate((double)(point2.x - point1.x), (double)(point2.y - point1.y), pIPacket->GetBaseTimeDifference());
	else
	{
		// XScale : ���� Interface�� ��� - vntsorl(20070327)
		CCoordinate AbsolutePt1 = AbsoluteToAbsolute(calData, point1);
		CCoordinate AbsolutePt2 = AbsoluteToAbsolute(calData, point2);
		CCoordinate relativePt1 = AbsoluteToRelative(calData, point1);
		CCoordinate relativePt2 = AbsoluteToRelative(calData, point2);
		cdResult = CCoordinate((AbsolutePt2.GetX() - AbsolutePt1.GetX()), (relativePt2.GetY() - relativePt1.GetY()), pIPacket->GetBaseTimeDifference());
	}
	if( pIPacket) pIPacket->Release();
	return cdResult;
}
/*
// ���� point�� index�� ��������.
int CCoordinateCalculator::GetCurrentDataIndex(const CRegion& region, const CPoint& point) const
{
	return CDataConversion::GetPacketDataIndex_Date(region.GetDrawingRegion(), point, 
			region.GetDisplayAttributes().GetDataStartInBlock(), 
			region.GetDisplayAttributes().GetDataEndInBlock());
}

// �׸� x��ǥ ��������.
int CCoordinateCalculator::GetDrawingPoint_X(const CRegion& region, const int nCurrentIndex) const
{
	return CDataConversion::RelativeToAbsolute_X(region.GetDrawingRegion(), 
			region.GetDisplayAttributes().GetDataStartInBlock(), 
			region.GetDisplayAttributes().GetDataEndInBlock(), nCurrentIndex);
}

*/

double CCoordinateCalculator::GetData_X(const CCoordinateCalculatorData& calData, const CPoint& point, const int /*nIndex*/) const
{
	int nBlockColumn, nCenter_x;
	int nRow, nCol;
	double dReturn = 0;
	if( calData.IsValidAddInInterface())
	{
		calData.m_pIChartManager->GetBlockIndex(point, nRow, nCol);
		dReturn = calData.m_pIChartManager->GetDoubleFromX(nRow, nCol, point.x, &nBlockColumn, nCenter_x);
	}
	return dReturn;
}

// private ====================================================================
bool CCoordinateCalculator::IsCalculator(IPacket* pPacket) const
{
	return (pPacket != NULL && pPacket->GetDataCount() > 0);
}

// ----------------------------------------------------------------------------
int CCoordinateCalculator::GetDisplayDataCurrentIndex(IPacket* pPacket, const int nData) const
{
	if(pPacket == NULL)
		return -1;
	
	return pPacket->GetIndexFromData(nData);
}

///////////////////////////////////////////////////////////////////////////////
// class CSelectableRegionCalculator


CSelectableRegionCalculator::CSelectableRegionCalculator():
	m_nProximityRange(4)
{
}

// ������ ������ ����
int CSelectableRegionCalculator::GetProximityRange() const
{
	return m_nProximityRange;
}

// ������ �մ� ���� �θ��� ����.
void CSelectableRegionCalculator::GetLineRegion(const CPoint& point1, const CPoint& point2, CRgn& region) const
{
	// ����
	if(point1.y == point2.y){
		CRect temp = GetHorzLineRegion(point1, point2);
		region.CreateRectRgn(temp.left, temp.top, temp.right, temp.bottom);
	}
	// ������
	else if(point1.x == point2.x){
		CRect temp = GetVertLineRegion(point1, point2);
		region.CreateRectRgn(temp.left, temp.top, temp.right, temp.bottom);
	}
	// �缱
	else
		GetAngleLineRegion(point1, point2, region);
}

// ����: origin, ������: l�� ���� point�� �ִ��� �˻�
bool CSelectableRegionCalculator::IsPointInCircle(const CPoint& origin, const CPoint& point) const
{
	return (pow(m_nProximityRange*2, 2) >= (pow(origin.x - point.x, 2) + pow(origin.y - point.y, 2)));
}

bool CSelectableRegionCalculator::IsPointInCircle(const CPoint& origin, const int nRadius, const CPoint& point) const
{
	return (pow(nRadius, 2) >= (pow(origin.x - point.x, 2) + pow(origin.y - point.y, 2)));
}


// private ====================================================================
// ����
CRect CSelectableRegionCalculator::GetHorzLineRegion(const CPoint& point1, const CPoint& point2) const
{
	assert(point1.y == point2.y);

	CRect selectableRegion(point1.x, (point1.y - m_nProximityRange), point2.x, (point1.y + m_nProximityRange +1));
	selectableRegion.NormalizeRect();

	return selectableRegion;
}

// ������
CRect CSelectableRegionCalculator::GetVertLineRegion(const CPoint& point1, const CPoint& point2) const
{
	assert(point1.x == point2.x);

	CRect selectableRegion((point1.x - m_nProximityRange), point1.y, (point1.x + m_nProximityRange +1), point2.y);
	selectableRegion.NormalizeRect();

	return selectableRegion;
}

// �缱
void CSelectableRegionCalculator::GetAngleLineRegion(const CPoint& point1, const CPoint& point2, CRgn& region) const
{
	assert(point1.x != point2.x && point1.y != point2.y);

	// (0, 0)�� �������� �� ProximityRange ������ �ΰ��� ��ǥ ���ϱ�.
	CPoint basePt1, basePt2;
	GetVerticalPointsFromLength(point1, point2, basePt1, basePt2);

	POINT selectableRegionPoints[4];
	selectableRegionPoints[0] = GetPointOnMovedOrigin(point1, basePt1);
	selectableRegionPoints[1] = GetPointOnMovedOrigin(point1, basePt2);
	selectableRegionPoints[2] = GetPointOnMovedOrigin(point2, basePt2);
	selectableRegionPoints[3] = GetPointOnMovedOrigin(point2, basePt1);

	region.CreatePolygonRgn(selectableRegionPoints, 4, ALTERNATE);
}

// ----------------------------------------------------------------------------
// ��ǥ �̵�
POINT CSelectableRegionCalculator::GetPointOnMovedOrigin(const CPoint& origin, const CPoint& point) const
{
	POINT movePoint;
	movePoint.x = origin.x + point.x;
	movePoint.y = origin.y + point.y;

	return movePoint;
}

// Ư�� ���� ��ŭ ������ ��ǥ ������
// --> ���Ⱑ a �̰� (0, 0)�� ������ (0, 0)���� Ư������ +- L ������ ��ǥ ���ϱ�.
// --> x1 = k, y1 = a*k,    x2 = -k, y2 = a*-k
// --> k = |(L*L)/(a*a + 1)| �� ��Ʈ��
void CSelectableRegionCalculator::GetVerticalPointsFromLength(const CPoint& pt1, const CPoint& pt2, CPoint& selectableRegionPt1, CPoint& selectableRegionPt2) const
{
	// ������ �������� ���Ѵ�. == ���� ���� �� ��ǥ�� ����
	double lineAngle = GetLineVerticalAngle(GetLineAngle(pt1, pt2));

	double x = 0.0;
	x = sqrt(fabs(pow(m_nProximityRange, 2) / (pow(lineAngle, 2) +1)));

	selectableRegionPt1 = CPoint(CMath::Round(x), CMath::Round(lineAngle * x));
	selectableRegionPt2 = CPoint(-selectableRegionPt1.x, -selectableRegionPt1.y);
}

// ----------------------------------------------------------------------------
// ��ǥ�� ����.
// --> ������ ���� : (y2-y1)/(x2-x1)
double CSelectableRegionCalculator::GetLineAngle(const CPoint& point1, const CPoint& point2) const
{
	return ((double)(point2.y - point1.y) / (double)(point2.x - point1.x));
}

// ������ ����.
// --> -1/a
double CSelectableRegionCalculator::GetLineVerticalAngle(const double& dAngle) const
{
	assert(dAngle != 0.0);
	return (-1.0/dAngle);
}