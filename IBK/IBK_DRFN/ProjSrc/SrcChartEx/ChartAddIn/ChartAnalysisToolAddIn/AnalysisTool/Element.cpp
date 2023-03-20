// Element.cpp: implementation of the CElement class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Element.h"

#include "DrawingData.h"
#include "CoordinateCalculator.h"
#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CElement

CElement::CElement(const CElementCommonEnvironment& environment):
	m_environment(environment)
{
}

// ������ element data.
CString CElement::GetSavedElementData(const COrder& order) const
{
	CString savedElementData("<Element>\r\n");
	// index ��
	CString strIndex = GetSavedIndex(order);
	if(strIndex.IsEmpty())
		return "";

	savedElementData += strIndex;
	// element data
	savedElementData += GetSavedElement();
	return savedElementData + "<\\Element>\r\n";
}

CElementCommonEnvironment CElement::GetCommonEnvironment() const
{
	return m_environment;
}

CElementCommonEnvironment& CElement::GetCommonEnvironment()
{
	return m_environment;
}

CAnalysis::COORDINATETYPE CElement::GetCoordinateType() const
{
	return m_environment.GetDataType().GetCoordinateType();
}

void CElement::ChangePenColor(const COLORREF& color)
{
	m_environment.GetPen().SetColor(color);
}

void CElement::ChangeTextColor(const COLORREF& color)
{
	CTextPosition* pTextPosition = GetTextPosition();
	if(pTextPosition == NULL)
		return;

	pTextPosition->GetTextEnvironment().SetColor(color);
}

//[A00000552]alzioyes_20110628 element ���β� �����ϱ�.
void CElement::ChangePenThick(int nNewWidth)
{
	if(m_environment.GetPen().GetStyle()==PS_SOLID)
		m_environment.GetPen().SetWidth(nNewWidth);
}

// ----------------------------------------------------------------------------
const CTextPosition* CElement::GetTextPosition() const
{
	return NULL;
}

CTextPosition* CElement::GetTextPosition()
{
	return NULL;
}

