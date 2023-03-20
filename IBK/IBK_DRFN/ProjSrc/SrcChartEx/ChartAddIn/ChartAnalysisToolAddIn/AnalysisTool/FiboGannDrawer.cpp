// FiboGannDrawer.cpp: implementation of the CFiboGannDrawer class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FiboGannDrawer.h"

#include "DrawingData.h"
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
// class CFiboDrawer

void CFiboDrawer::DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2)
{
	// �Ǻ������ �̿��� ��(ȣ)
// 2008.08.20 ������ �Ǻ���ġ Ȯ��
//		����ڷκ��� ����� �Է� �޾� ó���Ѵ�	<<
	/*
	for(int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		if(!DrawFiboShape(pDC, drawingData.GetDisplayData().GetDisplayRegion(), pt1, pt2, nIndex,
								drawingData.GetLineExtEnvironment().GetLeftExt(),
								drawingData.GetLineExtEnvironment().GetRightExt()))
			break;
	}
	*/

	CString strCoefficient = drawingData.GetDataType().GetExtData();
	if( strCoefficient.IsEmpty() || strCoefficient.CompareNoCase("All") == 0 )	
	{	// ���� ���ų� "All"�θ� ����Ǿ� �ִ� ��� ������ ���� ����
		strCoefficient = "0,236,382,500,618,786,1000";
	}
	
	if( strCoefficient != m_strCoefficient )	// ���� ���� �ٸ� ��쿡�� ���� Coefficient�� ���Ѵ�.
	{
		m_strCoefficient = strCoefficient;
		m_arrCoefficient.RemoveAll();

		CString strBuf = strCoefficient;
		CString strR;
		while( !strBuf.IsEmpty() )
		{
			strR = CDataConversion::GetStringData(strBuf, ",");
			if( strR.IsEmpty() )		
			{
				strR = strBuf;
				strBuf.Empty();
			}
			if( strR.IsEmpty() )
				break;

			if( strR.CompareNoCase("All") )
				m_arrCoefficient.Add(atoi(strR));
		}
	}

//>> Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)
	for(int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
//		// 2008.08.20 ������ �Ǻ���ġ Ȯ��
//		//	0 ���� �߰� => 0�� ��� ȣ�� �׸��� false�� Return �ȴ�. ���� 0�̿��� ���� �׸��� ���� Return���� �ʴ´�	<<
//		//if(!DrawFiboShape(pDC, drawingData.GetDisplayData().GetDisplayRegion(), pt1, pt2, nIndex,
//		//						drawingData.GetLineExtEnvironment().GetLeftExt(),
//		//						drawingData.GetLineExtEnvironment().GetRightExt()))
//		//	break;
//		DrawFiboShape(pDC, drawingData.GetDisplayData().GetDisplayRegion(), pt1, pt2, nIndex,
//								drawingData.GetLineExtEnvironment().GetLeftExt(),
//								drawingData.GetLineExtEnvironment().GetRightExt());
		DrawFiboShape(pDC, nIndex, drawingData);
	}
//<< Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)
}

void CFiboDrawer::DrawShadow(CDC* pDC, const CRect& /*region*/, const CPoint& pt1, const CPoint& pt2)
{
	// �缱
	CDrawer::DrawLine(pDC, pt1, pt2);
}

int CFiboDrawer::GetCount() const
{
// 2008.08.20 ������ �Ǻ���ġ Ȯ��
//		����ڷκ��� ����� �Է� �޾� ó���Ѵ�	<<
	return m_arrCoefficient.GetSize();
	//return 4;
//>>
}

// protected ==================================================================
double CFiboDrawer::GetCoefficient(const int nIndex) const
{
// 2008.08.20 ������ �Ǻ���ġ Ȯ��
//		����ڷκ��� ����� �Է� �޾� ó���Ѵ�	<<
	/*
	switch(nIndex)
	{
	case 0:
		return 0.236;
	case 1:
		return 0.382;
	case 2:
		return 0.5;
	case 3:
		return 0.618;
	}
	
	assert(false);
	return 0.0;
	*/
	if( nIndex < 0 || nIndex >= m_arrCoefficient.GetSize() )
		return 0.0;

	return (double)m_arrCoefficient.GetAt(nIndex)/1000.;
//>>
}

// ----------------------------------------------------------------------------
CPoint CFiboDrawer::GetCoefficientPoint(const CPoint& point, const int nHeight, const int nIndex) const
{
	int nLength = CMath::Round((double)nHeight * GetCoefficient(nIndex));
	return CPoint(point.x, point.y - nLength);
}

///////////////////////////////////////////////////////////////////////////////
// class CFiboArcDrawer

// ȣ�� ������ ���ϱ�.
int CFiboArcDrawer::GetRadius(const CPoint& pt1, const CPoint& pt2, const int nIndex) const
{
	return CMath::Round(CMath::Distance(pt2, pt1) * GetCoefficient(nIndex));
}

// protected ==================================================================
bool CFiboArcDrawer::DrawFiboShape(CDC* pDC, const CRect& /*region*/, const CPoint& pt1, const CPoint& pt2, 
								   const int nIndex,BOOL bLeftExt,BOOL bRightExt)
{
	CRect rect = GetSemicircleRegion(pt2, GetRadius(pt1, pt2, nIndex));
	if(rect.IsRectEmpty())
		return false;

	DrawArc(pDC, rect, pt2.y, pt1.y);
	return true;
}

//>> Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)
bool CFiboArcDrawer::DrawFiboShape(CDC* pDC, const int nIndex, const CTwoPointDrawingData& drawingData)
{
	CRect rect = GetSemicircleRegion(drawingData.GetEndPoints().GetDrawingPoint(), GetRadius(drawingData.GetStartPoints().GetDrawingPoint(), drawingData.GetEndPoints().GetDrawingPoint(), nIndex));
	if(rect.IsRectEmpty())
		return false;

	DrawArc(pDC, rect, drawingData.GetEndPoints().GetDrawingPoint().y, drawingData.GetStartPoints().GetDrawingPoint().y);
	return true;
}
//<< Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)

// private ====================================================================
// ȣ�� �׸� ������ ���Ѵ�.
CRect CFiboArcDrawer::GetSemicircleRegion(const CPoint& centerPt, const int length) const
{
	CRect rect(centerPt.x -length, centerPt.y -length, centerPt.x +length, centerPt.y +length);
	rect.NormalizeRect();
	return rect;
}

///////////////////////////////////////////////////////////////////////////////
// class CFiboSemicircleDrawer

void CFiboSemicircleDrawer::DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData)
{ 
	CTextShowPosition* pTextPosition = (CTextShowPosition*) drawingData.GetTextPosition();
	if(pTextPosition == NULL)
		return;

	if(pTextPosition->IsShow())
	{
		CPoint point;
		CRect rect;
		CPoint point_S = drawingData.GetStartPoints().GetDrawingPoint();
		CPoint point_E = drawingData.GetEndPoints().GetDrawingPoint();
		CRect region = drawingData.GetDiaplayRegion();
		int nTotalCount = GetCount();
		for(int nIndex = 0; nIndex < nTotalCount; nIndex++)
		{
			rect = GetSemicircleRegion(point_E, GetRadius(point_S, point_E, nIndex));			
			if(point_S.y < point_E.y)
				point.y = rect.top;
			else
				point.y = rect.bottom;

			if(point_S.x <= point_E.x)
			{				
				point.x = point_E.x;
				DrawText(pDC, region,nIndex,point);
			}
			else
			{
				CString strText = GetText(nIndex);
				CSize maxSize = pDC->GetTextExtent(strText);
				point.x = point_E.x - maxSize.cx/2;
				if(point_S.y < point_E.y)
					CDrawer::DrawTextOut(pDC, CDrawer::BOTTOM_RIGHT, strText , point, maxSize);	
				else
					CDrawer::DrawTextOut(pDC, CDrawer::TOP_RIGHT, strText , point, maxSize);									
			}				
		}
	}
}

// protected ==================================================================
void CFiboSemicircleDrawer::DrawArc(CDC* pDC, const CRect& region, const int startY, const int endY)
{
	if(startY >= endY)
		DrawSemicircle(pDC, region, region.right, region.left);
	else
		DrawSemicircle(pDC, region, region.left, region.right);
}

