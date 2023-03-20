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
// (2004.10.07, ��¿�) Interface Wrapper�� �⺻ Interface (IUnknown��)�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()
protected:
	CIWChartOCX( IChartCtrl *p_pIChartCtrl, CAddInManagerImp *p_pAddInManagerImp);
	~CIWChartOCX();
public:
	static IChartOCX *CreateObject( IChartCtrl *p_pIChartCtrl, CAddInManagerImp *p_pAddInManagerImp);

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
	virtual CMChartInterface *	GetMChartInterface( void);

// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
protected:
	virtual IChartManager *			GetIChartManager( void);
	virtual IPacketListManager *	GetIPacketListManager( void);
	virtual IPacketManager *		GetIPacketManager( void);
	virtual IGraphInfoManager *		GetIGraphInfoManager( void);
	virtual IAddInToolMgr *			GetIAddInToolMgr( void);

// (2006/8/15 - Seung-Won, Bae) Create Buffer
protected:
	virtual IStringList *	CreateObjectIStringList( void);

// (2006/4/18 - Seung-Won, Bae) Get/Set OCX's Properties
protected:
	virtual BOOL GetAllProperties3( IPropertyMap *p_pIProperties, BOOL p_bWithFull);
	virtual BOOL SetAllProperties3( IPropertyMap *p_pIProperties);

// (2004.10.19, ��¿�) Windows ���� Interface�� �����Ѵ�.
protected:
	// [04.10.11] ReDrawing�� ���� Interface�� �����Ѵ�.
	virtual	long		InvalidateControl( void);
	virtual void		InvalidateControlWithResetBlockRegion( void);
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
	// (2004.10.25, ��¿�) Chart ���� Font�� �����Ѵ�.
	virtual HFONT		GetChartFont( void);

// (2004.10.18, ��¿�) ȯ�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
protected:
	// [04.10.18] ����� Folder�� ���ϴ� Interface�� �����Ѵ�.
	virtual IString *	GetUserFilePathString( void);
	// ����(05/07/14) HTS�� Dir Path�� �˷��ش�.
	virtual IString *	GetHtsPathString( void);
	//sy 2006.02.21. OCX Path
	virtual IString *	GetOCXPathString( void);
	// (2006/8/19 - Seung-Won, Bae) Default File (Data) Folder Path
	virtual IString *	GetDefaultPathString( void);
// (2006/1/18 - Seung-Won, Bae) Set the Design Info
protected:
	virtual void		SetStrBlockRegion( const char *p_szBlockRegion);

// (2004.10.11, ��¿�) Chart�� �� ���¸� Ȯ���ϴ� Interface�� �����Ѵ�.
protected:
	// [04.10.11] Ư����Ʈ�� ������� Ȯ���ϴ� Interface�� �����Ѵ�. (���ð�/�Ｑ��ȯ��/P&F)
	virtual BOOL		IsOnSpecialChart( void);
	// (2005.01.20, ��¿�) ToolBar�� ���� Item�� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
	virtual	BOOL		HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem);
	// (2006/1/20 - Seung-Won, Bae) Get Numerical Inquiry Option Setting
	virtual UINT		GetNumericalInquiryOption( void) const;
	virtual BOOL		SetNumericalInquiryOption( UINT p_uOption);
	// (2006/1/17 - Seung-Won, Bae) Check RunTimeMode
	virtual BOOL		IsRuntimeMode( void) const;
	// (2006/1/21 - Seung-Won, Bae) Check Graph Menu Usable
	virtual BOOL		IsGraphMenuShow( void) const;
	// (2006/1/22 - Seung-Won, Bae) Retrieve Bong Count Margin.
	virtual short		GetBongCountMargin( void) const;
	// (2006/1/22 - Seung-Won, Bae) Retrieve Graph RButton Type.
	virtual short		GetGraphRButtonType( void) const;
	// (2006/2/6 - Seung-Won, Bae) Get OCX Region except ToolBar Region.
	virtual	RECT		GetOcxRegion( void) const;
	// (2006/2/6 - Seung-Won, Bae) Check ScrollBar
	virtual BOOL		IsUseToolScrollBar( void) const;
	// (2006/6/24 - Seung-Won, Bae) Get Tooltip's Background color.
	virtual COLORREF	GetToolTipBackColor( void);
	// (2004.02.11, ��¿�) ��ġ��ȸâ�� 1�� Block �������� �̵����� �ʰ�,
	//		Script���� ������ Property�� ���� OCX ���ݿ� ���� �̵��� �� �ֵ��� �����Ѵ�.
	//		(Property�� ��ȸ�ϴ� Interface�� �����Ѵ�.)
	virtual BOOL		IsOcxAreaNumericalInquiry( void);
	// (2006/3/29 - Seung-Won, Bae) Dynamic Caption Setting for each Site.
	virtual BOOL		IsOnCaptionFlagOfNIDLG( void) const;
	// (2006/7/4 - Seung-Won, Bae) Retrieve the Packet Setting Info
	virtual IString *	GetPacketInfoString( void);
	// (2006/7/25 - Seung-Won, Bae) Get/Set Normalized Minute Time Rule
	virtual BOOL		GetNormalizedMinuteTimeRule( void);
	virtual BOOL		SetNormalizedMinuteTimeRule( BOOL p_bEnable);
	// (2006/7/26 - Seung-Won, Bae) Check Logging State
	virtual BOOL		IsInLogging( void);
	// (2006/7/26 - Seung-Won, Bae) Check Mode in Real Usage
	virtual BOOL		IsInUseRealData( void);
	// (2006/7/27 - Seung-Won, Bae) Get Redraw after Updated All Packet Flag
	virtual BOOL		GetRedrawAfterUpdatedAllPacketFlag( void);

