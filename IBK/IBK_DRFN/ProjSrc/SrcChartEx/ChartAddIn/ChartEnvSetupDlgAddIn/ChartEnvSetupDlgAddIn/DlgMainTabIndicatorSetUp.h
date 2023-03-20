#if !defined(AFX_DLGMAINTABINDICATORSETUP_H__46B898E1_41B8_4B7C_A468_421DF2A2ACFF__INCLUDED_)
#define AFX_DLGMAINTABINDICATORSETUP_H__46B898E1_41B8_4B7C_A468_421DF2A2ACFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMainTabIndicatorSetUp.h : header file
//

#include <AFXTEMPL.H>
#include "BaseTabDlg.h"
#include "OrgViewData.h"
#include "GroupButtonCtrl.h"
#include "BaseDialogContainer.h"
#include "MakeChildDlg.h"
#include "TabSubIndicatorSetUpCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMainTabIndicatorSetUp dialog

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 9
// Comments		: 
// Using way	: 
// See			: class CDialog
//-----------------------------------------------------------------------------
class COutsideCommonInterfaceFunction;
class CDlgMainTabIndicatorSetUp : public CBaseTabDlg, public CTabParentDlg
{
	DECLARE_DYNAMIC( CDlgMainTabIndicatorSetUp)
// Construction
public:
	CDlgMainTabIndicatorSetUp( IChartOCX *p_pIChartOCX, CWnd *pParent = NULL );   // standard constructor
	~CDlgMainTabIndicatorSetUp();   // standard destructor

// Dialog Data
	//{{AFX_DATA(CDlgMainTabIndicatorSetUp)
	enum { IDD = IDD_MAINTAB_PAGE_INDICATOR };
	CTabSubIndicatorSetUpCtrl	m_tabIndicatorSetUpAll;
	CString	m_editIndicatorExp;
	BOOL	m_bSaveDefault;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMainTabIndicatorSetUp)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SelChangeGroupButton(const CString &strIndicatorName, const CString &strGraphName);					// �׷��ư ����
	void SelChangeGraph( const CString &strIndicatorName, const CString &strGraphName);		// ��ǥ����â ����
	void AddGraph( const CString &strIndicatorName, CString &p_strGraphName);								// ��ǥ �߰�
	BOOL RemoveGraph( const char *p_szGraphName);
	int GetInitChartIndex() const;

//	void InitializeAllIndicator( void );
	void InitializeSelectedIndicator( void );

	void Recover( void )	{ OnBtnRecover(); };
	void Save( void )		{ OnBtnSave(); };
	void PreView( void )	{ OnBtnPreview(); };

	void OK( void );
	void Cancel( void );

	// (2006/8/15 - Seung-Won, Bae) Support MChart Interface
	// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface
	void GetMChartInterface( COutsideCommonInterfaceFunction &p_outsideIF);

	// ( 2006 / 11 / 06 - Sang-Woo, Cho ) - private���� public���� �ű�.
	void ShowChildDialog(BOOL bShow);					// ChildDlg �����ֱ� ����.
	
	// ( 2006 / 11 / 15 - Sang-Woo, Cho ) 
	CString GetDataFolderPath( void );

public:
	virtual bool ChangeTabPage( CBaseTabDlg *pSourceDlgTabPage );
	virtual CBaseTabDlg::DLG_TYPE GetDialogType() const;
	
private:
	IGraphInfo* GetIndicatorInfoInBaseDlg(const CString& strIndicatorName);
	bool GetDrawingCurPrice(CBaseTabDlg *pSourceDlgTabPage, int& nDrawingCurPrice) const;

