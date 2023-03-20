// IWChartOcx050116.cpp: implementation of the CIWChartOcx050116 class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// *. ����!	050116 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� Wrapper�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		������ �ʴ� ������ 050116 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050116 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			��¿�
//			ChartPatternAddIn			GZone			������
//			ChartGuideLine				GZone			������
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWChartOcx050116.h"

#include MChartOcxInclude				// for MChartOcx
#include "IWPacketManager050116.h"		// for CIWPacketManager050116
#include "IWChartManager050116.h"		// for CIWChartManager050116
#include "IWGraphInfoManager050116.h"	// for CIWGraphInfoManager050116

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// (2004.10.08, ��¿�)	Chart OCX�� Pointer�� �����Ѵ�.
//		������ �� OCX�� Ctrl Class�̸�, StdAfx.h�� Macro�� �����Ͽ� �̿��Ѵ�.
CIWChartOcx050116::CIWChartOcx050116( MChartOcx *p_pChartOcx)
{
	m_pChartOcx = p_pChartOcx;
	if( !m_pChartOcx)
	{
		AfxMessageBox( "CIWChartOcx050116 Null Wrapping Error!");
		return;
	}
}

CIWChartOcx050116::~CIWChartOcx050116()
{

}

//////////////////////////////////////////////////////////////////////
// (2004.10.19, ��¿�) Windows ���� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04.10.11] ReDrawing�� ���� Interface�� �����Ѵ�.
void CIWChartOcx050116::InvalidateControl( void)
{
	if( m_pChartOcx) m_pChartOcx->InvalidateControl();
}

// [04/10/11] ReDrawing�� ���� Interface�� �����Ѵ�.
HWND CIWChartOcx050116::GetOcxHwnd( void)
{
	if( m_pChartOcx) return m_pChartOcx->GetSafeHwnd();
	return NULL;
}

// [04/11/02] Counter Capturing Interface�� �����Ѵ�.
HWND CIWChartOcx050116::AddSetCapture( void)
{
	if( m_pChartOcx)
	{
		CWnd *pWnd = m_pChartOcx->AddSetCapture();
		if( pWnd) return pWnd->GetSafeHwnd();
	}
	return NULL;
}

// [04/11/02] Counter Capturing Interface�� �����Ѵ�.
BOOL CIWChartOcx050116::RemoveSetCapture( void)
{
	if( m_pChartOcx) return m_pChartOcx->RemoveSetCapture();
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.18, ��¿�) ȯ�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04.10.18] ����� Folder�� ���ϴ� Interface�� �����Ѵ�.
BOOL CIWChartOcx050116::GetUserFilePath( CString &p_strUserFilePath)
{
	if( m_pChartOcx)
	{
		p_strUserFilePath = m_pChartOcx->GetUserFilePath();
		return !p_strUserFilePath.IsEmpty();
	}
	return FALSE;
}

// [04.10.25] Chart ���� Font�� �����Ѵ�.
CFont *CIWChartOcx050116::GetChartFont( void)
{
	if( !m_pChartOcx) return NULL;
	return m_pChartOcx->GetChartFont();
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) Chart�� �� ���¸� Ȯ���ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04.10.11] Ư����Ʈ�� ������� Ȯ���ϴ� Interface�� �����Ѵ�. (���ð�/�Ｑ��ȯ��/P&F)
BOOL CIWChartOcx050116::IsOnSpecialChart( void)
{
	if( m_pChartOcx) return m_pChartOcx->IsRunningOneChart();
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2005.01.20, ��¿�) ToolBar�� ���� Item�� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

BOOL CIWChartOcx050116::HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem)
{
	if( m_pChartOcx) return m_pChartOcx->HasToolItem( p_eToolItem);
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) Data Index �� Position ���� Interface
//////////////////////////////////////////////////////////////////////

