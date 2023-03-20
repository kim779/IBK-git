// ExListCtrl.h: interface for the CExListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXLISTCTRL_H__8BCCC1F8_8DA9_46E1_9E6E_42E9D4C60074__INCLUDED_)
#define AFX_EXLISTCTRL_H__8BCCC1F8_8DA9_46E1_9E6E_42E9D4C60074__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoaderListCtrl;
class CExListCtrl : public CListCtrl  
{
// Construction
public:
	CExListCtrl(BOOL bUseDefSkin = FALSE);

// Attributes
protected:
	CLoaderListCtrl*		m_pLoader;

// Operations
public:
 	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: �޸� �����ͳ� long�� ��
	// Paramter
	//		LPVOID lpAction : �޸� ������ (����ü, Ŭ�������..)
	// Comments		: ���ǵ� �Լ��̿ܿ� �۾��� �ؾ� �ϴ� ��� ActionControl �Լ���
	//				  ����� Ŭ���� �������� ������Ʈ ���� ����.
	//				  �Ķ���Ϳ� ���ϰ��� �� ȭ�鿡���� �䱸���׵��� ���� ������
	//				  ActionControl�� ����� �߰��� ������ ���� Ŭ���� �������� �Լ��� �߰�������
	//				  ��Ȱ�� ������Ʈ ������ ���� �ӽ÷� �� �Լ��� ����մϴ�.
	//-----------------------------------------------------------------------------	
	long	ActionControl(LPVOID lpAction);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: void
	// Paramter
	//		BOOL bDisable : �÷� ������¡ ���� ����
	// Comments		: ����Ʈ�� �÷� �ʺ� ������ ��Ȱ��ȭ �� ������ ����
	//-----------------------------------------------------------------------------
	void	DisableColumnSizing(BOOL bDisable);

//	void	SetHeaderHeight(int nHeight);
	void	SetRowHeight(int nHeight);
	void	SetAutoColSize(BOOL bAutoSize);
	void	SetTwotoneRowColor(int nRow = 1);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExListCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExListCtrl();

	// Generated message map functions
protected:

	//{{AFX_MSG(CExListCtrl)
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_EXLISTCTRL_H__8BCCC1F8_8DA9_46E1_9E6E_42E9D4C60074__INCLUDED_)
