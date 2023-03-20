// DlgOptimizer.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "DlgOptimizer.h"
//Include//
#include "../Common_ST/StrategyLoader.h"
#include "../Common_ST/STControllerDef.h"
#include ".\dlgoptimizer.h"
#include "../../inc/ISTManager.h"
#include "../../inc/IKSLibMng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptimizer dialog
#define CRBACKGROUND	RGB(211,223,241)
#define DEFAULT_BKGRND_COLOR		RGB(247,248,252)

#define IDC_GRID	0x00100
#define IDC_GRID2   0x00111


// CDlgSetEnv dialog
#define		NID_CODEINPUT		4578
#define		NID_ACCTINPUT		4579
#define		NID_FUTOPTINPUT		4582

// Strage Combo Mecro //////////////////////////////////////////////
#define STR_INDEX_CROSSABOVE_BASIS		"��ǥ���� ���ؼ� ���� ����"//���ؼ����� (0���� �ƴҰ�� ������������̽��� ���� �ɼǿ� �߰�)
#define STR_INDEX_CROSSBELOW_BASIS		"��ǥ���� ���ؼ� ���� ����"//���ؼ�����(0�ϰ�� 0�� �ɼǿ� �߰�) 
#define STR_INDEX_CROSSABOVE_ANOTHER	"��ǥ���� Signal�� ���� ����"//Signal �� ����.. + SignalIndex(Option)
#define STR_INDEX_CROSSBELOW_ANOTHER	"��ǥ���� Signal�� ���� ����"//Signal �� ����  +SignalIndex(Option)
#define STR_PRICE_CROSSABOVE_ANOTHER	"�ְ��� ���Ѽ�(���) ���� ����"//�ְ����� ����(�Ѽ� Index)
#define STR_PRICE_CROSSBELOW_ANOTHER	"�ְ��� ���Ѽ�(���) ���� ����"//�ְ��� ���� 

#define STR_INDEX_ZEROABOVE_BASIS     "��ǥ���� 0�� ���� ����"
#define STR_INDEX_ZEROBELOW_BASIS     "��ǥ���� 0�� ���� ����"

#define STR_INDEX_ABOVEBAN_BASIS	"��ǥ�� ��¹���"
#define STR_INDEX_BELOWBAN_BASIS	"��ǥ�� �϶�����"
	
///////////////////////////////////////////////////////////////////
#define		IDC_OUTLOOKBAR			1000
#define		IDC_STListCtrl			1001 //
#define MAX_COL 15
#define FILE_OPTIMIZE_CFG	"\\STDATA\\IndexOptimize.ini"

const UINT RMSG_STDATAMANAGER_RESULTDATA = ::RegisterWindowMessage("RMSG_STDATAMANAGER_RESULTDATA");
const UINT RMSG_OB_ITEMDBLCLK	= ::RegisterWindowMessage("RMSG_OB_ITEMDBLCLK");
const UINT RMSG_GETDLLSIZE		= ::RegisterWindowMessage("RMSG_GETDLLSIZE");

CDlgOptimizer::CDlgOptimizer(CWnd* pParent /*=NULL*/)
	: CEscDialog(CDlgOptimizer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptimizer)
//	m_EditGijun = 1;
	//}}AFX_DATA_INIT
	m_crBk = CRBACKGROUND;
	m_pStrategyItem = NULL;
	m_strGijun = "��";
	m_Code = "";
	m_nGijun = G_DATE;
	m_bReceive = FALSE;
	m_nTest =0;
	m_lSendNum =0;

	m_pDlgTimeSetup = NULL;
	m_nOptimizerDiv = 1001;
	m_pChartInterface = NULL;
	m_bGridClick = FALSE;
	m_STValNum = 0;
}


void CDlgOptimizer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptimizer)
	DDX_Control(pDX, IDC_BTN_ADDALL, m_CtlAddAll);
	DDX_Control(pDX, IDC_BTN_ADDONE, m_CtlAddOne);
	DDX_Control(pDX, IDC_BTN_DELALL, m_CtlDelAll);
	DDX_Control(pDX, IDC_BTN_DELONE, m_CtlDelOne);
	DDX_Control(pDX, IDC_BTN_STOP, m_BtnStop);
	DDX_Control(pDX, IDC_BTN_OPT, m_BtnOpt);
	DDX_Control(pDX, IDC_TAB1, m_CtlTab);
	DDX_Control(pDX, IDC_LIST_ST, m_listST);
	//}}AFX_DATA_MAP
	//DDX_Control(pDX, IDC_PROGRESS, m_CtlProgress);
}


