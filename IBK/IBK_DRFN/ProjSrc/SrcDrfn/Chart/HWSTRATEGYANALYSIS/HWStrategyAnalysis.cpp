// HWStrategyAnalysis.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "DlgAnalysis.h"
#include "../../inc/ISTManager.h" // �ý���Ʈ���̵� ���õ� ������ �������̽�
#include "../Common_ST/DataItem.h"
#include "../Common_ST/SDataItem.h"
#include "../../inc/ISysTabExChartItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE HWStrategyAnalysisDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("HWSTRATEGYANALYSIS.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(HWStrategyAnalysisDLL, hInstance))
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

		new CDynLinkLibrary(HWStrategyAnalysisDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("HWSTRATEGYANALYSIS.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(HWStrategyAnalysisDLL);
	}
	return 1;   // ok
}


///////////////////////////////////////////////////////////////////////////////
// ������             : ���ؿ�(Jun-Ok, Lee)
// Email              : alzioyes@hotmail.com
// ���� ��¥          : 2003/2/19
// �Լ� �̸�          : WINAPI
// ���� Ÿ��          : HRESULT
// ���� Ÿ�� ����     : 
// �Ķ����
//		STODINIT_IN* pIn  : 
//		STODINIT_OUT* pOut   : 
// �Լ� ����          : �����Ǳ����� �ʿ��� ������ �ѱ��.
// 060721 void* pInterface �߰�.
///////////////////////////////////////////////////////////////////////////////
HRESULT WINAPI fnInit(STODINIT_IN* pIn, STODINIT_OUT* pOut, void* pInterface)
{
// (2005/9/23 - Seung-Won, Bae) Load this Library to Protect Unload of Application Ending. (for Memory Leak Report)
#ifdef _DEBUG
	LoadLibrary( "HWStrategyAnalysis.dll");
#endif

	CDlgAnalysis* pDlgAnalysis = new CDlgAnalysis;
	pOut->m_pDllKey = pDlgAnalysis;

	// �������̽��� �̿��� ����
//#ifdef _DEBUG
	if(pInterface)
	{
		ISTInterface* pSTInterface = (ISTInterface*)pInterface;
		pDlgAnalysis->m_pChartInterface = (long)pInterface;// KHD : Interface ����
		ISTAnalysisInput* pMng = (ISTAnalysisInput*)pSTInterface->GetInterface(UUSTID_ISTAnalysisInput);
		//�Է� ���� ���� : KHD 2006.07.27
		pDlgAnalysis->m_pCtlList		=(CXListCtrl*)pMng->GetControl(1);//����Ʈ ��Ʈ�� ������ ������.
		pMng->SetUserData(FALSE, NULL);
		pDlgAnalysis->m_bGraphDivide = FALSE;//���� �׷����� ������.. �����м��� �����..
		pDlgAnalysis->m_nCodeType		=pMng->GetCodeType();
		pDlgAnalysis->m_stCode			=pMng->GetCode();
		pDlgAnalysis->m_stCodeName		=pMng->GetCodeName();
		pDlgAnalysis->m_nCount			=pMng->GetCount();
		pDlgAnalysis->m_nDivideCnt		=pMng->GetDivideCnt();
		pDlgAnalysis->m_DivideState		=pMng->GetDivideState();
		pMng->GetUnitPos();
		pDlgAnalysis->m_dfDivideRate	=pMng->GetDeivideRate();
		pDlgAnalysis->m_pChartItem      = (ISysTabExChartItem*)pMng->GetControl(0);//Chart��Ʈ���� �����´�.
		pDlgAnalysis->m_hTabChartEx		= pMng->GetControl(2);//ExTab�� �����´�. : KHD
		pDlgAnalysis->m_nStartType = 0;//�����м�����
		STMNG_ITEMINFO Info;
		pMng->GetSTInfo(&Info);
		pDlgAnalysis->m_stSTPath		=Info.szSTPath +"\\"+ Info.szSTName + ".st";//����ST���� ���
		pDlgAnalysis->m_stSTName		= Info.szSTName + ".ST";
		pDlgAnalysis->m_stEndDate	    =pMng->GetEndDay();

		if(pDlgAnalysis->m_stSTName ==""||pDlgAnalysis->m_stCodeName=="")
		{
			AfxMessageBox("������ �������� �ʾҽ��ϴ�. ���� �����м��� �� �� �����ϴ�.");
			pDlgAnalysis->DestroyWindow();
			delete pDlgAnalysis;
			pDlgAnalysis = NULL;
			pOut->m_pDllKey = NULL;
			return S_FALSE;
		}
		//END	

	}
//#endif
	// ~�������̽��� �̿��� ����
	return S_OK;
}


