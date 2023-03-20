// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB202200.h"
#include "MainWnd.h"
#include "toolwnd.h"
#include "groupwnd.h"
#include "TreeWnd.h"
#include "BaseWnd.h"
#include "optdlg.h"
#include "GroupWnd.h"
#include "UsefulSetupDlg.h"
#include <afxsock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd
#define TM_RTSTIME 9898

CMainWnd::CMainWnd(CWnd *pWnd)
{
	m_pWnd = pWnd;
	m_pToolWnd = nullptr;
	m_pTreeWnd = nullptr;
	m_pGroupWnd = nullptr;
	m_nTree = 0;
	m_nBasket = 0;
	m_nGroup = 0;
	m_bTree = FALSE;
	m_bBasket = FALSE;
	m_bRTS = FALSE;
	m_pOptDlg = nullptr;
	m_bDestroy = false;
	m_nResize = MO_NONE;
	m_bProc = false;

	m_bWaveApply = FALSE;

	m_clrKospi = RGB(0, 0, 0);
	m_clrKosdaq = RGB(128, 64, 64);
	m_bMKClrApply = FALSE;
	m_bPopupDlg = FALSE;
	m_bDispCode = FALSE;
	m_bNewsNChart = TRUE; // 2011.12.02	KSJ
	m_bMargin = FALSE;    // 2011.12.02	KSJ
	m_bInfo = TRUE;	      // 2011.12.02	KSJ
	m_bStartMsg = FALSE;
	m_bDLLDATA = FALSE;
	m_bRemove = FALSE;	  // 2011.12.29 KDK
	m_bDominoToolWnd = false; // 2012.01.13	KSJ
	m_bAlertx = FALSE;	  // 2012.01.19 KSJ Alertx �߰�

	m_strTag = "";
	m_bWin7or8 = true; // 2013.01.23 KSJ win7 or win8���� Ȯ��

	m_bUser9111 = FALSE;
	m_bUser9555 = FALSE;

	m_bChange = FALSE;

	m_bSkipRTS = TRUE;
	m_bcustomer = false;
}

CMainWnd::~CMainWnd()
{
}

BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
//{{AFX_MSG_MAP(CMainWnd)
ON_WM_CREATE()
ON_WM_DESTROY()
ON_WM_PAINT()
ON_WM_SIZE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_SETCURSOR()
ON_WM_LBUTTONDBLCLK()
ON_WM_TIMER()
//}}AFX_MSG_MAP
ON_MESSAGE(WM_MANAGE, OnManage)
ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers
int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(8787, 500, nullptr);

	CString slog;
	CString userip;
	m_bcustomer = m_pWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L);
	userip = CheckIP();
	CString filePath;
	filePath.Format("%s/%s/InterOption.ini", Variant(homeCC), "tab");

	int iTime = 800;
	if (!m_bcustomer) //������
	{
		iTime = GetPrivateProfileInt("STAFF", "TIME", 800, filePath);
		SetTimer(TM_RTSTIME, iTime, nullptr);
		return 1;
	}

	//���� HTS
	int icount = 0;
	iTime  = GetPrivateProfileInt("CUSTOMER", "TIME", 200, filePath);
	icount = GetPrivateProfileInt("IPLIST", "COUNT", 0, filePath);
	if (icount == 0)
	{
		SetTimer(TM_RTSTIME, iTime, nullptr);
		return 1;
	}
	else
	{
		BOOL binip = false;
		char buf[1024]{};
		CString siplist, eiplist, sip, eip;
		for (int ii = 0; ii < icount; ii++)
		{
			siplist.Format("IP00%d_STT", ii + 1);
			eiplist.Format("IP00%d_END", ii + 1);

			memset(buf, 0x00, sizeof(buf));
			GetPrivateProfileString("IPLIST", siplist, "", buf, sizeof(buf), filePath);
			sip.Format("%s", buf);

			memset(buf, 0x00, sizeof(buf));
			GetPrivateProfileString("IPLIST", eiplist, "", buf, sizeof(buf), filePath);
			eip.Format("%s", buf);

			binip = isIPInRange(userip, sip, eip); //���� HTS ������ ip�뿪�� �������Ǿ�
			if (binip)
			{
				iTime = GetPrivateProfileInt("SCUSTOMER", "TIME", 700, filePath);
				SetTimer(TM_RTSTIME, iTime, nullptr);
				// slog.Format("[2022]TIMER ��������IP�뿪 ip=[%s]  iTime=[%d]", userip, iTime);
				return 1;
			}
		}
	}


	//���������� IP�� ������ �뿪�� �ƴѰ�� (��κ� ��)
	// slog.Format("[2022]TIMER �����Ϲ� ip=[%s] iTime=[%d]", userip, iTime);
	SetTimer(TM_RTSTIME, iTime, nullptr);
	return 1;
}

void CMainWnd::GuideMessage(CString msg)
{
	AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axGUIDE, 1), (LONG)(char *)msg.operator LPCTSTR());
}

void CMainWnd::OnDestroy()
{
	m_bDestroy = true;

	CWnd::OnDestroy();

	saveinfo();

	DestroyChild();

	CString strCfg, filepath;
	strCfg.Format("intercfg%s.ini", m_strTag);
	filepath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, strCfg);

	CString basefile;
	basefile.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, FILE_CONFIG);

	CFileFind finder;

	if (finder.FindFile(filepath))
		::CopyFile(filepath, basefile, FALSE);


	CString file, memoFile;
	file.Format("%s\\%s\\%s\\%s.ini", m_home, USRDIR, m_user, m_user);
	memoFile.Format("%s\\%s\\%s\\memo.mmo", m_home, USRDIR, m_user);

	bool bMemo;

	bMemo = false;

	if (finder.FindFile(memoFile))
	{
		WritePrivateProfileString("MEMO", "USE", "1", file);
		bMemo = true;
	}
	else
	{
		WritePrivateProfileString("MEMO", "USE", "0", file);
		bMemo = false;
	}

	if (!bMemo)
		return;

	int lineN = 0;

	TRY
	{
		UINT nBytesRead = 0;
		CFile fileP(memoFile, CFile::modeRead | CFile::shareDenyNone);
		CString code, data;
		char fcode[12]{}, flen[4]{};
		int readL = 0;

		fileP.SeekToBegin();

		CString s;

		while (true)
		{
			nBytesRead = fileP.Read(&fcode, sizeof(fcode));
			if (nBytesRead < sizeof(fcode))
				break;
			nBytesRead = fileP.Read(&flen, sizeof(flen));
			if (nBytesRead < sizeof(flen))
				break;
			
			readL = atoi(CString(flen, sizeof(flen)));		
			nBytesRead = fileP.Read(data.GetBufferSetLength(readL), readL);
			data.ReleaseBuffer();

			code = CString(fcode, 12);
			if (!code.IsEmpty())
				lineN++;
		}
		fileP.Close();
	}
	CATCH(CException, e)
	{
	}
	END_CATCH

	CString sTmp;
	sTmp.Format("%d", lineN);
	WritePrivateProfileString("MEMO", "COUNT", sTmp, file);

	_mapRealData.clear();
}

