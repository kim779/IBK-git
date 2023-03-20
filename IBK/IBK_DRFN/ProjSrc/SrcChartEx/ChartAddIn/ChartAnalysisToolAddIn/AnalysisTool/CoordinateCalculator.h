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
	// ���� ��ǥ(Absolute) -> ��� ��ǥ(Relative) : ���� ������ ��ġ ������ ���� ��ǥ
	CCoordinate AbsoluteToRelativeAutoPrice(const CCoordinateCalculatorData& calData, const CPoint& point) const;
	// ��� ��ǥ -> ���� ��ǥ : ����� ��ǥ�� �̿��� �׸������� ��ǥ
	CPoint RelativeToAbsolute(const CCoordinateCalculatorData& calData, const CCoordinate& coordinate) const;	
	int RelativeToAbsolute_Y(const CCoordinateCalculatorData& calData,int nIndex,const double& dYValue) const;
	// ���� ��ǥ -> ���� ��ǥ : �׸��� ���� ��ǥ
	CPoint AbsoluteToAbsolute(const CCoordinateCalculatorData& calData, const CPoint& point) const;

	// �������� ����
	CCoordinate GetGap(const CCoordinateCalculatorData& calData, const CPoint& point1, const CPoint& point2) const;
	// ���� point�� index�� ��������.
	int GetCurrentDataIndex(const CRegion& region, const CPoint& point) const;
	// �׸� x��ǥ ��������.
	int GetDrawingPoint_X(const CRegion& region, const int nCurrentIndex) const;

protected:
	virtual double GetData_X(const CCoordinateCalculatorData& calData, const CPoint& point, const int nIndex) const = 0;	// XScale : ���� Interface�� ��� - vntsorl(20070410)
	virtual double GetData_Y(const CCoordinateCalculatorData& calData, const CPoint& point, const int nIndex) const = 0;
	virtual double GetData_Y(const CCoordinateCalculatorData& calData, const double& dData, const int nIndex) const = 0;

private:
	bool IsCalculator(IPacket* pPacket) const;

	// ����Ʈ �����ð� ���� - ojtaso (20080811)
	int GetDisplayDataCurrentIndex(IPacket* pPacket, const double dData, const CString strType = _T("")) const;
public:
	CPoint RelativeToAbsolute_NoAutoPrice(const CCoordinateCalculatorData& calData, const CCoordinate& point) const;
};

class CCoordinateCalculator_AutoPrice : public CCoordinateCalculator
{
protected:
	virtual double GetData_X(const CCoordinateCalculatorData& calData, const CPoint& point, const int nIndex) const;	// XScale : ���� Interface�� ��� - vntsorl(20070410)
	virtual double GetData_Y(const CCoordinateCalculatorData& calData, const CPoint& point, const int nIndex) const;
	virtual double GetData_Y(const CCoordinateCalculatorData& calData, const double& dData, const int nIndex) const;

private:
	bool IsAutoPriceCalculator(IPacket* pPacket, const int nIndex) const;
};

class CCoordinateCalculator_NoAutoPrice : public CCoordinateCalculator
{
protected:
	virtual double GetData_X(const CCoordinateCalculatorData& calData, const CPoint& point, const int nIndex) const;	// XScale : ���� Interface�� ��� - vntsorl(20070410)
	virtual double GetData_Y(const CCoordinateCalculatorData& calData, const CPoint& point, const int nIndex) const;
	virtual double GetData_Y(const CCoordinateCalculatorData& calData, const double& dData, const int nIndex) const;
};

class CCoordinateCalculatorFinder
{
public:
	static CCoordinateCalculator& FindCoordinateCalculator(const bool bIsAutoPrice);

private:
	static CCoordinateCalculator_AutoPrice m_autoPriceCal;
	static CCoordinateCalculator_NoAutoPrice m_noAutoPriceCal;
};

// ----------------------------------------------------------------------------
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
