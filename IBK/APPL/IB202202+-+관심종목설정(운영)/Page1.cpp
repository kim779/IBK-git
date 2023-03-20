// Page1.cpp : implementation file
//

#include "stdafx.h"
#include "IB202201.h"
#include "Page1.h"
#include "page4.h"
#include "NewGroup.h"
#include "EditGroup.h"
#include "ShareGroupDlg.h"
#include "ioformat.h"
#include "imm.h"

#include "../../h/axisfire.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STANDARDNUM	11
#define	saveFILE	"portfolio.ini"
#define	tempFILE	"portfolio.ini.tmp"
//#define ucKOSPI		"0"
//#define ucKOSDAQ	"2"

// tree_setitemdata()
constexpr	int xALLCODE = 1, xKOSPI = 2, xKOSDAQ = 3, xFUTURE = 4, xCALLCODE = 5, xPUTCODE = 6;
constexpr	int xKOPSI200 = 7,  xKOSDAQ50 = 8, xKRX100 = 9, xFREECODE = 10, xETFCODE = 11, xELWCODE = 12, xELWBASE = 13, xSINJUCODE = 14;
constexpr	int xKOSPIJISU = 15, xKOSDAQJISU = 16, xTHEMACODE = 17, xGROUPCODE = 18, xINTEREST = 19;
constexpr	int xELWISSUE = 20, xELWASSET = 21;   //22�� ELW�� ǲ�� ���
constexpr	int	xRecommand = 22, xETFALLCODE = 31, xELWSelect = 40, xELWPB = 41;
constexpr	int xKOSPIFUTURE = 42, xSTOCKFUTURE = 43, xINTERESTRATEFUTURE = 44, xCURRENCYFUTURE = 45;
constexpr	int xKONEX = 46;	//2013.06.25 �ڳؽ� �߰�
constexpr	int xETNCODE = 47;	//2014.11.06 KSJ ETN �߰�
constexpr	int xSPACCODE = 48;	//2015.11.03 KSJ ���� �߰�
constexpr	int xKBc = 101, xSECc = 102, xSKc = 103, xPOSCOc = 104, xKTc = 105, xKEPCOc = 106, xHYUNDAIc = 107;
constexpr	int xKBp = 201, xSECp = 202, xSKp = 203, xPOSCOp = 204, xKTp = 205, xKEPCOp = 206, xHYUNDAIp = 207;
constexpr	int xKOSPIUP = 50, xKOSDAQUP = 51, xKOSPIex = 52, xKOSDAQex = 53;


constexpr struct _exKospi {
	char* code{};
	char* name{};
} exKospi[] = {
	{"101", "KOSPI200"},	{"401", "KOSPI100"},
	{"402", "KOSPI50"},	{"403", "KOSPI IT"},
};
const int cnt_exKospi = sizeof(exKospi) / sizeof(struct _exKospi);

constexpr struct _exKosdaq {
	char* code{};
	char* name{};
} exKosdaq[] = {
	//{"301", "KOSDAQ50"},		// 20070608
//	{"242", "�����̾�"},
	{"303", "KOSTAR"},
};

const int cnt_exKosdaq = sizeof(exKosdaq) / sizeof(struct _exKosdaq);

#define	HANGULMSK	0x80
#define	ChosungS	0xA4A1	// ��
#define	ChosungE	0xA4BE	// ��

#define	SK_JONGMOK	0	// ����˻�
#define	SK_WORD		1	// �ܾ�˻�

BOOL	m_listsort;

constexpr char* xxKOSPI[] = {
	 "",
	 "KGG01P", //�ڽ���         1
	 "KGZ01P", //�ڽ��� ������	2	
	 "KGZ02P", //�ڽ��� ������	3
	 "KGZ03P", //�ڽ��� ������	4
	 "KGS01P", //���ķ�ǰ	5
	 "KGS02P", //����, �Ǻ�	6
	 "KGS03P", //����, ����	7
	 "KGS04P", //ȭ��		8
	 "KGS05P", //�Ǿ�ǰ		9	
	 "KGS06P", //��ݼӱ���	10
	 "KGS07P", //ö���ױݼ�	11
	 "KGS08P", //���		12
	 "KGS09P", //����, ����	13
	 "KGS19P", //�Ƿ�����	14
	 "KGS10P", //������	15
	 "KGS11P", //�����		16
	 "KGS20P", //���Ⱑ����	17
	 "KGS12P", //�Ǽ���		18	
	 "KGS13P", //���â��	19
	 "KGS21P", //��ž�		20
	 "KGS14P", //������		21
	 "KGS15P", //����		22
	 "",
	 "KGS16P", //����		24
	 "KGS17P", //����		25
	 "KGS22P", //���񽺾�	26
	 "KGS18P"  //������		27
};

constexpr char* xxKOSDAQ[] = { "", "", "", "", "", "",

	 "QGS02P",    // ���� 6
	 "QGS03P",    // �Ǽ� 7
	 "QGS04P",	  // ���� 8
	 "",
	 "QGS31P",	  // ��� 10 
	 "QGS05P",    // ���� 11
	 "QGS06P",    // ��Ź�� 12
	 "QGS07P",	  // IT S / W & SVC  13
	 "QGS08P",	  // IT H / W  14      
	 "QGS09P",    // ���ķᡤ���  15 
	 "QGS10P",	  // �������Ƿ�  16
	 "QGS11P",    // ���� ����   17
	 "QGS12P",	  // ���ǡ���ü����  18
	 "QGS13P",	  // ȭ��         19
	 "QGS33P",	  // ����         20
	 "QGS14P",    // ��ݼ�	  21
	 "QGS15P",	  // �� ��        22
	 "QGS16P",    // ��衤���   23
	 "QGS17P",	  // �Ϲ��������� 24
	 "QGS18P",	  // �Ƿᡤ���б��   25
	 "QGS19P",	//   �����񡤺�ǰ   26
	 "QGS20P",  //   ��Ÿ ����        27
	 "QGS21P",  //   ��ż���       28
	 "QGS22P",  //   ��ۼ���       29
	 "QGS23P",	//   ���ͳ�	      30
	 "QGS24P",  //   ������������     31
	 "QGS25P",	//	����Ʈ����    32
	 "QGS26P",   //   ��ǻ�ͼ���    33
	 "QGS27P",	//   ������	      34
	 "QGS28P",  //	�������      35
	 "QGS29P",	//	�ݵ�ü        36
	 "QGS30P",        //IT��ǰ        37
	 "QGS87P",	//         
	 "",
	 "",	//		     39
	 "",	//		     40
	 "QGS34P",	//��������ȭ         41
	 "QGG03P",  //�ڽ��� �췮�����  42
	 "QGG04P",  //�ڽ��� ��ó�����  43
	 "QGG05P",  //�ڽ��� �߰߱����  44
	 "QGG06P"    //�ڽ��� ����������� 45 

};

int CALLBACK ListViewCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) 
{ 
	_listitem *pItem1 = (_listitem *) lParam1; 
	_listitem *pItem2 = (_listitem *) lParam2; 

	int	compare{};
	CString	name1, name2;

	switch ((int)lParamSort)
	{
	case 0:
		compare = lstrcmpi((char*)pItem1->code, (char*)pItem2->code); 
		break;
	case 1:
		name1 = CString((char*)pItem1->name); 
		name2 = CString((char*)pItem2->name); 
		compare = lstrcmpi(name1, name2);
		break;
	}

	if (m_listsort)
	{
		if (compare > 0)
			compare = -1;
		else if (compare < 0)
			compare = 1;
	}

	return compare;
}

int CALLBACK ListViewCompareFunc2(LPARAM lParam1, LPARAM lParam2, LPARAM lParam) 
{ 
	_listitem *pItem1 = (_listitem *) lParam1; 
	_listitem *pItem2 = (_listitem *) lParam2; 

	int	compare{};
	CString	name1, name2;

	const int  nCol = HIWORD(lParam);
	const BOOL bAsc = LOWORD(lParam);

	switch (nCol)
	{
	case 0:
		compare = lstrcmpi((char*)pItem1->code, (char*)pItem2->code); 
		break;
	case 1:
		name1 = CString((char*)pItem1->name); 
		name2 = CString((char*)pItem2->name); 
		compare = lstrcmpi(name1, name2);
		break;
	}

	if (bAsc)
	{
		if (compare > 0)
			compare = -1;
		else if (compare < 0)
			compare = 1;
	}

	return compare;
}

//�߰�
int CompareChar( const void *elem1, const void *elem2 ) 
{ 
    CString *str1 = (CString*)elem1;
    CString *str2 = (CString*)elem2;
    return str1->Compare((*str2));
} 

int compareSortName( const void *arg1, const void *arg2 )
{
	struct _sortStr* pLVal= (struct _sortStr*)arg1;
	struct _sortStr* pRVal= (struct _sortStr*)arg2;
	
	return pLVal->name.Compare(pRVal->name);
}

int compareSortName2(const void* arg1, const void* arg2)
{
	CITEMS* p1 = (CITEMS*)arg1;
	CITEMS* p2 = (CITEMS*)arg2;

	int iCmp;
	iCmp = strcmp(p1->name, p2->name);

	if (m_gListsort)
	{
		if (iCmp > 0)
			iCmp = -1;
		else if (iCmp < 0)
			iCmp = 1;
	}

	return iCmp;
}

int compareSortCode2(const void* arg1, const void* arg2)
{
	CITEMS* p1 = (CITEMS*)arg1;
	CITEMS* p2 = (CITEMS*)arg2;

	int iCmp;
	iCmp = strcmp(p1->code, p2->code);

	if (m_gListsort)
	{
		if (iCmp > 0)
			iCmp = -1;
		else if (iCmp < 0)
			iCmp = 1;
	}

	return iCmp;
}

/////////////////////////////////////////////////////////////////////////////
// CPage1 property page
[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CPage1, CPropertyPage)

CPage1::CPage1(CWnd* parent) : CPropertyPage(CPage1::IDD)
{
	//{{AFX_DATA_INIT(CPage1)
	//}}AFX_DATA_INIT

	m_parent = parent;
	m_activegno = -1;
	m_autoScroll = false;
	m_dragImage = nullptr;

	m_activePage = false;
	m_bAddAll = true;

	m_appendGI = _T("");
	m_selectGroup = 0;

	m_bCol0Asc = TRUE;
	m_bCol1Asc = FALSE;
	m_nPrevCol = 0;

	m_nCol0Asc   = -1;
	m_nCol1Asc   = -1;
	m_nPrevList2 = -1;

	isClickServerDN = FALSE;
	m_bIsEdit = FALSE;
	m_isDuplicateCode = FALSE;

	for(int i=0 ; i<MAXGROUP ; i++)
	{
		m_manageGroup[i][0] = "";
		m_manageGroup[i][1] = "";
		m_manageGroup[i][2] = "";
		m_manageGroup[i][3] = "";
	}


	m_isChangeSequence = false;

	m_bFirstDuplicateNO = false;	//2012.02.13 KSJ
	m_nResult = 0;					//2012.04.02 KSJ

	m_cicode.RemoveAll();
	m_cccode.RemoveAll();
	m_sfcode.RemoveAll();

	m_gListsort = FALSE;
}

CPage1::~CPage1()
{
// 	if (m_dragImage) delete m_dragImage;
// 
// 	ClearListitem(); 
// 	ClearInterest();
// 	
// 	ClearList2item();
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1)
	DDX_Control(pDX, IDC_MOVEBOTTOM, m_btnMoveBtm);
	DDX_Control(pDX, IDC_MOVETOP, m_btnMoveTop);
	DDX_Control(pDX, IDC_DELALL, m_btn_DelAll);
	DDX_Control(pDX, IDC_SHAREIMPORT, m_btn_ShareImport);
	DDX_Control(pDX, IDC_SHAREEXPORT, m_btn_ShareExport);
	DDX_Control(pDX, IDC_NEWGROUP, m_btn_newGroup);
	DDX_Control(pDX, IDC_MOVEUP, m_btn_MoveUp);
	DDX_Control(pDX, IDC_MOVEDN, m_btn_MoveDn);
	DDX_Control(pDX, IDC_BLANK, m_btn_Blank);
	DDX_Control(pDX, IDC_ALLSAVE, m_btn_AllSave);
	DDX_Control(pDX, IDC_ADDALL, m_btn_AddAll);
	DDX_Control(pDX, IDC_ADD, m_btn_Add);
	DDX_Control(pDX, IDC_EDITGROUP, m_editGroup);
	DDX_Control(pDX, IDC_DNLOADR, m_btn_dnLoadR);
	DDX_Control(pDX, IDC_DNLOAD, m_btn_dnLoad);
	DDX_Control(pDX, IDC_DEL, m_btn_Del);
	DDX_Control(pDX, IDC_BTN_SORT22, m_btnSort22);
	DDX_Control(pDX, IDC_BTN_SORT21, m_btnSort21);
	DDX_Control(pDX, IDC_BTN_SORT20, m_btnSort20);
	DDX_Control(pDX, IDC_BTN_SORT19, m_btnSort19);
	DDX_Control(pDX, IDC_BTN_SORT18, m_btnSort18);
	DDX_Control(pDX, IDC_BTN_SORT17, m_btnSort17);
	DDX_Control(pDX, IDC_BTN_SORT16, m_btnSort16);
	DDX_Control(pDX, IDC_BTN_SORT15, m_btnSort15);
	DDX_Control(pDX, IDC_BTN_SORT14, m_btnSort14);
	DDX_Control(pDX, IDC_BTN_SORT13, m_btnSort13);
	DDX_Control(pDX, IDC_BTN_SORT12, m_btnSort12);
	DDX_Control(pDX, IDC_BTN_SORT11, m_btnSort11);
	DDX_Control(pDX, IDC_BTN_SORT10, m_btnSort10);
	DDX_Control(pDX, IDC_BTN_SORT09, m_btnSort09);
	DDX_Control(pDX, IDC_BTN_SORT08, m_btnSort08);
	DDX_Control(pDX, IDC_BTN_SORT07, m_btnSort07);
	DDX_Control(pDX, IDC_BTN_SORT06, m_btnSort06);
	DDX_Control(pDX, IDC_BTN_SORT05, m_btnSort05);
	DDX_Control(pDX, IDC_BTN_SORT04, m_btnSort04);
	DDX_Control(pDX, IDC_BTN_SORT03, m_btnSort03);
	DDX_Control(pDX, IDC_BTN_SORT02, m_btnSort02);
	DDX_Control(pDX, IDC_BTN_SORT01, m_btnSort01);
	DDX_Control(pDX, IDC_GNAME, m_gname);
	DDX_Control(pDX, IDC_SUM, m_sum);
	DDX_Control(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_COMBO_SEARCH, m_cbSearch);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_RADIO_ELWALL, m_Radio_ELWAll);
	DDX_Control(pDX, IDC_RADIO_ELWCALL, m_Radio_ELWCall);
	DDX_Control(pDX, IDC_RADIO_ELWPUT, m_Radio_ELWPut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkList2)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_ADDALL, OnAddall)
	ON_BN_CLICKED(IDC_DELALL, OnDelall)
	ON_BN_CLICKED(IDC_BLANK, OnBlank)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
	ON_BN_CLICKED(IDC_MOVEDN, OnMovedn)
	ON_BN_CLICKED(IDC_NEWGROUP, OnNewgroup)
	ON_BN_CLICKED(IDC_UPLOAD, OnUpload)
	ON_BN_CLICKED(IDC_DNLOAD, OnDnload)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST1, OnBegindragList1)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST2, OnBegindragList2)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_ALLSAVE, OnAllsave)
	ON_CBN_SELCHANGE(IDC_GNAME, OnSelchangeGname)
	ON_CBN_SELCHANGE(IDC_COMBO_SEARCH, OnSelchangeComboSearch)
	ON_BN_CLICKED(IDC_EDITGROUP, OnEditgroup)
	ON_BN_CLICKED(IDC_CNVINTEREST, OnCnvinterest)
	ON_BN_CLICKED(IDC_DNLOADR, OnDnloadr)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnColumnclickList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST2, OnColumnclickList2)
	ON_BN_CLICKED(IDC_SHAREIMPORT, OnShareimport)
	ON_BN_CLICKED(IDC_SHAREEXPORT, OnShareexport)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_MOVETOP, OnMovetop)
	ON_BN_CLICKED(IDC_MOVEBOTTOM, OnMovebottom)
	ON_BN_CLICKED(IDC_RADIO_ELWALL, OnRadioClick)
	ON_BN_CLICKED(IDC_RADIO_ELWCALL, OnRadioClick)
	ON_BN_CLICKED(IDC_RADIO_ELWPUT, OnRadioClick)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EDITX, OnEditXMsg)
	ON_MESSAGE(WM_TOPAGE1, OnAppendGroup)
	ON_MESSAGE(WM_TOPAGE2, OnAppendParentGroup)
	ON_MESSAGE(WM_TOSELECTPAGE, OnSelectGroup)
	ON_MESSAGE(WM_TOSELECTGROUPNCODE, OnSelectGroupNCode)
	ON_COMMAND_RANGE(IDC_BTN_SORT01, IDC_BTN_SORT22, OnClickBtnSort)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, &CPage1::OnGetdispinfoList1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1 message handlers
void CPage1::makingBackupFile()
{
	//CFileFind finder;
	//CString	filePath, fileTemp;
	//
	//filePath.Format("%s/%s/%s/portfolio.ini", m_root, USRDIR, m_name);
	//fileTemp.Format("%s/%s/%s/portbackup.ini", m_root, USRDIR, m_name);
	//
	//if (finder.FindFile(filePath))
	//	CopyFile(filePath, fileTemp, FALSE);
	//
	//for (int ii = 0; ii < maxGROUP; ii++)
	//{
	//	filePath.Format("%s/%s/%s/portfolio.i%02d",     m_root, USRDIR, m_name, ii);
	//	fileTemp.Format("%s/%s/%s/portbackup.i%02d",	m_root, USRDIR, m_name, ii);
	//	
	//	if (!finder.FindFile(filePath))
	//		continue;
	//	
	//	CopyFile(filePath, fileTemp, FALSE);
	//}	
}

void	CPage1::recoveryFile()
{
	//CFileFind finder;
	//CString	filePath, fileTemp;
	//
	//filePath.Format("%s/%s/%s/portfolio.ini", m_root, USRDIR, m_name);
	//fileTemp.Format("%s/%s/%s/portbackup.ini", m_root, USRDIR, m_name);
	//
	//if (finder.FindFile(fileTemp))
	//	CopyFile(fileTemp, filePath, FALSE);
	//
	//for(int ii = 0 ; ii< maxGROUP ; ii++)
	//{
	//	filePath.Format("%s/%s/%s/portfolio.i%02d",     m_root, USRDIR, m_name, ii);

	//	if (!finder.FindFile(filePath))
	//		continue;

	//	DeleteFile(filePath);
	//}

	//for (int ii = 0; ii < maxGROUP; ii++)
	//{
	//	filePath.Format("%s/%s/%s/portfolio.i%02d",     m_root, USRDIR, m_name, ii);
	//	fileTemp.Format("%s/%s/%s/portbackup.i%02d",	m_root, USRDIR, m_name, ii);
	//	
	//	if (!finder.FindFile(fileTemp))
	//		continue;
	//	
	//	CopyFile(fileTemp, filePath, FALSE);
	//}
}

void	CPage1::deleteBackupFile()
{
	CFileFind finder;
	CString	filePath, fileTemp;
	
	fileTemp.Format("%s/%s/%s/portbackup.ini", m_root, USRDIR, m_name);

	if (finder.FindFile(fileTemp))
		DeleteFile(fileTemp);

	for (int ii = 0; ii < maxGROUP; ii++)
	{
		fileTemp.Format("%s/%s/%s/portbackup.i%02d",	m_root, USRDIR, m_name, ii);
		
		if (!finder.FindFile(fileTemp))
			continue;
		
		DeleteFile(fileTemp);
	}
}


BOOL CPage1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_root = Variant(homeCC, "");
	m_name = Variant(nameCC, "");

	CString userD; 
	userD.Format("%s/%s/%s", m_root, USRDIR, m_name);

	CString saveN; 
	saveN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);

	CString tempN; 
	tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);

	CreateDirectory(userD, nullptr); 
	CopyFile(saveN, tempN, FALSE);

	loadingHJcode();
	loadingFJcode();
	loadingOJcode();
	loadingPJcode();
	loadingUPcode();
	loadingETFcode();
	loadingELWcode();
	loadingCFcode(true);
	loadingCFcode(false);
	loadingSFcode();

	makingBackupFile();

	initList(); 
	initTree(); 
#ifndef DF_SEARCH
	initCombo();
#endif
	
	m_cbSearch.SetCurSel(0);	//2015.11.02 KSJ ����

	//������ ����
	SetSize();
	loadConfig();

	//ȭ�� ������ ���� �������� ����Ʈ �����´�
//	OnDnload();


	selectKospi();	

	m_Radio_ELWAll.SetCheck(TRUE);

	DWORD	dwConversion{}, dwSentence{};
	HIMC hImc = ImmGetContext(this->m_hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		dwConversion |= IME_CMODE_HANGEUL; 
		ImmSetConversionStatus(hImc, dwConversion, dwSentence);
	}

	m_gListsort = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPage1::loadConfig()
{
	CString tempN; 

	tempN.Format("%s/%s/%s/interField.ini.tmp", m_root, USRDIR, m_name);

	CFileFind finder;
	if (!finder.FindFile(tempN))
	{
		tempN.Format("%s/%s/%s/interField.ini", m_root, USRDIR, m_name);
		if (!finder.FindFile(tempN))
		{
			return;
		}
	}
		
	int	readL = 0;

	readL = GetPrivateProfileInt("INTERFIELD", "OVERLAP", 0, tempN);
	m_isDuplicateCode = readL;
}


void CPage1::selectKospi()
{
	HTREEITEM root_Item{}, child_Item{};
	CString root_name, child_name;
	
	root_Item = m_tree.GetRootItem();
	
	while(root_Item)
	{
		root_name = m_tree.GetItemText(root_Item);
		
		if(root_name == "�ֽ�����")
		{
			child_Item = m_tree.GetChildItem(root_Item);
			
			while(child_Item)
			{
				child_name = m_tree.GetItemText(child_Item);
				
				if(child_name == "������")
				{
					m_tree.SelectItem(child_Item);
					xAllCodeToList();
					break;
				}
				
				child_Item = m_tree.GetNextSiblingItem(child_Item);
				
			}
			break;
		}
		
		root_Item = m_tree.GetNextSiblingItem(root_Item);
	}
}

void CPage1::selectElw()
{
	HTREEITEM root_Item{}, child_Item{};
	CString root_name, child_name;
	
	root_Item = m_tree.GetRootItem();
	
	while(root_Item)
	{
		root_name = m_tree.GetItemText(root_Item);
		
		if(root_name == "ELW")
		{
			child_Item = m_tree.GetChildItem(root_Item);
			
			while(child_Item)
			{
				child_name = m_tree.GetItemText(child_Item);
				
				if(child_name == "������")
				{
					m_tree.SelectItem(child_Item);
					xELWCodeToList();
					break;
				}
				
				child_Item = m_tree.GetNextSiblingItem(child_Item);
				
			}
			break;
		}
		
		root_Item = m_tree.GetNextSiblingItem(root_Item);
	}
}

void CPage1::ExceptionMsg()
{
	const int nCnt = m_gname.GetCount();
	int nIdx = -1;

	for (int i = 0; i < nCnt; i++)
	{
		if (m_activegno == (int)m_gname.GetItemData(i))
		{
			nIdx = i;
			break;
		}
	}

	if (-1 != nIdx)
	{
		CString strName;
		m_gname.GetLBText(nIdx, strName);

		MessageBox(strName + " ���� �ڵ忡 ������ �ֽ��ϴ�.\n\n�������� �ޱ� �Ǵ� �������񺹱�\n�Ͻñ� �ٶ��ϴ�.", "IBK��������");
	}
}
 
BOOL CPage1::OnSetActive() 
{
#ifdef DF_SEARCH
	CWnd* wnd = GetParent()->GetParent();
	wnd->SendMessage(WM_MSG, MAKEWPARAM(MSG_SET_ACTIVE_PAGE, 0), (LPARAM)1);

	if (m_activePage && m_activegno != -1)
	{
		loadingInterest(m_activegno);  //�����Ϸ� 
		loadConfig();
	}
	m_activePage = true;
#else
	if (m_activePage && m_activegno != -1)
	{
		loadingInterest(m_activegno);   //�����Ϸ� 

		_listitem* plist{};
		int nListCnt{};

		struct	_inters* pinter{};
		const int nCnt = m_inters.GetSize();

		for (int i = 0; i < nCnt; i++)
		{
			pinter = m_inters.GetAt(i);

			if (pinter)
			{
				nListCnt = m_list2item.GetSize();

				for (int j = 0; j < nListCnt; j++)
				{
					 plist  = ((_listitem*)m_list2.GetItemData(j));

					 if (0 == strcmp(pinter->code, plist->code))
					 {
						 plist->pInter = pinter;
					 }
				}
			}
		}

		loadConfig();
	}

	m_activePage = true;
#endif
	return CPropertyPage::OnSetActive();
}

BOOL CPage1::OnKillActive() 
{
	CString	gname;
	if (m_activegno != -1)
	{
		gname = GetGroupName(m_activegno);  //OnKillActive
		savingGroupFile(m_activegno, gname);

		//������ ����
		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upEachGroupACTION, m_activegno-1), 0);
	}

	return CPropertyPage::OnKillActive();
}

BOOL CPage1::endSendAction() 
{
	CString	gname;
	if (m_activegno != -1)
	{
		gname = GetGroupName(m_activegno);  //endSendAction
		savingGroupFile(m_activegno, gname);
		
		//������ ����
		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upEndACTION, m_activegno-1), 0);
	}
	else if(GetGroupCount() == 0)	//2015.04.09 KSJ ���õ� �׷��� ���� �׷�ī��Ʈ�� 0 �̶�� ��ü�����Ȱ����� �ľ��ϰ� â�� �ݴ´�.
	{
		CString string; 
		string = "OnPortfolio\tok";
		m_parent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)string);
		m_parent->SendMessage(WM_USER, closeDLL, 0);	
	}

	return CPropertyPage::OnKillActive();
}

int CPage1::SaveGroup()
{
	CString m_userD, m_tempN;
	m_userD.Format("%s/%s/%s",    m_root, USRDIR, m_name);
//	m_tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, "portfolio.ini.tmp");

	WritePrivateProfileString(_T("GROUPNAME"), nullptr, nullptr, m_userD);

	CString	string = _T(""), gnoS, gnameS;
	const int count = m_gname.GetCount();

	for (int ii = 0; ii < count; ii++)
	{
		m_gname.GetLBText(ii, gnameS);
		gnoS.Format("%02d", ii+1);
		
		string += gnoS; 
		string += ";";
		
		WritePrivateProfileString(_T("GROUPNAME"), gnoS, gnameS, m_userD);
	}
	
	WritePrivateProfileString(_T("GROUPORDER"), "00", string, m_userD);
	WritePrivateProfileString(nullptr, nullptr, nullptr, m_userD);

	return count;

}

void CPage1::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnAdd();
	*pResult = 0;
}

void CPage1::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDel();	
	*pResult = 0;
}

void CPage1::OnAdd() 
{
	insertList1toList2();
}

void CPage1::OnDel() 
{
	deleteList2();
}

void CPage1::OnAddall() 
{
	if (m_activegno == -1)
	{
		GuideMsg("���õ� �׷��� �����ϴ�. ���׷��� ����ϼ���", false);
		return;
	}

	m_bIsEdit = TRUE;
	m_bFirstDuplicateNO = false;		//2012.04.02 KSJ ó���� ����Ʈ��..

	deselcMark(&m_list2);
	
	

	CString code, name;
	const int gubn = m_tree.GetItemData(m_tree.GetSelectedItem());
	int position = m_list2.GetItemCount();

	for (int ii = 0; ii < m_list1.GetItemCount(); ii++)
	{
		code = m_list1.GetItemText(ii, 0);
		name = m_list1.GetItemText(ii, 1);

		if (existCodeInList2(code))
			continue;
		if (m_list2.GetItemCount() >= maxJONGMOK)
		{	
			GuideMsg(idTOstring(IDS_GUIDE1));
			break;
		}
		AppendItemToList2(position, gubn, code, name);
		position += 1;
	}
	selcMark(&m_list2, position -1);
	InitEdit();
}

void CPage1::OnDelall() 
{
	if (m_activegno == -1)
	{
		GuideMsg("���õ� �׷��� �����ϴ�. ���׷��� ����ϼ���", false);
		return;
	}
	
	m_bIsEdit = TRUE;

	ClearInterest();
	ClearList2item();
	m_list2.DeleteAllItems();
	updateList2sum();
}

void CPage1::OnBlank() 
{
	if (m_activegno == -1)
	{
		GuideMsg("���õ� �׷��� �����ϴ�. ���׷��� ����ϼ���", false);
		return;
	}
	
	m_bIsEdit = TRUE;

	if (m_list2.GetItemCount() >= maxJONGMOK)
	{
		GuideMsg(idTOstring(IDS_GUIDE1));
		return;
	}

	int	nitem{};
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	if (pos != nullptr)
	{
		nitem = m_list2.GetNextSelectedItem(pos);
		AppendItemToList2(nitem, 0, _T("      "), _T("      "), _T("0"), false);
	}
	else
	{
		nitem = m_list2.GetItemCount();
		AppendItemToList2(nitem, 0, _T("      "), _T("      "), _T("0"), true);
	}
	InitEdit();
}

void CPage1::OnMoveup() 
{
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	if (pos == nullptr)
	{
		GuideMsg(idTOstring(IDS_GUIDE2));
		return;
	}
	
	m_bIsEdit = TRUE;

	const int nitem = m_list2.GetNextSelectedItem(pos);

	if (nitem == 0) 
		return;

	int gubn{};
	CString code, name;
	char bookmark;
	bookmark = '0';


	_inters* pinter = m_inters.GetAt(nitem);

	gubn = (int)pinter->gubn[0];
	code = CString(pinter->code, sizeof(pinter->code));
	// MODIFY PSH 20070918
	//name = GetCodeName(pinter->code);
	name = ('m' == pinter->code[0]) ? pinter->name : GetCodeName(pinter->code);
	// END MODIFY
	bookmark = pinter->bookmark[0] == '1' ? '1':'0';//2015.04.03 KSJ 1�̾ƴϸ� 0���� ���ش�.

	delete pinter; 
	m_inters.RemoveAt(nitem);
	m_list2.DeleteItem(nitem);

	delete m_list2item.GetAt(nitem);
	m_list2item.RemoveAt(nitem);

	AppendItemToList2(nitem-1, gubn, code, name, bookmark, false);
	deselcMark(&m_list2); 
	selcMark(&m_list2, nitem-1);
}

void CPage1::OnMovedn() 
{
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	if (pos == nullptr)
	{
		GuideMsg(idTOstring(IDS_GUIDE2));
		return;
	}
	
	m_bIsEdit = TRUE;

	const int nitem = m_list2.GetNextSelectedItem(pos);
	if (nitem >= m_list2.GetItemCount()-1) return;
	
	int gubn{}; CString code, name;
	char bookmark;
	bookmark = '0';

	_inters* pinter = m_inters.GetAt(nitem);

	gubn = (int)pinter->gubn[0];
	code = CString(pinter->code, sizeof(pinter->code));
	// MODIFY PSH 20070918
	//name = GetCodeName(pinter->code);
	name = ('m' == pinter->code[0]) ? pinter->name : GetCodeName(pinter->code);
	// END MODIFY
	bookmark = pinter->bookmark[0] == '1' ? '1':'0';//2015.04.03 KSJ 1�̾ƴϸ� 0���� ���ش�.

	delete pinter; 
	m_inters.RemoveAt(nitem);
	m_list2.DeleteItem(nitem);

	delete m_list2item.GetAt(nitem);
	m_list2item.RemoveAt(nitem);

	AppendItemToList2(nitem+1, gubn, code, name, bookmark, false);
	deselcMark(&m_list2); 
	selcMark(&m_list2, nitem+1);
}