///////////////////////////////////////////////////////////////////////////////
// ������             : ���ؿ�(Jun-Ok, Lee)
// Email              : alzioyes@hotmail.com
// ���� ��¥          : 2003/2/19
// �Լ� �̸�          : WINAPI
// ���� Ÿ��          : HRESULT
// ���� Ÿ�� ����     : 
// �Ķ����
//		STODCREATEINFO_IN* pIn  : 
//		STODCREATEINFO_OUT* pOut   : 
// �Լ� ����          : ������ �ʿ��� ������ �� �ѱ� ���¿��� Create�� �ش��ϴ� ��ƾó��
/////////////////////////////////////////////////////////////////////////////////extern "C" _declspec(dllexport) HRESULT FPLoad(SZSTODCREATEINFO_IN* pIn, SZSTODCREATEINFO_OUT* pOut)
HRESULT WINAPI fnLoad(STODCREATEINFO_IN* pIn, STODCREATEINFO_OUT* pOut)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle(HWStrategyAnalysisDLL.hResource);
	AfxInitRichEdit();
	CDlgAnalysis* pDlgAnalysis = (CDlgAnalysis*)pIn->m_pInitOut->m_pDllKey;
	if(pDlgAnalysis==NULL) return S_FALSE;
	
	// >-------------------------------------------------------
	// �� ��ƾ���� �����ΰ� �ʿ��� ���� �����ϴ� ó���� �Ѵ�.
	// <-------------------------------------------------------
	if(pDlgAnalysis) 
	{
//@�����߰�
		// Root Path
		pDlgAnalysis->m_strRootPath = (LPCSTR)AfxGetPctrData(10);	//������[����_PC�����͸Ŵ���.xls]
		
		// Data Path
		pDlgAnalysis->m_strDataDir = (LPCSTR)AfxGetPctrData(12);	//������[����_PC�����͸Ŵ���.xls]
//@�����߰�
//@��������
//		// Root Path
//		GET_ROOT_DIR(pDlgAnalysis->m_strRootPath);
//		if(pDlgAnalysis->m_strRootPath.GetLength()==0)
//		{
//			AfxMessageBox("RootPath�� ���� �� �����ϴ�.");
//		}
//		// Data Path
//		
//		GET_DATA_DIR(pDlgAnalysis->m_strDataDir);
//		if(pDlgAnalysis->m_strDataDir.GetLength()==0)
//		{
//			AfxMessageBox("DataPath�� ���� �� �����ϴ�.");
//		}
//
//		// User Directory ��� : KHD : ��� ã�� ���� �κ� ..
//		pDlgAnalysis->m_strUserDir = pDlgAnalysis->m_strRootPath + "\\User\\" + pDlgAnalysis->m_strUserID;
//@��������

		// User Directory ��� 
		IMainInfoManager* pMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
		pDlgAnalysis->m_strUserDir = pMng->GetEncUserDir();

		// ���̾Ʒα� ����
		pDlgAnalysis->Create(CDlgAnalysis::IDD, CWnd::FromHandle(pIn->m_hParent));
		pDlgAnalysis->ShowWindow(SW_SHOW);
		

	}
