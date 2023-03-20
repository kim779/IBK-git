// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_WEBEDGE.h"
#include "MainWnd.h"
#include "../../H/axisfire.h"

#include "ViewComponent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static constexpr UINT s_runAsyncWindowMessage = WM_APP;

/////////////////////////////////////////////////////////////////////////////
// CMainWnd

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

  CMainWnd* pControlWnd = new CMainWnd();
	pControlWnd->m_pParent = parent;

	pControlWnd->SetParam((struct _param*)pParam);
	pControlWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, pControlWnd->m_Param.rect, parent, 100);

	return pControlWnd;
}


CMainWnd::CMainWnd()
{
	EnableAutomation();
}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CMainWnd, "Navigate", dispidNavigate, Navigate, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "Navigatesrc", dispidNavigatesrc, Navigatesrc, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "closeEdge", dispidcloseEdge, closeEdge, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {1F53C596-F91C-43E7-A3FA-98E92E2B137D}
static const IID IID_IMainWnd =
{ 0x1f53c596, 0xf91c, 0x43e7, { 0xa3, 0xfa, 0x98, 0xe9, 0x2e, 0x2b, 0x13, 0x7d } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers


int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	InitializeWebView();
	return 0;
}

void CMainWnd::InitializeWebView()
{
	CloseWebView();
	m_dcompDevice = nullptr;

	HRESULT hr2 = DCompositionCreateDevice2(nullptr, IID_PPV_ARGS(&m_dcompDevice));
	if (!SUCCEEDED(hr2))
	{
		AfxMessageBox("Attempting to create WebView using DComp Visual is not supported.\r\n"
			"DComp device creation failed.\r\n"
			"Current OS may not support DComp.\r\n"
			"Create with Windowless DComp Visual Failed", MB_OK);
		return;
	}

#ifdef USE_WEBVIEW2_WIN10
	m_wincompCompositor = nullptr;
#endif
	LPCWSTR subFolder = nullptr;
	auto options = Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>();
	options->put_AllowSingleSignOnUsingOSPrimaryAccount(FALSE);

	HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(subFolder, nullptr, options.Get(), Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(this, &CMainWnd::OnCreateEnvironmentCompleted).Get());
}

HRESULT CMainWnd::OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment)
{
	m_webViewEnvironment = environment;
	m_webViewEnvironment->CreateCoreWebView2Controller(this->GetSafeHwnd(), Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(this, &CMainWnd::OnCreateCoreWebView2ControllerCompleted).Get());

	return S_OK;
}

HRESULT CMainWnd::DCompositionCreateDevice2(IUnknown* renderingDevice, REFIID riid, void** ppv)
{
	HRESULT hr = E_FAIL;
	static decltype(::DCompositionCreateDevice2)* fnCreateDCompDevice2 = nullptr;
	if (fnCreateDCompDevice2 == nullptr)
	{
		HMODULE hmod = ::LoadLibraryEx("dcomp.dll", nullptr, 0);
		if (hmod != nullptr)
		{
			fnCreateDCompDevice2 = reinterpret_cast<decltype(::DCompositionCreateDevice2)*>(
				::GetProcAddress(hmod, "DCompositionCreateDevice2"));
		}
	}
	if (fnCreateDCompDevice2 != nullptr)
	{
		hr = fnCreateDCompDevice2(renderingDevice, riid, ppv);
	}
	return hr;
}

HRESULT CMainWnd::OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller)
{
	if (result == S_OK)
	{
		m_controller = controller;
		wil::com_ptr<ICoreWebView2> coreWebView2;
		m_controller->get_CoreWebView2(&coreWebView2);
		coreWebView2.query_to(&m_webView);

		NewComponent<ViewComponent>(
			this, m_dcompDevice.get(),
#ifdef USE_WEBVIEW2_WIN10
			m_wincompCompositor,
#endif
			m_creationModeId == IDM_CREATION_MODE_TARGET_DCOMP);

	/*	wchar_t strUnicode[256] = { 0, };
		char    strMultibyte[256] = { 0, };
		strcpy_s(strMultibyte, 256, "https://ibks.com");
		int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), strUnicode, nLen);

		HRESULT hresult = m_webView->Navigate(strUnicode);

		if (hresult == S_OK)
		{
			OutputDebugString("[cx_edge]Web Page Opened Successfully");
			ResizeEverything();
		}*/

	}
	else
	{
		TRACE("Failed to create webview");
		AfxMessageBox("Failed to create webview");
	}

	return S_OK;
}

void CMainWnd::CloseWebView(bool cleanupUserDataFolder)
{
	if (m_controller)
	{
		m_controller->Close();
		m_controller = nullptr;
		m_webView = nullptr;
	}
	m_webViewEnvironment = nullptr;
	if (cleanupUserDataFolder)
	{
		//Clean user data        
	}
}

void CMainWnd::ResizeEverything()
{
	RECT availableBounds = { 0 };
	GetClientRect(&availableBounds);

	if (auto view = GetComponent<ViewComponent>())
	{
		view->SetBounds(availableBounds);
	}
}

void CMainWnd::RunAsync(std::function<void()> callback)
{
	auto* task = new std::function<void()>(callback);
	PostMessage(s_runAsyncWindowMessage, reinterpret_cast<WPARAM>(task), 0);
}


void CMainWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	ResizeEverything();
}







void CMainWnd::Navigate(BSTR url)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_strurl.Format("%s", url);
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.

	wchar_t strUnicode[256] = { 0, };
	char    strMultibyte[256] = { 0, };
	strcpy_s(strMultibyte, 256, m_strurl);
	int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), strUnicode, nLen);

	HRESULT hresult = m_webView->Navigate(strUnicode);

	if (hresult == S_OK)
	{
		OutputDebugString("[cx_edge]Web Page Opened Successfully");
		ResizeEverything();
	}
}

void CMainWnd::Navigatesrc(BSTR src)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	
}

void CMainWnd::closeEdge()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	
}

void CMainWnd::SetParam(_param* pParam)
{
	m_Param.key = pParam->key;
	m_Param.name = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB = pParam->tRGB;
	m_Param.pRGB = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	CString	sToken;
	sToken = _T("/wurl");
	int nPos = m_Param.options.Find(sToken, 0);
	if (nPos >= 0)
	{
		CString sTmp = m_Param.options.Mid(nPos + sToken.GetLength());
		nPos = sTmp.Find("/");
		if (nPos < 0)
			_sURL = sTmp;
		else
			_sURL = sTmp.Left(nPos);
	}

	sToken = _T("/s");
	nPos = m_Param.options.Find(sToken, 0);
	if (nPos >= 0)
	{
		_bScroll = true;
	}
	else
		_bScroll = false;
}

















