// MainFrmWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrmWnd.h"

#include "Winsock2.h"


#include "./AddControls/DrawPattenCtrl.h"
#include "../../DrCommon/Dr_Control/DrXLSFile.h"
#include <FLOAT.H>

//#include "../../../SrcSite/include/axisvar.h"
#include "../../../SrcSite/include/axis_msg.h"

#include "./PatternChart/DlgChart_Stock.h"

#include "FieldEditDlg.h"
#include "DlgSavePattern.h"
#include "ScreenWnd.h"

#include "../../Chart/Include_Chart/Dll_Load/IMetaTable.h"

#define	ITTIMER_SELECTROW0	10


CString nCtrlName[] =
{ 
	"btnSearch",
	"btnPrint",
	"btnItemEdit",
	"btnItemSend",
	"btnSave",
	"btnInit"
};

int nCtrlID[] =
{ 
		IDC_BTN_SEARCH,
		IDC_BTN_PRINT,
		IDC_BTN_FIELDEDIT,
		IDC_BTN_RESULTCTRL,
		IDC_BTN_SAVE,
		IDC_BTN_ERASER
};

#define MIN_SLIDER 20
#define MAX_SLIDER 90

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;
extern void SetPermanentHandle(HANDLE h, CObject* permOb);
/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd

//extern CString		KSLink_GetFileHeader();

IMPLEMENT_DYNCREATE(CMainFrmWnd, CFrameWnd)

//const		LPCTSTR		LIST_TARGET_1ST[] = { "��ü", "�ڽ���", "�ڽ���", "�׸�", "��������"};
const		LPCTSTR		LIST_TARGET_1ST[] = { "��ü", "�ŷ���", "�ڽ���",  "��������"};

#define THUMB_WIDTH			55
#define THUMB_HEIGHT		52
#define THUMB_USER_WIDTH	80
#define THUMB_USER_HEIGHT	52

const	LPCTSTR	LIST_TIME[] = { " 1", " 3", " 5", "10", "15", "30", "60" };
const	UINT	LIST_TIME_INDEX[] = { 3, 4, 5, 6, 7, 8, 9 };

const	UINT	LIST_BONGTERM[] = { 100, 120, 150, 170, 200, 250, 300 };
const	UINT	LIST_SIMILAR[] = { 90, 80, 70, 60, 50 };


/*extern void SetPermanentHandle(HANDLE h, CObject* permOb);*/
//////////////////////////////////////////////////////////////////////////

CMainFrmWnd::CMainFrmWnd()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
	
	m_brushBK.CreateSolidBrush(COLOR_MAIN_CANVAS);
	
	m_pctrlDrawPatten = NULL;
	
	m_nTrIndex = 0;
	m_strTargetBaseType = "";
	
	m_nMarket = 0;
	m_bLink = TRUE;

	m_ptabCondition = NULL;
	
	m_clrMainSkin = RGB(247,248,251);

	m_bExpand = FALSE;

	m_nPrevScale = 40;
	m_strCaption = "�߼����� �׸���";
	
	//���絵 �ʵ� ���� ���� >>	
	m_stAlike.stHeadType.lHeadID = (long)9025;
	m_stAlike.nColNum = 0;
	m_stAlike.stHeadType.strHeadName = _T("���絵");
	m_stAlike.stHeadType.nUnitNum = 100;
	m_stAlike.stHeadType.strUnitText = _T("%");
	m_stAlike.stHeadType.strAlignment = _T("right");
	m_stAlike.stHeadType.nColWidth = 7;
	m_stAlike.stHeadType.strDataType = _T("double");
	m_stAlike.stHeadType.nDataLength = 20;
	m_stAlike.bView = TRUE;
	m_bSetControlBase = FALSE;
	// <<

	m_pDlgChart = NULL;

	m_nPeriodCnt = 4;

	m_nPreReceiveLen = -1;
}

CMainFrmWnd::~CMainFrmWnd()
{
	m_brushBK.DeleteObject();
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();
}


BEGIN_MESSAGE_MAP(CMainFrmWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrmWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONUP()
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP

	ON_EN_CHANGE(IDC_EDIT_CTRL, OnEdChangeScaleCnt)

	ON_CBN_SELCHANGE(IDC_COMBO_TARGET_1ST, OnChgComboTarget1st)
	ON_CBN_SELCHANGE(IDC_COMBO_TARGET_2ND, OnChgComboTarget2nd)

	ON_BN_CLICKED(IDC_BTN_DAY, OnBtnDay)
	ON_BN_CLICKED(IDC_BTN_WEEK, OnBtnWeek)
	ON_BN_CLICKED(IDC_BTN_MONTH,OnBtnMon)
	ON_BN_CLICKED(IDC_BTN_MIN, OnBtnMin)

	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_BTN_LOAD, OnBtnLoad)
	ON_BN_CLICKED(IDC_BTN_ERASER, OnBtnReset)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)

	ON_BN_CLICKED(IDC_CHK_BONG, OnBtnChkBong)
	ON_CBN_SELCHANGE(IDC_COMBO_BONG, OnChgBongCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_SIM, OnChgSimCombo)

	ON_MESSAGE(WM_NOTIFY_GRID, OnNotify_Grid)
	ON_MESSAGE(WM_NOTIFY_DRAWPATTEN, OnNotify_DrawPatten)
	ON_NOTIFY(NM_CLICK, IDC_ITEM_LIST, OnClickListItem)
	ON_NOTIFY(NM_CLICK, IDC_ITEM_USERLIST, OnClickUserListItem)

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SEARCH, OnChangeTab)

	ON_BN_CLICKED(IDC_BTN_PRINT, OnBtnPrint)
	ON_BN_CLICKED(IDC_BTN_FIELDEDIT, OnBtnFieldEdit)
	ON_BN_CLICKED(IDC_BTN_RESULTCTRL, OnBtnResultCtrl)
	ON_BN_CLICKED(IDC_BTN_RESULTEXPAND, OnBtnResultExpand)
	ON_BN_CLICKED(IDC_BTN_RESULTEXPAND_DOWN, OnBtnResultExpand)

	ON_COMMAND_RANGE(ID_MENU_RESULTCTRL_START, ID_MENU_RESULTCTRL_END, OnMenuResultCtrl)

	ON_REGISTERED_MESSAGE(RMSG_DELETE_ITEM, OnDeleteUserItem)
	ON_REGISTERED_MESSAGE(RMSG_GET_TIPTEXT, OnGetTipText)
	ON_REGISTERED_MESSAGE(RMSG_RECEIVETRANDATA, OnReceiveTranData)
	ON_REGISTERED_MESSAGE(RMSG_RECEIVEDIRECTFID, OnReceiveDirectFID)

	ON_REGISTERED_MESSAGE(RMSG_USERINFO, OnUserInfo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd message handlers

int CMainFrmWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	GetDir_DrfnRoot(m_strAppPath);
	
	g_iMetaTable.m_strDataFolderPath.Format(_T("%s\\icss\\"), m_strAppPath);
	g_iMetaTable.LoadMetaTable();

 	CreatePatternDrawCtrl();
 	MakeUserPatternList();

	CreateImageList();
 	MakeUserPatternImageList();

	CString strPath;
	GetDir_DrfnData(strPath);
	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	CreateButton();
	CreateStatic();
	CreateCombo();
	CreateMisc();

	AfxSetResourceHandle(hInstSave);
	
	m_pITrCommManager = (IAUTrCommManager*)AfxGetPctrInterface(UUID_IAUTrCommManager);
	if(m_pITrCommManager)
		m_pITrComm = (ITrComm2*)m_pITrCommManager->AdviseTrComm();

	m_WaitCursor.SetParentWnd(this);
	//tour2k
	Request_PortGroup();

	return 0;
}

void CMainFrmWnd::CreatePatternDrawCtrl()
{
	if (m_pctrlDrawPatten == NULL)
		m_pctrlDrawPatten = new CDrawPattenCtrl;
	m_pctrlDrawPatten->Create(this, IDC_DRAWPATTEN_CTRL);
}

