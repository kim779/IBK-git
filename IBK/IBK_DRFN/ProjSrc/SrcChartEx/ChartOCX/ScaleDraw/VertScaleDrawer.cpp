// VertScaleDrawer.cpp: implementation of the CVertScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VertScaleDrawer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "BaseData.h"
#include "Conversion.h"
#include "DataMath.h"

// packet.dll
#include "PacketBase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CVertScaleDrawer

CVertScaleDrawer::CVertScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CDrawer( p_hOcxWnd, p_eChartMode)
{
}

// public =============================================================================
void CVertScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale)
{
}

void CVertScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale)
{
}

bool CVertScaleDrawer::GetDrawingEnvironment(const CDisplayDataMinMax& displayDataMinMax, const CString& strPacketType, 
		const int totalCount, double& dGap, int& start, int& end, bool& bround) const
{
	if(totalCount <= 0)
		return false;

	if(!GetGap(displayDataMinMax, strPacketType, totalCount, dGap))
		return false;

	int nRealTotalCount = totalCount;
	//dGap�� ���� total count ����
	if(dGap != 0)
		nRealTotalCount = CMath::Round(displayDataMinMax.GetCount() / dGap) +1;
	// (2006/11/19 - Seung-Won, Bae) Enable microscopic scale.
	//		So. do not Reset Scale Gap.
	//	nGap�� 0�� �Ǵ°��� �������ش�
	//	if(dGap <= 0){
	//		dGap = 1.0;
	//		nRealTotalCount = CMath::Round(displayDataMinMax.GetDisplayMax() - displayDataMinMax.GetDisplayMin());
	//	}

	GetStartAndEnd(displayDataMinMax, nRealTotalCount, dGap, start, end);

	// (2007/2/24 - Seung-Won, Bae) Round must be processed by PacketType.
	// (2008/9/8 - Seung-Won, Bae) Set bRound with Data Size
	bround = strPacketType.Find( '.') < 0 && strPacketType.Find( '0') || CVertScaleDrawer::GetBround( displayDataMinMax);
	return true;
}


// protected ==========================================================================
// scale �׸��� ���� ���ǵ� �����´�. -> round ó�� ����
/*bool CVertScaleDrawer::GetDrawingEnvironment(CDC* pDC, const CRect& scaleTextRegion, const CDisplayDataMinMax& displayDataMinMax,
		int& start, int& end, double& dGap)
{
	int totalCount = CVertScaleDrawer::GetCount(pDC, scaleTextRegion.Height()) +1;
	if(totalCount <= 0)
		return false;
	
	dGap = CMath::Round(displayDataMinMax.GetCount() / (double)totalCount);
	CString strGap = CDataConversion::IntToString((int)dGap);	// �̰��� ���� double������ �� �Լ��� ����ؾ� �Ѵ� - �ؿ� round �Լ��� ����ϱ� �����̴�
	dGap = CMath::Round(dGap, strGap.GetLength() - 1);
	// �ݿø� �� ���� 5, 10 ������ �����Ѵ�
	// 08.12.2001 by yousun
	if(dGap < 10.0) dGap = 10;
	else if(dGap / pow(10, strGap.GetLength() - 1) < 5)
		dGap = (int) pow(10, strGap.GetLength() + 1) / 2;
	else if(dGap / pow(10, strGap.GetLength() - 1) < 10)
		dGap = (int) pow(10, strGap.GetLength() + 1);

	if(totalCount > 0 && dGap != 0.0)
		totalCount = CMath::Round(displayDataMinMax.GetCount() / dGap) +1;

	GetStartAndEnd(displayDataMinMax, totalCount, dGap, start, end);

	return true;
}*/

//sy 2002.8.31.
// -> round ó�� ��
bool CVertScaleDrawer::GetDrawingEnvironment(CDC* pDC, const CRect& scaleTextRegion, const CDisplayDataMinMax& displayDataMinMax,
		const CString& dataType, int& start, int& end, double& dGap, bool& bround)
{
// (2006/10/27 - Seung-Won, Bae) Support One Data Scale
//	if(displayDataMinMax.GetCount() == 0) return false;

	int totalCount = GetCount(pDC, scaleTextRegion.Height()) +1;
	if(totalCount <= 0)
		return false;

	return GetDrawingEnvironment(displayDataMinMax, dataType, totalCount, dGap, start, end, bround);
}
//end

