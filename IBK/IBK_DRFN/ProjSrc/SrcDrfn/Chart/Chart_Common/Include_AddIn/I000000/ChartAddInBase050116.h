// ChartAddInBase050116.h: interface for the CChartAddInBase050116 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTADDINBASE050116_H__DDC8A6DF_434B_4790_898D_B98EDE523730__INCLUDED_)
#define AFX_CHARTADDINBASE050116_H__DDC8A6DF_434B_4790_898D_B98EDE523730__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����!	050116 Version�� AddIn���� �̿�Ǵ� AddIn�� Interface Support Base Class�Դϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		������ �ʴ� ������ 050116 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050116 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			��¿�
//			ChartPatternAddIn			GZone			������
//			ChartGuideLine				GZone			������
//////////////////////////////////////////////////////////////////////

// (2004.10.08, ��¿�) Chart AddIn Object�� Base Class�� �����Ѵ�.
// (2004.10.18, ��¿�) Chart AddIn Object Base Class�� Chart Module���� Interface�� �����ϴ� �κ��̹Ƿ�,
//		Chart�ʿ��� AddIn Module�� �����ϸ鼭 ���������Ѵ�.	����, Version ��ȣ�� �����Ͽ� �����Ѵ�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../IWUnknown.h"				// for CIWUnknown
#include "./IChartAddIn050116.h"		// for IChartAddIn
#include "./IChartOcx050116.h"			// for IChartOCX
#include "../IAddInManager.h"			// for IAddInManager

class CChartAddInBase050116 : public CIWUnknown, public IChartAddIn
{
public:
	CChartAddInBase050116( IChartOCX *p_pIChartOCX);
	virtual ~CChartAddInBase050116();

// (2004.10.07, ��¿�) Interface Wrapper�� �⺻ Interface (IUnknown��)�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2004.10.08, ��¿�) Chart Ocx Interface�� �����Ͽ�, ���� Ÿ Object�� ��ȸ�� �� �ֵ��� �Ѵ�.
protected:
	IChartOCX *			m_pIChartOCX;

// (2004.10.25, ��¿�) Chart OCX Control Window Pointer�� �����Ͽ� Windows Interface�� �̿��� �� �ֵ��� �Ѵ�.
protected:
	CWnd *				m_pChartCWnd;

// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
protected:
	IPacketManager *	m_pIPacketManager;
	IChartManager *		m_pIChartManager;
	IGraphInfoManager *	m_pIGraphInfoManager;
// (2004.10.13, ��¿�) �⺻ Manager�� ��ϵǾ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
public:
	virtual BOOL		HasFullManager( void);

// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
public:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData)	{	return FALSE;	}

// (2004.10.11, ��¿�) �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
protected:
	// [04/10/11] Global ������ �����ϴ� Interface�� �����Ѵ�.
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare)	{}
	// [04/10/11] �ܺ� ToolBar�� Message�� �����Ͽ� AddIn DLL������ ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnExtTool( WPARAM wParam, LPARAM lParam)	{}
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

// (2004.10.22, ��¿�) AddIn DLL���� Mouse Event�� �����Ѵ�.
//		Return Value�� FALSE�� ��쿡�� ���� ó�� Routine�� �������� �ʴ´�.
public:
	// [04/10/11] Left Mouse Button Double Click Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	OnLButtonDblClk(	UINT nFlags, CPoint &point)	{	return TRUE;	}
	// [04/10/22] Mouse Moving Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	OnMouseMove(		UINT nFlags, CPoint &point)	{	return TRUE;	}
	// [04/10/25] Left Mouse Button Down Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	OnLButtonDown(		UINT nFlags, CPoint &point)	{	return TRUE;	}
	// [04/10/25] Left Mouse Button Down Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	OnLButtonUp(		UINT nFlags, CPoint &point)	{	return TRUE;	}
	// [04/10/29] Mouse Capture Release Event�� AddIn DLL���� �����ϵ��� �Ѵ�.
	//		����! OnCaptureChanged()�� SetCapture()/RelaseCapture()�� ���� ��� �ܿ���,
	//			Focus �� Activate, ToolTip, TrackMenu� ���ؼ��� ���÷� (SetCapture����)
	//			���� �ڱ� �ڽ��� �ٽ� SetCapture()�ϴ� ��쿡�� ȣ����� �����Ѵ�.
	virtual void	OnCaptureChanged(	CWnd *pWnd)	{}

// (2004.10.29, ��¿�) AddIn DLL���� �׹��� Windows Event�� �����Ѵ�.
public:
	// [04/10/29] Timer Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual void	OnTimer( UINT p_uTimerID)	{}
	// [04/11/09] KeyBoard Down Event�� ó���ϴ� Interface�� �����Ѵ�.
	//		Return Value�� FALSE�� ��쿡�� ���� ó�� Routine�� �������� �ʴ´�.
	virtual BOOL	OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)	{	return TRUE;	}
	// [04/11/09] KeyBoard Pu Event�� ó���ϴ� Interface�� �����Ѵ�.
	//		Return Value�� FALSE�� ��쿡�� ���� ó�� Routine�� �������� �ʴ´�.
	virtual BOOL	OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags)		{	return TRUE;	}

//////////////////////////////////////////////////////////////////////
// 050116 Version, ���� Interface
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			��¿�
//////////////////////////////////////////////////////////////////////

// Temp Interface I	: TimeMark Interface ~ �� ���������� �����ϵ��� �ϴ� Interface�̴�.
public:
	// [04/10/11] TimeMark�� Clear��Ű�� Interface�� �����Ѵ�.
	virtual	void	OnClearTimeMark( void)	{}
	// [04/10/11] TimeMark�� �����ϴ� Interface�� �����Ѵ�.
	virtual	void	OnSetTimeMark( const char *p_szMarkTime, BOOL p_bWithScroll)	{}
	// [04/10/11] �ð������� ������ ����ϴ� Flag�� �ξ� ������ �� �ֵ��� �Ѵ�.
	//				�߽��� �������� �ʴ´�.
	virtual void	OnEnableReceivingTimeShare(BOOL p_bEnable)	{}

// Temp Interface II	: BoundMark Interface ~ �� ���������� �����ϵ��� �ϴ� Interface�̴�.
public:
	// [04/10/13] ����/������ ǥ�ø� Ȱ��ȭ��Ű�� Interface�� �����Ѵ�.
	virtual void	OnEnableBoundMark( BOOL p_bEnable)	{}

// Temp Interface III	: Real ���� Event�� �����Ѵ�. (�ӵ� ����� �⺻ Interface�� �������� �ʴ´�.)
public:
	// [04/10/14] Real�� ������ �����Ѵ�.
	//				�ӵ� ����� DRDS�� Data�� �״�� Pass�Ѵ�.
	//				���� FALSE�� Return�� ��� ���� Module���� (PacketData���Ե�) �� �̻� Real�� �������� �ʴ´�.
	virtual BOOL	OnDrdsData( WPARAM p_wParam, LPARAM p_lParam)	{	return TRUE;	}
	// [04/10/15] Real Packet�� ������ �������� AddIn DLL���� �˷� ó����� �Ѵ�.
	virtual void	OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam)	{}

// Temp Interface IV	: ����Ǵ� Option ����� Interface�� �����Ѵ�.
public:
	// [04/10/18] Image�� �����ϴ� Interface�� �����Ѵ�.
	virtual BOOL	OnSaveToImage( void)	{	return FALSE;	}
};

#endif // !defined(AFX_CHARTADDINBASE050116_H__DDC8A6DF_434B_4790_898D_B98EDE523730__INCLUDED_)
