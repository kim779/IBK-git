// AddInManagerImp.h: interface for the CAddInManagerImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDINMANAGERIMP_H__C19C6BF7_A63C_43DE_9E4C_B5DFD874DB86__INCLUDED_)
#define AFX_ADDINMANAGERIMP_H__C19C6BF7_A63C_43DE_9E4C_B5DFD874DB86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>						// for CTypedPtrList
#include "./include_Addin/_IWUnknown.h"		// for CIWUnknown
#include "./include_Addin/_IAddInManager.h"	// for IAddInManager
#include "./include_Addin/IAddInDll.h"		// for IAddInDll

typedef CTypedPtrList<CPtrList, IAddInDll *> CListIAddInDll;
#define LIST_ADDINEVENT( OnAddInEvent)			m_lIAddInDll_##OnAddInEvent
#define LIST_ADDINEVENT_OVER( OnAddInEvent)		m_lIAddInDll_##OnAddInEvent##_Over
#define LIST_ADDINEVENT_UNDER( OnAddInEvent)	m_lIAddInDll_##OnAddInEvent##_Under

//////////////////////////////////////////////////////////////////////
// (2004.10.06, ��¿�) OCX AddIn DLL�� �����ϰ� OCX�� ���� Object Interface�� �����ϴ� �⺻ Manager Object Class�� �����Ѵ�.
//	[Interface ���� ��� ����]
//		1. Object�� Interface�� IUnknown Base�� Interface�� �����Ѵ�.
//		2. Ư�� Version�� ������ ���Ͽ� Interface�� �̸��� ����� �� 2�ڸ��� YYMMDD�� �ٿ�,
//			Version�� ����ȭ�Ѵ�.
//		3. �̷��� Version�� Interface�� File���� �����Ͽ� DLL�ʿ��� �ֽ��� Inteface File�� Include�� �� �ֵ��� �����Ѵ�.
//		4. Interface�� ������ Interface�� Wrapper Object�� �ΰ� Wrapper�� Interface�� ���������ν�,
//			������ ����Ǵ� Interface Version�� ��� �� Object�� ������ ������ Object�� Inteface�� �и��Ѵ�.
//		5. Inteface Folder�� �����ÿ��� ������ File�� �ٸ� Interface Folder�� �ű��,
//			���ο� Interface�� ���ο� Interface�� �̸����� ������ �����Ѵ�.
//		6. �� Interface Header���� Interface ���� �ؼ��������� �������� ����� ����Ѵ�.
//	[AddIn DLL Interface ���ٵ�]
//		Chart																				|	Share						|	DLL
//		AddInManager	-> IAddInDll(Base Interface)	-> CIWAddInDllYYMMDD (IWrapper)		-> IChartAddInYYMMDD			-> CChartAddInBase
//																								(with IChartAddIn Marco)
//	[Chart Object Interface ���ٵ�]
//		Chart													|	Share												|	Chart									|	Share
//		ChartObject		<- CIWChartObjectYYMMDD (IWrapper)		<- IChartObjectYYMMDD									<- CIWChartOcxYYMMDD 						<- IChartOcxYYMMDD			<- IAddInManager (ChartOcx Interface Gateway)
//																	(Chart Object Interface with IChartObject Macro)		(IWrapper with Site Standard Macro OCX)		(with IChartOcx Macro)

// (2005/12/6 - Seung-Won, Bae) AddIn Tool's Z-Order
//		1.	Rule I		: AddIn Order is Event Notify Order. So Head is Z-Top, Tail is Z-Bottom.
//		2.	Rule II		: So AddIn must be Add in Head.
//		3.	Rule III	: and Drawing Order is reverse of AddIn Order (Tail First)
//		4.	Rule IV		: LoadAddIn of loaded AddIn make it to be Top. 