// ----------------------------------------------------------------------------
//sy 2002.8.28. --> gap�� ���� packetDataType ����
CString CVertScaleDrawer::GetPacketDataType(const CString& strPacketType, const double& dGap) const
{
	CString strDataType = strPacketType;
	if(dGap == 1.0 || strDataType.Right(1) != '1' || strDataType.Find('.') == -1)
		return strDataType;

	CString strGap = CDataConversion::DoubleToString(dGap);
	if(dGap > 1.0){
		int nDecimalDownCount = CDataConversion::GetDecimalDownCount(strDataType) +1;
		if(strGap.GetLength() > nDecimalDownCount)
			return strDataType;
		CString strChangeDataType = strDataType.Left(strDataType.GetLength() - nDecimalDownCount);
		strChangeDataType.TrimRight('0');
		return strChangeDataType + '1';
	}

	int nDecimalDownCount = CDataConversion::GetDecimalDownCount(strGap, false);
	if(nDecimalDownCount <= 0)
		return strDataType;

	CString strChangeDataType = CDataConversion::GetStringData(strDataType, '.', false);
	for(int nIndex = 0; nIndex < nDecimalDownCount -1; nIndex++)
		strChangeDataType += '0';

	return strChangeDataType + '1';
}

// ------------------------------------------------------------------------------------
// text�� ǥ���� ��ġ.
CPoint CVertScaleDrawer::GetTextPosition(const int org_x, const int y, const CSize& textSize, const int textPos) const
{
	if(textPos == DT_BOTTOM)
		return CPoint(GetVertScalePosition_x(org_x, textSize.cx), (y - textSize.cy));
	return CPoint(GetVertScalePosition_x(org_x, textSize.cx), (y - textSize.cy/2));
}

// relative_y -> double������ ���� <- 08.08.2002
bool CVertScaleDrawer::GetVertScaleAbsolutePosition_y(const CRect& region, const double relative_y, 
		const CDisplayDataMinMax& displayDataMinMax, const bool bIsLog, int& nowPt_y)
{
	nowPt_y = CDataConversion::RelativeToAbsolute_Y(region, GetRealData(bIsLog, relative_y), 
					GetRealData(bIsLog, displayDataMinMax.GetDisplayMin()), 
					GetRealData(bIsLog, displayDataMinMax.GetDisplayMax()));

	return (nowPt_y >= region.top && nowPt_y <= region.bottom);
}

// ------------------------------------------------------------------------------------
// text�� ǥ���� data�� string������ ������.
//sy 2002.8.29. -> ����� ���õǾ� ����
CString CVertScaleDrawer::GetStrData(const double& dData, const CString& dataType, const bool bround, const int nRoundLength) const
{
	CString strData;
	// 20080910 JS.Kim		8����/32���� ó��	<<
	if( CDataConversion::IsBaseFormat(dataType) )
	{
		strData = CDataConversion::DoubleToString(dData);
	}
	else
	{
	//>>
		strData = CDataConversion::DoubleToString(dData, CDataConversion::GetDecimalDownCount(dataType));
		//�鸸���� �̻� ���� ��ƾ
		if(nRoundLength > 0 && bround){
			if(strData.Find('.') != -1)
				strData = CDataConversion::GetStringData(strData, '.');
			// (2007/2/24 - Seung-Won, Bae) Check Round All.
			if(strData.GetLength() > nRoundLength + 1 || ( strData.GetLength() > nRoundLength && strData.GetAt( 0) != '-'))
				strData = strData.Left(strData.GetLength() - nRoundLength);
			else  if( strData != "0") strData.Empty();
		}
	}

	if(dataType.IsEmpty() || bround)
		return CDataConversion::GetStringThousandFormat(strData);
	return CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strData, dataType);
}

// ���� �� �ڸ��� ���ϱ� - ��Ȯ�� Ÿ�� ����
// (2007/2/21 - Seung-Won, Bae) Support Not-Auto Scale Type.
CString CVertScaleDrawer::GetFormatTrans(const CDisplayDataMinMax& displayDataMinMax, const CString& dataType, BOOL p_bAutoScaleType)
{
	CString strMaxCutoff, strFormatTrans;
	
	double dStdTrans = displayDataMinMax.GetDisplayMax();
	if(displayDataMinMax.GetDisplayMin() < 0 && displayDataMinMax.GetDisplayMax() >= 0)
	{
		dStdTrans = CDataConversion::GetLongLengthData(displayDataMinMax.GetDisplayMin(), displayDataMinMax.GetDisplayMax(), dataType);
	}

	CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTransCutoff(dStdTrans, dataType, strMaxCutoff, strFormatTrans, p_bAutoScaleType);
	return strFormatTrans;
}