// (2006/1/24 - Seung-Won, Bae) Interface for Analysis Tool Option
protected:
	virtual BOOL		IsShowTrendLineText( void)		const;
	virtual BOOL		IsShowHorizonTextLeft( void)	const;
	virtual BOOL		IsShowHorizonTextRight( void)	const;
	virtual BOOL		IsShowVerticalText( void)		const;
	virtual BOOL		IsShowThreeText( void)			const;
	virtual BOOL		IsShowFourText( void)			const;
	virtual BOOL		IsShowFiboText( void)			const;
	virtual BOOL		SetAnalysisToolTextFlag( BOOL p_bTrendLine, BOOL p_bHorizonLeft, BOOL p_bHorizonRight, BOOL p_bVerticalLine, BOOL p_bThreeLine, BOOL p_bFourLine, BOOL p_bFiboLine);

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
	virtual void	DragZoomIn( const POINT p_ptStart, const POINT p_ptEnd);
	// (2006/7/27 - Seung-Won, Bae) Reset Scroll
	virtual BOOL	SetScrollData( const int nType = 0, const BOOL bIsHorzScaleDataCheck = TRUE, const BOOL bIsRedraw = FALSE);
	// (2006/8/16 - Seung-Won, Bae) Right Virtual Candle
	virtual int		GetRightMargin( void);
	virtual BOOL	SetRightMargin( const int p_nCount);


// [04.10.11] ���� ���� Interface
protected:
	// [04.10.11] �־��� Global ������ �����Ǿ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
	virtual BOOL		CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet);
	// [04.10.11] �־��� Global ������ ������ �߻���Ű�� Interface�� �����Ѵ�.
	virtual BOOL		FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData);
	// (2006/7/25 - Seung-Won, Bae) Set Local Shared Info with manual.
	virtual BOOL		AddLocalShareInfo( const char *p_szLSharedName, int p_nSettingType, const char *p_szLocalSharedData);
	// (2006/7/25 - Seung-Won, Bae) Get Local Shared Data.
	virtual IString *	GetLocalSharedDataString( const char *p_szLSharedName);
	// (2006/7/26 - Seung-Won, Bae) Remove Local Shared Info with manual.
	virtual BOOL		RemoveLocalShareInfo( const char *p_szLSharedName);


// (2004.10.19, ��¿�) Image ���� �� ����� ���� Drawing Interface�� �����Ѵ�.
protected:
	virtual void	DrawForImage( HDC p_hDC, RECT p_rctSource, BOOL p_bUseScreenColor);

// ����(05/06/07) �׷����� �߰�,����,���� ���� Interface�� �����Ѵ�.
protected:
	virtual	BOOL	AddChartBlock( const char *p_szIndicatorName);
	virtual void	AddPacket(const char* pcszPacketOption);	// Packet �߰�.
	virtual void	AddGraph(short nRow, short nColumn, short nVertScaleGroup, const char* szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale);	// Graph �߰�.
	virtual bool	ChangeSubGraphData(const char* pcszGraphName, const char* pcszSubGraphName, COLORREF clrLineColor, int nThickness, int nDrawStyle);	// �׷����� �Ӽ�����.
	virtual bool	SetStartPeriodOfPacket(const char* pcszPacketName, long lStartPeriod, BOOL bAddSpace);	// Packet�� �������ڸ� �����Ѵ�.
	virtual bool	SetStartIndexOfPacket(const char* pcszPacketName, int nStartDataIndex, BOOL bAddSpace);
	virtual bool	HasGraph(const char* pcszGraphName);	// �ش� Graph�� �ִ��� Ȯ���Ѵ�.
	virtual BOOL	HasGraphInSpecialBack( const char *p_szGraphName);
	virtual bool	RemoveGraph(const char* pcszGraphName);
	virtual bool	RemovePacket(const char* pcszPacketName, const bool bIsOnlyDataClear);

