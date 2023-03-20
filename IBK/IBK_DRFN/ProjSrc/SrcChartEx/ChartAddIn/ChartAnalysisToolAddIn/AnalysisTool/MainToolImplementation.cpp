// MainToolImplementation.cpp: implementation of the CMainToolImplementation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainToolImplementation.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/Conversion.h"

#include "Tool.h"
#include "Element.h"
#include "CurrentElementSet.h"
#include "Selecting.h"										// for CSelectingTool
#include "CoordinateCalculator.h"							// for CCoordinateCalculator

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CMainToolImplementation

CMainToolImplementation::CMainToolImplementation( CWnd* pParent, HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode):
	m_analysisData( p_hOcxWnd, p_eChartMode),
	m_toolSingleton(&m_analysisData)
{
	Initialize();	
	m_analysisData.SetParent(pParent);	 
	m_ptOldLBtnUp = CPoint(-1,-1);
	m_ptOldMouse = CPoint(-1,-1);
	m_pIChartOCX = NULL;
}

// public =====================================================================
int CMainToolImplementation::GetElementCount() const
{
	return m_analysisData.GetElementTree().GetElementCount();
}

CString CMainToolImplementation::GetSavedElementsData() const
{
	return m_analysisData.GetElementTree().GetSavedElementsData();
}

CAnalysis::TOOLTYPE CMainToolImplementation::GetCurrentToolType() const
{
	return m_analysisData.GetCurrentToolType();
}

CToolEnumData::ElementType CMainToolImplementation::GetSelectedToolType() const
{
	if(m_pSelectedElement == NULL)
		return CToolEnumData::Not_Element;

	return m_pSelectedElement->GetElementType();
}

// ���� ���õ� element�� data ��������. (-> element type �� "line" �� ��쿡��)
bool CMainToolImplementation::GetData_Line(CList<CPoint, CPoint&>& elementData, double& dVertDataType) const
{
	elementData.RemoveAll();
	if(m_pSelectedElement == NULL || m_pSelectedElement->GetElementType() != CToolEnumData::Line)
		return false;

	CElementPointList elementPointList;
	m_pSelectedElement->GetPoints(elementPointList);
	return GetData(elementPointList, elementData, dVertDataType);
}

// ----------------------------------------------------------------------------
void CMainToolImplementation::SetParent(CWnd* pParent)
{
	m_analysisData.SetParent(pParent);
}

void CMainToolImplementation::SetMainBlock(IChartManager* pMainBlock, IChartOCX *p_pIChartOCX)
{
	m_pIChartOCX = p_pIChartOCX;
	m_analysisData.SetMainBlock(pMainBlock, p_pIChartOCX);
}

void CMainToolImplementation::SetAutoPriceTool(const bool bIsAutoPrice)
{
	m_toolSingleton.SetAutoPrice(bIsAutoPrice);
}

void CMainToolImplementation::SetAutoPriceElement(const bool bIsAutoPrice)
{
	m_analysisData.GetElementTree().SetAutoPrice(bIsAutoPrice);
}

void CMainToolImplementation::SetBaseColor(const COLORREF& penColor, const COLORREF& textColor)
{
	m_toolSingleton.SetBaseColor(penColor, textColor);
	if(m_pTool != NULL)
	{
 		m_pTool->SetPenColor(penColor);
		m_pTool->SetTextColor(textColor);
	}
}

void CMainToolImplementation::SetBasePenColor(const COLORREF& color)
{
	m_toolSingleton.SetBasePenColor(color);
	if(m_pTool != NULL)
		m_pTool->SetPenColor(color);
}

void CMainToolImplementation::SetBasePenThickness(const int nThickness)
{
	m_toolSingleton.SetBasePenThickness(nThickness);
}

void CMainToolImplementation::SetLineDrawingInfo(const CAnalysis::TOOLTYPE eToolType, const COLORREF crLine, const int nStyle, const int nWeight)
{
	m_toolSingleton.SetLineDrawingInfo(eToolType, crLine, nStyle, nWeight);
}

void CMainToolImplementation::SetTextPosition(const CAnalysis::TOOLTYPE eToolType, const bool bLeft, const bool bRight, const bool bTop, const bool bBottom, const bool bRightRate)
{
	m_toolSingleton.SetTextPosition(eToolType, bLeft, bRight, bTop, bBottom, bRightRate);	
}

void CMainToolImplementation::SetLineExtEnvironment(const CAnalysis::TOOLTYPE eToolType, const int& nLeftExt, const int& nRightExt)
{
	m_toolSingleton.SetLineExtEnvironment(eToolType,nLeftExt,nRightExt);
}