// ------------------------------------------------------------------------------------
// ȣ������, ���ذ� ��� - ojtaso (20070917)
void CVertScaleDrawer::DrawTextOrGrid(CDC* pDC, const double orgY, const int typeRegionY, const int start, const int end, 
		const double& dGap, const bool bround, const CDrawingVertScaleData& drawingVertScaleData, const CVerticalScale& vertScale,
		const CRect& region1, const CRect& region2, const CString& dataOrgType, const CString& dataViewType, const bool bIsSignDraw, const double dStandardValue/* = 0.*/)
{
	// (2007/2/21 - Seung-Won, Bae) x 10.. Type's ORG Type is only "x 1"
	CString strOrgType = dataOrgType;
	if( strOrgType.Find( '.') < 0) strOrgType = "x 1";

	CRect noFrameGraphRegion = drawingVertScaleData.GetNoFrameGraphRegion();
	// ��ȣ�� ���� text, grid �׸���
	double sign = (double)drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin() * (double)drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMax();
	// ȣ������, ���ذ� ��� - ojtaso (20070917)
	if(sign < 0.0){
		DrawTextOrGrid_PlusAndMinusSign(pDC, 0, typeRegionY, start, end, dGap, bround, 
			drawingVertScaleData, vertScale, noFrameGraphRegion, region1, region2, 
			strOrgType, dataViewType, bIsSignDraw, dStandardValue);
	}
	else{
		// ȣ������, ���ذ� ��� - ojtaso (20070917)
		DrawTextOrGrid_PlusOrMinusSign(pDC, orgY, typeRegionY, start, end, dGap, bround, 
			drawingVertScaleData, vertScale, noFrameGraphRegion, region1, region2, 
			strOrgType, dataViewType, bIsSignDraw, dStandardValue);
	}
}

// graph type �׸���
int CVertScaleDrawer::DrawType(CDC* pDC, const CRect& typeTextRegion, const CString& dataType, const bool bround, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType, const COLORREF color)
{
	if(dataType.Find('.') != -1 || dataType.Right(1) == '1' || dataType.Right(1) == '%')
		return typeTextRegion.bottom;

	CString strNewDataType = dataType;
	if(eVertScaleUnitType == CScaleBaseData::VERT_NUM_CHAR)
		strNewDataType = CDataConversion( m_hOcxWnd, m_eChartMode).GetChangeFormat_NumCharUnit(dataType);

	CSize textSize = pDC->GetTextExtent(strNewDataType);
	CVertScaleDrawer::DrawTextOut(pDC, GetTextPosition(typeTextRegion.right, typeTextRegion.bottom, 
		textSize, DT_BOTTOM), strNewDataType, color);
	return typeTextRegion.bottom - textSize.cy;
}

// gaid �׸���
void CVertScaleDrawer::DrawGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, 
		const CScaleBaseData::SCALEGRIDTYPE gridType, const int pt_y, const int p_nActiveFlag)
{
	if( !( p_nActiveFlag & VSGA_GRIDLINE)) return;

	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	//	CScaleBaseData::SCALEGRIDTYPE eMainGridType = drawingVertScaleData.GetBaseScaleGridType();
	//	if( eMainGridType == CScaleBaseData::SLNONE) eMainGridType = gridType;

	//sy 2003.6.25. -> drawingGraphRegion�� �����ʿ� ���� ������ �����Ƿ� 
	// �� �������� �׸��� ���ؼ� noFrameGraphRegion �� �ʿ��ϴ�.
	CDrawer::DrawGrid(	pDC,
						gridType,
						CPoint(drawingVertScaleData.GetDrawingGraphRegion().left, pt_y),
						CPoint(drawingVertScaleData.GetNoFrameGraphRegion().right, pt_y),
						drawingVertScaleData.GetScaleColor().GetVertScaleColor(), 1);
}

//sy 2003.6.26. ���� �׸���
void CVertScaleDrawer::DrawScale(CDC * pDC, const CRect& region, double dGap, double orgY, int nstart, int nend, 
						const CDrawingVertScaleData& drawingVertScaleData)
{
	if(region.IsRectEmpty())
		return;

	int nScaleLineCount = GetScaleLineCount(dGap);
	bool bIsInvert = drawingVertScaleData.GetVertScaleInvertType();
	bool bIsLog = drawingVertScaleData.GetVertScaleLogType();
	if(CDrawer::GetPacketDataCount(drawingVertScaleData.GetPacket()) <= 0)
		bIsLog = false;

	//sy 2002.9.7 -> �ܴ����� ��
	CPen newPen;
	newPen.CreatePen(BS_SOLID, 1, drawingVertScaleData.GetCompartLineColor());
	CPen* pOldPen = pDC->SelectObject(&newPen);
	//end

	// (2006/10/27 - Seung-Won, Bae) Support One Data Scale
	if( nstart == nend)
	{
		double packetData = orgY;
		int nPt_y = 0;
		if( GetVertScaleAbsolutePosition_y(region, packetData, drawingVertScaleData.GetDisplayDataMinMax(), bIsLog, nPt_y))
		{
			// �䴫�� �ߴ´�.
			if(region.bottom >= nPt_y){
				DrawScaleLine(pDC, region, nPt_y, bIsInvert, (MARGIN * 2), MARGIN);
			}
			// �ܴ��� �ߴ´�.
			if(region.bottom >= nPt_y){
				DrawScaleLine(pDC, region, nScaleLineCount, drawingVertScaleData.GetDisplayDataMinMax(), packetData, dGap, bIsLog, bIsInvert, true);
			}
		}
	}
	else for(int nIndex = nstart; nIndex < nend; nIndex++)
	{
		double packetData = orgY + (double)dGap * (double)nIndex;
		int nPt_y = 0;
		if(!GetVertScaleAbsolutePosition_y(region, packetData, drawingVertScaleData.GetDisplayDataMinMax(), bIsLog, nPt_y))
			continue;

		// �䴫�� �ߴ´�.
		if(region.bottom >= nPt_y){
			DrawScaleLine(pDC, region, nPt_y, bIsInvert, (MARGIN * 2), MARGIN);
		}
		// �ܴ��� �ߴ´�.
		if(region.bottom >= nPt_y){
			DrawScaleLine(pDC, region, nScaleLineCount, drawingVertScaleData.GetDisplayDataMinMax(), packetData, dGap, bIsLog, bIsInvert, true);
		}
	}

	// orgY �� Min ���� �ٸ� ��� orgY ���� min�������� ���� ó��
	DrawScaleLine(pDC, region, nScaleLineCount, drawingVertScaleData.GetDisplayDataMinMax(), orgY, dGap, bIsLog, bIsInvert, false);

	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}


