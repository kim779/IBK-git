#pragma once
// ControlWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CControlWnd window
#include <afxtempl.h>
#include "../../h/axisfire.h"

#define DF_FILESIZE (1024 * 10)
//#define DF_FILESIZE (1024 )
#define DF_FIRST	'F'
#define DF_MIDDLE	'M'
#define DF_LAST		'L'

struct st_mid_PMHOUPLD
{
	char	flow;
	char	name[32];
	char	dlen[5];
	char	data[14000];  //DF_FILESIZE
};
#define	L_MID	sizeof(struct st_mid_PMHOUPLD)
#define	L_xMID	sizeof(struct st_mid_PMHOUPLD) - 14000	// - data size

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd();

	CWnd*	m_pWizard;
	struct	_param  m_param;

	CFile	m_file;
	int	m_offset;
	int	m_size;

	CString m_strFileName;
	CString m_filename;

	void	SetParam(_param *pParam);

// Attributes
public:

// Operations
protected:
	int	sendData();
	int	sendTR(CString strName, BYTE type, char* datb, int datl);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)

public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg BSTR GetFilepath();
	afx_msg void SetFilepath(LPCTSTR lpszNewValue);
	afx_msg short upload(LPCTSTR sPath);
	afx_msg BSTR GetFilePath();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};