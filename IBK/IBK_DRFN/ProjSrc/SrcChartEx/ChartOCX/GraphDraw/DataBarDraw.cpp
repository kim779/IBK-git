// DataBarDraw.cpp: implementation of the CDataBarDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataBarDraw.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "DataMath.h"
#include "Conversion.h"
#include "BaseData.h"
#include "PacketPointerList.h"
#include "PacketBase.h"
#include "PacketList.h"

#include "XScaleManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
bool CDataViewDraw::DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData)
{
	const CList<CPacket*, CPacket*> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;

	double dRealWidth = CDraw::CalRealWidth(realRegion, displayAttributes);
	const CGraphDrawStyle& drawStyle = subGraphData.GetDrawStyle();
	const CGraphColor& graphColor = subGraphData.GetColor();
	const int nRealStart = displayAttributes.GetDataStartInBlock();
	const int nRealEnd = displayAttributes.GetDataEndInBlock();
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	bool bLog = drawingGraphData.DoesVertScaleLogType();
	bool bInvert = drawingGraphData.DoesVertScaleInvertType();

	switch(subGraphData.GetStyle().GetGraphDataViewStyle())
	{
/*
	case CGraphBaseData::TopDataBar_Not_Used:
		return DrawDataBar(pDC, realRegion, drawStyle.GetGraphDataBarDrawStyle(), 
			graphColor, nRealStart, nRealEnd, dRealWidth, packetList, false);
	case CGraphBaseData::BottomDataBar_Not_Used:
		return DrawDataBar(pDC, realRegion, drawStyle.GetGraphDataBarDrawStyle(), 
			graphColor, nRealStart, nRealEnd, dRealWidth, packetList, true);
*/
	case CGraphBaseData::PointSignal:
		return DrawPointSignal(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, bLog, bInvert);
	case CGraphBaseData::TopSignal:
		return DrawTopSignal(pDC, realRegion, drawStyle.GetGraphDataSignalDrawStyle(), graphColor, 
			nRealStart, nRealEnd, dViewMax, dViewMin, dRealWidth, packetList, bLog, bInvert);
	case CGraphBaseData::BottomSignal:
		return DrawSignal_NoCharArrowSignal(pDC, realRegion, drawStyle.GetGraphDataSignalDrawStyle(), graphColor, 
			dRealWidth, nRealStart, nRealEnd, dViewMax, dViewMin, packetList, bLog, bInvert, false);
	case CGraphBaseData::CompartText:
		return DrawCompartText(pDC, realRegion, graphColor, nRealStart, nRealEnd, dRealWidth, packetList);
	}
	return false;
}


bool CDataViewDraw::DrawGraph(CDC* pDC,
					   const CRect& realRegion, 
					   const CDisplayAttributes& displayAttributes, 
					   const CDrawingGraphData& drawingGraphData,
					   const CSubGraphData& subGraphData,
					   const CString& strRQ,
				       CXScaleManager* pxScaleManager)
{
	if(pxScaleManager && pxScaleManager->GetThreadFlag() == THREAD_CALCULATION)
		return true;

	const CList<CPacket*, CPacket*> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;

	double dRealWidth = CDraw::CalRealWidth(realRegion, displayAttributes);
	const CGraphDrawStyle& drawStyle = subGraphData.GetDrawStyle();
	const CGraphColor& graphColor = subGraphData.GetColor();
	const int nRealStart = displayAttributes.GetDataStartInBlock();
	const int nRealEnd = displayAttributes.GetDataEndInBlock();
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	bool bLog = drawingGraphData.DoesVertScaleLogType();
	bool bInvert = drawingGraphData.DoesVertScaleInvertType();
	int nColumn = drawingGraphData.GetColumnIndex();
	
	switch(subGraphData.GetStyle().GetGraphDataViewStyle())
	{
/*
	case CGraphBaseData::TopDataBar_Not_Used:
		return DrawDataBar(pDC, realRegion, drawStyle.GetGraphDataBarDrawStyle(), 
			graphColor, nRealStart, nRealEnd, dRealWidth, packetList, false);
	case CGraphBaseData::BottomDataBar_Not_Used:
		return DrawDataBar(pDC, realRegion, drawStyle.GetGraphDataBarDrawStyle(), 
			graphColor, nRealStart, nRealEnd, dRealWidth, packetList, true);
*/
	case CGraphBaseData::PointSignal:
		return DrawPointSignal(pDC, realRegion, drawStyle, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, bLog, bInvert);
	case CGraphBaseData::TopSignal:
		return DrawTopSignal(pDC, realRegion, drawStyle.GetGraphDataSignalDrawStyle(), graphColor, 
			nRealStart, nRealEnd, dViewMax, dViewMin, dRealWidth, packetList, bLog, bInvert, strRQ, pxScaleManager, nColumn, drawingGraphData);
	case CGraphBaseData::BottomSignal:
		return DrawSignal_NoCharArrowSignal(pDC, realRegion, drawStyle.GetGraphDataSignalDrawStyle(), graphColor, 
			dRealWidth, nRealStart, nRealEnd, dViewMax, dViewMin, packetList, bLog, bInvert, false);
	case CGraphBaseData::CompartText:
		return DrawCompartText(pDC, realRegion, graphColor, nRealStart, nRealEnd, dRealWidth, packetList);
	}
	return false;

}

// private ==============================================================================
/*
bool CDataViewDraw::DrawDataBar(CDC* pDC, const CRect& realRegion, const CGraphBaseData::DATABARDRAWSTYLE drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double dRealWidth, const CList<CPacket*, CPacket*>& packetList, const bool bIsBottom)
{
	if(drawStyle == CGraphBaseData::Char_Bar)
		return DrawCharBar(pDC, realRegion, graphColor, nRealStart, nRealEnd, dRealWidth, packetList, bIsBottom);
	if(drawStyle == CGraphBaseData::Hatchat45Degrees_Bar)
		return DrawHatchat45DegreesDataBar(pDC, realRegion, graphColor, nRealStart, nRealEnd, dRealWidth, packetList);
	else if(drawStyle == CGraphBaseData::Fill_Bar)
		return DrawFillDataBar(pDC, realRegion, graphColor, nRealStart, nRealEnd, dRealWidth, packetList);

	return false;
}
*/

