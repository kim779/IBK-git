// DrawingRegionCalculator.cpp: implementation of the CDrawingRegionCalculator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawingRegionCalculator.h"

#include <assert.h>
#include <locale.h>					// for setlocale()
#include <math.h>					// for pow()

#include "../Include_Chart/Dll_Load/PacketList.h"				// PacketList.dll
#include "../Include_Chart/Dll_Load/PacketBase.h"				// PacketList.dll
#include "../Include_Chart/Dll_Load/IndicatorInfo.h"			// IndicatorCondition.dll
#include "../Include_Chart/Dll_Load/GraphDrawer.h"				// GraphDraw.dll
#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "../Include_Chart/Conversion.h"
#include "../Include_Chart/BaseData.h"
#include "../Include_Chart/ScaleCalculator.h"
#include "../Include_Chart/PacketPointerList.h"
#include "CalculatorData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CDrawingRegionCalculator

// public =================================================================================
// �׸��� ����(graph, scale)�� ���Ѵ�.
CDrawingRegion CDrawingRegionCalculator::GetDrawingRegion(CDC* pDC,	const CDrawingRegionCalData& calData, BOOL p_bReverse) const
{
	CGraphRegionCalculator graphRegionCalculator;
	// (2009/1/20 - Seung-Won, Bae) Support Reverse.
	CGraphRegions graphRegions = graphRegionCalculator.GetGraphRegions(pDC, calData, p_bReverse);

	CDrawingRegion drawingRegion;
	drawingRegion.SetGraphRegions(graphRegions);

	CRect blockRegion = calData.GetBlockRegion();
	CRect graphFrameRegion = graphRegions.GetFrameRegion();
	CRect graphDrawingRegion = drawingRegion.GetGraphRegions().GetDrawingRegion();
	drawingRegion.SetHorzScaleRegion(
		GetTopScaleRegion(		blockRegion, graphDrawingRegion, graphFrameRegion), 
		GetBottomScaleRegion(	blockRegion, graphDrawingRegion, graphFrameRegion));
	drawingRegion.SetVertScaleRegion(GetLeftScaleRegion(blockRegion, graphFrameRegion, graphDrawingRegion, calData.GetVertScaleRegionWidth().GetLeftVertScaleRegionWidth()), 
		GetRightScaleRegion(blockRegion, graphFrameRegion, graphDrawingRegion, calData.GetVertScaleRegionWidth().GetRightVertScaleRegionWidth()));

	return drawingRegion;
}

// private ================================================================================
// scale ����
// ����
CRect CDrawingRegionCalculator::GetLeftScaleRegion(const CRect& blockRegion, const CRect& graphFrameRegion, const CRect& graphRegion, const int /*nWidth*/) const
{
	return CRect(blockRegion.left, graphRegion.top, graphFrameRegion.left, graphRegion.bottom);
}

// ������
CRect CDrawingRegionCalculator::GetRightScaleRegion(const CRect& blockRegion, const CRect& graphFrameRegion, const CRect& graphRegion, const int nWidth) const
{
	return CRect(graphFrameRegion.right, graphRegion.top, blockRegion.right, graphRegion.bottom);
}

// ����
CRect CDrawingRegionCalculator::GetTopScaleRegion(		const CRect& blockRegion, const CRect &graphDrawRegion, const CRect& graphFrameRegion) const
{
	return CRect( blockRegion.left, blockRegion.top, blockRegion.right, graphFrameRegion.top - (blockRegion.top == graphFrameRegion.top ? 0 : 1));
}

// �Ʒ���
CRect CDrawingRegionCalculator::GetBottomScaleRegion(	const CRect& blockRegion, const CRect &graphDrawRegion, const CRect& graphFrameRegion) const
{
	return CRect(blockRegion.left, graphFrameRegion.bottom + (blockRegion.bottom == graphFrameRegion.bottom ? 0 : 1), blockRegion.right, blockRegion.bottom);
}


//////////////////////////////////////////////////////////////////////////////////////////
// class CGraphRegionCalculator

// public ================================================================================
// graph ������.
CGraphRegions CGraphRegionCalculator::GetGraphRegions(CDC* pDC, const CDrawingRegionCalData& calData, BOOL p_bReverse) const
{
	CGraphRegions graphRegions;
	// (2009/1/20 - Seung-Won, Bae) Support Reverse.
	graphRegions.SetFrameRegion( GetGraphFrameRegion(pDC, calData), calData.GetGraphRegionFrameThickness(), p_bReverse);
	graphRegions.SetVertScale(calData.IsVertScale());
	graphRegions.SetTitle(calData.IsGraphTitleDraw());
	graphRegions.SetText(IsGraphTextDraw(graphRegions.GetNoFrameRegion(), calData));
	graphRegions.SetBlockMoveDeleteFlag( calData.GetBlockMoveDeleteFlag());

	CSize textSize = pDC->GetTextExtent("A");
	graphRegions.SetFontSize(textSize.cy + MARGIN);

	return graphRegions;
}


// private ===============================================================================
// graph frame�� ���Ե� ������ ���Ѵ�.
CRect CGraphRegionCalculator::GetGraphFrameRegion(CDC* pDC, const CDrawingRegionCalData& calData) const
{
	// frame ���� �� graph ����
	CRect maxGraphDrawingRegion = GetMaxGraphFrameRegion(calData.GetBlockRegion(), calData.GetHorzScaleRegionHeight(), calData.GetVertScaleRegionWidth());
	return maxGraphDrawingRegion;

// (2006/10/10 - Seung-Won, Bae) why? comment? why direct return?
/*
	maxGraphDrawingRegion.DeflateRect(calData.GetGraphRegionFrameThickness(), calData.GetGraphRegionFrameThickness());

	// ȭ�鿡 ���� data ���� �����Ͽ� ����� ���� graph drawing region
	CRect graphFrameRegion = GetRealGraphRegion(calData, maxGraphDrawingRegion);
	graphFrameRegion.InflateRect(calData.GetGraphRegionFrameThickness(), calData.GetGraphRegionFrameThickness());
	return graphFrameRegion;
*/
}

