// GridDropTarget.cpp : implementation file
//
// MFC Grid Control - Drag/Drop target implementation
//
// CGridDropTarget is an OLE drop target for CGridCtrl. All it does
// is handle the drag and drop windows messages and pass them
// directly onto the grid control.
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2001. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.10+
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCtrl.h"

#ifndef GRIDCONTROL_NO_DRAGDROP
#include "GridDropTarget.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridDropTarget

CGridDropTarget::CGridDropTarget()
{
    m_pGridCtrl = NULL;
    m_bRegistered = FALSE;
}

CGridDropTarget::~CGridDropTarget()
{
}

// Overloaded Register() function performs the normal COleDropTarget::Register
// but also serves to connect this COleDropTarget with the parent grid control,
// where all drop messages will ultimately be forwarded.
BOOL CGridDropTarget::Register(CGridCtrl *pGridCtrl)
{
    if (m_bRegistered)
        return FALSE;

    // Stop re-entry problems
    static BOOL bInProcedure = FALSE;
    if (bInProcedure)
        return FALSE;
    bInProcedure = TRUE;

    ASSERT(pGridCtrl->IsKindOf(RUNTIME_CLASS(CGridCtrl)));
    ASSERT(pGridCtrl);

    if (!pGridCtrl || !pGridCtrl->IsKindOf(RUNTIME_CLASS(CGridCtrl)))
    {
        bInProcedure = FALSE;
        return FALSE;
    }

    m_pGridCtrl = pGridCtrl;

    m_bRegistered = COleDropTarget::Register(pGridCtrl);

    bInProcedure = FALSE;
    return m_bRegistered;
}

void CGridDropTarget::Revoke()
{
    m_bRegistered = FALSE;
    COleDropTarget::Revoke();
}

BEGIN_MESSAGE_MAP(CGridDropTarget, COleDropTarget)
    //{{AFX_MSG_MAP(CGridDropTarget)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridDropTarget message handlers

DROPEFFECT CGridDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint /*point*/)
{
    //TRACE("In CGridDropTarget::OnDragScroll\n");
    //if (pWnd->GetSafeHwnd() == m_pGridCtrl->GetSafeHwnd())
    {
		// �� �� �� : ���� (2002��2��14��   16��55��)
		// �������� : ī�� ����
		/*
		if (dwKeyState & MK_CONTROL)
            return DROPEFFECT_COPY;
        else
		*/
		// �����Ϸ� :(2002��2��14�� 16��55��)        
            return DROPEFFECT_MOVE;
    } 
	//else
    //    return DROPEFFECT_NONE;
}

DROPEFFECT CGridDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
    TRACE(_T("In CGridDropTarget::OnDragEnter\n"));
    ASSERT(m_pGridCtrl);

	//if (pWnd->GetSafeHwnd() == m_pGridCtrl->GetSafeHwnd())
        return m_pGridCtrl->OnDragEnter(pDataObject, dwKeyState, point);
    //else
    //    return DROPEFFECT_NONE;
}

void CGridDropTarget::OnDragLeave(CWnd* pWnd)
{
    TRACE(_T("In CGridDropTarget::OnDragLeave\n"));
    ASSERT(m_pGridCtrl);

    //if (pWnd->GetSafeHwnd() == m_pGridCtrl->GetSafeHwnd())
        m_pGridCtrl->OnDragLeave();
}

DROPEFFECT CGridDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
 //   TRACE("In CGridDropTarget::OnDragOver\n");
    ASSERT(m_pGridCtrl);
	
	// �� �� �� : ���� (2002��2��6��   20��09��)
	// �������� : Ư�� ��ġ�� Drag & Drop�� ����
	CCellID cell, cellLast;
	cell = m_pGridCtrl->GetCellFromPt(point);
	cellLast = m_pGridCtrl->GetLeftClickDownCell();
//	TRACE("[Before] cell.row = %d, cell.col = %d    cellLast.row = %d, cellLast.col = %d\n", 
//		cell.row, cell.col, cellLast.row, cellLast.col);


	// JSJ_ITCastle_Modify
	// �巡�� �������� �ż��ֹ� �̰ų�, �ŵ��ֹ� ���� �ƴϸ� ����
	if( !(cellLast.col == MAEDO_COL || cellLast.col == MAESU_COL) )
	{
		return DROPEFFECT_NONE;
	}