// ������ �����ʹ� ǥ�� ���Ѵ�.
bool CDataViewDraw::DrawCharBar(CDC* pDC, const CRect& realRegion, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double dRealWidth, const CList<CPacket*, CPacket*>& packetList, const bool bIsBottom)
{
	CDraw::DrawRectangle(pDC, realRegion, graphColor.GetColor1());

	int nLastestRealIndex = -1;
	CList<double, double>* pDataList = NULL;
	POSITION pos = NULL;
	if(!GetPacketData_CharBar(packetList, nRealStart, nLastestRealIndex, pDataList, pos))
		return false;

	CPen newPen(PS_SOLID, 1, graphColor.GetColor1());
	CPen* pOldPen = pDC->SelectObject(&newPen);
	COLORREF oldTextColor = pDC->SetTextColor(graphColor.GetColor3());
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);

	int nPrevPt_x = realRegion.left;
	for(int nIndex = 0; nIndex <= (nRealEnd - nRealStart); nIndex++){
		if(pos == NULL)
			break;

		// �� ĭ�� �޺κ��� �������ش�.
		int nCurPt_x = CDataConversion::GetVerticalPoint(realRegion, nIndex +1, dRealWidth, 0.0, 0.0, 0.0, false).x;
		CDraw::DrawLine(pDC, CPoint(nCurPt_x, realRegion.top), CPoint(nCurPt_x, realRegion.bottom));

		if(nLastestRealIndex >= 0 && nLastestRealIndex == (nRealStart + nIndex))
			pDC->SetTextColor(graphColor.GetColor5());
		else if(nLastestRealIndex >= 0 && nLastestRealIndex +1 == (nRealStart + nIndex))
			pDC->SetTextColor(graphColor.GetColor3());

		CString strData = CDataConversion::DoubleToString(pDataList->GetNext(pos), 0);
		//text�� "0" �� ��쿣 ǥ���� ���� �ʴ´�.
		if(!strData.IsEmpty() && strData != "0")
			CDraw::TextOut(pDC, CRect(nPrevPt_x, realRegion.top, nCurPt_x, realRegion.bottom), strData);

		nPrevPt_x = nCurPt_x;
	}

	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
	pDC->SetTextColor(oldTextColor);
	pDC->SetBkMode(oldBkMode);
	return true;
}

bool CDataViewDraw::GetPacketData_CharBar(const CList<CPacket*, CPacket*>& packetList, const int nStartIndex, 
		int& nLastestRealIndex, CList<double, double>*& pDataList, POSITION& pos) const
{
	if(packetList.GetHeadPosition() == NULL)
		return false;

	CPacket* pPacket = packetList.GetHead();
	if(pPacket == NULL)
		return false;

	pDataList = pPacket->GetnumericDataList();
	if(pDataList == NULL || pDataList->GetCount() <= 0)
		return false;

	pos = pDataList->FindIndex(nStartIndex);
	return (pos != NULL);
}

bool CDataViewDraw::DrawHatchat45DegreesDataBar(CDC* pDC, const CRect& realRegion, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double dRealWidth, const CList<CPacket*, CPacket*>& packetList)
{
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)
		return false;
	CPacket* dataPacket = packetList.GetNext(pos);
	if(dataPacket == NULL)
		return false;
	//displayData
	POSITION dataPos = dataPacket->GetnumericDataList()->FindIndex(nRealStart);
	if(dataPos == NULL) 
		return false;

	CPen BarPen1;
	CPen BarPen2;
	CPen BarPen3;
	BarPen1.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
	BarPen2.CreatePen(PS_SOLID, 1, graphColor.GetColor2());
	BarPen3.CreatePen(PS_SOLID, 1, graphColor.GetColor3());

	CBrush Brush1	(HS_BDIAGONAL, graphColor.GetColor1());
	CBrush Brush2	(HS_FDIAGONAL, graphColor.GetColor2());
	CBrush Brush3	(HS_DIAGCROSS, graphColor.GetColor3());

	int nWidth = CMath::Round(dRealWidth/2);
	CPen* pOldPen = pDC->SelectObject(&BarPen1);
	CBrush* pOldBrush = pDC->SelectObject(&Brush1);

	double ViewMin = 0.0;
	double ViewMax = 10.0;
	double tradeMode = 0.0;
	CPoint SaveDataPt;
	CPoint beforeDataPt;

	const CRect realRect(realRegion.left, realRegion.top, realRegion.right+1, realRegion.bottom+2);
	for(int i = nRealStart; i <= nRealEnd; i++){
		if(dataPos == NULL)
			break;
		double data = dataPacket->GetnumericDataList()->GetNext(dataPos);
		CPoint dataPt = CDataConversion::GetVerticalPoint(realRegion, (i - nRealStart), dRealWidth, ViewMin, ViewMax, data);
		if(i == nRealStart){
			tradeMode = data;
			SaveDataPt = dataPt;
			beforeDataPt = SaveDataPt;
		}
		if(tradeMode == 0){
			pDC->SelectObject(&BarPen3);
			pDC->SelectObject(GetStockObject(NULL_BRUSH));
			if(nRealStart == nRealEnd){
			}
			else if(data > 0){
				SaveDataPt = dataPt;
				tradeMode = data;
			}
		}
		else if(tradeMode == 1){
			pDC->SelectObject(GetStockObject(NULL_PEN));
			pDC->SelectObject(&Brush1);
			if(nRealStart == nRealEnd){
				pDC->Rectangle(realRect);
			}
			else if(data != tradeMode){
				CRect rect = CRect(SaveDataPt.x - nWidth, realRect.top, beforeDataPt.x + nWidth, realRect.bottom);
				if(i == nRealEnd)
					rect.right = realRect.right;
				CDraw::DrawRectangle(pDC, rect);
				pDC->SelectObject(&BarPen1);
				pDC->MoveTo(rect.left, rect.top);
				pDC->LineTo(rect.left, rect.bottom);
				pDC->MoveTo(rect.right-1, rect.top);
				pDC->LineTo(rect.right-1, rect.bottom);
				SaveDataPt = dataPt;
				tradeMode = data;
			}
		}
		else if(tradeMode == 2){
			pDC->SelectObject(GetStockObject(NULL_PEN));
			pDC->SelectObject(&Brush2);
			if(nRealStart == nRealEnd){
				pDC->Rectangle(realRect);
			}
			else if(data != tradeMode){
				CRect rect = CRect(SaveDataPt.x - nWidth, realRect.top, dataPt.x + nWidth, realRect.bottom);
				if(i == nRealEnd)
					rect.right = realRect.right;
				CDraw::DrawRectangle(pDC, rect);
				pDC->SelectObject(BarPen2);
				pDC->MoveTo(rect.left, rect.top);
				pDC->LineTo(rect.left, rect.bottom);
				pDC->MoveTo(rect.right-1, rect.top);
				pDC->LineTo(rect.right-1, rect.bottom);
				SaveDataPt = dataPt;
				tradeMode = data;
			}
		}
			beforeDataPt = dataPt;
	}
	pDC->SelectObject(&BarPen3);
	pDC->SelectObject(GetStockObject(NULL_BRUSH));

	pDC->MoveTo(realRegion.left, realRegion.top);
	pDC->LineTo(realRegion.right, realRegion.top);
	pDC->LineTo(realRegion.right, realRegion.bottom+2);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	BarPen1.DeleteObject();
	BarPen2.DeleteObject();
	BarPen3.DeleteObject();
	Brush1.DeleteObject();
	Brush2.DeleteObject();
	Brush3.DeleteObject();
	return true;
}

