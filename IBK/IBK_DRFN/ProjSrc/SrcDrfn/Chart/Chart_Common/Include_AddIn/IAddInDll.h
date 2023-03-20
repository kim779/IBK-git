// IAddInDll.h: interface for the IAddInDll class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IADDINDLL_H__BB3A45D2_5941_4D70_AAEB_1DF7AAC43A35__INCLUDED_)
#define AFX_IADDINDLL_H__BB3A45D2_5941_4D70_AAEB_1DF7AAC43A35__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����!	AddIn ����� �ִ� OCX������ �̿�˴ϴ�.
//		�� AddIn���� OCX�� �����ϰ� �̿��� �� �ֵ��� �����ϱ� ���� ǥ�� Interface��
//			Interface ���� ���游 �����մϴ�.
//		��, ��� �߰�(Ȯ��)�� ���� �� Type ������ �� AddIn Version���� IWAddInDllXXXXXX����
//			����Ǵ� IChartAddInXXXXXX�� �µ��� �����ϴ� �۾��� ���� �������� �����ϴ�.
//		��� OCX�������� �̿�Ǵ� Interface�� ������ ����� AddIn�� �Բ� ������ �ʿ�� ����.
// *. AddIn Version Info
//		AddIn OCX Site
//			���� GZone
//////////////////////////////////////////////////////////////////////

// (2004.10.07, ��¿�) Addin DLL�� Export�Ǿ�� �ϴ� Interface�� �����Ѵ�.
//		1. Addin DLL�� ������ �̶� �ϴ��� �� Interface�� ȣ��Ǳ� ������,
//			�̸� ��¹��� Interface Wrapper Class�� �����Ͽ� �������� ȣȯ�ǵ��� �����Ѵ�.
//		2. ���� �� Interface�� ������ ���, Version ��ġ�� ���� ���ڰ� �ƴϸ�,
//			���ο� Interface Wrapper�� ����Ͽ� �̿��ϰ�,
//			�������� Interface Wrapper���� �̿� ���� ȣȯ�ǵ��� ��� ������ ��� �Ѵ�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IAddInManager;
interface IAddInDll : public IUnknown  
{
protected:
	virtual ~IAddInDll()	{};

// [04/10/07] AddIn DLL�� �ʱ�ȭ��Ű�� Interface�� �����Ѵ�.
// (2004.10.08, ��¿�) �ʱ�ȭ�ÿ� IAddInManager(ChartOcx Interface Gateway)�� �����Ͽ�
//		DLL�� AddIn Interface�� Ȯ���Ѵ�.
public:
	virtual BOOL	InitAddInDll( IAddInManager *p_pIAddInManager) = 0;

// [04/10/11] AddIn DLL�� �̸��� ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	virtual const char *	GetAddInDllName( void) = 0;

// [04/10/11] AddIn Object�� Interface�� �����ϴ� Interface�� �����Ѵ�.
public:
	virtual IUnknown *		GetIChartAddIn( void) = 0;

// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
public:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData) = 0;
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData) = 0;	// (2005/9/2 - Seung-Won, Bae) for String Data
	
// [04/10/14] Event�� �����ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/11] Global ������ �����ϴ� Interface�� �����Ѵ�.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare) = 0;
	// (2005/11/21 - Seung-Won, Bae) Notify Local Shared Data Changed to AddIn
	virtual void	OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue) = 0;
	// (2005/8/10 - Seung-Won, Bae) use in Only Hanwha
	virtual void	OnSharedDataChange( const char *p_szShareName, const char *p_szSharedData, BOOL p_bStartShare = FALSE) = 0;
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
	// [04/10/25] Left Mouse Button Up Event�� ó���ϴ� Interface�� �����Ѵ�.
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
	virtual BOOL	OnDrdsData( WPARAM p_wParam, LPARAM p_lParam)		= 0;
	// [04/10/15] Real Packet�� ������ �������� AddIn DLL���� �˷� ó����� �Ѵ�.
	virtual void	OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam)	= 0;
// (2005/8/16 - Seung-Won, Bae) used in Only Hanwha
public:
	// 1. Notify for Every Real Item
	virtual BOOL	OnRealData( WORD p_wRealRecordID, WORD p_wRealItemID, const char *p_szRealKeyCode, long p_lPacketAddress, double dData) = 0;
	// 2. Notify for Every Real Record End
	virtual void	OnRealRedcordEnd( WORD p_wRealRecordID, const char *p_szRealKeyCode) = 0;
};

#endif // !defined(AFX_IADDINDLL_H__BB3A45D2_5941_4D70_AAEB_1DF7AAC43A35__INCLUDED_)
