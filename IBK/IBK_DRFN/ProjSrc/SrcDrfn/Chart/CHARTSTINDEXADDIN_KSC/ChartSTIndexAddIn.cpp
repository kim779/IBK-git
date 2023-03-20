// ChartSTIndexAddIn.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ChartSTIndexAddIn.h"

#include "./Include_AddIn/_IAddInManager.h"			// for IAddInManager
#include "./Include_AddIn/I139092/_IChartAddIn.h"	// for IChartAddIn
#include "SiteCustomAddin.h"		// for CSiteCustomAddin
#include "../../inc/IMasterDataMng.h"

//#ifdef __MULTI_LANG__
	#include "../Include_Chart/Dll_Load/IMetaTable.h"
	#include "../INCLUDE_ST/SiteDefine.h"
//#endif

//#include "../../IncSite/DefineForSite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HINSTANCE g_hInstance;

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
// CChartSTIndexAddInApp
CChartSTIndexAddInApp theApp;

BEGIN_MESSAGE_MAP(CChartSTIndexAddInApp, CWinApp)
	//{{AFX_MSG_MAP(CChartSTIndexAddInApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartSTIndexAddInApp construction

CChartSTIndexAddInApp::CChartSTIndexAddInApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bIsInitedMapIndex = FALSE;
}

#include "../../inc/IMainInfoMng.h"			// ��Ʈocx�� ��ΰ� bin/binD�� �ƴҰ�쿡�� �ý���Ʈ���̵� ����� ó���ϱ� ���ؼ�
BOOL CChartSTIndexAddInApp::InitInstance() 
{
	g_hInstance = theApp.m_hInstance;
	{
		CString strFullName = theApp.m_pszHelpFilePath;
		CString strResult;
		char aDrive[10]={0,}, aDir[_MAX_PATH]={0,};
		_tsplitpath(strFullName, aDrive, aDir, NULL, NULL);
		strResult.Format("%s%s", aDrive, aDir);
		IMainInfoManager* pMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
		if(pMng) pMng->SetDataString(29, (LPCSTR)strResult);		// 29 : Chart Ocx ��� ���ϱ�
	}

//#ifdef __MULTI_LANG__
	if(g_strChartProgID.IsEmpty()) AfxSetChartProgID();
//#endif
	return CWinApp::InitInstance();
}

int CChartSTIndexAddInApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_bIsInitedMapIndex = FALSE;
	return CWinApp::ExitInstance();
}

BOOL CChartSTIndexAddInApp::IsInitedMapIndex()
{
	return m_bIsInitedMapIndex;
}

