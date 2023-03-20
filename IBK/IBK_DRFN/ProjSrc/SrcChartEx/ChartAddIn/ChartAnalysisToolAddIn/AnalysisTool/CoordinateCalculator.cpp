// CoordinateCalculator.cpp: implementation of the CCoordinateCalculator class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoordinateCalculator.h"

#include <assert.h>

#include "../Include_Chart/Dll_Load/IMetaTable.h"						// for _MTEXT()
#include "../Include_Chart/Conversion.h"								// for IPacketListManager
#include "../Include_AddIn_133101/I133101/_IPacketListManager.h"		// for IPacketListManager
#include "../Include_Analysis/CoordinateCalculatorData.h"
#include "Coordinate.h"
#include "AnalysisData.h"
#include "DataMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CCoordinateCalculator

// ���� ��ǥ(Absolute) -> ��� ��ǥ(Relative) : ������ ���� ��ǥ
CCoordinate CCoordinateCalculator::AbsoluteToRelative(const CCoordinateCalculatorData& calData, const CPoint& point) const
{
	CCoordinate cdResult;
	if( !calData.IsValidAddInInterface()) return cdResult;

	BOOL IsStringDataType = calData.m_pIPacketListManager->IsBasePacketString();
	if( IsStringDataType)
	{
		IPacket *pIPacket = calData.GetPacket_X();
		if( !IsCalculator( pIPacket)) cdResult = CCoordinate(point);
		else
		{
			int nIndex = CDataConversion::GetPacketDataIndex_Date(calData.GetRegion(), point, 
						calData.GetDisplayDataStart(), calData.GetDisplayDataEnd());
			//nIndex = min(calData.GetDisplayDataEnd()+1,nIndex);
			double dX = 0.0;
			if( pIPacket) pIPacket->GetDataPastandFuture(nIndex, dX);
			double dY = GetData_Y(calData, point, nIndex);
			// ����Ʈ �����ð� ���� - ojtaso (20080811)
			ILPCSTR szPacketType = pIPacket->GetType();
			CString strPacketType( szPacketType);
			// 20081007 JS.Kim	����ڽð��� ����
			cdResult = CCoordinate(dX, dY, pIPacket->GetBaseTimeDifference(), strPacketType, nIndex); //0
		}
		if( pIPacket) pIPacket->Release();
		return cdResult;
	}

	// XScale : ���� Interface�� ��� - vntsorl(20070410)
	IPacket *pIPacket = calData.GetPacket_X();
	if( !pIPacket) return cdResult;
	ILPCSTR szPacketType = pIPacket->GetType();
	CString strPacketType( szPacketType);
	// ����Ʈ �����ð� ���� - ojtaso (20080811)
	// 20081007 JS.Kim	����ڽð��� ����

	// �����ҽ�.
	//if( !IsCalculator( pIPacket))	
	//	cdResult = CCoordinate(point);
	//else				
	//	cdResult = CCoordinate(GetData_X(calData, point, 0), GetData_Y(calData, point, 0), pIPacket->GetBaseTimeDifference(), strPacketType, 0);
	
	if( !IsCalculator( pIPacket))	
		cdResult = CCoordinate(point);
	else				
	{
		double dDataX = GetData_X(calData, point, 0);
		double dDataY = GetData_Y(calData, point, 0);
		int nTimeDiff = pIPacket->GetBaseTimeDifference();
		cdResult = CCoordinate(dDataX, dDataY, nTimeDiff, strPacketType, 0);
	}
	pIPacket->Release();
	return cdResult;
}

