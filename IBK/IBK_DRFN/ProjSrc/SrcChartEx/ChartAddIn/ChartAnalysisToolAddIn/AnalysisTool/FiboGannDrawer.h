// FiboGannDrawer.h: interface for the CFiboGannDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIBOGANNDRAWER_H__4E4C7A05_33D2_4A7D_A973_ACDF5C803F04__INCLUDED_)
#define AFX_FIBOGANNDRAWER_H__4E4C7A05_33D2_4A7D_A973_ACDF5C803F04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PointDrawer.h"

class CRegion;
class CVertScaleType;

// �Ǻ���ġ �׸���
class CFiboDrawer : public CTwoPointDrawer
{
public:
	virtual void DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2);
	virtual void DrawShadow(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2);
	virtual int GetCount() const;

protected:
	virtual double GetCoefficient(const int nIndex) const;
	virtual bool DrawFiboShape(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2, 
								const int nIndex,BOOL bLeftExt,BOOL bRightExt) = 0;
//>> Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)
	virtual bool DrawFiboShape(CDC* pDC, const int nIndex, const CTwoPointDrawingData& drawingData) = 0;
//<< Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)

	CPoint GetCoefficientPoint(const CPoint& point, const int nHeight, const int nIndex) const;

// 2008.08.20 ������ �Ǻ���ġ Ȯ��<<
public:
	CDWordArray		m_arrCoefficient;
	CString			m_strCoefficient;
//>>
};

// �Ǻ���ġ ȣ �׸���
class CFiboArcDrawer : public CFiboDrawer
{
public:
	// ȣ�� ������ ���ϱ�.
	int GetRadius(const CPoint& pt1, const CPoint& pt2, const int nIndex) const;

protected:
	virtual bool DrawFiboShape(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2, 
								const int nIndex,BOOL bLeftExt,BOOL bRightExt);
//>> Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)
	virtual bool DrawFiboShape(CDC* pDC, const int nIndex, const CTwoPointDrawingData& drawingData);
//<< Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)

	virtual void DrawArc(CDC* pDC, const CRect& region, const int startY, const int endY) = 0;

private:
	CRect GetSemicircleRegion(const CPoint& centerPt, const int length) const;
};

// �Ǻ���ġ �ݿ� �׸���
class CFiboSemicircleDrawer : public CFiboArcDrawer
{
protected:
	virtual void DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData);
	virtual void DrawArc(CDC* pDC, const CRect& region, const int startY, const int endY);

private:
	CString GetText(const int nIndex) const;
	void DrawSemicircle(CDC* pDC, const CRect& region, const int startX, const int endX);
	void DrawText(CDC* pDC, const CRect& region, const int nIndex,const CPoint point);
	bool GetPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const;
	// �ش� index���� ������.
	bool GetIndexPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const;
	CRect GetSemicircleRegion(const CPoint& centerPt, const int length) const;
};

// �Ǻ���ġ �� �׸���
class CFiboCircleDrawer : public CFiboArcDrawer
{
protected:
	virtual void DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData);
	virtual void DrawArc(CDC* pDC, const CRect& region, const int startY, const int endY);
private:
	CString GetText(const int nIndex) const;
	void DrawSemicircle(CDC* pDC, const CRect& region, const int startX, const int endX);
	void DrawText(CDC* pDC, const CRect& region, const int nIndex,const CPoint point);
	bool GetPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const;
	// �ش� index���� ������.
	bool GetIndexPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const;
	CRect GetSemicircleRegion(const CPoint& centerPt, const int length) const;
};

// �Ǻ���ġ ����� �׸���
class CFiboPenLineDrawer : public CFiboDrawer
{
public:
	virtual void DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData);
	// �ش� index���� ������.
	bool GetPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const;

protected:
	virtual bool DrawFiboShape(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2, 
								const int nIndex,BOOL bLeftExt,BOOL bRightExt);
