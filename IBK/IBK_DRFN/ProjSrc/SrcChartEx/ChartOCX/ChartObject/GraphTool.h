// GraphTool.h: interface for the CGraphTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHTOOL_H__D219A3D9_B1ED_41D4_BC93_1EF641BE86B7__INCLUDED_)
#define AFX_GRAPHTOOL_H__D219A3D9_B1ED_41D4_BC93_1EF641BE86B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/BaseData.h"	// 20081007 JS.Kim	����ڽð��� ����

#include "Tool.h"
#include "GraphMove.h"
#include "SelectedSubGraphDeque.h"

class CBlockImp;
interface IChartCtrl;

class CGraphTool : public CBaseTool
{
public:
	CGraphTool();

	void SetIChartCtrl(IChartCtrl* p_pICharetCtrl);
	void SetGraphMove(const bool bIsGraphMove);
	void SetGraphDelete(const bool bIsGraphDelete);
	// �� �ִ�ȭ �߰� - ojtaso (20080519)
	void SetGraphSize(const bool bIsGraphSize);

	// Tool�� ��� ������ �ʱ�ȭ
	virtual void InitializeAllData();
	virtual bool OnLButtonDown(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	virtual bool OnLButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual void OnMouseMove(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	virtual bool OnMenu(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);
	// ��ǥ ������ - ojtaso (20071023)
	virtual bool GetMenuCheck(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);

	// ��ǥ ������ - ojtaso (20071023)
	virtual bool OnMenu_SubGraphInvert(CMainBlock* pMainBlock, CSubGraph* pSubGraph);
	// ��ǥ ������ - ojtaso (20071023)
	virtual CGraphImp* IsGeneralGraph(CGraphImp* pGraph, CVertScaleGroup* pGroup);
	// ��ǥ ������ - ojtaso (20071023)
	virtual bool GetMenuCheck_SubGraphInvert(CMainBlock* pMainBlock, CSubGraph* pSubGraph);

	// 20081007 JS.Kim	����ڽð��� ����
	virtual bool OnMenu_SubGraphSelectTimeDiff(CMainBlock* pMainBlock, CSubGraph* pSubGraph, CTimeDiffInfo* pTimeInfo);
	virtual bool GetMenuCheck_SubGraphSelectTimeDiff(CMainBlock* pMainBlock, CSubGraph* pSubGraph, CTimeDiffInfo* pTimeInfo);

protected:
	virtual void OnLButtonDown_Real(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point) = 0;
	virtual bool OnLButtonUp() = 0;
	virtual void OnMouseMove(CDC* pDC, CMainBlock* pMainBlock) = 0;
	virtual bool OnMenu_SubGraph(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam) = 0;
	// ��ǥ ������ - ojtaso (20071023)
	virtual bool GetMenuCheck_SubGraph(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam) = 0;
	virtual bool IsSelectedSubGraph() const = 0;
	virtual bool IsSendMessage_SubGraph() const = 0;
	virtual CGraphImp* GetGraphMoving() = 0;
	virtual void GraphMoving_Ended(const bool bGraphMovingResult) = 0;

	// graph ��������.
	CGraphImp* GetGraph(CSubGraph* pSubGraph) const;
	
	// <��ǥ����> �� �°� ���� �����Ų��. - graph ���� �� ����.
	bool LoadIndicatorConfiguration(CMainBlock* pMainBlock, CSubGraph* pSubGraph);
	// data index�� ocx�� �ֱ�
	bool OnMenu_DataIndex(const CSelectedSubGraphData& selectedSubGraphData);
	// ���� scale �ٲٱ�
	void ChangeAllVertScaleGridType(CVertScaleGroup* pVertScaleGroup);

	// ocx�� �޽����� ������.
	void SendMessage_GraphType(CSubGraph* pSubGraph);

private:
	bool IsCursorType_NoneMode(const CPoint& point) const;

	// block ��������.
	CBlockImp* GetBlock(CGraphImp* pGraph) const;
	// cursor type ��������.
	CBaseToolEnumData::CURSORTYPE GetCursorType(CMainBlock* pMainBlock, const CPoint& point) const;

	// graph move
	void SetGraphMoving(const bool bIsGraphMoving);
	bool GraphMoving(CMainBlock* pMainBlock, const CPoint& targetPoint);

private:
	bool m_bIsGraphMove;
	bool m_bIsGraphDelete;
	CPoint m_lBtDownPt;
	CGraphMove m_graphMove;
};

// subGraph title �� ã�� Tool
class CSubGraphTitleTool : public CGraphTool
{
public:
	CSubGraphTitleTool();

	// Tool�� ��� ������ �ʱ�ȭ
	virtual void InitializeAllData();
	virtual CBlockBaseData::TOOLTYPE GetToolType() const;

	// mouse
	virtual bool OnLButtonDblClk(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual CMenuType::MENUTYPE OnRButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual void OnDraw(CDC* pDC, CMainBlock* pMainBlock);

	// �ش� graph�� ã�´�.
	bool Find(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	// <��ǥ����> �� �°� ���� �����Ų��. - graph ���� �� ����.
	bool LoadIndicatorConfiguration(CMainBlock* pMainBlock);

protected:
	virtual void OnLButtonDown_Real(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	virtual bool OnLButtonUp();
	virtual void OnMouseMove(CDC* pDC, CMainBlock* pMainBlock);
	virtual bool OnMenu_SubGraph(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);
	// ��ǥ ������ - ojtaso (20071023)
	virtual bool GetMenuCheck_SubGraph(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);
	virtual bool IsSelectedSubGraph() const;
	virtual bool IsSendMessage_SubGraph() const;
	virtual CGraphImp* GetGraphMoving();
	virtual void GraphMoving_Ended(const bool bGraphMovingResult);

private:
	bool FindSubGraphTitle(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point, CSubGraph*& pSubGraph);

	// ���� ���õ� subGraph�� vert scale�� ���̰� �Ѵ�.
	void ChangeAllVertScaleGridType();

	// subgraph title�� ���õ� ���
	void DrawSelectedSubGraphTitle(CDC* pDC);
	bool DrawSelectedSubGraphGrip(CDC* pDC);

	bool IsLoadIndicatorConfiguration(const CPoint& point) const;
	CSubGraph* GetSubGraph() const;
	// subgraph title�� ���� ����
	CRect GetSelectedSubGraphTitleRegion(const CRect& subGraphTitleRegion) const;
	
	void InitializeTitleRegion();

	// OnMenu
	bool OnMenu_SubGraphHide(const bool bIsAction);
	bool OnMenu_SubGraphSet(CMainBlock* pMainBlock);

private:
	CSelectedSubGraphData m_selectedSubGraphData;
	CRect m_selectedSubGraphTitleRegion;
	CRect m_oldSelectedSubGraphTitleRegion;
};


class CSubGraphTool : public CGraphTool
{
public:
	// Tool�� ��� ������ �ʱ�ȭ
	virtual void InitializeAllData();
	virtual CBlockBaseData::TOOLTYPE GetToolType() const;

	// mouse
	virtual bool OnLButtonDblClk(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual CMenuType::MENUTYPE OnRButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual void OnDraw(CDC* pDC, CMainBlock* pMainBlock);

	// �ش� graph�� ã�´�.
	bool Find(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);

protected:
	virtual void OnLButtonDown_Real(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	virtual bool OnLButtonUp();
	virtual void OnMouseMove(CDC* pDC, CMainBlock* pMainBlock);
	virtual bool OnMenu_SubGraph(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);
	// ��ǥ ������ - ojtaso (20071023)
	virtual bool GetMenuCheck_SubGraph(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);
	virtual bool IsSelectedSubGraph() const;
	virtual bool IsSendMessage_SubGraph() const;
	virtual CGraphImp* GetGraphMoving();
	virtual void GraphMoving_Ended(const bool bGraphMovingResult);

private:
	// subGraph ã��.
	void FindSubGraph(CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	bool FindSubGraph(CMainBlock* pMainBlock, const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque);

//	void SetSelectedCursor(const bool bIsSelectedCursor);

	// ���� ���õ� subGraph�� vert scale�� ���̰� �Ѵ�.
	void ChangeAllVertScaleGridType();

	// subGraph ����
	bool ReversedSubGraphHighlight(CMainBlock* pMainBlock, const CPoint& point);
	// data index�� ocx�� �ֱ�
	bool SelectedGraphDataIndex();
	// OnMenu
	bool OnMenu_SubGraphSet(CMainBlock* pMainBlock);
	bool OnMenu_Etc(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);

private:
	CSelectedSubGraphDeque m_selectedSubGraphDeque;
};

#endif // !defined(AFX_GRAPHTOOL_H__D219A3D9_B1ED_41D4_BC93_1EF641BE86B7__INCLUDED_)