// ���� ��ǥ(Absolute) -> ��� ��ǥ(Relative) : ���� ������ ��ġ ������ ���� ��ǥ
CCoordinate CCoordinateCalculator::AbsoluteToRelativeAutoPrice(const CCoordinateCalculatorData& calData, const CPoint& point) const
{
	CCoordinate cdResult;
	if( !calData.IsValidAddInInterface()) return cdResult;

	if( calData.m_pIPacketListManager->IsBasePacketString())
	{
		double dX = 0.0;
		double dY = 0.0;
		int nIndex = CDataConversion::GetPacketDataIndex_Date(calData.GetRegion(), point, 
			calData.GetDisplayDataStart(), calData.GetDisplayDataEnd());
		
		IPacket *pIPacketX = calData.GetPacket_X();
		if( !IsCalculator( pIPacketX)) 
		{
			cdResult = CCoordinate(point);
			dX = cdResult.GetX();
		}
		else
		{		
			if( pIPacketX) pIPacketX->GetDataPastandFuture(nIndex, dX);			
		}
		if( pIPacketX) pIPacketX->Release();
		
		IPacket *pIPacketY = calData.GetPacket_Y();
		if( !IsCalculator( pIPacketY)) 
		{
			cdResult = CCoordinate(point);
			dY = cdResult.GetY();
		}
		else
		{		
			if( pIPacketY) pIPacketY->GetDataPastandFuture(nIndex, dY);			
		}
		if( pIPacketY) pIPacketY->Release();
		
		cdResult.SetX(dX,0);
		cdResult.SetY(dY);
		return cdResult;
	}

	// XScale : ���� Interface�� ��� - vntsorl(20070410)
	double dX = 0.0;
	double dY = 0.0;
	int nIndex = calData.m_pIChartManager->FindViewDataIndex_Pt("DEFAULT", point.x);	// DRI�� ���õǾ�� ���� ������ ����

	IPacket *pIPacketX = calData.GetPacket_X();
	if( !IsCalculator( pIPacketX)) 
	{
		cdResult = CCoordinate(point);
		dX = cdResult.GetX();
	}
	else
	{		
		dX = GetData_X(calData, point, 0);	
	}
	if( pIPacketX) pIPacketX->Release();

	IPacket *pIPacketY = calData.GetPacket_Y();
	if( !IsCalculator( pIPacketY)) 
	{
		cdResult = CCoordinate(point);
		dY = cdResult.GetY();
	}
	else
	{		
		pIPacketY->GetDataPastandFuture(nIndex, dY);			
	}
	if( pIPacketY) pIPacketY->Release();

	cdResult.SetX(dX,0);
	cdResult.SetY(dY);
	return cdResult;
}