BOOL CPage1::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMTREEVIEW* nmtv{};
	NMHDR* pNMHDR{};

	int	selItem{};
	CString itemname;
	CString cg_item, result_item, stemp;

	switch (wParam)
	{
	case IDC_TREE:
		nmtv = (NMTREEVIEW *) lParam;
		if (nmtv->hdr.code != TVN_SELCHANGED)
			break;
		
		selItem = m_tree.GetItemData(nmtv->itemNew.hItem);
		switch (selItem)
		{
		case xALLCODE:			// all_coode
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xAllCodeToList();
			m_bAddAll = true;
			break;
		case xKOSPI:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xKospiToList();
			m_bAddAll = true;
			break;
		case xKOSDAQ:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xKosdaqToList();
			m_bAddAll = true;
			break;
		case xKOSPIFUTURE:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xFutureToList();
			m_bAddAll = true;
			break;
		case xSTOCKFUTURE:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xSFutureToList();
			m_bAddAll = true;
			break;
		case xINTERESTRATEFUTURE:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xCFutureToList(false);
			m_bAddAll = true;
			break;
		case xCURRENCYFUTURE:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xCFutureToList(true);
			m_bAddAll = true;
			break;
		case xFUTURE:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xFutureToList();
			m_bAddAll = true;
			break;
		case xCALLCODE:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xCallCodeToList();
			m_bAddAll = true;
			break;
		case xPUTCODE:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xPutCodeToList();
			m_bAddAll = true;
			break;
		case xKOPSI200:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xKospi200ToList();
			m_bAddAll = true;
			break;
		case xKOSDAQ50:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xKosdaq50ToList();
			m_bAddAll = true;
			break;
		case xKRX100:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xKRX100ToList();
			m_bAddAll = true;
			break;
		case xFREECODE:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xFreeCodeToList();
			m_bAddAll = true;
			break;
		case xETFCODE:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
//			xETFCodeToList();
			ETFInitialList();
			m_bAddAll = true;
			break;
		case xELWCODE:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_SHOW);
			m_Radio_ELWCall.ShowWindow(SW_SHOW);
			m_Radio_ELWPut.ShowWindow(SW_SHOW);
			xELWCodeToList();
			m_bAddAll = true;
			break;
		case xELWBASE:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_SHOW);
			m_Radio_ELWCall.ShowWindow(SW_SHOW);
			m_Radio_ELWPut.ShowWindow(SW_SHOW);
			xELWBaseToList();
			m_bAddAll = true;
			break;

		case xSINJUCODE:
			m_search.SetWindowText("");
			xSinjuCodeToList();
			m_bAddAll = true;
			break;
		case xKOSPIJISU:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xKospiJisuToList();
			m_bAddAll = true;
			break;
		case xKOSDAQJISU:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xKosdaqJisuToList();
			m_bAddAll = true;
			break;
		case xRecommand:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xRecommandtoList();
			m_bAddAll = true;			
			break;

		case xKBc: case xKBp:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xPjcodeToList("10", selItem == xKBc ? '2' : '3');
			break;
		case xSECc: case xSECp:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xPjcodeToList("11", selItem == xSECc ? '2' : '3');

			break;
		case xSKc: case xSKp:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xPjcodeToList("12", selItem == xSKc ? '2' : '3');
			break;
		case xPOSCOc: case xPOSCOp:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xPjcodeToList("13", selItem == xPOSCOc ? '2' : '3');
			break;
		case xKTc: case xKTp:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xPjcodeToList("14", selItem == xKTc ? '2' : '3');
			break;
		case xKEPCOc: case xKEPCOp:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xPjcodeToList("15", selItem == xKEPCOc ? '2' : '3');
			break;
		case xHYUNDAIc: case xHYUNDAIp:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xPjcodeToList("16", selItem == xHYUNDAIc ? '2' : '3');
			break;
		case xETFALLCODE:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			OnClickTree(pNMHDR, 0);
			m_bAddAll = true;
			break;
		case xELWSelect:
			m_Radio_ELWAll.ShowWindow(SW_SHOW);
			m_Radio_ELWCall.ShowWindow(SW_SHOW);
			m_Radio_ELWPut.ShowWindow(SW_SHOW);
			break;
		case xELWPB:
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_SHOW);
			m_Radio_ELWCall.ShowWindow(SW_SHOW);
			m_Radio_ELWPut.ShowWindow(SW_SHOW);
			OnClickELWTree(pNMHDR, 0);
			break;
		case xKONEX:	//2013.06.25 �ڳؽ� �߰�
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xKonexToList();
			m_bAddAll = true;
			break;
		case xETNCODE:	//2014.11.06 ETN �߰�
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xETNCodeToList();
			m_bAddAll = true;
			break;
		case xSPACCODE:	//2015.11.03 spac �߰�
			m_search.SetWindowText("");
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			xSPACCodeToList();
			m_bAddAll = true;
			break;

		default:
			m_Radio_ELWAll.ShowWindow(SW_HIDE);
			m_Radio_ELWCall.ShowWindow(SW_HIDE);
			m_Radio_ELWPut.ShowWindow(SW_HIDE);
			if (selItem <= 0)
				break;

			switch (selItem / 100)
			{
			case xKOSPIUP:
				m_search.SetWindowText("");
				xKospiUpToList(selItem);
				m_bAddAll = true;
				break;
			case xKOSDAQUP:
				m_search.SetWindowText("");
				stemp = m_tree.GetItemText(nmtv->itemNew.hItem);
				xKosdaqUpToList(selItem, stemp);
				m_bAddAll = true;
				break;
			case xINTEREST:
				m_search.SetWindowText("");
				xInterestToList(selItem);
				m_bAddAll = true;
				break;		
			}

			switch (selItem / 1000)
			{
			case xKOSPIex:
				m_search.SetWindowText("");
				xKospiExToList(selItem);
				m_bAddAll = true;
				break;
			case xKOSDAQex:
				m_search.SetWindowText("");
				xKosdaqExToList(selItem);
				m_bAddAll = true;
				break;
			case xTHEMACODE:
				m_search.SetWindowText("");
				xThemaCodeToList(selItem);
				m_bAddAll = true;
				break;
			case xGROUPCODE:
				m_search.SetWindowText("");
				xGroupCodeToList(selItem);
				m_bAddAll = true;
				break;
			case xELWISSUE:								//ELW����ȸ�纰 ����˻� �߰� 081219
				m_search.SetWindowText("");
				
				cg_item.Format("%05d", selItem);
				result_item = cg_item.Mid(2,3);
				ELWIssueDnload(atoi(result_item));		//selItem : ������ treeID->value				
				m_bAddAll = true;
				break;				
			}
			
			
			switch(selItem / 10000)
			{
			case xELWASSET:								//ELW�����ڻ꺰 ����˻� �߰� 081219
				m_search.SetWindowText("");
				
				//�ڵ尪�� ELWISSUE(21) + ��ǲ(1, 2) + �����ȣ(001 - 999)
				
				cg_item.Format("%06d", selItem);
				result_item = cg_item.Mid(2, 1);
				
				if(result_item == "1")
				{
					itemname = _T("��");
				}
				else if(result_item == "2")
				{
					itemname = _T("ǲ");
				}
				
				result_item = cg_item.Mid(3,3);
				ELWAssetDnload(atoi(result_item), itemname);			//selItem : ������ treeID->value
				m_bAddAll = true;
				break;
			}

			switch(selItem / 100000)
			{
			case xELWASSET:								//ELW�����ڻ꺰 ����˻� �߰� 081219
				m_search.SetWindowText("");
				
				//�ڵ尪�� ELWISSUE(21) + ��ǲ(1, 2) + �����ȣ(001 - 999)
				
				cg_item.Format("%06d", selItem);
				result_item = cg_item.Mid(2, 1);
				
				if(result_item == "1")
				{
					itemname = _T("��");
				}
				else if(result_item == "2")
				{
					itemname = _T("ǲ");
				}
				
				result_item = cg_item.Mid(3,3);
				ELWAssetDnload(atoi(result_item), itemname);			//selItem : ������ treeID->value
				m_bAddAll = true;
				break;
			}

			break;
		}
		
		break;
	case IDC_LIST1:
	{
		int iii = 1;
		switch (((LPNMHDR)lParam)->code)
		{
		case LVN_COLUMNCLICK:
#define pnm ((LPNMLISTVIEW)lParam)
			m_gListsort = !m_gListsort;
			switch ((LPARAM)pnm->iSubItem)
			{
			case 0:
				qsort(static_cast<void*>(&m_arList[0]), m_arList.GetSize(), sizeof(CITEMS), compareSortCode2);
				break;
			case 1:
				qsort(static_cast<void*>(&m_arList[0]), m_arList.GetSize(), sizeof(CITEMS), compareSortName2);
				break;
			}
			m_list1.Invalidate();
#undef pnm

			break;
		}
	}
		break;
	}
//	m_list1.ShowWindow(SW_SHOW);
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

long CPage1::OnEditXMsg(WPARAM wParam, LPARAM lParam)
{
	CString	string;
	switch ((int) wParam)
	{
	case wpBLANK:
		searchAction(0);	// blank name search by kwon
		//selcMark(&m_list1, 1);
		break;
	case wpENTER:		m_search.GetWindowText(string);
		if (!string.IsEmpty())
		{
			m_search.SetSel(0, -1);
			insertList1toList2();
		}
		break;
	case wpSEARCH1:
		//�ڵ� �м��Ͽ� ELW���� �Ǵ�
		analyzeELWCode();
		searchAction(0);	// number search by kwon
		break;
	case wpSEARCH2:
		analyzeELWCode();
		
		m_search.GetWindowText(string);
		CString temp = string.GetAt(0);
		if(atoi(temp) >= 5 && atoi(temp) < 9)
		{
			searchAction(0);	// char search by kwon
		}
		else
		{
			searchAction(1);	// char search by kwon
		}
		
		break;
	}

	return 0;
}

void CPage1::OnClickBtnSort(UINT nID)	// name btn click search by kwon
{
	CString	sName, sCode, sTitle;
	CWnd	*pBtn = GetDlgItem(nID);
	pBtn->GetWindowText(sTitle);

	if (sTitle == _T("����"))
	{
		_listitem* listitem{};
		int	position = 0;
		m_list1.DeleteAllItems();
		m_arList.RemoveAll();
		for (int ii = 0; ii < m_listitem.GetSize(); ii++)
		{
			listitem = m_listitem.GetAt(ii);

			sName = CString(listitem->name, sizeof(listitem->name));
			if (0x41 >  sName.GetAt(0) || sName.GetAt(0) > 0x7A)
				continue;

			sCode = CString(listitem->code, sizeof(listitem->code));
			AppendItemToList1(position, sCode, sName, listitem);
			position += 1;
		}
		m_list1.SetItemCountEx(m_arList.GetSize());
		m_list1.Invalidate();

		if (m_list1.GetItemCount() > 0)
		{
			m_listsort = false;
			ListView_SortItems(m_list1, ListViewCompareFunc, 1);

			deselcMark(&m_list1);
			selcMark(&m_list1, 0);
		}
		m_bAddAll = false;
		m_search.SetWindowText("");
	}
	else if (sTitle == _T("�����"))
	{
		m_search.SetWindowText("");
		searchAction(0);
	}
	else if (sTitle == _T("��"))
	{
		m_search.GetWindowText(sName);
		if (sName.IsEmpty())
		{
		}
		if (sName.GetLength() == 1)	// ����
		{
			m_search.SetWindowText("");
			searchAction(0);
		}
		else
		{
			const char	cChar = sName.GetAt(sName.GetLength()-1);
			if (cChar & HANGULMSK)	sName = sName.Left(sName.GetLength() - 2);
			else			sName = sName.Left(sName.GetLength() - 1);

			m_search.SetWindowText(sName);
			if (sName.IsEmpty())
				searchAction(0);
			else
				searchAction(1);
		}
	}
	else
	{
		m_search.GetWindowText(sName);
		m_search.SetWindowText(sName + sTitle);
		searchAction(1);
	}

	m_search.SetFocus();
	m_search.SetSel(0, -1);
	m_search.SetSel(-1, -1);
}

int CPage1::getManageCount()
{
	CString sequence;
	int i = 0;
	for(i=0 ; i<100 ; i++)
	{
		sequence = m_manageGroup[i][0];
		
		if(!sequence.IsEmpty())
			continue;

		break;
	}
	return i;
}

bool CPage1::checkChangeSequence()
{
	const int count = getManageCount();	
	bool result = false;

	for(int i=0 ; i<count ; i++)
	{
		if(m_manageGroup[i][0] != m_manageGroup[i][2])	
		{
			result = true;
			break;
		}
	}

	return result;
}

CString	CPage1::getManageGroupdata(int row, int col)
{
	return m_manageGroup[row][col];
}

void	CPage1::selectComboBox()
{
#ifdef DF_SEARCH
	m_list2.DeleteAllItems();
	ClearList2item();

	int idata = m_gname.GetCurSel();

	m_activegno = m_gname.GetItemData(m_gname.GetCurSel());

	const int count = loadingInterest(m_activegno);   //�����Ϸ� 
	return;




	const int nBreakCnt = 0;
	CString tcode;
	CString name;
	int nItem{}; _inters* pinter{};
	for (int ii = 0; ii < count; ii++)
	{
		pinter = m_inters.GetAt(ii);

		if (pinter->code[0] == 'm')
		{
			nItem = AddItem(&m_list2, "", ii);

			if (strlen(pinter->code))
				AddItem(&m_list2, pinter->name, nItem, 1);
		}

		else
		{
			tcode = pinter->code;
			tcode = tcode.Mid(0,12);
			tcode.TrimRight();
			nItem = AddItem(&m_list2, tcode, ii);
			name = GetCodeName(tcode);
			name.TrimLeft(); name.TrimRight();
			if (strlen(pinter->code))
				AddItem(&m_list2, name, nItem, 1);
		}

		_listitem* listitem = new _listitem();
		listitem->pInter = pinter;
		strcpy(listitem->code, tcode);
		strcpy(listitem->name, GetCodeName(tcode));

		m_list2item.Add(listitem);
		m_list2.SetItemData(nItem, (LPARAM) listitem);
		// END MODIFY
	}

	updateList2sum();
#else
	m_list2.DeleteAllItems();
	ClearList2item();

	m_activegno = m_gname.GetItemData(m_gname.GetCurSel());

	const int count = loadingInterest(m_activegno);  //�����Ϸ�
	const int nBreakCnt = 0;
	CString tcode;
	CString name;
	int nItem{}; _inters* pinter{};
	for (int ii = 0; ii < count; ii++)
	{
		pinter = m_inters.GetAt(ii);

		if (pinter->code[0] == 'm')
		{
			nItem = AddItem(&m_list2, "", ii);

			if (strlen(pinter->code))
				AddItem(&m_list2, pinter->name, nItem, 1);
		}

		else
		{
			tcode = pinter->code;
			tcode = tcode.Mid(0, 12);
			tcode.TrimRight();
			nItem = AddItem(&m_list2, tcode, ii);
			name = GetCodeName(tcode);
			name.TrimLeft(); name.TrimRight();
			if (strlen(pinter->code))
				AddItem(&m_list2, name, nItem, 1);
		}

		_listitem* listitem = new _listitem();
		listitem->pInter = pinter;
		strcpy(listitem->code, tcode);
		strcpy(listitem->name, GetCodeName(tcode));

		m_list2item.Add(listitem);
		m_list2.SetItemData(nItem, (LPARAM)listitem);
		// END MODIFY
	}

	updateList2sum();
#endif
}

void CPage1::OnSelchangeGname()
{
	CString	gname, tempS;
	
	//���� ������ �׷� �ϴ� ����
	if (m_activegno != -1)
	{
// 		m_gname.GetLBText(m_activegno-1, tempS);
// 
// 		if(tempS == "")
// 		{
// 			return;
// 		}
// 		else
		{
			gname = GetGroupName(m_activegno); //OnSelchangeGname
			savingGroupFile(m_activegno, gname);	

			//������ ����
			CWnd* wnd = GetParent()->GetParent();
			wnd->SendMessage(TOmapWnd, MAKEWPARAM(upEachGroupACTION, m_activegno-1), 0);
		}
	}

	selectComboBox();
}

void CPage1::OnSelchangeComboSearch()
{
	CString	string;
	m_search.GetWindowText(string);
	
	if (string.IsEmpty())
		searchAction(0);
	else
		searchAction(1);
	
	m_search.SetFocus();
	m_search.SetSel(0, -1);
}

void CPage1::OnAllsave() 
{
	//OnKillActive();

	//CFileFind finder;
	//CString	filePath, fileTemp;

	//filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);
	//fileTemp.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);

	//if (finder.FindFile(fileTemp))
	//	CopyFile(fileTemp, filePath, FALSE);

	//for (int ii = 0; ii < maxGROUP; ii++)
	//{
	//	filePath.Format("%s/%s/%s/portfolio.i%02d",     m_root, USRDIR, m_name, ii);
	//	fileTemp.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, ii);

	//	if (!finder.FindFile(fileTemp))
	//		continue;

	//	CopyFile(fileTemp, filePath, FALSE);
	//}
}

void CPage1::OnNewgroup()
{
	CString	gname;

	m_bIsEdit = TRUE;


	if (m_activegno != -1)
	{
		gname = GetGroupName(m_activegno);  //OnNewgroup
		savingGroupFile(m_activegno, gname);

		//������ ����
		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upEachGroupACTION, m_activegno-1), 0);  
	}

	const int cnt = m_gname.GetCount();
	gname.Format("�׷�%03d", cnt+1);

	CNewGroup newGroup(this, 0, gname);

	if (newGroup.DoModal() == IDOK)
	{
		gname = newGroup.m_defName;
		if (savingGroupOrder(gname))  //OnNewgroup
		{
			OnDelall();
			savingGroupFile(m_activegno, gname);
		}

		//Send �Ͽ� ������ ���Ľ�Ų��
		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upnewgroupACTION, 0), 0);	

	}
}

#ifdef DF_SEARCH
void	CPage1::resettingGroup()
{
	CString tcode;
	m_activegno = -1; 
	initCombo();
}
#else
void	CPage1::resettingGroup()
{
	CString tcode;
	m_activegno = -1;
	initCombo();

	const int count = loadingInterest(m_activegno);  //�����Ϸ�

	m_list2.DeleteAllItems();
	ClearList2item();

	int nItem{}; _inters* pinter{};
	for (int ii = 0; ii < count; ii++)
	{
		pinter = m_inters.GetAt(ii);


		if (pinter->code[0] == 'm')
		{
			nItem = AddItem(&m_list2, "", ii);

			if (strlen(pinter->code))
				AddItem(&m_list2, pinter->name, nItem, 1);
		}
		else
		{
			tcode = pinter->code;
			tcode = tcode.Mid(0, 12);
			tcode.TrimRight();
			nItem = AddItem(&m_list2, tcode, ii);
			if (strlen(pinter->code))
				AddItem(&m_list2, GetCodeName(tcode), nItem, 1);
		}

		_listitem* listitem = new _listitem();
		listitem->pInter = pinter;
		strcpy(listitem->code, pinter->code);
		strcpy(listitem->name, pinter->name);

		m_list2item.Add(listitem);
		m_list2.SetItemData(nItem, (LPARAM)listitem);
	}

	updateList2sum();
}
#endif

void CPage1::OnEditgroup() 
{
	if (m_activegno != -1)
	{
		CString gname = GetGroupName(m_activegno);  //OnEditgroup
		savingGroupFile(m_activegno, gname);

		//������ ����
		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upEachGroupACTION, m_activegno-1), 0);
	}

	m_bIsEdit = TRUE;

	CEditGroup editGroup(this, m_root, m_name);
	

	if (editGroup.DoModal() == IDOK)
	{
		CString	string, filePath;
		
		//Send �Ͽ� ������ ���Ľ�Ų��
		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upsequenceACTION, 0), 0);	
		
//		traceManageGroup();
	}
}

void CPage1::OnLButtonUp(UINT nFlags, CPoint point) 
{
	::ReleaseCapture();	

	if (m_dragImage)
	{
		m_dragImage->DragLeave(GetDesktopWindow());
		m_dragImage->EndDrag();
		m_dragImage->DeleteImageList();
		m_dragImage.reset();
		OnEndDrag(point);
	}

	CPropertyPage::OnLButtonUp(nFlags, point);
}

void CPage1::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect	list2RC, labelRC;

	m_list2.GetClientRect(&list2RC);
	m_list2.GetItemRect(0, labelRC, LVIR_LABEL);

	m_list2.ClientToScreen(&list2RC);
	const CRect	topRC	 = CRect(list2RC.left, list2RC.top - 45, list2RC.right, list2RC.top + 5);
	const CRect	bottomRC = CRect(list2RC.left, list2RC.bottom,   list2RC.right, list2RC.bottom + 70);

	if (m_dragImage)
	{
		CPoint	ptMove = point;
		ClientToScreen(&ptMove);

		m_dragImage->DragMove(ptMove);
		
		if (topRC.PtInRect(ptMove))
		{
			SetAutoScroll(&m_list2, -1);
		}
		else if (bottomRC.PtInRect(ptMove))
		{
			SetAutoScroll(&m_list2, +1);
		}
		else if (m_autoScroll)
		{
			CPoint	pt = ptMove;
			LVHITTESTINFO	lvhti;
			m_list2.ScreenToClient(&pt);
			lvhti.pt = pt;

			const int pItem = m_list2.HitTest(&lvhti);

			m_list2.Invalidate();
			
			m_autoScroll = false;
		}
	}
	
	CPropertyPage::OnMouseMove(nFlags, point);
}

void CPage1::OnBegindragList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	*pResult = 0;

	if (m_list1.GetSelectedCount() == 0)
		return;

	m_dragData.list = 1;
	m_dragData.item = pNMListView->iItem;

	m_dragImage.reset();

	CPoint	pt;
	m_dragImage = CreateDragImageSymbol(&pt, &m_list1);

	if (m_dragImage == nullptr)
		return;

	CPoint	ptStart = pNMListView->ptAction;
	m_list1.ClientToScreen(&ptStart);

	m_dragImage->BeginDrag(0, CPoint(0, 0));
	m_dragImage->DragEnter(GetDesktopWindow(), ptStart);
	SetCapture();
}

void CPage1::OnBegindragList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	if (m_list2.GetSelectedCount() == 0)
		return;

	m_dragData.list = 2;
	m_dragData.item = pNMListView->iItem;

	m_dragImage.reset();

	CPoint	pt;
	m_dragImage = CreateDragImageSymbol(&pt, &m_list2);

	if (m_dragImage == nullptr)
		return;

	CPoint	ptStart = pNMListView->ptAction;
	m_list2.ClientToScreen(&ptStart);

	m_dragImage->BeginDrag(0, CPoint(0, 0));
	m_dragImage->DragEnter(GetDesktopWindow(), ptStart);
	SetCapture();
}

void CPage1::OnEndDrag(CPoint point)
{
	CRect	list1RC, list2RC;
	
	m_list1.GetClientRect(&list1RC); 
	m_list1.ClientToScreen(&list1RC);
	m_list2.GetClientRect(&list2RC); 
	m_list2.ClientToScreen(&list2RC);
	
	ClientToScreen(&point);
	m_bIsEdit = TRUE;	

	if (list1RC.PtInRect(point))
	{
		if (m_dragData.list == 2)
			deleteList2();
	}
	else if (list2RC.PtInRect(point))
	{
		LVHITTESTINFO lvhti;
		m_list2.ScreenToClient(&point);
		lvhti.pt = point;
		
		int nitem = m_list2.HitTest(&lvhti);
		if (nitem < 0)	nitem = m_list2.GetItemCount()-1;
		
		if (m_dragData.list == 2)
		{
			int gubn{};
			CString code, name, xprc, xnum, xupnum, filter;
			CString bookmark;

			_inters* pinter = m_inters.GetAt(m_dragData.item);
			gubn = (int)pinter->gubn[0];
			code = CString(pinter->code, sizeof(pinter->code)); 
			
			if(code.Left(1) != " ")
			{
				code.TrimRight();

				if(pinter->code[0] == 'm')
				{
					name = CString(pinter->name);
				}
				else
				{
					name = GetCodeName(code);
				}
			}
			else
			{
				name = "       ";
			}
			
			
			
			xprc = CString(pinter->xprc, sizeof(pinter->xprc));
			xnum = CString(pinter->xnum, sizeof(pinter->xnum));
			xupnum = CString(pinter->xupnum, sizeof(pinter->xupnum));
			filter = CString(pinter->filler, sizeof(pinter->filler));
			bookmark = CString(pinter->bookmark[0], sizeof(pinter->bookmark));

			delete pinter; 
			m_inters.RemoveAt(m_dragData.item);
			m_list2.DeleteItem(m_dragData.item);
			
			delete m_list2item.GetAt(m_dragData.item);
			m_list2item.RemoveAt(m_dragData.item);
			
			AppendItemToList2(nitem, gubn, code, name, xprc, xnum, xupnum, filter, bookmark, false);
	
			deselcMark(&m_list2); 
			selcMark(&m_list2, nitem);
		}
		else if (m_dragData.list == 1)
		{	
			insertList1toList2(nitem);
		}
	}
}


void CPage1::OnUpload()   //������� �ʴ´�
{
	OnKillActive();

	CWnd* wnd = GetParent()->GetParent();
	wnd->SendMessage(TOmapWnd, MAKEWPARAM(upACTION, 0), 0);  //������� �ʴ´�
}

void CPage1::OnDnload() //�������� �������� �ޱ�
{
	clearAllFile();
	
	isClickServerDN = TRUE;
	m_bIsEdit = TRUE;

	CWnd* wnd = GetParent()->GetParent();
	wnd->SendMessage(TOmapWnd, MAKEWPARAM(dnACTION, 0), 0);

	((CWnd*)(wnd->GetDlgItem(IDOK)))->EnableWindow(FALSE);	//2016.07.12 KSJ ���� ��ư ������ ���� ������ ���� Ȯ�ι�ư�� false ���ش�.
}

void CPage1::tempSaveGroupIndex()
{
	CString filePath;
	char readB[2048]{};
	int readL = 0;
	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);
	readL = GetPrivateProfileString(_T("GROUPORDER"), "00", "", readB, sizeof(readB), filePath);
	m_strGroupNumber = CString(readB, readL);
}

void CPage1::xRecommandtoList()
{
	//isClickServerDN = TRUE;
	//
	//CWnd* wnd = GetParent()->GetParent();
	//wnd->SendMessage(TOmapWnd, RecommnadDnACTION, 0);
}

void CPage1::ELWIssueDnload(int selItem)  //������ �ʴ´�
{
	//clearAllFile();
	//
	//isClickServerDN = TRUE;
	//
	//CWnd* wnd = GetParent()->GetParent();
	//wnd->SendMessage(TOmapWnd, MAKEWPARAM(ELWIssueDndnACTION, selItem), 0);
}

void CPage1::ELWAssetDnload(int selItem, CString item)
{
	//clearAllFile();
	//
	//isClickServerDN = TRUE;
	//
	//CWnd* wnd = GetParent()->GetParent();
	//wnd->SendMessage(TOmapWnd, MAKEWPARAM(ELWAssetDndnACTION, selItem), (LPARAM)((char*)item.operator LPCTSTR()));
}

void CPage1::OnDnloadr() 
{
	CString msg = "������ ���������� ������ ������������ �����մϴ�.\n\n�׷��� ���� ����Ǿ� �ִ� ���������� ��� ������ �� �ֽ��ϴ�.\n\n\n���������� �����Ͻðڽ��ϱ�?";
	CString caption = "�������񺹱�";
	constexpr UINT type = MB_OKCANCEL|MB_ICONEXCLAMATION;
	if (MessageBox(msg, caption, type) == IDCANCEL)
		return;
	
	m_bIsEdit = TRUE;

//	tempSaveGroupIndex();
	clearAllFile();
	CWnd* wnd = GetParent()->GetParent();
	wnd->SendMessage(TOmapWnd, MAKEWPARAM(dnACTIONR, 0), 0);

	((CWnd*)(wnd->GetDlgItem(IDOK)))->EnableWindow(FALSE);	//2016.07.12 KSJ ���� ��ư ������ ���� ������ ���� Ȯ�ι�ư�� false ���ش�.
}