// private ====================================================================
void CFiboSemicircleDrawer::DrawSemicircle(CDC* pDC, const CRect& region, const int startX, const int endX)
{
	pDC->Arc(region, CPoint(startX, region.CenterPoint().y), CPoint(endX, region.CenterPoint().y));
}

void CFiboSemicircleDrawer::DrawText(CDC* pDC, const CRect& region, const int nIndex,const CPoint point)
{
	CString strText = GetText(nIndex);
	CSize maxSize = pDC->GetTextExtent(strText);
	CPoint pointCenter(point);
	pointCenter.x -= maxSize.cx/2;
	CDrawer::DrawTextOut(pDC, CDrawer::LEFT, strText , point, maxSize);	
}

CString CFiboSemicircleDrawer::GetText(const int nIndex) const
{
	double dCoefficient = GetCoefficient(nIndex) *100.0;
	CString strCoefficient = CDataConversion::DoubleToString(dCoefficient, 1);
	if(strCoefficient.IsEmpty())
		return "";

	return strCoefficient + "%";
}

// �ش� index���� ������.
bool CFiboSemicircleDrawer::GetPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const
{
	if(pt1.x <= pt2.x)
		return GetIndexPoint(region, pt1, pt2, nIndex, point);

	return GetIndexPoint(region, pt2, pt1, nIndex, point);
}

// �ش� index���� ������.
bool CFiboSemicircleDrawer::GetIndexPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const
{
	CPoint tempPt = CFiboDrawer::GetCoefficientPoint(pt2, (pt2.y-pt1.y), nIndex);

	return CDrawer::Intersection(pt1, tempPt, 
		CPoint(region.right, region.top), CPoint(region.right, region.bottom), point);
}

CRect CFiboSemicircleDrawer::GetSemicircleRegion(const CPoint& centerPt, const int length) const
{
	CRect rect(centerPt.x -length, centerPt.y -length, centerPt.x +length, centerPt.y +length);
	rect.NormalizeRect();
	return rect;
}
///////////////////////////////////////////////////////////////////////////////
// class CFiboCircleDrawer
void CFiboCircleDrawer::DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData)
{ 
	CTextShowPosition* pTextPosition = (CTextShowPosition*) drawingData.GetTextPosition();
	if(pTextPosition == NULL)
		return;

	if(pTextPosition->IsShow())
	{
		CPoint point;
		//CPoint point_L = drawingData.GetStartPoints().GetDrawingPoint();
		//CPoint point_R = drawingData.GetEndPoints().GetDrawingPoint();
		CPoint point_L = drawingData.GetLeftPoints().GetDrawingPoint();
		CPoint point_R = drawingData.GetRightPoints().GetDrawingPoint();
		CRect region = drawingData.GetDiaplayRegion();
		int nTotalCount = GetCount();
		for(int nIndex = 0; nIndex < nTotalCount; nIndex++)
		{
			point = CFiboDrawer::GetCoefficientPoint(point_R, (point_R.y-point_L.y), nIndex);
			point.x = point_R.x;
			DrawText(pDC, region,nIndex,point);
		}
	}
}
// protected ==================================================================
void CFiboCircleDrawer::DrawArc(CDC* pDC, const CRect& region, const int /*startY*/, const int /*endY*/)
{
	CPoint point(region.left, region.CenterPoint().y);
	pDC->Arc(region, point, point);
}

void CFiboCircleDrawer::DrawText(CDC* pDC, const CRect& region, const int nIndex,const CPoint point)
{
	CSize maxSize = pDC->GetTextExtent("4.236%");
	CString strText = GetText(nIndex);
	CPoint pointCenter(point);
	pointCenter.x -= 5;
	CDrawer::DrawTextOut(pDC, CDrawer::LEFT, strText , point, maxSize);	
}

CString CFiboCircleDrawer::GetText(const int nIndex) const
{
	double dCoefficient = GetCoefficient(nIndex) *100.0;
	CString strCoefficient = CDataConversion::DoubleToString(dCoefficient, 1);
	if(strCoefficient.IsEmpty())
		return "";

	return strCoefficient + "%";
}

///////////////////////////////////////////////////////////////////////////////
// class CFiboPenLineDrawer

void CFiboPenLineDrawer::DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData)
{ 
	CTextShowPosition* pTextPosition = (CTextShowPosition*) drawingData.GetTextPosition();
	if(pTextPosition == NULL)
		return;

	if(pTextPosition->IsShow())
	{
		CPoint point;
		CPoint point_L = drawingData.GetLeftPoints().GetDrawingPoint();
		CPoint point_R = drawingData.GetRightPoints().GetDrawingPoint();
		CRect region = drawingData.GetDiaplayRegion();
		int nTotalCount = GetCount();
		for(int nIndex = 0; nIndex < nTotalCount; nIndex++)
		{
			point = CFiboDrawer::GetCoefficientPoint(point_R, (point_R.y-point_L.y), nIndex);
			point.x = point_R.x;
			DrawText(pDC, region,nIndex,point);
		}
	}
}

// �ش� index���� ������.
bool CFiboPenLineDrawer::GetPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const
{
	if(pt1.x <= pt2.x)
		return GetIndexPoint(region, pt1, pt2, nIndex, point);

	return GetIndexPoint(region, pt2, pt1, nIndex, point);
}

// protected ==================================================================
bool CFiboPenLineDrawer::DrawFiboShape(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2, 
									   const int nIndex, BOOL bLeftExt,BOOL bRightExt)
{
	CPoint point;
	if(!GetPoint(region, pt1, pt2, nIndex, point))
		return false;

	DrawPen(pDC, pt1, pt2, point);
	return true;
}

//>> Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)
bool CFiboPenLineDrawer::DrawFiboShape(CDC* pDC, const int nIndex, const CTwoPointDrawingData& drawingData)
{
	CPoint point;
	if(!GetPoint(drawingData.GetDiaplayRegion(), drawingData.GetStartPoints().GetDrawingPoint(), drawingData.GetEndPoints().GetDrawingPoint(), nIndex, point))
		return false;

	DrawPen(pDC, drawingData.GetStartPoints().GetDrawingPoint(), drawingData.GetEndPoints().GetDrawingPoint(), point);
	return true;
}
//<< Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)

// private ====================================================================
void CFiboPenLineDrawer::DrawPen(CDC* pDC, const CPoint& pt1, const CPoint& pt2, const CPoint& point)
{
	if(pt1.x <= pt2.x)
		CDrawer::DrawLine(pDC, pt1, point);
	else
		CDrawer::DrawLine(pDC, pt2, point);
}

// �ش� index���� ������.
bool CFiboPenLineDrawer::GetIndexPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const
{
	CPoint tempPt = CFiboDrawer::GetCoefficientPoint(pt2, (pt2.y-pt1.y), nIndex);

	return CDrawer::Intersection(pt1, tempPt, 
		CPoint(region.right, region.top), CPoint(region.right, region.bottom), point);
}

void CFiboPenLineDrawer::DrawText(CDC* pDC, const CRect& region, const int nIndex,const CPoint point)
{
	CSize maxSize = pDC->GetTextExtent("4.236%");
	CString strText = GetText(nIndex);
	CPoint pointCenter(point);
	pointCenter.x -= 5;
	CDrawer::DrawTextOut(pDC, CDrawer::RIGHT, strText , point, maxSize);	
}

CString CFiboPenLineDrawer::GetText(const int nIndex) const
{
	double dCoefficient = GetCoefficient(nIndex) *100.0;
	CString strCoefficient = CDataConversion::DoubleToString(dCoefficient, 1);
	if(strCoefficient.IsEmpty())
		return "";

	return strCoefficient + "%";
}

///////////////////////////////////////////////////////////////////////////////
// class CFiboRetraceDrawer