//sy 2004.04.16. -> �׸��� ��ġ�� ���� ���� �������� �Ѵ�.
bool CDataViewDraw::DrawFillDataBar(CDC* pDC, const CRect& realRegion, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double dRealWidth, const CList<CPacket*, CPacket*>& packetList)
{
	//������ �������� ���� ��ġ�� ������ �κ�======= 
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)
		return false;
	CPacket* pPacket = packetList.GetNext(pos);
	if(pPacket == NULL)
		return false;
	
	CList<double, double>* pDataList = pPacket->GetnumericDataList();
	if(pDataList == NULL)
		return false;
	POSITION curPos = pDataList->FindIndex(nRealStart);
	if(curPos == NULL) 
		return false;

	CPen BarPen1, BarPen2;
	BarPen1.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
	BarPen2.CreatePen(PS_SOLID, 1, graphColor.GetColor2());

	CBrush Brush1, Brush2;
	Brush1.CreateSolidBrush(graphColor.GetColor1());
	Brush2.CreateSolidBrush(graphColor.GetColor2());

	// ��ü �׵θ��� �׸���.
	CDraw::DrawRectangle(pDC, CRect(realRegion.left - 1, realRegion.top, realRegion.right, realRegion.bottom + 1), graphColor.GetColor3());

	// �����͸� �׸���.
	CPen* pOldPen = pDC->SelectObject(&BarPen1);
	CBrush* pOldBrush = pDC->SelectObject(&Brush1);

	double dPreData = 0.0;
	POSITION prePos = curPos;
	pDataList->GetPrev(prePos);
	if(prePos != NULL)
		dPreData = pDataList->GetAt(prePos);
	int prePt_x = CDataConversion::GetVerticalPoint(realRegion, 0, dRealWidth, 0, 1, dPreData, false).x;
	int y_top = realRegion.top + 1; // �׵θ��� �׸� ���⸦ ����.
	int y_bottom = realRegion.bottom;
	if(realRegion.Height() > 8)
	{
		y_top += 1;
		y_bottom -= 1;
	}
	int y_center1 = y_top + (y_bottom - y_top)/2 - 1;
	int y_center2 = y_center1;
	if((y_bottom - y_top) > 4)
	{
		y_center2 += 2; // gap
	}

	for(int i = nRealStart; i <= nRealEnd; i++){
		if(curPos == NULL)
			break;
		double dCurData = pDataList->GetNext(curPos);
		int curPt_x = CDataConversion::GetVerticalPoint(realRegion, 
			(i - nRealStart + 1), dRealWidth, 0, 1, dCurData, false).x;

		if(curPt_x < 0)
			continue;

		int drawPt_x = prePt_x;
		if(curPt_x - prePt_x > 1)
			drawPt_x += 1;

		// 1: ������  2: ���ø�  3: ����,����
		if(dCurData == 1 || dCurData == 3){ // ���� ����
			pDC->SelectObject(&BarPen1);
			pDC->SelectObject(&Brush1);
			CDraw::DrawRectangle(pDC, CRect(drawPt_x, y_top, curPt_x, y_center1));
		}

		if(dCurData == 2 || dCurData == 3){ // ���� ����
			pDC->SelectObject(&BarPen2);
			pDC->SelectObject(&Brush2);
			CDraw::DrawRectangle(pDC, CRect(drawPt_x, y_center2, curPt_x, y_bottom));
		}

		prePt_x = curPt_x;
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	BarPen1.DeleteObject();
	BarPen2.DeleteObject();
	Brush1.DeleteObject();
	Brush2.DeleteObject();
	return true;
}

