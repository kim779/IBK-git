// Coordinate.h: interface for the CCoordinate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COORDINATE_H__738A8587_2CBE_4735_9219_34C5FB5D2705__INCLUDED_)
#define AFX_COORDINATE_H__738A8587_2CBE_4735_9219_34C5FB5D2705__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/DequeList.h"

// ��ü�� ����� ��ǥ -- �����ǥ
class CCoordinate
{
public:
	CCoordinate& operator=(const CCoordinate& point);
	bool operator==(const CCoordinate& point);
	bool operator!=(const CCoordinate& point);

public:
	CCoordinate();
	// ����Ʈ �����ð� ���� - ojtaso (20080811)
	// 20081007 JS.Kim	����ڽð��� ����
	//CCoordinate(const double& x, const double& y, const CString& strType, const int nIndex = 0);
	CCoordinate(const double& x, const double& y, const int nTimeDiff, const CString& strType, const int nIndex = 0);
	CCoordinate(const CPoint& point);
	CCoordinate(const CCoordinate& point);

	double GetX() const;
	double GetY() const;
	int GetIndex() const;
	CPoint GetPixel() const;
	// ����Ʈ �����ð� ���� - ojtaso (20080811)
	CString GetDateType() const;
	// 20081007 JS.Kim	����ڽð��� ����. 
	int GetTimeDiff() const;

	void SetIndex(int nIndex);
	void SetX(const double& x, const int nIndex);
	void SetY(const double& y);
	// ����Ʈ �����ð� ���� - ojtaso (20080811)
	void SetDateType(CString& strType);
	// 20081007 JS.Kim	����ڽð��� ����. 
	void SetTimeDiff(int nTimeDiff);

private:
	double m_dX;
	double m_dY;
	int m_nIndex;
	// ����Ʈ �����ð� ���� - ojtaso (20080811)
	CString m_strType;
	// 20081007 JS.Kim	����ڽð��� ����. 
	int		m_nTimeDiff;		//���� ����Ÿ�� �ð���� �����ϱ� ���Ͽ� ���� ����ڰ� ������ �ð������ ���� ���� �ִ´�.
};

class CPoints
{
public:
	CPoints& operator=(const CPoints& points);
	bool operator==(const CPoints& points);
	bool operator!=(const CPoints& points);

public:
	CPoints();
	CPoints(const CCoordinate& savingPoint, const CPoint& drawingPoint);

	void SetPoints(const CCoordinate& savingPoint, const CPoint& drawingPoint);
	void SetDrawingPoints(const CPoint& drawingPoint);
	void SetPoints_Point_Y(const int& nDrawingPointY);
	void SetPoints_Value_Y(const double& dSavingPointY);

	CCoordinate GetSavingPoint() const;
	CPoint GetDrawingPoint() const;

private:
	CCoordinate m_savingPoint;
	CPoint m_drawingPoint;
};

class CPointsList
{
public:
	CPointsList& operator=(const CPointsList& pointsList);
	bool operator==(const CPointsList& pointsList);
	bool operator!=(const CPointsList& pointsList);

public:
	CPointsList();
	CPointsList(const CPointsList& pointsList);

	// list�� points �߰�/����.
	void AddPoints(const CPoints& points);
	void RemoveAll();
	bool RemoveAt(const int nIndex);

	void SetPointsList(const CPointsList& pointsList);

	// points count
	int GetCount() const;
	CPoints GetAt(const int nIndex) const;
	CPoints& GetAt(const int nIndex);

private:
	CDequeList<CPoints> m_PointsList;

	bool HavingIndex(const int nIndex) const;
};

#endif // !defined(AFX_COORDINATE_H__738A8587_2CBE_4735_9219_34C5FB5D2705__INCLUDED_)