void CPage1::OnCnvinterest()  //������ �ʴ´�
{
	CString	gubn = Variant(systemCC, "");
	if (gubn.GetLength() <= 0) return;

	if (!GuideMsg(idTOstring(IDS_GUIDE9), true)) return;

	switch (gubn.GetAt(0))
	{
	case 'D':		// IBK ����
		{
			clearAllFile();

			CWnd* wnd = GetParent()->GetParent();
			wnd->SendMessage(TOmapWnd, MAKEWPARAM(dnACTIONX, 0), 0);  //������ �ʴ´�
		}
		break;
	case 'H':		// hana ���� X �����ҵ� 
		{
			//CFileDialog fDlg(TRUE, nullptr, nullptr, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
			//			"������������(*.igf)|*.igf|All Files(*.*)|*.*|");
			//if (fDlg.DoModal() != IDOK)
			//	break;

			//CString filePath = fDlg.GetPathName();

			//CFileFind finder;
			//if (!finder.FindFile(filePath))
			//{
			//	GuideMsg(idTOstring(IDS_GUIDE8));
			//	break;
			//}

			//struct	_inters  inters;
			//struct	_hanasec hanas;

			//UINT	readL;

			//CFile	fileH(filePath, CFile::modeRead);
			//readL = fileH.Read(&hanas.head[0], sizeof(hanas.head) + sizeof(hanas.gnox));
			//if (readL < sizeof(hanas.head) + sizeof(hanas.gnox)
			//	|| strncmp(hanas.head, "INTERESTGROUPFILE", sizeof(hanas.head)))
			//{
			//	fileH.Close();
			//	GuideMsg(idTOstring(IDS_GUIDE7));
			//	break;
			//}

			//int gcount = atoi(CString(hanas.gnox, sizeof(hanas.gnox)));
			//if (gcount <= 0)
			//{
			//	fileH.Close();
			//	GuideMsg(idTOstring(IDS_GUIDE7));
			//	break;
			//}

			//clearAllFile();

			//char	wB[64]{}, *xxP{};
			//CString	gname, code, name;

			//CString	portFile, grpFile, strOrder = _T(""), section;
			//grpFile.Format("%s/%s/%s/portfolio.ini", m_root, USRDIR, m_name);
			//WritePrivateProfileString(_T("GROUPORDER"), "00", strOrder, grpFile);

			//for (int ii = 0; ii < gcount; ii++)
			//{
			//	readL = fileH.Read(&hanas.gnam[0], sizeof(hanas.gnam) + sizeof(hanas.jrec));
			//	if (readL < sizeof(hanas.gnam) + sizeof(hanas.jrec))
			//		break;

			//	gname = CString(hanas.gnam, sizeof(hanas.gnam)); gname.TrimRight(); 

			//	section.Format("%02d", ii); strOrder += section + ";";
			//	WritePrivateProfileString("GROUPNAME", section, gname, grpFile);

			//	portFile.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, ii);
			//	CFile	fileIT(portFile, CFile::modeCreate|CFile::modeWrite);

			//	int jcount = atoi(CString(hanas.jrec, sizeof(hanas.jrec)));
			//	for (int jj = 0; jj < jcount; jj++)
			//	{
			//		ZeroMemory(&inters, sz_inters);
			//		readL = fileH.Read(&hanas.code[0], sizeof(hanas.code));
			//		if (readL < sizeof(hanas.code))
			//			break;

			//		inters.gubn[0] = '0';

			//		code = CString(hanas.code, sizeof(hanas.code)); 
			//		code.TrimRight();
			//		if (code.GetLength() > 0 && code.GetAt(0) == 'A') code = code.Mid(1);

			//		CopyMemory(inters.code, (char*)code.operator LPCTSTR(), min(code.GetLength(), sizeof(inters.code)));

			//		strncpy(wB, (char*)code.operator LPCTSTR(), code.GetLength());
			//		xxP = (char *)m_parent->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)wB);
			//		name = xxP; 
			//		name.TrimLeft(); 
			//		name.TrimRight();
			//		CopyMemory(inters.name, (char*)name.operator LPCTSTR(), name.GetLength());

			//		fileIT.Write(&inters, sz_inters);
			//	}
			//	fileIT.Close();
			//}
			//WritePrivateProfileString(_T("GROUPORDER"), "00", strOrder, grpFile);
			//WritePrivateProfileString(nullptr, nullptr, nullptr, grpFile);
			//fileH.Close(); 
			//reInitInterest();
		}
		break;
	}
}
void CPage1::SetSize()
{	
	if(IsWin8())
	{
		constexpr int x = 10;
		constexpr int y = 20;
		int btnx{}, btny{}, funtionx{}, funtiony{}, gx{}, gy{};
		m_tree.MoveWindow(x, y, 230, 285);						//Ʈ���޴�
		((CStatic *)GetDlgItem(IDC_TREE_STATIC))->MoveWindow(x-5, y-18, 241, 307);
		m_btn_dnLoadR.MoveWindow(x-6, y+290, 230, 23);			//�������񺹱�
		m_btn_dnLoad.MoveWindow((x-6)+100, y+290, 140, 23);			//���������ޱ�
		
		m_cbSearch.MoveWindow(x+247, y-16, 86, 20);			//2015.11.02 KSJ ����˻��޺��ڽ�
		m_search.MoveWindow(x+337, y-16, 112, 20);				//����˻�
		m_list1.MoveWindow(x+247, y+23, 222, 219);				//���񸮽�Ʈ �ڽ�
		((CStatic *)GetDlgItem(IDC_LIST1_STATIC))->MoveWindow(x+247, y+8, 90, 15);
		
		btnx = x+246;
		btny = y+247;
		m_btnSort01.MoveWindow(btnx, btny, 26, 21);				//"��"
		m_btnSort02.MoveWindow(btnx+28, btny, 26, 21);
		m_btnSort03.MoveWindow(btnx+56, btny, 26, 21);
		m_btnSort04.MoveWindow(btnx+84, btny, 26, 21);
		m_btnSort05.MoveWindow(btnx+112, btny, 26, 21);
		m_btnSort06.MoveWindow(btnx+140, btny, 26, 21);
		m_btnSort07.MoveWindow(btnx+168, btny, 26, 21);
		m_btnSort08.MoveWindow(btnx+196, btny, 26, 21);
		m_btnSort09.MoveWindow(btnx, btny+23, 26, 21);
		m_btnSort10.MoveWindow(btnx+28, btny+23, 26, 21);
		m_btnSort11.MoveWindow(btnx+56, btny+23, 26, 21);
		m_btnSort12.MoveWindow(btnx+84, btny+23, 26, 21);
		m_btnSort13.MoveWindow(btnx+112, btny+23, 26, 21);
		m_btnSort14.MoveWindow(btnx+140, btny+23, 26, 21);
		m_btnSort16.MoveWindow(btnx+168, btny+23, 26, 21);
		m_btnSort17.MoveWindow(btnx+196, btny+23, 26, 21);
		m_btnSort18.MoveWindow(btnx, btny+46, 26, 21);
		m_btnSort19.MoveWindow(btnx+28, btny+46, 26, 21);
		m_btnSort20.MoveWindow(btnx+56, btny+46, 26, 21);
		m_btnSort15.MoveWindow(btnx+84, btny+46, 54, 21);	//����
		m_btnSort21.MoveWindow(btnx+140, btny+46, 26, 21);
		m_btnSort22.MoveWindow(btnx+168, btny+46, 54, 21);		//"�����"
		
		funtionx = x+490;
		funtiony = y+20;
		m_btn_Add.MoveWindow(funtionx, funtiony, 63, 26);		//�߰�
		m_btn_Del.MoveWindow(funtionx, funtiony+28, 63, 26);
		m_btn_AddAll.MoveWindow(funtionx, funtiony+70, 63, 26);
		m_btn_DelAll.MoveWindow(funtionx, funtiony+98, 63, 26);
		m_btn_Blank.MoveWindow(funtionx, funtiony+140, 63, 26);
		m_btn_MoveUp.MoveWindow(funtionx, funtiony+168, 63, 26);
		m_btn_MoveDn.MoveWindow(funtionx, funtiony+196, 63, 26);
		m_btnMoveTop.MoveWindow(funtionx, funtiony+224, 63, 26);
		m_btnMoveBtm.MoveWindow(funtionx, funtiony+252, 63, 26);
		
		gx = x+572;
		gy = y+270;
		m_gname.MoveWindow(gx+60, y-18, 160, 20);				//�׷��
		m_list2.MoveWindow(gx, y+23, 219, 243);
		((CStatic *)GetDlgItem(IDC_LIST2_STATIC))->MoveWindow(gx, y+8, 90, 15);
		((CEdit *)GetDlgItem(IDC_SUM))->MoveWindow(gx+170, y+8, 32, 15);
		((CStatic *)GetDlgItem(IDC_SUM_STATIC))->MoveWindow(gx+202, y+8, 20, 15);
		m_btn_newGroup.MoveWindow(gx, gy, 109, 21);				//���׷�
		m_editGroup.MoveWindow(gx+110, gy, 109, 21);			//�׷�����
		m_btn_ShareImport.MoveWindow(gx, gy+22, 109, 21);		//�׷�ҷ�����
		m_btn_ShareExport.MoveWindow(gx+110, gy+22, 109, 21);	//�׷쳻������
	}
	else
	{
		constexpr int x = 10;
		constexpr int y = 20;
		int btnx{}, btny{}, funtionx{}, funtiony{}, gx{}, gy{};
		m_tree.SetWindowPos(nullptr,x, y, 240, 305, SWP_NOSIZE);						//Ʈ���޴�
		//	m_btn_dnLoadR.SetWindowPos(nullptr, x-6, y+290, 240, 23, SWP_NOSIZE);			//�������񺹱�
		//	m_btn_dnLoad.SetWindowPos(nullptr, (x-6)+240, y+290, 240, 23, SWP_NOSIZE);			//���������ޱ�
		
		m_cbSearch.SetWindowPos(nullptr, x+247, y-16, 86, 20, SWP_NOSIZE);			//2015.11.02 KSJ ����˻��޺��ڽ�
		m_search.SetWindowPos(nullptr, x+337, y-16, 112, 20, SWP_NOSIZE);				//����˻�
		m_list1.SetWindowPos(nullptr, x+247, y+23, 222, 219, SWP_NOSIZE);				//���񸮽�Ʈ �ڽ�
		
		btnx = x+246;
		btny = y+247;
		m_btnSort01.SetWindowPos(nullptr, btnx, btny, 26, 21, SWP_NOSIZE);				//"��"
		m_btnSort02.SetWindowPos(nullptr, btnx+28, btny, 26, 21, SWP_NOSIZE);
		m_btnSort03.SetWindowPos(nullptr, btnx+56, btny, 26, 21, SWP_NOSIZE);
		m_btnSort04.SetWindowPos(nullptr, btnx+84, btny, 26, 21, SWP_NOSIZE);
		m_btnSort05.SetWindowPos(nullptr, btnx+112, btny, 26, 21, SWP_NOSIZE);
		m_btnSort06.SetWindowPos(nullptr, btnx+140, btny, 26, 21, SWP_NOSIZE);
		m_btnSort07.SetWindowPos(nullptr, btnx+168, btny, 26, 21, SWP_NOSIZE);
		m_btnSort08.SetWindowPos(nullptr, btnx+196, btny, 26, 21, SWP_NOSIZE);
		m_btnSort09.SetWindowPos(nullptr, btnx, btny+23, 26, 21, SWP_NOSIZE);
		m_btnSort10.SetWindowPos(nullptr, btnx+28, btny+23, 26, 21, SWP_NOSIZE);
		m_btnSort11.SetWindowPos(nullptr, btnx+56, btny+23, 26, 21, SWP_NOSIZE);
		m_btnSort12.SetWindowPos(nullptr, btnx+84, btny+23, 26, 21, SWP_NOSIZE);
		m_btnSort13.SetWindowPos(nullptr, btnx+112, btny+23, 26, 21, SWP_NOSIZE);
		m_btnSort14.SetWindowPos(nullptr, btnx+140, btny+23, 26, 21, SWP_NOSIZE);
		m_btnSort16.SetWindowPos(nullptr, btnx+168, btny+23, 26, 21, SWP_NOSIZE);
		m_btnSort17.SetWindowPos(nullptr, btnx+196, btny+23, 26, 21, SWP_NOSIZE);
		m_btnSort18.SetWindowPos(nullptr, btnx, btny+46, 26, 21, SWP_NOSIZE);
		m_btnSort19.SetWindowPos(nullptr, btnx+28, btny+46, 26, 21, SWP_NOSIZE);
		m_btnSort20.SetWindowPos(nullptr, btnx+56, btny+46, 26, 21, SWP_NOSIZE);
		m_btnSort15.SetWindowPos(nullptr, btnx+84, btny+46, 26, 21, SWP_NOSIZE);
		m_btnSort21.SetWindowPos(nullptr, btnx+140, btny+46, 26, 21, SWP_NOSIZE);
		m_btnSort22.SetWindowPos(nullptr, btnx+168, btny+46, 26, 21, SWP_NOSIZE);		//"�����"
		
		funtionx = x+490;
		funtiony = y+20;
		m_btn_Add.SetWindowPos(nullptr, funtionx, funtiony, 63, 26, SWP_NOSIZE);		//�߰�
		m_btn_Del.SetWindowPos(nullptr, funtionx, funtiony+28, 63, 26, SWP_NOSIZE);
		m_btn_AddAll.SetWindowPos(nullptr, funtionx, funtiony+70, 63, 26, SWP_NOSIZE);
		m_btn_DelAll.SetWindowPos(nullptr, funtionx, funtiony+98, 63, 26, SWP_NOSIZE);
		m_btn_Blank.SetWindowPos(nullptr, funtionx, funtiony+140, 63, 26, SWP_NOSIZE);
		m_btn_MoveUp.SetWindowPos(nullptr, funtionx, funtiony+168, 63, 26, SWP_NOSIZE);
		m_btn_MoveDn.SetWindowPos(nullptr, funtionx, funtiony+196, 63, 26, SWP_NOSIZE);
		m_btnMoveTop.SetWindowPos(nullptr, funtionx, funtiony+224, 63, 26, SWP_NOSIZE);
		m_btnMoveBtm.SetWindowPos(nullptr, funtionx, funtiony+252, 63, 26, SWP_NOSIZE);
		
		gx = x+572;
		gy = y+270;
		m_gname.SetWindowPos(nullptr, gx+60, y-18, 200, 20, SWP_NOSIZE);				//�׷��
		m_list2.SetWindowPos(nullptr, gx, y+23, 219, 243, SWP_NOSIZE);
		m_btn_newGroup.SetWindowPos(nullptr, gx, gy, 109, 21, SWP_NOSIZE);				//���׷�
		m_editGroup.SetWindowPos(nullptr, gx+110, gy, 109, 21, SWP_NOSIZE);			//�׷�����
		m_btn_ShareImport.SetWindowPos(nullptr, gx, gy+22, 109, 21, SWP_NOSIZE);		//�׷�ҷ�����
		m_btn_ShareExport.SetWindowPos(nullptr, gx+110, gy+22, 109, 21, SWP_NOSIZE);	//�׷쳻������
	}
}

void CPage1::initList()
{
	if(IsWin8())
	{
		AddColumn(&m_list1, "�ڵ� ��", 0, 42);
		AddColumn(&m_list1, "�����", 1, 80);
		m_list1.SetExtendedStyle(LVS_EX_FULLROWSELECT);
		
		AddColumn(&m_list2, "�ڵ�", 0, 46);
		AddColumn(&m_list2, "�����", 1, 90);
		m_list2.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	}
	else
	{
		AddColumn(&m_list1, "�ڵ� ��", 0, 46);
		AddColumn(&m_list1, "�����", 1, 105);
		m_list1.SetExtendedStyle(LVS_EX_FULLROWSELECT);
		
		AddColumn(&m_list2, "�ڵ�", 0, 46);
		AddColumn(&m_list2, "�����", 1, 105);
		m_list2.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	}
}

void CPage1::initTree()
{
	HTREEITEM hRoot{};
	HTREEITEM hItem{}, hItem2{};

	PBCODE pbcode;
	CString code, name;
	_sjcode sjcode;

	hRoot = m_tree.InsertItem(_T("�ֽ�����"), 0, 1);
	m_tree.SetItemData(m_tree.InsertItem(_T("������"), hRoot, TVI_LAST), xALLCODE);
	m_tree.SetItemData(m_tree.InsertItem(_T("�ŷ���"), hRoot, TVI_LAST), xKOSPI);
	m_tree.SetItemData(m_tree.InsertItem(_T("�ڽ���"), hRoot, TVI_LAST), xKOSDAQ);
	m_tree.SetItemData(m_tree.InsertItem(_T("�ڳؽ�"), hRoot, TVI_LAST), xKONEX);	//2013.06.25 KSJ �ڳؽ� �߰�
	m_tree.SetItemData(m_tree.InsertItem(_T("K-OTC"), hRoot, TVI_LAST), xFREECODE);
	//m_tree.SetItemData(m_tree.InsertItem(_T("ETF"), hRoot, TVI_LAST), xETFCODE);
	
	hItem = m_tree.InsertItem(_T("ETF"), hRoot, TVI_LAST);
	m_tree.SetItemData(hItem, 0);
	ETFInitialTree(hItem);

	m_tree.SetItemData(m_tree.InsertItem(_T("ETN"), hRoot, TVI_LAST), xETNCODE);	//2013.06.25 KSJ �ڳؽ� �߰�
	m_tree.SetItemData(m_tree.InsertItem(_T("����"), hRoot, TVI_LAST), xSPACCODE);	//2015.11.03 KSJ spac �߰�

	hRoot = m_tree.InsertItem(_T("ELW"), 0, 0);
	m_tree.SetItemData(hRoot, 0);

	hItem = m_tree.InsertItem("������", 0, 1, hRoot);
	m_tree.SetItemData(hItem, 0);
	
	hItem2 = m_tree.InsertItem("�Ϲ�ELW", 0, 1, hItem);
	m_tree.SetItemData(hItem2, xELWPB);
	
	hItem2 = m_tree.InsertItem("��������ELW", 0, 1, hItem);
	m_tree.SetItemData(hItem2, xELWPB);
	
	hItem = m_tree.InsertItem("�����ڻ�����", 0, 1, hRoot);
	m_tree.SetItemData(hItem, 0);
	
	hItem2 = m_tree.InsertItem("KOSPI200", 0, 1, hItem);
	m_tree.SetItemData(hItem2, xELWPB);
	
	hItem2 = m_tree.InsertItem("BASKET", 0, 1, hItem);
	m_tree.SetItemData(hItem2, xELWPB);
	
	const int nelwbase = m_elwbase.GetSize();
	for(int ii = 0; ii < nelwbase; ii++)
	{
		sjcode = m_elwbase.GetAt(ii);
		code = sjcode.code;
		name = sjcode.name;
		
		hItem2 = m_tree.InsertItem(name, 0, 1, hItem);
		m_tree.SetItemData(hItem2, xELWPB);
	}
	
	
	hItem = m_tree.InsertItem("�����",0,1, hRoot);
	m_tree.SetItemData(hItem,0);
	
	const int nPBcode = m_arrayPBCode.GetSize();
	for(int ii = 0; ii < nPBcode; ii++)
	{
		pbcode = m_arrayPBCode.GetAt(ii);
		
		code = CString(pbcode.code, 12);
		name = CString(pbcode.name, 20);
		
		name.TrimLeft("+");
		name.TrimRight();
		
		hItem2 = m_tree.InsertItem(name, 0, 1, hItem);		
		m_tree.SetItemData(hItem2, xELWPB);
		
	}

	m_tree.SetItemData(m_tree.InsertItem(_T("KOSPI����"), 0, 0), xKOSPIFUTURE);
	m_tree.SetItemData(m_tree.InsertItem(_T("�ֽļ���"), 0, 0), xSTOCKFUTURE);
	m_tree.SetItemData(m_tree.InsertItem(_T("�ݸ�����"), 0, 0), xINTERESTRATEFUTURE);
	m_tree.SetItemData(m_tree.InsertItem(_T("��ȭ����"), 0, 0), xCURRENCYFUTURE);

	hRoot = m_tree.InsertItem(_T("KOSPI�ɼ�"), 0, 0);
	m_tree.SetItemData(m_tree.InsertItem(_T("�ݿɼ�"), hRoot, TVI_SORT), xCALLCODE);
	m_tree.SetItemData(m_tree.InsertItem(_T("ǲ�ɼ�"), hRoot, TVI_SORT), xPUTCODE);

	hRoot = m_tree.InsertItem(_T("KOSPI����"), 0, 0);
	SetItemDataUJ(hRoot, xKOSPIUP);
	
	hRoot = m_tree.InsertItem(_T("KOSDAQ����"), 0, 0);
	SetItemDataUJ(hRoot, xKOSDAQUP);

	m_tree.SetItemData(m_tree.InsertItem(_T("KOSPI200"), 0, 0), xKOPSI200);
	m_tree.SetItemData(m_tree.InsertItem(_T("KRX100"), 0, 0), xKRX100);
	m_tree.SetItemData(m_tree.InsertItem(_T("�����μ���"), 0, 0), xSINJUCODE);

	hRoot = m_tree.InsertItem(_T("��������"), 0, 0);	
	m_tree.SetItemData(m_tree.InsertItem(_T("�ŷ���"),   hRoot, TVI_SORT), xKOSPIJISU);
	m_tree.SetItemData(m_tree.InsertItem(_T("�ڽ���"), hRoot, TVI_SORT), xKOSDAQJISU);

	// ��������
	hRoot = m_tree.InsertItem(_T("��������"), 0, 0);
	SetItemDataX(hRoot, xTHEMACODE);
	
	// �׷�纰
	hRoot = m_tree.InsertItem(_T("�׷�纰"), 0, 0);
	SetItemDataX(hRoot, xGROUPCODE);

	// IBK��õ����
	//m_tree.SetItemData(m_tree.InsertItem(_T("IBK��õ����"), 0, 0), xRecommand);  //������ �ʴ´�
}

void CPage1::ETFInitialTree(HTREEITEM hRoot)
{
	
	_efopitem* efopitem{};
	_efoptema* efoptema{};
	_efopfore* efopfore{};
	
	CString data, line;
	CString type, opcd, hnam;
	CString tmcd, frcd, frnm, code, etfm, etfl;
	
	HTREEITEM hItem2{}, hItem3{};
	
	
	hItem2 = m_tree.InsertItem("��ü����",0,1, hRoot);
	m_tree.SetItemData(hItem2, xETFALLCODE);
	
	hItem2 = m_tree.InsertItem("���纰",0,1, hRoot);
	m_tree.SetItemData(hItem2,xETFALLCODE);
	
	const int nETFitem = m_ETFitem.GetSize();
	for(int ii = 0; ii < nETFitem; ii++)
	{
		efopitem = m_ETFitem.GetAt(ii);
		
		type = CString(efopitem->type, sizeof(efopitem->type));
		opcd = CString(efopitem->opcd, sizeof(efopitem->opcd));
		hnam = CString(efopitem->hnam, sizeof(efopitem->hnam));
		hnam.TrimLeft(); hnam.TrimRight();
		
		
		hItem3 = m_tree.InsertItem(hnam, 0, 1, hItem2);		
		m_tree.SetItemData(hItem3, xETFALLCODE);
		
	}
	
	hItem2 = m_tree.InsertItem("�����ڻ꺰",0,1, hRoot);	//2015.11.04 KSJ �׸��� --> �����ڻ꺰�� ����
	m_tree.SetItemData(hItem2, xETFALLCODE);
	
	const int nETFtema = m_ETFtema.GetSize();
	for(int ii = 0; ii < nETFtema; ii++)
	{
		efoptema = m_ETFtema.GetAt(ii);
		
		type = CString(efoptema->type, sizeof(efoptema->type));
		tmcd = CString(efoptema->tmcd, sizeof(efoptema->tmcd));
		hnam = CString(efoptema->tmnm, sizeof(efoptema->tmnm));
		hnam.TrimLeft(); hnam.TrimRight();
		
		
		
		hItem3 = m_tree.InsertItem(hnam, 0, 1, hItem2);		
		m_tree.SetItemData(hItem3, xETFALLCODE);
		
	}
	
	hItem2 = m_tree.InsertItem("����/��",0,1,hRoot);
	m_tree.SetItemData(hItem2, xETFALLCODE);
	
	const int nETFfore = m_ETFfore.GetSize();
	for(int ii = 0; ii < nETFfore; ii++)	
	{
		efopfore = m_ETFfore.GetAt(ii);
		
		type = CString(efopfore->type, sizeof(efopfore->type));
		frcd = CString(efopfore->frcd, sizeof(efopfore->frcd));
		frnm = CString(efopfore->frnm, sizeof(efopfore->frnm));
		frnm.TrimLeft(); frnm.TrimRight();
		
		hItem3 = m_tree.InsertItem(frnm, 0, 1, hItem2);		
		m_tree.SetItemData(hItem3, xETFALLCODE);
	}
	
}

void CPage1::initCombo(bool isEditOk)
{
	int	idx{};	
	CString	 string, gno, gname, filePath,stmp;
	m_gname.ResetContent();

#ifdef DF_SEARCH
	CWnd* wnd = GetParent()->GetParent();
	memset(m_pdata, 0x00, 1024 * 4);
	const bool ret = wnd->SendMessage(WM_MSG, MAKEWPARAM(MSG_GET_GROUPARR, 0), (LPARAM)m_pdata);
	if (ret == 0)
		return;

	string.Format("%s", m_pdata);

	int ii = 0;
	//�ʱ�ȭ
	if (isEditOk == false)
	{
		for (int kk = 0; kk < MAXGROUP; kk++)
		{
			for (int jj = 0; jj < 4; jj++)
			{
				m_manageGroup[kk][jj] = "";
			}
		}
	}

	while (!string.IsEmpty())
	{
		stmp = parseX(string, "\t");
		gno = parseX(stmp, ";");
		gname = stmp;
		if (gno.IsEmpty() || gno.GetLength() != 2)
			break;

		idx = m_gname.AddString(gname);
		m_gname.SetItemData(idx, atoi(gno));

		//�׷� ���� �迭 �ʱ� ����
		if (isEditOk == false)
		{
			m_manageGroup[ii][0] = gno;
			m_manageGroup[ii][1] = "";
			m_manageGroup[ii][2] = gno;
			m_manageGroup[ii][3] = gname;
		}

		ii++;
	}

	idx = m_gname.SetCurSel(0);
	m_activegno = m_gname.GetItemData(idx);
	loadingInterest(m_activegno);
	//if (m_activegno == -1 && m_gname.GetCount() > 0)
	//{
	//	if (m_selectGroup < 0)
	//		idx = m_gname.SetCurSel(0);
	//	else
	//		idx = m_gname.SetCurSel(m_selectGroup);//BUFFET 0

	//	m_activegno = m_gname.GetItemData(idx);
	//}

	//const int nowCursel = m_gname.GetCurSel();

	//if (nowCursel == -1)
	//{
	//	if (m_activegno > -1)
	//	{
	//		if (m_selectGroup < 0)
	//			idx = m_gname.SetCurSel(0);
	//		else
	//			idx = m_gname.SetCurSel(m_selectGroup);//BUFFET 0
	//	}
	//	else
	//	{
	//		idx = m_gname.SetCurSel(0);
	//	}

	//	m_activegno = m_gname.GetItemData(idx);
	//}
	
#else
	CFileFind finder;

//	if (!finder.FindFile(filePath))	
	
	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);

	readL = GetPrivateProfileString(_T("GROUPORDER"), "00", "", readB, sizeof(readB), filePath);

	if (readL <= 0)
		return;

	string = CString(readB, readL);
	
	int ii = 0;

	//�ʱ�ȭ
	if(isEditOk == false)
	{
		for(int kk = 0 ; kk< MAXGROUP ; kk++)
		{
			for(int jj =0 ; jj < 4 ; jj++)
			{
				m_manageGroup[kk][jj] = "";
			}
		}
	}

	while (!string.IsEmpty())
	{
		gno = parseX(string, ";");
		if (gno.IsEmpty() || gno.GetLength() != 2)
			break;
		
		readL = GetPrivateProfileString("GROUPNAME", gno, "", readB, sizeof(readB), filePath);
		gname = CString(readB, readL);
		
		idx = m_gname.AddString(gname); 
		m_gname.SetItemData(idx, atoi(gno));
		
		//�׷� ���� �迭 �ʱ� ����
		if(isEditOk == false)
		{
			m_manageGroup[ii][0] = gno;
			m_manageGroup[ii][1] = "";
			m_manageGroup[ii][2] = gno;
			m_manageGroup[ii][3] = gname;
		}

		ii++;
	}

	if (m_activegno == -1 && m_gname.GetCount() > 0)
	{
		if (m_selectGroup < 0 )
			idx = m_gname.SetCurSel(0);
		else
			idx = m_gname.SetCurSel(m_selectGroup);//BUFFET 0

		m_activegno = m_gname.GetItemData(idx);
	}
	
	const int nowCursel = m_gname.GetCurSel();

	if(nowCursel == -1)
	{
		if(m_activegno > -1)
		{
			if (m_selectGroup < 0 )
				idx = m_gname.SetCurSel(0);
			else
				idx = m_gname.SetCurSel(m_selectGroup);//BUFFET 0
		}
		else
		{
			idx = m_gname.SetCurSel(0);
		}

		m_activegno = m_gname.GetItemData(idx);
	}
#endif
}

void CPage1::SetItemDataUJ(HTREEITEM hItem, int gubn)
{
	int jgub{};
	switch (gubn)
	{
	case xKOSPIUP:
		jgub = upKOSPI;
		break;
	case xKOSDAQUP:
		jgub = upKOSDAQ;
		break;
	}

	CString name, string;
	struct	upcode	upcode;
	for (int ii = 0 ; ii < m_upcode.GetSize() ; ii++)
	{
		upcode = m_upcode.GetAt(ii);
		if (upcode.jgub == jgub)
		{
			name = CString(upcode.hnam, UNameLen);
			name.TrimLeft(); name.TrimRight();
		
			string.Format("%d%02d", gubn, (int)upcode.ucod);
			m_tree.SetItemData(m_tree.InsertItem(name, hItem), atoi(string));
		}
	}

	if (jgub == upKOSPI)
	{
		for (int ii = 0; ii < cnt_exKospi; ii++)
		{
			string.Format("%d%s", xKOSPIex, exKospi[ii].code);
			m_tree.SetItemData(m_tree.InsertItem(exKospi[ii].name, hItem), atoi(string));
		}
	}
/*
	else if (jgub == upKOSDAQ)
	{
		for (ii = 0; ii < cnt_exKosdaq; ii++)
		{
			string.Format("%d%s", xKOSDAQex, exKosdaq[ii].code);
			m_tree.SetItemData(m_tree.InsertItem(exKosdaq[ii].name, hItem, nullptr), atoi(string));
		}
	}
*/	//2016.05.16 KSJ KOSTAR ���� ����
}

void CPage1::SetItemDataX(HTREEITEM hItem, int gubn)
{
	switch (gubn)
	{
	case xTHEMACODE:
		{
			int readL = 0; char readB[256]{};
			CString	filePath, string, stringx, data, skey;

			filePath.Format("%s/%s/%s", m_root, TABDIR, "themcd.ini");

			for (int ii = 0; ii < 200; ii++)
			{
				skey.Format("%02d", ii);
				readL = GetPrivateProfileString(_T("FOLDER"), skey, "", readB, sizeof(readB), filePath);
				if (readL <= 0) break;

				string = CString(readB, readL); 
				stringx = parseX(string, ";"); 
				stringx.TrimRight();
				data.Format("%02d%03d", xTHEMACODE, atoi(string));
				m_tree.SetItemData(m_tree.InsertItem(stringx, hItem), atoi(data));
			}
		}
		break;

	case xGROUPCODE:
		{
			int readL = 0; 
			char readB[256]{};
			CString	filePath, string, stringx, data, skey;

			filePath.Format("%s/%s/%s", m_root, TABDIR, "gropcd.ini");

			for (int ii = 0; ii < 200; ii++)
			{
				skey.Format("%02d", ii);
				readL = GetPrivateProfileString(_T("FOLDER"), skey, "", readB, sizeof(readB), filePath);
				if (readL <= 0) break;

				string = CString(readB, readL);	
				stringx = parseX(string, ";"); 
				stringx.TrimRight();
				data.Format("%02d%03d", xGROUPCODE, atoi(string));
				m_tree.SetItemData(m_tree.InsertItem(stringx, hItem), atoi(data));
			}
		}
		break;
	case xELWISSUE:
		{
			int readL = 0; 
			char readB[256]{};
			CString	filePath, string, string_code, string_name, skey, data;
			
			filePath.Format("%s/%s/%s", m_root, TABDIR, "ELWLP.CBS");
			
			for(int ii=0 ; ii<200 ; ii++)
			{
				skey.Format("%d", ii);
				readL = GetPrivateProfileString("Items", skey, "", readB, sizeof(readB), filePath);
				if(readL <= 0) break;

				string = CString(readB, readL);
				string.TrimRight();
				string_code = string.Left(12);
				parseX(string, string_code); 
				
				if(ii>0)		//��ü�� ǥ�þ���
				{
					data.Format("%02d%03d", xELWISSUE, ii);
					m_tree.SetItemData(m_tree.InsertItem(string, hItem, TVI_LAST), atoi(data));
				}
			}
		}
		break;

	case xELWASSET:
		{
			int readL = 0; 
			char readB[256]{};
			CString	filePath, string, string_code, string_name, skey, data;
			HTREEITEM hMid = TVI_LAST;
			filePath.Format("%s/%s/%s", m_root, TABDIR, "ELWUD.CBS");

			m_tree.SetItemData(m_tree.InsertItem(_T("��"), hItem, TVI_LAST), xELWASSET);
			m_tree.SetItemData(m_tree.InsertItem(_T("ǲ"), hItem, TVI_LAST), xELWASSET);

			HTREEITEM hChild = m_tree.GetChildItem(hItem);
			CString child_name;
			
			while(hChild)
			{
				child_name = m_tree.GetItemText(hChild);
				if(child_name == "��")
				{
					for(int ii=0 ; ii<200 ; ii++)
					{
						skey.Format("%d", ii);
						readL = GetPrivateProfileString("Items", skey, "", readB, sizeof(readB), filePath);
						if(readL <= 0) break;
						
						string = CString(readB, readL);
						string.TrimRight();
						string_code = string.Left(12);
						parseX(string, string_code); 
						
						if(ii>1) //��ü�� ǥ�þ���
						{
							data.Format("%02d%d%03d", xELWASSET, 1, ii);
							m_tree.SetItemData(m_tree.InsertItem(string, hChild, TVI_LAST), atoi(data));
						}
					}
				}
				else if(child_name == "ǲ")
				{
					for(int ii=0 ; ii<200 ; ii++)
					{
						skey.Format("%d", ii);
						readL = GetPrivateProfileString("Items", skey, "", readB, sizeof(readB), filePath);
						if(readL <= 0) break;
						
						string = CString(readB, readL);
						string.TrimRight();
						string_code = string.Left(12);
						parseX(string, string_code); 
						
						if(ii>1) //��ü�� ǥ�þ���
						{
							data.Format("%02d%d%03d", xELWASSET, 2, ii);
							m_tree.SetItemData(m_tree.InsertItem(string, hChild, TVI_LAST), atoi(data));
						}
					}
				}

				hChild = m_tree.GetNextSiblingItem(hChild);
			}

			
		}
		break;

	case xINTEREST:
		{
			//int readL = 0; 
			//char readB[512]{}; //���ɱ׷� ������ 256->512
			//CString	filePath, string, gno, gname, data;

			//filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);

			//readL = GetPrivateProfileString(_T("GROUPORDER"), "00", "", readB, sizeof(readB), filePath);
			//if (readL <= 0)	return;

			//string = CString(readB, readL);

			//while (!string.IsEmpty())
			//{
			//	gno = parseX(string, ";");
			//	if (gno.IsEmpty() || gno.GetLength() != 2)
			//		break;

			//	readL = GetPrivateProfileString("GROUPNAME", gno, "", readB, sizeof(readB), filePath);
			//	gname = CString(readB, readL); 
			//	data.Format("%02d%s", xINTEREST, gno);

			//	if (ExistFile(atoi(gno)))
			//		m_tree.SetItemData(m_tree.InsertItem(gname, hItem), atoi(data));
			//}
		}
		break;
	}
}

void CPage1::ClearListitem()
{
	m_gListsort = FALSE;
	for (int ii = 0; ii < m_listitem.GetSize(); ii++)
		delete m_listitem.GetAt(ii);
	m_listitem.RemoveAll();
	m_arList.RemoveAll();
}

void CPage1::ClearList2item()
{
	for (int ii = 0; ii < m_list2item.GetSize(); ii++)
		delete m_list2item.GetAt(ii);
	m_list2item.RemoveAll();
}

void CPage1::ClearInterest()
{
	for (int ii = 0; ii < m_inters.GetSize(); ii++)
		delete m_inters.GetAt(ii);
	m_inters.RemoveAll();
}