BEGIN_MESSAGE_MAP(CDlgOptimizer, CDialog)
	//{{AFX_MSG_MAP(CDlgOptimizer)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_OPT, OnBtnOpt)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WMU_SET_KEYCODE, OnSetKeyCodeChange)
	ON_REGISTERED_MESSAGE(RMSG_STDATAMANAGER_RESULTDATA, OnSTDataManagerMessagge)
	ON_BN_CLICKED(IDC_BTN_ADDALL, OnBnClickedBtnAddall)
	ON_BN_CLICKED(IDC_BTN_ADDONE, OnBnClickedBtnAddone)
	ON_BN_CLICKED(IDC_BTN_DELALL, OnBnClickedBtnDelall)
	ON_BN_CLICKED(IDC_BTN_DELONE, OnBnClickedBtnDelone)
	ON_REGISTERED_MESSAGE(RMSG_OB_ITEMDBLCLK	,OnRmsgObItemDBClk)
	ON_REGISTERED_MESSAGE(RMSG_GETDLLSIZE, OnRmsgGetDllSize)
	ON_LBN_DBLCLK(IDC_LIST_ST, OnLbnDblclkListSt)
	ON_REGISTERED_MESSAGE(RMSG_STTREE_ITEMDBLCLK,OnRmsgSTTreeItemDBClk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptimizer message handlers
// CDlgAnalysis message handlers
void	CDlgOptimizer::FormInit()
{
}

void CDlgOptimizer::FormLoad()
{
}

BOOL CDlgOptimizer::OnInitDialog() 
{
	CDialog::OnInitDialog();

	AfxEnableControlContainer();

	// TODO: Add extra initialization here
	
	//IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager);
	//if(pKSLibMng)
	//	m_crBk = pKSLibMng->Lib_KSLink_GetColor(1);	// ����ȭ�� ���
	//else
	m_crBk = DEFAULT_BKGRND_COLOR;
//	m_crBk = DEFAULT_BKGRND_COLOR;
	m_brBkColor.CreateSolidBrush(m_crBk);
	//((CButton*)GetDlgItem(IDC_RADIO_DAY))->SetCheck(TRUE);
	//m_CtlEditGijun.EnableWindow(FALSE);
	//m_CtlEditBong.SetWindowText("1");
	
	LOGFONT lf;
	::memset(&lf, 0x00, sizeof(LOGFONT));
	lf.lfWidth = 0; 
	lf.lfEscapement = 0; 
	lf.lfOrientation = 0; 
	lf.lfWeight = FW_NORMAL; 
	lf.lfItalic = 0; 
	lf.lfUnderline = 0; 
	lf.lfStrikeOut = 0; 
	lf.lfCharSet = DEFAULT_CHARSET; 
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS; 
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = FF_ROMAN; 
	strcpy(lf.lfFaceName, "����ü");
	lf.lfHeight = 12;
	m_Font.CreateFontIndirect(&lf);

	InitCodeInfo();
	InitTab();
	InitGrid();
	ReSize();
	InitControls();
	LoadOutlookBar();//KHD : OutlookBar
	InitStrategy();
	InitSkin();
	CreateTImeSetupDlg();
	

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//����ȭ �ɼǿ� ���� ��Ʈ�Ѱ� �׸��带 ���� �����ϴ� ��ƾ :KHD
void CDlgOptimizer::HideControl()
{

	
}
//KHD : OutlookBar Add 2006.08.30 
HINSTANCE hTradeInst = NULL;
void CDlgOptimizer::LoadOutlookBar()
{
	CRect rc2;
	GetDlgItem(IDC_STFRAME)->GetWindowRect(rc2);
	ScreenToClient(rc2);
	rc2.top += 3;

	CString szImageChartDir = m_strImageDir + "\\drchart";
	//W190_H21===========================================================================
	BOOL bOK = FALSE;
//	CBitmap bitmap;
//	CString strBitmapFile;
//	CString szImageChartDir = m_strImageDir + "\\drchart";
//	if ( !m_ilW190H21.m_hImageList)
//	{		
//		if ( !m_ilW190H21.Create( 151, 21, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
//		{
//			AfxMessageBox ( "Can not Find W151_H21.bmp file" );
//		}
//		strBitmapFile.Format("%s\\%s", szImageChartDir,  "W151_H21.bmp");		
//		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
//			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
//		m_ilW190H21.Add(&bitmap, RGB(255, 0, 255));
//		bitmap.DeleteObject();
//	}
//
//	//icon image===========================================================================
//	CString szILPath = szImageChartDir + "\\treeIcon.bmp";
//	bitmap.Attach((HBITMAP)LoadImage(NULL, (LPSTR)(LPCTSTR)szILPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
//	m_ilSTIcon.Create(9, 18, ILC_MASK | ILC_COLORDDB, 1, 1);
//	m_ilSTIcon.Add(&bitmap, RGB(255, 0, 255));
//	bitmap.DeleteObject();
	CBitmap bitmap;
	CString strImageDir;
	strImageDir.Format("%s\\folder.bmp", szImageChartDir);
	bitmap.Attach((HBITMAP)LoadImage(NULL, (LPSTR)(LPCTSTR)strImageDir, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
	m_ImageListIcon.Create(18, 18, ILC_MASK | ILC_COLORDDB, 1, 1);
	m_ImageListIcon.Add(&bitmap, RGB(255, 0, 255));
	bitmap.DeleteObject();

	//20061213 ������ ���� 
	//LeftBar Ʈ������� �̹��� ����
//@�������� 
	strImageDir = "";
	strImageDir.Format("%s\\outbar_icon.bmp", szImageChartDir);
	bitmap.Attach((HBITMAP)LoadImage(NULL, (LPSTR)(LPCTSTR)strImageDir, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
	m_ImageOutbarIcon.Create(20, 20, ILC_MASK | ILC_COLORDDB, 1, 1);
	m_ImageOutbarIcon.Add(&bitmap, RGB(255, 0, 255));
	bitmap.DeleteObject();
//@�������� 

	DWORD dwf = fDragItems|fEditGroups|fEditItems|fRemoveGroups|fRemoveItems|fAddGroups|fAnimation;
	if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG)
	{
		typedef	LONG (FnSCW_IGfxOutBarCtrl)(int nType, long dwValue, long dwReserved1);
		if(!m_hSTConfigureWizardInst)
			m_hSTConfigureWizardInst  = LoadLibrary("Chart\\HWStrategyConfigureWizard.dll");
		if(!m_hSTConfigureWizardInst) return;

		FnSCW_IGfxOutBarCtrl* pFunc = (FnSCW_IGfxOutBarCtrl*)GetProcAddress(m_hSTConfigureWizardInst,_T("SCW_IGfxOutBarCtrl"));
		if(pFunc == NULL) return ;

		m_pWndOutBar = (IGfxOutBarCtrl2*)pFunc(0, 0, 0);

		m_pWndOutBar->CreateOutBar(WS_CHILD|WS_VISIBLE, rc2, this, IDC_OUTLOOKBAR, dwf);
		m_pWndOutBar->Setcr3dFace(RGB(191,211,233));
		m_pWndOutBar->Setcr3dUnFace(RGB(218,218,218));
		m_pWndOutBar->GetCoutBarCtrl()->SetOwner(this);

		int  nfSmallIcon = 1 << 0;
//		m_pWndOutBar->SetImageList(&m_ilSTIcon, nfSmallIcon);
//		m_pWndOutBar->SetFolderImageList(&m_ilW190H21, 0, 2, 3);
		m_pWndOutBar->SetImageList(&m_ImageListIcon, nfSmallIcon);
		m_pWndOutBar->SetFolderImageList(&m_ImageOutbarIcon, 0, 2, 3);
	}
	else
	{
		m_wndOutBar.Create(WS_CHILD|WS_VISIBLE, rc2, this, IDC_OUTLOOKBAR, dwf);
		m_wndOutBar.cr3dFace =  RGB(191,211,233);
		m_wndOutBar.cr3dUnFace = RGB(239,242,247);
		m_wndOutBar.SetOwner(this);

		int  nfSmallIcon = 1 << 0;
//		m_wndOutBar.SetImageList(&m_ilSTIcon, nfSmallIcon);
//		m_wndOutBar.SetFolderImageList(&m_ilW190H21, 0, 2, 3);
		m_wndOutBar.SetImageList(&m_ImageListIcon, nfSmallIcon);
		m_wndOutBar.SetFolderImageList(&m_ImageOutbarIcon, 0, 2, 3);

		COLORREF clrOLBkgrnd;
		COLORREF clrOLOutLine;
		if(m_pChartInterface && ((ISTInterface*)m_pChartInterface)->GetOutBarColor(clrOLBkgrnd, clrOLOutLine))
		{
			m_wndOutBar.cr3dFace = clrOLOutLine;
			m_wndOutBar.cr3dUnFace = clrOLOutLine;
			m_wndOutBar.crBackGroundColor1 = clrOLBkgrnd;
		}
	}
	m_arSTPath.RemoveAll();
	LoadSTList();
}

void CDlgOptimizer::LoadSTList()
{
	//@@@06.12.07[[
	if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG)
	{
		LoadSTList_SSStyle();
		return;
	}
	//@@@06.12.07]]
	if(gCURRENT_COMPANY_ID == nPCTRComID_EUGENE)
	{
		typedef	LONG (FPSCW_GetInterfaceOfStrategy)(LONG dwType/*=0*/);
		hTradeInst = LoadLibrary("HWStrategyConfigureWizard.dll");
		if(hTradeInst == NULL) return;

		FPSCW_GetInterfaceOfStrategy* pFunc = (FPSCW_GetInterfaceOfStrategy*)GetProcAddress(hTradeInst,_T("SCW_GetInterfaceOfStrategy"));
		if(pFunc == NULL) return ;

		LONG dwType = 0;	//CSTLoadManager*
		m_pSTLoadMnger = (ISTLoadManager2*)pFunc(dwType);
		if(m_pSTLoadMnger)
		{
			CString szDataDir = (LPCSTR)AfxGetPctrData(12);	// 12 : Data Directory
			CString szUserDir = (LPCSTR)AfxGetPctrData(17);	// 17 : User Directory

			CString szSTData_STPath = szDataDir + "\\" + STFOLDER_ST;
			m_arSTPath.Add(szSTData_STPath);
			if(m_nOptimizerDiv != VAL_OPTIMIZER)//��������ȭ�϶��� ����� ���� �ε� ���� �ʴ´�. KHD :2007.1.18 
			{
				CString szSTUser_STPath = szUserDir + "\\" + STFOLDER_ST;
				m_arSTPath.Add(szSTUser_STPath);
			}	

//			m_pSTLoadMnger->Init(TRUE, "*.ST", &m_ilSTIcon);
			m_pSTLoadMnger->Init(TRUE, "*.ST", &m_ImageListIcon);
			m_pSTLoadMnger->SetUseBoldFont(FALSE);

			int p_LoadOption = 1;
			m_pSTLoadMnger->LoadST2(&m_wndOutBar, &m_arSTPath, 0, 0, p_LoadOption);
		}
	}
}

//OutlookBar End
void CDlgOptimizer::InitStrategy()
{
//
//#ifdef _DEBUG
// CString strSystemPath = m_strRootPath +  "\\" + STSYSTEM_D;
// CString strUserPath = m_strRootPath + "\\" + STUSER_D;
//#else
// CString strSystemPath = m_strRootPath + "\\" + STSYSTEM;
// CString strUserPath = m_strRootPath + "\\" + STUSER;
//#endif
//	m_stArrayPath.RemoveAll(); 
//	m_stArrayPath.Add(strSystemPath);
//	m_stArrayPath.Add(strUserPath);
//
//	if(!m_pStrategyItem)
//	{
//		m_pStrategyItem = m_strategyLoader.GetStrategyItem();// �޸� ���� 
//		if(m_pStrategyItem == NULL) return;
//		//����, DBMGr, Ű������ ���� 
//	//	int nRetValue = m_pStrategyItem->Create(this,m_hSocket,(long)&m_IDrMLib_CDBMgr,(long)m_pStrategyItem,m_stArrayPath);
//	//	if(nRetValue==-1) 
//		{
//			m_pStrategyItem->DestoryWindow();
//			delete m_pStrategyItem;
//			m_pStrategyItem = NULL;
//		}
//	}
//	m_DlgTrade->SetStrategyItem(m_pStrategyItem);
}
HBRUSH CDlgOptimizer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		pDC->SetBkColor(m_crBk);
		return m_brBkColor;		
		break;
	}	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
void CDlgOptimizer::InitCodeInfo()
{
}

LRESULT CDlgOptimizer::OnSetKeyCodeChange(WPARAM wp, LPARAM lp)
{
	return TRUE;
}

CString CDlgOptimizer::GetJMNameAndGubunFromCode(CString csCode)
{
	return "";
}

void CDlgOptimizer::InitGrid()
{

	if(!m_GridTap.GetSafeHwnd())
	{
		CRect rect1;
		rect1.left = 193;
		rect1.right = 860;
		rect1.bottom = 195;
		rect1.top = -20;
		
		rect1.DeflateRect(0,20);
		m_GridTap.Create(rect1,this,IDC_GRID2,WS_CHILD | WS_TABSTOP | WS_VISIBLE   );
	
		m_GridTap.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
		
		m_GridTap.SetGridLineColor(RGB(204,204,204));
		if(m_nOptimizerDiv == INDEX_OPTIMIZER)
			m_GridTap.SetColumnCount(12);
		if(m_nOptimizerDiv == TIME_OPTIMIZER)
			m_GridTap.SetColumnCount(8);
		if(m_nOptimizerDiv == VAL_OPTIMIZER)
			m_GridTap.SetColumnCount(6);
		m_GridTap.SetFixedRowCount(1);
		m_GridTap.SetColumnResize(TRUE);
		m_GridTap.SetRowCount(1);
		m_GridTap.SetListMode();
		m_GridTap.SetRowHeight(0,20);
		m_GridTap.SetBkColor(RGB(255,255,255));		
		m_GridTap.SetEditable(FALSE);
		m_GridTap.EnableTitleTips(FALSE);
		m_GridTap.SetTrackFocusCell(FALSE);
		m_GridTap.SetSingleColSelection(FALSE);
		m_GridTap.SetSingleRowSelection();
		m_GridTap.SetFixedRowSelection(FALSE);
		m_GridTap.SetFixedColumnSelection(FALSE);
		m_GridTap.SetFont(&m_Font);
	}
}
//������ ���� Grid �÷� ���� �ϴ� �κ�..
void CDlgOptimizer::ReSize()
{
	if(m_GridTap.GetSafeHwnd())
	{
		char *psztitle_Index[MAX_COL]=
		{"Row","������","�Ѽ���","��ռ���","������","�Ѽս�","�ִ�����","�ִ�ս�","���ͷ�","ProfitFactor","�·�","�ŷ�"};//��������ȭ;
		char *psztitle_Val[MAX_COL]={"Row","�Ѽ���","��ռ���", "������", "�Ѽս�", "�ִ�����"};//��������ȭ;
		char *psztitle_Time[MAX_COL]= {"Row","��ǥ��","��","�Ѽ���","��ռ���","������","�Ѽս�","�ִ�����"};//�ð�����ȭ;
		
		int lWidth_Index[MAX_COL]= {0, 100, 70, 60, 70, 60, 75, 75, 60, 80, 45, 45};
		int lWidth_Val[MAX_COL]={0,100, 70, 80, 70, 70};
		int lWidth_Time[MAX_COL]= {0, 120, 7, 75, 50, 75,75,50};
		
		char *psztitle[MAX_COL];
		int  lWidth[MAX_COL];
		switch(m_nOptimizerDiv)
		{
		case INDEX_OPTIMIZER:
			memcpy(psztitle,psztitle_Index,sizeof(psztitle_Index));
			memcpy(lWidth, lWidth_Index, sizeof(lWidth_Index));		
			break;
		case VAL_OPTIMIZER:
			memcpy(psztitle,psztitle_Val,sizeof(psztitle_Val));
			memcpy(lWidth, lWidth_Val, sizeof(lWidth_Val));		
			break;
		case TIME_OPTIMIZER:
			memcpy(psztitle,psztitle_Time,sizeof(psztitle_Time));
			memcpy(lWidth, lWidth_Time, sizeof(lWidth_Time));		
			break;
		}
		
		CString str;
		GV_ITEM Item;
		int Colnum =0;
		if(m_nOptimizerDiv!= VAL_OPTIMIZER || m_pDlgTimeSetup ==NULL)
			Colnum = m_GridTap.GetColumnCount();
		else//��������ȭ�ϰ�� //
		{
			Colnum = 6 + m_STValNum;
			m_GridTap.SetColumnCount(Colnum);
		}
		for (int col = 0; col < Colnum; col++)
		{
			
    		Item.mask = GVIF_TEXT;
			Item.row = 0;
			Item.col = col;
			if(m_nOptimizerDiv == VAL_OPTIMIZER && m_pDlgTimeSetup!=NULL)
			{
				if( col < m_STValNum)
				{				
					Item.strText = m_pDlgTimeSetup->m_ValueOptimizerArray.GetAt(col).nTitle;
					m_GridTap.SetColumnWidth(col,65);
				}
				else
				{
					Item.strText = psztitle[col-m_STValNum];
					m_GridTap.SetColumnWidth(col,lWidth[col-m_STValNum]);
				}
			}
			else
			{
				Item.strText = psztitle[col];
				m_GridTap.SetColumnWidth(col,lWidth[col]);
			}
			Item.crBkClr = RGB(230,239,247);//RGB(175,194,221); // or - m_GridSTList.SetItemBkColour(row, col, clr);
			Item.crFgClr = RGB(51,51,51);    // or - m_GridSTList.SetItemFgColour(row, col, RGB(255,0,0));				    
			Item.nFormat = DT_CENTER;
			Item.mask    |= (GVIF_BKCLR|GVIF_FGCLR|GVIF_FORMAT);
			
			m_GridTap.SetItem(&Item);

		}
  		m_GridTap.ExpandColumnsToFit();	
	}

}

void CDlgOptimizer::AutoSizeColumns()
{
	if(m_GridTap.GetSafeHwnd())
	{
		if(m_GridTap.IsVisibleVScroll()==FALSE)
		{
			ReSize();
			return;
		}

		switch(m_nOptimizerDiv)
		{
		case INDEX_OPTIMIZER:
			m_GridTap.AutoSizeColumn(1);
			m_GridTap.AutoSizeColumn(3);
			m_GridTap.AutoSizeColumn(8);
			break;
		case VAL_OPTIMIZER:
			m_GridTap.AutoSizeColumn(5);
			break;
		case TIME_OPTIMIZER:
			m_GridTap.AutoSizeColumn(1);
			m_GridTap.AutoSizeColumn(4);
			m_GridTap.AutoSizeColumn(7);
			break;
		}
	}
}

void CDlgOptimizer::InitTab()
{
	CRect rc;
	m_CtlTab.GetClientRect(&rc);
	rc.left = 193;
	rc.top  = 195;
	m_strDataDir += "\\STData";
	m_CtlTab.SetTopLeftCorner(CPoint(rc.left,rc.top));
	m_CtlTab.SetLeftOffset(0);
	m_CtlTab.SetUpperOffset(0);
	m_CtlTab.SetSelectedColor(RGB(0, 0, 0),CRBACKGROUND);
	m_CtlTab.SetNormalColor(RGB(0,0,0), RGB(255,255,255) , RGB(226,225,233));
	m_CtlTab.SetTabBkColor(RGB(234,235,233));
	
	m_DlgResult = new CDlgResult;
	m_DlgResult->SetGridFont(&m_Font);
	m_DlgResult->Create(CDlgResult::IDD, this);
	m_DlgResult->SetOptimizer(this);
	m_DlgResult->SetDataDir(m_strDataDir);
	m_DlgResult->OpenWebView("\\IndexOptimizer.htm"); 

	m_arrayTab.Add(m_DlgResult);


	m_DlgSooik  = new CTabSoonikChart;
	m_DlgSooik->Create(CTabSoonikChart::IDD,this);
	m_DlgSooik->SetParentDlg(this);
	m_arrayTab.Add(m_DlgSooik);

	m_DlgSonik  = new CTabSonikChart;
	m_DlgSonik->Create(CTabSonikChart::IDD,this);
	m_DlgSonik->SetParentDlg(this);
	m_arrayTab.Add(m_DlgSonik);

	m_DlgTrade  = new CDlgTrade;
	m_DlgTrade->SetGridFont(&m_Font);
	m_DlgTrade->Create(CDlgTrade::IDD,this);
	m_DlgTrade->SetParentDlg(this);
	m_DlgTrade->SetDataDir(m_strDataDir);
	m_DlgTrade->OpenWebView("\\IOTrade.htm");
	m_arrayTab.Add(m_DlgTrade);

	m_CtlTab.AddTab(m_DlgResult,"�����м�");
	m_CtlTab.AddTab(m_DlgTrade,"�Ÿų���");
	m_CtlTab.AddTab(m_DlgSooik,"���ͷ���Ʈ");
	m_CtlTab.AddTab(m_DlgSonik,"���ͺ�����Ʈ");
}

void CDlgOptimizer::OnDestroy() 
{
	if(m_Font.GetSafeHandle())
		m_Font.DeleteObject();
	
	CDialog::OnDestroy();

	m_ImageListIcon.DeleteImageList();
	m_ImageOutbarIcon.DeleteImageList();

	DestoryTImeSetupDlg();

	if(m_pStrategyItem)
	{
		m_pStrategyItem->DestoryWindow();
		delete m_pStrategyItem;
		m_pStrategyItem = NULL;
	}

	OnBnClickedBtnDelall();
	// TODO: Add your message handler code here
	CDialog *pTabDlg = NULL;
	int nSize = m_arrayTab.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pTabDlg = m_arrayTab.GetAt(nPos);
		pTabDlg->DestroyWindow();
		delete pTabDlg;
	}

	ST_VALUESET *ValueSet;
	for(int i =0 ;i< m_arrayValue.GetSize();i++)
	{
		ValueSet = m_arrayValue.GetAt(i);
		delete ValueSet;
	}
	m_arrayTab.RemoveAll();	

	//@@@06.12.07[[
	{
		UnLoadSTList();
	   FreeLibrary(hTradeInst);
		typedef	LONG (FnSCW_IGfxOutBarCtrl)(int nType, long dwValue, long dwReserved1);

		FnSCW_IGfxOutBarCtrl* pFunc = (FnSCW_IGfxOutBarCtrl*)GetProcAddress(m_hSTConfigureWizardInst ,_T("SCW_IGfxOutBarCtrl"));
		if(pFunc == NULL) return ;

		if(m_pWndOutBar) {
			// m_pWndOutBar->GetCoutBarCtrl()->DestroyWindow();
			pFunc(1, (long)m_pWndOutBar, 0);
			m_pWndOutBar = NULL;
		}
	}
	//@@@06.12.07]]
}
void CDlgOptimizer::InitControls()
{
	//Init ��ǥ Index //
	return;
	CStdioFile IndexFile;
	CString strIndexData,str;
	if(!IndexFile.Open(m_strDataDir+"\\SystemTradingIndexList.dat",CFile::modeRead))
	{
		AfxMessageBox("Do Not Create Html File.");
		return;
	}
// �߰� ��Ű�� ���ƾ� �� �����
	char str1[265];
	char* p ;
	CStringArray RemoveData;
	::GetPrivateProfileString("REMOVE","DATA","No",str1,80,m_strDataDir+FILE_OPTIMIZE_CFG);
	 p = strtok(str1,",");
	while(p!=NULL)
	{
		RemoveData.Add(p);
		p = strtok(NULL,",");		
	}
////////////////////////////////////////////
	for(;;){
		if(IndexFile.ReadString(str))
		{
			//��ǥ�� Combo Init 
			int start = str.Find(";",0);
			strIndexData = str.Left(start);
			BOOL IsData = TRUE;
			for(int i=0;i<RemoveData.GetSize();i++)
			{
				if(RemoveData.GetAt(i)==strIndexData)
				{
					RemoveData.RemoveAt(i);
					IsData = FALSE;
					break;
				}
			}
			if(IsData == FALSE) continue;
	//		m_CtlComIndex.AddString(strIndexData);

			str.Delete(0,start+1);

			//SD ���� ���� 
			start = str.Find(";",0);
			strIndexData = str.Left(start);	
			m_stArraySDFileName.Add(strIndexData);
			str ="";
		}
		else
			break;
	}	
	IndexFile.Close();


    
}


void CDlgOptimizer::MedoMesuComboDataParsing()
{

	//CString strIndex;
	//strIndex = SearchFileToData("Index",m_strDataDir+FILE_OPTIMIZE_CFG);
	//m_CtlComMesu.ResetContent();
	//m_CtlComMedo.ResetContent();
	////00000 : Signal����, 0�� ���� , ���ؼ� ���� , ��ǥ������,��嵹�� 
	//if(strIndex == "N")
	//{
	//	AfxMessageBox("���� ������ ���� ������ �����ϴ�.");
	//	return;
	//}
	//else //00000�Ľ� 
	//{
	//	for(int i = 0 ;i < strIndex.GetLength();i++)
	//	{
	//		CString OneData = strIndex.GetAt(i); 
	//		int DataIndex = atoi(OneData);
	//		switch(i)
	//		{
	//
	//		case 0: //Signal
	//			if(DataIndex)
	//			{
	//				m_CtlComMesu.AddString(STR_INDEX_CROSSABOVE_ANOTHER);
	//				m_CtlComMesu.AddString(STR_INDEX_CROSSBELOW_ANOTHER);
	//				
	//				m_CtlComMedo.AddString(STR_INDEX_CROSSABOVE_ANOTHER);
	//				m_CtlComMedo.AddString(STR_INDEX_CROSSBELOW_ANOTHER);
	//			}
	//			break;
	//		case 1: // 0 Line
	//			if(DataIndex)
	//			{
	//				m_CtlComMesu.AddString(STR_INDEX_ZEROABOVE_BASIS);
	//				m_CtlComMesu.AddString(STR_INDEX_ZEROBELOW_BASIS);

	//				m_CtlComMedo.AddString(STR_INDEX_ZEROABOVE_BASIS);
	//				m_CtlComMedo.AddString(STR_INDEX_ZEROBELOW_BASIS);
	//			}
	//			break;
	//		case 2: // ���ؼ� : �������̽� �ʿ�
	//			if(DataIndex)
	//			{
	//				m_CtlComMesu.AddString(STR_INDEX_CROSSABOVE_BASIS);
	//				m_CtlComMesu.AddString(STR_INDEX_CROSSBELOW_BASIS);

	//				m_CtlComMedo.AddString(STR_INDEX_CROSSABOVE_BASIS);
	//				m_CtlComMedo.AddString(STR_INDEX_CROSSBELOW_BASIS);
	//			}
	//			break;
	//		case 3: // ��ǥ������  
	//			if(DataIndex)
	//			{
	//				m_CtlComMesu.AddString(STR_INDEX_ABOVEBAN_BASIS);
	//				m_CtlComMesu.AddString(STR_INDEX_BELOWBAN_BASIS);

	//				m_CtlComMedo.AddString(STR_INDEX_ABOVEBAN_BASIS);
	//				m_CtlComMedo.AddString(STR_INDEX_BELOWBAN_BASIS);
	//			}
	//			break;
	//		case 4: // ��嵹��
	//			if(DataIndex)
	//			{
	//				m_CtlComMesu.AddString(STR_PRICE_CROSSABOVE_ANOTHER);
	//				m_CtlComMesu.AddString(STR_PRICE_CROSSBELOW_ANOTHER);
	//				
	//				m_CtlComMedo.AddString(STR_PRICE_CROSSABOVE_ANOTHER);
	//				m_CtlComMedo.AddString(STR_PRICE_CROSSBELOW_ANOTHER);
	//			}
	//			break;
	//		}
	//	}
	//}
	
}

CString CDlgOptimizer::SearchFileToData(CString strIndex, CString strPath)
{
	/*CString strDataName;
	CHAR inBuf[80]; 
	m_CtlComIndex.GetLBText(m_CtlComIndex.GetCurSel(),strDataName);
	::GetPrivateProfileString(strDataName,strIndex,"N",inBuf,80,strPath);
	strIndex = inBuf;*/
	

	return strIndex;
}
//���ذ� �� ���� ���� �ϴ� �Լ� 
BOOL CDlgOptimizer::IsCurrentChartData(CString strValue)
{
	//int nMin;
	//if(m_nGijun == G_MINUTE)//���� ��� 
	//{
	//	m_CtlEditGijun.GetWindowText(m_strGijun);
	//	
	//	nMin = atoi(m_strGijun);
	//	if(nMin<0 || nMin>61)
	//	{
	//		AfxMessageBox("���غ��� 1 ~ 60���� �ٽ� �Է��ϼ���");
	//		m_CtlEditGijun.SetFocus();
	//		return FALSE;
	//	}
	//	m_strGijun +="��";
	//}
	//else 
	//	m_strGijun = "��";

	//nMin = atoi(strValue);
	//if(nMin<0||nMin>1001)
	//{
	//	AfxMessageBox("������ 1 ~ 1000 ������ ���� �����ϼ���");
	//	m_CtlEditBong.SetFocus();
	//	return FALSE;
	//}	

	return TRUE;
}
//�ٽ� ��Ʈ�� �ʱ�ȭ �ϴ� �Լ� 
void CDlgOptimizer::RemoveAllControlSet()
{
	/*ST_VALUESET *ValueSet;
	for(int j =0; j< m_arrayValue.GetSize();j++)
	{
		ValueSet = m_arrayValue.GetAt(j);
		ValueSet->stEdit->ShowWindow(SW_HIDE);
		ValueSet->stSpinBtn->ShowWindow(SW_HIDE);
		ValueSet->stStatic->ShowWindow(SW_HIDE);
		
		ValueSet->stEdit->SetWindowText("0");
		ValueSet->stEdit->SetReadOnly(FALSE);
		ValueSet->stEdit->MoveWindow(ValueSet->rt);
		
		ValueSet->stSpinBtn->EnableWindow(TRUE);
		
	}*/
}


//����ȭ ��ư :KHD

typedef struct {
	long pThisPoint;
}STOPTTHREAD;
UINT StartOptThread(LPVOID pParam)
{
	STOPTTHREAD* pInData = (STOPTTHREAD*)pParam;
	CDlgOptimizer* pDlg = (CDlgOptimizer*)pInData->pThisPoint;
	pDlg->StartOpt();

	return 0;
}

void CDlgOptimizer::StartOpt() 
{
	STOPTTHREAD inData;
	inData.pThisPoint = (long)this;

	AfxBeginThread(StartOptThread, &inData);
}

void CDlgOptimizer::OnBtnOpt()
{
	// TODO: Add your control notification handler code here
	//�ð�, ����, ���� ����ȭ ���� ��ƾ//////////////////
	//���� ���� ��� ������
	CString szSTPath;

	CStringArray stArraySTPath;
	CStringArray stArrayPath;
	
	//�׸��带 �ʱ�ȭ �մϴ�.
	//for(int i =1;i<m_GridTap.GetRowCount();i++)
	m_GridTap.SetRowCount(1);
	m_bGridClick = FALSE;
	if(m_nOptimizerDiv == TIME_OPTIMIZER || m_nOptimizerDiv == INDEX_OPTIMIZER)
	{
			
		int nListCount = m_listST.GetCount();
		if(nListCount <=0){
			AfxMessageBox("������ ����Ʈ�� �����ϴ�.");
			return;
		}
		STMNG_ITEMINFO* pItemInfo = NULL;
		for(int nIndx = 0; nIndx < nListCount; ++nIndx)
		{
			if(pItemInfo = (STMNG_ITEMINFO*)m_listST.GetItemData(nIndx))
			{
				szSTPath	= pItemInfo->szSTPath +"\\"+ pItemInfo->szSTName + ".st";
				stArraySTPath.Add(szSTPath);
			}
		}
	}
//@��������
//#ifdef _DEBUG
//	CString strSystemPath = m_strRootPath +  "\\" + STSD_D;
//	CString strUserPath = m_strRootPath + "\\" + STUSER_D;
//#else
//	CString strSystemPath = m_strRootPath + "\\" + STSD;
//	CString strUserPath = m_strRootPath + "\\" + STUSER;
//#endif
//@��������
//@�����߰�
	CString strSystemPath = m_strRootPath + "\\" + STSD;
	CString strUserPath = m_strRootPath + "\\" + STUSER;
//@�����߰�
	
	stArrayPath.Add(strSystemPath);//Data
	stArrayPath.Add(strUserPath);//User
	//�����޴��� ����
	if(!m_pStrategyItem)
	{
		m_pStrategyItem = m_strategyLoader.GetStrategyItem();// �޸� ���� 
		if(m_pStrategyItem == NULL) return;
		int nRetValue = m_pStrategyItem->Create(this, (long)m_pStrategyItem,stArrayPath,m_pChartInterface);
		if(nRetValue==-1) 
		{
			m_pStrategyItem->DestoryWindow();
			delete m_pStrategyItem;
			m_pStrategyItem = NULL;
		}
	}
	m_DlgTrade->SetStrategyItem(m_pStrategyItem);
	///////////////////////////�����ƾ��////////////////////////////////

///////////////�ð�����ȭ �� ��������ȭ ��ƾ //////////////////////////////
	BOOL bRetValue;
	if(m_nOptimizerDiv == INDEX_OPTIMIZER)
	{
		for(int nIndx =0;nIndx< stArraySTPath.GetSize();nIndx++)
		{
			szSTPath = stArraySTPath.GetAt(nIndx);
			bRetValue = m_pStrategyItem->LoadST(szSTPath,m_stCode,m_nCount,m_stEndDate); //

			if(bRetValue)
			{
				m_pStrategyItem->Run();
			}
		}
	}
	if(m_nOptimizerDiv == TIME_OPTIMIZER)
	{
		//�ð�����ȭ ���� �� ���۰� �������� �����´�.
		int BongStart = GetDlgItemInt(IDC_EDIT_START);
		int BongEnd   = GetDlgItemInt(IDC_EDIT_END);
		int BongIncrease = GetDlgItemInt(IDC_EDIT_ADV);
		//���� ���� 
		if(BongStart > BongEnd || BongEnd < BongIncrease || BongEnd ==0 || BongStart ==0)
		{
			{
				m_pStrategyItem->DestoryWindow();
				delete m_pStrategyItem;
				m_pStrategyItem = NULL;
			}
			AfxMessageBox("�� �Է°��� �߸� �����Ǿ����ϴ�. �ٽ� �������ֽʽÿ�");
			return;
		}

		if(BongStart >= 1000 || BongEnd >= 1000 || BongIncrease >= 1000)
		{
			{
				m_pStrategyItem->DestoryWindow();
				delete m_pStrategyItem;
				m_pStrategyItem = NULL;
			}		
			AfxMessageBox("1000 �̻��� ���� ����� �� �����ϴ�. �ٽ� �Է¹ٶ��ϴ�.");
			return;
		}

		for(int nBong = BongStart; nBong<=BongEnd; nBong+=BongIncrease)
		{   m_nBong = nBong;
			m_pStrategyItem->SetTimeBong(m_nBong); // ���� ���� ����

			for(int nIndx =0;nIndx< stArraySTPath.GetSize();nIndx++)
			{
				szSTPath = stArraySTPath.GetAt(nIndx);
				bRetValue = m_pStrategyItem->LoadST(szSTPath,m_stCode,m_nCount/nBong,m_stEndDate); //
				if(bRetValue)
				{
					m_pStrategyItem->Run();
				}
			}
		}
	}

	////////////Calculate SendValue//////////////////////////
	if(m_nOptimizerDiv == VAL_OPTIMIZER)
	{
		m_pDlgTimeSetup->SetInputData();//����ȭ ������ ���� �����Ѵ�.
		bRetValue = m_pStrategyItem->LoadST(m_stPath,m_stCode,m_nCount,m_stEndDate); //
		m_ValueOptimizerArray.Copy(m_pDlgTimeSetup->m_ValueOptimizerArray);//����� ������ �����´�
		int nSize = m_ValueOptimizerArray.GetSize();
		// nSize�� 0�ΰ��� �Է°��� �ʿ���� �����.
		//if(!nSize) 
		//{
		//	m_pStrategyItem->DestoryWindow();
		//	delete m_pStrategyItem;
		//	m_pStrategyItem = NULL;
		//	AfxMessageBox("�۾���...");
		//	return;
		//}
		if(nSize > 0) 
		{
			ST_VALOTM stValOtm;
			for(int nValIndx = 0; nValIndx < nSize; ++nValIndx)
			{
				stValOtm = m_ValueOptimizerArray.GetAt(nValIndx);
				if(stValOtm.nStart >= 1000 || stValOtm.nEnd >= 1000 || stValOtm.nAdv >= 1000)
				{
					{
						m_pStrategyItem->DestoryWindow();
						delete m_pStrategyItem;
						m_pStrategyItem = NULL;
					}		
					AfxMessageBox("1000 �̻��� ���� ����� �� �����ϴ�. �ٽ� �Է¹ٶ��ϴ�.");
					return;
				}
			}
		}

		ST_VALOTM stVal;
		double nOne =0;
		CArray<double,double> *nStart = new CArray<double,double>;
		double nRoof=1;
		CStringArray stArraySendData;
		int i = 0;
		for( i=0;i<m_ValueOptimizerArray.GetSize();i++)//�� �������� ���մϴ�.
		{
			stVal = m_ValueOptimizerArray.GetAt(i);
			nStart->Add(stVal.nStart);//������ �������� ���� ������ �ִ´�.

			double nRoof1;
			if(stVal.nEnd == stVal.nStart)
				nRoof1 = 1;
			else
				nRoof1 = ((stVal.nEnd - stVal.nStart)/stVal.nAdv)+1; 
			nRoof *= nRoof1;
		}

		int nFlag = 0;// ���� ������ �� �迭�� Index �� ������ �ִ�.
		for(i =0; i<(int)nRoof;i++)
		{
			if(nStart->GetAt(nFlag) > m_ValueOptimizerArray.GetAt(nFlag).nEnd)
			{
				nStart->SetAt(nFlag,m_ValueOptimizerArray.GetAt(nFlag).nStart);//������ ó�������� ������ ����..
				++nFlag; //
				if(nFlag == nSize)
				{
					nFlag = 0;
				}
				if(nFlag<nSize)//�� �������� �ϳ� ���� ��Ű��..
				{
					for(int k=nFlag;k<nStart->GetSize();k++)//������ų���� ã���ϴ�.
					{
						double val2= nStart->GetAt(k);
						val2+= m_ValueOptimizerArray.GetAt(k).nAdv;
						if(val2 <= m_ValueOptimizerArray.GetAt(k).nEnd)//������Ű�� Break
						{
							nStart->SetAt(k,val2);
							break;
						}
						else//���������� ũ�ٸ� Start������
						{
							nStart->SetAt(k,m_ValueOptimizerArray.GetAt(k).nStart);
						}
					}
					nFlag = 0;
				}
			}

			CString stOption;
			for(int j =0;j<nStart->GetSize();j++)//��·�ƾ 
			{
				CString stValue;
				double Val = nStart->GetAt(j);//���� ������� �����Ѵ�.
				stValue.Format("%s%d=%.2f,",INPUTVALUE, j, Val);//GridŬ���ÿ� �ε��ؾ� �ϹǷ� Wnd�� �����س��´�.
				stOption += stValue;
			}

			stOption.Delete(stOption.GetLength()-1,1);
			if(bRetValue)
			{
				m_pStrategyItem->SetSendValString(stOption);
				m_pStrategyItem->SetValOptimizer((LONG)nStart);
				bRetValue = m_pStrategyItem->LoadST(m_stPath,m_stCode,m_nCount,m_stEndDate); //
				m_pStrategyItem->Run(stOption);
			}
		 
			if( m_ValueOptimizerArray.GetAt(nFlag).nAdv==0 && nSize > nFlag+1)
				nFlag++;
			stOption ="";
			double val = nStart->GetAt(nFlag);
			val+= m_ValueOptimizerArray.GetAt(nFlag).nAdv;		
			nStart->SetAt(nFlag,val);

		}
		
		nStart->RemoveAll();
		delete nStart;
	}
}
//���⼭ �ε带 ����...
LRESULT CDlgOptimizer::OnSTDataManagerMessagge( WPARAM wParam, LPARAM lParam)
{
	//����ȭ �ʱ�ȭ ���� ����
	m_pStrategyItem = (CIStrategyItem*)wParam;	
	ST_DRAWINFO* pInfo = (ST_DRAWINFO*)lParam;
	CPtrArray* pPtrArrayPlot = pInfo->pPtrArraySDataItem;
	m_pDataItem = (CISDataItem *)pPtrArrayPlot->GetAt(0);
	long nTotalData	= m_pDataItem->GetSize();
	m_pStrategyItem->Stop();
	m_pStrategyItem->CalculateStatisticalInfo(0, nTotalData-1);//�����͸� ��ü��..
	
	if(m_bGridClick)//�׸��� Ŭ���ÿ� ���� �ܿ� �ٷ� �����͸� ������� �Ѵ�.
	{
		m_DlgResult->SetDataSpreadGrid();
		//m_DlgResult->OpenWebView("\\IndexOptimizerData.htm");
		m_DlgTrade->SetDataSpreadGrid();
		//m_DlgTrade->OpenWebView("\\IOTradeData.htm");
		return 0L;
	}	
	//�׸��忡 �߰� ��ƾ 
	////// ������ �� ������ �߰� �� �ð��� ���� ���� �׸��忡 �߰� ��ƾ 
	CString str;
	int Plus = 0; // �� ����ȭ ���� �׸��忡 �׸��� Column �� �ٸ��Ƿ� �߰� 
	if(m_nOptimizerDiv == INDEX_OPTIMIZER || m_nOptimizerDiv== TIME_OPTIMIZER)
	{
		m_GridTap.InsertRow("");
		//@@061208 ������������ ����.[[
		// index�� ������ ���� �Ѱ��ָ� �������� �Ѿ��, 0�̳� ��� ��ȿ�� ���� �ѱ�� �ش�sd�� �̸��� �Ѱ��ش�.
		// �׷��� -1������ �ѱ�.
		//str=m_pStrategyItem->GetName(0);//���������ɴϴ�..
		//str.Delete(str.GetLength()-3, 3);

		str=m_pStrategyItem->GetName(-1); //������
		int nEndPos = -1;
		if((nEndPos = str.Find(gSTGubunKey)) != -1)
			str = str.Left(nEndPos);
		if((nEndPos = str.Find('.')) != -1)
			str= str.Left(nEndPos);
		//@@061208 ������������ ����.]]
		m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,1,str);
		if(m_nOptimizerDiv == TIME_OPTIMIZER)//������ ���� 
		{
			str.Format("%d",m_pStrategyItem->GetTimeBong());
			m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,2,str);
			Plus =1;
		}
	}
	if(m_nOptimizerDiv == VAL_OPTIMIZER)
	{	//"%s%d=%lf,
		m_GridTap.InsertRow("");
		str.Format("%s",m_pStrategyItem->GetValOptimizer());
		m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,1,str);//����������
		CArray<double,double> *ValData = (CArray<double,double>*)m_pStrategyItem->GetValOptimizer();
		ST_VALOTM stVal = 	m_ValueOptimizerArray.GetAt(0);
		int j = 0;
		for( j =0;j<ValData->GetSize();j++)//��·�ƾ 
		{
			CString stValue;
			double Val = ValData->GetAt(j);//���� ������� �����Ѵ�.
			if(stVal.nDataType ==2)
				stValue.Format("%.2f",Val);
			else
				stValue.Format("%d",(INT)Val);
			m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,j,stValue);//����������
		}
		m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,j,m_pStrategyItem->GetSendValString());
		Plus =j-1;
	}
	//��ǥ����ȭ���� ������ ��������?? �׸��忡 �� �ѷ��ش�.3,9,4,5,7,8,0,18,15 
	////////�ð��� ���� ����ȭ ���� ���� /////////////////////
	double Profit = m_pStrategyItem->GetStatisticalInfo(PROFITLOSSALL);//�Ѽ���
	str.Format("%d",(int)Profit);
	str = m_DlgResult->GetComma(str);
	m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,2+Plus,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	Profit = m_pStrategyItem->GetStatisticalInfo(PROFITLOSSAVG);//��ռ���
	str.Format("%d",(int)Profit);
	str = m_DlgResult->GetComma(str);
	m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,3+Plus,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	Profit = m_pStrategyItem->GetStatisticalInfo(PROFITALL);//������
	str.Format("%d",(int)Profit);
	str = m_DlgResult->GetComma(str);
	m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,4+Plus,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	Profit = m_pStrategyItem->GetStatisticalInfo(LOSSALL);//�Ѽս�
	str.Format("%d",(int)Profit);
	str = m_DlgResult->GetComma(str);
	m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,5+Plus,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	Profit = m_pStrategyItem->GetStatisticalInfo(MAXPROFIT);//�ִ�����
	str.Format("%d",(int)Profit);
	str = m_DlgResult->GetComma(str);
	m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,6+Plus,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	//////////////////////////
	if(m_nOptimizerDiv == INDEX_OPTIMIZER)//���� ����ȭ�ÿ��� �ʿ��մϴ�.
	{
		Profit = m_pStrategyItem->GetStatisticalInfo(MAXLOSS);//�ִ�ս�
		str.Format("%d",(int)Profit);
		str = m_DlgResult->GetComma(str);
		m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,7,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
		Profit = m_pStrategyItem->GetStatisticalInfo(PROFITRATEACCMULATED);//���ͷ�
		str.Format("%.2f",Profit);
		str = m_DlgResult->GetComma(str);
		m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,8,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
		Profit = m_pStrategyItem->GetStatisticalInfo(PROFITLOSSRATEALL);//Profit Factor
		str.Format("%.2f",Profit);
		str = m_DlgResult->GetComma(str);
		m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,9,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
		Profit = m_pStrategyItem->GetStatisticalInfo(RATEPROFIT);//�·�
		str.Format("%.2f",Profit);
		str = m_DlgResult->GetComma(str);
		m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,10,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
		Profit = m_pStrategyItem->GetStatisticalInfo(CNTALL);//�ŷ���
		str.Format("%d",(int)Profit);
		str = m_DlgResult->GetComma(str);
		m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,11,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	}
	//AutoSizeColumns();
	m_GridTap.Refresh();
	return 0;
}
BOOL CDlgOptimizer::CalculateSendData()
{
	//������ ���ϱ�
//	CString Data,strD;
//	m_strArSendData.RemoveAll();
//	for(int k=0;k<m_strNum.GetSize();k++)
//	{
//		m_arrayValue.GetAt(k)->stEdit->GetWindowText(strD);
//		CString st;
//		st.Format("%s%d=%s,",INPUTVALUE, k, strD);
//		Data+=st;
//	}
//	Data.Delete(Data.GetLength()-1);
//	m_strArSendData.Add(Data);
//	return TRUE;

//	CStringArray NowValue;
//	CString SendValue;
//	int nValue = 1;
//	int nRow=0;
//	int MaxData = 0;
//	int MaxDataIndex = 0;
//	m_ArGridData.RemoveAll();
////	m_strArSendData.RemoveAll();
//	m_ArstrGridData.RemoveAll();
//	m_ArRunvalue.RemoveAll();
//	//m_strNum�� �ٽ� �����;� �Ѵ�!!!
//	for(int i=0;i<m_strNum.GetSize();i++)
//	{
//		ST_RUNVALUESET ValueSet;
//		m_arrayValue.GetAt(i)->stEdit->GetWindowText(SendValue);
//		//Add�� �Ҷ�... SendValue�� ���������� �ľ��Ͽ� 0�̳� ���ڿ��� �����Ѵ�.			
//		if(IsNumber(SendValue,0))
//		{
//			if(MaxData == 0) MaxDataIndex =i; //MaxData�� row�� ù �迭�� �ִ밪�� ��Ÿ����.
//			int nValue = atoi(SendValue);//
//			ValueSet.start = 1;
//			ValueSet.end = nValue*2;
//			ValueSet.bNumber = TRUE;
//			NowValue.Add("1");
//			MaxData++;
//		}
//		else
//		{
//			ValueSet.start = ValueSet.end = 0;
//			ValueSet.bNumber = FALSE;
//			NowValue.Add(SendValue);
//		}
//		m_ArRunvalue.Add(ValueSet);
//	}
//
//	ST_RUNVALUESET ValueSet;
//	nRow =NowValue.GetSize()-1;
//	if(nRow<0) return FALSE;
//	ValueSet = m_ArRunvalue.GetAt(nRow);
//	int prevData;
//	//��ƾ ���� 
//	for(;;)
//	{
//		SendValue ="";
//		ValueSet = m_ArRunvalue.GetAt(nRow);
//		
//		if(ValueSet.end <=nValue ||ValueSet.bNumber!=TRUE)
//		{
//			int UpValue;
//			--nRow;
//
//			if(nRow<0) break;
//			
//			SendValue = NowValue.GetAt(nRow);
//			if(IsNumber(SendValue,0))
//				UpValue = atoi(NowValue.GetAt(nRow));
//			else
//				continue;
//
//			ValueSet = m_ArRunvalue.GetAt(nRow);
//			++UpValue;
//			if(ValueSet.end <= UpValue) continue;
//			
//			for(int l=nRow+1; l < NowValue.GetSize();l++)
//			{
//				SendValue = NowValue.GetAt(l);
//				if(IsNumber(SendValue,0))
//					NowValue.SetAt(l,"1");//��ü1����..
//				else
//					continue;
//			}
//			SendValue.Format("%d",UpValue);
//			NowValue.SetAt(nRow,SendValue);
//			nRow = NowValue.GetSize()-1;
//			nValue = 1;
//		}
//		else
//		{
//			//���⼭ String�� ��������....
//			//strAdditionalItem.Format("%s%d=%s",INPUTVALUE(��ũ��),
//			//nIndexAdditional(����),strPacketName(������)); m_strArSendArray�� ����..
//			prevData = nValue;
//			CString strAdditionalItem, strByun;
//			SendValue.Format("%d",nValue);
//			if(nRow== MaxDataIndex) break;//���������� ������ �����ؾ� �Ѵ�.
//			NowValue.SetAt(nRow,SendValue);
//			for(int j=0;j<NowValue.GetSize();j++)
//			{
//				CString str;
//				str = NowValue.GetAt(j);
//
//			//	if(IsNumber(str,0))
//			//	{
//					str.Format("%s%d=%s,",INPUTVALUE, j, str);
//					strAdditionalItem+=str;
//			//	}
//					str = NowValue.GetAt(j);
//				if(IsNumber(str,0)){
//				str.Format(" [%s]",str);
//				strByun+=str;
//				}
//
//			}
//			strAdditionalItem.Delete(strAdditionalItem.GetLength()-1);
//		//	AfxMessageBox(strAdditionalItem);
//			ST_STRGRIDDATA strData;
//			strData.strGridData = strAdditionalItem;
//			strData.Byunsu = strByun;
//			m_ArstrGridData.Add(strData);
//		//	m_strArSendData.Add(strAdditionalItem);
//			++nValue;
//		
//		}
//	}
//	//Progress Init
//	m_CtlProgress.SetRange(0,m_ArstrGridData.GetSize());
	return TRUE;
}

