// CX_griddata.h : CX_griddata DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CCX_griddataApp
// �� Ŭ������ ������ ������ CX_griddata.cpp�� �����Ͻʽÿ�.
//

class CCX_griddataApp : public CWinApp
{
public:
	CCX_griddataApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
