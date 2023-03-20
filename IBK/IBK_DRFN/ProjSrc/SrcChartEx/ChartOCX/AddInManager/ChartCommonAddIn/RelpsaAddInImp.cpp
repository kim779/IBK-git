// RelpsaAddInImp.cpp: implementation of the CRelpsaAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "RelpsaAddInImp.h"

#include "LoadDrds.h"									// for DrdsData
#include "BlockBaseEnum.h"								// for ERRORVALUE
#include "../../Include_Chart/Dll_Load/IMetaTable.h"	// for _MTEXT()
#include "../Include_AddIn/AddInCommand.h"				// for EAI_DRDS_PACKET_END
#include "../Include_AddIn/I000000/_IPacket.h"			// for IPacket
#include "../Include_Addin/I000000/_IChartManager.h"	// for IChartManager
#include "../Include_Addin/I000000/_IPacketManager.h"	// for IPacketManager
#include "../Include_AddIn/I000000/_IString.h"			// for ILPCSTR

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRelpsaAddInImp::CRelpsaAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2004.10.15, ��¿�) Real�� �����Ͽ� Reset�� �ð������� Ȯ��/ó���Ѵ�.
	//		OnDrdsPacketEnd()������ ���ϴ� Real�̾����� �ĺ��� �� ���� OnDrdsData���� ��� ��Ȳ �Ǵ��� ���� ���� �����Ѵ�.
	m_bOnResetReal = FALSE;
}

CRelpsaAddInImp::~CRelpsaAddInImp()
{

}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CRelpsaAddInImp::m_strAddInItemName = "RELPSA";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CRelpsaAddInImp)
	ONADDINEVENT( OnRealData)
	ONADDINEVENT( OnRealPacketEnd)
	ONADDINEVENT( OnPacketDataMultiItem)
END_ADDIN_EVENT_MAP()

// (2004.10.14, ��¿�) �ʱ�ȭ ���� �ð�(����� �ð�)�� ������ �����Ѵ�.
void CRelpsaAddInImp::ResetRELPSA( const char *p_szResetMsg)
{
	// 0. �ʱ�ȭ�� ��츦 ó���Ѵ�.
	CString strResetMsg( p_szResetMsg);
	if( strResetMsg.IsEmpty())
	{
		// (2004.10.15, ��¿�) ���� Data �ʱ�ȭ�� ���� Real ���� ȯ���� Clear�ϴ� Routine�� �����Ѵ�.
		ClearRELPSA();
		return;
	}

	// 1. Format�� Ȯ���Ѵ�. (HHMMSS:00..00)
	// 1.0 ':'�� ��ġ�� Ȯ���Ѵ�.
	if( strResetMsg.Find( ':') != 6) return;
	// 1.1 ���� Code�� ������ Ȯ���Ѵ�.
	if( strResetMsg.GetLength() <= 7) return;

	// 2. ���ؽð��� ���� �ڵ带 �����Ѵ�.
	m_dResetTime = atof( strResetMsg.Left( 6));
	m_strResetItemCode = strResetMsg.Mid( 7);

	// (2004.10.15, ��¿�) Real�� �����Ͽ� Reset�� �ð������� Ȯ��/ó���Ѵ�.
	//		OnDrdsPacketEnd()������ ���ϴ� Real�̾����� �ĺ��� �� ���� OnDrdsData���� ��� ��Ȳ �Ǵ��� ���� ���� �����Ѵ�.
	m_bOnResetReal = FALSE;
}

