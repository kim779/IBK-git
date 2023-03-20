// PointElement.cpp: implementation of the CPointElement class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PointElement.h"

#include "../Include_Chart/DLL_Load/IMetaTable.h"						// _MTEXT0
#include "../Include_AddIn_133101/I133101/_IPacketListManager.h"		// for IPacketListManager
#include "../Include_AddIn_133101/I133101/_IDoubleList.h"				// for IDoubleList

#include "DrawingData.h"
#include "CoordinateCalculator.h"
#include "PointDrawer.h"
#include "DlgAnalysisToolSet.h"							// for CDlgAnalysisToolSet

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class COnePointElement

COnePointElement::COnePointElement(const CCoordinate& point, const CElementCommonEnvironment& environment):
	CElement(environment),
	m_point(point)
{
}

// public =====================================================================
// ���� ���õ� element�� ��ǥ�� ��������.
void COnePointElement::GetPoints(CElementPointList& elementPointList)
{
	elementPointList.SetVertDataType(CElement::GetCommonEnvironment().GetDataType().GetVert());
	elementPointList.AddTail(&m_point);
}

// point�� element ������ �ش� �Ǵ��� üũ
CToolEnumData::ModifyType COnePointElement::GetModifyType(CExtractingDataOfModifyType* pExtractingData) const
{
	CPoint drawingPoint = CElement::GetAbsolutePoint(pExtractingData->GetCoordinateCalculatorData(), m_point);

	for(int nIndex = 0; nIndex < pExtractingData->GetDrawingRegionCount(); nIndex++){
		CToolEnumData::ModifyType modifyType = GetModifyType(GetDisplayData(pExtractingData, nIndex), 
			drawingPoint, pExtractingData->GetMousePoint());
		if(modifyType != CToolEnumData::MODIFY_NONE)
			return modifyType;
	}

	return CToolEnumData::MODIFY_NONE;
}

CToolEnumData::ModifyType COnePointElement::GetModifyType_Point(const CPoint& drawingPt,const CPoint& point) const
{
	CSelectableRegionCalculator selectableRegionCalculator;
	if(selectableRegionCalculator.IsPointInCircle(drawingPt, point))
		return CToolEnumData::MODIFY_START;

	return CToolEnumData::MODIFY_NONE;
}

CCoordinate COnePointElement::GetCoordinate(const CToolEnumData::ModifyType eModifyType) const	
{
	CCoordinate coordValue;
	if(eModifyType == CToolEnumData::MODIFY_START)
	{
		return m_point;
	}

	return coordValue;
}
// ----------------------------------------------------------------------------
void COnePointElement::DrawShadowAndGrip(CDC* pDC, const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData)
{
	CPen newPen;
	CElement::CreatePen(false, newPen);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	GetOnePointDrawer().DrawShadowAndGrip(pDC, drawingRegions, GetGripPoint(calData));

	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

void COnePointElement::DrawSelected(CDC* pDC, const CSelectedElementDrawingData& selectedDrawingData)
{
	COnePointDrawingData drawingData = GetOnePointDrawingData(selectedDrawingData);

	GetOnePointDrawer().DrawTool(pDC, drawingData, R2_NOTXORPEN);//R2_NOT);
}

// protected ==================================================================
CCoordinate& COnePointElement::GetPoint()
{
	return m_point;
}

CCoordinate COnePointElement::GetPoint() const
{
	return m_point;
}

CString COnePointElement::GetSavedPoints() const
{
	return CElement::GetSavedPoint(m_point);
}

CPoint COnePointElement::GetSelectedDrawPoint(const CSelectedElementDrawingData& selectedDrawingData) const
{
	return CElement::GetSelectedDrawPoint(selectedDrawingData, m_point);
}

COnePointDrawingData COnePointElement::GetOnePointDrawingData(const CSelectedElementDrawingData& selectedDrawingData) const
{
	return GetOnePointDrawingData(selectedDrawingData.GetDrawingRegions(), 
			selectedDrawingData.GetCoordinateCalculatorData(), 
			CPoints(selectedDrawingData.GetCoordinate(),GetSelectedDrawPoint(selectedDrawingData)));
}

// ----------------------------------------------------------------------------
void COnePointElement::SetPoint(const CCoordinate& point)
{
	m_point = point;
}

void COnePointElement::RealDraw(CDC* pDC,const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData)
{
	COnePointDrawingData drawingData = 
		GetOnePointDrawingData(&drawingRegions, calData, 
		CPoints(m_point,CElement::GetAbsolutePoint(calData, m_point)));

	GetOnePointDrawer().DrawElement(pDC, drawingData);
}

// ----------------------------------------------------------------------------
// ���� �׸���ǥ. (�߽��� or ������)
CPoint COnePointElement::GetGripPoint(const CCoordinateCalculatorData& calData)
{
	// �߽���
	return CElement::GetAbsolutePoint(calData, m_point);
}

// private ====================================================================
COnePointDrawingData COnePointElement::GetOnePointDrawingData(const CDrawingRegions* pDrawingRegions, 
															  //const CCoordinateCalculatorData& calData, const CPoint& drawingPoint) const
															  const CCoordinateCalculatorData& calData, const CPoints& Points) const
															  
{

	// 2011.01.27 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	//return COnePointDrawingData(CElement::GetCommonEnvironment().GetDataType(), const_cast<CTextPosition*> (GetTextPosition()), 
	//							&CElement::GetCommonEnvironment().GetLineExt(),
	//							pDrawingRegions, CDisplayAttributes(calData.GetDisplayDataStart(), calData.GetDisplayDataEnd()), 
	//							calData.GetVertScaleType(), Points);

	ILPCSTR szHelpMsg = NULL;
	CString strDataType;	
	//CCoordinateCalculatorData calData = GetCoordinateCalculatorData( CMoveTool::GetPoints().GetDrawingPoint());
	ILPCSTR szRQ = calData.m_pIChartOCX->GetCurrentRQ();
	IPacketManager * pCurPacketManager = calData.m_pIPacketListManager->GetPacketManager(szRQ);
	pCurPacketManager->GetHelpMessageItem("UD", szHelpMsg); strDataType = szHelpMsg;
	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)atoi(strDataType);
	//

	return COnePointDrawingData(CElement::GetCommonEnvironment().GetDataType(), const_cast<CTextPosition*> (GetTextPosition()), 
								&CElement::GetCommonEnvironment().GetLineExt(),
								pDrawingRegions, CDisplayAttributes(calData.GetDisplayDataStart(), calData.GetDisplayDataEnd()), 
								calData.GetVertScaleType(), Points, eDateUnit);
	// 2011.01.27 by SYS <<

}

CDisplayData COnePointElement::GetDisplayData(CExtractingDataOfModifyType* pExtractingData, const int nIndex) const
{
	if(pExtractingData == NULL)
		return CDisplayData();

	return CDisplayData(CElement::GetCoordinateType(), 
		pExtractingData->GetGraphPartRegion(nIndex), pExtractingData->GetDisplayAttributes());
}

///////////////////////////////////////////////////////////////////////////////
// class CTwoPointElement

CTwoPointElement::CTwoPointElement(const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment):
	CElement(environment),
	m_startPoint(point1),
	m_endPoint(point2)
{
}

// public =====================================================================
// ���� ���õ� element�� ��ǥ�� ��������.
void CTwoPointElement::GetPoints(CElementPointList& elementPointList)
{
	elementPointList.SetVertDataType(CElement::GetCommonEnvironment().GetDataType().GetVert());
	elementPointList.AddTail(&m_startPoint);
	elementPointList.AddTail(&m_endPoint);
}

CDisplayData CTwoPointElement::GetDisplayData(CExtractingDataOfModifyType* pExtractingData, const int nIndex) const
{
	if(pExtractingData == NULL)
		return CDisplayData();

	return CDisplayData(CElement::GetCoordinateType(), 
		pExtractingData->GetGraphPartRegion(nIndex), pExtractingData->GetDisplayAttributes());
}

