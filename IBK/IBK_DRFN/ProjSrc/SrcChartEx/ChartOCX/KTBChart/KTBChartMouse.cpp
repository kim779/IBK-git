// KTBChartMouse.cpp: implementation of the Mouse Event Code.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTBChart.h"

#include "../AddInManager/Include_AddIn/_resource.h"		// for ID_AIC_PRICE_CHART
#include "../Include_Chart/DLL_Load/AddInManager.h"			// for CAddInManager
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/Block.h"							// for CBlock
#include "OutsideCommonInterface.h"							// for _CHARTDLL_CHARTTYPE_PRICE
#include "KTBChartCtl.h"									// for CKTBChartCtrl
#include "ObjTool.h"										// for CObjTool
#include "MainBlock.h"										// for CMainBlock
#include "ToolBarManager.h"									// for IToolBarManager
#include "PacketList.h"										// for CPacketList
#include "MessageDefine.h"									// for UM_CURSOR_EXCHANGE

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// (2004.10.22, ��¿�) Mouse Cursor�� ��� �� ��������� �����Ѵ�. (�� Cursor�� Handle�� �����Ѵ�.)
//////////////////////////////////////////////////////////////////////

// (2004.10.22, ��¿�) Mouse Cursor�� ��� �� ��������� �����Ѵ�. (�� Cursor�� Handle�� �����Ѵ�.)
//		���� ��ϵ� Cursor�� �ݿ��Ѵ�.
BOOL CKTBChartCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	TRACE( "\r\nCursor state in CKTBChartCtrl::OnSetCursor() : [%d]", m_hCursor);
	if( m_hCursor)
	{
		SetCursor( m_hCursor);
		return TRUE;
	}

	return COleControl::OnSetCursor(pWnd, nHitTest, message);
}

// (2004.10.22, ��¿�) Mouse Cursor�� ��� �� ��������� �����Ѵ�. (�� Cursor�� Handle�� �����Ѵ�.)
//		��û�� Cursor Type�� ����Ͽ� �����Ѵ�.
LRESULT CKTBChartCtrl::OnCursorExchange(WPARAM wParam, LPARAM lParam)
{
	// 1. ��û�� Cursor Type�� ���� �˸��� Cursor�� Loading�Ͽ� ����Ѵ�.
	if( wParam == CCursorShape::OBJECT_NONE)
	{
		m_hCursor = NULL;
		TRACE( "\r\nCursor cleared in CKTBChartCtrl::OnCursorExchange()");
	}
	else
	{
		// 1.1  ��û�� Cursor Type�� �´� Resource ID�� Ȯ���Ѵ�.
		WORD wCursorID = 0;
		switch( wParam)
		{
			case CCursorShape::BLOCKPOS_SPLITH:	wCursorID = IDC_SPLITV;
												break;
			case CCursorShape::BLOCKPOS_SPLITV:	wCursorID = IDC_SPLITH;
												break;
			case CCursorShape::BLOCKPOS_SPLITA:	wCursorID = IDC_SPLITA;
												break;
			case CCursorShape::OBJECT_SELECT:	wCursorID = IDC_HANDLE;
												break;
			case CCursorShape::OBJECT_MOVE:		wCursorID = IDC_GRAB;
												break;
			case CCursorShape::VIEW_ZOOM_IN:	wCursorID = IDC_ZOOMIN;
												break;
			case CCursorShape::WHOLE_VIEW:		wCursorID = IDC_WHOLEVIEW;
												break;
			case CCursorShape::TOOL_DRAW:		wCursorID = IDC_PEN;
												break;
			case CCursorShape::TOOL_SIZEALL:	wCursorID = IDC_MOVEALL;
												break;
			case CCursorShape::TOOL_SIZENESW:	wCursorID = IDC_MOVENESW;
												break;
			case CCursorShape::TOOL_NODROP:		wCursorID = IDC_NODROP;
												break;
			case CCursorShape::OBJECT_REMOVE:	wCursorID = IDC_REMOVE;
												break;
		}
		// 1.2 Cursor Resource�� Loading�Ѵ�.
		ML_SET_DEFAULT_RES();
		m_hCursor = ::LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE( wCursorID));
		TRACE( "\r\nCursor Changed in CKTBChartCtrl::OnCursorExchange() : [%d]", m_hCursor);
	}

	// 2. Loading�� Cursor�� �����Ѵ�.
	// (2006/10/31 - Seung-Won, Bae) Do not call OnSetCursor() directly
	SendMessage( WM_SETCURSOR, ( WPARAM)GetSafeHwnd(), 0);

	return 0L ;
}

