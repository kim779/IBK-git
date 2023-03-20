#if !defined(AFX_EXBITMAPBUTTON_H__564E404A_690E_4C65_973C_447E90B6E41B__INCLUDED_)
#define AFX_EXBITMAPBUTTON_H__564E404A_690E_4C65_973C_447E90B6E41B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExBitmapButton.h : header file
//
#include "ExButton.h"

/////////////////////////////////////////////////////////////////////////////
// CExBitmapButton window
class CLoaderBitmapButton;
class CExBitmapButton : public CExButton
{
// Construction
public:
	CExBitmapButton();

// Attributes
protected:
	CLoaderBitmapButton*	m_pLoader;

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

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: ��ư�� ��Ʈ�� �ڵ��� ���õǾ����� ����
	// Paramter
	//		LPCTSTR lpszUp : Up������ ��Ʈ�� ���� ���
	//		LPCTSTR lpszDown : Down������ ��Ʈ�� ���� ���
	//		LPCTSTR lpszDis : ��Ȱ��ȭ������ ��Ʈ�� ���� ���
	// Comments		: ��ư ����Ʈ�� ��Ʈ������ ó���� ��� �� �Լ��� �̿��մϴ�.
	//-----------------------------------------------------------------------------	
	BOOL		SetBitmapFile(LPCTSTR lpszUp, LPCTSTR lpszDown, LPCTSTR lpszDis = NULL);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExBitmapButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExBitmapButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExBitmapButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXBITMAPBUTTON_H__564E404A_690E_4C65_973C_447E90B6E41B__INCLUDED_)