// point�� element ������ �ش� �Ǵ��� üũ
CToolEnumData::ModifyType CTwoPointElement::GetModifyType(CExtractingDataOfModifyType* pExtractingData) const
{
	// ���� mouse point�� �ش� rowSet ������ �ִ����� ���Ѵ�.
	if(!pExtractingData->IsPointInRowSetRegion())
		return CToolEnumData::MODIFY_NONE;

	CPoint startPoint, endPoint;
	GetAbsolutePoints(pExtractingData->GetCoordinateCalculatorData(), startPoint, endPoint);

	//CycleLine�� 
	if(GetElementType() == CToolEnumData::Cycle_Line)
	{
		for(int nIndex = 0; nIndex < pExtractingData->GetDrawingRegionCount(); nIndex++){
			CToolEnumData::ModifyType modifyType = GetModifyType(GetDisplayData(pExtractingData, nIndex), 
				startPoint, endPoint, pExtractingData->GetMousePoint());
			
			if(modifyType != CToolEnumData::MODIFY_NONE)
				return modifyType;
		}
		
		return CToolEnumData::MODIFY_NONE;
	}
	else
	{
		if(!ElementInRegion(pExtractingData->GetFullGraphRegionInMousePoint(), startPoint, endPoint, pExtractingData->GetMousePoint()))
			return CToolEnumData::MODIFY_NONE;

		return GetModifyType(pExtractingData->GetFullGraphRegionInMousePoint(), startPoint, endPoint, pExtractingData->GetMousePoint());
	}	
}

// �ش� modifyType�� �°� ����
void CTwoPointElement::ElementModify(const CElementModifyData& elementModifyData)
{
	CCoordinate startPoint(m_startPoint);
	CCoordinate endPoint(m_endPoint);		
	if(elementModifyData.GetModifyType() == CToolEnumData::MODIFY_CENTER){
		startPoint = elementModifyData.GetModifyPoint(m_startPoint);
		endPoint = elementModifyData.GetModifyPoint(m_endPoint);
		
		// (2008/9/8 - Seung-Won, Bae) Move to bound if moved out.
		if(startPoint.GetX() == 0.0 || startPoint.GetIndex() < 0 ||
			endPoint.GetX() == 0.0 || endPoint.GetIndex() < 0)
		{
			startPoint = elementModifyData.GetModifyPoint(m_startPoint);
			endPoint = elementModifyData.GetModifyPoint(m_endPoint);
			if(startPoint.GetX() == 0.0 || startPoint.GetIndex() < 0 ||
				endPoint.GetX() == 0.0 || endPoint.GetIndex() < 0)
					return;
		}

		m_startPoint = startPoint;
		m_endPoint = endPoint;
	}
	else if(elementModifyData.GetModifyType() == CToolEnumData::MODIFY_START)
		m_startPoint = elementModifyData.GetPoint();
	else if(elementModifyData.GetModifyType() == CToolEnumData::MODIFY_END)
		m_endPoint = elementModifyData.GetPoint();
	else
		assert(false);
}

void CTwoPointElement::ElementModify_YValue(const CElementModifyData& elementModifyData)
{
	if(elementModifyData.GetModifyType() == CToolEnumData::MODIFY_CENTER){
		m_startPoint = elementModifyData.GetModifyPoint(m_startPoint);
		m_endPoint = elementModifyData.GetModifyPoint(m_endPoint);
	}
	else if(elementModifyData.GetModifyType() == CToolEnumData::MODIFY_START)
		m_startPoint.SetY(elementModifyData.GetPoint().GetY());
	else if(elementModifyData.GetModifyType() == CToolEnumData::MODIFY_END)
		m_endPoint.SetY(elementModifyData.GetPoint().GetY());
	else
		assert(false);
}

void CTwoPointElement::ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType)
{
	if(eModifyType == CToolEnumData::MODIFY_START)
	{
		m_startPoint = coordValue;
	}
	else if(eModifyType == CToolEnumData::MODIFY_END)
	{
		m_endPoint = coordValue;
	}
}