// --------------------------------------------------------------------------------------
// 1 -> �ż�  2 -> �ŵ�
bool CDataViewDraw::DrawPointSignal(CDC* pDC, const CRect& realRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor,
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin,
		const double dRealWidth, const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert)
{
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)
		return false;

	CPacket* pDataPacket = packetList.GetNext(pos);
	if(pDataPacket == NULL || pDataPacket->GetCount() <= 0)
		return false;

	pDataPacket->SetBaseData("x1", "x1", 0);

	//�׷����� ��ġ�� ��Ÿ���� packet.
	CPacket* pPointHighPacket = NULL;
	CPacket* pPointLowPacket = NULL;
	if(pos != NULL){ // ���߿� �ϳ��� NULL �� �ƴϸ� ���� �ɷ� �Ѵ�.
		pPointHighPacket = packetList.GetNext(pos);
		if(pos != NULL)
			pPointLowPacket = packetList.GetNext(pos);
		else
			pPointLowPacket = pPointHighPacket;
	}
	else{ // �Ѵ� ���� ��쿣 <��>, <�ð�>�� �Ѵ�.
		pPointHighPacket = pDataPacket->GetPacketList()->GetBaseData( _MTEXT( C0_HIGH));
		pPointLowPacket = pDataPacket->GetPacketList()->GetBaseData( _MTEXT( C0_LOW));
	}

	if(pPointHighPacket == NULL || pPointLowPacket == NULL)
		return false;

	CList<double, double>* pDataList = pDataPacket->GetnumericDataList();
	CList<double, double>* pPointHighList = pPointHighPacket->GetnumericDataList();
	CList<double, double>* pPointLowList = pPointLowPacket->GetnumericDataList();
	if(pDataList == NULL || pPointHighList == NULL || pPointLowList == NULL)
		return false;

	POSITION dataPos = pDataList->FindIndex(nRealStart);
	POSITION pointHighPos = pPointHighList->FindIndex(nRealStart);
	POSITION pointLowPos = pPointLowList->FindIndex(nRealStart);
	if(dataPos == NULL || pointHighPos == NULL|| pointLowPos == NULL) 
		return false;

	CPen Pen1, Pen2;
	Pen1.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
	Pen2.CreatePen(PS_SOLID, 1, graphColor.GetColor3());
	CBrush Brush1 (graphColor.GetColor1());
	CBrush Brush2 (graphColor.GetColor3());
	CPen* pOldPen = pDC->SelectObject(&Pen1);
	CBrush* pOldBrush = pDC->SelectObject(&Brush1);

	int nWidth = CMath::Round(dRealWidth/* 2/3*/);
	if(nWidth > 15)
		nWidth = 15;
	// (2004.02.11, ��¿�) ȭ��ǥ�� ���� �������� ���δ�.
	else if(nWidth < 5) nWidth = 5;

	//log�� ��� ==================================
	double realDataMax = CDraw::CalLogValue(bLog, dViewMax);
	double realDataMin = CDraw::CalLogValue(bLog, dViewMin);	

	pDC->SelectObject(&Pen1);
	pDC->SelectObject(&Brush1);
	bool checkUpDown;
	for(int i = nRealStart; i <= nRealEnd; i++){
		if(dataPos == NULL || pointHighPos == NULL || pointLowPos == NULL)
			break;

		//volumnData�� �ش��ϴ� �׷��� ������ ����Ʈ�� ������ �´�.
		double data = pDataList->GetNext(dataPos);
		double highPointData = CDraw::CalLogValue(bLog, pPointHighList->GetNext(pointHighPos));
		double lowPointData = CDraw::CalLogValue(bLog, pPointLowList->GetNext(pointLowPos));
		if(data != 1.0 && data != 2.0)
			continue;
			
		CPoint highDataPt = CDataConversion::GetVerticalPoint(realRegion, (i - nRealStart), dRealWidth, realDataMin, realDataMax, highPointData);
		CPoint lowDataPt = CDataConversion::GetVerticalPoint(realRegion, (i - nRealStart), dRealWidth, realDataMin, realDataMax, lowPointData);

		//draw Ÿ�Կ� ���� �׷��ش�.
		if(bInvert){
			highDataPt.y = CDataConversion::InvertGraphDataPt(realRegion, highDataPt.y);
			lowDataPt.y = CDataConversion::InvertGraphDataPt(realRegion, lowDataPt.y);
			if(checkUpDown)
				checkUpDown = false;
			else
				checkUpDown = true;
		}
		CPoint pPoint3[3];
		CPoint pPoint7[7];
		if(data == 1.0)
		{
			pDC->SelectObject(&Pen1);
			pDC->SelectObject(&Brush1);
			//Ÿ��üũ
			checkUpDown = false;
			if(bInvert)
				checkUpDown = true;
			if(drawStyle.GetGraphDataSignalDrawStyle() == CGraphBaseData::Triangle_Signal){
				DrawTriangleType(pDC, pPoint3, lowDataPt, nWidth, realRegion, checkUpDown);
				pDC->Polygon(pPoint3, 3);
			}
			else{
				DrawArrowType(pDC, pPoint7, lowDataPt, nWidth, realRegion, checkUpDown);
				pDC->Polygon(pPoint7, 7);
			}
		}
		else if(data == 2.0)
		{
			pDC->SelectObject(&Pen2);
			pDC->SelectObject(&Brush2);
			//Ÿ��üũ
			checkUpDown = true;
			if(bInvert)
				checkUpDown = false;
			if(drawStyle.GetGraphDataSignalDrawStyle() == CGraphBaseData::Triangle_Signal){
				DrawTriangleType(pDC, pPoint3, highDataPt, nWidth, realRegion, checkUpDown);
				pDC->Polygon(pPoint3, 3);
			}
			else{
				DrawArrowType(pDC, pPoint7, highDataPt, nWidth, realRegion, checkUpDown);
				pDC->Polygon(pPoint7, 7);
			}
		}
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	Pen1.DeleteObject();
	Brush1.DeleteObject();
	Pen2.DeleteObject();
	Brush2.DeleteObject();
	return true;
}

// --------------------------------------------------------------------------------------
bool CDataViewDraw::DrawTopSignal(CDC* pDC, const CRect& realRegion, const CGraphBaseData::DATASIGNALDRAWSTYLE drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, const double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert)
{
	if((int)drawStyle < 0)
		return false;

	if(drawStyle == CGraphBaseData::CharArrow_Signal){
		return DrawCharArrowSignal(pDC, realRegion, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, bLog, bInvert, true, false);
	}

	return DrawSignal_NoCharArrowSignal(pDC, realRegion, drawStyle, graphColor, 
		dRealWidth, nRealStart, nRealEnd, dViewMax, dViewMin, packetList, bLog, bInvert, true);
}

bool CDataViewDraw::DrawTopSignal(CDC* pDC, const CRect& realRegion, const CGraphBaseData::DATASIGNALDRAWSTYLE drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, const double dRealWidth, 
								  const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert, const CString& strRQ, CXScaleManager* pxScaleManager, int nColumn, const CDrawingGraphData& drawingGraphData)
{
	if((int)drawStyle < 0)
		return false;
	
	if(drawStyle == CGraphBaseData::CharArrow_Signal){
		return DrawCharArrowSignal(pDC, realRegion, graphColor, nRealStart, nRealEnd, 
			dViewMax, dViewMin, dRealWidth, packetList, bLog, bInvert, true, false, strRQ, pxScaleManager, nColumn, drawingGraphData);
	}
	
	return DrawSignal_NoCharArrowSignal(pDC, realRegion, drawStyle, graphColor, 
		dRealWidth, nRealStart, nRealEnd, dViewMax, dViewMin, packetList, bLog, bInvert, true);
}