void CMainFrmWnd::CreateImageList()
{
	CBitmap bitmap;
	CString strImgPath, strData;
	strImgPath = m_strAppPath + (CString)FOLDER_IMAGE + "\\W58_H20.bmp";

	//////////////////////////////////////////////////////////////////////////
	strImgPath = m_strAppPath + (CString)FOLDER_IMAGE + "\\W20_H20.bmp";

	if ( !m_ImageListBtn.m_hImageList)
	{
		if ( !m_ImageListBtn.Create( 20, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			SetMessageBox("Can not Find W20_H20.bmp file");		
		}

		bitmap.Attach ((HBITMAP)::LoadImage(NULL, strImgPath, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImageListBtn.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	CStringArray saPatternFileData;
	GetLoad_PatternItem(saPatternFileData);
	m_saPatternItemFile.RemoveAll();

	//��ȣ �̹��� ����Ʈ ===========================================================================
	int nPos_Find;
	CString strPatternDataFile;
	CString strPath_Img;
	strPath_Img = m_strAppPath + (CString)FOLDER_PATTERNIMAGE + "\\";
	
	if ( !m_ImageListItem.m_hImageList)
	{
		if ( !m_ImageListItem.Create( THUMB_WIDTH, THUMB_HEIGHT, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			SetMessageBox("Can not Find bmp file" );			
		}		
		int nCnt = min(saPatternFileData.GetSize(), CNT_DRAW_VAL);
		for (int nIndex = 0; nIndex < nCnt; nIndex++)
		{
			strData.Format("DrawItem %d", nIndex);			
			strData = saPatternFileData.GetAt(nIndex);
			nPos_Find = strData.Find("/");
			strPatternDataFile = strData.Left(nPos_Find);
			m_saPatternItemFile.Add(strPatternDataFile);
			
			strData = strData.Right(strData.GetLength() - (nPos_Find + 1));
			BOOL bOK = bitmap.Attach((HBITMAP)::LoadImage(NULL, strPath_Img + strData, IMAGE_BITMAP, 0, 0,
													LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
			m_ImageListItem.Add(&bitmap, RGB(255,0,0));

			bitmap.DeleteObject();
		}
	}

	if ( !m_ImageBK.m_hImageList)
	{
		if ( !m_ImageBK.Create( THUMB_USER_WIDTH, THUMB_USER_HEIGHT, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			SetMessageBox("Can not Find bmp file" );			
		}	
		
		BOOL bOK = bitmap.Attach((HBITMAP)::LoadImage(NULL, strPath_Img + "UserPattern.bmp", IMAGE_BITMAP, 0, 0,
												LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
		m_ImageBK.Add(&bitmap, RGB(255,0,0));

		bitmap.DeleteObject();
	}
}

void CMainFrmWnd::CreateButton()
{
	m_btnExpand.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_BTN_RESULTEXPAND);
	SetBtnImg(&m_btnExpand, "UP_EX");

	//////////////////////////////////////////////////////////////////////////
	m_btnReverseChk.Create(_T("�����׷�����"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_REVERSECHK);
	m_btnReverseChk.SetFont(&m_fontDefault);
	
	m_btnExceptionChk.Create(_T("�켱������"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_EXCEPTIONCHK);
	m_btnExceptionChk.SetFont(&m_fontDefault);
	//////////////////////////////////////////////////////////////////////////
	m_btnBongChk.Create(_T("���Ű˻�"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_CHK_BONG);
	m_btnBongChk.SetFont(&m_fontDefault);
	
	m_btnDay.Create(_T("��"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		CRect(0, 0, 0, 0), this, IDC_BTN_DAY);
	m_btnDay.SetFont(&m_fontDefault);
	
	m_btnWeek.Create(_T("��"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		CRect(0, 0, 0, 0), this, IDC_BTN_WEEK);
	m_btnWeek.SetFont(&m_fontDefault);
	
	m_btnMonth.Create(_T("��"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		CRect(0, 0, 0, 0), this, IDC_BTN_MONTH);
	m_btnMonth.SetFont(&m_fontDefault);

	m_btnMin.Create(_T("��"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		CRect(0, 0, 0, 0), this, IDC_BTN_MIN);
	m_btnMin.SetFont(&m_fontDefault);
	
	m_btnDay.FP_vSetImage(&m_ImageListBtn, /*m_clrMainSkin,*/ 81, 82, TRUE, "��", TRUE);
	m_btnWeek.FP_vSetImage(&m_ImageListBtn, /*m_clrMainSkin,*/ 81, 82, TRUE, "��", TRUE);
	m_btnMonth.FP_vSetImage(&m_ImageListBtn, /*m_clrMainSkin,*/ 81, 82, TRUE, "��", TRUE);
	m_btnMin.FP_vSetImage(&m_ImageListBtn, /*m_clrMainSkin,*/ 81, 82, TRUE, "��", TRUE);
	//////////////////////////////////////////////////////////////////////////
	m_btnLoad.Create("�ҷ�����", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		CRect(0, 0, 0, 0), this, IDC_BTN_LOAD);
	m_btnLoad.SetBtnColor(2);
	m_btnLoad.SetFont(&m_fontDefault);
	
	m_btnSave.Create(_T("����"), CRect(0, 0, 0, 0), this, IDC_BTN_SAVE);
	SetBtnImg(&m_btnSave, 2);
	
	m_btnReset.Create(_T("�ʱ�ȭ"), CRect(0, 0, 0, 0), this, IDC_BTN_ERASER);
	SetBtnImg(&m_btnReset, 4);
	
	m_btnSearch.Create(_T("��  ��"), CRect(0, 0, 0, 0), this, IDC_BTN_SEARCH);
	SetBtnImg(&m_btnSearch, 4, TRUE);

	m_btnPrint.Create(_T("�μ�"), CRect(0, 0, 0, 0), this, IDC_BTN_PRINT);
	SetBtnImg(&m_btnPrint, 2);

	m_btnFieldEdit.Create(_T("�׸�����"), CRect(0, 0, 0, 0), this, IDC_BTN_FIELDEDIT);
	SetBtnImg(&m_btnFieldEdit, 4);
    
	m_btnResultCtrl.Create(_T("��������"), CRect(0, 0, 0, 0), this, IDC_BTN_RESULTCTRL);
	SetBtnImg(&m_btnResultCtrl, 4);
}

void CMainFrmWnd::CreateStatic()
{
	m_stcStatusBar.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
	m_stcStatusBar.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	m_ctrlPatternName.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlPatternName.SetFont(&m_fontDefault);
	
	m_ctrlJongName.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlJongName.SetFont(&m_fontDefault);

	m_ctrlSimilar.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlSimilar.SetFont(&m_fontDefault);

	m_ctrlTarget.Create(_T("���"), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlTarget.SetFont(&m_fontDefault);

	m_ctrlPeriod.Create(_T("���ϱⰣ"), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlPeriod.SetFont(&m_fontDefault);

	m_ctrlSimilarity.Create(_T("���絵"), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlSimilarity.SetFont(&m_fontDefault);

	m_ctrlState.Create(_T("�������"), WS_CHILD | WS_VISIBLE | SS_RIGHT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlState.SetFont(&m_fontDefault);

	m_ctrlTime.Create(_T("�˻��ð� :     "), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlTime.SetFont(&m_fontDefault);

	m_ctrlNotice.Create(_T("�� �� ������ �����Ǵ��� ��������̸� ���� �Ǵ��� ���� å���� �� ���񽺸� ����Ͻô� �̿��ڿ��� �ֽ��ϴ�."), 
		WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, CRect(0, 0, 0, 0), this);
	m_ctrlNotice.SetFont(&m_fontDefault);

	m_ctrlCodeCntText.Create(_T("�˻���� "), WS_CHILD | WS_VISIBLE | SS_LEFT |SS_CENTERIMAGE , 
		CRect(0, 0, 0, 0), this);
	m_ctrlCodeCntText.SetFont(&m_fontDefault);
	m_ctrlCodeCntText.ShowWindow(SW_HIDE);

	m_ctrlCodeCnt.Create(_T("    "), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlCodeCnt.SetFont(&m_fontBold);
}

void CMainFrmWnd::CreateCombo()
{
	m_comboTarget_1st.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_TARGET_1ST);
	m_comboTarget_1st.SetFont(&m_fontDefault);
	int nIndex;
	for (nIndex = 0; nIndex < _countof(LIST_TARGET_1ST); nIndex++)
		m_comboTarget_1st.AddString(LIST_TARGET_1ST[nIndex]);
	
	m_comboTarget_2nd.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_TARGET_2ND);
	m_comboTarget_2nd.SetFont(&m_fontDefault);
	m_comboTarget_2nd.EnableWindow(FALSE);
	//////////////////////////////////////////////////////////////////////////
	m_comboTime.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_TIME);
	m_comboTime.SetFont(&m_fontDefault);
	
	CString srtTerm;
	for (nIndex = 0; nIndex < _countof(LIST_TIME); nIndex++)
	{
		srtTerm = (CString)LIST_TIME[nIndex] + "��";
		m_comboTime.AddString(srtTerm);
	}
	m_comboBongTerm.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_BONG);
	m_comboBongTerm.SetFont(&m_fontDefault);
	
	CString strData;
	for (nIndex = 0; nIndex < _countof(LIST_BONGTERM); nIndex++)
	{
		strData.Format("%d�� �̳�", (int)LIST_BONGTERM[nIndex]);
		m_comboBongTerm.AddString(strData);
	}
	m_comboBongTerm.SetCurSel(0);
	
	
	m_comboSimilar.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_SIM);
	m_comboSimilar.SetFont(&m_fontDefault);
	for (nIndex = 0; nIndex < _countof(LIST_SIMILAR); nIndex++)
	{
		strData.Format("%d%% �̻�", (int)LIST_SIMILAR[nIndex]);
		m_comboSimilar.AddString(strData);
	}
	m_comboSimilar.SetCurSel(0);
}

void CMainFrmWnd::CreateMisc()
{
	// Edit Control
	m_editPeriod.CreateEx(WS_EX_CLIENTEDGE, "edit", "40", WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, IDC_EDIT_CTRL);
	m_editPeriod.SetFont(&m_fontDefault);

	// image list	
	m_ctrlListItem.Create(CRect(0, 0, 100, 100), this, WS_CHILD|WS_VISIBLE|WS_TABSTOP, IDC_ITEM_LIST);
	m_ctrlListItem.SetDefaultFont(&m_fontDefault);
	m_ctrlListItem.CreateTooltip();
	m_ctrlListItem.SetMargin(4, 9);
	m_ctrlListItem.SetImageSpace(2, 6);
	m_ctrlListItem.SetImageData(m_ImageListItem, 3, THUMB_WIDTH, THUMB_HEIGHT);

	m_ctrlUserListItem.Create(CRect(0, 0, 100, 100), this, WS_CHILD|WS_TABSTOP, IDC_ITEM_USERLIST);
	m_ctrlUserListItem.SetDefaultFont(&m_fontDefault);
	m_ctrlUserListItem.CreateTooltip();
	m_ctrlUserListItem.SetMargin(4, 9);
	m_ctrlUserListItem.SetImageSpace(6, 6);
	m_ctrlUserListItem.SetUseDeleteMenu(TRUE);
	m_ctrlUserListItem.SetImageData(m_ImageUserListItem, 2, THUMB_USER_WIDTH, THUMB_USER_HEIGHT);

	// tab
	m_ptabCondition = new CDrTabCtrl();
	m_ptabCondition->Create(0, this, IDC_TAB_SEARCH);
	m_ptabCondition->SetBKColor(COLOR_MAIN_CANVAS);
	m_ptabCondition->SetCurSel(0);
		
	TC_ITEM item;
	item.mask = TCIF_TEXT | TCIF_IMAGE;
	item.pszText = "�⺻";
	m_ptabCondition->InsertItem(0, &item);

	item.pszText = "�����";
	m_ptabCondition->InsertItem(1, &item);
  
	
	// slider
// 	m_ctrlSliderVal.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_RIGHT , 
// 		CRect(0, 0, 0, 0), this);
// 	m_ctrlSliderVal.SetFont(&m_fontDefault);
	
	//Progress Create 
	m_ctrlSlider.Create(WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS ,
		CRect(0, 0, 0, 0), this, IDC_PROGRESS);
	m_ctrlSlider.SetRange(MIN_SLIDER, MAX_SLIDER);
	m_ctrlSlider.SetTicFreq(10);
	m_ctrlSlider.SetPageSize(10);
	
	// grid
	m_gridResult.CreateGrid(WS_CHILD | WS_VISIBLE| WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_GRID_CTRL);

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	m_pDlgChart		  =	new CDlgChart_Stock;
	m_pDlgChart->Create(IDD_DLG_CHART, this );
	m_pDlgChart->ModifyStyle(0, WS_CLIPCHILDREN);
	m_pDlgChart->m_ChartItem.SetMultiItemMode(TRUE);
	m_pDlgChart->ShowWindow(SW_SHOW);

	AfxSetResourceHandle(hInstSave);
}

void CMainFrmWnd::SetInit()
{
	m_strScreenNo = GetFormCode();
	
	m_gridResult.SetInit("RESULT");
	m_gridResult.ClearAll();

	GetLoadSet_Def();
	
	SetLoad_FieldInfo();
	SetGrid_Head(FALSE);
	SetGrid_Head(TRUE);
	m_gridResult.SetMultiSelectMode(TRUE);

	_DoGridWidthSet(0);		//0000207
	
	if (m_pctrlDrawPatten != NULL)
	{
		m_pctrlDrawPatten->SetInit();
		m_pctrlDrawPatten->SetData_XGridCnt(m_nDefult_Slider);
		m_pctrlDrawPatten->Invalidate();
	}

	//////////////////////////////////////////////////////////////////////////
	// �ʱⰪ ����
	if (m_nDefult_TargetBaseType == 0)	// ��ü
	{
		m_strTargetBaseType = "0";
	}
	else if (m_nDefult_TargetBaseType == 1) // �ڽ���-��ü
	{
		m_strTargetBaseType = "10";
	}
	else if (m_nDefult_TargetBaseType == 2) // �ڽ���-��ü
	{
		m_strTargetBaseType = "20";
	}
// 	else if (m_nDefult_TargetBaseType == 3) // �׸�
// 	{
// 		m_strTargetBaseType = "30";
// 	}
	else if (m_nDefult_TargetBaseType == 3) // ����
	{
		m_strTargetBaseType = "40";
	}
	m_comboTarget_1st.SetCurSel(m_nDefult_TargetBaseType);

	m_btnReverseChk.EnableWindow(FALSE);
	
	m_btnExceptionChk.SetCheck(m_nDefult_ExceptPreferred);
	m_btnBongChk.SetCheck(m_nDefult_BongTerm_User);
	
	OnBtnChkBong();
	
	m_strChartTerm.Format("%d", m_nDefult_ChartTerm);
	m_comboTime.EnableWindow(FALSE);
	m_comboTime.SetCurSel(m_nDefult_ChartTermMin);
	m_comboTime.Invalidate();
	
	if (m_nDefult_ChartTerm == 0)		//# ƽ
	{
	}
	else if (m_nDefult_ChartTerm == 1)  //# ��
	{
		m_btnMin.SetCheck(TRUE);
		m_comboTime.EnableWindow(TRUE);
		m_comboTime.Invalidate();
	}
	else if (m_nDefult_ChartTerm == 2) //# ��
		m_btnDay.SetCheck(TRUE);
	else if (m_nDefult_ChartTerm == 3)	//# ��
		m_btnWeek.SetCheck(TRUE);
	else if (m_nDefult_ChartTerm == 4)	//# ��
		m_btnMonth.SetCheck(TRUE);
	else if (m_nDefult_ChartTerm == 5)	//# ��
	{
	}
	
	m_ctrlSlider.SetPos(m_nDefult_Slider);
	m_nPrevScale = m_nDefult_Slider;

	CString strData;
	strData.Format("%d", m_nDefult_Slider);
	m_editPeriod.SetWindowText(strData);
	
	m_comboBongTerm.SetCurSel(m_nDefult_BongTerm);
	m_comboSimilar.SetCurSel(m_nDefult_Similar);
	
	//////////////////////////////////////////////////////////////////////////
	CString strPath;
	GetDir_DrfnRoot(strPath);
	

// 	m_Chart.SetInit(this->GetSafeHwnd(), (long)m_pBaseDesk, strPath, FALSE);	
 	m_pDlgChart->m_ChartItem.LoadAddIn("ChartPeriodSummaryAddIn.dll:DRAG_ZOOM_ANAL");

	//m_clrMainSkin = (COLORREF)GetParent()->SendMessage(RMSG_GET_SKINCOLOR, 0, 0);

	m_brushBK.DeleteObject();
	m_brushBK.CreateSolidBrush(m_clrMainSkin);

//20100210 SJ_KIM
	SetTrData_MSTCode();
	SetTrData_ThemeGroup();

	CString strType, strUserFileName;
	if(LookUpSearchCenterModify(strType, strUserFileName))
	{
		if(strType == "0")
			SearchUserItem(strUserFileName);
	}
}

void CMainFrmWnd::SetTrData_MSTCode()
{
	int nType = 2;

	char* lpData = new char[sizeof(int) + 1];
	::memset(lpData, 0x00, sizeof(int) + 1);
	memcpy(lpData, &nType, sizeof(int) + 1);

//	m_WaitCursor.BWaitCursorIntelli();
	int nState = Module_SendTr((CString)TRNO_MSTCODE, sizeof(int), (BYTE*)lpData);
//	SetStateTitleBar("[��ȸ��]");
		
	if (lpData)
		delete[] lpData;
}

void CMainFrmWnd::OnDestroy() 
{
	_DoGridWidthSet(1);		//0000207

	m_saCodeTheme.RemoveAll();
	m_saCodeMST.RemoveAll();
	m_saCodeGroup.RemoveAll();

	if (m_ptabCondition != NULL)
	{
		delete m_ptabCondition;
		m_ptabCondition = NULL;
	}

	if(m_pctrlDrawPatten)
	{
		if(m_pctrlDrawPatten->GetSafeHwnd())
			m_pctrlDrawPatten->SendMessage(WM_DESTROY);
		SafeDelete(m_pctrlDrawPatten);
	}

	if(m_pITrCommManager)
	{
		if(m_pITrComm)
		{
			m_pITrCommManager->UnAdviseTrComm(m_pITrComm);
			m_pITrComm = NULL;
		}
	}
	
	if(m_pDlgChart)			// ��Ʈ Dlg
	{
		if(m_pDlgChart->m_hWnd && IsWindow(m_pDlgChart->m_hWnd))
			m_pDlgChart->DestroyWindow();
		delete m_pDlgChart;
		m_pDlgChart = NULL;
	}

	m_ImageListItem.DeleteImageList();
	m_ImageUserListItem.DeleteImageList();
	m_ImageListBtn.DeleteImageList();
	m_ImageBK.DeleteImageList();

	m_ctrlListItem.DestroyWindow();
	m_ctrlUserListItem.DestroyWindow();

	CFrameWnd::OnDestroy();
}

LRESULT CMainFrmWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
	case MF_PFCONTORL_MOVE:	
		{	
			CWnd *pWnd = GetParent();
			if(!pWnd) return FALSE;
			GetParent()->GetParent()->SendMessage(RMSG_KSMOVECTRL, wParam, lParam);
			return TRUE;
		}
	case MF_PFCONTORL_EVENT:
		{
			CWnd *pWnd = GetParent();
			if(!pWnd) return FALSE;
			GetParent()->GetParent()->SendMessage(RMSG_PF_CONTROL_EVENT, wParam, lParam);
			return TRUE;
		}
	}
	return CFrameWnd::WindowProc(message, wParam, lParam);

}

void CMainFrmWnd::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
		
	MoveControls();
	OnPF_Contorl_Move();
}

void CMainFrmWnd::MoveControls()
{
	CRect reClient, reTop, reBottom, reStatusBar;
	GetClientRect(reClient);
	
	reClient.top += 6;
	reClient.bottom -= 6;
	reClient.left += 6;
	reClient.right -= 6;
	
	reTop = reClient;
	reBottom = reClient;
	//# Menu
	reTop.bottom = reTop.top + 20;
	OnSize_Menu(reTop);
	
	reBottom.top = reBottom.bottom - 20;
	reStatusBar = reBottom;

	reBottom.left += 2;
	reBottom.right = reBottom.left + 650;
	//m_ctrlTime.MoveWindow(&reBottom);
	m_ctrlNotice.MoveWindow(&reBottom);

	reBottom.right = reClient.right - 5;
	reBottom.left = reBottom.right - 170;
	m_ctrlState.MoveWindow(&reBottom);

	reStatusBar.top -= 2;
	reStatusBar.bottom += 3;
	reStatusBar.left -= 3;
	reStatusBar.right += 3;
	m_stcStatusBar.MoveWindow(&reStatusBar);

	int nWidth_Left = 197;
	reTop.top = reTop.bottom + 7;
	reTop.bottom = reBottom.top - 8;
	
	reTop.right = reTop.left + nWidth_Left;
	OnSize_Left(reTop);
	
	reTop.left = reTop.right + 5;
	reTop.right = reClient.right;
	reTop.top += 22;
	OnSize_Right(reTop);
}

void CMainFrmWnd::OnSize_Menu(CRect reClient)
{
	CRect reLeft, reRight, reLine;
	
	reLeft = reClient;
	reLeft.left += 197 + 5;
//	reLeft.right = reLeft.left + 55;
	reLeft.right = reLeft.left + 27;
	m_ctrlTarget.MoveWindow(&reLeft);
	
	reLeft.left = reLeft.right + 1;
	reLeft.right = reLeft.left + 75;
	reLeft.bottom += 80;
	m_comboTarget_1st.MoveWindow(&reLeft);
	
	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 150;
	reLeft.bottom += 100;
	m_comboTarget_2nd.MoveWindow(&reLeft);
	
	reLeft.left = reLeft.right + 10;
	reLeft.right = reLeft.left + 80;
	reLeft.bottom -= 180;
	m_btnExceptionChk.MoveWindow(&reLeft); // �켱�� ����
	
	reLeft.left = reLeft.right + 5;
	reLeft.right = reLeft.left + 90;
	m_btnReverseChk.MoveWindow(&reLeft); // �����׷�����

	//////////////////////////////////////////////////////////////////////////
	// �ι�° ��
	reLeft = reClient;
	reLeft.OffsetRect(0, 24);
	reLeft.left += 197 + 5;

	reLeft.top -= 1;
	reLeft.bottom -= 1;

	reLeft.right = reLeft.left + 20;
	m_btnDay.MoveWindow(&reLeft);;  // ��

	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 20;
	m_btnWeek.MoveWindow(&reLeft);;  // ��

	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 20;
	m_btnMonth.MoveWindow(&reLeft);;  // ��

	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 20;
	m_btnMin.MoveWindow(&reLeft);  // ��

	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 49;
	reLeft.bottom += 100;
	m_comboTime.MoveWindow(&reLeft);
	reLeft.bottom -= 100;

	reLeft.left = reLeft.right + 5;
	reLeft.right = reLeft.left + 50;
	m_ctrlPeriod.MoveWindow(&reLeft);  // "�˻��Ⱓ"

	reLeft.left = reLeft.right + 5;
	reLeft.right = reLeft.left + 20;
	m_editPeriod.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 1;
	reLeft.right = reLeft.left + 70;
	m_ctrlSlider.MoveWindow(&reLeft);
	//////////////////////////////////////////////////////////////////////////
 	reRight = reClient;
 	reRight.OffsetRect(0, 24);

	reRight.top -= 1;
	reRight.bottom += 1;
	reRight.right += 3;

	reRight.left = reRight.right - 90;
	reRight.bottom += 100;
	m_comboBongTerm.MoveWindow(&reRight);
	reRight.bottom -= 100;

	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 68;
	m_btnBongChk.MoveWindow(&reRight);

	reRight.right = reRight.left - 7;
	reRight.left = reRight.right - 80;
	reRight.bottom += 100;
	m_comboSimilar.MoveWindow(&reRight);
	reRight.bottom -= 100;

	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 40;
	m_ctrlSimilarity.MoveWindow(&reRight);  // "���絵"
}

void CMainFrmWnd::OnSize_Left(CRect reClient)
{
	CRect reTop, reBtn;
	
	reTop = reClient;

	reTop.right = 200;
	reTop.top -= 25;
	reTop.bottom++;

	m_rtTab = reTop;
	m_rtTab.top += 18;
	m_rtTab.bottom += 1;
	m_rtTab.left -= 5;
	m_rtTab.right += 1;

	CRect rtTemp;
	rtTemp = reTop;
	rtTemp.top -= 4;
	rtTemp.left -= 3;
	rtTemp.bottom = reTop.bottom + 3;

	if(m_ptabCondition)
		m_ptabCondition->MoveWindow(&rtTemp);
	CRect rcTab;
	m_ptabCondition->GetItemRect(0, &rcTab);

	reTop.top += (rcTab.bottom-4);
	reTop.left -= 2;
	reTop.DeflateRect(0,2,1,0);
	m_ctrlListItem.MoveWindow(&reTop);
	if (m_ptabCondition->GetCurSel() == 0)
		m_ctrlListItem.BringWindowToTop();

	m_ctrlUserListItem.MoveWindow(&reTop);
	if (m_ptabCondition->GetCurSel() == 1)
		m_ctrlUserListItem.BringWindowToTop();
}

void CMainFrmWnd::OnSize_Right(CRect reClient)
{
	CRect reTop, reBottom, reLine, reRight;
	
	int nBasePos;
	nBasePos = (reClient.bottom - reClient.top) / 8;

	//////////////////////////////////////////////////////////////////////////
	// ���� ����� ��Ʈ��
	reBottom = reClient;
	reBottom.bottom = reBottom.top + nBasePos*4 + 15;
	reBottom.right = reBottom.left + reClient.Width()/2 - 6;
	reBottom.bottom++;

	if (m_pctrlDrawPatten != NULL)
	{
		reTop = reBottom;
		reTop.top -= 1;
		reTop.bottom = reTop.top + 28;

		m_reTopChart = m_reTopPattern = reTop;
		m_reTopChart.left = reBottom.right + 2;
		m_reTopChart.right = reClient.right + 3;

		reTop.DeflateRect(0, 4);
		reTop.right -= 3;
		reTop.left = reTop.right - 36;
		m_btnSave.MoveWindow(&reTop);
		
		reTop.right = reTop.left - 2;
		reTop.left = reTop.right - 60;
		m_btnReset.MoveWindow(&reTop);

		reTop = m_reTopPattern;
		reTop.DeflateRect(0, 4);
		reTop.right -= 135;
		reTop.left += 2;
		m_ctrlPatternName.MoveWindow(&reTop);	// ���ϰ˻� �̸�

		reBottom.top += 30;
		reBottom.bottom -= 1;
		reBottom.right++;
		m_pctrlDrawPatten->MoveWindow(reBottom);
	}

	reTop = m_reTopChart;
	reTop.top += 2;
	reTop.bottom -= 2;
	reTop.left += 10;
	reTop.right = reTop.left + 100;
	m_ctrlJongName.MoveWindow(&reTop);	// �����

	reTop.left = reTop.right + 130;
	reTop.right = reTop.left + 100;
	m_ctrlSimilar.MoveWindow(&reTop);	// ���絵

	// íƮ
	reBottom.left = reBottom.right + 2;
	reBottom.right = reClient.right + 3; 
	reBottom.bottom--;
	m_pDlgChart->MoveWindow(&reBottom);

	//////////////////////////////////////////////////////////////////////////
	reBottom = reClient;

	if(!m_bExpand)
	{
		reBottom.top = reBottom.top + nBasePos*4 + 17;
	}
	else
		reBottom.top -= 24;

	reBottom.bottom = reBottom.top + 28;
	
	m_rcMiddle = reBottom;
	m_rcMiddle.right += 3;

	reBottom.DeflateRect(3, 4);

	reLine = reBottom; 
	reRight = reBottom;

	reLine.left += 1;
	reLine.right = reLine.left + 60;
	m_btnSearch.MoveWindow(&reLine); // �˻���ư

	if(m_bSetControlBase)
	{
		m_btnSearch.ShowWindow(SW_HIDE);
	}

	reLine.OffsetRect(0, 4);
	reLine.bottom = reLine.top + 16;
	reLine.left = reLine.right + 10;
	reLine.right = reLine.left + 55;
	m_ctrlCodeCntText.MoveWindow(&reLine);

	reLine.left = reLine.right + 5;
	reLine.right = reLine.left + 60;
	m_ctrlCodeCnt.MoveWindow(&reLine);

	reRight.right += 2;
	reRight.left = reRight.right - 20;
	if(m_btnExpand.m_hWnd)
	{	
	    m_btnExpand.MoveWindow(reRight);
	}

	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 60;
	m_btnResultCtrl.MoveWindow(reRight);

	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 60;
	m_btnFieldEdit.MoveWindow(reRight);

	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 36;
	m_btnPrint.MoveWindow(reRight);	

	reBottom = reClient;
	reBottom.right += 3;
	reBottom.bottom += 3;

	if(m_bExpand)
	{
		reBottom.top += 7;
	}
	else
	{
		reBottom.top = reBottom.top + nBasePos*4 + 48;
	}
	m_gridResult.MoveWindow(&reBottom);	
}


BOOL CMainFrmWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CMainFrmWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

 	CRect rcClient, rcText;
 	GetClientRect(&rcClient);

	dc.FillSolidRect(rcClient, m_clrMainSkin);
	// 2012.2.20 �ڼ���: IBK �� ������������ �׵θ� ���� >>
// 	dc.FillSolidRect(m_rtTab, GetAxColor(66));
// 	
// 	HPEN hOldPen;
// 	hOldPen = (HPEN)dc.SelectObject(GetAxPen(GetAxColor(13), 1, PS_SOLID));
// 	
// 	MoveToEx(dc.m_hDC, m_rtTab.right, m_rtTab.top + 3, NULL);		// ������ �׵θ�
// 	LineTo(dc.m_hDC, m_rtTab.right,  m_rtTab.bottom);
// 	
// 	MoveToEx(dc.m_hDC, m_rtTab.right, m_rtTab.bottom, NULL);	// �Ʒ� �׵θ�
// 	LineTo(dc.m_hDC, m_rtTab.left - 2, m_rtTab.bottom);
// 	
// 	MoveToEx(dc.m_hDC, m_rtTab.left, m_rtTab.bottom, NULL);			// ���� �׵θ�
// 	LineTo(dc.m_hDC, m_rtTab.left, m_rtTab.top + 2);
// 	dc.SelectObject(hOldPen);
	// 2012.2.20 �ڼ���: IBK �� ������������ �׵θ� ���� <<

	CPen *pOldPen, penGray(PS_SOLID, 1, RGB(166,166,166));
	pOldPen = dc.SelectObject(&penGray);

	CBrush*pBrush = dc.SelectObject(&m_brushBK);

// 	CRgn rgn;
// 	GetRgn_ExeptButton(rcClient, &rgn);	//ReDraw�� ��Ʈ�� �������� ������ ��Ʈ���� �������ϱ�
// 	dc.FillRgn(&rgn, pBrush);	
	
	if(!m_bExpand)
	{
		dc.Rectangle(&m_reTopPattern);
		dc.Rectangle(&m_reTopChart);

//		CFont* pFont = dc.SelectObject(&m_fontBold);
//		
//		rcText = m_reTopPattern;
//		rcText.DeflateRect(5, 2);
//		dc.DrawText(m_strCaption, rcText, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
		m_ctrlPatternName.SetWindowText(m_strCaption);
		
		if(!m_strItemName.IsEmpty())
		{
			m_ctrlJongName.SetWindowText(m_strItemName);
			m_ctrlSimilar.SetWindowText(m_strItemRight);
// 			rcText = m_reTopChart;
// 			rcText.DeflateRect(5, 2);
// 			dc.DrawText(m_strItemName, rcText, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
// 			dc.DrawText(m_strItemRight, rcText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
		}

//		dc.SelectObject(pFont);
	}
	
	dc.Rectangle(&m_rcMiddle);


	dc.SelectObject(pBrush);
	dc.SelectObject(pOldPen);
}

//TR������~
void CMainFrmWnd::SendTr_ChartData(CString strCode, CString strDate)
{
	KB_p0602_InRec1 stOutput;
	
	CTime timer;
	timer = CTime::GetCurrentTime();
	
	CString strToday;
	strToday.Format("%04d%02d%02d", timer.GetYear(), timer.GetMonth(), timer.GetDay());

	memset(&stOutput, 0x20, sizeof(stOutput));
	
	memset(stOutput.shcode, 0x20, sizeof(stOutput.shcode));
	memcpy(stOutput.shcode, strCode, strCode.GetLength());

	m_strCode = strCode;
	
	CString strCount;
	CString strDate_Start, strDate_End;
	strDate_Start = strDate.Left(8);
	strDate_End = strDate.Mid(14, 8);
	memcpy(stOutput.date, strDate_End.GetBuffer(0), strDate_End.GetLength());						// �˻���
	strDate_End.ReleaseBuffer();
	
	memcpy(stOutput.num,		    "0300",				 4);		// �˻��� ������ ����s
	
	if (atoi(m_strChartTerm) == 1)
 	{
 		CString strTerm;
 		strTerm.Format("%03d", atoi(LIST_TIME[m_comboTime.GetCurSel()]));
 		memcpy(stOutput.unit, strTerm.GetBuffer(0),	strTerm.GetLength());
 		strTerm.ReleaseBuffer();
	}
	else
		memcpy(stOutput.unit,	    "001",			 3);		// ƽ/��/�� ��ȸ���ݴ���
	
	memcpy(stOutput.button,		m_strChartTerm,		 1);	
	memcpy(stOutput.modGb,		"1",			 	 1);		// �����ְ� ���� 1:�����ְ��ݿ�
	memcpy(stOutput.nflag,		"0",				 1);
	memset(stOutput.nkey,		0x20,	sizeof(stOutput.nkey));
	memcpy(stOutput.nDataGubun,	"0",				 1);		// '0': ����, '1':����
	memcpy(stOutput.ImgRgb,		"1",				 1);
	
	memset(stOutput.Itime	 ,  0x00			,   sizeof(stOutput.Itime));
	memcpy(stOutput.Irecordgb, 	"0"	, 	1);
	memcpy(stOutput.nDayBefore, "00"	,	2);
	memcpy(stOutput.Itime	 , 	"000000"	,	6);	
	memcpy(stOutput.nowData	 ,  "0"	,	1);
	
	CString strKB0602Info;
	char* szKB0602Info = strKB0602Info.GetBuffer(sizeof(stOutput));
	memcpy(szKB0602Info, (char*)&stOutput, sizeof(stOutput));
	strKB0602Info.ReleaseBuffer();
	CString strKB0602InRec1;
	strKB0602InRec1 = strKB0602Info;
	
	CString szTrNo(QUERY_STRJONGMOK);
	
	m_pITrComm->SetBaseData("PtManager", m_szPTManager);	//For Solomon
	m_pITrComm->SetTR(szTrNo);
	m_pITrComm->Send2Server((LPVOID)(LPCTSTR)strKB0602InRec1, strKB0602InRec1.GetLength(), TRKEY_STRJONGMOK);
}

HRESULT CMainFrmWnd::StatusChange(int nState)
{
	return S_OK;
}

HRESULT CMainFrmWnd::ReceiveData(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen , int nKey )
{
	g_DRDebuger.DRLog_ResponseData(szTR, (LPBYTE)aTRData, dwTRDateLen);
	HRESULT dwResult = 0;
	
	char *pBuff;
	CString strData;
	CString strTRCode(szTR);
	
	if (dwTRDateLen == 0)		//# 0 : err
	{
	//	SetStateTitleBar("[��ȸ ��� ����]");
	//	SetStateTitleBar("��ȸ �Ϸ�");
		
		m_WaitCursor.EWaitCursor();

		SetSearchResultState(0, -1);

//		m_ctrlState.SetWindowText("[��ȸ ��� ����]");
		return S_OK;
	}
	else if (dwTRDateLen > 0)
	{
		if(!strTRCode.CompareNoCase(QUERY_STRJONGMOK_MIN) && m_strChartTerm.CompareNoCase("1") == 0)
			szTR = (LPCSTR)QUERY_STRJONGMOK;	//�е����� ��ȸ�� TR��ȣ ����	

//		BeginWaitCursor();
		SetStateTitleBar("[��ȸ �Ϸ�]");

		if(!strTRCode.CompareNoCase(TRNO_MSTCODE)) // �ֽ� ������ �ڵ� ����
		{
			// ���� ������ �������� ũ�Ⱑ ���� ���� ������ �Ǵ��ϰ�, ����ȸ.
			if (dwTRDateLen < MINLEN_MSTCODE_TR)
			{
				CString strFile, strDataDir;
				strDataDir.Format("%s%s\\%s", Variant(homeCC), FOLDER_FIND, gszCacheDir);
				strFile.Format("%s\\%s", strDataDir, "RequestToday.ini");
				::WritePrivateProfileString(gszDataKey_MstCode, "Today", "", strFile);
				strFile.Format("%s%s", strDataDir, "\\MstCode.dat");
				::DeleteFile(strFile);

				if (m_nPreReceiveLen == dwTRDateLen)
					AfxMessageBox(_T("\"MstCode.dat\" ������ ���������� ������ �� �����ϴ�.\n ȭ���� �ٽ� �����Ͽ� �ֽʽÿ�."));
				else
				{
					m_nPreReceiveLen = dwTRDateLen;

					SetTrData_MSTCode();
				}
				return S_OK;
			}

			m_WaitCursor.EWaitCursor();

			m_saCodeMST.RemoveAll();

			MSTCode stMSTCode;
		//	int nDataCnt = dwTRDateLen / sizeof(MSTCode);
			char szCount[5] = {0,};
			memcpy(szCount, aTRData, 4);
			int nDataCnt = atoi(szCount);	
			int nSizeOfCount = 4;

			CString strAddData;
			for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
			{
				memset(&stMSTCode, 0x00, sizeof(MSTCode));
				memcpy(&stMSTCode, (char*)aTRData + sizeof(MSTCode) * nIndex + 4, sizeof(MSTCode));


				strAddData = "";
				STR_CONVERT(pBuff, stMSTCode.sMarket, sizeof(stMSTCode.sMarket), strData);
				strAddData = strData;

				STR_CONVERT(pBuff, stMSTCode.sCode, sizeof(stMSTCode.sCode), strData);
				strData.TrimLeft(" ");
				strData.TrimRight(" ");

				strAddData += strData;
				m_saCodeMST.Add(strAddData);
			}
		}
		else if(!strTRCode.CompareNoCase(TRNO_THEMEGROUP)) // �׸��׷� ����
		{
// 			m_saCodeTheme.RemoveAll();
// 
// 			if(SZ_p0628OutBlock1 == dwTRDateLen)
// 			{
// 				p0628OutBlock1* pBlock = (p0628OutBlock1*)aTRData;
// 
// 				CString strCount;
// 				strCount.Format("%4.4s", pBlock->ThemeCnt);
// 
// 				m_nThemeGCount = atoi(strCount);
// 			}
// 			else
// 			{
// 				p0628OutBlock2* pBlock = (p0628OutBlock2*)aTRData;
// 
// 				CString strCode, strName;
// 				for(int i = 0; i < m_nThemeGCount; ++i)
// 				{
// 					strCode.Format("%4.4s", pBlock[i].gsThemeCode);
// 					strName.Format("%-36.36s",	pBlock[i].gsThemeName);
// 					strName.TrimRight();
// 					m_saCodeTheme.Add(strCode + strName);						
// 				}
// 			}			

		}
		else if(!strTRCode.CompareNoCase(TRNO_THEMEITEM)) // �׸� ���� ����
		{
// 			if(SZ_p0630OutBlock1 == dwTRDateLen)
// 			{
// 				p0630OutBlock1* pBlock = (p0630OutBlock1*)aTRData;
// 
// 				CString strCount;
// 				strCount.Format("%4.4s", pBlock->giCnt);
// 
// 				m_nThemeICount = atoi(strCount);
// 			}
// 			else
// 			{
// 				p0630OutBlock2* pBlock2 = (p0630OutBlock2*)aTRData;
// 
// 				CString strOut, strTemp;
// 				for(int i = 0; i < m_nThemeICount; ++i)
// 				{
// 					strTemp.Format("%6.6s%1.1s", pBlock2[i].gsItemCode, pBlock2[i].gsMarketFlag);
// 					strOut += strTemp;
// 				}
// 
// 				SetConvert_ReceiveThemeItem(strOut, m_nThemeICount);
// 			}			
		}
		else if(!strTRCode.CompareNoCase(TRNO_DRAW_RESULT))
		{
			SetEnable(TRUE);
			SetReceiveData_Result(LPTSTR(LPCTSTR(aTRData)), dwTRDateLen);

			m_WaitCursor.EWaitCursor();

			SetResultTemp();
		}
		else if(!strTRCode.CompareNoCase(QUERY_STRJONGMOK))
		{
			m_pDlgChart->m_ChartItem.ClearChartData(0);

			char *pszBuff=NULL;
			pszBuff = new char[dwTRDateLen+1];
			memset(pszBuff, 0x00, sizeof(pszBuff));
			memcpy(pszBuff, aTRData, dwTRDateLen);

			KB_p0602_OutRec1_EX *pStOutput = (KB_p0602_OutRec1_EX *)pszBuff;
			long lPosForChart = dwTRDateLen - (&pStOutput->fcnt[0] - &pStOutput->name[0]);
			
			if (m_pDlgChart && m_pDlgChart->m_ChartItem)
			{
				m_strItemName = CString(pStOutput->name, sizeof(pStOutput->name));
				m_strItemName.TrimLeft();	m_strItemName.TrimRight();
				m_strItemName.Replace("&", "&&");
				
				GetSimilarityFromGrid();

				m_pDlgChart->m_ChartItem.RemoveRealItem();
				m_pDlgChart->m_ChartItem.SetItemCode("DEFAULT", m_strCode, m_strItemName);
				m_pDlgChart->m_ChartItem.AddPacketAndGraph();
				m_pDlgChart->m_ChartItem.SetPacketData(m_pDlgChart->m_ChartItem.GetPacketName(),(long)pStOutput->fcnt, lPosForChart,TRUE);
				m_pDlgChart->m_ChartItem.SetDrdsAdvise(FALSE);
				
// 				m_pDlgChart->m_ChartItem.AddRealItem(strCode);		// �ǽð������ �����ڵ�
// 				m_pDlgChart->m_ChartItem.SetDrdsAdvise(TRUE);
				
				CString strDateTime;
				strDateTime = GetDateTime();
				BOOL bLoad = m_pDlgChart->m_ChartItem.InvokeAddInStr("ChartPeriodSummaryAddIn.dll:DRAG_ZOOM_ANAL", "0", strDateTime);

				int nData;
				char*pBuff;
				INT_CONVERT(pBuff, pStOutput->fcnt, sizeof(pStOutput->fcnt), nData);
				
				int nCount = atoi(m_strChartCnt)-60;
				m_pDlgChart->m_ChartItem.SetOnePageDataCount(min(nCount, nData));

				if(m_btnBongChk.GetCheck() && m_btnMin.GetCheck()) // ���̰� ������ȸ�� ����
				{
					CString strEnd = strDateTime.Right(14);
					m_pDlgChart->m_ChartItem.SetStartEndDateOfView(atof(strDateTime.Mid(4,8)), atof(strEnd.Mid(4,8)));
				}
				m_gridResult.SetFocus();
			}

			delete []pszBuff;
			pszBuff = NULL;
			
			m_WaitCursor.EWaitCursor();
			Invalidate();
		}
		else if(!strTRCode.CompareNoCase("13600")) // ���� ���� �ڵ� ���
		{
// 			CHARVECTOR2* pData = (CHARVECTOR2*)aTRData;
// 
// 			CString strCount, strOut;
// 			strCount.Format("%d", dwTRDateLen);
// 
// 			m_saCodeGroup.RemoveAll();
// 
// 			//������
// 			for(int i=0; i < dwTRDateLen; i++)
// 			{
// 				//strData = (*pData)[i][0]; //����
// 				strData = (*pData)[i][1]; // �ڵ�
// 				m_saCodeGroup.Add(strData); // �����
// 				//strData = (*pData)[i][2];
// 			}
// 
// 			m_WaitCursor.EWaitCursor();
		}
		else if ((CString)szTR == TRNO_FAVORITE) //tour2k �������� �׷�
		{
			char* pReceiveBuff = new char[dwTRDateLen + 1];
			memset(pReceiveBuff, 0x00, dwTRDateLen + 1);
			memcpy(pReceiveBuff, aTRData, dwTRDateLen);

			if (nKey == TRKEY_GWANSIMGROUP)//��������
			{
				CStringArray arrName, arrKey;
				ParseGroup(pReceiveBuff, arrKey, arrName);
			}
			delete[] pReceiveBuff; pReceiveBuff = nullptr;
		}

	}
	return S_OK;
}

int CMainFrmWnd::ParseGroup(char* pReceiveBuff, CStringArray& arrKey, CStringArray& arrName)
{
	int cnt = atoi(CString(pReceiveBuff, 4));
	int parseL = 4;

	struct _grSearch
	{
		char ngrs[2];
		char grseq[2];
		char gname[30];
	};

	struct _grSearch* stgr;
	char* pSeq, * pgame;
	pSeq = new char[sizeof(stgr->grseq) + 1];
	pgame = new char[sizeof(stgr->gname) + 1];

	m_GwanSimGroup.strKey.RemoveAll();
	m_GwanSimGroup.strGroupName.RemoveAll();

	for (int ii = 0; ii < cnt; ii++)
	{
		stgr = (struct _grSearch*)&pReceiveBuff[parseL];
		memset(pSeq, 0x00, sizeof(stgr->grseq) + 1);
		memcpy(pSeq, stgr->grseq, sizeof(stgr->grseq));
		arrKey.Add(pSeq);
		m_GwanSimGroup.strKey.Add(pSeq);

		memset(pgame, 0x00, sizeof(stgr->gname) + 1);
		memcpy(pgame, stgr->gname, sizeof(stgr->gname));

		arrName.Add(pgame);
		m_GwanSimGroup.strGroupName.Add(pgame);

		parseL += sizeof(struct _grSearch);

	}

	delete[] pSeq; pSeq = nullptr;
	delete[] pgame; pgame = nullptr;

	return 0;
}

CString CMainFrmWnd::GetDateTime()
{
	CString strResult = "";
	strResult = m_strDateTime_Select.Left(14);
	strResult += ":";
	strResult += m_strDateTime_Select.Right(14);
	
	return strResult;
}

// ���ŵ� �����˻� ��� ó��
BOOL CMainFrmWnd::SetReceiveData_Result(LPTSTR pData, DWORD dwDataLen)
{
	//20100211 SJ_KIM
	///////////////////////////////////////////////////////
	p0624OutBlock1 st0624Out;
	memset(&st0624Out, 0x00, SZ_p0624OutBlock1);
	memcpy(&st0624Out, pData, SZ_p0624OutBlock1);

	char* pBuff;
	DRAW_OUT_HEAD	stDRAW_OUT_HEAD;
	memset(&stDRAW_OUT_HEAD, 0x00, sizeof(DRAW_OUT_HEAD));

	LONG_CONVERT(pBuff, st0624Out.nTrIndex,			 4, stDRAW_OUT_HEAD.nTrIndex);
	LONG_CONVERT(pBuff, st0624Out.lTotalJongCnt,      4, stDRAW_OUT_HEAD.lTotalCnt);
	LONG_CONVERT(pBuff, st0624Out.lOutFieldCnt,	 	 4, stDRAW_OUT_HEAD.lOutFieldCnt);
	LONG_CONVERT(pBuff, st0624Out.lOutListPacketSize, 4, stDRAW_OUT_HEAD.lOutListPacketSize);
	LONG_CONVERT(pBuff, st0624Out.lFindTime,			 6, stDRAW_OUT_HEAD.lFindTime);

	m_gridResult.ClearAll();
	m_gridResult.SetNumberRows(stDRAW_OUT_HEAD.lTotalCnt);

	m_pDlgChart->m_ChartItem.InvokeAddInStr("ChartPeriodSummaryAddIn.dll:DRAG_ZOOM_ANAL", "0", "");
	m_pDlgChart->m_ChartItem.ClearChartData(0);

	if (stDRAW_OUT_HEAD.lTotalCnt == 0)
	{
		m_pDlgChart->m_ChartItem.ClearChartData(0);
		SetSearchResultState(0, -1);
		SetStateTitleBar("[��ȸ ��� ����.]");
		m_strItemName.Empty();
		m_strItemRight.Empty();
		Invalidate();
		return FALSE;
	}

	pData = &pData[SZ_p0624OutBlock1];	
	
	DRAW_OUT_LIST stDRAW_OUT_LIST;
	int nSize = sizeof(DRAW_OUT_LIST);
	int nIndex;

	CString strData;
	int nRow = 0, nTotalCount = stDRAW_OUT_HEAD.lTotalCnt;
	p0624OutBlock2 st0624List;
	
	for (nIndex = 0; nIndex < stDRAW_OUT_HEAD.lTotalCnt; nIndex++)
	{
		memset(&st0624List, 0x00, SZ_p0624OutBlock2);
		memcpy(&st0624List, pData, SZ_p0624OutBlock2);

		memset(&stDRAW_OUT_LIST, 0x00, SZ_DRAW_OUT_LIST);
		Safe_CheckCopy2(stDRAW_OUT_LIST.sJmcode,    st0624List.sJmcode, sizeof(stDRAW_OUT_LIST.sJmcode), strData);
		Safe_CheckCopy2(stDRAW_OUT_LIST.sHname,		st0624List.sHname, sizeof(stDRAW_OUT_LIST.sHname), strData);
		Safe_CheckCopy2(stDRAW_OUT_LIST.sSign,		st0624List.sSign, sizeof(stDRAW_OUT_LIST.sSign), strData);
		Safe_CheckCopy2(stDRAW_OUT_LIST.sMarket,	st0624List.sMarket, sizeof(stDRAW_OUT_LIST.sMarket), strData);
		LONG_CONVERT(pBuff, st0624List.iTarget,			 4, stDRAW_OUT_LIST.iTarget	);
		Safe_CheckCopy2(stDRAW_OUT_LIST.sDateTime_Start, st0624List.sDateTime_Start, sizeof(stDRAW_OUT_LIST.sDateTime_Start), strData);
		Safe_CheckCopy2(stDRAW_OUT_LIST.sDateTime_End,   st0624List.sDateTime_End, sizeof(stDRAW_OUT_LIST.sDateTime_End), strData);

		CString strFieldData="";
		for(int nFieldIndex = 0; nFieldIndex < stDRAW_OUT_HEAD.lOutFieldCnt; nFieldIndex++)
		{		
			STR_CONVERT(pBuff, &st0624List.dFieldData[nFieldIndex*16], 16, strFieldData);
			stDRAW_OUT_LIST.dFieldData[nFieldIndex] = atof(strFieldData);
		}

		for(int i = 0; i < 5; i++) // ��, ���� ��, ��, ��, ��
		{
			char szTemp[16];
			memset(szTemp, 0x00, sizeof(szTemp));
			memcpy(szTemp, &st0624List.dFieldData[(55 + i) * 16], 16);
			m_arCandleData[i] = atoi(szTemp);
		}
		
		//# �켱�� ����
		if (m_btnExceptionChk.GetCheck() == TRUE)
		{
			STR_CONVERT(pBuff, stDRAW_OUT_LIST.sJmcode, sizeof(stDRAW_OUT_LIST.sJmcode), strData);
			strData.TrimLeft(" ");
			strData.TrimRight(" ");

                        //�켱�����ܽ� üũ �ȵǴ� ���� ����
			//if (atoi(strData.Right(1)) > 0)
                        if (strData.Right(1) != "0")
			{
				pData = &pData[SZ_p0624OutBlock2];
				nTotalCount--;
				continue;
			}
		}
		
		m_gridResult.SetAddRow(nRow, stDRAW_OUT_HEAD.lOutFieldCnt, stDRAW_OUT_LIST, m_arCandleData);
		pData = &pData[SZ_p0624OutBlock2];
		nRow++;
	}
	m_gridResult.SetNumberRows(nRow);
	CUGHScroll* pGridHScroll = m_gridResult.m_CUGHScroll;
	pGridHScroll->SetScrollPos(0, FALSE);
	m_gridResult.RedrawAll();
	
	SetSearchResultState(nTotalCount, stDRAW_OUT_HEAD.lFindTime);

	if (nTotalCount > 0)
		m_gridResult.SetSelectRow(0);

	return TRUE;
}


void CMainFrmWnd::SetSearchResultState(LONG lCnt/* = 0*/, LONG lTime/* = -1*/)
{
	CString strResult;
	CString strTime, strTemp;
	strResult.Format(" %d��", lCnt);
	
	/*
	if (lCnt == 0)
	m_gridResult.ClearData(TRUE);
	*/
	
	if (lTime == -1)
	{
		m_ctrlCodeCnt.SetWindowText("0");		
		strTime = "";

		CTime timer;
		timer = CTime::GetCurrentTime();
	
		strTime.Format("%02d:%02d:%02d", timer.GetHour(), timer.GetMinute(), timer.GetSecond());		
	}
	else
	{
		int nLength;
		strTemp.Format("%06d", lTime);
		nLength = strTemp.GetLength();
		
		if (nLength == 3)
		{
			strTime = "0" + strTemp.Left(1) + ":";
			strTime += strTemp.Right(2);
		}
		else if (nLength == 4)
		{
			strTime = strTemp.Left(2) + ":";
			strTime += strTemp.Right(2);
		}
		else if (nLength == 6)
		{
			strTime = strTemp.Left(2) + ":";
			strTime += strTemp.Mid(2, 2) + ":";
			strTime += strTemp.Right(2);
		}
	}
	m_ctrlCodeCntText.ShowWindow(SW_SHOW);
	m_ctrlCodeCnt.SetWindowText(strResult);
	m_ctrlTime.SetWindowText("�˻��ð�:" + strTime);
	SetStateTitleBar("��ȸ�Ϸ�");
	
}

void CMainFrmWnd::SetByteOrderRevDouble(LPSTR pDouble)
{
//	return ; //���������� ������� �����Ƿ� returnó����.

	int nCount, nLength = 4;
	
	for (nCount = 0; nCount < nLength; nCount++)
	{
		unsigned char chTemp;
		chTemp = *(pDouble + nCount);
		*(pDouble + nCount) = *(pDouble + sizeof(double) - 1 - nCount);
		*(pDouble + sizeof(double) - 1 - nCount) = chTemp;
	}
}

HBRUSH CMainFrmWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFrameWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(m_clrMainSkin);
		//2009.10.08 by LYH >> window7 �׸� ����(�������) <<
		pDC->SetTextColor(RGB(0,0,0));
		//	pDC->SetBkColor(COLOR_MAIN_CANVAS);		
		hbr = m_brushBK;
	}	
	return hbr;
}

BOOL CMainFrmWnd::SetLoad_TargetBase_2nd(CString strTargetBaseType)
{
	BOOL bResult = TRUE;
	//# 0 : ��ü, 1:Kospi, 2:Kosdak, 3:����
	
	int nTargetBaseType_0st, nTargetBaseType_1st;
	GetTargetBaseType(nTargetBaseType_0st, nTargetBaseType_1st);

	if (nTargetBaseType_0st == 1)// kospi
		SetLoad_CUMaster();
	else if (nTargetBaseType_0st == 2)// kosdaq
		SetLoad_CKUMaster();
// 	else if (nTargetBaseType_0st == 3)	//# �׸�
// 		SetLoad_CTHMaster();
	else if (nTargetBaseType_0st == 3)		//# ����
		SetLoad_PortGroup();
	
	return bResult;
}

int CMainFrmWnd::SetLoad_CUMaster()
{
	CStringArray* arCode;
	CStringArray* arName;
	arCode = new CStringArray;
	arName = new CStringArray;
	int nIndex = 0;
	
	m_comboTarget_2nd.ResetContent();

	CList<CUMaster, CUMaster&> list;
	
	GetUpMasterData(arCode, arName, GETMARKET_KOSPI);
	for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
	{
		CUMaster	clsUMaster;
		clsUMaster.m_szCode = arCode->GetAt(nIndex);
		clsUMaster.m_szName = arName->GetAt(nIndex);
		clsUMaster.m_szName.TrimRight();
		clsUMaster.m_szGubun = _T("");
		list.AddTail(clsUMaster);
	}
	arCode->RemoveAll();
	arName->RemoveAll();

	GetUpMasterData(arCode, arName, GETMARKET_KOSPI200);
	for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
	{
		if(arCode->GetAt(nIndex) == "1")
		{
			CUMaster	clsUMaster;
			clsUMaster.m_szCode = arCode->GetAt(nIndex);
			clsUMaster.m_szName = arName->GetAt(nIndex);
			clsUMaster.m_szName.TrimRight();
			clsUMaster.m_szGubun = _T("");
			list.AddTail(clsUMaster);
		}
	}	
	delete arCode;
	delete arName;
		
	int nCode;
	char* pBuff;
	char aKeyB[5+1];
	for (POSITION pos=list.GetHeadPosition(); pos;)
	{
		CUMaster data = list.GetNext(pos);
		sprintf(aKeyB, "B%04.4s", data.m_szCode);// data.m_szCode.Mid(0));
		if(strcmp(aKeyB, "B1001")==0) 
			continue;
		
		nIndex = m_comboTarget_2nd.AddString(data.m_szName);
		INT_CONVERT(pBuff, data.m_szCode, sizeof(data.m_szCode), nCode);
		m_comboTarget_2nd.SetItemData(nIndex, (DWORD)nCode);
	//	m_comboTarget_2nd.SetItemData(nIndex, (DWORD&)data.m_szCode); 
	}
	
	m_comboTarget_2nd.SetCurSel(0);	

	return m_comboTarget_2nd.GetCount();
}

int CMainFrmWnd::SetLoad_CKUMaster()
{
	CStringArray* arCode;
	CStringArray* arName;
	arCode = new CStringArray;
	arName = new CStringArray;
	int nIndex = 0;
	
	m_comboTarget_2nd.ResetContent();
	
	CList<CUMaster, CUMaster&> list;
	
	GetUpMasterData(arCode, arName, GETMARKET_KOSDAQ);
	for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
	{
		CUMaster	clsUMaster;
		clsUMaster.m_szCode = arCode->GetAt(nIndex);
		clsUMaster.m_szName = arName->GetAt(nIndex);
		clsUMaster.m_szName.TrimRight();
		clsUMaster.m_szGubun = _T("");
		list.AddTail(clsUMaster);
	}		
	delete arCode;
	delete arName;
	
	int nCode;
	char* pBuff;
	char aKeyB[5+1];
	for (POSITION pos=list.GetHeadPosition(); pos;)
	{
		CUMaster data = list.GetNext(pos);
		sprintf(aKeyB, "B%04.4s", data.m_szCode);// data.m_szCode.Mid(0));
		if(strcmp(aKeyB, "B2001")==0) 
			continue;
		
		nIndex = m_comboTarget_2nd.AddString(data.m_szName);
		INT_CONVERT(pBuff, data.m_szCode, sizeof(data.m_szCode), nCode);
		m_comboTarget_2nd.SetItemData(nIndex, (DWORD)nCode);
		//	m_comboTarget_2nd.SetItemData(nIndex, (DWORD&)data.m_szCode); 
	}
	
	m_comboTarget_2nd.SetCurSel(0);	
	
	return m_comboTarget_2nd.GetCount();
}

BOOL CMainFrmWnd::GetData_UpJongToCode(UINT nCode, CString strGroupName)
{
	CStringArray arrCode, arrName;
	
	if(!strGroupName.CompareNoCase("KOSPI200") && nCode == 1)
		GetStockMasterData(arrCode, arrName, GETMARKET_KOSPI200, nCode);
	else
		GetStockMasterData(arrCode, arrName, m_nMarket, nCode);
	
	m_saCodeGroup.RemoveAll();
	for(int i=0; i < arrCode.GetSize(); i++)
	{
		m_saCodeGroup.Add(arrCode.GetAt(i));
	}

	return 0;
}

int CMainFrmWnd::SetLoad_CTHMaster()
{
	m_comboTarget_2nd.ResetContent();
	
	int nIndex;
	CString strOut, strData, strName;
	
	for(int nThemeIndex = 0; nThemeIndex < m_saCodeTheme.GetSize(); nThemeIndex++)
	{
		strOut = m_saCodeTheme.GetAt(nThemeIndex);
		strName = strOut.Mid(4, strOut.GetLength() - 4);
		strName.TrimRight();
		strData = strOut.Mid(0, 4);
		
		char strCode[5+1]={0,};
		memset(strCode, 0x00, 5);
		//memcpy(strCode, (LPCTSTR)strData, strData.GetLength());
		memcpy(strCode, (LPCTSTR)strData, 5);
		
		nIndex = m_comboTarget_2nd.AddString(strName);
	//	m_comboTarget_2nd.SetItemData(nIndex, (DWORD&)strCode);
		m_comboTarget_2nd.SetItemData(nIndex, (DWORD)atoi(strCode));
	}
	
	m_comboTarget_2nd.SetCurSel(0);
	
	return m_comboTarget_2nd.GetCount();
}

void CMainFrmWnd::GetData_ThemaToCode(CString strThemeCode)
{
 	m_saCodeGroup.RemoveAll();

	CStringArray arrCode, arrName;
	CString strCode;
	int nIndex;
	for(nIndex = 0; nIndex < arrCode.GetSize(); nIndex++)
	{
		strCode.Format("%6.6s", arrCode.GetAt(nIndex));
		m_saCodeGroup.Add(strCode);	
	}
}

int	CMainFrmWnd::SetLoad_PortGroup()
{
	m_comboTarget_2nd.ResetContent();
	
	int nType = 0;
	LPCSTR szGroup=NULL;
	CStringArray arrKey, arrName;
	
	GetData_PortGroup(arrKey, arrName);

	int nCnt = arrName.GetSize();
	int nIdx;
	for(int i=0; i<nCnt; i++)
	{
		nIdx = m_comboTarget_2nd.AddString(arrName.GetAt(i));
		m_comboTarget_2nd.SetItemData(nIdx, atoi(arrKey.GetAt(i)));
	}
	if(nCnt>0)
		m_comboTarget_2nd.SetCurSel(0);
	
	return nCnt;
}

void CMainFrmWnd::GetData_PortGroupToCode(CString szGroupKey, CStringArray *psaData)
{
	int nTargetBaseType_0st, nTargetBaseType_1st;
	GetTargetBaseType(nTargetBaseType_0st, nTargetBaseType_1st);

	psaData->RemoveAll();

	CStringArray strItemArray, strNameArray;

	GetData_PortItem(strItemArray, strNameArray, atoi(szGroupKey));

	CString strItem, strCode;
	int nSize = strItemArray.GetSize();
	for(int nIndex = 0; nIndex < nSize; nIndex++)
	{
		strItem = strItemArray.GetAt(nIndex);
		psaData->Add(strItem);
	}
}


BOOL CMainFrmWnd::GetLoadXml(CString strFileRoot, MSXML::IXMLDOMDocumentPtr &pXmlDom)
{
	char msg[500];
	CString strDebug;
	
	pXmlDom->async = false;
	if (!pXmlDom->load (strFileRoot.AllocSysString())) 
	{	
		MSXML::IXMLDOMParseErrorPtr pParsingErr = pXmlDom->parseError;			
		sprintf(msg, "Error 0x%.8X on line %d, position %d\r\n", 
			pParsingErr->errorCode,pParsingErr->line, pParsingErr->linepos);
		
		CString strTemp(BSTR(pParsingErr->reason));	
		strDebug = strTemp + CString(msg) + "\n" + strFileRoot;
		
		//SetMessageBox(strDebug, "[XML Files Error]", MB_OK);		
		return FALSE;
	}
	
	return TRUE;
}

void CMainFrmWnd::SetLoad_FieldInfo()
{
	MSXML::IXMLDOMDocumentPtr pXmlDom ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	
	CString strFileRoot;
	strFileRoot = m_strAppPath + FOLDER_FIND + "\\" + FIXFIELD_SYS_FILE;
	
	if (GetLoadXml(strFileRoot, pXmlDom) == TRUE)
	{
		GetLoadFixSysFieldData(pXmlDom->selectSingleNode("INDEX"), m_listFixHeadSys);
		m_strFixFieldVersion = GetVersion_FieldFile(pXmlDom->selectSingleNode("INDEX"));
	}
	
	BOOL bSetInit = TRUE;
	strFileRoot = m_strAppPath + "\\" + USRDIR + "\\" + Variant(nameCC) + FOLDER_FINDDATA + "\\" + FIXFIELD_USER_PATTERN_FILE;
	if (GetLoadXml(strFileRoot, pXmlDom) == TRUE)
	{
		//# Fix Field User Version Check!
		if (m_strFixFieldVersion == GetVersion_FieldFile(pXmlDom->selectSingleNode("INDEX")))
		{
			GetLoadFixUserFieldData(pXmlDom->selectSingleNode("INDEX"), m_listFixHeadUser);
			
			if (m_listFixHeadUser.GetCount() == 0)
				bSetInit = FALSE;
			else
				SetUpdateUserFieldData(&m_listFixHeadSys, &m_listFixHeadUser);		//# Info UpDate!
			
			bSetInit = FALSE;
		}
	}
	
	if (bSetInit == TRUE)
	{
		m_nFixCol = 2;
		m_listFixHeadUser.RemoveAll();
		
		CStringArray saFieldId;
		if (GetLoadFixField_Def(saFieldId) == TRUE)
		{
			m_listFixHeadUser.AddTail(m_stAlike);

			POSITION pos;
			HeadFixInfo		stHeadFixInfo;
			HeadInfoType	stHeadInfoType;
			for (int nIndex = 0; nIndex < m_listFixHeadSys.GetCount(); nIndex++)
			{
				pos = m_listFixHeadSys.FindIndex(nIndex);
				stHeadInfoType = m_listFixHeadSys.GetAt(pos);
				
				CString strFieldId;
				for (int nItem = 0; nItem < saFieldId.GetSize(); nItem++)
				{
					strFieldId = saFieldId.GetAt(nItem);
					if (stHeadInfoType.lHeadID == atol(strFieldId))
					{
						stHeadFixInfo.stHeadType = stHeadInfoType;
						stHeadFixInfo.nColNum = m_listFixHeadUser.GetCount();
						stHeadFixInfo.bView = TRUE;
						
						m_listFixHeadUser.AddTail(stHeadFixInfo);
						break;
					}
				}
			}
		}
	}
	
	SetSaveFixUserFieldData(&m_listFixHeadUser, m_strFixFieldVersion);
}

void CMainFrmWnd::SetGrid_Head(BOOL bView /*= FALSE*/)
{
	int nCol;
	POSITION pos;
	
	if (bView == TRUE)
	{
		m_gridResult.SetHeader(&m_listField);
		m_gridResult.m_plistField = &m_listField;
		
		m_gridResult.SetLockColumns(m_nFixCol, TRUE);	
	}
	else if (bView == FALSE)
	{
		m_listField.RemoveAll();
		
		HeadFixInfo	stHeadFixInfo;
		for (nCol = 0; nCol < m_listFixHeadUser.GetCount(); nCol++)
		{
			pos = m_listFixHeadUser.FindIndex(nCol);
			stHeadFixInfo = m_listFixHeadUser.GetAt(pos);
			
			if (stHeadFixInfo.bView == TRUE)
				m_listField.AddTail(stHeadFixInfo.stHeadType);
		}
	}
}

BOOL CMainFrmWnd::GetLoadSet_Def()
{
	CFileFind cFind;
	BOOL bRead = TRUE;
	
	m_nDefult_TargetBaseType = 0;
	m_nDefult_Slider = 40;
	m_nDefult_BongTerm = 2;
	m_nDefult_BongTerm_User = 0;
	m_nDefult_Similar = 2;
	m_nDefult_ChartTerm = 2;
	m_nDefult_ChartTermMin = 0;
	m_nDefult_ExceptPreferred = 1;

	CString strFileRoot = m_strAppPath + FOLDER_DATA + "\\" + PATTERNINFO_INIFILE;
	bRead = cFind.FindFile(strFileRoot);
	if (!bRead)
	{
		CString strMsg = (CString)PATTERNINFO_INIFILE + "�� ã�� ���߽��ϴ�.";
		SetStateTitleBar(strMsg);
	}
	else
	{
		CString strKey;
		strKey = "TargetBaseType";
		m_nDefult_TargetBaseType = GetPrivateProfileInt("DefaultSet", strKey, 0, strFileRoot);
		
		strKey = "Slider";
		m_nDefult_Slider = GetPrivateProfileInt("DefaultSet", strKey, 40, strFileRoot);
		
		strKey = "BongTerm";
		m_nDefult_BongTerm = GetPrivateProfileInt("DefaultSet", strKey, 2, strFileRoot);
		
		strKey = "BongTerm_User";
		m_nDefult_BongTerm_User = GetPrivateProfileInt("DefaultSet", strKey, 0, strFileRoot);

		strKey = "ExceptPreferred";
		m_nDefult_ExceptPreferred = GetPrivateProfileInt("DefaultSet", strKey, 1, strFileRoot);
		
		strKey = "Similar";
		m_nDefult_Similar = GetPrivateProfileInt("DefaultSet", strKey, 2, strFileRoot);
		
		strKey = "ChartTerm";// ��:1 ��:2 ��:3 ��:4
		m_nDefult_ChartTerm = GetPrivateProfileInt("DefaultSet", strKey, 2, strFileRoot);
		
		strKey = "ChartTermMin"; //�м���(1��:0, 3��:1, 5��:2, 10��:3, 15��:4, 30��:5, 60��:6)
		m_nDefult_ChartTermMin = GetPrivateProfileInt("DefaultSet", strKey, 0, strFileRoot);

		if(m_nDefult_TargetBaseType < 0 || m_nDefult_TargetBaseType > 4) m_nDefult_TargetBaseType = 0;
		if(m_nDefult_ChartTerm < 1 || m_nDefult_ChartTerm > 4) m_nDefult_ChartTerm = 2;
		if(m_nDefult_ChartTermMin < 0 || m_nDefult_ChartTermMin > 6) m_nDefult_ChartTermMin = 0;
		if(m_nDefult_ExceptPreferred < 0 || m_nDefult_ExceptPreferred > 1) m_nDefult_ExceptPreferred = 0;
		if(m_nDefult_Slider < 20 || m_nDefult_Slider > 90) m_nDefult_Slider = 40;
		if(m_nDefult_BongTerm < 0 || m_nDefult_BongTerm > 6) m_nDefult_BongTerm = 2;
		if(m_nDefult_BongTerm_User < 0 || m_nDefult_BongTerm_User > 1) m_nDefult_BongTerm_User = 0;
		if(m_nDefult_Similar < 0 || m_nDefult_Similar > 3) m_nDefult_Similar = 2;
	}
	
	return bRead;
}

BOOL CMainFrmWnd::GetLoadFixField_Def(CStringArray &saFieldId)
{
	CFileFind cFind;
	BOOL bRead = TRUE;

	CString strFilePath = GetFilePath("FINDERINFO");

	bRead = cFind.FindFile(strFilePath);
	if (!bRead)
	{
		CString strMsg = (CString)FINDERINFO_INIFILE + "�� ã�� ���߽��ϴ�.";
		SetSendMainMsg(strMsg);
	}
	else
	{
		char chColID[10];
		memset(chColID, ' ', sizeof(chColID));

		int nCount = GetPrivateProfileInt("DefaultColumn", "Line Count", 0, strFilePath);
		CString strTemp;
		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			strTemp.Format("%d", nIndex + 1);
			GetPrivateProfileString("DefaultColumn", strTemp, "", chColID, sizeof(chColID), strFilePath);
			saFieldId.Add(chColID);
		}
	}

	return bRead;
}

BOOL CMainFrmWnd::GetLoad_PatternItem(CStringArray &saPatternItem)
{
	CFileFind cFind;
	BOOL bRead = TRUE;
	
	saPatternItem.RemoveAll();
	
	CString strFileRoot = m_strAppPath + FOLDER_DATA + "\\" + PATTERNINFO_INIFILE;
	bRead = cFind.FindFile(strFileRoot);
	if (!bRead)
	{
		CString strMsg = (CString)PATTERNINFO_INIFILE + "�� ã�� ���߽��ϴ�.";
		SetStateTitleBar(strMsg);
	}
	else
	{
		char chBuff[BUFSIZ];
		memset(chBuff, 0x00, sizeof(chBuff));
		
		int nCount = GetPrivateProfileInt("Pattern Item", "Line Count", 0, strFileRoot);
		CString strTemp;
		int nIndex;
		for (nIndex = 0; nIndex < nCount; nIndex++)
		{
			strTemp.Format("%d", nIndex + 1);
			GetPrivateProfileString("Pattern Item", strTemp, "", chBuff, sizeof(chBuff), strFileRoot);
			saPatternItem.Add(chBuff);
		}

		nCount = GetPrivateProfileInt("Pattern Tip", "Line Count", 0, strFileRoot);
		for (nIndex = 0; nIndex < nCount; nIndex++)
		{
			strTemp.Format("%d", nIndex + 1);
			GetPrivateProfileString("Pattern Tip", strTemp, "", chBuff, sizeof(chBuff), strFileRoot);
			m_saPatternItemTip.Add(chBuff);
		}

	}
	
	return bRead;
}

CString CMainFrmWnd::GetVersion_FieldFile(MSXML::IXMLDOMNodePtr pNIndex)
{
	CString strResult = XML_VERSION_FIXFIELDFILE;
	
	MSXML::IXMLDOMNamedNodeMapPtr	pNAIndex;
	try 
	{
		MSXML::IXMLDOMNodePtr	pNAttributeTemp;
		pNAIndex = pNIndex->attributes;
		
		pNAttributeTemp = pNAIndex->getNamedItem("VERSION");
		strResult = CString((PCHAR)pNAttributeTemp->text);
	}
	catch(_com_error e) 
	{
		strResult = "";
	}
	
	return strResult;
}

BOOL CMainFrmWnd::SetSaveFixUserFieldData(CList<HeadFixInfo, HeadFixInfo&> *plistFixUser,
										  CString strVersion)
{
	BOOL bResult = TRUE;
	
	CString strFileRoot = m_strAppPath + "\\" + USRDIR + "\\" + Variant(nameCC) + FOLDER_FINDDATA + "\\" + FIXFIELD_USER_PATTERN_FILE;
	
	MSXML::IXMLDOMNodePtr						pNIndex;
	MSXML::IXMLDOMNamedNodeMapPtr		pNAIndex;
	
	MSXML::IXMLDOMNodePtr						pN_FItem;
	MSXML::IXMLDOMNamedNodeMapPtr		pNA_FItem;
	
	try 
	{
		CString strTemp;
		
		MSXML::IXMLDOMDocumentPtr pDOMIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
		
		MSXML::IXMLDOMProcessingInstructionPtr pPI;
		MSXML::IXMLDOMNamedNodeMapPtr					pNAPI;
		MSXML::IXMLDOMNodePtr									pNEncoding;
		
		pPI = pDOMIndex->createProcessingInstruction("xml", "version=\"1.0\"");
		
		pNEncoding = pDOMIndex->createAttribute("encoding");
		pNEncoding->text = "KSC_5601";
		
		pNAPI = pPI->attributes;
		pNAPI->setNamedItem(pNEncoding);
		
		pDOMIndex->appendChild(pPI);
		
		pNIndex = pDOMIndex->createElement("INDEX");
		pNAIndex = pNIndex->attributes;
		
		MSXML::IXMLDOMNodePtr pNAttributeTemp;
		pNAttributeTemp = pDOMIndex->createAttribute("VERSION");
		pNAttributeTemp->text = (LPCTSTR)strVersion;
		pNAIndex->setNamedItem(pNAttributeTemp);
		
		pNAttributeTemp = pDOMIndex->createAttribute("FIX_COL");
		strTemp.Format("%d", m_nFixCol);
		pNAttributeTemp->text = (LPCTSTR)strTemp;
		pNAIndex->setNamedItem(pNAttributeTemp);
		
		pDOMIndex->appendChild(pNIndex);
		
		POSITION pos;
		HeadFixInfo	stHeadFixInfo;
		int nFieldCount = plistFixUser->GetCount();
		for (int nIndex = 1;  nIndex < nFieldCount; nIndex++)	
		{
			pos = plistFixUser->FindIndex(nIndex);
			stHeadFixInfo = plistFixUser->GetAt(pos);
			
			MSXML::IXMLDOMNodePtr pNAttributeTemp;
			
			pN_FItem = pDOMIndex->createElement("FITEM");
			pNIndex->appendChild(pN_FItem);
			pNA_FItem = pN_FItem->attributes;
			
			//ID
			pNAttributeTemp = pDOMIndex->createAttribute("ID");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.lHeadID);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//COL_NO
			pNAttributeTemp = pDOMIndex->createAttribute("COL_NO");
			strTemp.Format("%d", stHeadFixInfo.nColNum);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//FIELD_NAME
			pNAttributeTemp = pDOMIndex->createAttribute("FIELD_NAME");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strHeadName;;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//UNIT
			pNAttributeTemp = pDOMIndex->createAttribute("UNIT");
			pNAttributeTemp->text = (LPCTSTR)"1";
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//ALIGN
			pNAttributeTemp = pDOMIndex->createAttribute("ALIGN");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strAlignment;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//COL_WIDTH
			pNAttributeTemp = pDOMIndex->createAttribute("COL_WIDTH");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.nColWidth);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//DATA_TYPE
			pNAttributeTemp = pDOMIndex->createAttribute("DATA_TYPE");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strDataType;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//VIEW_NAME
			pNAttributeTemp = pDOMIndex->createAttribute("VIEW_NAME");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strHeadName;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//VISIBLE
			pNAttributeTemp = pDOMIndex->createAttribute("VISIBLE");
			strTemp = (stHeadFixInfo.bView == TRUE)?"TRUE":"FALSE";
			pNAttributeTemp->text =	(LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//UNIT_NUM
			pNAttributeTemp = pDOMIndex->createAttribute("UNIT_NUM");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.nUnitNum);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//UNIT_TEXT
			pNAttributeTemp = pDOMIndex->createAttribute("UNIT_TEXT");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strUnitText;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//DATA_LENGTH
			pNAttributeTemp = pDOMIndex->createAttribute("DATA_LENGTH");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.nDataLength);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
		}
		
		pDOMIndex->save(_bstr_t(strFileRoot));
	}	
	catch(_com_error e)
	{
		bResult = FALSE;
	}
	
	return bResult;
}

void CMainFrmWnd::SetUpdateUserFieldData(CList<HeadInfoType, HeadInfoType&> *plistFixSys, 
										 CList<HeadFixInfo, HeadFixInfo&> *plistFixUser)
{
	POSITION	posSys, posUser;
	HeadInfoType stHeadInfoType;
	HeadFixInfo	 stHeadFixInfo;
	
	int nSysCnt = plistFixSys->GetCount();
	int nUserCnt = plistFixUser->GetCount();
	for (int nSysIndex = 0; nSysIndex < nSysCnt; nSysIndex++)
	{
		posSys = plistFixSys->FindIndex(nSysIndex);
		stHeadInfoType = plistFixSys->GetAt(posSys);
		
		for (int nUserIndex = 0; nUserIndex < nUserCnt; nUserIndex++)
		{
			posUser = plistFixUser->FindIndex(nUserIndex);
			stHeadFixInfo = plistFixUser->GetAt(posUser);
			
			if (stHeadInfoType.lHeadID == stHeadFixInfo.stHeadType.lHeadID)
			{
				if (stHeadFixInfo.stHeadType.strHeadName.Compare(stHeadInfoType.strHeadName) == -1)
					stHeadFixInfo.stHeadType.strHeadName = stHeadInfoType.strHeadName;
				
				stHeadFixInfo.stHeadType.strDataType = stHeadInfoType.strDataType;
				stHeadFixInfo.stHeadType.nUnitNum	= stHeadInfoType.nUnitNum;
				stHeadFixInfo.stHeadType.strUnitText = stHeadInfoType.strUnitText;
				stHeadFixInfo.stHeadType.nDataLength = stHeadInfoType.nDataLength;
				stHeadFixInfo.stHeadType.lHeadID = stHeadInfoType.lHeadID;
				stHeadFixInfo.stHeadType.strAlignment	= stHeadInfoType.strAlignment;
				stHeadFixInfo.stHeadType.nColWidth = stHeadInfoType.nColWidth;
				//				//stHeadFixInfo.strUNIT						= stHeadInfoType.strUNIT;
				//				//stHeadFixInfo.strVISIBLE					= stHeadInfoType.strVISIBLE;
				
				plistFixUser->SetAt(posUser, stHeadFixInfo);
			}
		}
	}
}

BOOL CMainFrmWnd::GetLoadFixSysFieldData(MSXML::IXMLDOMNodePtr pNIndex,
										 CList<HeadInfoType, HeadInfoType&> &listFixList)
{
	BOOL bResult = TRUE;
	
	MSXML::IXMLDOMNodeListPtr		pNS_FItem;	// Item List
	MSXML::IXMLDOMNodePtr			pN_FItem;	// Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;	// Item Attribute 	
	
	try
	{
		pNS_FItem  = pNIndex->selectNodes("FITEM");
		
		//System�� version�� �о�� �ʿ����
		//�ش� ����� �ε�Ǳ� ���� �̹� versionó�� �Ǿ�����.
		int nCount, nLength;
		nLength = pNS_FItem->length;
		for (nCount = 0; nCount < nLength; nCount++)
		{
			HeadInfoType stHeadInfoType;
			
			//node�� ���´�.
			pN_FItem = pNS_FItem->item[nCount];	
			
			//�Ӽ��� ���´�.
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNA_FItem= pN_FItem->attributes;
			
			//ID
			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
			stHeadInfoType.lHeadID = atol(CString((PCHAR)pNAttributeTemp->text));
			
			// ��, �� �ʵ� ����
			if(stHeadInfoType.lHeadID == 8000 || stHeadInfoType.lHeadID == 8001)
				continue;

			//FIELD_NAME
			pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
			stHeadInfoType.strHeadName = CString((PCHAR)pNAttributeTemp->text);
			
			//ALIGN
			pNAttributeTemp = pNA_FItem->getNamedItem("ALIGN");
			stHeadInfoType.strAlignment = CString((PCHAR)pNAttributeTemp->text);
			
			//COL_WIDTH
			pNAttributeTemp = pNA_FItem->getNamedItem("COL_WIDTH");
			stHeadInfoType.nColWidth = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			//DATA_TYPE
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_TYPE");
			stHeadInfoType.strDataType = CString((PCHAR)pNAttributeTemp->text);
			
			//UNIT_NUM
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_NUM");
			stHeadInfoType.nUnitNum = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			//UNIT_TEXT
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_TEXT");
			stHeadInfoType.strUnitText = CString((PCHAR)pNAttributeTemp->text);
			
			//DATA_LENGTH
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_LENGTH");
			stHeadInfoType.nDataLength = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			//			//UNIT
			//			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT");
			//			stHeadInfoType.nDataLength = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			// �ý��ۿ� ����Ʈ�� �߰�
			listFixList.AddTail(stHeadInfoType);
		}
	}
	catch(_com_error e)
	{
		CString strMsg = "�ý��� ������ �о���� �� ������ �߻��Ͽ����ϴ�. ���������ͷ� ���� �ٶ��ϴ�.";
		//SetSendMainMsg(strMsg);
		bResult = FALSE;
	}
	
	return bResult;
}

BOOL CMainFrmWnd::GetLoadFixUserFieldData(MSXML::IXMLDOMNodePtr pNIndex,
										  CList<HeadFixInfo, HeadFixInfo&> &listFixList)
{
	BOOL bResult = TRUE;
	
	MSXML::IXMLDOMNamedNodeMapPtr	pNAIndex;
	
	MSXML::IXMLDOMNodeListPtr			pNS_FItem;		// Item List
	MSXML::IXMLDOMNodePtr					pN_FItem;	// Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;		// Item Attribute
	
	CString							strView;
	CString							strViewName;
	try 
	{
		MSXML::IXMLDOMNodePtr	pNAttributeTemp;
		pNAIndex = pNIndex->attributes;
		
		pNAttributeTemp = pNAIndex->getNamedItem("VERSION");
		//	m_strFileVersion = CString((PCHAR)pNAttributeTemp->text);			
		
		pNAttributeTemp = pNAIndex->getNamedItem("FIX_COL");
		m_nFixCol = atoi(CString((PCHAR)pNAttributeTemp->text));
		
		pNS_FItem  = pNIndex->selectNodes("FITEM");
			
		//Attribute�� ���´�
		int nCount, nLength;
		nLength = pNS_FItem->length;
		for (nCount = 0; nCount < nLength; nCount++)
		{
			HeadFixInfo stHeadFixInfo;
			
			//node�� ���´�.
			pN_FItem = pNS_FItem->item[nCount];	
			
			//�Ӽ��� ���´�.
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNA_FItem = pN_FItem->attributes;
			
			//ID
			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
			stHeadFixInfo.stHeadType.lHeadID = atol(CString((PCHAR)pNAttributeTemp->text));
			
			//COL_NO
			pNAttributeTemp = pNA_FItem->getNamedItem("COL_NO");
			int nCol = atoi(CString((PCHAR)pNAttributeTemp->text));
			stHeadFixInfo.nColNum = nCol++; // ���絵 �����ʵ带 ���� col���� �ϳ��� �̵�
			
			//FIELD_NAME
			pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
			stHeadFixInfo.stHeadType.strHeadName = CString((PCHAR)pNAttributeTemp->text);
			
			// ���� �ʵ� �������� ���� ���絵 �ʵ� ����, �������� >>
			if(nCount == 0 && stHeadFixInfo.stHeadType.strHeadName != "���絵")
				listFixList.AddTail(m_stAlike);
			// <<
			
			//UNIT_NUM
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_NUM");
			stHeadFixInfo.stHeadType.nUnitNum = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			//UNIT_TEXT
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_TEXT");
			stHeadFixInfo.stHeadType.strUnitText = CString((PCHAR)pNAttributeTemp->text);
			
			//ALIGN
			pNAttributeTemp = pNA_FItem->getNamedItem("ALIGN");
			stHeadFixInfo.stHeadType.strAlignment = CString((PCHAR)pNAttributeTemp->text);
			
			//COL_WIDTH
			pNAttributeTemp = pNA_FItem->getNamedItem("COL_WIDTH");
			stHeadFixInfo.stHeadType.nColWidth = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			//DATA_TYPE
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_TYPE");
			stHeadFixInfo.stHeadType.strDataType = CString((PCHAR)pNAttributeTemp->text);
			
			//DATA_LENGTH
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_LENGTH");
			stHeadFixInfo.stHeadType.nDataLength = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			
			//VIEW_NAME
			pNAttributeTemp = pNA_FItem->getNamedItem("VIEW_NAME");
			strViewName = CString((PCHAR)pNAttributeTemp->text);
			
			//VISIBLE
			pNAttributeTemp = pNA_FItem->getNamedItem("VISIBLE");
			stHeadFixInfo.bView = (CString((PCHAR)pNAttributeTemp->text) == "TRUE")?TRUE:FALSE;
			
			listFixList.AddTail(stHeadFixInfo);
		}
		
		if(nLength == 0)
			listFixList.AddTail(m_stAlike);
	}
	catch(_com_error e) 
	{
		CString strMsg = "����� ������ �о���� �� ������ �߻��Ͽ����ϴ�. ���������ͷ� ���� �ٶ��ϴ�.";
		//SetSendMainMsg(strMsg);
		bResult = FALSE;
	}
	
	return bResult;
}

void CMainFrmWnd::SetSharedDataToCode(CString strCode)
{
	if (strCode.IsEmpty() == FALSE)
	{
		CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
		pScreenWnd->ShareCode(strCode);
	}
}

void CMainFrmWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar->m_hWnd == m_ctrlSlider.m_hWnd)
	{
		CString strData;
		strData.Format("%d", nPos);
		//m_ctrlSliderVal.SetWindowText(strData);
		m_editPeriod.SetWindowText(strData);
		
		if (nPos == 0)
		{
			m_nPrevScale = m_ctrlSlider.GetPos();
			strData.Format("%d", m_nPrevScale);

			m_editPeriod.SetWindowText(strData);
			
			m_pctrlDrawPatten->SetData_XGridCnt(m_ctrlSlider.GetPos());
		}
	}
	
	CFrameWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMainFrmWnd::SetStateTitleBar(CString strMsg)
{
	CString strData = "������� : ";
	strData += strMsg;
	if(m_ctrlState.GetSafeHwnd() && ::IsWindow(m_ctrlState.GetSafeHwnd()))
	{
		m_ctrlState.SetWindowText(strData);
		m_ctrlState.Invalidate();
	}
}

void CMainFrmWnd::SetEnable(BOOL bEnable)
{
	m_comboTarget_1st.EnableWindow(bEnable);
	
	m_btnReverseChk.EnableWindow(bEnable);
	if (bEnable == TRUE && m_comboTarget_1st.GetCurSel() == 0)
	{
		m_comboTarget_2nd.EnableWindow(FALSE);
		m_btnReverseChk.EnableWindow(FALSE);
	}
	m_btnExceptionChk.EnableWindow(bEnable);
	
	m_btnDay.EnableWindow(bEnable);
	m_btnWeek.EnableWindow(bEnable);
	m_btnMonth.EnableWindow(bEnable);
	m_btnMin.EnableWindow(bEnable);
	
	m_comboTime.EnableWindow(bEnable);
	if (bEnable == TRUE)
		m_comboTime.EnableWindow(m_btnMin.GetCheck());
	
	m_gridResult.EnableWindow(bEnable);
	m_editPeriod.EnableWindow(bEnable);
// 	m_btnLoad.EnableWindow(bEnable);
// 	m_btnSave.EnableWindow(bEnable);
// 	m_btnReset.EnableWindow(bEnable);
// 	m_btnSearch.EnableWindow(bEnable);
	
	if (m_pctrlDrawPatten != NULL)
		m_pctrlDrawPatten->SetEnable(bEnable);
	
	m_ctrlSlider.EnableWindow(bEnable);
	m_btnBongChk.EnableWindow(bEnable);
	
	m_comboBongTerm.EnableWindow(bEnable);
	if (bEnable == TRUE)
		m_comboBongTerm.EnableWindow(m_btnBongChk.GetCheck());
	
	m_comboSimilar.EnableWindow(bEnable);
	m_ctrlSlider.EnableWindow(bEnable);

//	m_btnSearch.SetFocus();
}

void CMainFrmWnd::DispatchCodeInput(COleDispatchDriver* pDispatch)
{
//	m_pctrlCodeInput = (IJMComboCtl*)pDispatch;
}

void CMainFrmWnd::InMakeFid_Packet(int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos,
								   int nOut1, int nOut2)
{
	memset(&szSaveBuf[nPos], 0x20, 80);
	nPos += 80;
	
	//2. TR�� ���ڿ��� �����.
	int nSendTrDataSize = nTrLen;
	memcpy(&szSaveBuf[nPos], pTrData, nSendTrDataSize);
	nPos += nSendTrDataSize;
	
	//3. ���ڵ� ��ȣ 1���� ���ʷ� 
	CString szOutRecord;
	
	// 1st Record
	szOutRecord.Format("%-16d", 1);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();
	
//	m_pDataUtilMng->GetFillString(szOutRecord, 0, nOut1 - 1, 3);	// 0 ~ 8
//	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();
	
	memcpy(&szSaveBuf[nPos], "|", 1);	//������.
	nPos++;
	
	// 2nd Record
	szOutRecord.Format("%-16d", 2);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();
	
//	m_pDataUtilMng->GetFillString(szOutRecord, 0, nOut2 - 1, 3);	// 0 ~ 9
//	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();
	
	memcpy(&szSaveBuf[nPos], ";", 1);	//������.
	nPos++;
	
}

// void CMainFrmWnd::ConvertMstData(LPCSTR szTR, LPVOID	aTRData, int dwTRDataLen)
// {
// 	m_saCodeMST.RemoveAll();
// 	
// 	CString strAddData, strData;
// 	
// 	strAddData = "";
// 	char *pBuff;
// 	
// 	MSTCode stMSTCode;
// 	char* pstData = (char*)aTRData;
// 	
// 	int nDataCnt = dwTRDataLen / sizeof(MSTCode);
// 	CString strCode;
// 	int nPos = 0;
// 	
// 	for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
// 	{
// 		memset(&stMSTCode, 0x00, sizeof(MSTCode));	
// 		memcpy(&stMSTCode, &pstData[nPos], sizeof(MSTCode));
// 		nPos += sizeof(MSTCode);
// 		
// 		STR_CONVERT(pBuff, stMSTCode.sMarket, sizeof(stMSTCode.sMarket), strData);
// 		strAddData = strData;
// 		
// 		STR_CONVERT(pBuff, stMSTCode.sCode, sizeof(stMSTCode.sCode), strData);
// 		strData.TrimLeft(" ");
// 		strData.TrimRight(" ");
// 		
// 		strAddData += strData;
// 		m_saCodeMST.Add(strAddData); //NULL�� �����Ƿ� �̷��� �ص� ��.
// 	}
// }

BOOL CMainFrmWnd::PreTranslateMessage(MSG* pMsg) 
{
	if(m_WaitCursor.WaitCursorProc(pMsg)) return TRUE;

	if(pMsg->message == WM_KEYDOWN)  
	{		
		UINT nChar = (UINT)pMsg->wParam;
		switch(nChar)
		{
		case VK_RETURN :
			{
				return FALSE;
			}
		case VK_TAB:
			if(::GetKeyState(VK_CONTROL) < 0) // �Ｚ: ctrl-tab �� �ȸԴ� ���� ������
			{
				m_btnSearch.SetFocus();
				GetParent()->GetParent()->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
				m_btnSearch.SetFocus();
				return FALSE;
			}
		}
	}
	else if(pMsg->message == WM_SYSKEYDOWN)
	{
		if( ::GetKeyState(VK_MENU) & 0x8000)
			return FALSE;
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrmWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	SetCommMenu(&point);

	CFrameWnd::OnRButtonUp(nFlags, point);
}

extern void KSLink_SetFormLinkTagData( CString sTag, CString sValue );
//extern void KSLink_SetLinkTagData( LPCTSTR sTag, LPCTSTR sValue);
void CMainFrmWnd::SetCommMenu(POINT *point)
{
//	HWND hChildFrame = (HWND)m_pKSGateMng->GetChildFrame();
//	if(!hChildFrame) return;
//
//	long lMenu = ::SendMessage(hChildFrame, UM_GET_SCREENLINKMENU, 0, 3 );
//	if ( lMenu > 0 )
//	{
//		CMenu* pMenu = (CMenu*)lMenu;
//		ASSERT ( pMenu );
//
//		GetCursorPos(point);
//		UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
//		int nRetValue = pMenu->TrackPopupMenu(nFlagsForMenu, point->x, point->y, this);
//		
//		if( nRetValue>0 )
//		{
//			::PostMessage(hChildFrame, WM_COMMAND, nRetValue, 0);
//			return;
//		}
//		pMenu->DestroyMenu();
//		delete pMenu;
//	}
}

void CMainFrmWnd::OnClickListItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMITEMACTIVATE* pIA = (LPNMITEMACTIVATE) pNMHDR;

	int nSelIndex = pIA->iItem;
	if(nSelIndex == -1) return;

	if (m_saPatternItemFile.GetSize() <= 0)
		return;
	
	CString strPath_Item;
	strPath_Item = m_strAppPath + (CString)FOLDER_DATA + "\\" + m_saPatternItemFile.GetAt(nSelIndex) + ".pdd";
	
	if (m_pctrlDrawPatten->GetLoad_DrawData(strPath_Item) == TRUE)
	{
		m_strCaption = m_saPatternItemTip.GetAt(nSelIndex);
		OnBtnSearch();
	}

	*pResult = 0;
}

void CMainFrmWnd::OnClickUserListItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMITEMACTIVATE* pIA = (LPNMITEMACTIVATE) pNMHDR;

	int nSelIndex = pIA->iItem;
	if(nSelIndex == -1) return;

	if (m_saUserPatternFiles.GetSize() <= 0)
		return;
	
	CString strPath_Item;
	strPath_Item = m_strUserPatternPath + "\\" + m_saUserPatternFiles.GetAt(nSelIndex);
	
	if (m_pctrlDrawPatten->GetLoad_DrawData(strPath_Item) == TRUE)
	{
		CString strTipText = m_saUserPatternFiles.GetAt(nSelIndex);
		strTipText.Replace(".pdd", "");

		m_strCaption = strTipText;
		OnBtnSearch();
	}

	*pResult = 0;
}

void CMainFrmWnd::OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
  int nTabIndex = m_ptabCondition->GetCurSel();

  BOOL bTabChange = TRUE;
  if(nTabIndex == 1)
  {
 	  m_ctrlListItem.ShowWindow(SW_HIDE);

	  m_ctrlUserListItem.ShowWindow(SW_SHOW);
	  m_ctrlUserListItem.BringWindowToTop();
	  m_ctrlUserListItem.Invalidate();
  }
  else
  {
 	  m_ctrlUserListItem.ShowWindow(SW_HIDE);

	  m_ctrlListItem.ShowWindow(SW_SHOW);
	  m_ctrlListItem.BringWindowToTop();
	  m_ctrlListItem.Invalidate();
  }
}

void CMainFrmWnd::OnBtnPrint()
{
	SetResultPrint();
}

void CMainFrmWnd::SetResultPrint()
{
	CPrintDialog printDlg(FALSE);
	if(printDlg.DoModal() != IDOK) return;

	CDC dc;
	dc.Attach(printDlg.GetPrinterDC());
	dc.m_bPrinting = TRUE;


	//�μ�� ��ť��Ʈ�� ���� ���� ����
	CString strTitle;
	strTitle = "���ǰ˻�";
	DOCINFO di;
	::ZeroMemory(&di, sizeof(DOCINFO));
	di.cbSize = sizeof(di);
	di.lpszDocName = strTitle;


	//��ť��Ʈ �μ� ����
	BOOL bPrintingOK = dc.StartDoc(&di);

	//CPrintInfo�� �μ� ���� ��������
	CPrintInfo Info;
	Info.m_rectDraw.SetRect(0, 0, dc.GetDeviceCaps(HORZRES),
		dc.GetDeviceCaps(VERTRES));
	
	
	//����Ʈ�� �� ��Ÿ��
	m_gridResult.SetBorderStyle4Print(UG_BDR_LTHIN | UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	
	//����Ʈ�� �� ���� ����
	m_gridResult.SetCellBorderColor4Print(RGB(0, 0, 0));


	//top��� ��� ����
	m_gridResult.PrintingSetOption(UG_PRINT_TOPHEADING, TRUE);
	

	//�¿��� ����.
	m_gridResult.PrintingSetOption(UG_PRINT_LEFTMARGIN, 5);
	m_gridResult.PrintingSetOption(UG_PRINT_RIGHTMARGIN, 5);

	
	//������ ����
	Info.SetMaxPage(
		m_gridResult.PrintingInit(&dc, &printDlg, 0, 0, m_gridResult.GetNumberCols()-1, m_gridResult.GetNumberRows()-1)
		);

	
	//�������� ����Ʈ
	for(UINT page = Info.GetMinPage(); page <= Info.GetMaxPage() && bPrintingOK; page++)
	{
		dc.StartPage();
		Info.m_nCurPage = page;

		m_gridResult.PrintingPage(&dc, Info.m_nCurPage);

		m_gridResult.PrintingTail(&dc, &Info);

		bPrintingOK = (dc.EndPage() > 0);
	}


	if(bPrintingOK) dc.EndDoc();
	else			dc.AbortDoc();

	
	dc.Detach();
}
//
HeadInfoType CMainFrmWnd::GetMoveFieldPosInfo()
{
	HeadInfoType	stHeadInfoType;

	//# ������ ��ġ!
// 	stHeadInfoType.lHeadID = (long)GRID_MOVEFIELD_POS_INDEX;
// 	stHeadInfoType.strHeadName = (CString)GRID_MOVEFIELD_POS_NAME;
// 	stHeadInfoType.nUnitNum = 0;
// 	stHeadInfoType.strUnitText = "";
// 	stHeadInfoType.strDataType = "";
// 	stHeadInfoType.nDataLength = 0;
// 	stHeadInfoType.nColWidth = 0;
// 	stHeadInfoType.strAlignment = "";

	return stHeadInfoType;
}

// CString CMainFrmWnd::GetFilePath(CString strLoadType)
// {
// 	CString strFile, strFolder;
// 	CString strFilePath = "";
// 	CString strAppPath = "";
// 	
// 	BOOL bUserFolder = FALSE;
// 	BOOL bSetIndustry = TRUE;
// 	
// 	strFile = (CString)FIXFIELD_USER_FILE;;
// 	
// 	GETPATH_APP(strAppPath);
// 	strFilePath = strAppPath + (CString)FOLDER_DATA + "\\" + strFile;
// 	
// 	return strFilePath;
// }

int CMainFrmWnd::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	return ::MessageBox(m_hWnd, lpszText, lpszCaption, nType);
}

void CMainFrmWnd::SetFieldEditApply()
{
	SetSaveFixUserFieldData(&m_listFixHeadUser, m_strFixFieldVersion);

	SetGrid_Head();
	SetGrid_Head(TRUE);
	OnBtnSearch();
}

void CMainFrmWnd::OnBtnFieldEdit()
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	CFieldEditDlg dlg(this, 0);
	if(dlg.DoModal() == IDOK)
	{

	}

	AfxSetResourceHandle(hInstSave);
}

void CMainFrmWnd::OnBtnResultCtrl()
{
	CRect reClient;
	m_btnResultCtrl.GetWindowRect(reClient);

	CPoint ptMenu;
	ptMenu.x = reClient.left;
	ptMenu.y = reClient.bottom;
	SetPopupMenuResultCtrl(ptMenu);
}

void CMainFrmWnd::OnBtnResultExpand()
{
	m_bExpand = !m_bExpand;

	if(m_bExpand)
	{
		m_pctrlDrawPatten->ShowWindow(SW_HIDE);
		m_btnBongChk.ShowWindow(SW_HIDE);
		m_comboBongTerm.ShowWindow(SW_HIDE);
		m_comboSimilar.ShowWindow(SW_HIDE);
//		m_ctrlSliderVal.ShowWindow(SW_HIDE);
		m_ctrlSlider.ShowWindow(SW_HIDE);

//		m_btnSearch.ShowWindow(SW_HIDE);
		m_btnReset.ShowWindow(SW_HIDE);
		m_btnSave.ShowWindow(SW_HIDE);

		m_ctrlPeriod.ShowWindow(SW_HIDE);
		m_ctrlSimilarity.ShowWindow(SW_HIDE);
		m_editPeriod.ShowWindow(SW_HIDE);

		m_btnMin.ShowWindow(SW_HIDE);
		m_btnMonth.ShowWindow(SW_HIDE);
		m_btnWeek.ShowWindow(SW_HIDE);
		m_btnDay.ShowWindow(SW_HIDE);
		m_comboTime.ShowWindow(SW_HIDE);

		SetBtnImg(&m_btnExpand, "DOWN_EX");
		m_pDlgChart->ShowWindow(SW_HIDE);

		m_ctrlPatternName.ShowWindow(SW_HIDE);
		m_ctrlJongName.ShowWindow(SW_HIDE);
		m_ctrlSimilar.ShowWindow(SW_HIDE);
	}
	else
	{
		m_pctrlDrawPatten->ShowWindow(SW_SHOW);
		m_btnBongChk.ShowWindow(SW_SHOW);
		m_comboBongTerm.ShowWindow(SW_SHOW);
		m_comboSimilar.ShowWindow(SW_SHOW);
//		m_ctrlSliderVal.ShowWindow(SW_SHOW);
		m_ctrlSlider.ShowWindow(SW_SHOW);

//		m_btnSearch.ShowWindow(SW_SHOW);
		m_btnReset.ShowWindow(SW_SHOW);
		m_btnSave.ShowWindow(SW_SHOW);

		m_ctrlPeriod.ShowWindow(SW_SHOW);
		m_ctrlSimilarity.ShowWindow(SW_SHOW);
		m_editPeriod.ShowWindow(SW_SHOW);

		m_btnMin.ShowWindow(SW_SHOW);
		m_btnMonth.ShowWindow(SW_SHOW);
		m_btnWeek.ShowWindow(SW_SHOW);
		m_btnDay.ShowWindow(SW_SHOW);
		m_comboTime.ShowWindow(SW_SHOW);

		SetBtnImg(&m_btnExpand, "UP_EX");
		m_pDlgChart->ShowWindow(SW_SHOW);

		m_ctrlPatternName.ShowWindow(SW_SHOW);
		m_ctrlJongName.ShowWindow(SW_SHOW);
		m_ctrlSimilar.ShowWindow(SW_SHOW);
	}

	MoveControls();

// 	int nX;
// 	int nY;
// 	CRect rt;
// 
// 	m_btnExpand.GetWindowRect(rt);
// 	nX = rt.left + rt.Width()/2;
// 	nY = rt.top + rt.Height()/2;
// 
// 	SetCursorPos(nX, nY);

	Invalidate();
}

void CMainFrmWnd::SetPopupMenuResultCtrl(CPoint ptMenu)
{
	int bIsHTS = 1;
	
	CMenu menu;
	menu.CreatePopupMenu();
	
	CMenu *pMenuChild = NULL;
	pMenuChild = new CMenu;
	pMenuChild->CreatePopupMenu();

	CString strScreenInfo;
	int nIndex, nMenuType, nMenuCnt;

	nMenuCnt = _countof(MENU_RESULTCTRL_NAME);
	for (nIndex = 0; nIndex < nMenuCnt; nIndex++)
	{
		if (MENU_RESULTCTRL_NAME[nIndex] == "")
			nMenuType = MF_SEPARATOR;
		else
			nMenuType = MF_STRING;

		if(bIsHTS)
		{
			strScreenInfo = (CString)MENU_RESULTCTRL_SCREEN_NOMAX[nIndex];

			if (atoi(strScreenInfo) <= 0)
			strScreenInfo = "";
		
			if (strScreenInfo.IsEmpty() == FALSE)
			{
				strScreenInfo = strScreenInfo.Left(4);
				strScreenInfo = "[" + strScreenInfo + "] " + (CString)MENU_RESULTCTRL_NAME[nIndex];
			}
			else
				strScreenInfo = "          " + (CString)MENU_RESULTCTRL_NAME[nIndex];
		}
		else
		{
			strScreenInfo = (CString)MENU_RESULTCTRL_SCREEN_NOEMP[nIndex];
			if (atoi(strScreenInfo) <= 0)
			strScreenInfo = "";
		
			if (strScreenInfo.IsEmpty() == FALSE)
			{
				strScreenInfo = strScreenInfo.Left(5);
				strScreenInfo = "[" + strScreenInfo + "] " + (CString)MENU_RESULTCTRL_NAME[nIndex];
			}
			else
				strScreenInfo = "          " + (CString)MENU_RESULTCTRL_NAME[nIndex];
		}			
		if(nIndex == 1)
		{
			menu.AppendMenu(MF_POPUP, (UINT)pMenuChild->GetSafeHmenu(), (LPCTSTR)strScreenInfo); 
			
			CStringArray arrKey, arrGroupName;
			GetData_PortGroup(arrKey, arrGroupName);
			
			int nMenuIndex = 11200;
			for(int i = 0; i < arrKey.GetSize(); i++)
				pMenuChild->AppendMenu(MF_STRING, nMenuIndex + atoi(arrKey.GetAt(i)), arrGroupName.GetAt(i));
			
			pMenuChild->AppendMenu(MF_SEPARATOR);
			
			pMenuChild->AppendMenu(MF_STRING, nMenuIndex, "�� ���ɱ׷� �߰�");
			
		}
		else
			menu.AppendMenu(nMenuType, MENU_RESULTCTRL_INDEX[nIndex], (LPCTSTR)strScreenInfo); 
	}

	GetCursorPos(&ptMenu);
	
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptMenu.x, ptMenu.y, this);

	pMenuChild = menu.GetSubMenu(0);
	if (pMenuChild)
	{
		pMenuChild->DestroyMenu();
		delete pMenuChild;
	}

	menu.DestroyMenu();		
}

// �˾� �޴� command ó��
void CMainFrmWnd::OnMenuResultCtrl(UINT nIndex)
{
	int bIsHTS = 1;

	CString strCode;
	CString strName;
	int nMarkType;
	ShardData		stShardData;
	CString strPortGroup;
	strPortGroup.Format("%d", nIndex);
	
	if(nIndex >= 11200)
		nIndex = 11020;

	int nDataCnt;
	nDataCnt = m_gridResult.GetNumberRows();

	if(nDataCnt <= 0)
	{
		SetSendMainMsg("������ �����Ͱ� ����.");
		return;
	}

	int nItem;
	for(nItem = 0; nItem < _countof(MENU_RESULTCTRL_INDEX); nItem++)
	{
		if(nIndex == MENU_RESULTCTRL_INDEX[nItem])
			break;
	}

	if (nIndex == (int)SCREEN_INDEX_EXCEL)
	{
		SetResultSave();
	}
	else if(nIndex == (int)SCREEN_INDEX_RUSULTPRINT)
	{
		SetResultPrint();
	}
	else
	{
		BOOL bCodeCheck=FALSE;
		if(atoi(strPortGroup) >= 11200)
		{
			stShardData.saCode.Add(strPortGroup.Right(2));
			stShardData.saName.Add("");
			stShardData.iaMarkType.Add(0);
		}

		for (int nRow = 0; nRow < nDataCnt; nRow++)
		{
			bCodeCheck = m_gridResult.IsRowCodeChecked(nRow);
			if(bCodeCheck)
			{
				/*m_gridResult.GetRowCode(nRow, strCode, nMarkType);*/
				m_gridResult.GetRowToCodeName(nRow, strCode, strName, nMarkType);

				if (!strCode.IsEmpty())
				{
					stShardData.saCode.Add(strCode);
					stShardData.saName.Add(strName);
					stShardData.iaMarkType.Add(nMarkType);
				}
			}
		}

		if (stShardData.saCode.GetSize() <= 0)
		{
			AfxMessageBox("������ �����Ͱ� �����ϴ�.");
			return;
		}

		
		stShardData.bOpen = MENU_RESULTCTRL_SCREEN_OPEN[nItem];
		
// 		if(bIsHTS)
// 			stShardData.strScreenNo = MENU_RESULTCTRL_SCREEN_NOMAX[nItem];
// 		else
// 			stShardData.strScreenNo = MENU_RESULTCTRL_SCREEN_NOEMP[nItem];

		stShardData.strScreenNo = MENU_RESULTCTRL_SCREEN_NOMAX[nItem];
		stShardData.strMapNo = MENU_RESULTCTRL_MAP_NO[nItem];
		
		ShardData *pstShardData = (ShardData*)&stShardData;
		SetSharedDataToSend(pstShardData);
	}
}

#define SCREEN_NO_ALLBUY				"NOT USER"
#define SCREEN_NO_ROLL					"NOT USER"
void CMainFrmWnd::SetSharedDataToSend(ShardData *pstShardData)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	CString strScreenNo(pstShardData->strScreenNo);

	if(pstShardData->saCode.GetSize() > 0)
	{
		pScreenWnd->OpenScreen(strScreenNo, &pstShardData->saCode, &pstShardData->saName);
	}
	else
	{
		pScreenWnd->OpenScreen(strScreenNo);
	}
}

// �˻��� ������ ����(excel �Ǵ� text)�� ����
void CMainFrmWnd::SetResultSave()
{
	CFileFind FileFinder;

	BOOL bExist = FALSE;
	BOOL bSave = TRUE;

	int nReturn = 0;
	CString strMsg = "";
	CString strFileName, strFileType;
	CString strDefaultName = "�ӽ�����";

	static char BASED_CODE szFilter[] = "WorkSheet Files (*.xls)|*.xls|EZ-Finder Data Files (*.txt)|*.txt||";
	CFileDialog saveDlg(FALSE, _T("xls"), strDefaultName + _T(".xls"), NULL, szFilter, NULL);
	
	while (bSave)
	{
		if (saveDlg.DoModal() == IDOK)
		{
			strFileType = saveDlg.GetFileExt();
			strFileName = saveDlg.GetPathName();
			
			bExist = FileFinder.FindFile(strFileName);
			if (bExist == TRUE)
			{
				strMsg.Format("%s ��(��) �̹� �ֽ��ϴ�.\n�ٲٽðڽ��ϱ�?", strFileName);
				nReturn = SetMessageBox(strMsg, PROGRAM_TITLE, MB_YESNO | MB_ICONQUESTION);
				if (nReturn == IDYES)
				{
					if (!strFileName.IsEmpty())
					{
						if (!strFileType.CompareNoCase("xls"))				
							SetSaveGridToExcel(strFileName);
						else if (!strFileType.CompareNoCase("txt"))				
							SetSaveGridToText(strFileName);
					}
					bSave = FALSE;
					break;
				}
				else
					bSave = TRUE;
			}
			else
			{
				if(!strFileName.IsEmpty())
				{
					if (!strFileType.CompareNoCase("xls"))				
						SetSaveGridToExcel(strFileName);
					else if (!strFileType.CompareNoCase("txt"))				
						SetSaveGridToText(strFileName); //txt file save 
				}
				bSave = FALSE;
				break;
			}
		}
		else
		{
			bSave = FALSE;
			break;
		}

		if (bSave == FALSE)
			break;
	}
}

// Excel ���Ϸ� ����
void CMainFrmWnd::SetSaveGridToExcel(CString strFileName)
{
	int nRow, nCol;
	CDrXLSFile xlsfile;
	
	CString strColName = "";
	CString strData = "";
			
	if (!xlsfile.OpenFile(strFileName))
	{
		CString strMsg = "������ ������ �ٸ� ���α׷����� ������Դϴ�";
		SetMessageBox(strMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}
	else
	{
		nRow = -1;
		for (nCol = 0; nCol < m_gridResult.GetNumberCols(); nCol++)
		{
			strData = m_gridResult.GetCellData(nRow, nCol);
			xlsfile.SetTextAlign(EXCEL_ALIGN_CENTER);
			xlsfile.WriteLabel(0, nCol, strData);
		}

		int nWriteRow=0;
		for (nRow = 0; nRow < m_gridResult.GetNumberRows(); nRow++)
		{
			//[20110825alzioyes]0000135: ���ǰ˻��������Ʈ��üũ�ڽ��ױ���߰�, �������.
			if(!m_gridResult.IsRowCodeChecked(nRow)) continue;

			for (nCol = 0; nCol < m_gridResult.GetNumberCols(); nCol++)
			{
				strData = m_gridResult.GetCellData(nRow, nCol);
				strData.Replace("&&", "&");

				strData.TrimRight(" ");
				strData.TrimLeft(" ");
						
				strColName.Format("%s", m_gridResult.GetCellData(-1, nCol));
				if ((strColName.Compare("�����") == 0) ||
					(strColName.Compare("���ϴ��") == 0))
				{
					COLORREF clrText, clrBack;
					m_gridResult.GetCellColor(nRow, nCol, clrText, clrBack);

					if (clrText == COLOR_BLUE)
						strData = "-" + strData;
					else if (clrText == COLOR_RED)
						strData = "+" + strData;
				}
        						
				//if (strColName.Compare("�����ڵ�") == 0)
				if (strColName.Find("�����ڵ�") >= 0)
					xlsfile.SetTextAlign(EXCEL_ALIGN_CENTER);
				else if (strColName.Find("�����") >= 0)
					xlsfile.SetTextAlign(EXCEL_ALIGN_LEFT);
				else
					xlsfile.SetTextAlign(EXCEL_ALIGN_RIGHT);
        
				xlsfile.WriteLabel(nWriteRow + 1, nCol, (LPCSTR)strData);
			}
			nWriteRow++;
		}
	}
	xlsfile.CloseFile();
}

//Text ���Ϸ� ����
void CMainFrmWnd::SetSaveGridToText(CString strFileName)
{
	CFile SaveFile(strFileName,CFile::modeCreate | CFile::modeWrite);
	
	int nRowCnt = m_gridResult.GetNumberRows();

	CString strLineData, strCell;
	int nRow, nCol;
	for (nRow = 0; nRow < nRowCnt; nRow++)
	{
		// Ÿ��Ʋ �����
		if (nRow == 0)
		{
			strCell = m_gridResult.GetCellData(0, 0);
			strLineData = strCell;	// �����
			strLineData += " ";
			
			for (nCol = 1; nCol < m_gridResult.GetNumberCols(); nCol++)
			{
				strCell = m_gridResult.GetCellData(0, nCol);
				strLineData += strCell;	// �� Item ����
				strLineData += " ";
			}
			
			strLineData += "\r\n";
			SaveFile.Write( strLineData, strLineData.GetLength() );			
		}

		//[20110825alzioyes]0000135: ���ǰ˻��������Ʈ��üũ�ڽ��ױ���߰�, �������.
		if(!m_gridResult.IsRowCodeChecked(nRow)) continue;

		strCell = m_gridResult.GetCellData(nRow, 0);
		strLineData = strCell;	// �����
		strLineData += " ";
		
		for (nCol = 1; nCol < m_gridResult.GetNumberCols();  nCol++)
		{
			strCell = m_gridResult.GetCellData(nRow, nCol);
			strLineData += strCell;	// �� Item ����
			strLineData += " ";
		}
		
		strLineData += "\r\n";
		SaveFile.Write(strLineData, strLineData.GetLength());
	}
	
	SaveFile.Close();
}

void CMainFrmWnd::SetSendMainMsg(CString strMsg)
{
	GetParent()->SendMessage(UMSG_DISP_MSG, (WPARAM)(LPCSTR)"", (LPARAM)(LPCSTR)strMsg);
}

// ����� �������� ����Ʈ �ۼ�
void CMainFrmWnd::MakeUserPatternList()
{
	CFileFind cFind;
	BOOL bRead = TRUE;

	CString strUserFile, strUserPatternPath;
	m_strUserPatternPath = GetFilePath("USERCONDFILE") + "\\finddata\\UserPtrn";

	BOOL bFind = FALSE;
	bRead = cFind.FindFile(m_strUserPatternPath);
	while(bRead)
	{
		bRead = cFind.FindNextFile();

		if(cFind.IsDots())  continue;
		if (cFind.IsDirectory())
		{
			bFind = TRUE;
			break;
		}
	}

	if(bFind == FALSE)
	{
		::CreateDirectory(m_strUserPatternPath, NULL);
	}

	// find user pattern files
	CString strFileFind = m_strUserPatternPath + "\\*.pdd";
	bRead = cFind.FindFile(strFileFind);
	if (!bRead)
	{
		CString strMsg = "����� ���� ������ ã�� ���߽��ϴ�.";
		SetStateTitleBar(strMsg);
	}
	else
	{
		while(bRead)
		{
			bRead = cFind.FindNextFile();

			if(cFind.IsDots())  continue;
			if (!cFind.IsDirectory())
			{
				strUserFile = cFind.GetFileName();
				// add filenme to array
				m_saUserPatternFiles.Add(strUserFile);
			}
		}
	}
}

// ���� ���� ���ϵ��� ����� �̹����� �����.
void CMainFrmWnd::MakeUserPatternImageList()
{
	CRect rcBorder(0, 0, THUMB_USER_WIDTH, THUMB_USER_HEIGHT);

	int nCount = m_saUserPatternFiles.GetSize();

	if( !m_ImageUserListItem.m_hImageList)
	{
		if ( !m_ImageUserListItem.Create(THUMB_USER_WIDTH, THUMB_USER_HEIGHT, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			SetMessageBox("Can not create ImageList.");			
			return;
		}
	}
	
	if(nCount == 0) return;

	CString strFileName;
	CClientDC cdc(this);
	CDC	memDC;
	memDC.CreateCompatibleDC(&cdc);

	for(int i = 0; i < nCount; i++)
	{
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(&cdc, THUMB_USER_WIDTH, THUMB_USER_HEIGHT);

		strFileName = m_saUserPatternFiles.GetAt(i);
		if(!MakePatternBitmap(&memDC, &bitmap, rcBorder, strFileName))
		{
			bitmap.DeleteObject();
			continue;
		}
		
		// Attach to Bitmap and Replace image in CImageList
		m_ImageUserListItem.Add(&bitmap, RGB(255,0,255));

		// Release Object
		bitmap.DeleteObject();
		memDC.SelectClipRgn(NULL);
	}

	//Release used DC
	memDC.DeleteDC();
}

// ���� ������ ����� ��Ʈ������ �����.
BOOL CMainFrmWnd::MakePatternBitmap(CDC* pDC, CBitmap* bitmap, CRect rcBorder, CString strPatternFilename)
{
	// open pattern data file
	CPtrArray arrDrawData;
	CRect rcCanvas;
	int nFileNameLen = strPatternFilename.GetLength();

	CString strFilePath = m_strUserPatternPath + "\\" + strPatternFilename;

	if(m_pctrlDrawPatten == NULL || ::IsWindow(m_pctrlDrawPatten->GetSafeHwnd()) == FALSE) return FALSE;

	if(m_pctrlDrawPatten->LoadPatternData4Thumbnail(strFilePath, rcCanvas, &arrDrawData) == FALSE)
		return FALSE;

	CBitmap *pOldBitmap;
	pOldBitmap = (CBitmap*)pDC->SelectObject(bitmap);

	// Draw Background
	pDC->FillSolidRect(rcBorder, RGB(255,255,255));

	if(!m_ImageBK.m_hImageList)
		pDC->Draw3dRect(&rcBorder, RGB(153,153,153), RGB(153,153,153));
	else
		m_ImageBK.Draw(pDC, 0, CPoint(0,0), ILD_NORMAL);

	// Draw Image
	DrawPattern(pDC, rcCanvas, &arrDrawData);

	CRect rcText(0,THUMB_USER_HEIGHT-16,THUMB_USER_WIDTH, THUMB_USER_HEIGHT);
	rcText.DeflateRect(1,1,1,1);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(153,153,153));//

	CFont* pOldFont = pDC->SelectObject(&m_fontDefault);
	pDC->DrawText(strPatternFilename.Left(nFileNameLen-4), rcText, DT_SINGLELINE|DT_END_ELLIPSIS|DT_CENTER);
	pDC->SelectObject(pOldFont);

	// Draw Border
	pDC->SelectObject(pOldBitmap);

	// delete memory
	Position *pptData;
	for (int nIndex = 0; nIndex < arrDrawData.GetSize(); nIndex++)
	{
		pptData = (Position*)arrDrawData.GetAt(nIndex);
		if (pptData != NULL)
		{
			delete pptData;
			pptData = NULL;
		}
	}

	arrDrawData.RemoveAll();
	arrDrawData.FreeExtra();

	return TRUE;
}

// ���� �迭 ������ dc�� �׸���.
void CMainFrmWnd::DrawPattern(CDC* pDC, CRect rcCanvas, CPtrArray *paData)
{
	if ((paData == NULL) || (paData->GetSize() == 0))
		return;

	Position *pstPosition, *pstPosition_Befor;

	// find max and min value
	int nIndex;
	float fMin = FLT_MAX, fMax = FLT_MIN;
	float fXMin = FLT_MAX, fXMax = FLT_MIN;
	for (nIndex = 0; nIndex < paData->GetSize(); nIndex++)
	{
		pstPosition = (Position*)paData->GetAt(nIndex);

		if(pstPosition->y > fMax) fMax = pstPosition->y;
		if(pstPosition->y < fMin) fMin = pstPosition->y;

		if(pstPosition->x > fXMax) fXMax = pstPosition->x;
		if(pstPosition->x < fXMin) fXMin = pstPosition->x;
	}

	rcCanvas.top = (int)fMin;
	rcCanvas.bottom = (int)fMax;
	rcCanvas.left = (int)fXMin;
	rcCanvas.right = (int)fXMax;

	CRect rcDrawArea(2, 3, THUMB_USER_WIDTH - 4, THUMB_USER_HEIGHT - 18);
	double dXScale = (double)(rcDrawArea.Width())/(double)rcCanvas.Width();
	double dYScale = (double)(rcDrawArea.Height())/(double)rcCanvas.Height();

	// adjust data for draw
	for (nIndex = 0; nIndex < paData->GetSize(); nIndex++)
	{
		pstPosition = (Position*)paData->GetAt(nIndex);

		if(pstPosition->y == 0.) continue;

		pstPosition->x -= fXMin;
		pstPosition->y -= fMin;

		pstPosition->x *= (float)dXScale;
		pstPosition->y *= (float)dYScale;
	}

	CPen penDraw(PS_SOLID, 2, RGB(220,194,130)), * pOldPen;
	pOldPen = (CPen*)pDC->SelectObject(&penDraw);
	
	BOOL bStart = TRUE;

	pDC->MoveTo((int)((Position*)paData->GetAt(0))->x + 2, (int)((Position*)paData->GetAt(0))->y + 3);
	for (nIndex = 1; nIndex < paData->GetSize(); nIndex++)
	{
		pstPosition = (Position*)paData->GetAt(nIndex);

		pDC->LineTo((int)pstPosition->x + 2, (int)pstPosition->y + 3);
		pDC->MoveTo((int)pstPosition->x + 2, (int)pstPosition->y + 3);

		pstPosition_Befor = pstPosition;
	}

	pDC->SelectObject(pOldPen);
}

// dc�� �׶���Ʈ ����� �׸���.
void CMainFrmWnd::DrawGradientRect(CDC *pDC, CRect rect, COLORREF Start, COLORREF End, BOOL bTopDown)
{
	CRect backup = rect;
		
	int r,g,b;//�� 3���� ������ ���� ����..
	float rStep, gStep, bStep;//�� 3���� ���� ������ ������ ����..
	float fStep;//�ִ� ���� ���̷� ����� �ѹ��� �̵��� �ɼ�..

	//�� ���� ���̸� ���Ѵ�..
	r = (GetRValue(End) - GetRValue(Start));
	g = (GetGValue(End) - GetGValue(Start));
	b = (GetBValue(End) - GetBValue(Start));

	//���� ū���̰� ���°��� ���Ѵ�..
	int Max = max( abs(r), max(abs(g), abs(b)) );

	//�׸� �簢���� ���̰� ���°� ��ŭ.. ������ ���̸� ���Ѵ�..
	if(bTopDown)
		fStep = (float)rect.Height()/(float)Max;     // ��-�� Gradation 
	else 
		fStep = (float)rect.Width()/(float)Max;     // ��-�� Gradation 

	//�� ���� �ܰ� ���̰�..
	rStep = r/(float)Max;
	gStep = g/(float)Max;
	bStep = b/(float)Max;

	//r,g,b�� ���� ������ �缳��...
	r = GetRValue(Start);
	g = GetGValue(Start);
	b = GetBValue(Start);

	//������ ���ؾ� �ϴ� Ƚ����ŭ.. �ݺ� �׸���.
	for(int iOnBand=0 ; iOnBand <Max; iOnBand++)  
	{
		if(bTopDown)
 			::SetRect( &rect, rect.left, int(fStep*iOnBand) + backup.top ,rect.right, backup.bottom );// ��-�� Gradation 
 		else     
			::SetRect( &rect, int(fStep*iOnBand) + backup.left, rect.top, rect.right, rect.bottom);//��-��
		pDC->FillSolidRect( &rect,  RGB( r + rStep*iOnBand, g + gStep*iOnBand, b + bStep*iOnBand ) );
	}
}

// �׸� �׷�/�ڵ� ������ ��ȸ�Ѵ�.
void CMainFrmWnd::SetTrData_ThemeGroup()
{
	CStringArray arrCode, arrName;
	CString strCode, strName;
	int nIndex;
	for(nIndex = 0; nIndex < arrCode.GetSize(); nIndex++)
	{
		strCode.Format("%4.4s", arrCode.GetAt(nIndex));
		strName.Format("%-36.36s",	arrName.GetAt(nIndex));
		m_saCodeTheme.Add(strCode + strName);	
	}
}

// ����� ĵ���� �̹�������Ʈ�� �����ĵ�鸮��Ʈ()���� ����
LONG CMainFrmWnd::OnDeleteUserItem(WPARAM wParam, LPARAM lParam)
{
	int nIndex = (int)wParam;

	m_ctrlUserListItem.m_ImageList.Remove(nIndex);
	m_ctrlUserListItem.Refresh();

	CString strFileName = m_saUserPatternFiles.GetAt(nIndex);
	CString strFilePath = m_strUserPatternPath + "\\" + strFileName;

	if(! strFileName.IsEmpty() && strFileName.GetLength() >= 5 && strFileName.Right(3) == "pdd")
	{
		BOOL bResult = ::DeleteFile(strFilePath);
		if(!bResult)
		{
			DWORD nError = GetLastError();
			CString strMsg;
			strMsg.Format("%s ���������� �������� ���߽��ϴ�.", strFileName);
			SetMessageBox(strMsg, PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
		}
		m_saUserPatternFiles.RemoveAt(nIndex);
	}
	
	return 0;
}

// �̹��� ����Ʈ��Ʈ�ѿ��� ���� �ؽ�Ʈ ��û �޽��� ó��
LONG CMainFrmWnd::OnGetTipText(WPARAM wParam, LPARAM lParam)
{
	int nImageIndex = HIWORD(wParam);
	int nID = LOWORD(wParam);
	char* szTipText = (char*)lParam;
	memset(szTipText, 0x00, 256);

	CString strTipText;
	if(nID == IDC_ITEM_LIST)
	{
		strTipText = m_saPatternItemTip.GetAt(nImageIndex);
		int nSize = min(255, strTipText.GetLength());
		sprintf(szTipText, "%*.*s", nSize, nSize, strTipText);
		return 1;
	}
	else if(nID == IDC_ITEM_USERLIST)
	{
		strTipText = m_saUserPatternFiles.GetAt(nImageIndex);
		strTipText.Replace(".pdd", "");
		int nSize = min(255, strTipText.GetLength());
		sprintf(szTipText, "%*.*s", nSize, nSize, strTipText);
		return 1;
	}

	return 0;
}

void CMainFrmWnd::GetSimilarityFromGrid()
{
	long nRow = m_gridResult.GetCurrentRow();
	int nCntCol = m_gridResult.GetNumberCols();
	int nSrcCol = -1;

	CString strTitle;

	for(int i = 0; i < nCntCol; i++)
	{
		strTitle = m_gridResult.GetCellData(-1, i);
		if(strTitle == "���絵")
		{ nSrcCol = i; break; }

	}

	if(nSrcCol != -1)
		m_strItemRight.Format("(���絵 %s)", m_gridResult.GetCellData(nRow, nSrcCol));
}


void CMainFrmWnd::OnEdChangeScaleCnt()
{
	CString strVal;
	m_editPeriod.GetWindowText(strVal);

	int nGap = atoi(strVal);

	if(nGap > MAX_SLIDER)
	{
		nGap = (int)MAX_SLIDER;
	}
	else if(nGap < MIN_SLIDER)
	{
		if(strVal.GetLength() < 2) return;
		nGap = (int)MIN_SLIDER;
	}

	if(m_nPrevScale == nGap) return;

	strVal.Format("%d", nGap);
	//m_editPeriod.SetWindowText(strVal);

	m_editPeriod.SetSel(strVal.GetLength(), -1);

	if(m_nPrevScale != nGap)
	{
		m_ctrlSlider.SetPos(nGap);
		if(m_pctrlDrawPatten && ::IsWindow(m_pctrlDrawPatten->GetSafeHwnd()))
			m_pctrlDrawPatten->SetData_XGridCnt(m_ctrlSlider.GetPos());

		m_nPrevScale = nGap;
	}
}

void CMainFrmWnd::GetTargetBaseType(int &nTargetBaseType_0st, int &nTargetBaseType_1st)
{
	if (m_strTargetBaseType.GetLength() == 0) 
	{
		nTargetBaseType_0st = -1;
		nTargetBaseType_1st = -1;
	}
	else
	{
		// 0:��ü, 1:�ڽ���, 2:�ڽ���, 3:����
		nTargetBaseType_0st = atoi(m_strTargetBaseType.Left(1));
		
		if (m_strTargetBaseType.GetLength() == 2)
			nTargetBaseType_1st = atoi(m_strTargetBaseType.Right(1));
		else
			nTargetBaseType_1st = -1;
	}
}

CString CMainFrmWnd::SetTargetBaseType(int nTargetBaseType_0st, int nTargetBaseType_1st)
{
	m_strTargetBaseType.Format("%d%d", nTargetBaseType_0st, nTargetBaseType_1st);
	
	if (nTargetBaseType_1st == -1)
		m_saCodeGroup.RemoveAll();

	return m_strTargetBaseType;
}

void CMainFrmWnd::OnChgComboTarget1st()
{
	int nTargetBaseType_0st, nTargetBaseType_1st;
	GetTargetBaseType(nTargetBaseType_0st, nTargetBaseType_1st);
	
	int nIndex = m_comboTarget_1st.GetCurSel();

	if (nIndex == 0) // ��ü
	{
		m_btnReverseChk.EnableWindow(FALSE);
		m_btnReverseChk.SetCheck(FALSE);

		m_comboTarget_2nd.ResetContent();
		m_comboTarget_2nd.EnableWindow(FALSE);
		SetTargetBaseType(0);

		m_btnReverseChk.EnableWindow(FALSE);
		m_btnReverseChk.SetCheck(FALSE);
	}
	else
	{
		m_btnReverseChk.EnableWindow(TRUE);
		
		m_comboTarget_2nd.EnableWindow(TRUE);

		CString strTargetBaseType = SetTargetBaseType(nIndex, 0);

		if (SetLoad_TargetBase_2nd(strTargetBaseType) == TRUE)
			OnChgComboTarget2nd();
	}
}

void CMainFrmWnd::OnChgComboTarget2nd()
{
	CString strCode, strName;
	int nTargetBase_2nd = m_comboTarget_2nd.GetCurSel();
	
	if(nTargetBase_2nd < 0)
		return;
	
	DWORD lCode;
	lCode = m_comboTarget_2nd.GetItemData(nTargetBase_2nd);

	m_comboTarget_2nd.GetLBText(m_comboTarget_2nd.GetCurSel(), strName);
	
	int nTargetBaseType_0st, nTargetBaseType_1st;
	GetTargetBaseType(nTargetBaseType_0st, nTargetBaseType_1st);
	

	if (nTargetBaseType_0st == 0)		//# ��ü
	{
		m_comboTarget_2nd.ResetContent();
		m_comboTarget_2nd.EnableWindow(FALSE);
	}
	else if (nTargetBaseType_0st == 1)	//# KOSPI
	{
		m_nMarket = GETMARKET_KOSPI;
		//strCode.Format("%04ld", lCode);
		GetData_UpJongToCode(lCode, strName);
	}
	else if (nTargetBaseType_0st == 2)	//# KOSDAQ
	{
		m_nMarket = GETMARKET_KOSDAQ;
		//strCode.Format("%04ld", lCode);
		GetData_UpJongToCode(lCode, strName);
	}
// 	else if (nTargetBaseType_0st == 3)	//# �׸�
// 	{
// 		strCode.Format("%04ld", lCode);
// 		GetData_ThemaToCode(strCode);
// 	}
	else if (nTargetBaseType_0st == 3)	//# ����
	{
		strCode.Format("%02ld", lCode);
		GetData_PortGroupToCode(strCode, &m_saCodeGroup);
	}
	
}


void CMainFrmWnd::OnBtnDay()
{
	m_strChartTerm = "2";
	m_comboTime.EnableWindow(FALSE);
	m_comboTime.Invalidate();

	m_btnDay.SetCheck(TRUE);
	m_btnWeek.SetCheck(FALSE);
	m_btnMonth.SetCheck(FALSE);
	m_btnMin.SetCheck(FALSE);

	OnBtnSearch();
}

void CMainFrmWnd::OnBtnWeek()
{
	m_strChartTerm = "3";
	m_comboTime.EnableWindow(FALSE);
	m_comboTime.Invalidate();

	m_btnDay.SetCheck(FALSE);
	m_btnWeek.SetCheck(TRUE);
	m_btnMonth.SetCheck(FALSE);
	m_btnMin.SetCheck(FALSE);

	OnBtnSearch();
}

void CMainFrmWnd::OnBtnMon()
{
	m_strChartTerm = "4";
	m_comboTime.EnableWindow(FALSE);
	m_comboTime.Invalidate();

	m_btnDay.SetCheck(FALSE);
	m_btnWeek.SetCheck(FALSE);
	m_btnMonth.SetCheck(TRUE);
	m_btnMin.SetCheck(FALSE);

	OnBtnSearch();
}

void CMainFrmWnd::OnBtnMin()
{
	m_strChartTerm = "1";
	m_comboTime.EnableWindow(TRUE);
	m_comboTime.Invalidate();

	m_btnDay.SetCheck(FALSE);
	m_btnWeek.SetCheck(FALSE);
	m_btnMonth.SetCheck(FALSE);
	m_btnMin.SetCheck(TRUE);

	OnBtnSearch();
}

void CMainFrmWnd::OnBtnSave()
{
	if(m_pctrlDrawPatten->HaveSaveData() == FALSE) return;

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	CDlgSavePattern dlg(this);

	if(dlg.DoModal() == IDOK)
	{
		CString strFile, strPatternName = dlg.m_strPatternName + ".pdd";
		BOOL bSameNameFind = FALSE;
		int nImageIndex = -1;

		for(int i = 0; i < m_saUserPatternFiles.GetSize(); i++)
		{
			strFile = m_saUserPatternFiles.GetAt(i);

			if(strPatternName == strFile)
			{
				bSameNameFind = TRUE;
				nImageIndex = i;
				break;
			}
		}

		int nRes = IDYES;
		if(bSameNameFind)
		{
			nRes = SetMessageBox("���� ���� �̸��� �����մϴ�. \n\n��� ���� �Ͻðڽ��ϱ�?", PROGRAM_TITLE, MB_YESNO);
		}

		if(nRes == IDYES)
		{
			if(!bSameNameFind)
				m_saUserPatternFiles.Add(strPatternName);
			
			CString strFilePath = m_strUserPatternPath + "\\" + strPatternName;
			m_pctrlDrawPatten->SetSave_DrawData(strFilePath); // �����ƾ ȣ��

			// Make bitmap
			CRect rcBorder(0, 0, THUMB_USER_WIDTH, THUMB_USER_HEIGHT);

			CClientDC cdc(this);
			CDC	memDC;
			memDC.CreateCompatibleDC(&cdc);

			CBitmap bitmap;
			bitmap.CreateCompatibleBitmap(&cdc, THUMB_USER_WIDTH, THUMB_USER_HEIGHT);

			if( MakePatternBitmap(&memDC, &bitmap, rcBorder, strPatternName) )
			{
				// add bitmap to imagelist
				if(!bSameNameFind)
					m_ctrlUserListItem.m_ImageList.Add(&bitmap, RGB(255,0,255));
				else
					m_ctrlUserListItem.m_ImageList.Replace(nImageIndex, &bitmap, NULL);

				memDC.SelectClipRgn(NULL);

				// refresh list window
				m_ctrlUserListItem.Refresh();

				m_ptabCondition->SetCurSel(1);
				m_ctrlListItem.ShowWindow(SW_HIDE);

				m_ctrlUserListItem.ShowWindow(SW_SHOW);
				m_ctrlUserListItem.BringWindowToTop();
				m_ctrlUserListItem.Invalidate();
			}

			bitmap.DeleteObject();

			// refresh pattern title
			m_strCaption = "�߼����� �׸���";
			CRect rcText = m_reTopPattern;
			rcText.DeflateRect(5, 2);
			InvalidateRect(&rcText);
		}
	}
	
	AfxSetResourceHandle(hInstSave);
}

void CMainFrmWnd::OnBtnLoad()
{
	TCHAR szFilters[] = _T("Patten Draw Data Files (*.pdd)|*.pdd||");
	
	CFileDialog FileDlg (TRUE, _T("pdd"), _T("*.pdd"),   OFN_FILEMUSTEXIST, szFilters);
	if (FileDlg.DoModal() == IDOK)
	{
		CString strFilePath;
		strFilePath = FileDlg.GetPathName();
		m_pctrlDrawPatten->GetLoad_DrawData(strFilePath);
		
		OnBtnSearch();
	}
}

void CMainFrmWnd::OnBtnReset()
{
	if (m_pctrlDrawPatten != NULL)
		m_pctrlDrawPatten->SetInit();

	// refresh pattern title
	m_strCaption = "�߼����� �׸���";
	CRect rcText = m_reTopPattern;
	rcText.DeflateRect(5, 2);
	InvalidateRect(&rcText);
}

// �˻� ����
void CMainFrmWnd::OnBtnSearch()
{
	DRAW_IN	stDrawIn;
	
	if (SetMakePacket(stDrawIn) == TRUE)
	{
//		SetBtyeOrdering_Packet(stDrawIn);

		p0624InBlock* pst0624;
		pst0624 = new p0624InBlock;
		SetConvert_SendPatternPacket(stDrawIn, pst0624);
 
// 		char szData[SZ_DRAW_IN + 1];
// 		memset(szData, 0x00, sizeof(szData));
// 		memcpy((void *)szData, (void *)&stDrawIn, SZ_DRAW_IN);
		
	//	m_WaitCursor.BWaitCursorIntelli();
	
		Module_SendTr(TRNO_DRAW_RESULT, SZ_p0624InBlock, (BYTE*)pst0624);
// 		m_pTRComm->SetDestination('V');
// 		m_pTRComm->SetTR(TRNO_DRAW_RESULT);
// 		m_pTRComm->SetBaseData("SCREENCODE", m_strScreenNo);
// 		m_pTRComm->Send2Server(szData, SZ_DRAW_IN, FALSE);
		
		delete pst0624;
		SetStateTitleBar("[��ȸ��]");

		SetEnable(FALSE);
	}

	return;
}

long CMainFrmWnd::GetSearchTimeTerm()
{
	long nResult = 0;
	if (atoi(m_strChartTerm) == 0)		//# ƽ
	{
		
	}
	else if (atoi(m_strChartTerm) == 1)  //# ��
	{
		//   30��", "1��", "3��","5��","10��","30��" ,"60��"};
		//# 3:1��, 4:3��, 5:5��, 6:10��, 7:15��, 8:30��, 9:60��
		nResult = LIST_TIME_INDEX[m_comboTime.GetCurSel()];
	}
	else if (atoi(m_strChartTerm) == 2) //# ��
		nResult = 0;
	else if (atoi(m_strChartTerm) == 3)	//# ��
		nResult = 1;
	else if (atoi(m_strChartTerm) == 4)	//# ��
		nResult = 2;
	else if (atoi(m_strChartTerm) == 5)	//# ��
		nResult = -1;

	return nResult;
}

BOOL CMainFrmWnd::SetMakePacket(DRAW_IN	&stDrawIn)
{
	BOOL bResult = FALSE;

	m_nTrIndex++;

	stDrawIn.nTrIndex = m_nTrIndex;

	stDrawIn.nS_Time = GetSearchTimeTerm();
	stDrawIn.lTarget = 4;           // ������� => 4 ����
	stDrawIn.lMoveAvg = 1;          // ����Ⱓ => 1����
	stDrawIn.lS_Pos = 0;            // �˻� ������ġ

	if (m_btnBongChk.GetCheck() == TRUE)
		stDrawIn.lS_Range = LIST_BONGTERM[m_comboBongTerm.GetCurSel()];        // �˻� ����
	else
		stDrawIn.lS_Range = m_ctrlSlider.GetPos();

	//# ���絵
	stDrawIn.lSimilar = LIST_SIMILAR[m_comboSimilar.GetCurSel()];

	stDrawIn.nFieldEditerCnt = m_listField.GetCount();

	POSITION pos;
	int nIndex;
	HeadInfoType		stHeadInfoType;
	for (nIndex = 0; nIndex < m_listField.GetCount(); nIndex++)
	{
		pos = m_listField.FindIndex(nIndex);
		stHeadInfoType = m_listField.GetAt(pos);

		stDrawIn.nFieldList[nIndex] = stHeadInfoType.lHeadID;
	}

	//# �ֱ� ����ġ ���뿩��
	//  : 1~ 10, ������� 0����.. (0.005)
	stDrawIn.stDrawData.lAccuVal = 2;
	
	//# �׸��� �Ⱓ����(20~120)
	stDrawIn.stDrawData.lDraw_Cnt = m_ctrlSlider.GetPos();    

	//# ������
	stDrawIn.stDrawData.lErrRate = 10;

	//# ���� ����
	stDrawIn.stDrawData.lGapRate = 40;
	
	long *pDrawData;
	pDrawData = new long[(int)CNT_DRAW_VAL];
	memset(pDrawData, 0x00, sizeof(long) * (int)CNT_DRAW_VAL);
	if (m_pctrlDrawPatten->GetData_DrawPacket(pDrawData) == 0)
	{
		SetStateTitleBar("[�˻��� ������ �������� �ʽ��ϴ�.]");
		delete [] pDrawData;
		return FALSE;
	}
	//# Ⱦ��üũ
	stDrawIn.cBoxFlag = (m_pctrlDrawPatten->GetCheck_Sidle() == TRUE) ? '1' : '0';

	stDrawIn.stDrawData.lDraw_Cnt = m_pctrlDrawPatten->GetData_XGridCnt();
	//2009.12.01 by LYH >> 60�� ������� ���̵��� ����
	//m_strChartCnt.Format("%d", stDrawIn.stDrawData.lDraw_Cnt + (stDrawIn.stDrawData.lDraw_Cnt / 4));
	m_strChartCnt.Format("%d", stDrawIn.stDrawData.lDraw_Cnt + (stDrawIn.stDrawData.lDraw_Cnt / 4) + 60);
	//2009.12.01 by LYH <<

	for (nIndex = 0; nIndex < stDrawIn.stDrawData.lDraw_Cnt; nIndex++)
		stDrawIn.stDrawData.lDraw_Value[stDrawIn.stDrawData.lDraw_Cnt - (nIndex + 1)] = pDrawData[nIndex];
	delete [] pDrawData;

	for (nIndex = stDrawIn.stDrawData.lDraw_Cnt; nIndex < (int)CNT_DRAW_VAL; nIndex++)
		stDrawIn.stDrawData.lDraw_Value[nIndex] = -1;

	int nTargetBaseType_0st, nTargetBaseType_1st;
	GetTargetBaseType(nTargetBaseType_0st, nTargetBaseType_1st);

	BOOL bReverse = FALSE;
	if (m_btnReverseChk.IsWindowEnabled() == TRUE)
		bReverse = m_btnReverseChk.GetCheck();

	CString strCode, strCodeCompare;

	if (nTargetBaseType_0st == 0) // ��ü
	{
		memset(&stDrawIn.sTargetBit, 0x31, sizeof(stDrawIn.sTargetBit));
	}
	else if (nTargetBaseType_0st == 1 || nTargetBaseType_0st == 2) // �ڽ���, �ڽ���
	{
		if (bReverse == TRUE)
			memset(&stDrawIn.sTargetBit, 0x31, sizeof(stDrawIn.sTargetBit));
		else
			memset(&stDrawIn.sTargetBit, 0x30, sizeof(stDrawIn.sTargetBit));

		for (int nIndex_Code = 0; nIndex_Code < m_saCodeGroup.GetSize(); nIndex_Code++)
		{
			strCodeCompare = m_saCodeGroup.GetAt(nIndex_Code);
			for (nIndex = 0; nIndex < m_saCodeMST.GetSize(); nIndex++)
			{
				strCode = m_saCodeMST.GetAt(nIndex);
				if (strCodeCompare == strCode.Mid(1, 6))		//# Kospi
				{
					stDrawIn.sTargetBit[nIndex + (int)RESULT_MAX_OFFSET] = (m_btnReverseChk.GetCheck() == TRUE) ? '0':'1';
					break;
				}
			}
		}
	}
	else if (nTargetBaseType_0st == 3 || nTargetBaseType_0st == 4)// �׸�, ����
	{
		if (bReverse == TRUE)
			memset(&stDrawIn.sTargetBit, 0x31, sizeof(stDrawIn.sTargetBit));
		else
			memset(&stDrawIn.sTargetBit, 0x30, sizeof(stDrawIn.sTargetBit));

		for (int nIndex_Code = 0; nIndex_Code < m_saCodeGroup.GetSize(); nIndex_Code++)
		{
			strCodeCompare = m_saCodeGroup.GetAt(nIndex_Code);
			for (nIndex = 0; nIndex < m_saCodeMST.GetSize(); nIndex++)
			{
				strCode = m_saCodeMST.GetAt(nIndex);
				if (strCodeCompare == strCode.Mid(1, 6))
				{
					stDrawIn.sTargetBit[nIndex + (int)RESULT_MAX_OFFSET] = (m_btnReverseChk.GetCheck() == TRUE) ? '0':'1';
					break;
				}
			}
		}
	}

	return TRUE;
}

// ����Ʈ ������
void CMainFrmWnd::SetBtyeOrdering_Packet(DRAW_IN& stDrawIn)
{
	stDrawIn.nTrIndex = ntohl(stDrawIn.nTrIndex);
    
	stDrawIn.nS_Time = ntohl(stDrawIn.nS_Time);
	stDrawIn.lTarget = ntohl(stDrawIn.lTarget);           // ������� => 4 ����
	stDrawIn.lMoveAvg = ntohl(stDrawIn.lMoveAvg);          // ����Ⱓ => 1����
	stDrawIn.lS_Pos = ntohl(stDrawIn.lS_Pos);            // �˻� ������ġ
	stDrawIn.lS_Range = ntohl(stDrawIn.lS_Range);          // �˻� ����
	stDrawIn.lSimilar = ntohl(stDrawIn.lSimilar);          // ���絵
	
	int nIndex;
	for (nIndex = 0; nIndex < stDrawIn.nFieldEditerCnt; nIndex++)
		stDrawIn.nFieldList[nIndex] = ntohl(stDrawIn.nFieldList[nIndex]);
	stDrawIn.nFieldEditerCnt = ntohl(stDrawIn.nFieldEditerCnt);								// �ʵ����� ����   [Fix]
	
	stDrawIn.stDrawData.lAccuVal = ntohl(stDrawIn.stDrawData.lAccuVal);       // �ֱ� ����ġ ���뿩��
	//  : 1~ 10, ������� 0����.. (0.005)
	stDrawIn.stDrawData.lDraw_Cnt = ntohl(stDrawIn.stDrawData.lDraw_Cnt);    // �׸��� �Ⱓ����(20~120)
	stDrawIn.stDrawData.lErrRate = ntohl(stDrawIn.stDrawData.lErrRate);       // ������
	stDrawIn.stDrawData.lGapRate = ntohl(stDrawIn.stDrawData.lGapRate);        // ���� ����
	
	for (nIndex = 0; nIndex < (int)CNT_DRAW_VAL; nIndex++)
		stDrawIn.stDrawData.lDraw_Value[nIndex] = ntohl(stDrawIn.stDrawData.lDraw_Value[nIndex]);
}

void CMainFrmWnd::OnBtnChkBong()
{
	m_comboBongTerm.EnableWindow(m_btnBongChk.GetCheck());
	m_comboBongTerm.Invalidate();
}

void CMainFrmWnd::OnChgBongCombo()
{
	
}

void CMainFrmWnd::OnChgSimCombo()
{
	
}

LRESULT CMainFrmWnd::OnNotify_Grid(WPARAM wParam, LPARAM lParam)
{
	CString strData;
	if (wParam == NM_GRID_CLK)
	{
		strData = *(CString*)lParam;		
		
		if (strData.IsEmpty() == FALSE)
		{
			int nPos_Find;
			nPos_Find = strData.Find("/");
			
			CString strCode_Select;
			strCode_Select = strData.Left(nPos_Find);
			m_strDateTime_Select = strData.Right(strData.GetLength() - (nPos_Find + 1));	
			
			if(strCode_Select.GetLength()>0)
			{
				SendTr_ChartData(strCode_Select, m_strDateTime_Select);
	
				if(m_bLink)
					SetSharedDataToCode(strCode_Select);
				else
					m_bLink = TRUE;
			}
		}	
		
	}
	else if (wParam == NM_DBCLICK_L)
	{
		CString strCode = *(CString*)lParam;

		if (strCode.IsEmpty() == FALSE)
		{
			
// 			CStdDialog  *pdlgStd = (CStdDialog*)GetParent();
// 			//pdlgStd->GetParent()->SendMessage(UMSG_LINK_CODE, 0, (LPARAM)(LPCSTR)strCode);
// 			CPatternSearchView* pView = (CPatternSearchView*)pdlgStd->GetParent();
// 			pView->OnDBClick(strCode);
			
		}
	}
	return 1L;
}

LRESULT CMainFrmWnd::OnNotify_DrawPatten(WPARAM wParam, LPARAM lParam)
{
	CString strData;
	if (wParam == NM_LBUTTONUP)
	{
		OnBtnSearch();
	}
	else if (wParam == NM_LBUTTONDOWN)
	{
		m_strCaption = "�߼����� �׸���";
		CRect rcText = m_reTopPattern;
		rcText.DeflateRect(5, 2);
		InvalidateRect(&rcText);
	}
	return 1L;
}

void CMainFrmWnd::ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor)
{
	if(!clrBkgnd)
		return;

	m_clrMainSkin = clrBkgnd;

	m_clrBkgnd = m_clrMainSkin;
	m_stTabColor = stTabColor;

	if(m_brushBK.DeleteObject())
		m_brushBK.CreateSolidBrush(m_clrMainSkin);

	m_ptabCondition->SetTabColor(m_clrMainSkin, m_clrMainSkin, stTabColor.clrB, NULL, NULL, m_clrMainSkin);
	m_ptabCondition->SetTextColor(stTabColor.clrSF, stTabColor.clrSF);
	m_ptabCondition->Invalidate();

	ST_GRIDCOLOR stGridColor;
	stGridColor.clrGridHeader = GetAxColor(74);
	stGridColor.clrGridHilight = GetAxColor(78);
	
	ChangeGridColor(stGridColor);

	m_ctrlTarget.Invalidate();
	m_ctrlCodeCntText.Invalidate();
	m_ctrlCodeCnt.Invalidate();
	m_ctrlJongName.Invalidate();
	m_ctrlPeriod.Invalidate();
	m_ctrlSimilarity.Invalidate();
	m_ctrlSimilar.Invalidate();

	
	m_btnExceptionChk.Invalidate();
	m_btnReverseChk.Invalidate();
	m_btnBongChk.Invalidate();
	
	m_comboTarget_2nd.Invalidate();
	m_comboTime.Invalidate();
	m_comboBongTerm.Invalidate();
	
	m_stcStatusBar.Invalidate();
	m_ctrlTime.Invalidate();
	m_ctrlNotice.Invalidate();
	m_ctrlState.Invalidate();

	::SendMessage(m_ctrlSlider.GetSafeHwnd(), PBM_SETBARCOLOR, 0, m_clrBkgnd);

	Invalidate();	
}

void CMainFrmWnd::ChangeGridColor(ST_GRIDCOLOR stGridColor)
{
	m_gridResult.ChangeGridColor(stGridColor);
	m_gridResult.Invalidate();
}

int CMainFrmWnd::Module_SendTr(CString szTrNo, int nTrLen, BYTE *TrData)
{
	CString strMsg;
	char szSaveBuf[8192] = { 0 };
	int nPos = 0;

	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	if(szTrNo.CompareNoCase(TRNO_DRAW_RESULT)==0)
	{	
		pScreenWnd->SendTR(TRNO_DRAW_RESULT, (char*)TrData, nTrLen, TRKEY_DRAW_RESULT, US_KEY);
		m_gridResult.ClearAll();
		
 	}
	if(szTrNo.CompareNoCase(TRNO_MSTCODE)==0)
	{
		if(CheckSavedMstcode()) return 1;
		
		InMakeFid_Mstcode(szTrNo, nTrLen, TrData, szSaveBuf, nPos);
		pScreenWnd->SendTR(TRNO_MSTCODE, (char*)TrData, nTrLen, TRKEY_MSTCODE, US_KEY);
	}
//���� �����ͷ� ����.
// 	else if(szTrNo.CompareNoCase(TRNO_THEMEGROUP)==0)
// 	{
// 		//		InMakeFid_Packet(nTrLen, TrData, szSaveBuf, nPos, 2, 2);
// 
// 		m_pTRComm->SetTR(szTrNo);
// 		m_pTRComm->Send2Server(TrData, nTrLen);		
// 	}
// 	else if(szTrNo.CompareNoCase(TRNO_THEMEITEM)==0)
// 	{
// 		//		InMakeFid_Packet(nTrLen, TrData, szSaveBuf, nPos, 2, 2);
// 
// 		m_pTRComm->SetTR(szTrNo);
// 		m_pTRComm->Send2Server(TrData, nTrLen);
// 	}
	if(szTrNo.CompareNoCase(QUERY_STRJONGMOK)==0)
	{			
		
// 		CString strTrInfo;
// 		strTrInfo.Format("%s", TrData);
// 
// 		CString stInput;
// 		stInput = strTrInfo;
// 
// 		if (m_pPartnerMng)
// 		{
// 			CString szReqTr = szTrNo;			
// 			return m_pPartnerMng->SendConvData((LPCTSTR)szReqTr, (LPBYTE)(LPCTSTR)stInput, stInput.GetLength());
// 		}		
		
			
	}

//	GetParent()->GetParent()->SendMessage(RMSG_WAIT_CORSOR, (WPARAM)TRUE, 0);
	return 1;
}

BOOL CMainFrmWnd::CheckSavedMstcode()
{
	
// 	long  rValue = NULL;
// 	int   rLen = 0;
// 	IMainExInfoManager3* pMainExMng = (IMainExInfoManager3*)AfxGetPctrInterface(UUID_IMainExInfoManager3, 1);
// 	if(pMainExMng)
// 	{
// 		BOOL bSaved = (BOOL)pMainExMng->GetUserData("MSTCODE", rValue, rLen);
// 		if(!bSaved) return FALSE;
// 
// 		LPCSTR szTR = TRNO_MSTCODE;
// 		LPCSTR szMsgCode="00000";
// 		LPCSTR szMessage="��ȸ�Ϸ�";
// 
// 		ConvertMstData(szTR, (BYTE*)rValue, rLen);
// 		//ReceiveData(1, szTR, szMsgCode, szMessage, (BYTE*)rValue, rLen);
// 		return TRUE;
// 	}
	
	return FALSE;
}

void CMainFrmWnd::ConvertMstData(LPCSTR szTR, LPVOID	aTRData, int dwTRDataLen)
{
	m_saCodeMST.RemoveAll();

	CString strMarket, strData;

	char *pBuff;

	//	MSTCode** pstMSTCode = (MSTCode**)aTRData;
	MSTCode stMSTCode;
	char* pstData = (char*)aTRData;

	int nDataCnt = dwTRDataLen / sizeof(MSTCode);
	CString strCode;
	int nPos = 0;

	for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
	{
		memset(&stMSTCode, 0x00, sizeof(MSTCode));	
		memcpy(&stMSTCode, &pstData[nPos], sizeof(MSTCode));
		nPos += sizeof(MSTCode);

		STR_CONVERT(pBuff, stMSTCode.sMarket, sizeof(stMSTCode.sMarket), strData);
		strMarket = strData;

		STR_CONVERT(pBuff, stMSTCode.sCode, sizeof(stMSTCode.sCode), strData);
		strData.TrimLeft(" ");
		strData.TrimRight(" ");

		strCode.Format("%1.1s%-6.6s", strMarket, strData);
		m_saCodeMST.Add(strCode);

	//	m_mapMarketCode[strData] = strMarket;
	}
	int size = m_saCodeMST.GetSize();
}

void CMainFrmWnd::SetConvert_ReceiveThemeItem(CString strData, int nItemCount)
{
	m_saCodeGroup.RemoveAll();

	long dwDataLen = strData.GetLength();

	char* lpData = new char[dwDataLen + 1];
	memset(lpData, 0x20, sizeof(lpData));
	memcpy(lpData, (LPCTSTR)strData, dwDataLen);

	p0630OutBlock2 st0630Out2;

	CString strCode;
	int strMarketType = 0;
	char *pBuff;
	for(int nIndex = 0; nIndex < nItemCount; nIndex++)
	{
		memset(&st0630Out2, 0x00, SZ_p0630OutBlock2);
		//		memcpy(&st0630Out2, lpData, SZ_p0630OutBlock2);
		memcpy(&st0630Out2, lpData + (nIndex * SZ_p0630OutBlock2) , SZ_p0630OutBlock2);

		STR_CONVERT(pBuff, st0630Out2.gsItemCode, sizeof(st0630Out2.gsItemCode), strCode);
		strMarketType = atoi(st0630Out2.gsMarketFlag);	

		if(strMarketType == 1 && m_nMarket == 1 || m_nMarket == 0)
			m_saCodeGroup.Add(strCode);
		else if(strMarketType == 2 && m_nMarket == 2 || m_nMarket == 0)
			m_saCodeGroup.Add(strCode);		

		//		lpData = &lpData[SZ_p0630OutBlock2];
		dwDataLen = dwDataLen - SZ_p0630OutBlock2;
	}

	delete [] lpData;
}

LRESULT CMainFrmWnd::OnReceiveTranData(WPARAM wParam, LPARAM lParam)
{
	
// 	CString strData = (LPCSTR)wParam;
// 	int nBuffSize = m_pPartnerMng->GetRecvLen(strData);
// 	if (nBuffSize <= 0)
// 		return 0L;
// 
// 	char *pszRecvData = new char[nBuffSize+1];
// 	memset(pszRecvData, 0x00, nBuffSize+1);
// 
// 	if (m_pPartnerMng->RecvConvData((LPCSTR)wParam, (LPBYTE)pszRecvData, nBuffSize))
// 		ReceiveData(0, (LPCSTR)wParam, "", "", (LPVOID)pszRecvData, (long)nBuffSize);
// 
// 	delete []pszRecvData;
	

	return 0L;
}

LRESULT CMainFrmWnd::OnReceiveDirectFID(WPARAM wParam, LPARAM lParam)
{
	
// 	LPITGBASE_HEADER lpITGBaseHead =(LPITGBASE_HEADER)wParam;
// 	LPITGUSER_HEADER lpITGUserHead =(LPITGUSER_HEADER)lParam;
// 
// 	CString strTR;
// 	strTR.Format(_T("%.*s"), sizeof(lpITGBaseHead->cTrcode), lpITGBaseHead->cTrcode);
// 
// 	if (!m_pPartnerMng)
// 		return 0L;
// 
// 	int nBuffSize = lpITGUserHead->nDataSize;
// 	LPVOID lpData = (LPVOID)lpITGUserHead->pData;
// 
// 	char *pszRecvData = NULL;
// 	long lSize = m_pPartnerMng->Receivedata(strTR, lpData, nBuffSize, &pszRecvData,	lpITGBaseHead->nRqid);
// 
// 	if (lSize > 0)
// 		ReceiveData(0, (LPCSTR)strTR, "", "", (LPVOID)pszRecvData, lSize);
// 
// 	if (pszRecvData)
// 		delete[] pszRecvData;
	

	return 1L;
}

BOOL CMainFrmWnd::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
//QC Monitor
// 	if(!m_pDebugMng || !pCopyDataStruct)
// 		return FALSE;
// 
// 	CString strTr;
// 	STIDM2Info_TranData *pData = (STIDM2Info_TranData*)pCopyDataStruct->lpData;
// 
// 	int nSize = (int)(pCopyDataStruct->cbData);
// 	CString strTrCopy(pData->aDataKey2,sizeof(pData->aDataKey2));
// 
// 	if(strTrCopy.CompareNoCase("so035020") == 0)
// 		strTr = TRNO_MSTCODE;
// 	else if(strTrCopy.CompareNoCase("piso0355") == 0)
// 		strTr = TRNO_DRAW_RESULT;
// //	else if(strTrCopy.CompareNoCase("d1010") == 0)
// //		strTr = QUERY_STRJONGMOK;
// 
// 	if (strTr.CompareNoCase("") != 0)
// 	{
// 		ReceiveData(0,strTr, "", "", (LPSTR)(LPCTSTR)pData->pData, nSize-(sizeof(pData)-1));		
// 	}

	return CWnd::OnCopyData(pWnd, pCopyDataStruct);
}

void CMainFrmWnd::InMakeFid_Mstcode(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos)
{
	//1. ������Ŷ, DRFN ����Ű�� ���ʿ��ֳ�???, �׷� space(80)
	memset(&szSaveBuf[nPos], 0x20, 80);
	nPos += 80;

	//2. TR�� ���ڿ��� �����.
	//	EU_p0621_InAll inAll;
	//	memset(&inAll, 0x20, sizeof(EU_p0621_InAll));
	//	int nSendTrDataSize = InMakeTr_Mstcode(pTrData, (BYTE*)&inAll);
	//	memcpy(&szSaveBuf[nPos], &inAll, nSendTrDataSize);
	//	nPos += nSendTrDataSize;

	//3. ���ڵ� ��ȣ 1���� ���ʷ� 
	CString szOutRecord;

	// 1st Record
	szOutRecord.Format("%-16d", 1);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	GetFillString(szOutRecord, 0, 0, 3);	// 0 ~ 4
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	memcpy(&szSaveBuf[nPos], "|", 1);	//������.
	nPos++;

	// 2st Record
	szOutRecord.Format("%-16d", 2);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	GetFillString(szOutRecord, 0, 3, 3);	// 0 ~ 5
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	memcpy(&szSaveBuf[nPos], ";", 1);	//������.
	nPos++;
}

BOOL CMainFrmWnd::SetConvert_SendPatternPacket(DRAW_IN &stDrawIn, p0624InBlock* pst0624)
{
	memset(pst0624, 0x20, sizeof(p0624InBlock));

	char tmpVal[2600]={0,};
	CString szVal;

	CString strBuff;
	LONG2STR_BUF(tmpVal, 4, stDrawIn.nTrIndex, strBuff);
	memcpy(pst0624->nTrIndex, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.nS_Time, strBuff);
	memcpy(pst0624->iS_Time, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.lTarget, strBuff);
	memcpy(pst0624->iTarget, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.lMoveAvg, strBuff);
	memcpy(pst0624->iMoveAvg, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.lS_Pos, strBuff);
	memcpy(pst0624->iS_Pos, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.lS_Range, strBuff);
	memcpy(pst0624->iS_Range, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.lSimilar, strBuff);
	memcpy(pst0624->iSimilar, tmpVal, 4);

	memcpy(pst0624->sBoxFlag, &stDrawIn.cBoxFlag, 1);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.nFieldEditerCnt, strBuff);
	memcpy(pst0624->FieldEditerCnt, tmpVal, 4);
	
	int fieldIndex=0;
	memset(tmpVal, 0x20, RESULT_FIELD_MAX_EUGENE);
	for(int nIndex = 0; fieldIndex < stDrawIn.nFieldEditerCnt; nIndex+=4)
	{
		strBuff.Format("%ld", stDrawIn.nFieldList[fieldIndex++]);
		memcpy(&tmpVal[nIndex], strBuff, strBuff.GetLength());
	}
	memcpy(pst0624->FieldList, tmpVal, RESULT_FIELD_MAX_EUGENE);	
	//	LONG2STR_BUF(tmpVal, RESULT_FIELD_MAX, stDrawIn.nFieldList, strBuff);
	//	memcpy(pst0624->FieldList, tmpVal, RESULT_FIELD_MAX);


	//	INT2STR_BUF(tmpVal, RESULT_MAX, stDrawIn.sTargetBit);
	//memcpy(pst0624->sTargetBit, stDrawIn.sTargetBit, RESULT_MAX);
	Safe_CheckCopy2(pst0624->sTargetBit, stDrawIn.sTargetBit, RESULT_MAX, szVal);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.stDrawData.lAccuVal, strBuff);
	memcpy(pst0624->iAccuVal, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.stDrawData.lDraw_Cnt, strBuff);
	memcpy(pst0624->iDraw_Cnt, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.stDrawData.lErrRate, strBuff);
	memcpy(pst0624->iErrRate, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.stDrawData.lGapRate, strBuff);
	memcpy(pst0624->iGapRate, tmpVal, 4);

	fieldIndex = 0;
	for(int nIndex = 0; nIndex < CNT_DRAW_VAL_EUGENE; nIndex += 4)
	{
		strBuff.Format("%04ld", stDrawIn.stDrawData.lDraw_Value[fieldIndex++]);
		memcpy(&tmpVal[nIndex], strBuff, strBuff.GetLength());
	}
	memcpy(pst0624->iDraw_Value, tmpVal, CNT_DRAW_VAL_EUGENE);	
	//	LONG2STR_BUF(tmpVal, CNT_DRAW_VAL, stDrawIn.stDrawData.lDraw_Value, strBuff);
	//	memcpy(pst0624->iDraw_Value, tmpVal, CNT_DRAW_VAL);	

	return TRUE;
}

