// MainAnalysisTool.cpp: implementation of the CMainAnalysisTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../Include_Analysis/Dll_Load/MainAnalysisTool.h"

#include "MainToolImplementation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CMainAnalysisTool

/*
CMainAnalysisTool::CMainAnalysisTool():
	m_pMainToolImpl(NULL)
{
	m_pMainToolImpl = new CMainToolImplementation(NULL);
}
*/

CMainAnalysisTool::CMainAnalysisTool( CWnd* pParent, HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode):
	m_pMainToolImpl(NULL)
{
	m_pMainToolImpl = new CMainToolImplementation( pParent, p_hOcxWnd, p_eChartMode);
}

CMainAnalysisTool::~CMainAnalysisTool()
{
	delete m_pMainToolImpl;
	m_pMainToolImpl = NULL;
}

// public =====================================================================
// ���� �ִ� ��ü�� ����
int CMainAnalysisTool::GetElementCount() const
{
	return m_pMainToolImpl->GetElementCount();
}

// ������ element ���� data
CString CMainAnalysisTool::GetSavedElementsData() const
{
	return m_pMainToolImpl->GetSavedElementsData();
}

// ���� tool mode.
CAnalysis::TOOLTYPE CMainAnalysisTool::GetCurrentToolType() const
{
	return m_pMainToolImpl->GetCurrentToolType();
}

CToolEnumData::ElementType CMainAnalysisTool::GetSelectedToolType() const
{
	return m_pMainToolImpl->GetSelectedToolType();
}

// ���� ���õ� element�� data ��������. (-> element type �� "line" �� ��쿡��)
bool CMainAnalysisTool::GetData_Line(CList<CPoint, CPoint&>& elementData, double& dVertDataType) const
{
	return m_pMainToolImpl->GetData_Line(elementData, dVertDataType);
}

// ----------------------------------------------------------------------------
void CMainAnalysisTool::Set(CWnd* pParent, IChartManager* pMainBlock, IChartOCX *p_pIChartOCX, const COLORREF& color, 
		const bool bIsAutoPrice, const CAnalysis::COORDINATETYPE eCoordinateType)
{
	SetParent(pParent);
	SetMainBlock(pMainBlock, p_pIChartOCX);
	SetBasePenColor(color);
	SetAutoPriceTool(bIsAutoPrice);
	SetCoordinateType(eCoordinateType);
}

void CMainAnalysisTool::SetParent(CWnd* pParent)
{
	m_pMainToolImpl->SetParent(pParent);
}

void CMainAnalysisTool::SetMainBlock(IChartManager* pMainBlock, IChartOCX *p_pIChartOCX)
{	
	m_pMainToolImpl->SetMainBlock(pMainBlock, p_pIChartOCX);
}

bool CMainAnalysisTool::OnToolCopy()		//�߼��� ����
{
	return m_pMainToolImpl->OnToolCopy();
}

void CMainAnalysisTool::SetAutoPriceTool(const bool bIsAutoPrice)
{
	m_pMainToolImpl->SetAutoPriceTool(bIsAutoPrice);
}

void CMainAnalysisTool::SetAutoPriceElement(const bool bIsAutoPrice)
{
	m_pMainToolImpl->SetAutoPriceElement(bIsAutoPrice);
}

void CMainAnalysisTool::SetBaseColor(const COLORREF& penColor, const COLORREF& textColor)
{
	m_pMainToolImpl->SetBaseColor(penColor, textColor);
}

void CMainAnalysisTool::SetBasePenColor(const COLORREF& color)
{
	m_pMainToolImpl->SetBasePenColor(color);
}

void CMainAnalysisTool::SetBasePenThickness(const int nThickness)
{
	m_pMainToolImpl->SetBasePenThickness(nThickness);
}

void CMainAnalysisTool::SetLineDrawingInfo(const CAnalysis::TOOLTYPE eToolType, const COLORREF crLine, const int nStyle, const int nWeight)
{
	m_pMainToolImpl->SetLineDrawingInfo(eToolType,crLine,nStyle,nWeight);
}

void CMainAnalysisTool::SetTextPosition(const CAnalysis::TOOLTYPE eToolType, const bool bLeft, const bool bRight, const bool bTop, const bool bBottom, const bool bRightRate)
{
	m_pMainToolImpl->SetTextPosition(eToolType, bLeft, bRight, bTop, bBottom, bRightRate);
}

void CMainAnalysisTool::SetLineExtEnvironment(const CAnalysis::TOOLTYPE eToolType,const int& nLeftExt,const int& nRightExt)
{
	m_pMainToolImpl->SetLineExtEnvironment(eToolType, nLeftExt, nRightExt);
}

void CMainAnalysisTool::SetEquiDivEnvironment(const CAnalysis::TOOLTYPE eToolType,const int& nEquiDiv)
{
	m_pMainToolImpl->SetEquiDivEnvironment(eToolType, nEquiDiv);
}

// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
void CMainAnalysisTool::SetPriceRangeDivEnvironment(const CAnalysis::TOOLTYPE eToolType,const int& nPriceRangeDiv)
{
	m_pMainToolImpl->SetPriceRangeDivEnvironment(eToolType, nPriceRangeDiv);
}

void CMainAnalysisTool::SetCoordinateType(const CAnalysis::COORDINATETYPE eCoordinateType)
{
	m_pMainToolImpl->SetCoordinateType(eCoordinateType);
}

void CMainAnalysisTool::SetCurrentAnalysisTool(const CAnalysis::TOOLTYPE eToolType)
{
	m_pMainToolImpl->SetCurrentAnalysisTool(eToolType);
}