class CPropertyMap;
class CIndicatorInfo;
interface IChartCtrl;
class CAddInManagerImp : public CIWUnknown, public IAddInManager
{
// (2004.10.07, ��¿�) Interface Wrapper�� �⺻ Interface (IUnknown��)�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2004.10.07, ��¿�) Chart OCX�� Interface�� ���ϴ� Interface�� �����Ѵ�.
// Not Used
protected:
	virtual IChartOCX423101 *	GetIChartOCX423101( void)	{	return NULL;	}	// DB - CATA
	virtual IChartOCX424221 *	GetIChartOCX424221( void)	{	return NULL;	}	// DB - DLVA
	virtual IChartOCX424661 *	GetIChartOCX424661( void)	{	return NULL;	}	// DB - DPPA
	virtual IChartOCX425914 *	GetIChartOCX425914( void)	{	return NULL;	}	// DB - ESUD
	virtual IChartOCX839091 *	GetIChartOCX839091( void)	{	return NULL;	}	// HW - STIA
	virtual IChartOCX913101 *	GetIChartOCX913101( void)	{	return NULL;	}	// SK - CATA
	virtual IChartOCX913951 *	GetIChartOCX913951( void)	{	return NULL;	}	// SK - CSOA
	virtual IChartOCX914221 *	GetIChartOCX914221( void)	{	return NULL;	}	// SK - DLVA
	virtual IChartOCX914661 *	GetIChartOCX914661( void)	{	return NULL;	}	// SK - DPPA
	virtual IChartOCX915914 *	GetIChartOCX915914( void)	{	return NULL;	}	// SK - ESUD
	virtual IChartOCX999091 *	GetIChartOCX999091( void)	{	return NULL;	}	// SS - STIA
	virtual IChartOCX429091 *	GetIChartOCX429091( void)	{	return NULL;	}	// DB - STIA
	virtual IChartOCX919091 *	GetIChartOCX919091( void)	{	return NULL;	}	// SK - STIA
	virtual IChartOCX139091 *	GetIChartOCX139091( void)	{	return NULL;	}	// SK - STIA
	virtual IChartOCX993101 *	GetIChartOCX993101( void)	{	return NULL;	}	// SS - CATA
	virtual IChartOCX994221 *	GetIChartOCX994221( void)	{	return NULL;	}	// SS - DLVA
	virtual IChartOCX994661 *	GetIChartOCX994661( void)	{	return NULL;	}	// SS - DPPA
	virtual IChartOCX995914 *	GetIChartOCX995914( void)	{	return NULL;	}	// SS - ESUD
	virtual IChartOCX993691 *	GetIChartOCX993691( void)	{	return NULL;	}	// SS - CPSA
	virtual IChartOCX993521 *	GetIChartOCX993521( void)	{	return NULL;	}	// SS - COLA
	virtual IChartOCX999092 *	GetIChartOCX999092( void)	{	return NULL;	}	// SS - STIA2
	virtual IChartOCX133521 *	GetIChartOCX133521( void)	{	return NULL;	}	// KC - COLA
// Common
protected:
	virtual IChartOCX000000 *	GetIChartOCX000000( void);	// Chart Common AddIn
	virtual IChartOCX003531 *	GetIChartOCX003531( void);	// CO - COCA	// Addin�и� - onlyojt (20100305)
	virtual IChartOCX003721 *	GetIChartOCX003721( void);	// SM - COCA
	virtual IChartOCX423611 *	GetIChartOCX423611( void);	// DB - CPAA
	virtual IChartOCX133721 *	GetIChartOCX133721( void);	// KC - CGLA
	virtual IChartOCX333721 *	GetIChartOCX333721( void);	// WW - CGLA
	virtual IChartOCX363721 *	GetIChartOCX363721( void);	// WF - CGLA
	virtual IChartOCX503721 *	GetIChartOCX503721( void);	// EG - CGLA
protected:
	IAddInDll *		GetCIWAddInDll000000( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn);	// Chart Common AddIn
	// Addin�и� - onlyojt (20100305)
	IAddInDll *		GetCIWAddInDll003531( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn);	// Chart Common AddIn
	IAddInDll *		GetCIWAddInDll003721( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn);	// 
	IAddInDll *		GetCIWAddInDll423611( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn);	// DB - CPAA
	IAddInDll *		GetCIWAddInDll133721( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn);	// KC - CGLA
	IAddInDll *		GetCIWAddInDll333721( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn);	// WW - CGLA
	IAddInDll *		GetCIWAddInDll363721( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn);	// WF - CGLA
	IAddInDll *		GetCIWAddInDll503721( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn);	// EG - CGLA

// Koscom
#ifdef _AIM_KOSCOM
protected:
	virtual IChartOCX133101 *	GetIChartOCX133101( void);	// KC - CATA
	virtual IChartOCX134221 *	GetIChartOCX134221( void);	// KC - DLVA
	virtual IChartOCX134661 *	GetIChartOCX134661( void);	// KC - DPPA
	virtual IChartOCX135914 *	GetIChartOCX135914( void);	// KC - ESUD
	virtual IChartOCX139092 *	GetIChartOCX139092( void);	// KC - STIA
	virtual IChartOCX133691 *	GetIChartOCX133691( void);	// KC - CPSA
	virtual IChartOCX381931 *	GetIChartOCX381931( void);	// WR - KSCA
	virtual IChartOCX339092 *	GetIChartOCX339092( void);	// WW - STIA
	virtual IChartOCX509092 *	GetIChartOCX509092( void);	// EG - STIA
protected:
	IAddInDll *		GetCIWAddInDll133101( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn);	// KC - CATA
	IAddInDll *		GetCIWAddInDll134221( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn);	// KC - DLVA
	IAddInDll *		GetCIWAddInDll134661( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn);	// KC - DPPA
	IAddInDll *		GetCIWAddInDll135914( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn);	// KC - ESUD
	IAddInDll *		GetCIWAddInDll139092( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn);	// KC - STIA
	IAddInDll *		GetCIWAddInDll133691( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn);	// KC - CPSA
	IAddInDll *		GetCIWAddInDll381931( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn);	// WR - KSCA
	IAddInDll *		GetCIWAddInDll339092( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn);	// WW - STIA
	IAddInDll *		GetCIWAddInDll509092( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn);	// EG - STIA
#else
protected:
	virtual IChartOCX133101 *	GetIChartOCX133101( void)	{	return NULL;	}	// KC - CATA
	virtual IChartOCX134221 *	GetIChartOCX134221( void)	{	return NULL;	}	// KC - DLVA
	virtual IChartOCX134661 *	GetIChartOCX134661( void)	{	return NULL;	}	// KC - DPPA
	virtual IChartOCX135914 *	GetIChartOCX135914( void)	{	return NULL;	}	// KC - ESUD
	virtual IChartOCX139092 *	GetIChartOCX139092( void)	{	return NULL;	}	// KC - STIA
	virtual IChartOCX133691 *	GetIChartOCX133691( void)	{	return NULL;	}	// KC - CPSA
	virtual IChartOCX381931 *	GetIChartOCX381931( void)	{	return NULL;	}	// WR - KSCA
	virtual IChartOCX339092 *	GetIChartOCX339092( void)	{	return NULL;	}	// WW - STIA
	virtual IChartOCX509092 *	GetIChartOCX509092( void)	{	return NULL;	}	// EG - STIA
#endif

#ifdef _DEBUG
// (2006/8/7 - Seung-Won, Bae) AddIn Version Check Counter
protected:
	BOOL	m_bVCheck000000;	// Chart Common AddIn
	BOOL	m_bVCheck003531;	// Chart Common AddIn	// Addin�и� - onlyojt (20100305)
	BOOL	m_bVCheck003721;	//
	BOOL	m_bVCheck133721;	// KC - CGLA
	BOOL	m_bVCheck333721;	// WW - CGLA
	BOOL	m_bVCheck363721;	// WF - CGLA
	BOOL	m_bVCheck503721;	// EG - CGLA
	BOOL	m_bVCheck423611;	// DB - CPAA