// private =========================================================================
// �������� data ���ϱ�
double CVertScaleDrawer::GetRealData(const bool bIsLog, const double& dData) const
{
	if(bIsLog)
		return CMath::Log(dData);
	return dData;
}

// scale ���� ���ݱ��ϱ�
int CVertScaleDrawer::GetVertScaleHight(CDC* pDC) const
{
	CSize textSize = pDC->GetTextExtent("0");
	return (textSize.cy * 3);
}

// scale text�� �׸��� ��ġ
int CVertScaleDrawer::GetVertScalePosition_x(const int right_pos, const int dataLength) const
{
	return (right_pos - dataLength);
}

//sy 2003.6.25. -> �ܴ��� ����
int CVertScaleDrawer::GetScaleLineCount(const double& dGap) const
{
	int nGap2 = (int)dGap;

	int scalegap = 1;
	//sy 2002.8.21.
	//�ܴ��� �׸��� �ʰ� �ϱ�.
	/*if(nGap2 % 10 == 0)
		scalegap = 2;
	else
		scalegap = 1;*/
	if(nGap2 % 500 == 0)
		scalegap = 5;
	else if(nGap2 % 400 == 0)
		scalegap = 4;
	else if(nGap2 % 300 == 0)
		scalegap = 3;
	else if(nGap2 % 100 == 0)
		scalegap = 2;
	else if(nGap2 % 50 == 0)
		scalegap = 5;
	else
		scalegap = 1;

	return scalegap;
}

// ----------------------------------------------------------------------------
// scale text�� ����
int CVertScaleDrawer::GetCount(CDC* pDC, const int height) const
{
	int scaleHight = GetVertScaleHight(pDC);
	if(scaleHight <= 0.0)
		return 0;
	return CMath::Round((double)height/(double)scaleHight);
}

void CVertScaleDrawer::GetStartAndEnd(const CDisplayDataMinMax& displayDataMinMax, const int totalCount, const double& dGap, int& start, int& end) const
{
	if(displayDataMinMax.GetDisplayMin() < 0 && displayDataMinMax.GetDisplayMax() > 0)
	{
		// (2006/11/19 - Seung-Won, Bae) Enable microscopic scale.
		//		So. Support 0.0 Gap.
		if(dGap > 0)
		{
			start = CMath::Round(displayDataMinMax.GetDisplayMin() / dGap -1);
			end = CMath::Round(displayDataMinMax.GetDisplayMax() / dGap +1);
		}
		else
		{
			start = totalCount;
			end = 0;
		}
		if(start * end >= 0)
		{
			end = abs(end - start);
			start = 0 ;
		}
	}
	else
		end = totalCount;
}

// (2009/12/8 - Seung-Won, Bae) Comment and Adjusting
// 0. dataType�� ����, ��ȿ�ڸ��� 1�� �ڸ��� �ǵ��� dGapMM�� ������Ű��, �� ����ġ�� dGapInGap���� ��ȯ�Ѵ�.
void CVertScaleDrawer::GetGapEnvironment( const CString& dataType, double& dGapMM, double& dGapInGap) const
{
	if( dGapMM > 20.0) return;

	//sy 2002.8.29.  -> dGap �� �Ҽ��϶�..
	int nDecimalDownCount = 0;	// �Ҽ��� �ڸ��� Ȯ��.
	if( !dataType.IsEmpty() && dataType.Find('.') != -1)
			nDecimalDownCount = CDataConversion::GetDecimalDownCount( dataType);
	else	nDecimalDownCount = CDataConversion::GetDecimalDownCount( CDataConversion::DoubleToString( dGapMM, false));
	
	dGapInGap = pow( 10, nDecimalDownCount);
	if( dGapInGap > 1) dGapMM *= dGapInGap;
}