// block region���� scale region�� �� ���� --> �ִ� graph region(frame ����)
CRect CGraphRegionCalculator::GetMaxGraphFrameRegion(const CRect& blockRegion, const CHorzScaleRegionHeight& horzScaleRegionHeight, const CVertScaleRegionWidth& vertScaleRegionWidth) const
{
	return CRect(blockRegion.left + vertScaleRegionWidth.GetLeftVertScaleRegionWidth(), blockRegion.top + horzScaleRegionHeight.GetTopHorzScaleRegionHeight(),
		blockRegion.right - vertScaleRegionWidth.GetRightVertScaleRegionWidth(), blockRegion.bottom - horzScaleRegionHeight.GetBottomHorzScaleRegionHeight());
}

// ȭ�鿡 ���� data ���� �����Ͽ� ����� �������� graph drawing region
CRect CGraphRegionCalculator::GetRealGraphRegion(const CDrawingRegionCalData& calData, const CRect& maxGraphDrawingRegion) const
{
	const CVertScaleRegionWidth& vertScaleRegionWidth = calData.GetVertScaleRegionWidth();

	CScaleBaseData::VERTSCALEPOSITION vertScalePosition = GetVertScalePosition(vertScaleRegionWidth);
	int remainedSpaceWidth = GetRemainedSpaceWidth(maxGraphDrawingRegion.Width(), calData.GetDisplayAttributes().GetDataCountInBlock());

	CRect adjustedGraphDrawingRegion = maxGraphDrawingRegion;
	adjustedGraphDrawingRegion.left = GetLeft(vertScalePosition, maxGraphDrawingRegion.left,
			calData.GetMaxVertScaleRegionWidth(), vertScaleRegionWidth.GetLeftVertScaleRegionWidth(), remainedSpaceWidth);
	adjustedGraphDrawingRegion.right = GetRight(vertScalePosition, maxGraphDrawingRegion.right,
			calData.GetMaxVertScaleRegionWidth(), vertScaleRegionWidth.GetRightVertScaleRegionWidth(), remainedSpaceWidth);

	return adjustedGraphDrawingRegion;
}

// ---------------------------------------------------------------------------------------
// drawing graph region�� data ���� ���� �����ų�� ���� scale �� Ÿ���� �����´�.
CScaleBaseData::VERTSCALEPOSITION CGraphRegionCalculator::GetVertScalePosition(const CVertScaleRegionWidth& vertScaleRegionWidth) const
{
	// scale�� ���ʿ� ���� ���
	if(vertScaleRegionWidth.GetLeftVertScaleRegionWidth() > 1 && vertScaleRegionWidth.GetRightVertScaleRegionWidth() >= 1)
		return CScaleBaseData::VERT_BOTH;
	// scale�� ���ʿ��� ���� ���
	else if(vertScaleRegionWidth.GetLeftVertScaleRegionWidth() >= 1)
		return CScaleBaseData::VERT_LEFT;
	// scale�� �����ʿ��� ���� ���
	else if(vertScaleRegionWidth.GetRightVertScaleRegionWidth() >= 1)
		return CScaleBaseData::VERT_RIGHT;

	return CScaleBaseData::VERT_NONE;
}

// graph �������� ���� scale �������� �ѱ� ��(x���� ����)
int CGraphRegionCalculator::GetRemainedSpaceWidth(const int graphDrawingRegionWidth, const int viewDataCount) const
{
	if(viewDataCount <= 0 || (graphDrawingRegionWidth / viewDataCount) <= 1)
		return 0;

	return (graphDrawingRegionWidth - viewDataCount * (graphDrawingRegionWidth / viewDataCount));
}

// ---------------------------------------------------------------------------------------
// graph drawing region�� ���� ��ǥ�� ���Ѵ�. ==> space ����
int CGraphRegionCalculator::GetLeft(const CScaleBaseData::VERTSCALEPOSITION vertScalePosition, const int orgLeft, 
		const int maxVertScaleRegionWidth, const int leftVertScaleRegionWidth, const int remainedSpaceWidth) const
{
	if(vertScalePosition == CScaleBaseData::VERT_LEFT)
		return GetData(orgLeft, maxVertScaleRegionWidth, leftVertScaleRegionWidth, remainedSpaceWidth, 1);
	// ���ʿ� scale�� ���� ��쿣 ���� ���� �ش�. (����)
	else if(vertScalePosition == CScaleBaseData::VERT_BOTH)
		return GetData(orgLeft, maxVertScaleRegionWidth, leftVertScaleRegionWidth, remainedSpaceWidth/2, 1);

	return orgLeft;
}

// graph drawing region�� ������ ��ǥ�� ���Ѵ�. ==> space ����
int CGraphRegionCalculator::GetRight(const CScaleBaseData::VERTSCALEPOSITION vertScalePosition, const int orgRight, 
		const int maxVertScaleRegionWidth, const int rightVertScaleRegionWidth, const int remainedSpaceWidth) const
{
	if(vertScalePosition == CScaleBaseData::VERT_RIGHT)
		return GetData(orgRight, maxVertScaleRegionWidth, rightVertScaleRegionWidth, remainedSpaceWidth, -1);
	// ���ʿ� scale�� ���� ��쿣 ���� ���� �ش�. (����)
	else if(vertScalePosition == CScaleBaseData::VERT_BOTH)
		return GetData(orgRight, maxVertScaleRegionWidth, rightVertScaleRegionWidth, remainedSpaceWidth/2, -1);

	return orgRight;
}

