// RadarControl.h: interface for the CToolRadarControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RADARCONTROL_H__BB35196F_CA15_446B_A5D1_40E5BED68D92__INCLUDED_)
#define AFX_RADARCONTROL_H__BB35196F_CA15_446B_A5D1_40E5BED68D92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRadar;
class CToolBarDlg;
interface IChartCtrl;
class CRadarControl  
{
public:
	CRadarControl();
	virtual ~CRadarControl();

// (2003.12.02, ��¿�) Radar Chart�� ����/������ ó���Ѵ�.
protected:
	BOOL	m_bOnRadar;

// (2003.12.02, ��¿�) Flat Button���� ������ Radar Chart�� Static���� ������ Index Status�� ����, �����Ѵ�.
protected:
	CRadar *					m_radar;
	CStatic *					m_static;
//public:
//	void				CreateRadarButton( CFont* pFont, CMiniToolBarDlg* MiniToolBarDlg, CRect controlItemRect,
//							CObjTool* objTool, CString filePath);

// (2003.12.02, ��¿�) Radar Status Static�� ����� Font�� �����Ѵ�.
protected:
	CFont *	m_pFont;

// (2003.12.02, ��¿�) Zoom & Scroll ���� ����� �����Ѵ�.
protected:
	// 1. Event�� �����ϱ� ���Ͽ� ���� ToolBar Object Pointer�� �����Ѵ�.
//	CMiniToolBarDlg *	m_pMiniToolBarDlg;
	// 2. Radar Chart���� ������ Data View ������ �����Ѵ�.
	//		(ó�� �����ÿ� Zoom Full Mode�� �����Ǵ� ���������� SetDataViewInfo�� ȣ����� �ʾ� ���� ȣ���Ѵ�.)
	int		m_nDataSize;
	int		m_nDataStartIndex;
	int		m_nOnePageDataCount;
public:
	// 1. Zoom Full Mode ���ο� ������ Data View ������ �Բ� ���޹޾� ó���Ѵ�.
	//		(Radar�� ��� Zoom Full Mode���� Data�� ���������� �ݿ��Ѵ�.)
	void	SetDataViewInfo( int nDataSize, int nDataStartIndex, int nOnePageDataCount, BOOL p_bZoomFullMode);

// (2004.03.11, ��¿�) ToolBar�� ũ�Ⱑ ����Ǵ� ���, Radar�� ��ġ�� ��������Ѵ�.
public:
	void	OnToolBarPositionChanged( CRect p_rctToolRect);

// ����(04/11/04) Create�Լ� �߰��Ѵ�. CCtrlFactory���� �� �Լ��� ȣ���Ͽ� RadarChartControl�� �����Ѵ�.
public:
	bool	Create(CWnd* pParent, UINT nID);

// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
protected:
	IChartCtrl	*m_pIChartCtrl;
public:
	void	SetIChartCtrl( IChartCtrl *p_pIChartCtrl)	{	m_pIChartCtrl = p_pIChartCtrl;	}
};

#endif // !defined(AFX_RADARCONTROL_H__BB35196F_CA15_446B_A5D1_40E5BED68D92__INCLUDED_)
