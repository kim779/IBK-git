// RealSettingInfoList.cpp: implementation of the CRealSettingInfoList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RealSettingInfoList.h"

#include "RsiPacketGroup.h"			// for CRsiPacketGroup

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// (2004.11.25, ��¿�) Direct Advise/Unadvise�� ���� ���� �������� �����Ѵ�.
#define _CHART_INNER_LSHARE_NAME	"$$$ChartInnerLShareName$$$ - "

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRealSettingInfoList::CRealSettingInfoList()
{

}

CRealSettingInfoList::~CRealSettingInfoList()
{
	// (2004.11.29, ��¿�) ��ϵ� ��� Real Setting Info�� �����Ѵ�.
	RemoveAllRealSettingInfo();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) Real Setting Info�� �����ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. XFM�� Script���� �����Ǵ� Real ���� �Ӽ� String Data�� �����ϴ� Interface�� �����Ѵ�.
BOOL CRealSettingInfoList::ResetRealSettingInfo( const char *p_szRealSettingData)
{
	// 0. ���� ������ �ʱ�ȭ�Ѵ�.
	RemoveAllRealSettingInfo();

	// 1. Real Setting String�� ���� ��츦 ó���Ѵ�. 
	if( !p_szRealSettingData || !*p_szRealSettingData) return TRUE;

	// 2. ���� Return ���� �ʱ�ȭ�Ѵ�.
	BOOL bReturn = TRUE;

	// 3. Real Setting Item�� �ϳ��� �и��Ͽ� ó���Ѵ�.
	CString strRealSetting;
	char *szRealSetting = strRealSetting.GetBufferSetLength( strlen( p_szRealSettingData));
	strcpy( szRealSetting, p_szRealSettingData);
	while( szRealSetting && *szRealSetting)
	{
		// 3.1 ���� Real Setting Item�� Ȯ���Ͽ� �и��Ѵ�.
		char *szNextRealSetting = strchr( szRealSetting, ';');
		if( szNextRealSetting) *szNextRealSetting++ = '\0';

		// 3.2 1���� Real Setting Item�� ����Ѵ�.
		bReturn = bReturn && AddRealSettingInfo( szRealSetting);

		// 3.3 ���� Real Setting Item�� �����ϵ��� �����Ѵ�.
		szRealSetting = szNextRealSetting;
	}

	// 4. ���� Return ���� ��ȯ�Ѵ�.
	return bReturn;
}

// 2. 1���� Real ���� Data�� �߰��ϴ� Interface�� �����Ѵ�.
//		Ư��, p_pstrShareName�� p_pstrSharedData�� �־����� ��쿡�� ������ Method(AddRealItem)�� ���Ͽ� �����Ǵ� �������
//			p_szRealSettingItem���� Local Share Name�� ���� �ٷ� Real Key Code�� �־�����. 
//		������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�Ѵ�.
//		(ResetRealSettingInfo()�� AddRealItem()�� ������ �� �ֵ��� �Ͽ� CString�� Pointer�� ó���Ѵ�.)
BOOL CRealSettingInfoList::AddRealSettingInfo( const char *p_szRealSettingItem, CString *p_pstrShareName, CString *p_pstrSharedData, char p_cSeparator)
{
	// 1. Real Setting Item Data�� Ȯ���Ѵ�.
	if( !p_szRealSettingItem || !*p_szRealSettingItem) return FALSE;

	// 2. Real Setting Item�� �����Ѵ�.
	CString strRealSettingItem;
	char *szRealSettingItem = strRealSettingItem.GetBufferSetLength( strlen( p_szRealSettingItem));
	strcpy( szRealSettingItem, p_szRealSettingItem);
	// 3.5 Item�� �����ڰ� ���� �� ������, �̸� �����Ѵ�.
	char *szNextRealSetting = strchr( szRealSettingItem, ';');
	if( szNextRealSetting) *szNextRealSetting++ = '\0';

	// 3. Chart Packet Name�� ���Ѵ�.
	char *szChartPacketName = szRealSettingItem;

	// 4. Real Packet Name�� ���Ͽ� �и��Ѵ�.
	char *szRealPacketName = strchr( szChartPacketName, p_cSeparator);
	if( !szRealPacketName) return FALSE;
	*szRealPacketName++ = '\0';

	// 5. Real Field Name�� ���Ͽ� �и��Ѵ�.
	char *szRealFieldName = strchr( szRealPacketName, p_cSeparator);
	if( !szRealFieldName) return FALSE;
	*szRealFieldName++ = '\0';

	// 6. Local Share Name�� ���Ͽ� �и��Ѵ�.
	const char *szShareName = strchr( szRealFieldName, p_cSeparator);
	if( !szShareName) return FALSE;
	*( char *)szShareName++ = '\0';

	// 7. ��ȯ�� Local Shared Data�� �̸� �����Ѵ�.
	if( p_pstrSharedData) *p_pstrSharedData = szShareName;

	// 8. �ش� Real Setting Info�� ����Ѵ�.
	return AddRealSettingInfo( szChartPacketName, szRealPacketName, szRealFieldName, szShareName, p_pstrShareName);
}