	BOOL	m_bVCheck133101;	// KC - CATA
	BOOL	m_bVCheck134221;	// KC - DLVA
	BOOL	m_bVCheck134661;	// KC - DPPA
	BOOL	m_bVCheck135914;	// KC - ESUD
	BOOL	m_bVCheck139092;	// KC - STIA
	BOOL	m_bVCheck133691;	// KC - CPSA
	BOOL	m_bVCheck381931;	// WR - KSCA
	BOOL	m_bVCheck339092;	// WW - STIA
	BOOL	m_bVCheck509092;	// EG - STIA
#endif

public:
	CAddInManagerImp( IChartCtrl *p_pIChartCtrl, const char *p_szOcxPath);	// (2006/1/18 - Seung-Won, Bae) for Load AddIn DLL in OCX Path
	virtual ~CAddInManagerImp();

// (2006/1/7 - Seung-Won, Bae) Manage Chart Control Interface for Access OCX
protected:
	IChartCtrl *	m_pIChartCtrl;

// (2006/1/18 - Seung-Won, Bae) for Load AddIn DLL in OCX Path
protected:
	CString			m_strOcxPath;

// (2004.10.11, ��¿�) AddIn DLL�� �����Ѵ�.
protected:
	CListIAddInDll	m_lIAddInDll;
// (2006/2/12 - Seung-Won, Bae) Register/Unregister AddIn Event Map
protected:
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType);
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_HDC_UINT_POINT_DWORD);
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_char);
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_char_char);
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_char_long);
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_IAddInDllBase);
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_long);
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char);
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char_char_char);
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long);
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long_long_long_long_long_long_long_long);
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType);
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_IGraphInfo);
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char);
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_CObject);
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long);
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long_long);
	CListIAddInDll	LIST_ADDINEVENT( OnCaptureChanged);
	CListIAddInDll	LIST_ADDINEVENT( OnCmdMsg);
	CListIAddInDll	LIST_ADDINEVENT( OnDrawAfterGraph);
	CListIAddInDll	LIST_ADDINEVENT( OnDrawBeforeGraph);
	CListIAddInDll	LIST_ADDINEVENT( OnGSharedDataChange);
	CListIAddInDll	LIST_ADDINEVENT_OVER(	OnKeyDown);
	CListIAddInDll	LIST_ADDINEVENT_UNDER(	OnKeyDown);
	CListIAddInDll	LIST_ADDINEVENT_OVER(	OnKeyUp);
	CListIAddInDll	LIST_ADDINEVENT_UNDER(	OnKeyUp);
	CListIAddInDll	LIST_ADDINEVENT_OVER(	OnLButtonDblClk);
	CListIAddInDll	LIST_ADDINEVENT_UNDER(	OnLButtonDblClk);
	CListIAddInDll	LIST_ADDINEVENT_OVER(	OnLButtonDown);
	CListIAddInDll	LIST_ADDINEVENT_UNDER(	OnLButtonDown);
	CListIAddInDll	LIST_ADDINEVENT_OVER(	OnLButtonUp);
	CListIAddInDll	LIST_ADDINEVENT_UNDER(	OnLButtonUp);
	CListIAddInDll	LIST_ADDINEVENT( OnLSharedDataChange);
	CListIAddInDll	LIST_ADDINEVENT_OVER(	OnMouseMove);
	CListIAddInDll	LIST_ADDINEVENT_UNDER(	OnMouseMove);
	CListIAddInDll	LIST_ADDINEVENT_OVER(	OnMouseWheel);
	CListIAddInDll	LIST_ADDINEVENT_UNDER(	OnMouseWheel);
	CListIAddInDll	LIST_ADDINEVENT_OVER(	OnRButtonDblClk);
	CListIAddInDll	LIST_ADDINEVENT_UNDER(	OnRButtonDblClk);
	CListIAddInDll	LIST_ADDINEVENT_OVER(	OnRButtonDown);
	CListIAddInDll	LIST_ADDINEVENT_UNDER(	OnRButtonDown);
	CListIAddInDll	LIST_ADDINEVENT_OVER(	OnRButtonUp);
	CListIAddInDll	LIST_ADDINEVENT_UNDER(	OnRButtonUp);
	CListIAddInDll	LIST_ADDINEVENT( OnResetChart);
	CListIAddInDll	LIST_ADDINEVENT( OnTimer);
	CListIAddInDll	LIST_ADDINEVENT( OnResetMainBlock);
	CListIAddInDll	LIST_ADDINEVENT( OnUserIndiatorConditionLoaded);
	CListIAddInDll	LIST_ADDINEVENT( OnChartItemChanged);
	CListIAddInDll	LIST_ADDINEVENT( OnGraphDataChanged);
	CListIAddInDll	LIST_ADDINEVENT( OnBlockRemoved);
	CListIAddInDll	LIST_ADDINEVENT( OnGraphAdded);
	CListIAddInDll	LIST_ADDINEVENT( OnManualRealData);
	CListIAddInDll	LIST_ADDINEVENT( OnContextMenu);
	CListIAddInDll	LIST_ADDINEVENT( OnGraphDeleted);
	CListIAddInDll	LIST_ADDINEVENT( OnPacketTruncated);
	CListIAddInDll	LIST_ADDINEVENT( OnBlockChanged);
	CListIAddInDll	LIST_ADDINEVENT( OnToolCommand);
	CListIAddInDll	LIST_ADDINEVENT( OnRealData);
	CListIAddInDll	LIST_ADDINEVENT( OnIndexedRealData);
	CListIAddInDll	LIST_ADDINEVENT( OnRealPacketEnd);
	CListIAddInDll	LIST_ADDINEVENT( OnAddInMsg);
	CListIAddInDll	LIST_ADDINEVENT( OnLoadEnvironment);
	CListIAddInDll	LIST_ADDINEVENT( OnSaveEnvironment);
	CListIAddInDll	LIST_ADDINEVENT( DrawCursor);
	CListIAddInDll	LIST_ADDINEVENT( OnRecalculationComplete);
	// �������� ������ ���� �߰� : �������� - ojtaso (20070125)
	CListIAddInDll	LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_char_CObject);
	// RQ�� Packet : �������� - ojtaso (20070111)
	CListIAddInDll	LIST_ADDINEVENT( OnPacketDataMultiItem);
	// (2008/10/22 - Seung-Won, Bae) for Real Filtering
	//	Do not return FALSE. if you do, the real data can not be delivered to other AddIn and OCX.
	CListIAddInDll	LIST_ADDINEVENT( OnPreRealData);
	CListIAddInDll	LIST_ADDINEVENT( OnPreIndexedRealData);
	CListIAddInDll	LIST_ADDINEVENT( OnPreRealPacketEnd);
	CListIAddInDll	LIST_ADDINEVENT( OnPreNMTRClock);
	CListIAddInDll	LIST_ADDINEVENT( OnUpdatePacketData);
