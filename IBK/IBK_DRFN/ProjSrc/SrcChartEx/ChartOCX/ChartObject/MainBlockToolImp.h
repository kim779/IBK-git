// MainBlockToolImp.h: interface for the CMainBlockToolImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINBLOCKTOOLIMP_H__3FC2AD6C_7ED3_4261_8B7C_0087AA269F57__INCLUDED_)
#define AFX_MAINBLOCKTOOLIMP_H__3FC2AD6C_7ED3_4261_8B7C_0087AA269F57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/Dll_Load/MainBlockTool.h"		// for CMainBlockTool
#include "../Include_Chart/BlockBaseData.h"
#include "BaseToolSingleton.h"								// for CBaseToolSingleton


class CMainBlockToolImp : public CMainBlockTool
{
public:
	CMainBlockToolImp(IChartCtrl* p_pICharetCtrl);

	void SetIChartCtrl(IChartCtrl* p_pICharetCtrl);
	// tool�� ������� ����
	// �� �ִ�ȭ �߰� - ojtaso (20080519)
	void SetUsingTool(const bool bIsUsing_compartLine, const bool bIsUsing_blockMove, const bool bIsUsing_blockDelete, 
		const bool bIsUsing_horzScale, const bool bIsUsing_vertScale, const bool bIsUsing_graphTitle, const bool bIsUsing_subGraph, 
		const bool bIsUsing_graphMove, const bool bIsUsing_graphDelete, const bool bIsUsing_blockSize);
	void SetUsingTool(const CBlockBaseData::TOOLTYPE eToolType, const bool bIsUsing);
	// ���õ� Tool�� ������ Reset�ϰ� ���������� �Ѵ�.
	void ResetCurrentTool();
	CBlockBaseData::TOOLTYPE GetCurrentToolType() const;

	// block �̵�
	bool OnLButtonDown(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	bool OnLButtonUp(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	bool OnLButtonDblClk(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	CMenuType::MENUTYPE OnRButtonUp(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	bool OnMouseMove(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);

	bool OnMenu(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);
	// ��ǥ ������ - ojtaso (20071023)
	bool GetMenuCheck(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);

	// ocx���� OnDraw() ȣ���.
	void OnDraw(CDC* pDC, CMainBlock* pMainBlock);

private:
	IChartCtrl* m_pIChartCtrl;
	CBaseToolSingleton m_toolSingleton;
	CBaseTool* m_pBaseTool;
	CBaseTool* m_pOldBaseTool;

	void Invalidate();
	void InitializeAllData(const bool bInProcessEnded = false);
	bool IsActionOfTool(CMainBlock* pMainBlock) const;
	bool IsSelectingTool(CMainBlock* pMainBlock) const;
	bool IsScaleTool(CBaseTool* pBaseTool) const;

	// ���� ���Ǵ� CBaseTool�� m_pBaseTool�� setting.
	void SetCurrentBaseTool(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point, const bool useScaleTool);
	void SetCurrentBaseTool_OnRButton(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point, const bool useScaleTool);

	bool OnMouseMove_ProcessFind(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
};

#endif // !defined(AFX_MAINBLOCKTOOLIMP_H__3FC2AD6C_7ED3_4261_8B7C_0087AA269F57__INCLUDED_)
