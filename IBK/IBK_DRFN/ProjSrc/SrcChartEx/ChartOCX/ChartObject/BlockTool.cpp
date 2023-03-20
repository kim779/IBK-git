// BlockTool.cpp: implementation of the CBlockTool class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlockTool.h"

#include "../Include_Chart/Dll_Load/MainBlock.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "BlockImp.h"
#include "BlockDrawer.h"
#include "resource.h"									// for IDS_CANNOT_DELETE_PRICE_CHART

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CBlockTool

// public =====================================================================
// ��� data�� �ʱ�ȭ �Ѵ�.
void CBlockTool::InitializeAllData()
{
	CBaseTool::InitializeAllData();
	m_sourceBlockIndex.SetIndex(-1, -1);
}

// ----------------------------------------------------------------------------
bool CBlockTool::OnLButtonDown(CDC* pDC, CMainBlock* pMainBlock, const UINT /*nFlags*/, const CPoint& point)
{
	CBaseTool::SetProcessEnded(false);

	// �� �ִ�ȭ �߰� - ojtaso (20080519)
	if( pMainBlock->GetMaximizedBlock().IsAllValueBiggerThanZero() && !pMainBlock->GetMaximizedBlock().IsSameIndex( m_sourceBlockIndex))
		return false;

	DrawSelectionBlock(pDC, pMainBlock, m_sourceBlockIndex);
	return true;
}

bool CBlockTool::OnLButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point)
{
	CBaseTool::SetProcessEnded(true);

	// �� �ִ�ȭ �߰� - ojtaso (20080519)
	if( pMainBlock->GetMaximizedBlock().IsAllValueBiggerThanZero() && !pMainBlock->GetMaximizedBlock().IsSameIndex( m_sourceBlockIndex))
		return false;

	if(!m_sourceBlockIndex.IsAllValueBiggerThanZero())
		return false;

	return Executing(pMainBlock, point);
}

void CBlockTool::OnMouseMove(CDC* pDC, CMainBlock* pMainBlock, const UINT /*nFlags*/, const CPoint& /*point*/)
{
	// �� �ִ�ȭ �߰� - ojtaso (20080519)
	if( pMainBlock->GetMaximizedBlock().IsAllValueBiggerThanZero() && !pMainBlock->GetMaximizedBlock().IsSameIndex( m_sourceBlockIndex))
		return;

	if(CBaseTool::DoesProcessEnded())
		CBaseTool::ChangeCursor(CBaseToolEnumData::SELECT);
	else
		CBaseTool::ChangeCursor(CBaseToolEnumData::MOVE);

	DrawSelectionBlock(pDC, pMainBlock, m_sourceBlockIndex);
}

void CBlockTool::OnDraw(CDC* pDC, CMainBlock* pMainBlock)
{
	if(!IsDrawSelectionButton(pMainBlock))
		return;

	DrawSelectionBlock(pDC, pMainBlock, m_sourceBlockIndex);
}

void CBlockTool::DrawSelectionButton(CDC* pDC, CMainBlock* pMainBlock)
{
	if(!IsDrawSelectionButton(pMainBlock))
		return;

	COLORREF frameColor(RGB(255, 255, 255)), backgroundColor(RGB(0, 0, 0));
	GetColor(pMainBlock->GetGraphRegionColor(), frameColor, backgroundColor);
	for(int nColumnIndex = 0; nColumnIndex < pMainBlock->GetColumnCount(); nColumnIndex++)
	{
		for(int nRowIndex = 0; nRowIndex < pMainBlock->GetRowCount(); nRowIndex++)
		{
			CRect region = pMainBlock->GetNoFrameGraphRegion(CBlockIndex(nRowIndex, nColumnIndex));
			DrawSelectionBlock(pDC, region, frameColor, backgroundColor);
		}
	}
}

// ----------------------------------------------------------------------------
// �ش� block ��ȣ ã�´�.
bool CBlockTool::Find(CMainBlock* pMainBlock, const CPoint& point)
{
	InitializeAllData();
	if(!IsDrawSelectionButton(pMainBlock))
		return false;

	return FindBlock(pMainBlock, point, m_sourceBlockIndex);
}

