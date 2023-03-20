// DaiCodeGroup.h: interface for the CDaiCodeGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAICODEGROUP_H__A3CAF812_F128_41AA_AD68_328137619069__INCLUDED_)
#define AFX_DAICODEGROUP_H__A3CAF812_F128_41AA_AD68_328137619069__INCLUDED_

// (2004.11.29, ��¿�) Packet�� DRDS Advised Info�� Code�� �����ϴ� Data Object List Class�̴�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>								// for CTypedPtrList
#include "../Include_AddIn/I000000/_IPacket.h"		// for IPacket

// (2004.11.29, ��¿�) DRDS Advised Info Object Class�� �����Ѵ�.
class CDrdsAdvisedInfo : public CObject
{
public:
	CDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealFieldName)
	{
		// RQ�� DRDS���� : �������� - ojtaso (20070111)
		m_lpszRQ				= NULL;
		m_pIPacket				= p_pIPacket;
		m_strRealFieldName		= p_szRealFieldName;
		if( m_pIPacket) m_pIPacket->AddRef();
	}
	// RQ�� DRDS���� : �������� - ojtaso (20070111)
	CDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealFieldName)
	{
		m_lpszRQ				= ::strdup(lpszRQ);
		m_pIPacket				= p_pIPacket;
		m_strRealFieldName		= p_szRealFieldName;
		if( m_pIPacket) m_pIPacket->AddRef();
	}
	~CDrdsAdvisedInfo()
	{
		if( m_pIPacket) m_pIPacket->Release();

		// RQ�� DRDS���� : �������� - ojtaso (20070111)
		if(m_lpszRQ)
		{
			::free(m_lpszRQ);
			m_lpszRQ = NULL;
		}
	}
public:
	IPacket *	m_pIPacket;
	CString		m_strRealFieldName;
	LPSTR		m_lpszRQ;		// RQ�� DRDS���� : �������� - ojtaso (20070111)
};

// (2004.11.29, ��¿�) ���� Real Packet Name ���� �����ϴ� DRDS Advised Info List Packet Group��
//		���� Real Key Code���� DRDS Advised Info�� �����ϴ� Code Group Base Class�� �����Ѵ�.
typedef CTypedPtrList< CObList, CDrdsAdvisedInfo *> CDaiCodeGroupBase;

// (2004.11.29, ��¿�) Packet�� DRDS Advised Info�� Real Key Code���� �����ϴ� Data Object Class�� �����Ѵ�.
class CDaiCodeGroup : public CObject
{
public:
	CDaiCodeGroup( const char *p_szRealKeyCode);
	virtual ~CDaiCodeGroup();

// (2004.11.29, ��¿�) �������� DRDS Advised Info�� Key Code ���� �����Ѵ�.
public:
	CString		m_strRealKeyCode;

// (2004.11.30, ��¿�) CDaiPacketGroup�� CDaiCodeGroup Adding�� CDaiCodeGroup�� CDaiPacketGroup�� 
//		�������� �ʵ��� CDaiCodeGroup�� CDaiCodeGroupBase���� ��¹��� �ʰ� Member�� �����Ѵ�.
protected:
	CDaiCodeGroupBase	m_lDaiCodeGroup;
public:
	void				RemoveAt( POSITION position)			{	m_lDaiCodeGroup.RemoveAt( position);			}
	BOOL				IsEmpty( void) const					{	return m_lDaiCodeGroup.IsEmpty();				}
	POSITION			AddTail( CDrdsAdvisedInfo *newElement)	{	return m_lDaiCodeGroup.AddTail( newElement);	}
	POSITION			GetHeadPosition( void) const			{	return m_lDaiCodeGroup.GetHeadPosition();		}
	CDrdsAdvisedInfo *	GetAt( POSITION position) const			{	return m_lDaiCodeGroup.GetAt( position);		}
	CDrdsAdvisedInfo *&	GetNext( POSITION& rPosition)			{	return m_lDaiCodeGroup.GetNext( rPosition);		}
	CDrdsAdvisedInfo *	RemoveHead( void)						{	return m_lDaiCodeGroup.RemoveHead();			}

// (2004.11.29, ��¿�) DRDS Advised Info�� �����ϴ� Interface�� �����Ѵ�.
public:
	// 1. 1���� DRDS Advised Info Data�� �߰��ϴ� Interface�� �����Ѵ�.
	BOOL	AddDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealFieldName);
	// RQ�� DRDS���� : �������� - ojtaso (20070111)
	BOOL	AddDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealFieldName);

// (2004.11.29, ��¿�) ������ DRDS Advised Info�� �����ϴ� Interface�� �����Ѵ�.
public:
	// 1. �ϰ� �����ϴ� Interface�� �����Ѵ�.
	void	RemoveAllDrdsAdvisedInfo( void);

// (2004.11.29, ��¿�) ������ DRDS Advised Info�� �˻��ϴ� Interface�� �����Ѵ�.
public:
	// 1. Drds Advised Info ���� �����޾� �˻��ϴ� Inteface�� �����Ѵ�.
	POSITION	FindDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealFieldName);
	// RQ�� DRDS���� : �������� - ojtaso (20070111)
	POSITION	FindDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealFieldName);

// (2004.12.01, ��¿�) Real Packet�� ���ſ��θ� �����ϴ� Interface�� �����Ѵ�.
//		(�� Packet�� ���Žø� Ȯ���Ͽ�, Drawing�� ���� ó���� �ϱ� �������̴�.)
public:
	BOOL		m_bPacketReceived;
};

#endif // !defined(AFX_DAICODEGROUP_H__A3CAF812_F128_41AA_AD68_328137619069__INCLUDED_)
