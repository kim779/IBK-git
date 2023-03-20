// xsystem.cpp : implementation file
//

#include "stdafx.h"
#include "Wizard.h"
#include "xsystem.h"
#include "screen.h"
#include "client.h"
#include "script.h"
#include "stream.h"

#include "../h/axisvar.h"
#include <winsock2.h>
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CxSystem

IMPLEMENT_DYNCREATE(CxSystem, CCmdTarget)

CxSystem::CxSystem(CGuard* guard)
{
	EnableAutomation();
	m_guard  = guard;

	WORD	wVersionRequested;
	WSADATA	wsaData;
	char	wb[128];
	PHOSTENT hostinfo;

	m_host = _T("");
	wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		if (LOBYTE(wsaData.wVersion) == 2 || HIBYTE(wsaData.wVersion) == 2)
		{
			if (gethostname(wb, sizeof(wb)) != SOCKET_ERROR)
			{
				if ((hostinfo = gethostbyname(wb)) != NULL)
					m_host = CString(inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list));
			}
		}
		WSACleanup( );
	}

	m_userKey = FALSE;
}

CxSystem::~CxSystem()
{
}


void CxSystem::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CxSystem, CCmdTarget)
	//{{AFX_MSG_MAP(CxSystem)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CxSystem, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CxSystem)
	DISP_PROPERTY_EX(CxSystem, "TriggerOn", _getTriggerOn, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CxSystem, "ConnectBy", _getConnectBy, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CxSystem, "UserID", _getUserID, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CxSystem, "TermID", _getTermID, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CxSystem, "OrderConfirm", _getOrderConfirm, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CxSystem, "OrderPopup", _getOrderPopup, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CxSystem, "Home", _getHome, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CxSystem, "Host", _getHost, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CxSystem, "Sound", _getSound, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CxSystem, "UserKey", _getUserKey, _setUserKey, VT_BOOL)
	DISP_PROPERTY_EX(CxSystem, "SystemID", _getSystemID, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CxSystem, "UserPass", _getUserPass, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CxSystem, "CertifyPass", _getCertifyPass, SetNotSupported, VT_BSTR)
	DISP_FUNCTION(CxSystem, "Convert", _Convert, VT_BSTR, VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CxSystem, "IsPopup", _IsPopup, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CxSystem, "Help", _Help, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CxSystem, "Navigate", _Navigate, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CxSystem, "Trigger", _Trigger, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CxSystem, "GetAccountName", _GetAccountName, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CxSystem, "GetAccountPass", _GetAccountPass, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CxSystem, "GetHistory", _GetHistory, VT_BSTR, VTS_BOOL VTS_BSTR)
	DISP_FUNCTION(CxSystem, "SetHistory", _SetHistory, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CxSystem, "Trace", _Trace, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CxSystem, "Push", _Push, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CxSystem, "Pop", _Pop, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CxSystem, "Open", _Open, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CxSystem, "SetAutoCode", _SetAutoCode, VT_EMPTY, VTS_BSTR VTS_I4)
	DISP_FUNCTION(CxSystem, "ExcelToText", _ExcelToText, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CxSystem, "TextToExcel", _TextToExcel, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CxSystem, "Exit", _Exit, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CxSystem, "Menu", _Menu, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CxSystem, "Print", _Print, VT_BOOL, VTS_BSTR VTS_BSTR VTS_I4)
	DISP_FUNCTION(CxSystem, "PlaySound", _PlaySound, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CxSystem, "CheckPasswd", _CheckPasswd, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CxSystem, "Encrypt", _Encrypt, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CxSystem, "Decrypt", _Decrypt, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CxSystem, "EncryptSHA", _EncryptSHA, VT_BSTR, VTS_BSTR)
// updateXX_20160503
	DISP_FUNCTION(CxSystem, "CertifyFull", CertifyFull, VT_I4, VTS_BSTR VTS_I4 VTS_PVARIANT)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IxSystem to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {0325C6BE-9B41-4C2D-9794-EDD6BE0A304D}
static const IID IID_IxSystem =
{ 0x325c6be, 0x9b41, 0x4c2d, { 0x97, 0x94, 0xed, 0xd6, 0xbe, 0xa, 0x30, 0x4d } };