void CMainToolImplementation::SetEquiDivEnvironment(const CAnalysis::TOOLTYPE eToolType,const int& nEquiDiv)
{
	m_toolSingleton.SetEquiDivEnvironment(eToolType,nEquiDiv);
}

// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
void CMainToolImplementation::SetPriceRangeDivEnvironment(const CAnalysis::TOOLTYPE eToolType,const int& nPriceRangeDiv)
{
	m_toolSingleton.SetPriceRangeDivEnvironment(eToolType,nPriceRangeDiv);
}

void CMainToolImplementation::SetCoordinateType(const CAnalysis::COORDINATETYPE eCoordinateType)
{
	m_toolSingleton.SetCoordinateType(eCoordinateType);
}

void CMainToolImplementation::SetCurrentAnalysisTool(const CAnalysis::TOOLTYPE eToolType,BOOL bRedraw)
{
	if(eToolType != GetCurrentToolType())
		Initialize();

	if(bRedraw)	m_analysisData.Invalidate();
	m_analysisData.SetCurrentToolType(eToolType);
	m_pTool = m_toolSingleton.FindTool(eToolType);		
}

// 2008.08.20 ������ �Ǻ���ġ Ȯ��	<<
void CMainToolImplementation::SetFiboCoefficient(const CAnalysis::TOOLTYPE eToolType,const CString& strFiboCoefficient)
{
	m_toolSingleton.SetFiboCoefficient(eToolType, strFiboCoefficient);
}
//>>

// 2014.09.24 �ڼ��� : 0010862: [7111] �Ｚ���� ��Ʈ ������ [�ϸ��ġ] ��� IBK��Ʈ���� �����ϰ� �߰� ��û >>
void CMainToolImplementation::SetBalanceCoefficient(const CAnalysis::TOOLTYPE eToolType,const CString& strBalanceCoefficient)
{
	m_toolSingleton.SetBalanceCoefficient(eToolType, strBalanceCoefficient);
}
// 2014.09.24 �ڼ��� : 0010862: [7111] �Ｚ���� ��Ʈ ������ [�ϸ��ġ] ��� IBK��Ʈ���� �����ϰ� �߰� ��û <<

// ----------------------------------------------------------------------------
bool CMainToolImplementation::OnLButtonDown(CDC* pDC, const CPoint& point)
{
	if(m_pTool == NULL)
		return false;

	// (2007/5/10 - Seung-Won, Bae) Check No Y Scale Min/Max.
	CCoordinateCalculatorData calData = m_pTool->GetCoordinateCalculatorData( point);
	if( -HUGE_VAL == calData.GetDisplayDataMax_ScaleType() || -HUGE_VAL == calData.GetDisplayDataMin_ScaleType()) return false;
	
	m_pSelectedElement = m_pTool->OnLButtonDown(pDC, point);
	if(DoesCurrentSelectTool() && m_pSelectedElement == NULL)
	{
		m_analysisData.Invalidate();
		return false;
	}

	//�ٸ� �߼��� ���� ���� 
	//10000�� �����ִ� ���� - �ٸ� �ʿ��� ���� ���ϰ�
	//���� Check�Ҷ��� 10000�� ����	
	if(m_pSelectedElement != NULL)
	{
		int nElementType = m_pSelectedElement->GetElementType() + 10000;
		m_pIChartOCX->RunToolCommand(CToolOptionInfo::T_TOOL_SELECTION,nElementType);
	}
	return true;
}

bool CMainToolImplementation::OnLButtonDbClk(CDC* pDC, const CPoint& point)
{
	if(m_pSelectedElement == NULL)
		return false;

	CBlockIndex blockIndex = m_analysisData.GetBlockIndexInPoint(point);

	IChartManager *pIChartManager = m_pIChartOCX->GetIChartManager();
	if( !pIChartManager) return false;
	CScaleBaseData::HORZSCALEDRAWERTYPE eBaseTimeDataType = pIChartManager->GetMasterValueType(blockIndex.GetRow(), blockIndex.GetColumn());
	pIChartManager->Release();

	// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
// 	CCurrentElementSet elementSet(m_analysisData.GetUserPath(), 
// 								  eBaseTimeDataType,
// 								  m_analysisData.GetParent(), 
// 								  m_pSelectedElement);

	// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	IPacketManager * pPacketManager = m_pIChartOCX->GetIDefaultPacketManager();
	ILPCSTR szHelpMsg = NULL;
	CString strFormatX;	
	pPacketManager->GetHelpMessageItem("RDATET", szHelpMsg);	// �����޽����� UD Ÿ���� ���´�.	
	strFormatX = szHelpMsg;
	// 2011.02.08 by SYS <<

	//
	CCurrentElementSet elementSet(m_pIChartOCX,
								  blockIndex,
								  strFormatX,
								  m_analysisData.GetUserPath(), 
								  eBaseTimeDataType,
								  m_analysisData.GetParent(), 
								  m_pSelectedElement,
								  (LONG)&m_xCanCallYou);
	// 2011.02.08 by SYS <<


	CString strElementData_Prev = m_pSelectedElement->GetSavedElementData(m_analysisData.GetElementTree().GetOrder());
	elementSet.OnMenu(CMenuType::MENU_SET, true);
	CString strElementData_After = m_pSelectedElement->GetSavedElementData(m_analysisData.GetElementTree().GetOrder());

	if(strElementData_Prev.Compare(strElementData_After))
		m_analysisData.GetUndoRedoManager().RegisterElementToUnRedoManager(URT_MODIFY_ENV, (long)m_pSelectedElement, strElementData_Prev, strElementData_After);

	return true;
}

