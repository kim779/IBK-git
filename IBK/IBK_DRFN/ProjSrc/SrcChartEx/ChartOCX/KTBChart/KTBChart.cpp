// KTBChart.cpp : Implementation of CKTBChartApp and DLL registration.

#include "stdafx.h"
#include "KTBChart.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for IMetaTable
#include "ProgIDInfo.h"									// for CProgIDInfo::ComServerFromProgID()
#include "KTBChartCtl.h"								// for CKTBChartCtrl
#include "OcxIDs.h"										// for _OCX_tlid

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// (2004.11.26, ��¿�) �Ǻк��� ����к��� ���� ������ Loading�ϴ� Interface Header�� �����Ͽ�,
//		OCX APP InitInstance()���� �⺻ ������ Loading�� �� �ֵ��� �Ѵ�.
CHARTOCX_API BOOL LoadNomalizedMinuteTimeRule( const char *p_szHtsPath);

CKTBChartApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid = _OCX_TYPE_LIBRARY_ID_VALUES;
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.23
//
//	����	:	OCX�� ��ġ�� ��θ� ���Ͽ� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
CString	g_strOcxPath;

// (2004.11.26, ��¿�) Real�� ����Ʈ�� ����������� ��ü �ð��� ������ �������� Flag���� ���� �����Ѵ�.
//		Ư��, ������ Global�� �Ӽ��� �ξ� HTS �⺻ ������ �����ϰ�,
//		������ Property�� �����Ͽ� Chart�� �Ӽ��� ������� �Ѵ�.
//		�⺻���� �Ǻ��� �������� �ϸ�, HTS�� �⺻ ���� CFG File�� �����Ͽ� ��������Ѵ�.
BOOL g_bNormalizedMinuteTimeRule = FALSE;

////////////////////////////////////////////////////////////////////////////
// CKTBChartApp::InitInstance - DLL initialization

BOOL CKTBChartApp::InitInstance()
{
	AfxEnableControlContainer();
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.

		// (2003.12.23, ��¿�) OCX�� ��ġ�� ��θ� ���Ͽ� �����Ѵ�.
		CString strOcxFile;
		CProgIDInfo::ComServerFromProgID( _OCX_PROG_ID, strOcxFile);
		g_strOcxPath = strOcxFile.Left( strOcxFile.ReverseFind( '\\') + 1);

		// (2004.11.26, ��¿�) Real�� ����Ʈ�� ����������� ��ü �ð��� ������ �������� Flag���� ���� �����Ѵ�.
		//		Ư��, ������ Global�� �Ӽ��� �ξ� HTS �⺻ ������ �����ϰ�,
		//		������ Property�� �����Ͽ� Chart�� �Ӽ��� ������� �Ѵ�.
		//		�⺻���� �Ǻ��� �������� �ϸ�, HTS�� �⺻ ���� CFG File�� �����Ͽ� ��������Ѵ�.
		//		�̶� ������ HTS Main Path�� ���޹޾�, ������ Loading�ϴ� Routine�� �̿��Ѵ�.
		//		���⼭�� OCX�� Path�� �������� ó���Ѵ�. (�⺻ Bin Folder�� �ִ� ���� ������ �Ѵ�.)
		LoadNomalizedMinuteTimeRule( g_strOcxPath + "..\\");

		// (2006/11/24 - Seung-Won, Bae) Check ToolName Define with Tool IDs
		if( IMetaTable::m_nToolOptionCount != CToolOptionInfo::T_TOOLOPTION_COUNT)
		{
			CString strMsg;
			strMsg.Format( "%d ToolOptinName Not Defined!\r\nPlease Check it!", CToolOptionInfo::T_TOOLOPTION_COUNT - IMetaTable::m_nToolOptionCount);
			AfxMessageBox( strMsg);
			return FALSE;
		}
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CKTBChartApp::ExitInstance - DLL termination

int CKTBChartApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	// (2006/8/23 - Seung-Won, Bae) Manage AddIn Load String
	CKTBChartCtrl::m_mapAddInLoadString.RemoveAll();

	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	g_iMetaTable.UnloadAllResourceDLL();

	return COleControlModule::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}

// (2004.11.26, ��¿�) Real�� ����Ʈ�� ����������� ��ü �ð��� ������ �������� Flag���� ���� �����Ѵ�.
//		Ư��, ������ Global�� �Ӽ��� �ξ� HTS �⺻ ������ �����ϰ�,
//		������ Property�� �����Ͽ� Chart�� �Ӽ��� ������� �Ѵ�.
//		�⺻���� �Ǻ��� �������� �ϸ�, HTS�� �⺻ ���� CFG File�� �����Ͽ� ��������Ѵ�.
//		�̶� ������ HTS Main Path�� ���޹޾�, ������ Loading�ϴ� Interface�� �����Ѵ�.
//		������ Loading�� �������� Return�Ѵ�.
CHARTOCX_API BOOL LoadNomalizedMinuteTimeRule( const char *p_szHtsPath)
{
	// 1. HTS Path�� �̿��Ͽ� CFG Path�� �����Ѵ�.
	CString strCfgPath( p_szHtsPath);
	int nHtsPathLength = strCfgPath.GetLength();
	if( 0 < nHtsPathLength)
	{
		if( strCfgPath.GetAt( nHtsPathLength - 1) != '\\') strCfgPath += "\\";
		strCfgPath += "Data\\Config.cfg";
		
		// 2. CFG�� CHART Section�� NMTR�� ���� �ݿ��Ѵ�. (0�� 1�� Ȯ���Ѵ�.)
		g_bNormalizedMinuteTimeRule = !( GetPrivateProfileInt( "CHART", "NMTR", g_bNormalizedMinuteTimeRule ? 1 : 0, strCfgPath) == 0);
	}

	return g_bNormalizedMinuteTimeRule;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/3/5
// Modifier		: 
// Comments		: Support Version Info for Loading of System Trading Strategy DLL
//					1 : Release
//					2 : Debug
//-----------------------------------------------------------------------------
CHARTOCX_API int IsDebugVersion( void)
{
#ifndef _DEBUG
	return FALSE;
#else
	return TRUE;
#endif
}
