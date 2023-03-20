#if !defined(AFX_BUTTONEX_H__25280FC2_5348_4128_9D02_4829CDFC5EB0__INCLUDED_)
#define AFX_BUTTONEX_H__25280FC2_5348_4128_9D02_4829CDFC5EB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ButtonEx.h : header file
// ���ٿ� ����ϴ� ��ưŬ����. 
// (��Ʈ���̹����� �ε��Ͽ� Draw, ������ �ؽ�Ʈ �����ؼ� �����ֱⰡ ������)

#include "ToolOption.h"

/////////////////////////////////////////////////////////////////////////////
// CButtonEx window
class CObjTool;
class CToolBarDlg;

class CButtonEx : public CButton
{
// Construction
public:
	CButtonEx();

// Attributes
protected:
	CToolBarDlg* m_pParent;
	CToolOptionInfo::TOOLOPTION m_enumToolOption;
	CString m_strOption1;
	CString m_strOption2;
	CString m_strParam;
	bool	m_bState;	// ��۹�ư�� üũ�ڽ��� �����Ǿ�� �ϳ� ���⼱ ��ưŬ������ �����ߴ�. 
						// ����䳻���� ���� ����.
private:
	CToolTipCtrl m_ToolTip;
	CString m_strToolTipText;

// Operations
public:
	CToolOptionInfo::TOOLOPTION GetToolOptionInfo();

	void SetParentWnd(CToolBarDlg* pParent){ m_pParent = pParent;};
	void SetToolObject(CObjTool* pObjTool, const CString& strParam = _T(""));	// ToolButton ���� ������Ʈ�� �����Ѵ�.
	void SetToolTipText(const CString& strText);	// ���� ���ڿ��� �����Ѵ�.
	void SetState(bool bState = true);	// ��ư ���¸� �����Ѵ�. true-����, false-�ȴ���.
	bool GetState();	// ��ư�� ���¸� �����Ѵ�.
	const CString& GetOption1();	// ��ư�� ù��° �ɼǰ� ����
	const CString& GetOption2();	// ��ư�� �ι�° �ɼǰ� ����
	const CString& GetParam();		// ��ư�� �Ķ���Ͱ� ����.

private:
	void AddToolInTooltip(const CString& strText, const bool bActivate);
	void SetToolTipText(const CString& strText, const bool bActivate);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonEx)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CButtonEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CButtonEx)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTONEX_H__25280FC2_5348_4128_9D02_4829CDFC5EB0__INCLUDED_)
