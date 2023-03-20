// GraphTitleDrawer.cpp: implementation of the CGraphTitleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphTitleDrawer.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTOL1()

#include "BaseData.h"
#include "GraphData.h"
#include "Conversion.h"
#include "DrawingPacketFinder.h"
#include "PacketPointerList.h"

#include "IndicatorList.h"
#include "IndicatorInfo.h"
#include "PacketBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CGraphTitleDrawer

CGraphTitleDrawer::CGraphTitleDrawer() :
	NODRAW_TITLEREGION("&"), // title == "&����" -> title ���� �ڽ��� �׸��� �ʴ´�.
	TITLE_COMPART(",")
{
}

// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
bool CGraphTitleDrawer::DrawGraphTitle( CDC* pDC, const CDrawingGraphData& drawingGraphData, 
			const CSubGraphData& subGraphData, int& nTitleEndPosition, int& nTitleEndLine)
{
	if(! IsGraphTitleDraw( subGraphData.GetTitle(), drawingGraphData.GetGraphRegions()))
		return false;

	// text ��°� �������� �׷������� ���ϱ�
	CString strAllTitle = GetGraphTitle(subGraphData, drawingGraphData.GetOcxHwnd());

	bool bResult = false;
	int nTitelIndex = 0;
	// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
	int nSubGraphIndex = subGraphData.GetSubGraphIndex();

	while(!strAllTitle.IsEmpty())
	{
		CString strTitle = CDataConversion::GetStringData(strAllTitle, TITLE_COMPART);
		if( strTitle.IsEmpty())
			continue;

		// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
		// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
		if(DrawGraphTitle(pDC, drawingGraphData, subGraphData, strTitle, nTitelIndex++, nTitleEndPosition, nTitleEndLine, nSubGraphIndex))
		{
			bResult = true;
			// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
			nSubGraphIndex ++;
		}
		//>> Ÿ��Ʋ ������ ǥ�ý� ������ ������ ������ �ʵ��� ó�� - vntsorl (20090106)
		else
		{
			bResult = false;
			break;
		}
		//<< Ÿ��Ʋ ������ ǥ�ý� ������ ������ ������ �ʵ��� ó�� - vntsorl (20090106)
	}

	return bResult;
}

// private ====================================================================
// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
bool CGraphTitleDrawer::DrawGraphTitle(CDC* pDC, const CDrawingGraphData& drawingGraphData, 
		const CSubGraphData& subGraphData, const CString& strTitle, const int nTitleIndex, int& nTitleEndPosition, int& nTitleEndLine, const int nSubGraphIndex)
{
	// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
	CString strRealTitle = GetGraphTitle(drawingGraphData, strTitle, nTitleIndex, nSubGraphIndex);
	if(strRealTitle.IsEmpty())
		return false;

	// title�� ����� ���� ���� ���ϱ�
	CRect noFrameGraphRegion = drawingGraphData.GetGraphRegions().GetNoFrameRegion();
	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	CRect titleRegion = GetGraphTitleRegion(pDC, noFrameGraphRegion.TopLeft(), strRealTitle, nTitleEndPosition, nTitleEndLine);

//>> Ÿ��Ʋ ������ ǥ�ý� ������ ������ ������ �ʵ��� ó�� - vntsorl (20090106)
	int nTitleStartPosition = VSGL_TITLE_START_POSITION + MARGIN + noFrameGraphRegion.left;
	if( titleRegion.left > nTitleStartPosition && 
		titleRegion.right + MARGIN * 2 > noFrameGraphRegion.right)
		return false;
//<< Ÿ��Ʋ ������ ǥ�ý� ������ ������ ������ �ʵ��� ó�� - vntsorl (20090106)

	// text �� ��� �Ѵ�.
	DrawGraphTitle(pDC, titleRegion, strRealTitle, GetTitleTextColor(drawingGraphData, subGraphData, nTitleIndex), subGraphData.GetColor().GetColor2());
	return true;
}