// ��� ��ǥ -> ���� ��ǥ : ����� ��ǥ�� �̿��� �׸������� ��ǥ
CPoint CCoordinateCalculator::RelativeToAbsolute(const CCoordinateCalculatorData& calData, const CCoordinate& point) const
{
	CPoint ptResult;
	if( !calData.IsValidAddInInterface()) return ptResult;

	if( calData.m_pIPacketListManager->IsBasePacketString())
	{
		IPacket *pIPacket = calData.GetPacket_X();
		if( !IsCalculator( pIPacket)) ptResult = point.GetPixel();
		else
		{
			// X-scale ��(�ڷ�����)���� ���� Display�Ǵ� Data�� Index�� ����
			// ����� ���� Data�� ��� �þ
			// ����Ʈ �����ð� ���� - ojtaso (20080811)
			int nIndex = GetDisplayDataCurrentIndex( pIPacket, point.GetX(), point.GetDateType());
			// �̷������� �ִ� point�� ��� ���� Index�� �� point�� Index�� ���� �ٸ� ����� ����ó��
			if(nIndex != point.GetIndex())
			{
				// ���� ����� Y-scale �� �ش� Index �� �׻� ���� ������ X-scale �� 0���� ���� (�̷������� ���� ���� ���� ����)
				// �̷������� �ִ� point�� Data Index�� ������ X-scale ���� ����
				if(point.GetX() == 0. && point.GetIndex() > 0)
				{
					double dX = 0.0;
					if( pIPacket) pIPacket->GetDataPastandFuture(point.GetIndex(), dX);	

					nIndex = point.GetIndex();
					double dXPos = point.GetX();
					if(dXPos == 0. || dXPos != dX)
					{
						// const�� ���õǾ� �־ type casting�Ͽ� Data�� �ǽð����� ������Ŵ
						(const_cast<CCoordinate&>(point)).SetX(dX, nIndex);
					}
				}
				// ȯ�漳������ ��¥ ����� 0, 1, 2������ ���õǴ� Index�� �ٽ� ����
				else if(point.GetX() > 0. && (point.GetIndex() == 0 || point.GetIndex() == 1 || point.GetIndex() == 2))
				{
					(const_cast<CCoordinate&>(point)).SetIndex(nIndex);
				}
				// �������߰��θ��� Button�� �̿��� �����Ͱ��� ����ÿ� ����
				// X-Scale �� �� ������ �ε��� ����
				else if(nIndex > point.GetIndex() + 1)
				{
					double dX = 0.0;
					if( pIPacket) pIPacket->GetDataPastandFuture(nIndex, dX);	

					(const_cast<CCoordinate&>(point)).SetX(dX, nIndex);
				}
				// ���� �������� �� ���� �߼����� ���� �ִ� ��� �ð� ����� ���� 
				// X-Scale �� �� ������ �ε��� ����
				else
				{
					double dX = 0.0;
					if( pIPacket) pIPacket->GetDataPastandFuture(point.GetIndex(), dX);	

					nIndex = point.GetIndex();
					(const_cast<CCoordinate&>(point)).SetX(dX, nIndex);
				}
			}
			// ����� ���� �������� �̷������� �ִ� ���
			//2008.07.24 by LYH >> Index 0�� �̷����� �ƴ�. 0��° �ε������� �߼����� �׸��� ���� ����
			/*
			else if(nIndex == 0 && point.GetIndex() == 0)
			{
				double dX = 0.0;
				if( pIPacket)
				{
					nIndex = pIPacket->GetDataCount() - 1;
					pIPacket->GetDataPastandFuture(nIndex, dX);

					(const_cast<CCoordinate&>(point)).SetX(dX, nIndex);
				}
			}
			*/
			//2008.07.24 by LYH <<

			ptResult = CDataConversion::RelativeToAbsolute(calData.GetRegion(), 
					calData.GetDisplayDataStart(), calData.GetDisplayDataEnd(), 
					calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType(), 
					nIndex,	GetData_Y(calData, point.GetY(), nIndex));
		}
		if( pIPacket) pIPacket->Release();
		return ptResult;
	}
	
	// XScale : ���� Interface�� ��� - vntsorl(20070410)
	IPacket *pIPacket = calData.GetPacket_X();
	if( !IsCalculator( pIPacket)) 
		ptResult = point.GetPixel();
	else
	{
//>> ȣ����Ʈ�� �м����� �������󰡱� ���� ���� - vntsorl(20090202)
//		ILPCSTR szRQ = pIChartManager->GetMainRQofBlock( calData.GetBlockIndex().GetRow(), calData.GetBlockIndex().GetColumn());
		ILPCSTR szRQ = calData.m_pIChartOCX->GetCurrentRQ();
		int nR = calData.GetBlockIndex().GetRow();
		int nC = calData.GetBlockIndex().GetColumn();
		int nV = 0, nG = -1;
		ILPCSTR szGraphName = calData.m_pIChartManager->FindNextIndicator( NULL, nR, nC, nV, nG, szRQ);
		if( szRQ.IsEmpty() || szGraphName.IsEmpty()
			|| !( nR == calData.GetBlockIndex().GetRow() && nC == calData.GetBlockIndex().GetColumn()))
			szRQ = calData.m_pIChartManager->GetMainRQofBlock( calData.GetBlockIndex().GetRow(), calData.GetBlockIndex().GetColumn());
		int nCenter_x;
		double dX = 0.0;
		dX = point.GetX();
		
		// 2011.01.20 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
		//int nXPos = calData.m_pIChartManager->GetXFromDouble(calData.GetBlockIndex().GetRow(), calData.GetBlockIndex().GetColumn(), dX, nCenter_x);

		CString strAnalType = point.GetDateType();
		ILPCSTR szPacketType = pIPacket->GetType();
		CString strPacketType( szPacketType);

		int nBlockRow = calData.GetBlockIndex().GetRow();
		int nBlockCol = calData.GetBlockIndex().GetColumn();
		int nXPos = calData.m_pIChartManager->GetXFromDouble(nBlockRow, nBlockCol, dX, nCenter_x);
		// 2011.01.20 by SYS <<

		int nIndex = calData.m_pIChartManager->FindViewDataIndex_Pt( szRQ, nXPos);
//<< ȣ����Ʈ�� �м����� �������󰡱� ���� ���� - vntsorl(20090202)
		ptResult = CDataConversion::RelativeToAbsolute(calData.GetRegion(), 
				calData.GetDisplayDataStart(), calData.GetDisplayDataEnd(), 
				calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType(), 
				nIndex,	GetData_Y(calData, point.GetY(), nIndex));
		ptResult.x = nXPos;
	}
	if( pIPacket) pIPacket->Release();
	return ptResult;
}