// (2004.10.25, ��¿�) Cursor Handle�� ��� �� ��ȸ�ϴ� Interface�� �����Ѵ�.
HCURSOR CKTBChartCtrl::SetCursorHandle( HCURSOR p_hNewCursor)
{
	// 1. ���� Cursor�� BackUp�Ѵ�.
	HCURSOR hOldCursor = m_hCursor;

	// 2. ���ο� Cursor�� �����Ѵ�.
	m_hCursor = p_hNewCursor;
	TRACE( "\r\nCursor Changed in CKTBChartCtrl::SetCursorHandle() : [%d]", m_hCursor);
	// (2006/10/31 - Seung-Won, Bae) Do not call OnSetCursor() directly
	SendMessage( WM_SETCURSOR, ( WPARAM)GetSafeHwnd(), 0);

	// 3. ���� Cursor�� Return�Ѵ�.
	return hOldCursor;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.02, ��¿�) Mouse Capturing�� ��ø������ ó���ǵ���, ��ø Count�� ������ Capturing Interface�� �����Ѵ�.
// (2006/2/6 - Seung-Won, Bae) ����! OnCaptureChanged()�� SetCapture()/RelaseCapture()�� ���� ��� �ܿ���,
//		Focus �� Activate, ToolTip, TrackMenu� ���ؼ��� ���÷� (SetCapture����)
//		���� �ڱ� �ڽ��� �ٽ� SetCapture()�ϴ� ��쿡�� ȣ����� �����Ѵ�.
//		*. ����, m_nMouseCaptureCount�� 0���� ū ��Ȳ���� Release Capture�Ǵ� ����
//				0�� ��Ȳ���� SetCapture�Ǵ� ��츸 Ȯ���Ͽ� Notify �Ѵ�.
//////////////////////////////////////////////////////////////////////

CWnd *CKTBChartCtrl::AddSetCapture( void)
{
	CWnd *pWnd = this;	// Set for SetCapturing Already.

TRACE( "Before SetCapture : %d\r\n", m_nMouseCaptureCount);

	// 1. ù Capturing�� ó���Ѵ�.
	if( m_nMouseCaptureCount == 0) pWnd = SetCapture();

	// 1. Capturing Counter�� ������Ų��.
	m_nMouseCaptureCount++;

TRACE( "After SetCapture : %d\r\n", m_nMouseCaptureCount);

	// 3. �̹� Capturing�� ��츦 ó���Ѵ�.
	return pWnd;
}

BOOL CKTBChartCtrl::RemoveSetCapture( void)
{
	if( m_nMouseCaptureCount <= 0) return FALSE;

TRACE( "Before ReleaseCapture : %d\r\n", m_nMouseCaptureCount);

	// 1. ������ Capturing�� ó���Ѵ�.
	BOOL bResult = TRUE;
	if( m_nMouseCaptureCount == 1) bResult = ReleaseCapture();	// ReleaseCapture�� OnCaptureChanged���� m_nMouseCaptureCount�� ���� �ʱ�ȭ�Ѵ�.
	else m_nMouseCaptureCount--;								// 0. Capturing Counter�� ���ҽ�Ų��. 

TRACE( "After ReleaseCapture : %d\r\n", m_nMouseCaptureCount);

	// 2. ���� Capturing�� ��츦 ó���Ѵ�.
	return bResult;
}

// (2004.10.29, ��¿�) Mouse Event�� AddIn DLL���� �����ϵ��� �Ѵ�.
void CKTBChartCtrl::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here

	// (2006/2/6 - Seung-Won, Bae) Check SetCapture or ReleaseCapture, LoseCapture
	if( ( 0 == m_nMouseCaptureCount && this == pWnd) || ( 0 < m_nMouseCaptureCount && this != pWnd))
	{
		// (2004.10.29, ��¿�) Mouse Event�� AddIn DLL���� �����ϵ��� �Ѵ�.
		if( m_pAddInManager)
		{
			CClientDC dcClient( this);
			CFont *pOldFont = dcClient.SelectObject( m_pRscFont);
			// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
			m_pAddInManager->ClearCursor( &dcClient, &m_bmpMainOSB);
			dcClient.SelectObject( pOldFont);

			m_pAddInManager->OnCaptureChanged( pWnd, &m_bmpMainOSB);
		}

		// (2004.11.02, ��¿�) No Capturing Mode�� �����Ѵ�.
		if( 0 < m_nMouseCaptureCount && this != pWnd)
		{
			m_nMouseCaptureCount = 0;
TRACE( "OnCaptureChanged with ReleaseCapture\r\n");
		}
		else
		{
TRACE( "OnCaptureChanged with SetCapture\r\n");
		}
	}
	
	COleControl::OnCaptureChanged(pWnd);
}



