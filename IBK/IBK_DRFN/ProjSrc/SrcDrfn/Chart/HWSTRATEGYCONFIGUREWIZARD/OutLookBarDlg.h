#pragma once

#include "../Common_ST/STGfxOutBarCtrl.h"
#include "../Common_ST/_STLoadmanager.h"
#include "resource.h"

#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExDrawButton.h"
#include "../../inc/ExStatic.h"
#include "../../inc/ExEdit.h"
#include "../../inc/ExComboBox.h"

// COutLookBarDlg ��ȭ �����Դϴ�.

class COutLookBarDlg : public CDialog
{
	DECLARE_DYNAMIC(COutLookBarDlg)

public:
	COutLookBarDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~COutLookBarDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_OUTLOOKBARDLG };
	CExDrawButton	m_btnFindST;
	CExDrawButton	m_btnNextST;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

public:
	CSTGfxOutBarCtrl			m_wndOutBar;
//	CImageList				m_ilSTIcon;
//	CImageList				m_ilW151H21;
	CImageList		m_ImageListIcon;
	CImageList		m_ImageOutbarIcon;
	CSTLoadManager*			m_pSTLoadMnger;

	COLORREF		m_crBk;
	CBrush			m_brBkColor;
	
	CStringArray	m_arSTPath;
	CString			m_szSTData_STPath;
	CString			m_szSTUser_STPath;
	CString			m_szCurFolder;
	CString			m_szCurSTType;
	CStringArray	m_arFindIndx;
	int				m_nFindIndx;
	STMNG_ITEMINFO	m_RunSTInfo;			// �������� ��������

public:
	void	SetSTParent(CWnd* pParent);
	void	LoadSTList();
	void	UpdateSTList(int nState, CString szSTPath);
	CString GetSTPathSelected(CString& szSTType);
	void	Resize();
	CRect	GetRect(int nID);
	BOOL	IsUserFolder(int nFolder);


	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedBtnFindst();
	afx_msg void OnBnClickedBtnNextst();
};