void CFiboRetraceDrawer::DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData)
{
	CTextHorzPosition* pTextPosition = (CTextHorzPosition*) drawingData.GetTextPosition();
	if(pTextPosition == NULL)
		return;

	// 2008.09.15 ������	�ؽ�Ʈ �������� �ɼ��� Dialog�� Left�� �־ �� �ɼ����� ���� �� ó���ϵ��� ����
	/*
	if(pTextPosition->GetLeft())
	{	
		DrawText_Left(pDC, drawingData);
	}
	if(pTextPosition->GetRight())
	{
		DrawText_Right(pDC, drawingData);
	}
	*/
	if(pTextPosition->GetLeft())
	{	
		DrawText_Left(pDC, drawingData);
		DrawText_Right(pDC, drawingData);
	}
}

// 2008.08.20 ������ �Ǻ���ġ Ȯ��>>	CFiboDrawer::GetCount()�� �׳� ���
/*
int CFiboRetraceDrawer::GetCount() const
{
	//by jwp(2006/10/02) - ���� �� Ȯ���� ���ϸ� �Ʒ� �ּ� Ǯ��
	//return 10;

	return 6;
}
*/
//>>

// �ش� index���� ������.
bool CFiboRetraceDrawer::GetPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const
{
	if(pt1.x == pt2.x)
		return false;

	if(pt1.x < pt2.x)
		point = GetIndexPoint(region, pt1, pt2, nIndex);
	else
		point = GetIndexPoint(region, pt2, pt1, nIndex);

	return true;
}

// protected ==================================================================
// 2008.08.20 ������ �Ǻ���ġ Ȯ��>>	CFiboDrawer::GetCoefficient()�� �׳� ���
/*
double CFiboRetraceDrawer::GetCoefficient(const int nIndex) const
{
	switch(nIndex)
	{
	case 0:
		return 0.0;
	case 1:
		return 0.236;
	case 2:
		return 0.382;
	case 3:
		return 0.5;
	case 4:
		return 0.618;
	case 5:
		return 1.0;
	case 6:
		return 1.618;
	case 7:
		return 2.618;
	case 8:
		return 4.236;
	case 9:
		return 5.618;
	}

	assert(false);
	return 0.0;
}
*/
//>>

bool CFiboRetraceDrawer::DrawFiboShape(CDC* pDC, const CRect& region, const CPoint& pt1, 
									   const CPoint& pt2, const int nIndex,
									   BOOL bLeftExt,BOOL bRightExt)
{
	if(pt1.y == pt2.y)
		return false;

	CPoint point;
	if(!GetPoint(region, pt1, pt2, nIndex, point))
		return false;

	DrawLine(pDC, region,pt1, pt2, point,bLeftExt,bRightExt);
	return true;
}

//>> Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)
bool CFiboRetraceDrawer::DrawFiboShape(CDC* pDC, const int nIndex, const CTwoPointDrawingData& drawingData)
{
	if(drawingData.GetStartPoints().GetDrawingPoint().y == drawingData.GetEndPoints().GetDrawingPoint().y)
		return false;

	CRect region = drawingData.GetDiaplayRegion();
	CPoint point_L = drawingData.GetLeftPoints().GetDrawingPoint();
	CPoint point_R = drawingData.GetRightPoints().GetDrawingPoint();

//	int nTotalCount = GetCount();
//	for(int nIndex = 0; nIndex < nTotalCount; nIndex++)
	{
		CPoint point;
		CPoint ptStart(drawingData.GetStartPoints().GetDrawingPoint());
		CPoint ptEnd(drawingData.GetEndPoints().GetDrawingPoint());

		// ��->��
		if(ptStart.x < ptEnd.x)
		{
			// ��->��
			if(ptStart == point_L)
				point = CFiboDrawer::GetCoefficientPoint(point_L, (point_L.y-point_R.y), nIndex);
			// ��->��
			else
				point = CFiboDrawer::GetCoefficientPoint(point_L, (point_R.y-point_L.y), nIndex);
		}
		// ��->��
		else if(ptStart.x > ptEnd.x)
		{
			// ��->��
			if(ptStart == point_R)
				point = CFiboDrawer::GetCoefficientPoint(point_R, (point_R.y-point_L.y), nIndex);
			// ��->��
			else
				point = CFiboDrawer::GetCoefficientPoint(point_R, (point_L.y-point_R.y), nIndex);
		}

		point.x = point_R.x;
		DrawLine(pDC, region, ptStart, ptEnd, point, drawingData.GetLineExtEnvironment().GetLeftExt(), drawingData.GetLineExtEnvironment().GetRightExt());
	}
	
	return true;
}
//<< Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)

// private ====================================================================
void CFiboRetraceDrawer::DrawLine(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2, const CPoint& point,BOOL bLeftExt,BOOL bRightExt)
{
	CPoint ptStart;
	CPoint ptEnd;
	CPoint ptTemp;

	if(bLeftExt == FALSE && bRightExt == FALSE)
	{
		CDrawer::DrawLine(pDC, CPoint((pt1.x < pt2.x ? pt1.x: pt2.x), point.y), point);
	}
	else if(bLeftExt == TRUE && bRightExt == FALSE)
	{
		ptStart = CPoint((pt1.x < pt2.x ? pt1.x: pt2.x), point.y);
		ptStart.x = region.left;
		CDrawer::DrawLine(pDC,ptStart , point);
	}
	else if(bLeftExt == FALSE && bRightExt == TRUE)
	{
		ptStart = CPoint((pt1.x < pt2.x ? pt1.x: pt2.x), point.y);
		ptTemp = point;
		if(ptTemp.x > ptStart.x)
			ptTemp.x = ptStart.x;
		ptStart.x = region.right;
		CDrawer::DrawLine(pDC, ptStart, ptTemp);
	}
	else
	{
		ptStart = CPoint((pt1.x < pt2.x ? pt1.x: pt2.x), point.y);
		ptStart.x = region.left;
		ptEnd = point;
		ptEnd.x = region.right;
		CDrawer::DrawLine(pDC, ptStart, ptEnd);
	}
}

void CFiboRetraceDrawer::DrawText_Left(CDC* pDC, const CTwoPointDrawingData& drawingData)
{
	CRect region = drawingData.GetDiaplayRegion();
	CPoint point_L = drawingData.GetLeftPoints().GetDrawingPoint();
	CPoint point_R = drawingData.GetRightPoints().GetDrawingPoint();

	CSize maxSize;
	CString strLeftText;
	int nTotalCount = GetCount();
	for(int nIndex = 0; nIndex < nTotalCount; nIndex++)
	{
//>> Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)
//		CPoint point;
//		if(!GetPoint(region, point_L, point_R, nIndex, point))
//			break;

		CPoint point;
		CPoint ptStart(drawingData.GetStartPoints().GetDrawingPoint());
		CPoint ptEnd(drawingData.GetEndPoints().GetDrawingPoint());

		CPoints points_L = drawingData.GetLeftPoints();
		CPoints points_R = drawingData.GetRightPoints();

		double dDiff = points_L.GetSavingPoint().GetY() - points_R.GetSavingPoint().GetY();
		if( dDiff < 0.0 )		dDiff *= -1;

		double dMin = 0.;
		double dMax = 0.;
		double dLeftValue = 0.;
		
		CVertScaleType vertScaleType = drawingData.GetVertScaleType();

		if(ptStart.x == ptEnd.x)
			break;
		// ��->��
		else if(ptStart.x < ptEnd.x)
		{
			// ��->��
//			if(ptStart == point_L)
			if(ptStart.y <= ptEnd.y)
			{
				point = CFiboDrawer::GetCoefficientPoint(point_L, (point_L.y-point_R.y), nIndex);
				if(vertScaleType.IsInvert())
				{
					dMin = min(points_L.GetSavingPoint().GetY(), points_R.GetSavingPoint().GetY());
					dLeftValue = dMin + dDiff * GetCoefficient(nIndex);
				}
				else
				{
					dMax = max(points_L.GetSavingPoint().GetY(), points_R.GetSavingPoint().GetY());
					dLeftValue = dMax - dDiff * GetCoefficient(nIndex);
				}
			}
			// ��->��
			else
			{
				point = CFiboDrawer::GetCoefficientPoint(point_L, (point_L.y-point_R.y), nIndex);
				if(vertScaleType.IsInvert())
				{
					dMax = max(points_L.GetSavingPoint().GetY(), points_R.GetSavingPoint().GetY());
					dLeftValue = dMax - dDiff * GetCoefficient(nIndex);
				}
				else
				{
					dMin = min(points_L.GetSavingPoint().GetY(), points_R.GetSavingPoint().GetY());
					dLeftValue = dMin + dDiff * GetCoefficient(nIndex);
				}
			}
		}
		// ��->��
		else if(ptStart.x > ptEnd.x)
		{
			// ��->��
//			if(ptStart == point_R)
			if(ptStart.y <= ptEnd.y)
			{
				point = CFiboDrawer::GetCoefficientPoint(point_R, (point_R.y-point_L.y), nIndex);
				if(vertScaleType.IsInvert())
				{
					dMin = min(points_L.GetSavingPoint().GetY(), points_R.GetSavingPoint().GetY());
					dLeftValue = dMin + dDiff * GetCoefficient(nIndex);
				}
				else
				{
					dMax = max(points_L.GetSavingPoint().GetY(), points_R.GetSavingPoint().GetY());
					dLeftValue = dMax - dDiff * GetCoefficient(nIndex);
				}
			}
			// ��->��
			else
			{
				point = CFiboDrawer::GetCoefficientPoint(point_R, (point_R.y-point_L.y), nIndex);
				if(vertScaleType.IsInvert())
				{
					dMax = max(points_L.GetSavingPoint().GetY(), points_R.GetSavingPoint().GetY());
					dLeftValue = dMax - dDiff * GetCoefficient(nIndex);
				}
				else
				{
					dMin = min(points_L.GetSavingPoint().GetY(), points_R.GetSavingPoint().GetY());
					dLeftValue = dMin + dDiff * GetCoefficient(nIndex);
				}
			}
		}

//		strLeftText = GetLeftText(nIndex, drawingData);
		strLeftText = CDataConversion::GetStringThousandFormat(dLeftValue, drawingData.GetDataType().GetStrVert());
//<< Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)
		maxSize = pDC->GetTextExtent(strLeftText);
		CDrawer::DrawTextOut(pDC, CDrawer::LEFT, strLeftText, CPoint(point_L.x, point.y), maxSize);
	}
}

