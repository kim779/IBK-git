// fx_codectrl.cpp : �ش� DLL�� �ʱ�ȭ ��ƾ�� �����մϴ�.
//

#include "stdafx.h"
#include "fx_codectrl.h"
#include "Controlwnd.h"

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

// Cfx_codectrlApp

BEGIN_MESSAGE_MAP(Cfx_codectrlApp, CWinApp)
END_MESSAGE_MAP()


// Cfx_codectrlApp ����

Cfx_codectrlApp::Cfx_codectrlApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ Cfx_codectrlApp ��ü�Դϴ�.

Cfx_codectrlApp theApp;

const GUID CDECL _tlid = { 0x9717A53F, 0x325A, 0x456E, { 0x99, 0xEF, 0x57, 0xD5, 0x28, 0x3, 0x29, 0xD5 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// Cfx_codectrlApp �ʱ�ȭ

BOOL Cfx_codectrlApp::InitInstance()
{
	CWinApp::InitInstance();

	// OLE ����(���͸�)�� ���� ���� ������ ����մϴ�. �̷��� �ϸ�
	//  OLE ���̺귯���� �ٸ� ���� ���α׷����� ��ü�� ���� �� �ֽ��ϴ�.
	COleObjectFactory::RegisterAll();

	return TRUE;
}

// DllGetClassObject - Ŭ���� ���͸��� ��ȯ�մϴ�.

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}


// DllCanUnloadNow - COM���� DLL�� ��ε��� �� �ֵ��� �մϴ�.

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}


// DllRegisterServer - �ý��� ������Ʈ���� �׸��� �߰��մϴ�.

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll())
		return SELFREG_E_CLASS;

	return S_OK;
}


// DllUnregisterServer - �ý��� ������Ʈ������ �׸��� �����մϴ�.

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll(FALSE))
		return SELFREG_E_CLASS;

	return S_OK;
}

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CControlwnd *m_pControlWnd = new CControlwnd();
	m_pControlWnd->m_pWizard = parent;
	m_pControlWnd->SetParam((struct _param*)pParam);
	
	m_pControlWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, m_pControlWnd->m_Param.rect, parent, 100);
	return m_pControlWnd;
	
}
