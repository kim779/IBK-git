// IPacketManager.h: interface for the IPacketManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPACKETMANAGER_H__BAD6BB66_01E4_46F1_8938_3A15A0B0048A__INCLUDED_)
#define AFX_IPACKETMANAGER_H__BAD6BB66_01E4_46F1_8938_3A15A0B0048A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>			// for CTypedPtrList
#include "_AddInVer.h"			// for AddIn Version Define
#include "_IString.h"					// for ILPCSTR

#include "BlockBaseEnum.h"		// for CScaleBaseData::HORZSCALEDRAWERTYPE

interface IPacket;
interface IPacketList;
interface IHelpMessageMap;
interface IPacketManager : public IUnknown  
{
public:
	typedef enum
	{
		INONAME = -1,
		ISTOCK = 0,	// �ֽ�
		IINDUSTRY,	// ����
		IFUTURES,	// ����
		IOPTION		// �ɼ�
	} ICOMMODITY_BOND_NAME;

protected:
	virtual ~IPacketManager()	{}

// [04/10/11] Packet Object�� Interface�� ���ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/11] Packet Name���� Packet Object�� ���ϴ� Interface�� �����Ѵ�.
	virtual IPacket *	GetPacket( const char *p_szPacketName) = 0;
	// (2005/9/2 - Seung-Won, Bae) Get Packet Interface List for Packet Names
	virtual BOOL		GetPacketList( const char *p_szPacketNames, IPacketList *p_lIPacketList) = 0;
	// [04/11/30] Packet Object�� Memory Address���� Packet Object�� ���ϴ� Interface�� �����Ѵ�.
	virtual IPacket *	GetPacketInterface( unsigned long p_ulPacketAddress) = 0;
	// (2006/1/9 - Seung-Won, Bae) Get Packet Interface List from CPacket List
	//					 ~ CObject * <= const CList<CPacket*, CPacket*> *
	virtual BOOL		GetIPacketListFromCPacketList( CObject *p_plCPacket, IPacketList *p_lIPacketList) = 0;

// [04/10/13] ���� Message�� �˻��ϴ� Interface�� �����Ѵ�. (�ֱٿ� ���ŵ� TR�� ���ѵȴ�.)
public:
	virtual BOOL				GetHelpMessageItem( const char *p_szMsgKey, ILPCSTR &p_pszMessage) = 0;
	// [05/09/29] Get Help Message List
	virtual IHelpMessageMap *	GetHelpMessageMap( void) = 0;

// (2006/6/22 - Seung-Won, Bae) Retrieve Packet Infos
public:
	// (2006/6/22 - Seung-Won, Bae) Get RealUpdated Data Index
	virtual int			GetLastestRealIndex( void) = 0;
	// (2006/6/22 - Seung-Won, Bae) Get Maximum Data Count
	virtual long		GetMaximumDataCount( void) = 0;
	virtual long		GetMaximumDataCount( const char *p_szPacketNames, const char *p_szCompartString = ",") = 0;
	// (2006/6/26 - Seung-Won, Bae) Get the BaseTime Info
	virtual CScaleBaseData::HORZSCALEDRAWERTYPE	GetBaseTimeDataType( void) = 0;
	virtual int			GetBaseTimeUnit( void) = 0;
	// (2006/7/26 - Seung-Won, Bae) Retrieve the flag of Real Usage
	virtual BOOL		IsInUseRealData( void) = 0;
	// (2006/8/15 - Seung-Won, Bae) Retrieve Main Item Type
	virtual ICOMMODITY_BOND_NAME	GetCommodityBondNameInBase( void) = 0;

// (2006/7/27 - Seung-Won, Bae) Process Data
public:
	virtual BOOL		SetRealData( IPacket *p_pIPacket, double p_dData, int &p_nRemovePacketDataCount) = 0;
	virtual BOOL		SetRealData( IPacket *p_pIPacket, const char *p_szField, const char *p_szKey, double p_dData, int &p_nRemovePacketDataCount) = 0;
};

#endif // !defined(AFX_IPACKETMANAGER_H__BAD6BB66_01E4_46F1_8938_3A15A0B0048A__INCLUDED_)
