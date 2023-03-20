// IWChartOCX.h: interface for the CIWChartOCX class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_)
#define AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\_IWUnknown.h"	// for CIWUnknown
#include "_IChartOCX.h"		// for IChartOCX

class CAddInManagerImp;
class CCommonInterface;
interface IChartCtrl;
class CIWChartOCX : public CIWUnknown, public IChartOCX
{
public:
	CIWChartOCX( IChartCtrl *p_pIChartCtrl, CAddInManagerImp *p_pAddInManagerImp);
	virtual ~CIWChartOCX();

// (2004.10.07, ��¿�) Interface Wrapper�� �⺻ Interface (IUnknown��)�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2004.10.08, ��¿�)	Chart OCX�� Pointer�� �����Ѵ�.
//		������ �� OCX�� Ctrl Class�̸�, StdAfx.h�� Macro�� �����Ͽ� �̿��Ѵ�.
protected:
	IChartCtrl *	m_pIChartCtrl;
// (2006/1/17 - Seung-Won, Bae) Manage CAddInManagerImp Object Pointer to create CWIAddInToolMgr Object
protected:
	CAddInManagerImp *	m_pAddInManagerImp;

// (2006/1/17 - Seung-Won, Bae) Get ChartCtrl Address for Special Usage
protected:
	virtual CCommonInterface *	GetCommonInterface( void);

// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
protected:
	virtual IChartManager *		GetIChartManager( void);
	virtual IPacketManager *	GetIPacketManager( void);
	virtual IGraphInfoManager *	GetIGraphInfoManager( void);
	virtual IAddInToolMgr *		GetIAddInToolMgr( void);

// (2004.10.19, ��¿�) Windows ���� Interface�� �����Ѵ�.
protected:
	// [04.10.11] ReDrawing�� ���� Interface�� �����Ѵ�.
	virtual	long		InvalidateControl( void);
	// [04/10/11] ReDrawing�� ���� Interface�� �����Ѵ�.
	virtual HWND		GetOcxHwnd( void);
	// [04/11/02] Counter Capturing Interface�� �����Ѵ�.
	virtual HWND		AddSetCapture( void);
	// [04/11/02] Counter Capturing Interface�� �����Ѵ�.
	virtual BOOL		RemoveSetCapture( void);
	// [04/10/25] ���ο� Cursor�� �����ϸ鼭, ���� ������ Cursor�� Ȯ���Ѵ�.
	virtual HCURSOR		SetCursorHandle( HCURSOR p_hNewCursor);
	// [04/10/26] ���� ������ Cursor�� Ȯ���Ѵ�.
	virtual HCURSOR		GetCursorHandle( void);
	// [04.10.25] Chart ���� Font�� �����Ѵ�.
	virtual CFont *		GetChartFont( void);

// (2004.10.18, ��¿�) ȯ�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
protected:
	// [04.10.18] ����� Folder�� ���ϴ� Interface�� �����Ѵ�.
	virtual const char *	GetUserFilePath( void);
	// ����(05/07/14) HTS�� Dir Path�� �˷��ش�.
	virtual const char *	GetHtsPath( void);
	//sy 2006.02.21. OCX Path
	virtual const char *	GetOCXPath( void);
// (2006/1/18 - Seung-Won, Bae) Set the Design Info
protected:
	virtual void		SetStrBlockRegion( const char *p_szBlockRegion);

// (2004.10.11, ��¿�) Chart�� �� ���¸� Ȯ���ϴ� Interface�� �����Ѵ�.
protected:
	// [04.10.11] Ư����Ʈ�� ������� Ȯ���ϴ� Interface�� �����Ѵ�. (���ð�/�Ｑ��ȯ��/P&F)
	virtual BOOL	IsOnSpecialChart( void);
	// (2005.01.20, ��¿�) ToolBar�� ���� Item�� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
	virtual	BOOL	HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem);
	// (2006/1/20 - Seung-Won, Bae) Get Numerical Inquiry Option Setting
	virtual UINT	GetNumericalInquiryOption( void) const;
	// (2006/1/17 - Seung-Won, Bae) Check RunTimeMode
	virtual BOOL	IsRuntimeMode( void) const;
	// (2006/1/21 - Seung-Won, Bae) Check Graph Menu Usable
	virtual BOOL	IsGraphMenuShow( void) const;
	// (2006/1/22 - Seung-Won, Bae) Retrieve Bong Count Margin.
	virtual short	GetBongCountMargin( void) const;
	// (2006/1/22 - Seung-Won, Bae) Retrieve Graph RButton Type.
	virtual short	GetGraphRButtonType( void) const;
	// (2006/2/6 - Seung-Won, Bae) Get OCX Region except ToolBar Region.
	virtual	CRect	GetOcxRegion( void) const;
	// (2006/2/6 - Seung-Won, Bae) Check ScrollBar
	virtual BOOL	IsUseToolScrollBar( void) const;