// ----------------------------------------------------------------------------
void CElement::Draw(CDC* pDC,const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData)
{
	CPen newPen;
	CreatePen(true, newPen);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	RealDraw(pDC,drawingRegions, calData);

//>> ȣ����Ʈ�� �м����� �������󰡱� ���� ���� - vntsorl(20090202)
	if( GetElementType() == CToolEnumData::Linear_Regression ||
		GetElementType() == CToolEnumData::Raff_Regression )
		GetCommonEnvironment().SetAutoPrice(false);
//<< ȣ����Ʈ�� �м����� �������󰡱� ���� ���� - vntsorl(20090202)

	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

// protected ==================================================================
// �׸� ������ǥ ��������
CPoint CElement::GetAbsolutePoint(const CCoordinateCalculatorData& calData, const CCoordinate& point) const
{
	if(!m_environment.GetDataType().IsRelativePointType())
		return point.GetPixel();

	return GetCoordinateCalculator().RelativeToAbsolute(calData, point);
}

CPoint CElement::GetSelectedDrawPoint(const CSelectedElementDrawingData& selectedDrawingData, const CCoordinate& point) const
{
	return selectedDrawingData.GetSelectedDrawPoint(GetCoordinateType(), m_environment.IsAutoPrice(), point);
}

CPoints CElement::GetSelectedDrawPoints(const CSelectedElementDrawingData& selectedDrawingData, const CCoordinate& point) const
{
	return CPoints(point, GetSelectedDrawPoint(selectedDrawingData, point));
}

CPoints CElement::GetSelectedDrawPoints(const CCoordinateCalculatorData& calData, const CCoordinate& point) const
{
	return CPoints(point, GetAbsolutePoint(calData, point));
}

CString CElement::GetSavedPoint(const CCoordinate& point)
{
	CString savedPoint("<Point>");
	// ����Ʈ �����ð� ���� - ojtaso (20080811)
	// 20081007 JS.Kim	����ڽð��� ����. 
	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//savedPoint += CDataConversion::IntToString(point.GetX()) + "," 
	//					+ CDataConversion::DoubleToString(point.GetY()) + "," 
	//					+ CDataConversion::IntToString(point.GetTimeDiff()) + "," 
	//					+ point.GetDateType();

	double dDataX		= point.GetX();
	double dDataY		= point.GetY();
	int	   nTimeDiff	= point.GetTimeDiff();
	CString strType		= point.GetDateType();

	//
	CString strDataX	= CDataConversion::DoubleToString(dDataX);
	CString strDataY	= CDataConversion::DoubleToString(dDataY);
	CString strTimeDiff	= CDataConversion::IntToString(nTimeDiff);

	savedPoint  += strDataX + "," 
				+ strDataY + "," 
				+ strTimeDiff + "," 
				+ strType;
	// 2011.01.06 by SYS <<

	savedPoint += "<\\Point>\r\n";
	return savedPoint;
}

void CElement::CreatePen(const bool bIsElement, CPen& pen)
{
	m_environment.GetPen().CreatePen(bIsElement, pen);
}

// �����Ҽ� �ִ� ����
bool CElement::PointInElementSelectableRegion(const CPoint& regionPoint1, const CPoint& regionPoint2, const CPoint& point) const
{
	CRgn selectableRegion;
	CSelectableRegionCalculator selectableRegionCalculator;
	selectableRegionCalculator.GetLineRegion(regionPoint1, regionPoint2, selectableRegion);
	if( !HRGN( selectableRegion)) return false;
	return ( selectableRegion.PtInRegion(point) ? true : false);
}

bool CElement::PointInElementSelectableFrameRect(const CPoint& regionPoint1, const CPoint& regionPoint2, const CPoint& point) const
{
	CPoint point1(regionPoint1);
	CPoint point2(regionPoint2);

	point1.y = regionPoint2.y;
	if(CElement::PointInElementSelectableRegion(point1, point2, point))
		return true;

	point1.x = regionPoint2.x;
	point1.y = regionPoint1.y;
	if(CElement::PointInElementSelectableRegion(point1, point2, point))
		return true;

	point1.x = regionPoint1.x;
	point2.y = regionPoint1.y;
	if(CElement::PointInElementSelectableRegion(point1, point2, point))
		return true;

	point1.y = regionPoint1.y;
	point2.x = regionPoint1.x;
	point2.y = regionPoint2.y;
	if(CElement::PointInElementSelectableRegion(point1, point2, point))
		return true;
	
	return false;
}

CRect CElement::GetLineRect(const CRect &p_rctGraphArea,const CPoint &ptStart,const CPoint &ptEnd,BOOL bLeftExt,BOOL bRightExt) const
{
	CRect rcGraphArea(p_rctGraphArea);
	if(bLeftExt == FALSE)
	{
		rcGraphArea.left = min(ptStart.x,ptEnd.x);
	}

	if(bRightExt == FALSE)
	{
		rcGraphArea.right = max(ptStart.x,ptEnd.x);
	}

	// 2. ����� ������ �켱 ������ ������ ���Ѵ�.
	// 2.1 ���� Line �߼��� Ȯ���ϱ� ���Ͽ� ������ ��ȭ���� Ȯ���Ѵ�.
	double dDX = ptEnd.x - ptStart.x;
	double dDY = ptEnd.y - ptStart.y;
	// 2.2 �� �ະ�� Bound�� Ȯ���Ѵ�.
	CRect rctExtLineRect = rcGraphArea;
	// 2.2.1 ���� X���� Bound�� Ȯ���Ѵ�. ( Y Bound�� �־� X Bound�� ������ ����.)
	if( fabs( dDY) != 0)
	{
		int nX1 = int( ( dDX / dDY) * ( rcGraphArea.top		- ptStart.y) + ptStart.x);
		int nX2 = int( ( dDX / dDY) * ( rcGraphArea.bottom	- ptStart.y) + ptStart.x);
		rctExtLineRect.left		= max( min( nX1, nX2), rcGraphArea.left);
		rctExtLineRect.right	= min( max( nX1, nX2), rcGraphArea.right);
	}
	// 2.2.2 Y���� Bound�� Ȯ���Ѵ�. ( X Bound�� �־� X Bound�� ������ ����.)
	if( fabs( dDX) != 0)
	{
		int nY1 = int( ( dDY / dDX) * ( rcGraphArea.left	- ptStart.x) + ptStart.y);
		int nY2 = int( ( dDY / dDX) * ( rcGraphArea.right	- ptStart.x) + ptStart.y);
		rctExtLineRect.top		= max( min( nY1, nY2), rcGraphArea.top);
		rctExtLineRect.bottom	= min( max( nY1, nY2), rcGraphArea.bottom);
	}

	return rctExtLineRect;
}
// private ====================================================================
CCoordinateCalculator& CElement::GetCoordinateCalculator() const
{
	return CCoordinateCalculatorFinder::FindCoordinateCalculator(m_environment.IsAutoPrice());
}

// ----------------------------------------------------------------------------
CString CElement::GetSavedIndex(const COrder& order) const
{
	int nIndex = order.GetIndex(this);
	if(nIndex < 0)
		return "";

	return ("<Index>" + CDataConversion::IntToString(nIndex) + "<\\Index>\r\n");
}

CString CElement::GetSavedElement() const
{
	CString savedElementData;
	savedElementData += GetSavedToolType();
	savedElementData += GetSavedPoint();
	savedElementData += m_environment.GetSavedData();
	savedElementData += GetSavedTextPosition();
	return savedElementData;
}

CString CElement::GetSavedToolType() const
{
	return ("<ToolType>" + CDataConversion::IntToString((int) GetElementType()) + "<\\ToolType>\r\n");
}

CString CElement::GetSavedPoint() const
{
	return ("<Points>\r\n" + GetSavedPoints() + "<\\Points>\r\n");
}

CString CElement::GetSavedTextPosition() const
{
	const CTextPosition* pTextPosition = GetTextPosition();
	if(pTextPosition == NULL)
		return "";

	return pTextPosition->GetSavedData();
}

CString CElement::GetSavedLineExt() const
{
	return m_environment.GetLineExt().GetSavedData();
}

CPoint CElement::GetAbsoluteDrawingPoint(const CCoordinateCalculatorData& calData, const CCoordinate& point) const
{
	if(!m_environment.GetDataType().IsRelativePointType())
		return point.GetPixel();

	return GetCoordinateCalculator().RelativeToAbsolute(calData, point);
}