void CMainFrmWnd::OnPF_Contorl_Move()
{
// 	ST_MOVEITEM stItem;	
// 	stItem.nType = PF_ITEM_BTN;
// 
// 	SET_ST_MOVEITEM(stItem, "btnSearch", &m_btnSearch);	
// 	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);
// 
// 	SET_ST_MOVEITEM(stItem, "btnPrint", &m_btnPrint);
// 	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);
// 
// 	SET_ST_MOVEITEM(stItem, "btnItemEdit", &m_btnFieldEdit);
// 	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);
// 
// 	SET_ST_MOVEITEM(stItem, "btnItemSend", &m_btnResultCtrl);
// 	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);	
// 
// 	SET_ST_MOVEITEM(stItem, "btnSave", &m_btnSave);
// 	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);	
// 
// 	SET_ST_MOVEITEM(stItem, "btnInit", &m_btnReset);
// 	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);

}

void CMainFrmWnd::PlatFormBtn_OnSearh()
{
	OnBtnSearch();
}

void CMainFrmWnd::PlatFormBtn_OnPrint()
{
	OnBtnPrint();
}

void CMainFrmWnd::PlatFormBtn_OnItemEdit()
{
	OnBtnFieldEdit();
}

void CMainFrmWnd::PlatFormBtn_OnItemSend()
{
	OnBtnResultCtrl();
}

