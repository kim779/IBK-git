// DaiCodeGroup.cpp: implementation of the CDaiCodeGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DaiCodeGroup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDaiCodeGroup::CDaiCodeGroup( const char *p_szRealKeyCode)
{
	// (2004.11.29, ��¿�) �������� DRDS Advised Info�� Real Key Code ���� �����Ѵ�.
	m_strRealKeyCode = p_szRealKeyCode;

	// (2004.12.01, ��¿�) Real Packet�� ���ſ��θ� �����ϴ� Interface�� �����Ѵ�.
	//		(�� Packet�� ���Žø� Ȯ���Ͽ�, Drawing�� ���� ó���� �ϱ� �������̴�.)
	m_bPacketReceived = FALSE;
}

CDaiCodeGroup::~CDaiCodeGroup()
{
	// (2004.11.29, ��¿�) ��ϵ� ��� DRDS Advised Info�� �����Ѵ�.
	RemoveAllDrdsAdvisedInfo();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) DRDS Advised Info�� �����ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. 1���� DRDS Advised Info Data�� �߰��ϴ� Interface�� �����Ѵ�.
BOOL CDaiCodeGroup::AddDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealFieldName)
{
	// 0. �̹� ��ϵǾ� �ִ��� Ȯ���Ѵ�.
	if( FindDrdsAdvisedInfo( p_pIPacket, p_szRealFieldName)) return FALSE;

	// 1. ���ο� DRDS Advised Info�� �����Ͽ� ����Ѵ�.
	AddTail( new CDrdsAdvisedInfo( p_pIPacket, p_szRealFieldName));

	return TRUE;
}

// RQ�� DRDS���� : �������� - ojtaso (20070111)
BOOL CDaiCodeGroup::AddDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealFieldName)
{
	// 0. �̹� ��ϵǾ� �ִ��� Ȯ���Ѵ�.
	if( FindDrdsAdvisedInfo(lpszRQ, p_pIPacket, p_szRealFieldName)) return FALSE;

	// 1. ���ο� DRDS Advised Info�� �����Ͽ� ����Ѵ�.
	AddTail( new CDrdsAdvisedInfo(lpszRQ, p_pIPacket, p_szRealFieldName));

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) ������ DRDS Advised Info�� �����ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. �ϰ� �����ϴ� Interface�� �����Ѵ�.
void CDaiCodeGroup::RemoveAllDrdsAdvisedInfo( void)
{
	while( !IsEmpty()) delete RemoveHead();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) ������ DRDS Advised Info�� �˻��ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. Drds Advised Info ���� �����޾� �˻��ϴ� Inteface�� �����Ѵ�.
POSITION CDaiCodeGroup::FindDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealFieldName)
{
	POSITION posPrevDAI;
	POSITION posDAI = GetHeadPosition();
	while( posDAI)
	{
		posPrevDAI = posDAI;
		CDrdsAdvisedInfo *pDAI = GetNext( posDAI);
		if( pDAI->m_pIPacket == p_pIPacket && pDAI->m_strRealFieldName == p_szRealFieldName)
			return posPrevDAI;
	}
	return NULL;
}

// RQ�� DRDS���� : �������� - ojtaso (20070111)
POSITION CDaiCodeGroup::FindDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealFieldName)
{
	POSITION posPrevDAI;
	POSITION posDAI = GetHeadPosition();
	while( posDAI)
	{
		posPrevDAI = posDAI;
		CDrdsAdvisedInfo *pDAI = GetNext( posDAI);
		if(pDAI && pDAI->m_lpszRQ && !::lstrcmp(pDAI->m_lpszRQ, lpszRQ) &&
			pDAI->m_pIPacket == p_pIPacket && pDAI->m_strRealFieldName == p_szRealFieldName)
			return posPrevDAI;
	}
	return NULL;
}