LRESULT CDlgOptimizer::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgOptimizer::OnTimer(UINT nIDEvent) 
{
	
	// TODO: Add your message handler code here and/or call default
//	long Size = m_ArstrGridData.GetSize();
//
//	if(m_lSendNum == Size||m_lSendNum > Size)
//	{
//	//	AfxMessageBox("���̰ŵ�");
//		KillTimer(m_nTimer);		
//		m_pStrategyItem->Stop();
//		m_lSendNum++;
//		m_CtlProgress.SetPos(m_lSendNum);
//		GetDlgItem(IDC_BTN_OPT)->ShowWindow(TRUE);
//		GetDlgItem(IDC_BTN_STOP)->ShowWindow(FALSE);
//		SpreadGridData();
//		SetControlState(TRUE);
//		CDialog::OnTimer(nIDEvent);
//		return;
//	}
//	BOOL s = KillTimer(m_nTimer);
//	
//	m_pStrategyItem->Stop();
//	ST_STRGRIDDATA GridData;
//	GridData = m_ArstrGridData.GetAt(m_lSendNum);
//	BOOL d=	m_pStrategyItem->Run(GridData.strGridData);
//
////	CString str;
////	str.Format("Data: %s Num:%d",m_strArSendData.GetAt(m_lSendNum),m_lSendNum );
////	GetDlgItem(IDC_STATIC_TEST)->SetWindowText(str);
////	SetDlgItemText(IDC_STATIC_TEST, str);
//	m_lSendNum++;
//	m_CtlProgress.SetPos(m_lSendNum);
//	m_bReceive = FALSE;

	CDialog::OnTimer(nIDEvent);
}

