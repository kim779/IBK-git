// IAddInDll.h: interface for the IAddInDll class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IADDINDLL_H__BB3A45D2_5941_4D70_AAEB_1DF7AAC43A35__INCLUDED_)
#define AFX_IADDINDLL_H__BB3A45D2_5941_4D70_AAEB_1DF7AAC43A35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Include_Chart/BlockBaseEnum.h"	// for CBlockBaseData
#include "_IAddInDllBase.h"						// for IAddInDllBase

enum EAICommandType;
class CAddInManagerImp;
class CIndicatorInfo;
interface IPropertyMap;
interface IAddInManager;
interface IAddInDll : public IAddInDllBase
{
protected:
	virtual ~IAddInDll()	{};

// [04/10/07] AddIn DLL�� �ʱ�ȭ��Ű�� Interface�� �����Ѵ�.
// (2004.10.08, ��¿�) �ʱ�ȭ�ÿ� IAddInManager(ChartOcx Interface Gateway)�� �����Ͽ�
//		DLL�� AddIn Interface�� Ȯ���Ѵ�.
// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	virtual BOOL	InitAddInDll( const char *p_szAddInItem, IAddInManager *p_pIAddInManager, IAddInDll *p_pIAddInDll) = 0;

// [04/10/11] AddIn DLL�� �̸��� ��ȸ�ϴ� Interface�� �����Ѵ�.
// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	virtual const char *	GetAddInDllName( void) = 0;
	virtual const char *	GetAddInItemName( void) = 0;

// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
public:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData) = 0;
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData) = 0;	// (2005/9/2 - Seung-Won, Bae) for String Data

// [05/07/08] AddIn ���� Interface�� �����Ѵ�.
//		*. ����. �ݵ��� ���� Interface Pointer�� Converting�� IUnknown�� Pointer�� ��ȯ�Ͽ� �����Ѵ�.
public:
	virtual IUnknown *	GetAddInCustomInterface( void) = 0;
	
// (2006/2/12 - Seung-Won, Bae) Register AddIn Event Map
public:
	virtual EAddInEventType						GetGlobalAddInEventType( const long p_eAddInEventType) = 0;
	virtual IAddInDllBase::EAddInEventType *	GetAdditionalRequestEventMap( void) = 0;
public:
	virtual int		RegisterEventMap( CAddInManagerImp *p_pAddInManager, const BOOL p_bOverExclusive) = 0;
	virtual int		UnregisterEventMap( CAddInManagerImp *p_pAddInManager) = 0;

// (2007/3/4 - Seung-Won, Bae) Notify OCX Destroying to Addin for Reset OCX's Interface
public:
	virtual void	OnOcxDestroyWindow( void) = 0;

// (2009/12/2 - Seung-Won, Bae) Notify to other modules.
public:
	virtual void	ChangeChartMode( CChartInfo::CHART_MODE p_eChartMode) = 0;

//////////////////////////////////////////////////////////////////////
// Event Routines
//////////////////////////////////////////////////////////////////////

