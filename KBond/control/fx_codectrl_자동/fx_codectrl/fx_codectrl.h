// fx_codectrl.h : fx_codectrl DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// Cfx_codectrlApp
// �� Ŭ������ ������ ������ fx_codectrl.cpp�� �����Ͻʽÿ�.
//

class Cfx_codectrlApp : public CWinApp
{
public:
	Cfx_codectrlApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
