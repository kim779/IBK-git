// DaiPacketGroup.cpp: implementation of the CDaiPacketGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DaiPacketGroup.h"

#include "DaiCodeGroup.h"		// for CDaiCodeGroup

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDaiPacketGroup::CDaiPacketGroup( const char *p_szRealPacketName)
{
	// (2004.11.29, ��¿�) �������� DRDS Advised Info�� Packet ���� �����Ѵ�.
	m_strRealPacketName = p_szRealPacketName;
}

CDaiPacketGroup::~CDaiPacketGroup()
{
	// (2004.11.29, ��¿�) ��ϵ� ��� DRDS Advised Info�� �����Ѵ�.
	RemoveAllDrdsAdvisedInfo();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) DRDS Advised Info�� �����ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. 1���� DRDS Advised Info Data�� �߰��ϴ� Interface�� �����Ѵ�.
BOOL CDaiPacketGroup::AddDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealFieldName, const char *p_szRealKeyCode)
{
	// 1. Real Key Code Group�� ��ϵǾ� ���� �ʴ� ��쿡�� ���� �����Ѵ�.
	POSITION posDaiCodeGroup = FindDaiCodeGroup( p_szRealKeyCode);
	if( !posDaiCodeGroup)
	{
		AddTail( new CDaiCodeGroup( p_szRealKeyCode));
		posDaiCodeGroup = GetTailPosition();
	}

	// 2. �˻��� Real Key Code Group�� ���ο� DRDS Advised Info�� �����Ͽ� ����Ѵ�.
	return GetAt( posDaiCodeGroup)->AddDrdsAdvisedInfo( p_pIPacket, p_szRealFieldName);
}

// RQ�� DRDS���� : �������� - ojtaso (20070111)
BOOL CDaiPacketGroup::AddDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealFieldName, const char *p_szRealKeyCode)
{
	// 1. Real Key Code Group�� ��ϵǾ� ���� �ʴ� ��쿡�� ���� �����Ѵ�.
	POSITION posDaiCodeGroup = FindDaiCodeGroup(p_szRealKeyCode);
	if( !posDaiCodeGroup)
	{
		AddTail( new CDaiCodeGroup( p_szRealKeyCode));
		posDaiCodeGroup = GetTailPosition();
	}

	// 2. �˻��� Real Key Code Group�� ���ο� DRDS Advised Info�� �����Ͽ� ����Ѵ�.
	return GetAt( posDaiCodeGroup)->AddDrdsAdvisedInfo(lpszRQ, p_pIPacket, p_szRealFieldName);
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) ������ DRDS Advised Info�� �����ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. �ϰ� �����ϴ� Interface�� �����Ѵ�.
void CDaiPacketGroup::RemoveAllDrdsAdvisedInfo( void)
{
	while( !IsEmpty()) delete RemoveHead();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) ������ DRDS Advised Info�� �˻��ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. Drds Advised Info ���� �����޾� �˻��ϴ� Inteface�� �����Ѵ�.
POSITION CDaiPacketGroup::FindDaiCodeGroup( const char *p_szRealKeyCode)
{
	// (2006/4/20 - Seung-Won, Bae) RTrim for space.  After Advise with space, DRDS Send WM_DRDS_PACKET_END without space
	CString strAdviseKeyCode, strRealKeyCode( p_szRealKeyCode);
	strRealKeyCode.TrimRight( ' ');

	POSITION posPrevDaiCodeGroup;
	POSITION posDaiCodeGroup = GetHeadPosition();
	while( posDaiCodeGroup)
	{
		posPrevDaiCodeGroup = posDaiCodeGroup;
		strAdviseKeyCode = GetNext( posDaiCodeGroup)->m_strRealKeyCode;
		strAdviseKeyCode.TrimRight( ' ');
		if( strAdviseKeyCode == strRealKeyCode) return posPrevDaiCodeGroup;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// (2004.12.01, ��¿�) Real Packet�� ���ſ��θ� �����ϴ� Interface�� �����Ѵ�.
//		(�� Packet�� ���Žø� Ȯ���Ͽ�, Drawing�� ���� ó���� �ϱ� �������̴�.)
//////////////////////////////////////////////////////////////////////

// 1. Real Packet�� ���ſ��θ� �����ϴ� Interface�� �����Ѵ�.
//		(�̶�, �� Packet�� ���ſ��θ� Ȯ���Ͽ� Return�Ѵ�.)
BOOL CDaiPacketGroup::SetPacketReceivedFlag( const char *p_szRealKeyCode)
{
	// 1. Real Key Code Group�� ��ϵǾ� ���� �ʴ� ��쿡�� ���� �����Ѵ�.
	POSITION posDaiCodeGroup = FindDaiCodeGroup( p_szRealKeyCode);
	if( !posDaiCodeGroup) return FALSE;

	// 2. �˻��� Real Key Code Group�� ���ο� DRDS Advised Info�� �����Ͽ� ����Ѵ�.
	GetAt( posDaiCodeGroup)->m_bPacketReceived = TRUE;
	return TRUE;
}

// 2. ��� Real Packet�� ���ŵǾ������� Ȯ���ϴ� Interface�� �����Ѵ�.
BOOL CDaiPacketGroup::HasOneMoreReceiveForEachPacket( void)
{
	POSITION posDaiCodeGroup = GetHeadPosition();
	while( posDaiCodeGroup) if( !GetNext( posDaiCodeGroup)->m_bPacketReceived) return FALSE;
	return TRUE;
}

// 3. ��� Real Packet�� ���� ���� Flag�� �ʱ�ȭ ó���ϴ� Interface�� �����Ѵ�.
void CDaiPacketGroup::ResetPacketReceiveFlag( void)
{
	POSITION posDaiCodeGroup = GetHeadPosition();
	while( posDaiCodeGroup) GetNext( posDaiCodeGroup)->m_bPacketReceived = FALSE;
}
