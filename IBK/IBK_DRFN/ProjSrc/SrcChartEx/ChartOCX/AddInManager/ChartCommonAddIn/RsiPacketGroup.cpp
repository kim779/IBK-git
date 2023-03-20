// RsiPacketGroup.cpp: implementation of the CRsiPacketGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RsiPacketGroup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRsiPacketGroup::CRsiPacketGroup( const char *p_szRealPacketName)
{
	// (2004.11.29, ��¿�) �������� Real Setting Info�� Packet ���� �����Ѵ�.
	m_strRealPacketName = p_szRealPacketName;
}

CRsiPacketGroup::~CRsiPacketGroup()
{
	// (2004.11.29, ��¿�) ��ϵ� ��� Real Setting Info�� �����Ѵ�.
	RemoveAllRealSettingInfo();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) Real Setting Info�� �����ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////
// 1. 1���� Real ���� Data�� �߰��ϴ� Interface�� �����Ѵ�.
BOOL CRsiPacketGroup::AddRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName)
{
	// 0. �̹� ��ϵǾ� �ִ��� Ȯ���Ѵ�.
	if( FindRealSettingInfo( p_szChartPacketName, p_szRealFieldName, p_szShareName)) return FALSE;

	// 1. ���ο� Real Setting Info�� �����Ͽ� ����Ѵ�.
	AddTail( new CRealSettingInfo( p_szChartPacketName, p_szRealFieldName, p_szShareName));

	return TRUE;
}

// RQ�� Real���� : �������� - ojtaso (20070111)
BOOL CRsiPacketGroup::AddRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName)
{
	// 0. �̹� ��ϵǾ� �ִ��� Ȯ���Ѵ�.
	if( FindRealSettingInfo(lpszRQ, p_szChartPacketName, p_szRealFieldName, p_szShareName)) return FALSE;

	// 1. ���ο� Real Setting Info�� �����Ͽ� ����Ѵ�.
	AddTail( new CRealSettingInfo(lpszRQ, p_szChartPacketName, p_szRealFieldName, p_szShareName));

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) ������ Real Setting Info�� �����ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. �ϰ� �����ϴ� Interface�� �����Ѵ�.
void CRsiPacketGroup::RemoveAllRealSettingInfo( void)
{
	while( !IsEmpty()) delete RemoveHead();
}

// 2. Real Setting Info ���� �����޾� Real Setting Info�� �����ϴ� Interface�� �����Ѵ�.
BOOL CRsiPacketGroup::RemoveRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName)
{
	POSITION posRealSettingInfo = FindRealSettingInfo( p_szChartPacketName, p_szRealFieldName, p_szShareName);
	if( !posRealSettingInfo) return FALSE;

	delete GetAt( posRealSettingInfo);
	RemoveAt( posRealSettingInfo);
	return TRUE;
}

// RQ�� Real���� : �������� - ojtaso (20070111)
BOOL CRsiPacketGroup::RemoveRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName)
{
	POSITION posRealSettingInfo = FindRealSettingInfo(lpszRQ, p_szChartPacketName, p_szRealFieldName, p_szShareName);
	if( !posRealSettingInfo) return FALSE;

	delete GetAt( posRealSettingInfo);
	RemoveAt( posRealSettingInfo);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) ������ Real Setting Info�� �˻��ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. Real Setting Info ���� �����޾� �˻��ϴ� Inteface�� �����Ѵ�.
POSITION CRsiPacketGroup::FindRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName)
{
	POSITION posPrevRSI;
	POSITION posRSI = GetHeadPosition();
	while( posRSI)
	{
		posPrevRSI = posRSI;
		CRealSettingInfo *pRSI = GetNext( posRSI);
		if( pRSI->m_strChartPacketName		== p_szChartPacketName
			&& pRSI->m_strRealFieldName		== p_szRealFieldName
			&& pRSI->m_strShareName	== p_szShareName)
				return posPrevRSI;
	}
	return NULL;
}

// RQ�� Real���� : �������� - ojtaso (20070111)
POSITION CRsiPacketGroup::FindRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName)
{
	POSITION posPrevRSI;
	POSITION posRSI = GetHeadPosition();
	while( posRSI)
	{
		posPrevRSI = posRSI;
		CRealSettingInfo *pRSI = GetNext( posRSI);
		if( pRSI->m_strChartPacketName		== p_szChartPacketName
			&& pRSI->m_strRealFieldName		== p_szRealFieldName
//			&& pRSI->m_strShareName	== p_szShareName
			&& pRSI->m_strRQ	== lpszRQ)
				return posPrevRSI;
	}
	return NULL;
}

// 2. Local Share Name���� �����޾� �ش� ���������� ��ϵ� Real Setting Info�� �ִ��� �˻��Ѵ�.
POSITION CRsiPacketGroup::FindShareName( const char *p_szShareName)
{
	POSITION posPrevRSI;
	POSITION posRSI = GetHeadPosition();
	while( posRSI)
	{
		posPrevRSI = posRSI;
		CRealSettingInfo *pRSI = GetNext( posRSI);
		if( pRSI->m_strShareName	== p_szShareName) return posPrevRSI;
	}
	return NULL;
}

