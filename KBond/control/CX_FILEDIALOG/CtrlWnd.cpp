// CtrlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_FILEDIALOG.h"
#include "CtrlWnd.h"
#include "../../h/axiscolor.h"
#include <algorithm>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _TRIMDATA(data) \
		data.TrimLeft(); \
		data.TrimRight();

#define TRKEY_LOAD	1
#define TRKEY_SAVE	2

#define FILE_OPEN	1
#define FILE_SAVE	2
#define FILE_SAVEAS	3
#define FILE_FOLDER	4

static int CALLBACK BrowseForFolder_CallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
   if(uMsg == BFFM_INITIALIZED)
       SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, (LPARAM)lpData);
   return 0;
} 

/////////////////////////////////////////////////////////////////////////////
// CCtrlWnd

CCtrlWnd::CCtrlWnd(CWnd* pWizard, _param* pParam)
	: m_pWizard(pWizard)
	, m_strImgPath(_T(""))
	, m_pBitmap(NULL)
	, m_pBitmap_dn(NULL)
	, m_pBitmap_en(NULL)
	, m_bCheck(FALSE)
	, m_bChecked(FALSE)
	, m_bCapture(FALSE)
	, m_bHOver(FALSE)
	, m_bTracking(FALSE)
	, m_strRoot(_T(""))
	, m_strName(_T(""))
	, m_strText(_T(""))	
	, m_nDialogMode(0)
	, m_strPathName(_T(""))
	, m_strFileName(_T(""))
	, m_strFileFilter(_T(""))
	, m_bEnable(TRUE)
{
	EnableAutomation();
	SetParam(pParam);
}

CCtrlWnd::~CCtrlWnd()
{

}

void CCtrlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CCtrlWnd, CWnd)
	//{{AFX_MSG_MAP(CCtrlWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CCtrlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CCtrlWnd)
	DISP_FUNCTION(CCtrlWnd, "SetMode", SetMode, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CCtrlWnd, "GetPath", GetPath, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CCtrlWnd, "SetFileName", SetFileName, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CCtrlWnd, "SetPath", SetPath, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CCtrlWnd, "Enable", Enable, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CCtrlWnd, "SetFileFilter", SetFileFilter, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ICtrlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {C22E2C6B-43EB-44A4-9221-01CB8A8A0D75}
static const IID IID_ICtrlWnd =
{ 0xc22e2c6b, 0x43eb, 0x44a4, { 0x92, 0x21, 0x1, 0xcb, 0x8a, 0x8a, 0xd, 0x75 } };

BEGIN_INTERFACE_MAP(CCtrlWnd, CWnd)
	INTERFACE_PART(CCtrlWnd, IID_ICtrlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCtrlWnd message handlers

BOOL CCtrlWnd::Initialize(BOOL bDLL)
{
	m_strRoot = Variant(homeCC);
	m_strName = Variant(nameCC);

	
	m_strFileFilter =  _T("Excel (*.xls)|*.xls|모든 파일 (*.*)|*.*||");
	SetButtonImages();

	m_clrBtnText =  GetIndexColor(CLR_BTNTEXT);
	return TRUE;
}


void CCtrlWnd::SetParam(_param* pParam)
{
	m_Param.key = pParam->key;
	m_Param.name = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB = pParam->tRGB;
	m_Param.pRGB = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	CString strTemp = m_Param.options;
	m_strImgPath = OptionParser(strTemp, "/i");

	if (!m_strImgPath.IsEmpty())
	{
		m_strImgPath.MakeLower();
		m_strImgPath.Replace(".bmp", "");
	}

	m_strText = OptionParser(strTemp, "/t");
}

CString CCtrlWnd::OptionParser(CString strOption, CString strKey)
{
	/*
	int nSPos = strOption.Find(strKey);

	if (nSPos == -1)
		return "";

	nSPos += 2;

	int nEPos = strOption.Find(_T("/"), nSPos);

	if (nEPos == -1)
		return "";

	return strOption.Mid(nSPos, nEPos - nSPos);
	*/
	CString tmp = strOption;
	Parser(tmp, strKey);
	tmp = Parser(tmp, "/");
	return tmp;
}

CString CCtrlWnd::Parser(CString &strSource, CString strSep)
{
	CString strTemp(_T(""));

	if (strSource.Find(strSep) == -1)
	{
		strTemp = strSource;
		strSource.Empty();
			return strTemp;
	}
	else
	{
		strTemp = strSource.Left(strSource.Find(strSep));
		strSource = strSource.Mid(strSource.Find(strSep) + strSep.GetLength());
		return strTemp;
	}

	return "";
}

void CCtrlWnd::SetButtonImages()
{
	CString strPath(_T(""));

	strPath.Format("%s\\image\\%s", m_strRoot, m_strImgPath);

	m_pBitmap = GetBitmap(strPath + ".bmp");
	m_pBitmap_dn = GetBitmap(strPath + "_dn.bmp");
	m_pBitmap_en = GetBitmap(strPath + "_en.bmp");
}

CBitmap* CCtrlWnd::GetBitmap(CString strPath)
{
	return (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (LPARAM)(LPCTSTR)strPath);
}

void CCtrlWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient;
	GetClientRect(&rcClient);

	dc.FillSolidRect(&rcClient, GetIndexColor(m_Param.pRGB));

	dc.SetBkMode(TRANSPARENT);

	CBitmap* pOldBitmap = NULL;
	CBitmap* pBitmap = NULL;
	BITMAP bi;
	UINT nStyle = DFCS_BUTTONPUSH;

	if (m_bCheck && m_bChecked && m_pBitmap_dn)
		pBitmap = m_pBitmap_dn;
	else if (m_bCapture && m_bHOver)
	{
		if (m_pBitmap_dn)
			pBitmap = m_pBitmap_dn;
		else
			nStyle |= DFCS_PUSHED;
	}
	else if (m_bHOver && m_pBitmap_en)
		pBitmap = m_pBitmap_en;
	else if (m_pBitmap)
		pBitmap = m_pBitmap;

	if (pBitmap)
	{
		CDC memDC;
		pBitmap->GetBitmap(&bi);

		memDC.CreateCompatibleDC(&dc);	

		if (memDC.m_hDC != NULL) 
			pOldBitmap = (CBitmap*)memDC.SelectObject(pBitmap);
		
		//[Warn4] 2010.02.11
		//int nX = (rcClient.Width() - bi.bmWidth) / 2;
		//int nY = (rcClient.Height() - bi.bmHeight) / 2;

		::TransparentBlt(dc.m_hDC, 0, 0, rcClient.Width(), rcClient.Height(), memDC.m_hDC, 0, 0, bi.bmWidth, bi.bmHeight, RGB(255, 0, 255));
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
	else
		dc.DrawFrameControl(&rcClient, DFC_BUTTON, nStyle);

	m_strText.TrimLeft();

	if (!m_strText.IsEmpty())
	{
		CFont* pFont = GetFont(m_Param.fonts, m_Param.point, m_Param.style);
		CFont* pOldFont = (CFont*)dc.SelectObject(pFont);
		
		if (IsWindowEnabled())
		{
			if(m_bEnable)
				dc.SetTextColor(m_clrBtnText);
			else
				dc.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		}
		else
			dc.SetTextColor(GetSysColor(COLOR_GRAYTEXT));

		

		/*
		if (pBitmap)
		{
			rcClient.right = bi.bmWidth;
			rcClient.bottom = bi.bmHeight;
		}
		*/
		
		if (m_bCapture && m_bHOver)
			rcClient.OffsetRect(1, 1);
		
		dc.DrawText(m_strText, &rcClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		dc.SelectObject(pOldFont);
	}

}

CString CCtrlWnd::Variant(int nComm, CString strData)// = _T("") )
{
	CString retValue;
	char* pData = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);

	if ((long)pData > 1)
		retValue = pData;
	else
		return "";

	return retValue;
}

COLORREF CCtrlWnd::GetIndexColor(int nIndex)
{
	return m_pWizard->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)nIndex);
}

