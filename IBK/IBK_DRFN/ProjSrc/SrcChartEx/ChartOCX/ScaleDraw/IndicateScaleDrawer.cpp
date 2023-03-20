// IndicateScaleDrawer.cpp: implementation of the CIndicateScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndicateScaleDrawer.h"

#include "ScaleRegion.h"
#include "BaseData.h"
#include "Conversion.h"
#include "DataMath.h"

// packet.dll
#include "PacketBase.h"

// IndicatorCondition.lib
#include "IndicatorInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CIndicateScaleDrawer

CIndicateScaleDrawer::CIndicateScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CPriceScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

// scale text, grid�� �׸���.
/*
//-> ���ؼ��� �ִ� ���� ���ؼ��� ǥ��.
void CIndicateScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale)
{
	CList<double, double>* pBaseLineList = GetBaseLineList(drawingVertScaleData.GetIndicatorInfo());
	if(pBaseLineList == NULL || pBaseLineList->GetCount() <= 0){
		CPriceScaleDrawer::DrawTextOrGrid(pDC, drawingVertScaleData, scaleRegion, vertScale, 
			GetPacketDataType(drawingVertScaleData.GetPacket(), drawingVertScaleData.GetDisplayDataMinMax()));
		return;
	}

	// ���ؼ�
	DrawTextOrGrid_BaseLine(pDC, drawingVertScaleData, scaleRegion.GetScaleTextRegion(), vertScale, false, pBaseLineList);
}*/

void CIndicateScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale)
{
	CPriceScaleDrawer::DrawTextOrGrid(pDC, drawingVertScaleData, scaleRegion, vertScale, 
		GetPacketDataType(drawingVertScaleData.GetPacket(), drawingVertScaleData.GetDisplayDataMinMax()));

	CList<double, double>* pBaseLineList = GetBaseLineList(drawingVertScaleData.GetIndicatorInfo());
	if(pBaseLineList == NULL || pBaseLineList <= 0)
		return;

	// ���ؼ�
	DrawTextOrGrid_BaseLine(pDC, drawingVertScaleData, drawingVertScaleData.GetDrawingGraphRegion(), vertScale, true, pBaseLineList);
}

// ����
void CIndicateScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale)
{
	CList<double, double>* pBaseLineList = GetBaseLineList(drawingVertScaleData.GetIndicatorInfo());
	if(pBaseLineList == NULL || pBaseLineList->GetCount() <= 0){
		CPriceScaleDrawer::DrawTextOrGrid_Both(pDC, drawingVertScaleData, vertScaleRegion, vertScale, 
			GetPacketDataType(drawingVertScaleData.GetPacket(), drawingVertScaleData.GetDisplayDataMinMax()));
		return;
	}

	CRect scaleTextRegion_left = vertScaleRegion.GetLeftScaleTextRegion();
	CRect scaleTextRegion_right = vertScaleRegion.GetRightScaleTextRegion();

	// ���� : ���ؼ�
	DrawTextOrGrid_BaseLine(pDC, drawingVertScaleData, scaleTextRegion_left, vertScale, false, pBaseLineList);

	// ������ : data���� ǥ��.
	DrawTextOrGrid_Value(pDC, drawingVertScaleData, scaleTextRegion_right, vertScale);
}


// private ==========================================================================
// ���ؼ� �׸���
void CIndicateScaleDrawer::DrawTextOrGrid_BaseLine(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CRect& scaleRegion, const CVerticalScale& vertScale, 
		const bool bIsLeft, CList<double, double>* pBaseLineList)
{
	if(pBaseLineList == NULL || pBaseLineList->GetCount() <= 0)
		return;

	// 03.18.2002  invert���� ���Ѵ�
	bool bInvert = drawingVertScaleData.GetVertScaleInvertType();
	CString strPacketType = CDrawer::GetPacketDataType(drawingVertScaleData.GetPacket());
	int nowPt_x = (bIsLeft ? scaleRegion.left : scaleRegion.right);

	// 20081007 JS.Kim	����ڽð��� ����
	CPacket *pPacket = drawingVertScaleData.GetPacket();
	CString strType = pPacket->GetType();

	POSITION pos = pBaseLineList->GetHeadPosition();
	while(pos != NULL){
		double dBaseLine = pBaseLineList->GetNext(pos);
		int nowPt_y = GetVertScaleAbsolutePosition_Y(scaleRegion.bottom, scaleRegion.Height(),
			(double)drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin(), 
			drawingVertScaleData.GetDisplayDataMinMax().GetCount(), 
			GetChangeData(strPacketType, dBaseLine));

		// 03.08.2002  invert�̸� �����´�
		if(bInvert)
			nowPt_y = CDataConversion::InvertGraphDataPt(scaleRegion, nowPt_y);

		if(!IsPointInRegion(scaleRegion, nowPt_y))
			continue;

		// 20081007 JS.Kim	����ڽð��� ����
		CString strData = CDataConversion::DoubleToString( dBaseLine, strType);
	
		// ���� : ���ؼ����� ������ ����
		//CDrawer::DrawTextOut(pDC, 
		//	CVertScaleDrawer::GetTextPosition(scaleRegion.right, nowPt_y, pDC->GetTextExtent(strData), bIsRight), 
		//	strData, drawingVertScaleData.GetScaleColor().GetScaleTextColor());
		CSize textSize = pDC->GetTextExtent(strData);
		if(bIsLeft)
			textSize.cx = -MARGIN;

		CDrawer::DrawTextOut(pDC, 
			CVertScaleDrawer::GetTextPosition(nowPt_x, nowPt_y, textSize), 
			strData, drawingVertScaleData.GetScaleColor().GetScaleTextColor());
		CVertScaleDrawer::DrawGrid(pDC, drawingVertScaleData, vertScale.GetGridType(), nowPt_y, vertScale.GetActiveFlag());
	}
}

