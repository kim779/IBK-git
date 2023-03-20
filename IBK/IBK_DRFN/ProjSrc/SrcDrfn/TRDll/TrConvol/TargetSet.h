#if !defined(AFX_TARGETSET_H__89C9D833_BDDD_4F89_82C7_DABE0D79E397__INCLUDED_)
#define AFX_TARGETSET_H__89C9D833_BDDD_4F89_82C7_DABE0D79E397__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TargetSet.h : header file
//
#include <AFXTEMPL.H>

#include "resource.h"

#include "./AddControl/SetGrid.h"
#include "./AddControl/ComboCompletion.h"

#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
#include "../../../SrcSite/include/CodeCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTargetSet dialog

typedef struct
{
	CString szData;
} ST_TargetSet_Data;

typedef CList<ST_TargetSet_Data*, ST_TargetSet_Data*> List_ST_TargetSet_Data;

class CTargetSet : public CDialog
{
// Construction
public:
//	CTargetSet(CWnd* pParent = NULL);   // standard constructor
	CTargetSet(CWnd* pParent, CString strScreenNo, ST_SEARCHTARGET* pMainSrc);

	ST_SEARCHTARGET	  m_stSearchTarget; // �˻��������� ����ü
	ST_SEARCHTARGET	*m_pMainData;

	CStringArray	m_saCodeMST;
	CStringArray	m_saCodeKOSDAQ;
	CStringArray	m_saCodeKOSPI;
	
	CStringArray	m_saGroupTheme;
	CStringArray	m_saThemeItem;

private:
	List_ST_TargetSet_Data	m_ListSTData;
	void fnClearTargetSet_Data();
	LPCSTR fnGetTargetSet_DataString(DWORD _pos);

	CString			    m_strAccount;
	CString			    m_strPassWord;
//	BOOL				bTrReceive;
	CStringArray		m_saAccountInCode;
	CMapStringToPtr		m_mapAccountInCode;
public:
//	IAUTrCommManager	*m_pITrCommManager;
//	ITrComm2			*m_pITrComm;
	//IDataUtilManager2	*m_pDataUtilMng;
	
	CString				m_strFullPath;
	CString				m_strScreenNo;
//	CImageList			m_treeImageList;

	CString			m_strGroup;
	CString			m_strSubGroup;

	CSetGrid		m_gridExResult;

//-->�����޺�
	CCodeCtrl*		m_pctrlCodeInput;
	HWND			m_hCodeHwnd;
	void DoCodeRectCalc();

	void DoCodeInput(int nType);
//<--
	
public:
	//LPSTR StringDivideCodeAndName(CString strData, CString &strName);
	DWORD StringDivideCodeAndName(CString strData, CString &strName);

public:
	void SetInit();
	void CreateTreeCtrl();
	void SetInit_Tree();
	void DoCheckTreeItem();

//	void HeaderSet_ExceptGet();
	void HeaderSet_ExResult();
//	void SetAdd_ExceptList(CStringArray *saCodeMst);

	void OnChangeTreeItem(NMHDR* pNMHDR, LRESULT* pResult);
	void SetChangeGroupTree(HTREEITEM hItem);	//�׷� Ʈ�� Ŭ����
	void SetChangeSubTree(HTREEITEM hItem);		//���� Ʈ�� Ŭ����
	 
	void SetLoad_Account(CStringArray *saAccount);   //���� ������
	BOOL GetAccountToHaveCode(CString strAccount, CStringArray *saAccountInCode);   //���¹�ȣ�� �ʿ� ����� �������� ��������
	void SetAccountToCode(CString strAccount, CStringArray *psaAccountInCode); //�ʿ� ���¹�ȣ�� �������� ����
	void SetDelete_Account();
	BOOL CommSendData_Jan(CString strAcc);

	void SetLoad_KUMaster(CStringArray *saCatogoryKosdaq); //�ڽ��� �����ڵ� 
	void SetLoad_CUMaster(CStringArray *saCatogoryKospi);  //�ڽ��� �����ڵ�

	void SetLoad_K200Code(CStringArray *saCodeK200);		//KOSPI299 ���� �ڵ�
	void SetLoad_KRX100Code(CStringArray *saCodeKRX100);	//KRX100   ���� �ڵ�

	void SetLoad_PortGroup(CStringArray *saPortGroup);					   //���� �׷��, Ű��
	void SetLoad_PortGroupToCode(CString strKey, CStringArray *saPortCode);//���� �׷쳻 ���� �ڵ�

	void SetTrData_ThemeGroup();
	void SetTrData_ThemeGroupToItem(CString strCode);

	void CheckItem_SetInfo(); //�˻����� Dlg�� Check���� �׸����� 	
	void GetInfo_AllItem();

	static BOOL SetSave_ConVolTargetInfo(CString strFullPath, ST_SEARCHTARGET	*pstSEARCHTARGET);
	static BOOL GetLoad_ConVolTargetInfo(CString strFullPath, ST_SEARCHTARGET	*pstSEARCHTARGET);

	void DisplayStatic();
	
//	void SetTrData_MSTCode();

// Dialog Data
	//{{AFX_DATA(CTargetSet)
	enum { IDD = IDD_DLG_SEARCHTARGET };
//	CStatic	m_ctrlDiv;
//	CStatic	m_ctrlSubGroup;
	CStatic	m_ctrlGroup;
	CButton	m_chkInvestRiskNotice;
	CButton	m_chkELW;
	CButton	m_chkInvestAttention;
	CButton	m_chkInvestWarn;
	CButton	m_chkInvestRisk;
	CButton	m_chkDishonest;
	CButton	m_chkLogicSell;
	CButton	m_chkDisposit;
	CButton	m_chkPreferShare;
	CButton	m_chkTradeStop;
	CButton	m_chkManage;
	CStatic	m_ctrlRegCnt;
	CListCtrl	m_listExResult;
//	CListCtrl	m_listExcept;
	CTreeCtrl	m_treectrl;
//	CComboCompletion	m_cmbCode;
//	CComboBox			m_cmbCodeName;	//�ѱ��� AutoCompletion ��ɿ� �����־ ����.
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTargetSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
//	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
protected:
	void fnSyncCombo(int nType);
	CString GetCodeToName(CString strCode);
//	BOOL DoCheckCodeFirst();

	// Generated message map functions
	//{{AFX_MSG(CTargetSet)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnAddExItem();
	afx_msg void OnBtnDelExItem();
	afx_msg void OnBtnDelallExItem();
//	afx_msg void OnSelchangeCmbCode();
//	afx_msg void OnSelchangeCmbCodeName();
	afx_msg void OnOk();
	//}}AFX_MSG
	afx_msg long OnGetData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	long OnRmsgDBClick(WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TARGETSET_H__89C9D833_BDDD_4F89_82C7_DABE0D79E397__INCLUDED_)
