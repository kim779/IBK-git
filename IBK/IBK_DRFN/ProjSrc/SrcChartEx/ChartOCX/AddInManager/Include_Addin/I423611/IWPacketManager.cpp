// IWPacketManager.cpp: implementation of the CIWPacketManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWPacketManager.h"

#include "Packetbase.h"		// for CPacket
#include "IWPacket.h"		// for CIWPacket
// PacketListManager : �������� - ojtaso (20070110)
#include "PacketListManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWPacketManager)

//////////////////////////////////////////////////////////////////////
// [04/10/11] Packet Object�� Interface�� ���ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04/10/11] Packet Name���� Packet Object�� ���ϴ� Interface�� �����Ѵ�.
IPacket *CIWPacketManager::GetPacket( const char *p_szPacketName)
{
	if( !m_pPacketList) return NULL;
	CPacket *pPacket = m_pPacketList->GetBaseData( p_szPacketName);
	if( !pPacket) return NULL;
	return CIWPacket::CreateObject( pPacket);
}

// RQ�� Packet���� : �������� - ojtaso (20070112)
IPacket *CIWPacketManager::GetPacket(LPCTSTR lpszRQ, const char *p_szPacketName)
{
	if(!m_pPacketListManager)
		return GetPacket(p_szPacketName);

	CPacketList* pPacketList = m_pPacketListManager->GetPacketList(lpszRQ);
	if( !pPacketList) return NULL;

	CPacket *pPacket = pPacketList->GetBaseData( p_szPacketName);
	if( !pPacket) return NULL;
	return CIWPacket::CreateObject(lpszRQ, pPacket);
}

// (2005/9/2 - Seung-Won, Bae) Get Packet Interface List for Packet Names
BOOL CIWPacketManager::GetPacketList( const char *p_szPacketNames, CTypedPtrList< CPtrList, IPacket *> *p_plIPackets)
{
	if( !m_pPacketList) return FALSE;

	CList<CPacket*, CPacket*> lPackets;
	if( !m_pPacketList->GetPacketList( p_szPacketNames, ";", lPackets)) return FALSE;

	CPacket *pPacket = NULL;
	IPacket *pIPacket = NULL;
	POSITION posPacket = lPackets.GetHeadPosition();
	while( posPacket)
	{
		pPacket = lPackets.GetNext( posPacket);
		p_plIPackets->AddTail( CIWPacket::CreateObject( pPacket));
	}

	return TRUE;
}

// [04/11/30] Packet Object�� Memory Address���� Packet Object�� ���ϴ� Interface�� �����Ѵ�.
IPacket *CIWPacketManager::GetPacketInterface( unsigned long p_ulPacketAddress)
{
	if( !m_pPacketList) return NULL;
	if( !p_ulPacketAddress) return NULL;
	return CIWPacket::CreateObject( ( CPacket *)p_ulPacketAddress);
}

// (2006/1/9 - Seung-Won, Bae) Get Packet Interface List from CPacket List
//					 ~ CObject * <= const CList<CPacket*, CPacket*> *
BOOL CIWPacketManager::GetIPacketListFromCPacketList( CObject *p_plCPacket, CList <IPacket *, IPacket *> *p_lIPacketList)
{
	const CList<CPacket*, CPacket*> *plCPacketList = ( const CList<CPacket*, CPacket*> *)p_plCPacket;
	if( !plCPacketList || !p_lIPacketList) return FALSE;
	POSITION pos = plCPacketList->GetHeadPosition();
	if( !pos) return FALSE;
	CPacket *pPacket = NULL;
	IPacket *pIPacket = NULL;
	while( pos)
	{
		pPacket = plCPacketList->GetNext(pos);
		p_lIPacketList->AddTail( CIWPacket::CreateObject( pPacket));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// [04/10/13] ���� Message�� �˻��ϴ� Interface�� �����Ѵ�. (�ֱٿ� ���ŵ� TR�� ���ѵȴ�.)
//////////////////////////////////////////////////////////////////////
BOOL CIWPacketManager::GetHelpMessageItem( const char *p_szMsgKey, CString &p_szMessage)
{
	if( !m_pPacketList) return FALSE;
	return m_pPacketList->GetHelpMessageItem( p_szMsgKey, p_szMessage);
}
// [05/09/29] Get Help Message List
const CMapStringToString *CIWPacketManager::GetHelpMessageMap( void)
{
	if( !m_pPacketList) return NULL;
	return m_pPacketList->GetHelpMessageMap();
}

// PacketListManager Setting : �������� - ojtaso (20070307)
void CIWPacketManager::SetPacketListManager(LPVOID lpPacketListManager)
{
	m_pPacketListManager = (CPacketListManager*)lpPacketListManager;
}