void CPage1::selcMark(CListCtrl* listctrl, int nItem, bool scroll /*=true*/)
{
	CRect	rc; CSize size;

	listctrl->SetItemState(nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

	if (scroll)
	{
		listctrl->GetItemRect(0, rc, LVIR_LABEL);
		size.cx = 0;
		size.cy = rc.Height() * (nItem - listctrl->GetTopIndex());
		listctrl->Scroll(size);
	}
}

void CPage1::deselcMark(CListCtrl* listctrl)
{
	int nItem{};
	POSITION pos = listctrl->GetFirstSelectedItemPosition();
	if (pos != nullptr)
	{
		while (pos)
		{
			nItem = listctrl->GetNextSelectedItem(pos);
			listctrl->SetItemState(nItem, NULL, LVIS_SELECTED|LVIS_FOCUSED);
		}
	}
}

void CPage1::insertList1toList2(int sitem)
{
	CString	gname;
	if (m_activegno == -1)
	{
		GuideMsg("���õ� �׷��� �����ϴ�. ���׷��� ����ϼ���", false);
		return;
	}
		
	m_bIsEdit = TRUE;
	m_bFirstDuplicateNO = false;		//2012.04.02 KSJ ó���� ����Ʈ��..

	deselcMark(&m_list2);

	int	nitem{};
	CString	code, name;

	POSITION pos = m_list1.GetFirstSelectedItemPosition();

	while (pos)
	{
		nitem = m_list1.GetNextSelectedItem(pos);

		code = m_list1.GetItemText(nitem, 0);
		name = m_list1.GetItemText(nitem, 1);

		
		if (existCodeInList2(code))
		{	
//			GuideMsg("������ ������ ���ɱ׷� ���� �����մϴ�.");
			continue;
		}

		if (m_list2.GetItemCount() >= maxJONGMOK)
		{
			GuideMsg(idTOstring(IDS_GUIDE1));
			break;
		}

		int gubn = m_tree.GetItemData(m_tree.GetSelectedItem());
		
		CString temp;

		if(gubn < 100000 && gubn > 10000)
		{
			temp.Format("%05d", gubn);
			temp = temp.Mid(0,2);
			gubn = atoi(temp);
		}
		else if(gubn >= 100000)
		{
			
			temp.Format("%06d", gubn);
			temp = temp.Mid(0,2);
			gubn = atoi(temp);
		}
			
		if (sitem < 0)
		{
			const int position = m_list2.GetItemCount();
			AppendItemToList2(position, gubn, code, name);

			selcMark(&m_list2, position);
		}
		else	// drag
		{
			AppendItemToList2(sitem++, gubn, code, name, _T("0"), false);
		}
	}
	InitEdit();
}

void CPage1::deleteList2()
{
	if (m_activegno == -1)
	{
		GuideMsg("���õ� �׷��� �����ϴ�. ���׷��� ����ϼ���", false);
		return;
	}
	
	m_bIsEdit = TRUE;

	int nItem = -1;
	POSITION pos{};
	int count = m_list2.GetItemCount();
	for (int ii = 0; ii < count; ii++)
	{
		pos = m_list2.GetFirstSelectedItemPosition();
		if (pos != nullptr)
		{
			nItem = m_list2.GetNextSelectedItem(pos);
			DeleteItemToList2(nItem);
		}
	}

	count = m_list2.GetItemCount();
	if (count > nItem)
		selcMark(&m_list2, nItem, false);
	else
		selcMark(&m_list2, nItem -1, false);
}

void CPage1::searchAction(int column)
{
	CString	string; 
	m_search.GetWindowText(string);
	m_list1.UpdateData(false);
	if (column == 0)	// number search
	{
		if (!m_bAddAll)
		{
			CString	code, name;
			_listitem* listitem{};
			int	position = 0;
			m_list1.DeleteAllItems();
			m_arList.RemoveAll();
			for (int ii = 0; ii < m_listitem.GetSize(); ii++)
			{
				listitem = m_listitem.GetAt(ii);

				name = CString(listitem->name, sizeof(listitem->name));
				code = CString(listitem->code, sizeof(listitem->code));
				AppendItemToList1(position, code, name, listitem);
				position += 1;
			}
			m_bAddAll = true;
			m_list1.SetItemCountEx(m_arList.GetSize());
			m_list1.Invalidate();
		}

		if (m_list1.GetItemCount() > 0)
		{
			int	idx = -1;
			if (string.GetLength() > 0)
			{
				m_listsort = false;
				ListView_SortItems(m_list1, ListViewCompareFunc, column);

				deselcMark(&m_list1);

				idx = findMatchIndex(string, column);
				if (idx >= 0) selcMark(&m_list1, idx);
			}
			else
			{
				deselcMark(&m_list1);
				selcMark(&m_list1, 0);

			}
		}
	}
	else			// name search
	{
		m_bAddAll = SearchData(m_cbSearch.GetCurSel(), string, m_bAddAll);
		if (m_list1.GetItemCount() > 0)
		{
			m_listsort = false;
			ListView_SortItems(m_list1, ListViewCompareFunc, column);

			deselcMark(&m_list1);
			selcMark(&m_list1, 0);
		}
	}
	m_list1.UpdateData(true);
}

int CPage1::findMatchIndex(CString text)
{
	const int nameCOL = 1;

	CString	string;
	const int	textL = text.GetLength();
	for (int ii = 0; ii < m_list1.GetItemCount(); ii++)
	{
		string = m_list1.GetItemText(ii, nameCOL);
		string = string.Mid(1);

		if (strncmp(text, string, textL) <= 0)
			return ii;
	}
	return -1;
}

int CPage1::findMatchIndex(CString text, int column)
{
	CString	string;
	const int	textL = text.GetLength();
	for (int ii = 0; ii < m_list1.GetItemCount(); ii++)
	{
		string = m_list1.GetItemText(ii, column);
		if (column == 1) string = string.Mid(1);

		if (!strncmp(text, string, textL))
			return ii;
	}
	return -1;
}

void CPage1::updateList2sum()
{
	CString string;
	string.Format("%d", m_inters.GetSize());
	m_sum.SetWindowText(string);
}


//CImageList*
std::shared_ptr<CImageList> CPage1::CreateDragImageSymbol(LPPOINT lpPoint, CListCtrl* listctrl)
{
	const int count = listctrl->GetSelectedCount();
	if (count < 1) return nullptr;

	
	CRect itemRC, totalRC; totalRC.SetRectEmpty();
	
	POSITION pos = listctrl->GetFirstSelectedItemPosition();
	if (!pos) return nullptr;

	int item = listctrl->GetNextSelectedItem(pos);
	listctrl->GetItemRect(item, itemRC, LVIR_BOUNDS);
	totalRC = itemRC;
	totalRC.bottom = totalRC.top + totalRC.Height() * count;

	CDC memDC; CClientDC dcClient(this);
	if (!memDC.CreateCompatibleDC(&dcClient))
		return nullptr;

	CBitmap bitmap;
	if (!bitmap.CreateCompatibleBitmap(&dcClient, totalRC.Width(), totalRC.Height()))
		return nullptr;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(0, 0, totalRC.Width(), totalRC.Height(), RGB(255, 255, 255));

	auto pCompleteImageList = std::make_shared<CImageList>();
	pCompleteImageList->Create(totalRC.Width(), totalRC.Height(), ILC_COLOR|ILC_MASK, 0, 1);

	int index = 0;
	pos = listctrl->GetFirstSelectedItemPosition();
	while (pos)
	{
		item = listctrl->GetNextSelectedItem(pos);
		listctrl->GetItemRect(item, itemRC, LVIR_LABEL);

		CPoint	pt;
		auto pimglstSingle = std::unique_ptr<CImageList>(listctrl->CreateDragImage(item, &pt));	
		if (pimglstSingle)
		{
			pimglstSingle->Draw(&memDC, 0, CPoint(0, itemRC.Height() * index++), ILD_MASK);
			pimglstSingle.reset();
		}
	}

	memDC.SelectObject(pOldMemDCBitmap);
	pCompleteImageList->Add(&bitmap, RGB(255, 255, 0)); 
	
	if (lpPoint)
	{
		lpPoint->x = itemRC.left;
		lpPoint->y = itemRC.top;
	}
	return pCompleteImageList;
}

void CPage1::SetAutoScroll(CListCtrl* listctrl, int direct)
{
	CRect	rc;
	CSize	size;

	listctrl->GetItemRect(0, rc, LVIR_LABEL);

	const int pos = listctrl->GetTopIndex();

	size.cx = 0;
	size.cy = rc.Height() * ((pos + direct) - pos);

	listctrl->Scroll(size);

	if (direct < 0)
		listctrl->Invalidate();

	m_autoScroll = true;
}

///////////////////////////////////////////////////////////////////////////////
// 
BOOL CPage1::AddColumn(CListCtrl *ctrl, LPCTSTR string, int item, int width)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR) string;
	lvc.cx      = ctrl->GetStringWidth(lvc.pszText) + width;

	return ctrl->InsertColumn(item, &lvc);
}

int CPage1::AddItem(CListCtrl *ctrl, LPCSTR string, int nItem, int nSubItem)
{
	if (nSubItem < 0) // code
		return ctrl->InsertItem(LVIF_TEXT, nItem, string, 0, 0, 0, 0);
	else		  // name
		return ctrl->SetItemText(nItem, nSubItem, string);
}

bool CPage1::ExistFile(int gno, bool temp /*=false*/)
{
	///*CString	filePath;

	//if (temp)
	//	filePath.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, gno);
	//else
	//	filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);

	//CFileFind finder;
	//if (!finder.FindFile(filePath))
	//	return false;*/

	return true;
}

void CPage1::loadingHJcode()
{
	if (m_hjcode.GetSize() > 0) return;

	CFile	fileH;
	CString	filePath;
	struct  hjcodex  hjcode{};
	struct	_shjcode s_hjcode {};
	
	//filePath.Format("%s/%s/%s", m_root, TABDIR, HJCODE);
	filePath.Format("%s/%s/%s", m_root, TABDIR, "hjcode3.dat");
  	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox(idTOstring(IDS_EM_HJCODE));
		return;
	}

	const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct hjcodex));

	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&hjcode, sizeof(struct hjcodex));

		s_hjcode.name = CString(hjcode.hnam, HNameLen);
		const int idx = s_hjcode.name.Find('\0');
		if (idx != -1) s_hjcode.name = s_hjcode.name.Left(idx);
		s_hjcode.name.MakeUpper(); s_hjcode.name.TrimRight();


		s_hjcode.symb = CString(hjcode.symb, HSymbolLen);
		s_hjcode.symb.TrimRight();

		s_hjcode.ecng = hjcode.ecng;
		s_hjcode.size = hjcode.size;
		memcpy(s_hjcode.acdl, hjcode.acdl, strlen(hjcode.acdl));
		//s_hjcode.ucdm = hjcode.ucdm;   //acdl
		memcpy(s_hjcode.ucdm, hjcode.acdm, strlen(hjcode.acdm));
	//	s_hjcode.ucds = hjcode.ucds;
		memcpy(s_hjcode.ucds, hjcode.acds, strlen(hjcode.acds));
		s_hjcode.jjug = hjcode.jjug;
		s_hjcode.kpgb = hjcode.kpgb;
		s_hjcode.ucmd = hjcode.ucmd;
		s_hjcode.kosd = (char)hjcode.kosd;
		s_hjcode.ssgb = hjcode.ssgb;
		s_hjcode.wsgb = hjcode.wsgb;
		s_hjcode.jsiz = hjcode.jsiz;
		s_hjcode.itgb = hjcode.itgb;
		s_hjcode.star = hjcode.star;
		s_hjcode.unio = hjcode.unio;
		s_hjcode.prmr = hjcode.kqpg;

		switch (hjcode.ssgb)
		{
		case jmSINJU:
		case jmSINJS:
		case jmHYFND:
			s_hjcode.code = CString(hjcode.code, HCodeLen);
			break;
		default:
			s_hjcode.code = CString(hjcode.code, HCodeLen).Mid(1);
			break;
		}

		switch (s_hjcode.ssgb)
		{
		case jmHYFND:
//			continue;
		default:
			m_hjcode.Add(s_hjcode);
			break;
		}
    }

	//�̺κ� �߰�
	QSortCArray( m_hjcode, CompareChar ); 
	fileH.Close();
}

void CPage1::loadingFJcode()
{
	if (m_fjcode.GetSize() > 0) return;

	CFile	fileH;
	CString	filePath;
	struct  fjcode  fjcode;
	struct	_sjcode s_jcode;

	filePath.Format("%s/%s/%s", m_root, TABDIR, FJCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox(idTOstring(IDS_EM_FJCODE));
		return;
	}

	const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct fjcode));
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&fjcode, sizeof(struct fjcode));

		s_jcode.code = CString(fjcode.cod2, FCodeLen);
		s_jcode.name = CString(fjcode.hnam, FNameLen);

		m_fjcode.Add(s_jcode);
	}

	fileH.Close();
}

void CPage1::loadingOJcode()
{
	if (m_ojcode.GetSize() > 0) return;

	CFile	fileH;
	CString	filePath;

	struct  ojcodh  ojcodH;
	struct  ojcode  ojcode;

	filePath.Format("%s/%s/%s", m_root, TABDIR, OJCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox(idTOstring(IDS_EM_OPCODE));
		return;
	}

	fileH.Read(&ojcodH, sizeof(struct ojcodh));

	const int countC = gsl::narrow_cast<int>((fileH.GetLength() - sizeof(struct ojcodh)) / sizeof(struct ojcode));
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&ojcode, sizeof(struct ojcode));
		m_ojcode.Add(ojcode);
	}

	fileH.Close();
}

void CPage1::loadingPJcode()
{
	if (m_pjcode.GetSize() > 0) return;

	CFile	fileH;
	CString	filePath;
	struct  pjcode  pjcode;

	filePath.Format("%s/%s/%s", m_root, TABDIR, PJCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox(idTOstring(IDS_EM_PJCODE));
		return;
	}

	const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct pjcode));
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&pjcode, sizeof(struct pjcode));
		m_pjcode.Add(pjcode);
	}

	fileH.Close();
}

void CPage1::loadingUPcode()
{
	if (m_upcode.GetSize() > 0) return;

	CFile	fileH;
	CString	filePath;
	struct  upcode  upcode;

	filePath.Format("%s/%s/%s", m_root, TABDIR, UPCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox(idTOstring(IDS_EM_UPCODE));
		return;
	}

	const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct upcode));
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&upcode, sizeof(struct upcode));
		m_upcode.Add(upcode);
	}

	fileH.Close();
}

void CPage1::loadingELWcode()
{
	if (m_elwbase.GetSize() > 0)
		return;

	
	CFile	file;
	_sjcode	sjcode;
	struct  elwcode  ELWCode;
	_elwdata elwdata;
	PBCODE pbcode;
	//CString code, name, ktype, pcode, pname, mjmt, bkoba;
	int idx{};
	CString bkoba;
	ITEMS item;

	CString path = m_root + "\\tab\\elwcod.dat";
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))	return;

	const int	codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct elwcode));

//	SORTSTR *pBaseSortArr = new SORTSTR[codeN];
	
	bool	bFound{};
	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&ELWCode, sizeof(struct elwcode));
		
		//elwdata = new _elwdata();

		//item = new ITEMS;
		elwdata.code = CString(ELWCode.codx, sizeof(ELWCode.codx));
		elwdata.code.TrimRight();	
		elwdata.name.Format("%s", CString(ELWCode.hnam, ELWHNameLen).Mid(1));
		elwdata.name.TrimRight(); elwdata.name.TrimLeft();
		elwdata.ktype = CString(ELWCode.krye, 2);
		elwdata.pcode = CString(ELWCode.bgce, 12);
		elwdata.pname = CString(ELWCode.hbge, 20);
		elwdata.mjmt  = CString(ELWCode.mjmt, 8);
		elwdata.bkoba = (ELWCode.koba[0]==2) ? TRUE : FALSE;

		for (int jj = 0; jj < 5; jj ++)
		{
			elwdata.bcode[jj] = CString((char*)ELWCode.gcjs[jj], 12);
			elwdata.bcode[jj].TrimRight();
		}

		if(elwdata.bkoba == 0)
			bkoba = "0";
		else
			bkoba = "1";

// 		item.code = elwdata.code;
// 		item.name = elwdata.name;
// 		item.bkoba = elwdata.bkoba;
// 		item.ktype = elwdata.ktype;

		m_ELWdata.Add(elwdata);
		//m_ELWcompany.SetAt(item.code, &item);

	
		idx = FindPBCode(elwdata.pcode);
	
		if (idx < 0)
		{
			pbcode.code = elwdata.pcode;
			pbcode.name = elwdata.pname;
			m_arrayPBCode.Add(pbcode);
		}

		
		for (int jj = 0; jj < 5; jj ++)
		{
			CString bcode = CString((char*)ELWCode.gcjs[jj], 12);
			bcode.TrimRight();
			if (bcode == "") break;

			bcode = bcode.Mid(1);
			
			bFound = false;
			for (int kk = 0; kk < m_elwbase.GetSize(); kk++)
			{
				if (bcode.CompareNoCase(m_elwbase.GetAt(kk).code) == 0)
				{
					bFound = true;
					break;
				}
			}

			if (!bFound)
			{
				CString	bname = GetCodeName(bcode);
				//TRACE("base name : %s\n", bname);
				if (!bname.IsEmpty())	
					;
				else if (bcode.CompareNoCase("OSPI200") == 0)
					continue;
				else if (bcode.CompareNoCase("ASKET") == 0)
					break;
				else
					continue;


				sjcode.code = bcode;
				sjcode.name = bname;

			
				sjcode.name.TrimRight();

				
				
//				m_ELWbase.SetAt(sjcode.code, sjcode);
				m_elwbase.Add(sjcode);
			}
		}

// 		if(elwdata != nullptr)
// 		{
// 			elwdata = nullptr;
// 			delete elwdata;
// 		}
	}

	QSortCArray( m_elwbase, compareSortName); 
	
	file.Close();
}

void CPage1::loadingCFcode(bool bCurrency)//2012.10.04 KSJ �ݸ�(6x), ��ȭ(7x)
{	
	if(bCurrency)
	{
		if (m_cccode.GetSize() > 0) return;	
		
		CFile	fileH;
		CString	filePath, tjgb;
		struct  ccode  ccode;
		
		filePath.Format("%s/%s/%s", m_root, TABDIR, CFCODE);
		if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
		{
			AfxMessageBox("�ݸ�/��ȭ�����ڵ� ������ ���� �� �����ϴ�.");
			return;
		}
		
		const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct ccode));
		for (int ii = 0; ii < countC; ii++)
		{
			fileH.Read(&ccode, sizeof(struct ccode));
			
			tjgb = CString(ccode.tjgb, 2);
			
			if(tjgb.GetAt(0) == '7')
				m_cccode.Add(ccode);
		}
		
		fileH.Close();
	}
	else
	{
		if (m_cicode.GetSize() > 0) return;	
		
		CFile	fileH;
		CString	filePath, tjgb;
		struct  ccode  ccode;
		
		filePath.Format("%s/%s/%s", m_root, TABDIR, CFCODE);
		if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
		{
			AfxMessageBox("�ݸ�/��ȭ�����ڵ� ������ ���� �� �����ϴ�.");
			return;
		}
		
		const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct ccode));
		for (int ii = 0; ii < countC; ii++)
		{
			fileH.Read(&ccode, sizeof(struct ccode));
			
			tjgb = CString(ccode.tjgb, 2);
			
			if(tjgb.GetAt(0) == '6')
				m_cicode.Add(ccode);
		}
		
		fileH.Close();
	}
}

void CPage1::loadingSFcode()	//2012.10.04 KSJ �ֽļ���
{
	if (m_sfcode.GetSize() > 0) return;	
	
	CFile	fileH;
	CString	filePath, strCode;
	struct  sfcode  sfcode;
	
	filePath.Format("%s/%s/%s", m_root, TABDIR, SFCODEDAT);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("�ֽļ����ڵ� ������ ���� �� �����ϴ�.");
		return;
	}
	
	const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct sfcode));
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&sfcode, sizeof(struct sfcode));

// 		strCode = CString(sfcode.codx, 8);
// 		
// 		if(strCode.GetAt(0) == '1')
			m_sfcode.Add(sfcode);
	}
	
	fileH.Close();
}

int CPage1::FindPBCode(CString code)
{
//	bool bRegistered = false;
	PBCODE pbcode;
	for (int ii = 0; ii < m_arrayPBCode.GetSize(); ii++)
	{
		pbcode = m_arrayPBCode.GetAt(ii);
		if (pbcode.code == code)
		{
			return ii;
		}
	}
	return -1;
}
#ifdef DF_SEARCH
int CPage1::loadingInterest(int gno)  //�����Ϸ�
{
	ClearInterest();
	CWnd* wnd = GetParent()->GetParent();
	char* pdata = (char*)wnd->SendMessage(WM_MSG, MSG_SEARCH_GROUPCODE, gno);
	return 0;
}
#else
int CPage1::loadingInterest(int gno)  //�����Ϸ�
{
	ClearInterest();

	CString	filePath, fileBook;

	if (ExistFile(gno))
 		filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);
	else
		return 0;
	
	fileBook.Format("%s/%s/%s/bookmark.i%02d", m_root, USRDIR, m_name, gno);

	UINT	readL{};
	struct	_inters* pinter{};
	struct  _bookmarkinfo* bInfo{};

	bool    isFile = true;

	CFile	fileH(filePath, CFile::modeRead);
	CFile	fileB;

	if( !fileB.Open(fileBook, CFile::modeRead) )
	{
		isFile = false;		//���� �������
	}

	for (int ii = 0; ii < maxJONGMOK; ii++)
	{
		pinter = (_inters *) new char[sz_inters];
		ZeroMemory(pinter, sz_inters);


		readL = fileH.Read(pinter, sz_inters);
		if (readL < sz_inters)
		{
			delete pinter;
			break;
		}
		
		if(isFile == true)
		{
			
			bInfo = (_bookmarkinfo *) new char[sizeof(_bookmarkinfo)];
			ZeroMemory(bInfo, sizeof(_bookmarkinfo));

			readL = fileB.Read(bInfo, sizeof(_bookmarkinfo));

		

			if(readL < sizeof(_bookmarkinfo))
			{
				delete bInfo;
			}
			else
			{
				CString temp = CString((pinter->code));
				temp = temp.Left(12);
				temp.TrimRight();
				CString temp2 = CString(bInfo->code);
				temp2.TrimRight();
				
				if(strcmp(temp, temp2) == 0)
				{
					if (pinter->code[0] == 'm')
					{
						CopyMemory(pinter->name, bInfo->name, sizeof(bInfo->name));
					}
					
					pinter->bookmark[0] = bInfo->bookmark[0] == '1' ? '1':'0';//2015.04.03 KSJ 1�̾ƴϸ� 0���� ���ش�.
				}
			}
		}

		pinter->name[31] = 0;	//2016.01.25 KSJ 225050 �������� 33byte�̹Ƿ� �ΰ��� ���� �ʾ� �����߻��Ѵ�.

		m_inters.Add(pinter);
	}
	
	if(isFile == true)
	{
		fileB.Close();
	}

	

	fileH.Close();
	return m_inters.GetSize();
}
#endif

void CPage1::savingInterest(int gno)
{
#ifdef DF_SEARCH
	CString	filePath, fileBook;
	fileBook.Format("%s/%s/%s/bookmark.i%02d", m_root, USRDIR, m_name, gno);

	struct	_inters* pinter{};
	struct  _bookmarkinfo* bInfo{};
	bool	isfile = false;
	::DeleteFile(fileBook);
	CFile	fileB(fileBook, CFile::modeWrite | CFile::modeCreate);

	if (fileB.m_hFile == CFile::hFileNull)
	{
		isfile = true;
	}

	for (int ii = 0; ii < m_inters.GetSize(); ii++)// 
	{
		pinter = m_inters.GetAt(ii);

		bInfo = (_bookmarkinfo*) new char[sz_bookmark];
		ZeroMemory(bInfo, sz_bookmark);

		CString temp = CString((pinter->code), sizeof(pinter->code));
		temp = temp.Left(12);
		temp.TrimRight();

		//SPACE ����
		char* nullcode = "            ";

		if (strlen(pinter->code) == 0)
		{
			CopyMemory(bInfo->code, nullcode, min(strlen(nullcode), sizeof(bInfo->code)));
			bInfo->bookmark[0] = '0';
		}
		else
		{
			CopyMemory(bInfo->code, temp, min(sizeof(pinter->code), sizeof(bInfo->code)));
			if (pinter->code[0] == 'm')
				CopyMemory(bInfo->name, pinter->name, min(sizeof(pinter->name), sizeof(bInfo->name)));
		
			bInfo->bookmark[0] = pinter->bookmark[0] == '1' ? '1' : '0';//2015.04.03 KSJ 1�̾ƴϸ� 0���� ���ش�.
		}
		fileB.Write(bInfo, sz_bookmark);
	}
#else
	CString	filePath, fileBook;
	filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);
	fileBook.Format("%s/%s/%s/bookmark.i%02d", m_root, USRDIR, m_name, gno);

	struct	_inters* pinter{};
	struct  _bookmarkinfo* bInfo{};

	bool	isfile = false;
	
	::DeleteFile(filePath);
	CFile	fileH(filePath, CFile::modeWrite|CFile::modeCreate);
	::DeleteFile(fileBook);
	CFile	fileB(fileBook, CFile::modeWrite|CFile::modeCreate);

	if (fileH.m_hFile == CFile::hFileNull) return;

	if (fileB.m_hFile == CFile::hFileNull) 
	{
		isfile = true;
	}


	for (int ii = 0; ii < m_inters.GetSize(); ii++)// 
	{
		pinter = m_inters.GetAt(ii);

		bInfo = (_bookmarkinfo *) new char[sz_bookmark];
		ZeroMemory(bInfo, sz_bookmark);

		int len = 0;
		CString temp = CString((pinter->code), sizeof(pinter->code));
		temp = temp.Left(12);
		len = temp.GetLength();
		temp.TrimRight();

		//SPACE ����
		char *nullcode = "            ";

		if(temp.IsEmpty())
		{
			CopyMemory(pinter->code, nullcode, min(strlen(nullcode), sizeof(pinter->code)));
			CopyMemory(pinter->name, nullcode, min(strlen(nullcode), sizeof(pinter->code)));
		}
		else
		{
			CopyMemory(pinter->code, temp, min(strlen(temp),sizeof(pinter->code)));
		}
		

		fileH.Write(pinter, sizeof(_inters));
		
		//�ϸ�ũ ��� �߰�
		if(strlen(pinter->code) == 0)
		{
			CopyMemory(bInfo->code, nullcode, min(strlen(nullcode), sizeof(bInfo->code)));
			bInfo->bookmark[0] = '0';
		}
		else
		{
			
			CopyMemory(bInfo->code, temp, min(sizeof(pinter->code), sizeof(bInfo->code)));
			

			if (pinter->code[0] == 'm')
			{
				CopyMemory(bInfo->name, pinter->name, min(sizeof(pinter->name), sizeof(bInfo->name)));
			}

			bInfo->bookmark[0] = pinter->bookmark[0] == '1' ? '1':'0';//2015.04.03 KSJ 1�̾ƴϸ� 0���� ���ش�.
		}
		

		if(isfile == false)
			fileB.Write(bInfo, sz_bookmark);

		delete bInfo;
	}

	fileH.Close();

 	if(isfile == false)
 		fileB.Close();
#endif
}

void CPage1::savingGroupFile(int gno, CString gname)
{
#ifdef DF_SEARCH
	savingInterest(gno);
#else
	CString tempN; 
	tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);

	CString	section; 
	section.Format("%02d", gno);
	WritePrivateProfileString("GROUPNAME", section, gname, tempN);
	WritePrivateProfileString(nullptr, nullptr, nullptr, tempN);
	
	savingInterest(gno);
#endif
}

bool CPage1::savingGroupOrder(CString gname)
{
#ifdef DF_SEARCH
	int iGrpNum = 0;
	CWnd* wnd = GetParent()->GetParent();
	iGrpNum = wnd->SendMessage(WM_MSG, MAKEWPARAM(MSG_GET_GROUPCOUNT, 0), (LPARAM)m_pdata);

	if (iGrpNum >= maxGROUP)
		return false;

	if (!gname.IsEmpty())
	{
		CString string;
		memset(m_pdata, 0x00, 1024 * 4);
		wnd->SendMessage(WM_MSG, MAKEWPARAM(MSG_SET_NEWGROUP, 0), (LPARAM)(LPSTR)(LPCTSTR)gname);

		const int idx = m_gname.AddString(gname);
		iGrpNum++;
		m_gname.SetItemData(idx, iGrpNum);

	
		m_gname.SetCurSel(idx);
		m_activegno = iGrpNum;

		return true;
	}
	return false;
#else
	int	ttL = 0;
	char	ttB[1024]{};
	CString	strOrder = _T(""), string;
	
	CString filePath; 
	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);
	ttL = GetPrivateProfileString(_T("GROUPORDER"), "00", "", ttB, sizeof(ttB), filePath);
	if (ttL > 0) strOrder = CString(ttB, ttL);
	
	CString temp, tempIndex[100], grpNum;
	int iGrpNum = 0;

	temp = strOrder;
	int ii = 0;
	for(ii = 0 ; ii<100 ; ii++)
	{
		tempIndex[ii] = "";
	}
	
	ii = 0;

	while (!temp.IsEmpty())
	{
		tempIndex[ii] = parseX(temp, ";");
		ii++;
	}
	
	temp = "";

	for(int ii=0; ii<100 ; ii++)
	{
		temp = tempIndex[ii];
		
		if(!temp.IsEmpty())
			continue;

		if (ii > 1)
			grpNum = tempIndex[ii-1];
		
		iGrpNum = atoi(grpNum) +1;
		string.Format("%02d", iGrpNum);
		break;
	}

	if (iGrpNum >= maxGROUP)
		return false;

	if (!gname.IsEmpty())
	{
		const int idx = m_gname.AddString(gname);
		m_gname.SetItemData(idx, iGrpNum);
		
		string.Format("%02d;", iGrpNum); 
		strOrder += string;
		WritePrivateProfileString(_T("GROUPORDER"), "00", strOrder, filePath);
		WritePrivateProfileString(nullptr, nullptr, nullptr, filePath);

		m_gname.SetCurSel(idx); 
		m_activegno = iGrpNum;

		return true;
	}

	return false;
#endif
}

int CPage1::xAllCodeToList()
{
	ClearListitem();
	m_list1.DeleteAllItems();

	_shjcode   hjcode; 
	 CString code, name;
	 int position = 0;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			switch (hjcode.ssgb)
			{
			case jmSINJU:
			case jmSINJS:
			case jmHYFND:
			case jmELW:
				continue;
			default:
				break;
			}
			break;
		default:
			continue;
		}

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(position, code, name.Mid(1));
		position += 1;
	}

	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

int CPage1::xKospiToList()
{
	ClearListitem(); 
	m_list1.DeleteAllItems();

	_shjcode   hjcode; 
	 CString code, name;
	 int position = 0;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
			switch (hjcode.ssgb)
			{
			case jmSINJU:
			case jmSINJS:
			case jmHYFND:
			case jmELW:
				continue;
			default:
				break;
			}
			break;
		default:
			continue;
		}

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(position, code, name.Mid(1));
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

//2013.06.25 �ڳؽ� �߰�
int CPage1::xKonexToList()
{
	ClearListitem(); 
	m_list1.DeleteAllItems();
	
	_shjcode   hjcode; 
	CString code, name;
	int position = 0;
	
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);
		
		switch (hjcode.ssgb)
		{
		case jmKONEX:
			break;
			
		default:
			continue;
		}
		
		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);
		
		AppendItemToList1(position, code, name.Mid(1));
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}
//2013.06.25 �ڳؽ� �߰� end

//2014.11.06 ETN �߰�
int CPage1::xETNCodeToList()
{
	ClearListitem(); 
	m_list1.DeleteAllItems();
	
	_shjcode   hjcode; 
	CString code, name;
	int position = 0;
	
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);
		
		switch (hjcode.ssgb)
		{
		case jmETN:
			break;
			
		default:
			continue;
		}
		
		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);
		
		AppendItemToList1(position, code, name.Mid(1));
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}
//2014.11.06 ETN �߰� end

//2015.11.03 spac �߰�
int CPage1::xSPACCodeToList()
{
	ClearListitem(); 
	m_list1.DeleteAllItems();
	
	_shjcode   hjcode; 
	CString code, name;
	int position = 0;
	
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);
		
		switch (hjcode.ssgb)
		{
		case jmSPAC:
			break;
			
		default:
			continue;
		}
		
		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);
		
		AppendItemToList1(position, code, name.Mid(1));
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}
//2015.11.03 spac �߰� end