// - max scale left region : max	- min scale left region : min
// - ������ ��ġ : org				- remained space : r
// scale position : left �� ���  => if((org+r) >= max) org+(max-min)
//									 else org+r
// scale position : right �� ��� => if((org+r) >= max) org-(max-min)
//									 else org-r
// scalePosition �� +1(scale position : left), -1(scale position : right) �̴�!
int CGraphRegionCalculator::GetData(const int orgData, const int maxVertScaleRegionWidth, const int vertScaleRegionWidth, const int remainedSpaceWidth, const int scalePosition) const
{
	if((vertScaleRegionWidth + remainedSpaceWidth) >= maxVertScaleRegionWidth)
		return (orgData + (maxVertScaleRegionWidth - vertScaleRegionWidth)*scalePosition);

	return (orgData + remainedSpaceWidth*scalePosition);
}

// ---------------------------------------------------------------------------------------
// graph text�� �� ���� ����
bool CGraphRegionCalculator::IsGraphTextDraw(const CRect& graphRegion, const CDrawingRegionCalData& calData) const
{
	if(!calData.IsGraphTextDrawCheck())
		return calData.IsGraphTextDraw();

	if(!calData.IsGraphTextDraw())
		return false;

	if(graphRegion.Width() < MARGIN * 30 || graphRegion.Height() < MARGIN * 15)
		return false;

	return (calData.GetDisplayAttributes().GetDataCountInBlock() > 2);
}


//////////////////////////////////////////////////////////////////////////////////////////
// class  CTextSizeCalculator

// public =================================================================================
// scale�� �������ϱ� ���� scale�� ǥ���� data�� ���� string�� size.
CSize CTextSizeCalculator::GetMaxHorzScaleTextSize(CDC* pDC, const CHorzScaleMaxWidthCalculatorData& calData) const
{
	if(calData.GetScaleCompart() == NULL || !IsTextSizeCalculator(calData.GetPacket()))
		return CSize(0, 0);

	// <�ڷ�����> �� ���
	if(calData.GetPacketName() == _MTEXT( C0_DATE_TIME))
		return GetMaxScaleTextSize_DataDate(pDC, calData.GetPacket(), *calData.GetScaleCompart());

	// <���� packet name> �� ���
	return GetMaxScaleTextSize_Packet(pDC, calData);
}

// ���� scale�� �ִ� text size
CSize CTextSizeCalculator::GetMaxVertScaleTextSize(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	if(!IsTextSizeCalculator(calData.GetSubGraphPacketList()))
		return CSize(0, 0);

	// packet�� data �� ���
	CSize dataSize = GetMaxScaleText_Data(pDC, calData);
	CSize rateSize = GetMaxScaleText_Rate(pDC, calData);
	if(dataSize.cx > rateSize.cx)
		return dataSize;
	return rateSize;
}


// private ================================================================================
// text size�� �������� ����
bool CTextSizeCalculator::IsTextSizeCalculator(CPacket* pPacket) const
{
	return (pPacket != NULL && pPacket->GetCount() > 0);
}

bool CTextSizeCalculator::IsTextSizeCalculator(CSubGraphPacketList* pSubGraphPacketList) const
{
	if(pSubGraphPacketList == NULL || pSubGraphPacketList->GetCount() <= 0)
		return false;

	return IsTextSizeCalculator(pSubGraphPacketList->GetHeadPacket());
}

// <���ؼ�> �� �ִ����� ����
bool CTextSizeCalculator::IsBaseLineInIndicateData(CIndicatorInfo* pIndicatorInfo) const
{
	if(pIndicatorInfo == NULL || pIndicatorInfo->GetBaseLineList() == NULL)
		return false;

	return (pIndicatorInfo->GetBaseLineList()->GetCount() > 0);
}

// <���ؼ�> �� �׸��� ���������� ����
// y�� ��ġ : "������" �Ǵ� "����" �� ���� ��� -> <Data> scale
//			  "����" �� ��� -> "����" : <���ؼ�>, "������" <Data> scale
bool CTextSizeCalculator::IsBaseLineInRegion(const CVertScaleMaxWidthCalculatorData& calData) const
{
	CScaleBaseData::VERTSCALEPOSITION eVertScalePos = calData.GetVerticalScale().GetVertScalePosition();
	if(eVertScalePos == CScaleBaseData::VERT_BOTH)
		return !calData.DoesRight();

	return false;
	/*if(!calData.DoesRight())
		return true;

	return (calData.DoesRight() && calData.GetVerticalScale().GetVertScalePosition() == CScaleBaseData::VERT_RIGHT);*/
}


// ----------------------------------------------------------------------------------------
// scale �� ���� data�� �ִ� ����
CSize CTextSizeCalculator::GetMaxScaleText_Data(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	// <�����> �� ���
	// (2007/2/16 - Seung-Won, Bae) It can be with ','(comma).
	if( !strncmp( _MTEXT( C6_PERCENT), calData.GetVerticalScale().GetScaleData(), 6))
		return pDC->GetTextExtent("100.00%");
	// <��ǥ Data> �� ���
	// (2007/2/16 - Seung-Won, Bae) It can be with ','(comma).
	else if( !strncmp( _MTEXT( C6_INDICATOR_DATA), calData.GetVerticalScale().GetScaleData(), 9))
		return GetMaxScaleTextSize_IndicateData(pDC, calData);

	// <packet> �� ���
	CPacket* pPacket = GetVertScalePacket(calData.GetSubGraphPacketList(), calData.GetVertScalePacketName());
	return GetMaxScaleTextSize_Packet(pDC, pPacket, calData);
}

