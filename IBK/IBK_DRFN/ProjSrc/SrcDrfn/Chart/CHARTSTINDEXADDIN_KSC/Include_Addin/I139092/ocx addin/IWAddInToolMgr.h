// IWAddInToolMgr.h: interface for the CIWAddInToolMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWADDINTOOLMGR_H__E1D8DADE_E2D2_4234_902E_A4931D02D9E3__INCLUDED_)
#define AFX_IWADDINTOOLMGR_H__E1D8DADE_E2D2_4234_902E_A4931D02D9E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../_IWUnknown.h"	// for CIWUnknown
#include "_IAddInToolMgr.h"	// for IAddInToolMgr
#include "../../AddInManagerImp.h"		// for CAddInManagerImp

class CIWAddInToolMgr : public CIWUnknown, public IAddInToolMgr
{
// (2004.10.07, ��¿�) Interface Wrapper�� �⺻ Interface (IUnknown��)�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IAddInToolMgr, CIWAddInToolMgr, CAddInManagerImp, m_pAddInManager)

// (2006/2/12 - Seung-Won, Bae) Register/Unregister AddIn Event Map
public:
	virtual BOOL	RegisterEventMap( const IChartAddIn::EAddInEventType p_eAddInEventType, IAddInDllBase *p_pIAddInDllBase, const BOOL p_bOverExclusive);
	virtual BOOL	UnregisterEventMap( const IChartAddIn::EAddInEventType p_eAddInEventType, IAddInDllBase *p_pIAddInDllBase, const BOOL p_bOverExclusive);

// (2006/1/17 - Seung-Won, Bae) Keyboard & Mouse Exclusive Setting
protected:
	virtual BOOL	GetKeyboardExclusive(		IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	IsKeyboardExclusive(		IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	ReleaseKeyboardExclusive(	IAddInDllBase *p_pIAddInDllBase);
protected:
	virtual BOOL	GetAllMouseExclusive(		IAddInDllBase *p_pIAddInDllBase);	// return TRUE in one more Exclusive Created
	virtual BOOL	IsAllMouseExclusive(		IAddInDllBase *p_pIAddInDllBase);	// return TRUE in one more Exclusive Setted
	virtual BOOL	ReleaseAllMouseExclusive(	IAddInDllBase *p_pIAddInDllBase);	// return TRUE in one more Exclusive Released
protected:
	virtual BOOL	GetLButtonExclusive(		IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	IsLButtonExclusive(			IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	ReleaseLButtonExclusive(	IAddInDllBase *p_pIAddInDllBase);
protected:
	virtual BOOL	GetRButtonExclusive(		IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	IsRButtonExclusive(			IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	ReleaseRButtonExclusive(	IAddInDllBase *p_pIAddInDllBase);
protected:
	virtual BOOL	GetMouseMoveExclusive(		IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	IsMouseMoveExclusive(		IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	ReleaseMouseMoveExclusive(	IAddInDllBase *p_pIAddInDllBase);
protected:
	virtual BOOL	GetMouseWheelExclusive(		IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	IsMouseWheelExclusive(		IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	ReleaseMouseWheelExclusive(	IAddInDllBase *p_pIAddInDllBase);

// (2004.10.11, ��¿�) AddIn DLL�� �����Ѵ�.
public:
	// (2006/1/7 - Seung-Won, Bae) Interface for Spacial Interface of AddIn DLL
	virtual IUnknown *	GetAddInCustomInterface( const char *p_szAddInModule);

// (2005/2/4 - Seung-Won, Bae) Real ���� Event�� �����Ѵ�.
// (2005/8/16 - Seung-Won, Bae) Not used in Hanwha
protected:
	// [04/10/14] Real�� ������ �����Ѵ�.
	//				�ӵ� ����� DRDS�� Data�� �״�� Pass�Ѵ�.
	virtual void	OnDrdsData( WPARAM p_wParam, LPARAM p_lParam);
	// [04/10/15] Real Packet�� ������ �������� AddIn DLL���� �˷� ó����� �Ѵ�.
	virtual void	OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam);

// (2006/7/27 - Seung-Won, Bae) Real & TR Data Update Event
protected:
	virtual void	OnUpdatePacketData( BOOL p_bTrUpdate = FALSE);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
protected:
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDllBase *p_pParam);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3);
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2);
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID);
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam);
};

#endif // !defined(AFX_IWADDINTOOLMGR_H__E1D8DADE_E2D2_4234_902E_A4931D02D9E3__INCLUDED_)
