// PriceScaleDrawer.cpp: implementation of the CPriceScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PriceScaleDrawer.h"

#include "../Include_Chart/DLL_Load/PacketBase.h"		// for CPacket
#include "ScaleRegion.h"
#include "BaseData.h"
#include "Conversion.h"
#include "DataMath.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CPriceScaleDrawer

CPriceScaleDrawer::CPriceScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CVertScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

// scale text, grid�� �׸���.
void CPriceScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale)
{
	DrawTextOrGrid(pDC, drawingVertScaleData, scaleRegion, vertScale, CDrawer::GetPacketDataType(drawingVertScaleData.GetPacket()));
}

// ����
void CPriceScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale)
{
	DrawTextOrGrid_Both(pDC, drawingVertScaleData, vertScaleRegion, vertScale, CDrawer::GetPacketDataType(drawingVertScaleData.GetPacket()));
}

// protected ============================================================================
// (2009/12/8 - Seung-Won, Bae) Comment and Adjusting
void CPriceScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale, const CString& dataType)
{
	// 0. �� Data Packet�� Ȯ���Ѵ�.
	CPacket *pPacket = drawingVertScaleData.GetPacket();
	if( !pPacket) return;

	// 0. ������ ������ ������ Text�� ���� �� �ִ� ���� Ȯ��
	int totalCount = CVertScaleDrawer::GetCount( pDC, ( scaleRegion.GetScaleTextRegion()).Height()) +1;
	if( totalCount <= 0) return;

	int start = 0, end = 0;	// Scale ���м� ������ġ�� ����ġ
	double dGap = 0.0;		// Scale ���м� ����
	bool bround = false;	// �ڵ� Scale ���� ���� ��� ���� (?)
	if( !GetPriceDrawingEnvironment( drawingVertScaleData.GetDisplayDataMinMax(), dataType, totalCount,
			dGap, start, end, bround, vertScale, drawingVertScaleData.GetHogaUnit()))
	{
		if(!CVertScaleDrawer::GetDrawingEnvironment( pDC, scaleRegion.GetScaleTextRegion(), 
			drawingVertScaleData.GetDisplayDataMinMax(), dataType, start, end, dGap, bround))
		return;
	}
	//}}

	// ���� �� �ڸ��� ���ϱ� - ��Ȯ�� Ÿ�� �ٽ� ���� 12.24.2001
	BOOL bAutoScaleType = pPacket->IsAutoScaleType();
	CString strFormatTrans = CVertScaleDrawer::GetFormatTrans(drawingVertScaleData.GetDisplayDataMinMax(), dataType, bAutoScaleType);
	pPacket->SetScaleType( bAutoScaleType ? strFormatTrans : pPacket->GetType());

	// type ����
	int typeRegionY = CVertScaleDrawer::DrawType(pDC, scaleRegion.GetScaleTextRegion(),	strFormatTrans, 
		bround, vertScale.GetVertScaleUnitType(), drawingVertScaleData.GetScaleColor().GetScaleTextColor());

	// scale ����
	// ȣ������, ���ذ� ��� - ojtaso (20070917)
	CVertScaleDrawer::DrawTextOrGrid(pDC, drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin(), 
		typeRegionY, start, end, dGap, bround, drawingVertScaleData, vertScale,
		scaleRegion.GetScaleTextRegion(), CRect(0, 0, 0, 0), dataType, strFormatTrans, true, drawingVertScaleData.GetStandardValue());
}

void CPriceScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale, const CString& dataType)
{
	CPacket *pPacket = drawingVertScaleData.GetPacket();
	if( !pPacket) return;

	CRect scaleTextRegion_left = vertScaleRegion.GetLeftScaleTextRegion();
	CRect scaleTextRegion_right = vertScaleRegion.GetRightScaleTextRegion();

	//sy 2002.8.28. --> gap�� ���� packetDataType ����(��ǥ�� ���)
	CString changeDataType = dataType;
	//if(!drawingVertScaleData.GetGraphName().IsEmpty())
	//	changeDataType = CVertScaleDrawer::GetPacketDataType(dGap);

	int start = 0, end = 0;
	double dGap = 0.0;
	bool bround = false;

	//2007.01.22 Scale����� ����,ȣ��������
	int totalCount = CVertScaleDrawer::GetCount(pDC, (scaleTextRegion_left.Height())) +1;
	if(totalCount <= 0)
		return;

//	if(!CVertScaleDrawer::GetDrawingEnvironment(pDC, scaleTextRegion_left, 
//		drawingVertScaleData.GetDisplayDataMinMax(), dataType, start, end, dGap, bround))
//		return;

	//{{2007.07.09 by LYH ���� �Ǿ� ���� ���� ��� ���� Gap��� ������� ó��
	// ȣ������, ���ذ� ��� - ojtaso (20070917)
	if(!GetPriceDrawingEnvironment(drawingVertScaleData.GetDisplayDataMinMax(), dataType, totalCount,
								dGap,start, end,  bround,vertScale, drawingVertScaleData.GetHogaUnit()))
	{
		if(!CVertScaleDrawer::GetDrawingEnvironment(pDC, scaleTextRegion_left, 
		drawingVertScaleData.GetDisplayDataMinMax(), dataType, start, end, dGap, bround))
		return;
	}
	//}}

	// ���� �� �ڸ��� ���ϱ� - ��Ȯ�� Ÿ�� �ٽ� ���� 12.24.2001
	BOOL bAutoScaleType = pPacket->IsAutoScaleType();
	CString strFormatTrans = CVertScaleDrawer::GetFormatTrans(drawingVertScaleData.GetDisplayDataMinMax(), changeDataType, bAutoScaleType);
	pPacket->SetScaleType( bAutoScaleType ? strFormatTrans : pPacket->GetType());

	// type ����
	int typeRegionY = CVertScaleDrawer::DrawType(pDC, scaleTextRegion_left, strFormatTrans, 
		bround, vertScale.GetVertScaleUnitType(), drawingVertScaleData.GetScaleColor().GetScaleTextColor());
	typeRegionY = CVertScaleDrawer::DrawType(pDC, scaleTextRegion_right, strFormatTrans, 
		bround, vertScale.GetVertScaleUnitType(), drawingVertScaleData.GetScaleColor().GetScaleTextColor());

	// scale ����	
	// ȣ������, ���ذ� ��� - ojtaso (20070917)
	CVertScaleDrawer::DrawTextOrGrid(pDC, drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin(), 
		typeRegionY, start, end, dGap, bround, drawingVertScaleData, vertScale, 
		scaleTextRegion_left, scaleTextRegion_right, dataType, strFormatTrans, true, drawingVertScaleData.GetStandardValue());
}