///////////////////////////////////////////////////////////////////////////////
//	author	:	Geun-moon Oh
//	E-Mail	:	gm0604@dooriic.co.kr
//	date	:	2005.08.18
//	Object	:	Load Graph Index List into Map.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartSTIndexAddInApp::InitMapIndex(LPCTSTR lpMainPath)
{
	CString strListFile;
	//>>@Solomon
	//CString strGraphDirPath; // = lpMainPath;
	CString strGraphDirPath = lpMainPath;
	//<<
	//strListFile += "\\DATA\\STDATA\\SystemTradingIndexList.dat";
	//strListFile += "\\DATA\\SystemTradingIndexList.dat";		//��Ʈ������ data ���丮�� �����ϱ� ������ ���� ��η� ������.
	strListFile = strGraphDirPath + "\\icss\\SystemTradingIndexList.dat";	//@Solomon
//@��������
//#ifdef _DEBUG
//	strGraphDirPath += "\\DATA\\STDATA\\STSystemD\\";
//#else
//	strGraphDirPath += "\\DATA\\STDATA\\STSystem\\";
//#endif
//@��������
//@�����߰�
	//strGraphDirPath += "\\DATA\\STDATA\\STSystem\\";
	strGraphDirPath += "\\icss\\STDATA\\STSystem\\";	//@Solomon
//@�����߰�

	CStdioFile file;
	if(!file.Open(strListFile, CFile::modeRead))
	{
		CString strMsg;
		strMsg.Format("[%s]������ �������� �ʽ��ϴ�.");
		AfxMessageBox(strListFile);
		return FALSE;
	}

	m_mapIndex.RemoveAll();
	m_mapIndexNoPriceInput.RemoveAll();
	m_mapIndexType.RemoveAll();
	m_mapIndexScaleType.RemoveAll();

	CString strGraphName;
	CString strGraphPath;
	CString strHavingPrice;
	CString strType;
	CString strScaleType;
	int nPos = -1;
	CString rStrLine;
	while(file.ReadString(rStrLine))
	{
		strGraphName = "";
		strGraphPath = "";
		strHavingPrice = "";
		strType = "";
		strScaleType = "";
		
		nPos = rStrLine.Find(';');
		if(nPos==-1)	continue;		
		strGraphName = rStrLine.Left(nPos);			// first..
		rStrLine.Delete(0,nPos+1);		

		nPos = rStrLine.Find(';');
		if(nPos==-1)	continue;		
		strGraphPath = rStrLine.Left(nPos);			// second..		
		rStrLine.Delete(0,nPos+1);

		nPos = rStrLine.Find(';');
		if(nPos==-1)	goto SETDATA;		
		strHavingPrice = rStrLine.Left(nPos);		// third..
		rStrLine.Delete(0,nPos+1);

		nPos = rStrLine.Find(';');
		if(nPos==-1)	goto SETDATA;		
		strType = rStrLine.Left(nPos);				// fourth..		
		rStrLine.Delete(0,nPos+1);

		nPos = rStrLine.Find(';');
		if(nPos==-1)	goto SETDATA;		
		strScaleType = rStrLine.Left(nPos);		// fifth..
		rStrLine.Delete(0,nPos+1);


SETDATA:
		//////////////////////////////////////////////////////////////
		strGraphPath = strGraphDirPath + strGraphPath;
		m_mapIndex.SetAt(strGraphName,strGraphPath);
		if(!atol(strHavingPrice))
		{
			m_mapIndexNoPriceInput.SetAt(strGraphName,strGraphName);
		}
		if(strType.GetLength())
		{
			m_mapIndexType.SetAt(strGraphName,strType);
		}
		if(strScaleType.GetLength())
		{
			m_mapIndexScaleType.SetAt(strGraphName,strScaleType);
		}
	}
	m_bIsInitedMapIndex = TRUE;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	author	:	Geun-moon Oh
//	E-Mail	:	gm0604@dooriic.co.kr
//	date	:	2005.08.18
//	Object	:	Check whether the index has Price input variable.
//////////////////////////////////////////////////////////////////////////////
CString CChartSTIndexAddInApp::GetIndexFile(LPCTSTR lpIndexName, BOOL &rBHasPriceInput,CString& rStrType, CString& rStrScaleType)
{
	CString strGraphPath;

	CString szParamCombine = lpIndexName;
	int nDivinePos = szParamCombine.Find(";");
	CString szIndicatorName = szParamCombine.Left(nDivinePos);
	CString szGraphName = szParamCombine.Mid(nDivinePos + 1);

	//	BOOL bRetValue =m_mapIndex.Lookup(lpIndexName,strGraphPath);
	BOOL bRetValue =m_mapIndex.Lookup(szIndicatorName,strGraphPath);
	if(bRetValue)	
	{
		CString strIndexName;
		//		rBHasPriceInput = m_mapIndexNoPriceInput.Lookup(lpIndexName,strIndexName);
		rBHasPriceInput = m_mapIndexNoPriceInput.Lookup(szIndicatorName,strIndexName);
		rBHasPriceInput = !rBHasPriceInput;

		//		m_mapIndexType.Lookup(lpIndexName,rStrType);
		//		m_mapIndexScaleType.Lookup(lpIndexName,rStrScaleType);
		m_mapIndexType.Lookup(szIndicatorName,rStrType);
		m_mapIndexScaleType.Lookup(szIndicatorName,rStrScaleType);

		return strGraphPath;
	}
	return "";
}

void CChartSTIndexAddInApp::GetCodeName(UINT nMarket, LPCTSTR lpszCode, LPSTR lpszName, size_t nSize)
{
	IMasterDataManager* pIDataManager = (IMasterDataManager*)AfxGetPctrInterface(5);
	if(nMarket == 0)		// �ֽ�
	{
		CString strName = pIDataManager->GetCodeInfo("JMASTER", 0, lpszCode);
		if(strName.IsEmpty())
			strName = pIDataManager->GetCodeInfo("KMASTER", 0, lpszCode);

		if( int(nSize-1) < strName.GetLength())
		{
			::memset(lpszName, 0x00, nSize);
			::strncpy(lpszName, strName, nSize);
		}

		::lstrcpy(lpszName, strName);
	}
	else if(nMarket == 1)	// �����ɼ�
	{
		CString strName = pIDataManager->GetCodeInfo("FMASTER:PMASTER", 0, lpszCode);

		if( int(nSize-1) < strName.GetLength())
		{
			::memset(lpszName, 0x00, nSize);
			::strncpy(lpszName, strName, nSize);
		}

		::lstrcpy(lpszName, strName);
	}
	else
		::memset(lpszName, 0x00, nSize);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CChartSTIndexAddInApp object


// (2004.10.07, ��¿�) AddIn DLL�� Interface�� �����ϱ� ���� Export Type�� �����Ѵ�.
#define ADDIN_API extern "C" __declspec(dllexport)

// (2004.10.08, ��¿�) Chart AddIn DLL�� Interface Version�� �����ϴ� Macro�� �����Ѵ�.
#define _CHART_ADDIN_IVERSION( nOVersion)										\
	int		g_nChartAddInVersion = 1##nOVersion - 1000000;						\
	char	g_szChartAddInIVersion[ 50] = "ChartAddIn Version : " #nOVersion;
// (2004.10.08, ��¿�) Chart AddIn DLL�� Version�� Static String���� �����Ͽ� DLL Module File�� ��õǵ��� �Ѵ�.
//		���� int�ε� �����Ͽ� GetAddInIVersion���� �̿��� �� �ֵ��� �Ѵ�.
_CHART_ADDIN_IVERSION( 139092)

// [04/10/07] Chart AddIn DLL�� Version�� �˷��ִ� Interface�� �����Ѵ�.
ADDIN_API int GetAddInIVersion(void)
{
	return g_nChartAddInVersion;
}

// [04/10/07] Chart AddIn DLL�� Version�� �˷��ִ� Interface�� �����Ѵ�.
ADDIN_API IChartAddIn *InitAddInDll( const char *p_szAddInItem, IAddInManager *p_pIAddInManager, IAddInDllBase *p_pIAddInDllBase)
{
	// 1. Chart Ocx�� Interface�� ���Ѵ�.
	IChartOCX *pIChartOCX = p_pIAddInManager->GetIChartOCX();
	if( !pIChartOCX) return NULL;
	// 2. Chart Ocx�� Interface�� �̿��Ͽ� AddIn Object�� �����Ѵ�.
	IChartAddIn *pIChartAddIn = new CSiteCustomAddin( pIChartOCX, p_pIAddInDllBase);
	pIChartAddIn->AddRef();
	// 3. Chart Ocx�� Interface�� Release�Ѵ�.
	pIChartOCX->Release();

	// (2004.10.13, ��¿�) ������ AddIn Object�� �⺻ Chart Manager���� ����� ��ϵ��� ������,
	//		AddIn ������ ��ҽ�Ų��.
	if( !pIChartAddIn->HasFullManager())
	{
		pIChartAddIn->Release();
		pIChartAddIn = NULL;
	}

	// 4. ������ AddIn Object�� Interface�� Return�Ѵ�.
	return pIChartAddIn;
}


