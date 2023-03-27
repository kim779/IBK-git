// IBKSConnectorCtl.cpp : Implementation of the CIBKSConnectorCtrl ActiveX Control class.

#include "stdafx.h"
#include "IBKSConnector.h"
#include "IBKSConnectorCtl.h"
#include "IBKSConnectorPpg.h"
#include "AxisUtil.h"
#include "grid_i.h"
#include <math.h>

#define MB_CAPTION		("IBK��������")
#define MB_ERROPTION	(MB_OK|MB_ICONERROR)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// #define FOR_WARSHIP


IMPLEMENT_DYNCREATE(CIBKSConnectorCtrl, COleControl)

BEGIN_MESSAGE_MAP(CIBKSConnectorCtrl, COleControl)
	//{{AFX_MSG_MAP(CIBKSConnectorCtrl)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()
//DISP_FUNCTION(CIBKSConnectorCtrl, "TR3202", TR3202, VT_BOOL, VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
//DISP_FUNCTION(CIBKSConnectorCtrl, "TR3201", TR3201, VT_BOOL, VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4)
//DISP_FUNCTION(CIBKSConnectorCtrl, "TR3211", TR3211, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR)
//DISP_FUNCTION(CIBKSConnectorCtrl, "TR3221", TR3221, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR)
//DISP_FUNCTION(CIBKSConnectorCtrl, "TR3222", TR3222, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR)
//DISP_FUNCTION(CIBKSConnectorCtrl, "TR3231", TR3231, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_R8 VTS_I4 VTS_I4 VTS_R8 VTS_R8)
BEGIN_DISPATCH_MAP(CIBKSConnectorCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CIBKSConnectorCtrl)
	DISP_FUNCTION(CIBKSConnectorCtrl, "Initialize", Initialize, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CIBKSConnectorCtrl, "Uninitialize", Uninitialize, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CIBKSConnectorCtrl, "Login", Login, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4)
	DISP_FUNCTION(CIBKSConnectorCtrl, "GetLastErrMsg", GetLastErrMsg, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CIBKSConnectorCtrl, "GetUserName", GetUserName, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CIBKSConnectorCtrl, "GetHome", GetHome, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CIBKSConnectorCtrl, "GetUserID", GetUserID, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CIBKSConnectorCtrl, "Logout", Logout, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR1001", TR1001, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR1201", TR1201, VT_BOOL, VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR1211", TR1211, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR1221", TR1221, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR1231", TR1231, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_R8)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR3001", TR3001, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR3201", TR3201, VT_BOOL, VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR3211", TR3211, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR3221", TR3221, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR3231", TR3231, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_R8 VTS_I4 VTS_I4 VTS_R8 VTS_R8)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR1801", TR1801, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR1802", TR1802, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR1803", TR1803, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR1804", TR1804, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "GetAccounts", GetAccounts, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR1002", TR1002, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR1003", TR1003, VT_BOOL, VTS_I4 VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR3002", TR3002, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR3003", TR3003, VT_BOOL, VTS_I4 VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "GetCode", GetCode, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR1202", TR1202, VT_BOOL, VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR3202", TR3202, VT_BOOL, VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4224", TR4224, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_R8)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4223", TR4223, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4303", TR4303, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4201", TR4201, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4101", TR4101, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4221", TR4221, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4401", TR4401, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4202", TR4202, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4003", TR4003, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4004", TR4004, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "SetPTS", SetPTS, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4501", TR4501, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4502", TR4502, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4503", TR4503, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4511", TR4511, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4512", TR4512, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4521", TR4521, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4522", TR4522, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4523", TR4523, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4524", TR4524, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4525", TR4525, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4526", TR4526, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4527", TR4527, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "SaveConfig", SaveConfig, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR8001", TR8001, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "GoURL", GoURL, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR1222", TR1222, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_I4)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR3222", TR3222, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR4403", TR4403, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR3411", TR3411, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "GetGreeksData", GetGreeksData, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR3232", TR3232, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_I4)
	DISP_FUNCTION(CIBKSConnectorCtrl, "TR2001", TR2001, VT_BOOL, VTS_I4 VTS_BSTR VTS_I4)
	DISP_FUNCTION(CIBKSConnectorCtrl, "SetPrograms", SetPrograms, VT_EMPTY, VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_EVENT_MAP(CIBKSConnectorCtrl, COleControl)
	//{{AFX_EVENT_MAP(CIBKSConnectorCtrl)
	EVENT_CUSTOM("OnLogin", FireOnLogin, VTS_BOOL  VTS_BSTR)
	EVENT_CUSTOM("OnContract", FireOnContract, VTS_BSTR)
	EVENT_CUSTOM("OnRealData", FireOnRealData, VTS_BSTR)
	EVENT_CUSTOM("OnRecvData", FireOnRecvData, VTS_I4  VTS_I4  VTS_I4  VTS_BOOL  VTS_BSTR)
	EVENT_CUSTOM("OnError", FireOnError, VTS_BSTR)
	EVENT_CUSTOM("OnClose", FireOnClose, VTS_NONE)
	EVENT_CUSTOM("OnAlert", FireOnAlert, VTS_I4  VTS_BSTR)
	EVENT_CUSTOM("OnRecvDataID", FireOnRecvDataID, VTS_I4  VTS_I4  VTS_I4  VTS_BOOL  VTS_BSTR  VTS_I4)
	EVENT_CUSTOM("OnVerUpdate", FireOnVerUpdate, VTS_NONE)
	EVENT_CUSTOM("OnUpdateStart", FireOnUpdateStart, VTS_NONE)
	EVENT_CUSTOM("OnUpdating", FireOnUpdating, VTS_BSTR  VTS_I4)
	EVENT_CUSTOM("OnUpdateEnd", FireOnUpdateEnd, VTS_NONE)
	EVENT_CUSTOM("OnGuideMsg", FireOnGuideMsg, VTS_I4  VTS_BSTR)
	EVENT_CUSTOM("OnFBalance", FireOnFBalance, VTS_BSTR)
	EVENT_CUSTOM("OnSBalance", FireOnSBalance, VTS_BSTR)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()

BEGIN_EVENTSINK_MAP(CIBKSConnectorCtrl, CWnd)
ON_EVENT(CIBKSConnectorCtrl, -1, 1, OnAxisFire, VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

BEGIN_PROPPAGEIDS(CIBKSConnectorCtrl, 1)
	PROPPAGEID(CIBKSConnectorPropPage::guid)
END_PROPPAGEIDS(CIBKSConnectorCtrl)

IMPLEMENT_OLECREATE_EX(CIBKSConnectorCtrl, "IBKSCONNECTOR.IBKSConnectorCtrl.1",
	0xcdadd338, 0xc7ab, 0x4977, 0xb6, 0x5d, 0x8e, 0x98, 0x8b, 0x59, 0x58, 0xe3)


IMPLEMENT_OLETYPELIB(CIBKSConnectorCtrl, _tlid, _wVerMajor, _wVerMinor)

const IID BASED_CODE IID_DIBKSConnector =
		{ 0xc66f0d7b, 0x235c, 0x409a, { 0x90, 0x18, 0x5, 0x87, 0xd1, 0x9, 0x4c, 0x20 } };
const IID BASED_CODE IID_DIBKSConnectorEvents =
		{ 0x55f8c481, 0x7091, 0x4aca, { 0x8e, 0x9c, 0xa5, 0xba, 0x13, 0x94, 0x43, 0x6 } };

static const DWORD BASED_CODE _dwIBKSConnectorOleMisc =
//	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CIBKSConnectorCtrl, IDS_IBKSCONNECTOR, _dwIBKSConnectorOleMisc)

#pragma warning (disable : 4996)

BOOL CIBKSConnectorCtrl::CIBKSConnectorCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_IBKSCONNECTOR,
			IDB_IBKSCONNECTOR,
			afxRegApartmentThreading,
			_dwIBKSConnectorOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}

CIBKSConnectorCtrl::CIBKSConnectorCtrl()
{
	InitializeIIDs(&IID_DIBKSConnector, &IID_DIBKSConnectorEvents);

	// COM Object ������ ���� �ʱ�ȭ �۾�
	m_bInit = m_bLogin = FALSE;
	m_bPTS = false;		//2012.08.08 KSJ PTS�⺻�� false
	m_bUpdate	=	false;	//2012.08.27 KSJ UpdateStart�� ȣ��Ǿ����� üũ�ؼ� UpdateEnd�� ȣ���Ѵ�.
	m_nPggb = 0;		// 2014.04.21 KSJ ����/������ ����
	m_hiSha256 = NULL;

	CoInitialize(NULL);
	AfxEnableControlContainer();
}

CIBKSConnectorCtrl::~CIBKSConnectorCtrl()
{
	CoUninitialize();
}

void CIBKSConnectorCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	CRect rect;
	HICON hIcon = AfxGetApp()->LoadIcon(IDI_IBK48);
#if 0
	GetClientRect(&rect);
	DrawIconEx(pdc->m_hDC, 0, 0, hIcon, rect.Width(), rect.Height(), 0, NULL, DI_IMAGE);
#else
	DrawIconEx(pdc->m_hDC, 0, 0, hIcon, rcBounds.Width(), rcBounds.Height(), 0, NULL, DI_IMAGE);
#endif
	DestroyIcon(hIcon);
}

void CIBKSConnectorCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

}

void CIBKSConnectorCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
}

BOOL CIBKSConnectorCtrl::Initialize() 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (IsInit()) return TRUE;

	int n;
	char szPath[MAX_PATH];
	GetCurrentDirectory(sizeof(szPath), szPath);
	CString sRoot = GetRootPath();

	CString slog;
	slog.Format("\r\n [openapi]  sRoot = [%s] \r\n", sRoot);
	OutputDebugString(slog);

	SetCurrentDirectory(sRoot);

	for(n=0; OCX[n]!=NULL; ++n)
	{
		if (!RegisterOCX( sRoot + "\\bin\\" + OCX[n] ))
		{
			if (!RegisterOCX( sRoot + "\\" + OCX[n] ))
			{
				m_sLastMsg.Format("ActiveX ��� ����[%d] - %s[%s]", GetLastError(),OCX[n], sRoot);
				return FALSE;
			}
		}
	}

	const char * const REG_KEY = "IBKSConnector";
	SetWindowText(REG_KEY);

	sRoot = sRoot.Left(sRoot.ReverseFind('\\'));

	SetAxisRegistry(REG_KEY, sRoot);

	if (!m_Wizard.Create(NULL, NULL, WS_CHILD|WF_OLECTLCONTAINER, CRect(0,0,0,0), this, 0))
	{
		int ierr = GetLastError();
		m_sLastMsg = "IBKSWizard ���� ����";
		return FALSE;
	}

	if (m_dummyWnd.Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), &m_Wizard, 0))
	{
		m_Wizard.axAttach((long)&m_dummyWnd, vtypeNRM, 0);
	} 
	else
	{
		m_sLastMsg = "MessageWindow ���� ����";
		return FALSE;
	}

	// TR ���� Init
	for(n=0; limit[n].trkey!=eNone ; ++n)
		m_limit[ limit[n].trkey ] = limit[n];

	// �ֹ�ȣ�� Init
	for(n=0; n<(sizeof(pibosodr_hogb)/sizeof(int)); ++n)
		m_shoga.insert(pibosodr_hogb[n]);
	for(n=0; n<(sizeof(pibofodr_hogb)/sizeof(int)); ++n)
		m_fhoga.insert(pibofodr_hogb[n]);

	SetCurrentDirectory(szPath);

	return (m_bInit=TRUE);
}

bool CIBKSConnectorCtrl::IsInit()
{
	return m_bInit;
}

bool CIBKSConnectorCtrl::IsLogin()
{
	return m_bLogin;
}

bool CIBKSConnectorCtrl::IsPTS()
{
	return m_bPTS;
}

afx_msg int CIBKSConnectorCtrl::OnAxisFire(int type, WPARAM wParam, LPARAM lParam)
{
// 	CString emsg;
// 	emsg.Format("[KSJ] OnAxisFire - [%d][%d][%d]\n", type, wParam, lParam);
// 	OutputDebugString(emsg);

	switch(type)
	{
	case FEV_OPEN: 
#ifdef FOR_WARSHIP
		return OnFevOpen(wParam, lParam);
#endif
		break;
	case FEV_AXIS:
		return OnFevAxis(wParam, lParam);
	case FEV_FMX:
		return OnFevFmx (wParam, lParam);
	case FEV_ANM:
		return OnFevAnm (wParam, lParam);
	case FEV_RUN:
		if(wParam == 0)
		{
			if(lParam == 1) // �÷��� ���� ���׷��̵� ������
			{
				FireOnVerUpdate();	//2012.08.27 �÷���������Ʈ �̺�Ʈ �߻���Ŵ
			}
			else
			{
				if (m_bUpdate)	//2012.08.27 �α��� �ϱ� ���� ���Ͼ�����Ʈ �����ٴ� �̺�Ʈ �߻���Ŵ
				{
					FireOnUpdateEnd();
					m_bUpdate = false;
				}

				return OnFevOpen(wParam, lParam);
			}
		}
		break;
	case FEV_ERROR:
// 		OutputDebugString("[KSJ]FEV_ERROR\n");

		if (lParam && LOWORD(wParam)==1)
		{
			CString emsg = (char*)lParam;
// 			OutputDebugString("[KSJ][" + emsg + "]");
			int ecod = atoi(emsg.Left(4));

			if (ecod==4368)
			{
				int pos[2], nTotal, nRemain;
				
				pos[0] = emsg.Find("ȸ");
				if (pos[0]==-1) break;
				nTotal = atoi(emsg.Mid(pos[0]-1,1));
				
				pos[1] = emsg.Find("ȸ", pos[0]+2);
				if (pos[1]==-1) break;
				nRemain = atoi(emsg.Mid(pos[1]-1,1));
				
				emsg.Format("���Ӻ�й�ȣ�� %dȸ �߸��Է��ϼ̽��ϴ�.", nTotal-nRemain);
				// 3ȸ �̻� ����
				if (nRemain==0)
				{
					emsg.Format(
						"���Ӻ�й�ȣ�� ��%dȸ �߸��Է��Ͽ� ����� �����Ǿ����ϴ�.\n\n"
						"Ȩ������ Ȥ�� ������ �����Ͽ� ����Ƚ���� �ʱ�ȭ �Ͻñ� �ٶ��ϴ�.", nTotal);
				}
				else if ((nTotal-nRemain)>=3)
				{
					CString mmsg;
					mmsg.Format("%s\n\n�� %dȸ ������ ��������˴ϴ�.", emsg, nTotal);
					emsg = mmsg;
				}
			}
			else if(ecod==4369)
			{
				emsg.Format(
					"���Ӻ�й�ȣ ���ӿ��� ���Ƚ���� �ʰ��Ͽ����ϴ�.\n\n"
					"Ȩ������ Ȥ�� ������ �����Ͽ� ����Ƚ���� �ʱ�ȭ �Ͻñ� �ٶ��ϴ�.");
			}
			FireOnError(emsg);
		}
		break;
	case FEV_STAT:
// 		emsg.Format("[KSJ] FEV_STAT - [%d][%s][%d]\n", type, (LPCSTR)lParam, (long)wParam);
// 		OutputDebugString(emsg);

		FireOnUpdating((LPCSTR)lParam, (long)wParam);	//2012.08.27 ������Ʈ�߿� �����̸�, ������� �����ش�.
//  		OutputDebugString("[KSJ]FEV_STAT\n");
		break;
	case FEV_SIZE:
		FireOnUpdateStart();	//2012.08.27 ���Ͼ�����Ʈ ���� �̺�Ʈ �߻���Ŵ
		m_bUpdate = true;
		break;
	case FEV_CLOSE:
		FireOnClose();
		break;
	default:
		break;
	}
	return 0;
}

void CIBKSConnectorCtrl::Uninitialize() 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (!IsInit()) return;

	CString sRoot = GetRootPath();
	
	for(int n=0; OCX[n]!=NULL; ++n)
		UnregisterOCX( sRoot + "\\bin\\" + OCX[n] );

	CoUninitialize();
}

int CIBKSConnectorCtrl::OnFevOpen(WPARAM wParam, LPARAM lParam)
{
	// Login ��û
	if (lParam==0)
	{
		CString sEnc;
		sEnc.Format("%s\t%s", (LPCSTR)m_uspw, (LPCSTR)m_usid);
		char *szEnc = (char*)m_Wizard.axWizard(MAKEWPARAM(encPASS, modeUID), (LPARAM)(LPCSTR)sEnc);
		
		axlogon_mid mid;
		memset(&mid, ' ', L_axlogon_mid);
		strcpy(mid.user, (LPCSTR)m_usid);
		strcpy(mid.pass, (LPCSTR)szEnc);
		strcpy(mid.cpas, (LPCSTR)m_certpw);
		strcpy(mid.uips, (LPCSTR)m_cltip);
		strcpy(mid.madr, (LPCSTR)m_maddr);
		mid.forc[0] = '0';	// ����[0] ����[2]
		mid.atcp[0] = '1';	// ����[0] �ڵ�[1]
		mid.norm[0] = '0';	// �ֹ�[0] �ü�[1]
		mid.cust[0] = '1';	// ����[0] ��[1]

// 		if(IsPTS())
// 			mid.type[0] = 'P';		//PTS�̸� 'P'
// 		else
//		mid.type[0] = 'Y';		//Open Api�̸� 'Y'
		
		mid.type[0]	= 'D';	// D�� �ø����� �������� Y�� �ٲ�.�������� �� �ٲٱ⿡�� �δ��� Ŀ�� �α��δܿ��� �ٲ�ġ�� �ϱ�� õ����԰� ���� 2018.10.15
 
// 		CString strTemp;
// 		strTemp.Format("[KSJ]1[%s]", mid.type);
// 		OutputDebugString(strTemp);

		memset(mid.rsvd, 0, 5);	//2012.04.17 6 -> 5�� ����
		m_Wizard.RunAxis(signUSER, (long)&mid, L_axlogon_mid);		
		
// 		char szBuf[128];
// 
// 		CopyMemory(szBuf, (char *)&mid, L_axlogon_mid);
// 		szBuf[L_axlogon_mid] = '\0';
// 
// 		strTemp.Format("[KSJ] API[%-12s%-10s%-1s%-1s%-1s%-1s%-1s%-5s%-30s%-15s%-16s]", mid.user, mid.pass, mid.forc, mid.atcp, mid.norm, mid.cust, mid.type, mid.rsvd, mid.cpas, mid.uips, mid.madr);
// 		OutputDebugString(strTemp);

	}
	return 0;
}