protected:
	CListIAddInDll *	GetEventMapAddInList( const IAddInDll::EAddInEventType p_eAddInEventType, const BOOL p_bOverExclusive);//
public:
	virtual BOOL	RegisterEventMap( const long p_lAddInEventType, IAddInDll *p_pIAddInDll, const BOOL p_bOverExclusive, const BOOL p_bAddtional = FALSE);
	virtual BOOL	UnregisterEventMap( const long p_lAddInEventType, IAddInDll *p_pIAddInDll, const BOOL p_bOverExclusive, const BOOL p_bAddtional = FALSE);

// (2006/1/17 - Seung-Won, Bae) Keyboard & Mouse Exclusive Setting
protected:
	IAddInDll *		m_pIAddInDll_KeyboardExclusive;
	IAddInDll *		m_pIAddInDll_LButtonExclusive;
	IAddInDll *		m_pIAddInDll_RButtonExclusive;
	IAddInDll *		m_pIAddInDll_MouseMoveExclusive;
	IAddInDll *		m_pIAddInDll_MouseWheelExclusive;
public:
	virtual BOOL	GetKeyboardExclusive(		IAddInDll *p_pIAddInDll);
	virtual BOOL	IsKeyboardExclusive(		IAddInDll *p_pIAddInDll);
	virtual BOOL	ReleaseKeyboardExclusive(	IAddInDll *p_pIAddInDll);
