// ExSignal.cpp: implementation of the CExSignal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Inc/ExSignal.h"
#include "../../Inc/ExOCXDLL.h"
#include "Include/LoadHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPCSTR CExSignal::CX2000BaseDesk::GetBasePath()
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	m_szPath = pThis->m_pBaseDesk2->GetBasePath();
	return m_szPath;
}

HWND  CExSignal::CX2000BaseDesk::GetDeskHwnd()
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)
	
	return pThis->m_pBaseDesk2->GetDeskHwnd();
}

LPDISPATCH CExSignal::CX2000BaseDesk::GetUnknown()
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetUnknown();
}

LPUNKNOWN CExSignal::CX2000BaseDesk::GetCtrlUnknown(LPCTSTR szName)
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetCtrlUnknown(szName);
}

HWND CExSignal::CX2000BaseDesk::GetCtrlHwnd(LPCTSTR szName)
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetCtrlHwnd(szName);
}

LPVOID CExSignal::CX2000BaseDesk::GetSharedInterface()
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetSharedInterface();
}

BOOL CExSignal::CX2000BaseDesk::InvokeEvent(HWND _hWnd, LPCSTR _szEventName, LPCSTR _szParam)
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	return pThis->InvokeEvent(_szEventName, _szParam);
}

BOOL CExSignal::CX2000BaseDesk::InvokeEvent2(LPCSTR _szCtrlName, LPCSTR _szEventName, LPCSTR _szParam)
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	return pThis->InvokeEvent(_szEventName, _szParam);
}
	
BOOL CExSignal::CX2000BaseDesk::SendKey(HWND _hWnd, UINT nChar, BOOL _bIsShift, BOOL _bIsCtrl) 
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	return pThis->m_pBaseDesk2->SendKey(_hWnd, nChar, _bIsShift, _bIsCtrl);
}

LPVOID CExSignal::CX2000BaseDesk::GetLSharedData()
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetLSharedData();
}



///////////////////////////////////////////////////////////////////////////////
// ������            : �ѵ���(Dong-Hyuk, Han)
// Email             : aviman@dooriic.co.kr
// ���� ��¥         : 2002/2/1
// �Լ� �̸�         : CExSignal::CX2000BaseDesk::CallCommand
// ���� Ÿ��         : void
// �Ķ����
//		LPCSTR _szCmd  : 
// �Լ� ����         : 
///////////////////////////////////////////////////////////////////////////////
void CExSignal::CX2000BaseDesk::CallCommand(LPCSTR _szCmd)
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	pThis->m_pBaseDesk2->CallCommand(_szCmd);
}

// * ����� �÷� ������ �����Ѵ�.
OLE_COLOR CExSignal::CX2000BaseDesk::GetUserColor(short nIndex, LPCTSTR szName, short nFlag)
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetUserColor(nIndex, szName, nFlag);
}

// * ���� �ƹ��͵� Ű���� ���� ���ǰ� �Ǿ����� �����Ƿ� �θ���.
// * �䱸 �Ǵ� �ʿ䰡 ������ ���⿡�� �����Ѵ�.
long CExSignal::CX2000BaseDesk::GetDeskInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetDeskInfo(szKey);
}

long CExSignal::CX2000BaseDesk::GetUserInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)
		
	return pThis->m_pBaseDesk2->GetUserInfo(szKey);
}

long CExSignal::CX2000BaseDesk::GetFrameInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)
		
	return pThis->m_pBaseDesk2->GetFrameInfo(szKey);
}

long CExSignal::CX2000BaseDesk::GetMainInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)
		
	return pThis->m_pBaseDesk2->GetMainInfo(szKey);
}




//* ���� ���� (2002.8.22 001) ����͸� �� �߰�
BOOL CExSignal::CX2000BaseDesk::FP_vAdviseDebug(LPCSTR sName, UINT pMonitoringCommand)
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	return pThis->m_pBaseDesk2->FP_vAdviseDebug(sName, pMonitoringCommand);
}


void CExSignal::CX2000BaseDesk::FP_vUnadviseDebug(LPCSTR sName, UINT pMonitoringCommand)
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	pThis->m_pBaseDesk2->FP_vUnadviseDebug(sName, pMonitoringCommand);
}

LPCSTR CExSignal::CX2000BaseDesk::FP_sGetMyVBName(HWND hWnd)
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	return pThis->m_pBaseDesk2->FP_sGetMyVBName(hWnd);
}

void CExSignal::CX2000BaseDesk::FP_vSendDebug(LPCSTR csSection, LPCSTR csKey, LPCSTR csValue)
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	pThis->m_pBaseDesk2->FP_vSendDebug(csSection, csKey, csValue);
}
//* ���� �� (2002.8.22 001) 


// CControlPos�� m_awndControls �ɹ������� �����͸� �����Ѵ�.
// �� ������ �������� �� ������Ʈ�� ������ ��� �ִ�.
long CExSignal::CX2000BaseDesk::FP_nGetCtrlPos()
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	return pThis->m_pBaseDesk2->FP_nGetCtrlPos();
}