// 3. 1���� Real ���� Data�� �߰��ϴ� Interface�� �����Ѵ�.
//		����, p_strShareName�� �־����� ����, ������ Method(AddRealItem)�� ���Ͽ� �����Ǵ� ����̸�,
//			p_szRealSettingItem���� Local Share Name�� ���� �ٷ� Real Key Code�� �־�����. 
//		������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�Ѵ�.
//		(ResetRealSettingInfo()�� AddRealItem()�� ������ �� �ֵ��� �Ͽ� CString�� Pointer�� ó���Ѵ�.)
BOOL CRealSettingInfoList::AddRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szShareName, CString *p_pstrShareName)
{
	// 7. p_pstrShareName�� �����Ǵ� ���,
	//			�� Real Setting Item�� ������ Method(AddRealItem)�� ���Ͽ� �����Ǵ� �������
	//			Local Share Name�� ���� �ٷ� Real Key Code�� �־�����. 
	//		�̶����� ������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�Ѵ�.
	if( p_pstrShareName)
	{
		*p_pstrShareName = CString( _CHART_INNER_LSHARE_NAME) + p_szShareName;
		p_szShareName = *p_pstrShareName;
	}

	// 8. �ش� Real Setting Info Packet Group�� �˻��Ѵ�. (���� ��� �����Ͽ� ����Ѵ�.)
	POSITION posRsiPacketGroup = FindRsiPacketGroup( p_szRealPacketName);
	if( !posRsiPacketGroup)
	{
		AddTail( new CRsiPacketGroup( p_szRealPacketName));
		posRsiPacketGroup = GetTailPosition();
	}

	// 9. �˻��� Real Setting Info Packet Group�� Real Setting Info�� ��Ͻ�Ų��.
	// (2006/4/3 - Seung-Won, Bae) Use Reference for Numega Memory Leak Checking.
	CRsiPacketGroup *&pRsiPacketGroup = GetAt( posRsiPacketGroup);
	return pRsiPacketGroup->AddRealSettingInfo( p_szChartPacketName, p_szRealFieldName, p_szShareName);
}

// RQ�� Real���� : �������� - ojtaso (20070111)
BOOL CRealSettingInfoList::AddRealSettingInfo(LPCTSTR lpszRQ, const char *p_szRealSettingItem, CString *p_pstrShareName, CString *p_pstrSharedData, char p_cSeparator)
{
	// 1. Real Setting Item Data�� Ȯ���Ѵ�.
	if( !p_szRealSettingItem || !*p_szRealSettingItem) return FALSE;

	// 2. Real Setting Item�� �����Ѵ�.
	CString strRealSettingItem;
	char *szRealSettingItem = strRealSettingItem.GetBufferSetLength( strlen( p_szRealSettingItem));
	strcpy( szRealSettingItem, p_szRealSettingItem);
	// 3.5 Item�� �����ڰ� ���� �� ������, �̸� �����Ѵ�.
	char *szNextRealSetting = strchr( szRealSettingItem, ';');
	if( szNextRealSetting) *szNextRealSetting++ = '\0';

	// 3. Chart Packet Name�� ���Ѵ�.
	char *szChartPacketName = szRealSettingItem;

	// 4. Real Packet Name�� ���Ͽ� �и��Ѵ�.
	char *szRealPacketName = strchr( szChartPacketName, p_cSeparator);
	if( !szRealPacketName) return FALSE;
	*szRealPacketName++ = '\0';

	// 5. Real Field Name�� ���Ͽ� �и��Ѵ�.
	char *szRealFieldName = strchr( szRealPacketName, p_cSeparator);
	if( !szRealFieldName) return FALSE;
	*szRealFieldName++ = '\0';

	// 6. Local Share Name�� ���Ͽ� �и��Ѵ�.
	const char *szShareName = strchr( szRealFieldName, p_cSeparator);
	if( !szShareName) return FALSE;
	*( char *)szShareName++ = '\0';

	// 7. ��ȯ�� Local Shared Data�� �̸� �����Ѵ�.
	if( p_pstrSharedData) *p_pstrSharedData = szShareName;

	// 8. �ش� Real Setting Info�� ����Ѵ�.
	return AddRealSettingInfo(lpszRQ, szChartPacketName, szRealPacketName, szRealFieldName, szShareName, p_pstrShareName);
}

