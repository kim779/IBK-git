// PanAddInImp.cpp: implementation of the CPanAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "PanAddInImp.h"

#include <math.h>											// for pow()
#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../../Include_Chart/MessageDefine.h"				// for UM_CURSOR_EXCHANGE
#include "../Include_AddIn/AddInCommand.h"					// for EAI_ALL_ON_CHANGE_CURSOR_MODE
#include "../Include_AddIn/I000000/_IAddInToolMgr.h"		// for IAddInToolMgr
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IBlock.h"				// for IBlock


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPanAddInImp::CPanAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// Manage Mouse Action ID for Serial Action.
	m_dwMouseActionID = UINT_MAX;

	// Mouse�� ó�� �������� ���� ��ġ
	m_ptPanStart = CPoint(-1, -1);
	m_dPanStart = 0;

	// ���� ADDIn�� ����Ǿ�� �Ǵ��� Ȯ���ϴ� Flag 
	m_bRun = FALSE;
	m_bLButtonDown = FALSE;

	m_nStartIndex = 0;
	m_nEndIndex = 0;
	
	m_dBarWidth = 0;
	m_dViewMin = 0;
	m_dViewMax = 0;

	m_bIsLock = FALSE;
	
	m_rctGraphDrawingRegion = CRect(0,0,0,0);

	// (2008/12/11 - Seung-Won, Bae) for Y Zoom.
	m_nRow = -1;
	m_nColumn = -1;
	m_ptYZoomStart = CPoint( -1, -1);
	m_nVScaleIndex = -1;
	m_nScaleHeight = 0;
}


CPanAddInImp::~CPanAddInImp()
{

}

// Multiple Item in DLL
CString CPanAddInImp::m_strAddInItemName = "PAN";


//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CPanAddInImp)
	ONADDINEVENT( OnToolCommand )
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonUp)
	ONADDINEVENT( OnRButtonUp)
END_ADDIN_EVENT_MAP()


//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 24
// Return void	: 
// Param  const int p_nToolType : 
// Param  const int p_nCmdOption : 
// Comments		: 
//-----------------------------------------------------------------------------
void CPanAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption )
{
	CToolOptionInfo::TOOLOPTION eToolOption = (CToolOptionInfo::TOOLOPTION)p_nToolType;

	// 1. ��� Tool�� �����϶�� Message�� ������ List�� �ʱ�ȭ�Ѵ�.
	if( eToolOption == CToolOptionInfo::T_DRAG_X_ZOOM || eToolOption == CToolOptionInfo::T_DRAG_Y_ZOOM )
		m_bRun = FALSE;
	else if( eToolOption == CToolOptionInfo::T_PAN )
		m_bRun = TRUE;
	// 2008.10.15 by LYH >> �д� 
	else if( m_bRun) m_bRun = FALSE;
	else return;
	// 2008.10.15 by LYH <<

	m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_PAN, m_bRun);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 22
