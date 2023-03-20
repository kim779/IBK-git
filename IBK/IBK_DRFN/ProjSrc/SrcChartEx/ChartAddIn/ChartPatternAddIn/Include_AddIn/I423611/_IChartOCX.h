// IChartOCX.h: interface for the IChartOCX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_)
#define AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseEnum.h"		// for CToolOptionInfo::TOOLOPTION
#include "_AddInVer.h"			// for AddIn Version Define
#include "_IString.h"					// for ILPCSTR

class CCommonInterface;
interface IAddInDll;
interface IAddInToolMgr;
interface IChartManager;
interface IPacketManager;
interface IGraphInfoManager;
interface IChartOCX : public IUnknown  
{
protected:
	virtual ~IChartOCX() {}

// (2006/1/17 - Seung-Won, Bae) Get CCommonInterface of OCX for Special Usage
public:
	virtual CCommonInterface *	GetCommonInterface( void)	= 0;

// [04/10/11] OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
public:
	virtual IChartManager *		GetIChartManager( void)		= 0;
	virtual IPacketManager *	GetIPacketManager( void)	= 0;
	virtual IGraphInfoManager *	GetIGraphInfoManager( void)	= 0;
	virtual IAddInToolMgr *		GetIAddInToolMgr( void)		= 0;

// (2004.10.19, ��¿�) Windows ���� Interface�� �����Ѵ�.
public:
	// [04/10/11] ReDrawing�� ���� Interface�� �����Ѵ�.
	virtual	void		InvalidateControl( void) = 0;
	// [04/10/11] ReDrawing�� ���� Interface�� �����Ѵ�.
	virtual HWND		GetOcxHwnd( void) = 0;
	// [04/11/02] Counter Capturing Interface�� �����Ѵ�.
	virtual HWND		AddSetCapture( void) = 0;
	// [04/11/02] Counter Capturing Interface�� �����Ѵ�.
	virtual BOOL		RemoveSetCapture( void) = 0;
	// [04/10/25] ���ο� Cursor�� �����ϸ鼭, ���� ������ Cursor�� Ȯ���Ѵ�.
	virtual HCURSOR		SetCursorHandle( HCURSOR p_hNewCursor) = 0;
	// [04/10/26] ���� ������ Cursor�� Ȯ���Ѵ�.
	virtual HCURSOR		GetCursorHandle( void) = 0;
	// (2004.10.25, ��¿�) Chart ���� Font�� �����Ѵ�.
	virtual CFont *		GetChartFont( void) = 0;

// (2004.10.18, ��¿�) ȯ�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	// [04.10.18] ����� Folder�� ���ϴ� Interface�� �����Ѵ�.
	virtual const char *	GetUserFilePath( void)	= 0;
	// ����(05/07/14) HTS�� Dir Path�� �˷��ش�.
	virtual const char *	GetHtsPath( void)		= 0;
	//sy 2006.02.21. OCX Path
	virtual const char *	GetOCXPath( void)		= 0;
// (2006/1/18 - Seung-Won, Bae) Set the Design Info
public:
	virtual void		SetStrBlockRegion( const char *p_szBlockRegion)	= 0;

// (2004.10.11, ��¿�) Chart�� �� ���¸� Ȯ���ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/11] Ư����Ʈ�� ������� Ȯ���ϴ� Interface�� �����Ѵ�. (���ð�/�Ｑ��ȯ��/P&F)
	virtual BOOL	IsOnSpecialChart( void) = 0;
	// (2005.01.20, ��¿�) ToolBar�� ���� Item�� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
	virtual	BOOL	HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem) = 0;
	// (2006/1/20 - Seung-Won, Bae) Get Numerical Inquiry Option Setting
	virtual UINT	GetNumericalInquiryOption( void) const = 0;
	// (2006/1/17 - Seung-Won, Bae) Check RunTimeMode
	virtual BOOL	IsRuntimeMode( void) const = 0;
	// (2006/1/21 - Seung-Won, Bae) Check Graph Menu Usable
	virtual BOOL	IsGraphMenuShow( void) const = 0;
	// (2006/1/22 - Seung-Won, Bae) Retrieve Bong Count Margin.
	virtual short	GetBongCountMargin( void) const = 0;
	// (2006/1/22 - Seung-Won, Bae) Retrieve Graph RButton Type.
	virtual short	GetGraphRButtonType( void) const = 0;
	// (2006/2/6 - Seung-Won, Bae) Get OCX Region except ToolBar Region.
	virtual	CRect	GetOcxRegion( void) const = 0;
	// (2006/2/6 - Seung-Won, Bae) Check ScrollBar
	virtual BOOL	IsUseToolScrollBar( void) const = 0;

