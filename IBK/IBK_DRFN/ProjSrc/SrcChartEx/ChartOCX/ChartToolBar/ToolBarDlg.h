#if !defined(AFX_TOOLBARDLG_H__C5096DEC_1C31_4AF0_AC80_7DCE5DA597B3__INCLUDED_)
#define AFX_TOOLBARDLG_H__C5096DEC_1C31_4AF0_AC80_7DCE5DA597B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolBarDlg.h : header file
//

#include "Resource.h"
#include "BlockBaseEnum.h"
#include "BlockBaseData.h"
#include <AFXTEMPL.H>

/////////////////////////////////////////////////////////////////////////////
// CToolBarDlg dialog

class CToolOption;
class CScrollBarEx;
class CSliderCtrlEx;
class CGraphStyleCombo;
class CGraphLineThicknessCombo;
class CToolBarManagerImplementation;
class CRadarControl;
class CPacketList;
class CPacketListManager;
class CXScaleManager;
class CButtonEx;
class CButtonProperty;
interface IChartCtrl;

typedef CList<CWnd*, CWnd*> CWndList;	// ���ٿ� ��ϵ� ��Ʈ�ѵ��� ��� ����ƮŸ�� ����.
typedef CList<CButtonEx*, CButtonEx*> CButtonList;	// ���ٿ� ��ϵ� ��ư���� ��� ����Ʈ Ÿ�� ����.

class CToolBarDlg : public CDialog
{
// Construction
public:
	CToolBarDlg(CWnd* pParent, CToolBarManagerImplementation* pToolBarImpl, IChartCtrl *p_pIChartCtrl);   // standard constructor
	virtual ~CToolBarDlg();
// Dialog Data
	//{{AFX_DATA(CToolBarDlg)
	enum { IDD = IDD_TOOLBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolBarDlg)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Function
private:
	bool MakeBitMap(const CString& strFileName);
	void ToggleBT(const CToolOptionInfo::TOOLOPTION eToolOption, const bool bState);

public:
	bool CreateCtrl(CToolOption* pToolOption, const CFont* pFont, const CString& strImageFile, const int nScrollWidth, const bool bRadarChart, const bool bStatusBar);
	void DeleteCtrl();
	void MoveToolBarDlg( void);

	// OCX�� ���� �����ϴ� �Լ���.
	void SetStartEndIndexToOcx(const int nStartIndex, const int nEndIndex);
	void SetToolFunction(CToolOptionInfo::TOOLOPTION enumToolOption, CButtonEx* pButton = NULL, long nParam = 0);
	void ReleaseToolFunction(CToolOptionInfo::TOOLOPTION enumToolOption, CButtonEx* pButton = NULL, long nParam = 0);
	void SetStateWholeView(const int nState);	

	// ���� ����� ����
	void ExecuteZoomInZoomOut(CButtonEx* pButton);	// Zoom In/Out ����
	void ExecuteRadar();							// Radar Chart ����
	void ExecuteGraphColorSetting();				// Graph Color����â ����


	// OCX���� ���� �������� �Լ���.
	int GetCountMinPerPage();
	bool IsWholeView();

