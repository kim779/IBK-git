// IWChartOcx050116.h: interface for the CIWChartOcx050116 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWCHARTOCX050116_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_)
#define AFX_IWCHARTOCX050116_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����!	050116 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� Wrapper�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		������ �ʴ� ������ 050116 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050116 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			��¿�
//			ChartPatternAddIn			GZone			������
//			ChartGuideLine				GZone			������
//////////////////////////////////////////////////////////////////////

// (2004.10.08, ��¿�) 050116 Version�� Chart Ocx Interface Wrapper Class�� �����Ѵ�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\IWUnknown.h"		// for CIWUnknown
#include "IChartOcx050116.h"	// for IChartOcx050116

class MChartOcx;
class CIWChartOcx050116 : public CIWUnknown, public IChartOcx050116
{
public:
	CIWChartOcx050116( MChartOcx *p_pChartOCX);
	virtual ~CIWChartOcx050116();

// (2004.10.07, ��¿�) Interface Wrapper�� �⺻ Interface (IUnknown��)�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2004.10.08, ��¿�)	Chart OCX�� Pointer�� �����Ѵ�.
//		������ �� OCX�� Ctrl Class�̸�, StdAfx.h�� Macro�� �����Ͽ� �̿��Ѵ�.
protected:
	MChartOcx *		m_pChartOcx;

// (2004.10.19, ��¿�) Windows ���� Interface�� �����Ѵ�.
public:
	// [04.10.11] ReDrawing�� ���� Interface�� �����Ѵ�.
	virtual	void	InvalidateControl( void);
	// [04/10/11] ReDrawing�� ���� Interface�� �����Ѵ�.
	virtual HWND	GetOcxHwnd( void);
	// [04/11/02] Counter Capturing Interface�� �����Ѵ�.
	virtual HWND	AddSetCapture( void);
	// [04/11/02] Counter Capturing Interface�� �����Ѵ�.
	virtual BOOL	RemoveSetCapture( void);

// (2004.10.18, ��¿�) ȯ�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	// [04.10.18] ����� Folder�� ���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetUserFilePath( CString &p_strUserFilePath);
	// [04.10.25] Chart ���� Font�� �����Ѵ�.
	virtual CFont *		GetChartFont( void);

// (2004.10.11, ��¿�) Chart�� �� ���¸� Ȯ���ϴ� Interface�� �����Ѵ�.
public:
	// [04.10.11] Ư����Ʈ�� ������� Ȯ���ϴ� Interface�� �����Ѵ�. (���ð�/�Ｑ��ȯ��/P&F)
	virtual BOOL	IsOnSpecialChart( void);

// (2005.01.20, ��¿�) ToolBar�� ���� Item�� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
public:
	virtual	BOOL	HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem);

// (2004.10.11, ��¿�) Data Index �� Position ���� Interface
public:
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

// [04.10.11] ���� ���� Interface
public:
	// [04.10.11] �־��� Global ������ �����Ǿ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet);
	// [04.10.11] �־��� Global ������ ������ �߻���Ű�� Interface�� �����Ѵ�.
	virtual BOOL	FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData);

// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
public:
	virtual IChartManager050116 *		GetIChartManager050116( void);
	virtual IPacketManager050116 *		GetIPacketManager050116( void);
	virtual IGraphInfoManager050116 *	GetIGraphInfoManager050116( void);

// (2004.10.19, ��¿�) Image ���� �� ����� ���� Drawing Interface�� �����Ѵ�.
public:
	virtual void	DrawForImage( CDC *p_pDC, CRect p_rctSource, BOOL p_bUseScreenColor);

// (2004.10.25, ��¿�) Cursor ���� Interface�� �����Ѵ�.
public:
	// [04/10/25] ���ο� Cursor�� �����ϸ鼭, ���� ������ Cursor�� Ȯ���Ѵ�.
	virtual HCURSOR		SetCursorHandle( HCURSOR p_hNewCursor);
	// [04/10/26] ���� ������ Cursor�� Ȯ���Ѵ�.
	virtual HCURSOR		GetCursorHandle( void);

// (2005.01.13, ��¿�) ��Ʈ�� ��ü���� Mode ���θ� Ȯ�� �� �����ϴ� Interface�� �����Ѵ�.
public:
	virtual BOOL		CheckFullView( void) const;
	virtual void		SetFullView( BOOL p_bFullView);

//////////////////////////////////////////////////////////////////////
// 050116 Version, ���� Interface
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			��¿�
//////////////////////////////////////////////////////////////////////

// Temp Interface I : DLL Container ���� Interface
public:
	// [04.10.11] DLL Container���� Message�ϴ� Interface�� �����Ѵ�.
	virtual LRESULT		SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam);
};

#endif // !defined(AFX_IWCHARTOCX050116_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_)