bool CMainToolImplementation::OnLButtonUp(CDC* pDC, const CPoint& point)
{
	if(!DoesAction())
		return false;

	m_pTool->OnLButtonUp(pDC, point);
	m_ptOldLBtnUp = point;
	m_analysisData.Invalidate();
	
	if(!DoesCurrentMultiTool())
		SetCurrentAnalysisTool(CAnalysis::EN_ANALYSIS_SELECTION);
	if(!DoesAction())
		m_analysisData.SendMessageToParent(CCursorShape::OBJECT_NONE);

	return true;
}

bool CMainToolImplementation::OnMouseMove(CDC* pDC, const CPoint& point,int nFlags,const HBITMAP p_hbmpMainOSB)
{	
	m_ptOldMouse = point;
	if(!DoesAction())
		return false;
	
	return m_pTool->OnMouseMove(pDC, point,nFlags);
}

CMenuType::MENUTYPE CMainToolImplementation::OnRButtonUp(CDC* pDC, const CPoint& point)
{
	if(m_pTool == NULL || m_pSelectedElement != NULL)
		return GetRButtonMenuType(point);

	m_pTool->OnRButtonUp(pDC, point);
	return CMenuType::MENU_NONE;
}

bool CMainToolImplementation::OnMenu(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	if(m_pSelectedElement == NULL && CMenuType::MENU_TIME_DIFF != menuType)
		return false;

	switch(menuType)
	{
	case CMenuType::MENU_DELETE:	// ����
		return DeleteCurrentElement();
	default:
		{
		IPacketManager* pPackManager = m_analysisData.GetPacketManager();

		// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
// 		CCurrentElementSet elementSet(m_analysisData.GetUserPath(), 
// 									  pPackManager->GetBaseTimeDataType(),
// 									  m_analysisData.GetParent(), 
// 									  m_pSelectedElement);

		//
		CBlockIndex blockIndex = m_analysisData.GetBlockIndexInPoint(m_ptOldLBtnUp);

		// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
		ILPCSTR szHelpMsg = NULL;
		CString strFormatX;	
		pPackManager->GetHelpMessageItem("RDATET", szHelpMsg);	// �����޽����� UD Ÿ���� ���´�.	
		strFormatX = szHelpMsg;
		// 2011.02.08 by SYS <<

		CCurrentElementSet elementSet(m_pIChartOCX,
									  blockIndex,
									  strFormatX,
									  m_analysisData.GetUserPath(), 
									  pPackManager->GetBaseTimeDataType(),
									  m_analysisData.GetParent(), 
									  m_pSelectedElement,
									  (LONG)&m_xCanCallYou);
		// 2011.02.08 by SYS <<

		pPackManager->Release();
		return elementSet.OnMenu(menuType, wParam);
		}
	}

	return false;
}

CRect CMainToolImplementation::GetGraphTotalRegion(CPoint point)
{
	IChartManager *pIChartManager = m_analysisData.GetMainBlock();
	CRect rectRegion = pIChartManager->GetGraphRegionSumInColumn(point);
	pIChartManager->Release();	
	return rectRegion;
}

