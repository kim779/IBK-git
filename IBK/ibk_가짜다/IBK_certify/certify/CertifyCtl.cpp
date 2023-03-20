// CertifyCtl.cpp : Implementation of the CCertifyCtrl ActiveX Control class.

#include "stdafx.h"
#include "Certify.h"
#include "CertifyCtl.h"
#include "CertifyPpg.h"
#include "guide.h"
#include "issue.h"
#include "invalid.h"
#include "overpass.h"
#include "countpass.h"
//#include "passInput.h"
#include "ExitPass.h"

#include "../h/axis.h"
#include "../h/axiserr.h"
#include "../h/axisfire.h"
#include "../h/axisvar.h"


#pragma	comment(lib, "CaLib/SKComdIF")
#pragma	message("Automatically linking with SKComdIF library")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TM_MSG	10000


IMPLEMENT_DYNCREATE(CCertifyCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCertifyCtrl, COleControl)
	//{{AFX_MSG_MAP(CCertifyCtrl)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CCertifyCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CCertifyCtrl)
	DISP_FUNCTION(CCertifyCtrl, "OnCertify", OnCertify, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CCertifyCtrl, "Certify", Certify, VT_BOOL, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CCertifyCtrl, "CertifyErr", CertifyErr, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(CCertifyCtrl, "CertifyId", CertifyId, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CCertifyCtrl, "CertifyEx", CertifyEx, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(CCertifyCtrl, "CertifyFull", CertifyFull, VT_I4, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CCertifyCtrl, "CertifyName", CertifyName, VT_I4, VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CCertifyCtrl, COleControl)
	//{{AFX_EVENT_MAP(CCertifyCtrl)
	EVENT_CUSTOM("OnFire", OnFire, VTS_I4  VTS_I4  VTS_I4)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CCertifyCtrl, 1)
	PROPPAGEID(CCertifyPropPage::guid)
END_PROPPAGEIDS(CCertifyCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCertifyCtrl, "AxisCertify.CertifyCtrl.IBK",
	0x5d693fff, 0x52bf, 0x4dda, 0x85, 0xa6, 0x14, 0x29, 0x88, 0x7d, 0x20, 0xab)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CCertifyCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DCertify =
		{ 0x240ce94c, 0x9426, 0x4c02, { 0xba, 0x78, 0xcb, 0xb4, 0x38, 0x5b, 0xde, 0x8e } };
const IID BASED_CODE IID_DCertifyEvents =
		{ 0xf976a329, 0x37a, 0x407a, { 0xa0, 0x73, 0x38, 0xe2, 0xb5, 0xb, 0x61, 0x4d } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwCertifyOleMisc =
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CCertifyCtrl, IDS_CERTIFY, _dwCertifyOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CCertifyCtrl::CCertifyCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CCertifyCtrl

BOOL CCertifyCtrl::CCertifyCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_CERTIFY,
			IDB_CERTIFY,
			afxRegApartmentThreading,
			_dwCertifyOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CCertifyCtrl::CCertifyCtrl - Constructor

CCertifyCtrl::CCertifyCtrl()
{
	InitializeIIDs(&IID_DCertify, &IID_DCertifyEvents);

	m_ca   = caNO;
	m_user = _T("");
	m_name = _T("cn=");
	ZeroMemory(m_encpass, sizeof(m_encpass));

	m_auto     = false;
	m_calogon  = false;
//	m_passDlg  = NULL;
	m_certifys = _T("AXIS Certify !!!");
	m_msg = _T("");

	m_emaps.RemoveAll();
}


/////////////////////////////////////////////////////////////////////////////
// CCertifyCtrl::~CCertifyCtrl - Destructor

CCertifyCtrl::~CCertifyCtrl()
{
	ZeroMemory(m_encpass, sizeof(m_encpass));
	m_user = _T("");
	m_name = _T("");
//	if (m_passDlg)
//		delete m_passDlg;

	m_emaps.RemoveAll();
}


/////////////////////////////////////////////////////////////////////////////
// CCertifyCtrl::OnDraw - Drawing function

void CCertifyCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
}


/////////////////////////////////////////////////////////////////////////////
// CCertifyCtrl::DoPropExchange - Persistence support

void CCertifyCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
}


/////////////////////////////////////////////////////////////////////////////
// CCertifyCtrl::OnResetState - Reset control to default state

void CCertifyCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
}


/////////////////////////////////////////////////////////////////////////////
// CCertifyCtrl message handlers