void CMainFrmWnd::PlatFormBtn_OnSave()
{	
	OnBtnSave();
}

void CMainFrmWnd::PlatFormBtn_OnInit()
{
	OnBtnReset();
}

void CMainFrmWnd::GetRgn_ExeptButton(CRect rect, CRgn* pRgn)
{//ReDraw����Ʈ�� �������� ������ �ش翵���� ����(�÷��� ��Ʈ�� ��ü������ ReDraw)
	CRect rectBtn;
	CRgn rgnBtn;
	pRgn->CreateRectRgnIndirect(rect);	

	for(int nIndex = 0; nIndex < _countof(nCtrlID); nIndex++)
	{
		GetDlgItem(nCtrlID[nIndex])->GetWindowRect(&rectBtn);
		ScreenToClient(&rectBtn);
		rgnBtn.CreateRectRgnIndirect(rectBtn);
		pRgn->CombineRgn(pRgn, &rgnBtn, RGN_DIFF);
		rgnBtn.DeleteObject();
	}
}

// void CMainFrmWnd::SetControlBase(ICtlBase* _pBtnSearch,ICtlBase* _pBtnPrint, 
// 					ICtlBase*	_pBtnFeildEdit, ICtlBase* _pBtnSend, ICtlBase*	_pIBtnSave, ICtlBase*	_pIBtnInit)
// {
// 	m_bSetControlBase	= TRUE;
// 	m_pIbtnSearch		= _pBtnSearch;
// 	m_pIBtnPrint		= _pBtnPrint;
// 	m_pIBtnFeildEdit	= _pBtnFeildEdit;
// 	m_pIBtnSend			= _pBtnSend;
// 	m_pIBtnSave			= _pIBtnSave;
// 	m_pIBtnInit			= _pIBtnInit;
// 
// 	HWND hWnd = m_pIbtnSearch->GetHwnd();
// 	::SetParent(hWnd ,GetSafeHwnd());
// 	hWnd = m_pIBtnPrint->GetHwnd();
// 	::SetParent(hWnd ,GetSafeHwnd());
// 	hWnd = m_pIBtnFeildEdit->GetHwnd();
// 	::SetParent(hWnd ,GetSafeHwnd());
// 	hWnd = m_pIBtnSend->GetHwnd();
// 	::SetParent(hWnd ,GetSafeHwnd());
// 	hWnd = m_pIBtnSave->GetHwnd();
// 	::SetParent(hWnd ,GetSafeHwnd());
// 	hWnd = m_pIBtnInit->GetHwnd();
// 	::SetParent(hWnd ,GetSafeHwnd());
// 
// 	MoveControls();
// 
// }