BOOL CMainToolImplementation::GetCurrentPosData(CPoint point,CString &strTextRight,CString &strTextBottom)
{
	strTextRight.Empty();
	strTextBottom.Empty();
	IChartManager *pIChartManager = m_analysisData.GetMainBlock();
	CRect rectRegion = pIChartManager->GetGraphRegionSumInColumn(point);
	//>> (2008/12/26 - Seung-Lyong, Park) �޸𸮸� ����
//	pIChartManager->Release();

	IPacketManager* pPacketManager = m_analysisData.GetPacketManager();
	const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod = pPacketManager->GetBaseTimeDataType();
	pPacketManager->Release();

	if(rectRegion.PtInRect(point) == TRUE)
	{
		CCoordinateCalculatorData calData = m_analysisData.GetCoordinateCalculatorData(point);
		if(calData.GetDisplayDataEnd() < 0)
		{
			pIChartManager->Release();
			return FALSE;
		}
		
		IPacket *pIPacket = calData.GetPacket_X();
//		if(pIPacket == NULL) return FALSE;
		if(pIPacket == NULL)
		{
			pIChartManager->Release();
			return FALSE;
		}
		
		CCoordinate cdResult;
		if( !(pIPacket != NULL && pIPacket->GetDataCount() > 0)) 
		{
			cdResult = CCoordinate(point);
		}
		else
		{
			// XScale : ���� Interface�� ��� - vntsorl(20070327)
//			int nIndex = CDataConversion::GetPacketDataIndex_Date(calData.GetRegion(), point, 
//						calData.GetDisplayDataStart(), calData.GetDisplayDataEnd());
			int nIndex = pIChartManager->FindViewDataIndex_Pt("DEFAULT", point.x);
			
			double dX = 0.0;
			if( pIPacket) pIPacket->GetDataPastandFuture(nIndex, dX);


			//>>[20110518]alzioyes:���ڼ� Y���� Log�� ����
			//double dY = CDataConversion::AbsoluteToRelative_Y(calData.GetRegion(), point.y, 
			//		calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType());

			CDisplayDataVertMinMax& DispDataVertMinMax = calData.GetDisplayDataVertMinMax();
			
			BOOL bLog = DispDataVertMinMax.GetVertScaleType().IsLog();
			BOOL bReverse = DispDataVertMinMax.GetVertScaleType().IsInvert();
			double dY = m_pIChartOCX->ConvertYPositionToData(point.y, calData.GetRegion().top, calData.GetRegion().bottom, DispDataVertMinMax.GetMax(), DispDataVertMinMax.GetMin(), bLog, bReverse);
			//<<[20110518]alzioyes:���ڼ� Y���� Log�� ����

			// (2007/5/10 - Seung-Won, Bae) Check No Y Scale Min/Max.
			if( -HUGE_VAL == dY)
			{
				if( pIPacket) pIPacket->Release();
				pIChartManager->Release();
				return FALSE;
			}

			// ����Ʈ �����ð� ���� - ojtaso (20080811)
			ILPCSTR szPacketType = pIPacket->GetType();
			CString strPacketType( szPacketType);
			// 20081007 JS.Kim	����ڽð��� ����
			cdResult = CCoordinate(dX, dY, pIPacket->GetBaseTimeDifference(), strPacketType, 0);
		}

		ILPCSTR szPacketType = pIPacket->GetType();
		CString strPacketType( szPacketType);

		// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
		//strTextBottom = CDataConversion( m_analysisData.GetOcxHwnd(), m_analysisData.GetChartMode()).GetStringFormatTrans(CDataConversion::IntToString((int)cdResult.GetX()), strPacketType, "");
		if (strPacketType == _T("YYYYMMDDHHMMSS"))
		{
			//int nDataUnit = pIPacket->GetDateUnitType();
			//strTextBottom = CDataConversion( m_analysisData.GetOcxHwnd(), m_analysisData.GetChartMode()).GetStringFormatTrans2(CDataConversion::DoubleToString(cdResult.GetX()), nDataUnit, "");

			// nDataUnit (0:nontype, 1:��, 2:��, 3:��, 4:��, 5:ƽ, 6:Text)
			CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)pIPacket->GetDateUnitType();
			
			CString strFormat = _T("YYYYMMDD");	// ����Ʈ����
			switch(eDateUnit)
			{
			case CScaleBaseData::HORZ_DAILY:		// ��
			case CScaleBaseData::HORZ_WEEKLEY:		// �� 
				strFormat = _T("YYYYMMDD");		
				break;
			case CScaleBaseData::HORZ_MONTHLY:		// �� 
				strFormat = _T("YYYYMM");		
				break;
			case CScaleBaseData::HORZ_TIME_ONE:		// ��
			case CScaleBaseData::HORZ_TIME_TWO:
				strFormat = _T("MMDDHHMM");
				break;
			case CScaleBaseData::HORZ_TICK:			// ƽ 
			case CScaleBaseData::HORZ_TIME_SECOND:
				strFormat = _T("DDHHMMSS");
				break;
			}
			strTextBottom = CDataConversion( m_analysisData.GetOcxHwnd(), m_analysisData.GetChartMode()).GetStringFormatTrans(CDataConversion::DoubleToString(cdResult.GetX()), strFormat, "");
		}
		else
			strTextBottom = CDataConversion( m_analysisData.GetOcxHwnd(), m_analysisData.GetChartMode()).GetStringFormatTrans(CDataConversion::DoubleToString(cdResult.GetX()), strPacketType, "");
		// 2011.01.06 by SYS <<

		pIPacket->Release();
		
		// 20081009 �ڵ��� 8����/32���� ó�� >>
		IPacket *pIPacket_Y = calData.GetPacket_Y();
		if( pIPacket_Y)
		{
			ILPCSTR szPacketType = pIPacket_Y->GetType();
			CString strPacketType( szPacketType);
			strTextRight = CDataConversion::GetStringThousandFormat(cdResult.GetY(), strPacketType);
			pIPacket_Y->Release();
		}
		else strTextRight = CDataConversion::GetStringThousandFormat(cdResult.GetY(), calData.GetVertDataType());
		// 20081009 �ڵ��� <<
	}
	pIChartManager->Release();
	//<< (2008/12/26 - Seung-Lyong, Park) �޸𸮸� ����

	return TRUE;
}
// ----------------------------------------------------------------------------
void CMainToolImplementation::OnDraw(CDC* pDC)
{
	// element / grip �׸���.
	m_analysisData.DrawElement(pDC, m_pSelectedElement);
	
	// ���� ���õ� tool �׸���.
	//if(DoesAction())
	if(m_pTool != NULL && m_pSelectedElement != NULL)
		m_pTool->OnDraw(pDC);	
}

