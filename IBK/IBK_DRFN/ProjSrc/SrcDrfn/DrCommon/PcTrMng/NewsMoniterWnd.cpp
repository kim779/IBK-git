// NewsMoniterWnd.cpp : implementation file
//

#include "stdafx.h"
#include "NewsMoniterWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//# Open / Close! => �ݵ�� Open �� Colse!
//extern "C" __declspec(dllexport) int _DLL_SetLibOpen(CWnd* pwndMain, HINSTANCE hInstance);
//extern "C" __declspec(dllexport) void _DLL_SetLibClose(void);

//# Signal Rev Reg! ���/����(ȭ�麰)
//BOOL bAdvise TRUE:��û, FALSE:����
//extern "C" __declspec(dllexport) int _DLL_SetSiganl(BOOL bAdvise, CWnd* pwndMain, CString strType);

typedef int  (* DLL_SetLibOpen)(CWnd* pwndMain, HINSTANCE hInstance);
typedef void  (* DLL_SetLibClose)(void);
typedef int  (* DLL_SetSiganl)(BOOL bAdvise, CWnd* pwndMain, CString strType, CString strKey);
typedef int  (* DLL_GetSiganlTable)(CString strType, CStringArray &saSignalTable );

typedef void (*DLL_GetSignalData)(CString strType, CMapStringToPtr &mapSignalSet);
typedef void (*DLL_SetSignalData)(CString strType, CMapStringToPtr *pmapSignalSet);

typedef void (*DLL_GetFilterData)(long &stFilterInfo);
typedef void (*DLL_SetFilterData)(long *pstFilterInfo);
typedef HWND (*DLL_GetDummyHwnd)();

/////////////////////////////////////////////////////////////////////////////
// CNewsMoniterWnd

CNewsMoniterWnd::CNewsMoniterWnd()
{
}

CNewsMoniterWnd::~CNewsMoniterWnd()
{
}


BEGIN_MESSAGE_MAP(CNewsMoniterWnd, CWnd)
	//{{AFX_MSG_MAP(CNewsMoniterWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewsMoniterWnd message handlers

BOOL CNewsMoniterWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CWnd::PreCreateWindow(cs);
}

int CNewsMoniterWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CString sDllFile; 
	sDllFile.Format("%s/%s", m_szRootPath, "Lib_SignalMng.dll");
	CFileFind fileFind;
	if( !fileFind.FindFile( sDllFile ) )
	{
		//MessageBox("���� ���ÿ� Lib_SignalMng.dll' �� ������ ���� ���� �ʽ��ϴ�.");
		return 0;
	}

	m_hDll = LoadLibrary(sDllFile);		// ��������� ���� DLL
	if(m_hDll)
	{
	//	AfxMessageBox("Lib_SignalMng.dll Loaded");
		DLL_SetLibOpen	lpSetLibOpen = (DLL_SetLibOpen)GetProcAddress(m_hDll,"_DLL_SetLibOpen");

		if( lpSetLibOpen )
		{
	//		AfxMessageBox("Found _DLL_SetLibOpen and Call SetLibOpen");
			lpSetLibOpen(this, AfxGetInstanceHandle());
		}
		else
		{
	//		MessageBox("'Lib_SignalMng.dll�� _DLL_SetLibOpen()�Լ��� �ջ� �Ǿ����ϴ�.");
			return 0;
		}
	}else
	{
	//	MessageBox("Lib_SignalMng �� ������ �ε� �ϴµ� ���� �Ͽ����ϴ�.");
		return 0;
	}

	return 0;
}

void CNewsMoniterWnd::OnDestroy() 
{
	// ���� ����� ���� 
	if( m_hDll != NULL )
	{
		DLL_SetLibClose	lpSetLibClose = 
				(DLL_SetLibClose)GetProcAddress(m_hDll,"_DLL_SetLibClose");

		if( lpSetLibClose )
		{
			lpSetLibClose();
		}

		::FreeLibrary( m_hDll );
	}

	CWnd::OnDestroy();
}