int CPage1::xKosdaqToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	_shjcode   hjcode; 
	 CString code, name;
	 int position = 0;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSDAQ:
			switch (hjcode.ssgb)
			{
			case jmSINJU:
			case jmSINJS:
			case jmHYFND:
			case jmELW:
				continue;
			default:
				break;
			}
			break;
		default:
			continue;
		}

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(position, code, name.Mid(1));
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

int CPage1::xKospi200ToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	_shjcode   hjcode; 
	 CString code, name;
	 int position = 0;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
			if (hjcode.kpgb >= 1)
				break;
		default:
			continue;
		}

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(position, code, name.Mid(1));
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

int CPage1::xKosdaq50ToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	_shjcode   hjcode; 
	 CString code, name;
	 int position = 0;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSDAQ:
			if (hjcode.kpgb >= 1)
				break;
		default:
			continue;
		}

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(position, code, name.Mid(1));
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

int CPage1::xKRX100ToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	_shjcode   hjcode; 
	 CString code, name;
	 int position = 0;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.unio != 1)
			continue;

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(position, code, name.Mid(1));
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

int CPage1::xFreeCodeToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	_shjcode   hjcode; 
	 CString code, name;
	 int position = 0;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.kosd != jm3RD)
			continue;

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(position, code, name.Mid(1));
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

int CPage1::xELWCodeToList()
{
	ClearListitem(); 
	m_list1.DeleteAllItems();

	_shjcode   hjcode; 
	 CString code, name;
	 int position = 0;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			if (hjcode.ssgb != jmELW)
				continue;
			break;
		default:
			continue;
		}

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(position, code, name);
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

int CPage1::xELWBaseToList()
{
	loadingELWcode();

	ClearListitem(); 
	m_list1.DeleteAllItems();

	_sjcode   sjcode; 
	 CString code, name;
	 int position = 0;

	for (int ii = 0; ii < m_elwbase.GetSize(); ii++)
	{
		sjcode = m_elwbase.GetAt(ii);

		AppendItemToList1(position, sjcode.code, sjcode.name);
		position += 1;
	}

	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	m_listsort = false;
	ListView_SortItems(m_list1, ListViewCompareFunc, 1);

	return position;
}

int CPage1::xETFCodeToList()
{
	ClearListitem(); 
	m_list1.DeleteAllItems();

	_shjcode   hjcode; 
	 CString code, name;
	 int position = 0;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			if (hjcode.ssgb != jmETF)
				continue;
			break;
		default:
			continue;
		}

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(position, code, name.Mid(1));
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

int CPage1::xSinjuCodeToList()
{
	ClearListitem();
	m_list1.DeleteAllItems();

	_shjcode   hjcode; 
	 CString code, name;
	 int position = 0;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			switch (hjcode.ssgb)
			{
			case jmSINJU:
			case jmSINJS:
			//case jmHYFND:	//2015.03.30 KSJ �峻������������ �����μ��� ����Ʈ���� ����
				break;
			default:
				continue;
			}
			break;
		default:
			continue;
		}

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(position, code, name.Mid(1));
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

void CPage1::parsingRecommandDN(char* datB, int datL)
{
	m_list1.DeleteAllItems();

	struct  mod
	{
		char    nrec[4];						/*	����			*/
		char    acod[1024][12];					/*  �����ڵ� ����Ʈ */
	}* mod;

	mod = (struct mod*)datB;
	

	CString strCnt = CString(mod->nrec, 4);
	const int nCnt = atoi(strCnt);
	
	CString code, name;
	int position = 0;

	for(int i=0 ; i<nCnt ; i++)
	{
		code = CString(mod->acod[i], 12);
		code.TrimLeft();
		code.TrimRight();


		if (!code.IsEmpty())
		{
			if (code.GetAt(0) == 'J')
				code = code.Mid(1);
			
			name = GetCodeName(code);
			name.TrimLeft();
			name.TrimRight();

			AppendItemToList1(position, code, name);
			position += 1;
		}
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
}

void CPage1::parsingELWIssueDN(char* datB, int datL)
{
	m_list1.DeleteAllItems();

	struct  grid 
	{
		char    seqn[4];
		char    cod2[7];            /* RTS COD      */
		char    code[6];            /* �����ڵ�     */
		char    hnam[20];           /* �����       */
		char    curr[8];            /* ���簡       */
		char    diff[7];            /* ���         */
		char    rate[6];            /* �����       */
		char    mdga[8];            /* �ŵ�ȣ��     */
		char    msga[8];            /* �ż�ȣ��     */
		char    cvol[9];            /* ü�ᷮ       */
		char    gvol[9];            /* �ŷ���       */
		char    gamt[12];           /* �ŷ����     */
		char    dvol[8];            /* ���ŵ��ܷ�   */
		char    svol[8];            /* ���ż��ܷ�   */
		char    irga[7];            /* �̷а�       */
		char    grat[7];            /* ������       */
		char    prty[7];            /* �и�Ƽ       */
		char    gerr[7];            /* ������   */
		char    brer[7];            /* ���ͺб���   */
		char    cfpv[7];            /* �ں�������   */
		char    gcjs[18];           /* �����ڻ�     */
		char    bhsa[20];           /* �����       */
		char    jjis[6];            /* �����ϼ�     */
		char    hsgg[9];            /* ȯ�갡       */
		char    hsga[9];            /* ��簡��     */
		char    jhre[9];            /* ��ȯ����     */
		char    delt[9];            /* ��Ÿ         */
		char    eger[9];            /* e*��     */
	};
	
	typedef struct  {		/* GRID form output format  */
		char    aflg[1];		/* add action to top or bottom  */
		/* '0':replace '1':top, '2':bottom */
		char    sdir[1];		/* sort direction       */
		/* '1':asc, '2':desc    */
		char    scol[16];		/* sorting column symbol    */
		char    xpos[1];		/* continuous status        */
		/* 0x40: default(normal)    */
		/* 0x01: PgUp, ScrUp enable */
		/* 0x02: PgDn, ScrDn enable */
		/* 0x04: no local sorting   */
		char    page[4];		/* # of page            */
		char    save[80];		/* grid inout save field    */
	} GRID_O;			/* grid header */
	
	struct mod
	{
		char flag[1];	
		GRID_O  grid_o;
		char nrec[4];				//gr7id row ����
		struct grid grid[1];
	}* mod;
	
	mod = (struct mod*)datB;
	
	CString oflag, nrec, code, name;
	GRID_O m_grido{};
	int ncnt = 0;
//	constexpr int nsize = sizeof(struct grid);
	int position = 0;

	oflag.Format("%.*s", sizeof(mod->flag), mod->flag);
	CopyMemory(&m_grido, &mod->grid_o, sizeof(GRID_O));
	nrec.Format("%.*s", sizeof(mod->nrec), mod->nrec);
	ncnt = atoi(nrec);
	
	for(int ii=0 ; ii<ncnt ; ii++)
	{
		code.Format("%.*s", sizeof(mod->grid[ii].cod2), mod->grid[ii].cod2);
		code.TrimRight();
		name.Format("%.*s", sizeof(mod->grid[ii].hnam), mod->grid[ii].hnam);
		name.TrimRight();
		
		if (!code.IsEmpty())
		{
			if (code.GetAt(0) == 'J')
				code = code.Mid(1);

			AppendItemToList1(position, code, " " + name);
			position += 1;
		}
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
}

void CPage1::parsingELWAssetDN(char* datB, int datL)
{
	m_list1.DeleteAllItems();

	struct  grid 
	{
		char    seqn[4];
		char    cod2[7];            /* RTS COD      */
		char    code[6];            /* �����ڵ�     */
		char    hnam[20];           /* �����       */
		char    curr[8];            /* ���簡       */
		char    diff[7];            /* ���         */
		char    rate[6];            /* �����       */
		char    mdga[8];            /* �ŵ�ȣ��     */
		char    msga[8];            /* �ż�ȣ��     */
		char    cvol[9];            /* ü�ᷮ       */
		char    gvol[9];            /* �ŷ���       */
		char    gamt[12];           /* �ŷ����     */
		char    dvol[8];            /* ���ŵ��ܷ�   */
		char    svol[8];            /* ���ż��ܷ�   */
		char    irga[7];            /* �̷а�       */
		char    grat[7];            /* ������       */
		char    prty[7];            /* �и�Ƽ       */
		char    gerr[7];            /* ������   */
		char    brer[7];            /* ���ͺб���   */
		char    cfpv[7];            /* �ں�������   */
		char    gcjs[18];           /* �����ڻ�     */
		char    bhsa[20];           /* �����       */
		char    jjis[6];            /* �����ϼ�     */
		char    hsgg[9];            /* ȯ�갡       */
		char    hsga[9];            /* ��簡��     */
		char    jhre[9];            /* ��ȯ����     */
		char    delt[9];            /* ��Ÿ         */
		char    eger[9];            /* e*��     */
	};
	
	typedef struct  {		/* GRID form output format  */
		char    aflg[1];		/* add action to top or bottom  */
		/* '0':replace '1':top, '2':bottom */
		char    sdir[1];		/* sort direction       */
		/* '1':asc, '2':desc    */
		char    scol[16];		/* sorting column symbol    */
		char    xpos[1];		/* continuous status        */
		/* 0x40: default(normal)    */
		/* 0x01: PgUp, ScrUp enable */
		/* 0x02: PgDn, ScrDn enable */
		/* 0x04: no local sorting   */
		char    page[4];		/* # of page            */
		char    save[80];		/* grid inout save field    */
	} GRID_O;			/* grid header */
	
	struct mod
	{
		char flag[1];	
		GRID_O  grid_o;
		char nrec[4];				//gr7id row ����
		struct grid grid[1];
	}* mod;
	
	mod = (struct mod*)datB;
	
	CString oflag, nrec, code, name;
	GRID_O m_grido{};
	int ncnt = 0;
	int position = 0;
	
	oflag.Format("%.*s", sizeof(mod->flag), mod->flag);
	CopyMemory(&m_grido, &mod->grid_o, sizeof(GRID_O));
	nrec.Format("%.*s", sizeof(mod->nrec), mod->nrec);
	ncnt = atoi(nrec);
	
	for(int ii=0 ; ii<ncnt ; ii++)
	{
		code.Format("%.*s", sizeof(mod->grid[ii].cod2), mod->grid[ii].cod2);
		code.TrimRight();
		name.Format("%.*s", sizeof(mod->grid[ii].hnam), mod->grid[ii].hnam);
		name.TrimRight();
		
		if (!code.IsEmpty())
		{
			if (code.GetAt(0) == 'J')
				code = code.Mid(1);
			
			AppendItemToList1(position, code, " " + name);
			position += 1;
		}
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
}

int CPage1::xFutureToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	_sjcode   scode;
	CString	code, name;
	int position = 0;

	for (int ii = 0; ii < m_fjcode.GetSize(); ii++)
	{
		scode = m_fjcode.GetAt(ii);

		code = CString(scode.code, FCodeLen);
		name = CString(scode.name, FNameLen);

		code.TrimLeft(); code.TrimRight();
		if (code.IsEmpty()) continue;

		AppendItemToList1(position, code, name);
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

int CPage1::xCallCodeToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	struct ojcode	ojcode;
	int position = 0;
	CString	code, name;

	for (int ii = 0; ii < m_ojcode.GetSize(); ii++)
	{
		ojcode = m_ojcode.GetAt(ii);

		for (int jj = 0; jj < STANDARDNUM; jj++)
		{
			if (ojcode.call[jj].yorn != '1')
				continue;

			code = CString(ojcode.call[jj].cod2, OCodeLen);
			name = CString(ojcode.call[jj].hnam, ONameLen);

			AppendItemToList1(position, code, name);
			position += 1;
		}
	}

	m_listsort = false;

	m_list1.SetItemCountEx(m_arList.GetSize());
	if(m_arList.GetSize() > 0)
		qsort(static_cast<void*>(&m_arList[0]), m_arList.GetSize(), sizeof(CITEMS), compareSortCode2);
	m_list1.Invalidate();
	//ListView_SortItems(m_list1, ListViewCompareFunc, 1);

	return position;
}

int CPage1::xPutCodeToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	struct ojcode	ojcode;
	CString	code, name;
	int position = 0;

	for (int ii = 0; ii < m_ojcode.GetSize(); ii++)
	{
		ojcode = m_ojcode.GetAt(ii);

		for (int jj = 0; jj < STANDARDNUM; jj++)
		{
			if (ojcode.put[jj].yorn != '1')
				continue;

			code = CString(ojcode.put[jj].cod2, OCodeLen);
			name = CString(ojcode.put[jj].hnam, ONameLen);

			AppendItemToList1(position, code, name);
			position += 1;
		}
	}

	m_listsort = false;
	m_list1.SetItemCountEx(m_arList.GetSize());
	if (m_arList.GetSize() > 0)
		qsort(static_cast<void*>(&m_arList[0]), m_arList.GetSize(), sizeof(CITEMS), compareSortCode2);
	m_list1.Invalidate();
	//ListView_SortItems(m_list1, ListViewCompareFunc, 1);
	return position;
}

int CPage1::xKospiJisuToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	struct upcode	upcode;
	CString	code, name;
	int position = 0;

	for (int ii = 0; ii < m_upcode.GetSize(); ii++)
	{
		upcode = m_upcode.GetAt(ii);

		if (upcode.jgub != upKOSPI)
			continue;

		code.Format("%01d%02d", (int)upcode.jgub, (int)upcode.ucod);
		name = CString(upcode.hnam, UNameLen);

		AppendItemToList1(position, code, name);
		position += 1;
	}

	for (int ii = 0; ii < cnt_exKospi; ii++)
	{
		code = exKospi[ii].code;
		name = exKospi[ii].name;

		AppendItemToList1(position, code, name);
		position += 1;
	}

	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	ListView_SortItems(m_list1, ListViewCompareFunc, 0);

	return position;
}

int CPage1::xKosdaqJisuToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	struct upcode	upcode;
	CString	code, name;
	int position = 0;

	for (int ii = 0; ii < m_upcode.GetSize(); ii++)
	{
		upcode = m_upcode.GetAt(ii);

		if (upcode.jgub != upKOSDAQ)
			continue;

		code.Format("%01d%02d", (int)upcode.jgub, (int)upcode.ucod);
		name = CString(upcode.hnam, UNameLen);

		AppendItemToList1(position, code, name);
		position += 1;
	}

/*
	for (ii = 0; ii < cnt_exKosdaq; ii++)
	{
		code = exKosdaq[ii].code;
		name = exKosdaq[ii].name;

		AppendItemToList1(position, code, name);
		position += 1;
	}
*/
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	ListView_SortItems(m_list1, ListViewCompareFunc, 0);
	return position;
}

int CPage1::xPjcodeToList(CString code, char kind)
{
	ClearListitem(); m_list1.DeleteAllItems();

	int	position = 0;
	struct	pjcode pjcode;
	CString	codx, name, tjgb;
	for (int ii = 0; ii < m_pjcode.GetSize(); ii++)
	{
		pjcode = m_pjcode.GetAt(ii);

		codx = CString(pjcode.codx, PCodeLen);
		tjgb = CString(pjcode.tjgb, 2);

		if (codx.GetAt(0) == kind && tjgb == code)
		{
			name = CString(pjcode.hnam, sizeof(pjcode.hnam));

			AppendItemToList1(position, codx, name);
			position += 1;
		}
	}

	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	m_bAddAll = true;
	return position;
}

int CPage1::xKospiUpToList(int selItem)
{
	ClearListitem(); 
	m_list1.DeleteAllItems();

	const int kind = selItem % 100;

	CString slog;
	slog.Format("\r\n------------------------kind = [%d] ------------------\r\n", kind);
	OutputDebugString(slog);


	int position = 0;
	CString	code, name, stmp, stemp;

	bool append = false;
	struct _shjcode hjcode; 
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.kosd != jmKOSPI)
			continue;

		switch (hjcode.ssgb)
		{
		case jmSINJU: case jmSINJS: case jmHYFND: case jmELW:
			continue;
		}

		append = false;
		if (kind == 1)
		{
			append = true;
		}
		else if (kind < 5)
		{
			if ((int)hjcode.size == kind)
				append = true;
		}
		else if (kind < 27)
		{
			const CString xxx = xxKOSPI[kind];
			stmp.Format("%s", hjcode.ucds); stmp.TrimRight();
			stemp.Format("%s", hjcode.ucdm); stemp.TrimRight();
			if (stmp.Find(xxx) >= 0 || stemp.Find(xxx) >= 0)
				append = true;
		}
		else if (kind == 27)
		{
			if ((int)hjcode.jjug == kind)
				append = true;
		}
		else if (kind < 41)
		{
			if ((int)hjcode.jsiz == kind)
				append = true;
		}
		else if (kind == 41)
		{
			if ((int)hjcode.jsiz >= kind + 38 && (int)hjcode.jsiz <= kind + 41)
				append = true;
		}
		else if (kind == 42)
		{
			if ((int)hjcode.wsgb == 1)
				append = true;
		}

		if (!append) continue;

		code = hjcode.code;
		name = hjcode.name;

		AppendItemToList1(position, code, name.Mid(1));
		position += 1;
	}

	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

int CPage1::xKosdaqUpToList(int selItem, CString sData)
{
	/*ClearListitem(); 
	m_list1.DeleteAllItems();

	const int kind = selItem % 100;

	int position = 0;
	CString	code, name;
	CString stemp;
	
	bool append = false;
	struct _shjcode hjcode; 
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.kosd != jmKOSDAQ)
			continue;
		
		switch (hjcode.ssgb)
		{
		case jmSINJU: case jmSINJS: case jmHYFND: case jmELW:
			continue;
		}
		append = false;

		if (kind == 1)
		{
			append = true;
		}
		else if (kind == 2)
		{
			if ((int)hjcode.jjug == kind)
				append = true;
		}
		else if (kind < 5)
		{
			if ((int)hjcode.size == kind)
				append = true;
		}
		else if (kind == 5)
		{
			if ((int)hjcode.size == 4 && (int)hjcode.jjug == 2)
				append = true;
		}
		else if (kind < 15 || kind == 41)
		{
			if ((int)hjcode.ucdm == kind)
				append = true;
		}
		else if (kind < 38)
		{
			if ((int)hjcode.ucds == kind)
				append = true;
		}
		else if (kind < 41)
		{
			if ((int)hjcode.jsiz == kind - 37)
				append = true;
		}
		else if (kind == 42)
		{
			if ((int)hjcode.prmr == kind - 41)
				append = true;
		}

		if (!append) continue;

		code = hjcode.code;
		name = hjcode.name;

		AppendItemToList1(position, code, name.Mid(1));
		position += 1;
	}*/
	ClearListitem();
	m_list1.DeleteAllItems();

	int position = 0;
	const int kind = selItem % 100;

	CString	code, name, stmp, stemp;

	bool append = false;
	struct _shjcode hjcode;
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.kosd != jmKOSDAQ)
			continue;

		switch (hjcode.ssgb)
		{
		case jmSINJU: case jmSINJS: case jmHYFND: case jmELW:
			continue;
		}

		append = false;

		if (kind == 1)
		{
			append = true;
		}
		else if (kind == 2)
		{
			if ((int)hjcode.jjug == kind)
				append = true;
		}
		else if (kind < 5)
		{
			if ((int)hjcode.size == kind)
				append = true;
		}
		else if (kind == 5)
		{
			if ((int)hjcode.size == 4 && (int)hjcode.jjug == 2)
				append = true;
		}
		else if (kind < 38 || kind == 41)
		{
			const CString xxx = xxKOSDAQ[kind];
			stmp.Format("%s", hjcode.ucds); stmp.TrimRight();
			stemp.Format("%s", hjcode.ucdm); stemp.TrimRight();
			if (stmp.Find(xxx) >= 0 || stemp.Find(xxx) >= 0 )
				append = true;
		}
		else if (kind < 41)
		{
			if ((int)hjcode.jsiz == kind - 37)
				append = true;
		}
		else if (kind == 42)
		{
			if ((int)hjcode.prmr == kind - 41)
				append = true;
		}
		else if (IS_WITHINx(42, 46, kind))
		{
			const CString xxx = xxKOSDAQ[kind];
			stmp.Format("%s", hjcode.acdl); stmp.TrimRight();
			if (stmp.Find(xxx) >= 0)
				append = true;
		}

		if (!append) continue;

		code = hjcode.code;
		name = hjcode.name;

		AppendItemToList1(position, code, name.Mid(1));
		position += 1;
	}

	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

int CPage1::xKospiExToList(int selItem)
{
	ClearListitem(); 
	m_list1.DeleteAllItems();

	const int kind = selItem % 1000;

	int position = 0;
	CString	code, name, tmp;

	bool append = false;
	struct _shjcode hjcode; 
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.kosd != jmKOSPI)
			continue;

		append = false;
		tmp.Format("%s", hjcode.ucdm);

		switch (kind)
		{
		case 101: // KOSPI200
			if ((int)hjcode.kpgb == 3 || (int)hjcode.kpgb == 2 || (int)hjcode.kpgb == 1)
				append = true;
			break;
		case 401: // KOSPI100
			if ((int)hjcode.kpgb == 3 || (int)hjcode.kpgb == 2)
				append = true;
			break;
		case 402: // KOSPI50
			if ((int)hjcode.kpgb == 3)
				append = true;
			break;
		case 403: // KOSPI IT
			if ((int)hjcode.itgb == 1)
				append = true;
			break;
		}

		if (!append) continue;

		code = hjcode.code;
		name = hjcode.name;

		AppendItemToList1(position, code, name.Mid(1));
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

int CPage1::xKosdaqExToList(int selItem)
{
	ClearListitem(); 
	m_list1.DeleteAllItems();

	const int kind = selItem % 1000;

	int position = 0;
	CString	code, name;

	bool append = false;
	struct _shjcode hjcode; 
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.kosd != jmKOSDAQ)
			continue;

		append = false;

		switch (kind)
		{
		case 301: // KOSDAQ50
			if ((int)hjcode.kpgb == 1)
				append = true;
			break;
		case 302: // KOSDAQ IT
			if ((int)hjcode.itgb == 1)
				append = true;
			break;
		case 303: // KOSTAR
			if ((int)hjcode.star == 1)
				append = true;
			break;				
		}

		if (!append) continue;

		code = hjcode.code;
		name = hjcode.name;

		AppendItemToList1(position, code, name.Mid(1));
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