void CMainFrmWnd::GetTabClr(COLORREF* clrBkgnd, ST_TABCOLOR* stTabColor)
{
	*clrBkgnd = m_clrBkgnd;
	*stTabColor = m_stTabColor;
}

CString CMainFrmWnd::GetUserFilePath(CString strOnlyFileName)
{
	CString strFilePath = "";
	CString strUserPath;

	GetDir_DrfnRoot(strUserPath);
	strUserPath.Format("%s%s%s", strUserPath, USRDIR, Variant(nameCC));

	strFilePath = (CString)strUserPath + (CString)FOLDER_FINDDATA + "\\" + strOnlyFileName;
	return strFilePath;
}

CString CMainFrmWnd::GetFilePath(CString strLoadType)
{
	CString strFile, strFolder;
	CString strFilePath = "";
	CString strAppPath;

	BOOL bUserFolder = FALSE;
	BOOL bSetIndustry = TRUE;

	strFile = (CString)FINDERINFO_INIFILE;

	GetDir_DrfnRoot(strAppPath);
	if ("USERCONDFILE" == strLoadType)
	{
		strFilePath.Format("%s\\%s\\%s", strAppPath, USRDIR, Variant(nameCC));	
	}
	else
	{
		strFilePath = strAppPath + (CString)FOLDER_FIND + "\\" + strFile;
	}

	return strFilePath;
}

