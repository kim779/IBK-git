#if !defined(AFX_POPUPLIST_H__5A511268_124A_4B8B_8177_575E4AFAE751__INCLUDED_)
#define AFX_POPUPLIST_H__5A511268_124A_4B8B_8177_575E4AFAE751__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PopupList.h : header file
//
#include "./control/RscDlg.h"
#include "resource.h"
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CPopupList dialog
#define CHistoryList CList<CHistory*, CHistory*> 
#define WM_POPUPLIST_SELITEM WM_USER + 77
#define MAX_ITMESIZE 10 //�ѹ��� �����ټ� �ִ� �������� 10���� ���� - 10���� ������ ��ũ�� ����

class CHistory
{
public:
	long _Type;
	CString _Code;
	CString _Name;
	CHistory(long nType, LPCTSTR szCode, LPCTSTR szName)
	{
		_Type = nType;
		_Code = szCode;
		_Name = szName;
	}
};


class CPopupList : public CRscDlg
{
// Construction
public:
	CPopupList(CHistoryList* HistoryInfo, int type = 0, CWnd* pParent = NULL);   // standard constructor

	char			m_Code[30];
	CWnd*			m_pParent;						// �θ� ������ �ڵ�

	int				m_lstType;						// 0�̸� �ڵ�, 1�̸� ����, 2�̸� ����(�ֹ�����)�� ���� ��
	CHistoryList*	m_lstItemInfo;					// ����Ʈ�� ���� ���
	void			MakeListCtrlColumn();			// ����Ʈ�� �÷��� ����	
	void			MakeInitHistoryIntoListCtrl();	// ����Ʈ�� �׸��� ����
	void			AdjustListCtrl();				// �������� ������ŭ ����� ����-��ũ�� �Ȼ��⵵��

// Dialog Data
	//{{AFX_DATA(CPopupList)
	enum { IDD = IDD_POPUPLIST };
	CListCtrl	m_lstPopup;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPopupList)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUPLIST_H__5A511268_124A_4B8B_8177_575E4AFAE751__INCLUDED_)
