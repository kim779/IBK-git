// CX_POP.h : CX_POP DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CCX_POPApp
// �� Ŭ������ ������ ������ CX_POP.cpp�� �����Ͻʽÿ�.
//

class CCX_POPApp : public CWinApp
{
public:
	CCX_POPApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