int CIBKSConnectorCtrl::OnFevAxis(WPARAM wParam, LPARAM lParam)
{
	int mKey = LOWORD(wParam);
	int sKey = HIWORD(wParam);
	LPCSTR szMsg = (LPCSTR)lParam;
	CString strMsg, strAcno;
 
	switch(mKey)
	{
	case loginERR:	// �α��� ����
		FireOnLogin(FALSE, szMsg);
		break;
	case runAXIS:	// �α��� ����
		CheckValidUser();
		break;
	case alarmPAN:  // ��ް���
		if (HIWORD(wParam))
			FireOnAlert(0, szMsg);
		break;
	case dialogPAN:	// �ý��۸޼���
		strMsg = szMsg;
		switch(sKey)
		{
		case 0x01:
			break;
		case 0x99:	//2015.04.06 KSJ ������ API���� �����ϸ� �ش���¸� �����ϵ��� ����
			if (strMsg.GetLength()>0)
			{
				switch(strMsg.GetAt(0))
				{
				case '#':		// �ش���¸� ����
					strAcno = strMsg.Mid(1, 11);

					FireOnAlert(sKey, strMsg);
					for(size_t n=0; n<m_acno.size(); ++n)
					{
						if (!strAcno.Compare(m_acno[n]))
						{
							FireOnAlert(sKey, m_acno[n]);

							if(m_bSaveLog)
								testSaveFile("Not Found", m_acno[n], 11);
							
							m_acno.erase(m_acno.begin()+n);
							m_acnm.erase(m_acnm.begin()+n);
							break;
						}
					}	
					break;
				}
			}
			break;
		default:
			FireOnAlert(sKey, szMsg);
			break;
		}
		break;
	case noticePAN:	// ü���뺸
		FireOnContract(szMsg);
		break;
	default:
		break;
	}
	return 0;
}

int CIBKSConnectorCtrl::OnFevFmx (WPARAM wParam, LPARAM lParam)
{
//	OutputDebugString("[KSJ] OnFevFmx");
	map<int, TR_CALLBACK>::iterator pos = m_callback.find(LOWORD(wParam));

// 	CString strTemp;
// 	strTemp.Format("[KSJ] WPARAM[%d]", LOWORD(wParam));
	if (pos!=m_callback.end())
	{
// 		OutputDebugString("[KSJ] cd start\n");
		TR_CALLBACK cb = pos->second;
		UnregCallBack(LOWORD(wParam));
		(this->*cb)(wParam, lParam);
// 		OutputDebugString("[KSJ] cd End\n");
	}
	return 0;
}

int CIBKSConnectorCtrl::OnFevAnm (WPARAM wParam, LPARAM lParam)
{
#ifdef xFOR_WARSHIP
	return 0;
#endif
	struct _alertR *alert = (struct _alertR*)lParam;

	//return 0;

	DWORD *dptr;
	char buff[4096];
	for(int ii=0, pos=0; ii<alert->size; ++ii)
	{
		
		dptr = (DWORD*)alert->ptr[ii];
		pos = sprintf(&buff[pos], "%s\t", alert->code);

		for(int jj=0; jj<maxSYMBOL; ++jj)
		{
			if (dptr[jj])
				pos += sprintf(&buff[pos], "%03d\t%s\t", jj, (char*)dptr[jj]);
			buff[pos] = NULL;
		}
		FireOnRealData((LPCSTR)buff);
	}
	return 0;
}

void* CIBKSConnectorCtrl::Variant( int cmd, LPARAM lParam )
{
	return (void*)::SendMessage(m_dummyWnd.GetSafeHwnd(), WM_USER, MAKEWPARAM(variantDLL, cmd), lParam);
}

void CIBKSConnectorCtrl::GetLedger(void *param)
{
	::SendMessage(m_dummyWnd.GetSafeHwnd(), WM_USER, ledgerDLL, (LPARAM)param);
	struct ledger *ledger = (struct ledger *)param;
	CString pcip = CString(ledger->pcip, sizeof(ledger->pcip));
	pcip.TrimRight(); pcip.TrimLeft();
	if (pcip.GetLength()==0 && m_fcltip.GetLength()==12)
		memcpy(ledger->pcip, (LPCSTR)m_fcltip, sizeof(ledger->pcip));
}

int CIBKSConnectorCtrl::SendTR( LPCSTR trnm, int tkey, int stat, LPCSTR data, int size, TR_CALLBACK cb)
{
	if (!RegCallBack(tkey, cb)) return FALSE;

	vector<char> buff(L_user_th + size);
	struct user_th *th = (struct user_th*)&buff[0];
	th->key = (char)tkey;
	th->stat = (char)stat;
	memcpy(th->trxc, trnm, 8);
	memcpy(&buff[L_user_th], data, size);

// 	CString strTemp;
// 	strTemp.Format("[KSJ] SendTr data[%s] size[%d] buff[%s]\n", data, size, (LPCSTR)&buff[0]);
// 	OutputDebugString(strTemp);


	return m_Wizard.axTR((long)(LPCSTR)&buff[0], (long)buff.size()-L_user_th);
}

bool CIBKSConnectorCtrl::RegCallBack(int key, TR_CALLBACK cb)
{
	m_cs.Lock();
	map<int, TR_CALLBACK>::iterator pos = m_callback.find(key);
	bool bRet = true;
	if (pos == m_callback.end())
	{
		m_callback[key] = cb;
		m_cbcount[key] = 1;
	}
	else
	{
		if (pos->second==cb)
		{
			++m_cbcount[key];
		}
		else
		{
			m_sLastMsg = "�ߺ�Ű ���";
			bRet = false;
		}
	}
	m_cs.Unlock();
	return bRet;
}

void CIBKSConnectorCtrl::UnregCallBack(int key)
{
	m_cs.Lock();
	map<int, int>::iterator pos = m_cbcount.find(key);
	if (pos->second>1)
		--(pos->second);
	else
		m_callback.erase(key);
	m_cs.Unlock();
}

bool CIBKSConnectorCtrl::AccessTr(TR_KEY key)
{
#ifdef FOR_WARSHIP
	return true;
#else
	map<TR_KEY, struct LIMIT_ITEM>::iterator pos = m_limit.find(key);
	if (pos!=m_limit.end())
	{
		LIMIT_ITEM *li = &(pos->second);
		DWORD tick = GetTickCount();
		DWORD term = tick - li->stick;

		if ( term >= li->check )
		{
			// ��ġ�� Access�ð��� �����ð����� Ŭ��� Pass
			li->stick = tick;
			li->count = 1;
			return true;
		}
		else
		{
			li->count += 1;
			// �����ð����� ����Ƚ������ ���� �����ߴ��� �Ǵ�!
			bool bRet = (li->count<=li->limit);
			if (!bRet)
				m_sLastMsg = "[TR����] ����� �̿��Ͻʽÿ�.";
			return bRet;
		}
	}
	else 
		return true;
#endif
}

BOOL CIBKSConnectorCtrl::Login(LPCTSTR user_id, LPCTSTR user_pw, LPCTSTR cert_pw, LPCTSTR svr_ip, long svr_port) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!IsInit())
	{
		m_sLastMsg = "�ʱ�ȭ���� �ʾҽ��ϴ�.";
		MessageBox(m_sLastMsg, MB_CAPTION, MB_ERROPTION);
		return FALSE;
	}
	if (IsLogin())
	{
		m_sLastMsg = "�̹� �α��εǾ� �ֽ��ϴ�. ������ �� �̿��ϼ���";
		MessageBox(m_sLastMsg, MB_CAPTION, MB_ERROPTION);
		return FALSE;
	}

	m_usid = user_id;
	m_uspw = user_pw;
	m_certpw = cert_pw;
	m_svrip = svr_ip;
	m_svrport = svr_port;
	
	m_usid.TrimRight(); m_usid.TrimLeft();
	m_uspw.TrimRight(); m_uspw.TrimLeft();
	//m_certpw.TrimRight(); m_certpw.TrimLeft();
	m_svrip.TrimRight(); m_svrip.TrimLeft();
	
	if (m_usid.GetLength()==0)
	{
		MessageBox("����ID�� �Է��ϼ���.", MB_CAPTION, MB_ERROPTION);
		return FALSE;
	}

	if (m_uspw.GetLength()==0)
	{
		MessageBox("���Ӻ�й�ȣ�� �Է��ϼ���.", MB_CAPTION, MB_ERROPTION);
		return FALSE;
	}

	if (m_certpw.GetLength()==0)
	{
		MessageBox("��������й�ȣ�� �Է��ϼ���.", MB_CAPTION, MB_ERROPTION);
		return FALSE;
	}

	if (m_svrip.IsEmpty())
		m_svrip = GetGlbIP(m_usid, "", "");
	m_cltip = GetLocalIP(m_svrip, m_svrport);
	m_fcltip = ToKoscomIP(m_cltip);
	
	m_maddr = GetMacAddress(m_cltip);

#ifdef FOR_WARSHIP
	m_Wizard.RunAxis(loginSHOP, (LONG)(LPCSTR)m_svrip, m_svrport);	//�׳� �α���
#else
	m_Wizard.RunAxis(loginAXIS, (LONG)(LPCSTR)m_svrip, m_svrport);	//�÷������� ���� ������Ʈ�ϰ� �α���
#endif

// 	CString strTemp;
// 	strTemp.Format("[KSJ]Login ip[%s] port[%d]", m_svrip, m_svrport);
// 	OutputDebugString(strTemp);
	return TRUE;
}

CString CIBKSConnectorCtrl::ToKoscomIP(CString addr)
{
	if (addr.IsEmpty()) return "";

	CString cls[4], naddr;
	int st=0, pos=0;
	pos = addr.Find(".", st); cls[0] = addr.Mid(st, pos-st); st = pos+1;
	pos = addr.Find(".", st); cls[1] = addr.Mid(st, pos-st); st = pos+1;
	pos = addr.Find(".", st); cls[2] = addr.Mid(st, pos-st); st = pos+1;
	cls[3] = addr.Mid(st);
	naddr.Format("%03d%03d%03d%03d", atoi(cls[0]), atoi(cls[1]), atoi(cls[2]), atoi(cls[3]));
	return naddr;
}

void CIBKSConnectorCtrl::CheckValidUser()
{
	S_PIDOUINI();
}

BSTR CIBKSConnectorCtrl::GetLastErrMsg() 
{
	return m_sLastMsg.AllocSysString();
}

BOOL CIBKSConnectorCtrl::S_PIDOUINI()
{
	/*struct pidouini_mid mid;
	ZeroMemory(&mid, L_pidouini_mid);
	mid.gubn[0] = 'Q';
	sprintf(mid.item.usid, "%s", (LPCSTR)m_usid);
	sprintf(mid.item.innm, "IBKSCONNECTOR");
	sprintf(mid.item.senm, "ALLOW_USER");
	sprintf(mid.item.skey, "ENABLE");*/

	struct pidouini_mid* pmid;
	char* pdata = new char[L_pidouini_mid];
	memset(pdata, ' ', L_pidouini_mid);
	pmid = (struct pidouini_mid*)pdata;

	pmid->gubn[0] = 'Q';
	memcpy(pmid->item.usid, m_usid, m_usid.GetLength());
	memcpy(pmid->item.innm, "IBKSCONNECTOR", 13);
	memcpy(pmid->item.senm, "ALLOW_USER", 10);
	memcpy(pmid->item.skey, "ENABLE", 6);
	//memcpy(pmid->item.date, "123456", 6);

	//return SendTR("pidouini", 1, 0, (LPCSTR)&mid, L_pidouini_mid, C_PIDOUINI);  //vc2019
	return  SendTR("pidouini", 1, 0, (LPCSTR)pmid, L_pidouini_mid, &CIBKSConnectorCtrl::C_PIDOUINI);
}

char* CIBKSConnectorCtrl::GetSHA256(char* data, int dataL,HMODULE hModule,bool isfile)
{
	//HMODULE hModule = LoadLibrary("CX_SHA256.DLL");
	
	char* sha256;
	
	if (hModule)
	{
		if(!isfile)
		{
			typedef long (WINAPI *GETSHAFUNC)(char*, int);
			
			GETSHAFUNC func = (GETSHAFUNC)GetProcAddress(hModule, "axEncrypt");
			if (func)
			{
				sha256 =  (char*)func(data,dataL);
			}
		}
		else
		{
			typedef long (WINAPI *GETSHAFUNC)(char*);
			
			GETSHAFUNC func = (GETSHAFUNC)GetProcAddress(hModule, "axFileEncrypt");
			if (func)
			{
				sha256 =  (char*)func(data);
			}
		}
		
		//FreeLibrary(hModule);
	}
	
	return sha256;
}

char* CIBKSConnectorCtrl::GetFileSHA256( CString sPath, HMODULE hModule)
{
	char* sha256;
	
	CFileFind finder;
	
	if (finder.FindFile(sPath))
	{
		TRY
		{
			sha256 = GetSHA256((LPSTR)(LPCTSTR)sPath,0,hModule,TRUE);
		}
		CATCH (CFileException, e) 
		{
		}
		END_CATCH;
	}
	else
	{
		return NULL;
	}
	
	finder.Close();
	
	return sha256;
}

BOOL CIBKSConnectorCtrl::S_PIDOVERS()
{
	struct	pihoitgy_mid	mid;
	
	FillMemory((char*) &mid, sizeof(pihoitgy_mid), ' ');

	HMODULE hModule = LoadLibrary("CX_SHA256.DLL");

	CString filename;

	if(hModule)
	{
		filename.Format("%s\\exe\\IBKSCONNECTOR.OCX", m_home);

		char* str = GetFileSHA256(filename,hModule);

		CString sdat;
		
		CString strCnt,strMsg;

		CopyMemory(mid.gubn,"2",sizeof(mid.gubn));
		sprintf(mid.nrec, "%04d", 1);			// �ݺ�Ƚ��

		CopyMemory(mid.grid2[0].gubn,"EXE",3);
		CopyMemory(mid.grid2[0].mnam,"IBKSCONNECTOR.OCX",17);
		CopyMemory(mid.grid2[0].skey,str,strlen(str));
		
		//CopyMemory(mid.grid2,(char*)&grid,sizeof(mid.grid2));
	}
	
//	return SendTR("pihoitgy", 1, 0, (LPCSTR)&mid, L_pihoitgy_mid, C_PIDOVERS);  //vc2019
	return SendTR("pihoitgy", 1, 0, (LPCSTR)&mid, L_pihoitgy_mid, &CIBKSConnectorCtrl::C_PIDOVERS);
}

void CIBKSConnectorCtrl::C_PIDOVERS(WPARAM wParam, LPARAM lParam)
{
	int key = LOWORD(wParam);
	int len = HIWORD(wParam);

	pihoitgy_mod* mod = (struct pihoitgy_mod*) lParam;
	int	cnt = atoi(CString(mod->nrec, sizeof(mod->nrec)));
	//AfxMessageBox(CString(mod->nrec, sizeof(mod->nrec)));

	if(cnt > 0)
	{
		m_bLogin = false;
		FireOnLogin(false,"�������� OCX ������ �ƴմϴ�.������ OCX�� ����� �ּ���.");

		m_Wizard.RunAxis(loginAXIS, 0, -1);
	}
	else
	{
		m_bLogin = true;
		//FireOnLogin(true,"����Ȯ�οϷ�");
	}
}

void CIBKSConnectorCtrl::C_PIDOUINI(WPARAM wParam, LPARAM lParam)
{
	struct pidouini_mod *mod = (struct pidouini_mod*)lParam;
	CString val = CString(mod->item.valu, 4);
	val.MakeUpper();

	if(IsPTS())		//2012.08.08 PTS�� ����ڵ�� �˻����� �����Ѵ�.
		val = "TRUE";

	if (val=="TRUE")
	{
		// ����ڸ� ����
		m_usnm = (LPCSTR)Variant(nameCC, 0);
		
		// Ȩ���丮 ����
		m_home = (LPCSTR)Variant(homeCC, 0);

		//2012.08.16 KSJ  0�̸� �α����� ������� 1�̸� �α����� ����
		LoadIni();

		// ���¹�ȣ ����
		m_acno.clear();
		int pos=0, ed;
		CString acno = (LPCSTR)Variant(accountCC, 0);

		if(m_bSaveLog)
			testSaveFile("accountCC", acno, acno.GetLength());

		if (acno.GetLength()>=11)
		{
			do {
				ed = acno.Find("\n", pos);
				CString acc = acno.Mid(pos, ed-pos);
				m_acno.push_back( acc.Mid(0, 11) );
				m_acnm.push_back( acc.Mid(12, acc.Find("|", 12)-12) );
				pos = ed+1;

			} while(pos<acno.GetLength());
		}

		// ����� ID - 1���� ���μ������� ���� �ν��Ͻ��� ������ ��� userCC�� ���������θ� ���´�. (static�ε�)
		//m_usid = (LPCSTR)Variant(userCC, 0);
		
#ifdef FOR_WARSHIP
		// for warship
		InitJanGo();
		return;
#endif

		// ���¾�����ȸ
		m_chk_acno.clear();
		if(!IsPTS())
		{
//	�������¸� ��ȸ�ϴ� ���� : SACEQ320
// 			vector<char> data(L_ledger+L_saceq320_mid);
// 
// 			CTime time = CTime::GetCurrentTime();
// 			CString today = time.Format("%Y%m%d");
// 			struct ledger  *ledger = (struct ledger*)&data[0];
// 			GetLedger(ledger);
// 			memcpy(ledger->svcd, "SACEQ320", 8);
// 			memcpy(ledger->fkey, "C   ", 4);
// 			FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
// 			memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
// 			memcpy(ledger->brno, "000", 3);
// 			memcpy(ledger->rcnt, "0015", 4);
// 
// 			struct saceq320_mid *mid = (struct saceq320_mid*)&data[L_ledger];
// 
// 			FillMemory(mid, L_saceq320_mid, ' ');
// 			CopyMemory(mid->In, "00001", 5);
// 			CopyMemory(mid->fnUserId, m_usid, 16);
// 			CopyMemory(mid->servCode, "3056", 4);
// 
// 			SendTR("piboPBxQ", 100, US_ENC, (LPCSTR)&data[0], data.size(), C_SACEQ320);
//	���� ������������ üũ�ϴ� ����
			vector<char> data(L_ledger+L_sacaq504_mid);
			
			CTime time = CTime::GetCurrentTime();
			CString today = time.Format("%Y%m%d");
			struct ledger  *ledger = (struct ledger*)&data[0];
			GetLedger(ledger);
			memcpy(ledger->svcd, "SACAQ504", 8);
			memcpy(ledger->fkey, "C   ", 4);
			FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
			memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
			memcpy(ledger->brno, "000", 3);
			memcpy(ledger->rcnt, "0015", 4);

			struct sacaq504_mid *mid = (struct sacaq504_mid*)&data[L_ledger];
			for(int n=0; n<m_acno.size(); ++n)
			{
				FillMemory(mid, L_sacaq504_mid, ' ');
				CopyMemory(mid->In, "00001", 5);
				CopyMemory(mid->QryTp, "2", 1);
				CopyMemory(mid->SvcKindCode, SYSTRADING_SVCCODE, 4);
				
				switch( atoi(m_acno[n].Mid(3, 2)) )
				{
				case 10: case 11:
					CopyMemory(mid->PdptnCode, "01", 2); break;
				case 20: case 21:
					CopyMemory(mid->PdptnCode, "03", 2); break;
				default:
					continue;
				}

				if(m_bSaveLog)
					testSaveFile("sacaq504_mid", m_acno[n], 11);

				m_chk_acno[n] = m_acno[n];
				
				CopyMemory(mid->QrySrtDt, "20110401", 8);
				CopyMemory(mid->QryEndDt, today, today.GetLength());
				CopyMemory(mid->RegTp, "0", 1);
				CopyMemory(mid->AcntNo, (LPCSTR)m_acno[n], m_acno[n].GetLength());
				
				//SendTR("piboPBxQ", n, US_ENC, (LPCSTR)&data[0], data.size(), C_SACAQ504);
				SendTR("piboPBxQ", n, US_ENC, (LPCSTR)&data[0], data.size(), &CIBKSConnectorCtrl::C_SACAQ504);  //vc2019

				// �ʹ� ����TR�� �ѹ��� �ø��� �����.
				Sleep(200);
			}
		}
		else	//2012.09.13 KSJ �ý��۸Ÿ� �����̼� �� �������Ȯ�� PTS�϶��� üũ�ϸ� �ȵ�
		{
			InitJanGo();    
		}

		//S_PIDOVERS();
	}
	else
	{
		m_bLogin = false;
		FireOnLogin(false, "���� ����ڰ� �ƴմϴ�.");
	}
}

