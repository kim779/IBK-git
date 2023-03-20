// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
// IWChartOcx050531.cpp: implementation of the CIWChartOcx050531 class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// *. ����!	050531 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� Wrapper�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		������ �ʴ� ������ 050531 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			��ȭ			��¿�
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWChartOcx050531.h"

#include MChartOcxInclude				// for MChartOcx
#include "IWPacketManager050531.h"		// for CIWPacketManager050531
#include "IWChartManager050531.h"		// for CIWChartManager050531
#include "IWGraphInfoManager050531.h"	// for CIWGraphInfoManager050531

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
CIWChartOcx050531::CIWChartOcx050531( MChartOcx *p_pChartOcx)
{
	m_pChartOcx = p_pChartOcx;
	if( !m_pChartOcx)
	{
		AfxMessageBox( "CIWChartOcx050531 Null Wrapping Error!");
		return;
	}
}

CIWChartOcx050531::~CIWChartOcx050531()
{

}

//////////////////////////////////////////////////////////////////////
// (2004.10.19, ��¿�) Windows ���� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04.10.11] ReDrawing�� ���� Interface�� �����Ѵ�.
void CIWChartOcx050531::InvalidateControl( void)
{
	if( m_pChartOcx) m_pChartOcx->InvalidateControl();
}

// [04/10/11] ReDrawing�� ���� Interface�� �����Ѵ�.
HWND CIWChartOcx050531::GetOcxHwnd( void)
{
	if( m_pChartOcx) return m_pChartOcx->GetSafeHwnd();
	return NULL;
}

// [04/11/02] Counter Capturing Interface�� �����Ѵ�.
HWND CIWChartOcx050531::AddSetCapture( void)
{
	if( m_pChartOcx)
	{
		CWnd *pWnd = m_pChartOcx->AddSetCapture();
		if( pWnd) return pWnd->GetSafeHwnd();
	}
	return NULL;
}

// [04/11/02] Counter Capturing Interface�� �����Ѵ�.
BOOL CIWChartOcx050531::RemoveSetCapture( void)
{
	if( m_pChartOcx) return m_pChartOcx->RemoveSetCapture();
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.18, ��¿�) ȯ�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04.10.18] ����� Folder�� ���ϴ� Interface�� �����Ѵ�.
BOOL CIWChartOcx050531::GetUserFilePath( CString &p_strUserFilePath)
{
	if( m_pChartOcx)
	{
		p_strUserFilePath = m_pChartOcx->GetUserFilePath();
		return !p_strUserFilePath.IsEmpty();
	}
	return FALSE;
}

// [04.10.25] Chart ���� Font�� �����Ѵ�.
CFont *CIWChartOcx050531::GetChartFont( void)
{
	if( !m_pChartOcx) return NULL;
	return m_pChartOcx->GetChartFont();
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) Chart�� �� ���¸� Ȯ���ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04.10.11] Ư����Ʈ�� ������� Ȯ���ϴ� Interface�� �����Ѵ�. (���ð�/�Ｑ��ȯ��/P&F)
BOOL CIWChartOcx050531::IsOnSpecialChart( void)
{
	if( m_pChartOcx) return m_pChartOcx->IsRunningOneChart();
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2005.01.20, ��¿�) ToolBar�� ���� Item�� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

BOOL CIWChartOcx050531::HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem)
{
	if( m_pChartOcx) return m_pChartOcx->HasToolItem( p_eToolItem);
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) Data Index �� Position ���� Interface
//////////////////////////////////////////////////////////////////////

// [04/10/11] ������ �� Data�� Index�� ���ϴ� Interface�� �����Ѵ�.
int CIWChartOcx050531::GetLastRealDataIndex( void)
{
	if( m_pChartOcx) return m_pChartOcx->GetDataCountForToolBar();
	return -1;
}

// [04/10/11] ScrollBar���� ������ Scroll Index�� ���ϴ� Interface�� �����Ѵ�. (��������)
BOOL CIWChartOcx050531::GetMaxScrollRange( int &p_nMaxScrollRange)
{
	if( m_pChartOcx) return m_pChartOcx->GetScrollRange( p_nMaxScrollRange);
	return FALSE;
}