long CCertifyCtrl::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case checkPass:
		if (HIWORD(wParam))	// remove certificate
			removeCertificate();
		else
			return checkPasswd((char *)lParam);
	case rebootAxis:
		OnFire(FEV_CA, MAKELONG(closeCA, HIWORD(wParam)), 0);
		break;
	case encryptPass:
		OnFire(FEV_CA, MAKELONG(encryptCA, HIWORD(wParam)), lParam);
		return (long)lParam;
	}
	return 0;
} 

long CCertifyCtrl::OnCertify(long pBytes, long nBytes) 
{
	if (pBytes == NULL)
	{
		m_ca   = caNO;
		m_name = _T("cn=");
		return NULL;
	}

	CString	refs, auths, msg = _T("");
	int	idx, retv, caL = *(int *)nBytes;
	int	ecode = -1, result = 0;
	bool	reissue, query = true;
	bool	retry = false;
	CString	text;
	struct	_caH*	caH;

	retv = -1;
	*(int *)nBytes = retv;
	switch (m_ca)
	{
	case caPWDa:
		retv--;
	//	if (m_passDlg)
	//		m_passDlg->SetMessage(CString((char *)pBytes, caL));
		m_msg = CString((char *)pBytes, caL);
		SetTimer(TM_MSG, 200, NULL);
		m_ca = caRUN;
		*(int *)nBytes = retv;
		return NULL;

	case caNO:
		ZeroMemory(&m_context, sizeof(APP_CONTEXT));
		ZeroMemory(&m_contextNew, sizeof(SD_API_CONTEXT_NEW));
		if (caL <= 0)
			return NULL;
		if (sk_if_cert_InitContextApp(&m_context, NULL, 0) == -1)
		{
			MessageBox("�������� ���� ������ �߻��� �� �ֻ���� ������(1588-0030, 1544-0050)�� ���ǹٶ��ϴ�.",
				"�������� �ʱ�ȭ ����", MB_OKCANCEL|MB_SYSTEMMODAL|MB_ICONINFORMATION);
			return NULL;
		}

		caH = (struct _caH *)pBytes;
		m_name = CString(caH->dns, sizeof(caH->dns));
		// �ʼ� ȭ��
		text   = CString(caH->map, sizeof(caH->map));
		idx    = text.Find('\0');
		if (idx != -1)
			text = text.Left(idx);
		for ( ; !text.IsEmpty(); )
		{
			m_emaps.SetAt(text.Left(L_MAPN), NULL);
			if (text.GetLength() > L_MAPN)
				text = text.Mid(L_MAPN);
			else
				text.Empty();
		}
		//
		text   = CString(caH->ecode, sizeof(caH->ecode));
		if (text.GetAt(0) == 'S')
			ecode = atoi(text.Mid(1));
		else if (text.GetAt(0) == 'E' || atoi(text))
			msg = m_name;

		idx = m_name.Find('\0');
		if (idx != -1)
			m_name = m_name.Left(idx);
		m_name.TrimRight();

		reissue = false;
		if (ecode >= 0 || !msg.IsEmpty())
		{
			idx = 0;
			switch (ecode)
			{
			case 1:	// ����
				{
					CGuide guide(typeREVOKE);
					idx = guide.DoModal();
					reissue = true;
				}
				break;
			case 2:	// ����
				{
					CGuide guide(typeSUSPEND);
					idx = guide.DoModal();
					reissue = true;
				}
				break;
			case 3:	// ����
				{
					CGuide guide(typeEXPIRE);
					idx = guide.DoModal();
					reissue = true;
				}
				break;
			case 4:	// �̵��
				{
					CIssue issue;
					idx = issue.DoModal();
				}
				break;
			case 5:	// �űԹ߱�
				{
					CInvalid invalid;
					idx = invalid.DoModal();
				}
				break;
			case 20:// ��й�ȣ 5ȸ����
				{
					COverPass passover;
					idx = passover.DoModal();
					reissue = true;
				}
				break;
			case 9:	// ���ºҺи�
				msg = m_name;
			default:
				if (!guideMsg((msgNO)ecode, msg))
					return NULL;
				break;
			}

			if (idx > 0)
			{
				switch (idx)
				{
				case IDC_ISSUE:
					certify(reissue);
					*(int *)nBytes = -2;
					return NULL;

				case IDC_OTHER:
					otherCertificate();
					*(int *)nBytes = -2;
				default:
					return NULL;
				}
			}
			query = false;
		}

	case caPWD:
		if (m_ca == caPWD)
		{
			retry = true;
			struct	_pwdR* pwdR = (struct _pwdR *)pBytes;
			switch (pwdR->ret[0])
			{
			case 'X':
				retry = false;
			case '0':
				break;
			default:
				msg = CString(pwdR->msg, sizeof(pwdR->msg));
				msg.TrimLeft(); msg.TrimRight();
				guideMsg((msgNO)-1, msg);
				return NULL;
			}
			idx = atoi(CString(pwdR->pwdn, sizeof(pwdR->pwdn)));

			CCountPass countDlg(idx, retry);
			switch (countDlg.DoModal())
			{
			case IDOK:
				break;
			case IDC_ISSUE:
				certify(true);
				*(int *)nBytes = -2;
				return NULL;

			case IDC_OTHER:
				otherCertificate();
				*(int *)nBytes = -2;
			default:
				return NULL;
			}
		}

		if (query)
		{
			for ( ; true; )
			{
				result = queryDn(m_name, (int *)nBytes, retry);
				switch (result)
				{
				case rspNULL:
					return NULL;

				case rspPASSWD:
					m_ca     = caPWD;
					m_string = _T("pswd\t");
					*(int *)nBytes = m_string.GetLength();
					strcpy(m_pBytes, (char *)m_string.operator LPCTSTR());
					return (long)m_pBytes;

				case rspOTHER:
					otherCertificate();
					*(int *)nBytes = -2;
					return NULL;

				case rspISSUE:
				case rspREISSUE:
					certify(result == rspREISSUE);
					*(int *)nBytes = -2;
					return NULL;

				case rspOK:
					break;
				}
				break;
			}
		}

	case caNOx:
		if (!sign())
		{
			sk_if_cert_static_context_release();
			return NULL;
		}
		m_ca = caOK;
		break;

	case caOK:
		m_ca = caRUN;
		*(int *)nBytes = 0;

		savePasswd();
		return NULL;

	case caOKx:
		if (caL <= 0)
			return NULL;

		caH = (struct _caH *)pBytes;
		m_name = CString(caH->dns, sizeof(caH->dns));
		// �ʼ� ȭ��
		text = CString(caH->map, sizeof(caH->map));
		idx  = text.Find('\0');
		if (idx != -1)
			text = text.Left(idx);
		for ( ; !text.IsEmpty(); )
		{
			m_emaps.SetAt(text.Left(L_MAPN), NULL);
			if (text.GetLength() > L_MAPN)
				text = text.Mid(L_MAPN);
			else
				text.Empty();
		}
		//
		idx = m_name.Find('\0');
		if (idx != -1)
			m_name = m_name.Left(idx);
		m_name.TrimRight();

		m_ca = caRUN;
		*(int *)nBytes = 0;
		return NULL;

	default:
		return NULL;
	}

	*(int *)nBytes = m_nBytes;
	return (long)m_pBytes;
}

