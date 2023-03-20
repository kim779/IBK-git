// MiniJisuAddInImp.h: interface for the CMiniJisuAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINIJISUADDINIMP_H__740AE3F4_ECC2_4E1B_A098_B8A7B21A70EB__INCLUDED_)
#define AFX_MINIJISUADDINIMP_H__740AE3F4_ECC2_4E1B_A098_B8A7B21A70EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Addin/I000000/_ChartAddInBase.h"	// for CChartAddInBase
#include "IMiniJisu.h"									// for IMiniJisu

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/3/23
// Comments		: MiniJisu (Comparing Chart of Future and Option) Data Query Tool
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

class CMiniJisuAddInImp : public CChartAddInBase, public IMiniJisu
{
public:
	CMiniJisuAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CMiniJisuAddInImp();

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

// (2006/3/23 - Seung-Won, Bae) MiniJisu Interface
protected:
	virtual void	SendToChartMiniJisu(LPCTSTR szCommand, LPCTSTR szParam1, LPCTSTR szParam2, LPCTSTR szParam3);

// (2006/3/23 - Seung-Won, Bae) MiniJisu Sub Logics
protected:
	BOOL m_bMiniStartDrag;
	BOOL m_bSendDataList2Parent;
protected:
	CString GetPacketDataForMiniJisuByIndex(int nDataIndex);
	CString GetPacketDataForMiniJisu(short nPoint_x, short nPoint_y);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_MINIJISUADDINIMP_H__740AE3F4_ECC2_4E1B_A098_B8A7B21A70EB__INCLUDED_)