BOOL CDlgOptimizer::IsNumber(CString str, int nIndex)
{
	BOOL bNumber;
	TCHAR c = str.GetAt(nIndex);
	if(c>='0' && c<='9')
		bNumber = TRUE;
	else
		bNumber = FALSE;
	return bNumber;
}

void CDlgOptimizer::SpreadGridData()
{

//	ST_GRIDVALUEDATA GridData;
//	ST_STRGRIDDATA  strGridData;
//	CString str;
//	int nSize =m_ArGridData.GetSize();
//	if(nSize == 0) return;
//	//m_GridStatus.SetRowCount(nSize+1);
/////////��� �׸��� ////////////////////////////
////ó���� ���õ� ������ �����´�. 
//	GridData = m_ArGridData.GetAt(nSize/2);
//	strGridData = m_ArstrGridData.GetAt(nSize/2);
//	str.Format("%d",nSize/2);
//	m_GridTap.SetItemText(1,0, str);
//	m_GridTap.SetItemText(1,1, strGridData.Byunsu);
//	m_GridTap.SetItemText(1,2, GridData.RateCurrent);
//	m_GridTap.SetItemText(1,3, GridData.LossSavg);
//	m_GridTap.SetItemText(1,4, GridData.ProfitAll);
//	m_GridTap.SetItemText(1,5, GridData.LossAll);
//	m_GridTap.SetItemText(1,6, GridData.MaxProfit);
//	m_GridTap.SetItemText(1,7, GridData.MaxLoss);
//	m_GridTap.SetItemText(1,8, GridData.RateAccm);
//	m_GridTap.SetItemText(1,9, GridData.ProfitFactor);
//	m_GridTap.SetItemText(1,10,GridData.RateProfitLoss);
//	m_GridTap.SetItemText(1,11,GridData.CntAll);
//	
//	
//	int dd = m_ArstrGridData.GetSize();
///////�ϴ� �׸��� ////////////////////////////////
	/*for(int i = 0; i<nSize;i++)
	{
		GridData = m_ArGridData.GetAt(i);
	
		str.Format("%d",i);
		strGridData = m_ArstrGridData.GetAt(i);
		m_GridStatus.SetItemText(i+1,0, str);
		m_GridStatus.SetItemText(i+1,1, strGridData.Byunsu);
		m_GridStatus.SetItemText(i+1,2, GridData.RateCurrent);
		m_GridStatus.SetItemText(i+1,3, GridData.LossSavg);
		m_GridStatus.SetItemText(i+1,4, GridData.ProfitAll);
		m_GridStatus.SetItemText(i+1,5, GridData.LossAll);
		m_GridStatus.SetItemText(i+1,6, GridData.MaxProfit);
		m_GridStatus.SetItemText(i+1,7, GridData.MaxLoss);
		m_GridStatus.SetItemText(i+1,8, GridData.RateAccm);
		m_GridStatus.SetItemText(i+1,9, GridData.ProfitFactor);
		m_GridStatus.SetItemText(i+1,10,GridData.RateProfitLoss);
		m_GridStatus.SetItemText(i+1,11,GridData.CntAll);
	}

	m_GridStatus.SortTextItems(2,TRUE);*/
	m_GridTap.Refresh();
	//m_GridStatus.Refresh();
}