int CCoordinateCalculator::RelativeToAbsolute_Y(const CCoordinateCalculatorData& calData,int nIndex,const double& dYValue) const
{
	int nYPos = -1;
	IPacket *pIPacket = calData.GetPacket_X();
	if( IsCalculator( pIPacket)) 
	{
		nYPos = CDataConversion::RelativeToAbsolute_Y(calData.GetRegion(), GetData_Y(calData, dYValue, nIndex),
				calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType());
	}
	if( pIPacket) pIPacket->Release();
	return nYPos;
}

// ���� ��ǥ -> ���� ��ǥ : �׸��� ���� ��ǥ
CPoint CCoordinateCalculator::AbsoluteToAbsolute(const CCoordinateCalculatorData& calData, const CPoint& point) const
{
	IPacket *pIPacket = calData.GetPacket_X();
	BOOL bResult = IsCalculator( pIPacket);
	if( pIPacket)
	{
		pIPacket->Release();
		pIPacket = NULL;
	}
	if( !bResult) return point;

	CCoordinate relativePt = AbsoluteToRelative(calData, point);
	CPoint drawingPoint = RelativeToAbsolute_NoAutoPrice(calData, relativePt);
	return drawingPoint;
}

// �������� ����
CCoordinate CCoordinateCalculator::GetGap(const CCoordinateCalculatorData& calData, const CPoint& point1, const CPoint& point2) const
{
	CCoordinate cdResult;
	if( !calData.IsValidAddInInterface()) return cdResult;

	IPacket *pIPacket = calData.GetPacket_X();
	ILPCSTR szPacketType = pIPacket->GetType();
	CString strPacketType( szPacketType);
	if( calData.m_pIPacketListManager->IsBasePacketString())
	{
		// 20081007 JS.Kim	����ڽð��� ����
		if( !IsCalculator( pIPacket)) cdResult = CCoordinate((double)(point2.x - point1.x), (double)(point2.y - point1.y), pIPacket->GetBaseTimeDifference(), strPacketType, 0);
		else
		{
			CCoordinate relativePt1 = AbsoluteToRelative(calData, point1);
			CCoordinate relativePt2 = AbsoluteToRelative(calData, point2);
			int nIndex = GetDisplayDataCurrentIndex( pIPacket, relativePt2.GetX())
				- GetDisplayDataCurrentIndex(pIPacket, relativePt1.GetX());
			// ����Ʈ �����ð� ���� - ojtaso (20080811)
			cdResult = CCoordinate( ( double)nIndex, (relativePt2.GetY() - relativePt1.GetY()), pIPacket->GetBaseTimeDifference(), strPacketType, 0);
		}
	}
	else
	{
		// packet �� ���� ��쿣 �׳� ��ǥ�� �Ѵ�!!
		// ����Ʈ �����ð� ���� - ojtaso (20080811)
		// 20081007 JS.Kim	����ڽð��� ����
		if( !IsCalculator( pIPacket)) cdResult = CCoordinate((double)(point2.x - point1.x), (double)(point2.y - point1.y), pIPacket->GetBaseTimeDifference(), strPacketType, 0);
		else
		{
			// XScale : ���� Interface�� ��� - vntsorl(20070327)
			CCoordinate AbsolutePt1 = AbsoluteToAbsolute(calData, point1);
			CCoordinate AbsolutePt2 = AbsoluteToAbsolute(calData, point2);
			CCoordinate relativePt1 = AbsoluteToRelative(calData, point1);
			CCoordinate relativePt2 = AbsoluteToRelative(calData, point2);
			// ����Ʈ �����ð� ���� - ojtaso (20080811)
			cdResult = CCoordinate( ( AbsolutePt2.GetX() - AbsolutePt1.GetX()), (relativePt2.GetY() - relativePt1.GetY()), pIPacket->GetBaseTimeDifference(), strPacketType, 0);
		}
	}

	if( pIPacket) pIPacket->Release();
	return cdResult;
}

