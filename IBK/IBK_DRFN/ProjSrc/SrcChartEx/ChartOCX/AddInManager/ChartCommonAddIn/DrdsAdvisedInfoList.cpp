// DrdsAdvisedInfoList.cpp: implementation of the CDrdsAdvisedInfoList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrdsAdvisedInfoList.h"	// for CDrdsAdvisedInfoList

#include "DaiPacketGroup.h"		// for CDaiPacketGroup

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrdsAdvisedInfoList::CDrdsAdvisedInfoList()
{

}

CDrdsAdvisedInfoList::~CDrdsAdvisedInfoList()
{
	// (2004.11.29, ��¿�) ��ϵ� ��� DRDS Advised Info�� �����Ѵ�.
	RemoveAllDrdsAdvisedInfo();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) DRDS Advised Info�� �����ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. DRDS Advised Info Packet Group�� �߰��ϴ� Interface�� �����Ѵ�.
BOOL CDrdsAdvisedInfoList::AddDaiPacketGroup( const char *p_szRealPacketName)
{
	// 0. �̹� ��ϵ� ��츦 �����Ѵ�.
	if( FindDaiPacketGroup( p_szRealPacketName)) return FALSE;

	// 1. ���ο� DRDS Advised Info Packet Group�� ��Ͻ�Ų��.
	AddTail( new CDaiPacketGroup( p_szRealPacketName));
	return TRUE;
}

// 2. DRDS Advised Info�� �߰��ϴ� Interface�� �����Ѵ�.
BOOL CDrdsAdvisedInfoList::AddDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szRealKeyCode)
{
	// 0. ���� DRDS Advised Info Packet Group�� ��ϵ��� ���� ��츦 �����Ѵ�.
	POSITION posDaiPacketGroup = FindDaiPacketGroup( p_szRealPacketName);
	if( !posDaiPacketGroup) return FALSE;

	// 1. DRDS Advised Info Packet Group�� ���Ͽ� DRDS Advised Info�� ����Ѵ�.
	return GetAt( posDaiPacketGroup)->AddDrdsAdvisedInfo( p_pIPacket, p_szRealFieldName, p_szRealKeyCode);
}

// RQ�� DRDS���� : �������� - ojtaso (20070111)
BOOL CDrdsAdvisedInfoList::AddDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szRealKeyCode)
{
	if(!lpszRQ)
		return AddDrdsAdvisedInfo(p_pIPacket, p_szRealPacketName, p_szRealFieldName, p_szRealKeyCode);

	// 0. ���� DRDS Advised Info Packet Group�� ��ϵ��� ���� ��츦 �����Ѵ�.
	POSITION posDaiPacketGroup = FindDaiPacketGroup( p_szRealPacketName);
	if( !posDaiPacketGroup) return FALSE;

	// 1. DRDS Advised Info Packet Group�� ���Ͽ� DRDS Advised Info�� ����Ѵ�.
	return GetAt( posDaiPacketGroup)->AddDrdsAdvisedInfo(lpszRQ, p_pIPacket, p_szRealFieldName, p_szRealKeyCode);
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) ������ DRDS Advised Info�� �����ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. �ϰ� �����ϴ� Interface�� �����Ѵ�.
void CDrdsAdvisedInfoList::RemoveAllDrdsAdvisedInfo( void)
{
	while( !IsEmpty())
	{
		CDaiPacketGroup *pDaiPacketGroup = RemoveHead();
		pDaiPacketGroup->RemoveAllDrdsAdvisedInfo();
		delete pDaiPacketGroup;
	}
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) ������ DRDS Advised Info�� �˻��ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. DRDS Advised Info Packet Group�� �˻��ϴ� Inteface�� �����Ѵ�.
//		(�ߺ� ����� �����ϱ� ���Ͽ� �̿�ȴ�.)
POSITION CDrdsAdvisedInfoList::FindDaiPacketGroup( const char *p_szRealPacketName)
{
	POSITION posPrevDaiPacketGroup;
	POSITION posDaiPacketGroup = GetHeadPosition();
	while( posDaiPacketGroup)
	{
		posPrevDaiPacketGroup = posDaiPacketGroup;
		CDaiPacketGroup *pDaiPacketGroup = GetNext( posDaiPacketGroup);
		if( pDaiPacketGroup->m_strRealPacketName == p_szRealPacketName) return posPrevDaiPacketGroup;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// (2004.12.01, ��¿�) Real Packet�� ���ſ��θ� �����ϴ� Interface�� �����Ѵ�.
//		(�� Packet�� ���Žø� Ȯ���Ͽ�, Drawing�� ���� ó���� �ϱ� �������̴�.)
//////////////////////////////////////////////////////////////////////

// 1. Real Packet�� ���ſ��θ� �����ϴ� Interface�� �����Ѵ�.
//		(�̶�, �� Packet�� ���ſ��θ� Ȯ���Ͽ� Return�Ѵ�.)
BOOL CDrdsAdvisedInfoList::SetPacketReceivedFlag( const char *p_szRealPacketName, const char *p_szRealKeyCode)
{
	// 1. ���� DRDS Advised Info Packet Group�� �˻��Ѵ�.
	POSITION posDaiPacketGroup = FindDaiPacketGroup( p_szRealPacketName);
	if( !posDaiPacketGroup) return FALSE;

	// 2. DRDS Advised Info Packet Group�� ���Ͽ� �ش� Real Packet�� ���� ���θ� �����Ѵ�.
	CDaiPacketGroup *pDaiPacketGroup = GetAt( posDaiPacketGroup);
	if( !pDaiPacketGroup->SetPacketReceivedFlag( p_szRealKeyCode)) return FALSE;

	return HasOneMoreReceiveForEachPacket();
}

// 2. ��� Real Packet�� ���ŵǾ������� Ȯ���ϴ� Interface�� �����Ѵ�.
BOOL CDrdsAdvisedInfoList::HasOneMoreReceiveForEachPacket( void)
{
	POSITION posDaiPacketGroup = GetHeadPosition();
	while( posDaiPacketGroup) if( !GetNext( posDaiPacketGroup)->HasOneMoreReceiveForEachPacket()) return FALSE;
	return TRUE;
}

// 3. ��� Real Packet�� ���� ���� Flag�� �ʱ�ȭ ó���ϴ� Interface�� �����Ѵ�.
void CDrdsAdvisedInfoList::ResetPacketReceiveFlag( void)
{
	POSITION posDaiPacketGroup = GetHeadPosition();
	while( posDaiPacketGroup) GetNext( posDaiPacketGroup)->ResetPacketReceiveFlag();
}