void CGraphTitleDrawer::DrawGraphTitle(CDC* pDC, const CRect& titleRegion, const CString& strTitle, const COLORREF& color1, const COLORREF& color2)
{
	CString strRealTitle = strTitle;
	// text region �׸���
	if(IsGraphTitleRegionDraw(strRealTitle))
		DrawGraphTitleRegion(pDC, titleRegion, color1);
	else
		strRealTitle.TrimLeft(NODRAW_TITLEREGION);

	// text �� ��� �Ѵ�.
	DrawGraphTitleText(pDC, titleRegion.TopLeft(), strRealTitle, color1, color2);
}

void CGraphTitleDrawer::DrawGraphTitleRegion(CDC* pDC, const CRect& titleRegion, const COLORREF& color)
{
	CBrush brush(color);
	CPen pen(PS_SOLID, 1, RGB(217, 217, 217));
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	CPen* pOldPen = pDC->SelectObject(&pen);

	CPoint point(titleRegion.left, titleRegion.top + titleRegion.Height()/2);
	CRect rect(point.x, point.y, point.x +MARGIN, point.y +MARGIN*2);
	rect.NormalizeRect();

	pDC->MoveTo(rect.right-1, rect.top+1);
	pDC->LineTo(rect.right-1, rect.bottom-1);
	pDC->LineTo(rect.left, rect.bottom-1);

	pDC->SelectObject(GetStockObject(NULL_PEN));
	pDC->Rectangle(&rect);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pen.DeleteObject();
	brush.DeleteObject();
}

void CGraphTitleDrawer::DrawGraphTitleText(CDC* pDC, const CPoint& titlePoint, const CString& strTitle, const COLORREF& color1, const COLORREF& color2)
{
	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF ordColor = pDC->GetTextColor();
	pDC->SetTextColor(color1);

	if(strTitle == _MTEXT( C3_NEWS_AND_NOTICE))
	{
		pDC->TextOut(titlePoint.x+4+MARGIN/2, titlePoint.y + MARGIN, CString( _MTEXT( C6_NEWS)) + " ");
		pDC->SetTextColor(color2);
		pDC->TextOut(titlePoint.x+4+MARGIN/2 + 29, titlePoint.y + MARGIN, _MTEXT( C6_NOTICE));
	}
	//@Solomon:091201 alzioyes. #0287 �ӽÿ� Ư���ڵ�.
	if(strTitle == "�ŵ� �ż�")
	{
		pDC->TextOut(titlePoint.x+4+MARGIN/2, titlePoint.y + MARGIN, "�ŵ� ");
		//pDC->SetTextColor(color2);
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(titlePoint.x+4+MARGIN/2 + 29, titlePoint.y + MARGIN, "�ż�");
	}
	else
		pDC->TextOut(titlePoint.x+4+MARGIN/2, titlePoint.y + MARGIN, strTitle);

	pDC->SetBkMode(nOldMode);
	pDC->SetTextColor(ordColor);
}

// ----------------------------------------------------------------------------
bool CGraphTitleDrawer::IsGraphTitleDraw(const CString& strTitle, const CGraphRegions& graphRegions) const
{
	if(!graphRegions.IsTitle())
		return false;

	// title �� " " �� ��쿡�� title�� �����.
	return (!strTitle.IsEmpty() && strTitle != " ");
}

// strTitle�� ù�ڰ� "&" �� ��쿡�� titleRegion�� �׸��� �ʴ´�.
bool CGraphTitleDrawer::IsGraphTitleRegionDraw(const CString& strTitle) const
{
	return (strTitle.Left(NODRAW_TITLEREGION.GetLength()) != NODRAW_TITLEREGION);
}

// ���ؼ� ���� ������������ ����
// -> "����-����Ʈ", "����-ä���/����" packet�� ���ؼ��� �־�� �Ѵ�.
bool CGraphTitleDrawer::IsTitleTextColor_StandardValue(CPacket* pPacket, const CSubGraphData& subGraphData) const
{
	if(pPacket == NULL || !pPacket->IsUseStandardValue())
		return false;

	CGraphBaseData::GRAPHTYPE eType = subGraphData.GetType();
	CGraphStyle style = subGraphData.GetStyle();
	if(eType == CGraphBaseData::Bong_Type)
		return (style.GetGraphBongStyle() == CGraphBaseData::LineBong);
	else if(eType == CGraphBaseData::Line_Type)
		return (style.GetGraphLineStyle() == CGraphBaseData::FillLine || style.GetGraphLineStyle() == CGraphBaseData::ComparisonLine);

	return false;
}

