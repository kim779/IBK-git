// SpecialScaleDrawer.cpp: implementation of the CSpecialScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpecialScaleDrawer.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/Dll_Load/IndicatorInfo.h"		// for CIndicatorInfo
#include "ScaleRegion.h"
#include "BaseData.h"
#include "Conversion.h"
#include "DataMath.h"
#include "PacketList.h"										// packet.dll
#include "PacketBase.h"										// packet.dll

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CSaleScaleDrawer

CSaleScaleDrawer::CSaleScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CPriceScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

// scale text, grid�� �׸���.
void CSaleScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale)
{
	// (2008/4/28 - Seung-Won, Bae) Show Price for Only One Scale Area
	//	if(scaleRegion.GetScaleTextRegion().left > drawingVertScaleData.GetDrawingGraphRegion().left)
	//		DrawTextOrGrid_Volumn(pDC, drawingVertScaleData, scaleRegion.GetScaleTextRegion(), vertScale);
	//	else if(scaleRegion.GetScaleTextRegion().left < drawingVertScaleData.GetDrawingGraphRegion().left)
	DrawTextOrGrid_Price(pDC, drawingVertScaleData, scaleRegion, vertScale);
}

// ����
void CSaleScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale)
{
	DrawTextOrGrid_Price(pDC, drawingVertScaleData, CScaleDrawingRegion(vertScaleRegion.GetLeftScaleTextRegion(), vertScaleRegion.GetLeftScaleGridRegion()), vertScale);
	DrawTextOrGrid_Volumn(pDC, drawingVertScaleData, vertScaleRegion.GetRightScaleTextRegion(), vertScale);
}

// private ==============================================================================
// "���Ź�" scale --> packet�� data�� %�� ���
void CSaleScaleDrawer::DrawTextOrGrid_Volumn(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CRect& scaleTextRegion, const CVerticalScale& vertScale)
{
	int nTotalCount = 0;
	double dOnePtWidth = 0.0, dTotalData = 0.0;
	if(!GetSaleData(drawingVertScaleData.GetPacket(), scaleTextRegion.Height(), nTotalCount, dOnePtWidth, dTotalData))
		return;

	CList<double, double>* pDataList = drawingVertScaleData.GetPacket()->GetnumericDataList();
	//sy 2002.7.29.
	if(pDataList == NULL || pDataList->GetCount() <= 0 || nTotalCount <= 0)
		return;

	//sy 2002.9.17. -> ���Ź�. ���簡/�ִ밪 index �����ϴ°� ����
	POSITION pos = pDataList->GetHeadPosition();
	int nIndex = 0;
	while(pos != NULL){
		int nowPt_y = scaleTextRegion.top + CMath::Round(dOnePtWidth *(double)nIndex +dOnePtWidth/2.0);
		double dPercentData = GetPercentData(dTotalData, pDataList->GetNext(pos));

		// (2008/6/13 - Seung-Won, Bae) for Support BuySell Overhanging Supply.
		if( 0 != dPercentData)
		{
			CString strData = CDataConversion::DoubleToString(dPercentData, 2) + "%";
			CDrawer::DrawTextOut(pDC, CVertScaleDrawer::GetTextPosition(scaleTextRegion.right, nowPt_y + 3, pDC->GetTextExtent(strData)),
				strData, drawingVertScaleData.GetScaleColor().GetScaleTextColor());
		}

		if(++nIndex == nTotalCount)
			break;
	}
}