//>> Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)
	virtual bool DrawFiboShape(CDC* pDC, const int nIndex, const CTwoPointDrawingData& drawingData);
//<< Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)

private:
	void DrawPen(CDC* pDC, const CPoint& pt1, const CPoint& pt2, const CPoint& point);

	// �ش� index���� ������.
	bool GetIndexPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const;
	CString GetText(const int nIndex) const;
	void DrawText(CDC* pDC, const CRect& region, const int nIndex,const CPoint point);
};

// �Ǻ���ġ �ǵ��� �׸���
class CFiboRetraceDrawer : public CFiboDrawer
{
public:
	// 2008.08.20 ������ �Ǻ���ġ Ȯ��>>	CFiboDrawer::GetCount()�� �׳� ���
	//virtual int GetCount() const;
	//>>
	// �ش� index���� ������.
	bool GetPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const;
protected:
	virtual void DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData);

	// 2008.08.20 ������ �Ǻ���ġ Ȯ��>>	CFiboDrawer::GetCoefficient()�� �׳� ���
	//virtual double GetCoefficient(const int nIndex) const;
	//>>
	virtual bool DrawFiboShape(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2, 
								const int nIndex,BOOL bLeftExt,BOOL bRightExt);
//>> Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)
	virtual bool DrawFiboShape(CDC* pDC, const int nIndex, const CTwoPointDrawingData& drawingData);
//<< Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)
private:
	void DrawLine(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& point,BOOL bLeftExt,BOOL bRightExt);
	//void DrawText(CDC* pDC, const CRect& region, const CPoint& point_L, const CPoint& point_R);
	void DrawText_Left(CDC* pDC, const CTwoPointDrawingData& drawingData);
	void DrawText_Right(CDC* pDC, const CTwoPointDrawingData& drawingData);

	// �ش� index���� y ��ǥ.
	int GetPoint_Y(const int nY1, const int nY2, const int nIndex, const CVertScaleType& vertScaleType) const;
	// �ش� index���� ������.
	CPoint GetIndexPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex) const;
	//CString GetText(const int nIndex) const;
	CString GetLeftText(const int nIndex, const CTwoPointDrawingData& drawingData) const;
	CString GetRightText(const int nIndex) const;

	CPoints GetPoints(const CTwoPointDrawingData& drawingData, const CPoints& points1, const CPoints& points2, const int nIndex) const;
	double GetSavingPoint_Y(const CVertScaleType& vertScaleType, const CPoints& points1, const CPoints& points2, const int nDrawingPoint_Y) const;
	int GetDrawingPoint_Y(const int origin, const int nHeight, const int nIndex, const CVertScaleType& vertScaleType) const;
};

// �Ǻ� �ð���
class CFiboTimeDrawer : public CVertLineDrawer
{
public:
	// fibo �� ��ǥ�� ���ϱ�.
	bool GetPoint(const CDisplayData& displayData, const CPoint& startPoint, const int nIndex, CPoint& fiboPoint) const;

protected:
	virtual bool IsDrawing(const CRect& region, const CPoint& point) const;
	// 2008.08.20 ������ �Ǻ���ġ Ȯ��	COnePointDrawingData ������ �̿��Ͽ� �Ǻ���� ���ϱ� ������ ����
	virtual void DrawShape(CDC* pDC, const COnePointDrawingData& drawingData, const CDisplayData& displayData, const CPoint& point, const bool isCurrentRegion);
	virtual void DrawText(CDC* pDC, const COnePointDrawingData& drawingData);
	void DrawText_stand(CDC* pDC, const TEXTPOSITION eTextPosition, const CString& data, const CPoint& point, const CSize& maxSize);

private:
	// �׸���.
	void DrawFigure_Data(CDC* pDC, const CDisplayData& displayData, const CPoint& point);
	void DrawFigure_Pixel(CDC* pDC, const CRect& region, const CPoint& point);
	// fibo ��ǥ�� ���ϱ�.
	bool GetPoint_Data(const CRegion& region, const CPoint& startPoint, const int nIndex, CPoint& fiboPoint) const;
	bool GetPoint_Pixel(const CPoint& startPoint, const int nIndex, CPoint& fiboPoint) const;
	// fibo ��ǥ�� ���ϱ� ���� data.
	CRegion GetCoordinateCalRegion(const CDisplayData& displayData) const;