bool CVertScaleDrawer::GetGap(const CDisplayDataMinMax& displayDataMinMax, const CString& dataType, const int totalCount, double& dGap) const
{
	if(totalCount <= 0)
		return false;
	
	double dGapMM = displayDataMinMax.GetCount();
	double dGapInGap = 1.0;
	GetGapEnvironment(dataType, dGapMM, dGapInGap);

	dGap = CMath::Round(dGapMM / totalCount, 1);
	CString strGap = CDataConversion::DoubleToString(dGap, 0);
	dGap = CMath::Round(dGap, strGap.GetLength() - 1);

	// �ݿø� �� ���� 5, 10 ������ �����Ѵ�
	//sy 2002.8.29.  -> dGap �� �Ҽ��϶�..
	if(dGapInGap > 1.0 && dGapMM < 1.0);
	//sy 2002.10.29. -> ������ ū��쿡�� gap�� ���� �д�.
	else if(dGap <= 5){
		double dMinMaxGap = displayDataMinMax.GetCount();
		if(dMinMaxGap <= 5){
			if(dMinMaxGap > 3 && totalCount < 5 && displayDataMinMax.GetDisplayMin() > 100)
				dGap = 1;
			else
				dGap = 5;
		}
		else{
			if(dMinMaxGap <= 40)
				dGap = 5;
			else
				dGap = 10;

			//sy 2003.6.25. -> ���� �����ϰ� ǥ��
			// packet type�� �Ҽ����� �ִ� ���
			if(!dataType.IsEmpty() && dataType.Find('.') >= 0){
				int nCount = (int)(dMinMaxGap / dGap);
				if(dGapInGap > 1)
					nCount = (int)(dMinMaxGap / (dGap / dGapInGap));

				int nMinCount = CMath::Round((double)totalCount / 3.0 * 2.0);
				if(nMinCount > nCount){
					dGap = dGap / 2;
				}
			}
		}
	}
	else if(dGap <= 10) 
		dGap = 10;
	else{
		//sy 2002.11.13.
		double dPowData = pow(10, strGap.GetLength() - 1);
		int nTemp = CMath::Round(dGap/dPowData);
		if(nTemp == 1)
			dGap = dPowData;
		// (2006/12/28 - Seung-Won, Bae) Deny the Overlapping with other scale Text.
		else if(nTemp <= 2)
			dGap = pow(10, strGap.GetLength()) / 5;
		else if(nTemp <= 5)
			dGap = pow(10, strGap.GetLength()) / 2;
		else if(nTemp <= 10)
			dGap = pow(10, strGap.GetLength());
	}

	//aa
	//sy 2002.8.29.  -> dGap �� �Ҽ��϶�..
	if(dGapInGap > 1)
		dGap /= dGapInGap;
	
	//sy 2002.9.9.  -> dGap�� �ʹ� ���� ���
	if(displayDataMinMax.GetCount() < dGap)
		dGap = displayDataMinMax.GetCount();
	else if(displayDataMinMax.GetCount() < dGap * 3 && dGap == 10)
		dGap = 5;

	return true;
}

// packet type�� ���� data �ݿø��� �ϱ� ���� rounding �ڸ� ���ϱ�
int CVertScaleDrawer::GetRoundIndexDecimalDown(const CString& strPacketType) const
{
	int nRoundIndex = CDataConversion::GetDecimalDownCount(strPacketType);
	if(nRoundIndex > 0)
		nRoundIndex++;

	return nRoundIndex;
}

//sy 2003.6.25. -> ���� �ϴܿ� �Ⱥ��̴� scale ���̰��ϱ� ���ؼ�...
// scale text�� �׸����� ����
// text ��ġ�� type ��ġ���� ũ�ų� ���� ��쿡�� scale text�� ����.
// frame �� ������ graph ����(title �� dataview ����ǥ��) �ȿ� �����ؾ� �Ѵ�.
bool CVertScaleDrawer::IsScaleDataDrawing(const CRect& maxRegion, const int nTypePosition, const int nDataPosition) const
{
	if(maxRegion.top >= nDataPosition || maxRegion.bottom <= nDataPosition)
		return false;

	return (nTypePosition >= nDataPosition);
}

// ----------------------------------------------------------------------------
// min, max�� ��ȣ�� (+, -) �ٸ� ���
// ȣ������, ���ذ� ��� - ojtaso (20070917)
void CVertScaleDrawer::DrawTextOrGrid_PlusAndMinusSign(CDC* pDC, const double orgY, const int typeRegionY, 
		const int start, const int end, const double& dGap, const bool bround, const CDrawingVertScaleData& drawingVertScaleData, const CVerticalScale& vertScale, 
		const CRect& noFrameGraphRegion, const CRect& region1, const CRect& region2, const CString& dataOrgType, const CString& dataViewType, const bool bIsSignDraw,
		const double dStandardValue)
{
	int newStart = start, newEnd = end;
	// newStart : -  newEnd : +
	if(start > 0){
		newStart = end;
		newEnd = start;
	}

	DrawTextOrGrid_PlusOrMinusSign(pDC, orgY, typeRegionY, 0, newEnd, dGap, bround, 
		drawingVertScaleData, vertScale, noFrameGraphRegion, region1, region2, 
		dataOrgType, dataViewType, bIsSignDraw, dStandardValue);
	DrawTextOrGrid_PlusOrMinusSign(pDC, orgY, typeRegionY, newStart, 0, dGap, bround, 
		drawingVertScaleData, vertScale, noFrameGraphRegion, region1, region2, 
		dataOrgType, dataViewType, bIsSignDraw, dStandardValue);
}