void CFiboRetraceDrawer::DrawText_Right(CDC* pDC, const CTwoPointDrawingData& drawingData)
{
	CRect region = drawingData.GetDiaplayRegion();
	CPoint point_L = drawingData.GetLeftPoints().GetDrawingPoint();
	CPoint point_R = drawingData.GetRightPoints().GetDrawingPoint();

	CSize maxSize = pDC->GetTextExtent("4.236%");
	int nTotalCount = GetCount();
	for(int nIndex = 0; nIndex < nTotalCount; nIndex++)
	{
//>> Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)
//		CPoint point;
//		if(!GetPoint(region, point_L, point_R, nIndex, point))
//			break;

		CPoint point;
		CPoint ptStart(drawingData.GetStartPoints().GetDrawingPoint());
		CPoint ptEnd(drawingData.GetEndPoints().GetDrawingPoint());
		if(ptStart.x == ptEnd.x)
			break;
		// ��->��
		else if(ptStart.x < ptEnd.x)
		{
			// ��->��
			if(ptStart == point_L)
			{
				point = CFiboDrawer::GetCoefficientPoint(point_L, (point_L.y-point_R.y), nIndex);
				point.x = point_R.x;
			}
			// ��->��
			else
			{
				point = CFiboDrawer::GetCoefficientPoint(point_L, (point_R.y-point_L.y), nIndex);
				point.x = point_R.x;
			}
		}
		// ��->��
		else if(ptStart.x > ptEnd.x)
		{
			// ��->��
			if(ptStart == point_R)
			{
				point = CFiboDrawer::GetCoefficientPoint(point_R, (point_R.y-point_L.y), nIndex);
				point.x = point_R.x;
			}
			// ��->��
			else
			{
				point = CFiboDrawer::GetCoefficientPoint(point_R, (point_L.y-point_R.y), nIndex);
				point.x = point_R.x;
			}
		}
//<< Left�� x��ǥ�� Right�� x���� Ŭ ��� ������ Text�� ��/�Ʒ��� �ٲ�� ���� - vntsorl(20090722)
		CDrawer::DrawTextOut(pDC, CDrawer::RIGHT, GetRightText(nIndex), CPoint(point_R.x, point.y), maxSize);
	}
}

// �ش� index���� y ��ǥ.
int CFiboRetraceDrawer::GetPoint_Y(const int nY1, const int nY2, const int nIndex, const CVertScaleType& vertScaleType) const
{
	if(vertScaleType.IsInvert())
		return GetDrawingPoint_Y(min(nY1, nY2), abs(nY1 - nY2), nIndex, vertScaleType);
	else
		return GetDrawingPoint_Y(max(nY1, nY2), abs(nY1 - nY2), nIndex, vertScaleType);
}

// �ش� index���� ������.
CPoint CFiboRetraceDrawer::GetIndexPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex) const
{
 	CPoint tempPt = CFiboDrawer::GetCoefficientPoint(pt2, (pt2.y-pt1.y), nIndex);
	tempPt.x = (pt1.x > pt2.x?pt1.x:pt2.x);	
	return tempPt;
}


CString CFiboRetraceDrawer::GetLeftText(const int nIndex, const CTwoPointDrawingData& drawingData) const
{
	CPoints points_L = drawingData.GetLeftPoints();
	CPoints points_R = drawingData.GetRightPoints();

	double aaa = points_L.GetSavingPoint().GetY();
	double bbb = points_R.GetSavingPoint().GetY();
	// 2008.09.19 ������   Y ���� ���� 0���� ���� ���� ��� abs()�� ���� 0���� �ٲ�� ���� ����
	//double dDiff = abs(points_L.GetSavingPoint().GetY() - points_R.GetSavingPoint().GetY());
	double dDiff = points_L.GetSavingPoint().GetY() - points_R.GetSavingPoint().GetY();
	if( dDiff < 0.0 )		dDiff *= -1;

	double dMin = 0.;
	double dMax = 0.;
	double dLeftValue = 0.;
	
	CVertScaleType vertScaleType = drawingData.GetVertScaleType();

	// �»� - ����
	if((points_L.GetSavingPoint().GetX() > points_R.GetSavingPoint().GetX() && points_L.GetSavingPoint().GetY() < points_R.GetSavingPoint().GetY()) ||
		(points_L.GetSavingPoint().GetX() < points_R.GetSavingPoint().GetX() && points_L.GetSavingPoint().GetY() > points_R.GetSavingPoint().GetY()))
	{
		dMin = min(points_L.GetSavingPoint().GetY(), points_R.GetSavingPoint().GetY());
		dLeftValue = dMin + dDiff * GetCoefficient(nIndex);
	}
	// ��� - ����
	else if((points_L.GetSavingPoint().GetX() < points_R.GetSavingPoint().GetX() && points_L.GetSavingPoint().GetY() < points_R.GetSavingPoint().GetY()) ||
		(points_L.GetSavingPoint().GetX() > points_R.GetSavingPoint().GetX() && points_L.GetSavingPoint().GetY() > points_R.GetSavingPoint().GetY()))
	{
		dMax = max(points_L.GetSavingPoint().GetY(), points_R.GetSavingPoint().GetY());
		dLeftValue = dMax - dDiff * GetCoefficient(nIndex);
	}
	
	return CDataConversion::GetStringThousandFormat(dLeftValue, drawingData.GetDataType().GetStrVert());
}

CString CFiboRetraceDrawer::GetRightText(const int nIndex) const
{
	double dCoefficient = GetCoefficient(nIndex) *100.0;
	CString strCoefficient = CDataConversion::DoubleToString(dCoefficient, 1);
	if(strCoefficient.IsEmpty())
		return "";

	return strCoefficient + "%";
}

