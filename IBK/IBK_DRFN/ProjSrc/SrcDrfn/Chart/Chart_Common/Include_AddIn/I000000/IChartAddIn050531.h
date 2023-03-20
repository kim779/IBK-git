// IChartAddIn050531.h: interface for the IChartAddIn050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTADDIN050531_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_)
#define AFX_ICHARTADDIN050531_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����!	050531 Version�� OCX���� �̿�Ǵ� AddIn�� Interface�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		Interface ���� ���� ���� ������ �ʽ��ϴ�.
//		������ �ʴ� ������ 050531 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			��ȭ			��¿�
//		ChartAssistIndexAddIn		��ȭ			���ٹ�
//////////////////////////////////////////////////////////////////////

// (2004.10.08, ��¿�) Chart AddIn Object�� Interface�� �����Ѵ�.
//		AddIn DLL ���� Interface�� Macro Interface�� �����Ѵ�.
#if IChartAddInVer < 050531
	#define	IChartAddIn		IChartAddIn050531
	#undef	IChartAddInVer
	#define	IChartAddInVer	050531
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IChartAddIn050531 : public IUnknown  
{
protected:
	virtual ~IChartAddIn050531()	{}

// (2004.10.13, ��¿�) �⺻ Manager�� ��ϵǾ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
public:
	virtual BOOL	HasFullManager( void) = 0;

// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
public:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData) = 0;
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData) = 0;	// (2005/9/2 - Seung-Won, Bae) for String Data

// [05/07/08] AddIn ���� Interface�� �����Ѵ�.
//		*. ����. �ݵ��� ���� Interface Pointer�� Converting�� IUnknown�� Pointer�� ��ȯ�Ͽ� �����Ѵ�.
public:
	virtual IUnknown *	GetAddInCustomInterface( void) = 0;

public:
	// [04/10/11] ������ �����ϴ� Interface�� �����Ѵ�.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare) = 0;
	// (2005/11/21 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue) = 0;
	// (2005/8/10 - Seung-Won, Bae) use in Only Hanwha
	virtual void	OnSharedDataChange( const char *p_szShareName, const char *p_szSharedData, BOOL p_bStartShare) = 0;

	// [04/10/11] �ܺ� ToolBar�� Message�� �����Ͽ� AddIn DLL������ ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnExtTool( WPARAM wParam, LPARAM lParam) = 0;
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

// (2004.10.22, ��¿�) AddIn DLL���� Mouse Event�� �����Ѵ�.
//		Return Value�� FALSE�� ��쿡�� ���� ó�� Routine�� �������� �ʴ´�.
public:
	// [04/10/11] Left Mouse Button Double Click Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	OnLButtonDblClk(	UINT nFlags, CPoint &point) = 0;
	// [04/10/22] Mouse Moving Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	OnMouseMove(		UINT nFlags, CPoint &point) = 0;
	// [04/10/25] Left Mouse Button Down Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	OnLButtonDown(		UINT nFlags, CPoint &point) = 0;
	// [04/10/25] Left Mouse Button Down Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	OnLButtonUp(		UINT nFlags, CPoint &point) = 0;
	// [04/10/29] Mouse Capture Release Event�� AddIn DLL���� �����ϵ��� �Ѵ�.
	//		����! OnCaptureChanged()�� SetCapture()/RelaseCapture()�� ���� ��� �ܿ���,
	//			Focus �� Activate, ToolTip, TrackMenu� ���ؼ��� ���÷� (SetCapture����)
	//			���� �ڱ� �ڽ��� �ٽ� SetCapture()�ϴ� ��쿡�� ȣ����� �����Ѵ�.
	virtual void	OnCaptureChanged(	CWnd *pWnd) = 0;

// (2004.10.29, ��¿�) AddIn DLL���� �׹��� Windows Event�� �����Ѵ�.
public:
	// [04/10/29] Timer Event�� ó���ϴ� Interface�� �����Ѵ�.
	virtual void	OnTimer( UINT p_uTimerID) = 0;
	// [04/11/09] KeyBoard Down Event�� ó���ϴ� Interface�� �����Ѵ�.
	//		Return Value�� FALSE�� ��쿡�� ���� ó�� Routine�� �������� �ʴ´�.
	virtual BOOL	OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	// [04/11/09] KeyBoard Up Event�� ó���ϴ� Interface�� �����Ѵ�.
	//		Return Value�� FALSE�� ��쿡�� ���� ó�� Routine�� �������� �ʴ´�.
	virtual BOOL	OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags) = 0;

// (2005/2/4 - Seung-Won, Bae) Real ���� Event�� �����Ѵ�.
// (2005/8/16 - Seung-Won, Bae) Not used in Hanwha
public:
	// [04/10/14] Real�� ������ �����Ѵ�.
	//				�ӵ� ����� DRDS�� Data�� �״�� Pass�Ѵ�.
	//				���� FALSE�� Return�� ��� ���� Module���� (PacketData���Ե�) �� �̻� Real�� �������� �ʴ´�.
	virtual BOOL	OnDrdsData( WPARAM p_wParam, LPARAM p_lParam) = 0;
	// [04/10/15] Real Packet�� ������ �������� AddIn DLL���� �˷� ó����� �Ѵ�.
	virtual void	OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam) = 0;
// (2005/8/16 - Seung-Won, Bae) used in Only Hanwha
public:
	// 1. Notify for Every Real Item
	virtual BOOL	OnRealData( WORD p_wRealRecordID, WORD p_wRealItemID, const char *p_szRealKeyCode, long p_lPacketAddress, double dData) = 0;
	// 2. Notify for Every Real Record End
	virtual void	OnRealRedcordEnd( WORD p_wRealRecordID, const char *p_szRealKeyCode) = 0;
};

#endif // !defined(AFX_ICHARTADDIN050531_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_)