//CharArrow_Signal,		// ����+ȭ��ǥ
bool CDataViewDraw::DrawCharArrowSignal(CDC* pDC, const CRect& realRegion, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, const double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert, bool bHighLow, bool bRightLeft)
{
	//������ �������� ���� ��ġ�� ������ �κ�======= 
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)
		return false;
	CPacket* dataPacket = packetList.GetNext(pos);
	if(pos == NULL)
		return false;
	CPacket* pointPacket = packetList.GetNext(pos); //�׷����� ��ġ�� ��Ÿ���� packet == indicator���� ������ �;� �Ѵ�.
	if(dataPacket == NULL || pointPacket == NULL) return false;
	if( pointPacket->GetnumericDataList() == NULL) return false;
	if( !dataPacket->GetstrDataList()) return false;

	//displayData
	//POSITION dataPos = dataPacket->GetstrDataList()->FindIndex(nRealStart-1);
	//POSITION pointPos = pointPacket->GetnumericDataList()->FindIndex(nRealStart-1);
	POSITION dataPos = dataPacket->GetstrDataList()->FindIndex(nRealStart);
	POSITION pointPos = pointPacket->GetnumericDataList()->FindIndex(nRealStart);
	if(dataPos == NULL || pointPos == NULL) 
		return false;

	CPen Pen1;
	Pen1.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
	CBrush Brush1 (graphColor.GetColor1());
	CPen* pOldPen = pDC->SelectObject(&Pen1);
	CBrush* pOldBrush = pDC->SelectObject(&Brush1);

	int nWidth = CMath::Round(dRealWidth/* 2/3*/);
	if(nWidth > 15)
		nWidth = 15;
	else if(nWidth < 5)
		nWidth = 5;

	//log�� ��� ==================================
	double realDataMax = CDraw::CalLogValue(bLog, dViewMax);
	double realDataMin = CDraw::CalLogValue(bLog, dViewMin);	

	pDC->SelectObject(&Pen1);
	pDC->SelectObject(&Brush1);
	COLORREF clr = pDC->SetTextColor(graphColor.GetColor2());
	int MODE = pDC->SetBkMode(TRANSPARENT);

	double dPrevData = 0;
	for(int i = nRealStart; i <= nRealEnd; i++)
	{
		if(dataPos == NULL || pointPos == NULL)
			break;
		//volumnData�� �ش��ϴ� �׷��� ������ ����Ʈ�� ������ �´�.
		CString data = dataPacket->GetstrDataList()->GetNext(dataPos);
		
		double dData = pointPacket->GetnumericDataList()->GetNext(pointPos);

		// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
		//		if Error, Set All with Close for Base or Market Close Price.
		if( 0 == dData) dData = dPrevData;
		else			dPrevData = dData;
		
		double PointData = CDraw::CalLogValue(bLog, dData);
		CPoint dataPt = CDataConversion::GetVerticalPoint(realRegion, (i - nRealStart), dRealWidth, realDataMin, realDataMax, PointData);
		if(bInvert)
			dataPt.y = CDataConversion::InvertGraphDataPt(realRegion, dataPt.y);
		//draw Ÿ�Կ� ���� �׷��ش�.
		if(!data.IsEmpty())
		{
			//sy 2005.11.23.
			CSize strDataSize = pDC->GetTextExtent(data);
			bool bIsRight = bRightLeft, bIsUp = bHighLow;
			int nArrowLength = 11, nArrowHight = 20;
			GetArrowLengthAndHight(realRegion, dataPt.y, bHighLow, nArrowLength, nArrowHight, bIsUp);
			int nArrowPos_x = nArrowLength + MARGIN;
			if(bRightLeft)
			{
				if((dataPt.x + (nArrowPos_x + strDataSize.cx)) >= realRegion.right)
				{
					nArrowPos_x = dataPt.x - (nArrowPos_x + strDataSize.cx);
				}
				else
				{
					bIsRight = false;
					nArrowPos_x = dataPt.x + nArrowPos_x;
				}
			}
			else
			{
				if((dataPt.x - (nArrowPos_x + strDataSize.cx)) <= realRegion.left)
				{
					nArrowPos_x = dataPt.x + nArrowPos_x;
				}
				else
				{
					bIsRight = true;
					nArrowPos_x = dataPt.x - (nArrowPos_x + strDataSize.cx);
				}
			}

			int nArrowPos_y = nArrowHight +1;
			if(bIsUp)
				nArrowPos_y = dataPt.y - (nArrowPos_y + strDataSize.cy / 2);//��
			else
				nArrowPos_y = dataPt.y + (nArrowPos_y - (int)(strDataSize.cy / 2.0 - 1));//�Ʒ�

			pDC->TextOut(nArrowPos_x, nArrowPos_y, data);
			DrawArrow(pDC, dataPt, bIsRight, bIsUp, strDataSize, nArrowLength, nArrowHight);
			//sy end
		}
	}

	pDC->SetBkMode(MODE);
	pDC->SetTextColor(clr);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	Pen1.DeleteObject();
	Brush1.DeleteObject();
	return true;
}
bool CDataViewDraw::DrawCharArrowSignal(CDC* pDC, const CRect& realRegion, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, const double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert, bool bHighLow, bool bRightLeft, const CString& strRQ, CXScaleManager* pxScaleManager, int nColumn, const CDrawingGraphData& drawingGraphData)
{
	//������ �������� ���� ��ġ�� ������ �κ�======= 
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)
		return false;
	CPacket* dataPacket = packetList.GetNext(pos);
	if(pos == NULL)
		return false;
	CPacket* pointPacket = packetList.GetNext(pos); //�׷����� ��ġ�� ��Ÿ���� packet == indicator���� ������ �;� �Ѵ�.
	if(dataPacket == NULL || pointPacket == NULL) return false;
	if( pointPacket->GetnumericDataList() == NULL) return false;
	if( !dataPacket->GetstrDataList()) return false;

	//displayData
	//POSITION dataPos = dataPacket->GetstrDataList()->FindIndex(nRealStart-1);
	//POSITION pointPos = pointPacket->GetnumericDataList()->FindIndex(nRealStart-1);
	POSITION dataPos = dataPacket->GetstrDataList()->FindIndex(nRealStart);
	POSITION pointPos = pointPacket->GetnumericDataList()->FindIndex(nRealStart);
	if(dataPos == NULL || pointPos == NULL) 
		return false;

	CPen Pen1;
	Pen1.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
	CBrush Brush1 (graphColor.GetColor1());
	CPen* pOldPen = pDC->SelectObject(&Pen1);
	CBrush* pOldBrush = pDC->SelectObject(&Brush1);

	int nWidth = CMath::Round(dRealWidth/* 2/3*/);
	if(nWidth > 15)
		nWidth = 15;
	else if(nWidth < 5)
		nWidth = 5;

	//log�� ��� ==================================
	double realDataMax = CDraw::CalLogValue(bLog, dViewMax);
	double realDataMin = CDraw::CalLogValue(bLog, dViewMin);	

	pDC->SelectObject(&Pen1);
	pDC->SelectObject(&Brush1);
	COLORREF clr = pDC->SetTextColor(graphColor.GetColor2());
	int MODE = pDC->SetBkMode(TRANSPARENT);

	int nDTI_Start = -1, nDTI_End = -1;
	pxScaleManager->GetDrawIndex(strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
	if(nDTI_Start == -1) return true;

	int nZeroDTI = 0;
	CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( nColumn, strRQ, nZeroDTI);
	if( !pblaAreafDTI) return false;
	CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
	pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

	double dPrevData = 0;
	CXScaleManager::PAREA parea;
	for( int nDTI = nDTI_Start; nDTI <= nDTI_End ; nDTI++)
	{
		parea = pblaAreafDTI->GetNext( dblCursor);

		if(dataPos == NULL || pointPos == NULL)
			break;
		//volumnData�� �ش��ϴ� �׷��� ������ ����Ʈ�� ������ �´�.
		CString data = dataPacket->GetstrDataList()->GetNext(dataPos); 
		
		double dData = pointPacket->GetnumericDataList()->GetNext(pointPos);

		// (2007/3/12 - Seung-Won, Bae) Check Normal Candle Low <= Open/Close <= High
		//		if Error, Set All with Close for Base or Market Close Price.
		if( 0 == dData) dData = dPrevData;
		else			dPrevData = dData;
		
		double PointData = CDraw::CalLogValue(bLog, dData);
		CPoint dataPt = CDataConversion::GetVerticalPoint(realRegion, ( nDTI - nDTI_Start), dRealWidth, realDataMin, realDataMax, PointData);
		if(bInvert)
			dataPt.y = CDataConversion::InvertGraphDataPt(realRegion, dataPt.y);
		dataPt.x = parea->center;

		//draw Ÿ�Կ� ���� �׷��ش�.
		if(!data.IsEmpty())
		{
			//sy 2005.11.23.
		CSize strDataSize = pDC->GetTextExtent(data);
			bool bIsRight = bRightLeft, bIsUp = bHighLow;
			int nArrowLength = 11, nArrowHight = 20;
			GetArrowLengthAndHight(realRegion, dataPt.y, bHighLow, nArrowLength, nArrowHight, bIsUp);
			int nArrowPos_x = nArrowLength + MARGIN;
			if(bRightLeft)
			{
				if((dataPt.x + (nArrowPos_x + strDataSize.cx)) >= realRegion.right)
				{
					nArrowPos_x = dataPt.x - (nArrowPos_x + strDataSize.cx);
				}
					else
				{
					bIsRight = false;
					nArrowPos_x = dataPt.x + nArrowPos_x;
				}
			}
					else
			{
				if((dataPt.x - (nArrowPos_x + strDataSize.cx)) <= realRegion.left)
				{
					nArrowPos_x = dataPt.x + nArrowPos_x;
				}
				else
				{
					bIsRight = true;
					nArrowPos_x = dataPt.x - (nArrowPos_x + strDataSize.cx);
			}
				}

			int nArrowPos_y = nArrowHight +1;
			if(bIsUp)
				nArrowPos_y = dataPt.y - (nArrowPos_y + strDataSize.cy / 2);//��
					else
				nArrowPos_y = dataPt.y + (nArrowPos_y - (int)(strDataSize.cy / 2.0 - 1));//�Ʒ�

			pDC->TextOut(nArrowPos_x, nArrowPos_y, data);
			DrawArrow(pDC, dataPt, bIsRight, bIsUp, strDataSize, nArrowLength, nArrowHight);
			//sy end
				}
			}

	pDC->SetBkMode(MODE);
	pDC->SetTextColor(clr);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	Pen1.DeleteObject();
	Brush1.DeleteObject();
	return true;
}

