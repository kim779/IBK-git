// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_WEBEDGE.h"
#include "ControlWnd.h"
#include "AxBrowser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

#include <windows.h>

CControlWnd::CControlWnd()
{
	EnableAutomation();

	m_pHtml = nullptr;
	m_sURL = _T("");
	//m_pApp = (Ccx_THtmlExApp*)AfxGetApp();
	m_bScroll = true;
	m_scrGap = 0;

	m_sos = "0";
	m_sprocess = "0";
}

CControlWnd::~CControlWnd()
{
}

void CControlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_NOTIFY(CControlWnd, "strLog", m_strLog, OnStrLogChanged, VT_BSTR)
	DISP_FUNCTION(CControlWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "Navigate", Navigate, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "Seturl", _Seturl, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SetVisible", SetVisible, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CControlWnd, "Seturl2", Seturl2, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GoBack", GoBack, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "GoForward", GoForward, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "PopUpUrl", PopUpUrl, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "PopUpChrome", PopUpChrome, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {7E741965-4318-4459-A8F9-3823123B9B0E}
static const IID IID_IControlWnd =
{ 0x7e741965, 0x4318, 0x4459, { 0xa8, 0xf9, 0x38, 0x23, 0x12, 0x3b, 0x9b, 0xe } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

BSTR CControlWnd::GetProperties() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here

	return strResult.AllocSysString();
}

void CControlWnd::SetProperties(LPCTSTR sParam) 
{
	// TODO: Add your dispatch handler code here

}

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (m_bScroll)
	{
		m_Htmlrc.SetRect(-2, -2, m_Param.rect.Width()+2, m_Param.rect.Height()+2);
		m_pHtml = new CAxBrowser;
		m_pHtml->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, m_Htmlrc, this, 0);
	}
	else
	{
		std::unique_ptr<CWnd> pRefView = std::make_unique<CWnd>();
		pRefView->Create(NULL, NULL, WS_CHILD|WS_VSCROLL, CRect(0, 0, 50, 50), this, 0);

		CRect	crc;
		pRefView->GetClientRect(&crc);
		pRefView.reset();

		m_scrGap = 50 - crc.Width();
		
		m_Htmlrc.SetRect(-2, -2, m_Param.rect.Width()+2+m_scrGap, m_Param.rect.Height()+2+m_scrGap);
		m_pHtml = new CAxBrowser;
		m_pHtml->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, m_Htmlrc, this, 0);
		m_pHtml->SetParentView(m_pParent);
		m_pHtml->m_scrGap = m_scrGap;
	}
	if (IsDev())
	{
		m_sURL.Replace("www.ibks.com","test.ibks.com");
	}
	if (!m_sURL.IsEmpty())	m_pHtml->Navigate2(m_sURL);
	//SetTimer(1,10,NULL);
	CheckOS();
	return 0;
}

void CControlWnd::OnDestroy() 
{
	CWnd::OnDestroy();
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (!m_pHtml)
		return;

	CRect	rc;
	GetWindowRect(&rc);

	m_pHtml->SetWindowPos(NULL, -2, -2, rc.Width()+2+m_scrGap, rc.Height()+2+m_scrGap, NULL);
	Invalidate();
}

void CControlWnd::SetParam(_param *pParam)
{
	m_Param.key  = pParam->key;
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
			m_sURL = sTmp;
		else
			m_sURL = sTmp.Left(nPos);
	}

	sToken = _T("/d");
	nPos = m_Param.options.Find(sToken, 0);
	if (nPos >= 0)
	{
		CString sTmp = m_Param.options.Mid(nPos + sToken.GetLength());
		if(sTmp == "1")
			m_bScroll = true;
		else
			m_bScroll = false;
	}
	else
		m_bScroll = false;

}

void CControlWnd::Navigate(LPCTSTR url)
{
	m_sURL.Format("%s", url);

	if (m_pHtml)
	{
		m_pHtml->Navigate2(m_sURL);
	}

}

void CControlWnd::_Seturl(LPCTSTR url) 
{
	int nPos;
	CString sTmp = url;
	
	nPos = sTmp.Find("/");
	if (nPos < 0)
		m_sURL = "http:\\\\" + sTmp;
	else
		m_sURL = "http:\\\\" + sTmp.Left(nPos);

	if (m_pHtml)
	{
		m_pHtml->Navigate2(m_sURL);
	}
}



