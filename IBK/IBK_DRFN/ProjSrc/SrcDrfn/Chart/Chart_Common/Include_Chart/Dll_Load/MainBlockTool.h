// MainBlockTool.h: interface for the CMainBlockTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINBLOCKTOOL_H__83E090D9_A6F4_4795_95C3_B39DEBCC7ED6__INCLUDED_)
#define AFX_MAINBLOCKTOOL_H__83E090D9_A6F4_4795_95C3_B39DEBCC7ED6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"

class CCommonInterface;
class CMainBlock;
class CMainBlockToolImplementation;


class AFX_EXT_CLASS CMainBlockTool
{
public:
	CMainBlockTool();
	CMainBlockTool(CCommonInterface* pParentIF);
	virtual ~CMainBlockTool();

	void SetParentInterface(CCommonInterface* pParentIF);
	// tool�� �ܼ�/������������ ����
	void SetSimpleModeTool(const bool bIsSimpleModeTool = true);
	// tool�� ������� ����
	void SetUsingTool(const bool bIsUsing_compartLine, const bool bIsUsing_blockMove, const bool bIsUsing_blockDelete, 
		const bool bIsUsing_horzScale, const bool bIsUsing_vertScale, const bool bIsUsing_graphTitle, const bool bIsUsing_subGraph, 
		const bool bIsUsing_graphMove, const bool bIsUsing_graphDelete);
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

	// ocx���� OnDraw() ȣ���.
	void OnDraw(CDC* pDC, CMainBlock* pMainBlock);

private:
	CMainBlockToolImplementation* m_pMainBlockToolImpl;
};

#endif // !defined(AFX_MAINBLOCKTOOL_H__83E090D9_A6F4_4795_95C3_B39DEBCC7ED6__INCLUDED_)
