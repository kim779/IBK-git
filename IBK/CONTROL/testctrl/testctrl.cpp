// testctrl.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "testctrl.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CTestctrlApp

BEGIN_MESSAGE_MAP(CTestctrlApp, CWinApp)
	//{{AFX_MSG_MAP(CTestctrlApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestctrlApp construction

CTestctrlApp::CTestctrlApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_hDlg = nullptr;
	m_pView = nullptr;
	m_pImgCtrl = nullptr;
}

CTestctrlApp::~CTestctrlApp()
{
	if (m_pImgCtrl)
	{
		m_pImgCtrl->DeleteImageList();
	}
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTestctrlApp object

CTestctrlApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTestctrlApp initialization

BOOL CTestctrlApp::InitInstance()
{
	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleObjectFactory::RegisterAll();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Special entry points required for inproc servers

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}

// by exporting DllRegisterServer, you can use regsvr.exe
STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	COleObjectFactory::UpdateRegistryAll();
	return S_OK;
}

#include "MainWnd.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* pParent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	auto pMainWnd = std::make_unique<CMainWnd>(pParent, pParam);
	pMainWnd->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, ((_param*)pParam)->rect, pParent, 100);
	return pMainWnd.release();
	//CDlg dlg;
	//dlg.DoModal();
	//CMainWnd	*pMainWnd = new CMainWnd(pParent, pParam);
	//
	//pMainWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, pParam->rect, pParent, 100);
	//return pMainWnd;
	//return nullptr;
}

__declspec(dllexport) char* WINAPI axPropDlg(char* pStr, char* pMtblPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	static CString retVal;
	CTestctrlApp* pApp = (CTestctrlApp*)AfxGetApp();

	if (!pApp->m_hDlg)
		pApp->m_hDlg = LoadLibrary("C_UserGrpDlg.dll");

	if (pApp->m_hDlg)
	{
		char* (APIENTRY * axPropDlg)(char*, char*);

		axPropDlg = (char* (APIENTRY*)(char*, char*))GetProcAddress(pApp->m_hDlg, "axPropDlg");
		if (axPropDlg)
			retVal = axPropDlg(pStr, pMtblPath);
	}

	return (char*)retVal.GetString();
}

__declspec(dllexport) CWnd* WINAPI axCreateEx(CWnd* pParent, _param* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CMainWnd	*pMainWnd = new CMainWnd(pParent, pParam);
	
	if (!pMainWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, pParam->rect, pParent, 100))
	{
		delete pMainWnd;
		return NULL;
	}	

	return pMainWnd;
}

char* CTestctrlApp::GetSampleData(int pInfo1, int pInfo2, int* pLen)
{
	if (!m_hDlg)
		m_hDlg = LoadLibrary("C_UserGrpDlg.dll");

	if (m_hDlg)
	{
		char* (APIENTRY * axGetSample)(int, int, int*);
		axGetSample = (char* (APIENTRY*)(int, int, int*))
			GetProcAddress(m_hDlg, "axGetSample");

		if (axGetSample)
			return axGetSample(pInfo1, pInfo2, pLen);
	}

	return nullptr;
}

bool CTestctrlApp::CreateImage()
{
	if (m_pImgCtrl)	return true;

	if (!m_pView)	return false;

	CString	path;
	CString	root = (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0);
	path.Format("%s\\%s\\GTC_CTRL.bmp", root, IMAGEDIR);

	m_pImgCtrl = getImage(path, 16, 16, ILC_COLOR24 | ILC_MASK, RGB(255, 0, 255));
	if (m_pImgCtrl)	return true;

	return false;
}

std::shared_ptr<CImageList> CTestctrlApp::getImage(CString path, int Width, int Height, UINT flag, COLORREF crMask)
{
	HBITMAP hbitmap = (HBITMAP)LoadImage(nullptr, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hbitmap)
	{
		CBitmap	bitmap;

		bitmap.DeleteObject();
		bitmap.Attach(hbitmap);
		auto pImage = std::make_shared<CImageList>();
		pImage->Create(Width, Height, flag, 0, 1);
		pImage->Add(&bitmap, crMask);
		bitmap.DeleteObject();
		return pImage;

	}
	return nullptr;
}

CFont* CTestctrlApp::GetFont(CWnd* pView, int point, CString name, int bold, bool italic)
{
	_fontR rFont;
	rFont.name = (char*)name.operator LPCTSTR();
	rFont.point = point;
	rFont.italic = italic;
	rFont.bold = bold;
	return (CFont*)pView->SendMessage(WM_USER, getFONT, long(&rFont));
}

CPen* CTestctrlApp::GetPen(CWnd* pView, int style, int width, COLORREF clr)
{
	_penR rPen;
	rPen.clr = clr;
	rPen.width = width;
	rPen.style = style;
	return (CPen*)pView->SendMessage(WM_USER, getPEN, long(&rPen));
}

CBrush* CTestctrlApp::GetBrush(CWnd* pView, COLORREF rColor)
{
	return (CBrush*)pView->SendMessage(WM_USER, getBRUSH, rColor);
}

COLORREF CTestctrlApp::GetColor(CWnd* pView, int index)
{
	if (index & 0x02000000)
		return index;

	return pView->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)index);
}

int CTestctrlApp::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_hDlg)	FreeLibrary(m_hDlg);

	return CWinApp::ExitInstance();
}