// min, max�� ��ȣ�� (+, -) ���� ���
// ȣ������, ���ذ� ��� - ojtaso (20070917)
// (2009/12/8 - Seung-Won, Bae) Comment and Adjusting
void CVertScaleDrawer::DrawTextOrGrid_PlusOrMinusSign(CDC* pDC, const double orgY, const int typeRegionY, const int start, const int end, 
		const double& dGap, const bool bround, const CDrawingVertScaleData& drawingVertScaleData, const CVerticalScale& vertScale, 
		const CRect& noFrameGraphRegion, const CRect& region1, const CRect& region2, const CString& dataOrgType, const CString& dataViewType, const bool bIsSignDraw,
		const double dStandardValue)
{
	// vntsorl:[0000268] ��Ʈ ȯ�� �������� ���ϱ��� pip���� �����ϸ� y���� pip ��� �����Ͱ� �� �� ������ ����
	// ȣ������, ���ذ� ��� - ojtaso (20070917)
	BOOL bHogaUnit = vertScale.GetVertUserHogaScaleType();

	double y = orgY;
	int nstart = start, nend = end;
	if( start * end < 0)
	{
		nend = abs(start - end);
		nstart = 0;
	}

	if(!bHogaUnit)
	{
		//sy 2002.8.29.  -> dGap �� �Ҽ��϶�..
		double dGapMM = dGap;
		double dGapInGap = 1.0;
		GetGapEnvironment(dataViewType, dGapMM, dGapInGap);
		if(dGapInGap > 1)
			y *= dGapInGap;
		//end
		
		int nY = (int)CMath::Floor(y, 0);
		int nGapMM = (int)CMath::Floor(dGapMM, 0);
		if(nGapMM == 0)
			nGapMM = 1;
		
		if(nY % nGapMM != 0)	{
			if(y < 0)
				y -= dGapMM + (nY % nGapMM);
			else
				y += dGapMM - (nY % nGapMM);
		}
		
		// (2007/2/16 - SKBng-Won, Bae) Why check it? It can be negative number.
		//	if(y > 10)
		y = CMath::Floor(y, 0);
		
		//sy 2002.8.29.  -> dGap �� �Ҽ��϶�..
		if(dGapInGap >= 1)
			y /= dGapInGap;
		//end
	}

	// �ִ밪
	// (2006/10/27 - Seung-Won, Bae) Support One Data Scale
	double max = drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMax();
	double min = drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin();
	if( y > max) y = max;
	if( nstart == nend)
	{
		if( max == min) nend = nstart - 1;	// for No Data
		else y = ( max + drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin()) / 2;
	}

	// ���� �� �ڸ��� ���ϱ�
	int nRoundLength = 0;
	if(!dataViewType.IsEmpty() && dataViewType.Find(".") == -1)
	{
		//sy 2004.06.16.
		// �⺻ type �� "x10" �̻��� ��� round �� count�� ���� type ���� �⺻ type �� ����� �Ѵ�.
		// ��, �⺻ type = "x10" �̰� TR data �� "100" �� ���� ���� �ǹ��ϴ� ���� "1000" �̱� �����̴�.!!
		int nTemp = 0;
		if(!dataOrgType.IsEmpty() && dataOrgType.Find(".") == -1)
		{
			nTemp = CDataConversion::GetStringDataCount(dataOrgType, "0");
		}
		nRoundLength = CDataConversion::GetStringDataCount(dataViewType, "0");
		nRoundLength -= nTemp;
		//sy end
	}

	// invert-scale���� Ȯ���Ѵ�
	bool bInvert = drawingVertScaleData.GetVertScaleInvertType();

	// ���� �׸��� add 02.06.2002
	DrawScale(pDC, region1, dGap, y, nstart, nend, drawingVertScaleData);
	DrawScale(pDC, region2, dGap, y, nstart, nend, drawingVertScaleData);

	//sy 2002.9.11. ������ Ŭ��쿣 �Ҽ����� ǥ�� ���Ѵ�.
	/*bool bRound = false;
	CString strMax = CVertScaleDrawer::GetStrData(max, dataType, bround, nRoundLength);
	CString strMin = CVertScaleDrawer::GetStrData(drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin(), dataType, bround, nRoundLength);
	strMax.TrimLeft('-');
	strMin.TrimLeft('-');
	if(strMax.GetLength() > 7 || strMin.GetLength() > 7)
		bRound = true;
	//end*/
	//sy 2003.01.09. ��ǥ ���ҽ� ȭ�鿡 ǥ���� ����Ÿ�� ��ǥ ���ϱ�
	// ex) packetData = 12.8900 �ε� ǥ���� �����Ͱ� 12 ��� �ϸ� 12�� ��ǥ ���Ѵ�.
	CString strOrgData;
	int nRoundIndex = GetRoundIndexDecimalDown(dataViewType);
	//end

	// (2006/10/27 - Seung-Won, Bae) Support One Data Scale
	if( nstart == nend)
	{
		//sy 2003.01.09.
		//double packetData = y/*orgY*/ + dGap * (double)count;
		double packetData = CMath::Round( y, nRoundIndex);
		//end

		int nowPt_y = 0;
		if( GetVertScaleAbsolutePosition_y(region1, packetData, drawingVertScaleData.GetDisplayDataMinMax(), false, nowPt_y))
		{
			if( IsScaleDataDrawing(noFrameGraphRegion, typeRegionY, nowPt_y))
			{
				CString strData = CVertScaleDrawer::GetStrData(packetData, dataViewType, bround, nRoundLength);
				//sy 2003.7.21.
				// ��ȣ�� �׸��� �ʴ´�.
				if(!bIsSignDraw){
					strData.TrimLeft('-');
				}

				//if(strData.GetLength() > 7 && strData.Find(".") > 0)
				if(/*bRound &&*/ bround && strData.Find(".") > 0)
					strData = strData.Left(strData.Find("."));
				
				// sy 2003.01.09.
				if( !strData.IsEmpty() && strData != strOrgData)
				{
					strOrgData = strData;
					CSize textSize = pDC->GetTextExtent(strData);
					int pt_y = nowPt_y;
					// invert�̸� �����´�
					if(bInvert)
						pt_y = CDataConversion::InvertGraphDataPt(region1, nowPt_y);

					CVertScaleDrawer::DrawTextOut(pDC, 
						CVertScaleDrawer::GetTextPosition(region1.right, pt_y, textSize), 
						strData, drawingVertScaleData.GetScaleColor().GetScaleTextColor());
					if(region2.IsRectEmpty() == FALSE){				
						CVertScaleDrawer::DrawTextOut(pDC, 
							CVertScaleDrawer::GetTextPosition(region2.right, pt_y, textSize), 
							strData, drawingVertScaleData.GetScaleColor().GetScaleTextColor());
					}
					CVertScaleDrawer::DrawGrid(pDC, drawingVertScaleData, vertScale.GetGridType(), pt_y, vertScale.GetActiveFlag());
				}
			}
		}
	}
	else
	{
		for(int count = nstart; count < nend; count++)
		{
			double packetData = y + dGap * ( double)count;

			int nowPt_y = 0;
			if(!GetVertScaleAbsolutePosition_y(region1, packetData, 
				drawingVertScaleData.GetDisplayDataMinMax(), false, nowPt_y))
				continue;

			if(!IsScaleDataDrawing(noFrameGraphRegion, typeRegionY, nowPt_y))
				continue;

			CString strData = CVertScaleDrawer::GetStrData(packetData, dataViewType, bround, nRoundLength);
			//sy 2003.7.21.
			// ��ȣ�� �׸��� �ʴ´�.
			if(!bIsSignDraw){
				strData.TrimLeft('-');
			}

			//if(strData.GetLength() > 7 && strData.Find(".") > 0)
			if(/*bRound &&*/ bround && strData.Find(".") > 0)
				strData = strData.Left(strData.Find("."));
			
			// sy 2003.01.09.
			if(strData.IsEmpty() || strData == strOrgData)
				continue;

			strOrgData = strData;
			CSize textSize = pDC->GetTextExtent(strData);
			int pt_y = nowPt_y;
			// invert�̸� �����´�
			if(bInvert)
				pt_y = CDataConversion::InvertGraphDataPt(region1, nowPt_y);

			CVertScaleDrawer::DrawTextOut(pDC, 
				CVertScaleDrawer::GetTextPosition(region1.right, pt_y, textSize), 
				strData, drawingVertScaleData.GetScaleColor().GetScaleTextColor());
			if(region2.IsRectEmpty() == FALSE){				
				CVertScaleDrawer::DrawTextOut(pDC, 
					CVertScaleDrawer::GetTextPosition(region2.right, pt_y, textSize), 
					strData, drawingVertScaleData.GetScaleColor().GetScaleTextColor());
			}
			CVertScaleDrawer::DrawGrid(pDC, drawingVertScaleData, vertScale.GetGridType(), pt_y, vertScale.GetActiveFlag());
		}
	}
	// write min & max
//	WriteValueMinMax(pDC, region1, bround, dataType, drawingVertScaleData);
//	WriteValueMinMax(pDC, region2, bround, dataType, drawingVertScaleData);
}