AfxSetResourceHandle(hInst);
	return S_OK;
}

// * ����
HRESULT WINAPI fnUnLoad(void* dwDllKey)
{
	CDlgAnalysis* pDlgAnalysis = (CDlgAnalysis*)dwDllKey;
	if(pDlgAnalysis)
	{
		pDlgAnalysis->DestroyWindow();
		
		delete pDlgAnalysis;
		pDlgAnalysis = NULL;
	}
	return S_OK;
}

HRESULT WINAPI fnFuncCall(void* dwDllKey, LPCSTR szFuncName, LPCSTR szParam, VARIANT& result)
{
	CDlgAnalysis* pDlgAnalysis = (CDlgAnalysis*)dwDllKey;
	if(pDlgAnalysis)
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

	HRESULT hr = fnInit(&InitIn, &InitOut, NULL);

	STODCREATEINFO_IN  InData; 
	STODCREATEINFO_OUT OutData;

	INIT_STODCREATEINFO_IN((&InData));
	InData.m_pInitOut = &InitOut;
	InData.m_hParent = pParent->GetSafeHwnd();
	InData.m_nID = iTradingID;
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

	HRESULT hr = fnInit(&InitIn, &InitOut, pInterface);
	
	if(InitOut.m_pDllKey==NULL) return NULL;
	STODCREATEINFO_IN  InData;
	STODCREATEINFO_OUT OutData;

	INIT_STODCREATEINFO_IN((&InData));
	InData.m_pInitOut = &InitOut;
	InData.m_hParent = pParent->GetSafeHwnd();
	InData.m_nID = iTradingID;
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

// ---------------------------------------------------------------------------------
// extern "C" __declspec(dllexport) BOOL SA_ApplyChart(LONG dwChartOcx, CListCtrl* pDivList, LONG dwKey)
// ��Ƽ��Ʈ�� �ý����ǿ��� "����" ��ư�� ������ �� ȣ����.
// ��Ʈ�� �������� �� ����Ʈ�� ���ҳ����� ǥ���ϱ� ���ؼ� ȣ����.
// Start : 06.07.28 alzio
// dwReserved1, dwReserved2 : ���� ����� ���ؼ� �������� ������.
// ---------------------------------------------------------------------------------
extern "C" __declspec(dllexport) CWnd* SA_ApplyChart(LONG dwChartOcx, CListCtrl* pDivList, LONG dwInfo, LONG pInterface, LONG dwReserved2)
{
	STMNG_ITEMINFO* pInfo = (STMNG_ITEMINFO*)dwInfo;
	STODINIT_IN  InitIn;
	STODINIT_OUT InitOut;

	INIT_STODINIT_IN((&InitIn));
	INIT_STODINIT_OUT2(InitOut);

	HRESULT hr = fnInit(&InitIn, &InitOut, (void*)pInterface);
	if(InitOut.m_pDllKey==NULL) return NULL;

	CDlgAnalysis* pDlgAnalysis;
	pDlgAnalysis =  (CDlgAnalysis*)InitOut.m_pDllKey;
	

	if(pDlgAnalysis==NULL) return NULL;

	IMainInfoManager* pMainInfo = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);

	// Root Path
	pDlgAnalysis->m_strRootPath = pMainInfo->GetRootDir();
		
	// Data Path
	pDlgAnalysis->m_strDataDir = pMainInfo->GetDataDir();

	// User Path
	pDlgAnalysis->m_strUserDir = pMainInfo->GetEncUserDir("chart");

	// ���̾Ʒα� ����
	pDlgAnalysis->m_bGraphDivide = TRUE;
	pDlgAnalysis->m_nStartType =1;// 0:�����м�, 1:�������
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle(HWStrategyAnalysisDLL.hResource);
	pDlgAnalysis->Create(CDlgAnalysis::IDD);
	pDlgAnalysis->ShowWindow(SW_HIDE);
	
	AfxSetResourceHandle(hInst);


	return pDlgAnalysis;
}
