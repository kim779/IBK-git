// cx_history.h : cx_history DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// Ccx_historyApp
// �� Ŭ������ ������ ������ cx_history.cpp�� �����Ͻʽÿ�.
//

class Ccx_historyApp : public CWinApp
{
public:
	Ccx_historyApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
