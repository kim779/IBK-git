// WizardCtl.cpp : Implementation of the CWizardCtrl ActiveX Control class.

#include "stdafx.h"
#include "Wizard.h"
#include "WizardCtl.h"
#include "WizardPpg.h"
#include "Client.h"
#include "Key.h"
#include "xsystem.h"
#include "../h/mapform.h"
#include "../h/axisvar.h"
#include "../h/axisanm.h"
#include "../h/axiserr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	structure for caFULL
//
struct	_cainfo	{
	char	caL[5];
	char	dnL[5];
};
#define	L_CAINFO	sizeof(struct _cainfo)

IMPLEMENT_DYNCREATE(CWizardCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CWizardCtrl, COleControl)
	//{{AFX_MSG_MAP(CWizardCtrl)
	ON_WM_CREATE()
	ON_MESSAGE(WM_USER, OnFireAxis)
	ON_MESSAGE(WM_GUIDE, OnGuide)
	ON_MESSAGE(WM_VERS, OnVersion)
	ON_MESSAGE(WM_ANM, OnFireAlert)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CWizardCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CWizardCtrl)
	DISP_FUNCTION(CWizardCtrl, "RunAxis", RunAxis, VT_BOOL, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CWizardCtrl, "axTR", axTR, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(CWizardCtrl, "axAttach", axAttach, VT_I4, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CWizardCtrl, "axDetach", axDetach, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CWizardCtrl, "axWorkShop", axWorkShop, VT_BOOL, VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CWizardCtrl, "axWorkName", axWorkName, VT_I4, VTS_I4)
	DISP_FUNCTION(CWizardCtrl, "axWorkView", axWorkView, VT_I4, VTS_BSTR VTS_I4)
	DISP_FUNCTION(CWizardCtrl, "axWizard", axWizard, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CWizardCtrl, "axInvoke", axInvoke, VT_I4, VTS_I4 VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CWizardCtrl, COleControl)
	//{{AFX_EVENT_MAP(CWizardCtrl)
	EVENT_CUSTOM("OnFire", OnFire, VTS_I4  VTS_I4  VTS_I4)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()

BEGIN_EVENTSINK_MAP(CWizardCtrl, COleControl)
	//{{AFX_EVENTSINK_MAP(CWizardCtrl)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CWizardCtrl, -1, 1, OnFireEvent, VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CWizardCtrl, 1)
	PROPPAGEID(CWizardPropPage::guid)
END_PROPPAGEIDS(CWizardCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

//IMPLEMENT_OLECREATE_EX(CWizardCtrl, "AxisWizard.WizardCtrl.IBK",  //vc2019 ibk ibk2019
IMPLEMENT_OLECREATE_EX(CWizardCtrl, "AxisWizard.WizardCtrl.IBK2019",
	0x7b26de02, 0xe5ea, 0x4ed5, 0xaa, 0x20, 0xfb, 0x9, 0x40, 0x62, 0x36, 0x6c)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CWizardCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DWizard =
		{ 0x11b7db3b, 0x47c, 0x49d3, { 0x91, 0xfb, 0x89, 0x4c, 0xb1, 0xbe, 0x9c, 0x76 } };
const IID BASED_CODE IID_DWizardEvents =
		{ 0x4c00cf12, 0x7418, 0x4b50, { 0x8a, 0xbf, 0x32, 0x2d, 0xf, 0x61, 0xec, 0xfb } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwWizardOleMisc =
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CWizardCtrl, IDS_WIZARD, _dwWizardOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CWizardCtrl::CWizardCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CWizardCtrl

BOOL CWizardCtrl::CWizardCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.
	CString slog;
	slog.Format("[axwizard][UpdateRegistry] ------------------------ bRegister =[%d]\n", bRegister);
	OutputDebugString(slog);
	CString strPath;
	if (GetModuleFileName(nullptr, strPath.GetBuffer(_MAX_PATH + 1), MAX_PATH) != FALSE)
	{
		strPath.ReleaseBuffer();
		CString slog, strResult;
		strResult = strPath.Left(strPath.ReverseFind('\\') + 1);
		slog.Format("[axwizard][CWizardCtrl]--------------------[%s] \n", strResult);
		OutputDebugString(slog);
	}
	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_WIZARD,
			IDB_WIZARD,
			afxRegApartmentThreading,
			_dwWizardOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CWizardCtrl::CWizardCtrl - Constructor

CWizardCtrl::CWizardCtrl()
{
	InitializeIIDs(&IID_DWizard, &IID_DWizardEvents);

	m_mode  = mtNO;
	m_xtype = xtWS;
	m_guard = new CGuard;

	m_axisL = 0;
	m_axisB = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CWizardCtrl::~CWizardCtrl - Destructor

CWizardCtrl::~CWizardCtrl()
{
	if (m_guard->m_xecure)
	{
		delete m_guard->m_xecure;
		m_guard->m_xecure = NULL;
	}

	if (m_guard->m_certify)
	{
		delete m_guard->m_certify;
		m_guard->m_certify = NULL;
	}

	delete m_guard;

	if (m_axisL && m_axisB)
		delete [] m_axisB;
}


/////////////////////////////////////////////////////////////////////////////
// CWizardCtrl::OnDraw - Drawing function

void CWizardCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{

}


/////////////////////////////////////////////////////////////////////////////
// CWizardCtrl::DoPropExchange - Persistence support

void CWizardCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
}


/////////////////////////////////////////////////////////////////////////////
// CWizardCtrl::OnResetState - Reset control to default state

void CWizardCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
}


/////////////////////////////////////////////////////////////////////////////
// CWizardCtrl message handlers

void CWizardCtrl::OnFireEvent(int type, char* pBytes, int nBytes)
{
	switch (type)
	{
	case FEV_ANM:
		OnAlert(pBytes, nBytes);
		break;
	case FEV_PUSH:
		OnPush(pBytes, nBytes);
		break;
	case FEV_AXIS:
		OnRead(pBytes, nBytes);
		break;
	case FEV_ERROR:
		OnFire(type, 0, (long)pBytes);
		if (m_mode == mtSIGN)
			OnFire(FEV_AXIS, MAKEWPARAM(loginERR, axisERR), 0);
		break;
	case FEV_CLOSE:
		m_mode = mtNO;
		OnFire(FEV_CLOSE, 0, 0);
		break;
	case FEV_STAT:
	case FEV_SIZE:
		if (m_mode != mtRUN)
			OnFire(type, nBytes, (long)pBytes);
		break;
	case FEV_VERS:
		m_guard->UpdateVers(pBytes, nBytes);
		break;
	case FEV_CA:
		OnCertify(pBytes, nBytes);
		break;
	default:
		OnAxis(type, pBytes, nBytes);
		break;
	}
}

int CWizardCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	return m_guard->Initial(this);
}

LONG CWizardCtrl::OnFireAxis(WPARAM wParam, LPARAM lParam)
{
	OnFire(FEV_AXIS, wParam, lParam);
	return 0;
}

LONG CWizardCtrl::OnGuide(WPARAM wParam, LPARAM lParam)
{
	if (lParam)
		OnFire(FEV_ERROR, wParam, lParam);
	else
		OnFire(FEV_GUIDE, LOWORD(wParam), HIWORD(wParam));
	return 0;
}

LONG CWizardCtrl::OnVersion(WPARAM wParam, LPARAM lParam)
{
	OnFire(FEV_VERS, wParam, lParam);
	return 0;
}

LONG CWizardCtrl::OnFireAlert(WPARAM wParam, LPARAM lParam)
{
	OnFire(FEV_ANM, wParam, lParam);
	return 0;
}

BOOL CWizardCtrl::RunAxis(long mode, long pBytes, long nBytes) 
{
	BOOL	retv;
	CString slog;


	switch (mode)
	{
	case loginSHOP:
		m_xtype = xtRM;
		break;
	case loginAXIS:
		m_xtype = xtWS;
		break;
	case loginAXISx:
		m_xtype = xtXEC;
		break;

	case signUSER:
	case signUSERc:
		switch (m_mode)
		{
		case mtCON:
			break;
		case mtRUN:
			m_guard->CertifyId((char *)pBytes, true);
		case mtNO:
		default:
			return TRUE;
		}
		m_guard->CertifyId((char *)pBytes);
		if (m_guard->Login(mode, (char *)pBytes, nBytes, m_xtype == xtXEC))
		{
			m_mode = mtSIGN;
			return TRUE;
		}
		return FALSE;

	case listenUDP:
		if (nBytes < 0)
		{

slog.Format("[WIZARD][CWizardCtrl](DI_DCLOSE)  RunAxis nBytes = [%d] ", nBytes);
OutputDebugString(slog);

			m_guard->m_sock->InvokeHelper(DI_DCLOSE, DISPATCH_METHOD, VT_EMPTY, NULL, VTS_NONE);
			return TRUE;
		}

slog.Format("[WIZARD][CWizardCtrl](DI_DOPEN)  RunAxis nBytes = [%d] ", nBytes);
OutputDebugString(slog);


		m_guard->m_ips = (char *)pBytes;
		m_guard->m_sock->InvokeHelper(DI_DOPEN, DISPATCH_METHOD, VT_BOOL, (void*) &retv,
									(BYTE *)VTS_I4, nBytes);
		return retv;

	case signUSERx:
		m_guard->Startup();
		return TRUE;

	default:
		return FALSE;
	}

	if (nBytes == 0 || (pBytes && !strcmp((char *)pBytes, "0.0.0.0")))
	{
		m_guard->Startup();
		PostMessage(WM_USER, runAXIS);
		return TRUE;
	}

	m_guard->Cleanup();
	if (nBytes < 0)
		return TRUE;

slog.Format("[WIZARD][CGuard](RunAxis) ");
OutputDebugString(slog);

	m_guard->m_sock->InvokeHelper(DI_OPEN, DISPATCH_METHOD, VT_BOOL, (void*) &retv,
			(BYTE *)VTS_BSTR VTS_BSTR VTS_I4, m_guard->m_root, pBytes, nBytes);
	return retv;
}

BOOL CWizardCtrl::axTR(long pBytes, long nBytes) 
{
	return m_guard->Write((char *)pBytes, (int)nBytes, winK_FMX);
}

long CWizardCtrl::axAttach(long view, long type, long key) 
{
	return m_guard->Attach((CWnd*)view, type, key);
}

void CWizardCtrl::axDetach(long key) 
{
	m_guard->Detach(key);
}

BOOL CWizardCtrl::axWorkShop(long key, LPCTSTR maps, long size, BOOL only) 
{
	CWorks*	works;

	if (!m_guard->GetClient(key, works))
		return FALSE;

	works->m_view->GetClientRect(&works->m_rect);
	works->m_rect.right  = works->m_rect.left + LOWORD(size);
	works->m_rect.bottom = works->m_rect.top + HIWORD(size);
	return works->Attach(maps, only ? true : false);
}

long CWizardCtrl::axWorkName(long key) 
{
	CWorks*	works;

	if (!m_guard->GetClient(key, works))
		return 0;
	return (long)works->m_mapN.operator LPCTSTR();
}

long CWizardCtrl::axWorkView(LPCTSTR maps, long size) 
{
	return m_guard->WorkView(maps, (int *)size);
}

long CWizardCtrl::axWizard(long kind, long variant) 
{
	CWorks* works;
	CString	text;
	int	value;
	static CString	tmps;

	CString slog;

	switch (LOWORD(kind))
	{
	case loadACC:
		m_guard->GetAccounts(tmps);
		return (long)tmps.operator LPCTSTR();
	case saveACC:
		m_guard->SetAccounts((char *)variant);
		break;
	case setGID:
		if (m_guard->GetClient(LOWORD(variant), works))
			works->SetGID(short(HIWORD(variant)));
		break;
	case getFDC:
		tmps.Empty();
		text = (char *)variant;
		if (m_guard->GetClient(HIWORD(kind), works))
		{
			if (works->GetData(text, tmps))
				works->GetAt(text, tmps);
		}
		return (long)tmps.operator LPCTSTR();
	case setFDC:
		if (HIWORD(kind) & 0xff00)
		{
			if (m_guard->GetClient(HIWORD(kind) & 0x00ff, works))
			{
				int	idx;
				tmps = (char *)variant;
				if ((idx = tmps.Find('\t')) != -1)
					m_guard->OnTrigger(works, (char *)tmps.Left(idx).operator LPCTSTR(), tmps.Mid(idx+1));
			}
			break;
		}
		if (m_guard->GetClient(HIWORD(kind), works))
		{
			int	idx;
			tmps = (char *)variant;
			if ((idx = tmps.Find('\t')) != -1)
				works->OnTrigger(tmps.Left(idx), tmps.Mid(idx+1), -1);
			else
				works->OnTrigger(_T(""), tmps, -1);
		}
		break;
	case setFCB:
		if (variant)
			m_guard->m_status |= WS_FCB;
		else
			m_guard->m_status &= ~WS_FCB;
		m_guard->SetFCB(variant ? true : false, m_guard->m_flash);
		break;
	case setPAL:
		m_guard->SetPalette();
		break;
	case devEV:
		if (m_guard->GetClient(HIWORD(kind), works) && works->isWorks())
			((CClient *)works)->OnDevice();
		break;
	case homeEV:
		if (m_guard->GetClient(HIWORD(kind), works) && works->isWorks())
			((CClient *)works)->m_keyx->OnHome(false);
		break;
	case setVID:
		m_guard->m_vid = short(HIWORD(variant));
		break;
	case setTRG:
		m_guard->SetTriggerMode(HIWORD(kind), HIWORD(variant));
		break;
	case setFONT:
		return m_guard->SetFont(HIWORD(variant));
	case encPASS:
		tmps = (char *)variant;
		if (!tmps.IsEmpty())
		{
			int	idx = tmps.Find('\t');
			if (idx == -1)
				idx = tmps.GetLength();
			tmps = m_guard->Secure(tmps.Left(idx), (idx < tmps.GetLength()) ? tmps.Mid(idx+1) : _T(""), HIWORD(kind));
		}
		return (long)tmps.operator LPCTSTR();
	case encSHA:
	case encSHAx:	/* ??? */
		text = (char *)variant;
		if (!text.IsEmpty())
			text = m_guard->Secure2(text);
		return (long)text.operator LPCTSTR();
	case setSCN:
		if (m_guard->GetClient(HIWORD(kind), works) && works->isWorks())
			((CClient *)works)->SetLedger((char *)variant);
		break;
	case setFONTx:
		return m_guard->SetFont(HIWORD(variant), true, (short)HIWORD(kind));
	case applyACC:
		m_guard->OnAccount();
		break;
	case getCA:
		m_guard->GetCetifyInfo(tmps);
		return (long)tmps.operator LPCTSTR();
	case getLEDGER:
		m_guard->Ledger((char *)variant);
		break;
	case keyEV:
		if (m_guard->GetClient(HIWORD(kind), works) && works->isWorks())
			return ((CClient *)works)->OnKey(LOWORD(variant));
		break;
	case getFOCUS:
		return m_guard->GetFocus();
	case setACCG:
		if (HIWORD(kind))
			m_guard->m_future_accs = (char *)variant;
		else
			m_guard->m_stock_accs = (char *)variant;
		break;
	case setACCB:
		m_guard->AddRegistry((char *)variant, HIWORD(kind), true);
		break;
	case setNOMSG:
		if (variant)
			m_guard->m_status |= WS_NOMSG;
		else
			m_guard->m_status &= ~WS_NOMSG;
		break;
	case getRTS:

slog.Format("[WIZARD][axWizard](getRTS) ");
OutputDebugString(slog);

		m_guard->m_sock->InvokeHelper(DI_ENUM, DISPATCH_METHOD, VT_I4, (void*)&value,
							(BYTE*) VTS_I4 VTS_I4 VTS_I4, NULL, NULL, enumRTS);
		return value;
	case getNDC:
		tmps.Empty();
		m_guard->PopData((char *)variant, tmps);
		return (long)tmps.operator LPCTSTR();
	case setNDC:
		m_guard->PushData((char *)variant);
		break;
	case getDEPT:
		text = m_guard->GetLoginData(0);
		return (long)text.operator LPCTSTR();
	case getHISTORY:
		tmps = _T("");
		m_guard->GetCodes((char *)variant, tmps, true);
		return (long)tmps.operator LPCTSTR();
	case delHISTORY:
		m_guard->RemoveCode((char *)variant, false);
		break;
	case caFULL:
		{
			char	wb[1024*8];
			int	desL, len = L_CAINFO;
			struct	_cainfo	*cainfo;
			char*	ptr = (char *)variant;

			value = HIWORD(kind);
			if (value <= 0 || ptr == NULL)
				return -1;

			m_guard->OnCertify(NULL, 0);
			value = m_guard->CertifyFull(ptr, value, wb, desL);

			cainfo = (struct _cainfo *)ptr;
			memset(cainfo, 0x20, L_CAINFO);
			if (desL > 0)
			{
				desL = m_guard->m_xsystem->ConvertHEX(wb, desL);
				CopyMemory(&ptr[len], wb, desL);
				tmps.Format("%05d", desL);
				CopyMemory(cainfo->caL, tmps, sizeof(cainfo->caL));
				len += desL;
			}
			desL = m_guard->CertifyName(wb);
			if (desL > 0)
			{
				CopyMemory(&ptr[len], wb, desL);
				tmps.Format("%05d", desL);
				CopyMemory(cainfo->dnL, tmps, sizeof(cainfo->dnL));
				len += desL;
			}
			return value;
		}
		break;
	case setTRACE:
		m_guard->xTRACE(HIWORD(kind), (HWND)variant);
		break;
	}
	return 0;
}

long CWizardCtrl::axInvoke(long wParam, long lParam) 
{
	CWorks*	works;

	if (m_guard->GetClient(HIWORD(wParam), works))
		return works->OnInvoke(wParam, lParam);
	return 0;
}

void CWizardCtrl::OnAlert(char* pBytes, int nBytes)
{
	m_guard->OnAlert(pBytes, nBytes);
}

void CWizardCtrl::OnAlert(int type, char* pBytes, int nBytes)
{
	int	key;
	CString	code, text;

	text = CString(pBytes, nBytes);
//	m_guard->xTRACE(x_RTMs, (char*)text.operator LPCTSTR());

	key = text.Find('\t');
	if (key == -1)	return;

/*	if (type & stat_TICK)
		OnFire(FEV_ANM, type & stat_TICK, (long)text.Mid(key+1).operator LPCTSTR());
	else
		OnFire(FEV_ANM, 0, (long)text.operator LPCTSTR());
*/
	code = text.Left(key++);
	pBytes += key;
	nBytes -= key;
	m_guard->OnAlert(code, pBytes, nBytes, type);
}

void CWizardCtrl::OnPush(char* pBytes, int nBytes)
{
	int	anmL;
	struct _anmH*	anmH;

	if (nBytes < L_anmH)
		return;

	anmH = (struct _anmH *)pBytes;
	anmL = atoi(CString(anmH->datL, sizeof(anmH->datL)));
	if (anmL > int(nBytes - L_anmH))
		return;

	switch (anmH->anmK)
	{
	case anmK_ALIVE:
		break;
	case anmK_PUSH:
		m_guard->OnPush(CString(&pBytes[L_anmH], anmL));
		break;
	}
}

void CWizardCtrl::OnRead(char* pBytes, int nBytes)
{
	struct _axisH *axisH;
	int	axisL;
	char	*chain;
	CString	text;

	while (nBytes >= L_axisH)
	{
		axisH = (struct _axisH*) pBytes;
		pBytes += L_axisH;
		nBytes -= L_axisH;

		axisL = atoi(CString(axisH->datL, sizeof(axisH->datL)));
		if (axisL > nBytes)
			break;

		chain = pBytes;
		pBytes += axisL;
		nBytes -= axisL;

		if (axisH->stat & statENC && !m_guard->Xecure(DI_DEC, chain, axisL))
		{
			OnFire(FEV_GUIDE, axisH->winK, AE_SSECURE);
			continue;
		}

		switch (axisH->msgK)
		{
		case msgK_SIGN:
		case msgK_SIGNx:
			OnStream(axisH, chain, axisL);
			break;
		case msgK_RSM:
			m_guard->OnRsm(axisH, chain, axisL);
			break;
		case msgK_AXIS:
		case msgK_TAB:
			switch (axisH->winK)
			{
			case winK_FMX:
				if (OnAux(axisH, chain, axisL))
					OnFire(FEV_FMX, MAKEWPARAM(axisH->unit, axisL), (LPARAM)chain);
				continue;
			case winK_DDE:
				if (OnAux(axisH, chain, axisL))
					m_guard->OnDDE(CString(chain, axisL), axisH->unit);
				continue;
			default:
				break;
			}
		case msgK_HTM:
		case msgK_SVC:
		case msgK_APC:
		case msgK_CTRL:
		case msgK_UPF:
		case msgK_DNF:
			OnAxis(axisH, chain, axisL);
			break;

		case msgK_XCA:
		case msgK_ENC:
			OnXecure(axisH->unit, chain, axisL);
			break;

		case msgK_TICK:
			text = CString(chain, axisL);
			m_guard->OnNotice(chain, axisL);
			OnFire(FEV_AXIS, MAKEWPARAM(noticePAN, true),
					(LPARAM)text.operator LPCTSTR());
			break;
		case msgK_POP:
			OnFire(FEV_AXIS, MAKEWPARAM(alarmPAN, true),
					(LPARAM)CString(chain, axisL).operator LPCTSTR());
			break;
		case msgK_ARM:
			OnFire(FEV_AXIS, MAKEWPARAM(alarmPAN, false),
					(LPARAM)CString(chain, axisL).operator LPCTSTR());
			break;
		case msgK_AUX:
			OnAlert(0, chain, axisL);
			break;
		case msgK_DIAL:
			OnFire(FEV_AXIS, MAKEWPARAM(dialogPAN, axisH->winK),
					(LPARAM)CString(chain, axisL).operator LPCTSTR());
			break;
		}
	}
}

BOOL CWizardCtrl::OnAxis(int type, char* pBytes, int variant)
{
	BOOL	retv;
	int	value, nBytes;

	switch (type)
	{
	case FEV_OPEN:
		if (variant)
		{
			OnFire(type, 0, variant);
			return TRUE;
		}

		Sleep(500);	// wait for establish
				// 250 -> 500 : for wireless (080215)
		nBytes = (m_xtype == xtRM) ? modeWS : modeRM;
		m_mode = mtCON;
		break;

	case FEV_RUN:
		if (m_mode == mtNO)
			return TRUE;

		if (variant)
		{
			m_mode = mtNO;
			OnFire(type, 0, variant);
			return TRUE;
		}
		nBytes = modeWS;
		break;

	default:
		return TRUE;
	}

	if (nBytes == modeRM)
	{
		value = m_guard->m_app->GetProfileInt(WORKSTATION, AXISONLY, 0);
		if (value > 0)
			nBytes = modeRMx;
	}
	value = m_guard->m_app->GetProfileInt(WORKSTATION, DOWNLOAD, 0);
	if (value > 0)
		nBytes = -nBytes;

CString slog;
slog.Format("[WIZARD][OnAxis]");
OutputDebugString(slog);

	m_guard->m_sock->InvokeHelper(DI_AXIS, DISPATCH_METHOD, VT_BOOL, (void*)&retv,
							(BYTE*) VTS_I4 VTS_I4, pBytes, nBytes);

	if (m_xtype == xtXEC && type == FEV_RUN)
	{
		m_mode = mtXEC;
		OnFire(FEV_GUIDE, 0, AE_SECURE);
		if (Xecure(NULL, 0))
			return TRUE;

		m_xtype = xtWS;
		m_mode  = mtCON;
	}
	OnFire(type, 0, variant);
	return retv;
}

void CWizardCtrl::OnSign(int signK, char *pBytes, int nBytes)
{
	if (nBytes < L_signR)
		return;

	CString	dns;
	struct	_signR *sign;

	sign = (struct _signR *) pBytes;
	sign->guide[sizeof(sign->guide)-1] = '\0';  

	switch (sign->signK)
	{
	case signON:
	case signOX:
		dns = _T("");
		m_guard->Sign(signK, (char *)sign, nBytes, dns, m_xtype == xtRM);
		break;
	case signER:
		OnFire(FEV_ERROR, true, (long)sign->guide);
		if (m_mode == mtSIGN)
		{
			m_mode = mtCON;
			OnFire(FEV_AXIS, MAKEWPARAM(loginERR, signERR), 0);
		}
		else
			OnFire(FEV_AXIS, MAKEWPARAM(loginERR, axisERR), 0);
		return;
	}

	OnFire(FEV_ERROR, 0, (long)sign->guide);
	if (m_mode != mtSIGN)
		return;

	if (!m_guard->Certify(FALSE, sign->flag & flagCA, sign->flag & flagCAX, sign->flag & flagXCS))
	{
		m_mode = mtCON;
		OnFire(FEV_GUIDE, 0, AE_CLEAR);
		OnFire(FEV_AXIS, MAKEWPARAM(loginERR, certifyERR), 0);
		return;
	}

	if ((sign->flag & flagCAX) && !(sign->flag & flagXCS))
		sign->flag &= ~flagCA;
	if (sign->flag & flagENX)
		sign->flag &= ~flagENC;
	m_guard->m_term = sign->flag;


	if (sign->flag & flagCA)
	{
		if (dns.IsEmpty())
		{
			m_mode = mtCON;
			OnFire(FEV_GUIDE, 0, AE_EDN);
			OnFire(FEV_AXIS, MAKEWPARAM(loginERR, certifyERR), 0);
			return;
		}


		switch (signK)
		{
		case msgK_SIGNx:
			m_guard->OnCertify((char *)dns.operator LPCTSTR(), dns.GetLength());
			break;
		case msgK_SIGN:
		default:
			m_mode = mtCA;
			OnFire(FEV_GUIDE, 0, AE_CERTIFY);

			m_guard->OnCertify(NULL, 0);
			switch (m_guard->OnCertify((char *)dns.operator LPCTSTR(), dns.GetLength()))
			{
			case -1:
				m_mode = mtCON;
				OnFire(FEV_GUIDE, 0, AE_ECERTIFY);
				OnFire(FEV_AXIS, MAKEWPARAM(loginERR, certifyERR), 0);
				return;
			case -3:	// terminate AXIS...reboot
				OnFire(FEV_AXIS, MAKEWPARAM(loginERR, certifyERR), 0);
				PostMessage(WM_USER, MAKEWPARAM(closeAXIS, true));
				return;
			default:
				if (m_guard->m_term & flagXCS)
				{
					if (sign->flag & flagCAX)
						sign->flag &= ~flagCA;
					break;
				}
				return;
			}
			break;
		}
	}

	Xecure();
	if (m_guard->m_certify && (m_guard->m_term & flagCAX))
	{
		delete m_guard->m_certify;
		m_guard->m_certify = NULL;
	}
	if (m_guard->m_xecure && (m_guard->m_term & flagENX))
	{
		delete m_guard->m_xecure;
		m_guard->m_xecure = NULL;
	}
}

void CWizardCtrl::OnCertify(char* pBytes, int nBytes)
{
	CString	text;

	switch (LOWORD(nBytes))
	{
	case invokeCA:
		m_guard->Write(msgK_XCA, "AXISENCA", pBytes, HIWORD(nBytes));
		break;
	case encryptCA:
		text = pBytes;
		if (!text.IsEmpty())
		{
			int	idx = text.Find('\t');
			if (idx == -1)
				idx = text.GetLength();
			text = m_guard->Secure(text.Left(idx), (idx < text.GetLength()) ? text.Mid(idx+1) : _T(""), HIWORD(nBytes));
			strcpy(pBytes, text);
		}
		break;
	case guideCA:
		if (pBytes)
			m_guard->SetGuide(pBytes);
		else
			m_guard->SetGuide(HIWORD(nBytes));
		break;
	case closeCA:
		PostMessage(WM_USER, MAKEWPARAM(closeAXIS, true));
		break;
	case htmlCA:
		m_guard->SendAxis(MAKEWPARAM(htmlVIEW, 0), (LPARAM)pBytes);
		break;
	}
}

void CWizardCtrl::Xecure()
{
	if (m_guard->m_term & flagENC && m_xtype != xtXEC)
	{
		m_mode = mtXEC;
		OnFire(FEV_GUIDE, 0, AE_SECURE);
		if (Xecure(NULL, 0) == -1)
		{
			m_mode = mtCON;
		AfxMessageBox("��ȣȭ ���� ����");
			OnFire(FEV_GUIDE, 0, AE_ESECURE);
			OnFire(FEV_AXIS, MAKEWPARAM(loginERR, secureERR), 0);
		}
		return;
	}
	Run();
}

void CWizardCtrl::OnXecure(int encK, char *pBytes, int nBytes)
{
	switch (encK)
	{
	case encERR:
		OnFire(FEV_AXIS, MAKEWPARAM(loginERR, m_mode == mtCA ? certifyERR : secureERR), 0);
		if (m_mode == mtCA && m_guard->CertifyErr(pBytes, nBytes))
			break;
		OnFire(FEV_ERROR, true, (long)CString(pBytes, nBytes).operator LPCTSTR());
		break;
	case encENC:
		if (m_mode == mtCA || m_mode == mtRUN)
		{
			switch (m_guard->OnCertify(pBytes, nBytes))
			{
			case 0:
				m_guard->m_status |= WS_SELF;
				Xecure();
				break;
			case -1:
				OnFire(FEV_GUIDE, 0, AE_ECERTIFY);
				OnFire(FEV_AXIS, MAKEWPARAM(loginERR, certifyERR), 0);
				break;
			case -3:	// terminate AXIS...reboot
				OnFire(FEV_AXIS, MAKEWPARAM(loginERR, certifyERR), 0);
				PostMessage(WM_USER, MAKEWPARAM(closeAXIS, true));
				break;
			default:
				break;
			}
			break;
		}

		if (Xecure(pBytes, nBytes) == 0)
		{
			if (m_xtype == xtXEC)
			{
				m_mode = mtCON;
				OnFire(FEV_RUN, 0, 0);
			}
			else
				Run();
		}
		break;
	case encOK:
		if (m_xtype == xtXEC)
		{
		//	if (m_guard->m_xecure)
		//	{
		//		delete m_guard->m_xecure;
		//		m_guard->m_xecure = NULL;
		//	}

			m_mode  = mtCON;
			m_xtype = xtWS;
			OnFire(FEV_RUN, 0, 0);
		}
		break;
	}
}

void CWizardCtrl::OnAxis(struct _axisH* axisH, char *pBytes, int nBytes)
{
	char	mapN[L_MAPN+1];
	CopyMemory(mapN, axisH->trxC, L_MAPN);
	mapN[L_MAPN] = '\0';

	CWorks* works;
	if (!m_guard->GetClient(axisH->winK, works))
	{
		if (axisH->stat & statNEW)
		{
			if (!axisH->winK)
				axisH->winK = m_guard->GetWorks();
			m_guard->SendAxis(MAKEWPARAM(renVIEW, axisH->winK), (LPARAM)mapN);
		}

		if (!m_guard->GetClient(axisH->winK, works))
		{
			m_guard->SetGuide((axisH->stat & statNEW) ? AE_EWINDOW : AE_XWINDOW);
			return;
		}
	}
	
	if (axisH->auxs & auxsCLOSE)
	{
		m_guard->PostAxis(MAKEWPARAM(delVIEW, axisH->winK), (LPARAM)0);
		return;
	}

	if (axisH->unit == unitMAIN && axisH->auxs & auxsMAP &&
			memcmp(mapN, works->m_mapN, L_MAPN) && mapN[0] > ' ')
	{
		if (!works->Attach(mapN, true))
			return;
	}

	m_guard->xTRACE(x_RCVs, (char *)axisH, L_axisH+nBytes);
//	m_guard->xTRACE(x_RCVs, pBytes, nBytes);
	works->OnStream(axisH, pBytes, nBytes);
}

int CWizardCtrl::Xecure(char* pBytes, int nBytes)
{
	if (m_guard->m_xecure == NULL)
	{
		m_guard->m_xecure = new CWnd();
		if (!m_guard->m_xecure->CreateControl("AxisXecure.XecureCtrl.IBK", NULL, 0, CRect(0, 0, 0, 0), this, 0))
		{
			delete m_guard->m_xecure;
			m_guard->m_xecure = NULL;
		AfxMessageBox("��ȣȭ ��Ʈ�� ���� ����");
			OnFire(FEV_GUIDE, 0, AE_EXECCTRL);
			return -2;
		}

	}

	long	retv;

	m_guard->m_xecure->InvokeHelper(DI_XEC, DISPATCH_METHOD, VT_I4, (void*)&retv,
						(BYTE *) VTS_I4 VTS_I4, pBytes, &nBytes);

	switch (nBytes)
	{
	case 0:
		return 0;
	case -1:
		if (retv != NULL)
		{
			OnFire(FEV_ERROR, true, retv);
			return -2;
		}
		break;
	default:
		if (m_guard->Write(msgK_ENC, "AXISENCX", (char *)retv, nBytes))
			return nBytes;
		break;
	}
//	OnFire(FEV_GUIDE, 0, AE_ESECURE);
	return -1;
}

void CWizardCtrl::Run()
{
	m_mode = mtRUN;
	m_guard->Startup();
	OnFireAxis(runAXIS, 0);
	m_guard->LoadHistory();
}

bool CWizardCtrl::OnAux(struct _axisH* axisH, char* &pBytes, int& nBytes)
{
	m_guard->xTRACE(x_RCVs, (char *)axisH, L_axisH+nBytes);

	if (axisH->stat & statAUX)
	{
		struct _auxH*	auxH;

		auxH = (struct _auxH *)pBytes;
		nBytes -= L_auxH;
		if (nBytes < 0)
			return false;
		pBytes += L_auxH;
		nBytes -= auxH->datL;
		if (nBytes < 0)
			return false;

		pBytes += auxH->datL;
	}
	return true;
}

void CWizardCtrl::OnStream(struct _axisH* axisH, char *pBytes, int nBytes)
{
	if (m_axisL > 0)
	{
		char*	axisB = m_axisB;
		m_axisB = new char[m_axisL+nBytes+1];
		CopyMemory(m_axisB, axisB, m_axisL);
		delete [] axisB;
	}
	else
	{
		m_axisL = L_axisH;
		m_axisB = new char[L_axisH+nBytes+1];
		CopyMemory(m_axisB, axisH, L_axisH);
	}
	CopyMemory(&m_axisB[m_axisL], pBytes, nBytes);
	m_axisL += nBytes;

	if (!(axisH->stat & statCON))
	{
		axisH = (struct _axisH *)m_axisB;
		pBytes = &m_axisB[L_axisH];
		nBytes = m_axisL-L_axisH;
		switch (axisH->msgK)
		{
		case msgK_SIGN:
		case msgK_SIGNx:
			OnSign(axisH->msgK, pBytes, nBytes);
			break;
		default:
			break;
		}
		m_axisL = 0;
		delete [] m_axisB;
	}
}
