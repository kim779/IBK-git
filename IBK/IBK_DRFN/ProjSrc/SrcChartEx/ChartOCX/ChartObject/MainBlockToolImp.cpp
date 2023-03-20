// MainBlockToolImp.cpp: implementation of the CMainBlockToolImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainBlockToolImp.h"

#include "../Include_Chart/Dll_Load/MainBlock.h"
#include "../Include_Chart/IChartCtrl.h"				// for IChartCtrl

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CMainBlockToolImp

CMainBlockToolImp::CMainBlockToolImp(IChartCtrl* p_pICharetCtrl):
	m_pIChartCtrl(p_pICharetCtrl),
	m_pBaseTool(NULL),
	m_pOldBaseTool(NULL)
{
}

// public =====================================================================
void CMainBlockToolImp::SetIChartCtrl(IChartCtrl* p_pICharetCtrl)
{
	m_pIChartCtrl = p_pICharetCtrl;
	m_toolSingleton.SetIChartCtrl(p_pICharetCtrl);
}

// tool�� ������� ����
// �� �ִ�ȭ �߰� - ojtaso (20080519)
void CMainBlockToolImp::SetUsingTool(const bool bIsUsing_compartLine, const bool bIsUsing_blockMove, const bool bIsUsing_blockDelete, 
		const bool bIsUsing_horzScale, const bool bIsUsing_vertScale, const bool bIsUsing_graphTitle, const bool bIsUsing_subGraph, 
		const bool bIsUsing_graphMove, const bool bIsUsing_graphDelete, const bool bIsUsing_blockSize)
{
	// block ũ�� tool
	SetUsingTool(CBlockBaseData::TOOL_COMPARTLINE, bIsUsing_compartLine);
	// block �̵� tool
	SetUsingTool(CBlockBaseData::TOOL_BLOCKMOVE, bIsUsing_blockMove);
	// block ���� tool
	SetUsingTool(CBlockBaseData::TOOL_BLOCKDELETE, bIsUsing_blockDelete);
	// x�� scale tool
	SetUsingTool(CBlockBaseData::TOOL_HORZSCALE, bIsUsing_horzScale);
	// y�� scale tool
	SetUsingTool(CBlockBaseData::TOOL_VERTSCALE, bIsUsing_vertScale);
	// graph title tool (����, ����)
	SetUsingTool(CBlockBaseData::TOOL_GRAPHTITLE, bIsUsing_graphTitle);
	// sugGraph tool	(����, ����, ����)
	SetUsingTool(CBlockBaseData::TOOL_SUBGRAPH, bIsUsing_subGraph);
	// graph title / subGraph tool �� �̵�
	SetUsingTool(CBlockBaseData::TOOL_GRAPHMOVE, bIsUsing_graphMove);
	// graph title / subGraph tool �� ����
	SetUsingTool(CBlockBaseData::TOOL_GRAPHDELETE, bIsUsing_graphDelete);
	// �� �ִ�ȭ �߰� - ojtaso (20080519)
	SetUsingTool(CBlockBaseData::TOOL_BLOCKSIZE, bIsUsing_blockSize);
}

void CMainBlockToolImp::SetUsingTool(const CBlockBaseData::TOOLTYPE eToolType, const bool bIsUsing)
{
	m_toolSingleton.SetUsingTool(eToolType, bIsUsing);
}

// ���õ� Tool�� ������ Reset�ϰ� ���������� �Ѵ�.
void CMainBlockToolImp::ResetCurrentTool()
{
	if(m_pBaseTool != NULL)
		m_pBaseTool->InitializeAllData();
	
	m_pBaseTool = NULL;
	m_pOldBaseTool = NULL;
}

// ----------------------------------------------------------------------------
CBlockBaseData::TOOLTYPE CMainBlockToolImp::GetCurrentToolType() const
{
	if(m_pBaseTool == NULL)
		return CBlockBaseData::TOOL_NONE;
	return m_pBaseTool->GetToolType();
}

// ----------------------------------------------------------------------------
// block �̵�
bool CMainBlockToolImp::OnLButtonDown(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point)
{
	SetCurrentBaseTool(pDC, pMainBlock, point, false);
	if(!IsActionOfTool(pMainBlock))
		return false;

	if(!m_pBaseTool) return true;

	// �� �ִ�ȭ �߰� - ojtaso (20080519)
	if( m_pBaseTool->GetToolType() == CBlockBaseData::TOOL_COMPARTLINE && pMainBlock->GetMaximizedBlock().IsAllValueBiggerThanZero())
		return false;

	bool bResult = m_pBaseTool->OnLButtonDown(pDC, pMainBlock, nFlags, point);
	if(bResult)
		Invalidate();

	return bResult;
}

bool CMainBlockToolImp::OnLButtonUp(CDC* pDC, CMainBlock* pMainBlock, const UINT /*nFlags*/, const CPoint& point)
{
	if(!m_pBaseTool) return true;

	// �� �ִ�ȭ �߰� - ojtaso (20080519)
	if( m_pBaseTool->GetToolType() == CBlockBaseData::TOOL_COMPARTLINE && pMainBlock->GetMaximizedBlock().IsAllValueBiggerThanZero())
		return false;

	bool result = false;
	if(IsActionOfTool(pMainBlock)){
		result = m_pBaseTool->OnLButtonUp(pDC, pMainBlock, point);
		Invalidate();
	}
	
	InitializeAllData();
	return result;
}

bool CMainBlockToolImp::OnLButtonDblClk(CDC* pDC, CMainBlock* pMainBlock, const UINT /*nFlags*/, const CPoint& point)
{
	SetCurrentBaseTool(pDC, pMainBlock, point, true);
	if(!IsActionOfTool(pMainBlock))
		return false;

	if(!m_pBaseTool) return true;
	bool bResult = m_pBaseTool->OnLButtonDblClk(pDC, pMainBlock, point);
	if(bResult)
		Invalidate();
	InitializeAllData();
	// (2007/1/3 - Seung-Won, Bae) for deny Next AddIn LButtonDlbClick Process
	return true;
}