BOOL CMainFrmWnd::LookUpSearchCenterModify(CString& strType, CString& strUserFileName)
{
	CString strFileName = GetFilePath("USERCONDFILE") + "\\SearchCenterModify.xml";

	BOOL bResult = TRUE;
	VARIANT_BOOL bLoadedXMLFile;

	DOCUMENT pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pDOMConditionIndex->async = false;
	bLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strFileName));

	if(bLoadedXMLFile == VARIANT_FALSE) 
	{
		bResult = FALSE;
		return bResult;
	}
	try
	{
		XMLELEMENT pParent = pDOMConditionIndex->documentElement;

		NODE pUserItem = pParent->firstChild;
		ATTRIBUTE pAttr = pUserItem->attributes;

		int i;
		for(i = 0; i < pAttr->length; ++i)
		{
			if(_stricmp(pAttr->item[i]->nodeName, "TYPE") == 0)
				strType = (char*)pAttr->item[i]->text;
			else if(_stricmp(pAttr->item[i]->nodeName, "FULLNAME") == 0)
			{
				strUserFileName = (char*)pAttr->item[i]->text;
				if(strUserFileName.Left(1) == "\\")
					strUserFileName = strUserFileName.Mid(1);
			}
		}
	}
	catch(_com_error e)
	{
		bResult = FALSE;
	}

	return bResult;
}