void CMainWnd::loadinfo()
{
	CString filepath;

	CString strCfg;
	strCfg.Format("intercfg%s.ini", m_strTag);
	filepath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, strCfg);

	if (GetPrivateProfileInt(SEC_MAIN, KEY_VERSION, 0, filepath) < 1)
		::DeleteFile(filepath);

	char buf[1024]{};
	CString sztmp, szUnit;
	CString section, key;
	// int	val = 0;
	memset(buf, 0x00, sizeof(buf));

	// height
	m_size.cx = m_size.cy = 0;
	m_size.cy = GetPrivateProfileInt(SEC_MAIN, KEY_HEIGHT, SIZE_HEIGHT, filepath);

	// Ʈ�� ������
	m_nTree = GetPrivateProfileInt(SEC_MAIN, KEY_TREESIZE, SIZE_TREE, filepath);
	m_bTree = (BOOL)GetPrivateProfileInt(SEC_MAIN, KEY_TREEUSE, 0, filepath);

	// ��ġ �ɼ�
	m_nOption = GetPrivateProfileInt(SEC_MAIN, KEY_SOPTION, OPT_DEFAULT, filepath);

	if (m_bTree)
	{
		m_size.cx += m_nTree;
		m_size.cx += SIZE_DIVIDE;
	}

	if (m_bRTS)
		m_size.cx += SIZE_RTS;

	// ��ٱ��� ������
	m_nBasket = GetPrivateProfileInt(SEC_MAIN, KEY_BASKETSIZE, SIZE_BASKET, filepath);
	m_bBasket = (BOOL)GetPrivateProfileInt(SEC_MAIN, KEY_BASKETUSE, 0, filepath);

	if (m_bBasket)
	{
		m_size.cx += m_nBasket;
		m_size.cx += SIZE_DIVIDE;
	}

	// group
	m_nGroup = GetPrivateProfileInt(SEC_MAIN, KEY_GROUPSIZE, SIZE_GROUP, filepath);

	m_size.cx += m_nGroup;

	// Wave File
	m_bWaveApply = (BOOL)GetPrivateProfileInt(SEC_MAIN, "APPWAVE", 0, filepath);

	CString strWaveFile;
	strWaveFile.Format("%s\\%s\\%s", m_home, IMAGEDIR, "000.WAV");

	memset(buf, 0x00, sizeof(buf));
	if (0 == GetPrivateProfileString(SEC_MAIN, "WAVE00", "", buf, sizeof(buf), filepath))
	{
		m_strWaveFile = strWaveFile;
	}
	else
	{
		m_strWaveFile.Format("%s", buf);
	}

	m_bMKClrApply = (BOOL)GetPrivateProfileInt(SEC_MAIN, "APPMKCLR", 0, filepath);

	m_clrKospi = GetPrivateProfileInt(SEC_MAIN, "KOSPI", 0, filepath);

	m_clrKosdaq = GetPrivateProfileInt(SEC_MAIN, "KOSDAQ", 0, filepath);

	if (0 == GetPrivateProfileString(SEC_MAIN, "POPUP1", "", buf, sizeof(buf), filepath))
	{
		m_bPopupDlg = FALSE;
	}
	else
	{
		m_bPopupDlg = atoi(buf);
	}

	if (0 == GetPrivateProfileString(SEC_MAIN, KEY_DISPCODE, "", buf, sizeof(buf), filepath))
	{
		m_bDispCode = FALSE;
	}
	else
	{
		m_bDispCode = atoi(buf);
	}

	// 2011.12.12	KSJ
	if (0 == GetPrivateProfileString(SEC_MAIN, KEY_NEWSNCHART, "", buf, sizeof(buf), filepath))
	{
		m_bNewsNChart = TRUE;
	}
	else
	{
		m_bNewsNChart = atoi(buf);
	}

	if (0 == GetPrivateProfileString(SEC_MAIN, KEY_MARGIN, "", buf, sizeof(buf), filepath))
	{
		m_bMargin = FALSE;
	}
	else
	{
		m_bMargin = atoi(buf);
	}

	if (0 == GetPrivateProfileString(SEC_MAIN, KEY_SYMINFO, "", buf, sizeof(buf), filepath))
	{
		m_bInfo = FALSE;
	}
	else
	{
		m_bInfo = atoi(buf);
	}

	if (0 == GetPrivateProfileString(SEC_MAIN, KEY_TICKER, "", buf, sizeof(buf), filepath))
	{
		m_bTicker = FALSE;
	}
	else
	{
		m_bTicker = atoi(buf);
	}
	// KSJ
}

void CMainWnd::saveinfo()
{
	CString filepath;
	CString sztmp = _T("");

	CString strCfg;
	strCfg.Format("intercfg%s.ini", m_strTag);

	filepath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, strCfg);

	WritePrivateProfileString(SEC_MAIN, KEY_VERSION, "1", filepath);

	// Ʈ��������
	sztmp.Format("%d", m_nTree);
	WritePrivateProfileString(SEC_MAIN, KEY_TREESIZE, sztmp, filepath);
	WritePrivateProfileString(SEC_MAIN, KEY_TREEUSE, (m_bTree) ? "1" : "0", filepath);

	sztmp.Format("%d", m_nOption);
	WritePrivateProfileString(SEC_MAIN, KEY_SOPTION, sztmp, filepath);

	// ��ٱ��� ������
	sztmp.Format("%d", m_nBasket);
	WritePrivateProfileString(SEC_MAIN, KEY_BASKETSIZE, sztmp, filepath);
	WritePrivateProfileString(SEC_MAIN, KEY_BASKETUSE, (m_bBasket) ? "1" : "0", filepath);

	// group
	sztmp.Format("%d", m_size.cx - ((m_bTree) ? m_nTree + SIZE_DIVIDE : 0)); // BUFFET ADD FOR SIZE

	WritePrivateProfileString(SEC_MAIN, KEY_GROUPSIZE, sztmp, filepath);

	sztmp.Format("%d", m_size.cy);
	WritePrivateProfileString(SEC_MAIN, KEY_HEIGHT, sztmp, filepath);

	sztmp.Format("%d", m_bWaveApply);
	WritePrivateProfileString(SEC_MAIN, "APPWAVE", sztmp, filepath);

	WritePrivateProfileString(SEC_MAIN, "WAVE00", m_strWaveFile, filepath); // 2017.01.25 KSJ GetBuffer(0) ����

	sztmp.Format("%d", m_bMKClrApply);
	WritePrivateProfileString(SEC_MAIN, "APPMKCLR", sztmp, filepath);

	sztmp.Format("%d", m_clrKospi);
	WritePrivateProfileString(SEC_MAIN, "KOSPI", sztmp, filepath);

	sztmp.Format("%d", m_clrKosdaq);
	WritePrivateProfileString(SEC_MAIN, "KOSDAQ", sztmp, filepath);

	sztmp.Format("%d", m_bPopupDlg);
	WritePrivateProfileString(SEC_MAIN, "POPUP1", sztmp, filepath);
}

HBITMAP CMainWnd::getBitmap(CString path)
{
	CBitmap *pBitmap = (CBitmap *)m_pWnd->SendMessage(WM_USER, getBITMAP, (LPARAM)path.operator LPCTSTR());
	return pBitmap->operator HBITMAP();
}

CFont *CMainWnd::GetAxFont(CString fName, int point, bool bBold, bool bItalic)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = bItalic;
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;
	return (CFont *)m_pWnd->SendMessage(WM_USER, getFONT, (long)&fontR);
}

void CMainWnd::CreateChild()
{
	const COLORREF color = getAxColor(64);
	CString fileIMG;
	fileIMG.Format("%s\\%s\\", m_home, IMAGEDIR);

	HBITMAP hBITMAP{}, hBITMAP_DN{}, hBITMAP_HV{};
	hBITMAP = getBitmap(fileIMG + "TROPT.bmp");
	hBITMAP_DN = getBitmap(fileIMG + "TROPT_dn.bmp");
	hBITMAP_HV = getBitmap(fileIMG + "TROPT_en.bmp");
	m_pFont = GetAxFont();

	m_btOption.Create("�˻��ɼ�\n����������", CRect(0, 0, 0, SIZE_BUTTONS), this, IDC_BT_CONFIG, TRUE, TRUE);
	m_btOption.SetFont(m_pFont, false);
	m_btOption.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	m_btOption.SetCheck(false);
	m_nOptHeight = 0;
	if (m_pOptDlg == nullptr)
	{
		m_pOptDlg = std::make_unique<COptDlg>(this);
		m_pOptDlg->Create(COptDlg::IDD, this);
		CRect rect;
		SearchOption(MO_SET, m_nOption);
		m_pOptDlg->GetWindowRect(rect);
		m_nOptHeight = rect.Height();
	}
	if (m_pTreeWnd == nullptr)
	{
		m_pTreeWnd = std::make_unique<CTreeWnd>(this);
		m_pTreeWnd->Create(WS_CHILD | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS, CRect(0, 0, 0, 0), this, IDC_TREE);
	}
	if (m_pToolWnd == nullptr)
	{
		m_pToolWnd = std::make_unique<CToolWnd>(this);
		m_pToolWnd->Create(nullptr, nullptr, WS_CHILD, CRect(0, 0, 0, 0), this, IDC_TOOL);
		m_pToolWnd->SendMessage(WM_MANAGE, MK_SETPAL, (LPARAM)color);
	}

	if (m_pGroupWnd == nullptr)
	{
		m_pGroupWnd = std::make_unique<CGroupWnd>(this);
		m_pGroupWnd->m_strWaveFile = m_strWaveFile;
		m_pGroupWnd->m_bWaveApply = m_bWaveApply;
		m_pGroupWnd->m_bMKClrApply = m_bMKClrApply;
		m_pGroupWnd->m_clrKospi = m_clrKospi;
		m_pGroupWnd->m_clrKosdaq = m_clrKosdaq;
		m_pGroupWnd->m_bPopupDlg = m_bPopupDlg;
		m_pGroupWnd->m_bDispCode = m_bDispCode;
		m_pGroupWnd->m_bInfo = m_bInfo;		   // 2011.12.02	KSJ
		m_pGroupWnd->m_bMargin = m_bMargin;	   // 2011.12.02	KSJ
		m_pGroupWnd->m_bNewsNChart = m_bNewsNChart; // 2011.12.02	KSJ

		m_pGroupWnd->Create(nullptr, nullptr, WS_CHILD, CRect(0, 0, 0, 0), this, IDC_GROUP);
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_SETPAL, (LPARAM)color);
	}

	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKEWPARAM(m_size.cx, m_size.cy));
	MoveWindow(0, 0, m_size.cx, m_size.cy);
	RequestMarketTime();
}