void CDlgOptimizer::OnBtnStop() 
{
	// TODO: Add your control notification handler code here
	m_pStrategyItem->Stop();
	KillTimer(m_nTimer);		
	long Size = m_ArstrGridData.GetSize();
	m_lSendNum = Size;
	//m_CtlProgress.SetPos(m_lSendNum);
	GetDlgItem(IDC_BTN_STOP)->ShowWindow(FALSE);
	GetDlgItem(IDC_BTN_OPT)->ShowWindow(TRUE);
	//SpreadGridData();
	SetControlState(TRUE);
}

void CDlgOptimizer::SetControlState(BOOL State)
{
	//if(State)// ��Ʈ�ѵ��� Enable ��Ų��. 
	//{
	//	GetDlgItem(IDC_COM_INDEX)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_COM_MEDO)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_COM_MESU)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_MEDOGIJUN)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_SPIN_MEDO)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_MESUGIJUN)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_SPIN_MESU)->EnableWindow(TRUE);
	//	ST_VALUESET *ValueSet;
	//	for(int i =0 ;i< m_arrayValue.GetSize();i++)
	//	{
	//		ValueSet = m_arrayValue.GetAt(i);
	//		ValueSet->stEdit->EnableWindow(TRUE);
	//		ValueSet->stSpinBtn->EnableWindow(TRUE);
	//	}
	//}
	//else
	//{
	//	GetDlgItem(IDC_COM_INDEX)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_COM_MEDO)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_COM_MESU)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_MEDOGIJUN)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_SPIN_MEDO)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_MESUGIJUN)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_SPIN_MESU)->EnableWindow(FALSE);
	//	ST_VALUESET *ValueSet;
	//	for(int i =0 ;i< m_arrayValue.GetSize();i++)
	//	{
	//		ValueSet = m_arrayValue.GetAt(i);
	//		ValueSet->stEdit->EnableWindow(FALSE);
	//		ValueSet->stSpinBtn->EnableWindow(FALSE);
	//	}
	//}
}