int CPage1::xThemaCodeToList(int selItem)
{
	ClearListitem(); 
	m_list1.DeleteAllItems();

	CString	skey; skey.Format("%03d", selItem % 1000);

	char	readB[512]{};
	UINT	readL{}, position = 0;
	CString	filePath, code, name, string = _T("");

	filePath.Format("%s/%s/%s", m_root, TABDIR, "themcd.ini");

	readL = GetPrivateProfileString(_T("TMCODE"), skey, "", readB, sizeof(readB), filePath);
	if (readL > 0) string = CString(readB, readL);

	while (!string.IsEmpty())
	{
		code = parseX(string, ";");
		name = GetCodeName(code);

		AppendItemToList1(position, code, name);
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

int CPage1::xGroupCodeToList(int selItem)
{
	ClearListitem(); 
	m_list1.DeleteAllItems();

	CString	skey; skey.Format("%03d", selItem % 1000);

	char	readB[512]{};
	UINT	readL{}, position = 0;
	CString	filePath, code, name, string = _T("");

	filePath.Format("%s/%s/%s", m_root, TABDIR, "gropcd.ini");

	readL = GetPrivateProfileString(_T("GRCODE"), skey, "", readB, sizeof(readB), filePath);
	if (readL > 0) string = CString(readB, readL);

	while (!string.IsEmpty())
	{
		code = parseX(string, ";");
		name = GetCodeName(code);

		AppendItemToList1(position, code, name);
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

int CPage1::xInterestToList(int selItem)
{
	return 0;
	//ClearListitem(); 
	//m_list1.DeleteAllItems();

	//const int gno = selItem % 100;

	//CString	filePath;
	//if (ExistFile(gno, true))
	//	filePath.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, gno);
	//else if (ExistFile(gno))
	//	filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);
	//else
	//	return 0;

	//UINT	readL{}, position = 0;
	//CString	code, name;
	//struct	_inters interest;

	//CFile	fileH(filePath, CFile::modeRead);
	//if (fileH.m_hFile == CFile::hFileNull)
	//	return 0;

	//for (int ii = 0; ii < maxJONGMOK; ii++)
	//{
	//	readL = fileH.Read(&interest, sz_inters);
	//	if (readL < sz_inters)
	//		break;

	//	code = CString(interest.code, sizeof(interest.code));
	//	name = GetCodeName(code);//interest.name;

	//	AppendItemToList1(position, code, name);
	//	position += 1;
	//}
	//fileH.Close();

	//return position;
}

int CPage1::xCFutureToList(bool bCurrency)	//2012.10.04 KSJ �ݸ�/��ȭ ��ȭ�϶��� true
{
	ClearListitem(); m_list1.DeleteAllItems();
	
	ccode   ccode;
	CString	code, name;
	int position = 0;
	
	if(bCurrency)
	{
		for (int ii = 0; ii < m_cccode.GetSize(); ii++)
		{
			ccode = m_cccode.GetAt(ii);
			
			code = CString(ccode.codx, FCodeLen);
			name = CString(ccode.hnam, FNameLen*2 + 10);
			
			code.TrimLeft(); code.TrimRight();
			if (code.IsEmpty()) continue;
			
			AppendItemToList1(position, code, name);
			position += 1;
		}
	}
	else
	{
		for (int ii = 0; ii < m_cicode.GetSize(); ii++)
		{
			ccode = m_cicode.GetAt(ii);
			
			code = CString(ccode.codx, FCodeLen);
			name = CString(ccode.hnam, FNameLen*2 + 10);
			
			code.TrimLeft(); code.TrimRight();
			if (code.IsEmpty()) continue;
			
			AppendItemToList1(position, code, name);
			position += 1;
		}
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

int CPage1::xSFutureToList()	//2012.10.04 KSJ �ֽļ���
{
	ClearListitem(); m_list1.DeleteAllItems();
	
	sfcode   sfcode;
	CString	code, name;
	int position = 0;
	
	for (int ii = 0; ii < m_sfcode.GetSize(); ii++)
	{
		sfcode = m_sfcode.GetAt(ii);
		
		code = CString(sfcode.codx, FCodeLen);
		name = CString(sfcode.hnam, FNameLen*2 + 10);
		
		code.TrimLeft(); code.TrimRight();
		if (code.IsEmpty()) continue;
		
		AppendItemToList1(position, code, name);
		position += 1;
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return position;
}

void CPage1::OKaction()
{
	//CFileFind finder;
	//CString	filePath, fileTemp;

	//filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);
	//fileTemp.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);

	//if (finder.FindFile(fileTemp))
	//{
	//	DeleteFile(filePath); 
	//	CopyFile(fileTemp, filePath, FALSE);
	//}

	//for (int ii = 0; ii < maxGROUP; ii++)
	//{
	//	filePath.Format("%s/%s/%s/portfolio.i%02d",     m_root, USRDIR, m_name, ii);
	//	fileTemp.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, ii);

	//	if (!finder.FindFile(fileTemp))
	//		continue;

	//	CopyFile(fileTemp, filePath, FALSE);		
	//}

	//CCaction();
}

void CPage1::OKactionOne(int gnum)
{
	//CFileFind finder;
	//CString	filePath, fileTemp;

	//filePath.Format("%s/%s/%s/portfolio.i%02d",     m_root, USRDIR, m_name, gnum);
	//fileTemp.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, gnum);
	//
	//if(finder.FindFile(fileTemp))
	//{
	//	CopyFile(fileTemp, filePath, FALSE);
	//	DeleteFile(fileTemp); 	
	//}
}

void CPage1::CCaction()
{
	//CString tempN; tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);
	//DeleteFile(tempN);

	//CFileFind finder;
	//CString	fileTemp;
	//for (int ii = 0; ii < maxGROUP; ii++)
	//{
	//	fileTemp.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, ii);

	//	if (!finder.FindFile(fileTemp))
	//		continue;

	//	DeleteFile(fileTemp);
	//}
}


 CString	CPage1::GetTempData(int gno, CString& name, int& count)
 {
 	CString	filePath;
 	CString result;
 	CString temStr;
	
 XMSG(GetTempData Ȯ��.);
 	name = GetGroupName(gno);  //GetTempData
 	filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);

 	UINT readL{};
 	struct	_inters	inters;
 	struct	_jinfo	jinfo{};
	
 	CFile	fileH(filePath, CFile::modeRead);
 	if (fileH.m_hFile == CFile::hFileNull)
 		return "";

 	result = "";
 	temStr = "";
 	int ii = 0;
 	for (ii = 0; ii < maxJONGMOK; ii++)
 	{
 		readL = fileH.Read(&inters, sz_inters);
		
 		if (readL < sz_inters)
 			break;
		
  		FillMemory(&jinfo, sz_jinfo, ' ');
 		jinfo.gubn[0] = inters.gubn[0];
 		CopyMemory(jinfo.code, inters.code, strlen(inters.code));
 		CopyMemory(jinfo.xprc, inters.xprc, strlen(inters.xprc));
 		CopyMemory(jinfo.xnum, inters.xnum, strlen(inters.xnum));

		
 		//������ ` ���̱�
 		temStr = CString((char*)&jinfo, sizeof(_jinfo));
 		result += temStr + "`";
 	}

 	count = ii;

 	fileH.Close();

 	return result;
 }

void CPage1::CheckPage4()
{
	CString sData;
	sData = m_page->GetGridData();

	if (sData.IsEmpty())
		return;

	CString stmp, stemp, scode, sprc, sval;
	int icnt = 0;
	struct	_inters* pinter{};
	while (!sData.IsEmpty())
	{
		stmp = parseX(sData, "\n");
		scode = parseX(stmp, "\t");		 scode.TrimRight(); scode.TrimLeft();
		sval = parseX(stmp, "\t");			 sprc.TrimRight(); sprc.TrimLeft();
		sprc = parseX(stmp, "\t");		 sval.TrimRight(); sval.TrimLeft();

		pinter = m_inters.GetAt(icnt);
		stemp.Format("%s", pinter->code); stemp.TrimRight();
		if (stemp == scode)
		{
			if(!sprc.IsEmpty())
				CopyMemory(pinter->xprc, sprc, sprc.GetLength());
			if (!sval.IsEmpty())
				CopyMemory(pinter->xnum, sval, sval.GetLength());
		}
	}
}

int CPage1::GetUploadData(int gno, CString& name, char* datB)
{
#ifdef DF_SEARCH
	CWnd* wnd = GetParent()->GetParent();
	int ipage = wnd->SendMessage(WM_MSG, MAKEWPARAM(MSG_GET_ACTIVE_PAGE, 0), 0);
	if (ipage == 4)
		CheckPage4();
	
	name = GetGroupName(gno);  //GetUploadData
	int datL = 0;
	struct	_inters* pinter{};
	struct	_jinfo* jinfo{};
	for (int ii = 0; ii < m_inters.GetSize(); ii++)
	{
		pinter = m_inters.GetAt(ii);
		jinfo = (struct _jinfo*)&datB[datL];
		datL += sz_jinfo;

		memset(jinfo->gubn, ' ', sizeof(jinfo->gubn));
		memset(jinfo->code, ' ', sizeof(jinfo->code));
		memset(jinfo->xprc, ' ', sizeof(jinfo->xprc));
		memset(jinfo->xnum, ' ', sizeof(jinfo->xnum));

		jinfo->gubn[0] = pinter->gubn[0];
		CopyMemory(jinfo->code, pinter->code, strlen(pinter->code));
		CopyMemory(jinfo->xprc, pinter->xprc, strlen(pinter->xprc));
		CopyMemory(jinfo->xnum, pinter->xnum, strlen(pinter->xnum));
	}

	return m_inters.GetSize();
#else
	if (!ExistFile(gno))
	{
		name = _T(""); return 0;
	}

	name = GetGroupName(gno);   //����

	CString	filePath; 
	filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);

	UINT	readL{}, datL = 0;

	struct	_inters	inters;
	struct	_jinfo* jinfo{};

	CFile	fileH(filePath, CFile::modeRead);
	if (fileH.m_hFile == CFile::hFileNull)
		return 0;

	int ii = 0;
	for (ii = 0; ii < maxJONGMOK; ii++)
	{
		readL = fileH.Read(&inters, sz_inters);

		if (readL < sz_inters)
			break;

		jinfo = (struct _jinfo *) &datB[datL];
		datL += sz_jinfo;
		
		memset(jinfo->gubn, ' ', sizeof(jinfo->gubn));
		memset(jinfo->code, ' ', sizeof(jinfo->code));
		memset(jinfo->xprc, ' ', sizeof(jinfo->xprc));
 		memset(jinfo->xnum, ' ', sizeof(jinfo->xnum));

		jinfo->gubn[0] = inters.gubn[0];
		CopyMemory(jinfo->code, inters.code, strlen(inters.code));
		CopyMemory(jinfo->xprc, inters.xprc, strlen(inters.xprc));
		CopyMemory(jinfo->xnum, inters.xnum, strlen(inters.xnum));
	}
	fileH.Close();

	return ii;
#endif
}

int CPage1::SetDnloadData(int gno, CString gname, int count, char* datB)
{
	return 0;
	//int	readL = 0;
	//char	readB[512];
	//CString	string = _T("");

	//CString filePath, fileTemp, fileNewBookmark, fileOldBookmark;
	//filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);
	//fileTemp.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);
	//
	//CString	section; 
	//section.Format("%02d", gno);
	//
	//readL = GetPrivateProfileString("GROUPORDER", "00", "", readB, sizeof(readB), filePath);
	//string = CString(readB, readL); 
	//string += section; 
	//string += ";"; 
	//WritePrivateProfileString("GROUPORDER", "00", string, filePath);

	//WritePrivateProfileString("GROUPNAME", section, gname, filePath); 
	//WritePrivateProfileString(nullptr, nullptr, nullptr, filePath);

	//filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);

	//CFile	fileH(filePath, CFile::modeWrite|CFile::modeCreate);

	//if (fileH.m_hFile == CFile::hFileNull) return 0;

	//int	parseL = 0;
	//CString	code(_T(""));
	//CString name(_T(""));
	//

	//CString wB;
	//CString xxP;

	//struct	_inters	inters;
	//struct	_jinfo* jinfo{};

	//for (int ii = 0; ii < count; ii++)
	//{
	//	jinfo = (struct _jinfo *) &datB[parseL];
	//	parseL += sz_jinfo;

	//	memset((void*)&inters, 0, sz_inters);

	//	inters.gubn[0] = jinfo->gubn[0];
	//	CopyMemory(inters.code, jinfo->code, sizeof(jinfo->code));
	//	CopyMemory(inters.xprc, jinfo->xprc, sizeof(jinfo->xprc));
	//	CopyMemory(inters.xnum, jinfo->xnum, sizeof(jinfo->xnum));

	//	code = CString(inters.code, sizeof(inters.code)); 

	//	if(code.Left(1) != " ")
	//	{
	//		code.TrimRight();
	//	}
	//
	//	xxP = GetCodeName(code);

	//	name = xxP; 
	//	name.TrimLeft(); 
	//	name.TrimRight();

	//	if(code.Left(1) == " ")
	//	{
	//		name = "      ";
	//	}

	//	strncpy(inters.name, (LPSTR)(LPCTSTR)name, name.GetLength());
	//	
	//	fileH.Write(&inters, sz_inters);
	//}

	//fileH.Close(); 
	//return count;
}

void CPage1::reInitInterest()   
{
	CString	string, filePath;

	m_activegno = -1; 
	initCombo();

	const int count = loadingInterest(m_activegno);  

	//if(count == 0)
	//	return;

	//HTREEITEM hItem = m_tree.GetRootItem();
	//
	//m_list2.DeleteAllItems();
	//ClearList2item();

	//int nItem{}; _inters* pinter{};
	//for (int ii = 0; ii < count; ii++)
	//{
	//	pinter = m_inters.GetAt(ii);
	//	
	//	if (pinter->code[0] == 'm')
	//	{
	//		nItem = AddItem(&m_list2, "", ii);

	//		if (strlen(pinter->code))
	//			AddItem(&m_list2, pinter->name, nItem, 1);
	//	}
	//	else
	//	{
	//		nItem = AddItem(&m_list2, CString(pinter->code, sizeof(pinter->code)), ii);
	//		if (strlen(pinter->code))
	//			AddItem(&m_list2, GetCodeName(CString(pinter->code, sizeof(pinter->code))), nItem, 1);
	//	}

	//	_listitem* listitem = new _listitem();
	//	listitem->pInter = pinter;
	//	strcpy(listitem->code, pinter->code);
	//	strcpy(listitem->name, pinter->name);

	//	m_list2item.Add(listitem);
	//	m_list2.SetItemData(nItem, (LPARAM) listitem);
	//}

	//updateList2sum();
}

void CPage1::clearAllFile()
{
	//CString filePath; 
	//filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);

	//CString fileTemp; 
	//fileTemp.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);

	//DeleteFile(filePath); 
	//DeleteFile(fileTemp);

	//for (int ii = 0; ii < maxGROUP; ii++)
	//{
	//	filePath.Format("%s/%s/%s/portfolio.i%02d",     m_root, USRDIR, m_name, ii);
	//	fileTemp.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, ii);

	//	DeleteFile(filePath); 
	//	DeleteFile(fileTemp);
	//}
}

void CPage1::enableButton(BOOL enable)
{
	CButton* btn = nullptr;
	
	btn = (CButton *) GetDlgItem(IDC_UPLOAD); btn->EnableWindow(enable);
	btn = (CButton *) GetDlgItem(IDC_DNLOAD); btn->EnableWindow(enable);
}

int CPage1::GetCodeType(CString code)
{
	const int codeL = code.GetLength();
	if (codeL <= 0) return 0;

	const int result = m_parent->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)code);

	switch (result)
	{
	case kospiType:				// kospi code
	case kosdaqType:			// kosdaq code
	case etfType:				// etf code
		return	kospiCODE;;
	case elwType:				// ELW
		return	elwCODE;
	case futureType:
		return	futureCODE;		// ����
	case sfutureType:
		return sfCODE;		
	case callType:				// call option
	case putType:				// put  option
		return  optionCODE;
	case indexType:				// ����
		return	indexCODE;
	case thirdType:				// ��3����
		return	thirdCODE;
	case sinjuType:				// �����μ���
		return	sinjuCODE;
	case mufundType:			// ������ݵ�
	case reitsType:				// ����
	case hyfundType:			// �����ϵ��ݵ�
		return 200;
	}
	
	return 0;
}

CString CPage1::GetCodeName(CString code)
{
	code = code.Left(12);
	code.TrimRight();
	code.TrimLeft();
//	code.Remove(0);
	
//	int codeL = code.GetLength();
	const int codeL = strlen(code);

	if (codeL == 6)					// kospi code
	{
		_shjcode   hjcode; 
		for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
		{
			hjcode = m_hjcode.GetAt(ii);
			if (code != hjcode.code)
				continue;

			switch (hjcode.kosd)
			{
			case jmKOSPI:
			case jmKOSDAQ:
			case jm3RD:
				if (hjcode.ssgb == jmELW)
					return hjcode.name;
				break;
			}
			return hjcode.name.Mid(1);
		}

	}
	else if (codeL == 9)				// �����μ��� �߰�
	{
		_shjcode   hjcode; 
		for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
		{
			hjcode = m_hjcode.GetAt(ii);
			if (code != hjcode.code)
				continue;
			
			switch (hjcode.kosd)
			{
			case jmSINJU:
				if (hjcode.ssgb == jmSINJU)
					return hjcode.name;
				break;
			}
			return hjcode.name.Mid(1);
		}
	}
	else if (codeL <= 3)				// upjong code
	{
		struct	upcode	upcode;
		for (int ii = 0 ; ii < m_upcode.GetSize() ; ii++)
		{
			upcode = m_upcode.GetAt(ii);

			CString upCodeStr;
			int upCodeInt;
			upCodeStr.Format("%d%02d", upcode.jgub, upcode.ucod);
			upCodeInt = atoi(upCodeStr);

			if (atoi(code) != upCodeInt)
				continue;

			return CString(upcode.hnam, UNameLen);
		}
	}
	else if (code[0] == '1' || code[0] == '4')	// future code
	{
		int nGubn = atoi(code.Mid(1,2));
		
		if(nGubn > 10 && nGubn < 60)	//�ֽļ���
		{
			sfcode   sfcode;
			CString strCode;
			for (int ii = 0; ii < m_sfcode.GetSize(); ii++)
			{
				sfcode = m_sfcode.GetAt(ii);
				
				strCode = CString(sfcode.codx , 8);
				if (code != strCode)
					continue;
				
				return CString(sfcode.hnam, 50);
			}
		}
		else if(nGubn > 60 && nGubn < 70)	//�ݸ�����
		{
			ccode   ccode;
			CString strCode;
			for (int ii = 0; ii < m_cicode.GetSize(); ii++)
			{
				ccode = m_cicode.GetAt(ii);
				
				strCode = CString(ccode.codx , 8);
				if (code != strCode)
					continue;
				
				return CString(ccode.hnam, 50);
			}
		}
		else if(nGubn > 70 && nGubn < 80)	//��ȭ����
		{
			ccode   ccode;
			CString strCode;
			for (int ii = 0; ii < m_cccode.GetSize(); ii++)
			{
				ccode = m_cccode.GetAt(ii);
				
				strCode = CString(ccode.codx , 8);
				if (code != strCode)
					continue;
				
				return CString(ccode.hnam, 50);
			}
		}
		else
		{
			_sjcode   sjcode;
			for (int ii = 0; ii < m_fjcode.GetSize(); ii++)
			{
				sjcode = m_fjcode.GetAt(ii);
				if (code != sjcode.code)
					continue;
				
				return sjcode.name;
			}
		}
	}
	else if (code[0] == '2' || code[0] == '3')
	{
		if (code[1] == '0')			// future option code
		{
			struct ojcode	ojcode;
			for (int ii = 0; ii < m_ojcode.GetSize(); ii++)
			{
				ojcode = m_ojcode.GetAt(ii);

				for (int jj = 0; jj < STANDARDNUM; jj++)
				{
					if (ojcode.call[jj].yorn != '1')
						continue;
					if (code != CString(ojcode.call[jj].cod2, OCodeLen))
						continue;

					return CString(ojcode.call[jj].hnam, ONameLen);
				}
				for (int jj = 0; jj < STANDARDNUM; jj++)
				{
					if (ojcode.put[jj].yorn != '1')
						continue;
					if (code != CString(ojcode.put[jj].cod2, OCodeLen))
						continue;

					return CString(ojcode.put[jj].hnam, ONameLen);
				}
			}
		}
		else					// 
		{
			struct	pjcode pjcode;
			for (int ii = 0; ii < m_pjcode.GetSize(); ii++)
			{
				pjcode = m_pjcode.GetAt(ii);
				if (code != CString(pjcode.codx, PCodeLen))
					continue;

				return CString(pjcode.hnam, sizeof(pjcode.hnam));
			}
		}
	}

	return _T("");
}

CString	CPage1::GetGroupORder()
{
	int	readL = 0;
	char	readB[512];
	CString	filePath; filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);

	readL = GetPrivateProfileString(_T("GROUPORDER"), "00", "", readB, sizeof(readB), filePath);
	return CString(readB, readL);
}

CString	CPage1::GetGroupORder2(CString& data)
{
	CString temp1, temp2, string;
	temp1 = "";
	temp2 = "";
	string = "";
	
	data = "";
	for(int i =0 ; i<MAXGROUP ; i++)
	{
		temp1 = m_manageGroup[i][0];
		temp2 = m_manageGroup[i][1];

		if(!temp1.IsEmpty())
		{
			string += temp1;
			string += ";";

			data += temp2;
			data += ";";
		}
	}

	return string;
}

LRESULT CPage1::OnSelectGroup(WPARAM wParam, LPARAM lParam)
{
	SetTimer(WM_TOSELECTPAGE, 400, nullptr);	
	return 0;
}

LRESULT CPage1::OnSelectGroupNCode(WPARAM wParam, LPARAM lParam)
{
	SetTimer(WM_TOSELECTGROUPNCODE, 400, nullptr);	
	return 0;
}

long CPage1::OnAppendParentGroup(WPARAM wParam, LPARAM lParam)
{
	const char gnCHAR = 0x7f;		//DEL
	const char gnTab = 0x09;		//TAB



	if (m_appendGI.IsEmpty())
		return 0;

	CString	gname = parseX(m_appendGI, gnCHAR);

	

	ShowWindow(SW_SHOW);
	CNewGroup newGroup(this, 0, gname);

	if (newGroup.DoModal() != IDOK)
	{
		m_appendGI = _T("");
		return 0;
	}
	
	gname = newGroup.m_defName;
	if (savingGroupOrder(gname))  //OnAppendParentGroup
	{
		OnDelall();
		savingGroupFile(m_activegno, gname);
	}

	//Send �Ͽ� ������ ���Ľ�Ų��
	CWnd* wnd = GetParent()->GetParent();
	wnd->SendMessage(TOmapWnd, MAKEWPARAM(upnewgroupACTION, 0), 0);	

	char	wB[64]{}, *xxP{};
	int	position{};
	CString	code, name, temp;
	char bookmark;
	bookmark = '0';

	while (!m_appendGI.IsEmpty())
	{
		if (m_list2.GetItemCount() >= maxJONGMOK)
			break;

//		char iii[64];
//		memcpy( iii, m_appendGI, 64 );

	
		code = parseX(m_appendGI, gnCHAR);
		temp = parseX(m_appendGI, gnCHAR);
		CopyMemory(&bookmark, temp, min(strlen(temp), sizeof(bookmark)));
//		code = temp;
		
		strcpy(wB, code);
		xxP = (char *)m_parent->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)wB);
		name = xxP; 
		name.TrimLeft(); 
		name.TrimRight();

		position = m_list2.GetItemCount();
		AppendItemToList2(position, 0, code, name, bookmark);
	}

	m_appendGI = _T(""); 
	return 0;
}

long CPage1::OnAppendGroup(WPARAM wParam, LPARAM lParam)
{
	const char gnCHAR = 0x7f;
	
	if (m_appendGI.IsEmpty())
		return 0;
	
	CString	gname = parseX(m_appendGI, gnCHAR);
	
	ShowWindow(SW_SHOW);
	CNewGroup newGroup(this, 0, gname);
	
	if (newGroup.DoModal() != IDOK)
	{
		m_appendGI = _T("");
		return 0;
	}
	
	gname = newGroup.m_defName;
	if (savingGroupOrder(gname))  //OnAppendGroup
	{
		OnDelall();
		savingGroupFile(m_activegno, gname);
	}

	//Send �Ͽ� ������ ���Ľ�Ų��
	CWnd* wnd = GetParent()->GetParent();
	wnd->SendMessage(TOmapWnd, MAKEWPARAM(upnewgroupACTION, 0), 0);		

	char	wB[64]{}, *xxP{};
	int	position{};
	CString	code, name;
	while (!m_appendGI.IsEmpty())
	{
		if (m_list2.GetItemCount() >= maxJONGMOK)
			break;
		
		code = parseX(m_appendGI, gnCHAR);
		
		strcpy(wB, code);
		const int codeSize = strlen(wB);

		if(codeSize > 0)
		{
			xxP = (char *)m_parent->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)wB);
			name = xxP; 
			name.TrimLeft(); 
			name.TrimRight();
			
			position = m_list2.GetItemCount();
			AppendItemToList2(position, 0, code, name);
		}

	}
	
	m_appendGI = _T(""); 
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////
///
void CPage1::AppendItemToList1(int ipos, CString code, CString name, CString bookmark, _listitem *pListItem)
{
	int nItem = 0;
	_listitem* listitem{};
	
	name.TrimRight();
	
	nItem = AddItem(&m_list1, code, ipos);
	AddItem(&m_list1, name, nItem, 1);
	
	if (pListItem)
	{
		m_list1.SetItemData(nItem, (LPARAM) pListItem);
	}
	else	// ó�� ������ �����Ҷ��� �߰��ȴ�.
	{
		listitem = new _listitem();
		strcpy(listitem->code, code);
		strcpy(listitem->name, name);
		m_listitem.Add(listitem);
		m_list1.SetItemData(nItem, (LPARAM) listitem);
	}
}

void CPage1::AppendItemToList1(int ipos, CString code, CString name,  _listitem *pListItem)
{
	code.TrimRight();
	name.TrimRight();
	CITEMS item;
	item.code = code;
	item.name = name;
	//item.date = "";
	m_arList.SetAtGrow(ipos, item);

	_listitem* listitem;
	_listitem* list;
	if (pListItem)
	{

		if (!m_listdata.Lookup(code, (void*&)list))
			m_listdata.SetAt(code, pListItem);
	}
	else	// ó�� ������ �����Ҷ��� �߰��ȴ�.
	{
		listitem = new _listitem();
		//		memset(listitem->name, 0x00, sizeof(listitem->name));
		//		strncpy(listitem->code, code, code.GetLength());
		//		strncpy(listitem->name, name, name.GetLength());
		strcpy(listitem->code, code);
		strcpy(listitem->name, name);
		m_listitem.Add(listitem);
	}

	return;
	/*
	int nItem = 0;
	_listitem* listitem{};

	name.TrimRight();

	nItem = AddItem(&m_list1, code, ipos);
	AddItem(&m_list1, name, nItem, 1);

	if (pListItem)
	{
		m_list1.SetItemData(nItem, (LPARAM) pListItem);
	}
	else	// ó�� ������ �����Ҷ��� �߰��ȴ�.
	{
		listitem = new _listitem();
		strcpy(listitem->code, code);
		strcpy(listitem->name, name);
		m_listitem.Add(listitem);
		m_list1.SetItemData(nItem, (LPARAM) listitem);
	}
	*/
}

void CPage1::AppendItemToList2(int ipos, int gubn, CString code, CString name, CString xprc, CString xnum, CString xupnum, CString filter, CString bookmark, bool add)
{
	int	nItem = 0;

	if(code.Left(1) != " ")
	{
		code.TrimRight();
		if(code.GetAt(0) == 'm')
		{
			name.TrimRight();
		}
		else
		{
			name = GetCodeName(code);
			name.TrimRight();
		}
		xprc.TrimRight();
		xprc.TrimLeft();
		xnum.TrimRight();
		xnum.TrimLeft();
		xupnum.TrimRight();
		xupnum.TrimLeft();
		filter.TrimRight();
		filter.TrimLeft();
	}

	
	// MODIFY PSH 20070918
	//nItem = AddItem(&m_list2, code, ipos);
	CString strCode = ('m' == code.GetAt(0)) ? "" : code;
	nItem = AddItem(&m_list2, strCode, ipos);
	// END MODIFY
	AddItem(&m_list2, name, nItem, 1);
	
	_inters* inter = new _inters();
	ZeroMemory(inter, sizeof(_inters));
	
//	inter->gubn[0] = (char)gubn;
	itoa(gubn, inter->gubn, 10);
	strcpy(inter->code, code);
	strcpy(inter->name, name);
	strcpy(inter->xnum, xnum);
	strcpy(inter->xprc, xprc);
	strcpy(inter->xupnum, xupnum);
	strcpy(inter->filler, filter);
	strcpy(inter->bookmark, bookmark);



	_listitem* listitem = new _listitem();
	listitem->pInter = inter;
	strcpy(listitem->code, code);
	strcpy(listitem->name, name);
	
	if (nItem < m_list2item.GetSize())
		m_list2item.InsertAt(nItem, listitem);
	else
		m_list2item.Add(listitem);
	
	m_list2.SetItemData(nItem, (LPARAM) listitem);
	
	if (add) m_inters.Add(inter);
	else     m_inters.InsertAt(ipos, inter);
	
	updateList2sum();
	
	m_nCol0Asc = -1;
	m_nCol1Asc = -1;
	SetListColumnText(&m_list2, 1, -1);
	SetListColumnText(&m_list2, 0, -1);
}

void CPage1::AppendItemToList2(int ipos, int gubn, CString code, CString name, CString bookmark, bool add /*=true*/)
{
	int	nItem = 0;
	
	if(name.Left(1) != " ")
	{
		name.TrimRight();
	}
	

	// MODIFY PSH 20070918
	//nItem = AddItem(&m_list2, code, ipos);
	CString strCode = ('m' == code.GetAt(0)) ? "" : code;
	nItem = AddItem(&m_list2, strCode, ipos);
	// END MODIFY
	AddItem(&m_list2, name, nItem, 1);

	_inters* pinter = new _inters();
	ZeroMemory(pinter, sizeof(_inters));
	
	itoa(gubn, pinter->gubn, 10);
//	pinter->gubn[0] = (char)gubn;
	strcpy(pinter->code, code);
	strcpy(pinter->name, name);
	strcpy(pinter->bookmark, bookmark);
//	pinter->bookmark[0] = bookmark;

	_listitem* listitem = new _listitem();
	listitem->pInter = pinter;
	strcpy(listitem->code, code);
	strcpy(listitem->name, name);

	if (nItem < m_list2item.GetSize())
		m_list2item.InsertAt(nItem, listitem);
	else
		m_list2item.Add(listitem);
	
	m_list2.SetItemData(nItem, (LPARAM) listitem);

	if (add) 
		m_inters.Add(pinter);
	else    
		m_inters.InsertAt(ipos, pinter);

	updateList2sum();

	m_nCol0Asc = -1;
	m_nCol1Asc = -1;
	SetListColumnText(&m_list2, 1, -1);
	SetListColumnText(&m_list2, 0, -1);
}

void CPage1::DeleteItemToList2(int ipos)
{
	if (m_list2.GetItemCount() > ipos)
		m_list2.DeleteItem(ipos);

	if (m_inters.GetSize() > ipos)
	{
		delete m_inters.GetAt(ipos);
		m_inters.RemoveAt(ipos);
	}

	if (m_list2item.GetSize() > ipos)
	{
		delete m_list2item.GetAt(ipos);
		m_list2item.RemoveAt(ipos);
	}

	updateList2sum();
}

bool CPage1::existCodeInList2(CString code)
{
	CString	chkcode;
	int result = -1;

	if(m_isDuplicateCode == true)	// ���� �ߺ���� ���
	{
		
	}
	else
	{
		for (int ii = 0 ; ii < m_list2.GetItemCount() ; ii++)
		{
			chkcode = m_list2.GetItemText(ii, 0);
			chkcode.TrimRight();
			chkcode.TrimLeft();
			
			if (code.Compare(chkcode) == 0)
			{
				if(!m_bFirstDuplicateNO)	//2012.04.02 KSJ �ѹ��� �����
				{
					result = MessageBox("������ ������ ���ɱ׷� ���� �����մϴ�. �ߺ������ ����Ͻðڽ��ϱ�?", "�������� ����", MB_YESNO);
					m_nResult = result;	//2012.04.02 KSJ ����� ����.
				}
				else 
				{
					result = m_nResult;	//2012.04.02 KSJ ���� ��������� �ٽ� ����
				}
				
				if(result == IDYES)
				{
					m_bFirstDuplicateNO = true;		//2012.04.02 KSJ �ѹ��� �����
					break;
				}
				else
				{
					m_bFirstDuplicateNO = true;			//2012.04.02 KSJ �ѹ��� �����
					return true;	//2012.04.02 KSJ �ߺ���� ������.
				}
			}
		}
	}

	return false;
}

CString	CPage1::GetGroupName(int gno)
{
#ifdef DF_SEARCH
	CString string, stmp;
	CWnd* wnd = GetParent()->GetParent();
	memset(m_pdata, 0x00, 1024 * 4);
	const bool ret = wnd->SendMessage(WM_MSG, MAKEWPARAM(MSG_GET_GROUPARR, 0), (LPARAM)m_pdata);
	if (ret == 0)
		return "";

	string.Format("%s", m_pdata);
	int igroup{};
	while (!string.IsEmpty())
	{
		stmp = parseX(string, "\t");
		igroup = atoi(parseX(stmp, ";"));
		if (gno == igroup)
			return stmp;
	}

	return "";
#else
	CFileFind finder; CString filePath;

	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);

	CString	string; 
	
	string.Format("%02d", gno);
	int	wbL = 0; 
	char	wbX[128];

	wbL = GetPrivateProfileString("GROUPNAME", string, nullptr, wbX, sizeof(wbX), filePath);

	if (wbL <= 0)
	{
		sprintf(wbX, "%s%02d", "��������", gno);
		wbL = strlen(wbX);
	}

	return CString(wbX, wbL);
#endif
}

void CPage1::GuideMsg(CString msg)
{
//	MessageBox("["+msg+"]", "IBK��������");
	Variant(guideCC, msg);
}

bool CPage1::GuideMsg(CString msg, bool question)
{
	UINT nType = MB_OK|MB_ICONEXCLAMATION;
	if (question)
		nType = MB_OKCANCEL|MB_ICONEXCLAMATION;

	if (MessageBox(msg, idTOstring(IDS_REGISTERINTER), nType) == IDCANCEL)
		return false;

	return true;
}

CString CPage1::parseX(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

CString CPage1::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}

CString CPage1::idTOstring(UINT id)
{
	CString	msg;
	if (!msg.LoadString(id))
		msg = _T("");
	return msg;
}

void CPage1::SetOther(class CPage4* page)
{
	m_page = page; 
}

int CPage1::GetActiveGroup()
{
	return m_activegno;
}

void CPage1::SetArray(CArray<struct _inters*, struct _inters*>& arInters)
{
	struct	_inters* pinternew{};
	struct	_inters* pinterold{};
	const int ncnt = m_inters.GetSize();

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		pinterold = m_inters.GetAt(ii);
		pinternew = arInters.GetAt(ii);

		if (strcmp(pinternew->code, pinterold->code) == 0)
		{
			memset(pinterold->xnum, 0x00, sizeof(pinterold->xnum));
			memcpy(pinterold->xnum, pinternew->xnum, sizeof(pinternew->xnum));
			memset(pinterold->xprc, 0x00, sizeof(pinterold->xprc));
			memcpy(pinterold->xprc, pinternew->xprc, sizeof(pinternew->xprc));
			memset(pinterold->bookmark, 0x00, sizeof(pinterold->bookmark));
			memcpy(pinterold->bookmark, pinternew->bookmark, sizeof(pinternew->bookmark));
		}
	}
}

void CPage1::InitEdit()
{
	m_search.SetWindowText("");
	m_search.SetFocus();
}

bool CPage1::SearchWord(CString sName, bool bAddAll)
{
	CString	code, name;
	_listitem* listitem{};
	
	if (sName.IsEmpty())
	{	// add all
		if (!bAddAll)
		{
			m_list1.DeleteAllItems();
			for (int ii = 0; ii < m_listitem.GetSize(); ii++)
			{
				listitem = m_listitem.GetAt(ii);
				
				name = CString(listitem->name, sizeof(listitem->name));
				//if (hjc.ssgb != jmELW)	
				name = name.Mid(1);
				
				code = CString(listitem->code, sizeof(listitem->code));
				name.TrimRight();
				AppendItemToList1(ii, code, name);
			}
			m_list1.SetItemCountEx(m_arList.GetSize());
			m_list1.Invalidate();
		}
		return true;
	}
	
	m_list1.DeleteAllItems();
	
	int	idx = 0;
	
	CPtrArray	arSearch;
	arSearch.RemoveAll();
	for (int ii = 0; ii < m_listitem.GetSize(); ii++)
	{
		listitem = m_listitem.GetAt(ii);
		name = CString(listitem->name, sizeof(listitem->name));	
		if (name.Find(sName) >= 0)	arSearch.Add(listitem);
	}
	
	m_arList.RemoveAll();
	for (int ii = 0; ii < arSearch.GetSize(); ii++)
	{
		listitem = (_listitem*)arSearch.GetAt(ii);
		
		name = CString(listitem->name, sizeof(listitem->name));
		//if (hjc.ssgb != jmELW)	
		//	name = name.Mid(1);
		
		code = CString(listitem->code, sizeof(listitem->code));
		name.TrimRight();
		//2015.11.03 KSJ ExistCodeList �ּ�ó���ϰ� AppendItemToList1(idx++, code, name)�� listitem �߰���.
		//if (!ExistCodeList(code))
		AppendItemToList1(idx++, code, name, listitem);
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return false;
}

bool CPage1::SearchData(int kind, CString sName, bool bAddAll)
{
	if (kind == SK_WORD)
		return SearchWord(sName, bAddAll);
	
	return SearchJongmok(sName, bAddAll);
}

bool CPage1::SearchJongmok(CString sName, bool bAddAll)
{
	CString	code, name;
	_listitem* listitem{};
	int	position = 0;

	if (sName.IsEmpty())
	{
		if (!bAddAll)
		{
			m_list1.DeleteAllItems();
			m_arList.RemoveAll();
			for (int ii = 0; ii < m_listitem.GetSize(); ii++)
			{
				listitem = m_listitem.GetAt(ii);

				name = CString(listitem->name, sizeof(listitem->name));
				code = CString(listitem->code, sizeof(listitem->code));
				AppendItemToList1(position, code, name, listitem);
				position += 1;
			}
			m_list1.SetItemCountEx(m_arList.GetSize());
			m_list1.Invalidate();
		}
		return true;
	}

	m_list1.DeleteAllItems();

	char	cName{};
	const int sLen = sName.GetLength();
	if (sLen == 1)
	{	// �Ϲݰ˻�����(���̸�ŭ �´°͸� ����)
		const int sLen = sName.GetLength();
		m_arList.RemoveAll();
		for (int ii = 0; ii < m_listitem.GetSize(); ii++)
		{
			listitem = m_listitem.GetAt(ii);

			name = CString(listitem->name, sizeof(listitem->name));
			if (strncmp(sName, name, sLen) != 0)
				continue;

			code = CString(listitem->code, sizeof(listitem->code));
			AppendItemToList1(position, code, name, listitem);
			position += 1;
		}
		m_list1.SetItemCountEx(m_arList.GetSize());
		m_list1.Invalidate();
	}
	else
	{	// �ѱ� �˻�
		WORD wHangul{}, wHangul2{}, wStart{}, wEnd{};
		CPtrArray	arSearch;
		arSearch.RemoveAll();
		int ii = 0;

		cName = sName.GetAt(ii);
		if (cName & HANGULMSK)	// �ѱ��϶� 2Byte
		{
			wHangul = MakeHangul(sName.GetAt(ii), sName.GetAt(ii+1));
			if (IsChosung(wHangul, wStart, wEnd))
			{	// �ʼ� ��
				for (int jj = 0; jj < m_listitem.GetSize(); jj++)
				{
					listitem = m_listitem.GetAt(jj);
					name = CString(listitem->name, sizeof(listitem->name));
					//�հ�������
					name.TrimLeft();

					if (name.GetLength() < 2)	continue;
					wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
					if (wStart <= wHangul2 && wHangul2 <= wEnd)	arSearch.Add(listitem);
				}
			}
			else
			{	// �ѱ� ��

				for (int jj = 0; jj < m_listitem.GetSize(); jj++)
				{
					listitem = m_listitem.GetAt(jj);
					name = CString(listitem->name, sizeof(listitem->name));
					//�հ�������
					name.TrimLeft();

					if (name.GetLength() < 2)	continue;
					wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
					if (wHangul == wHangul2)	arSearch.Add(listitem);
				}
			}
			ii += 2;
		}
		else			// �׿ܴ� 1Byte
		{
			for (int jj = 0; jj < m_listitem.GetSize(); jj++)
			{
				listitem = m_listitem.GetAt(jj);
				name = CString(listitem->name, sizeof(listitem->name));

				if (name.GetLength() < 1)	continue;
				if (cName == name.GetAt(ii))	arSearch.Add(listitem);
			}
			ii += 1;
		}

		for (; ii < sLen && arSearch.GetSize();)
		{
			cName = sName.GetAt(ii);
			if (cName & HANGULMSK)	// �ѱ��϶� 2Byte
			{
				wHangul = MakeHangul(sName.GetAt(ii), sName.GetAt(ii+1));
				if (IsChosung(wHangul, wStart, wEnd))
				{	// �ʼ� ��
					for (int jj = arSearch.GetUpperBound(); jj >= 0; jj--)
					{
						listitem = (_listitem*)arSearch.GetAt(jj);
						name = CString(listitem->name, sizeof(listitem->name));
						//�հ�������
						name.TrimLeft();

						if (name.GetLength() < ii+2)	arSearch.RemoveAt(jj);;
						wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
						if (wStart > wHangul2 || wHangul2 > wEnd)	arSearch.RemoveAt(jj);
					}
				}
				else
				{	// �ѱ� ��

					for (int jj = arSearch.GetUpperBound(); jj >= 0; jj--)
					{
						listitem = (_listitem*)arSearch.GetAt(jj);
						name = CString(listitem->name, sizeof(listitem->name));
						//�հ�������
						name.TrimLeft();

						if (name.GetLength() < ii+2)	
							arSearch.RemoveAt(jj);
						wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
						if (wHangul != wHangul2)
							arSearch.RemoveAt(jj);
					}
				}
				ii += 2;
			}
			else			// �׿ܴ� 1Byte
			{
				for (int jj = arSearch.GetUpperBound(); jj >= 0; jj--)
				{
					listitem = (_listitem*)arSearch.GetAt(jj);
					name = CString(listitem->name, sizeof(listitem->name));

					if (name.GetLength() < ii+1)	arSearch.RemoveAt(jj);
					if (cName != name.GetAt(ii))	arSearch.RemoveAt(jj);
				}
				ii += 1;
			}
		}

		m_arList.RemoveAll();
		for (ii = 0; ii < arSearch.GetSize(); ii++)
		{
			listitem = (_listitem*)arSearch.GetAt(ii);

			name = CString(listitem->name, sizeof(listitem->name));
			code = CString(listitem->code, sizeof(listitem->code));
			AppendItemToList1(position, code, name, listitem);
			position += 1;
		}
	}

	if(m_arList.GetSize() > 0)
		qsort(static_cast<void*>(&m_arList[0]), m_arList.GetSize(), sizeof(CITEMS), compareSortName2); //test
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	return false;
}

bool CPage1::IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd)
{	// �ʼ��� ���� �ѱ�����~~
	if (ChosungS > wHangul || wHangul > ChosungE)
		return false;

	const WORD	pChosung[19] = 
	{
		0xA4A1, 0xA4A2, 0xA4A4, 0xA4A7, 0xA4A8, 0xA4A9, 0xA4B1,	// ��,��,��,��,��,��,��
		0xA4B2, 0xA4B3, 0xA4B5, 0xA4B6, 0xA4B7, 0xA4B8, 0xA4B9,	// ��,��,��,��,��,��,��
		0xA4BA, 0xA4BB, 0xA4BC, 0xA4BD, 0xA4BE			// ��,��,��,��,��
	};

	const WORD	pHangulStart[19] = 
	{
		0xB0A1, 0xB1EE, 0xB3AA, 0xB4D9, 0xB5FB, 0xB6F3, 0xB8B6,	// ��,��,��,��,��,��,��
		0xB9D9, 0xBAFC, 0xBBE7, 0xBDCE, 0xBEC6, 0xC0DA, 0xC2A5,	// ��,��,��,��,��,��,¥
		0xC2F7, 0xC4AB, 0xC5B8, 0xC6C4, 0xC7CF			// ��,ī,Ÿ,��,��
	};
	const WORD	pHangulEnd[19] = 
	{
			0xB1ED, 0xB3A9, 0xB4D8, 0xB5FA, 0xB6F2, 0xB8B5,		
		0xB9D8, 0xBAFB, 0xBBE6, 0xBDCD, 0xBEC5, 0xC0D9, 0xC2A4,	
		0xC2F6, 0xC4AA, 0xC5B7, 0xC6C3, 0xC7CE, 0xC8FE			
	};

	int ii = 0;
	for (ii = 0; ii < 19; ii++)
	{
		if (wHangul == pChosung[ii])
		{
			wStart = pHangulStart[ii];
			wEnd = pHangulEnd[ii];
			return true;
		}			
	}

	wStart = pHangulStart[ii] - 1;
	wEnd = pHangulEnd[ii] + 1;

	return true;
}

