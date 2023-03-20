#if !defined(AFX_CODECTRL_H__8C6C1F37_A51A_4D7F_B461_FC7C57E521EF__INCLUDED_)
#define AFX_CODECTRL_H__8C6C1F37_A51A_4D7F_B461_FC7C57E521EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CodeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCodeCtrl window

class CCodeCtrl : public CWnd
{
// Construction
public:
	CCodeCtrl();

// Attributes
public:

private:
	UINT	m_nID;

	CWnd*	m_pwndCodeCtrl;
	CWnd*	m_pwndParent;
	CWnd*	m_pwndWizard;


	HINSTANCE	m_hiCodeCtrl;

	

public:
	BOOL	CreateCodeCtrl(CWnd* pwndParent, CWnd* pwndWizard, CRect rect, UINT nID);
	CString	GetCode();
	CString	GetName();
	bool	SetCode(CString strCode, bool bOnlySet = false, bool bSetHistory = true);
	bool	SetColor(COLORREF rgbFG, COLORREF rgbBG);
	bool	SetFont(CFont* pfont);
	bool	SetUnit(CString strUnit);	// "�ֽ�" : �ֽ� + ELW
						// "����" : ����
						// "����" : kospi ����
						// "�ɼ�" : kospi �ɼ�
						// "�ؿ�" : �ؿ�
						// "ELW" : ELW
						// "�����ɼ�" : kospi ���� + kospi �ɼ� 
						// "�ֽļ���" : �ֽļ���
						// "�ֽĿɼ�" : �ֽĿɼ�
						// "��ǰ�����ɼ�" : ��ǰ���� + ��ǰ�ɼ�

private:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeCtrl)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCodeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCodeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CODECTRL_H__8C6C1F37_A51A_4D7F_B461_FC7C57E521EF__INCLUDED_)



/*--------------------------------------------------------------------------------
// ����
//--------------------------------------------------------------------------------

����:
	m_pCodeCtrl = new CCodeCtrl;
	m_pCodeCtrl->CreateCodeCtrl(this, m_pwndWizard, CRect(1, 1, 1 + 70, 1 + 20), IDC_CODECTRL);
	m_pCodeCtrl->SetUnit("�ֽ�");


����ٲ�� ������ �̺�Ʈ:

.h ���Ͽ� ������ ���� ����
afx_msg void OnSelendokCodeCtrl();

.cpp ���Ͽ� ������ ���� ���� 
ON_CBN_SELENDOK(IDC_CODECTRL, OnSelendokCodeCtrl)

void CMapWnd::OnSelendokCodeCtrl() 
{
	CString strCode = m_pCodeCtrl->GetCode();
}

*/