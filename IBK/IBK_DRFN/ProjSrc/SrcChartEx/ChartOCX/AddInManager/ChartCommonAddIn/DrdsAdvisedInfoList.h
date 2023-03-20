// DrdsAdvisedInfoList.h: interface for the CDrdsAdvisedInfoList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRDSADVISEDINFOLIST_H__CBA26066_3E6F_4590_BE50_3FBBCE8F1BF6__INCLUDED_)
#define AFX_DRDSADVISEDINFOLIST_H__CBA26066_3E6F_4590_BE50_3FBBCE8F1BF6__INCLUDED_

// (2004.11.29, ��¿�) DRDS Advised Info�� �����ϴ� Data Object List Class�̴�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>								// for CTypedPtrList
#include "../Include_AddIn/I000000/_IPacket.h"		// for IPacket

// (2004.11.29, ��¿�) DRDS Advised Info List Base Class�� �����Ѵ�.
class CDaiPacketGroup;
typedef CTypedPtrList< CObList, CDaiPacketGroup *> CDrdsAdvisedInfoListBase;

// (2004.11.29, ��¿�) DRDS Advised Info�� �����ϴ� Data Object List Class�� �����Ѵ�.
class CDrdsAdvisedInfoList : public CDrdsAdvisedInfoListBase
{
public:
	CDrdsAdvisedInfoList();
	virtual ~CDrdsAdvisedInfoList();

// (2004.11.29, ��¿�) DRDS Advised Info�� �����ϴ� Interface�� �����Ѵ�.
public:
	// 1. DRDS Advised Info Packet Group�� �߰��ϴ� Interface�� �����Ѵ�.
	BOOL	AddDaiPacketGroup( const char *p_szRealPacketName);
	// 2. DRDS Advised Info�� �߰��ϴ� Interface�� �����Ѵ�.
	//		(����! ���� �ش� Real Packet Name�� DRDS Advise Info Packet Group�� ��ϵǾ� �־�� �Ѵ�.)
	BOOL	AddDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szRealKeyCode);
	// RQ�� DRDS���� : �������� - ojtaso (20070111)
	BOOL	AddDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szRealKeyCode);

// (2004.11.29, ��¿�) ������ DRDS Advised Info�� �����ϴ� Interface�� �����Ѵ�.
public:
	// 1. �ϰ� �����ϴ� Interface�� �����Ѵ�.
	void	RemoveAllDrdsAdvisedInfo( void);

// (2004.11.29, ��¿�) ������ DRDS Advised Info�� �˻��ϴ� Interface�� �����Ѵ�.
public:
	// 1. DRDS Advised Info Packet Group�� �˻��ϴ� Inteface�� �����Ѵ�.
	POSITION	FindDaiPacketGroup( const char *p_szRealPacketName);

// (2004.12.01, ��¿�) Real Packet�� ���ſ��θ� �����ϴ� Interface�� �����Ѵ�.
//		(�� Packet�� ���Žø� Ȯ���Ͽ�, Drawing�� ���� ó���� �ϱ� �������̴�.)
public:
	// 1. Real Packet�� ���ſ��θ� �����ϴ� Interface�� �����Ѵ�.
	//		(�̶�, �� Packet�� ���ſ��θ� Ȯ���Ͽ� Return�Ѵ�.)
	BOOL	SetPacketReceivedFlag( const char *p_szRealPacketName, const char *p_szRealKeyCode);
	// 2. ��� Real Packet�� ���ŵǾ������� Ȯ���ϴ� Interface�� �����Ѵ�.
	BOOL	HasOneMoreReceiveForEachPacket( void);
	// 3. ��� Real Packet�� ���� ���� Flag�� �ʱ�ȭ ó���ϴ� Interface�� �����Ѵ�.
	void	ResetPacketReceiveFlag( void);
};

#endif // !defined(AFX_DRDSADVISEDINFOLIST_H__CBA26066_3E6F_4590_BE50_3FBBCE8F1BF6__INCLUDED_)