// data���� ��� ǥ��.
// 08.22.2001
// scale text, grid�� �׸���.
void CIndicateScaleDrawer::DrawTextOrGrid_Value(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CRect& scaleRegion, const CVerticalScale& vertScale)
{
	CPacket *pPacket = drawingVertScaleData.GetPacket();
	if( !pPacket) return;

	int start = 0, end = 0;
	double dGap = 0.0;
	bool bround = false;
	//sy 2002.8.28. --> gap�� ���� packetDataType ����
	CString dataType = CDrawer::GetPacketDataType(drawingVertScaleData.GetPacket());
	//CString dataType = CVertScaleDrawer::GetPacketDataType(dGap);

	if(!GetDrawingEnvironment(pDC, scaleRegion, drawingVertScaleData.GetDisplayDataMinMax(), 
		dataType, start, end, dGap, bround))
		return;

	// ���� �� �ڸ��� ���ϱ� - ��Ȯ�� Ÿ�� �ٽ� ���� 12.24.2001
	// update : 08.09.2002  -> min/max�� ���밪�߿� ū ������ ��������� ���Ѵ�
	BOOL bAutoScaleType = pPacket->IsAutoScaleType();
	CString strFormatTrans = CVertScaleDrawer::GetFormatTrans(drawingVertScaleData.GetDisplayDataMinMax(), dataType, bAutoScaleType);
	pPacket->SetScaleType( bAutoScaleType ? strFormatTrans : pPacket->GetType());

	// type ����
	int typeRegionY = CVertScaleDrawer::DrawType(pDC, scaleRegion, strFormatTrans, 
		bround, vertScale.GetVertScaleUnitType(), drawingVertScaleData.GetScaleColor().GetScaleTextColor());

	// scale ����
	CVertScaleDrawer::DrawTextOrGrid(pDC, drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin(), typeRegionY, 
		start, end, dGap, bround, drawingVertScaleData, vertScale,
		scaleRegion, CRect(0, 0, 0, 0), dataType, strFormatTrans, true);
}

// ���ؼ� ���� �����´�.
CList<double, double>* CIndicateScaleDrawer::GetBaseLineList(CIndicatorInfo* pIndicatorInfo)
{
	if(pIndicatorInfo == NULL)
		return NULL;

	return pIndicatorInfo->GetBaseLineList();
}

// packet data type ��������
CString CIndicateScaleDrawer::GetPacketDataType(CPacket* pPacket, const CDisplayDataMinMax& displayDataMinMax) const
{
	if(displayDataMinMax.GetDisplayMax() != 20 || displayDataMinMax.GetDisplayMin() != -20)
		return CDrawer::GetPacketDataType(pPacket);

	if(pPacket == NULL || pPacket->GetCount() <= 0)
		return CString("");

	double nMin = 0.0, nMax = 0.0;
	if(pPacket->GetMinMax(0, pPacket->GetCount() -1, nMin, nMax) < 0)
		return CString("");

	if(nMin == nMax)
		return CString("");
	return CDrawer::GetPacketDataType(pPacket);
}

// type�� ���� �� ����
double CIndicateScaleDrawer::GetChangeData(const CString& strPacketType, const double& orgData) const
{
	if(strPacketType.IsEmpty())
		return orgData;

	CString strType = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans('1', strPacketType);
	double nDataType = CDataConversion::GetDataType(strType);
	if(nDataType != 0)
		return (orgData/nDataType);
	return orgData;
}

// ǥ�� ������ǥ
int CIndicateScaleDrawer::GetVertScaleAbsolutePosition_Y(const int absolute_Org, const int absolute_L, 
			const double& relative_Org, const double& relative_L, const double& relative_y) const
{
	if(absolute_L <= 0, relative_L <= 0)
		return -1;
		
	int ChangingLength = CMath::Round(absolute_L * ((relative_y - relative_Org) / relative_L));
	return (absolute_Org - ChangingLength);
}

// draw �� �������� üũ
bool CIndicateScaleDrawer::IsPointInRegion(const CRect& region, const int y) const
{
	return (y >= region.top && y <= region.bottom);
}
