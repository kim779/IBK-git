// IB700000.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "IB700000.h"

#include "../../h/axisvar.h"
#include "../../h/axisfire.h"
#include "../../h/axisgwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIB700000App

BEGIN_MESSAGE_MAP(CIB700000App, CWinApp)
	//{{AFX_MSG_MAP(CIB700000App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIB700000App construction

CIB700000App::CIB700000App()
{
	m_hView = NULL;
}

CIB700000App::~CIB700000App()
{
	if (m_hView)
		FreeLibrary(m_hView);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIB700000App object

CIB700000App theApp;

#include "CustomChartInfo.h"

CWnd* CIB700000App::Create(CWnd* pMain)
{
	int key = pMain->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC));
	LPCTSTR mapName = (LPCTSTR)pMain->SendMessage(WM_USER, MAKEWPARAM(mapDLL, key));

	CAxChartInfo* chartInfo = CreateChartInfo(pMain, mapName);

	if (chartInfo) 
	{
		chartInfo->Initialize();
		delete chartInfo;
	}
	else 
	{
		return NULL;
	}

	m_hView = LoadLibrary("axisGView.dll");
	if (!m_hView)
	{
		TRACE("axisGView.dll load error !!\n");
		return NULL;
	}

	CWnd* (APIENTRY *axCreate)(CWnd *, char*);
	axCreate = (CWnd* (APIENTRY *)(CWnd *, char*)) GetProcAddress(m_hView, "axCreate");
	if (axCreate == NULL)
	{
		FreeLibrary(m_hView);
		TRACE("axisGView.dll axCreate error !!\n");
		return NULL;
	}

	return axCreate(pMain, (LPSTR)(LPCTSTR)mapName);
}
