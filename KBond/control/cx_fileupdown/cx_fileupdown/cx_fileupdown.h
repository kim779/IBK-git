// cx_fileupdown.h : cx_fileupdown DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// Ccx_fileupdownApp
// �� Ŭ������ ������ ������ cx_fileupdown.cpp�� �����Ͻʽÿ�.
//

class Ccx_fileupdownApp : public CWinApp
{
public:
	Ccx_fileupdownApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