// protected ==================================================================
// block üũ���� ���ÿ��ο� �°� �׸���.
void CBlockTool::DrawSelectionBlock(CDC* pDC, CMainBlock* pMainBlock, const CBlockIndex& blockIndex)
{
	if(!blockIndex.IsAllValueBiggerThanZero())
	{
		Invalidate();
		return;
	}

	CBlockDrawer().DrawFrameRegion(pDC, pMainBlock->GetBlockRegion(blockIndex), pMainBlock->GetGraphRegionColor().GetFrameColor(), 2);
	COLORREF frameColor(RGB(255, 255, 255)), backgroundColor(RGB(0, 0, 0));
	GetColor(pMainBlock->GetGraphRegionColor(), frameColor, backgroundColor);
	DrawSelectionBlock(pDC, pMainBlock->GetNoFrameGraphRegion(blockIndex), backgroundColor, frameColor);
}

bool CBlockTool::Executing(CMainBlock* pMainBlock, const CPoint& point)
{	
	if(pMainBlock == NULL)
		return false;

	CBlockIndex targetBlockIndex = pMainBlock->FindBlockIndex(point);
	if(!targetBlockIndex.IsAllValueBiggerThanZero())
		return false;

	return Executing(pMainBlock, targetBlockIndex);
}

CBlockIndex CBlockTool::GetSourceBlockIndex() const
{
	return m_sourceBlockIndex;
}


// private ====================================================================
bool CBlockTool::IsDrawSelectionButton(CMainBlock* pMainBlock) const
{
	if(pMainBlock == NULL)
		return false;
	
	return ((pMainBlock->GetColumnCount() * pMainBlock->GetRowCount()) > 1);
}

void CBlockTool::GetColor(const CGraphRegionColor& graphRegionColor, COLORREF& frameColor, COLORREF& backgroundColor) const
{
	backgroundColor = graphRegionColor.GetFrameColor();

	// backgroundColor �� ����迭�� ��� frame�� �Ⱥ��δ�.
	int color_R = GetRValue(backgroundColor);
	int color_G = GetGValue(backgroundColor);
	int color_B = GetBValue(backgroundColor);
	if(color_R > 200 && color_G > 200 && color_B > 200)
		frameColor = RGB(0, 0, 0);
	else
		frameColor = RGB(255, 255, 255);
}


///////////////////////////////////////////////////////////////////////////////
// class CBlockMoveTool


// public =====================================================================
CBlockBaseData::TOOLTYPE CBlockMoveTool::GetToolType() const
{
	return CBlockBaseData::TOOL_BLOCKMOVE;
}


// protected ==================================================================
// �ش� block ��ȣ ã�´�.
bool CBlockMoveTool::FindBlock(CMainBlock* pMainBlock, const CPoint& point, CBlockIndex& blockIndex)
{
	blockIndex = pMainBlock->FindBlockMovingRegion(point);
	return blockIndex.IsAllValueBiggerThanZero();
}

// ----------------------------------------------------------------------------
// block üũ���� ���ÿ��ο� �°� �׸���.
void CBlockMoveTool::DrawSelectionBlock(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor)
{
	if(region.IsRectEmpty())
		return;

	CBlockDrawer blockDrawer;
	blockDrawer.DrawBlockMove2DButton(pDC, region.TopLeft(), penColor, brushColor);
}

bool CBlockMoveTool::Executing(CMainBlock* pMainBlock, const CBlockIndex& targetBlockIndex)
{
	return pMainBlock->MoveBlock(CBlockTool::GetSourceBlockIndex(), targetBlockIndex);
}


///////////////////////////////////////////////////////////////////////////////
// class CBlockDeleteTool

// public =====================================================================
CBlockBaseData::TOOLTYPE CBlockDeleteTool::GetToolType() const
{
	return CBlockBaseData::TOOL_BLOCKDELETE;
}


// protected ==================================================================
// �ش� block ��ȣ ã�´�.
bool CBlockDeleteTool::FindBlock(CMainBlock* pMainBlock, const CPoint& point, CBlockIndex& blockIndex)
{
	blockIndex = pMainBlock->FindBlockDeletingRegion(point);
	return blockIndex.IsAllValueBiggerThanZero();
}

// ----------------------------------------------------------------------------
// block üũ���� ���ÿ��ο� �°� �׸���.
void CBlockDeleteTool::DrawSelectionBlock(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor)
{
	if(region.IsRectEmpty())
		return;

	CBlockDrawer().DrawBlockDelete2DButton(pDC, CPoint(region.right, region.top), penColor, brushColor);
}

