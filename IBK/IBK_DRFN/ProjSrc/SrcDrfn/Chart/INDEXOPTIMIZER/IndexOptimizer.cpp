// IndexOptimizer.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "DlgOptimizer.h"
#include "../../inc/ISysTabExChartItem.h"
#include "../../inc/ISTManager.h" // �ý���Ʈ���̵� ���õ� ������ �������̽�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE IndexOptimizerDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("INDEXOPTIMIZER.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(IndexOptimizerDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(IndexOptimizerDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("INDEXOPTIMIZER.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(IndexOptimizerDLL);
	}
	return 1;   // ok
}
// �Լ� ����          : �����Ǳ����� �ʿ��� ������ �ѱ��.
///////////////////////////////////////////////////////////////////////////////
HRESULT WINAPI fnInit(STODINIT_IN* pIn, STODINIT_OUT* pOut, void* pInterface,int iTradingID)
{
	CDlgOptimizer* pDlgOptimizer = new CDlgOptimizer;
	pOut->m_pDllKey = pDlgOptimizer;

	if(pInterface)
	{
		ISTInterface* pSTInterface = (ISTInterface*)pInterface;
		pDlgOptimizer->m_pChartInterface = (long)pInterface;// KHD : Interface ����
		ISTAnalysisInput* pMng = (ISTAnalysisInput*)pSTInterface->GetInterface(UUSTID_ISTAnalysisInput);
		
		if(iTradingID == TIME_OPTIMIZER)
			pMng->SetUserData(TRUE, &pDlgOptimizer->m_xSTUserData);
		else
			pMng->SetUserData(FALSE, NULL);

		pDlgOptimizer->m_stEndDate	    =pMng->GetEndDay();
		pDlgOptimizer->m_stCodeName		=pMng->GetCodeName();
		pDlgOptimizer->m_nCodeType		=pMng->GetCodeType();
		pDlgOptimizer->m_nCount			=pMng->GetCount();
		pDlgOptimizer->m_stCode			=pMng->GetCode();
		pDlgOptimizer->m_nTimeState		=pMng->GetDivideState();//KHD : �߰�
	}
	return S_OK;
}

// �Լ� ����          : ������ �ʿ��� ������ �� �ѱ� ���¿��� Create�� �ش��ϴ� ��ƾó��
/////////////////////////////////////////////////////////////////////////////////extern "C" _declspec(dllexport) HRESULT FPLoad(SZSTODCREATEINFO_IN* pIn, SZSTODCREATEINFO_OUT* pOut)
HRESULT WINAPI fnLoad(STODCREATEINFO_IN* pIn, STODCREATEINFO_OUT* pOut)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle(IndexOptimizerDLL.hResource);
	AfxInitRichEdit();
	CDlgOptimizer* pDlgOptimizer = (CDlgOptimizer*)pIn->m_pInitOut->m_pDllKey;
	// >-------------------------------------------------------
	// �� ��ƾ���� �����ΰ� �ʿ��� ���� �����ϴ� ó���� �Ѵ�.
	// <-------------------------------------------------------
	if(pDlgOptimizer)
	{
		//Optimizer Flag
		pDlgOptimizer->m_nOptimizerDiv = pIn->m_nID;
//@�����߰�
		// Root Path
		pDlgOptimizer->m_strRootPath = (LPCSTR)AfxGetPctrData(10);	//������[����_PC�����͸Ŵ���.xls]
		
		// Data Path
		pDlgOptimizer->m_strDataDir = (LPCSTR)AfxGetPctrData(12);	//������[����_PC�����͸Ŵ���.xls]

		// Image Path
//		pDlgOptimizer->m_strImageDir = (LPCSTR)AfxGetPctrData(14);
		//@Solomon pDlgOptimizer->m_strImageDir = pDlgOptimizer->m_strRootPath + "\\Image";
		pDlgOptimizer->m_strImageDir = pDlgOptimizer->m_strRootPath + "\\Icss\\Image";
//@�����߰�
//@��������
//		// Root Path
//		GET_ROOT_DIR(pDlgOptimizer->m_strRootPath);
//		if(pDlgOptimizer->m_strRootPath.GetLength()==0)
//		{
//			AfxMessageBox("RootPath�� ���� �� �����ϴ�.");
//		}
//
//		// Data Path
//	
//		GET_DATA_DIR(pDlgOptimizer->m_strDataDir);
//		if(pDlgOptimizer->m_strDataDir.GetLength()==0)
//		{
//			AfxMessageBox("DataPath�� ���� �� �����ϴ�.");
//		}
//
//		// Image Path
//		GET_IMAGE_DIR(pDlgOptimizer->m_strImageDir);
//		if(pDlgOptimizer->m_strImageDir.GetLength()==0)
//		{
//			AfxMessageBox("ImagePath�� ���� �� �����ϴ�.");
//		}
//
//		// User Directory ��� : KHD : ��� ã�� ���� �κ� ..
//		//pDlgOptimizer->m_strUserDir = pDlgOptimizer->m_strRootPath + "\\User\\" + pDlgOptimizer->m_strUserID;
//@��������

		// ���̾Ʒα� ����
		pDlgOptimizer->Create(CDlgOptimizer::IDD, CWnd::FromHandle(pIn->m_hParent));
		pDlgOptimizer->FormInit();					// socket, drds ���� �ʱ�ȭ
		pDlgOptimizer->FormLoad();					// �ʱ�ȭ �Ŀ� ������ �۾� ó��
		pDlgOptimizer->ShowWindow(SW_SHOW);

	}
	AfxSetResourceHandle(hInst);
	return S_OK;
}