BSTR CIBKSConnectorCtrl::GetUserName() 
{
	if (IsInit() && IsLogin())
		return m_usnm.AllocSysString();
	else
		return CString("").AllocSysString();
}

BSTR CIBKSConnectorCtrl::GetHome() 
{
	if (IsInit() && IsLogin())
		return m_home.AllocSysString();
	else
		return CString("").AllocSysString();
}

BSTR CIBKSConnectorCtrl::GetUserID() 
{
	if (IsInit() && IsLogin())
		return m_usid.AllocSysString();
	else
		return CString("").AllocSysString();
}

void CIBKSConnectorCtrl::DEF_CALLBACK(WPARAM wParam, LPARAM lParam)
{
	int key = LOWORD(wParam);
	int size = HIWORD(wParam);

	if(m_bSaveLog)
		testSaveFile(key, (char*)lParam, size);

	FireOnRecvData(key, (long)lParam, size, false, "");
}

void CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK(WPARAM wParam, LPARAM lParam)
{
// 	OutputDebugString("[KSJ] DEF_KOSCOM_CALLBACK");

	char nID[8]={0,};
	char nkey[19]={0,};
	char zErrMsg[99]={0,};

	struct ledger *ledg = (struct ledger *)lParam;

	BOOL bNext = (ledg->next[0]=='Y');
	if (bNext)
		memcpy(nkey, ledg->nkey, sizeof(ledg->nkey));

	ledg->emsg[98] = '\0';
	memcpy(zErrMsg, ledg->emsg, sizeof(ledg->emsg));

	int key = LOWORD(wParam);
	int size = HIWORD(wParam)-L_ledger;
	LPCSTR data = (LPCSTR)(lParam+L_ledger);

	//�Էµ����� ���˿���
	if(strlen(ledg->ecod))
		FireOnGuideMsg(key, zErrMsg);
	
	if(m_bSaveLog)
		testSaveFile(key, data, size);

	FireOnRecvData(key, (long)data, size, bNext, nkey);

	//2012.06.13 KSJ ID�� �����ؼ� �̺�Ʈ�� ���� ����
	memcpy(nID, ledg->csym, sizeof(ledg->csym));
	int id = atoi(nID);

// 	CString strTemp;
// 	strTemp.Format("[KSJ]nID[%s]id[%d]ledg->csym[%s]", nID, id, ledg->csym);
// 	OutputDebugString(strTemp);
	
	FireOnRecvDataID(key, (long)data, size, bNext, nkey, id);
	//KSJ
}

bool CIBKSConnectorCtrl::IsValidAccount(LPCSTR acno)
{
	bool bRet = (m_acno.end()!=find(m_acno.begin(), m_acno.end(), acno));
	if (!bRet)
		m_sLastMsg = "�ùٸ� ���¹�ȣ�� �Է��Ͻʽÿ�.";
	return bRet;
}

//2012.08.17 KSJ ��ü���°� ���հ������� Ȯ��, ���࿹���� ����� �������� Ȯ��
bool CIBKSConnectorCtrl::IsValidSendAccount(LPCSTR acno)
{
	bool bRet = true;
	CString strAnco = acno;

	if(strAnco.GetLength() < 11)
	{
		m_sLastMsg = "�ùٸ� ���¹�ȣ�� �Է��Ͻʽÿ�.";
		bRet = false;
	}
	else
	{
		if(strAnco.Mid(3, 2) == "00")
		{
			m_sLastMsg = "���հ��¹�ȣ(ex.001-00-123456)�� ��ü�� �Ұ��մϴ�.\n�������¹�ȣ(ex.001-10-123456)�� �Է��Ͽ� �ֽʽÿ�.";
			bRet = false;
		}
		else if(atoi(strAnco.Mid(0, 3)) >= 551 && atoi(strAnco.Mid(0, 3)) <= 555)
		{
			m_sLastMsg = "IBK�ֽ������������δ� ��ü�� �Ұ��մϴ�.";
			bRet = false;
		}
	}

	return bRet;
}

bool CIBKSConnectorCtrl::IsValidState()
{
	if (!IsInit())
	{
		m_sLastMsg = "�ʱ�ȭ���� �ʾҽ��ϴ�.";
		return false;
	}

	if (!IsLogin())
	{
		m_sLastMsg = "�α��� �� �̿��Ͻñ� �ٶ��ϴ�.";
		return false;
	}

	return true;
}

bool CIBKSConnectorCtrl::IsValidAcnoPswd(LPCSTR pswd)
{
	if (strlen(pswd)!=4)
	{
		m_sLastMsg = "���º�й�ȣ�� �Է��Ͻʽÿ�.";
		return false;
	}
	else return true;
}

bool CIBKSConnectorCtrl::IsValidPTS()
{
	if (!IsPTS())
	{
		m_sLastMsg = "�������� ���� TR �Դϴ�.";
		return false;
	}
	else return true;
}