CPoints CFiboRetraceDrawer::GetPoints(const CTwoPointDrawingData& drawingData, const CPoints& points1, const CPoints& points2, const int nIndex) const
{
	if(nIndex == 0)
		return points1;
	else if(nIndex == (GetCount()-1))
		return CPoints(points2.GetSavingPoint(), CPoint(points1.GetDrawingPoint().x, points2.GetDrawingPoint().y));

	CRect region = drawingData.GetDiaplayRegion();
	CVertScaleType vertScaleType = drawingData.GetVertScaleType();
	CPoint indexPoint = GetIndexPoint(region, points1.GetDrawingPoint(), points2.GetDrawingPoint(), nIndex);
	CPoint drawPoint(points1.GetDrawingPoint().x, indexPoint.y);
	double saving_y = GetSavingPoint_Y(vertScaleType, points1, points2, drawPoint.y);
	// ����Ʈ �����ð� ���� - ojtaso (20080811)
	// 20081007 JS.Kim	����ڽð��� ����
	CCoordinate savePoint(points1.GetSavingPoint().GetX(), saving_y, points1.GetSavingPoint().GetTimeDiff(), points1.GetSavingPoint().GetDateType(), 0);
	return CPoints(savePoint, drawPoint);
}

double CFiboRetraceDrawer::GetSavingPoint_Y(const CVertScaleType& vertScaleType, const CPoints& points1, const CPoints& points2, const int nDrawingPoint_Y) const
{
	CRect rect(points1.GetDrawingPoint(), points2.GetDrawingPoint());
	rect.NormalizeRect();

	CDisplayDataVertMinMax displayDataMinMax(points1.GetSavingPoint().GetY(), points2.GetSavingPoint().GetY(), vertScaleType);
	displayDataMinMax.NormalizeMinMax();

	double y = CDataConversion::AbsoluteToRelative_Y(rect, nDrawingPoint_Y, displayDataMinMax.GetMin_ScaleType(), displayDataMinMax.GetMax_ScaleType());
	return vertScaleType.ApplyExp(y);
}

int CFiboRetraceDrawer::GetDrawingPoint_Y(const int origin, const int nHeight, const int nIndex, const CVertScaleType& vertScaleType) const
{
	int nLength = CMath::Round((double)nHeight * GetCoefficient(nIndex));

	if(vertScaleType.IsInvert())
		return (origin + nLength);
	else
		return (origin - nLength);
}

///////////////////////////////////////////////////////////////////////////////
// class CFiboTimeDrawer -- �ð��� �׸���

// fibo �� ��ǥ�� ���ϱ�.
bool CFiboTimeDrawer::GetPoint(const CDisplayData& displayData, const CPoint& startPoint, const int nIndex, CPoint& fiboPoint) const
{
	if(displayData.IsRelativePointType())
	{
		return GetPoint_Data(GetCoordinateCalRegion(displayData), startPoint, nIndex, fiboPoint);
	}
		
	return GetPoint_Pixel(startPoint, nIndex, fiboPoint);
}

// protected ==================================================================
bool CFiboTimeDrawer::IsDrawing(const CRect& region, const CPoint& point) const
{
	return true;
}

// 2008.08.20 ������ �Ǻ���ġ Ȯ��	 COnePointDrawingData ������ �̿��Ͽ� �Ǻ���� ���ϱ� ������ ����<<
//void CFiboTimeDrawer::DrawShape(CDC* pDC, const CDisplayData& displayData, const CPoint& point, const bool /*isCurrentRegion*/)
void CFiboTimeDrawer::DrawShape(CDC* pDC, const COnePointDrawingData& drawingData, const CDisplayData& displayData, const CPoint& point, const bool /*isCurrentRegion*/)
{
// 2008.08.20 ������ �Ǻ���ġ Ȯ��
//		����ڷκ��� ����� �Է� �޾� ó���Ѵ�	<<
	CString strCoefficient = drawingData.GetDataType().GetExtData();
	if( strCoefficient.IsEmpty() || strCoefficient.CompareNoCase("All") == 0 )	
	{	// ���� ���ų� "All"�θ� ����Ǿ� �ִ� ��� ������ ���� ����
		strCoefficient = "0,1,2,3,5,8,13,21,34,55";
	}
	
	if( strCoefficient != m_strCoefficient )	// ���� ���� �ٸ� ��쿡�� ���� Coefficient�� ���Ѵ�.
	{
		m_strCoefficient = strCoefficient;
		m_arrCoefficient.RemoveAll();

		CString strBuf = strCoefficient;
		CString strR;
		while( !strBuf.IsEmpty() )
		{
			strR = CDataConversion::GetStringData(strBuf, ",");
			if( strR.IsEmpty() )		
			{
				strR = strBuf;
				strBuf.Empty();
			}
			if( strR.IsEmpty() )
				break;

			if( strR.CompareNoCase("All") )
				m_arrCoefficient.Add(atoi(strR));
		}
	}
//>>
	if(displayData.IsRelativePointType())
		DrawFigure_Data(pDC, displayData, point);
	else
		DrawFigure_Pixel(pDC, displayData.GetDisplayRegion(), point);
}

// private ====================================================================
// �Ǻ���� : 0, 1, 2, 3, 5, 8, 13, .... a1, a2, (a1 + a2)
// data ������ �׸���.
void CFiboTimeDrawer::DrawFigure_Data(CDC* pDC, const CDisplayData& displayData, const CPoint& point)
{
	CRegion coordinateCalRegion = GetCoordinateCalRegion(displayData);
	int nCurrentDataIndex = CDrawer::GetCurrentDataIndex(coordinateCalRegion, point);
	int nDisplayStartIndex = displayData.GetDisplayAttributes().GetDataStartInBlock();
	int nDisplayEndIndex = displayData.GetDisplayAttributes().GetDataEndInBlock();
	int nFiboCoefficient1 = 0, nFiboCoefficient2 = 0;
	int nIndex = 0;	// 2008.08.20 ������ �Ǻ���ġ Ȯ��
	
	while(nCurrentDataIndex < nDisplayEndIndex)
	{
		// �׸���
		if(nCurrentDataIndex >= nDisplayStartIndex)
		{
			int drawPt_X = CDrawer::GetDrawingPoint_X(coordinateCalRegion, nCurrentDataIndex, false);
			CDrawer::DrawLine(pDC, CPoint(drawPt_X, displayData.GetDisplayRegion().top), 
				CPoint(drawPt_X, displayData.GetDisplayRegion().bottom));
		}

		// 2008.08.20 ������ �Ǻ���ġ Ȯ��	<<
		nIndex++;
		nFiboCoefficient2 = GetCoefficient(nIndex);
		if( nFiboCoefficient2 < 0 )		break;
		//ChangeFiboCoefficient(nFiboCoefficient1, nFiboCoefficient2);
		//>>
		nCurrentDataIndex += nFiboCoefficient2;
	}
}

// point ������ �׸���.
void CFiboTimeDrawer::DrawFigure_Pixel(CDC* pDC, const CRect& region, const CPoint& point)
{
	int nIndex = 0, nFiboCoefficient1 = 0, nFiboCoefficient2 = 0;
	CPoint drawPt1(point.x, region.top), drawPt2(point.x, region.bottom);
	
	while(drawPt1.x < region.right)
	{
		drawPt1.x = drawPt2.x = drawPt1.x + nFiboCoefficient2;
		CDrawer::DrawLine(pDC, drawPt1, drawPt2);

		// 2008.08.20 ������ �Ǻ���ġ Ȯ��	<<
		nIndex++;
		nFiboCoefficient2 = GetCoefficient(nIndex);
		if( nFiboCoefficient2 < 0 )		break;
		//ChangeFiboCoefficient(nFiboCoefficient1, nFiboCoefficient2);
		//>>
	}
}

// ----------------------------------------------------------------------------
// data ������ fibo ��ǥ�� ���ϱ�.
bool CFiboTimeDrawer::GetPoint_Data(const CRegion& region, const CPoint& startPoint, const int nIndex, CPoint& fiboPoint) const
{
	int nCurrentDataIndex = CDrawer::GetCurrentDataIndex(region, startPoint);
	if(nCurrentDataIndex < 0)
		return false;

	int nFiboCoefficient1 = 0, nFiboCoefficient2 = 0;

	CPoint point = startPoint;
	for(int i = 0; i <= nIndex; i++)
	{
		if(nIndex == i)
		{
			int drawPt_X = CDrawer::GetDrawingPoint_X(region, nCurrentDataIndex, false);
			fiboPoint = CPoint(drawPt_X, startPoint.y);
			return true;
		}

		// 2008.08.20 ������ �Ǻ���ġ Ȯ��	<<
		nFiboCoefficient2 = GetCoefficient(i);
		if( nFiboCoefficient2 < 0 )		break;
		//ChangeFiboCoefficient(nFiboCoefficient1, nFiboCoefficient2);
		//>>
		nCurrentDataIndex += nFiboCoefficient2;
	}

	return false;
}

