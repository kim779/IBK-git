// UserSaveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cx_updown.h"
#include "UserSaveDlg.h"
#include "afxdialogex.h"
#include "Controlwnd.h"


#include "../../../h/axisfire.h"
#include "../../../h/axisvar.h"

#define	UPMAXSIZE	1024*15
#define	TM_WAIT		1234
#define	WM_DOWNFILE	WM_USER+1000
#define	WM_UPFILE	WM_USER+1001

// CUserSaveDlg dialog

IMPLEMENT_DYNAMIC(CUserSaveDlg, CDialogEx)

CUserSaveDlg::CUserSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserSaveDlg::IDD, pParent)
{
	m_index = 0;
	m_parent = pParent;
}

CUserSaveDlg::~CUserSaveDlg()
{
}

void CUserSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_UPDOWN, m_tab);
}


BEGIN_MESSAGE_MAP(CUserSaveDlg, CDialogEx)
	ON_MESSAGE(WM_USER, OnUSER)
	ON_MESSAGE(WM_DOWNFILE, OnDownfile)
	ON_MESSAGE(WM_UPFILE, OnUpfile)
	ON_BN_CLICKED(IDC_BTN_UPDOWN, &CUserSaveDlg::OnBnClickedBtnUpdown)
	ON_WM_CREATE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_UPDOWN, &CUserSaveDlg::OnSelchangeTabUpdown)
END_MESSAGE_MAP()


// CUserSaveDlg message handlers


void CUserSaveDlg::OnBnClickedBtnUpdown()
{
	if(m_tab.GetCurSel() == 0)
		Upload();
	else
		Upload();
}

void CUserSaveDlg::Upload()
{
	OnUpfile(0,0);
}

void CUserSaveDlg::Download()
{
	
}

LRESULT CUserSaveDlg::OnUpfile(WPARAM wParam, LPARAM lParam)
{
	CString file, fileN, str;
	CFile hFile;
	char sndB[UPMAXSIZE];
	bool match = false;

	if(!m_pUpDlg->IsWindowVisible())
		return -1;

	m_selectlist.RemoveAll();
	m_pUpDlg->GetListData(m_selectlist);

	for(int ii = m_index; ii < m_selectlist.GetSize(); ii++)
	{
		fileN = m_selectlist.GetAt(ii);
		//file.Format(_T("\\%s\\%s\\%s"), m_home, USRDIR, m_user, fileN);
		if(!hFile.Open(fileN, CFile::modeRead|CFile::typeBinary))
			continue;

		int fileL = hFile.GetLength();
		int fSize = L_usr + fileL;
		if (fSize > UPMAXSIZE)
		{
			hFile.Close();
			continue;
		}

		usrinfo* info = (usrinfo*)sndB;
		memset(sndB, ' ', fSize);
		hFile.Read(&sndB[L_usr], fileL);
		hFile.Close();

		sprintf(info->gubn, "UD");
		info->dirt[0] = 'U';	// pc -> host
		CopyMemory(info->name, fileN, fileN.GetLength());
		sprintf(info->nblc, "%05d", fileL);
		info->retc[0] = '0';
		m_tr = _T("PIHIUPDN");
		((CControlwnd*)m_parent)->sendTR(_T("PIHIUPDN"), CString(sndB, UPMAXSIZE));
		match = true;
		m_index = ii+1;
		break;
	}

	/*
	if (!match)
	{
		if (m_inter)
			InterUpload();
		else
		{
			for (ii = 0; ii < m_splitlist.GetSize(); ii++)
				DeleteFile(m_splitlist.GetAt(ii));
			m_cursor = 0;
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			if (m_startUP)
				ShowMsg();
		}
	}
	*/
	return 0;
}

LRESULT CUserSaveDlg::OnDownfile(WPARAM wParam, LPARAM lParam)
{
	CString		file, fileN, str;

	bool	match  = false;
	for (int ii = m_index; ii < m_selectlist.GetSize(); ii++)
	{
		fileN = m_selectlist.GetAt(ii);
		fileN.MakeLower();
		int jj = 0;
		for (jj = 0; jj < m_filelist.GetSize(); jj++)
		{
			str = m_filelist.GetAt(jj);
			str.MakeLower();
			if (fileN != str)
				continue;
			match  = true;
			break;
		}

		if (!match)
		{
			m_index++;
			continue;
		}

		char	sndB[1024];
		usrinfo* info = (usrinfo *) sndB;
		memset(sndB, ' ', sizeof(sndB));

		sprintf(info->gubn, "UD");
		info->dirt[0] = 'D';	// host -> pc
		int	len = m_filelist.GetAt(jj).GetLength();
		if (sizeof(info->name) < len)
			continue;
		m_tr = "PIHIUPDN";
		((CControlwnd *) m_parent)->sendTR(_T("PIHIUPDN"),  CString(sndB, UPMAXSIZE));
		m_index++;
		break;
	}

	/*
	if (m_index >= m_selectlist.GetSize() && !match)
	{
		if (m_inter)
			InterDownload();
		else
		{
			fileMerge();
			m_cursor = 0;
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}

		CString msg = "설정정보를 서버에서 불러왔습니다";
		if (m_bProgress)	MessageBox(msg, "확인");

		return 0;
	}
	*/
	return 0;
}

LONG CUserSaveDlg::OnUSER(WPARAM wParam, LPARAM lParam)
{
	switch(LOBYTE(LOWORD(wParam)))
	{
		case DLL_OUB:
		{
			char* pBytes = (char*)lParam;
			struct _extTH* extTH = (struct _extTH *)pBytes;
			pBytes += L_extTH;
		}
		break;
	}
	return 0;
}