bool CBlockDeleteTool::Executing(CMainBlock* pMainBlock, const CBlockIndex& targetBlockIndex)
{
	// RQ�� ��ǥ�˻� : �������� - ojtaso (20070109)
	if(!pMainBlock->IsExistenceIndicatorInBlock(_T("DEFAULT"), _MTEXT( C2_PRICE_CHART), targetBlockIndex))
		return DeleteBlock(pMainBlock, targetBlockIndex);

// "<������Ʈ>�� ���� �� �� �����ϴ�!! " �޽����� �ƾ� ǥ������ �ʴ´�.
// 	_MHWND( pMainBlock->GetParent_Handle());
// 	ML_SET_LANGUAGE_RES();
// 	CString strMsg;
// 	strMsg.LoadString( IDS_CANNOT_DELETE_PRICE_CHART);
// 	CString strTitle;
// 	strTitle.LoadString( IDS_WARNNING);
// 	MessageBox(CBaseTool::GetParent_Handle(), strMsg, strTitle, MB_ICONWARNING | MB_OK);
	return false;
}


// private ====================================================================
bool CBlockDeleteTool::DeleteBlock(CMainBlock* pMainBlock, const CBlockIndex& targetBlockIndex)
{
	if(pMainBlock == NULL)
		return false;

	return pMainBlock->DeleteBlock(targetBlockIndex, false);
}

///////////////////////////////////////////////////////////////////////////////
// class CBlockSizeTool

// public =====================================================================
// �� �ִ�ȭ �߰� - ojtaso (20080519)
CBlockSizeTool::CBlockSizeTool()
{
}

// �� �ִ�ȭ �߰� - ojtaso (20080519)
CBlockBaseData::TOOLTYPE CBlockSizeTool::GetToolType() const
{
	return CBlockBaseData::TOOL_BLOCKSIZE;
}

// protected ==================================================================
// �ش� block ��ȣ ã�´�.
// �� �ִ�ȭ �߰� - ojtaso (20080519)
bool CBlockSizeTool::FindBlock(CMainBlock* pMainBlock, const CPoint& point, CBlockIndex& blockIndex)
{
	blockIndex = pMainBlock->FindBlockSizingRegion(point);
	return blockIndex.IsAllValueBiggerThanZero();
}

// ----------------------------------------------------------------------------
// block üũ���� ���ÿ��ο� �°� �׸���.
// �� �ִ�ȭ �߰� - ojtaso (20080519)
void CBlockSizeTool::DrawSelectionBlock(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor)
{
	if(region.IsRectEmpty())
		return;

	CBlockDrawer().DrawBlockSize2DButton(pDC, CPoint(region.right, region.top), penColor, brushColor);
}

// �� �ִ�ȭ �߰� - ojtaso (20080519)
bool CBlockSizeTool::Executing(CMainBlock* pMainBlock, const CBlockIndex& targetBlockIndex)
{
	if( pMainBlock->GetMaximizedBlock().IsAllValueBiggerThanZero())
			pMainBlock->SetMaximizedBlock( CBlockIndex( -1, -1));
	else	pMainBlock->SetMaximizedBlock( targetBlockIndex);
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// class CCompartLineTool

// public =====================================================================
// ��� data�� �ʱ�ȭ �Ѵ�.
void CCompartLineTool::InitializeAllData()
{
	CBaseTool::InitializeAllData();
	m_compartLineIndex.SetIndex(-1, -1);
	m_bPreMMovePointUsed = false;
}

CBlockBaseData::TOOLTYPE CCompartLineTool::GetToolType() const
{
	return CBlockBaseData::TOOL_COMPARTLINE;
}

// ----------------------------------------------------------------------------
bool CCompartLineTool::OnLButtonDown(CDC* /*pDC*/, CMainBlock* /*pMainBlock*/, const UINT /*nFlags*/, const CPoint& point)
{
	CBaseTool::SetProcessEnded(false);
	m_lBtDownPt = point;
	CBaseTool::ChangeCursor(GetBlockCursorType(m_compartLineIndex));
	return (m_compartLineIndex.GetRow() > -1 || m_compartLineIndex.GetColumn() > -1);
}

bool CCompartLineTool::OnLButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point)
{
	CBaseTool::SetProcessEnded(true);

	// ���м��� ���� ���� ������ �Ѵ�.
	CRect blockRegion = pMainBlock->GetAllBlockRegion();
	if(!IsPermissionChangingCompartLine(blockRegion, point))
		return false;

	return pMainBlock->MoveBlocksPosition(GetTwoPointGap(m_lBtDownPt, point), m_compartLineIndex);
}

