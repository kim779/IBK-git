// AutoScrollAddInImp.cpp: implementation of the CAutoScrollAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "AutoScrollAddInImp.h"

#include "BlockBaseEnum.h"										// for ERRORVALUE
#include "../Include_AddIn/_resource.h"							// for ID_ASA_AUTOSCROLL_TIMER
#include "../Include_AddIn/I000000/_IChartOCX.h"				// for IChartOCX

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAutoScrollAddInImp::CAutoScrollAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	m_nCurrentMode = CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_STOP;
}

CAutoScrollAddInImp::~CAutoScrollAddInImp()
{
	// �Ҹ���� ���� Timer�� �Ҹ��Ѵ�.
	m_pChartCWnd->KillTimer( ID_ASA_AUTOSCROLL_TIMER);
}


// ( 2006 / 11 / 21 - Sang-Woo, Cho )  Multiple Item in DLL
CString CAutoScrollAddInImp::m_strAddInItemName = "AUTO_SCROLL";


//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CAutoScrollAddInImp)
	ONADDINEVENT( OnTimer)
	ONADDINEVENT( OnToolCommand)
END_ADDIN_EVENT_MAP()


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 21
// Return void	: 
// Param  const int p_nToolType : Button�� ��
// Param  const int p_nCmdOption : �������� ( 0 : �������� ����.	1 : ���� )
// Comments		: �ڵ� ��ũ�� ��ư�� �������� �ش�Ǵ� ������ �����Ѵ�.
//-----------------------------------------------------------------------------
void CAutoScrollAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption )
{
	// (2007/1/29 - Seung-Won, Bae) Re-Notify the State to Tool UI
	if( CToolOptionInfo::T_ALL_TOOL_TRUE_QUERY == p_nToolType)
	{
		m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT, m_nCurrentMode == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT);
		m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT, m_nCurrentMode == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT);
		return;
	}

	// 1. CmdOption�� 1�� ��쿡�� ó���Ѵ�.
	if( p_nCmdOption == 0 ) return;

	if( p_nToolType < CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT || CToolOptionInfo::T_ADDIN_AUTOSCROLL_STOP < p_nToolType)
		return;

	// 2. ���� ���� Type�� �����Ѵ�.
	m_nCurrentMode = p_nToolType;

	// 3. ���� Type�� �´� �۾��� �Ѵ�.
	switch( m_nCurrentMode)
	{
		// Stop Button�� ������ �� 
		case CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_STOP :
		{
			m_pChartCWnd->KillTimer( ID_ASA_AUTOSCROLL_TIMER);
			// (2006/11/25 - Seung-Won, Bae) Reset UI
			m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT, FALSE);
			m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT, FALSE);
			break;
		}
		
		// Left Scroll, Right Scroll Button�� ������ ��
		case CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_LEFT  :				// Run Left
		case CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_RIGHT :				// Run Right
		{
			// �ڵ� ��ũ�� ���� ���� - ojtaso (20080425)
			UINT nTime = (UINT)(1000. / p_nCmdOption);
			m_pChartCWnd->KillTimer( ID_ASA_AUTOSCROLL_TIMER);
			m_pChartCWnd->SetTimer( ID_ASA_AUTOSCROLL_TIMER, nTime, NULL );
			// (2006/11/25 - Seung-Won, Bae) Reset UI
			m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT, m_nCurrentMode == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT);
			m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT, m_nCurrentMode == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT);
			break;
		}
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 21
// Return void	: 
// Param  UINT p_uTimerID : Timer ID�� ID_ASA_AUTOSCROLL_TIMER�϶��� �����Ѵ�.
// Comments		: 
//-----------------------------------------------------------------------------
void CAutoScrollAddInImp::OnTimer( UINT p_uTimerID )
{
	// 1. Timer�� ID�� ID_ASA_AUTOSCROLL_TIMER�� ��쿡�� ó���Ѵ�.
	if( p_uTimerID != ID_ASA_AUTOSCROLL_TIMER)
		return;

	int nStartIndex, nEndIndex;

	// 2. Scroll Left�� ��쿡�� Left�� 1���� �̵��Ѵ�.
	if( m_nCurrentMode == CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_LEFT )
		m_pIChartOCX->ScrollTo( -1, TRUE );

	// 3. Scroll Right�� ��쿡�� Right�� 1���� �̵��Ѵ�.
	else if( m_nCurrentMode == CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_RIGHT )
		m_pIChartOCX->ScrollTo( 1, TRUE );

	else
		return;
	
	// 4. Scroll�� ���������� �ϰ� �� �� Timer�� ������� ���������� �Ǵ��Ѵ�.
	m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex );

	int nMaxRange;
	m_pIChartOCX->GetMaxScrollRange( nMaxRange );

	if( nStartIndex == 0 || nMaxRange <= nEndIndex )
		OnToolCommand( CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_STOP, 1);
}