// RQ�� Real���� : �������� - ojtaso (20070111)
BOOL CRealSettingInfoList::AddRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szShareName, CString *p_pstrShareName)
{
	// 7. p_pstrShareName�� �����Ǵ� ���,
	//			�� Real Setting Item�� ������ Method(AddRealItem)�� ���Ͽ� �����Ǵ� �������
	//			Local Share Name�� ���� �ٷ� Real Key Code�� �־�����. 
	//		�̶����� ������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�Ѵ�.
	if( p_pstrShareName)
	{
		*p_pstrShareName = CString( _CHART_INNER_LSHARE_NAME) + p_szShareName;
		p_szShareName = *p_pstrShareName;
	}

	// 8. �ش� Real Setting Info Packet Group�� �˻��Ѵ�. (���� ��� �����Ͽ� ����Ѵ�.)
	POSITION posRsiPacketGroup = FindRsiPacketGroup( p_szRealPacketName);
	if( !posRsiPacketGroup)
	{
		AddTail( new CRsiPacketGroup( p_szRealPacketName));
		posRsiPacketGroup = GetTailPosition();
	}

	// 9. �˻��� Real Setting Info Packet Group�� Real Setting Info�� ��Ͻ�Ų��.
	// (2006/4/3 - Seung-Won, Bae) Use Reference for Numega Memory Leak Checking.
	CRsiPacketGroup *&pRsiPacketGroup = GetAt( posRsiPacketGroup);
	return pRsiPacketGroup->AddRealSettingInfo(lpszRQ, p_szChartPacketName, p_szRealFieldName, p_szShareName);
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) ������ Real Setting Info�� �����ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. �ϰ� �����ϴ� Interface�� �����Ѵ�.
void CRealSettingInfoList::RemoveAllRealSettingInfo( void)
{
	while( !IsEmpty())
	{
		CRsiPacketGroup *pRsiPacketGroup = RemoveHead();
		pRsiPacketGroup->RemoveAllRealSettingInfo();
		delete pRsiPacketGroup;
	}
}

