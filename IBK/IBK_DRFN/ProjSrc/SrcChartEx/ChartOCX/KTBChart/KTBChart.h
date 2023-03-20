#if !defined(AFX_KTBCHART_H__DBEBD6AC_DC7A_43F7_8796_9868D99F730C__INCLUDED_)
#define AFX_KTBCHART_H__DBEBD6AC_DC7A_43F7_8796_9868D99F730C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// KTBChart.h : main header file for KTBCHART.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CKTBChartApp : See KTBChart.cpp for implementation.

class CKTBChartApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.23
//
//	����	:	OCX�� ��ġ�� ��θ� ���Ͽ� �����Ѵ�.
//
//	����	:	1. ���� '\'�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
extern CString	g_strOcxPath;

// (2004.11.26, ��¿�) Real�� ����Ʈ�� ����������� ��ü �ð��� ������ �������� Flag���� ���� �����Ѵ�.
//		Ư��, ������ Global�� �Ӽ��� �ξ� HTS �⺻ ������ �����ϰ�,
//		������ Property�� �����Ͽ� Chart�� �Ӽ��� ������� �Ѵ�.
//		�⺻���� �Ǻ��� �������� �ϸ�, HTS�� �⺻ ���� INI File�� �����Ͽ� ��������Ѵ�.
extern BOOL g_bNormalizedMinuteTimeRule;

// (2004.11.26, ��¿�) �ܺο� ������ Interface�� Export Type�� Macro�� �����Ѵ�.
#define CHARTOCX_API extern "C" __declspec(dllexport)

#define _ICHARTCTRL_CTRL_HEADER			"KTBChartCtl.h"
#define _ICHARTCTRL_CTRL				CKTBChartCtrl

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KTBCHART_H__DBEBD6AC_DC7A_43F7_8796_9868D99F730C__INCLUDED)