void CMainToolImplementation::DrawTool(CDC *pDC)
{
	//m_pTool = m_toolSingleton.FindTool(C);	
	// ���� ���õ� tool �׸���.
	if(m_ptOldLBtnUp == m_ptOldMouse)
		return;

	if(DoesAction())	
	{
		// 2011.01.27 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
// 		ILPCSTR szHelpMsg = NULL;
// 		CString strDataType;	
// 		IPacketManager* pPacketManager = m_analysisData.GetPacketManager();
// 		pPacketManager->GetHelpMessageItem("UD", szHelpMsg);	// �����޽����� UD Ÿ���� ���´�.	
// 		strDataType = szHelpMsg;
// 		const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod = pPacketManager->GetBaseTimeDataType();
// 		pPacketManager->Release();
		// 2011.01.27 by SYS <<

		m_pTool->OnDraw(pDC);	
	}
}
// ----------------------------------------------------------------------------
// element �����.
void CMainToolImplementation::BuildElementsFromString(const CString& strElementsData)
{
	m_analysisData.GetElementTree().BuildElementsFromString(m_analysisData,strElementsData);
}

// block �߰��� elementTree�� �߰��ϱ�.
bool CMainToolImplementation::InsertElementTree(const CBlockIndex& insertBlockIndex)
{
	return m_analysisData.InsertElementTree(insertBlockIndex);
}

// block �̵�
bool CMainToolImplementation::MoveElementTree(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex)
{
	if(!m_analysisData.GetElementTree().MoveElementTree(sourceBlockIndex, targetBlockIndex))
		return false;

	m_toolSingleton.InitializeAllTool();
	return true;
}

// ----------------------------------------------------------------------------
// ��� element ����
void CMainToolImplementation::DeleteAllElement()
{
	//�߼��� �׸��� �ϴٰ� ��ü����� �ϰ� ���� �׸��� �ٽñ׸��� ������ �Ʒ� ������
	//SetCurrentAnalysisTool(CAnalysis::EN_ANALYSIS_SELECTION,FALSE);
	m_analysisData.GetElementTree().DeleteAll();
	m_analysisData.GetUndoRedoManager().DeleteAll();
//	m_analysisData.GetElementUndoTree().DeleteAll();
//	m_analysisData.GetElementRedoTree().DeleteAll();
	SetElementDelete();
}

// Ư�� element�� ����
bool CMainToolImplementation::DeleteElement(const CToolEnumData::ElementType eElementTYPE)
{
	//SetCurrentAnalysisTool(CAnalysis::EN_ANALYSIS_SELECTION);
	if(!m_analysisData.GetElementTree().DeleteElement(eElementTYPE))
		return false;

	SetElementDelete();
	return true;
}

// ���� ���õ� element ����
bool CMainToolImplementation::DeleteCurrentElement()
{
	//SetCurrentAnalysisTool(CAnalysis::EN_ANALYSIS_SELECTION);
	if(m_pSelectedElement == NULL)
		return false;

	m_analysisData.UnRegisterElementToOrderManager((long)m_pSelectedElement);

	// Undo / Redo
	m_analysisData.GetUndoRedoManager().RegisterElementToUnRedoManager(URT_NEW_OR_DEL, (long)m_pSelectedElement);

	m_analysisData.GetElementTree().DeleteElement(m_pSelectedElement);	

	SetElementDelete();
	return true;
}

