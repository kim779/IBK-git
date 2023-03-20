// LocalShareInfoList.cpp: implementation of the CLocalShareInfoList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTBChart.h"
#include "LocalShareInfoList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLocalShareInfoList::CLocalShareInfoList()
{

}

CLocalShareInfoList::~CLocalShareInfoList()
{
	// (2004.11.29, ��¿�) ��ϵ� ��� Local Share Info�� �����Ѵ�.
	RemoveAllLocalShareInfo();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) Local Share Info�� �����ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. Local Share Info�� �߰��ϴ� Interface�� �����Ѵ�.
//		Ư��, ��� ���� Local Share Setting Flag�� Ȯ���� �� �ֵ��� Object Pointer�� Return�Ѵ�.
// (2004.12.01, ��¿�) �ʱ� ��Ͻ� �������� ������ �� �ֵ��� �Ѵ�.
CLocalShareInfo *CLocalShareInfoList::AddLocalShareInfo( const char *p_szLocalShareName, int p_nLocalShareSettingCode, const char *p_szLocalSharedData)
{
	// 0. �̹� ��ϵ� Local Share Info���� Ȯ���Ѵ�.
	if( FindLocalShareInfo( p_szLocalShareName)) return NULL;

	// 1. ���ο� Local Share Info�� �����Ͽ� ����Ѵ�.
	CLocalShareInfo *pLocalShareInfo = new CLocalShareInfo( p_szLocalShareName, p_nLocalShareSettingCode, p_szLocalSharedData);
	AddTail( pLocalShareInfo);
	return pLocalShareInfo;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) ������ Local Share Info�� �����ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. �ϰ� �����ϴ� Interface�� �����Ѵ�.
void CLocalShareInfoList::RemoveAllLocalShareInfo( void)
{
	while( !IsEmpty()) delete RemoveHead();
}

// 2. �����Ǵ� Local Share Info�� �����ϴ� Interface�� �����Ѵ�.
BOOL CLocalShareInfoList::RemoveLocalShareInfo( const char *p_szLocalShareName)
{
	POSITION posLocalShareInfo = FindLocalShareInfo( p_szLocalShareName);
	if( !posLocalShareInfo) return FALSE;
	delete GetAt( posLocalShareInfo);
	RemoveAt( posLocalShareInfo);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) ������ Local Share Info�� �˻��ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. Local Share Info�� �˻��ϴ� Inteface�� �����Ѵ�.
POSITION CLocalShareInfoList::FindLocalShareInfo( const char *p_szLocalShareName)
{
	POSITION posPrevLSI;
	POSITION posLSI = GetHeadPosition();
	while( posLSI)
	{
		posPrevLSI = posLSI;
		CLocalShareInfo *pLSI = GetNext( posLSI);
		if( pLSI->m_strLocalShareName == p_szLocalShareName) return posPrevLSI;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) Local Shared Data�� ���� �� ��ȸ�ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. Local Shared Data�� �����ϴ� Interface�� �����Ѵ�.
BOOL CLocalShareInfoList::SetLocalSharedData( const char *p_szLocalShareName, const char *p_szLocalSharedData)
{
	// 0. ��ϵ��� ���� Local Share Info���� Ȯ���Ѵ�.
	POSITION posLSI = FindLocalShareInfo( p_szLocalShareName);
	if( !posLSI) return FALSE;

	// 1. Local Shared Data�� �����Ѵ�.
	GetAt( posLSI)->m_strLocalSharedData = p_szLocalSharedData;
	return TRUE;
}

// 2. Local Shared Data�� ��ȸ�ϴ� Interface�� �����Ѵ�.
const char *CLocalShareInfoList::GetLocalSharedData( const char *p_szLocalShareName)
{
	// 0. ��ϵ��� ���� Local Share Info���� Ȯ���Ѵ�.
	POSITION posLSI = FindLocalShareInfo( p_szLocalShareName);
	if( !posLSI) return NULL;

	// 1. Local Shared Data�� ��ȸ�Ѵ�.
	CLocalShareInfo *pLocalShareInfo = GetAt( posLSI);
	return pLocalShareInfo->m_strLocalSharedData;
}
