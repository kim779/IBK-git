// axMisc.cpp: implementation of the CAxMisc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axis.h"
#include "axisDoc.h"
#include "axMisc.h"
#include "mainfrm.h"
#include "childfrm.h"
#include "NPalette.h"

#include <comdef.h>
#define INITGUID
#include <initguid.h>
#include <exdisp.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAxMisc::CAxMisc(CString root, CString regkey)
{
	m_root   = root;
	const int	nLen = m_root.GetLength();
	
	if (nLen > 0)
	{
		if (m_root.GetAt(nLen - 1) == '\\')
			m_root = m_root.Left(nLen - 1);
	}

	m_regkey = regkey;
	LoadGuide();
}

CAxMisc::~CAxMisc()
{
	m_guide.RemoveAll();
}

int CAxMisc::MsgBox(CString msg, CString title, UINT type)
{
	if (title.IsEmpty())	title = _T("NOTIFY");
	type |= (MB_ICONEXCLAMATION|MB_TOPMOST|MB_SETFOREGROUND);

	//**return MessageBox(NULL, msg, title, type);
	return Axis::MessageBox(NULL, msg, type);
}

void CAxMisc::LoadGuide()
{
	CString		tmps;
	CStdioFile	sfile;

	m_guide.RemoveAll();
	tmps = _T("");
	m_guide.SetAt(0, tmps);
	tmps.Format("%s\\%s\\%s", m_root, MTBLDIR, AXISMSG);
	if (!sfile.Open(tmps, CFile::modeRead))
		return;

	int	idx{}, key{};
	while (sfile.ReadString(tmps))
	{
		if (!tmps.Find("//"))
			continue;
		idx = tmps.Find('=');
		if (idx < 0)
			continue;
		key = atoi(tmps.Left(idx++));
		tmps = tmps.Mid(idx);
		idx = tmps.Find("//");
		if (idx != -1)
			tmps = tmps.Left(idx);
		tmps.TrimRight();
		m_guide.SetAt(key, tmps);
	}
	sfile.Close();
}

void CAxMisc::GetGuide(int key, CString& string)
{
	if (!m_guide.Lookup(key, string))
		string.Format("UNKNOWN ERROR ... [%d]", key);
}

BOOL CAxMisc::RunVers(int type, CString user, CString pass, CString cpass)
{
	CString	aps, cmds, exes;
	STARTUPINFO		si;
	PROCESS_INFORMATION	pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb          = sizeof(STARTUPINFO);
	si.dwFlags     = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	const CAxisApp* app = (class CAxisApp *) AfxGetApp();
	if (app->m_exeName.IsEmpty())
		exes.Format("%s.exe", app->m_pszExeName);
	else	exes.Format("%s.exe", app->m_exeName);		
#pragma warning(disable : 6273)
	if (app->m_exeName.IsEmpty() && app->m_progK == 'B')
	{
		if (app->m_mode == MD_DEV)
			cmds.Format(" /c %d /d \"%s\" /a \"%s\" /p B /s %c", AfxGetMainWnd()->m_hWnd, m_root, exes, 0x7f);
		else
		{
			cmds.Format(" /c %d /d \"%s\" /a \"%s\" /p B", AfxGetMainWnd()->m_hWnd, m_root, exes);
		}
	}
	else
	{
		if (app->m_mode == MD_DEV)
			cmds.Format(" /c %d /d \"%s\" /a %s /s %c", AfxGetMainWnd()->m_hWnd, m_root, exes, 0x7f);
		else	
		{
			cmds.Format(" /c %d /d \"%s\" /a %s", AfxGetMainWnd()->m_hWnd, m_root, exes);
		}
	}
	CString reg; reg.Format(" /k %s", m_regkey);
	cmds += reg;
	
	if (!app->m_forceIP.IsEmpty())
		cmds += _T(" /z "+app->m_forceIP);
	else
	{
		/*
		if ((app->m_conIP == "172.16.205.20")||(app->m_conIP == "172.16.202.106")||(app->m_conIP == "211.255.204.131")||(app->m_conIP == "211.255.204.134"))
			cmds += _T(" /z "+app->m_conIP);
		*/
	}
#pragma warning(default : 6273)
	cmds += _T(" /u");
	if (!user.IsEmpty())
	{
		
		cmds += " ";
		cmds += user;
		cmds += " ";
		cmds += pass;
		cmds += "#";
		cmds += cpass;
	}

	if (type == verUSERID)
		cmds += _T(" /i");
	else if (type == verRETRY)
		cmds += _T(" /r");

	aps.Format("%s\\%s\\axisver.exe", m_root, RUNDIR);
	
	FILE* fp;
	fopen_s(&fp, Axis::home + "\\exe\\axMisc.log", "wb");
	if (fp)
	{
		const CTime time = CTime::GetCurrentTime();
		fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S]\n"));
		fprintf(fp, "\tapp - %s\n", (LPSTR)(LPCTSTR)aps);
		CString tcmd = cmds;
		tcmd.Replace(pass, "XXXXXXXX");
		tcmd.Replace(cpass, "XXXXXXXX");
		fprintf(fp, "\tcmd - %s\n", (LPSTR)(LPCTSTR)tcmd);
		fclose(fp);
	}

	const BOOL bRc = CreateProcess(
			aps,				// application name
			(char*)(const char*)cmds,// command line
			NULL,				// process attribute
			NULL,				// thread attribute
			FALSE,				// is inherit handle
			0,				// creation flags
			NULL,				// environment
			NULL,				// current directory
			&si,				// STARTUPINFO
			&pi);				// PROCESS_INFORMATION

	return bRc;
}