// ������Ʈ�� �������� ��Ų��.
void CExSignal::CX2000BaseDesk::FP_vResizeCtrl()
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)
	
	pThis->m_pBaseDesk2->FP_vResizeCtrl();
}

// {{ OCX���� DESK�� �޼��� ���� �� �ֵ��� ��û(����) 2003.05.19
void CExSignal::CX2000BaseDesk::SetMapMessage(long lType, LPCSTR lpData)
{
	MYMETHOD_PROLOGUE(CExSignal, 2000BaseDesk)

	pThis->m_pBaseDesk2->SetMapMessage(lType, lpData);
}
// }}

/////////////////////////////////////////////////////////////////////////////
// CExSignal

CExSignal::CExSignal()
{
	m_pLoader = new CLoaderSignal(this);
	m_bCreated = FALSE;
	m_pParent = NULL;
}

CExSignal::~CExSignal()
{
	delete m_pLoader;
}

BOOL CExSignal::InvokeEvent(LPCSTR lpszEventName, LPCSTR lpszParam)
{
	ST_CTRLEVENT stEvent;
	stEvent.lpszEventName = lpszEventName;
	stEvent.lpszParam = lpszParam;

	if(m_pParent)
		m_pParent->SendMessage(RMSG_EVENT_SIGNAL, (WPARAM)GetSafeHwnd(), (LPARAM)&stEvent);

	return TRUE;
}

void CExSignal::SetMyBaseDesk2(CBaseDesk2* pBaseDesk)
{
	m_pBaseDesk2 = (CX2000BaseDesk*)pBaseDesk;
	m_x2000BaseDesk.m_hDeskHwnd = m_pBaseDesk2->m_hDeskHwnd;
	m_x2000BaseDesk.m_pDeskUnk = m_pBaseDesk2->m_pDeskUnk;
	m_x2000BaseDesk.m_pDisp = m_pBaseDesk2->m_pDisp;
	m_x2000BaseDesk.m_szPath = m_pBaseDesk2->m_szPath;
}

BOOL CExSignal::InitSignal(CDialog* pParent, UINT nID, CStringList* pListProperty, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	CRect rcControl;
	pParent->GetDlgItem(nID)->GetWindowRect(rcControl);
	pParent->ScreenToClient(rcControl);
	
	pParent->GetDlgItem(nID)->ShowWindow(SW_HIDE);

	return InitSignal(pParent, rcControl, nID, pListProperty, pBaseDesk);
}

BOOL CExSignal::InitSignal(CDialog* pParent, CRect& rcControl, UINT nID, CStringList* pListProperty, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	if(m_bCreated)
		m_pLoader->ExitSignal();
	
	if(pBaseDesk)
	{
		m_pParent = pParent;
		m_bCreated = TRUE;
		SetMyBaseDesk2(pBaseDesk);
	}
	else
	{
		m_pParent = pParent;

		while(!pParent->IsKindOf(RUNTIME_CLASS(CExOCXDLL)))
		{
			pParent = (CDialog*)pParent->GetParent();
			if(!pParent || !IsWindow(pParent->GetSafeHwnd()))
				return FALSE;
		}

		m_bCreated = TRUE;
		SetMyBaseDesk2(((CExOCXDLL*)pParent)->m_pBaseDesk);
	}
	
	LPVOID lpControl = m_pLoader->CreateSignal(m_pParent, &rcControl, nID, (long)&m_x2000BaseDesk, pListProperty);
	::SetPermanentHandle((HANDLE)m_pLoader->GetSafeHwnd(), (CObject*)lpControl);

	return (BOOL)lpControl;
}

BOOL CExSignal::InitSignal(CDialog* pParent, UINT nID, LPCTSTR* lpszProperties, int nSize, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	CRect rcControl;
	pParent->GetDlgItem(nID)->GetWindowRect(rcControl);
	pParent->ScreenToClient(rcControl);

	pParent->GetDlgItem(nID)->ShowWindow(SW_HIDE);

	return InitSignal(pParent, rcControl, nID, lpszProperties, nSize, pBaseDesk);
}

BOOL CExSignal::InitSignal(CDialog* pParent, CRect& rcControl, UINT nID, LPCTSTR* lpszProperties, int nSize, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	if(m_bCreated)
		m_pLoader->ExitSignal();
	
	if(pBaseDesk)
	{
		m_pParent = pParent;
		m_bCreated = TRUE;
		SetMyBaseDesk2(pBaseDesk);
	}
	else
	{
		m_pParent = pParent;

		while(!pParent->IsKindOf(RUNTIME_CLASS(CExOCXDLL)))
		{
			pParent = (CDialog*)pParent->GetParent();
			if(!pParent || !IsWindow(pParent->GetSafeHwnd()))
				return FALSE;
		}

		m_bCreated = TRUE;
		SetMyBaseDesk2(((CExOCXDLL*)pParent)->m_pBaseDesk);
	}

	LPVOID lpControl = m_pLoader->CreateSignal(m_pParent, &rcControl, nID, (long)&m_x2000BaseDesk, lpszProperties, nSize);
	::SetPermanentHandle((HANDLE)m_pLoader->GetSafeHwnd(), (CObject*)lpControl);

	return (BOOL)lpControl;
}

