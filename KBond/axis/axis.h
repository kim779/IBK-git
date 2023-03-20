// axis.h : main header file for the AXIS application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"					// main symbols
#include "UnhandledException/ExceptionHandler.h"
/////////////////////////////////////////////////////////////////////////////
// CAxisApp:
// See axis.cpp for the implementation of this class
//
// program kind
#define	PK_DEV		'D'	// DEFAULT DEV
#define	PK_BUSINESS	'B'	// ������
#define	PK_HTS		'H'	// �ݼ���/������
				// �ü�����/�����
				// ����, �ü�����
				// �Ϲݻ����

#define	MD_DEV		0x7f	// ���߿� ��� ��� ȭ�� open


#define FRAMELINE	  RGB(58, 65, 80)//RGB(65, 67, 71)

//SHPARK �ӽ�
#define VK_OEM_7          0xDE  //  ''"' for US
#define VK_OEM_1          0xBA  //  ''"' for US

class CAxisApp : public CWinApp, public IExceptionHandler
{
public:
	CAxisApp();
	~CAxisApp();

	CExceptionHandler m_eh;   //e_log
	void	ExceptionInformation(CString strInfo);		// elog

	class CImpIDispatch* m_pDispOM;
public:
	class	CAxInstance*	m_instance;
	CString	m_regkey;
	CString m_exeName;
	CString	m_user;
	CString	m_sFireKey;
	CString	m_eFireKey;
	
	char	m_progK;
	char	m_flag;
	char	m_mode;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CAxisApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
extern CAxisApp theApp;
//---------------------------------------------------------------------------
//
//	CCommParam
//
class CCommParam : public CCommandLineInfo
{
// Construction
public:
	CCommParam();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommParam)
	virtual void ParseParam(LPCSTR lpszParam, BOOL bFlag, BOOL bLast);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCommParam();
};