BOOL CCertifyCtrl::Certify(long pBytes, long nBytes, long infos) 
{
	switch (m_ca)
	{
	case caNO:
		return TRUE;
	case caRUN:
		break;
	default:
		OnFire(FEV_CA, MAKELONG(guideCA, AE_ECERTIFY), 0);
		return FALSE;
	}

	char	encpass[32+1];
	UString	src, des;

	ZeroMemory(encpass, sizeof(encpass));
	CString maps = CString((char *)infos, L_MAPN);
	if (m_auto && !isMustCertify(maps))
		CopyMemory(encpass, m_encpass, sizeof(encpass));
	else
	{
		/*
		if (m_passDlg)
			delete m_passDlg;

		m_passDlg = new CPassInput(this);
		if (m_passDlg->DoModal() != IDOK)
		{
			delete m_passDlg;
			m_passDlg = NULL;
			m_ca = caRUN;
			return FALSE;
		}
		sk_if_GetEncryptedPassword((char *)m_passDlg->m_pass.operator LPCTSTR(), m_encpass);
		CopyMemory(encpass, m_encpass, sizeof(encpass));
		delete m_passDlg;
		m_passDlg = NULL;
		*/

		CString	pass;
		pass = checkPasswd();
		if (pass.IsEmpty())
		{
			if (m_ca != caPWDa)
				m_ca = caRUN;
			return FALSE;
		}
		sk_if_GetEncryptedPassword((char *)pass.operator LPCTSTR(), m_encpass);
		CopyMemory(encpass, m_encpass, sizeof(encpass));
		pass = _T("");
	}

	m_sync.Lock();
	m_nBytes   = *(int *)nBytes;
	src.length = m_nBytes;
	src.value  = (unsigned char *)pBytes;
	if (sk_if_cert_SignData_notEncode(&m_context, encpass, &src, &des, NULL))
	{
		m_sync.Unlock();
		ZeroMemory(encpass, sizeof(encpass));
		OnFire(FEV_CA, MAKELONG(guideCA, AE_ECERTIFY), 0);
		return FALSE;
	}

	pBytes   += m_nBytes;
	m_nBytes += des.length;

	*(int *)nBytes = m_nBytes;
	CopyMemory((void *)pBytes, des.value, des.length);
	sk_if_cert_MemFree(des.value);
	ZeroMemory(encpass, sizeof(encpass));
	m_sync.Unlock();
	return TRUE;
}

