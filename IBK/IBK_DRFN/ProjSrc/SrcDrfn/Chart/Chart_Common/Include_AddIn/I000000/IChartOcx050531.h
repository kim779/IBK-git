// IChartOcx050531.h: interface for the IChartOcx050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTOCX050531_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_)
#define AFX_ICHARTOCX050531_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����!	050531 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		Interface ���� ���� ���� ������ �ʽ��ϴ�.
//		������ �ʴ� ������ 050531 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			��ȭ			��¿�
//		ChartAssistIndexAddIn		��ȭ			���ٹ�
//////////////////////////////////////////////////////////////////////

// (2004.10.08, ��¿�) Char OCX�� Interface�� �����Ѵ�.
//		AddIn DLL ���� Interface�� Macro Interface�� �����Ѵ�.
#if IChartOcxVer < 050531
	#define	IChartOCX		IChartOcx050531
	#undef	IChartOcxVer
	#define	IChartOcxVer	050531
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IPacketManager050531.h"		// for IPacketManager050531
#include "IChartManager050531.h"		// for IChartManager050531
#include "IGraphInfoManager050531.h"	// for IGraphInfoManager050531
#include "../ChartDefines.h"			// for CToolOptionInfo::TOOLOPTION

interface IChartOcx050531 : public IUnknown  
{
protected:
	virtual ~IChartOcx050531() {}

// (2004.10.19, ��¿�) Windows ���� Interface�� �����Ѵ�.
public:
	// [04/10/11] ReDrawing�� ���� Interface�� �����Ѵ�.
	virtual	void	InvalidateControl( void) = 0;
	// [04/10/11] ReDrawing�� ���� Interface�� �����Ѵ�.
	virtual HWND	GetOcxHwnd( void) = 0;
	// [04/11/02] Counter Capturing Interface�� �����Ѵ�.
	virtual HWND	AddSetCapture( void) = 0;
	// [04/11/02] Counter Capturing Interface�� �����Ѵ�.
	virtual BOOL	RemoveSetCapture( void) = 0;

// (2004.10.18, ��¿�) ȯ�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/18] ����� Folder�� ���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetUserFilePath( CString &p_strUserFilePath) = 0;

// (2004.10.11, ��¿�) Chart�� �� ���¸� Ȯ���ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/11] Ư����Ʈ�� ������� Ȯ���ϴ� Interface�� �����Ѵ�. (���ð�/�Ｑ��ȯ��/P&F)
	virtual BOOL	IsOnSpecialChart( void) = 0;

// (2005.01.20, ��¿�) ToolBar�� ���� Item�� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
public:
	virtual	BOOL	HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem) = 0;

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

// (2004.10.11, ��¿�) ���� ���� Interface
public:
	// [04/10/11] �־��� Global ������ �����Ǿ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet) = 0;
	// [04/10/11] �־��� Global ������ ������ �߻���Ű�� Interface�� �����Ѵ�.
	virtual BOOL	FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData) = 0;

// [04/10/11] OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
public:
	virtual IChartManager050531 *		GetIChartManager050531( void) = 0;
	virtual IPacketManager050531 *		GetIPacketManager050531( void) = 0;
	virtual IGraphInfoManager050531 *	GetIGraphInfoManager050531( void) = 0;

// (2004.10.19, ��¿�) Image ���� �� ����� ���� Drawing Interface�� �����Ѵ�.
public:
	virtual void	DrawForImage( CDC *p_pDC, CRect p_rctSource, BOOL p_bUseScreenColor) = 0;

// (2004.10.25, ��¿�) Cursor ���� Interface�� �����Ѵ�.
public:
	// [04/10/25] ���ο� Cursor�� �����ϸ鼭, ���� ������ Cursor�� Ȯ���Ѵ�.
	virtual HCURSOR		SetCursorHandle( HCURSOR p_hNewCursor) = 0;
	// [04/10/26] ���� ������ Cursor�� Ȯ���Ѵ�.
	virtual HCURSOR		GetCursorHandle( void) = 0;

// (2004.10.25, ��¿�) Chart ���� Font�� �����Ѵ�.
public:
	virtual CFont *		GetChartFont( void) = 0;

// (2005.01.13, ��¿�) ��Ʈ�� ��ü���� Mode ���θ� Ȯ�� �� �����ϴ� Interface�� �����Ѵ�.
public:
	virtual BOOL		CheckFullView( void) const		= 0;
	virtual void		SetFullView( BOOL p_bFullView)	= 0;

// ����(05/06/01) �׷����� �߰�, ����, ���� ���� Interface�� �����Ѵ�.
public:
	virtual void		AddPacket(const char* pcszPacketOption) = 0;
	virtual void		AddGraph(short nRow, short nColumn, short nVertScaleGroup, const char* szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale) = 0;
	virtual bool		ChangeSubGraphData(const char* pcszIndicatorName, const char* pcszSubGraphName, COLORREF clrLineColor, int nThickness, int nDrawStyle) = 0;
	virtual bool		SetStartPeriodOfPacket(const char* pcszPacketName, long lStartPeriod, BOOL bAddSpace) = 0;
	virtual bool		SetStartIndexOfPacket(const char* pcszPacketName, int nStartDataIndex, BOOL bAddSpace) = 0;
	virtual bool		HasGraph(const char* pcszGraphName) = 0;
	virtual bool		RemoveGraph(const char* pcszIndicatorName) = 0;
	virtual bool		RemovePacket(const char* pcszPacketName, const bool bIsOnlyDataClear) = 0;
	

// ����(05/07/14) Hts�� Dir path�� �˷��ش�.
	virtual LPCTSTR		GetHtsPath(void) = 0;
	
	// ����(05/08/22) AddIn���� ���� ������ ������ �ִ� Interface�� �����Ѵ�.
	// nOption = 1:Update, 2:Append
	virtual void		SetRealData(const char* pcszPacketName, double dData, short nOption) =0;

// (2005/8/18 - Seung-Won, Bae) for Event from AddIn
public:
	virtual void		FireAddInEvent( const char *p_szAddInName, short p_nEventID, const char *p_szData, double p_dData, long p_lData) = 0;

//////////////////////////////////////////////////////////////////////
// 050531 Version, ���� Interface
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			Ez-Net			������
//////////////////////////////////////////////////////////////////////

// Temp Interface I : DLL Container ���� Interface
public:
	// [04/10/11] DLL Container���� Message�ϴ� Interface�� �����Ѵ�.
	virtual LRESULT		SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam) = 0;


// (2005/8/10 - Seung-Won, Bae) used in Only Hanwha
public:
	// (2005/8/10 - Seung-Won, Bae) Retrieve the Share Info Item Setting
	virtual BOOL	GetShareInfoItem( const char *p_szShareName, int &p_nShareType, int &p_nShareArea, long &p_lCodeFilter) = 0;
	// (2005/8/10 - Seung-Won, Bae) Fire Shared Data
	virtual BOOL	FireSharedData( const char *p_szShareName, const char *p_szSharedData) = 0;
};

#endif // !defined(AFX_ICHARTOCX050531_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_)
