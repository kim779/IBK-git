// RealSettingInfoList.h: interface for the CRealSettingInfoList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REALSETTINGINFOLIST_H__3B37BD54_A9BA_4125_95E6_1C46C6103BB2__INCLUDED_)
#define AFX_REALSETTINGINFOLIST_H__3B37BD54_A9BA_4125_95E6_1C46C6103BB2__INCLUDED_

// (2004.11.29, ��¿�) Real Setting Info�� �����ϴ� Data Object List Class�̴�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>		// for CTypedPtrList

// (2004.11.29, ��¿�) Real Setting Info List Base Class�� �����Ѵ�.
class CRsiPacketGroup;
typedef CTypedPtrList< CObList, CRsiPacketGroup *> CRealSettingInfoListBase;

// (2004.11.29, ��¿�) Real Setting Info�� �����ϴ� Data Object List Class�� �����Ѵ�.
class CRealSettingInfoList : public CRealSettingInfoListBase
{
public:
	CRealSettingInfoList();
	virtual ~CRealSettingInfoList();

// (2004.11.29, ��¿�) Real Setting Info�� �����ϴ� Interface�� �����Ѵ�.
public:
	// 1. XFM�� Script���� �����Ǵ� Real ���� �Ӽ� String Data�� �����ϴ� Interface�� �����Ѵ�.
	BOOL	ResetRealSettingInfo( const char *p_szRealSettingData);
	// 2. 1���� Real ���� Data�� �߰��ϴ� Interface�� �����Ѵ�.
	//		Ư��, p_pstrShareName�� p_pstrSharedData�� �־����� ��쿡�� ������ Method(AddRealItem)�� ���Ͽ� �����Ǵ� �������
	//			p_szRealSettingItem���� Local Share Name�� ���� �ٷ� Real Key Code�� �־�����. 
	//		������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�Ѵ�.
	//		(ResetRealSettingInfo()�� AddRealItem()�� ������ �� �ֵ��� �Ͽ� CString�� Pointer�� ó���Ѵ�.)
	// (2006/4/28 - Seung-Won, Bae) with Separator
	BOOL	AddRealSettingInfo( const char *p_szRealSettingItem, CString *p_pstrShareName = NULL, CString *p_pstrSharedData = NULL, char p_cSeparator = '.');
	// 3. 1���� Real ���� Data�� �߰��ϴ� Interface�� �����Ѵ�.
	//		����, p_strShareName�� �־����� ����, ������ Method(AddRealItem)�� ���Ͽ� �����Ǵ� ����̸�,
	//			p_szRealSettingItem���� Local Share Name�� ���� �ٷ� Real Key Code�� �־�����. 
	//		������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�Ѵ�.
	//		(ResetRealSettingInfo()�� AddRealItem()�� ������ �� �ֵ��� �Ͽ� CString�� Pointer�� ó���Ѵ�.)
	BOOL	AddRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szShareName, CString *p_pstrShareName);

	// RQ�� Real���� : �������� - ojtaso (20070111)
	BOOL	AddRealSettingInfo(LPCTSTR lpszRQ, const char *p_szRealSettingItem, CString *p_pstrShareName, CString *p_pstrSharedData, char p_cSeparator);
	BOOL	AddRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szShareName, CString *p_pstrShareName);

// (2004.11.29, ��¿�) ������ Real Setting Info�� �����ϴ� Interface�� �����Ѵ�.
public:
	// 1. �ϰ� �����ϴ� Interface�� �����Ѵ�.
	void	RemoveAllRealSettingInfo( void);
	// 2. ������ Method(RemoveRealItem)�� ���Ͽ� �����Ǵ� 1���� Real Setting Info�� �����ϴ� Interface�� �����Ѵ�.
	//		Ư��, Local Share Name�� ���� �ٷ� Real Key Code�� �־���,
	//			������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�ؾ� �Ѵ�.
	//		����� ���̻� �ش� ���� Key�� ������ Real�� ���� ��쿡�� �������� ��ȯ�ȴ�.
	//		(AddRealSettingInfo()�� Interface�� �����ϵ��� �Ͽ� CString�� Pointer�� ó���Ѵ�.)
	// (2006/4/28 - Seung-Won, Bae) with Separator
	BOOL	RemoveRealSettingInfo( const char *p_szRealSettingItem, CString *p_pstrShareName = NULL, char p_cSeparator = '.');
	// 3. Real Setting Info ���� �����޾� Real Setting Info�� �����ϴ� Interface�� �����Ѵ�.
	//		����, p_strShareName�� �־����� ����, ������ Method(AddRealItem)�� ���Ͽ� �����Ǵ� ����̸�,
	//			Local Share Name�� ���� �ٷ� Real Key Code�� �־�����. 
	//		������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�Ѵ�.
	//		(AddRealSettingInfo()�� Interface�� �����ϵ��� �Ͽ� CString�� Pointer�� ó���Ѵ�.)
	BOOL	RemoveRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szShareName, CString *p_pstrShareName);

	// RQ�� Real���� : �������� - ojtaso (20070111)
	BOOL	RemoveRealSettingInfo(LPCTSTR lpszRQ, const char *p_szRealSettingItem, CString *p_pstrShareName = NULL, char p_cSeparator = '.');
	BOOL	RemoveRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szShareName, CString *p_pstrShareName);

// (2004.11.29, ��¿�) ������ Real Setting Info�� �˻��ϴ� Interface�� �����Ѵ�.
public:
	// 1. Real Setting Info ���� �����޾� �˻��ϴ� Inteface�� �����Ѵ�.
	POSITION	FindRsiPacketGroup( const char *p_szRealPacketName);
	// 2. Local Share Name���� �����޾� �ش� ���������� ��ϵ� Real Setting Info�� �ִ��� �˻��Ѵ�.
	POSITION	FindShareName( const char *p_szShareName);
};

#endif // !defined(AFX_REALSETTINGINFOLIST_H__3B37BD54_A9BA_4125_95E6_1C46C6103BB2__INCLUDED_)
