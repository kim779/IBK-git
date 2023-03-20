// IChartAddIn.h: interface for the IChartAddIn class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_)
#define AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../_IAddInDllBase.h"		// for IAddInDllBase
#include "_AddInVer.h"				// for AddIn Version Define

enum EAICommandType;
class CStyleData;
class CBlockBehaviorData;
interface IChartAddIn : public IUnknown  
{
protected:
	virtual ~IChartAddIn()	{}

// (2004.10.13, ��¿�) �⺻ Manager�� ��ϵǾ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
public:
	virtual BOOL	HasFullManager( void) = 0;

// [05/07/08] AddIn ���� Interface�� �����Ѵ�.
//		*. ����. �ݵ��� ���� Interface Pointer�� Converting�� IUnknown�� Pointer�� ��ȯ�Ͽ� �����Ѵ�.
public:
	virtual IUnknown *	GetAddInCustomInterface( void) = 0;

// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
public:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData) = 0;
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData) = 0;	// (2005/9/2 - Seung-Won, Bae) for String Data


// (2006/2/12 - Seung-Won, Bae) Register AddIn Event Map
public:
	// (2006/2/12 - Seung-Won, Bae) Define Using Enum Type of AddIn Event in this AddIn Interface Version
	//	********************************************************************************
	//	* Caution! It can be sorted.
	//	********************************************************************************
	typedef enum
	{
		ADDINEVENT( OnAddInToolCommand_EAICommandType),							// OnAddInToolCommand(	const EAICommandType p_eCommandID)																
		ADDINEVENT( OnAddInToolCommand_EAICommandType_CBlockBehaviorData),		// OnAddInToolCommand(	const EAICommandType p_eCommandID, const CBlockBehaviorData *p_pParam)								
		ADDINEVENT( OnAddInToolCommand_EAICommandType_CDC_UINT_CPoint_DWORD),	// OnAddInToolCommand(	const EAICommandType p_eCommandID, CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)
		ADDINEVENT( OnAddInToolCommand_EAICommandType_char),					// OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam)																		
		ADDINEVENT( OnAddInToolCommand_EAICommandType_char_long),				// OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)						
		ADDINEVENT( OnAddInToolCommand_EAICommandType_CStringList_Not_Used),	// OnAddInToolCommand(	const EAICommandType p_eCommandID, const CStringList *p_pParam)						
		ADDINEVENT( OnAddInToolCommand_EAICommandType_CStyleData),				// OnAddInToolCommand(	const EAICommandType p_eCommandID, const CStyleData *p_pParam)						
		ADDINEVENT( OnAddInToolCommand_EAICommandType_IAddInDllBase),			// OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDllBase *p_pParam)						
		ADDINEVENT( OnAddInToolCommand_EAICommandType_long),					// OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam)					
		ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char),				// OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam)						
		ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char_char_char),		// OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3)									
		ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long),				// OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)						
		ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long_long_long_long_long_long_long_long),	// OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2, const long p_lParam3, const long p_lParam4, const long p_lParam5, const long p_lParam6, const long p_lParam7, const long p_lParam8, const long p_lParam9)																		
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType),					// OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID)														
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char),				// OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam)															
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_CObject),		// OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam, CObject *p_pParam)																		
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long),				// OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam)															
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long_long),			// OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)																	
		ADDINEVENT( OnCaptureChanged),			// OnCaptureChanged(	CWnd *pWnd)											
		ADDINEVENT( OnCmdMsg),					// OnCmdMsg(			UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)							
		ADDINEVENT( OnDrawAfterGraph),			// OnDrawAfterGraph( CDC *p_pDC)																	
		ADDINEVENT( OnDrawBeforeGraph),			// OnDrawBeforeGraph( CDC *p_pDC)																		
		ADDINEVENT( OnDrdsData),				// OnDrdsData( WPARAM p_wParam, LPARAM p_lParam)											
		ADDINEVENT( OnDrdsPacketEnd),			// OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam)												
		ADDINEVENT( OnGSharedDataChange),		// OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare)																		
		ADDINEVENT( OnKeyDown),					// OnKeyDown(			UINT nChar, UINT nRepCnt, UINT nFlags)	
		ADDINEVENT( OnKeyUp),					// OnKeyUp(			UINT nChar, UINT nRepCnt, UINT nFlags)
		ADDINEVENT( OnLButtonDblClk),			// OnLButtonDblClk(	CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)																		
		ADDINEVENT( OnLButtonDown),				// OnLButtonDown(	CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)				
		ADDINEVENT( OnLButtonUp),				// OnLButtonUp(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)																
		ADDINEVENT( OnLSharedDataChange),		// OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue)																		
		ADDINEVENT( OnMouseMove),				// OnMouseMove(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)		
		ADDINEVENT( OnMouseWheel),				// OnMouseWheel(	UINT nFlags, short zDelta, CPoint pt, const DWORD p_dwMouseActionID)			
		ADDINEVENT( OnPacketData),				// OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset)																
		ADDINEVENT( OnRButtonDblClk),			// OnRButtonDblClk(	CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)																		
		ADDINEVENT( OnRButtonDown),				// OnRButtonDown(	CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)																		
		ADDINEVENT( OnRButtonUp),				// OnRButtonUp(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)																
		ADDINEVENT( OnRealData_Not_Used),		// OnRealData( WORD p_wRealRecordID, WORD p_wRealItemID, const char *p_szRealKeyCode, long p_lPacketAddress, double dData)											
		ADDINEVENT( OnRealRedcordEnd_Not_Used),	// OnRealRedcordEnd( WORD p_wRealRecordID, const char *p_szRealKeyCode)												
		ADDINEVENT( OnResetChart),				// OnResetChart( void)																
		ADDINEVENT( OnSharedDataChange),		// OnSharedDataChange( const char *p_szShareName, const char *p_szSharedData, BOOL p_bStartShare)																		
		ADDINEVENT( OnTimer),					// OnTimer( UINT p_uTimerID)										
		ADDINEVENT( OnUpdatePacketData),		// OnUpdatePacketData( BOOL p_bTrUpdate)													
		// �������� ������ ���� �߰� : �������� - ojtaso (20070125)
		//ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_char_CObject),		// OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3, CObject *p_pParam)
		// RQ�� Packet : �������� - ojtaso (20070111)
		ADDINEVENT( OnPacketDataMultiItem),		// OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
		// �������� Real�ݿ� : �������� - ojtaso (20070131)
		ADDINEVENT( OnUpdatePacketDataMultiItem),	// OnUpdatePacketDataMultiItem(LPCTSTR lpszRQ, BOOL p_bTrUpdate)
		EAE_END,								// the count of Defined Type or Registered Event End (No more) Flag.
	} EAddInEventType;