// 2. ������ Method(RemoveRealItem)�� ���Ͽ� �����Ǵ� 1���� Real Setting Info�� �����ϴ� Interface�� �����Ѵ�.
//		Ư��, Local Share Name�� ���� �ٷ� Real Key Code�� �־���,
//			������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�ؾ� �Ѵ�.
//		����� ���̻� �ش� ���� Key�� ������ Real�� ���� ��쿡�� �������� ��ȯ�ȴ�.
//		(AddRealSettingInfo()�� Interface�� �����ϵ��� �Ͽ� CString�� Pointer�� ó���Ѵ�.)
BOOL CRealSettingInfoList::RemoveRealSettingInfo( const char *p_szRealSettingItem, CString *p_pstrShareName, char p_cSeparator)
{
	// 0. ���� ������ ��츦 ����Ͽ� �������� Clear�Ѵ�.
	if( !p_pstrShareName) return FALSE;
	p_pstrShareName->Empty();

	// 0. Real Setting Item Data�� Ȯ���Ѵ�.
	if( !p_szRealSettingItem || !*p_szRealSettingItem) return FALSE;

	// 1. ���� �ش� Real Setting Item�� �����Ѵ�.
	// 1.1 Real Setting Item�� �����Ѵ�.
	CString strRealSettingItem;
	char *szRealSettingItem = strRealSettingItem.GetBufferSetLength( strlen( p_szRealSettingItem));
	strcpy( szRealSettingItem, p_szRealSettingItem);
	// 1.2 Item�� �����ڰ� ���� �� ������, �̸� �����Ѵ�.
	char *szNextRealSetting = strchr( szRealSettingItem, ';');
	if( szNextRealSetting) *szNextRealSetting++ = '\0';
	// 1.3 Chart Packet Name�� ���Ѵ�.
	char *szChartPacketName = szRealSettingItem;
	// 1.4 Real Packet Name�� ���Ͽ� �и��Ѵ�.
	char *szRealPacketName = strchr( szChartPacketName, p_cSeparator);
	if( !szRealPacketName) return FALSE;
	*szRealPacketName++ = '\0';
	// 1.5 Real Field Name�� ���Ͽ� �и��Ѵ�.
	char *szRealFieldName = strchr( szRealPacketName, p_cSeparator);
	if( !szRealFieldName) return FALSE;
	*szRealFieldName++ = '\0';
	// 1.6 Local Share Name�� ���Ͽ� �и��Ѵ�.
	const char *szShareName = strchr( szRealFieldName, p_cSeparator);
	if( !szShareName) return FALSE;
	*( char *)szShareName++ = '\0';
	// 1.7 �� Real Setting Item�� ������ Method(AddRealItem)�� ���Ͽ� �����Ǵ� �������
	//			Local Share Name�� ���� �ٷ� Real Key Code�� �־�����. 
	//		�̶����� ������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�Ѵ�.
	// 1.8 Real Setting Info�� �����Ѵ�.
	if( !RemoveRealSettingInfo( szChartPacketName, szRealPacketName, szRealFieldName, szShareName, p_pstrShareName)) return FALSE;

	// 2. ���� ���� Key�� ���� Real Setting Info�� �ִ��� Ȯ���Ѵ�.
	//		Local Share Info������ ���� ���� �������� �����ϱ� �����̴�.
	//		�������� ���ƾ� �ϴ� ��� p_strShareName�� Empty() ó���Ѵ�.
	if( FindShareName( *p_pstrShareName)) p_pstrShareName->Empty();

	// 3. ó�� �Ϸ�� ��� TRUE�� Return�Ѵ�.
	return TRUE;
}

// 3. Real Setting Info ���� �����޾� Real Setting Info�� �����ϴ� Interface�� �����Ѵ�.
//		����, p_strShareName�� �־����� ����, ������ Method(AddRealItem)�� ���Ͽ� �����Ǵ� ����̸�,
//			Local Share Name�� ���� �ٷ� Real Key Code�� �־�����. 
//		������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�Ѵ�.
//		(AddRealSettingInfo()�� Interface�� �����ϵ��� �Ͽ� CString�� Pointer�� ó���Ѵ�.)
BOOL CRealSettingInfoList::RemoveRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szRealKeyCode, CString *p_pstrShareName)
{
	// 0. ������ Local Share Name�� �����ѵ� �̿��Ѵ�.
	*p_pstrShareName = CString( _CHART_INNER_LSHARE_NAME) + p_szRealKeyCode;

	// 1. Real Setting Info Packet Group�� �˻��Ͽ� Real Setting Info�� �����Ѵ�.
	POSITION posRsiPacketGroup = FindRsiPacketGroup( p_szRealPacketName);
	if( !posRsiPacketGroup) return FALSE;
	CRsiPacketGroup *pRsiPacketGroup = GetAt( posRsiPacketGroup);
	if( !pRsiPacketGroup->RemoveRealSettingInfo( p_szChartPacketName, p_szRealFieldName, *p_pstrShareName)) return FALSE;

	// 2. ���Ű� ������ ���, �ش� Real Setting Info Packet Group�� ������� Ȯ���ϰ� �̵� �����Ѵ�.
	if( pRsiPacketGroup->IsEmpty())
	{
		RemoveAt( posRsiPacketGroup);
		delete pRsiPacketGroup;
	}

	return TRUE;
}