BOOL CCertifyCtrl::CertifyErr(long pBytes, long nBytes) 
{
	enum	{ eNONE = 0, eHTS = 1, eSIGN = 2 } eKind;

	eKind = eNONE;
	if (nBytes <= 0)
		return (int)eKind;
	
	m_ca = caNO;
	switch (((char *)pBytes)[0])
	{
	case 'h': case 'H':
		eKind = eHTS; break;
	case 's': case 'S':
		eKind = eSIGN; break;
	default:
		return (int)eKind;
	}

	int	idx;
	CString	eCode;
	CString	string = CString((char *)(pBytes+1), nBytes-1);

	eCode  = string.Left(4);
	string = string.Mid(4);

	switch (atoi(eCode))
	{
	case 2001:
		if (eKind == eSIGN) guideMsg(msg1);		break;
	case 2008:
	case 4444:
	case 1:		// ����
		if (eKind == eSIGN)
		{
			CGuide guide(typeREVOKE);
			idx = guide.DoModal();
			switch (idx)
			{
			case IDC_ISSUE:
				certify(true);
				break;
			case IDC_OTHER:
				otherCertificate();
			default:
				removeCertificate();
				return FALSE;
			}
		}
		break;

	case 2424:
		if (eKind == eSIGN) guideMsg(msg2);		break;
	case 1405:
		if (eKind == eHTS)  guideMsg(msg4);		break;
	case 8888:
		if (eKind == eHTS)  guideMsg(msg0, string);	break;
	case 8889:
		if (eKind == eHTS)  guideMsg(msg8);		break;
	case 8899:
		if (eKind == eHTS)  guideMsg(msg5);		break;
	case 8989:
		if (false && eKind == eHTS)
			removeCertificate();
		break;
	case 3532:	// ��й�ȣ over
		if (eKind == eSIGN)
		{
			COverPass passover;
			idx = passover.DoModal();

			switch (idx)
			{
			case IDC_ISSUE:
				certify(true);
				break;
			case IDC_OTHER:
				otherCertificate();
			default:
				removeCertificate();
				return FALSE;
			}
		}
		break;
	case 9999:	// SignKorea ���ӿ���
	case 9998:	// �Է°� ����ġ
		if (eKind == eSIGN)  guideMsg(msg0, string);	break;
	default:
		switch (eKind)
		{
		case eSIGN:
			guideMsg(msg3);	break;
		case eHTS:
			guideMsg(msg0, string);
		default:
			break;
		}
		break;
	}

	if (eKind == eSIGN)
	{
		switch (atoi(eCode))
		{
		case 2001: case 2008:
		case 1: case 3532:
			removeCertificate();
		default:
			break;
		}
	}
	sk_if_cert_static_context_release();

	TRACE("[%c][%d][%s]\n", eKind == eHTS ? 'H' : 'S', atoi(eCode), string);
	return FALSE;
}

// id(12) + pass(10) + info(10) + cpass(30) + ip(15) + mac(16)
// info[1] : �ֹ��ڵ�����, info[2] : �����������
void CCertifyCtrl::CertifyId(long pBytes) 
{
	int	idx;
	CString	info, pass;
//	char* nAuto = _T("�ֹ��øŹ���й�ȣ�Է��Ѵ�");
//	char* yAuto = _T("�ֹ��øŹ���й�ȣ�Է������ʴ´�");

	m_user = CString((char *)pBytes, 12);
	info   = CString((char *)(pBytes+22), 10);
	pass   = CString((char *)(pBytes+32), 30);
	m_user.TrimRight();
	idx = pass.Find('\0');
	if (idx != -1)
		pass = pass.Left(idx);
//	pass.TrimRight();	// �������� ��й�ȣ �������� ' '(space)�� �ִ� ��찡 �־� TrimRight ����
	FillMemory((char *)(pBytes+32), 30, ' ');

	if (!m_calogon)
	{
		sk_if_SetPasswordEncMode(1);
		sk_if_GetEncryptedPassword((char *)pass.operator LPCTSTR(), m_encpass);
		pass = _T("");
	}
	m_calogon = false;

	m_auto = (info.GetAt(1) == '1') ? true : false;
	m_certifys.Format("%s", m_user);
//	m_certifys.Format("%s(%s)=%s", m_user, "�����", m_auto ? yAuto : nAuto);
}