//sy 2005.11.23.
void CDataViewDraw::GetArrowLengthAndHight(const CRect& graphRegion, const int dataPt_y, const bool bHighLow, int& nLength, int& nHight, bool& bIsUp) const
{
	nLength = 11;
	nHight = 20;
	
	int nMaxGap = (int)(graphRegion.Height()/5 * 4);
	// ���� �׸���
	if(bHighLow)
	{
		bIsUp = true;
		if(graphRegion.top > (graphRegion.bottom - nMaxGap - 20))
		{
			bIsUp = false;
			nHight = (graphRegion.bottom - 20) - dataPt_y;
		}
		else
		{
			nHight = dataPt_y - (graphRegion.bottom - nMaxGap);
		}
	}
	// �Ʒ��� �׸���
	else
	{
		bIsUp = false;
		if(graphRegion.bottom < (graphRegion.top + nMaxGap + 20))
		{
			bIsUp = true;
			nHight = dataPt_y - (graphRegion.top + 20);
		}
		else
		{
			nHight = (graphRegion.top + nMaxGap) - dataPt_y;
		}
	}
	
	if(nHight < 20)
		nHight = 20;
}
//sy end

/*bool CDataViewDraw::DrawCharArrowSignal(CDC* pDC, const CRect& realRegion, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, const double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert, bool bHighLow)
{
	//������ �������� ���� ��ġ�� ������ �κ�======= 
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)
		return false;
	CPacket* dataPacket = packetList.GetNext(pos);
	if(pos == NULL)
		return false;
	CPacket* pointPacket = packetList.GetNext(pos); //�׷����� ��ġ�� ��Ÿ���� packet == indicator���� ������ �;� �Ѵ�.
	if(dataPacket == NULL || pointPacket == NULL)
		return false;
	if(!dataPacket->GetstrDataList())
		return false;
	//displayData
	//POSITION dataPos = dataPacket->GetstrDataList()->FindIndex(nRealStart-1);
	//POSITION pointPos = pointPacket->GetnumericDataList()->FindIndex(nRealStart-1);
	POSITION dataPos = dataPacket->GetstrDataList()->FindIndex(nRealStart);
	POSITION pointPos = pointPacket->GetnumericDataList()->FindIndex(nRealStart);
	if(dataPos == NULL || pointPos == NULL) 
		return false;

	CPen Pen1;
	Pen1.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
	CBrush Brush1 (graphColor.GetColor1());
	CPen* pOldPen = pDC->SelectObject(&Pen1);
	CBrush* pOldBrush = pDC->SelectObject(&Brush1);

	int nWidth = CMath::Round(dRealWidth);
	if(nWidth > 15)
		nWidth = 15;
	else if(nWidth < 5)
		nWidth = 5;

	//log�� ��� ==================================
	double realDataMax = CDraw::CalLogValue(bLog, dViewMax);
	double realDataMin = CDraw::CalLogValue(bLog, dViewMin);

	pDC->SelectObject(&Pen1);
	pDC->SelectObject(&Brush1);
	COLORREF clr = pDC->SetTextColor(graphColor.GetColor2());
	int MODE = pDC->SetBkMode(TRANSPARENT);

	bool checkLeftRight = true;
	bool checkUpDown = true;
	int nArrowHight = 20;
	int nArrowLength = 11;
	for(int i = nRealStart; i <= nRealEnd; i++){
		if(dataPos == NULL || pointPos == NULL)
			break;
		//volumnData�� �ش��ϴ� �׷��� ������ ����Ʈ�� ������ �´�.
		CString data = dataPacket->GetstrDataList()->GetNext(dataPos); 
		double PointData = CDraw::CalLogValue(bLog, pointPacket->GetnumericDataList()->GetNext(pointPos));
		CPoint dataPt = CDataConversion::GetVerticalPoint(realRegion, (i - nRealStart), dRealWidth, realDataMin, realDataMax, PointData);
		if(bInvert)
			dataPt.y = CDataConversion::InvertGraphDataPt(realRegion, dataPt.y);
		//draw Ÿ�Կ� ���� �׷��ش�.
		CSize strDataSize = pDC->GetTextExtent(data);
		if(!data.IsEmpty()){
			int nArrowPos_x = nArrowLength + MARGIN;
			int nArrowPos_y = nArrowHight + 1;
			if(bHighLow){
				checkLeftRight = true;
				if((dataPt.x + strDataSize.cx + nArrowPos_x) >= realRegion.right){
					if(bInvert)
						pDC->TextOut(dataPt.x - (strDataSize.cx + nArrowPos_x), dataPt.y + nArrowPos_y, data);
			else
						pDC->TextOut(dataPt.x -(strDataSize.cx + nArrowPos_x), dataPt.y - (strDataSize.cy / 2 + nArrowPos_y), data);
		}
			else{
					checkLeftRight = false;
					if(bInvert)
						pDC->TextOut(dataPt.x + nArrowPos_x, dataPt.y + nArrowPos_y, data);
					else
						pDC->TextOut(dataPt.x + nArrowPos_x, dataPt.y - (strDataSize.cy / 2 + nArrowPos_y), data);
					if(bInvert)
						checkUpDown = false;
			}
				DrawArrow(pDC, dataPt, checkLeftRight, checkUpDown, strDataSize, nArrowLength, nArrowHight);
				}
				else{
				checkUpDown = false;
				checkLeftRight = true;
				if((dataPt.x - (strDataSize.cx + nArrowPos_x)) <= realRegion.left){
					checkLeftRight = false;
					if(bInvert)
						pDC->TextOut(dataPt.x + nArrowPos_x, dataPt.y - (strDataSize.cy / 2 + nArrowPos_y), data);
					else
						pDC->TextOut(dataPt.x + nArrowPos_x, dataPt.y + nArrowPos_y, data);
				}
				else{
					if(bInvert)
						pDC->TextOut(dataPt.x - (strDataSize.cx + nArrowPos_x), dataPt.y - (strDataSize.cy / 2 + nArrowPos_y), data);
					else
						pDC->TextOut(dataPt.x - (strDataSize.cx + nArrowPos_x), dataPt.y + nArrowPos_y, data);
					if(bInvert)
						checkUpDown = true;
			}
				DrawArrow(pDC, dataPt, checkLeftRight, checkUpDown, strDataSize, nArrowLength, nArrowHight);
		}
	}
	}

	pDC->SetBkMode(MODE);
	pDC->SetTextColor(clr);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	Pen1.DeleteObject();
	Brush1.DeleteObject();
	return true;
}*/