BOOL CIBKSConnectorCtrl::S_PIBOFODR(int key, int mmgb, LPCSTR acno, LPCSTR pswd, int ojno, LPCSTR fcod, int jqty, int jprc, int hogb, int mdgb )
{
	if (!IsValidState())		return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;

	if (mmgb<1 || mmgb>4)
	{
		m_sLastMsg = "�Ÿű��� �Է°��� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	
	if (m_fhoga.find(hogb)==m_fhoga.end()) 
	{
		m_sLastMsg = "ȣ������ �Է°��� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;	
	}
	if (mdgb<0 || mdgb>2) return FALSE;
	if (strlen(fcod)!=8)
	{
		m_sLastMsg = "�����ڵ尡 �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	if (jqty<0) 
	{
		m_sLastMsg = "�ֹ������� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	if (jqty==0 && (mmgb==1 || mmgb==2))
	{
		m_sLastMsg = "�ֹ������� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	if ((mmgb==3 || mmgb==4) && ojno==0)
	{
		m_sLastMsg = "���ֹ���ȣ�� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	
	vector<char> buff( L_ledger + L_tr3201_mid );
	memset(&buff[0], ' ', buff.size());
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct tr3201_mid *mid = (struct tr3201_mid*)&buff[L_ledger];
	
	//  Ledger ����
	GetLedger(ledger);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0000", 4);
	memcpy(ledger->fkey, "5   ", 4);
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());

	// �ֹ� ����
	memcpy(mid->gubn, "^C", 2);				//^C 2014.04.21 KSJ ����/������ ������ �߰�
	sprintf(mid->nrec, "%04d", 1);			// �ݺ�Ƚ��
	mid->odgb[0] = '2';						// ����
	mid->mmgb[0] = '0' + mmgb;				// �Ÿű���
	memcpy(mid->accn, acno, 11);				// ���¹�ȣ
	//memcpy(mid->pswd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->pswd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	sprintf(mid->ojno, "%012d", ojno);		// ���ֹ���ȣ
	sprintf(mid->fcod, "%.8s", fcod);		// �����ڵ�
	sprintf(mid->jqty, "%08d", jqty);		// �ֹ�����
	sprintf(mid->jprc, "%010d", jprc);		// �ֹ�����
	sprintf(mid->hogb, "%02d", hogb%10);	// ȣ������
	sprintf(mid->cdgb, "%d", hogb/10);		// IOC/FOK
	sprintf(mid->mdgb, "%d", mdgb);			// �������
	mid->prgb[0] = 'X';
	sprintf(mid->pggb, "%02d", m_nPggb == 0 ? 3:1);	// ���α׷�(00:�Ϲ� 01:���� 03:������) 2014.04.21 �߰�
													// ������ �������� ����. 03�Ϲ�, 01����

	//return SendTR("pibofodr", key, US_CA|US_KEY, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	return SendTR("pibofodr", key, US_CA | US_KEY, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
}

//2012.06.04 KSJ �ֹ��� ���������ID �߰�
BOOL CIBKSConnectorCtrl::S_PIBOFODR_ID(int key, int mmgb, LPCSTR acno, LPCSTR pswd, int ojno, LPCSTR fcod, int jqty, int jprc, int hogb, int mdgb, int id )
{
	if (!IsValidState())		return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;
	
	if (mmgb<1 || mmgb>4)
	{
		m_sLastMsg = "�Ÿű��� �Է°��� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	
	if (m_fhoga.find(hogb)==m_fhoga.end()) 
	{
		m_sLastMsg = "ȣ������ �Է°��� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;	
	}
	if (mdgb<0 || mdgb>2) return FALSE;
	if (strlen(fcod)!=8)
	{
		m_sLastMsg = "�����ڵ尡 �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	if (jqty<0) 
	{
		m_sLastMsg = "�ֹ������� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	if (jqty==0 && (mmgb==1 || mmgb==2))
	{
		m_sLastMsg = "�ֹ������� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	if ((mmgb==3 || mmgb==4) && ojno==0)
	{
		m_sLastMsg = "���ֹ���ȣ�� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	
	vector<char> buff( L_ledger + L_tr3201_mid );
	memset(&buff[0], ' ', buff.size());
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct tr3201_mid *mid = (struct tr3201_mid*)&buff[L_ledger];
	
	//  Ledger ����
	GetLedger(ledger);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0000", 4);
	memcpy(ledger->fkey, "5   ", 4);
	CString strTemp;
	strTemp.Format("%08d", id);
	CopyMemory(ledger->csym, strTemp, strTemp.GetLength());		// 2012.06.04 KSJ ����� ���� ID �߰�
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());

	// �ֹ� ����
	memcpy(mid->gubn, "^C", 2);				//^C 2014.04.21 KSJ ����/������ ������ �߰�
	sprintf(mid->nrec, "%04d", 1);			// �ݺ�Ƚ��
	mid->odgb[0] = '2';						// ����
	mid->mmgb[0] = '0' + mmgb;				// �Ÿű���
	memcpy(mid->accn, acno, 11);				// ���¹�ȣ
	//memcpy(mid->pswd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->pswd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	sprintf(mid->ojno, "%012d", ojno);		// ���ֹ���ȣ
	sprintf(mid->fcod, "%.8s", fcod);		// �����ڵ�
	sprintf(mid->jqty, "%08d", jqty);		// �ֹ�����
	sprintf(mid->jprc, "%010d", jprc);		// �ֹ�����
	sprintf(mid->hogb, "%02d", hogb%10);	// ȣ������
	sprintf(mid->cdgb, "%d", hogb/10);		// IOC/FOK
	sprintf(mid->mdgb, "%d", mdgb);			// �������
	mid->prgb[0] = 'X';
	sprintf(mid->pggb, "%02d", m_nPggb == 0 ? 3:1);	// ���α׷�(00:�Ϲ� 01:���� 03:������) 2014.04.21 �߰�
	// ������ �������� ����. 03�Ϲ�, 01����
	
	//return SendTR("pibofodr", key, US_CA|US_KEY, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	return SendTR("pibofodr", key, US_CA | US_KEY, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
}
//KSJ

BOOL CIBKSConnectorCtrl::S_PIBOSODR(int key, int mmgb, LPCSTR acno, LPCSTR pswd, int ojno, LPCSTR fcod, int jqty, int jprc, int hogb, int mdgb )
{
	if (!IsValidState())		return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;

	if (mmgb<1 || mmgb>4)
	{
		m_sLastMsg = "�Ÿű��� �Է°��� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	
	if (m_shoga.find(hogb)==m_shoga.end()) 
	{
		m_sLastMsg = "ȣ������ �Է°��� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;	
	}
	if (mdgb<0 || mdgb>2) return FALSE;
	if (strlen(fcod)<7)
	{
		m_sLastMsg = "�����ڵ尡 �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	if (jqty<0) 
	{
		m_sLastMsg = "�ֹ������� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	if (jqty==0 && (mmgb==1 || mmgb==2))
	{
		m_sLastMsg = "�ֹ������� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	if ((mmgb==3 || mmgb==4) && ojno==0)
	{
		m_sLastMsg = "���ֹ���ȣ�� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	
	vector<char> buff( L_ledger + L_tr1201_mid );
	memset(&buff[0], ' ', buff.size());
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct tr3201_mid *mid = (struct tr3201_mid*)&buff[L_ledger];
	
	//  Ledger ����
	GetLedger(ledger);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0000", 4);
	memcpy(ledger->fkey, "5   ", 4);

	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());

	// �ֹ� ����
	memcpy(mid->gubn, "^C", 2);				//^C 2014.04.21 KSJ ����/������ ������ �߰�
	sprintf(mid->nrec, "%04d", 1);			// �ݺ�Ƚ��
	mid->odgb[0] = '1';						// ����
	mid->mmgb[0] = '0' + mmgb;				// �Ÿű���
	memcpy(mid->accn, acno, 11);				// ���¹�ȣ
	//memcpy(mid->pswd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->pswd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	sprintf(mid->ojno, "%012d", ojno);		// ���ֹ���ȣ
	sprintf(mid->fcod, "%.8s", fcod);		// �����ڵ�
	sprintf(mid->jqty, "%08d", jqty);		// �ֹ�����
	sprintf(mid->jprc, "%010d", jprc);		// �ֹ�����
	if (hogb==61 || hogb==81)
	{
		sprintf(mid->hogb, "%02d", hogb);		// ȣ������
		sprintf(mid->cdgb, "%d", 0);		// IOC/FOK
	}
	else
	{
		sprintf(mid->hogb, "%02d", hogb%10);		// ȣ������
		sprintf(mid->cdgb, "%d", hogb/10);		// IOC/FOK
	}
	sprintf(mid->mdgb, "%d", mdgb);			// �������
	mid->prgb[0] = 'X';
	sprintf(mid->pggb, "%02d", m_nPggb);	// ���α׷�(00:�Ϲ� 01:���� 03:������) 2014.04.21 �߰�

	//return SendTR("pibosodr", key, US_CA|US_KEY, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	return SendTR("pibosodr", key, US_CA | US_KEY, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
}

//2012.06.04 KSJ �ֹ��� ���������ID �߰�
BOOL CIBKSConnectorCtrl::S_PIBOSODR_ID(int key, int mmgb, LPCSTR acno, LPCSTR pswd, int ojno, LPCSTR fcod, int jqty, int jprc, int hogb, int mdgb, int id )
{
	if (!IsValidState())		return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;
	
	if (mmgb<1 || mmgb>4)
	{
		m_sLastMsg = "�Ÿű��� �Է°��� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	
	if (m_shoga.find(hogb)==m_shoga.end()) 
	{
		m_sLastMsg = "ȣ������ �Է°��� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;	
	}
	if (mdgb<0 || mdgb>2) return FALSE;
	if (strlen(fcod)<7)
	{
		m_sLastMsg = "�����ڵ尡 �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	if (jqty<0) 
	{
		m_sLastMsg = "�ֹ������� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	if (jqty==0 && (mmgb==1 || mmgb==2))
	{
		m_sLastMsg = "�ֹ������� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	if ((mmgb==3 || mmgb==4) && ojno==0)
	{
		m_sLastMsg = "���ֹ���ȣ�� �ùٸ��� �ʽ��ϴ�.";
		return FALSE;
	}
	
	vector<char> buff( L_ledger + L_tr1201_mid );
	memset(&buff[0], ' ', buff.size());
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct tr3201_mid *mid = (struct tr3201_mid*)&buff[L_ledger];
	
	//  Ledger ����
	GetLedger(ledger);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0000", 4);
	memcpy(ledger->fkey, "5   ", 4);

	CString strTemp;
	strTemp.Format("%08d", id);
	CopyMemory(ledger->csym, strTemp, strTemp.GetLength());		// 2012.06.04 KSJ ����� ���� ID �߰�

	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());

// 	CString strTemp;
// 	strTemp.Format("[KSJ][%s]", ledger->csym);
// 	OutputDebugString(strTemp);

	// �ֹ� ����
	memcpy(mid->gubn, "^C", 2);				//^C 2014.04.21 KSJ ����/������ ������ �߰�
	sprintf(mid->nrec, "%04d", 1);			// �ݺ�Ƚ��
	mid->odgb[0] = '1';						// ����
	mid->mmgb[0] = '0' + mmgb;				// �Ÿű���
	memcpy(mid->accn, acno, 11);				// ���¹�ȣ
	//memcpy(mid->pswd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->pswd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	sprintf(mid->ojno, "%012d", ojno);		// ���ֹ���ȣ
	sprintf(mid->fcod, "%.8s", fcod);		// �����ڵ�
	sprintf(mid->jqty, "%08d", jqty);		// �ֹ�����
	sprintf(mid->jprc, "%010d", jprc);		// �ֹ�����
	if (hogb==61 || hogb==81)
	{
		sprintf(mid->hogb, "%02d", hogb);		// ȣ������
		sprintf(mid->cdgb, "%d", 0);		// IOC/FOK
	}
	else
	{
		sprintf(mid->hogb, "%02d", hogb%10);		// ȣ������
		sprintf(mid->cdgb, "%d", hogb/10);		// IOC/FOK
	}
	sprintf(mid->mdgb, "%d", mdgb);			// �������
	mid->prgb[0] = 'X';
	sprintf(mid->pggb, "%02d", m_nPggb);	// ���α׷�(00:�Ϲ� 01:���� 03:������) 2014.04.21 �߰�

	//return SendTR("pibosodr", key, US_CA|US_KEY, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	return SendTR("pibosodr", key, US_CA | US_KEY, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
}
//KSJ

BOOL CIBKSConnectorCtrl::S_PIBO3002(int key, LPCSTR fcod)
{
	if (strlen(fcod)!=8)
	{
		m_sLastMsg = "�����ڵ带 Ȯ���ϼ���.";
		return FALSE;
	}
	//return SendTR("pibo3002", key, 0, fcod, 8, DEF_CALLBACK);  //vc2019
	return SendTR("pibo3002", key, 0, fcod, 8, &CIBKSConnectorCtrl::DEF_CALLBACK);
}

BOOL CIBKSConnectorCtrl::S_PIBOFCHG(int key, LPCSTR acno, LPCSTR pswd, int dlgb, int sort, LPCSTR code, int cpgb, int dsgb, LPCSTR nkey)
{
	if (!IsValidState())		return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd)) return FALSE;
 
	if (dlgb<0 || dlgb>2) { m_sLastMsg="��ȸ���а��� Ȯ���ϼ���.[dlgb]"; return FALSE; }
	if (sort<1 || sort>2) { m_sLastMsg="��ȸ���а��� Ȯ���ϼ���.[sort]"; return FALSE; }
	if (dsgb<0 || dsgb>2) { m_sLastMsg="��ȸ���а��� Ȯ���ϼ���.[dsgb]"; return FALSE; }
	
	switch(cpgb)
	{
	case 1:	cpgb = 'F'-'0'; break;
	case 2:	cpgb = 'C'-'0'; break;
	case 3:	cpgb = 'P'-'0'; break;
	case 4:	cpgb = 'S'-'0'; break;
	case 5:	cpgb = 'X'-'0'; break;
	}
	
	vector<char> buff(L_ledger+L_tr3211_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct tr3211_mid *mid = (struct tr3211_mid*)&buff[L_ledger];
	
	//  Ledger ����
	GetLedger(ledger);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0000", 4);
	if (strlen(nkey)==0)
		memcpy(ledger->fkey, "C   ", 4);
	else
	{
		ledger->next[0] = 'Y';
		memcpy(ledger->fkey, "7   ", 4);
		CString strTemp;
		strTemp.Format("%s", nkey);
		CopyMemory(ledger->nkey, strTemp, strTemp.GetLength());
	}
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->acno, acno, 11);
	//memcpy(mid->pswd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->pswd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	mid->dlgb[0] = '0' + dlgb;
	mid->sort[0] = '0' + sort;
	if (strlen(code)==8)
		memcpy(mid->cod2, code, 8);
	mid->cpgb[0] = '0' + cpgb;
	mid->dsgb[0] = '0' + dsgb;
	mid->dllf[0] = '1';

	//return SendTR("pihofchg", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK); //vc2019
	return SendTR("pihofchg", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
}

BOOL CIBKSConnectorCtrl::S_PIBOFJGO( int key, LPCSTR acno, LPCSTR pswd, LPCSTR nkey )
{
	if (!IsValidState())		return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;
	
	vector<char> buff(L_ledger+L_tr3221_mid, ' ');
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct tr3221_mid *mid = (struct tr3221_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SONBQ021", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0030", 4);
	if (strlen(nkey)==0)
		memcpy(ledger->fkey, "C   ", 4);
	else
	{
		ledger->next[0] = 'Y';
		memcpy(ledger->fkey, "7   ", 4);
		CString strTemp;
		strTemp.Format("%s", nkey);
		CopyMemory(ledger->nkey, strTemp, strTemp.GetLength());
	}
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->acno, acno, 11);
	//memcpy(mid->pswd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->pswd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	
	//return SendTR("pibofjgo", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	return SendTR("pibofjgo", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
}

BOOL CIBKSConnectorCtrl::S_PIBO5101(LONG key, LPCTSTR code)
{
	if (!IsValidState())		return FALSE;

	int slen = strlen(code);
	if (slen%6!=0)
	{
		m_sLastMsg = "�����ڵ带 Ȯ���ϼ���.";
		return FALSE;
	}
	if (slen>600)
	{
		m_sLastMsg = "100���� �̻��� ��û�� �� �����ϴ�.";
		return FALSE;
	}

	int nrec = slen/6;
	vector<char> buff(4+slen);
	sprintf(&buff[0], "%04d%.*s", nrec, slen, code);
	
	//return SendTR("pibo5101", key, 0, &buff[0], buff.size(), DEF_CALLBACK);  //vc2019
	return SendTR("pibo5101", key, 0, &buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_CALLBACK);
}

BOOL CIBKSConnectorCtrl::S_PIBO5102(LONG key, LPCTSTR code)
{
	if (!IsValidState())		return FALSE;

	int slen = strlen(code);
	if (slen%8!=0)
	{
		m_sLastMsg = "�����ڵ带 Ȯ���ϼ���.";
		return FALSE;
	}
	if (slen>800)
	{
		m_sLastMsg = "100���� �̻��� ��û�� �� �����ϴ�.";
		return FALSE;
	}
	
	int nrec = slen/8;
	vector<char> buff(4+slen);
	sprintf(&buff[0], "%04d%.*s", nrec, slen, code);
	
	//return SendTR("pibo5102", key, 0, &buff[0], buff.size(), DEF_CALLBACK);  //vc2019
	return SendTR("pibo5102", key, 0, &buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_CALLBACK);
}

BOOL CIBKSConnectorCtrl::S_PIBO5103(LONG key, LPCTSTR code)
{
	if (!IsValidState())		return FALSE;
	
	int slen = strlen(code);
	if (slen%4!=0)
	{
		m_sLastMsg = "�����ڵ带 Ȯ���ϼ���.";
		return FALSE;
	}
	if (slen>40)
	{
		m_sLastMsg = "10���� �̻��� ��û�� �� �����ϴ�.";
		return FALSE;
	}
	
	int nrec = slen/4;
	vector<char> buff(4+slen);
	sprintf(&buff[0], "%04d%.*s", nrec, slen, code);
	
	//return SendTR("pibo5103", key, 0, &buff[0], buff.size(), DEF_CALLBACK);  //vc2019
	return SendTR("pibo5103", key, 0, &buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_CALLBACK);
}

BOOL CIBKSConnectorCtrl::S_PIBO5104(LONG key, LPCTSTR code)
{
	if (!IsValidState())		return FALSE;
	
	int slen = strlen(code);
	if (slen%4!=0)
	{
		m_sLastMsg = "�����ڵ带 Ȯ���ϼ���.";
		return FALSE;
	}
	
	int nrec = slen/4;
	vector<char> buff(4+slen);
	sprintf(&buff[0], "%04d%.*s", nrec, slen, code);
	
	//return SendTR("pibo5104", key, 0, &buff[0], buff.size(), DEF_CALLBACK);  //vc2019
	return SendTR("pibo5104", key, 0, &buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_CALLBACK);
}

BOOL CIBKSConnectorCtrl::S_PIBO1003(int key, LPCSTR jcod)
{
	if (!IsValidState())		return FALSE;
	
	if (strlen(jcod)!=6)
	{
		m_sLastMsg = "�����ڵ带 Ȯ���ϼ���.";
		return FALSE;
	}

	//return SendTR("pibo1003", key, 0, jcod, 6, DEF_CALLBACK); //vc2019
	return SendTR("pibo1003", key, 0, jcod, 6, &CIBKSConnectorCtrl::DEF_CALLBACK);
}

BOOL CIBKSConnectorCtrl::S_SONBQ504(int key, LPCSTR acno, LPCSTR pswd, int zBalEvalTp, int zPrcEvalTp, LPCSTR nkey)
{
	if (!IsValidState())		return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;

	if (zBalEvalTp<1 || zBalEvalTp>2) return FALSE;
	if (zPrcEvalTp<1 || zPrcEvalTp>2) return FALSE;
	
	vector<char> buff(L_ledger+L_TR4202_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4202_mid *mid = (struct TR4202_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SONBQ504", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0009", 4);
	if (strlen(nkey)==0)
		memcpy(ledger->fkey, "C   ", 4);
	else
	{
		ledger->next[0] = 'Y';
		memcpy(ledger->fkey, "7   ", 4);
		CString strTemp;
		strTemp.Format("%s", nkey);
		CopyMemory(ledger->nkey, strTemp, strTemp.GetLength());
	}
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zInptPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zInptPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	memcpy(mid->zOrdDt, "00000000", 8);

	mid->zBalEvalTp[0] = '0' + zBalEvalTp;
	mid->zFutsPrcEvalTp[0] = '0' + zPrcEvalTp;
	mid->zLqdtQtyQryTp[0] = '1';

	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK); //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_PIBOSCHG(int key, LPCSTR acno, LPCSTR pswd, LPCSTR code, int dsgb, int sygb, int dlgb, int sort, LPCSTR nkey)
{
	if (!IsValidState())		return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd)) return FALSE;

	if (dsgb<0 || dsgb>2) { m_sLastMsg="��ȸ���а��� Ȯ���ϼ���.[dsgb]"; return FALSE; }
	if (sygb<0 || sygb>2) { m_sLastMsg="��ȸ���а��� Ȯ���ϼ���.[sygb]"; return FALSE; }
	if (dlgb<0 || dlgb>2) { m_sLastMsg="��ȸ���а��� Ȯ���ϼ���.[dlgb]"; return FALSE; }
	if (sort<1 || sort>2) { m_sLastMsg="��ȸ���а��� Ȯ���ϼ���.[sort]"; return FALSE; }

	vector<char> buff(L_ledger+L_tr1211_grid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct tr1211_mid *mid = (struct tr1211_mid*)&buff[L_ledger];
	
	//  Ledger ����
	GetLedger(ledger);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0000", 4);
	if (strlen(nkey)==0)
		memcpy(ledger->fkey, "C   ", 4);
	else
	{
		ledger->next[0] = 'Y';
		memcpy(ledger->fkey, "7   ", 4);
		CString strTemp;
		strTemp.Format("%s", nkey);
		CopyMemory(ledger->nkey, strTemp, strTemp.GetLength());
	}
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());

	// Data ����
	mid->gubn[0] = '>';	//2015.05.28 KSJ ���а� �߰�
	memcpy(mid->acno, acno, 11);
	//memcpy(mid->pswd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->pswd, "HEAD", 4);			// ���º��	2015.05.23 KSJ

	if (strlen(code)==6)
		memcpy(mid->cod2, code, 6);
	mid->dsgb[0] = '0' + dsgb;
	mid->sygb[0] = '0' + sygb;
	mid->dlgb[0] = '0' + dlgb;
	mid->sort[0] = '0' + sort;
	mid->dllf[0] = '1';
	
	//return SendTR("piboschg", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK); //vc2019
	return SendTR("piboschg", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
}

BOOL CIBKSConnectorCtrl::S_PIBOSJGO(int key, LPCSTR acno, LPCSTR pswd, int allf, LPCSTR nkey)
{
	if (!IsValidState())		return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;
	if (allf<0 || allf>2) { m_sLastMsg="��ȸ���а��� Ȯ���ϼ���.[allf]"; return FALSE; }

	vector<char> buff(L_ledger+L_tr1221_mid, ' ');
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct tr1221_mid *mid = (struct tr1221_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SONAQ052", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0030", 4);
	if (strlen(nkey)==0)
		memcpy(ledger->fkey, "C   ", 4);
	else
	{
		ledger->next[0] = 'Y';
		memcpy(ledger->fkey, "7   ", 4);
		CString strTemp;
		strTemp.Format("%s", nkey);
		CopyMemory(ledger->nkey, strTemp, strTemp.GetLength());
	}
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->acno, acno, 11);
	//memcpy(mid->pswd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->pswd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	mid->allf[0] = allf + '0';
	
	//return SendTR("pibosjgo", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	return SendTR("pibosjgo", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
}

BOOL CIBKSConnectorCtrl::S_SONBQ101(int key, LPCSTR acno, LPCSTR pswd, LPCSTR code, int zBnsTp, double dOrdPrc, int hogb, int zQryTp, double lOrdAmt, double dRatVal)
{
	if (!IsValidState())		return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;

	if (zBnsTp<1 || zBnsTp>2) { m_sLastMsg="��ȸ���а��� Ȯ���ϼ���.[mmgb]"; return FALSE; }
	if (hogb!=0 && hogb!=3 && hogb!=5 && hogb!=6) { m_sLastMsg="��ȸ���а��� Ȯ���ϼ���.[hogb]"; return FALSE; }
	if (zQryTp<1 || zQryTp>3) { m_sLastMsg="��ȸ���а��� Ȯ���ϼ���.[qtpy]"; return FALSE; }
	if (strlen(code)!=8) { m_sLastMsg="�����ڵ带 Ȯ���ϼ���.[code]"; return FALSE; }
	if (zQryTp==2) lOrdAmt = 0;
	if (zQryTp==3) dRatVal = 0;
	if (lOrdAmt<0) { m_sLastMsg="��ȸ���а��� Ȯ���ϼ���.[jamt]"; return FALSE; }
	if (dRatVal<0) { m_sLastMsg="��ȸ���а��� Ȯ���ϼ���.[jrat]"; return FALSE; }

	vector<char> buff(L_ledger+L_tr3231_mid+1, ' ');
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct tr3231_mid *mid = (struct tr3231_mid*)&buff[L_ledger];
	GetLedger(ledger);
	memcpy(ledger->svcd, "SONBQ101", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0000", 4);
	memcpy(ledger->fkey, "C   ", 4);

	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	mid->zQryTp[0] = '0' + zQryTp;
	
	sprintf(mid->lOrdAmt, "%+016.f", lOrdAmt);
	sprintf(mid->dRatVal, "%+019.8f", dRatVal);
	sprintf(mid->zFnoIsuNo, "%s", code);
	mid->zBnsTp[0] = '0' + zBnsTp;
	sprintf(mid->dOrdPrc, "%+013.2f", dOrdPrc);
	sprintf(mid->zFnoOrdprcPtnCode, "%.02d", hogb);
	//return SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size()-1, DEF_KOSCOM_CALLBACK);  //vc2019
	return SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size() - 1, &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
}

BOOL CIBKSConnectorCtrl::S_SONAQ200(int key, LPCSTR acno, LPCSTR pswd, LPCSTR code, int zBnsTp, double dOrdPrc)
{
	if (!IsValidState())		return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;
	
	if (zBnsTp<1 || zBnsTp>2) { m_sLastMsg="��ȸ���а��� Ȯ���ϼ���.[zBnsTp]"; return FALSE; }
	if (strlen(code)!=7) { m_sLastMsg="�����ڵ带 Ȯ���ϼ���.[Code]"; return FALSE; }
	if (dOrdPrc<0) { m_sLastMsg="��ȸ���а��� Ȯ���ϼ���.[dOrdPrc]"; return FALSE; }
	
	vector<char> buff(L_ledger+L_tr1231_mid+1, ' ');
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct tr1231_mid *mid = (struct tr1231_mid*)&buff[L_ledger];
	GetLedger(ledger);
	memcpy(ledger->svcd, "SONAQ200", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0000", 4);
	memcpy(ledger->fkey, "C   ", 4);
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	sprintf(mid->zIsuNo, "%s", code);
	mid->zBnsTp[0] = '0' + zBnsTp;
	sprintf(mid->dOrdPrc, "%+013.2f", dOrdPrc);
	//return SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size()-1, DEF_KOSCOM_CALLBACK);  //vc2019
	return SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size() - 1, &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
}

BOOL CIBKSConnectorCtrl::S_SSLBQ033(int key, LPCTSTR acno, LPCTSTR pswd, int fstp, int ertp) //�ɼ� ���ϼ��� ��ȸ KSJ 2013.04.08
{
	if (!IsValidState())		return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;
	
	if (fstp != 0 && fstp != 1) { m_sLastMsg="������� ���а��� Ȯ���ϼ���.[fstp]"; return FALSE; }
	if (ertp < 1 || ertp > 3) { m_sLastMsg="���ͷ� ���а��� Ȯ���ϼ���.[ertp]"; return FALSE; }
	
	CTime time = CTime::GetCurrentTime();
	CString today = time.Format("%Y%m%d");

	vector<char> buff(L_ledger+L_tr3232_mid+1, ' ');
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct tr3232_mid *mid = (struct tr3232_mid*)&buff[L_ledger];
	GetLedger(ledger);
	memcpy(ledger->svcd, "SSLBQ033", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0000", 4);
	memcpy(ledger->fkey, "C   ", 4);
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->zIn, "00001", 5);
	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	CopyMemory(mid->zSrtDt, today, today.GetLength());
	CopyMemory(mid->zEndDt, today, today.GetLength());
	mid->zFtsubtTp[0] = '0' + fstp;
	mid->zErnratTp[0] = '0' + ertp;
	//return SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size()-1, DEF_KOSCOM_CALLBACK);  //vc2019
	return SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size() - 1, &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
}

void CIBKSConnectorCtrl::Logout() 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (IsInit()/* && IsLogin()*/)
	{
		m_Wizard.RunAxis(loginAXIS, 0, -1);
		m_bLogin = false;
	}
	else
	{
		m_sLastMsg = "�α��λ��°� �ƴմϴ�.";
	}
}

BOOL CIBKSConnectorCtrl::TR1001(long key, LPCTSTR code) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR1001) ? S_PIBO1003(key, code) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR1201(long key, long mmgb, LPCTSTR acno, LPCTSTR pswd, long ojno, LPCTSTR code, long jqty, long jprc, long hogb, long mdgb) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (hogb==61 || hogb==81)	// �����ð�������, �ð������� �ֹ��� ��� ������ �޸��Ѵ�.
		return AccessTr(eTR1201_01) ? S_PIBOSODR(key, mmgb, acno, pswd, ojno, code, jqty, jprc, hogb, mdgb) : FALSE;
	else
		return AccessTr(eTR1201) ? S_PIBOSODR(key, mmgb, acno, pswd, ojno, code, jqty, jprc, hogb, mdgb) : FALSE;
}

//2012.06.04 KSJ �ֽ��ֹ��� ���������ID �߰�
BOOL CIBKSConnectorCtrl::TR1202(long key, long mmgb, LPCTSTR acno, LPCTSTR pswd, long ojno, LPCTSTR code, long jqty, long jprc, long hogb, long mdgb, long id) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (hogb==61 || hogb==81)	// �����ð�������, �ð������� �ֹ��� ��� ������ �޸��Ѵ�.
		return AccessTr(eTR1201_01) ? S_PIBOSODR_ID(key, mmgb, acno, pswd, ojno, code, jqty, jprc, hogb, mdgb, id) : FALSE;
	else
		return AccessTr(eTR1201) ? S_PIBOSODR_ID(key, mmgb, acno, pswd, ojno, code, jqty, jprc, hogb, mdgb, id) : FALSE;
}
//KSJ

BOOL CIBKSConnectorCtrl::TR1211(long key, LPCTSTR acno, LPCTSTR pswd, LPCTSTR code, long dsgb, long sygb, long dlgb, long sort, LPCTSTR nkey) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR1211) ? S_PIBOSCHG(key, acno, pswd, code, dsgb, sygb, dlgb, sort, nkey) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR1221(long key, LPCTSTR acno, LPCTSTR pswd, long allf, LPCTSTR nkey) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR1221) ? S_PIBOSJGO(key, acno, pswd, allf, nkey) : FALSE;
}

//2012.09.24 KSJ �ֽ� �ǽð� �ܰ�
BOOL CIBKSConnectorCtrl::TR1222(long key, LPCTSTR acno, LPCTSTR pswd, long allf) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!IsValidState())		return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;

	CString sData;
	sData.Format("%s\t%s", acno, pswd);
	
	m_strAcno = acno;

	m_Notify.GetWindow(GW_CHILD)->SendMessage(WM_ACCOUNT, MAKEWPARAM(this->m_hWnd, S_NOTIFY), (LPARAM)(LPCTSTR)sData);
	return TRUE;	
//	return AccessTr(eTR1222) ? S_PIBOSJGO(key, acno, pswd, allf, nkey) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR1231(long key, LPCTSTR acno, LPCTSTR pswd, LPCTSTR code, long mmgb, double jprc) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR1231) ? S_SONAQ200(key, acno, pswd, code, mmgb, jprc) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR3001(long key, LPCTSTR code) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR3001) ? S_PIBO3002(key, code) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR3201(long key, long mmgb, LPCTSTR acno, LPCTSTR pswd, long ojno, LPCTSTR code, long jqty, long jprc, long hogb, long mdgb) 
{
	return FALSE;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR3201) ? S_PIBOFODR(key, mmgb, acno, pswd, ojno, code, jqty, jprc, hogb, mdgb) : FALSE;
}

//2012.06.04 KSJ �����ֹ��� ���������ID �߰�
BOOL CIBKSConnectorCtrl::TR3202(long key, long mmgb, LPCTSTR acno, LPCTSTR pswd, long ojno, LPCTSTR code, long jqty, long jprc, long hogb, long mdgb, long id) 
{
	return FALSE;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR3201) ? S_PIBOFODR_ID(key, mmgb, acno, pswd, ojno, code, jqty, jprc, hogb, mdgb, id) : FALSE;
}
//KSJ

BOOL CIBKSConnectorCtrl::TR3211(long key, LPCTSTR acno, LPCTSTR pswd, long dlgb, long sort, LPCTSTR code, long cpgb, long dsgb, LPCTSTR nkey) 
{
	return FALSE;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR3211) ? S_PIBOFCHG(key, acno, pswd, dlgb, sort, code, cpgb, dsgb, nkey) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR3221(long key, LPCTSTR acno, LPCTSTR pswd, LPCTSTR nkey) 
{
	return FALSE;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	return AccessTr(eTR3221) ? S_PIBOFJGO(key, acno, pswd, nkey) : FALSE;
}

//2012.09.24 KSJ �����ɼ� �ǽð� �ܰ�
BOOL CIBKSConnectorCtrl::TR3222(long key, LPCTSTR acno, LPCTSTR pswd) 
{
	return FALSE;
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!IsValidState())		return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;

	CString sData;
	sData.Format("%s\t%s", acno, pswd);
	
	m_strAcno = acno;

	m_Notify.GetWindow(GW_CHILD)->SendMessage(WM_ACCOUNT, MAKEWPARAM(this->m_hWnd, F_NOTIFY), (LPARAM)(LPCTSTR)sData);
	return TRUE;	
// 	return AccessTr(eTR3222) ? S_PIBOFJGO(key, acno, pswd, nkey) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR3231(long key, LPCTSTR acno, LPCTSTR pswd, LPCTSTR code, long mmgb, double jprc, long hogb, long qtpy, double jamt, double jrat) 
{
	return FALSE;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR3231) ? S_SONBQ101(key, acno, pswd, code, mmgb, jprc, hogb, qtpy, jamt, jrat) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR3232(long key, LPCTSTR acno, LPCTSTR pswd, long fstp, long ertp) //�ɼ� ���ϼ��� ��ȸ KSJ 2013.04.08
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR3232) ? S_SSLBQ033(key, acno, pswd, fstp, ertp) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR3411(long key, LPCTSTR mymd) 	//�ɼ� ������ ������ �ü� ��ȸ KSJ 2013.04.08
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR3411) ? S_PIBO4013(key, mymd) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR1801(long key, LPCTSTR code) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR1801) ? S_PIBO5101(key, code) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR1802(long key, LPCTSTR code) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR1802) ? S_PIBO5102(key, code) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR1803(long key, LPCTSTR code) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR1803) ? S_PIBO5103(key, code) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR1804(long key, LPCTSTR jidx) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR1804) ? S_PIBO5104(key, jidx) : FALSE;
}