// point ������ fibo ��ǥ�� ���ϱ�.
bool CFiboTimeDrawer::GetPoint_Pixel(const CPoint& startPoint, const int nIndex, CPoint& fiboPoint) const
{
	int nFiboCoefficient1 = 0, nFiboCoefficient2 = 0;
	CPoint point = startPoint;

	for(int i = 0; i <= nIndex; i++)
	{
		point.x = point.x + nFiboCoefficient2;
		if(nIndex == i)
		{
			fiboPoint = point;
			return true;
		}
		
		// 2008.08.20 ������ �Ǻ���ġ Ȯ��	<<
		nFiboCoefficient2 = GetCoefficient(i);
		if( nFiboCoefficient2 < 0 )		break;
		//ChangeFiboCoefficient(nFiboCoefficient1, nFiboCoefficient2);
		//>>
	}

	return false;
}

// fibo ��ǥ�� ���ϱ� ���� data.
CRegion CFiboTimeDrawer::GetCoordinateCalRegion(const CDisplayData& displayData) const
{
	return CRegion(displayData.GetCoordinateCalRegion(), displayData.GetDisplayAttributes());
}

// ----------------------------------------------------------------------------
// �Ǻ���ġ ����� �ٲٱ�.
void CFiboTimeDrawer::ChangeFiboCoefficient(int& nFiboCoefficient1, int& nFiboCoefficient2) const
{
	int nNewFiboCoefficient = GetCoefficient(nFiboCoefficient1, nFiboCoefficient2);
	nFiboCoefficient1 = nFiboCoefficient2;
	nFiboCoefficient2 = nNewFiboCoefficient;
}

// ----------------------------------------------------------------------------
// �Ǻ���ġ ����� ��������.
int CFiboTimeDrawer::GetCoefficient(const int nFiboCoefficient1, const int nFiboCoefficient2) const
{
	if(nFiboCoefficient2 == 0)
		return 1;
	else if(nFiboCoefficient2 == 1)
		return 2;
	return (nFiboCoefficient1 + nFiboCoefficient2);
}

// 2008.08.20 ������ �Ǻ���ġ Ȯ��<<
//		����ڷκ��� ����� �Է� �޾� ó���Ѵ�	<<
int CFiboTimeDrawer::GetCount() const
{
	return m_arrCoefficient.GetSize();
}

int CFiboTimeDrawer::GetCoefficient(const int nIndex) const
{
	if( nIndex < 0 || nIndex >= m_arrCoefficient.GetSize() )
		return -1;

	return (int)m_arrCoefficient.GetAt(nIndex);
}

CString CFiboTimeDrawer::GetText(const int nIndex) const
{
	int nCoefficient = GetCoefficient(nIndex);
	double	dCoefficient = 0.0;
	switch( nCoefficient )
	{
	case 0:		dCoefficient = 0.0;			break;
	case 1:		dCoefficient = 23.6;		break;
	case 2:		dCoefficient = 38.2;		break;
	case 3:		dCoefficient = 50.0;		break;
	case 5:		dCoefficient = 61.8	;		break;
	case 8:		dCoefficient = 78.6	;		break;
	case 13:	dCoefficient = 100.0;		break;
	case 21:	dCoefficient = 127.2;		break;
	case 34:	dCoefficient = 161.8;		break;
	case 55:	dCoefficient = 200.0;		break;
	case 89:	dCoefficient = 261.8;		break;
	};
	CString strCoefficient;
	strCoefficient.Format("%d (%.1f%%)", nCoefficient, dCoefficient);

	return strCoefficient;

/*	double dCoefficient = GetCoefficient(nIndex) * 100.0;
	CString strCoefficient = CDataConversion::DoubleToString(dCoefficient, 1);
	if(strCoefficient.IsEmpty())
		return "";

	return strCoefficient + "%";
*/
}

void CFiboTimeDrawer::DrawText(CDC* pDC, const COnePointDrawingData& drawingData)
{
	if(!CVertLineDrawer::IsDrawing(drawingData.GetCurrentRegion(), drawingData.GetPoints().GetDrawingPoint()))
		return;

	CTextShowPosition* pTextPosition = (CTextShowPosition *) drawingData.GetTextPosition();
	if(pTextPosition == NULL)
		return;
	if(!pTextPosition->IsShow())
		return;

	int idxRegion = drawingData.GetDrawingRegions()->GetBlockRegionList().GetCurrentNumber();
	CGraphPartRegion graphPartRegion = drawingData.GetDrawingRegions()->GetGraphPartRegion(idxRegion);
	CDisplayData displayData(drawingData.GetDataType().GetCoordinateType(), 
		graphPartRegion, drawingData.GetDisplayAttributes());
	CPoint point(drawingData.GetPoints().GetDrawingPoint());

	CRegion coordinateCalRegion = GetCoordinateCalRegion(displayData);
	int nCurrentDataIndex = CDrawer::GetCurrentDataIndex(coordinateCalRegion, point);
	int nDisplayStartIndex = displayData.GetDisplayAttributes().GetDataStartInBlock();
	int nDisplayEndIndex = displayData.GetDisplayAttributes().GetDataEndInBlock();
	int nFiboCoefficient1 = 0, nFiboCoefficient2 = 0;
	int nIndex = 0;	// 2008.08.20 ������ �Ǻ���ġ Ȯ��
	
	CString strText;
	CSize maxSize = pDC->GetTextExtent("89(423.6%)");

	int drawPt_X = 0;
//	int drawPt_Y = displayData.GetDisplayRegion().top+maxSize.cy;
	int drawPt_Y = (displayData.GetDisplayRegion().Height()-maxSize.cy)/2 + 1;

	
	while(nCurrentDataIndex < nDisplayEndIndex)
	{
		drawPt_X = CDrawer::GetDrawingPoint_X(coordinateCalRegion, nCurrentDataIndex, false);
		strText = GetText(nIndex);
		//CDrawer::DrawTextOut(pDC,CDrawer::TOP_RIGHT,strText,CPoint(drawPt_X, drawPt_Y),
		//	maxSize);
		DrawText_stand(pDC,CDrawer::TOP_RIGHT,strText,CPoint(drawPt_X, drawPt_Y),maxSize);
		nIndex++;
		nFiboCoefficient2 = GetCoefficient(nIndex);
		if( nFiboCoefficient2 < 0 )		break;
		nCurrentDataIndex += nFiboCoefficient2;
	}

	return;
}

void CFiboTimeDrawer::DrawText_stand(CDC* pDC, const TEXTPOSITION eTextPosition, const CString& data, const CPoint& point, const CSize& maxSize)
{
	CFont            m_fontVertCaption;
	CFont			 font;
	if (font.CreateStockObject (DEFAULT_GUI_FONT))
	{
     		LOGFONT lf;
       		memset (&lf, 0, sizeof (LOGFONT));
       		if (font.GetLogFont (&lf) != 0)
       		{
       			lf.lfOrientation = 900;
       			lf.lfEscapement = 900;
       			lf.lfHeight = maxSize.cy;
       			lf.lfWidth = 0;
       			lf.lfWeight = 400;
				m_fontVertCaption.CreateFontIndirect (&lf);
       		}
	}
	CFont* pOldFont = pDC->SelectObject (pDC->GetCurrentFont());
	pDC->SelectObject(m_fontVertCaption);
	int MODE = pDC->SetBkMode(TRANSPARENT);
	CDrawer::DrawTextOut(pDC, eTextPosition, data, point, maxSize);
	pDC->SelectObject(pOldFont);
	m_fontVertCaption.DeleteObject();
}
//>>