//////////////////////////////////////////////////////////////////////
// (2004.12.13, ��¿�) Run Time�� Mouse Action�� ó���Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2004.11.04, ��¿�, �м�) Mouse�� �⺻ Moving���� ó���� �м��Ѵ�.
void CKTBChartCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// (2006/1/17 - Seung-Won, Bae) Notify Mouse Move Event to Addin
	if( m_pAddInManager)
	{
		// Caution! Use Client DC for MsgLoop like TrackPopupMenu()
		//		if GetDC(), The DC Instance can be destroyed. (You may lost the Handle of DC)
		CClientDC dcClient( this);
		CDC memoryDC;
		if( memoryDC.CreateCompatibleDC( &dcClient))
		{
			CFont* pOldFont = memoryDC.SelectObject( m_pRscFont);
	
			CBitmap bmpLocalOSB;
			bmpLocalOSB.CreateCompatibleBitmap( &dcClient, m_rcMain.Width(), m_rcMain.Height());

			CBitmap* pOldbm = memoryDC.SelectObject( &bmpLocalOSB);

			// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
			m_pAddInManager->ClearCursor( &memoryDC, &m_bmpMainOSB, &m_rcMain);
			m_pAddInManager->OnMouseMove( &memoryDC, nFlags, point, m_dwMouseActionID, &m_bmpMainOSB);
			m_pAddInManager->DrawCursor( &memoryDC);

			dcClient.BitBlt( m_rcMain.left, m_rcMain.top, m_rcMain.Width(), m_rcMain.Height(), &memoryDC, 0, 0, SRCCOPY) ;

			memoryDC.SelectObject(pOldFont) ;
			memoryDC.SelectObject(pOldbm) ;
			memoryDC.DeleteDC();

			bmpLocalOSB.DeleteObject();
		}
	}

	COleControl::OnMouseMove(nFlags, point);
}

void CKTBChartCtrl::OnLButtonDown( UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	TRACE( "::OnLButtonDown()\r\n");

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse DownUp Count for AddIn's Mouse Action ID
	if( ULONG_MAX == m_dwMouseActionID) m_dwMouseActionID = 0;
	else m_dwMouseActionID++;

	// (2006/2/6 - Seung-Won, Bae) for Mouse Action ID and Capture Count Handling
	// (2006/3/5 - Seung-Won, Bae) Do not process after AddSetCapture() for some possible other action.
	OnLButtonDownProc( nFlags, point);

	// (2004.11.02, ��¿�) Mouse Capturing�� ��ø������ ó���ǵ���, ��ø Count�� ������ Capturing Interface�� �����Ѵ�.
	AddSetCapture();

	COleControl::OnLButtonDown( nFlags, point);
}

// (2006/2/6 - Seung-Won, Bae) for Mouse Action ID and Capture Count Handling
void CKTBChartCtrl::OnLButtonDownProc( UINT nFlags, CPoint point)
{
	// (2006/1/17 - Seung-Won, Bae) Notify Mouse LButton Down Event to Addin
	if( m_pAddInManager)
	{
		// Caution! Use Client DC for MsgLoop like TrackPopupMenu()
		//		if GetDC(), The DC Instance can be destroyed. (You may lost the Handle of DC)
		CClientDC dcClient( this);
		CFont *pOldFont = dcClient.SelectObject( m_pRscFont);
		m_pAddInManager->OnLButtonDown( &dcClient, nFlags, point, m_dwMouseActionID);
		dcClient.SelectObject( pOldFont);
	}
}