BOOL CDlgOptimizer::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	//Grid���� �̺�Ʈ �߻� �Ҷ� ���� SendMessage�� ����ü�� ������ ä�� 
	//���̵�� �Բ��Ϸ� ������.
	
	NM_GRIDVIEW *pNmgv = (NM_GRIDVIEW *)lParam;
	//GridŬ���� �ű⿡ ���� ������ �ε��ؾ��Ѵ�.
	if(wParam==IDC_GRID2)
	{
		if(pNmgv->hdr.code==NM_CLICK)
		{
			if(pNmgv->iRow < 0)
				return CDialog::OnNotify(wParam, lParam, pResult);
			
			m_bGridClick = TRUE;
			if(pNmgv->iRow ==0)
			{
				//m_GridTap.SetSortColumn(pNmgv->iColumn);
				if(m_nOptimizerDiv == INDEX_OPTIMIZER && pNmgv->iColumn==1 )
					m_GridTap.SortItems(m_GridTap.pfnCellTextCompare,pNmgv->iColumn,!m_GridTap.GetSortAscending());
				else
					m_GridTap.SortItems(m_GridTap.pfnCellNumericCompare,pNmgv->iColumn,!m_GridTap.GetSortAscending());
				m_GridTap.Refresh();
				return CDialog::OnNotify(wParam, lParam, pResult);
			}
			if(m_nOptimizerDiv!=VAL_OPTIMIZER)
			{
				CString stSTName;
				stSTName = m_GridTap.GetItemText(pNmgv->iRow,1);//�������� �о�´�. 
				if(stSTName=="") return CDialog::OnNotify(wParam, lParam, pResult);
				CString szSTPath;
				int nListCount = m_listST.GetCount();
				STMNG_ITEMINFO* pItemInfo = NULL;
				for(int nIndx = 0; nIndx < nListCount; ++nIndx)
				{
					if(pItemInfo = (STMNG_ITEMINFO*)m_listST.GetItemData(nIndx))
					{	
						szSTPath = pItemInfo->szSTName;
						int nEndPos = -1;
						if((nEndPos = szSTPath.Find(gSTGubunKey)) != -1)
							szSTPath = szSTPath.Left(nEndPos);
						if(szSTPath == stSTName)
						{
							szSTPath	= pItemInfo->szSTPath +"\\"+ pItemInfo->szSTName + ".st";
							
							break;
						}
					}
				}
				if(m_nOptimizerDiv == TIME_OPTIMIZER)//�ش������ �����Ѵ�.
				{
					m_pStrategyItem->SetTimeBong(atoi(m_GridTap.GetItemText(pNmgv->iRow,2)));
				}
				//Ŭ���Ҷ� ���� �Ʒ��� ���� ä��� ���� �ش� ������ �ٽ� �ε��Ͽ� Run�ؾ� �Ѵ�.
				BOOL bRetValue = m_pStrategyItem->LoadST(szSTPath,m_stCode,m_nCount,m_stEndDate); //
				if(bRetValue)
				{
					m_pStrategyItem->Run();
				}
			}else{//���� ����ȭ���� ���.....
				CString stSendVal;
				stSendVal = m_GridTap.GetItemText(pNmgv->iRow,m_STValNum);
				BOOL bRetValue = m_pStrategyItem->LoadST(m_stPath,m_stCode,m_nCount,m_stEndDate);
				if(bRetValue)
				{
					m_pStrategyItem->Run(stSendVal);
				}
			}

		}  
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CDlgOptimizer::SetSTMessageType(short Smt)
{
	m_STMessageType = Smt;
}

void CDlgOptimizer::InitSkin()
{
	//m_BtnDef.SetSkin(IDB_BITMAP1_up,IDB_BITMAP1_over,IDB_BITMAP1_up,IDB_BITMAP1_dis);
	//m_BtnStop.SetSkin(IDB_BITMAP3_up,IDB_BITMAP3_over,IDB_BITMAP3_up,IDB_BITMAP3_dis);
	//m_BtnOpt.SetSkin(IDB_BITMAP2_up,IDB_BITMAP2_over,IDB_BITMAP2_up,IDB_BITMAP2_dis);
}

void CDlgOptimizer::OnBnClickedBtnAddall()
{
	LIST_STMNG_ITEMINFO STListInfo;
	STListInfo.RemoveAll();
	
	m_pSTLoadMnger->GetFolderST((DWORD)&STListInfo);

	STMNG_ITEMINFO* pItemInfo = NULL;
	CString szSTName;
	POSITION pos = STListInfo.GetHeadPosition();
	int nAtPos = -1;
	while(pos) {
		if(pItemInfo = (STMNG_ITEMINFO*)STListInfo.GetNext(pos)) 
		{
			szSTName = pItemInfo->szSTName;
			nAtPos = szSTName.Find(gSTGubunKey);
			if(nAtPos > 0)
				szSTName = szSTName.Mid(0, nAtPos);

			//if(m_listST.FindString(0,szSTName)==LB_ERR)//ã�� �������� �߰�!!KHD
			//{
				m_listST.AddString(szSTName);
				m_listST.SetItemData(m_listST.GetCount()-1, (DWORD)pItemInfo);
			//}
		}
	}
}

void CDlgOptimizer::OnBnClickedBtnAddone()
{
	LIST_STMNG_ITEMINFO STListInfo;
	STListInfo.RemoveAll();

	m_pSTLoadMnger->GetItemST((DWORD)&STListInfo);
	
	STMNG_ITEMINFO* pItemInfo = NULL;
	CString szSTName;
	POSITION pos = STListInfo.GetHeadPosition();
	int nAtPos = -1;
	while(pos) {
		if(pItemInfo = (STMNG_ITEMINFO*)STListInfo.GetNext(pos)) {
			szSTName = pItemInfo->szSTName;
			nAtPos = szSTName.Find(gSTGubunKey);
			if(nAtPos > 0)
				szSTName = szSTName.Mid(0, nAtPos);

			//if(m_listST.FindString(0,szSTName)==LB_ERR)//ã�� �������� �߰�!!KHD
			//{
				m_listST.AddString(szSTName);
				m_listST.SetItemData(m_listST.GetCount() - 1, (DWORD)pItemInfo);
			//}
		}
	}
}

void CDlgOptimizer::OnBnClickedBtnDelall()
{
	int nListCount = m_listST.GetCount();
	STMNG_ITEMINFO* pItemInfo = NULL;
	for(int nIndx = 0; nIndx < nListCount; ++nIndx)
	{
		if(pItemInfo = (STMNG_ITEMINFO*)m_listST.GetItemData(nIndx)) {
			delete pItemInfo;
			pItemInfo = NULL;
		}
	}

	m_listST.ResetContent();
}

void CDlgOptimizer::OnBnClickedBtnDelone()
{
	
	int nCurSel = m_listST.GetCurSel();
	STMNG_ITEMINFO* pItemInfo = (STMNG_ITEMINFO*)m_listST.GetItemData(nCurSel);
	m_listST.DeleteString(nCurSel);
}

//�ð�, ����, ���� ����ȭ�� ���� ��Ų�� �޸� �����̵ȴ�. 
void CDlgOptimizer::CreateTImeSetupDlg()
{

	switch(m_nOptimizerDiv) {
	case INDEX_OPTIMIZER:
		{
			GetDlgItem(IDC_STATIC_START)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_END)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_ADV)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_START)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_ADV)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_END)->ShowWindow(SW_HIDE);
			// m_listST.SetWindowPos(NULL,5,310,190,150,SWP_SHOWWINDOW);
			
		}
		break;
	case VAL_OPTIMIZER:
		{
			m_pDlgTimeSetup = new CDlgTimeSetup;

			BOOL bRet = m_pDlgTimeSetup->Create(CDlgTimeSetup::IDD, this);

			if(bRet)
			{
				CRect rect;
				GetDlgItem(IDC_STATIC_TIME)->GetWindowRect(rect);
				ScreenToClient(rect);
				rect.top += 1;
				m_pDlgTimeSetup->MoveWindow(rect);
				m_pDlgTimeSetup->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BTN_ADDALL)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_ADDONE )->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_DELALL )->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_DELONE )->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_LIST_ST )->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_START)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_END)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_ADV)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDIT_START)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDIT_ADV)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDIT_END)->ShowWindow(SW_HIDE);

			}
		}
		break;
	case TIME_OPTIMIZER://Default�̱� ������..
		{
			GetDlgItem(IDC_STATIC_START)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_END)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ADV)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_START)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_ADV)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_END)->ShowWindow(SW_SHOW);
			
			SetDlgItemText(IDC_EDIT_START,"1");
			SetDlgItemText(IDC_EDIT_END,"5");
			SetDlgItemText(IDC_EDIT_ADV,"1");
		}
		break;
	default:
		AfxMessageBox("����ȭ�� �߸� ���õǾ����ϴ�.");
	}
	
}