CCoordinate CTwoPointElement::GetCoordinate(const CToolEnumData::ModifyType eModifyType) const
{
	CCoordinate coordValue;
	if(eModifyType == CToolEnumData::MODIFY_START)
		return m_startPoint;
	else if(eModifyType == CToolEnumData::MODIFY_END)
		return m_endPoint;

	return coordValue;
}
// ----------------------------------------------------------------------------
void CTwoPointElement::DrawShadowAndGrip(CDC* pDC, const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData)
{
	CPen newPen;
	CElement::CreatePen(false, newPen);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	CPoint startPoint, endPoint;
	GetAbsolutePoints(calData, startPoint, endPoint);

	GetTwoPointDrawer().DrawShadowAndGrip(pDC, drawingRegions.GetCurrentFullGraphRegion(), startPoint, endPoint);

	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

void CTwoPointElement::DrawSelected(CDC* pDC, const CSelectedElementDrawingData& selectedDrawingData)
{
	if(selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_CENTER || 
		selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_START || 
		selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_END)
	{
		const CCoordinateCalculatorData &calData = selectedDrawingData.GetCoordinateCalculatorData();
		CTwoPointDrawingData &drawingData = GetTwoPointDrawingData( selectedDrawingData);

		// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
		if( CDlgAnalysisToolSet::G_PRICE_RANGE_DIV_CHECK == GetCommonEnvironment().GetPriceRangeDiv())
		{
			CCoordinateCalculator& coordinateCalculator = CCoordinateCalculatorFinder::FindCoordinateCalculator(false);
			double dMax = 0.0;
			double dMin = DBL_MAX;
			CPoints ptsStart = drawingData.GetStartPoints();
			CPoints ptsEnd = drawingData.GetEndPoints();
			CCoordinate cdStart = coordinateCalculator.AbsoluteToRelative( calData, ptsStart.GetDrawingPoint());
			CCoordinate cdEnd = coordinateCalculator.AbsoluteToRelative( calData, ptsEnd.GetDrawingPoint());
			if( UpdatePointsForPriceRangeOption( calData, cdStart, cdEnd, dMax, dMin))
			{
				cdStart.SetY( dMax);
				cdEnd.SetY( dMin);
				CPoint startPoint = coordinateCalculator.RelativeToAbsolute_NoAutoPrice(calData, cdStart);
				CPoint endPoint = coordinateCalculator.RelativeToAbsolute_NoAutoPrice(calData, cdEnd);
				drawingData.SetStartYPoints_Pos( startPoint.y);
				drawingData.SetEndYPoints_Pos( endPoint.y);
			}
		}

		GetTwoPointDrawer().DrawTool(pDC, drawingData, R2_NOT);
	}
	else
		assert(false);
}

// protected ==================================================================
// list�� ����� �������� �����´�.
CCoordinate& CTwoPointElement::GetStartPoint()
{
	return m_startPoint;
}

CCoordinate CTwoPointElement::GetStartPoint() const
{
	return m_startPoint;
}

// list�� ����� ������ �����´�.
CCoordinate& CTwoPointElement::GetEndPoint()
{
	return m_endPoint;
}

CCoordinate CTwoPointElement::GetEndPoint() const
{
	return m_endPoint;
}

// list�� ����� �������� �����Ѵ�.
void CTwoPointElement::SetStartPoint(const CCoordinate& point)
{
	m_startPoint = point;
}

// list�� ����� ������ �����Ѵ�.
void CTwoPointElement::SetEndPoint(const CCoordinate& point)
{
	m_endPoint = point;
}

// ----------------------------------------------------------------------------
CString CTwoPointElement::GetSavedPoints() const
{
	return CElement::GetSavedPoint(m_startPoint) + CElement::GetSavedPoint(m_endPoint);
}

void CTwoPointElement::RealDraw(CDC* pDC,const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData)
{
	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	if( CDlgAnalysisToolSet::G_PRICE_RANGE_DIV_CHECK == GetCommonEnvironment().GetPriceRangeDiv())
	{
		double dMax = 0.0;
		double dMin = DBL_MAX;
		if( UpdatePointsForPriceRangeOption( calData, m_startPoint, m_endPoint, dMax, dMin))
		{
			m_startPoint.SetY( dMax);
			m_endPoint.SetY( dMin);
		}
	}

	GetTwoPointDrawer().DrawElement(pDC,GetTwoPointDrawingData(drawingRegions.GetCurrentGraphPartRegion(),calData));
}

// ----------------------------------------------------------------------------
// �ش� ������ Element�� �ִ����� ����
//bool CTwoPointElement::ElementInRegion(const CRect& /*region*/, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const
bool CTwoPointElement::ElementInRegion(const CRect& region, const CPoint& startPoint, const CPoint& endPoint, const CPoint& point) const
{
	CRect drawingRegion(startPoint, endPoint);
	BOOL bLeftExt = GetCommonEnvironment().IsLeftLineExt();
	BOOL bRightExt = GetCommonEnvironment().IsRightLineExt();
	if(bLeftExt == TRUE)
		drawingRegion.left = region.left;

	if(bRightExt == TRUE)
		drawingRegion.right = region.right;
	drawingRegion.NormalizeRect();
	drawingRegion.InflateRect(5, 5);
	return ( drawingRegion.PtInRect(point) ? true : false);
}

// ----------------------------------------------------------------------------
// point�� element ������ �ش� �Ǵ��� üũ
CToolEnumData::ModifyType CTwoPointElement::GetModifyType(const CRect& /*region*/, const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const
{
	CToolEnumData::ModifyType sidePointModifyType = GetModifyType_SidePoints(drawingStartPt, drawingEndPt, point);
	if(sidePointModifyType != CToolEnumData::MODIFY_NONE)
		return sidePointModifyType;

	return GetModifyType_Line(drawingStartPt, drawingEndPt, point);
}

CToolEnumData::ModifyType CTwoPointElement::GetModifyType(const CDisplayData& displayData,const CPoint& drawingStartPt, 
														  const CPoint& drawingEndPt, const CPoint& point) const
{
	CToolEnumData::ModifyType sidePointModifyType = GetModifyType_SidePoints(drawingStartPt, drawingEndPt, point);
	if(sidePointModifyType != CToolEnumData::MODIFY_NONE)
		return sidePointModifyType;

	return GetModifyType_Line(drawingStartPt, drawingEndPt, point);
}

// ----------------------------------------------------------------------------
// �糡 point�� ���� modifyType�� ��������.
CToolEnumData::ModifyType CTwoPointElement::GetModifyType_SidePoints(const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const
{
	CSelectableRegionCalculator selectableRegionCalculator;
	if(selectableRegionCalculator.IsPointInCircle(drawingStartPt, point))
		return CToolEnumData::MODIFY_START;

	if(selectableRegionCalculator.IsPointInCircle(drawingEndPt, point))
		return CToolEnumData::MODIFY_END;

	return CToolEnumData::MODIFY_NONE;
}

// �������� �̷�� ���� ���� modifyType ��������
CToolEnumData::ModifyType CTwoPointElement::GetModifyType_Line(const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const
{
	if(CElement::PointInElementSelectableRegion(drawingStartPt, drawingEndPt, point))
		return CToolEnumData::MODIFY_CENTER;
	
	return CToolEnumData::MODIFY_NONE;
}

// �������� �̷�� ���簢���� ���� modifyType ��������
CToolEnumData::ModifyType CTwoPointElement::GetModifyType_Rect(const CPoint& drawingStartPt, const CPoint& drawingEndPt, const CPoint& point) const
{
	if(CElement::PointInElementSelectableFrameRect(drawingStartPt, drawingEndPt, point))
		return CToolEnumData::MODIFY_CENTER;
	
	return CToolEnumData::MODIFY_NONE;	
}

// private ====================================================================
// �׸� ������ǥ ��������
void CTwoPointElement::GetAbsolutePoints(const CCoordinateCalculatorData& calData, CPoint& startPoint, CPoint& endPoint) const
{
	if(m_endPoint.GetIndex() > calData.GetDisplayDataEnd())
	{
		startPoint = CElement::GetAbsolutePoint(calData, m_startPoint);
		endPoint = CElement::GetAbsolutePoint(calData, m_endPoint);
		int nWidth = (int)max(1,(calData.GetRegion().Width()/calData.GetDisplayDataCount()));
		if( GetElementType() == CToolEnumData::Candle_Bong && 
			abs(startPoint.x - endPoint.x) > (nWidth-1))
		{
			if(startPoint.x < endPoint.x)
				endPoint.x = startPoint.x + nWidth;
			else
				endPoint.x = startPoint.x - nWidth;
		}
	}
	else
	{
		// ����Ʈ �����ð� ���� - ojtaso (20080811)
		/*
		startPoint = CElement::GetAbsolutePoint(calData, m_startPoint);
		endPoint = CElement::GetAbsolutePoint(calData, m_endPoint);
		*/
		IPacket *pIPacket = calData.GetPacket_X();
		if( pIPacket)
		{
			double dStartData = 0.0;
			pIPacket->GetData( 0, dStartData);
			int nDateUnit = pIPacket->GetDateUnitType();
			ILPCSTR szPacketType = pIPacket->GetType();
			CString strType( szPacketType);

			if(nDateUnit == 4 || nDateUnit == 5)
			{
				// ���̳� ƽ�� ���...
				CCoordinate startPoints(m_startPoint);
				CCoordinate endPoints(m_endPoint);

				CString strStartType = m_startPoint.GetDateType();
				if(strStartType != strType)
				{
					// 2011.01.20 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
					if(strType == "DDHHMMSS")
					{
						// ƽ
						__int64 lTimeData = (__int64)startPoints.GetX();
// vntsorl_20110406:[A00000319] ��Ʈ �м���������� ��������
// 						__int64 lTemp = lTimeData % 1000000;
// 						lTemp *= 100;
						__int64 lTemp = lTimeData % 100000000;
						
						startPoints.SetX(lTemp, startPoints.GetIndex());
					}
					else if(strType == "YYYYMMDDHHMMSS")
					{
						// �� (14Byte����)
						__int64 lTimeData = (__int64)startPoints.GetX();
						__int64 lTemp = 0;
						long lMonth = 0;
// 
 						if (strStartType == "YYYYMMDD")
 						{
 							// ��Data
 							lTemp = lTimeData;// * 1000000;
 						}
 						else
						{
							lTemp = lTimeData / 100;
							lMonth = dStartData / 1000000;
							lMonth *= 1000000;
							lTemp += lMonth;
						}
						
						startPoints.SetX(lTemp, startPoints.GetIndex());
					}
					else
					{
						// �� (����)
 						__int64 lTimeData = (__int64)startPoints.GetX();
// vntsorl_20110406:[A00000319] ��Ʈ �м���������� ��������
// 						__int64 lTemp = lTimeData / 100;
// 						long lMonth = dStartData / 1000000;
// 						lMonth *= 1000000;
// 						lTemp += lMonth;
						__int64 lTemp = lTimeData % 10000000000;
						lTemp = lTemp / 100;
						
						startPoints.SetX(lTemp, startPoints.GetIndex());
					}
					// 2011.01.20 by SYS <<
				}

				CString strEndType = m_endPoint.GetDateType();
				if(strEndType != strType)
				{
					if(strType == "DDHHMMSS")
					{
						// ƽ
						__int64 lTimeData = (__int64)endPoints.GetX();
// vntsorl_20110406:[A00000319] ��Ʈ �м���������� ��������
// 						__int64 lTemp = lTimeData % 1000000;
// 						lTemp *= 100;
						__int64 lTemp = lTimeData % 100000000;
						
						endPoints.SetX(lTemp, endPoints.GetIndex());
					}
					// 2011.01.20 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
					else if(strType == "YYYYMMDDHHMMSS")
					{
						// �� (14Byte����)
						__int64 lTimeData = (__int64)endPoints.GetX();
						__int64 lTemp = 0;
						long lMonth = 0;
						
 						if (strStartType == "YYYYMMDD")
 						{
 							// ��Data
 							lTemp = lTimeData;// * 1000000;
 						}
 						else
						{
							lTemp = lTimeData / 100;
							lMonth = dStartData / 1000000;
							lMonth *= 1000000;
							lTemp += lMonth;
						}
						
						endPoints.SetX(lTemp, endPoints.GetIndex());
					}
					// 2011.01.20 by SYS <<
					else
					{
						// ��
 						__int64 lTimeData = (__int64)endPoints.GetX();
// vntsorl_20110406:[A00000319] ��Ʈ �м���������� ��������
// 						__int64 lTemp = lTimeData / 100;
// 						long lMonth = dStartData / 1000000;
// 						lMonth *= 1000000;
// 						lTemp += lMonth;
						__int64 lTemp = lTimeData % 10000000000;
						lTemp = lTemp / 100;
						
						endPoints.SetX(lTemp, endPoints.GetIndex());
					}
				}
				// 20081007 JS.Kim	����ڽð��� ����. XScale�� RQ���� �ٸ��� ������ ���ϸ� �ȵȴ�.<<
				/*
				if(dStartData > startPoints.GetX() || dStartData > endPoints.GetX())
				{
					startPoint = CPoint(-1, -1);
					endPoint = CPoint(-1, -1);

					return;
				}
				*/
				//>>

				startPoint = CElement::GetAbsolutePoint(calData, startPoints);
				endPoint = CElement::GetAbsolutePoint(calData, endPoints);

				return;
			}

// (2009/10/14 - Seung-Won, Bae) Support the range out of data.
//			if(dStartData > m_startPoint.GetX() || dStartData > m_endPoint.GetX())
//			{
//				startPoint = CPoint(-1, -1);
//				endPoint = CPoint(-1, -1);
//
//				return;
//			}
			pIPacket->Release();
		}

		startPoint = CElement::GetAbsolutePoint(calData, m_startPoint);
		endPoint = CElement::GetAbsolutePoint(calData, m_endPoint);
	}	
}

/*
void CTwoPointElement::GetAbsolutePoints(const CCoordinateCalculatorData& calData, CPoint& startPoint, CPoint& endPoint) const
{
	if(m_endPoint.GetIndex() > calData.GetDisplayDataEnd())
	{
		startPoint = CElement::GetAbsolutePoint(calData, m_startPoint);
		endPoint = CElement::GetAbsolutePoint(calData, m_endPoint);
		int nWidth = (int)max(1,(calData.GetRegion().Width()/calData.GetDisplayDataCount()));
		if( GetElementType() == CToolEnumData::Candle_Bong && 
			abs(startPoint.x - endPoint.x) > (nWidth-1))
		{
			if(startPoint.x < endPoint.x)
				endPoint.x = startPoint.x + nWidth;
			else
				endPoint.x = startPoint.x - nWidth;
		}
	}
	else
	{
		// ����Ʈ �����ð� ���� - ojtaso (20080811)
		IPacket *pIPacket = calData.GetPacket_X();
		if( pIPacket)
		{
			double dStartData = 0.0;
			pIPacket->GetData( 0, dStartData);
			int nDateUnit = pIPacket->GetDateUnitType();
			ILPCSTR szPacketType = pIPacket->GetType();
			CString strType( szPacketType);
			if(nDateUnit == 4 || nDateUnit == 5)
			{
				CCoordinate startPoints(m_startPoint);
				CCoordinate endPoints(m_endPoint);

				if(m_startPoint.GetDateType() != strType)
				{
					if(strType == "DDHHMMSS")
					{
						// ƽ
						long lTimeData = (long)startPoints.GetX();
						long lTemp = lTimeData % 1000000;
						lTemp *= 100;
						
						startPoints.SetX(lTemp, startPoints.GetIndex());
					}
					// 2011.01.20 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
					if(strType == "YYYYMMDDHHMMSS")
					{
						// �� (14Byte����)
						long lTimeData = (long)startPoints.GetX();
						long lTemp = lTimeData / 100;
						long lMonth = dStartData / 1000000;
						lMonth *= 1000000;
						lTemp += lMonth;
						
						startPoints.SetX(lTemp, startPoints.GetIndex());
					}
					// 2011.01.20 by SYS <<
					else
					{
						// �� (����)
						long lTimeData = (long)startPoints.GetX();
						long lTemp = lTimeData / 100;
						long lMonth = dStartData / 1000000;
						lMonth *= 1000000;
						lTemp += lMonth;
						
						startPoints.SetX(lTemp, startPoints.GetIndex());
					}
				}

				if(m_endPoint.GetDateType() != strType)
				{
					if(strType == "DDHHMMSS")
					{
						// ƽ
						long lTimeData = (long)endPoints.GetX();
						long lTemp = lTimeData % 1000000;
						lTemp *= 100;
						
						endPoints.SetX(lTemp, endPoints.GetIndex());
					}
					else
					{
						// ��
						long lTimeData = (long)endPoints.GetX();
						long lTemp = lTimeData / 100;
						long lMonth = dStartData / 1000000;
						lMonth *= 1000000;
						lTemp += lMonth;
						
						endPoints.SetX(lTemp, endPoints.GetIndex());
					}
				}
				//>>

				startPoint = CElement::GetAbsolutePoint(calData, startPoints);
				endPoint = CElement::GetAbsolutePoint(calData, endPoints);

				return;
			}

// (2009/10/14 - Seung-Won, Bae) Support the range out of data.
//			if(dStartData > m_startPoint.GetX() || dStartData > m_endPoint.GetX())
//			{
//				startPoint = CPoint(-1, -1);
//				endPoint = CPoint(-1, -1);
//
//				return;
//			}
			pIPacket->Release();
		}

		startPoint = CElement::GetAbsolutePoint(calData, m_startPoint);
		endPoint = CElement::GetAbsolutePoint(calData, m_endPoint);
	}	
}
*/

// ----------------------------------------------------------------------------
CTwoPointDrawingData CTwoPointElement::GetTwoPointDrawingData(const CGraphPartRegion& graphPartRegion, const CCoordinateCalculatorData& calData) const
{
	CPoint startPoint, endPoint;
	GetAbsolutePoints(calData, startPoint, endPoint);

	// 2011.01.27 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
// 	return CTwoPointDrawingData(
// 				CElement::GetCommonEnvironment().GetDataType(),
// 				CElement::GetCommonEnvironment().GetEquiDiv(), 
// 				CElement::GetCommonEnvironment().GetPriceRangeDiv(),	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
// 				const_cast<CTextPosition*> (GetTextPosition()),
// 				CElement::GetCommonEnvironment().GetLineExt(),
// 				graphPartRegion,
// 				CDisplayAttributes(calData.GetDisplayDataStart(), calData.GetDisplayDataEnd()), 
// 				calData.GetVertScaleType(),
// 				CPoints(m_startPoint, startPoint),
// 				CPoints(m_endPoint, endPoint));

	ILPCSTR szHelpMsg = NULL;
	CString strDataType;	
	//CCoordinateCalculatorData calData = GetCoordinateCalculatorData( CMoveTool::GetPoints().GetDrawingPoint());
	ILPCSTR szRQ = calData.m_pIChartOCX->GetCurrentRQ();
	IPacketManager * pCurPacketManager = calData.m_pIPacketListManager->GetPacketManager(szRQ);
	pCurPacketManager->GetHelpMessageItem("UD", szHelpMsg); strDataType = szHelpMsg;
	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)atoi(strDataType);
	//

	return CTwoPointDrawingData(
				CElement::GetCommonEnvironment().GetDataType(),
				CElement::GetCommonEnvironment().GetEquiDiv(), 
				CElement::GetCommonEnvironment().GetPriceRangeDiv(),	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
				const_cast<CTextPosition*> (GetTextPosition()),
				CElement::GetCommonEnvironment().GetLineExt(),
				graphPartRegion,
				CDisplayAttributes(calData.GetDisplayDataStart(), calData.GetDisplayDataEnd()), 
				calData.GetVertScaleType(),
				CPoints(m_startPoint, startPoint),
				CPoints(m_endPoint, endPoint), eDateUnit);

	// 2011.01.27 by SYS <<
}

CTwoPointDrawingData CTwoPointElement::GetTwoPointDrawingData(const CSelectedElementDrawingData& selectedDrawingData) const
{
	// 2011.01.27 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
// 	CTwoPointDrawingData drawingData(
// 							CElement::GetCommonEnvironment().GetDataType(),
// 							CElement::GetCommonEnvironment().GetEquiDiv(),  
// 							CElement::GetCommonEnvironment().GetPriceRangeDiv(),  // (2008/8/15 - Seung-Won, Bae) Add option for using price range.
// 							const_cast<CTextPosition*> (GetTextPosition()), 
// 							CElement::GetCommonEnvironment().GetLineExt(),
// 							selectedDrawingData.GetCurrentGraphPartRegion(), 
// 							selectedDrawingData.GetRegion().GetDisplayAttributes(), 
// 							selectedDrawingData.GetVertScaleType());

	ILPCSTR szHelpMsg = NULL;
	CString strDataType;	
	//CCoordinateCalculatorData calData = GetCoordinateCalculatorData( CMoveTool::GetPoints().GetDrawingPoint());
	const CCoordinateCalculatorData calData = selectedDrawingData.GetCoordinateCalculatorData();
	ILPCSTR szRQ = calData.m_pIChartOCX->GetCurrentRQ();
	IPacketManager * pCurPacketManager = calData.m_pIPacketListManager->GetPacketManager(szRQ);
	pCurPacketManager->GetHelpMessageItem("UD", szHelpMsg); strDataType = szHelpMsg;
	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)atoi(strDataType);
	//

	CTwoPointDrawingData drawingData(
							CElement::GetCommonEnvironment().GetDataType(),
							CElement::GetCommonEnvironment().GetEquiDiv(),  
							CElement::GetCommonEnvironment().GetPriceRangeDiv(),  // (2008/8/15 - Seung-Won, Bae) Add option for using price range.
							const_cast<CTextPosition*> (GetTextPosition()), 
							CElement::GetCommonEnvironment().GetLineExt(),
							selectedDrawingData.GetCurrentGraphPartRegion(), 
							selectedDrawingData.GetRegion().GetDisplayAttributes(), 
							selectedDrawingData.GetVertScaleType(), eDateUnit);

	// 2011.01.27 by SYS <<


	if(selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_CENTER){
		drawingData.SetStartPoints(CElement::GetSelectedDrawPoints(selectedDrawingData, m_startPoint));
		drawingData.SetEndPoints(CElement::GetSelectedDrawPoints(selectedDrawingData, m_endPoint));
	}
	else if(selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_START)
	{
		if(GetElementType() == CToolEnumData::Candle_Bong)
		{
			CPoints ptStarts = CElement::GetSelectedDrawPoints(selectedDrawingData, m_startPoint);
			drawingData.SetStartYPoints_Val(ptStarts.GetSavingPoint().GetY());
			drawingData.SetStartYPoints_Pos(ptStarts.GetDrawingPoint().y);
		}
		else
		{
			drawingData.SetStartPoints(CElement::GetSelectedDrawPoints(selectedDrawingData, m_startPoint));
			drawingData.SetEndPoints(CElement::GetSelectedDrawPoints(selectedDrawingData.GetCoordinateCalculatorData(), m_endPoint));
		}
	}
	else if(selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_END)
	{
		if(GetElementType() == CToolEnumData::Candle_Bong)
		{
			CPoints ptEnds = CElement::GetSelectedDrawPoints(selectedDrawingData, m_endPoint);
			drawingData.SetEndYPoints_Val(ptEnds.GetSavingPoint().GetY());
			drawingData.SetEndYPoints_Pos(ptEnds.GetDrawingPoint().y);
		}
		else
		{
			drawingData.SetStartPoints(CElement::GetSelectedDrawPoints(selectedDrawingData.GetCoordinateCalculatorData(), m_startPoint));
			drawingData.SetEndPoints(CElement::GetSelectedDrawPoints(selectedDrawingData, m_endPoint));
		}		
	}
	else
	{
		assert(false);
	}

	return drawingData;
}

// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
BOOL CTwoPointElement::UpdatePointsForPriceRangeOption( const CCoordinateCalculatorData& calData, CCoordinate &p_startPoint, CCoordinate &p_endPoint, double &dMax, double &dMin)
{
	BOOL bResult = FALSE;
	if( !calData.IsValidAddInInterface()) return bResult;

	IPacket *pIPacketH = calData.m_pIPacketListManager->GetPacket( "DEFAULT", _MTEXT( C0_HIGH));
	if( !pIPacketH) return bResult;

	IPacket *pIPacketL = calData.m_pIPacketListManager->GetPacket( "DEFAULT", _MTEXT( C0_LOW));
	if( pIPacketL)
	{
		IDoubleList *pilHigh = pIPacketH->GetDataList();
		IDoubleList *pilLow = pIPacketL->GetDataList();
		if( pilHigh && pilLow)
		{
			int nXStart = 0;
			calData.m_pIChartManager->GetXFromDouble( calData.GetBlockIndex().GetRow(), calData.GetBlockIndex().GetColumn(), p_startPoint.GetX(), nXStart);
			int nDTIStart = calData.m_pIChartManager->FindViewDataIndex_Pt( "DEFAULT", nXStart, calData.GetBlockIndex().GetColumn(), FALSE);
			int nXEnd = 0;
			calData.m_pIChartManager->GetXFromDouble( calData.GetBlockIndex().GetRow(), calData.GetBlockIndex().GetColumn(), p_endPoint.GetX(), nXEnd);
			int nDTIEnd = calData.m_pIChartManager->FindViewDataIndex_Pt( "DEFAULT", nXEnd, calData.GetBlockIndex().GetColumn(), FALSE);
			if( 0 <= nDTIStart && 0 <= nDTIEnd)
			{
				int i = 0;
				if( nDTIEnd < nDTIStart)
				{
					i = nDTIEnd;
					nDTIEnd = nDTIStart;
					nDTIStart = i;
				}
				double dHighPrice = 0.0;
				double dLowPrice = DBL_MAX;
				POSITION psnH = pilHigh->FindIndex( nDTIStart);
				POSITION psnL = pilLow->FindIndex( nDTIStart);
				for( i = nDTIStart; i <= nDTIEnd && psnH && psnL; i++)
				{
					pilHigh->GetNext( psnH, dHighPrice);
					pilLow->GetNext( psnL, dLowPrice);
					if( dMax < dHighPrice) dMax = dHighPrice;
					if( dLowPrice < dMin) dMin = dLowPrice;
				}
				bResult = TRUE;
			}
		}
		if( pilHigh) pilHigh->Release();
		if( pilLow)	pilLow->Release();
		pIPacketL->Release();
	}
	pIPacketH->Release();
	return bResult;
}

///////////////////////////////////////////////////////////////////////////////
// class CThreePointElement

CThreePointElement::CThreePointElement(const CCoordinate& point1, const CCoordinate& point2, const CCoordinate& point3, const CElementCommonEnvironment& environment):
	CElement(environment),
	m_point1(point1),
	m_point2(point2),
	m_point3(point3)
{
}

// public =====================================================================
// ���� ���õ� element�� ��ǥ�� ��������.
void CThreePointElement::GetPoints(CElementPointList& elementPointList)
{
	elementPointList.SetVertDataType(CElement::GetCommonEnvironment().GetDataType().GetVert());
	elementPointList.AddTail(&m_point1);
	elementPointList.AddTail(&m_point2);
	elementPointList.AddTail(&m_point3);
}

// point�� element ������ �ش� �Ǵ��� üũ
CToolEnumData::ModifyType CThreePointElement::GetModifyType(CExtractingDataOfModifyType* pExtractingData) const
{
	// ���� mouse point�� �ش� rowSet ������ �ִ����� ���Ѵ�.
	if(!pExtractingData->IsPointInRowSetRegion())
		return CToolEnumData::MODIFY_NONE;

	CPoint pt1, pt2, pt3;
	GetAbsolutePoints(pExtractingData->GetCoordinateCalculatorData(), pt1, pt2, pt3);

	if(!ElementInRegion(pExtractingData->GetFullGraphRegionInMousePoint(), pt1, pt2, pt3, pExtractingData->GetMousePoint()))
		return CToolEnumData::MODIFY_NONE;

	return GetModifyType(pExtractingData->GetFullGraphRegionInMousePoint(), pt1, pt2, pt3, pExtractingData->GetMousePoint());
}

// �ش� modifyType�� �°� ����
void CThreePointElement::ElementModify(const CElementModifyData& elementModifyData)
{
	if(elementModifyData.GetModifyType() == CToolEnumData::MODIFY_CENTER)
	{
		// (2008/8/12 - Seung-Won, Bae) Do not move to outside as future.
		CCoordinate point1, point2, point3;
		point1 = elementModifyData.GetModifyPoint( m_point1);
		point2 = elementModifyData.GetModifyPoint( m_point2);
		point3 = elementModifyData.GetModifyPoint( m_point3);
		
		if( point1.GetX() == 0.0 || point1.GetIndex() < 0
			|| point2.GetX() == 0.0 || point2.GetIndex() < 0
			|| point3.GetX() == 0.0 || point3.GetIndex() < 0)
			return;

		m_point1 = point1;
		m_point2 = point2;
		m_point3 = point3;
	}
	else if(elementModifyData.GetModifyType() == CToolEnumData::MODIFY_START)
		m_point1 = elementModifyData.GetPoint();
	else if(elementModifyData.GetModifyType() == CToolEnumData::MODIFY_MIDDLE)
		m_point2 = elementModifyData.GetPoint();
	else if(elementModifyData.GetModifyType() == CToolEnumData::MODIFY_END)
		m_point3 = elementModifyData.GetPoint();
	else
		assert(false);
}

void CThreePointElement::ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType)
{
}
void CThreePointElement::ElementModify_YValue(const CElementModifyData& elementModifyData)
{
}

CCoordinate CThreePointElement::GetCoordinate(const CToolEnumData::ModifyType eModifyType) const
{
	CCoordinate coordValue;
	if(eModifyType == CToolEnumData::MODIFY_START)
		return m_point1;
	else if(eModifyType == CToolEnumData::MODIFY_CENTER)
		return m_point2;
	else if(eModifyType == CToolEnumData::MODIFY_END)
		return m_point3;

	return coordValue;
}
// ----------------------------------------------------------------------------
void CThreePointElement::DrawShadowAndGrip(CDC* pDC, const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData)
{
	CPen newPen;
	CElement::CreatePen(false, newPen);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	CPoint pt1, pt2, pt3;
	GetAbsolutePoints(calData, pt1, pt2, pt3);
	GetThreePointDrawer().DrawShadowAndGrip(pDC, drawingRegions.GetCurrentFullGraphRegion(), pt1, pt2, pt3);

	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

void CThreePointElement::DrawSelected(CDC* pDC, const CSelectedElementDrawingData& selectedDrawingData)
{
	if(selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_CENTER)
		GetThreePointDrawer().DrawElement(pDC, GetThreePointDrawingData(selectedDrawingData), R2_NOT);
	else if(selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_START)
		GetThreePointDrawer().DrawElement(pDC, GetThreePointDrawingData(selectedDrawingData), R2_NOT);
	else if(selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_MIDDLE)
		GetThreePointDrawer().DrawElement(pDC, GetThreePointDrawingData(selectedDrawingData), R2_NOT);
	else if(selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_END)
		GetThreePointDrawer().DrawElement(pDC, GetThreePointDrawingData(selectedDrawingData), R2_NOT);
	else
		assert(false);
}

// protected ==================================================================
CString CThreePointElement::GetSavedPoints() const
{
	return CElement::GetSavedPoint(m_point1) + CElement::GetSavedPoint(m_point2) + CElement::GetSavedPoint(m_point3);
}

// ----------------------------------------------------------------------------
void CThreePointElement::RealDraw(CDC* pDC,const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData)
{
	GetThreePointDrawer().DrawElement(pDC, GetThreePointDrawingData(drawingRegions.GetCurrentFullGraphRegion(), calData));
}

// ----------------------------------------------------------------------------
CToolEnumData::ModifyType CThreePointElement::GetModifyType(const CRect& /*region*/, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& point) const
{
	return GetModifyType_SidePoints(pt1, pt2, pt3, point);
}

// private ====================================================================
// �׸� ������ǥ ��������
void CThreePointElement::GetAbsolutePoints(const CCoordinateCalculatorData& calData, CPoint& pt1, CPoint& pt2, CPoint& pt3) const
{
	pt1 = CElement::GetAbsolutePoint(calData, m_point1);
	pt2 = CElement::GetAbsolutePoint(calData, m_point2);
	pt3 = CElement::GetAbsolutePoint(calData, m_point3);
}

CThreePointDrawingData CThreePointElement::GetThreePointDrawingData(const CRect& rect, const CCoordinateCalculatorData& calData) const
{
	CPoint pt1, pt2, pt3;
	GetAbsolutePoints(calData, pt1, pt2, pt3);

	// 2011.01.27 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
// 	return CThreePointDrawingData(CElement::GetCommonEnvironment().GetDataType(), const_cast<CTextPosition*> (GetTextPosition()), 
// 		&CElement::GetCommonEnvironment().GetLineExt(),
// 		rect, CDisplayAttributes(calData.GetDisplayDataStart(), calData.GetDisplayDataEnd()), 
// 		calData.GetVertScaleType(), 3, CPoints(m_point1, pt1), CPoints(m_point2, pt2), CPoints(m_point3, pt3));

	ILPCSTR szHelpMsg = NULL;
	CString strDataType;	
	//CCoordinateCalculatorData calData = GetCoordinateCalculatorData( CMoveTool::GetPoints().GetDrawingPoint());
	ILPCSTR szRQ = calData.m_pIChartOCX->GetCurrentRQ();
	IPacketManager * pCurPacketManager = calData.m_pIPacketListManager->GetPacketManager(szRQ);
	pCurPacketManager->GetHelpMessageItem("UD", szHelpMsg); strDataType = szHelpMsg;
	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)atoi(strDataType);
	//

	return CThreePointDrawingData(CElement::GetCommonEnvironment().GetDataType(), const_cast<CTextPosition*> (GetTextPosition()), 
		&CElement::GetCommonEnvironment().GetLineExt(),
		rect, CDisplayAttributes(calData.GetDisplayDataStart(), calData.GetDisplayDataEnd()), 
		calData.GetVertScaleType(), 3, CPoints(m_point1, pt1), CPoints(m_point2, pt2), CPoints(m_point3, pt3), eDateUnit);

	// 2011.01.27 by SYS <<
}

CThreePointDrawingData CThreePointElement::GetThreePointDrawingData(const CSelectedElementDrawingData& selectedDrawingData) const
{
	// 2011.01.27 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)

// 	CThreePointDrawingData drawingData(CElement::GetCommonEnvironment().GetDataType(), const_cast<CTextPosition*> (GetTextPosition()), 
// 		&CElement::GetCommonEnvironment().GetLineExt(),
// 		selectedDrawingData.GetRegion().GetRect(), selectedDrawingData.GetRegion().GetDisplayAttributes(), 
// 		selectedDrawingData.GetVertScaleType());

	ILPCSTR szHelpMsg = NULL;
	CString strDataType;	
	const CCoordinateCalculatorData calData = selectedDrawingData.GetCoordinateCalculatorData();
	ILPCSTR szRQ = calData.m_pIChartOCX->GetCurrentRQ();
	IPacketManager * pCurPacketManager = calData.m_pIPacketListManager->GetPacketManager(szRQ);
	pCurPacketManager->GetHelpMessageItem("UD", szHelpMsg); strDataType = szHelpMsg;
	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)atoi(strDataType);
	//

	CThreePointDrawingData drawingData(CElement::GetCommonEnvironment().GetDataType(), const_cast<CTextPosition*> (GetTextPosition()), 
		&CElement::GetCommonEnvironment().GetLineExt(),
		selectedDrawingData.GetRegion().GetRect(), selectedDrawingData.GetRegion().GetDisplayAttributes(), 
		selectedDrawingData.GetVertScaleType(), eDateUnit);

	// 2011.01.27 by SYS <<

	if(selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_CENTER){
		drawingData.SetPoints1(CElement::GetSelectedDrawPoints(selectedDrawingData, m_point1));
		drawingData.SetPoints2(CElement::GetSelectedDrawPoints(selectedDrawingData, m_point2));
		drawingData.SetPoints3(CElement::GetSelectedDrawPoints(selectedDrawingData, m_point3));
	}
	else if(selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_START){
		drawingData.SetPoints1(CElement::GetSelectedDrawPoints(selectedDrawingData, m_point1));
		drawingData.SetPoints2(CElement::GetSelectedDrawPoints(selectedDrawingData.GetCoordinateCalculatorData(), m_point2));
		drawingData.SetPoints3(CElement::GetSelectedDrawPoints(selectedDrawingData.GetCoordinateCalculatorData(), m_point3));
	}
	else if(selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_MIDDLE){
		drawingData.SetPoints1(CElement::GetSelectedDrawPoints(selectedDrawingData.GetCoordinateCalculatorData(), m_point1));
		drawingData.SetPoints2(CElement::GetSelectedDrawPoints(selectedDrawingData, m_point2));
		drawingData.SetPoints3(CElement::GetSelectedDrawPoints(selectedDrawingData.GetCoordinateCalculatorData(), m_point3));
	}
	else if(selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_END){
		drawingData.SetPoints1(CElement::GetSelectedDrawPoints(selectedDrawingData.GetCoordinateCalculatorData(), m_point1));
		drawingData.SetPoints2(CElement::GetSelectedDrawPoints(selectedDrawingData.GetCoordinateCalculatorData(), m_point2));
		drawingData.SetPoints3(CElement::GetSelectedDrawPoints(selectedDrawingData, m_point3));
	}
	else
		assert(false);

	drawingData.SetPointsCount(3);
	return drawingData;
}

