// TB205001.h : TB205001 DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CTB205001App
// �� Ŭ������ ������ ������ TB205001.cpp�� �����Ͻʽÿ�.
//

class CTB205001App : public CWinApp
{
public:
	CTB205001App();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