// [04/10/11] ������ �� Data�� Index�� ���ϴ� Interface�� �����Ѵ�.
int CIWChartOcx050116::GetLastRealDataIndex( void)
{
	if( m_pChartOcx) return m_pChartOcx->GetDataCountForToolBar();
	return -1;
}

// [04/10/11] ScrollBar���� ������ Scroll Index�� ���ϴ� Interface�� �����Ѵ�. (��������)
BOOL CIWChartOcx050116::GetMaxScrollRange( int &p_nMaxScrollRange)
{
	if( m_pChartOcx) return m_pChartOcx->GetScrollRange( p_nMaxScrollRange);
	return FALSE;
}

// [04/10/11] Chart�� ���� ���̴� View ������ ù Data Index�� �� Data Index�� ���ϰų�, �����ϴ� Interface�� �����Ѵ�.
BOOL CIWChartOcx050116::GetViewStartEndIndex( int &p_nStartIndex, int &p_nEndIndex)
{
	if( m_pChartOcx) return m_pChartOcx->GetDataStartEnd( p_nStartIndex, p_nEndIndex);
	return FALSE;
}
BOOL CIWChartOcx050116::SetViewStartEndIndex( int p_nStartIndex, int p_nEndIndex)
{
	if( m_pChartOcx) return m_pChartOcx->SetDataStartEnd( p_nStartIndex, p_nEndIndex);
	return FALSE;
}

// [04/10/27] Data ��ǥ�� Screen ��ǥ�� ��ȯ�ϴ� Interface�� �����Ѵ�.
int CIWChartOcx050116::ConvertDataToYPosition( double p_dData, double p_dDataMax, double p_dDataMin, int p_nYPositionMax, int p_nYPositionMin, BOOL p_bLog, BOOL p_bReverse)
{
	if( m_pChartOcx) return m_pChartOcx->ConvertDataToYPosition( p_dData, p_dDataMax, p_dDataMin, p_nYPositionMax, p_nYPositionMin, p_bLog, p_bReverse);
	return 0;
}
double CIWChartOcx050116::ConvertYPositionToData( int p_nYPosition, int p_nYPositionMax, int p_nYPositionMin, double p_dDataMax, double p_dDataMin, BOOL p_bLog, BOOL p_bReverse)
{
	if( m_pChartOcx) return m_pChartOcx->ConvertYPositionToData( p_nYPosition, p_nYPositionMax, p_nYPositionMin, p_dDataMax, p_dDataMin, p_bLog, p_bReverse);
	return 0;
}

// [04/11/09] Scroll/Zoom�� ���� ó���ϴ� Interface�� �����Ѵ�.
BOOL CIWChartOcx050116::ScrollTo( int p_nScrollValue, BOOL p_bRelative)
{
	if( m_pChartOcx) return m_pChartOcx->ScrollTo( p_nScrollValue, p_bRelative);
	return FALSE;
}
BOOL CIWChartOcx050116::ZoomTo( int p_nZoomValue, BOOL p_bRelative)
{
	if( m_pChartOcx) return m_pChartOcx->ZoomTo( p_nZoomValue, p_bRelative);
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// [04.10.11] ���� ���� Interface
//////////////////////////////////////////////////////////////////////

// [04.10.11] �־��� Global ������ �����Ǿ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
BOOL CIWChartOcx050116::CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet)
{
	if( !m_pChartOcx) return FALSE;
	ST_GSHAREDFORMAT SDF = m_pChartOcx->FindGSharedDataInfo( p_szGSharedName);
	if( SDF.m_szSharedName.IsEmpty()) return FALSE;
	return SDF.m_bStart && p_bStart || SDF.m_bGet && p_bGet || SDF.m_bSet && p_bSet;
}