void CDlgOptimizer::DestoryTImeSetupDlg()
{
	if(m_pDlgTimeSetup) delete m_pDlgTimeSetup;
	m_pDlgTimeSetup=NULL;
}

//íƮ������ �����ϴ°�....
// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
int CDlgOptimizer::CSTUserData::GetChartData(LONG& dwDataTime, LONG& dwDataOpen, LONG& dwDataHigh, LONG& dwDataLow, LONG& dwDataClose,LONG& dwVolume, LONG& dwDataExt, int& nBong)
{
	MYMETHOD_PROLOGUE(CDlgOptimizer, STUserData)
	ISTInterface* pSTInterface = (ISTInterface*)pThis->m_pChartInterface;
	if(pSTInterface)
	{
		//�ٲ�ġ�� ��ƾ �ʿ�!!
		ISTAnalysisInput* pMng = (ISTAnalysisInput*)pSTInterface->GetInterface(UUSTID_ISTAnalysisInput);
		// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
		int nCnt = pMng->GetChartData(dwDataTime, dwDataOpen, dwDataHigh, dwDataLow, dwDataClose,dwVolume, dwDataExt);//����������
		nBong = pThis->m_pStrategyItem->GetTimeBong();//���� ���õ� ���� ������ ��� �մϴ�.
		if(nBong ==1) return 0;//1������ ��ƾ�� Ż�ʿ䰡 �����Ƿ�.. �� �״�� ��ȯ
		//�迭 �� ������Copy ����
		CSDataItem* dTimeDataTime		= new CSDataItem;
		CSTDataItem* dTimeDataOpen		= new CSTDataItem;
		CSTDataItem* dTimeDataHigh		= new CSTDataItem;
		CSTDataItem* dTimeDataLow		= new CSTDataItem;
		CSTDataItem* dTimeDataClose		= new CSTDataItem;

		double nBongOpen; 
		double nBongHigh;
		double nBongLow;
		double nBongClose;
		int nStartI;
		int nEnd = (nCnt-(nCnt%nBong)); //-> ������ �������� ���Ѵ�.
		for(int i = 0; i<nEnd ; i+=nBong)
		{	
			nStartI = i;
			nBongOpen	= ((CSTDataItem*)dwDataOpen)->GetAt(nStartI);
			nBongClose	= ((CSTDataItem*)dwDataClose)->GetAt(nStartI+nBong-1);
			//ó�� ���� �����Ѵ�.
			nBongHigh		=((CSTDataItem*)dwDataHigh)->GetAt(nStartI);
			nBongLow		=((CSTDataItem*)dwDataLow)->GetAt(nStartI);
			for(int j=0;j<nBong;j++)	
			{
				nBongHigh = max(nBongHigh, ((CSTDataItem*)dwDataHigh)->GetAt(nStartI+j));
				nBongLow  = min(nBongLow,  ((CSTDataItem*)dwDataLow)->GetAt(nStartI+j));
			}

			dTimeDataTime->Add(((CSDataItem*)dwDataTime)->GetAt(nStartI));
			dTimeDataOpen->Add(nBongOpen);
			dTimeDataHigh->Add(nBongHigh);
			dTimeDataLow->Add(nBongLow);
			dTimeDataClose->Add(nBongClose);
		}

		dwDataTime  = (LONG)dTimeDataTime;
		dwDataOpen	= (LONG)dTimeDataOpen;
		dwDataHigh	= (LONG)dTimeDataHigh;
		dwDataLow	= (LONG)dTimeDataLow;
		dwDataClose	= (LONG)dTimeDataClose;
		//////////////////////////////////////////////////////
		return nCnt;
	}
	return 0; // ������Count ����
}
LRESULT CDlgOptimizer::OnRmsgObItemDBClk(WPARAM wParam, LPARAM lParam)
{
	STMNG_ITEMINFO* pArg = (STMNG_ITEMINFO*)lParam;
	if(m_nOptimizerDiv == VAL_OPTIMIZER)
	{
		CString szRootDir = (LPCSTR)AfxGetPctrData(10);	// 10 : RootDir
		//CString strUserPath = szRootDir +"\\Data\\STData\\VarOptima.cfg";
		CString strUserPath = szRootDir +"\\Icss\\STData\\VarOptima.cfg";	//@Solomon
		CString szSTName;
		m_pDlgTimeSetup->Reset();
		szSTName = pArg->szSTName; 
		szSTName=szSTName.Mid(0, szSTName.Find(gSTGubunKey));
		if(m_pDlgTimeSetup->LoadCFG(strUserPath, szSTName)==FALSE)
		{
			AfxMessageBox("���� ����ȭ���� ����� �� ���� �����Դϴ�.");
			return 0L;
		}
		m_pDlgTimeSetup->ParseCFG();
		//���� ����Ŭ�� �� ���� ǮPath �����Ѵ�.
		m_stPath = pArg->szSTPath + "\\" +pArg->szSTName+".st";
		m_GridTap.SetRowCount(1);
		m_STValNum =m_pDlgTimeSetup->m_ValueOptimizerArray.GetSize();
		
		ReSize();
		
	}
	else{
		OnBnClickedBtnAddone();
	}

	/*m_xSTAnalysisInput.m_Info.nSTType		= pArg->nSTType;
	m_xSTAnalysisInput.m_Info.szSTName		= pArg->szSTName;
	m_xSTAnalysisInput.m_Info.szSTPath		= pArg->szSTPath;
	m_xSTAnalysisInput.m_Info.szSTRData		= pArg->szSTRData;	
	m_xSTAnalysisInput.m_Info.nFolderIndx	= pArg->nFolderIndx;
	m_xSTAnalysisInput.m_Info.nItemIndx		= pArg->nItemIndx;*/
	return 0L;
}

	

