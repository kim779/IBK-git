// IWPacketManager050531.h: interface for the CIWPacketManager050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWPACKETMANAGER050531_H__5B20ECF2_9545_47B2_8A7A_0763FEF08F08__INCLUDED_)
#define AFX_IWPACKETMANAGER050531_H__5B20ECF2_9545_47B2_8A7A_0763FEF08F08__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����!	050531 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� Wrapper�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		������ �ʴ� ������ 050531 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			��ȭ			��¿�
//		ChartAssistIndexAddIn		��ȭ			���ٹ�
//////////////////////////////////////////////////////////////////////

// (2004.10.11, ��¿�) Packet Data List�� Interface Wrapper Class�� �����Ѵ�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../IWUnknown.h"
#include "IPacketManager050531.h"

class CPacketList;
class CIWPacketManager050531 : public CIWUnknown, public IPacketManager050531  
{
public:
	CIWPacketManager050531( CPacketList *p_pPacketList);
	virtual ~CIWPacketManager050531();

// (2004.10.07, ��¿�) Interface Wrapper�� �⺻ Interface (IUnknown��)�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2004.10.11, ��¿�) Packet List Object�� Pointer�� �����Ѵ�.
protected:
	CPacketList *	m_pPacketList;

// [04/10/11] Packet Object�� Interface�� ���ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/11] Packet Name���� Packet Object�� ���ϴ� Interface�� �����Ѵ�.
	virtual IPacket *	GetPacket( const char *p_szPacketName);
	// (2005/9/2 - Seung-Won, Bae) Get Packet Interface List for Packet Names
	virtual BOOL		GetPacketList( const char *p_szPacketNames, CTypedPtrList< CPtrList, IPacket *> *p_plIPackets);
	// [04/11/30] Packet Object�� Memory Address���� Packet Object�� ���ϴ� Interface�� �����Ѵ�.
	virtual IPacket *	GetPacketInterface( unsigned long p_ulPacketAddress);

// [04/10/13] ���� Message�� �˻��ϴ� Interface�� �����Ѵ�. (�ֱٿ� ���ŵ� TR�� ���ѵȴ�.)
public:
	virtual BOOL		GetHelpMessageItem( const char *p_szMsgKey, CString &p_szMessage);
	// [05/09/29] Get Help Message List
	virtual const CMapStringToString *	GetHelpMessageMap( void);
};

#endif // !defined(AFX_IWPACKETMANAGER050531_H__5B20ECF2_9545_47B2_8A7A_0763FEF08F08__INCLUDED_)
