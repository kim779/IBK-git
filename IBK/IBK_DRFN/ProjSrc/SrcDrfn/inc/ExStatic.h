// ExStatic.h: interface for the CExStatic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXSTATIC_H__751A3F09_37B9_468F_AF59_79B0C19C6A5B__INCLUDED_)
#define AFX_EXSTATIC_H__751A3F09_37B9_468F_AF59_79B0C19C6A5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoaderStatic;
class CExStatic : public CStatic  
{
	DECLARE_DYNCREATE(CExStatic);

// Construction
public:
	CExStatic(BOOL bUseDefSkin = FALSE);

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
	// Return		: ����ƽ ���� ���� ����
	// Paramter
	//		COLORREF clrOutLine : �ٱ��� ���� ����
	//		COLORREF clrBackground : ��� ����
	//		COLORREF clrText : �ؽ�Ʈ ����
	// Comments		: �÷����̺��� �̿����� �ʰ� ����ƽ�� �׸�
	//-----------------------------------------------------------------------------
	BOOL	SetStaticColor(COLORREF clrOutLine, COLORREF clrBackground, COLORREF clrText);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: ����ƽ ���� ���� ����
	// Paramter
	//		UINT nOutLine : �ٱ��� ���� ���� �ε��� (�÷����̺� ���ǵǾ��ִ� �ε���)
	//		UINT nBackground : ��� ���� �ε��� (�÷����̺� ���ǵǾ��ִ� �ε���)
	//		UINT nText : �ؽ�Ʈ ���� �ε��� (�÷����̺� ���ǵǾ��ִ� �ε���)
	// Comments		: �÷����̺� �ε����� �̿��Ͽ� ����ƽ�� �׸�
	//-----------------------------------------------------------------------------
	BOOL	SetSkinIndex(UINT nOutLine, UINT nBackground, UINT nText);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: ����ƽ ���� ���� ����
	// Paramter
	//		LPCTSTR lpszOutLine : �ٱ��� ���� ���� �̸� (�÷����̺� ���ǵǾ��ִ� �̸�)
	//		LPCTSTR lpszBackground : ��� ���� �̸� (�÷����̺� ���ǵǾ��ִ� �̸�)
	//		LPCTSTR lpszText : �ؽ�Ʈ ���� �̸� (�÷����̺� ���ǵǾ��ִ� �̸�)
	// Comments		: �÷����̺� �̸��� �̿��Ͽ� ����ƽ�� �׸�
	//-----------------------------------------------------------------------------
	BOOL	SetSkinString(LPCTSTR lpszOutLine, LPCTSTR lpszBackground, LPCTSTR lpszTextClr);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: ������ ���� ����
	//		CSize size : ������ ũ��
	//		HICON hIcon : Up ������ �ڵ�
	//		HICON hIconPushed : Down ������ �ڵ�
	//		UINT nIconAlign : ������ ���Ĺ�� (STC_ICON_NORMAL:����, STC_ICON_RIGHT:������, STC_ICON_TWOROWS:����)
	// Comments		: �������� �����մϴ�.
	//-----------------------------------------------------------------------------
	BOOL	SetIconImage(CSize size, HICON hIcon, UINT nIconAlign = STC_ICON_NORMAL);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: void
	// Paramter
	//		LPCTSTR lpszText : ������ ����ƽ �ؽ�Ʈ
	// Comments		: ����ƽ�� �ؽ�Ʈ�� �����մϴ�.
	//				  SetWindowText�Լ��� �ؽ�Ʈ�� �����Ҷ����� Invalidate�Լ� �ٽ� �׷��� �մϴ�. 
	//-----------------------------------------------------------------------------
	void	SetText(LPCTSTR lpszText);

	void	GetWindowText(CString& rString);

// Attributes
protected:
	CLoaderStatic*		m_pLoader;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExStatic)
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExStatic)
	afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_EXSTATIC_H__751A3F09_37B9_468F_AF59_79B0C19C6A5B__INCLUDED_)
