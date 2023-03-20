// axiscore.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "axiscore.h"
#include "axiscoreDlg.h"
#include "../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	PK_DEV		'D'	// DEFAULT DEV
#define	PK_BUSINESS	'B'	// 업무계
#define	PK_HTS		'H'	// HTS

#define	DEVKEY		"AXIS"				// axis.exe

#define IBK_DEV				"IBK"
#define IBK_DEV_STAFF		"IBK_STAFF"
#define IBK_HTS				"IBK투자증권MAC"
#define IBK_HTS_STAFF		"IBKMAC_STAFF"
/////////////////////////////////////////////////////////////////////////////
// CAxiscoreApp

BEGIN_MESSAGE_MAP(CAxiscoreApp, CWinApp)
	//{{AFX_MSG_MAP(CAxiscoreApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxiscoreApp construction

CAxiscoreApp::CAxiscoreApp()
{
	m_pk = PK_HTS;

	switch (m_pk)
	{
	default:
	case PK_HTS:
		m_regKey = IBK_HTS;	break;
	case PK_DEV:
		m_regKey = IBK_DEV;	break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAxiscoreApp object

CAxiscoreApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAxiscoreApp initialization

BOOL CAxiscoreApp::InitInstance()
{
//	if(::MessageBox(NULL, "Window Update가 필요합니다. 진행하시겠습니까?", "IBK투자증권",  MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL)
//		return true;

	CString m_slog;
	CCommParam cmdInfo;
	ParseCommandLine(cmdInfo);
	SetRegistryKey(m_regKey);

	char szMain[MAX_PATH] = {0,};
	::GetModuleFileName(NULL, szMain, sizeof(szMain));
	m_root.Format("%s", szMain);
	int ifind = m_root.ReverseFind('\\');
	m_root = m_root.Left(ifind);

	m_slog.Format("[axiscore] m_root =[%s]\n", m_root);
	OutputDebugString(m_slog);

	char buff[1024];
	DWORD dwSize;
	CString filefind;
	filefind.Format("%s\\AXISver.ini", m_root);
	filefind.Replace("exe", "tab");
	dwSize = GetPrivateProfileString("Axis", "ver", "0", buff, sizeof(buff), filefind);
	int iver = atoi(buff);

	dwSize = GetPrivateProfileString("Axis", "time", "2023012400", buff, sizeof(buff), filefind);
	int iOpenday = atoi(buff);

	m_slog.Format("[axiscore] ver =[%s]\n", buff);
	OutputDebugString(m_slog);
	
	//고객은 날짜가 1/25일 이후라면 업데이트를 한다.
	CTime tmCur = CTime::GetCurrentTime();
	CString sTime;
	sTime.Format("%d%02d%02d%02d", tmCur.GetYear(), tmCur.GetMonth(), tmCur.GetDay(), tmCur.GetHour());
	int itime = atoi(sTime);
	BOOL bWINupdate = FALSE;
	if(itime > iOpenday)
	{
		bWINupdate = Check_OS();
		//bWINupdate = true;  //test
		if(bWINupdate)
			return FALSE;

		if(atoi(buff) < 5) 
		{
			if (::MessageBox(NULL, "IBK update 진행합니다.", "IBK투자증권", MB_OK))
				WebInstall();
		}

		Check_VC_redist();
	}
	
	CString	string;
	char	wb[1024];
	BOOL	fBool;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;


	sprintf(wb, "%s\\axis.exe", m_root);
#if 0
	//	deb mode
	string.Format(" %s /k \"%s\" /n %s /p %c /s %c", m_lpCmdLine, m_regKey, m_pszExeName, m_pk, 0x7f);
#else
	//	real mode
	CString	sKey, eKey;
	if (0) //m_regKey == FCLIVE || m_regKey == FCLIVEFOXNET)
	{
		sKey = _T("22A3DE952232E078ADCE2568D376FA434E145741CDDFD2CB");
		eKey = _T("41CDDFD2CB");
	}

// first class live
//	string.Format(" %s /f \"%s\" /g \"%s\" /k \"%s\" /n %s /p %c",
//		m_lpCmdLine, sKey, eKey, m_regKey, m_pszExeName, m_pk);

// first class live Foxnet
	
	string.Format(" %s /f \"%s\" /g \"%s\" /k \"%s\" /n %s /p %c",
		m_lpCmdLine, sKey, eKey, m_regKey, m_pszExeName, m_pk);
	if (!m_forceIP.IsEmpty()) string+=" /a "+m_forceIP;
	if (!m_forcePort.IsEmpty()) string+=" /t "+m_forcePort;
	string += " /x";
#endif

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb          = sizeof(STARTUPINFO);
	si.dwFlags     = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	fBool = CreateProcess(  wb,					// application name
				(char *)string.operator LPCTSTR(),	// command line
				NULL,					// process attribute
				NULL,					// thread attribute
				FALSE,					// is inherit handle
				0,					// creation flags
				NULL,					// environment
				NULL,					// current directory
				&si,					// STARTUPINFO
				&pi);					// PROCESS_INFORMATION

	return FALSE;
}

void CAxiscoreApp::WriteReg()
{
	write_Regkey();
	WriteProfileInt(ENVIRONMENT, "KeyProtect", 1);
	WriteProfileInt(ENVIRONMENT, "Status", 4);

	WriteProfileInt(INFORMATION, "AOS", 1);
	WriteProfileInt(INFORMATION, "ConnectBy", 0);
	WriteProfileInt(INFORMATION, "PCFireWall", 1);
	WriteProfileString(INFORMATION, "Port", "15201");
	WriteProfileInt(INFORMATION, "Proxy", 0);
	WriteProfileInt(INFORMATION, "SDI", 0);
	WriteProfileString(INFORMATION, "Server", "211.255.204.71");
	WriteProfileInt(INFORMATION, "ToolStatus", 15);
	WriteProfileString(INFORMATION, "URL", "211.255.204.125");
	WriteProfileInt(INFORMATION, "clock_mini", 0);
	WriteProfileInt(INFORMATION, "clock_visible", 1);

	WriteProfileString(WORKSTATION, "BackUserID", "");
	WriteProfileInt(WORKSTATION, "CfgCopy", 1);
	WriteProfileInt(WORKSTATION, "HTS", 1);
	WriteProfileInt(WORKSTATION, "IdleTimeout", 0);
	WriteProfileString(WORKSTATION, "InitMap", "");
	WriteProfileInt(WORKSTATION, "OnlyGuest", 0);
	WriteProfileInt(WORKSTATION, "OnlySise", 0);
	WriteProfileInt(WORKSTATION, "RightMode", 1);
	WriteProfileInt(WORKSTATION, "SaveID", 1);
	WriteProfileInt(WORKSTATION, "SavePass", 1);
	WriteProfileInt(WORKSTATION, "ServerTime", 1);
	WriteProfileString(WORKSTATION, "ServiceID", "");
	WriteProfileString(WORKSTATION, "SignOnID", "");
	WriteProfileString(WORKSTATION, "SiteID", "");
	WriteProfileString(WORKSTATION, "TerminalName", "");
	WriteProfileInt(WORKSTATION, "TranTimeout", 0);
	WriteProfileInt(WORKSTATION, "UsageTimeout", 0);
	WriteProfileInt(WORKSTATION, "VerMode", 4);
}

void CAxiscoreApp::write_Regkey()
{
	DWORD	value;
	char	buf[1024];
	CString	home, tmpS;
	// ENVIRONMENT sectioon
	value = GetCurrentDirectory(sizeof(buf), buf);
	home = CString(buf, value);
	tmpS = home.Mid(home.GetLength() - 4);
	if (!tmpS.Compare("\\exe"))
		home = home.Left(home.GetLength() -4);

	WriteProfileString(ENVIRONMENT, ROOTDIR, home);
}

bool CAxiscoreApp::IsInfoErr()
{
	if (m_pk != PK_BUSINESS)	return false;

	CString crpyt  = GetProfileString(WORKSTATION, "crpyt");
	CString server, tmpS = GetProfileString(INFORMATION, "Server");
	server = tmpS;

	int	len, pos = server.Find('|');
	if (pos != -1)	server = tmpS.Left(pos);

	len = server.GetLength();
	char	src[16], dest[16], Key = 0x3D;
	sprintf(src, server, len);
	memset(dest, 0x00, sizeof(dest));

	for (int ii = 0; ii < len; ii++)
		dest[ii] = src[ii] ^ (Key + ii);

	if (crpyt.Compare(dest))
	{
		::MessageBox(NULL, "프로그램정보가 변경되었습니다.\n전산실로 문의 바랍니다.", "프로그램 정보변경", MB_ICONWARNING);
		return true;
	}

	return false;
}
#include <TlHelp32.h>

bool GetProcessModule(DWORD dwPID, char* sProcessName)
{
    HANDLE        hModuleSnap = NULL;
    MODULEENTRY32 me32        = {0};
    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	
    if (hModuleSnap == (HANDLE)-1)
        return (FALSE);
	
    me32.dwSize = sizeof(MODULEENTRY32);
	CString slog;
	//해당 프로세스의 모듈리스트를 루프로 돌려서 프로세스이름과 동일하면
    if(Module32First(hModuleSnap, &me32))
    {
        do
        {
            printf("process name : %s\n", me32.szModule);
			slog.Format("[mac] GetProcessModule =[%s] \r\n", me32.szModule);
		//	OutputDebugString(slog);
			if( strcmp(me32.szModule, sProcessName) == 0 )
			{
				CloseHandle (hModuleSnap);
				return true;
			}
        }
        while(Module32Next(hModuleSnap, &me32));
    }
	
    CloseHandle (hModuleSnap);
    return false;
}
//
HANDLE CAxiscoreApp::ProcessFind(char* strProcessName)
{

	HANDLE         hProcessSnap = NULL;
    BOOL           bRet      = FALSE;
    PROCESSENTRY32 pe32      = {0};
	
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
    if (hProcessSnap == (HANDLE)-1)
        return false;
	
    pe32.dwSize = sizeof(PROCESSENTRY32);
	CString slog;
    //프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다
    if (Process32First(hProcessSnap, &pe32))
    {
        BOOL          bCurrent = FALSE;
        MODULEENTRY32 me32       = {0};
		
        do
        {
            bCurrent = GetProcessModule(pe32.th32ProcessID,strProcessName);
			slog.Format("[MAC] [%s] \r\n", pe32.szExeFile);
			OutputDebugString(slog);
            if(bCurrent)
            {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
                if(hProcess)
                   return hProcess;
            }
        }
        while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프를 돈다.
    }
    CloseHandle (hProcessSnap);

	return NULL;
}

#include "DlgDown.h"
bool CAxiscoreApp::WebInstall()
{
	CDlgDown dlg;
	dlg.m_root = m_root;
	dlg.m_regkey = m_regKey;

	if(m_regKey.Find("STAFF") >= 0)
	{
		dlg.m_strUrl = DF_URL_STAFF_DOWN;
		dlg.m_strFileName = DF_SUPDATE;
	}
	else
	{
		dlg.m_strUrl = DF_URL_DOWN;   
		dlg.m_strFileName = DF_UPDATE;
	}
	
	dlg.DoModal();
	Install();
	
	return true;
}


#include <stdio.h>
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;
BOOL CAxiscoreApp::Is64BitWindows()
{
	CString m_sos, m_sprocess;
	BOOL bIsWow64 = true;
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
	
	if(fnIsWow64Process != NULL)
	{
		OutputDebugString("!!!!!!!!!!!!!!!!!BIT check FAIL!!!!!!!!!!!");
	}
	
	bool bProcess64 = false;
#if defined(_WIN64)   //64프로세스 정의되있는 경우 os는 64 (64os 에서는 64프로세스만 돌아간다.)
	bProcess64 = true;
	m_sos = "64os"; 
#elif defined(_WIN32) //32프로세스 정의인 경우 os 는 64 혹은 32 일수 있다
	BOOL f64 = FALSE;
	bProcess64 = fnIsWow64Process(GetCurrentProcess(), &f64) && f64;
#endif
	
	if(bProcess64)
		return TRUE;
	else
		return FALSE;
	
}

BOOL CAxiscoreApp::Check_OS()
{
	CString slog;
	OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    if( !GetVersionEx((OSVERSIONINFO *) &osvi))
    {
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);	
        GetVersionEx ((OSVERSIONINFO *)&osvi);
    }
	
	BOOL is64 = Is64BitWindows();

slog.Format("[axiscore]----------- Check_OS build numer = [%d] maj=[%d] min=[%d] b64=[%d]", osvi.dwBuildNumber, 
																							 osvi.dwMajorVersion,	
																							 osvi.dwMinorVersion,
																							 is64);
OutputDebugString(slog);

	if(osvi.dwBuildNumber == 7100)
	{
		CDlgDown dlg;
		dlg.m_root = m_root;
		dlg.m_regkey = m_regKey;
		
		if(is64)
		{
			dlg.m_strUrl = DF_URL_Win7_64;
			dlg.m_strFileName = DF_Win7_64;
		//	dlg.m_strUrl = DF_URL_DOWN;
		//	dlg.m_strFileName = DF_UPDATE;
		}
		else
		{
			dlg.m_strUrl = DF_URL_Win7_86;
			dlg.m_strFileName = DF_Win7_86;
		}
		
		dlg.DoModal();

		CString sfile;
		sfile.Format("%s\\%s", m_root, dlg.m_strFileName);
		OutputDebugString("[axiscore]   " + sfile);
	//	::MessageBox(NULL, "Window Update가 필요합니다. 진행하시겠습니까?", "IBK투자증권", MB_OK);
		if(::MessageBox(NULL, "윈도우 업데이트가 필요합니다. 업데이트 후 HTS를 다시 실행해주십시요", "IBK투자증권",  MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
		{
			::ShellExecute(NULL, "open", (LPCTSTR)sfile, NULL, NULL, SW_SHOWNORMAL); 
			return TRUE;
		}
		else
			return TRUE;
	}
	return FALSE;
}

void CAxiscoreApp::Check_VC_redist()
{
	CString slog;;
	CFileFind ff;
	CString strfile;
	char* pfilearr[] = {"mfc140.dll", "mfc140u.dll", NULL};

	for(int ii = 0 ; pfilearr[ii] != NULL; ii++)
	{
		strfile.Format("C:\\Windows\\System32\\%s", pfilearr[ii]);
slog.Format("[version] Check_VC_redist [%s]\r\n", strfile);
OutputDebugString(slog);
		CFileFind ff;
		if(!ff.FindFile(strfile))
		{
			CString string;
			string.Format("%s\\exe\\%s", GetProfileString(ENVIRONMENT, ROOTDIR), DF_UPDATE_VC);

			
			STARTUPINFO si;
			::ZeroMemory (&si, sizeof (STARTUPINFO));
			si.cb = sizeof (STARTUPINFO);
			PROCESS_INFORMATION pi;

			ZeroMemory(&si, sizeof(STARTUPINFO));
			ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
			
			si.cb          = sizeof(STARTUPINFO);
			si.dwFlags     = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_SHOWNORMAL;
			
			if( CreateProcess(  string,					// application name
				NULL,	// command line
				NULL,					// process attribute
				NULL,					// thread attribute
				FALSE,					// is inherit handle
				0,					// creation flags
				NULL,					// environment
				NULL,					// current directory
				&si,					// STARTUPINFO
				&pi))					// PROCESS_INFORMATION
			{
				
				WaitForSingleObject(pi.hThread, INFINITE) ;
				CloseHandle(pi.hThread);
			}
			return;
		}
	}
	return;
}

bool CAxiscoreApp::Install()
{
	CString slog;
	CString	string, filefind, strRoot;
	BOOL	fBool;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	if(m_regKey.Find("STAFF") >= 0)
		string.Format("%s\\exe\\%s", GetProfileString(ENVIRONMENT, ROOTDIR), DF_SUPDATE);
	else
		string.Format("%s\\exe\\%s", GetProfileString(ENVIRONMENT, ROOTDIR), DF_UPDATE);

	CFileFind ff;
	if(!ff.FindFile(string))
	{
		::MessageBox(NULL, "업데이트 파일을 내려받지 못했습니다.\n전산실로 문의 바랍니다. (1544-0050)", "프로그램 정보변경", MB_ICONWARNING);	
		return true;
	}

	HINSTANCE hprocess;
	SHELLEXECUTEINFO sei = {0,};
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.lpVerb = "open";
	sei.lpFile = string;
	sei.lpDirectory = "";
	sei.lpParameters = "";
	sei.nShow = SW_SHOWNORMAL;
	sei.hInstApp = NULL;
	sei.hProcess = hprocess;
	
	if(ShellExecuteEx(&sei))
	{
		int i =0;
		while(1)
		{
			Sleep(1000);
			i++;
			if(i >= 3)
				break;
		}

		HANDLE hwnd = ProcessFind("IFinst27.exe");
		if(hwnd)
			WaitForSingleObject(hwnd, INFINITE);
		else
		{
			slog.Format("[axiscore] no handle  err=[%d]", GetLastError());
			OutputDebugString(slog);
		}
	}
	else
	{
		slog.Format("[axiscore] Setupfile excute fail err=[%d]\n", GetLastError());
		OutputDebugString(slog);
	}

	

	return true;

//	
}

/////////////////////////
////////////////////////////////////////////////////
//
//	CCommParam
//

CCommParam::CCommParam()
{
}

CCommParam::~CCommParam()
{
}

void CCommParam::ParseParam(LPCSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	CAxiscoreApp *app = (CAxiscoreApp *) AfxGetApp();
	
	if (bFlag)
	{
		CString	tmps = lpszParam;

		if (tmps.GetLength() == 1)
		{
			switch (tmps[0])
			{
			case 'i': case 'k': case 'n': case 'p': case 's': case 'f': case 'g':
			case 'a': case 't': case 'z': case 'y':
				app->m_flag = tmps[0];
				break;
			}
		}
		return;
	}

	CString	str, tmps;
	switch (app->m_flag)
	{
	case 'i':
		app->userID += lpszParam;
		app->userID += '\t';
		break;
	case 'y':
	case 'k':
	default:
		
		app->m_regKey = lpszParam;
		break;
	case 'n':
//		app->m_exeName = lpszParam;
		break;
	case 'p':
/*
		str = lpszParam;
		if (!str.IsEmpty())
			app->m_progK = str.GetAt(0);
*/
		break;
	case 's':
/*
		str = lpszParam;
		if (!str.IsEmpty())
			app->m_mode = str.GetAt(0);
*/
		break;
	case 'f':
//		app->m_sFireKey = lpszParam;
		break;
	case 'g':
//		app->m_eFireKey = lpszParam;
		break;
	case 'a': // force ip address for op.
	case 'z':
		app->m_forceIP = lpszParam;
		//AfxMessageBox(app->m_forceIP);
		break;
	case 't': // force port number for op.
		app->m_forcePort = atoi(lpszParam);
		break;
	}

}


	/*
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	
	si.cb          = sizeof(STARTUPINFO);
	si.dwFlags     = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;
	
	if( CreateProcess(  string,					// application name
		NULL,	// command line
		NULL,					// process attribute
		NULL,					// thread attribute
		FALSE,					// is inherit handle
		0,					// creation flags
		NULL,					// environment
		NULL,					// current directory
		&si,					// STARTUPINFO
		&pi))					// PROCESS_INFORMATION
	{
		EnableWindow(FALSE);
		WaitForSingleObject(pi.hThread, INFINITE) ;
	}


	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
*/