public:
	virtual IAddInDllBase::EAddInEventType	GetGlobalAddInEventType( const long p_eAddInEventType) const = 0;
public:
	virtual int		RegisterEventMap( void) = 0;
	virtual int		UnregisterEventMap( void) = 0;

//////////////////////////////////////////////////////////////////////
// Event Routines
//////////////////////////////////////////////////////////////////////

public:
	// [04/10/11] ������ �����ϴ� Interface�� �����Ѵ�.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare) = 0;
	// (2005/11/21 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue) = 0;
	// (2005/8/10 - Seung-Won, Bae) use in Only Hanwha
	virtual void	OnSharedDataChange( const char *p_szShareName, const char *p_szSharedData, BOOL p_bStartShare) = 0;

	// [04/10/11] AddIn DLL�鿡�� ResetChart ��Ȳ�� �˷� ó���ǵ��� �Ѵ�.
	virtual void	OnResetChart( void) = 0;
	// [04/10/11] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawBeforeGraph( CDC *p_pDC) = 0;
	// [04/12/25] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawAfterGraph( CDC *p_pDC) = 0;
	// [04/10/13] TR�� ������ AddIn DLL�鿡�� �˷� ������� �Ѵ�.
	virtual void	OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset) = 0;
	// [04/12/07] TR�̳� Real�� ���ſ� ���� Packet Data�� Update�Ǿ����� AddIn DLL�鿡 �˷� ��ǥ���� ó����� �Ѵ�.
	virtual void	OnUpdatePacketData( BOOL p_bTrUpdate) = 0;

	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset) = 0;
	// �������� Real�ݿ� : �������� - ojtaso (20070131)
	virtual void	OnUpdatePacketDataMultiItem(LPCTSTR lpszRQ, BOOL p_bTrUpdate) = 0;