//*******************************************************************/
// Function Name : SetSingSiganl
// Function      : �ñ׳��� ��û �� ���� �Ѵ�
// Param         : BOOL bAdvise:	TRUE:��û , FALSE:���� 
//                 CWnd* pwndMain:
//                 CString strType: 
// Return        : int 
// Create        : BT.Jung , 2006/10/31
// Comment       : 
//******************************************************************/
int CNewsMoniterWnd::SetSingSiganl(BOOL bAdvise, CWnd* pwndMain, CString strType, CString strKey)
{
	if( m_hDll == NULL ) return 0;

	int nRet = 0;
	DLL_SetSiganl	lpSetSiganl = 
			(DLL_SetSiganl)GetProcAddress(m_hDll,"_DLL_SetSiganl");

	if( lpSetSiganl )
	{
		nRet = lpSetSiganl(bAdvise, pwndMain, strType, strKey );
	}

	return nRet;
}

void CNewsMoniterWnd::GetSignalData(CString strType, CMapStringToPtr &mapSignalSet)
{
	if( m_hDll == NULL ) return;

	DLL_GetSignalData	lpGetSignalData = 
			(DLL_GetSignalData)GetProcAddress(m_hDll,"_DLL_GetSignalData");

	if( lpGetSignalData )
	{
		lpGetSignalData(strType, mapSignalSet );
	}

	return;
}


void CNewsMoniterWnd::SetSignalData(CString strType, CMapStringToPtr *pmapSignalSet)
{
	if( m_hDll == NULL ) return;

	DLL_SetSignalData	lpSetSignalData = 
			(DLL_SetSignalData)GetProcAddress(m_hDll,"_DLL_SetSignalData");

	if( lpSetSignalData )
	{
		lpSetSignalData(strType, pmapSignalSet );
	}

	return ;
}

void CNewsMoniterWnd::GetFilterData(long &stFilterInfo)
{
	if( m_hDll == NULL ) return;

	DLL_GetFilterData	lpGetFilterData = 
			(DLL_GetFilterData)GetProcAddress(m_hDll,"_DLL_GetFilterData");

	if( lpGetFilterData )
	{
		lpGetFilterData(stFilterInfo );
	}

	return;
}

void CNewsMoniterWnd::SetFilterData(long *pstFilterInfo)
{
	if( m_hDll == NULL ) return;

	DLL_SetFilterData	lpSetFilterData = 
			(DLL_SetFilterData)GetProcAddress(m_hDll,"_DLL_SetFilterData");

	if( lpSetFilterData )
	{
		lpSetFilterData(pstFilterInfo );
	}

	return ;
}


int  CNewsMoniterWnd::GetSiganlTable(CString strType, CStringArray &saSignalTable)
{
	if( m_hDll == NULL ) return 0;

	DLL_GetSiganlTable	lpGetSiganlTable = 
			(DLL_GetSiganlTable)GetProcAddress(m_hDll,"_DLL_GetSiganlTable");

	ASSERT ( lpGetSiganlTable );
	if( lpGetSiganlTable )
	{
		return lpGetSiganlTable(strType, saSignalTable );
	}
	return 0;
}

BOOL CNewsMoniterWnd::OpenWindow()
{
	CString		szClassName;
	szClassName =	AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		0, 0, 0);
	
	if (!CWnd::CreateEx(WS_EX_TOPMOST, szClassName, _T(""), 
	WS_POPUP, 
	0, 0, 0, 0, // size & position updated when needed
	NULL, 0, NULL))
	{
		return FALSE;
	}
	return TRUE;
}

HWND CNewsMoniterWnd::GetDummyHwnd()
{
	DLL_GetDummyHwnd	lpDllFn = 
		(DLL_GetDummyHwnd)GetProcAddress(m_hDll,"_DLL_GetDummyHwnd");

	if(lpDllFn)
		return lpDllFn();

	return NULL;
}