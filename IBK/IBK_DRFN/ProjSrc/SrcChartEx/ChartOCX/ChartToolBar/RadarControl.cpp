// RadarControl.cpp: implementation of the CRadarControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RadarControl.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "Definition.h"
#include "ToolStatic.h"									// for CToolStatic

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Radar.h"
//#include "ObjTool.h"
#include "ToolBarDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRadarControl::CRadarControl()
{
//	m_pMiniToolBarDlg = NULL;
	m_bOnRadar = FALSE;
	m_radar = NULL;
	m_static = NULL;

	// (2003.12.03, ��¿�) Radar Chart���� ������ Data View ������ �����Ѵ�.
	//		(ó�� �����ÿ� Zoom Full Mode�� �����Ǵ� ���������� SetDataViewInfo�� ȣ����� �ʾ� ���� ȣ���Ѵ�.)
	m_nDataSize			= 0;
	m_nDataStartIndex	= 0;
	m_nOnePageDataCount	= 1;

	// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
	m_pIChartCtrl = NULL;
}

CRadarControl::~CRadarControl()
{
	// 1. Radar Button Window�� �����Ѵ�.
	if( m_radar)
	{
		delete m_radar;
		m_radar = NULL;
	}
	// 2. Radar Button Window�� �����Ѵ�.
	if( m_static)
	{
		delete m_static;
		m_static = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.03
//
//	����	:	Zoom Full Mode ���ο� ������ Data View ������ ���޹޾� Radar Chart�� �ݿ��Ѵ�.
//
//	����	:	(2003.12.03) Zoom Full Mode�� ó���� ���Ѵ�.
//					(Radar�� ��� Zoom Full Mode���� Data�� ���������� �ݿ��Ѵ�.)
///////////////////////////////////////////////////////////////////////////////
void CRadarControl::SetDataViewInfo( int nDataSize, int nDataStartIndex, int nOnePageDataCount, BOOL bWholeView)
{
	// 1. ���� ��ȿ���� ���� Ȯ���Ѵ�.
//	if( !m_pMiniToolBarDlg) return;

	// (2003.12.03, ��¿�) Radar Chart���� ������ Data View ������ �����Ѵ�.
	//		(ó�� �����ÿ� Zoom Full Mode�� �����Ǵ� ���������� SetDataViewInfo�� ȣ����� �ʾ� ���� ȣ���Ѵ�.)
	m_nDataSize			= nDataSize;
	m_nDataStartIndex	= nDataStartIndex;
	m_nOnePageDataCount	= nOnePageDataCount;

	// 2. Radar Chart���� Data View Info�� �����Ѵ�. (�̶� Data Size�� 0�� �� �ִ�.)
	if( m_radar) m_radar->SetDataViewInfo( nDataSize, nDataStartIndex, nOnePageDataCount, bWholeView);
}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.02
//
//	�м�	:	Radar Button�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
//void CRadarControl::CreateRadarButton(CFont* pFont, CMiniToolBarDlg* MiniToolBarDlg, CRect controlItemRect, CObjTool* objTool, CString filePath)
//{
//	// 1. ���� ���� ToolBar Object Pointer�� ��ȿ���� Ȯ���ϰ� �����Ѵ�.
//	if( !MiniToolBarDlg) return;
//	m_pMiniToolBarDlg = MiniToolBarDlg;
//
//	// 2. Data Index�� �����ֱ� ���� Font�� �����Ѵ�.
//	m_pFont = pFont;
//
//	// 3. Radar Chart�� �����ϴ� Button�� �����Ѵ�.
////	MiniToolBarDlg->CreateButtonItem( &m_radarButton, objTool->GetName(), controlItemRect, IDC_RADARBUTTON, objTool->GetType());
//	m_radarButton.Create( objTool->GetName(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, controlItemRect, MiniToolBarDlg, IDC_RADARBUTTON);
//	MiniToolBarDlg->m_allControlIdList.AddTail( IDC_RADARBUTTON);
//	MiniToolBarDlg->m_allControlRectList.AddTail( controlItemRect);
//	m_radarButton.SetButtonStyle( BS_OWNERDRAW);
//	m_radarButton.SizeToContent();
//	m_radarButton.SetToolOption( objTool->GetType());
//	m_radarButton.SetToolTipText( objTool->GetName());
//	m_radarButton.SetMiniToolBarDlg( MiniToolBarDlg);
//
//}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.02
//
//	�м�	:	Radar Button�� Click�� ���� Radar Chart�� �����ϰų� �����Ѵ�.
//
//	����	:	(2003.12.03) Zoom Full Mode������ Radar�� ���� ������ �� �ֵ��� �Ѵ�.
///////////////////////////////////////////////////////////////////////////////
//void CRadarControl::RadarButtonDown()
//{
//	// 1. ���� ToolBar Object Pointer�� Ȯ���Ѵ�.
//
//	if( !m_pMiniToolBarDlg) return;
//
//	// 2. Button�� Click�� �ٷ� Focus�� ���ش�. (��?)
//	m_radarButton.SendMessage(WM_KILLFOCUS);
//
//	// 3. Radar�� �̹� ������ ��� ������ ó���Ѵ�.
//	if( m_bOnRadar)
//	{
//		// 3.1 ���� Radar Button�� Pop�Ѵ�. (�� �ΰ��� State�� �����ϴ���..?)
//		m_radarButton.SetState( FALSE);
//		m_radarButton.SetButtonState( FALSE);
//		// 3.2 Radar Button Window�� �����Ѵ�.
//		if( m_radar)
//		{
//			m_radar->DestroyWindow();
//			delete m_radar;
//			m_radar = NULL;
//		}
//		// 3.3 Radar Button Window�� �����Ѵ�.
//		if( m_static)
//		{
//			m_static->DestroyWindow();
//			delete m_static;
//			m_static = NULL;
//		}
//	}
//	// 4. Radar Chart�� ���� �����ϴ� ��츦 ó���Ѵ�.
//	else
//	{
//		// 4.1 ���� Radar Button�� Push�Ѵ�. (�� �ΰ��� State�� �����ϴ���..?)
//		m_radarButton.SetState( TRUE);
//		m_radarButton.SetButtonState( TRUE);
//
//		// 4.2 ToolBar�� Client ���� ũ�⸦ Ȯ���Ѵ�.
//		//		(Radar Chart�� Radar Status�� ��ġ�� ũ�� ���⿡ �̿��Ѵ�.)
//		CRect rctToolClient;
//		m_pMiniToolBarDlg->GetClientRect( &rctToolClient);	
//
//		// 4.3 Radar Flat Button�� �����Ѵ�.
//		// 4.3.1 Radar Flat Button�� ��ġ�� �����Ѵ�.
//		CRect rctRadarItem( rctToolClient);
//		rctRadarItem.top	= rctToolClient.top	+ 18;
//		rctRadarItem.bottom	= rctRadarItem.top + _TOOLRADARCONTROL_RADAR_HEIGHT;
//		rctRadarItem.right -= 120;
//		// 4.3.2 Radar Flat Button�� �����Ѵ�.
//		m_radar = new CRadar();
//		m_radar->Create( NULL, "radar", WS_CHILD | WS_VISIBLE | BS_FLAT | BS_OWNERDRAW, rctRadarItem, m_pMiniToolBarDlg, IDC_RADAR);
//		// 4.3.3 Radar Chart�� Graph�� �׸� �� �ֵ��� Packet ������ �����Ѵ�.
//		CString data = "����";
//		// (2003.12.04, ��¿�) ���� ������ ������ ���� ToolBar Dialog�� Pointer�� �Բ� �����Ѵ�.
//		m_radar->SetData(m_pMiniToolBarDlg->GetPacketList(), data, m_pMiniToolBarDlg->GetOcxWnd());
//
//		// 4.4 Radar Status Static�� �����Ѵ�.
//		// 4.4.1 Radar Status Static�� ��ġ�� �����Ѵ�.
//		rctRadarItem.left	= rctRadarItem.right;
//		rctRadarItem.right	= rctToolClient.right;
//		rctRadarItem.DeflateRect( 0, 4);
//		// 4.4.2 Radar Status Static�� �����Ѵ�.
//		m_static = new CStatic();
//		m_static->Create( _T( ""), WS_CHILD | WS_VISIBLE | SS_CENTER, rctRadarItem, ( CWnd *)m_pMiniToolBarDlg);
//		// 4.4.3 Radar Status Static�� �����Ѵ�.
//		m_static->SetFont( m_pFont);
//
//		// 4.5 Radar Flat Button���� Status Static�� �����Ѵ�.
//		m_radar->SetRadarStatus( m_static);
//
//		// (2003.12.03, ��¿�) Radar Chart���� ������ Data View ������ �����Ѵ�.
//		//		(Radar Chart�� ��� Zoom Full Mode�� ������ �������� �ʴ´�.)
//		m_radar->SetDataViewInfo( m_nDataSize, m_nDataStartIndex, m_nOnePageDataCount, pMainOcxWnd->IsWholeView());
//	}
//
//	m_bOnRadar = !m_bOnRadar;
//}

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2004.03.11
//
//	����	:	ToolBar�� ũ�Ⱑ ����Ǵ� ���, Radar�� ��ġ�� ��������Ѵ�.
///////////////////////////////////////////////////////////////////////////////
void CRadarControl::OnToolBarPositionChanged( CRect p_rctToolRect)
{
	// 1. ���� Radar�� �������ΰ� Ȯ���Ѵ�.
//	if( !m_bOnRadar) return;

	// ����(04/11/04) ����.
	// 2. Radar Input Button�� ��ġ�� �����Ѵ�.
	CRect rctRadarItem (0, 0, p_rctToolRect.Width(), p_rctToolRect.Height());

	rctRadarItem.top	= rctRadarItem.top	+ TB_BUTTON_HEIGHT;
	rctRadarItem.bottom	= rctRadarItem.top + TB_RADAR_HEIGHT; 
	rctRadarItem.right -= TB_RADAR_STATIC_WIDTH;
	m_radar->MoveWindow( rctRadarItem);
	// 4.4 Radar Output Static�� ��ġ�� �����Ѵ�.
	rctRadarItem.left	= rctRadarItem.right;
	rctRadarItem.right	= p_rctToolRect.Width();
	rctRadarItem.DeflateRect( 0, 4);
	m_static->MoveWindow( rctRadarItem);
}



/*-----------------------------------------------------------------------------------------
 - Function    :  Create
 - Created at  :  2004-11-04   10:58
 - Author      :  ������
 - Description :  CCtrlFactory���� �� �Լ��� ȣ���Ͽ� RadarChartControl�� �����Ѵ�. 
				  (������� �ҽ� 90% ī��. ^^; )
 -----------------------------------------------------------------------------------------*/
bool CRadarControl::Create(CWnd* pParent, UINT nID)
{
	CRect rectToolClient;
	pParent->GetClientRect(&rectToolClient);
	
	// 1. Radar Chart ����.
	CRect rectRadarItem(rectToolClient);
	rectRadarItem.top = rectRadarItem.bottom - TB_RADAR_HEIGHT;
	rectRadarItem.right -= TB_RADAR_STATIC_WIDTH;

	m_radar = new CRadar( m_pIChartCtrl);
	m_radar->Create( NULL, "radar", WS_CHILD|WS_VISIBLE|BS_FLAT|BS_OWNERDRAW, rectRadarItem, pParent, nID);
	CString name = _MTEXT( C0_CLOSE);

	CToolBarDlg* pToolBarDlg = (CToolBarDlg*)pParent;
	m_radar->SetData(pToolBarDlg->GetPacketList(), pToolBarDlg->GetPacketListManager(), pToolBarDlg->GetXScaleManager(), name);

	// 2. Radar Static ����.
	rectRadarItem.left = rectRadarItem.right;
	rectRadarItem.right = rectToolClient.right;
	rectRadarItem.DeflateRect(0, 4);

	m_static = new CToolStatic();
	m_static->Create(_T(""), WS_CHILD|WS_VISIBLE|SS_CENTER, rectRadarItem, pParent);
	m_static->SetFont(((CToolBarDlg*)pParent)->GetResourceFont());

	// Radar Chart���� Status Static�� �����Ѵ�.
	m_radar->SetRadarStatus(m_static);

	m_radar->SetDataViewInfo(m_nDataSize, m_nDataStartIndex, m_nOnePageDataCount, ((CToolBarDlg*)pParent)->IsWholeView());
	
	return true;
}