// RQ�� Real���� : �������� - ojtaso (20070111)
BOOL CRealSettingInfoList::RemoveRealSettingInfo(LPCTSTR lpszRQ, const char *p_szRealSettingItem, CString *p_pstrShareName, char p_cSeparator)
{
	// 0. ���� ������ ��츦 ����Ͽ� �������� Clear�Ѵ�.
	if( !p_pstrShareName) return FALSE;
	p_pstrShareName->Empty();

	// 0. Real Setting Item Data�� Ȯ���Ѵ�.
	if( !p_szRealSettingItem || !*p_szRealSettingItem) return FALSE;

	// 1. ���� �ش� Real Setting Item�� �����Ѵ�.
	// 1.1 Real Setting Item�� �����Ѵ�.
	CString strRealSettingItem;
	char *szRealSettingItem = strRealSettingItem.GetBufferSetLength( strlen( p_szRealSettingItem));
	strcpy( szRealSettingItem, p_szRealSettingItem);
	// 1.2 Item�� �����ڰ� ���� �� ������, �̸� �����Ѵ�.
	char *szNextRealSetting = strchr( szRealSettingItem, ';');
	if( szNextRealSetting) *szNextRealSetting++ = '\0';
	// 1.3 Chart Packet Name�� ���Ѵ�.
	char *szChartPacketName = szRealSettingItem;
	// 1.4 Real Packet Name�� ���Ͽ� �и��Ѵ�.
	char *szRealPacketName = strchr( szChartPacketName, p_cSeparator);
	if( !szRealPacketName) return FALSE;
	*szRealPacketName++ = '\0';
	// 1.5 Real Field Name�� ���Ͽ� �и��Ѵ�.
	char *szRealFieldName = strchr( szRealPacketName, p_cSeparator);
	if( !szRealFieldName) return FALSE;
	*szRealFieldName++ = '\0';
	// 1.6 Local Share Name�� ���Ͽ� �и��Ѵ�.
	const char *szShareName = strchr( szRealFieldName, p_cSeparator);
	if( !szShareName) return FALSE;
	*( char *)szShareName++ = '\0';
	// 1.7 �� Real Setting Item�� ������ Method(AddRealItem)�� ���Ͽ� �����Ǵ� �������
	//			Local Share Name�� ���� �ٷ� Real Key Code�� �־�����. 
	//		�̶����� ������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�Ѵ�.
	// 1.8 Real Setting Info�� �����Ѵ�.
	if( !RemoveRealSettingInfo(lpszRQ, szChartPacketName, szRealPacketName, szRealFieldName, szShareName, p_pstrShareName)) return FALSE;

	// 2. ���� ���� Key�� ���� Real Setting Info�� �ִ��� Ȯ���Ѵ�.
	//		Local Share Info������ ���� ���� �������� �����ϱ� �����̴�.
	//		�������� ���ƾ� �ϴ� ��� p_strShareName�� Empty() ó���Ѵ�.
	if( FindShareName(*p_pstrShareName)) p_pstrShareName->Empty();

	// 3. ó�� �Ϸ�� ��� TRUE�� Return�Ѵ�.
	return TRUE;
}