// (2004.10.29, ��¿�) AddIn DLL���� �׹��� Windows Event�� �����Ѵ�.
public:
	// [04/10/29] Timer Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual void	OnTimer( UINT p_uTimerID) = 0;
	// [04/10/29] Mouse Capture Release Event�� AddIn DLL���� �����ϵ��� �Ѵ�.
	//		����! OnCaptureChanged()�� SetCapture()/RelaseCapture()�� ���� ��� �ܿ���,
	//			Focus �� Activate, ToolTip, TrackMenu� ���ؼ��� ���÷� (SetCapture����)
	//			���� �ڱ� �ڽ��� �ٽ� SetCapture()�ϴ� ��쿡�� ȣ����� �����Ѵ�.
	virtual void	OnCaptureChanged(	CWnd *pWnd) = 0;

// (2005/2/4 - Seung-Won, Bae) Real ���� Event�� �����Ѵ�.
// (2005/8/16 - Seung-Won, Bae) Not used in Hanwha
public:
	// [04/10/14] Real�� ������ �����Ѵ�.
	//				�ӵ� ����� DRDS�� Data�� �״�� Pass�Ѵ�.
	virtual void	OnDrdsData( WPARAM p_wParam, LPARAM p_lParam) = 0;
	// [04/10/15] Real Packet�� ������ �������� AddIn DLL���� �˷� ó����� �Ѵ�.
	virtual void	OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam) = 0;
// (2005/8/16 - Seung-Won, Bae) used in Only Hanwha
public:
	// 1. Notify for Every Real Item
	virtual void	OnRealData( WORD p_wRealRecordID, WORD p_wRealItemID, const char *p_szRealKeyCode, long p_lPacketAddress, double dData) = 0;
	// 2. Notify for Every Real Record End
	virtual void	OnRealRedcordEnd( WORD p_wRealRecordID, const char *p_szRealKeyCode) = 0;

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
public:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID)	= 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam) = 0;
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const CBlockBehaviorData *p_pParam) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const CStringList *p_pParam) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const CStyleData *p_pParam) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDllBase *p_pParam) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam) = 0;
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam) = 0;
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2, const long p_lParam3, const long p_lParam4, const long p_lParam5, const long p_lParam6, const long p_lParam7, const long p_lParam8, const long p_lParam9) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam, CObject *p_pParam) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2) = 0;
	// �������� ������ ���� �߰� : �������� - ojtaso (20070125)
	//virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3, CObject *p_pParam) = 0;
public:
	virtual BOOL	OnKeyDown(			UINT nChar, UINT nRepCnt, UINT nFlags)	= 0;
	virtual BOOL	OnKeyUp(			UINT nChar, UINT nRepCnt, UINT nFlags)	= 0;
	virtual BOOL	OnMouseWheel(		UINT nFlags, short zDelta, CPoint pt, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnMouseMove(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnLButtonDown(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnLButtonUp(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnLButtonDblClk(	CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnRButtonDown(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnRButtonUp(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnRButtonDblClk(	CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)	= 0;
};

#endif // !defined(AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_)