BOOL CCertifyCtrl::CertifyEx(long pBytes, long nBytes) 
{
	if (pBytes == NULL)
	{
		if (MessageBox("�������� ������ ������ �� �����ϴ�.\r\n���������� ������� �ʰ� ������ ����Ͻðڽ��ϱ�?",
				"�������� �̻�� ���� Ȯ��", MB_OKCANCEL|MB_SYSTEMMODAL|MB_ICONINFORMATION) == IDOK)
			return TRUE;

		OnFire(FEV_CA, MAKELONG(closeCA, true), 0);	// terminate AXIS
		return FALSE;
	}

	return TRUE;
}

bool CCertifyCtrl::sign()
{
	UString src, des;

	src.length = m_certifys.GetLength();
	src.value = (unsigned char *)m_certifys.operator LPCTSTR();
	if (sk_if_cert_SignData(&m_context, NULL, &src, &des))
		return false;

	CString	keys = _T("key\t");
	m_nBytes = keys.GetLength();
	CopyMemory(m_pBytes, keys, m_nBytes);

	CopyMemory(&m_pBytes[m_nBytes], des.value, des.length);
	m_nBytes += des.length;

	sk_if_cert_MemFree(des.value);
	return true;
}

int CCertifyCtrl::queryDn(CString dn_name, int* nBytes, bool retry)
{
	CWnd*	pWnd;
	BOOL	success = FALSE;

	pWnd = AfxGetMainWnd();
	sk_if_SetWrongPasswordLimit(1);
	m_contextNew.sd.bOldStorage = 0x00;
	m_contextNew.sd.bForceAllCaSearchMode = 1;
	strcpy(m_contextNew.sd.szUserId, (char *)dn_name.operator LPCTSTR());
	ZeroMemory(m_contextNew.sd.szOldPasswd, sizeof(m_contextNew.sd.szOldPasswd));
	sk_if_SetKeySaferMode(1);
	if (retry || m_encpass[0] == NULL)
	{
		if (pWnd && IsWindow(pWnd->GetSafeHwnd()))
			sk_if_DialogModalMode(pWnd->GetSafeHwnd());
		success = sk_if_CertSetSelectExt(&m_contextNew, CONTEXT_SELECT2, SEARCH_ALLMEDIA);
	}
	else
	{
		CopyMemory(m_contextNew.sd.szOldPasswd, m_encpass, min(sizeof(m_encpass), sizeof(m_contextNew.sd.szOldPasswd)));
		success = sk_if_CertSetSelect(&m_contextNew.sd);
	}

	if (!success)
	{
		switch (sk_if_GetLastErrorCode())
		{
		case 2001:		// ����
			sk_if_cert_static_context_release();
			{
				CGuide guide(typeEXPIRE);
				switch (guide.DoModal())
				{
				case IDC_OTHER:
					return rspOTHER;
				case IDC_ISSUE:
					return rspREISSUE;
				default:
					break;
				}
				return rspNULL;
			}

		case 2501:		// ���
			guideMsg(msg7);
			sk_if_cert_static_context_release();
			return rspNULL;

		case 2500:		// ������ ����
			{
				CInvalid invalid;
				switch (invalid.DoModal())
				{
				case IDC_OTHER:
					return rspOTHER;
				case IDC_ISSUE:
					return rspISSUE;
				case IDOK:
					return queryDn(dn_name, nBytes);
				default:
					break;
				}
				return rspNULL;
			}
		case 1001:		// ������ ������� (5ȸ)
		case 2417:
			return rspPASSWD;

		case 2508:		// ������ ���� ����
			guideMsg(msg19);
			sk_if_cert_static_context_release();
			return rspNULL;

		default:
			guideMsg(msg12);
			break;
		}
	}

	if (sk_if_cert_preset_context(&m_context, &m_contextNew.sd) < 0)
	{
		MessageBox(sk_if_GetLastErrorMsg(), "��������-Preset ����", MB_OK|MB_SYSTEMMODAL|MB_ICONINFORMATION);
		return rspNULL;
	}
	return rspOK;
}

void CCertifyCtrl::savePasswd()
{
	if (m_context.pInterfaceContext == NULL)
		return;

	CString	pass;

	pass = CString(m_context.pInterfaceContext->szOldPasswd, sizeof(m_context.pInterfaceContext->szOldPasswd));
	sk_if_GetEncryptedPassword((char *)pass.operator LPCTSTR(), m_encpass);
	pass = _T("");
}

