// ChartStyleDataManagerAddInImp.h: interface for the CChartStyleDataManagerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTSTYLEDATAMANAGERADDINIMP_H__A8F01B76_4AB8_443E_BDA3_78C34280CE89__INCLUDED_)
#define AFX_CHARTSTYLEDATAMANAGERADDINIMP_H__A8F01B76_4AB8_443E_BDA3_78C34280CE89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/IStyleManager.h"			// for IStyleManager
#include "Include_AddIn.h"							// for Ixxxxxx.h
#include "StyleDataManager.h"						// for CStyleDataManager

class CChartStyleDataManagerAddInImp : public CChartAddInBase, public IStyleManager
{
public:
	CChartStyleDataManagerAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartStyleDataManagerAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// [05/07/08] AddIn ���� Interface�� �����Ѵ�.
//		*. ����. �ݵ��� ���� Interface Pointer�� Converting�� IUnknown�� Pointer�� ��ȯ�Ͽ� �����Ѵ�.
//		*. ���� ������ _IWRAPPER_IUNKNOWN_INTERFACE�� �����Ѵ�.
protected:
	virtual IUnknown *	GetAddInCustomInterface( void);
// (2005.07.08, ��¿�) AddIn Special Interface�� ������ ���Ͽ� �⺻ Interface (IUnknown��)�� �����Ѵ�.
//		*. ���� GetAddInCustomInterface()�� ��� ������ ���� ������, 
//			GetAddInCustomInterface() ������ �Բ� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2006/8/20 - Seung-Won, Bae) Manage StyleData with Share.
protected:
	CStyleDataManager	m_styleDataManager;

// (2009/6/17 - Seung-Won, Bae) for Custom Interface
protected:
	virtual int				GetStyleCount( void);
	virtual const char *	GetStyleName( int p_nStyleIndex);
	virtual void			SetStyleToChart( int p_nStyleIndex);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);

// (2009/9/21 - Seung-Won, Bae) Show Style Type on Price, Volume and MAs.
protected:
	virtual void	LoadCurrentStyle( void);
	virtual void	DrawSampleImage( HDC p_hDC, int p_nLeft, int p_nTop, int p_nRight, int p_nBottom, int p_nStyleIndex, HGDIOBJ p_hFont);
	virtual void	UpdateIndexStyle( const char *p_szGraphName, int p_nStyleIndex);

// 2008.01.25 by LYH >>
protected:
	HWND	m_hOcxWnd;
// 2008.01.25 by LYH <<
};

#endif // !defined(AFX_CHARTSTYLEDATAMANAGERADDINIMP_H__A8F01B76_4AB8_443E_BDA3_78C34280CE89__INCLUDED_)
