#pragma once
// RetailBondDlg.h : header file
//

#include <afxtempl.h>
#include "skinlistctrl.h"

typedef struct	SJcode	{
	CString	code;
	char	cgub{};			// ä�Ǳ���
					// 1: ��ä
					// 2: ���ä
					// 3: ����ä
					// 4: �����Ư��ä
					// 5: �ֻ�ä
					// 6: ��Ÿ
	char    ngub{};			// ���� 0: ��ü, 1:�ֱ�����
	CString	hnam;			// ä���̸�
} SJCODE;


static	BOOL	m_gsortReTail;
/////////////////////////////////////////////////////////////////////////////
// CRetailBondDlg dialog

class CRetailBondDlg : public CDialog
{
// Construction
public:
	CRetailBondDlg(CPoint pt, CWnd* pParent = nullptr);   // standard constructor

	void	Set_KS_HanMode();
	void	SetDlgPos(int x, int y);
	void	DeleteAllData();
	BOOL	FindCode(CString szCode);

	void	AddRow(int sort);
	BOOL	LoadSJCode();
	CString	GetCode(){ return m_szCode; }
	CString	GetName(){ return m_szName; }
	void	SetName(CString szName){ m_szName = szName; }

// Dialog Data
	//{{AFX_DATA(CRetailBondDlg)
	enum { IDD = IDD_DIALOG_RETAILBOND };
	CSkinListCtrl	m_ListCtrl;
	int		m_nRadio;
	CString		m_szEditSearch;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRetailBondDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	void listsort(const int column);
	
	CFont	m_Font;
	CPoint	m_point;
	std::vector<std::tuple<CString, CString, char, char>> m_RetailBondArray;
	std::vector<std::tuple<CString, CString, char, char>> _vData;
	
	CString	m_szCode;
	CString	m_szName;
	CString _szData;

	// Generated message map functions
	//{{AFX_MSG(CRetailBondDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkListRetailbond(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListRetailbond(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditSearch();
	afx_msg void OnChklast();
	afx_msg void OnRadio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGetdispinfoListRetailbond(NMHDR* pNMHDR, LRESULT* pResult);
};