void CMainWnd::DestroyChild()
{
	m_btOption.DestroyWindow();
}

LONG CMainWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG ret = 0;
	switch (LOWORD(wParam))
	{
	case MK_GETKEY:
		ret = atoi(m_strTag);
		break;
	case MK_ISTREE:
		ret = m_bTree;
		break;

	case MK_GROUPHEIGHT:
	{
		int nHeight = m_size.cy;
		if (!m_pToolWnd->SendMessage(WM_MANAGE, MK_TOOLSIZE))
			nHeight -= (SIZE_TOOL / 2 + 1);
		else
			nHeight -= SIZE_TOOL;
		if (m_bBasket)
			nHeight -= m_nBasket;
		ret = nHeight;
	}
	break;
	case MK_PROCDLL:
		m_bProc = true;
		break;
	case MK_VARIANT:
	{
		m_szRET = _T("");
		m_szRET.Format("%s", Variant(HIWORD(wParam), (char *)lParam)); // test
		/*  �������̻� �ڸ����� �״´� �Ʒ�ó�� ó�� ����� �Ѵ�. �켱 �ּ�
		if(HIWORD(wParam) == passCCx )
			m_szRET.Format("%s", Variant(HIWORD(wParam), ""));
		else
			m_szRET.Format("%s", Variant(HIWORD(wParam), (char*)lParam));
		*/
		ret = (LONG)(char *)m_szRET.operator LPCTSTR();
	}
	break;
	case MK_SHOWTOOLTIP:
		Variant(guideCC, (char *)lParam);
		break;

	case MK_TOOLSIZE:
		ResizeOper(m_size.cx, m_size.cy);
		break;

	case MK_SYNC:
		switch (HIWORD(wParam))
		{
		case MO_LOCK:
			break;
		case MO_UNLOCK:
			break;
		}
		break;
	case MK_GETID:
		ret = (LONG)(char *)m_id.operator LPCTSTR();
		break;
	case MK_GETROOT:
		ret = (LONG)(char *)m_home.operator LPCTSTR();
		break;
	case MK_GETUSER:
		ret = (LONG)(char *)m_user.operator LPCTSTR();
		break;
	case MK_GETBITMAP:
		ret = (LONG)m_pWnd->SendMessage(WM_USER, getBITMAP, lParam);
		break;
	case MK_TREEVISIBLE:
		ret = OperTreeVisible(HIWORD(wParam));
		break;
	case MK_RTSVISIBLE:
		break;
	case MK_BASKETVISIBLE:
		ret = OperBasketVisible(HIWORD(wParam));
		break;
	case MK_SIZE:
		OperSize(HIWORD(wParam), CSize(LOWORD(lParam), HIWORD(lParam)));
		break;
	case MK_OPENSCR:
		OpenScreen(HIWORD(wParam), (char *)lParam);
		break;
	case MK_SEARCHOPT:
		ret = SearchOption(HIWORD(wParam), (DWORD)lParam);
		break;
	case MK_GETWND:
		switch (HIWORD(wParam))
		{
		case MO_VIEW:
			ret = (LONG)m_pWnd;
			break;
		case MO_GROUP:
			ret = (LONG)m_pGroupWnd.get();
			break;
		case MO_TOOL:
			ret = (LONG)m_pToolWnd.get();
			break;
		case MO_TREE:
			ret = (LONG)m_pTreeWnd.get();
			break;
		case MO_GRID:
			ret = m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GRID));
			break;
		}
		break;
	case MK_SELGROUP:
		if (HIWORD(wParam) == MO_SET)
			SelectGroup((int)lParam);
		break;
	// by pass message
	case MK_SENDTR:
		SendOper((CSendData *)lParam);
		break;
	case MK_GROUP:
	case MK_GETGROUP:
	case MK_CHANGEFIELD:
	case MK_EXCEL:
	case MK_EXPECT:
		ret = (LONG)m_pGroupWnd->SendMessage(WM_MANAGE, wParam, lParam);
		break;
	case MK_AUTOEXPECT:
		ret = (LONG)m_pGroupWnd->SendMessage(WM_MANAGE, wParam, lParam);
		break;
	case MK_EQUALIZER:
		ret = (LONG)m_pGroupWnd->SendMessage(WM_MANAGE, wParam, lParam);
		break;
	case MK_TREEDATA:
		ret = (LONG)m_pGroupWnd->SendMessage(WM_MANAGE, wParam, lParam); // 4
		break;
	case MK_UPJONGDATA:
		ret = (LONG)m_pGroupWnd->SendMessage(WM_MANAGE, wParam, lParam); // 4
		break;
	case MK_ALERTWAVE:
	{
		CUsefulSetupDlg *pDlg = (CUsefulSetupDlg *)lParam;

		if (nullptr != pDlg)
		{
			m_bWaveApply = pDlg->m_bWaveApply;
			m_strWaveFile = pDlg->m_strFile;
		}
	}
	break;
	case MK_MARKETCOLOR:
	{
		CUsefulSetupDlg *pDlg = (CUsefulSetupDlg *)lParam;

		if (nullptr != pDlg)
		{
			m_bMKClrApply = pDlg->m_bClrApply;
			m_clrKospi = pDlg->m_clrKospi;
			m_clrKosdaq = pDlg->m_clrKosdaq;
		}
	}
	break;
	case MK_CLOSE:
	{
		OnDestroy();
	}
	break;
	case MK_MARKETCODE:
	{
		m_bDispCode = (BOOL)lParam;

		CString sztmp;
		sztmp.Format("%d", m_bDispCode);

		SaveItemInfo(KEY_DISPCODE, sztmp);
	}
	break;
		// 2011.12.02	KSJ
	case MK_NEWSNCHART:
	{
		m_bNewsNChart = (BOOL)lParam;

		CString sztmp;
		sztmp.Format("%d", m_bNewsNChart);

		SaveItemInfo(KEY_NEWSNCHART, sztmp);
	}
	break;
		// KSJ

		// 2011.12.05	KSJ
	case MK_MARGIN:
	{
		m_bMargin = (BOOL)lParam;

		CString sztmp;
		sztmp.Format("%d", m_bMargin);

		SaveItemInfo(KEY_MARGIN, sztmp);
	}
	break;
		// KSJ

		// 2011.12.12	KSJ
	case MK_INFO:
	{
		m_bInfo = (BOOL)lParam;

		CString sztmp;
		sztmp.Format("%d", m_bInfo);

		SaveItemInfo(KEY_SYMINFO, sztmp);
	}
	break;
		// KSJ

		// 2011.12.12	KSJ
	case MK_TICKER:
	{
		m_bTicker = (BOOL)lParam;

		CString sztmp;
		sztmp.Format("%d", m_bTicker);

		SaveItemInfo(KEY_TICKER, sztmp);
	}
	break;
		// KSJ

	case MK_ARRANGE:
		ret = (LONG)m_pGroupWnd->SendMessage(WM_MANAGE, wParam, lParam);
		break;
	case MK_RTS_DATA:
		//RTS_ThreadSendToGroup();
		break;
	case MK_ISSKIP:
		m_bSkipRTS = lParam;
		break;
	case MK_MARKETTIME:
	{
		CString slog;
		CString sdata;
		sdata.Format("%s", (char *)lParam);
		m_strBeginTime = IH::Parser(sdata, "|"); //����ȣ�� ����۽ð�
		m_strBeginTimeEnd = IH::Parser(sdata, "|");
		m_strEndTime = IH::Parser(sdata, "|"); //����ȣ�� �帶���ð�
		m_strEndTimeEnd = IH::Parser(sdata, "|");
	}
	break;

	case MK_GETGROUPTR:
		Request_GroupList();
		break;

	default:
		break;
	}
	return ret;
}