// (2004.10.15, ��¿�) ���� Data �ʱ�ȭ�� ���� Real ���� ȯ���� Clear�ϴ� Routine�� �����Ѵ�.
void CRelpsaAddInImp::ClearRELPSA( void)
{
	m_dResetTime = 0;
	m_strResetItemCode.Empty();
	m_bOnResetReal = FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
//////////////////////////////////////////////////////////////////////

// RQ�� Packet : �������� - ojtaso (20070111)
void CRelpsaAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	// (2004.10.13, ��¿�) �켱 �����Ѱ� ���� Message�� ������ �������� Ȯ���Ѵ�.
	// (2004.10.22, ��¿�) ����/���Ѱ� ǥ�� ���� ���� ���Ϻ� �ʱ�ȭ �����
	//			��� ���� ��Ʈ�� �ִ� ������ �ʱ�ȭ TR�� ���Ž����� �ش��Ѵ�.
	//		�� ��ɵ��� ���� ������Ʈ�� ����/������ ����/������ ����Ʈ�� �����ϸ鼭,
	//			��/���Ѱ��� ǥ���ϰų� ���� ���� ǥ�ø� Reset ��Ű�� ����� ó���ϴ� ���̴�.
	//		Ư�� �� ��ɵ��� ��Ʈ�� 1���� Graph���� ������ �� �ִ� �������
	//			(����� Data ������ AddIn �� 1���� ���ѵ� ������� �����̴�.)
	//			���� Packet�� �̿��ϴ� ������Ʈ���� �����Ǵ� ������� �����Ѵ�.
	//		*. ���� �ڼ��� ������ �� Class Header ����
	if( !( BOOL)strstr( p_szPacketNames, CString( _MTEXT( C0_CLOSE)) + ";") || !p_bWithReset) return;

	// (2004.10.14, ��¿�) ���� ���Ϻ� �ʱ�ȭ �ð��� Ȯ���Ѵ�.
	ILPCSTR szHelpMsg;
	if( m_pIDefaultPacketManager) m_pIDefaultPacketManager->GetHelpMessageItem( "RELPSA", szHelpMsg);
	ResetRELPSA( szHelpMsg);
}

// (2007/2/8 - Seung-Won, Bae) On Real Data
void CRelpsaAddInImp::OnRealData( const char *p_szPacketName, double p_dData, const char *p_szRQ)
{
	// 1. ó���� ���������� Ȯ���Ѵ�.
	// 1.1 RELPSA�� ���ؽð��� �ִ��� Ȯ���Ѵ�.
	if( m_dResetTime == 0 || m_strResetItemCode.IsEmpty()) return;

	// (2006/11/20 - Seung-Won, Bae) Do not release packet interface.
	//		It is released in OffDrdsAdvise() Time
	//	pIPacket->Release();
	if( strcmp( p_szPacketName, _MTEXT( C0_DATE_TIME))) return;

	// 2. Real�� �ð��� Reset�� �ð��� ����ߴ��� Ȯ���Ѵ�.
	m_bOnResetReal = ( m_dResetTime <= p_dData);
}

// (2006/10/4 - Seung-Won, Bae) On Real Packet End
void CRelpsaAddInImp::OnRealPacketEnd( const char *p_szRQ)
{
	// 0. ���� ���� Data�� Reset ���Ѿ� �� ��Ȳ���� Ȯ���Ѵ�.
	if( !m_bOnResetReal) return;

	// 1. ��/��/�� Data�� ���� Data�� �����Ѵ�.
	// 1.0 ���� ������ Ȯ���Ѵ�.
	if( !m_pIDefaultPacketManager) return;
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_CLOSE));
	if( !pIPacket) return;
	int nDataIndex = pIPacket->GetDataCount() - 1;	// 0 Base
	if( nDataIndex < 0)
	{
		pIPacket->Release();
		return;
	}
	double dPrice;
	if( !pIPacket->GetData( nDataIndex, dPrice)) return;
	pIPacket->Release();
	// 1.1 �ð��� �����Ѵ�.
	pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_OPEN));
	if( pIPacket)
	{
		pIPacket->SetData( nDataIndex, dPrice);
		pIPacket->Release();
	}
	// 1.2 ���� �����Ѵ�.
	pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_HIGH));
	if( pIPacket)
	{
		pIPacket->SetData( nDataIndex, dPrice);
		pIPacket->Release();
	}
	// 1.1 ������ �����Ѵ�.
	pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_LOW));
	if( pIPacket)
	{
		pIPacket->SetData( nDataIndex, dPrice);
		pIPacket->Release();
	}

	// 2. ó���� ������, �� �̻��� �ݺ��۾��� ������ RELPSA ���� Data�� Clear��Ų��.
	ClearRELPSA();
}
