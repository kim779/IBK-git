// ChartAddInBase.h: interface for the CChartAddInBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTADDINBASE_H__DDC8A6DF_434B_4790_898D_B98EDE523730__INCLUDED_)
#define AFX_CHARTADDINBASE_H__DDC8A6DF_434B_4790_898D_B98EDE523730__INCLUDED_

// (2004.10.08, ��¿�) Chart AddIn Object�� Base Class�� �����Ѵ�.
// (2004.10.18, ��¿�) Chart AddIn Object Base Class�� Chart Module���� Interface�� �����ϴ� �κ��̹Ƿ�,
//		Chart�ʿ��� AddIn Module�� �����ϸ鼭 ���������Ѵ�.	����, Version ��ȣ�� �����Ͽ� �����Ѵ�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../_IWUnknown.h"			// for CIWUnknown
#include "_IChartAddIn.h"			// for IChartAddIn
#include "_IChartOCX.h"				// for IChartOCX

class CChartAddInBase : public CIWUnknown, public IChartAddIn
{
protected:
	CChartAddInBase( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartAddInBase();

// (2004.10.07, ��¿�) Interface Wrapper�� �⺻ Interface (IUnknown��)�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2004.10.08, ��¿�) Chart OCX Interface�� �����Ͽ�, ���� Ÿ Object�� ��ȸ�� �� �ֵ��� �Ѵ�.
protected:
	IChartOCX *			m_pIChartOCX;

// (2004.10.25, ��¿�) Chart OCX Control Window Pointer�� �����Ͽ� Windows Interface�� �̿��� �� �ֵ��� �Ѵ�.
protected:
	CWnd *				m_pChartCWnd;

// (2006/1/17 - Seung-Won, Bae) Manage IAddInDll for AddIn Tool Exclusive
protected:
	IAddInDllBase *	m_pIAddInDllBase;

// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
protected:
	IPacketManager *	m_pIPacketManager;
	IChartManager *		m_pIChartManager;
	IGraphInfoManager *	m_pIGraphInfoManager;
	IAddInToolMgr *		m_pIAddInToolMgr;
// (2004.10.13, ��¿�) �⺻ Manager�� ��ϵǾ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
protected:
	virtual BOOL		HasFullManager( void);

// [05/07/08] AddIn ���� Interface�� �����Ѵ�.
//		*. ����. �ݵ��� ���� Interface Pointer�� Converting�� IUnknown�� Pointer�� ��ȯ�Ͽ� �����Ѵ�.
protected:
	virtual IUnknown *	GetAddInCustomInterface( void)				{	return NULL;	}

// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
protected:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData)	{	return FALSE;	}
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)	{	return FALSE;	}	// (2005/9/2 - Seung-Won, Bae) for String Data

// (2006/2/12 - Seung-Won, Bae) Register AddIn Event Map
protected:
	BOOL								m_bOverExclusive;
protected:
	virtual const EAddInEventType *		GetRegisteredEventMap( void) const = 0;
	virtual IAddInDllBase::EAddInEventType	GetGlobalAddInEventType( const long p_eAddInEventType) const	{	return IAddInDllBase::EAddInEventType::EAE_NOT_DEFINED;	}
public:
	virtual int						RegisterEventMap( const BOOL p_bOverExclusive);
	virtual int						UnregisterEventMap( void);

//////////////////////////////////////////////////////////////////////
// Event Routines
//////////////////////////////////////////////////////////////////////