void CControlWnd::SetVisible(BOOL bVisible) 
{
	// TODO: Add your dispatch handler code here
	if (bVisible)
	{
		ShowWindow(SW_SHOW);
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
	
}


LONG CControlWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	//switch (LOBYTE(LOWORD(wParam)))
	//{
	//case DLL_SETPAL:
	//	//m_fRGB = GetPalette(BGIndex);
	//	//Invalidate();
	//	m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange/*Change*/)), 
	//		(LPARAM)m_Param.name.GetString());
	//	break;
	//}
	return 0;
}

BOOL CControlWnd::IsDev()
{
	CString title;
	CWinApp* app = AfxGetApp();
	CWnd*	wnd = nullptr;

	app->GetMainWnd()->GetWindowText(title);
	if (title == ("IBK hot Trading"))
		wnd = FindWindow(NULL,"IBK hot Trading");
	else if (title == "[직원]IBK hot Trading")
	{
		wnd = FindWindow(NULL,"[직원]IBK hot Trading");
	}
	else if (title == "IBK개발")
		wnd = FindWindow(NULL,"IBK개발");
	else if (title == "[직원]IBK개발")
		wnd = FindWindow(NULL,"[직원]IBK개발");
	if (!wnd)  //SDI인경우 메인폼 접근이 안되서 편법이용
	{
		wnd = FindWindow(NULL,"IBK hot Trading");
		if (!wnd) wnd = FindWindow(NULL,"[직원]IBK hot Trading");
		if (!wnd) wnd = FindWindow(NULL,"IBK개발");		
		if (!wnd) wnd = FindWindow(NULL,"[직원]IBK개발");	
	}

	if (wnd)
		wnd->GetWindowText(title);

	if (title.Find("개발")!=-1)
		return true;

	return false;
}

void CControlWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(nIDEvent);
	if (!m_sURL.IsEmpty())	m_pHtml->Navigate2(m_sURL);
	CWnd::OnTimer(nIDEvent);
}

void CControlWnd::Seturl2(LPCTSTR url) 
{
	if (m_pHtml)
	{
		m_pHtml->SetSilent(TRUE);

		m_pHtml->Navigate2(url);
	}
}


void CControlWnd::GoBack() 
{
	// TODO: Add your dispatch handler code here
	if (m_pHtml)
	{
		m_pHtml->GoBack();
	}
}


void CControlWnd::SetBrowserReg()
{
	
}

void CControlWnd::OnStrLogChanged() 
{
	// TODO: Add notification handler code

}

#include <stdio.h>

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process;

void CControlWnd::CheckOS()
{
	BOOL	bIsWow64 = false;
	HMODULE	hMod = GetModuleHandle("kernel32");

	if (hMod)
		fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(hMod, "IsWow64Process");
	else
		fnIsWow64Process = NULL;
	
	if (fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			m_sprocess = "unknown";
		} 
	}
	

	m_sprocess = "32process";


bool	bProcess64 = false;
#if defined(_WIN64)   //64프로세스 정의되있는 경우 os는 64 (64os 에서는 64프로세스만 돌아간다.)
	bProcess64 = true;
	m_sos = "64os"; 
#elif defined(_WIN32) //32프로세스 정의인 경우 os 는 64 혹은 32 일수 있다
	BOOL f64 = FALSE;
	if (fnIsWow64Process)
		bProcess64 = fnIsWow64Process(GetCurrentProcess(), &f64) && f64;
#endif

	if (bProcess64)
		m_sos = "64os";
	else
		m_sos = "32os";
}

void CControlWnd::GoForward() 
{
	// TODO: Add your dispatch handler code here
	if (m_pHtml)
	{
		m_pHtml->GoForward();
	}
}

void CControlWnd::PopUpUrl(LPCTSTR url) 
{
	// TODO: Add your dispatch handler code here
	CString sUrl;
	sUrl.Format("%s", url);
	ShellExecute(NULL, _T("open"), sUrl, NULL, NULL, SW_SHOWNORMAL);
}


void CControlWnd::PopUpChrome(LPCTSTR url) 
{
	// TODO: Add your dispatch handler code here
	CString sUrl;
	sUrl.Format("%s", url);
	//ShellExecute(NULL, _T("open"), sUrl, NULL, NULL, SW_SHOWNORMAL);
	ShellExecute(NULL, "open", "C:\\Chrome\\GoogleChromePortable.exe", sUrl, NULL, SW_SHOW|SW_SHOWMAXIMIZED);
}