public:
	virtual BOOL	GetAllMouseExclusive(		IAddInDll *p_pIAddInDll);	// return TRUE in one more Exclusive Created
	virtual BOOL	IsAllMouseExclusive(		IAddInDll *p_pIAddInDll);	// return TRUE in one more Exclusive Setted
	virtual BOOL	ReleaseAllMouseExclusive(	IAddInDll *p_pIAddInDll);	// return TRUE in one more Exclusive Released
public:
	virtual BOOL	GetLButtonExclusive(		IAddInDll *p_pIAddInDll);
	virtual BOOL	IsLButtonExclusive(			IAddInDll *p_pIAddInDll);
	virtual BOOL	ReleaseLButtonExclusive(	IAddInDll *p_pIAddInDll);
public:
	virtual BOOL	GetRButtonExclusive(		IAddInDll *p_pIAddInDll);
	virtual BOOL	IsRButtonExclusive(			IAddInDll *p_pIAddInDll);
	virtual BOOL	ReleaseRButtonExclusive(	IAddInDll *p_pIAddInDll);
public:
	virtual BOOL	GetMouseMoveExclusive(		IAddInDll *p_pIAddInDll);
	virtual BOOL	IsMouseMoveExclusive(		IAddInDll *p_pIAddInDll);
	virtual BOOL	ReleaseMouseMoveExclusive(	IAddInDll *p_pIAddInDll);
