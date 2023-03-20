// DaiPacketGroup.h: interface for the CDaiPacketGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAIPACKETGROUP_H__4B929BC4_05F2_441D_B492_605AB5FBB18F__INCLUDED_)
#define AFX_DAIPACKETGROUP_H__4B929BC4_05F2_441D_B492_605AB5FBB18F__INCLUDED_

// (2004.11.29, ��¿�) DRDS Advised Info�� Packet ���� �����ϴ� Data Object List Class�̴�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>								// for CTypedPtrList
#include "../Include_AddIn/I000000/_IPacket.h"		// for IPacket
#include "DaiCodeGroup.h"							// for CDaiCodeGroup

// (2004.11.29, ��¿�) ���� Real Packet Name ���� �����ϴ� DRDS Advised Info List Packet Group Base Class�� �����Ѵ�.
typedef CTypedPtrList< CObList, CDaiCodeGroup *> CDaiPacketGroupBase;

// (2004.11.29, ��¿�) DRDS Advised Info�� Real Packet Name���� �����ϴ� Data Object Class�� �����Ѵ�.
class CDaiPacketGroup : public CObject
{
public:
	CDaiPacketGroup( const char *p_szRealPacketName);
	virtual ~CDaiPacketGroup();

// (2004.11.29, ��¿�) �������� DRDS Advised Info�� Packet ���� �����Ѵ�.
public:
	CString	m_strRealPacketName;

// (2004.11.30, ��¿�) CDrdsAdvisedInfoList�� CDaiPacketGroup Adding�� CDaiPacketGroup�� CDrdsAdvisedInfoList�� 
//		�������� �ʵ��� CDaiPacketGroup�� CDaiPacketGroupBase���� ��¹��� �ʰ� Member�� �����Ѵ�.
protected:
	CDaiPacketGroupBase		m_lDaiPacketGroup;
public:
	void				RemoveAt( POSITION position)			{	m_lDaiPacketGroup.RemoveAt( position);			}
	BOOL				IsEmpty( void) const					{	return m_lDaiPacketGroup.IsEmpty();				}
	POSITION			AddTail( CDaiCodeGroup *newElement)		{	return m_lDaiPacketGroup.AddTail( newElement);	}
	POSITION			GetHeadPosition( void) const			{	return m_lDaiPacketGroup.GetHeadPosition();		}
	POSITION			GetTailPosition( void) const			{	return m_lDaiPacketGroup.GetTailPosition();		}
	CDaiCodeGroup *		GetAt( POSITION position) const			{	return m_lDaiPacketGroup.GetAt( position);		}
	CDaiCodeGroup *&	GetNext( POSITION& rPosition)			{	return m_lDaiPacketGroup.GetNext( rPosition);	}
	CDaiCodeGroup *		RemoveHead( void)						{	return m_lDaiPacketGroup.RemoveHead();			}

// (2004.11.29, ��¿�) DRDS Advised Info�� �����ϴ� Interface�� �����Ѵ�.
public:
	// 1. 1���� DRDS Advised Info Data�� �߰��ϴ� Interface�� �����Ѵ�.
	BOOL	AddDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealFieldName, const char *p_szRealKeyCode);
	// RQ�� DRDS���� : �������� - ojtaso (20070111)
	BOOL	AddDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealFieldName, const char *p_szRealKeyCode);

// (2004.11.29, ��¿�) ������ DRDS Advised Info�� �����ϴ� Interface�� �����Ѵ�.
public:
	// 1. �ϰ� �����ϴ� Interface�� �����Ѵ�.
	void	RemoveAllDrdsAdvisedInfo( void);

// (2004.11.29, ��¿�) ������ DRDS Advised Info�� �˻��ϴ� Interface�� �����Ѵ�.
public:
	// 1. Drds Advised Info ���� �����޾� �˻��ϴ� Inteface�� �����Ѵ�.
	POSITION	FindDaiCodeGroup( const char *p_szRealKeyCode);

// (2004.12.01, ��¿�) Real Packet�� ���ſ��θ� �����ϴ� Interface�� �����Ѵ�.
//		(�� Packet�� ���Žø� Ȯ���Ͽ�, Drawing�� ���� ó���� �ϱ� �������̴�.)
public:
	// 1. Real Packet�� ���ſ��θ� �����ϴ� Interface�� �����Ѵ�.
	//		(�̶�, �� Packet�� ���ſ��θ� Ȯ���Ͽ� Return�Ѵ�.)
	BOOL	SetPacketReceivedFlag( const char *p_szRealKeyCode);
	// 2. ��� Real Packet�� ���ŵǾ������� Ȯ���ϴ� Interface�� �����Ѵ�.
	BOOL	HasOneMoreReceiveForEachPacket( void);
	// 3. ��� Real Packet�� ���� ���� Flag�� �ʱ�ȭ ó���ϴ� Interface�� �����Ѵ�.
	void	ResetPacketReceiveFlag( void);
};

#endif // !defined(AFX_DAIPACKETGROUP_H__4B929BC4_05F2_441D_B492_605AB5FBB18F__INCLUDED_)
