// KeyboardScrollAddInImp.cpp: implementation of the CKeyboardScrollAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "KeyboardScrollAddInImp.h"

#include "../Include_AddIn/I000000/_IChartOCX.h"				// for IChartOCX

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyboardScrollAddInImp::CKeyboardScrollAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
}

CKeyboardScrollAddInImp::~CKeyboardScrollAddInImp()
{

}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CKeyboardScrollAddInImp::m_strAddInItemName = "KBD_SCROLL";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CKeyboardScrollAddInImp)
	ONADDINEVENT( OnKeyDown)
	ONADDINEVENT( OnKeyUp)
END_ADDIN_EVENT_MAP()


//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////
BOOL CKeyboardScrollAddInImp::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// (2004.11.09, ��¿�) Keyboard�� ���� ZoomIn/Out �� Scrolling�� ó���Ѵ�.
	// 1. Handling�� ���� ���� KeyDown�� ó���Ѵ�.
	if( nChar != VK_LEFT && nChar != VK_RIGHT && nChar != VK_UP && nChar != VK_DOWN) return FALSE;
	// 2. ���� Shift Key�� Ȯ���Ѵ�.
	int nWithShift = 0;
	if( GetKeyState( VK_SHIFT) & 0x8000) nWithShift = 1;	// Shift Key Down Checking
	// 3. Key Code�� ���� Scroll/Zoom�� ó���Ѵ�.
	// (2005.01.20, ��¿�) VK_LEFT/VK_RIGHT�� ScrollBar�� VK_UP/VK_DOWN�� SlideBar�� �ִ��� Ȯ���Ͽ� ó���Ѵ�.
	if( VK_LEFT == nChar || VK_RIGHT == nChar)
	{
		// 3.1 �·� 1 �̵��Ѵ�. (Shift Key ������ 5�̵��Ѵ�.)
		if( VK_LEFT == nChar)
		{
			if( !m_pIChartOCX->ScrollTo( ( 1 + nWithShift * ( 5 - 1)) * ( -1), TRUE)) return FALSE;
		}
		// 3.2 ��� 1 �̵��Ѵ�. (Shift Key ������ 5�̵��Ѵ�.)
		if( VK_RIGHT == nChar)
		{
			if( !m_pIChartOCX->ScrollTo( ( 1 + nWithShift * ( 5 - 1)) * ( +1), TRUE)) return FALSE;
		}
	}
	if( VK_UP == nChar || VK_DOWN == nChar)
	{
		// 3.3 ȭ���� ������ 1 ���δ�. (Shift Key ������ 10�� ���δ�.)
		if( VK_UP == nChar)
		{
			if( !m_pIChartOCX->ZoomTo( ( 1 + nWithShift * ( 10 - 1)) * ( -1), TRUE)) return FALSE;
		}
		// 3.3 ȭ���� ������ 1 ���δ�. (Shift Key ������ 10�� ���δ�.)
		if( VK_DOWN == nChar)
		{
			if( !m_pIChartOCX->ZoomTo( ( 1 + nWithShift * ( 10 - 1)) * ( +1), TRUE)) return FALSE;
		}
		// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
		int nStartIndex = 0, nEndIndex = 0;
		m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex);
		m_pIChartOCX->OnUserZoomChanged( nEndIndex - nStartIndex + 1);
	}

	return TRUE;
}

BOOL CKeyboardScrollAddInImp::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// (2005.01.13, ��¿�) ��ü���� Mode�� ��� ��ȯ�ϴ� Key UI�� �����Ѵ�.
	// 1. Page Up / Down Key Input�� �����ϰ�� ��� �����Ų��.
	if( nChar != VK_PRIOR && nChar != VK_NEXT) return FALSE;
	// (2005.01.20, ��¿�) VK_PRIOR/VK_NEXT�� SlideBar�� �ִ��� Ȯ���Ͽ� ó���Ѵ�.
	if( !m_pIChartOCX->HasToolItem( CToolOptionInfo::T_DISPLAY_SLIDERBAR)) return FALSE;
	// 2. ������ ��ü���� ���ο� ���� ����ó���Ѵ�.
	//		VK_PRIOR	: ��ü���� ����
	//		VK_NEXT		: ��ü���� ����
	if( !m_pIChartOCX) return FALSE;
	if(			m_pIChartOCX->CheckFullView()	&& nChar == VK_NEXT)	m_pIChartOCX->SetFullView( FALSE);
	else if(	!m_pIChartOCX->CheckFullView()	&& nChar == VK_PRIOR)	m_pIChartOCX->SetFullView( TRUE);
	else return FALSE;

	return TRUE;
}
