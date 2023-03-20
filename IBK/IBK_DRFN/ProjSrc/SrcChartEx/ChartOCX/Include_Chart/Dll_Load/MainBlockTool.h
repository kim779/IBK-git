// MainBlockTool.h: interface for the CMainBlockTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINBLOCKTOOL_H__83E090D9_A6F4_4795_95C3_B39DEBCC7ED6__INCLUDED_)
#define AFX_MAINBLOCKTOOL_H__83E090D9_A6F4_4795_95C3_B39DEBCC7ED6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../BlockBaseData.h"

interface IChartCtrl;
class CMainBlock;

// �� �ִ�ȭ �߰� - ojtaso (20080519)
interface IPropertyMap;
class CMainBlockTool
{
public:
	virtual ~CMainBlockTool()	{};

public:
	virtual void SetIChartCtrl(IChartCtrl* p_pICharetCtrl)	= 0;
	// tool�� ������� ����
	// �� �ִ�ȭ �߰� - ojtaso (20080519)
	virtual void SetUsingTool(const bool bIsUsing_compartLine, const bool bIsUsing_blockMove, const bool bIsUsing_blockDelete, 
		const bool bIsUsing_horzScale, const bool bIsUsing_vertScale, const bool bIsUsing_graphTitle, const bool bIsUsing_subGraph, 
		const bool bIsUsing_graphMove, const bool bIsUsing_graphDelete, const bool bIsUsing_blockSize)	= 0;
	virtual void SetUsingTool(const CBlockBaseData::TOOLTYPE eToolType, const bool bIsUsing)	= 0;
	// ���õ� Tool�� ������ Reset�ϰ� ���������� �Ѵ�.
	virtual void ResetCurrentTool()	= 0;
	virtual CBlockBaseData::TOOLTYPE GetCurrentToolType() const	= 0;

	// block �̵�
	virtual bool OnLButtonDown(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point)	= 0;
	virtual bool OnLButtonUp(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point)	= 0;
	virtual bool OnLButtonDblClk(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point)	= 0;
	virtual CMenuType::MENUTYPE OnRButtonUp(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point)	= 0;
	virtual bool OnMouseMove(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point)	= 0;

	virtual bool OnMenu(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)	= 0;
	// ��ǥ ������ - ojtaso (20071023) 
	virtual bool GetMenuCheck(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)= 0;

	// ocx���� OnDraw() ȣ���.
	virtual void OnDraw(CDC* pDC, CMainBlock* pMainBlock)	= 0;
};

typedef CMainBlockTool *(*FN_CreateBlockTool)( void); 

#endif // !defined(AFX_MAINBLOCKTOOL_H__83E090D9_A6F4_4795_95C3_B39DEBCC7ED6__INCLUDED_)
