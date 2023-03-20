// ToolBarManager.h: interface for the ToolBarManager class.
// ����DLL�� �������̽�.  ��������Ŭ����(�������̽�)�� ��������� �� ������ ����... T_T
// ��. �׸� ����� COM�� �Ǿ�����±�. ��~ �̷��� COM���Գ�����.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBARMANAGER_H__235DAFB3_898C_471D_8156_332FEDFFC006__INCLUDED_)
#define AFX_TOOLBARMANAGER_H__235DAFB3_898C_471D_8156_332FEDFFC006__INCLUDED_

#include "../BlockBaseEnum.h"
#include "../BlockBaseData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// (2004.05.25, ��¿�) ���� ToolBar�� ���̰�, ����� Interface�� �����Ѵ�.
//		������ Flag�� �ξ�, ToolBar�� OCX�� �������� �Ʒ��ʿ� ��ġ�ǵ��� �Ѵ�.
#define _OCX_INNERTOOLBAR_HEIGHT 17

class CToolBarManagerImplementation;
class CCommonInterface;
class CPacketList;
class CPacketListManager;
class CXScaleManager;
class CIndicatorList;
class CMainBlock;

interface IToolBarManager
{
// (2006/2/9 - Seung-Won, Bae) Destroy by OCX (Created in ChartToolBar.dll)
public:
	virtual ~IToolBarManager()	{}

public:
	// ���ٸ� �����Ѵ�.
	virtual bool CreateToolBar(const CString& strToolBarOption, const CString& strImageFile) = 0;
	// ������ ��ġ ����
	virtual void ToolBarPosChanged(const CRect rectToolBarPos) = 0;
	// Slider�� �ּҰ��� �����Ѵ�.
	virtual void SetSliderMin(const int nSliderMin) = 0;
	// ����ŸŬ�������� �����Ѵ�.
	virtual void SetControlData(CPacketList* pPacketList, CPacketListManager* pPacketListManager, CXScaleManager* pxScaleManager, CMainBlock* pMainBlock, CIndicatorList* pIndicatorList) = 0;
	// �׷��� Ÿ���� �޺��� �����Ѵ�.
	virtual void SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex) = 0;
	// ������ ������ �޺��� �����Ѵ�.
	virtual void SetLineThickness(int nThickness) = 0;
	// ������ǥ���θ� �����Ѵ�.
	virtual void SetOneChart(bool bOneChart, bool p_bIsNoZoomChart) = 0;
	// �׷��� ������ �����Ѵ�.
	virtual void SetGraphColor(COLORREF clrGraphColor) = 0;

	// ������ ����� �����Ѵ�. (OCX ũ�� �������ϱ� ����)
	virtual CRect GetToolBarRect() const = 0;
	// ����Ÿ�� �����ε����� ���ε����� �����´�.
	virtual bool GetStartEndIndex(int& nStartIndex, int& nEndIndex) = 0;
	// MaxRange���� �����´�.
	virtual int GetMaxRange() = 0;
	// ����Dialog�� �������ڵ��� �����Ѵ�.
	virtual HWND GetToolBarHwnd() = 0;

	// ���ٸ� �ٽñ׸���.
	virtual void OnDraw() = 0;
	// ��ũ�ѹ� ��������� �����Ѵ�.
	virtual bool IsToolScrollBar() = 0;
	// ���ٿ� �ش� ���������� �ִ��� Ȯ���Ѵ�.
	virtual BOOL GetToolOption(CToolOptionInfo::TOOLOPTION enumToolOption) = 0;
	// ���ٿ� �ش� ���������� ����ϴ��� Ȯ���Ѵ�.
	virtual BOOL UseToolOption(CToolOptionInfo::TOOLOPTION enumToolOption) = 0;
	// ����ư�� ������ ��Ų��.
	virtual void ReleaseOtherButtonUI(CToolOptionInfo::TOOLOPTION enumToolOption) = 0;

	//---------------��ġ��ȸâ����
	virtual void OnNumericalInquiryOptionChanged( const long p_lNumericalInquiryOption) = 0;	// (2006/3/4 - Seung-Won, Bae) Notify Cursor Mode Change for Drag Cross Line & Numercial Inquiry DLG

	// (2006/11/15 - Seung-Won, Bae) Tool On/Off State Changed.
	virtual BOOL	OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn) = 0;

	// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
	virtual void	UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount, BOOL p_bWholeView) = 0;
};

interface IChartCtrl;
class CToolBarManager : public IToolBarManager
{
public:
	CToolBarManager( IChartCtrl *p_pIChartCtrl);
	virtual ~CToolBarManager();

	// ���ٸ� �����Ѵ�.
	virtual bool CreateToolBar(const CString& strToolBarOption, const CString& strImageFile);
	// ������ ��ġ ����
	virtual void ToolBarPosChanged(const CRect rectToolBarPos);
	// Slider�� �ּҰ��� �����Ѵ�.
	virtual void SetSliderMin(const int nSliderMin);
	// ����ŸŬ�������� �����Ѵ�.
	virtual void SetControlData(CPacketList* pPacketList, CPacketListManager* pPacketListManager, CXScaleManager* pxScaleManager, CMainBlock* pMainBlock, CIndicatorList* pIndicatorList);
	// �׷��� Ÿ���� �޺��� �����Ѵ�.
	virtual void SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex);
	// ������ ������ �޺��� �����Ѵ�.
	virtual void SetLineThickness(int nThickness);
	// ������ǥ���θ� �����Ѵ�.
	virtual void SetOneChart(bool bOneChart, bool p_bIsNoZoomChart);
	// �׷��� ������ �����Ѵ�.
	virtual void SetGraphColor(COLORREF clrGraphColor);

	// ������ ����� �����Ѵ�. (OCX ũ�� �������ϱ� ����)
	virtual CRect GetToolBarRect() const;
	// ����Ÿ�� �����ε����� ���ε����� �����´�.
	virtual bool GetStartEndIndex(int& nStartIndex, int& nEndIndex);
	// MaxRange���� �����´�.
	virtual int GetMaxRange();
	// ����Dialog�� �������ڵ��� �����Ѵ�.
	virtual HWND GetToolBarHwnd();

	// ���ٸ� �ٽñ׸���.
	virtual void OnDraw();
	// ��ũ�ѹ� ��������� �����Ѵ�.
	virtual bool IsToolScrollBar();
	// ���ٿ� �ش� ���������� �ִ��� Ȯ���Ѵ�.
	virtual BOOL GetToolOption(CToolOptionInfo::TOOLOPTION enumToolOption);	
	// ���ٿ� �ش� ���������� ����ϴ��� Ȯ���Ѵ�.
	virtual BOOL UseToolOption(CToolOptionInfo::TOOLOPTION enumToolOption);
	// ����ư�� ������ ��Ų��.
	virtual void ReleaseOtherButtonUI(CToolOptionInfo::TOOLOPTION enumToolOption);

	//---------------��ġ��ȸâ����
	virtual void OnNumericalInquiryOptionChanged( const long p_lNumericalInquiryOption);	// (2006/3/4 - Seung-Won, Bae) Notify Cursor Mode Change for Drag Cross Line & Numercial Inquiry DLG

	// (2006/11/15 - Seung-Won, Bae) Tool On/Off State Changed.
	virtual BOOL	OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn);

	// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
	virtual void	UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount, BOOL p_bWholeView);

private:
	CToolBarManagerImplementation* m_pManagerImpl;
};

#endif // !defined(AFX_TOOLBARMANAGER_H__235DAFB3_898C_471D_8156_332FEDFFC006__INCLUDED_)