// ���� point�� index�� ��������.
int CCoordinateCalculator::GetCurrentDataIndex(const CRegion& region, const CPoint& point) const
{
	// XScale : ���� Interface�� ��� - vntsorl(20070327)
	return CDataConversion::GetPacketDataIndex_Date(region.GetRect(), point, 
			region.GetDisplayAttributes().GetDataStartInBlock(), 
			region.GetDisplayAttributes().GetDataEndInBlock());
//	CCoordinateCalculatorData calData;
//	CPoints points = GetPoints(point, calData);
//	return ->FindViewDataIndex_Pt(point.x);
}

// �׸� x��ǥ ��������.
int CCoordinateCalculator::GetDrawingPoint_X(const CRegion& region, const int nCurrentIndex) const
{
	// XScale : ���� Interface�� ��� - vntsorl(20070327)
	return CDataConversion::RelativeToAbsolute_X(region.GetRect(), 
			region.GetDisplayAttributes().GetDataStartInBlock(), 
			region.GetDisplayAttributes().GetDataEndInBlock(), nCurrentIndex);
//	CCoordinateCalculatorData calData;
//	CPoints points = GetPoints(point, calData);
//	return ->GetXPosition(nCurrentIndex, 0);
}

// private ====================================================================
bool CCoordinateCalculator::IsCalculator(IPacket* pPacket) const
{
	return (pPacket != NULL && pPacket->GetDataCount() > 0);
}

// ----------------------------------------------------------------------------
// ����Ʈ �����ð� ���� - ojtaso (20080811)
int CCoordinateCalculator::GetDisplayDataCurrentIndex(IPacket* pPacket, const double dData, const CString strType/* = _T("")*/) const
{
	if(pPacket == NULL)
		return -1;
	
	return pPacket->GetIndexFromData(dData, strType);
}

// ��� ��ǥ -> ���� ��ǥ : ����� ��ǥ�� �̿��� �׸������� ��ǥ
CPoint CCoordinateCalculator::RelativeToAbsolute_NoAutoPrice(const CCoordinateCalculatorData& calData, const CCoordinate& point) const
{
	CPoint ptResult;
	if( !calData.IsValidAddInInterface()) return ptResult;

	IPacket *pIPacket = calData.GetPacket_X();
	if( calData.m_pIPacketListManager->IsBasePacketString())
	{
		if(!IsCalculator( pIPacket)) ptResult = point.GetPixel();
		else
		{
			int nIndex = GetDisplayDataCurrentIndex( pIPacket, point.GetX());
			ptResult = CDataConversion::RelativeToAbsolute(calData.GetRegion(), 
				calData.GetDisplayDataStart(), calData.GetDisplayDataEnd(), 
				calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType(), 
				nIndex,	calData.ApplyLog(point.GetY()));
		}
	}
	else
	{
		// XScale : ���� Interface�� ��� - vntsorl(20070410)
		if(!IsCalculator( pIPacket)) ptResult = point.GetPixel();
		else
		{
			int nCenter_x;
			double dX = 0.0;
			dX = point.GetX();
			int nXPos = calData.m_pIChartManager->GetXFromDouble( calData.GetBlockIndex().GetRow(), calData.GetBlockIndex().GetColumn(), dX, nCenter_x);
			int nIndex = calData.m_pIChartManager->FindViewDataIndex_Pt("DEFAULT", nXPos);
			ptResult = CDataConversion::RelativeToAbsolute(calData.GetRegion(), 
					calData.GetDisplayDataStart(), calData.GetDisplayDataEnd(), 
					calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType(), 
					nIndex,	calData.ApplyLog(point.GetY()));
			ptResult.x = nXPos;
		}
	}

	if( pIPacket) pIPacket->Release();
	return ptResult;
}