// (2006/1/24 - Seung-Won, Bae) Interface for Analysis Tool Option
protected:
	virtual BOOL		IsShowTrendLineText( void)		const;
	virtual BOOL		IsShowHorizonTextLeft( void)	const;
	virtual BOOL		IsShowHorizonTextRight( void)	const;
	virtual BOOL		IsShowVerticalText( void)		const;
	virtual BOOL		IsShowThreeText( void)			const;
	virtual BOOL		IsShowFourText( void)			const;
	virtual BOOL		IsShowFiboText( void)			const;
	virtual COLORREF	GetAnalysisToolColor( void)		const;

// (2004.10.11, ��¿�) Data Index �� Position ���� Interface
protected:
	// [04/10/11] ������ �� Data�� Index�� ���ϴ� Interface�� �����Ѵ�.
	virtual int		GetLastRealDataIndex( void);
	// [04/10/11] ScrollBar���� ������ Scroll Index�� ���ϴ� Interface�� �����Ѵ�. (��������)
	virtual BOOL	GetMaxScrollRange( int &p_nMaxScrollRange);
	// [04/10/11] Chart�� ���� ���̴� View ������ ù Data Index�� �� Data Index�� ���ϰų�, �����ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetViewStartEndIndex( int &p_nStartIndex, int &p_nEndIndex);
	virtual BOOL	SetViewStartEndIndex( int p_nStartIndex, int p_nEndIndex);
	// [04/10/27] Data ��ǥ�� Screen ��ǥ�� ��ȯ�ϴ� Interface�� �����Ѵ�.
	virtual int		ConvertDataToYPosition( double p_dData, double p_dDataMax, double p_dDataMin,
						int p_nYPositionTop, int p_nYPositionBottom, BOOL p_bLog, BOOL p_bReverse);
	virtual double	ConvertYPositionToData( int p_nYPosition, int p_nYPositionTop, int p_nYPositionBottom,
						double p_dDataMax, double p_dDataMin, BOOL p_bLog, BOOL p_bReverse);
	// [04/11/09] Scroll/Zoom�� ���� ó���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	ScrollTo( int p_nScrollValue, BOOL p_bRelative);
	virtual BOOL	ZoomTo( int p_nZoomValue, BOOL p_bRelative);
	// (2005.01.13, ��¿�) ��Ʈ�� ��ü���� Mode ���θ� Ȯ�� �� �����ϴ� Interface�� �����Ѵ�.
	virtual BOOL	CheckFullView( void) const;
	virtual void	SetFullView( BOOL p_bFullView);
	// (2006/2/6 - Seung-Won, Bae) DragZoomIn for DragZoom AddIn
	virtual void	DragZoomIn( const CPoint &p_ptStart, const CPoint &p_ptEnd);

// [04.10.11] ���� ���� Interface
protected:
	// [04.10.11] �־��� Global ������ �����Ǿ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet);
	// [04.10.11] �־��� Global ������ ������ �߻���Ű�� Interface�� �����Ѵ�.
	virtual BOOL	FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData);