// 2008.08.20 ������ �Ǻ���ġ Ȯ��	<<
void CMainAnalysisTool::SetFiboCoefficient(const CAnalysis::TOOLTYPE eToolType,const CString& strFiboCoefficient)
{
	m_pMainToolImpl->SetFiboCoefficient(eToolType, strFiboCoefficient);
}
//>>

// 2014.09.24 �ڼ��� : 0010862: [7111] �Ｚ���� ��Ʈ ������ [�ϸ��ġ] ��� IBK��Ʈ���� �����ϰ� �߰� ��û >>
void CMainAnalysisTool::SetBalanceCoefficient(const CAnalysis::TOOLTYPE eToolType,const CString& strBalanceCoefficient)
{
	m_pMainToolImpl->SetBalanceCoefficient(eToolType, strBalanceCoefficient);
}
// 2014.09.24 �ڼ��� : 0010862: [7111] �Ｚ���� ��Ʈ ������ [�ϸ��ġ] ��� IBK��Ʈ���� �����ϰ� �߰� ��û <<

// ----------------------------------------------------------------------------
bool CMainAnalysisTool::OnLButtonDown(CDC* pDC, const CPoint& point)
{
	return m_pMainToolImpl->OnLButtonDown(pDC, point);
}

bool CMainAnalysisTool::OnLButtonDbClk(CDC* pDC, const CPoint& point)
{
	return m_pMainToolImpl->OnLButtonDbClk(pDC, point);
}

bool CMainAnalysisTool::OnLButtonUp(CDC* pDC, const CPoint& point)
{
	return m_pMainToolImpl->OnLButtonUp(pDC, point);
}

bool CMainAnalysisTool::OnMouseMove(CDC* pDC, const CPoint& point,int nFlags,const HBITMAP p_hbmpMainOSB)
{
	return m_pMainToolImpl->OnMouseMove(pDC, point,nFlags,p_hbmpMainOSB);
}

CMenuType::MENUTYPE CMainAnalysisTool::OnRButtonUp(CDC* pDC, const CPoint& point)
{
	return m_pMainToolImpl->OnRButtonUp(pDC, point);
}

bool CMainAnalysisTool::OnMenu(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	return m_pMainToolImpl->OnMenu(menuType, wParam);
}

CRect CMainAnalysisTool::GetGraphTotalRegion(CPoint point)		
{
	return m_pMainToolImpl->GetGraphTotalRegion(point);
}

BOOL CMainAnalysisTool::GetCurrentPosData(CPoint point,CString &strTextRight,CString &strTextBottom)
{
	return m_pMainToolImpl->GetCurrentPosData(point,strTextRight,strTextBottom);
}
// ----------------------------------------------------------------------------
void CMainAnalysisTool::OnDraw(CDC* pDC,BOOL bDrawAfter)
{
	m_pMainToolImpl->OnDraw(pDC);
}

void CMainAnalysisTool::DrawTool(CDC* pDC)
{
	m_pMainToolImpl->DrawTool(pDC);
}
// ----------------------------------------------------------------------------
// element �����.
void CMainAnalysisTool::BuildElementsFromString(const CString& strElementsData)
{
	m_pMainToolImpl->BuildElementsFromString(strElementsData);
}

// block �߰��� elementTree�� �߰��ϱ�.
bool CMainAnalysisTool::InsertElementTree(const CBlockIndex& insertBlockIndex)
{
	return m_pMainToolImpl->InsertElementTree(insertBlockIndex);
}

// block �̵�
bool CMainAnalysisTool::MoveElementTree(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex)
{
	return m_pMainToolImpl->MoveElementTree(sourceBlockIndex, targetBlockIndex);
}

// ----------------------------------------------------------------------------
// ��� element ����
void CMainAnalysisTool::DeleteAllElement()
{
	m_pMainToolImpl->DeleteAllElement();
}

// Ư�� element�� ����
bool CMainAnalysisTool::DeleteElement(const CToolEnumData::ElementType eElementTYPE)
{
	return m_pMainToolImpl->DeleteElement(eElementTYPE);
}

// ���� ���õ� element ����
bool CMainAnalysisTool::DeleteCurrentElement()
{
	return m_pMainToolImpl->DeleteCurrentElement();
}

// �ֱٿ� ����������  element ����
void CMainAnalysisTool::DeleteTail()
{
	m_pMainToolImpl->DeleteTail();
}

BOOL CMainAnalysisTool::DeleteElementPos(const long lAddress)
{
	return m_pMainToolImpl->DeleteElementPos(lAddress);
}

// �ش� block�� �ִ� ��� element ����
void CMainAnalysisTool::DeleteAllElementInBlock(const CBlockIndex& blockIndex)
{
	m_pMainToolImpl->DeleteAllElementInBlock(blockIndex);
}

// ----------------------------------------------------------------------------
// element �� ����
void CMainAnalysisTool::ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor)
{
	m_pMainToolImpl->ChangeAllElementsColor(penColor, textColor);
}

BOOL CMainAnalysisTool::UndoElement(long lUndoID)
{
	return m_pMainToolImpl->UndoElement(lUndoID);
}

BOOL CMainAnalysisTool::RedoElement(long lRedoID)
{
	return m_pMainToolImpl->RedoElement(lRedoID);
}

// 20080916  ������		�м��� �ڵ� ����
BOOL CMainAnalysisTool::DoesAction() const
{
	return m_pMainToolImpl->DoesAction();
}

// 20081007 JS.Kim	����ڽð��� ����
bool CMainAnalysisTool::RecalcElementXPoint(const char* szName, int nTimeDiff)
{
	return m_pMainToolImpl->RecalcElementXPoint(szName, nTimeDiff);
}