	// ToolBar�� ���� �����ϴ� �Լ���.
	void SetControlData(CPacketList* pPacketList, CPacketListManager* pPacketListManager, CXScaleManager* pxScaleManager);
	void SetScrollData_TR(const int nTotalDataCount, const int nPageDataCount, const bool bFullPage, CPacketDataInfo::DATAINSERTTYPE dataType, const int nGlanceBalanceMargin);
	void SetScrollData_Real(const int nTotalDataCount, const int nPageDataCount, const bool bFullPage);
	void SetScrollData_RemovePacket(const int nTotalDataCount, const bool bFullPage, const int nRemoveDataCount);
	void SetSliderPos(int nPageDataCount);
	void SetSliderMin(const int nSliderMin);
	void SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex);
	void SetLineThickness(int nThickness);
	void SetGraphColor(COLORREF clrGraphColor);
	void SetOneChart( bool bOneChart, bool p_bIsNoZoomChart);

	// ToolBar���� ���� �̾Ƴ��� �Լ���.
	int GetDataCount();
	bool GetStartEndIndex(int& nStartIndex, int& nEndIndex);
	CPacketList* GetPacketList();
	CPacketListManager* GetPacketListManager();
	CXScaleManager* GetXScaleManager();
	CFont* GetResourceFont();
	bool GetOneChart();
	// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
	bool	IsNoneZoomChart( void)		{	return m_bIsNoZoomChart;	}
	int GetMaxRange();
	CButtonEx* GetButtonEx(CToolOptionInfo::TOOLOPTION enumToolOption);

	void ReDraw();
	bool IsScrollBar();
	void ReleaseOtherButtonUI(CToolOptionInfo::TOOLOPTION enumToolOption, const CString& strParam = _T(""));

	int GetPageCountNextStepToOcx(const int nSliderPos_Prev, const int nSliderPos_New, const UINT nSBCode, int nMin = -1, int nMax = -1);
	BOOL GetReverseToOcx();

	// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
	void	UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount, BOOL p_bWholeView);

// Field
public:
	static CBitmap m_bitmapButton;	// ButtonŬ�������� Draw�Ҷ� �ʿ�� �ϴ� ���̴� ���� �������ϵ��� Public������ ��.
	CPacketListManager* m_pPacketListManager;
	CXScaleManager* m_pxScaleManager;
	
private:
	CSliderCtrlEx* m_pSliderCtrl;				// Slider Bar ��Ʈ��
	CScrollBarEx* m_pScrollBar;					// Scroll Bar ��Ʈ��
	CGraphStyleCombo* m_pGraphStyleCombo;		// �׷�����Ÿ�� �޺��ڽ� ��Ʈ��
	CGraphLineThicknessCombo* m_pGraphLineCombo;// ������ �޺��ڽ� ��Ʈ��
	CRadarControl* m_pRadarControl;				// ���̴���Ʈ ��Ʈ��
	COLORREF m_clrSelection;					// �� ������.
	int m_nSliderMin;							// Min Range
	int m_nSliderMax;							// Max Range
	int m_nRemainCount;							// ������ ���� ���� ����Ÿī��Ʈ(Ȯ��/��ҽ�)
	int m_nGlanceBalanceMargin;					// �ϸ����ǥ�� �� ����.
	int m_nScrollWidth;							// Scroll Bar�� �ʺ�.
	int m_nScrollingDataCount;					// �ڵ���ũ���� ����Ÿ ����.
	bool m_bCheckGlanceBalance;					// �ϸ����ǥ ��� ����
	bool m_bRadarChart;
	bool m_bStatusBar;							// '��ġ��ȸ����Ÿ����'�� Status Bar ��� ����
	bool m_bOneChart;							// ������Ʈ ��� ����
	bool m_bIsNoZoomChart;						// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
	BOOL m_bUseRadarChart;						// Radar Chart ��� ����
	CToolBarManagerImplementation* m_pToolBarManagerImple;
	CWndList m_listToolBarCtrl;					// ����Dlg�� ��ϵ� ��Ʈ���� ��Ƶ� List.
	CButtonList m_listToolButton;				// Dlg�� ��ϵ� ����ư ����Ʈ.
	CButtonProperty* m_pButtonProperty;			// ����ư �Ӽ��� ���� State ���¸� �����ϴ� ������Ʈ
	CPacketList* m_pPacketList;
	// (2006/11/18 - Seung-Won, Bae) Support Chart OCX Ctrl Interface
	IChartCtrl *	m_pIChartCtrl;

	// (2007/2/8 - Seung-Won, Bae) Disable WholeView Btn in Special Chart.
	CButtonEx *	m_pWholeViewBtn;
	// (2007/2/8 - Seung-Won, Bae) Disable Zoom Btn in Special Chart.
	CButtonEx *	m_pZoomInBtn;
	CButtonEx *	m_pZoomOutBtn;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARDLG_H__C5096DEC_1C31_4AF0_AC80_7DCE5DA597B3__INCLUDED_)