void CKTBChartCtrl::OnLButtonUp( UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
TRACE( "::OnLButtonUp()\r\n");

	// (2004.11.02, ��¿�) Mouse Capturing�� ��ø������ ó���ǵ���, ��ø Count�� ������ Capturing Interface�� �����Ѵ�.
	//		���� ������ Capturing�� �����Ѵ�. (If�� �������� �ι��� �ʿ������..)
	RemoveSetCapture();

	// (2006/2/6 - Seung-Won, Bae) for Mouse Action ID and Capture Count Handling
	// (2006/3/5 - Seung-Won, Bae) Do not process before RemoveSetCapture() for some possible other action.
	OnLButtonUpProc( nFlags, point);

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse DownUp Count for AddIn's Mouse Action ID
	if( ULONG_MAX == m_dwMouseActionID) m_dwMouseActionID = 0;
	else m_dwMouseActionID++;

	COleControl::OnLButtonUp(nFlags, point);
}

// (2006/2/6 - Seung-Won, Bae) for Mouse Action ID and Capture Count Handling
void CKTBChartCtrl::OnLButtonUpProc( UINT nFlags, CPoint point)
{
	// (2006/1/17 - Seung-Won, Bae) Notify Mouse LButton Up Event to Addin
	if( m_pAddInManager)
	{
		// Caution! Use Client DC for MsgLoop like TrackPopupMenu()
		//		if GetDC(), The DC Instance can be destroyed. (You may lost the Handle of DC)
		CClientDC dcClient( this);
		CFont *pOldFont = dcClient.SelectObject( m_pRscFont);
		m_pAddInManager->OnLButtonUp( &dcClient, nFlags, point, m_dwMouseActionID);
		dcClient.SelectObject( pOldFont);
	}
}

void CKTBChartCtrl::OnLButtonDblClk( UINT nFlags, CPoint point)
{
// (2009/9/6 - Seung-Won, Bae) for TEST of Horizontal Scale Height
//#ifdef _DEBUG
//	CBlockIndex biSelect = m_pMainBlock->FindBlockIndex( point);
//	if( biSelect.IsAllValueBiggerThanZero())
//	{
//		CBlock* pBlock = m_pMainBlock->GetBlock( biSelect);
//		if( pBlock)
//		{
//			int nScaleHeight = pBlock->GetHorizontalScaleHeight();
//			nScaleHeight += 10;
//			m_pMainBlock->SetHorizontalScaleHeight( biSelect.GetRow(), biSelect.GetColumn(), nScaleHeight);
//		}
//	}
//#endif

	// TODO: Add your message handler code here and/or call default
	TRACE( "::OnLButtonDblClk()\r\n");

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse DownUp Count for AddIn's Mouse Action ID
	if( ULONG_MAX == m_dwMouseActionID) m_dwMouseActionID = 0;
	else m_dwMouseActionID++;

	// (2004.11.02, ��¿�) Mouse Capturing�� ��ø������ ó���ǵ���, ��ø Count�� ������ Capturing Interface�� �����Ѵ�.
	AddSetCapture();

	// (2006/2/6 - Seung-Won, Bae) for Mouse Action ID and Capture Count Handling
	OnLButtonDblClkProc( nFlags, point);

	COleControl::OnLButtonDblClk(nFlags, point);
}

// (2006/2/6 - Seung-Won, Bae) for Mouse Action ID and Capture Count Handling
void CKTBChartCtrl::OnLButtonDblClkProc( UINT nFlags, CPoint point)
{
	// (2006/1/21 - Seung-Won, Bae) Notify Mouse Move Event to Addin
	if( m_pAddInManager)
	{
		// Caution! Use Client DC for MsgLoop like TrackPopupMenu()
		//		if GetDC(), The DC Instance can be destroyed. (You may lost the Handle of DC)
		CClientDC dcClient( this);
		CFont *pOldFont = dcClient.SelectObject( m_pRscFont);
		BOOL bRet = m_pAddInManager->OnLButtonDblClk( &dcClient, nFlags, point, m_dwMouseActionID);
		dcClient.SelectObject( pOldFont);
	}
}

void CKTBChartCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
TRACE( "::OnRButtonDown()\r\n");

	// (2009/5/7 - Seung-Won, Bae) Set focus to kill focus of Zoom Edit on MultiChart.
	SetFocus();

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse DownUp Count for AddIn's Mouse Action ID
	if( ULONG_MAX == m_dwMouseActionID) m_dwMouseActionID = 0;
	else m_dwMouseActionID++;

	// (2004.11.02, ��¿�) Mouse Capturing�� ��ø������ ó���ǵ���, ��ø Count�� ������ Capturing Interface�� �����Ѵ�.
	AddSetCapture();

	// (2006/2/6 - Seung-Won, Bae) for Mouse Action ID and Capture Count Handling
	OnRButtonDownProc( nFlags, point);

	COleControl::OnRButtonDown(nFlags, point);
}

