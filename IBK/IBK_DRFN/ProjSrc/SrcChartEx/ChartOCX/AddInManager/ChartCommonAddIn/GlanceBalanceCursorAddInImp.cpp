// GlanceBalanceCursorAddInImp.cpp: implementation of the CGlanceBalanceCursorAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "GlanceBalanceCursorAddInImp.h"

#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../../Include_Chart/MessageDefine.h"				// for UDM_ADDIN_MSG
#include "../../Include_Chart/IPropertyMap.h"				// for IPropertyMap
// �������� ���� �޴� ���� - ojtaso (20071025)
#include "../Include_AddIn/_resource.h"						// for ID_GLANCEBALANCE_CURSOR
#include "../Include_AddIn/AddInCommand.h"					// for EAI_ST_CALCULATE_INDICATOR
// �������� ���� �޴� ���� - ojtaso (20071025)
#include "../Include_AddIn/I000000/_ICmdUI.h"				// for ICmdUI
#include "../Include_Addin/I000000/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn/I000000/_IPacket.h"				// for IPacket
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IBlock.h"				// for IBlock
#include "../Include_AddIn/I000000/_IGraphInfo.h"			// for IGraphInfo
#include "../Include_AddIn/I000000/_IGraphInfoManager.h"	// for IGraphInfoManager
#include "../Include_AddIn/I000000/_IString.h"				// for IDoubleList

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 06
// Return void	: 
// Param  IChartOCX *p_pIChartOCX : 
// Param  IAddInDllBase *p_pIAddInDllBase : 
// Comments		: 
//-----------------------------------------------------------------------------
CGlanceBalanceCursorAddInImp::CGlanceBalanceCursorAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase), m_ptCursor( 0, 0)
{
	// 1. ������ Flag�� ���� �ʱ�ȭ �Ѵ�.
	// ó������ Span Mouse Cursor�� ����ϴ� ������ �����Ѵ�.
	m_bGlanceBalanceState = FALSE;			

	m_strIndicatorName = _MTEXT( C2_OBVIOUSLY_BALANCE_CHART);
	
	// 2. View�� ȯ���� �����ϴ� ������ �ʱ�ȭ �Ѵ�.
	m_rctBlockRegion = CRect(0, 0, 0, 0);
	m_rctGraphDrawingRegion = CRect(0, 0, 0, 0);

	m_nStartIndex = 0;
	m_nEndIndex = 0;
	m_nPrevSpanIndex = 0;
	m_nAfterSpanIndex = 0;

	// (2006/10/17 - Seung-Won, Bae) Manage flag of Enable State.
	m_bEnableGlanceBalanceCursor = TRUE;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 06
// Comments		: 
// Using way	: 
// See			: destruction
// (2006/11/13 - Seung-Won, Bae) Do not use MainOSB. It has collision with DragZoom Rectangle.
//-----------------------------------------------------------------------------
CGlanceBalanceCursorAddInImp::~CGlanceBalanceCursorAddInImp()
{
}

// Multiple Item in DLL
CString CGlanceBalanceCursorAddInImp::m_strAddInItemName = "GLANCEBALANCE_CURSOR";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CGlanceBalanceCursorAddInImp )
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnGraphAdded)
	ONADDINEVENT( OnGraphDeleted)
	ONADDINEVENT( OnLoadEnvironment)
	ONADDINEVENT( OnSaveEnvironment)
	ONADDINEVENT( DrawCursor)
	ONADDINEVENT( OnAddInMsg)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType)
	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnCmdMsg)		// �������� ���� �޴� ���� - ojtaso (20071025)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
//////////////////////////////////////////////////////////////////////

void CGlanceBalanceCursorAddInImp::OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)
{
	const char *szEnvValue = NULL;
	if( !p_pIAddInEnvMap->Lookup( m_strAddInItemName, szEnvValue)) return;
	m_bEnableGlanceBalanceCursor = ( atoi( szEnvValue) == 1);
}
void CGlanceBalanceCursorAddInImp::OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)
{
	const char *szEnvValue = NULL;
	if( p_pIAddInEnvMap->Lookup( m_strAddInItemName, szEnvValue)) return;

	CString strEnvValue;
	strEnvValue.Format( "%d", m_bEnableGlanceBalanceCursor ? 1 : 0);
	p_pIAddInEnvMap->SetAt( m_strAddInItemName, strEnvValue);
}

// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
RECT CGlanceBalanceCursorAddInImp::DrawCursor( HDC p_hDC)
{
	CRect rct;
	rct.SetRectEmpty();

	if( !m_bGlanceBalanceState || !m_bEnableGlanceBalanceCursor) return rct;
	if( 0 == m_ptCursor.x && 0 == m_ptCursor.y) return rct;

	// 1. DC�� Handle�� ���Ѵ�.
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return rct;

	// 2. ������ Mouse Position���� �ش�Ǵ� �ڷ����ڸ� ���´�.
	CRect rcChartOcx;
	m_pChartCWnd->GetClientRect( &rcChartOcx);

	// 4. Span Data�� �� ���ڰ� ������� �������ش�.
	m_strPrevSpan.TrimLeft();	m_strPrevSpan.TrimRight();
	m_strAfterSpan.TrimLeft();	m_strAfterSpan.TrimRight();

	// 5. Span Data�� Size�� ���Ѵ�.
	CSize szPrevSpan = pDC->GetTextExtent( m_strPrevSpan );
	CRect rctPrevSpan = CRect( m_ptPrevSpan.x - ( szPrevSpan.cx / 2 + 5 ) , m_ptPrevSpan.y - 20,
		m_ptPrevSpan.x + ( szPrevSpan.cx / 2 + 5 ), m_ptPrevSpan.y + szPrevSpan.cy + 3 );

	CSize szAfterSpan = pDC->GetTextExtent( m_strAfterSpan );
	CRect rctAfterSpan = CRect( m_ptAfterSpan.x - ( szAfterSpan.cx / 2 + 5 ) , m_ptAfterSpan.y - 20,
		m_ptAfterSpan.x + ( szAfterSpan.cx / 2 + 5 ), m_ptAfterSpan.y + szAfterSpan.cy + 3 );

	// 8. Memory DC�� ���Ѵ�.
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	pDC->SetBkMode(TRANSPARENT);

	// 9. Span Data�� ��¿� ���� �̹����� ����Ѵ�.
	ML_SET_DEFAULT_RES();
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BMP_GLANCEBALANCE_ARROW);
	BITMAP bmpInfo;
	bitmap.GetBitmap(&bmpInfo);
	CBitmap *pOldBitmap = (CBitmap *)MemDC.SelectObject(&bitmap);	

	// 10. ���� Span�� Index�� Graph������ Start Index���� ������ �׷��ش�.
	CRect rcTemp;
	if( m_nStartIndex <= m_nPrevSpanIndex )
	{
		rcTemp = rctPrevSpan;
		rcTemp.DeflateRect( 5, 24, 0, 0 );
		pDC->DrawText( m_strPrevSpan, rcTemp, DT_NOCLIP);

		pDC->BitBlt(rctPrevSpan.left + ((rctPrevSpan.Width()-bmpInfo.bmWidth) /2), rctPrevSpan.top + 5,
			bmpInfo.bmWidth, bmpInfo.bmHeight, &MemDC, 0, 0, SRCAND );
	}

	// 11. ���� Span�� Index�� Graph������ End Index���� ������ �׷��ش�.
	//{{ 2007.03.26 by LYH XScale ����
	//if( m_nAfterSpanIndex <= m_nEndIndex )
	if( m_nAfterSpanIndex <= (m_nEndIndex + m_nAfterSpan) )
	//}}
	{
		rcTemp = rctAfterSpan;
		rcTemp.DeflateRect( 5, 24, 0, 0 );
		pDC->DrawText( m_strAfterSpan, rcTemp, DT_NOCLIP);
	
		pDC->BitBlt(rctAfterSpan.left + ((rctPrevSpan.Width()-bmpInfo.bmWidth) /2), rctAfterSpan.top + 5,
			bmpInfo.bmWidth, bmpInfo.bmHeight, &MemDC, 0, 0, SRCAND );

	}

	// 12. ����� �̹����� �ݳ��Ѵ�.
	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
	
	rct.UnionRect( rctPrevSpan, rctAfterSpan);
	return rct;
}

//////////////////////////////////////////////////////////////////////
// Receive Command Message
//////////////////////////////////////////////////////////////////////

BOOL CGlanceBalanceCursorAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID)
{
	// �ϸ����ǥ ���� Ŀ�� ���� - ojtaso (20071109)
	switch(p_eCommandID)
	{
	// (2006/10/17 - Seung-Won, Bae) Manage flag of Enable State.
	case EAI_GLANCEBALANCE_CURSOR_TOGGLE_STATE:
		m_bEnableGlanceBalanceCursor = !m_bEnableGlanceBalanceCursor;
		m_pIChartOCX->InvalidateControl();
		return TRUE;
	case EAI_GLANCEBALANCE_CURSOR_SET_STATE:
		GetSpanData();
		return TRUE;
	}

	return TRUE;
}

BOOL CGlanceBalanceCursorAddInImp::OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID)
{
	if( p_eCommandID != EAI_GLANCEBALANCE_CURSOR_GET_STATE) return FALSE;
	p_lResult = m_bEnableGlanceBalanceCursor;
	return TRUE;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 19
// Return BOOL	: 
// Param  void  : 
// Comments		: �ϸ����ǥ�� IndicatorInfo���� Span�� ���� ���´�.
//-----------------------------------------------------------------------------
BOOL CGlanceBalanceCursorAddInImp::GetSpanData( void )
{
	// 1. "���� Span"�� "���� Span"�� ������ ���� ���´�.
	if( m_pIGraphInfoManager)
	{
		// 1.1 GraphData�� �����´�.

		IGraphInfo *pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, m_strIndicatorName);
		
		if( !pIGraphInfo)
			return FALSE;

		// 1.2 Graph�� ���ǰ��� �о�´�.
		IDoubleList* pCalcConditionList = pIGraphInfo->GetCommonCalculateConditionList();
		if( pCalcConditionList)
		{
			POSITION pos = pCalcConditionList->GetHeadPosition();
			double dCalcCond = 0;
			
			// 1.2.1 ���� Span���� �����´�.
			if( pos )
			{
				pCalcConditionList->GetNext( pos, dCalcCond );
				// �ϸ����ǥ ���� Ŀ�� 2�� ���� - ojtaso (20071113)
				m_nPrevSpan = (int)dCalcCond - 1;
			}

			// 1.2.2 ���� Span���� �����´�.
			m_nAfterSpan = m_nPrevSpan;


			// 1.2.3 I�� ���۵Ǵ� Indterface�� �ݵ�� Release()�� ���ش�.
			pCalcConditionList->Release();
		}

		// 1.3 I�� ���۵Ǵ� Indterface�� �ݵ�� Release()�� ���ش�.
		pIGraphInfo->Release();
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// Drawing Span Data
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 11
// Return void	: 
// Param  HDC p_hDC : 
// Comments		: Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
//				: "�ϸ����ǥ"�� ������ Mouse Cursor�� ������ "����Span"��
//				: "����Span"�� ǥ�����ش�.
//-----------------------------------------------------------------------------
void CGlanceBalanceCursorAddInImp::OnDrawAfterGraph( HDC p_hDC )
{
	// 1. "Span Cursor"�� ������� ������ return�Ѵ�.
	if( !m_bGlanceBalanceState || !m_bEnableGlanceBalanceCursor) return;

	// 2. "Ư����Ʈ"�� ������ ������ �ʰ� �Ѵ�.
	if( m_pIChartOCX->IsOnSpecialChart()) return;

	// 4. Mouse�� position���� Block�� ������ ���Ѵ�.
	GetGraphBlockRegion();
}

//////////////////////////////////////////////////////////////////////
// Receive Mouse Event
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 04
// Return BOOL	: 
// Param  HDC p_hDC : 
// Param  UINT nFlags : 
// Param  POINT &point : 
// Param  const DWORD p_dwMouseActionID : 
// Comments		: Mouse cursor�� "�ϸ����ǥ"�� ���� ���� ��쿡�� Mouse cursor�� ���� 
//				: "����Span", "����Span"�� ǥ�����ش�.
//-----------------------------------------------------------------------------
BOOL CGlanceBalanceCursorAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
{
	// 1. "Span Cursor"�� �����ϸ� ó������ �ʰ� return�Ѵ�.
	if( !m_bGlanceBalanceState || !m_bEnableGlanceBalanceCursor) return FALSE;
	
	m_ptCursor.x = 0;
	m_ptCursor.y = 0;

	// 2. "�ϸ����ǥ" ������ Mouse�� ��ġ�� ��츦 ó���Ѵ�.
	if( m_rctBlockRegion.PtInRect( point))
	{
		GetSpanTime( point);
		m_ptCursor = point;

	}

	// return FALSE to pass the Event to a next AddIn Routine
	return FALSE;
}


//////////////////////////////////////////////////////////////////////
// Set Environment data & calculate condition
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 19
// Return BOOL	: 
// Param  CPoint ptPoint : 
// Comments		: �־��� Point�� �ش��ϴ� Block�� Graph Region�� ���Ѵ�.
//				: Block ������ Height��, Graph�� Width ������ Input �������� Ȯ���Ѵ�.
//-----------------------------------------------------------------------------
BOOL CGlanceBalanceCursorAddInImp::GetGraphBlockRegion( void )
{
	// 1. "�ϸ����ǥ"�� ���Ե� Block�� Index�� ���Ѵ�.
	int nR, nC, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( m_strIndicatorName, nR, nC, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return FALSE;

	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock )
		return FALSE;
		
	// 3. Block�� Region�� ���Ѵ�.
	CRect rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( m_rctBlockRegion, rctNoFrameAndMargin);

	// 4. Block������ Graph Resion�� ���Ѵ�.
	m_rctGraphDrawingRegion = pIBlock->GetGraphDrwingRegion();
	pIBlock->Release();
	pIBlock = NULL;

	// 5. Block Region�� �����Ѵ�.
	m_rctBlockRegion.left = m_rctGraphDrawingRegion.left;
	m_rctBlockRegion.right = m_rctGraphDrawingRegion.right;
	
	return TRUE;
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 15
// Return BOOL	: 
// Param  CPoint p_ptPoint : 
// Comments		: Mouse�� ��ġ�� Span�� ���� ���´�.
//-----------------------------------------------------------------------------
BOOL CGlanceBalanceCursorAddInImp::GetSpanTime( CPoint ptPoint)
{
//	// 1. ���� ȭ�鿡 ���̴� Data�� Start/End Index�� ���Ѵ�.
//	if( !m_pIChartOCX->GetViewStartEndIndex( m_nStartIndex, m_nEndIndex ))
//		return FALSE;
//
//	// 2. Mouse Point�� Data Index�� ���Ѵ�.
//	int nDataOnPage = m_nEndIndex - m_nStartIndex + 1;
//	double dBarWidth = double( m_rctGraphDrawingRegion.Width() ) / double( nDataOnPage );
//
//
//	int nDataIndex = m_nStartIndex + int( double( ptPoint.x - m_rctBlockRegion.left ) / dBarWidth );
//
//	// 3. ���� Mouse Point�� �̿��Ͽ� �� Span�� Index�� ���Ѵ�.
//	m_nPrevSpanIndex = nDataIndex - m_nPrevSpan;
//	m_nAfterSpanIndex = nDataIndex + m_nAfterSpan;
//
//	// 4. �� Span�� ���� Point�� ����Ѵ�.
//	int nBarLeftEdge, nBarRightEdge, nX;
//	int nDataOffset = m_nPrevSpanIndex - m_nStartIndex;
//	nBarLeftEdge = m_rctGraphDrawingRegion.left + int( dBarWidth * double( nDataOffset) + 0.5) + 1;
//	nBarRightEdge = m_rctGraphDrawingRegion.left + int( dBarWidth * double( nDataOffset + 1) + 0.5) - 1;
//	nX = ( nBarLeftEdge + nBarRightEdge) / 2;
//
//	m_ptPrevSpan.x = nX;
//	m_ptPrevSpan.y = ptPoint.y;
//
//	nDataOffset = m_nAfterSpanIndex - m_nStartIndex;
//	nBarLeftEdge = m_rctGraphDrawingRegion.left + int( dBarWidth * double( nDataOffset) + 0.5) + 1;
//	nBarRightEdge = m_rctGraphDrawingRegion.left + int( dBarWidth * double( nDataOffset + 1) + 0.5) - 1;
//	nX = ( nBarLeftEdge + nBarRightEdge) / 2;
//
//	m_ptAfterSpan.x = nX;
//	m_ptAfterSpan.y = ptPoint.y;
//
//	// 5. Block���� XScale �����ڸ� ���´�.
//	int nR, nC;
//	if( !m_pIChartManager->GetBlockIndex( ptPoint, nR, nC))
//		return FALSE;
//	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
//	if( !pIBlock)
//		return FALSE;
//
//	ILPCSTR szDateCompart = pIBlock->GetDateCompart();
//	m_strDateCompartment = szDateCompart;
//
//	ILPCSTR szTimeCompart = pIBlock->GetTimeCompart();
//	m_strTimeCompartment = szTimeCompart;
//
//
//	// 6. Span Data�� �ش�Ǵ� ���ڿ��� ���Ѵ�.
//	GetStringfromDataIdx( m_nPrevSpanIndex, m_strPrevSpan );
//	GetStringfromDataIdx( m_nAfterSpanIndex, m_strAfterSpan );
//
//	pIBlock->Release();
//	return TRUE;

//{{ 2007.03.26 by LYH XScale ����
	// 1. ���� ȭ�鿡 ���̴� Data�� Start/End Index�� ���Ѵ�.
	if( !m_pIChartManager->GetStartEndDrawIdx_Cur("DEFAULT", m_nStartIndex, m_nEndIndex ))
		return FALSE;

	// 2. Mouse Point�� Data Index�� ���Ѵ�.
	int nDataIndex = m_pIChartManager->FindViewDataIndex_Pt("DEFAULT", ptPoint.x);
	// 3. ���� Mouse Point�� �̿��Ͽ� �� Span�� Index�� ���Ѵ�.
	if(m_pIChartManager->GetGraphXArea_Start() > ptPoint.x)	//Reverse
	{
		m_nPrevSpanIndex = nDataIndex + m_nPrevSpan;
		m_nAfterSpanIndex = nDataIndex - m_nAfterSpan;
	}
	else
	{
		m_nPrevSpanIndex = nDataIndex - m_nPrevSpan;
		m_nAfterSpanIndex = nDataIndex + m_nAfterSpan;
	}
	// 4. Block���� XScale �����ڸ� ���´�.
	int nR, nC;
	if( !m_pIChartManager->GetBlockIndex( ptPoint, nR, nC))
		return FALSE;
	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
	if( !pIBlock)
		return FALSE;

	// 5. �� Span�� ���� Point�� ����Ѵ�.
	m_ptPrevSpan.x = m_pIChartManager->GetXPosition("DEFAULT", m_nPrevSpanIndex, nC);
	m_ptPrevSpan.y = ptPoint.y;

	m_ptAfterSpan.x = m_pIChartManager->GetXPosition("DEFAULT", m_nAfterSpanIndex, nC);
	m_ptAfterSpan.y = ptPoint.y;

	ILPCSTR szDateCompart = pIBlock->GetDateCompart();
	m_strDateCompartment = szDateCompart;

	ILPCSTR szTimeCompart = pIBlock->GetTimeCompart();
	m_strTimeCompartment = szTimeCompart;


	// 6. Span Data�� �ش�Ǵ� ���ڿ��� ���Ѵ�.
	GetStringfromDataIdx( m_nPrevSpanIndex, m_strPrevSpan );
	GetStringfromDataIdx( m_nAfterSpanIndex, m_strAfterSpan );

	pIBlock->Release();
	return TRUE;
//}}
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 15
// Return BOOL	: 
// Param  int nDataIdx : 
// Param  CString &p_strSpan : 
// Comments		: �ش� Data Index�� �ð� ������ ���Ѵ�.
//-----------------------------------------------------------------------------
BOOL CGlanceBalanceCursorAddInImp::GetStringfromDataIdx( int nDataIdx, CString &strSpan )
{
	// 1. �ڷ����� Packet�� ���Ѵ�.
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
	if( !pIPacket)
	{
		strSpan = "00/00/00";
		return FALSE;
	}

	// 2. Data�� ���Ѵ�.
	double dMarkTime;
	if( !pIPacket->GetData( nDataIdx, dMarkTime ))
		pIPacket->GetDataPastandFuture( nDataIdx, dMarkTime );
	
	long lMarkTime = (long)dMarkTime;
	CString szMarkTime;
	szMarkTime.Format("%d", lMarkTime);

	// 3. Packet Type�� ���Ѵ�.
	ILPCSTR szPacketType = pIPacket->GetType();
	CString strPacketType( szPacketType);
	
	int nPacketTypeLength = strPacketType.GetLength();

	// 4. PacketType�� ���̿� Data�� ���̰� �ٸ��� '0'���� ä���ش�.
	//SK�� ������ ��ƾ�� ��ȭ���� ������.
	//>>[KSJ000000023]�ϸ� ����ǥ �״� ���� ���� sj_kim(20110404)
	int nGap = nPacketTypeLength - szMarkTime.GetLength();
	if(nGap >= 0)
	{
		for( int i = nGap ; i ; i-- )
			szMarkTime = "0" + szMarkTime;
	}
	else
		szMarkTime = szMarkTime.Left(szMarkTime.GetLength() + nGap);
	//<<[KSJ000000023]�ϸ� ����ǥ �״� ���� ���� sj_kim(20110404)

	char strChar = strPacketType.GetAt(0);
	strSpan = szMarkTime.GetAt(0);

	// 5. XScale�� ���Ǵ� ������ �� Type�� �����Ѵ�.
	CString strCompartment = m_strDateCompartment;
	BOOL bUseOneCharDateCompartment = ( m_strDateCompartment.GetLength() > 1 ) ? FALSE : TRUE;
	BOOL bUseOneCharTimeCompartment = ( m_strTimeCompartment.GetLength() > 1 ) ? FALSE : TRUE;

	
	// "��"�� "��"�� PacketType�� ���� 'M'�� ����ϱ� ������ �������ش�.
	BOOL bIsMonth = FALSE;				
	CString strPacketChar = "";
	
	// 6. PacketType�� ���̸�ŭ ���ڿ��� parsing�Ѵ�.
	ML_SET_LANGUAGE_RES();
	for( int nIdx = 1 ; nPacketTypeLength > nIdx ; nIdx++ )
	{
		strPacketChar = strPacketType.GetAt(nIdx);

		// 6.1 PacketType������ Data�� Type�� �ٲ�� �ش�Ǵ� �����ڸ� ���ڿ��� �߰����ش�.
		if( strChar != strPacketChar )
		{
			// 6.1.1 Date�� �����ڰ� "�� �� ��"�� ����� ��� 
			if( !bUseOneCharDateCompartment )
			{
				if( strChar == 'Y' && strPacketChar == 'M' )
				{
					strCompartment.LoadString( IDS_YEAR);
					bIsMonth = TRUE;
				}
				
				else if( strChar == 'M' && strPacketChar == 'D' )		strCompartment.LoadString( IDS_MONTH);
				else if( strChar == 'D' && strPacketChar == 'H' )		strCompartment.LoadString( IDS_DAY);
			}

			// 6.1.2 Time�� �����ڰ� "�� �� ��"�� ����� ��� 
			if( !bUseOneCharTimeCompartment )
			{
				if( strChar == 'H' && strPacketChar == 'M' )
				{
					strCompartment.LoadString( IDS_HOUR);
					bIsMonth = FALSE;
				}
				
				else if( strChar == 'M' && strPacketChar == 'S' )		strCompartment.LoadString( IDS_MINUTE);
				else if( strChar == 'S' && strPacketChar == 'N' )		strCompartment.LoadString( IDS_SECOND);
			}


			// 6.1.3 ���� �����ڰ� �Ϲ� �����ϰ�� ��¥���� �ð����� ����� �� ���� ���ڸ� �������ش�.
			if( strChar == 'H' && strPacketChar == 'M' && bUseOneCharTimeCompartment )
				strCompartment = m_strTimeCompartment;

			strSpan = strSpan + strCompartment + szMarkTime.GetAt(nIdx);

			strChar = strPacketType.GetAt(nIdx);
		}

		// 6.2 PacketType�� ������� ������ ���ڸ� 
		else
		{
			// 6.2.1 PacketType�� ������ ������ Span���ڸ� �����ش�.
			strSpan = strSpan + szMarkTime.GetAt(nIdx);
			
			// 6.2.2 szMarkTime�� ��� parsing�ϰ� ���� ���������� �����ڸ� �ٿ��� �� ��츦 Ȯ���ϰ� �ٿ��ش�.
			CString strComp;
			if( nPacketTypeLength == ( nIdx + 1 ) )
			{
				if( strChar == 'Y' && !bUseOneCharDateCompartment )
				{
					strComp.LoadString( IDS_YEAR);
					strSpan = strSpan + strComp;
				}
				else if( strChar == 'M' && !bUseOneCharDateCompartment && bIsMonth )
				{
					strComp.LoadString( IDS_MONTH);
					strSpan = strSpan + strComp;
				}
				else if( strChar == 'D' && !bUseOneCharDateCompartment )
				{
					strComp.LoadString( IDS_DAY);
					strSpan = strSpan + strComp;
				}				
				else if( strChar == 'H' && !bUseOneCharTimeCompartment )
				{
					strComp.LoadString( IDS_HOUR);
					strSpan = strSpan + strComp;
				}
				else if( strChar == 'M' && !bUseOneCharTimeCompartment && !bIsMonth)
				{
					strComp.LoadString( IDS_MINUTE);
					strSpan = strSpan + strComp;
				}
				else if( strChar == 'S' && !bUseOneCharTimeCompartment )
				{
					strComp.LoadString( IDS_SECOND);
					strSpan = strSpan + strComp;
				}
			}
		}
	}

	pIPacket->Release();
	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 13
// Return BOOL	: 
// Param  long &p_lResult : 
// Param  const EAICommandType p_eCommandID : �Է¸�ɾ�
// Param  const char *p_szParam : �߰��Ǵ� ��ǥ��
// Param  CObject *p_pParam : 
// Comments		: ��ǥ�߰��� ȣ���. �ϸ����ǥ�� �߰��� ��츦 Ȯ���ϰ� �ش�Ǵ�
//				: �۾��� ���ش�.
//-----------------------------------------------------------------------------
// (2006/11/17 - Seung-Won, Bae) On Graph Added
void CGlanceBalanceCursorAddInImp::OnGraphAdded( const char *p_szGraphName, const char *p_szIndicatorName)
{
	// 2. ���� �߰��Ǵ� ��ǥ�� "�ϸ����ǥ"�϶� Span���� ��Ÿ���� Mouse Cursor��
	// ����� �������� Ȯ���Ѵ�.
	if( m_strIndicatorName != p_szIndicatorName) return;

	// 2.1 Span�� ��Ÿ���� Mouse Cursor�� ����ϴ°����� ����.
	m_bGlanceBalanceState = TRUE;
	// 2.2 ��ǥ�� �߰��ϰ� ���� Span ���� ������ �´�.
	GetSpanData();
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 13
// Return BOOL	: 
// Param  const EAICommandType p_eCommandID : 
// Param  const char *p_szParam : 
// Comments		: ��ǥ���Ž� ȣ���.
//-----------------------------------------------------------------------------
// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
void CGlanceBalanceCursorAddInImp::OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName)
{
	// 2. ���� �����ϴ� ��ǥ�� "�ϸ����ǥ"�϶� Span���� ��Ÿ���� Mouse Cursor��
	// ����� �������� Ȯ���Ѵ�.
	if( p_szIndicatorName == m_strIndicatorName)
	{
		// Span�� ��Ÿ���� Mouse Cursor�� ����ϴ°����� ����.
		m_bGlanceBalanceState = FALSE;
	}
}

// �������� ���� �޴� ���� - ojtaso (20071025)
BOOL CGlanceBalanceCursorAddInImp::OnCmdMsg( UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	if( ID_GLANCEBALANCE_CURSOR != nID) return FALSE;
	if( CN_COMMAND == nCode) return OnAddInToolCommand( EAI_GLANCEBALANCE_CURSOR_TOGGLE_STATE);
	else if( CN_UPDATE_COMMAND_UI == nCode)
	{
		if( p_pICmdUI) 
		{
			p_pICmdUI->Enable( TRUE);

			long lResult = 0;
			OnAddInToolCommand( lResult, EAI_GLANCEBALANCE_CURSOR_GET_STATE);
			p_pICmdUI->SetCheck( lResult);
		}
		return TRUE;
	}
	return FALSE;
}
