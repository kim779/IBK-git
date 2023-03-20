// GridWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB202200.h"
#include "GridWnd.h"
#include "intgrid.h"
#include "../../h/grid.h"
#include "GroupWnd.h"
#include "MainWnd.h"
#include "UsefulSetupDlg.h"
#include "MoveSetupDlg.h"     // ADD PSH 20070918
#include "AutoSaveSetupDlg.h" // ADD PSH 20070918
#include "AlertSoundDlg.h"    // ADD PSH 20070918
#include "PopupDlg.h"
#include "CodeDuplicate.h"
#include "InterDomino.h"
#include "mmsystem.h"
#include <math.h>
#include "resource.h"

#define GRIDMAXNUM 31
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef CWnd *(*_DLL_SetLibOpen)(CWnd *pwndMain, HINSTANCE hInstance, CWnd *pWndModuleWnd);
typedef void (*_DLL_SetLibClose)(CWnd *pWndTarget);
typedef int (*_DLL_SetSignal)(BOOL bAdvise, CWnd *pwndMain, CString strType, CString sPBID, CString strKey);

_DLL_SetLibOpen SignalLib_Open;
_DLL_SetLibClose SignalLib_Close;
_DLL_SetSignal SignalLib_SetSignal;

/////////////////////////////////////////////////////////////////////////////
// CGridWnd

CGridWnd::CGridWnd(CWnd *pMainWnd, int nIndex) : CBaseWnd(pMainWnd)
{
	SetIndex(nIndex);

	m_grid = nullptr;
	m_kind = 0;
	m_szTitle = _T("");
	m_updateROW = -1;
	m_endsort = false;
	m_posField = false;
	m_ccField = false;
	m_drag = m_drop = -1;
	m_nTurn = 0;
	m_seldrop = -1;
	m_bongField = -1;
	m_sonikField = -1;
	m_suikField = -1;

	m_bTicker = FALSE;
	m_bfirstStart = NULL;

	m_bMargin = FALSE;
	m_bSorting = FALSE;
	m_mapMargin.RemoveAll();
	m_clrMarkerBKG = RGB(240, 240, 240);
	m_clrMarkerTXT = RGB(142, 142, 142);

	m_bShadow = TRUE;
	m_bAllApply = TRUE;
	m_bMoveCfg = FALSE;
	m_nMoveSave = 0;
	m_pMsgWnd = nullptr;
	m_bInfo = FALSE;
	m_bNewsNChart = TRUE; // 2011.12.02 KSJ
	m_bPlusMinus = false; // 2011.12.05 KSJ

	m_bAddCnd = FALSE;
	m_bChgCnd = FALSE;
	m_bAutoSaveAllApply = FALSE;
	m_nRowHeight = 0;
	m_bWaveApply = FALSE;

	m_clrKospi = RGB(0, 0, 0);
	m_clrKosdaq = RGB(128, 64, 64);
	m_bMKClrApply = FALSE;

	m_bExpect = FALSE;
	m_bAutoExpect = FALSE;
	m_bPopupDlg = FALSE;

	m_bRTS = FALSE;
	m_bDispCode = FALSE;

	m_nCodeWidth = 80;

	m_nNewsNChartWidth = 20; // 2011.12.02 KSJ

	m_pRCDlg = nullptr;
	m_bMonitor = FALSE;
	m_bMonSym = FALSE;
	m_bEditWork = FALSE;

	m_bIsFirst = FALSE;

	m_dBase = 0;
	m_ClickRow = 0;

	for (int i = 0; i < IARRAYSIZE; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			m_iYDayVol[i][j] = "";
		}
	}

	for (int i = 0; i < IARRAYSIZE; i++)
	{
		m_irowCode[i] = 0;
	}

	m_yDayVolField = -1; //���ϰŷ�������ʵ�
	m_EqualizerField = -1;

	m_recomDataField = -1;
	m_recomBasicCostField = -1;
	m_recomSuikrateField = -1;
	m_recomBMjisuField = -1;
	m_recomBMSuikrateField = -1;
	m_recomgoalCostField = -1;
	m_recomJisuGubnField = -1;
	m_upjongCode = -1;
	m_beforeVolField = -1;
	m_newCurrField = -1;
	m_epbDataField = -1; // 2016.04.15 KSJ epb ��õ����

	m_nGridWidth = 0; //�׸��� ����
	// END ADD
	for (int cc = 0; cc < 200; cc++)
	{
		chg_count[cc] = 0;
	}

	m_dKosdaq = 0;
	m_dKospi = 0;

	m_iStartPoint = 0;
	m_iEndPoint = 0;
	m_iSendcount = -1;
	m_staticUpdate = -1;

	m_bContinue = FALSE;
	m_bSecond = FALSE;

	m_pSearchMap.RemoveAll();
	m_mapCurValue.RemoveAll();

	m_strBeginTime = "084000";
	m_strEndTime = "085959";
	m_nCount = 0;
	m_nBookMarkRow = 0;

	m_arEpbDate.RemoveAll(); // 2016.04.15 KSJ
	m_epbDataField = -1;	 // 2016.04.15 KSJ

	m_bLastSend = TRUE;

	CString path;
	path.Format("%s/%s/%s", m_root, DEVDIR, "lib_signalmng.dll");
	m_hSignalMng = ::LoadLibrary(path);

	if (m_hSignalMng)
	{
		SignalLib_SetSignal = (_DLL_SetSignal)GetProcAddress(m_hSignalMng, "_DLL_SetSignal");
	}

	m_bSkipRTS = false;

	m_arrSymbol.Add("3051");
	m_arrSymbol.Add("3071");
	m_arrSymbol.Add("3041");
	m_arrSymbol.Add("3061");
	m_arrSymbol.Add("3101");
	m_arrSymbol.Add("3106");
	m_arrSymbol.Add("3104");
	m_arrSymbol.Add("3109");
}

CGridWnd::~CGridWnd()
{
	ClearInterest();
	if (m_hSignalMng)
	{
		FreeLibrary(m_hSignalMng);
	}
	m_arrSymbol.RemoveAll();
}

BEGIN_MESSAGE_MAP(CGridWnd, CBaseWnd)
//{{AFX_MSG_MAP(CGridWnd)
ON_WM_LBUTTONDOWN()
ON_WM_TIMER()
ON_WM_RBUTTONUP() // ADD PSH 20070912
ON_WM_SIZING()
//}}AFX_MSG_MAP
ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridWnd message handlers

HBITMAP CGridWnd::getBitmap(CString path)
{
	CBitmap *pBitmap = (CBitmap *)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETBITMAP, (LPARAM)(char *)path.operator LPCTSTR());
	return pBitmap->operator HBITMAP();
}

void CGridWnd::OperInit()
{
	m_pView = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
	m_pGroupWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
	m_pToolWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	m_pTreeWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));

	HBITMAP hBITMAP{}, hBITMAP_DN{}, hBITMAP_HV{};

	// Ʈ�� ��ġ�� ��ư
	CString fileIMG;
	fileIMG.Format("%s\\%s\\", m_root, IMAGEDIR);

	LOGFONT lf;
	m_pFont->GetLogFont(&lf);
	struct _fontR fontR;
	fontR.bold = FW_BOLD;
	fontR.italic = FALSE;
	fontR.point = ((CGroupWnd *)m_pGroupWnd)->GetFontSize();
	fontR.name = lf.lfFaceName;
	m_pFontB = GetAxFont(&fontR);
	lf.lfHeight = fontR.point * 10;

	m_rowC = 2;
	// END MDOIFY
	m_colC = 8;
	m_clrFOCUS[0] = RGB(255, 255, 128);
	m_clrFOCUS[1] = RGB(255, 255, 255);
	m_clrTEXT[0] = RGB(100, 100, 100);
	m_clrTEXT[1] = RGB(100, 100, 100);
	m_size.cy = ((CGroupWnd *)m_pGroupWnd)->GetRowHeight();
	loadcfg();

	m_grid = std::make_unique<CintGrid>(m_pMainWnd, this, &lf);
	m_grid->Create(CRect(0, 0, 0, 0), this, IDC_GRID, GVSC_BOTH, GVDD_FULL);
	m_grid->m_iTime = m_iTime;

	// ADD PSH 20070911
	m_grid->SetBmpIcon(fileIMG + "MARKER.bmp", 0);
	m_grid->SetBmpIcon(fileIMG + "��.bmp", 1);
	m_grid->SetBmpIcon(fileIMG + "��������.bmp", 2);
	m_grid->SetBmpIcon(fileIMG + "��.bmp", 3);
	m_grid->SetBmpIcon(fileIMG + "��.bmp", 4);
	m_grid->SetBmpIcon(fileIMG + "��2.bmp", 5);
	m_grid->SetBmpIcon(fileIMG + "��õ����.BMP", 6);

	m_grid->SetBmpIcon(fileIMG + "ȯ.BMP", 7);
	m_grid->SetBmpIcon(fileIMG + "�̻�޵�.BMP", 8);
	m_grid->SetBmpIcon(fileIMG + "��.BMP", 9);
	m_grid->SetBmpIcon(fileIMG + "��2.BMP", 10); //����
	m_grid->SetBmpIcon(fileIMG + "��1.BMP", 11); //����
	m_grid->SetBmpIcon(fileIMG + "��.BMP", 12);  //���
	m_grid->SetBmpIcon(fileIMG + "��.BMP", 13);  //����
	m_grid->SetBmpIcon(fileIMG + "��.BMP", 14);  // 2012.10.31 KSJ �ܱ���� �߰�
	m_grid->SetBmpIcon(fileIMG + "��1.BMP", 15); // 2013.03.18 KSJ ���迹��
	m_grid->SetBmpIcon(fileIMG + "��.BMP", 16);  // 2013.03.18 KSJ �Ǹ�
	m_grid->SetBmpIcon(fileIMG + "��1.BMP", 17); // 2013.03.18 KSJ ���
	m_grid->SetBmpIcon(fileIMG + "��.BMP", 18);  // 2013.03.18 KSJ �׸�
	m_grid->SetBmpIcon(fileIMG + "��.BMP", 19);  // 2013.03.18 KSJ ����
	m_grid->SetBmpIcon(fileIMG + "��.BMP", 20);  // 2013.03.18 KSJ �պ�
	m_grid->SetBmpIcon(fileIMG + "��1.BMP", 21); // 2013.03.18 KSJ ����
	m_grid->SetBmpIcon(fileIMG + "��.BMP", 22);  // 2013.03.18 KSJ ����
	m_grid->SetBmpIcon(fileIMG + "��.BMP", 23);  // 2016.06.14 KSJ ����
	m_grid->SetBmpIcon(fileIMG + "��.BMP", 24);  // 2017.03.08 KDK �޵�

	m_bMoveCfg = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVECFG);
	m_nMoveSave = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVESAVE);

	InitPopupMenu();

	m_btReload.Create("\n�׷� �ٽ� �ҷ�����", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_RELOAD);
	m_btDomino.Create("\n�������簡2 ���񿬵�", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_DOMINO);
	m_btSAVE.Create("\n������������", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_SAVE);
	m_btCLOSE.Create("\n�׷��ʱ�ȭ", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_ERASE);

	hBITMAP = getBitmap(fileIMG + "btn_Refresh.bmp");
	hBITMAP_DN = getBitmap(fileIMG + "btn_Refresh_DN.bmp");
	hBITMAP_HV = getBitmap(fileIMG + "btn_Refresh_EN.bmp");

	m_btReload.SetFont(m_pFont, false);
	m_btReload.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

	hBITMAP = getBitmap(fileIMG + "GTI_MATRIX2.bmp");
	hBITMAP_DN = getBitmap(fileIMG + "GTI_MATRIX2_dn.bmp");
	hBITMAP_HV = getBitmap(fileIMG + "GTI_MATRIX2_en.bmp");

	m_btDomino.SetFont(m_pFont, false);
	m_btDomino.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

	hBITMAP = getBitmap(fileIMG + "SAVEINT.bmp");
	hBITMAP_DN = getBitmap(fileIMG + "SAVEINT_dn.bmp");
	hBITMAP_HV = getBitmap(fileIMG + "SAVEINT_en.bmp");

	m_btSAVE.SetFont(m_pFont, false);

	m_btSAVE.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

	hBITMAP = getBitmap(fileIMG + "�ݱ�1.bmp");
	hBITMAP_DN = getBitmap(fileIMG + "�ݱ�1_dn.bmp");
	hBITMAP_HV = getBitmap(fileIMG + "�ݱ�1_en.bmp");

	m_btCLOSE.SetFont(m_pFont, false);
	m_btCLOSE.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

	RemoveAll();

	// 2016.04.11 KSJ ó���������� Ʈ���޴����� �ʵ�¾�
	if (CAST_TREEID(m_kind)->kind == xEPBCODE)
		FieldSetup(false, 4, true);
	else
		FieldSetup(false);

	if (m_hSignalMng)
	{
		CString strRealType;
		strRealType.Format("%d", (int)TYPE_SIGNAL_ALERT);
		SignalLib_SetSignal(true, this, strRealType, "", "8");
	}

	m_btReload.Invalidate(TRUE);
	m_btDomino.Invalidate(TRUE);
	m_btSAVE.Invalidate(TRUE);
	m_btCLOSE.Invalidate(TRUE);
}

CString CGridWnd::CalMaketTime(CString strTime, bool bEnd)
{
	CString strData;

	strTime.Replace("��", "");
	strTime.Replace("��", "");

	CString strTemp;

	if (bEnd)
	{
		if (strTime.IsEmpty())
			strTime = "90";

		int nTime = atoi(strTime) - 1;

		if (nTime % 10 == 9)
			nTime -= 4;

		strData.Format("%03d959", nTime); // 085959, 092959, 095959
	}
	else
	{
		if (strTime.IsEmpty())
			strTime = "80";

		strData.Format("%03d000", atoi(strTime) + 4); // 081000, 084000, 09100
	}

	return strData;
}

// 2013.04.30 bResize SendGrid���� ȣ��ɶ��� ������¡ ���� �ʴ´�.
void CGridWnd::FieldSetup(bool bDlgSetup, int iEqualizer, bool bResize)
{
	const int nIndex = (int)m_pToolWnd->SendMessage(WM_MANAGE, MK_GETAFIELD);
	m_bongField = -1;
	m_sonikField = -1;
	m_suikField = -1;
	m_yDayVolField = -1;
	m_recomDataField = -1;
	m_recomBasicCostField = -1;
	m_recomSuikrateField = -1;
	m_recomBMjisuField = -1;
	m_recomBMSuikrateField = -1;
	m_recomgoalCostField = -1;
	m_recomJisuGubnField = -1;
	m_upjongCode = -1;
	m_EqualizerField = -1;
	m_beforeVolField = -1;
	m_newCurrField = -1;
	m_epbDataField = -1; // 2016.04.15 KSJ ebp ��õ����

	m_ccField = false;
	m_posField = false;


	m_grid->Clear();


	loadField(nIndex, bDlgSetup, iEqualizer);

	initRSymbol();
	initialGrid(bResize);

	SetGridinfo(TRUE);

	// ADD PSH 20070918
	MarkerSetup();
	//	m_grid->MakeEqualizerBmp();
	// END ADD
}

void CGridWnd::OperDestory()
{
	savecfg();
	m_btReload.DestroyWindow();
	m_btDomino.DestroyWindow();
	m_btSAVE.DestroyWindow();
	m_btCLOSE.DestroyWindow();
	ClearInterest();

	//	m_grid->DestroyWindow();

	KillTimer(1000);
}

void CGridWnd::DrawTitle(CDC *pDC)
{
	const int nCurSel = m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_GET));
	const BOOL bFocus = (nCurSel == m_nIndex) ? TRUE : FALSE;
	const int nmode = pDC->SetBkMode(TRANSPARENT);
	IH::DrawBox(pDC, m_rcTitle, RGB(200, 200, 200), BOX_ALL, (bFocus) ? m_clrFOCUS[0] : m_clrFOCUS[1], TRUE);
	const COLORREF clr = pDC->SetTextColor((bFocus) ? m_clrTEXT[0] : m_clrTEXT[1]);
	CFont *oldfont = nullptr;
	if (bFocus)
		oldfont = pDC->SelectObject(m_pFontB);

	if (IsInterest())
	{
		pDC->SetTextColor(RGB(255, 0, 0));
	}
	CRect rect = m_rcTitle;
	rect.OffsetRect(0, 1);
	pDC->DrawText(m_szTitle, rect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
	pDC->SetTextColor(clr);

	if (oldfont)
		pDC->SelectObject(oldfont);
	pDC->SetBkMode(nmode);
}

void CGridWnd::OperDraw(CDC *pDC)
{
	DrawTitle(pDC);
}

void CGridWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SendMessage(WM_MANAGE, MK_CLICKGROUP);
	CBaseWnd::OnLButtonDown(nFlags, point);
}

// ADD PSH 20070912
void CGridWnd::OnRButtonUp(UINT nFlags, CPoint point)
{

	if (m_rcTitle.PtInRect(point))
	{
		ShowPopupMenu();
	}

	CBaseWnd::OnRButtonUp(nFlags, point);
}
// END ADD

void CGridWnd::OperResize(int cx, int cy)
{
	CRect rect, rcButton;
	rect = m_rcTitle = m_rect;
	m_rcTitle.bottom = m_rcTitle.top + 24; //@@Zeta m_size.cy + 1;
	rcButton = m_rcTitle;

	rcButton.right -= 1;
	rcButton.top += 2;
	rcButton.left = rcButton.right - SIZE_BUTTONS - 1;
	rcButton.bottom = rcButton.top + SIZE_BUTTONS;
	m_btCLOSE.MoveWindow(rcButton, TRUE);
	rcButton.OffsetRect(-SIZE_BUTTONS - 2, 0);
	m_btSAVE.MoveWindow(rcButton, TRUE);
	rcButton.OffsetRect(-SIZE_BUTTONS - 2, 0);
	m_btDomino.MoveWindow(rcButton, TRUE);
	rcButton.OffsetRect(-SIZE_BUTTONS - 2, 0);
	m_btReload.MoveWindow(rcButton, TRUE);

	rect.top = m_rcTitle.bottom;

	m_grid->MoveWindow(rect, TRUE);
	m_grid->Adjust();

	if (IsInit())
	{
		m_grid->ShowWindow(SW_SHOW);
		m_btReload.ShowWindow(true);
		m_btDomino.ShowWindow(true);
		m_btSAVE.ShowWindow(true);
		m_btCLOSE.ShowWindow(true);
		m_btReload.Invalidate(TRUE);
		m_btDomino.Invalidate(TRUE);
		m_btSAVE.Invalidate(TRUE);
		m_btCLOSE.Invalidate(TRUE);
	}

	InvalidateRect(m_rcTitle);
	InvalidateRect(rect);

	m_btReload.ShowWindow(SW_SHOW);
	m_btDomino.ShowWindow(SW_SHOW);
	m_btSAVE.ShowWindow(SW_SHOW);
	m_btCLOSE.ShowWindow(SW_SHOW);

	m_btReload.Invalidate(TRUE);
	m_btDomino.Invalidate(TRUE);
	m_btSAVE.Invalidate(TRUE);
	m_btCLOSE.Invalidate(TRUE);
}

void CGridWnd::InsertRowNCode(int row)
{
	if (row <= 0)
		return;

	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	int rowcount = 0;

	if (nOver == MO_VISIBLE)
	{
		pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
		rowcount = ((CGroupWnd *)pWnd)->sumEachGroupCount();
	}
	else
	{
		rowcount = GetRowValidcount();
	}

	if (rowcount >= MAX_LINE)
	{
		Variant(guideCC, IH::idTOstring(IDS_GUIDE4));

		return;
	}

	insertInterest(row - 1);
	insertRow(row);
	SetLineColor();
	m_grid->memoCheck();
	m_grid->memoRefresh();
	m_grid->SetFocusCellEdit(row, colNAME, true);

	if (m_bAddCnd)
		saveInterest();
}

LONG CGridWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG ret = 0;
	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int retArrange = pWnd->SendMessage(WM_MANAGE, MK_GETARRANGE);

	// DWORD* data;
	CString strTemp;

	switch (LOWORD(wParam))
	{
	case MK_SISECATCH:
		OperAlarm(HIWORD(wParam), (char *)lParam);
		break;
	case MK_MEMO:
		m_grid->memoCheck();
		Invalidate(FALSE);
		break;
	case MK_DELETEMEMO:
		break;
	case MK_NEWS:
		queryNewsCode();
		break;
	case MK_GETDATAKIND:
		ret = GetKind();
		break;
	case MK_CHART:
		ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_CHART);
		break;
	case MK_TURNCODE:
	{
		const int nsec = HIWORD(wParam);
		if (nsec == 0)
		{
			KillTimer(1000);
			m_nTurn = 0;
		}
		else
		{
			if (CAST_TREEID(m_kind)->kind == xISSUE)
				break;

			m_nTurn = 0;
			SetTimer(1000, nsec * 1000, nullptr);
		}
	}
	break;
	case MK_EXCEL:
		m_grid->RunExcel();
		break;
	case MK_HIDECODE:
		m_grid->HideCode();
		break;
	case MK_SELGROUP:
		InvalidateRect(m_rcTitle, FALSE);
		break;
	case MK_CLICKGROUP:
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)m_nIndex);
		break;
	case MK_TREEDATA:
		ParseData((class CGridData *)lParam);
		break;
	case MK_UPJONGDATA:
		ParseUpjongData((class CGridData *)lParam);
		break;
	case MK_REMAINDATA:
		ParseRemainData((class CGridData *)lParam);
		break;
	case MK_SENDTR:
		sendTransaction();
		break;
	case MK_RECVDATA:
		RecvOper(HIWORD(wParam), (CRecvData *)lParam);
		break;
	case MK_RTSDATA:
		// 		RecvRTS((char*)lParam);
		break;
		// 2012.01.19 KSJ Alertx �߰�
	case MK_RTSDATAx:
		RecvRTSx(lParam);
		break;
		// 2012.01.19 KSJ Alertx �߰� ��
	case MK_ENDDRAG:
		m_drag = m_drop = -1;
		m_grid->FreeDragDrop();
		CintGrid::m_dropdata.Reset();
		break;
	case MK_INSERTROW:
		ret = insertRow(HIWORD(wParam));
		break;
	case MK_INSERTCODE:
		if (retArrange == 0)
		{

			if (CAST_TREEID(m_kind)->kind == xINTEREST)
				m_bEditWork = true;

			InsertRowNCode(HIWORD(wParam));
		}

		break;
	case MK_DELETEROW:
		DeleteRow(HIWORD(wParam));
		insertInterest(MAX_LINE - 1);
		SetLineColor();

		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_bEditWork = true;

		break;
	case MK_TRIGGEROW:
		triggerCode(HIWORD(wParam));
		break;
	case MK_SAVE:
	{
		if (GVNM_ENDDRAG == lParam)
		{
			if (!m_bSorting)
			{
				saveInterest();
			}
			else
			{
				if (m_bMoveCfg && (1 == m_nMoveSave))
				{
					RemoveAllBaseInters();
					m_grid->SortBase();
					saveInterest();
				}
				else if (!m_bMoveCfg)
				{
					m_bMoveCfg = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVECFG);
					m_nMoveSave = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVESAVE);

					// 091013 ����
					CMoveSetupDlg dlg;
					dlg.m_nOption = m_nMoveSave;
					dlg.m_bAlert = m_bMoveCfg;

					// dlg �׻� ���� �ɼ� üũ
					if (dlg.m_bAlert)
					{
						if (1 == dlg.m_nOption)
						{
							RemoveAllBaseInters();
							m_grid->SortBase();
							saveInterest();
							m_bEditWork = false;
						}
						else
						{
							if (CAST_TREEID(m_kind)->kind == xINTEREST)
								m_bEditWork = true;
						}
					}
					else
					{
						if (dlg.DoModal())
						{
							if (1 == dlg.m_nOption)
							{
								RemoveAllBaseInters();
								m_grid->SortBase();
								saveInterest();
								m_bEditWork = false;
							}
							else
							{
								if (CAST_TREEID(m_kind)->kind == xINTEREST)
									m_bEditWork = true;
							}
						}
					}

					m_pToolWnd->SendMessage(WM_MANAGE, MK_SETMOVECFG, dlg.m_bAlert);
					m_pToolWnd->SendMessage(WM_MANAGE, MK_SETMOVESAVE, dlg.m_nOption);
				}
			}
		}
		else
		{
			saveInterest();
		}
	}
	// END MODIFY
	break;
	case MK_HAWKEYE:
	{
		const LONG val = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
		hawkeyeAction((val) ? true : false);
		//			Invalidate(FALSE);
	}
	break;
	// ADD PSH 20070911
	case MK_HSCROLL:
	{
	}
	break;
	case MK_SORTBASE:
	{
		if (!m_bNews)
			BaseSorting();
	}
	break;
	// END ADD
	default:
		ret = CBaseWnd::OnManage(wParam, lParam);
		break;
	}
	return ret;
}

void CGridWnd::triggerCode(int row)
{
	m_ClickRow = row;
	CString code = m_grid->GetItemText(row, colCODE);
	CString string;

	code.TrimLeft();
	code.TrimRight();

	if (code.IsEmpty())
		return;

	if (((CGroupWnd *)m_pGroupWnd)->GetSelAction() == 0)
		return;

	switch (GetCodeType(code))
	{
	case kospiCODE:
		string.Format("%s\t%s", HCOD, code);
		break;
	case futureCODE:
		string.Format("%s\t%s", "ed_focod", code); // 2013.01.03 KSJ 2200��������ȭ�鿡�� Ʈ���ź���
		Variant(triggerCC, string);

		string.Format("%s\t%s", FCOD, code);
		break;
	case optionCODE:
		string.Format("%s\t%s", "ed_focod", code); // 2013.01.03 KSJ 2200��������ȭ�鿡�� Ʈ���ź���
		Variant(triggerCC, string);

		string.Format("%s\t%s", OCOD, code);
		break;
	case foptionCODE:
		string.Format("%s\t%s", "ed_focod", code); // 2013.01.03 KSJ 2200��������ȭ�鿡�� Ʈ���ź���
		Variant(triggerCC, string);

		string.Format("%s\t%s", PCOD, code);
		break;
	case indexCODE:
		string.Format("%s\t%s", UCOD, code);
		break;
	case sinjuCODE:
		string.Format("%s\t%s", SINJUCOD, code);
		break;
	case elwCODE:
		string.Format("%s\t%s", HCOD, code);
		Variant(triggerCC, string);
		//					m_grid->SetFocus();
		string.Format("%s\t%s", ELWCOD, code);
		break;
	case thirdCODE:
		string.Format("%s\t%s", THIRDCOD, code);
		break;
	case 0:							  // 2012.10.11 KSJ �ֽ�, �ݸ�, ��ȭ���� �߰�
		if (code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //�ֽ�, �ݸ�, ��ȭ����   //�Ļ���ǰ �ڵ尳��
		{
			int nGubn = atoi(code.Mid(1, 2));
			CString strGubn = code.Mid(1, 2); // 2014.10.21 KSJ �ֽļ��� �ű����� ����

			if (nGubn > 10 && nGubn < 60) //�ֽļ���
			{
				string.Format("%s\t%s", "ed_fcod", code); // 2013.01.02 KSJ 2300��������ȭ�鿡�� Ʈ���ź���
				Variant(triggerCC, string);

				string.Format("%s\t%s", SFOD, code);
				break;
			}
			else if (nGubn > 60 && nGubn < 70) //�ݸ�����
			{
				string.Format("%s\t%s", SFOD, code);
				break;
			}
			else if (nGubn > 70 && nGubn < 80) //��ȭ����
			{
				string.Format("%s\t%s", SFOD, code);
				break;
			}
			else if (strGubn >= "B0" && strGubn <= "ZZ") // 2014.10.21 KSJ �ֽļ��� �ű����� ����
			{
				string.Format("%s\t%s", "ed_fcod", code); // 2013.01.02 KSJ 2300��������ȭ�鿡�� Ʈ���ź���
				Variant(triggerCC, string);

				string.Format("%s\t%s", SFOD, code);
				break;
			}
		}
	}
	Variant(triggerCC, string);
	Variant(codeCC, string);

	m_grid->SetFocus();
}

void CGridWnd::RecvOper(int kind, CRecvData *rdata)
{
	struct _trkey *trkey = (struct _trkey *)&kind;
	if (trkey->group != (UINT)m_nIndex)
		return;

	switch (trkey->kind)
	{
	case TRKEY_GRIDUPJONG:
	case TRKEY_GRIDNEW:
		parsingOubs(rdata->data, rdata->size);
		// Map �ʱ�ȭ KSI
		ClearSearchMap();
		ReSetSearchMap(); // 2011.12.29 KSJ
		break;
	case TRKEY_GRIDNEWS:
		parsingNewsCode(rdata);
		break;
	case TRKEY_GRIDROW:
		parsingOubsOne(rdata->data, rdata->size, m_updateROW);
		// Map �ʱ�ȭ KSI
		ClearSearchMap();
		ReSetSearchMap(); // 2011.12.29 KSJ
		break;
	case TRKEY_GRIDSAVE:
		uploadOK(); // 2016.07.11 KSJ �׷�����(G)�� ������ ������������(V) ������.
		break;
	case TRKEY_RECOMMAND:
		parseReCommandData(rdata->data, rdata->size);
		break;
	case TRKEY_LASTSET: // 2016.07.11 KSJ 'V' ����.
		break;
	}
}

void CGridWnd::AddData_Overoper(int nIndex, CString sztmp)
{
	BOOL bInt = FALSE;
	CString code, amount, price, bookmark;

	// ADD PSH 20070913
	CString strName;

	if (0 == nIndex)
	{
		const UINT attr = m_grid->GetItemAttr(nIndex + 1, colNAME) & ~GVAT_MARKER;
		m_grid->SetItemAttr(nIndex + 1, colNAME, attr);
	}
	else
	{
		insertRow(nIndex + 1);
	}
	// END ADD

	code = IH::Parser(sztmp, PTAB);
	code.Trim();

	if (CAST_TREEID(m_kind)->kind == xINTEREST)
	{
		bInt     = TRUE;
		amount   = IH::Parser(sztmp, PTAB);
		price    = IH::Parser(sztmp, PTAB);
		strName  = IH::Parser(sztmp, PTAB);
		bookmark = IH::Parser(sztmp, PTAB);
	}

	const int count = _vInters.size();
	if (!bInt && code.IsEmpty() && (count > nIndex))
		return;

	auto& pinters = _vInters.at(nIndex);
	pinters->gubn = '0';
	pinters->code = code;

	if (bInt)
	{
		pinters->xnum = amount;
		pinters->xprc = price;
		pinters->bookmark = bookmark[0];
	}

	// ADD PSH 20070914
	if (!code.IsEmpty() && code.GetAt(0) == 'm')
	{
		pinters->gubn = 'm';
		pinters->name = strName;

		const UINT attr = m_grid->GetItemAttr(nIndex + 1, colNAME) | GVAT_MARKER;
		m_grid->SetItemAttr(nIndex + 1, colNAME, attr);
		m_grid->SetItemText(nIndex + 1, colNAME, strName);

		return;
	}
	// END ADD
}

void CGridWnd::AddData(int nIndex, CString sztmp)
{
	BOOL bInt = FALSE;
	CString code, amount, price, bookmark;

	// ADD PSH 20070913
	CString strName;

	if (0 == nIndex)
	{
		const UINT attr = m_grid->GetItemAttr(nIndex, colNAME & ~GVAT_MARKER);
		m_grid->SetItemAttr(nIndex, colNAME, attr);
	}
	else
	{
		insertRow(nIndex);
	}
	// END ADD

	code = IH::Parser(sztmp, PTAB);

	code.TrimLeft();
	code.TrimRight();

	if (CAST_TREEID(m_kind)->kind == xINTEREST)
	{
		bInt = TRUE;
		amount = IH::Parser(sztmp, PTAB);
		price = IH::Parser(sztmp, PTAB);
		// ADD PSH 20070914
		strName = IH::Parser(sztmp, PTAB);
		bookmark = IH::Parser(sztmp, PTAB);
		// END ADD
	}
	const int count = _vInters.size();
	if (!bInt && code.IsEmpty() && (count > nIndex))
		return;

	auto& pinters = _vInters.at(nIndex);
	pinters->gubn = '0';
	pinters->code = code;

	if (bInt)
	{
		pinters->xnum = amount;
		pinters->xprc = price;
		pinters->name = strName;
		pinters->bookmark = bookmark[0];
	}

	// ADD PSH 20070914
	if (!code.IsEmpty() && code.GetAt(0) == 'm')
	{
		pinters->gubn = 'm';
		pinters->name = strName;
		const UINT attr = m_grid->GetItemAttr(nIndex + 1, colNAME);
		m_grid->SetItemAttr(nIndex + 1, colNAME, attr | GVAT_MARKER);
		m_grid->SetItemText(nIndex + 1, colNAME, strName);

		return;
	}
	else
	{
		m_grid->SetItemAttr(nIndex + 1, colNAME, m_grid->GetItemAttr(nIndex + 1, colNAME) & ~GVAT_MARKER);
	}
	// END ADD
}

void CGridWnd::RemoveAll()
{
	ClearInterest();

	for (int ii = 0; ii < MAX_LINE; ii++)
	{
		auto& pinters = _vInters.emplace_back(std::make_shared<struct _intersx>());
		pinters->gubn = '0';
	}

	// ADD PSH 20070917
	int nRow = m_grid->GetRowCount() - 1;
	for (; nRow > 1; nRow--)
	{
		m_grid->DeleteRow(nRow);
		m_grid->memoDelete(nRow);
	}

	for (int i = 0; i < m_colC; i++)
	{
		m_grid->SetItemText(1, i, "");
		m_grid->SetItemData(1, i, 0);
	}


	m_rowC = 2; // BUFFET 2->1
	// END ADD

	// 2012.03.22 KSJ �����׷��� �ٸ� ȭ�鿡�� ��ﶧ ù��° �ٿ��� �ǽð� �����͸� �޾���.
	ClearSearchMap(); // 2011.12.29 KSJ
	ReSetSearchMap(); // 2011.12.29 KSJ
}

void CGridWnd::ParseRemainData(class CGridData *sdata)
{
	RemoveAll();

	SetKind(sdata->GetKind());
	InvalidateRect(m_rcTitle, FALSE);
	ClearInterest();

	CString sztmp = _T("");
	CString code, amount, price, name, strBookmark;
	char bookmark[1];
	bookmark[0] = '0';

	const int ncnt = sdata->GetCount();
	BOOL bInt = FALSE;

	if (CAST_TREEID(m_kind)->kind == xREMAIN)
		bInt = TRUE;

	int ii = 0;
	for (ii = 0; ii < ncnt; ii++)
	{
		sztmp = sdata->m_arDatas.GetAt(ii);
		code = IH::Parser(sztmp, PTAB);
		code.Trim();


		if (!bInt && code.IsEmpty())
			continue;

		auto& pinters = _vInters.emplace_back(std::move(std::make_shared<_intersx>()));
	//	pinters = new _intersx();
	//	ZeroMemory(pinters, sz_inters);

		pinters->gubn = '0';
		pinters->code = code;

		if (pinters->code[0] == 'm')
		{
			pinters->gubn = 'm';
		}

		if (bInt || !sztmp.IsEmpty())
		{
			amount = IH::Parser(sztmp, PTAB);
			price = IH::Parser(sztmp, PTAB);
			name = IH::Parser(sztmp, PTAB);
			strBookmark = IH::Parser(sztmp, PTAB);

			if (strBookmark.Compare("1")) // 2015.04.03 KSJ 1�̾ƴϸ� 0���� ���ش�.
			{
				strBookmark = "0";
			}

			// END ADD
			pinters->xnum = amount;
			pinters->xprc = price;
			pinters->name = name;
			//�ϸ�ũ �߰�
			pinters->bookmark = strBookmark[0];
			// END ADD
		}

		if (ii != 0)
		{
			insertRow(ii, FALSE);
		}
	}

	for (; ii < MAX_LINE; ii++)
	{
		auto& pinters = _vInters.emplace_back(std::move(std::make_shared<_intersx>()));
		pinters->gubn = '0';
		//AddInters(pinters);
	}

	// ADD PSH 20070918
	MarkerSetup();
	// END ADD
}

void CGridWnd::parseReCommandData(char *datB, int datL)
{
//	struct mod
//	{
//		char nrec[4];	     /*	����			*/
//		char acod[1024][12]; /*  �����ڵ� ����Ʈ */
//	};
//
//#define sz_mod sizeof(struct mod)
//
//	struct mod *mod = nullptr;
//	mod = (struct mod *)new char[sz_mod]{};
//	ZeroMemory(mod, sz_mod);
//
//	class CGridData sdata;
//
//	mod = (struct mod *)datB;
//
//	CString strCnt = CString(mod->nrec, 4);
//	const int nCnt = atoi(strCnt);
//
//	CString code, name;
//
//	for (int i = 0; i < nCnt; i++)
//	{
//		code = CString(mod->acod[i], 12);
//		code.TrimLeft();
//		code.TrimRight();
//
//		if (!code.IsEmpty())
//		{
//			sdata.m_arDatas.Add(code);
//		}
//	}
//
//	ParseData(&sdata);
//	sendTransaction();
}

void CGridWnd::ParseUpjongData(class CGridData *sdata)
{
	RemoveAll();
	ClearInterest();

	CString sztmp = _T("");
	CString code;

	const int ncnt = sdata->GetCount();
	const BOOL bInt = FALSE;
	int ii = 0;
	for (ii = 0; ii < ncnt; ii++)
	{
		sztmp = sdata->m_arDatas.GetAt(ii);
		code = IH::Parser(sztmp, PTAB).Trim();

		if (code.IsEmpty())
			continue;

		auto& pinters = _vInters.emplace_back(std::move(std::make_shared<_intersx>()));
		pinters->gubn = '0';
		pinters->code = code;
		if (ii != 0)
			insertRow(ii, FALSE);
	}

	for (; ii < MAX_LINE; ii++)
	{
		auto& pinters = _vInters.emplace_back(std::move(std::make_shared<_intersx>()));
		pinters->gubn = '0';
	}
	sendTransaction(200);
}

void CGridWnd::ParseData(class CGridData *sdata)
{
	RemoveAll();
	SetKind(sdata->GetKind());
	InvalidateRect(m_rcTitle, FALSE);
	ClearInterest();
	m_arEpbDate.RemoveAll(); // 2016.04.15 KSJ ���⼭�� �����ϸ� �ȴ�.


	CString sztmp = _T("");
	CString code, amount, price, name, strBookmark, futureGubn, creditPrc, maeipPrc, date = _T("");
	char bookmark[1];
	bookmark[0] = '0';

	const int ncnt = sdata->GetCount();
	BOOL bInt = FALSE;

	if (CAST_TREEID(m_kind)->kind == xINTEREST)
		bInt = TRUE;


	int ii = 0;
	for (ii = 0; ii < ncnt; ii++)
	{
		sztmp = sdata->m_arDatas.GetAt(ii);
		code = IH::Parser(sztmp, PTAB);

		code.TrimLeft();
		code.TrimRight();

		if (!bInt && code.IsEmpty())
			continue;

		auto& pinters = _vInters.emplace_back(std::move(std::make_shared<_intersx>()));
		pinters->gubn = '0';
		pinters->code = code;
		if (pinters->code[0] == 'm')
		{
			pinters->gubn = 'm';
		}

		if (bInt || !sztmp.IsEmpty())
		{
			amount = IH::Parser(sztmp, PTAB);
			price = IH::Parser(sztmp, PTAB);
			name = IH::Parser(sztmp, PTAB);
			strBookmark = IH::Parser(sztmp, PTAB);
			futureGubn = IH::Parser(sztmp, PTAB);
			creditPrc = IH::Parser(sztmp, PTAB);
			maeipPrc = IH::Parser(sztmp, PTAB);
			date = IH::Parser(sztmp, PTAB); // 2016.04.15 KSJ ��õ����

			// END ADD
			pinters->xnum = amount;
			pinters->xprc = price;
			pinters->name = name;
			pinters->futureGubn = futureGubn[0];
			pinters->creditPrc = atof(creditPrc);
			pinters->maeipPrc = atof(maeipPrc);

			if (strBookmark.CompareNoCase("1") != 0) // 2015.04.03 KSJ 1�̾ƴϸ� 0���� ���ش�.
			{
				strBookmark = "0";
			}

 			pinters->bookmark = strBookmark[0];
		}

		m_arEpbDate.Add(date); // 2016.04.15 KSJ ebp ��õ����

		if (ii != 0)
			insertRow(ii, FALSE);
	}

	for (; ii < MAX_LINE; ii++)
	{
		auto& pinters = _vInters.emplace_back(std::move(std::make_shared<_intersx>()));
		pinters->gubn = '0';
	}

	_arrBaseInters.clear();
	rebuildInterest(); // 2013.05.21 KSJ ���⼭ m_inter�� baseinter�� ���� ������ش�.
	// ADD PSH 20070918

	MarkerSetup();

	// END ADD
}

int CGridWnd::loadcfg_data()
{
	const int result = GetPrivateProfileInt(m_section, KEY_DATA, 0, m_fileCFG);

	return result;
}

void CGridWnd::savecfg_data(CString keydata)
{
	WritePrivateProfileString(m_section, KEY_DATA, keydata.operator LPCTSTR(), m_fileCFG);
}

void CGridWnd::loadcfg()
{
	const int confirm_start = GetPrivateProfileInt(m_section, KEY_DATA, 9999, m_fileCFG);

	if (confirm_start == 9999)
	{
		m_bfirstStart = TRUE;
	}
	else
	{
		m_bfirstStart = FALSE;
	}

	// data kind
	m_kind = GetPrivateProfileInt(m_section, KEY_DATA, 134316051, m_fileCFG);

	if (CAST_TREEID(m_kind)->kind == xREMAIN)
		m_kind = 0;

	char buf[1024];

	memset(buf, 0x00, sizeof(buf));
	GetPrivateProfileString(m_section, KEY_FIELD, "", buf, sizeof(buf), m_fileCFG);
	m_szFIELD.Format("%s", buf);

	memset(buf, 0x00, sizeof(buf));
	GetPrivateProfileString(m_section, KEY_WIDTH, "", buf, sizeof(buf), m_fileCFG);
	m_szWIDTH.Format("%s", buf);

	// 2013.07.12 KSJ ,0,0,0,0�� �������� ����� �����Ѵ�.
	// 7852~7855���� �߰��Ѱ� �߸� �ݿ��ߴٰ� ���� ��¿�� ���� �ڵ�
	if (m_szWIDTH.Find(",0,0,0,0"))
	{
		m_szWIDTH.Replace(",0,0,0,0", "");
		WritePrivateProfileString(m_section, KEY_WIDTH, m_szWIDTH, m_fileCFG);
	}
	// KSJ

	// ADD PSH 20070912
	memset(buf, 0x00, sizeof(buf));
	GetPrivateProfileString(SEC_MAIN, KEY_MARGIN, "", buf, sizeof(buf), m_fileCFG);

	if (m_bfirstStart == TRUE)
	{
		m_bMargin = 0;
	}
	else
	{
		m_bMargin = atoi(buf);
	}

	DWORD dwRes;

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "BKGCLR", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_clrMarkerBKG = RGB(240, 240, 240);
	}
	else
	{
		m_clrMarkerBKG = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TXTCLR", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_clrMarkerTXT = RGB(142, 142, 142);
	}
	else
	{
		m_clrMarkerTXT = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TXTSHD", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_bShadow = TRUE;
	}
	else
	{
		m_bShadow = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "ALLAPPL", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_bAllApply = TRUE;
	}
	else
	{
		m_bAllApply = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(SEC_MAIN, KEY_SYMINFO, "", buf, sizeof(buf), m_fileCFG);

	if (m_bfirstStart == TRUE)
	{
		m_bInfo = 1;
	}
	else
	{
		if (0 == dwRes)
		{
			m_bInfo = FALSE;
		}
		else
		{
			m_bInfo = atoi(buf);
			dwRes = 0;
		}
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKAPPL", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_bApply = FALSE;
	}
	else
	{
		m_tkConfig.m_bApply = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKVOL", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_vol = 0;
	}
	else
	{
		m_tkConfig.m_vol = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKAMT", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_amt = 0;
	}
	else
	{
		m_tkConfig.m_amt = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKAND", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_and = FALSE;
	}
	else
	{
		m_tkConfig.m_and = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKPRC", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_price = FALSE;
	}
	else
	{
		m_tkConfig.m_price = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKSPRC", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_sprc = 0;
	}
	else
	{
		m_tkConfig.m_sprc = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKEPRC", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_eprc = 0;
	}
	else
	{
		m_tkConfig.m_eprc = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKULIM", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_ulimit = TRUE;
	}
	else
	{
		m_tkConfig.m_ulimit = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKUP", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_up = TRUE;
	}
	else
	{
		m_tkConfig.m_up = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKFLAT", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_flat = TRUE;
	}
	else
	{
		m_tkConfig.m_flat = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKDLIM", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_dlimit = TRUE;
	}
	else
	{
		m_tkConfig.m_dlimit = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKDOWN", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_down = TRUE;
	}
	else
	{
		m_tkConfig.m_down = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "ALLAUTO", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_bAutoSaveAllApply = FALSE;
	}
	else
	{
		m_bAutoSaveAllApply = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "ADDCND", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_bAddCnd = FALSE;
	}
	else
	{
		m_bAddCnd = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "CHGCND", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_bChgCnd = FALSE;
	}
	else
	{
		m_bChgCnd = (BOOL)atoi(buf);
		dwRes = 0;
	}

	// 2011.12.02 KSJ
	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "NEWSNCHARTWIDTH", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_nNewsNChartWidth = 20;
	}
	else
	{
		m_nNewsNChartWidth = atoi(buf);
		dwRes = 0;
	}
	// KSJ

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "CODEWIDTH", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_nCodeWidth = 80;
	}
	else
	{
		m_nCodeWidth = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "GRIDWIDTH", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_nGridWidth = 0;
	}
	else
	{
		m_nGridWidth = atoi(buf);
		dwRes = 0;
	}

	//ó�� �ѹ��� ����������
	dwRes = GetPrivateProfileString(m_section, "ISFIRSTGRIDSETINFO", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		SetGridinfo();
	}

	// END ADD
}

void CGridWnd::savecfg()
{
	CString slog;
	// data kind
	CString str;

	if (CAST_TREEID(m_kind)->kind == xREMAIN)
		m_kind = 0;

	str.Format("%d", m_kind);
	WritePrivateProfileString(m_section, KEY_DATA, str.operator LPCTSTR(), m_fileCFG);

	int nWidth = 0;
	const int ncnt = m_gridHdrX.GetSize();
	struct _gridHdr grdHdr
	{
	};
	CString sztmp, szWIDTH = _T(""), szFIELD = _T("");

	for (int ii = 0; ii < ncnt; ii++)
	{
		grdHdr = m_gridHdrX.GetAt(ii);
		nWidth = m_grid->GetColumnWidth(ii);
		CString strSymbol = (CString)grdHdr.symbol;

		//��õ�����ʵ�� �������� �ʴ´�
		if (strSymbol.Find("191") > -1)
			continue;

		if (strSymbol.Find("2022") > -1)
			continue;

		if (ii >= colNAME)
		{
			sztmp.Format("%d,", nWidth);
			szWIDTH += sztmp;
		}
		else if (ii == colCODE && 0 < nWidth)
		{
			m_nCodeWidth = nWidth;
		}

		// 2011.12.02 KSJ
		else if (ii == colSIG && 0 < nWidth)
		{
			m_nNewsNChartWidth = nWidth;
		}
		// KSJ

		if (ii > colCURR)
		{
			sztmp.Format("%s,", grdHdr.symbol);
			if (sztmp.Compare("2111,") == 0)
				sztmp.Format("2023,");
			else if (sztmp.Compare("2115,") == 0)
				sztmp.Format("2024,");
			else if (sztmp.Compare("2116,") == 0)
				sztmp.Format("2033,");
			else if (sztmp.Compare("2112,") == 0)
				sztmp.Format("2027,");

			if (grdHdr.needs == 1)
			{
				if (sztmp.Compare("2029,") == 0)
				{
					sztmp = "#1##,";
				}
				else if (sztmp.Compare("2030,") == 0)
				{
					sztmp = "#2##,";
				}
				else if (sztmp.Compare("2031,") == 0)
				{
					sztmp = "#3##,";
				}
			}
			else if (grdHdr.needs == 3)
			{
				if (sztmp.Compare("2029,") == 0)
				{
					sztmp = "#4##,";
				}
				else if (sztmp.Compare("2030,") == 0)
				{
					sztmp = "#5##,";
				}
				else if (sztmp.Compare("2031,") == 0)
				{
					sztmp = "#6##,";
				}
			}

			szFIELD += sztmp;
		}
	}

	WritePrivateProfileString(m_section, KEY_FIELD, szFIELD.operator LPCTSTR(), m_fileCFG);
	WritePrivateProfileString(m_section, KEY_WIDTH, szWIDTH.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_clrMarkerBKG);
	WritePrivateProfileString(m_section, "BKGCLR", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_clrMarkerTXT);
	WritePrivateProfileString(m_section, "TXTCLR", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bShadow);
	WritePrivateProfileString(m_section, "TXTSHD", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bAllApply);
	WritePrivateProfileString(m_section, "ALLAPPL", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_tkConfig.m_bApply);
	WritePrivateProfileString(m_section, "TKAPPL", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_vol); // ü�ᷮ ����   (vol * 1,000)
	WritePrivateProfileString(m_section, "TKVOL", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_amt); // ü��ݾ� ���� (amt * 10,000,000)
	WritePrivateProfileString(m_section, "TKAMT", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_and); // and / or
	WritePrivateProfileString(m_section, "TKAND", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_price); // ���ݼ��� flag
	WritePrivateProfileString(m_section, "TKPRC", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_sprc); // ���ݼ��� ���۰�
	WritePrivateProfileString(m_section, "TKSPRC", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_eprc); // ���ݼ��� ����
	WritePrivateProfileString(m_section, "TKEPRC", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_ulimit); // ����
	WritePrivateProfileString(m_section, "TKULIM", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_up); // ���
	WritePrivateProfileString(m_section, "TKUP", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_flat); // ����
	WritePrivateProfileString(m_section, "TKFLAT", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_dlimit); // ����
	WritePrivateProfileString(m_section, "TKDLIM", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_down); // �϶�
	WritePrivateProfileString(m_section, "TKDOWN", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bAutoSaveAllApply);
	WritePrivateProfileString(m_section, "ALLAUTO", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bAddCnd);
	WritePrivateProfileString(m_section, "ADDCND", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bChgCnd);
	WritePrivateProfileString(m_section, "CHGCND", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_nCodeWidth);
	WritePrivateProfileString(m_section, "CODEWIDTH", sztmp.operator LPCTSTR(), m_fileCFG);
	// END ADD

	// 2011.12.02 KSJ
	sztmp.Format("%d", m_nNewsNChartWidth);
	WritePrivateProfileString(m_section, "NEWSNCHARTWIDTH", sztmp.operator LPCTSTR(), m_fileCFG);
	// KSJ

	//����ũ�� ����
	CRect rc;
	GetWindowRect(rc);
	sztmp.Format("%d", rc.Width());
	WritePrivateProfileString(m_section, "GRIDWIDTH", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("1"); // �϶�
	WritePrivateProfileString(m_section, "ISFIRSTGRIDSETINFO", sztmp.operator LPCTSTR(), m_fileCFG);
}

BOOL CGridWnd::IsInterest()
{
	if (m_kind == 99999)
		return TRUE;

	struct _treeID *treeID = (struct _treeID *)&m_kind;
	if (treeID->kind == xINTEREST)
		return TRUE;
	return FALSE;
}

void CGridWnd::sendTransactionTR(int update, int nStart, int nEnd)
{
	int sendL = 0;
	char tempB[64]{};
	const int bufSize = sizeof(struct _gridHi) + 50 + m_gridHdrX.GetSize() * 5 + _vInters.size() * 12 + 12;
	std::string sendB;
	sendB.resize(bufSize);
	FillMemory(sendB.data(), bufSize, ' ');

	SetInitRecommandInfo();

	const BOOL bExpect = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_EXPECT);
	m_bAutoExpect = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_AUTOEXPECT);

	if (bExpect)
	{
		sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 1, P_TAB);
	}
	else
	{
		if (m_bAutoExpect)
		{
			sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 1, P_TAB);
		}
		else
		{
			sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 0, P_TAB);
		}
	}

	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "%s%c", gSYMBOL, P_DELI);
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	struct _gridHi *gridHi;
	gridHi = (struct _gridHi *)&sendB[sendL];
	sendL += sizeof(struct _gridHi);

	CopyMemory(gridHi->visible, "99", sizeof(gridHi->visible));
	sprintf(tempB, "%04d", _vInters.size());
	CopyMemory(gridHi->rows, tempB, sizeof(gridHi->rows));

	gridHi->type = '0';
	gridHi->dir = '1';
	gridHi->sort = '0';

	for (int ii = nStart; ii < nEnd; ii++)
	{
		strcpy(tempB, (LPTSTR)(LPCTSTR)m_pGridArray.GetAt(ii));
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}

	sprintf(tempB, "2321%c", P_NEW); //���ϰŷ���
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "2950%c", P_NEW); // 2012.06.19 KSJ �������
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "2951%c", P_NEW); // 2012.06.19 KSJ ��������
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "%s%c", CCOD, P_DELI); // �����ڵ�
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	CString tempStr;
	for_each(_vInters.begin(), _vInters.end(), [&](auto& pinters) {
		sprintf(tempB, "%s%c", pinters->code.IsEmpty() == TRUE ? " " : pinters->code, P_DELI);
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	});

	sendB[sendL] = P_NEW;
	sendL += 1;
	sendB[sendL] = P_TAB;
	sendL += 1;
	sendB[sendL] = 0x00;

	CSendData sdata;
	char key{};
	_trkey *trkey = (struct _trkey *)&key;

	trkey->group = m_nIndex;

	if (m_staticUpdate < 0)
	{
		trkey->kind = TRKEY_GRIDNEW;
	}
	else if (m_staticUpdate >= 0 && m_staticUpdate < 200)
	{
		trkey->kind = TRKEY_GRIDROW;
	}
	else
	{
		trkey->kind = TRKEY_GRIDUPJONG;
	}

	m_updateROW = m_staticUpdate;

	sdata.SetData("pooppoop", key, sendB.data(), sendL, "");

	m_bSending = true;
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);
	m_endsort = false;

	sendB.clear();

}

void CGridWnd::sendTransaction(int update)
{
	m_pGridArray.RemoveAll();
	m_staticUpdate = update;

	char tempB[64]{};

	//////////////////////////////////////
	struct _gridHdr gridhdr{};

	for (int ii = 0; ii < m_gridHdrX.GetSize(); ii++)
	{
		gridhdr = m_gridHdrX.GetAt(ii);

		sprintf(tempB, "%s%c", gridhdr.symbol, P_NEW);
		m_pGridArray.Add(tempB);
	}
	//////////////////////////////////////
	// 	CWnd* m_pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
	// 	m_pWnd->SendMessage(WM_MANAGE, MK_GETTREEITEM);

	if (m_pGridArray.GetSize() > GRIDMAXNUM)
	{
		const int nRem = m_pGridArray.GetSize() - GRIDMAXNUM;

		if (!m_bContinue && !m_bSecond)
		{
			m_bContinue = TRUE;

			sendTransactionTR(m_staticUpdate, 0, GRIDMAXNUM);
		}
	}
	else
	{
		m_bContinue = FALSE;
		sendTransactionTR(m_staticUpdate, 0, m_pGridArray.GetSize());
	}
}

void CGridWnd::settingFieldBeforeSend(int &sendL, char *sendB)
{
	char tempB[64]{};
	struct _gridHdr gridHdr{};

	const int getSize = m_gridHdrX.GetSize();

	if (m_iEndPoint == 0)
	{
		if (getSize > GRIDMAXNUM)
		{
			m_iSendcount = 0; // �����ؼ� ��ȸ�� �ؾ� �Ѵٴ� ��ȣ�� �����ش�
			m_iEndPoint = GRIDMAXNUM;
		}
		else
		{
			m_iEndPoint = m_gridHdrX.GetSize();
		}
	}

	for (int ii = 0; ii < m_gridHdrX.GetSize(); ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);

		sprintf(tempB, "%s%c", gridHdr.symbol, P_NEW);
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}

	sprintf(tempB, "2321%c", P_NEW); //���ϰŷ���
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	m_iStartPoint = m_iEndPoint;
	m_iEndPoint = m_gridHdrX.GetSize();
}

BOOL CGridWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *pResult)
{
	NM_GVITEM *nmgv = (NM_GVITEM *)lParam;
	if (wParam != IDC_GRID)
		return CBaseWnd::OnNotify(wParam, lParam, pResult);

	switch (nmgv->hdr.code)
	{
	case GVNM_REFRESH:
	{
		m_bEditWork = FALSE;

		// 2016.04.14 KSJ basesorting, �϶� �ٽ��ѹ� ��ȸ�Ѵ�. epb ������ �߰���.
		if (CAST_TREEID(m_kind)->kind == xISSUE)
		{
			class CGridData sdata;
			sdata.SetKind(m_kind);
			m_pTreeWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEDATA, 100), (LPARAM)&sdata); //���ϴٽ� �о sdata ����
			// m_pMainWnd->SendMessage(WM_MANAGE, MK_TREEDATA, (LPARAM)&sdata);
			//sdata�� �׸��� �ٽ� �ʱ�ȭ
			m_pTreeWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_ISSUE, 0), (LPARAM)-1);
		}
		else if (CAST_TREEID(m_kind)->kind != xINTEREST)
		{
			class CGridData sdata;
			sdata.SetKind(m_kind);

			m_pTreeWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEDATA, 100), (LPARAM)&sdata); //���ϴٽ� �о sdata ����
			m_pMainWnd->SendMessage(WM_MANAGE, MK_TREEDATA, (LPARAM)&sdata);		  // sdata�� �׸��� �ٽ� �ʱ�ȭ
		}
		else
		{
			m_pToolWnd->SendMessage(WM_MANAGE, MK_REFRESH);
		}

		ClearSearchMap(); // 2011.12.29 KSJ
		ReSetSearchMap(); // 2011.12.29 KSJ
	}
	break;
	case GVNM_CHGCOLSIZE:
	{
		const int ncnt = m_gridHdrX.GetSize();
		CString sztmp = _T("");
		int nWidth{};
		m_szWIDTH = _T("");
		struct _gridHdr grdHdr
		{
		};
		for (int ii = 0; ii < ncnt; ii++)
		{
			grdHdr = m_gridHdrX.GetAt(ii);
			nWidth = m_grid->GetColumnWidth(ii);
			CString strSymbol = (CString)grdHdr.symbol;

			//��õ�����ʵ�� �������� �ʴ´�
			if (strSymbol.Find("191") > -1)
				continue;

			if (strSymbol.Find("2022") > -1)
				continue;

			if (ii >= colNAME)
			{
				sztmp.Format("%d,", nWidth);
				m_szWIDTH += sztmp;

				if (ii == colCODE && nWidth != 0)
					m_nCodeWidth = nWidth;

				// 2011.12.02 KSJ
				if (ii == colSIG && nWidth != 0)
					m_nNewsNChartWidth = nWidth;
				// KSJ
			}
		}
		savecfg();
	}
	break;
	case GVNM_CHANGECODE:
	{
		m_grid->SetFocus();
		CString code, oldname, name;
		code = m_grid->GetItemText(nmgv->row, colCODE);

		if (code.GetLength() == 6 || code.GetLength() == 8 || code.GetLength() == 9)
		{
			name = GetCodeName(code);
		}

		struct _intersx *pinters{};
		const int count = _vInters.size();
		const int xrow = nmgv->row - 1;

		CString strTemp;

		//���� �ߺ� ��� ���
		if (((CGroupWnd *)GetParent())->GetOverLapAction() == FALSE)
		{
			if (code.GetLength() && IsDuplicateCode(code, nmgv->row))
			{
				CCodeDuplicate dlg;

				// dlg �׻� ���� �ɼ� üũ
				if (dlg.DoModal())
				{
					if (dlg.m_bAlert == FALSE)
					{
						m_grid->SetItemText(nmgv->row, colCODE, "");

						if (m_bAddCnd)
						{
							saveInterest();
						}
						break;
					}
					else
					{
						//���Ͽ� ����
						((CGroupWnd *)GetParent())->saveOverLap(TRUE);
					}
				}
				else
				{
					if (count > xrow)
					{
						auto& pInter = _vInters.at(nmgv->row - 1);
						m_grid->SetItemText(nmgv->row, colCODE, pInter->code);
						DeleteRow(nmgv->row);
						insertInterest(MAX_LINE - 1);
						if (m_bAddCnd)
						{
							saveInterest();
						}
						break;
					}
				}
			}
		}

		if (count > xrow)
		{
			auto& pinters = _vInters.at(xrow);

			// ADD PSH 20070913
		//	const int nRowCnt = m_arrBaseInters.GetSize();

			if (m_bSorting)
			{
				if (!pinters->code.IsEmpty())
				{
					// 2011.12.19 KSJ �����ϰ��� ������ �ٲٸ� ���⼭ ������.
					// m_arrBaseInters ����� 0��..
					//CopyInter(m_arrBaseInters.GetAt(xrow), pinters);
					_arrBaseInters.at(xrow) = pinters;
				}
				else
				{
					auto& pBaseInter = _arrBaseInters.emplace_back(std::make_shared<_intersx>());

					pBaseInter->gubn = '0';
					pBaseInter->code = code;
					pBaseInter->name = name;
				}
			}
			// END ADD

			pinters->gubn = '0';
			pinters->code = code;
			pinters->name = name;
			m_grid->SetItemText(xrow + 1, colNAME, name);
			//_vInters.at(xrow) =  pinters;
		}
		else
		{
			for (int ii = count; ii < xrow; ii++)
			{
				auto& pinters = _vInters.emplace_back(std::move(std::make_shared<_intersx>()));
				pinters->empty();
			}

			auto& pinters = _vInters.emplace_back(std::move(std::make_shared<_intersx>()));
			pinters->gubn = '0';
			pinters->code = code;
			pinters->name = name;
			m_grid->SetItemText(xrow + 1, colNAME, name);
			//AddInters(pinters);
		}

		if (code.GetLength() <= 0)
		{
			for (int ii = 0; ii < m_colC; ii++)
				m_grid->SetItemText(nmgv->row, ii, "");
			m_grid->memoCheck(nmgv->row);
			break;
		}

		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_bEditWork = true;

		if (m_bChgCnd)
		{
			saveInterest();
		}

		sendTransaction(xrow);
	}

		ClearSearchMap(); // 2011.12.29 KSJ
		ReSetSearchMap(); // 2011.12.29 KSJ

		break;
	case GVNM_ENDEDIT:
	case GVNM_ENDRETURN:
	{
		m_grid->SetFocus();

		const int nRow = nmgv->row, nCol = nmgv->col;

		CString symbol, curr;
		symbol = m_grid->GetItemSymbol(nRow, nCol);
		curr = m_grid->GetItemText(nRow, colCURR);
		const auto& pInters = GetData(nRow - 1);
		CString value = m_grid->GetItemText(nRow, nCol);

		CString zeroValue = "0";

		// 2011.12.20 KSJ
		if (symbol.IsEmpty())
			break;
		// KSJ

		if (symbol.CompareNoCase("###1") == 0) // ��������
		{
			value = m_grid->GetItemText(nRow, nCol);
			pInters->xnum = value;

			if (m_bChgCnd)
			{
				saveInterest();
			}
		}
		else if (symbol.CompareNoCase("###2") == 0) // �����ܰ�
		{

			pInters->xprc = value;
			if (m_bChgCnd)
			{
				saveInterest();
			}
		}
		else
		// 2011.12.20 KSJ �߰�ȣ �Ǿ� ���� �ʾƼ� �߰���.
		{
			Invalidate(false);
			break;
		}
		// KSJ

		if (m_sonikField > colCURR)
		{
			value = CalcuPyungaSonik(pInters.get(), curr);
			m_grid->SetItemText(nRow, m_sonikField, value);
		}

		if (m_suikField > colCURR)
		{
			value = CalcuSuik(pInters.get(), curr);
			m_grid->SetItemText(nRow, m_suikField, value);
		}

		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_bEditWork = true;
	}

		ClearSearchMap(); // 2011.12.29 KSJ
		ReSetSearchMap(); // 2011.12.29 KSJ

		break;

	case GVNM_COLUMNMOVE:
	{
		if (nmgv->row >= m_gridHdrX.GetSize() || nmgv->col >= m_gridHdrX.GetSize())
			break;

		m_btReload.Invalidate(TRUE);
		m_btDomino.Invalidate(TRUE);
		m_btSAVE.Invalidate(TRUE);
		m_btCLOSE.Invalidate(TRUE);
		_gridHdr gridHdr;

		gridHdr = m_gridHdrX.GetAt(nmgv->col);
		m_gridHdrX.InsertAt(nmgv->row, gridHdr);

		if (nmgv->col > nmgv->row)
			nmgv->col += 1;

		m_gridHdrX.RemoveAt(nmgv->col);
		ReIndexSpecial();

		const int ncnt = m_gridHdrX.GetSize();
		CString sztmp, szFIELD = _T("");
		int nWidth{};
		m_szWIDTH = _T("");
		m_szFIELD = _T("");
		for (int ii = 0; ii < ncnt; ii++)
		{
			gridHdr = m_gridHdrX.GetAt(ii);
			nWidth = m_grid->GetColumnWidth(ii);
			if (ii >= colNAME)
			{
				sztmp.Format("%d,", nWidth);
				m_szWIDTH += sztmp;
			}
			if (ii > colCURR)
			{
				sztmp.Format("%s,", gridHdr.symbol);
				if (sztmp.Compare("2111,") == 0)
					sztmp.Format("2023,");
				else if (sztmp.Compare("2115,") == 0)
					sztmp.Format("2024,");
				else if (sztmp.Compare("2116,") == 0)
					sztmp.Format("2033,");
				else if (sztmp.Compare("2112,") == 0)
					sztmp.Format("2027,");

				m_szFIELD += sztmp;
			}
		}

		SaveAsInterField();
	}

	break;

	case GVNM_BEGINDRAG:
	{
		m_drag = nmgv->row;
		m_drop = -1;
		m_strCode = m_grid->GetItemText(m_drag, colCODE);

		m_grid->m_bOutPos = false;

		if (CAST_TREEID(m_kind)->kind == xISSUE)
			CintGrid::m_dropdata.SetCode(m_grid->GetItemText(m_drag, colCODE));
	}

	break;
	case GVNM_OUTDRAG:
	{
		if (m_grid->m_bOutPos)
		{
			if (CAST_TREEID(m_kind)->kind == xINTEREST)
			{
				DeleteRow(m_drag);
				m_bEditWork = true;
			}
			m_grid->m_bOutPos = false;
		}
	}

	break;
	case GVNM_ENDEND:
	{
		//�巡�� ī��Ʈ�� ȣ��
		((CGroupWnd *)m_pGroupWnd)->AddDragInCount();

		const int xdrag = m_drag, xdrop = nmgv->row;
		m_drop = nmgv->row;
		int nIdx = 0;
		CString code, name;

		m_grid->m_bOutPos = false;

		code = CintGrid::m_dropdata.GetCode();
		name = CintGrid::m_dropdata.GetCode();

		CString strTemp;

		//���� �ߺ� ��� ���
		if (((CGroupWnd *)GetParent())->GetOverLapAction() == FALSE)
		{
			if (!code.IsEmpty() && IsDuplicateCode(code, m_drop, true))
			{
				CCodeDuplicate dlg;

				// dlg �׻� ���� �ɼ� üũ
				if (dlg.DoModal())
				{
					if (dlg.m_bAlert == FALSE)
					{
						m_grid->Invalidate(FALSE);
						break;
					}
					else
					{
						//���Ͽ� ����
						((CGroupWnd *)GetParent())->saveOverLap(TRUE);
					}
				}
				else
				{
					m_grid->Invalidate(FALSE);
					break;
				}
			}
		}

		if (m_drop < 0)
			m_drop = m_grid->GetRowCount();
		else if (m_drop == 0)
			m_drop = 1;

		if (m_drop - 2 < 0)
			nIdx = 0;
		else
			nIdx = m_drop - 1;

		const int nCount = _vInters.size();
		if (nCount > nIdx)
		{
			auto& pinters = _vInters.at(nIdx);
			if (strlen(pinters->code) != 0)
			{
				CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
				const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
				int rowcount = 0;

				if (nOver == MO_VISIBLE)
				{
					pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
					rowcount = ((CGroupWnd*)pWnd)->sumEachGroupCount();
				}
				else
				{
					rowcount = GetRowValidcount();
				}

				if (rowcount >= MAX_LINE)
				{

					Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
					m_grid->Invalidate(FALSE);
					break;
				}
				else
				{
					DeleteRow(MAX_LINE);
				}
				// END DEL

				insertInterest(m_drop - 1);
				insertRow(m_drop);
				m_drop -= 1;
				pinters = _vInters.at(m_drop);
			}

			//struct _intersx InterData;
			//memset(&InterData, 0x20, sizeof(struct _intersx));

			pinters->gubn = '0';
			pinters->code = code;
			pinters->name = name;

			//memcpy(pinters, &InterData, sizeof(struct _intersx));

			if (CAST_TREEID(m_kind)->kind == xINTEREST)
				m_bEditWork = true;

			m_drag = m_drop = -1;
			m_seldrop = xdrop;
			sendTransaction();
			m_pGroupWnd->SendMessage(WM_MANAGE, MK_ENDDRAG);

			CString strINI;
			strINI.Format("%s\\User\\%s\\userconf.ini", m_root, m_user);
			const int dragcnt = GetPrivateProfileInt("IB202200", "DRAGCNT", 0, strINI);

			CString cnt;
			cnt.Format("%d", ((CGroupWnd*)m_pGroupWnd)->GetDragInCount() + dragcnt);
			WritePrivateProfileString("IB202200", "DRAGCNT", cnt, strINI);
		}
	}

	break;
	case GVNM_ENDDRAG:
	{
		if (CAST_TREEID(m_kind)->kind == xISSUE)
		{
			m_grid->Invalidate(FALSE);
			break;
		}

		m_grid->m_bOutPos = false;

		CDropData *pDropData = &CintGrid::m_dropdata; // drop information
		CintGrid *pGrid = pDropData->GetGrid();	      // drag grid
		if (pGrid == nullptr)
			break;

		const int nDGrow = pDropData->GetRow(); // drag row

		const int ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETARRANGE);

		if (ret != 0) //��ϼ��� �ƴҰ�� �巡�� ��� ����
		{
			m_grid->SetSelectRow(nDGrow);
			return 0;
		}

		CGridWnd *pDropGrid = (CGridWnd *)pDropData->GetGrid()->GetParent(); // drag gridwnd
		CGroupWnd *pDropGroup = (CGroupWnd *)pDropGrid->GetParent();	     // drag groupwnd
		const int xdrag = m_drag, xdrop = nmgv->row;
		int nIdx = 0; // BUFFET
		CString xCODE;
		CString code, name;
		BOOL bNewsDrop = FALSE;
		const UINT kind = (UINT)pDropGrid->SendMessage(WM_MANAGE, MK_GETDATAKIND);
		if (CAST_TREEID(kind)->kind == xISSUE)
			bNewsDrop = TRUE;
		if (bNewsDrop)
			xCODE = CintGrid::m_dropdata.GetCode();
		m_drop = nmgv->row;


		const int count = _vInters.size();

		if (count >= m_drag && count >= m_drop)
		{
			if (pDropData->GetGrid()->m_hWnd == m_grid->m_hWnd)		// ������ ȭ�鿡�� drop
			{
				if (m_drop < 0)
					m_drop = m_grid->GetRowCount() - 1;
				else if (m_drop == 0)
					m_drop = 1;

				if (m_drag == m_drop)
					break;

				auto pDrag = _vInters.at(m_drag - 1);
				_vInters.erase(_vInters.begin() + m_drag - 1);
				_vInters.insert(_vInters.begin() + m_drop - 1, pDrag);
			}
			else if (pDropData->GetGrid()->m_hWnd != m_grid->m_hWnd) //(m_drag < 0 && m_drop > 0)	// �ٸ� ȭ�鿡�� drop
			{
				code = (bNewsDrop) ? xCODE : pGrid->GetItemText(nDGrow, colCODE);
				name = pGrid->GetItemText(nDGrow, colNAME);

				if (m_drop < 0)
					m_drop = m_grid->GetRowCount();
				else if (m_drop == 0)
					m_drop = 1;


				if (pDropData->GetGrid()->m_hWnd != m_grid->m_hWnd)
				{
					//���� �ߺ� ��� ���
					if (((CGroupWnd*)GetParent())->GetOverLapAction() == FALSE)
					{
						if (!code.IsEmpty() && IsDuplicateCode(code, m_drop, true))
						{
							CCodeDuplicate dlg;
							// dlg �׻� ���� �ɼ� üũ
							if (dlg.DoModal())
							{
								if (dlg.m_bAlert == FALSE)
								{
									m_grid->Invalidate(FALSE);
									break;
								}
								else
								{
									//���Ͽ� ����
									((CGroupWnd*)GetParent())->saveOverLap(TRUE);
								}
							}
							else
							{
								m_grid->Invalidate(FALSE);
								break;
							}
						}
					}
				}

				if (m_drop - 2 < 0)
					nIdx = 0;
				else
					nIdx = m_drop - 2;

				const auto& pinters = _vInters.at(nIdx);
				if (!pinters->code.IsEmpty())
				{
					CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
					const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
					int rowcount = 0;

					if (nOver == MO_VISIBLE)
					{
						pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
						rowcount = ((CGroupWnd*)pWnd)->sumEachGroupCount();
					}
					else
					{
						rowcount = GetRowValidcount();
					}

					if (rowcount >= MAX_LINE)
					{
						Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
						m_grid->Invalidate(FALSE);
						break;
					}
					else
					{
						DeleteRow(MAX_LINE);
					}
					// END DEL

					insertInterest(m_drop - 1);
					insertRow(m_drop);
					m_drop -= 1;
				}
				//�׸��忡 ���ٵ� ���� ��� ���Ƿ� ���� �߰� ����
				else
				{
					if (nIdx == 0 && m_rowC < 2)
					{
						insertRow(m_drop);
					}
				}

				auto& pDrop = _vInters.at(m_drop);
				// 2013.01.22 KSJ �����Ŀ��� �׸����� �����͸� ������ �� �ݵ�� ���� m_inters�� ������ġ�� ã�ƾ� �Ѵ�.
				const auto& pInterDrag = pDropGrid->GetData(pDropGrid->GetInter(code));
				pDrop = pInterDrag;

				// ADD PSH 20070918
				UINT uAttr{};
				if ('m' == pinters->gubn)
				{
					uAttr = m_grid->GetItemAttr(m_drop + 1, colNAME) | GVAT_MARKER;
					m_grid->SetItemAttr(m_drop + 1, colNAME, uAttr);
					m_grid->SetItemText(m_drop + 1, colNAME, pinters->name);
				}
				else
				{
					uAttr = m_grid->GetItemAttr(m_drop + 1, colNAME) & ~GVAT_MARKER;
					m_grid->SetItemAttr(m_drop + 1, colNAME, uAttr);
					m_grid->SetItemText(m_drop + 1, colNAME, pinters->name);
				}
				// END ADD

				const LPARAM data = pGrid->GetItemData(nDGrow, colSIG);
				m_grid->SetItemData(xdrop, colSIG, data);

				// index�� �ϳ� ���̰� ���Ƿ�
				if (pDropGroup->m_hWnd == m_pGroupWnd->m_hWnd)
				{
					if (!bNewsDrop)
						pDropGrid->SendMessage(WM_MANAGE, MAKEWPARAM(MK_DELETEROW, nDGrow));
				}
			}

			if (!m_bSorting)
			{
				if (CAST_TREEID(m_kind)->kind == xINTEREST)
				{
					m_bEditWork = true;
				}
			}
			else
			{
				if (m_bMoveCfg && (1 == m_nMoveSave))
				{
					RemoveAllBaseInters();
					m_grid->SortBase();
					saveInterest();
				}
				else if (!m_bMoveCfg)
				{
					m_bMoveCfg = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVECFG);
					m_nMoveSave = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVESAVE);

					// 091013 ����
					CMoveSetupDlg dlg;
					dlg.m_nOption = m_nMoveSave;
					dlg.m_bAlert = m_bMoveCfg;

					// dlg �׻� ���� �ɼ� üũ
					if (dlg.m_bAlert)
					{
						if (1 == dlg.m_nOption)
						{
							RemoveAllBaseInters();
							m_grid->SortBase();
							saveInterest();
							m_bEditWork = false;
						}
						else
						{
							if (CAST_TREEID(m_kind)->kind == xINTEREST)
								m_bEditWork = true;
						}
					}
					else
					{
						if (dlg.DoModal())
						{
							if (1 == dlg.m_nOption)
							{
								RemoveAllBaseInters();
								m_grid->SortBase();
								saveInterest();
								m_bEditWork = false;
							}
							else
							{
								if (CAST_TREEID(m_kind)->kind == xINTEREST)
									m_bEditWork = true;
							}
						}
					}

					m_pToolWnd->SendMessage(WM_MANAGE, MK_SETMOVECFG, dlg.m_bAlert);
					m_pToolWnd->SendMessage(WM_MANAGE, MK_SETMOVESAVE, dlg.m_nOption);
				}
			}

			m_drag = m_drop = -1;
			m_seldrop = xdrop;
			sendTransaction();
			m_pGroupWnd->SendMessage(WM_MANAGE, MK_ENDDRAG);
		}
	}
	break;
	case GVNM_SELCHANGED:
	{
		CString code, string;
		if (nmgv->row >= m_grid->GetFixedRowCount() && nmgv->col >= colNAME)
		{
			code = m_grid->GetItemText(nmgv->row, colCODE);
			code.TrimLeft();
			code.TrimRight();

			if (code.IsEmpty())
				break;

			if (((CGroupWnd *)m_pGroupWnd)->GetSelAction() == 0)
				break;

			switch (GetCodeType(code))
			{
			case kospiCODE:
				string.Format("%s\t%s", HCOD, code);
				break;
			case futureCODE:
				string.Format("%s\t%s", "ed_focod", code); // 2013.01.03 KSJ 2200��������ȭ�鿡�� Ʈ���ź���
				Variant(triggerCC, string);

				string.Format("%s\t%s", FCOD, code);
				break;
			case optionCODE:
				string.Format("%s\t%s", "ed_focod", code); // 2013.01.03 KSJ 2200��������ȭ�鿡�� Ʈ���ź���
				Variant(triggerCC, string);

				string.Format("%s\t%s", OCOD, code);
				break;
			case foptionCODE:
				string.Format("%s\t%s", "ed_focod", code); // 2013.01.03 KSJ 2200��������ȭ�鿡�� Ʈ���ź���
				Variant(triggerCC, string);

				string.Format("%s\t%s", PCOD, code);
				break;
			case indexCODE:
				string.Format("%s\t%s", UCOD, code);
				break;
			case sinjuCODE:
				string.Format("%s\t%s", SINJUCOD, code);
				break;
			case elwCODE:
				string.Format("%s\t%s", HCOD, code);
				Variant(triggerCC, string);
				string.Format("%s\t%s", ELWCOD, code);
				m_grid->SetFocus();
				break;
			case thirdCODE:
				string.Format("%s\t%s", THIRDCOD, code);
				break;
			case 0:							  // 2012.10.11 KSJ �ֽ�, �ݸ�, ��ȭ���� �߰�
				if (code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //�ֽ�, �ݸ�, ��ȭ����  //�Ļ���ǰ �ڵ尳��
				{
					int nGubn = atoi(code.Mid(1, 2));
					CString strGubn = code.Mid(1, 2); // 2014.10.21 KSJ �ֽļ��� �ű����� ����

					if (nGubn > 10 && nGubn < 60) //�ֽļ���
					{
						string.Format("%s\t%s", "ed_fcod", code); // 2013.01.02 KSJ 2300��������ȭ�鿡�� Ʈ���ź���
						Variant(triggerCC, string);

						string.Format("%s\t%s", SFOD, code);
						break;
					}
					else if (nGubn > 60 && nGubn < 70) //�ݸ�����
					{
						string.Format("%s\t%s", SFOD, code);
						break;
					}
					else if (nGubn > 70 && nGubn < 80) //��ȭ����
					{
						string.Format("%s\t%s", SFOD, code);
						break;
					}
					else if (strGubn >= "B0" && strGubn <= "ZZ") // 2014.10.21 KSJ �ֽļ��� �ű����� ����
					{
						string.Format("%s\t%s", "ed_fcod", code); // 2013.01.02 KSJ 2300��������ȭ�鿡�� Ʈ���ź���
						Variant(triggerCC, string);

						string.Format("%s\t%s", SFOD, code);
						break;
					}
				}
			}

			m_grid->SetFocus();

			break;
		}
		break;
		// Map �ʱ�ȭ KSI
		ClearSearchMap();
		ReSetSearchMap(); // 2011.12.29 KSJ
	}

	case GVNM_TRIGGER:
	{		
		CString code = m_grid->GetItemText(nmgv->row, colCODE);
		CString string;
		// if (nmgv->row >= m_grid->GetFixedRowCount() && nmgv->col >= colNAME)
		{
			// code = m_grid->GetItemText(nmgv->row, colCODE);
			code.TrimLeft();
			code.TrimRight();

			if (code.IsEmpty())
				break;

			if (((CGroupWnd *)m_pGroupWnd)->GetSelAction() == 0)
				break;

			switch (GetCodeType(code))
			{
			case kospiCODE:
				string.Format("%s\t%s", HCOD, code);
				break;
			case futureCODE:
				string.Format("%s\t%s", FCOD, code);
				break;
			case optionCODE:
				string.Format("%s\t%s", OCOD, code);
				break;
			case foptionCODE:
				string.Format("%s\t%s", PCOD, code);
				break;
			case indexCODE:
				string.Format("%s\t%s", UCOD, code);
				break;
			case sinjuCODE:
				string.Format("%s\t%s", SINJUCOD, code);
				break;
			case elwCODE:
				string.Format("%s\t%s", HCOD, code);
				Variant(triggerCC, string);
				//					m_grid->SetFocus();
				string.Format("%s\t%s", ELWCOD, code);
				break;
			case thirdCODE:
				string.Format("%s\t%s", THIRDCOD, code);
				break;
			case 0:							  // 2012.10.11 KSJ �ֽ�, �ݸ�, ��ȭ���� �߰�
				if (code.GetAt(0) == '1' || code.GetAt(0) == '4 ' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //�ֽ�, �ݸ�, ��ȭ����  //�Ļ���ǰ �ڵ尳��
				{
					int nGubn = atoi(code.Mid(1, 2));
					CString strGubn = code.Mid(1, 2); // 2014.10.21 KSJ �ֽļ��� �ű����� ����

					if (nGubn > 10 && nGubn < 60) //�ֽļ���
					{
						string.Format("%s\t%s", "ed_fcod", code); // 2013.01.02 KSJ 2300��������ȭ�鿡�� Ʈ���ź���
						Variant(triggerCC, string);

						string.Format("%s\t%s", SFOD, code);
						break;
					}
					else if (nGubn > 60 && nGubn < 70) //�ݸ�����
					{
						string.Format("%s\t%s", SFOD, code);
						break;
					}
					else if (nGubn > 70 && nGubn < 80) //��ȭ����
					{
						string.Format("%s\t%s", SFOD, code);
						break;
					}
					else if (strGubn >= "B0" && strGubn <= "ZZ") // 2014.10.21 KSJ �ֽļ��� �ű����� ����
					{
						string.Format("%s\t%s", "ed_fcod", code); // 2013.01.02 KSJ 2300��������ȭ�鿡�� Ʈ���ź���
						Variant(triggerCC, string);

						string.Format("%s\t%s", SFOD, code);
						break;
					}
				}
			}
			Variant(triggerCC, string);
			Variant(codeCC, string);

			if (nmgv != nullptr)
			{
				if (nmgv->row != m_ClickRow)
				{
					m_grid->SetFocus();
					m_ClickRow = nmgv->row;
				}
			}
			break;
		}
	}
	break;
	case GVNM_LMOUSEDOWN:
	{
		CString code, string;
		CString strINI;
		strINI.Format("%s\\User\\%s\\userconf.ini", m_root, m_user);

		if (nmgv->row >= m_grid->GetFixedRowCount() && nmgv->col == colSIG)
		{
			// 2015.01.15 KSJ ���� ���Ƿ� trim ���ش�.
			CString sRecommand;
			sRecommand = GetRecommandInfo(nmgv->row);
			sRecommand.TrimRight();
			sRecommand.TrimLeft();

			if (sRecommand != "")
			{
				code = m_grid->GetItemText(nmgv->row, colCODE);
				code.TrimLeft();
				code.TrimRight();
				if (code.IsEmpty())
					break;

				WritePrivateProfileString("IB202600", "POPUP_FLAG", "1", strINI);
				WritePrivateProfileString("IB202600", "POPUP_CODE", code, strINI);
				string.Format("IB202600 /S/t0/d%s\t%s", HCOD, code);
				openView(typeVIEW, string);
				CString codepopup;
				codepopup.Format("%s\t%s", HCOD, code);
			}
		}
	}
	break;
	case GVNM_DBLCLICKED:
	{
		CString code, string;
		if (nmgv->row >= m_grid->GetFixedRowCount() && nmgv->col >= colNAME)
		{
			code = m_grid->GetItemText(nmgv->row, colCODE);
			code.TrimLeft();
			code.TrimRight();
			if (code.IsEmpty())
				break;

			switch (((CGroupWnd *)m_pGroupWnd)->GetDblAction())
			{
			case dblCURR:
				switch (GetCodeType(code))
				{
				case kospiCODE:
				case thirdCODE: // 2013.08.07 ���������϶��� 2001���� ����ش�.
					string.Format("%s /S/t0/d%s\t%s", kcurrMAP, HCOD, code);
					openView(typeVIEW, string);
					break;
				case futureCODE:
					string.Format("%s /S/t0/d%s\t%s", fcurrMAP, FCOD, code);
					openView(typeVIEW, string);
					break;
				case optionCODE:
					string.Format("%s /S/t0/d%s\t%s", ocurrMAP, OCOD, code);
					openView(typeVIEW, string);
					break;
				case foptionCODE:
					string.Format("%s /S/t0/d%s\t%s", focurrMAP, PCOD, code);
					openView(typeVIEW, string);
					break;
				case indexCODE:
					string.Format("%s /S/t0/d%s\t%s", icurrMAP, UCOD, code);
					openView(typeVIEW, string);
					break;
				case elwCODE:
					string.Format("%s /S/t0/d%s\t%s", ecurrMAP, ELWCOD, code);
					openView(typeVIEW, string);
					break;
				case 0:							  // 2012.10.11 KSJ �ֽ�, �ݸ�, ��ȭ���� �߰�
					if (code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //�ֽ�, �ݸ�, ��ȭ����//�Ļ���ǰ �ڵ尳��
					{
						int nGubn = atoi(code.Mid(1, 2));
						CString strGubn = code.Mid(1, 2); // 2014.10.21 KSJ �ֽļ��� �ű����� ����

						if (nGubn > 10 && nGubn < 60) //�ֽļ���
						{
							string.Format("%s /S/t0/d%s\t%s", sfcurrMAP, EDFCOD, code);
							openView(typeVIEW, string);
							break;
						}
						else if (nGubn > 60 && nGubn < 70) //�ݸ�����
						{
							string.Format("%s /S/t0/d%s\t%s", cccurrMAP, SFOD, code);
							openView(typeVIEW, string);
							break;
						}
						else if (nGubn > 70 && nGubn < 80) //��ȭ����
						{
							string.Format("%s /S/t0/d%s\t%s", cccurrMAP, SFOD, code);
							openView(typeVIEW, string);
							break;
						}
						else if (strGubn >= "B0" && strGubn <= "ZZ") // 2014.10.21 KSJ �ֽļ��� �ű����� ����
						{
							string.Format("%s /S/t0/d%s\t%s", sfcurrMAP, EDFCOD, code);
							openView(typeVIEW, string);
							break;
						}
					}
				}
				break;
			case dblCHART:
				string.Format("%s /S/t0/d%s\t%s", chartMAP, CCOD, code);
				openView(typeVIEW, string);
				break;
			case dblUSER:
			{
				CString mapname = ((CGroupWnd *)m_pGroupWnd)->GetLinkmap();
				if (mapname.IsEmpty())
					break;

				switch (GetCodeType(code))
				{
				case kospiCODE:
					string.Format("%s /S/t0/d%s\t%s", mapname, HCOD, code);
					openView(typeVIEW, string);
					break;
				case futureCODE:
					string.Format("%s /S/t0/d%s\t%s", mapname, FCOD, code);
					openView(typeVIEW, string);
					break;
				case optionCODE:
					string.Format("%s /S/t0/d%s\t%s", mapname, OCOD, code);
					openView(typeVIEW, string);
					break;
				case foptionCODE:
					string.Format("%s /S/t0/d%s\t%s", mapname, PCOD, code);
					openView(typeVIEW, string);
					break;
				case indexCODE:
					string.Format("%s /S/t0/d%s\t%s", mapname, UCOD, code);
					openView(typeVIEW, string);
					break;
				case 0:							  // 2012.10.11 KSJ �ֽ�, �ݸ�, ��ȭ���� �߰�
					if (code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //�ֽ�, �ݸ�, ��ȭ����//�Ļ���ǰ �ڵ尳��
					{
						int nGubn = atoi(code.Mid(1, 2));
						const CString strGubn = code.Mid(1, 2); // 2014.10.21 KSJ �ֽļ��� �ű����� ����

						if (nGubn > 10 && nGubn < 60) //�ֽļ���
						{
							string.Format("%s /S/t0/d%s\t%s", mapname, SFOD, code);
							openView(typeVIEW, string);
							break;
						}
						else if (nGubn > 60 && nGubn < 70) //�ݸ�����
						{
							string.Format("%s /S/t0/d%s\t%s", mapname, SFOD, code);
							openView(typeVIEW, string);
							break;
						}
						else if (nGubn > 70 && nGubn < 80) //��ȭ����
						{
							string.Format("%s /S/t0/d%s\t%s", mapname, SFOD, code);
							openView(typeVIEW, string);
							break;
						}
						else if (strGubn >= "B0" && strGubn <= "ZZ") // 2014.10.21 KSJ �ֽļ��� �ű����� ����
						{
							string.Format("%s /S/t0/d%s\t%s", mapname, SFOD, code);
							openView(typeVIEW, string);
							break;
						}
					}
				}
				break;
			}
			}
		}
	}
	break;
	case GVNM_ENDSORT:
	{
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
		{
			// 				m_bEditWork = true;		//2012.02.13 KSJ �����Ѱ� ��� ��Ʈ�ϰ��� �����϶���ؼ� �ּ�ó����.
		}

		m_endsort = true;
		m_grid->memoCheck();
		rebuildInterest();

		ClearSearchMap();
		ReSetSearchMap(); // 2011.12.29 KSJ

		COLORREF bkColor{};
		for (int ii = 0; ii < m_grid->GetRowCount(); ii++)
		{
			CString strTemp = m_grid->GetItemText(ii, colNAME);
			if (!ii)
				continue;

			bkColor = ((CGroupWnd *)m_pGroupWnd)->GetBkColor1();
			if (ii > 1 && ((ii - 1) / ((CGroupWnd *)m_pGroupWnd)->GetLine()) % 2)
				bkColor = ((CGroupWnd *)m_pGroupWnd)->GetBkColor2();
			for (int jj = 0; jj < m_colC; jj++)
				m_grid->SetItemBkColor(ii, jj, bkColor);
		}

		const LONG ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
		hawkeyeAction((ret) ? true : false);
		// DEL PSH 20070913
		// saveInterest();
		// END DEL

		// ADD PSH 20070913
		m_bSorting = TRUE;
		// END ADD

		// Map �ʱ�ȭ KSI
		ClearSearchMap();
		ReSetSearchMap(); // 2011.12.29 KSJ
	}
	break;

	case GVNM_RMOUSEDOWN:
		// MODIFY PSH 20070912
		{
			if (0 < nmgv->row)
			{
				RbuttonAction(nmgv->row);
			}
			else
			{
				ShowPopupMenu();
			}
		}
		// END MODIFY
		break;
	// ADD PSH 20070914
	case GVNM_CHANGEMARKER:
	{
		m_grid->SetFocus();
		CString strMarker, strMarkerCode;
		strMarker = m_grid->GetItemText(nmgv->row, colNAME);

		const int nCount = _vInters.size();
		const int nMarkerRow = nmgv->row - 1;
		strMarkerCode.Format("m%05d", nMarkerRow);

		if (!m_bSorting && nCount > nMarkerRow)
		{
			auto& pInter = _vInters.at(nMarkerRow);
			pInter->code = strMarkerCode;
			pInter->name = strMarker;
			saveInterest();
		}
	}
	break;
		// END ADD
	}

	return CBaseWnd::OnNotify(wParam, lParam, pResult);
}
bool CGridWnd::IsDuplicateCode(CString code, int nrow, bool drag)
{
	CString string;
	const int nMax = m_grid->GetRowCount();

	for (int ii = 1; ii < nMax /*m_rowC*/; ii++)
	{
		if (!drag)
		{
			if (ii == nrow)
				continue;
		}

		string = m_grid->GetItemText(ii, colCODE);
		if (code == string)
			return true;
	}

	return false;
}

int CGridWnd::GetCodeType(CString code)
{
	const int codeL = code.GetLength();
	if (codeL <= 0)
		return 0;

	const int result = m_pView->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)code);

	switch (result)
	{
	case kospiType:	 // kospi code
	case kosdaqType: // kosdaq code
	case etfType:	 // etf code
	case etnType:	 // 2014.10.28 KSJ ETN �߰�
		return kospiCODE;
		;
	case elwType: // ELW
		return elwCODE;
	case futureType:
		return futureCODE; // ����
	case sfutureType:
		return sfCODE;
	case callType: // call option
	case putType:  // put  option
		return optionCODE;
	case indexType: // ����
		return indexCODE;
	case thirdType: // ��3����
		return thirdCODE;
	case sinjuType: // �����μ���
		return sinjuCODE;
	case mufundType: // ������ݵ�
	case reitsType:	 // ����
	case hyfundType: // �����ϵ��ݵ�
		return 200;
	}

	return 0;
}

int CGridWnd::openView(int type, CString data)
{
	return m_pView->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)(LPCTSTR)data);
}

void CGridWnd::hawkeyeAction(bool toggle)
{
	CString code;

	if (toggle) // hawkeye color
	{
		for (int ii = 1; ii < m_rowC; ii++)
		{
			code = m_grid->GetItemText(ii, colCODE);
			code.TrimLeft();
			code.TrimRight();
			if (!code.IsEmpty())
				hawkEyeColor(ii);
			else
				SetLineColor(ii);
		}
	}
	else
		SetLineColor();

	m_grid->m_nAlmRow = -1; // BUFFET
	m_grid->memoRefresh();
}

void CGridWnd::RbuttonAction(int row)
{
	if (row == 0)
		return;

	const int userBASE = 100;
	const int menuBASE = 200;

	m_grid->SetSelectRow(row);

	CPoint point;
	GetCursorPos(&point);

	int index = -1, offs{};
	char *ptr{}, wb[1024]{};
	;
	CStringArray items;
	items.RemoveAll();
	CString string, path, domino, mapN = _T("IB202200");

	CString code;

	if (m_bPopupDlg)
	{
		CPopupDlg dlg;
		dlg.m_nX = point.x;
		dlg.m_nY = point.y;

		path.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, "IB20");

		DWORD rc = GetPrivateProfileSection(mapN, wb, sizeof(wb), path);
		if (rc <= 0)
		{
			path.Format("%s/%s/%s", m_root, MTBLDIR, "IB20");
			rc = GetPrivateProfileSection(mapN, wb, sizeof(wb), path);
		}

		ptr = wb;
		for (; rc > 0; ptr++, rc--)
		{
			string = ptr;
			ptr += string.GetLength();
			rc -= string.GetLength();
			if (string.GetLength() <= L_MAPN)
			{
				if (string.Find('-') == 0)
					continue;
			}
			mapN = string.Left(L_MAPN);
			string = string.Mid(L_MAPN);
			offs = string.Find('=');
			if (offs != -1)
				string = string.Mid(offs + 1);

			index = items.Add(mapN);
			dlg.AddMenu(menuBASE + index, string);
		}

		code = m_grid->GetItemText(row, colCODE);
		code.TrimRight();

		Variant(triggerCC, code);
		m_grid->SetFocus();

		const int dataidx = CAST_TREEID(m_kind)->kind;

		if (dataidx != xISSUE)
		{
			dlg.AddMenu(userBASE + 0, "���� �߰�");
			dlg.AddMenu(userBASE + 1, "���� ����");
			dlg.AddMenu(userBASE + 2, "���� ����");

			if (!code.IsEmpty())
			{
				dlg.AddMenu(userBASE + 3, "�޸� �Է�");
				dlg.AddMenu(userBASE + 11, "�޸� ����");
				dlg.AddMenu(userBASE + 12, "�ϸ�ũ ����");
				dlg.AddMenu(userBASE + 13, "�ϸ�ũ ����");
			}

			dlg.AddMenu(userBASE + 9, "å���� �߰�");
			dlg.AddMenu(userBASE + 10, "å���� ����");
		}

		//		dlg.AddMenu(userBASE+14, "�������簡2 ���񿬵�");

		if (dataidx == xINTEREST)
			dlg.AddMenu(userBASE + 6, "���ɱ׷�����");
		else
			dlg.AddMenu(userBASE + 7, "���ɱ׷��߰�");

		dlg.AddMenu(userBASE + 8, "�׷��ʱ�ȭ");

		dlg.AddMenu(userBASE + 4, "����ȭ�� ����...");

		dlg.AddMenu(userBASE + 5, "������ ������");

		if (-1 == dlg.DoModal())
			return;

		index = dlg.m_nResult;
	}
	else
	{
		auto popM = std::make_unique<CMenu>();
		popM->CreatePopupMenu();

		path.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, "IB20");

		DWORD rc = GetPrivateProfileSection(mapN, wb, sizeof(wb), path);
		if (rc <= 0)
		{
			path.Format("%s/%s/%s", m_root, MTBLDIR, "IB20");
			rc = GetPrivateProfileSection(mapN, wb, sizeof(wb), path);
		}

		ptr = wb;
		for (; rc > 0; ptr++, rc--)
		{
			string = ptr;
			ptr += string.GetLength();
			rc -= string.GetLength();
			if (string.GetLength() <= L_MAPN)
			{
				if (string.Find('-') == 0)
					popM->AppendMenu(MF_SEPARATOR, 0);
				continue;
			}
			mapN = string.Left(L_MAPN);
			string = string.Mid(L_MAPN);
			offs = string.Find('=');
			if (offs != -1)
				string = string.Mid(offs + 1);

			index = items.Add(mapN);
			popM->AppendMenu(MF_STRING | MF_ENABLED, menuBASE + index, string);
		}

		if (index >= 0)
		{
			index++;
			popM->AppendMenu(MF_SEPARATOR, 0);
		}

		code = m_grid->GetItemText(row, colCODE);
		code.TrimRight();

		const int dataidx = CAST_TREEID(m_kind)->kind;

		// pyt
		CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		const int ret = pWnd->SendMessage(WM_MANAGE, MK_GETARRANGE);
		if (dataidx != xISSUE && ret == 0)
		//		if (dataidx != xISSUE)
		{
			popM->AppendMenu(MF_STRING, userBASE + 0, "���� �߰�<INSERT>");
			popM->AppendMenu(MF_STRING, userBASE + 1, "���� ����<DELETE>");
			popM->AppendMenu(MF_STRING, userBASE + 2, "���� ����");

			if (!code.IsEmpty())
			{
				popM->AppendMenu(MF_STRING, userBASE + 3, "�޸� �Է�");
				popM->AppendMenu(MF_STRING, userBASE + 11, "�޸� ����");
				popM->AppendMenu(MF_STRING, userBASE + 12, "�ϸ�ũ ����");
				popM->AppendMenu(MF_STRING, userBASE + 13, "�ϸ�ũ ����");
			}
			// MODIFY PSH 20070914
			popM->AppendMenu(MF_STRING, userBASE + 15, "�����߰�");
			popM->AppendMenu(MF_STRING, userBASE + 9, "å���� �߰�");
			popM->AppendMenu(MF_STRING, userBASE + 10, "å���� ����");
			// END MODIFY
		}

		// 2016.08.12 KSJ �̽������̾ �������簡2 �� �������Ѵ�.
		popM->AppendMenu(MF_STRING, userBASE + 14, "�������簡2 ���񿬵�");
		popM->AppendMenu(MF_SEPARATOR, 0);

		if (dataidx == xINTEREST)
			popM->AppendMenu(MF_STRING, userBASE + 6, "���ɱ׷�����");
		else
			popM->AppendMenu(MF_STRING, userBASE + 7, "���ɱ׷��߰�");
		popM->AppendMenu(MF_STRING, userBASE + 8, "�׷��ʱ�ȭ");
		popM->AppendMenu(MF_SEPARATOR, 0);
		popM->AppendMenu(MF_STRING, userBASE + 4, "����ȭ�� ����...");
		popM->AppendMenu(MF_SEPARATOR, 0);
		popM->AppendMenu(MF_STRING, userBASE + 5, "������ ������");

		index = popM->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, point.x, point.y, m_pView);
	}

	domino = _T("");
	if (!code.IsEmpty())
	{
		switch (GetCodeType(code))
		{
		case kospiCODE:
			domino.Format("%s\t%s", HCOD, code);
			break;
		case futureCODE:
			domino.Format("%s\t%s", FCOD, code);
			break;
		case optionCODE:
			domino.Format("%s\t%s", OCOD, code);
			break;
		case foptionCODE:
			domino.Format("%s\t%s", PCOD, code);
			break;
		case indexCODE:
			domino.Format("%s\t%s", UCOD, code);
			break;
		case elwCODE:
			domino.Format("%s\t%s", HCOD, code);
			break;
		case 0:							  // 2012.10.11 KSJ �ֽ�, �ݸ�, ��ȭ���� �߰�
			if (code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //�ֽ�, �ݸ�, ��ȭ����//�Ļ���ǰ �ڵ尳��
			{
				int nGubn = atoi(code.Mid(1, 2));
				CString strGubn = code.Mid(1, 2); // 2014.10.21 KSJ �ֽļ��� �ű����� ����

				if (nGubn > 10 && nGubn < 60) //�ֽļ���
				{
					domino.Format("%s\t%s", SFOD, code);
					break;
				}
				else if (nGubn > 60 && nGubn < 70) //�ݸ�����
				{
					domino.Format("%s\t%s", SFOD, code);
					break;
				}
				else if (nGubn > 70 && nGubn < 80) //��ȭ����
				{
					domino.Format("%s\t%s", SFOD, code);
					break;
				}
				else if (strGubn >= "B0" && strGubn <= "ZZ") // 2014.10.21 KSJ �ֽļ��� �ű����� ����
				{
					domino.Format("%s\t%s", SFOD, code);
					break;
				}
			}
		}
	}

	if (index >= menuBASE)
	{
		index -= menuBASE;
		if (index < items.GetSize())
		{
			string = items.GetAt(index);
			string += "/s/t0/d";
			string += domino;
			openView(typeVIEW, string);
		}
		items.RemoveAll();
		return;
	}
	items.RemoveAll();

	switch (index)
	{
	case userBASE + 0: // �߰�
	{
		int gno = -1;
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			gno = CAST_TREEID(m_kind)->value;

		if (gno == -1)
		{
			MessageBox("�׷��� ���õ��� �ʾ� �������� ����� �Ұ��մϴ�.\n���ϴ� �׷��� �����ϰų� �׷��� ���� ��� ����ȭ�鿡�� ���׷��� ����Ͻñ� �ٶ��ϴ�.", "��������", MB_OK);
			return;
		}

		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_bEditWork = true;

		CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
		int rowcount = 0;

		if (nOver == MO_VISIBLE)
		{
			pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
			rowcount = ((CGroupWnd *)pWnd)->sumEachGroupCount();
		}
		else
		{
			rowcount = GetRowValidcount();
		}

		if (rowcount >= MAX_LINE)
		{

			Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
			return;
		}

		insertInterest(row - 1);
		insertRow(row);
		SetLineColor();
		m_grid->memoCheck();
		//			m_grid->memoCheck(row+1);
		m_grid->memoRefresh();
		m_grid->SetFocusCellEdit(row, colNAME, true);

		if (m_bAddCnd)
		{
			saveInterest();
		}
	}
	break;
	case userBASE + 1: // ����
	{
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_bEditWork = true;

		if (gsl::narrow_cast<int>(_vInters.size()) >= row - 1)
		{
			if (!(m_grid->GetItemAttr(row, colNAME) & GVAT_MARKER))
			{
				DeleteRow(row);
				insertInterest(MAX_LINE - 1);
				// DEL PSH 20070914
				// insertRow(m_grid->GetRowCount());
				// END DEL
				//					saveInterest();
			}
			// END MODIFY
		}
	}
	break;
	case userBASE + 2:
		m_grid->SetFocusCellEdit(row, colNAME, true);
		break;
	case userBASE + 3:
		m_grid->memoWindowEdit(row);
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_MEMO);
		break;
	case userBASE + 4:
		mapN = _T("IB202200");
		m_pView->SendMessage(WM_USER, MAKEWPARAM(linkDLL, 0), (LPARAM)(LPCTSTR)mapN);
		break;
	case userBASE + 5:
		if (m_grid)
			m_grid->RunExcel();
		break;
	case userBASE + 6: // ���ɱ׷�����
		saveInterestX();
		break;
	case userBASE + 7: // ���ɱ׷��߰�
			   //		saveInterestX();
		break;
	case userBASE + 8: // �׷�ȭ���ʱ�ȭ
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			Reset(false);
		else
			Reset(true);
		break;
	case userBASE + 15: //���� �߰�
	{
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_bEditWork = true;

		CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
		int rowcount = 0;

		if (nOver == MO_VISIBLE)
		{
			pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
			rowcount = ((CGroupWnd *)pWnd)->sumEachGroupCount();
		}
		else
		{
			rowcount = GetRowValidcount();
		}

		if (rowcount >= MAX_LINE)
		{

			Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
			return;
		}

		insertInterest(row - 1);
		insertRow(row);
		SetLineColor();
		m_grid->memoCheck();
		//			m_grid->memoCheck(row+1);
		m_grid->memoRefresh();

		if (m_bAddCnd)
		{
			saveInterest();
		}
	}
	break;
	case userBASE + 9: // å���� �߰�
	{
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_bEditWork = true;

		CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
		int rowcount = 0;

		if (nOver == MO_VISIBLE)
		{
			pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
			rowcount = ((CGroupWnd *)pWnd)->sumEachGroupCount();
		}
		else
		{
			rowcount = GetRowValidcount();
		}

		if (rowcount >= MAX_LINE)
		{

			Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
			break;
		}

		insertInterest(row - 1);
		insertRow(row);
		SetLineColor();
		m_grid->memoCheck();
		m_grid->memoRefresh();

		const UINT attr = m_grid->GetItemAttr(row, colNAME) | GVAT_MARKER;
		m_grid->SetItemAttr(row, colNAME, attr);


		const int nCount = _vInters.size();

		if (nCount > (row - 1))
		{
			auto& pInter = _vInters.at(row - 1);
			CString strMarker;
			strMarker.Format("m%05d", row);

			if (pInter)
			{
				pInter->gubn = 'm';
				pInter->code = strMarker;
			}

			m_grid->SetItemText(row, colCODE, strMarker);

			m_grid->SetFocusCellEdit(row, colNAME, true);

			if (m_bAddCnd)
			{
				saveInterest();
			}
		}
	}
	break;
	case userBASE + 10: // å���ǻ���
	{
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_bEditWork = true;

		if (gsl::narrow_cast<int>(_vInters.size()) >= row - 1)
		{
			if (m_grid->GetItemAttr(row, colNAME) & GVAT_MARKER)
			{
				DeleteRow(row);
				insertInterest(MAX_LINE - 1);
			}
		}
	}
	break;
	case userBASE + 11: //�޸� ���� 091013 ����
	{
		if (MessageBox("�޸� �����Ͻðڽ��ϱ�?\n", "�޸� ���� Ȯ��â", MB_YESNO) == IDYES)
		{
			m_grid->delectMemo(row);
		}
	}
	break;
	case userBASE + 12: //�ϸ�ũ ���� 091102
	{
		const UINT uAttr = m_grid->GetItemAttr(row, colNAME);
		m_grid->SetItemAttr(m_drop, colNAME, uAttr);

		if (GVAT_MARKER & uAttr)
		{
		}
		else
		{
			setBookmark(row);
		}
	}
	break;

	case userBASE + 13: //�ϸ�ũ ����
	{
		const UINT uAttr = m_grid->GetItemAttr(row, colNAME);
		m_grid->SetItemAttr(m_drop, colNAME, uAttr);

		if (GVAT_MARKER & uAttr)
		{
		}
		else
		{
			deleteBookmark(row);
		}
	}
	break;
	case userBASE + 14: //�������簡2�� ���� �����ϱ�
		sendtoMutiHoga();
		break;
		// END ADD
		// END ADD
	}
}

void CGridWnd::InsertEmpty(int row)
{
	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	int rowcount = 0;

	if (nOver == MO_VISIBLE)
	{
		pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
		rowcount = ((CGroupWnd *)pWnd)->sumEachGroupCount();
	}
	else
	{
		rowcount = GetRowValidcount();
	}

	if (rowcount >= MAX_LINE)
	{

		Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
		return;
	}

	DeleteRow(MAX_LINE);
	insertRow(row);
}

void CGridWnd::hawkEyeColor(int row)
{
	double curr{}, open{}, high{}, low{};
	double pcurr{}, popen{}, phigh{}, plow{}, dval{};

	curr = IH::TOfabs(m_grid->GetItemText(row, colCURR));
	open = IH::TOfabs(m_grid->GetItemText(row, colOPEN));
	high = IH::TOfabs(m_grid->GetItemText(row, colHIGH));
	low = IH::TOfabs(m_grid->GetItemText(row, colLOW));

	pcurr = IH::TOfabs(m_grid->GetItemText(row, colPCURR));
	popen = IH::TOfabs(m_grid->GetItemText(row, colPOPEN));
	phigh = IH::TOfabs(m_grid->GetItemText(row, colPHIGH));
	plow = IH::TOfabs(m_grid->GetItemText(row, colPLOW));

	if (curr <= 0)
		curr = pcurr;
	if (curr <= 0)
		return;

	// colNAME foreground
	if (open >= phigh) // ��°�
	{
		m_grid->SetItemFgColor(row, colNAME, colorKKRED);
	}
	else if (open <= plow) // �϶���
	{
		m_grid->SetItemFgColor(row, colNAME, colorKKBLUE);
	}
	else
	{
		m_grid->SetItemFgColor(row, colNAME, GetAxColor(69));
	}

	// colNAME background
	if (curr >= high)
	{
		m_grid->SetItemBkColor(row, colNAME, colorLTRED);
	}
	else if (curr <= low)
	{
		m_grid->SetItemBkColor(row, colNAME, colorLTBLUE);
	}
	else
	{
		m_grid->SetItemBkColor(row, colNAME, GetAxColor(68));
	}

	dval = (phigh - plow) / 3.;

	// colCURR background
	if (curr >= phigh)
	{
		m_grid->SetItemBkColor(row, colCURR, colorDKRED);
	}
	else if (curr >= (plow + dval * 2))
	{
		m_grid->SetItemBkColor(row, colCURR, colorLTRED);
	}
	else if (curr >= (plow + dval))
	{
		m_grid->SetItemBkColor(row, colCURR, GetAxColor(68));
	}
	else if (curr >= plow)
	{
		m_grid->SetItemBkColor(row, colCURR, colorLTBLUE);
	}
	else if (curr < plow)
	{
		m_grid->SetItemBkColor(row, colCURR, colorDKBLUE);
	}
	else // default
	{
		//		TRACE("check  error....\n");
	}
}

void CGridWnd::DeleteRow(int row)
{
	// 2011.11.09 KSJ
	// GetRowCount�� 2�̸� ���� Row�� �ϳ� ���� ���̹Ƿ� ����� �ȵ�.
	//����� Insert�� ���� ����.
	// row�� 0�̸� (ƼĿ)��..

	CString strCode;
	if (m_grid->GetRowCount() <= 2 || row <= 0)
		return;

	// 2012.12.18 KSJ ������ �ѻ��¿��� �����ϸ� �ش��ϴ� row�� m_inters�� ���� ���� �ʴ�.
	// m_inters�� ���� baseSorting �������� �Ǿ� �ֱ� ������.
	//�׷��� �ڵ带 ������ ��ġ�� ã�ƾ� �Ѵ�.
	strCode = m_grid->GetItemText(row, colCODE);

	m_grid->DeleteRow(row);
	m_grid->memoDelete(row);
	m_rowC--;

	// 2013.01.28 KSJ �ڵ带 ã�Ƽ� ����� �����϶��� �������� �ʴ´�.
	//�׷��� �ڵ尡 ���϶��� row -1�� �����.
	strCode.TrimLeft();
	strCode.TrimRight();
	if (!strCode.IsEmpty())
		RemoveInters(GetInter(strCode));
	else
		RemoveInters(row - 1);
	// 2013.01.28 KSJ End
	// 2012.12.18 KSJ End

	ReIndexing();

	ClearSearchMap(); // 2011.12.29 KSJ
	ReSetSearchMap(); // 2011.12.29 KSJ
}

void CGridWnd::insertInterest(int idx)
{
	auto& pinters = _vInters.emplace(_vInters.begin() + idx, std::move(std::make_shared<struct _intersx>()));
	(*pinters)->gubn = '0';
	(*pinters)->bookmark = '0'; // 2014.05.16 KSJ �⺻��

	// struct _intersx* pinters = new _intersx();
	// pinters->gubn = '0';
	// pinters->bookmark = '0'; // 2014.05.16 KSJ �⺻��
	// m_inters.InsertAt(idx, pinters);
}

void CGridWnd::initialGrid(bool size)
{

	if (m_nIndex == 0)
	{
		CintGrid::m_dropdata.Reset();
	}

	m_grid->Initial(m_rowC, m_colC, 1, 0); // colCURR + 1);//BUFFET SCROLL BAR


	// ADD PSH 20070911
	m_grid->SetMarginRatio(m_bMargin);
	m_grid->SetBkMarkerColor(m_clrMarkerBKG);
	m_grid->SetMarkerColor(m_clrMarkerTXT);
	m_grid->SetMarkerShadow(m_bShadow);
	m_grid->SetInfo(m_bInfo);
	////////////////////////////////////////////////////
	const char use = ((CGroupWnd *)m_pGroupWnd)->GetBkUse();
	if (use == '1' || use == '2')
	{
		int nLine = ((CGroupWnd *)m_pGroupWnd)->GetLine();
		const COLORREF bkColor = ((CGroupWnd *)m_pGroupWnd)->GetBkColor1();
		const COLORREF bkCol2 = ((CGroupWnd *)m_pGroupWnd)->GetBkColor2();

		if (nLine <= 0)
			nLine = 1;
		m_grid->SetStepColor(nLine, bkColor, bkCol2); // BUFF
	}
	else
		m_grid->SetStepColor(1, GetAxColor(68), GetAxColor(77)); // BUFF
	m_nRowHeight = m_grid->GetRowHeight(0);
	// END ADD

	LOGFONT lf{}, lfB{};
	const int fontsize = ((CGroupWnd *)m_pGroupWnd)->GetFontSize();
	CString fontname = ((CGroupWnd *)m_pGroupWnd)->GetFontName();
	// MODIFY PSH 20070917
	// BOOL	bBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();
	const BOOL bCurrBold = ((CGroupWnd *)m_pGroupWnd)->GetCurrBold();
	const BOOL bNameBold = ((CGroupWnd *)m_pGroupWnd)->GetNameBold();
	const BOOL bAllBold = ((CGroupWnd *)m_pGroupWnd)->GetAllBold();
	// END MODIFY

	m_pFont = GetAxFont(fontname, fontsize);
	m_pFontB = GetAxFont(fontname, fontsize, true);

	//m_pFont->GetLogFont(&lf);
	//lf.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);
	//m_pFontB->GetLogFont(&lfB);
	//lfB.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	GVITEM gvitem;
	_gridHdr gridHdr{};

	//////////////////////////////////////////////////////////////////////////
	// 2013.03.27 KSJ �۲�ũ�⿡ ���� ������ ����
	CString IniFile;
	int nResize = 0;
	int readL = 0;
	char readB[1024]{};

	IniFile.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_FIELD);

	nResize = GetPrivateProfileInt("INTERFIELD", "RESIZE", 0, IniFile);
	readL = GetPrivateProfileString("INTERFIELD", "FONTPOINT", "", readB, sizeof(readB), IniFile);

	// 2013.04.30 KSJ nResize�� 0�̾ƴϰ� false�϶� �������� ���� �ʴ´�.
	//�׷��� �����׷� �� ����â�� ������� ���� �����ȴ�.
	if (nResize != 0 && size == false)
		return;
	//////////////////////////////////////////////////////////////////////////
	// column header setting
	for (int ii = 0; ii < m_colC; ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);

		gvitem.row    = 0;
		gvitem.col    = ii;
		gvitem.state  = 0;
		gvitem.mask   = GVMK_TEXT | GVMK_FORMAT | GVMK_FONT | GVMK_ATTR | GVMK_FGCOLOR | GVMK_BKCOLOR | GVMK_MISC;
		gvitem.format = GVFM_HEADER;
		gvitem.fgcol  = GetAxColor(76);
		gvitem.bkcol  = GetAxColor(74);

		//CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));	
		gvitem.font = m_pFont;

		if (gridHdr.stid > 0)
			gvitem.text = IH::idTOstring(gridHdr.stid);
		gvitem.attr = gridHdr.attr;
		m_grid->SetItem(&gvitem);

		if (ii == colCODE)
		{
			m_grid->SetColumnWidth(ii, m_bDispCode ? m_nCodeWidth : 0);
		}
		else if (ii == colSIG)
		{
			// 2011.12.02 KSJ

			m_grid->SetColumnWidth(ii, m_bNewsNChart ? m_nNewsNChartWidth : 0);

			if (m_bNewsNChart)
				m_grid->SetColumnWidth(ii, ((CGroupWnd *)GetParent())->GetRowHeight());
			// KSJ
		}
		else
		{
			if (size)
				m_grid->SetColumnWidth(ii, gridHdr.width);
		}
	}

	for (int ii = 0; ii < m_colC; ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);

		gvitem.state = 0;
		gvitem.mask = GVMK_FORMAT | GVMK_FONT | GVMK_ATTR | GVMK_FGCOLOR | GVMK_BKCOLOR | GVMK_SYMBOL;

		gvitem.format = gridHdr.format;
		gvitem.symbol = gridHdr.symbol;
		gvitem.attr = gridHdr.attrx;

		gvitem.fgcol = GetAxColor(69);
		// MODIFY PSH 20070912
		// gvitem.bkcol = GetAxColor(68);
		gvitem.bkcol = ((CGroupWnd *)m_pGroupWnd)->GetRTMColor();
		// END MODIFY

	//	CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		gvitem.font = m_pFont;

		// MODIFY PSH 20070911
		if ((ii == colCODE) || (ii == colNAME) || (ii == colCURR) || bAllBold)
		// END MODIFY
		{					   // 20070706 kwon
			if (gvitem.symbol.Compare("2024")) // 2012.11.29 KSJ ����϶��� GVAT_CONDITIONx�ϸ� �������� �ȵ�
				gvitem.attr |= GVAT_CONDITIONx;

			// MODIFY PSH 20070917
			// if (bBold)
			if ((ii == colCODE) || (ii == colCURR && bCurrBold) || (ii == colNAME && bNameBold) || bAllBold)
			// END MODIFY
			{
				//CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));
				gvitem.font = m_pFontB;
			}
		}

		gvitem.bkcol = GetAxColor(68);

		if (ii == colNAME)
		{
			gvitem.mask |= GVMK_PARAM;
			gvitem.param = colCODE;
		}

		m_grid->SetColumnItems(ii, &gvitem);

		if (ii == colCODE)
		{
			m_grid->SetColumnWidth(ii, m_bDispCode ? m_nCodeWidth : 0);

			gvitem.mask |= GVMK_PARAM;
			gvitem.param = colCODE;
		}
		else if (ii == colSIG)
		{
			// 2011.12.02 KSJ
			m_grid->SetColumnWidth(ii, m_bNewsNChart ? m_nNewsNChartWidth : 0);

			gvitem.mask |= GVMK_PARAM;
			gvitem.param = colSIG;

			if (m_bNewsNChart)
			{
				m_grid->SetColumnWidth(ii, ((CGroupWnd *)GetParent())->GetRowHeight());
			}
			// KSJ
		}
		else
		{
			if (size)
			{
				// 				CString strTemp;

				if (nResize < 0) //�� ���� ���̱�
				{
					m_grid->SetColumnWidth(ii, gsl::narrow_cast<int>(gridHdr.width * (1.0 + abs(nResize) / 20.0) + 0.5));
				}
				else if (nResize > 0) //�� ���� ���̱�
				{
					m_grid->SetColumnWidth(ii, gsl::narrow_cast<int>(gridHdr.width / (1.0 + abs(nResize) / 20.0) + 0.5));
				}
				else
				{
					m_grid->SetColumnWidth(ii, gridHdr.width);
				}
			}
		}
	}

	// 2011.12.12 KSJ
	SetDispCode(m_bDispCode);
	SetNewsNChart(m_bNewsNChart);
	SetLineColor();
	// ADD PSH 20070918
	m_grid->SetBkColor(RGB(255, 255, 255)); // BUFFET FOR SAME BACKCOLOR
	m_grid->SetFixBkColor(GetAxColor(74));
	// END ADD
	m_grid->SetBKSelColor(GetAxColor(78));
	m_grid->SetRowColResize(FALSE, TRUE);
	m_grid->SetKeepingColumn(colCURR + 1); // keep column count
	m_grid->SetGridFocusLine(FALSE, TRUE);
	m_grid->SetGridColor(GetAxColor(65));
	m_grid->SetConditionColor(GetAxColor(CLR_UP), GetAxColor(CLR_DN), GetAxColor(CLR_TEXT));
	m_grid->SetTitleTips(TRUE);
	m_grid->Adjust();

}

// ADD PSH 20070912
void CGridWnd::InitPopupMenu()
{
	m_menuSubHeader.CreatePopupMenu();
	m_menuSubHeader.AppendMenu(MF_STRING, IDC_MENU_SAVE_CND1, "�׸��߰��� �ڵ�����");
	m_menuSubHeader.AppendMenu(MF_STRING, IDC_MENU_SAVE_CND2, "�׸񺯰�� �ڵ�����");

	m_menuHeader.CreatePopupMenu();
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_INTERSAVE, "�������� ����ϱ�");
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_INTERTAKE, "�������� �����");
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_BASESORT, "��ϼ����� ����");

	m_menuHeader.AppendMenu(MF_SEPARATOR);
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_MULTICHART, "�������� ��Ƽ��Ʈ ����");
	m_menuHeader.AppendMenu(MF_SEPARATOR);
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_NEWSNCHART, "����/��Ʈ ����"); // 2011.12.02 KSJ
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_CODE, "�����ڵ� ����");
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_INFO, "����Ư�̻��� ����");
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_MARGIN, "�ֽ����ű� ����");
//	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_TICKER, "����ƼĿ ����");
	m_menuHeader.AppendMenu(MF_SEPARATOR);
//	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_TICKER_SETUP, "����ƼĿ ����");
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_SAVE_CND1, "���Ǳ�� ����");

	m_menuHeader.CheckMenuItem(IDC_MENU_NEWSNCHART, m_bNewsNChart ? MF_CHECKED : MF_UNCHECKED); // 2011.12.02 KSJ
	m_menuHeader.CheckMenuItem(IDC_MENU_CODE, m_bDispCode ? MF_CHECKED : MF_UNCHECKED);
	m_menuHeader.CheckMenuItem(IDC_MENU_INFO, m_bInfo ? MF_CHECKED : MF_UNCHECKED);
	m_menuHeader.CheckMenuItem(IDC_MENU_MARGIN, m_bMargin ? MF_CHECKED : MF_UNCHECKED);
	// m_menuHeader.SetSelectDisableMode(TRUE);
}

void CGridWnd::SetInfo(BOOL bInfo /* = TRUE */)
{
	m_bInfo = bInfo;

	m_menuHeader.CheckMenuItem(IDC_MENU_INFO, m_bInfo ? MF_CHECKED : MF_UNCHECKED);

	m_grid->SetInfo(m_bInfo);

	CRect rcWnd;
	GetWindowRect(rcWnd);
	ScreenToClient(rcWnd);

	InvalidateRect(rcWnd);
}
// KSJ

void CGridWnd::ShowPopupMenu(int nX /* = -1 */, int nY /* = -1 */)
{
	if ((nX == -1) || (nY == -1))
	{
		CPoint point;
		GetCursorPos(&point);

		nX = point.x;
		nY = point.y;
	}

	int nResult{};

	if (m_bPopupDlg)
	{
		CPopupDlg dlg;
		dlg.m_nX = nX;
		dlg.m_nY = nY;

		const int nResultID[] = {
		    IDC_MENU_SAVE_CND1,
		    IDC_MENU_SAVE_CND2,
		    IDC_MENU_INTERSAVE,
		    IDC_MENU_INTERTAKE,
		    IDC_MENU_BASESORT,
		    IDC_MENU_MULTICHART,
		    IDC_MENU_INFO,
		    IDC_MENU_MARGIN,
		    IDC_MENU_TICKER,
		    IDC_MENU_TICKER_SETUP,
		    IDC_MENU_SAVE_CND1,
		};

		constexpr int nBase = 100;

		dlg.AddMenu(nBase + 2, "�������� ����ϱ�");

		if (!m_szTitle.IsEmpty())
			dlg.AddMenu(nBase + 3, "�������� �����");

		dlg.AddMenu(nBase + 4, "��ϼ����� ����");
		//		dlg.AddMenu(nBase + 12, "���������� ����");
		//		dlg.AddMenu(nBase + 13, "���������� ����");
		//		dlg.AddMenu(nBase + 14, "�����ڵ����� ����");

		dlg.AddMenu(nBase + 5, "�������� ��Ƽ��Ʈ ����");
		//		dlg.AddMenu(nBase + 6, "ü�Ḯ��Ʈ ����");

		dlg.AddMenu(nBase + 7, "����Ư�̻��� ����", m_bInfo);
		dlg.AddMenu(nBase + 8, "�ֽ����ű� ����", m_bMargin);

		/*
				dlg.AddMenu(nBase + 9, "����ƼĿ ����", m_bTicker);
		*/

//		dlg.AddMenu(nBase + 10, "����ƼĿ ����");
		dlg.AddMenu(nBase + 11, "���Ǳ�� ����");

		if (-1 == dlg.DoModal())
			return;

		nResult = nResultID[dlg.m_nResult - nBase];
	}
	else
	{
		m_menuHeader.EnableMenuItem(IDC_MENU_INTERTAKE, m_szTitle.IsEmpty() ? MF_DISABLED : MF_ENABLED);
		m_menuHeader.EnableMenuItem(IDC_MENU_MARKER, m_bSorting ? MF_DISABLED : MF_ENABLED);

		nResult = m_menuHeader.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, nX, nY, this);
	}

	switch (nResult)
	{
	case IDC_MENU_INTERSAVE:
	{
		saveInterestX();
	}
	break;
	case IDC_MENU_INTERTAKE:
	{
		saveInterest();
	}
	break;
	case IDC_MENU_BASESORT:
	{
		BaseSorting();
	}
	break;
	case IDC_MENU_UPJONGSORT:
	{
	}
	break;
	case IDC_MENU_NAMESORT:
	{
	}
	break;
	case IDC_MENU_CODESORT:
	{
	}
	break;
	case IDC_MENU_MULTICHART:
	{
		MultiChart();
	}
	break;
		// 2011.12.02 KSJ ������ ��Ʈ ���̱�
	case IDC_MENU_NEWSNCHART:
	{
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_NEWSNCHART, !m_bNewsNChart);
		m_menuHeader.CheckMenuItem(IDC_MENU_NEWSNCHART, m_bNewsNChart ? MF_CHECKED : MF_UNCHECKED);
	}
	break;
		// KSJ

	case IDC_MENU_CODE:
	{
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_MARKETCODE, !m_bDispCode);
		m_menuHeader.CheckMenuItem(IDC_MENU_CODE, m_bDispCode ? MF_CHECKED : MF_UNCHECKED);
	}
	break;
	case IDC_MENU_INFO:
	{
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_INFO, !m_bInfo);
		m_menuHeader.CheckMenuItem(IDC_MENU_INFO, m_bInfo ? MF_CHECKED : MF_UNCHECKED);
		m_grid->SetInfo(m_bInfo);
	}
	break;
	case IDC_MENU_MARGIN:
	{
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_MARGIN, !m_bMargin);
		m_menuHeader.CheckMenuItem(IDC_MENU_MARGIN, m_bMargin ? MF_CHECKED : MF_UNCHECKED);
		m_grid->SetMarginRatio(m_bMargin);
		m_grid->Invalidate(false);
	}
	break;
	case IDC_MENU_TICKER:
	{
	}
	break;
	case IDC_MENU_TICKER_SETUP:
	{
		Csetup dlg(nullptr, &m_tkConfig);
		dlg.DoModal();
	}
	break;
	case IDC_MENU_SAVE_CND1:
	{
		CUsefulSetupDlg dlg;

		dlg.m_bClrApply = m_bMKClrApply;
		dlg.m_clrKospi = m_clrKospi;
		dlg.m_clrKosdaq = m_clrKosdaq;

		dlg.m_clrMarkerBKG = m_clrMarkerBKG;
		dlg.m_clrMarkerTXT = m_clrMarkerTXT;
		dlg.m_bShadow = m_bShadow;
		dlg.m_bMKAllApply = m_bAllApply;

		dlg.m_strFile = m_strWaveFile;
		dlg.m_bWaveApply = m_bWaveApply;

		dlg.m_bAllApply = m_bAutoSaveAllApply;
		dlg.m_bAddCnd = m_bAddCnd;
		dlg.m_bChgCnd = m_bChgCnd;

		if (dlg.DoModal())
		{
			if (-1 != dlg.m_strFile.Find("psh0419"))
			{
				CString strTemp;
				if (nullptr == m_pRCDlg)
				{
					m_pRCDlg = std::make_unique<CRealCheckDlg>();
					m_pRCDlg->Create(IDD_REALTIME_CHECK);
					m_pRCDlg->ShowWindow(SW_SHOW);

					m_pRCDlg->m_stcTitle.SetWindowText(m_szTitle);
					m_pRCDlg->m_pGridWnd = this;
				}
				else
				{
					m_pRCDlg->ShowWindow(SW_SHOW);
				}

				const int nCnt = m_pRCDlg->m_listGrid.GetCount();

				for (int i = nCnt - 1; i >= 0; i--)
				{
					m_pRCDlg->m_listGrid.DeleteString(i);
				}

				m_pRCDlg->m_edSymbol.SetWindowText("");
				m_pRCDlg->m_edData.SetWindowText("");

				for (int i = 1; i < m_rowC; i++)
				{
					strTemp.Format("%03d : %s - %s", i, m_grid->GetItemText(i, colRCOD), m_grid->GetItemText(i, colCODE));
					m_pRCDlg->m_listGrid.AddString(strTemp);
				}

				break;
			}

			if (dlg.m_bClrApply != m_bMKClrApply ||
			    dlg.m_clrKospi != m_clrKospi ||
			    dlg.m_clrKosdaq != m_clrKosdaq)
			{
				m_pGroupWnd->SendMessage(WM_MANAGE, MK_MARKETCOLOR, (LPARAM)&dlg);
			}

			if (dlg.m_clrMarkerBKG != m_clrMarkerBKG ||
			    dlg.m_clrMarkerTXT != m_clrMarkerTXT ||
			    dlg.m_bShadow != m_bShadow ||
			    dlg.m_bMKAllApply != m_bAllApply)
			{
				_marker mkProp;
				mkProp.clrMarkerBKG = dlg.m_clrMarkerBKG;
				mkProp.clrMarkerTXT = dlg.m_clrMarkerTXT;
				mkProp.bShadow = dlg.m_bShadow;
				mkProp.bAllApply = dlg.m_bMKAllApply;

				if (dlg.m_bMKAllApply)
				{
					m_pGroupWnd->SendMessage(WM_MANAGE, MK_MKPROP, (LPARAM)&mkProp);
				}
				else
				{
					SetMarkerProp(&mkProp);
				}
			}

			if (dlg.m_strFile != m_strWaveFile ||
			    dlg.m_bWaveApply != m_bWaveApply)
			{
				m_pGroupWnd->SendMessage(WM_MANAGE, MK_ALERTWAVE, (LPARAM)&dlg);
			}

			if (dlg.m_bAllApply != m_bAutoSaveAllApply ||
			    dlg.m_bAddCnd != m_bAddCnd ||
			    dlg.m_bChgCnd != m_bChgCnd)
			{
				m_pGroupWnd->SendMessage(WM_MANAGE, MK_AUTOSAVE, (LPARAM)&dlg);
			}
		}
	}
	break;
	}
}

bool CGridWnd::GetCodeMap(CMap<CString, LPCTSTR, int, int> &arCodeMap)
{
	CString code;
	if (m_rowC <= 1)
		return false;

	for (int ii = 1; ii < m_rowC; ii++)
	{
		code = m_grid->GetItemText(ii, colCODE);
		if (code != "")
		{
			arCodeMap.SetAt(code, ii);
		}
	}

	return true;
}

void CGridWnd::BaseSorting()
{
	CMap<CString, LPCTSTR, int, int> mapBaseSort, mapCurCode;
	CString strIndex, strCode;

	const int nCnt = _arrBaseInters.size();
	if (1 > nCnt)
		return;
	if (!GetCodeMap(mapCurCode))
		return;

XMSG("�����۵��ϴ� ?");
	int ii = 0;
	for_each(_arrBaseInters.begin(), _arrBaseInters.end(), [&](auto& item) {
		if (item->code.IsEmpty())
			mapBaseSort.SetAt(strCode, ii++);
	});

	//_intersx *pInter = nullptr;
	//for (int i = 0; i < nCnt; i++)
	//{
	//	if (pInter = m_arrBaseInters.GetAt(i))
	//	{
	//		strCode = pInter->code;
	//	}

	//	if (!strCode.IsEmpty())
	//	{
	//		mapBaseSort.SetAt(strCode, i);
	//	}
	//}

	m_grid->SortBase(mapBaseSort, mapCurCode);
	m_bSorting = FALSE;
}

void CGridWnd::SetLineColor()
{
	for (int ii = 0; ii < m_grid->GetRowCount(); ii++)
	{
		SetLineColor(ii);
	}
}

void CGridWnd::SetLineColor(int nRow, CintGrid *pGrid)
{
	if (pGrid == nullptr)
		pGrid = m_grid.get();

	COLORREF bkColor{};
	pGrid->SetRowHeight(nRow, ((CGroupWnd *)m_pGroupWnd)->GetRowHeight());

	if (!nRow)
		return;

	bkColor = ((CGroupWnd *)m_pGroupWnd)->GetBkColor1();

	if (nRow > 1 && ((nRow - 1) / ((CGroupWnd *)m_pGroupWnd)->GetLine()) % 2)
		bkColor = ((CGroupWnd *)m_pGroupWnd)->GetBkColor2();

	for (int jj = 0; jj < m_colC; jj++)
	{
		if (jj == colNAME)
			pGrid->SetItemFgColor(nRow, colNAME, GetAxColor(69));

		pGrid->SetItemBkColor(nRow, jj, bkColor);
	}
}
// END MODIFY

//�ϸ�ũ �� �����ϱ�
void CGridWnd::setBookmark(int row)
{
	LOGFONT lf{}, lfB{};
	const int fontsize = ((CGroupWnd *)m_pGroupWnd)->GetFontSize();
	CString fontname = ((CGroupWnd *)m_pGroupWnd)->GetFontName();

	const BOOL bCurrBold = ((CGroupWnd *)m_pGroupWnd)->GetCurrBold();
	const BOOL bNameBold = ((CGroupWnd *)m_pGroupWnd)->GetNameBold();
	const BOOL bAllBold = ((CGroupWnd *)m_pGroupWnd)->GetAllBold();

	m_pFont = GetAxFont(fontname, fontsize);
	m_pFontB = GetAxFont(fontname, fontsize, true);

	m_pFont->GetLogFont(&lf);
	lf.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	m_pFontB->GetLogFont(&lfB);
	lfB.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	GVITEM gvitem, gvitemnew;
	_gridHdr gridHdr{};
	for (int ii = 0; ii < m_colC; ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);

		gvitem.row = row;
		gvitem.col = ii;

		// m_grid->GetItem(&gvitem);
		gvitem.state = 0;
		gvitem.mask = GVMK_FORMAT | GVMK_FONT | GVMK_ATTR | GVMK_FGCOLOR | GVMK_BKCOLOR | GVMK_SYMBOL;

		gvitem.format = gridHdr.format;
		gvitem.symbol = gridHdr.symbol;
		gvitem.attr = gridHdr.attrx;
		gvitem.fgcol = GetAxColor(69);
		gvitem.bkcol = ((CGroupWnd *)m_pGroupWnd)->GetBookMarkColor();
		//
		//CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		gvitem.font = m_pFont;
		if ((ii == colCODE) || (ii == colNAME) || (ii == colCURR) || bAllBold)
		{					   // 20070706 kwon
			if (gvitem.symbol.Compare("2024")) // 2012.11.29 KSJ ����϶��� GVAT_CONDITIONx�ϸ� �������� �ȵ�
				gvitem.attr |= GVAT_CONDITIONx;

			// MODIFY PSH 20070917
			// if (bBold)
			if ((ii == colCODE) || (ii == colCURR && bCurrBold) || (ii == colNAME && bNameBold) || bAllBold)
			{
				//CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));
				gvitem.font = m_pFontB;
			
			}
		}
		if (ii == colNAME)
		{
			gvitem.mask |= GVMK_PARAM;
			gvitem.param = colCODE;
		}

		if (ii == colCODE)
		{
			gvitem.mask |= GVMK_PARAM;
			CString string = m_grid->GetItemText(row, colCODE);

			int result = 2;
			if (string.GetLength() == 6)
			{
				result = m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)string);

				if (result == 1)
				{
					result = 1;
				}
				else if (result == 2)
				{
					result = 0;
				}
				else
				{
					result = 2;
				}
			}

			gvitem.param = (LPARAM)result;
		}

		m_grid->SetItem(&gvitem);
	}

	m_grid->SetGridFocusLine(FALSE, TRUE);

	const int count = _vInters.size();
	if (count > (row - 1))
	{
		auto pInter = _vInters.at(row - 1);
		pInter->bookmark = '1';

		m_nBookMarkRow = row - 1;
		saveInterest(false, -1, true); // 2013.05.14 KSJ �ϸ�ũ�����Ҷ��� ���� ������ ������ ����
	}
	m_grid->Adjust();
}

void CGridWnd::deleteBookmark(int row)
{
	LOGFONT lf{}, lfB{};
	const int fontsize = ((CGroupWnd *)m_pGroupWnd)->GetFontSize();
	CString fontname = ((CGroupWnd *)m_pGroupWnd)->GetFontName();

	const BOOL bCurrBold = ((CGroupWnd *)m_pGroupWnd)->GetCurrBold();
	const BOOL bNameBold = ((CGroupWnd *)m_pGroupWnd)->GetNameBold();
	const BOOL bAllBold = ((CGroupWnd *)m_pGroupWnd)->GetAllBold();

	m_pFont = GetAxFont(fontname, fontsize);
	m_pFontB = GetAxFont(fontname, fontsize, true);

	m_pFont->GetLogFont(&lf);
	lf.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	m_pFontB->GetLogFont(&lfB);
	lfB.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	GVITEM gvitem;
	_gridHdr gridHdr{};
	for (int ii = 0; ii < m_colC; ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);

		gvitem.row = row;
		gvitem.col = ii;

		//		m_grid->GetItem(&gvitem);
		gvitem.state = 0;
		gvitem.mask = GVMK_FORMAT | GVMK_FONT | GVMK_ATTR | GVMK_FGCOLOR | GVMK_BKCOLOR | GVMK_SYMBOL;

		gvitem.format = gridHdr.format;
		gvitem.symbol = gridHdr.symbol;
		gvitem.attr = gridHdr.attrx;
		gvitem.fgcol = GetAxColor(69);
		gvitem.bkcol = GetAxColor(68);
		//
		//CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		gvitem.font = m_pFont;

		if ((ii == colCODE) || (ii == colNAME) || (ii == colCURR) || bAllBold)
		{					   // 20070706 kwon
			if (gvitem.symbol.Compare("2024")) // 2012.11.29 KSJ ����϶��� GVAT_CONDITIONx�ϸ� �������� �ȵ�
				gvitem.attr |= GVAT_CONDITIONx;

			// MODIFY PSH 20070917
			// if (bBold)
			if ((ii == colCODE) || (ii == colCURR && bCurrBold) || (ii == colNAME && bNameBold) || bAllBold)
			{
				//CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));
				gvitem.font = m_pFontB;
			}
		}
		if (ii == colNAME)
		{
			gvitem.mask |= GVMK_PARAM;
			gvitem.param = colCODE;
		}

		if (ii == colCODE)
		{
			gvitem.mask |= GVMK_PARAM;
			CString string = m_grid->GetItemText(row, colCODE);

			int result = 2;
			if (string.GetLength() == 6)
			{
				result = m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)string);

				if (result == 1)
				{
					result = 1;
				}
				else if (result == 2)
				{
					result = 0;
				}
				else
				{
					result = 2;
				}
			}

			gvitem.param = (LPARAM)result;
		}

		m_grid->SetItem(&gvitem);
	}

	m_grid->SetGridFocusLine(FALSE, TRUE);
	const int count = _vInters.size();
	
	if (count > (row - 1))
	{
		//�ϸ�ũ ����
		auto& pInter = _vInters.at(row - 1);
		pInter->bookmark = '0';
		m_nBookMarkRow = row - 1;
		saveInterest(false, -1, true); // 2013.05.14 KSJ �ϸ�ũ�����Ҷ��� ���� ������ ������ ����
		sendTransaction(row - 1);
	}
	m_grid->Adjust();
}

COLORREF CGridWnd::GetBookMarkColor()
{
	return ((CGroupWnd *)m_pGroupWnd)->GetBookMarkColor();
}

COLORREF CGridWnd::GetBookMarkPenColor()
{
	return ((CGroupWnd *)m_pGroupWnd)->GetBookMarkPenColor();
}

int CGridWnd::insertRow2(int afterRow, int beforeRow, BOOL bInsertInter)
{
	if (afterRow > m_grid->GetRowCount())
		afterRow = m_grid->GetRowCount();

	m_rowC++;


	const int ret = m_grid->InsertRow("", afterRow);
	const int count = _vInters.size();

	if (count <= beforeRow)
		return ret;

	auto& pInter = _vInters.at(beforeRow);

	//	if (bInsertInter)
	//		insertInterest(row - 1);

	LOGFONT lf{}, lfB{};
	const int fontsize = ((CGroupWnd *)m_pGroupWnd)->GetFontSize();
	CString fontname = ((CGroupWnd *)m_pGroupWnd)->GetFontName();

	const BOOL bCurrBold = ((CGroupWnd *)m_pGroupWnd)->GetCurrBold();
	const BOOL bNameBold = ((CGroupWnd *)m_pGroupWnd)->GetNameBold();
	const BOOL bAllBold = ((CGroupWnd *)m_pGroupWnd)->GetAllBold();

	m_pFont = GetAxFont(fontname, fontsize);
	m_pFontB = GetAxFont(fontname, fontsize, true);

	//m_pFont->GetLogFont(&lf);
	//lf.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	//m_pFontB->GetLogFont(&lfB);
	//lfB.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	GVITEM gvitem;
	_gridHdr gridHdr{};
	for (int ii = 0; ii < m_colC; ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);

		gvitem.row = afterRow;
		gvitem.col = ii;
		gvitem.state = 0;
		gvitem.mask = GVMK_FORMAT | GVMK_FONT | GVMK_ATTR | GVMK_FGCOLOR | GVMK_BKCOLOR | GVMK_SYMBOL;

		gvitem.format = gridHdr.format;
		gvitem.symbol = gridHdr.symbol;
		gvitem.attr = gridHdr.attrx;
		gvitem.fgcol = GetAxColor(69);

		//�ϸ�ũ�Ǿ��ִ��� Ȯ��
		if (pInter->bookmark == '1')
		{
			gvitem.bkcol = ((CGroupWnd *)m_pGroupWnd)->GetBookMarkColor();
		}
		else
		{
			gvitem.bkcol = GetAxColor(68);
		}

		//CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		gvitem.font = m_pFont;

		if ((ii == colCODE) || (ii == colNAME) || (ii == colCURR) || bAllBold)
		{					   // 20070706 kwon
			if (gvitem.symbol.Compare("2024")) // 2012.11.29 KSJ ����϶��� GVAT_CONDITIONx�ϸ� �������� �ȵ�
				gvitem.attr |= GVAT_CONDITIONx;

			// MODIFY PSH 20070917
			if ((ii == colCODE) || (ii == colCURR && bCurrBold) || (ii == colNAME && bNameBold) || bAllBold)
			{
				//CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));
				gvitem.font = m_pFontB;
			}
		}
		if (ii == colNAME)
		{
			gvitem.param = colCODE;
		}

		if (ii == colCODE)
		{
			gvitem.mask |= GVMK_PARAM;
			CString string = m_grid->GetItemText(afterRow, colCODE);
			int result = 2;

			if (string.GetLength() == 6)
			{
				m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)string);

				if (result == 1)
				{
					result = 1;
				}
				else if (result == 2)
				{
					result = 0;
				}
				else
				{
					result = 2;
				}
			}

			gvitem.param = (LPARAM)result;
		}

		m_grid->SetItem(&gvitem);
	}

	m_grid->SetGridFocusLine(FALSE, TRUE);

	ClearSearchMap(); // 2011.12.29 KSJ
	ReSetSearchMap(); // 2011.12.29 KSJ

	return ret;
}

int CGridWnd::insertRow(int row, BOOL bInsertInter /* = TRUE */)
{
	// 2011.11.09 KSJ
	// row�� 0�̸� (ƼĿ)��..
	if (row <= 0)
		return 0;

	if (row > m_grid->GetRowCount())
		row = m_grid->GetRowCount();

	m_rowC++;

	LOGFONT lf{}, lfB{};
	const int fontsize = ((CGroupWnd *)m_pGroupWnd)->GetFontSize();
	CString fontname = ((CGroupWnd *)m_pGroupWnd)->GetFontName();
	const BOOL bCurrBold = ((CGroupWnd *)m_pGroupWnd)->GetCurrBold();
	const BOOL bNameBold = ((CGroupWnd *)m_pGroupWnd)->GetNameBold();
	const BOOL bAllBold = ((CGroupWnd *)m_pGroupWnd)->GetAllBold();

	m_pFont = GetAxFont(fontname, fontsize);
	m_pFontB = GetAxFont(fontname, fontsize, true);

	//m_pFont->GetLogFont(&lf);
	//lf.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);
	//m_pFontB->GetLogFont(&lfB);
	//lfB.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	if (row >= gsl::narrow_cast<int>(_vInters.size()))
	{
		return 0;
	}
	const int ret = m_grid->InsertRow("", row);
	auto& pInter = _vInters.at(row);

	GVITEM gvitem{};
	const COLORREF bookcolor = ((CGroupWnd*)m_pGroupWnd)->GetBookMarkColor();
	const COLORREF fontcolor = GetAxColor(69);
	const COLORREF color = GetAxColor(68);
	for (int ii = 0; ii < m_colC; ii++)
	{
		const _gridHdr gridHdr = m_gridHdrX.GetAt(ii);
		gvitem.row = row;
		gvitem.col = ii;
		gvitem.state = 0;
		gvitem.mask = GVMK_FORMAT | GVMK_FONT | GVMK_ATTR | GVMK_FGCOLOR | GVMK_BKCOLOR | GVMK_SYMBOL;
		gvitem.format = gridHdr.format;
		gvitem.symbol = gridHdr.symbol;
		gvitem.attr   = gridHdr.attrx & ~GVAT_MARKER;
		gvitem.fgcol  = fontcolor;

		//�ϸ�ũ�Ǿ��ִ��� Ȯ��
		if (pInter->bookmark == '1')
		{
			gvitem.bkcol = bookcolor;
		}
		else
		{
			gvitem.bkcol = color;
		}

		//CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		gvitem.font = m_pFont;

		if ((ii == colCODE) || (ii == colCURR) || (ii == colNAME) || bAllBold)
		{					   // 20070706 kwon
			if (gvitem.symbol.Compare("2024")) // 2012.11.29 KSJ ����϶��� GVAT_CONDITIONx�ϸ� �������� �ȵ�
				gvitem.attr |= GVAT_CONDITIONx;

			if ((ii == colCODE) || (ii == colCURR && bCurrBold) || (ii == colNAME && bNameBold) || bAllBold)
			{
				//CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));
				gvitem.font = m_pFontB;
			}
		}
		if (ii == colNAME)
		{
			gvitem.param = colCODE;
		}
		m_grid->SetItem(&gvitem);
	}

	m_grid->SetGridFocusLine(FALSE, TRUE);
	if (bInsertInter)
	{
		ClearSearchMap(); // 2011.12.29 KSJ
		ReSetSearchMap(); // 2011.12.29 KSJ
	}
	return ret;
}

void CGridWnd::parsingField(CString symbol, bool bDlgSetup /* = false*/)
{
	CString string;
	const int gridHdrC = sizeof(gridHdr) / sizeof(_gridHdr);
	m_gridHdrX.RemoveAll();

	if (CheckField(symbol)) //ȣ�� �޾ƾ� �ϴ� �ʵ� ������ �Ͽ��� ��� false , �ʵ尡 ������� TRUE
		m_bSkipRTS = false;
	else
		m_bSkipRTS = true;
	m_pMainWnd->SendMessage(WM_MANAGE, MK_ISSKIP, m_bSkipRTS);

	for (int ii = 0; ii < gridHdrC; ii++)
	{
		if (gridHdr[ii].needs == 2)
		{
			m_gridHdrX.Add(gridHdr[ii]);
		}
	}

	if (symbol.IsEmpty())
	{
		m_szFIELD = _T(""), m_szWIDTH = _T("");
		for (int ii = 0; ii < gridHdrC; ii++)
		{
			if (gridHdr[ii].needs == 1) // default
				m_gridHdrX.Add(gridHdr[ii]);
		}
		return;
	}

	if (!symbol.IsEmpty())
		IH::Parser(symbol, deliMITER); // field name ����

	m_posField = false;
	m_ccField = false;
	int nIndex = 0;
	CString sztmp = _T(""), sztmp2;
	CMapStringToString mapSymbol;
	CWordArray arWidth;
	CStringArray arSymbol;

	int ncnt = m_gridHdrX.GetSize();
	int wcnt = 0;
	struct _gridHdr gHdr
	{
	};
	bool bField = true;

	sztmp.Format("%s", symbol);
	m_szFIELD = sztmp;

	while (!sztmp.IsEmpty())
	{
		string = IH::Parser(sztmp, deliMITER);
		arSymbol.Add(string);
	}

	sztmp.Format("%s", m_szWIDTH);
	while (sztmp.GetLength())
	{
		string = IH::Parser(sztmp, ",");

		if (string.IsEmpty())
			continue;

		arWidth.Add(atoi(string));
	}

	wcnt = arWidth.GetSize();
	sztmp.Format("%s", m_szFIELD);

	while (sztmp.GetLength())
	{
		string = IH::Parser(sztmp, ",");
		if (string.IsEmpty())
			continue;

		mapSymbol.SetAt(string, "");
	}

	wcnt = mapSymbol.GetCount();

	if (!bDlgSetup && wcnt == arSymbol.GetSize())
	{
		bField = true;

		CString strSym;
		for (int ii = 0; ii < wcnt; ii++)
		{
			strSym = arSymbol.GetAt(ii);
			if (!mapSymbol.Lookup(strSym, sztmp2))
			{
				if ((strSym == "#1##") || (strSym == "#4##") ||
				    (strSym == "#2##") || (strSym == "#5##") ||
				    (strSym == "#3##") || (strSym == "#6##"))
				{
					continue;
				}

				bField = false;
				break;
			}
		}

		if (bField)
		{
			symbol.Format("%s", m_szFIELD);
			symbol.Replace(",", ";");
		}
	}

	while (!symbol.IsEmpty())
	{
		string = IH::Parser(symbol, deliMITER);
		if (string.IsEmpty())
			continue;

		for (int ii = 0; ii < gridHdrC; ii++)
		{

			if (gridHdr[ii].needs == 2)
				continue;

			if (string.Compare(gridHdr[ii].symbol))
				continue;

			nIndex = m_gridHdrX.Add(gridHdr[ii]);
			sztmp.Format("%d", gridHdr[ii].stid);

			if ((gridHdr[ii].needs == 1) || (gridHdr[ii].needs == 3))
			{
				if ((string.Compare("#1##") == 0) || (string.Compare("#4##") == 0))
				{
					strcpy(m_gridHdrX[nIndex].symbol, "2029");
				}
				else if ((string.Compare("#2##") == 0) || (string.Compare("#5##") == 0))
				{
					strcpy(m_gridHdrX[nIndex].symbol, "2030");
				}
				else if ((string.Compare("#3##") == 0) || (string.Compare("#6##") == 0))
				{
					strcpy(m_gridHdrX[nIndex].symbol, "2031");
				}
			}

			if (gridHdr[ii].needs == 9)
			{
				m_posField = true;

				if (string.Compare("###3") == 0)
					m_sonikField = nIndex;
				if (string.Compare("###4") == 0)
					m_suikField = nIndex;
			}

			if (gridHdr[ii].needs == 8)
				m_ccField = true;

			if (gridHdr[ii].needs == 7)
				m_bongField = nIndex;

			if (gridHdr[ii].needs == 10)
				m_yDayVolField = nIndex;

			if (gridHdr[ii].needs == 11)
				m_EqualizerField = nIndex;

			if (gridHdr[ii].needs == 12)
				m_recomDataField = nIndex;

			if (gridHdr[ii].needs == 13)
				m_recomBasicCostField = nIndex;

			if (gridHdr[ii].needs == 14)
				m_recomSuikrateField = nIndex;

			if (gridHdr[ii].needs == 18)
				m_recomJisuGubnField = nIndex;

			if (gridHdr[ii].needs == 19)
				m_upjongCode = nIndex;

			if (gridHdr[ii].needs == 21)
				m_newCurrField = nIndex;

			if (gridHdr[ii].needs == 22)
				m_beforeVolField = nIndex;

			// 2016.04.15 KSJ epb ��õ����
			if (gridHdr[ii].needs == 25)
				m_epbDataField = nIndex;

			break;
		}
	}

	CString strSymbol;
	if (bField)
	{
		ncnt = m_gridHdrX.GetSize();
		wcnt = arWidth.GetSize();

		if (((ncnt - colNAME - 3) == wcnt))
		{
			for (int ii = 0; ii < wcnt; ii++)
			{
				gHdr = m_gridHdrX.GetAt(ii + colNAME);
				gHdr.width = arWidth.GetAt(ii);

				m_gridHdrX.SetAt(ii + colNAME, gHdr);
			}
		}
	}
}

bool CGridWnd::loadField(int fno /* = -1*/, bool bDlgSetup /* = false*/, int iEqualizer)
{
	CString tempFile;
	tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);

	CFileFind finder;
	if (!finder.FindFile(tempFile))
	{
		parsingField("", bDlgSetup);
		m_colC = m_gridHdrX.GetSize();
		return true;
	}

	if (fno < 0)
		return false;

	int readL = 0;
	char readB[1024];
	CString string, fieldno;

	fieldno.Format("%02d", fno);

	readL = GetPrivateProfileString("FIELDS", fieldno, "", readB, sizeof(readB), tempFile);

	string = CString(readB, readL);
	if (iEqualizer == 0) //�Ϲ�
	{
		string += "2022;1915;";
	}
	else if (iEqualizer == 3) //��õ����
	{
		string = "�ʵ� 00;2024;2033;2027;7777;1910;1911;2022;1915;";
	}
	else if (iEqualizer == 4) // 2016.04.08 KSJ Epb����
	{
		string = "�ʵ� 00;2024;2033;2027;7777;1999;2022;1915;"; // 2016.04.15 KSJ 1�� epb ���� ��õ���ڸ� ǥ���Ѵ�.
	}

	string += "1918;";
	parsingField(string, bDlgSetup);
	m_colC = m_gridHdrX.GetSize();

	WritePrivateProfileString("FIELDS", "ACTIVE", fieldno, tempFile);
	return true;
}

void CGridWnd::SaveAsInterField()
{
	if (m_szFIELD.IsEmpty())
		return;

	CString tempFile;
	tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);

	CFileFind finder;
	if (!finder.FindFile(tempFile))
		return;

	const int nIndex = (int)m_pToolWnd->SendMessage(WM_MANAGE, MK_GETAFIELD);
	if (nIndex < 0)
		return;

	int readL = 0;
	char readB[1024]{};
	CString sField, sNewField, sFieldName, sFieldNo;

	sFieldNo.Format("%02d", nIndex);

	readL = GetPrivateProfileString("FIELDS", sFieldNo, "", readB, sizeof(readB), tempFile);
	sField = CString(readB, readL);
	const int index = sField.Find(deliMITER);
	if (index == -1)
		return;

	sFieldName = sField.Left(index);
	sField.Format("%s", m_szFIELD);
	sField.Replace(",", deliMITER);
	sNewField.Format("%s%s%s", sFieldName, deliMITER, sField);

	WritePrivateProfileString("FIELDS", sFieldNo, sNewField, tempFile);
}

void CGridWnd::parsingOubsUpjong(char *datB, int datL)
{
	const int gridHoL = sizeof(struct _gridHo);
	if (datL < gridHoL)
		return;

	CString string, stringx, entry;

	string = CString(&datB[gridHoL], datL - gridHoL);

	CString strCode;
	CString tempData, tempStr;

	if (!string.IsEmpty())
	{
		stringx = IH::Parser(string, PNEW);
		tempData = IH::Parser(stringx, PTAB);
		tempData = IH::Parser(stringx, PTAB);
		tempData = IH::Parser(stringx, PTAB);
		tempData = IH::Parser(stringx, PTAB);
		m_dKospi = atof(tempData);
		tempStr.Format("%.2f", m_dKospi);
		m_dKospi = atof(tempStr);

		stringx = IH::Parser(string, PNEW);
		tempData = IH::Parser(stringx, PTAB);
		tempData = IH::Parser(stringx, PTAB);
		tempData = IH::Parser(stringx, PTAB);
		tempData = IH::Parser(stringx, PTAB);
		m_dKosdaq = atof(tempData);
		tempStr.Format("%.2f", m_dKosdaq);
		m_dKosdaq = atof(tempStr);
	}
}

void CGridWnd::parsingOubs(char *datB, int datL)
{
	// KSJ ���ð� ����
	CString strTime = ((CMainWnd *)m_pMainWnd)->GetMarketTime();
	CString strEndTime = ((CMainWnd *)m_pMainWnd)->GetMarketEndTime();
	CString strTemp;
	CString strCurr;

	if (!strTime.IsEmpty())
	{
		strTime.Trim();
		strTime.Replace("+", "");
		strTime.Replace("-", "");
		strTime.Replace(" ", "");

		const char ch = 0x7e;
		strTemp = strTime.Mid(0, strTime.Find(ch));
		strTemp.Replace("��", "");
		strTemp.Replace("��", "");
		strTemp.Replace(" ", "");
		m_strBeginTime.Format("0%s%s00", strTemp.Left(1), strTemp.Mid(1, 2));

		m_strEndTime = CalMaketTime(strTime.Mid(strTime.Find(ch) + 1, strTime.GetLength()), true);

		strEndTime.Replace("��", "");
		strEndTime.Replace("��", "");
		strEndTime.Replace("~", "");
		strEndTime.Replace("+", "");
		strEndTime.Replace("-", "");
		strEndTime.Replace(" ", "");
		m_strBeginTimeEnd.Format("%s%s00", strEndTime.Left(2), strEndTime.Mid(2, 2));
		m_strEndTimeEnd.Format("%s%d59", strEndTime.Mid(4, 2), atoi(strEndTime.Mid(6, 2)) - 1);
		// ���ð� ����

		strTemp.Format("%s|%s|%s|%s", m_strBeginTime, m_strEndTime, m_strBeginTimeEnd, m_strEndTimeEnd);
		m_pMainWnd->SendMessage(WM_MANAGE, MK_MARKETTIME, (LPARAM)(char *)strTemp.operator LPCTSTR());
	}

	const int gridHoL = sizeof(struct _gridHo);
	if (datL < gridHoL)
		return;

	if (!m_bContinue || (m_bContinue && !m_bSecond))
	{
		if (m_seldrop < 0)
		{
			m_grid->Clear();
			m_grid->MessageToGrid(WM_VSCROLL, SB_TOP);
		}
	}

	CString str950, str951; // 2012.06.19 KSJ ���, ����
	CString string, stringx, entry, strField;
	CString bongdata = _T("");
	const BOOL bBold = ((CGroupWnd *)m_pGroupWnd)->GetCurrBold();

	string = CString(&datB[gridHoL], datL - gridHoL);
	m_arExist.RemoveAll();
	const int kind = CAST_TREEID(m_kind)->kind;
	LPARAM lParam = 0;

	int nStart{}, nEnd{};

	nStart = 0;
	nEnd = m_gridHdrX.GetSize();

	if (m_bContinue)
	{
		if (m_bSecond)
		{
			// 			m_bSecond = FALSE;
			// 			m_bContinue = FALSE;

			nStart = GRIDMAXNUM;
			nEnd = m_gridHdrX.GetSize();
		}
		else
		{
			m_mapCodeABR.RemoveAll();
			nStart = 0;
			nEnd = GRIDMAXNUM;
		}
	}

	if (kind == xISSUE)
	{
		lParam = MAKELPARAM(0, 1);
	}

	CString strCode, strMargin, strTodayVolumn, strData;
	CString tempData;
	//, recommand1, recommand2, recommand3, recommand4, recommand5;

	m_grid->beginDrawHolding();
	if (!string.IsEmpty())
	{
		for (size_t ii = 0; ii < _vInters.size(); ii++)
		{
			tempData = "";
		//	recommand1 = "";
		//	recommand2 = "";
		//	recommand3 = "";
		//	recommand4 = "";
		//	recommand5 = "";

			strTodayVolumn = "";
			stringx = IH::Parser(string, PNEW);
			strData = stringx;

			// 2013.07.04 KSJ �ʵ尡 GRIDMAXNUM(27)���� ������ 2�� ������ ��ȸ�Ѵ�.
			//�׷��⶧���� ù���� �����Ϳ��� 27��°������ �̾Ƽ� �޵����Ͷ� ���̸�
			//�����ѵ����Ͱ� �ȴ�.
			if (m_bContinue && m_bSecond)
			{
				CString strValue;
				strCode = m_grid->GetItemText(ii + 1, colCODE);

				if (m_mapCurValue.Lookup(strCode, strValue))
				{
					int nIndex = 0, nCount = 0;

					while (nIndex != -1)
					{
						nIndex = strValue.Find("\t", nIndex + 1);

						if (++nCount == GRIDMAXNUM)
						{
							strValue = strValue.Left(nIndex + 1);
							break;
						}
					}

					strValue += strData;
					m_mapCurValue.SetAt(strCode, strValue);
				}
			}
			// 2013.07.04 END

			if (!stringx.IsEmpty())
			{
				CString tempOnebyte = stringx.Left(1);
				tempOnebyte.TrimLeft();

				const int iHeadersymbol = 0; //�׸��� ����ɺ� ��

				for (int jj = nStart; jj < nEnd; jj++)
				{
					entry = IH::Parser(stringx, PTAB);
					strField = entry;

					strField.TrimRight();
					strField.TrimLeft();

					if (strField == "0" || strField == "-0" || strField == "0.00" || strField == "+0" || strField == "+0.00" || strField == "-0.00" || strField == ".00") // 2012.07.17 �ɼ��϶��� ��� �߰�
					{
						entry = " ";
					}

					if (jj == colCODE && !entry.IsEmpty())
					{
						CString sztmp1, sztmp2;
						sztmp1 = m_grid->GetItemText(ii + 1, 0);

						sztmp2.Format("%d", ii);
						m_arExist.SetData(sztmp1, sztmp2);
						if (m_seldrop < 0)
							m_grid->SetItemData(ii + 1, colSIG, lParam); // KSJ colSIG

						strCode = entry;

						// 2012.04.03 KSJ ó�� ���簡�� �����͵��� ����.
						m_mapCurValue.SetAt(strCode, strData);
					}

					const _gridHdr gridHdr = m_gridHdrX.GetAt(jj);
					auto& pinters = _vInters.at(ii);

					if (jj == colNAME)
					{
						UINT attr{};

						if (pinters->gubn == 'm')
						{
							entry.Format("%s", pinters->name);

							//å���� ���·� ����
							attr = m_grid->GetItemAttr(ii + 1, colNAME) | GVAT_MARKER;
							m_grid->SetItemAttr(ii + 1, colNAME, attr);
							m_grid->SetItemText(ii + 1, colCODE, pinters->code);
						}
						else
						{
							attr = m_grid->GetItemAttr(ii + 1, colNAME) & ~GVAT_MARKER;
							m_grid->SetItemAttr(ii + 1, colNAME, attr);
						}

						strMargin = m_grid->GetItemText(ii + 1, colMAR);

						strMargin += "|" + m_grid->GetItemText(ii + 1, colMAR1);

						m_mapMargin.SetAt(strMargin, strMargin);
						m_grid->SetItemData(ii + 1, colNAME, (long)(LPCTSTR)strMargin);
					}
					else if (jj == colMAR)
					{
						entry.TrimLeft();
					}
					else if (!entry.IsEmpty() && jj == colRATE && (gridHdr.attr & GVAT_HIDDEN))
					{
						int nType = 0;
						switch (entry[0])
						{
						case '1':
							nType = 1;
							break;
						case '+':
						case '2':
							nType = 2;
							break;
						case '4':
							nType = 4;
							break;
						case '-':
						case '5':
							nType = 5;
							break;
						default:
							nType = 3;
						}

						m_grid->SetItemData(ii + 1, jj, nType);
					}
					else if (jj == colCODE)
					{
						if (entry.GetLength() == 6)
						{
							switch (m_pView->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)entry))
							{
							case kospiType:
								m_grid->SetItemData(ii + 1, jj, 1);
								break;
							case kosdaqType:
								m_grid->SetItemData(ii + 1, jj, 0);
								break;
							default:
								m_grid->SetItemData(ii + 1, jj, 2);
							}
						}
						else
							m_grid->SetItemData(ii + 1, jj, 2);
					}

					if (!(gridHdr.attr & GVAT_HIDDEN) && !entry.IsEmpty())
					{
						if ((strcmp("2029", gridHdr.symbol) == 0) ||
						    (strcmp("2030", gridHdr.symbol) == 0) ||
						    (strcmp("2031", gridHdr.symbol) == 0))
						{
							if ((1 == gridHdr.needs) || (3 == gridHdr.needs))
							{
								CString strVal = entry;
								CString strDiff = strVal;

								if (strVal[0] == '+' || strVal[0] == '-')
								{
									strDiff = strVal.Mid(1);
								}

								double dDiffOpen = 0.0;
								double dPClose{}, dVal{};
								dVal = atof(strDiff);					 // 2017.01.25 KSJ GetBuffer(0) ����
								dPClose = atof((m_grid->GetItemText(ii + 1, colPCURR))); // 2017.01.25 KSJ GetBuffer(0) ����

								if (dVal != 0 && dPClose != 0)
								{
									dDiffOpen = (dVal - dPClose) / dPClose * 100;

									if (1 == gridHdr.needs)
									{
										if (0 > dDiffOpen)
										{
											entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
										}
										else
										{
											entry.Format("%s( %0.2f%c)", strVal, dDiffOpen, P_PER);
										}
									}
									else if (3 == gridHdr.needs)
									{
										if (0 > dDiffOpen)
										{
											entry.Format("%0.2f%c", dDiffOpen, P_PER);
										}
										else if (0 == dDiffOpen)
										{
											entry.Format(" %0.2f%c", dDiffOpen, P_PER);
										}
										else
										{
											entry.Format(" +%0.2f%c", dDiffOpen, P_PER);
										}
									}
								}
							}
						}
					}
					// END ADD

					// 2013.08.23 KSJ �������������̸� ����+, �϶�+, ���- �� ���� ��ȣ�� ǥ��������Ѵ�.
					if (jj == colCURR)
					{
						if (strCode.GetLength() == 8 && (strCode.GetAt(0) == '4' || strCode.GetAt(0) == 'D')) //�Ļ���ǰ �ڵ尳��
						{
							if (entry.GetAt(0) == ' ')	 //ó���� ��ĭ�̸� ���տ� -, + ǥ�ð� �ִ� ���̴�.
								entry.Replace(" ", "0"); //�׶��� '��' �� ���� 1�� �� �־��ش�. �����̽� �ƴ�..
						}
					}
					// 2013.08.23 KSJ END
					// khs

					CString strsym, sEntry, sLog, sCode;
					strsym = (CString)gridHdr.symbol;
					if (strsym == "5401" || strsym == "5402" || strsym == "5403" || strsym == "5404" || strsym == "5405")
					{
						strCurr = m_grid->GetItemText(ii + 1, colCURR);
						strCurr.Replace("+", "");
						strCurr.Replace("-", "");

						CMapStringToString *m_symmap{};
						sCode.Format("%s", strCode);
						sCode.TrimRight();
						sCode.TrimLeft();

						if (!sCode.IsEmpty() && sCode.GetLength() > 0)
						{
							entry.TrimLeft();
							entry.TrimRight();
							if (entry.IsEmpty() || entry.GetLength() == 0)
								entry = "0";

							if (m_mapCodeABR.Lookup(sCode, (CObject *&)m_symmap))
							{
								m_symmap->SetAt(strsym, entry);
								m_mapCodeABR.SetAt(sCode, (CObject *&)m_symmap);

								sLog.Format("receive1 ABRtest strsym=%s sCode=%s  entry=%s size=%d \n", strsym, sCode, entry, m_symmap->GetCount());
							}
							else
							{

								m_symmap = new CMapStringToString();
								m_symmap->SetAt(strsym, entry);
								m_symmap->SetAt("23", strCurr);
								m_mapCodeABR.SetAt(sCode, (CObject *&)m_symmap);
								sLog.Format("receive2 ABRtest sCode=%s  entry=%s size=%d \n", sCode, entry, m_symmap->GetCount());
							}

							CString strdata;
							const double dData = abs(atoi(strCurr)) - atof(entry);
							if (entry == "0")
								entry == "0";
							else
								entry.Format("%.2lf", dData / atof(entry) * 100); //��ȭ�� / ��ȸ�� * 100

							if (atof(entry) > 0)
								entry = "+" + entry;
							else if (atof(entry) == 0)
								entry = "";
						}
					}

					// khs
					m_grid->SetItemText(ii + 1, jj, entry);

					//���������� ����(2320�ɺ�)
					CString tempYvol;

					if (jj == colPCURR)
					{
						tempYvol = entry;
						tempYvol.Remove('+');
						tempYvol.Remove('-');
						m_iYDayVol[ii][YDAYLASTVALUE] = tempYvol;
					}

					if (jj == colVOL)
					{
						tempYvol = entry;
						tempYvol.Remove('+');
						tempYvol.Remove('-');
						strTodayVolumn = tempYvol;
					}

					if (m_yDayVolField >= 0)
					{
						if (m_yDayVolField == jj)
						{
							entry.Remove('+'), entry.Remove('-');
							m_grid->SetItemText(ii + 1, jj, entry);
						}
					}

					if (m_bongField >= 0)
					{
						switch (jj)
						{
						case colHIGH:
							bongdata.Format("%s%c", entry, P_TAB);
							break;
						case colLOW:
							bongdata += entry;
							break;
						}

						if (m_bongField == jj)
						{
							bongdata.Remove('+') /*, bongdata.Remove('-')*/;
							m_grid->SetItemText(ii + 1, jj, bongdata);
						}
					}

					//ü�� ���������� �ʵ尡 ���� ���
					if (m_EqualizerField >= 0)
					{
					}

					if (m_recomJisuGubnField >= 0)
					{
						if (m_recomJisuGubnField == jj)
						{
							entry.TrimRight();
							entry.TrimLeft();
							m_iYDayVol[ii][JISUGUBN] = entry;
						}
					}

					if (jj == colCURR)
					{
						const int iHeadersymbol = atoi(gridHdr.symbol);

						CString tempStr;

						if (strCode == "001")
						{
							m_dKospi = atof(entry);
							tempStr.Format("%.2f", m_dKospi);
							m_dKospi = atof(tempStr);
						}
						else if (strCode == "201")
						{
							m_dKosdaq = atof(entry);
							tempStr.Format("%.2f", m_dKosdaq);
							m_dKosdaq = atof(tempStr);
						}
					}

					if (m_beforeVolField >= 0)
					{
						if (m_beforeVolField == jj)
						{
							// 							entry.TrimRight();
							// 							entry.TrimLeft();
							//���ϰŷ����� ����(2321�ɺ�)
							// 							m_iYDayVol[ii][JMCODE] = strCode;
							// 							m_iYDayVol[ii][YDAYVOLUMN] = entry;
						}
					}

					if (m_newCurrField >= 0)
					{
						if (m_newCurrField == jj)
						{
							entry.TrimRight();
							entry.TrimLeft();
							entry.Remove('+');
							entry.Remove('-');
							//���ϰŷ����� ����(2321�ɺ�)
							m_iYDayVol[ii][TDAYVALUE] = entry;
						}
					}

					if (m_recomJisuGubnField >= 0)
					{
						if (m_recomJisuGubnField == jj)
						{
							if (((CGroupWnd *)m_pGroupWnd)->getRecommandCursel(m_nIndex) < 0)
							{
								entry.TrimRight();
								entry.TrimLeft();
								m_iYDayVol[ii][JISUGUBN] = entry;
							}
						}
					}
				}

				tempData = IH::Parser(stringx, PTAB);
				tempData.TrimRight();
				tempData.TrimLeft();

				//���ϰŷ����� ����(2321�ɺ�)
				if (!m_bSecond)
				{
					m_iYDayVol[ii][JMCODE] = strCode;
				}

				m_iYDayVol[ii][YDAYVOLUMN] = tempData;

				// 2012.06.20 KSJ ���, ���� �߰�
				str950 = IH::Parser(stringx, PTAB);
				str950.TrimRight();
				str950.TrimLeft();

				str951 = IH::Parser(stringx, PTAB);
				str951.TrimRight();
				str951.TrimLeft();
				// KSJ
				double dval1{}, dval2{}, dval3{}, creditPrc{};
				int iDval = 0;

				CString str;
				dval1 = dval2 = dval3 = creditPrc = 0.0;
				CString futurnGubn, mCode;
				int sizeCode{};

				//��õ����
				if (m_recomDataField > 0)
				{
					CString rDate, fDate;

					rDate = m_grid->GetItemText(ii + 1, m_recomDataField);
					rDate.TrimLeft();
					rDate.TrimRight();

					rDate.Remove('/');

					if (!rDate.IsEmpty())
					{
						fDate.Format("%s/%s/%s", rDate.Left(4), rDate.Mid(4, 2), rDate.Mid(6, 2));
						//						fDate.Format("%04s %02s% 02s", rDate.Left(4), rDate.Mid(4, 2), rDate.Mid(6,2));
						m_grid->SetItemText(ii + 1, m_recomDataField, fDate);
					}
				}

				if (m_epbDataField > 0)
				{
					CString rDate, fDate;

					// 2016.04.15 KSJ epb ��õ���� �߰�
					if (m_arEpbDate.GetSize() > 0 && m_arEpbDate.GetSize() > (int)ii)
					{
						rDate = m_arEpbDate.GetAt(ii);
						fDate.Format("%s/%s/%s", rDate.Left(4), rDate.Mid(4, 2), rDate.Mid(6, 2));

						m_grid->SetItemText(ii + 1, m_epbDataField, fDate);
					}
				}

				for (int jj = nStart; jj < nEnd; jj++)
				{
					const _gridHdr gridHdr = m_gridHdrX.GetAt(jj);
					auto& pinters = _vInters.at(ii);

					mCode = pinters->code; 
					mCode.TrimRight();
					mCode.TrimLeft();
					sizeCode = strlen(mCode);

					if (!m_posField)
						continue; //���Դܰ� �� Ư���ʵ尡 ������� �Ʒ��� �Ѿ

					if (gridHdr.needs != 9)
						continue;

					switch (gridHdr.symbol[3])
					{
					case '1': // ��������
						entry = pinters->xnum;
						break;
					case '2': // ���Դܰ�
						dval1 = IH::TOfabs(pinters->xprc);

						if (sizeCode == 6 || sizeCode == 9)
						{
							iDval = gsl::narrow_cast<int>(Round(dval1));
							entry.Format("%d", iDval);
						}
						else if (sizeCode != 0) // 2012.03.22 KSJ �ڵ���̰� 0�̸� ���þ�����.
						{
							entry.Format("%.2f", dval1);
						}

						break;
					case '3': // �򰡼���
						dval1 = IH::TOfabs(pinters->xnum);
						dval2 = IH::TOfabs(pinters->xprc);
						dval3 = IH::TOfabs(m_grid->GetItemText(ii + 1, colCURR));

						if (dval1 <= 0 || dval2 <= 0)
							continue;

						if (sizeCode == 6 || sizeCode == 9)
						{
							// 2016.04.12 KSJ ���簡���� �׻� �ڿ� ����.
							entry = CalcuPyungaSonik(pinters.get(), m_grid->GetItemText(ii + 1, colCURR));
							// 							entry = CalcuPyungaSonik(pinters, m_iYDayVol[ii][TDAYVALUE]);
						}
						else
						{
							futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
							entry = CalFutureEvalPrice(pinters.get(), strCode, futurnGubn, dval3, dval2, dval1);
							//							entry = setPyungaSonik(pinters);
						}
						break;
					case '4': // ������
						dval1 = IH::TOfabs(pinters->xnum);
						dval2 = IH::TOfabs(pinters->xprc);
						dval3 = IH::TOfabs(m_grid->GetItemText(ii + 1, colCURR));

						if (dval1 <= 0 || dval2 <= 0)
							continue;

						if (sizeCode == 6 || sizeCode == 9)
						{
							// 2016.04.12 KSJ ���簡���� �׻� �ڿ� ����.
							entry = CalcuSuik(pinters.get(), m_grid->GetItemText(ii + 1, colCURR));
							// 							entry = CalcuSuik(pinters, m_iYDayVol[ii][TDAYVALUE]);
						}
						else
						{
							futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
							entry = CalFutureEvalRate(pinters.get(), strCode, futurnGubn, dval3, dval2, dval1);
							//							entry = setsuikRate(pinters);
						}

						break;
					default:
						continue;
					}
					m_grid->SetItemText(ii + 1, jj, entry);
				}

				//	strCode = "";
			}

			entry = m_grid->GetItemText(ii + 1, colINFO); // ����Ư�̻���
			CString strName = m_grid->GetItemText(ii + 1, colNAME);

			//����� ����տ� ��ȣ�� ���� �̹����� �����ְ� �ִ�
			//�̸� �����������񿡸� �̹��� �����ֵ��� ����
			if (!strName.IsEmpty())
			{
				// 2012.06.20 KSJ ���, ���� �߰�
				//���= 11 �ż�����,12 �ż�����,13 �ŵ�����,14 �ŵ�����
				//����= 12:���ʵ��� �������� ����, 14:�������� �������� ���� , 16:�ð��ܴ��ϰ� �������� ����
				bool bCheck = true; // 2012.11.02 KSJ ���ǹ�� > �ܱ�������� > ����溸 ������

				if (!str950.IsEmpty() /*&& str950.GetAt(0) != '0'*/)
				{
					if (str950.GetAt(0) == '1')
					{
						m_grid->SetItemData(ii + 1, colINFO, 12);
						bCheck = false;
					}
					else
					{
						m_grid->SetItemData(ii + 1, colINFO, 0);
						bCheck = true;
					}
				}
				else if (!str951.IsEmpty() /*&& str951.GetAt(0) != '0'*/)
				{
					if (str951 == "12" || str951 == "14" || str951 == "16")
					{
						m_grid->SetItemData(ii + 1, colINFO, 13);
						bCheck = false;
					}
					else
					{
						m_grid->SetItemData(ii + 1, colINFO, 0);
						bCheck = true;
					}
				}
				// KSJ
				if (bCheck)
				{
					SetColInfo(strName, ii + 1, entry);
				}
			}
		}
	}

	m_grid->endDrawHolding();

	if (m_ccField)
		calcInClient();
	const LONG ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
	hawkeyeAction((ret) ? true : false);

	m_grid->memoCheck();
	if (m_seldrop >= 0)
		m_grid->SetSelectRow(m_seldrop), m_seldrop = -1;

	if (nullptr != m_pMsgWnd)
	{
		delete m_pMsgWnd;
		m_pMsgWnd = nullptr;
	}

	if (m_bContinue)
	{
		if (!m_bSecond)
		{
			m_bSecond = TRUE;
			sendTransactionTR(-1, GRIDMAXNUM, m_pGridArray.GetSize());
		}
		else
		{
			m_bContinue = FALSE;
			m_bSecond = FALSE;
			m_bSending = false;
		}
	}
	else
		m_bSending = false;
}

void CGridWnd::getBase(CString m_code)
{
	m_code.TrimLeft();
	m_code.TrimRight();

	if (m_code.IsEmpty())
		return;
	const int nkind = getJKind(m_code);

	m_dBase = 100000;

	switch (nkind)
	{
	case JK_JCALLOPT:
	case JK_JPUTOPT:
		m_dBase = 1;
		break;
	case JK_SFUTURE:
	case JK_SFSPREAD:
		m_dBase = 10;
		break;
	case JK_FUTURE:
	case JK_SPREAD:
		m_dBase = 500000;
		break;
	case JK_STAR:
		m_dBase = 10000;
		break;
	case JK_KTBFUTURE:
		m_dBase = 1000000;
		break;
	case JK_USDFUTURE:
		m_dBase = 10000;
		break;
	case JK_GOLDFUTURE:
		m_dBase = 1000;
		break;
	}
}

CString CGridWnd::getFormatData(CString src)
{
	CString sRtn = src;
	if (!sRtn.IsEmpty() && sRtn.FindOneOf("+-") == 0)
		sRtn = sRtn.Mid(1);
	sRtn.TrimLeft();
	return sRtn;
}

int CGridWnd::getJKind(CString m_code)
{
	if (m_code.GetLength() != 8)
		return -1;
	const TCHAR ch1 = m_code.GetAt(0);
	const TCHAR ch2 = m_code.GetAt(1);
	const TCHAR ch3 = m_code.GetAt(2);

	switch (ch1)
	{
	case '1': // future
	case 'A': //�Ļ���ǰ �ڵ尳��
	{
		if (ch2 == '0')
		{
			if (ch3 == '1')
				return JK_FUTURE;
			else
				return JK_STAR;
		}
		else if (ch2 == '6')
			return JK_KTBFUTURE;
		else if (ch2 == '7')
			return JK_USDFUTURE;
		else if (ch2 == '8')
			return JK_GOLDFUTURE;
		else
			return JK_SFUTURE;
	}
	case '4': // future spread
	case 'D': //�Ļ���ǰ �ڵ尳��
		if (ch2 == '0')
			return JK_SPREAD;
		else
			return JK_SFSPREAD;
	case '2': // call option
	case 'B': //�Ļ���ǰ �ڵ尳��
		if (m_code.GetAt(1) == '0')
			return JK_CALLOPT;
		else
			return JK_JCALLOPT;
		break;
	case '3': // put option
	case 'C': //�Ļ���ǰ �ڵ尳��
		if (m_code.GetAt(1) == '0')
			return JK_PUTOPT;
		else
			return JK_JPUTOPT;
		break;
	}
	return -1;
}

double CGridWnd::Round(double data) //(�ݿø�)
{
	return (int)(data + 0.5);
}

CString CGridWnd::CalJusikEvalPrice(struct _intersx *pinters, double m_curr, double m_maip, double m_remian)
{
	double result{};
	CString entry, m_rprice;

	const double rPrice = m_curr * m_remian;
	const double mPrice = pinters->maeipPrc;

	result = rPrice - mPrice;

	if (result > 0)
	{
		entry.Format("%+.0f", result);
	}
	else if (result < 0)
	{
		entry.Format("-%.0f", result);
	}

	return entry;
}

CString CGridWnd::CalJusikEvalRate(struct _intersx *pinters, double m_curr, double m_maip, double m_remian)
{
	CString entry;
	double result{};
	const double creditPrc = pinters->creditPrc;
	const double resultVal = 0;

	const double rPrice = m_curr * m_remian;
	const double mPrice = pinters->maeipPrc;

	result = rPrice - mPrice;

	entry.Format("%+.2f", (result / (mPrice - creditPrc)) * 100);

	return entry;
}

CString CGridWnd::setPyungaSonik(struct _intersx *pinters)
{
	const double pyunggaPrc = pinters->creditPrc;

	CString result;

	if (pyunggaPrc > 0)
		result.Format("+%.0f", pyunggaPrc);
	else if (pyunggaPrc < 0)
		result.Format("-%.0f", pyunggaPrc);

	return result;
}

CString CGridWnd::setsuikRate(struct _intersx *pinters)
{
	const double sonikRate = pinters->maeipPrc;

	CString result;

	result.Format("%.2f", sonikRate);

	if (sonikRate < 0)
		result = "" + result;
	else if (sonikRate > 0)
		result = "+" + result;

	return result;
}

void CGridWnd::parsingOubsOne(char *datB, int datL, int update)
{
	if (m_staticUpdate < 0)
		return;
	const int gridHoL = sizeof(struct _gridHo);
	if (datL < gridHoL)
		return;

	CString str950, str951;

	CString string, stringx, entry, strData, strField;
	CString bongdata = _T("");
	string = CString(&datB[gridHoL], datL - gridHoL);
	const BOOL bBold = ((CGroupWnd *)m_pGroupWnd)->GetCurrBold();

	CString strCode, strMargin;
	CString tempData, recommand1, recommand2, recommand3, recommand4, recommand5;

	int nStart{}, nEnd{};

	nStart = 0;
	nEnd = m_gridHdrX.GetSize();
	if (m_bContinue)
	{
		if (m_bSecond)
		{
			m_bSecond = FALSE;
			m_bContinue = FALSE;

			nStart = GRIDMAXNUM;
			nEnd = m_gridHdrX.GetSize();
		}
		else
		{
			nStart = 0;
			nEnd = GRIDMAXNUM;
		}
	}

	for (size_t ii = 0; ii < _vInters.size(); ii++)
	{
		if (string.IsEmpty())
			break;

		tempData = "";
		recommand1 = "";
		recommand2 = "";
		recommand3 = "";
		recommand4 = "";
		recommand5 = "";

		stringx = IH::Parser(string, PNEW);
		strData = stringx;
		if ((int)ii < m_staticUpdate)
			continue;

		// 2013.07.04 KSJ �ʵ尡 GRIDMAXNUM(27)���� ������ 2�� ������ ��ȸ�Ѵ�.
		//�׷��⶧���� ù���� �����Ϳ��� 27��°������ �̾Ƽ� �޵����Ͷ� ���̸�
		//�����ѵ����Ͱ� �ȴ�.
		if (m_bContinue && m_bSecond)
		{
			CString strValue;
			strCode = m_grid->GetItemText(ii + 1, colCODE);

			if (m_mapCurValue.Lookup(strCode, strValue))
			{
				int nIndex = 0, nCount = 0;

				while (nIndex != -1)
				{
					nIndex = strValue.Find("\t", nIndex + 1);

					if (++nCount == GRIDMAXNUM)
					{
						strValue = strValue.Left(nIndex + 1);
						break;
					}
				}

				strValue += strData;
				m_mapCurValue.SetAt(strCode, strValue);
			}
		}
		// 2013.07.04 END

		for (int jj = nStart; jj < nEnd; jj++)
		{
			if (stringx.IsEmpty())
				break;

			entry = IH::Parser(stringx, PTAB);

			strField = entry;

			strField.TrimLeft();
			strField.TrimRight();

			if (strField == "0" || strField == "-0" || strField == "0.00" || strField == "+0" || strField == " 0" || strField == ".00" || strField == "+0.00" || strField == "-0.00") // 2012.07.17 KSJ �ɼ��϶��� ��� �߰�
			{
				entry = " ";
			}

			// ADD PSH 20070912
			const _gridHdr gridHdr = m_gridHdrX.GetAt(jj);
			auto& pinters = _vInters.at(ii);

			if (jj == colCODE)
			{
				strCode = entry;

				// 2012.08.31 �߰��� �����͵� �ʿ� ������
				m_mapCurValue.SetAt(strCode, strData);

				if (entry.GetLength() == 6)
				{
					// 2012.06.01 KSJ ������ �κ�
					switch (m_pView->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)entry))
					{
					case kospiType:
						m_grid->SetItemData(ii + 1, jj, 1);
						break;
					case kosdaqType:
						m_grid->SetItemData(ii + 1, jj, 0);
						break;
					default:
						m_grid->SetItemData(ii + 1, jj, 2); // 2012.06.01 KSJ �ѴپƴҶ� �����ϱ�����
						break;
					}
				}
				else
				{
					m_grid->SetItemData(ii + 1, jj, 2);
				}
				// KSJ
			}
			else if (jj == colNAME)
			{
				if (pinters->gubn == 'm')
				{
					entry.Format("%s", pinters->name);
				}

				strMargin = m_grid->GetItemText(ii + 1, colMAR);
				m_mapMargin.SetAt(strCode, strMargin);
				m_grid->SetItemData(ii + 1, colNAME, atoi(strMargin)); // 2017.01.25 KSJ GetBuffer(0) ����
			}
			else if (!entry.IsEmpty() && jj == colRATE && (gridHdr.attr & GVAT_HIDDEN))
			{
				int nType = 0;
				switch (entry[0])
				{
				case '1':
					nType = 1;
					break;
				case '+':
				case '2':
					nType = 2;
					break;
				case '4':
					nType = 4;
					break;
				case '-':
				case '5':
					nType = 5;
					break;
				default:
					nType = 3;
				}
				m_grid->SetItemData(ii + 1, jj, nType);
			}

			CString strsym, sEntry, sLog, sCode, strCurr;
			strsym = (CString)gridHdr.symbol;
			const int xrow = ii + 1;
			if (strsym == "5401" || strsym == "5402" || strsym == "5403" || strsym == "5404" || strsym == "5405")
			{
				strCode = m_grid->GetItemText(xrow, colCODE);
				strCurr = m_grid->GetItemText(xrow, colCURR);
				strCurr.Replace("+", "");
				strCurr.Replace("-", "");

				CMapStringToString *m_symmap{};
				sCode.Format("%s", strCode);
				sCode.TrimRight();
				sCode.TrimLeft();

				if (!sCode.IsEmpty() && sCode.GetLength() > 0)
				{
					entry.TrimLeft();
					entry.TrimRight();
					if (entry.IsEmpty() || entry.GetLength() == 0)
						entry = "0";

					if (m_mapCodeABR.Lookup(sCode, (CObject *&)m_symmap))
					{
						m_symmap->SetAt(strsym, entry);
						m_mapCodeABR.SetAt(sCode, (CObject *&)m_symmap);
					}
					else
					{

						m_symmap = new CMapStringToString();
						m_symmap->SetAt(strsym, entry);
						m_symmap->SetAt("23", strCurr);
						m_mapCodeABR.SetAt(sCode, (CObject *&)m_symmap);
					}

					CString strdata;
					const double dData = abs(atoi(strCurr)) - atof(entry);
					if (entry == "0")
						entry == "0";
					else
						entry.Format("%.2lf", dData / atof(entry) * 100); //��ȭ�� / ��ȸ�� * 100

					if (atof(entry) > 0)
						entry = "+" + entry;
					else if (atof(entry) == 0)
						entry = "";
				}
			}

			if (!(gridHdr.attr & GVAT_HIDDEN) && !entry.IsEmpty())
			{
				if ((strcmp("2029", gridHdr.symbol) == 0) ||
				    (strcmp("2030", gridHdr.symbol) == 0) ||
				    (strcmp("2031", gridHdr.symbol) == 0))
				{
					if ((1 == gridHdr.needs) || (3 == gridHdr.needs))
					{
						CString strVal = entry;
						CString strDiff = strVal.Mid(1);
						double dDiffOpen{}, dPClose{}, dVal{};
						dVal = atof(strDiff);					 // 2017.01.25 KSJ GetBuffer(0) ����
						dPClose = atof((m_grid->GetItemText(ii + 1, colPCURR))); // 2017.01.25 KSJ GetBuffer(0) ����

						dDiffOpen = (dVal - dPClose) / dPClose * 100;

						if (1 == gridHdr.needs)
						{
							if (0 > dDiffOpen)
							{
								entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
							}
							else
							{
								entry.Format("%s( %0.2f%c)", strVal, dDiffOpen, P_PER);
							}
						}
						else if (3 == gridHdr.needs)
						{
							if (0 > dDiffOpen)
							{
								entry.Format("%0.2f%c", dDiffOpen, P_PER);
							}
							else if (0 == dDiffOpen)
							{
								entry.Format(" %0.2f%c", dDiffOpen, P_PER);
							}
							else
							{
								entry.Format(" +%0.2f%c", dDiffOpen, P_PER);
							}
						}
					}
				}
			}
			// END ADD

			m_grid->SetItemText(ii + 1, jj, entry);

			if (m_bongField >= 0)
			{
				switch (jj)
				{
				case colHIGH:
					bongdata.Format("%s%c", entry, P_TAB);
					break;
				case colLOW:
					bongdata += entry;
					break;
				}

				if (m_bongField == jj)
				{
					bongdata.Remove('+'), bongdata.Remove('-');
					m_grid->SetItemText(ii + 1, jj, bongdata);
				}
			}

			if (m_recomJisuGubnField >= 0)
			{
				if (m_recomJisuGubnField == jj)
				{
					entry.TrimRight();
					entry.TrimLeft();
					m_iYDayVol[ii][JISUGUBN] = entry;
				}
			}

			if (m_beforeVolField >= 0)
			{
				if (m_beforeVolField == jj)
				{
					entry.TrimRight();
					entry.TrimLeft();
				}
			}

			if (m_newCurrField >= 0)
			{
				if (m_newCurrField == jj)
				{
					entry.TrimRight();
					entry.TrimLeft();
					entry.Remove('+');
					entry.Remove('-');
					//���ϰŷ����� ����(2321�ɺ�)
					m_iYDayVol[ii][TDAYVALUE] = entry;
				}
			}

			if (m_recomJisuGubnField >= 0)
			{
				if (m_recomJisuGubnField == jj)
				{
					if (((CGroupWnd *)m_pGroupWnd)->getRecommandCursel(m_nIndex) < 0)
					{
						entry.TrimRight();
						entry.TrimLeft();
						m_iYDayVol[ii][JISUGUBN] = entry;
					}
				}
			}

			//���������ʵ尡 �ִ� Ȯ��
			if (!m_posField)
				continue;

			if (gridHdr.needs != 9)
				continue;

			CString mCode;
			int sizeCode;
			mCode = pinters->code;
			mCode.TrimRight();
			mCode.TrimLeft();
			sizeCode = strlen(mCode);

			switch (gridHdr.symbol[3])
			{
			case '1': // ��������
				entry = pinters->xnum;
				break;
			case '2':		   // ���Դܰ�
				if (sizeCode != 0) // 2212.03.22 KSJ �ڵ尡�������� �ƹ��͵� �����ʴ´�.
					entry = pinters->xprc;
				else
					entry = "";
				break;
			case '3': // �򰡼���
				entry = CalcuPyungaSonik(pinters.get(), m_grid->GetItemText(ii + 1, colCURR));
				break;
			case '4': // ������
				entry = CalcuSuik(pinters.get(), m_grid->GetItemText(ii + 1, colCURR));
				break;
			default:
				continue;
			}
			m_grid->SetItemText(ii + 1, jj, entry);
		}

		//���ϰŷ����� ����(2321�ɺ�)
		tempData = IH::Parser(stringx, PTAB);
		tempData.TrimRight();
		tempData.TrimLeft();
		if (!m_bSecond)
		{
			m_iYDayVol[ii][JMCODE] = strCode;
		}

		m_iYDayVol[ii][YDAYVOLUMN] = tempData;
		// save the Current Value
		m_iYDayVol[ii][YDAYLASTVALUE] = m_grid->GetItemText(ii + 1, colCURR);

		// 2012.06.20 KSJ ���, ���� �߰�
		str950 = IH::Parser(stringx, PTAB);
		str950.TrimRight();
		str950.TrimLeft();

		str951 = IH::Parser(stringx, PTAB);
		str951.TrimRight();
		str951.TrimLeft();
		// KSJ

		entry = m_grid->GetItemText(ii + 1, colINFO); // ����Ư�̻���
		{
			CString strName = m_grid->GetItemText(ii + 1, colNAME);

			if (!strName.IsEmpty())
			{
				// 2012.06.20 KSJ ���, ���� �߰�
				//���= 11 �ż�����,12 �ż�����,13 �ŵ�����,14 �ŵ�����
				//����= 12:���ʵ��� �������� ����, 14:�������� �������� ���� , 16:�ð��ܴ��ϰ� �������� ����
				bool bCheck = true; // 2012.11.02 KSJ ���ǹ�� > �ܱ�������� > ����溸 ������

				if (!str950.IsEmpty() /*&& str950.GetAt(0) != '0'*/)
				{
					if (str950.GetAt(0) == '1')
					{
						m_grid->SetItemData(ii + 1, colINFO, 12);
						bCheck = false;
					}
					else
					{
						m_grid->SetItemData(ii + 1, colINFO, 0);
						bCheck = true;
					}
				}
				else if (!str951.IsEmpty() /*&& str951.GetAt(0) != '0'*/)
				{
					if (str951 == "12" || str951 == "14" || str951 == "16")
					{
						m_grid->SetItemData(ii + 1, colINFO, 13);
						bCheck = false;
					}
					else
					{
						m_grid->SetItemData(ii + 1, colINFO, 0);
						bCheck = true;
					}
				}
				// KSJ
				if (bCheck)
				{
					SetColInfo(strName, ii + 1, entry);
				}
			}
		}
		// END ADD

		if (m_ccField)
			calcInClient(ii + 1);
		const LONG ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
		if (ret)
			hawkEyeColor(ii + 1);
		if (m_grid->memoCheck(ii + 1))
			m_grid->memoRefresh();
		break;
	}

	// ADD PSH 20070927

	stringx = IH::Parser(string, PTAB);

	if (!stringx.IsEmpty())
	{
		m_mapMargin.SetAt(strCode, stringx);
	}

	m_grid->memoCheck(m_updateROW + 1);
	if (m_seldrop >= 0)
		m_grid->SetSelectRow(m_seldrop), m_seldrop = -1;

	if (m_bContinue)
	{
		m_bSecond = TRUE;
		sendTransactionTR(-1, GRIDMAXNUM, m_pGridArray.GetSize());
	}
	else
	{
		m_bSending = false;
		m_updateROW = -1;
		ReIndexing();
		Invalidate(false);
	}
}

void CGridWnd::InsertNews(CString datB)
{
	m_bNews = true;
	int idx = 0;
	CString code = _T(""), szTitle = _T(""), symbol = _T(""), entry = _T("");

	while (!datB.IsEmpty())
	{
		idx = datB.Find('\t');
		if (idx == -1)
			break;

		symbol = datB.Left(idx++);
		datB = datB.Mid(idx);

		idx = datB.Find('\t');
		if (idx == -1)
		{
			entry = datB;
			datB = _T("");
		}
		else
		{
			entry = datB.Left(idx++);
			datB = datB.Mid(idx);
		}

		if (symbol.Compare("015") == 0)
			szTitle = entry;

		if (symbol.Compare("301") == 0)
		{
			code = entry;
			code.TrimRight();
			break;
		}
	}

	if (!code.IsEmpty())
	{
		m_szTitle.Format(" %s", (char *)m_pTreeWnd->SendMessage(WM_MANAGE, MK_GETDATATITLE, m_kind));
		m_szTitle += ("- " + szTitle);
		InvalidateRect(m_rcTitle, FALSE);
		const int ncnt = _vInters.size();
		//struct _intersx *pinters = nullptr;
		int nIndex = -1;
		for (int ii = 0; ii < ncnt; ii++)
		{
			auto& pinters = _vInters.at(ii);
			if (strlen(pinters->code) == 0)
			{
				break;
			}

			if (strcmp(pinters->code, (char *)code.operator LPCTSTR()) == 0)
			{
				nIndex = ii;
				break;
			}
		}

		if (nIndex < 0)
		{
			DeleteRow(MAX_LINE);
		}
		else
		{
			DeleteRow(nIndex + 1);
		}

		insertRow(1);
		auto& pinters = _vInters.at(0);
		m_grid->SetItemData(1, colSIG, MAKEWPARAM(0, 1));
		pinters->code = code;
		sendTransaction();
	}
	m_bNews = false;
}

// 2012.01.19 KSJ Alertx �߰�
void CGridWnd::InsertNewsx(DWORD *data)
{
	class CIndexMap *idx = nullptr;
	CString code = _T(""), szTitle = _T(""), strCode, strTemp;

	szTitle = (char *)data[15];
	code = (char *)data[301];
	code.Trim();
	strCode = code;

	if (_vInters.at(0)->code.CompareNoCase(code) == 0)
		return;

	if (!code.IsEmpty())
	{
		m_szTitle.Format(" %s", (char *)m_pTreeWnd->SendMessage(WM_MANAGE, MK_GETDATATITLE, m_kind));
		m_szTitle += ("- " + szTitle);
		InvalidateRect(m_rcTitle, FALSE);

		if (code.GetAt(0) == '5')
			code = "Q" + code;
		else
			code = "A" + code; // 2012.09.11 A�� �ٿ��ش�.

		int nIndex = 0;
		if (m_pSearchMap.GetCount() > 0)
		{
			if (!m_pSearchMap.Lookup(code, (CObject *&)idx))
			{
				DeleteRow(MAX_LINE);
			}
			else
			{
				nIndex = idx->index[0];
				if (nIndex < 100)
				{
					DeleteRow(nIndex); // 2016.07.04 KSJ ���������� �����̸� �ش������ ����
				}
			}
			// 2012.10.09 KSJ ���ؿ� �ε����� �����ε�����. ������ nIndex +1 �� nIndex �� ������.
		}

		insertRow(1);
		const auto pinters = _vInters.emplace(_vInters.begin(), std::make_shared<_intersx>());
		m_grid->SetItemData(1, colSIG, MAKEWPARAM(0, 1));
		m_grid->SetItemText(1, colRCOD, code);
		m_grid->SetItemText(1, colCODE, strCode);
		(*pinters)->code = strCode;

		_vInters.resize(MAX_LINE);
		sendTransaction(0); // 2016.06.03 KSJ �Ҷ� ù�ٸ� poop �¿��� �Ѵ�.
	}
}
// 2012.01.19 KSJ Alertx �߰� ��

void CGridWnd::ReplaceSymbol(CMapStringToString &fms, CString szold, CString sznew)
{
	CString entry = _T("");
	if (fms.Lookup(szold, entry))
	{
		fms.RemoveKey(szold);
		fms.SetAt(sznew, entry);
	}
}

CString SplitString(CString &strData, CString strToken)
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

void CGridWnd::SettingGridHeaderName(int index)
{
	const struct _symbol
	{
		UINT stid1;
		char *symb1;
		UINT stid2;
		char *symb2;
	} chksym[] = {
	    {IDS_GH_CURR, "2023", IDS_GH_ANTIPRC, "2111"},
	    {IDS_GH_DIFF, "2024", IDS_GH_DIFF, "2115"},
	    {IDS_GH_RATE, "2033", IDS_GH_RATE, "2116"},
	    {IDS_GH_VOL, "2027", IDS_GH_ANTIVOL, "2112"},
	};

	const int chksymC = sizeof(chksym) / sizeof(_symbol);

	if (index == 1)
	{
		_gridHdr gridHdr{};
		for (int ii = 0; ii < chksymC; ii++)
		{
			for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
			{
				gridHdr = m_gridHdrX.GetAt(jj);
				if (atoi(gridHdr.symbol) != atoi(chksym[ii].symb1))
					continue;

				gridHdr.stid = chksym[ii].stid2;
				m_gridHdrX.SetAt(jj, gridHdr);
			}
		}
		for (int jj = 0; jj < m_gridHdrX.GetSize(); jj++)
		{
			gridHdr = m_gridHdrX.GetAt(jj);
		}
	}
	else
	{
		_gridHdr gridHdr{};
		for (int ii = 0; ii < chksymC; ii++)
		{
			for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
			{
				gridHdr = m_gridHdrX.GetAt(jj);
				if (atoi(gridHdr.symbol) != atoi(chksym[ii].symb2))
					continue;

				gridHdr.stid = chksym[ii].stid1;
				m_gridHdrX.SetAt(jj, gridHdr);
				break;
			}
		}
	}
}

void CGridWnd::RTS_parsingAlertx(struct _Ralert *palert)
{
	CString slog;
	int xrow{};
	CString code, name, symbol, entry, datB, strValue, dataExceptCode, strCode, strData, sTime;
	CString strGubn; // 2012.02.09 KSJ ȣ�� ü�� ����
	CString strTemp;
	CString str950, str951;
	CString strTmp;

	code.Format("%s", palert->code);
	code.TrimRight();

	if (code.GetLength() == 7)
	{
		strCode = code.Mid(1);
	}
	else if (code.GetAt(0) == 'X')
	{
		strCode = code.Mid(1);
	}
	else
		strCode = code;

	DWORD *data{};

	if (palert->ptr[0])
	{
		strGubn = (char *)palert->ptr[0].get();
		strGubn.Trim();
	}

	if (strGubn == "X" && code.GetLength() == 5) //�����϶�
	{
		code.Delete(0);	     //ù ���� 'X'�� �����Ѵ�.
		code.Insert(0, 'K'); //ù ���ڸ� 'K'�� �ٲ��ش�.
		strCode = code;
	}

//XMSG(realtime);
//AxStd::_Msg("[%s]", strGubn);

	if (code.CompareNoCase("S0000") == 0)
	{
		if (CAST_TREEID(m_kind)->kind == xISSUE)
		{
			InsertNewsx((DWORD *)palert->ptr);
		}
		else
		{
			parsingNewsx((DWORD *)palert->ptr);
		}
		return; // 2012.09.11 KSJ �����϶��� �Ʒ����� Ż�ʿ� ����.
	}

	int count = 0;
	count = CheckRealTimeCode(code);
	if (count == 0)
		return;

	m_grid->beginDrawHolding();

	for (int rowPosition = 0; rowPosition < count; rowPosition++)
	{
		xrow = m_irowCode[rowPosition];

		// BOOL bNot = FALSE;

		// if (!palert->ptr[34] && palert->ptr[40])
		// {
		// 	if (!palert->ptr[111])
		// 		bTicker = FALSE;
		// }

		// if (palert->ptr[734] || palert->ptr[740])
		// {
		// 	bTicker = FALSE;
		// }

		entry = _T("");
		CString oldEXP = m_grid->GetItemText(xrow, colEXPECT);
		CString newEXP = _T("");
		const BOOL bManual = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_EXPECT);
		const BOOL bAutoCheck = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_AUTOEXPECT);
		BOOL bTransSymbol = FALSE; // 2012.02.09 KSJ �ɺ�����Ǵ� ��Ȳ �϶� TRUE
		BOOL bDaebi = FALSE;	   // 2012.03.20 KSJ ������ 0�϶� üũ��.
		BOOL bZisu = FALSE;	   // 2013.08.27 KSJ �����϶��� ������ 'X'�̰� ���󰡰� 23�� �´�.

		CString strTime, expect, real, excep;
		CString codeExceptA;

		//������ ���������� �迭�� ������ �� ���簡 ����Ÿ ������Ʈ
		CString en2, saveData;

		if (palert->ptr[111])
		{
			saveData.Format("%s", palert->ptr[111].get());
			if (palert->ptr[34])
				en2.Format("%s", palert->ptr[34].get());
			else
				en2 = "";
		}
		else if (palert->ptr[23])
		{
			saveData.Format("%s", palert->ptr[23].get());
			if (palert->ptr[34])
				en2.Format("%s", palert->ptr[34].get());
			else
				en2 = "";

			if (!en2.IsEmpty())
			{
				for (int i = 0; i < MAX_LINE; i++)
				{
					codeExceptA = strCode;

					//�����ȣ�� ��Ī�ؼ� �����͸� �����ش�
					if (codeExceptA == m_iYDayVol[i][JMCODE])
					{
						m_iYDayVol[i][TDAYVALUE] = saveData;
					}
				}
			}
		}
		/////////////////�������

		//�������� �� �������� ����
		// 111�� ���������� 023�� ���������� �Ǵ��ؼ� ���� ����
		entry.Empty();

		CString str90;
		if (palert->ptr[90])
			str90.Format("%s", palert->ptr[90].get());
		else
			str90 = "";

		BOOL bLast = FALSE; // 2015.01.15 ���͸������� ���� ������ ������ �����ؾ���.��
		str90.Trim();

		// 2015.01.15 ����ȣ�� 4 �߰��� 2015.02.03 KSJ 9�� ������� ���� ���͸��� �����Ͱ� ���´�.
		if ((strGubn == "L" || strGubn == "4" || strGubn == "P" || strGubn == "g") && ((str90 == "99") || (str90 == "40")))
		{
			m_grid->SetItemText(xrow, colEXPECT, "0"); // 2013.09.13 KSJ �ɼ����͸������� �������� ���󰡸� �ִµ� �����ؾ��Ѵ�.
			m_grid->SetItemData(xrow, colEXPECT, 0);

			bLast = TRUE;
		}

		const int nEndOPMarket = m_grid->GetItemData(xrow, colEXPECT); // 2013.09.17 KSJ �ش������� ������ �Ǿ�����

		if ((strGubn == "X" || palert->ptr[111] || nEndOPMarket == 1) && !bLast) // ���� ����	2013.08.22 �������󰡴�111�ɺ��� ���� ���а���X�� �´�.
		{
			if (palert->ptr[111])
				entry.Format("%s", palert->ptr[111].get());
			else if (nEndOPMarket == 1) // 2013.09.17 KSJ �ش������� ������ �Ǿ�����
				entry = " ";	    //���󰡰� 0�� �ö��� ���� ��Ȳ��.
			else
			{
				bZisu = TRUE;
				if (palert->ptr[23])
					entry.Format("%s", palert->ptr[23].get()); // 2013.08.22 �������󰡴�111�ɺ��� ���� ���а���X�� �´�.
			}

			if (entry != "0" && entry != "-0" && entry != "0.00" && entry != "+0" && entry != " 0" && entry != " ")
			{
				m_grid->SetItemText(xrow, colEXPECT, "1");
			}
			else
			{
				// 2012.05.09 KSJ ���󰡰� 0�� �ö��� ���簡�� �ѷ��ش�.
				m_mapCurValue.Lookup(strCode, strData);

				m_grid->SetItemText(xrow, colEXPECT, "0"); //���� ���
				int i = col7852;

				_gridHdr gridHdr{};
				CString str7852, str7853, str7854, str7855;
				int nSymbol = 0;
				CString strCurrData;

				while (AfxExtractSubString(entry, strData, i, '\t'))
				{
					if (i >= m_gridHdrX.GetSize() - 1) // khs
						break;

					if (entry == "(nullptr)" || entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == " 0" || entry == " ")
						entry = "";

					// 2013.07.01 KSJ ���簡 ����
					gridHdr = m_gridHdrX.GetAt(i);
					symbol = CString(gridHdr.symbol, strlen(gridHdr.symbol));
					CString strsym;
					strsym = symbol;

					if (symbol.GetLength() >= 3)
						symbol = symbol.Right(3);

					nSymbol = atoi(symbol);

					if (i == col7852)
						str7852 = entry;
					else if (i == col7853)
						str7853 = entry;
					else if (i == col7854)
						str7854 = entry;
					else if (i == col7855)
						str7855 = entry;
					else if (i == colMAR || i == colMAR1 || i == colSIG || i == colCODE || i == colNAME /*|| i == colCURR*/) // 2013.07.25 KSJ ���簡�� ���⼭ Ÿ�� �ȵȴ�.
						entry = m_grid->GetItemText(xrow, i);
					else if (nSymbol == 23 || nSymbol == 111) //
						entry = str7852;
					else if (nSymbol == 27 || nSymbol == 112) //
						entry = str7853;
					else if (nSymbol == 024 || nSymbol == 115) //
						entry = str7854;
					else if (nSymbol == 33 || nSymbol == 116) //
						entry = str7855;

					if (strsym == "5401" || strsym == "5402" || strsym == "5403" || strsym == "5404" || strsym == "5405") // khs
					{
						strCurrData = m_grid->GetItemText(xrow, colCURR);
						strCurrData.Replace("+", "");
						strCurrData.Replace("-", "");
						entry = getABRdata((LPSTR)(LPCTSTR)strCurrData, strCode, strsym);
						m_grid->SetItemText(xrow, i, entry);
					}

					if (entry != "0" && entry != "-0" && entry != "0.00" && entry != "+0" && entry != " 0" && entry != " ")
					{
						// 2013.07.01 KSJ
						m_grid->SetItemText(xrow, i++, entry);
					}
					else if (!symbol.Compare("##1") || !symbol.Compare("##2") || !symbol.Compare("##3") || !symbol.Compare("##4")) // 2014.02.12 ��������, ���Դܰ�, �򰡼���, ���ͷ��� ����ڰ� �Է��ϴ� �κ��̹Ƿ� �翬�� ���̴�.
					{
						m_grid->SetItemText(xrow, i, m_grid->GetItemText(xrow, i++));
					}
					else
					{
						m_grid->SetItemText(xrow, i++, " ");
					}
				}
				entry = m_grid->GetItemText(xrow, colCURR);
				// KSJ
				m_grid->endDrawHolding();
				return;
			}

			//���� �����ư�� �ȴ����� ������, ������ �ɺ��� �ٲپ �׸��忡 �����͸� �����ְ� �ִ�
			//(���簡 �ɺ��� �Ǿ��ֱ� ������ )
			//�׷� �ڵ� üũ ������������, �����ָ� �ǰ�
			//�ڵ� üũ �����ÿ�, ���� �Ⱥ����ָ� �ȴ�
			if (!bManual) //���� ��ư �ȴ��� ����
			{
				// replace symbol
				if (bAutoCheck) // �ڵ��� üũ�� ����
				{
					bTransSymbol = TRUE; // 2012.02.09 KSJ ���簡 �ɺ��� ���� �ɺ��� �ٲ�����.
				}
				else
				{
					if (!entry.IsEmpty())
					{
						m_grid->SetItemText(xrow, colEXPECT, "0");
					}
				}
			}
			else
				bTransSymbol = TRUE; // 2012.02.09 KSJ ���簡 �ɺ��� ���� �ɺ��� �ٲ�����.
		}
		else if (palert->ptr[23])
		{
			entry.Format("%s", palert->ptr[23].get());

			if (!bManual) //���� ��ư �ȴ��� ����
			{
				if (bAutoCheck) // �ڵ��� üũ�� ����
				{
					if (palert->ptr[34])
						strTime.Format("%s", palert->ptr[34].get());
					else
						strTime = "";

					if (m_strBeginTime <= strTime && m_strEndTime >= strTime)
					{
						if (entry != "0" && entry != "-0" && entry != "0.00" && entry != "+0" && entry != " 0" && entry != " ")
						{
							m_grid->SetItemText(xrow, colEXPECT, "1");
						}
						else
						{
							m_grid->SetItemText(xrow, colEXPECT, "0"); // 2012.08.29 KSJ ���� �����. 8:10 ~ 8:30�� ���̿� ü���� ��������
							entry = m_grid->GetItemText(xrow, colCURR);
							m_grid->endDrawHolding();
							return;
						}
					}
					else
					{
						m_grid->SetItemText(xrow, colEXPECT, "0");
					}
				}
			}
			else
			{
				if (!entry.IsEmpty())
				{
					bTransSymbol = TRUE; // 2012.02.09 KSJ ���簡 �ɺ��� ���� �ɺ��� �ٲ�����.

					if (entry != "0" && entry != "-0" && entry != "0.00" && entry != "+0" && entry != " 0" && entry != " ")
					{
						m_grid->SetItemText(xrow, colEXPECT, "1");
					}
					else
					{
						m_grid->SetItemText(xrow, colEXPECT, "0"); // 2012.08.23 KSJ ���� ��� �߰���

						entry = m_grid->GetItemText(xrow, colCURR);
						m_grid->endDrawHolding();
						return;
					}
				}
			}

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 2012.05.10 KSJ ParseOubs���� ������m_mapCurValue�� �����͵��� �ʱⰪ�̹Ƿ� 2012.08.22 ����
			//�ǽð����� ����� ���� ������ �־���Ѵ�. �ŷ���, ü�ᷮ ����..
			//�Ʒ��� ���� ���󰡰� 0�� ���ö��� �������ָ� ��ũ�� ���� �������� �־ ��� ������Ʈ �Ѵ�.
			//���� ȣ�� �ð��϶��� ���� �����Ѵ�.
			//�ڵ��� üũ�Ǿ� �������� �����Ѵ�.

			const int countX = m_gridHdrX.GetSize();
			_gridHdr gridHdr{};

			CString strNewData, strNewTemp;
			strNewData.Empty();

			for (int jj = col7852; jj < countX; jj++)
			{
				gridHdr = m_gridHdrX.GetAt(jj);

				symbol = CString(gridHdr.symbol, strlen(gridHdr.symbol));

				//	if(atoi(symbol) > 120)
				//		continue;

				//���μ������� �ǽð��� �ƴϴ�.
				if (symbol == "2204")
					continue;

				if (symbol.GetLength() >= 3)
					symbol = symbol.Right(3);

				// 2012.11.19 KSJ �����Ͱ� ������ �Ǹ� �ִ´�.
				// 2013.07.01 KSJ ���簡 ����
				strTime = "";
				if (atoi(symbol) == 0) // 2014.03.18 KSJ �ɺ����� #�� ���� 0�̵Ǿ B,F,G�̷� ���� ����.
					strNewTemp = "";
				// strNewTemp.Format("\t");
				else if (palert->ptr[atoi(symbol)])
				{
					if (palert->ptr[atoi(symbol)])
						strNewTemp.Format("%s", palert->ptr[atoi(symbol)].get());
				}
				else if (jj == col7852)
				{
					if (palert->ptr[23])
						strNewTemp.Format("%s", palert->ptr[23].get());
				}
				else if (jj == col7853)
				{
					if (palert->ptr[27])
						strNewTemp.Format("%s", palert->ptr[27].get());
				}
				else if (jj == col7854)
				{
					if (palert->ptr[24])
						strNewTemp.Format("%s", palert->ptr[24].get());
				}
				else if (jj == col7855)
				{
					if (palert->ptr[33])
						strNewTemp.Format("%s", palert->ptr[33].get());
				}
				else
					strNewTemp = "";

				strNewData += "\t" + strNewTemp;
			}

			if (m_mapCurValue.Lookup(strCode, strData))
			{
				int nIndex = 0, nCount = 0;
				while (nIndex != -1)
				{
					nIndex = strData.Find(P_TAB, nIndex + 1);

					nCount++;

					if (nCount == col7852)
					{
						strTemp = strData.Left(nIndex);
						strData = strTemp + strNewData;
						m_mapCurValue.SetAt(strCode, strData);
						break;
					}
				}
			}
		}

		newEXP = m_grid->GetItemText(xrow, colEXPECT);
		const BOOL bForceDraw = (newEXP == oldEXP) ? FALSE : TRUE;
		BOOL bExpect = (BOOL)atoi(m_grid->GetItemText(xrow, colEXPECT)); // ������ ������ 0 �̳� 1 �� return
		bool updatePoss = false;
		const int countX = m_gridHdrX.GetSize();
		_gridHdr gridHdr{};
		// 			LONG	ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
		// LONG ret = 0;

		// 			COLORREF rtmColor = ((CGroupWnd*)m_pGroupWnd)->GetRTMColor();
		// 			int	rtmAction = ((CGroupWnd*)m_pGroupWnd)->GetRtmAction();
		// 			BOOL	bBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();

		// 2012.11.08 KSJ �ֽİ� �����ɼ��� �ɺ����� �Ȱ��� ���� �ִ�. ���μ����� ������.
		bool bKospi = true;
		if (strCode.GetLength() == 6)
			bKospi = true;
		else
			bKospi = false;
		// KSJ

		int nSymbol = 0;
		CString strsym;

		for (int ii = 2; ii < countX; ii++)
		{
			gridHdr = m_gridHdrX.GetAt(ii);

			symbol = CString(gridHdr.symbol, strlen(gridHdr.symbol));
			strsym = symbol;

			//���μ������� �ǽð��� �ƴϴ�.
			if (symbol == "2204")
				continue;

			if (symbol.GetLength() >= 3)
				symbol = symbol.Right(3);

			nSymbol = atoi(symbol);
			if (/*data[atoi(symbol)] && */ !bTransSymbol) // 2013.07.08 ����üũ�Ǿ� ������ �ؿ� Ÿ����
			{
				// 2012.03.16 KSJ ����, �ŷ��� �����־ �߰���..
				switch (nSymbol)
				{
				//����
				case 111:
				{
					if (palert->ptr[23])
						entry.Format("%s", palert->ptr[23].get()); //���簡
					else
						entry = "";
				}
				break;
				//����ŷ���
				case 112:
				{
					if (palert->ptr[27])
						entry.Format("%s", palert->ptr[27].get());
					else
						entry = "";
				}
				break;
				//�������ϴ��
				case 115:
				{
					if (palert->ptr[24])
						entry.Format("%s", palert->ptr[24].get());
					else
						entry = "";
				}

				break;
				//��������
				case 116:
				{
					if (palert->ptr[33])
						entry.Format("%s", palert->ptr[33].get());
					else
						entry = "";
				}

				break;
				//���μ�����
				case 204:
					entry = bKospi ? entry : " ";
					break;
				// 2014.03.18 KSJ �ɺ����� #�� ���� 0�̵�
				case 0:
					entry = " ";
					break;
				default:
					//		entry = (char*)data[nSymbol]; break;
					if (palert->ptr[nSymbol])
						entry.Format("%s", palert->ptr[nSymbol].get());
					else
						entry = "";
					break;
				}
			}
			else if (palert->ptr[atoi(symbol)] && bZisu) // 2013.08.27 KSJ �����϶��� �ɺ��� �ε����� ��ġ��.
			{
				if (palert->ptr[atoi(symbol)])
					entry.Format("%s", palert->ptr[atoi(symbol)].get());
				entry = "";
			}
			else if (bTransSymbol)
			{
				// 2012.03.16 KSJ ����, �ŷ��� �����־ �߰���..
				switch (nSymbol)
				{
				//����
				case 23:
				{
					if (palert->ptr[111])
						entry.Format("%s", palert->ptr[111].get());
					else
						entry = "";
				}
				break;
				//�ŷ���
				case 27:
					// entry = (char*)data[112];	break; //����ŷ���'
					{
						if (palert->ptr[112])
							entry.Format("%s", palert->ptr[112].get());
						else
							entry = "";
					}
					break;
				//���ϴ��
				case 24:
				{
					// entry = (char*)data[115];	break; //�������ϴ��
					if (palert->ptr[115])
						entry.Format("%s", palert->ptr[115].get());
					else
						entry = "";
				}
				break;
				//�����
				case 33:
					// entry = (char*)data[116];	break; //��������
					{
						if (palert->ptr[116])
							entry.Format("%s", palert->ptr[116].get());
						else
							entry = "";
					}

					break;
				//���μ�����
				case 204:
					entry = bKospi ? entry : " ";
					break;
				// 2014.03.18 KSJ �ɺ����� #�� ���� 0�̵�
				case 0:
					entry = " ";
					break;

				default:
				{
					if (palert->ptr[nSymbol])
						entry.Format("%s", palert->ptr[nSymbol].get());
					else
						entry = "";
				}
				break;
				}
			}
			else
				continue;

			if (!bForceDraw && IH::TOf(entry) == IH::TOf(m_grid->GetItemText(xrow, ii)))
				continue;

			if (!(gridHdr.attr & GVAT_HIDDEN) && !entry.IsEmpty())
			{
				if ((strcmp("2029", gridHdr.symbol) == 0) ||
				    (strcmp("2030", gridHdr.symbol) == 0) ||
				    (strcmp("2031", gridHdr.symbol) == 0))
				{
					if ((1 == gridHdr.needs) || (3 == gridHdr.needs))
					{
						CString strVal = entry;
						CString strDiff = strVal;

						if (strVal[0] == '+' || strVal[0] == '-')
						{
							strDiff = strVal.Mid(1);
						}

						double dDiffOpen = 0.0;
						double dPClose{}, dVal{};
						dVal = atof(strDiff);
						dPClose = atof((m_grid->GetItemText(xrow, colPCURR)));

						if (dVal != 0 && dPClose != 0)
						{
							dDiffOpen = (dVal - dPClose) / dPClose * 100;

							if (1 == gridHdr.needs)
							{
								if (0 > dDiffOpen)
								{
									entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
								}
								else
								{
									entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
								}
							}
							else if (3 == gridHdr.needs)
							{
								if (0 > dDiffOpen)
								{
									entry.Format("%0.2f%c", dDiffOpen, P_PER);
								}
								else if (0 == dDiffOpen)
								{
									entry.Format(" %0.2f%c", dDiffOpen, P_PER);
								}
								else
								{
									entry.Format(" +%0.2f%c", dDiffOpen, P_PER);
								}
							}
						}
					}
				}
			}

			if ((ii == colCURR) && bExpect && !m_bExpect) // �����ư�� �ȴ������ְ�, ����ȣ����
			{
				entry.TrimLeft();
				entry.TrimRight();

				if ((entry == "0") || (entry == "-0") || (entry == "+0") || (entry == ""))
				{
					entry = " ";

					entry = m_grid->GetItemText(xrow, colCURR);
					m_grid->endDrawHolding();
					return;
				}
			}

			CString tempStr;

			if (ii == colCURR)
			{
				if (code.Find("K0001") > -1)
				{
					m_dKospi = atof(entry);
					tempStr.Format("%.2f", m_dKospi);
					m_dKospi = atof(tempStr);
				}
				else if (code.Find("KQ001") > -1)
				{
					m_dKosdaq = atof(entry);
					tempStr.Format("%.2f", m_dKosdaq);
					m_dKosdaq = atof(tempStr);
				}
			}

			// 2012.03.20 KSJ ���簡 ���յɶ� ���, ����� �������� �ʴ� ���� ����
			if ((strcmp("2115", gridHdr.symbol) == 0) || (strcmp("2024", gridHdr.symbol) == 0) || (strcmp("2116", gridHdr.symbol) == 0) || (strcmp("2033", gridHdr.symbol) == 0))
			{
				entry.TrimLeft();
				entry.TrimRight();
				if (entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == " 0" || entry == "30" || entry == ".00" || entry == "+0.00" || entry == "-0.00") // 2012.07.17 KSJ �ɼ��϶��� ��� �߰�
				{
					bDaebi = TRUE;
				}
			}
			else
			{
				// 2012.03.22 KSJ ��������, ���Դܰ�, �򰡼��� �� �ʵ尪�� #�� ���� ���� atoi���� 0�� ���´�.
				//�׷��� entry�� data[0]���� �̾ƿ��Ƿ� ���� ���а� d,f,b, �̷������� �� �׸��忡 �ѷ�����.
				entry.TrimLeft();
				entry.TrimRight();
				if (entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == " 0" || entry == ".00" || entry == "+0.00" || entry == "-0.00" || atof(entry) == 0 || atof(symbol) == 0) // KSJ 2012.07.17 atoi --> atof�� ����
				{
					if (entry.GetLength() > 0 && atof(entry.Mid(1)) == 0)
						entry = " "; // 2013.09.24 KSJ �������������� ++ +- �� ��ȣ�� ���ڸ��� �����µ� �̶��� atof�� ���� ���ϸ� 0���� ���´�. �׷��� ���ڸ� �ڸ��� ���غ�
				}
			}

			CString tmp;

			if (ii == colCURR)
			{
				if (entry == "")
				{
					entry = m_grid->GetItemText(xrow, colCURR);
				}
			}

			CString strPreValue;

			strPreValue = m_grid->GetItemText(xrow, ii);
			strPreValue.TrimLeft();
			strPreValue.TrimRight();

			// khs

			if (strsym == "5401" || strsym == "5402" || strsym == "5403" || strsym == "5404" || strsym == "5405")
			{
				if (palert->ptr[23])
				{
					entry = getABRdata(palert->ptr[23].get(), strCode, strsym);
				}
				else
				{
					CString strcurr;
					strcurr.Format("%s %s\n", palert->ptr[23].get(), palert->ptr[111].get());
					strcurr = m_grid->GetItemText(xrow, colCURR);
					strcurr.Replace("+", "");
					strcurr.Replace("-", "");
					entry = getABRdata((LPSTR)(LPCTSTR)strcurr, strCode, strsym);
				}
			}

			if (ii != colCURR && strPreValue != entry)
			{
				entry.TrimLeft();
				entry.TrimRight();

				if (!entry.IsEmpty()) // 2012.03.20 KSJ ���簡 ���յɶ� ���, ����� �������� �ʴ� ���� ����
				{
					if (bDaebi)
						entry = "";

					bDaebi = FALSE;

					// 2012.02.27 KSJ �׷����� �����Ϳ� �̻��������� ���⸦ üũ�ؾ���
					m_grid->SetItemText(xrow, ii, entry);
				}
			}
			else if (ii == colCURR)
			{
				// 2013.08.23 KSJ �������������̸� ����+, �϶�+, ���- �� ���� ��ȣ�� ǥ��������Ѵ�.
				if (strCode.GetLength() == 8 && (strCode.GetAt(0) == '4' || strCode.GetAt(0) == 'D')) //�Ļ���ǰ �ڵ尳��
				{
					if (entry.GetAt(0) == ' ')	  //ó���� ��ĭ�̸� ���տ� -, + ǥ�ð� �ִ� ���̴�.
						entry.Replace(" ", "��"); //�׶��� '��' �� ���� 1�� �� �־��ش�. �����̽� �ƴ�..
					else if (entry.GetAt(0) == '0')
					{
						entry.Delete(0);
						entry.Insert(0, "��");
					}
				}
				// 2013.08.23 KSJ END
				m_grid->SetItemText(xrow, ii, entry);
			}

			//���ϰŷ������ �ǽð� ���
			CString str2027, str2403;
			double int2027 = 0;
			double int2403 = 0;
			double int2321 = 0;

			if (palert->ptr[27])
			{
				str2027.Format("%s", palert->ptr[27].get());

				int2027 = atoi(str2027);
				for (int i = 0; i < IARRAYSIZE; i++)
				{
					//�����ȣ�� ��Ī�ؼ� �����͸� �����ش�
					if (strCode == m_iYDayVol[i][JMCODE])
					{
						int2321 = atoi(m_iYDayVol[i][YDAYVOLUMN]);
					}
				}

				if (int2321 != 0)
				{
					int2403 = (int2027 / int2321) * 100;
				}
				else
				{
					int2403 = 0;
				}

				if (!bExpect && m_yDayVolField >= 0)
				{
					entry.Format("%.2f", int2403);

					m_grid->SetItemText(xrow, m_yDayVolField, entry);
				}
			}

			if (ii == colCURR)
			{
				if (!bExpect && m_bongField >= 0)
				{
					CString bongdata = _T(""), open = _T(""), high = _T(""), low = _T("");

					if (palert->ptr[29])
					{
						open.Format("%s", palert->ptr[29].get());
						m_grid->SetItemText(xrow, colOPEN, open);
					}

					if (palert->ptr[30])
					{
						high.Format("%s", palert->ptr[30].get());
						high.Remove('+'), high.Remove('-');

						if (palert->ptr[31])
						{
							low.Format("%s", palert->ptr[31].get());
							low.Remove('+'), low.Remove('-');
							bongdata.Format("%s%c%s", high, P_TAB, low);

							m_grid->SetItemText(xrow, m_bongField, bongdata);
						}
					}
				}

				if (m_posField && !bExpect)
				{
					updatePoss = true;
				}
			}

			CString cheg;
			if (m_EqualizerField >= 0 && palert->ptr[32])
			{
				//	cheg = (char*)data[32];
				cheg.Format("%s", palert->ptr[32].get());
				m_grid->SetItemText(xrow, m_EqualizerField, cheg);
			}

			entry.Empty(); // 2012.02.20 KSJ �ʱ�ȭ
		}

	
		if (m_ccField)
			calcInClient(xrow);

		if (/*(bRealData && bTicker) ||*/ m_posField)
		{
			for (int jj = 0; jj < m_gridHdrX.GetSize(); jj++)
			{
				//if (m_posField)
				{
					double dval1{}, dval2{}, dval3{};
					CString str;
					dval1 = dval2 = dval3 = 0.0;
					CString futurnGubn, mCode, strCurr;
					int sizeCode{};

					const _gridHdr gridHdr = m_gridHdrX.GetAt(jj);
					if (gridHdr.needs != 9)
						continue;

					auto& pinters = _vInters.at(xrow - 1);

					mCode = pinters->code;
					mCode.TrimRight();
					mCode.TrimLeft();
					sizeCode = strlen(mCode);

					if (bExpect == FALSE) // ����
					{
						switch (gridHdr.symbol[3])
						{
						case '3': // �򰡼���
							dval1 = IH::TOfabs(pinters->xnum);
							dval2 = IH::TOfabs(pinters->xprc);
							dval3 = IH::TOfabs(m_grid->GetItemText(xrow, colCURR));

							if (dval1 <= 0 || dval2 <= 0)
								continue;

							if (sizeCode == 6 || sizeCode == 9)
							{
								entry = CalcuPyungaSonik(pinters.get(), m_grid->GetItemText(xrow, colCURR));
							}
							else
							{
								futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
								entry = CalFutureEvalPrice(pinters.get(), code, futurnGubn, dval3, dval2, dval1);
							}

							break;

						case '4': // ������
							dval1 = IH::TOfabs(pinters->xnum);
							dval2 = IH::TOfabs(pinters->xprc);
							dval3 = IH::TOfabs(m_grid->GetItemText(xrow, colCURR));

							if (dval1 <= 0 || dval2 <= 0)
								continue;

							if (sizeCode == 6 || sizeCode == 9)
							{
								entry = CalcuSuik(pinters.get(), m_grid->GetItemText(xrow, colCURR));
							}
							else
							{
								futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
								entry = CalFutureEvalRate(pinters.get(), code, futurnGubn, dval3, dval2, dval1);
							}

							break;

						default:
							continue;
						}

						m_grid->SetItemText(xrow, jj, entry);
					}
					else //����ȣ�� �ð�
					{
						if (bAutoCheck)
						{
							int ii = 0;
							switch (gridHdr.symbol[3])
							{
							case '3': // �򰡼���

								dval1 = IH::TOfabs(pinters->xnum);
								dval2 = IH::TOfabs(pinters->xprc);

								for (ii = 0; ii < IARRAYSIZE; ii++)
								{
									//�����ȣ�� ��Ī�ؼ� �����͸� �����ش�
									if (strCode == m_iYDayVol[ii][JMCODE])
									{
										strCurr = m_iYDayVol[ii][TDAYVALUE];
									}
								}

								if (dval1 <= 0 || dval2 <= 0)
									continue;

								if (sizeCode == 6 || sizeCode == 9)
								{
									entry = CalcuPyungaSonik(pinters.get(), strCurr);
								}
								else
								{
									futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
									entry = CalFutureEvalPrice(pinters.get(), code, futurnGubn, dval3, dval2, dval1);
								}

								break;

							case '4': // ������
								dval1 = IH::TOfabs(pinters->xnum);
								dval2 = IH::TOfabs(pinters->xprc);

								for (ii = 0; ii < IARRAYSIZE; ii++)
								{
									//�����ȣ�� ��Ī�ؼ� �����͸� �����ش�
									if (strCode == m_iYDayVol[ii][JMCODE])
									{
										strCurr = m_iYDayVol[ii][TDAYVALUE];
									}
								}

								if (dval1 <= 0 || dval2 <= 0)
									continue;

								if (sizeCode == 6 || sizeCode == 9)
								{
									entry = CalcuSuik(pinters.get(), strCurr);
								}
								else
								{
									futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
									entry = CalFutureEvalRate(pinters.get(), code, futurnGubn, dval3, dval2, dval1);
								}

								break;
							default:
								continue;
							}

							m_grid->SetItemText(xrow, jj, entry);

							entry.Empty(); // 2012.02.20 KSJ �ʱ�ȭ
						}
					}

					/* 2011.12.22 KSJ Blink ���� ������� �ʴ� �Լ���
					if (rtmAction == rtmCELL)
						m_grid->Blink(xrow, jj, rtmColor);
					*/
				}
			}
		}

		// 2012.06.20 KSJ ���, ���� �߰�
		if (palert->ptr[950])
		{

			str950.Format("%s", palert->ptr[950].get());

			if (!str950.IsEmpty() /*&& str950.GetAt(0) != '0'*/)
			{
				if (str950.GetAt(0) == '1')
				{
					m_grid->SetItemData(xrow, colINFO, 12);
				}
				else
				{
					entry = m_grid->GetItemText(xrow, colINFO); // ����Ư�̻���
					CString strName = m_grid->GetItemText(xrow, colNAME);

					SetColInfo(strName, xrow, entry);
				}
			}
			// m_grid->Invalidate(FALSE);
		}

		if (palert->ptr[950]) //���ľ���
		{
			str951.Format("%s", palert->ptr[951].get());
			if (!str951.IsEmpty() /*&& str951.GetAt(0) != '0'*/)
			{
				if (str951 == "12" || str951 == "14" || str951 == "16")
				{
					m_grid->SetItemData(xrow, colINFO, 13);
				}
				else
				{
					entry = m_grid->GetItemText(xrow, colINFO); // ����Ư�̻���
					CString strName = m_grid->GetItemText(xrow, colNAME);
					SetColInfo(strName, xrow, entry);
				}
			}
		}
		// KSJ
	}
	m_grid->endDrawHolding();
}
bool bfind_k001 = false;
void writelog(CString str)
{
	if (bfind_k001)
	{
		OutputDebugString("[2022] " + str + "\r\n");
	}
}

// 2012.01.19 KSJ Alertx �߰�
void CGridWnd::parsingAlertx(LPARAM lParam)
{
	CString strlog;
	if (m_bSending)
	{
		return;
	}
	int xrow{};
	CString code, name, symbol, entry, datB, strValue, dataExceptCode, strCode, strData, sTime;
	CString strGubn; // 2012.02.09 KSJ ȣ�� ü�� ����
	CString strTemp;
	const bool bCheType = false; // 2012.06.20 KSJ ü���϶��� true�� �ٲ��ش�.
	CString str950, str951;
	CString strTmp;
	CString slog;

	struct _alertR *alertR = (struct _alertR *)lParam;
	code = alertR->code;
//AxStd::_Msg();
//AxStd::_Msg("[%s]", code);
	if (code.GetLength() == 7)
	{
		strCode = code.Mid(1);
	}
	else if (code.GetAt(0) == 'X')
	{
		strCode = code.Mid(1);
	}
	else
		strCode = code;

	if (strCode.Find("001") >= 0)
		bfind_k001 = true;

	DWORD *data = (DWORD *)alertR->ptr[0];
	if (data[0])
	{
		strGubn = (char *)data[0];
	}
	// KSJ

	// 2013.08.26 KSJ	�����϶� ���� ǥ��
	/*
		08:30~09:00 'X' ����ǥ��
		09:01~14:50 'J' ���簡ǥ��
		14:50~15:00 'X' ����ǥ��
		15:00~15:01 'J' ���簡ǥ��
	*/

	if (strGubn == "X" && code.GetLength() == 5) //�����϶�
	{
		code.Delete(0);	     //ù ���� 'X'�� �����Ѵ�.
		code.Insert(0, 'K'); //ù ���ڸ� 'K'�� �ٲ��ش�.
		strCode = code;
		strTmp.Format("[202200] code=[%s]\r\n", code);
		OutputDebugString(strTmp);

	}

	// 2013.08.26 KSJ END
	if (code.CompareNoCase("S0000") == 0)
	{
		if (CAST_TREEID(m_kind)->kind == xISSUE)
		{
			InsertNewsx(data);
		}
		else
		{
			parsingNewsx(data);
		}
		return; // 2012.09.11 KSJ �����϶��� �Ʒ����� Ż�ʿ� ����.
	}

	int count = 0;
	count = CheckRealTimeCode(code); // �ߺ�����ó��...
	if (count == 0)
		return;

	//�׷� ���� �ߺ� ����� ���,
	m_grid->beginDrawHolding();

	for (int rowPosition = 0; rowPosition < count; rowPosition++)
	{
		xrow = m_irowCode[rowPosition]; // �ߺ�����ó��...

		//if (!data[34] && data[40]) //ü��ð�...  ȣ���ð�....
		//{
		//	if (!data[111]) // ����ü�ᰡ....
		//		bTicker = FALSE;
		//}

		//if (data[734] || data[740]) // ECN ü��ð�...    ECN ȣ���ð�...
		//{
		//	bTicker = FALSE;
		//}

		entry = _T("");
		CString oldEXP = m_grid->GetItemText(xrow, colEXPECT);
		CString newEXP = _T("");
		const BOOL bManual = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_EXPECT);
		const BOOL bAutoCheck = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_AUTOEXPECT);
		BOOL bTransSymbol = FALSE; // 2012.02.09 KSJ �ɺ�����Ǵ� ��Ȳ �϶� TRUE
		BOOL bDaebi = FALSE;	   // 2012.03.20 KSJ ������ 0�϶� üũ��.
		BOOL bZisu = FALSE;	   // 2013.08.27 KSJ �����϶��� ������ 'X'�̰� ���󰡰� 23�� �´�.

		CString strTime, expect, real, excep;
		CString codeExceptA;

		//������ ���������� �迭�� ������ �� ���簡 ����Ÿ ������Ʈ
		CString en2, saveData;

		if (data[111]) //����ü�ᰡ...
		{
			saveData = (char *)data[111];
			en2 = (char *)data[34]; // ü��ð�...
			strlog.Format("data[111]  �ִ� [%s][%s]", saveData, en2);
			writelog(strlog);
		}
		else if (data[23]) // ü�ᰡ...
		{
			saveData = (char *)data[23];
			en2 = (char *)data[34]; // ü��ð�...
			strlog.Format("data[23]  �ִ� [%s][%s]", saveData, en2);
			writelog(strlog);
			if (!en2.IsEmpty())
			{
//XMSG(why????);
				for (int i = 0; i < MAX_LINE; i++)
				{
					codeExceptA = strCode;
					//�����ȣ�� ��Ī�ؼ� �����͸� �����ش�
					if (codeExceptA == m_iYDayVol[i][JMCODE])
					{
						m_iYDayVol[i][TDAYVALUE] = saveData;
					}
				}
			}
		}
		//�������� �� �������� ����
		// 111�� ���������� 023�� ���������� �Ǵ��ؼ� ���� ����

		entry.Empty();

		CString str90 = (char *)data[90]; // 10���켱ȣ�� �ŵ�
		str90.Trim();
		BOOL bLast = FALSE; // 2015.01.15 ���͸������� ���� ������ ������ �����ؾ���.��

		// 2015.01.15 ����ȣ�� 4 �߰��� 2015.02.03 KSJ 9�� ������� ���� ���͸��� �����Ͱ� ���´�.
		if ((strGubn == "L" || strGubn == "4" || strGubn == "P" || strGubn == "g") && ((str90 == "99") || (str90 == "40")))
		{
			m_grid->SetItemText(xrow, colEXPECT, "0"); // 2013.09.13 KSJ �ɼ����͸������� �������� ���󰡸� �ִµ� �����ؾ��Ѵ�.
			m_grid->SetItemData(xrow, colEXPECT, 0);
			bLast = TRUE;
		}

		const int nEndOPMarket = m_grid->GetItemData(xrow, colEXPECT);	  // 2013.09.17 KSJ �ش������� ������ �Ǿ�����
		if ((strGubn == "X" || data[111] || nEndOPMarket == 1) && !bLast) // ���� ����	2013.08.22 �������󰡴�111�ɺ��� ���� ���а���X�� �´�.
		{
			strlog.Format("���� ���� [%s][%s]", saveData, en2);
			writelog(strlog);

			if (data[111])
				entry = (char *)data[111];
			else if (nEndOPMarket == 1) // 2013.09.17 KSJ �ش������� ������ �Ǿ�����
				entry = " ";	    //���󰡰� 0�� �ö��� ���� ��Ȳ��.
			else
			{
				bZisu = TRUE;
				entry = (char *)data[23]; // 2013.08.22 �������󰡴�111�ɺ��� ���� ���а���X�� �´�.
				strlog.Format("�������󰡴�111�ɺ��� ���� ���а���X�� �´� [%s]", entry);
				writelog(strlog);
			}

			if (entry != "0" && entry != "-0" && entry != "0.00" && entry != "+0" && entry != " 0" && entry != " ")
			{
				m_grid->SetItemText(xrow, colEXPECT, "1");
			}
			else
			{
				writelog("����ð��ε� 23,111 �Ѵ� ���� ������");
				// 2012.05.09 KSJ ���󰡰� 0�� �ö��� ���簡�� �ѷ��ش�.
				m_mapCurValue.Lookup(strCode, strData);
				m_grid->SetItemText(xrow, colEXPECT, "0"); //���� ���
				int i = col7852;

				_gridHdr gridHdr{};
				CString str7852, str7853, str7854, str7855;
				int nSymbol = 0;
				CString strCurrData;
				while (AfxExtractSubString(entry, strData, i, '\t'))
				{
					if (i >= m_gridHdrX.GetSize() - 1) // khs
						break;

					if (entry == "(nullptr)" || entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == " 0" || entry == " ")
						entry = "";

					// 2013.07.01 KSJ ���簡 ����
					gridHdr = m_gridHdrX.GetAt(i);
					symbol = CString(gridHdr.symbol, strlen(gridHdr.symbol));
					CString strsym;
					strsym = symbol;

					if (symbol.GetLength() >= 3)
						symbol = symbol.Right(3);

					nSymbol = atoi(symbol);

					if (i == col7852)
						str7852 = entry;
					else if (i == col7853)
						str7853 = entry;
					else if (i == col7854)
						str7854 = entry;
					else if (i == col7855)
						str7855 = entry;
					else if (i == colMAR || i == colMAR1 || i == colSIG || i == colCODE || i == colNAME /*|| i == colCURR*/) // 2013.07.25 KSJ ���簡�� ���⼭ Ÿ�� �ȵȴ�.
						entry = m_grid->GetItemText(xrow, i);
					else if (nSymbol == 23 || nSymbol == 111) //
						entry = str7852;
					else if (nSymbol == 27 || nSymbol == 112) //
						entry = str7853;
					else if (nSymbol == 024 || nSymbol == 115) //
						entry = str7854;
					else if (nSymbol == 33 || nSymbol == 116) //
						entry = str7855;

					if (strsym == "5401" || strsym == "5402" || strsym == "5403" || strsym == "5404" || strsym == "5405") // khs
					{
						strCurrData = m_grid->GetItemText(xrow, colCURR);
						strCurrData.Replace("+", "");
						strCurrData.Replace("-", "");
						entry = getABRdata((LPSTR)(LPCTSTR)strCurrData, strCode, strsym);
						m_grid->SetItemText(xrow, i, entry);
					}

					if (entry != "0" && entry != "-0" && entry != "0.00" && entry != "+0" && entry != " 0" && entry != " ")
					{
						// 2013.07.01 KSJ
						m_grid->SetItemText(xrow, i++, entry);
					}
					else if (!symbol.Compare("##1") || !symbol.Compare("##2") || !symbol.Compare("##3") || !symbol.Compare("##4")) // 2014.02.12 ��������, ���Դܰ�, �򰡼���, ���ͷ��� ����ڰ� �Է��ϴ� �κ��̹Ƿ� �翬�� ���̴�.
					{
						m_grid->SetItemText(xrow, i, m_grid->GetItemText(xrow, i++));
					}
					else
					{
						m_grid->SetItemText(xrow, i++, " ");
					}
				}


				entry = m_grid->GetItemText(xrow, colCURR);
				// KSJ
				m_grid->endDrawHolding();
				return;
			}

			//���� �����ư�� �ȴ����� ������, ������ �ɺ��� �ٲپ �׸��忡 �����͸� �����ְ� �ִ�
			//(���簡 �ɺ��� �Ǿ��ֱ� ������ )
			//�׷� �ڵ� üũ ������������, �����ָ� �ǰ�
			//�ڵ� üũ �����ÿ�, ���� �Ⱥ����ָ� �ȴ�
			if (!bManual) //���� ��ư �ȴ��� ����
			{
				// replace symbol
				if (bAutoCheck) // �ڵ��� üũ�� ����
				{
					bTransSymbol = TRUE; // 2012.02.09 KSJ ���簡 �ɺ��� ���� �ɺ��� �ٲ�����.
				}
				else
				{
					if (!entry.IsEmpty())
					{
						m_grid->SetItemText(xrow, colEXPECT, "0");
					}
				}
			}
			else
				bTransSymbol = TRUE; // 2012.02.09 KSJ ���簡 �ɺ��� ���� �ɺ��� �ٲ�����.
		}
		else if (data[23])
		{
			entry = (char *)data[23];

			if (!bManual) //���� ��ư �ȴ��� ����
			{
				if (bAutoCheck) // �ڵ��� üũ�� ����
				{
					strTime = (char *)data[34];

					if (m_strBeginTime <= strTime && m_strEndTime >= strTime)
					{
						if (entry != "0" && entry != "-0" && entry != "0.00" && entry != "+0" && entry != " 0" && entry != " ")
						{
							m_grid->SetItemText(xrow, colEXPECT, "1");
						}
						else
						{
							m_grid->SetItemText(xrow, colEXPECT, "0"); // 2012.08.29 KSJ ���� �����. 8:10 ~ 8:30�� ���̿� ü���� ��������
							entry = m_grid->GetItemText(xrow, colCURR);
							m_grid->endDrawHolding();
							return;
						}
					}
					else
					{
						m_grid->SetItemText(xrow, colEXPECT, "0"); //����
					}
				}
			}
			else
			{
				if (!entry.IsEmpty())
				{
					bTransSymbol = TRUE; // 2012.02.09 KSJ ���簡 �ɺ��� ���� �ɺ��� �ٲ�����.

					if (entry != "0" && entry != "-0" && entry != "0.00" && entry != "+0" && entry != " 0" && entry != " ")
					{
						m_grid->SetItemText(xrow, colEXPECT, "1");
					}
					else
					{
						m_grid->SetItemText(xrow, colEXPECT, "0"); // 2012.08.23 KSJ ���� ��� �߰���

						entry = m_grid->GetItemText(xrow, colCURR);
						m_grid->endDrawHolding();
						return;
					}
				}
			}

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 2012.05.10 KSJ ParseOubs���� ������m_mapCurValue�� �����͵��� �ʱⰪ�̹Ƿ� 2012.08.22 ����
			//�ǽð����� ����� ���� ������ �־���Ѵ�. �ŷ���, ü�ᷮ ����..
			//�Ʒ��� ���� ���󰡰� 0�� ���ö��� �������ָ� ��ũ�� ���� �������� �־ ��� ������Ʈ �Ѵ�.
			//���� ȣ�� �ð��϶��� ���� �����Ѵ�.
			//�ڵ��� üũ�Ǿ� �������� �����Ѵ�.

			const int countX = m_gridHdrX.GetSize();
			_gridHdr gridHdr{};

			CString strNewData, strNewTemp;
			strNewData.Empty();

			for (int jj = col7852; jj < countX; jj++)
			{
				gridHdr = m_gridHdrX.GetAt(jj);
				symbol = CString(gridHdr.symbol, strlen(gridHdr.symbol));
				writelog(symbol+"\r\n");
				//���μ������� �ǽð��� �ƴϴ�.
				if (symbol == "2204")
					continue;

				if (symbol.GetLength() >= 3)
					symbol = symbol.Right(3);

				// 2012.11.19 KSJ �����Ͱ� ������ �Ǹ� �ִ´�.
				// 2013.07.01 KSJ ���簡 ����
				if (atoi(symbol) == 0) // 2014.03.18 KSJ �ɺ����� #�� ���� 0�̵Ǿ B,F,G�̷� ���� ����.
					strNewTemp = "";
				else if (data[atoi(symbol)])
					strNewTemp = (char *)data[atoi(symbol)];
				else if (jj == col7852)
					strNewTemp = (char *)data[atoi("023")];
				else if (jj == col7853)
					strNewTemp = (char *)data[atoi("027")];
				else if (jj == col7854)
					strNewTemp = (char *)data[atoi("024")];
				else if (jj == col7855)
					strNewTemp = (char *)data[atoi("033")];
				else
					strNewTemp = "";

				strNewData += "\t" + strNewTemp;
				strlog += "\t[" +   symbol + "]  " + strNewTemp;
			}

writelog("strlog =" + strlog);

			if (m_mapCurValue.Lookup(strCode, strData))
			{
				int nIndex = 0, nCount = 0;
				while (nIndex != -1)
				{
					nIndex = strData.Find(P_TAB, nIndex + 1);
					nCount++;

					if (nCount == col7852)
					{
						strTemp = strData.Left(nIndex);
						strData = strTemp + strNewData;

						m_mapCurValue.SetAt(strCode, strData);
						break;
					}
				}
			}
		}

		newEXP = m_grid->GetItemText(xrow, colEXPECT);
		const BOOL bForceDraw = (newEXP == oldEXP) ? FALSE : TRUE;
		BOOL bExpect = (BOOL)atoi(m_grid->GetItemText(xrow, colEXPECT)); // ������ ������ 0 �̳� 1 �� return
		bool updatePoss = false;
		const int countX = m_gridHdrX.GetSize();
		_gridHdr gridHdr{};
		// LONG ret = 0;

		// 2012.11.08 KSJ �ֽİ� �����ɼ��� �ɺ����� �Ȱ��� ���� �ִ�. ���μ����� ������.
		bool bKospi = true;
		if (strCode.GetLength() == 6)
			bKospi = true;
		else
			bKospi = false;
		// KSJ

		int nSymbol = 0;
		CString strsym;
		for (int ii = 2; ii < countX; ii++)
		{
			gridHdr = m_gridHdrX.GetAt(ii);

			symbol = CString(gridHdr.symbol, strlen(gridHdr.symbol));
			strsym = symbol;

			//���μ������� �ǽð��� �ƴϴ�.
			if (symbol == "2204")
				continue;

			if (symbol.GetLength() >= 3)
				symbol = symbol.Right(3);

			nSymbol = atoi(symbol);

			if (/*data[atoi(symbol)] && */ !bTransSymbol) // 2013.07.08 ����üũ�Ǿ� ������ �ؿ� Ÿ����
			{
				// 2012.03.16 KSJ ����, �ŷ��� �����־ �߰���..
				switch (nSymbol)
				{
				//����
				case 111:
					entry = (char *)data[23];
					break; //���簡
				//����ŷ���
				case 112:
					entry = (char *)data[27];
					break; //�ŷ���
				//�������ϴ��
				case 115:
					entry = (char *)data[24];
					break; //���ϴ��
				//��������
				case 116:
					entry = (char *)data[33];
					break; //�����
				//���μ�����
				case 204:
					entry = bKospi ? entry : " ";
					break;
				// 2014.03.18 KSJ �ɺ����� #�� ���� 0�̵�
				case 0:
					entry = " ";
					break;

				default:
					entry = (char *)data[nSymbol];
					break;
				}
			}
			else if (data[atoi(symbol)] && bZisu) // 2013.08.27 KSJ �����϶��� �ɺ��� �ε����� ��ġ��.
			{
				entry = (char *)data[atoi(symbol)];
			}
			else if (bTransSymbol)
			{
				// 2012.03.16 KSJ ����, �ŷ��� �����־ �߰���..
				switch (nSymbol)
				{
				//����
				case 23:
					entry = (char *)data[111];
					break; //�������簡
				//�ŷ���
				case 27:
					entry = (char *)data[112];
					break; //����ŷ���
				//���ϴ��
				case 24:
					entry = (char *)data[115];
					break; //�������ϴ��
				//�����
				case 33:
					entry = (char *)data[116];
					break; //��������
				//���μ�����
				case 204:
					entry = bKospi ? entry : " ";
					break;
				// 2014.03.18 KSJ �ɺ����� #�� ���� 0�̵�
				case 0:
					entry = " ";
					break;

				default:
					entry = (char *)data[nSymbol];
					break;
				}
			}
			else
				continue;

			if (!bForceDraw && IH::TOf(entry) == IH::TOf(m_grid->GetItemText(xrow, ii)))
			{
				if (entry.GetLength() > 0)
				{
					strlog.Format("�������̶� ���� �ȱ׸��� [%d] [%s] \r\n", ii, entry);
					writelog(strlog);
				}
				continue;
			}

			if (!(gridHdr.attr & GVAT_HIDDEN) && !entry.IsEmpty())
			{
				if ((strcmp("2029", gridHdr.symbol) == 0) ||
				    (strcmp("2030", gridHdr.symbol) == 0) ||
				    (strcmp("2031", gridHdr.symbol) == 0))
				{
					if ((1 == gridHdr.needs) || (3 == gridHdr.needs))
					{
						CString strVal = entry;
						CString strDiff = strVal;

						if (strVal[0] == '+' || strVal[0] == '-')
						{
							strDiff = strVal.Mid(1);
						}

						double dDiffOpen = 0.0;
						double dPClose{}, dVal{};
						dVal = atof(strDiff);
						dPClose = atof((m_grid->GetItemText(xrow, colPCURR)));

						if (dVal != 0 && dPClose != 0)
						{
							dDiffOpen = (dVal - dPClose) / dPClose * 100;

							if (1 == gridHdr.needs)
							{
								if (0 > dDiffOpen)
								{
									entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
								}
								else
								{
									entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
								}
							}
							else if (3 == gridHdr.needs)
							{
								if (0 > dDiffOpen)
								{
									entry.Format("%0.2f%c", dDiffOpen, P_PER);
								}
								else if (0 == dDiffOpen)
								{
									entry.Format(" %0.2f%c", dDiffOpen, P_PER);
								}
								else
								{
									entry.Format(" +%0.2f%c", dDiffOpen, P_PER);
								}
							}
						}
					}
				}
			}

			if ((ii == colCURR) && bExpect && !m_bExpect) // �����ư�� �ȴ������ְ�, ����ȣ����
			{
				entry.TrimLeft();
				entry.TrimRight();

				if ((entry == "0") || (entry == "-0") || (entry == "+0") || (entry == ""))
				{
					entry = " ";

					entry = m_grid->GetItemText(xrow, colCURR);
					m_grid->endDrawHolding();
					return;
				}
			}

			CString tempStr;

			if (ii == colCURR)
			{
				if (code.Find("K0001") > -1)
				{
					m_dKospi = atof(entry);
					tempStr.Format("%.2f", m_dKospi);
					m_dKospi = atof(tempStr);
				}
				else if (code.Find("KQ001") > -1)
				{
					m_dKosdaq = atof(entry);
					tempStr.Format("%.2f", m_dKosdaq);
					m_dKosdaq = atof(tempStr);
				}
			}

			// 2012.03.20 KSJ ���簡 ���յɶ� ���, ����� �������� �ʴ� ���� ����
			if ((strcmp("2115", gridHdr.symbol) == 0) || (strcmp("2024", gridHdr.symbol) == 0) || (strcmp("2116", gridHdr.symbol) == 0) || (strcmp("2033", gridHdr.symbol) == 0))
			{
				entry.TrimLeft();
				entry.TrimRight();
				if (entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == " 0" || entry == "30" || entry == ".00" || entry == "+0.00" || entry == "-0.00") // 2012.07.17 KSJ �ɼ��϶��� ��� �߰�
				{
					bDaebi = TRUE;
				}
			}
			else
			{
				// 2012.03.22 KSJ ��������, ���Դܰ�, �򰡼��� �� �ʵ尪�� #�� ���� ���� atoi���� 0�� ���´�.
				//�׷��� entry�� data[0]���� �̾ƿ��Ƿ� ���� ���а� d,f,b, �̷������� �� �׸��忡 �ѷ�����.
				entry.TrimLeft();
				entry.TrimRight();
				if (entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == " 0" || entry == ".00" || entry == "+0.00" || entry == "-0.00" || atof(entry) == 0 || atof(symbol) == 0) // KSJ 2012.07.17 atoi --> atof�� ����
				{
					if (entry.GetLength() > 0 && atof(entry.Mid(1)) == 0)
						entry = " "; // 2013.09.24 KSJ �������������� ++ +- �� ��ȣ�� ���ڸ��� �����µ� �̶��� atof�� ���� ���ϸ� 0���� ���´�. �׷��� ���ڸ� �ڸ��� ���غ�
				}
			}

			CString tmp;

			if (ii == colCURR)
			{
				if (entry == "")
				{
					entry = m_grid->GetItemText(xrow, colCURR);
				}
			}

			CString strPreValue;

			strPreValue = m_grid->GetItemText(xrow, ii);
			strPreValue.TrimLeft();
			strPreValue.TrimRight();

			// khs

			if (strsym == "5401" || strsym == "5402" || strsym == "5403" || strsym == "5404" || strsym == "5405")
			{
				if (data[23])
				{
					entry = getABRdata((char *)data[23], strCode, strsym);
				}
				else
				{
					CString strcurr;
					strcurr.Format("%s %s\n", (char *)data[23], (char *)data[111]);
					strcurr = m_grid->GetItemText(xrow, colCURR);
					strcurr.Replace("+", "");
					strcurr.Replace("-", "");
					entry = getABRdata((LPSTR)(LPCTSTR)strcurr, strCode, strsym);
				}
			}

			if (ii != colCURR && strPreValue != entry)
			{
				entry.TrimLeft();
				entry.TrimRight();

				if (!entry.IsEmpty()) // 2012.03.20 KSJ ���簡 ���յɶ� ���, ����� �������� �ʴ� ���� ����
				{
					if (bDaebi)
						entry = "";

					bDaebi = FALSE;

					// 2012.02.27 KSJ �׷����� �����Ϳ� �̻��������� ���⸦ üũ�ؾ���
					m_grid->SetItemText(xrow, ii, entry);
				}
			}
			else if (ii == colCURR)
			{
				// 2013.08.23 KSJ �������������̸� ����+, �϶�+, ���- �� ���� ��ȣ�� ǥ��������Ѵ�.
				if (strCode.GetLength() == 8 && (strCode.GetAt(0) == '4' || strCode.GetAt(0) == 'D')) //�Ļ���ǰ �ڵ尳��
				{
					if (entry.GetAt(0) == ' ')	  //ó���� ��ĭ�̸� ���տ� -, + ǥ�ð� �ִ� ���̴�.
						entry.Replace(" ", "��"); //�׶��� '��' �� ���� 1�� �� �־��ش�. �����̽� �ƴ�..
					else if (entry.GetAt(0) == '0')
					{
						entry.Delete(0);
						entry.Insert(0, "��");
					}
				}
				// 2013.08.23 KSJ END
				m_grid->SetItemText(xrow, ii, entry);
			}

			//���ϰŷ������ �ǽð� ���
			CString str2027, str2403;
			double int2027 = 0;
			double int2403 = 0;
			double int2321 = 0;

			if (data[27])
			{
				str2027 = (char *)data[27];

				int2027 = atoi(str2027);
				for (int i = 0; i < IARRAYSIZE; i++)
				{
					//�����ȣ�� ��Ī�ؼ� �����͸� �����ش�
					if (strCode == m_iYDayVol[i][JMCODE])
					{
						int2321 = atoi(m_iYDayVol[i][YDAYVOLUMN]);
					}
				}

				if (int2321 != 0)
				{
					int2403 = (int2027 / int2321) * 100;
				}
				else
				{
					int2403 = 0;
				}

				if (!bExpect && m_yDayVolField >= 0)
				{
					entry.Format("%.2f", int2403);

					m_grid->SetItemText(xrow, m_yDayVolField, entry);
				}
			}

			if (ii == colCURR)
			{
				if (!bExpect && m_bongField >= 0)
				{
					CString bongdata = _T(""), open = _T(""), high = _T(""), low = _T("");

					if (data[29])
					{
						open = (char *)data[29];
						m_grid->SetItemText(xrow, colOPEN, open);
					}

					if (data[30])
					{
						high = (char *)data[30];
						high.Remove('+'), high.Remove('-');

						if (data[31])
						{
							low = (char *)data[31];
							low.Remove('+'), low.Remove('-');
							bongdata.Format("%s%c%s", high, P_TAB, low);

							m_grid->SetItemText(xrow, m_bongField, bongdata);
						}
					}
				}

				if (m_posField && !bExpect)
				{
					updatePoss = true;
				}
			}

			//CString cheg;
			//if (m_EqualizerField >= 0 && data[32])
			//{
			//	cheg = (char *)data[32];

			//	m_grid->SetItemText(xrow, m_EqualizerField, cheg);
			//}

			//entry.Empty(); // 2012.02.20 KSJ �ʱ�ȭ

		} // for �� ��

		//BOOL bRealData = TRUE;
		//CString strTemp;
		//int nTemp = 0;

		// ADD PSH 20070912
		//if (m_tkConfig.m_bApply)
		//{
		//	bRealData = FALSE;

		//	nTemp = m_grid->GetItemData(xrow, colRATE);

		//	if ((m_tkConfig.m_ulimit) && (nTemp == 1))
		//	{
		//		bRealData = TRUE;
		//	}

		//	if ((m_tkConfig.m_up) && (nTemp == 2))
		//	{
		//		bRealData = TRUE;
		//	}

		//	if ((m_tkConfig.m_flat) && (nTemp == 3))
		//	{
		//		bRealData = TRUE;
		//	}

		//	if ((m_tkConfig.m_dlimit) && (nTemp == 4))
		//	{
		//		bRealData = TRUE;
		//	}

		//	if ((m_tkConfig.m_down) && (nTemp == 5))
		//	{
		//		bRealData = TRUE;
		//	}

		//	if (bRealData)
		//	{
		//		strTemp = m_grid->GetItemText(xrow, colTVOL);
		//		nTemp = atoi(strTemp); // 2017.01.25 KSJ GetBuffer(0) ����

		//		if (0 > nTemp)
		//			nTemp = -nTemp;

		//		if (nTemp >= m_tkConfig.m_vol * 1000)
		//		{
		//			bRealData = TRUE;
		//		}
		//		else
		//		{
		//			bRealData = FALSE;
		//		}

		//		//if (bRealData || !m_tkConfig.m_and)
		//		//{
		//		//	if (m_tkConfig.m_and)
		//		//	{
		//		//		strTemp = m_grid->GetItemText(xrow, colCURR);
		//		//		nTemp = nTemp * atoi(strTemp); // 2017.01.25 KSJ GetBuffer(0) ����

		//		//		if (nTemp < 0)
		//		//			nTemp = -nTemp;

		//		//		if (nTemp >= m_tkConfig.m_amt * 10000000)
		//		//		{
		//		//			bRealData = TRUE;
		//		//		}
		//		//		else
		//		//		{
		//		//			bRealData = FALSE;
		//		//		}
		//		//	}

		//		//	//if (bRealData)
		//		//	//{
		//		//	//	if (m_tkConfig.m_price)
		//		//	//	{
		//		//	//		strTemp = m_grid->GetItemText(xrow, colCURR);
		//		//	//		nTemp = atoi(strTemp); // 2017.01.25 KSJ GetBuffer(0) ����

		//		//	//		if (0 > nTemp)
		//		//	//			nTemp = -nTemp;

		//		//	//		if ((nTemp >= m_tkConfig.m_sprc) && (nTemp <= m_tkConfig.m_eprc))
		//		//	//		{
		//		//	//			bRealData = TRUE;
		//		//	//		}
		//		//	//		else
		//		//	//		{
		//		//	//			bRealData = FALSE;
		//		//	//		}
		//		//	//	}
		//		//	//}
		//		//}
		//	}
		//} // if�� ��

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (m_ccField)
			calcInClient(xrow);

		if (/*(bRealData && bTicker) ||*/ m_posField)
		{
			for (int jj = 0; jj < m_gridHdrX.GetSize(); jj++)
			{
				//if (m_posField)
				{
					double dval1{}, dval2{}, dval3;
					CString str;
					dval1 = dval2 = dval3 = 0.0;
					CString futurnGubn, mCode, strCurr;
					int sizeCode{};

					const _gridHdr gridHdr = m_gridHdrX.GetAt(jj);
					if (gridHdr.needs != 9)
						continue;

					auto& pinters = _vInters.at(xrow - 1);

					mCode = pinters->code;
					mCode.TrimRight();
					mCode.TrimLeft();
					sizeCode = strlen(mCode);

					if (bExpect == FALSE) // ����
					{
						switch (gridHdr.symbol[3])
						{
						case '3': // �򰡼���
							dval1 = IH::TOfabs(pinters->xnum);
							dval2 = IH::TOfabs(pinters->xprc);
							dval3 = IH::TOfabs(m_grid->GetItemText(xrow, colCURR));

							if (dval1 <= 0 || dval2 <= 0)
								continue;

							if (sizeCode == 6 || sizeCode == 9)
							{
								entry = CalcuPyungaSonik(pinters.get(), m_grid->GetItemText(xrow, colCURR));
							}
							else
							{
								futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
								entry = CalFutureEvalPrice(pinters.get(), code, futurnGubn, dval3, dval2, dval1);
							}

							break;

						case '4': // ������
							dval1 = IH::TOfabs(pinters->xnum);
							dval2 = IH::TOfabs(pinters->xprc);
							dval3 = IH::TOfabs(m_grid->GetItemText(xrow, colCURR));

							if (dval1 <= 0 || dval2 <= 0)
								continue;

							if (sizeCode == 6 || sizeCode == 9)
							{
								entry = CalcuSuik(pinters.get(), m_grid->GetItemText(xrow, colCURR));
							}
							else
							{
								futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
								entry = CalFutureEvalRate(pinters.get(), code, futurnGubn, dval3, dval2, dval1);
							}

							break;

						default:
							continue;
						}

						m_grid->SetItemText(xrow, jj, entry);
					}
					else //����ȣ�� �ð�
					{
						if (bAutoCheck)
						{
							int ii = 0;
							switch (gridHdr.symbol[3])
							{
							case '3': // �򰡼���

								dval1 = IH::TOfabs(pinters->xnum);
								dval2 = IH::TOfabs(pinters->xprc);

								for (ii = 0; ii < IARRAYSIZE; ii++)
								{
									//�����ȣ�� ��Ī�ؼ� �����͸� �����ش�
									if (strCode == m_iYDayVol[ii][JMCODE])
									{
										strCurr = m_iYDayVol[ii][TDAYVALUE];
									}
								}

								if (dval1 <= 0 || dval2 <= 0)
									continue;

								if (sizeCode == 6 || sizeCode == 9)
								{
									entry = CalcuPyungaSonik(pinters.get(), strCurr);
								}
								else
								{
									futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
									entry = CalFutureEvalPrice(pinters.get(), code, futurnGubn, dval3, dval2, dval1);
								}

								break;

							case '4': // ������
								dval1 = IH::TOfabs(pinters->xnum);
								dval2 = IH::TOfabs(pinters->xprc);

								for (ii = 0; ii < IARRAYSIZE; ii++)
								{
									//�����ȣ�� ��Ī�ؼ� �����͸� �����ش�
									if (strCode == m_iYDayVol[ii][JMCODE])
									{
										strCurr = m_iYDayVol[ii][TDAYVALUE];
									}
								}

								if (dval1 <= 0 || dval2 <= 0)
									continue;

								if (sizeCode == 6 || sizeCode == 9)
								{
									entry = CalcuSuik(pinters.get(), strCurr);
								}
								else
								{
									futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
									entry = CalFutureEvalRate(pinters.get(), code, futurnGubn, dval3, dval2, dval1);
								}

								break;
							default:
								continue;
							}

							m_grid->SetItemText(xrow, jj, entry);

							entry.Empty(); // 2012.02.20 KSJ �ʱ�ȭ
						}
					}

					/* 2011.12.22 KSJ Blink ���� ������� �ʴ� �Լ���
					if (rtmAction == rtmCELL)
						m_grid->Blink(xrow, jj, rtmColor);
					*/
				}
			}
		}

		// 2012.06.20 KSJ ���, ���� �߰�
		if (data[950])
		{
			str950 = (char *)data[950];
			if (!str950.IsEmpty())
			{
				if (str950.GetAt(0) == '1')
				{
					m_grid->SetItemData(xrow, colINFO, 12);
				}
				else
				{
					entry = m_grid->GetItemText(xrow, colINFO); // ����Ư�̻���
					CString strName = m_grid->GetItemText(xrow, colNAME);
					SetColInfo(strName, xrow, entry);
				}
			}
		}

		if (data[951])
		{
			str951 = (char *)data[951];
			if (!str951.IsEmpty())
			{
				if (str951 == "12" || str951 == "14" || str951 == "16")
				{
					m_grid->SetItemData(xrow, colINFO, 13);
				}
				else
				{
					entry = m_grid->GetItemText(xrow, colINFO); // ����Ư�̻���
					CString strName = m_grid->GetItemText(xrow, colNAME);
					SetColInfo(strName, xrow, entry);
				}
			}
		}
		// KSJ
	}
	m_grid->endDrawHolding();
}

CString CGridWnd::getABRdata(const char *pcurr, CString strCode, CString strsym)
{
	int idata = 0;
	if (pcurr == nullptr)
		idata = 0;
	else
		idata = gsl::narrow_cast<int>(atof((char *)pcurr));

	CString slog;
	CString entry;
	entry = "";
	CMapStringToString *m_symmap;
	if (m_mapCodeABR.Lookup(strCode, (CObject *&)m_symmap))
	{
		CString strdata, strCurr;
		if (idata == 0)
		{
			strCurr = "0";
			m_symmap->Lookup("23", strCurr);
			idata = gsl::narrow_cast<int>(atof(strCurr));
		}

		if (m_symmap->Lookup(strsym, strdata))
		{
			if (strdata.IsEmpty() || strdata == "0")
				entry = "0";
			else
			{
				const double dData = abs(idata) - atof(strdata);
				entry.Format("%.2lf", dData / atof(strdata) * 100); //��ȭ�� / ��ȸ�� * 100
			}

			if (atof(entry) > 0)
				entry = "+" + entry;
			else if (atof(entry) == 0)
				entry = "";
		}
	}

	return entry;
}
// 2012.01.19 KSJ Alertx �߰� ��

void CGridWnd::SetColInfo(CString strName, int nRow, CString strEntry) // 2012.11.02 KSJ ����Ư�̻��� ����
{
	if (strName.GetAt(0) == 'X')
	{
		m_grid->SetItemData(nRow, colINFO, 10);
	}
	else if ("����" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 11);
	}
	else if ("�ܱ�" == strEntry) // 2012.10.31 KSJ �ܱ���� �߰�
	{
		m_grid->SetItemData(nRow, colINFO, 14);
	}
	else if ("��������" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 3);
	}
	else if ("���ڰ��" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 4);
	}
	else if ("���迹��" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 15);
	}
	else if ("��������" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 5);
	}
	else if ("�޵�" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 24);
	}
	else if ("��������ȯ��" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 7);
	}
	else if (strName.GetAt(0) == '&')
	{
		m_grid->SetItemData(nRow, colINFO, 1);
	}
	else if ("����" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 22);
	}
	else if (strName.GetAt(0) == '%')
	{
		m_grid->SetItemData(nRow, colINFO, 8);
	}
	else if (strName.GetAt(0) == '@')
	{
		m_grid->SetItemData(nRow, colINFO, 9);
	}
	else if ("�Ǹ�" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 16);
	}
	else if ("���" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 17);
	}
	else if ("�׸�" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 18);
	}
	else if ("�պ�" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 20);
	}
	else if ("����" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 21);
	}
	else if ("����������" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 23);
	}
	else // 2011.12.06 KSJ ��ġ�ϴ°� ������ 0���� �������ش�.
	{
		m_grid->SetItemData(nRow, colINFO, 0);
	}
}

void CGridWnd::alertBMsuik()
{
	for (int ii = 1; ii < m_rowC; ii++)
	{
		//��õ BM���ͷ� ���
		//���ͷ� = 100.0 * (�������� - BM����) / BM����
		if (m_recomBMSuikrateField >= 0)
		{
			CString strBMjisu, suik;
			double dBMjisu = 0;
			double result = 0;

			strBMjisu = m_grid->GetItemText(ii, m_recomBMjisuField);
			dBMjisu = atof(strBMjisu);

			CString upjong;

			if (ii > 0)
			{
				upjong = GetRecommandInfo(ii);
			}

			m_dKospi = fabs(((CGroupWnd *)GetParent())->GetGridWndUpjn()->m_dKospi);
			m_dKosdaq = fabs(((CGroupWnd *)GetParent())->GetGridWndUpjn()->m_dKosdaq);

			if (!upjong.IsEmpty() && m_dKospi > 0 && m_dKosdaq > 0)
			{

				if (atoi(upjong) == 1)
				{
					result = ((m_dKospi - dBMjisu) / dBMjisu) * 100;
				}
				else if (atoi(upjong) == 2)
				{
					result = ((m_dKosdaq - dBMjisu) / dBMjisu) * 100;
				}

				if (result > 0)
				{
					suik.Format("+%.2f", result);
				}
				else
				{
					suik.Format("%.2f", result);
				}

				m_grid->SetItemText(ii, m_recomBMSuikrateField, suik);
			}
		}
	}
}

void CGridWnd::calcInClient()
{
	CString code;
	for (int ii = 1; ii < m_rowC; ii++)
	{
		code = m_grid->GetItemText(ii, colCODE);
		code.TrimLeft();
		code.TrimRight();
		if (code.IsEmpty())
			continue;

		calcInClient(ii);
	}
}

void CGridWnd::calcInClient(int row)
{
	CString string, stringx;
	double curr{}, open{}, high{}, low{};

	curr = IH::TOfabs(m_grid->GetItemText(row, colCURR));
	open = IH::TOfabs(m_grid->GetItemText(row, colOPEN));
	high = IH::TOfabs(m_grid->GetItemText(row, colHIGH));
	low = IH::TOfabs(m_grid->GetItemText(row, colLOW));

	for (int ii = 0; ii < m_gridHdrX.GetSize(); ii++)
	{
		const _gridHdr gridHdr = m_gridHdrX.GetAt(ii);

		if (gridHdr.needs != 8)
			continue;

		string = _T("");

		switch (gridHdr.symbol[2])
		{
		case '1': // �ð����(��)
			if (curr <= 0. || open <= 0.)
				break;
			string.Format("%+.0f", curr - open);
			break;
		case '2': // �ð����(%)
			if (curr <= 0. || open <= 0.)
				break;
			string.Format("%+.2f%%", (curr - open) / curr * 100);
			break;
		case '3': // �����(��)
			if (curr <= 0. || high <= 0.)
				break;
			string.Format("%+.0f", curr - high);
			break;
		case '4': // �����(%)
			if (curr <= 0. || high <= 0.)
				break;
			string.Format("%+.2f%%", (curr - high) / curr * 100);
			break;
		case '5': // �������(��)
			if (curr <= 0. || low <= 0.)
				break;
			string.Format("%+.0f", curr - low);
			break;
		case '6': // �������(%)
			if (curr <= 0. || low <= 0.)
				break;
			string.Format("%+.2f%%", (curr - low) / curr * 100);
			break;
		}
		stringx = m_grid->GetItemText(row, ii);
		if (atof(stringx) != atof(string))
		{
			m_grid->SetItemText(row, ii, string);
			/* 2011.12.22 KSJ Blink ������� �ʴ� �Լ���
			if (((CGroupWnd*)m_pGroupWnd)->GetRtmAction() == rtmCELL)
				m_grid->Blink(row, ii, ((CGroupWnd*)m_pGroupWnd)->GetRTMColor());
			*/
		}
	}
}

#include "mmsystem.h"
#pragma comment(lib, "winmm")

int CGridWnd::CheckRealTimeCode(CString code)
{
	class CIndexMap *idx = nullptr;

	const int realtimeCol = 0;
	CString string;

	constexpr int count = 0;
	int ii = 0;

	for (ii = 0; ii < IARRAYSIZE; ii++) // 2012.09.10 KSJ 200 -> 110���� ����
		m_irowCode[ii] = 0;

	// �ӵ����� Search ��� ����
	if (m_pSearchMap.GetCount() > 0)
	{
		if (!m_pSearchMap.Lookup(code, (CObject *&)idx))
			return count;

		for (ii = 0; ii < idx->idxCnt; ii++)
			m_irowCode[ii] = idx->index[ii];
		return idx->idxCnt;
	}
	return count;
}

// 2011.12.29 KSJ
void CGridWnd::ReSetSearchMap()
{
	class CIndexMap *idx = nullptr;
	const int realtimeCol = 0;
	CString string, strTemp;

	int count = 0, ii = 0;
	for (count = 0, ii = 1; ii < m_grid->GetRowCount(); ii++)
	{
		string = m_grid->GetItemText(ii, realtimeCol);
		m_irowCode[count++] = ii;

		string.Trim();
		if (string.IsEmpty())
			continue;

		_mapSymbol.emplace(std::make_pair(string, 1));

		// map�� �ߺ� ������ üũ
		if (!m_pSearchMap.Lookup(string, (CObject *&)idx))
			idx = new CIndexMap();

		if (idx->idxCnt >= maxIDX)
			continue;

		idx->index[idx->idxCnt++] = ii;
		m_pSearchMap.SetAt(string, (CObject *)idx);
	}
}

void CGridWnd::ClearSearchMap()
{
	POSITION pos{};
	class CIndexMap *idx = nullptr;
	CString code = "";
	for (pos = m_pSearchMap.GetStartPosition(); pos != nullptr;)
	{
		m_pSearchMap.GetNextAssoc(pos, code, (CObject *&)idx);
		delete idx;
		idx = nullptr;
	}
	m_pSearchMap.RemoveAll();
}

void CGridWnd::ClearInterest()
{
	_mapSymbol.clear();
	_vInters.clear();
}


// ADD PSH 20070913
void CGridWnd::CopyInter(_intersx *pCpy, _intersx *pSrc)
{
	*pCpy = *pSrc;
}

void CGridWnd::SetMarkerProp(_marker *pProp)
{
	if (nullptr != pProp)
	{
		BOOL bChanged = FALSE;

		if ((m_clrMarkerBKG != pProp->clrMarkerBKG) ||
		    (m_clrMarkerTXT != pProp->clrMarkerTXT) ||
		    (m_bShadow != pProp->bShadow))
		{
			bChanged = TRUE;
		}

		if (bChanged)
		{
			m_clrMarkerBKG = pProp->clrMarkerBKG;
			m_clrMarkerTXT = pProp->clrMarkerTXT;
			m_bShadow = pProp->bShadow;

			m_grid->SetBkMarkerColor(m_clrMarkerBKG);
			m_grid->SetMarkerColor(m_clrMarkerTXT);
			m_grid->SetMarkerShadow(m_bShadow);

			m_grid->Invalidate(false);
		}
		m_bAllApply = pProp->bAllApply;
	}
}

void CGridWnd::MarkerSetup()
{
	int nRow = 1;
	for_each(_vInters.begin(), _vInters.end(), [&](auto& pInter) {
		UINT attr = m_grid->GetItemAttr(nRow++, colNAME);
		if ('m' == pInter->gubn)
			attr |= GVAT_MARKER;
		else
			attr &= ~GVAT_MARKER;

		m_grid->SetItemAttr(nRow, colNAME, attr);
	});
}

void CGridWnd::RemoveAllBaseInters()
{
	_arrBaseInters.clear();
}
// END ADD

void CGridWnd::AddInters(std::shared_ptr<struct _intersx> pinters)
{
	if (pinters == nullptr)
		return;

	return _vInters.push_back(pinters);
}

void CGridWnd::RemoveInters(int nIndex)
{
	if (nIndex == -1)
		return; // 2013.08.07 KSJ -1�̸� ����
	const int count = _vInters.size();
	if (count > nIndex)
	{
		auto& pinters = _vInters.at(nIndex);
		if (pinters->code.GetLength())
		{
			CString code = pinters->code;
			// ADD PSH 20070913	2013.06.03 KSJ m_arrBaseInters�� ���� ������ �ʿ䰡 ����.
			if (!m_bSorting)
			{

				auto et = std::remove_if(_arrBaseInters.begin(), _arrBaseInters.end(), [&code](auto& item) {
					if (item && code.CompareNoCase(item->code) == 0)
						return true;
					return false;
				});

				if (et != _arrBaseInters.end())
					_arrBaseInters.erase(et);
			}
			m_mapMargin.RemoveKey(pinters->code);
		}

		_vInters.erase(_vInters.begin() + nIndex);
	}
	//insertInterest(MAX_LINE - 1);
}

void CGridWnd::SetKind(UINT kind)
{
	m_kind = kind;
	m_szTitle.Format(" %s", (char *)m_pTreeWnd->SendMessage(WM_MANAGE, MK_GETDATATITLE, kind));

	InvalidateRect(m_rcTitle, FALSE);
	m_grid->SetSortColumn(-1);
}

CString CGridWnd::FindTitle(UINT kind)
{
	CString title = _T("");
	if (m_pTreeWnd)
		title.Format(" %s", (char *)m_pTreeWnd->SendMessage(WM_MANAGE, MK_GETDATATITLE, kind));

	return title;
}
void CGridWnd::RecvRTS(char *ptr)
{
	//	parsingAlert(ptr);
}

void CGridWnd::RecvRTSx(LPARAM lParam)
{
	parsingAlertx(lParam);
}

void CGridWnd::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1000)
	{
		const int ncnt = m_arExist.GetCount();
		if (ncnt < 0)
		{
			KillTimer(1000);
			return;
		}
		int nIndex = atoi(m_arExist.GetValue(m_nTurn));
		CString code = m_grid->GetItemText(nIndex + 1, colCODE);
		CString string;

		switch (GetCodeType(code))
		{
		case kospiCODE:
			string.Format("%s\t%s", HCOD, code);
			break;
		case futureCODE:
			string.Format("%s\t%s", FCOD, code);
			break;
		case optionCODE:
			string.Format("%s\t%s", OCOD, code);
			break;
		case foptionCODE:
			string.Format("%s\t%s", PCOD, code);
			break;
		case indexCODE:
			string.Format("%s\t%s", UCOD, code);
			break;
		case sinjuCODE:
			string.Format("%s\t%s", SINJUCOD, code);
			break;
		case elwCODE:
			string.Format("%s\t%s", ELWCOD, code);
			break;
		case 0:							  // 2012.10.11 KSJ �ֽ�, �ݸ�, ��ȭ���� �߰�
			if (code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //�ֽ�, �ݸ�, ��ȭ����//�Ļ���ǰ �ڵ尳��
			{
				int nGubn = atoi(code.Mid(1, 2));
				CString strGubn = code.Mid(1, 2); // 2014.10.21 KSJ �ֽļ��� �ű����� ����

				if (nGubn > 10 && nGubn < 60) //�ֽļ���
				{
					string.Format("%s\t%s", "ed_fcod", code); // 2013.01.02 KSJ 2300��������ȭ�鿡�� Ʈ���ź���
					Variant(triggerCC, string);

					string.Format("%s\t%s", SFOD, code);
					break;
				}
				else if (nGubn > 60 && nGubn < 70) //�ݸ�����
				{
					string.Format("%s\t%s", SFOD, code);
					break;
				}
				else if (nGubn > 70 && nGubn < 80) //��ȭ����
				{
					string.Format("%s\t%s", SFOD, code);
					break;
				}
				else if (strGubn >= "B0" && strGubn <= "ZZ") // 2014.10.21 KSJ �ֽļ��� �ű����� ����
				{
					string.Format("%s\t%s", "ed_fcod", code); // 2013.01.02 KSJ 2300��������ȭ�鿡�� Ʈ���ź���
					Variant(triggerCC, string);

					string.Format("%s\t%s", SFOD, code);
					break;
				}
			}
		}

		Variant(triggerCC, string);
		m_grid->SetFocus();
		m_grid->SetSelectRow(nIndex + 1);
		m_nTurn++;
		m_nTurn = m_nTurn % ncnt;
	}
	CBaseWnd::OnTimer(nIDEvent);
}

void CGridWnd::rebuildInterest()
{
	std::copy(_vInters.begin(), _vInters.end(), std::back_inserter(_arrBaseInters));
}

void CGridWnd::Reset(bool bAll)
{
	KillTimer(1000);
	RemoveAll();
	const int nActive = (int)m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETGROUP, MO_ACTIVE));
	if (nActive == m_nIndex)
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET));
	m_grid->Clear();

	if (bAll)
	{
		m_szTitle = _T("");
		SetKind(0);
	}
}

//ȭ�� ��ȣ tab ���� tab ���� ��ȣ | ���� ��ȣ | ���� ��ȣ
void CGridWnd::sendtoMutiHoga()
{
	int nScnt = 0;
	CString code(_T("")), string(_T("")), data(_T("")), string_code(_T(""));
	const char chTab = P_TAB;
	const char chLf = P_NEW;

	int i = 0;
	for (i = _vInters.size() - 1; i >= 0; i--)
	{
		auto& pInters = _vInters.at(i);
		if (!pInters->code.IsEmpty())
			break;
	}

	if (i < 0)
		nScnt = 0;
	else
		nScnt = i;

	int rowCount = 0;


	for (i = 0; i <= nScnt; i++)
	{
		auto& pInters = _vInters.at(i);

		if (pInters->code[0] != 'm')
		{
			if (!pInters->code.IsEmpty())
			{
				string_code += pInters->code;
				string_code += "|";
				rowCount++;
			}
		}
	}

	CInterDomino dlgInter(m_pMainWnd, rowCount, string_code);
	dlgInter.DoModal();
}


void CGridWnd::saveServer(const std::vector<std::shared_ptr<struct _intersx>>& vInters, int gno)
{
	if (gno < 0)
	{
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			gno = CAST_TREEID(m_kind)->value;
		else
			return;
	}
	const int nScnt = GetRowValidcount(vInters);

	std::string buffer;
	buffer.resize(sz_updn + sz_jinfo * nScnt + 1, ' ');


	struct _updn* updn = (struct _updn*)buffer.data();
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'G';

	CopyMemory(updn->uinfo.nblc, _T("00001"), sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'O';

	CString sNum = AxStd::FORMAT("%02d", gno);
	CString strGrouptName = GetGroupName(gno);

	CopyMemory(updn->ginfo.gnox, sNum, sizeof(updn->ginfo.gnox));
	CopyMemory(updn->ginfo.gnam, (LPCTSTR)strGrouptName, min(sizeof(updn->ginfo.gnam), strGrouptName.GetLength()));

	sNum.Format("%04d", nScnt);
	CopyMemory(updn->ginfo.jrec, sNum, sizeof(updn->ginfo.jrec));

	const gsl::span<struct _jinfo> spanInter((struct _jinfo*)(buffer.data() + sz_updn), nScnt);

	int ii = 0;
	for_each(spanInter.begin(), spanInter.end(), [&](auto& jinfo){
		auto& pInters = vInters.at(ii++);
		if (!pInters->code.IsEmpty())
		{
			FillMemory(&jinfo, sz_jinfo, ' ');
			jinfo.gubn[0] = pInters->gubn > 0 ? pInters->gubn : '0';
			CopyMemory(jinfo.code, pInters->code, min(sizeof(jinfo.code), pInters->code.GetLength()));

			if (!pInters->xprc.IsEmpty())
				CopyMemory(jinfo.xprc, pInters->xprc, min(sizeof(jinfo.xprc), pInters->xprc.GetLength()));
			if (!pInters->xnum.IsEmpty())
				CopyMemory(jinfo.xnum, pInters->xnum, min(sizeof(jinfo.xnum), pInters->xnum.GetLength()));
		}
	});

	char key{};
	_trkey *trkey = (struct _trkey *)&key;
	trkey->kind = TRKEY_GRIDSAVE;
	trkey->group = m_nIndex;

	CSendData sData;
	sData.SetData(trUPDOWN, key, buffer.data(), buffer.size(), "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);
	m_pMainWnd->SendMessage(WM_MANAGE, MK_PROCDLL);

	if (CAST_TREEID(m_kind)->kind == xINTEREST)
		m_bEditWork = true;

	CString string = "OnPortfolio\tok";
	m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)string);

}

void CGridWnd::saveBookMark(int gno)
{
	CString strTemp, strBook;
	strBook.Format("%s/%s/%s/bookmark.i%02d", m_root, USRDIR, m_user, gno);

	const int nScnt = GetRowValidcount();
	bool bSetBookMark = false;		// 2014.06.05 KSJ ���࿡ �ϸ�ũ�� �ϳ��� �����Ǿ� ���� ������ ������ �ʿ䰡 ����.

	::DeleteFile(strBook);
	CFile file;
	if (file.Open(strBook, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone))
	{
		// 2013.05.21 KSJ pInters�� �ϸ�ũ�� �� row�� pBInters���� ã�Ƽ� �־��ش�.
		auto& pInters = _vInters.at(m_nBookMarkRow);
		CString code = pInters->code;

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 2013.05.21 KSJ �ϸ�ũ�����Ҷ��� �ϸ�ũ ���ϸ� ���� ���̽��� �������� �����ϵ���
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto ft = std::find_if(_arrBaseInters.begin(), _arrBaseInters.end(), [&code](auto& item) {
			if (code.CompareNoCase(item->code) == 0)
				return true;
			return false;
		});
		if (ft != _arrBaseInters.end())
			(*ft)->bookmark = pInters->bookmark == '1' ? '1' : '0'; // 2015.04.03 KSJ 1�̾ƴϸ� 0���� ���ش�.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////

		std::for_each(_vInters.begin(), _vInters.end(), [&](auto& pInters) {
			if (!pInters->code.IsEmpty())
			{
				//�ϸ�ũ�� ���� ���Ϸ� ����
				struct _bookmarkinfo binfo;
				FillMemory(&binfo, sizeof(_bookmarkinfo), ' ');

				if (pInters->code.IsEmpty())
				{
					binfo.bookmark[0] = '0';
				}
				else
				{
					CopyMemory(binfo.code, pInters->code, min(codelen, pInters->code.GetLength()));
					CopyMemory(binfo.name, pInters->name, min(namelen, pInters->name.GetLength()));
					binfo.bookmark[0] = pInters->bookmark == '1' ? '1' : '0'; // 2015.04.03 KSJ 1�̾ƴϸ� 0���� ���ش�.
					if (pInters->bookmark == '1' || pInters->code[0] == 'm')  // 2015.05.31 KSJ  å���ǵ� bookmark.i �� ����ȴ�.
						bSetBookMark = true;
				}
				file.Write(&binfo, sizeof(_bookmarkinfo));
			}
		});
		file.Close();
	}

	// 2014.06.05 KSJ �ϸ�ũ�� �����Ǿ� ���� ������ ������ �ʿ䰡 ��� �����Ѵ�.
	if (!bSetBookMark)
		::DeleteFile(strBook);

}

void CGridWnd::saveInterest(bool btmp, int gno, bool bBookMark) // 2013.05.14 KSJ �ϸ�ũ�����Ҷ��� ���Ҹ�� �����ϵ���.
{
	if (gno < 0)
	{
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			gno = CAST_TREEID(m_kind)->value;
		else
			return;
	}

	CWnd *pToolWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int nOver = (int)pToolWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	////���ҷ� �������� ���
	if (nOver == MO_VISIBLE && !bBookMark)
	{
		//���ҷ� �������� �ִ� �׷� �����ϱ�
		const auto [gno, arr] = ((CGroupWnd*)m_pGroupWnd)->WriteFileSumtoEachGroup(m_kind);
		//������ �ø���
		saveServer(arr, gno);
		return;
	}

	if (!bBookMark)
		saveServer(_vInters, gno);

	saveBookMark(gno);
}

int CGridWnd::GetRowValidcount(const std::vector<std::shared_ptr<struct _intersx>> &inter)
{
	const int maxRow = inter.size() - 1;
	int i = 0;
	for (i = maxRow; i >= 0; i--)
	{
		auto& pInters = inter.at(i);
		if (!pInters->code.IsEmpty())
			break;
	}
	return i + 1;
}

int CGridWnd::GetRowValidcount()
{
	const int maxRow = _vInters.size() - 1;
	int i = 0;
	for (i = maxRow; i >= 0; i--)
	{
		auto& pInters = _vInters.at(i);
		if (!pInters->code.IsEmpty())
			break;
	}
	return i + 1;
}



CString CGridWnd::GetcolName(int index)
{
	return m_grid->GetItemText(index + 1, colNAME);
}

CString CGridWnd::GetGroupName(int gno)
{
	auto& map = ((CGroupWnd*)m_pGroupWnd)->_GroupName;
	auto it = map.find(gno);

	if (it != map.end())
		return it->second;

	return AxStd::FORMAT("%s%02d", "��������", gno);
}

// ������������ ��ư ���ý�
void CGridWnd::saveInterestX()
{
	CString code, bookmark, string, stringx;
	struct _intersx *pinters{};

	CWnd *pTreeWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	CWnd *pGroupWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
	const int nOver = (int)pTreeWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

	// 2012.02.09 KSJ ���⼭ �� m_inters�� ���ϴ��� �𸣰ڴ�..
	//�������δ� BaseSort���� ������ �Ұ� ������..
	// rebuildInterest�� �Ⱦ��� ������ �ּ�ó���ϴϱ� ����ε�..
	const int scnt = GetRowValidcount();

	if (scnt == 0)
	{
		if (!m_bPlusMinus) // 2011.12.05 KSJ  �׷� �߰� �����ÿ��� �޼��� �ȶߵ���
			Variant(guideCC, "���õ� �׷쿡 ������ �����ϴ�.");
		return;
	}

	if (CAST_TREEID(m_kind)->kind == xINTEREST)
	{
		//�������� ������� �Ǵ��ϴ� ���� �߰�
		const bool isEdited = ((CGroupWnd *)m_pGroupWnd)->isModified(GetKind());

		if (m_bEditWork)
		{
			// 'b' - 'G' - 'V'
			saveInterest(true);
			m_bEditWork = FALSE;
		}
		else
		{
			//�������� ������� �Ǵ��ϴ� ���� �߰�
			if (isEdited == TRUE)
			{
				saveInterest(true);
			}
			else
			{
				if (!m_bPlusMinus) // 2011.12.05 KSJ  �׷� �߰� �����ÿ��� �޼��� �ȶߵ���
				{
					string = "�ش� ���ɱ׷��� ����� ������ �����ϴ�";
					m_pMainWnd->SendMessage(WM_MANAGE, MK_SHOWTOOLTIP, (LPARAM)string.operator LPCTSTR());
				}
			}
		}
	}
	else
	{
		//�׷�� �˻��ؼ� ���������� vol �߰��ϱ�
		stringx = makeGroupName();
		for (int ii = 0; ii < scnt; ii++)
		{
			auto& pinters = _vInters.at(ii);
			code = CString(pinters->code, sizeof(pinters->code)).Trim();
			if (pinters->bookmark != '1')
				pinters->bookmark = '0';

			string.Format("%s%c%c%c", code, P_DELI, pinters->bookmark, P_DELI);
			stringx += string;
		}

		CString tem;
		tem.Format("[%s]", stringx);

		string.Format("%s /p5 /S/d%s%c%s%c", MAP_CONFIG, "appendParentGROUP", P_TAB, stringx, P_NEW);
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_OPENSCR, typePOPUP), (LPARAM)string.operator LPCTSTR());
	}
}

CString CGridWnd::makeGroupName()
{
	bool isTrue = false;
	CString name = _T("����������");

	auto& map = ((CGroupWnd*)m_pGroupWnd)->_GroupName;
	int makeGNo = 0;
	for_each(map.begin(), map.end(), [&](const auto& item){
		if (!item.second.IsEmpty())
		{
			CString gName = item.second;
			CString gnameRTrim;
			CString gnameLTrim = gName.Left(10);
			if (item.second.Left(10).CompareNoCase("����������") == 0)
			{
				isTrue = true;
				if (gName.GetLength() > 11)
				{
					gnameRTrim = gName.Mid(10, 2);
				}
				else
				{
					gnameRTrim = "0";
				}

				if (!gnameLTrim.IsEmpty())
				{
					int idx = atoi(gnameRTrim);
					if (idx > makeGNo)
					{
						makeGNo = idx;
					}
				}
			}
		}
	});

	if (isTrue == true)
		makeGNo++;
	else
		makeGNo = 1;

	CString stringx;
	stringx.Format("%s%02d%c", name, makeGNo, P_DELI);
	return stringx;
}

LRESULT CGridWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_ERASEBKGND)
		return 1;
	return CBaseWnd::WindowProc(message, wParam, lParam);
}

// int counts = 0;
// CString datas[6] = {"A000660\t023\t+3333\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t111\t+2222\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n",
// 					"A000660\t111\t-2222\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t023\t+4444\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n",
// 					"A000660\t023\t+3334\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t111\t+2223\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n",
// 					"A000660\t111\t-2223\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t023\t+4445\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n",
// 					"A000660\t023\t+3335\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t111\t+2224\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n",
// 					"A000660\t111\t-2224\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t023\t+4446\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n"};
BOOL CGridWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_BT_RELOAD:
	{
		SendMessage(WM_MANAGE, MK_CLICKGROUP);
		m_pToolWnd->PostMessage(WM_MANAGE, MK_REFRESH);
	}
	break;
	case IDC_BT_DOMINO:
		sendtoMutiHoga();
		break;
	case IDC_BT_SAVE:
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)m_nIndex);
		saveInterestX();
		break;
	case IDC_BT_ERASE:
	{
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_CLOSEGRID, m_nIndex));
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_INITGRID);

		if (((CGroupWnd *)m_pGroupWnd)->GetGroupCount() <= 1)
			m_pToolWnd->SendMessage(WM_MANAGE, MK_DISABLE_MINUS); // 2015.08.04 KSJ �׸����� 'X' ��ư�� ��������
	}
	break;
	}
	return CBaseWnd::OnCommand(wParam, lParam);
}

void CGridWnd::DrawGradient(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight)
{
	CRect rect;
	int nWidth{}, nHeight{};

	drawRC.right += 1;
	drawRC.bottom += 1;
	nWidth = drawRC.Width();
	nHeight = drawRC.Height();

	int rs{}, gs{}, bs{};
	rs = GetRValue(sColor);
	gs = GetGValue(sColor);
	bs = GetBValue(sColor);

	int re{}, ge{}, be{};
	re = GetRValue(eColor);
	ge = GetGValue(eColor);
	be = GetBValue(eColor);

	float rStep{}, gStep{}, bStep{};
	CPen *sPen{};
	if (bRight)
	{
		if (!nWidth)
		{
			CPen cPen(PS_SOLID, 1, sColor);
			sPen = pDC->SelectObject(&cPen);

			pDC->MoveTo(drawRC.left, drawRC.top);
			pDC->LineTo(drawRC.right, drawRC.bottom);
			pDC->SelectObject(sPen);
			cPen.DeleteObject();
			return;
		}

		rStep = float(rs - re) / float(nWidth);
		gStep = float(gs - ge) / float(nWidth);
		bStep = float(bs - be) / float(nWidth);

		if (!nHeight)
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				CPen cPen(PS_SOLID, 1, RGB(rs - rStep * ii, gs - gStep * ii, bs - bStep * ii));
				sPen = pDC->SelectObject(&cPen);

				pDC->MoveTo(drawRC.left + ii, drawRC.top);
				pDC->LineTo(drawRC.left + ii + 1, drawRC.top);
				pDC->SelectObject(sPen);

				cPen.DeleteObject();
			}
		}
		else
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				rect.SetRect(drawRC.left + ii, drawRC.top, drawRC.left + ii + 1, drawRC.bottom);

				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep * ii, gs - gStep * ii, bs - bStep * ii));
				pDC->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
	else
	{
		if (!nHeight)
		{
			CPen cPen(PS_SOLID, 1, sColor);
			sPen = pDC->SelectObject(&cPen);

			pDC->MoveTo(drawRC.left, drawRC.top);
			pDC->LineTo(drawRC.right, drawRC.bottom);
			pDC->SelectObject(sPen);
			cPen.DeleteObject();
			return;
		}

		rStep = float(rs - re) / float(nHeight);
		gStep = float(gs - ge) / float(nHeight);
		bStep = float(bs - be) / float(nHeight);

		if (!nWidth)
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				CPen cPen(PS_SOLID, 1, RGB(rs - rStep * ii, gs - gStep * ii, bs - bStep * ii));
				sPen = pDC->SelectObject(&cPen);

				pDC->MoveTo(drawRC.left, drawRC.top + ii);
				pDC->LineTo(drawRC.left, drawRC.top + ii + 1);
				pDC->SelectObject(sPen);

				cPen.DeleteObject();
			}
		}
		else
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				rect.SetRect(drawRC.left, drawRC.top + ii, drawRC.right, drawRC.top + ii + 1);

				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep * ii, gs - gStep * ii, bs - bStep * ii));
				pDC->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
}

void CGridWnd::Assign(CGridWnd *pGrid)
{
	const int nActive = (int)m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETGROUP, MO_ACTIVE));
	if (nActive == m_nIndex)
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET));
	m_grid->Clear();

	m_szTitle = _T("");
	SetKind(0);

	SetKind(pGrid->GetKind());
	m_gridHdrX.RemoveAll();

	//�׸����� row �缳��
	RemoveAll();

	pGrid->GetHearder(m_gridHdrX);
	auto& arr = pGrid->getArray(); // m_inters�� row������ ��ȯ
	std::copy(arr.begin(), arr.end(), std::back_inserter(_vInters));
	const int count = GetRowValidcount();

	for (int ii = 1; ii < count; ii++)
		insertRow(ii, FALSE);

	MarkerSetup();
}

void CGridWnd::GetHearder(CArray<_gridHdr, _gridHdr> &arHeader)
{
	arHeader.Copy(m_gridHdrX);
}

//void CGridWnd::GetData(CArray<_intersx *, _intersx *> &arData)
//{
//	const int ncnt = arData.GetSize();
//	const int nTot = m_inters.GetSize();
//	int nMax = 0;
//
//	struct _intersx *pInter1 = nullptr;
//	struct _intersx *pInter2 = nullptr;
//
//	if (ncnt >= nTot)
//		nMax = nTot;
//	else
//		nMax = ncnt;
//
//	for (int ii = 0; ii < nMax; ii++)
//	{
//		pInter1 = arData.GetAt(ii);
//		pInter2 = m_inters.GetAt(ii);
//		memcpy(pInter1, pInter2, sizeof(struct _intersx));
//	}
//}
//
//void CGridWnd::GetData(std::vector<std::shared_ptr<_intersx>&arData, int *nRowCount)
//{
//	const int ncnt = arData.GetSize();
//	const int nTot = m_inters.GetSize();
//	int nMax = 0;
//	char tCode[12]{};
//	int rCnt = 0;
//
//	struct _intersx *pInter1 = nullptr;
//	struct _intersx *pInter2 = nullptr;
//
//	if (ncnt >= nTot)
//		nMax = nTot;
//	else
//		nMax = ncnt;
//
//	for (int ii = 0; ii < nMax; ii++)
//	{
//		pInter1 = arData.GetAt(ii);
//		pInter2 = m_inters.GetAt(ii);
//
//		ZeroMemory(tCode, sizeof(tCode));
//		CopyMemory(tCode, m_inters.GetAt(ii)->code, sizeof(tCode));
//
//		if (CString(tCode).IsEmpty() == false)
//		{
//			rCnt++;
//		}
//
//		memcpy(pInter1, pInter2, sizeof(struct _intersx));
//	}
//
//	*nRowCount = rCnt;
//}

// 2013.01.22 KSJ �Է��ڵ��� m_inters������ ��ġ ã��
int CGridWnd::GetInter(CString code)
{
	auto ft = std::find_if(_vInters.begin(), _vInters.end(), [code](auto& pInter) {
		if (strcmp(pInter->code, (char*)code.operator LPCTSTR()) == 0)
		{
			return true;
		}
		// 2013.08.12 KSJ �����μ��� ���� ���� ���� ��ȸ�� �ǽð����� �����ڵ带 �޾ƿö� J, K, ���� �ڵ尡 �Ⱥپ� ������
		//�����������Ͽ��� �پ��ֱ� ������ ���� �������� ���ڿ��˻��� �ѹ� �� �˻����ش�.
		else if (strstr(pInter->code, (char*)code.operator LPCTSTR()) != nullptr)
		{
			return true;
		}
		return false;
	});

	if (ft != _vInters.end())
	{
		return ft - _vInters.begin();
	}
	return -1;
}

void CGridWnd::toggleAction(bool toggle)
{
	if (!GetKind())
		return;

	m_bExpect = toggle;

	const struct _symbol
	{
		UINT stid1;
		char *symb1;
		UINT stid2;
		char *symb2;
	} chksym[] = {
	    {IDS_GH_CURR, "2023", IDS_GH_ANTIPRC, "2111"},
	    {IDS_GH_DIFF, "2024", IDS_GH_DIFF, "2115"},
	    {IDS_GH_RATE, "2033", IDS_GH_RATE, "2116"},
	    {IDS_GH_VOL, "2027", IDS_GH_ANTIVOL, "2112"},
	};
	const int chksymC = sizeof(chksym) / sizeof(_symbol);

	m_grid->m_bExpect = toggle;

	// 2011.12.27 KSJ ������ �����϶��� �ɺ��ٲ��� �ȵ��� �����ϱ� ����

	CString strCode;

	if (toggle) // �� Row���� �ڽ���, �ڽ����̸� ����� �����ϰ� �ɺ��� �����Ѵ�.
	{
		_gridHdr gridHdr{};

		for (int nRow = 1; nRow < m_grid->GetRowCount(); nRow++)
		{
			strCode.Empty();
			strCode = m_grid->GetItemText(nRow, colCODE);

			if (strCode.GetLength() == 6 || strCode.GetLength() == 8) //�ڽ���, �ڽ���
			{
				for (int ii = 0; ii < chksymC; ii++)
				{
					for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
					{
						gridHdr = m_gridHdrX.GetAt(jj);

						if (atoi(m_grid->GetItemSymbol(nRow, jj)) != atoi(chksym[ii].symb1))
							continue;

						// 2013.07.08 SetItemSymbol�� �ϸ� �ɺ��� �ٲ��� �ʾƼ� �޸�ī�Ƿ� ������.
						// m_grid->SetItemSymbol(nRow, jj, chksym[ii].symb2);

						CopyMemory(gridHdr.symbol, chksym[ii].symb2, 4);

						gridHdr.stid = chksym[ii].stid2;

						m_gridHdrX.SetAt(jj, gridHdr);
						break;
					}
				}
			}
		}
	}
	else // false�϶��� ������ ���簡�� �ٲ��ش�.
	{
		_gridHdr gridHdr{};

		for (int nRow = 1; nRow < m_grid->GetRowCount(); nRow++)
		{
			strCode.Empty();
			strCode = m_grid->GetItemText(nRow, colCODE);

			if (strCode.GetLength() == 6 || strCode.GetLength() == 8) //�ڽ���, �ڽ���
			{
				for (int ii = 0; ii < chksymC; ii++)
				{
					for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
					{
						gridHdr = m_gridHdrX.GetAt(jj);

						// 2013.07.08 chksym[ii].symb2�� ���ؾ���.
						if (atoi(m_grid->GetItemSymbol(nRow, jj)) != atoi(chksym[ii].symb2))
							continue;

						// 2013.07.08 SetItemSymbol�� �ϸ� �ɺ��� �ٲ��� �ʾƼ� �޸�ī�Ƿ� ������.
						// m_grid->SetItemSymbol(nRow, jj, chksym[ii].symb1);

						CopyMemory(gridHdr.symbol, chksym[ii].symb1, 4);
						gridHdr.stid = chksym[ii].stid1;

						m_gridHdrX.SetAt(jj, gridHdr);
						break;
					}
				}
			}
		}
	}

	initialGrid(false);
	sendTransaction();
}

void CGridWnd::toggleAutoAction(bool toggle)
{
	if (!GetKind())
		return;

	m_bAutoExpect = toggle;
	sendTransaction();
}

void CGridWnd::queryNewsCode()
{
	struct mid
	{
		char func[1]; /* '1':�ֱ�100����      */
		char date[8]; /* ��¥                 */
	} mid;

	memset(&mid, ' ', sizeof(mid));
	mid.func[0] = '1';
	CString senddata = CString((char *)&mid, sizeof(mid));
	CSendData sdata;
	char key{};
	_trkey *trkey = (struct _trkey *)&key;
	trkey->kind = TRKEY_NEWS;
	trkey->group = m_nIndex;
	sdata.SetData("PIBO2022", key, (char *)senddata.operator LPCTSTR(), senddata.GetLength(), ""); // 2012.07.17 KSJ pibo --> pibf 2013.08.05 ����
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);
}

void CGridWnd::parsingNewsCode(CRecvData *data)
{
	struct grid
	{
		char cod2[12];
	};

	struct mod
	{
		char nrec[4];
		struct grid grid[1];
	} * mod{};

	char *buf = data->data;
	mod = (struct mod *)buf;
	CString nrec, code;
	int ncnt = 0;
	const int nsize = sizeof(struct grid);
	nrec.Format("%.*s", sizeof(mod->nrec), mod->nrec);
	ncnt = atoi(nrec);

	SetKind(MAKE_TREEID(xISSUE));

	for (int ii = 0; ii < ncnt; ii++)
	{
		code.Format("%.*s", sizeof(mod->grid[ii].cod2), mod->grid[ii].cod2);
		code.TrimRight();
		if (!code.IsEmpty())
		{
			if (code.GetAt(0) == 'A' || code.GetAt(0) == 'Q') // 2016.07.04 KSJ �̽������϶� ETN������ ǥ��
				code = code.Mid(1);
			_vInters.at(ii)->code = code;
		}
	}
	sendTransaction();
}

void CGridWnd::parsingNews(CString datB)
{
	int idx = 0;
	CString code = _T(""), szTitle = _T(""), symbol = _T(""), entry = _T("");

	while (!datB.IsEmpty())
	{
		idx = datB.Find('\t');
		if (idx == -1)
			break;

		symbol = datB.Left(idx++);
		datB = datB.Mid(idx);

		idx = datB.Find('\t');
		if (idx == -1)
		{
			entry = datB;
			datB = _T("");
		}
		else
		{
			entry = datB.Left(idx++);
			datB = datB.Mid(idx);
		}

		if (symbol.Compare("301") == 0)
		{
			code = entry;
			code.TrimRight();
			break;
		}
	}

	if (!code.IsEmpty())
	{
		const int ncnt = m_arExist.GetCount();
		const int total = _vInters.size();
		int nIndex = 0;

		for (int ii = 0; ii < ncnt; ii++)
		{
			nIndex = atoi(m_arExist.GetValue(ii));	
			if (total > nIndex)
			{
				auto& pInter = _vInters.at(nIndex);
				if (strcmp(pInter->code, (char *)code.operator LPCTSTR()) == 0)
				{
					const LPARAM lParam = m_grid->GetItemData(nIndex + 1, colSIG);
					const WORD low = LOWORD(lParam);
					constexpr WORD high = 1;
					m_grid->SetItemData(nIndex + 1, colSIG, MAKELPARAM(low, high)); // KSJ colSIG
					if (m_bWaveApply)
						PlayWave();
				}
			}
		}
	}
}

// 2012.01.19 KSJ Alertx �߰�

void CGridWnd::parsingNewsx(DWORD *data)
{
	class CIndexMap *idx = nullptr;
	CString code = _T(""), szTitle = _T("");
	szTitle = (char *)data[15];
	code = (char *)data[301];

	code.TrimLeft();
	code.TrimRight();

	if (!code.IsEmpty())
	{
		if (code.GetAt(0) == '5')
			code = "Q" + code;
		else
			code = "A" + code; // 2012.09.11 A�� �ٿ��ش�.

		int nIndex = 0;

		if (m_pSearchMap.GetCount() > 0)
		{
			if (!m_pSearchMap.Lookup(code, (CObject *&)idx))
				return;
			// 2012.10.09 KSJ ���ؿ� �ε����� �����ε�����. ������ nIndex +1 �� nIndex �� ������.
			for (int ii = 0; ii < idx->idxCnt; ii++)
			{
				nIndex = idx->index[ii];
				const LPARAM lParam = m_grid->GetItemData(nIndex, colSIG);
				const WORD low = LOWORD(lParam);
				constexpr WORD high = 1;
				m_grid->SetItemData(nIndex, colSIG, MAKELPARAM(low, high)); // KSJ colSIG

				if (m_bWaveApply)
					PlayWave();
			}
		}
	}
}

// 2012.01.19 KSJ Alertx �߰� ��

CString CGridWnd::GetCodeName(CString code)
{
	CString ret = _T("");
	CString sztmp;
	sztmp.Format("%s", code);
	ret.Format("%s", (char *)m_pView->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)sztmp.operator LPCTSTR()));
	ret.TrimLeft(), ret.TrimRight();

	if (ret.GetLength() > 32)
		ret = ret.Mid(0, 32);
	return ret;
}

void CGridWnd::OperAlarm(int kind, char *lParam)
{
	LPARAM data = 0;
	WORD hval = 0;
	const int ncnt = m_arExist.GetCount();
	int nindex = -1;

	if (kind == MO_INIT)
	{
		for (int ii = 0; ii < ncnt; ii++)
		{
			nindex = atoi(m_arExist.GetValue(ii));
			data = m_grid->GetItemData(nindex + 1, colSIG);
			hval = HIWORD(data);
			data = MAKELPARAM(0, hval);
			m_grid->SetItemData(nindex + 1, colSIG, data); // KSJ colSIG
		}
	}
	else
	{
		CString strA, strB;
		strA.Format("%s", lParam);
		if (m_arExist.GetCount())
		{
			strB = m_arExist.GetData(strA);
			strB.TrimRight();
			if (strB.IsEmpty())
				return;
			nindex = atoi(strB);
			data = m_grid->GetItemData(nindex + 1, colSIG);
			hval = HIWORD(data);
			data = MAKELPARAM(1, hval);
			m_grid->SetItemData(nindex + 1, colSIG, data); // KSJ colSIG

			if (nindex >= 0)
			{
				m_grid->m_nAlmRow = nindex + 1;
				m_grid->Invalidate(false);
			}
		}
	}
}

void CGridWnd::ReIndexing()
{
	m_arExist.RemoveAll();
	CString sztmp, szindex;
	for (int ii = 0; ii < MAX_LINE; ii++)
	{
		sztmp = m_grid->GetItemText(ii + 1, 0);
		if (!sztmp.IsEmpty())
		{
			szindex.Format("%d", ii);
			m_arExist.SetData(sztmp, szindex);
		}
	}
}

int CGridWnd::GetIndex(CString symbol)
{
	const int ncnt = m_gridHdrX.GetSize();
	struct _gridHdr gridHdr
	{
	};
	int ret = -1;

	for (int ii = colCURR + 1; ii < ncnt; ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);

		if (symbol.CompareNoCase(gridHdr.symbol) == 0)
		{
			ret = ii;
			break;
		}
	}

	return ret;
}

CString CGridWnd::CalcuPyungaSonik(struct _intersx *pinters, CString curr)
{
	CString ret = _T("");
	const double dval1 = IH::TOfabs(pinters->xnum);
	const double dval2 = IH::TOfabs(pinters->xprc);
	const double dval3 = IH::TOfabs(curr);
	const double maeipPrc = pinters->maeipPrc;
	const double creditPrc = pinters->creditPrc;
	double result = 0;
	double rPrice = 0;

	if (dval1 <= 0 || dval2 <= 0)
		return ret;

	//	�򰡼��� = �򰡱ݾ�(�ſ�ݾ� ����) - ���Աݾ�
	rPrice = (dval3 * dval1) - creditPrc;

	if (maeipPrc != 0)
	{
		result = rPrice - maeipPrc;
	}
	else
	{
		result = rPrice - (dval2 * dval1);
	}

	if (result > 0)
	{
		ret.Format("%+.0f", result);
	}
	else if (result < 0)
	{
		ret.Format("-%.0f", result);
	}
	return ret;
}

CString CGridWnd::CalcuSuik(struct _intersx *pinters, CString curr)
{
	CString ret = _T("");
	const double dval1 = IH::TOfabs(pinters->xnum);
	const double dval2 = IH::TOfabs(pinters->xprc);
	const double dval3 = IH::TOfabs(curr);
	const double creditPrc = pinters->creditPrc;
	const double maeipPrc = pinters->maeipPrc;
	double result = 0;
	const double resultVal = 0;
	double rPrice = 0;

	if (dval1 <= 0 || dval2 <= 0)
		return ret;

	//	�򰡼��� = �򰡱ݾ�(�ſ�ݾ� ����) - ���Աݾ�
	rPrice = (dval3 * dval1) - creditPrc;

	if (maeipPrc != 0)
	{
		result = rPrice - maeipPrc;
	}
	else
	{
		result = rPrice - (dval2 * dval1);
	}

	//	ret.Format("%+.0f", result);

	//	resultVal = atof(ret);

	//  ���ͷ� = �򰡼��� * 100 / ���԰�
	if (maeipPrc != 0)
	{
		ret.Format("%+.2f", ((result * 100.0) / maeipPrc));
	}
	else
	{
		ret.Format("%+.2f", ((result * 100.0) / (dval2 * dval1)));
	}
	return ret;
}

CString CGridWnd::CalFutureEvalPrice(struct _intersx *pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian)
{
	//	if (!m_dBase)
	getBase(m_code);

	const double maeipPrc = pinters->maeipPrc; //���忡�� ���� ���Աݾ�

	double dSonik = 0, dGubn = 1;
	bool bMd = false;
	CString m_pprice, m_pgsonik;
	double m_pmaeip = 0;

	if (m_curr == 0)
	{
		return "";
	}

	const int kind = getJKind(m_code);
	if (m_gubn == "1")
		bMd = true;
	if (bMd)
		dGubn = -1;

	m_pprice.Format("%.f", m_remian * m_curr * m_dBase);

	if (maeipPrc != 0)
	{
		m_pmaeip = maeipPrc;
	}
	else
	{
		m_pmaeip = m_remian * m_maip * m_dBase;
	}

	dSonik = atof(getFormatData(m_pprice)) - m_pmaeip;
	dSonik *= dGubn;

	if (dSonik > 0)
		m_pgsonik.Format("+%.0f", dSonik);
	else if (dSonik < 0)
		m_pgsonik.Format("-%.0f", dSonik);

	return m_pgsonik;
}

CString CGridWnd::CalFutureEvalRate(struct _intersx *pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian)
{
	getBase(m_code);

	const double maeipPrc = pinters->maeipPrc; //���忡�� ���� ���Աݾ�

	double dSonik = 0, dGubn = 1;
	bool bMd = false;
	CString m_pprice, m_pgsonik, m_suik;
	double m_pmaeip = 0;

	if (m_curr == 0)
	{
		return "";
	}

	const int kind = getJKind(m_code);
	if (m_gubn == "1")
		bMd = true;
	if (bMd)
		dGubn = -1;

	m_pprice.Format("%.f", m_remian * m_curr * m_dBase);

	if (maeipPrc != 0)
	{
		m_pmaeip = maeipPrc;
	}
	else
	{
		m_pmaeip = m_remian * m_maip * m_dBase;
	}

	dSonik = atof(getFormatData(m_pprice)) - m_pmaeip;
	dSonik *= dGubn;

	// ���ͷ�
	dSonik *= 100;
	dSonik /= m_pmaeip; //(atof(m_rprice) * m_dBase * atof(m_remian));
	m_suik.Format("%.2f", dSonik);
	if (dSonik < 0)
		m_suik = "" + m_suik;
	else if (dSonik > 0)
		m_suik = "+" + m_suik;

	return m_suik;
}

void CGridWnd::SetGridBkColor(COLORREF color)
{
	color = RGB(255, 255, 255); ////BUFFET FOR FIXED BACKCOLOR

	if (m_grid != nullptr && m_grid->GetSafeHwnd())
	{
		m_grid->SetBkColor(color);

		// ADD PSH 20070911
		m_grid->SetGridColor(GetAxColor(65));
		m_grid->Invalidate(false);
		m_btReload.Invalidate(TRUE);
		m_btDomino.Invalidate(TRUE);
		m_btSAVE.Invalidate(TRUE);
		m_btCLOSE.Invalidate(TRUE);
		// END ADD

		initialGrid(false); // BUFFET
	}
}

void CGridWnd::ReIndexSpecial()
{
	const int ncnt = m_gridHdrX.GetSize();
	UINT needs = 0;
	CString string;
	for (int ii = 0; ii < ncnt; ii++)
	{
		needs = m_gridHdrX.GetAt(ii).needs;
		if (needs == 2)
			continue;

		string.Format("%s", m_gridHdrX.GetAt(ii).symbol);

		if (needs == 9)
		{
			if (string.Compare("###3") == 0)
				m_sonikField = ii;
			if (string.Compare("###4") == 0)
				m_suikField = ii;
		}

		if (needs == 7)
			m_bongField = ii;

		//���ϰŷ������ �÷� index ����
		if (needs == 10)
			m_yDayVolField = ii;

		if (needs == 11)
			m_EqualizerField = ii;

		if (needs == 12)
			m_recomDataField = ii;

		if (needs == 13)
			m_recomBasicCostField = ii;

		if (needs == 14)
			m_recomSuikrateField = ii;

		/* 2011.12.16 KSJ
		if (needs == 15)
			m_recomBMjisuField = ii;

		if (needs == 16)
			m_recomBMSuikrateField = ii;

		if (needs == 17)
			m_recomgoalCostField = ii;
		*/

		if (needs == 18)
			m_recomJisuGubnField = ii;

		if (needs == 19)
			m_upjongCode = ii;

		if (needs == 22)
			m_beforeVolField = ii;

		if (needs == 21)
			m_newCurrField = ii;

		// 2016.04.15 KSJ epb ��õ����
		if (needs == 25)
			m_epbDataField = ii;
	}
}

void CGridWnd::PlayWave(CString strFile /* = "" */)
{
	if (strFile.IsEmpty())
	{
		strFile = m_strWaveFile;
	}

	CFileFind ff;

	if (ff.FindFile(strFile))
		sndPlaySound(strFile, SND_ASYNC);
}

void CGridWnd::SetMarketColor(BOOL bApply, COLORREF clrKospi, COLORREF clrKosdaq)
{
	m_bMKClrApply = bApply;
	m_clrKospi = clrKospi;
	m_clrKosdaq = clrKosdaq;

	m_grid->m_bMKClrApply = m_bMKClrApply;
	m_grid->m_clrKospi = m_clrKospi;
	m_grid->m_clrKosdaq = m_clrKosdaq;
}

// 2011.12.02	KSJ
void CGridWnd::SetNewsNChart(BOOL bNewsNChart)
{
	int nWidth = 0;
	m_bNewsNChart = bNewsNChart ? true : false;

	if (m_bNewsNChart)
		nWidth = m_nNewsNChartWidth;

	m_menuHeader.CheckMenuItem(IDC_MENU_NEWSNCHART, m_bNewsNChart ? MF_CHECKED : MF_UNCHECKED);

	m_grid->SetColumnWidth(colSIG, nWidth);
	m_grid->ResetScrollBars();
	m_grid->Invalidate(false);

	m_btReload.Invalidate(TRUE);
	m_btDomino.Invalidate(TRUE);
	m_btSAVE.Invalidate(TRUE);
	m_btCLOSE.Invalidate(TRUE);
}
// KSJ

// 2011.12.05	KSJ
void CGridWnd::SetMargin(BOOL bMargin)
{
	m_bMargin = bMargin;

	m_menuHeader.CheckMenuItem(IDC_MENU_MARGIN, m_bMargin ? MF_CHECKED : MF_UNCHECKED);

	m_grid->SetMarginRatio(m_bMargin);
	m_grid->Invalidate(false);
}
// KSJ

void CGridWnd::SetDispCode(BOOL bDispCode)
{
	int nWidth = 0;
	m_bDispCode = bDispCode;

	if (m_bDispCode)
		nWidth = m_nCodeWidth;

	m_menuHeader.CheckMenuItem(IDC_MENU_CODE, m_bDispCode ? MF_CHECKED : MF_UNCHECKED);

	m_grid->SetColumnWidth(colCODE, nWidth);
	m_grid->ResetScrollBars();
	m_grid->Invalidate(false);

	m_btReload.Invalidate(TRUE);
	m_btDomino.Invalidate(TRUE);
	m_btSAVE.Invalidate(TRUE);
	m_btCLOSE.Invalidate(TRUE);
}

void CGridWnd::MultiChart()
{
	CString strData(_T(""));
	strData.Format("%s /S/t0/d%s\t", mchartMAP, MCSYMBOL);

	const int nCount = m_grid->GetRowCount();

	for (int i = m_grid->GetFixedRowCount(); i < nCount; i++)
	{
		if (!m_grid->IsSelectedRow(i))
			continue;

		strData += m_grid->GetItemText(i, colCODE);
	}

	openView(typeVIEW, strData);
}

BOOL CGridWnd::IsValidCode(CString strCode)
{
	BOOL bFind = FALSE;
	if (m_grid)
	{
		for (int i = 1; i < m_grid->GetRowCount(); i++)
		{
			CString strGridCode(m_grid->GetItemText(i, 0));

			if (strGridCode.GetLength() == 7)
				strGridCode.Delete(0);

			if (strGridCode == strCode)
			{
				bFind = TRUE;
				break;
			}
		}
	}

	return bFind;
}

void CGridWnd::OnSizing(UINT fwSide, LPRECT pRect)
{
	CBaseWnd::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here

	OperResize(pRect->right - pRect->left, pRect->bottom - pRect->top);
}

int CGridWnd::GetScrollCtrl()
{
	//	return m_grid->GetGridScroll();
	return m_grid->GetHscroll();
}

int CGridWnd::GetGridWidth()
{
	const int childWidth = m_grid->GetGridWidth();

	return childWidth;
}

int CGridWnd::GetWidth()
{
	int width;
	CRect rc;
	GetWindowRect(rc);

	width = rc.Width();

	if (width <= 0)
	{
		width = m_nGridWidth;
	}

	return width;
}

CString CGridWnd::GetRecommandInfo(int row)
{
	return m_grid->GetItemText(row, m_recomJisuGubnField);
}

void CGridWnd::SetInitRecommandInfo()
{
	for (int i = 0; i < IARRAYSIZE; i++)
	{
		m_iYDayVol[i][JISUGUBN] = "";
	}
}

int CGridWnd::ArrangeField(int type, CString save[100][2])
{
	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

	int maxRow = 0;

	if (nOver == MO_SELECT)
	{
		maxRow = 100;
	}
	else
	{
		maxRow = GetRowValidcount();
	}

	maxRow = m_rowC - 1;
	for (int i = 0; i < 100; i++)
	{
		save[i][0] = "";
		save[i][1] = "";
	}

	//�ϴ� �׸����� ����� ������ ������ ��ƿ´�
	for (int i = 0; i < maxRow; i++)
	{
		save[i][0] = m_grid->GetItemText(i + 1, colCODE);

		if (type == 0) //��ϼ�
		{
		}
		else if (type == 1) //������
		{
			save[i][1] = m_grid->GetItemText(i + 1, m_upjongCode);
		}
		else if (type == 2) //������
		{
			save[i][1] = m_grid->GetItemText(i + 1, colNAME);
		}
		else //�����ڵ��
		{
			save[i][1] = m_grid->GetItemText(i + 1, colCODE);
		}
	}

	return maxRow;
}

void CGridWnd::SendWhenVisibleMode(CString data[100][2])
{
	m_pGridArray.RemoveAll();

	char tempB[64]{};

	//////////////////////////////////////
	struct _gridHdr gridHdr
	{
	};
	for (int ii = 0; ii < m_gridHdrX.GetSize(); ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);

		sprintf(tempB, "%s%c", gridHdr.symbol, P_NEW);
		m_pGridArray.Add(tempB);
	}

	if (m_pGridArray.GetSize() > GRIDMAXNUM)
	{
		const int nRem = m_pGridArray.GetSize() - GRIDMAXNUM;

		m_bContinue = TRUE;

		SendWhenVisibleModeTR(&data[0], 0, GRIDMAXNUM);
	}
	else
	{
		m_bContinue = FALSE;

		SendWhenVisibleModeTR(&data[0], 0, m_pGridArray.GetSize());
	}
}

void CGridWnd::SendWhenVisibleModeTR(CString data[100][2], int nStart, int nEnd)
{
	int sendL = 0;
	char tempB[64]{};
	const int bufSize = sizeof(struct _gridHi) + 50 + m_gridHdrX.GetSize() * 5 + _vInters.size() * 12 + 12;
	std::string sendB;
	sendB.resize(bufSize);
	FillMemory(sendB.data(), bufSize, ' ');

	const BOOL bExpect = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_EXPECT);
	m_bAutoExpect = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_AUTOEXPECT);

	if (bExpect)
	{
		sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 1, P_TAB);
	}
	else
	{
		if (m_bAutoExpect)
		{
			sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 1, P_TAB);
		}
		else
		{
			sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 0, P_TAB);
		}
	}

	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	// 2011.12.08 KSJ 2013.08.28 KSJ ���� �Ϲݽü��� ���͸��Ǽ� �������� ������ ���� ���� �ʿ� ����.
	// 	sprintf(tempB, "%s%c%d%c", gFILTERING, P_DELI, 1, P_TAB);
	// 	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	// 	sendL += strlen(tempB);
	//

	sprintf(tempB, "%s%c", gSYMBOL, P_DELI);
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	struct _gridHi *gridHi{};

	gridHi = (struct _gridHi *)&sendB[sendL];
	sendL += sizeof(struct _gridHi);

	CopyMemory(gridHi->visible, "99", sizeof(gridHi->visible));
	sprintf(tempB, "%04d", _vInters.size());
	CopyMemory(gridHi->rows, tempB, sizeof(gridHi->rows));

	gridHi->type = '0';
	gridHi->dir = '1';
	gridHi->sort = '0';

	// struct	_gridHdr gridHdr;
	for (int ii = nStart; ii < nEnd; ii++)
	{
		//		gridHdr = m_gridHdrX.GetAt(ii);

		// 		sprintf(tempB, "%s%c", gridHdr.symbol, P_NEW);
		// 		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		// 		sendL += strlen(tempB);

		strcpy(tempB, (LPTSTR)(LPCTSTR)m_pGridArray.GetAt(ii));
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}

	sprintf(tempB, "2321%c", P_NEW); //���ϰŷ���
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);
	//
	// 	sprintf(tempB, "2022%c", P_NEW);					//���� �߰��� �ɺ��� ���簡
	// 	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	// 	sendL += strlen(tempB);

	if (((CGroupWnd *)m_pGroupWnd)->getRecommandCursel(m_nIndex) >= 0)
	{
		// 		sprintf(tempB, "1915%c", P_NEW);					//���� ����
		// 		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		// 		sendL += strlen(tempB);
	}
	else
	{
		// 		sprintf(tempB, "1910%c", P_NEW);					//��õ����
		// 		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		// 		sendL += strlen(tempB);
		//
		// 		sprintf(tempB, "1915%c", P_NEW);					//���� ����
		// 		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		// 		sendL += strlen(tempB);
	}

	int nInterCnt = 0;

	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

	if (nOver == MO_VISIBLE)
	{
		nInterCnt = GetRowValidcount();
	}
	else
	{
		nInterCnt = 100;
	}

	CString code;
	int ncnt = 0;
	CString tempStr;
	int pos = -1;

	for (int ii = 0; ii < nInterCnt; ii++)
	{
		code = data[ii][0];

		if (strlen(code))
		{
			tempStr = code;

			pos = tempStr.Find(" ");
			if (pos != -1)
			{
				//				strcpy(temp, tempStr.Mid(0, pos));
				code = tempStr.Mid(0, pos);
			}

			ncnt++;
		}

		if (strlen(code) > 0)
		{
			sprintf(tempB, "%s%c", code.GetString(), P_DELI);
			CopyMemory(&sendB[sendL], tempB, strlen(tempB));
			sendL += strlen(tempB);
		}
	}

	sendB[sendL] = P_NEW;
	sendL += 1;
	sendB[sendL] = P_TAB;
	sendL += 1;
	sendB[sendL] = 0x00;
	CSendData sdata;

	char key{};
	_trkey *trkey = (struct _trkey *)&key;

	trkey->group = m_nIndex;
	trkey->kind = TRKEY_GRIDNEW;

	m_updateROW = -1;

	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);
	m_endsort = false;

}

void CGridWnd::SetEditWork(bool tp)
{
	m_bEditWork = tp;
}

void CGridWnd::SetGridinfo(bool bWidthSave)
{
	const int ncnt = m_gridHdrX.GetSize();
	CString sztmp = _T("");
	int nWidth{};
	m_szWIDTH = _T("");
	struct _gridHdr grdHdr
	{
	};
	for (int ii = 0; ii < ncnt; ii++)
	{
		grdHdr = m_gridHdrX.GetAt(ii);
		nWidth = m_grid->GetColumnWidth(ii);
		CString strSymbol = (CString)grdHdr.symbol;

		//��õ�����ʵ�� �������� �ʴ´�
		if (strSymbol.Find("191") > -1)
			continue;

		if (strSymbol.Find("2022") > -1)
			continue;

		if (ii >= colNAME)
		//		if (ii >= colCODE)
		{
			sztmp.Format("%d,", nWidth);
			m_szWIDTH += sztmp;

			if (ii == colCODE && nWidth != 0)
				m_nCodeWidth = nWidth;

			if (ii == colSIG && nWidth != 0)
				m_nNewsNChartWidth = nWidth;
		}
	}

	// 2013.04.16 KSJ ó���� width�� �����Ѵ�.
	// 	if(bWidthSave)
	// 	{
	// 		WritePrivateProfileString(m_section, KEY_WIDTH, m_szWIDTH.operator LPCTSTR(), m_fileCFG);
	//
	// 	}
	// 	else
	savecfg();
}

void CGridWnd::uploadOK()
{
	// 2017.02.21 ������ 'G'���� 'V' ������
	if (!m_bLastSend)
	{
		m_bLastSend = TRUE;
		return;
	}

	int sendL = 0;
	std::string sendB;
	sendB.resize(1024 * 16);
	char tempB[32]{};
	//	char	strUinfo[500];

	struct _uinfo *uinfo = (struct _uinfo *)&sendB[sendL];
	sendL += sz_uinfo;

	FillMemory((char *)uinfo, sz_uinfo, ' ');

	CopyMemory(uinfo->gubn, "MY", sizeof(uinfo->gubn));
	uinfo->dirt[0] = 'U';
	uinfo->cont[0] = 'V';

	sprintf(tempB, "00000");
	CopyMemory(uinfo->nblc, tempB, sizeof(uinfo->nblc));

	uinfo->retc[0] = 'O';
	sendB[sendL] = 0x00;

	//	SendTR
	CSendData sdata;
	char key{};
	_trkey *trkey = (struct _trkey *)&key;

	trkey->group = m_nIndex;
	trkey->kind = TRKEY_LASTSET;

	sdata.SetData(trUPDOWN, key, sendB.data(), sendB.size(), "");

	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);
}

bool CGridWnd::CheckField(CString symbol)
{
	CString arr;
	for (int ii = 0; ii < m_arrSymbol.GetSize(); ii++)
	{
		arr = m_arrSymbol.GetAt(ii);
		if (symbol.Find(arr) >= 0)
			return true;
	}
	return false;
}

void CGridWnd::initRSymbol()
{
	_mRsymbol.clear();
	_mRsymbol.emplace(std::make_pair(0, 0));
	_mRsymbol.emplace(std::make_pair(950, 950));
	_mRsymbol.emplace(std::make_pair(951, 951));
	_mRsymbol.emplace(std::make_pair(32, 32));
	_mRsymbol.emplace(std::make_pair(31, 31));
	_mRsymbol.emplace(std::make_pair(30, 30));
	_mRsymbol.emplace(std::make_pair(29, 29));
	_mRsymbol.emplace(std::make_pair(27, 27));
	_mRsymbol.emplace(std::make_pair(23, 23));
	_mRsymbol.emplace(std::make_pair(24, 24));
	_mRsymbol.emplace(std::make_pair(111, 111));
	_mRsymbol.emplace(std::make_pair(112, 112));
	_mRsymbol.emplace(std::make_pair(115, 115));
	_mRsymbol.emplace(std::make_pair(116, 116));
	_mRsymbol.emplace(std::make_pair(33, 33));
	_mRsymbol.emplace(std::make_pair(34, 34));
	_mRsymbol.emplace(std::make_pair(40, 40));
	_mRsymbol.emplace(std::make_pair(734, 734));
	_mRsymbol.emplace(std::make_pair(740, 740));
	_mRsymbol.emplace(std::make_pair(15, 15));
	_mRsymbol.emplace(std::make_pair(301, 301));

	const int countX = m_gridHdrX.GetSize();
	for (int jj = col7852; jj < countX; jj++)
	{
		const _gridHdr gridHdr = m_gridHdrX.GetAt(jj);
		CString sSymbol = CString(gridHdr.symbol, strlen(gridHdr.symbol));

		//���μ������� �ǽð��� �ƴϴ�.
		if (sSymbol.CompareNoCase("2204") == 0 || sSymbol.IsEmpty())
			continue;

		sSymbol = sSymbol.Right(3);
		const int symbol = atoi(sSymbol);
		if (symbol > 0)
			_mRsymbol.emplace(std::make_pair(symbol, symbol));
	}
}
