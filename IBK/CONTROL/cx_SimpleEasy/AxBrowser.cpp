#include "stdafx.h"
#include "AxBrowser.h"
#include "Strsafe.h"
#include <stdio.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

[[ gsl::suppress(26409) ]]
IMPLEMENT_DYNCREATE(CAxBrowser, CHtmlView)

CAxBrowser::CAxBrowser()
{
	//{{AFX_DATA_INIT(CAxBrowser)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAxBrowser::~CAxBrowser()
{
}

void CAxBrowser::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxBrowser)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAxBrowser, CHtmlView)
	//{{AFX_MSG_MAP(CAxBrowser)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#ifdef _DEBUG
void CAxBrowser::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CAxBrowser::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

#endif //_DEBUG

void CAxBrowser::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel) 
{
	/*
	CWnd *pWnd = GetParent();
	LPCSTR stURL = "axis://menu/";
	CString mapn, url = lpszURL;

	if (url.Find(stURL)!=-1 && pWnd)
	{
		url.Replace(stURL, "");
		if (url.GetLength()==7) 
			url = url.Right(6);
		mapn.Format("IB200100 /t/S/d1301\t%s\n", url);
		pWnd->SendMessage(WX_EVT_MAPN, NULL, (LPARAM)(LPCSTR)mapn);
		*pbCancel = TRUE;
	}
*/
	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags,	lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}

void CAxBrowser::NavigateFromMemory( LPCSTR dat, int len )
{
	if (dat==NULL) return;

	HRESULT	hr = S_OK;
	HGLOBAL hGlobal = nullptr;
	IStream* pStream = nullptr;
	ULONG	lWritten = 0;

	do {
		HGLOBAL hGlobal = GlobalAlloc(GHND, len);
		if (hGlobal == nullptr)
			break;

		hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
		if (FAILED(hr) || pStream == nullptr)
			break;

		hr = pStream->Write(dat, len, &lWritten);
		if (SUCCEEDED(hr))
		{
			LARGE_INTEGER pos;
			pos.QuadPart = 0;
			pStream->Seek((LARGE_INTEGER)pos, STREAM_SEEK_SET, NULL);
		}
		else
			break;

		NavigateFromStream(pStream);

	} while (FALSE);

	if (pStream) pStream->Release();
	if (hGlobal) GlobalFree(hGlobal);
}

void CAxBrowser::NavigateFromStream(IStream *pStream)
{
	if (pStream == nullptr)
		return;

	HRESULT hr = S_OK;
	IDispatch* pHtmlDoc = nullptr;
	IPersistStreamInit* pPersistStreamInit = nullptr;

	do {
		
		hr = m_pBrowserApp->get_Document(&pHtmlDoc);
		if (FAILED(hr) || pHtmlDoc == nullptr)
			break;

		hr = pHtmlDoc->QueryInterface(IID_IPersistStreamInit, (void**)&pPersistStreamInit);
		if (FAILED(hr) || pPersistStreamInit == nullptr)
			break;

		hr = pPersistStreamInit->InitNew();
		if (FAILED(hr))
			break;
		
		hr = pPersistStreamInit->Load(pStream);
		
	} while (FALSE);
	
	if (pPersistStreamInit) pPersistStreamInit->Release();
	if (pHtmlDoc) pHtmlDoc->Release();
}
