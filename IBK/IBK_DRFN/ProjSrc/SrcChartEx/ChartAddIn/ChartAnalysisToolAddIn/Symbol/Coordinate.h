// Coordinate.h: interface for the CCoordinate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COORDINATE_H__738A8587_2CBE_4735_9219_34C5FB5D2705__INCLUDED_)
#define AFX_COORDINATE_H__738A8587_2CBE_4735_9219_34C5FB5D2705__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// ��ü�� ����� ��ǥ -- �����ǥ
class CCoordinate
{
public:
	CCoordinate();
	CCoordinate(const double& x, const double& y);
	// 20081007 JS.Kim	����ڽð��� ����
	CCoordinate(const double& x, const double& y, const int nTimeDiff);
	CCoordinate(const CPoint& point);

	double GetX() const;
	double GetY() const;
	CPoint GetPixel() const;
	// 20081007 JS.Kim	����ڽð��� ����. 
	int GetTimeDiff() const;

	void SetX(const double& x);
	void SetY(const double& y);
	// 20081007 JS.Kim	����ڽð��� ����. 
	void SetTimeDiff(int nTimeDiff);

private:
	double m_nX;
	double m_nY;
	// 20081007 JS.Kim	����ڽð��� ����. 
	int		m_nTimeDiff;		//���� ����Ÿ�� �ð���� �����ϱ� ���Ͽ� ���� ����ڰ� ������ �ð������ ���� ���� �ִ´�.
};

#endif // !defined(AFX_COORDINATE_H__738A8587_2CBE_4735_9219_34C5FB5D2705__INCLUDED_)
