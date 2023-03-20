// PanAddInImp.h: interface for the CPanAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANADDINIMP_H__17056A3D_4502_4194_B02A_8C63D0368259__INCLUDED_)
#define AFX_PANADDINIMP_H__17056A3D_4502_4194_B02A_8C63D0368259__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../Include_Addin/I000000/_ChartAddInBase.h"				// for CChartAddInBase


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 24
// Comments		: 
// Using way	: 
// See			: Chartȭ���� Mouse�� �̵��� �� �ְ� �ϴ� AddIn
//-----------------------------------------------------------------------------
class CPanAddInImp : public CChartAddInBase  
{
public:
	CPanAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CPanAddInImp();

	DECLARE_ADDIN_EVENT_MAP()


// Multiple Item in DLL
public:
	static CString m_strAddInItemName;

// save point for inflate when mouse drag & drop
protected:
	CPoint	m_ptPanStart;
	double  m_dPanStart;

	int		m_nStartIndex, m_nEndIndex;
	double  m_dBarWidth;
	double  m_dViewMin, m_dViewMax;

// Manage Mouse Action ID for Serial Action.
protected:
	BOOL	m_bRun;

	DWORD	m_dwMouseActionID;
	BOOL	m_bLButtonDown;

	CRect	m_rcGraphRegion;
	CRect   m_rctGraphDrawingRegion;

	BOOL    m_bIsLock;

	BOOL	m_bLog;
	BOOL	m_bReverse;

	
// �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
protected:
	// Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void OnToolCommand( const int p_nToolType, const int p_nCmdOption );
		
	virtual BOOL	OnMouseMove(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	virtual BOOL	OnLButtonDown(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonUp(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
// 2008.10.20 by LYH >> ������ ���콺 ������ �д� ��� Ǯ����
	virtual BOOL	OnRButtonUp(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
// 2008.10.20 by LYH <<
	
// (2008/12/11 - Seung-Won, Bae) for Y Zoom.
protected:
	int		m_nRow;
	int		m_nColumn;
	CPoint	m_ptYZoomStart;
	int		m_nVScaleIndex;
	int		m_nScaleHeight;
protected:
	BOOL	OnLButtonDownForYZoom(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	BOOL	OnMouseMoveForYZoom(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	BOOL	OnLButtonUpForYZoom(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_PANADDINIMP_H__17056A3D_4502_4194_B02A_8C63D0368259__INCLUDED_)