// 2011.12.13 KSJ
//������Ȳ, �����ڵ�, Ư�̻���, ���ű�, ƼĿ�� �ٲ� ���� INI������ �����Ѵ�.
void CMainWnd::SaveItemInfo(CString strKeyName, CString strValue)
{
	CString strCfg;
	strCfg.Format("%s\\%s\\%s\\intercfg%s.ini", m_home, USRDIR, m_user, m_strTag);

	WritePrivateProfileString(SEC_MAIN, strKeyName, strValue, strCfg);
}
// KSJ

void CMainWnd::SetPallette()
{
	const COLORREF color = getAxColor(64);
	if (m_pToolWnd)
		m_pToolWnd->SendMessage(WM_MANAGE, MK_SETPAL, (LPARAM)color);

	if (m_pGroupWnd)
		m_pGroupWnd->SetGridBkColor(color);

	m_strPal = GetPalette();

	Invalidate(false);
}

LONG CMainWnd::OperDLLOUB(CRecvData& rdata)
{
	if (m_bDestroy)
		return 0;

	constexpr LONG ret = 0;
	WORD kind = rdata.key;
	struct _trkey* trkey = (struct _trkey*)&kind;

	switch (trkey->kind)
	{
	case TRKEY_GROUPACCNLIST:
	case TRKEY_ACCCUSTOM:
	case TRKEY_ACCSTAFF:
	case TRKEY_NEWS:
	case TRKEY_REMAIN:		
		m_pTreeWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata);
		break;
	case TRKEY_ELWISSUESEC:
		m_pTreeWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata);
		break;
	case TRKEY_ELWBASEASSET:
		m_pTreeWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata);
		break;
	case TRKEY_RECOMMAND:
		m_pTreeWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata);
		break;
	case TRKEY_GRIDUPJONG:
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata);
		break;
	case TRKEY_MARKETTIME: // 0212.11.25 ���ð� ��ȸ
		SetMarketTime((char *)rdata.data);
		break;
	case TRKEY_GRIDNEW:
	case TRKEY_GRIDNEWS:
	case TRKEY_GRIDROW:
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata);
		break;
	case TRKEY_GRIDSAVE:
		// 2016.12.13 KSJ �������� �׸��� ���̺� ���� Ƚ�� ��ŭ ++
		// 			if(m_bUser9111)
		// 				m_nLastCloseCnt++;

		//			removeRTM();
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata);
		break;

	case TRKEY_LASTSET:
		// 2016.12.13 KSJ �������� �׸��� ���̺� ���� Ƚ�� ��ŭ --
		// 			if(m_bUser9111)
		// 				m_nLastCloseCnt--;

		// 2016.12.12 KSJ Ȥ�ó� ���ú긦 �ʰ� ���� ��쿡 �Ϸ�Ǹ� $SAVEINTER�� TRIGGER�� ��������Ѵ�.
		// 2017.02.21 ������ ����������'V' ������ ������ 9111(���δݱ�)-9555(2022�ݱ�)
		if (m_bUser9111 && m_bUser9555)
		{
			Variant(triggerCC, "$SAVEINTER\tOK");
		}

		break;
	case TRKEY_GROUP:
	case TRKEY_INTER:
		receiveOub(trkey->kind, rdata.data);
		break;

	default:
		break;
	}
	return ret;
}

LONG CMainWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	LONG ret = 0;
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
	{
		CRecvData rdata;	
		rdata.SetData(HIBYTE(LOWORD(wParam)), HIWORD(wParam), (char*)lParam);
			
		ret = OperDLLOUB(rdata);
		this->EnableWindow(TRUE);
		SendMode(false);
	}
	break;
	case DLL_OUBx:
	{
		struct	_extTHx* th = (struct _extTHx*)lParam;
		
		CRecvData rdata;
		rdata.SetData(th->size, th->key, th->data);

		ret = OperDLLOUB(rdata);
		this->EnableWindow(TRUE);
		SendMode(false);
	}
	break;
	// 2012.01.19 KSJ RTM ó������� ���� ���˺���
	case DLL_ALERTx:
	{
		if (m_bDestroy)
			break;

		m_bAlertx = TRUE;
		RTS_RecvRTSx(lParam);
	}
	break;
	// KSJ
	case DLL_NOTICE:
		break;
	case DLL_SETPAL:
		SetPallette();
		break;
	case DLL_TRIGGER:
		if (m_bDestroy)
			break;


		parsingTrigger((char *)lParam);
		break;
	case DLL_DOMINO:

		parsingDomino((char *)lParam); //����ȭ�鿡�� 2022���鼭 �׷����� ���� ��
		break;
	case DLL_GUIDE:
		return 1;
		break;
	default:
		break;
	}

	return ret;
}

void CMainWnd::parsingDomino(CString datB)
{
	m_bDominoToolWnd = false;

	CString symbol, entry;

	while (!datB.IsEmpty())
	{
		symbol = IH::Parser(datB, "\t");
		entry = IH::Parser(datB, "\n");

		if (symbol == _T("selectGroup") && !entry.IsEmpty())
		{
			// 2012.01.13 KSJ �ΰ��϶� ȣ������ ����.
			if (m_pToolWnd == nullptr)
			{
				m_bDominoToolWnd = true;
				m_nGroupIndex = atoi(entry);
				return;
			}
			// KSJ

			m_pToolWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELECTGROUP, (LPARAM)atoi(entry)));
		}
	}
}

void CMainWnd::parsingTrigger(CString datB)
{
	CString symbol, strDataB;
	strDataB = datB;
	symbol = IH::Parser(datB, "\t");

	if (symbol == _T("OnPortfolio") && datB == _T("ok"))
	{
		if (!m_bProc)
		{
			// 2016.12.14 KSJ ���������� ������ ������� ������ ������ �ִٸ� refresh ���� �ʴ´�.
			if (m_pGroupWnd->IsNewDrop())
			{
				m_bProc = false;
				return;
			}

			const int ret = (int)m_pToolWnd->SendMessage(WM_MANAGE, MK_SETUPOK);
			m_pTreeWnd->SendMessage(WM_MANAGE, MK_SETUPOK);


			// ������ ���Ѿ��Ѵ�. m_pToolWnd�� m_pTreeWnd�� ���� setting���Ŀ� �װ��� �������� m_pGroupWnd�� ���Ѵ�.	
			m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SETUPOK, ret));
			m_pToolWnd->PostMessage(WM_MANAGE, MK_SENDTREE);
		}
		else
		{
			// 2016.12.21 KSJ ������ ���� ������������ ���������� �д´�.
			m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SETUPOK, 99));
		}
		m_bProc = false;
	}
	else if (symbol == _T("OnCatchChange") && datB == _T("ok")) // ���Ǻ���
	{
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SISECATCH, MO_INIT));
	}
	else if (symbol == _T("Alarm")) // �ش�����
	{
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SISECATCH, MO_CATCH), (LPARAM)(char *)datB.operator LPCTSTR());
	}
	else if (symbol == _T("1301")) // 2012.04.19 KSJ ���������� Ȱ��ȭ �Ǿ��������� Ʈ���Ű� �Ⱥ������� �߰���.
	{
		Variant(triggerCC, strDataB);
	}
}

COLORREF CMainWnd::getAxColor(int color)
{
	if (color & 0x02000000)
		return color;

	return m_pWnd->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

void CMainWnd::sendTR(CString trCode, char *datB, int datL, int key)
{ 
	std::string sendB;
	sendB.resize(L_userTH + datL + 1);
	struct _userTH *uTH{};
	uTH = (struct _userTH *)sendB.c_str();
	strcpy(uTH->trc, trCode);
	uTH->key = key;
	uTH->stat = US_PASS;

	CopyMemory(&sendB[L_userTH], datB, datL);
	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM)sendB.c_str());
	sendB.clear();
}

CString CMainWnd::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	char *dta = (char *)m_pWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = CString(dta);

	return retvalue;
}

void CMainWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
			   //	CMemDC	mdc(&dc);
	DrawDivide(&dc);
}

void CMainWnd::init()
{
	m_home = Variant(homeCC);
	m_user = Variant(nameCC);
	m_id = Variant(userCC);
	m_pBrush = GetAxBrush(RGB(100, 100, 100));
}

void CMainWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if (cx <= 10 || cy <= 10)
	{
		return;
	}
	ResizeOper(cx, cy);
}


CBrush *CMainWnd::GetAxBrush(COLORREF clr)
{
	return (CBrush *)m_pWnd->SendMessage(WM_USER, getBRUSH, (long)clr);
}

void CMainWnd::ResizeOper(int cx, int cy)
{
	if (m_pGroupWnd == nullptr || m_pToolWnd == nullptr || m_pTreeWnd == nullptr || m_pOptDlg == nullptr)
		return;

	int minx = ((m_bTree) ? m_nTree : 0);
	const int miny = SIZE_TOOL + SIZE_GROUPMINY + ((m_bBasket) ? SIZE_BASKETMINY : 0);
	const int ncnt = (int)m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETGROUP, MO_ALL));

	minx += (ncnt * SIZE_GROUPMINX);

	if (cx < minx)
		cx = minx;
	if (cy < miny)
		cy = miny;


	m_size = CSize(cx, cy);
	const CRect rect(0, GAP2, m_size.cx, m_size.cy);

	CRect rcTree(GAP1, SIZE_DIVIDE * GAP2, 0, m_size.cy - 2 * SIZE_DIVIDE); // BUFFET
	CRect rcBasket(0, SIZE_DIVIDE, m_size.cx, m_size.cy - 2 * SIZE_DIVIDE); // BUFFET

	const BOOL bBig = m_pToolWnd->SendMessage(WM_MANAGE, MK_TOOLSIZE);
	CRect rcRTS(0, (bBig) ? SIZE_TOOL + SIZE_DIVIDE * 3 : SIZE_TOOL / 2 + SIZE_DIVIDE * 4, 0, m_size.cy - 2 * SIZE_DIVIDE);
	CRect rcTool((m_bTree) ? SIZE_DIVIDE + GAP1 : GAP1, GAP2, m_size.cx - GAP1, (bBig) ? SIZE_TOOL : SIZE_TOOL / 2 + 3), rcTemp(0, 0, 0, 0);

	m_rcDivide[0] = CRect(0, 0, 0, 0);
	m_rcDivide[1] = CRect(0, 0, 0, 0);

	m_rcPanel[0]  = CRect(0, 0, 0, 0);
	m_rcPanel[1]  = CRect(0, 0, 0, 0);
	m_rcPanel[2]  = CRect(0, 0, 0, 0);

	if (m_bTree)
	{
		m_rcDivide[0].left = rcTree.right = m_nTree;
		m_rcDivide[0].right = m_rcDivide[0].left + SIZE_DIVIDE;
		m_rcDivide[0].top = 0;
		m_rcDivide[0].bottom = m_size.cy;

		// for rounding
		m_rcPanel[0].left = GAP1;
		m_rcPanel[0].right = rcTree.right + SIZE_DIVIDE;
		m_rcPanel[0].top = GAP2;
		m_rcPanel[0].bottom = m_size.cy - 1;
		// for rounding

		// rcBasket.left = rcTool.left = m_rcDivide[0].right + GAP1;
		rcBasket.left = m_rcDivide[0].right + GAP1;
		rcTool.left = m_rcDivide[0].right + SIZE_DIVIDE + GAP1;

		rcTemp = rect;
		rcTemp.left = rcTemp.left + SIZE_DIVIDE; // BUFFET
		rcTemp.right = rcTree.right - SIZE_DIVIDE;
		rcTemp.bottom = m_size.cy - SIZE_DIVIDE;

		if (m_btOption.IsChecked())
		{
			rcTemp.right = rcTree.right;
			rcTemp.top = rcTemp.bottom - m_nOptHeight;
			m_pOptDlg->MoveWindow(rcTemp, TRUE);
			m_pOptDlg->ShowWindow(SW_SHOW);
			rcTemp.bottom = rcTemp.top;
			rcTemp.top = rcTemp.bottom - SIZE_BUTTONS;
		}
		else
		{
			m_pOptDlg->ShowWindow(SW_HIDE);
			rcTemp.right = rcTree.right;
			rcTemp.top = rcTemp.bottom - SIZE_BUTTONS;
		}

		m_btOption.MoveWindow(rcTemp, TRUE);
		rcTree.bottom = rcTemp.top;
		m_btOption.ShowWindow(SW_SHOW);
		m_pTreeWnd->MoveWindow(rcTree, TRUE);
		m_pTreeWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pTreeWnd->ShowWindow(SW_HIDE);
		m_btOption.ShowWindow(SW_HIDE);
		m_pOptDlg->ShowWindow(SW_HIDE);
	}

	if (m_bRTS)
	{
		rcRTS.left = rcTool.left + SIZE_DIVIDE;
		rcRTS.right = rcRTS.left + SIZE_RTS;
		m_rcDivide[1].left = rcRTS.right - 1;
		m_rcDivide[1].right = m_rcDivide[1].left + SIZE_DIVIDE;
		m_rcDivide[1].top = rcTool.bottom;
		m_rcDivide[1].bottom = m_size.cy;

		// END MODIFY
	}

	m_pToolWnd->MoveWindow(rcTool);
	m_pToolWnd->ShowWindow(SW_SHOW);

	// GRID BACKGROUND OUTBOX
	m_rcPanel[2].right = rcTool.right + GAP1;
	m_rcPanel[2].bottom = m_size.cy;

	// GRID OUTBOX
	m_rcPanel[1].left = m_bTree ? rcTree.right + SIZE_DIVIDE * 2 + GAP1 : GAP1;
	m_rcPanel[1].top = rcTool.bottom + SIZE_DIVIDE * 3 / 2; // back
	m_rcPanel[1].right = rcTool.right;
	m_rcPanel[1].bottom = m_size.cy - GAP1;

	rcTool.top = rcTool.bottom + SIZE_DIVIDE * 2; // BUFFET ADD SIZE_DIVIDE

	rcTool.left = m_bTree ? rcTree.right + SIZE_DIVIDE * 8 / 3 : SIZE_DIVIDE * 2 / 3;
	rcTool.left += m_bRTS ? rcRTS.Width() + SIZE_DIVIDE : 0;
	rcTool.bottom = m_size.cy - SIZE_DIVIDE;	   // buffet
	rcTool.right = rcTool.right - SIZE_DIVIDE * 2 / 3; // BUFFET
	m_pGroupWnd->MoveWindow(rcTool);
	m_pGroupWnd->ShowWindow(SW_SHOW);

	Invalidate(false);
}

void CMainWnd::DrawDivide(CDC *pDC)
{
	pDC->FillSolidRect(&m_rcPanel[2], getAxColor(64));

	pDC->FillSolidRect(&m_rcPanel[0], getAxColor(192));
	if (m_bTree)
		DrawRoundBitmap(pDC, m_rcPanel[0], ROUND_PANEL);

	pDC->FillSolidRect(&m_rcPanel[1], getAxColor(192));
	DrawRoundBitmap(pDC, m_rcPanel[1], ROUND_PANEL);
}

LONG CMainWnd::OperTreeVisible(int opt)
{
	LONG ret = 0;

	if (opt) // set visible
	{
		switch (opt)
		{
		case IDC_BT_PREV:
			m_size.cx += (m_nTree + SIZE_DIVIDE);
			m_bTree = TRUE;
			break;
		case IDC_BT_NEXT:
			m_size.cx -= (m_nTree + SIZE_DIVIDE);
			m_bTree = FALSE;
			break;
		}

		m_pWnd->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, nullptr), MAKELPARAM(m_size.cx, m_size.cy));
		// SendMinMaxInfo();//BUFFET BLOCK
	}
	else // get visible state
	{
		ret = m_bTree;
	}

	return ret;
}

LONG CMainWnd::OperBasketVisible(int opt)
{
	LONG ret = 0;

	if (opt) // set visible
	{
		switch (opt)
		{
		case MO_SHOW:
			m_bBasket = TRUE;
			break;
		case MO_HIDE:
			m_bBasket = FALSE;
			break;
		}
		ResizeOper(m_size.cx, m_size.cy);
	}
	else // get visible state
	{
		ret = m_bBasket;
	}

	return ret;
}

//+, - ��ư Ŭ����
/*void CMainWnd::OperSize(int opt, CSize size)
{
	switch (opt)
	{
	case MO_MINUS:
		m_size = m_size - size;
		m_size.cx -= 2;
		break;
	case MO_PLUS:
		m_size = m_size + size;
		break;
	}

	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, nullptr), MAKELPARAM(m_size.cx, m_size.cy));
}*/