WORD CPage1::MakeHangul(char cChar1, char cChar2)
{	// 2Byte���ڷκ��� �ѱ��� �ͱ۾��ֶ�~~
	const WORD	wHangul = (cChar1<<8) | (0x00FF & cChar2);
	return wHangul;
}


void CPage1::EnableButton(BOOL bEnable)
{
	constexpr UINT btns[] = { IDC_DNLOADR, IDC_DNLOAD, IDC_NEWGROUP, IDC_EDITGROUP, IDC_ALLSAVE,
		IDC_GNAME, IDC_ADD, IDC_DEL, IDC_ADDALL, IDC_DELALL, IDC_BLANK, IDC_MOVEUP, IDC_MOVEDN,
		IDC_SEARCH, IDC_BTN_SORT01, IDC_BTN_SORT02, IDC_BTN_SORT03, IDC_BTN_SORT04, IDC_BTN_SORT05, 
		IDC_BTN_SORT06, IDC_BTN_SORT07, IDC_BTN_SORT08, IDC_BTN_SORT09, IDC_BTN_SORT10, 
		IDC_BTN_SORT11, IDC_BTN_SORT12, IDC_BTN_SORT13, IDC_BTN_SORT14, IDC_BTN_SORT15, 
		IDC_BTN_SORT16, IDC_BTN_SORT17, IDC_BTN_SORT18, IDC_BTN_SORT19, IDC_BTN_SORT20, 
		IDC_BTN_SORT21, IDC_BTN_SORT22};

	for (int ii = 0; ii < sizeof(btns)/sizeof(UINT); ii++)
	{
		GetDlgItem(btns[ii])->EnableWindow(bEnable);
	}

}

void CPage1::OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if (nullptr != pNMListView)
	{
		switch(pNMListView->iSubItem) {
		case 0:
			{
				if (0 != m_nPrevCol)
				{
					m_bCol0Asc = FALSE;
				}

				m_bCol0Asc = !m_bCol0Asc;
				m_listsort = !m_bCol0Asc;

				ListView_SortItems(m_list1, ListViewCompareFunc, 0);

				SetListColumnText(&m_list1, 0, m_bCol0Asc);
				SetListColumnText(&m_list1, 1, -1);
			}
			break;
		case 1:
			{
				if (1 != m_nPrevCol)
				{
					m_bCol1Asc = FALSE;
				}

				m_bCol1Asc = !m_bCol1Asc;
				m_listsort = !m_bCol1Asc;

				ListView_SortItems(m_list1, ListViewCompareFunc, 1);

				SetListColumnText(&m_list1, 1, m_bCol1Asc);
				SetListColumnText(&m_list1, 0, -1);
			}
			break;
		} 

		m_nPrevCol = pNMListView->iSubItem;
	}

	*pResult = 0;
}

void CPage1::SetListColumnText(CListCtrl* pList, int nCol, int nAsc)
{
	CHeaderCtrl* pCtrl = pList->GetHeaderCtrl();

	if (nullptr != pCtrl)
	{
		CString strText;

		switch(nCol) {
		case 0:
			strText = "�ڵ�";
			break;
		case 1:
			strText = "�����";
			break;
		}

		switch (nAsc) {
		case 0:
			{
				strText += " ��";
			}
			break;
		case 1:
			{
				strText += " ��";
			}
			break;
		}

		HDITEM hdItem;
		hdItem.mask = HDI_TEXT;
		hdItem.pszText = strText.GetBuffer(0);

		pCtrl->SetItem(nCol, &hdItem);
	}
}

void CPage1::OnColumnclickList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if (nullptr != pNMListView)
	{
		BOOL bBase = FALSE;

		switch(pNMListView->iSubItem) {
		case 0:
			{
				if (0 != m_nPrevList2)
				{
					m_nCol0Asc = -1;
				}

				m_nCol0Asc = m_nCol0Asc - 1;

				switch (m_nCol0Asc) {
				case -1:
					break;
				case 0:
					m_listsort = TRUE;
					break;
				default:
					m_listsort = FALSE;
					m_nCol0Asc = 1;
					break;
				}

				if (-1 != m_nCol0Asc)
				{
					ListView_SortItems(m_list2, ListViewCompareFunc, 0);
				}
				else
				{
					bBase = TRUE;
				}

				SetListColumnText(&m_list2, 0, m_nCol0Asc);
				SetListColumnText(&m_list2, 1, -1);
			}
			break;
		case 1:
			{
				if (1 != m_nPrevList2)
				{
					m_nCol1Asc = -1;
				}

				m_nCol1Asc = m_nCol1Asc - 1;

				switch (m_nCol1Asc) {
				case -1:
					break;
				case 0:
					m_listsort = TRUE;
					break;
				default:
					m_listsort = FALSE;
					m_nCol1Asc = 1;
					break;
				}

				if (-1 != m_nCol1Asc)
				{
					ListView_SortItems(m_list2, ListViewCompareFunc, 1);
				}
				else
				{
					bBase = TRUE;
				}

				SetListColumnText(&m_list2, 1, m_nCol1Asc);
				SetListColumnText(&m_list2, 0, -1);
			}
			break;
		} 

		if (TRUE == bBase)
		{
			ClearList2item();
			m_list2.DeleteAllItems();

			int nItem{};
			CString strCode;
			_inters* pinter = nullptr;

			const int nCnt = m_inters.GetSize();
			for (int i = 0; i < nCnt; i++)
			{
				pinter = m_inters.GetAt(i);

				strCode.Format("%s", pinter->code);
				strCode = strCode.Left(12);
				strCode.TrimLeft();
				strCode.TrimRight();

				if (pinter->code[0] == 'm')
					strCode = "";

				nItem = AddItem(&m_list2, strCode, i);
				AddItem(&m_list2, pinter->name, nItem, 1);

				_listitem* listitem = new _listitem();
				listitem->pInter = pinter;
				strcpy(listitem->code, pinter->code);
				strcpy(listitem->name, pinter->name);

				m_list2item.Add(listitem);
				m_list2.SetItemData(nItem, (LPARAM) listitem);
			}
		}

		m_nPrevList2 = pNMListView->iSubItem;
	}
	
	*pResult = 0;
}

void CPage1::OnShareimport()  //���ɱ׷� �ҷ�����
{
	constexpr char szFilter[] = "Share Interest Group Files (*.sig)|*.sig||";
	int nPos = -1;
	UINT	readL = 0;
	struct	_inters* pinter = nullptr;

	CFileDialog dlg(TRUE, nullptr, nullptr, OFN_FILEMUSTEXIST,
                      szFilter, this);
	
	if (IDOK == dlg.DoModal())
	{
		m_bIsEdit = TRUE;

		CString strFile = dlg.GetPathName();
		
		if (strFile.IsEmpty())
			return;

		CWnd* wnd = GetParent()->GetParent();

		CString	strGroupName;
		if (m_activegno != -1)
		{
			strGroupName = GetGroupName(m_activegno);  //OnShareimport
			savingGroupFile(m_activegno, strGroupName);

			//������ ����
			wnd->SendMessage(TOmapWnd, MAKEWPARAM(upEachGroupACTION, m_activegno-1), 0);
		}

		strGroupName = dlg.GetFileName();
		nPos = strGroupName.Find(_T(".sig"));

		if (nPos == -1)
			return;

		strGroupName.Delete(nPos, 4);

		if (savingGroupOrder(strGroupName))  //OnShareimport
		{
			OnDelall();
			savingGroupFile(m_activegno, strGroupName);
		}

		CString temp;
		
		temp.Format("%02d", m_activegno);
		
		if (m_activegno > 1)
		{
			m_manageGroup[m_activegno - 1][0] = temp;
			m_manageGroup[m_activegno - 1][1] = "N";
			m_manageGroup[m_activegno - 1][2] = temp;
			m_manageGroup[m_activegno - 1][3] = strGroupName;
		}
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upnewgroupACTION, 0), 0);

		//ClearInterest();
		int count = 0;

		CFile fileH(strFile, CFile::modeRead);
		int i = 0;
		for (i = 0; i < maxJONGMOK; i++)
		{
			pinter = (_inters *) new char[sz_inters];
			ZeroMemory(pinter, sz_inters);
			
			readL = fileH.Read(pinter, sz_inters);
			if (readL < sz_inters)
			{
				delete pinter;
				break;
			}
			
			m_inters.Add(pinter);
		}
		
		fileH.Close();

		count = i;

		CString tcode;
		int nItem{}; _inters* pinter{};
		for (int ii = 0; ii < count; ii++)
		{
			pinter = m_inters.GetAt(ii);
			
			if (pinter->code[0] == 'm')
			{
				nItem = AddItem(&m_list2, "", ii);
				
				if (strlen(pinter->code))
					AddItem(&m_list2, pinter->name, nItem, 1);
			}
			
			else
			{
				tcode = pinter->code;
				tcode = tcode.Mid(0,12);
				tcode.TrimRight();
				nItem = AddItem(&m_list2, tcode, ii);
				if (strlen(pinter->code))
					AddItem(&m_list2, GetCodeName(tcode), nItem, 1);
			}
			
			_listitem* listitem = new _listitem();
			listitem->pInter = pinter;
			strcpy(listitem->code, tcode);
			strcpy(listitem->name, GetCodeName(tcode));
			
			m_list2item.Add(listitem);
			m_list2.SetItemData(nItem, (LPARAM) listitem);
			// END MODIFY
		}
		
		updateList2sum();
	}
}

void CPage1::OnShareexport()  //���ɱ׷� ��������
{
	CString strGroupName = GetGroupName(m_activegno);  //OnShareexport

	constexpr char szFilter[] =
		"Share Interest Group Files (*.sig)|*.sig||";

	CFileDialog dlg(FALSE, nullptr, (LPCTSTR)strGroupName, OFN_FILEMUSTEXIST,
                      szFilter, this);

	if (IDOK == dlg.DoModal())
	{
		m_bIsEdit = TRUE;
	
		CString	strFile = dlg.GetPathName();
		
		if (strFile.IsEmpty())
			return;

		strFile += _T(".sig");

		struct	_inters* pinter{};
		
		CFile	fileH(strFile, CFile::modeWrite|CFile::modeCreate);
		if (fileH.m_hFile == CFile::hFileNull) return;
		
		_listitem* plist{};
		const int nCnt = m_list2item.GetSize();
		for (int ii = 0; ii < nCnt; ii++)
		{
			plist  = ((_listitem*)m_list2.GetItemData(ii));
			pinter = plist->pInter;
			fileH.Write(pinter, sizeof(_inters));
		}
		
		fileH.Close();
	} 
}

void CPage1::ExactGroupData()
{
#ifdef DF_SEARCH
	m_list2.DeleteAllItems();
	ClearList2item();

	if (m_selectGroup != -1)
	{
		m_gname.SetCurSel(m_selectGroup);
		m_activegno = (int)m_gname.GetItemData(m_selectGroup);
	}

	if (m_activegno != -1)
		loadingInterest(m_activegno);  //�����Ϸ�
#else
	m_list2.DeleteAllItems();
	ClearList2item();
	
	if(m_selectGroup != -1)
	{
		m_gname.SetCurSel(m_selectGroup);
		m_activegno = (int)m_gname.GetItemData(m_selectGroup);
	}
	
	if (m_activegno != -1)
	{
//		int nBreakCnt = 0;
		const int count = loadingInterest(m_activegno);   //�����Ϸ�
		CString tcode(_T(""));
		int nItem{};
		_inters* pinter{};
		
		for (int ii = 0; ii < count; ii++)
		{
			pinter = m_inters.GetAt(ii);
			
			if (pinter->code[0] == 'm')
			{
				nItem = AddItem(&m_list2, "", ii);
				
				if (strlen(pinter->code))
				{
					AddItem(&m_list2, pinter->name, nItem, 1);
				}
				
			}
			else
			{
				tcode = CString(pinter->code, sizeof(pinter->code));
				tcode = tcode.Left(12);
				tcode.TrimLeft(); 
				tcode.TrimRight(); 
//				tcode.Remove(0);
				
				nItem = AddItem(&m_list2, tcode, ii);
				CString name;
				name = GetCodeName(pinter->code);
				name.TrimLeft();name.TrimRight();

				AddItem(&m_list2, name, nItem, 1);
			}
			
			_listitem* listitem = new _listitem();
			listitem->pInter = pinter;
			strcpy(listitem->code, pinter->code);
			strcpy(listitem->name, pinter->name);
			
			listitem->name[31] = 0;	//2016.01.25 KSJ 225050 �������� 33byte�̹Ƿ� �ΰ��� ���� �ʾ� �����߻��Ѵ�.

			m_list2item.Add(listitem);
			m_list2.SetItemData(nItem, (LPARAM) listitem);
			// END MODIFY
		}
	}
	
	updateList2sum();	
#endif
}
void CPage1::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == WM_TOSELECTPAGE)
	{
		KillTimer(WM_TOSELECTPAGE);

		ExactGroupData();
	}
	else if(nIDEvent == WM_TOSELECTGROUPNCODE)
	{
		KillTimer(WM_TOSELECTGROUPNCODE);
		
		//ELW���� �ľ�
		analyzeELWCode();
			
		//�׷켱��
		ExactGroupData();
	
		//������
		searchAction(0);
	}
	CPropertyPage::OnTimer(nIDEvent);
}

void CPage1::analyzeELWCode()
{
	//ELW���� �ľ�
// 	CString	string; 
// 	int code_first = 0;
// 	m_search.GetWindowText(string);
// 	
// 	code_first = atoi(string.Left(1));
// 	if(code_first >= 5 && code_first < 9)
// 	{
// 		selectElw();
// 		m_search.SetWindowText(string);
// 	}
	//ELW���� �ľ�
	CString	string; 
	int code_first = -1;
	m_search.GetWindowText(string);
		
	
	if(string.GetLength() <= 0)
	{
		return;
	}
	
	const BYTE test = string.GetAt(0);
	if(IsDBCSLeadByte(test))
	{
		//�ѱ��� ����
		return;
	}
	else
	{
		code_first = atoi(string.Left(1));
	}
	
	if(string.GetLength() == 1)
	{
		if(code_first >= 5 && code_first < 9)
		{
			selectElw();
			m_search.SetWindowText(string);
		}
		else if(code_first >=0 && code_first < 5 || code_first == 9)
		{
			//selectKospi();
			m_search.SetWindowText(string);
		}
		
		m_search.SetSel(0, -1);
		m_search.ReplaceSel(string);
		m_search.SetFocus();
	}
}

int	CPage1::GetGroupCount()
{
	const int result = m_gname.GetCount();

	return result;
}

void CPage1::setManageGroup(char* datB)
{
	#define MAX_JM  120             /* �ִ� ���ɱ׷� ����   */
	#define MAX_SIZE  4084            /* �ִ� ���ɱ׷� ����   */
	
	struct  glist {
		char    ngrs[2];           /* New Group Seqn       */                                               
		char    ogrs[2];           /* New Group Seqn       */
		char    gnam[30];           /* Group Name           */
	};
	
	struct  grpfold {
		char    nrec[4];           /* Group Count          */
		struct  glist   glist[MAX_JM];
	};

	struct grpfold* gFold{};
	gFold = (struct grpfold *)(datB);

	int nCount = 0;
	char nrec[4];

	CopyMemory(nrec, gFold->nrec, sizeof(gFold->nrec));

	nCount = atoi(nrec);
	struct glist list{};

	CString tempAry[MAXGROUP][4];
	CString newNos, oldNoS;

	if(nCount > 0)
	{
		//�ӽù迭�� ����
		for(int i=0 ; i<MAXGROUP ; i++)
		{
			for(int j=0 ; j<4 ; j++)
			{
				tempAry[i][j] = m_manageGroup[i][j];
			}
		}
		
		for(int i=0 ; i<nCount ; i++)
		{
			memcpy(&list, &gFold->glist[i], sizeof(glist));
			
			newNos.Format("%.2s", list.ngrs);
			oldNoS.Format("%.2s", list.ogrs);
			
			if(atoi(oldNoS) != atoi(newNos))
			{
				for(int j=0 ; j<4 ; j++)
				{
					if(j != 2)
					{
						m_manageGroup[atoi(newNos)-1][j] = tempAry[atoi(oldNoS)-1][j];
					}	
				}				
			}
		}
	}

}

void CPage1::sortManageGroup(int drag, int drop)
{
	const int maxCount = getManageCount();
	CString drag1, drag2, drag3, temp1, temp2, temp3;

	drag1 = ""; drag2 = ""; drag3 = "";
	temp1 = ""; temp2 = ""; temp3 = "";

	if(drop == 0)
	{
		drag1 = m_manageGroup[drag][0];
		drag2 = m_manageGroup[drag][1];
		drag3 = m_manageGroup[drag][3];	
		
		//������ �ϳ��� ����
		for(int i=drag ; i>drop ; i--)
		{
			m_manageGroup[i][0] = m_manageGroup[i-1][0]; 
			m_manageGroup[i][1] = m_manageGroup[i-1][1]; 
			m_manageGroup[i][3] = m_manageGroup[i-1][3];							
		}
		
		//�ű� �ڸ��� ����
		m_manageGroup[drop][0] = drag1;
		m_manageGroup[drop][1] = drag2;
		m_manageGroup[drop][3] = drag3;			
	}
	else if(drop == maxCount)
	{
		//�ϴ� �ű� ���� ���� 
		drag1 = m_manageGroup[drag][0];
		drag2 = m_manageGroup[drag][1];
		drag3 = m_manageGroup[drag][3];
		
		//���� �ϳ��� ����
		for(int i=drag ; i<drop ; i++)
		{
			m_manageGroup[i][0] = m_manageGroup[i+1][0]; 
			m_manageGroup[i][1] = m_manageGroup[i+1][1]; 
			m_manageGroup[i][3] = m_manageGroup[i+1][3];							
		}
		
		//�ű� �ڸ��� ����
		m_manageGroup[drop][0] = drag1;
		m_manageGroup[drop][1] = drag2;
		m_manageGroup[drop][3] = drag3;	
	}
	else
	{
		if(drag < drop)
		{	
			//�ϴ� �ű� ���� ���� 
			drag1 = m_manageGroup[drag][0];
			drag2 = m_manageGroup[drag][1];
			drag3 = m_manageGroup[drag][3];

			//���� �ϳ��� ����
			for(int i=drag ; i<drop ; i++)
			{
				m_manageGroup[i][0] = m_manageGroup[i+1][0]; 
				m_manageGroup[i][1] = m_manageGroup[i+1][1]; 
				m_manageGroup[i][3] = m_manageGroup[i+1][3];							
			}

			//�ű� �ڸ��� ����
			m_manageGroup[drop][0] = drag1;
			m_manageGroup[drop][1] = drag2;
			m_manageGroup[drop][3] = drag3;			
		}
		else if(drag > drop)
		{
			drag1 = m_manageGroup[drag][0];
			drag2 = m_manageGroup[drag][1];
			drag3 = m_manageGroup[drag][3];	
			
			//������ �ϳ��� ����
			for(int i=drag ; i>drop ; i--)
			{
				m_manageGroup[i][0] = m_manageGroup[i-1][0]; 
				m_manageGroup[i][1] = m_manageGroup[i-1][1]; 
				m_manageGroup[i][3] = m_manageGroup[i-1][3];							
			}
			
			//�ű� �ڸ��� ����
			m_manageGroup[drop][0] = drag1;
			m_manageGroup[drop][1] = drag2;
			m_manageGroup[drop][3] = drag3;				
		}
	}
}

void CPage1::OnMovetop() 
{
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	if (pos == nullptr)
	{
		GuideMsg(idTOstring(IDS_GUIDE2));
		return;
	}
	
	m_bIsEdit = TRUE;
	
	const int nitem = m_list2.GetNextSelectedItem(pos);
	
	if (nitem == 0) 
		return;
	
	int gubn{};
	CString code, name;
	char bookmark;
	bookmark = '0';
	
	
	_inters* pinter = m_inters.GetAt(nitem);
	
	gubn = (int)pinter->gubn[0];
	code = CString(pinter->code, sizeof(pinter->code));
	// MODIFY PSH 20070918
	//name = GetCodeName(pinter->code);
	name = ('m' == pinter->code[0]) ? pinter->name : GetCodeName(pinter->code);
	// END MODIFY
	bookmark = pinter->bookmark[0] == '1' ? '1':'0';//2015.04.03 KSJ 1�̾ƴϸ� 0���� ���ش�.
	
	delete pinter; 
	m_inters.RemoveAt(nitem);
	m_list2.DeleteItem(nitem);
	
	delete m_list2item.GetAt(nitem);
	m_list2item.RemoveAt(nitem);
	
	AppendItemToList2(0, gubn, code, name, bookmark, false);
	deselcMark(&m_list2); 
	selcMark(&m_list2, 0);	
}

void CPage1::OnMovebottom() 
{
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	if (pos == nullptr)
	{
		GuideMsg(idTOstring(IDS_GUIDE2));
		return;
	}
	
	m_bIsEdit = TRUE;
	
	const int nitem = m_list2.GetNextSelectedItem(pos);
	if (nitem >= m_list2.GetItemCount()-1) return;
	
	int gubn{}; CString code, name;
	char bookmark;
	bookmark = '0';
	
	_inters* pinter = m_inters.GetAt(nitem);
	
	gubn = (int)pinter->gubn[0];
	code = CString(pinter->code, sizeof(pinter->code));
	// MODIFY PSH 20070918
	//name = GetCodeName(pinter->code);
	name = ('m' == pinter->code[0]) ? pinter->name : GetCodeName(pinter->code);
	// END MODIFY
	bookmark = pinter->bookmark[0] == '1' ? '1':'0';//2015.04.03 KSJ 1�̾ƴϸ� 0���� ���ش�.
	
	delete pinter; 
	m_inters.RemoveAt(nitem);
	m_list2.DeleteItem(nitem);
	
	delete m_list2item.GetAt(nitem);
	m_list2item.RemoveAt(nitem);
	
	AppendItemToList2(m_list2.GetItemCount(), gubn, code, name, bookmark, false);
	deselcMark(&m_list2); 
	selcMark(&m_list2, m_list2.GetItemCount()-1);	
}

void CPage1::loadingETFcode()
{
	CString ETFile;

	char* buf{};
	_efopitem* efop_item{};
	_efoptema *efop_tema{};
	_efopfore *efop_fore{};
	_efopcode *efop_code{};

	CString data, line;
	CString type, opcd, hnam;
	CString tmcd, frcd, frnm, code, etfm, etfl;
	
	CString gubn;
	
	ETFile.Format("%s/%s/efcode.dat", m_root, TABDIR);
	CStdioFile file(ETFile, CFile::modeRead);


	
	while(file.ReadString(line))
	{

		gubn = line.Left(1);

		
		buf = line.GetBuffer(line.GetLength());
		if(strcmp(gubn, "1") == 0)
		{
			_efopitem *efopitem = (struct _efopitem*)buf;
			efop_item = new _efopitem();
			
			type = CString(efopitem->type, sizeof(efopitem->type));
			opcd = CString(efopitem->opcd, sizeof(efopitem->opcd));
			hnam = CString(efopitem->hnam, sizeof(efopitem->hnam));
			hnam.TrimLeft(); hnam.TrimRight();
			
			strcpy(efop_item->type, type);
			strcpy(efop_item->opcd, opcd);
			strcpy(efop_item->hnam, hnam);

			m_ETFitem.Add(efop_item);

			if(efop_item != nullptr)
			{
				efop_item = nullptr;
				delete efop_item;
			}
		}
		else if(strcmp(gubn, "2") == 0)
		{
			_efoptema *efoptema = (struct _efoptema*)buf;
			efop_tema = new _efoptema();

			type = CString(efoptema->type, sizeof(efoptema->type));
			tmcd = CString(efoptema->tmcd, sizeof(efoptema->tmcd));
			hnam = CString(efoptema->tmnm, sizeof(efoptema->tmnm));
			hnam.TrimLeft(); hnam.TrimRight();

			strcpy(efop_tema->type, type);
			strcpy(efop_tema->tmcd, tmcd);
			strcpy(efop_tema->tmnm, hnam);

			m_ETFtema.Add(efop_tema);
			
			if(efop_tema != nullptr)
			{
				efop_tema = nullptr;
				delete efop_tema;
			}
		}
		else if(strcmp(gubn, "3") == 0)
		{
		
			_efopfore *efopfore = (struct _efopfore*)buf;
			efop_fore = new _efopfore();

			type = CString(efopfore->type, sizeof(efopfore->type));
			frcd = CString(efopfore->frcd, sizeof(efopfore->frcd));
			frnm = CString(efopfore->frnm, sizeof(efopfore->frnm));
			frnm.TrimLeft(); frnm.TrimRight();

			strcpy(efop_fore->type, type);
			strcpy(efop_fore->frcd, frcd);
			strcpy(efop_fore->frnm, frnm);
			
// 			strncpy(efop_fore.type, type, sizeof(type)+1);
// 			strncpy(efop_fore.frcd, frcd, sizeof(frcd)+1);
// 			strncpy(efop_fore.frnm, frnm, sizeof(frnm)+1);
			m_ETFfore.Add(efop_fore);

			if(efop_fore != nullptr)
			{
				efop_fore = nullptr;
				delete efop_fore;
			}
		}
		else if(strcmp(gubn, "4") == 0)
		{
		
			_efopcode *efopcode = (struct _efopcode*)buf;
			efop_code = new _efopcode();
			
			type = CString(efopcode->type, sizeof(efopcode->type));
			code = CString(efopcode->code, sizeof(efopcode->code));
			hnam = CString(efopcode->hnam, sizeof(efopcode->hnam));
			hnam.TrimLeft(); hnam.TrimRight();

			opcd = CString(efopcode->opcd, sizeof(efopcode->opcd));
			etfm = CString(efopcode->etfm, sizeof(efopcode->etfm));
			etfl = CString(efopcode->etfl, sizeof(efopcode->etfl));

			strcpy(efop_code->type, type);
			strcpy(efop_code->code, code);
			strcpy(efop_code->hnam, hnam);
			strcpy(efop_code->opcd, opcd);
			strcpy(efop_code->etfm, etfm);
			strcpy(efop_code->etfl, etfl);
			


			m_ETFcode.Add(efop_code);
			
			efop_code = nullptr;
			delete efop_code;
// 			if(efop_code != nullptr)
// 			{
// 				efop_code = nullptr;
// 				delete [] efop_code;
// 			}
		}
	
	}

// 	if(efop_item != nullptr)
// 		delete efop_item;
// 	if(efop_fore != nullptr)
// 		delete efop_fore;
// 	if(efop_tema != nullptr)
// 		delete efop_tema;
// 	if(efop_code != nullptr)
// 		delete efop_code;

	file.Close();
	

}

void CPage1::ETFInitialList()
{
	_efopcode* efopcode{};	
	CString type, opcd, hnam;
	CString frcd, code, etfm, etfl;
	
	int position = 0;	
	ClearListitem();
	m_list1.DeleteAllItems();
		
	const int ncnt = m_ETFcode.GetSize();
	for(int ii = 0; ii < ncnt; ii++)
	{
		efopcode = m_ETFcode.GetAt(ii);
		
		type = CString(efopcode->type, sizeof(efopcode->type));
		code = CString(efopcode->code, sizeof(efopcode->code));
		hnam = CString(efopcode->hnam, sizeof(efopcode->hnam));
		hnam.TrimLeft(); hnam.TrimRight();
		
		opcd = CString(efopcode->opcd, sizeof(efopcode->opcd));
		etfm = CString(efopcode->etfm, sizeof(efopcode->etfm));
		etfl = CString(efopcode->etfl, sizeof(efopcode->etfl));
		
		
		
		AppendItemToList1(position, code, hnam);
		position += 1;
	}		
//	m_search = FALSE;
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
}

void CPage1::OnRadioClick() 
{
	HTREEITEM hItem{};
	HTREEITEM hParent{};
	CString str;
	CString parent;
	
	hItem = m_tree.GetSelectedItem();
	hParent = m_tree.GetParentItem(hItem);
	
	str = m_tree.GetItemText(hItem);
	parent = m_tree.GetItemText(hParent);
	
	if(parent.Compare("������") == 0)
	{
		DataReloadELWAll(str);
		ListView_SortItems(m_list1, ListViewCompareFunc, 0);
	}
	else if(parent.Compare("�����ڻ�����") == 0)
	{
		DataReloadELWBase(str);
		ListView_SortItems(m_list1, ListViewCompareFunc, 0);
	}
	else if(parent.Compare("�����") == 0)
	{
		DataReloadELWPB(str);
		ListView_SortItems(m_list1, ListViewCompareFunc, 0);
	}
	
}

