// BlockTool.h: interface for the CBlockTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKTOOL_H__209534FC_5C91_43C1_9167_63CEC0ED9A9B__INCLUDED_)
#define AFX_BLOCKTOOL_H__209534FC_5C91_43C1_9167_63CEC0ED9A9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tool.h"
// �� �ִ�ȭ �߰� - ojtaso (20080519)
#include <afxtempl.h>

class CMainBlock;
class CGraphRegionColor;

// ���� block�� ã�� Tool
class CBlockTool : public CBaseTool
{
public:
	// Tool�� ��� ������ �ʱ�ȭ
	virtual void InitializeAllData();

	// mouse
	virtual bool OnLButtonDown(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	virtual bool OnLButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual void OnMouseMove(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	virtual void OnDraw(CDC* pDC, CMainBlock* pMainBlock);
	void DrawSelectionButton(CDC* pDC, CMainBlock* pMainBlock);

	bool Find(CMainBlock* pMainBlock, const CPoint& point);

protected:
	virtual bool FindBlock(CMainBlock* pMainBlock, const CPoint& point, CBlockIndex& blockIndex) = 0;
	virtual void DrawSelectionBlock(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor) = 0;
	virtual bool Executing(CMainBlock* pMainBlock, const CBlockIndex& targetBlockIndex) = 0;

	void DrawSelectionBlock(CDC* pDC, CMainBlock* pMainBlock, const CBlockIndex& blockIndex);
	bool Executing(CMainBlock* pMainBlock, const CPoint& point);
	CBlockIndex GetSourceBlockIndex() const;

private:
	CBlockIndex m_sourceBlockIndex; // block ��ȣ

	bool IsDrawSelectionButton(CMainBlock* pMainBlock) const;
	void GetColor(const CGraphRegionColor& graphRegionColor, COLORREF& frameColor, COLORREF& backgroundColor) const;
};


// block move tool
class CBlockMoveTool : public CBlockTool
{
public:
	virtual CBlockBaseData::TOOLTYPE GetToolType() const;

protected:
	virtual bool FindBlock(CMainBlock* pMainBlock, const CPoint& point, CBlockIndex& blockIndex);
	virtual void DrawSelectionBlock(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor);
	virtual bool Executing(CMainBlock* pMainBlock, const CBlockIndex& targetBlockIndex);
};


// block delete tool
class CBlockDeleteTool : public CBlockTool
{
public:
	virtual CBlockBaseData::TOOLTYPE GetToolType() const;

protected:
	virtual bool FindBlock(CMainBlock* pMainBlock, const CPoint& point, CBlockIndex& blockIndex);
	virtual void DrawSelectionBlock(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor);
	virtual bool Executing(CMainBlock* pMainBlock, const CBlockIndex& targetBlockIndex);

private:
	bool DeleteBlock(CMainBlock* pMainBlock, const CBlockIndex& targetBlockIndex);
};

// �� �ִ�ȭ �߰� - ojtaso (20080519)
class CBlockSizeTool : public CBlockTool
{
public:
	CBlockSizeTool();
	virtual CBlockBaseData::TOOLTYPE GetToolType() const;

protected:
	virtual bool FindBlock(CMainBlock* pMainBlock, const CPoint& point, CBlockIndex& blockIndex);
	virtual void DrawSelectionBlock(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor);
	virtual bool Executing(CMainBlock* pMainBlock, const CBlockIndex& targetBlockIndex);
};

// ���м��� ���� Tool
class CCompartLineTool : public CBaseTool
{
public:
	// Tool�� ��� ������ �ʱ�ȭ
	virtual void InitializeAllData();
	virtual CBlockBaseData::TOOLTYPE GetToolType() const;

	// mouse
	virtual bool OnLButtonDown(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	virtual bool OnLButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual void OnMouseMove(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);

	virtual void OnDraw(CDC* pDC, CMainBlock* pMainBlock);

	// �ش� ���м� ã�´�.
	bool Find(CMainBlock* pMainBlock, const CPoint& point);

private:
	bool m_bPreMMovePointUsed;		// m_preMMovePt ��� ����
	CPoint m_lBtDownPt;				// �巡�׸� ���� ó�� ��ġ�� �����Ѵ�.
	CPoint m_preMMovePt;			// mouse move point
	CBlockIndex m_compartLineIndex; // ���м��� ��ȣ

	bool IsPermissionChangingCompartLine(const CRect& blockRegion, const CPoint& point) const;
	CPoint GetTwoPointGap(const CPoint& pt1, const CPoint& pt2) const;
	CBaseToolEnumData::CURSORTYPE GetBlockCursorType(const CBlockIndex& compartLineIndex) const;

	void DrawCompartLine_Move(CDC* pDC, const CRect& region, const CPoint& point);
	void DrawCompartLine(CDC* pDC, const CRect& region, const CPoint& point);
	void DrawLine(CDC* pDC, const CPoint& pt1, const CPoint& pt2);
};

#endif // !defined(AFX_BLOCKTOOL_H__209534FC_5C91_43C1_9167_63CEC0ED9A9B__INCLUDED_)