void CMainFrmWnd::SearchUserItem(CString& strFullName)
{
	CString strPath_Item;
	strPath_Item = GetFilePath("USERCONDFILE") + "\\UserPtrn\\" + strFullName;

	if (m_pctrlDrawPatten->GetLoad_DrawData(strPath_Item) == TRUE)
	{
		m_ptabCondition->SetCurSel(1);
		OnChangeTab(NULL, NULL);

		CString strTipText = strFullName;
		strTipText.Replace(".pdd", "");

		m_strCaption = strTipText;
		OnBtnSearch();
	}
}

BOOL CMainFrmWnd::SetTempGroupReg(CStringArray *psaCode, CUIntArray *iaMarkType)
{
	if (psaCode->GetSize() <= 0)
		return FALSE;

	CString strCode;

	return TRUE;
}

BOOL CMainFrmWnd::SetPortfolioReg(CString strOpenDlg, CStringArray *psaCode)
{
	
// 	CString strCode = psaCode->GetAt(0);
// 	if(pView)
// 	{
// 		IAUGwanManager* pGwanMng = (IAUGwanManager*)AfxGetPctrInterface(UUID_IAUGwanManager, 1);
// 		if(!pGwanMng)
// 			return FALSE;
// 
// 		CStringArray arrCode;
// 		for(int nIndex = 0; nIndex < psaCode->GetSize(); nIndex++)
// 		{	
// 			strCode = psaCode->GetAt(nIndex);
// 			strCode = strCode.Left(6);
// 			arrCode.Add(strCode);							
// 		}	
// 		//20100202 SJ_KIM
// 		pGwanMng->ShowGwansimDlgWithList(this, arrCode);	
// 	}
	

	return TRUE;
}

