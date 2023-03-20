#if !defined(AFX_EXDRAWBUTTON_H__42FFF992_01A3_4F06_BEFC_D26E6C64F1E4__INCLUDED_)
#define AFX_EXDRAWBUTTON_H__42FFF992_01A3_4F06_BEFC_D26E6C64F1E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExDrawButton.h : header file
//
#include "ExButton.h"

/////////////////////////////////////////////////////////////////////////////
// CExDrawButton window
class CLoaderDrawButton;
class CExDrawButton : public CExButton
{
// Construction
public:
	CExDrawButton(BOOL bUseDefSkin = FALSE);

// Attributes
protected:
	CLoaderDrawButton*		m_pLoader;
	POSITION				m_posChangedSkin;

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
	// Return		: �������� ���� ����
	// Paramter
	//		UINT nButton : ��ư ���� (�⺻��ư SKINBTN_DEFAULT)
	//		SKINBTN_ACCENT : ������ư, SKINBTN_TAB : �ǹ�ư, SKINBTN_BUY : �ż���ư, SKINBTN_SELL : �ŵ���ư, SKINBTN_CCLMDY : ���������ư
	// Comments		: �⺻��ư�� ��� �� �Լ��� ȣ������ �ʾƵ� �˴ϴ�.
	//				  �⺻��ư�� �÷����̺� �⺻��ư ������ �̿��մϴ�. (���� ������ �ʿ����)
	//				  ���ο��� ��Ų�� ����Ǿ����� �ڵ����� ����˴ϴ�.
	//-----------------------------------------------------------------------------
	BOOL		SetButtonColor(UINT nButton = 0);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: ������ ���� ����
	// Paramter
	//		CSize size : ������ ũ��
	//		HICON hIcon : Up ������ �ڵ�
	//		HICON hIconPushed : Down ������ �ڵ�
	//		UINT nIconAlign : ������ ���Ĺ�� (BTN_ICON_NORMAL:����, BTN_ICON_RIGHT:������, BTN_ICON_TWOROWS:����)
	// Comments		: �������� �����մϴ�.
	//				  ��Ʈ�� ������ ���̴� �������� ����� TransParent�̰� ��ư ũ�⺯�濡�� �������� �����˴ϴ�.
	//-----------------------------------------------------------------------------
	BOOL		SetIconImage(CSize size, HICON hIcon, HICON hIconPushed = NULL, UINT nIconAlign = BTN_ICON_NORMAL);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: void
	// Paramter
	//		LPCTSTR lpszText : ������ ��ư �ؽ�Ʈ
	// Comments		: ��ư�� �ؽ�Ʈ�� �����մϴ�.
	//				  SetWindowText�Լ��� �ؽ�Ʈ�� �����Ҷ����� Invalidate�Լ� �ٽ� �׷��� �մϴ�. 
	//-----------------------------------------------------------------------------
	void		SetText(LPCTSTR lpszText);
	LPCTSTR		GetText();

	virtual void	UseToggleMode(BOOL bUse = TRUE);
	virtual BOOL	GetToggleState();
	virtual void	SetToggleState(BOOL bPush);
	virtual void	ReleasePushed();
	
	// ������ ���ڴ� �׻� NULL
	void AddToggleGroup(CExDrawButton* pButton, ...);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExDrawButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExDrawButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExDrawButton)
	afx_msg void OnDestroy();
	afx_msg BOOL OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXDRAWBUTTON_H__42FFF992_01A3_4F06_BEFC_D26E6C64F1E4__INCLUDED_)