// (2009/12/8 - Seung-Won, Bae) Comment and Adjusting
// 0. ȣ������, ����� ���� ������ �⺻���� �Ѵ�.
bool CPriceScaleDrawer::GetPriceDrawingEnvironment( const CDisplayDataMinMax& displayDataMinMax, const CString& strPacketType, 
		const int totalCount, double& dGap, int& start, int& end, bool& bround, const CVerticalScale& vertScale,
		const double dHogaUnit)
{
	ASSERT( 0 < totalCount);

	// 0. Get Default Gap Unit.
	if( dHogaUnit <= 0)	dGap = 0;
	else				dGap = dHogaUnit;

	int nHoGaNumber = ( int)vertScale.GetVertScaleHoGaNumber();
	if( 0 < nHoGaNumber)	dGap = dGap * nHoGaNumber;
	else					dGap = 0;

	// ����� �����϶� ���� ������ 0(�ڵ�)�̸� �ִ� ������ ���� �� �ִ� ������ �����Ѵ�.
	int nHogaScaleType = vertScale.GetVertUserHogaScaleType();
	int nUserCount = ( int)vertScale.GetVertScaleUserNumber();
	double dScaleMaxGap = displayDataMinMax.GetCount();
	if( 1 != nHogaScaleType || dGap <= 0)
	{
		// vntsorl:[0000268] ��Ʈ ȯ�� �������� ���ϱ��� pip���� �����ϸ� y���� pip ��� �����Ͱ� �� �� ������ ����
		if( nHogaScaleType == 0 && nUserCount == 0)
			return false;

		int nCount = nUserCount;
		if( nCount <= 0) nCount = totalCount - 1;
		if( nCount <= 0) return false;

		dGap = dScaleMaxGap / nCount;

		// ȣ�� ������ �޴� ��� ���� ȣ�������� �����Ѵ�.
		double dMinScaleUnit = dHogaUnit;
		// ȣ�� ������ ���� ��� Packet Type�� ������� �Ѵ�.
		if( dMinScaleUnit <= 0)
		{
			if( strPacketType.Find( '.') < 0)
			{
				double dDigits = log10( dGap);
				if( dDigits != int( dDigits)) dDigits = int( dDigits);
				dMinScaleUnit = pow( 10, dDigits);
			}
			else if( strPacketType.Find( '/') < 0)
			{
				int nStart = strPacketType.Find( '0');
				if( nStart < 0) return false;
				dMinScaleUnit = atof( strPacketType.Mid( nStart));
			}
			else
			{
				int nBase = 0, nDecLen1 = 0, nDecLen2 = 0;
				if( !CDataConversion::GetBaseFormatInfo( strPacketType, nBase, nDecLen1, nDecLen2))
					return false;
				dMinScaleUnit = ( 1.0 / nBase) / pow( 10, nDecLen2);
			}
		}

		if( dGap <= dMinScaleUnit) dGap = dMinScaleUnit;
		else
		{
			double dHoGaNumber = dGap / dMinScaleUnit;
			// ȣ���� ��� �ǹ� �ڸ����ڴ� ���ڸ��� �ǵ��� �Ѵ�.
			int nHoGaDigits = int( log10( dHoGaNumber));
			dHoGaNumber = int( dHoGaNumber / pow( 10, nHoGaDigits)) * pow( 10, nHoGaDigits);
			dGap = dMinScaleUnit * dHoGaNumber;
		}
	}

	// ������ ���� Gap�� �ڵ� �����ؾ� �ϴ� ��� (��Ȯ�� ����� ������ �ƴѰ��)
	if( dGap <= 0) return false;
	CVertScaleDrawer::GetStartAndEnd( displayDataMinMax, int( dScaleMaxGap / dGap) + 1, dGap, start, end);
	bround = CVertScaleDrawer::GetBround(displayDataMinMax);
	return true;
}
