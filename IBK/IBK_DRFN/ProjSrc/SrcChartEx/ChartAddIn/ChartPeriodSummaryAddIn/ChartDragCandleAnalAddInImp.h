// ChartDragCandleAnalAddInImp.h: interface for the CChartDragCandleAnalAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTDRAGCANDLEANALADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
#define AFX_CHARTDRAGCANDLEANALADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Include_AddIn/Include_AddIn.h"							// for Ixxxxxx.h
#include "./Include_Chart/IPropertyMap.h"

class CChartDragCandleAnalAddInImp : public CChartAddInBase
{
public:
	CChartDragCandleAnalAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartDragCandleAnalAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;
	virtual BOOL InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData);
	
// save point for inflate when mouse drag & drop
protected:
	CPoint	m_ptStartDragDrop;
	CPoint	m_ptCurrDragDrop;
	CPoint	m_ptStart, m_ptEnd;
	long	m_lStartDate, m_lEndDate;
	int		m_nStartIndex, m_nEndIndex;

// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
protected:
	DWORD	m_dwMouseActionID;

// (2006/2/6 - Seung-Won, Bae) Draw Trace Rectangle
protected:
	CRect DrawDragRect(CDC * pdc, CPoint ptFirst, CPoint ptCurr, int nWeight);

// ( 2006 / 11 / 27 - Sang-Woo, Cho ) 
	BOOL	m_bRun;

protected:
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC);

protected:
	virtual void	OnDrawAfterGraph( HDC p_hDC);		// ��Ʈ���� �� �ʿ� �׸� ��
	virtual BOOL	OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual void OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);	// �����Ͱ� ��� �� �� ȣ���
	virtual void OnResetChart( void);	// ��Ʈ�� ���µ� �� ȣ���

protected:
	CRect	DrawFill(CDC *pDC, const CPoint &Startpt, const CPoint &Endpt, const CRect& drawingRegion);
	void	SelectClipRegion(CDC* pDC, const CRect& rect);
	void	SelectClipRegion(CDC* pDC);
	BOOL	GetChartBasicData(int &p_nStartIndex, int &p_nEndIndex, int &p_nR, int &p_nC, CRect &p_rctBlockRegion, CRect &p_rctGraphRegion);
	int		GetIndexFromPoint(int x, int nStartIndex, int nEndIndex, CRect rctBlockRegion);
	int		GetIndexFromValue(CString strTime, int nStartIndex, int nEndIndex);
	int		GetXPositoin(double dDataOffset, double dCandleWidth, CRect rctGraphRegion);
	BOOL	m_bLog;
	BOOL	m_bReverse;
	double	m_dViewMin, m_dViewMax;
	CList<CString, CString&>		m_listDataWindow;
	BOOL AddData(int nStart, int nEnd);
	CString TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/);	// ��Ʈ���� ����
};

#endif // !defined(AFX_CHARTDRAGCANDLEANALADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
