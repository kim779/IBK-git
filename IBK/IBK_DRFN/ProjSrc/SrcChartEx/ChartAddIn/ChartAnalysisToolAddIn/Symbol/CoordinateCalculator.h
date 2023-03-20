// CoordinateCalculator.h: interface for the CCoordinateCalculator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COORDINATECALCULATOR_H__4514D7F6_F1A7_4081_B2BA_BE4243553ED1__INCLUDED_)
#define AFX_COORDINATECALCULATOR_H__4514D7F6_F1A7_4081_B2BA_BE4243553ED1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h

class CCoordinate;
class CCoordinateCalculatorData;
class CRegion;

// ����/��� ��ǥ�� ����
class CCoordinateCalculator
{
public:
	// ���� ��ǥ(Absolute) -> ��� ��ǥ(Relative) : ������ ���� ��ǥ
	CCoordinate AbsoluteToRelative(const CCoordinateCalculatorData& calData, const CPoint& point) const;
	// ��� ��ǥ -> ���� ��ǥ : ����� ��ǥ�� �̿��� �׸������� ��ǥ
	CPoint RelativeToAbsolute(const CCoordinateCalculatorData& calData, const CCoordinate& coordinate) const;
	// ���� ��ǥ -> ���� ��ǥ : �׸��� ���� ��ǥ
	CPoint AbsoluteToAbsolute(const CCoordinateCalculatorData& calData, const CPoint& point) const;

	// �������� ����
	CCoordinate GetGap(const CCoordinateCalculatorData& calData, const CPoint& point1, const CPoint& point2) const;
	// ���� point�� index�� ��������.
//	int GetCurrentDataIndex(const CRegion& region, const CPoint& point) const;
	// �׸� x��ǥ ��������.
//	int GetDrawingPoint_X(const CRegion& region, const int nCurrentIndex) const;
	double GetData_X(const CCoordinateCalculatorData& calData, const CPoint& point, const int /*nIndex*/) const;

private:
	bool IsCalculator(IPacket* pPacket) const;

	int GetDisplayDataCurrentIndex(IPacket* pPacket, const int nData) const;
};


// element ������ ���� �ʿ��� ������ �������� ���� class
class CSelectableRegionCalculator
{
public:
	CSelectableRegionCalculator();

	// ������ ������ ����
	int GetProximityRange() const;

	// ������ �մ� ���� �θ��� ����.
	void GetLineRegion(const CPoint& point1, const CPoint& point2, CRgn& region) const;
	// ����: origin, ������: l�� ���� point�� �ִ��� �˻�
	bool IsPointInCircle(const CPoint& origin, const CPoint& point) const;
	bool IsPointInCircle(const CPoint& origin, const int nRadius, const CPoint& point) const;

private:
	int m_nProximityRange;

	// ����
	CRect GetHorzLineRegion(const CPoint& point1, const CPoint& point2) const;
	// ������
	CRect GetVertLineRegion(const CPoint& point1, const CPoint& point2) const;
	// �缱
	void GetAngleLineRegion(const CPoint& point1, const CPoint& point2, CRgn& region) const;

	// ��ǥ �̵�
	POINT GetPointOnMovedOrigin(const CPoint& origin, const CPoint& point) const;
	// ����ǥ�� �������� Ư�� ���� ��ŭ ������ ��ǥ.
	void GetVerticalPointsFromLength(const CPoint& pt1, const CPoint& pt2, CPoint& selectableRegionPt1, CPoint& selectableRegionPt2) const;

	// ��ǥ�� ����.
	double GetLineAngle(const CPoint& point1, const CPoint& point2) const;
	// ������ ����.
	double GetLineVerticalAngle(const double& dAngle) const;
};

#endif // !defined(AFX_COORDINATECALCULATOR_H__4514D7F6_F1A7_4081_B2BA_BE4243553ED1__INCLUDED_)
