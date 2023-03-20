// IPacketListManager.h: interface for the IPacketListManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPACKETLISTMANAGER_H__7F7BE182_1F90_44BA_AAAC_5CE7FAD1D48A__INCLUDED_)
#define AFX_IPACKETLISTMANAGER_H__7F7BE182_1F90_44BA_AAAC_5CE7FAD1D48A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "_AddInVer.h"			// for AddIn Version Define

interface IPacket;
interface IStringList;
interface IPacketManager;

interface IPacketListManager : public IUnknown
{
protected:
	virtual ~IPacketListManager()	{}

// (2008/10/14 - Seung-Won, Bae) Retrieve PacketManager
public:
	virtual	IPacketManager *	GetPacketManager( const char *p_szRQ) = 0;

public:
	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual IPacket *	GetPacket(LPCTSTR lpszRQ, const char *p_szPacketName) = 0;
	// RQ���� RealData ó�� : �������� - ojtaso (20070110)
	virtual BOOL		SetRealData(LPCTSTR lpszRQ, BOOL& bUpdate, IPacket *p_pIPacket, double p_dData, int &p_nRemovePacketDataCount) = 0;
	virtual BOOL		SetRealData(LPCTSTR lpszRQ, BOOL& bUpdate, IPacket *p_pIPacket, const char *p_szField, const char *p_szKey, double p_dData, int &p_nRemovePacketDataCount) = 0;
	virtual IStringList *	GetRQByItemCode( LPCTSTR lpszItemCode) = 0;

public:
	// 2008.08.07 ������ ���� Ratio chart		: ��� RQ�� �̸��� ���ؿ´�		<<
	virtual void		GetAllRQ(CStringArray& arRQ) = 0;
	virtual void		GetItemCode(LPCTSTR lpszRQ, LPCTSTR& lpszItemCode, LPCTSTR& lpszItemName) = 0;

	// 20081024 JS.Kim	������� ocx �� AddIn ���� �ֱ����� ������� �ð��� �Ǻ��ð����� ȯ���Ѵ�.
	virtual bool		CalcNMTimeToRealTime( LPCTSTR lpszRQ, IPacket *p_pIPacket, double p_dData, double& dRealTime, double& dCloseData) = 0;
	// 20081027 JS.Kim	RealData �ް� �ȹް��� ó���� ���񺰷� �Ѵ�
	virtual bool		IsInUseRealData( LPCTSTR lpszRQ) = 0;
};

#endif // !defined(AFX_IPACKETLISTMANAGER_H__7F7BE182_1F90_44BA_AAAC_5CE7FAD1D48A__INCLUDED_)