///////////////////////////////////////////////////////////////////////////////
// class CGannDrawer

void CGannDrawer::DrawShape(CDC* pDC, const CTwoPointDrawingData& drawingData, const CPoint& pt1, const CPoint& pt2)
{
	if(!IsDrawing(pt1, pt2))
		return;

	DrawShape(pDC, drawingData.GetDisplayData().GetDisplayRegion(), pt1, pt2);
}

// private ====================================================================
// �׸����� ����
bool CGannDrawer::IsDrawing(const CPoint& pt1, const CPoint& pt2) const
{
	return (pt1.x != pt2.x && pt1.y != pt2.y);
}

///////////////////////////////////////////////////////////////////////////////
// class CGannLineDrawer

// �׸� ��ǥ��.
bool CGannLineDrawer::GetLineEndPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, CPoint& point) const
{
	// pt2�� ������ ����
	if(pt1.x < pt2.x)
		return CDrawer::Intersection(pt1, pt2, CPoint(region.right, region.top), CPoint(region.right, region.bottom), point);
	// pt2�� ���� ����
	else if(pt1.x > pt2.x)
		return CDrawer::Intersection(pt1, pt2, region.TopLeft(), CPoint(region.left, region.bottom), point);
	// pt2�� ���� ����
	else if(pt1.x == pt2.x && pt1.y > pt2.y)
		return CDrawer::Intersection(pt1, pt2, CPoint(region.left, region.top), CPoint(region.right, region.top), point);
	// pt2�� �Ʒ��� ����
	else if(pt1.x == pt2.x && pt1.y < pt2.y)
		return CDrawer::Intersection(pt1, pt2, CPoint(region.left, region.bottom), CPoint(region.right, region.bottom), point);

	return false;
}

// protected ==================================================================
void CGannLineDrawer::DrawShape(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2)
{
	DrawPairLine(pDC, region, pt1, pt2);
}

bool CGannLineDrawer::DrawPairLine(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2)
{
	CPoint point;
	if(!GetLineEndPoint(region, pt1, pt2, point))
		return false;

	CDrawer::DrawLine(pDC, pt1, point);
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// class CGannFanDrawer

int CGannFanDrawer::GetCount() const
{
	return 9;
}

bool CGannFanDrawer::GetLineEndPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, const int nIndex, CPoint& point) const
{
	if(nIndex < 0 || nIndex > GetCount())
		return false;

	CPoint anglePoint = GetAnglePoint(pt1, pt2, nIndex);
	return CGannLineDrawer::GetLineEndPoint(region, pt1, anglePoint, point);
}

// protected ==================================================================
void CGannFanDrawer::DrawText(CDC* pDC, const CTwoPointDrawingData& drawingData)
{
	CTextShowPosition* pTextPosition = (CTextShowPosition*) drawingData.GetTextPosition();
	if(pTextPosition == NULL)
		return;

	if(pTextPosition->IsShow())
	{
		CString strText;
		CPoint anglePoint;
		CSize maxSize = pDC->GetTextExtent("3x3");
		CPoint pt1 = drawingData.GetStartPoints().GetDrawingPoint();
		CPoint pt2 = drawingData.GetEndPoints().GetDrawingPoint();
		CRect region = drawingData.GetDiaplayRegion();

		int n4DiffSize = 0;
		int n3DiffSize = 0;
		int n2DiffSize = 0;
		int n1DiffSize = 0;
		CPoint pointCenter = GetAnglePoint(pt1, pt2, 4);
		for(int nIndex = GetCount()-1; nIndex >= 0 ; nIndex--)
		{
			strText = GetText(nIndex);
			if(nIndex < 4)
			{
				anglePoint.y = pt2.y;
				if(nIndex == 3)
					anglePoint.x = pt2.x - abs(pt2.y - n1DiffSize)+15;
				else if(nIndex == 2)
					anglePoint.x = pt2.x - abs(pt2.y - n2DiffSize)-2;
				else if(nIndex == 1)
					anglePoint.x = pt2.x - abs(pt2.y - n3DiffSize)-15;
				else if(nIndex == 0)
					anglePoint.x = pt2.x - abs(pt2.y - n4DiffSize)-20;
				CDrawer::DrawTextOut(pDC,CDrawer::TOP_RIGHT,strText,anglePoint,maxSize);	
			}
			else if(nIndex == 4)
			{
				continue;
			}
			else 
			{
				anglePoint = GetAnglePoint(pt1, pt2, nIndex);
				if(nIndex==8)
				{
					n4DiffSize = anglePoint.y;
					anglePoint.y = anglePoint.y + 7;
				}
				else if(nIndex==7)
				{
					n3DiffSize = anglePoint.y;
					anglePoint.y = anglePoint.y + nIndex - 1;
				}
				else if(nIndex==6)
				{
					n2DiffSize = anglePoint.y;
					anglePoint.y = anglePoint.y + nIndex - 1;
				}
				else if(nIndex==5)
				{
					anglePoint.y = anglePoint.y + nIndex - 1;
					n1DiffSize = anglePoint.y;					
				}
				CDrawer::DrawTextOut(pDC,CDrawer::TOP_RIGHT,strText,anglePoint,maxSize);
			}	
		}
	}
}

void CGannFanDrawer::DrawShape(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2)
{
	for(int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		CPoint anglePoint = GetAnglePoint(pt1, pt2, nIndex);
		CGannLineDrawer::DrawPairLine(pDC, region, pt1, anglePoint);
	}
}

// private ====================================================================
// ������ ���� : 90�� ~ 45�� -> 2:2:1:2:5 �̴�.
// ex) org angle : 45 -> 82.5, 75, 71.25, 63.75, 45, 26.25, 18.75, 15, 7.5
// Ȯ�� ��Ҹ� ������� ���� �����ν� ������� ����
double CGannFanDrawer::GetAngle(const int nIndex) const
{
	switch(nIndex)
	{
	case 0:
		return 37.5;
	case 1:
		return 30.0;
	case 2:
		return 26.25;
	case 3:
		return 18.75;
	case 4:
		return 0.0;
	case 5:
		return -18.75;
	case 6:
		return -26.25;
	case 7:
		return -30.0;
	case 8:
		return -37.5;
	}

	assert(false);
	return 0.0;
}

CString CGannFanDrawer::GetText(const int nIndex) const
{
	CString strGannFan;
	switch(nIndex)
	{
	case 0:
		strGannFan.Format("1x4");
		break;
	case 1:
		strGannFan.Format("1x3");
		break;
	case 2:
		strGannFan.Format("1x2");
		break;
	case 3:
		strGannFan.Format("1x1");
		break;
	case 4:
		//strGannFan.Format("1x1");
		break;
	case 5:
		strGannFan.Format("1x1");
		break;
	case 6:
		strGannFan.Format("2x1");
		break;
	case 7:
		strGannFan.Format("3x1");
		break;
	case 8:
		strGannFan.Format("4x1");
		break;
	}

	return strGannFan;
}
//////////////////////////////////////////////////////////////////////////
// 2006.7 �߰�
// Ȯ�� ������ ���� ������ ����
double CGannFanDrawer::GetAngle(const int nIndex, double dZFactor) const
{
	double pi = 3.1415926535897931;
	double f = 0.0175;
	switch(nIndex)
	{
	case 0:
		return atan(8.0 * dZFactor)*180/pi;
	case 1:
		return atan(4.0 * dZFactor)*180/pi;
	case 2:
		return atan(3.0 * dZFactor)*180/pi;
	case 3:
		return atan(2.0 * dZFactor)*180/pi;
	case 4:
		return atan(1.0 * dZFactor)*180/pi;
	case 5:
		return atan(1/2.0 * dZFactor)*180/pi;
	case 6:
		return atan(1/3.0 * dZFactor)*180/pi;
	case 7:
		return atan(1/4.0 * dZFactor)*180/pi;
	case 8:
		return atan(1/8.0 * dZFactor)*180/pi;
	}

	assert(false);
	return 0.0;
}

//////////////////////////////////////
// ������� ����
// �׷��� GetAngle �Լ��� ������� �ʰ� 
// �Ʒ��� ������� ������ ���� ����
const double dGannFactor[] =
{
	8.0,4.0,3.0,2.0,1.0,0.1/2.0,1/3.0,1/4.0,1/8.0
};