void CMainWnd::OperSize(int opt, CSize size)
{
	CSize szTemp = m_size;

	switch (opt)
	{
	case MO_MINUS:
		szTemp = szTemp - size;
		szTemp.cx -= 2;
		break;
	case MO_PLUS:
		szTemp = szTemp + size;
		break;
	}

	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, nullptr), MAKELPARAM(szTemp.cx, szTemp.cy));

	if (m_size.cx != szTemp.cx || m_size.cy != szTemp.cy)
		m_pWnd->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, nullptr), MAKELPARAM(m_size.cx, m_size.cy));

	MoveWindow(0, 0, m_size.cx, m_size.cy);
}

void CMainWnd::OpenScreen(int type, CString data)
{
	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)data.operator LPCTSTR());
}

// 2015.04.08 KSJ ������ �������� �����Ȱ� ������� �ʾ� ������ ��Ŵ.
void CMainWnd::Wait(DWORD dwMillisecond)
{
	MSG msg{};
	DWORD dwStart;
	dwStart = GetTickCount64();

	while (GetTickCount64() - dwStart < dwMillisecond)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

LRESULT CMainWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_ERASEBKGND)
	{
		return 1;
	}
	else if (message == WM_USER + 9557) // 2016.12.16 KSJ ���ο��� ����� ������ �ƴ��� üũ�Ѵ�.
	{
		return m_bChange;
	}
	else if (message == WM_USER + 9555) // 2016.12.13 KSJ ���ο��� Ʈ���� �����ֱ� ��ٸ��� �ִ�.
	{
		m_bUser9555 = TRUE;

		// 2016.12.12 KSJ �Ϸ�Ǹ� $SAVEINTER�� TRIGGER�� ��������Ѵ�.
		//�����ؼ� ������ �������� �����ش�.

		if (m_bChange && m_bUser9111)
		{
			Variant(triggerCC, "$SAVEINTER\tOK");
		}
		else if (!m_bUser9111)
		{
			m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_INTSAVE, 1));
		}

		Sleep(150); // 2016.08.18 KSJ G������ ���Ϲ��� �ð��� ���� ����... �׷��� V�� ������ �ִ�.
		// 2016.12.12 KSJ �Ϸ�Ǹ� $SAVEINTER�� TRIGGER�� ��������Ѵ�.
	}
	else if (message == WM_DESTROY)
	{
		Sleep(150);
	}
	else if (message == WM_USER + 9111) //������ ������.
	{

		m_bUser9111 = TRUE;

		char *wb;

		wb = (char *)lParam;
		sprintf(wb, m_strTag);

		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_INTSAVE, 0));

		Sleep(100); // 2016.08.18 KSJ G������ ���Ϲ��� �ð��� ���� ����... �׷��� V�� ������ �ִ�.

		return 1;
	}
	else if (message == WM_USER + 9333) // LASTSTAT�� �ö�ö�
	{
		if (m_bStartMsg)
		{
			CString strTmp = (char *)lParam;

			strTmp.Replace("USRDEFDATA", "");

			strTmp.TrimLeft();
			strTmp.TrimRight();

			CString strCfg, filepath;
			strCfg.Format("intercfg%s.ini", strTmp);
			filepath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, strCfg);

			CFileFind finder;

			if (finder.FindFile(filepath))
			{
				// CString slog;
				// slog.Format("[IB202200][CMainWnd::WindowProc] LASTSTAT ������ ����� �����̴�..? WM_USER+9333 = [%s] \n", filepath);
				::DeleteFile(filepath);
			}

			return 1;
		}

		m_bStartMsg = TRUE;

		CString str = (char *)lParam;

		int pos = str.Find("\n");

		if (pos >= 0)
		{
			while (pos >= 0)
			{
				CString sToken = SplitString(str, "\n");
				if (sToken.Find("USRDEFDATA") >= 0)
				{
					sToken.Replace("USRDEFDATA", "");

					str = sToken;
					break;
				}

				pos = str.Find("\n");
			}

			if (str.Find("USRDEFDATA") >= 0)
			{
				str.Replace("USRDEFDATA", "");
			}
		}
		else
		{
			str.Replace("USRDEFDATA", "");
		}

		str.TrimLeft();
		str.TrimRight();

		m_strTag = str;
		Variant(titleCC, "��������");
		init();

		if (m_strTag == "LASTSTAT" || m_strTag == "")
		{
			m_strTag = "";

			int nKey = m_pWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), NULL);

			if (nKey < 10)
			{
				nKey = nKey + 1000;
			}

			m_strTag.Format("%d", nKey);

			CString strCfg, filepath;
			strCfg.Format("intercfg%s.ini", m_strTag);
			filepath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, strCfg);

			CString basefile;
			basefile.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, FILE_CONFIG);

			CFileFind finder;

			if (finder.FindFile(basefile))
			{
				::CopyFile(basefile, filepath, FALSE);
			}
		}
		else
		{
			CString strCfg, filepath, filepathNew;
			strCfg.Format("intercfg%s.ini", m_strTag);
			filepath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, strCfg);

			int nKey = atoi(m_strTag);

			if (nKey < 10)
			{
				nKey = nKey + 10000;

				m_strTag = "";
				m_strTag.Format("%d", nKey);

				strCfg.Format("intercfg%s.ini", m_strTag);
				filepathNew.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, strCfg);

				CFileFind finder;

				if (finder.FindFile(filepath))
					::CopyFile(filepath, filepathNew, FALSE);
			}
		}

		loadinfo();
		CreateChild(); // lastsatat
		SetPallette();

		// Invalidate(false);
		ResizeOper(m_size.cx, m_size.cy);

		return 1;
	}
	else if (message == WM_USER + 9444) //�޴��� ��������
	{
		m_bStartMsg = TRUE;

		if (m_strTag == "")
		{
			int nKey = m_pWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), NULL);
			if (nKey < 10)
				nKey = nKey + 1000;
			m_strTag.Format("%d", nKey);
		}

		Variant(titleCC, "��������");
		init();

		CString strCfg, filepath;
		strCfg.Format("intercfg%s.ini", m_strTag);
		filepath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, strCfg);

		// 2011.12.09 KSJ
		CString basefile;
		basefile.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, FILE_CONFIG);
		CFileFind finder;

		if (finder.FindFile(basefile))
			::CopyFile(basefile, filepath, FALSE);

		loadinfo();
		CreateChild(); // menu

		SetPallette();

		// Invalidate(false);
		ResizeOper(m_size.cx, m_size.cy);
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

CString CMainWnd::SplitString(CString &strData, CString strToken)
{
	CString sResult = strData;
	if (strToken == "")
		return sResult;
	const int iFind = strData.Find(strToken);
	if (iFind > -1)
	{
		sResult = strData.Left(iFind);
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength());
	}
	return sResult;
}

void CMainWnd::SelectGroup(int kind)
{
	if (m_pTreeWnd)
		m_pTreeWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)kind);

	if (m_pToolWnd)
		m_pToolWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)kind);
}

void CMainWnd::SendOper(class CSendData *sdata)
{
	SendMode(true);
	sendTR(sdata->GetTRCODE(), sdata->GetData(), sdata->GetDataLen(), sdata->GetDataKey());
}

BOOL CMainWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(wParam) == IDC_BT_CONFIG)
	{
		ResizeOper(m_size.cx, m_size.cy);
	}
	return CWnd::OnCommand(wParam, lParam);
}

DWORD CMainWnd::SearchOption(WORD kind, DWORD option)
{
	DWORD ret = 0;
	switch (kind)
	{
	case MO_SET:
		m_pOptDlg->SetOption((int)option);
		break;
	case MO_GET:
		ret = m_pOptDlg->GetOption();
		break;
	case MO_ADD:
		m_pOptDlg->AddOption((int)option);
		break;
	case MO_DEL:
		m_pOptDlg->DelOption((int)option);
		break;
	}
	return ret;
}

void CMainWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_rcDivide[0].PtInRect(point))
	{
		SetCapture();
		m_nResize = MO_LEFTRIGHT;
		m_xPosMove = m_xPos = point;
		CRect rect = m_rcDIVIDE = m_rcDivide[0];
		rect.left = SIZE_TREEMINX;
		rect.right = rect.left + 200;
		ClientToScreen(rect);
		ClipCursor(rect);
		DrawMove(m_rcDIVIDE);
	}
	else if (m_rcDivide[1].PtInRect(point))
	{
		SetCapture();
		m_nResize = MO_TOPBOTTOM;
		m_xPosMove = m_xPos = point;
		CRect rect = m_rcDIVIDE = m_rcDivide[2];
		rect.bottom = m_size.cy - SIZE_BASKETMINY;
		rect.top = rect.bottom - 300;
		ClientToScreen(rect);
		ClipCursor(rect);
		DrawMove(m_rcDIVIDE);
	}

	// Invalidate(false);

	CWnd::OnLButtonDown(nFlags, point);
}

void CMainWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	ClipCursor(nullptr);

	if ((m_nResize == MO_LEFTRIGHT && point.x != m_xPos.x) || (m_nResize == MO_TOPBOTTOM && point.y != m_xPos.y))
	{
		switch (m_nResize)
		{
		case MO_LEFTRIGHT:
			m_nTree += (point.x - m_xPos.x);
			break;
		case MO_TOPBOTTOM:
			m_nBasket -= (point.y - m_xPos.y);
			break;
		}

		ResizeOper(m_size.cx, m_size.cy);
	}

	DrawMove(m_rcDIVIDE);
	m_nResize = MO_NONE;
	m_rcDIVIDE.SetRectEmpty();
	m_xPosMove = m_xPos = CPoint(-1, -1);
	// InvalidateDivide();
	Invalidate(false); // BUFFET
	CWnd::OnLButtonUp(nFlags, point);
}

void CMainWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_nResize != MO_NONE)
	{
		DrawMove(m_rcDIVIDE);

		switch (m_nResize)
		{
		case MO_LEFTRIGHT:
			m_rcDIVIDE.OffsetRect(point.x - m_xPosMove.x, 0);
			m_xPosMove.x = point.x;
			break;
		case MO_TOPBOTTOM:
			m_rcDIVIDE.OffsetRect(0, point.y - m_xPosMove.y);
			m_xPosMove.y = point.y;
			break;
		}

		DrawMove(m_rcDIVIDE);

		// Invalidate(false);
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CMainWnd::DrawMove(CRect rect)
{
	if (m_pBrush)
	{
		CWindowDC dc(this);
		const int nMode = dc.SetROP2(R2_XORPEN);
		CBrush* oldbrush = dc.SelectObject(m_pBrush);
		dc.Rectangle(rect);
		dc.SelectObject(oldbrush);
		dc.SetROP2(nMode);
	}
}

BOOL CMainWnd::OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message)
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	if (m_rcDivide[0].PtInRect(pt))
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_LR));
		return TRUE;
	}
	else if (m_rcDivide[2].PtInRect(pt))
	{
		//		MessageBox("IDC_CURSOR_TB");
		//		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_TB));
		return TRUE;
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CMainWnd::InvalidateDivide()
{
	CRect rect = m_rcDivide[0] + CRect(0, 0, 1, 0);
	InvalidateRect(rect, FALSE);
	InvalidateRect(m_rcDivide[2], FALSE);
}

void CMainWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_rcDivide[0].PtInRect(point))
	{
		m_nTree = SIZE_TREE;
		ResizeOper(m_size.cx, m_size.cy);
	}
	else if (m_rcDivide[2].PtInRect(point))
	{
		m_nBasket = SIZE_BASKET;
		ResizeOper(m_size.cx, m_size.cy);
	}
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CMainWnd::SendMode(bool bSend /* = true */)
{
	if (bSend)
	{
		Enable(MO_ALLWND, !bSend);
		KillTimer(1000);
		SetTimer(1000, 10 * 1000, nullptr);
	}
	else
	{
		Enable(MO_ALLWND, !bSend);
	}
}

void CMainWnd::Enable(int kind, bool bEnable /* = true */)
{
	if (kind & MO_MAIN)
		EnableWindow(bEnable);
	if (kind & MO_TOOL)
		if (m_pToolWnd)
			m_pToolWnd->EnableWindow(bEnable);
	if (kind & MO_GROUP)
		if (m_pGroupWnd)
			m_pGroupWnd->EnableWindow(bEnable);
	if (kind & MO_TREE)
	{
		if (m_pTreeWnd)
			m_pTreeWnd->EnableWindow(bEnable);

		if (m_pOptDlg)
			m_pOptDlg->EnableWindow(bEnable);

		if (m_btOption.m_hWnd)
			m_btOption.EnableWindow(bEnable);
	}
}

void CMainWnd::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == 1000)
	{
		KillTimer(1000);
		SendMode(false);
	}
	else if (nIDEvent == 8787)
	{
		KillTimer(8787);

		if (m_bStartMsg != TRUE)
		{
			m_bStartMsg = TRUE;

			if (m_strTag == "")
			{
				const int nKey = m_pWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), NULL);
				m_strTag.Format("%d", nKey);
			}

			Variant(titleCC, "��������");
			init();
			loadinfo();
			CreateChild(); // ontimer
			SetPallette();
		}
		Invalidate(false);

		// 2012.01.13 KSJ ��ȭ�鿡�� ���������� ���� ���ٰ� �������� �ʾҴµ� ���� ���� ���� �׷��� ���� ��������� �����ϰ�
		//�����Ǹ� �޼��� �����D.
		if (m_bDominoToolWnd && m_pToolWnd != nullptr)
			m_pToolWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELECTGROUP, (LPARAM)m_nGroupIndex));
	}
	else if (nIDEvent == TM_RTSTIME) // 20200324
	{
		if (m_pGroupWnd == nullptr)
			return;

		if (_mRealtime.size() == 0)
			return;

		for_each(_mRealtime.begin(), _mRealtime.end(), [&](auto code){		
			const auto&it = _mapRealData.find(code.first);	
			if (it != _mapRealData.end())
				m_pGroupWnd->RecvRTSx((LPARAM)it->second.get());
		});		
		_mRealtime.clear();
	}
	CWnd::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
//���ð� ��ȸ
void CMainWnd::RequestMarketTime()
{
	CSendData sdata;
	char key{};
	_trkey *trkey = (struct _trkey *)&key;
	trkey->kind = TRKEY_MARKETTIME;
	sdata.SetData("bo221199", key, "1", 1, "");

	SendOper(&sdata);
}

void CMainWnd::uploadBackup()
{
	int sendL = 0;
	std::string sendB;
	sendB.resize(16 * 1024);
	char tempB[32]{};
	//	char	strUinfo[500];

	struct _uinfo *uinfo = (struct _uinfo *)&sendB[sendL];
	sendL += sz_uinfo;

	FillMemory((char *)uinfo, sz_uinfo, ' ');

	CopyMemory(uinfo->gubn, "MY", sizeof(uinfo->gubn));
	uinfo->dirt[0] = 'U';
	uinfo->cont[0] = 'b';

	sprintf(tempB, "00000");
	CopyMemory(uinfo->nblc, tempB, sizeof(uinfo->nblc));

	uinfo->retc[0] = 'O';

	sendB[sendL] = 0x00;

	//	SendTR
	CSendData sdata;
	char key{};
	_trkey *trkey = (struct _trkey *)&key;

	trkey->kind = TRKEY_BACKUP;

	sdata.SetData(trUPDOWN, key, sendB.data(), strlen(sendB.c_str()), "");

	SendOper(&sdata);
}

struct MarketTime
{
	char row1[30]; //�����ð���
	char row2[30]; //����ȣ��
	char row3[30]; //�峻�ð�
	char row4[30]; //����ȣ��
	char row5[30]; //���Ľð���
	char row6[30]; //�ð��� ���ϰ�
};

void CMainWnd::SetMarketTime(char *datB) // 2012.11.26 KSJ ������ ��ȸ
{
	struct MarketTime *mkTime = (struct MarketTime *)datB;

	CString strRow1, strRow2, strRow3, strRow4, strRow5, strRow6;
	CString strTemp;

	strRow1 = CString(mkTime->row1, 30); //�����ð���
	strRow2 = CString(mkTime->row2, 30); //����ȣ��
	strRow3 = CString(mkTime->row3, 30); //�峻�ð�
	strRow4 = CString(mkTime->row4, 30); //����ȣ��
	strRow5 = CString(mkTime->row5, 30); //���Ľð���
	strRow6 = CString(mkTime->row6, 30); //�ð��� ���ϰ�

	m_strMarketEndTime = strRow4;
	m_strMarketTime = strRow2;
}