// (2006/2/6 - Seung-Won, Bae) for Mouse Action ID and Capture Count Handling
void CKTBChartCtrl::OnRButtonDownProc( UINT nFlags, CPoint point)
{
	// (2006/1/21 - Seung-Won, Bae) Notify Mouse Move Event to Addin
	if( m_pAddInManager)
	{
		// Caution! Use Client DC for MsgLoop like TrackPopupMenu()
		//		if GetDC(), The DC Instance can be destroyed. (You may lost the Handle of DC)
		CClientDC dcClient( this);
		CFont *pOldFont = dcClient.SelectObject( m_pRscFont);
		m_pAddInManager->OnRButtonDown( &dcClient, nFlags, point, m_dwMouseActionID);
		dcClient.SelectObject( pOldFont);
	}
}

void CKTBChartCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
TRACE( "::OnRButtonUp()\r\n");

	// (2006/2/6 - Seung-Won, Bae) for Mouse Action ID and Capture Count Handling
	BOOL bResult = OnRButtonUpProc( nFlags, point);

	// (2004.11.02, ��¿�) Mouse Capturing�� ��ø������ ó���ǵ���, ��ø Count�� ������ Capturing Interface�� �����Ѵ�.
	//		���� ������ Capturing�� �����Ѵ�. (If�� �������� �ι��� �ʿ������..)
	RemoveSetCapture();

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse DownUp Count for AddIn's Mouse Action ID
	if( ULONG_MAX == m_dwMouseActionID) m_dwMouseActionID = 0;
	else m_dwMouseActionID++;

	// (2006/10/30 - Seung-Won, Bae) to deny Context Menu.
	if( bResult) COleControl::OnRButtonUp(nFlags, point);
}

// (2009/6/22 - Seung-Won, Bae) Check Market Index.
const UINT RMSG_CAN_ADD_MARKET_INDEX = ::RegisterWindowMessage( _T( "RMSG_CAN_ADD_MARKET_INDEX"));

// Chart���������� �޽��� �߰��� �� �ִ� ��ȸ�� �ִ� �޽���.
const UINT RMSG_ADD_PLATFORM_MENU = ::RegisterWindowMessage(_T("RMSG_ADD_PLATFORM_MENU"));

// wParam - Index Name String
// lParam - the Pointer to receive the result Msg String address (Out) - Can be NULL.
// return value : 0 - can not add, 1 - can add.