// [04/10/11] Chart�� ���� ���̴� View ������ ù Data Index�� �� Data Index�� ���ϰų�, �����ϴ� Interface�� �����Ѵ�.
BOOL CIWChartOcx050531::GetViewStartEndIndex( int &p_nStartIndex, int &p_nEndIndex)
{
	if( m_pChartOcx) return m_pChartOcx->GetDataStartEnd( p_nStartIndex, p_nEndIndex);
	return FALSE;
}
BOOL CIWChartOcx050531::SetViewStartEndIndex( int p_nStartIndex, int p_nEndIndex)
{
	if( m_pChartOcx) return m_pChartOcx->SetDataStartEnd( p_nStartIndex, p_nEndIndex);
	return FALSE;
}

// [04/10/27] Data ��ǥ�� Screen ��ǥ�� ��ȯ�ϴ� Interface�� �����Ѵ�.
int CIWChartOcx050531::ConvertDataToYPosition( double p_dData, double p_dDataMax, double p_dDataMin, int p_nYPositionMax, int p_nYPositionMin, BOOL p_bLog, BOOL p_bReverse)
{
	if( m_pChartOcx) return m_pChartOcx->ConvertDataToYPosition( p_dData, p_dDataMax, p_dDataMin, p_nYPositionMax, p_nYPositionMin, p_bLog, p_bReverse);
	return 0;
}
double CIWChartOcx050531::ConvertYPositionToData( int p_nYPosition, int p_nYPositionMax, int p_nYPositionMin, double p_dDataMax, double p_dDataMin, BOOL p_bLog, BOOL p_bReverse)
{
	if( m_pChartOcx) return m_pChartOcx->ConvertYPositionToData( p_nYPosition, p_nYPositionMax, p_nYPositionMin, p_dDataMax, p_dDataMin, p_bLog, p_bReverse);
	return 0;
}

// [04/11/09] Scroll/Zoom�� ���� ó���ϴ� Interface�� �����Ѵ�.
BOOL CIWChartOcx050531::ScrollTo( int p_nScrollValue, BOOL p_bRelative)
{
	if( m_pChartOcx) return m_pChartOcx->ScrollTo( p_nScrollValue, p_bRelative);
	return FALSE;
}
BOOL CIWChartOcx050531::ZoomTo( int p_nZoomValue, BOOL p_bRelative)
{
	if( m_pChartOcx) return m_pChartOcx->ZoomTo( p_nZoomValue, p_bRelative);
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// [04.10.11] ���� ���� Interface
//////////////////////////////////////////////////////////////////////

// [04.10.11] �־��� Global ������ �����Ǿ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
BOOL CIWChartOcx050531::CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet)
{
	if( !m_pChartOcx) return FALSE;
	ST_GSHAREDFORMAT SDF = m_pChartOcx->FindGSharedDataInfo( p_szGSharedName);
	if( SDF.m_szSharedName.IsEmpty()) return FALSE;
	return SDF.m_bStart && p_bStart || SDF.m_bGet && p_bGet || SDF.m_bSet && p_bSet;
}

// [04.10.11] �־��� Global ������ ������ �߻���Ű�� Interface�� �����Ѵ�.
BOOL CIWChartOcx050531::FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData)
{
	if( !m_pChartOcx) return FALSE;
	return m_pChartOcx->FireGSharedData( p_szGSharedName, p_szGSharedData);
}

