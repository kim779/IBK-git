// IWPacketManager050531.cpp: implementation of the CIWPacketManager050531 class.
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
#include "IWPacketManager050531.h"

#include "Packetbase.h"				// for CPacket
#include "PacketList.h"				// for CPacketList
#include "IWPacket050531.h"			// for CIWPacket050531

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIWPacketManager050531::CIWPacketManager050531( CPacketList *p_pPacketList)
{
	// (2004.10.11, ��¿�) Packet List Object�� Pointer�� �����Ѵ�.
	m_pPacketList = p_pPacketList;
	if( !m_pPacketList)
	{
		AfxMessageBox( "CIWPacketManager050531 Null Wrapping Error!");
		return;
	}
}

CIWPacketManager050531::~CIWPacketManager050531()
{

}

//////////////////////////////////////////////////////////////////////
// [04/10/11] Packet Object�� Interface�� ���ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04/10/11] Packet Name���� Packet Object�� ���ϴ� Interface�� �����Ѵ�.
IPacket *CIWPacketManager050531::GetPacket( const char *p_szPacketName)
{
	if( !m_pPacketList) return NULL;
	CPacket *pPacket = m_pPacketList->GetBaseData( p_szPacketName);
	if( !pPacket) return NULL;
	IPacket *pIPacket = new CIWPacket050531( pPacket);
	pIPacket->AddRef();
	return pIPacket;
}

// (2005/9/2 - Seung-Won, Bae) Get Packet Interface List for Packet Names
BOOL CIWPacketManager050531::GetPacketList( const char *p_szPacketNames, CTypedPtrList< CPtrList, IPacket *> *p_plIPackets)
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

		pIPacket = new CIWPacket050531( pPacket);
		pIPacket->AddRef();

		p_plIPackets->AddTail( pIPacket);
	}

	return TRUE;
}

// [04/11/30] Packet Object�� Memory Address���� Packet Object�� ���ϴ� Interface�� �����Ѵ�.
IPacket *CIWPacketManager050531::GetPacketInterface( unsigned long p_ulPacketAddress)
{
	if( !m_pPacketList) return NULL;
	if( !p_ulPacketAddress) return NULL;
	IPacket *pIPacket = new CIWPacket050531( ( CPacket *)p_ulPacketAddress);
	pIPacket->AddRef();
	return pIPacket;
}

//////////////////////////////////////////////////////////////////////
// [04/10/13] ���� Message�� �˻��ϴ� Interface�� �����Ѵ�. (�ֱٿ� ���ŵ� TR�� ���ѵȴ�.)
//////////////////////////////////////////////////////////////////////
BOOL CIWPacketManager050531::GetHelpMessageItem( const char *p_szMsgKey, CString &p_szMessage)
{
	if( !m_pPacketList) return FALSE;
	return m_pPacketList->GetHelpMessageItem( p_szMsgKey, p_szMessage);
}
// [05/09/29] Get Help Message List
const CMapStringToString *CIWPacketManager050531::GetHelpMessageMap( void)
{
	if( !m_pPacketList) return NULL;
	return m_pPacketList->GetHelpMessageMap();
}