// (2004.10.11, ��¿�) �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
protected:
	// [04/10/11] ������ �����ϴ� Interface�� �����Ѵ�.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare)	{}
	// (2005/11/21 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue)	{}
	// (2005/8/10 - Seung-Won, Bae) use in Only Hanwha
	virtual void	OnSharedDataChange( const char *p_szShareName, const char *p_szSharedData, BOOL p_bStartShare)	{}

	// [04/10/11] AddIn DLL�鿡�� ResetChart ��Ȳ�� �˷� ó���ǵ��� �Ѵ�.
	virtual void	OnResetChart( void)	{}
	// [04/10/11] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawBeforeGraph( CDC *p_pDC)	{}
	// [04/12/25] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawAfterGraph( CDC *p_pDC)	{}
	// [04/10/13] TR�� ������ AddIn DLL�鿡�� �˷� ������� �Ѵ�.
	virtual void	OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset)	{}
	// [04/12/07] TR�̳� Real�� ���ſ� ���� Packet Data�� Update�Ǿ����� AddIn DLL�鿡 �˷� ��ǥ���� ó����� �Ѵ�.
	virtual void	OnUpdatePacketData( BOOL p_bTrUpdate)	{}

// (2004.10.29, ��¿�) AddIn DLL���� �׹��� Windows Event�� �����Ѵ�.
protected:
	// [04/10/29] Timer Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual void	OnTimer( UINT p_uTimerID)	{}
	// [04/10/29] Mouse Capture Release Event�� AddIn DLL���� �����ϵ��� �Ѵ�.
	//		����! OnCaptureChanged()�� SetCapture()/RelaseCapture()�� ���� ��� �ܿ���,
	//			Focus �� Activate, ToolTip, TrackMenu� ���ؼ��� ���÷� (SetCapture����)
	//			���� �ڱ� �ڽ��� �ٽ� SetCapture()�ϴ� ��쿡�� ȣ����� �����Ѵ�.
	virtual void	OnCaptureChanged(	CWnd *pWnd)	{}

// (2005/2/4 - Seung-Won, Bae) Real ���� Event�� �����Ѵ�.
// (2005/8/16 - Seung-Won, Bae) Not used in Hanwha
protected:
	// [04/10/14] Real�� ������ �����Ѵ�.
	//				�ӵ� ����� DRDS�� Data�� �״�� Pass�Ѵ�.
	virtual void	OnDrdsData( WPARAM p_wParam, LPARAM p_lParam)		{}
	// [04/10/15] Real Packet�� ������ �������� AddIn DLL���� �˷� ó����� �Ѵ�.
	virtual void	OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam)	{}
// (2005/8/16 - Seung-Won, Bae) used in Only Hanwha
protected:
	// 1. Notify for Every Real Item
	virtual void	OnRealData( WORD p_wRealRecordID, WORD p_wRealItemID, const char *p_szRealKeyCode, long p_lPacketAddress, double dData)	{}
	// 2. Notify for Every Real Record End
	virtual void	OnRealRedcordEnd( WORD p_wRealRecordID, const char *p_szRealKeyCode)													{}

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)									{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID)																		{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)	{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam)												{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)							{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const CBlockBehaviorData *p_pParam)									{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const CStringList *p_pParam)											{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const CStyleData *p_pParam)											{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDllBase *p_pParam)										{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam)													{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam)							{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3)	{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)							{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2, const long p_lParam3, const long p_lParam4, const long p_lParam5, const long p_lParam6, const long p_lParam7, const long p_lParam8, const long p_lParam9)	{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, CWnd *p_pWnd, CPoint &point)											{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID)														{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam)								{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam, CObject *p_pParam)			{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam)								{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)			{	return FALSE;	}
protected:
	virtual BOOL	OnKeyDown(			UINT nChar, UINT nRepCnt, UINT nFlags)																	{	return FALSE;	}
	virtual BOOL	OnKeyUp(			UINT nChar, UINT nRepCnt, UINT nFlags)																	{	return FALSE;	}
	virtual BOOL	OnMouseWheel(		UINT nFlags, short zDelta, CPoint pt, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnMouseMove(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnLButtonDown(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnLButtonUp(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnLButtonDblClk(	CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnRButtonDown(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnRButtonUp(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnRButtonDblClk(	CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
};

#endif // !defined(AFX_CHARTADDINBASE_H__DDC8A6DF_434B_4790_898D_B98EDE523730__INCLUDED_)
