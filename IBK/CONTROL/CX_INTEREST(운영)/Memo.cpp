// Memo.cpp : implementation file
//

#include "stdafx.h"
#include "CX_INTERGRID.h"
#include "Memo.h"
#include "memdc.h"
#include "sharemsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int headerH = 20, dataH = 20;
const int titleH = 18, tabW = 150, gap1 = 1, exitW = 14;

/////////////////////////////////////////////////////////////////////////////
// CEditX

CEditX::CEditX()
{
}

CEditX::~CEditX()
{
	m_brush.DeleteObject();
}


BEGIN_MESSAGE_MAP(CEditX, CEdit)
	//{{AFX_MSG_MAP(CEditX)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditX message handlers
BOOL CEditX::Create(CWnd* parent, UINT nID)
{
	m_brush.DeleteObject();
	m_brush.CreateSolidBrush(GetSysColor(COLOR_INFOBK));

	const DWORD dwStyle = WS_CHILD|WS_VISIBLE|ES_LEFT|ES_AUTOVSCROLL|ES_WANTRETURN|ES_MULTILINE;
	return CEdit::Create(dwStyle, CRect(), parent, nID);
}

HBRUSH CEditX::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetBkColor(GetSysColor(COLOR_INFOBK));
	
	return m_brush;
}


/////////////////////////////////////////////////////////////////////////////
// CMemo

CMemo::CMemo(CString code, CString jnam, CString root, CString name)
{
	m_edit = nullptr;

	m_code = code;
	m_root = root;
	m_name = name;

	m_jnam.Format("%s �޸�", jnam);
}

CMemo::~CMemo()
{	
}


BEGIN_MESSAGE_MAP(CMemo, CWnd)
	//{{AFX_MSG_MAP(CMemo)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SETCURSOR()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ACTIVATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMemo message handlers
BOOL CMemo::createMemo(CWnd* parent, CWnd* sendW, CRect rect)
{
	m_parent = parent;
	m_sendW  = sendW;

	CString clsName = AfxRegisterWndClass(0);
	const UINT	dwStyle = WS_POPUP|WS_BORDER;
	const UINT	dwExStyle = WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_DLGMODALFRAME;

	if (!CreateEx(dwExStyle, clsName, nullptr, dwStyle, rect, nullptr, 0, nullptr))
	{
		TRACE("Create memo error....!!\n");
		return FALSE;
	}

	return TRUE;
}

int CMemo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_font = getAxFont("����ü", 9, 0);
	m_edit = std::make_unique<CEditX>(); 
	m_edit->Create(this, 100); m_edit->SetFont(m_font);

//	loadMemo(m_code); 
	return 0;
}

void CMemo::ShowMemo(CString code, CString name, CRect rect)
{
	m_code = code;
	m_jnam = name;
	loadMemo(m_code);
	MoveWindow(rect, FALSE);
	ShowWindow(SW_SHOW);
}

void CMemo::OnSetFocus(CWnd* pOldWnd) 
{
	MSG msg{};
	m_edit->SetFocus();
	for(m_fDone = FALSE; !m_fDone; WaitMessage())
	{
		while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if ((msg.message == WM_KILLFOCUS && msg.hwnd == this->m_hWnd) || 
				((msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN) && (msg.hwnd != this->m_hWnd && msg.hwnd != m_edit->m_hWnd)) ||
				(msg.message == WM_USER && msg.hwnd == this->m_hWnd))
			{ 
				if (msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN)
					::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
				m_fDone = TRUE;
				break;
			}

			if (true || !IsDialogMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (m_fDone)
			break;
	}

	ShowWindow(SW_HIDE);
	if (!m_code.IsEmpty())
	{
		saveMemo(m_code);
		m_parent->SendMessage(WM_MANAGE, MAKEWPARAM(MK_MEMOUPDATE, 0), (LPARAM) 0);
	}

	CWnd::OnSetFocus(pOldWnd);
}

void CMemo::procedureDeleteMemo(CString code)
{
	if (!code.IsEmpty())
	{
		deleteMemo(code);
		m_parent->SendMessage(WM_MANAGE, MAKEWPARAM(MK_MEMOUPDATE, 0), (LPARAM) 0);
	}	
}


BOOL CMemo::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint point; GetCursorPos(&point); ScreenToClient(&point);
	CRect rect;   GetClientRect(&rect);

	rect.left = rect.right - 20;
	rect.top  = rect.bottom - 20;

	if (rect.PtInRect(point))
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
	else
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


void CMemo::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	if (nState == WA_INACTIVE) PostMessage(WM_USER, 0, 0);
}

