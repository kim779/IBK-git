// AddInManager.h: interface for the CAddInManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDINMANAGER_H__B2AFE33F_01C0_4998_A6B8_3DF0F97FB79F__INCLUDED_)
#define AFX_ADDINMANAGER_H__B2AFE33F_01C0_4998_A6B8_3DF0F97FB79F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../BlockBaseEnum.h"		// for CBlockBaseData::BLOCKBEHAVIOR

// (2006/8/18 - Seung-Won, Bae) for Using this header in Ixxxxxx.lib
#ifndef AFX_EXT_CLASS_AIM
	#ifdef _AIM_EXPORT				// in AddInManager
		#define AFX_EXT_CLASS_AIM	AFX_CLASS_EXPORT
	#elif defined _AIM_NOT_IMPORT	// in Ixxxxxx
		#define AFX_EXT_CLASS_AIM			
	#else							// in Other, To Link
		#define AFX_EXT_CLASS_AIM	AFX_CLASS_IMPORT
	#endif
#endif

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/1/7
// Comments		: AddInManager Interface Class
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

enum EAICommandType;
class CPropertyMap;
class CIndicatorInfo;
class CAddInManagerImp;
interface IAddInDll;
interface IChartCtrl;
class AFX_EXT_CLASS_AIM CAddInManager : public CObject
{
public:
	CAddInManager( IChartCtrl *p_pIChartCtrl, const char *p_szOcxPath);	// (2006/1/18 - Seung-Won, Bae) for Load AddIn DLL in OCX Path
	virtual ~CAddInManager();

protected:
	CAddInManagerImp *	m_pAddInManagerImpl;

// (2004.10.11, ��¿�) AddIn DLL�� �����Ѵ�.
public:
	// (2004.10.07, ��¿�) Addin DLL�� Loading�ϴ� Interface�� �����Ѵ�.
	// (2004.10.13, ��¿�) AddIn DLL Loading�� �ٷ� AddIn Interface�� �����Ѵ�.
	// (2005.01.18, ��¿�) AddIn Object�� Interface�� �������� �ʰ�, DLL Wrapper�� �����Ͽ�,
	//		������ ������ �����ϵ��� ó���Ѵ�.
	BOOL		LoadAddIn( const char *p_szAddInModule, const BOOL p_bOverExclusive = FALSE);
	// (2005/10/25 - Seung-Won, Bae) Unload AddIn DLL
	BOOL		UnloadAddIn( const char *p_szAddInModule);
	// (2004.10.11, ��¿�) Addin DLL�� �ϰ� Unloading�ϴ� Interface�� �����Ѵ�.
	//		AddIn Unload�ÿ��� �������� OCX�� Access�� �� �ֵ��� Window Destroy Time���� �ϰ� Unload�ϵ��� �Ѵ�.
	void		UnloadAllAddin( void);
	// (2006/1/7 - Seung-Won, Bae) Interface for Spacial Interface of AddIn DLL
	IUnknown *	GetAddInCustomInterface( const char *p_szAddInModule);
	// (2006/8/25 - Seung-Won, Bae) Check AddIn Loaded (DLL:IDS)
	BOOL		IsAddInLoaded( const char *p_szAddInItem);
	// (2006/1/7 - Seung-Won, Bae) Set Command to the AddIn DLL
	BOOL		InvokeAddIn( const char *p_szAddInDllName, short p_nCommandType, long p_lData);
	BOOL		InvokeAddInStr( const char *p_szAddInDllName, const char *p_szCommandName, LPCTSTR p_szData);
	// (2007/3/4 - Seung-Won, Bae) Notify OCX Destroying to Addin for Reset OCX's Interface
	void		OnOcxDestroyWindow( void);
	// (2009/12/2 - Seung-Won, Bae) Notify to other modules.
	void		ChangeChartMode( CChartInfo::CHART_MODE p_eChartMode);

// (2004.10.11, ��¿�) AddIn DLL�� OCX Event�� �����Ѵ�.
public:
	// (2004.10.11, ��¿�) Global ������ �����ϴ� Interface�� �����Ѵ�.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare = FALSE);
	// (2005/11/21 - Seung-Won, Bae) Notify Local Shared Data Changed to AddIn
	void	OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue);
	// (2004.10.11, ��¿�) AddIn DLL�鿡�� ResetChart ��Ȳ�� �˷� ó���ǵ��� �Ѵ�.
	void	OnResetChart( void);
	// (2004.10.11, ��¿�) Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	void	OnDrawBeforeGraph( CDC *p_pDC);
	// (2004.12.25, ��¿�) Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	void	OnDrawAfterGraph( CDC *p_pDC);
	// RQ�� Packet : �������� - ojtaso (20070111)
	void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2004.10.29, ��¿�) Timer Event�� ó���ϴ� Interface�� �����Ѵ�.
	void	OnTimer( UINT p_uTimerID);
	// (2004.10.29, ��¿�) Mouse Capture Release Event�� AddIn DLL���� �����ϵ��� �Ѵ�.
	//		����! OnCaptureChanged()�� SetCapture()/RelaseCapture()�� ���� ��� �ܿ���,
	//			Focus �� Activate, ToolTip, TrackMenu� ���ؼ��� ���÷� (SetCapture����)
	//			���� �ڱ� �ڽ��� �ٽ� SetCapture()�ϴ� ��쿡�� ȣ����� �����Ѵ�.
	void	OnCaptureChanged( CWnd *pWnd, const CBitmap *p_bmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	// (2006/9/25 - Seung-Won, Bae) On Reset MainBlock
	void	OnResetMainBlock( void);
	// (2006/9/25 - Seung-Won, Bae) On User IndicatorInfo Loaded
	void	OnUserIndiatorConditionLoaded( void);
	// (2006/9/25 - Seung-Won, Bae) On ChartItem Changed.
	void	OnChartItemChanged( void);
	// (2006/9/25 - Seung-Won, Bae) On GraphData Changed.
	void	OnGraphDataChanged( void);
	// (2006/9/25 - Seung-Won, Bae) On Block Removed.
	void	OnBlockRemoved( const int p_nRow, const int p_nColumn);
	// (2006/9/25 - Seung-Won, Bae) On Graph Added.
	void	OnGraphAdded( const char *p_szGraphName, const char *p_szIndicatorName);
	// (2006/9/25 - Seung-Won, Bae) On Graph Added.
	void	OnManualRealData( const char *p_szPacketName, const double p_dData, const int p_nOption);
	// (2006/9/27 - Seung-Won, Bae) On Context Menu.
	void	OnContextMenu( CWnd *p_pWnd, CPoint &p_pt);
	// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
	void	OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName);
	// (2006/9/27 - Seung-Won, Bae) On Packet Truncated.
	void	OnPacketTruncated( const int p_nDataCount);
	// (2006/9/27 - Seung-Won, Bae) On Block Changed.
	void	OnBlockChanged( const CBlockBaseData::BLOCKBEHAVIOR p_eChangeType, const int p_nSourceRow, const int p_nSourceColumn, const int p_nTargetRow, const int p_nTargetColumn);
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	void	OnLoadEnvironment( CPropertyMap *p_pAddInEnvMap);
	void	OnSaveEnvironment( CPropertyMap *p_pAddInEnvMap);
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	void	ClearCursor( CDC *p_pDC = NULL, const CBitmap *p_bmpMainOSB = NULL, CRect *p_pRect = NULL);
	void	DrawCursor( CDC *p_pDC);
	// (2007/1/18 - Seung-Won, Bae) Notify Recalculation.
	void	OnRecalculationComplete( void);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