BOOL CExSignal::InitSignal(CDialog* pParent, UINT nID, LPCTSTR lpszProperties, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	CRect rcControl;
	pParent->GetDlgItem(nID)->GetWindowRect(rcControl);
	pParent->ScreenToClient(rcControl);

	pParent->GetDlgItem(nID)->ShowWindow(SW_HIDE);

	return InitSignal(pParent, rcControl, nID, lpszProperties, pBaseDesk);
}

BOOL CExSignal::InitSignal(CDialog* pParent, CRect& rcControl, UINT nID, LPCTSTR lpszProperties, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	if(m_bCreated)
		m_pLoader->ExitSignal();
	
	if(pBaseDesk)
	{
		m_pParent = pParent;
		m_bCreated = TRUE;
		SetMyBaseDesk2(pBaseDesk);
	}
	else
	{
		m_pParent = pParent;

		while(!pParent->IsKindOf(RUNTIME_CLASS(CExOCXDLL)))
		{
			pParent = (CDialog*)pParent->GetParent();
			if(!pParent || !IsWindow(pParent->GetSafeHwnd()))
				return FALSE;
		}

		m_bCreated = TRUE;
		SetMyBaseDesk2(((CExOCXDLL*)pParent)->m_pBaseDesk);
	}

	LPVOID lpControl = m_pLoader->CreateSignal(m_pParent, &rcControl, nID, (long)&m_x2000BaseDesk, lpszProperties);
	::SetPermanentHandle((HANDLE)m_pLoader->GetSafeHwnd(), (CObject*)lpControl);
	
	return (BOOL)lpControl;
}

BOOL CExSignal::InitSignal(CDialog* pParent, UINT nID, UINT nProperties, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	CRect rcControl;
	pParent->GetDlgItem(nID)->GetWindowRect(rcControl);
	pParent->ScreenToClient(rcControl);

	pParent->GetDlgItem(nID)->ShowWindow(SW_HIDE);

	return InitSignal(pParent, rcControl, nID, nProperties, pBaseDesk);
}

BOOL CExSignal::InitSignal(CDialog* pParent, CRect& rcControl, UINT nID, UINT nProperties, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	if(m_bCreated)
		m_pLoader->ExitSignal();
	
	if(pBaseDesk)
	{
		m_pParent = pParent;
		m_bCreated = TRUE;
		SetMyBaseDesk2(pBaseDesk);
	}
	else
	{
		m_pParent = pParent;

		while(!pParent->IsKindOf(RUNTIME_CLASS(CExOCXDLL)))
		{
			pParent = (CDialog*)pParent->GetParent();
			if(!pParent || !IsWindow(pParent->GetSafeHwnd()))
				return FALSE;
		}

		m_bCreated = TRUE;
		SetMyBaseDesk2(((CExOCXDLL*)pParent)->m_pBaseDesk);
	}

	LPVOID lpControl = m_pLoader->CreateSignal(m_pParent, &rcControl, nID, (long)&m_x2000BaseDesk, nProperties);
	::SetPermanentHandle((HANDLE)m_pLoader->GetHWnd(), (CObject*)lpControl);
	
	return (BOOL)lpControl;
}

OLE_HANDLE CExSignal::GetHWnd()
{
	return m_pLoader->GetHWnd();
}

HWND CExSignal::GetSafeHwnd()
{
	return m_pLoader->GetSafeHwnd();
}

void CExSignal::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint/* = TRUE*/)
{
	m_pLoader->MoveWindow(x, y, nWidth, nHeight, bRepaint);
}

void CExSignal::MoveWindow(LPCRECT lpRect, BOOL bRepaint/* = TRUE*/)
{
	m_pLoader->MoveWindow(lpRect, bRepaint);
}

BOOL CExSignal::SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
{
	return m_pLoader->SetWindowPos(pWndInsertAfter, x, y, cx, cy, nFlags);
}

BOOL CExSignal::ShowWindow(int nCmdShow)
{
	return m_pLoader->ShowWindow(nCmdShow);
}

void CExSignal::GetWindowRect(LPRECT lpRect)
{
	m_pLoader->GetWindowRect(lpRect);
}

void CExSignal::GetClientRect(LPRECT lpRect)
{
	m_pLoader->GetClientRect(lpRect);
}

void CExSignal::SetFocus()
{
	m_pLoader->SetFocus();
}

BOOL CExSignal::OnDrdsAdvise(LPCTSTR lpszCode)
{
	return m_pLoader->OnDrdsAdvise(lpszCode);
}

BOOL CExSignal::OffDrdsAdvise()
{
	return m_pLoader->OffDrdsAdvise();
}
