#pragma once

#include "Updlg.h"
#include "Downdlg.h"

// CUserSaveDlg ��ȭ �����Դϴ�.

class CUserSaveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserSaveDlg)

public:
	CUserSaveDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUserSaveDlg();

	void	Upload();
	void	Download();
	void	process_INTERDOWN(char* recB);
	void	process_INTERUP(char* recB);

	void	SetDlgPos(int x, int y);
	void	GetledgerH(struct _ledgerH* pLedgerH);

	CUpdlg*		 m_pUpDlg;
	CDowndlg*    m_pDownDlg;
	CTabCtrl*    m_pTab;
	class CControlwnd*  m_pParent;

	CWnd*	 m_pWizard;
	CWnd*	 m_parent;
	CString	 m_home;
	CString	 m_user;
	CString	 m_tr;
	int		 m_index;
	int		 m_cursor;
	bool		 m_upload;
	bool		 m_bSplit;

	CStringArray	m_selectlist;
	CStringArray	m_filelist;
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_UPDOWN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	void sendTR(CString trC, char* pBytes, int nBytes);
	CTabCtrl m_tab;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabUpdown(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedBtnUpdown();
	afx_msg void OnBnClickedCancel();
};