public:
	virtual BOOL	GetMouseWheelExclusive(		IAddInDll *p_pIAddInDll);
	virtual BOOL	IsMouseWheelExclusive(		IAddInDll *p_pIAddInDll);
	virtual BOOL	ReleaseMouseWheelExclusive(	IAddInDll *p_pIAddInDll);

// (2004.10.11, ��¿�) AddIn DLL�� �����Ѵ�.
protected:
	// (2004.10.11, ��¿�) AddIn Object�� Interface�� ��ȸ�Ͽ� OCX���� ���� Access�� �� �ֵ��� �����Ѵ�.
	IAddInDll *	GetIAddInDll( const char *p_szAddInModule, const char *p_szAddInItem);
	// (2006/2/8 - Seung-Won, Bae) Check Already Loaded.
	IAddInDll *	GetIAddInDllFromList( CListIAddInDll &p_lIAddInDll, const char *p_szAddInModule, const char *p_szAddInItem, BOOL p_bWithPop = FALSE);
	// (2006/2/14 - Seung-Won, Bae) Multiple AddIn Item DLL Version
	BOOL	LoadAddInItem( const char *p_szAddInModule, const char *p_szAddInItem, const BOOL p_bOverExclusive);
	BOOL	UnloadAddInItem( const char *p_szAddInModule, const char *p_szAddInItem);
	BOOL	LoadMultipleAddInItem( const char *p_szAddInModule, const BOOL p_bOverExclusive);
	BOOL	UnloadMultipleAddInItem( const char *p_szAddInModule);
	BOOL	InvokeAddInItem( const char *p_szAddInDllName, const char *p_szAddInItem, short p_nCommandType, long p_lData);
	BOOL	InvokeAddInItemStr( const char *p_szAddInDllName, const char *p_szAddInItem, const char *p_szCommandName, LPCTSTR p_szData);
