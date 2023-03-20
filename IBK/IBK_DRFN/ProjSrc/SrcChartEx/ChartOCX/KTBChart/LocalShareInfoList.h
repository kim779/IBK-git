// LocalShareInfoList.h: interface for the CLocalShareInfoList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCALSHAREINFOLIST_H__4ADF05F0_B0F7_4610_B8DB_63CB55B9A91C__INCLUDED_)
#define AFX_LOCALSHAREINFOLIST_H__4ADF05F0_B0F7_4610_B8DB_63CB55B9A91C__INCLUDED_

// (2004.11.29, ��¿�) Local Share Info�� �����ϴ� Data Object List Class�̴�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>		// for CTypedPtrList

// (2004.11.29, ��¿�) Local Share Info Object Class�� �����Ѵ�.
class CLocalShareInfo : public CObject
{
public:
	CLocalShareInfo( const char *p_szLocalShareName, int p_nLocalShareSettingCode, const char *p_szLocalSharedData = NULL)
	{
		m_strLocalShareName		= p_szLocalShareName;
		m_bStart				= BOOL( p_nLocalShareSettingCode & 1);
		m_bSet					= BOOL( p_nLocalShareSettingCode & 2);
		m_bGet					= BOOL( p_nLocalShareSettingCode & 4);
		m_strLocalSharedData	= p_szLocalSharedData;
	}
public:
	CString		m_strLocalShareName;
	BOOL		m_bStart;
	BOOL		m_bSet;
	BOOL		m_bGet;
	CString		m_strLocalSharedData;
};

// (2004.11.29, ��¿�) Local Share Info List Base Class�� �����Ѵ�.
typedef CTypedPtrList< CObList, CLocalShareInfo *> CLocalShareInfoListBase;

// (2004.11.29, ��¿�) Local Share Info�� �����ϴ� Data Object List Class�� �����Ѵ�.
class CLocalShareInfoList : public CLocalShareInfoListBase
{
public:
	CLocalShareInfoList();
	virtual ~CLocalShareInfoList();

// (2004.11.29, ��¿�) Local Share Info�� �����ϴ� Interface�� �����Ѵ�.
public:
	// 1. Local Share Info�� �߰��ϴ� Interface�� �����Ѵ�.
	//		Ư��, ��� ���� Local Share Setting Flag�� Ȯ���� �� �ֵ��� Object Pointer�� Return�Ѵ�.
	// (2004.12.01, ��¿�) �ʱ� ��Ͻ� �������� ������ �� �ֵ��� �Ѵ�.
	CLocalShareInfo *	AddLocalShareInfo( const char *p_szLocalShareName, int p_nLocalShareSettingCode, const char *p_szLocalSharedData = NULL);

// (2004.11.29, ��¿�) ������ Local Share Info�� �����ϴ� Interface�� �����Ѵ�.
public:
	// 1. �ϰ� �����ϴ� Interface�� �����Ѵ�.
	void	RemoveAllLocalShareInfo( void);
	// 2. �����Ǵ� Local Share Info�� �����ϴ� Interface�� �����Ѵ�.
	BOOL	RemoveLocalShareInfo( const char *p_szLocalShareName);

// (2004.11.29, ��¿�) ������ Local Share Info�� �˻��ϴ� Interface�� �����Ѵ�.
public:
	// 1. Local Share Info�� �˻��ϴ� Inteface�� �����Ѵ�.
	POSITION	FindLocalShareInfo( const char *p_szLocalShareName);

// (2004.11.29, ��¿�) Local Shared Data�� ���� �� ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	// 1. Local Shared Data�� �����ϴ� Interface�� �����Ѵ�.
	BOOL			SetLocalSharedData( const char *p_szLocalShareName, const char *p_szLocalSharedData);
	// 2. Local Shared Data�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	const char *	GetLocalSharedData( const char *p_szLocalShareName);
};

#endif // !defined(AFX_LOCALSHAREINFOLIST_H__4ADF05F0_B0F7_4610_B8DB_63CB55B9A91C__INCLUDED_)