// ������ scale �� ���� data rate text size.
CSize CTextSizeCalculator::GetMaxScaleText_Rate(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	if(!calData.DoesRight() || calData.GetCalcRiseFallRate_CurData() <= 0)
		return CSize(0, 0);

	CTextInRightScaleRegionDrawer textData(calData.GetGraphName(), calData.GetSubGraphPacketList(), 
		CRect(0, 0, 100, 100), calData.GetCalcRiseFallRate_CurData(), calData.GetDisplayAttributes().GetDataEndInBlock());

// vntsorl_20110609:[A00000259] ���ݴ��� ���� ���簡 ǥ�� ����
	CString strCurPrice, strDiffPrice, strPipPrice, strRiseFallRate;
	if(!CGraphDrawer().GetTextInRightScaleRegionDrawer(textData, strCurPrice, strDiffPrice, strPipPrice, strRiseFallRate))
		return CSize(0, 0);

// 	if(strCurPrice.GetLength() > strRiseFallRate.GetLength())
// 		return pDC->GetTextExtent(strCurPrice);
// 	return pDC->GetTextExtent(strRiseFallRate);

	CString strMaxLenValue(strCurPrice);
	if( strMaxLenValue.GetLength() < strDiffPrice.GetLength())
		strMaxLenValue = strDiffPrice;
	if( strMaxLenValue.GetLength() < strPipPrice.GetLength())
		strMaxLenValue = strPipPrice;
	if( strMaxLenValue.GetLength() < strRiseFallRate.GetLength())
		strMaxLenValue = strRiseFallRate;

	return pDC->GetTextExtent(strMaxLenValue);
}

/*CSize CTextSizeCalculator::GetMaxScaleText_Rate(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	if(!calData.DoesRight() || calData.GetCalcRiseFallRate_CurData() <= 0)
		return CSize(0, 0);

	double dPrevPrice = 0.0, dCurPrice = 0.0;
	if(!GetPrices(calData, dPrevPrice, dCurPrice))
		return CSize(0, 0);

	CString strCurPrice = GetCurrentPriceText(dCurPrice);
	CString strRiseFallRate = GetRateText(dPrevPrice, dCurPrice);
	if(strCurPrice.GetLength() > strRiseFallRate.GetLength())
		return pDC->GetTextExtent(strCurPrice);
	return pDC->GetTextExtent(strRiseFallRate);
}*/

// ----------------------------------------------------------------------------------------
// <�ڷ�����>�� ���.
CSize CTextSizeCalculator::GetMaxScaleTextSize_DataDate(CDC* pDC, CPacket* pPacket, const CScaleCompart& scaleCompart) const
{
	if(pPacket == NULL)
		return CSize(0, 0);

	CString headPacketData, dataType;
	if(GetDatePacketHeadDataAndType_DataDate(pPacket, headPacketData, dataType) == false)
		return CSize(0, 0);

	CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType = (CScaleBaseData::HORZSCALEDRAWERTYPE) pPacket->GetDateUnitType();

	CString strData = CScaleCalculator( m_hOcxWnd, m_eChartMode).GetCompartScaleData(scaleDrawerType, headPacketData, dataType, scaleCompart);
	if(strData.IsEmpty())
		return CSize(0, 0);

	return GetMaxScaleTextSize(CScaleCalculator::GetTextSize(pDC, strData), strData.GetLength(), scaleCompart.GetScaleTextType());
}

// ----------------------------------------------------------------------------
// <��ǥ data>�� ���.
CSize CTextSizeCalculator::GetMaxScaleTextSize_IndicateData(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	// (2008/6/13 - Seung-Won, Bae) for Support BuySell Overhanging Supply.
	if( calData.GetGraphName() == _MTEXT( C2_BUY_SELL_OVERHANGING_SUPPLY))
		return GetMaxScaleTextSize_Sale2(pDC, calData);
	// <���Ź�> �� ���
	if( g_iMetaTable.IsVolumeForSale(calData.GetGraphName()))
		return GetMaxScaleTextSize_Sale(pDC, calData);
	// <������Ʈ> �� ���
	if( calData.GetGraphName() == _MTEXT( C2_COMPARISON_CHART))
		return pDC->GetTextExtent("100%");
	// <���ؼ��� �ִ� ��ǥ Data> �� ���
	if(IsBaseLineInIndicateData(calData.GetIndicatorInfo()))
		return GetMaxScaleTextSize_BaseLine(pDC, calData);

	// <packet> �� ���
	return GetMaxScaleTextSize_Packet(pDC, calData);
}

// -> <���Ź�>�� ���.
CSize CTextSizeCalculator::GetMaxScaleTextSize_Sale(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	// <���Ź�>�� Ư�����϶��� "����:%, ������:���ݴ�" ������
	// ÷�������� �����ڿ��� "������ ���ݴ�"�� �����Ǿ���,
	// ��, ������Ʈ�� ���� ���� �� �Լ��� ȣ������ �ʴ´�.

	/*// <���Ź�> - ����(�ִ� 99.99%)
	if(!calData.DoesRight())
		return GetMaxScaleTextSize(pDC, 6);*/

	// <ü�ᰡ> - ������(����)
	if(calData.GetSubGraphPacketList() == NULL)
		return CSize(0, 0);

	CPacket* pPacket = calData.GetSubGraphPacketList()->GetPacket(1);
	if(pPacket == NULL)
		return CSize(0, 0);

	return GetMaxScaleTextSize_Packet(pDC, pPacket, calData);
}