BSTR CIBKSConnectorCtrl::GetAccounts() 
{
	CString sRet = "";
	for(size_t n=0; n<m_acno.size(); ++n)
	{
		if (m_acno[n].GetLength()!=11) continue;
		if (m_acno[n].Mid(3,2)=="00") continue;
		sRet += m_acno[n] + "\t" + m_acnm[n] + "\n";
	}

	sRet.TrimRight();

	if(m_bSaveLog)
		testSaveFile("GetAccounts", sRet, sRet.GetLength());

	return sRet.AllocSysString();
}

BOOL CIBKSConnectorCtrl::S_TR1002(int key, LPCSTR code, LPCSTR data)
{
	if (strlen(code)!=6)
	{
		m_sLastMsg = "�����ڵ带 Ȯ���Ͻʽÿ�";
		return FALSE;
	}
	CString sdat;
	sdat.Format("1301\x7f%s\t%s\t", code, data);
	//return SendTR("pooppoop", key, US_OOP, (LPCSTR)sdat, sdat.GetLength(), C_TR1002);  //vc2019
	return SendTR("pooppoop", key, US_OOP, (LPCSTR)sdat, sdat.GetLength(), &CIBKSConnectorCtrl::C_TR1002);
}

void CIBKSConnectorCtrl::C_TR1002(WPARAM wParam, LPARAM lParam)
{
	int key = LOWORD(wParam);
	int size = HIWORD(wParam);
	LPCSTR data = (LPCSTR)lParam;
	CString odat = CString(data, size);
	odat.TrimRight();

	FireOnRecvData(key, (long)(LPCSTR)odat, odat.GetLength(), false, "");
}

BOOL CIBKSConnectorCtrl::S_TR1003(int key, LPCSTR code, int type, LPCSTR columns, LPCSTR nkey)
{
	if (strlen(code)!=6)
	{
		m_sLastMsg = "�����ڵ带 Ȯ���Ͻʽÿ�";
		return FALSE;
	}

	int ilen, olen, glen;
	grid_i gin;
	CString sin, sout;

	if (strlen(nkey)>=sizeof(grid_o))
	{
		grid_o gout(nkey);
		gin.setGRIDO(gout);
		if (gout.IsNext())
			gin.setIKEY(2);
	}

	sin.Format("1301\x7f%s\t10302\x7f%d\t$10310\x7f", code, type);
	sout = columns; sout.TrimRight(); sout.Replace('\t', '\n'); sout += "\n\t";
	ilen = sin.GetLength();
	olen = sout.GetLength();
	glen = gin.size();

	vector<char> buff(ilen+glen+olen); 
	memcpy(&buff[0],		 (LPCSTR)sin,  ilen);
	memcpy(&buff[ilen],		 &gin,	       glen);
	memcpy(&buff[ilen+glen], (LPCSTR)sout, olen);
	//return SendTR("pooppoop", key, US_OOP, (LPCSTR)&buff[0], buff.size(), C_TR1003);  //vc2019
	return SendTR("pooppoop", key, US_OOP, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::C_TR1003);
}

void CIBKSConnectorCtrl::C_TR1003(WPARAM wParam, LPARAM lParam)
{
	int key = LOWORD(wParam);
	int size = HIWORD(wParam);
	LPCSTR gdata = (LPCSTR)lParam;
	LPCSTR data = gdata + sizeof(grid_o);
	if (size>=sizeof(grid_o))
	{
		grid_o o(gdata);
		CString odat = CString(data, size-sizeof(grid_o));
		odat.TrimRight();
		//DebugFile("C:\\poop_out.dat", "wb", (LPCSTR)odat, odat.GetLength());
		FireOnRecvData(key, (long)(LPCSTR)odat, odat.GetLength(), o.IsNext(), o.GetNKey());
	}
}

BOOL CIBKSConnectorCtrl::S_TR3002(int key, LPCSTR code, LPCSTR data)
{
	if (strlen(code)!=8)
	{
		m_sLastMsg = "�����ڵ带 Ȯ���Ͻʽÿ�";
		return FALSE;
	}
	if (code[0]<'1' || code[0]>'4')
	{
		m_sLastMsg = "�����ڵ带 Ȯ���Ͻʽÿ�";
		return FALSE;
	}

	CString sdat;

	if (code[0]=='1' || code[0]=='4')
		sdat.Format("30301\x7f%s\t%s\t", code, data);
	else
		sdat.Format("40301\x7f%s\t%s\t", code, data);

	//return SendTR("pooppoop", key, US_OOP, (LPCSTR)sdat, sdat.GetLength(), C_TR3002);  //vc2019
	return SendTR("pooppoop", key, US_OOP, (LPCSTR)sdat, sdat.GetLength(), &CIBKSConnectorCtrl::C_TR3002);
}
void CIBKSConnectorCtrl::C_TR3002(WPARAM wParam, LPARAM lParam)
{
	int key = LOWORD(wParam);
	int size = HIWORD(wParam);
	LPCSTR data = (LPCSTR)lParam;
	CString odat = CString(data, size);
	odat.TrimRight();
	FireOnRecvData(key, (long)(LPCSTR)odat, odat.GetLength(), false, "");
}

BOOL CIBKSConnectorCtrl::S_TR3003(int key, LPCSTR code, int type, LPCSTR columns, LPCSTR nkey)
{
	if (strlen(code)!=8)
	{
		m_sLastMsg = "�����ڵ带 Ȯ���Ͻʽÿ�";
		return FALSE;
	}
	if (code[0]<'1' || code[0]>'4')
	{
		m_sLastMsg = "�����ڵ带 Ȯ���Ͻʽÿ�";
		return FALSE;
	}

	int ilen, olen, glen;
	grid_i gin;
	CString sin, sout;
	
	if (strlen(nkey)>=sizeof(grid_o))
	{
		grid_o gout(nkey);
		gin.setGRIDO(gout);
		if (gout.IsNext())
			gin.setIKEY(2);
	}
	
	if (code[0]=='1' || code[0]=='4')
		sin.Format("30301\x7f%s\t33302\x7f%d\t$33310\x7f", code, type);
	else
		sin.Format("40301\x7f%s\t43302\x7f%d\t$43310\x7f", code, type);
	sout = columns; sout.TrimRight(); sout.Replace('\t', '\n'); sout += "\n\t";
	ilen = sin.GetLength();
	olen = sout.GetLength();
	glen = gin.size();
	
	vector<char> buff(ilen+glen+olen); 
	memcpy(&buff[0],		 (LPCSTR)sin,  ilen);
	memcpy(&buff[ilen],		 &gin,	       glen);
	memcpy(&buff[ilen+glen], (LPCSTR)sout, olen);
	//return SendTR("pooppoop", key, US_OOP, (LPCSTR)&buff[0], buff.size(), C_TR3003);  //vc2019
	return SendTR("pooppoop", key, US_OOP, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::C_TR3003);
}

void CIBKSConnectorCtrl::C_TR3003(WPARAM wParam, LPARAM lParam)
{
	int key = LOWORD(wParam);
	int size = HIWORD(wParam);
	LPCSTR gdata = (LPCSTR)lParam;
	LPCSTR data = gdata + sizeof(grid_o);
	
	if (size>=sizeof(grid_o))
	{
		grid_o o(gdata);
		CString odat = CString(data, size-sizeof(grid_o));
		odat.TrimRight();
		//DebugFile("C:\\poop_out.dat", "wb", (LPCSTR)odat, odat.GetLength());
		FireOnRecvData(key, (long)(LPCSTR)odat, odat.GetLength(), o.IsNext(), o.GetNKey());
	}
}

BOOL CIBKSConnectorCtrl::S_TR2001(int key, LPCTSTR upcd, int dtgb)
{
	if (strlen(upcd)!=3)
	{
		m_sLastMsg = "�����ڵ带 Ȯ���Ͻʽÿ�";
		return FALSE;
	}
	if (dtgb < 0 || dtgb > 1)
	{
		m_sLastMsg = "���а��� Ȯ���Ͻʽÿ�";
		return FALSE;
	}
	
	CString sdat;
	
	sdat.Format("%s%d", upcd, dtgb);
	
	//return SendTR("bo292101", key, 0, (LPCSTR)sdat, sdat.GetLength(), DEF_CALLBACK);  //vc2019
	return SendTR("bo292101", key, 0, (LPCSTR)sdat, sdat.GetLength(), &CIBKSConnectorCtrl::DEF_CALLBACK);
}

void CIBKSConnectorCtrl::C_TR2001(WPARAM wParam, LPARAM lParam)
{
	int key = LOWORD(wParam);
	int size = HIWORD(wParam);
	LPCSTR gdata = (LPCSTR)lParam;
	LPCSTR data = gdata + sizeof(grid_o);
	
	if (size>=sizeof(grid_o))
	{
		grid_o o(gdata);
		CString odat = CString(data, size-sizeof(grid_o));
		odat.TrimRight();
		//DebugFile("C:\\poop_out.dat", "wb", (LPCSTR)odat, odat.GetLength());
		FireOnRecvData(key, (long)(LPCSTR)odat, odat.GetLength(), o.IsNext(), o.GetNKey());
	}
}

void CIBKSConnectorCtrl::DebugFile(LPCSTR path, LPCSTR mode, LPCSTR data, int dlen)
{
	FILE *fp = fopen(path, mode);
	if (fp)
	{
		fwrite(data, dlen, 1, fp);
		fclose(fp);
	}
}

BOOL CIBKSConnectorCtrl::TR1002(long key, LPCTSTR code, LPCTSTR symb) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR1002) ? S_TR1002(key, code, symb) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR1003(long key, LPCTSTR code, long type, LPCTSTR symb, LPCTSTR nkey) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR1003) ? S_TR1003(key, code, type, symb, nkey) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR3002(long key, LPCTSTR code, LPCTSTR symb) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR3002) ? S_TR3002(key, code, symb) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR3003(long key, LPCTSTR code, long type, LPCTSTR symb, LPCTSTR nkey) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR3003) ? S_TR3003(key, code, type, symb, nkey) : FALSE;
}

int CIBKSConnectorCtrl::atoi2(LPCSTR data, int size)
{
	vector<char> buff = vector<char>(size+1, 0);
	memcpy(&buff[0], data, size);
	return atoi(&buff[0]);
}

BOOL CIBKSConnectorCtrl::S_PIBOTRAN(LONG key, LPCSTR reqtype, int seqn, int pos)
{
	char *allows[] = { "hjcode", "elwcod", "fjcode", "opcode", NULL };
	for(int n=0; allows[n]!=NULL; ++n)
	{
		if (strcmp(reqtype, allows[n])==0)
		{
			m_cname[key] = reqtype;
			char buff[256];
			sprintf(buff, "%06d%08d/ibkis/axis/rsc/bin/tab/%s.dat\x00%-40s", seqn, pos, reqtype, "");
			TRACE("pibotran - %s\n", buff);
			//return SendTR("pibotran", key, 0, buff, L_pibotran_mid, C_PIBOTRAN);
			return SendTR("pibotran", key, 0, buff, L_pibotran_mid, &CIBKSConnectorCtrl::C_PIBOTRAN);
		}
	}
	m_sLastMsg = "������ ���� ��ûŸ���Դϴ�.";
	return FALSE;
}

void CIBKSConnectorCtrl::C_PIBOTRAN(WPARAM wParam, LPARAM lParam)
{
	int key = LOWORD(wParam);
	int size = HIWORD(wParam);
	struct pibotran_mod *mod = (struct pibotran_mod*)lParam;

	if (m_codes.find(key)==m_codes.end())
		m_codes[key] = vector<char>();

	vector<char> *pVec = &m_codes[key];
	int seqn = atoi2(mod->seqn, sizeof(mod->seqn));
	int max_seqn = atoi2(mod->max_seqn, sizeof(mod->max_seqn));
	int pos = atoi2(mod->start, sizeof(mod->start));
	int dlen = atoi2(mod->size, sizeof(mod->size));
	
	// reserved buffer
	if (seqn==1) pVec->clear();
	int max_dlen = (1024*32) * max_seqn;
	if (pVec->capacity()<max_dlen)
		pVec->reserve(max_dlen);

	pVec->insert(pVec->end(), mod->data, mod->data+dlen);
	//TRACE("%s : seqn[%d] mseqn[%d] pos[%d] dlen[%d]\n", m_cname[key], seqn, max_seqn, pos, dlen);
	if (seqn!=max_seqn)
	{
		char buff[256];
		sprintf(buff, "%06d%08d/ibkis/axis/rsc/bin/tab/%s.dat\x00%-40s", seqn+1, pos, (LPCSTR)m_cname[key], "");
		//SendTR("pibotran", key, 0, buff, L_pibotran_mid, C_PIBOTRAN);  //vc2019
		SendTR("pibotran", key, 0, buff, L_pibotran_mid, &CIBKSConnectorCtrl::C_PIBOTRAN);
	}
	else
	{
		//DebugFile("C:\\"+m_cname[key]+".dat", "wb", &(*pVec)[0], pVec->size());
		FireOnRecvData(key, (long)&(*pVec)[0], pVec->size(), false, "");
	}
}

BOOL CIBKSConnectorCtrl::GetCode(long key, LPCTSTR reqtype) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return S_PIBOTRAN(key, reqtype);
}

void CIBKSConnectorCtrl::C_SACAQ504( WPARAM wParam, LPARAM lParam )
{
	int key = LOWORD(wParam);
	int size = HIWORD(wParam);

	struct ledger *ledger = (struct ledger *)lParam;
	struct sacaq504_mod *mod = (struct sacaq504_mod *)( (LPCSTR)lParam + L_ledger);
	
	if(m_bSaveLog)
		testSaveFile("sacaq504_mod", (LPCSTR)lParam + L_ledger + L_sacaq504_mid, size - L_ledger - L_sacaq504_mid);

	if (ledger->ecod[0]!='0')
	{
		// ��ϳ����� ���ٸ� ���¸�Ͽ��� ����
		for(size_t n=0; n<m_acno.size(); ++n)
		{
			if (m_chk_acno[key]==m_acno[n])
			{
				if(m_bSaveLog)
					testSaveFile("Not Found", m_acno[n], 11);

				m_acno.erase(m_acno.begin()+n);
				m_acnm.erase(m_acnm.begin()+n);
				break;
			}
		}
	}
	else if (ledger->next[0] == 'Y')	//2014.11.25 KSJ �̰�δ븮���� ������ ���Ƽ� FEP�� �����. ����ó����.
	{
		memcpy(ledger->fkey, "7   ", 4);
		//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)lParam, L_ledger + L_sacaq504_mid, C_SACAQ504);  //vc2019
		SendTR("piboPBxQ", key, US_ENC, (LPCSTR)lParam, L_ledger + L_sacaq504_mid, &CIBKSConnectorCtrl::C_SACAQ504);
	}
	else
	{
		int idx = atoi(CString(mod->Out2, sizeof(mod->Out2))) - 1;

		if(m_bSaveLog)
			testSaveFile("RegTp", mod->grid[idx].RegTp, 1);

		if (mod->grid[idx].RegTp[0]!='1')
		{
			// ������ ������ ����� �ƴ϶�� ������ ���� ���� ����
			for(size_t n=0; n<m_acno.size(); ++n)
			{
				if (m_chk_acno[key]==m_acno[n])
				{
					if(m_bSaveLog)
						testSaveFile("UnReg", m_acno[n], 11);

					m_acno.erase(m_acno.begin()+n);
					m_acnm.erase(m_acnm.begin()+n);
					break;
				}
			}
		}
	}
     
	m_chk_acno.erase(key);
	InitJanGo();
}