void CDlgOptimizer::UnLoadSTList()
{
	if(!m_pSTLoadMnger) return;

	typedef	void (FPSCW_DeleteInterfaceOfStrategy)(LONG dwKey, LONG dwType);
	if(hTradeInst == NULL) return;

	FPSCW_DeleteInterfaceOfStrategy* pFunc = (FPSCW_DeleteInterfaceOfStrategy*)GetProcAddress(hTradeInst,_T("SCW_DeleteInterfaceOfStrategy"));
	if(pFunc == NULL) return ;

	LONG dwType = 0;	//CSTLoadManager*
	if(m_pSTLoadMnger)
	{
		m_pSTLoadMnger->UnloadST();
		pFunc((LONG)m_pSTLoadMnger, dwType);
	}	
}


LRESULT CDlgOptimizer::OnRmsgGetDllSize(WPARAM wParam, LPARAM lParam)
{
	CPoint* pSize = (CPoint*)wParam;
	pSize->x = 865;
	pSize->y = 532;

	return 0;
}


void CDlgOptimizer::OnLbnDblclkListSt()
{
	OnBnClickedBtnDelone();
}
//@@@06.12.07[[


void CDlgOptimizer::LoadSTList_SSStyle()
{
	if(!m_pWndOutBar) return;

	m_STTree.m_szRootDir = m_strRootPath;
	m_STBasicTree.m_szRootDir = m_strRootPath;

	DWORD dwStyle = WS_CHILD | WS_CLIPCHILDREN | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS;
	CWnd* pParentWnd = m_pWndOutBar->GetCoutBarCtrl();

	UINT nID = 1000;
	BOOL bResult = m_STTree.Create(dwStyle, CRect(0, 0, 0, 0), pParentWnd, nID);
	m_STTree.LoadSTTree();

	nID++;
	bResult = m_STBasicTree.Create(dwStyle, CRect(0, 0, 0, 0), pParentWnd, nID);
	m_STBasicTree.LoadSTTree();

	m_pWndOutBar->AddFolderBar("�ʱ�", &m_STBasicTree);
	m_pWndOutBar->AddFolderBar("���", &m_STTree);

	m_pWndOutBar->SetSelFolder(0);
}
//LONG CDlgOptimizer::OnRmsgSTTreeItemClk(WPARAM wParam, LPARAM lParam)
//{	
//	return 0L;
//	//return OnRmsgObItemClk(wParam, lParam);
//}

LONG CDlgOptimizer::OnRmsgSTTreeItemDBClk(WPARAM wParam, LPARAM lParam)
{	
	return OnRmsgObItemDBClk(wParam, lParam);
}
//@@@06.12.07]]