// ----------------------------------------------------------------------------
// ���鿡 ���� ����
CToolEnumData::ModifyType CThreePointElement::GetModifyType_SidePoints(const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& point) const
{
	CSelectableRegionCalculator selectableRegionCalculator;
	if(selectableRegionCalculator.IsPointInCircle(pt1, point))
		return CToolEnumData::MODIFY_START;

	if(selectableRegionCalculator.IsPointInCircle(pt2, point))
		return CToolEnumData::MODIFY_MIDDLE;

	if(selectableRegionCalculator.IsPointInCircle(pt3, point))
		return CToolEnumData::MODIFY_END;

	return CToolEnumData::MODIFY_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// class CMultiPointElement

CMultiPointElement::CMultiPointElement(const int& nPointsCount,const CPoints* pPointsList,const CElementCommonEnvironment& environment):
	CElement(environment)
{
	m_nPointsCount = nPointsCount;
	for(int i=0;i<nPointsCount;i++)
	{
		m_ptPointsList[i].SetPoints(pPointsList[i].GetSavingPoint(),pPointsList[i].GetDrawingPoint());
	}
}
	
CMultiPointElement::CMultiPointElement(const CDequeList<CCoordinate>& pointList,const CElementCommonEnvironment& environment):
	CElement(environment)
{	
	m_nPointsCount = pointList.GetCount();
	for(int i=0;i<m_nPointsCount;i++)
	{
		m_ptPointsList[i].SetPoints(pointList.GetAt(i),CPoint(-1,-1));
	}
}

// public =====================================================================
// ���� ���õ� element�� ��ǥ�� ��������.
CToolEnumData::ModifyType CMultiPointElement::GetModifyType(CExtractingDataOfModifyType* pExtractingData) const
{		
	CPoint startPoint;
	CPoint endPoint;
	for(int nIndex = 0; nIndex < m_nPointsCount-1;nIndex++)
	{
		GetAbsolutePoints(pExtractingData->GetCoordinateCalculatorData(), startPoint, nIndex);
		GetAbsolutePoints(pExtractingData->GetCoordinateCalculatorData(), endPoint, nIndex+1);
		
		CToolEnumData::ModifyType modifyType = GetModifyType_SidePoints(startPoint, endPoint, pExtractingData->GetMousePoint());
		if(modifyType != CToolEnumData::MODIFY_NONE)
			return modifyType;

		modifyType = GetModifyType(pExtractingData->GetFullGraphRegionInMousePoint(), startPoint, endPoint, pExtractingData->GetMousePoint());
		if(modifyType != CToolEnumData::MODIFY_NONE)
			return modifyType;
	}	

	return CToolEnumData::MODIFY_NONE;
}

// point�� element ������ �ش� �Ǵ��� üũ
CToolEnumData::ModifyType CMultiPointElement::GetModifyType(const CRect& region,const CPoint& drawingStartPt, const CPoint& drawingEndPt,const CPoint& point) const
{
	if(CElement::PointInElementSelectableRegion(drawingStartPt, drawingEndPt, point))
		return CToolEnumData::MODIFY_CENTER;
	
	return CToolEnumData::MODIFY_NONE;
}

// �ش� modifyType�� �°� ����
void CMultiPointElement::ElementModify(const CElementModifyData& elementModifyData)
{
	// (2008/8/15 - Seung-Won, Bae) Support moving of pencel draw object.
	// (2008/8/12 - Seung-Won, Bae) Do not move to outside as future.
	if(elementModifyData.GetModifyType() == CToolEnumData::MODIFY_CENTER)
	{
		CCoordinate point[ 500];
		int i = 0;
		for(  i = 0; i < m_nPointsCount; i++)
		{
			point[ i] = elementModifyData.GetModifyPoint( m_ptPointsList[ i].GetSavingPoint());
			if( point[ i].GetX() == 0.0 || point[ i].GetIndex() < 0) return;
		}
		for( i = 0; i < m_nPointsCount; i++)
			m_ptPointsList[ i].SetPoints( point[ i],
				m_ptPointsList[ i].GetDrawingPoint()
					+ CPoint( elementModifyData.GetPoint().GetX(), elementModifyData.GetPoint().GetY()));
	}
}

void CMultiPointElement::ElementModify_YValue(const CElementModifyData& elementModifyData)
{
}

void CMultiPointElement::ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType)
{
}