///////////////////////////////////////////////////////////////////////////////
// class CCoordinateCalculator_AutoPrice

// protected ==================================================================
// XScale : ���� Interface�� ��� - vntsorl(20070410)
double CCoordinateCalculator_AutoPrice::GetData_X(const CCoordinateCalculatorData& calData, const CPoint& point, const int /*nIndex*/) const
{
	double dResult = 0.0;
	if( !calData.IsValidAddInInterface()) return dResult;

	int nBlockColumn, nCenter_x;
	int nRow, nCol;
	calData.m_pIChartManager->GetBlockIndex( point, nRow, nCol);
	
//>> ȣ����Ʈ�� �м����� �������󰡱� ���� ���� - vntsorl(20090202)
	ILPCSTR szRQ = calData.m_pIChartOCX->GetCurrentRQ();
	int nR = nRow;
	int nC = nCol;
	int nV = 0, nG = -1;
	ILPCSTR szGraphName = calData.m_pIChartManager->FindNextIndicator( NULL, nR, nC, nV, nG, szRQ);
	if( szRQ.IsEmpty() || szGraphName.IsEmpty() || !( nR == nRow && nC == nCol))
		szRQ = calData.m_pIChartManager->GetMainRQofBlock( nRow, nCol);
//	dResult = pIChartManager->GetDoubleFromX(nRow, nCol, point.x, &nBlockColumn, nCenter_x);
	dResult = calData.m_pIChartManager->GetDoubleFromX( nRow, nCol, point.x, &nBlockColumn, nCenter_x, szRQ);
//<< ȣ����Ʈ�� �м����� �������󰡱� ���� ���� - vntsorl(20090202)

	return dResult;
}

double CCoordinateCalculator_AutoPrice::GetData_Y(const CCoordinateCalculatorData& calData, const CPoint& point, const int /*nIndex_*/) const
{
	if( !calData.IsValidAddInInterface()) return -HUGE_VAL;

	int nIndex = calData.m_pIChartManager->FindViewDataIndex_Pt( calData.GetRQ(), point.x, calData.GetBlockIndex().GetColumn(), FALSE);
	IPacket *pIPacket = calData.GetPacket_Y();

	double dY = 0.0;
	if( pIPacket && IsAutoPriceCalculator( pIPacket, nIndex))
			pIPacket->GetData( nIndex, dY);
	else	dY = CDataConversion::AbsoluteToRelative_Y( calData.GetRegion(), point.y, 
				calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType());
	
	if( pIPacket) pIPacket->Release();

	// (2007/5/10 - Seung-Won, Bae) Check No Y Scale Min/Max.
	if( -HUGE_VAL == dY) return -HUGE_VAL;

	return calData.ApplyExp(dY);
}

double CCoordinateCalculator_AutoPrice::GetData_Y(const CCoordinateCalculatorData& calData, const double& dData, const int nIndex) const
{
	IPacket *pIPacket = calData.GetPacket_Y();
	double dY = dData;
	if( pIPacket && IsAutoPriceCalculator( pIPacket, nIndex)) pIPacket->GetData( nIndex, dY);
	if( pIPacket) pIPacket->Release();
	return calData.ApplyLog(dY);
}

