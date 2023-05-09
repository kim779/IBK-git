#pragma once
// ToolWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolWnd window
#include "BaseWnd.h"

//#include "../../control/fx_misc/fxImgButton.h"

//#include "fxSpin.h"

//#include "btntab.h"
//#include "NewMenu.h"	// ADD PSH 20070912


class CToolWnd : public CBaseWnd
{
// Construction
public:
	CToolWnd(CWnd* pMainWnd);
	CString m_slog;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolWnd)
	protected:
	

	//}}AFX_VIRTUAL
	void OperInit();
	void OperDraw(CDC* pDC);
	void OperDestory();
	void OperResize(int cx, int cy);
	void OperPallette();
	
// Implementation
public:
	virtual ~CToolWnd();

	// Generated message map functions
public:
	//{{AFX_MSG(CToolWnd)
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	
	void	loadcfg();
	void	savecfg();
	void	SendTree(int nIndex);
	void SetViewType();


	//�ű�
	BOOL m_bExpectCheck = FALSE;
	int m_iViewType;    //0 100����,  1 ��
	BOOL m_bAutoCheck;
	int GetMainViewType();  //���ο� �ִ� m_iViewType �����´�

	CString		m_fileFIELD;
	BOOL		m_bMoveCfg;
	int			m_nMoveSave;

};