// (2006/1/24 - Seung-Won, Bae) Interface for Analysis Tool Option
public:
	virtual BOOL		IsShowTrendLineText( void)		const = 0;
	virtual BOOL		IsShowHorizonTextLeft( void)	const = 0;
	virtual BOOL		IsShowHorizonTextRight( void)	const = 0;
	virtual BOOL		IsShowVerticalText( void)		const = 0;
	virtual BOOL		IsShowThreeText( void)			const = 0;
	virtual BOOL		IsShowFourText( void)			const = 0;
	virtual BOOL		IsShowFiboText( void)			const = 0;
	virtual COLORREF	GetAnalysisToolColor( void)		const = 0;

// (2004.10.11, ��¿�) Data Index �� Position ���� Interface
public:
	// [04/10/11] ������ �� Data�� Index�� ���ϴ� Interface�� �����Ѵ�.
	virtual int		GetLastRealDataIndex( void) = 0;
	// [04/10/11] ScrollBar���� ������ Scroll Index�� ���ϴ� Interface�� �����Ѵ�. (��������)
	virtual BOOL	GetMaxScrollRange( int &p_nMaxScrollRange) = 0;
	// [04/10/11] Chart�� ���� ���̴� View ������ ù Data Index�� �� Data Index�� ���ϰų�, �����ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetViewStartEndIndex( int &p_nStartIndex, int &p_nEndIndex) = 0;
	virtual BOOL	SetViewStartEndIndex( int p_nStartIndex, int p_nEndIndex) = 0;
	// [04/10/27] Data ��ǥ�� Screen ��ǥ�� ��ȯ�ϴ� Interface�� �����Ѵ�.
	virtual int		ConvertDataToYPosition( double p_dData, double p_dDataMax, double p_dDataMin,
						int p_nYPositionTop, int p_nYPositionBottom, BOOL p_bLog, BOOL p_bReverse) = 0;
	virtual double	ConvertYPositionToData( int p_nYPosition, int p_nYPositionTop, int p_nYPositionBottom,
						double p_dDataMax, double p_dDataMin, BOOL p_bLog, BOOL p_bReverse) = 0;
	// [04/11/09] Scroll/Zoom�� ���� ó���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	ScrollTo( int p_nScrollValue, BOOL p_bRelative)	= 0;
	virtual BOOL	ZoomTo( int p_nZoomValue, BOOL p_bRelative)		= 0;
	// (2005.01.13, ��¿�) ��Ʈ�� ��ü���� Mode ���θ� Ȯ�� �� �����ϴ� Interface�� �����Ѵ�.
	virtual BOOL	CheckFullView( void) const		= 0;
	virtual void	SetFullView( BOOL p_bFullView)	= 0;
	// (2006/2/6 - Seung-Won, Bae) DragZoomIn for DragZoom AddIn
	virtual void	DragZoomIn( const CPoint &p_ptStart, const CPoint &p_ptEnd) = 0;

// (2004.10.11, ��¿�) ���� ���� Interface
public:
	// [04/10/11] �־��� Global ������ �����Ǿ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet) = 0;
	// [04/10/11] �־��� Global ������ ������ �߻���Ű�� Interface�� �����Ѵ�.
	virtual BOOL	FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData) = 0;