// ----------------------------------------------------------------------------
// ȭ��� �ִ� �ּҰ��� ���´�
// 2.14.2002
void CVertScaleDrawer::WriteValueMinMax(CDC * pDC, const CRect region, const bool bround, const CString& dataType, const CDrawingVertScaleData& drawingVertScaleData)
{
	CPacket *pPacket = drawingVertScaleData.GetPacket();
	if( region.IsRectEmpty() || !pPacket) return;

	double packetData = drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMax();
	int nowPt_y = 0;
	// ���� �� �ڸ��� ���ϱ�
	CString strMax, strMaxCutoff, strFormatTrans;
	BOOL bAutoScaleType = pPacket->IsAutoScaleType();
	int ndatalength = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTransCutoff(packetData, dataType, strMaxCutoff, strFormatTrans, bAutoScaleType);
	strMax = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString(packetData), dataType);
	pPacket->SetScaleType( bAutoScaleType ? strMax : pPacket->GetType());

	int nRoundLength = strMax.GetLength() - (ndatalength > 3 ? ndatalength + 1 : ndatalength);
	// max
	GetVertScaleAbsolutePosition_y(region, packetData, drawingVertScaleData.GetDisplayDataMinMax(), 
		drawingVertScaleData.GetVertScaleLogType(), nowPt_y);
	int pt_y = nowPt_y;
	CString strData = CVertScaleDrawer::GetStrData(packetData, dataType, bround, nRoundLength);
	CSize textSize = pDC->GetTextExtent(strData);
	CPoint point = CVertScaleDrawer::GetTextPosition(region.right, pt_y, textSize);					
	CVertScaleDrawer::DrawTextOut(pDC, 
				CRect(point, CPoint(region.right, pt_y + textSize.cy)),
				strData, RGB(255, 200, 200),
				drawingVertScaleData.GetScaleColor().GetScaleTextColor());
	// min
	packetData = drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin();
	if(packetData != 0)	{
		GetVertScaleAbsolutePosition_y(region, packetData, drawingVertScaleData.GetDisplayDataMinMax(), 
			drawingVertScaleData.GetVertScaleLogType(), nowPt_y);
		pt_y = nowPt_y;
		strData = CVertScaleDrawer::GetStrData(packetData, dataType, bround, nRoundLength);
		textSize = pDC->GetTextExtent(strData);
		point = CVertScaleDrawer::GetTextPosition(region.right, pt_y, textSize);					
		CVertScaleDrawer::DrawTextOut(pDC, 
					CRect(point, CPoint(region.right, pt_y + textSize.cy)),
					strData, RGB(200, 200, 255),
					drawingVertScaleData.GetScaleColor().GetScaleTextColor());
	}
}