// (2005/8/10 - Seung-Won, Bae) used in Only Hanwha
BOOL CIWChartOcx050531::GetShareInfoItem( const char *p_szShareName, int &p_nShareType, int &p_nShareArea, long &p_lCodeFilter)
{
	return FALSE;
}
BOOL CIWChartOcx050531::FireSharedData( const char *p_szShareName, const char *p_szSharedData)
{
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
IPacketManager050531 *CIWChartOcx050531::GetIPacketManager050531( void)
{
	if( !m_pChartOcx) return NULL;
	CPacketList *pPacketList = m_pChartOcx->GetPacketList();
	if( !pPacketList) return NULL;
	IPacketManager050531 *pIPacketManager050531 = new CIWPacketManager050531( pPacketList);
	pIPacketManager050531->AddRef();
	return pIPacketManager050531;
}
IChartManager050531 *CIWChartOcx050531::GetIChartManager050531( void)
{
	if( !m_pChartOcx) return NULL;
	CMainBlock *pMainBlock = m_pChartOcx->GetMainBlock();
	if( !pMainBlock) return NULL;
	IChartManager050531 *pIChartManager050531 = new CIWChartManager050531( pMainBlock);
	pIChartManager050531->AddRef();
	return pIChartManager050531;
}
IGraphInfoManager050531 *CIWChartOcx050531::GetIGraphInfoManager050531( void)
{
	if( !m_pChartOcx) return NULL;
	CIndicatorList *pIndicatorList = m_pChartOcx->GetIndicatorList();
	if( !pIndicatorList) return NULL;
	IGraphInfoManager050531 *pIGraphInfoManager050531 = new CIWGraphInfoManager050531( pIndicatorList);
	pIGraphInfoManager050531->AddRef();
	return pIGraphInfoManager050531;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.19, ��¿�) Image ���� �� ����� ���� Drawing Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// (2004.10.19, ��¿�) Image ���� �� ����� ���� Drawing Interface�� �����Ѵ�.
void CIWChartOcx050531::DrawForImage( CDC *p_pDC, CRect p_rctSource, BOOL p_bUseScreenColor)
{
	if( !m_pChartOcx) return;
	CBitmap bufferBitmap;
	m_pChartOcx->OnDrawForPrint( *p_pDC, bufferBitmap, p_rctSource, p_bUseScreenColor);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.25, ��¿�) Cursor ���� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04/10/25] ���ο� Cursor�� �����ϸ鼭, ���� ������ Cursor�� Ȯ���Ѵ�.
HCURSOR CIWChartOcx050531::SetCursorHandle( HCURSOR p_hNewCursor)
{
	if( !m_pChartOcx) return NULL;
	return m_pChartOcx->SetCursorHandle( p_hNewCursor);
}

// [04/10/26] ���� ������ Cursor�� Ȯ���Ѵ�.
HCURSOR CIWChartOcx050531::GetCursorHandle( void)
{
	if( !m_pChartOcx) return NULL;
	return m_pChartOcx->GetCursorHandle();
}

//////////////////////////////////////////////////////////////////////
// (2005.01.13, ��¿�) ��Ʈ�� ��ü���� Mode ���θ� Ȯ�� �� �����ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

BOOL CIWChartOcx050531::CheckFullView( void) const
{
	if( !m_pChartOcx) return FALSE;
	return m_pChartOcx->IsWholeView();
}

void CIWChartOcx050531::SetFullView( BOOL p_bFullView)
{
	if( !m_pChartOcx) return;
	if( ( p_bFullView && m_pChartOcx->IsWholeView())
		|| ( !p_bFullView && !m_pChartOcx->IsWholeView())) return;

	m_pChartOcx->SetWholeView( p_bFullView);
	m_pChartOcx->InvalidateControl();
}

//////////////////////////////////////////////////////////////////////
// ����(05/06/01) �׷����� �߰�,����,���� ���� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////
/*-----------------------------------------------------------------------------------------
 - Function    :  AddPacket
 - Created at  :  2005-06-01   10:12
 - Author      :  ������
 - Description :  ��Ŷ�� �߰��Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CIWChartOcx050531::AddPacket(const char* pcszPacketOption)
{
	if(!m_pChartOcx)
		return;
	m_pChartOcx->AddPacketAdapter(pcszPacketOption);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  AddGraph
 - Created at  :  2005-06-01   13:35
 - Author      :  ������
 - Description :  ��ǥ�� �߰��� �Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CIWChartOcx050531::AddGraph(short nRow, short nColumn, short nVertScaleGroup, const char* szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale)
{
	if(!m_pChartOcx)
		return;
	m_pChartOcx->AddGraphAdapter(nRow, nColumn, nVertScaleGroup, szGraphOption, bCreateBlock, bCreateVertScaleGroup, bUseAutoScale);
	return;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  ChangeSubGraphData
 - Created at  :  2005-06-01   16:45
 - Author      :  ������
 - Description :  ��ǥ���� �׷��� �Ӽ�(��, �β�, ����)�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOcx050531::ChangeSubGraphData(const char* pcszIndicatorName, const char* pcszSubGraphName, COLORREF clrLineColor, int nThickness, int nDrawStyle)
{
	if(!m_pChartOcx)
		return false;
	return m_pChartOcx->ChangeSubGraphData(pcszIndicatorName, pcszSubGraphName, clrLineColor, nThickness, nDrawStyle);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetStartPeriodOfPacket
 - Created at  :  2005-06-07   15:55
 - Author      :  ������
 - Description :  Packet�� �������ڸ� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOcx050531::SetStartPeriodOfPacket(const char* pcszPacketName, long lStartPeriod, BOOL bAddSpace)
{
	if(!m_pChartOcx)
		return false;
	return m_pChartOcx->SetStartPeriodOfPacketAdapter(pcszPacketName, lStartPeriod, bAddSpace);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetStartIndexOfPacket
 - Created at  :  2005-08-27   17:44
 - Author      :  ������
 - Description :  Packet�� �����ε����� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOcx050531::SetStartIndexOfPacket(const char* pcszPacketName, int nStartDataIndex, BOOL bAddSpace)
{
	if(!m_pChartOcx)
		return false;
	return m_pChartOcx->SetStartIndexOfPacket(pcszPacketName, nStartDataIndex, bAddSpace);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  HasGraph
 - Created at  :  2005-06-07   15:56
 - Author      :  ������
 - Description :  �ش� �׷����� �ִ��� Ȯ���Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOcx050531::HasGraph(const char* pcszGraphName)
{
	if(!m_pChartOcx)
		return false;
	return m_pChartOcx->HasGraphAdapter(pcszGraphName);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  RemoveGraph
 - Created at  :  2005-11-07
 - Author      :  �輱��
 - Description :  �ش� �׷����� �����ϱ�.
 -----------------------------------------------------------------------------------------*/
