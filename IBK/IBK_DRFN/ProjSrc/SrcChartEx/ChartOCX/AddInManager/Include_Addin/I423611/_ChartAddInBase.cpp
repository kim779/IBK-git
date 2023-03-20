// ChartAddInBase.cpp: implementation of the CChartAddInBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "_ChartAddInBase.h"

#include "../_IAddInDllBase.h"	// for IAddInDllBase
#include "_IAddInToolMgr.h"		// for IAddInToolMgr
#include "_IChartManager.h"		// for IChartManager
#include "_IPacketManager.h"	// for IPacketManager
#include "_IGraphInfoManager.h"	// for IGraphInfoManager

// 2008.02.20 by LYH >>
#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
// 2008.02.20 by LYH <<
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartAddInBase::CChartAddInBase( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase, const BOOL p_bOverExclusive)
{
	// (2006/2/10 - Seung-Won, Bae) Manage State for CWnd::FromHandle()
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	m_hOcxWnd = NULL;
	if( p_pIChartOCX) m_hOcxWnd = p_pIChartOCX->GetOcxHwnd();

	// (2006/2/12 - Seung-Won, Bae) Manage Over Exclusive Flag for Registering AddIn Event Map
	m_bOverExclusive = p_bOverExclusive;

	// (2004.10.15, ��¿�) OCX Interface�� NULL�� ��츦 ����Ͽ�, Member�� �̸� NULL �ʱ�ȭ�� ó���Ѵ�.
	m_pIPacketManager		= NULL;
	m_pIChartManager		= NULL;
	m_pIGraphInfoManager	= NULL;
	m_pChartCWnd			= NULL;
	m_pIAddInToolMgr		= NULL;
	// (2006/1/17 - Seung-Won, Bae) Manage IAddInDll for AddIn Tool Exclusive
	m_pIAddInDllBase		= p_pIAddInDllBase;

	// (2004.10.08, ��¿�) Chart OCX Interface�� �����Ͽ�, ���� Ÿ Object�� ��ȸ�� �� �ֵ��� �Ѵ�.
	m_pIChartOCX = p_pIChartOCX;
	if( !m_pIChartOCX)
	{
		AfxMessageBox( "CChartAddInBase" _AIVERS " Null Wrapping Error!");
		return;
	}
	m_pIChartOCX->AddRef();

	// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
	m_pIPacketManager		= m_pIChartOCX->GetIPacketManager();
	m_pIChartManager		= m_pIChartOCX->GetIChartManager();
	m_pIGraphInfoManager	= m_pIChartOCX->GetIGraphInfoManager();
	m_pIAddInToolMgr		= m_pIChartOCX->GetIAddInToolMgr();

	// (2004.10.25, ��¿�) Chart OCX Control Window Pointer�� �����Ͽ� Windows Interface�� �̿��� �� �ֵ��� �Ѵ�.
	m_pChartCWnd = CWnd::FromHandle( m_pIChartOCX->GetOcxHwnd());
}

CChartAddInBase::~CChartAddInBase()
{
	// (2004.10.08, ��¿�) Chart OCX Interface�� �����Ͽ�, ���� Ÿ Object�� ��ȸ�� �� �ֵ��� �Ѵ�.
	m_pIChartOCX->Release();

	// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
	if( m_pIPacketManager)
	{
		m_pIPacketManager->Release();
		m_pIPacketManager = NULL;
	}
	if( m_pIChartManager)
	{
		m_pIChartManager->Release();
		m_pIChartManager = NULL;
	}
	if( m_pIGraphInfoManager)
	{
		m_pIGraphInfoManager->Release();
		m_pIGraphInfoManager = NULL;
	}
	if( m_pIAddInToolMgr)
	{
		m_pIAddInToolMgr->Release();
		m_pIAddInToolMgr = NULL;
	}
}

// (2004.10.13, ��¿�) �⺻ Manager�� ��ϵǾ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
BOOL CChartAddInBase::HasFullManager( void)
{
	return m_pIChartOCX && m_pIPacketManager && m_pIChartManager && m_pIGraphInfoManager && m_pIAddInToolMgr;
}

// (2006/2/12 - Seung-Won, Bae) Register AddIn Event Map
int CChartAddInBase::RegisterEventMap( void)
{
	const EAddInEventType *pRegisteredEventMap = GetRegisteredEventMap();

	int nRegisteredCount = 0;
	for( int i = 0; pRegisteredEventMap[ i] != EAE_END; i++)
		if( m_pIAddInToolMgr->RegisterEventMap( pRegisteredEventMap[ i], m_pIAddInDllBase, m_bOverExclusive)) nRegisteredCount++;

	return nRegisteredCount;
}

// (2006/2/12 - Seung-Won, Bae) Unregister AddIn Event Map
int CChartAddInBase::UnregisterEventMap( void)
{
	const EAddInEventType *pRegisteredEventMap = GetRegisteredEventMap();

	int nUnregisteredCount = 0;
	for( int i = 0; pRegisteredEventMap[ i] != EAE_END; i++)
		if( m_pIAddInToolMgr->UnregisterEventMap( pRegisteredEventMap[ i], m_pIAddInDllBase, m_bOverExclusive)) nUnregisteredCount++;

	return nUnregisteredCount;
}

