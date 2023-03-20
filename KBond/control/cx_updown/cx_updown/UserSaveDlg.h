#pragma once


// CUserSaveDlg dialog


#include "Updlg.h"
#include "Downdlg.h"


class CUserSaveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserSaveDlg)

public:
	CUserSaveDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserSaveDlg();

	void Upload();
	void Download();
	void	process_INTERDOWN(char* recB);
	void	process_INTERUP(char* recB);

	void	SetDlgPos(int x, int y);

	CUpdlg*		 m_pUpDlg;
	CDowndlg*    m_pDownDlg;
	CTabCtrl*    m_pTab;

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
// Dialog Data
	enum { IDD = IDD_DLG_UPDOWN };

	void	process_OUB(char* recB);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg	LONG OnUSER(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnDownfile(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnUpfile(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnUpdown();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CTabCtrl m_tab;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabUpdown(NMHDR *pNMHDR, LRESULT *pResult);
};