// (2005/8/10 - Seung-Won, Bae) used in Only Hanwha
protected:
	// (2005/8/10 - Seung-Won, Bae) Retrieve the Share Info Item Setting
	virtual BOOL	GetShareInfoItem( const char *p_szShareName, int &p_nShareType, int &p_nShareArea, long &p_lCodeFilter);
	// (2005/8/10 - Seung-Won, Bae) Fire Shared Data
	virtual BOOL	FireSharedData( const char *p_szShareName, const char *p_szSharedData);

// (2004.10.19, ��¿�) Image ���� �� ����� ���� Drawing Interface�� �����Ѵ�.
protected:
	virtual void	DrawForImage( CDC *p_pDC, CRect p_rctSource, BOOL p_bUseScreenColor);

// ����(05/06/07) �׷����� �߰�,����,���� ���� Interface�� �����Ѵ�.
protected:
	virtual void		AddPacket(const char* pcszPacketOption);	// Packet �߰�.
	virtual void		AddGraph(short nRow, short nColumn, short nVertScaleGroup, const char* szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale);	// Graph �߰�.
	virtual bool		ChangeSubGraphData(const char* pcszGraphName, const char* pcszSubGraphName, COLORREF clrLineColor, int nThickness, int nDrawStyle);	// �׷����� �Ӽ�����.
	virtual bool		SetStartPeriodOfPacket(const char* pcszPacketName, long lStartPeriod, BOOL bAddSpace);	// Packet�� �������ڸ� �����Ѵ�.
	virtual bool		SetStartIndexOfPacket(const char* pcszPacketName, int nStartDataIndex, BOOL bAddSpace);
	virtual bool		HasGraph(const char* pcszGraphName);	// �ش� Graph�� �ִ��� Ȯ���Ѵ�.
	virtual bool		RemoveGraph(const char* pcszGraphName);
	virtual bool		RemovePacket(const char* pcszPacketName, const bool bIsOnlyDataClear);

// ����(05/08/22) ������ ó���Ҽ� �ִ� Interface�� �����Ѵ�.
// nOption = 1:Update, 2:Append
protected:
	virtual void		SetRealData(const char* pcszPacketName, double dData, short nOption);
	// (2006/1/20 - Seung-Won, Bae) Show Tooltip
	virtual void		ShowTooltip( const CPoint &p_ptPoint);
	virtual void		HideTooltip( void);
	// (2006/1/21 - Seung-Won, Bae) Save Current Graph State
	virtual BOOL		SaveChartCfg( void);
	// (2006/1/23 - Seung-Won, Bae) Set Logging Flag
	virtual void		SetLoggingFlag( const BOOL p_bInLogging);
	// (2006/1/28 - Seung-Won, Bae) Reset Tool UI
	virtual void		ReleaseToolUI( const CToolOptionInfo::TOOLOPTION p_eToolOption);
	// (2006/2/7 - Seung-Won, Bae) Show and Hide Numerical Inquiry Dialog
	virtual void		ShowNumericalInquiryDlg( const BOOL p_bShow, const BOOL p_bAutoMove);
	virtual void		ShowNumericalInquiryDlg( CDC *p_pDC, const CPoint &p_ptPoint);

// (2006/1/17 - Seung-Won, Bae) Notify from AddIn
protected:
	// (2005/8/18 - Seung-Won, Bae) for Event from AddIn
	virtual void		FireAddInEvent( const char *p_szAddInName, short p_nEventID, const char *p_szData, double p_dData, long p_lData);
	// [04.10.11] DLL Container���� Message�ϴ� Interface�� �����Ѵ�.
	virtual LRESULT		SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam);
	// (2006/11/15 - Seung-Won, Bae) Notify Tool State Changing to MChart an ToolBar
	virtual BOOL		OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn);
	// (2007/1/5 - Seung-Won, Bae) Excute Tool Command
	virtual BOOL		RunToolCommand( const CToolOptionInfo::TOOLOPTION p_eToolOption, const int p_nParam);

// [07.05.18] ��������
	virtual IString* GetSelectedRQ( void);
};

#endif // !defined(AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_)