bool CDataViewDraw::DrawSignal_NoCharArrowSignal(CDC* pDC, const CRect& realRegion, 
		const CGraphBaseData::DATASIGNALDRAWSTYLE drawStyle, const CGraphColor& graphColor, 
		const double dRealWidth, const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert, bool bUpDown)
{
	//������ �������� ���� ��ġ�� ������ �κ�======= 
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)
		return false;
	CPacket* dataPacket = packetList.GetNext(pos);
	if(pos == NULL)
		return false;
	CPacket* pointPacket = packetList.GetNext(pos); //�׷����� ��ġ�� ��Ÿ���� packet == indicator���� ������ �;� �Ѵ�.
	if(dataPacket == NULL || pointPacket == NULL)
		return false;
	//displayData
	//POSITION dataPos = dataPacket->GetnumericDataList()->FindIndex(nRealStart-1);
	//POSITION pointPos = pointPacket->GetnumericDataList()->FindIndex(nRealStart-1);
	POSITION dataPos = dataPacket->GetnumericDataList()->FindIndex(nRealStart);
	POSITION pointPos = pointPacket->GetnumericDataList()->FindIndex(nRealStart);
	if(dataPos == NULL || pointPos == NULL) 
		return false;

	CPen Pen1;
	Pen1.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
	CBrush Brush1 (graphColor.GetColor1());
	CPen* pOldPen = pDC->SelectObject(&Pen1);
	CBrush* pOldBrush = pDC->SelectObject(&Brush1);

	int nWidth = CMath::Round(dRealWidth/* 2/3*/);
	if(nWidth > 15)
		nWidth = 15;
	else if(nWidth < 10)
		nWidth = 10;

	//log�� ��� ==================================
	double realDataMax = CDraw::CalLogValue(bLog, dViewMax);
	double realDataMin = CDraw::CalLogValue(bLog, dViewMin);

	pDC->SelectObject(&Pen1);
	pDC->SelectObject(&Brush1);
	for(int i = nRealStart; i <= nRealEnd; i++){
		if(dataPos == NULL || pointPos == NULL)
			break;
		//volumnData�� �ش��ϴ� �׷��� ������ ����Ʈ�� ������ �´�.
		double data = dataPacket->GetnumericDataList()->GetNext(dataPos); 
		double PointData = CDraw::CalLogValue(bLog, pointPacket->GetnumericDataList()->GetNext(pointPos));
		CPoint dataPt = CDataConversion::GetVerticalPoint(realRegion, (i - nRealStart), dRealWidth, realDataMin, realDataMax, PointData);

		//draw Ÿ�Կ� ���� �׷��ش�.
		if(bInvert){
			dataPt.y = CDataConversion::InvertGraphDataPt(realRegion, dataPt.y);
			if(bUpDown)
				bUpDown = false;
			else
				bUpDown = true;
		}
		CPoint pPoint3[3];
		CPoint pPoint7[7];

		if(data > 0)
		{
			//Ÿ��üũ
			if(drawStyle == CGraphBaseData::Triangle_Signal){
				DrawTriangleType(pDC, pPoint3, dataPt, nWidth, realRegion, bUpDown);
				pDC->Polygon(pPoint3, 3);
	}
			else{
				DrawArrowType(pDC, pPoint7, dataPt, nWidth, realRegion, bUpDown);
				pDC->Polygon(pPoint7, 7);
}

			if(data > 1){
				dataPt.y = dataPt.y +  nWidth;
				//Ÿ��üũ
				if(drawStyle == CGraphBaseData::Triangle_Signal){
					DrawTriangleType(pDC, pPoint3, dataPt, nWidth, realRegion, bUpDown);
					pDC->Polygon(pPoint3, 3);
	}
	else{
					;
	}

	}
	}
}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	Pen1.DeleteObject();
	Brush1.DeleteObject();
	return true;
}