// private ====================================================================
bool CCoordinateCalculator_AutoPrice::IsAutoPriceCalculator(IPacket* pPacket, const int nIndex) const
{
	if(pPacket == NULL)
		return false;

	return (nIndex >= 0 && nIndex < pPacket->GetDataCount());
}


///////////////////////////////////////////////////////////////////////////////
// class CCoordinateCalculator_NoAutoPrice

// protected ==================================================================
// XScale : ���� Interface�� ��� - vntsorl(20070410)
double CCoordinateCalculator_NoAutoPrice::GetData_X(const CCoordinateCalculatorData& calData, const CPoint& point, const int /*nIndex*/) const
{
	int nBlockColumn, nCenter_x;
	int nRow, nCol;
	double dResult = 0.0;
	calData.m_pIChartManager->GetBlockIndex(point, nRow, nCol);
	dResult = calData.m_pIChartManager->GetDoubleFromX(nRow, nCol, point.x, &nBlockColumn, nCenter_x);

	return dResult;
}

double CCoordinateCalculator_NoAutoPrice::GetData_Y(const CCoordinateCalculatorData& calData, const CPoint& point, const int /*nIndex*/) const
{
	double dY = CDataConversion::AbsoluteToRelative_Y(calData.GetRegion(), point.y, 
				calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType());

	// (2007/5/10 - Seung-Won, Bae) Check No Y Scale Min/Max.
	if( -HUGE_VAL == dY) return -HUGE_VAL;
	
	return calData.ApplyExp(dY);
}

double CCoordinateCalculator_NoAutoPrice::GetData_Y(const CCoordinateCalculatorData& calData, const double& dData, const int nIndex) const
{
	return calData.ApplyLog(dData);
}

///////////////////////////////////////////////////////////////////////////////
// class CCoordinateCalculatorFinder

CCoordinateCalculator_AutoPrice CCoordinateCalculatorFinder::m_autoPriceCal;
CCoordinateCalculator_NoAutoPrice CCoordinateCalculatorFinder::m_noAutoPriceCal;

CCoordinateCalculator& CCoordinateCalculatorFinder::FindCoordinateCalculator(const bool bIsAutoPrice)
{
	if(bIsAutoPrice)
		return m_autoPriceCal;
	return m_noAutoPriceCal;
}


///////////////////////////////////////////////////////////////////////////////
// class CSelectableRegionCalculator


CSelectableRegionCalculator::CSelectableRegionCalculator():
	m_nProximityRange(4)
{
}

// ������ ������ ����
int CSelectableRegionCalculator::GetProximityRange() const
{
	return m_nProximityRange;
}

// ������ �մ� ���� �θ��� ����.
void CSelectableRegionCalculator::GetLineRegion(const CPoint& point1, const CPoint& point2, CRgn& region) const
{
	// ����
	if(point1.y == point2.y)
	{
		CRect temp = GetHorzLineRegion(point1, point2);
		region.CreateRectRgn(temp.left, temp.top, temp.right, temp.bottom);
	}
	// ������
	else if(point1.x == point2.x)
	{
		CRect temp = GetVertLineRegion(point1, point2);
		region.CreateRectRgn(temp.left, temp.top, temp.right, temp.bottom);
	}
	// �缱
	else
		GetAngleLineRegion(point1, point2, region);
}

// ����: origin, ������: l�� ���� point�� �ִ��� �˻�
bool CSelectableRegionCalculator::IsPointInCircle(const CPoint& origin, const CPoint& point) const
{
	return (pow(m_nProximityRange*2, 2) >= (pow(origin.x - point.x, 2) + pow(origin.y - point.y, 2)));
	//return (pow(m_nProximityRange*2, 3) >= (pow(origin.x - point.x, 3) + pow(origin.y - point.y, 3)));
}