void CIBKSConnectorCtrl::InitJanGo()
{	
	// SACAQ302�� ����� ��� �Ʒ� if���� ���´�.
	if (m_chk_acno.empty())
	{
		vector<CString>::iterator st = m_acno.begin(), ed = m_acno.end();
		for(; st!=ed; ++st)	
		{
			CString emsg;
			emsg.Format("������ : %s\n", *st);
// 			OutputDebugString(emsg);
		}
		m_bLogin = true;
		FireOnLogin(TRUE, "�α��μ���");

		if (m_jngoWnd.Create(NULL, NULL, WS_CHILD, CRect(0,0,100,100), &m_Wizard, 1))
		{
			CString strTemp;
			long nView = m_Wizard.axWorkView("IBXXXX02", 0);
			
			long nReturn = m_Wizard.axAttach((long)&m_jngoWnd, nView, 128);
			
			long size = MAKELONG(100,100);
			if(!m_Wizard.axWorkShop(nReturn,"IBXXXX02",size,false))
				AfxMessageBox("IBXXXX02 axWorkShop ����");
			
			m_jngoWnd.SetWindowPos(NULL, 0, 0, 100, 100, SWP_SHOWWINDOW);
		} 
		else
		{
			m_sLastMsg = "IBXXXX02 ���� ����";
		}

		if (m_Notify.Create(NULL, NULL, WS_CHILD, CRect(0,0,100,100), &m_Wizard, 2))
		{
			CString strTemp;
			long nView = m_Wizard.axWorkView("IBXXXX11", 0);
			
			long nReturn = m_Wizard.axAttach((long)&m_Notify, nView, 128);
			
			long size = MAKELONG(100,100);
			if(!m_Wizard.axWorkShop(nReturn,"IBXXXX11",size,false))
				AfxMessageBox("IBXXXX11 axWorkShop ����");
			
			m_Notify.SetWindowPos(NULL, 0, 0, 100, 100, SWP_SHOWWINDOW);
		} 
		else
		{
			m_sLastMsg = "IBXXXX11 ���� ����";
		}

		S_PIDOVERS();
	}
}

BOOL CIBKSConnectorCtrl::S_PIBO4013(int key, LPCSTR mymd)	//�ɼ� ������ ������ �ü� ��ȸ
{
	if (strlen(mymd)!=6)
	{
		m_sLastMsg = "��������� Ȯ���Ͻʽÿ�.[mymd]";
		return FALSE;
	}

	struct tr3411_mid mid;
	ZeroMemory(&mid, sizeof(struct tr3411_mid));
	mid.ysgb = '0';	//���󰡴� 1�� ����
	mid.func = ' ';
	CopyMemory(&mid.mymd, mymd, strlen(mymd));
	
	//SendTR("PIBO4013", key, 0, (char *)&mid, sizeof(struct tr3411_mid), DEF_CALLBACK);  //vc2019
	SendTR("PIBO4013", key, 0, (char*)&mid, sizeof(struct tr3411_mid), &CIBKSConnectorCtrl::DEF_CALLBACK);
	return TRUE;
}


void CIBKSConnectorCtrl::OnSetClientSite()
{
	if (m_pClientSite)
	{
		VERIFY(CreateControlWindow(::GetDesktopWindow(), CRect(0,0,0,0), CRect(0,0,0,0)));
// 		ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
// 		VERIFY(PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM));
	}
	COleControl::OnSetClientSite();
}

long CIBKSConnectorCtrl::OnUser(WPARAM wParam, LPARAM lParam)
{	
	switch(wParam)
	{
		case WP_F_JNGO_CHANGE:					
			FireOnFBalance((char*)lParam);
			// 		strTemp.Format("[KSJ]WP_JNGO_CHANGE[%s]\n", (char*)lParam);
			// 		OutputDebugString(strTemp);
			break;
		case WP_S_JNGO_CHANGE:
			FireOnSBalance((char*)lParam);			
		// 		strTemp.Format("[KSJ]WP_JNGO_CHANGE[%s]\n", (char*)lParam);
		// 		OutputDebugString(strTemp);
			break;
	}
	
	return 0;
}