// Return BOOL	: 
// Param  HDC p_hDC : 
// Param  UINT nFlags : 
// Param  POINT &point : 
// Param  const DWORD p_dwMouseActionID : 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CPanAddInImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID )
{
	// (2008/12/11 - Seung-Won, Bae) for Y Zoom.
	if( OnLButtonDownForYZoom( p_hDC, nFlags, point, p_dwMouseActionID))
		return TRUE;

	// 1. ���� �����ؾߵ��� Ȯ���ϰ� �����ؾ� �� ��쿡�� �����Ѵ�.
	if( !m_bRun )
		return FALSE;
	
	// 2. Check Empty Chart!
	if( !m_pIChartOCX->IsUseToolScrollBar() || m_pIChartOCX->IsOnSpecialChart() || m_pIChartManager->IsEmpty())
		return FALSE;
	
	// 3. ������Ʈ�� ������ ���´�.
	int nR, nC, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return FALSE;

	// 4. Block ������ ���´�.
	IBlock *pIBlock = NULL;
	pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock )
		return FALSE;
	
	// 5. Block���� GraphResion�� ���´�.
	CRect rctNoFrameAndMargin;
	pIBlock->GetGraphRegions(m_rcGraphRegion, rctNoFrameAndMargin);
	if( m_rcGraphRegion.IsRectEmpty() )
		return FALSE;

	if( !m_rcGraphRegion.PtInRect(point) )
		return FALSE;

	m_rctGraphDrawingRegion	= pIBlock->GetGraphDrwingRegion();
	if( m_rctGraphDrawingRegion.IsRectEmpty() )
		return FALSE;

	// 10. ������Ʈ�� Block�� Log/������ �������� Ȯ���Ѵ�.
	m_bLog = pIBlock->IsLogScale();
	m_bReverse = pIBlock->IsVScaleInvert( nV);

	CScaleBaseData::VERTMINMAXTYPE eMinMaxType;
	pIBlock->GetVScaleMinMaxSetting( nV, m_bIsLock, eMinMaxType, m_dViewMin, m_dViewMax);

	pIBlock->Release();
	pIBlock = NULL;

	// 6. Manage Mouse Action ID for Serial Action.
	m_dwMouseActionID = p_dwMouseActionID;

	// 7. ������ point�� Drag�� ���� point�� �����Ѵ�.
	m_ptPanStart = point;

	// 1. ���� ȭ�鿡 ���̴� Data�� Start/End Index�� ���Ѵ�.
	if( !m_pIChartOCX->GetViewStartEndIndex( m_nStartIndex, m_nEndIndex ))
		return FALSE;

	int nDataOnPage = m_nEndIndex - m_nStartIndex + 1;
	m_dBarWidth = double( m_rctGraphDrawingRegion.Width() ) / double( nDataOnPage );

	m_dPanStart = m_pIChartOCX->ConvertYPositionToData( m_ptPanStart.y, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);

#ifdef _DEBUG
	TRACE( "PAN : BarWidth %f, StartIndex : %d, EndIndex : %d, YPointPrice : %f\n",
			m_dBarWidth, m_nStartIndex, m_nEndIndex, m_dPanStart );
#endif

	m_bLButtonDown = TRUE;
	
	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 22
// Return BOOL	: 
// Param  HDC p_hDC : 
// Param  UINT nFlags : 
// Param  POINT &point : 
// Param  const DWORD p_dwMouseActionID : 
// Param  const HBITMAP p_hbmpMainOSB : 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CPanAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{
	// (2008/12/11 - Seung-Won, Bae) for Y Zoom.
	if( OnMouseMoveForYZoom( p_hDC, nFlags, point, p_dwMouseActionID, p_hbmpMainOSB))
		return TRUE;

	// 1. ���� �����ؾߵ��� Ȯ���ϰ� �����ؾ� �� ��쿡�� �����Ѵ�.
	if( !m_bRun && !m_bLButtonDown )
		return FALSE;

	// 2. DC�� ���´�.
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return FALSE;

	// 3. Manage Mouse Action ID for Serial Action.
	if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))
	{
		if( p_dwMouseActionID != m_dwMouseActionID)
			return FALSE;
		
		if( !m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase))
			return FALSE;
	}


	// 5. ������ point�� Drag�� �����ߴ� point�� ������ return�Ѵ�.
	if( m_ptPanStart == point )
		return TRUE;

	// 7. ������Ʈ�� ���Ե� Block�� Index�� ���Ѵ�.
	int nR, nC, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return TRUE;

	// 8. Use Block Interface
	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock) return TRUE;

	// 10. ������Ʈ�� Block�� Log/������ �������� Ȯ���Ѵ�.
//	BOOL bLog, bReverse;
//	pIBlock->GetLogAndReverseScaleFlag( bLog, bReverse);