// --------------------------------------------------------------------------------------
bool CDataViewDraw::DrawCompartText(CDC* pDC, const CRect& realRegion, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double dRealWidth, const CList<CPacket*, CPacket*>& packetList)
{
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)
		return false;
	CPacket* dataPacket = packetList.GetNext(pos);
	if(dataPacket == NULL)
		return false;
	if(!dataPacket->GetstrDataList())
		return false;
	//displayData
	//POSITION dataPos = dataPacket->GetstrDataList()->FindIndex(nRealStart-1);
	POSITION dataPos = dataPacket->GetstrDataList()->FindIndex(nRealStart);
	if(dataPos == NULL) 
		return false;

	CPen Pen;
	Pen.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
	CPen* pOldPen = pDC->SelectObject(&Pen);

	COLORREF clr = pDC->SetTextColor(graphColor.GetColor1());
	int MODE = pDC->SetBkMode(TRANSPARENT);
	
	int ViewMin = 0;
	int ViewMax = 10;
	int blankSize = 10;
	int textCount = 0;
	for(int i = nRealStart; i <= nRealEnd; i++){
		if(dataPos == NULL)
			break;
		CString data = dataPacket->GetstrDataList()->GetNext(dataPos);
		CPoint dataPt = CDataConversion::GetVerticalPoint(realRegion, (i - nRealStart), dRealWidth, ViewMin, ViewMax, 5);
		if(!data.IsEmpty()){
			CSize strDataSize = pDC->GetTextExtent(data);
			int drawPosition = strDataSize.cy * textCount;
			if(realRegion.top/* - strDataSize.cy/2*/ + strDataSize.cy * textCount > realRegion.bottom)
				textCount = 0;

			CDraw::DrawLine(pDC, CPoint(dataPt.x, realRegion.top/* - strDataSize.cy/2*/ + drawPosition), dataPt.x, realRegion.bottom);
			CDraw::DrawLine(pDC, CPoint(dataPt.x, realRegion.top/* - strDataSize.cy/2*/ + drawPosition), dataPt.x + strDataSize.cy/2, realRegion.top/* - strDataSize.cy/2*/ + drawPosition);
			pDC->ExtTextOut(dataPt.x + blankSize, realRegion.top/* - strDataSize.cy*/ + drawPosition, /*NULL*/ETO_CLIPPED, realRegion, data, data.GetLength(), NULL);
			textCount++;
	}
	}
	pDC->SetBkMode(MODE);
	pDC->SetTextColor(clr);
	pDC->SelectObject(pOldPen);
	Pen.DeleteObject();
	return true;
	}

void CDataViewDraw::DrawTriangleType(CDC *pDC, CPoint* pPoint, CPoint Point, int width, CRect realRegion, bool checkUpDown)
{
	int height = width;
	int tempHeight = pDC->GetTextExtent("0").cy;
	
	if(checkUpDown){
		Point.y = Point.y - 4;
		if(Point.y - width < realRegion.top - tempHeight)
			height = realRegion.top - tempHeight- Point.y;
	}
	else{
		Point.y = Point.y + 4;
		if(Point.y + width > realRegion.bottom + tempHeight)
			height = realRegion.bottom + tempHeight - Point.y;
	}

	//CPoint pPoint[3];
	pPoint[0] = pPoint[1]= pPoint[2] = Point;
	if(checkUpDown){
		pPoint[1].y -= height;
		pPoint[2].y -= height;
		pPoint[1].x = pPoint[0].x - width;
		pPoint[2].x = pPoint[0].x + width;
	}
	else{
		pPoint[1].y += height;
		pPoint[2].y += height;
		pPoint[1].x = pPoint[0].x - width;
		pPoint[2].x = pPoint[0].x + width;
	}
}

/*void CDataViewDraw::DrawCircleType(CDC *pDC, CPoint* pPoint, CPoint Point, int width, CRect realRegion, bool checkUpDown)
{
}*/

/* 
// signal �� ũ�� ����
void CDataViewDraw::DrawArrowType(CDC *pDC, CPoint* pPoint, CPoint Point, int width, CRect realRegion, bool checkUpDown)
{
//	int height = width;
	int height = 8;
	int tempHeight = pDC->GetTextExtent("0").cy;
	
	if(checkUpDown){
		Point.y = Point.y - 4;
		//if(Point.y - width*2 < realRegion.top - tempHeight)
		//	height = realRegion.top - tempHeight- Point.y;
	}
	else{
		Point.y = Point.y + 4;
		//if(Point.y + width*2 > realRegion.bottom + tempHeight)
		//	height = realRegion.bottom + tempHeight - Point.y;
	}
	height  = height/2;
	if(height < 1)
		height = 1;

	//CPoint pPoint[7];
	pPoint[0] = Point; 
	if(checkUpDown){
		pPoint[5].y = pPoint[2].y = pPoint[6].y = pPoint[1].y = pPoint[0].y - height;
		pPoint[4].y = pPoint[3].y = pPoint[0].y - height*2;	
}
	else{
		pPoint[1].y = pPoint[2].y = pPoint[5].y = pPoint[6].y = pPoint[0].y + height;
		pPoint[3].y = pPoint[4].y = pPoint[0].y + height*2;
	}

	pPoint[1].x = pPoint[0].x - 3;
	pPoint[2].x = pPoint[3].x = pPoint[0].x - 1;
	pPoint[4].x = pPoint[5].x = pPoint[0].x + 1;
	pPoint[6].x = pPoint[0].x + 3;
	
}*/

// signal �� ũ�� ����
void CDataViewDraw::DrawArrowType(CDC *pDC, CPoint* pPoint, CPoint Point, int width, CRect realRegion, bool checkUpDown)
{
	if (checkUpDown)
		Point.y = Point.y - 4;
	else
		Point.y = Point.y + 4;

	int height = 8;
	if (width < 4)
		width = 4;

	// (2004.02.11, ��¿�) ȭ��ǥ�� ���� �������� ���δ�.
	//		Ư�� ���̴� ������ ũ�� �ʵ��� �Ѵ�.
	if( width < height) height = width;

	pPoint[0] = Point; 
	if(checkUpDown){
		pPoint[5].y = pPoint[2].y = pPoint[6].y = pPoint[1].y = pPoint[0].y - height;
		pPoint[4].y = pPoint[3].y = pPoint[0].y - height*2;	
	}
	else{
		pPoint[1].y = pPoint[2].y = pPoint[5].y = pPoint[6].y = pPoint[0].y + height;
		pPoint[3].y = pPoint[4].y = pPoint[0].y + height*2;
	}

	pPoint[1].x = pPoint[0].x - (width / 2);
	pPoint[2].x = pPoint[3].x = pPoint[0].x - (width / 4);
	pPoint[4].x = pPoint[5].x = pPoint[0].x + (width / 4);
	pPoint[6].x = pPoint[0].x + (width / 2);
}