const double dGannAngle[] =
{
	37.5,30.0,26.25,18.75,0,-18.75,-26.25,-30,-37.5
};
//////////////////////////////////////////

CPoint CGannFanDrawer::GetAnglePoint(const CPoint& pt1, const CPoint& pt2, const int nIndex) const
{
	// 2006.7 ����
	// Ȯ�� ���ο� ���� �̹� �׸� ������ ����
	// Ȯ�� ������ ���� ����Ǵ� ���ؼ��� ����(dOrgAngle)�� ���� Ȯ�� ��Ҹ� ����(dZFactor)
	// �ֺ����� ������ �� dZFactor�� �ݿ��Ͽ� ����
	// �� Ȯ�� �� ���Ӱ� Gann Fan�� �׸��� 45�� ����, �� Ȯ�� �� Gann Fan���� ���̰� ����.
	double dOrgAngle = GetOrgAngle(pt1, pt2);
	double dZoomFactor = CMath::TanD(dOrgAngle);
	double dTan = CMath::TanD(GetAngle(nIndex, dZoomFactor));
	
	if(dTan == 0.0)
		return CPoint(pt2.x, pt1.y);

	int y = CMath::Round((-dTan * (pt2.x - pt1.x) + pt1.y));
	CPoint ptReturn = CPoint(pt2.x, y);
	return ptReturn;
}

double CGannFanDrawer::GetOrgAngle(const CPoint& pt1, const CPoint& pt2) const
{
	// x��
	if(pt1.y == pt2.y)
	{
		if(pt1.x <= pt2.x)
			return 0.0;
		return 180.0;
	}
	// y��
	if(pt1.x == pt2.x)
	{
		if(pt1.y <= pt2.y)
			return 270.0;
		return 90.0;
	}

	double dOrgTan = CDrawer::GetLineAngle(pt1, pt2) * -1;
	return CMath::Angle(dOrgTan);
}

///////////////////////////////////////////////////////////////////////////////
// class CGannGridDrawer

// �׸� ��ǥ��.
bool CGannGridDrawer::GetLineEndPoints(const CRect& region, const CPoint& pt1, const CPoint& pt2, 
		const int nIndex, CPoint& point1, CPoint& point2) const
{
	// �ش� ��ǥ�� ���� ������
	if(nIndex <= 0)
		return GetRectangleIntersectionPoint(region, pt1, pt2, point1, point2);

	// side ��ǥ�� ���� ������
	return GetPoints_Side(region, pt1, pt2, nIndex, point1, point2);
}

// protected ==================================================================
void CGannGridDrawer::DrawShape(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2)
{
	DrawPairLine(pDC, region, pt1, pt2);
	DrawOneSide(pDC, region, pt1, pt2, (pt1.x - pt2.x));
	DrawOneSide(pDC, region, pt1, pt2, (pt2.x - pt1.x));
}

// private ====================================================================
void CGannGridDrawer::DrawOneSide(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2, const int length_x)
{
	CPoint drawingPt1 = pt1;
	CPoint drawingPt2 = pt2;

	while(true)
	{
		drawingPt1 = GetNextPoint(drawingPt1, length_x);
		drawingPt2 = GetNextPoint(drawingPt2, length_x);
		if(!DrawPairLine(pDC, region, drawingPt1, drawingPt2))
			break;
	}
}

bool CGannGridDrawer::DrawPairLine(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2)
{
	bool isSlashDrawn = DrawSlashLine(pDC, region, pt1, pt2);
	bool isReverseSlashDrawn = DrawReverseSlashLine(pDC, region, pt1, pt2);

	return isSlashDrawn || isReverseSlashDrawn;
}

bool CGannGridDrawer::DrawSlashLine(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2)
{
	return DrawLine(pDC, region, pt1, pt2);
}

bool CGannGridDrawer::DrawReverseSlashLine(CDC* pDC, const CRect& region, const CPoint& pt1, const CPoint& pt2)
{
	return DrawLine(pDC, region, pt1, GetSymmetryPointOfXAxis(pt1, pt2));
}

bool CGannGridDrawer::DrawLine(CDC* pDC, const CRect& region, const CPoint& lineStart, const CPoint& lineEnd)
{
	CPoint start, end;

	if(GetRectangleIntersectionPoint(region, lineStart, lineEnd, start, end))
	{
		CDrawer::DrawLine(pDC, start, end);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
// line1 or line2 �� �׸��� �������� �׸��� ���� ���� ��ǥ.
CPoint CGannGridDrawer::GetNextPoint(const CPoint& point, const int length_x) const
{
	return CPoint((point.x + (length_x *2)), point.y);
}

// line2 �� �׸� x�࿡ ��Ī�� �Ǵ� ��ǥ.
CPoint CGannGridDrawer::GetSymmetryPointOfXAxis(const CPoint& orgPt, const CPoint& point) const
{
	return CPoint(point.x, (point.y + ((orgPt.y - point.y) * 2)));
}

// ----------------------------------------------------------------------------
bool CGannGridDrawer::GetRectangleIntersectionPoint(const CRect& region, const CPoint& pt1, const CPoint& pt2, CPoint& start, CPoint& end) const
{
	// ������ ������ -- ������ ������
	bool ret1 = GetIntersectionPoint_VertLine(pt1, pt2, CPoint(region.right, region.top), CPoint(region.right, region.bottom), start);
	// ������ ������ -- ���� ������
	bool ret2 = GetIntersectionPoint_VertLine(pt1, pt2, region.TopLeft(), CPoint(region.left, region.bottom), end);

	if(ret1 || ret2)
		return true;

	// ������ ������ -- ���� ����
	if(GetIntersectionPoint_HorzLine(pt1, pt2, CPoint(region.left, region.bottom), CPoint(region.right, region.bottom)))
		return true;

	// ������ ������ -- �Ʒ��� ����
	if(GetIntersectionPoint_HorzLine(pt1, pt2, CPoint(region.left, region.bottom), CPoint(region.right, region.bottom)))
		return true;

	return false;
}

// ������ ������ -- ���� ������
bool CGannGridDrawer::GetIntersectionPoint_VertLine(const CPoint& line1_Pt1, const CPoint& line1_Pt2, const CPoint& line2_Pt1, const CPoint& line2_Pt2, CPoint& intersectionPoint) const
{
	bool result = CDrawer::Intersection(line1_Pt1, line1_Pt2, line2_Pt1, line2_Pt2, intersectionPoint);
	if(result)
	{
		if(intersectionPoint.y > line2_Pt2.y || intersectionPoint.y < line2_Pt1.y)
			return false;
	}

	return result;
}

// ������ ������ -- ����
bool CGannGridDrawer::GetIntersectionPoint_HorzLine(const CPoint& line1_Pt1, const CPoint& line1_Pt2, const CPoint& line2_Pt1, const CPoint& line2_Pt2) const
{
	CPoint intersectionPoint;
	bool result = CDrawer::Intersection(line1_Pt1, line1_Pt2, line2_Pt1, line2_Pt2, intersectionPoint);
	if(result)
	{
		if(intersectionPoint.x < line2_Pt1.x || intersectionPoint.x > line2_Pt2.x)
			return false;
	}

	return result;
}

// ----------------------------------------------------------------------------
// side�� ���� �׸� ��ǥ��.
bool CGannGridDrawer::GetPoints_Side(const CRect& region, const CPoint& pt1, const CPoint& pt2, 
		const int nIndex, CPoint& point1, CPoint& point2) const
{
	// side ��ǥ�� ���� ������
	CPoint drawingPt1 = pt1;
	CPoint drawingPt2 = pt2;
	int count = 1;
	while(true)
	{
		drawingPt1 = GetNextPoint(drawingPt1, (pt1.x - pt2.x));
		drawingPt2 = GetNextPoint(drawingPt2, (pt1.x - pt2.x));

		if(count == nIndex)
			return GetRectangleIntersectionPoint(region, drawingPt1, drawingPt2, point1, point2);

		count++;
	}

	return false;
}