// title color 5��°
// -> "����-ä���/����" ������ ����
bool CGraphTitleDrawer::IsTitleTextColor_Five(const CSubGraphData& subGraphData) const
{
	if(subGraphData.GetType() != CGraphBaseData::Line_Type)
		return false;

	CGraphStyle style = subGraphData.GetStyle();
	return (style.GetGraphLineStyle() == CGraphBaseData::FillLine || style.GetGraphLineStyle() == CGraphBaseData::ComparisonLine);
}

// -----------------------------------------------------------------------------------------
CString CGraphTitleDrawer::GetGraphTitle(const CSubGraphData& subGraphData, HWND p_hOcxWnd) const
{
	CString strTitle = subGraphData.GetTitle();

	// <���ེ��> �� ��� title�� <���ེ��1>, <���ེ��2>�� ������ ǥ��.
	if(subGraphData.GetName() == _MTEXT( C3_PRESPAN))
	{
		if(strTitle == _MTEXT( C3_PRESPAN))
		{
			strTitle = subGraphData.GetTitle() + "1" + TITLE_COMPART + subGraphData.GetTitle() + "2" + TITLE_COMPART;
		}
	}

	_MHWND( p_hOcxWnd);
	CString strLanguageTitle = _MTOL1_N( strTitle);
	if( strLanguageTitle.IsEmpty()) strLanguageTitle = _MTOL3( strTitle);
	// ����� ǥ�� : �������� - ojtaso (20070625)
	CString strItemName = subGraphData.GetItemName();
	if( strItemName.IsEmpty()) strTitle = strLanguageTitle;
	else
	{
		if(strTitle.Compare( _MTEXT( C3_PRICE)))
			strTitle.Format(_T("%s:%s"), strItemName, strLanguageTitle);
		else
			strTitle = strItemName;
	}

	if(strTitle.Right(TITLE_COMPART.GetLength()) != TITLE_COMPART)
		strTitle += TITLE_COMPART;

	return strTitle;
}

// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
// ����� ǥ�� : �������� - ojtaso (20070625)
CString CGraphTitleDrawer::GetGraphTitle(const CDrawingGraphData& drawingGraphData, const CString& strGraphTitle, const int nIndex, const int nSubGraphIndex) const
{
	if(strGraphTitle.IsEmpty())
		return "";

	// cal condition ��������.
	CString strCondition;
	// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
	if(drawingGraphData.DoesConditionTitleDraw())
		// 20080908 ������	8����/32���� ǥ�� ����
		//strCondition = GetGraphTitle_Condition(drawingGraphData.GetIndicatorInfo(), nSubGraphIndex);
		strCondition = GetGraphTitle_Condition(drawingGraphData, nSubGraphIndex);

	// ������ price ��������.
	CString strData;
	if(drawingGraphData.DoesDataTitleDraw())
		strData = GetGraphTitle_Price(drawingGraphData, nIndex);

	if(strCondition.IsEmpty() && strData.IsEmpty())
		return strGraphTitle;
	if(!strCondition.IsEmpty() && !strData.IsEmpty())
		strData = ")(" + strData;

	return (strGraphTitle + '(' + strCondition + strData + ')');
}

// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
// 20080908 ������	8����/32���� ǥ�� ����
//CString CGraphTitleDrawer::GetGraphTitle_Condition(CIndicatorInfo* pIndicatorInfo, const int nSubGraphIndex) const
CString CGraphTitleDrawer::GetGraphTitle_Condition(const CDrawingGraphData& drawingGraphData, const int nSubGraphIndex) const
{
	// 20080908 ������	8����/32���� ǥ�� ����
	CIndicatorInfo* pIndicatorInfo = drawingGraphData.GetIndicatorInfo();
	if(pIndicatorInfo == NULL)
		return "";

	//SubGraphPartData �� ���ǰ��� ������ �װ��� ���򿡸� �ش�ȴ�.
	//�׸��� ������ ���ǰ��� Title�� ��Ÿ���� �ʴ´�.
	CList<double, double>* pCalConditionList = pIndicatorInfo->GetCommonCalConditionList();
	if(pCalConditionList == NULL || pCalConditionList->GetCount() <= 0)
		return "";

	// 20081107 JS.Kim	8����/32���� ǥ�� ������ ������ ���� �ʰ� ��쿡 ���� �Ǵ�
	CString strIndicatorName = drawingGraphData.GetIndicatorName();

	// 20080908 ������	8����/32���� ǥ�� ���� >>
	const CList< CPacket*, CPacket*> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	CPacket* pPacket = NULL;
	if( packetList.GetCount() > 0)
		pPacket = CDrawingPacketFinder().GetPracticalPacket( strIndicatorName, packetList);
	//>>
	// 20081107 JS.Kim	8����/32���� ǥ�� ������ ������ ���� �ʰ� ��쿡 ���� �Ǵ� <<
	CString	strConditionType = "";
	if( strIndicatorName == _MTEXT( C2_RENKO_CHART) )
	{
		if( pPacket && CDataConversion::IsBaseFormat(pPacket->GetType()) )
			strConditionType = pPacket->GetType();
	}
//>> vntsorl(20090828)
	else if( strIndicatorName == _MTEXT( C2_PARABOLIC_SAR) )
	{
		if( pPacket && !CDataConversion::IsBaseFormat(pPacket->GetType()) )
			strConditionType = pPacket->GetType();
	}
//<< vntsorl(20090828)
	//>>

	CString strCondition;
	// ��ǥ�� ���ǰ� ����׷����� ǥ�� - ojtaso (20071116)
	if(nSubGraphIndex < 0)
	{
		CString strCondType;
		POSITION pos = pCalConditionList->GetHeadPosition();
		while(pos != NULL)
		{
			double dCondition = pCalConditionList->GetNext(pos);
			// 20081107 JS.Kim	8����/32���� ǥ�� ������ ������ ���� �ʰ� ��쿡 ���� �Ǵ�
			// 20080908 JS.Kim	8����/32���� ǥ�� ����
//			strCondition += (CDataConversion::DoubleToString(dCondition, pPacket ? pPacket->GetType() : "") + ',');
			if( strIndicatorName == _MTEXT( C2_PARABOLIC_SAR) )
			{
				strCondType = CDataConversion::DoubleToString(dCondition, strConditionType);
				strCondType.TrimRight('0');
				strCondition += (strCondType + ',');
			}
			//20100515 �̹��� >>
			//else
			//	strCondition += (CDataConversion::DoubleToString(dCondition, strConditionType) + ',');
			else
			{
				if(strConditionType.IsEmpty())
					strCondition += (CDataConversion::DoubleToString(dCondition) + ',');
				else
				strCondition += (CDataConversion::DoubleToString(dCondition, strConditionType) + ',');
		}
			//20100515 �̹��� <<
	}
		
		strCondition.TrimRight(',');
	}
	else
	{
		POSITION pos = pCalConditionList->FindIndex(nSubGraphIndex);
		if(pos)
		{
			double dCondition = pCalConditionList->GetAt(pos);
			// 20081107 JS.Kim	8����/32���� ǥ�� ������ ������ ���� �ʰ� ��쿡 ���� �Ǵ�
			// 20080908 JS.Kim	8����/32���� ǥ�� ����
//			strCondition += (CDataConversion::DoubleToString(dCondition, pPacket ? pPacket->GetType() : ""));
			strCondition += (CDataConversion::DoubleToString(dCondition, strConditionType));
		}
	}

	return strCondition;
}

CString CGraphTitleDrawer::GetGraphTitle_Price(const CDrawingGraphData& drawingGraphData, const int nIndex) const
{
	CPacket* pPacket = GetPacket(drawingGraphData, nIndex);
	if(pPacket == NULL || pPacket->GetnumericDataList() == NULL || pPacket->GetDataCount() <= 0)
		return "";

	// 20080910 JS.Kim		8����/32���� ó��
	//return CDataConversion::GetStringThousandFormat(pPacket->GetnumericDataList()->GetTail(), pPacket->GetTypeFormatDouble(false));
	return CDataConversion::GetStringThousandFormat(pPacket->GetnumericDataList()->GetTail(), pPacket->GetType(false));
}