// * ����
HRESULT WINAPI fnUnLoad(void* dwDllKey)
{
	CDlgOptimizer* pDlgOptimizer = (CDlgOptimizer*)dwDllKey;
	if(pDlgOptimizer)
	{
		pDlgOptimizer->DestroyWindow();
		
		delete pDlgOptimizer;
		pDlgOptimizer = NULL;
	}
	return S_OK;
}

HRESULT WINAPI fnFuncCall(void* dwDllKey, LPCSTR szFuncName, LPCSTR szParam, VARIANT& result)
{
	CDlgOptimizer* pDlgOptimizer = (CDlgOptimizer*)dwDllKey;
	if(pDlgOptimizer)
	{
		CString strFuncName;
		strFuncName = szFuncName;

		if(strFuncName == "SetFrameInfo")
		{
		}
	}
	
	return S_OK;

}

//CWnd* WINAPI CreateTradingAup(int iTradingID, CWnd* pParent)
extern "C" __declspec(dllexport) CWnd* CREATE_TRADING(int iTradingID, CWnd* pParent)
{
	STODINIT_IN  InitIn;
	STODINIT_OUT InitOut;

	INIT_STODINIT_IN((&InitIn));
	INIT_STODINIT_OUT2(InitOut);

	HRESULT hr = fnInit(&InitIn, &InitOut,NULL,iTradingID);

	STODCREATEINFO_IN  InData;
	STODCREATEINFO_OUT OutData;

	INIT_STODCREATEINFO_IN((&InData));
	InData.m_pInitOut = &InitOut;
	InData.m_hParent = pParent->GetSafeHwnd();
	InData.m_nID = iTradingID;//1001:��ǥ����ȭ,1002:��������ȭ,1003:�ð�����ȭ
	
	InData.m_Rect = CRect(0,0,300,400);

	hr = fnLoad(&InData, &OutData);
	CWnd* pWnd = (CWnd*)InData.m_pInitOut->m_pDllKey;
	return pWnd;
}
extern "C" __declspec(dllexport) CWnd* CREATE_TRADING_EX(int iTradingID, CWnd* pParent, void* pInterface)
{
	STODINIT_IN  InitIn;
	STODINIT_OUT InitOut;

	INIT_STODINIT_IN((&InitIn));
	INIT_STODINIT_OUT2(InitOut);

	HRESULT hr = fnInit(&InitIn, &InitOut, pInterface,iTradingID);

	if(InitOut.m_pDllKey==NULL) return NULL;
	STODCREATEINFO_IN  InData;
	STODCREATEINFO_OUT OutData;

	INIT_STODCREATEINFO_IN((&InData));
	InData.m_pInitOut = &InitOut;
	InData.m_hParent = pParent->GetSafeHwnd();
	InData.m_nID = iTradingID;//1001:��ǥ����ȭ,1002:��������ȭ,1003:�ð�����ȭ
	InData.m_Rect = CRect(0,0,300,400);

	hr = fnLoad(&InData, &OutData);
	CWnd* pWnd = (CWnd*)InData.m_pInitOut->m_pDllKey;
	return pWnd;
}

//extern "C" __declspec(dllexport) void DestroyTradingAup(CWnd *pWnd)
extern "C" __declspec(dllexport) void DESTROY_TRADING(CWnd *pWnd)
{
	if ( pWnd )
	{
		fnUnLoad(pWnd);
	}
}

