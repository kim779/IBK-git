#if !defined(AFX_DLGSETTINGTOOLBAR_H__2588FCC5_D941_4353_97F5_B3D35F5BBC45__INCLUDED_)
#define AFX_DLGSETTINGTOOLBAR_H__2588FCC5_D941_4353_97F5_B3D35F5BBC45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSettingToolBar.h : header file
//
#include "../../inc/IGateMng.h"
#include "../../inc/IMainInfoMng.h"

//#define		VERSION				20061013			
/////////////////////////////////////////////////////////////////////////////
// CDlgSettingToolBar dialog

class CDlgSettingToolBar : public CDialog
{
// Construction
	BOOL	m_bApply;	//�����ư
	
	CArray<int, int>	m_arrayAnalIdx;			//�м���������Ʈ �ε���
	CArray<int, int>	m_arrayAdditionIdx;		//�ΰ���ɸ���Ʈ �ε���
	CArray<int, int>	m_arrayUserIdx;			//����ڼ�������Ʈ �ε���
	CMap<int, int, CString, CString>	m_mapAnalList;			//�м���������Ʈ
	CMap<int, int, CString, CString>	m_mapAdditionList;		//�ΰ���ɸ���Ʈ
	CMap<int, int, CString, CString>	m_mapUserList;			//����ڼ�������Ʈ

	void GetFieldData(CString strFieldData, int &nFieldKey, CString &strFieldName);

	IGateManager *		m_pGMng;
	// File
	void	ReadIniFile(int nCulSel=3);			// ������ �о� �ʱ�ȭ��

	void FillShowList(CArray<int, int> &arrayList, CMap<int, int, CString, CString> &mapList); 
	void FillAvailableList(CArray<int, int> &arrayList, CMap<int, int, CString, CString> &mapList);
	int m_nDataVer;

public:
	CDlgSettingToolBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSettingToolBar)
	enum { IDD = IDD_DLGSETTINGTOOLBAR };
	CListBox	m_listShow;
	CListBox	m_listAvailable;
	CComboBox	m_cmbShow;
	CComboBox	m_cmbAvailable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSettingToolBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgSettingToolBar)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnApply();
	afx_msg void OnSelchangeCmbAvailable();
	afx_msg void OnSelchangeCmbShow();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnInit();
	afx_msg void OnBtnMovedown();
	afx_msg void OnBtnMovsyp();
	afx_msg void OnBtnRemove();
	afx_msg void OnBtnRemoveall();
	afx_msg void OnBtnAddall();
	afx_msg void OnDblclkListAdd();
	afx_msg void OnDblclkListRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETTINGTOOLBAR_H__2588FCC5_D941_4353_97F5_B3D35F5BBC45__INCLUDED_)
