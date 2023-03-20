// ToolBarManagerImplementation.h: interface for the CToolBarManagerImplementation class.
// �������� ���ٸ� �����ϴ� Ŭ������ ������ ��Ʈ�ѵ��� �߾����߽����� �� Ŭ������ �����
// �Ǿ��ִ�.
// �� Ŭ���������� �ΰ��� Dialog�� �����Ѵ�. �̴�����Dlg�� ��ġ��ȸDlg.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBARMANAGERIMPLEMENTATION_H__523BA067_CD45_4CD0_B61B_7F1A6291D3DF__INCLUDED_)
#define AFX_TOOLBARMANAGERIMPLEMENTATION_H__523BA067_CD45_4CD0_B61B_7F1A6291D3DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/BlockBaseEnum.h"
#include "../Include_Chart/BlockBaseData.h"

interface IChartCtrl;
class CMainBlock;
class CPacketList;
class CPacketListManager;
class CXScaleManager;
class CToolOption;
class CToolBarDlg;
class CRowViewData;
class CIndicatorList;
class COutsideCommonInterface;
class CNumericalInquiryDlg;
class CToolBarManagerImplementation  
{
// Function
public:
	CToolBarManagerImplementation( IChartCtrl *p_pIChartCtrl);
	virtual ~CToolBarManagerImplementation();

// Field
private:	
	CToolBarDlg* m_pToolBarDlg;						// �̴����� ���̾�α�.

	CToolOption *		m_pToolOption;
	IChartCtrl *		m_pIChartCtrl;	// OCX�� �������̽�
	CPacketList *		m_pPacketList;
	CPacketListManager* m_pPacketListManager;
	CXScaleManager*		m_pxScaleManager;
	CMainBlock *		m_pMainBlock;
	CIndicatorList *	m_pIndicatorList;
	CRect m_rectToolBarDlg;					// ToolBar DLL�� ������ ����.
	int m_nMaxRange;						// MaxRange
	int m_nDataStartIndex;					// ���� �ε���
	int m_nDataEndIndex;					// �� �ε���

private:
	bool FilterSimpleInquiryData(const CString& strPacketNames);	// ������ ��ȸ����Ÿ�� �����ϱ� ���� �Լ��� ���ʿ��� ��ȸ����Ÿ�� �����ϱ� ����.

private:
	const CRect CalculateToolBarRect(int& nScrollBarWidth, bool& bRadarChart, bool& bStatusBar);	// ���ٿ��� ���.(������)
	bool		CreateToolBarDialog();	// ���� Dialog ����.
	bool		CreateToolBarControl(const CString& strImageFile, const int nScrollBarWidth, const bool bRadarChart, const bool bStatusBar);	// ������ ����Ctrl�� ����.

public:
	//-----------------------------����.
	bool CreateToolBar(const CString& strToolBarOption, const CString& strImageFile);	// ���� ����.
	void ToolBarPosChanged(const CRect rectToolBarPos);	// ���� ��ġ ���.
	
	void SetSliderMin(const int nSliderMin);	// Slider�� Min Range�����ϱ�.
	void SetControlData(CPacketList* pPacketList, CPacketListManager* pPacketListManager, CXScaleManager* pxScaleManager, CMainBlock* pMainBlock, CIndicatorList* pIndicatorList);	// �⺻ ��������Ÿ ����.
	void SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex);	// Graph���� ����.
	void SetLineThickness(int nThickness);	// ������ ����
	void SetGraphColor(COLORREF clrGraphColor);	// ���� ����.
	void SetOneChart(bool bOneChart, bool p_bIsNoZoomChart);	// ����Ʈ ����.
	void SetStartEndIndexToOcx(const int nStart, const int nEnd);	// Start,End Index�� OCX�� ������.

	const CRect			GetToolBarRect();	// ������ ������ ����.
	void				SetToolBarRect( const CRect p_rctToolBar)	{	m_rectToolBarDlg = p_rctToolBar;	}	// (2006/4/14 - Seung-Won, Bae) Reset New ToolBar Rect by CToolBarDlg
	bool				GetStartEndIndex(int& nStartIndex, int& nEndIndex);	// Data�� Start, End Index ����.
	int					GetMaxRange() const;	// SliderBar�� Max Range ����.
	HWND				GetToolBarHwnd();	// ToolBar Dialog�� �������ڵ��� ����.
	
	void ReDraw();				//���ٸ� �׸���.
	bool IsSpecialIndicator();	// ���� Ư����ǥ�� ����������� ����.
	// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
	bool IsRunningNoZoomChart();
	bool IsToolScrollBar();	// ��ũ�ѹٸ� ����ϴ��� ���θ� ����.
	void ReleaseOtherButtonUI(CToolOptionInfo::TOOLOPTION enumToolOption);	// ������ ��ư ����.
	BOOL UseToolOption(CToolOptionInfo::TOOLOPTION enumToolOption);	// ��ư ��� ����.
	BOOL GetToolOption(CToolOptionInfo::TOOLOPTION enumToolOption);	// ��ư ��� ���� ����.
	
	//--------------------------------------------------��ġ��ȸâ.
public:
	void SetNumericalInquiryDlgMoveState(const short nMove);	// �̵�/����/���콺����ٴϱ� ����.
	void OnNumericalInquiryOptionChanged( const long p_lNumericalInquiryOption);	// (2006/3/4 - Seung-Won, Bae) Notify Cursor Mode Change for Drag Cross Line & Numercial Inquiry DLG
	CString GetRealPacketNames(const CString& strTitle, const CString& strPacketNames) const;				// 1ƽ�ϰ�� ��Ŷ����.

	// (2006/11/15 - Seung-Won, Bae) Tool On/Off State Changed.
	BOOL	OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn);

// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
public:
	COutsideCommonInterface *GetMChartInterface( void)	{	return NULL;	}
	int GetPageCountNextStepToOcx(const int nSliderPos_Prev, const int nSliderPos_New, const UINT nSBCode, int nMin = -1, int nMax = -1);
	BOOL GetReverseToOcx();

// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
public:
	void	UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount, BOOL p_bWholeView);
};

#endif // !defined(AFX_TOOLBARMANAGERIMPLEMENTATION_H__523BA067_CD45_4CD0_B61B_7F1A6291D3DF__INCLUDED_)
