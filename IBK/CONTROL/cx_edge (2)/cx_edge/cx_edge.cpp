﻿// cx_edge.cpp : DLL의 초기화 루틴을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "cx_edge.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 이 DLL이 MFC DLL에 대해 동적으로 링크되어 있는 경우
//		MFC로 호출되는 이 DLL에서 내보내지는 모든 함수의
//		시작 부분에 AFX_MANAGE_STATE 매크로가
//		들어 있어야 합니다.
//
//		예:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 일반적인 함수 본문은 여기에 옵니다.
//		}
//
//		이 매크로는 MFC로 호출하기 전에
//		각 함수에 반드시 들어 있어야 합니다.
//		즉, 매크로는 함수의 첫 번째 문이어야 하며
//		개체 변수의 생성자가 MFC DLL로
//		호출할 수 있으므로 개체 변수가 선언되기 전에
//		나와야 합니다.
//
//		자세한 내용은
//		MFC Technical Note 33 및 58을 참조하십시오.
//

// CcxedgeApp

BEGIN_MESSAGE_MAP(CcxedgeApp, CWinApp)
END_MESSAGE_MAP()


// CcxedgeApp 생성

CcxedgeApp::CcxedgeApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CcxedgeApp 개체입니다.

CcxedgeApp theApp;

const GUID CDECL _tlid = {0x1d1313ca,0x1919,0x4b6b,{0xae,0xe3,0x7b,0x07,0xc9,0x35,0x5d,0x38}};
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CcxedgeApp 초기화

BOOL CcxedgeApp::InitInstance()
{
	AfxOleInit();
	CWinApp::InitInstance();

	// OLE 서버(팩터리)를 실행 중인 것으로 등록합니다.  이렇게 하면
	//  OLE 라이브러리가 다른 애플리케이션에서 개체를 만들 수 있습니다.
	COleObjectFactory::RegisterAll();

	return TRUE;
}

// DllGetClassObject - 클래스 팩터리를 반환합니다.

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}


// DllCanUnloadNow - COM에서 DLL을 언로드할 수 있도록 합니다.

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}


// DllRegisterServer - 시스템 레지스트리에 항목을 추가합니다.

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll())
		return SELFREG_E_CLASS;

	return S_OK;
}


// DllUnregisterServer - 시스템 레지스트리에서 항목을 제거합니다.

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll(FALSE))
		return SELFREG_E_CLASS;

	return S_OK;
}
__declspec(dllexport) char* APIENTRY axEncdata(char* pdata)
{
	return nullptr;
}

struct	_param {
	int		key;			// LOWORD(key) : screen key, HIWORD(key) : input size
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point;			// font point
	int		style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};
#include "CMainWnd.h"
#include "CTest.h"
__declspec(dllexport) CWnd* APIENTRY axCreate(CWnd* parent, void* ptr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	/*
	CString slog, stmp;
	CTest cl_test;

	slog.Format("[vc2019] axcreate");
	OutputDebugString(slog);


	_param* pParam = (_param*)ptr;
	slog.Format("[vc2019] axcreate param option =  [%s]", pParam->options);
	OutputDebugString(slog);

	char* pid = (char*)parent->SendMessage(WM_USER, MAKEWPARAM(0x10, 0x0b), 0L);
	stmp.Format("%s", pid);
	slog.Format("[vc2019] axcreate ID =  [%s]", pid);
	OutputDebugString(slog);

	cl_test.Encryt(stmp);
	*/
	OutputDebugString(_T("[cx_edge] axcreate"));
	CMainWnd* pWnd = new CMainWnd;
	//pWnd.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(0, 0, 10, 10), parent, (UINT)9898);
	
	if (!pWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(0,0,10,10), parent, (UINT)pWnd))
	{
			//AfxMessageBox("window create fail");
		OutputDebugString(_T("[edge] create fail"));
		 	delete pWnd;
			pWnd = NULL;
	}
	return pWnd;
}