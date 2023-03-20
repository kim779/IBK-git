// axiscore.h : main header file for the AXISCORE application
//

#if !defined(AFX_AXISCORE_H__B77A1479_189F_4D03_878E_8A3D4ED695B2__INCLUDED_)
#define AFX_AXISCORE_H__B77A1479_189F_4D03_878E_8A3D4ED695B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAxiscoreApp:
// See axiscore.cpp for the implementation of this class
//

class CAxiscoreApp : public CWinApp
{
public:
	char m_flag;
	CString m_forcePort;
	CString m_forceIP;
	CString userID;
	CString	m_regKey;
	CString m_root;
	CString m_slog;
	CAxiscoreApp();
	HANDLE ProcessFind(char* strProcessName);
	BOOL Check_OS();
	BOOL Is64BitWindows();
//	void	WorkerThreadProc();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxiscoreApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL
protected:
	char	m_pk;
	
// Implementation
	void	write_Regkey();
	bool	IsInfoErr();
	void	WriteReg();
	bool	Install();
	bool	WebInstall();
	void	Check_VC_redist();
	//{{AFX_MSG(CAxiscoreApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


/////////////////////////////////StatusCallBack///////////////////////////////////////////////////////////////
	class StatusCallBack : public IBindStatusCallback
	{
		public:
		//	class CAxiscoreApp* pdlg;
			virtual HRESULT STDMETHODCALLTYPE OnStartBinding(
				/* [in] */ DWORD dwReserved,
				/* [in] */ IBinding* pib) {
				CString slog;
				slog.Format("[VERSION] OnStartBinding \n");
				OutputDebugString(slog);
				return S_OK;
			}

			virtual HRESULT STDMETHODCALLTYPE GetPriority(
				/* [out] */ LONG* pnPriority) {
				return S_OK;
			}

			virtual HRESULT STDMETHODCALLTYPE OnLowResource(
				/* [in] */ DWORD reserved) {
				return S_OK;
			}

			/*
			 URLDownloadToFile에 IBindStatusCallBack을 넣어서, OnProgress가 불려지지 않으면 블럭킹으로 간주한다.
			다운로드 함수에서는 블럭킹을 감지하거나, 다운로드가 끝나기를 기다린다.
			*/

			virtual HRESULT STDMETHODCALLTYPE OnProgress(
				/* [in] */ ULONG ulProgress,
				/* [in] */ ULONG ulProgressMax,
				/* [in] */ ULONG ulStatusCode,
				/* [in] */ LPCWSTR szStatusText)
			{
				CString slog;
				slog.Format("[VERSION] OnProgress \n");
				OutputDebugString(slog);
				if (ulProgressMax != 0)
				{
					
					int idata = int(100.0 * ulProgress / ulProgressMax);
					slog.Format("%d----[%d][%d]\n", idata, ulProgress, ulProgressMax);
					OutputDebugString(slog);
					int ii = 0;
					
				//	pdlg->m_progress.SetPos(idata);
					Sleep(100);
				
						
				}
				return S_OK;
			}

			virtual HRESULT STDMETHODCALLTYPE OnStopBinding(
				/* [in] */ HRESULT hresult,
				/* [unique][in] */ LPCWSTR szError) {
				return S_OK;
			}


			virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetBindInfo(
				/* [out] */ DWORD* grfBINDF,
				/* [unique][out][in] */ BINDINFO* pbindinfo) {
				return S_OK;
			}

			virtual /* [local] */ HRESULT STDMETHODCALLTYPE OnDataAvailable(
				/* [in] */ DWORD grfBSCF,
				/* [in] */ DWORD dwSize,
				/* [in] */ FORMATETC* pformatetc,
				/* [in] */ STGMEDIUM* pstgmed) {
				return S_OK;
			}

			virtual HRESULT STDMETHODCALLTYPE OnObjectAvailable(
				/* [in] */ REFIID riid,
				/* [iid_is][in] */ IUnknown* punk) {
				return S_OK;
			}

			STDMETHOD_(ULONG, AddRef)() { return 0; }
			STDMETHOD_(ULONG, Release)() { return 0; }

			STDMETHOD(QueryInterface)(
				/* [in] */ REFIID riid,
				/* [iid_is][out] */ void __RPC_FAR* __RPC_FAR* ppvObject) {
				return E_NOTIMPL;
			}

		public:
			StatusCallBack()
			{

			}

		private:

	};
/////////////////////////////////StatusCallBack///////////////////////////////////////////////////////////////

};

//---------------------------------------------------------------------------
//
//	CCommParam
//
class CCommParam : public CCommandLineInfo
{
// Construction
public:
	CCommParam();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommParam)
	virtual void ParseParam(LPCSTR lpszParam, BOOL bFlag, BOOL bLast);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCommParam();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISCORE_H__B77A1479_189F_4D03_878E_8A3D4ED695B2__INCLUDED_)
