/*****************************************************************************
 
    Copyright   AhnLab, Inc. 1998-2006, All rights reserved.
 
    Any part of this source code can not be copied with
    any method without prior written permission from
    the author or authorized person.
	   
*****************************************************************************/

#ifndef __SUIPREDEF_H_
#define __SUIPREDEF_H_


#include "SUiPreCommon.h"

// ������Ʈ�� ����
// return : ��ǰ��ġ ����(TRUE:��ġ�� �Ǿ����Ƿ� ������ ������ ����, FALSE:����Ұ� ����)
extern "C" __declspec(dllimport) BOOL SUiPre_StartA(	DWORD dwPdCode,				// ��ǰ�ڵ�
														LPCTSTR szUniqueKey,		// �������� ����ũŰ
														HWND Hwnd,					// ���� �������� HWND
														PFN_SUI_STATUS pfnStatus,	// �ݹ��Լ�(NULL ����)
														void* pParam,				// �ݹ� ������(NULL ����)
														DWORD* pdwErrorCode			// �����ڵ�
													);

// ������Ʈ�� ����(������ ������ SDK�� ���ó�� ���߱� ���ؼ��� ����Ѵ�)
// return : ��ǰ��ġ ����(TRUE:��ġ�� �Ǿ����Ƿ� ������ ������ ����, FALSE:����Ұ� ����)
extern "C" __declspec(dllimport) BOOL SUiPre_Start(	DWORD dwPdCode,				// ��ǰ�ڵ�
													LPCTSTR szUniqueKey,		// �������� ����ũŰ
													long lParam,				// �ݹ� ������(NULL ����)
													PFN_SUI_STATUS pfnStatus,	// �ݹ��Լ�(NULL ����)
													DWORD* pdwErrorCode			// �����ڵ�
													);

// UI ǥ�ø�� ����(0:ǥ�þ���, 1:ǥ����)
extern "C" __declspec(dllimport) void SUiPre_SetModeUI(int nModeUI);

// �񵿱��� ����(TRUE:�񵿱���, FALSE:������)
extern "C" __declspec(dllimport) void SUiPre_SetModeAsync(BOOL bAsync);

// ������Ʈ ���࿩�� ����, ��ǰ�ڵ尡 �����Ǿ� ������ �ش� ��ǰ ������Ʈ���� �ʰ� �н�
extern "C" __declspec(dllexport) void SUiPre_SetIfOnThenPass(DWORD dwIfOnThenPass);

// ��ǰ���� ����
extern "C" __declspec(dllimport) BOOL SUiPre_TerminatePd(DWORD dwServiceCode, LPCTSTR szItem);

#endif // __SUIPREDEF_H_