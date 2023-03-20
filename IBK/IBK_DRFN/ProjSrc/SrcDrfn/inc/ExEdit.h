// ExEdit.h: interface for the CExEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXEDIT_H__D6370A44_19DC_4797_8EDB_92FF270DE1A8__INCLUDED_)
#define AFX_EXEDIT_H__D6370A44_19DC_4797_8EDB_92FF270DE1A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoaderEdit;
class CExEdit : public CEdit  
{
	DECLARE_DYNAMIC(CExEdit)
// Construction
public:
	CExEdit(BOOL bUseDefSkin = FALSE);

// Attributes
protected:
	CLoaderEdit*		m_pLoader;

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
	long		ActionControl(LPVOID lpAction);

	void	UseNumericType(BOOL bUse);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExEdit)
	afx_msg void OnPaint();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_EXEDIT_H__D6370A44_19DC_4797_8EDB_92FF270DE1A8__INCLUDED_)
