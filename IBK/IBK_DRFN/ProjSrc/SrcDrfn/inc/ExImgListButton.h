#if !defined(AFX_EXIMGLISTBUTTON_H__810E2818_9499_44EC_9E49_1FECA4720093__INCLUDED_)
#define AFX_EXIMGLISTBUTTON_H__810E2818_9499_44EC_9E49_1FECA4720093__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExImgListButton.h : header file
//
#include "ExButton.h"

/////////////////////////////////////////////////////////////////////////////
// CExImgListButton window
class CLoaderImgListButton;
class CExImgListButton : public CExButton
{
// Construction
public:
	CExImgListButton();

// Attributes
protected:
	CLoaderImgListButton*		m_pLoader;

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
	// Return		: CImageList* ������ �Ǿ����� ����
	// Paramter
	//		CImageList* pImageList : ��ư���� �̹����� ������ �ִ� �̹��� ����Ʈ ������
	//		int nNormal : Up ���� ����Ʈ �ε���
	//		int nDown : Down ���� ����Ʈ �ε���
	//		int nDisable : ��Ȱ��ȭ ���� ����Ʈ �ε���
	//		int nOver : MouseOver ���� ����Ʈ �ε���
	//		LPCSTR lpszText : ��ư �ؽ�Ʈ
	// Comments		: CImageList�� �̹����� �̿��Ͽ� ��ư�� �׸��ϴ�.
	//-----------------------------------------------------------------------------
	BOOL	SetImageList(CImageList* pImageList, int nNormal, int nDown, int nDisable, int nOver, LPCTSTR lpszText = NULL, COLORREF clrText = 0x000000, COLORREF clrTextDis = 0x000000);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: void
	// Paramter
	//		LPCTSTR lpszText : ������ ��ư �ؽ�Ʈ
	// Comments		: ��ư�� �ؽ�Ʈ�� �����մϴ�.
	//-----------------------------------------------------------------------------
	void		SetText(LPCTSTR lpszText);
	LPCTSTR		GetText();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExImgListButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExImgListButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExImgListButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXIMGLISTBUTTON_H__810E2818_9499_44EC_9E49_1FECA4720093__INCLUDED_)
