// IWAddInDll050531.h: interface for the CIWAddInDll050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWADDINDLL050531_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_)
#define AFX_IWADDINDLL050531_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����!	OCX���� �̿�Ǵ� AddIn��  ǥ�� Interface�� �� Version�� Interface�� �����ִ� Wrapper�Դϴ�.
//		ǥ�� AddIn Interface�� ����� ������ �Բ� �� Version�� AddIn Interface Wrapper�� �� Version�� �µ��� �����Ǿ�� �մϴ�.
//		�̶�, �����Ǵ� ����� �����Ͽ� ����� AddIn�� �̿��� �� �ֵ��� �ϰ� �Ǹ�, ���� AddIn�� �Բ� ����ó�� �Ǿ�� �ϰ�,
//			�׷��� �ʴ� ��쿡�� Wrapper���� �ش� Version�� AddIn Interface�� �����Ű�� �������ν� ���������� ����ó�� �� �� �ֵ��� �ϴ� ���Դϴ�.
//////////////////////////////////////////////////////////////////////

// (2004.10.07, ��¿�) Addin DLL�� Interface Wrapper�� �����Ѵ�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../IWUnknown.h"					// for CIWUnknown
#include "../IAddInDll.h"					// for IAddInDll
#include "./IChartAddIn050531.h"	// IChartAddIn050531

class CIWAddInDll050531 : public CIWUnknown, public IAddInDll
{

// [04/10/07]
public:
	CIWAddInDll050531( const char *p_szAddInModule, HMODULE p_hAddInDll);
protected:
	virtual ~CIWAddInDll050531();

// (2004.10.07, ��¿�) Interface Wrapper�� �⺻ Interface (IUnknown��)�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()


// (2004.10.07, ��¿�) Addin DLL�� Module Name�� Handle�� �����Ͽ� DLL Interface�� ȣ���� �� �ֵ��� �Ѵ�.
protected:
	HMODULE		m_hAddInDll;
	CString		m_strModuleName;


// (2004.10.07, ��¿�) AddIn Interface Wrapper�� Version Value�� �����Ѵ�.
typedef enum EIVERSION	{ IVERSION = _IWRAPPER_DECIMAL( 050531)	};

// (2004.10.07, ��¿�) AddIn Interface Wrapper Selector Routine�� �����Ѵ�.
//		AddIn Manager�� LoadAddIn()���� Loading�� DLL�� Addin Version�� �´� Wrapper�� �����ϴ� Routine�̴�.
//		Ư�� Addin Manager�� �׻� ���� Version�� Selector Routine�� ȣ���Ͽ� ������ Wrapper Object�� ���� �� �ֵ��� �Ѵ�.
public:
	static IAddInDll *CreateAddInDLLIWrapper( const char *p_szAddInModule, HMODULE p_hAddInDll, int p_nAddInIVersion);


//[04/10/07] AddIn DLL�� �ʱ�ȭ��Ű�� Interface�� �����Ѵ�.
// (2004.10.08, ��¿�) �ʱ�ȭ�ÿ� IAddInManager(ChartOcx Interface Gateway)�� �����Ͽ�
//		DLL�� AddIn Interface�� Ȯ���Ѵ�.
protected:
	IChartAddIn050531 *	m_pIChartAddIn;
public:
	virtual BOOL		InitAddInDll( IAddInManager *p_pIAddInManager);

// [04/10/11] AddIn DLL�� �̸��� ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	virtual const char *	GetAddInDllName( void)	{	return m_strModuleName;	}

// [04/10/11] AddIn Object�� Interface�� �����ϴ� Interface�� �����Ѵ�.
public:
	virtual IUnknown *		GetIChartAddIn( void);

// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
public:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData)
					{	return m_pIChartAddIn->InvokeAddIn( p_nCommandType, p_lData);	}
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)	// (2005/9/2 - Seung-Won, Bae) for String Data
					{	return m_pIChartAddIn->InvokeAddInStr( p_szCommandName, p_szData);	}

public:
	// [04/10/11] ������ �����ϴ� Interface�� �����Ѵ�.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare)
					{	m_pIChartAddIn->OnGSharedDataChange( p_szGShareKey, p_szGShareValue, p_bStartShare);	}
	virtual void	OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue)
					{	m_pIChartAddIn->OnLSharedDataChange( p_szLShareKey, p_szLShareValue);	}
	// (2005/8/10 - Seung-Won, Bae) use in Only Hanwha
	virtual void	OnSharedDataChange( const char *p_szShareName, const char *p_szSharedData, BOOL p_bStartShare)
					{	m_pIChartAddIn->OnSharedDataChange( p_szShareName, p_szSharedData, p_bStartShare);	}

	// [04/10/11] �ܺ� ToolBar�� Message�� �����Ͽ� AddIn DLL������ ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnExtTool( WPARAM wParam, LPARAM lParam)
					{	m_pIChartAddIn->OnExtTool( wParam, lParam);	}
	// [04/10/11] AddIn DLL�鿡�� ResetChart ��Ȳ�� �˷� ó���ǵ��� �Ѵ�.
	virtual void	OnResetChart( void)
					{	m_pIChartAddIn->OnResetChart();	}
	// [04/10/11] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawBeforeGraph( CDC *p_pDC)
					{	m_pIChartAddIn->OnDrawBeforeGraph( p_pDC);	}
	// [04/12/25] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawAfterGraph( CDC *p_pDC)
					{	m_pIChartAddIn->OnDrawAfterGraph( p_pDC);	}
	// [04/10/13] TR�� ������ AddIn DLL�鿡�� �˷� ������� �Ѵ�.
	virtual void	OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset)
					{	m_pIChartAddIn->OnPacketData( p_szPacketNames, p_bWithReset);	}
	// [04/12/07] TR�̳� Real�� ���ſ� ���� Packet Data�� Update�Ǿ����� AddIn DLL�鿡 �˷� ��ǥ���� ó����� �Ѵ�.
	virtual void	OnUpdatePacketData( BOOL p_bTrUpdate)
					{	m_pIChartAddIn->OnUpdatePacketData( p_bTrUpdate);	}

