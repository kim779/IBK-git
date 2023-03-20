#if !defined(AFX_MAINWND_H__79F491B7_F6A1_4445_82CA_E672611EF4C1__INCLUDED_)
#define AFX_MAINWND_H__79F491B7_F6A1_4445_82CA_E672611EF4C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainWnd.h : header file
//

#include "ComponentBase.h"
#include <dcomp.h>
#include <functional>
#include <memory>
#include <ole2.h>
#include <string>
#include <vector>
#include <winnt.h>

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
class CParam
{
public:
	int		key = 0;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point = 0;		// font point
	int		style = 0;		// font style
	DWORD		tRGB = 0;			// text color index
	DWORD		pRGB = 0;			// paint color index
	CString		options;		// properties
};

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd();
	virtual ~CMainWnd();
	void	SetParam(struct _param* pParam);

	void InitializeWebView();
	HRESULT OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment);
	HRESULT OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller);
	void RunAsync(std::function<void(void)> callback);
	void ResizeEverything();
	HRESULT DCompositionCreateDevice2(IUnknown* renderingDevice, REFIID riid, void** ppv);

	void CloseWebView(bool cleanupUserDataFolder = false);

	ICoreWebView2Controller* GetWebViewController()
	{
		return m_controller.get();
	}
	ICoreWebView2* GetWebView()
	{
		return m_webView.get();
	}
	ICoreWebView2Environment* GetWebViewEnvironment()
	{
		return m_webViewEnvironment.get();
	}
	HWND GetMainWindow()
	{
		return this->GetSafeHwnd();
	}
protected:
	HICON m_hIcon;
	DWORD m_creationModeId = 0;
	wil::com_ptr<ICoreWebView2Environment> m_webViewEnvironment;
	wil::com_ptr<IDCompositionDevice> m_dcompDevice;
	wil::com_ptr<ICoreWebView2Controller> m_controller;
	wil::com_ptr<ICoreWebView2> m_webView;
	std::vector<std::unique_ptr<ComponentBase>> m_components;
	HWND m_mainWindow = nullptr;
	HINSTANCE g_hInstance;
	static constexpr size_t s_maxLoadString = 100;
	template <class ComponentType, class... Args> void NewComponent(Args&&... args);

	template <class ComponentType> ComponentType* GetComponent();
// Attributes
public:

// Operations
public:
	CWnd* m_pParent{};
	CParam m_Param;
	CString m_strurl;

private:
	CString _sURL;
	bool	_bScroll{};

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	enum
	{
		dispidcloseEdge = 3L,
		dispidNavigatesrc = 2L,
		dispidNavigate = 1L,
	};

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:

	void Navigate(BSTR url);
	void Navigatesrc(BSTR src);
	void closeEdge();
};

template <class ComponentType, class... Args> void CMainWnd::NewComponent(Args&&... args)
{
	m_components.emplace_back(new ComponentType(std::forward<Args>(args)...));
}

template <class ComponentType> ComponentType* CMainWnd::GetComponent()
{
	for (auto& component : m_components)
	{
		if (auto wanted = dynamic_cast<ComponentType*>(component.get()))
		{
			return wanted;
		}
	}
	return nullptr;
}
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINWND_H__79F491B7_F6A1_4445_82CA_E672611EF4C1__INCLUDED_)