bool CSelectableRegionCalculator::IsPointInCircle(const CPoint& origin, const int nRadius, const CPoint& point) const
{
	return (pow(nRadius, 2) >= (pow(origin.x - point.x, 2) + pow(origin.y - point.y, 2)));
	//return (pow(nRadius, 3) >= (pow(origin.x - point.x, 3) + pow(origin.y - point.y, 3)));
}


// private ====================================================================
// ����
CRect CSelectableRegionCalculator::GetHorzLineRegion(const CPoint& point1, const CPoint& point2) const
{
	assert(point1.y == point2.y);

	CRect selectableRegion(point1.x, (point1.y - m_nProximityRange), point2.x, (point1.y + m_nProximityRange +1));
	selectableRegion.NormalizeRect();

	return selectableRegion;
}

// ������
CRect CSelectableRegionCalculator::GetVertLineRegion(const CPoint& point1, const CPoint& point2) const
{
	assert(point1.x == point2.x);

	CRect selectableRegion((point1.x - m_nProximityRange), point1.y, (point1.x + m_nProximityRange +1), point2.y);
	selectableRegion.NormalizeRect();

	return selectableRegion;
}

// �缱
void CSelectableRegionCalculator::GetAngleLineRegion(const CPoint& point1, const CPoint& point2, CRgn& region) const
{
	assert(point1.x != point2.x && point1.y != point2.y);

	// (0, 0)�� �������� �� ProximityRange ������ �ΰ��� ��ǥ ���ϱ�.
	CPoint basePt1, basePt2;
	GetVerticalPointsFromLength(point1, point2, basePt1, basePt2);

	POINT selectableRegionPoints[4];
	selectableRegionPoints[0] = GetPointOnMovedOrigin(point1, basePt1);
	selectableRegionPoints[1] = GetPointOnMovedOrigin(point1, basePt2);
	selectableRegionPoints[2] = GetPointOnMovedOrigin(point2, basePt2);
	selectableRegionPoints[3] = GetPointOnMovedOrigin(point2, basePt1);

	region.CreatePolygonRgn(selectableRegionPoints, 4, ALTERNATE);
}

// ----------------------------------------------------------------------------
// ��ǥ �̵�
POINT CSelectableRegionCalculator::GetPointOnMovedOrigin(const CPoint& origin, const CPoint& point) const
{
	POINT movePoint;
	movePoint.x = origin.x + point.x;
	movePoint.y = origin.y + point.y;

	return movePoint;
}

// Ư�� ���� ��ŭ ������ ��ǥ ������
// --> ���Ⱑ a �̰� (0, 0)�� ������ (0, 0)���� Ư������ +- L ������ ��ǥ ���ϱ�.
// --> x1 = k, y1 = a*k,    x2 = -k, y2 = a*-k
// --> k = |(L*L)/(a*a + 1)| �� ��Ʈ��
void CSelectableRegionCalculator::GetVerticalPointsFromLength(const CPoint& pt1, const CPoint& pt2, CPoint& selectableRegionPt1, CPoint& selectableRegionPt2) const
{
	// ������ �������� ���Ѵ�. == ���� ���� �� ��ǥ�� ����
	double lineAngle = GetLineVerticalAngle(GetLineAngle(pt1, pt2));

	double x = 0.0;
	x = sqrt(fabs(pow(m_nProximityRange, 2) / (pow(lineAngle, 2) +1)));

	selectableRegionPt1 = CPoint(CMath::Round(x), CMath::Round(lineAngle * x));
	selectableRegionPt2 = CPoint(-selectableRegionPt1.x, -selectableRegionPt1.y);
}

// ----------------------------------------------------------------------------
// ��ǥ�� ����.
// --> ������ ���� : (y2-y1)/(x2-x1)
double CSelectableRegionCalculator::GetLineAngle(const CPoint& point1, const CPoint& point2) const
{
	return ((double)(point2.y - point1.y) / (double)(point2.x - point1.x));
}

// ������ ����.
// --> -1/a
double CSelectableRegionCalculator::GetLineVerticalAngle(const double& dAngle) const
{
	assert(dAngle != 0.0);
	return (-1.0/dAngle);
}
