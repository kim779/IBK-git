// Tool.h: interface for the CTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOL_H__8271865E_145F_401B_9A3F_DD98FCC88A86__INCLUDED_)
#define AFX_TOOL_H__8271865E_145F_401B_9A3F_DD98FCC88A86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/BlockBaseData.h"

interface IChartCtrl;
class CMainBlock;

class CBaseToolEnumData
{
public:
	enum CURSORTYPE
	{
		CTNONE = 0,		// cursor ���� ����
		SPLITE_ROW,		// block ������ġ ���� cursor
		SPLITE_COLUMN,	// block ������ġ ���� cursor
		SPLITE_ALL,		// block�� ��� ��ġ ���� cursor
		SELECT,			// ��ü ���� cursor
		MOVE,			// ���õ� ��ü �̵�
		NODROP,			// ���õ� ��ü �̵� �ȵ�
		REMOVE,			// Delete
		INSERT,			// Insert Graph �и�/���� - ojtaso (20070731)
	};
};


// �⺻ base Tool
class CBaseTool
{
public:
	CBaseTool();

	void SetIChartCtrl(IChartCtrl* p_pICharetCtrl);
	IChartCtrl* GetIChartCtrl() const;
	CWnd* GetParent() const;
	HWND GetParent_Handle();

	// Tool�� �۾� �ϷῩ�� (true = �۾��Ϸ�)
	bool DoesProcessEnded() const;

	// Tool�� ��� ������ �ʱ�ȭ
	virtual void InitializeAllData();
	// �ڽ��� ToolType�� ��������
	virtual CBlockBaseData::TOOLTYPE GetToolType() const = 0;

	// mouse
	virtual bool OnLButtonDown(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	virtual bool OnLButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual bool OnLButtonDblClk(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual CMenuType::MENUTYPE OnRButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual void OnMouseMove(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);

	virtual bool OnMenu(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);
	// ��ǥ ������ - ojtaso (20071023)
	virtual bool GetMenuCheck(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);

	virtual void OnDraw(CDC* pDC, CMainBlock* pMainBlock);

protected:
	void ChangeCursor(const CBaseToolEnumData::CURSORTYPE& cursorType);
	void SetProcessEnded(const bool isProcessEnded);

	void Invalidate(const bool bErase = true);
	void InvalidateRect(const CRect& region, const bool bErase = true);

	int GetMargin() const;

protected:
	IChartCtrl* m_pIChartCtrl; // �θ�(OCX) Interface
	bool m_bProcessEnded;	// Tool�� �۾��� ���� ����. (true = ����)
};

#endif // !defined(AFX_TOOL_H__8271865E_145F_401B_9A3F_DD98FCC88A86__INCLUDED_)