void CMemo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
#ifdef _DEBUG
	CRect	rc;
	CPen* pOldPen;

	GetClientRect(&rc);
	rc.DeflateRect(0, 18, 0, 0);

	dc.FillSolidRect(rc, GetSysColor(COLOR_INACTIVEBORDER));
	drawCaption(&dc);

	pOldPen = (CPen*)dc.SelectObject(getAxPen(GetSysColor(COLOR_HIGHLIGHTTEXT), 1, PS_SOLID));

	// exit button
	GetClientRect(&rc);
	rc.left = rc.right - exitW;
	rc.top = rc.top + 5;
	rc.bottom = rc.top + 10;
	rc.right = rc.left + 10;
	dc.Rectangle(rc);
	rc.DeflateRect(1, 1);
	dc.FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));
	rc.InflateRect(1, 1);
	dc.MoveTo(rc.left, rc.top);
	dc.LineTo(rc.right - 1, rc.bottom - 1);
	dc.MoveTo(rc.right - 1, rc.top);
	dc.LineTo(rc.left, rc.bottom - 1);
#else
	xxx::CMemDC	memdc(&dc);
	CRect	rc;
	CPen	*pOldPen;

	GetClientRect(&rc); 
	rc.DeflateRect(0, 18, 0, 0);

	memdc.FillSolidRect(rc, GetSysColor(COLOR_INACTIVEBORDER));
	drawCaption(&memdc);

	pOldPen = (CPen*)memdc.SelectObject(getAxPen(GetSysColor(COLOR_HIGHLIGHTTEXT), 1, PS_SOLID));

	// exit button
	GetClientRect(&rc);
	rc.left = rc.right - exitW;
	rc.top = rc.top + 5;
	rc.bottom = rc.top + 10;
	rc.right = rc.left + 10;
	memdc.Rectangle(rc);
	rc.DeflateRect(1, 1);
	memdc.FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));
	rc.InflateRect(1, 1);
	memdc.MoveTo(rc.left, rc.top);
	memdc.LineTo(rc.right - 1, rc.bottom - 1);
	memdc.MoveTo(rc.right - 1, rc.top);
	memdc.LineTo(rc.left, rc.bottom - 1);
#endif
}

void CMemo::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect rect; GetClientRect(&rect);
	rect.DeflateRect(1, headerH+1, 1, 2);
	m_edit->MoveWindow(&rect);
}

void CMemo::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect	rect, crect; GetClientRect(&crect);

	rect = crect;
	rect.top   += gap1;
	rect.bottom = rect.top + titleH;
	rect.right -= exitW;

	if (rect.PtInRect(point))
	{
		CWnd::OnLButtonDown(nFlags, point);
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		return;
	}

	rect = crect;
	rect.left   = rect.right - exitW;
	rect.top    = rect.top   +  4;
	rect.bottom = rect.top   + 10;
	rect.right  = rect.left  + 10;

	if (rect.PtInRect(point)) ShowWindow(SW_HIDE);	

	CWnd::OnLButtonDown(nFlags, point);
}