CCoordinate CMultiPointElement::GetCoordinate(const CToolEnumData::ModifyType eModifyType) const
{
	CCoordinate coordValue;
	return coordValue;
}

// ----------------------------------------------------------------------------
void CMultiPointElement::DrawShadowAndGrip(CDC* pDC, const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData)
{
	CPen newPen;
	CElement::CreatePen(false, newPen);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	CPoint posSelect;
	for(int nIndex = 0; nIndex < m_nPointsCount;nIndex++)
	{
		GetAbsolutePoints(calData, posSelect, nIndex);
		GetMultiPointDrawer().DrawShadowAndGrip(pDC, drawingRegions.GetCurrentFullGraphRegion(), posSelect);
		nIndex += (int) (m_nPointsCount / 10);
	}	

	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

void CMultiPointElement::DrawSelected(CDC* pDC, const CSelectedElementDrawingData& selectedDrawingData)
{
	// (2008/8/15 - Seung-Won, Bae) Support moving of pencel draw object.
	if(selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_CENTER || 
		selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_START || 
		selectedDrawingData.GetModifyType() == CToolEnumData::MODIFY_END)
		GetMultiPointDrawer().DrawTool(pDC,GetMultiPointDrawingData( selectedDrawingData) , R2_NOT);
	else
		assert(false);
}

// protected ==================================================================
CString CMultiPointElement::GetSavedPoints() const
{
	CString strSavedPoints;
	for(int i=0;i<m_nPointsCount;i++)
	{
		strSavedPoints += CElement::GetSavedPoint(m_ptPointsList[i].GetSavingPoint());
	}
	return strSavedPoints;
}

// ----------------------------------------------------------------------------
void CMultiPointElement::RealDraw(CDC* pDC,const CDrawingRegions& drawingRegions, const CCoordinateCalculatorData& calData)
{
	GetMultiPointDrawer().DrawElement(pDC, GetMultiPointDrawingData(drawingRegions.GetCurrentFullGraphRegion(), calData));
}

// private ====================================================================

void CMultiPointElement::GetPoints(CElementPointList& elementPointList)
{
	elementPointList.SetVertDataType(CElement::GetCommonEnvironment().GetDataType().GetVert());
	for(int i=0;i<m_nPointsCount;i++)
	{
		elementPointList.AddTail(&m_ptPointsList[i].GetSavingPoint());
	}
}

// �׸� ������ǥ ��������
void CMultiPointElement::GetAbsolutePoints(const CCoordinateCalculatorData& calData, CPoint& drawingPoint,int nIndex) const
{
	drawingPoint = CElement::GetAbsolutePoint(calData,m_ptPointsList[nIndex].GetSavingPoint());	
}

CMultiPointDrawingData CMultiPointElement::GetMultiPointDrawingData(const CRect& rect, const CCoordinateCalculatorData& calData) const
{	
	// (2008/8/15 - Seung-Won, Bae) Support moving of pencel draw object.
	CPoints ptPointsList[500];
	CPoint point;
	CCoordinate coordinate;
	for(int i=0;i<m_nPointsCount;i++)
	{
		point = CElement::GetAbsolutePoint(calData,m_ptPointsList[i].GetSavingPoint());
		ptPointsList[i].SetPoints(m_ptPointsList[i].GetSavingPoint(),point);
	}

	// 2011.01.27 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
// 	return CMultiPointDrawingData(CElement::GetCommonEnvironment().GetDataType(), const_cast<CTextPosition*> (GetTextPosition()), 
// 		&CElement::GetCommonEnvironment().GetLineExt(),
// 		rect, CDisplayAttributes(calData.GetDisplayDataStart(), calData.GetDisplayDataEnd()), 
// 		calData.GetVertScaleType(),m_nPointsCount,ptPointsList);

	ILPCSTR szHelpMsg = NULL;
	CString strDataType;	
	//CCoordinateCalculatorData calData = GetCoordinateCalculatorData( CMoveTool::GetPoints().GetDrawingPoint());
	ILPCSTR szRQ = calData.m_pIChartOCX->GetCurrentRQ();
	IPacketManager * pCurPacketManager = calData.m_pIPacketListManager->GetPacketManager(szRQ);
	pCurPacketManager->GetHelpMessageItem("UD", szHelpMsg); strDataType = szHelpMsg;
	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)atoi(strDataType);

	return CMultiPointDrawingData(CElement::GetCommonEnvironment().GetDataType(), const_cast<CTextPosition*> (GetTextPosition()), 
		&CElement::GetCommonEnvironment().GetLineExt(),
		rect, CDisplayAttributes(calData.GetDisplayDataStart(), calData.GetDisplayDataEnd()), 
		calData.GetVertScaleType(),m_nPointsCount,ptPointsList,eDateUnit);

	// 2011.01.27 by SYS <<
}

