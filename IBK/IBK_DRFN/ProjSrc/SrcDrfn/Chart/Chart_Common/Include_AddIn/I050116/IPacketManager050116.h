// IPacketManager050116.h: interface for the IPacketManager050116 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPACKETMANAGER050116_H__BAD6BB66_01E4_46F1_8938_3A15A0B0048A__INCLUDED_)
#define AFX_IPACKETMANAGER050116_H__BAD6BB66_01E4_46F1_8938_3A15A0B0048A__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����!	050116 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		Interface ���� ���� ���� ������ �ʽ��ϴ�.
//		������ �ʴ� ������ 050116 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050116 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			��¿�
//			ChartPatternAddIn			GZone			������
//			ChartGuideLine				GZone			������
//////////////////////////////////////////////////////////////////////

// (2004.10.11, ��¿�) Packet Data List�� Object Interface�� �����Ѵ�.
//		AddIn DLL ���� Interface�� Macro Interface�� �����Ѵ�.
#if IPacketManagerVer < 050116
	#define	IPacketManager		IPacketManager050116
	#undef	IPacketManagerVer
	#define	IPacketManagerVer	050116
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IPacket050116.h"		// for IPacket

interface IPacketManager050116 : public IUnknown  
{
protected:
	virtual ~IPacketManager050116()	{}

// [04/10/11] Packet Object�� Interface�� ���ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/11] Packet Name���� Packet Object�� ���ϴ� Interface�� �����Ѵ�.
	virtual IPacket *	GetPacket( const char *p_szPacketName) = 0;
	// [04/11/30] Packet Object�� Memory Address���� Packet Object�� ���ϴ� Interface�� �����Ѵ�.
	virtual IPacket *	GetPacketInterface( unsigned long p_ulPacketAddress) = 0;

// [04/10/13] ���� Message�� �˻��ϴ� Interface�� �����Ѵ�. (�ֱٿ� ���ŵ� TR�� ���ѵȴ�.)
public:
	virtual BOOL		GetHelpMessageItem( const char *p_szMsgKey, CString &p_szMessage) = 0;
};

#endif // !defined(AFX_IPACKETMANAGER050116_H__BAD6BB66_01E4_46F1_8938_3A15A0B0048A__INCLUDED_)