// RQ�� Real���� : �������� - ojtaso (20070111)
BOOL CRealSettingInfoList::RemoveRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szRealKeyCode, CString *p_pstrShareName)
{
	// 0. ������ Local Share Name�� �����ѵ� �̿��Ѵ�.
	*p_pstrShareName = CString( _CHART_INNER_LSHARE_NAME) + p_szRealKeyCode;

	// 1. Real Setting Info Packet Group�� �˻��Ͽ� Real Setting Info�� �����Ѵ�.
	POSITION posRsiPacketGroup = FindRsiPacketGroup( p_szRealPacketName);
	if( !posRsiPacketGroup) return FALSE;
	CRsiPacketGroup *pRsiPacketGroup = GetAt( posRsiPacketGroup);
	if( !pRsiPacketGroup->RemoveRealSettingInfo(lpszRQ, p_szChartPacketName, p_szRealFieldName, *p_pstrShareName)) return FALSE;

	// 2. ���Ű� ������ ���, �ش� Real Setting Info Packet Group�� ������� Ȯ���ϰ� �̵� �����Ѵ�.
	if( pRsiPacketGroup->IsEmpty())
	{
		RemoveAt( posRsiPacketGroup);
		delete pRsiPacketGroup;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, ��¿�) ������ Real Setting Info�� �˻��ϴ� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// 1. Real Setting Info ���� �����޾� �˻��ϴ� Inteface�� �����Ѵ�.
POSITION CRealSettingInfoList::FindRsiPacketGroup( const char *p_szRealPacketName)
{
	POSITION posPrevRsiPacketGroup;
	POSITION posRsiPacketGroup = GetHeadPosition();
	while( posRsiPacketGroup)
	{
		posPrevRsiPacketGroup = posRsiPacketGroup;
		CRsiPacketGroup *pRsiPacketGroup = GetNext( posRsiPacketGroup);
		if( pRsiPacketGroup->m_strRealPacketName	== p_szRealPacketName) return posPrevRsiPacketGroup;
	}
	return NULL;
}

// 2. Local Share Name���� �����޾� �ش� ���������� ��ϵ� Real Setting Info�� �ִ��� �˻��Ѵ�.
POSITION CRealSettingInfoList::FindShareName( const char *p_szShareName)
{
	POSITION posPrevRsiPacketGroup;
	POSITION posRsiPacketGroup = GetHeadPosition();
	while( posRsiPacketGroup)
	{
		posPrevRsiPacketGroup = posRsiPacketGroup;
		CRsiPacketGroup *pRsiPacketGroup = GetNext( posRsiPacketGroup);
		if( pRsiPacketGroup->FindShareName( p_szShareName)) return posPrevRsiPacketGroup;
	}
	return NULL;
}

/*
// (2004.11.25, ��¿�) ������� ���� ���� Real ��û�� ��ϵǾ� �ִ��� Ȯ���ϴµ� �̿�ȴ�.
//		Ư�� �ڷ����ڰ� ���� Real�� �����ϴ��� Ȯ���� �� �ֵ��� Packet������ �˻��ϴ� ��ɵ� �����Ѵ�.
//		"�ڷ�����" Packet�� ���� Real�� ����ϸ鼭 p_bOnlyThisOne�� TRUE�� ����ϸ� ������ ���� Real�� �ݵ�� �ְ�,
//		�ٸ� Real��û�� �������� ������ Ȯ���Ͽ�, ������� ���� ���� Real�� Unadivse�� ������� �Ѵ�.
BOOL CKoscomChartCtrl::LookupRealInfo( CPacket *p_pRealPacket, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szRealKeyCode, BOOL p_bOnlyThisOne)
{
	if( m_AdviseList != NULL )
	{
		// 1. Ư�� Real ������ �˻��ϴ� ����� ó���Ѵ�.
		if( !p_bOnlyThisOne)
		{
			POSITION posAdviseList = m_AdviseList->GetHeadPosition();
			while( posAdviseList)
			{
				CRealInfo *pRealInfo = m_AdviseList->GetNext( posAdviseList);
				if( pRealInfo->LookupInfo( p_pRealPacket, p_szRealPacketName, p_szRealFieldName, p_szRealKeyCode, FALSE)) return TRUE;
			}
			return FALSE;
		}
		// 2. Ư�� Real ������ ���Ҵ��� Ȯ���ϴ� ����� ó���Ѵ�.
		// 2.1 ���� �� Ư�� ������ �ִ����� Ȯ���Ѵ�.
		if( !LookupRealInfo( p_pRealPacket, p_szRealPacketName, p_szRealFieldName, p_szRealKeyCode, FALSE)) return FALSE;
		// 2.2 �ִ� ���, �� ���� ������ �ִ��� Ȯ���Ѵ�.
		POSITION posAdviseList = m_AdviseList->GetHeadPosition();
		while( posAdviseList)
		{
			CRealInfo *pRealInfo = m_AdviseList->GetNext( posAdviseList);
			if( pRealInfo->LookupInfo( p_pRealPacket, p_szRealPacketName, p_szRealFieldName, p_szRealKeyCode, TRUE)) return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}


*/