// �ֱٿ� ����������  element ����
void CMainToolImplementation::DeleteTail()
{
	//SetCurrentAnalysisTool(CAnalysis::EN_ANALYSIS_SELECTION);
	CElement* pElement = m_analysisData.GetElementTree().GetTail();
	m_analysisData.GetElementTree().DeleteTail();

	// Undo / Redo
	m_analysisData.GetUndoRedoManager().RegisterElementToUnRedoManager(URT_NEW_OR_DEL, (long)pElement);
	SetElementDelete();
}

BOOL CMainToolImplementation::DeleteElementPos(const long lAddress)
{
	//CElement *pElement = (CElement *)lAddress;
	//long lNewElement = m_analysisData.GetElementUndoTree().CopyElement(pElement);

	//>> (2008/12/17 - Seung-Lyong Park) �߼��� ��������� �� UndoRedo
	// Undo / Redo
	m_analysisData.GetUndoRedoManager().RegisterElementToUnRedoManager(URT_NEW_OR_DEL, lAddress);
	SetElementDelete();
	
	if(m_analysisData.GetElementTree().DeleteElementPos(lAddress) == true)
	{
		//SetCurrentAnalysisTool(CAnalysis::EN_ANALYSIS_SELECTION);

//		// Undo / Redo
//		m_analysisData.GetUndoRedoManager().RegisterElementToUnRedoManager(URT_NEW_OR_DEL, lAddress);
//		SetElementDelete();
		return TRUE;
	}
	//<< (2008/12/17 - Seung-Lyong Park) �߼��� ��������� �� UndoRedo

	return FALSE;
}

// �ش� block�� �ִ� ��� element ����
void CMainToolImplementation::DeleteAllElementInBlock(const CBlockIndex& blockIndex)
{
	CDequePointerList<CElement> elementList;
	int nIndex = 0;
	m_analysisData.GetElementTree().GetElementListOfBlock(blockIndex, elementList);
	for( nIndex = 0; nIndex < elementList.GetCount(); ++nIndex)
	{
		m_analysisData.UnRegisterElementToOrderManager((long)elementList.GetAt(nIndex));
	}
	m_analysisData.GetElementTree().DeleteRowSet(blockIndex);

	// Undo / Redo
	CDequePointerList<CUndo_Redo_Info> undoredoInfoList;
	m_analysisData.GetUndoRedoManager().GetUndoRedoInfoListOfBlock(blockIndex, undoredoInfoList);
	for(nIndex = 0; nIndex < undoredoInfoList.GetCount(); ++nIndex)
	{
		m_analysisData.GetUndoRedoManager().UnRegisterElementToUnRedoManager((long)undoredoInfoList.GetAt(nIndex));
	}

	m_analysisData.Invalidate();
	m_pSelectedElement = NULL;
}

// ----------------------------------------------------------------------------
// element �� ����
void CMainToolImplementation::ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor)
{
	m_analysisData.GetElementTree().ChangeAllElementsColor(penColor, textColor);
}

// private ====================================================================
void CMainToolImplementation::Initialize()
{
	m_toolSingleton.InitializeAllTool();
	m_analysisData.SetCurrentToolType(CAnalysis::EN_ANALYSIS_NONE);
	m_pTool = NULL;
	m_pSelectedElement = NULL;
}

// ----------------------------------------------------------------------------
// ���� Tool�� select tool ������ ����.
bool CMainToolImplementation::DoesCurrentSelectTool() const
{
	return (m_analysisData.GetCurrentToolType() == CAnalysis::EN_ANALYSIS_SELECTION);
}

// tool �� ���������� ����. (true = ����)
bool CMainToolImplementation::DoesAction() const
{
	return (m_pTool != NULL && m_pTool->DoesProcessStarting());
}

// ���� tool�� ���� ���� tool ���� ����.
bool CMainToolImplementation::DoesCurrentMultiTool() const
{
	/*switch(m_analysisData.GetCurrentToolType())
	{
	case CAnalysis::EN_ANALYSIS_FIBO_TIME:		// �Ǻ���ġ �ð���
	case CAnalysis::EN_ANALYSIS_GANN_GRID:		// ���׸���
		return false;
	}*/

	return true;
}