public:
	BOOL	OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	BOOL	OnAddInMsg( const char *p_szAddInItemName, const long p_lParam);
	BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID);
	BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID);
	BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam);
	BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2);
	BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);
	BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDll *p_pParam);
	BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam);
	BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3);
	BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2);
	BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2, const long p_lParam3, const long p_lParam4, const long p_lParam5, const long p_lParam6, const long p_lParam7, const long p_lParam8, const long p_lParam9);
	BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID);
	BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, CIndicatorInfo *p_pParam);
	BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam);
	BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, CObject *p_pParam);
	// �������� ������ ���� �߰� : �������� - ojtaso (20070125)
	BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3, CObject *p_pParam);
	BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam);
	BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2);
public:
	BOOL	OnKeyDown(			UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL	OnKeyUp(			UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL	OnMouseWheel(		UINT nFlags, short zDelta, CPoint pt, const DWORD p_dwMouseActionID);
	BOOL	OnMouseMove(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID, const CBitmap *p_bmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	BOOL	OnLButtonDown(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID);
	BOOL	OnLButtonUp(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID);
	BOOL	OnLButtonDblClk(	CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID);
	BOOL	OnRButtonDown(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID);
	BOOL	OnRButtonUp(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID);
	BOOL	OnRButtonDblClk(	CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_ADDINMANAGER_H__B2AFE33F_01C0_4998_A6B8_3DF0F97FB79F__INCLUDED_)