CMultiPointDrawingData CMultiPointElement::GetMultiPointDrawingData(const CSelectedElementDrawingData& selectedDrawingData) const
{
	// 2011.01.27 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)

// 	CMultiPointDrawingData drawingData(	CElement::GetCommonEnvironment().GetDataType(),
// 										const_cast<CTextPosition*> (GetTextPosition()), 
// 										&CElement::GetCommonEnvironment().GetLineExt(),
// 										selectedDrawingData.GetRegion().GetRect(),
// 										selectedDrawingData.GetRegion().GetDisplayAttributes(), 
// 										selectedDrawingData.GetVertScaleType());

	ILPCSTR szHelpMsg = NULL;
	CString strDataType;	
	const CCoordinateCalculatorData calData = selectedDrawingData.GetCoordinateCalculatorData();
	ILPCSTR szRQ = calData.m_pIChartOCX->GetCurrentRQ();
	IPacketManager * pCurPacketManager = calData.m_pIPacketListManager->GetPacketManager(szRQ);
	pCurPacketManager->GetHelpMessageItem("UD", szHelpMsg); strDataType = szHelpMsg;
	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)atoi(strDataType);

	CMultiPointDrawingData drawingData(	CElement::GetCommonEnvironment().GetDataType(),
										const_cast<CTextPosition*> (GetTextPosition()), 
										&CElement::GetCommonEnvironment().GetLineExt(),
										selectedDrawingData.GetRegion().GetRect(),
										selectedDrawingData.GetRegion().GetDisplayAttributes(), 
										selectedDrawingData.GetVertScaleType(), eDateUnit);
	// 2011.01.27 by SYS <<

	CPoints *ptPointList = drawingData.GetPoints();
	for( int i = 0; i < m_nPointsCount; i++)
		ptPointList[ i] = CElement::GetSelectedDrawPoints( selectedDrawingData, m_ptPointsList[ i].GetSavingPoint());
	drawingData.SetPointsCount( m_nPointsCount);

	return drawingData;
}