BEGIN_INTERFACE_MAP(CxSystem, CCmdTarget)
	INTERFACE_PART(CxSystem, IID_IxSystem, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CxSystem message handlers


BOOL CxSystem::_getTriggerOn() 
{
	int	value;
	
	value = m_guard->m_app->GetProfileInt(ENVIRONMENT, STATUS, 0);
	return (value & stTrigger) ? TRUE : FALSE;
}

long CxSystem::_getConnectBy() 
{
	return m_guard->m_app->GetProfileInt(INFORMATION, CONNECTBY, 0);
}

BSTR CxSystem::_getUserID() 
{
	CString strResult;

	m_guard->GetSignOn(strResult);
	return strResult.AllocSysString();
}

BSTR CxSystem::_getTermID() 
{
	CString strResult;

	m_guard->GetTerm(strResult);
	return strResult.AllocSysString();
}

BOOL CxSystem::_getOrderConfirm() 
{
	int	value;

	value = m_guard->m_app->GetProfileInt(ENVIRONMENT, STATUS, 0);
	return (value & stConfirm) ? TRUE : FALSE;
}

BOOL CxSystem::_getOrderPopup() 
{
	int	value;

	value = m_guard->m_app->GetProfileInt(ENVIRONMENT, STATUS, 0);
	return (value & stPopup) ? TRUE : FALSE;
}

BSTR CxSystem::_getHome() 
{
	CString strResult = m_guard->m_root;
	return strResult.AllocSysString();
}

BSTR CxSystem::_getHost() 
{
	return m_host.AllocSysString();
}

BSTR CxSystem::_getSound() 
{
	CString strResult;
	CString	tmps;
	char	wb[512];

	m_guard->GetSignName(tmps);
	strResult.Format("%s\\%s\\%s\\ensetup.ini", m_guard->m_root, USRDIR, tmps);
	GetPrivateProfileString("SCREEN", "soundfile", "", wb, sizeof(wb), strResult);
	strResult = wb;
	return strResult.AllocSysString();
}

BOOL CxSystem::_getUserKey() 
{
	return m_userKey;
}

void CxSystem::_setUserKey(BOOL bNewValue) 
{
	m_userKey = bNewValue;
}

BSTR CxSystem::_getSystemID() 
{
	CString strResult;

	strResult = m_guard->m_app->GetProfileString(WORKSTATION, SITEID, "D");
	if (strResult.IsEmpty())
		strResult = _T("D");
	return strResult.AllocSysString();
}

BSTR CxSystem::_getUserPass() 
{
	CString strResult;

	strResult = m_guard->GetLoginPass();
	return strResult.AllocSysString();
}

BSTR CxSystem::_getCertifyPass() 
{
	CString strResult;

	strResult = m_guard->GetCAPass();
	return strResult.AllocSysString();
}

BSTR CxSystem::_Convert(long type, LPCTSTR src, LPCTSTR file) 
{
	CString	strResult = _T("");
	CString	str, tmps;

	str = src;
	if (!str.IsEmpty())
	{
		char	wb[80];

		wb[0]  = '\0';
		tmps.Format("%s\\%s\\%s", m_guard->m_root, MTBLDIR, file);
		switch (type)
		{
		case 1:
			GetPrivateProfileString("Code", str, "", wb, sizeof(wb), tmps);
			break;
		case 2:
			GetPrivateProfileString("Description", str, "", wb, sizeof(wb), tmps);
			break;
		}

		strResult = wb;
	}

	return strResult.AllocSysString();
}

BOOL CxSystem::_IsPopup(LPCTSTR mapN) 
{
	CString	str = mapN;

	if (!str.IsEmpty())
		return m_guard->IsPopup(mapN) ? TRUE : FALSE;

	return FALSE;
}

void CxSystem::_Help(LPCTSTR file) 
{
	m_guard->RunHelp((char *)file);
}

void CxSystem::_Navigate(LPCTSTR url) 
{
	m_guard->SendAxis(MAKEWPARAM(htmlVIEW, 0), (LPARAM)url);
}

void CxSystem::_Trigger(LPCTSTR procs, LPCTSTR params, LPCTSTR maps) 
{
	m_guard->OnProcedure(procs, params, maps);
}

BSTR CxSystem::_GetAccountName(LPCTSTR account) 
{
	CString strResult, str;

	str = CString(account);
	strResult = _T("");
	if (!str.IsEmpty())
		m_guard->GetAcName(str, strResult);
	return strResult.AllocSysString();
}

BSTR CxSystem::_GetAccountPass(LPCTSTR account) 
{
	CString strResult, str;

	str = CString(account);
	strResult = _T("");
	if (!str.IsEmpty())
		m_guard->GetAcPass(str, strResult);
	return strResult.AllocSysString();
}

BSTR CxSystem::_GetHistory(BOOL all, LPCTSTR symbol) 
{
	CString strResult, str;

	str = symbol;
	if (str.IsEmpty())
		str = _T("1301");

	if (all)
		m_guard->GetHistory(str, strResult);
	else
		m_guard->GetCode(str, strResult);

	return strResult.AllocSysString();
}

void CxSystem::_SetHistory(LPCTSTR symbol, LPCTSTR code) 
{
	m_guard->SetCode(symbol, code);
}

void CxSystem::_Trace(LPCTSTR str) 
{
	m_guard->xTRACE(x_STRs, (char *)str);
}

void CxSystem::_Push(LPCTSTR name, LPCTSTR data) 
{
	CString	text;

	text.Format("%s\t%s", name, data);
	m_guard->PushData(text);
}

BSTR CxSystem::_Pop(LPCTSTR name) 
{
	CString strResult;

	m_guard->PopData(name, strResult);
	return strResult.AllocSysString();
}

BOOL CxSystem::_Open(LPCTSTR file) 
{
	if (ShellExecute(NULL, "open", file, NULL, NULL, SW_SHOW) <= (HINSTANCE)32)
		return FALSE;
	return TRUE;
}

void CxSystem::_SetAutoCode(LPCTSTR data, long count) 
{
	m_guard->SendAxis(MAKEWPARAM(autoVIEW, count), (LPARAM)data);
}

BOOL CxSystem::_ExcelToText(LPCTSTR files) 
{
	return m_guard->MakeXlsToTxt(files);
}

BOOL CxSystem::_TextToExcel(LPCTSTR files) 
{
	return m_guard->MakeTxtToXls(files);
}

void CxSystem::_Exit(BOOL reboot) 
{
	m_guard->PostAxis(MAKEWPARAM(closeAXIS, reboot ? true : false));
}

BOOL CxSystem::_Menu(LPCTSTR menus) 
{
	return m_guard->LoadMenu(menus);
}

BOOL CxSystem::_Print(LPCTSTR data, LPCTSTR fName, long fPoint) 
{
	if (fPoint)
		return m_guard->OnPrint(data, "Printing...", fName, fPoint);
	else
		return ShellExecute(NULL, "print", data, NULL, NULL, SW_HIDE) <= (HINSTANCE)32 ? FALSE : TRUE;
}

BOOL CxSystem::_PlaySound(LPCTSTR fileN) 
{
	if (::GetFileAttributes(fileN) == 0xffffffff)
		return FALSE;

	::sndPlaySound(NULL, SND_ASYNC|SND_NODEFAULT);
	return ::sndPlaySound(fileN, SND_ASYNC | SND_NODEFAULT);
}

BOOL CxSystem::_CheckPasswd(LPCTSTR passwd) 
{
	CString	pass;

	pass = m_guard->GetLoginPass();
	return pass.Compare(passwd) ? FALSE : TRUE;
}

BSTR CxSystem::_Encrypt(LPCTSTR data, LPCTSTR keys) 
{
	CString strResult;

	strResult = m_guard->Secure(data, keys, 0, true);
	return strResult.AllocSysString();
}

BSTR CxSystem::_Decrypt(LPCTSTR data, LPCTSTR keys) 
{
	CString strResult;

	strResult = m_guard->Secure(data, keys, 0, false);
	return strResult.AllocSysString();
}


BSTR CxSystem::_EncryptSHA(LPCTSTR data) 
{
	CString strResult;

	strResult = m_guard->Secure2(data);
	return strResult.AllocSysString();
}

// updateXX_20160503
const int maxCERT = 1024*8;
long CxSystem::CertifyFull(LPCTSTR srcB, long srcL, VARIANT FAR* desB) 
{
	if (srcL <= 0 || desB->vt != VT_BSTR)
		return -1;

	int	rc, retL = 0;
	CString	retB = CString(desB->bstrVal);

	rc = m_guard->CertifyFull(srcB, srcL, retB.GetBuffer(maxCERT), retL);
	if (retL > 0)
	{
		retL = ConvertHEX((char*) retB.operator LPCTSTR(), retL);
	}
	retB.ReleaseBuffer();

	desB->bstrVal = retB.AllocSysString();
	return retL;
}

int CxSystem::ConvertHEX(char* datB, int datL)
{
	int ii, jj;
	char* workB = new char[datL + 1];
	CopyMemory(workB, datB, datL);

	for (ii = 0, jj = 0; ii < datL;)
	{
		sprintf(&datB[jj], "%02X", (unsigned char) workB[ii]);
		ii += 1; jj += 2;
	}

	delete [] workB;
	return jj;
}