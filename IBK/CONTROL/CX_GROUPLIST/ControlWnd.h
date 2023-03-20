#pragma once
// ControlWnd.h : header file
//

#include "../../h/axisfire.h"

#include "GroupListCtrl.h"

typedef struct _item_
{
	CString brAccn;		// ����������ȣ
	CString accn;		// ���¹�ȣ
	CString accnName;	// ���¸�
	CString code;		// �����ȣ
	CString codeName;	// �����
	CString gubn;		// �Ÿű��� 
	CString jucode;		// �ֹ������ڵ�
	CString juName;		// �ֹ�������
	CString juNumber;	// �ֹ���
	CString chNumber;	// ü�ᷮ	
	CString noChNumber;	// ��ü�ᷮ
	CString chPrice;	// ü����հ�
	CString chAmount;	// ü��ݾ�
} sItem;

class CParam
{
public:
	int		key = 0;		// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point = 0;		// font point
	int		style = 0;		// font style
	DWORD		tRGB = 0;		// text color index
	DWORD		pRGB = 0;		// paint color index
	CString		options;		// properties
};
/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd(struct _param* pParam, CWnd* pWizard);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL
	
protected:
	std::unique_ptr<CGroupListCtrl> m_list;
	CWnd*	m_pWizard;
	CParam	m_param;

	void	GetString(CString str, sItem* p);
	void	AddList(sItem* p);
	void	SetList(sItem* p, int nItem);	
	void	UpdateGroup(sGLI* pg);

	CString MakeMoneyString(CString str);
	CString MakeMoneyString(__int64 dw);
	CString MakeAccnString(CString str);


// Implementation
public:
	virtual ~CControlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnItemClick(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg void SetProperties(LPCTSTR str);
	afx_msg BSTR GetProperties();
	afx_msg void AddRow(LPCTSTR str);
	afx_msg void AddAlert(LPCTSTR str);
	afx_msg void MakeList();
	afx_msg void ReadyList();
	afx_msg void Visible(BOOL bShow);	
	afx_msg void RemoveAlert(LPCTSTR str);
	afx_msg void ExpandTree(BOOL bExpand);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};