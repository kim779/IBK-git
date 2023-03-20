// CtrlFactory.h: interface for the CCtrlFactory class.
// ToolBar�� �ø� ��Ʈ�ѵ��� ������Ű�� ���丮 Ŭ����.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTRLFACTORY_H__9C549468_DCA5_40D7_81BE_70EFD148CE14__INCLUDED_)
#define AFX_CTRLFACTORY_H__9C549468_DCA5_40D7_81BE_70EFD148CE14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"
#include "ToolOption.h"

// (2006/2/8 - Seung-Won, Bae) Dynamic UI Control IDs
#define IDC_SCROLL                      17000
#define IDC_SLIDER                      17001
#define IDC_GRAPHSTYLE_COMBO            17002
#define IDC_LINETHICKNESS_COMBO         17003
#define IDC_RADAR                       17004

interface IChartCtrl;
class CScrollBarEx;
class CSliderCtrlEx;
class CButtonEx;
class CGraphStyleCombo;
class CGraphLineThicknessCombo;
class CRadarControl;
class CObjTool;

class CCtrlFactory  
{
public:
	CCtrlFactory( IChartCtrl *p_pIChartCtrl, CWnd* pParent, const CRect rectToolBar, const bool bStatusBar, const bool bRadarChart, const int nScrollWidth, const bool bBitmap);
	virtual ~CCtrlFactory();	// ����! ������ ��Ʈ�� �Ҹ��� ToolBarDlg���� �ؾ���.
	void Initialize();	// �ʱ���� ����.

	CScrollBarEx* CreateScrollBar();	// ��ũ�ѹ� ����
	CSliderCtrlEx* CreateSliderCtrl();	// �����̴��� ����.
	CGraphStyleCombo* CreateGraphStyleCombo(const CString& strName);	// �׷������� �޺��ڽ� ����
	CGraphLineThicknessCombo* CreatreGraphLineThicknessCombo(const CString& strName);	// ������ ���� �޺� �ڽ� ����
	CRadarControl* CreateRadarControl();	// ���̴���Ʈ ����
	CButtonEx* CreateButton(CObjTool* pObjTool, const CString& strParam = _T(""));	// ��ư���� ����.
	
private:
	CRect m_rectCtrl;		// ��Ʈ�� ������ ����� ���.
	bool m_bStatusBar;		// ��ġ��ȸ����Ÿ����(StatusBar) ��뿩��
	bool m_bRadarChart;		// ���̴���Ʈ(��ħ��) ��뿩��.
	bool m_bBitmap;			// ��Ʈ�� �ε��� �����ߴ��� ����. (��ư�� OWNER_DRAW�� �׸������� �Ǵ��ϱ� ����)
	CWnd* m_pParent;		// �θ�������������.
	UINT	m_nCtrlID;		// ��Ʈ�� ���̵�.
	UINT	m_nToolBarWidth;// ������ Width.(Status Bar, Radar Chart ��)
	int		m_nScrollWidth;	// Scroll Bar�� Width.
	HWND	m_hOcxWnd;

protected:
	IChartCtrl *	m_pIChartCtrl;
};

#endif // !defined(AFX_CTRLFACTORY_H__9C549468_DCA5_40D7_81BE_70EFD148CE14__INCLUDED_)
