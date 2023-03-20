#if !defined(AFX_DLGGRAPHSETTING_H__C784ACD7_9191_46F3_939B_1BA9EFCB9CE0__INCLUDED_)
#define AFX_DLGGRAPHSETTING_H__C784ACD7_9191_46F3_939B_1BA9EFCB9CE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGraphSetting.h : header file
//
#include <afxtempl.h>
#include "CheckComboBox.h"
#include "DlgToolTip.h"
#include "NoSystemMenuEdit.h"
#include "NoSystemMenuCombo.h"
#include "DialogExpander.h"
#include "GraphColorCollection.h"	// Added by ClassView
#include "LineColor.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSetting dialog
class CChartLink;
class CLineColor;
class CDlgGraphSetting : public CDialog
{
// Construction
public:
	CDlgGraphSetting(CChartLink *pItem, CLineColor* LineColor, IGraphInfoManager *p_pIGraphInfoManager, HWND p_hOcxWnd, CWnd* pParent = NULL);   // standard constructor
	~CDlgGraphSetting();

public:
	CString GetIndicatorName();
	//set received data from parent.
	void SetAllItemInfoOfPacketPage(const CString& strAllItemInfoOfPacketPage);
	//�׷��� Ÿ���� Ư�����϶� disable�Ǿ� �ִ� �Լ��� GraphStyle���� ������ �׸��� �޺��� display
	void SetIndicatorNameToEditFromDlgGraphColor(const CString& strGraphStyle);
	//Tab���� �ϴ� ��ư Ŭ��������..
	void ButtonClicked(const int& nIndex);
	//Tree Class���� �� Dlgd���� ������ ��簪�� �ϳ��� String���� ����� �������� �Ҷ�.
	CString GetSubGraphDatas( const char *p_szIndicatorName);

// Dialog Data
	//{{AFX_DATA(CDlgGraphSetting)
	enum { IDD = IDD_DIALOG_GRAPHSETTING };
	CButton	m_Signal_Bt;
	CCheckComboBox	m_pPacketComboBox;
	CNoSystemMenuCombo	m_IndicatorFuctionComboBox;
	CString	m_strIndicatorName;
	CString	m_strCheckedPacketItems;
	BOOL	m_bSignalShow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGraphSetting)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CExpandDialog m_ExpandDialog;
	// Generated message map functions
	//{{AFX_MSG(CDlgGraphSetting)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnEditchangeIndicatorfunctionname();
	afx_msg void OnSelchangeIndicatorfunctionname();
	afx_msg void OnSelchangeComboPacketlist();
	afx_msg void OnEtcsetBt();
	afx_msg void OnBtnNextSubgraph();
	afx_msg void OnBtnPrevSubgraph();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CheckDataViewItems();
	bool DoThisIndicatorNeedSignal(const CString& strIndicatorName);
	CLineColor* m_pLineColor;
	void UnCheckAllCheckedPacketItems();
	//tree�� ���õǾ� �ִ� ��ǥ�̸� ������ ����.
	CString m_strIndicatorType;
	//��� color�� ��� �ִ� String
	CString m_strSubGraphDatas;
	//Packet���������� ���� string
	CString m_strAllItemInfoOfPacketPage;   
	//����Ǿ� �ִ� �Լ���� ������ �Լ����� �ٸ���� DlgGraphColor�� ������ ���� Setting ���� ����.
	CString m_strOldIndicatorName;
	//�Ӽ�Dll�� ������.
	//CheckComboBox�� üũ�� �׸��� �ִ��� Ȯ��
	bool m_bCheckComboState;
	CChartLink* m_pItem;
	//����� Data���� ���õ� �׸��� �ִ��� ����.
	bool m_bIsSelectedPacketData;
	//Graph�� �� �����ϱ� ���� Dialog �����ϴ� Ŭ����.
	CGraphColorCollection* m_GraphColorCollection;
	//����.
	CDlgToolTip		m_toolTip;
	//��������
	void CreateToolTip();

	//PacketPage���� ���õ� Packet�� ����� Data �޺��� �ֱ�.
	void AddStringToPacketCombo(); 
	
	//IndicatorFuntion �޺��� list ä��.
	void AddStringToIndicatorFunctionCombo();
	void AddStringToIndicatorFunctionComboWhenNoSpecialGraphType();
	void AddStringToIndicatorFunctionComboWhenSpecialGraphType();

	//Enable or disable Scale Position Radio Button
	void SetDisableWindow(UINT nRadioButton, const BOOL bCheck);
	void SetRadioDisableWindow(const BOOL bCheck);

	//Color�� title �����ϱ� ���� Control ��ǥ���� �ٸ��� ����.
	void SetSubGraphDlgOnGraphSettingDlg();

	//����� Data���� ������ �����ߴ� �׸� üũ ���¸� ����� �ֱ� ����
	void CheckPacketbyCheckedPacketItems();

	//�Ϲ���ǥ : ����, ����, ����
	bool IsGeneralIndicatorType() const;
	//none ��ǥ : Ư����, ������ǥ����, �����ɼ���
	bool IsNoneIndicatorType() const;

	void SetSubGraphDatas_Indicater();

//sy 2006.04.17
private:
	//��ǥ�� ���濡 ���� ȭ�鰪 ����
	void CheckGraphSettingbyIndicatorName(const CString& strIndicatorName);
	//�Լ��� ���� Packet �ڵ����� üũ�ϱ�
	void CheckDefaultPacketbyIndicatorInfo(	IGraphInfo *p_pIGraphInfo);
	void CheckGraphSetting2byIndicatorInfo(	IGraphInfo *p_pIGraphInfo, const char *p_szIndicatorName);
	void CheckSignalbyIndicatorInfo(		IGraphInfo *p_pIGraphInfo);
	void CheckSignal(const bool bIsShow);

	void SetSignal(CChartLink* pItem);

	BOOL m_bSignalUse;

private:	
	CGraphSetting2Data m_graphSetting2Data;

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
protected:
	IGraphInfoManager *	m_pIGraphInfoManager;

// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
protected:
	HWND	m_hOcxWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAPHSETTING_H__C784ACD7_9191_46F3_939B_1BA9EFCB9CE0__INCLUDED_)
