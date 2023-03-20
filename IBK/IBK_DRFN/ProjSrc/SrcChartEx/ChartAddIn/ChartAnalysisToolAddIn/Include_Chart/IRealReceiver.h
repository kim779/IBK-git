// IRealReceiver.h: interface for the IRealReceiver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IREALRECEIVER_H__4E6768EB_BD1F_4493_87B9_2DCB96D51980__INCLUDED_)
#define AFX_IREALRECEIVER_H__4E6768EB_BD1F_4493_87B9_2DCB96D51980__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/7/25
// Comments		: Real Receiving Interface of Real Receiver AddIn
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

//********************************************************************
// Caution! Do not update or delete or add in old interface
//	This header was released already.
//********************************************************************

interface IRealReceiver : public IUnknown
{
protected:
	virtual ~IRealReceiver()	{}

//////////////////////////////////////////////////////////////////////
// (2006/8/1 - Seung-Won, Bae) for DRDS Real
//////////////////////////////////////////////////////////////////////
//********************************************************************
// Caution! Do not update or delete or add in old interface
//	This header was released already.
//********************************************************************
// (2006/7/25 - Seung-Won, Bae) Support Real Requesting
public:
	// 1. XFM�� Script���� �����Ǵ� Real ���� �Ӽ� String Data�� �����ϴ� Interface�� �����Ѵ�.
	virtual BOOL	ResetRealSettingInfo( const char *p_szRealSettingInfo)	{	return FALSE;	}
	// 2. 1���� Real ���� Data�� �߰��ϴ� Interface�� �����Ѵ�.
	//		Ư��, p_pstrShareName�� p_pstrSharedData�� �־����� ��쿡�� ������ Method(AddRealItem)�� ���Ͽ� �����Ǵ� �������
	//			p_szRealSettingItem���� Local Share Name�� ���� �ٷ� Real Key Code�� �־�����. 
	//		������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�Ѵ�.
	//		(ResetRealSettingInfo()�� AddRealItem()�� ������ �� �ֵ��� �Ͽ� CString�� Pointer�� ó���Ѵ�.)
	// (2006/4/28 - Seung-Won, Bae) with Separator
	virtual BOOL	AddRealSettingInfo( const char *p_szRealSettingItem, CString *p_pstrShareName = NULL, CString *p_pstrSharedData = NULL, char p_cSeparator = '.')	{	return FALSE;	}
	// 2. ������ Method(RemoveRealItem)�� ���Ͽ� �����Ǵ� 1���� Real Setting Info�� �����ϴ� Interface�� �����Ѵ�.
	//		Ư��, Local Share Name�� ���� �ٷ� Real Key Code�� �־���,
	//			������ Local Share Name�� �����Ͽ� �����ϰ�, ������ �� �ֵ��� ��ȯ�ؾ� �Ѵ�.
	//		����� ���̻� �ش� ���� Key�� ������ Real�� ���� ��쿡�� �������� ��ȯ�ȴ�.
	//		(AddRealSettingInfo()�� Interface�� �����ϵ��� �Ͽ� CString�� Pointer�� ó���Ѵ�.)
	// (2006/4/28 - Seung-Won, Bae) with Separator
	virtual BOOL	RemoveRealSettingInfo( const char *p_szRealSettingItem, CString *p_pstrShareName = NULL, char p_cSeparator = '.')	{	return FALSE;	}
public:
	virtual	BOOL	SetRealAdvise( BOOL p_bAdvise)							{	return FALSE;	}
	virtual BOOL	GetRealAdvisedInfo( CString &p_strAdvisedInfo)			{	return FALSE;	}

// (2006/7/25 - Seung-Won, Bae) Support DRDS Interface
public:
	virtual BOOL	SetDrdsData( UINT wParam, LPARAM lParam)				{	return FALSE;	}
	virtual BOOL	SetDrdsPacketEnd( UINT wParam, LPARAM lParam)			{	return FALSE;	}

//////////////////////////////////////////////////////////////////////
// (2006/8/1 - Seung-Won, Bae) for Direct Real
//////////////////////////////////////////////////////////////////////
//********************************************************************
// Caution! Do not update or delete or add in old interface
//	This header was released already.
//********************************************************************
public:
	virtual BOOL	SetRealData( const char *p_szPacketName, double p_dData)	{	return FALSE;	}
	virtual BOOL	SetRealPacketEnd( void)										{	return FALSE;	}
};

#endif // !defined(AFX_IREALRECEIVER_H__4E6768EB_BD1F_4493_87B9_2DCB96D51980__INCLUDED_)