// (2006/2/6 - Seung-Won, Bae) for Mouse Action ID and Capture Count Handling
BOOL CKTBChartCtrl::OnRButtonUpProc( UINT nFlags, CPoint point)
{
	if( !m_pMainBlock) return FALSE;

	// cursor ����
	// (2006/3/23 - Seung-Won, Bae) Do not call OnCursorExchange() directlry. If you do that, WM_CONTEXTMENU will be occur.
	if(IsRunTimeMode()) SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::OBJECT_NONE, NULL);

	// (2006/1/21 - Seung-Won, Bae) Notify Mouse Move Event to Addin
	if( m_pAddInManager)
	{
		// Caution! Use Client DC for MsgLoop like TrackPopupMenu()
		//		if GetDC(), The DC Instance can be destroyed. (You may lost the Handle of DC)
		CClientDC dcClient( this);
		CFont *pOldFont = dcClient.SelectObject( m_pRscFont);
		BOOL bResult = m_pAddInManager->OnRButtonUp( &dcClient, nFlags, point, m_dwMouseActionID);
		dcClient.SelectObject( pOldFont);
		if( bResult) return FALSE;	// (2006/10/30 - Seung-Won, Bae) to deny Context Menu.
	}

	if( IsRunTimeMode())
	{
		ML_SET_LANGUAGE_RES();

		CMenu Context;
		CMenu * pPopup = NULL;
		BOOL bResult = FALSE;
		if( m_bAddGraphMenuShow)
		{
			// (2006/4/14 - Seung-Won, Bae) Show Special Indicator Ending Menu.
			if( IsRunningOneChart())
			{
				// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
				switch( m_nChartMode)
				{
					case CChartInfo::DEVELOPER:	bResult	= Context.LoadMenu( IDR_RBUTTON_SPECIAL_DEV);
												break;
					case CChartInfo::HTS:		bResult	= Context.LoadMenu( IDR_RBUTTON_SPECIAL_HTS);
												break;
					case CChartInfo::FX:		bResult	= Context.LoadMenu( IDR_RBUTTON_SPECIAL_FX);
												break;
					case CChartInfo::WORLD_ON:	bResult	= Context.LoadMenu( IDR_RBUTTON_SPECIAL_WORLDON);
												break;
				}
				if( bResult) pPopup = Context.GetSubMenu(0);
			}

			// (2004.05.25, ��¿�) Graph�� ��ϵǾ� ���� �ʴ� ��Ȳ������,
			//		Popup Menu�� �������� �ʴ´�.
			// (2004.07.07. ��¿�) ������Ʈ�� Graph�� ��ϵǾ� ���� �ʴ� ��츦 �Ǵ��ϴ� ����,
			//		DLL Container�� �ִ� ���� �����Ѵ�.
			//		(�Ϲ� Map������ ������Ʈ ��� �ٸ� Graph�� �ִ� ��찡 �����ϴ�.)
			else if( !( m_hContainerDllWnd && !m_pMainBlock->IsExistenceIndicator( _MTEXT( C2_PRICE_CHART))))
			{
				if(m_nRButtonMenuType == 0)
				{//�ܼ���
					bResult	= Context.LoadMenu(IDR_RBUTTON_SIMPLE);					
					if(bResult)	pPopup = Context.GetSubMenu(0);
				}
				else
				{ //������

					// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
					switch( m_nChartMode)
					{
						case CChartInfo::DEVELOPER:	bResult	= Context.LoadMenu( IDR_RBUTTON_FULL_DEV);
													break;
						case CChartInfo::HTS:		bResult	= Context.LoadMenu( IDR_RBUTTON_FULL_HTS);
													break;
						case CChartInfo::FX:		bResult	= Context.LoadMenu( IDR_RBUTTON_FULL_FX);
													break;
						case CChartInfo::WORLD_ON:	bResult	= Context.LoadMenu( IDR_RBUTTON_FULL_WORLDON);
													break;
					}

					if(bResult)
					{
						pPopup = Context.GetSubMenu(0);
						// (2004.11.02, ��¿�) �� Mouse ��ġ�� Chart Block�� ������ ó���ϴ� Popup Menu�� �����Ѵ�.
						//		OnUpdateReverseYscale()�� Menu ���ý� �ٽ� ȣ��Ǿ� Cursor Position�� �����ϹǷ� 
						//		�̰����� �̸� Menu Eable�� ���� ���� Ȯ���Ѵ�. (�޴������� ���� ������ �Ʒ����� �Ǵ� ��찡 �ֱ� �����̴�.)
						// 0. ���� �ʱ�ȭ�Ѵ�.
						m_bReverseYScale_ReverseChart	= FALSE;
						m_biReverseYScale_BlockIndex	= CBlockIndex( -1, -1);
						// 1. �� Cursor Position�� Block Index�� �����Ѵ�.
						if( m_pMainBlock) m_biReverseYScale_BlockIndex = m_pMainBlock->FindBlockIndex( point);

						// (2009/9/2 - Seung-Won, Bae) Check Market Index with search.
						CWnd *pWnd = GetParent();

						if( pWnd) pWnd->SendMessage( RMSG_ADD_PLATFORM_MENU, 0, ( long)pPopup->GetSafeHmenu());

						long lResult = 0;
						if( pWnd && pPopup && CChartInfo::HTS == m_nChartMode)
						{
							CString strAddIndicator;
							strAddIndicator.LoadString( IDS_ADD_INDICATOR);
							CMenu *pMenuAddIndicator = GetSubMenu( pPopup, strAddIndicator);
							if( pMenuAddIndicator)
							{
								// Check Market Index Menu.
								CString strMenuString;
								int nMenuMarket = GetSubMenuIndex( pMenuAddIndicator, _LTEXT7( C7_MARKET));
								CMenu *pMenuMarket = pMenuAddIndicator->GetSubMenu( nMenuMarket);
								int nMenuCount = pMenuMarket->GetMenuItemCount();
								for( int i = nMenuCount - 1; 0 <= i; i--)
								{
									pMenuMarket->GetMenuString( i, strMenuString, MF_BYPOSITION);
									if( !pWnd->SendMessage( RMSG_CAN_ADD_MARKET_INDEX, ( unsigned int)_LTOM2( strMenuString), NULL))
										pMenuMarket->DeleteMenu( i, MF_BYPOSITION);
								}
								if( pMenuMarket->GetMenuItemCount() <= 0)
									pMenuAddIndicator->DeleteMenu( nMenuMarket, MF_BYPOSITION);

								// Check ETC Menu.
								CMenu *pMenuETC = GetSubMenu( pMenuAddIndicator, _LTEXT7( C7_ETC));
								if( pMenuETC)
								{
									// Volume Amount
									int nMenuVolumeAmount = GetSubMenuIndex( pMenuETC, _LTEXT2( C2_VOLUME_AMOUNT));
									if( 0 <= nMenuVolumeAmount) if( !pWnd->SendMessage( RMSG_CAN_ADD_MARKET_INDEX, ( unsigned int)_MTEXT( C2_VOLUME_AMOUNT), NULL))
										pMenuETC->DeleteMenu( nMenuVolumeAmount, MF_BYPOSITION);
								}
							}
						}
					}
				}
			}
		}
		else
		{
			bResult	= Context.LoadMenu( IDR_RBUTTON_NONE);					
			if(bResult)	pPopup = Context.GetSubMenu( 0);
		}

		// (2006/8/25 - Seung-Won, Bae) Process Popup Menu.
		if( pPopup)
		{
			// (2008/3/7 - Seung-Won, Bae) Backup R-Button Menu popup Position for ON_UPDATE_COMMAND_UI and ON_COMMAND
			m_ptRButtonMenuPopupPosition = point;
			ClientToScreen(&point);	

			CString strAddIndicator;
			strAddIndicator.LoadString( IDS_ADD_INDICATOR);
			CString strChangeSpecialChart;
			strChangeSpecialChart.LoadString( IDS_CHANGE_SPECIAL_CHART);
			// (2006/8/25 - Seung-Won, Bae) Delete disabled menu.
			//		Caution! so, Make sure the OnUpdateCammandUI() Function for Enable with force.
			int nMenuID = 0;
			int nMenuCount = pPopup->GetMenuItemCount();
			CCmdUI cmdUI; 
			CString strMenuName;
			for( int i = nMenuCount - 1; 0 <= i; i--)
			{
				cmdUI.m_nID			= pPopup->GetMenuItemID( i);
				// Check the ID for Indicator Menu Process AddIn
				if( pPopup->GetMenuString( i, strMenuName, MF_BYPOSITION))
					if( strMenuName == strAddIndicator || strMenuName == strChangeSpecialChart)
						cmdUI.m_nID = ID_AIC_START_ID;

				// Skip Seperator
				if( 0 == cmdUI.m_nID)
				{
					// delete Serial Bottom Seperators
					if( i == nMenuCount - 1)
					{
						do
						{
							pPopup->DeleteMenu( i, MF_BYPOSITION);
							i = pPopup->GetMenuItemCount() - 1;
						}
						while( 0 <= i && !pPopup->GetMenuItemID( i));
						i++;
					}
					// delete Serial Seperators
					else if( i + 1 < nMenuCount && 0 == pPopup->GetMenuItemID( i + 1))
					{
						pPopup->DeleteMenu( i, MF_BYPOSITION);
						nMenuCount--;
					}
					// Skip Seperator
					continue;
				}

				cmdUI.m_nIndex		= i;
				cmdUI.m_pMenu		= pPopup;
				cmdUI.m_pSubMenu	= NULL;
				cmdUI.m_pOther		= NULL;
				cmdUI.m_nIndexMax	= nMenuCount;
				cmdUI.Enable( FALSE);
				OnCmdMsg( cmdUI.m_nID, CN_UPDATE_COMMAND_UI, &cmdUI, NULL);
				nMenuID = pPopup->GetMenuItemID( i);
				if( ( pPopup->GetMenuState( i, MF_BYPOSITION) & MF_DISABLED) && ( nMenuID < ID_PLM_ADD_MENU || ID_PLM_ADD_MENU_END < nMenuID))
				{
					MENUITEMINFO mii;
					mii.cbSize = sizeof( MENUITEMINFO); // must fill up this field
					mii.fMask = MIIM_DATA;             // get the state of the menu item
					pPopup->GetMenuItemInfo( i, &mii, TRUE);
					if( mii.dwItemData != 9999)
					{
						pPopup->DeleteMenu( i, MF_BYPOSITION);
						nMenuCount--;
					}
				}
			}
			// delete Top Seperators
			while( 0 < pPopup->GetMenuItemCount() && 0 == pPopup->GetMenuItemID( 0))
			{
				pPopup->DeleteMenu( 0, MF_BYPOSITION);
				nMenuCount--;
			}

			// (2007/1/6 - Seung-Won, Bae) Add Platform Menu
			CWnd *pWnd = GetParent();
			if( pWnd) pWnd->SendMessage( UDM_ADD_PLATFORM_MENU, 0, ( long)pPopup->GetSafeHmenu());

			if( pPopup->GetMenuItemCount())
				pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this, NULL);

			Context.DestroyMenu();
		}
	}

	return TRUE;
}