	// �Ǻ���ġ ����� �ٲٱ�.
	void ChangeFiboCoefficient(int& nFiboCoefficient1, int& nFiboCoefficient2) const;

	// �Ǻ���ġ ����� ��������.
	int GetCoefficient(const int nFiboCoefficient1, const int nFiboCoefficient2) const;
	int GetCoefficient(const int nIndex) const;// 2008.08.20 ������ �Ǻ���ġ Ȯ��

	int GetCount() const;
	CString GetText(const int nIndex) const;

// 2008.08.20 ������ �Ǻ���ġ Ȯ��<<
public:
	CDWordArray		m_arrCoefficient;
	CString			m_strCoefficient;
//>>
};


// ----------------------------------------------------------------------------
// ��
class CGannDrawer : public CTwoPointDrawer
{
public:
	virtual void DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2);
protected:
	virtual void DrawShape(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2) = 0;

private:
	bool IsDrawing(const CPoint& pt1, const CPoint& pt2) const;
};

// ������ �׸���
class CGannLineDrawer : public CGannDrawer
{
public:
	bool GetLineEndPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, CPoint& point) const;

protected:
	virtual void DrawShape(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2);

	bool DrawPairLine(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2);
};

// ���� �׸���
class CGannFanDrawer : public CGannLineDrawer
{
public:	
	int GetCount() const;
	bool GetLineEndPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const;

protected:
	virtual void DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData);
	virtual void DrawShape(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2);

private:
	CString GetText(const int nIndex) const;
	double GetAngle(const int nIndex) const;
	double GetAngle(const int nIndex, double dZFactor) const;
	CPoint GetAnglePoint(const CPoint& pt1, const CPoint& pt2, const int nIndex) const;
	double GetOrgAngle(const CPoint& pt1, const CPoint& pt2) const;
};

// ���׸��� �׸���
class CGannGridDrawer : public CGannDrawer
{
public:
	bool GetLineEndPoints(const CRect& region, const CPoint& pt1, const CPoint& pt2, 
			const int nIndex, CPoint& point1, CPoint& point2) const;

protected:
	virtual void DrawShape(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2);

private:
	void DrawOneSide(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2, const int length_x);
	bool DrawPairLine(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2);
	bool DrawSlashLine(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2);
	bool DrawReverseSlashLine(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2);
	bool DrawLine(CDC* pDC, const CRect& region, const CPoint& lineStart, const CPoint& lineEnd);


	CPoint GetNextPoint(const CPoint& point, const int length_x) const;
	CPoint GetSymmetryPointOfXAxis(const CPoint& orgPt, const CPoint& point) const;

	// ������ ������
	bool GetRectangleIntersectionPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, CPoint& start, CPoint& end) const;
	bool GetIntersectionPoint_VertLine(const CPoint& line1_Pt1, const CPoint& line1_Pt2, const CPoint& line2_Pt1, const CPoint& line2_Pt2, CPoint& intersectionPoint) const;
	bool GetIntersectionPoint_HorzLine(const CPoint& line1_Pt1, const CPoint& line1_Pt2, const CPoint& line2_Pt1, const CPoint& line2_Pt2) const;

	// side�� ���� �׸� ��ǥ��.
	bool GetPoints_Side(const CRect& region, const CPoint& pt1, const CPoint& pt2, 
			const int nIndex, CPoint& point1, CPoint& point2) const;
};

#endif // !defined(AFX_FIBOGANNDRAWER_H__4E4C7A05_33D2_4A7D_A973_ACDF5C803F04__INCLUDED_)