private:
	void SetOrgViewData();								// OrgViewData�� �����Ѵ�. (��ҽÿ� �ʿ��� Ŭ�����̴�!)
	void SetIndicatorSize();							// IndicatorCtrl�� ������ �����ϱ�.
	void SelChangeChildDlg( const CString &strIndicatorName, const CString &strGraphName);	// �ش���ǥ�� ChildDlgâ�� ����.

	void SetChildDlgPosition();							// ChildDlg�� ��ġ�� �����Ѵ�.
	void SyncItemCheck();								// ListCtrl�� ������üũ���¸� Chart�� ��ġ��Ų��.
	void ApplyGraph(const CString &strIndicatorName);	// ChildDlg���� �����Ȱ��� (��ǥ)Graph�� �����Ų��.
	void RestoreChartCfg(const int nInitChartIndex);

protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgMainTabIndicatorSetUp)
	virtual BOOL OnInitDialog();
	afx_msg void OnInitilizeAllindicator();
	afx_msg void OnDefaultIndicatordata();
	afx_msg void OnChkWithDefault();
	afx_msg void OnBtnSave();
	afx_msg void OnBtnRecover();
	afx_msg void OnBtnPreview();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnAddAll();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnDeleteAll();
	afx_msg void OnBtnWithDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CFont m_pftGroupNameFont;
	CFont *m_pftOldStaticFont;
	CMapStringToPtr m_mapIndicatorExp;
	BOOL m_bPreview;
	BOOL m_bSave;
	BOOL m_bDlgMode;									// TRUE : ��ǥ ����, FALSE : ��ǥ ����

private:
	bool GetGraphNameList(const bool bIsOnlyViewData, IStringList * indicatorNameList);
	bool GetGraphNameList(IStringList *sourceIndiNameList, IStringList * targetIndiNameList);
	bool IsExistenceIndicatorName(IStringList *indicatorNameList, const char *p_szGraphName) const;

	bool IsActionInitilizeAllindicator();
	bool Initilizeindicator(CBaseDialog* pBaseDlg);

private:
	const bool m_bIsOnlyViewData;
	IChartOCX *			m_pIChartOCX;
	IChartManager *		m_pIChartManager;
	IGraphInfoManager *	m_pIGraphInfoManager;
	IPacketManager *	m_pIPacketManager;
	
	IStringList *	m_pIndicatorNameList;				// ���� ������� ��ǥ����� ����Ʈ.
	IStringList *	m_pOrgIndicatorNameList;			// ������ ��ǥ���� List
	
	int	m_nInitChartIndex;								// ��ǥ����â ���� �ʱ��� ��ǥ���� �ε���. ('���'�ÿ� �̰��� ���)
	CString m_strIndicatorName;							// ��ǥ����â���� ���� ��Ŀ�̵Ǿ��ִ� ��ǥ��.

	CBaseDialog *m_pChildDlg;	// ��ǥ�� ����DialogŬ���� (��ǥ����â�� �ΰ��� ������ ��쵵 �����Ƿ� CList�� ���µ��ϴ�.)

	CMakeChildDlg m_MakeChildDlg;						// ChildDlg ���� ���丮Ŭ����
	CBaseDialogContainer m_BaseDialogContainer;			// ��ǥ����dialog ����Ŭ����.

	COrgViewData m_OrgViewData;							// IGraphInfo ���ʼ����� ����Ŭ����.

// ( 2007 / 01 / 09 - Sang-Woo, Cho ) - ��ġ�̵�
public:
	CGroupButtonCtrl m_ctrlGroupButton;					// GroupButton�� ListCtrl ����Ŭ����.
	
	// ( 2007 / 01 / 08 - Sang-Woo, Cho ) - �����ǥ GroupButton
	CGroupButtonCtrl m_ctrlUseGroupButton;				// �����ǥ GroupButton

// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
public:
	void	OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName);

// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
protected:
	DWORD	m_dwLastClickedTime;

// 2008.01.25 by LYH >>
protected:
	HWND	m_hOcxWnd;
// 2008.01.25 by LYH <<
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMAINTABINDICATORSETUP_H__46B898E1_41B8_4B7C_A468_421DF2A2ACFF__INCLUDED_)
