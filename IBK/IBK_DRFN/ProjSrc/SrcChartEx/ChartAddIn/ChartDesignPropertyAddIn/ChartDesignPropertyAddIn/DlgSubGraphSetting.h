#if !defined(AFX_DLGSUBGRAPHSETTING_H__DA53AC9C_848B_4CA3_97EE_B922167258A9__INCLUDED_)
#define AFX_DLGSUBGRAPHSETTING_H__DA53AC9C_848B_4CA3_97EE_B922167258A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSubGraphSetting.h : header file
//

#include "Include_AddIn.h"			// for Ixxxxxx.h
#include "colourpicker.h"
#include "DlgToolTip.h"
#include "NoSystemMenuCombo.h"
#include "NoSystemMenuEdit.h"		// for CNoSystemMenuEdit

/////////////////////////////////////////////////////////////////////////////
// CDlgSubGraphSetting dialog

class CDlgGraphSetting;
class CDlgSubGraphSetting : public CDialog
{
// Construction
public:
	CDlgSubGraphSetting( HWND p_hOcxWnd, CWnd* pDlgChartWnd, CWnd* pParent = NULL);	// standard constructor
	~CDlgSubGraphSetting();											// standard destructor

public:
	void SetDrawStyle(const int nDrawStyle);
	void SetGraphType(const int graphtype);
	//��Dlg �����Ҷ� �Ӽ� Dll���� Style�� Set.
	void SetGraphStyle(const int nStyle);
	//��ǥ���� ���� ���õ� ��ǥ�� Ÿ���� �����϶� InitDialog���� ������ 6���� ������ Set.
	void SetLineColor(const CString& strLineColor);
	//ocx�� String�� �Ѿ�� ������ �ʴ� SubGraphName�� �ѱ�� ���ؼ� �Ӽ� Dll���� ���� ���� ����
	void SetSubGraphName(const CString& strSubGraphName);
	//dlg���� Control�� Setting
	void SetSubGraphDatas(const CString& strSubGraphDatas);
	//������ ������ �ϳ��� String ���� �����.
	CString GetAllSettingData( const char *p_szIndicatorName);

	//Color�� ���� String.
	CString m_strclrUpEdge;
	CString m_strclrUp;
	CString m_strclrEqual;
	CString m_strclrEqualEdge;
	CString m_strclrDownEdge;
	CString m_strclrDown;	
// Dialog Data
	//{{AFX_DATA(CDlgSubGraphSetting)
	enum { IDD = IDD_DIALOG_SUBGRAPHSETTING };
	CColourPicker	m_clrEqualEdge;
	CNoSystemMenuEdit	m_TitleEdit;
	CNoSystemMenuEdit	m_UpThicknessEdit;
	CNoSystemMenuEdit	m_DownThicknessEdit;
	CNoSystemMenuEdit	m_EqualThicknessEdit;
	CColourPicker	m_clrUp;
	CColourPicker	m_clrUpEdge;
	CColourPicker	m_clrDown;
	CColourPicker	m_clrDownEdge;
	CColourPicker	m_clrEqual;
	CNoSystemMenuCombo	m_TypeCombo;
	CNoSystemMenuCombo	m_DrawStyleCombo;
	CComboBox	m_GraphStyleCombo;
	BOOL	m_bFillUp;
	int		m_nUpThickNess;
	BOOL	m_bFillDown;
	int		m_nDownThickNess;
	int		m_nBorderWidth;
	CString	m_strTitle;
	BOOL	m_bGraphHide;
	int		m_nGraphType;
	int		m_nGraphStyle;
	int		m_nDrawStyle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSubGraphSetting)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSubGraphSetting)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeGraphType();
	afx_msg void OnSelchangeGraphstyle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int GetDrawStyle(CString& strStringValue);
	int GetDrawStyle();
	int GetGraphStyle(CString &strControlValue);
	int GetGraphType(CString &strControlValue);
	//�θ� ������ ������
	CDlgGraphSetting* m_pDlgGraphSetting;
	//���Ҽ� ���� ��ǥ�̸�.
	CString m_strSubGraphName;
	//Graph�� Style. ������, ��ܽ�, �Ϲݽ�..��..
	CString m_strGraphStyle;
	//����.
	CDlgToolTip		m_toolTip;

	//Dlg�� ��� ��ư�� Color�ʱ�ȭ
	void InitButtonColor();
	//RGB���� �����ϱ� ���ؼ� Picker�� String���� ��ȯ
	void MakeFromPickerColorToStrColor();
	//����
	void CreateToolTip();

	//GraphStyle�޺��� �Ӽ� Dll���� ���� ������ Add.
	void Set_GraphTypeCombo();
	void Set_GraphStyleCombo();
	void Set_GraphDrawStyleCombo();

	//GrapyType/Style �˻�.
	void CheckingGraphTyleAndStyleAndCombo();
	void CheckingGraphTyleAndStyle();

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
protected:
	IGraphInfoManager *	m_pIGraphInfoManager;
public:
	void SetIGraphInfoManager( IGraphInfoManager *p_pIGraphInfoManager);

	//graphDrawStyle ���� �޺��� enable ����
	bool IsEnable_GraphDrawStyleCombo(const CGraphBaseData::GRAPHTYPE eGraphType) const;
	//graphDrawStyle �� ���� Ÿ��
	bool IsNoGraphDrawStyle(const CGraphBaseData::GRAPHTYPE eGraphType) const;

// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
protected:
	HWND	m_hOcxWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUBGRAPHSETTING_H__DA53AC9C_848B_4CA3_97EE_B922167258A9__INCLUDED_)
