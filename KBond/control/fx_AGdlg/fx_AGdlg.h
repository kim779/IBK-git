// fx_AGdlg.h : fx_AGdlg DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// Cfx_AGdlgApp
// �� Ŭ������ ������ ������ fx_AGdlg.cpp�� �����Ͻʽÿ�.
//

class Cfx_AGdlgApp : public CWinApp
{
public:
	Cfx_AGdlgApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