// "���ݴ�" scale
// (2008/6/12 - Seung-Won, Bae) Support scale with bar count option.
void CSaleScaleDrawer::DrawTextOrGrid_Price(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale2)
{
	CPacket *pPacket = drawingVertScaleData.GetPacket();
	if( !pPacket) return;
	CPacketList *pPacketList = pPacket->GetPacketList();
	if( !pPacketList) return;
	pPacket = pPacketList->GetBaseData( _MTEXT( C0_BUY_SELL_OVERHANGING_SUPPLY_PRICE_BAND));
	if( !pPacket) return;
	CList < double, double> *pPriceList = pPacket->GetnumericDataList();
	if( !pPriceList) return;

	int nDataCount = pPriceList->GetCount();
	if( nDataCount <= 0) return;

	CIndicatorInfo *pIndicatorInfo = drawingVertScaleData.GetIndicatorInfo();
	if( !pIndicatorInfo) return;
	CList< double, double> *pConditionList = pIndicatorInfo->GetCommonCalConditionList();
	if( !pConditionList) return;
	if( pConditionList->GetCount() < 2) return;

	CDisplayDataMinMax ddMinMax = drawingVertScaleData.GetDisplayDataMinMax();
	CString dataType = CDrawer::GetPacketDataType( pPacket);
	// (2007/2/21 - Seung-Won, Bae) x 10.. Type's ORG Type is only "x 1"
	if( dataType.Find( '.') < 0) dataType = "x 1";
	int decimalDownCount = CDataConversion::GetDecimalDownCount( dataType);
	long lDifference = long( ( ddMinMax.GetDisplayMax() * pow( 10, decimalDownCount)) - long( ddMinMax.GetDisplayMin() * pow( 10, decimalDownCount)) );
	int nBarCount = int( pConditionList->GetHead());
	if( nBarCount < 1) nBarCount = 1;
	long lMod = lDifference % nBarCount;
	if( pConditionList->GetTail() == 0 && ( lMod != 0 || lDifference == 0))
	{
		if( dataType.Find( '.') < 0)	dataType = "x 0.1";
		else							dataType.Replace( "0.", "0.0");
		decimalDownCount = CDataConversion::GetDecimalDownCount( dataType);
	}
	// (2008/6/23 - Seung-Won, Bae) 
	pPacket = pPacketList->GetBaseData( _MTEXT( C0_BUY_SELL_OVERHANGING_SUPPLY));
	if( pPacket) pPacket->SetScaleType( dataType);

	CString strFormatTrans = CVertScaleDrawer::GetFormatTrans( ddMinMax, dataType, FALSE);

	// ���� �� �ڸ��� ���ϱ�
	int nRoundLength = 0;
	if(!strFormatTrans.IsEmpty() && strFormatTrans.Find(".") == -1)
	{
		//sy 2004.06.16.
		// �⺻ type �� "x10" �̻��� ��� round �� count�� ���� type ���� �⺻ type �� ����� �Ѵ�.
		// ��, �⺻ type = "x10" �̰� TR data �� "100" �� ���� ���� �ǹ��ϴ� ���� "1000" �̱� �����̴�.!!
		int nTemp = 0;
		if(!dataType.IsEmpty() && dataType.Find(".") == -1)
		{
			nTemp = CDataConversion::GetStringDataCount(dataType, "0");
		}
		nRoundLength = CDataConversion::GetStringDataCount(strFormatTrans, "0");
		nRoundLength -= nTemp;
		//sy end
	}

	CPen newPen;
	newPen.CreatePen(BS_SOLID, 1, drawingVertScaleData.GetCompartLineColor());
	CPen* pOldPen = pDC->SelectObject(&newPen);

	CRect rctTextRegion = scaleRegion.GetScaleTextRegion();
	bool bIsLog = drawingVertScaleData.GetVertScaleLogType();
	bool bIsInvert = drawingVertScaleData.GetVertScaleInvertType();
	bool bround = CVertScaleDrawer::GetBround( ddMinMax);
	int nPt_y = 0, nScaleLineCount = 5, nPeriodIndex = 0;
	double dPriceData = 0.0, dPrevPrice = 0.0, dSubData = 0.0, dSubInc = DBL_MAX, dPrevSub = 0.0;
	double dGapData = 0.0, dFactor = pow( 10, decimalDownCount);
	CString strData;
	CSize textSize;

	COLORREF clrText = drawingVertScaleData.GetScaleColor().GetScaleTextColor();
	CVerticalScale vertScale(vertScale2);
	vertScale.SetVertUserHogaScaleType( 1);
	vertScale.SetVertScaleHoGaNumber( 1);

	POSITION psn = pPriceList->GetHeadPosition();
	while( psn)
	{
		dPriceData = pPriceList->GetNext( psn);
		dPriceData -= ( dPriceData * DBL_EPSILON);

//		strData = CVertScaleDrawer::GetStrData( dPriceData, strFormatTrans, bround, nRoundLength);
//		strData = CDataConversion::DoubleToString( dPriceData, CDataConversion::GetDecimalDownCount( strFormatTrans));
		dPriceData *= dFactor;
		if( dPriceData != int( dPriceData)) dPriceData = int( dPriceData) + 1;
		dPriceData /= dFactor;

		strData.Format("%f", dPriceData);
		nPeriodIndex = strData.Find( '.');
		if( 0 < decimalDownCount)		strData = strData.Left( nPeriodIndex + 1 + decimalDownCount);
		else if( 0 == decimalDownCount)	strData = strData.Left( nPeriodIndex);
		while( 3 < nPeriodIndex)
		{
			nPeriodIndex -= 3;
			if( isdigit( strData.GetAt( nPeriodIndex))) strData.Insert( nPeriodIndex, ',');
		}

		dGapData = ( dPrevPrice - dPriceData);
		if( 0 != dGapData)
			if( GetVertScaleAbsolutePosition_y( rctTextRegion, dPriceData, ddMinMax, bIsLog, nPt_y))
		{
			CVertScaleDrawer::DrawScaleLine( pDC, rctTextRegion, nPt_y, bIsInvert, (MARGIN * 2), MARGIN);

			textSize = pDC->GetTextExtent( strData);
			CVertScaleDrawer::DrawTextOut( pDC, CVertScaleDrawer::GetTextPosition( rctTextRegion.right, nPt_y, textSize), 
				strData, clrText);
			CVertScaleDrawer::DrawGrid( pDC, drawingVertScaleData, vertScale.GetGridType(), nPt_y, vertScale.GetActiveFlag());

//			CVertScaleDrawer::DrawScaleLine(pDC, scaleRegion.GetScaleTextRegion(), nScaleLineCount, drawingVertScaleData.GetDisplayDataMinMax(), packetData, dGap, bIsLog, bIsInvert, true);
			// orgY �� Min ���� �ٸ� ��� orgY ���� min�������� ���� ó��
			if( 0 != dPrevPrice)
			{
				if( dSubInc == DBL_MAX)
				{
					for( int nIndex = 0 ; nIndex < nScaleLineCount ; nIndex++)
					{
						dSubData = dPriceData + ( dGapData / nScaleLineCount) * nIndex;
						dSubData -= ( dSubData * DBL_EPSILON);
						dSubData *= dFactor;
						if( dSubData != int( dSubData)) dSubData = int( dSubData) + 1;
						dSubData /= dFactor;

						if( dPrevSub != dSubData) if( dSubData - dPrevSub < dSubInc) dSubInc = dSubData - dPrevSub;

						dPrevSub = dSubData;
					}
					if( 0 < dSubInc) for( dSubData = ddMinMax.GetDisplayMin() + dSubInc; dSubData < ddMinMax.GetDisplayMax(); dSubData += dSubInc)
					{
						nPt_y = 0;
						if( GetVertScaleAbsolutePosition_y( rctTextRegion, dSubData, ddMinMax, bIsLog, nPt_y))
							CVertScaleDrawer::DrawScaleLine(pDC, rctTextRegion, nPt_y, bIsInvert, (MARGIN * 2), (MARGIN * 3 / 2));
					}
				}
			}
		}

		dPrevPrice = dPriceData;
	}

	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

// --------------------------------------------------------------------------------------
bool CSaleScaleDrawer::GetSaleData(CPacket* pPacket, const int nHeight, int& nTotalCount, double& dOnePtWidth, double& dTotalData)
{
	if(pPacket == NULL)
		return false;

	CList<double, double>* pDataList = pPacket->GetnumericDataList();
	if(pDataList == NULL)
		return false;

	//sy 2002.9.17. -> ���Ź�. ���簡/�ִ밪 index �����ϴ°� ����
	nTotalCount = GetDataCount(pDataList);
	if(nTotalCount <= 0)
		return false;

	dOnePtWidth = CDrawer::GetOnePointWidth(nHeight, nTotalCount);
	dTotalData = GetTotalData(pDataList);

	return (dOnePtWidth > 0.0 && dTotalData > 0.0);
}

int CSaleScaleDrawer::GetDataCount(const CList<double, double>* pDataList) const
{
	//sy 2002.9.17. -> ���Ź�. ���簡/�ִ밪 index �����ϴ°� ����
	int nTotalCount = pDataList->GetCount();
	if(nTotalCount >= 2)
		nTotalCount -= 2;

	return nTotalCount;
}

double CSaleScaleDrawer::GetTotalData(const CList<double, double>* pDataList) const
{
	//sy 2002.9.17. -> ���Ź�. ���簡/�ִ밪 index �����ϴ°� ����
	if(pDataList == NULL || pDataList->GetCount() <= 2)
		return 0.0;

	double dTotalData = 0.0;
	POSITION pos = pDataList->GetHeadPosition();
	int nTotalCount = GetDataCount(pDataList);
	int nIndex = 0;
	while(pos != NULL){
		double dData = pDataList->GetNext(pos);
		if(dData < 0)
			dData = fabs(dData);
		dTotalData += dData;

		if(++nIndex == nTotalCount)
			break;
	}

	return dTotalData;
}

double CSaleScaleDrawer::GetPercentData(const double& dTotalData, const double& dData) const
{
	if(dTotalData == 0.0)
		return dData;

	return ((dData * 100.0) / dTotalData);
}


//////////////////////////////////////////////////////////////////////
// class CMarketProfileScaleDrawer

CMarketProfileVertScaleDrawer::CMarketProfileVertScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CPriceScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

// scale text, grid�� �׸���.
void CMarketProfileVertScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale)
{
	CPriceScaleDrawer::DrawTextOrGrid(pDC, drawingVertScaleData, scaleRegion, 
		vertScale, GetPacketType(drawingVertScaleData.GetPacket()));
}

// ����
void CMarketProfileVertScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale)
{
	CPriceScaleDrawer::DrawTextOrGrid_Both(pDC, drawingVertScaleData, vertScaleRegion, 
		vertScale, GetPacketType(drawingVertScaleData.GetPacket()));
}

// private ==============================================================================
CString CMarketProfileVertScaleDrawer::GetPacketType(CPacket* pPacket) const
{
	if(pPacket == NULL)
		return "x0.01";
	return pPacket->GetType();
}
