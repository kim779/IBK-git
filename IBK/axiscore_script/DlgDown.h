#if !defined(AFX_DLGDOWN_H__2C110034_A7D3_4AB5_A54B_AD7290B01236__INCLUDED_)
#define AFX_DLGDOWN_H__2C110034_A7D3_4AB5_A54B_AD7290B01236__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDown.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDown dialog


class CDlgDown : public CDialog
{
// Construction
public:
	CDlgDown(CWnd* pParent = NULL);   // standard constructor
	void WorkerThreadProc();
	void Unzip_Thread();

	CString m_strFileName;
	CString m_strUrl;
	CString m_regkey;
	CString m_slog;
	CString m_root;
	HBITMAP	m_bitmap;
	int m_iDown;
// Dialog Data
	//{{AFX_DATA(CDlgDown)
	enum { IDD = IDD_DLG_DOWN };
	CStatic	m_static;
	CProgressCtrl	m_progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDown)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDown)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


	/////////////////////////////////StatusCallBack///////////////////////////////////////////////////////////////
	class StatusCallBack : public IBindStatusCallback
	{
		public:
			class CDlgDown* pdlg;
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
		//		CString slog;
			//	slog.Format("[VERSION] OnProgress \n");
			//	OutputDebugString(slog);
				if (ulProgressMax != 0)
				{
					
					int idata = int(100.0 * ulProgress / ulProgressMax);
				//	slog.Format("%d----[%d][%d]\n", idata, ulProgress, ulProgressMax);
				//	OutputDebugString(slog);
				//	int ii = 0;
				//	int ipro = pdlg->m_progress.GetPos();
				//	pdlg->m_progress.SetPos(idata + ipro);
					pdlg->m_progress.SetPos(idata);
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

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDOWN_H__2C110034_A7D3_4AB5_A54B_AD7290B01236__INCLUDED_)