void CPage1::DataReloadELWAll(CString str)
{
	_elwdata elwdata;
	_elwdata elw;
	CString code, name, ktype, pcode, pname, mjmt;
	int bkoba{};
	CString rdocheck;
	_listitem2* item{};
	CPtrArray arCode;
	CPtrArray gbCode;

	if(str.Compare("�Ϲ�ELW") == 0)
		bkoba = 0;
	else
		bkoba = 1;

	m_arList.RemoveAll();

	if(m_Radio_ELWAll.GetCheck())
	{
		rdocheck.Format("%02d", 3);
	}
	else if(m_Radio_ELWCall.GetCheck())
	{
		rdocheck.Format("%02d", 1);
	}
	else
		rdocheck.Format("%02d", 2);
	
	const int ncnt = m_ELWdata.GetSize();
	for(int ii = 0; ii < ncnt; ii++)
	{
		elwdata = m_ELWdata.GetAt(ii);	
		item = new _listitem2;
		
		code = CString(elwdata.code, 12);
		name = CString(elwdata.name, ELWHNameLen);
		
		code.TrimRight(); code.TrimRight();
		name.TrimLeft(); name.TrimRight();

		strcpy(item->code, code);
		strcpy(item->name, name);

		if(bkoba == elwdata.bkoba && rdocheck.Compare(elwdata.ktype) == 0)
			arCode.Add(item);
		else if(bkoba == elwdata.bkoba && rdocheck.Compare("03") == 0)
 			arCode.Add(item);
		else
			gbCode.Add(item);
	}


	m_list1.DeleteAllItems();
	LockWindowUpdate();

	
	ClearListitem();

	const int narCode = arCode.GetSize();
	for(int ii = 0; ii < narCode; ii++)
	{
		item = (_listitem2*)arCode.GetAt(ii);

		AppendItemToList1(ii, item->code, item->name);
	}

	m_list1.SetItemCountEx(m_arList.GetSize());
	if (m_arList.GetSize() > 0)
		qsort(static_cast<void*>(&m_arList[0]), m_arList.GetSize(), sizeof(CITEMS), compareSortCode2);
	m_list1.Invalidate();

	UnlockWindowUpdate();

	for(int ii = 0; ii < arCode.GetSize(); ii++)
		delete arCode.GetAt(ii);
	arCode.RemoveAll();
	
	for(int ii = 0; ii < gbCode.GetSize(); ii++)
		delete gbCode.GetAt(ii);
	gbCode.RemoveAll();
}

void CPage1::DataReloadELWPB(CString str)
{
	_elwdata elwdata;
//	_elwdata *elw;
	CString code, name, ktype, pcode, pname, mjmt;
//	BOOL bkoba;
	int row = 0;
	CString rdocheck;

	if(m_Radio_ELWAll.GetCheck())
	{
		rdocheck.Format("%02d", 3);
	}
	else if(m_Radio_ELWCall.GetCheck())
	{
		rdocheck.Format("%02d", 1);
	}
	else
		rdocheck.Format("%02d", 2);

	m_list1.DeleteAllItems();
	ClearListitem();
	
	const int nelwdata = m_ELWdata.GetSize();
	for(int ii = 0; ii < nelwdata; ii++)
	{
		elwdata = m_ELWdata.GetAt(ii);
			
		code = CString(elwdata.code, ELWCodeLen);
		name = CString(elwdata.name, ELWHNameLen);
		ktype = CString(elwdata.ktype, 2);
		pcode = CString(elwdata.pcode, 12);
		pname = CString(elwdata.pname, 20);
		mjmt = CString(elwdata.mjmt, 12);
		//bkoba = CString(elwdata->bkoba);
		
		pname.TrimLeft("+");

		if(pname.Compare(str) == 0 && ktype == rdocheck)
		{
			AppendItemToList1(row, code, name);
			row++;
		}
		else if(pname.Compare(str) == 0 && rdocheck.Compare("03") == 0)
		{
			AppendItemToList1(row, code, name);
			row++;
		}
	}

	m_list1.SetItemCountEx(m_arList.GetSize());
	if (m_arList.GetSize() > 0)
		qsort(static_cast<void*>(&m_arList[0]), m_arList.GetSize(), sizeof(CITEMS), compareSortCode2);
	m_list1.Invalidate();
}

void CPage1::DataReloadELWBase(CString str)
{
	_elwdata elwdata;
//	_elwdata *elw;
	CString code, name, ktype, pcode, pname, mjmt, bcode[5];
//	BOOL bkoba;
	int row = 0;
	CString rdocheck;
	CString bscode, bsname, basename;
	_sjcode sjcode;

	if(m_Radio_ELWAll.GetCheck())
	{
		rdocheck.Format("%02d", 3);
	}
	else if(m_Radio_ELWCall.GetCheck())
	{
		rdocheck.Format("%02d", 1);
	}
	else
		rdocheck.Format("%02d", 2);
	
	m_list1.DeleteAllItems();
	ClearListitem();

	const int nelwdata = m_ELWdata.GetSize();
	for(int ii = 0; ii < nelwdata; ii++)
	{
		elwdata = m_ELWdata.GetAt(ii);
		
		code = CString(elwdata.code, ELWCodeLen);
		name = CString(elwdata.name, ELWHNameLen);
		ktype = CString(elwdata.ktype, 2);
		pcode = CString(elwdata.pcode, 12);
		pname = CString(elwdata.pname, 20);
		mjmt = CString(elwdata.mjmt, 12);
		
		for (int jj = 0; jj < 5; jj ++)
		{
			bcode[jj] = CString(elwdata.bcode[jj], 12);
			bcode[jj].TrimRight();
			bcode[jj] = bcode[jj].Mid(1);
		}

	
		if(bcode[0].Compare("OSPI200") == 0)
		{
			basename = "KOSPI200";
			
		}
		else if(bcode[0].Compare("ASKET") == 0)
		{
			basename = "BASKET";
		
		}
		else
		{
			const int nelwbase = m_elwbase.GetSize();
			for(int jj = 0; jj < nelwbase; jj++)
			{
				sjcode = m_elwbase.GetAt(jj);
				bscode = sjcode.code;
				bsname = sjcode.name;
				
				if(bscode.Compare(bcode[0]) == 0)
				{
					basename = bsname;
					break;
				}
			
			}
		}
		//bkoba = CString(elwdata->bkoba);
		
		
		if(basename.Compare(str) == 0 && ktype == rdocheck)
		{
			AppendItemToList1(row, code, name);
			row++;
		}
		else if(basename.Compare(str) == 0 && rdocheck.Compare("03") == 0)
		{
			AppendItemToList1(row, code, name);
			row++;
		}
	}
	m_list1.SetItemCountEx(m_arList.GetSize());
	if (m_arList.GetSize() > 0)
		qsort(static_cast<void*>(&m_arList[0]), m_arList.GetSize(), sizeof(CITEMS), compareSortCode2);
	m_list1.Invalidate();
}

void CPage1::DataGubn(HTREEITEM hItem)
{
	CString		gubn;
	CString		data;
	ClearListitem();
	m_list1.DeleteAllItems();
		
	HTREEITEM hParentItem;
	
	hParentItem = m_tree.GetParentItem(hItem);
	
//	m_searchData.RemoveAll();
	m_list1.DeleteAllItems();

	gubn = m_tree.GetItemText(hParentItem);
	data = m_tree.GetItemText(hItem);

	if(gubn.Compare("���纰") == 0)
	{
		DataReloadItem(data);
	}
	else if(gubn.Compare("�����ڻ꺰") == 0)	//2015.11.04 KSJ �׸��� --> �����ڻ꺰�� ����
	{
		DataReloadTema(data);
	}
	else if(gubn.Compare("����/��") == 0)
	{
		DataReloadFore(data);
	}

//	m_search = TRUE;

 }

void CPage1::DataReloadItem(CString data)
{
	_efopcode* efopcode{};
	_efopitem* efopitem{};

	CString type, opcd, hnam;
	CString tmcd, frcd, frnm, code, etfm, etfl;
	
	int row = 0;
	const int nETFitem = m_ETFitem.GetSize();
	for(int ii = 0; ii < nETFitem; ii++)
	{
		efopitem = m_ETFitem.GetAt(ii);

		type = CString(efopitem->type, sizeof(efopitem->type));
		opcd = CString(efopitem->opcd, sizeof(efopitem->opcd));
		hnam = CString(efopitem->hnam, sizeof(efopitem->hnam));
		hnam.TrimLeft(); hnam.TrimRight();

		if(hnam.Compare(data) == 0)
		{
			data = opcd;
			break;
		}
	}

	const int nETFcode = m_ETFcode.GetSize();
	for(int ii = 0; ii < nETFcode; ii++)
	{
		efopcode = m_ETFcode.GetAt(ii);
		
		type = CString(efopcode->type, sizeof(efopcode->type));
		code = CString(efopcode->code, sizeof(efopcode->code));
		hnam = CString(efopcode->hnam, sizeof(efopcode->hnam));
		hnam.TrimLeft(); hnam.TrimRight();
		
		opcd = CString(efopcode->opcd, sizeof(efopcode->opcd));
		etfm = CString(efopcode->etfm, sizeof(efopcode->etfm));
		etfl = CString(efopcode->etfl, sizeof(efopcode->etfl));
		
		if(data.Compare(opcd) == 0)
		{
	//		AddItem(row, code, hnam, TRUE);
			AppendItemToList1(row, code, hnam);
			row++;
		}
	}	

	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
	
}

void CPage1::DataReloadTema(CString data)
{
	_efopcode* efopcode{};
	_efoptema* efoptema{};
	
	CString type, opcd, hnam;
	CString tmcd, frcd, frnm, code, etfm, etfl;
	
	int row = 0;
	const int nETFtema = m_ETFtema.GetSize();
	for(int ii = 0; ii < nETFtema; ii++)
	{
		efoptema = m_ETFtema.GetAt(ii);
		
		type = CString(efoptema->type, sizeof(efoptema->type));
		tmcd = CString(efoptema->tmcd, sizeof(efoptema->tmcd));
		hnam = CString(efoptema->tmnm, sizeof(efoptema->tmnm));
		hnam.TrimLeft(); hnam.TrimRight();
		
		if(hnam.Compare(data) == 0)
		{
			data = tmcd;
			break;
		}
				
	}
	
	const int nETFcode = m_ETFcode.GetSize();
	for(int ii = 0; ii < nETFcode; ii++)
	{
		efopcode = m_ETFcode.GetAt(ii);
		
		type = CString(efopcode->type, sizeof(efopcode->type));
		code = CString(efopcode->code, sizeof(efopcode->code));
		hnam = CString(efopcode->hnam, sizeof(efopcode->hnam));
		hnam.TrimLeft(); hnam.TrimRight();
		
		opcd = CString(efopcode->opcd, sizeof(efopcode->opcd));
		etfm = CString(efopcode->etfm, sizeof(efopcode->etfm));
		etfl = CString(efopcode->etfl, sizeof(efopcode->etfl));
		
		if(data.Compare(etfm) == 0)
		{
			AppendItemToList1(row, code, hnam);
			row++;
		}
	}	
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
}

void CPage1::DataReloadFore(CString data)
{
	_efopcode* efopcode{};
	_efopfore* efopfore{};
	
	CString type, opcd, hnam;
	CString tmcd, frcd, frnm, code, etfm, etfl;
	
	int row = 0;
	const int nETFfore = m_ETFfore.GetSize();
	for(int ii = 0; ii < nETFfore; ii++)	
	{
		efopfore = m_ETFfore.GetAt(ii);
		
		type = CString(efopfore->type, sizeof(efopfore->type));
		frcd = CString(efopfore->frcd, sizeof(efopfore->frcd));
		frnm = CString(efopfore->frnm, sizeof(efopfore->frnm));
		frnm.TrimLeft(); frnm.TrimRight();
		
		if(frnm.Compare(data) == 0)
		{
			data = frcd;
		}
	}
	
	const int nETFcode = m_ETFcode.GetSize();
	for(int ii = 0; ii < nETFcode; ii++)
	{
		efopcode = m_ETFcode.GetAt(ii);
		
		type = CString(efopcode->type, sizeof(efopcode->type));
		code = CString(efopcode->code, sizeof(efopcode->code));
		hnam = CString(efopcode->hnam, sizeof(efopcode->hnam));
		hnam.TrimLeft(); hnam.TrimRight();
		
		opcd = CString(efopcode->opcd, sizeof(efopcode->opcd));
		etfm = CString(efopcode->etfm, sizeof(efopcode->etfm));
		etfl = CString(efopcode->etfl, sizeof(efopcode->etfl));
		
		if(data.Compare(etfl) == 0)
		{
			AppendItemToList1(row, code, hnam);
			row++;
		}
	}	
	m_list1.SetItemCountEx(m_arList.GetSize());
	m_list1.Invalidate();
}

void CPage1::OnClickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	CPoint      point;   
	UINT        nFlags = 0;   
    
	GetCursorPos(&point);   
	::ScreenToClient(m_tree.m_hWnd, &point);   
	
	HTREEITEM   hItem = m_tree.HitTest(point, &nFlags);   
	HTREEITEM	hChildItem = m_tree.GetChildItem(hItem);	
	CString str = m_tree.GetItemText(hItem);	
	
	
	if((hItem = m_tree.GetSelectedItem()) != nullptr)
	{
		hChildItem = m_tree.GetChildItem(hItem);	
		str = m_tree.GetItemText(hItem);
		
		if(str.Compare("��ü����") == 0)
		{
			ETFInitialList();
			//m_List.Selected(0);
		}
		
		if(hChildItem == nullptr && str.Compare("��ü����") != 0)
		{
			DataGubn(hItem);
			//m_ListCtrl.Selected(0);
			//			ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ETFCompareFunc, 0);
		}
		
	}
	
}

void CPage1::OnClickELWTree(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	CPoint      point;       
	GetCursorPos(&point);   
	::ScreenToClient(m_tree.m_hWnd, &point);   
	
	HTREEITEM hItem;
	
	//     HTREEITEM   hItem = m_tree.HitTest(point, &nFlags);   
	// 	HTREEITEM	hParent = m_tree.GetParentItem(hItem);
	// 	
	// 
	// 	CString str = m_tree.GetItemText(hItem);	
	// 	CString parent = m_tree.GetItemText(hParent);
	
	//	m_list1.UpdateData(FALSE);
	
	// 	if (hItem != nullptr && (nFlags == 4 && TVHT_ONITEMSTATEICON != 0))   
	// 	{   
	// 		if(parent.Compare("������") == 0)
	// 		{
	// 			DataReloadELWAll(str);
	// 			ListView_SortItems(m_list1, ListViewCompareFunc, 0);
	// 		}
	// 		else if(parent.Compare("�����ڻ�����") == 0)			
	// 		{
	// 			DataReloadELWBase(str);
	// 			ListView_SortItems(m_list1, ListViewCompareFunc, 0);
	// 		}
	// 		else if(parent.Compare("�����") == 0)
	// 		{	
	// 			DataReloadELWPB(str);
	// 			ListView_SortItems(m_list1, ListViewCompareFunc, 0);
	// 		}
	// 	}	
	
	if((hItem = m_tree.GetSelectedItem()) != nullptr)
	{
		HTREEITEM hParent = m_tree.GetParentItem(hItem);	
		
		CString str = m_tree.GetItemText(hItem);
		CString parent = m_tree.GetItemText(hParent);
		
		if(parent.Compare("������") == 0)
		{
			DataReloadELWAll(str);
			ListView_SortItems(m_list1, ListViewCompareFunc, 0);
		}
		else if(parent.Compare("�����ڻ�����") == 0)			
		{
			DataReloadELWBase(str);
			ListView_SortItems(m_list1, ListViewCompareFunc, 0);
		}
		else if(parent.Compare("�����") == 0)
		{	
			DataReloadELWPB(str);
			ListView_SortItems(m_list1, ListViewCompareFunc, 0);
		}
		
	}
	//	m_list1.SetFocus();
	//	m_list1.UpdateData(TRUE);
}

void CPage1::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	ClearListitem(); 
	ClearInterest();	
	ClearList2item();
	
	for(int ii = 0; ii < m_ETFtema.GetSize(); ii++)
		delete m_ETFtema.GetAt(ii);
	
	for(int ii =0; ii < m_ETFitem.GetSize(); ii++)
		delete m_ETFitem.GetAt(ii);
	
	for(int ii =0; ii < m_ETFfore.GetSize(); ii++)
		delete m_ETFfore.GetAt(ii);
	
	for(int ii = 0; ii < m_ETFcode.GetSize(); ii++)
		delete m_ETFcode.GetAt(ii);
	
	m_ETFfore.RemoveAll();
	m_ETFcode.RemoveAll();
	m_ETFitem.RemoveAll();
	m_ETFtema.RemoveAll();
	
	m_listdata.RemoveAll();
	m_elwbase.RemoveAll();
	m_ELWdata.RemoveAll();
	m_arrayPBCode.RemoveAll();
	
}

bool CPage1::IsWin8()
{
	bool bReturn = false;
	//2013.01.07 KSJ win8������ ����� 366���� ���ش�.
	OSVERSIONINFOEX osvi;
	osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFOA);
	GetVersionExA((LPOSVERSIONINFO)&osvi);
	
	if(osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
	{
		bReturn = true;
	}
	//KSJ
	return bReturn;
}

void CPage1::SendMsgToPage(int igubn, CString sdata)
{
	switch (igubn)
	{
		case MSG_MTP_CPL_SEARCH_GROUPCODE:
		{
			CString sResult;
			CWnd* wnd = GetParent()->GetParent();
			memset(m_pdata, 0x00, 1024 * 4);
			BOOL ret = wnd->SendMessage(WM_MSG, MAKEWPARAM(MSG_GET_GROUPCODE, atoi(sdata)), (LPARAM)m_pdata);
			if (ret == FALSE)
				return;

			ClearInterest();
			CString fileBook;
			fileBook.Format("%s/%s/%s/bookmark.i%02d", m_root, USRDIR, m_name, atoi(sdata));

			UINT	readL{};
			struct	_inters* pinter{};
			struct  _bookmarkinfo* bInfo{};

			CString stmp;
			CString strseq = CString(m_pdata, 2);
			CString strgname = CString(&m_pdata[2], 20);
			int count = atoi(CString(&m_pdata[22], 4));
			int parseL = 26;

			bool    isFile = true;
			CFile	fileB;
			if (!fileB.Open(fileBook, CFile::modeRead))
				isFile = false;		//�ϸ�ũ ���� �������

			struct _jinfo* jinfo{};
			for (int ii = 0; ii < count; ii++)
			{
				pinter = (_inters*) new char[sz_inters];
				ZeroMemory(pinter, sz_inters);

				jinfo = (struct _jinfo*)&m_pdata[parseL];

				memcpy(pinter->gubn, jinfo->gubn, sizeof(jinfo->gubn));		// ���񱸺�	0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
				memcpy(pinter->code, jinfo->code, sizeof(jinfo->code));			// �����ڵ�[12]
				memcpy(pinter->xprc, jinfo->xprc, sizeof(jinfo->xprc));				// �����ܰ�[10]
				memcpy(pinter->xnum, jinfo->xnum, sizeof(jinfo->xnum));  	     // ��������[10]

				if (isFile == true)
				{
					bInfo = (_bookmarkinfo*) new char[sizeof(_bookmarkinfo)];
					ZeroMemory(bInfo, sizeof(_bookmarkinfo));

					readL = fileB.Read(bInfo, sizeof(_bookmarkinfo));
					if (readL < sizeof(_bookmarkinfo))
					{
						delete bInfo;
					}
					else
					{
						CString temp = CString((pinter->code));
						temp = temp.Left(12);
						temp.TrimRight();
					//	CString temp2 = CString(bInfo->code  //test 20230203
						CString temp2 = CString(bInfo->code, 12).TrimRight();
						temp2.TrimRight();

						if (strcmp(temp, temp2) == 0)
						{
							if (pinter->code[0] == 'm')
							{
								//temp.Format("%.32s", bInfo->name);
								//temp.TrimRight();
								char* ptmp = new char[sizeof(bInfo->name) + 1];
								memset(ptmp, 0x00, sizeof(bInfo->name) + 1);
								memcpy(ptmp, bInfo->name, sizeof(bInfo->name));
								temp.Format("%s", ptmp);
								temp.TrimRight();
								CopyMemory(pinter->name, (LPSTR)(LPCTSTR)temp, temp.GetLength());
							}
							pinter->bookmark[0] = bInfo->bookmark[0] == '1' ? '1' : '0';//2015.04.03 KSJ 1�̾ƴϸ� 0���� ���ش�.
						}
					}
				}  //�ϸ�ũ

				pinter->name[31] = 0;	//2016.01.25 KSJ 225050 �������� 33byte�̹Ƿ� �ΰ��� ���� �ʾ� �����߻��Ѵ�.
				m_inters.Add(pinter);

				parseL += sizeof(struct _jinfo);
			}  //for

			m_list2.DeleteAllItems();
			ClearList2item();

			CString tcode(_T(""));
			int nItem{};
		
			for (int ii = 0; ii < count; ii++)
			{
				pinter = m_inters.GetAt(ii);

				if (pinter->code[0] == 'm')
				{
					nItem = AddItem(&m_list2, "", ii);

					if (strlen(pinter->code))
					{
						AddItem(&m_list2, pinter->name, nItem, 1);
					}

				}
				else
				{
					tcode = CString(pinter->code, sizeof(pinter->code));
					tcode = tcode.Left(12);
					tcode.TrimLeft();
					tcode.TrimRight();
			
					nItem = AddItem(&m_list2, tcode, ii);
					CString name;
					name = GetCodeName(pinter->code);
					name.TrimLeft(); name.TrimRight();

					AddItem(&m_list2, name, nItem, 1);
				}

				_listitem* listitem = new _listitem();
				listitem->pInter = pinter;
				strcpy(listitem->code, pinter->code);
				strcpy(listitem->name, pinter->name);

				listitem->name[31] = 0;	//2016.01.25 KSJ 225050 �������� 33byte�̹Ƿ� �ΰ��� ���� �ʾ� �����߻��Ѵ�.

				m_list2item.Add(listitem);
				m_list2.SetItemData(nItem, (LPARAM)listitem);
				// END MODIFY
			}

			updateList2sum();
		}
		break;  //MSG_MTP_CPL_SEARCH_GROUPCODE


	}
}


//if (kind == 1)
	//{
	//	append = true;
	//}
	//else if (kind < 5)    //������ ����
	//{
	//	if ((int)hjcode.size == kind)
	//		append = true;
	//}
	//else if (kind < 27)  //test  ���ķ�� ����
	//{
	//	if ((int)hjcode.ucds == kind || (int)hjcode.ucdm == kind)  //test
	//		append = true;
	//}
	//else if (kind == 27)  //����������
	//{
	//	if ((int)hjcode.jjug == kind)
	//		append = true;
	//}
	//else if (kind < 41)  //�����ں��� ���߼�
	//{
	//	if ((int)hjcode.jsiz == kind)
	//		append = true;
	//}
	//else if (kind == 41)
	//{
	//	if ((int)hjcode.jsiz >= kind + 38 && (int)hjcode.jsiz <= kind + 41)
	//		append = true;
	//}
	//else if (kind == 42)  //�Ϲ����豸��������
	//{
	//	if ((int)hjcode.wsgb == 1)
	//		append = true;
	//}

//switch (kind)
//{
//case 101: // KOSPI200
//	if ((int)hjcode.kpgb == 3 || (int)hjcode.kpgb == 2 || (int)hjcode.kpgb == 1)
//		append = true;
//	break;
//case 401: // KOSPI100
//	if ((int)hjcode.kpgb == 3 || (int)hjcode.kpgb == 2)
//		append = true;
//	break;
//case 402: // KOSPI50
//	if ((int)hjcode.kpgb == 3)
//		append = true;
//	break;
//case 403: // KOSPI IT
//	if ((int)hjcode.itgb == 1)
//		append = true;
//	break;
//}


//if (sData == "�ڽ���")
//{
//	if (stemp.Find("QGG01P"))
//		append = true;
//}
//else if (sData == "�ڽ��� ������")
//{
//	if (stemp.Find("QGZ01P"))
//		append = true;
//}
//else if (sData == "�ڽ��� ������")
//{
//	if (stemp.Find("QGZ02P"))
//		append = true;
//}
//else if (sData == "�ڽ��� ������")
//{
//	if (stemp.Find("QGZ03P"))
//		append = true;
//}
//else if (sData == "�߼ұ������")
//{
//	if (stemp.Find("QGS35P"))
//		append = true;
//}
//else if (sData == "����")
//{
//	if (stemp.Find("QGS36P"))
//		append = true;
//}
//else if (sData == "���뼭��")
//{
//	if (stemp.Find("QGS37P"))
//		append = true;
//}
//else if (sData == "�Ǽ�")
//{
//	if (stemp.Find("QGS38P"))
//		append = true;
//}
//else if (sData == "����")
//{
//	if (stemp.Find("QGS39P"))
//		append = true;
//}
//else if (sData == "��Ÿ����")
//{
//	if (stemp.Find("QGS01P"))
//		append = true;
//}
//else if (sData == "�ڽ��� IT")
//{
//	if (stemp.Find("QGS32P"))
//		append = true;
//}
//else if (sData == "��������ȭ")
//{
//	if (stemp.Find("QGS42P"))
//		append = true;
//}
//else if (sData == "����")
//{
//	if (stemp.Find("QGS02P"))
//		append = true;
//}
//else if (sData == "�Ǽ�")
//{
//	if (stemp.Find("QGS03P"))
//		append = true;
//}
//else if (sData == "����")
//{
//	if (stemp.Find("QGS04P"))
//		append = true;
//}
//else if (sData == "���")
//{
//	if (stemp.Find("QGS31P"))
//		append = true;
//}
//else if (sData == "����")
//{
//	if (stemp.Find("QGS05P"))
//		append = true;
//}
//else if (sData == "�������")
//{
//	if (stemp.Find("QGS49P"))
//		append = true;
//}
//else if (sData == "��������ȭ")
//{
//	if (stemp.Find("QGS34P"))
//		append = true;
//}
//else if (sData == "��Ź�ۼ���")
//{
//	if (stemp.Find("QGS06P"))
//		append = true;
//}
//else if (sData == "IT S/W & SVC")
//{
//	if (stemp.Find("QGS07P"))
//		append = true;
//}
//else if (sData == "IT H / W")
//{
//	if (stemp.Find("QGS08P"))
//		append = true;
//}
//else if (sData == "���ķᡤ���")
//{
//	if (stemp.Find("QGS09P"))
//		append = true;
//}
//else if (sData == "�������Ƿ�")
//{
//	if (stemp.Find("QGS10P"))
//		append = true;
//}
//else if (sData == "���̡�����")
//{
//	if (stemp.Find("QGS11P"))
//		append = true;
//}
//else if (sData == "���ǡ���ü����")
//{
//	if (stemp.Find("QGS12"))
//		append = true;
//}
//else if (sData == "ȭ��")
//{
//	if (stemp.Find("QGS13P"))
//		append = true;
//}
//else if (sData == "����")
//{
//	if (stemp.Find("QGS33P"))
//		append = true;
//}
//else if (sData == "��ݼ�")
//{
//	if (stemp.Find("QGS14P"))
//		append = true;
//}
//else if (sData == "�ݼ�")
//{
//	if (stemp.Find("QGS15P"))
//		append = true;
//}
//else if (sData == "��衤���")
//{
//	if (stemp.Find("QGS16P"))
//		append = true;
//}
//else if (sData == "�Ϲ���������")
//{
//	if (stemp.Find("QGS17P"))
//		append = true;
//}
//else if (sData == "�Ƿᡤ���б��")
//{
//	if (stemp.Find("QGS18P"))
//		append = true;
//}
//else if (sData == "�����񡤺�ǰ")
//{
//	if (stemp.Find("QGS19P"))
//		append = true;
//}
//else if (sData == "��Ÿ ����")
//{
//	if (stemp.Find("QGS20P"))
//		append = true;
//}
//else if (sData == "���հǼ�")
//{
//	if (stemp.Find("QGS65P"))
//		append = true;
//}
//else if (sData == "�����Ǽ�")
//{
//	if (stemp.Find("QGS66P"))
//		append = true;
//}
//else if (sData == "����")
//{
//	if (stemp.Find("QGS68P"))
//		append = true;
//}
//else if (sData == "����")
//{
//	if (stemp.Find("QGS75P"))
//		append = true;
//}
//else if (sData == "�������")
//{
//	if (stemp.Find("QGS81P"))
//		append = true;
//}
//else if (sData == "��������ȭ")
//{
//	if (stemp.Find("QGS87P"))
//		append = true;
//}
//else if (sData == "��ż���")
//{
//	if (stemp.Find("QGS21P"))
//		append = true;
//}
//else if (sData == "��ۼ���")
//{
//	if (stemp.Find("QGS22P"))
//		append = true;
//}
//else if (sData == "���ͳ�")
//{
//	if (stemp.Find("QGS23P"))
//		append = true;
//}
//else if (sData == "������������ ")
//{
//	if (stemp.Find("QGS24P"))
//		append = true;
//}
//else if (sData == "����Ʈ����")
//{
//	if (stemp.Find("QGS25P"))
//		append = true;
//}
//else if (sData == "��ǻ�ͼ���")
//{
//	if (stemp.Find("QGS26P"))
//		append = true;
//}
//else if (sData == "������")
//{
//	if (stemp.Find("QGS27P"))
//		append = true;
//}
//else if (sData == "�������")
//{
//}
//else if (sData == "�ݵ�ü")
//{
//}
//else if (sData == "IT��ǰ")
//{
//}
//else if (sData == "�ڽ��� �췮�����")
//{
//}
//else if (sData == "�ڽ��� ��ó�����")
//{
//}
//else if (sData == "�ڽ��� �߰߱����")
//{
//}
//else if (sData == "�ڽ��� �����������")
//{
//}


//switch (kind)
//{
//case 1:    //�ڽ���
//	append = true; break;
//case 2:    //�ڽ��� ������
//	if (stmp.Find("KGZ01P") >= 0) append = true; break;
//case 3:    //�ڽ��� ������
//	if (stmp.Find("KGZ02P") >= 0) append = true; break;
//case 4:    //�ڽ��� ������
//	if (stmp.Find("KGZ03P") >= 0) append = true; break;
//case 5:    //���ķ�ǰ
//	if (stmp.Find("KGS01P") >= 0) append = true; break;
//case 6:    //����,�Ǻ� 
//	if (stmp.Find("KGS02P") >= 0) append = true; break;
//case 7:    //����,����
//	if (stmp.Find("KGS03P") >= 0) append = true; break;
//case 8:    //ȭ��
//	if (stmp.Find("KGS04P") >= 0) append = true; break;
//case 9:    //�Ǿ�ǰ
//	if (stmp.Find("KGS05P") >= 0) append = true; break;
//case 10:    //��ݼӱ���
//	if (stmp.Find("KGS06P") >= 0) append = true; break;
//case 11:    //ö���ױݼ� 
//	if (stmp.Find("KGS07P") >= 0) append = true; break;
//case 12:    //��� 
//	if (stmp.Find("KGS08P") >= 0) append = true; break;
//case 13:    //����,����
//	if (stmp.Find("KGS09P") >= 0) append = true; break;
//case 14:    //�Ƿ�����  
//	if (stmp.Find("KGS19P") >= 0) append = true; break;
//case 15:    //������
//	if (stmp.Find("KGS10P") >= 0) append = true; break;
//case 16:    //�����  
//	if (stmp.Find("KGS11P") >= 0) append = true; break;
//case 17:    //���Ⱑ����  
//	if (stmp.Find("KGS20P") >= 0) append = true; break;
//case 18:    //�Ǽ���
//	if (stmp.Find("KGS12P") >= 0) append = true; break;
//case 19:    //���â��  
//	if (stmp.Find("KGS13P") >= 0) append = true; break;
//case 20:    //��ž�  
//	if (stmp.Find("KGS21P") >= 0) append = true; break;
//case 21:    //������  
//	if (stmp.Find("KGS14P") >= 0) append = true; break;
//case 22:    //����  
//	if (stmp.Find("KGS15P") >= 0) append = true; break;
//case 24:    //����  
//	if (stmp.Find("KGS16P") >= 0) append = true; break;
//case 25:    //����  
//	if (stmp.Find("KGS17P") >= 0) append = true; break;
//case 26:    //���񽺾�  
//	if (stmp.Find(" KGS22P") >= 0) append = true; break;
//}

void CPage1::OnGetdispinfoList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	LV_ITEM* pItem = &(pDispInfo)->item;
	CITEMS rLabel = m_arList.ElementAt(pItem->iItem);

	if (pItem->mask & LVIF_TEXT) //valid text buffer?
	{
		switch (pItem->iSubItem)
		{
		case 0:
			lstrcpy(pItem->pszText, rLabel.code);
			break;
		case 1:
			lstrcpy(pItem->pszText, rLabel.name);
			break;
		}
	}
	*pResult = 0;
}
