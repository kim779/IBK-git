// KDefenseLancher.cpp: implementation of the CKDefenseLancher class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KApi2Launcher.h"
#include <winnt.h>

#include <io.h>
#include <errno.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKApi2Launcher::CKApi2Launcher() :
	m_hDll(NULL),
	m_lpkdfAutoStart(NULL),
	m_lpkdfGetVersion(NULL),
	m_lpkdfProtectModeOn(NULL),
	m_lpkdfProtectModeCheck(NULL),
#ifdef USE_KDFADDEDITCRTL
	m_lpkdfAddEditCtrl(NULL),
	m_lpkdfDelEditCtrl(NULL),
	m_lpkdfGetPassword(NULL), 
	m_lpkdfSetPassword(NULL), 
#endif
	m_lpkdfSelfCheckIntegrity(NULL),
	m_lpkdfSetImageDir(NULL),
	m_lpkdfExOption(NULL),
	m_bStart(false)
{
	loadKdfLibrary();
}

CKApi2Launcher::~CKApi2Launcher()
{
	kdfAutoStartClean();
}

bool CKApi2Launcher::loadKdfLibrary()
{
	if(m_hDll) return true;

// 	OSVERSIONINFOEX osvi;
// 	osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFOA);
// 	GetVersionExA((LPOSVERSIONINFO)&osvi);

	m_hDll = NULL;
	
	//HMODULE m_hDll =NULL;
	
// 	if(osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
// 	{
// 		// os ������ 8 ����
// 		
// 		m_hDll = LoadLibrary("kdfapi3.dll");
// 	}
// 	else
// 	{
// 		// os ������ 7 ���� ���� 
// 		
// 		m_hDll = LoadLibrary("kdfapi2.dll");
// 	}

	m_hDll = LoadLibrary(_T("kdfapi2.dll"));

	m_lpkdfAutoStart			= (kdfAutoStart_t)		GetProcAddress(m_hDll, "kdfAutoStart");
	m_lpkdfGetVersion			= (kdfGetVersion_t)		GetProcAddress(m_hDll, "kdfGetVersion");
	m_lpkdfProtectModeOn		= (kdfProtectModeOn_t)	GetProcAddress(m_hDll, "kdfProtectModeOn");
	m_lpkdfProtectModeCheck		= (kdfProtectModeCheck_t)GetProcAddress(m_hDll,"kdfProtectModeCheck");
	m_lpkdfSelfCheckIntegrity	= (kdfSelfCheckIntegrity_t)GetProcAddress(m_hDll, "kdfSelfCheckIntegrity");

	m_lpkdfExOption				= (kdfExOption_t)GetProcAddress(m_hDll, "kdfExOption");
/*
	kdfAutoStartClean_t		lpkdfAutoStartClean
		= (kdfAutoStartClean_t)	GetProcAddress(m_hDll, _T("kdfAutoStartClean"));
*/
	m_lpkdfSetImageDir		= (kdfSetImageDir_t)	GetProcAddress(m_hDll, "kdfSetImageDir");

#ifdef USE_KDFADDEDITCRTL
	m_lpkdfAddEditCtrl			= (kdfAddEditCtrl_t)GetProcAddress(m_hDll, "kdfAddEditCtrl");
	m_lpkdfDelEditCtrl			= (kdfDelEditCtrl_t)GetProcAddress(m_hDll, "kdfDelEditCtrl");
	m_lpkdfGetPassword			= (kdfGetPassword_t)GetProcAddress(m_hDll, "kdfGetPassword");
	m_lpkdfSetPassword			= (kdfSetPassword_t)GetProcAddress(m_hDll, "kdfSetPassword");
#endif


	if( m_lpkdfAutoStart == NULL		|| m_lpkdfGetVersion == NULL		||
		m_lpkdfProtectModeOn == NULL	|| m_lpkdfProtectModeCheck == NULL )
	{
#ifdef _DEBUG		
		OutputDebugString("[KLauncher] load function fail");
#endif
		this->freeKdfLibrary();
		
		return false;
	}

	return true;
}