// (2004.10.22, ��¿�) AddIn DLL���� Mouse Event�� �����Ѵ�.
//		Return Value�� FALSE�� ��쿡�� ���� ó�� Routine�� �������� �ʴ´�.
public:
	// [04/10/11] Left Mouse Button Double Click Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	OnLButtonDblClk(	UINT nFlags, CPoint &point)
					{	return m_pIChartAddIn->OnLButtonDblClk(	nFlags, point);	}
	// [04/10/22] Mouse Moving Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	OnMouseMove(		UINT nFlags, CPoint &point)
					{	return m_pIChartAddIn->OnMouseMove(		nFlags, point);	}
	// [04/10/25] Left Mouse Button Down Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	OnLButtonDown(		UINT nFlags, CPoint &point)
					{	return m_pIChartAddIn->OnLButtonDown(	nFlags, point);	}
	// [04/10/25] Left Mouse Button Down Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	OnLButtonUp(		UINT nFlags, CPoint &point)
					{	return m_pIChartAddIn->OnLButtonUp(		nFlags, point);	}
	// [04/10/29] Mouse Capture Release Event�� AddIn DLL���� �����ϵ��� �Ѵ�.
	virtual void	OnCaptureChanged(	CWnd *pWnd)
					{	m_pIChartAddIn->OnCaptureChanged( pWnd);	}

// (2004.10.29, ��¿�) AddIn DLL���� �׹��� Windows Event�� �����Ѵ�.
public:
	// [04/10/29] Timer Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual void	OnTimer( UINT p_uTimerID)
					{	m_pIChartAddIn->OnTimer( p_uTimerID);	}
	// [04/11/09] KeyBoard Down Event�� ó���ϴ� Interface�� �����Ѵ�.
	//		Return Value�� FALSE�� ��쿡�� ���� ó�� Routine�� �������� �ʴ´�.
	virtual BOOL	OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
					{	return m_pIChartAddIn->OnKeyDown( nChar, nRepCnt, nFlags);	}
	// [04/11/09] KeyBoard Up Event�� ó���ϴ� Interface�� �����Ѵ�.
	//		Return Value�� FALSE�� ��쿡�� ���� ó�� Routine�� �������� �ʴ´�.
	virtual BOOL	OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags)
					{	return m_pIChartAddIn->OnKeyUp( nChar, nRepCnt, nFlags);	}

// (2005/2/4 - Seung-Won, Bae) Real ���� Event�� �����Ѵ�.
// (2005/8/16 - Seung-Won, Bae) Not used in Hanwha
public:
	// [04/10/14] Real�� ������ �����Ѵ�.
	//				�ӵ� ����� DRDS�� Data�� �״�� Pass�Ѵ�.
	//				���� FALSE�� Return�� ��� ���� Module���� (PacketData���Ե�) �� �̻� Real�� �������� �ʴ´�.
	virtual BOOL	OnDrdsData( WPARAM p_wParam, LPARAM p_lParam)
					{	return m_pIChartAddIn->OnDrdsData( p_wParam, p_lParam);	}
	// [04/10/15] Real Packet�� ������ �������� AddIn DLL���� �˷� ó����� �Ѵ�.
	virtual void	OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam)
					{	m_pIChartAddIn->OnDrdsPacketEnd( p_wParam, p_lParam);	}
// (2005/8/16 - Seung-Won, Bae) used in Only Hanwha
public:
	// 1. Notify for Every Real Item
	virtual BOOL	OnRealData( WORD p_wRealRecordID, WORD p_wRealItemID, const char *p_szRealKeyCode, long p_lPacketAddress, double dData)
					{	return m_pIChartAddIn->OnRealData( p_wRealRecordID, p_wRealItemID, p_szRealKeyCode, p_lPacketAddress, dData);	}
	// 2. Notify for Every Real Record End
	virtual void	OnRealRedcordEnd( WORD p_wRealRecordID, const char *p_szRealKeyCode)
					{	m_pIChartAddIn->OnRealRedcordEnd( p_wRealRecordID, p_szRealKeyCode);	}
};

#endif // !defined(AFX_IWADDINDLL050531_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_)
