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
	CString m_root, m_slog;
	CCommParam cmdInfo;
	ParseCommandLine(cmdInfo);
	SetRegistryKey(m_regKey);


	char szMain[MAX_PATH] = { 0, };
	::GetModuleFileName(NULL, szMain, sizeof(szMain));
	m_root.Format("%s", szMain);
	int ifind = m_root.ReverseFind('\\');
	m_root = m_root.Left(ifind);
	ifind = m_root.ReverseFind('\\');
	m_root = m_root.Left(ifind);

	m_slog.Format("[axiscore] m_root =[%s] m_regKey=[%s]\n", m_root, m_regKey);
	OutputDebugString(m_slog);


	CString	string;
	char	wb[1024];
	BOOL	fBool;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	//string = GetProfileString(ENVIRONMENT, ROOTDIR);
	//if (string.IsEmpty())
	//{
	//	::MessageBox(NULL, "Nothing install information!!", "Install error", MB_ICONWARNING);
	//	return FALSE;
	//	WriteReg();
	//}
	CString stmp;
	stmp.Format("[core] string [%s]", m_root);
	OutputDebugString(stmp);
	
//	if (IsInfoErr())	return FALSE;
	sprintf(wb, "%s\\exe\\axis.exe", m_root);
	//sprintf(wb, "%s\\exe\\aaaa.exe", m_root);
	m_slog.Format("%s", wb);
	OutputDebugString(m_slog);
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
	OutputDebugString("[core] " + string);
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
	m_slog.Format("[core][%s] wb=%s", __FUNCTION__, wb);
	OutputDebugString(m_slog);
	m_slog.Format("[core] %d", GetLastError());
	OutputDebugString(m_slog);
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

/////////////////////////////////////////////////////////////////////////////
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
	CString m_slog;
	CAxiscoreApp *app = (CAxiscoreApp *) AfxGetApp();
	m_slog.Format("[core] lpszParam=%s", lpszParam);
	OutputDebugString(m_slog);
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
		tmps.Format("%d", atoi(lpszParam));
		tmps.TrimRight();
		app->m_forcePort = tmps;
		break;
	}

}