// [04.10.11] �־��� Global ������ ������ �߻���Ű�� Interface�� �����Ѵ�.
BOOL CIWChartOcx050116::FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData)
{
	if( !m_pChartOcx) return FALSE;
	return m_pChartOcx->FireGSharedData( p_szGSharedName, p_szGSharedData);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
IPacketManager050116 *CIWChartOcx050116::GetIPacketManager050116( void)
{
	if( !m_pChartOcx) return NULL;
	CPacketList *pPacketList = m_pChartOcx->GetPacketList();
	if( !pPacketList) return NULL;
	IPacketManager050116 *pIPacketManager050116 = new CIWPacketManager050116( pPacketList);
	pIPacketManager050116->AddRef();
	return pIPacketManager050116;
}
IChartManager050116 *CIWChartOcx050116::GetIChartManager050116( void)
{
	if( !m_pChartOcx) return NULL;
	CMainBlock *pMainBlock = m_pChartOcx->GetMainBlock();
	if( !pMainBlock) return NULL;
	IChartManager050116 *pIChartManager050116 = new CIWChartManager050116( pMainBlock);
	pIChartManager050116->AddRef();
	return pIChartManager050116;
}
IGraphInfoManager050116 *CIWChartOcx050116::GetIGraphInfoManager050116( void)
{
	if( !m_pChartOcx) return NULL;
	CIndicatorList *pIndicatorList = m_pChartOcx->GetIndicatorList();
	if( !pIndicatorList) return NULL;
	IGraphInfoManager050116 *pIGraphInfoManager050116 = new CIWGraphInfoManager050116( pIndicatorList);
	pIGraphInfoManager050116->AddRef();
	return pIGraphInfoManager050116;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.19, ��¿�) Image ���� �� ����� ���� Drawing Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2004.10.19, ��¿�) Image ���� �� ����� ���� Drawing Interface�� �����Ѵ�.
void CIWChartOcx050116::DrawForImage( CDC *p_pDC, CRect p_rctSource, BOOL p_bUseScreenColor)
{
	if( !m_pChartOcx) return;
	CBitmap bufferBitmap;
	m_pChartOcx->OnDrawForPrint( *p_pDC, bufferBitmap, p_rctSource, p_bUseScreenColor);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.25, ��¿�) Cursor ���� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04/10/25] ���ο� Cursor�� �����ϸ鼭, ���� ������ Cursor�� Ȯ���Ѵ�.
HCURSOR CIWChartOcx050116::SetCursorHandle( HCURSOR p_hNewCursor)
{
	if( !m_pChartOcx) return NULL;
	return m_pChartOcx->SetCursorHandle( p_hNewCursor);
}

// [04/10/26] ���� ������ Cursor�� Ȯ���Ѵ�.
HCURSOR CIWChartOcx050116::GetCursorHandle( void)
{
	if( !m_pChartOcx) return NULL;
	return m_pChartOcx->GetCursorHandle();
}

//////////////////////////////////////////////////////////////////////
// (2005.01.13, ��¿�) ��Ʈ�� ��ü���� Mode ���θ� Ȯ�� �� �����ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

BOOL CIWChartOcx050116::CheckFullView( void) const
{
	if( !m_pChartOcx) return FALSE;
	return m_pChartOcx->IsWholeView();
}

void CIWChartOcx050116::SetFullView( BOOL p_bFullView)
{
	if( !m_pChartOcx) return;
	if( ( p_bFullView && m_pChartOcx->IsWholeView())
		|| ( !p_bFullView && !m_pChartOcx->IsWholeView())) return;

	m_pChartOcx->SetWholeView( p_bFullView);
	m_pChartOcx->InvalidateControl();
}

//////////////////////////////////////////////////////////////////////
// 050116 Version, ���� Interface
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			��¿�
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Temp Interface I : DLL Container ���� Interface
//////////////////////////////////////////////////////////////////////

// [04.10.11] DLL Container���� Message�ϴ� Interface�� �����Ѵ�.
LRESULT CIWChartOcx050116::SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam)
{
	if( m_pChartOcx) m_pChartOcx->SendEventMsgToDLL( message, wParam, lParam);
	return 0;
}