// ----------------------------------------------------------------------------
bool CMainToolImplementation::GetData(const CElementPointList& elementPointList, CList<CPoint, CPoint&>& elementData, double& dVertDataType) const
{
	if(elementPointList.GetCount() <= 0)
		return false;

	dVertDataType = elementPointList.GetVertDataType();

	for(int nIndex = 0; nIndex < elementPointList.GetCount(); nIndex++)
	{
		CCoordinate* pCoordinate = elementPointList.GetAt(nIndex);
		if(pCoordinate == NULL)
			continue;
		CPoint point(pCoordinate->GetX(), pCoordinate->GetY());
		elementData.AddTail(point);
	}

	return (elementData.GetCount() > 0);
}

CMenuType::MENUTYPE CMainToolImplementation::GetRButtonMenuType(const CPoint& point)
{
	if(m_pSelectedElement == NULL || !m_analysisData.PointInGraphRegion(point))
		return CMenuType::MENU_NONE;

	m_analysisData.SendMessageToParent(CCursorShape::OBJECT_NONE);
	return CMenuType::MENU_ANALYSTOOL;
}

// ----------------------------------------------------------------------------
void CMainToolImplementation::SetElementDelete()
{
	m_toolSingleton.InitializeAllTool();
	m_pSelectedElement = NULL;
	m_analysisData.InvalidateAndSendMessageToParent(CCursorShape::OBJECT_NONE);
}

bool CMainToolImplementation::OnToolCopy()		//�߼��� ����
{
	if(m_pSelectedElement == NULL)
		return false;
	
	//>> (2008/12/16 - Seung-Lyong Park) �м����� ����
	CElement* pSelectedElement = NULL;
	pSelectedElement = m_analysisData.CopyElement(m_pSelectedElement);
	if(!pSelectedElement)
		return false;
	
	// (2008/12/12 - Seung-Won, Bae) Move to shift.
	m_pSelectedElement = pSelectedElement;
	CElementPointList elementPointList;
	m_pSelectedElement->GetPoints(elementPointList);
	
	CPoint orgPoint;
	CPoint modifyPoint;
	CElementModifyData orgData;
	CElementModifyData modifyData;
	if(m_pSelectedElement->GetElementType() == CToolEnumData::Vert_Line)
	{
		( ( CSelectingTool *)m_pTool)->SetSelectToModify( m_pSelectedElement, CToolEnumData::MODIFY_X);
		CSelectedElementDrawingData selectedDrawingData = ( ( CSelectingTool *)m_pTool)->GetSelectedElementDrawData();
		orgPoint = m_pSelectedElement->GetSelectedDrawPoint(selectedDrawingData, *elementPointList.GetAt(0));
		modifyPoint = orgPoint;
		modifyPoint.x += 10;

		orgData = ( ( CSelectingTool *)m_pTool)->GetElementModifyData( orgPoint);
		for(int nIndx = 0; nIndx < 10; ++nIndx)
		{
			modifyData = ( ( CSelectingTool *)m_pTool)->GetElementModifyData( modifyPoint);
			if(orgData.GetPoint().GetX() < modifyData.GetPoint().GetX())
				break;

			modifyPoint.x += 10;
		}

		if(modifyData.GetBlockIndex().GetRow() < 0 || modifyData.GetBlockIndex().GetColumn() < 0)
		{
			modifyData.GetBlockIndex().SetRow(0);
			modifyData.GetBlockIndex().SetColumn(0);
		}

		( ( CSelectingTool *)m_pTool)->SetBtDownPoint(( ( CSelectingTool *)m_pTool)->GetDrawingPoint(modifyPoint));
		m_pSelectedElement->ElementModify( modifyData);
	}
	else if(m_pSelectedElement->GetElementType() == CToolEnumData::Horz_Line)
	{
		( ( CSelectingTool *)m_pTool)->SetSelectToModify( m_pSelectedElement, CToolEnumData::MODIFY_Y);
		CSelectedElementDrawingData selectedDrawingData = ( ( CSelectingTool *)m_pTool)->GetSelectedElementDrawData();
		orgPoint = m_pSelectedElement->GetSelectedDrawPoint(selectedDrawingData, *elementPointList.GetAt(0));
		modifyPoint = orgPoint;
		modifyPoint.y += 10;
		modifyData = ( ( CSelectingTool *)m_pTool)->GetElementModifyData( modifyPoint);

		( ( CSelectingTool *)m_pTool)->SetBtDownPoint(( ( CSelectingTool *)m_pTool)->GetDrawingPoint(modifyPoint));
		m_pSelectedElement->ElementModify( modifyData);
	}
	else
	{
		( ( CSelectingTool *)m_pTool)->SetSelectToModify( m_pSelectedElement, CToolEnumData::MODIFY_CENTER);
		modifyData = ( ( CSelectingTool *)m_pTool)->GetElementModifyData( CPoint( 10, 10));
		m_pSelectedElement->ElementModify( modifyData);
	}
	m_analysisData.Invalidate();

	//>> (2008/12/17 - Seung-Lyong Park) �߼��� ��������� �� UndoRedo
	m_analysisData.RegisterElementToOrderManager( ( long)m_pSelectedElement);

	m_analysisData.GetElementTree().Add(modifyData.GetBlockIndex(), m_pSelectedElement);	

	// Undo / Redo
	m_analysisData.GetUndoRedoManager().RegisterElementToUnRedoManager(URT_NEW_OR_DEL, ( long)m_pSelectedElement);
	//<< (2008/12/17 - Seung-Lyong Park) �߼��� ��������� �� UndoRedo

	//<< (2008/12/16 - Seung-Lyong Park) �м����� ����
	return true;
}