// scale ���� �ߴ´�
void CVertScaleDrawer::DrawScaleLine(CDC* pDC, const CRect& region, const int nScaleLineCount, 
		const CDisplayDataMinMax& displayDataMinMax, const double dOrgY, const double& dGap, 
		const bool bIsLog, const bool bIsInvert, const bool bIsIncrease)
{
	// orgY �� Min ���� �ٸ� ��� orgY ���� min�������� ���� ó��
	for(int nIndex = 1 ; nIndex < nScaleLineCount ; nIndex++){
		double dData = dOrgY;
		if(bIsIncrease)
			dData += (nIndex * (dGap / (double)nScaleLineCount));
		else
			dData -= (nIndex * (dGap / (double)nScaleLineCount));

		int nPt_y = 0;
		if(!GetVertScaleAbsolutePosition_y(region, dData, displayDataMinMax, bIsLog, nPt_y))
			continue;

		DrawScaleLine(pDC, region, nPt_y, bIsInvert, (MARGIN * 2), (MARGIN * 3 / 2));
	}
}

void CVertScaleDrawer::DrawScaleLine(CDC* pDC, const CRect& region, const int nPt_y, 
		const bool bIsInvert, const int nGap1, const int nGap2)
{
	int realPt_y = nPt_y;
	// invert�̸� �����´�
	if(bIsInvert)
		realPt_y = CDataConversion::InvertGraphDataPt(region, nPt_y);
	
	pDC->MoveTo(region.left - nGap1, realPt_y);
	pDC->LineTo(region.left - nGap2, realPt_y);
	pDC->MoveTo(region.right + nGap2, realPt_y);
	pDC->LineTo(region.right + nGap1, realPt_y);
}
bool CVertScaleDrawer::GetBround(const CDisplayDataMinMax& displayDataMinMax) const
{
	//sy 2002.8.29. -> �鸸���� �̻� ���� ��ƾ
	double dMax = displayDataMinMax.GetDisplayMax();
	if(dMax < fabs(displayDataMinMax.GetDisplayMin()))
		dMax = displayDataMinMax.GetDisplayMin();

	if(CDataConversion::DoubleToString(dMax, 0).GetLength() >= 7)
		return true;
	return false;
}
