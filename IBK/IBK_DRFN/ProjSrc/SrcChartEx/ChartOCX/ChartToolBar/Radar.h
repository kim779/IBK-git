#if !defined(AFX_RADAR_H__FCAD36A5_D98B_11D6_8777_00E07D8FB0D6__INCLUDED_)
#define AFX_RADAR_H__FCAD36A5_D98B_11D6_8777_00E07D8FB0D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Radar.h : header file
// 

#include "../Include_Chart/OutsideCommonInterfaceFunction.h"		// for COutsideCommonInterfaceFunction

/////////////////////////////////////////////////////////////////////////////
// CRadar window
class CPacketList;
class CPacketListManager;
class CXScaleManager;
interface IChartCtrl;
// (2003.12.04, ��¿�, ����) LButtonDown�ó� Dragging�� ���� Client ������ ��츶��,
//		Drawing�� �ٽ� �߻��ϴ� ���� �����ϱ� ���Ͽ� CButton�� �ƴ϶� CWnd���� ��¹޴´�.
class CRadar : public CWnd
{
// Construction
public:
	CRadar( IChartCtrl *p_pIChartCtrl);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRadar)
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
protected:
	//{{AFX_MSG(CRadar)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// (2003.12.02, ��¿�) Index ���� ����� Radar Status Static�� �����Ѵ�.
protected:
	CStatic *	m_pstRadarStatus;
public:
	void		SetRadarStatus( CStatic *p_stRadarStatus)	{	m_pstRadarStatus = p_stRadarStatus;	}

// (2003.12.03, ��¿�) ����� Packet Data�� ������ �����޾� �����Ѵ�.
// (2003.12.04, ��¿�) ���� ������ ������ ���� ToolBar Dialog�� Pointer�� �Բ� �����Ѵ�.
protected:
	CString				m_strDataName;
	CPacketList*		m_pPacketList;
	CPacketListManager* m_pPacketListManager;
	CXScaleManager*		m_pxScaleManager;
	
public:
	void		SetData(CPacketList* pPacketList, CPacketListManager* pPacketListManager, CXScaleManager* pxScaleManager, CString& strDataName);	// ����(04/10/25) ����DLL. DLL�������� ���� ������.

// (2003.12.03) Zoom Full Mode Flag�� �����Ͽ�, �ش� Mode������ Mouse Tracing���� �۾��� �����Ѵ�.
// (2004.02.17) Zoom Full Mode�� ��ü���� ������ �ƴ϶� ���Ź��� �ƴ� ���� Chart�� ���,
//	Zoom & Scroll�� �����ϱ� ���� Mode�� �̿��Ѵ�.
protected:
	BOOL	m_bZoomFullMode;

// (2003.12.03, ��¿�) Zoom & Scroll ���� ����� �����Ѵ�.
//		(*.���� :	Rectagle�� Region�� �����Ͽ� �̿��Ѵ�.
//					Rectangle�� Right ��ǥ�� Bottom ��ǥ ������ ��꿡 ���Ե��� ������,
//					Region�� ��� ���Խ�Ų��.)
protected:
	// 1. Draw �ӵ��� ���� ó���ϵ��� Radar Chart�� Client ������ ���Ͽ� ���´�.
	CRect	m_rctRadarClient;
	// 2. Draw �ӵ��� ���� ó���ϱ� ���Ͽ� Radar Chart�� ���õ� ������ Rect�� ���Ͽ� ���´�.
	CRect	m_rctRadarSelect;
	// 3. Draw �ӵ��� ���� ó���ϱ� ���Ͽ� Graph Data�� Min/Max �� Index ������ �����Ѵ�.
	int		m_nRadarDataLeft;
	int		m_nRadarDataRight;
	double	m_dRadarDataMin;
	double	m_dRadarDataMax;
	// 4. Draw �ӵ��� ���� ó���ϱ� ���Ͽ� Graph�� �׷����� ������ ��ǥ�� �̸� ���Ͽ� ���´�.
	CRect	m_rgnRadarGraph;
	// 5. Draw �ӵ��� ���� ó���ϱ� ���Ͽ� Graph�� Scale ��ȯ ��ġ�� �̸� ���Ͽ� ���´�.
	double	m_dXScale, m_dYScale;
	// 6. Data View ������ �����Ѵ�.
	int		m_nDataStartIndex;
	int		m_nOnePageDataCount;
public:
	// 1. Data View ������ ���޹޾� Radar Chart�� �ݿ��ϴ� Interface�� �����Ѵ�.
	void	SetDataViewInfo( int nDataSize, int nDataStartIndex, int nOnePageDataCount, BOOL p_bZoomFullMode);

// (2003.12.04, ��¿�) ���� ���� ����� �߰��Ѵ�.
protected:
	// 1. Dragging ���� ������ Region Range�� �����Ѵ�.
	long	m_nDragSelectStart;
	long	m_nDragSelectEnd;
protected:
	// 1. Dragging ������ ����ϴ� Routine�� ������ �������Ѵ�.
	void	DrawDragSelectRectangle( CDC *pDC, long p_nDragSelectStart, long p_nDragSelectEnd, long *p_pPreDragSelectEnd = NULL);
	// 2. Mouse�� Point�� �ش��ϴ� Data Index��, Mouse Point�� �����ϸ� �����ϴ� Routine�� ���� �����Ѵ�.
	//		(������ ��û���� ���� ����� ������ -1�� Return�ȴ�.)
	//		����:1. Index ������ �־ �������� ��� m_rgnRadarGraph.left + 1�� �����Ѵ�.
	//					�װ��� Drawing�ÿ� Pen�� ���Ⱑ 2�� �Ͱ� �����Ǿ� ������ ���̴�.
	//			 2. ����, �̷��� ������ PointX�� ������ �ٽ� DataIndex�� �����ϸ� �������� �� ������,
	//					�����ϸ� 1ȸ�� ó���Ѵ�.
	int		GetDataIndex( long &p_nPointX, BOOL p_bWithCorrection = TRUE) const;
	// 3. ���۰� �� ������ �ش��ϴ� ���ÿ����� Rectangle�� ����ϴ� Routine�� �����Ѵ�.
	void	DrawRectangleInRegion( CDC *p_pDC, long p_nDragSelectStart, long p_nDragSelectEnd) const;

// (2004.04.16, ��¿�) Radar�� Resize�ÿ���, Resize�� Graph�� ���̵���,
//		Radar Graph�� �籸���ϴ� Routine�� ������ �����Ѵ�.
public:
	void	DrawNewRadarView( void);

// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
public:
	IChartCtrl *m_pIChartCtrl;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RADAR_H__FCAD36A5_D98B_11D6_8777_00E07D8FB0D6__INCLUDED_)
