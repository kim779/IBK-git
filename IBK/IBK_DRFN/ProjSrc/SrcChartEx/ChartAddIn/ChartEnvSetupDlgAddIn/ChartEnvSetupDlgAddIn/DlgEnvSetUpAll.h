#if !defined(AFX_DLGENVSETUPALL_H__F2C2DED3_AAE9_4EFE_8771_1C6823B12C4D__INCLUDED_)
#define AFX_DLGENVSETUPALL_H__F2C2DED3_AAE9_4EFE_8771_1C6823B12C4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEnvSetUpAll.h : header file
//

#include "Include_AddIn.h"					// for IChartOCX

#include "TabMainChartEnvSetUp.h"
#include "EnvironmentData.h"

#define LOAD_SETUP_ALL			0			// ��ü�� Load�Ѵ�. ��������
//#define LOAD_SETUP_IND_ONLY		1			// ��ǥ���� �κи� Load�Ѵ�.
//#define LOAD_SETUP_ENV_ONLY		2			// ȯ�漳�� �κи� Load�Ѵ�.


// ( 2006 / 10 / 31 - Sang-Woo, Cho ) - �������ǰ� Ÿ ���ǻ��� UI�� �����ϱ� ���� Define�Ѵ�.
// ���������� ��ü UI�� ��� Load�Ѵ�.
#define LOAD_OTHER_UI			1			// �Ｚ, ����, SK�� UI
#define LOAD_SEL_ITEM			3			// ȯ�漳�� �Ǻ� ���� ���� - ojtaso (20080109)


class CChartEnvSetupDlgAddInImp;

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 7
// Comments		: 
// Using way	: 
// See			: class CDialog
//-----------------------------------------------------------------------------
class CDlgEnvSetUpAll : public CDialog
{
// Construction
public:
	CDlgEnvSetUpAll( IChartOCX *p_pIChartOCX, const int nType, const int nActiveTabIdx, CChartAddInBase *pParentAddIn, CWnd* pParent, BOOL p_bWin98);
	~CDlgEnvSetUpAll();


// Dialog Data
	//{{AFX_DATA(CDlgEnvSetUpAll)
	enum { IDD = IDD_ENV_SETUP_ALL };
	CTabMainChartEnvSetUp	m_mtabChartEnvSetUpAll;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEnvSetUpAll)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void OK( void )			{ OnOK(); };
	void Cancel( void )		{ OnCancel(); };


	// ( 2006 / 12 / 11 - Sang-Woo, Cho ) - IndicatorInfoUndoRedoManagerAddIn�� ��ǥ�� ������ BackUp�Ѵ�.
	void BackUpIndicatorInfo( IStringList *pStringList = NULL );

protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgEnvSetUpAll)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnPreview();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	IChartOCX *m_pIChartOCX;
	int m_nType;
	int m_nActiveTabIdx;

	// ( 2006 / 12 / 11 - Sang-Woo, Cho ) 
	CChartAddInBase *m_pParentAddIn;

// (2007/1/19 - Seung-Won, Bae) Check OS Version. (for excepting Win98)
protected:
	BOOL	m_bWin98;

// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
public:
	void	OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName)
				{	m_mtabChartEnvSetUpAll.OnGraphDeleted( p_szGraphName, p_szIndicatorName);	}

	int		GetGridTypeCombo(int nType);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGENVSETUPALL_H__F2C2DED3_AAE9_4EFE_8771_1C6823B12C4D__INCLUDED_)