BOOL CMainToolImplementation::UndoElement(long lUndoID)
{
	return m_analysisData.GetUndoRedoManager().UndoElement(lUndoID);
}

BOOL CMainToolImplementation::RedoElement(long lRedoID)
{
	return m_analysisData.GetUndoRedoManager().RedoElement(lRedoID);
}

// 20081007 JS.Kim	����ڽð��� ����
// Graph�� �ð��밡 ����Ǹ� Tool �� �ð��� �ٲ��ش�.
bool CMainToolImplementation::RecalcElementXPoint(const char* szName, int nTimeDiff)
{
	IChartManager *pIChartManager = m_analysisData.GetMainBlock();
	ILPCSTR szCurrentRQ = pIChartManager->GetCurrentRQ();
	IPacket* pPacket= m_analysisData.GetDatePacket(szCurrentRQ);
	CString strCurrentRQ(szCurrentRQ);

	int	nBaseTimeDiff;
	if( strlen(szName) == 0 )	nBaseTimeDiff = pPacket->GetOrgBaseTimeDifference();		// �����ð����� �ǵ��� ���
	else						nBaseTimeDiff = nTimeDiff;

	int nBlkColCount = pIChartManager->GetColumnCount();
	int nBlkRowCount;
	for( int col = 0; col < nBlkColCount; col++ )
	{
		nBlkRowCount = pIChartManager->GetRowCountInColumn(col);
		for( int row = 0; row < nBlkRowCount; row++ )
		{
			ILPCSTR szMasterRQ = pIChartManager->GetMainRQofBlock(row, col);
			if( strCurrentRQ.Compare(szMasterRQ) )		continue;

			RecalcElementListXPoint(row, col, nBaseTimeDiff, pPacket);
		}
	}

	if( pPacket) pPacket->Release();
	pIChartManager->Release();

	return true;
}

void CMainToolImplementation::RecalcElementListXPoint(int row, int col, int nBaseTimeDiff, IPacket* pPacket)
{
	CDequePointerList<CElement> elementList;

	CBlockIndex blkIdx(row, col);
	m_analysisData.GetElementTree().GetElementListOfBlock(blkIdx, elementList);

	ILPCSTR szPacketType = pPacket->GetType();
	
	double dXDate, oldXDate;
	int oldTimeDiff;
	CCoordinate *pCoordinate = NULL;
	CElement* pElement;
	for(int nIndex = 0; nIndex < elementList.GetCount(); nIndex++)
	{
		CElementPointList elementPointList;			// �ݵ�� for �� �ȿ��� ������ ��.. pElement->GetPoints()���� �ʱ�ȭ ���ϹǷ�.
		pElement = (CElement *)elementList.GetAt(nIndex);
		pElement->GetPoints(elementPointList);

		for(int i=0;i<elementPointList.GetCount();i++)
		{
			pCoordinate = elementPointList.GetAt(i);
			oldXDate = pCoordinate->GetX();
			oldTimeDiff = pCoordinate->GetTimeDiff();
			if( oldTimeDiff != nBaseTimeDiff )
			{
				dXDate = CDataConversion::CalcTimeDiff(oldXDate, nBaseTimeDiff - oldTimeDiff, pPacket->GetOrgBaseTimeDifference(), CString( szPacketType));
				pCoordinate->SetX(dXDate, pCoordinate->GetIndex());
				pCoordinate->SetTimeDiff(nBaseTimeDiff);
			}
		}
	}
}

HRESULT CMainToolImplementation::CCanCallYou::DoCallYou(int nCallType, LONG dwData)
{
	MYMETHOD_PROLOGUE(CMainToolImplementation, CanCallYou)

	if(nCallType==ICCYT_changeThick)
	{
		pThis->m_analysisData.GetElementTree().ChangeAllElementsThick(pThis->m_pSelectedElement->GetElementType(), (int)dwData);
	}
	return S_OK;
}