// -> <���ؼ�>�� ���.
CSize CTextSizeCalculator::GetMaxScaleTextSize_BaseLine(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	// <���ؼ�> - ������ ��� ���ʿ���
	if(IsBaseLineInRegion(calData)){
		return GetMaxScaleTextSize(pDC, GetLongLength(calData.GetIndicatorInfo()));
	}

	// <�� (Data)> - ������
	// ( + ) percent ������ ��� - "�ɸ���ǥ", "RSI", "Stochastics"
	if( g_iMetaTable.IsScaleGroup_PlusPercent(calData.GetGraphName()))
		return GetMaxScaleTextSize(pDC, 4);
	// (-) percent ������ ��� - "William's %R"
	else if( g_iMetaTable.IsScaleGroup_MinusPercent(calData.GetGraphName()))
		return GetMaxScaleTextSize(pDC, 5);

	return GetMaxScaleTextSize_Packet(pDC, calData);
}

// ----------------------------------------------------------------------------
// <packet>�� ���.
CSize CTextSizeCalculator::GetMaxScaleTextSize_Packet(CDC* pDC, const CHorzScaleMaxWidthCalculatorData& calData) const
{
	int nLength = GetDrawingDataLength( calData.GetPacket(), calData.GetDisplayAttributes(), CScaleBaseData::VERT_NUM);
	return GetMaxScaleTextSize(pDC, nLength, calData.GetScaleCompart()->GetScaleTextType());
}

CSize CTextSizeCalculator::GetMaxScaleTextSize_Packet(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	int nMaxLength = GetMaxPacketDataLength(calData.GetGraphName(), calData.GetSubGraphPacketList(), 
		calData.GetDisplayAttributes(), calData.GetVerticalScale().GetVertScaleUnitType(),
		calData.GetSubGraphDisplayMinMax());

	return GetMaxScaleTextSize(pDC, nMaxLength);
}

CSize CTextSizeCalculator::GetMaxScaleTextSize_Packet(CDC* pDC, CPacket* pPacket, const CVertScaleMaxWidthCalculatorData& calData) const
{
	int nMaxLength = GetDrawingDataLength(pPacket, calData.GetDisplayAttributes(), calData.GetVerticalScale().GetVertScaleUnitType());
	return GetMaxScaleTextSize(pDC, nMaxLength);
}

// ----------------------------------------------------------------------------
// packet���� min/max ��������.
bool CTextSizeCalculator::GetPacketMinMax(CPacket* pPacket, const CDisplayAttributes& displayAttributes, double& dMin, double& dMax) const
{
	if(pPacket == NULL || pPacket->GetCount() <= 0)
		return false;

	int nDataEndInBlock = pPacket->GetnumericDataList()->GetCount() -1;
	if(nDataEndInBlock > displayAttributes.GetDataEndInBlock())
		nDataEndInBlock = displayAttributes.GetDataEndInBlock();

	if(pPacket->GetMinMax(displayAttributes.GetDataStartInBlock(), nDataEndInBlock, dMin, dMax) < 0)
		return false;
	return true;
}

// ----------------------------------------------------------------------------
// ���� �� ���� ��������
int CTextSizeCalculator::GetLongLength(CIndicatorInfo* pIndicatorInfo) const
{
	assert(pIndicatorInfo != NULL);
	if(pIndicatorInfo->GetBaseLineList() == NULL)
		return 0;

	int nLength = 0;
	POSITION pos = pIndicatorInfo->GetBaseLineList()->GetHeadPosition();
	while(pos != NULL){
		double dData = pIndicatorInfo->GetBaseLineList()->GetNext(pos);
		int temp = CDataConversion::DoubleToString(dData).GetLength();
		if(nLength < temp)
			nLength = temp;
	}

	return nLength;
}

// ----------------------------------------------------------------------------
int CTextSizeCalculator::GetMaxPacketDataLength( const CString& strGraphName, CSubGraphPacketList* pSubGraphPacketList,
												const CDisplayAttributes& displayAttributes, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType,
												const CDisplayDataMinMax &p_displaySubGraphMinMax) const
{
	if(pSubGraphPacketList == NULL)
		return 0;

	int nMaxLength = 0;
	POSITION pos = GetDrawingDataLengthStartPosition(strGraphName, pSubGraphPacketList);
	while(pos != NULL)
	{
		CPacket* pPacket = pSubGraphPacketList->GetNext(pos);
		// packe name �� "�ڷ�����" �� ���ԵǾ� ������ ������ �ʴ´�.
		if(pPacket == NULL || pPacket->GetName().Find( _MTEXT( C0_DATE_TIME)) != -1)
			continue;

		int nLength = GetDrawingDataLength_WithMinMax(pPacket, displayAttributes, eVertScaleUnitType, p_displaySubGraphMinMax);
		if(nMaxLength < nLength)
			nMaxLength = nLength;
	}

	return nMaxLength;
}

POSITION CTextSizeCalculator::GetDrawingDataLengthStartPosition(const CString& strGraphName, CSubGraphPacketList* pSubGraphPacketList) const
{
	if(pSubGraphPacketList == NULL)
		return NULL;

	if( g_iMetaTable.IsViewVertMinMax_EndPacket(strGraphName))
		return pSubGraphPacketList->GetTailPosition();
	return pSubGraphPacketList->GetHeadPosition();
}