// ������ ���� �ƴ����� �Ǵ��ϰ��� �ϴ� �Լ�
BOOL CIBKSConnectorCtrl::IsNumber(CString str)
{
    BOOL bRet = TRUE;
    int nLen = str.GetLength();
    for( int i=0 ; i < nLen ; ++i)
    {
        if( isdigit(str.GetAt(i)) == FALSE)
        {
            bRet = FALSE;
            break;
        }
    }
    return bRet;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PTS �߰�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CIBKSConnectorCtrl::S_SONBQ123(int key, LPCSTR acno, LPCSTR pswd, double zKpi200Idx) // �����ɼ� �������Ͽ������ű���ȸ2
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;
	
	vector<char> buff(L_ledger+L_TR4224_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4224_mid *mid = (struct TR4224_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SONBQ123", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0000", 4);
	memcpy(ledger->fkey, "C   ", 4);
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zInptPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zInptPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	CString strTemp;
	strTemp.Format("%+013.2f", zKpi200Idx);
	CopyMemory(mid->zKpi200Idx, strTemp, strTemp.GetLength());	

	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK); //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_SONDQ401(int key, LPCSTR acno, LPCSTR pswd, LPCSTR zSrtDt, LPCSTR zEndDt, LPCSTR nkey) // �����ɼ� �߰����űݹ߻���Ȳ
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;
	
	vector<char> buff(L_ledger+L_TR4223_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4223_mid *mid = (struct TR4223_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SONDQ401", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0010", 4);
	if (strlen(nkey)==0)
		memcpy(ledger->fkey, "C   ", 4);
	else
	{
		ledger->next[0] = 'Y';
		memcpy(ledger->fkey, "7   ", 4);
		CString strTemp;
		strTemp.Format("%s", nkey);
		CopyMemory(ledger->nkey, strTemp, strTemp.GetLength());
	}
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	memcpy(mid->zSrtDt, zSrtDt, 8);
	memcpy(mid->zEndDt, zEndDt, 8);

	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_SSLAQ826(int key, LPCSTR acno, LPCSTR pswd, LPCSTR zQrySrtDt, LPCSTR zQryEndDt, int zQryTp, int zStnlnSeqTp, int zBalEvalTp, LPCTSTR nkey) // �����ɼ� �Ϻ� ���¼��ͳ���
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;
	
	if (zQryTp<1 || zQryTp>3) { m_sLastMsg="��ȸ���а��� Ȯ���ϼ���.[zQryTp]"; return FALSE; }
	if (zStnlnSeqTp<1 || zStnlnSeqTp>2) { m_sLastMsg="���ļ������� Ȯ���ϼ���.[zStnlnSeqTp]"; return FALSE; }
	if (zBalEvalTp<0 || zBalEvalTp>2) { m_sLastMsg="�ܰ��а��� Ȯ���ϼ���.[zBalEvalTp]"; return FALSE; }

	vector<char> buff(L_ledger+L_TR4303_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4303_mid *mid = (struct TR4303_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SSLAQ826", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0010", 4);
	if (strlen(nkey)==0)
		memcpy(ledger->fkey, "C   ", 4);
	else
	{
		ledger->next[0] = 'Y';
		memcpy(ledger->fkey, "7   ", 4);
		CString strTemp;
		strTemp.Format("%s", nkey);
		CopyMemory(ledger->nkey, strTemp, strTemp.GetLength());
	}
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	memcpy(mid->zQrySrtDt, zQrySrtDt, 8);
	memcpy(mid->zQryEndDt, zQryEndDt, 8);

	mid->zQryTp[0] = '0' + zQryTp;
	mid->zStnlnSeqTp[0] = '0' + zStnlnSeqTp;
	mid->zBalEvalTp[0] = '0' + zBalEvalTp;

	
	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

/* zFnoClssCode �����ɼǺз��ڵ�		00.��ü, 11.����, 12.�ɼ�*/
BOOL CIBKSConnectorCtrl::S_SONBQ740(int key, LPCSTR acno, LPCSTR pswd, int zFnoClssCode, LPCTSTR nkey) // �����ɼ� �����ܰ���ȸ
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;

	if (zFnoClssCode != 0 && zFnoClssCode != 11 && zFnoClssCode != 12) { m_sLastMsg="�����ɼǺз��ڵ带 Ȯ���ϼ���.[zFnoClssCode]"; return FALSE; }

	CString strTemp;

	vector<char> buff(L_ledger+L_TR4201_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4201_mid *mid = (struct TR4201_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SONBQ740", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0010", 4);
	if (strlen(nkey)==0)
		memcpy(ledger->fkey, "C   ", 4);
	else
	{
		ledger->next[0] = 'Y';
		memcpy(ledger->fkey, "7   ", 4);
		CString strTemp;
		strTemp.Format("%s", nkey);
		CopyMemory(ledger->nkey, strTemp, strTemp.GetLength());
	}
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ

	strTemp.Format("%02d", zFnoClssCode);
	CopyMemory(mid->zFnoClssCode, strTemp, 2);
	memcpy(mid->zPdGrpCode, "00", 2);

	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_SACMT238(int key, LPCSTR acno, LPCSTR pswd) // ���º�й�ȣ ����
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	if (!IsValidAccount(acno))	return FALSE;

	vector<char> buff(L_ledger+L_TR4101_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4101_mid *mid = (struct TR4101_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SACMT238", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
// 	memcpy(ledger->rcnt, "0010", 4);
	memcpy(ledger->fkey, "C   ", 4);
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	
	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_SONBQ105(int key, LPCSTR acno, LPCSTR pswd) // �����ɼ� ���¿�Ź�����ű���ȸ
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;

	vector<char> buff(L_ledger+L_TR4221_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4221_mid *mid = (struct TR4221_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SONBQ105", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
// 	memcpy(ledger->rcnt, "0010", 4);
	memcpy(ledger->fkey, "C   ", 4);
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	
	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_SDPBQ001(int key, LPCSTR acno, LPCSTR pswd, int zQryTp, LPCSTR zQrySrtDt, LPCSTR zQryEndDt, LPCSTR zIsuNo, LPCSTR nkey) // �����ɼ� ���¿�Ź�����ű���ȸ
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;
	
	if (zQryTp<0 || zQryTp>3) { m_sLastMsg="��ȸ���а��� Ȯ���ϼ���.[zQryTp]"; return FALSE; }
	
	vector<char> buff(L_ledger+L_TR4401_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4401_mid *mid = (struct TR4401_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SDPBQ001", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0009", 4);
	if (strlen(nkey)==0)
		memcpy(ledger->fkey, "C   ", 4);
	else
	{
		ledger->next[0] = 'Y';
		memcpy(ledger->fkey, "7   ", 4);
		CString strTemp;
		strTemp.Format("%s", nkey);
		CopyMemory(ledger->nkey, strTemp, strTemp.GetLength());
	}
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	mid->zQryTp[0] = '0' + zQryTp;
	
	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	memcpy(mid->zQrySrtDt, zQrySrtDt, 8);
	memcpy(mid->zQryEndDt, zQryEndDt, 8);
	memcpy(mid->zSrtNo, "+000000000", 10);
	memcpy(mid->zPdptnCode, "00", 2);

	if(strlen(zIsuNo) == 0)
		FillMemory(mid->zIsuNo, 12, ' ');
	else
		memcpy(mid->zIsuNo, zIsuNo, 12);

	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

//zRoopData 	zFnoIsuNo[32]\tzBnsTp[1]\tzBnsTpNm[10]\tzUnsttQty[16]\tzLqdtQty[16]\tzExecPrc[13]\n
BOOL CIBKSConnectorCtrl::S_SONBQ114(int key, LPCSTR acno, LPCSTR pswd, int zMprcOrdTp, LPCSTR zRoopData) // �����ɼ� ����û���ֹ�ü�Ό�����ű�
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;

	CString strData, strLine, strField, strTemp;
	int nCount = 0;
	if (zMprcOrdTp<0 || zMprcOrdTp>1) { m_sLastMsg="���屸�а��� Ȯ���ϼ���.[zMprcOrdTp]"; return FALSE; }
	
	if(strlen(zRoopData) != 0)
	{
		LPCSTR st = zRoopData, ed = zRoopData + _tcslen(zRoopData);
		nCount = std::count(st, ed, '\n');
		if ( *(ed-1) != '\n' ) 
			++nCount ;
	}
		
	vector<char> buff(L_ledger+L_TR4003_mid + L_TR4003_mid2, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4003_mid *mid = (struct TR4003_mid*)&buff[L_ledger];
	struct TR4003_mid2 *mid2 = (struct TR4003_mid2*)&buff[L_ledger+L_TR4003_mid];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SONBQ114", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
// 	memcpy(ledger->rcnt, "0045", 4);
	memcpy(ledger->fkey, "C   ", 4);
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zInptPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zInptPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ

	mid->zMprcOrdTp[0] = '0' + zMprcOrdTp;

	strTemp.Format("%05d", nCount);
	CopyMemory(mid2->In2, strTemp, min(sizeof(mid2->In2), strTemp.GetLength()));

	nCount = 0;	
	strData = zRoopData;
//  	OutputDebugString("[KSJ] zRoopData[" + strData + "]\n");

	while(AfxExtractSubString(strLine, strData, nCount, '\n'))
	{
		if (strLine.IsEmpty())
			break;
		
		AfxExtractSubString(strTemp, strLine, 0, '\t');	//zFnoIsuNo[32]
		strField.Format("%32s", strTemp);
		CopyMemory(mid2->grid[nCount].zFnoIsuNo, strField, 32);

// 		OutputDebugString("[KSJ] zFnoIsuNo[" + strField + "]\n");

		AfxExtractSubString(strTemp, strLine, 1, '\t');	//zBnsTp[1]
		strField.Format("%s", strTemp);
		CopyMemory(mid2->grid[nCount].zBnsTp, strField, strField.GetLength());
		
// 		OutputDebugString("[KSJ] zBnsTp[" + strField + "]\n");

		AfxExtractSubString(strTemp, strLine, 2, '\t');	//zBnsTpNm[10]
		strField.Format("%10s", strTemp);
		CopyMemory(mid2->grid[nCount].zBnsTpNm, strField, strField.GetLength());

// 		OutputDebugString("[KSJ] zBnsTpNm[" + strField + "]\n");

		AfxExtractSubString(strTemp, strLine, 3, '\t');	//zUnsttQty[16]
		strField.Format("%+016d", atoi(strTemp));
		CopyMemory(mid2->grid[nCount].zUnsttQty, strField, strField.GetLength());

// 		OutputDebugString("[KSJ] zUnsttQty[" + strField + "]\n");

		AfxExtractSubString(strTemp, strLine, 4, '\t');	//zLqdtQty[16]
		strField.Format("%+016d", atoi(strTemp));
		CopyMemory(mid2->grid[nCount].zLqdtQty, strField, strField.GetLength());

// 		OutputDebugString("[KSJ] zLqdtQty[" + strField + "]\n");

		AfxExtractSubString(strTemp, strLine, 5, '\t');	//zExecPrc[13]
		strField.Format("%+013.2f", atof(strTemp));
		CopyMemory(mid2->grid[nCount].zExecPrc, strField, strField.GetLength());

// 		OutputDebugString("[KSJ] zExecPrc[" + strField + "]\n");

		nCount++;
	}
		
// 	OutputDebugString("[KSJ] Send Pre[" + strResult + "]\n");
	
	int nLength = buff.size() - (100-nCount)*sizeof(struct TR4003_grid_mid);

	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], nLength, DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], nLength, &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);

// 	OutputDebugString("[KSJ] Send Next[" + strResult + "]\n");

	return TRUE;
}

//zRoopData zFnoIsuNo[32]\tzBnsTp[1]\tzFnoOrdprcPtnCode[2]\tzOrdQty[16]\tzOrdPrc[13]\tzCurPrc[13]\tzOrdNo[10]\tzMsgCode[4]
BOOL CIBKSConnectorCtrl::S_SONBT709(int key, LPCSTR acno, LPCSTR pswd, LPCSTR zRoopData) // �����ɼ� ���������ֹ�
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;
	
	CString strData, strLine, strField, strTemp;
	int nCount = 0;
		
	LPCSTR st = zRoopData, ed = zRoopData + _tcslen(zRoopData);
	nCount = std::count(st, ed, '\n');
	if ( *(ed-1) != '\n' ) 
		++nCount ;
	
	vector<char> buff(L_ledger+L_TR4004_mid + L_TR4004_mid2, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4004_mid *mid = (struct TR4004_mid*)&buff[L_ledger];
	struct TR4004_mid2 *mid2 = (struct TR4004_mid2*)&buff[L_ledger+L_TR4004_mid];

	GetLedger(ledger);
	memcpy(ledger->svcd, "SONBT709", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0045", 4);
	memcpy(ledger->fkey, "C   ", 4);
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zInptPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zInptPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ

	mid->zTrxTp[0] = '2';	//�ֹ�������ȸ1 �ֹ� 2

	memcpy(mid->zFnoTrdPtnCode, "03", 2);
	memcpy(mid->zCommdaCode, "01", 2);
	memcpy(mid->zIp, ledger->pcip, 12);

	strTemp.Format("%05d", nCount);
	CopyMemory(mid2->In2, strTemp, min(sizeof(mid2->In2), strTemp.GetLength()));

	nCount = 0;
	strData = zRoopData;

// 	OutputDebugString("[KSJ] zRoopData[" + strData + "]\n");

	while(AfxExtractSubString(strLine, strData, nCount, '\n'))
	{
		if (strLine.IsEmpty())
			break;

		AfxExtractSubString(strTemp, strLine, 0, '\t');	//zFnoIsuNo[32]
		strField.Format("%32s", strTemp);
		CopyMemory(mid2->grid[nCount].zFnoIsuNo, strField, strField.GetLength());
		
// 		OutputDebugString("[KSJ] zFnoIsuNo[" + strField + "]\n");

		AfxExtractSubString(strTemp, strLine, 1, '\t');	//zBnsTp[1]
		strField.Format("%s", strTemp);
		CopyMemory(mid2->grid[nCount].zBnsTp, strField, strField.GetLength());

// 		OutputDebugString("[KSJ] zBnsTp[" + strField + "]\n");

		AfxExtractSubString(strTemp, strLine, 2, '\t');	//zFnoOrdprcPtnCode[2]
		strField.Format("%02s", strTemp);
		CopyMemory(mid2->grid[nCount].zFnoOrdprcPtnCode, strField, strField.GetLength());

// 		OutputDebugString("[KSJ] zFnoOrdprcPtnCode[" + strField + "]\n");

		AfxExtractSubString(strTemp, strLine, 3, '\t');	//zOrdQty[16]
		strField.Format("%+016d", atoi(strTemp));
		CopyMemory(mid2->grid[nCount].zOrdQty, strField, strField.GetLength());
	
// 		OutputDebugString("[KSJ] zOrdQty[" + strField + "]\n");

		AfxExtractSubString(strTemp, strLine, 4, '\t');	//zOrdPrc[13]
		strField.Format("%+013.2f", atof(strTemp));
		CopyMemory(mid2->grid[nCount].zOrdPrc, strField, strField.GetLength());
	
// 		OutputDebugString("[KSJ] zOrdPrc[" + strField + "]\n");

		AfxExtractSubString(strTemp, strLine, 5, '\t');	//zCurPrc[13]
		strTemp.TrimLeft(); strTemp.TrimRight();
		if(strTemp.IsEmpty())
			strTemp = "0";

		strField.Format("%+013.2f", atof(strTemp));
		CopyMemory(mid2->grid[nCount].zCurPrc, strField, strField.GetLength());
		
// 		OutputDebugString("[KSJ] zCurPrc[" + strField + "]\n");

// 		AfxExtractSubString(strTemp, strLine, 6, '\t');	//zOrdNo[10]
		strField.Format("%+010d", 0);
		CopyMemory(mid2->grid[nCount].zOrdNo, strField, strField.GetLength());

//		AfxExtractSubString(strTemp, strLine, 7, '\t');	//zMsgCode[4]
		strField.Format("%4s", "");
		CopyMemory(mid2->grid[nCount].zMsgCode, strField, strField.GetLength());

		nCount++;
	}
	
// 	OutputDebugString("[KSJ] strResult[" + strResult + "]\n");
// 	OutputDebugString("[KSJ] buff + strResult[" + strResult + "]\n");

	int nLength = buff.size() - (100-nCount)*sizeof(struct TR4004_grid_mid);
	
	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], nLength, DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], nLength, &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
		
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_SONDQ304(int key, LPCSTR acno, LPCSTR pswd, int zFnoClssCode, LPCSTR zCmsnAmtAddupCode, LPCSTR zSrtDt, LPCSTR zEndDt, LPCSTR nkey) // �����ɼ� ���º� ��ü�����׼����� ����
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;
	
	if (zFnoClssCode != 0 && zFnoClssCode != 11 && zFnoClssCode != 12) { m_sLastMsg="�����ɼǺз��ڵ带 Ȯ���ϼ���.[zFnoClssCode]"; return FALSE; }

	vector<char> buff(L_ledger+L_TR4403_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4403_mid *mid = (struct TR4403_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SONDQ304", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0027", 4);
	if (strlen(nkey)==0)
		memcpy(ledger->fkey, "C   ", 4);
	else
	{
		ledger->next[0] = 'Y';
		memcpy(ledger->fkey, "7   ", 4);
		CString strTemp;
		strTemp.Format("%s", nkey);
		CopyMemory(ledger->nkey, strTemp, strTemp.GetLength());
	}
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	mid->zChoicInptTp[0] = '3';
	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ

	CString strTemp;
	strTemp.Format("%02d", zFnoClssCode);
	CopyMemory(mid->zFnoClssCode, strTemp, 2);

	memcpy(mid->zPdGrpCode, "00", 2);
	memcpy(mid->zCmsnAmtAddupCode, zCmsnAmtAddupCode, 2);
	memcpy(mid->zSrtDt, zSrtDt, 8);
	memcpy(mid->zEndDt, zEndDt, 8);
		
	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

// ��ݰ���, ��ݰ��º�й�ȣ, ��ݱݾ�, �Ա������ڵ�, �Աݰ��¹�ȣ, �Աݰ��º�й�ȣ, �Ƿ��θ�
BOOL CIBKSConnectorCtrl::S_SDPKT360(int key, LPCSTR zMnyoutAcntno, LPCSTR zMnyoutPswd, int zlMnyoutAmt, LPCSTR zMnyinBnkCode, LPCSTR zMnyinBnkAcntNo, LPCSTR zAskpsnNm, LPCSTR zTelExno, LPCSTR zTelRgno, LPCSTR zTelSeqno) // ������ü���(����->����)
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	if (!IsValidAccount(zMnyoutAcntno))	return FALSE;
	if (!IsValidAcnoPswd(zMnyoutPswd))	return FALSE;
	
	vector<char> buff(L_ledger+L_TR4501_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4501_mid *mid = (struct TR4501_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SDPKT360", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
// 	memcpy(ledger->rcnt, "0050", 4);
	ledger->mkty[0] = '3';
	ledger->odrf[0] = '3';
	memcpy(ledger->fkey, "5   ", 4);
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(zMnyoutPswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zCardIssCnt, "+000000000", 10);
	memcpy(mid->zMgrCardIssCnt, "+000000000", 10);
	memcpy(mid->zMnyoutAcntNo, zMnyoutAcntno, 11);
	//memcpy(mid->zPwd, zMnyoutPswd, strlen(zMnyoutPswd));	// ���º��
	memcpy(mid->zPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ

	CString strTemp;
	strTemp.Format("%+016d", zlMnyoutAmt);
	CopyMemory(mid->zMnyoutAmt, strTemp, strTemp.GetLength());

	memcpy(mid->zMnyinBnkCode, zMnyinBnkCode, 3);
	memcpy(mid->zBnkAcntNo, zMnyinBnkAcntNo, strlen(zMnyinBnkAcntNo));

	mid->zMnyoutPtnTp[0] = '1';

	if(strlen(zAskpsnNm) > 0)
	{
		memcpy(mid->zAskpsnNm, zAskpsnNm, strlen(zAskpsnNm));
		memcpy(mid->zTelExno, zTelExno, strlen(zTelExno));/* ��ȭ����ȣ*/
		memcpy(mid->zTelRgno, zTelRgno, strlen(zTelRgno));/* ��ȭ������ȣ*/
		memcpy(mid->zTelSeqno, zTelSeqno, strlen(zTelSeqno));/* ��ȭ�Ϸù�ȣ*/
	}

	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_SDPDT901(int key, LPCSTR acno, LPCSTR pswd, int lBkeepAmt, LPCSTR zMnyinAcntNo) // ���°��ڱݴ�ü(���� -> ����)
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	if (!IsValidAccount(acno))				return FALSE;
	if (!IsValidAcnoPswd(pswd))				return FALSE;
	if (!IsValidSendAccount(zMnyinAcntNo))	return FALSE;
	
	vector<char> buff(L_ledger+L_TR4502_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4502_mid *mid = (struct TR4502_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SDPDT901", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	// 	memcpy(ledger->rcnt, "0035", 4);
	ledger->mkty[0] = '3';
	ledger->odrf[0] = '3';
	memcpy(ledger->fkey, "C   ", 4);
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	memcpy(mid->lCardIssCnt, "+000000000", 10);
	memcpy(mid->lMgrCardIssCnt, "+000000000", 10);
	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ

	CString strTemp;
	strTemp.Format("%+016d", lBkeepAmt);
	CopyMemory(mid->lBkeepAmt, strTemp, strTemp.GetLength());

	mid->zMnyoutPtnTp[0] = '4';

	memcpy(mid->zMnyinAcntNo, zMnyinAcntNo, strlen(zMnyinAcntNo));
	
	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_SDPKT300(int key, LPCSTR zMnyinAcntNo, LPCSTR zBnkCode, LPCSTR zBnkAcntNo, LPCSTR zBnkPwd, int zMnyinAmt) // ������ü���(���� -> ����)
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
// 	if (!IsValidAccount(zMnyinAcntNo))		return FALSE;
	
	if(strlen(zBnkCode) == 0 )		{ m_sLastMsg="�����ڵ带 Ȯ���ϼ���.[zBnkCode]"; return FALSE; }
	if(strlen(zBnkAcntNo) == 0 )	{ m_sLastMsg="������¹�ȣ�� Ȯ���ϼ���.[zBnkAcntNo]"; return FALSE; }
	if(strlen(zBnkPwd) < 4)			{ m_sLastMsg="�����й�ȣ�� Ȯ���ϼ���.[zBnkPwd]"; return FALSE; }
	if(zMnyinAmt <= 0 )				{ m_sLastMsg="�Աݱݾ��� Ȯ���ϼ���.[zMnyinAmt]"; return FALSE; }

	vector<char> buff(L_ledger+L_TR4503_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4503_mid *mid = (struct TR4503_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SDPKT300", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	// 	memcpy(ledger->rcnt, "0035", 4);
	ledger->mkty[0] = '3';
	ledger->odrf[0] = '3';
	memcpy(ledger->fkey, "5   ", 4);
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(zBnkPwd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zMnyinAcntNo, zMnyinAcntNo, 11);
	memcpy(mid->zBnkCode, zBnkCode, 3);
	memcpy(mid->zBnkAcntNo, zBnkAcntNo, strlen(zBnkAcntNo));
	//memcpy(mid->zPwd, zBnkPwd, strlen(zBnkPwd));
	memcpy(mid->zPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ

	CString tmp;
	tmp.Format("%+016d", zMnyinAmt);
	memcpy(mid->zMnyinAmt, tmp, tmp.GetLength());
	
	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_SDPNQ002(int key, LPCSTR acno, LPCSTR pswd, LPCSTR zQrySrtDt, LPCSTR zQryEndDt, LPCSTR nkey) // ���º�������ü�ŷ�����
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
// 	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;

	vector<char> buff(L_ledger+L_TR4511_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4511_mid *mid = (struct TR4511_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SDPNQ002", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0012", 4);
	if (strlen(nkey)==0)
		memcpy(ledger->fkey, "C   ", 4);
	else
	{
		ledger->next[0] = 'Y';
		memcpy(ledger->fkey, "7   ", 4);
		CString strTemp;
		strTemp.Format("%s", nkey);
		CopyMemory(ledger->nkey, strTemp, strTemp.GetLength());
	}
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	memcpy(mid->zQrySrtDt, zQrySrtDt, 8);
	memcpy(mid->zQryEndDt, zQryEndDt, 8);
	
	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_SDPBQ905(int key, LPCSTR acno, LPCSTR pswd, LPCSTR zQrySrtDt, LPCSTR zQryEndDt, LPCSTR nkey) // ���´�ü�ŷ�����
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;
	
	vector<char> buff(L_ledger+L_TR4512_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4512_mid *mid = (struct TR4512_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SDPBQ905", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0010", 4);
	if (strlen(nkey)==0)
		memcpy(ledger->fkey, "C   ", 4);
	else
	{
		ledger->next[0] = 'Y';
		memcpy(ledger->fkey, "7   ", 4);
		CString strTemp;
		strTemp.Format("%s", nkey);
		CopyMemory(ledger->nkey, strTemp, strTemp.GetLength());
	}
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());

	memcpy(mid->In, "00001", 5);
	memcpy(mid->zBrnNo, "000", 3);
	memcpy(mid->zQrySrtDt, zQrySrtDt, 8);
	memcpy(mid->zQryEndDt, zQryEndDt, 8);
	memcpy(mid->zSrtTrdNo, "+00000", 6);
	mid->zBkeepTp[0] = '1';

	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ
	memcpy(mid->zQryCnt, "+000000020", 10);
	
	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_SDPNQ060(int key) // ���ฮ��Ʈ��ȸ
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	
	vector<char> buff(L_ledger, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SDPNQ060", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0050", 4);
	memcpy(ledger->fkey, "C   ", 4);
	
	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_SDPNQ031(int key, LPCSTR acno, LPCSTR pswd, LPCSTR zBankCode) // ������¹�ȣ��ȸ
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;
	
	vector<char> buff(L_ledger+L_TR4522_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4522_mid *mid = (struct TR4522_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SDPNQ031", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0050", 4);
	memcpy(ledger->fkey, "C   ", 4);
	
	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zAcntNo, acno, 11);
	//memcpy(mid->zInptPwd, pswd, strlen(pswd));	// ���º��
	memcpy(mid->zInptPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ

	memcpy(mid->zBankCode, zBankCode, 3);
	
	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK); //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_SACMQ913(int key, LPCSTR nkey) // HTS�α��ΰ��¸��
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	
	vector<char> buff(L_ledger+L_TR4523_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4523_mid *mid = (struct TR4523_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SACMQ913", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0035", 4);
	if (strlen(nkey)==0)
		memcpy(ledger->fkey, "C   ", 4);
	else
	{
		ledger->next[0] = 'Y';
		memcpy(ledger->fkey, "7   ", 4);
		CString strTemp;
		strTemp.Format("%s", nkey);
		CopyMemory(ledger->nkey, strTemp, strTemp.GetLength());
	}
	
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zElecfnUserId, ledger->usid, sizeof(ledger->usid));
	
	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_SBPGT339(int key) // �����⺻������ȸ
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	
	vector<char> buff(L_ledger+L_TR4524_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4524_mid *mid = (struct TR4524_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SBPGT339", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0035", 4);
	memcpy(ledger->fkey, "C   ", 4);
	
	memcpy(mid->In, "00001", 5);
	mid->zTrxTp[0] = 'I';

	memcpy(mid->zUserId, ledger->usid, sizeof(ledger->usid));
	
	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_SDPNQ180(int key, LPCSTR zMnyoutAcntno) // ��ݰ��ɱݾ� �� �Ǹ��ȣ��ȸ
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	if (!IsValidAccount(zMnyoutAcntno))	return FALSE;
	
	vector<char> buff(L_ledger+L_TR4525_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4525_mid *mid = (struct TR4525_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SDPNQ180", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
// 	memcpy(ledger->rcnt, "0035", 4);
	memcpy(ledger->fkey, "C   ", 4);
	
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zMnyoutAcntNo, zMnyoutAcntno, strlen(zMnyoutAcntno));
	
	mid->zMnyoutPtnTp[0] = '1';
	
	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;

	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_SACAQ519(int key, LPCSTR acno, LPCSTR pswd, LPCSTR nkey) // ���� ������Ȳ
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	if (!IsValidAcnoPswd(pswd))	return FALSE;
	
	vector<char> buff(L_ledger+L_TR4526_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4526_mid *mid = (struct TR4526_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SACAQ519", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
	memcpy(ledger->rcnt, "0035", 4);
	if (strlen(nkey)==0)
		memcpy(ledger->fkey, "C   ", 4);
	else
	{
		ledger->next[0] = 'Y';
		memcpy(ledger->fkey, "7   ", 4);
		CString strTemp;
		strTemp.Format("%s", nkey);
		CopyMemory(ledger->nkey, strTemp, strTemp.GetLength());
	}

	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�
	CString sPswd;
	sPswd = GetEncPassword(pswd);
	CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
	CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	
	// Data ����
	memcpy(mid->In, "00001", 5);

	mid->zAcntTp[0] = '2';
	memcpy(mid->zAcntNo, acno, 11);
	memcpy(mid->zInptPwd, pswd, strlen(pswd));
	memcpy(mid->zInptPwd, "HEAD", 4);			// ���º��	2015.05.23 KSJ

	memcpy(mid->zSvcKindCode, "1101", 4);
	
	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_SACMQ229(int key, LPCSTR acno) // ���¸���ȸ
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;
	if (!IsValidAccount(acno))	return FALSE;
	
	vector<char> buff(L_ledger+L_TR4527_mid, ' ');
	
	struct ledger *ledger = (struct ledger*)&buff[0];
	struct TR4527_mid *mid = (struct TR4527_mid*)&buff[L_ledger];
	
	GetLedger(ledger);
	memcpy(ledger->svcd, "SACMQ229", 8);
	FillMemory(ledger->usid, sizeof(ledger->usid), ' ');
	memcpy(ledger->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(ledger->brno, "000", 3);
// 	memcpy(ledger->rcnt, "0012", 4);
	memcpy(ledger->fkey, "C   ", 4);
	
	memcpy(mid->In, "00001", 5);
	memcpy(mid->zAcntNo, acno, 11);
	
	//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), DEF_KOSCOM_CALLBACK);  //vc2019
	SendTR("piboPBxQ", key, US_ENC, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_KOSCOM_CALLBACK);
	return TRUE;
}

BOOL CIBKSConnectorCtrl::S_PIHOCUST(int key, LPCSTR acno) // �� ��� ��ȸ
{
	if (!IsValidState())		return FALSE;
	if (!IsValidPTS())			return FALSE;

	vector<char> buff(L_TR8001_mid, ' ');
	struct TR8001_mid *mid = (struct TR8001_mid*)&buff[0];

	memcpy(mid->usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(mid->accn, acno, 11);

	//return SendTR("pihocust", key, 0, (LPCSTR)&buff[0], buff.size(), DEF_CALLBACK);  //vc2019
	return SendTR("pihocust", key, 0, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_CALLBACK);
}


BOOL CIBKSConnectorCtrl::TR4224(long key, LPCSTR acno, LPCSTR pswd, double zKpi200Idx) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4224) ? S_SONBQ123(key, acno, pswd, zKpi200Idx) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4223(long key, LPCSTR acno, LPCSTR pswd, LPCSTR zSrtDt, LPCSTR zEndDt, LPCSTR nkey) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4223) ? S_SONDQ401(key, acno, pswd, zSrtDt, zEndDt, nkey) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4303(long key, LPCSTR acno, LPCSTR pswd, LPCSTR zQrySrtDt, LPCSTR zQryEndDt, long zQryTp, long zStnlnSeqTp, long zBalEvalTp, LPCTSTR nkey) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4303) ? S_SSLAQ826(key, acno, pswd, zQrySrtDt, zQryEndDt, zQryTp, zStnlnSeqTp, zBalEvalTp, nkey) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4201(long key, LPCSTR acno, LPCSTR pswd, long zFnoClssCode, LPCTSTR nkey) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4201) ? S_SONBQ740(key, acno, pswd, zFnoClssCode, nkey) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4101(long key, LPCSTR acno, LPCSTR pswd) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4101) ? S_SACMT238(key, acno, pswd) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4221(long key, LPCSTR acno, LPCSTR pswd) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4221) ? S_SONBQ105(key, acno, pswd) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4401(long key, LPCSTR acno, LPCSTR pswd, long zQryTp, LPCSTR zQrySrtDt, LPCSTR zQryEndDt, LPCSTR zIsuNo, LPCTSTR nkey) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4401) ? S_SDPBQ001(key, acno, pswd, zQryTp, zQrySrtDt, zQryEndDt, zIsuNo, nkey) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4202(long key, LPCSTR acno, LPCSTR pswd, long zBalEvalTp, long zPrcEvalTp, LPCSTR nkey) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4202) ? S_SONBQ504(key, acno, pswd, zBalEvalTp, zPrcEvalTp, nkey) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4003(long key, LPCSTR acno, LPCSTR pswd, long zMprcOrdTp, LPCSTR zRoopData) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4003) ? S_SONBQ114(key, acno, pswd, zMprcOrdTp, zRoopData) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4004(long key, LPCSTR acno, LPCSTR pswd, LPCSTR zRoopData) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4004) ? S_SONBT709(key, acno, pswd, zRoopData) : FALSE;
}

void CIBKSConnectorCtrl::SetPTS() 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_bPTS = true;
}

BOOL CIBKSConnectorCtrl::TR4501(long key, LPCSTR zMnyoutAcntno, LPCSTR zMnyoutPswd, long zlMnyoutAmt, LPCSTR zMnyinBnkCode, LPCSTR zMnyinBnkAcntNo, LPCSTR zAskpsnNm, LPCSTR zTelExno, LPCSTR zTelRgno, LPCSTR zTelSeqno) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4501) ? S_SDPKT360(key, zMnyoutAcntno, zMnyoutPswd, zlMnyoutAmt, zMnyinBnkCode, zMnyinBnkAcntNo, zAskpsnNm, zTelExno, zTelRgno, zTelSeqno) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4502(long key, LPCSTR acno, LPCSTR pswd, long lBkeepAmt, LPCSTR zMnyinAcntNo) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4502) ? S_SDPDT901(key, acno, pswd, lBkeepAmt, zMnyinAcntNo) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4503(long key, LPCSTR zMnyinAcntNo, LPCSTR zBnkCode, LPCSTR zBnkAcntNo, LPCSTR zBnkPwd, long zMnyinAmt) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4503) ? S_SDPKT300(key, zMnyinAcntNo, zBnkCode, zBnkAcntNo, zBnkPwd, zMnyinAmt) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4511(long key, LPCSTR acno, LPCSTR pswd, LPCSTR zQrySrtDt, LPCSTR zQryEndDt, LPCSTR nkey) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4511) ? S_SDPNQ002(key, acno, pswd, zQrySrtDt, zQryEndDt, nkey) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4512(long key, LPCSTR acno, LPCSTR pswd, LPCSTR zQrySrtDt, LPCSTR zQryEndDt, LPCSTR nkey) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4512) ? S_SDPBQ905(key, acno, pswd, zQrySrtDt, zQryEndDt, nkey) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4521(long key) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4521) ? S_SDPNQ060(key) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4522(long key, LPCSTR acno, LPCSTR pswd, LPCSTR zBankCode) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4522) ? S_SDPNQ031(key, acno, pswd, zBankCode) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4523(long key, LPCSTR nkey) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4523) ? S_SACMQ913(key, nkey) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4524(long key) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4524) ? S_SBPGT339(key) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4525(long key, LPCSTR zMnyoutAcntno) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4525) ? S_SDPNQ180(key, zMnyoutAcntno) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4526(long key, LPCSTR acno, LPCSTR pswd, LPCSTR nkey) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4526) ? S_SACAQ519(key, acno, pswd, nkey) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4527(long key, LPCSTR acno) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4527) ? S_SACMQ229(key, acno) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR8001(long key, LPCSTR acno)
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR8001) ? S_PIHOCUST(key, acno) : FALSE;
}

