// CX_FileRead.h : CX_FileRead DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CCX_FileReadApp
// �� Ŭ������ ������ ������ CX_FileRead.cpp�� �����Ͻʽÿ�.
//

class CCX_FileReadApp : public CWinApp
{
public:
	CCX_FileReadApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