void CUserSaveDlg::process_OUB(char* recB)
{
	if(!m_tr.Compare(_T("PIHOMYST")))
	{
		if (m_upload)
			process_INTERUP(recB);
		else
			process_INTERDOWN(recB);
		return;
	}

	usrinfo* info = (usrinfo *) recB;
	CString	 gubn = CString(info->gubn, sizeof(info->gubn));

	if (gubn == "UL")
	{
		m_filelist.RemoveAll();
		int count = atoi(CString(info->nblc, sizeof(info->nblc)).GetBuffer());
		d_list* list = (d_list*)&recB[L_usr];
		bool	issplit = false;

		for (int ii = 0; ii < count; ii++)
		{
			CString name = CString(list[ii].name, sizeof(list[ii].name));
			name.TrimRight();
			m_filelist.Add(name);
			if (name == "split.ini")	
				issplit = true;
		}

		if(issplit)
		{
			m_index = 0;
			m_selectlist.RemoveAll();
			m_selectlist.Add("split.ini");
			m_bSplit = true;
			PostMessage(WM_DOWNFILE);
		}
		else
		{
			m_cursor = 0;
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
	}
	else if(gubn == "UD" && (!m_upload || m_bSplit))
	{
		if (m_bSplit)	
			m_bSplit = false;

		CString		file, fileN, str, szDir;
		CFile			hFile;
		UINT		nOpenFlags;

		if(info->retc[0] == 'E')
		{
			str = CString(info->emsg, sizeof(info->emsg));
			::MessageBox(m_hWnd, str, "에러", MB_ICONINFORMATION);
			PostMessage(WM_DOWNFILE);
			return;
		}

		fileN = CString(info->name, sizeof(info->name));
		fileN.TrimRight();
		if (fileN.Find("gex/") != -1)
		{
			fileN.Replace("gex/", "");
			file.Format("%s\\gex\\%s\\%s", m_home, m_user, fileN);
			nOpenFlags = CFile::modeCreate|CFile::modeWrite|CFile::typeBinary;

			szDir.Format("%s\\gex\\%s", m_home, m_user);
			CreateDirectory(szDir, NULL);
		}
		else
		{
			file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, fileN);
			nOpenFlags = CFile::modeCreate|CFile::modeWrite;
		}
		::DeleteFile(file);
		if (!hFile.Open(file, nOpenFlags))
		{
			::MessageBox(m_hWnd, "파일 생성오류 입니다.", "파일에러", MB_ICONINFORMATION);
			PostMessage(WM_DOWNFILE);
			return;
		}

		hFile.Write(&recB[L_usr], atoi(CString(info->nblc, sizeof(info->nblc))));
		hFile.Close();

	//	if (m_bProgress)	
	//		m_progress.SetPos(m_index);
		PostMessage(WM_DOWNFILE);
	}
	else
	{
	//	m_progress.SetPos(m_index);
		PostMessage(WM_UPFILE);
	}
}

void CUserSaveDlg::process_INTERUP(char* recB)
{

}

void CUserSaveDlg::process_INTERDOWN(char* recB)
{

}

int CUserSaveDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CPoint pt;
	GetCursorPos(&pt);
	//ClientToScreen(&pt);
	SetDlgPos(pt.x, pt.y);
	return 0;
}

void CUserSaveDlg::SetDlgPos(int x, int y)
{
	if(x < 0 || y < 0)
		return;

	CRect rc;
	GetClientRect(rc);
	SetWindowPos(NULL, x, y, rc.Width(), rc.Height() + 25, SWP_SHOWWINDOW);
	/*
	int scWidth, scHeight;
	scWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	scHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	CRect rc;
	GetClientRect(&rc);

	int gabX = scWidth - (x - rc.Width());
	int	gabY = scHeight - (y + rc.Height());
	if (gabX < 0)
		x += gabX - 7;
	if (gabY < 0)
		y += gabY - 7;

	SetWindowPos(NULL, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	*/
}


BOOL CUserSaveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CRect rect;
	m_pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_UPDOWN);
	//ScreenToClient(&rect);
	m_pTab->GetClientRect(&rect);
	TC_ITEM item;
	item.mask = TCIF_TEXT;

	item.pszText = _T("Up");
	m_pTab->InsertItem(0, &item);

	item.pszText = _T("Down");
	m_pTab->InsertItem(1, &item);

	m_pUpDlg = new CUpdlg;
	m_pUpDlg->Create(IDD_DLGUP, m_pTab);
	m_pUpDlg->MoveWindow(0, 20, rect.Width(), rect.Height());
	m_pUpDlg->ShowWindow(SW_SHOW);


	m_pDownDlg = new CDowndlg;
	m_pDownDlg->Create(IDD_DLGDOWN, m_pTab);
	m_pDownDlg->MoveWindow(0, 20, rect.Width(), rect.Height());
	m_pDownDlg->ShowWindow(SW_HIDE);
	
//	m_pDownDlg = new CDowndlg;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CUserSaveDlg::OnSelchangeTabUpdown(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nTab = m_pTab->GetCurSel();

	switch(nTab)
	{
		case 0:
			m_pUpDlg->ShowWindow(SW_SHOW);
			m_pDownDlg->ShowWindow(SW_HIDE);
			((CWnd*)GetDlgItem(IDC_BTN_UPDOWN))->SetWindowTextA(_T("Up load"));
			break;
		case 1:
			m_pUpDlg->ShowWindow(SW_HIDE);
			m_pDownDlg->ShowWindow(SW_SHOW);
			((CWnd*)GetDlgItem(IDC_BTN_UPDOWN))->SetWindowTextA(_T("Down load"));
			break;
	}
	*pResult = 0;
}