//	// 5. Block���� GraphResion�� ���´�.
	CRect rctNoFrameAndMargin;
	pIBlock->GetGraphRegions(m_rcGraphRegion, rctNoFrameAndMargin);
	if( m_rcGraphRegion.IsRectEmpty() )
		return FALSE;
	
	m_rctGraphDrawingRegion	= pIBlock->GetGraphDrwingRegion();
	if( m_rctGraphDrawingRegion.IsRectEmpty() )
		return FALSE;

	// 4. GraphRegion�� �Ѿ�� ������ GraphRegion���� �����Ѵ�.
	if( m_rcGraphRegion.top >= point.y )
		point.y = m_rcGraphRegion.top;

	if( m_rcGraphRegion.bottom <= point.y )
		point.y = m_rcGraphRegion.bottom;

	// point���� ���̰����� �̵��� �����Ѵ�.
	int nXGap = m_ptPanStart.x - point.x;
	BOOL bInvalidate = FALSE;
	int nXMove = 0;
	if( abs(nXGap) > m_dBarWidth )
	{
		nXMove = (int)(nXGap / m_dBarWidth);

		int nLast;
		m_pIChartOCX->GetMaxScrollRange(nLast);

		if( (m_nStartIndex + nXMove >= 0) && (m_nEndIndex + nXMove <= nLast ) )
			m_pIChartOCX->SetViewStartEndIndex( m_nStartIndex + nXMove, m_nEndIndex + nXMove );
		bInvalidate = TRUE;
	}

	// Block�� �������� �����Ѵ�.
	if( !m_bIsLock )
	{
		double dDragCur = 0;
		dDragCur = m_pIChartOCX->ConvertYPositionToData( point.y, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);

		if( ( m_dViewMin + ( m_dPanStart - dDragCur ) > 0 ) && ( m_dPanStart - dDragCur ) != 0 )
		{
			m_dViewMin = m_dViewMin + ( m_dPanStart - dDragCur );
			m_dViewMax = m_dViewMax + ( m_dPanStart - dDragCur );
			pIBlock->SetVScaleMinMaxSetting( nV, m_bIsLock,
											 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_USER,
											 m_dViewMin, m_dViewMax);

			bInvalidate = TRUE;
		}
	}
		
	if( pIBlock )
		pIBlock->Release();

	pIBlock = NULL;

	// 6. Drag�� ����Ǿ������� Mouse Cursor�� �����Ѵ�.
	m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::OBJECT_MOVE, NULL);
	
#ifdef _DEBUG
	TRACE( "PAN-MOVE: nXMove:%d, StartPoint.x:%d, StartPoint.y:%d, point.x:%d, point.y:%d\n",
			nXMove, m_ptPanStart.x, m_ptPanStart.y, point.x, point.y );
#endif

	if( bInvalidate )
		m_pIChartOCX->InvalidateControl();

	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 23
// Return BOOL	: 
// Param  HDC p_hDC : 
// Param  UINT nFlags : 
// Param  POINT &point : 
// Param  const DWORD p_dwMouseActionID : 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CPanAddInImp::OnLButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// (2008/12/11 - Seung-Won, Bae) for Y Zoom.
	if( OnLButtonUpForYZoom( p_hDC, nFlags, point, p_dwMouseActionID))
		return TRUE;

	// 1. ���� �����ؾߵ��� Ȯ���ϰ� �����ؾ� �� ��쿡�� �����Ѵ�.
	if( !m_bRun )
		return FALSE;

	// 2. DC�� ���´�.
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return FALSE;

	// 3. Manage Mouse Action ID for Serial Action.
	if( !m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase))
		return FALSE;

	// 5. Make Real Effect Gap 5 Pixel.
	if( abs(m_ptPanStart.x - point.x) < m_dBarWidth )
		return TRUE;

	// 6. Drag�� ����Ǿ������� Mouse Cursor�� �����Ѵ�.
	m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::OBJECT_NONE, NULL);

	m_ptPanStart = CPoint(-1, -1);
	
	m_bLButtonDown = FALSE;

	return TRUE;
}

