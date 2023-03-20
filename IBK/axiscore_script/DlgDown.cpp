// DlgDown.cpp : implementation file
//

#include "stdafx.h"
#include "axiscore.h"
#include "DlgDown.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgDown dialog
#define TM_DOWNLOAD_WAIT 9999


CDlgDown::CDlgDown(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDown::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDown)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iDown = 0;
}


void CDlgDown::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDown)
	DDX_Control(pDX, IDC_STATIC_BITMAP, m_static);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDown, CDialog)
	//{{AFX_MSG_MAP(CDlgDown)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDown message handlers



UINT gThread_Func(void* pv)
{
	CDlgDown* pdlg = (CDlgDown*)pv;
	pdlg->WorkerThreadProc();
	return 0;
}

BOOL CDlgDown::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString	path;
	path = m_root;
	path.Replace("exe", "image");
	path = path + "\\AXISUPDATE.BMP";

m_slog.Format("[VERSION]  OnInitDialog [%s]\n", path);
OutputDebugString(m_slog);

	m_bitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (m_bitmap)
	{
		CSize	size, gap;
		BITMAP	bi;
		CBitmap	bitmap;
		CRect	cRc, wRc;
		CWnd* item;
	
		bitmap.DeleteObject();
		bitmap.Attach(m_bitmap);
		bitmap.GetBitmap(&bi);
		bitmap.Detach();

		GetClientRect(&wRc);


		wRc.right = wRc.left + bi.bmWidth;
		wRc.bottom = wRc.top + bi.bmHeight;

		SetWindowPos(NULL, 0, 0, wRc.Width(), wRc.Height(), SWP_NOZORDER | SWP_NOMOVE | SWP_SHOWWINDOW);
		CenterWindow();

		item = GetDlgItem(IDC_PROGRESS1);
		wRc.DeflateRect(20, 25);
		wRc.OffsetRect(0, 10);
		item->SetWindowPos(this, wRc.left, wRc.top, wRc.Width(), wRc.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);
	
		m_static.SetBitmap(m_bitmap);
		m_progress.SetRange(0, 100);
	}
	
	CWinThread* pWorkerThread;
	pWorkerThread = AfxBeginThread(gThread_Func, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	pWorkerThread->ResumeThread();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDown::WorkerThreadProc()
{
	StatusCallBack callback;
	callback.pdlg = this;
	KillTimer(TM_DOWNLOAD_WAIT); 
	SetTimer(TM_DOWNLOAD_WAIT, 30000, NULL);  //30초 기다려 본다 
	
m_slog.Format("[VERSION] m_root =[%s] m_regKey=[%s] m_strFileName=[%s] \r\n", m_root, m_regkey, m_strFileName);
OutputDebugString(m_slog);

	m_iDown++;
	HRESULT hr = URLDownloadToFile(NULL, m_strUrl, m_root + "\\" + m_strFileName, 0, &callback);

m_slog.Format("[VERSION] !!!!!!!!!!!!!!!!!!!!!URLDownloadToFile hr [%d]\n", hr);
OutputDebugString(m_slog);

	if (hr == S_OK)
	{		
m_slog.Format("[VERSION] !!!!!!!!!!!!!!!!!!!!! file down succress [%s]\n", m_root + "\\" + m_strFileName);
OutputDebugString(m_slog);	
			m_iDown--;
			if(m_iDown == 0)
				SendMessage(WM_CLOSE, 0, 0);
			KillTimer(TM_DOWNLOAD_WAIT);
	}
	else
	{

m_slog.Format("[VERSION] !!!!!!!!!!!!!!!!!!!!! file down Fail [%d]\n", GetLastError());
OutputDebugString(m_slog);

	}

	
}

BOOL CDlgDown::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message==WM_KEYDOWN)
	{
		if (pMsg->wParam==VK_RETURN)
			return TRUE;	
		else if (pMsg->wParam==VK_ESCAPE)
			return TRUE;
	
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgDown::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
		case TM_DOWNLOAD_WAIT:
		{
			AfxMessageBox("고객센터에 연락 바랍니다.. ");
			SendMessage(WM_CLOSE,0,0);

		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

