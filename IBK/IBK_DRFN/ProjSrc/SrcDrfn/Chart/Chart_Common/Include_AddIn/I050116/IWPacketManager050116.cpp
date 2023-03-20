// IWPacketManager050116.cpp: implementation of the CIWPacketManager050116 class.
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
#include "IWPacketManager050116.h"

#include "Packetbase.h"				// for CPacket
#include "PacketList.h"				// for CPacketList
#include "IWPacket050116.h"			// for CIWPacket050116

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIWPacketManager050116::CIWPacketManager050116( CPacketList *p_pPacketList)
{
	// (2004.10.11, ��¿�) Packet List Object�� Pointer�� �����Ѵ�.
	m_pPacketList = p_pPacketList;
	if( !m_pPacketList)
	{
		AfxMessageBox( "CIWPacketManager050116 Null Wrapping Error!");
		return;
	}
}

CIWPacketManager050116::~CIWPacketManager050116()
{

}

//////////////////////////////////////////////////////////////////////
// [04/10/11] Packet Object�� Interface�� ���ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04/10/11] Packet Name���� Packet Object�� ���ϴ� Interface�� �����Ѵ�.
IPacket *CIWPacketManager050116::GetPacket( const char *p_szPacketName)
{
	if( !m_pPacketList) return NULL;
	CPacket *pPacket = m_pPacketList->GetBaseData( p_szPacketName);
	if( !pPacket) return NULL;
	IPacket *pIPacket = new CIWPacket050116( pPacket);
	pIPacket->AddRef();
	return pIPacket;
}

// [04/11/30] Packet Object�� Memory Address���� Packet Object�� ���ϴ� Interface�� �����Ѵ�.
IPacket *CIWPacketManager050116::GetPacketInterface( unsigned long p_ulPacketAddress)
{
	if( !m_pPacketList) return NULL;
	if( !p_ulPacketAddress) return NULL;
	IPacket *pIPacket = new CIWPacket050116( ( CPacket *)p_ulPacketAddress);
	pIPacket->AddRef();
	return pIPacket;
}

//////////////////////////////////////////////////////////////////////
// [04/10/13] ���� Message�� �˻��ϴ� Interface�� �����Ѵ�. (�ֱٿ� ���ŵ� TR�� ���ѵȴ�.)
//////////////////////////////////////////////////////////////////////
BOOL CIWPacketManager050116::GetHelpMessageItem( const char *p_szMsgKey, CString &p_szMessage)
{
	if( !m_pPacketList) return FALSE;
	return m_pPacketList->GetHelpMessageItem( p_szMsgKey, p_szMessage);
}
