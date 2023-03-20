/*
 * (C) Copyright 2005-2010. AhnLab, Inc.
 *
 * Any part of this source code can not be copied with
 * any method without prior written permission from
 * the author or authorized person.
 *
 * @version			$Revision: 1056 $
 * 
 */

#ifndef __AOSSDKRADDEF_H_
#define __AOSSDKRADDEF_H_

#include "aossdkerr.h"
#include "aossdkcom.h"
#include "aossdksb.h"

#ifdef AOSSDKRAD_EXPORTS
#define AOSSDKRAD_API __declspec(dllexport)
#else
#define AOSSDKRAD_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif


// --------------------------------------------------------------------------------------
// AOS �ʱ�ȭ : Interface

// Initialize
AOSSDKRAD_API void _stdcall Aossdkrad_1();

// Uninitialize
AOSSDKRAD_API void _stdcall Aossdkrad_10();



// --------------------------------------------------------------------------------------
// AOS Ű���� ���� : Interface

// Initialize
AOSSDKRAD_API DWORD _stdcall Aossdkrad_2(DWORD dwMode, BOOL bIntegratedWithAos = FALSE);

// Uninitialize
AOSSDKRAD_API DWORD _stdcall Aossdkrad_3();

// ProtectEditControl
AOSSDKRAD_API DWORD _stdcall Aossdkrad_4(HWND hWnd);

// ProtectEditControl2
AOSSDKRAD_API DWORD _stdcall Aossdkrad_5(HWND hWnd, DWORD dwType, DWORD dwMode, DWORD dwMaxLength);

// SetInsertMode
AOSSDKRAD_API DWORD _stdcall Aossdkrad_6(HWND hWnd, BOOL bInsert);

// GetTextType
AOSSDKRAD_API DWORD _stdcall Aossdkrad_7(HWND hWnd, DWORD* pdwType);

// GetTextA
AOSSDKRAD_API DWORD _stdcall Aossdkrad_8_A(HWND hWnd, CHAR* szOutput, DWORD* pdwSize);

// GetTextW
AOSSDKRAD_API DWORD _stdcall Aossdkrad_8_W(HWND hWnd, WCHAR* szOutput, DWORD* pdwSize);

// SetTextA
AOSSDKRAD_API DWORD _stdcall Aossdkrad_9_A(HWND hWnd, CONST CHAR* lpszText);

// SetTextW
AOSSDKRAD_API DWORD _stdcall Aossdkrad_9_W(HWND hWnd, CONST WCHAR* lpszText);



// --------------------------------------------------------------------------------------
// AOS ������Ʈ :  Interface

//StartAosSDK
AOSSDKRAD_API BOOL _stdcall Aossdkrad_11_A(CONST CHAR* szPidList, CONST CHAR* szUniqueKey, HWND Hwnd, DWORD* pdwErrorCode);
AOSSDKRAD_API BOOL _stdcall Aossdkrad_11_W(CONST WCHAR* szPidList, CONST WCHAR* szUniqueKey, HWND Hwnd, DWORD* pdwErrorCode);

// --------------------------------------------------------------------------------------
// AOS ������Ʈ :  MKD2.5, MF40�� ���� ������Ʈ ����
//StartExAosSDK
AOSSDKRAD_API BOOL _stdcall Aossdkrad_12_A(CONST CHAR* szUniqueKey, HWND Hwnd, DWORD* pdwErrorCode);
AOSSDKRAD_API BOOL _stdcall Aossdkrad_12_W(CONST WCHAR* szUniqueKey, HWND Hwnd, DWORD* pdwErrorCode);

// --------------------------------------------------------------------------------------
// AOS ������Ʈ : ����������� ����
//SetAuthServer
AOSSDKRAD_API void _stdcall Aossdkrad_13_A(CONST CHAR* szAuthServer);
AOSSDKRAD_API void _stdcall Aossdkrad_13_W(CONST WCHAR* szAuthServer);

// --------------------------------------------------------------------------------------
// AOS ������Ʈ : UI ǥ�ø�� ����(0:ǥ�þ���, 1:ǥ����)
//SetModeUI
AOSSDKRAD_API void _stdcall Aossdkrad_14(int nModeUI);

// --------------------------------------------------------------------------------------
// AOS ������Ʈ : �񵿱��� ����(TRUE:�񵿱���, FALSE:������)
//SetModeAsync
AOSSDKRAD_API void _stdcall Aossdkrad_15(BOOL bAsync);

// --------------------------------------------------------------------------------------
// AOS ������Ʈ : Anti KeyLogger�� ������ ��ǰ���� ����
//TerminatePd
AOSSDKRAD_API void _stdcall Aossdkrad_16_A(CONST CHAR* szItem);
AOSSDKRAD_API void _stdcall Aossdkrad_16_W(CONST WCHAR* szItem);

// --------------------------------------------------------------------------------------
// AOS ������Ʈ : �������� ��Ʈ��� ����
//SetAuthRootUrl
AOSSDKRAD_API void _stdcall Aossdkrad_17_A(CONST CHAR* szAuthRootUrl);
AOSSDKRAD_API void _stdcall Aossdkrad_17_W(CONST WCHAR* szAuthRootUrl);

//TerminatePd
AOSSDKRAD_API void _stdcall Aossdkrad_18_A(CONST CHAR* szItem);
AOSSDKRAD_API void _stdcall Aossdkrad_18_W(CONST WCHAR* szItem);

//----------------------------------------------------------------------------------
// AOS Secure Browser
// 

// AosSB_Initialize
AOSSDKRAD_API DWORD _stdcall Aossdkrad_19(DWORD dwOption, PFN_SBSDK_Callback fnCallback);

// AosSB_Initialize2
AOSSDKRAD_API DWORD _stdcall Aossdkrad_24(DWORD dwOption, HWND hWnd, DWORD dwMsgNum);

// AosSB_Uninitialize
AOSSDKRAD_API DWORD _stdcall Aossdkrad_20();

// --------------------------------------------------------------------------------------
// AOS Verify Interface

// IsRunningPd
AOSSDKRAD_API BOOL _stdcall Aossdkrad_21(DWORD dwServiceCode);

// VerifyTrustSelfA
AOSSDKRAD_API BOOL _stdcall Aossdkrad_22_A(LPCSTR szSignedName);

// VerifyTrustSelfW
AOSSDKRAD_API BOOL _stdcall Aossdkrad_22_W(LPCWSTR swSignedName);

// VerifyTrustSDK
AOSSDKRAD_API BOOL _stdcall Aossdkrad_23();

#ifdef __cplusplus
}
#endif

#endif // __AOSSDKRADDEF_H_