// (2006/1/21 - Seung-Won, Bae) Operations
protected:
	// ����(05/08/22) ������ ó���Ҽ� �ִ� Interface�� �����Ѵ�.
	// nOption = 1:Update, 2:Append
	virtual void	SetRealData(const char* pcszPacketName, double dData, short nOption);
	// (2006/1/23 - Seung-Won, Bae) Set Logging Flag
	virtual void	SetLoggingFlag( const BOOL p_bInLogging);
	// (2006/1/28 - Seung-Won, Bae) Reset Tool UI
	virtual void	ReleaseOtherToolUI( const CToolOptionInfo::TOOLOPTION p_eToolOption);

// (2006/1/17 - Seung-Won, Bae) Notify from AddIn
protected:
	// (2005/8/18 - Seung-Won, Bae) for Event from AddIn
	virtual void	FireAddInEvent( const char *p_szAddInName, short p_nEventID, const char *p_szData, double p_dData, long p_lData);
	// [04.10.11] DLL Container���� Message�ϴ� Interface�� �����Ѵ�.
	virtual LRESULT	SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam);

// (2006/3/23 - Seung-Won, Bae) Set/Get StyleData
protected:	// Get
	virtual	COLORREF	GetStyleValueGraphBGColor(		void);
	virtual	COLORREF	GetStyleValueGraphBorderColor(	void);
	virtual	COLORREF	GetStyleValueGraphGridColor(	void);
	virtual	int			GetStyleValueGridHeight(		void);
	virtual	int			GetStyleValueGridWidth(			void);
	virtual	COLORREF	GetStyleValueGridHLineColor(	void);
	virtual	COLORREF	GetStyleValueGridVLineColor(	void);
	virtual	COLORREF	GetStyleValueScaleTextColor(	void);
	virtual	int			GetFirstBlockHGridLineType(		void);
	virtual	int			GetFirstBlockVGridLineType(		void);
	virtual	COLORREF	GetStyleValueScaleBGColor(		void);
	virtual	COLORREF	GetStyleValueScaleBorderColor(	void);
	virtual	COLORREF	GetStyleValueAnalysisToolColor( void);
	virtual	COLORREF	GetStyleValueNIDLGColor(		void);
public:	// SetGraphRegionColor
	virtual	BOOL		SetStyleValueGraphBGColor(		COLORREF	p_clrGraphBG,		BOOL p_bWithUpdate = FALSE);
	virtual	BOOL		SetStyleValueGraphBorderColor(	COLORREF	p_clrGraphBorder,	BOOL p_bWithUpdate = FALSE);
	virtual	BOOL		SetStyleValueGraphGridColor(	COLORREF	p_clrGraphGrid,		BOOL p_bWithUpdate = FALSE);
public:	// SetGraphBackgroundGap
	virtual	BOOL		SetStyleValueGridHeight(		int			p_nGridHeight,		BOOL p_bWithUpdate = FALSE);
	virtual	BOOL		SetStyleValueGridWidth(			int			p_nGridWidth,		BOOL p_bWithUpdate = FALSE);
public:	// SetScaleColor
	virtual	BOOL		SetStyleValueGridHLineColor(	COLORREF	p_clrGridHLine,		BOOL p_bWithUpdate = FALSE);
	virtual	BOOL		SetStyleValueGridVLineColor(	COLORREF	p_clrGridVLine,		BOOL p_bWithUpdate = FALSE);
	virtual	BOOL		SetStyleValueScaleTextColor(	COLORREF	p_clrScaleText,		BOOL p_bWithUpdate = FALSE);
public:	// SetAllHorzScaleGridType & SetAllVertScaleGridType
	virtual	BOOL		SetAllHorzScaleGridType(		int			p_nGridHLineType,	BOOL p_bWithUpdate = FALSE);
	virtual	BOOL		SetAllVertScaleGridType(		int			p_nGridVLineType,	BOOL p_bWithUpdate = FALSE);
public:	// Updated in OnDraw
	virtual	BOOL		SetStyleValueScaleBGColor(		COLORREF	p_clrScaleBG);
	virtual	BOOL		SetStyleValueScaleBorderColor(	COLORREF	p_clrScaleBorder);
public:	// Updated in EAI_ALL_APPLY_STYLE
	virtual	BOOL		SetStyleValueAnalysisToolColor(	COLORREF	p_clrAnalysisTool);
	virtual	BOOL		SetStyleValueNIDLGColor(		COLORREF	p_clrNIDL);

// (2006/8/16 - Seung-Won, Bae) Set/Get the Graph Title Option
protected:
	virtual BOOL	GetGraphTitleOption( BOOL &p_bWithConditionValue, BOOL &p_bWithLastValue);
	virtual BOOL	SetGraphTitleOption( BOOL p_bWithConditionValue, BOOL p_bWithLastValue);

// (2006/8/15 - Seung-Won, Bae) with SaveChartCFG
protected:
	virtual BOOL	SaveChartCFG_RemoveFromIndexToEnd( const int p_nIndex);
	virtual BOOL	SaveChartCFG_ResetWithCurrentStyle( void);
	virtual BOOL	SaveChartCFG_RestoreChartOfIndex( const int p_nIndex);
	virtual int		SaveChartCFG_SaveNow( void);
};

#endif // !defined(AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_)
