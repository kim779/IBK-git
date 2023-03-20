//DivDrop.cpp

#include "stdafx.h"
#include "ChartDrop.h"

CChartDrop::CChartDrop()
{
    m_pWnd = NULL;
    m_bIsDragging = FALSE;
}

DROPEFFECT CChartDrop::OnDragEnter(CWnd* pWnd,COleDataObject* pObj,
		DWORD dwKeyState, CPoint point)
{
    return OnDragOver( pWnd, pObj, dwKeyState, point );
}

DROPEFFECT CChartDrop::OnDragOver(CWnd* pWnd,COleDataObject* pObj,
		DWORD dwKeyState,CPoint point)
{
	if(m_pWnd != NULL) {
		m_pWnd->SendMessage(WM_DRAGCHARTDATA, 0, 
					(LPARAM)MAKELONG(point.x, point.y));
		
	}
	DROPEFFECT deResult = DROPEFFECT_NONE;
	if(pObj->IsDataAvailable(CF_TEXT)) {
		if(dwKeyState & MK_CONTROL )
			deResult = DROPEFFECT_COPY;
		else
			deResult = DROPEFFECT_MOVE;
	}
	return deResult;
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.26
//
//	�м�	:	Graph�� ����� ó���Ѵ�.
///////////////////////////////////////////////////////////////////////////////
BOOL CChartDrop::OnDrop( CWnd* pWnd, COleDataObject *pObj, DROPEFFECT de, CPoint point)
{
	// 1. ���� OLE Data Object�κ��� TEXT Data�� ��ϵ� ChartItem Data�� Pointer�� ���Ѵ�.
	HGLOBAL	hGlobal = pObj->GetGlobalData( CF_TEXT);
	if( !hGlobal) return FALSE;
	ChartItem *pData = ( ChartItem *)GlobalLock( hGlobal);
	if( !pData)
	{
		GlobalFree(hGlobal);
		return FALSE;
	}

	// 2. ������ ChartItem Data�� Tree Control���� �����Ͽ� ó����� �Ѵ�.
	m_pWnd->SendMessage( WM_DROPCHARTDATA, 0, ( LPARAM)pData);

	// 3. �� �̿��� ChartItem Data�� �����Ѵ�.
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
	return TRUE;
}

void CChartDrop::SetName(LPSTR Name)
{
    m_Name = Name;
}

void CChartDrop::SetParent(CWnd* pWnd)
{
    m_pWnd = pWnd;
}

