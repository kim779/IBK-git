#include "stdafx.h"
#include "liblogin.h"
#include "axislogin.h"
#include "ledgerx.h"


CString m_slog;
__declspec(dllexport) void* APIENTRY axLogin(void* data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_slog.Format("%s", data);
	LOG_OUTP(3, "axislogin", __FUNCTION__, m_slog);

	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();
	if (data == NULL)
	{
		CLogin* login = new CLogin;
		app->m_login = login;
		return login;
	}
	else
		return (void *)(app->m_login->SetLogin((char *)data).operator LPCTSTR());

	return NULL;
}

__declspec(dllexport) void APIENTRY axLoginLedger(void* ledger)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_slog.Format("%s", (char*)ledger);
	LOG_OUTP(3, "axislogin", __FUNCTION__, m_slog);

	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();
	struct	_ledgerH* ledgerH = (struct _ledgerH *)ledger;
	CopyMemory(ledgerH->usid, app->m_user, min(app->m_user.GetLength(), sizeof(ledgerH->usid)));
	CopyMemory(ledgerH->pcip, app->m_host, min(app->m_host.GetLength(), sizeof(ledgerH->pcip)));
	CopyMemory(ledgerH->brno, app->m_brno, min(app->m_brno.GetLength(), sizeof(ledgerH->brno)));
	CopyMemory(ledgerH->cogb, app->m_cogb, min(app->m_cogb.GetLength(), sizeof(ledgerH->cogb)));
	CopyMemory(ledgerH->rcnt, "0000", sizeof(ledgerH->rcnt));
}

__declspec(dllexport) void* APIENTRY axLedger(void* data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_slog.Format("%s", (char*)data);
	LOG_OUTP(3, "axislogin", __FUNCTION__, m_slog);

	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();
	CLedger* ledger = new CLedger(data);
	return ledger;
}

__declspec(dllexport) void APIENTRY axLedgerEx(void* ledger, void* data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_slog.Format("ledger=[%s] data=[%s]", (char*)ledger, (char*)data);
	LOG_OUTP(3, "axislogin", __FUNCTION__, m_slog);
	((CLedger *)ledger)->LedgerEx(data);
}

__declspec(dllexport) void* APIENTRY axGetLedger(void* ledger, int pos, int length)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_slog.Format("pos=[%d] length=[%d] ledger=[%s]", pos, length, (char*)ledger);
	LOG_OUTP(3, "axislogin", __FUNCTION__, m_slog);

	return (void *)((CLedger *)ledger)->GetLedger(pos, length).operator LPCTSTR();
}

__declspec(dllexport) void APIENTRY axSetLedger(void* ledger, int pos, int length, void* data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//m_slog.Format("pos=[%d] length=[%d]  ledger=[%s] data=[%s]", pos, length, (char*)ledger, (char*)data);
	//LOG_OUTP(3, "axislogin", __FUNCTION__, m_slog);

	((CLedger *)ledger)->SetLedger(pos, length, data);
}

__declspec(dllexport) void* APIENTRY axGetLedgerEx(void* ledger, int id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_slog.Format("id=[%d] ledger=[%s]", id, (char*)ledger);
	LOG_OUTP(3, "axislogin", __FUNCTION__, m_slog);
//	m_slog.Format("[%s]", ((CLedger*)ledger)->GetClassledger());
//	LOG_OUTP(3, "axislogin", __FUNCTION__, m_slog);
	return (void *)((CLedger *)ledger)->GetLedgerEx(id, NULL).operator LPCTSTR();
}

__declspec(dllexport) void* APIENTRY axGetLedgerData(void* ledger, void* data, int id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_slog.Format("id=[%d] ret=[%d]  ledger=[%s] data=[%s] ", 
		id, (char*)((CLedger*)ledger)->GetLedgerEx(id, data).operator LPCTSTR(), (char*)ledger, data);
	LOG_OUTP(3, "axislogin", __FUNCTION__, m_slog);

	return (void *)((CLedger *)ledger)->GetLedgerEx(id, data).operator LPCTSTR();
}

__declspec(dllexport) void* APIENTRY axLoginData(int id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();

	m_slog.Format("id=[%d] ret=[%s] ", id, (char*)app->m_login->GetLogin(id).operator LPCTSTR());
	LOG_OUTP(3, "axislogin", __FUNCTION__, m_slog);

	return (void *)(app->m_login->GetLogin(id).operator LPCTSTR());
}