// ----------------------------------------------------------------------------
COLORREF CGraphTitleDrawer::GetTitleTextColor(const CDrawingGraphData& drawingGraphData, const CSubGraphData& subGraphData, const int nIndex) const
{
// 	if(drawingGraphData.GetGraphName() == _MTEXT( C2_PRICE_CHART))
// 		return GetTitleTextColor_PriceChart(drawingGraphData, subGraphData);
// 	else if(drawingGraphData.GetGraphName() == _MTEXT( C2_VOLUME_CHART)
// 		|| drawingGraphData.GetGraphName() == _MTEXT( C2_VOLUME_AMOUNT))
// 		return GetTitleTextColor_VolumeChart(drawingGraphData, subGraphData);
	if(drawingGraphData.GetIndicatorName() == _MTEXT( C2_PRICE_CHART))
		return GetTitleTextColor_PriceChart(drawingGraphData, subGraphData);
	else if(drawingGraphData.GetIndicatorName() == _MTEXT( C2_VOLUME_CHART)
		|| drawingGraphData.GetIndicatorName() == _MTEXT( C2_VOLUME_AMOUNT))
		return GetTitleTextColor_VolumeChart(drawingGraphData, subGraphData);

	if(IsTitleTextColor_Five(subGraphData))
		return GetTitleTextColor_Five(subGraphData);

	return GetTitleTextColor_General(subGraphData.GetColor(), nIndex);
}

COLORREF CGraphTitleDrawer::GetTitleTextColor_General(const CGraphColor& graphColor, const int nIndex) const
{
	if(nIndex <= 0)
		return graphColor.GetColor1();

	int nNumber = (nIndex + 1) % 3;
	switch(nNumber)
	{
	case 0: // nIndex = 2, 5, 8, ...
		return graphColor.GetColor5();
	case 1: // nIndex = 0, 3, 6, ...
		return graphColor.GetColor1();
	case 2: // nIndex = 1, 4, 7, ...
		return graphColor.GetColor3();
	}

	return graphColor.GetColor1();
}

// "������Ʈ" �� title �� ���ϱ�
COLORREF CGraphTitleDrawer::GetTitleTextColor_PriceChart(const CDrawingGraphData& drawingGraphData, const CSubGraphData& subGraphData) const
{
	// (2008/10/29 - Seung-Won, Bae) Do not use Color3. Use just only Color1.
	if( CChartInfo::WORLD_ON == drawingGraphData.GetChartMode())
		return subGraphData.GetColor().GetColor1();

	CPacket* pPacket = GetPacket(drawingGraphData, -1);
// 	if(!IsTitleTextColor_StandardValue(pPacket, subGraphData))
// 		return subGraphData.GetColor().GetColor1();
	if(!IsTitleTextColor_StandardValue(pPacket, subGraphData))
	{
		// (2010/6/21 - Seung-Won, Bae) for Line only Price Chart.
		CSubGraphPacketList* pSubGraphPacketList = drawingGraphData.GetSubGraphPacketList();
		if( pSubGraphPacketList)
		{
			const CList<CPacket*, CPacket*> &packetList = pSubGraphPacketList->GetSubGraphPacketList();
			if( packetList.GetCount() == 1)
			{
				CPacket *pPacket = packetList.GetHead();
				if( pPacket) if( !pPacket->IsUseStandardValue()) return subGraphData.GetColor().GetColor5();
	}
}

		return subGraphData.GetColor().GetColor1();
	}

	// ���ؼ�(��������)�� ����ϴ� ��� -> packet("����") Min/Max ������ ���� ����
	// -> Max <= ���ؼ� �ϰ�� : �϶�  (������ : ���) 
	double dStandardValue = pPacket->GetStandardValue();
	double dMin = 0.0, dMax = 0.0;
	if(pPacket->GetMinMax(drawingGraphData.GetDisplayAttributes().GetDataStartInBlock(), drawingGraphData.GetDisplayAttributes().GetDataEndInBlock(), dMin, dMax) >= 0)
{
		if(dMax <= dStandardValue)
			return subGraphData.GetColor().GetColor3();
	}

	return subGraphData.GetColor().GetColor1();
}