void CKApi2Launcher::freeKdfLibrary()
{
	if(m_hDll)
	{
		m_lpkdfAutoStart			= NULL;
		m_lpkdfGetVersion			= NULL;
		m_lpkdfProtectModeOn		= NULL;
		m_lpkdfProtectModeCheck		= NULL;
		m_lpkdfSelfCheckIntegrity	= NULL;
		m_lpkdfSetImageDir			= NULL;
		m_lpkdfExOption				= NULL;
#ifdef USE_KDFADDEDITCRTL
		m_lpkdfAddEditCtrl			= NULL;
		m_lpkdfDelEditCtrl			= NULL;
		m_lpkdfGetPassword			= NULL;
		m_lpkdfSetPassword			= NULL;
#endif
		
		FreeLibrary(m_hDll);
		m_hDll = NULL;
	}
}

bool CKApi2Launcher::kdfAutoStart()
{
	if(m_hDll == NULL)
	{
		loadKdfLibrary();
		if(m_hDll == NULL) return false;
	}

	/*
	if(m_lpkdfSetImageDir)
	{
		m_lpkdfSetImageDir("C:\\temp");		// TrayIcon�� �ִ� ������ ����
	}
	else return false;
	*/
	m_lpkdfSetImageDir((LPSTR)(LPCTSTR)Format("%s\\%s", Axis::home, IMAGEDIR));

	if(m_lpkdfAutoStart && m_bStart == false)
	{
		//ex1) kdfAutoStart(NULL, FALSE, 0x00010001); // Default Icon ���(�ǰ����)
		//ex2) kdfAutoStart(NULL, FALSE, 0x0a010001); // ���� Tray Icon ���
		//ex3) kdfAutoStart(NULL, FALSE, 0x00020001); // Icon ��� ����

		// Param1 : �߰��� SubClassing ClassName
		// Param3 : ���� 16bit - ����Ʈ�ڵ�
		//if(m_lpkdfAutoStart(NULL, FALSE, 0x41010001) == FALSE)
		if(m_lpkdfAutoStart(NULL, FALSE, 0x00010001) == FALSE)
		{
#ifdef _DEBUG
			OutputDebugString("[KLauncher] kdfAutoStart fail");
#endif
			return false;
		}
#ifdef _DEBUG
		else OutputDebugString("[KLauncher] kdfAutoStart");
#endif
		m_bStart = true;
	}
	else return false;

	// Ȯ�� Options
	if(m_lpkdfExOption)
	{
		m_lpkdfExOption(KDFINJ_EXTYPE_PROTECT_HANNUM, 0, 0, 0);				//�ѱ� ��忡�� ���� ��ȣ ����

		// m_lpkdfExOption(KDFINJ_EXTYPE_BYPASS_STR, "ABCDEF", 0, 0);		//GetMsg���� ��ȣȭ�� ���ڿ� "ABCDEF" ���

		// m_lpkdfExOption(KDFINJ_EXTYPE_KDFMOD_OPT, 0, 0, 0);				//Ư�������� ��� GetMsg���� ��ȣȭ �Ѵ�.
	}

	//-- ���� ��ȣ������� �Ϲݸ������ �˾Ƴ���.
	m_bStart = (m_lpkdfProtectModeCheck() == 0)? false:true;

	return m_bStart;
	/*
	if(!m_bStart)
	{
		// �Ϲݸ���̸� ��ȣ���� ����
		if(!m_lpkdfProtectModeOn())
		{
			OutputDebugString("[KLauncher] protect mode on fail");
			return false;
		}
	}

	return true;
	*/
}

bool CKApi2Launcher::kdfAutoStartClean()
{
 	if(m_hDll && m_bStart)
	{
		kdfAutoStartClean_t		lpkdfAutoStartClean
			= (kdfAutoStartClean_t)	GetProcAddress(m_hDll, "kdfAutoStartClean");

		if(lpkdfAutoStartClean)
		{
			lpkdfAutoStartClean();
			freeKdfLibrary();
			m_bStart = false;
			return true;
		}
	}

	return false;
}