int CTextSizeCalculator::GetDrawingDataLength(CPacket* pPacket, const CDisplayAttributes& displayAttributes,
											  const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType) const
{
	if(pPacket == NULL)
		return 0;

	// <����> �� ���
	if( pPacket->GetType() == _MTEXT( C6_CHARACTER))
//////////////////////////////////////////////////////////////////////////
// 2006.12.20 �������� �� �Ʒ��� ���� trim
// �������� �� �Ʒ��� ���� trim�� ���� �Ʒ��� ���� ����
// 		return pPacket->GetLength();-->
		return GetDrawingDataLength_Text(pPacket);
//////////////////////////////////////////////////////////////////////////

	// <����> �� ���
	return GetDrawingDataLength_Number(pPacket, displayAttributes, eVertScaleUnitType);
}

int CTextSizeCalculator::GetDrawingDataLength_WithMinMax(CPacket* pPacket, const CDisplayAttributes& displayAttributes,
											  const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType,
												const CDisplayDataMinMax &p_displaySubGraphMinMax) const
{
	if(pPacket == NULL)
		return 0;

	// <����> �� ���
	if( pPacket->GetType() == _MTEXT( C6_CHARACTER))
//////////////////////////////////////////////////////////////////////////
// 2006.12.20 �������� �� �Ʒ��� ���� trim
// �������� �� �Ʒ��� ���� trim�� ���� �Ʒ��� ���� ����
// 		return pPacket->GetLength();-->
		return GetDrawingDataLength_Text(pPacket);
//////////////////////////////////////////////////////////////////////////

	// <����> �� ���
	return GetDrawingDataLength_Number_WithMinMax(pPacket, displayAttributes, eVertScaleUnitType, p_displaySubGraphMinMax);
}

int CTextSizeCalculator::GetDrawingDataLength_Text(CPacket* pPacket) const
{
	int nMaxLength = 0;

	POSITION pos = pPacket->GetstrDataList()->GetHeadPosition();
	while(pos != NULL){
		CString strData = pPacket->GetstrDataList()->GetNext(pos);
		strData.TrimRight(" ");
		strData.TrimLeft(" ");
		
// 		if(strData.GetLength() > nMaxLength)
// 			nMaxLength = strData.GetLength();
		//-------------------------------------------------------->
		// 2007.1.8 ���� �Ʒ� �߸��� �� �ذ�
		// ���� �� ���  
		setlocale( LC_CTYPE, "Korean" );	// ������ �ѱ����� ����
		if(_mbstrlen(strData) > nMaxLength)	// _mbstrlen�� ��ȯ���� �ѱ��� ��� 2byte�� 1���ڷ� ���
			nMaxLength = _mbstrlen(strData);
		//<--------------------------------------------------------
	}

	return nMaxLength;
}

int CTextSizeCalculator::GetDrawingDataLength_Number( CPacket* pPacket, const CDisplayAttributes& displayAttributes,
													const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType) const
{
	CString strData, strFormatTrans;
	if(!GetDrawingData_Number(pPacket, displayAttributes, strData, strFormatTrans))
		return 0;

	// �Ҽ��� ������ ��쿡 scale���� ���翩��
	int nNumLength = GetDataLength(strData);
	int nFormatLength = GetFormatLength(strFormatTrans, eVertScaleUnitType);

	return ((nNumLength > nFormatLength) ? nNumLength: nFormatLength);
}

int CTextSizeCalculator::GetDrawingDataLength_Number_WithMinMax( CPacket* pPacket, const CDisplayAttributes& displayAttributes,
													const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType,
													const CDisplayDataMinMax &p_displaySubGraphMinMax) const
{
	CString strData, strFormatTrans;
	if(!GetDrawingData_Number_WithMinMax(pPacket, displayAttributes, strData, strFormatTrans, p_displaySubGraphMinMax))
		return 0;

	// �Ҽ��� ������ ��쿡 scale���� ���翩��
	int nNumLength = GetDataLength(strData);
	int nFormatLength = GetFormatLength(strFormatTrans, eVertScaleUnitType);

	return ((nNumLength > nFormatLength) ? nNumLength: nFormatLength);
}

bool CTextSizeCalculator::GetDrawingData_Number( CPacket* pPacket, const CDisplayAttributes& displayAttributes, CString& strData,
												CString& strFormatTrans) const
{
	double dMin = 0, dMax = 0;
	if(!GetPacketMinMax(pPacket, displayAttributes, dMin, dMax))
		return 0;
	
	CString dataType = pPacket->GetType(false);
	double dMaxLengthData = CDataConversion::GetLongLengthData(dMin, dMax, dataType);
	BOOL bAutoScaleType = pPacket->IsAutoScaleType();
	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTransCutoff(dMaxLengthData, dataType, strData, strFormatTrans, bAutoScaleType);
	if( strData.IsEmpty()) return 0;
	strFormatTrans.Replace(" ", "");

	pPacket->SetScaleType( bAutoScaleType ? strFormatTrans : pPacket->GetType());
	return (!strData.IsEmpty() || !strFormatTrans.IsEmpty());
}

