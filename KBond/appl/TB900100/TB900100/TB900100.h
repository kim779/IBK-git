// TB900100.h : TB900100 DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CTB900100App
// �� Ŭ������ ������ ������ TB900100.cpp�� �����Ͻʽÿ�.
//

class CTB900100App : public CWinApp
{
public:
	CTB900100App();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