// (2004.10.11, ��¿�) AddIn DLL�� OCX Event�� �����Ѵ�.
public:
	// [04/10/11] Global ������ �����ϴ� Interface�� �����Ѵ�.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare) = 0;
	// (2005/11/21 - Seung-Won, Bae) Notify Local Shared Data Changed to AddIn
	virtual void	OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue) = 0;
	// [04/10/11] �ܺ� ToolBar�� Message�� �����Ͽ� AddIn DLL������ ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnResetChart( void) = 0;
	// [04/10/11] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawBeforeGraph( HDC p_hDC) = 0;
	// [04/12/25] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawAfterGraph( HDC p_hDC) = 0;
	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset) = 0;
	// [04/10/29] Timer Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual void	OnTimer( UINT p_uTimerID) = 0;
	// [04/10/29] Mouse Capture Release Event�� AddIn DLL���� �����ϵ��� �Ѵ�.
	//		����! OnCaptureChanged()�� SetCapture()/RelaseCapture()�� ���� ��� �ܿ���,
	//			Focus �� Activate, ToolTip, TrackMenu� ���ؼ��� ���÷� (SetCapture����)
	//			���� �ڱ� �ڽ��� �ٽ� SetCapture()�ϴ� ��쿡�� ȣ����� �����Ѵ�.
	virtual void	OnCaptureChanged( CWnd *pWnd, const HBITMAP p_hbmpMainOSB)	= 0;	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	// [04/10/14] Real�� ������ �����Ѵ�.
	//				�ӵ� ����� DRDS�� Data�� �״�� Pass�Ѵ�.
	virtual void	OnResetMainBlock( void)	= 0;
	// (2006/9/25 - Seung-Won, Bae) On User IndicatorInfo Loaded
	virtual void	OnUserIndiatorConditionLoaded( void) = 0;
	// (2006/9/25 - Seung-Won, Bae) On ChartItem Changed.
	virtual void	OnChartItemChanged( void) = 0;
	// (2006/9/25 - Seung-Won, Bae) On GraphData Changed.
	virtual void	OnGraphDataChanged( void) = 0;
	// (2006/9/25 - Seung-Won, Bae) On Block Removed.
	virtual void	OnBlockRemoved( const int p_nRow, const int p_nColumn) = 0;
	// (2006/9/25 - Seung-Won, Bae) On Graph Added.
	virtual void	OnGraphAdded( const char *p_szGraphName, const char *p_szIndicatorName) = 0;
	// (2006/9/25 - Seung-Won, Bae) On Graph Added.
	virtual void	OnManualRealData( const char *p_szPacketName, const double p_dData, const int p_nOption) = 0;
	// (2006/9/27 - Seung-Won, Bae) On Context Menu.
	virtual void	OnContextMenu( HWND p_hWnd, POINT &p_pt) = 0;
	// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
	virtual void	OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName) = 0;
	// (2006/9/27 - Seung-Won, Bae) On Packet Truncated.
	virtual void	OnPacketTruncated( const int p_nDataCount) = 0;
	// (2006/9/27 - Seung-Won, Bae) On Block Changed.
	virtual void	OnBlockChanged( const CBlockBaseData::BLOCKBEHAVIOR p_eChangeType, const int p_nSourceRow, const int p_nSourceColumn, const int p_nTargetRow, const int p_nTargetColumn) = 0;
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption) = 0;
	// (2007/2/8 - Seung-Won, Bae) On Real Data
	virtual void	OnRealData( const char *p_szPacketName, double p_dData, const char *p_szRQ) = 0;
	virtual void	OnIndexedRealData( const char *p_szPacketName, const int p_nIndex, double p_dData) = 0;
	// (2006/10/4 - Seung-Won, Bae) On Real Packet End
	virtual void	OnRealPacketEnd( const char *p_szRQ) = 0;
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap) = 0;
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap) = 0;
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC) = 0;
	// (2007/1/18 - Seung-Won, Bae) Notify Recalculation.
	virtual void	OnRecalculationComplete( void) = 0;
	// (2008/10/22 - Seung-Won, Bae) for Real Filtering
	//	Do not return FALSE. if you do, the real data can not be delivered to other AddIn and OCX.
	//	You can change the Real Data Value. it will be set to chart.
	virtual BOOL	OnPreRealData( const char *p_szPacketName, double &p_dData, const char *p_szRQ = NULL, BOOL p_bRealData = TRUE) = 0;
	virtual BOOL	OnPreIndexedRealData( const char *p_szPacketName, const int p_nIndex, double &p_dData, BOOL p_bRealData = TRUE) = 0;
	virtual BOOL	OnPreRealPacketEnd( const char *p_szRQ = NULL, BOOL p_bRealData = TRUE) = 0;
	virtual BOOL	OnPreNMTRClock( __int64 &p_nRealClockSecs) = 0;

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
public:
	virtual BOOL	OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) = 0;
	virtual BOOL	OnAddInMsg( const char *p_szAddInItemName, const long p_lParam) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID)	= 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2) = 0;
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDll *p_pParam) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam) = 0;
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam) = 0;
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2, const long p_lParam3, const long p_lParam4, const long p_lParam5, const long p_lParam6, const long p_lParam7, const long p_lParam8, const long p_lParam9) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, CIndicatorInfo *p_pParam) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, CObject *p_pParam) = 0;
	// �������� ������ ���� �߰� : �������� - ojtaso (20070125)
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3, CObject *p_pParam) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)	= 0;
public:
	virtual BOOL	OnKeyDown(						UINT nChar, UINT nRepCnt, UINT nFlags)	= 0;
	virtual BOOL	OnKeyUp(						UINT nChar, UINT nRepCnt, UINT nFlags)	= 0;
	virtual BOOL	OnMouseWheel(					UINT nFlags, short zDelta, POINT pt, const DWORD p_dwMouseActionID)		= 0;
	virtual BOOL	OnMouseMove(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)	= 0;	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	virtual BOOL	OnLButtonDown(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnLButtonUp(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnLButtonDblClk(				HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnRButtonDown(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnRButtonUp(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnRButtonDblClk(				HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
};

#endif // !defined(AFX_IADDINDLL_H__BB3A45D2_5941_4D70_AAEB_1DF7AAC43A35__INCLUDED_)
