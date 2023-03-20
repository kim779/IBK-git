/*
 * (C) Copyright 2005-2010. AhnLab, Inc.
 *
 * Any part of this source code can not be copied with
 * any method without prior written permission from
 * the author or authorized person.
 *
 * @version			$Revision: 1705 $
 * 
 */

#ifndef __AOSSDKDEF_H_
#define __AOSSDKDEF_H_

#include "aossdkerr.h"
#include "aossdkcom.h"
#include "aossdksb.h"

#ifdef AOSSDK_EXPORTS
#define AOSSDK_API __declspec(dllexport)
#else
#define AOSSDK_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

// --------------------------------------------------------------------------------------
// AOS �ʱ�ȭ : Anti KeyLogger �ʱ�ȭ/����
AOSSDK_API void Aossdk_Initialize();
AOSSDK_API void Aossdk_Uninitialize();


// --------------------------------------------------------------------------------------
// AOS ������Ʈ :  ������Ʈ ����
AOSSDK_API BOOL Aossdk_StartAosSDKA(								 
								LPCSTR szPidList,			// ��ǰ �ڵ� ����Ʈ  ex) e5|e0
								LPCSTR szUniqueKey,		// �������� ����ũŰ
								HWND Hwnd,					// ���� �������� HWND
								DWORD* pdwErrorCode			// �����ڵ�
								);
								
AOSSDK_API BOOL Aossdk_StartAosSDKW(								 
								LPCWSTR wszPidList,			// ��ǰ �ڵ� ����Ʈ  ex) e5|e0
								LPCWSTR wszUniqueKey,		// �������� ����ũŰ
								HWND Hwnd,					// ���� �������� HWND
								DWORD* pdwErrorCode			// �����ڵ�
								);								

// --------------------------------------------------------------------------------------
// AOS ������Ʈ :  MKD, MF40�� ���� ������Ʈ ����
AOSSDK_API BOOL Aossdk_StartExAosSDKA( LPCSTR szUniqueKey, HWND Hwnd, DWORD* pdwErrorCode);
AOSSDK_API BOOL Aossdk_StartExAosSDKW( LPCWSTR wszUniqueKey, HWND Hwnd, DWORD* pdwErrorCode);

// --------------------------------------------------------------------------------------
// AOS ������Ʈ : ����������� ����
AOSSDK_API void Aossdk_SetAuthServerA(LPCSTR szAuthServer);
AOSSDK_API void Aossdk_SetAuthServerW(LPCWSTR wszAuthServer);

// --------------------------------------------------------------------------------------
// AOS ������Ʈ : �������� ��Ʈ��� ����
AOSSDK_API void Aossdk_SetAuthRootUrlA(LPCSTR szAuthRootUrl);
AOSSDK_API void Aossdk_SetAuthRootUrlW(LPCWSTR wszAuthRootUrl);

// --------------------------------------------------------------------------------------
// AOS ������Ʈ : UI ǥ�ø�� ����(0:ǥ�þ���, 1:ǥ����)
AOSSDK_API void Aossdk_SetModeUI(int nModeUI);

// --------------------------------------------------------------------------------------
// AOS ������Ʈ : �񵿱��� ����(TRUE:�񵿱���, FALSE:������)
AOSSDK_API void Aossdk_SetModeAsync(BOOL bAsync);

// --------------------------------------------------------------------------------------
// AOS ������Ʈ : Anti KeyLogger�� ������ ��ǰ���� ����
AOSSDK_API BOOL Aossdk_TerminatePdA(DWORD dwServiceCode, LPCSTR szItem);
AOSSDK_API BOOL Aossdk_TerminatePdW(DWORD dwServiceCode, LPCWSTR wszItem);

AOSSDK_API void Aossdk_TerminateSDKA( LPCSTR szItem );
AOSSDK_API void Aossdk_TerminateSDKW( LPCWSTR szItem );

// --------------------------------------------------------------------------------------
// AOS ������Ʈ : ������Ʈ ���� Ű���� ���� �ε� �����ϵ��� ����
AOSSDK_API void Aossdk_SkipUpdate();

// --------------------------------------------------------------------------------------
// AOS Verify Interface
AOSSDK_API IAosVerify* Aossdk_GetVerifyObject(DWORD* pdwErrorCode);

// --------------------------------------------------------------------------------------
// AOS Ű���� ���� : Interface
AOSSDK_API IAosMkdA* Aossdk_GetMkdObjectA(DWORD* pdwErrorCode);
AOSSDK_API IAosMkdW* Aossdk_GetMkdObjectW(DWORD* pdwErrorCode);
AOSSDK_API IAosMkdS4* Aossdk_GetMkdS4Object(DWORD* pdwErrorCode);
AOSSDK_API void Aossdk_MkdUnload();

// --------------------------------------------------------------------------------------
// AOS Secure Browser : Interface
AOSSDK_API IAosSB*	Aossdk_GetSBObject(DWORD* pdwErrorCode);
AOSSDK_API BOOL	Aossdk_GetSBStatus(DWORD dwOption);

#ifdef __cplusplus
}
#endif

#endif // __AOSSDKDEF_H_