void CMemo::drawCaption(CDC *pDC)
{ 
	CFont* pOldFont{};
	CRect	clientRc, rect;
	
	GetClientRect(&clientRc); rect = clientRc;

	rect.top += gap1; rect.bottom = rect.top + titleH;
	pDC->FillSolidRect(rect, GetSysColor(COLOR_INFOBK));

	pDC->SetTextColor(RGB(38, 55, 100));
	pOldFont = (CFont*)pDC->SelectObject(getAxFont("����ü", 9, 3));

	rect.right = rect.left + tabW;
	pDC->DrawText(m_jnam, -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject(pOldFont);

	rect.left = rect.right; rect.right = clientRc.right; rect.DeflateRect(0, 2, 2, 2);
	pDC->FillSolidRect(rect, GetSysColor(COLOR_HIGHLIGHT));
}

void CMemo::deleteMemo(CString skey)
{
	if (skey.IsEmpty() || !m_edit) 
		return;
	
	CString str, rDir, wDir;
	rDir.Format("%s\\%s\\%s\\memo.mmo", m_root, USER, m_name);
	wDir.Format("%s\\%s\\%s\\memo.mm2", m_root, USER, m_name);
	
	//������ �߰�
	str = "";
	
	char key[12]{}, lBytes[4]{};
	CString dat;
	
	TRY
	{
		CFile wFile(wDir, CFile::modeWrite | CFile::modeCreate);
		
		if (!str.IsEmpty())
		{
			FillMemory(key, sizeof(key), ' ');
			FillMemory(lBytes, sizeof(lBytes), ' ');
			
			strcpy(key, skey.operator LPCTSTR());
			itoa(str.GetLength(), lBytes, 10);
			wFile.Write(key, 12);
			wFile.Write(lBytes, 4);
			wFile.Write(str, str.GetLength());
		}
		
		CFileFind	finder;
		if (!finder.FindFile(rDir))
		{
			wFile.Close();
			CopyFile(wDir, rDir, false);
			DeleteFile(wDir);
			return;
		}
		//		m_parent->SendMessage(WM_MANAGE, MAKEWPARAM(MK_MEMOUPDATE, 0), (LPARAM) 0);
		
		CFile rFile(rDir, CFile::modeRead);
		
		UINT nBytesRead;
		do
		{
			FillMemory(key, sizeof(key), ' ');
			FillMemory(lBytes, sizeof(lBytes), ' ');
			
			nBytesRead = rFile.Read(&key, sizeof(key));
			if (nBytesRead == sizeof(key))
			{
				nBytesRead = rFile.Read(&lBytes, sizeof(lBytes));
				if (nBytesRead == sizeof(lBytes))
				{
					int lSize = atoi(CString(lBytes, 4));
					nBytesRead = rFile.Read(dat.GetBuffer(lSize), lSize);
					
					if ((int)nBytesRead != lSize)
						break;
					
					if (!skey.Compare(key))
					{
						dat.ReleaseBuffer();
						continue;
					}
					wFile.Write(key, 12);
					wFile.Write(lBytes, 4);
					wFile.Write(dat, nBytesRead);
					dat.ReleaseBuffer();
				}
				else
					break;
			}
			else
				break;
			
		}while((int)nBytesRead);
		
		
		rFile.Close();

		wFile.Close();
		CopyFile(wDir, rDir, false);
		DeleteFile(wDir);
		
		m_parent->SendMessage(WM_MANAGE, MAKEWPARAM(MK_MEMOUPDATE, 0), (LPARAM) 0);
	}CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH
}

void CMemo::saveMemo(CString skey)
{
	if (skey.IsEmpty() || !m_edit) 
		return;

	CString str, rDir, wDir;
	rDir.Format("%s\\%s\\%s\\memo.mmo", m_root, USER, m_name);
	wDir.Format("%s\\%s\\%s\\memo.mm2", m_root, USER, m_name);

	m_edit->GetWindowText(str);
	
	char key[12]{}, lBytes[4]{};
	CString dat;
	CFile wFile(wDir, CFile::modeWrite | CFile::modeCreate);

	if (!str.IsEmpty())
	{
		FillMemory(key, sizeof(key), ' ');
		FillMemory(lBytes, sizeof(lBytes), ' ');

		strcpy(key, skey.operator LPCTSTR());
		itoa(str.GetLength(), lBytes, 10);
		wFile.Write(key, 12);
		wFile.Write(lBytes, 4);
		wFile.Write(str, str.GetLength());
	}

	CFileFind	finder;
	if (!finder.FindFile(rDir))
	{
		wFile.Close();
		CopyFile(wDir, rDir, false);
		DeleteFile(wDir);
		return;
	}
	
	TRY
	{
//		m_parent->SendMessage(WM_MANAGE, MAKEWPARAM(MK_MEMOUPDATE, 0), (LPARAM) 0);

		CFile rFile(rDir, CFile::modeRead);
		
		UINT nBytesRead;
		do
		{
			FillMemory(key, sizeof(key), ' ');
			FillMemory(lBytes, sizeof(lBytes), ' ');

			nBytesRead = rFile.Read(&key, sizeof(key));
			if (nBytesRead == sizeof(key))
			{
				nBytesRead = rFile.Read(&lBytes, sizeof(lBytes));
				if (nBytesRead == sizeof(lBytes))
				{
					int lSize = atoi(CString(lBytes, 4));
					nBytesRead = rFile.Read(dat.GetBuffer(lSize), lSize);
					
					if ((int)nBytesRead != lSize)
						break;
					
					if (!skey.Compare(key))
					{
						dat.ReleaseBuffer();
						continue;
					}
					wFile.Write(key, 12);
					wFile.Write(lBytes, 4);
					wFile.Write(dat, nBytesRead);
					dat.ReleaseBuffer();
				}
				else
					break;
			}
			else
				break;
			
		}while((int)nBytesRead);

		
		rFile.Close();
	}CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH

	wFile.Close();
	CopyFile(wDir, rDir, false);
	DeleteFile(wDir);
	
	m_parent->SendMessage(WM_MANAGE, MAKEWPARAM(MK_MEMOUPDATE, 0), (LPARAM) 0);
}

void CMemo::loadMemo(CString skey)
{
	if (skey.IsEmpty())
		return;

	CString dir, dat;
	char key[12]{}, lBytes[4]{};

	dir.Format("%s\\%s\\%s\\memo.mmo", m_root, USER, m_name);
	CFileFind	finder;
	if (!finder.FindFile(dir))
		return;

	TRY
	{ 
		m_edit->SetWindowText("");
		CFile file(dir, CFile::modeRead);
		UINT nBytesRead;
		do
		{
			nBytesRead = file.Read(&key, sizeof(key));
			if (nBytesRead == sizeof(key))
			{
				nBytesRead = file.Read(&lBytes, sizeof(lBytes));
				if (nBytesRead == sizeof(lBytes))
				{
					int lSize = atoi(CString(lBytes, 4));
					nBytesRead = file.Read(dat.GetBufferSetLength(lSize), lSize);

					if ((int)nBytesRead != lSize)
					{
						dat.ReleaseBuffer();
						break;
					}

					if (!skey.Compare(key))
					{
						m_edit->SetWindowText(dat);
						m_edit->SetSel(0, -1);
						dat.ReleaseBuffer();
						break;
					}

					dat.ReleaseBuffer();
				}
				else
					break;
			}
			else
				break;
				
		}while((int)nBytesRead);
		file.Close();
	}
	CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH
}

CString CMemo::loadMemoUsingTip(CString skey)
{
	CString result;
	result.Empty();

	if (skey.IsEmpty())
		return result;
	
	CString dir, dat;
	char key[12]{}, lBytes[4]{};
	
	dir.Format("%s\\%s\\%s\\memo.mmo", m_root, USER, m_name);
	CFileFind	finder;
	if (!finder.FindFile(dir))
		return result;
	
	TRY
	{ 
		CFile file(dir, CFile::modeRead);
		UINT nBytesRead;
		do
		{
			nBytesRead = file.Read(&key, sizeof(key));
			if (nBytesRead == sizeof(key))
			{
				nBytesRead = file.Read(&lBytes, sizeof(lBytes));
				if (nBytesRead == sizeof(lBytes))
				{
					int lSize = atoi(CString(lBytes, 4));
					nBytesRead = file.Read(dat.GetBufferSetLength(lSize), lSize);
					
					if ((int)nBytesRead != lSize)
					{
						dat.ReleaseBuffer();
						break;
					}
					
					if (!skey.Compare(key))
					{
						result = dat;
						dat.ReleaseBuffer();
						break;
					}
					
					dat.ReleaseBuffer();
				}
				else
					break;
			}
			else
				break;
			
		}while((int)nBytesRead);

		file.Close();
		return result;
	}
	CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH

		return result;
}
CPen* CMemo::getAxPen(COLORREF clr, int width, int style)
{
	_penR penR;
	penR.clr = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_sendW->SendMessage(WM_USER, getPEN, (long)&penR);
}

CBrush* CMemo::getAxBrush(COLORREF clr)
{
	return (CBrush*) m_sendW->SendMessage(WM_USER, getBRUSH, (long)clr);
}

CFont* CMemo::getAxFont(CString fName, int point, int style)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = false;
	fontR.bold = 0;
	switch(style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)m_sendW->SendMessage(WM_USER, getFONT, (long)&fontR);
}

void CMemo::OnDestroy() 
{
	CWnd::OnDestroy();

	if (m_edit != nullptr) 			
		m_edit->DestroyWindow();
}