CMenuType::MENUTYPE CMainBlockToolImp::OnRButtonUp(CDC* pDC, CMainBlock* pMainBlock, const UINT /*nFlags*/, const CPoint& point)
{
	SetCurrentBaseTool_OnRButton(pDC, pMainBlock, point, true);
	if(!IsActionOfTool(pMainBlock)){
		InitializeAllData(true);
		return CMenuType::MENU_NONE;
	}

	if(!m_pBaseTool) return CMenuType::MENU_NONE;
	CMenuType::MENUTYPE result = m_pBaseTool->OnRButtonUp(pDC, pMainBlock, point);
	InitializeAllData();
	return result;
}

bool CMainBlockToolImp::OnMouseMove(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point)
{
	if(!IsSelectingTool(pMainBlock))
		return false;
	
	if(m_pBaseTool == NULL)
		return OnMouseMove_ProcessFind(pDC, pMainBlock, nFlags, point);

	m_pOldBaseTool = NULL;
	
	// �� �ִ�ȭ �߰� - ojtaso (20080519)
	if( m_pBaseTool->GetToolType() == CBlockBaseData::TOOL_COMPARTLINE && pMainBlock->GetMaximizedBlock().IsAllValueBiggerThanZero())
		return false;

	m_pBaseTool->OnMouseMove(pDC, pMainBlock, nFlags, point);
	return false;
}

bool CMainBlockToolImp::OnMenu(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	bool result = false;
	if(IsActionOfTool(pMainBlock)){
		result = m_pBaseTool->OnMenu(pMainBlock, menuType, wParam);
		if(result)
			Invalidate();
	}

	InitializeAllData();
	return result;
}

// ��ǥ ������ - ojtaso (20071023)
bool CMainBlockToolImp::GetMenuCheck(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	bool result = false;
	if(IsActionOfTool(pMainBlock)){
		result = m_pBaseTool->GetMenuCheck(pMainBlock, menuType, wParam);
	}

	InitializeAllData();
	return result;
}

void CMainBlockToolImp::OnDraw(CDC* pDC, CMainBlock* pMainBlock)
{
	// tool�� ���õ� �⺻ �׸� �׸���
	m_toolSingleton.OnDraw(pDC, pMainBlock);

	// ���� ����ǰ� �ִ� Tool �� ���� ���.
	if(IsActionOfTool(pMainBlock))
		m_pBaseTool->OnDraw(pDC, pMainBlock);

	if(m_pOldBaseTool != NULL)
		m_pOldBaseTool->OnDraw(pDC, pMainBlock);
}


// private ====================================================================
void CMainBlockToolImp::Invalidate()
{
	if(m_pIChartCtrl != NULL)
		m_pIChartCtrl->InvalidateOCX();
}

// ��� tool�� ���� �ʱ�ȭ �Ѵ�.
void CMainBlockToolImp::InitializeAllData(const bool bInProcessEnded)
{
	if(m_pBaseTool != NULL){
		if(!bInProcessEnded && !m_pBaseTool->DoesProcessEnded())
			return;
		m_pBaseTool->InitializeAllData();
	}
		
	m_pBaseTool = NULL;
	m_pOldBaseTool = NULL;
}

// tool �� �����ϴ�.
bool CMainBlockToolImp::IsActionOfTool(CMainBlock* pMainBlock) const
{
	return (pMainBlock != NULL && m_pBaseTool != NULL && m_pBaseTool->GetIChartCtrl() != NULL);
}

// tool ���ÿ� ���� ����.
bool CMainBlockToolImp::IsSelectingTool(CMainBlock* pMainBlock) const
{
	if(pMainBlock == NULL)
		return false;

	if(!pMainBlock->GetDesignMode())
		return true;

	return (pMainBlock->GetBlockCompartType() == CBlockBaseData::COMPARTLINE);
}

// ������ Tool�� scale Tool ������ ����
bool CMainBlockToolImp::IsScaleTool(CBaseTool* pBaseTool) const
{
	if(pBaseTool == NULL)
		return false;
	
	return (pBaseTool->GetToolType() == CBlockBaseData::TOOL_HORZSCALE || pBaseTool->GetToolType() == CBlockBaseData::TOOL_VERTSCALE);
}

// ----------------------------------------------------------------------------
// ���� ���Ǵ� CBaseTool�� m_pBaseTool�� setting.
void CMainBlockToolImp::SetCurrentBaseTool(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point, const bool useScaleTool)
{
	if(!IsSelectingTool(pMainBlock))
		return;

	InitializeAllData(true);

	m_pBaseTool = m_toolSingleton.GetTool(pDC, pMainBlock, point, useScaleTool);
}

void CMainBlockToolImp::SetCurrentBaseTool_OnRButton(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point, const bool useScaleTool)
{
	if(m_pBaseTool != NULL && !IsScaleTool(m_pBaseTool))
		return;

	SetCurrentBaseTool(pDC, pMainBlock, point, useScaleTool);
}

// ----------------------------------------------------------------------------
bool CMainBlockToolImp::OnMouseMove_ProcessFind(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point)
{
	CBaseTool* pBaseTool = m_toolSingleton.GetTool(pDC, pMainBlock, point, false, false);

	if(m_pOldBaseTool != pBaseTool)
		Invalidate();

	m_pOldBaseTool = pBaseTool;
	if(pBaseTool == NULL)
		return false;

	pBaseTool->OnMouseMove(pDC, pMainBlock, nFlags, point);
	return true;
}
