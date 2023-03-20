// ExMaskEdit.h: interface for the CExMaskEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXMASKEDIT_H__5CBB0784_4C64_4D30_AEAD_AF031A4F5FD4__INCLUDED_)
#define AFX_EXMASKEDIT_H__5CBB0784_4C64_4D30_AEAD_AF031A4F5FD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ExEdit.h"

class CLoaderMaskEdit;
class CExMaskEdit : public CEdit
{
	DECLARE_DYNAMIC(CExMaskEdit)
	
public:
	CExMaskEdit(BOOL bUseDefSkin = FALSE);

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

	BOOL	CanOverType();
	BOOL	CanUseMask();
	BOOL	IsModified();
	BOOL	PosInRange(int nPos);
	TCHAR	GetPromptChar();
	void	MaskClear();
	void	MaskCopy();
	void	MaskCut();
	void	MaskPaste();
	void	MaskSelectAll();
	void	MaskUndo();
	void	SetOverType(BOOL bOverType);
	void	SetPromptChar(TCHAR ch);
	void	SetUseMask(BOOL bUseMask);
	
	virtual void SetEditMask(LPCTSTR lpszMask, LPCTSTR lpszLiteral, LPCTSTR lpszDefault = NULL);
	virtual BOOL CheckChar(UINT& nChar, BOOL bBeep = TRUE);
	virtual BOOL ProcessMask(UINT& nChar, int nEndPos);

protected:
	CLoaderMaskEdit*		m_pLoader;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExAcctEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	virtual BOOL CorrectPosition(int& nPos, BOOL bForward = TRUE, BOOL bBeep = TRUE);
	virtual void DeleteCharAt(int nPos);
	virtual void InsertCharAt(int nPos, TCHAR nChar);
	virtual BOOL CopyToClipboard(LPCTSTR lpszText);
	virtual CString GetMaskedText(int nPos = 0);
	virtual BOOL SelectionMade();

// Implementation
public:
	virtual ~CExMaskEdit();
	
protected:
	//{{AFX_MSG(CExAcctEdit)
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_EXMASKEDIT_H__5CBB0784_4C64_4D30_AEAD_AF031A4F5FD4__INCLUDED_)