public:
	// (2006/2/14 - Seung-Won, Bae) AddIn Loading with Multiple.
	BOOL	LoadMultipleAddInDll( const char *p_szAddInModule, const BOOL p_bOverExclusive);
	// (2005/10/25 - Seung-Won, Bae) Unload AddIn DLL
	BOOL	UnloadMultipleAddInDll( const char *p_szAddInModule);
	// (2004.10.11, ��¿�) Addin DLL�� �ϰ� Unloading�ϴ� Interface�� �����Ѵ�.
	//		AddIn Unload�ÿ��� �������� OCX�� Access�� �� �ֵ��� Window Destroy Time���� �ϰ� Unload�ϵ��� �Ѵ�.
	void	UnloadAllAddin( void);
	// (2006/1/7 - Seung-Won, Bae) Interface for Spacial Interface of AddIn DLL
	IUnknown *	GetAddInCustomInterface( const char *p_szAddInModule);
	// (2006/8/25 - Seung-Won, Bae) Check AddIn Loaded (DLL:IDS)
	BOOL	IsAddInLoaded( const char *p_szAddInItem);
	// (2006/1/7 - Seung-Won, Bae) Set Command to the AddIn DLL
	BOOL	InvokeAddIn( const char *p_szAddInDllName, short p_nCommandType, long p_lData);
	BOOL	InvokeAddInStr( const char *p_szAddInDllName, const char *p_szCommandName, LPCTSTR p_szData);
	// (2007/3/4 - Seung-Won, Bae) Notify OCX Destroying to Addin
	void	OnOcxDestroyWindow( void);
	// (2009/12/2 - Seung-Won, Bae) Notify to other modules.
	void	ChangeChartMode( CChartInfo::CHART_MODE p_eChartMode);

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
	void	OnDrawBeforeGraph( HDC p_hDC);
	// (2004.12.25, ��¿�) Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	void	OnDrawAfterGraph( HDC p_hDC);
	// RQ�� Packet : �������� - ojtaso (20070111)
	void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2004.10.29, ��¿�) Timer Event�� ó���ϴ� Interface�� �����Ѵ�.
	void	OnTimer( UINT p_uTimerID);
	// (2004.10.29, ��¿�) Mouse Capture Release Event�� AddIn DLL���� �����ϵ��� �Ѵ�.
	//		����! OnCaptureChanged()�� SetCapture()/RelaseCapture()�� ���� ��� �ܿ���,
	//			Focus �� Activate, ToolTip, TrackMenu� ���ؼ��� ���÷� (SetCapture����)
	//			���� �ڱ� �ڽ��� �ٽ� SetCapture()�ϴ� ��쿡�� ȣ����� �����Ѵ�.
	void	OnCaptureChanged( CWnd *pWnd, const HBITMAP p_hbmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
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
	void	OnContextMenu( HWND p_hWnd, POINT &p_pt);
	// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
	void	OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName);
	// (2006/9/27 - Seung-Won, Bae) On Packet Truncated.
	void	OnPacketTruncated( const int p_nDataCount);
	// (2006/9/27 - Seung-Won, Bae) On Block Changed.
	void	OnBlockChanged( const CBlockBaseData::BLOCKBEHAVIOR p_eChangeType, const int p_nSourceRow, const int p_nSourceColumn, const int p_nTargetRow, const int p_nTargetColumn);
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);
	// (2007/2/8 - Seung-Won, Bae) On Real Data
	void	OnRealData( const char *p_szPacketName, double p_dData, const char *p_szRQ = NULL);
	void	OnIndexedRealData( const char *p_szPacketName, const int p_nIndex, double p_dData);
	// (2006/10/4 - Seung-Won, Bae) On Real Packet End
	void	OnRealPacketEnd( const char *p_szRQ = NULL);
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	void	OnLoadEnvironment( CPropertyMap *p_pAddInEnvMap);
	void	OnSaveEnvironment( CPropertyMap *p_pAddInEnvMap);
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	CRect	m_rctCursorArea;
	void	ClearCursor( CDC *p_pDC, const CBitmap *p_bmpMainOSB, CRect *p_pRect = NULL);
	void	DrawCursor( CDC *p_pDC);
	// (2007/1/18 - Seung-Won, Bae) Notify Recalculation.
	void	OnRecalculationComplete( void);
	// (2008/10/22 - Seung-Won, Bae) for Real Filtering
	//	Do not return FALSE. if you do, the real data can not be delivered to other AddIn and OCX.
	//	You can change the Real Data Value. it will be set to chart.
	BOOL	OnPreRealData( const char *p_szPacketName, double &p_dData, const char *p_szRQ = NULL, BOOL p_bRealData = TRUE);
	BOOL	OnPreIndexedRealData( const char *p_szPacketName, const int p_nIndex, double &p_dData, BOOL p_bRealData = TRUE);
	BOOL	OnPreRealPacketEnd( const char *p_szRQ = NULL, BOOL p_bRealData = TRUE);
	BOOL	OnPreNMTRClock( __int64 &p_nRealClockSecs);
	void	OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset);
		// [04/12/07] TR�̳� Real�� ���ſ� ���� Packet Data�� Update�Ǿ����� AddIn DLL�鿡 �˷� ��ǥ���� ó����� �Ѵ�.
	void	OnUpdatePacketData( BOOL p_bTrUpdate);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
public:
	BOOL	OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	BOOL	OnAddInMsg( const char *p_szAddInItemName, const long p_lParam);
	BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID);
	BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
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
	BOOL	OnKeyDown(						UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL	OnKeyUp(						UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL	OnMouseWheel(					UINT nFlags, short zDelta, POINT pt, const DWORD p_dwMouseActionID);
	BOOL	OnMouseMove(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	BOOL	OnLButtonDown(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	BOOL	OnLButtonUp(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	BOOL	OnLButtonDblClk(				HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	BOOL	OnRButtonDown(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	BOOL	OnRButtonUp(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	BOOL	OnRButtonDblClk(				HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_ADDINMANAGERIMP_H__C19C6BF7_A63C_43DE_9E4C_B5DFD874DB86__INCLUDED_)
