// fx_edit.h : fx_edit DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// Cfx_editApp
// �� Ŭ������ ������ ������ fx_edit.cpp�� �����Ͻʽÿ�.
//

class Cfx_editApp : public CWinApp
{
public:
	Cfx_editApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
