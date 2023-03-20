// ScaleAndGraphDrawer.cpp: implementation of the CScaleAndGraphDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScaleAndGraphDrawer.h"

#include "../Include_Chart/Dll_Load/ScaleDrawer.h"			// ScaleDraw.dll
#include "../Include_Chart/Dll_Load/PacketList.h"			// PacketData.dll
#include "../Include_Chart/Dll_Load/PacketBase.h"			// PacketData.dll
#include "../Include_Chart/Dll_Load/XScaleManager.h"		// for CXScaleManager
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/ScaleBaseData.h"
#include "../Include_Chart/BaseData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// CBlockScaleDrawer

// public =====================================================================
// RQ�� Unit���� ���� : �������� - ojtaso (20070601)
void CBlockScaleDrawer::DrawHorzScale(CDC* pDC, CDrawingHorzScaleData* pDrawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, LPCTSTR lpszRQ, HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
{
	if(pDrawingHorzScaleData == NULL)
		return;

	GetDrawingHorzScaleData(pDrawingHorzScaleData, horzScale.GetScaleData(), pxScaleManager, lpszRQ);
	CScaleDrawer( p_hOcxWnd, p_eChartMode).DrawHorzTextOrGrid(pDC, *pDrawingHorzScaleData, horzScaleRegion, horzScale, pxScaleManager, lpszRQ);
}

void CBlockScaleDrawer::DrawVertScale(CDC* pDC, CDrawingVertScaleData* pDrawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale, HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
{
	if(pDrawingVertScaleData == NULL)
		return;

	GetDrawingVertScaleData(pDrawingVertScaleData, vertScale.GetScaleData());
	CScaleDrawer( p_hOcxWnd, p_eChartMode).DrawVertTextOrGrid(pDC, *pDrawingVertScaleData, vertScaleRegion, vertScale);
}


// private ====================================================================
// RQ�� Unit���� ���� : �������� - ojtaso (20070601)
void CBlockScaleDrawer::GetDrawingHorzScaleData(CDrawingHorzScaleData* pDrawingHorzScaleData, const CString& scaleData, CXScaleManager* pxScaleManager, LPCTSTR lpszRQ) const
{
	if(pDrawingHorzScaleData == NULL)
		return;

	pDrawingHorzScaleData->SetScaleDrawerType(GetHorzScaleDrawerType(pDrawingHorzScaleData, scaleData, pxScaleManager, lpszRQ));
}

// ���� scale type.
// RQ�� Unit���� ���� : �������� - ojtaso (20070601)
CScaleBaseData::HORZSCALEDRAWERTYPE CBlockScaleDrawer::GetHorzScaleDrawerType(CDrawingHorzScaleData* pDrawingHorzScaleData, const CString& scaleData, CXScaleManager* pxScaleManager, LPCTSTR lpszRQ) const
{
	if(pDrawingHorzScaleData == NULL || scaleData.IsEmpty())
		return CScaleBaseData::HORZ_NOTYPE;

	if(scaleData.Find( _MTEXT( C0_DATE_TIME)) >= 0){ // "�ڷ�����"
		// 20080925 JS.Kim	���� �ٸ� �ð����� Scale ó��. 
		//	���� ���õǾ� �ִ� �����ϰ� ���߱� ���� CurrentRQ�� �������� ���Ѵ� <<
//		return pxScaleManager->GetScaleUnit_Max(lpszRQ);
		return pxScaleManager->GetScaleUnit_Max("");
		//>>
		//if(pDrawingHorzScaleData->GetPacket() == NULL)
		//	return CScaleBaseData::HORZ_DAILY;
		//return (CScaleBaseData::HORZSCALEDRAWERTYPE) pDrawingHorzScaleData->GetPacket()->GetDateUnitType();
	}
	else if(scaleData.Find( _MTEXT( C0_TEXT)) >= 0) // "Text"
		return CScaleBaseData::HORZ_TEXT;
	else if(scaleData == _MTEXT( C6_EXERCISED_PRICE))	  // "��簡"
		return CScaleBaseData::HORZ_PRICEZONE;
	else if(scaleData == _MTEXT( C3_MARKET_PROFILE))// "MarketProfile"
		return CScaleBaseData::HORZ_MARKETPROFILE;

	// "�¿��", "�¿������", "���������¿��"
	if(pDrawingHorzScaleData->GetHeadGraphType() == CGraphBaseData::Bar_Type){
		if(pDrawingHorzScaleData->GetHeadGraphStyle().GetGraphBarStyle() == CGraphBaseData::LeftRightBar || 
			pDrawingHorzScaleData->GetHeadGraphStyle().GetGraphBarStyle() == CGraphBaseData::LeftRightCombBar || 
			pDrawingHorzScaleData->GetHeadGraphStyle().GetGraphBarStyle() == CGraphBaseData::FixedRateLeftRightBar)
			return CScaleBaseData::HORZ_LEFTRIGHT;
	}

	return CScaleBaseData::HORZ_DATA;
}

// ���� scale type.
void CBlockScaleDrawer::GetDrawingVertScaleData(CDrawingVertScaleData* pDrawingVertScaleData, const CString& scaleData) const
{
	// ���� scale type setting.
	if(pDrawingVertScaleData->GetScaleDrawerType() == CScaleBaseData::VERT_NOTYPE)
	{
		pDrawingVertScaleData->SetScaleDrawerType(GetVertScaleDrawerType(scaleData, pDrawingVertScaleData->GetIndicatorName()));
	}

	// <��ǥ Data>, <�����> �� ���
	// (2007/2/16 - Seung-Won, Bae) It can be with ','(comma).
	if( !strncmp( _MTEXT( C6_INDICATOR_DATA), scaleData, 9) || !strncmp( _MTEXT( C6_PERCENT), scaleData, 6))
		return;

	// Ư�� <Packet> �� ���
	if(pDrawingVertScaleData->GetPacket() == NULL)
		return;

	pDrawingVertScaleData->SetPacket(pDrawingVertScaleData->GetPacket()->GetPacketList()->GetBaseData(scaleData));
}

CScaleBaseData::VERTSCALEDRAWERTYPE CBlockScaleDrawer::GetVertScaleDrawerType(const CString& scalePacketName, const CString &p_strIndicatorName) const
{
	// <��ǥ Data>
	// (2007/2/16 - Seung-Won, Bae) It can be with ','(comma).
	if( !strncmp( _MTEXT( C6_INDICATOR_DATA), scalePacketName, 9))
		return GetVertScaleDrawerType( p_strIndicatorName);
	// <Text>
	else if( !strncmp( _MTEXT( C0_TEXT), scalePacketName, 4))
		return CScaleBaseData::VERT_TEXT;
	// <�����>
	else if( !strncmp( _MTEXT( C6_PERCENT), scalePacketName, 6))
		return CScaleBaseData::VERT_PERCENT;

	// <���� packet data>
	return CScaleBaseData::VERT_PACKETDATA;
}

CScaleBaseData::VERTSCALEDRAWERTYPE CBlockScaleDrawer::GetVertScaleDrawerType( const CString &p_strIndicatorName) const
{
	if(g_iMetaTable.IsScaleGroup_Price( p_strIndicatorName))	// ������Ʈ
		return CScaleBaseData::VERT_PRICE;
	else if( p_strIndicatorName == _MTEXT( C2_COMPARISON_CHART))	// ������Ʈ
		return CScaleBaseData::VERT_PERCENT;
	else if( g_iMetaTable.IsScaleGroup_Volume( p_strIndicatorName))	// �ŷ�����Ʈ
		return CScaleBaseData::VERT_VOLUME;
	else if( p_strIndicatorName == _MTEXT( C3_MARKET_PROFILE))// MarketProfile
		return CScaleBaseData::VERT_MARKETPROFILE;
	//2004.05.12. -> "���Ź�"�� Ư�������� "������Ʈ" ÷�������� �����.
	else if( p_strIndicatorName == _MTEXT( C3_BUY_SELL_OVERHANGING_SUPPLY)) // ���Ź�
		return CScaleBaseData::VERT_SALE;

	// "��ǥ"
	return CScaleBaseData::VERT_BASELINE;
}