// (2009/9/2 - Seung-Won, Bae) Check Market Index with search.
int CKTBChartCtrl::GetSubMenuIndex( CMenu *p_pMainMenu, const char *p_szMenuString)
{
	if( !p_pMainMenu || !p_szMenuString) return -1;
	int nMenuItemCount = p_pMainMenu->GetMenuItemCount();
	if( nMenuItemCount <= 0 || !*p_szMenuString) return -1;

	CString strMenuString;
	for( int i = 0; i < nMenuItemCount; i++)
	{
		p_pMainMenu->GetMenuString( i, strMenuString, MF_BYPOSITION);
		if( strMenuString == p_szMenuString) return i;
	}
	return -1;
}
CMenu *CKTBChartCtrl::GetSubMenu( CMenu *p_pMainMenu, const char *p_szMenuString)
{
	int nMenuIndex = GetSubMenuIndex( p_pMainMenu, p_szMenuString);
	if( nMenuIndex <= 0) return NULL;
	return p_pMainMenu->GetSubMenu( nMenuIndex);
}

void CKTBChartCtrl::OnRButtonDblClk(UINT nFlags, CPoint point)
{
// (2009/9/6 - Seung-Won, Bae) for TEST of Horizontal Scale Height
//#ifdef _DEBUG
//	CBlockIndex biSelect = m_pMainBlock->FindBlockIndex( point);
//	if( biSelect.IsAllValueBiggerThanZero())
//	{
//		CBlock *pBlock = m_pMainBlock->GetBlock( biSelect);
//		if( pBlock)
//		{
//			int nScaleHeight = pBlock->GetHorizontalScaleHeight();
//			nScaleHeight -= 10;
//			pBlock->SetHorizontalScaleHeight( nScaleHeight);
//		}
//	}
//#endif

	// TODO: Add your message handler code here and/or call default
TRACE( "::OnRButtonDblClk()\r\n");

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse DownUp Count for AddIn's Mouse Action ID
	if( ULONG_MAX == m_dwMouseActionID) m_dwMouseActionID = 0;
	else m_dwMouseActionID++;

	// (2004.11.02, ��¿�) Mouse Capturing�� ��ø������ ó���ǵ���, ��ø Count�� ������ Capturing Interface�� �����Ѵ�.
	AddSetCapture();

	// (2006/2/6 - Seung-Won, Bae) for Mouse Action ID and Capture Count Handling
	OnRButtonDblClkProc( nFlags, point);
	
	COleControl::OnRButtonDblClk(nFlags, point);
}

// (2006/2/6 - Seung-Won, Bae) for Mouse Action ID and Capture Count Handling
void CKTBChartCtrl::OnRButtonDblClkProc( UINT nFlags, CPoint point)
{
	// (2006/1/17 - Seung-Won, Bae) Notify Mouse Move Event to Addin
	if( m_pAddInManager)
	{
		// Caution! Use Client DC for MsgLoop like TrackPopupMenu()
		//		if GetDC(), The DC Instance can be destroyed. (You may lost the Handle of DC)
		CClientDC dcClient( this);
		CFont *pOldFont = dcClient.SelectObject( m_pRscFont);
		m_pAddInManager->OnRButtonDblClk( &dcClient, nFlags, point, m_dwMouseActionID);
		dcClient.SelectObject( pOldFont);
	}
}