bool CIWChartOcx050531::RemoveGraph(const char* pcszIndicatorName)
{
	if(m_pChartOcx == NULL)
		return false;
	return m_pChartOcx->RemoveChart(pcszIndicatorName);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  RemovePacket
 - Created at  :  2005-11-07
 - Author      :  �輱��
 - Description :  �ش� packet �����ϱ�.
					bIsOnlyDataClear = true : data �� ����
					bIsOnlyDataClear = false : CPacket ����
 -----------------------------------------------------------------------------------------*/
bool CIWChartOcx050531::RemovePacket(const char* pcszPacketName, const bool bIsOnlyDataClear)
{
	if(m_pChartOcx == NULL)
		return false;

	if(!bIsOnlyDataClear)
		return m_pChartOcx->RemovePacket(pcszPacketName);
	return m_pChartOcx->ClearPacketData(pcszPacketName, true);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetHtsPath
 - Created at  :  2005-07-14   10:59
 - Author      :  ������
 - Description :  Hts�� Dir Path�� �˷��ش�.
 -----------------------------------------------------------------------------------------*/
LPCTSTR CIWChartOcx050531::GetHtsPath(void)
{
	if(!m_pChartOcx)
		return _T("");
	return m_pChartOcx->GetBaseDeskPath();
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/18
// Modifier		: 
// Comments		: for Event from AddIn
//-----------------------------------------------------------------------------
void CIWChartOcx050531::FireAddInEvent( const char *p_szAddInName, short p_nEventID, const char *p_szData, double p_dData, long p_lData)
{
	if( !m_pChartOcx) return;
	m_pChartOcx->NeoAddInEvent( p_szAddInName, p_nEventID, p_szData, p_dData, p_lData);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetRealData
 - Created at  :  2005-08-22   20:09
 - Author      :  ������
 - Parameters  :  pcszPacketName - ��Ŷ��
 -                dData          - ������Ÿ
 -                nOption        - 1: Update, 2:Append
 - Description :  ������Ÿ�� ó���ϴ� Interface�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CIWChartOcx050531::SetRealData(const char* pcszPacketName, double dData, short nOption)
{
	if(!m_pChartOcx)
		return;
	m_pChartOcx->SetRealDataAdapter(pcszPacketName, dData, nOption);
}

//////////////////////////////////////////////////////////////////////
// 050531 Version, ���� Interface
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			��ȭ			��¿�
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Temp Interface I : DLL Container ���� Interface
//////////////////////////////////////////////////////////////////////

// [04.10.11] DLL Container���� Message�ϴ� Interface�� �����Ѵ�.
LRESULT CIWChartOcx050531::SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam)
{
	if( m_pChartOcx) m_pChartOcx->SendEventMsgToDLL( message, wParam, lParam);
	return 0;
}
