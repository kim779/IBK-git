// IWPacketListManager.h: interface for the CIWPacketListManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWPACKETLISTMANAGER_H__CD122C5A_B302_4E58_9056_18A7EE51BE4B__INCLUDED_)
#define AFX_IWPACKETLISTMANAGER_H__CD122C5A_B302_4E58_9056_18A7EE51BE4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../Include_Chart/Dll_Load/PacketListManager.h"	// for CPacketListManager
#include "../_IWUnknown.h"
#include "_IPacketListManager.h"
#include "_IChartOCX.h"											// for IChartOCX

class CIWPacketListManager : public CIWUnknown, public IPacketListManager
{
// (2004.10.07, ��¿�) Interface Wrapper�� �⺻ Interface (IUnknown��)�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IPacketListManager, CIWPacketListManager, CPacketListManager, m_pPacketListManager)

// (2008/10/14 - Seung-Won, Bae) Retrieve PacketManager
protected:
	virtual	IPacketManager *	GetPacketManager( const char *p_szRQ);

// (2007/2/13 - Seung-Won, Bae) Define Interfaces.
protected:
	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual IPacket *		GetPacket( LPCTSTR lpszRQ, const char *p_szPacketName);
	virtual IStringList *	GetRQByItemCode( LPCTSTR lpszItemCode);
	virtual IStringList *	GetAllRQ( void);
	virtual void			GetItemCode(LPCTSTR lpszRQ, LPCTSTR& lpszItemCode, LPCTSTR& lpszItemName);
	// ���簡 ����� - ojtaso (20080602)
	virtual void			OffsetPacketData(LPCTSTR lpszRQ, double dData, CString strPacketName = _T(""));
	// (2009/2/16 - Seung-Won, Bae) Support Non-Time X Scale Manager.
	virtual BOOL			IsBasePacketString( void);

// 20081007 JS.Kim	����ڽð��� ����
protected:
	virtual IPacket		*GetDatePacket(LPCTSTR lpszRQ);
};

#endif // !defined(AFX_IWPACKETLISTMANAGER_H__CD122C5A_B302_4E58_9056_18A7EE51BE4B__INCLUDED_)