/*
	// ��� ���� �߰��� ���� �巡�׾� ��� ��� ����
	if( cell.row == ROW_END )
	{
		if (cell.col != COL_FIRST && cell.col != COL_END)
			return DROPEFFECT_NONE;
	}
	else if( cell.col != cellLast.col )
	{
		// ��� ����ƽ
		if( cell.col == COL_FIRST || cell.col == COL_END )//|| (cell.row > ROW_FIRST && cell.col > MAEDO_COL && cell.col < MAESU_COL) )
		{
			if (cell.row == ROW_FIRST)		// JSJ_Modify
				return DROPEFFECT_NONE;
//			if( cell.col == COL_FIRST )
//				if( !m_pGridCtrl->InsideStaticLeft(cell.row, cell.col) )		// ���� ��� ����ƽ (ROW_FIRST+1,COL_FIRST) -> (ROW_END, COL_FIRST)
//					return DROPEFFECT_NONE;
//				
//			if( cell.col == COL_END )
//				if( !m_pGridCtrl->InsideStaticRight(cell.row, cell.col) )		// ���� ��� ����ƽ (ROW_FIRST+1,COL_END) -> (ROW_END, COL_END)
// 					return DROPEFFECT_NONE;
		}
		else
			return DROPEFFECT_NONE;
	}*/


	if( m_pGridCtrl->GetItemText(cell.row, cell.col) == "")
		return DROPEFFECT_NONE;
	
		
	//TRACE("[After] cell.row = %d, cell.col = %d    cellLast.row = %d, cellLast.col = %d\n", 
	//	cell.row, cell.col, cellLast.row, cellLast.col);
	
    //if (pWnd->GetSafeHwnd() == m_pGridCtrl->GetSafeHwnd())
	return m_pGridCtrl->OnDragOver(pDataObject, dwKeyState, point);
    //else
    //    return DROPEFFECT_NONE;
}

BOOL CGridDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,  DROPEFFECT dropEffect, CPoint point)
{
//    TRACE(_T("In CGridDropTarget::OnDrop\n"));
    ASSERT(m_pGridCtrl);

	// �� �� �� : ���� (2002��2��6��   20��09��)
	// �������� : Ư�� ��ġ�� Drag & Drop�� ����
	CCellID cell, cellLast;
	cell = m_pGridCtrl->GetCellFromPt(point);
	cellLast = m_pGridCtrl->GetLeftClickDownCell();
//	TRACE("row = %d, col = %d\n", cell.row, cell.col);


	// JSJ_ITCastle_Modify
	// �巡�� �������� �ż��ֹ� �̰ų�, �ŵ��ֹ� ���� �ƴϸ� ����
	if( !(cellLast.col == MAEDO_COL || cellLast.col == MAESU_COL) )
	{
		return DROPEFFECT_NONE;
	}
	
/*
	// ��� ���� �߰��� ���� �巡�׾� ��� ��� ����
	if( cell.row == ROW_END )
	{
		if (cell.col != COL_FIRST && cell.col != COL_END)
			return DROPEFFECT_NONE;
	}
	else if( cell.col != cellLast.col )				
	{													  
		// ��� ����ƽ
		if( cell.col == COL_FIRST || cell.col == COL_END )//|| (cell.row > ROW_FIRST && cell.col > MAEDO_COL && cell.col < MAESU_COL) )
		{
			if (cell.row == ROW_FIRST)		// JSJ_Modify
				return DROPEFFECT_NONE;
//			if( cell.col == COL_FIRST )
//				if( !m_pGridCtrl->InsideStaticLeft(cell.row, cell.col) )		// ���� ��� ����ƽ (ROW_FIRST+1,COL_FIRST) -> (ROW_END, COL_FIRST)
//					return DROPEFFECT_NONE;
//				
//			if( cell.col == COL_END )
//				if( !m_pGridCtrl->InsideStaticRight(cell.row, cell.col) )		// ���� ��� ����ƽ (ROW_FIRST+1,COL_END) -> (ROW_END, COL_END)
// 					return DROPEFFECT_NONE;
		}
		else 
			return DROPEFFECT_NONE;
	}	

	if( cell.col == MAEDO_COL || cell.col == MAESU_COL )
	{
		CString strText = m_pGridCtrl->GetItemText(cell.row, COL_CENTER);
		strText.TrimLeft();
		strText.TrimRight();
		if( strText == "" )
			return DROPEFFECT_NONE;
	}*/


//	if( m_pGridCtrl->GetItemText(cell.row, cell.col) == "")		// JSJ_Block
//		return DROPEFFECT_NONE;

	// �����Ϸ� :(2002��2��6�� 20��09��)

    //if (pWnd->GetSafeHwnd() == m_pGridCtrl->GetSafeHwnd())
    return m_pGridCtrl->OnDrop(pDataObject, dropEffect, point);
    //else
    //    return FALSE;
}

#endif // GRIDCONTROL_NO_DRAGDROP