// "�ŷ�����Ʈ" �� title �� ���ϱ�
COLORREF CGraphTitleDrawer::GetTitleTextColor_VolumeChart(const CDrawingGraphData& drawingGraphData, const CSubGraphData& subGraphData) const
{
	// ������ ���ջ����� ���̰� �ϱ�
	return subGraphData.GetColor().GetColor5();
/*
	int nDrawCond = 0;
	if(!GetDrawConditionData(drawingGraphData.GetIndicatorInfo(), 0, nDrawCond))
		return subGraphData.GetColor().GetColor5();

	// �׸���Ÿ�� -> 1 : �Ϲ�, 2 : ����϶�, 3 : ĵ����� ����, 4 : ���������� ���������� ���/�϶�
	if(nDrawCond == 1)
		return subGraphData.GetColor().GetColor5();
	else if(nDrawCond == 3)
	{
		CGraphColor graphColor(subGraphData.GetColor());
		GetGraphColor(drawingGraphData.GetIndicatorList(), _MTEXT( C2_PRICE_CHART), 0, graphColor);
		return graphColor.GetColor1();
	}

	return subGraphData.GetColor().GetColor1();
*/
}

// 5��° color ���
COLORREF CGraphTitleDrawer::GetTitleTextColor_Five(const CSubGraphData& subGraphData) const
{
	return subGraphData.GetColor().GetColor5();
}

// ----------------------------------------------------------------------------
// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
CRect CGraphTitleDrawer::GetGraphTitleRegion(CDC* pDC, const CPoint& orgPoint, const CString& strTitle, int& nTitleEndPosition, int& nTitleEndLine) const
{
	CSize titleSize = pDC->GetTextExtent(strTitle);
	int x = nTitleEndPosition + MARGIN;
	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	int y = orgPoint.y + ((titleSize.cy + MARGIN) * (nTitleEndLine - 1));
	nTitleEndPosition = x + titleSize.cx + MARGIN * 2;

	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	return CRect(orgPoint.x +x, y, orgPoint.x +x +titleSize.cx, y +titleSize.cy);
}

CPacket* CGraphTitleDrawer::GetPacket(const CDrawingGraphData& drawingGraphData, const int nIndex) const
{
	CSubGraphPacketList* pSubGraphPacketList = drawingGraphData.GetSubGraphPacketList();
	if(pSubGraphPacketList == NULL)
		return NULL;

	const CList<CPacket*, CPacket*> &packetList = pSubGraphPacketList->GetSubGraphPacketList();
	CPacket* pPacket = CDrawingPacketFinder().GetPacket( packetList, nIndex);
	if(pPacket != NULL)
		return pPacket;

	return CDrawingPacketFinder().GetPracticalPacket( drawingGraphData.GetGraphName(), packetList);
}

bool CGraphTitleDrawer::GetDrawConditionData(const CIndicatorInfo* pIndicatorInfo, const int nIndex, int& nDrawCond) const
{
	if(pIndicatorInfo == NULL || nIndex < 0)
		return false;

	CList<double, double>* pDrawCondList = pIndicatorInfo->GetDrawConditionList();
	if(pDrawCondList == NULL || pDrawCondList->GetCount() <= 0)
		return false;

	POSITION pos = pDrawCondList->FindIndex(nIndex);
	if(pos == NULL)
		return false;

	nDrawCond = (int)pDrawCondList->GetAt(pos);
	return true;
}

/*
bool CGraphTitleDrawer::GetGraphColor( CIndicatorList* pIndicatorList, const CString& strGraphName, const int nSubGraphIndex, CGraphColor& graphColor) const
{
	if(pIndicatorList == NULL || strGraphName.IsEmpty() || nSubGraphIndex < 0)
		return false;

	CIndicatorInfo* pIndicatorInfo = pIndicatorList->GetIndicatorInfo( EILT_USER, strGraphName, EIIGT_WITH_DEFAULT_GET);
	if(pIndicatorInfo == NULL)
		return false;

	CSubGraphData* pSubGraphData = pIndicatorInfo->GetSubGraphData(nSubGraphIndex);
	if(pSubGraphData == NULL)
		return false;

	graphColor = pSubGraphData->GetColor();
	return true;
}
*/