// ----------------------------------------------------------------------------
// ���鿡 ���� ����
CToolEnumData::ModifyType CMultiPointElement::GetModifyType_SidePoints(const CPoint& pt1, const CPoint& pt2, const CPoint& point) const
{
	CSelectableRegionCalculator selectableRegionCalculator;
	if(selectableRegionCalculator.IsPointInCircle(pt1, point))
		return CToolEnumData::MODIFY_CENTER;
	
	if(selectableRegionCalculator.IsPointInCircle(pt2, point))
		return CToolEnumData::MODIFY_CENTER;

	return CToolEnumData::MODIFY_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// class CVertElement

CVertElement::CVertElement(const CCoordinate& point, const CElementCommonEnvironment& environment):
	COnePointElement(point, environment)
{
}

// public =====================================================================
void CVertElement::ElementModify(const CElementModifyData& elementModifyData)
{
	//>> (2008/12/16 - Seung-Lyong Park) �м����� ����
	assert(elementModifyData.GetModifyType() == CToolEnumData::MODIFY_X || elementModifyData.GetModifyType() == CToolEnumData::MODIFY_CENTER);
	CCoordinate modifyPoint(elementModifyData.GetModifyPoint(COnePointElement::GetPoint()));
	
	if(modifyPoint.GetX() == 0.0 || modifyPoint.GetIndex() < 0)
	{
		modifyPoint = elementModifyData.GetModifyPoint(COnePointElement::GetPoint());
		if(modifyPoint.GetX() == 0.0 || modifyPoint.GetIndex() < 0)
			return;
	}

	COnePointElement::SetPoint(modifyPoint);
//	COnePointElement::SetPoint(elementModifyData.GetModifyPoint(COnePointElement::GetPoint()));
	//<< (2008/12/16 - Seung-Lyong Park) �м����� ����
}

void CVertElement::ElementModify_YValue(const CElementModifyData& elementModifyData)
{
}

void CVertElement::ElementModify_Coordinate(const CCoordinate& coordValue,const CToolEnumData::ModifyType eModifyType)
{
}

// protected ==================================================================
// ������ type ��������
CToolEnumData::ModifyType CVertElement::GetModifyType(const CDisplayData& displayData, const CPoint& elementDrawingPt, const CPoint& point) const
{
	return GetModifyType(displayData.GetDisplayRegion(), elementDrawingPt, point);
}

CToolEnumData::ModifyType CVertElement::GetModifyType(const CRect& region, const CPoint& elementDrawingPt, const CPoint& point) const
{
	if(PointInElementSelectableRegion(region, elementDrawingPt, point))
		return CToolEnumData::MODIFY_X;
	return CToolEnumData::MODIFY_NONE;
}

// private ====================================================================
// �����Ҽ� �ִ� ����
bool CVertElement::PointInElementSelectableRegion(const CRect& region, const CPoint& regionPt, const CPoint& point) const
{
	return CElement::PointInElementSelectableRegion(CPoint(regionPt.x, region.top), CPoint(regionPt.x, region.bottom), point);
}