BOOL CIBKSConnectorCtrl::TR4403(long key, LPCTSTR acno, LPCTSTR pswd, long zFnoClssCode, LPCTSTR zCmsnAmtAddupCode, LPCTSTR zStrDt, LPCTSTR zEndDt, LPCTSTR nkey) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR4403) ? S_SONDQ304(key, acno, pswd, zFnoClssCode, zCmsnAmtAddupCode, zStrDt, zEndDt, nkey) : FALSE;
}

//2014.02.03 KSJ �����ں� ���ں� �Ÿŵ���
BOOL CIBKSConnectorCtrl::TR2001(long key, LPCTSTR upcd, long dtgb) 
{
	// TODO: Add your dispatch handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AccessTr(eTR2001) ? S_TR2001(key, upcd, dtgb) : FALSE;
}


void CIBKSConnectorCtrl::SaveConfig(LPCTSTR sSendName, LPCTSTR sKey, LPCTSTR sValue) 
{
	// TODO: Add your dispatch handler code here
	
	int key = -1;

	vector<char> buff(L_pidouini_mid, ' ');
	
	struct pidouini_mid *mid = (struct pidouini_mid*)&buff[0];

	CString sdat;
	CTime time;
	time = time.GetCurrentTime();
	sdat.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	CString fname = m_usnm + ".ini";
	mid->gubn[0] = 'I';

	memcpy(mid->item.usid, (LPCSTR)m_usid, m_usid.GetLength());
	memcpy(mid->item.innm, fname, fname.GetLength());
	memcpy(mid->item.senm, sSendName, strlen(sSendName));
	memcpy(mid->item.skey, sKey, strlen(sKey));
	memcpy(mid->item.valu, sValue, strlen(sValue));
	memcpy(mid->item.date, sdat, sdat.GetLength());

	//SendTR("pidouini", key, 0, (LPCSTR)&buff[0], buff.size(), DEF_CALLBACK);  //vc2019
	SendTR("pidouini", key, 0, (LPCSTR)&buff[0], buff.size(), &CIBKSConnectorCtrl::DEF_CALLBACK);
}

// ��ü����
void CIBKSConnectorCtrl::GoURL(long nType)
{
	if (!IsValidState())		return;
	if (!IsValidPTS())			return;
	
	
	CString strURL = "http://www.ibks.com";
	
	if(!m_svrip.Compare("172.16.202.106") || !m_svrip.Compare("211.255.204.134"))
		strURL = "http://test.ibks.com";
	
	switch (nType)
	{
	case TRANSFER_URL:
		strURL = strURL + "/LoadService.jsp?url=/online/TR/ITR00001_00.jsp&" + GetAuthParam();
		break;
		
	case CONTRACT_URL:
		strURL = strURL + "/LoadService.jsp?url=/online/SR/ISR00039_00.jsp&" + GetAuthParam();
		break;
		
	case CME_URL:
		strURL = strURL + "/LoadService.jsp?url=/online/SR/ISR00012_00.jsp&" + GetAuthParam();
		break;
		
	case QNA_URL:
		strURL = strURL + "/LoadService.jsp?url=/customer/faq/pts_qna_writeForm.do&" + GetAuthParam();
		break;
		
	default:
		m_sLastMsg = "���ǵ� Ÿ���� �ƴմϴ�.";
		break;
	}

	strURL += " 800 800";
	CString strFile = m_home + "\\exe";
	
	ShellExecute(NULL, "open", "VIEWURL.EXE", strURL, strFile, SW_SHOW);
}

CString CIBKSConnectorCtrl::GetAuthParam() 
{
	// TODO: Add your dispatch handler code here

	CString auth = "dt=";
	
	SYSTEMTIME st;
	CString strTime;
	GetLocalTime(&st);
	strTime.Format("%04d%02d%02d%02d%02d%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	
	auth += URLEncode(strTime);

	auth += "&id=" + URLEncode(m_usid);
	
	auth += "&pw=" + URLEncode(m_uspw);
	
// 	auth += "&certpw=" + URLEncode(m_certpw);

	auth = URLEncode(HTSEncode(auth, "ibkis"));

	auth = "auth=" + auth;

	return auth;
}

CString CIBKSConnectorCtrl::URLEncode(const char* lpszURL)
{
	CString dest("");
	
	struct Encode
	{
	private:
		BOOL isexcept(char ch)
		{
			const char excepts[] = {
				'_', '.', 0x00
			};
			
			for (int i = 0; excepts[i]; i++)
				if (ch == excepts[i])
					return TRUE;
				return FALSE;
		}
	public:
		CString value;
		
		Encode(char ch)
		{
			if (isalpha(ch) || (ch >= '0' && ch <= '9') || isexcept(ch))
				value = ch;
			else
				value.Format("%%%02X", ch);
		}
	};
	
	for (int i = 0; i < (int)strlen(lpszURL); i++)
	{
		dest += Encode(lpszURL[i]).value;
	}
	
	return dest;
}

CString CIBKSConnectorCtrl::HTSEncode(const char* lpszSource, const char* key)
{
	int lenKey = strlen(key);
	int lenSource = strlen(lpszSource);

	if (!lenSource || !lenKey)
		return "";

	LPSTR buffer = new char[lenSource + 1];

	for (int i = 0; i < lenSource; i++)
	{
		buffer[i] = (lpszSource[i] ^ (0x0f & key[i % lenKey])) - 1;
	}
	buffer[lenSource] = 0x00;

	CString result;
	result = buffer;

	/** Decoding test 

	for (i = 0; i < lenSource; i++)
	{
		buffer[i] = (result[i] + 1) ^ (0x0f & key[i % lenKey]);
	}

	AfxMessageBox(result);
	AfxMessageBox(buffer);
	**/


	delete [] buffer;

	return result;
}

CString CIBKSConnectorCtrl::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CIBKSConnectorCtrl::LoadIni()
{
	CString strFile;
	strFile.Format("%s\\%s", m_home, "IBKSConnector.ini");
	
	m_bSaveLog = (bool)(GetPrivateProfileInt("SAVELOG", "FLAG", 0, strFile) ? true : false);
}

void CIBKSConnectorCtrl::testSaveFile(int tr, LPCSTR datB, int size)
{
	CString strPath(_T("")), titletime;
	strPath.Format("%s\\saveLog.dat", m_home);
	
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	
	CString strCurTime;
	strCurTime.Format(_T("%02d:%02d:%02d"), stime.wHour, stime.wMinute, stime.wSecond);
	
	CStdioFile file;
	
	file.Open(strPath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	file.SeekToEnd(); 	
	
	titletime.Format("[%s][%d]", strCurTime, tr);
	file.Write(titletime, titletime.GetLength());
	file.SeekToEnd(); 	

	file.Write(datB, size);
	file.WriteString("\n");

	file.Close();	
}

void CIBKSConnectorCtrl::testSaveFile(LPCSTR gubn, LPCSTR datB, int size)
{
	CString strPath(_T("")), titletime;
	strPath.Format("%s\\saveLog.dat", m_home);
	
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	
	CString strCurTime;
	strCurTime.Format(_T("%02d:%02d:%02d"), stime.wHour, stime.wMinute, stime.wSecond);
	
	CStdioFile file;
	
	file.Open(strPath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	file.SeekToEnd(); 	
	
	titletime.Format("[%s][%s]", strCurTime, gubn);
	file.Write(titletime, titletime.GetLength());
	file.SeekToEnd(); 	
	
	file.Write(datB, size);
	file.WriteString("\n");
	
	file.Close();	
}

//�����ڵ�\t�����ϼ�\t��簡\tcd�ݸ�\t���簡\t�����ڻ�
BSTR CIBKSConnectorCtrl::GetGreeksData(LPCTSTR sData) 
{
	// TODO: Add your dispatch handler code here
	CString strResult;
	strResult = CalcGreeks(sData);

	if (IsInit() && IsLogin())
		return strResult.AllocSysString();
	else
		return CString("").AllocSysString();
}

//Return : ��Ÿ\t����\t��Ÿ\t����\t��\tIV(���纯����)
CString CIBKSConnectorCtrl::CalcGreeks(CString sData) 
{
	OPTION ins;
	CString strReturn;
	double iv, delta, gamma, theta, vegga, rho;
	double high = 1, low = 0, ret = 0, d1, d2;

	strcpy(ins.code, Parser(sData));		//�����ڵ�
	ins.remainDays = atof(Parser(sData));	//�����ϼ�
	ins.strike = fabs(atof(Parser(sData)));		//��簡
	ins.cd = atof(Parser(sData))/100;		//cd�ݸ�
// 	ins.baedangJisu = atof(Parser(sData));	//�������
	ins.currAmt = fabs(atof(Parser(sData)));		//���簡
	ins.underlying = fabs(atof(Parser(sData)));	//�����ڻ�	-�� �ö��� �����Ƿ� ���밪�� �־��ش�.
// 	ins.volatility = atof(Parser(sData));	//������������

	double thoery, curr = ins.currAmt / ins.underlying;

	iv = 0.0;
	delta = 0.0;
	gamma = 0.0;
	theta = 0.0;
	vegga = 0.0;
	rho = 0.0;
	if (ins.strike == 0.0 || ins.remainDays == 0.0 || ins.underlying == 0.0)
		return "";
	
	// IV ���
	for (;;) {
		if (high - low <= 0.0001)
			break;
		
		ins.volatility = (high + low) / 2;
		thoery = GetOptionTheoryAmt(&ins);
		
		if (thoery / ins.underlying > curr)
			high = (high + low) / 2;
		else
			low = (high + low) / 2;
	}
	iv = ((high + low) / 2);
	ins.volatility = iv;
	
	// ���뺯��
	d1 = (log((ins.underlying/* - in->baedangJisu*/) / ins.strike) + (ins.cd + pow(ins.volatility, 2) / 2) * ins.remainDays) /
		(ins.volatility * sqrt(ins.remainDays));
	d2 = d1 - ins.volatility * sqrt(ins.remainDays);
	
	// Delta 
	{
		delta = CNormal(d1);
		if (CallPut(ins.code) == PUT)
			delta = delta - 1;
	}
	
	// Gamma
	{
		gamma = GetND(d1) / ((ins.underlying/* - in->baedangJisu*/) * ins.volatility * sqrt(ins.remainDays)/*pow(in->remainDays, 0.5)*/);
	}
	
	// Theta
	{
		if (CallPut(ins.code) == CALL) 
			ret = ((-1 * (ins.underlying/* - in->baedangJisu*/) * GetND(d1) * ins.volatility / (2 * sqrt(ins.remainDays))) -
			ins.cd * ins.strike * exp(-1 * ins.cd * ins.remainDays) * CNormal(d2)) / 100;
		else
			ret = ((-1 * (ins.underlying/* - in->baedangJisu*/) * GetND(d1) * ins.volatility / (2 * sqrt(ins.remainDays))) +
			ins.cd * ins.strike * exp(-1 * ins.cd * ins.remainDays) * CNormal(-1 * d2)) / 100;
		theta = (ret / 3.65);
	}
	
	// vegga
	{
		vegga = (ins.underlying/* - in->baedangJisu*/) * sqrt(ins.remainDays) * GetND(d1) / 100;
	}
	
	// rho
	{
		int flag = CallPut(ins.code) == CALL ? 1: -1;
		rho = flag * ins.strike * ins.remainDays * exp(-1 * ins.cd * ins.remainDays) * CNormal(flag * d2) / 100;
	}

	strReturn.Format("%.04f\t%.04f\t%.04f\t%.04f\t%.04f\t%.02f\t", delta, gamma, theta, vegga, rho, iv*100);

	return strReturn;
}

int CIBKSConnectorCtrl::CallPut(LPSTR code)
{
	switch (code[0]) {
	case '2': return CALL;
	case '3': return PUT;
	}
	
	return 0;
}

double CIBKSConnectorCtrl::GetND(double value)
{
	return  1/sqrt(2 * PI) * exp(-1*pow(value, 2)/2);
}

double CIBKSConnectorCtrl::CNormal(double value)
{
	double i = 0, phai = 0, ret = 0;
	
	i = 1 / (1 + GAMMA * value);
	phai = 3.1415924814;
	
	if (value >= 0) 
		ret = 1 - (((1 / (pow((2 * phai), 0.5))) * exp(-1 * pow(value, 2) / 2)) * 
		(CNORMAL_1 * i + CNORMAL_2 * pow(i, 2) + CNORMAL_3 * pow(i, 3) + CNORMAL_4 * pow(i, 4) + CNORMAL_5 * pow(i, 5)));
	else
		ret = 1 - CNormal(-1 * value);
	
	return ret;
}

double CIBKSConnectorCtrl::GetOptionTheoryAmt(POPTION ins)
{
	if (ins->strike == 0.0 || ins->volatility == 0.0 || ins->remainDays == 0.0)
		return 0.0;
	
	double d1 = 0, d2 = 0, ret = 0;
	
	d1 = (log((ins->underlying/* - in->baedangJisu*/) / ins->strike) + (ins->cd + pow(ins->volatility, 2) / 2) * ins->remainDays) /
		(ins->volatility * pow(ins->remainDays, 0.5));
	d2 = d1 - (ins->volatility * pow(ins->remainDays, 0.5));
	
	ret = (ins->underlying/* - in->baedangJisu*/) * CNormal(d1) - ins->strike * exp(-1 * ins->cd * ins->remainDays) * CNormal(d2);
	
	if (CallPut(ins->code) == PUT)
		ret = ret - (ins->underlying/* - in->baedangJisu*/) + ins->strike * exp(-1 * ins->cd * ins->remainDays);
	//CString sTest;
	//sTest.Format("IC >>code:%s jisu:%f bdjs:%f iron:%f hv:%f hsga:%f cdkr:%f dbjjmg:%f bdjs:%f d1:%f d2:%f\n", 
	//	     in->code, in->underlying, in->baedangJisu, ret, in->volatility, in->strike, in->cd, in->remainDays, in->baedangJisu, d1, d2);
	//OutputDebugString(sTest);
	
	return ret;
}

CString CIBKSConnectorCtrl::GetEncPassword(CString sPswd)
{
	CString dllPath;
	dllPath.Format(_T("%s%s"), m_home, _T("\\dev\\CX_SHA256.DLL"));
	CString strRetrun;
	
	if(m_hiSha256 == NULL)
	{
		m_hiSha256 = LoadLibrary(dllPath);
		
		if (!m_hiSha256)
		{
			TRACE("CX_SHA256 ��Ʈ�� ���� ����1");
			return "";
		}
	}
	
	if(m_hiSha256)
	{
		typedef long (WINAPI *GETSHAFUNC)(char*, int);	
		GETSHAFUNC func = (GETSHAFUNC)GetProcAddress(m_hiSha256, "axEncrypt");
		
		if (func)
		{
			strRetrun =  (char*)func(sPswd.GetBuffer(sPswd.GetLength()),sPswd.GetLength());
		}
	}	
	return strRetrun;
}

//00:�Ϲ� 01:���� 03:������ 17:ETN���Ͱŷ��̸鼭 ���������Ͱŷ��� ���
//18:ETN���Ͱŷ��̸鼭 ���������Ͱŷ��� �ƴ� ��� 25:ETN�����ŷ�
void CIBKSConnectorCtrl::SetPrograms(long pggb) 
{
	// TODO: Add your dispatch handler code here
	switch(pggb)
	{
	case 0:		//�Ϲ�
		m_nPggb = 0;
		break;
	case 1:		//����
		m_nPggb = 1;
		break;
	case 2:		//������
		m_nPggb = 3;
		break;
	case 3:		//ETN���Ͱŷ��̸鼭 ���������Ͱŷ��� ���
		m_nPggb = 17;
		break;
	case 4:		//ETN���Ͱŷ��̸鼭 ���������Ͱŷ��� �ƴ� ���
		m_nPggb = 18;
		break;
	case 5:		//ETN�����ŷ�
		m_nPggb = 25;
		break;
	}
}

void CIBKSConnectorCtrl::C_SACEQ320( WPARAM wParam, LPARAM lParam )
{
	int key = LOWORD(wParam);
	int size = HIWORD(wParam);
	
	struct ledger *ledger = (struct ledger *)lParam;
	struct saceq320_mod *mod = (struct saceq320_mod *)( (LPCSTR)lParam + L_ledger);
	
	if(m_bSaveLog)
		testSaveFile("saceq320_mod", (LPCSTR)lParam + L_ledger + L_sacaq504_mid, size - L_ledger - L_sacaq504_mid);
  
	vector<CString> acnt,acnm;

	acnt.clear();
	acnm.clear();

	CString sRecord = CString(mod->In,5);

	int nCnt = atoi(sRecord);

	for(int i = 0;i < nCnt; i++)
	{
		CString sAcnt = CString(mod->grid[i].Acnt,20);

		sAcnt.TrimLeft();
		sAcnt.TrimRight();

		if (ledger->next[0] == 'Y')	//2014.11.25 KSJ �̰�δ븮���� ������ ���Ƽ� FEP�� �����. ����ó����.
		{
			memcpy(ledger->fkey, "7   ", 4);
			//SendTR("piboPBxQ", key, US_ENC, (LPCSTR)lParam, L_ledger + L_saceq320_mid, C_SACEQ320);  //vc2019
			SendTR("piboPBxQ", key, US_ENC, (LPCSTR)lParam, L_ledger + L_saceq320_mid, &CIBKSConnectorCtrl::C_SACEQ320);
		}
		else
		{			
			for(size_t n=0; n<m_acno.size(); ++n)
			{
				if (sAcnt==m_acno[n])
				{
					acnt.push_back(sAcnt);
					acnm.push_back(m_acnm[n]);
// 					m_acno.erase(m_acno.begin()+n);
// 					m_acnm.erase(m_acnm.begin()+n);
					break;
				}
			}
		}
	}

	m_acno.clear();
	m_acnm.clear();

	for(int j = 0; j < acnt.size() ; j++)
	{
		m_acno.push_back(acnt[j]);
		m_acnm.push_back(acnm[j]);
	}

	//m_chk_acno.erase(key);
	InitJanGo();
}

#pragma warning (default : 4996)