// TB900100.cpp : �ش� DLL�� �ʱ�ȭ ��ƾ�� �����մϴ�.
//

#include "stdafx.h"
#include "TB900100.h"
#include "MapWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: �� DLL�� MFC DLL�� ���� �������� ��ũ�Ǿ� �ִ� ���
//		MFC�� ȣ��Ǵ� �� DLL���� ���������� ��� �Լ���
//		���� �κп� AFX_MANAGE_STATE ��ũ�ΰ�
//		��� �־�� �մϴ�.
//
//		��:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �Ϲ����� �Լ� ������ ���⿡ �ɴϴ�.
//		}
//
//		�� ��ũ�δ� MFC�� ȣ���ϱ� ����
//		�� �Լ��� �ݵ�� ��� �־�� �մϴ�.
//		��, ��ũ�δ� �Լ��� ù ��° ���̾�� �ϸ� 
//		��ü ������ �����ڰ� MFC DLL��
//		ȣ���� �� �����Ƿ� ��ü ������ ����Ǳ� ����
//		���;� �մϴ�.
//
//		�ڼ��� ������
//		MFC Technical Note 33 �� 58�� �����Ͻʽÿ�.
//

// CTB900100App

BEGIN_MESSAGE_MAP(CTB900100App, CWinApp)
END_MESSAGE_MAP()


// CTB900100App ����

CTB900100App::CTB900100App()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CTB900100App ��ü�Դϴ�.

CTB900100App theApp;


// CTB900100App �ʱ�ȭ

BOOL CTB900100App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *pParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CMapWnd *pMapWnd = new CMapWnd(pParent);
	pMapWnd->m_pWizard = pParent;
	if (!pMapWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 
		CRect(0, 0, 620, 530), pParent, 100, NULL))
	{
		delete pMapWnd;
		return NULL;
	}
	
	return pMapWnd;
}