bool CTextSizeCalculator::GetDrawingData_Number_WithMinMax( CPacket* pPacket, const CDisplayAttributes& displayAttributes, CString& strData,
												CString& strFormatTrans, const CDisplayDataMinMax &p_displaySubGraphMinMax) const
{
	double dMin = 0, dMax = 0;
	// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
	//		It is initialized in CSubGraph::GetDisplayDataMinMax() for CMainBlockImp::CalculateDataForDrawing().
	//	if(!GetPacketMinMax(pPacket, displayAttributes, dMin, dMax))
	//		return 0;
	dMax = p_displaySubGraphMinMax.GetDisplayMax();
	dMin = p_displaySubGraphMinMax.GetDisplayMin();

	CString dataType = pPacket->GetType(false);
	double dMaxLengthData = CDataConversion::GetLongLengthData(dMin, dMax, dataType);
	BOOL bAutoScaleType = pPacket->IsAutoScaleType();
	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTransCutoff(dMaxLengthData, dataType, strData, strFormatTrans, bAutoScaleType);
	if( strData.IsEmpty()) return 0;
	strFormatTrans.Replace(" ", "");

	pPacket->SetScaleType( bAutoScaleType ? strFormatTrans : pPacket->GetType());
	return (!strData.IsEmpty() || !strFormatTrans.IsEmpty());
}

/*double CTextSizeCalculator::GetLongLengthData(const double& dData1, const double& dData2, const CString& dataType) const
{
	int nDecimalDownCount = CDataConversion::GetDecimalDownCount(dataType);
	CString strMin = CDataConversion::DoubleToString(dData1, nDecimalDownCount);
	CString strMax = CDataConversion::DoubleToString(dData2, nDecimalDownCount);

	if(strMin.GetLength() > strMax.GetLength())
		return dData1;

	return dData2;
}*/

int CTextSizeCalculator::GetDataLength(const CString& strData) const
{
	// ���� �Ҽ��� �̸� �״�� �Ҽ����� ǥ���Ѵ�.
	return strData.GetLength();

	// 000.00 ������ ��쿣 �Ҽ��� .00 �� ǥ��
	// 0,000.00 �϶��� 0,000 ���� ǥ��  -> CScaleDrawer
	if(strData.Find(".") == -1)
		return strData.GetLength();

	int nNumLength = strData.GetLength();
	CString strTemp = strData;
	strTemp.TrimLeft('-');
	if(strTemp.GetLength() > 7){
		strTemp = strData.Left(strData.Find("."));
		nNumLength = strTemp.GetLength();
	}
	return nNumLength;
}

/*int CTextSizeCalculator::GetDataLength(const CString& strData) const
{
	if(strData.Find(".") == -1)
		return strData.GetLength();

	CString strTemp = strData;
	strTemp.TrimLeft('-');
	int nNumLength = strTemp.GetLength();
	if(nNumLength > 7){
		strTemp = strData.Left(strData.Find("."));
		nNumLength = strTemp.GetLength();
	}
	return nNumLength;
}*/

int CTextSizeCalculator::GetFormatLength(const CString& strFormat, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType) const
{
	CString strTemp = strFormat;
	if(eVertScaleUnitType == CScaleBaseData::VERT_NUM_CHAR)
		strTemp = CDataConversion( m_hOcxWnd, m_eChartMode).GetChangeFormat_NumCharUnit(strFormat);

	int nFormatLength = strTemp.GetLength();
	if(strTemp.Find(".") != -1)	
		nFormatLength = 0;

	return nFormatLength;
}

// ---------------------------------------------------------------------------------------
// packet �� �����´�.
CPacket* CTextSizeCalculator::GetVertScalePacket(CSubGraphPacketList* pSubGraphPacketList, const CString& strPacketName) const
{
	if(pSubGraphPacketList == NULL || strPacketName.IsEmpty())
		return NULL;

	CPacket* pHeadPacket = pSubGraphPacketList->GetHeadPacket();
	if(pHeadPacket == NULL)
		return NULL;

	if(pHeadPacket->GetName() == strPacketName)
		return pHeadPacket;

	return GetVertScalePacket(pHeadPacket->GetPacketList(), strPacketName);
}

CPacket* CTextSizeCalculator::GetVertScalePacket(CPacketList* pPacketList, const CString& strPacketNames) const
{
	if(pPacketList == NULL || strPacketNames.IsEmpty())
		return NULL;

	CString strTemp = strPacketNames;
	if(strTemp.Right(1) != ',')
		strTemp += ',';

	while(!strTemp.IsEmpty())
	{
		CString strPacketName = CDataConversion::GetStringData(strTemp, ',');
		if(strPacketName.IsEmpty())
			continue;
		
		CPacket* pPacket = pPacketList->GetBaseData(strPacketName);
		if(pPacket != NULL && pPacket->GetCount() > 0)
			return pPacket;
	}

	return NULL;
}

// packet�� ó�� data�� type�� �����´�.
bool CTextSizeCalculator::GetDatePacketHeadDataAndType_DataDate(CPacket* pPacket, CString& headPacketData, CString& dataType) const
{
	if(pPacket == NULL)
		return false;

	CList<double, double>* dataList = pPacket->GetnumericDataList();
	if(dataList->GetHeadPosition() == NULL || dataList->GetCount() <= 0)
		return false;

	dataType = pPacket->GetType(false);
	headPacketData = CDataConversion::DoubleToString(dataList->GetHead(), 0);
	int nZeroCount = dataType.GetLength() - headPacketData.GetLength();
	for(int nCount = 0; nCount < nZeroCount; nCount++){
		headPacketData = "0" + headPacketData;
	}

	return true;
}

// ---------------------------------------------------------------------------------------
// text�� ������ ���� Size�� �����Ѵ�.
CSize CTextSizeCalculator::GetMaxScaleTextSize(CDC* pDC, const int nLength, const CScaleBaseData::TEXTCOMPARTTYPE textCompart) const
{
	if(nLength <= 0)
		return CSize(0, 0);

	return GetMaxScaleTextSize(pDC->GetTextExtent("0"), nLength, textCompart);
}