// (2005/8/10 - Seung-Won, Bae) used in Only Hanwha
public:
	// (2005/8/10 - Seung-Won, Bae) Retrieve the Share Info Item Setting
	virtual BOOL	GetShareInfoItem( const char *p_szShareName, int &p_nShareType, int &p_nShareArea, long &p_lCodeFilter) = 0;
	// (2005/8/10 - Seung-Won, Bae) Fire Shared Data
	virtual BOOL	FireSharedData( const char *p_szShareName, const char *p_szSharedData) = 0;

// (2004.10.19, ��¿�) Image ���� �� ����� ���� Drawing Interface�� �����Ѵ�.
public:
	virtual void	DrawForImage( CDC *p_pDC, CRect p_rctSource, BOOL p_bUseScreenColor) = 0;

// ����(05/06/01) �׷����� �߰�, ����, ���� ���� Interface�� �����Ѵ�.
public:
	virtual void		AddPacket(const char* pcszPacketOption) = 0;
	virtual void		AddGraph(short nRow, short nColumn, short nVertScaleGroup, const char* szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale) = 0;
	virtual bool		ChangeSubGraphData(const char* pcszGraphName, const char* pcszSubGraphName, COLORREF clrLineColor, int nThickness, int nDrawStyle) = 0;
	virtual bool		SetStartPeriodOfPacket(const char* pcszPacketName, long lStartPeriod, BOOL bAddSpace) = 0;
	virtual bool		SetStartIndexOfPacket(const char* pcszPacketName, int nStartDataIndex, BOOL bAddSpace) = 0;
	virtual bool		HasGraph(const char* pcszGraphName) = 0;
	virtual bool		RemoveGraph(const char* pcszGraphName) = 0;
	virtual bool		RemovePacket(const char* pcszPacketName, const bool bIsOnlyDataClear) = 0;

// (2006/1/21 - Seung-Won, Bae) Operations
public:
	// ����(05/08/22) AddIn���� ���� ������ ������ �ִ� Interface�� �����Ѵ�.
	// nOption = 1:Update, 2:Append
	virtual void		SetRealData(const char* pcszPacketName, double dData, short nOption) =0;
	// (2006/1/20 - Seung-Won, Bae) Show Tooltip
	virtual void		ShowTooltip( const CPoint &p_ptPoint) = 0;
	virtual void		HideTooltip( void) = 0;
	// (2006/1/21 - Seung-Won, Bae) Save Current Graph State
	virtual BOOL		SaveChartCfg( void) = 0;
	// (2006/1/23 - Seung-Won, Bae) Set Logging Flag
	virtual void		SetLoggingFlag( const BOOL p_bInLogging) = 0;
	// (2006/1/28 - Seung-Won, Bae) Reset Tool UI
	virtual void		ReleaseToolUI( const CToolOptionInfo::TOOLOPTION p_eToolOption) = 0;
	// (2006/2/7 - Seung-Won, Bae) Show and Hide Numerical Inquiry Dialog
	virtual void		ShowNumericalInquiryDlg( const BOOL p_bShow, const BOOL p_bAutoMove) = 0;
	virtual void		ShowNumericalInquiryDlg( CDC *p_pDC, const CPoint &p_ptPoint) = 0;

// (2006/1/17 - Seung-Won, Bae) Notify from AddIn
public:
	// (2005/8/18 - Seung-Won, Bae) for Event from AddIn
	virtual void		FireAddInEvent( const char *p_szAddInName, short p_nEventID, const char *p_szData, double p_dData, long p_lData) = 0;
	// [04/10/11] DLL Container���� Message�ϴ� Interface�� �����Ѵ�.
	virtual LRESULT		SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam) = 0;
	// (2006/11/15 - Seung-Won, Bae) Notify Tool State Changing to MChart an ToolBar
	virtual BOOL		OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn) = 0;
	// (2007/1/5 - Seung-Won, Bae) Excute Tool Command
	virtual BOOL		RunToolCommand( const CToolOptionInfo::TOOLOPTION p_eToolOption, const int p_nParam) = 0;

// [07.05.18] ��������
	virtual IString*	GetSelectedRQ( void) = 0;
};

#endif // !defined(AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_)
