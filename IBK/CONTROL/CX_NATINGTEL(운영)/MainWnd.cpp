#include "DPhonePad.h"
#include "stdafx.h"
#include "CX_NATINGTEL.h"
#include "MainWnd.h"
#include <atlbase.h>
/////////////////////////////////////////////////////////////////////////////
// CMainWnd

void DoEvents(int nMilliSec)
{
	MSG	msg{};
	
#if defined(_WINDOWS)
	
	const	int	nLoop = nMilliSec / 10;
	for (int i = 0; i <= nLoop; i++)
	{
		for (int n = 0; n < 16 && PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ); n++)	//�̳༮�� 10 MilliSec ���� ����ϴ°� ����
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			i++;
		}
		
		if (nMilliSec > 0)
			Sleep(1);
	}
	
#endif
}

CMainWnd::CMainWnd(CWnd* parent)
{
	EnableAutomation();

	m_pWizard = parent;
	m_instance = nullptr;
}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	DISP_FUNCTION(CMainWnd, "Request", Request, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "Load", Load, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "ExitPhonePad", ExitPhonePad, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "StopRequest", StopRequest, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "GetStatus", GetStatus, VT_I2, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {9159A58B-E75F-42F8-9CB3-F97F543BF749}
static const IID IID_IMainWnd =
{ 0x9159a58b, 0xe75f, 0x42f8, { 0x9c, 0xb3, 0xf9, 0x7f, 0x54, 0x3b, 0xf7, 0x49 } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

CString bstrToCString(BSTR bstr)
{
	USES_CONVERSION;		//convert�� ���� ��ũ��

	char*	pbstr = nullptr;
	pbstr = OLE2A(bstr);		//bstr�� char�� convert
	return CString(pbstr);
} 

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

BSTR CMainWnd::Request() 
{
	CString strResult;
	CString string;
	const	short	nSinarioNum = 12, nTerminated = 1;
	short	nRtn = 0;
	
	if (m_instance == nullptr)
		return strResult.AllocSysString();

	const	int	nState = GetState();

	switch (nState)
	{
	case 2:
		string.Format("Set_Msg\t%s", "���е尡 �غ���� �ʾҽ��ϴ�.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;

	case 3:
		string.Format("Set_Msg\t%s", "Call Man ����̹��� ��ġ���� �ʾҽ��ϴ�.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;

	case 4:
		string.Format("Set_Msg\t%s", "Call Man �ó����� ���α׷��� ������� �ʾҽ��ϴ�.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;

	case 5:
		string.Format("Set_Msg\t%s", "���� ����� �������Դϴ�.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;

	case 9:
		string.Format("Set_Msg\t%s", "��Ÿ ������ ������ �� �����ϴ�.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;

	default:break;
	}

	if (nState != 1)
		return strResult.AllocSysString();
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------
	//BSTR CallRequestPasswdFuncB(short nCntDigits, short nTerminated, short nSinarioNum, short nTimeout, short &nRtn)
	//���� : ��й�ȣ ��û(������ ���ϵǱ���� ��ٸ�)
	//���� : nCntDigits - ��й�ȣ �ڸ���, ���Ṯ��(*, #) 
	//		 nTerminated - ���Ṯ��(*) �Է� ����(1 : �Է�, 0 : �Է¾���)
	//		 nSinarioNum - �ó����� ��ȣ
	//		 nTimeout - �Է� ��� ��
	//		 nRtn - ���� ����(1: ����, 0 : ������)
	//���ϰ� : �޼���(��й�ȣ, �Է� ��� ��)
	//-----------------------------------------------------------------------------------------------------------------------------------------------------
	
	typedef BSTR (__stdcall *CallRequestPasswdFuncB)(short, short, short, short, short *); 
	CallRequestPasswdFuncB pCallRequestPasswdFuncB;
	
	VERIFY(pCallRequestPasswdFuncB = (CallRequestPasswdFuncB)GetProcAddress(m_instance, "CallRequestPasswdFuncB"));

	if (pCallRequestPasswdFuncB == NULL)
	{
		//AfxMessageBox("CallRequestPasswdFuncB �Լ� ȣ�� ����");
		string.Format("Set_Msg\t%s", "CallRequestPasswdFuncB �Լ� ȣ�� ����");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		return strResult.AllocSysString();
	}
	
	//�ó�������ȣ(6 : ��й�ȣ, 7 : ���¹�ȣ, 1 : �ֹι�ȣ, 2 : �׽�Ʈ)
	
	//���Ṯ��(*, #) �Է� ����(1 : �Է�, 0 : �Է¾���)

	string.Format("Set_Msg\t%s", "��й�ȣ �Է��� ������̴� ��ø� ��ٷ��ֽʽÿ�.");
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
	
	BSTR	bstrMsg = (pCallRequestPasswdFuncB)(4, nTerminated, nSinarioNum, 10, &nRtn);
	if (nRtn)
	{
		strResult = bstrToCString(bstrMsg);

		string.Format("Set_Msg\t%s", "��й�ȣ �Է¿� �����߽��ϴ�.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
	}
	else
	{
		//AfxMessageBox("��й�ȣ �Է¿� �����߽��ϴ�.");
		string.Format("Set_Msg\t%s", "��й�ȣ �Է¿� �����߽��ϴ�.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
	}

	//AfxMessageBox(strResult);

	return strResult.AllocSysString();
}

int CMainWnd::GetState()
{
	short	nRtn = 0;
	CString strProcess = _T("");
	
	if (m_instance == nullptr)
		return -1;
	
	//-------------------------------------------------------------------------------------------------
	//BSTR CallGetPhonePadStateFuncB(BSTR bstrProcess, short &nRtn)
	//���� : ���е� ���� �ޱ�
	//���� : bstrProcess - ���е� �ó����� ���α׷���(PhonePad)
	//					   �۾������ڿ� ��µ� EXE���� ©������ �� �����Ƿ� Ȯ���ڴ� ���ܽ�Ŵ
	//		 nRtn - ���е� ���� �����ڵ�
	//				1 : ����
	//				2 : DEVICE UNREADY
	//	 			3 : ���е� ����̹� ��ġ ��	
	//				4 : ���е� �ó����� ���α׷� �������� �ƴ�
	//	 			9 : ��Ÿ
	//���ϰ� : �޼���
	//-------------------------------------------------------------------------------------------------
	
	typedef BSTR (__stdcall *CallGetPhonePadStateFuncB)(BSTR, short *); 
	CallGetPhonePadStateFuncB pCallGetPhonePadStateFuncB;
	
	VERIFY(pCallGetPhonePadStateFuncB = (CallGetPhonePadStateFuncB)GetProcAddress(m_instance, "CallGetPhonePadStateFuncB"));
	CString string;

	if (pCallGetPhonePadStateFuncB == NULL)
	{
		//AfxMessageBox("CallGetPhonePadStateFuncB �Լ� ȣ�� ����");
		string.Format("Set_Msg\t%s", "CallGetPhonePadStateFuncB �Լ� ȣ�� ����");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		return -1;
	}
	
	strProcess = "PhonePad";
	BSTR bstrMsg = (pCallGetPhonePadStateFuncB)(strProcess.AllocSysString(), &nRtn);

	return nRtn;
}

void CMainWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
// 	short nRtn = 0;
// 
// 	typedef BSTR (__stdcall *CallStopPasswdFuncB)(short *);
// 	CallStopPasswdFuncB pCallStopPasswdFuncB;
// 	
// 	VERIFY(pCallStopPasswdFuncB = (CallStopPasswdFuncB)GetProcAddress(m_instance, "CallStopPasswdFuncB"));
// 	
// 	BSTR bstrStop = (pCallStopPasswdFuncB)(&nRtn);
// 
// 	if (nRtn == 1)
// 	{
// 		typedef BSTR (__stdcall *CallExitPhonePadFuncB)(BSTR, short *); 
// 		CallExitPhonePadFuncB pCallExitPhonePadFuncB;
// 		
// 		VERIFY(pCallExitPhonePadFuncB = (CallExitPhonePadFuncB)GetProcAddress(m_instance, "CallExitPhonePadFuncB"));
// 		
// 		if (pCallExitPhonePadFuncB == NULL) {
// 			//AfxMessageBox("CallExitPhonePadFuncB �Լ� ȣ�� ����");
// 			return;
// 		}
// 		
// 		CString strProcess = "PhonePad";
// 		BSTR bstrMsg = (pCallExitPhonePadFuncB)(strProcess.AllocSysString(), &nRtn);
// 		
// 		if (m_instance)
// 		{
// 			FreeLibrary(m_instance);
// 			m_instance = NULL;
// 		}
// 	}
}

BOOL CMainWnd::Load() 
{
	CString string;

	m_PhonePadPath = _T("C:\\PhonePad\\PhonePad.exe");

	//���� DPhonepad.dll������ ������ ���� ���� ���� ������ Dphonepad.dll�� �̹� �ö� �־�
	//�ڲ� exe ������ �ִ� �������� �ε���.�׷��� ���Ƿ� DXPHONEPAD.DLL�� �̸�����
	//C:\\Program Files (x86)\\IBKS_STAFF\\IBK hot Trading(������)\\dev\\DXphonepad.dll
	m_instance = LoadLibrary("DXphonepad.dll");
	if (m_instance == nullptr)
	{
		//AfxMessageBox("DPHONEPAD.DLL �ε忡 �����߽��ϴ�.");
		string.Format("Set_Msg\t%s", "DPHONEPAD.DLL �ε忡 �����߽��ϴ�.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		return FALSE;
	}

	//-------------------------------------------------------------------------------------------------
	//BSTR CallSetSiteCodeFuncB(BSTR bstrSiteCode, short &nRtn)
	//���� : ����Ʈ�ڵ� ����
	//���� : bstrSiteCode - ����Ʈ�ڵ�(TYS)
	//		 nRtn - ���� ���(1: ����, 0 : ������)
	//���ϰ� : ���� ��� �޼���
	//-------------------------------------------------------------------------------------------------
	short	nRtn = 0;
	CString strSiteCode = "TYS";

	typedef BSTR (__stdcall *CallSetSiteCodeFuncB)(BSTR, short *);
	CallSetSiteCodeFuncB pCallSetSiteCodeFuncB;
	
	VERIFY(pCallSetSiteCodeFuncB = (CallSetSiteCodeFuncB)GetProcAddress(m_instance, "CallSetSiteCodeFuncB"));
	
	if (pCallSetSiteCodeFuncB == NULL)
	{
		//AfxMessageBox("CallSetSiteCodeFuncB �Լ� ȣ�� ����");
		string.Format("Set_Msg\t%s", "CallSetSiteCodeFuncB �Լ� ȣ�� ����");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		return FALSE;
	}
	
	BSTR	bstrMsg = (pCallSetSiteCodeFuncB)(strSiteCode.AllocSysString(), &nRtn);
	if (nRtn == 0)
	{
		//AfxMessageBox("�������� ���е� ����Ʈ�ڵ� ������ ���� �߽��ϴ�.");
		string.Format("Set_Msg\t%s", "�������� ���е� ����Ʈ�ڵ� ������ ���� �߽��ϴ�.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		return FALSE;
	}
	
	typedef BSTR (__stdcall *CallExecPhonePadFuncB)(BSTR, short *);
	CallExecPhonePadFuncB pCallExecPhonePadFuncB;
	
	VERIFY(pCallExecPhonePadFuncB = (CallExecPhonePadFuncB)GetProcAddress(m_instance, "CallExecPhonePadFuncB"));
	
	if (pCallExecPhonePadFuncB == NULL)
	{
		//AfxMessageBox("CallExecPhonePadFuncB �Լ� ȣ�� ����");
		string.Format("Set_Msg\t%s", "CallExecPhonePadFuncB �Լ� ȣ�� ����");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		return FALSE;
	}
	
	bstrMsg = (pCallExecPhonePadFuncB)(m_PhonePadPath.AllocSysString(), &nRtn);
	
	switch (nRtn)
	{
	case 1:
		string.Format("Set_Msg\t%s", "���е尡 �غ� �Ǿ����ϴ�.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;
	case 2:
		//AfxMessageBox("CALL MAN ����̹� ��ġ�� �Ǿ����� �ʽ��ϴ�.");
		string.Format("Set_Msg\t%s", "CALL MAN ����̹� ��ġ�� �Ǿ����� �ʽ��ϴ�.2");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;
	case 3:
		//AfxMessageBox("CALL MAN ���α׷��� �������� �ʽ��ϴ�.");
		string.Format("Set_Msg\t%s", "CALL MAN ���α׷��� �������� �ʽ��ϴ�.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;
	case 4:
		//AfxMessageBox("CALL MAN ���α׷��� ���࿡ �����߽��ϴ�.");
		string.Format("Set_Msg\t%s", "CALL MAN ���α׷��� ���࿡ �����߽��ϴ�.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;
	default:break;
	}

	return (nRtn == 1) ? TRUE : FALSE;
}

void CMainWnd::OnClose() 
{	
	CWnd::OnClose();
}

void CMainWnd::ExitPhonePad() 
{
	short	nRtn = 0;

	if (GetState() == 5)
	{
		typedef BSTR (__stdcall *CallStopPasswdFuncB)(short *);
		CallStopPasswdFuncB pCallStopPasswdFuncB;
		
		VERIFY(pCallStopPasswdFuncB = (CallStopPasswdFuncB)GetProcAddress(m_instance, "CallStopPasswdFuncB"));
		
		BSTR bstrStop = (pCallStopPasswdFuncB)(&nRtn);

		DoEvents(1100);
	}

	typedef BSTR (__stdcall *CallExitPhonePadFuncB)(BSTR, short *); 
	CallExitPhonePadFuncB pCallExitPhonePadFuncB;
	
	VERIFY(pCallExitPhonePadFuncB = (CallExitPhonePadFuncB)GetProcAddress(m_instance, "CallExitPhonePadFuncB"));
	
	if (pCallExitPhonePadFuncB == nullptr)
	{
		AfxMessageBox("CallExitPhonePadFuncB �Լ� ȣ�� ����");
		return;
	}
	
	CString strProcess = "PhonePad";
	BSTR bstrMsg = (pCallExitPhonePadFuncB)(strProcess.AllocSysString(), &nRtn);
	
	DoEvents(1100);
	
	if (m_instance)
	{
		FreeLibrary(m_instance);
		m_instance = nullptr;
	}
}

void CMainWnd::StopRequest() 
{
	short	nRtn = 0;
	CString string;
	
	typedef BSTR (__stdcall *CallStopPasswdFuncB)(short *);
	CallStopPasswdFuncB pCallStopPasswdFuncB;
	
	string.Format("Set_Msg\t%s", "������û�� �ߴ��� ��û�մϴ�.");
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());

	VERIFY(pCallStopPasswdFuncB = (CallStopPasswdFuncB)GetProcAddress(m_instance, "CallStopPasswdFuncB"));
	
	BSTR	bstrStop = (pCallStopPasswdFuncB)(&nRtn);
	if (nRtn == 1)
	{
		string.Format("Set_Msg\t%s", "������û�� �ߴ��Ͽ����ϴ�.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
	}
	else
	{
		string.Format("Set_Msg\t%s", "������û�� �ߴܿ� �����߽��ϴ�.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
	}
}

short CMainWnd::GetStatus() 
{
	return GetState();
}