BOOL CCertifyCtrl::checkPasswd(CString pass)
{
	CString	text;
	UString	src, des;
	char	encpass[32+1];

	m_sync.Lock();
	ZeroMemory(encpass, sizeof(encpass));
	sk_if_GetEncryptedPassword((char *)pass.operator LPCTSTR(), encpass);

	src.length = 0;
	src.value  = NULL;
	if (sk_if_cert_SignData_notEncode(&m_context, encpass, &src, &des, NULL))
	{
		if (sk_if_GetLastErrorCode() == 2417)
		{
			m_ca     = caPWDa;
			m_string = _T("pswd\t");
			OnFire(FEV_CA, MAKELONG(invokeCA, m_string.GetLength()), (long)(char *)m_string.operator LPCTSTR());
		}
		ZeroMemory(encpass, sizeof(encpass));
		m_sync.Unlock();
		return FALSE;
	}

	sk_if_cert_MemFree(des.value);
	ZeroMemory(encpass, sizeof(encpass));
	m_sync.Unlock();
	return TRUE;
}

CString CCertifyCtrl::checkPasswd()
{
	CString	text, pass = _T("");
	UString	src, des;
	char	encpass[32+1];
	int	pswdL;

	m_sync.Lock();
	ZeroMemory(encpass, sizeof(encpass));
	pswdL = sizeof(m_context.pInterfaceContext->szOldPasswd);
	ZeroMemory(m_context.pInterfaceContext->szOldPasswd, pswdL);

	src.length = 0;
	src.value  = NULL;
	sk_if_SetKeySaferMode(1);
	if (sk_if_cert_SignData_notEncode(&m_context, encpass, &src, &des, NULL))
	{
		if (sk_if_GetLastErrorCode() == 2417)
		{
			m_ca     = caPWDa;
			m_string = _T("pswd\t");
			OnFire(FEV_CA, MAKELONG(invokeCA, m_string.GetLength()), (long)(char *)m_string.operator LPCTSTR());
		}
		ZeroMemory(encpass, sizeof(encpass));
		m_sync.Unlock();
		return pass;
	}
	sk_if_cert_MemFree(des.value);

	ZeroMemory(encpass, sizeof(encpass));
	pass = CString(m_context.pInterfaceContext->szOldPasswd, pswdL);
	m_sync.Unlock();
	return pass;
}

bool CCertifyCtrl::certify(bool reissue)
{
	m_string = _T("http://www.ibks.com/LoadService.jsp?url=/customer/certificate/newissue.jsp");
	OnFire(FEV_CA, MAKELONG(htmlCA, 0), (long)(char *)m_string.operator LPCTSTR());
	return true;
}

void CCertifyCtrl::otherCertificate()
{
	m_string = _T("http://www.ibks.com/LoadService.jsp?url=/customer/certificate/etc_entry.jsp");
	OnFire(FEV_CA, MAKELONG(htmlCA, 0), (long)(char *)m_string.operator LPCTSTR());
}

void CCertifyCtrl::removeCertificate()
{
//	if (strlen(m_contextNew.sd.szUserId) > 0)
//		sk_if_RemoveFromMediaExt(&m_contextNew);
}

