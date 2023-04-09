// fxGrid.cpp : implementation file
//

#include "stdafx.h"
#include "intGrid.h"
#include "memdc.h"
#include "excel9.h"


#include "InPlaceEdit.h"
#include "InPlaceCombo.h"
#include "InPlaceDate.h"
#include "sharemsg.h"
#include <afxadv.h>		// OLE stuff for clipboard operations
#include <afxconv.h>
#include <mmsystem.h>
#include "GridWnd.h"
#include "MainWnd.h"

#include "../../h/userdecrypt.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	IDC_INPLACE_EDIT	8
#define	IDC_INPLACE_COMBO	9
#define	IDC_INPLACE_DATE	10
#define	IDC_INPLACE_TIME	11


#define IDC_HISTORY	1000
const int indicatorW = 11;

COLORREF infoCOLOR = PALETTERGB(128, 128, 128);
COLORREF memoCOLOR = PALETTERGB( 68, 225, 115);

const int codeCOL = 0, nameCOL = colNAME;
#define	CCOD		"1301"
#define	HCOD		"1301"



#define	GRIDCTRL_CLASSNAME	"AxisGrid"

#define	press_SHIFTKEY()	((GetKeyState(VK_SHIFT)   & (1 << (sizeof(SHORT)*8-1))) != 0)
#define	press_CTRLKEY()		((GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0)

const int tmRTM		= 200;
const int tmINTERVAL	= 200;		// drag & drop
const int tmINTERVALx	= 500;		// column move
const int rszCAPTURE	= 3;
const int selcWEIGHT	= 600;
const int tmAUTOSCRL	= 60;

const int hdrHEIGHT	= 2;
const int footerHEIGHT	= 2;
const int leftMARGIN	= 4;
const int rightMARGIN	= 4;
const int prnGAP	= 1;

#define	ARR_UP		"��"
#define	ARR_DN		"��"
#define	TRI_UP		"��"
#define	TRI_DN		"��"

const COLORREF upCOLOR    = PALETTERGB(255,   0,   0);
const COLORREF dnCOLOR    = PALETTERGB(  0,   0, 255);
const COLORREF eqCOLOR    = PALETTERGB(  0,   0,   0);

[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CintGrid, CWnd)

// Market Type
#define MarketDefault			0
#define MarketStock				1
#define MarketIndex				2
#define MarketFuture			3
#define MarketOption			4
#define MarketELW				5
#define MarketForeign			6
#define MarketInvestor			7
#define MarketStockOption		8
#define MarketProgram			9
#define MarketInvestorForeign	10

#define CODELEN_STOCK		6
#define CODELEN_INDEX		3
#define CODELEN_FUTOPT		8
#define CODELEN_FOREIGN		6
#define CODELEN_INVESTOR	6

CDropData CintGrid::m_dropdata;

void AFXAPI DDX_GridControl(CDataExchange* pDX, int nIDC, CintGrid& rControl)
{
	if (rControl.GetSafeHwnd() == nullptr)    // not subclassed yet
	{
		ASSERT(!pDX->m_bSaveAndValidate);
		HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
		if (!rControl.SubclassWindow(hWndCtrl))
		{
			ASSERT(FALSE);      // possibly trying to subclass twice?
			AfxThrowNotSupportedException();
		}
		else
		{
			// If the control has reparented itself (e.g., invisible control),
			// make sure that the CWnd gets properly wired to its control site.
			if (pDX->m_pDlgWnd->GetSafeHwnd() != ::GetParent(rControl.GetSafeHwnd()))
				rControl.AttachControlSite(pDX->m_pDlgWnd);
		}
	}
}

UINT GetMouseScrollLines()
{
	HKEY hKey{};
	int scrollLines = 3;            // reasonable default

	if (RegOpenKeyEx(HKEY_CURRENT_USER,  _T("Control Panel\\Desktop"),
				0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
		TCHAR szData[128]{};
		DWORD dwKeyDataType{};
		DWORD dwDataBufSize = sizeof(szData);

		if (RegQueryValueEx(hKey, _T("WheelScrollLines"), nullptr, &dwKeyDataType,
					(LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
		{
			scrollLines = _tcstoul(szData, nullptr, 10);
		}
		RegCloseKey(hKey);
	}

	return scrollLines;
}

/////////////////////////////////////////////////////////////////////////////
// CintGrid
CintGrid::CintGrid(CWnd* pMainWnd, CWnd* pParent, LOGFONT* logfont /*nullptr*/)
{
	RegisterWindowClass();
	m_pMainWnd = pMainWnd;
	m_pParent = (CGridWnd*)pParent;		//�θ� ������	
	m_oleInit = false;
	m_pTooltip = nullptr;
	m_pToolChart = nullptr;
	m_pToolNews = nullptr;

	const _AFX_THREAD_STATE* pState = AfxGetThreadState();
	if (!pState->m_bNeedTerm)
	{
		const SCODE sc = ::OleInitialize(nullptr);
		if (FAILED(sc))
			AfxMessageBox(_T("OLE initialization failed. Make sure that the OLE libraries are the correct version"));
		else
			m_oleInit = true;
	}

	m_winfgCOL  = ::GetSysColor(COLOR_WINDOWTEXT);
	m_3dfaceCOL = ::GetSysColor(COLOR_3DFACE);
	m_shadowCOL = ::GetSysColor(COLOR_3DSHADOW);

	m_fgSELCOL = PALETTERGB(255, 255, 255);
	m_bkSELCOL = PALETTERGB(128, 128, 255);

	m_nRows = m_nFixedRows = m_nCols = m_nFixedCols = 0;
	m_Vscroll = m_Hscroll = 0;
	m_defHeight = 0;
	m_defWidth  = 0;
	m_margin = 0;
	m_rowCountWheel = GetMouseScrollLines();
		
	m_mousemode = mouseNOTHING;

	m_gridCOL   = ::GetSysColor(COLOR_3DDKSHADOW);
	m_gridLine  = GVLN_BOTH;
	m_gridLineType = GVLT_SOLID;

	m_focusLine = FALSE;
	m_selectLine = TRUE;
	m_dragdrop  = GVDD_NONE;
	m_enabletip = FALSE;
	m_handletabkey = TRUE;
	m_useOLE = FALSE;
	m_sensitive = FALSE;

	m_enableRowResize = FALSE;
	m_enableColResize = FALSE;

	m_ascending  = FALSE;
	m_sortColumn = -1;

	m_timerID   = 1;
	m_imageList = nullptr;
	m_NOscroll  = GVSC_BOTH;
	m_DIscroll  = false;

	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));    
	CopyMemory(&m_logfont, &(ncm.lfMessageFont), sizeof(LOGFONT));
	m_logfont.lfHeight *= 10;

	if (logfont != nullptr)
		CopyMemory(&m_logfont, logfont, sizeof(LOGFONT));

	m_fgCOL    = m_winfgCOL;
	m_bkCOL	   = m_shadowCOL;
	m_fixfgCOL = m_winfgCOL;
	m_fixbkCOL = m_3dfaceCOL;
	m_upCOL    = upCOLOR;
	m_dnCOL	   = dnCOLOR;
	m_eqCOL    = eqCOLOR;

	m_stepUnit = 0;
	m_stepCOL1 = m_bkCOL;
	m_stepCOL2 = m_bkCOL;

	m_mapSelCell.RemoveAll();
	m_mapPreCell.RemoveAll();

	m_mapRTM.RemoveAll();

	m_noMessage = false;

	m_excelS = _T("");
	m_excelH = nullptr;

	m_keepcolumn = 0;
	m_enableMouseTrace = false;
	m_baseFixed = 0;
	m_dragWnd = nullptr;

	m_memoAry.RemoveAll();
	m_memoWnd = nullptr;
	m_updateRow = -1;

	for (int ii = 0; ii < MAX_LINE; ii++)
		m_memoAry.Add(false);

	m_history = nullptr;
	m_memoRGN.CreateRectRgn(0, 0, 0, 0);

	// ADD PSH 20070911
	m_bHeadSet		= TRUE;
	m_bEnSelect		= TRUE;
	m_nVirHScroll	= 0;
	m_nVirHPos		= 0;
	m_bVirHPos		= FALSE;
	m_nBeforeScrollPos = 0;

	m_clrBkMarkerColor	= RGB(240, 240, 240);
	m_clrMarkerColor	= RGB(142, 142, 142);
	m_bMkShadow			= TRUE;
	m_pBmpMarker		= nullptr;

	m_bInfo				= FALSE;
	m_pBmpInfo1			= nullptr;
	m_pBmpInfo2			= nullptr;
	m_pBmpInfo3			= nullptr;
	m_pBmpInfo4			= nullptr;
	m_pBmpInfo5			= nullptr;
	m_pBmpInfo6			= nullptr;
	m_pBmpInfo7			= nullptr;
	m_pBmpInfo8			= nullptr;
	m_pBmpInfo9			= nullptr;
	m_pBmpInfo10		= nullptr;
	m_pBmpInfo11		= nullptr;
	m_pBmpInfo12		= nullptr;	//2012.06.20 KSJ ���
	m_pBmpInfo13		= nullptr;	//2012.06.20 KSJ ����
	m_pBmpInfo14		= nullptr;	//2012.10.31 KSJ �ܱ����
	m_pBmpInfo15		= nullptr;	//2013.03.18 KSJ ���迹��
	m_pBmpInfo16		= nullptr;	//2013.03.18 KSJ �Ǹ�
	m_pBmpInfo17		= nullptr;	//2013.03.18 KSJ ���
	m_pBmpInfo18		= nullptr;	//2013.03.18 KSJ �׸�
	m_pBmpInfo19		= nullptr;	//2013.03.18 KSJ ����
	m_pBmpInfo20		= nullptr;	//2013.03.18 KSJ �պ�
	m_pBmpInfo21		= nullptr;	//2013.03.18 KSJ ����
	m_pBmpInfo22		= nullptr;	//2013.03.18 KSJ ����
	m_pBmpInfo23		= nullptr;	//2016.06.14 KSJ ����������
	m_pBmpExpect		= nullptr;
	// END ADD

	m_isLbtnClick = FALSE;				//Ŭ������ 091013 ����

	m_nAlmRow = -1;
	m_bOutPos = false;

	hScrolltoleft = 0;						//���ν�ũ�� �̺�Ʈ�߻��� ��ġ��  

	for(int k=0;k<MAX_EQUALIZER;k++)
	{
		for(int l=0;l<MAX_EQUALIZER;l++)
		{
			m_eq_levs[k][l] = 0;
		}
	}

	m_isEqualizerField = false;
	m_originRect.SetRectEmpty();
	
	m_xRate = 1;
	m_yRate = 1;
	m_point = 9;

	m_bPaintLock = FALSE;
	m_bDrag = FALSE;
}

CintGrid::~CintGrid()
{
	if (m_excelH != nullptr)
	{
		m_excelS = _T(""); 
		Sleep(1000);
		TerminateThread(m_excelH, 0); 
		Sleep(50);
	}

	for(int n=0; n<3; ++n)
		eq_bmp[n].DeleteObject();

	m_mapMerge.RemoveAll();
	m_mapRTM.RemoveAll();

	DeleteAllItems();
	DestroyWindow();
	m_font.DeleteObject();

	if (m_oleInit) ::OleUninitialize();
	m_memoRGN.DeleteObject();

	if(m_pBmpMarker)
		m_pBmpMarker->DeleteObject();
	if(m_pBmpExpect)
		m_pBmpExpect->DeleteObject();
	if(m_pBmpInfo1)
		m_pBmpInfo1->DeleteObject();
	if(m_pBmpInfo2)
		m_pBmpInfo2->DeleteObject();
	if(m_pBmpInfo3)
		m_pBmpInfo3->DeleteObject();
	if(m_pBmpInfo4)
		m_pBmpInfo4->DeleteObject();
	if(m_pBmpInfo5)
		m_pBmpInfo5->DeleteObject();
	if(m_pBmpInfo6)
		m_pBmpInfo6->DeleteObject();
	if(m_pBmpInfo7)
		m_pBmpInfo7->DeleteObject();
	if(m_pBmpInfo8)
		m_pBmpInfo8->DeleteObject();
	if(m_pBmpInfo9)
		m_pBmpInfo9->DeleteObject();
	if(m_pBmpInfo10)
		m_pBmpInfo10->DeleteObject();
	if(m_pBmpInfo11)
		m_pBmpInfo11->DeleteObject();
	//2012.06.20 KSJ ���, ���� �߰�
	if(m_pBmpInfo12)
		m_pBmpInfo12->DeleteObject();
	if(m_pBmpInfo13)
		m_pBmpInfo13->DeleteObject();
	if(m_pBmpInfo14)
		m_pBmpInfo14->DeleteObject();
	if(m_pBmpInfo15)
		m_pBmpInfo15->DeleteObject();
	if(m_pBmpInfo16)
		m_pBmpInfo16->DeleteObject();
	if(m_pBmpInfo17)
		m_pBmpInfo17->DeleteObject();
	if(m_pBmpInfo18)
		m_pBmpInfo18->DeleteObject();
	if(m_pBmpInfo19)
		m_pBmpInfo19->DeleteObject();
	if(m_pBmpInfo20)
		m_pBmpInfo20->DeleteObject();
	if(m_pBmpInfo21)
		m_pBmpInfo21->DeleteObject();
	if(m_pBmpInfo22)
		m_pBmpInfo22->DeleteObject();
	if(m_pBmpInfo23)
		m_pBmpInfo23->DeleteObject();
	//KSJ
}

BOOL CintGrid::RegisterWindowClass()
{
	WNDCLASS wndcls{};
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, GRIDCTRL_CLASSNAME, &wndcls)))
	{
		wndcls.style            = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wndcls.lpfnWndProc      = ::DefWindowProc;
		wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
		wndcls.hInstance        = hInst;
		wndcls.hIcon            = nullptr;
		wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground    = (HBRUSH) nullptr;
		wndcls.lpszMenuName     = nullptr;
		wndcls.lpszClassName    = GRIDCTRL_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

CWnd* CintGrid::Create(const RECT& rect, CWnd* parent, UINT nID, int NOscroll, int dragdrop, DWORD dwStyle)
{
	ASSERT(parent->GetSafeHwnd());

	if (!CWnd::Create(GRIDCTRL_CLASSNAME, nullptr, dwStyle, rect, parent, nID)) 
		return nullptr;

	m_dragdrop = (int) dragdrop;
	if (m_dragdrop != GVDD_NONE) m_dropTarget.Register(this);

	m_aryRowHeight.resize(m_nRows);
	m_aryColWidth.resize(m_nCols);

	//TRACE(" ROW HEIGHT %d ====\n",m_defHeight);
	for (int ii = 0; ii < m_nRows; ii++)
		m_aryRowHeight[ii] = m_defHeight;
	for (int ii = 0; ii < m_nCols; ii++)
		m_aryColWidth[ii] = m_defWidth;

	m_NOscroll = NOscroll;

	ResetScrollBars();
	
	CString	fileIMG;
	fileIMG.Format("%s\\%s\\%s", m_root, IMAGEDIR,"��02.bmp");
	
	HBITMAP hBitmap = (HBITMAP)::LoadImage(nullptr, fileIMG.GetBuffer(0),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	m_pBmpExpect = CBitmap::FromHandle(hBitmap);

	return this;
}


void CintGrid::loadEqualizer()
{
	const int def_vols[] = {0,20,40,60,80};
	const int def_levs[] = {500, 1000, 3000, 5000, 10000};
	const int levs[7][5] = {{500, 2500, 5000, 10000, 30000},
					{300, 100, 3000, 5000, 10000},
					{100,500,1000,3000,5000},
					{50,250,500,1000,2500},
					{25,100,250,500,1000},
					{10,50,100,300,500},
					{5,25,50,75,100}};	

	int n{};
	CString path, key;
	const char *eq_section = "EQUALIZER";
	char* buff{}, fname[MAX_PATH]{};
	CString m_name;
	
	buff = (char*)m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), (LPARAM)0);
	m_name = (char*)m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), (LPARAM)0);
//	m_name = CAxisProfile::GetHashValue(m_name);

	path.Format("%s\\user\\%s\\%s.ini", buff,  m_name, m_name);
	
	m_eq_cnt = 5;
	m_eq_type = max(1, GetPrivateProfileInt(eq_section, "TYPE", 1, path));
	
	int k=4;
	for(n=0; n<5; ++n)
	{
// 		key.Format("VOL%02d", n+1);
// 		m_eq_vol[n] = GetPrivateProfileInt(eq_section, key, def_vols[n], path);
		m_eq_vol[n] = def_vols[n];

		key.Format("LEV%02d", n+1);
		m_eq_lev[n] = GetPrivateProfileInt(eq_section, key, def_levs[n], path);
	

		key.Format("DCOL%02d", n+1);
		m_eq_mdcol[k] = GetPrivateProfileInt(eq_section, key, RGB(0xE0-(n*48), 0xE0-(n*48), 0xFF), path);
		
		key.Format("SCOL%02d", n+1);
		m_eq_mscol[k] = GetPrivateProfileInt(eq_section, key, RGB(0xFF, 0xE0-(n*48), 0xE0-(n*48)), path);
		
		m_eq_nmcol[k] = RGB(0xE0,0xE0,0xE0);
		k--;
	}


	//ü����� 
	for(n=0 ;n<7 ; n++)
	{
		for(int nn=0 ; nn<5 ; nn++)
		{
			m_eq_levs[n][nn] = levs[n][nn];
		}
	}

	for(n=4; n>=0; --n)
	{
		if (n==0)
			m_eq_vol[n] = (20 - m_eq_vol[n]);
		else
			m_eq_vol[n] = (m_eq_vol[n]-m_eq_vol[n-1]);
	}
	
	m_eq_alarm[0] = GetPrivateProfileInt(eq_section, "MADO_ALARM", 0, path);
	m_eq_alarm[1] = GetPrivateProfileInt(eq_section, "MASU_ALARM", 0, path);
	m_eq_alarm_lev[0] = GetPrivateProfileInt(eq_section, "MADO_ALARM_LEV", 0, path);
	m_eq_alarm_lev[1] = GetPrivateProfileInt(eq_section, "MASU_ALARM_LEV", 0, path);
	GetPrivateProfileString(eq_section, "MADO_ALARM_WAV", "", fname, sizeof(fname), path);
	m_eq_alarm_wav[0] = fname;
	GetPrivateProfileString(eq_section, "MASU_ALARM_WAV", "", fname, sizeof(fname), path);
	m_eq_alarm_wav[1] = fname;
	

	MakeEqualizerBmp();
}

void CintGrid::MakeEqualizerBmp()
{
	CDC *pDC = GetDC();
	const int rows = GetRowCount();
	const int col = m_pParent->GetEqualizerField();
	CRect rect;
	

	for(int ii=1 ; ii<rows ; ii++)
	{
		GetCellRect(ii, col, rect);
	
		int i{}, n{}, w{}, h{}, x{}, sx{};
		double rate{};

		w = rect.Width();
		h = rect.Height();
		
		for(i=0; i<3; ++i)
		{
			CBitmap* pBmp{};
			CPen Pen, * pPen{};
			CDC memDC;
			
			eq_bmp[i].DeleteObject();
			eq_bmp[i].CreateCompatibleBitmap(pDC, w, h);
			memDC.CreateCompatibleDC(pDC);
			
			pBmp = memDC.SelectObject(&eq_bmp[i]);
			
			Pen.CreatePen(PS_SOLID, 1, RGB(238,238,238));
			pPen  = memDC.SelectObject(&Pen);
			
			memDC.FillSolidRect(0, 0, w, h, RGB(255, 255, 255)); 
// 			TRACE("%d,%d,%d,%d\n",rect.top, rect.left, rect.bottom, rect.right);
			memDC.MoveTo(0, h-3);
			memDC.LineTo(w, h-3);
			memDC.MoveTo(0, 3);
			memDC.LineTo(w, 3);
			
			x = 0;
			rate = w/100.0f;
			
			for(n=0; n<m_eq_cnt; ++n)
			{
				sx = (int)(rate * m_eq_vol[n]);
				switch(i)
				{
				case 0: memDC.FillSolidRect(x, 3, sx, h-6, m_eq_mdcol[m_eq_cnt-n-1] ); break;
				case 1: memDC.FillSolidRect(x, 3, sx, h-6, m_eq_mscol[m_eq_cnt-n-1] ); break;
				}
				x += sx;
			}
			
			for(n=w-5; n>0; n-=5) 
			{
 				memDC.MoveTo(n, 3);
 				memDC.LineTo(n, h-3);
			}

			memDC.SelectObject(pPen);
			memDC.SelectObject(pBmp);
			Pen.DeleteObject();
			pBmp->DeleteObject();
			ReleaseDC(&memDC);
			memDC.DeleteDC();
			
			eq_bmp[i].DeleteObject();
		}
	}

	ReleaseDC(pDC);
}

int	CintGrid::SearchCurrValStep(double curr)
{
	//�˻�
	int result = 0;
	
	if(curr>0 && curr < 1000)
		result = 0;
	else if(curr>1000 && curr <5000)
		result = 1;
	else if(curr>5000 && curr <10000)
		result = 2;
	else if(curr>10000 && curr < 50000)
		result = 3;
	else if(curr>50000 && curr < 100000)
		result = 4;
	else if(curr>100000 && curr < 500000)
		result = 5;
	else if(curr>500000)
		result = 6;

	return result;
}


bool CintGrid::Initial(int nRow, int nCol, int fixRow, int fixCol)
{
	ClearAllRTM(); 
	ClearMerge();

	// MODIFY PSH 20070918
	//SetScrollPos32(SB_VERT, 0);
	if (m_NOscroll & GVSC_HORZ)
	{
		SetScrollPos32(SB_HORZ, 0);
	}

	if (m_NOscroll & GVSC_VERT)
	{
		SetScrollPos32(SB_VERT, 0);
	}
	// END MODIFY

	SetRowCount(nRow);
	SetColumnCount(nCol);
	SetFixedRowCount(fixRow);
	SetFixedColumnCount(fixCol);

	

	return true;
}

void CintGrid::Refresh(bool NOselect)
{
	if (NOselect)
		ResetSelectedRange();
	UpdateWindow();		// updateX_20050531
}

void CintGrid::Adjust()
{
	ResetScrollBars(); 
	Invalidate();
}

void CintGrid::Clear(int sidxRow, int sidxCol)
{
	if (sidxRow < 0) sidxRow = m_nFixedRows;
	if (sidxCol < 0) sidxCol = 0;

	ClearAllRTM(); 
	ResetSelectedRange();
	memoClear();
	for (int row = sidxRow; row < m_nRows; row++) 
	{
		for (int col = sidxCol; col < m_nCols; col++)
		{
			SetItemState(row, col, 0); SetItemText (row, col, "");
			SetItemData(row, col ,0);
			if (IsCellAttribute(CIdCell(row, col), GVAT_CHECK))
				SetItemParam(row, col, 0);
		}
	}
	m_idCurrent.row = m_idCurrent.col = -1;
	m_nAlmRow = -1;//BUFFET

	if (GetSafeHwnd())
	{
		ResetScrollBars();
		Invalidate();
	}
}

BOOL CintGrid::Sort(int nCol, bool ascending)
{
	if (nCol < 0 || nCol > m_nCols)
		return FALSE;

	m_ascending = ascending;
	m_sortColumn = nCol;

	return SortTextItems(nCol, ascending);
}

// ADD PSH 20070912
void CintGrid::SortBase(CMapStringToString *pMap /* = nullptr */)
{
	m_ascending  = FALSE;
	m_sortColumn = -1;

	if (nullptr == pMap) 
	{
		Invalidate();
		return;
	}

	ClearAllRTM(); ResetSelectedRange();

	int nBaseRow	= -1;
	const int nRowCnt = pMap->GetCount();
	const int nColCnt = GetColumnCount();

	xROWS	  pTempRow;
	int	  nTempRowHeight = 0;
	
	CString strCode, strVal;

	for (int nRow = 0; nRow < nRowCnt;)
	{
		if (nullptr == pTempRow)
		{
			nRow++;
			strCode = GetItemText(nRow, colCODE);
		}
		else if (pTempRow)
		{
			strCode = pTempRow->at(colCODE)->text;
		}

		if (pMap->Lookup(strCode, strVal))
		{
			nBaseRow = atoi(strVal.GetBuffer(0));

			if (nullptr == pTempRow)
			{
				if (nBaseRow == nRow) continue;

				pTempRow	= m_aryGridRows[nBaseRow];
				nTempRowHeight	= m_aryRowHeight[nBaseRow];

				m_aryGridRows[nBaseRow]		= m_aryGridRows[nRow];
				m_aryRowHeight[nBaseRow]	= m_aryRowHeight[nRow];

				m_aryGridRows[nRow]	= nullptr;
			}
			else
			{
				auto pRow = m_aryGridRows[nBaseRow];

				if (pRow == nullptr)
				{
					m_aryGridRows[nBaseRow]		= pTempRow;
					m_aryRowHeight[nBaseRow]	= nTempRowHeight;

					pTempRow		= nullptr;
					nTempRowHeight	= 0;
				}
				else
				{
					const int nRowHeight = m_aryRowHeight[nBaseRow];

					m_aryGridRows[nBaseRow]		= pTempRow;
					m_aryRowHeight[nBaseRow]	= nTempRowHeight;

					pTempRow		= pRow;
					nTempRowHeight	= nRowHeight;
				}
			}
		}
	}

	MessageToParent(m_sortColumn, 0, GVNM_ENDSORT);
	Invalidate();
}
// END ADD


void CintGrid::SortBase(const CMap <CString, LPCTSTR, int, int>& pBaseMap, CMap <CString, LPCTSTR, int, int>& pCurMap)
{
	m_ascending  = FALSE;
	m_sortColumn = -1;

	ClearAllRTM(); 
	ResetSelectedRange();

	int nBaseRow	= -1;
	const int nRowCnt = pBaseMap.GetCount();
	const int nColCnt = GetColumnCount();

	xROWS	pTempRow;
	int	nTempRowHeight = 0;
	
	CString strCode;
/*
	for (int ii = 1; ii < m_nRows; ii++)
	{
		pTempRow = m_aryGridRows[ii];
		strCode = pTempRow->GetAt(colCODE)->text;  
		if(!strCode.IsEmpty()) 
			pCurMap.SetAt(strCode, ii);
	}
*/
	int nRow = 0;
	for (POSITION pos=pBaseMap.GetStartPosition(); pos != nullptr;)
	{
		strCode = _T("");
		pBaseMap.GetNextAssoc( pos, strCode, nBaseRow);

		//2013.01.24 KSJ nBaseRow�� m_aryGridRows.GetSize()���� ũ�ų� ������ �������Ƿ� �ɷ��ش�.
		if(nBaseRow >= gsl::narrow_cast<int>(m_aryGridRows.size()))
			continue;
		//2013.01.24 KSJ END

		if(pos)
		{
			if( pCurMap.Lookup(strCode, nRow) )
			{
				pTempRow = m_aryGridRows[nBaseRow];
				nTempRowHeight	= m_aryRowHeight[nBaseRow];

				m_aryGridRows[nBaseRow]	= m_aryGridRows[nRow];
				m_aryRowHeight[nBaseRow] = m_aryRowHeight[nRow];

				m_aryGridRows[nRow]	= pTempRow;
				m_aryRowHeight[nRow] = nTempRowHeight;
				strCode = pTempRow->at(colCODE)->text;
				pCurMap.SetAt(strCode, nRow);  
			}
		}
	}

	MessageToParent(m_sortColumn, 0, GVNM_ENDSORT);
	Invalidate();
}

void CintGrid::Blink(int nRow, int nCol, COLORREF color, int interval)
{
	if (nRow < 0 || nRow > m_nRows) return;
	if (nCol < 0 || nCol > m_nCols) return;

	COLORREF  bkcol{};
	UINT idEvent = 0;
	struct _timerID* timerID = (struct _timerID*)&idEvent;
	timerID->kind = 0;
	timerID->row = nRow;
	timerID->col = nCol;
	if (m_mapRTM.Lookup(idEvent, bkcol))
	{
		KillTimer(idEvent);
		SetTimer(idEvent, interval, nullptr);
		return;
	}

	m_sync.Lock();
	bkcol = GetItemBkColor(nRow, nCol);
	m_mapRTM.SetAt(idEvent, bkcol);
	m_sync.Unlock();

	SetItemBkColor(nRow, nCol, color, true);
	SetTimer(idEvent, interval, nullptr);

}

// ADD PSH 20070912
void CintGrid::SetMarginRatio(BOOL bMargin /* = TRUE */, BOOL bErase /* = TRUE */)
{ 
	m_bMargin = bMargin; 

	if (bErase)
	{
		const int nFixedColWidth  = GetFixedColumnWidth();

		CRect rcWnd;
		GetWindowRect(rcWnd);
		ScreenToClient(rcWnd);
		rcWnd.right = nFixedColWidth;

		InvalidateRect(rcWnd);
	}
}

void CintGrid::SetInfo(BOOL bInfo /* = TRUE */, BOOL bErase /* = TRUE */)
{ 
	m_bInfo = bInfo; 

	if (bErase)
	{
		const int nFixedColWidth  = GetFixedColumnWidth();

		CRect rcWnd;
		GetWindowRect(rcWnd);
		ScreenToClient(rcWnd);
		rcWnd.right = nFixedColWidth;

		InvalidateRect(rcWnd);
	}
}
// END ADD

BOOL CintGrid::GetCellSize(int fromRow, int endRow, int& colW, int& rowH)
{
	if (fromRow < 0 || endRow > GetRowCount())
		return FALSE;

	rowH = colW = 0;

	for (int ii = fromRow; ii < endRow; ii++)
		rowH += GetRowHeight(ii);
	for (int ii = 0; ii < GetColumnCount(); ii++)
		colW += GetColumnWidth(ii);

	rowH += 1; colW += 1;
	return TRUE;
}

void CintGrid::GetCellFromPoint(CPoint pt, int& nRow, int& nCol)
{
	CIdCell cell = GetCellFromPoint(pt);
	nRow = cell.row;
	nCol = cell.col;

	return;
}

void CintGrid::GetTopLeftNonFixedCell(int& nRow, int& nCol)
{
	CIdCell idCell = GetTopleftNonFixedCell();

	nRow = idCell.row;
	nCol = idCell.col;

	return;
}

void CintGrid::GetSelectedCellRange(int& minRow, int& maxRow, int& minCol, int& maxCol)
{
	const CRangeCell selection = GetSelectedCellRange();

	minRow = selection.GetMinRow();
	maxRow = selection.GetMaxRow();
	minCol = selection.GetMinCol();
	maxCol = selection.GetMaxCol();

	return;
}

void CintGrid::ClearAllRTM()
{
	COLORREF bkcol{};
	int key{}, row{}, col{};

	m_sync.Lock();
	for (POSITION pos = m_mapRTM.GetStartPosition(); pos; )
	{
		m_mapRTM.GetNextAssoc(pos, key, bkcol);
		KillTimer(key);

		const struct _timerID* timerID = (struct _timerID*)&key;
		row = timerID->row;
		col = timerID->col;

		SetItemBkColor(row, col, bkcol, true);
	}
	m_mapRTM.RemoveAll();
	m_sync.Unlock();
}

LONG CintGrid::MessageToGrid(UINT msg, int scode)
{
	m_noMessage = true;
	const LONG response = SendMessage(msg, MAKEWPARAM(scode, SB_THUMBPOSITION), 0);
	m_noMessage = false;
	
	return response;
}

UINT RunExcelThread(LPVOID lpvoid)
{
	CintGrid* fxGrid = (CintGrid *) lpvoid;

	fxGrid->m_syncx.Lock();
	fxGrid->RunExcel_();
	fxGrid->m_syncx.Unlock();
	return 0;
}

void CintGrid::RunExcel(bool visible)
{
	m_syncx.Lock();

	m_excelF = visible;
	m_excelS = _T("");

	CString	text = _T(""), temps;

	xROWS pRow;
	xCELL cell;
	for (int ii = 0; ii < m_nRows; ii++)
	{
		pRow = m_aryGridRows[ii];
		for (int jj = 0; jj < m_nCols; jj++)
		{
			cell = pRow->at(jj);
			text = cell->text;

			if (IsCellAttribute(CIdCell(0, jj), GVAT_HIDDEN))
				continue;

			if (text.GetLength() > 0)
			{
				if (cell->attr & GVAT_CONTRAST)
				{
					switch (text[0])
					{
					case '1':		temps = ARR_UP; break;
					case '2': case '+':	temps = TRI_UP; break;
					case '4':		temps = ARR_DN; break;
					case '5': case '-':	temps = TRI_DN; break;
					default:		temps = _T("  "); break;
					}
					text = text.Mid(1); text.Insert(0, temps);
				}

				if (cell->attr & GVAT_CONDITIONx)
				{
					text.Remove('+'); text.Remove('-');
				}
			}

			m_excelS += text; m_excelS += "\t";
		}
		m_excelS += "\n";
	}
	m_syncx.Unlock();

	m_excelH = AfxBeginThread(RunExcelThread, this, THREAD_PRIORITY_HIGHEST);
}

bool CintGrid::RunExcel_()
{
	if (m_excelS.IsEmpty())
		return false;

	::OleInitialize(nullptr);

	_Application	excel;
	if (!excel.CreateDispatch("Excel.Application"))
	{
//		TRACE("CreateDispatch error....\n");
		::OleUninitialize();
		return true;
	}

	COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	Workbooks  Wbs; _Workbook  Wb;
	Worksheets Wss; _Worksheet Ws;
	Range	range;

	Wbs = excel.GetWorkbooks();
	Wb  = Wbs.Add(vOpt);

	Wss = Wb.GetWorksheets();
	Ws  = Wss.Add(vOpt, vOpt, vOpt, vOpt);

	Ws.SetName("Document");

	if (m_excelS.IsEmpty())
	{
		excel.ReleaseDispatch();
		::OleUninitialize(); m_excelH = nullptr;
		return false;
	}

	const	int alphaC = 24;
	CString	string; string.Format("A1:%c%d", (char)('A' + m_nCols), m_nFixedRows + m_nRows);
	if (m_nCols >= alphaC)
		string.Format("A1:%c%c%d", (char)('A' + ((m_nCols/alphaC) -1)),
					(char)('A' + m_nCols % alphaC), m_nFixedRows + m_nRows);

	range = Ws.GetRange(COleVariant((LPCTSTR)string), vOpt);
	range.Clear();

	if (m_excelS.IsEmpty())
	{
		excel.ReleaseDispatch();
		::OleUninitialize(); m_excelH = nullptr;
		return false;
	}

	for (int jj = 0; m_nFixedRows && jj < m_nCols; jj++)
	{
		if (!IsCellAttribute(CIdCell(0, jj), GVAT_TEXT))
			continue;

		string.Format("%c%d:%c%d", (char)('A' + jj), m_nFixedRows, (char)('A' + jj), m_nRows);
		if (jj >= alphaC)
		{
			string.Format("%c%c%d:%c%c%d", (char)('A' + ((jj / alphaC)-1)),
							(char)('A' + jj % alphaC), m_nFixedRows,
							(char)('A' + ((jj / alphaC)-1)),
							(char)('A' + jj % alphaC), m_nRows);
		}

		Range rangex = Ws.GetRange(COleVariant((LPCTSTR)string), vOpt);
		string = _T("@"); rangex.SetNumberFormat(COleVariant(string));
	}

	long row{}, col{};
	bool last = false;
	int idx{}; CString stringX;
	for (int ii = 0; !m_excelS.IsEmpty() && ii < m_nRows; ii++)
	{
		idx = m_excelS.Find("\n");
		if (idx != -1)
		{
			stringX = m_excelS.Left(idx);
			m_excelS = m_excelS.Mid(idx+1);
		}
		else
		{
			stringX = m_excelS; m_excelS = _T(""); last = true;
		}

		for (int jj = 0; !stringX.IsEmpty() && jj < m_nCols; jj++)
		{
			idx = stringX.Find("\t");
			if (idx != -1)
			{
				string  = stringX.Left(idx);
				stringX = stringX.Mid(idx+1);
			}
			else
			{
				string = stringX; stringX = _T("");
			}

			if (string.IsEmpty())
				continue;

			TRY
			{
				row = ii+1; col = jj+1;
				range.Set_Default(COleVariant(row), COleVariant(col), COleVariant((LPCTSTR)string));
			}
			CATCH(COleDispatchException, e)
			{
				TRACE("runexcel error [%s]\n", e->m_strDescription);
				break;
			}
			END_CATCH
		}

		if (!Ws.m_lpDispatch) break;
	}

	excel.SetVisible(m_excelF);
	if (!m_excelF)
	{
		Wbs.Close(); excel.Quit();
	}

	excel.ReleaseDispatch();
	::OleUninitialize();

	m_excelH = nullptr;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
///
void CintGrid::PreSubclassWindow() 
{    
	CWnd::PreSubclassWindow();

	HFONT hFont = ::CreateFontIndirect(&m_logfont);
	OnSetFont((LPARAM)hFont, 0);
	DeleteObject(hFont);

	ResetScrollBars();
}

BOOL CintGrid::SubclassWindow(HWND hWnd) 
{    
	if (!CWnd::SubclassWindow(hWnd))
		return FALSE;

//	m_dropTarget.Register(this);

	if (m_pTooltip == nullptr)
	{
		m_pTooltip = std::make_unique<CTipDlg>(m_pViewWnd, this);
		m_pTooltip->Create(CTipDlg::IDD, nullptr);	
	}

	if (m_pToolChart == nullptr)
	{
		m_pToolChart = std::make_unique<CTipDlg>(m_pViewWnd, this, true);			
		m_pToolChart->Create(CTipDlg::IDD, nullptr);	
	}

	if (m_pToolNews == nullptr)
	{
		m_pToolNews = std::make_unique<CTipDlg>(m_pViewWnd, this, true);			
		m_pToolNews->Create(CTipDlg::IDD, nullptr);	
	}

	return TRUE;
}

LRESULT CintGrid::MessageToParent(int nRow, int nCol, int nMessage)
{
	if (!IsWindow(m_hWnd))
		return 0;

	NM_GVITEM nmgv;
// 	TRACE("MessageToParent row : %d", nRow);
	nmgv.row = nRow;
	nmgv.col = nCol;

	nmgv.hdr.hwndFrom = m_hWnd;
	nmgv.hdr.idFrom   = GetDlgCtrlID();
	nmgv.hdr.code     = nMessage;

	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		return pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
	
	return 0;
}

BEGIN_MESSAGE_MAP(CintGrid, CWnd)
	//{{AFX_MSG_MAP(CintGrid)
	ON_WM_PAINT()
	ON_WM_GETDLGCODE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_CAPTURECHANGED()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CHAR()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_WM_NCMOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_NOTIFY(GVNM_ENDEDIT,  IDC_INPLACE_EDIT,  OnEndInPlaceEdit)
	ON_NOTIFY(GVNM_ENDCOMBO, IDC_INPLACE_COMBO, OnEndInPlaceCombo)
	ON_NOTIFY(GVNM_ENDDATE,  IDC_INPLACE_DATE,  OnEndInPlaceDate)
	ON_NOTIFY(GVNM_ENDTIME,  IDC_INPLACE_TIME,  OnEndInPlaceTime)
	ON_NOTIFY(GVNM_SELCHANGECOMBO, IDC_INPLACE_COMBO, OnSelChangeInPlaceCombo)
	ON_CBN_SELENDOK(IDC_HISTORY, OnHistorySelEndOk)
	ON_MESSAGE(WM_MANAGE, OnManage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CintGrid message handlers
void CintGrid::OnPaint() 
{
	if(m_bPaintLock)
		return;

	CPaintDC dc(this); // device context for painting
	
#ifdef _DEBUG
	OnDraw(&dc);

	if (m_memoAry.GetSize() > 0)
		memoDraw(&dc);
#else
	xxx::CMemDC	MemDC(&dc);
	OnDraw(&MemDC);

	if (m_memoAry.GetSize() > 0)
		memoDraw(&MemDC);
#endif
}

void CintGrid::SetPaintLock(BOOL bLock)
{
	m_bPaintLock = bLock;
}


void CintGrid::EraseBkgnd(CDC* pDC) 
{	
	CRect  visRect, clipRect, rect;
	CBrush fixedBack(m_fixbkCOL), back(m_bkCOL);

	if (pDC->GetClipBox(clipRect) == ERROR) 		return;

	GetVisibleNonFixedCellRange(visRect);

	// Draw Fixed columns background
	const int fixedColWidth = GetFixedColumnWidth();
	if (clipRect.left < fixedColWidth && clipRect.top < visRect.bottom)
		pDC->FillRect(CRect(clipRect.left, clipRect.top, fixedColWidth, visRect.bottom), &fixedBack);
        
	// Draw Fixed rows background
	const int fixedRowHeight = GetFixedRowHeight();
	if (clipRect.top < fixedRowHeight && clipRect.right > fixedColWidth && clipRect.left < visRect.right)
		pDC->FillRect(CRect(fixedColWidth -1, clipRect.top, visRect.right, fixedRowHeight), &fixedBack);

	// Draw non-fixed cell background
	if (rect.IntersectRect(visRect, clipRect)) 
	{
		CRect cellRect(max(fixedColWidth, rect.left),  max(fixedRowHeight, rect.top),
					rect.right, rect.bottom);
		pDC->FillRect(cellRect, &back);
	}

	// Draw right hand side of window outside grid
	if (visRect.right < clipRect.right)
		pDC->FillRect(CRect(visRect.right, clipRect.top, clipRect.right, clipRect.bottom), &back);

	// Draw bottom of window below grid
	if (visRect.bottom < clipRect.bottom && clipRect.left < visRect.right) 
		pDC->FillRect(CRect(clipRect.left, visRect.bottom, visRect.right, clipRect.bottom), &back);

	fixedBack.DeleteObject();
	back.DeleteObject();
}

UINT CintGrid::OnGetDlgCode() 
{
	UINT nCode = DLGC_WANTARROWS|DLGC_WANTCHARS;

	if (m_handletabkey && !press_CTRLKEY()) 
		nCode |= DLGC_WANTTAB;

	return nCode;
}

void CintGrid::OnSysColorChange() 
{
	CWnd::OnSysColorChange();
    
	if (m_fgCOL == m_winfgCOL)	m_fgCOL = ::GetSysColor(COLOR_WINDOWTEXT);
	if (m_bkCOL == m_shadowCOL)	m_bkCOL = ::GetSysColor(COLOR_3DSHADOW);

	// MODIFY PSH 20070911
	//if (m_fixfgCOL = m_winfgCOL)	m_fixfgCOL = ::GetSysColor(COLOR_WINDOWTEXT);
	//if (m_fixbkCOL = m_3dfaceCOL)	m_fixbkCOL = ::GetSysColor(COLOR_3DFACE);
	if (m_fixfgCOL == m_winfgCOL)	m_fixfgCOL = ::GetSysColor(COLOR_WINDOWTEXT);
	if (m_fixbkCOL == m_3dfaceCOL)	m_fixbkCOL = ::GetSysColor(COLOR_3DFACE);
	// END MODIFY

	m_winfgCOL = ::GetSysColor(COLOR_WINDOWTEXT);
	m_3dfaceCOL  = ::GetSysColor(COLOR_3DFACE);
	m_shadowCOL  = ::GetSysColor(COLOR_3DSHADOW);
}

void CintGrid::OnCaptureChanged(CWnd *pWnd) 
{
	if (pWnd->GetSafeHwnd() == GetSafeHwnd())
		return;

	if (m_timerID != 0)
	{
		KillTimer(m_timerID);
		m_timerID = 0;
	}

	//091013 ����
	if (m_mousemode == mouseDRAGGING)
	{
		m_mousemode = mouseNOTHING;
	}
}

int CintGrid::GetMarketByCode(CString strCode)
{
	strCode.TrimLeft();
	strCode.TrimRight();

	int nMarketType = MarketStock;

	switch(strCode.GetLength()) {
	case CODELEN_INDEX:
		{
			nMarketType = MarketIndex;			    // ����
		}
		break;
	case CODELEN_STOCK:
		{
			if ((strCode[0] == '5') || (strCode[0] == '6'))
			{
				nMarketType = MarketELW;		    // ELW
			}
			else
			{
				char *tmp;
				strtol(strCode.operator LPCTSTR(), &tmp, 10);

				if (0 == *tmp)
				{
					nMarketType = MarketStock;		// �ֽ�
				}
				else
				{
					nMarketType = MarketForeign;	// �ؿ�
				}
			}
		}
		break;
	case CODELEN_FUTOPT:
		{
			if(strCode.Left(1) == "1" || strCode.Left(1) == "4" || strCode.Left(3) == "999")	//����, ��������, ���� ����
			{
				nMarketType = MarketFuture;
			}
			else if(strCode.Left(2) == "20" || strCode.Left(2) == "30")	//�ɼ�(��,ǲ)
			{
				nMarketType = MarketOption;
			}
			else if(strCode.Left(2) == "21" || strCode.Left(2) == "31")	//�����ֽĿɼ�(��,ǲ)
			{
				nMarketType = MarketStockOption;
			}
			else 
			{
				nMarketType = MarketForeign;
			}
		}
		break;
	default:  // �ؿ�
		{
			nMarketType = MarketForeign;
		}
		break;
	}

	return nMarketType;
}

void CintGrid::OnTimer(UINT nIDEvent)
{
	if (nIDEvent != WM_LBUTTONDOWN)
	{
		KillTimer(nIDEvent);
		
		COLORREF  bkcol{};
		struct _timerID* timerID = (struct _timerID*)&nIDEvent;
		const int row = timerID->row;
		const int col = timerID->col;
		
		if (timerID->kind != TK_NORMAL)
		{
			CPoint	pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			int	cRow = 0, cCol = 0;
			GetCellFromPoint(pt, cRow, cCol);
		
			if (cRow != row || cCol != col)
				return;

			CRect	cellRC;
			GetCellRect(row, col, cellRC);
			CRect	rect;
			GetClientRect(rect);			

			switch (timerID->kind)
			{
// 			case TK_CHART:
 			case TK_NEWS:
				{
					CString code = GetItemText(row, colCODE);
					CString name = GetItemText(row, colNAME);
					const int type = GetTypeInRect(cellRC, pt);
				
					if ((int)timerID->kind != type)
						return;

					CWnd*	pWnd = GetParent();
					const UINT	dataidx = (UINT)pWnd->SendMessage(WM_MANAGE, MK_GETDATAKIND);
					CString	mapname = _T(""), option = _T("");
					//BOOL	bChart = pWnd->SendMessage(WM_MANAGE, MK_CHART);

					switch (type)
					{
// 					case TK_CHART:
// 						{
// 							if (!bChart)
// 							{
// 								mapname = "IB202210";
// 								option  = "1301\t";
// 
// 								switch(GetMarketByCode(code)) {
// 								case MarketFuture:
// 									mapname = "IB202211";
// 									option  = "31301\t";
// 									break;
// 								case MarketOption:
// 									mapname = "IB202212";
// 									option  = "41301\t";
// 									break;
// 								case MarketIndex:
// 									mapname = "IB202213";
// 									option  = "21301\t";
// 									break;
// /*								case MarketStockOption:
// 									mapname = "IB202214";
// 									option  = "71301\t";
// 									break;
// */								}
// 							
// 								option += code;
// 								if (!code.IsEmpty())
// 								{
// 									m_pToolChart->ShowMap(cellRC, rect, mapname, option);
// 									if (m_pToolNews)
// 											m_pToolNews->HideTips();	
// 								}
// 							}
// 						}
// 						break;
					case TK_NEWS:
						{
							const LPARAM lParam = GetItemData(row, col);
							WORD	news = HIWORD(lParam);
							
							if (CAST_TREEID(dataidx)->kind == xISSUE)
							{
								news = 1;
							}
							const WORD	high = 0;
							SetItemData(row, colSIG, MAKELPARAM(news, high));

							CString strValue;

							strValue = m_pParent->GetNewsData(code);
							
							CString str;
							str.Format("IBNEWSXX /t /p5 /d %s", strValue);
			
							if(strValue != "")
							{
								if(m_pViewWnd)	
									m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typePOPUPX), (LPARAM)(LPCTSTR)str);
							}

// 							if (news && !code.IsEmpty())
// 							{
// 								mapname = "IB202220";
// 								option = "1301\t" + code;
// 								m_pToolNews->ShowMap(cellRC, rect, mapname, option);
// 								if (m_pToolChart)
// 											m_pToolChart->HideTips();	
// 							}
						}
						break;
					}
				}
				break;
			case TK_CONTENT:
				{
					CString string = GetItemText(row, col);
// 					if (string.Compare(m_pTooltip->GetTip()))
// 					{
// 						//m_pTooltip->ShowTips(cellRC, rect, string);
// 					}
				}
				break;
			case TK_CODE:
				{
					//����ȭ���̱� ������ ���� ǥ�������� ����.
// 					CString string = GetItemText(row, colCODE);
// 
// 					if (GetItemAttr(row, colNAME)&GVAT_MARKER)
// 						break;
// 
// 					if (!string.IsEmpty())
// 					{
// 						CString	szTmp = GetItemText(row, col);
// 						szTmp = GetCodeType(szTmp);
// 
// 						if ("!" == szTmp)
// 						{
// 							szTmp = " : " + GetItemText(row, colINFO);
// 						}
// 						//��������ȯ�� ���� ���� �߰�
// 						CString	str = GetItemText(row, colINFO);
// 
// 						if("�ܱ�" == str)
// 						{
// 							szTmp.Format(" : %s", "�ܱ��������"); 
// 						}
// 						else if ("��������ȯ��" == str)
// 						{
// 							szTmp.Format(" : %s", "��������ȯ��"); 
// 						}
// 						else if("���ڰ��" == str)
// 						{
// 							szTmp.Format(" : %s", "���ڰ��"); 
// 						}
// 						else if("���迹��" == str)
// 						{
// 							szTmp.Format(" : %s", "���迹��"); 
// 						}
// 						else if("����" == str)
// 						{
// 							szTmp.Format(" : %s", "����"); 
// 						}
// 						else if("�Ǹ�" == str)
// 						{
// 							szTmp.Format(" : %s", "�Ǹ�"); 
// 						}
// 						else if("���" == str)
// 						{
// 							szTmp.Format(" : %s", "���"); 
// 						}
// 						else if("�׸�" == str)
// 						{
// 							szTmp.Format(" : %s", "�׸�"); 
// 						}
// 						else if("����" == str)
// 						{
// 							szTmp.Format(" : %s", "����"); 
// 						}
// 						else if("�պ�" == str)
// 						{
// 							szTmp.Format(" : %s", "�պ�"); 
// 						}
// 						else if("����" == str)
// 						{
// 							szTmp.Format(" : %s", "����"); 
// 						}
// 
// 						string += szTmp;
// 						if (string.Compare(m_pTooltip->GetTip()))
// 						{
// 							//m_pTooltip->ShowTips(cellRC, rect, string);					
// 						}
// 						
// 					}
				}
				break;
			case TK_MEMO:
				{
					CString string;

					if (GetItemAttr(row, colNAME)&GVAT_MARKER)
						break;

					//�޸� �ִ��� Ȯ��
					CString tMemo;
					tMemo.Empty();
					if (row >= GetFixedRowCount() && m_memoAry.GetAt(row - 1))
					{
						CString	code = GetItemText(row, codeCOL);
						tMemo = m_memoWnd->loadMemoUsingTip(code);
					}
					
					if(!tMemo.IsEmpty())
					{
						string += tMemo;
					}

// 					if (string.Compare(m_pTooltip->GetTip()))
// 					{
// 						//m_pTooltip->ShowTips(cellRC, rect, string);					
// 					}
				}
				break;
			case TK_RCODE:
				{
					CString string = GetItemText(row, colRCOD);
					
					if (!string.IsEmpty())
					{
						CString	szTmp = GetItemText(row, col);
						szTmp = GetCodeType(szTmp);

						string += szTmp;
// 						if (string.Compare(m_pTooltip->GetTip()))
// 						{
// 							//m_pTooltip->ShowTips(cellRC, rect, string);					
// 						}
						
					}
				}
				break;
			case TK_BONG:
				{				
					CString	rtscode = GetItemText(row, 0);

					if (rtscode.GetLength())
					{
						int	ndot = 0;

						//2012.07.17 KSJ �ɼ��� 'X' �� �پ�´�.
// 						if(rtscode.GetAt(0) == 'X' || rtscode.GetAt(0) == 'x')
// 						{
// 							rtscode = rtscode.Mid(1);
// 						}
						// KSJ

						switch (rtscode.GetAt(0))
						{
						case '1':	//2012.10.09 KSJ �ֽļ����� �Ҽ����� �Ⱥپ�� �Ѵ�.
							{				
								CString strScoop = rtscode.Mid(1, 2);
								
								if( strScoop <= "ZZ" && strScoop > "10")
									ndot = 0;
								else
									ndot = 2;
							}
							break;
						case '2':
						case '3':
						case '4':
							ndot = 2;
							break;
						}
						CString	string = _T("");
						CString	szunit;
						CString	bongdata = GetItemText(row, col);
						double	curr{}, open{}, high{}, low{}, pcurr{}, vol{};
						
						
						
						curr  = IH::TOfabs(GetItemText(row, colCURR));
						open  = IH::TOfabs(GetItemText(row, colOPEN));
						high  = IH::TOfabs(IH::Parser(bongdata, PTAB));
						low   = IH::TOfabs(bongdata);
						vol   = IH::TOfabs(GetItemText(row, colVOL));
						pcurr = IH::TOfabs(GetItemText(row, colPCURR));
						
						string += "\t";
						szunit.Format("%.*f", ndot, open); 
						szunit = "��    �� :\t" + CommaModify(szunit) + "\n";
						string += szunit;
						szunit.Format("%.*f", ndot, high);
						szunit = "��    �� :\t" + CommaModify(szunit) + "\n";
						string += szunit;
						szunit.Format("%.*f", ndot, low);
						szunit = "��    �� :\t" + CommaModify(szunit) + "\n";
						string += szunit;
						szunit.Format("%.*f", ndot, curr);
						szunit = "��    �� :\t" + CommaModify(szunit) + "\n";
						string += szunit;
						szunit.Format("%ld", (LONG)vol);
						szunit = "�� �� �� :\t" + CommaModify(szunit) + "\n";
						string += szunit;
						szunit.Format("%.*f", ndot, pcurr);
						szunit = "�� �� �� :\t" + CommaModify(szunit) + "\n";
						string += szunit;
						//m_pTooltip->ShowTips(cellRC, rect, string);
					}			
				}
				break;
			}
			return;
		}

		if (!m_mapRTM.Lookup(nIDEvent, bkcol))
			return;

		m_sync.Lock();
		m_mapRTM.RemoveKey(nIDEvent);
		m_sync.Unlock();

		SetItemBkColor(row, col, bkcol, true);
		return;
	}

	if (IsCellAttribute(m_idSelect, GVAT_DRAGDROP)
			&& GetItemState(m_idSelect.row, m_idSelect.col) & GVST_SELECTED)
	{
		if(m_mousemode == mouseNOTHING)
		{
			KillTimer(nIDEvent);
		}
		
		m_isLbtnClick = TRUE;				//Ŭ������ 091013 ����
// 		TRACE("OnTimer::m_isLbtnClick\n");
		m_mousemode = mousePREPAREDRAG;
		OnBeginDrag();
		return;
	}
	else if (IsCellAttribute(m_idSelect, GVAT_DRAGMOVE))
	{
		KillTimer(nIDEvent);
		m_mousemode = mouseMOVING;
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));

		ResetSelectedRange();

		CRect clientRC; GetClientRect(&clientRC);
		ClientToScreen(&clientRC);
		ClipCursor(clientRC);
		return;
	}

	CPoint pt, origPt;
	if (!GetCursorPos(&origPt))
		return;

	ScreenToClient(&origPt);

	CRect rect; 
	GetClientRect(rect);
	
// 	TRACE("OnTimer::GetCursorPos\n");

	const int nFixedRowHeight = GetFixedRowHeight();
	const int nFixedColWidth = GetFixedColumnWidth();

	pt = origPt;
	if (pt.y > rect.bottom)
	{
		SendMessage(WM_KEYDOWN, VK_DOWN, 0);

		if (pt.x < rect.left)  pt.x = rect.left;
		if (pt.x > rect.right) pt.x = rect.right;
		pt.y = rect.bottom;

		OnSelecting(GetCellFromPoint(pt));
	}
	else if (pt.y < nFixedRowHeight)
	{
		SendMessage(WM_KEYDOWN, VK_UP, 0);

		if (pt.x < rect.left)  pt.x = rect.left;
		if (pt.x > rect.right) pt.x = rect.right;
		pt.y = nFixedRowHeight + 1;

		OnSelecting(GetCellFromPoint(pt));
	}

	pt = origPt;
	if (pt.x > rect.right)
	{
		SendMessage(WM_KEYDOWN, VK_RIGHT, 0);

		if (pt.y < rect.top)	pt.y = rect.top;
		if (pt.y > rect.bottom) pt.y = rect.bottom;
		pt.x = rect.right;

		OnSelecting(GetCellFromPoint(pt));
	} 
	else if (pt.x < nFixedColWidth)
	{
		SendMessage(WM_KEYDOWN, VK_LEFT, 0);

		if (pt.y < rect.top)	pt.y = rect.top;
		if (pt.y > rect.bottom)	pt.y = rect.bottom;
		pt.x = nFixedColWidth + 1;

		OnSelecting(GetCellFromPoint(pt));
	}
}

void CintGrid::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!IsValid(m_idCurrent)) 
	{
		CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
		return;
	}

	const CIdCell nextCell = m_idCurrent;

	switch (nChar)
	{
	case VK_DOWN:
	case VK_RETURN:
		if (nextCell.row <= (GetRowCount() -1))
		{
			GetParent()->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TRIGGEROW, nextCell.row));
		}
		break;
	case VK_UP:
		if (nextCell.row >= m_nFixedRows)
		{
			GetParent()->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TRIGGEROW, nextCell.row));
		}
		break;
     case VK_NEXT:   
		{
			GetParent()->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TRIGGEROW, nextCell.row));
		}
		break;
     case VK_PRIOR:  
		{
			GetParent()->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TRIGGEROW, nextCell.row));
		}
        default:
		CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
		break;
	}
}

void CintGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!IsValid(m_idCurrent)) 
	{
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	CIdCell nextCell = m_idCurrent;
	BOOL bChangeLine = FALSE;

	if (press_CTRLKEY() && m_useOLE)
	{
		switch (nChar)
		{
		case 'A':
			OnEditSelectAll();  break;
		case 'X':
			OnEditCut();        break;
		case 'C':
			OnEditCopy();       break;
		case 'V':
			OnEditPaste();      break;
		}
	}

	switch (nChar)
	{
	case VK_INSERT:		//091013 ����
		{
			const UINT	kind = (UINT)GetParent()->SendMessage(WM_MANAGE, MK_GETDATAKIND);
			if (CAST_TREEID(kind)->kind != xISSUE)
				GetParent()->SendMessage(WM_MANAGE, MAKEWPARAM(MK_INSERTCODE, m_idCurrent.row));
		}
		break;
	case VK_DELETE:
		{
			const UINT	kind = (UINT)GetParent()->SendMessage(WM_MANAGE, MK_GETDATAKIND);
			if (CAST_TREEID(kind)->kind != xISSUE)
				GetParent()->SendMessage(WM_MANAGE, MAKEWPARAM(MK_DELETEROW, m_idCurrent.row));

			m_idCurrent = nextCell;
		}

		break;
    case VK_TAB:
		if (press_SHIFTKEY())
		{
			if (nextCell.col > m_nFixedCols)
			{
				nextCell.col--;
				for (int ii = nextCell.col; ii > m_nFixedCols; ii--)
				{
					if (!IsCellAttribute(nextCell, GVAT_HIDDEN))
						break;
					nextCell.col--;
				}
			}
			else if (nextCell.col == m_nFixedCols && nextCell.row > m_nFixedRows) 
			{
				nextCell.row--; 
				nextCell.col = GetColumnCount() - 1; 
				bChangeLine = TRUE;
			}
			else
				CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		}
		else
		{
			if (nextCell.col < (GetColumnCount() - 1)) 
			{
				nextCell.col++;
				for (int ii = nextCell.col; ii < m_nCols; ii++)
				{
					if (!IsCellAttribute(nextCell, GVAT_HIDDEN))
						break;
					nextCell.col++;
				}
			}
			else if (nextCell.col == (GetColumnCount() - 1) && nextCell.row < (GetRowCount() - 1))
			{
				nextCell.row++; 
				nextCell.col = m_nFixedCols; 
				bChangeLine = TRUE;
			}
			else
				CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		} 
		break;
		
	case VK_DOWN:
	case VK_RETURN:
		if (nextCell.row < (GetRowCount() -1))
		{
			nextCell.row++;

			m_idDrag.row = nextCell.row;
			m_idDrag.col = nextCell.col;
// 			GetParent()->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TRIGGEROW, nextCell.row));


			for (int ii = 0; ii < m_nRows; ii++)
			{
				if (GetRowHeight(nextCell.row) > 0)
					break;

				
				nextCell.row++;
			}
		}
		break;
	case VK_UP:
		if (nextCell.row > m_nFixedRows)
		{
			nextCell.row--;

			m_idDrag.row = nextCell.row;
			m_idDrag.col = nextCell.col;
// 			GetParent()->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TRIGGEROW, nextCell.row));


			for (int ii = nextCell.row; ii > m_nFixedRows; ii--)
			{
				if (GetRowHeight(nextCell.row) > 0)
					break;

				
				nextCell.row--;
			}
		}
		break;
        case VK_RIGHT:  
		if (nextCell.col < (GetColumnCount() - 1))
		{
			nextCell.col++;
			for (int ii = nextCell.col; ii < m_nCols; ii++)
			{
				if (!IsCellAttribute(nextCell, GVAT_HIDDEN)
						&& GetColumnWidth(nextCell.col) > 0)
					break;
				nextCell.col++;
			}
		}
		break;
        case VK_LEFT:   
		if (nextCell.col > m_nFixedCols)
		{
			nextCell.col--;
			for (int ii = nextCell.col; ii > m_nFixedCols; ii--)
			{
				if (!IsCellAttribute(nextCell, GVAT_HIDDEN)
						&& GetColumnWidth(nextCell.col) > 0)
					break;
				nextCell.col--;
			}
		}
		break;
        case VK_NEXT:   
		{
			const CIdCell oldCell = GetTopleftNonFixedCell();
			SendMessage(WM_VSCROLL, SB_PAGEDOWN, 0);

			const CIdCell newCell = GetTopleftNonFixedCell();

			const int increment = newCell.row - oldCell.row;
			if (increment)
			{
				nextCell.row += increment;
				if (nextCell.row > (GetRowCount() - 1)) 
					nextCell.row = GetRowCount() - 1;
			}
			else
				nextCell.row = GetRowCount() - 1;

			m_idDrag.row = nextCell.row;
			m_idDrag.col = nextCell.col;
// 			GetParent()->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TRIGGEROW, nextCell.row));
		}
		break;
            case VK_PRIOR:  
		{
			const CIdCell oldCell = GetTopleftNonFixedCell();
			SendMessage(WM_VSCROLL, SB_PAGEUP, 0);

			const CIdCell newCell = GetTopleftNonFixedCell();

			const int increment = newCell.row - oldCell.row;
			if (increment) 
			{
				nextCell.row += increment;
				if (nextCell.row < m_nFixedRows) 
					nextCell.row = m_nFixedRows;
			}
			else
				nextCell.row = m_nFixedRows;

			m_idDrag.row = nextCell.row;
			m_idDrag.col = nextCell.col;
// 			GetParent()->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TRIGGEROW, nextCell.row));
		}
		break;
        case VK_HOME:   
		SendMessage(WM_VSCROLL, SB_TOP, 0);
		nextCell.row = m_nFixedRows;
		break;
        case VK_END:    
		SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
		nextCell.row = GetRowCount() - 1;
		break;
        default:
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		break;
	}
  
	if(nextCell.row == m_idCurrent.row && nextCell.col == m_idCurrent.col)
	{
		return;
	}
	else//if (nextCell.row != m_idCurrent.row && nextCell.col != m_idCurrent.col)
	{
		if (m_mousemode == mouseNOTHING)
		{
			m_mapPreCell.RemoveAll();
			// updateX_20050328
			m_mousemode = (m_dragdrop == GVDD_NONE && !m_selectLine) ? mouseSELECTCELLS : mouseSELECTROW;
			if (!press_SHIFTKEY() || nChar == VK_TAB)
				m_idSelect = nextCell;
			OnSelecting(nextCell);
			m_mousemode = mouseNOTHING;
		}
		SetFocusCell(nextCell);

		if (!IsCellVisible(nextCell))
		{   
			EnsureVisible(nextCell); // Make sure cell is visible

			switch (nChar)
			{
			case VK_RIGHT:  
				SendMessage(WM_HSCROLL, SB_LINERIGHT, 0); 
				break;
			case VK_LEFT:   
				SendMessage(WM_HSCROLL, SB_LINELEFT, 0);  
				break;
			case VK_DOWN:   
				SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);  
				break;
			case VK_UP:     
				SendMessage(WM_VSCROLL, SB_LINEUP, 0);    
				break;                
			case VK_TAB:    
				if (press_SHIFTKEY())
				{
					if (bChangeLine) 
					{
						SendMessage(WM_VSCROLL, SB_LINEUP, 0);
						SetScrollPos32(SB_HORZ, m_Hscroll);
					}
					else 
						SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
				}
				else
				{
					if (bChangeLine) 
					{
						SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
						SetScrollPos32(SB_HORZ, 0);
						break;
					}
					else 
						SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
				}
				break;
			}
			Invalidate();
		}
	}
}

void CintGrid::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!press_CTRLKEY() && m_mousemode == mouseNOTHING)
	{
		if (!m_handletabkey)
			OnEditCell(m_idCurrent.row, m_idCurrent.col, nChar);
		else if (nChar != VK_TAB && nChar != VK_RETURN)
			OnEditCell(m_idCurrent.row, m_idCurrent.col, nChar);
	}
	
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CintGrid::OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{    
	NM_USER *nmuser = (NM_USER *)pNMHDR;
	GVITEM  *gvitem = &nmuser->item;

	// In case OnEndInPlaceEdit called as window is being destroyed
	if (!IsWindow(GetSafeHwnd()))
		return;

	CString	str = GetItemText(gvitem->row, 0);
	if (str.IsEmpty())
		return;
					

	const int	nval = atoi(gvitem->text);
	if (nval == 0)
	{
		CString name;
		CString isCode = GetItemText(nmuser->item.row, colCODE);
		if(IsCellAttribute(CIdCell(nmuser->item.row, colNAME), GVAT_MARKER) == FALSE && isCode.IsEmpty() == FALSE)
		{
			name = m_pParent->GetCodeName(isCode);
			OnEndEditCell(gvitem->row, gvitem->col, name);
		}
		else
		{
			OnEndEditCell(gvitem->row, gvitem->col, "");
		}
	}
	else
		OnEndEditCell(gvitem->row, gvitem->col, gvitem->text);
	
	InvalidateCellRect(CIdCell(gvitem->row, gvitem->col));

	switch (gvitem->param)		// for kiwoom
	{
	case VK_TAB:
// 		TRACE("MessageToParent1 row : %d", gvitem->row);
		MessageToParent(gvitem->row, gvitem->col, GVNM_ENDTABKEY);	
		break;
	case VK_RETURN:
// 		TRACE("MessageToParent2 row : %d", gvitem->row);
		MessageToParent(gvitem->row, gvitem->col, GVNM_ENDRETURN);	
		break;
	default:
// 		TRACE("MessageToParent3 row : %d", gvitem->row);
		MessageToParent(gvitem->row, gvitem->col, GVNM_ENDEDIT);	
		break;		
	}

	switch (gvitem->param) 
	{
	case VK_DOWN: 
	case VK_UP:   
	case VK_RIGHT:
	case VK_LEFT:  
	case VK_NEXT:  
	case VK_PRIOR: 
	case VK_HOME:  
	case VK_END:    
		OnKeyDown(gvitem->param, 0, 0);
		OnEditCell(m_idCurrent.row, m_idCurrent.col, gvitem->param);
		break;
	}

	*pResult = 0;
}

void CintGrid::OnEndInPlaceCombo(NMHDR* pNMHDR, LRESULT* pResult) 
{    
	NM_USER *nmuser = (NM_USER *)pNMHDR;
	GVITEM  *gvitem = &nmuser->item;

	// In case OnEndInPlacecombo called as window is being destroyed
	if (!IsWindow(GetSafeHwnd()))
		return;

	OnEndComboCell(gvitem->row, gvitem->col, gvitem->text, gvitem->param);
	InvalidateCellRect(CIdCell(gvitem->row, gvitem->col));
// 	TRACE("MessageToParent4 row : %d", gvitem->row);
	MessageToParent(gvitem->row, gvitem->col, GVNM_ENDCOMBO);

	*pResult = 0;
}

void CintGrid::OnSelChangeInPlaceCombo(NMHDR* pNMHDR, LRESULT* pResult) 
{    
	NM_USER *nmuser = (NM_USER *)pNMHDR;
	GVITEM  *gvitem = &nmuser->item;
	if (!IsWindow(GetSafeHwnd()))
		return;
	OnEndComboCell(gvitem->row, gvitem->col, gvitem->text, gvitem->param);
// 	TRACE("MessageToParent5 row : %d", gvitem->row);
	MessageToParent(gvitem->row, gvitem->col, GVNM_SELCHANGECOMBO);
	
	*pResult = 0;
}

void CintGrid::OnEndInPlaceDate(NMHDR* pNMHDR, LRESULT* pResult) 
{    
	NM_USER *nmuser = (NM_USER *)pNMHDR;
	GVITEM  *gvitem = &nmuser->item;

	// In case OnEndInPlacecombo called as window is being destroyed
	if (!IsWindow(GetSafeHwnd()))
		return;

	OnEndDateCell(gvitem->row, gvitem->col, gvitem->text);
	InvalidateCellRect(CIdCell(gvitem->row, gvitem->col));
// 	TRACE("MessageToParent6 row : %d", gvitem->row);
	MessageToParent(gvitem->row, gvitem->col, GVNM_ENDDATE);

	*pResult = 0;
}

void CintGrid::OnEndInPlaceTime(NMHDR* pNMHDR, LRESULT* pResult) 
{    
	NM_USER *nmuser = (NM_USER *)pNMHDR;
	GVITEM  *gvitem = &nmuser->item;

	// In case OnEndInPlacecombo called as window is being destroyed
	if (!IsWindow(GetSafeHwnd()))
		return;

	OnEndTimeCell(gvitem->row, gvitem->col, gvitem->text);
	InvalidateCellRect(CIdCell(gvitem->row, gvitem->col));
// 	TRACE("MessageToParent7 row : %d", gvitem->row);
	MessageToParent(gvitem->row, gvitem->col, GVNM_ENDTIME);

	*pResult = 0;
}

// ADD PSH 20070912
void CintGrid::SetHScroll(UINT nSBCode, UINT nPos)
{
	if (m_pTooltip)
		m_pTooltip->HideTips();

	if (m_pToolChart)
		m_pToolChart->HideTips();

	if (m_pToolNews)
		m_pToolNews->HideTips();

	m_bVirHPos = TRUE;

	const int scrollPos  = m_nVirHPos;
	const CIdCell tlCell = GetTopleftNonFixedCell();
	CRect rect; GetClientRect(rect);

	switch (nSBCode)
	{
	case SB_LINERIGHT:
		{
			const int Xscroll = GetColumnWidth(tlCell.col);
			m_nVirHPos = scrollPos + Xscroll;
			
			rect.left = GetFixedColumnWidth() + Xscroll;
			ScrollWindow(-Xscroll, 0, rect);
			rect.left = rect.right - (Xscroll+1);
			InvalidateRect(rect);
		}
		break;
	case SB_LINELEFT:
		{
			const int Xscroll = GetColumnWidth(tlCell.col-1);
			m_nVirHPos = max(0, scrollPos - Xscroll);
			
			rect.left = GetFixedColumnWidth();
			ScrollWindow(Xscroll, 0, rect);
			rect.right = rect.left + (Xscroll+1);
			InvalidateRect(rect);
		}
		break;
	case SB_PAGERIGHT:
		if (scrollPos < m_nVirHScroll)
		{
			rect.left = GetFixedColumnWidth();
			const int offset = rect.Width() - GetSystemMetrics(SM_CXVSCROLL);
			const int pos = min(m_nVirHScroll, scrollPos + offset);
			m_nVirHPos = pos;

			if ((int)nPos < m_nVirHPos) m_nVirHPos = nPos;
			
			rect.left = GetFixedColumnWidth();
			InvalidateRect(rect);
		}
		break;
	case SB_PAGELEFT:
		if (scrollPos > 0)
		{
			rect.left = GetFixedColumnWidth();
			const int offset = -rect.Width() + GetSystemMetrics(SM_CXVSCROLL);
			const int pos = max(0, scrollPos + offset);
			m_nVirHPos = pos;
			
			rect.left = GetFixedColumnWidth();
			InvalidateRect(rect);
		}
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		{
			m_nVirHPos = nPos;
			CIdCell newtlCell = GetTopleftNonFixedCell();
			if (newtlCell != tlCell)
			{
				rect.left = GetFixedColumnWidth();
				InvalidateRect(rect);
			}
		}
		break;
	case SB_LEFT:
		if (scrollPos > 0)
		{
			Invalidate();
		}
		break;
	case SB_RIGHT:
		if (scrollPos < m_nVirHScroll)
		{
			Invalidate();
		}
		break;
	default:
		break;
	}
}
// END ADD

void CintGrid::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (!m_noMessage && GetFocus()->GetSafeHwnd() != GetSafeHwnd()) 
	{
// 		TRACE("hscroll\n");
		SetFocus();
	}

	if (m_pTooltip)
		m_pTooltip->HideTips();

	if (m_pToolChart)
		m_pToolChart->HideTips();

	if (m_pToolNews)
		m_pToolNews->HideTips();

	const int scrollPos = GetScrollPos32(SB_HORZ);
//	CIdCell tlCell = GetTopleftNonFixedCell();
	CRect rect; 
	GetClientRect(rect);

	switch (nSBCode)
	{
	case SB_LINERIGHT:
		{
			const CIdCell tlCell = GetTopleftNonFixedCell(true);
			
			if (scrollPos < m_Hscroll)
			{
				const int Xscroll = GetColumnWidth(tlCell.col);
				SetScrollPos32(SB_HORZ, scrollPos + Xscroll);
				if (GetScrollPos32(SB_HORZ) == scrollPos)
				{
					if (!m_noMessage)
						MessageToParent(SB_LINERIGHT, 0, GVNM_RHSCROLL);
					break;
				}	
				rect.left = GetFixedColumnWidth() + Xscroll;
				ScrollWindow(-Xscroll, 0, rect);
				Invalidate(FALSE);
	// 			TRACE("SB_LINERIGHT: \t scrollPos:%d  Xscroll:%d  rect.left:%d\n", scrollPos, Xscroll, rect.left);
				// ADD PSH 20070911
				m_nBeforeScrollPos = scrollPos + Xscroll;

				hScrolltoleft = scrollPos + Xscroll;		//���������� ��ũ���̵��� ����

				//GetParent()->SendMessage(WM_MANAGE, MAKEWPARAM(MK_HSCROLL, nSBCode), nPos);
				// END ADD
			}
		}
		break;
	case SB_LINELEFT:
		{
			const CIdCell tlCell = GetTopleftNonFixedCell(true, false);
			
			if (scrollPos > 0 && tlCell.col > GetFixedColumnCount())
			{
				const int Xscroll = GetColumnWidth(tlCell.col);
				
				if(tlCell.col == 1)		SetScrollPos32(SB_HORZ, max(0, Xscroll));
				else					SetScrollPos32(SB_HORZ, max(0, scrollPos - Xscroll));
				
				rect.left = GetFixedColumnWidth();
				ScrollWindow(Xscroll, 0, rect);
				Invalidate(FALSE);
				//			TRACE("SB_LINELEFT\n");
				// ADD PSH 20070911
				m_nBeforeScrollPos = scrollPos - Xscroll;
				
				hScrolltoleft = scrollPos - Xscroll;
				
				//GetParent()->SendMessage(WM_MANAGE, MAKEWPARAM(MK_HSCROLL, nSBCode), nPos);
				// END ADD
			}
			else if (scrollPos <= 0 && !m_noMessage)
			{
				MessageToParent(SB_LINELEFT, 0, GVNM_LHSCROLL);
			}
		}
		break;
	case SB_PAGERIGHT:
		if (scrollPos < m_Hscroll)
		{
			rect.left = GetFixedColumnWidth();
			const int offset = rect.Width();
			const int pos = min(m_Hscroll, scrollPos + offset);
			SetScrollPos32(SB_HORZ, pos);
			Invalidate(FALSE);

// 			TRACE("SB_PAGERIGHT: \t pos:%d  offset:%d  rect.left:%d\n", pos, offset, rect.left);

			hScrolltoleft = pos;

			// ADD PSH 20070911
			//GetParent()->SendMessage(WM_MANAGE, MAKEWPARAM(MK_HSCROLL, nSBCode), GetScrollPos32(SB_HORZ));
			// END ADD

			if (pos >= m_Hscroll && !m_noMessage)
				MessageToParent(SB_PAGERIGHT, 0, GVNM_RHSCROLL);
		}
		break;
	case SB_PAGELEFT:
		if (scrollPos > 0)
		{
			rect.left = GetFixedColumnWidth();
			const int offset = -rect.Width();
			const int pos = max(0, scrollPos + offset);
			SetScrollPos32(SB_HORZ, pos);
			Invalidate(FALSE);

			hScrolltoleft = pos;
// 			TRACE("SB_PAGELEFT\n");
			// ADD PSH 20070911
			//GetParent()->SendMessage(WM_MANAGE, MAKEWPARAM(MK_HSCROLL, nSBCode), nPos);
			// END ADD
		}
		else if (scrollPos <= 0 && !m_noMessage)
		{
			MessageToParent(SB_PAGELEFT, 0, GVNM_LHSCROLL);
		}
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		{
			const CIdCell tlCell = GetTopleftNonFixedCell(true);
			
			SetScrollPos32(SB_HORZ, GetScrollPos32(SB_HORZ, TRUE));
			CIdCell newtlCell = GetTopleftNonFixedCell();
			if (newtlCell != tlCell)
			{
				rect.left = GetFixedColumnWidth();
				InvalidateRect(rect);
			}
			
			hScrolltoleft = GetScrollPos32(SB_HORZ, TRUE);
			if (nSBCode == SB_THUMBPOSITION && !m_noMessage)
			{
				rect.left = GetFixedColumnWidth();
				
				if (scrollPos <= 0)
				{
					MessageToParent(SB_THUMBPOSITION, 0, GVNM_LHSCROLL);
				}
				else if (min(m_Hscroll, scrollPos+rect.Width()) >= m_Hscroll)
				{
					MessageToParent(SB_THUMBPOSITION, 0, GVNM_RHSCROLL); 
				}
			}
			
			// ADD PSH 20070911
			Invalidate(FALSE);
			//GetParent()->SendMessage(WM_MANAGE, MAKEWPARAM(MK_HSCROLL, nSBCode), GetScrollPos32(SB_HORZ));
			// END ADD
		}
		break;
	case SB_LEFT:
// 		TRACE("SB_LEFT\n");
		if (scrollPos > 0)
		{
			SetScrollPos32(SB_HORZ, 0);
			Invalidate(false);
		}
		else if (scrollPos <= 0 && !m_noMessage)
		{
			MessageToParent(SB_LEFT, 0, GVNM_LHSCROLL);
		}
		break;
	case SB_RIGHT:
// 		TRACE("SB_RIGHT\n");
		if (scrollPos < m_Hscroll)
		{
			SetScrollPos32(SB_HORZ, m_Hscroll);
			Invalidate(false);

			rect.left = GetFixedColumnWidth();
			if (min(m_Hscroll, scrollPos+rect.Width()) >= m_Hscroll)
				MessageToParent(SB_RIGHT, 0, GVNM_RHSCROLL);
		}
		break;
	default:
		break;
	}
}

void CintGrid::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (!m_noMessage && GetFocus()->GetSafeHwnd() != GetSafeHwnd()) 
		SetFocus();

	CRect	rect;
	CIdCell	tlCell;
	int scrollPos = GetScrollPos32(SB_VERT);

	if (m_pTooltip)
		m_pTooltip->HideTips();

	if (m_pToolChart)
		m_pToolChart->HideTips();

	if (m_pToolNews)
		m_pToolNews->HideTips();

	
	tlCell = GetTopleftNonFixedCell();
	GetClientRect(rect);

	//2013.12.03 KSJ Ŀ���� ȭ��ۿ� ������ ��ũ������ ����.
	CRect rt, rcMoniter;
	CPoint cp;
	GetCursorPos(&cp);
	
	rcMoniter.SetRect(GetSystemMetrics(SM_XVIRTUALSCREEN), GetSystemMetrics(SM_YVIRTUALSCREEN), GetSystemMetrics(SM_XVIRTUALSCREEN) + GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_YVIRTUALSCREEN) + GetSystemMetrics(SM_CYVIRTUALSCREEN));
	
	m_pViewWnd->GetClientRect(&rt);
	ClientToScreen(&rt);
	rt.left -= 8;	
	rt.right -= 4;
	rt.PtInRect(cp);
		
	if(!rt.PtInRect(cp))	return;
	//2013.12.03 KSJ END Ŀ���� ȭ��ۿ� ������ ��ũ������ ����.

	switch (nSBCode)
	{
	case SB_LINEDOWN:

		if (scrollPos < m_Vscroll)
		{
			int Yscroll = 0;
			if (tlCell.row < (GetRowCount() -1) && (rect.Height() < m_nRows * GetFixedRowHeight()))	//2016.07.11 KSJ ���� �׸��� ũ�⺸�� ũ�� ��ũ�� �Ѵ�.
			{
				for (int ii = 0; ii < m_nRows; ii++)
				{
					Yscroll = GetRowHeight(tlCell.row);
					if (Yscroll > 0) break;
					tlCell.row++;
				}
			}

			SetScrollPos32(SB_VERT, scrollPos + Yscroll);
			if (GetScrollPos32(SB_VERT) == scrollPos)
			{
				if (!m_noMessage)
					MessageToParent(SB_LINEDOWN, 0, GVNM_DNVSCROLL);
				break;
			}

			rect.top = GetFixedRowHeight() + Yscroll;
			ScrollWindow(0, -Yscroll, rect);

			rect.top = rect.bottom - (Yscroll+1);
			InvalidateRect(rect);
		}
		break;
        case SB_LINEUP:
		if (scrollPos > 0 && tlCell.row > GetFixedRowCount())
		{
			int Yscroll = 0; tlCell.row--;
			for (int ii = tlCell.row; ii >= GetFixedRowCount(); ii--)
			{
				Yscroll = GetRowHeight(tlCell.row);
				if (Yscroll > 0) break;
				tlCell.row--;
			}

			SetScrollPos32(SB_VERT, max(0, scrollPos - Yscroll));
			rect.top = GetFixedRowHeight();
			ScrollWindow(0, Yscroll, rect);
			rect.bottom = rect.top + (Yscroll +1);
			InvalidateRect(rect);
		}
		else if (scrollPos <= 0 && !m_noMessage)
		{
			MessageToParent(SB_LINEUP, 0, GVNM_UPVSCROLL);
		}
		break;
        case SB_PAGEDOWN:
		if (scrollPos < m_Vscroll)
		{
			rect.top = GetFixedRowHeight();
			scrollPos = min(m_Vscroll, scrollPos + rect.Height());
			SetScrollPos32(SB_VERT, scrollPos);
			rect.top = GetFixedRowHeight();
			InvalidateRect(rect);

			if (scrollPos >= m_Vscroll && !m_noMessage)
				MessageToParent(SB_PAGEDOWN, 0, GVNM_DNVSCROLL);
		}
		break;
        case SB_PAGEUP:
		if (scrollPos > 0)
		{
			rect.top = GetFixedRowHeight();
			const int offset = -rect.Height();
			const int pos = max(0, scrollPos + offset);
			SetScrollPos32(SB_VERT, pos);
			rect.top = GetFixedRowHeight();
			InvalidateRect(rect);
		}
		else if (scrollPos <= 0 && !m_noMessage)
		{
			MessageToParent(SB_PAGEUP, 0, GVNM_UPVSCROLL);
		}
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		{
			SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT, TRUE));
			CIdCell newtlCell = GetTopleftNonFixedCell();
			if (newtlCell != tlCell)
			{
				rect.top = GetFixedRowHeight();
				InvalidateRect(rect);
			}

			if (nSBCode == SB_THUMBPOSITION && !m_noMessage)
			{
				if (scrollPos <= 0)
					MessageToParent(SB_THUMBPOSITION, 0, GVNM_UPVSCROLL);
				else if (min(m_Vscroll, scrollPos+rect.Height()) >= m_Vscroll)
					MessageToParent(SB_THUMBPOSITION, 0, GVNM_DNVSCROLL); 
			}
		}
		break;
        case SB_TOP:
		if (scrollPos > 0)
		{
			SetScrollPos32(SB_VERT, 0);
			Invalidate();
		}
		else if (scrollPos <= 0 && !m_noMessage)
		{
			MessageToParent(SB_TOP, 0, GVNM_UPVSCROLL);
		}
		break;
        case SB_BOTTOM:
		if (scrollPos < m_Vscroll)
		{
			SetScrollPos32(SB_VERT, m_Vscroll);
			Invalidate();

			if (min(m_Vscroll, scrollPos+rect.Height()) >= m_Vscroll && !m_noMessage)
				MessageToParent(SB_BOTTOM, 0, GVNM_DNVSCROLL);
		}
		break;
	default:
		break;
	}
}

void CintGrid::OnKillFocus(CWnd* pNewWnd) 
{
	MessageToParent(m_idCurrent.row, m_idCurrent.col, GVNM_KILLFOCUS);
	CWnd::OnKillFocus(pNewWnd);
}
/////////////////////////////////////////////////////////////////////////////
// CintGrid implementation functions



void CintGrid::OnDraw(CDC* pDC)
{
	CRect rect;
	int row{}, col{};

	CRect clipRect;
	if (pDC->GetClipBox(&clipRect) == ERROR)
		return;



	EraseBkgnd(pDC);
	
	const int nFixedRowHeight = GetFixedRowHeight();
	const int nFixedColWidth  = GetFixedColumnWidth();

//	nFixedRowHeight = nFixedRowHeight * m_yRate;
	
//	TRACE("Height : %d Width : %d\n", nFixedRowHeight, nFixedColWidth);

	const CIdCell tlCell = GetTopleftNonFixedCell();
	const int minVisibleRow = tlCell.row;
	const int minVisibleCol = tlCell.col;
	
	CRect visRect;
	const CRangeCell visCellRange = GetVisibleNonFixedCellRange(visRect);
	const int maxVisibleRow = visCellRange.GetMaxRow();
	const int maxVisibleCol = visCellRange.GetMaxCol();

	// draw top-left cells 0..m_nFixedRows-1, 0..m_nFixedCols-1
	rect.bottom = -1;
	for (row = 0; row < m_nFixedRows; row++)
	{
		rect.top = rect.bottom +1;
//		rect.bottom = rect.top + GetRowHeight(row) * m_yRate -1;
		rect.bottom = rect.top + GetRowHeight(row) -1;
		rect.right = -1;

		for (col = 0; col < m_nFixedCols; col++)
		{
			if (IsCellAttribute(CIdCell(row, col), GVAT_HIDDEN))
				continue;

			rect.left = rect.right +1;
//			rect.right = rect.left + GetColumnWidth(col) * m_xRate -1;
			rect.right = rect.left + GetColumnWidth(col) -1;

			DrawFixedCell(pDC, row, col, rect);
		}
	}
     
	// draw fixed column cells:  m_nFixedRows..n, 0..m_nFixedCols-1
	rect.bottom = nFixedRowHeight -1;
	for (row = minVisibleRow; row <= maxVisibleRow; row++)
	{
		rect.top = rect.bottom +1;
//		rect.bottom = rect.top + GetRowHeight(row) * m_yRate -1;
		rect.bottom = rect.top + GetRowHeight(row) -1;

		if (rect.top > clipRect.bottom) break;
		if (rect.bottom < clipRect.top)	continue;

		rect.right = -1;	

		for (col = 0; col < m_nFixedCols; col++)
		{
			if (IsCellAttribute(CIdCell(row, col), GVAT_HIDDEN))
				continue;

			rect.left  = rect.right+1;
//			rect.right = rect.left + GetColumnWidth(col) * m_xRate -1;
			rect.right = rect.left + GetColumnWidth(col) -1;

			if (rect.left > clipRect.right) 
			{
				break;
			}
			if (rect.right < clipRect.left) continue;

			DrawFixedCell(pDC, row, col, rect);
		}
	}
    
	// draw fixed row cells  0..m_nFixedRows, m_nFixedCols..n
	rect.bottom = -1;
	for (row = 0; row < m_nFixedRows; row++)
	{
		rect.top = rect.bottom+1;
//		rect.bottom = rect.top + GetRowHeight(row)* m_yRate -1;
		rect.bottom = rect.top + GetRowHeight(row) -1;

		if (rect.top > clipRect.bottom)
		{
			break;
		}
		else
		{
		}

		if (rect.bottom < clipRect.top) continue;

		rect.right = nFixedColWidth -1;

		for (col = minVisibleCol; col <= maxVisibleCol; col++)
		{
			if (IsCellAttribute(CIdCell(row, col), GVAT_HIDDEN))
				continue;

			rect.left = rect.right + 1;
//			rect.right = rect.left + GetColumnWidth(col) * m_xRate -1;
			rect.right = rect.left + GetColumnWidth(col) -1;

			if (rect.left > clipRect.right)	
			{
				break;
			}
			else
			{

			}
			if (rect.right < clipRect.left)	continue;

			DrawFixedCell(pDC, row, col, rect);
		}
	}
	
	// draw rest of non-fixed cells
	rect.bottom = nFixedRowHeight-1;	
	for (row = minVisibleRow; row <= maxVisibleRow; row++)
	{
		rect.top = rect.bottom +1;
//		rect.bottom = rect.top + GetRowHeight(row) * m_yRate -1;
		rect.bottom = rect.top + GetRowHeight(row) -1;
		rect.bottom = rect.bottom;

		// rect.bottom = bottom pixel of previous row
		if (rect.top > clipRect.bottom)
		{
			break;
		}
		else
		{
		}

		if (rect.bottom < clipRect.top) continue;


		rect.right = nFixedColWidth -1;

		for (col = minVisibleCol; col <= maxVisibleCol; col++)
		{
			if (IsCellAttribute(CIdCell(row, col), GVAT_HIDDEN))
				continue;

			rect.left = rect.right +1;
//			rect.right = rect.left + GetColumnWidth(col) * m_xRate -1;
			rect.right = rect.left + GetColumnWidth(col) -1;

			if (rect.left > clipRect.right)	
			{
				break;
			}
			if (rect.right < clipRect.left)	continue;

//			TRACE("OnDraw::DrawCell1\n");
			DrawCell(pDC, row, col, rect);
		}
	}

	CPen pen; 
	pen.CreatePen(m_gridLineType, 0, m_gridCOL);
	pDC->SelectObject(&pen); 
	pDC->SetBkMode(TRANSPARENT);

	pDC->SelectStockObject(NULL_BRUSH); 
	pDC->Rectangle(visRect);//BUFFET LINE

	// draw vertical lines (drawn at ends of cells)
	if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_VERT) 
	{
		int xx = 0;
		for (col = 0; col < m_nFixedCols-1; col++)
		{
// 			xx += GetColumnWidth(col) * m_xRate;
			xx += GetColumnWidth(col);
			pDC->MoveTo(xx-1, 0);
			pDC->LineTo(xx-1, visRect.bottom); 
		}

		pDC->MoveTo(0, 0); 
		pDC->LineTo(0, visRect.bottom);
		pDC->MoveTo(nFixedColWidth-1, 0); 
		pDC->LineTo(nFixedColWidth-1, visRect.bottom);

		int x = nFixedColWidth;

		for (col = minVisibleCol; col <= maxVisibleCol; col++)
		{
// 			x += GetColumnWidth(col) * m_xRate;
			x += GetColumnWidth(col);
			pDC->MoveTo(x-1, 0);
			pDC->LineTo(x-1, visRect.bottom);   
		}
	}
 
	CRect rectR;
	GetClientRect(rectR);
	pDC->Rectangle(rectR);
	
	CRect rectS; 
	rectS.SetRectEmpty();


	// draw horizontal lines (drawn at bottom of each cell)
// updateX_20060125(manie)
	pDC->MoveTo(0, nFixedRowHeight-1); 
	pDC->LineTo(visRect.right,  nFixedRowHeight-1);
	if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_HORZ) 
	{
//		pDC->MoveTo(0, 0); pDC->LineTo(visRect.right,  0);
//		pDC->MoveTo(0, nFixedRowHeight-1); pDC->LineTo(visRect.right,  nFixedRowHeight-1);

		int y = nFixedRowHeight;
		for (row = minVisibleRow; row <= maxVisibleRow; row++)
		{
			if ( m_nAlmRow > 0 ) if ( row == m_nAlmRow ) rectS.top = y;//BUFFET

//			y += GetRowHeight(row) * m_yRate;
			y += GetRowHeight(row);
			if ( m_nAlmRow > 0 ) if ( row == m_nAlmRow ) rectS.bottom = y;//BUFFET

			pDC->MoveTo(0, y-1);
			//pDC->LineTo(visRect.right, y-1);//������ �׸��� ����
		}

		rectS.left = visRect.left ;
		rectS.right = visRect.right;//BUFFET
	}

	if ( m_nAlmRow > 0 )
	{
		CPen pena(PS_SOLID, 2, RGB(0,0,255));
		CPen*	oldpen = pDC->SelectObject(&pena);

		pDC->Rectangle(rectS);

		pDC->SelectObject(oldpen);
		pena.DeleteObject();
	}

	// ADD PSH 20070914
	// draw Marker cells
	if (m_bHeadSet)
	{
		rect.bottom = -1;
		rect.top = rect.bottom +1;
//		rect.bottom = rect.top + GetRowHeight(m_nFixedRows - 1) * m_yRate -1;
		rect.bottom = rect.top + GetRowHeight(m_nFixedRows - 1) -1;
		rect.right = visRect.right -1;	
		rect.left = 1;
		rect.bottom = nFixedRowHeight -1;

		const COLORREF oldTextColor = pDC->SetTextColor(m_clrMarkerColor);
		GVITEM gvitem;
		gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_STATE|GVMK_ATTR|GVMK_IMAGE|GVMK_BKCOLOR|GVMK_FGCOLOR|GVMK_PARAM;
		gvitem.col = colNAME;

		LOGFONT *logfont = GetItemFont(1, colNAME);
		CFont mkFont, *pOldFont; 
		mkFont.CreatePointFontIndirect(logfont);
		pOldFont = pDC->SelectObject(&mkFont);

		CRect rcShadow;


		for (row = minVisibleRow; row <= maxVisibleRow; row++)
		{
			rect.top = rect.bottom +1;
//			rect.bottom = rect.top + GetRowHeight(row) * m_yRate -1;
			rect.bottom = rect.top + GetRowHeight(row) -1;

			if (rect.top > clipRect.bottom)
			{
				break;
			}
			else
			{
			}

			if (rect.bottom < clipRect.top)	continue;
			
			if (!IsCellAttribute(CIdCell(row, colNAME), GVAT_MARKER))
				continue;

			gvitem.row = row;

			if (!GetItem(&gvitem)) continue;

			rect.left = 0;

			COLORREF bkCol{};

			if (m_stepUnit && row >= m_nFixedRows)
			{
				bkCol = m_stepCOL1;
				if (((row -1) / m_stepUnit) % 2) 
					bkCol = m_stepCOL2;
				else
					bkCol = m_stepCOL1;
			}

			pDC->FillSolidRect(rect, bkCol);

			if (nullptr != m_pBmpMarker)
			{
				const COLORREF crOldBack = pDC->SetBkColor(RGB(255, 255, 255));
				const COLORREF crOldText = pDC->SetTextColor(RGB(0, 0, 0));
				CDC dcImage, dcTrans;

				// Create two memory dcs for the image and the mask
				dcImage.CreateCompatibleDC(pDC);
				dcTrans.CreateCompatibleDC(pDC);

				// Select the image into the appropriate dc
				CBitmap* pOldBitmapImage = (CBitmap*)dcImage.SelectObject(m_pBmpMarker);

				// Create the mask bitmap
				CBitmap bitmapTrans;
				const int nWidth  = 16;
				const int nHeight = 16;
				bitmapTrans.CreateBitmap(nWidth, nHeight, 1, 1, nullptr);

				// Select the mask bitmap into the appropriate dc
				CBitmap* pOldBitmapTrans = dcTrans.SelectObject(&bitmapTrans);

				// Build mask based on transparent colour
				dcImage.SetBkColor(RGB(255, 255, 255));
				dcTrans.BitBlt(0, 0, nWidth, nHeight, &dcImage, 0, 0, SRCCOPY);

				// Do the work - True Mask method - cool if not actual display
				pDC->BitBlt(rect.left+1, rect.top+1, nWidth, nHeight, &dcImage, 0, 0, SRCINVERT);
				pDC->BitBlt(rect.left+1, rect.top+1, nWidth, nHeight, &dcTrans, 0, 0, SRCAND);
				pDC->BitBlt(rect.left+1, rect.top+1, nWidth, nHeight, &dcImage, 0, 0, SRCINVERT);

				// Restore settings
				dcImage.SelectObject(pOldBitmapImage);
				dcTrans.SelectObject(pOldBitmapTrans);
				pDC->SetBkColor(crOldBack);
				pDC->SetTextColor(crOldText);
				
				ReleaseDC(&dcImage);
				dcImage.DeleteDC();
				ReleaseDC(&dcTrans);
				dcTrans.DeleteDC();

				bitmapTrans.DeleteObject();

				/*
				CDC buffDC;
				buffDC.CreateCompatibleDC(pDC);
				CBitmap* pOldBitmap = buffDC.SelectObject(CBitmap::FromHandle(m_bmpMarker));

				pDC->TRANSPARENT >StretchBlt(rect.left+1, rect.top+1, 16, 16, &buffDC, 0, 0, 16, 16, SRCCOPY);

				buffDC.SelectObject(pOldBitmap);
				*/
			}

//		rect.left += GetColumnWidth(colSIG) * m_xRate + 5;
		rect.left += GetColumnWidth(colSIG) + 5;

			if (m_bMkShadow)
			{
				rcShadow = rect;
				rcShadow.left += 1;
				rcShadow.top += 1;

			//	pDC->SetTextColor(RGB(192, 192, 192));
				pDC->SetTextColor(RGB(255, 0, 0));
				DrawText(pDC->m_hDC, gvitem.text, -1, rcShadow, gvitem.format);
			}

			pDC->SetTextColor(m_clrMarkerColor);
		//	DrawText(pDC->m_hDC, gvitem.text, -1, rect, gvitem.format);
		}

		pDC->SetTextColor(oldTextColor);
		pDC->SelectObject(pOldFont);
	}
	// END ADD

	CFont font, * oldfont{};
	int key{};
	_fixMerge fixMerge;
	for (POSITION pos = m_mapMerge.GetStartPosition(); pos != nullptr; )
	{
		m_mapMerge.GetNextAssoc(pos, key, (_fixMerge &)fixMerge);

		pDC->FillSolidRect(fixMerge.rect, fixMerge.bkcol == CLR_DEFAULT ? m_fixbkCOL : fixMerge.bkcol);

		pDC->MoveTo(fixMerge.rect.right,fixMerge.rect.top);
		pDC->LineTo(fixMerge.rect.left, fixMerge.rect.top);
		pDC->LineTo(fixMerge.rect.left, fixMerge.rect.bottom);

		pDC->MoveTo(fixMerge.rect.right, fixMerge.rect.top);
		pDC->LineTo(fixMerge.rect.right, fixMerge.rect.bottom);
		pDC->LineTo(fixMerge.rect.left,  fixMerge.rect.bottom);

		fixMerge.rect.DeflateRect(1+m_margin, 1);

		font.CreatePointFontIndirect(&fixMerge.font);
		oldfont = pDC->SelectObject(&font);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(fixMerge.fgcol);
		fixMerge.rect.top += 2;

		DrawText(pDC->m_hDC, fixMerge.text, -1, fixMerge.rect, fixMerge.format);

		pDC->SelectObject(oldfont); 
		font.DeleteObject();
	}

	pDC->SelectStockObject(NULL_PEN);

	pen.DeleteObject();
}


BOOL CintGrid::DrawFixedCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk)
{
	if (nRow)
	{
// 		TRACE("DrawFixedCell::DrawCell\n");
		DrawCell(pDC, nRow, nCol, rect, bEraseBk);
		return TRUE;
	}

	CString	str;
	CWnd* pTool = nullptr;
	BOOL	bExpect = FALSE;

	if (nCol == colSIG)
	{
		// MODIFY PSH 20070911
		/*
		DrawSise(pDC, rect, true);
		DrawNews(pDC, rect);
		*/
		if (m_bHeadSet)
		{
			//DrawSise(pDC, rect, true);	
			DrawHeaderNews(pDC, rect);
		}
		else
		{
			DrawSig(pDC, rect, nRow);
		}
		// END MODIFY

		CPen	pen(PS_SOLID, 1, RGB(200, 200, 200));
		CPen*	oldpen = pDC->SelectObject(&pen);
		pDC->MoveTo(rect.left, rect.bottom);
		//pDC->LineTo(rect.right, rect.top);
		pDC->SelectObject(oldpen);
		pen.DeleteObject();
		return TRUE;
	}
	// ADD PSH 20070913
	else if (nCol == colCURR)
	{
		str = GetItemText(nRow, colEXPECT);
		pTool = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		bExpect = pTool->SendMessage(WM_MANAGE, MK_EXPECT);

		if (bExpect || atoi(str.GetBuffer(0)))
		{
			pDC->SetTextColor(RGB(150, 150, 150));
		}
	}
	// END ADD

	GVITEM gvitem;

	gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_IMAGE|GVMK_BKCOLOR|GVMK_FGCOLOR|GVMK_STATE|GVMK_ATTR|GVMK_PARAM|GVMK_MISC;
	gvitem.row  = nRow;
	gvitem.col  = nCol;

	if (!GetItem(&gvitem)) return FALSE;

	const int savedDC = pDC->SaveDC();

	// HDRMERGE column rectangle calculate
	if ((gvitem.attr & GVAT_HDRMERGE) == GVAT_HDRMERGE)
	{
		rect.top = rect.top + (rect.Height() / 2);

		CRect Rect;
		_fixMerge fixMerge;
		if (m_mapMerge.Lookup(nCol, fixMerge))
		{
			for (int ii = 0; ii < maxMERGE; ii++)
			{
				if (fixMerge.cols[ii] == -1)
					break;
				if (!GetCellRect(nRow, fixMerge.cols[ii], Rect))
					continue;
				
				if (!ii)
				{
					Rect.bottom = Rect.bottom - (Rect.Height() / 2);
					if (nCol > m_nFixedCols) Rect.left  -= 1;
					fixMerge.rect = Rect;
				}
				else
				{
					fixMerge.rect.right = Rect.right;
				}
			}

			fixMerge.rect.bottom -= 2;
			m_mapMerge.SetAt(nCol, fixMerge);
		}
	}

	bEraseBk = TRUE;
	if (gvitem.state & GVST_DROPHILITED && !pDC->IsPrinting())
	{
		rect.right++; 
		rect.bottom++;
		pDC->SelectStockObject(BLACK_PEN);
		pDC->SelectStockObject(LTGRAY_BRUSH); //NULL_BRUSH);
		pDC->Rectangle(rect);
		rect.right--; 
		rect.bottom--;
	}
	else if (gvitem.state & GVST_FOCUSED && !pDC->IsPrinting()) 
	{
		rect.right++; rect.bottom++;	// FillRect doesn't draw RHS or bottom
		if (bEraseBk) 
		{
			const COLORREF color = (gvitem.bkcol == CLR_DEFAULT) ? m_fixbkCOL : gvitem.bkcol;
			CBrush brush(color); 
			pDC->FillRect(rect, &brush);
			brush.DeleteObject();
		}

		rect.right--; 
		rect.bottom--;

		CRect	chkRC = rect;
		if (chkRC.left <= 0) chkRC.left = 1;
		pDC->SelectStockObject(BLACK_PEN);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(chkRC);
	}
	else if (m_selectLine && gvitem.state & GVST_SELECTED && !pDC->IsPrinting() && nRow >= m_nFixedRows)
	{
		rect.right++;
		rect.bottom++;	// FillRect doesn't draw RHS or bottom
		pDC->FillSolidRect(rect, m_bkSELCOL);
		rect.right--; 
		rect.bottom--;
		pDC->SetTextColor(PALETTERGB(255, 255, 255));
	}
	else if (bEraseBk && !pDC->IsPrinting())
	{
		rect.right++; 
		rect.bottom++;
		const COLORREF color = (gvitem.bkcol == CLR_DEFAULT) ? m_fixbkCOL : gvitem.bkcol;
		pDC->FillSolidRect(rect, color);
		rect.right--; 
		rect.bottom--;
	}

	pDC->SetTextColor((gvitem.fgcol == CLR_DEFAULT) ? m_fixfgCOL : gvitem.fgcol);
    
	LOGFONT *logfont  = GetItemFont(nRow, nCol);
	const CIdCell focusCell = GetFocusCell();

	if (false && (focusCell.row == nRow || focusCell.col == nCol))
	{
		static LOGFONT lf;
		memcpy(&lf, logfont, sizeof(LOGFONT));
		lf.lfWeight = selcWEIGHT;
		logfont = &lf;
	}
   
	CFont font; font.CreatePointFontIndirect(logfont);
	pDC->SelectObject(&font); 
	pDC->SetBkMode(TRANSPARENT);

	rect.DeflateRect(m_margin+1, 1);

	if (m_imageList && gvitem.image >= 0)
	{
		IMAGEINFO Info;
		if (m_imageList->GetImageInfo(gvitem.image, &Info))
		{
			const int nImageWidth  = Info.rcImage.right - Info.rcImage.left+1;
			const int nImageHeight = Info.rcImage.bottom - Info.rcImage.top;

			const CPoint	drawPT(rect.left, rect.top + ((rect.Height() - nImageHeight) / 2));

			m_imageList->Draw(pDC, gvitem.image, drawPT, ILD_NORMAL);
			rect.left += nImageWidth + m_margin;
		}
	}

	if ((gvitem.attr & GVAT_CHECK) == GVAT_CHECK)
	{
		const int nWidth = 15;
		const int nHeight = 15;
		const int topx = (rect.Height() - nHeight) / 2;
		CRect	checkRC(rect.left, rect.top+topx, rect.left+nWidth, rect.top+topx+nHeight);
		if (gvitem.text.GetLength() <= 0)
		{
			const 	int offset = (rect.Width() - nWidth) / 2;
			checkRC.SetRect(rect.left+offset, rect.top+topx, rect.left+offset+nWidth, rect.top+topx+nHeight);
		}

		UINT state = DFCS_BUTTONCHECK;

		switch (int(gvitem.param))
		{
		case  1: state |= DFCS_CHECKED;		break;
		case -1: state |= DFCS_INACTIVE;	break;
		}
		pDC->DrawFrameControl(checkRC, DFC_BUTTON, state);
		rect.left += nWidth+m_margin;
	}

	CString string = gvitem.text;
	if ((gvitem.attr & GVAT_CONTRAST) == GVAT_CONTRAST && !string.IsEmpty())
	{
		CString	temps;
		switch (string[0])
		{
		case '1':
// updateX_20060109
//			temps = ARR_UP; gvitem.fgcol = m_upCOL; break;
			temps = ARR_UP; 
			gvitem.bkcol = m_upCOL; 
			gvitem.fgcol = m_fgSELCOL;

			if (m_selectLine && (gvitem.state & GVST_SELECTED) && !(gvitem.state & GVST_FOCUSED))
			{
				gvitem.fgcol = m_upCOL; 
				gvitem.bkcol = m_bkSELCOL;
			}
			rect.InflateRect(m_margin, 0); 
			pDC->FillSolidRect(rect, gvitem.bkcol); 
			rect.DeflateRect(m_margin, 0);
			break;
		case '+':
		case '2':
			temps = TRI_UP; gvitem.fgcol = m_upCOL; break;
		case '4':
// updateX_20060109
//			temps = ARR_DN; gvitem.fgcol = m_dnCOL; break;
			temps = ARR_DN; 
			gvitem.bkcol = m_dnCOL;
			gvitem.fgcol = m_fgSELCOL;

			if (m_selectLine && (gvitem.state & GVST_SELECTED) && !(gvitem.state & GVST_FOCUSED))
			{
				gvitem.fgcol = m_dnCOL;
				gvitem.bkcol = m_bkSELCOL;
			}
			rect.InflateRect(m_margin, 0); 
			pDC->FillSolidRect(rect, gvitem.bkcol); 
			rect.DeflateRect(m_margin, 0);
			break;
		case '-':
		case '5':
			temps = TRI_DN; 
			gvitem.fgcol = m_dnCOL; 
			break;
		default:
			temps = _T("  "); 
			gvitem.fgcol = m_eqCOL; 
			break;
		}

		const CSize size = pDC->GetOutputTextExtent(temps);
		COLORREF clr{};

		pDC->SetTextColor(gvitem.fgcol);
// updateX_20060109
		if (false && m_selectLine && (gvitem.state & GVST_SELECTED) && !(gvitem.state & GVST_FOCUSED))
		{
			clr = m_fgSELCOL;
			pDC->SetTextColor(m_fgSELCOL);
		}
		clr = gvitem.fgcol;

		const int topx = (rect.Height() - size.cy) / 2;
		const CRect	contRC(rect.left, rect.top+topx, rect.left+size.cx, rect.top+topx+size.cy);

		drawContrast(pDC, contRC, temps, clr);
		string = string.Mid(1);
		rect.left += size.cx+m_margin;
	}

	COLORREF rgb{};

	if (0 != nRow && colNAME == nCol)
	{
		if (!m_bMKClrApply)
		{
			rgb = m_eqCOL;
		}
		else
		{
			if (1 == GetItemData(nRow, colCODE))
				rgb = m_clrKospi;
			else
				rgb = m_clrKosdaq;
		}

		if (!string.IsEmpty() && (string.FindOneOf("+- ") != -1))
		{
			string.Delete(0);
		}
	}
	else
	{
		rgb = DisplayAttribute(gvitem.attr, string);
	}

	if (rgb != (COLORREF) 0)
	{
		gvitem.fgcol = rgb; pDC->SetTextColor(rgb);
// updateX_20090109
		if (false && m_selectLine && (gvitem.state & GVST_SELECTED) && !(gvitem.state & GVST_FOCUSED))
		{
			pDC->SetTextColor(m_fgSELCOL);
		}
	}

	if (m_sortColumn != -1 && m_sortColumn == nCol)
	{
		if (m_ascending == 1)
			string.Insert(0, "��");
		else if (m_ascending ==	0)
			string.Insert(0, "��");

	}

	// ADD PSH 20070912
	rect.top += 1;
	rect.bottom += 1;

	if (!string.IsEmpty() && (nCol == colNAME) && !m_bHeadSet)
	{
		if (m_bMargin)
		{
			CRect rcMRatio = rect;
			rcMRatio.left -= 1;
			rcMRatio.right = rcMRatio.left + 16;
			rcMRatio.top++;
			rcMRatio.bottom--;

			rect.left += 14;

			DrawMarginRatio(pDC, rcMRatio, nRow);
		}

		if (m_bInfo)
		{
			CBitmap* pInfo = nullptr;

			switch(GetItemData(nRow, colINFO))
			{
			case 1:
				pInfo = m_pBmpInfo1;
				break;
			case 2:
				pInfo = m_pBmpInfo2;
				break;
			case 3:
				pInfo = m_pBmpInfo3;
				break;
			case 4:
				pInfo = m_pBmpInfo4;
				break;
			case 5:
				pInfo = m_pBmpInfo5;
				break;
			case 6:
				pInfo = m_pBmpInfo6;
				break;
// 			case 7:
// 				pInfo = m_pBmpInfo7;
// 				break;
// 			case 8:
// 				pInfo = m_pBmpInfo8;
// 				break;
// 			case 9:
// 				pInfo = m_pBmpInfo9;
// 				break;
// 			case 10:
// 				pInfo = m_pBmpInfo10;
// 				break;
// 			case 11:
// 				pInfo = m_pBmpInfo11;
// 				break;
			case 12:
				pInfo = m_pBmpInfo12;
				break;
			case 13:
				pInfo = m_pBmpInfo13;
				break;
			case 14:
				pInfo = m_pBmpInfo14;
				break;
			case 15:
				pInfo = m_pBmpInfo15;
				break;
			case 16:
				pInfo = m_pBmpInfo16;
				break;
			case 17:
				pInfo = m_pBmpInfo17;
				break;
			case 18:
				pInfo = m_pBmpInfo18;
				break;
			case 19:
				pInfo = m_pBmpInfo19;
				break;
			case 20:
				pInfo = m_pBmpInfo20;
				break;
			case 21:
				pInfo = m_pBmpInfo21;
				break;
			case 22:
				pInfo = m_pBmpInfo22;
				break;
			case 23:
				pInfo = m_pBmpInfo23;
				break;
			}

			if (nullptr != pInfo)
			{
				CDC buffDC;
				buffDC.CreateCompatibleDC(pDC);
				CBitmap* pOldBitmap = buffDC.SelectObject(pInfo);

				pDC->StretchBlt(rect.left+1, rect.top+1, 14, 14, &buffDC, 0, 0, 14, 14, SRCCOPY);

				rect.left += 17;
				rect.right += 17;

				buffDC.SelectObject(pOldBitmap);

				buffDC.DeleteDC();
			}
			else
			{
				rect.left += 2;
				rect.right += 2;
			}
		}
		else
		{
			rect.left += 2;
			rect.right += 2;
		}
	}
	// END ADD

	// updateX_20051018
	if ((++rect.top < rect.bottom) && !(nCol == colCODE && 0 == m_aryColWidth[nCol]))
	{
		DrawText(pDC->m_hDC, string, -1, rect, gvitem.format);
	}

	//2011.12.22 KSJ �ڽ����϶��� ���� �׸����� �ڵ带 ���´�.
	CString strCode = GetItemText(nRow, colCODE);

	if(nCol == colCURR && (bExpect || atoi(str)) && string != "����" && strCode.GetLength() > 0 /*&& strCode.GetLength() == 6*/)	//2011.12.22 KSJ �ڵ���̰� 6�϶��� ���� �׸����� 2013.08.05 KSJ ����, ����, �ֽļ����϶��� ���� ǥ���ϵ��� ��û����.
	{		
		CDC buffDC;
		buffDC.CreateCompatibleDC(pDC);
		CBitmap* pOldBitmap = buffDC.SelectObject(m_pBmpExpect);
		
		pDC->StretchBlt(rect.left-1, rect.top+3, 10, 10, &buffDC, 0, 0, 10, 10, SRCCOPY);
		
		// 					rect.left += 17;
		// 					rect.right += 17;
		
		buffDC.SelectObject(pOldBitmap);
		
		buffDC.DeleteDC();
	}

	pDC->RestoreDC(savedDC);
	font.DeleteObject();

	return TRUE;
}

#include "../../h/interst.h"

BOOL CintGrid::DrawCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk)
{
	if (nCol == colCODE && 0 == m_aryColWidth[nCol]) return TRUE;

	GVITEM gvitem;
	const CRect	rcOrg = rect;

	gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_STATE|GVMK_ATTR|GVMK_IMAGE|GVMK_BKCOLOR|GVMK_FGCOLOR|GVMK_PARAM;
	gvitem.row = nRow;
	gvitem.col = nCol;

	if (!GetItem(&gvitem)) return FALSE;

	COLORREF rtmCol= -1;
	const COLORREF fgCol = (gvitem.fgcol == CLR_DEFAULT) ? m_fgCOL : gvitem.fgcol;
	COLORREF bkCol = (gvitem.bkcol == CLR_DEFAULT) ? m_bkCOL : gvitem.bkcol;
	
	const COLORREF bkColExpect = PALETTERGB(150,150,150);

	if (nRow > 0)
	{
		const auto& pInter = m_pParent->GetData(nRow - 1);   //test Ȯ��
		if (pInter)
		{
			if (pInter->bookmark == '1')
			{
				//fgCol = m_pParent->GetBookMarkPenColor();	
				bkCol = m_pParent->GetBookMarkColor();
			}
			else
			{
				//		TRACE("DrawCell :: gvitem.attr -> %d\n", gvitem.attr);
				if (m_stepUnit && nRow >= m_nFixedRows)
				{
					bkCol = m_stepCOL1;
					if (((nRow - 1) / m_stepUnit) % 2) bkCol = m_stepCOL2;
				}
			}
		}
	}
	CString	str = GetItemText(nRow, colEXPECT);
	CWnd* pTool = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const BOOL	bExpect = pTool->SendMessage(WM_MANAGE, MK_EXPECT);


	bEraseBk = TRUE;
	const int savedDC = pDC->SaveDC();

	pDC->SetBkMode(TRANSPARENT);
	CRect	visRect; 
	GetVisibleNonFixedCellRange(visRect);
	UINT	idEvent = 0;
	struct _timerID* timerID = (struct _timerID*)&idEvent;
	timerID->kind = 0;
	timerID->row = nRow;
	timerID->col = nCol;
	

	if (gvitem.state & GVST_FOCUSED && !pDC->IsPrinting()) 
	{
		if (nRow == 0 && nRow == m_nFixedRows)
			rect.top += 1;

		rect.right++;
		rect.bottom++;	// FillRect doesn't draw RHS or bottom
		if (bEraseBk) 
		{
			CBrush brush(bkCol);
			pDC->FillRect(rect, &brush);
			brush.DeleteObject();
		}
		rect.right--; 
		rect.bottom--;

		// updateX_20050530
		int visibleCol = 0;
		for (visibleCol = 0; visibleCol < m_nCols; visibleCol++)
		{
			if (IsCellVisible(nRow, visibleCol))
				break;
		}
		if (visibleCol == nCol) rect.left += 1;

		pDC->SelectStockObject(BLACK_PEN);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(rect);
		pDC->SetTextColor(fgCol);

		if (visibleCol == nCol) rect.left -= 1;
		if (nRow == 0 && nRow == m_nFixedRows)
			rect.top -= 1;
	}
	else if (m_mapRTM.Lookup(idEvent, rtmCol))
	{
		rect.right++; 

		rect.bottom++;	// FillRect doesn't draw RHS or bottom
		if (bEraseBk) 
		{
			CBrush brush(bkCol);
			pDC->FillRect(rect, &brush);
			brush.DeleteObject();
		}
		rect.right--; 
		rect.bottom--;
		pDC->SetTextColor(fgCol);
	}
	else if (m_selectLine && gvitem.state & GVST_SELECTED && !pDC->IsPrinting())
	{
		rect.right++; 
		rect.bottom++;	// FillRect doesn't draw RHS or bottom
		pDC->FillSolidRect(rect, m_bkSELCOL);
		rect.right--; 
		rect.bottom--;
// updateX_20060109
//		fgCol = m_fgSELCOL;
		pDC->SetTextColor(fgCol);
	}
	else
	{
		rect.right++; 
		rect.bottom++;	// FillRect doesn't draw RHS or bottom
		if (bEraseBk) 
		{
			CBrush brush(bkCol);
			pDC->FillRect(rect, &brush);
			brush.DeleteObject();
		}
		rect.right--; 
		rect.bottom--;
		pDC->SetTextColor(fgCol);
	}

	if (gvitem.state & GVST_DROPHILITED && !pDC->IsPrinting())
	{
		pDC->SelectStockObject(BLACK_PEN);
		pDC->SelectStockObject(LTGRAY_BRUSH); //NULL_BRUSH);
		pDC->Rectangle(rect);
	}

	// 	VERIFY(font.CreateFont(
	// 		MulDiv(16,m_iXP,m_iXD),                        // nHeight
	// 		0,                         // nWidth
	// 		0,                         // nEscapement
	// 		0,                         // nOrientation
	// 		FW_BOLD,                 // nWeight
	// 		FALSE,                     // bItalic
	// 		FALSE,                     // bUnderline
	// 		0,                         // cStrikeOut
	// 		ANSI_CHARSET,              // nCharSet
	// 		OUT_DEFAULT_PRECIS,        // nOutPrecision
	// 		CLIP_DEFAULT_PRECIS,       // nClipPrecision
	// 		DEFAULT_QUALITY,           // nQuality
	// 		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	//      "����ü"));                 // lpszFacename
	
//	font.CreatePointFont(m_point*10, "����ü");
	
	LOGFONT *logfont = GetItemFont(nRow, nCol);

	CFont font; 
	
	logfont->lfHeight = m_point * 4 + 54;
	font.CreatePointFontIndirect(logfont);

	pDC->SelectObject(font);

	if (m_imageList && gvitem.image >= 0)
	{
		IMAGEINFO Info;
		if (m_imageList->GetImageInfo(gvitem.image, &Info))
		{
			const int nImageWidth = Info.rcImage.right - Info.rcImage.left+1;
			const int nImageHeight = Info.rcImage.bottom - Info.rcImage.top;

			const CPoint	drawPT(rect.left, rect.top + ((rect.Height() - nImageHeight) / 2));

			m_imageList->Draw(pDC, gvitem.image, drawPT, ILD_NORMAL);
			rect.left += nImageWidth+m_margin;
		}
	}

	rect.DeflateRect(m_margin, 0);
	
	if ((gvitem.attr & GVAT_CHECK) == GVAT_CHECK)
	{
		const int nWidth = 15;
		const int nHeight = 15;
		const int topx = (rect.Height() - nHeight) / 2;
		CRect	checkRC(rect.left, rect.top+topx, rect.left+nWidth, rect.top+topx+nHeight);
		if (gvitem.text.GetLength() <= 0)
		{
			const 	int offset = (rect.Width() - nWidth) / 2;
			checkRC.SetRect(rect.left+offset, rect.top+topx, rect.left+offset+nWidth, rect.top+topx+nHeight);
		}

		const UINT state = (gvitem.param) ? DFCS_BUTTONCHECK|DFCS_CHECKED : DFCS_BUTTONCHECK;
		pDC->DrawFrameControl(checkRC, DFC_BUTTON, state);
		rect.left += nWidth+m_margin;
	}

	CString string = gvitem.text;
	if ((gvitem.attr & GVAT_CONTRAST) == GVAT_CONTRAST && !string.IsEmpty())
	{
		CString	temps;
		switch (string[0])
		{
		case '1':
// updateX_20060109
//			temps = ARR_UP; gvitem.fgcol = m_upCOL; break;
			temps = ARR_UP; gvitem.bkcol = m_upCOL; gvitem.fgcol = m_fgSELCOL;
			if (rtmCol == -1 && m_selectLine && (gvitem.state & GVST_SELECTED) && !(gvitem.state & GVST_FOCUSED))
			{
				gvitem.fgcol = m_upCOL; 
				gvitem.bkcol = m_bkSELCOL;
			}
			rect.InflateRect(m_margin, 0); 
			pDC->FillSolidRect(rect, gvitem.bkcol);
			rect.DeflateRect(m_margin, 0);
			break;
		case '+':
		case '2':
			temps = TRI_UP; gvitem.fgcol = m_upCOL; break;
		case '4':
// updateX_20060109
//			temps = ARR_DN; gvitem.fgcol = m_dnCOL; break;
			temps = ARR_DN; gvitem.bkcol = m_dnCOL; gvitem.fgcol = m_fgSELCOL;
			if (rtmCol == -1 && m_selectLine && (gvitem.state & GVST_SELECTED) && !(gvitem.state & GVST_FOCUSED))
			{
				gvitem.fgcol = m_dnCOL; gvitem.bkcol = m_bkSELCOL;
			}
			rect.InflateRect(m_margin, 0); pDC->FillSolidRect(rect, gvitem.bkcol); rect.DeflateRect(m_margin, 0);
			break;
		case '-':
		case '5':
			temps = TRI_DN; gvitem.fgcol = m_dnCOL; break;
		default:
			temps = _T("  "); gvitem.fgcol = m_eqCOL; break;
		}

		const CSize size = pDC->GetOutputTextExtent(temps);
		COLORREF clr = gvitem.fgcol;

		pDC->SetTextColor(gvitem.fgcol);
// updateX_20060109
		if (false && rtmCol == -1 && m_selectLine && (gvitem.state & GVST_SELECTED) && !(gvitem.state & GVST_FOCUSED))
		{
			clr = m_fgSELCOL;
			pDC->SetTextColor(m_fgSELCOL);
		}
		clr = gvitem.fgcol;

		const int topx = (rect.Height() - size.cy) / 2;
		const CRect	contRC(rect.left, rect.top+topx, rect.left+size.cx, rect.top+topx+size.cy);

		drawContrast(pDC, contRC, temps, clr);
		string = string.Mid(1);
		rect.left += size.cx+m_margin;
	}

	// MODIFY PSH 20070917
	//COLORREF rgb = DisplayAttribute(gvitem.attr, string);
	CString strDiff;
	const int nIdx = string.Find("(", 0);
	if (-1 != nIdx)
	{
		strDiff = string.Mid(nIdx);
		string = string.Mid(0, nIdx);
	}

	COLORREF rgb{};

	if (0 != nRow && colNAME == nCol)
	{
		if (!m_bMKClrApply)
		{
			rgb = m_eqCOL;
		}
		else
		{
			CGridWnd* pWnd = (CGridWnd*)GetParent();
			CString strTitle = pWnd->GetTitle();
			if (strTitle.Find(_T(" [��������] - �ŷ���")) != -1)
			{
				rgb = m_clrKospi;
			}
			else if (strTitle.Find(_T(" [��������] - �ڽ���")) != -1)
			{
				rgb = m_clrKosdaq;
			}
			else
			{					
				if (0 == GetItemData(nRow, colCODE))
					rgb = m_clrKosdaq;
				else if (1 == GetItemData(nRow, colCODE))
					rgb = m_clrKospi;
				else
					rgb = m_eqCOL;
			}		
		}
		
		string.TrimLeft(); 
		const int isignFind = string.FindOneOf("+-");

		if (!string.IsEmpty() && (isignFind == 0))
			string.Delete(0);
		string.TrimLeft(); 
		string.TrimRight(); 

//		if (!string.IsEmpty() && (string.FindOneOf("+- ") != -1))
//		{
//			string.Delete(0);
//		}
	}
	else
	{
		rgb = DisplayAttribute(gvitem.attr, string);
	}

	if (-1 != nIdx)
	{
		string += strDiff;
	}
	// END MODIFY

	if (rgb != (COLORREF) 0)
	{
		gvitem.fgcol = rgb; pDC->SetTextColor(rgb);
// updateX_20090109
		if (false && rtmCol == -1 && m_selectLine && (gvitem.state & GVST_SELECTED) && !(gvitem.state & GVST_FOCUSED))
		{
			pDC->SetTextColor(m_fgSELCOL);
		}
	}

	if ((gvitem.format & GVFM_MULTI) == GVFM_MULTI) rect.DeflateRect(0, 4);
	if (rect.top < rect.bottom)
	{
		CString	symbol = GetItemSymbol(nRow, nCol);

		if (symbol.Compare("7777") == 0)
			DrawBong(pDC, rcOrg, nRow, nCol);

/*		2014.13.28 KSJ ü������������ ����ٰ� ������ ����̹Ƿ� �ּ�ó����.
		else if (symbol.Compare("7778") == 0)	// ü�� ����������
		{
			DrawEqualizer(pDC, rcOrg, nRow, nCol);
		}
*/
		else if (symbol.Compare("###3") == 0 || symbol.Compare("###4") == 0)
		{
			CString	str = GetItemText(nRow, colEXPECT);
//			if (!atoi(str))
			{
				DrawText(pDC->m_hDC, string, -1, rect, gvitem.format);
			}
		}
		else
		{
			BOOL bDrawExpect = FALSE;

			if (nCol == colSIG)
			{
				DrawSig(pDC, rcOrg, nRow);
			}
			else
			{
				rect.top += 1;
				rect.bottom += 1;

				if (nCol == colCURR)
				{
// 					CString	str = GetItemText(nRow, colEXPECT);
// 					CWnd* pTool = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
// 					BOOL	bExpect = pTool->SendMessage(WM_MANAGE, MK_EXPECT);
					if (bExpect || atoi(str))
					{
						bDrawExpect = TRUE;
						//DrawExpect(pDC, rect, string);
//						pDC->SetTextColor(RGB(150, 150, 150));
						//����ü�ᰡ ���ڻ� ����
//						pDC->SetTextColor(PALETTERGB(71, 76, 80));
					}
				}
				// ADD PSH 20070912
				else if (!string.IsEmpty() && (nCol == colNAME))
				{					
					if (m_bMargin)
					{
						CRect rcMRatio = rect;
						rcMRatio.left -= 1;
						rcMRatio.right = rcMRatio.left + 16;
						rcMRatio.top++;
						rcMRatio.bottom-=3;

						rect.left += 14;

						DrawMarginRatio(pDC, rcMRatio, nRow);
					}

					if (m_bInfo)
					{
						CBitmap* pInfo = nullptr;

						switch(GetItemData(nRow, colINFO))
						{
							// 2011.12.06 KSJ
						case 0 :
							pInfo = nullptr;
							break;
							//KSJ
						case 1:
							pInfo = m_pBmpInfo1;
							break;
						case 2:
							pInfo = m_pBmpInfo2;
							break;
						case 3:
							pInfo = m_pBmpInfo3;
							break;
						case 4:
							pInfo = m_pBmpInfo4;
							break;
						case 5:
							pInfo = m_pBmpInfo5;
							break;
						case 6:
							pInfo = m_pBmpInfo6;
							break;
						case 7:
							pInfo = m_pBmpInfo7;
							break;
						case 8:
							pInfo = m_pBmpInfo8;
							break;
						case 9:
							pInfo = m_pBmpInfo9;
							break;
						case 10:
							pInfo = m_pBmpInfo10;
							break;
						case 11:
							pInfo = m_pBmpInfo11;
							break;
						case 12:
							pInfo = m_pBmpInfo12;
							break;
						case 13:
							pInfo = m_pBmpInfo13;
							break;
						case 14:
							pInfo = m_pBmpInfo14;
							break;
						case 15:
							pInfo = m_pBmpInfo15;
							break;
						case 16:
							pInfo = m_pBmpInfo16;
							break;
						case 17:
							pInfo = m_pBmpInfo17;
							break;
						case 18:
							pInfo = m_pBmpInfo18;
							break;
						case 19:
							pInfo = m_pBmpInfo19;
							break;
						case 20:
							pInfo = m_pBmpInfo20;
							break;
						case 21:
							pInfo = m_pBmpInfo21;
							break;
						case 22:
							pInfo = m_pBmpInfo22;
							break;
						case 23:
							pInfo = m_pBmpInfo23;
							break;
						}

						if (nullptr != pInfo)
						{
							CDC buffDC;
							buffDC.CreateCompatibleDC(pDC);
							CBitmap* pOldBitmap = buffDC.SelectObject(pInfo);
							
							pDC->StretchBlt(rect.left+1, rect.top+1, 14, 14, &buffDC, 0, 0, 14, 14, SRCCOPY);
							
							rect.left += 17;
							rect.right += 17;
							
							buffDC.SelectObject(pOldBitmap);

							buffDC.DeleteDC();
						}
						else
						{
							rect.left += 2;
							rect.right += 2;
						}
					}
					else
					{
						rect.left += 2;
						rect.right += 2;
					}
				}
				// END ADD
				
				if (gvitem.attr & GVAT_SIGNx)
				{
					if(string.GetLength() )
					{
						if(string.Left(1) == "+") 
							string = string.Right(string.GetLength() - 1);
					}
				}

				DrawText(pDC->m_hDC, string, -1, rect, gvitem.format);

				//2011.12.27 KSJ �ڽ����϶��� ���� �׸����� �ڵ带 ���´�.
				CString strCode = GetItemText(nRow, colCODE);
				
				if(nCol == colCURR && (bExpect || atoi(str)) && strCode.GetLength() > 0/*&& strCode.GetLength() == 6*/)	//2011.12.22 KSJ �ڵ���̰� 6�϶��� ���� �׸����� 2013.08.05 KSJ ����, ����, �ֽļ����϶��� ���� ǥ���ϵ��� ��û����.
				{
					bDrawExpect = FALSE;
					
					CDC buffDC;
					buffDC.CreateCompatibleDC(pDC);
					CBitmap* pOldBitmap = buffDC.SelectObject(m_pBmpExpect);
					
					pDC->StretchBlt(rect.left-1, rect.top+3, 10, 10, &buffDC, 0, 0, 10, 10, SRCCOPY);
					
					
					buffDC.SelectObject(pOldBitmap);
					
					buffDC.DeleteDC();
				}	
			}
		}	
	}

	pDC->RestoreDC(savedDC);
	font.DeleteObject();
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////
// CintGrid Cell selection stuff
BOOL CintGrid::IsValid(const CIdCell& cell) const
{
	return (cell.row >= 0 && cell.row < m_nRows && cell.col >= 0 && cell.col < m_nCols);
}

BOOL CintGrid::IsValid(const CRangeCell& range) const
{
	return (range.GetMinRow() >= 0 && range.GetMinCol() >= 0
			&& range.GetMaxRow() >= 0 && range.GetMaxCol() >= 0
			&& range.GetMaxRow() < m_nRows && range.GetMaxCol() < m_nCols
			&& range.GetMinRow() <= range.GetMaxRow() && range.GetMinCol() <= range.GetMaxCol());
}

BOOL CintGrid::RedrawCell(const CIdCell& cell, CDC* pDC)
{
	return RedrawCell(cell.row, cell.col, pDC);
}

BOOL CintGrid::RedrawCell(int nRow, int nCol, CDC* pDC)
{
	if (!GetSafeHwnd())
		return FALSE;

	const BOOL bResult = TRUE;
	const BOOL bMustReleaseDC = FALSE;

	const CIdCell tlCell = GetTopleftNonFixedCell();
	const int minVisibleRow = tlCell.row;
	const int minVisibleCol = tlCell.col;

	CRect visRect;
	const CRangeCell visCellRange = GetVisibleNonFixedCellRange(visRect);
	const int maxVisibleRow = visCellRange.GetMaxRow();
	const int maxVisibleCol = visCellRange.GetMaxCol();

	if (IS_WITHIN(minVisibleRow, maxVisibleRow, nRow) && IS_WITHIN(minVisibleCol, maxVisibleCol, nCol))
	{
		if (!IsCellVisible(nRow, nCol))
			return FALSE;

		CRect rect;
		if (!GetCellRect(nRow, nCol, rect))
			return FALSE;

		InvalidateRect(rect, FALSE);
		return TRUE;
	}
	return FALSE;
}

CIdCell CintGrid::SetFocusCell(int nRow, int nCol)
{
	return SetFocusCell(CIdCell(nRow, nCol));
}

CIdCell CintGrid::SetFocusCell(CIdCell focusCell)
{
	if (focusCell == m_idCurrent) 
		return m_idCurrent;

	CIdCell prevCell = m_idCurrent;
	m_idCurrent = focusCell;

	if (IsValid(prevCell)) 
	{
		MessageToParent(prevCell.row, prevCell.col, GVNM_SELCHANGING); 

		SetItemState(prevCell.row, prevCell.col,
			GetItemState(prevCell.row, prevCell.col) & ~GVST_FOCUSED);
		RedrawCell(prevCell);

		if (prevCell.col != m_idCurrent.col)
		{
			for (int row = 0; row < m_nFixedRows; row++)
				RedrawCell(row, prevCell.col);
		}

		if (prevCell.row != m_idCurrent.row)
		{
			for (int col = 0; col < m_nFixedCols; col++) 
				RedrawCell(prevCell.row, col);
		}
	}

	if (IsValid(m_idCurrent))
	{
		SetItemState(m_idCurrent.row, m_idCurrent.col,
			GetItemState(m_idCurrent.row, m_idCurrent.col)|(m_focusLine ? GVST_FOCUSED : 0));
		RedrawCell(m_idCurrent);

		if (prevCell.col != m_idCurrent.col)
		{
			for (int row = 0; row < m_nFixedRows; row++) 
				RedrawCell(row, m_idCurrent.col);
		}
		if (prevCell.row != m_idCurrent.row)
		{
			for (int col = 0; col < m_nFixedCols; col++) 
				RedrawCell(m_idCurrent.row, col);
		}
		
		//������� Ŭ���� ����Ű�� �̵���, ������ �ʴ� ���� �ذ�
		if ( m_idCurrent.col >= 16 )
			MessageToParent(m_idCurrent.row, m_idCurrent.col, GVNM_SELCHANGED);

		if (m_sensitive)
		{
			if (IsCellAttribute(m_idCurrent, GVAT_EDIT))
				OnEditCell(m_idCurrent.row, m_idCurrent.col, VK_TAB);
			else if (IsCellAttribute(m_idCurrent, GVAT_COMBO))
				OnComboCell(m_idCurrent.row, m_idCurrent.col);
			else if (IsCellAttribute(m_idCurrent, GVAT_DATE))
				OnDateCell(m_idCurrent.row, m_idCurrent.col);
			else if (IsCellAttribute(m_idCurrent, GVAT_TIME))
				OnTimeCell(m_idCurrent.row, m_idCurrent.col);
		}
	}

	return prevCell;
}

void CintGrid::SetSelectedRange(const CRangeCell& range, BOOL bForceRepaint)
{
	SetSelectedRange(range.GetMinRow(), range.GetMinCol(), 
				range.GetMaxRow(), range.GetMaxCol(), bForceRepaint);
}

void CintGrid::SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol, BOOL bForceRepaint)
{
	CDC* pDC = nullptr;

	if (bForceRepaint) pDC = GetDC();

	for (POSITION pos = m_mapSelCell.GetStartPosition(); pos != nullptr; )
	{
		DWORD key{};
		CIdCell idCell{};
		//	cell{};
		m_mapSelCell.GetNextAssoc(pos, key, (CIdCell&)idCell);
		if (IsValid(idCell))
		{
			SetItemState(idCell.row, idCell.col, GetItemState(idCell.row, idCell.col) & ~GVST_SELECTED);
			if (nMinRow <= idCell.row && idCell.row <= nMaxRow 
						&& nMinCol <= idCell.col && idCell.col <= nMaxCol)
				continue;

			if (m_selectLine)
			{
				if (bForceRepaint && pDC)
					RedrawCell(idCell.row, idCell.col, pDC);
				else
					InvalidateCellRect(idCell);
			}
		}
	}
    
	for (POSITION pos = m_mapPreCell.GetStartPosition(); pos != nullptr;)
	{
		DWORD key; CIdCell idCell;
		m_mapPreCell.GetNextAssoc(pos, key, (CIdCell&)idCell);

		if (!IsValid(idCell)) continue;

		const int nState = GetItemState(idCell.row, idCell.col);
		SetItemState(idCell.row, idCell.col, nState|GVST_SELECTED);

		if (m_selectLine)
		{
			if (bForceRepaint && pDC)
				RedrawCell(idCell.row, idCell.col, pDC);
			else
				InvalidateCellRect(idCell);
		}
	}

	bool fToggle = false;
	if (nMinRow >= 0 && nMinCol >= 0 && nMaxRow >= 0 && nMaxCol >= 0
		&& nMaxRow < m_nRows && nMaxCol < m_nCols && nMinRow <= nMaxRow && nMinCol <= nMaxCol)
	{
		if (press_CTRLKEY() && m_selectLine && nMinRow == nMaxRow)
		{
			if (GetItemState(nMinRow, nMinCol) & GVST_SELECTED)
				fToggle = true;
		}

		for (int row = nMinRow; row <= nMaxRow; row++)
		{
			for (int col = nMinCol; col <= nMaxCol; col++) 
			{
				const int nState = GetItemState(row, col);
				if (nState & GVST_SELECTED || fToggle)
				{
					SetItemState(row, col, (nState & ~GVST_SELECTED));
					continue;
				}

				const CIdCell idCell(row, col), cell;
				SetItemState(row, col, nState|GVST_SELECTED);

				if (m_selectLine)
				{
					if (bForceRepaint && pDC)
						RedrawCell(row, col, pDC);
					else
						InvalidateCellRect(idCell);
				}
			}
		}
	}

	if (pDC != nullptr) ReleaseDC(pDC);

	int key{}; _fixMerge fixMerge;
	for (POSITION pos = m_mapMerge.GetStartPosition(); pos != nullptr; )
	{
		m_mapMerge.GetNextAssoc(pos, key, (_fixMerge &)fixMerge);
		InvalidateRect(fixMerge.rect);
	}
} 

void CintGrid::UpdateRowSelectedRange(int row, bool insert)
{
	DWORD key{}; CIdCell idCell;

	m_idSelect.row = -1; m_idSelect.col = -1;

	m_mapPreCell.RemoveAll();
	for (POSITION pos = m_mapSelCell.GetStartPosition(); pos != nullptr; )
	{
		m_mapSelCell.GetNextAssoc(pos, key, (CIdCell&)idCell);
		m_mapPreCell.SetAt(key, idCell);
	}

	m_mapSelCell.RemoveAll();
	for (POSITION pos = m_mapPreCell.GetStartPosition(); pos != nullptr; )
	{
		m_mapPreCell.GetNextAssoc(pos, key, (CIdCell&)idCell);
		if (idCell.row < row)
		{
			m_mapSelCell.SetAt(key, idCell);
			continue;
		}

		if (insert)
		{
			idCell.row += 1;
			key = MAKELONG(idCell.row, idCell.col);
			m_mapSelCell.SetAt(key, idCell);
		}
		else if (!insert)		// delete
		{
			if (idCell.row == row)
				continue;

			idCell.row -= 1;
			key = MAKELONG(idCell.row, idCell.col);
			m_mapSelCell.SetAt(key, idCell);
		}
	}

	m_mapPreCell.RemoveAll();
	for (POSITION pos = m_mapSelCell.GetStartPosition(); pos != nullptr; )
	{
		m_mapSelCell.GetNextAssoc(pos, key, (CIdCell&)idCell);
		m_mapPreCell.SetAt(key, idCell);
	}
}

void CintGrid::UpdateColSelectedRange(int col, bool insert)
{
	DWORD key{}; CIdCell idCell;

	m_idSelect.row = -1; m_idSelect.col = -1;

	m_mapPreCell.RemoveAll();
	for (POSITION pos = m_mapSelCell.GetStartPosition(); pos != nullptr; )
	{
		m_mapSelCell.GetNextAssoc(pos, key, (CIdCell&)idCell);
		m_mapPreCell.SetAt(key, idCell);
	}

	m_mapSelCell.RemoveAll();
	for (POSITION pos = m_mapPreCell.GetStartPosition(); pos != nullptr; )
	{
		m_mapPreCell.GetNextAssoc(pos, key, (CIdCell&)idCell);
		if (idCell.col < col)
		{
			m_mapSelCell.SetAt(key, idCell);
			continue;
		}

		if (insert)
		{
			idCell.col += 1;
			key = MAKELONG(idCell.row, idCell.col);
			m_mapSelCell.SetAt(key, idCell);
		}
		else if (!insert)		// delete
		{
			if (idCell.col == col)
				continue;

			idCell.col -= 1;
			key = MAKELONG(idCell.row, idCell.col);
			m_mapSelCell.SetAt(key, idCell);
		}
	}

	m_mapPreCell.RemoveAll();
	for (POSITION pos = m_mapSelCell.GetStartPosition(); pos != nullptr; )
	{
		m_mapSelCell.GetNextAssoc(pos, key, (CIdCell&)idCell);
		m_mapPreCell.SetAt(key, idCell);
	}
}

void CintGrid::SelectAllCells()
{
	SetSelectedRange(m_nFixedRows, m_nFixedCols, GetRowCount()-1, GetColumnCount()-1);
}

void CintGrid::SelectColumns(CIdCell current)
{
	if (current.col < colNAME || !IsValid(current))
		return;

	SetSelectedRange(GetFixedRowCount(), min(m_idSelect.col, current.col), 
				GetRowCount()-1, max(m_idSelect.col, current.col)); 
}

void CintGrid::SelectRows(CIdCell current)
{  
	if (current.row < m_nFixedRows || !IsValid(current))
		return;

	SetSelectedRange(min(m_idSelect.row, current.row), colSIG/*GetFixedColumnCount()*/,
				max(m_idSelect.row, current.row), GetColumnCount() -1);
}

void CintGrid::SelectCells(CIdCell current)
{
	if (current.col < m_nFixedRows || current.col < m_nFixedCols)
		return;
	if (!IsValid(current))
		return;

	SetSelectedRange(min(m_idSelect.row, current.row), min(m_idSelect.col, current.col), 
			max(m_idSelect.row, current.row), max(m_idSelect.col, current.col)); 
}

void CintGrid::OnSelecting(const CIdCell& current)
{
	switch (m_mousemode)
	{
	case mouseSELECTALL:
		SelectAllCells();
		break;
	case mouseSELECTCOL:
		SelectColumns(current);
		break;
	case mouseSELECTROW:
		SelectRows(current);
		break;
	case mouseSELECTCELLS:
		SelectCells(current);
		break;
	}
}

BOOL CintGrid::IsRowResizeArea(const CPoint& point) const
{
	if (m_nFixedCols > 0)
	{
		if (point.x >= GetFixedColumnWidth())
			return FALSE;
	}
	else
	{
		if (point.x >= (int)m_aryColWidth[0])
			return FALSE;
	}

	const CIdCell current = GetCellFromPoint(point);
	CPoint start;
	if (!GetCellOrigin(current.row, current.col, &start))
		return FALSE;

	const int endy = start.y + GetRowHeight(current.row);
	if ((point.y - start.y <= rszCAPTURE && current.row != 0) || endy - point.y <= rszCAPTURE)
		return TRUE;

	return FALSE;
}

BOOL CintGrid::IsColResizeArea(const CPoint& point) const
{
	if (point.y >= GetFixedRowHeight())
		return FALSE;

	CPoint start;
	const CIdCell current = GetCellFromPoint(point);
	if (!GetCellOrigin(current.row, current.col, &start)) 
		return FALSE;

	const int endx = start.x + GetColumnWidth(current.col);
    
	if ((point.x - start.x <= rszCAPTURE && current.col != 0) || endx - point.x <= rszCAPTURE)
		return TRUE;
	
	return FALSE;
}

CIdCell CintGrid::GetCellFromPoint(CPoint point, BOOL bAllowFixedCellCheck /*=TRUE*/) const
{  
	CIdCell cell;
	const CIdCell tlCell = GetTopleftNonFixedCell();

	const int fixedColWidth = GetFixedColumnWidth();
	if (point.x < 0 || (!bAllowFixedCellCheck && point.x < fixedColWidth))
		cell.col = -1;
	else if (point.x < fixedColWidth) // in fixed col
	{
		int xpos = 0;
		int col = 0;
		for (col = 0; col < m_nFixedCols; col++)
		{
			xpos += GetColumnWidth(col);
			if (xpos > point.x) break;
		}
		cell.col = col;
	}
	else
	{
		int xpos = fixedColWidth;
		int col = 0;
		for (col = tlCell.col; col < GetColumnCount(); col++)
		{
			xpos += GetColumnWidth(col);
			if (xpos > point.x) break;
		}

		if (col >= GetColumnCount())
			cell.col = -1;
		else
			cell.col = col;
	}
    
	const int fixedRowHeight = GetFixedRowHeight();
	if (point.y < 0 || (!bAllowFixedCellCheck && point.y < fixedRowHeight)) // not in window
		cell.row = -1;
	else if (point.y < fixedRowHeight) // in fixed col
	{
		int ypos = 0;
		int row = 0;
		for (row = 0; row < m_nFixedRows; row++)
		{
			ypos += GetRowHeight(row);
			if (ypos > point.y) 
				break;
		}
		cell.row = row;
	}
	else
	{
		int ypos = fixedRowHeight;
		int row = 0;
		for (row = tlCell.row; row < GetRowCount(); row++)
		{
			ypos += GetRowHeight(row);
			if (ypos > point.y) 
				break;
		}

		if (row >= GetRowCount())
			cell.row = -1;
		else
			cell.row = row;
	}

	return cell;
}

////////////////////////////////////////////////////////////////////////////////
// cellrange functions

CIdCell CintGrid::GetTopleftNonFixedCell(bool bVisibleRow, bool bRight) const
{
	const int Vscroll = GetScrollPos(SB_VERT);
	// MODIFY PSH 20070911
	//int Hscroll = GetScrollPos(SB_HORZ);
	const int Hscroll = (m_bVirHPos ? m_nVirHPos : GetScrollPos(SB_HORZ));
	// END MODIFY
	int nColumn = m_nFixedCols, nRight = 0;
	
	if(bVisibleRow)		//2015.12.16 KSJ ����� �ִ� �ͺ��� ���
	{
		if(bRight)		//2015.12.16 KSJ ������ ��ũ��
		{
			if(Hscroll == 0)
			{
				while (nColumn < (GetColumnCount()-1))
				{
					const int nColSize = GetColumnWidth(nColumn);
					if(0 < nColSize)
						break;
					
					nColumn++;
				}
			}
			else
			{
				while (nRight <= Hscroll && nColumn < (GetColumnCount()-1))
				{
					const int nColSize = GetColumnWidth(++nColumn);
					if(0 < nColSize)	nRight += nColSize;
				}
			}
		}
		else
		{
			if(Hscroll == 0)
			{
				nColumn = 1;
			}
			else
			{
				CStringArray arrCol;
				CString strCol;
				
				arrCol.Add("1");
				while (nRight < Hscroll && nColumn < (GetColumnCount()-1))
				{
					const int nColSize = GetColumnWidth(++nColumn);
					if(0 < nColSize)
					{
						strCol.Format("%d", nColumn);
						arrCol.Add(strCol);
						nRight += nColSize;
					}
				}
				
				//2015.12.16 KSJ ���� �����͸� �����;� �ϱ⶧����
				nColumn = atoi(arrCol[arrCol.GetSize()-1]);			
			}
		}
	}
	else
	{
		while (nRight < Hscroll && nColumn < (GetColumnCount()-1))
			nRight += GetColumnWidth(nColumn++);
	}
	
	int nRow = m_nFixedRows, nTop = 0;
	while (nTop < Vscroll && nRow < (GetRowCount()-1))
		nTop += GetRowHeight(nRow++);
	
	return CIdCell(nRow, nColumn);
}

CRangeCell CintGrid::GetVisibleNonFixedCellRange(LPRECT pRect) const
{
	CRect rect; GetClientRect(rect);
	const CIdCell tlCell = GetTopleftNonFixedCell();

	// calc bottom
	int bottom = GetFixedRowHeight();// * m_yRate;
	int ii = 0;
	for (ii = tlCell.row; ii < GetRowCount(); ii++)
	{
//		bottom += GetRowHeight(ii) * m_yRate;
		bottom += GetRowHeight(ii);
		if (bottom >= rect.bottom)
		{
			bottom = rect.bottom;
			break;
		}
	}                                
	const int maxVisibleRow = min(ii, GetRowCount() - 1);
	
	// calc right
	int right = GetFixedColumnWidth();
	for (ii = tlCell.col; ii < GetColumnCount(); ii++)
	{
//		right += GetColumnWidth(ii) * m_xRate;
		right += GetColumnWidth(ii);
		if (right >= rect.right)
		{
			right = rect.right;
			break;
		}
	}
	const int maxVisibleCol = min(ii, GetColumnCount() - 1);
	if (pRect)
	{
		pRect->left = pRect->top = 0;
		pRect->right = right;
		pRect->bottom = bottom;
	}

	return CRangeCell(tlCell.row, tlCell.col, maxVisibleRow, maxVisibleCol);
}

CRangeCell CintGrid::GetUnobstructedNonFixedCellRange() const
{
	CRect rect; GetClientRect(rect);

	const CIdCell tlCell = GetTopleftNonFixedCell();

	// calc bottom
	int bottom = GetFixedRowHeight();
	int ii = 0;
	for (ii = tlCell.row; ii < GetRowCount(); ii++)
	{
		bottom += GetRowHeight(ii);
		if (bottom >= rect.bottom)
			break;
	}
	int maxVisibleRow = min(ii, GetRowCount() - 1);
	if (maxVisibleRow > 0 && bottom > rect.bottom)
		maxVisibleRow--;

	// calc right
	int right = GetFixedColumnWidth();
	for (int ii = tlCell.col; ii < GetColumnCount(); ii++)
	{
		right += GetColumnWidth(ii);
		if (right >= rect.right) 
			break;
	}
	int maxVisibleCol = min(ii, GetColumnCount() - 1);
	if (maxVisibleCol > 0 && right > rect.right)
		maxVisibleCol--;

	return CRangeCell(tlCell.row, tlCell.col, maxVisibleRow, maxVisibleCol);
}

CRangeCell CintGrid::GetSelectedCellRange(bool onlyrow) const
{
	CRangeCell range(GetRowCount(), GetColumnCount(), -1, -1);

// updateX_20060113
	if (onlyrow)
	{
		range.SetMinRow(m_idSelect.row);
		range.SetMaxRow(m_idSelect.row);
		range.SetMinCol(0);
		range.SetMaxCol(GetColumnCount()-1);
		return range;
	}

	for (POSITION pos = m_mapSelCell.GetStartPosition(); pos != nullptr; )
	{
		DWORD key; CIdCell cell;
		m_mapSelCell.GetNextAssoc(pos, key, (CIdCell&)cell);

		range.SetMinRow(min(range.GetMinRow(), cell.row));
		range.SetMinCol(min(range.GetMinCol(), cell.col));
		range.SetMaxRow(max(range.GetMaxRow(), cell.row));
		range.SetMaxCol(max(range.GetMaxCol(), cell.col));
	}

	return range;
}

CRangeCell CintGrid::GetCellRange() const
{
	return CRangeCell(0, 0, GetRowCount() - 1, GetColumnCount() - 1);
}

void CintGrid::ResetSelectedRange()
{
	m_mapPreCell.RemoveAll();
	SetSelectedRange(-1, -1, -1, -1);
	SetFocusCell(-1, -1);
}

int CintGrid::GetScrollPos32(int nBar, BOOL bGetTrackPos)
{
	SCROLLINFO sinfo;
	sinfo.cbSize = sizeof(SCROLLINFO);

	if (bGetTrackPos)
	{
		if (GetScrollInfo(nBar, &sinfo, SIF_TRACKPOS))
			return sinfo.nTrackPos;
	}
	else 
	{
		if (GetScrollInfo(nBar, &sinfo, SIF_POS))
			return sinfo.nPos;
	}

	return 0;
}

BOOL CintGrid::SetScrollPos32(int nBar, int nPos, BOOL bRedraw)
{
	SCROLLINFO sinfo;

	sinfo.cbSize = sizeof(SCROLLINFO);
	sinfo.fMask  = SIF_POS;
	sinfo.nPos   = nPos;

	return SetScrollInfo(nBar, &sinfo, bRedraw);
}

void CintGrid::ResetScrollBars()
{
	if (!::IsWindow(GetSafeHwnd())) 
		return;

	CRect rect; 
	GetClientRect(rect);

	rect.left += GetFixedColumnWidth();
	rect.top  += GetFixedRowHeight();
	
	if (rect.left >= rect.right || rect.top >= rect.bottom)
		return;

	const CRect visibleRC(GetFixedColumnWidth(), GetFixedRowHeight(), rect.right, rect.bottom);
	const CRect virtualRC(GetFixedColumnWidth(), GetFixedRowHeight(), GetVirtualWidth(), GetVirtualHeight());

	CString strTemp;

	const CRangeCell visibleCells = GetUnobstructedNonFixedCellRange();
	if (!IsValid(visibleCells)) return;

	m_Vscroll = 0;
	if (visibleRC.Height() < virtualRC.Height()) m_Vscroll = virtualRC.Height() - 1;

	m_Hscroll = 0;
	if (visibleRC.Width() < virtualRC.Width()) m_Hscroll = virtualRC.Width() - 1;

	// ADD PSH 20070911
	m_nVirHScroll	= m_Hscroll;
	// END ADD

	switch (m_NOscroll)
	{
		case GVSC_NONE:
			m_Vscroll = 0; m_Hscroll = 0; break;
		case GVSC_HORZ:
			m_Hscroll = 0;	break;
		case GVSC_VERT:
			{
				m_Vscroll = visibleRC.Height();
			}
			break;
		case GVSC_VERTAUTO:
			{
				if(visibleRC.Height() >= m_Vscroll)
				{
					m_Vscroll = visibleRC.Height();
				}
			}
			break;
		default:
			break;
	}

	SCROLLINFO sinfo;

	sinfo.cbSize = sizeof(SCROLLINFO);
	sinfo.fMask  = SIF_PAGE|SIF_RANGE;
	//if (m_DIscroll) 
		//sinfo.fMask |= SIF_DISABLENOSCROLL;
	//sinfo.fMask |= SIF_DISABLENOSCROLL;

	sinfo.nPage  = visibleRC.Height();
	sinfo.nMin   = 0; 
	sinfo.nMax   = m_Vscroll; 

	if(m_NOscroll == GVSC_VERT && sinfo.nPage == sinfo.nMax)
	{
		//sinfo.nMax = 0;
		EnableScrollBar(SB_VERT,ESB_DISABLE_BOTH);
		SetScrollPos32(SB_VERT, 0);		// 2012.05.07 KSJ ��ũ���� ������ ������ ��� ����� ������ ������ �ʴ� ���� ����
	}
	else if(m_NOscroll == GVSC_VERTAUTO && sinfo.nPage == sinfo.nMax)
	{
		EnableScrollBar(SB_VERT,ESB_DISABLE_BOTH);
		SetScrollPos32(SB_VERT, 0);		// 2012.05.07 KSJ ��ũ���� ������ ������ ��� ����� ������ ������ �ʴ� ���� ����
	}
	else
	{
		EnableScrollBar(SB_VERT,ESB_ENABLE_BOTH);
	}

	SetScrollInfo(SB_VERT, &sinfo, TRUE);
	//TRACE("%d=====%d\n",sinfo.nMax,sinfo.nPage);

	sinfo.nPage  = visibleRC.Width();
	sinfo.nMin   = 0; 
	sinfo.nMax   = m_Hscroll; 

	if(sinfo.nPage == sinfo.nMax)
	{
		sinfo.nMax = 0;
	}

	SetScrollInfo(SB_HORZ, &sinfo, TRUE);
}

BOOL CintGrid::GetVScrollExist()
{
	SCROLLINFO sinfo;

	GetScrollInfo(SB_VERT,&sinfo,SIF_ALL);

	if(sinfo.nMax == 0)
		return FALSE;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column position functions
BOOL CintGrid::GetCellOrigin(int nRow, int nCol, LPPOINT pt) const
{
	if (!IsValid(CIdCell(nRow, nCol))) return FALSE;
	if (IsCellAttribute(CIdCell(nRow, nCol), GVAT_HIDDEN)) return FALSE;

	CIdCell tlCell;
	if (nCol >= m_nFixedCols || nRow >= m_nFixedRows)
		tlCell = GetTopleftNonFixedCell();

	if ((nRow >= m_nFixedRows && nRow < tlCell.row) || (nCol >= m_nFixedCols && nCol < tlCell.col))
		return FALSE;

	pt->x = 0;
	if (nCol < m_nFixedCols)                      // is a fixed column
	{
		for (int ii = 0; ii < nCol; ii++)
			pt->x += GetColumnWidth(ii);	
	}
	else
	{                                        // is a scrollable data column
		for (int ii = 0; ii < m_nFixedCols; ii++)
			pt->x += GetColumnWidth(ii);	
		for (int ii = tlCell.col; ii < nCol; ii++)
			pt->x += GetColumnWidth(ii);	
	}

	pt->y = 0;
	if (nRow < m_nFixedRows)                      // is a fixed row
	{
		for (int ii = 0; ii < nRow; ii++)
			pt->y += GetRowHeight(ii);
	}
	else
	{                                        // is a scrollable data row
		for (int ii = 0; ii < m_nFixedRows; ii++)
			pt->y += GetRowHeight(ii);
		for (int ii = tlCell.row; ii < nRow; ii++)
			pt->y += GetRowHeight(ii);
	}

	return TRUE;
}

BOOL CintGrid::GetCellRect(int nRow, int nCol, LPRECT pRect) const
{    
	CPoint origin;
	if (!GetCellOrigin(nRow, nCol, &origin))
		return FALSE;

	pRect->left   = origin.x;
	pRect->top    = origin.y;
	pRect->right  = origin.x + GetColumnWidth(nCol) -1;
	pRect->bottom = origin.y + GetRowHeight(nRow)  -1;

	return TRUE;
}

int CintGrid::GetLastColumnPos()
{
	CRect rect;

	GetCellRect(1, m_aryColWidth.size()-1,rect);

	return rect.left + rect.Width();
}

BOOL CintGrid::GetTextRect(int nRow, int nCol, LPRECT pRect)
{
	GVITEM	gvitem;

	gvitem.mask = GVMK_IMAGE;
	gvitem.row = nRow;
	gvitem.col = nCol;

	if (!GetItem(&gvitem)) return FALSE;
	if (!GetCellRect(nRow, nCol, pRect)) return FALSE;

	if (m_imageList && gvitem.image >= 0)
	{
		IMAGEINFO Info;
		if (m_imageList->GetImageInfo(gvitem.image, &Info))
		{
			const int nImageWidth = Info.rcImage.right - Info.rcImage.left +1;
			pRect->left += nImageWidth + m_margin;
		}
	}

	return TRUE;
}

BOOL CintGrid::GetCellRangeRect(const CRangeCell& range, LPRECT lpRect) const
{
	CPoint minOrigin, maxOrigin;

	if (!GetCellOrigin(range.GetMinRow(), range.GetMinCol(), &minOrigin)) 
		return FALSE;
	if (!GetCellOrigin(range.GetMaxRow(), range.GetMaxCol(), &maxOrigin)) 
		return FALSE;

	lpRect->left   = minOrigin.x;
	lpRect->top    = minOrigin.y;
	lpRect->right  = maxOrigin.x + GetColumnWidth(range.GetMaxCol() -1);
	lpRect->bottom = maxOrigin.y + GetRowHeight(range.GetMaxRow() -1);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// Grid attribute functions

LRESULT CintGrid::OnSetFont(WPARAM hFont, LPARAM lParam)
{
	const LRESULT result = Default();

	LOGFONT logfont;
	if (!GetObject((HFONT) hFont, sizeof(LOGFONT), &logfont))
		return result;
//    	memcpy(&m_logfont, &logfont, sizeof(LOGFONT));
    
	for (int row = 0; false && row < GetRowCount(); row++)
	{
		for (int col = 0; col < GetColumnCount(); col++)
			SetItemFont(row, col, &logfont);
	}

	// Get the font size and hence the default cell size
	CDC* pDC = GetDC();

	if (pDC) 
	{
		m_font.DeleteObject();
		m_font.CreatePointFontIndirect(&m_logfont);
		CFont* pOldFont = pDC->SelectObject(&m_font);

		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);

		m_margin = pDC->GetOutputTextExtent(_T(" "), 1).cx; m_margin /= 2;
		pDC->SelectObject(pOldFont);

		//2012.04.18 KSJ	���⼭ 20���� ���õǾ� ��ũ�ѹٰ� �����. �׷��� m_margin�� -1�� ����
		m_defHeight = tm.tmHeight + tm.tmExternalLeading + (2 * (m_margin -1));	
		m_defWidth  = tm.tmAveCharWidth * 4 + 2 * m_margin;
	}

	ReleaseDC(pDC);

	if (::IsWindow(GetSafeHwnd())) 
		Invalidate();

	return result;
}

LRESULT CintGrid::OnGetFont(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return (LRESULT) (HFONT) m_font;
}

BOOL CintGrid::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest != HTCLIENT)
	{
		return CWnd::OnSetCursor(pWnd, nHitTest, message);
	}

	switch (m_mousemode) 
	{
	case mouseOVERCOLDIVIDE:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		break;
	case mouseOVERROWDIVIDE:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		break;
	case mouseMOVING:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
		break;
	case mouseEDITCELL:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
		break;
	case mouseDRAGGING:
		break;
	default:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		break;
	}
	return TRUE;
}

BOOL CintGrid::SetFixedRowCount(int nFixedRows)
{
	ASSERT(nFixedRows >= 0);

	if (nFixedRows > GetRowCount() && !SetRowCount(nFixedRows)) 
		return FALSE;

	if (m_idCurrent.row < nFixedRows)
		SetFocusCell(-1, -1);

	m_nFixedRows = nFixedRows;

	if (GetSafeHwnd()) Invalidate();

	return TRUE;
}

// jdh
BOOL CintGrid::SetFixedColumnCount(int nFixedCols)
{
	ASSERT(nFixedCols >= 0);

	if (nFixedCols > GetColumnCount() && !SetColumnCount(nFixedCols))
		return FALSE;

	if (m_idCurrent.col < nFixedCols)
		SetFocusCell(-1,-1);

	m_nFixedCols = nFixedCols;

	if (GetSafeHwnd()) Invalidate();

	return TRUE;
}

BOOL CintGrid::SetRowCount(int nRows)
{
	ASSERT(nRows >= 0);

	if (nRows == GetRowCount())
		return TRUE;

	if (nRows < m_nFixedRows) 
		m_nFixedRows = nRows;

	if (m_idCurrent.row >= nRows)
		SetFocusCell(-1, -1);

	int addedRows = nRows - GetRowCount();
	//if (addedRows < 0)
	//{
	//	for (int row = nRows; row < m_nRows; row++)
	//	{
	//		for (int col = 0; col < m_nCols; col++) 
	//		{
	//			CCell* pCell = GetCell(row, col);
	//			if (pCell)
	//			{
	//				EmptyCell(pCell, row, col);
	//				delete pCell;
	//			}
	//		}
	//		GRID_ROW* pRow = m_aryGridRows[row];
	//		if (pRow) delete pRow;
	//	}
	//}


	m_nRows = nRows;
	m_aryRowHeight.resize(m_nRows);

	while (addedRows > 0)
	{
		// initialize row heights and data
		const int row = m_aryGridRows.size();
		const auto& vitem = m_aryGridRows.emplace_back(std::make_shared<GRID_ROW>());
		m_aryRowHeight[row] = m_defHeight;
		for (int col = 0; col < GetColumnCount(); col++)
		{
			vitem->emplace_back(CreateCell(row, col));
		}
		addedRows--;
	}
	m_aryGridRows.resize(m_nRows);


	if (GetSafeHwnd())
	{
		ResetScrollBars();
		Invalidate(false);
	}

	return TRUE;
}

BOOL CintGrid::SetColumnCount(int nCols)
{

	ASSERT(nCols >= 0);

	if (nCols == GetColumnCount())
		return TRUE;

	if (nCols < m_nFixedCols)
		m_nFixedCols = nCols;

	if (m_idCurrent.col >= nCols)
		SetFocusCell(-1, -1);

	const int addedCols = nCols - GetColumnCount();
	if (addedCols < 0)
	{
		int ii = 0;
		for (const auto& vitem : m_aryGridRows)
		{
			for (int col = nCols; col < GetColumnCount(); col++)
				SetItemState(ii, col, 0);

			//vitem->erase(vitem->begin() + nCols, vitem->end());
			vitem->resize(nCols);
			ii++;
		}

	}

	m_nCols = nCols;
	m_aryColWidth.resize(m_nCols);

	for (int ii = 0; ii < m_nRows; ii++)
	{
		if (m_aryGridRows[ii])
			m_aryGridRows[ii]->resize(nCols);
	}

	if (addedCols > 0)
	{
		const int startCol = nCols - addedCols;
		for_each(m_aryColWidth.begin() + startCol, m_aryColWidth.end(), [this](auto& width) {
			width = m_defWidth;
			});


		int ii = 0;
		for_each(m_aryGridRows.begin(), m_aryGridRows.end(), [&ii, this](const auto& vitem) {
			for (int col = 0; col < GetColumnCount(); col++)
				vitem->at(col) = CreateCell(ii, col);
			ii++;
			});
	}

	if (GetSafeHwnd())
	{
		ResetScrollBars();
		Invalidate();
	}
	return TRUE;
}

// jdh
int CintGrid::InsertColumn(LPCTSTR strHeading, UINT nFormat, int nColumn)
{
	if (nColumn >= 0 && nColumn >= GetColumnCount())
		return -1;

	//	ResetSelectedRange();
	SetFocusCell(-1, -1);

	if (m_nRows < 1)
		SetRowCount(1);
	if (nColumn < 0)
	{
		nColumn = m_nCols;
		m_aryColWidth.emplace_back(0);

		int row = 0;
		for_each(m_aryGridRows.begin(), m_aryGridRows.end(), [this, nColumn, &row](const auto& vitem) {
			vitem->emplace_back(CreateCell(row++, nColumn));
			});
	}
	else
	{
		const UINT n = 0;
		m_aryColWidth.insert(m_aryColWidth.begin() + nColumn, n);
		int row = 0;
		for_each(m_aryGridRows.begin(), m_aryGridRows.end(), [this, nColumn, &row](const auto& vitem) {
			vitem->insert(vitem->begin() + nColumn, CreateCell(row++, nColumn));
			});
	}

	m_nCols++;

	SetItemText(0, nColumn, strHeading);
	SetItemFormat(0, nColumn, GVFM_HEADER);

	for (int row = 0; row < m_nRows; row++)
		SetItemFormat(row, nColumn, nFormat);

	// initialized column width
	AutoSizeColumn(nColumn);

	if (m_idCurrent.col != -1 && nColumn < m_idCurrent.col)
		m_idCurrent.col++;

	UpdateColSelectedRange(nColumn, true);

	if (GetSafeHwnd())
	{
		ResetScrollBars();
		Invalidate();
	}
	return nColumn;
}

int CintGrid::InsertRow(LPCTSTR strHeading, int nRow, BOOL bRefresh)
{
	if (nRow >= 0 && nRow > GetRowCount())
		return -1;

//	ResetSelectedRange();
	SetFocusCell(-1, -1);

	if (m_nCols < 1)
		SetColumnCount(1);    
	if (nRow < 0)
	{
		nRow = m_nRows;
		m_aryRowHeight.emplace_back(0);
		m_aryGridRows.emplace_back(std::make_shared<GRID_ROW>());
	}
	else
	{
		const UINT n = 0;
		m_aryRowHeight.emplace(m_aryRowHeight.begin() + nRow, n);
		m_aryGridRows.insert(m_aryGridRows.begin() + nRow, std::make_shared<GRID_ROW>());
	}
    
	xROWS rowItem = m_aryGridRows[nRow];
	m_nRows++;

	// Initialise cell data
	for (int col = 0; col < GetColumnCount(); col++)
		rowItem->emplace_back(CreateCell(nRow, col));

	rowItem->resize(m_nCols);

	SetItemText(nRow, 0, strHeading);

	m_pParent->InsertAtRecommandInfo(nRow, "");	//2012.05.07 KSJ Insert�ÿ� ��õ�����ʵ� Insert���ش�.

	const int ncnt = m_aryRowHeight[nRow] = m_defHeight;

	if (m_idCurrent.row != -1 && nRow < m_idCurrent.row)
		m_idCurrent.row++;
    
	if (GetSafeHwnd())
	{
		if(bRefresh)
			ResetScrollBars(); Invalidate();
	}

	UpdateRowSelectedRange(nRow, true);
	return nRow;
}

int CintGrid::InsertRow(int drag, int& drop, BOOL bRefresh)
{
	if (drag < 0 || drag > GetRowCount())
		return -1;
	if (drop < 0 || drop > GetRowCount())
		return -1;

	if (drag == drop)
		return 0;

	ResetSelectedRange();

	const int height = m_aryRowHeight[drag];
	m_aryRowHeight.erase(m_aryRowHeight.begin() + drag);

	xROWS rowItem = m_aryGridRows[drag];
	m_aryGridRows.erase(m_aryGridRows.begin() + drag);

	if (drop > drag) drop -= 1;

	m_aryRowHeight.insert(m_aryRowHeight.begin() + drop, height);
	m_aryGridRows.insert(m_aryGridRows.begin() + drop, rowItem);

	if (GetSafeHwnd()) ResetScrollBars();
	Invalidate(false);

	return 0;
}

xCELL CintGrid::CreateCell(int nRow, int nCol)
{
	auto pCell = std::make_shared<CCell>();
	if (!pCell) return nullptr;

	int refRow = 0, refCol = 0;

	// row insert
	if (nRow > 0 && nCol >= 0 && nCol < m_nCols)
	{
		refRow = nRow - 1;
		if (refRow < m_nFixedRows && nRow < m_nRows - m_nFixedRows)
			refRow = nRow + 1;

		pCell->attr = GetItemAttr(refRow, nCol);
		pCell->format = GetItemFormat(refRow, nCol);
		pCell->fgcol = GetItemFgColor(refRow, nCol);
		pCell->bkcol = GetItemBkColor(refRow, nCol);
		pCell->symbol = GetItemSymbol(refRow, nCol);
		pCell->misc = GetItemMisc(refRow, nCol);
	}
	else if (nRow > 0 && nCol >= 0 && nCol == m_nCols)
	{
		refCol = nCol - 1;
		pCell->fgcol = GetItemFgColor(nRow, refCol);
		pCell->bkcol = GetItemBkColor(nRow, refCol);
	}

	memcpy(&(pCell->font), &m_logfont, sizeof(LOGFONT));
	return pCell;
}

void CintGrid::EmptyCell(CCell* pCell, int nRow, int nCol)
{
	SetItemState(nRow, nCol, 0);
	pCell->text.Empty();
}

BOOL CintGrid::DeleteColumn(int nColumn)
{
	if (nColumn < 0 || nColumn >= GetColumnCount())
		return FALSE;
	//	ResetSelectedRange();
	SetFocusCell(-1, -1);

	int ii = 0;
	for_each(m_aryGridRows.begin(), m_aryGridRows.end(), [this, &ii, nColumn](const auto& vitem) {
		SetItemState(ii, nColumn, 0);
		vitem->erase(vitem->begin() + nColumn);
		ii++;
		});

	//for (int row = 0; row < GetRowCount(); row++) 
	//{
	//	GRID_ROW* pRow = m_aryGridRows[row];
	//	if (!pRow)
	//		return FALSE;

	//	CCell* pCell = pRow->GetAt(nColumn);
	//	if (pCell)
	//	{
	//		EmptyCell(pCell, row, nColumn);
	//		delete pCell;
	//	}
	//	pRow->RemoveAt(nColumn);
	//}

	m_aryColWidth.erase(m_aryColWidth.begin() + nColumn);
	m_nCols--;
	if (nColumn < m_nFixedCols)
		m_nFixedCols--;

	if (nColumn == m_idCurrent.col)
		m_idCurrent.row = m_idCurrent.col = -1;
	else if (nColumn < m_idCurrent.col)
		m_idCurrent.col--;

	UpdateColSelectedRange(nColumn, false);

	if (GetSafeHwnd())
	{
		ResetScrollBars();
		Invalidate();
	}

	return TRUE;
}

BOOL CintGrid::DeleteRow(int nRow)
{
	if (nRow < 0 || nRow >= GetRowCount())
		return FALSE;

	auto pRow = m_aryGridRows[nRow];
	if (!pRow) return FALSE;


// 	ResetSelectedRange();
	SetFocusCell(-1, -1);
	

	for (int jj = 0; jj < GetColumnCount(); jj++)
		SetItemState(nRow, jj, 0);

	m_aryGridRows[nRow]->clear();
	m_aryGridRows.erase(m_aryGridRows.begin() + nRow);
	m_aryRowHeight.erase(m_aryRowHeight.begin() + nRow);

	m_nRows--;
	if (nRow < m_nFixedRows)
		m_nFixedRows--;

	if (nRow == m_idCurrent.row)
		m_idCurrent.row = m_idCurrent.col = -1;
	else if (nRow < m_idCurrent.row)
		m_idCurrent.row--;




	UpdateRowSelectedRange(nRow, false);

	if (GetSafeHwnd())
	{
		ResetScrollBars();
		Invalidate();
	}

	return TRUE;
}

BOOL CintGrid::DeleteAllItems()
{
	ClearAllRTM(); ClearMerge(); ResetSelectedRange();

	m_aryColWidth.clear();
	m_aryRowHeight.clear();

	for (int row = 0; row < m_nRows; row++)
	{
		for (int col = 0; col < m_nCols; col++)
			SetItemState(row, col, 0);
	}

	m_aryGridRows.clear();

	m_idCurrent.row = m_idCurrent.col = -1;
	m_nRows = m_nFixedRows = m_nCols = m_nFixedCols = 0;

	if (GetSafeHwnd())
	{
		ResetScrollBars();
		Invalidate();
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Set CListCtrl::GetNextItem for details

BOOL CintGrid::SortTextItems(int nCol, BOOL bAscending)
{
	ClearAllRTM(); 
	ResetSelectedRange();

	CString	string;
	int eRow = 0;
	for (eRow = m_nRows - m_nFixedRows; eRow >= m_nFixedRows; eRow--)
	{
		string = GetItemText(eRow, nCol);
		if (!string.IsEmpty())
		{
			eRow += 1; break;
		}
	}

	if (eRow > GetRowCount()) 
		eRow = m_nRows - m_nFixedRows;

	return SortTextItems(nCol, bAscending, GetFixedRowCount(), eRow);
}

BOOL CintGrid::SortTextItems(int sortcol, BOOL ascending, int srow, int erow)
{
	if (sortcol >= GetColumnCount())
		return FALSE;
	if (erow == -1) erow = GetRowCount() -1;

	int cellType = cellNot;
	if (GetItemAttr(0, sortcol) & GVAT_SORTVAL)
		cellType = cellVal;
	else if (GetItemAttr(0, sortcol) & GVAT_SORTABS)
		cellType = cellAbs;
	else if (GetItemAttr(0, sortcol) & GVAT_SORTSTR)
		cellType = cellStr;
	else
		return FALSE;

	bool	swap = false;
	CString	string, stringx;
	for (int xrow = srow; xrow < erow-1; xrow++)
	{
		for (int yrow = xrow+1 ; yrow < erow; yrow++)
		{
			string  = GetItemText(xrow, sortcol);
			stringx = GetItemText(yrow, sortcol);

			if (GetItemAttr(xrow, sortcol) & GVAT_CONTRAST)
			{
				if (string.GetLength() > 0)
				{
					switch (string.GetAt(0))
					{
					case '1': case '2':
						string = string.Mid(1);	string.Insert(0, '+');
						break;
					case '3':
						string = string.Mid(1);
						break;
					case '4': case '5':
						string = string.Mid(1); string.Insert(0, '-');
					}
				}
				if (stringx.GetLength() > 0)
				{
					switch (stringx.GetAt(0))
					{
					case '1': case '2':
						stringx = stringx.Mid(1); stringx.Insert(0, '+');
						break;
					case '3':
						stringx = stringx.Mid(1);
						break;
					case '4': case '5':
						stringx = stringx.Mid(1); stringx.Insert(0, '-');
					}
				}
			}

			switch (cellType)
			{
			case cellVal:
				if (ascending)
				{
					if (string.IsEmpty() && !stringx.IsEmpty())
						swap = true;
					else if (!stringx.IsEmpty() && atof(string) > atof(stringx))
						swap = true;
				}
				else
				{
					if (string.IsEmpty() && !stringx.IsEmpty())
						swap = true;
					else if (!stringx.IsEmpty() && atof(string) < atof(stringx))
						swap = true;
				}
				break;
			case cellAbs:
				if (ascending)
				{
					if (string.IsEmpty() && !stringx.IsEmpty())
						swap = true;
					else if (!stringx.IsEmpty() && TOfabs(string) > TOfabs(stringx))
						swap = true;
				}
				else
				{
					if (string.IsEmpty() && !stringx.IsEmpty())
						swap = true;
					else if (!stringx.IsEmpty() && TOfabs(string) < TOfabs(stringx))
						swap = true;
				}
				break;
			case cellStr:
				if (ascending)
				{
					if (string.IsEmpty() && !stringx.IsEmpty())
						swap = true;
					else if (!stringx.IsEmpty() && string > stringx)
						swap = true;
				}
				else
				{
					if (string.IsEmpty() && !stringx.IsEmpty())
						swap = true;
					else if (!stringx.IsEmpty() && string < stringx)
						swap = true;
				}
				break;
			}

			if (swap)
			{
				CString strData;
				for (int col = 0; col < GetColumnCount(); col++)
				{
					const auto pCell = GetCell(xrow, col);
					SetCell(xrow, col, GetCell(yrow, col));
					SetCell(yrow, col, pCell);
				}
				unsigned int rowH    = m_aryRowHeight[xrow];
				m_aryRowHeight[xrow] = m_aryRowHeight[yrow];
				m_aryRowHeight[yrow] = rowH;

				//2012.04.24 KSJ ��õ���� ������.
				//���������� �׸��忡 ���ԵǾ� ������ CX_Interest������ �׸��忡 ���ԵǾ� ���� ����.
				strData = m_pParent->GetRecommandInfo(xrow);
				m_pParent->SetRecommandInfo(xrow, m_pParent->GetRecommandInfo(yrow));
				m_pParent->SetRecommandInfo(yrow, strData);
				// KSJ

				//2013.05.13 KSJ �ϸ�ũ �����Ȱ͵� ���� ���� �ǵ��� �ϱ� ���ؼ� m_inters�� ��ġ�� �ٲ� �ش�.
				//�̷� ������ ������ �ϸ� base�� ó�����õǾ� �ϸ�ũ�� ���·� ����ȴ�.....
				//_inters* pInter = nullptr;
				const auto& pInter = m_pParent->GetData(xrow-1);				
				m_pParent->SetData(xrow-1, m_pParent->GetData(yrow-1));
				m_pParent->SetData(yrow-1, pInter);
				//KSJ

				swap = false;
			}
		}
	}
	
	return TRUE;
}

BOOL CintGrid::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data)
{
	ResetSelectedRange();
	return SortItems(pfnCompare, nCol, bAscending, data, GetFixedRowCount(), -1);
}

BOOL CintGrid::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data, int low, int high)
{
	if (nCol >= GetColumnCount())
		return FALSE;
	if (high == -1)
		high = GetRowCount() - 1;

	int lo = low;
	int hi = high;

	if (hi <= lo) return FALSE;

	const LPARAM midItem = GetItemData((lo+hi)/2, nCol);

	while (lo <= hi)
	{
		if (bAscending)
		{
			while (lo < high  && pfnCompare(GetItemData(lo, nCol), midItem, data) < 0)
				++lo;
		}
		else
		{
			while (lo < high && pfnCompare(GetItemData(lo, nCol), midItem, data) > 0)
				++lo;
		}

		if (bAscending)
		{
			while (hi > low && pfnCompare(GetItemData(hi, nCol), midItem, data) > 0)
				--hi;
		}
		else
		{
			while (hi > low && pfnCompare(GetItemData(hi, nCol), midItem, data) < 0)
				--hi;
		}

		if (lo <= hi)
		{
			if (pfnCompare(GetItemData(lo, nCol), GetItemData(hi, nCol), data) != 0)
			{
				for (int col = 0; col < GetColumnCount(); col++)
				{
					const auto pCell = GetCell(lo, col);
					SetCell(lo, col, GetCell(hi, col));
					SetCell(hi, col, pCell);
				}
				UINT nRowHeight = m_aryRowHeight[lo];
				m_aryRowHeight[lo] = m_aryRowHeight[hi];
				m_aryRowHeight[hi] = nRowHeight;
			}
			++lo; --hi;
		}
	}

	if (low < hi)  SortItems(pfnCompare, nCol, bAscending, data, low, hi);
	if (lo < high) SortItems(pfnCompare, nCol, bAscending, data, lo, high);

	return TRUE;
}

BOOL CintGrid::SetItem(const GVITEM* gvitem)
{
	if (!gvitem) return FALSE;

	const auto pCell = GetCell(gvitem->row, gvitem->col);
	if (!pCell) return FALSE;

	pCell->state = 0; 
	pCell->misc = _T(""); 
	pCell->param = 0;

	if (gvitem->mask & GVMK_STATE)   pCell->state = gvitem->state;
	if (gvitem->mask & GVMK_ATTR)	 pCell->attr  = gvitem->attr;
	if (gvitem->mask & GVMK_FORMAT)  pCell->format= gvitem->format;
	if (gvitem->mask & GVMK_TEXT)    pCell->text  = gvitem->text;
	if (gvitem->mask & GVMK_IMAGE)   pCell->image = gvitem->image;
	if (gvitem->mask & GVMK_FONT)    memcpy(&(pCell->font), &(gvitem->font), sizeof(LOGFONT));
	if (gvitem->mask & GVMK_FGCOLOR) pCell->fgcol = gvitem->fgcol;
	if (gvitem->mask & GVMK_BKCOLOR) pCell->bkcol = gvitem->bkcol;
	if (gvitem->mask & GVMK_SYMBOL)	 pCell->symbol= gvitem->symbol;
	if (gvitem->mask & GVMK_MISC)	 pCell->misc  = gvitem->misc;
	if (gvitem->mask & GVMK_PARAM)   pCell->param = gvitem->param;

	if (pCell->attr & GVAT_EDIT) m_focusLine = TRUE;
	if (pCell->attr & GVAT_HIDDEN) SetColumnWidth(gvitem->col, 0);

	if (gvitem->attr & GVAT_HDRMERGE && !gvitem->misc.IsEmpty())
	{
		int key = -1; CString temp;
		CString	string = gvitem->misc;

		if ((key = string.Find('|')) != -1)
		{
			temp = string.Left(key++);
			string = string.Mid(key);
			key = atoi(temp);
		}

		_fixMerge fixMerge;
		if (!m_mapMerge.Lookup(key, fixMerge))
		{
			for (int ii = 0; ii < maxMERGE; ii++)
				fixMerge.cols[ii] = -1;

			fixMerge.cols[0] = gvitem->col;
			fixMerge.rect.SetRectEmpty();

			fixMerge.fgcol = gvitem->fgcol;
			fixMerge.bkcol = gvitem->param;
			fixMerge.format= gvitem->format;
			fixMerge.text  = string;
			CopyMemory(&(fixMerge.font), &(gvitem->font), sizeof(LOGFONT));

			m_mapMerge.SetAt(key, fixMerge);
		}
		else
		{
			for (int ii = 0; ii < maxMERGE; ii++)
			{
				if (fixMerge.cols[ii] == -1)
				{
					fixMerge.cols[ii] = gvitem->col;
					break;
				}
			}
			m_mapMerge.SetAt(key, fixMerge);
		}
	}

	return TRUE;
}

BOOL CintGrid::GetItem(GVITEM* gvitem)
{
	if (!gvitem) return FALSE;

	const auto pCell = GetCell(gvitem->row, gvitem->col);
	if (!pCell) return FALSE;

	if (gvitem->mask & GVMK_STATE)	 gvitem->state = pCell->state;
	if (gvitem->mask & GVMK_ATTR)	 gvitem->attr = pCell->attr;
	if (gvitem->mask & GVMK_FORMAT)  gvitem->format= pCell->format;
	if (gvitem->mask & GVMK_TEXT)    gvitem->text  = GetItemText(gvitem->row, gvitem->col);
	if (gvitem->mask & GVMK_IMAGE)   gvitem->image = pCell->image;
	if (gvitem->mask & GVMK_FONT)    memcpy(&(gvitem->font), &(pCell->font), sizeof(LOGFONT));
	if (gvitem->mask & GVMK_FGCOLOR) gvitem->fgcol = pCell->fgcol;
	if (gvitem->mask & GVMK_BKCOLOR) gvitem->bkcol = pCell->bkcol;
	if (gvitem->mask & GVMK_SYMBOL)	 gvitem->symbol= pCell->symbol;
	if (gvitem->mask & GVMK_MISC)	 gvitem->misc  = pCell->misc;
	if (gvitem->mask & GVMK_PARAM)   gvitem->param = pCell->param;

	return TRUE;
}

BOOL CintGrid::SetColumnItems(int nCol, GVITEM* gvitem)
{
	if (!gvitem || nCol >= GetColumnCount())
		return FALSE;

	for (int row = GetFixedRowCount(); row < GetRowCount(); row++)
	{
		gvitem->col = nCol;
		gvitem->row = row;
		SetItem(gvitem);
	}
	return TRUE;
}

BOOL CintGrid::SetItemText(int nRow, int nCol, LPCTSTR str)
{
	//CString str1;
	//if (nRow == 2 &&  (nCol == 0 || nCol ==21))
	//{	
	//	str1.Format("[cx_interest] ------------ nRow=[%d] nCol=[%d] str=[%s]\r\n", nRow, nCol, str);
	//	OutputDebugString(str1);
	//}


	const auto pCell = GetCell(nRow, nCol);
	
	if (!pCell) return FALSE;

	CString strPreText = pCell->text;

	pCell->text = str;

	CString	dstr = _T("");
	dstr = str;

	if (pCell->symbol.Compare("7777") == 0)
		dstr = _T("AAAAAAAA"); 
/*	2014.13.28 KSJ ü������������ ����ٰ� ������ ����̹Ƿ� �ּ�ó����.
	else if(pCell->symbol.Compare("7778") == 0)
	{
		dstr = str;
	}
*/
	else
		DisplayAttribute(pCell->attr, dstr);

	pCell->dtext = dstr;

	//2016.03.30 KSJ ���������Ϳ� ������ �ٽ� �׸��� �ʴ´�.
	if(strPreText.Compare(str) || nCol == colNAME || nCol == colCURR)	RedrawCell(nRow,nCol);

	return TRUE;
}

CString CintGrid::GetItemText(int nRow, int nCol)
{
	if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols)
		return "";

	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return "";

	return pCell->text;
}

CString CintGrid::GetItemDrawText(int nRow, int nCol)
{
	if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols)
		return "";

	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return "";

	return pCell->dtext;
}

BOOL CintGrid::SetItemData(int nRow, int nCol, LPARAM lParam)
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	pCell->param = lParam;

	RedrawCell(nRow, nCol);
	return TRUE;
}

LPARAM CintGrid::GetItemData(int nRow, int nCol) const
{    
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return (LPARAM) 0;

	return pCell->param;
}

BOOL CintGrid::SetItemImage(int nRow, int nCol, int image)
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	if (pCell->image != image)
	{
		pCell->image = image;
		RedrawCell(nRow, nCol);
	}
	return TRUE;
}

int CintGrid::GetItemImage(int nRow, int nCol) const
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return -1;

	return pCell->image;
}

BOOL CintGrid::SetItemState(int nRow, int nCol, UINT state)
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	// If the cell is being unselected, remove it from the selected list
	if ((pCell->state & GVST_SELECTED) && !(state & GVST_SELECTED))
	{
		CIdCell cell;
		const DWORD key = MAKELONG(nRow, nCol);

		if (m_mapSelCell.Lookup(key, (CIdCell&)cell))
			m_mapSelCell.RemoveKey(key);
	}
	else if (!(pCell->state & GVST_SELECTED) && (state & GVST_SELECTED))
	{
		CIdCell cell(nRow, nCol);
		m_mapSelCell.SetAt(MAKELONG(nRow, nCol), cell);
	}

	pCell->state = state;
	return TRUE;
}

UINT CintGrid::GetItemState(int nRow, int nCol) const
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return 0;

	return pCell->state;
}

BOOL CintGrid::SetItemAttr(int nRow, int nCol, UINT attr)
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	pCell->attr = attr;

	if (attr & GVAT_HIDDEN)
		SetColumnWidth(nCol, 0);

	return TRUE;
}

UINT CintGrid::GetItemAttr(int nRow, int nCol) const
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return 0;

	return pCell->attr;
}

BOOL CintGrid::SetItemFormat(int nRow, int nCol, UINT format)
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	pCell->format = format;
	return TRUE;
}

UINT CintGrid::GetItemFormat(int nRow, int nCol) const
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return 0;

	return pCell->format;
}

BOOL CintGrid::SetItemBkColor(int nRow, int nCol, COLORREF col, bool force)
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	COLORREF bkcol{};
	UINT	idEvent = 0;
	struct _timerID* timerID = (struct _timerID*)&idEvent;
	timerID->kind = 0;
	timerID->row = nRow;
	timerID->col = nCol;
		
	if (!force && m_mapRTM.Lookup(idEvent, bkcol))
	{
		m_sync.Lock();
		m_mapRTM.SetAt(idEvent, col);
		m_sync.Unlock();

		return TRUE;
	}

	pCell->bkcol = col;
	RedrawCell(nRow, nCol);
	if (nCol == colNAME)	memoRefresh();
	return TRUE;
}

COLORREF CintGrid::GetItemBkColor(int nRow, int nCol) const
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return 0;

	COLORREF bkcol{};
	UINT	idEvent = 0;
	struct _timerID* timerID = (struct _timerID*)&idEvent;
	timerID->kind = 0;
	timerID->row = nRow;
	timerID->col = nCol;
	if (m_mapRTM.Lookup(idEvent, bkcol))
		return bkcol;

	return pCell->bkcol;
}

BOOL CintGrid::SetItemFgColor(int nRow, int nCol, COLORREF col, bool force)
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	pCell->fgcol = col;

// updateX_20061128
	if (nRow < GetFixedRowCount() && pCell->attr & GVAT_HDRMERGE)
	{
		_fixMerge fixMerge;
		if (m_mapMerge.Lookup(nCol, fixMerge))
		{
			fixMerge.fgcol = col;
			m_mapMerge.SetAt(nCol, fixMerge);
		}
	}

	RedrawCell(nRow, nCol);
	return TRUE;
}

COLORREF CintGrid::GetItemFgColor(int nRow, int nCol) const
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return 0;

	return pCell->fgcol;
}

BOOL CintGrid::SetItemFont(int nRow, int nCol, LOGFONT* logfont)
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	memcpy(&(pCell->font), logfont, sizeof(LOGFONT));
	RedrawCell(nRow, nCol);
	return TRUE;
}

LOGFONT* CintGrid::GetItemFont(int nRow, int nCol)
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return &m_logfont;

	return &(pCell->font);
}

BOOL CintGrid::SetItemParam(int nRow, int nCol, LPARAM param)
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	pCell->param = param;

// updateX_20061128
	if (nRow < GetFixedRowCount() && pCell->attr & GVAT_HDRMERGE)
	{
		_fixMerge fixMerge;
		if (m_mapMerge.Lookup(nCol, fixMerge))
		{
			fixMerge.bkcol = param;
			m_mapMerge.SetAt(nCol, fixMerge);
		}
	}

	RedrawCell(nRow, nCol);
	return TRUE;
}

LPARAM CintGrid::GetItemParam(int nRow, int nCol)
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return (LPARAM) -1;

	return pCell->param;
}

BOOL CintGrid::SetItemSymbol(int nRow, int nCol, LPCTSTR str)
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	pCell->symbol = str;
	return TRUE;
}

CString CintGrid::GetItemSymbol(int nRow, int nCol)
{
	const auto pCell = GetCell(nRow, nCol);
	//2011.12.20 KSJ ���鸮������ ����
//	if (!pCell) return (LPARAM) -1;
	if (!pCell) return "";
	// KSJ

	return pCell->symbol;
}

BOOL CintGrid::SetItemMisc(int nRow, int nCol, LPCTSTR str)
{
	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return FALSE;

	pCell->misc = str;
	RedrawCell(nRow, nCol);
	return TRUE;
}

CString CintGrid::GetItemMisc(int nRow, int nCol)
{
	if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols)
		return "";

	const auto pCell = GetCell(nRow, nCol);
	if (!pCell) return "";

	return pCell->misc;
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column size functions

long CintGrid::GetVirtualWidth() const
{
	long width = 0;

	for (int ii = 0; ii < GetColumnCount(); ii++)
		width += m_aryColWidth[ii];

	return gsl::narrow_cast<long>(width * m_xRate);
}

long CintGrid::GetVirtualHeight() const
{
	long height = 0;

	CString strTemp;

	for (int ii = 0; ii < GetRowCount(); ii++)
	{
		height += m_aryRowHeight[ii];
	}

	return gsl::narrow_cast<long>(height * m_yRate);
}

int CintGrid::GetRowHeight(int nRow) const
{
	ASSERT(nRow >= 0 && nRow < m_nRows);
	if (nRow < 0 || nRow >= m_nRows)
		return -1;

	return gsl::narrow_cast<int>(m_aryRowHeight[nRow] * m_yRate);
}

int CintGrid::GetColumnWidth(int nCol) const
{
//	ASSERT(nCol >= 0 && nCol < m_nCols);  //test 20230207
	if (nCol < 0 || nCol >= m_nCols)
		return -1;

	return gsl::narrow_cast<int>(m_aryColWidth[nCol] * m_xRate);
}

//2015.02.25 KSJ ��üȭ�������� x_rate �����ϴ°�
int CintGrid::GetRealColumnWidth(int nCol) const
{
	ASSERT(nCol >= 0 && nCol < m_nCols);
	if (nCol < 0 || nCol >= m_nCols)
		return -1;
	
	return m_aryColWidth[nCol];
}

BOOL CintGrid::SetRowHeight(int nRow, int height)
{
	ASSERT(nRow >= 0 && nRow < m_nRows && height >= 0);
	if (nRow < 0 || nRow >= m_nRows || height < 0)
		return FALSE;

	m_aryRowHeight[nRow] = height;
	return TRUE;
}

BOOL CintGrid::SetColumnWidth(int nCol, int width)
{
	ASSERT(nCol >= 0 && nCol < m_nCols && width >= 0);
	if (nCol < 0 || nCol >= m_nCols || width < 0)
		return FALSE;

	m_aryColWidth[nCol] = width;
	return TRUE;
}

int CintGrid::GetFixedRowHeight() const
{
	int height = 0;
	for (int ii = 0; ii < m_nFixedRows; ii++)
		height += GetRowHeight(ii);

	return height;
}

int CintGrid::GetFixedColumnWidth() const
{
	int width = 0;
	for (int ii = 0; ii < m_nFixedCols; ii++)
		width += GetColumnWidth(ii);

	return width;
}

BOOL CintGrid::AutoSizeColumn(int nCol)
{
	ASSERT(nCol >= 0 && nCol < m_nCols);
	if (nCol < 0 || nCol >= m_nCols)
		return FALSE;

	CSize size;
	CDC* pDC = GetDC();
	if (!pDC) return FALSE;

	int width = 0;
	for (int nRow = m_nFixedRows; nRow < GetRowCount(); nRow++)
	{
		size = GetCellExtent(nRow, nCol, pDC);
		if (size.cx > width)
			width = size.cx;
	}

	m_aryColWidth[nCol] = width;

	ReleaseDC(pDC);
	ResetScrollBars();

	return TRUE;
}

BOOL CintGrid::AutoSizeRow(int nRow)
{
	ASSERT(nRow >= 0 && nRow < m_nRows);
	if (nRow < 0 || nRow >= m_nRows)
		return FALSE;

	CSize size;
	CDC* pDC = GetDC();
	if (!pDC) return FALSE;

	int height = 0;
	for (int nCol = 0; nCol < GetColumnCount(); nCol++)
	{  
		size = GetCellExtent(nRow, nCol, pDC);
		if (size.cy > height)
			height = size.cy;
	}  
	m_aryRowHeight[nRow] = height;

	ReleaseDC(pDC);
	ResetScrollBars();

	return TRUE;
}

// jdh
void CintGrid::AutoSizeColumns()
{
	for (int nCol = 0; nCol < GetColumnCount(); nCol++)
		AutoSizeColumn(nCol);
}

void CintGrid::AutoSizeRows()
{
	for (int nRow = 0; nRow < GetRowCount(); nRow++)
		AutoSizeRow(nRow);
}
void CintGrid::AutoSize()
{
	CDC* pDC = GetDC();
	if (!pDC) return;

	const int nNumColumns = GetColumnCount();
	const int nNumRows = GetRowCount();
    
	for (int nCol = 0; nCol < nNumColumns; nCol++)
		m_aryColWidth[nCol] = 0;
    
	for (int nRow = 0; nRow < nNumRows; nRow++)
		m_aryRowHeight[nRow] = 0;
    
	CSize size;
	for (int nCol = 0; nCol < nNumColumns; nCol++)
	{
		for (int nRow = 0; nRow < nNumRows; nRow++)
		{
			size = GetCellExtent(nRow, nCol, pDC);
			if (size.cx > (int) m_aryColWidth[nCol])  m_aryColWidth[nCol] = size.cx;
			if (size.cy > (int) m_aryRowHeight[nRow]) m_aryRowHeight[nRow] = size.cy;
		}
	}

	ReleaseDC(pDC);

	ResetScrollBars();
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////////////
// GridCtrl cell visibility tests and invalidation/redraw functions
void CintGrid::EnsureVisible(int nRow, int nCol)
{
	const CRangeCell visibleCells = GetVisibleNonFixedCellRange();

	int right = nCol - visibleCells.GetMaxCol();
	int left  = visibleCells.GetMinCol() - nCol;
	int down  = nRow - visibleCells.GetMaxRow();
	int up    = visibleCells.GetMinRow() - nRow;
	
	m_noMessage = true;

	while (right > 0)
	{
		SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
		right--;
	}
	while (left > 0)
	{
		SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
		left--;
	}
	while (down > 0)
	{
		SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
		down--;
	}
	while (up > 0)
	{
		SendMessage(WM_VSCROLL, SB_LINEUP, 0);
		up--; 
	}

	CRect cellRC, winRC;
	GetCellRect(nRow, nCol, cellRC);
	GetClientRect(winRC);

	if (cellRC.right > winRC.right)
		SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
	if (cellRC.bottom > winRC.bottom)
		SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);

	m_noMessage = false;
}

BOOL CintGrid::IsCellAttribute(const CIdCell& cell, UINT attr) const
{
	CIdCell hidden = cell;
	if (attr == GVAT_HIDDEN && m_nFixedRows > 0)
		hidden.row = 0;

	return ((GetItemAttr(hidden.row, hidden.col) & attr) == attr);
}

BOOL CintGrid::IsCellVisible(CIdCell cell) const
{  
	return IsCellVisible(cell.row, cell.col);
}

// jdh
BOOL CintGrid::IsCellVisible(int nRow, int nCol) const
{  
	if (!IsWindow(m_hWnd))
		return FALSE;
	if (IsCellAttribute(CIdCell(nRow, nCol), GVAT_HIDDEN))
		return FALSE;

	int cx{}, cy{};

	CIdCell tlCell;

	if (nCol >= GetFixedColumnCount() || nRow >= GetFixedRowCount())
	{
		tlCell = GetTopleftNonFixedCell();
		if (nCol >= GetFixedColumnCount() && nCol < tlCell.col)
			return FALSE;
		if (nRow >= GetFixedRowCount() && nRow < tlCell.row)
			return FALSE;
	}

	CRect rect; 
	GetClientRect(rect);
	
	if (nCol < GetFixedColumnCount())
	{
		cx = 0;
		for (int ii = 0; ii <= nCol; ii++) 
		{
			if (cx >= rect.right)
				return FALSE;
			cx += GetColumnWidth(ii);    
		}
	} 
	else 
	{
		cx = GetFixedColumnWidth();
		for (int ii = tlCell.col; ii <= nCol; ii++) 
		{
			if (cx >= rect.right)
				return FALSE;
			cx += GetColumnWidth(ii);    
		}
	}

	if (nRow < GetFixedRowCount())
	{
		cy = 0;
		for (int ii = 0; ii <= nRow; ii++) 
		{
			if (cy >= rect.bottom)
				return FALSE;
			cy += GetRowHeight(ii);    
		}
	} 
	else 
	{
		if (nRow < tlCell.row)
			return FALSE;
		cy = GetFixedRowHeight();
		for (int ii = tlCell.row; ii <= nRow; ii++) 
		{
			if (cy >= rect.bottom)
				return FALSE;
			cy += GetRowHeight(ii);    
		}
	}

	return TRUE;
}

BOOL CintGrid::InvalidateCellRect(const CIdCell& cell)
{
	if (!::IsWindow(GetSafeHwnd()))
		return FALSE;

	if (!IsValid(cell) || !IsCellVisible(cell.row, cell.col))
		return FALSE;

	CRect rect;
	if (!GetCellRect(cell.row, cell.col, rect))
		return FALSE;

	rect.right++; rect.bottom++;
	InvalidateRect(rect, TRUE);

	return TRUE;
}

BOOL CintGrid::InvalidateCellRect(const CRangeCell& range)
{
	ASSERT(IsValid(range));
	if (!::IsWindow(GetSafeHwnd()))
		return FALSE;

	const CRangeCell visibleCellRange = GetVisibleNonFixedCellRange().Intersect(range);

	CRect rect;
	if (!GetCellRangeRect(visibleCellRange, rect))
		return FALSE;

	rect.right++; rect.bottom++;
	InvalidateRect(rect, TRUE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CintGrid Mouse stuff
BOOL CintGrid::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (m_NOscroll == GVSC_NONE)
		return CWnd::OnMouseWheel(nFlags, zDelta, pt);

	SCROLLINFO info;

	GetScrollInfo(SB_VERT,&info,SIF_ALL);

	if(info.nPage == info.nMax)
	{
		return CWnd::OnMouseWheel(nFlags, zDelta, pt);
	}

	if (m_rowCountWheel == -1)
	{
		const int scrollPage = zDelta / 120;
		if (scrollPage > 0)
		{
			for (int ii = 0; ii < scrollPage; ii++)
				PostMessage(WM_VSCROLL, SB_PAGEUP, 0);
		}
		else
		{
			for (int ii = 0; ii > scrollPage; ii--)
				PostMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
		}
	}
	else
	{
		const int scrollPage = m_rowCountWheel * zDelta / 120;
		if (scrollPage > 0)
		{
			for (int ii = 0; ii < scrollPage; ii++)
				PostMessage(WM_VSCROLL, SB_LINEUP, 0);
		}
		else
		{
			for (int ii = 0; ii > scrollPage; ii--)
				PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
		}
	}
	
	Invalidate();
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CintGrid::OnMouseMove(UINT nFlags, CPoint point) 
{

	CRect rect; 
	GetClientRect(rect);

	//if( nFlags & MK_LBUTTON  )
	//	TRACE("OnMouseMove\n");

	if (m_nRows <= 0 && m_nCols <= 0)
		return;
	if (m_mousemode != mouseDRAGGING && !rect.PtInRect(point))
		return;

	CDC* pDC{};
	CRect	oldRC, newRC, cellRC;
	CPoint	cellpt;
	CIdCell	current;

	if (!(nFlags & MK_LBUTTON) || (m_mousemode == mouseNOTHING && (nFlags & MK_LBUTTON)))
	{
		if (m_enableColResize && IsColResizeArea(point))
		{
			if (m_mousemode != mouseOVERCOLDIVIDE)
			{
				current = GetCellFromPoint(point);
				GetCellRect(current.row, current.col, cellRC);
				
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
				m_mousemode = mouseOVERCOLDIVIDE;
			}
		}
		else if (m_enableRowResize && IsRowResizeArea(point))
		{
			if (m_mousemode != mouseOVERROWDIVIDE)
			{
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
				m_mousemode = mouseOVERROWDIVIDE;
			}
		}
		else if (m_mousemode != mouseNOTHING)
		{
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			m_mousemode = mouseNOTHING;
		}

		if (m_mousemode == mouseNOTHING && m_enabletip)
		{
			CRect textRC, cellRC;
			current = GetCellFromPoint(point);
			if (GetTextRect(current.row, current.col, textRC)
				&& GetCellRect(current.row, current.col, cellRC))
			{
				UINT idEvent = 0;
				struct _timerID* timerID = (struct _timerID*)&idEvent;
				
				// MODIFY PSH 20070912
				//if (current.row == 0)
				if (m_bHeadSet && (current.row == 0))
				// END MODIFY
				{
					if (current.col == colSIG)
					{
// 						CString	string;
// 						switch (GetTypeInRect(cellRC, point))
// 						{
// 						case TK_CHART:
// 							string.Format("[��Ʈ����/�ü�����]\n���񺰷� ��Ʈ ������ ���� �� ������\n�ü����� ������ ���� ������ ������ ǥ���մϴ�.");
// 							break;
// 						case TK_NEWS:
// 							string.Format("[��������]\n���� ǥ�ð� �ִ� ���� ���Ͽ�\n������ �˻��� �� �ֽ��ϴ�.");
// 							break;
// 						}
// 						CRect	rect;
// 						GetClientRect(rect);
// 						if (string.Compare(m_pTooltip->GetTip()))
// 						{
// 							m_pTooltip->ShowTips(cellRC, rect, string);
// 						}
					}
					else
					{
						timerID->kind = TK_CONTENT;
						timerID->row = current.row;
						timerID->col = current.col;
						SetTimer(idEvent, 200, nullptr);
					}
				}
				else
				{
					if (IsCellAttribute(current, GVAT_TOOLTIP))
					{
						if (current.col == colSIG)
						{
							timerID->kind = GetTypeInRect(cellRC, point);
							timerID->row = current.row;
							timerID->col = colSIG;
							SetTimer(idEvent, 300, nullptr);					
						}
						else if (current.col == colNAME)
						{
							//�޸����� �ִ��� Ȯ��
							if (m_memoRGN.PtInRegion(point))
							{
 								timerID->kind = TK_MEMO;
 								timerID->row = current.row;
 								timerID->col = current.col;
 								SetTimer(idEvent, 200, nullptr);
							}
							else
							{
								if (m_idCurrent.row == current.row)
									SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
		//							SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
								else
									SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
								timerID->kind = TK_CODE;
								timerID->row = current.row;
								timerID->col = current.col;
								SetTimer(idEvent, 200, nullptr);
							}

						}
						else if (current.col == colCODE)
						{
							if (m_idCurrent.row == current.row)
								SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
							else
								SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

							timerID->kind = TK_CODE;//TK_RCODE;
							timerID->row = current.row;
							timerID->col = current.col;
							SetTimer(idEvent, 200, nullptr);
						}
						else
						{
							CString symbol = GetItemSymbol(current.row, current.col);
							if (symbol.Compare("7777") == 0)
							{
								timerID->kind = TK_BONG;
								timerID->row = current.row;
								timerID->col = current.col;
								SetTimer(idEvent, 200, nullptr);
							}
						}					
					}
					else
					{
						if (IsCellAttribute(current, GVAT_EDIT))
						{
							if (m_idCurrent.row == current.row)
								SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
							else
								SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
						}
					}
				}
			}
		}

// updateX_20061124
		if (m_mousemode == mouseNOTHING && m_enableMouseTrace)
		{
			current = GetCellFromPoint(point);
			if (!IsValid(current)) return;
			if (current != GetFocusCell())
			{
				ResetSelectedRange();
				m_idSelect = CIdCell(current.row, 0);
				SelectRows(current);
			}
		}

		m_ptLast = point;
		CWnd::OnMouseMove(nFlags, point);
		return;
	}

	if (!IsValid(m_idClick))
	{
		m_ptLast = point; return;
	}

	if (nFlags & MK_LBUTTON) 
	{
		switch (m_mousemode)
		{
		case mouseSELECTALL:        
			break;
		case mouseSELECTCOL:
		case mouseSELECTROW:    
		case mouseSELECTCELLS:    
			current = GetCellFromPoint(point);
			if (!IsValid(current)) 
				return;

			if (current != GetFocusCell())
			{
				OnSelecting(current);
				if (m_focusLine && current.row >= m_nFixedRows && current.col >= m_nFixedCols)
					SetFocusCell(current);
				MessageToParent(m_idDrag.row,m_idDrag.col, GVNM_TRIGGER);//FOR TRIGGER
// 				TRACE("MessageToParent11 row : %d", m_idDrag.row);
			}
			
			break;
		case mouseMOVING:

			if (!(pDC = GetDC())) break;
			
			current = GetCellFromPoint(point);
			if (!IsValid(current))
			{
				ReleaseDC(pDC);
				return;
			}
			if (current.col < m_nFixedCols || current.col < m_keepcolumn)
			{
				ReleaseDC(pDC);
				return;
			}

			GetCellRect(current.row, current.col, cellRC);

			cellpt.x = cellRC.left; cellpt.y = point.x;

			oldRC.SetRect(m_ptMove.x, rect.top, m_ptMove.x +2, rect.bottom);
			pDC->InvertRect(&oldRC);

			newRC.SetRect(cellpt.x, rect.top, cellpt.x +2, rect.bottom);
			pDC->InvertRect(&newRC);

			m_ptMove = cellpt;
			ReleaseDC(pDC);
			break;

		case mouseSIZINGCOL:
			if (!(pDC = GetDC())) break;

			current = GetCellFromPoint(point);
			oldRC.SetRect(m_ptLast.x, rect.top, m_ptLast.x + 2, rect.bottom);
			pDC->InvertRect(&oldRC);
			newRC.SetRect(point.x, rect.top, point.x + 2, rect.bottom);
			pDC->InvertRect(&newRC);

			m_ptLast = point;
			ReleaseDC(pDC);
			break;
		case mouseSIZINGROW:      
			if (!(pDC = GetDC())) break;

			oldRC.SetRect(rect.left, m_ptLast.y, rect.right, m_ptLast.y +2);
			pDC->InvertRect(&oldRC);
			newRC.SetRect(rect.left, point.y, rect.right, point.y +2);
			pDC->InvertRect(&newRC);
			m_ptLast = point;

			ReleaseDC(pDC);
			break;
		case mousePREPAREDRAG:
// 			TRACE("OnMouseMove::OnBeginDrag");
			OnBeginDrag(); 
			break;
		}    
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CintGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_mousemode == mouseOVERCOLDIVIDE) 
	{
		CIdCell cell = GetCellFromPoint(point);
		ASSERT(IsValid(cell));

		CPoint start;
		if (!GetCellOrigin(0, cell.col, &start))
			return;

		if (cell.col <= colCODE)
			return;

		if (point.x - start.x <= rszCAPTURE)     // Clicked right of border
			cell.col--;

		AutoSizeColumn(cell.col); Invalidate();
// 		TRACE("MessageToParent12 row : %d", cell.row);
		MessageToParent(cell.row, cell.col, GVNM_CHGCOLSIZE);
	} 
	else if (m_mousemode == mouseOVERROWDIVIDE)
	{
		CIdCell cell = GetCellFromPoint(point);
		ASSERT(IsValid(cell));

		CPoint start;
		if (!GetCellOrigin(0, cell.col, &start))
			return;

		if (point.y - start.y <= rszCAPTURE)     // Clicked below border
			cell.row--;

		AutoSizeRow(cell.row); Invalidate();
// 		TRACE("MessageToParent13 row : %d", cell.row);
		MessageToParent(cell.row, cell.col, GVNM_CHGROWSIZE);
	}
	else if (m_mousemode == mouseNOTHING)
	{
		const CIdCell cell = GetCellFromPoint(point);
		if (IsValid(cell))
		{
			if (IsCellAttribute(cell, GVAT_SORTVAL)
				|| IsCellAttribute(cell, GVAT_SORTABS) || IsCellAttribute(cell, GVAT_SORTSTR))
			{
				CWaitCursor waiter;
				if (cell.col == m_sortColumn)
				{
					if (TRUE == m_ascending)
					{
						//2012.05.09 KSJ ���������϶��� MK_SORTBASE ������ ����
						//���������϶� 3�ܰ� �����ϸ� �����׷��� ���̴� ������ �߻�
						if (((CMainWnd*)m_pMainWnd)->m_bRemain == TRUE)	
						{
							CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
							pWnd->SendMessage(WM_MANAGE, MK_TURNCODE);
							
							if(((CMainWnd*)m_pMainWnd)->m_bRemain)
							{
								CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
								
								pWnd->SendMessage(WM_MANAGE,MK_REMAIN);
							}
						}
						// KSJ
						else
						{
							GetParent()->PostMessage(WM_MANAGE, MK_SORTBASE);
						}
					}
					
//					m_ascending = !m_ascending;
					m_ascending++;

					if(m_ascending > 2)
						m_ascending = FALSE;
				}
				else 
				{
					m_ascending = FALSE; m_sortColumn = cell.col;
				}

				if(m_ascending == 2)
				{
					if (((CMainWnd*)m_pMainWnd)->m_bRemain != TRUE)		//2012.05.09 KSJ ���������϶��� GVNM_REFRESH ������ ����
						MessageToParent(m_sortColumn, 0, GVNM_REFRESH);
				}
				else
				{
					SortTextItems(m_sortColumn, m_ascending);
					MessageToParent(m_sortColumn, 0, GVNM_ENDSORT);
				}
				
				Invalidate();
			}
			
			MessageToParent(cell.row, cell.col, GVNM_DBLCLICKED);
		}
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CintGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	HideCode();
	GetParent()->PostMessage(WM_MANAGE, MK_CLICKGROUP);

	if (m_memoRGN.PtInRegion(point))
	{
		const CIdCell click = GetCellFromPoint(point, TRUE);

		if (click.row >= GetFixedRowCount() && m_memoAry.GetAt(click.row - 1))
		{
			const int cx = 225, cy = 150;

			CString	code = GetItemText(click.row, codeCOL);
			CString	name = GetItemText(click.row, nameCOL);
			CRect	rect; rect.SetRect(point.x, point.y, point.x + cx, point.y + cy);
			ClientToScreen(&rect); m_updateRow = click.row; memoWindow(code, name, rect);
			return;
		}
	}

	HWND hOldFocusWnd = ::GetFocus();
	
	m_ptClick = point;
	m_idClick = GetCellFromPoint(point, TRUE);

	if (!IsValid(m_idClick)) return;

	SetFocus();
	m_idSelect = (nFlags & MK_SHIFT) ? m_idCurrent : m_idClick;

	if (m_idClick == m_idCurrent)
	{
		if (IsCellAttribute(m_idClick, GVAT_EDIT))
			m_mousemode = mouseEDITCELL;
		else if (IsCellAttribute(m_idClick, GVAT_CHECK))
			m_mousemode = mouseCHECKCELL;
		else if (IsCellAttribute(m_idClick, GVAT_COMBO))
			m_mousemode = mouseCOMBOCELL;
		else if (IsCellAttribute(m_idClick, GVAT_DATE))
			m_mousemode = mouseDATECELL;
		else if (IsCellAttribute(m_idClick, GVAT_TIME))
			m_mousemode = mouseTIMECELL;
		
		switch (m_mousemode)
		{
		case mouseEDITCELL:
		case mouseCHECKCELL:
		case mouseCOMBOCELL:
		case mouseDATECELL:
		case mouseTIMECELL:
		case mouseCODECELL:
// 			TRACE("MessageToParent15 row : %d", m_idClick.row);
			MessageToParent(m_idClick.row, m_idClick.col, GVNM_LMOUSEDOWN);
			break;  //return; <-- �̰Ͷ����� �巡�װ� �Ұ��ϴ�
		}
	}   
//	else if (m_mousemode != mouseOVERCOLDIVIDE && m_mousemode != mouseOVERROWDIVIDE) 
	if (m_mousemode != mouseOVERCOLDIVIDE && m_mousemode != mouseOVERROWDIVIDE)
	{
		SetFocusCell(-1,-1); 
		SetFocusCell(max(m_idClick.row, m_nFixedRows), max(m_idClick.col, colSIG + 1));

		if (IsCellAttribute(m_idClick, GVAT_DRAGDROP)) // && hOldFocusWnd == GetSafeHwnd())
		{
			SelectRows(m_idClick);
//			TRACE("NBTNDOWN tmINTERVAL %d\n", tmINTERVAL);
			m_timerID = SetTimer(WM_LBUTTONDOWN, tmINTERVAL, 0);
		}
		else if (IsCellAttribute(m_idClick, GVAT_CHECK))
		{
			SelectRows(m_idClick);
			m_mousemode = mouseCHECKCELL;
		}
#ifdef	_winix
		else if (IsCellAttribute(m_idClick, GVAT_COMBO))
		{
			SelectRows(m_idClick);
			m_mousemode = mouseCOMBOCELL;
		}
		else if (IsCellAttribute(m_idClick, GVAT_DATE))
		{
			SelectRows(m_idClick);
			m_mousemode = mouseDATECELL;
		}
		else if (IsCellAttribute(m_idClick, GVAT_TIME))
		{
			SelectRows(m_idClick);
			m_mousemode = mouseTIMECELL;
		}
		else if (IsCellAttribute(m_idClick, GVAT_CODE))
		{
			SelectRows(m_idClick);
			m_mousemode = mouseCODECELL;
		}
#endif
	}

	SetCapture();


	if (m_mousemode == mouseNOTHING)
	{
		if (m_enableColResize && IsColResizeArea(point))
		{
			if (m_mousemode != mouseOVERCOLDIVIDE)
			{
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
				m_mousemode = mouseOVERCOLDIVIDE;
			}

		}
		else if (m_enableRowResize && IsRowResizeArea(point))
		{
			if (m_mousemode != mouseOVERROWDIVIDE)
			{
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
				m_mousemode = mouseOVERROWDIVIDE;
			}
		}
		else if (m_idClick.row >= GetFixedRowCount() && GetCapture()->GetSafeHwnd() == GetSafeHwnd())
		{
			m_timerID = SetTimer(WM_LBUTTONDOWN, tmAUTOSCRL/10, 0);
			m_timerID = SetTimer(WM_LBUTTONDOWN, tmAUTOSCRL, 0);
//			TRACE("NBTNDOWN tmAUTOSCRL %d\n", tmAUTOSCRL);
			//090807 ������ ������ Ŭ���� ������ �ȵǴ� �κ� �־� �߰� ����
//			TRACE("OnLButtonDown::OnBeginDrag");
			m_isLbtnClick = TRUE;				//Ŭ������ 091013 ����
			m_mousemode = mousePREPAREDRAG;
			OnBeginDrag();
//			TRACE("OnLButtonDown::OnBeginDrag mousemode : %d", m_mousemode);
//			TRACE("tmAUTOSCRL : %d", tmAUTOSCRL);
		}
		else{

			if (m_idClick.col == colSIG)
			{
				const WORD	high = 0;
				const WORD	news = 1;
					
				for(int ii=1 ; ii<GetRowCount() ; ii++)
				{
					SetItemData(ii, colSIG, MAKELPARAM(news, high));
				}
			}

			if (IsCellAttribute(m_idClick, GVAT_SORTVAL)
				|| IsCellAttribute(m_idClick, GVAT_SORTABS) 
				|| IsCellAttribute(m_idClick, GVAT_SORTSTR))
			{
				CWaitCursor waiter;
				if (m_idClick.col == m_sortColumn)
				{
					if (TRUE == m_ascending)
					{
						//2012.05.09 KSJ ���������϶��� MK_SORTBASE ������ ����
						//���������϶� 3�ܰ� �����ϸ� �����׷��� ���̴� ������ �߻�
						if (((CMainWnd*)m_pMainWnd)->m_bRemain == TRUE)	
						{
							CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
							pWnd->SendMessage(WM_MANAGE, MK_TURNCODE);
							
							if(((CMainWnd*)m_pMainWnd)->m_bRemain)
							{
								CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
								
								pWnd->SendMessage(WM_MANAGE,MK_REMAIN);
							}
						}
						// KSJ
						else
						{
							GetParent()->PostMessage(WM_MANAGE, MK_SORTBASE);
						}
					}

//					m_ascending = !m_ascending;
					m_ascending++;
					
					if(m_ascending > 2)
						m_ascending = FALSE;

				}
				else 
				{
					m_ascending = FALSE; m_sortColumn = m_idClick.col;
				}

				
				if(m_ascending == 2)
				{
					if (((CMainWnd*)m_pMainWnd)->m_bRemain != TRUE)		//2012.05.09 KSJ ���������϶��� GVNM_REFRESH ������ ����
						MessageToParent(m_sortColumn, 0, GVNM_REFRESH);
				}
				else
				{
					SortTextItems(m_sortColumn, m_ascending);
					MessageToParent(m_sortColumn, 0, GVNM_ENDSORT);
				}

				
				Invalidate();
			}
		}	
	}

	if (m_mousemode == mouseOVERCOLDIVIDE) // sizing column
	{
		if (m_idClick.col <= colSIG && point.x < m_defHeight + 10)
		{
// 			TRACE("ReleaseCapture");
			ReleaseCapture();
			return;
		}
		m_mousemode = mouseSIZINGCOL;
		CPoint start;
		if (!GetCellOrigin(0, m_idClick.col, &start))
			return;

		CRect rect; GetClientRect(rect);
		const CRect invertedRC(point.x, rect.top, point.x + 2, rect.bottom);

		CDC* pDC = GetDC();
		if (pDC)
		{
			pDC->InvertRect(&invertedRC);
			ReleaseDC(pDC);
		}

		if (point.x - start.x <= rszCAPTURE)        // clicked right of border
			if (!GetCellOrigin(0, --m_idClick.col, &start))
				return;

		rect.left = start.x;
		ClientToScreen(rect);

		ClipCursor(rect);
	}
	else if (m_mousemode == mouseOVERROWDIVIDE) // sizing row
	{
		m_mousemode = mouseSIZINGROW;
		CPoint start;
		if (!GetCellOrigin(m_idClick.row, m_idClick.col, &start))
			return;

		CRect rect; GetClientRect(rect);
		const CRect invertedRC(rect.left, point.y, rect.right, point.y + 2);

		CDC* pDC = GetDC();
		if (pDC)
		{
			pDC->InvertRect(&invertedRC);
			ReleaseDC(pDC);
		}

		if (point.y - start.y <= rszCAPTURE)            // clicked below border
			if (!GetCellOrigin(--m_idClick.row, 0, &start))
				return;

		rect.top = start.y;
		ClientToScreen(rect);
		ClipCursor(rect);
	}
	// MODIFY PSH 20070911
	//else
	else if (m_bEnSelect && m_mousemode != mouseEDITCELL)
	// END MODIFY
	{    
		m_mapPreCell.RemoveAll();
		if (nFlags & MK_CONTROL)
		{
			for (POSITION pos = m_mapSelCell.GetStartPosition(); pos != nullptr; )
			{
				DWORD key; CIdCell cell;
				m_mapSelCell.GetNextAssoc(pos, key, (CIdCell&)cell);
				m_mapPreCell.SetAt(key, cell);
			}
		}

		if (m_idClick.row < GetFixedRowCount())
			OnFixedRowClick(m_idClick);
		else if (m_idClick.col < GetFixedColumnCount())
			OnFixedColumnClick(m_idClick);
		else if (m_mousemode == mouseNOTHING)		
		{
			if (IsCellAttribute(m_idClick, GVAT_ONLYROW))
				m_mousemode = mouseSELECTROW;
			else
				m_mousemode = (m_dragdrop == GVDD_NONE) ? mouseSELECTCELLS : mouseSELECTROW;
			OnSelecting(m_idClick);
		}

		if(m_isLbtnClick == TRUE)		//091013 ����  L��ư Ŭ���� ���� ȿ�� �ش�
		{
			OnSelecting(m_idClick);
			m_isLbtnClick = FALSE;
		}
	}
	
// 	TRACE("MessageToParent16 row : %d", m_idClick.row);
	MessageToParent(m_idClick.row, m_idClick.col, GVNM_LMOUSEDOWN);
	m_ptLast = point;
}

void CintGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);
	
//	TRACE("OnLButtonUp::m_isLbtnClick : %d\n", m_isLbtnClick);
	m_isLbtnClick = FALSE;				//Ŭ������ 091013 ����
	ClipCursor(nullptr);
	if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
	{
		ReleaseCapture();
		KillTimer(m_timerID);
		m_timerID = 0;
	}

	switch (m_mousemode)
	{
	case mouseEDITCELL:
		{
			if (m_bHeadSet)
			{
				m_bDrag = TRUE;

				OnEditCell(m_idCurrent.row, m_idCurrent.col, VK_END);
			}
		}
		break;
	case mouseCHECKCELL:
		{
			LPARAM param = GetItemParam(m_idClick.row, m_idClick.col);
			param = param == 0 ? 1 : 0;
			SetItemParam(m_idClick.row, m_idClick.col, param);
			InvalidateCellRect(m_idClick);

			MessageToParent(m_idClick.row, m_idClick.col, GVNM_LMOUSEUP);
		}
		break;
	case mouseCOMBOCELL:
		{
			if (m_bHeadSet)
				OnComboCell(m_idClick.row, m_idClick.col);
		}
		break;
	case mouseDATECELL:
		OnDateCell(m_idClick.row, m_idClick.col);
		break;
	case mouseTIMECELL:
		OnTimeCell(m_idClick.row, m_idClick.col);
		break;
	case mousePREPAREDRAG:
		ResetSelectedRange();
		break;
	case mouseMOVING:
		{
			CRect	rect; GetClientRect(rect);
			const CRect	invertedRC(m_ptMove.x, rect.top, m_ptMove.x +2, rect.bottom);

			CDC* pDC = GetDC();
			if (pDC)
			{
				pDC->InvertRect(&invertedRC);
				ReleaseDC(pDC);
			}

			if (m_ptClick != point)
			{
				const CIdCell click = GetCellFromPoint(m_ptClick);
				const CIdCell move  = GetCellFromPoint(point);
				if (move.col >= m_nFixedCols && move.col >= m_keepcolumn)
				{
					MoveColAction(click.col, move.col);
// 					TRACE("MessageToParent18 row : %d", move.col);
					MessageToParent(move.col, click.col, GVNM_COLUMNMOVE);
				}
			}
			ResetScrollBars(); Invalidate();
		}
		break;
	case mouseSIZINGCOL:
		{
			CRect rect; GetClientRect(rect);
			ScreenToClient(&rect);
			const CRect invertedRC(m_ptLast.x, rect.top, m_ptLast.x + 2, rect.bottom);

			CDC* pDC = GetDC();
			if (pDC)
			{
				pDC->InvertRect(&invertedRC);
				ReleaseDC(pDC);
			}

			if (m_ptClick != point) 
			{   
				CPoint start;
				if (!GetCellOrigin(m_idClick.row, m_idClick.col, &start))
					return;

				ScreenToClient(&point);
				ScreenToClient(&start);

// 				int nOldLength = GetRealColumnWidth(m_idClick.col);
				SetColumnWidth(m_idClick.col, point.x - start.x);
// 				int nLength = GetRealColumnWidth(m_idClick.col);

				ResetScrollBars(); Invalidate();
				
/*	2014.13.28 KSJ ü������������ ����ٰ� ������ ����̹Ƿ� �ּ�ó����.
				if(m_isEqualizerField)
					MakeEqualizerBmp();
*/

// 				TRACE("MessageToParent19 row : %d", m_idClick.row);
				MessageToParent(m_idClick.row, m_idClick.col, GVNM_CHGCOLSIZE);
			}
		}
		break;
	case mouseSIZINGROW:
		{
			CRect rect; GetClientRect(rect);
			const CRect invertedRC(rect.left, m_ptLast.y, rect.right, m_ptLast.y + 2);

			CDC* pDC = GetDC();
			if (pDC)
			{
				pDC->InvertRect(&invertedRC);
				ReleaseDC(pDC);
			}

			if (m_ptClick != point) 
			{
				CPoint start;
				if (!GetCellOrigin(m_idClick.row, m_idClick.col, &start))
					return;

				SetRowHeight(m_idClick.row, point.y - start.y);
				ResetScrollBars(); Invalidate();

// 				TRACE("MessageToParent20 row : %d", m_idClick.row);
				MessageToParent(m_idClick.row, m_idClick.col, GVNM_CHGROWSIZE);
			}
		}
		break;
	default:
		{
			const CIdCell upCell  = GetCellFromPoint(point);
// 			TRACE("MessageToParent21 row : %d", upCell.row);
			MessageToParent(upCell.row, upCell.col, GVNM_LMOUSEUP);
		}
		break;
	}
// 	TRACE("MessageToParent22 row : %d", m_idDrag.row);
	if(m_bDrag == FALSE)
	{
		MessageToParent(m_idDrag.row,m_idDrag.col, GVNM_TRIGGER);
	}
	else
	{
		m_bDrag = FALSE;
	}

	m_mousemode = mouseNOTHING;
	
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	if (!IsValid(m_idClick))
		return;

	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->PostMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), BN_CLICKED), (LPARAM) GetSafeHwnd());
}

void CintGrid::OnRButtonDown(UINT nFlags, CPoint point) 
{
	const CIdCell Rclick = GetCellFromPoint(point, TRUE);


	if (!IsValid(Rclick)) return;


	MessageToParent(Rclick.row, Rclick.col, GVNM_RMOUSEDOWN);
	CWnd::OnRButtonDown(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// CintGrid printing 
void CintGrid::Print() 
{
	CDC dc;
	CPrintDialog printDlg(FALSE);

	if (printDlg.DoModal() != IDOK)
		return;

	dc.Attach(printDlg.GetPrinterDC());
	dc.m_bPrinting = TRUE;

	CString strTitle;
	strTitle.LoadString(AFX_IDS_APP_TITLE);

	DOCINFO di;
	memset(&di, 0, sizeof (DOCINFO));
	di.cbSize = sizeof (DOCINFO);
	di.lpszDocName = strTitle;

	BOOL bPrintingOK = dc.StartDoc(&di);

	CPrintInfo Info;
	Info.m_rectDraw.SetRect(0,0, dc.GetDeviceCaps(HORZRES), dc.GetDeviceCaps(VERTRES));

	OnBeginPrinting(&dc, &Info);
	for (UINT page = Info.GetMinPage(); page <= Info.GetMaxPage() && bPrintingOK; page++)
	{
		dc.StartPage();
		Info.m_nCurPage = page;
		OnPrint(&dc, &Info);
		bPrintingOK = (dc.EndPage() > 0);
	}
	OnEndPrinting(&dc, &Info);

	if (bPrintingOK)
		dc.EndDoc();
	else
		dc.AbortDoc();

	dc.Detach();
}

/////////////////////////////////////////////////////////////////////////////
// CintGrid printing overridables - for Doc/View print/print preview framework

void CintGrid::OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo)
{
	ASSERT(pDC && pInfo);
	if (!pDC || !pInfo) return;

	const int nMaxRowCount = GetRowCount() - GetFixedRowCount();
	if (!nMaxRowCount)
		return;

	CDC *pCurrentDC = GetDC();
	if (!pCurrentDC)
		return;

	const CSize PaperPixelsPerInch(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
	const CSize ScreenPixelsPerInch(pCurrentDC->GetDeviceCaps(LOGPIXELSX), pCurrentDC->GetDeviceCaps(LOGPIXELSY));

	// Create the printer font
	const int nFontSize = -9;
	CString strFontName = "Times New Roman";
	m_prnfont.CreateFont(nFontSize, 0,0,0, FW_NORMAL, 0,0,0, DEFAULT_CHARSET,
				OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_DONTCARE, strFontName);
            
	CFont *pOldFont = pDC->SelectObject(&m_prnfont);

	// Get the average character width (in GridCtrl units) and hence the margins
	m_szChar = pDC->GetOutputTextExtent(_T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSATUVWXYZ"),52);
	m_szChar.cx /= 52;
	const int nMargins = (leftMARGIN+rightMARGIN)*m_szChar.cx;

	// Get the page sizes (physical and logical)
	m_szPaper = CSize(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
	m_szLogical.cx = GetVirtualWidth()+nMargins;

	m_szLogical.cy = MulDiv(m_szLogical.cx, m_szPaper.cy, m_szPaper.cx);

	m_pgHeight = m_szLogical.cy - GetFixedRowHeight()
					- (hdrHEIGHT+footerHEIGHT + 2 * prnGAP) * m_szChar.cy;

	// Get the number of pages. Assumes no row is bigger than the page size.
	int nTotalRowHeight = 0;
	int nNumPages = 1;
	for (int row = GetFixedRowCount(); row < GetRowCount(); row++)
	{
		nTotalRowHeight += GetRowHeight(row);
		if (nTotalRowHeight > m_pgHeight)
		{
			nNumPages++;
			nTotalRowHeight = GetRowHeight(row);
		}
	}

	// Set up the print info
	pInfo->SetMaxPage(nNumPages);
	pInfo->m_nCurPage = 1;                        // start printing at page# 1

	ReleaseDC(pCurrentDC);
	pDC->SelectObject(pOldFont);
}

void CintGrid::OnPrint(CDC *pDC, CPrintInfo *pInfo)
{
	if (!pDC || !pInfo) return;

	CFont *pOldFont = pDC->SelectObject(&m_prnfont);

	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(m_szLogical);
	pDC->SetViewportExt(m_szPaper);
	pDC->SetWindowOrg(-leftMARGIN*m_szChar.cx, 0);

	pInfo->m_rectDraw.top    = 0;
	pInfo->m_rectDraw.left   = 0;
	pInfo->m_rectDraw.right  = m_szLogical.cx - (leftMARGIN+rightMARGIN)*m_szChar.cx;
	pInfo->m_rectDraw.bottom = hdrHEIGHT*m_szChar.cy;
	PrintHeader(pDC, pInfo);
	pDC->OffsetWindowOrg(0, -hdrHEIGHT*m_szChar.cy);

	pDC->OffsetWindowOrg(0, -prnGAP*m_szChar.cy);

	pInfo->m_rectDraw.bottom = GetFixedRowHeight(); 
	PrintColumnHeadings(pDC, pInfo);
	pDC->OffsetWindowOrg(0, -GetFixedRowHeight()); 

	int nTotalRowHeight = 0;
	UINT nNumPages = 1;
	int nCurrPrintRow = GetFixedRowCount();

	while (nCurrPrintRow < GetRowCount() && nNumPages < pInfo->m_nCurPage)
	{
		nTotalRowHeight += GetRowHeight(nCurrPrintRow);
		if (nTotalRowHeight > m_pgHeight)
		{
			nNumPages++;
			if (nNumPages == pInfo->m_nCurPage) break;
			nTotalRowHeight = GetRowHeight(nCurrPrintRow);
		}
		nCurrPrintRow++;
	}
	if (nCurrPrintRow >= GetRowCount()) return;

	BOOL bFirstPrintedRow = TRUE;
	CRect rect;
	rect.bottom = -1;
	while (nCurrPrintRow < GetRowCount())
	{
		rect.top = rect.bottom+1;
		rect.bottom = rect.top + GetRowHeight(nCurrPrintRow) - 1;

		if (rect.bottom > m_pgHeight) break;            // Gone past end of page

		rect.right = -1;
		for (int col = 0; col < GetColumnCount(); col++)
		{
			rect.left = rect.right+1;
			rect.right = rect.left + GetColumnWidth(col) - 1;

// 			TRACE("OnPaint::DrawCell1\n");
			DrawCell(pDC, nCurrPrintRow, col, rect);

			if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_HORZ) 
			{
				const int Overlap = (col == 0)? 0:1;
				pDC->MoveTo(rect.left-Overlap, rect.bottom);
				pDC->LineTo(rect.right, rect.bottom);
				if (nCurrPrintRow == 0)
				{
					pDC->MoveTo(rect.left-Overlap, rect.top);
					pDC->LineTo(rect.right, rect.top);
				}
			}
			if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_VERT) 
			{
				const int Overlap = (bFirstPrintedRow)? 0:1;
				pDC->MoveTo(rect.right, rect.top-Overlap);
				pDC->LineTo(rect.right, rect.bottom);    
				if (col == 0)
				{
					pDC->MoveTo(rect.left, rect.top-Overlap);
					pDC->LineTo(rect.left, rect.bottom);    
				}
			}
		}

		nCurrPrintRow++;
		bFirstPrintedRow = FALSE;
	}

	// Footer
	pInfo->m_rectDraw.bottom = footerHEIGHT*m_szChar.cy;
	pDC->SetWindowOrg(-leftMARGIN*m_szChar.cx, -m_szLogical.cy + footerHEIGHT*m_szChar.cy);
	PrintFooter(pDC, pInfo);

	// SetWindowOrg back for next page
	pDC->SetWindowOrg(0,0);

	pDC->SelectObject(pOldFont);
}

void CintGrid::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	m_prnfont.DeleteObject();
}

void CintGrid::PrintColumnHeadings(CDC *pDC, CPrintInfo* /*pInfo*/)
{
	CFont *pOldFont = pDC->SelectObject(&m_prnfont);

	CRect rect; rect.bottom = -1;
	for (int row = 0; row < GetFixedRowCount(); row++)
	{
		rect.top = rect.bottom+1;
		rect.bottom = rect.top + GetRowHeight(row) - 1;

		rect.right = -1;
		for (int col = 0; col < GetColumnCount(); col++)
		{
			rect.left = rect.right+1;
			rect.right = rect.left + GetColumnWidth(col) - 1;

			DrawFixedCell(pDC, row, col, rect);

			if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_HORZ) 
			{
				const int Overlap = (col == 0)? 0:1;
				pDC->MoveTo(rect.left-Overlap, rect.bottom);
				pDC->LineTo(rect.right, rect.bottom);
				if (row == 0)
				{
					pDC->MoveTo(rect.left-Overlap, rect.top);
					pDC->LineTo(rect.right, rect.top);
				}
			}
			if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_VERT) 
			{
				const int Overlap = (row == 0)? 0:1;
				pDC->MoveTo(rect.right, rect.top-Overlap);
				pDC->LineTo(rect.right, rect.bottom);    
				if (col == 0)
				{
					pDC->MoveTo(rect.left, rect.top-Overlap);
					pDC->LineTo(rect.left, rect.bottom);    
				}
			}
		}
	}

	pDC->SelectObject(pOldFont);
}

void CintGrid::PrintHeader(CDC *pDC, CPrintInfo *pInfo)
{
	CRect   rc(pInfo->m_rectDraw);
	CString strHeaderString;
	CFont   BoldFont;
	LOGFONT lf;

	//create bold font for header and footer
	VERIFY(m_prnfont.GetLogFont(&lf));
	lf.lfWeight = FW_BOLD; lf.lfHeight = 100;
	VERIFY(BoldFont.CreatePointFontIndirect(&lf));

	CFont *pNormalFont = pDC->SelectObject(&BoldFont);
	const int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);

	// print App title on top right margin
	strHeaderString.LoadString(AFX_IDS_APP_TITLE);
	pDC->DrawText(strHeaderString, &rc, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);

	// print parent window title in the centre (Gert Rijs)
	CWnd *pParentWnd = GetParent();
	while (pParentWnd)
	{
		pParentWnd->GetWindowText(strHeaderString);
		if (strHeaderString.GetLength())  // can happen if it is a CView, CChildFrm has the title
			break;
		pParentWnd = pParentWnd->GetParent();
	}
	pDC->DrawText(strHeaderString, &rc, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);


	pDC->SetBkMode(nPrevBkMode);
	pDC->SelectObject(pNormalFont);
	BoldFont.DeleteObject();

	pDC->SelectStockObject(BLACK_PEN);
	pDC->MoveTo(rc.left, rc.bottom);
	pDC->LineTo(rc.right, rc.bottom);
}

void CintGrid::PrintFooter(CDC *pDC, CPrintInfo *pInfo)
{
	CRect rc(pInfo->m_rectDraw);
	CFont BoldFont;
	LOGFONT lf;

	//draw line
	pDC->MoveTo(rc.left, rc.top);
	pDC->LineTo(rc.right, rc.top);

	//create bold font for header and footer
	m_prnfont.GetLogFont(&lf);
	lf.lfWeight = FW_BOLD; lf.lfHeight = 100;
	BoldFont.CreatePointFontIndirect(&lf);

	CFont *pNormalFont = pDC->SelectObject(&BoldFont);
	const int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);

	// draw page number
	CString   sTemp ;
	rc.OffsetRect(0, m_szChar.cy/2);
	sTemp.Format(_T("Page %d of %d"), pInfo->m_nCurPage, pInfo->GetMaxPage());
	pDC->DrawText(sTemp,-1,rc, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

	const CTime t = CTime::GetCurrentTime();
	sTemp = t.Format(_T("%c"));
	pDC->DrawText(sTemp,-1,rc, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

	pDC->SetBkMode(nPrevBkMode);
	pDC->SelectObject(pNormalFont);
	BoldFont.DeleteObject();
}

CImageList* CintGrid::CreateDragImage(CPoint *pHotSpot)
{
	//CDC* pDC = GetDC();
	//if (!pDC) return nullptr;

	//CRect rect;
	//const CIdCell	cell = GetFocusCell();
	//if (!GetCellRect(cell.row, cell.col, rect))
	//	return nullptr;

	//// Translate coordinate system
	//rect.BottomRight() = CPoint(rect.Width(), rect.Height());
	//rect.TopLeft()     = CPoint(0,0);
	//*pHotSpot = rect.BottomRight(); 

	//// Create a new imagelist (the caller of this function has responsibility
	//// for deleting this list)
	//CImageList* pList = new CImageList;
	//if (!pList || !pList->Create(rect.Width(), rect.Height(), ILC_MASK, 1,1))
	//{    
	//	if (pList) delete pList;
	//	return nullptr;
	//}

	//// Create mem DC and bitmap
	//CDC MemDC;
	//CBitmap bm;
	//MemDC.CreateCompatibleDC(pDC);
	//bm.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	//CBitmap* pOldBitmap = MemDC.SelectObject(&bm);
	//MemDC.SetWindowOrg(0,0);

	//// Draw cell onto bitmap in memDC
	//DrawCell(&MemDC, cell.row, cell.col, rect, TRUE);

	//// Clean up
	//MemDC.SelectObject(pOldBitmap);
	//ReleaseDC(pDC);

	//// Add the bitmap we just drew to the image list.
	//pList->Add(&bm, m_bkCOL);
	//bm.DeleteObject();

	//MemDC.DeleteDC();

	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////
// CintGrid overrideables
void CintGrid::OnFixedRowClick(CIdCell& cell)
{
	if (!IsValid(cell))
		return;

	if (cell.col < GetFixedColumnCount() && cell.col < colNAME)
	{
		m_mousemode = mouseSELECTALL;
		OnSelecting(cell);
	} 
	else
	{
		if (IsCellAttribute(m_idClick, GVAT_ONLYROW))
		{
			m_mousemode = mouseSELECTROW;
			OnSelecting(cell);
		}
		else
		{ 
			m_mousemode = mouseSELECTCOL;
			OnSelecting(cell);
		}
	}

	if (cell.col >= GetFixedColumnCount() && IsCellAttribute(cell, GVAT_DRAGMOVE))
	{
		m_mousemode = mousePREPAREMOVE;
		m_timerID = SetTimer(WM_LBUTTONDOWN, tmINTERVALx, 0);

		CRect	rect; GetCellRect(cell.row, cell.col, rect);
		CRect	clientRC; GetClientRect(&clientRC);

		m_ptMove.x = rect.left; m_ptMove.y = rect.top;
	}

	if (cell.col >= GetFixedColumnCount() && IsCellAttribute(cell, GVAT_CHECK))
		m_mousemode = mouseCHECKCELL;
}

void CintGrid::OnFixedColumnClick(CIdCell& cell)
{
	if (!IsValid(cell))
		return;

	if (cell.row < GetFixedRowCount()) 
	{
		m_mousemode = mouseSELECTALL;
		OnSelecting(cell);
	}
	else 
	{
		m_mousemode = mouseSELECTROW;
		OnSelecting(cell);
	}
}

CSize CintGrid::GetTextExtent(LPCTSTR str, BOOL bUseSelectedFont)
{
	CDC* pDC = GetDC();
	if (!pDC) return CSize(0,0);

	CFont* pOldFont{}, font;

	if (bUseSelectedFont)
	{
		LOGFONT logfont;
		memcpy(&logfont, &m_logfont, sizeof(LOGFONT));
		logfont.lfWeight = selcWEIGHT;

		font.CreatePointFontIndirect(&logfont);

		pOldFont = pDC->SelectObject(&font);
	}
	else
	{
		pOldFont = pDC->SelectObject(&m_font);
	}

	const CSize size = pDC->GetOutputTextExtent(str);
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	font.DeleteObject();

	return size + CSize(2 * m_margin, 2 * m_margin);
}

CSize CintGrid::GetCellExtent(int nRow, int nCol, CDC* pDC)
{
	if (IsCellAttribute(CIdCell(nRow, nCol), GVAT_HIDDEN))
		return CSize(0, 0);

	LOGFONT *plogfont = GetItemFont(nRow, nCol);

	// use selected font since it's thicker   
	LOGFONT logfont;
	memcpy(&logfont, plogfont, sizeof(LOGFONT));

	if (nRow < m_nFixedRows || nCol < m_nFixedCols)
		logfont.lfWeight = selcWEIGHT;
            
	CFont font;
	font.CreatePointFontIndirect(&logfont);

	CFont* pOldFont = pDC->SelectObject(&font);
	const int	attr = GetItemAttr(nRow, nCol);
	CString string = GetItemDrawText(nRow, nCol);
	if (string.IsEmpty())
		string = GetItemText(nRow, nCol);
	if (IsCellAttribute(CIdCell(nRow, nCol), GVAT_TICKTREND))
		string = string.Mid(string.GetLength() / 2);
	if (string.IsEmpty()) string = _T("9");
	CSize size = pDC->GetOutputTextExtent(string);
	pDC->SelectObject(pOldFont);

//	size += CSize(4*m_margin, 2*m_margin);
	size += CSize(3*m_margin, 2*m_margin);
	if (attr & GVAT_CONTRAST)
		size.cx += 10;

	CSize ImageSize(0,0);
	if (m_imageList)
	{
		const int nImage = GetItemImage(nRow, nCol);
		if (nImage >= 0)
		{
			IMAGEINFO Info;
			if (m_imageList->GetImageInfo(nImage, &Info))
				ImageSize = CSize(Info.rcImage.right-Info.rcImage.left+1, 
					Info.rcImage.bottom-Info.rcImage.top+1);
		}
	}

	font.DeleteObject();

	return CSize(size.cx + ImageSize.cx, max(size.cy, ImageSize.cy));
}


bool CintGrid::OnEditCell(int nRow, int nCol, UINT nChar)
{
	// MODIFY PSH 20070914
	//if (nCol == colNAME)

	const CIdCell cell(nRow, nCol);

	if ((nCol == colNAME) && !IsCellAttribute(cell, GVAT_MARKER))
	// END MODIFY
	{
		const UINT kind = (UINT)GetParent()->SendMessage(WM_MANAGE, MK_GETDATAKIND);
		if (CAST_TREEID(kind)->kind == xISSUE)
			return false;
		// DEL PSH 20070914
		//CIdCell cell(nRow, nCol);
		// END DEL
		if (!IsValid(cell) || !IsCellAttribute(cell, GVAT_EDIT) || !IsCellVisible(nRow, nCol)) 
		{
			MessageToParent(nChar, nCol, GVNM_ONCHAR);
			return false;
		}

		EnsureVisible(nRow, nCol);

		CRect rect;
		if (!GetCellRect(cell.row, cell.col, rect))
			return false;

		MessageToParent(nRow, nCol, GVNM_BEGINEDIT);

		GVITEM gvitem;

		gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_PARAM;
		gvitem.row  = nRow;
		gvitem.col  = nCol;
		m_nClickCol = gvitem.col;
		m_nClickRow = nRow;
		if (!GetItem(&gvitem)) return false;
		
		CreateCode(rect, gvitem.text);
	}
	else
	{
		const CIdCell cell(nRow, nCol);
		if (!IsValid(cell) || !IsCellAttribute(cell, GVAT_EDIT) || !IsCellVisible(nRow, nCol)) 
		{
			MessageToParent(nChar, nCol, GVNM_ONCHAR);
			return false;
		}

		EnsureVisible(nRow, nCol);

		CRect rect;
		if (!GetCellRect(cell.row, cell.col, rect))
			return false;

		MessageToParent(nRow, nCol, GVNM_BEGINEDIT);

		GVITEM gvitem;

		gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_PARAM;
		gvitem.row  = nRow;
		gvitem.col  = nCol;

		if (!GetItem(&gvitem)) return false;

		// MODIFY PSH 20070914
		//DWORD dwStyle = ES_LEFT|ES_NUMBER;
		DWORD dwStyle{};
		if (IsCellAttribute(cell, GVAT_MARKER))
		{
			dwStyle = ES_LEFT;
		}
		else
		{
			dwStyle = ES_LEFT|ES_NUMBER;
		}
		// END MODIFY

		if (gvitem.format & DT_RIGHT)		dwStyle = ES_RIGHT;
		else if (gvitem.format & DT_CENTER)	dwStyle = ES_CENTER;

//		dwStyle |= ES_UPPERCASE;		// �빮�ڸ� �Է�

		if (nCol < m_nFixedCols) rect.DeflateRect(2, 1, 1, 1);

		CreateInPlaceEdit(rect, dwStyle, IDC_INPLACE_EDIT, nRow, nCol, gvitem.text, (int)gvitem.param, nChar);
	}
	
	
	return true;
}

void CintGrid::CreateInPlaceEdit(CRect& rect, DWORD dwStyle, UINT nID,
					int nRow, int nCol, LPCTSTR szText, int limit, int nChar)
{
	LOGFONT* logfont = GetItemFont(nRow, nCol);
	[[gsl::suppress(26409)]]
	[[gsl::suppress(26400)]]
	CInPlaceEdit* pEdit = new CInPlaceEdit(this, rect, dwStyle, nID, nRow, nCol, szText, limit, nChar, logfont);
	// MODIFY PSH 20070914
	//pEdit->SetLimitText(10);
	pEdit->SetLimitText(20);
	// END MODIFY
}

void CintGrid::OnEndEditCell(int nRow, int nCol, CString str)
{
	CString strCurrent = GetItemText(nRow,nCol);
	if (strCurrent != str)
		SetItemText(nRow, nCol, str);
}

bool CintGrid::OnComboCell(int nRow, int nCol)
{
	EnsureVisible(nRow, nCol);

	const CIdCell cell(nRow, nCol);
	if (!IsValid(cell) || !IsCellAttribute(cell, GVAT_COMBO) || !IsCellVisible(nRow, nCol)) 
		return false;

	CRect rect;
	if (!GetCellRect(cell.row, cell.col, rect))
		return false;

// 	TRACE("MessageToParent24 row : %d", nRow);
	MessageToParent(nRow, nCol, GVNM_BEGINCOMBO);

	GVITEM gvitem;

	gvitem.mask = GVMK_TEXT|GVMK_MISC|GVMK_PARAM;
	gvitem.row  = nRow;
	gvitem.col  = nCol;

	if (!GetItem(&gvitem)) return false;

	const DWORD dwStyle = CBS_AUTOHSCROLL|CBS_DROPDOWNLIST;
	CreateInPlaceCombo(rect, dwStyle, IDC_INPLACE_COMBO, nRow, nCol, gvitem.misc, gvitem.param);
	return true;
}

void CintGrid::CreateInPlaceCombo(CRect& rect, DWORD dwStyle, UINT nID,
						int nRow, int nCol, LPCTSTR szText, int active)
{
	LOGFONT* logfont = GetItemFont(nRow, nCol);
	[[gsl::suppress(26409)]]
	new CInPlaceCombo(this, rect, dwStyle, nID, nRow, nCol, szText, logfont, active);
}

void CintGrid::OnEndComboCell(int nRow, int nCol, CString str, LPARAM lparam)
{
	CString	curstr = GetItemText(nRow, nCol);
	if (curstr != str)
	{
		SetItemText(nRow, nCol, str);
		SetItemParam(nRow, nCol, lparam);
	}
}

bool CintGrid::OnDateCell(int nRow, int nCol)
{
	EnsureVisible(nRow, nCol);

	const CIdCell cell(nRow, nCol);
	if (!IsValid(cell) || !IsCellAttribute(cell, GVAT_DATE) || !IsCellVisible(nRow, nCol)) 
		return false;

	CRect rect;
	if (!GetCellRect(cell.row, cell.col, rect))
		return false;

// 	TRACE("MessageToParent25 row : %d", nRow);
	MessageToParent(nRow, nCol, GVNM_BEGINDATE);

	GVITEM gvitem;

	gvitem.mask = GVMK_TEXT|GVMK_MISC;
	gvitem.row  = nRow;
	gvitem.col  = nCol;

	if (!GetItem(&gvitem)) return false;

	CString	init = CString(gvitem.text, gvitem.text.GetLength()); init.Remove('/');
	CreateInPlaceDate(rect, IDC_INPLACE_DATE, init, nRow, nCol, gvitem.misc);
	return true;
}

void CintGrid::CreateInPlaceDate(CRect& rect, UINT nID, LPCTSTR init, int nRow, int nCol, LPCTSTR szText)
{
	[[gsl::suppress(26409)]]
	new CInPlaceDate(this, rect, nID, init, nRow, nCol, szText);
}

void CintGrid::OnEndDateCell(int nRow, int nCol, CString str)
{
	CString	curstr = GetItemText(nRow, nCol);
	if (curstr != str) SetItemText(nRow, nCol, str);
}

bool CintGrid::OnTimeCell(int nRow, int nCol)
{
	EnsureVisible(nRow, nCol);

	const CIdCell cell(nRow, nCol);
	if (!IsValid(cell) || !IsCellAttribute(cell, GVAT_TIME) || !IsCellVisible(nRow, nCol)) 
		return false;

	CRect rect;
	if (!GetCellRect(cell.row, cell.col, rect))
		return false;

// 	TRACE("MessageToParent26 row : %d", nRow);
	MessageToParent(nRow, nCol, GVNM_BEGINTIME);

	GVITEM gvitem;

	gvitem.mask = GVMK_TEXT|GVMK_MISC;
	gvitem.row  = nRow;
	gvitem.col  = nCol;

	if (!GetItem(&gvitem)) return false;

	CString init = CString(gvitem.text, gvitem.text.GetLength()); init.Remove('/');
	CreateInPlaceTime(rect, IDC_INPLACE_TIME, init, nRow, nCol, gvitem.misc);
	return true;
}

void CintGrid::CreateInPlaceTime(CRect& rect, UINT nID, LPCTSTR init, int nRow, int nCol, LPCTSTR szText)
{
[[gsl::suppress(26409)]]
	new CInPlaceTime(this, rect, nID, init, nRow, nCol, szText);
}

void CintGrid::OnEndTimeCell(int nRow, int nCol, CString str)
{
	CString	curstr = GetItemText(nRow, nCol);
	if (curstr != str) SetItemText(nRow, nCol, str);
}

/////////////////////////////////////////////////////////////////////////////
// drag & drop
void CintGrid::OnBeginDrag()
{        
	if (m_dragdrop == GVDD_NONE) return;

	COleDataSource* pSource = DragRowAction();
	if (pSource)
	{    
		m_idDrag.row = m_idClick.row;
		m_idDrag.col = m_idClick.col;

// 		TRACE("MessageToParent27 row : %d", m_idDrag.row);
		MessageToParent(m_idDrag.row, m_idDrag.col, GVNM_BEGINDRAG);
		MessageToParent(m_idDrag.row,m_idDrag.col, GVNM_TRIGGER);//FOR TRIGGER
		m_dragWnd = this;

		CintGrid::m_dropdata.SetData(this, m_idDrag.row, m_idDrag.col);
		m_mousemode = mouseDRAGGING;
// 		TRACE("Begin::m_mousemode : %d\n", m_mousemode);
		const DROPEFFECT dropEffect = pSource->DoDragDrop(DROPEFFECT_COPY);
		
		if ( dropEffect == DROPEFFECT_NONE )
		{
			MessageToParent(m_idDrop.row, m_idDrop.col, GVNM_OUTDRAG);
		}
		else 
			m_bOutPos = false;

		if (pSource) delete pSource;
	}
}

//091013 ����
void CintGrid::dragScrollGrid(CPoint pt)
{
	CPoint origPt;
	if (!GetCursorPos(&origPt))
		return;

	if(m_isLbtnClick == FALSE)
		return;
	
	ScreenToClient(&origPt);
	
	CRect rect; 
	GetClientRect(rect);
	
	const int nFixedRowHeight = GetFixedRowHeight();
	const int nFixedColWidth = GetFixedColumnWidth();
	
	pt = origPt;
	
	static DWORD tick = 0;
	
// 	TRACE("pt.y : %d, rect.top : %d        ", pt.y, rect.top);
	
// 	TRACE("pt.y : %d, rect.bottom : %d\n", pt.y, rect.bottom);
	if (pt.y < rect.bottom && pt.y > (rect.bottom - 40))
	{
		const DWORD dwdownCur = timeGetTime();
		
		if( dwdownCur - tick > 40 )
		{
		//	SendMessage(WM_KEYDOWN, VK_DOWN, 0);
			PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
			
			if (pt.x < rect.left)  pt.x = rect.left;
			if (pt.x > rect.right) pt.x = rect.right;
			pt.y = rect.bottom;
			
			tick = dwdownCur;
		}
		
	}
	else if (pt.y > rect.top && pt.y < (rect.top + 40))
	{
		const DWORD dwupCur = timeGetTime();
		
		if( dwupCur - tick > 40 )
		{
		//	SendMessage(WM_KEYDOWN, VK_UP, 0);
			PostMessage(WM_VSCROLL, SB_LINEUP, 0);
			
			if (pt.x < rect.left)  pt.x = rect.left;
			if (pt.x > rect.right) pt.x = rect.right;
			pt.y = nFixedRowHeight + 1;

			tick = dwupCur;
		}
	}
	
	pt = origPt;
	if (pt.x > rect.right)
	{
		SendMessage(WM_KEYDOWN, VK_RIGHT, 0);
		
		if (pt.y < rect.top)	pt.y = rect.top;
		if (pt.y > rect.bottom) pt.y = rect.bottom;
		pt.x = rect.right;
		
	} 
	else if (pt.x < nFixedColWidth)
	{
		SendMessage(WM_KEYDOWN, VK_LEFT, 0);
		
		if (pt.y < rect.top)	pt.y = rect.top;
		if (pt.y > rect.bottom)	pt.y = rect.bottom;
		pt.x = nFixedColWidth + 1;
		
	}
}


DROPEFFECT CintGrid::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if (m_dragdrop == GVDD_NONE || !pDataObject->IsDataAvailable(CF_TEXT))
		return DROPEFFECT_NONE;

	CIdCell cell = GetCellFromPoint(point, TRUE);

	dragScrollGrid(point);

/*	if (!IsValid(cell))
	{
		OnDragLeave();
		m_idDrop = CIdCell(-1, -1);
		return DROPEFFECT_NONE;
	}

	//if (!IsCellAttribute(cell, GVAT_EDIT))
	if (!IsCellAttribute(cell, GVAT_DRAGDROP))
		return DROPEFFECT_NONE;
BUFDRAG BLOCK*/

	if (m_dragdrop == GVDD_HALF && !IsValidDrop(pDataObject))
		return DROPEFFECT_NONE;

	if (cell != m_idDrop) 
	{
		// Set the previously drop-highlighted cell as no longer drop-highlighted
		if (IsValid(m_idDrop))
		{
			const UINT nState = GetItemState(m_idDrop.row, m_idDrop.col);
			SetItemState(m_idDrop.row, m_idDrop.col, nState & ~GVST_DROPHILITED);
			RedrawCell(m_idDrop);
		}

		m_idDrop = cell;

		// Set the new cell as drop-highlighted
		if (IsValid(m_idDrop))
		{
			const UINT nState = GetItemState(m_idDrop.row, m_idDrop.col);
			SetItemState(m_idDrop.row, m_idDrop.col, nState | GVST_DROPHILITED);
			RedrawCell(m_idDrop);
		}
	}

	return DROPEFFECT_COPY;
}

DROPEFFECT CintGrid::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if (m_dragdrop == GVDD_NONE || !pDataObject->IsDataAvailable(CF_TEXT))
		return DROPEFFECT_NONE;

//	m_idDrag = GetCellFromPoint(point, TRUE);
/*	if (!IsValid(m_idDrag))
		return DROPEFFECT_NONE;

	if (!IsCellAttribute(m_idDrag, GVAT_EDIT))
		return DROPEFFECT_NONE;
BUFDRAG BLOCK*/

	if (m_dragdrop == GVDD_HALF && !IsValidDrop(pDataObject))
		return DROPEFFECT_NONE;

	if (IsValid(m_idDrag))
	{
		const UINT nState = GetItemState(m_idDrag.row, m_idDrag.col);
		SetItemState(m_idDrag.row, m_idDrag.col, nState|GVST_DROPHILITED);
		RedrawCell(m_idDrag);
	}

	m_idDrop = m_idDrag;
	return DROPEFFECT_COPY;
}

void CintGrid::OnDragLeave()
{
	//2013.09.12 KSJ ���� Ŀ����ġ�� ���� �Ǵ�.
	CPoint pt;
	GetCursorPos(&pt);

	ScreenToClient(&pt);

	int	cRow = 0, cCol = 0;
	GetCellFromPoint(pt, cRow, cCol);

	if (cRow != -1 && cCol != -1)
		m_bOutPos = false;
	else
		m_bOutPos = true;
	//2013.09.12 KSJ 


	if (IsValid(m_idDrop))
	{
		const UINT nState = GetItemState(m_idDrop.row, m_idDrop.col);
		SetItemState(m_idDrop.row, m_idDrop.col, nState & ~GVST_DROPHILITED);
		RedrawCell(m_idDrop);
	}
}

BOOL CintGrid::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	const BOOL bResult = FALSE;

	//if (m_dragdrop == GVDD_NONE || !IsCellAttribute(m_idDrop, GVAT_EDIT))
/*	if (m_dragdrop == GVDD_NONE || !IsCellAttribute(m_idDrop, GVAT_DRAGDROP))
		return bResult;
BUFDRAG BLOCK*/

	m_mousemode = mouseNOTHING;
	OnDragLeave();

	const BOOL response = DropRowAction(pDataObject);

	const BOOL bOut = ParseFromOuter(pDataObject);
	
	if ( bOut )
		MessageToParent(m_idDrop.row, m_idDrop.col, GVNM_ENDEND);
	else
	MessageToParent(m_idDrop.row, m_idDrop.col, GVNM_ENDDRAG);

	m_dragWnd = (CWnd *) 0;
	m_idDrag = CIdCell(-1, -1); m_idDrop = CIdCell(-1, -1);
	return response;
}

void CintGrid::OnEditCut()
{
	m_bCopy = false;
	COleDataSource* pSource = CopyTextFromGrid();
	if (pSource)
	{
		pSource->SetClipboard();
		CutSelectedText();
	}
}

void CintGrid::OnEditCopy()
{
	m_bCopy = true;
	COleDataSource* pSource = CopyTextFromGrid();
	if (pSource) pSource->SetClipboard();

}

void CintGrid::OnEditPaste()
{
	CIdCell	idcell = GetFocusCell();

	if (!IsValid(idcell)) idcell = GetTopleftNonFixedCell();  
	if (!IsValid(idcell)) return;

	// Attach a COleDataObject to the clipboard and paste the data to the grid
	COleDataObject obj;
	if (obj.AttachClipboard())
		PasteTextToGrid(idcell, &obj);
	
}

void CintGrid::OnEditSelectAll() 
{
	SelectAllCells();
}

////////////////////////////////////////////////////////////////////////////////////////
// Clipboard functions
void CintGrid::CutSelectedText()
{
	// Clear contents of selected cells.
	for (POSITION pos = m_mapSelCell.GetStartPosition(); pos != nullptr; )
	{
		DWORD key; CIdCell cell;
		m_mapSelCell.GetNextAssoc(pos, key, (CIdCell&)cell);

//		if (!IsCellAttribute(cell, GVAT_EDIT))
//			continue;

		const auto pCell = GetCell(cell.row, cell.col);
		if (pCell) EmptyCell(pCell.get(), cell.row, cell.col);
	}
	Invalidate();
}

COleDataSource* CintGrid::CopyTextFromGrid()
{
	const CRangeCell selection = GetSelectedCellRange();
	if (!IsValid(selection)) return nullptr;

	// Write to shared file (REMEBER: CF_TEXT is ANSI, not UNICODE, so we need to convert)
	CSharedFile sharedFile(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if (m_bCopy)
		MessageToParent(selection.GetMinRow(), selection.GetMinCol(), GVNM_BEGINCOPY);
	else
		MessageToParent(selection.GetMinRow(), selection.GetMinCol(), GVNM_BEGINCUT);

	// Get a tab delimited string to copy to cache
	CString str;// CCell* pCell{};
	for (int row = selection.GetMinRow(); row <= selection.GetMaxRow(); row++) 
	{
		str.Empty();
		for (int col = selection.GetMinCol(); col <= selection.GetMaxCol(); col++)
		{
			auto pCell = GetCell(row, col);
			if (pCell && (pCell->state  & GVST_SELECTED))
			{
				if (pCell->attr & GVAT_NOTOLECOLUMN)
					continue;

				if (pCell->text.IsEmpty())
					str += _T(" ");
				else 
					str += pCell->text;

				if (pCell->attr & GVAT_COMBO)
				{
					str += "\r"; str += pCell->misc;
				}
			}
			if (col != selection.GetMaxCol()) 
				str += _T("\t");
		}
		if (row != selection.GetMaxRow()) 
			str += _T("\n");

		sharedFile.Write(T2A(str.GetBuffer(1)), str.GetLength());
		str.ReleaseBuffer();
		if (m_bCopy)
			MessageToParent(row, selection.GetMaxRow(), GVNM_COPYROW);
		else
			MessageToParent(row, selection.GetMaxRow(), GVNM_CUTROW);

	}

	if (m_bCopy)
		MessageToParent(selection.GetMaxRow(), selection.GetMaxRow(), GVNM_ENDCOPY);
	else
		MessageToParent(selection.GetMaxRow(), selection.GetMaxRow(), GVNM_ENDCUT);

	const char c = '\0'; sharedFile.Write(&c, 1);

	const DWORD dwLen = gsl::narrow_cast<DWORD>(sharedFile.GetLength());
	HGLOBAL hMem = sharedFile.Detach();
#if _MFC_VER	<= 0x0600
	::GlobalUnlock(hMem);
#endif
	if (!hMem) return nullptr;

	hMem = ::GlobalReAlloc(hMem, dwLen, GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
	if (!hMem) return nullptr;

	[[gsl::suppress(26409)]]
	[[gsl::suppress(26400)]]
	// Cache data
	COleDataSource* pSource = new COleDataSource();
	pSource->CacheGlobalData(CF_TEXT, hMem);

	return pSource;
}

BOOL CintGrid::PasteTextToGrid(CIdCell cell, COleDataObject* pDataObject)
{
	if (!IsValid(cell) || !pDataObject->IsDataAvailable(CF_TEXT))
		return FALSE;

	// Get the text from the COleDataObject
	HGLOBAL hmem = pDataObject->GetGlobalData(CF_TEXT);
	CMemFile memFile((BYTE*) ::GlobalLock(hmem), ::GlobalSize(hmem));

	// CF_TEXT is ANSI text, so we need to allocate a char* buffer to hold this.
	
	CString strText;


	memFile.Read(strText.GetBuffer(::GlobalSize(hmem) + 1), ::GlobalSize(hmem));
	::GlobalUnlock(hmem); ::GlobalFree(hmem);
	strText.ReleaseBuffer();

	// Parse text data and set in cells...
	strText.LockBuffer();

	CString strLine = strText;

	int nLine = 0;
	int nIndex{};
	CString string, text;

	//int cnt = 0;
	while (!strLine.IsEmpty())
	{
		nIndex = strLine.Find('\n');
		if (nIndex != -1)
		{
			string = strLine.Left(nIndex);
			strLine = strLine.Mid(nIndex+1);
		}
		else
		{
			string = strLine;
			strLine = _T("");
		}
		nLine++;
	}
	MessageToParent(cell.row, nLine, GVNM_COUNTPASTE);

	
	CIdCell tmpCell;
	CIdCell TargetCell;
	CCell* pCell{};
	strLine = strText;
	nLine = 0;
	MessageToParent(cell.row, cell.col, GVNM_BEGINPASTE);

	while (!strLine.IsEmpty())
	{
		int nColumn = 0;

		nIndex = strLine.Find('\n');
		if (nIndex != -1)
		{
			string = strLine.Left(nIndex);
			strLine = strLine.Mid(nIndex+1);
		}
		else
		{
			string = strLine;
			strLine = _T("");
		}

		while (!string.IsEmpty())
		{
			nIndex = string.Find('\t');
			if (nIndex != -1)
			{
				text = string.Left(nIndex);
				string = string.Mid(nIndex+1);
			}
			else
			{
				text = string;
				string = _T("");
			}

			const auto pCell = GetCell(cell.row + nLine, cell.col + nColumn);
			if (pCell && pCell->attr & GVAT_NOTOLECOLUMN)
			{
				nColumn++;	continue;
			}
			TargetCell.SetItem(cell.row + nLine, cell.col + nColumn);
			if (IsValid(TargetCell))
			{
				text = validAttribute(TargetCell.row, TargetCell.col, text);
				SetItemText(TargetCell.row, TargetCell.col, text);
				SetItemState(TargetCell.row, TargetCell.col,
					GetItemState(TargetCell.row, TargetCell.col) & ~GVST_SELECTED);
			}
			nColumn++;
		}
		
		MessageToParent(TargetCell.row, TargetCell.col, GVNM_PASTEROW);
		tmpCell = TargetCell;

		nLine++;
		
	}

	if (IsValid(tmpCell))
		MessageToParent(tmpCell.row, tmpCell.col, GVNM_ENDPASTE);
	else
		MessageToParent(-1, -1, GVNM_ENDPASTE);
	
	strText.UnlockBuffer();
	Invalidate();

	return TRUE;
}

COleDataSource* CintGrid::DragRowAction()
{
// updateX_20060113
	const CRangeCell selection = GetSelectedCellRange(true);
	if (!IsValid(selection)) return nullptr;

	// Write to shared file (REMEBER: CF_TEXT is ANSI, not UNICODE, so we need to convert)
	CSharedFile sharedFile(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	// Get a tab delimited string to copy to cache
	CCell* pCell{}; CString string = (char)0x01;
	for (int row = selection.GetMinRow(); row <= selection.GetMaxRow(); row++) 
	{
		for (int col = 0; col < GetColumnCount(); col++)
		{
			const auto pCell = GetCell(row, col);
			// updateX_20060114
			if (pCell && IsCellAttribute(CIdCell(row, col), GVAT_DRAGDROP))
			{
				auto pTempCell = GetCell(row, col - 1);
				string += pTempCell->symbol; string += "\t";
				string += pTempCell->text; string += "\t";
			}
		}
		if (row != selection.GetMaxRow()) 
			string += _T("\r");

		sharedFile.Write(T2A(string.GetBuffer(0)), string.GetLength());
		string.ReleaseBuffer();
	}

	const char c = '\0'; 
	sharedFile.Write(&c, 1);

	const DWORD dwLen = gsl::narrow_cast<DWORD>(sharedFile.GetLength());
	HGLOBAL hMem = sharedFile.Detach();
#if _MFC_VER	<= 0x0600
	::GlobalUnlock(hMem);
#endif
	if (!hMem) return nullptr;

	hMem = ::GlobalReAlloc(hMem, dwLen, GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
	if (!hMem) return nullptr;

	// Cache data

	[[gsl::suppress(26409)]]
	[[gsl::suppress(26400)]]
	COleDataSource* pSource = new COleDataSource();
	pSource->CacheGlobalData(CF_TEXT, hMem);

	return pSource;
}

BOOL CintGrid::IsValidDrop(COleDataObject*pDataObject)
{
	if (!pDataObject->IsDataAvailable(CF_TEXT))
		return FALSE;

	HGLOBAL hmem = pDataObject->GetGlobalData(CF_TEXT);
	CMemFile memFile((BYTE*) ::GlobalLock(hmem), ::GlobalSize(hmem));

	CString strText;
	memFile.Read(strText.GetBuffer(::GlobalSize(hmem)), ::GlobalSize(hmem));
	::GlobalUnlock(hmem); ::GlobalFree(hmem);
	strText.ReleaseBuffer();

	if (strText.GetAt(0) != (char)0x01)
		return FALSE;
	return TRUE;
}

BOOL CintGrid::DropRowAction(COleDataObject* pDataObject)
{
/*	if (!IsValid(m_idDrop) || !IsCellAttribute(m_idDrop, GVAT_EDIT) || !pDataObject->IsDataAvailable(CF_TEXT))
		return FALSE;
BUFDRAG BLOCK*/

	m_idDrag.row = -1;
	m_idSelect = m_idDrop; SelectRows(m_idDrop);
	return TRUE;
}

BOOL CintGrid::MoveColAction(int sCol, int dCol)
{
	if (sCol < 0 || sCol >= m_nCols)
		return FALSE;
	if (dCol < 0 || dCol >= m_nCols)
		return FALSE;

	if (sCol == dCol) return TRUE;
	if (sCol < dCol) dCol -= 1;

	for_each(m_aryGridRows.begin(), m_aryGridRows.end(), [sCol, dCol](const auto& vitem) {
		auto cell = vitem->at(sCol);
		vitem->erase(vitem->begin() + sCol);
		vitem->insert(vitem->begin() + dCol, cell);
		});

	const int nWidth = m_aryColWidth[sCol];
	m_aryColWidth.erase(m_aryColWidth.begin() + sCol);
	m_aryColWidth.insert(m_aryColWidth.begin() + dCol, nWidth);

	return TRUE;
}

COLORREF CintGrid::DisplayAttribute(int attr, CString& data)
{
	if (data.IsEmpty())
	{
		data = _T("");	return (COLORREF) 0;
	}
	
	const int checkattr = GVAT_ZEROSUP|GVAT_ZEROSUPx|GVAT_COMMA|GVAT_FLOAT1|GVAT_FLOAT2|GVAT_FLOAT4|GVAT_CONDITION|GVAT_CONDITIONx;

	if (!(attr & checkattr)) return (COLORREF) 0;

	int idx{}; char sign = ' ';
	CString	string = data, stringx;
	string.TrimLeft(); string.TrimRight();

	if ((idx = string.FindOneOf("+-")) == 0)
	{
		sign = string.GetAt(idx);
		string.Delete(idx);
	}

	if (attr & GVAT_ZEROSUP)
	{
		while (string.GetLength())
		{
			if (string[0] == _T('0'))
			{
				string.Delete(0);
				continue;
			}
			break;
		}

		if (string.IsEmpty())
			string += _T("0");
	}

	if (attr & GVAT_ZEROSUPx)
	{
		if (atof(string) == 0) string.Empty();
	}

	stringx = string;
	const int stringL = stringx.GetLength();

	switch (attr & (GVAT_COMMA|GVAT_FLOAT1|GVAT_FLOAT2|GVAT_FLOAT4))
	{
	case GVAT_COMMA:
		stringx = CommaModify(string);
		break;
	case GVAT_FLOAT1:
		if (stringL >= 2)
			stringx.Format("%s.%s", string.Left(stringL -1), string.Right(1));
		else
			stringx.Format("0.%s", string);
		break;
	case GVAT_FLOAT2:
		if (stringL >= 3)
			stringx.Format("%s.%s", string.Left(stringL -2), string.Right(2));
		else
			stringx.Format("0.%s", string);
		break;
	case GVAT_FLOAT4:
		if (stringL >= 5)
			stringx.Format("%s.%s", string.Left(stringL -4), string.Right(4));
		else
			stringx.Format("0.%s", string);
		break;
	}

	data = stringx;

	if (attr & (GVAT_CONDITION|GVAT_CONDITIONx))
	{
		if ((attr & GVAT_CONDITION) && sign != ' ')
			data.Insert(0, sign);

		switch (sign)
		{
		case '+': return m_upCOL;
		case '-': return m_dnCOL;
		default:  return m_eqCOL;
		}
	}
	else if (sign != ' ')
	{
		data.Insert(0, sign);
	}

	return (COLORREF) 0;
}


CString CintGrid::CommaModify(CString string)
{
	CString stripData = string;
	stripData.TrimLeft(); stripData.TrimRight();

	if (stripData.GetLength() <= 3)
		return stripData;

	if (stripData.Find('.') != -1)
		return stripData;

	char dChar = 0x00;
	switch (stripData[0])
	{
	case '+':
		dChar = '+'; stripData.Delete(0);
		break;
	case '-':
		dChar = '-'; stripData.Delete(0);
		break;
	}

	stripData.MakeReverse();
	string = _T("");
	for (int ii = 0; ii < stripData.GetLength(); ii++)
	{
		if ((ii != 0) && ((ii % 3) == 0))
			string += ',';

		string += stripData[ii];
	}

	string.MakeReverse();
	if (dChar != 0x00) string.Insert(0, dChar);
	return string;
}

CString CintGrid::validAttribute(int row, int col, CString text)
{
	if (!IsValid(CIdCell(row, col)) || text.IsEmpty())
		return _T("");

	CString	string = text;
	string.TrimLeft(); string.TrimRight();

	const UINT	attr = GetItemAttr(row, col);

	if (attr & GVAT_COMMA) string.Remove(',');
	if (attr & (GVAT_FLOAT1|GVAT_FLOAT2|GVAT_FLOAT4)) string.Remove('.');

	return string;
}

bool CintGrid::SetImageListIndex(int nRow, int nCol, int index)
{
	GVITEM gvitem;

	gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_STATE|GVMK_ATTR|GVMK_IMAGE|GVMK_BKCOLOR|GVMK_FGCOLOR;
	gvitem.row = nRow;
	gvitem.col = nCol;

	if (!GetItem(&gvitem) || gvitem.image == index)
		return false;

	gvitem.image = index;
	SetItem(&gvitem);

	InvalidateCellRect(CIdCell(nRow, nCol));
	return true;
}

void CintGrid::SetSelectRow(int row)
{
	const CIdCell cell(row, 0);

	ResetSelectedRange();
//	EnsureVisible(cell);	// (20070806) ���� : �ڵ� Scroll(Ŭ���� ���� ù ��° �� Position �̵�) �۵� ����

	m_idSelect = CIdCell(row, 0);
	SelectRows(cell);
	//SetFocusCell(max(m_idSelect.row, m_nFixedRows), max(m_idSelect.col, colSIG + 1));
	SetFocusCell(max(m_idSelect.row, m_nFixedRows), 16);
}

void CintGrid::SetFocusCellEdit(int row, int col, bool edit)
{
	const CIdCell cell(row, col);

	ResetSelectedRange();
	EnsureVisible(cell); 

	m_idSelect = CIdCell(row, 0);
	SelectRows(cell);

	SetFocusCell(cell);

	if (edit) OnEditCell(row, col, VK_END);
}

void CintGrid::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	HideCode();

/*	if(!m_originRect.IsRectEmpty())
	{
		m_xRate = (double)((double)cx / (double)m_originRect.Width());
		m_yRate = (double)((double)cy / (double)m_originRect.Height());
		
	
	}
	else
	{
		GetClientRect(m_originRect);
	}
*/
	Adjust();	
}



void CintGrid::SetConditionColor(COLORREF upCOL, COLORREF dnCOL, COLORREF eqCOL)
{
	m_upCOL = upCOL;
	m_dnCOL = dnCOL;
	m_eqCOL = eqCOL;
}

void CintGrid::SetTitleTips(BOOL bEnable)	
{ 
	m_enabletip = bEnable; 
// 	if (m_enabletip)
// 	{
// 		if (m_pTooltip == nullptr)
// 		{
// 			m_pTooltip = new CTipDlg(m_pViewWnd, this);
// 			m_pTooltip->Create(CTipDlg::IDD, nullptr);		
// 		}
// 
// 		if (m_pToolNews == nullptr)
// 		{
// 			m_pToolNews = new CTipDlg(m_pViewWnd, this, true);			
// 			m_pToolNews->Create(CTipDlg::IDD, nullptr);	
// 		}
// 	}
}

void CintGrid::drawContrast(CDC *pDC, CRect cellRc, CString sText, COLORREF clr)
{
	CRect	pRc = cellRc;
	float	hh = (float)cellRc.Height();
	CSize	size;
	CPoint	pts[7];
	CPen	pen, * pOldPen{};
	CBrush	brush, * pOldBrush{};
	
	pen.CreatePen(PS_SOLID, 1, clr);
	pOldPen = (CPen *) pDC->SelectObject(&pen);

	brush.CreateSolidBrush(clr); // bkcolor
	pOldBrush = (CBrush *) pDC->SelectObject(&brush);

	pRc.top = (cellRc.top + cellRc.Height()/2) - (int)(hh/2);
	pRc.bottom = (int)(pRc.top + hh);
	
	size = pDC->GetOutputTextExtent(ARR_UP);
	if (size.cx > 10)
		pRc.left = (int)(pRc.right - (size.cx * 0.8));
	else
		pRc.left = pRc.right - size.cx;
	
	hh = (float)(int)(pRc.Height()+1)/2;
	hh = (float)(int)((hh+1)/2) * 2 - 1;
	int ww = pRc.Width();
	pRc.right = pRc.left + 8;
	ww = pRc.Width() / 2 - 1;
	
	if (pRc.left >= cellRc.left && pRc.right <= cellRc.right)
	{
		pDC->SetPolyFillMode(ALTERNATE);
		if (!sText.Compare(ARR_UP))
		{
			pts[1].y = pRc.top + (int)hh;
			pts[2].y = pRc.top + (int)hh;
			pts[5].y = pRc.top + (int)hh;
			pts[6].y = pRc.top + (int)hh;
			pts[0].y = pRc.top;
			pts[3].y = pRc.bottom;
			pts[4].y = pRc.bottom;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.left + 1;
			pts[6].x = pRc.right - 1;
			pts[2].x = pRc.left + ww;
			pts[3].x = pRc.left + ww;
			pts[4].x = pRc.right - ww;
			pts[5].x = pRc.right - ww;
			pDC->Polygon(pts, 7);
		}
		else if (!sText.Compare(ARR_DN))	
		{
			pts[1].y = pRc.top + (int)hh;
			pts[2].y = pRc.top + (int)hh;
			pts[5].y = pRc.top + (int)hh;
			pts[6].y = pRc.top + (int)hh;
			pts[0].y = pRc.bottom;
			pts[3].y = pRc.top;
			pts[4].y = pRc.top;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.right - 1;
			pts[6].x = pRc.left + 1;
			pts[2].x = pRc.right - ww;
			pts[3].x = pRc.right - ww;
			pts[4].x = pRc.left + ww;
			pts[5].x = pRc.left + ww;
			pDC->Polygon(pts, 7);
		}
		else if (!sText.Compare(TRI_UP))	
		{
			pRc.top += 2;
			pRc.bottom -= 2;
			if (!(pRc.Height() % 2))
				pRc.top++;
			pts[0].y = pRc.top;
			pts[1].y = pRc.bottom;
			pts[2].y = pRc.bottom;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.left;
			pts[2].x = pRc.right;
			pDC->Polygon(pts, 3);
		}
		else if (!sText.Compare(TRI_DN))	
		{
			pRc.top += 2;
			pRc.bottom -= 2;
			if (!(pRc.Height() % 2))
				pRc.top++;
			pts[0].y = pRc.bottom;
			pts[1].y = pRc.top;
			pts[2].y = pRc.top;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.right;
			pts[2].x = pRc.left;
			pDC->Polygon(pts, 3);
		}
	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
	pen.DeleteObject();
}

#include "math.h"
double CintGrid::TOfabs(CString string)
{
	double dval = 0;
	if (string.IsEmpty()) return dval;

	dval = fabs(atof(string));
	return dval;
}


CPen* CintGrid::GetAxPen(COLORREF clr, int width, int style)
{
	struct	_penR penR;
	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pViewWnd->SendMessage(WM_USER, getPEN, (long)&penR);
}

CPen* CintGrid::GetAxPen(struct _penR* pen)
{
	return GetAxPen(pen->clr, pen->width, pen->style);	
}

CFont* CintGrid::GetAxFont(CString fName, int point, bool bBold, bool bItalic)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = bItalic; 
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;
	return (CFont*)m_pViewWnd->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CFont* CintGrid::GetAxFont(struct _fontR* font)
{
	return GetAxFont(font->name, font->point, (font->bold == FW_BOLD) ? true : false, font->italic);
}

CBrush* CintGrid::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_pViewWnd->SendMessage(WM_USER, getBRUSH, (long)clr);
}

// ��������ǥ��
void CintGrid::drawIndicator1(CDC* dc, CRect rect, COLORREF clr)
{
	CPoint	pts[3];
	int	gap{}, height{};

	struct _penR	penR;
	penR.clr = clr;
	penR.width = 1;
	penR.style = PS_SOLID;
	CPen*	pen = GetAxPen(&penR);
	CBrush*	brush = GetAxBrush(clr);
	CPen*   oldPen   = dc->SelectObject(pen);
	CBrush* oldBrush = dc->SelectObject(brush);

	dc->SetPolyFillMode(ALTERNATE);

	rect.DeflateRect(1, 1, 2, 2);
	gap    = rect.Height() / 6;
	height = (rect.Height() - gap) / 2;

	rect.top  = rect.bottom - height;
	rect.left = rect.right - height;

	pts[0].x = rect.left;
	pts[0].y = rect.bottom;
	pts[1].x = rect.right;
	pts[1].y = rect.top;
	pts[2].x = rect.right;
	pts[2].y = rect.bottom;
	dc->Polygon(pts, 3);

	dc->SelectObject(oldBrush);
	dc->SelectObject(oldPen);
}

// �޸�ǥ�ÿ���
void CintGrid::drawIndicator2(CDC* dc, CRect rect, COLORREF clr, CRgn& rgn)
{
	const int pWidth = 1;

	struct _penR	penR;
	penR.clr = clr;
	penR.width = pWidth;
	penR.style = PS_SOLID;
	CPen*	pen = GetAxPen(&penR);
	CBrush*	brush = GetAxBrush(clr);
	const CPen*   oldPen   = dc->SelectObject(pen);
	const CBrush* oldBrush = dc->SelectObject(brush);

	dc->FillSolidRect(rect.right - 10 , rect.top, 10, rect.Height(), clr);
}

void CintGrid::memoDraw(CDC* pDC)
{
	const CIdCell tlCell = GetTopleftNonFixedCell();
	const int minVisibleRow = tlCell.row;
	const int minVisibleCol = nameCOL;
	
	CRect visRect;
	const CRangeCell visCellRange = GetVisibleNonFixedCellRange(visRect);
	const int maxVisibleRow = visCellRange.GetMaxRow();
	const int maxVisibleCol = visCellRange.GetMaxCol();
	const int curCol = visCellRange.GetMinCol();
	// // // 
	m_memoRGN.DeleteObject();
	m_memoRGN.CreateRectRgn(0, 0, 0, 0);
	
	CRect	rect; 
	rect.SetRectEmpty();
	
	rect.right = GetFixedColumnWidth() -1 - GetColumnWidth(colNAME) - GetColumnWidth(colCURR);
	
	//17 : ���簡
	if( curCol >= 17 )
		return;
	
	for (int col = minVisibleCol; col <= maxVisibleCol ; col++)
	{
		if (IsCellAttribute(CIdCell(0, col), GVAT_HIDDEN))
			continue;
		
		rect.left = rect.right +1;
		rect.right = rect.left + GetColumnWidth(col) -1;
		
		if (col == nameCOL)
		{
			rect.left = rect.right - indicatorW;
			break;
		}
	}
	
	const int nSize = m_memoAry.GetSize();
	
	rect.bottom = GetFixedRowHeight() -1;
	for (int row = minVisibleRow; row <= maxVisibleRow; row++)
	{
		rect.top = rect.bottom +1;
		rect.bottom = rect.top + GetRowHeight(row) -1;
		
		// MODIFY PSH 20070911
		//if (m_memoAry.GetAt(row - 1))
		if ( (0 < row) && (row < nSize) && (m_memoAry.GetAt(row - 1)) )
			// END MODIFY
		{
			CRgn	rgn;
			
			//0 : ����/��Ʈ COL, 15 : ����� 17 : ���簡
			if( curCol == 0 )
				rect.right = GetColumnWidth(colSIG) + GetColumnWidth(colCODE) + GetColumnWidth(colNAME);
			else
				rect.right = GetColumnWidth(colCODE) + GetColumnWidth(colNAME);
			
			rect.left  = rect.right - indicatorW;
			drawIndicator2(pDC, rect, memoCOLOR, rgn);	// �޸𿩺�
			m_memoRGN.CombineRgn(&m_memoRGN, &rgn, RGN_OR);
		}
	}
}
void CintGrid::memoRefresh()
{
	if (m_memoRGN.m_hObject)
		InvalidateRgn(&m_memoRGN);
	else
		Invalidate();
}

bool CintGrid::memoCheck()
{
	m_memoAry.RemoveAll();

	CString	filePath; 
	filePath.Format("%s/%s/%s/%s", m_root, "USER", m_user, "memo.mmo");

	CFileFind finder;
	if (!finder.FindFile(filePath))
	{
		for (int ii = 0; ii < MAX_LINE ; ii++)
			m_memoAry.Add(false);
		return false;
	}

	TRY
	{
		UINT	nBytesRead = 0;
		CFile	file(filePath, CFile::modeRead);
		CString	code, data;
		char	fcode[12]{}, flen[4]{};
		int	readL = 0;

		for (int ii = GetFixedRowCount(); ii < MAX_LINE + 1 ; ii++)
		{
			m_memoAry.Add(false);

			code = GetItemText(ii, codeCOL); 
			code.TrimLeft(); 
			code.TrimRight();

			if (code.IsEmpty()) continue;

			file.SeekToBegin();

			while (true)
			{
				nBytesRead = file.Read(&fcode, sizeof(fcode));
				if (nBytesRead < sizeof(fcode))
					break;
				nBytesRead = file.Read(&flen, sizeof(flen));
				if (nBytesRead < sizeof(flen))
					break;

				readL = atoi(CString(flen, sizeof(flen)));
				nBytesRead = file.Read(data.GetBufferSetLength(readL), readL);
				data.ReleaseBuffer();

				if (!code.Compare(fcode) && readL > 0)
				{
					m_memoAry.SetAt(ii - 1, true);
					break;
				}
			}

		}

		file.Close();
	}
	CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH

	return true;
}

void CintGrid::memoReDraw()
{
}

bool CintGrid::memoCheck(int xrow)
{
	m_memoAry.SetAt(xrow - 1, false);

	CString	filePath; 
	filePath.Format("%s/%s/%s/%s", m_root, "USER", m_user, "memo.mmo");

	CFileFind finder;
	if (!finder.FindFile(filePath)) return false;

	TRY
	{ 
		UINT	nBytesRead = 0;
		CFile	file(filePath, CFile::modeRead);
		CString	code, data;
		char	fcode[12]{}, flen[4]{};
		
		code = GetItemText(xrow, codeCOL);

		do
		{
			nBytesRead = file.Read(&fcode, sizeof(fcode));
			if (nBytesRead == sizeof(fcode))
			{
				nBytesRead = file.Read(&flen, sizeof(flen));
				if (nBytesRead == sizeof(flen))
				{
					const int readL = atoi(CString(flen, sizeof(flen)));
					nBytesRead = file.Read(data.GetBufferSetLength(readL), readL);
					data.ReleaseBuffer();

					if (!code.Compare(fcode) && readL > 0)
					{
						m_memoAry.SetAt(xrow - 1, true);
						return true;
					}
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

	return false;
}

void CintGrid::memoDelete(int xrow)
{
	if (xrow < m_memoAry.GetSize())
	{
		m_memoAry.RemoveAt(xrow - 1);
		m_memoAry.Add(false);
	}
}

void CintGrid::memoClear(int xrow)
{
	if (xrow < 0)
	{
		const int ncnt = m_memoAry.GetSize();
		for ( int ii = 0 ; ii < ncnt ; ii++ )
		{
			m_memoAry.SetAt(ii, false);
		}
	}
	else
		m_memoAry.SetAt(xrow - 1, false);
}

void CintGrid::memoDropAction(int drag, int drop)
{
	if (drag > 0 && drop > 0)		// ������ ȭ�鿡�� drop
	{
		const bool fBool = m_memoAry.GetAt(drag - 1);
		m_memoAry.RemoveAt(drag - 1);

		if (drag < drop)
			drop -= 1;

		m_memoAry.InsertAt(drop - 1, fBool);
	}
	else if (drag < 0 && drop > 0)	// �ٸ� ȭ�鿡�� drop
	{
		m_memoAry.RemoveAt(m_memoAry.GetUpperBound());

		const bool fBool = CintGrid::m_dropdata.GetGrid()->memoCheck(CintGrid::m_dropdata.GetRow());
		m_memoAry.InsertAt(drop - 1, fBool);
	}
}

void CintGrid::memoWindow(CString code, CString name, CRect rect)
{
	m_memoWnd->ShowMemo(code, name, rect);
}

void CintGrid::delectMemo(int row)
{
	m_updateRow = row;
	CString	code = GetItemText(row, codeCOL);
	m_memoWnd->procedureDeleteMemo(code);
}


void CintGrid::memoWindowEdit(int row)
{
	CRect	itemRC; 
	itemRC.SetRectEmpty();
	GetItemRect(row, nameCOL, &itemRC); 
	ClientToScreen(itemRC);

	CString	code = GetItemText(row, codeCOL);
	CString	name = GetItemText(row, nameCOL);

	const int cx = 225, cy = 150;
	CRect	rect; 
	rect.SetRect(itemRC.right, itemRC.top,  itemRC.right + cx, itemRC.top + cy);
 	m_updateRow = row; 

	memoWindow(code, name, rect);
}


void CintGrid::HideCode()
{
	if (m_history)
		m_history->ShowWindow(SW_HIDE);
}

int CintGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_root.Format("%s", (char*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETROOT));	
	m_user.Format("%s", (char*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETUSER));
	m_pViewWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
	m_pFont = GetAxFont();
	
	m_pFontS = GetAxFont("����ü", 5);
	if (m_memoWnd == nullptr)
	{
		const int cx = 225, cy = 150;

		CRect	rect; rect.SetRect(0, 0, cx, cy);
			
		m_memoWnd = std::make_unique<CMemo>("", "", m_root, m_user);
		m_memoWnd->createMemo(this, m_pViewWnd, &rect);
	}

	CString	path;
	path.Format("%s\\%s\\newschart.bmp", m_root, IMAGEDIR);
	
	// ADD PSH 20070912
	m_pFontMR = GetAxFont("����ü", 8);
	// END ADD

	return 0;
}

void CintGrid::OnDestroy() 
{
	CWnd::OnDestroy();

	if (m_memoWnd)
	{
		m_memoWnd->DestroyWindow();
	}	

	if (m_pTooltip)
	{
		m_pTooltip->DestroyWindow();
	}

	if (m_pToolChart)
	{
		m_pToolChart->DestroyWindow();
	}

	if (m_pToolNews)
	{
		m_pToolNews->DestroyWindow();
	}
	
	DestroyCode();

	// ADD PSH 20070918
	if (nullptr != m_pBmpMarker)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpMarker->Detach();

		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}

	if (nullptr != m_pBmpInfo1)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo1->Detach();

		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}

	if (nullptr != m_pBmpInfo2)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo2->Detach();

		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}

	if (nullptr != m_pBmpInfo3)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo3->Detach();

		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}

	if (nullptr != m_pBmpInfo4)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo4->Detach();

		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}

	if (nullptr != m_pBmpInfo5)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo5->Detach();

		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}

	if (nullptr != m_pBmpInfo6)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo6->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}

	if (nullptr != m_pBmpInfo7)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo7->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}
	
	if (nullptr != m_pBmpInfo8)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo8->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}
	
	if (nullptr != m_pBmpInfo9)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo9->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}
	
	if (nullptr != m_pBmpInfo10)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo10->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}

	if (nullptr != m_pBmpInfo11)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo11->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}

	//2012.06.20 KSJ ����, ��� �߰�
	if (nullptr != m_pBmpInfo12)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo12->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}
	
	if (nullptr != m_pBmpInfo13)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo13->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}

	if (nullptr != m_pBmpInfo14)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo14->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}
	if (nullptr != m_pBmpInfo15)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo15->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}
	
	if (nullptr != m_pBmpInfo16)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo16->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}
	
	if (nullptr != m_pBmpInfo17)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo17->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}
	
	if (nullptr != m_pBmpInfo18)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo18->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}
	
	if (nullptr != m_pBmpInfo19)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo19->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}
	
	if (nullptr != m_pBmpInfo20)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo20->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}
	
	if (nullptr != m_pBmpInfo21)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo21->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}

	if (nullptr != m_pBmpInfo22)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo22->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}

	if (nullptr != m_pBmpInfo23)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpInfo23->Detach();
		
		if (nullptr != hBitmap)
			DeleteObject(hBitmap);
	}

	//KSJ

	if(nullptr != m_pBmpExpect)
	{
		HBITMAP hBitmap = (HBITMAP)m_pBmpExpect->Detach();
		
		if(nullptr != hBitmap)
			DeleteObject(hBitmap);
	}
	// END ADD
}

// ADD PSH 20070914
BOOL CintGrid::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NM_USER* nmuser = (NM_USER*) lParam;
	
	if (wParam != IDC_INPLACE_EDIT)
		return CWnd::OnNotify(wParam, lParam, pResult);	

	switch (nmuser->hdr.code)
	{
	case GVNM_ENDEDIT:
		{
			SetItemText(nmuser->item.row, nmuser->item.col, nmuser->item.text);
			
			if (IsCellAttribute(CIdCell(nmuser->item.row, colNAME), GVAT_MARKER))
			{
				MessageToParent(nmuser->item.row, nmuser->item.col, GVNM_CHANGEMARKER);
			}
		}
		break;
	}

	return CWnd::OnNotify(wParam, lParam, pResult);
}
// END ADD

void CintGrid::CreateCode(CRect rect, CString szText)
{
	szText.TrimLeft(), szText.TrimRight();

	m_pFont = GetAxFont("����ü",m_point);

	if (m_history == nullptr)
	{
		CString	tabPath; tabPath.Format("%s/%s", m_root, TABDIR);
		m_history = std::make_unique<CIHCodeCtrl>(this, tabPath);
		const int	gap2 = 2;
		const int	comboW = 120;
		const int	comboH = 20;
		m_history->Create(this, rect, IDC_HISTORY);
	}
	//m_history->SetFont(&font, false);
	m_history->SetFont(m_pFont, false);
	m_history->MoveWindow(rect);
	m_history->SetHistory(Variant(historyCC, HCOD));
	m_history->SetEditData(szText);
	m_history->ShowWindow(SW_SHOW);
	m_history->SetFocus();	
}

void CintGrid::DestroyCode()
{
	if (m_history != nullptr)
		m_history->DestroyWindow();
}

CString CintGrid::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	char* dta = (char*)m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue.Format("%s", dta);
	
	return retvalue;
}

void CintGrid::OnHistorySelEndOk()
{
	CString	code;
	code = m_history->GetEditData(); code.TrimLeft(); code.TrimRight();
	if (code.GetLength() <= 0) return;
	OnEndEditCell(m_nClickRow, m_nClickCol - 1, code);
	HideCode();
	MessageToParent(m_nClickRow, m_nClickCol, GVNM_CHANGECODE);
}

LONG CintGrid::OnManage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case MK_MEMOUPDATE:
		if (m_updateRow != -1)
		{
			memoCheck();
			memoCheck(m_updateRow);
			memoRefresh();
		}
		m_updateRow = -1;
		break;
	default:
		break;
	}

	return 0;
}

CString CintGrid::GetCodeType(CString sname)
{
	CString	ret = _T("");
	if (sname.IsEmpty())
		return _T("");
	switch (sname.GetAt(0))
	{
	case '@':
		ret.Format(" : %s", "�Ҽ��� ����");
		break;
	case '/':
		ret.Format(" : %s", "�׸����(����)");
		break;
	case '$':
		ret.Format(" : %s", "��������");
		break;
	case '%':
		ret.Format(" : %s", "����(�̻�޵�)");
		break;
	case '&':
		ret.Format(" : %s", "��������");
		break;
	case 'X':
		ret.Format(" : %s", "�ŷ���������");
		break;
	case '!':
		ret.Format(" : %s", "�Ǹ���");
		break;
	case '#':
		ret.Format(" : %s", "����");
		break;
	}
	return ret;
}

void CintGrid::DrawBong(CDC* pDC, CRect rect, int nRow, int nCol)
{	
	CString	str = GetItemText(nRow, colEXPECT);
	
	if (atoi(str))
		return;

	CRect	rcorg = rect;
	rcorg.DeflateRect(0, 1);
	rect.DeflateRect(m_margin, 3);

	const CPoint	pt = rect.CenterPoint();
	CPen	pen(PS_SOLID, 1, RGB(128, 128, 128)), penUP(PS_SOLID, 1, RGB(255, 0, 0)), penDOWN(PS_SOLID, 1, RGB(0, 0, 255));
	CBrush	brushUP(RGB(255, 0, 0)), brushDOWN(RGB(0, 0, 255));
	CPen*	oldpen = pDC->SelectObject(&pen);
	CBrush*	oldbrush = pDC->SelectObject(&brushUP);
	double	curr{}, open{}, high{}, low{}, pcurr{}, max{}, min{}, aval{}, bval{};
	const double	width = rect.Width();
	CString	bongdata;

	CPoint	ptHigh, ptLow;
	CPoint	ptBox[5];

	ptHigh.y = ptLow.y = pt.y;
	ptBox[0].y = ptBox[3].y = ptBox[4].y = pt.y - 5;
	ptBox[1].y = ptBox[2].y = pt.y + 5;
	
	curr  = IH::TOfabs(GetItemText(nRow, colCURR));
	open  = IH::TOfabs(GetItemText(nRow, colOPEN));
	bongdata = GetItemText(nRow, nCol);
	high  = IH::TOfabs(IH::Parser(bongdata, PTAB));
	low   = IH::TOfabs(bongdata);
	pcurr = IH::TOfabs(GetItemText(nRow, colPCURR));
	
	if (pcurr)
	{
		pDC->MoveTo(pt.x, rcorg.top);
		pDC->LineTo(pt.x, rcorg.bottom);
	}

	CString	rcod;
	rcod = GetItemText(nRow, 0);
	if (rcod.IsEmpty() || !open || !high || !low || !curr)
	{
		pDC->SelectObject(oldbrush);
		pDC->SelectObject(oldpen);

		pen.DeleteObject();
		penUP.DeleteObject();
		penDOWN.DeleteObject();
		brushUP.DeleteObject();
		brushDOWN.DeleteObject();
		return;
	}
	
	double ldif = 0.295;		//2015.06.16 KSJ 0.15 -> 0.295
	if (rcod.GetAt(0) == 'A')
	{
		const double lchk = abs((int)(pcurr-low)) /pcurr;
		if(lchk > 0.30 	)	ldif = 0.5;		//2015.06.16 KSJ 0.15 -> 0.30

		max = pcurr + pcurr * ldif;
		min = pcurr - pcurr * ldif;
	}
	else
	{
		max = high;
		min = low;

		if (pcurr)
		{
			double	gap1 = fabs(high - pcurr);
			const double	gap2 = fabs(pcurr - low);

			
			gap1 = max(gap1, gap2);
			max = pcurr + gap1;
			min = pcurr - gap1;
		}
		
	}

	ptLow.x = (int)(width * (low - min) / (max - min) + (double)rect.left);
	ptHigh.x = (int)(width * (high - min) / (max - min) + (double)rect.left);

	if (curr > open)
	{
		pDC->SelectObject(&penUP);
		aval = open;
		bval = curr;
	}
	else if (curr < open)
	{
		pDC->SelectObject(&penDOWN);
		pDC->SelectObject(&brushDOWN);
		aval = curr;
		bval = open;
	}
	else
	{
		aval = bval = open;
		if (open >= pcurr)
		{
			pDC->SelectObject(&penUP);
		}
		else
		{
			pDC->SelectObject(&penDOWN);
			pDC->SelectObject(&brushDOWN);
		}
	}
	
	ptBox[0].x = ptBox[1].x = ptBox[4].x = (int)(width * (aval - min) / (max - min) + (double)rect.left);
	ptBox[2].x = ptBox[3].x = (int)(width * (bval - min) / (max - min) + (double)rect.left);

	pDC->MoveTo(ptHigh);
	pDC->LineTo(ptLow);
	pDC->Polygon(ptBox, 5);
	pDC->SelectObject(oldbrush);
	pDC->SelectObject(oldpen);

	pen.DeleteObject();
	penUP.DeleteObject();
	penDOWN.DeleteObject();
	brushUP.DeleteObject();
	brushDOWN.DeleteObject();
}

void CintGrid::DrawSig(CDC* pDC, CRect rect, int nRow)
{
	const LPARAM	lParam = GetItemData(nRow, colSIG);
	const WORD	sise = LOWORD(lParam);
	const WORD	news = HIWORD(lParam);
	CWnd*	pWnd = GetParent();
	const int	dataidx = pWnd->SendMessage(WM_MANAGE, MK_GETDATAKIND);

	CString recomDate;
	if(nRow > 0)
	{
		recomDate = m_pParent->GetRecommandInfo(nRow);
	}
	
	if(!recomDate.Compare("1") || !recomDate.Compare("2"))	//2012.11.19 KSJ ��õ�����϶��� recomDate�� 1�� �Ǿ �����´�. �׷��Ƿ� 1�̾ƴϸ� �׸��� ���ƾ���
	{
		CBitmap* pInfo = m_pBmpInfo6;
		
		if (nullptr != pInfo)
		{
			CDC buffDC;
			buffDC.CreateCompatibleDC(pDC);
			CBitmap* pOldBitmap = buffDC.SelectObject(pInfo);
			
			pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &buffDC, 0, 0, 14, 14, SRCCOPY);
			
			buffDC.SelectObject(pOldBitmap);

			buffDC.DeleteDC();
		}		
	}

	if (sise)
	{
// 		if(nRow < 1)
// 			DrawSise(pDC, rect);
	}

	if (news)
		DrawNews(pDC, rect);

	


	CPen	pen(PS_SOLID, 1, RGB(200, 200, 200));
	CPen*	oldpen = pDC->SelectObject(&pen);
	pDC->MoveTo(rect.left, rect.bottom);
	//pDC->LineTo(rect.right, rect.top);
	pDC->SelectObject(oldpen);
	pen.DeleteObject();
}

// ADD PSH 20070912
void CintGrid::DrawMarginRatio(CDC* pDC, CRect rect, int nRow)
{
	const LPARAM	lParam = GetItemData(nRow, colNAME);

	CString strText;
	UINT	bHund = FALSE;

	if (lParam % 100 == 0)
	{
		strText = "00";
		bHund   = TRUE;
	}
	else
		strText.Format("%d", lParam);

	CBrush NewBrush, *pOldBrush = nullptr;
	NewBrush.CreateSolidBrush(RGB(200, 200, 200));

	CFont *pOldFont = pDC->SelectObject(m_pFontMR);
	pOldBrush = pDC->SelectObject(&NewBrush);

	pDC->SelectStockObject(NULL_PEN);
	pDC->RoundRect(rect, CPoint(2, 2));

	rect.left--;
	const COLORREF clrTxt = pDC->SetTextColor(RGB(255, 255, 255));

	if (bHund)
	{
		rect.left  += 1;
		rect.right += 1;
		DrawText(pDC->m_hDC, strText, -1, rect, GVFM_HEADER);

		CPen NewPen, *pOldPen;
		NewPen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

		pOldPen = pDC->SelectObject(&NewPen);
		pDC->MoveTo(rect.left + 1, rect.top + 3);
		pDC->LineTo(rect.left + 1, rect.bottom - 4);
		pDC->SelectObject(pOldPen);
	}
	else
		DrawText(pDC->m_hDC, strText, -1, rect, GVFM_HEADER);

	pDC->SetTextColor(clrTxt);

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldBrush);
	
	NewBrush.DeleteObject();

	/*CPen	pen(PS_SOLID, 1, RGB(200, 200, 200));
	CPen*	oldpen = pDC->SelectObject(&pen);
	pDC->MoveTo(rect.left, rect.bottom);
	pDC->LineTo(rect.right, rect.top);
	pDC->SelectObject(oldpen);
	pen.DeleteObject();*/
}

void CintGrid::SetBmpIcon(CString strFile, int nType)
{
	HBITMAP hBitmap = (HBITMAP)::LoadImage(nullptr, strFile.GetBuffer(0),
					  IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); 

	switch(nType)
	{
	case 0:
		{
			if (nullptr == m_pBmpMarker)
			{				
				if (nullptr == hBitmap)
					return;

				m_pBmpMarker = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 1:
		{
			if (nullptr == m_pBmpInfo1)
			{	
				if (nullptr == hBitmap)
					return;

				m_pBmpInfo1 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 2:
		{
			if (nullptr == m_pBmpInfo2)
			{
				if (nullptr == hBitmap)
					return;

				m_pBmpInfo2 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 3:
		{
			if (nullptr == m_pBmpInfo3)
			{
				if (nullptr == hBitmap)
					return;

				m_pBmpInfo3 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 4:
		{
			if (nullptr == m_pBmpInfo4)
			{
				if (nullptr == hBitmap)
					return;

				m_pBmpInfo4 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 5:
		{
			if (nullptr == m_pBmpInfo5)
			{	
				if (nullptr == hBitmap)
					return;

				m_pBmpInfo5 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 6:
		{
			if (nullptr == m_pBmpInfo6)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo6 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 7:
		{
			if (nullptr == m_pBmpInfo7)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo7 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 8:
		{
			if (nullptr == m_pBmpInfo8)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo8 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 9:
		{
			if (nullptr == m_pBmpInfo9)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo9 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 10:
		{
			if (nullptr == m_pBmpInfo10)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo10 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 11:
		{
			if (nullptr == m_pBmpInfo11)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo11 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;

		//2012.06.20 KSJ ����, ��� �߰�
	case 12:
		{
			if (nullptr == m_pBmpInfo12)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo12 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 13:
		{
			if (nullptr == m_pBmpInfo13)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo13 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;

	case 14:
		{
			if (nullptr == m_pBmpInfo14)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo14 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 15:
		{
			if (nullptr == m_pBmpInfo15)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo15 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 16:
		{
			if (nullptr == m_pBmpInfo16)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo16 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 17:
		{
			if (nullptr == m_pBmpInfo17)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo17 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 18:
		{
			if (nullptr == m_pBmpInfo18)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo18 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 19:
		{
			if (nullptr == m_pBmpInfo19)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo19 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 20:
		{
			if (nullptr == m_pBmpInfo20)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo20 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 21:
		{
			if (nullptr == m_pBmpInfo21)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo21 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 22:
		{
			if (nullptr == m_pBmpInfo22)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo22 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
	case 23:
		{
			if (nullptr == m_pBmpInfo23)
			{	
				if (nullptr == hBitmap)
					return;
				
				m_pBmpInfo23 = CBitmap::FromHandle(hBitmap);
			}
		}
		break;
		//KSJ
	}
}
// END ADD

HBITMAP CintGrid::getBitmap(CString path)
{
	CBitmap*	pBitmap = (CBitmap*)m_pViewWnd->SendMessage(WM_USER, getBITMAP, (LPARAM)path.operator LPCTSTR());
	return pBitmap->operator HBITMAP();
}

int CintGrid::GetTypeInRect(CRect rect, CPoint pt)
{
	if (!rect.PtInRect(pt))
		return TK_NORMAL;
	CPoint	ptTL[4];
	CRgn	rgn;

	ptTL[0] = ptTL[3] = rect.TopLeft();
	ptTL[1] = CPoint(rect.left, rect.bottom);
	ptTL[2] = CPoint(rect.right, rect.top);

	rgn.CreatePolygonRgn(ptTL, 4, ALTERNATE);
	if (rgn.PtInRegion(pt))
		return TK_NEWS;
	return TK_CHART;
}

void CintGrid::DrawHeaderNews(CDC* pDC, CRect rect)
{
	CPoint	pt[4];
	CRgn	rgn;
	const COLORREF color = pDC->SetTextColor(RGB(255, 255, 255));
	//CFont*	pFont = pDC->SelectObject(m_pFont);
	CFont* pFont = GetAxFont("����ü",8);
	pDC->SelectObject(pFont);
	const int	nmode = pDC->SetBkMode(TRANSPARENT);
	
	pt[0] = rect.TopLeft();	
	pt[1] = CPoint(rect.left, rect.bottom);
	pt[2] = rect.BottomRight();
	pt[3] = CPoint(rect.right, rect.top);

	rgn.DeleteObject();
	rgn.CreatePolygonRgn(pt, 4, ALTERNATE);
	CBrush	brush(RGB(128, 128, 255));
	pDC->FillRgn(&rgn, &brush);
	rect.OffsetRect(2,1);
	//rect.OffsetRect(3, 2);

	pDC->DrawText("��", rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	pDC->SetBkMode(nmode);
	pDC->SetTextColor(color);
	pDC->SelectObject(pFont);
	rgn.DeleteObject();
	brush.DeleteObject();
}

void CintGrid::DrawNews(CDC* pDC, CRect rect)
{
	CPoint	pt[4];
	CRgn	rgn;
	const COLORREF color = pDC->SetTextColor(RGB(255, 255, 255));
	CFont*	pFont = pDC->SelectObject(m_pFontS);
	const int	nmode = pDC->SetBkMode(TRANSPARENT);
	
	pt[0] = pt[3] = rect.TopLeft();	
	pt[1] = CPoint(rect.left, rect.bottom);
	pt[2] = CPoint(rect.right, rect.top);
	rgn.DeleteObject();
	rgn.CreatePolygonRgn(pt, 4, ALTERNATE);
	CBrush	brush(RGB(128, 128, 255));
	pDC->FillRgn(&rgn, &brush);
	rect.OffsetRect(3, 2);
	pDC->DrawText("��", rect, DT_SINGLELINE|DT_TOP|DT_LEFT);
	pDC->SetBkMode(nmode);
	pDC->SetTextColor(color);
	pDC->SelectObject(pFont);
	rgn.DeleteObject();
	brush.DeleteObject();
}

void CintGrid::DrawSise(CDC* pDC, CRect rect, bool bChart)
{
// 	CPoint	pt[4];
// 	CRgn	rgn;
// 	CRect	rcGraph = rect;
// 	
// 	pt[0] = pt[3] = rect.BottomRight();
// 	pt[1] = CPoint(rect.left, rect.bottom);
// 	pt[2] = CPoint(rect.right, rect.top);
// 
// 	rgn.DeleteObject();
// 	rgn.CreatePolygonRgn(pt, 4, ALTERNATE);
// 	CBrush	brush(RGB(255, 255, 128));
// 	pDC->FillRgn(&rgn, &brush);
// 	rgn.DeleteObject();
// 	brush.DeleteObject();
// 	
// 	if (bChart)
// 	{
// 		// graph1
// 		rcGraph.left += 6;
// 		rcGraph.top = rcGraph.bottom - 3;
// 		rcGraph.right = rcGraph.left + 3;
// 		pDC->FillSolidRect(rcGraph, RGB(0, 0, 255));
// 
// 		// graph2
// 		rcGraph.OffsetRect(4, 0);
// 		rcGraph.top -= 4;
// 		pDC->FillSolidRect(rcGraph, RGB(255, 0, 0));
// 
// 		// graph3
// 		rcGraph.OffsetRect(4, 0);
// 		rcGraph.top -= 4;
// 		pDC->FillSolidRect(rcGraph, RGB(0, 255, 0));
// 	}
}


void CintGrid::DrawExpect(CDC* pDC, CRect rect, CString str)
{
	CPen	pen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen*	oldpen = pDC->SelectObject(&pen);
	const CSize	size = pDC->GetTextExtent(str);
	CPoint	pt = rect.CenterPoint();
	pt.y += ((size.cy / 2));

	pDC->MoveTo(rect.right, pt.y);
	pDC->LineTo(rect.right - size.cx, pt.y);
	pDC->MoveTo(rect.right, pt.y + 1);
	pDC->LineTo(rect.right - size.cx, pt.y + 1);

	pDC->SelectObject(oldpen);
	pen.DeleteObject();
}

void CintGrid::OnMove(int x, int y) 
{
	CWnd::OnMove(x, y);
	
	const int nX = x;
	const int nY = y;
}


BOOL CintGrid::IsSelectedRow(int nRow)
{
	if (nRow > GetRowCount() || nRow < GetFixedRowCount())
		return FALSE;

	const int nCount = GetColumnCount();

	for (int i = GetFixedColumnCount(); i < nCount; i++)
	{
		const auto pCell = GetCell(nRow, i);

		if (!pCell)
			return FALSE;

		if (!(pCell->state & GVST_SELECTED))
			return FALSE;
	}

	return TRUE;
}





BOOL CintGrid::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CWnd* grpWnd{};
	
	switch (LOWORD(wParam))
	{
	case IDC_BT_DOMINO:
		grpWnd = (CWnd*) GetParent(); 
		grpWnd->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_BT_DOMINO,0), 0);
		break;
	case IDC_BT_SAVE:
		//m_GridWnd[m_nCurSel]->saveInterestX();
		grpWnd = (CWnd*) GetParent(); 
		grpWnd->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_BT_SAVE,0), 0);
		
		break;
	case IDC_BT_ERASE:
//		CloseGrid(m_nIndex);
		grpWnd = (CWnd*) GetParent(); 
		grpWnd->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_BT_ERASE,0), 0);
		break;
	}
	return CWnd::OnCommand(wParam, lParam);
}

BOOL CintGrid::ParseFromOuter(COleDataObject *pDataObject)
{
	// Get the text from the COleDataObject
	HGLOBAL hmem = pDataObject->GetGlobalData(CF_TEXT);
	CMemFile memFile((BYTE*) ::GlobalLock(hmem), ::GlobalSize(hmem));

	// CF_TEXT is ANSI text, so we need to allocate a char* buffer to hold this.
	CString strText;


	memFile.Read(strText.GetBuffer(::GlobalSize(hmem) + 1), ::GlobalSize(hmem));
	::GlobalUnlock(hmem); ::GlobalFree(hmem);	
	strText.ReleaseBuffer();

	CString strA,strB,strC;
	strA = IH::Parser(strText,"\t");
	strB = IH::Parser(strText,"\t");
	strC = IH::Parser(strText,"\t");

	strC.TrimRight();
	if ( strC.IsEmpty() && strA.Find("1301") >= 0)
	{
		CintGrid::m_dropdata.SetCode(strB);
		return TRUE;
	}
	return FALSE;
}


void CintGrid::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	TRACE("CintGrid::OnNcMouseMove\n");
	
	CWnd::OnNcMouseMove(nHitTest, point);
}

int	CintGrid::GetGridWidth()
{
	int sum = 0;
	for(int i=0 ; i<GetColumnCount() ; i++)
	{
		sum = sum + GetColumnWidth(i);
	}
	
	return sum;
}

void CintGrid::SetFontSize(int size)
{
	m_point = size;
}