CFont* CCtrlWnd::GetFont(CString strFontName, int nPoint, int style)
{

	struct _fontR fontR;
	fontR.name = (LPSTR)strFontName.operator LPCTSTR();
	fontR.point = nPoint;
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
	return (CFont*)m_pWizard->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CFont* CCtrlWnd::GetFont()
{
	return GetFont(_T("굴림"), 9, 0);
}

void CCtrlWnd::ShowFileDialog(CRect rc)
{
	ClientToScreen(&rc);

	//true:열기 false:읽기
	BOOL bOk = FALSE;
	CString strTmpPath = _T("");
	char szFilter[] = "All Files(*.*)|*.*||";

	if(m_nDialogMode == 0)
		return;


/*
fileDlg.m_ofn.lpstrTitle = " 저장할 이름을 선택하세요."; // 파일댜열로그 제목 
fileDlg.m_ofn.lpstrInitialDir = "초기디렉토리"; // 
fileDlg.m_ofn.lpstrFilter = "모든파일( *.* )\0*.*\0\0"; // 파일 필터 
*/
	switch(m_nDialogMode)
	{
	case FILE_OPEN:
		{
			CFileDialog dlg(TRUE, NULL, m_strFileName, OFN_HIDEREADONLY, (LPSTR)(LPCTSTR)m_strFileFilter);		
			//dlg.m_ofn.lpstrTitle = (LPSTR)(LPCTSTR)m_strFileFilter;
			dlg.m_ofn.lpstrInitialDir  = m_strPathName;
			if(IDOK == dlg.DoModal())	
			{
				strTmpPath = dlg.GetPathName();			
				bOk = TRUE;
			}
		}
		break;
	case FILE_SAVE:
		{
			CFileDialog dlg(FALSE, NULL, m_strFileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, (LPSTR)(LPCTSTR)m_strFileFilter);
			dlg.m_ofn.lpstrTitle = "저장하기"; // 파일댜열로그 제목 
			dlg.m_ofn.lpstrInitialDir  = m_strPathName;
			if(IDOK == dlg.DoModal())	
			{
				strTmpPath = dlg.GetPathName();
				bOk = TRUE;
			}		
		}
		break;
	case FILE_SAVEAS:
		{
			CFileDialog dlg(FALSE, NULL, m_strFileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, (LPSTR)(LPCTSTR)m_strFileFilter);
			dlg.m_ofn.lpstrInitialDir  = m_strPathName;			
			if(IDOK == dlg.DoModal())	
			{
				strTmpPath = dlg.GetPathName();
				bOk = TRUE;
			}
		}
		break;
	case FILE_FOLDER:
		{
			ITEMIDLIST *pidlBrowse;   
			char pszPathname[_MAX_PATH];  
			BROWSEINFO BrInfo;    
			BrInfo.hwndOwner = GetSafeHwnd();  
			BrInfo.pidlRoot = NULL; 
			memset(&BrInfo, 0, sizeof(BrInfo)); 
			BrInfo.pszDisplayName = pszPathname; 
			BrInfo.lpszTitle = "경로 선택 :";  
			BrInfo.ulFlags = BIF_RETURNONLYFSDIRS; 			
			BrInfo.lpfn = BrowseForFolder_CallbackProc;
			BrInfo.lParam =  (LPARAM)(LPCSTR)m_strPathName;

			pidlBrowse = ::SHBrowseForFolder(&BrInfo);  

			if( pidlBrowse != NULL)	
			{
				if(::SHGetPathFromIDList(pidlBrowse, pszPathname))
				{
					strTmpPath.Format("%s", pszPathname);			
					bOk = TRUE;
				}
			}			
		}
		break;
	}


	if(bOk)
	{
		m_strPathName = strTmpPath;
		m_pWizard->PostMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange)), (long)m_Param.name.operator LPCTSTR());
	}
}


void CCtrlWnd::OnMouseMove(UINT nFlags, CPoint point) 
{	
	if (!m_bTracking && m_bEnable)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CCtrlWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_bCapture)
		ReleaseCapture();
	SetCapture();
	m_bCapture = TRUE;
	Invalidate();
	
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CCtrlWnd::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	// TODO: Add your message handler code here and/or call default
	m_bHOver = TRUE;
	Invalidate();
}


LRESULT CCtrlWnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHOver=FALSE;
	Invalidate();
	return 0;
}

void CCtrlWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bCapture)
		ReleaseCapture();
	SetCapture();
	m_bCapture = TRUE;
	Invalidate();
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CCtrlWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bCapture)
	{
		CRect rcClient;
		m_bCapture = FALSE;
		ReleaseCapture();
		GetClientRect(&rcClient);
		if (rcClient.PtInRect(point))
		{
			if (m_bCheck)
				m_bChecked = !m_bChecked;


			if(m_bEnable)
				ShowFileDialog(rcClient);			
		}
		Invalidate();
	}

	CWnd::OnLButtonUp(nFlags, point);
}


long CCtrlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	//[Warn4] 2010.02.11
	//struct _extTH *thInfo = NULL;
	//int key = 0;
	//int nLen = 0;

	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_INIT:
		break;
	case DLL_OUB:
		break;		
	case DLL_TRIGGER:		
		break;
	}

	return 0;
}


void CCtrlWnd::SetMode(short nMode) 
{
	// TODO: Add your dispatch handler code here
	m_nDialogMode = nMode;
}


BSTR CCtrlWnd::GetPath() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	strResult = m_strPathName;
	return strResult.AllocSysString();
}

void CCtrlWnd::SetFileName(LPCTSTR strFileName) 
{
	// TODO: Add your dispatch handler code here
	m_strFileName = strFileName;
}

void CCtrlWnd::SetPath(LPCTSTR strPath) 
{
	// TODO: Add your dispatch handler code here
	m_strPathName = strPath;
}

void CCtrlWnd::Enable(BOOL bEnable) 
{
	// TODO: Add your dispatch handler code here
	m_bEnable = bEnable;
	Invalidate();
}

void CCtrlWnd::SetFileFilter(LPCTSTR strFilter) 
{
	// TODO: Add your dispatch handler code here
	m_strFileFilter = strFilter;
}