// (2008/12/11 - Seung-Won, Bae) for Y Zoom.
BOOL CPanAddInImp::OnLButtonDownForYZoom( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( 0 < m_ptYZoomStart.x && 0 < m_ptYZoomStart.y) return FALSE;

	if( !m_pIChartManager->GetBlockIndex( point, m_nRow, m_nColumn)) return FALSE;
	IBlock *pIBlock = m_pIChartManager->GetBlock( m_nRow, m_nColumn);
	if( !pIBlock) return FALSE;

	CRect rctLeftScale, rctRightScale;
	pIBlock->GetVertScaleRegion( rctLeftScale, rctRightScale);

	m_nVScaleIndex = pIBlock->GetActiveVScaleIndex( rctLeftScale.PtInRect( point) ? CScaleBaseData::VERT_LEFT : CScaleBaseData::VERT_RIGHT);
	if( 0 <= m_nVScaleIndex)
	{
		CScaleBaseData::VERTMINMAXTYPE eMinMaxType;
		pIBlock->GetVScaleMinMaxSetting( m_nVScaleIndex, m_bIsLock, eMinMaxType, m_dViewMin, m_dViewMax);
	}
	pIBlock->Release();

	if( m_bIsLock || m_nVScaleIndex < 0 || ( !rctLeftScale.PtInRect( point) && !rctRightScale.PtInRect( point))) return FALSE;
	m_nScaleHeight = rctLeftScale.Height();
	m_ptYZoomStart = point;

	m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase);
	m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::BLOCKPOS_SPLITH, NULL);

	return TRUE;
}
BOOL CPanAddInImp::OnMouseMoveForYZoom( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{
	if( m_ptYZoomStart.x < 0 && m_ptYZoomStart.y < 0) return FALSE;
	if( m_nRow < 0 || m_nColumn < 0) return TRUE;

	IBlock *pIBlock = m_pIChartManager->GetBlock( m_nRow, m_nColumn);
	if( !pIBlock) return TRUE;

	double dViewMin = m_dViewMin;
	double dViewMax = m_dViewMax;
	if( m_dViewMin != m_dViewMax && point.y != m_ptYZoomStart.y)
	{
		double dViewRange = ( m_dViewMax - m_dViewMin) / 2;
		double dFactor = pow( 10, ( double( abs(point.y - m_ptYZoomStart.y)) / m_nScaleHeight) / ( 200. / 100.));
		if( m_ptYZoomStart.y < point.y) dViewRange /= dFactor;
		else							dViewRange *= dFactor;

		dViewMin = ( m_dViewMax + m_dViewMin) / 2  - dViewRange;
		dViewMax = ( m_dViewMax + m_dViewMin) / 2  + dViewRange;
	}

	pIBlock->SetVScaleMinMaxSetting( m_nVScaleIndex, m_bIsLock,
									 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_USER,
									 dViewMin, dViewMax);
	pIBlock->Release();

	m_pIChartOCX->InvalidateControl();	
	return TRUE;
}
BOOL CPanAddInImp::OnLButtonUpForYZoom( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( m_ptYZoomStart.x < 0 && m_ptYZoomStart.y < 0) return FALSE;

	m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase);
	m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::OBJECT_NONE, NULL);

	m_ptYZoomStart.x = -1;
	m_ptYZoomStart.y = -1;
	return TRUE;
}
// 2008.10.20 by LYH >> ������ ���콺 ������ �д� ��� Ǯ����
BOOL CPanAddInImp::OnRButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// 1. ���� �����ؾߵ��� Ȯ���ϰ� �����ؾ� �� ��쿡�� �����Ѵ�.
	if( !m_bRun) return FALSE;
	m_pIChartOCX->RunToolCommand( CToolOptionInfo::T_TOOL_SELECTION, 1);
	return FALSE;
}
// 2008.10.20 by LYH <<