void CCompartLineTool::OnMouseMove(CDC* pDC, CMainBlock* pMainBlock, const UINT /*nFlags*/, const CPoint& point)
{
	if(pMainBlock == NULL || CBaseTool::DoesProcessEnded()){
		CBaseTool::ChangeCursor(GetBlockCursorType(m_compartLineIndex));
		return;
	}

	// ���м��� ���� ���� ������ �Ѵ�.
	CRect blockRegion = pMainBlock->GetAllBlockRegion();
	if(!IsPermissionChangingCompartLine(blockRegion, point))
		return;

	// mouse move�� ���м��� �׸���.
	DrawCompartLine_Move(pDC, blockRegion, point);
	CBaseTool::ChangeCursor(GetBlockCursorType(m_compartLineIndex));
}

void CCompartLineTool::OnDraw(CDC* pDC, CMainBlock* pMainBlock)
{
	if(m_bPreMMovePointUsed)
		DrawCompartLine(pDC, pMainBlock->GetAllBlockRegion(), m_preMMovePt);
}

// ----------------------------------------------------------------------------
// �ش� ���м� ã�´�.
bool CCompartLineTool::Find(CMainBlock* pMainBlock, const CPoint& point)
{
	InitializeAllData();
	if(pMainBlock->GetColumnCount() <= 1 && pMainBlock->GetRowCount() <= 1)
		return false;

	m_compartLineIndex = pMainBlock->FindCompartLine(point);
	return (m_compartLineIndex.GetRow() > -1 || m_compartLineIndex.GetColumn() > -1);
}


// private ====================================================================
// ���м� ������ ��������� ����.
bool CCompartLineTool::IsPermissionChangingCompartLine(const CRect& blockRegion, const CPoint& point) const
{
	if(!blockRegion.PtInRect(point))
		return false;

	CRect changingRegion = blockRegion;
	if(m_compartLineIndex.GetRow() != -1)
		changingRegion.bottom = point.y;
	if(m_compartLineIndex.GetColumn() != -1)
		changingRegion.right = point.x;

	return (changingRegion.Height() > MARGIN && changingRegion.Width() > MARGIN);
}

// ���� ������ ����
CPoint CCompartLineTool::GetTwoPointGap(const CPoint& pt1, const CPoint& pt2) const
{
	return CPoint(pt2.x - pt1.x, pt2.y - pt1.y);
}

CBaseToolEnumData::CURSORTYPE CCompartLineTool::GetBlockCursorType(const CBlockIndex& compartLineIndex) const
{
	if(compartLineIndex.GetRow() != -1 && compartLineIndex.GetColumn() != -1)
		return CBaseToolEnumData::SPLITE_ALL;
	else if(compartLineIndex.GetRow() != -1 && compartLineIndex.GetColumn() == -1)
		return CBaseToolEnumData::SPLITE_ROW;
	else if(compartLineIndex.GetRow() == -1 && compartLineIndex.GetColumn() != -1)
		return CBaseToolEnumData::SPLITE_COLUMN;
	return CBaseToolEnumData::CTNONE;
}

// --------------------------------------------------------------------------------------
void CCompartLineTool::DrawCompartLine_Move(CDC* pDC, const CRect& region, const CPoint& point)
{
	if(!m_bPreMMovePointUsed)
		m_bPreMMovePointUsed = true;
	else
		DrawCompartLine(pDC, region, m_preMMovePt);
	
	DrawCompartLine(pDC, region, point);
	m_preMMovePt = point;
}

void CCompartLineTool::DrawCompartLine(CDC* pDC, const CRect& region, const CPoint& point)
{
	// ���� ���м�
	if(m_compartLineIndex.GetRow() > -1)
	{
		DrawLine(pDC, CPoint(region.left, point.y), CPoint(region.right, point.y));
	}
	// ���� ���м�
	if(m_compartLineIndex.GetColumn() > -1)
	{
		DrawLine(pDC, CPoint(point.x, region.top), CPoint(point.x, region.bottom));
	}
}

void CCompartLineTool::DrawLine(CDC* pDC, const CPoint& pt1, const CPoint& pt2)
{
	int oldROP2 = pDC->GetROP2();
	pDC->SetROP2(R2_NOT);

	pDC->MoveTo(pt1);
	pDC->LineTo(pt2);

	pDC->SetROP2(oldROP2);
}