bool CCertifyCtrl::guideMsg(msgNO msgno, CString guide, CString title)
{
	struct	_msg {
		char	no;
		bool	confirm;
		char*	title;
		char*	msg;
	};
	
	struct	_msg	msg[] = {
		{  0, false, NULL,		NULL },
		{  1, false, "",		"����Ͻ� �������� ����� �������Դϴ�." },
		{  2, false, "",		"����Ͻ� �������� IBK�������ǿ��� ����Ͻ� �� ���� �������Դϴ�.\r\n�űԹ߱� : �ٽ� �α��� �� ������ ����â���� ��Ҹ� �����Ͽ� �ֽʽÿ�." },
		{  3, false, "",		"���ڼ����� �����Ͽ����ϴ�. �����Ϳ� �����Ͽ� �ֽʽÿ�.\r\n(������ 1588-0030, 1544-0050)" },
		{  4, false, "",		"����Ͻ� �������� IBK�������ǿ� ��ϵ��� ���� �������Դϴ�.\r\n�űԹ߱� : �ٽ� �α��� �� ������ ����â���� ��Ҹ� �����Ͽ� �ֽʽÿ�." },
		{  5, false, "",		"�������� ��߱��ϱ� ���Ͽ� ��� ������(1588-0030, 1544-0050)�� ���Ͽ�\r\n���ο��θ� Ȯ���Ͻ� �� �������� ��߱� �����ñ� �ٶ��ϴ�." },
		{  6, false, "",		"������ �߱��� ����߽��ϴ�." },
		{  7, false, "������ ���� ���","������ ������ ����Ͽ����ϴ�" },
		{  8, false, "",		"�Է��Ͻ� ���ڼ��� ��й�ȣ�� ���� �ʽ��ϴ�. ���ڼ��� ��й�ȣ�� 5ȸ������ ���Ͽ�\r\n�� �̻� ���ڼ����� ������ �� �����ϴ�.\r\n�����͸� ���� ���ο��θ� Ȯ���Ͻ� �� �������� ��߱� �����ñ� �ٶ��ϴ�.\r\n(������ 1588-0030, 1544-0050)" },
		{  9, false, "",		"���ڼ��� ��й�ȣ�� Ʋ���ϴ�" },
		{ 10, false, "",		"������ �������� ���������� �߱�(��߱�) �Ǿ����ϵ�.\r\n�ٽ� �α����Ͻñ� �ٶ��ϴ�.\r\n��Ÿ �ñ��Ͻ� ������ �����ͷ� �����Ͻñ� �ٶ��ϴ�.\r\n(������ 1588-0030, 1544-0050)" },
		{ 11, true,  "",		"�������� ���������� �߱޵Ǿ����ϴ�.\r\n�������� ����� ������ �����ϰ� �����ϱ� ���� ��������� �����մϴ�.\r\n����� �Ϸ���� �÷��ǵ���(�Ǵ� �̵���ü)�� �������� ��ũ�����˻�⸦ �̿��Ͽ�\r\n�������� Ȯ���Ͻ� ��, �����ϰ� �����Ͻñ� �ٶ��ϴ�.\r\n\r\n������ ����� ���� ����� �÷��ǵ���(�Ǵ� �̵���ü)�� �غ��Ͽ� �ֽʽÿ�."},
		{ 12, false, "��Ÿ����",	"�� �� ���� ������ �߻��߽��ϴ�.\r\n������(1588-0030, 1544-0050)�� �����Ͻñ� �ٶ��ϴ�." },
		{ 13, false, "",		"����� ���� ���� ��濡 �����Ͽ����ϴ�. �����ͷ� ���� �ٶ��ϴ�.\r\n(������ 1588-0030, 1544-0050)" },
		{ 14, false, "",		"����� ���� ������ �����Ͽ����ϴ�. �����ͷ� ���� �ٶ��ϴ�.\r\n(������ 1588-0030, 1544-0050)" },
		{ 15, false, "",		"�ÿ��� �ΰ����� �Է��� ����߽��ϴ�." },
		{ 16, false, "",		"Ÿ��� ������ ����� ����߽��ϴ�." },
		{ 17, false, "",		"���ڼ��� �����Ͽ����ϴ�." },
		{ 18, false, "",		"Ÿ��� ������ ����� ���� ó�� �Ǿ����ϴ�. ���α׷��� ������մϴ�." },
		{ 19, false, "",		"������ ������ �ٽ� �̿�ٶ��ϴ�." },
	};

	UINT	type = MB_OK|MB_ICONINFORMATION|MB_SYSTEMMODAL;

	if (guide.IsEmpty())
	{
		if ((int)msgno < 0 || (int)msgno >= sizeof(msg) || msg[msgno].msg == NULL)
			return false;
		guide = msg[msgno].msg;
		if (msg[msgno].confirm)
			type = MB_OKCANCEL|MB_ICONINFORMATION|MB_SYSTEMMODAL;
		title = msg[msgno].title;
	}
	if (title.IsEmpty())
		title = _T("IBK��������");

	if (MessageBox(guide, title, type) == IDCANCEL)
		return false;

	return true;
}

BOOL CCertifyCtrl::isMustCertify(CString maps)
{
	void*	ptr;

	return m_emaps.Lookup(maps, ptr);
}

void CCertifyCtrl::OnTimer(UINT nIDEvent) 
{	
	COleControl::OnTimer(nIDEvent);
	KillTimer(nIDEvent);

	if (nIDEvent != TM_MSG)
		return;

	struct	_pwdR* pwdR;
	int	value;
	CString	string;

	pwdR  = (struct _pwdR *)(char *)m_msg.operator LPCTSTR();
	switch (pwdR->ret[0])
	{
	case 'X':
		{
			CExitPass exitpass;
			exitpass.DoModal();
			SendMessage(WM_USER, MAKEWPARAM(checkPass, 1));
			SendMessage(WM_USER, MAKEWPARAM(rebootAxis, true));
		}
		return;
	case '0':
		value = atoi(CString(pwdR->pwdn, sizeof(pwdR->pwdn)));
		string.Format("���ڼ��� ��й�ȣ %dȸ �����Դϴ�", value);
		break;
	default:
		string = CString(pwdR->msg, sizeof(pwdR->msg));
		string.TrimLeft();
		string.TrimRight();
		break;
	}
	MessageBox(string, "IBK��������", MB_ICONINFORMATION|MB_OK);
}