CSize CTextSizeCalculator::GetMaxScaleTextSize(const CSize& textSize, const int nLength, const CScaleBaseData::TEXTCOMPARTTYPE textCompart) const
{
	if(nLength <= 0) // ���� �������� ���� ���
		return CSize(0, 0);

	// text ���� type�� �������� ���
	if(textCompart == CScaleBaseData::VERT_TEXT_COMPART)
		return CSize(textSize.cx, textSize.cy*nLength);	// 2007.1.8 �Ʒ� ©���� ���� �ذ��� ���� (nLength+2)/2 --> nLength

	return CSize(textSize.cx*nLength, textSize.cy);
}

// ---------------------------------------------------------------------------------------
// "���簡" �� ȭ����� ����Ÿ!!
/*bool CTextSizeCalculator::GetPrices(const CVertScaleMaxWidthCalculatorData& calData, double& dPrevPrice, double& dCurPrice) const
{
	CPacket* pPacket = calData.GetSubGraphPacketList()->GetHeadPacket();
	if(pPacket == NULL || pPacket->GetnumericDataList() == NULL || pPacket->GetnumericDataList()->GetCount() <= 0)
		return false;

	POSITION pos = pPacket->GetnumericDataList()->FindIndex(calData.GetDisplayAttributes().GetDataEndInBlock() -1);
	if(pos == NULL)
		return false;

	dPrevPrice = pPacket->GetnumericDataList()->GetNext(pos);
	if(pos == NULL)
		return false;
	dCurPrice = pPacket->GetnumericDataList()->GetNext(pos);
	return true;
}

// ���簡
CString CTextSizeCalculator::GetCurrentPriceText(const double& dCurPrice) const
{
	CString strCurPrice = CDataConversion::DoubleToString(dCurPrice, 2);
	strCurPrice.TrimRight("0");
	strCurPrice.TrimRight(".");

	return CDataConversion::GetStringThousandFormat(strCurPrice);
}

// �������� ���� ����
CString CTextSizeCalculator::GetRateText(const double& dPrevPrice, const double& dCurPrice) const
{
	double dDataRate = 0.0;
	if(dPrevPrice != 0.0)
		dDataRate = (dCurPrice - dPrevPrice) / dPrevPrice * 100.0;

	return CDataConversion::DoubleToString(dDataRate, 2) + "%";
}*/


// (2008/6/13 - Seung-Won, Bae) for Support BuySell Overhanging Supply.
CSize CTextSizeCalculator::GetMaxScaleTextSize_Sale2(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const
{
	// 1. Support Price Band Scale.
	if( calData.IsDualScale() && calData.DoesRight())
		return GetMaxScaleTextSize(pDC, 6);				// <���Ź�> - ����(�ִ� 99.99%)

	// <ü�ᰡ> - ������(����)
	if(calData.GetSubGraphPacketList() == NULL) return CSize(0, 0);
	CPacket *pPacket = calData.GetSubGraphPacketList()->GetPacket( 1);
	if( pPacket == NULL) return CSize(0, 0);
	int nDataCount = pPacket->GetCount();
	if( nDataCount <= 0) return CSize(0, 0);

//	int nMaxLength = GetDrawingDataLength(pPacket, calData.GetDisplayAttributes(), calData.GetVerticalScale().GetVertScaleUnitType());
//	int nMaxLength = GetDrawingDataLength_Number(pPacket, displayAttributes, eVertScaleUnitType);
	int nMaxLength = 0;
	CString strData, strFormatTrans;
//	if( GetDrawingData_Number( pPacket, calData.GetDisplayAttributes(), strData, strFormatTrans))
	double dMin = 0, dMax = 0;
//	if( GetPacketMinMax(pPacket, calData.GetDisplayAttributes(), dMin, dMax))
	if( 0 < pPacket->GetMinMax( 0, nDataCount - 1, dMin, dMax))
	{
		CString dataType = pPacket->GetType(false);
		if( dataType.Find( '.') < 0) dataType = "x 1";
		double dMaxLengthData = CDataConversion::GetLongLengthData(dMin, dMax, dataType);

		int decimalDownCount = CDataConversion::GetDecimalDownCount( dataType);
		long lDifference = long( ( dMax * pow( 10, decimalDownCount)) - long( dMin * pow( 10, decimalDownCount)) );
		CIndicatorInfo *pIndicatorInfo = calData.GetIndicatorInfo();
		if( pIndicatorInfo)
		{
			CList < double, double> *pConditionList = pIndicatorInfo->GetCommonCalConditionList();
			int nBarCount = int( pConditionList->GetHead());
			if( nBarCount < 1) nBarCount = 1;
			long lMod = lDifference % nBarCount;
			if( pConditionList->GetTail() == 0 && ( lMod != 0 || lDifference == 0))
			{
				if( dataType.Find( '.') < 0)	dataType = "x 0.1";
				else							dataType.Replace( "0.", "0.0");
			}
		}

		CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTransCutoff(dMaxLengthData, dataType, strData, strFormatTrans, FALSE);
		if( !strData.IsEmpty())
		{
			strFormatTrans.Replace(" ", "");

			pPacket->SetScaleType( dataType);
			if( !strData.IsEmpty() || !strFormatTrans.IsEmpty())
			{
				// �Ҽ��� ������ ��쿡 scale���� ���翩��
				int nNumLength = GetDataLength(strData);
				int nFormatLength = GetFormatLength(strFormatTrans, calData.GetVerticalScale().GetVertScaleUnitType());
				nMaxLength = ((nNumLength > nFormatLength) ? nNumLength: nFormatLength);
			}
		}
	}

	return GetMaxScaleTextSize(pDC, nMaxLength);
}