void CMainWnd::RTS_RecvRTSx(LPARAM lParam)
{
	if (m_pGroupWnd == nullptr)
		return;

	CString slog;
	CString code;
	CString sgubn;
	struct _alertR *alertR = (struct _alertR *)lParam;
	DWORD *data = nullptr;
	char *pdata = nullptr;
	
	data = (DWORD *)alertR->ptr[0];
	code = alertR->code;
	code.Trim();



	COleDateTime oTime;
	oTime = COleDateTime::GetCurrentTime();
	CString strCurTime;
	strCurTime.Format(_T("%02d%02d%02d"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());

	//ȣ���� ������ return
	bool bCheckTime = false;
	sgubn.Format("%s", (char*)data[0]);

	if (atoi(m_strBeginTime) <= atoi(strCurTime) && atoi(m_strBeginTimeEnd) >= atoi(strCurTime)) //��������ȣ�� �ð��̸�
		bCheckTime = true;

	if (atoi(m_strEndTime) <= atoi(strCurTime) /*&& atoi(m_strEndTimeEnd) + 1000 >= atoi(strCurTime)*/) //�帶������ȣ�� �ð��̸�
		bCheckTime = true;

	if(bCheckTime)	//����ȣ�� �ð��̸� ����ó��
	{
		m_pGroupWnd->RecvRTSx(lParam, 1);
		return;
	}

	if (!m_pGroupWnd->isCodeSymbol(code))
		return;

	auto& rmap = m_pGroupWnd->getRSymbol();
	constexpr int arr[9] = { 41, 51, 61, 71, 101, 104, 106, 107, 109 };

	bool bHoga = false;
	for (const auto symbol : arr)
	{
		const auto ft = rmap.find(symbol);
		if (ft != rmap.end())
			bHoga = true;
	}
	
	if (bHoga == false && sgubn.FindOneOf("DLy") != -1)
		return;

	const auto mt = _mapRealData.emplace(code, std::make_unique<struct _Ralert>());

	mt.first->second->size = alertR->size;
	mt.first->second->stat = alertR->stat;
	mt.first->second->code = code;

	mt.first->second->ptr[111].reset();
	for (auto item : rmap)
	{
		const int ii = item.first;
		pdata = (char*)data[ii];

		if (pdata == nullptr)
		{
			continue;
		}
		const int len = strlen(pdata) + 1;	
		mt.first->second->ptr[ii].reset(std::make_unique<char[]>(len).release());
		ZeroMemory(mt.first->second->ptr[ii].get(), len);
		strncpy((char*)mt.first->second->ptr[ii].get(),(char*)pdata, len - 1);
	}
	_mRealtime.emplace(std::make_pair(code, 1));	

}

CString CMainWnd::GetDataTitle(int nKind)
{
	CString strTitle = (char *)m_pTreeWnd->SendMessage(WM_MANAGE, MK_GETDATATITLE, nKind);
	return strTitle;
}

BOOL CMainWnd::IsValidCode(CString strCode)
{
	if (m_pGroupWnd)
		return m_pGroupWnd->IsValidCode(strCode);

	return FALSE;
}

void CMainWnd::DrawRoundBitmap(CDC *pDC, CRect rc, int nKind)
{
	CRect bitRc;
	switch (nKind)
	{
	case ROUND_PANEL:
		// left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lt.bmp");

		// right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rt.bmp");

		// left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lb.bmp");

		// right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rb.bmp");
		break;
	case ROUND_CONTENTS:
		// left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lt.bmp");

		// right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rt.bmp");

		// left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lb.bmp");

		// right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rb.bmp");
		break;
	}
}

void CMainWnd::DrawBitmap(CDC *pDC, CRect rc, CString strImage)
{
	CString strPath = Variant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;

	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CDC memDC;

	memDC.CreateCompatibleDC(pDC);

	if (memDC.m_hDC != nullptr)
	{
		memDC.SelectObject(BitMap);
		::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);
	}
	DeleteObject(BitMap);
	memDC.DeleteDC();
}

CString CMainWnd::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	file.Format("%s\\%s\\%s", m_home, "tab", "palette.ini");

	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;

	return strPal;
}

CString CMainWnd::CheckIP()
{
	char szHostName[64] = {0};
	CString m_ip;
	CString tmps, ipAddr;

	::gethostname(szHostName, sizeof(szHostName));

	if (lstrcmp(szHostName, "") != 0)
	{
		HOSTENT FAR *lphostent = ::gethostbyname(szHostName);

		ipAddr = "";
		for (int ii = 0; lphostent; ii++)
		{
			if (!lphostent->h_addr_list[ii])
				break;
			sprintf(szHostName, "%u.%u.%u.%u",
				0xff & lphostent->h_addr_list[ii][0],
				0xff & lphostent->h_addr_list[ii][1],
				0xff & lphostent->h_addr_list[ii][2],
				0xff & lphostent->h_addr_list[ii][3]);
			ipAddr = szHostName;
		}
	}
	return ipAddr;
}

unsigned int CMainWnd::IPToUInt(CString ip)
{
	int a{}, b{}, c{}, d{};
	unsigned int addr = 0;

	if (sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d) != 4)
		return 0;

	addr = a << 24;
	addr |= b << 16;
	addr |= c << 8;
	addr |= d;
	return addr;
}

bool CMainWnd::isIPInRange(CString ip, CString network_s, CString network_e)
{
	const unsigned int ip_addr   = IPToUInt(ip);
	const unsigned int net_lower = IPToUInt(network_s);
	const unsigned int net_upper = IPToUInt(network_e);

	if (ip_addr >= net_lower &&
	    ip_addr <= net_upper)
		return true;

	return false;
}

void CMainWnd::Request_GroupList()
{
	int	sendL = 0;
	std::string sendB;
	sendB.resize(16 * 1024);
	const int isz_updn = sz_uinfo;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;

	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'g';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'O';
	sendTR(trUPDOWN, sendB.data(), sendL, TRKEY_GROUP);
}

void CMainWnd::Request_GroupCode(int iseq)
{
	const int index = iseq;
	int sendL = 0;
	CString stmp;
	std::string sendB;
	sendB.resize(16 * 1024);

	const int isz_updn = sz_uinfo + 2;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;
	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'j';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'O';
	stmp.Format("%d", index);
	memcpy((char*)&sendB[sz_uinfo], stmp, stmp.GetLength());
	sendTR(trUPDOWN, sendB.data(), sendL, TRKEY_INTER);
}

void CMainWnd::receiveOub(int key, CString data)
{
	struct  _glistex {
		char    ngrs[2];           /* New Group Seqn       */
		char    ogrs[2];           /* Old Group Seqn       */
		char	gnam[30];
	};

	if (key == TRKEY_GROUP)
	{
		const int cnt = atoi(data.Left(4));

//XMSG(�׷찳��);
//AxStd::_Msg("�׷� �Ѱ���[%d]",  cnt);
		data = data.Mid(4);
		data.Trim();
		if (!data.IsEmpty() && cnt > 0)
		{
			std::vector<std::pair<CString, CString>> vGroupName;
			const gsl::span<_glistex> groupList((_glistex*)data.GetString(), cnt);

			std::sort(groupList.begin(), groupList.end(), [](auto item, auto item1) {
				return CString(item.ngrs, 2).Trim().Compare((CString(item1.ngrs, 2).Trim())) < 0;
			});

			for_each(groupList.begin(), groupList.end(), [&](auto item) {
				const CString groupKey = CString(item.ngrs, 2).Trim();
				const CString groupName = CString(item.gnam, 30).Trim();
				//AxStd::_Msg("�׷�Ű[%s]", groupKey);
				//AxStd::_Msg("�׷��[%s]", groupName);
				vGroupName.emplace_back(std::make_pair(groupName, groupKey));
			});

			if (m_pTreeWnd)
				m_pTreeWnd->makeInterGroup(vGroupName);
			if (m_pToolWnd)
				m_pToolWnd->addGroup(vGroupName);

			if (m_pGroupWnd)
			{
				auto& map = m_pGroupWnd->_GroupName;
				for_each(vGroupName.begin(), vGroupName.end(), [&](const auto item) {
					map.emplace(std::make_pair(atoi(item.second), item.first));
					});

				m_pGroupWnd->SelectOper();
				if (!_bInit)
				{
					m_pGroupWnd->InitSetGroup();
					_bInit = true;
				}
			}
		}
	}
	else
	{	
		if (m_pTreeWnd)
			m_pTreeWnd->receiveOub(data, key);
	}
}