// updateXX_20160503
long CCertifyCtrl::CertifyFull(long pInB, long pInL, long pOutB, long pOutL)
{
	UString src, des;
	char	encpass[32+1];
	int	pswdL, rc = 0;
	CWnd*	pWnd;
	BOOL	success = FALSE;

	*(int *)pOutL = 0;
	ZeroMemory(encpass, sizeof(encpass));
	switch (m_ca)
	{
	case caNO:
		ZeroMemory(&m_context, sizeof(APP_CONTEXT));
		ZeroMemory(&m_contextNew, sizeof(SD_API_CONTEXT_NEW));
		if (sk_if_cert_InitContextApp(&m_context, NULL, 0) == -1)
		{
			rc = sk_if_GetLastErrorCode();
			MessageBox("�������� ���� ������ �߻��� �� �ֻ���� ������(1588-0030, 1544-0050)�� ���ǹٶ��ϴ�.",
				"�������� �ʱ�ȭ ����", MB_OKCANCEL|MB_SYSTEMMODAL|MB_ICONINFORMATION);
			return rc;
		}

		m_name = _T("");
		pWnd = AfxGetMainWnd();
		sk_if_SetWrongPasswordLimit(1);
		m_contextNew.sd.bOldStorage = 0x00;
		m_contextNew.sd.bForceAllCaSearchMode = 1;
		strcpy(m_contextNew.sd.szUserId, (char *)m_name.operator LPCTSTR());
		ZeroMemory(m_contextNew.sd.szOldPasswd, sizeof(m_contextNew.sd.szOldPasswd));
		sk_if_SetKeySaferMode(1);
		if (pWnd && IsWindow(pWnd->GetSafeHwnd()))
			sk_if_DialogModalMode(pWnd->GetSafeHwnd());
		success = sk_if_CertSetSelectExt(&m_contextNew, CONTEXT_SELECT2, SEARCH_ALLMEDIA);
		if (!success)
		{
			rc = sk_if_GetLastErrorCode();
			switch (rc)
			{
			case 2501:		// ���
				sk_if_cert_static_context_release();
				*(int *)pOutL = -3;
				break;
			case 1001:		// ������ ������� (5ȸ)
			case 2417:
				*(int *)pOutL = -4;
				break;

			default:
				*(int *)pOutL = -2;
				break;
			}
			return rc;
		}

		if (sk_if_cert_preset_context(&m_context, &m_contextNew.sd) < 0)
		{
			rc = sk_if_GetLastErrorCode();
			MessageBox(sk_if_GetLastErrorMsg(), "��������-Preset ����", MB_OK|MB_SYSTEMMODAL|MB_ICONINFORMATION);
			*(int *)pOutL = -2;
			return rc;
		}
		break;
	case caRUN:
		pswdL = sizeof(m_context.pInterfaceContext->szOldPasswd);
		ZeroMemory(m_context.pInterfaceContext->szOldPasswd, pswdL);
		break;
	default:
		OnFire(FEV_CA, MAKELONG(guideCA, AE_ECERTIFY), 0);
		return -1;
	}

	m_sync.Lock();

	src.length = pInL;
	src.value = (unsigned char *) pInB;
	if (sk_if_cert_SignData(&m_context, encpass, &src, &des))
	{
		m_sync.Unlock();
		rc = sk_if_GetLastErrorCode();
		switch (rc)
		{
		case 1000:	// ���
			*(int *)pOutL = -3;
			break;
		case 2417:	// �н����� ����
			*(int *)pOutL = -4;
			break;
		default:
			*(int *)pOutL = -2;
			break;
		}

		return rc;
	}

	*(int *)pOutL = des.length;
	CopyMemory((void *) pOutB, des.value, des.length);
	sk_if_cert_MemFree(des.value);
	m_sync.Unlock();

	if (m_ca == caNO)
		m_ca = caOKx;
	m_calogon = true;
	savePasswd();

	return 0;

}

long CCertifyCtrl::CertifyName(long pBytes) 
{
	int	rc = 0;

	rc = strlen(m_contextNew.sd.szDN);
	CopyMemory((char *)pBytes, m_contextNew.sd.szDN, rc);
	return rc;
}