void CMainFrmWnd::SetSendSurfing(CStringArray *psaCode)
{
	if (SetPortfolioReg("0", psaCode) == TRUE)
	{
		SetSaveTempGroup(psaCode);
		SetSendMainMsg("���������� �ӽñ׷����� ������ ���½��ϴ�.");
	}
}

void CMainFrmWnd::SetSaveTempGroup(CStringArray *psaCode)
{
	CString strAppPath;
	GetDir_DrfnRoot(strAppPath);
	CString strFileName = strAppPath + "\\Usr\\1807_Result.ini";//20090615 SJ_KIM Fix the path

	CFile SaveFile(strFileName, CFile::modeCreate | CFile::modeWrite);
	CString strLineData;

	strLineData = "[REG_GROUP_LIST]\r\n";
	SaveFile.Write(strLineData, strLineData.GetLength());

	strLineData.Format("REG_CNT=%d\r\n", psaCode->GetSize());
	SaveFile.Write(strLineData, strLineData.GetLength());
	for (int nIndex = 0; nIndex < psaCode->GetSize(); nIndex++)
	{
		strLineData.Format("%d=%s\r\n", nIndex, psaCode->GetAt(nIndex));
		SaveFile.Write(strLineData, strLineData.GetLength());
	}

	SaveFile.Close();
}

//>>0000207: ���ǰ˻� �ǽð�, ������� Column ������ ��ġ(ũ��)���� �������� ���� �� ���� ��� �߰�
#define	szDoGridWidthSetSectKey "UserGridWidth"
void CMainFrmWnd::_DoGridWidthSet(int nOption)
{
	CUGCtrl*pGridCtrl = (CUGCtrl*)&m_gridResult;
	CMapStringToString UserColWidthsMap;
	LPCSTR szSectKey = szDoGridWidthSetSectKey;
	CString			   szUserMapFile;

	//CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CString szScreenCFG;
	szScreenCFG.Format("%s.cfg", m_strScreenNo);	//szScreenCFG.Format("%s.cfg", GetScreenKey());
	szUserMapFile = GetUserFilePath(szScreenCFG);
	//szUserMapFile = pwndMainFrm->GetFilePath("FINDERINFO");

	if(nOption==0)
	{
		CFindData_GridUtil::LoadFromUserEnvFile(szUserMapFile, szSectKey, &UserColWidthsMap);
		if(UserColWidthsMap.GetCount()>0)
			CFindData_GridUtil::SetGridAllWidth(pGridCtrl, &UserColWidthsMap);
	}
	else if(nOption==1)
	{
 		CFindData_GridUtil::GetGridAllWidth(pGridCtrl, &UserColWidthsMap);
 		CFindData_GridUtil::SaveToUserEnvFile(szUserMapFile, szSectKey, &UserColWidthsMap);
	}
}
//<<

CFont* CMainFrmWnd::GetFont(BOOL bBold)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	CFont* pFont;
	
	if(!bBold)
		pFont = pScreenWnd->GetDefFont();
	else
		pFont = pScreenWnd->GetBoldFont();
	
	return pFont;
}

COLORREF CMainFrmWnd::GetColor(int nColor)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	return pScreenWnd->GetAxColor(nColor);
}

CString CMainFrmWnd::Variant(int nComm, CString strData)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	return pScreenWnd->Variant(nComm, strData);
}

void CMainFrmWnd::SetBtnImg(CfxImgButton *pBtn, CString strFileName)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->SetBtnImg(pBtn, strFileName);
}

void CMainFrmWnd::SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->SetBtnImg(pBtn, nTextLen, bBold);
}

COLORREF CMainFrmWnd::GetAxColor(int nColor)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetAxColor(nColor);
}

HBRUSH CMainFrmWnd::GetAxBrush(COLORREF clr)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return (HBRUSH)(*pScreenWnd->GetAxBrush(clr));
}

CPen* CMainFrmWnd::GetAxPen(COLORREF clr, int nWidth, int nStyle)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetAxPen(clr, nWidth, nStyle);
}

CString CMainFrmWnd::GetFormCode()
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetFormCode();
}

CWnd* CMainFrmWnd::GetModuleMainWnd()
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetModuleMainWnd();
}

int CMainFrmWnd::GetFillString(CString& _szGetStr, int nStart, int nEnd, int nOneWordSize)
{
	int nLen = nEnd-nStart+1;
	if(nLen<=0) return 0;
	
	_szGetStr.Empty();
	
	char *szGetStr = (char*)malloc((nLen*nOneWordSize)+1);
	char *szOneWord = (char*)malloc(nOneWordSize+1);
	memset(szOneWord, 0, nOneWordSize+1);
	
	for(int i=nStart; i<=nEnd; i++)
	{
		sprintf(szOneWord, "%*.*d", nOneWordSize, nOneWordSize, i);
		memcpy(&szGetStr[nOneWordSize*i], szOneWord, nOneWordSize);
	}
	
	szGetStr[nLen*nOneWordSize] = 0;
	_szGetStr = szGetStr;
	free(szOneWord);
	free(szGetStr);
	
	return _szGetStr.GetLength();
}

BOOL CMainFrmWnd::GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetStockMasterData(arrCode, arrName, Jgub, Upgub);
}

BOOL CMainFrmWnd::GetUpMasterData(CStringArray *&psaCode, CStringArray *&psaName, int jgub)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetUpMasterData(psaCode, psaName, jgub);
}

CString CMainFrmWnd::GetMarketNameToCode(CString strMaketCode)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetMarketNameToCode(strMaketCode);
}

int CMainFrmWnd::SetResultTemp()
{
	CString strCode, strCodeName; 
	CStringArray strArrayCode, strArrayCodeName;
	
	int nDataCnt = m_gridResult.GetNumberRows();
	if(nDataCnt>0)
	{
		int nRow;
		int nMarkType;
		for (nRow = 0; nRow < m_gridResult.GetNumberRows(); nRow++)
		{
			m_gridResult.GetRowCode(nRow, strCode, nMarkType);
			strCodeName = m_gridResult.GetCellData(nRow, 0);
			
			strArrayCode.Add(strCode);
			strArrayCodeName.Add(strCodeName);
		}
		
		CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
		pScreenWnd->SetUserMaster(usrMasterCondition, strArrayCode.GetSize(), &strArrayCode, &strArrayCodeName);
	}
	return 0;
}

void CMainFrmWnd::Request_PortGroup()
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();

	int sendL = 0;
	char sendB[16 * 1024] = { 0, }, tempB[32];
	int isz_updn = sz_uinfo;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;
	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'g';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'U';

	pScreenWnd->SendTR(TRNO_FAVORITE, sendB, sendL, TRKEY_GWANSIMGROUP, US_PASS);

}

void CMainFrmWnd::GetData_PortGroup(CStringArray& saKey, CStringArray& saGroupName)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
#if (0)
	pScreenWnd->GetData_PortGroup(saKey, saGroupName);
#else
	saKey.Copy(m_GwanSimGroup.strKey);
	saGroupName.Copy(m_GwanSimGroup.strGroupName);
#endif
}

BOOL CMainFrmWnd::GetData_PortItem(CStringArray& saCode, CStringArray& saName, int nKey)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetData_PortItem(saCode, saName, nKey);
}

void CMainFrmWnd::DoSetPartnerSite(IPartnerManager* pPtManager)
{
	m_pPTManager = pPtManager;
	m_szPTManager.Format("%08X", pPtManager);
	if(pPtManager) pPtManager->SetSite(&m_xPartnerSite);
}

LRESULT CMainFrmWnd::OnUserInfo(WPARAM wp, LPARAM lp)
{
	BYTE bt = LOBYTE(LOWORD(wp));
	switch (bt)
	{
	case USERINFO_SET:
		{
			WORD dwKey = HIWORD(wp);
			if(dwKey==0)
			{
				IPartnerManager* pPtManager = (IPartnerManager*)lp;
				DoSetPartnerSite(pPtManager);
			}
		}
		break;
	}
	
	return 0L;
}

HRESULT CMainFrmWnd::CPartnerSite::RecieveFidData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	METHOD_PROLOGUE(CMainFrmWnd, PartnerSite)
		
		return S_OK;
}

HRESULT CMainFrmWnd::CPartnerSite::RecieveTRData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	METHOD_PROLOGUE(CMainFrmWnd, PartnerSite)
		
		if(strcmp(QUERY_STRJONGMOK, szTR)==0 && strcmp(QUERY_STRGUIDELINE, szMsgCode) != 0)
			pThis->ReceiveData(szTR, aTRData, dwTRDateLen);
		
		return S_OK;
}

HRESULT CMainFrmWnd::CPartnerSite::RecieveRealData(long dwKey, LONG dwData)
{
	return S_OK;
}

HRESULT CMainFrmWnd::CPartnerSite::RecieveKeyData(int nKey, LPCSTR _szData)
{
	METHOD_PROLOGUE(CMainFrmWnd, PartnerSite)
		
		switch(nKey)
	{
		case IPartnerSite::IPS_RK_gwansim:
			{
				//���ɱ׷��� ����Ǿ��ٴ� �޽����� �޾Ƽ� ���ɱ׷� ������ ��û��.
				// �ڼ���
				// 				if(pThis->m_pGwanMng)
				// 					pThis->m_pGwanMng->RequestGwansimGroup();
			}		
			break;
	}
	
	return S_OK;
}

HRESULT CMainFrmWnd::CPartnerSite::ReceiveOnUser(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}