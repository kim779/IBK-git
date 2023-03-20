// JongmukDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AxisCode.h"
#include "JongmukDlg.h"

#include "../../h/axisvar.h"
#include "../../h/axisfire.h"
#include "../../h/interSt.h"
#include <imm.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UPCODE_KOSPI	"0"
#define UPCODE_KOSDAQ	"2"

#define KPI_TOTAL	_T("001")
#define KPI_TOTAL_STR	_T("��       ��")
#define KDQ_TOTAL	_T("201")
#define KDQ_TOTAL_STR	_T("����")
#define KPI200		_T("101")
#define CODE_DOW	_T(".DJI")
#define CODE_NAS	_T("COMP")
#define STR_KOSPI	_T("(��)")
#define STR_KOSDAQ	_T("(��)")

#define EXKOSPI		0
#define EXKOSDAQ	1

#define	COLUMN1_WIDTH	132
#define	LISTCTRL_WIDTH	314
#define LISTCTRL_XPOS	135
#define GAPPLUS		32

#define	HANGULMSK	0x80
#define	ChosungS	0xA4A1	// ��
#define	ChosungE	0xA4BE	// ��

#define	SK_JONGMOK	0	// ����˻�
#define	SK_WORD		1	// �ܾ�˻�

using namespace AxStd;

enum tree_class{
	tree_jjm = 0,
	tree_kspjjm,
	tree_ksdjjm,
	tree_kspup,
	tree_ksdup,
	tree_ksp200,
	tree_ksd50,
	tree_krx100,
	tree_frbd,
	tree_etf,
	tree_etf_item,
	tree_etf_tema,
	tree_etf_fore,
	tree_elw,
	tree_elw_base,
	tree_for,
	tree_ks,
	tree_tm,
	tree_group,
	tree_reits,
	tree_elw_gencode,
	tree_elw_jogicode,
	tree_konex,		//2013.06.05 KSJ �ڳؽ� �߰�
	tree_etn,		//2014.10.28 KSJ ETN�߰�
	tree_etfetn,		//2014.10.28 KSJ etf/ETN �������߰�
	tree_spac		//2015.11.04 KSJ spac �߰�
};



/////////////////////////////////////////////////////////////////////////////
// CJongmukDlg dialog

CJongmukDlg::CJongmukDlg(int kind, int type, CPoint pt, CWnd* pParent /*=nullptr*/)
	: CDialog(CJongmukDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJongmukDlg)
	//}}AFX_DATA_INIT
	m_pApp = (CAxisCodeApp *)AfxGetApp();

	auto future =  std::async([&]() {
		for (auto mt : m_pApp->_mapCODEx)
			_mapCODE.emplace(mt);
	});

	const std::chrono::milliseconds wtime(100);
	while (future.wait_for(wtime) != std::future_status::ready)
		AxStd::_Msg("Thread runing");

	m_fcB = m_ocB = nullptr;
	m_arUPCODE.RemoveAll();
	m_select = 0;
	m_ewFilter = ewALL;
	m_szCode = "";
	m_szName = "";
	m_kind   = kind;
	m_Type   = type;

	m_gListsort = FALSE;
	m_point = pt;

	m_nRadioOption = 0;
	m_bOption = FALSE;

	m_root = ((CAxisCodeApp*)AfxGetApp())->m_root;
	m_user = ((CAxisCodeApp*)AfxGetApp())->m_folder;

	m_arrayELWCode.RemoveAll();
	m_arrayELWGenCode.RemoveAll();
	m_arrayELWJogiCode.RemoveAll();
	m_arrayPBCode.RemoveAll();
	m_bAddAll = true;
}

void CJongmukDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJongmukDlg)
	DDX_Control(pDX, IDC_BUTTON_KONEXWORK, m_ctlButtonKONEXWORK);
	DDX_Control(pDX, IDC_BUTTON_PREMIER, m_ctlBtnPremier);
	DDX_Control(pDX, IDC_STATIC_SEARCH, m_ComboSearchKind);
	DDX_Control(pDX, IDC_BUTTON_22, m_BtnHangul22);
	DDX_Control(pDX, IDC_BUTTON_21, m_BtnHangul21);
	DDX_Control(pDX, IDC_BUTTON_20, m_BtnHangul20);
	DDX_Control(pDX, IDC_BUTTON_19, m_BtnHangul19);
	DDX_Control(pDX, IDC_BUTTON_18, m_BtnHangul18);
	DDX_Control(pDX, IDC_BUTTON_17, m_BtnHangul17);
	DDX_Control(pDX, IDC_BUTTON_9, m_BtnHangul9);
	DDX_Control(pDX, IDC_BUTTON_8, m_BtnHangul8);
	DDX_Control(pDX, IDC_BUTTON_7, m_BtnHangul7);
	DDX_Control(pDX, IDC_BUTTON_6, m_BtnHangul6);
	DDX_Control(pDX, IDC_BUTTON_5, m_BtnHangul5);
	DDX_Control(pDX, IDC_BUTTON_4, m_BtnHangul4);
	DDX_Control(pDX, IDC_BUTTON_3, m_BtnHangul3);
	DDX_Control(pDX, IDC_BUTTON_2, m_BtnHangul2);
	DDX_Control(pDX, IDC_BUTTON_16, m_BtnHangul16);
	DDX_Control(pDX, IDC_BUTTON_15, m_BtnHangul15);
	DDX_Control(pDX, IDC_BUTTON_14, m_BtnHangul14);
	DDX_Control(pDX, IDC_BUTTON_13, m_BtnHangul13);
	DDX_Control(pDX, IDC_BUTTON_12, m_BtnHangul12);
	DDX_Control(pDX, IDC_BUTTON_11, m_BtnHangul11);
	DDX_Control(pDX, IDC_BUTTON_10, m_BtnHangul10);
	DDX_Control(pDX, IDC_BUTTON_1, m_BtnHangul1);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_EditSearch);
	DDX_Control(pDX, IDC_BUTTON_THIRDJS, m_CtlButtonThirdJS);
	DDX_Control(pDX, IDC_BUTTON_SECTOR, m_CtlButtonSector);
	DDX_Control(pDX, IDC_TREE_BUNRYU, m_Tree);
	DDX_Control(pDX, IDC_BUTTON_ELW, m_ctlButtonELW);
	DDX_Control(pDX, IDC_BUTTON_SINJU, m_ctlButtonSINJU);
	DDX_Control(pDX, IDC_LIST_PUT, m_ListPut);
	DDX_Control(pDX, IDC_LIST_CALL, m_ListCall);
	DDX_Control(pDX, IDC_BUTTON_FUTURE4, m_CtlButFuture4);
	DDX_Control(pDX, IDC_BUTTON_FUTURE3, m_CtlButFuture3);
	DDX_Control(pDX, IDC_BUTTON_FUTURE2, m_CtlButFuture2);
	DDX_Control(pDX, IDC_BUTTON_FUTURE1, m_CtlButFuture1);
	DDX_Control(pDX, IDC_LIST_CTRL, m_ListCtrl);
	DDX_Control(pDX, IDC_BUTTON_THIRDMARKET, m_ctlButtonTHIRD);
	DDX_Control(pDX, IDC_BUTTON_KOSPIWORK, m_ctlButtonKOSPIWORK);
	DDX_Control(pDX, IDC_BUTTON_KOSPICODE, m_ctlButtonKOSPI);
	DDX_Control(pDX, IDC_BUTTON_KOSDAQWORK, m_ctlButtonKOSDAQWORK);
	DDX_Control(pDX, IDC_BUTTON_KOSDAQCODE, m_ctlButtonKOSDAQ);
	DDX_Control(pDX, IDC_BUTTON_ECNCODE, m_ctlButtonECN);
	DDX_Control(pDX, IDC_BUTTON_ALLCODE, m_ctlButtonAll);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_ComboGroup);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CJongmukDlg, CDialog)
	//{{AFX_MSG_MAP(CJongmukDlg)
	ON_WM_DESTROY()
//	ON_BN_CLICKED(IDC_BUTTON_ALLCODE, OnButtonAllcode)
//	ON_BN_CLICKED(IDC_BUTTON_KOSPICODE, OnButtonKospicode)
//	ON_BN_CLICKED(IDC_BUTTON_KOSDAQCODE, OnButtonKosdaqcode)
//	ON_BN_CLICKED(IDC_BUTTON_ECNCODE, OnButtonEcncode)
//	ON_BN_CLICKED(IDC_BUTTON_THIRDMARKET, OnButtonThirdmarket)
	ON_BN_CLICKED(IDC_BUTTON_KOSPIWORK, OnButtonKospiwork)
	ON_BN_CLICKED(IDC_BUTTON_KOSDAQWORK, OnButtonKosdaqwork)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_CLICK, IDC_LIST_CTRL, OnClickListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CTRL, OnDblclkListCtrl)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_FUTURE1, OnButtonFuture1)
	ON_BN_CLICKED(IDC_BUTTON_FUTURE2, OnButtonFuture2)
	ON_BN_CLICKED(IDC_BUTTON_FUTURE3, OnButtonFuture3)
	ON_BN_CLICKED(IDC_BUTTON_FUTURE4, OnButtonFuture4)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_FUTURE1, OnDoubleclickedButtonFuture1)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_FUTURE2, OnDoubleclickedButtonFuture2)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_FUTURE3, OnDoubleclickedButtonFuture3)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_FUTURE4, OnDoubleclickedButtonFuture4)
	ON_LBN_DBLCLK(IDC_LIST_CALL, OnDblclkListCall)
	ON_LBN_DBLCLK(IDC_LIST_PUT, OnDblclkListPut)
//	ON_BN_CLICKED(IDC_BUTTON_SINJU, OnButtonSinju)
//	ON_BN_CLICKED(IDC_BUTTON_ELW, OnButtonElw)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_BUNRYU, OnDblclkTreeBunryu)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_BUNRYU, OnSelchangedTreeBunryu)
	ON_BN_CLICKED(IDC_BUTTON_SECTOR, OnButtonSector)
	ON_BN_CLICKED(IDC_BUTTON_THIRDJS, OnButtonThirdjs)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP, OnSelchangeComboGroup)
	ON_CBN_SELCHANGE(IDC_STATIC_SEARCH, OnSelchangeStaticSearch)
	ON_BN_CLICKED(IDC_BUTTON_PREMIER, OnButtonPremier)
	ON_BN_CLICKED(IDC_BUTTON_KONEXWORK, OnButtonKonexwork)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_CTRL, OnGetdispinfoListCtrl)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_RADIO1, IDC_RADIO4, OnRadio)
	ON_COMMAND_RANGE(IDC_BUTTON_1, IDC_BUTTON_22, OnClickBtnSort)
	ON_MESSAGE(WM_EDITX, OnEditXMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJongmukDlg message handlers

BOOL CJongmukDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	const CRect rect(100, 40, 274, 292);

	if (_mapCODE.size() <= 0)
		MessageBox("������ ����ġ �ʽ��ϴ�.");

	SetDlgPos(m_point.x, m_point.y);

	m_Font.CreateFont(12, "����ü");
	m_ListCtrl.SetFont(&m_Font);

	m_ListCtrl.SetBkColor(COR_WHITE);
	m_ListCtrl.SetTextBkColor(COR_WHITE);
	m_ListCtrl.SetTextColor(RGB(0,0,0));
	m_ListCtrl.InsertColumn(1, "�ڵ�", LVCFMT_LEFT, 60);
	m_ListCtrl.InsertColumn(2, "�����", LVCFMT_LEFT, COLUMN1_WIDTH);

	m_Tree.ModifyStyle(0,TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_DISABLEDRAGDROP |TVS_SHOWSELALWAYS);
	m_Tree.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	LoadELWCode();
	baseCX_Category();
	BtnSetup();
	Part();

	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	Set_KS_HanMode();

	m_ComboSearchKind.SetCurSel(0);

	if (m_kind == reitsCODE)
		ProcPart(tree_reits, 0);

	if (m_kind == kospiCODE && m_Type == 111 )
		SelectETF();


	m_gListsort = false;
	return TRUE;
}

void CJongmukDlg::GetExCode()
{
	m_exCode[EXKOSPI].RemoveAll();
	m_exCode[EXKOSDAQ].RemoveAll();

	const struct _infop {
		char* code;
		char* name;
	} infop[] = {
		{"101", "KOSPI200"},	{"401", "KOSPI100"},
		{"402", "KOSPI50"},	{"403", "KOSPI IT"}
	};

	const struct _infod {
		char* code;
		char* name;
	} infod[] = {
		{"301", "KOSDAQ50"},	{"302", "KOSDAQ IT"}
		/*{"303", "KOSTAR"}*/	//2016.05.16 KSJ KOSTAR ���� ����
	};

	_exCode	exCode;

	for (int ii = 0 ; ii < sizeof(infop) / sizeof(_infop) ; ii++)
	{
		exCode.code = infop[ii].code;
		exCode.name = infop[ii].name;

		m_exCode[EXKOSPI].Add(exCode);
	}

	for (int jj = 0 ; jj < sizeof(infod) / sizeof(_infod) ; jj++)
	{
		exCode.code = infod[jj].code;
		exCode.name = infod[jj].name;

		m_exCode[EXKOSDAQ].Add(exCode);
	}
}

void CJongmukDlg::SetDlgPos(int x, int y)
{
	if (x < 0 || y < 0)
		return;

	int	scWidth{}, scHeight{};
	scWidth	= GetSystemMetrics(SM_CXFULLSCREEN);
	scHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	CAxisCodeApp* pApp = (CAxisCodeApp *)AfxGetApp();
	const int virScrX = pApp->GetVirtualScreenX();
	if (virScrX > 0)
		scWidth = virScrX;

	CRect	rc;
	GetClientRect(&rc);

	const int	gabX = scWidth - (x + rc.Width());
	const int	gabY = scHeight - (y + rc.Height());
	if (gabX < 0)
		x += gabX - 7;
	if (gabY < 0)
		y += gabY - 7;

	SetWindowPos(nullptr, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
}

void CJongmukDlg::OnDestroy()
{
	if (m_ListCtrl.GetItemCount() < 0)
	{
		m_szCode = "";
		m_szName = "";
	}

	ListRemoveAll();
	DeleteAllData();

	CDialog::OnDestroy();

	DeleteObject(m_Font);
	DeleteObject(m_hBrush);
}

void CJongmukDlg::OnRadio(UINT nID)
{
	switch (nID)
	{
	case IDC_RADIO1:
		m_nRadioOption = 0;
		break;
	case IDC_RADIO2:
		m_nRadioOption = 1;
		break;
	case IDC_RADIO3:
		m_nRadioOption = 2;
		break;
	case IDC_RADIO4:
		m_nRadioOption = 3;
		break;
	}

	Filllistoption(m_nRadioOption);
}

BOOL CJongmukDlg::LoadUSCode()
{
	if (m_arUSCODE.size() > gsl::narrow_cast<size_t>(0))
		return TRUE;

	TRY
	{
		CString		path = m_root + "/tab/" + USCODE_FILE;
		CFile		file(path, CFile::modeRead);
		UINT		nBytesRead;
		do
		{
			auto pUSCode = std::make_shared<_USCode>();
			nBytesRead = file.Read(pUSCode.get(), sizeof(_USCode));
			if (nBytesRead == sizeof(_USCode))
				m_arUSCODE.emplace_back(pUSCode);
		}while((int)nBytesRead);
		file.Close();
	}CATCH(CFileException, e)
	{
		#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
		#endif
	}
	END_CATCH

	return TRUE;
}

BOOL CJongmukDlg::LoadELWCode()
{
	int	codeN{};

	CFile	file;
	struct  elwcode  ELWCode{};
	struct _elwcode	savecode;
	PBCODE pbcode;
	CString path;

	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\elwcod.dat";

	if (m_arrayELWCode.GetSize() > 0)				
		return FALSE;
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))	
		return FALSE;


	codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct elwcode));
//	SORTSTR *pMonthSortArr = new SORTSTR[codeN];
//	SORTSTR *pBaseSortArr  = new SORTSTR[codeN];

	std::vector<BASECODE> vBase;
//	vBase.resize(codeN);


	CJongmukDlg dlg(codeNAME, 0);
	CString sBuffer;


	const int tsize = gsl::narrow_cast<int>(file.GetLength());
	const int rsize = gsl::narrow_cast<int>(file.Read(sBuffer.GetBuffer(tsize + 1), tsize));
	sBuffer.ReleaseBuffer();
	file.Close();

	int idx{};
	bool bFound{};
//	int nBase = 0;
	CString sMonth;
	if (tsize == rsize)
	{
		gsl::span<struct elwcode> spanelw((struct elwcode*)sBuffer.GetString(), codeN);
		
		for (const auto& ELWCode : spanelw)
		{
			savecode.code  = CString(ELWCode.codx, ELWCodeLen);
			savecode.name  = CString(ELWCode.hnam, ELWHNameLen).Mid(1).Trim();
			savecode.ktype = CString(ELWCode.krye, 2);
			savecode.htype = CString(ELWCode.khbe, 2);
			savecode.pcode = CString(ELWCode.bgce, 12);
			savecode.pname = CString(ELWCode.hbge, 20);
			savecode.mjmt  = CString(ELWCode.mjmt, 8);
			savecode.bkoba = (ELWCode.koba[0] == 2) ? TRUE : FALSE;

			for (int jj = 0; jj < 5; jj++)
				savecode.lpnum[jj] = CString((char*)ELWCode.lphn[jj], 3);
			for (int jj = 0; jj < 5; jj++)
				savecode.bcode[jj] = CString((char*)ELWCode.gcjs[jj], 12).Trim();;

			idx = FindPBCode(savecode.pcode);
			if (idx < 0)
			{
				pbcode.code = savecode.pcode;
				pbcode.name = savecode.pname;
				m_arrayPBCode.Add(pbcode);
			}

			for (int jj = 0; jj < 5; jj++)
			{
				CString bcode;
				bcode.Format("%s", savecode.bcode[jj]); bcode.TrimRight();
				if (bcode == "") break;

				bFound = false;	
				for (auto item : vBase)
				{
					if (bcode.CompareNoCase(item.code) == 0)
					{
						bFound = true;
						break;
					}
				}

				if (!bFound)
				{
					auto& base = vBase.emplace_back(_basecode(bcode, ""));

					if (const auto ft = _mapCODE.find(bcode.Mid(1)); ft != _mapCODE.end())
						base.name = CString(ft->second->hnam, HNameLen).Trim();
					else if (bcode.CompareNoCase("KOSPI200") == 0)
						continue;
					else if (bcode.CompareNoCase("BASKET") == 0)
						break;
					else
						continue;
					
					//nBase++;
				}
			}

			/*	
			        bFound = false;

				for (int jj = 0; jj < nMonth; jj++)
				{
					if (sMonth.CompareNoCase(pMonthSortArr[jj].code) == 0)
					{
						bFound = true;
						break;
				         }
			        }
			}*/


			if (savecode.bkoba == 0)
			{
				m_arrayELWGenCode.Add(savecode);
			}
			else
			{
				m_arrayELWJogiCode.Add(savecode);
			}

			m_arrayELWCode.Add(savecode);
		}
	}


	std::sort(vBase.begin(), vBase.end(), [](const auto item1, const auto item2){
		return item1.name.Compare(item2.name) < 0;
	});

//	BASECODE basecode;
//	qsort((void*)pBaseSortArr, nBase, sizeof(struct _sortStr), compareSortName2);

	//for (int ii = 0; ii < nBase; ii++)
	//{
	//	basecode.code = pBaseSortArr[ii].code;
	//	basecode.name = pBaseSortArr[ii].name;

	//	m_arrayBASECode.Add(basecode);
	//}
	//delete[] pBaseSortArr;

	
	return TRUE;
}

int CJongmukDlg::FindPBCode(CString code)
{
	const bool bRegistered = false;
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


void CJongmukDlg::SelectETF()
{
	HTREEITEM item = m_Tree.GetFirstVisibleItem();
	while( item )
	{
		item = m_Tree.GetNextItem( item, TVGN_NEXT );
		CString strItem = m_Tree.GetItemText( item );
		if( strItem == "ETF")
		{
			//m_Tree.SetItemState(item,TVIS_SELECTED,TVIS_SELECTED);
			m_Tree.SelectItem( item );
			break;
		}
	}
}

hjcodex* CJongmukDlg::FindOnlyELW(CString sCode)
{
	const auto ft = _mapCODE.find(sCode);
	if (ft != _mapCODE.end() && ft->second->ssgb == jmELW)
		return ft->second;
	return nullptr;
}

void CJongmukDlg::ApplyELW(CString code)
{
	m_gListsort = FALSE;
	clearEditSearch();

	m_select = OTHERTYPE;
	m_ComboGroup.ShowWindow(SW_HIDE);

	MoveGrid(0);
	HideButton();

	struct hjcodex	*hjc = nullptr;
	std::vector<HCODEX*> vData;

	const int ncnt = m_arrayELWCode.GetSize();
	CString pbcode;

	for(int ii = 0; ii < ncnt; ii++)
	{
		_elwcode elwcode = m_arrayELWCode.GetAt(ii);
		pbcode =elwcode.pcode;
		pbcode.Trim();

		if(code.Compare(pbcode) == 0)
		{
			hjc = FindOnlyELW(elwcode.code);
			if(hjc)	vData.emplace_back(hjc);
		}
	}

	CodeListSome(vData);

	m_ListCtrl.Selected(0);
	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);
	m_szName.TrimLeft();
}

void CJongmukDlg::ApplyELWBase(CString code)
{
	m_gListsort = FALSE;
	clearEditSearch();

	m_select = OTHERTYPE;
	m_ComboGroup.ShowWindow(SW_HIDE);

	MoveGrid(0);
	HideButton();

	struct hjcodex* hjc{};
	std::vector<HCODEX*> vData;
	const int ncnt = m_arrayELWCode.GetSize();
	CString basecode;

	for(int ii = 0; ii < ncnt; ii++)
	{
		_elwcode elwcode = m_arrayELWCode.GetAt(ii);
		basecode =elwcode.bcode[0];
		if(code.Compare(basecode) == 0)
		{
			hjc = FindOnlyELW(elwcode.code);
			if(hjc)	vData.emplace_back(hjc);
		}
	}

	CodeListSome(vData);

	m_ListCtrl.Selected(0);
	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);
	m_szName.TrimLeft();
}

hjcodex* CJongmukDlg::FindHJCode(CString sCode)
{
	const CAxisCodeApp* pApp = (CAxisCodeApp *)AfxGetApp();

	const auto ft = _mapCODE.find(sCode);
	if (ft == _mapCODE.end())
		return nullptr;

	return ft->second;
}


BOOL CJongmukDlg::CodeListAll()
{
	if (_mapCODE.size() <= 0)
		return FALSE;

	const char* combodata = "������%��  ��%������%������%������%���ķ��%����, �Ǻ�%����, ����%ȭ  ��%�Ǿ�ǰ%��ݼӱ���%ö��, �ݼ�%��  ��%����, ����%�Ƿ�����%������%�����%���Ⱑ����%�Ǽ���%���â��%��ž�%������%��  ��%���� ������%��  ��%��  ��%���񽺾�%������%KOSPI 200%KOSPI 100%KOSPI 50%KOSPI IT%�ڽ��� ���� %�ڽ��� ��ó%�ڽ��� ��Ÿ����%�ڽ��� IT����%�ڽ��� IT��ó%�ڽ��� ����%�ڽ��� �Ǽ�%�ڽ��� ����%�ڽ��� ����, ����%�ڽ��� ���%�ڽ��� ����%�ڽ��� ��Ź��%�ڽ��� IT S/W%�ڽ��� IT H/W%�ڽ��� ���ķ�, ���%�ڽ��� ����, �Ƿ�%�ڽ��� ����, ����%�ڽ��� ����, ��ü%�ڽ��� ȭ��%�ڽ��� ����%�ڽ��� ��ݼ�%�ڽ��� �ݼ�%�ڽ��� ������%�ڽ��� ��������%�ڽ��� �Ƿ�, ����%�ڽ��� ���, ��ǰ%�ڽ��� ��Ÿ ����%�ڽ��� ��ż���%�ڽ��� ��ۼ���%�ڽ��� ���ͳ�%�ڽ��� ������%�ڽ��� ����Ʈ����%�ڽ��� ��ǻ�� ����%�ڽ��� ������%�ڽ��� �������%�ڽ��� �ݵ�ü%�ڽ��� IT��ǰ%�ڽ��� 100%�ڽ��� MID 300%�ڽ��� SMALL%�ڽ��� ����, ��ȭ%�ڽ���  50%�ڽ��� IT%";//2016.05.16 KSJ KOSTAR ���� ����
	AddCombo(CString(combodata));
	ListRemoveAll();

	std::vector<HCODEX*> vData;	
	for_each(_mapCODE.begin(), _mapCODE.end(), [&vData](auto item) {
		if (item.second->ssgb == jmSINJU || item.second->ssgb == jmSINJS || item.second->ssgb == jmHYFND || item.second->ssgb == jmELW)// �����μ��� �������� ����
			return;
		vData.push_back(item.second);
	});
	CodeListSome(vData);

	return TRUE;
}

BOOL CJongmukDlg::CodeListSome(vector<HCODEX*>& arr, bool bflag)
{
	ListRemoveAll(bflag);
	for_each(arr.begin(), arr.end(), [&](auto item){
		_vCODE.push_back(std::move(std::pair(CString(item->code, HCodeLen).Mid(1), CString(item->hnam, HNameLen).Mid(1).Trim())));
		if (bflag)
			_vSCODE.emplace_back(std::move(item));
	});
	m_ListCtrl.SetItemCountEx(_vCODE.size());
	m_ListCtrl.Invalidate();
	return TRUE;
}

// ������
//void CJongmukDlg::OnButtonAllcode()
//{
//	m_gListsort = FALSE;
//	clearEditSearch();
//	Hideall(0);
//
//	InitBtn();
//	m_ctlButtonAll.SetBold(TRUE);
//	m_ctlButtonAll.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
//	m_ctlButtonAll.SetSelected(TRUE);
//
//	m_select = 0;
//	m_ComboGroup.ShowWindow(SW_SHOW);
//	CodeListAll();
//	MoveGrid();
//	HideButton();
//
////	m_ListCtrl.SetFocus();
//	m_ListCtrl.Selected(0);
//
//	m_szCode = m_ListCtrl.GetItemText(0, 0);
//	m_szName = m_ListCtrl.GetItemText(0, 1);
//	m_szName.TrimLeft();
//
//}

// KOSPI
//void CJongmukDlg::OnButtonKospicode()
//{
// 	CPtrArray arCODE;
// 	CString	string;
// //	struct	hjcode*	hjc;
// 	std::vector<struct hjcode*> vArr;

// 	m_gListsort = FALSE;
// 	clearEditSearch();
// 	Hideall(0);

// 	InitBtn();
// 	m_ctlButtonKOSPI.SetBold(TRUE);
// 	m_ctlButtonKOSPI.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
// 	m_ctlButtonKOSPI.SetSelected(TRUE);

// 	m_select = 1;
// 	string = "��  ��%������%������%������%���ķ��%����, �Ǻ�%����, ����%ȭ  ��%�Ǿ�ǰ%��ݼӱ���%ö��, �ݼ�%��  ��%����, ����%�Ƿ�����%������%�����%���Ⱑ����%�Ǽ���%���â��%��ž�%������%��  ��%���� ������%��  ��%��  ��%���񽺾�%������%KOSPI 200%KOSPI 100%KOSPI 50%KOSPI IT%";
// 	AddCombo(string);
// 	m_ComboGroup.ShowWindow(SW_SHOW);
// 	MoveGrid();
// 	HideButton();

// 	for_each(_mapCODE.begin(), _mapCODE.end(), [vArr](auto item){
// 			if (item.second->kosd == jmKOSPI &&
// 	 		item.second->ssgb != jmSINJU &&
// 	 		item.second->ssgb != jmSINJS &&
// 	 		item.second->ssgb != jmHYFND &&
// 	 		item.second->ssgb != jmFOREIGN)
// 	 		vArr.Add(item.second);
// 	});

// 	// arCODE.RemoveAll();
// 	// for (int ii = 0; ii < m_arHCODE.GetSize(); ii++)
// 	// {
// 	// 	hjc = (struct hjcode *)m_arHCODE.GetAt(ii);
// 	// 	if (hjc->kosd == jmKOSPI &&
// 	// 		hjc->ssgb != jmSINJU &&
// 	// 		hjc->ssgb != jmSINJS &&
// 	// 		hjc->ssgb != jmHYFND &&
// 	// 		hjc->ssgb != jmFOREIGN)
// 	// 		arCODE.Add(hjc);
// 	// }

// 	//CodeListSome(arCODE);
// 	m_ListCtrl.Selected(0);
// 	m_szCode = m_ListCtrl.GetItemText(0, 0);
// 	m_szName = m_ListCtrl.GetItemText(0, 1);
// 	m_szName.TrimLeft();
// 	ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc, 0);
//}

//// KOSDAQ
//void CJongmukDlg::OnButtonKosdaqcode()
//{
//	CPtrArray arCODE;
//	CString	string;
//	struct	hjcode*	hjc;
//
//	m_gListsort = FALSE;
//	clearEditSearch();
//	Hideall(0);
//
//	InitBtn();
//	m_ctlButtonKOSDAQ.SetBold(TRUE);
//	m_ctlButtonKOSDAQ.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
//	m_ctlButtonKOSDAQ.SetSelected(TRUE);
//
//	m_select = -1;
//	string = "��  ��%��  ó%��Ÿ����%IT����%IT��ó%��  ��%��  ��%��  ��%����, ����%��  ��%��  ��%��Ź��%IT S/W%IT H/W%���ķ�, ���%����, �Ƿ�%����, ����%����, ��ü%ȭ  ��%��  ��%��ݼ�%��  ��%������ %��������%�Ƿ�, ����%���, ��ǰ%��Ÿ ����%��ż���%��ۼ���%���ͳ�%������%����Ʈ����%��ǻ�� ����%������%�������%�ݵ�ü%IT��ǰ%KOSDAQ 100%KOSDAQ MID 300%KOSDAQ SMALL%����, ��ȭ%KOSDAQ 50%KOSDAQ IT%";//2016.05.16 KSJ KOSTAR ���� ����
//	AddCombo(string);
//	MoveGrid();
//	m_ComboGroup.ShowWindow(SW_SHOW);
//	HideButton();
//
//	arCODE.RemoveAll();
//	for (int ii = 0; ii < m_arHCODE.GetSize(); ii++)
//	{
//		hjc = (struct hjcode *)m_arHCODE.GetAt(ii);
//		if (hjc->kosd == jmKOSDAQ)
//			arCODE.Add(hjc);
//	}
//
//	CodeListSome(arCODE);
//	m_ListCtrl.Selected(0);
//	m_szCode = m_ListCtrl.GetItemText(0, 0);
//	m_szName = m_ListCtrl.GetItemText(0, 1);
//	m_szName.TrimLeft();
//	ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc, 0);
//}

// ��3����
//void CJongmukDlg::OnButtonThirdmarket()
//{
//	m_gListsort = FALSE;
//	clearEditSearch();
//
//	InitBtn();
//	m_ctlButtonTHIRD.SetBold(TRUE);
//	m_ctlButtonTHIRD.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
//	m_ctlButtonTHIRD.SetSelected(TRUE);
//
//	if (m_kind == all2CODE)
//	{
//		m_bOption = FALSE;
//		Hideall(2);
//		HideButton();
//		MoveGrid(2);
//
//		Invalidate();
//		ListRemoveAll();
//		LoadUSCode();
//
//		for (int ii = 0 ; ii < m_arUSCODE.GetSize() ; ii++)
//		{
//			CString symb = CString(m_arUSCODE[ii]->symb, sizeof(m_arUSCODE[ii]->symb));
//			CString knam = CString(m_arUSCODE[ii]->knam, sizeof(m_arUSCODE[ii]->knam));
//			AddItem(ii, symb, knam);
//		}
//
//		m_ListCtrl.SetItemCountEx(m_arList.GetSize());
//	//	qsort(static_cast<void*>(&m_arList[0]), m_arList.GetSize(), sizeof( ITEMS), compareSortCode2);
//		m_ListCtrl.Invalidate();
//	}
//	else
//	{
//		CPtrArray arCODE;
//		struct	hjcode*	hjc;
//
//		m_select = OTHERTYPE;
//		m_ComboGroup.ShowWindow(SW_HIDE);
//		MoveGrid(0);
//		HideButton();
//
//		if (m_kind == kospiCODE || m_kind == allCODE || (m_kind == indexCODE && m_Type % 100 == 99))
//		{
//			InitBtn();
//			m_ctlButtonTHIRD.SetBold(TRUE);
//			m_ctlButtonTHIRD.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
//			m_ctlButtonTHIRD.SetSelected(TRUE);
//		}
//
//		arCODE.RemoveAll();
//		for (int ii = 0; ii < m_arHCODE.GetSize(); ii++)
//		{
//			hjc = (struct hjcode *)m_arHCODE.GetAt(ii);
//			if (hjc->kosd == jm3RD)
//				arCODE.Add(hjc);
//		}
//
//		CodeListSome(arCODE);
//		m_ListCtrl.Selected(0);
//		m_szCode = m_ListCtrl.GetItemText(0, 0);
//		m_szName = m_ListCtrl.GetItemText(0, 1);
//		m_szName.TrimLeft();
//		ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc, 0);
//	}
//}

//// ECN
//void CJongmukDlg::OnButtonEcncode()
//{
//	m_gListsort = FALSE;
//	clearEditSearch();
//
//	InitBtn();
//	m_ctlButtonECN.SetBold(TRUE);
//	m_ctlButtonECN.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
//	m_ctlButtonECN.SetSelected(TRUE);
//
//	if (m_kind == all2CODE)
//	{
//		m_bOption = TRUE;
//		Hideall(1);
//		Invalidate();
//		Filllistoption(m_nRadioOption);
//	}
//	else
//	{
//		CPtrArray arCODE;
//		struct	hjcode*	hjc;
//
//		m_select = OTHERTYPE;
//		MoveGrid(0);
//		m_ComboGroup.ShowWindow(SW_HIDE);
//		HideButton();
//
//		arCODE.RemoveAll();
//		for (int ii = 0; ii < m_arHCODE.GetSize(); ii++)
//		{
//			hjc = (struct hjcode *)m_arHCODE.GetAt(ii);
//			if (hjc->ecng == 1)
//				arCODE.Add(hjc);
//		}
//
//		CodeListSome(arCODE);
//		m_ListCtrl.Selected(0);
//		m_szCode = m_ListCtrl.GetItemText(0, 0);
//		m_szName = m_ListCtrl.GetItemText(0, 1);
//		m_szName.TrimLeft();
//	}
//}

// �ڽ��� ����
void CJongmukDlg::OnButtonKospiwork()
{
	CString	string;
	struct	upcode* upc{};
	CRect rect(100 + GAPPLUS, 20, LISTCTRL_WIDTH + GAPPLUS, 340);

	m_gListsort = TRUE;
	clearEditSearch();
	Hideall(0);

	InitBtn();
	m_ctlButtonKOSPIWORK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
	m_ctlButtonKOSPIWORK.SetSelected(TRUE);

	m_ListCtrl.MoveWindow(rect);
        m_ComboGroup.ShowWindow(SW_HIDE);
	HideButton(TRUE);

	ListRemoveAll();
	LoadUPCode();

	if (m_kind == allCODE || (m_kind == indexCODE && m_Type % 100 == 99))
	{
		InitBtn();
		m_ctlButtonKOSPIWORK.SetBold(TRUE);
		m_ctlButtonKOSPIWORK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
		m_ctlButtonKOSPIWORK.SetSelected(TRUE);
	}

	_vCODE.clear();
	for (int ii = 0, count = 0; ii < m_arUPCODE.GetSize(); ii++)
	{
		upc = (struct upcode *)m_arUPCODE.GetAt(ii);
		if (m_Type == 10 || m_Type == 199)	// 40���� ���� ����
		{
			if (upc->jgub != 0 || upc->ucod >= 40)
				continue;
		}
		if (upc->jgub == 0 || upc->jgub == 1 || upc->jgub == 4)
		{
			string.Format("%d%02d", upc->jgub, upc->ucod);
			AddItem(count++, string, CString(upc->hnam, UNameLen));
		}
	}

	m_ListCtrl.SetItemCountEx(_vCODE.size());
	listsort(0);
	
	m_ListCtrl.Invalidate();
	m_ListCtrl.Selected(0);

	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);
	m_szName.TrimLeft();
}

// �ڽ��� ����
void CJongmukDlg::OnButtonKosdaqwork()
{
	CString	string;
	struct	upcode* upc{};
	CRect rect(100 + GAPPLUS, 20, LISTCTRL_WIDTH + GAPPLUS, 340);

	m_gListsort = TRUE;
	clearEditSearch();
	Hideall(0);

	InitBtn();
	m_ctlButtonKOSDAQWORK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
	m_ctlButtonKOSDAQWORK.SetSelected(TRUE);

	m_ListCtrl.MoveWindow(rect);

	m_ComboGroup.ShowWindow(SW_HIDE);
	HideButton(TRUE);
	LoadUPCode();
	ListRemoveAll();

	if (m_kind == allCODE ||(m_kind == indexCODE && m_Type % 100 == 99))
	{
		InitBtn();
		m_ctlButtonKOSDAQWORK.SetBold(TRUE);
		m_ctlButtonKOSDAQWORK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
		m_ctlButtonKOSDAQWORK.SetSelected(TRUE);
	}

	_vCODE.clear();
	for (int ii = 0, count = 0; ii <  m_arUPCODE.GetSize(); ii++)
	{
		upc = (struct upcode *)m_arUPCODE.GetAt(ii);
		if (m_Type == 19 || m_Type == 199)	// 300���� ����
		{
			if (upc->jgub == 3)
				continue;
		}
		if (upc->jgub == 2 || upc->jgub == 3)
		{
			string.Format("%d%02d", upc->jgub, upc->ucod);
			AddItem(count++, string, CString(upc->hnam, UNameLen));
		}
	}

	m_ListCtrl.SetItemCountEx(_vCODE.size());
	listsort(0);
	m_ListCtrl.Invalidate();
	m_ListCtrl.Selected(0);

	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);
	m_szName.TrimLeft();
}

//�ڳؽ� ���� 2013.06.05 KSJ
void CJongmukDlg::OnButtonKonexwork()
{
	// TODO: Add your control notification handler code here
	CString	string;
	struct	upcode* upc{};
	CRect rect(100 + GAPPLUS, 20, LISTCTRL_WIDTH + GAPPLUS, 340);

	m_gListsort = TRUE;
	clearEditSearch();
	Hideall(0);

	InitBtn();
	m_ctlButtonKONEXWORK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
	m_ctlButtonKONEXWORK.SetSelected(TRUE);

	m_ListCtrl.MoveWindow(rect);

	m_ComboGroup.ShowWindow(SW_HIDE);
	HideButton(TRUE);
	LoadUPCode();
	ListRemoveAll();

	if (m_kind == allCODE ||(m_kind == konexCODE && m_Type % 100 == 99))
	{
		InitBtn();
		m_ctlButtonKONEXWORK.SetBold(TRUE);
		m_ctlButtonKONEXWORK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
		m_ctlButtonKONEXWORK.SetSelected(TRUE);
	}

	_vCODE.clear();
	for (int ii = 0, count = 0; ii <  m_arUPCODE.GetSize(); ii++)
	{
		upc = (struct upcode *)m_arUPCODE.GetAt(ii);

		if (upc->jgub == 5)
		{
			string.Format("%d%02d", upc->jgub, upc->ucod);
			AddItem(count++, string, CString(upc->hnam, UNameLen));
		}
	}

	m_ListCtrl.SetItemCountEx(_vCODE.size());
	listsort(0);
	m_ListCtrl.Invalidate();

	m_ListCtrl.Selected(0);

	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);
	m_szName.TrimLeft();
}

// �����μ��� ����
//void CJongmukDlg::OnButtonSinju()
//{
//	clearEditSearch();
//
//	InitBtn();
//	m_ctlButtonSINJU.SetBold(TRUE);
//	m_ctlButtonSINJU.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
//	m_ctlButtonSINJU.SetSelected(TRUE);
//
//	if (m_kind == all2CODE)
//	{
//		m_bOption = TRUE;
//		Hideall(1);
//		Invalidate();
//		Filllistoption(m_nRadioOption);
//	}
//	else
//	{
//		CPtrArray arCODE;
//		struct	hjcode*	hjc;
//
//		m_select = OTHERTYPE;
//		MoveGrid(0);
//		m_ComboGroup.ShowWindow(SW_HIDE);
//		HideButton();
//
//		arCODE.RemoveAll();
//		for (int ii = 0; ii < m_arHCODE.GetSize(); ii++)
//		{
//			hjc = (struct hjcode *)m_arHCODE.GetAt(ii);
//			if (hjc->ssgb == jmSINJU || hjc->ssgb == jmSINJS || hjc->ssgb == jmHYFND)
//				arCODE.Add(hjc);
//		}
//
//		CodeListSome(arCODE);
//		m_gListsort = TRUE;
//		ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc, 0);
//		m_ListCtrl.Selected(0);
//		m_szCode = m_ListCtrl.GetItemText(0, 0);
//		m_szName = m_ListCtrl.GetItemText(0, 1);
//		m_szName.TrimLeft();
//	}
//}

void CJongmukDlg::MoveGrid(int kind)
{
	CRect	rect;
	switch (kind)
	{
	case 0:	rect.SetRect(100 + GAPPLUS, 17, LISTCTRL_WIDTH + GAPPLUS, 260);	break;	// hide combo
	case 1:	rect.SetRect(100 + GAPPLUS, 40, LISTCTRL_WIDTH + GAPPLUS, 260);	break;	// show combo
	case 2:	rect.SetRect(100 + GAPPLUS, 17, LISTCTRL_WIDTH + GAPPLUS, 283);	break;
	case 3:	rect.SetRect(100 + GAPPLUS, 40, LISTCTRL_WIDTH + GAPPLUS, 345);	break;
	case 4:	rect.SetRect(100 + GAPPLUS, 17, LISTCTRL_WIDTH + GAPPLUS, 353);	break;
	default:
		return;
	}
	m_ListCtrl.MoveWindow(rect);
}

void CJongmukDlg::HideButton(BOOL flag)
{
	const UINT	showCmd = flag ? SW_HIDE : SW_SHOW;

	for (int ii = IDC_BUTTON_1; ii <= IDC_STATIC_SEARCH; ii++)
		((CButton*)GetDlgItem(ii))->ShowWindow(showCmd);
}

void CJongmukDlg::AddCombo(CString& szText)
{
	CString temp;
	int	pos{}, nCnt = 0;

	m_ComboGroup.ResetContent();
	while (!szText.IsEmpty())
	{
		pos = szText.Find('%');
		nCnt++;
		if (pos != 0)
		{
			temp = szText.Left(pos);
			m_ComboGroup.SetItemData(m_ComboGroup.AddString(temp), nCnt);
		}
		szText = szText.Mid(pos + 1);
	}

	m_ComboGroup.SetCurSel(0);
}

void CJongmukDlg::AddGrid(CString& str, int kind)
{
	int num{}, pos{}, ii = 0;
	CString temp, code;

	ListRemoveAll();

	while(!str.IsEmpty())
	{
		++ii;
		pos  = str.Find('%');
		temp = str.Left(pos);
		num  = str.Find(' ');
		code = str.Left(num);
		temp = temp.Mid(num + 1);

		if(ii <=  kind)
			AddItem(ii - 1, code, temp);
		str = str.Mid(pos + 1);
	}

	m_ComboGroup.SetCurSel(0);
}

BOOL CJongmukDlg::ListAllWorkSort(int nItem)
{
	std::vector<HCODEX*> vData;
	for_each(_mapCODE.begin(), _mapCODE.end(), [nItem, &vData](const auto item){
		HCODEX* hjc = item.second;
		if (hjc->ssgb == jmSINJU || hjc->ssgb == jmSINJS || hjc->ssgb == jmHYFND)	// �����μ��� �������� ����
			return;
		switch (nItem)
		{
		case 1:
			vData.emplace_back(hjc);
			break;
		case 2:		// KOSPI ����
			if (hjc->kosd == jmKOSPI)
				vData.emplace_back(hjc);
			break;
		case 3:		// ������
			if (hjc->kosd == jmKOSPI && hjc->size == 2)
				vData.emplace_back(hjc);
			break;
		case 4:		// ������
			if (hjc->kosd == jmKOSPI && hjc->size == 3)
				vData.emplace_back(hjc);
			break;
		case 5:		// ������
			if (hjc->kosd == jmKOSPI && hjc->size == 4)
				vData.emplace_back(hjc);
			break;

		case 6:		// ���ķ��
			if (CString(item.second->acdm, 6).CompareNoCase("KGS01P") == 0)
				vData.emplace_back(item.second);
			break;

		case 7:		// ����, �Ǻ�
			if (CString(item.second->acdm, 6).CompareNoCase("KGS02P") == 0)
				vData.emplace_back(item.second);
			break;
		case 8:		// �� ��
			if (CString(item.second->acdm, 6).CompareNoCase("KGS03P") == 0)
				vData.emplace_back(item.second);
			break;
		case 9:		// ȭ ��
			if (CString(item.second->acdm, 6).CompareNoCase("KGS04P") == 0)
				vData.emplace_back(item.second);
			break;
		case 10:		// �Ǿ�ǰ/����
			if (CString(item.second->acdm, 6).CompareNoCase("KGS05P") == 0)
				vData.emplace_back(item.second);
			break;
		case 11:	// ��ݼӱ���
			if (CString(item.second->acdm, 6).CompareNoCase("KGS06P") == 0)
				vData.emplace_back(item.second);
			break;
		case 12:	// ö ��
			if (CString(item.second->acdm, 6).CompareNoCase("KGS07P") == 0)
				vData.emplace_back(item.second);
			break;
		case 13:	// �� ��
			if (CString(item.second->acdm, 6).CompareNoCase("KGS08P") == 0)
				vData.emplace_back(item.second);
			break;
		case 14:	// ���� ����/IT
			if (CString(item.second->acdm, 6).CompareNoCase("KGS09P") == 0)
				vData.emplace_back(item.second);
			break;
		case 15:	// �Ƿ�����
			if (CString(item.second->acdm, 6).CompareNoCase("KGS19P") == 0)
				vData.emplace_back(item.second);
			break;
		case 16:	// ������
			if (CString(item.second->acdm, 6).CompareNoCase("KGS10P") == 0)
				vData.emplace_back(item.second);
			break;
		case 17:	// �����
			if (CString(item.second->acdm, 6).CompareNoCase("KGS11P") == 0)
				vData.emplace_back(item.second);
			break;
		case 18:	// ���� ������
			if (CString(item.second->acdm, 6).CompareNoCase("KGS20P") == 0)
				vData.emplace_back(item.second);
			break;
		case 19:	// �Ǽ���
			if (CString(item.second->acdm, 6).CompareNoCase("KGS12P") == 0)
				vData.emplace_back(item.second);
			break;
		case 20:	// ���â��
			if (CString(item.second->acdm, 6).CompareNoCase("KGS13P") == 0)
				vData.emplace_back(item.second);
			break;
		case 21:	// ��ž�
			if (CString(item.second->acdm, 6).CompareNoCase("KGS21P") == 0)
				vData.emplace_back(item.second);
			break;
		case 22:	// ������
			if (CString(item.second->acdm, 6).CompareNoCase("KGS14P") == 0)
				vData.emplace_back(item.second);
			break;

		case 23:	// �� ��
//			if (CString(item.second->acds, 6).CompareNoCase("KGS15P") == 0)
//				vData.emplace_back(item.second);
//			break;
		case 24:	// ���� ����
			if (CString(item.second->acds, 6).CompareNoCase("KGS14P") == 0)
				vData.emplace_back(item.second);
			break;
		case 25:	// �� ��
			if (CString(item.second->acds, 6).CompareNoCase("KGS16P") == 0)
				vData.emplace_back(item.second);
			break;
		case 26:	// �� ��
			if (CString(item.second->acds, 6).CompareNoCase("KGS17P") == 0)
				vData.emplace_back(item.second);
			break;
		case 27:	// ���񽺾�
			if (CString(item.second->acdm, 6).CompareNoCase("KGS22P") == 0)
				vData.emplace_back(item.second);
			break;

		case 28:	// ������
			if (CString(item.second->acdl, 6).CompareNoCase("KGS22P") == 0)
				vData.emplace_back(hjc);
			break;
	/*	case 29:	// ������ (�ں�)
			if (hjc->kosd == jmKOSPI && hjc->jsiz == 38)
				m_arSelCode.Add(hjc);
			break;
		case 30:	// ������ (�ں�)
			if (hjc->kosd == jmKOSPI && hjc->jsiz == 39)
				m_arSelCode.Add(hjc);
			break;
		case 31:	// ������ (�ں�)
			if (hjc->kosd == jmKOSPI && hjc->jsiz == 40)
				m_arSelCode.Add(hjc);
			break;	*/
		case 32:	// �������
			if (hjc->kosd == jmKOSPI && (hjc->jsiz >= 38 + 41 && hjc->jsiz <= 41 + 41))
				vData.emplace_back(hjc);
			break;
		case 33:	// ���豸�����
			if (hjc->kosd == jmKOSPI && (hjc->wsgb == 1))
				vData.emplace_back(hjc);
			break;
		case 34:	// �ڽ��� 200
			if (hjc->kosd == jmKOSPI && (hjc->kpgb == 1 || hjc->kpgb == 2 || hjc->kpgb == 3))
				vData.emplace_back(hjc);
			break;
		case 35:	// �ڽ��� 100
			if (hjc->kosd == jmKOSPI && (hjc->kpgb == 3 || hjc->kpgb == 2))
				vData.emplace_back(hjc);
			break;
		case 36:	// �ڽ��� 50
			if (hjc->kosd == jmKOSPI && hjc->kpgb == 3)
				vData.emplace_back(hjc);
			break;
		case 37:	// �ڽ��� IT
			if (hjc->kosd == jmKOSPI && hjc->itgb == 1)
				vData.emplace_back(hjc);
			break;
		case 38:	// �ڽ��� ����
			if (hjc->kosd == jmKOSDAQ)
				vData.emplace_back(hjc);
			break;
		case 39:	// ��ó����
			if (hjc->kosd == jmKOSDAQ && hjc->jjug == 2)
				vData.emplace_back(hjc);
			break;
		case 40:	// ��Ÿ ����
			if (hjc->kosd == jmKOSDAQ && hjc->size == 3)
				vData.emplace_back(hjc);
			break;
		case 41:	// �ڽ��� IT ����
			if (hjc->kosd == jmKOSDAQ && hjc->size == 4)
				vData.emplace_back(hjc);
			break;
		case 42:	// �ڽ��� IT ��ó
			if (hjc->kosd == jmKOSDAQ && hjc->size == 4 && hjc->jjug == 2)
				vData.emplace_back(hjc);
			break;

		case 43:	// �� ��
			if (CString(item.second->acdm, 6).CompareNoCase("QGS36P") == 0)
				vData.emplace_back(hjc);
			break;
		case 44:	// ����/�Ǽ�
			if (CString(item.second->acdm, 6).CompareNoCase("QGS38P") == 0)
				vData.emplace_back(hjc);
			break;
		case 45:	// �� ��
			if (CString(item.second->acdm, 6).CompareNoCase("QGS04P") == 0)
				vData.emplace_back(hjc);
			break;
		case 46:	// �� ��
//			if (CString(item.second->acdm, 6).CompareNoCase("QGS34P") == 0)
//				vData.emplace_back(hjc);
			break;
		case 47:	// �� ��
			if (CString(item.second->acdm, 6).CompareNoCase("QGS31P") == 0)
				vData.emplace_back(hjc);
			break;
		case 48:	// �� ��/ �� ��
			if (CString(item.second->acdm, 6).CompareNoCase("QGS39P") == 0 ||
				CString(item.second->acdm, 6).CompareNoCase("QGS05P") == 0 )
				vData.emplace_back(hjc);
			break;

		case 49:	// ��Ź�ۼ���
			if (CString(item.second->acdm, 6).CompareNoCase("QGS06P") == 0)
				vData.emplace_back(hjc);
			break;
		case 50:	// S/W & SVC
			if (CString(item.second->acdm, 6).CompareNoCase("QGS07P") == 0)
				vData.emplace_back(hjc);
			break;
		case 51:	// IT H/W
			if (CString(item.second->acdm, 6).CompareNoCase("QGS08P") == 0)
				vData.emplace_back(hjc);
			break;

		case 52:	// ���ķ�, ���
			if (CString(item.second->acds, 6).CompareNoCase("QGS09P") == 0)
				vData.emplace_back(hjc);
			break;
		case 53:	// ����, �Ƿ�
			if (CString(item.second->acds, 6).CompareNoCase("QGS10P") == 0)
				vData.emplace_back(hjc);
			break;
		case 54:	// ����, ����
			if (CString(item.second->acds, 6).CompareNoCase("QGS11P") == 0)
				vData.emplace_back(hjc);
			break;
		case 55:	// ���� ��ü����
			if (CString(item.second->acds, 6).CompareNoCase("QGS12P") == 0)
				vData.emplace_back(hjc);
			break;
		case 56:	// ȭ ��
			if (CString(item.second->acds, 6).CompareNoCase("QGS13P") == 0)
				vData.emplace_back(hjc);
			break;
		case 57:	// �� ��
			if (CString(item.second->acds, 6).CompareNoCase("QGS33P") == 0)
				vData.emplace_back(hjc);
			break;
		case 58:	// ��ݼ�
			if (CString(item.second->acds, 6).CompareNoCase("QGS14P") == 0)
				vData.emplace_back(hjc);
			break;
		case 59:	// �� ��
			if (CString(item.second->acds, 6).CompareNoCase("QGS15P") == 0)
				vData.emplace_back(hjc);
			break;
		case 60:	// ��� ���
			if (CString(item.second->acds, 6).CompareNoCase("QGS16P") == 0)
				vData.emplace_back(hjc);
			break;
		case 61:	// �Ϲ� ���� ����
			if (CString(item.second->acds, 6).CompareNoCase("QGS17P") == 0)
				vData.emplace_back(hjc);
			break;
		case 62:	// �Ƿ� , ���б��
			if (CString(item.second->acds, 6).CompareNoCase("QGS18P") == 0)
				vData.emplace_back(hjc);
			break;
		case 63:	// ������ ��ǰ
			if (CString(item.second->acds, 6).CompareNoCase("QGS19P") == 0)
				vData.emplace_back(hjc);
			break;
		case 64:	// ��Ÿ ����
			if (CString(item.second->acds, 6).CompareNoCase("QGS20P") == 0)
				vData.emplace_back(hjc);
			break;
		case 65:	// ��� ����
			if (CString(item.second->acds, 6).CompareNoCase("QGS21P") == 0)
				vData.emplace_back(hjc);
			break;
		case 66:	// ��� ����
			if (CString(item.second->acds, 6).CompareNoCase("QGS22P") == 0)
				vData.emplace_back(hjc);
			break;
		case 67:	// ���ͳ�
			if (CString(item.second->acds, 6).CompareNoCase("QGS23P") == 0)
				vData.emplace_back(hjc);
			break;
		case 68:	// ������ ������
			if (CString(item.second->acds, 6).CompareNoCase("QGS24P") == 0)
				vData.emplace_back(hjc);
			break;
		case 69:	// ����Ʈ����
			if (CString(item.second->acds, 6).CompareNoCase("QGS25P") == 0)
				vData.emplace_back(hjc);
			break;
		case 70:	// ��ǻ�� ����
			if (CString(item.second->acds, 6).CompareNoCase("QGS26P") == 0)
				vData.emplace_back(hjc);
			break;
		case 71:	// ������
			if (CString(item.second->acds, 6).CompareNoCase("QGS27P") == 0)
				vData.emplace_back(hjc);
			break;
		case 72:	// �������
			if (CString(item.second->acds, 6).CompareNoCase("QGS28P") == 0)
				vData.emplace_back(hjc);
			break;
		case 73:	// �ݵ�ü
			if (CString(item.second->acds, 6).CompareNoCase("QGS29P") == 0)
				vData.emplace_back(hjc);
			break;
		case 74:	// IT ��ǰ
			if (CString(item.second->acds, 6).CompareNoCase("QGS30P") == 0)
				vData.emplace_back(hjc);
			break;


		case 75:	// KOSDAQ 100
			if (hjc->kosd == jmKOSDAQ && hjc->jsiz == 1)
				vData.emplace_back(hjc);
			break;
		case 76:	// KOSDAQ MID 300
			if (hjc->kosd == jmKOSDAQ && hjc->jsiz == 2)
				vData.emplace_back(hjc);
			break;
		case 77:	// KOSDAQ SMALL
			if (hjc->kosd == jmKOSDAQ && hjc->jsiz == 3)
				vData.emplace_back(hjc);
			break;

		case 78:	// KOSDAQ ����/��ȭ
			if (CString(hjc->acds, 6).CompareNoCase("QGS87P") == 0)
				vData.emplace_back(hjc);
			break;
		case 79:	// KOSDAQ 50
			if (hjc->kosd == jmKOSDAQ && hjc->kpgb == 1)
				vData.emplace_back(hjc);
			break;
		case 80:	// KOSDAQ IT
			if (hjc->kosd == jmKOSDAQ && hjc->itgb == 1)
				vData.emplace_back(hjc);
			break;
		case 81:	// KOSTAR
			if (hjc->kosd == jmKOSDAQ && hjc->star == 1)
				vData.emplace_back(hjc);
			break;
		}

	});


	ListRemoveAll();
	const int count = vData.size();
	if (count < 18)
	{
		m_ListCtrl.SetColumnWidth(1, COLUMN1_WIDTH);
		if (count == 0)
			return FALSE;
	}

	CodeListSome(vData);
	m_bAddAll = true;

	return TRUE;
}

void CJongmukDlg::ListKospiSort(int nItem)
{
	vector<HCODEX*> vData;
	_vCODE.clear();
	for_each(_mapCODE.begin(), _mapCODE.end(), [nItem, &vData](auto item){
		switch (nItem)
		{
		case 1:
			if (item.second->kosd != jmKOSPI)	return;
			if (item.second->ssgb != jmSINJU
				&& item.second->ssgb != jmSINJS
				&& item.second->ssgb != jmHYFND
				&& item.second->ssgb != jmELW
				&& item.second->ssgb != jmFOREIGN)	// �����μ��� �������� ����
				vData.emplace_back(item.second);
			break;
		case 2:		// ������
			if (item.second->kosd == jmKOSPI && item.second->size == 2)
				vData.emplace_back(item.second);
			break;
		case 3:		// ������
			if (item.second->kosd == jmKOSPI && item.second->size == 3)
				vData.emplace_back(item.second);
			break;
		case 4:		// ������
			if (item.second->kosd == jmKOSPI && item.second->size == 4)
				vData.emplace_back(item.second);
			break;
		case 5:		// ���ķ��
			if (CString(item.second->acdm, 6).CompareNoCase("KGS01P") == 0)
				vData.emplace_back(item.second);
			break;

		case 6:		// ����, �Ǻ�
			if (CString(item.second->acdm, 6).CompareNoCase("KGS02P") == 0)
				vData.emplace_back(item.second);
			break;
		case 7:		// �� ��
			if (CString(item.second->acdm, 6).CompareNoCase("KGS03P") == 0)
				vData.emplace_back(item.second);
			break;
		case 8:		// ȭ ��
			if (CString(item.second->acdm, 6).CompareNoCase("KGS04P") == 0)
				vData.emplace_back(item.second);
			break;
		case 9:		// �Ǿ�ǰ/����
			if (CString(item.second->acdm, 6).CompareNoCase("KGS05P") == 0)
				vData.emplace_back(item.second);
			break;
		case 10:	// ��ݼӱ���
			if (CString(item.second->acdm, 6).CompareNoCase("KGS06P") == 0)
				vData.emplace_back(item.second);
			break;
		case 11:	// ö ��
			if (CString(item.second->acdm, 6).CompareNoCase("KGS07P") == 0)
				vData.emplace_back(item.second);
			break;
		case 12:	// �� ��
			if (CString(item.second->acdm, 6).CompareNoCase("KGS08P") == 0)
				vData.emplace_back(item.second);
			break;
		case 13:	// ���� ����/IT
			if (CString(item.second->acdm, 6).CompareNoCase("KGS09P") == 0)
				vData.emplace_back(item.second);
			break;
		case 14:	// �Ƿ�����
			if (CString(item.second->acdm, 6).CompareNoCase("KGS19P") == 0)
				vData.emplace_back(item.second);
			break;
		case 15:	// ������
			if (CString(item.second->acdm, 6).CompareNoCase("KGS10P") == 0)
				vData.emplace_back(item.second);
			break;
		case 16:	// �����
			if (CString(item.second->acdm, 6).CompareNoCase("KGS11P") == 0)
				vData.emplace_back(item.second);
			break;
		case 17:	// ���� ������
			if (CString(item.second->acdm, 6).CompareNoCase("KGS20P") == 0)
				vData.emplace_back(item.second);
			break;
		case 18:	// �Ǽ���
			if (CString(item.second->acdm, 6).CompareNoCase("KGS12P") == 0)
				vData.emplace_back(item.second);
			break;
		case 19:	// ���â��
			if (CString(item.second->acdm, 6).CompareNoCase("KGS13P") == 0)
				vData.emplace_back(item.second);
			break;
		case 20:	// ��ž�
			if (CString(item.second->acdm, 6).CompareNoCase("KGS21P") == 0)
				vData.emplace_back(item.second);
			break;
		case 21:	// ������
			if (CString(item.second->acdm, 6).CompareNoCase("KGS14P") == 0)
				vData.emplace_back(item.second);
			break;

		case 22:	// �� ��
//			if (CString(item.second->acds, 6).CompareNoCase("KGS15P") == 0)
//				vData.emplace_back(item.second);
			break;
		case 23:	// ���� ����
//			if (CString(item.second->acds, 6).CompareNoCase("KGS14P") == 0)
//				vData.emplace_back(item.second);
			break;
		case 24:	// �� ��
			if (CString(item.second->acds, 6).CompareNoCase("KGS16P") == 0)
				vData.emplace_back(item.second);
			break;
		case 25:	// �� ��
			if (CString(item.second->acds, 6).CompareNoCase("KGS17P") == 0)
				vData.emplace_back(item.second);
			break;
		case 26:	// ���񽺾�
			if (CString(item.second->acdm, 6).CompareNoCase("KGS22P") == 0)
				vData.emplace_back(item.second);
			break;
//*****************************************************************************************************
		case 27:	// ������
			if (item.second->kosd == jmKOSPI && item.second->jjug == 27)
				vData.emplace_back(item.second);
			break;
/*		
		case 28:	// ������(�ں�)
			if (hjc->kosd == jmKOSPI && hjc->jsiz == 38)
				m_arSelCode.Add(hjc);
			break;
		case 29:	// ������(�ں�)
			if (hjc->kosd == jmKOSPI && hjc->jsiz == 39)
				m_arSelCode.Add(hjc);
			break;
		case 30:	// ������(�ں�)
			if (hjc->kosd == jmKOSPI && hjc->jsiz == 40)
				m_arSelCode.Add(hjc);
			break;	
*/
		case 31:	// �������
			if (item.second->kosd == jmKOSPI && (item.second->jsiz >= 38 + 41 && item.second->jsiz <= 41 + 41))
				vData.emplace_back(item.second);
			break;
		case 32:	// ���豸�����
			if (item.second->kosd == jmKOSPI && item.second->wsgb == 1)
				vData.emplace_back(item.second);
			break;
		case 101:	// �ڽ��� 200
			if (item.second->kosd == jmKOSPI && (item.second->kpgb == 1 || item.second->kpgb == 2 || item.second->kpgb == 3))
				vData.emplace_back(item.second);
			break;
		case 401:	// �ڽ��� 100
			if (item.second->kosd == jmKOSPI && (item.second->kpgb == 3 || item.second->kpgb == 2))
				vData.emplace_back(item.second);
			break;
		case 402:	// �ڽ��� 50
			if (item.second->kosd == jmKOSPI && item.second->kpgb == 3)
				vData.emplace_back(item.second);
			break;
		case 403:	// �ڽ��� IT
			if (item.second->kosd == jmKOSPI && item.second->itgb == 1)
				vData.emplace_back(item.second);
			break;
		}
	});

	ListRemoveAll();
	const int count = vData.size();
	if (count < 18)
	{
		m_ListCtrl.SetColumnWidth(1, COLUMN1_WIDTH);
		if (count == 0)
			return;
	}

	CodeListSome(vData);
}

void CJongmukDlg::ListKosDaqSort(int nItem)		// �ڽ��� �������� ����Ʈ��...
{
	std::vector<HCODEX*> vData;

	for_each(_mapCODE.begin(), _mapCODE.end(), [nItem, &vData](auto item){
	switch (nItem)
		{
		case 201:
			if (item.second->kosd == jmKOSDAQ)
				vData.emplace_back(item.second);
			break;
		case 202:		// ��ó����
			if (item.second->kosd == jmKOSDAQ && item.second->jjug == 2)
				vData.emplace_back(item.second);
			break;
		case 203:		// ��Ÿ ����
			if (item.second->kosd == jmKOSDAQ && item.second->size == 3)
				vData.emplace_back(item.second);
			break;
		case 204:		// �ڽ��� IT ����
			if (item.second->kosd == jmKOSDAQ && item.second->size == 4)
				vData.emplace_back(item.second);
			break;
		case 205:		// �ڽ��� IT ��ó
			if (item.second->kosd == jmKOSDAQ && item.second->size == 4 && item.second->jjug == 2)
				vData.emplace_back(item.second);
			break;	
// ucdm ����....			

		case 206:		// �� ��
			if (CString(item.second->acdm, 6).CompareNoCase("QGS02P") == 0)
				vData.emplace_back(item.second);
			break;
		case 207:		// ����/�Ǽ�
			if (CString(item.second->acdm, 6).CompareNoCase("QGS03P") == 0)
				vData.emplace_back(item.second);
			break;
		case 208:		// �� ��
			if (CString(item.second->acdm, 6).CompareNoCase("QGS04P") == 0)
				vData.emplace_back(item.second);
			break;
		case 209:		// �� ��
//			if (CString(item.second->acdm, 6).CompareNoCase("QGS01P") == 0) 
//				vData.emplace_back(item.second);
			break;
		case 210:	// �� ��
			if (CString(item.second->acdm, 6).CompareNoCase("QGS31P") == 0)
				vData.emplace_back(item.second);
			break;
		case 211:	// �� ��/ �� ��

			if (CString(item.second->acdm, 6).CompareNoCase("QGS39P") == 0 ||
				CString(item.second->acdm, 6).CompareNoCase("QGS05P") == 0)
				vData.emplace_back(item.second);
			break;
		case 212:	// ��Ź�ۼ���
			if (CString(item.second->acdm, 6).CompareNoCase("QGS06P") == 0)
				vData.emplace_back(item.second);
			break;
		case 213:	// S/W & SVC
			if (CString(item.second->acdm, 6).CompareNoCase("QGS07P") == 0)
				vData.emplace_back(item.second);
			break;
		case 214:	// IT H/W
			if (CString(item.second->acdm, 6).CompareNoCase("QGS08P") == 0)
				vData.emplace_back(item.second);
			break;

		case 215:	// ���ķ�, ���
			if (CString(item.second->acds, 6).CompareNoCase("QGS09P") == 0)
				vData.emplace_back(item.second);
			break;
		case 216:	// ����, �Ƿ�
			if (CString(item.second->acds, 6).CompareNoCase("QGS10P") == 0)
				vData.emplace_back(item.second);
			break;
		case 217:	// ����, ����
			if (CString(item.second->acds, 6).CompareNoCase("QGS11P") == 0)
				vData.emplace_back(item.second);
			break;
		case 218:	// ���� ��ü����
			if (CString(item.second->acds, 6).CompareNoCase("QGS12P") == 0)
				vData.emplace_back(item.second);
			break;
		case 219:	// ȭ ��
			if (CString(item.second->acds, 6).CompareNoCase("QGS13P") == 0)
				vData.emplace_back(item.second);
			break;
		case 220:	// �� ��
			if (CString(item.second->acds, 6).CompareNoCase("QGS33P") == 0)
				vData.emplace_back(item.second);
			break;
		case 221:	// ��ݼ�
			if (CString(item.second->acds, 6).CompareNoCase("QGS14P") == 0)
				vData.emplace_back(item.second);
			break;
		case 222:	// �� ��
			if (CString(item.second->acds, 6).CompareNoCase("QGS15P") == 0)
				vData.emplace_back(item.second);
			break;
		case 223:	// ��� ���
			if (CString(item.second->acds, 6).CompareNoCase("QGS16P") == 0)
				vData.emplace_back(item.second);
			break;
		case 224:	// �Ϲ� ���� ����
			if (CString(item.second->acds, 6).CompareNoCase("QGS17P") == 0)
				vData.emplace_back(item.second);
			break;
		case 225:	// �Ƿ�, ���б��
			if (CString(item.second->acds, 6).CompareNoCase("QGS18P") == 0)
				vData.emplace_back(item.second);
			break;
		case 226:	// ������ ��ǰ
			if (CString(item.second->acds, 6).CompareNoCase("QGS19P") == 0)
				vData.emplace_back(item.second);
			break;
		case 227:	// ��Ÿ ����
			if (CString(item.second->acds, 6).CompareNoCase("QGS20P") == 0)
				vData.emplace_back(item.second);
			break;
		case 228:	// ��� ����
			if (CString(item.second->acds, 6).CompareNoCase("QGS21P") == 0)
				vData.emplace_back(item.second);
			break;
		case 229:	// ��� ����
			if (CString(item.second->acds, 6).CompareNoCase("QGS22P") == 0)
				vData.emplace_back(item.second);
			break;
		case 230:	// ���ͳ�
			if (CString(item.second->acds, 6).CompareNoCase("QGS23P") == 0)
				vData.emplace_back(item.second);
			break;
		case 231:	// ������ ������
			if (CString(item.second->acds, 6).CompareNoCase("QGS24P") == 0)
				vData.emplace_back(item.second);
			break;
		case 232:	// ����Ʈ����
			if (CString(item.second->acds, 6).CompareNoCase("QGS25P") == 0)
				vData.emplace_back(item.second);
			break;
		case 233:	// ��ǻ�� ����
			if (CString(item.second->acds, 6).CompareNoCase("QGS26P") == 0)
				vData.emplace_back(item.second);
			break;
		case 234:	// ������
			if (CString(item.second->acds, 6).CompareNoCase("QGS27P") == 0)
				vData.emplace_back(item.second);
			break;
		case 235:	// �������
			if (CString(item.second->acds, 6).CompareNoCase("QGS28P") == 0)
				vData.emplace_back(item.second);
			break;
		case 236:	// �ݵ�ü
			if (CString(item.second->acds, 6).CompareNoCase("QGS29P") == 0)
				vData.emplace_back(item.second);
			break;
		case 237:	// IT ��ǰ
			if (CString(item.second->acds, 6).CompareNoCase("QGS30P") == 0)
				vData.emplace_back(item.second);
			break;




		case 238:	// KOSDAQ 100
			if (item.second->kosd == jmKOSDAQ && item.second->jsiz == 1)
				vData.emplace_back(item.second);
			break;
		case 239:	// KOSDAQ MID 300
			if (item.second->kosd == jmKOSDAQ && item.second->jsiz == 2)
				vData.emplace_back(item.second);
			break;
		case 240:	// KOSDAQ SMALL
			if (item.second->kosd == jmKOSDAQ && item.second->jsiz == 3)
				vData.emplace_back(item.second);
			break;
		case 241:	// KOSDAQ ����/��ȭ
			if (CString(item.second->acds, 6).CompareNoCase("QGS87P") == 0)
				vData.emplace_back(item.second);
			break;




		case 243:	// 2016.08.22 KSJ KOSDAQ �췮���
			if (item.second->kosd == jmKOSDAQ && item.second->uc20 == 1)
				vData.emplace_back(item.second);
			break;
		case 244:	// 2016.08.22 KSJ KOSDAQ ��ó���
			if (item.second->kosd == jmKOSDAQ && item.second->uc20 == 2)
				vData.emplace_back(item.second);
			break;
		case 245:	// 2016.08.22 KSJ KOSDAQ �߰߱��
			if (item.second->kosd == jmKOSDAQ && item.second->uc20 == 4)
				vData.emplace_back(item.second);
			break;
		case 246:	// 2016.08.22 KSJ KOSDAQ �ż�����
			if (item.second->kosd == jmKOSDAQ && item.second->uc20 == 8)
				vData.emplace_back(item.second);
			break;
		case 301:	// KOSDAQ 50
			if (item.second->kosd == jmKOSDAQ && item.second->kpgb == 1)
				vData.emplace_back(item.second);
			break;
		case 302:	// KOSDAQ IT 50
			if (item.second->kosd == jmKOSDAQ && item.second->itgb == 1)
				vData.emplace_back(item.second);
			break;
		case 305:	// 2016.08.22 KSJ KOSDAQ 150
			if (item.second->kosd == jmKOSDAQ && item.second->ssgb == jmKOSDAQ150)
				vData.emplace_back(item.second);
			break;
		}
	});

	ListRemoveAll();
	const int count = vData.size();
	if (count < 18)
	{
		m_ListCtrl.SetColumnWidth(1, COLUMN1_WIDTH);
		if (count == 0)
			return;
	}

	CodeListSome(vData);
}

void CJongmukDlg::ListReitsSort(int nItem)
{
	std::vector<HCODEX*> vData;
	ListRemoveAll();
	
	for_each(_mapCODE.begin(), _mapCODE.end(), [nItem, &vData](const auto hjc){
		switch (nItem)
		{
		case 1:
			if (hjc.second->ssgb == jmREITS || hjc.second->ssgb == jmSHIP || hjc.second->ssgb == jmINFRA)
				vData.emplace_back(hjc.second);
			break;
		case 2:
			if (hjc.second->ssgb == jmREITS)
				vData.emplace_back(hjc.second);
			break;
		case 3:
			if (hjc.second->ssgb == jmSHIP)
				vData.emplace_back(hjc.second);
			break;
		case 4:
			if (hjc.second->ssgb == jmINFRA)
				vData.emplace_back(hjc.second);
			break;
		}
	});

	const int count = vData.size();
	if (count < 18)
	{
		m_ListCtrl.SetColumnWidth(1, COLUMN1_WIDTH);
		if (count == 0)
			return;
	}	
	CodeListSome(vData);
}


void CJongmukDlg::OnClickBtnSort(UINT nID)
{
	CString	sName, sCode, sTitle;
	const CWnd	*pBtn = GetDlgItem(nID);
	pBtn->GetWindowText(sTitle);

	std::vector<HCODEX*> vData;
	if (sTitle == _T("����"))
	{
		ListRemoveAll(false);
		for_each(_vSCODE.begin(), _vSCODE.end(), [&vData](const auto hjc){
			CString Name = CString(hjc->hnam, sizeof(hjc->hnam)).Mid(1).Trim();
			if (0x41 >  Name.GetAt(0) || Name.GetAt(0) > 0x7A)
				return;
			vData.emplace_back(hjc);
		});

		CodeListSome(vData);

		if (m_ListCtrl.GetItemCount() > 0)
		{
			m_gListsort = TRUE;
			listsort(1);
			m_ListCtrl.Selected(0);
			m_szCode = m_ListCtrl.GetItemText(0, 0);
			m_szName = m_ListCtrl.GetItemText(0, 1);
			m_szName.TrimLeft();
		}

		m_bAddAll = false;
		m_EditSearch.SetWindowText("");
	}
	else if (sTitle == _T("�����"))
	{
		m_EditSearch.SetWindowText("");
		searchAction(0);
	}
	else if (sTitle == _T("��"))
	{
		m_EditSearch.GetWindowText(sName);
		if (sName.GetLength() == 0)
		{
			return;
		}

		if (sName.GetLength() == 1)	// ����
		{
			m_EditSearch.SetWindowText("");
			searchAction(0);
		}
		else
		{
			const char cChar = sName.GetAt(sName.GetLength()-1);
			if (cChar & HANGULMSK)	sName = sName.Left(sName.GetLength() - 2);
			else			sName = sName.Left(sName.GetLength() - 1);

			m_EditSearch.SetWindowText(sName);
			if (sName.IsEmpty())
				searchAction(0);
			else
				searchAction(1);
		}
	}
	else
	{
		m_EditSearch.GetWindowText(sName);
		m_EditSearch.SetWindowText(sName + sTitle);
		searchAction(1);
	}

	m_EditSearch.SetFocus();
	m_EditSearch.SetSel(0, -1);
	m_EditSearch.SetSel(-1, -1);
}

long CJongmukDlg::OnEditXMsg(WPARAM wParam, LPARAM lParam)
{
	UpdateData();
	CString	string;
	switch ((int) wParam)
	{
	case wpBLANK:
		searchAction(0);	// blank name search by kwon
		break;
	case wpENTER:
		break;
	case wpSEARCH1:
		searchAction(0);	// number search by kwon
		break;
	case wpSEARCH2:
		searchAction(1);	// char search by kwon
		break;
	case wpUP:
		if (m_ListCtrl.GetItemCount() > 0)
		{
			POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();
			if (pos == nullptr)
				break;
			int nItem = m_ListCtrl.GetNextSelectedItem(pos);
			nItem--;
			if (nItem < 0)	nItem = 0;
			m_ListCtrl.Selected(nItem);
			m_szCode = m_ListCtrl.GetItemText(nItem, 0);
			m_szName = m_ListCtrl.GetItemText(nItem, 1);
			m_szName.TrimLeft();

			m_EditSearch.SetFocus();
			m_EditSearch.SetSel(0, -1);
			m_EditSearch.SetSel(-1, -1);
		}
		break;
	case wpDOWN:
		if (m_ListCtrl.GetItemCount() > 0)
		{
			POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();
			if (pos == nullptr)
				break;
			int nItem = m_ListCtrl.GetNextSelectedItem(pos);
			nItem++;
			if (nItem >= m_ListCtrl.GetItemCount())	nItem = m_ListCtrl.GetItemCount() - 1;
			m_ListCtrl.Selected(nItem);
			m_szCode = m_ListCtrl.GetItemText(nItem, 0);
			m_szName = m_ListCtrl.GetItemText(nItem, 1);
			m_szName.TrimLeft();

			m_EditSearch.SetFocus();
			m_EditSearch.SetSel(0, -1);
			m_EditSearch.SetSel(-1, -1);
		}
		break;
	}

	return 0;
}

void CJongmukDlg::OnButtonOk()
{
	OnOK();
}

void CJongmukDlg::Part()
{
	CRect	rect;
	CString szText;
	Hideall(0);
	switch (m_kind)
	{
	case all2CODE:
		InitOption();
//		OnButtonAllcode();
		break;

	case ecnCODE:
		GetDlgItem(IDC_BUTTON_ALLCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSPICODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSDAQCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THIRDMARKET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSDAQWORK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSPIWORK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SINJU)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ELW)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SECTOR)->EnableWindow(FALSE);	// 2006. 07. 26 ����ȣ : �����߰�
		GetDlgItem(IDC_BUTTON_THIRDJS)->EnableWindow(FALSE);	// 2006. 12. 14 ������ : �������������߰�
		m_ctlBtnPremier.EnableWindow(FALSE);
		m_ctlButtonKONEXWORK.EnableWindow(FALSE);		//2013.06.05 KSJ �ڳؽ� �߰�

		szText = "ECN �ڵ� �ȳ�";
//		OnButtonEcncode();
		break;

	case kospiCODE:
		GetDlgItem(IDC_BUTTON_KOSPIWORK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSDAQWORK)->EnableWindow(FALSE);
		szText = "�ֽ� �ڵ� �ȳ�";
//		OnButtonAllcode();
		ShowTreeView();
		break;

	case reitsCODE:
		GetDlgItem(IDC_BUTTON_KOSPIWORK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSDAQWORK)->EnableWindow(FALSE);
		szText = "�ֽ� �ڵ� �ȳ�";
//		OnButtonAllcode();
		ShowTreeView();
		break;

	case thirdCODE:
		GetDlgItem(IDC_BUTTON_KOSPIWORK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSDAQWORK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ALLCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSPICODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSDAQCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ECNCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SINJU)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ELW)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SECTOR)->EnableWindow(FALSE);	// 2006. 07. 26 ����ȣ : �����߰�
		GetDlgItem(IDC_BUTTON_THIRDJS)->EnableWindow(FALSE);	// 2006. 12. 14 ������ : �������������߰�
		m_ctlBtnPremier.EnableWindow(FALSE);
		m_ctlButtonKONEXWORK.EnableWindow(FALSE);		//2013.06.05 KSJ �ڳؽ� �߰�
		//OnButtonThirdmarket();
		szText = "K-OTC �ڵ� �ȳ�";
		break;

	case indexCODE:
		GetDlgItem(IDC_BUTTON_ALLCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSPICODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSDAQCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ECNCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THIRDMARKET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SINJU)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ELW)->EnableWindow(FALSE);

		if (m_Type == 0 || m_Type == 10)	// KOSPI
		{
			((CButton*)GetDlgItem(IDC_BUTTON_KOSDAQWORK))->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SECTOR)->EnableWindow(FALSE);	// 2006. 07. 26 ����ȣ : �����߰�
			GetDlgItem(IDC_BUTTON_THIRDJS)->EnableWindow(FALSE);	// 2006. 12. 14 ������ : �������������߰�
			m_ctlBtnPremier.EnableWindow(FALSE);
			m_ctlButtonKONEXWORK.EnableWindow(FALSE);		//2013.06.05 KSJ �ڳؽ� �߰�
			OnButtonKospiwork();
		}
		else if (m_Type == 9 || m_Type == 19)	// KOSDAQ
		{
			((CButton*)GetDlgItem(IDC_BUTTON_KOSPIWORK))->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SECTOR)->EnableWindow(FALSE);	// 2006. 07. 26 ����ȣ : �����߰�
			GetDlgItem(IDC_BUTTON_THIRDJS)->EnableWindow(FALSE);	// 2006. 12. 14 ������ : �������������߰�
			m_ctlBtnPremier.EnableWindow(FALSE);
			m_ctlButtonKONEXWORK.EnableWindow(FALSE);		//2013.06.05 KSJ �ڳؽ� �߰�
			OnButtonKosdaqwork();
		}
		else if (m_Type == 99)		// ������
		{
			OnButtonKospiwork();
		}
		else if (m_Type == 199)		// ������(kospi+kosdaq)
		{
			OnButtonKospiwork();
			GetDlgItem(IDC_BUTTON_SECTOR)->EnableWindow(FALSE);	// 2006. 07. 26 ����ȣ : �����߰�
			GetDlgItem(IDC_BUTTON_THIRDJS)->EnableWindow(FALSE);	// 2006. 12. 14 ������ : �������������߰�
			m_ctlBtnPremier.EnableWindow(FALSE);
			m_ctlButtonKONEXWORK.EnableWindow(FALSE);		//2013.06.05 KSJ �ڳؽ� �߰�
		}
		else if (m_Type == 200)	// ��������
		{
			((CButton*)GetDlgItem(IDC_BUTTON_KOSPIWORK))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON_KOSDAQWORK))->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_THIRDJS)->EnableWindow(FALSE);	// 2006. 12. 14 ������ : �������������߰�
			m_ctlBtnPremier.EnableWindow(FALSE);
			m_ctlButtonKONEXWORK.EnableWindow(FALSE);		//2013.06.05 KSJ �ڳؽ� �߰�
			OnButtonSector();
		}
		else if (m_Type == 201)	// ������������
		{
			((CButton*)GetDlgItem(IDC_BUTTON_KOSPIWORK))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON_KOSDAQWORK))->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SECTOR)->EnableWindow(FALSE);	// 2006. 12. 14 ������ : �������������߰�
			OnButtonThirdjs();
			m_ctlBtnPremier.EnableWindow(FALSE);
			m_ctlButtonKONEXWORK.EnableWindow(FALSE);		//2013.06.05 KSJ �ڳؽ� �߰�
		}
		else if(m_Type == 801 ) // �����̾� ����
		{
			OnButtonPremier();
			((CButton*)GetDlgItem(IDC_BUTTON_KOSPIWORK))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON_KOSDAQWORK))->EnableWindow(FALSE);
			m_CtlButtonThirdJS.EnableWindow( FALSE );
			m_CtlButtonSector.EnableWindow(FALSE);
			m_ctlButtonKONEXWORK.EnableWindow(FALSE);		//2013.06.05 KSJ �ڳؽ� �߰�
		}
		else if(m_Type == 501 ) // 2013.06.05 KSJ �ڳؽ����� �߰�
		{
			((CButton*)GetDlgItem(IDC_BUTTON_KOSPIWORK))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON_KOSDAQWORK))->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SECTOR)->EnableWindow(FALSE);	// 2006. 07. 26 ����ȣ : �����߰�
			GetDlgItem(IDC_BUTTON_THIRDJS)->EnableWindow(FALSE);	// 2006. 12. 14 ������ : �������������߰�
			m_ctlBtnPremier.EnableWindow(FALSE);
			OnButtonKonexwork();
		}

		szText = "���� �ڵ� �ȳ�";
		break;

	case kospi2CODE:
		GetDlgItem(IDC_BUTTON_ALLCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSPICODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSDAQCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ECNCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THIRDMARKET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSDAQWORK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSPIWORK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SINJU)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ELW)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SECTOR)->EnableWindow(FALSE);	// 2006. 07. 26 ����ȣ : �����߰�
		GetDlgItem(IDC_BUTTON_THIRDJS)->EnableWindow(FALSE);	// 2006. 12. 14 ������ : �������������߰�
		m_ctlBtnPremier.EnableWindow(FALSE);
		m_ctlButtonKONEXWORK.EnableWindow(FALSE);		//2013.06.05 KSJ �ڳؽ� �߰�
		rect.SetRect(100 + GAPPLUS, 17, LISTCTRL_WIDTH + GAPPLUS, 323);
		m_ComboGroup.ShowWindow(SW_HIDE);
		HideButton(TRUE);
		m_ListCtrl.MoveWindow(rect);
		szText = "KOSPI 200 �ڵ� �ȳ�";
		ListKospiSort(31);
		break;
	case allCODE:
	case kkelwCODE:
		szText = "�ڵ� �ȳ�";
//		OnButtonAllcode();
		ShowTreeView();
		break;

	case kkCODE:
		GetDlgItem(IDC_BUTTON_ECNCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THIRDMARKET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSDAQWORK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSPIWORK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SINJU)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ELW)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SECTOR)->EnableWindow(FALSE);	// 2006. 07. 26 ����ȣ : �����߰�
		GetDlgItem(IDC_BUTTON_THIRDJS)->EnableWindow(FALSE);	// 2006. 12. 14 ������ : �������������߰�
		m_ctlBtnPremier.EnableWindow(FALSE);
		m_ctlButtonKONEXWORK.EnableWindow(FALSE);		//2013.06.05 KSJ �ڳؽ� �߰�
		szText = "�ڵ� �ȳ�";
//		OnButtonAllcode();
		break;
	}

	SetWindowText(szText);
}

HBRUSH CJongmukDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	DeleteObject(m_hBrush);

	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		m_hBrush = CreateSolidBrush(USER_BACKCOLOR);
		return m_hBrush;
	}
	return hbr;
}

void CJongmukDlg::BtnSetup()
{
	const short	shBtnColor = 245;
	const int nheight	= 9;
	CString sztext	= "����";

	for (int ii = IDC_BUTTON_FUTURE1 ; ii <= IDC_BUTTON_22 ; ii++)
	{
		((CButtonST*)GetDlgItem(ii))->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
		((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, USER_FOCUS_COR);
		((CButtonST*)GetDlgItem(ii))->SetWnd(this);
		((CButtonST*)GetDlgItem(ii))->SetBtnCursor(IDC_CURSOR_HAND);
	}

	//2013.06.05 KSJ  �ڳؽ� �ʱ�ȭ �߰�
	((CButtonST*)GetDlgItem(IDC_BUTTON_KONEXWORK))->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	((CButtonST*)GetDlgItem(IDC_BUTTON_KONEXWORK))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
	((CButtonST*)GetDlgItem(IDC_BUTTON_KONEXWORK))->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, USER_FOCUS_COR);
	((CButtonST*)GetDlgItem(IDC_BUTTON_KONEXWORK))->SetWnd(this);
	((CButtonST*)GetDlgItem(IDC_BUTTON_KONEXWORK))->SetBtnCursor(IDC_CURSOR_HAND);
	//2013.06.05 KSJ  �ڳؽ� �ʱ�ȭ �߰� END

}

void CJongmukDlg::OnSelchangeComboGroup()
{
	int item = m_ComboGroup.GetCurSel();

	if (item == CB_ERR)
		return;
	item = m_ComboGroup.GetItemData(item);

	switch (m_select)
	{
 	case ALLTYPE:
		ListAllWorkSort(item);
 		break;
	case KOSPITYPE:
		ListKospiSort(item);
 		break;
 	case KOSDAQTYPE:
 		ListKosDaqSort(item+200);
 		break;
//	case ELWTYPE:
//		switch(m_ewFilter)
//		{
		// case ewNormal:	SelectELW(1); break;
		// case ewKoba:	SelectELW(2); break;
		// case ewALL:
		// default:	SelectELW(0); break;
//		}
//		break;
	case REITSTYPE:
		ListReitsSort(item);
		break;
 	default:
 		break;
	}
//	ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc, 0);
}

bool CJongmukDlg::FileCheck(int fNo)
{
	bool rValue = true;

	CString filename, path;
	filename.Format("/PortFolio.i%02d", fNo);

	path = m_root + "/user/"
		+ m_user + filename;

	CFileFind finder;
	if (finder.FindFile(path))
		rValue = true;
	else
		rValue = false;

	return rValue;
}

void CJongmukDlg::AddItem(int row, CString szCode, CString szName)
{
	//ITEMS item{};
	//item.code = szCode;
	//item.name = szName;
	//item.date = "";
	//m_arList.SetAtGrow(row,item);

	_vCODE.push_back(std::move(std::make_pair(szCode, szName)));
}

// void CJongmukDlg::InsertItem(int row, CString szCode, CString szName)
// {
// 	LISTITEM* pitem;
// 	LVITEM item;
// 	//szName.TrimLeft();
// 	pitem = new LISTITEM;
//
// 	strcpy((char*)pitem->Code, szCode);
// 	strcpy((char*)pitem->Name, szName);
//
// 	item.mask    = LVIF_PARAM|LVIF_TEXT;
// 	item.pszText = (LPSTR)szCode.operator LPCTSTR();
// 	item.lParam  = (LPARAM)pitem;
// 	item.iItem   = row;
// 	item.iSubItem = 0;
// 	//item.cchTextMax = szCode.GetLength()+1;
//
// 	m_ListCtrl.InsertItem(row)
// 	m_ListCtrl.SetItemText(row, 1, szName);
// }

void CJongmukDlg::clearEditSearch()
{
	m_EditSearch.SetWindowText("");
	m_EditSearch.SetFocus();
}

void CJongmukDlg::OnClickListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	const int idx = m_ListCtrl.GetSelectionMark();

	if (m_kind == allCODE || (m_kind == indexCODE && m_Type % 100 == 99))
	{
		m_szCode = m_ListCtrl.GetItemText(idx, 0);
		m_szName = m_ListCtrl.GetItemText(idx, 1);
	}
	else
	{
		m_szCode = m_ListCtrl.GetItemText(idx, 0);
		m_szName = m_ListCtrl.GetItemText(idx, 1);
	}

	m_szName.TrimLeft();
	*pResult = 0;
}

void CJongmukDlg::InitBtn()
{
	for (int ii = IDC_BUTTON_ALLCODE ; ii <= IDC_BUTTON_ELW ; ii++)
	{
		((CButtonST*)GetDlgItem(ii))->SetBold(FALSE);
		((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
		((CButtonST*)GetDlgItem(ii))->SetSelected(FALSE);
	}

	// 2006. 07. 26 �������� �����ϸ鼭 �ϵ��ڵ�
	((CButtonST*)GetDlgItem(IDC_BUTTON_SECTOR))->SetBold(FALSE);
	((CButtonST*)GetDlgItem(IDC_BUTTON_SECTOR))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
	((CButtonST*)GetDlgItem(IDC_BUTTON_SECTOR))->SetSelected(FALSE);

	// 2006. 12. 14 ������ : �������������߰�
	((CButtonST*)GetDlgItem(IDC_BUTTON_THIRDJS))->SetBold(FALSE);
	((CButtonST*)GetDlgItem(IDC_BUTTON_THIRDJS))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
	((CButtonST*)GetDlgItem(IDC_BUTTON_THIRDJS))->SetSelected(FALSE);

	// 2010. 02. 17 �����̾� ����
	m_ctlBtnPremier.SetBold( FALSE );
	m_ctlBtnPremier.SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
	m_ctlBtnPremier.SetSelected( FALSE );

	// 2013.06.05 KSJ �ڳؽ� �����ϸ鼭 �ϵ��ڵ�
	m_ctlButtonKONEXWORK.SetBold( FALSE );
	m_ctlButtonKONEXWORK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
	m_ctlButtonKONEXWORK.SetSelected( FALSE );

}

void CJongmukDlg::OnDblclkListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnClickListCtrl(pNMHDR, pResult);
	OnOK();

	*pResult = 0;
}

BOOL CJongmukDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	static int row;

 	if (wParam == IDC_LIST_CTRL)// <-- ���⼭ �׸��� ��Ʈ���� ����..
 	{

 		switch(((LPNMHDR) lParam)->code)
 		{
 		case LVN_COLUMNCLICK:
			{
				const tagNMLISTVIEW* pnm = (LPNMLISTVIEW)lParam;
				listsort(pnm->iSubItem);
			}
 			break;
		default:
			break;
 		}
 	}
	return CDialog::OnNotify(wParam, lParam, pResult);


}

void CJongmukDlg::listsort(int column)
{
	m_gListsort = !m_gListsort;
	switch (column)
	{
	case 0:
		std::sort(_vCODE.begin(), _vCODE.end(), [](const auto item1, const auto item2)->bool {
			if (m_gListsort)
				return (item1.first.Compare(item2.first) > 0);
			else
				return (item1.first.Compare(item2.first) < 0);
			});
		break;
	case 1:
		std::sort(_vCODE.begin(), _vCODE.end(), [](const auto item1, const auto item2)->bool {
			if (m_gListsort)
				return (item1.second.Compare(item2.second) > 0);
			else
				return (item1.second.Compare(item2.second) < 0);
			});
		break;
	default:
		break;
	}
	m_ListCtrl.Invalidate();
}


void CJongmukDlg::ListRemoveAll(bool select)
{
//	LISTITEM* pitem;
//	int count = m_ListCtrl.GetItemCount();

	m_ListCtrl.SetColumnWidth(1, COLUMN1_WIDTH);
//	for (int ii = 0; ii < count; ii++)
//	{
//		pitem = (LISTITEM*)m_ListCtrl.GetItemData(ii);
//		delete pitem;
//	}
	m_ListCtrl.DeleteAllItems();
	_vCODE.clear();
	if (select)
		_vSCODE.clear();

//	m_arList.RemoveAll();
}

BOOL CJongmukDlg::LoadUPCode()
{
	m_arUPCODE.RemoveAll();
	if (m_arUPCODE.GetSize() > 0)
		return FALSE;

	CFile	file;
	int	size{};

	CString	path;
	struct	upcode* upc{};

	path.Format("%s/%s/%s", m_root, TABDIR, UPCODE);
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
		return FALSE;

	size = gsl::narrow_cast<int>(file.GetLength());
	
	file.Read(_sData.GetBuffer(size+ 1), size);
	_sData.ReleaseBuffer();
	file.Close();

	//struct upcode* pUpc{};
	//for (int ii = 0; ii < m_arUPCODE.GetSize(); ii++)
	//{
	//	pUpc = (struct upcode*)m_arUPCODE.GetAt(ii);
	//	delete pUpc;
	//}
	//m_arUPCODE.RemoveAll();

	size /= sizeof(struct upcode);
	upc = (struct upcode *)_sData.GetString();
	
	for (int ii = 0; ii < size; ii++, upc++)
	{
		//pUpc = new struct upcode;
		//CopyMemory(pUpc, upc, sizeof(struct upcode));

		m_arUPCODE.Add(upc);
	}
	
	GetExCode();
	return TRUE;
}

void CJongmukDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CPen	pen(PS_SOLID, 1, RGB(128, 128, 128)), *poldpen{};
	CFont* poldfont{};

	poldpen  = dc.SelectObject(&pen);
	poldfont = dc.SelectObject(&m_Font);

	dc.MoveTo(10, 120);
	dc.LineTo(80 + GAPPLUS, 120);
	dc.MoveTo(10, 181);
	dc.LineTo(80 + GAPPLUS, 181);
	dc.MoveTo(10, 215);
	dc.LineTo(80 + GAPPLUS, 215);
	dc.MoveTo(10, 250);
	dc.LineTo(80 + GAPPLUS, 250);

	dc.SelectObject(poldpen);
	dc.SelectObject(poldfont);
	pen.DeleteObject();

	// Do not call CDialog::OnPaint() for painting messages
}


BOOL CJongmukDlg::FindCode(int kind, CString szCode, int type, bool bNoDelete)
{	// �ܺο��� ȣ��
	int	ii{};
	bool	find{};
	CString	name, string;
	struct	hjcodex* hjc = nullptr;
	const CAxisCodeApp* pApp = (CAxisCodeApp *)AfxGetApp();

	szCode.TrimRight();
	switch (kind)
	{
	case whichTYPE:
	case typeforTAX:
		m_jongmuk = 0;

		// �ֽ�����

	//	if (pApp->m_hname.Lookup(szCode, (void *&)hjc))
		if (const auto ft = _mapCODE.find(szCode); ft != _mapCODE.end())
		{
			hjc = ft->second;
			name = CString(hjc->hnam, HNameLen).Trim();
			SetName(name);
			switch (hjc->kosd)
			{
			case jmKOSPI:
				m_jongmuk = kospiType;		break;
			case jmKOSDAQ:
				m_jongmuk = kosdaqType;		break;
			case jm3RD:
				m_jongmuk = thirdType;		break;
			case jmMUFND:
		//	case jmMUFND2:
				m_jongmuk = mufundType;		break;
			case jmREITS:
				m_jongmuk = reitsType;		break;
		//	case jmETF:
		//		m_jongmuk = 6;		break;
			}

			if( hjc->ssgb == jmETF)
				m_jongmuk = etfType;
			if (hjc->ssgb == jmSINJU || hjc->ssgb == jmSINJS)
				m_jongmuk = sinjuType;
			if (hjc->ssgb == jmHYFND)
				m_jongmuk = hyfundType;
			if (hjc->ssgb == jmELW)
				m_jongmuk = elwType;
			if (hjc->ssgb == jmFOREIGN)
				m_jongmuk = foreignType;
			if (hjc->ssgb == jmETN)//KSJ 2014.10.28 ETN �߰�
				m_jongmuk = etnType;

			//20191114 ��ȭ�鿡�� getcodetype�� ����ϴ� ���� �ʹ� ���Ƽ� kind�� typeforTAX�� �ִ� ��쿡�� Ÿ���� ����ȭ ����
			//SCREEN.GetName(199, �����ڵ�, 0)
			if(hjc->ssgb == jmKONEX && kind == typeforTAX)
		//	if(hjc->ssgb == jmKONEX )
				m_jongmuk = KONEXType;

//			DeleteAllData();
			return TRUE;
		}
		break;

	case codeNAME:
	case bondNAME:
		m_jongmuk = 0;
		if (const auto ft = _mapCODE.find(szCode); ft != _mapCODE.end())
		{
			hjc = ft->second;
			name = CString(hjc->hnam, HNameLen);
			name.TrimRight();
			SetName(name);
			m_jongmuk = hjc->kosd;
			if (!bNoDelete)
				DeleteAllData();
			return TRUE;
		}
		break;

	case indexNAME:
	{
		if (m_arUPCODE.GetSize() <= 0)
			return FALSE;

		struct	upcode* upc{};

		find = false;
		name.Empty();
		for (ii = 0; ii < m_arUPCODE.GetSize(); ii++)
		{
			upc = (struct upcode*)m_arUPCODE.GetAt(ii);

			switch (upc->jgub)
			{
			case upKOSPI:
			case upKOSPI200:
			case upKOSDAQ:
			case upKOSDAQ50:
			case upKOSPIx:
			case 5:			//  2013.06.11 KSJ �ڳؽ� �߰�
			case 6:			//  ��������
			case 7:			// �������� ����
			case 8:
			case 9:
				string.Format("%1d%02d", upc->jgub, upc->ucod);
				break;
			default:
				string.Empty();
				break;
			}

			if (!szCode.Compare(string))
			{
				name = CString(upc->hnam, UNameLen);
				find = true;
				break;
			}
		}

		if (!find)
		{
			for (ii = 0; ii < m_exCode[EXKOSPI].GetSize(); ii++)
			{
				if (!szCode.Compare(m_exCode[EXKOSPI][ii].code))
				{
					name = m_exCode[EXKOSPI][ii].name;
					find = true;
					break;
				}
			}
		}

		if (!find)
		{
			for (ii = 0; ii < m_exCode[EXKOSDAQ].GetSize(); ii++)
			{
				if (!szCode.Compare(m_exCode[EXKOSDAQ][ii].code))
				{
					name = m_exCode[EXKOSDAQ][ii].name;
					find = true;
					break;
				}
			}
		}

		if (find)
		{
			SetName(name);
			return TRUE;
		}
	}
		break;

	default:
		break;
	}

	return FALSE;
}


void CJongmukDlg::DeleteAllData()
{
	//if (m_fcB)
	//	delete [] m_fcB;
	//if (m_ocB)
	//	delete [] m_ocB;

	//for (int ii = 0 ; ii < m_arUSCODE.GetSize() ; ii++)
	//	delete m_arUSCODE[ii];

	//for (int ii = 0; ii < m_codeArr.GetSize(); ii++)
	//{
	//	m_codeArr.GetAt(ii)->RemoveAll();
	//	delete m_codeArr.GetAt(ii);
	//}

// updateX_20060218
	//for (int ii = 0; ii < m_arFCODE.GetSize(); ii++)
	//	delete m_arFCODE.GetAt(ii);

	//for (int ii = 0; ii < m_arOCODE.GetSize(); ii++)
	//	delete m_arOCODE.GetAt(ii);

	m_arrayELWJogiCode.RemoveAll();
	m_arrayPBCode.RemoveAll();
	m_arrayELWJogiCode.RemoveAll();
	m_arrayELWCode.RemoveAll();
	m_arrayELWCode.RemoveAll();
	m_arrayELWGenCode.RemoveAll();
	m_arrayELWCode.RemoveAll();
	m_arrayBASECode.clear();
	m_arUSCODE.clear();
	m_arFCODE.RemoveAll();
	m_arOCODE.RemoveAll();
	
	m_codeArr.clear();
	_sData.Empty();
}

void CJongmukDlg::SetListCtrl()
{
//	m_ListCtrl.SetFocus();
//	((CEdit*)GetDlgItem(IDC_EDIT_SEARCH))->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////////////////////

void CJongmukDlg::InitOption()
{
	CString	name;
	struct	fjcode* fjc{};

	m_ctlButtonECN.SetWindowText("�����ɼ�");
	m_ctlButtonTHIRD.SetWindowText("�ؿ�����");

	loadFjCode();
	loadOjCode();

	for (int ii = 0; ii < m_arFCODE.GetSize(); ii++)
	{
		fjc  = (struct fjcode *)m_arFCODE.GetAt(ii);
		name = CString(fjc->hnam, FNameLen);
		name.TrimRight();
		GetDlgItem(IDC_BUTTON_FUTURE1+ii)->SetWindowText(name);
	}
}

void CJongmukDlg::Hideall(int flag)
{
	m_ListCtrl.ShowWindow(SW_HIDE);
	m_ComboGroup.ShowWindow(SW_HIDE);

	m_CtlButFuture1.ShowWindow(SW_HIDE);
	m_CtlButFuture2.ShowWindow(SW_HIDE);
	m_CtlButFuture3.ShowWindow(SW_HIDE);
	m_CtlButFuture4.ShowWindow(SW_HIDE);
	m_ListCall.ShowWindow(SW_HIDE);
	m_ListPut.ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO1))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO2))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO3))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO4))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_STATIC_OPTION))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_STATIC_CALL))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_STATIC_PUT))->ShowWindow(SW_HIDE);

	switch (flag)
	{
	case 1: // ���� Button
		GetDlgItem(IDC_TREE_BUNRYU)->ShowWindow(SW_HIDE);

		m_CtlButFuture1.ShowWindow(SW_SHOW);
		m_CtlButFuture2.ShowWindow(SW_SHOW);
		m_CtlButFuture3.ShowWindow(SW_SHOW);
		m_CtlButFuture4.ShowWindow(SW_SHOW);
		m_ListCall.ShowWindow(SW_SHOW);
		m_ListPut.ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_RADIO1))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_RADIO2))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_RADIO3))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_RADIO4))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_STATIC_OPTION))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_STATIC_CALL))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_STATIC_PUT))->ShowWindow(SW_SHOW);
		break;
	case 0: // �ɼ�
		GetDlgItem(IDC_TREE_BUNRYU)->ShowWindow(SW_HIDE);
		m_ListCtrl.ShowWindow(SW_SHOW);
		m_ComboGroup.ShowWindow(SW_SHOW);
		break;
	case 2: // �ؿ�(��3����)
		GetDlgItem(IDC_TREE_BUNRYU)->ShowWindow(SW_HIDE);
		m_ListCtrl.ShowWindow(SW_SHOW);
		break;
	}
}

void CJongmukDlg::ShowTreeView()
{
	HideButton(FALSE);
	m_ListCtrl.ShowWindow(SW_SHOW);
	m_ComboGroup.ShowWindow(SW_SHOW);

	m_CtlButFuture1.ShowWindow(SW_HIDE);
	m_CtlButFuture2.ShowWindow(SW_HIDE);
	m_CtlButFuture3.ShowWindow(SW_HIDE);
	m_CtlButFuture4.ShowWindow(SW_HIDE);
	m_ListCall.ShowWindow(SW_HIDE);
	m_ListPut.ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO1))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO2))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO3))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO4))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_STATIC_OPTION))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_STATIC_CALL))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_STATIC_PUT))->ShowWindow(SW_HIDE);

	((CButton*)GetDlgItem(IDC_BUTTON_ALLCODE))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_KOSPICODE))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_KOSDAQCODE))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_THIRDMARKET))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_KOSPIWORK))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_KOSDAQWORK))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_SINJU))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_ELW))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_ECNCODE))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BUTTON_SECTOR))->ShowWindow(SW_HIDE);		// 2006. 07. 26 ����ȣ : �����߰�
	((CButton*)GetDlgItem(IDC_BUTTON_THIRDJS))->ShowWindow(SW_HIDE);	// 2006. 12. 14 ������ : �������������߰�
	m_ctlBtnPremier.ShowWindow(SW_HIDE);
	m_ctlButtonKONEXWORK.ShowWindow(SW_HIDE);							//2013.06.05 KSJ �ڳؽ� �߰�

	GetDlgItem(IDC_TREE_BUNRYU)->ShowWindow(SW_SHOW);
}

void CJongmukDlg::OnButtonFuture1()
{
	struct	fjcode*	fjc;
	CString	code, name;

	fjc  = (struct fjcode *)m_arFCODE.GetAt(0);
	code = CString(fjc->cod2, FCodeLen);
	code.TrimRight();
	name = CString(fjc->hnam, FNameLen);
	name.TrimRight();

	SetCode(code);
	SetName(name);
}

void CJongmukDlg::OnButtonFuture2()
{
	struct	fjcode*	fjc;
	CString	code, name;

	fjc  = (struct fjcode *)m_arFCODE.GetAt(1);
	code = CString(fjc->cod2, FCodeLen);
	code.TrimRight();
	name = CString(fjc->hnam, FNameLen);
	name.TrimRight();

	SetCode(code);
	SetName(name);
}

void CJongmukDlg::OnButtonFuture3()
{
	struct	fjcode*	fjc;
	CString	code, name;

	fjc  = (struct fjcode *)m_arFCODE.GetAt(2);
	code = CString(fjc->cod2, FCodeLen);
	code.TrimRight();
	name = CString(fjc->hnam, FNameLen);
	name.TrimRight();

	SetCode(code);
	SetName(name);
}

void CJongmukDlg::OnButtonFuture4()
{
	struct	fjcode*	fjc;
	CString	code, name;

	fjc  = (struct fjcode *)m_arFCODE.GetAt(3);
	code = CString(fjc->cod2, FCodeLen);
	code.TrimRight();
	name = CString(fjc->hnam, FNameLen);
	name.TrimRight();

	SetCode(code);
	SetName(name);
}

BOOL CJongmukDlg::loadFjCode()
{
	//if (m_arFCODE.GetSize() > 0)
	//	return FALSE;

	//CFile	file;
	//CString path;
	//int	size{};

	//if (m_fcB)
	//{
	//	delete [] m_fcB; m_fcB = nullptr;
	//}

	//path.Format("%s/%s/%s", m_root, TABDIR, FJCODE);
	//if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	//{
	//	MessageBox("������ �������� �ʽ��ϴ�.");
	//	return FALSE;
	//}

	//size  = gsl::narrow_cast<int>(file.GetLength());
	//m_fcB = new char[size];
	//file.Read(m_fcB, size);
	//file.Close();
	//size /= sizeof(struct fjcode);

	//struct  fjcode* fjc = (struct fjcode *)m_fcB;
	//for (int ii = 0; ii < size; ii++, fjc++)
	//	m_arFCODE.Add(fjc);

	//if (m_kind == allCODE && m_Type == 10)
	//{
	//	fjc = new struct fjcode();
	//	xxCopy(fjc->cod2, "99999999");
	//	xxCopy(fjc->hnam, "���ἱ��");
	//	xxCopy(fjc->enam, "yungyul sunmul");
	//	fjc->mchk = 0;
	//	m_arFCODE.Add(fjc);
	//}

	return TRUE;
}

void CJongmukDlg::OnDoubleclickedButtonFuture1()
{
	OnButtonFuture1();
	OnOK();
}

void CJongmukDlg::OnDoubleclickedButtonFuture2()
{
	OnButtonFuture2();
	OnOK();
}

void CJongmukDlg::OnDoubleclickedButtonFuture3()
{
	OnButtonFuture3();
	OnOK();
}

void CJongmukDlg::OnDoubleclickedButtonFuture4()
{
	OnButtonFuture4();
	OnOK();
}

BOOL CJongmukDlg::loadOjCode()
{
	//if (m_arOCODE.GetSize() > 0)
	//	return TRUE;

	//CFile	file;
	//int	size{};
	//CString	string;
	//struct  ojcodh* ojh{};
	//struct  ojcode* ojc{};

	//if (m_ocB)
	//{
	//	delete[] m_ocB; m_ocB = nullptr;
	//}

	//string.Format("%s/%s/%s", m_root, TABDIR, OJCODE);
	//if (!file.Open(string, CFile::modeRead|CFile::typeBinary))
	//{
	//	MessageBox("������ �������� �ʽ��ϴ�.");
	//	return FALSE;
	//}

	//size  = gsl::narrow_cast<int>(file.GetLength());
	//m_ocB = new char[size]();
	//file.Read(m_ocB, size);
	//file.Close();

	//m_arMONTH.RemoveAll();
	//ojh = (struct ojcodh *)m_ocB;
	//for (int jj = 0; jj < 11; jj++)
	//{
	//	string = CString(ojh->cjym[jj], 6).Right(4);
	//	m_arMONTH.Add(string);
	//}

	//if (m_kind != codeNAME)
	//{
	//	if (((CButton*)GetDlgItem(IDC_RADIO1)))
	//	{
	//		((CButton*)GetDlgItem(IDC_RADIO1))->SetWindowText(m_arMONTH.GetAt(3));
	//		((CButton*)GetDlgItem(IDC_RADIO2))->SetWindowText(m_arMONTH.GetAt(2));
	//		((CButton*)GetDlgItem(IDC_RADIO3))->SetWindowText(m_arMONTH.GetAt(1));
	//		((CButton*)GetDlgItem(IDC_RADIO4))->SetWindowText(m_arMONTH.GetAt(0));
	//	}
	//}

	//size -= sizeof(struct ojcodh);
	//size /= sizeof(struct ojcode);

	//ojc = (struct ojcode *)&m_ocB[sizeof(struct ojcodh)];
	//for (int ii = 0; ii < size; ii++, ojc++)
	//	m_arOCODE.Add(ojc);

	return TRUE;
}

void CJongmukDlg::Filllistoption(int sel)
{
	int	csel{};
	struct  ojcode* ojc{};
	CString szCall, szPut, szCallnam, szPutnam;

	csel = sel - 3;
	csel = abs(csel);
	DeleteAllItem();
	for (int ii = 0, count = 0; ii < m_arOCODE.GetSize(); ii++)
	{
		ojc = (struct ojcode *)m_arOCODE.GetAt(ii);
		if (ojc->call[csel].yorn == '1')
		{
			szCall = CString(ojc->call[csel].cod2, 8);
			szPut  = CString(ojc->put[sel].cod2, 8);
			szCall.TrimRight(); szPut.TrimRight();

			szCallnam = CString(ojc->call[csel].hnam, ONameLen);
			szPutnam  = CString(ojc->put[sel].hnam, ONameLen);
			szCallnam.TrimRight(); szPutnam.TrimRight();

			szCall += ' ';
			szCall += szCallnam;
			szPut  += ' ';
			szPut  += szPutnam;

			m_ListCall.InsertString(count, szCall);
			m_ListPut.InsertString(count, szPut);
			count++;
		}
	}
}

void CJongmukDlg::DeleteAllItem()
{
	m_ListCall.ResetContent();
	m_ListPut.ResetContent();
}

void CJongmukDlg::OnDblclkListCall()
{
	CString szTemp;
	const int item = m_ListCall.GetCurSel();

	m_ListCall.GetText(item , szTemp);

	SetCode(Parser(szTemp, " "));
	SetName(szTemp);

	OnOK();
}

void CJongmukDlg::OnDblclkListPut()
{
	CString szTemp;
	const int item = m_ListPut.GetCurSel();

	m_ListPut.GetText(item , szTemp);

	SetCode(Parser(szTemp, " "));
	SetName(szTemp);

	OnOK();
}

CString CJongmukDlg::Parser(CString &srcstr, CString substr)
{
	CString temp;

	if (srcstr.Find(substr) == -1)
	{
		temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}

	return "";
}

bool CJongmukDlg::CheckDigit(CString code)
{
	bool rValue = true;

	for (int ii = 0 ; ii < code.GetLength() ; ii++)
	{
		const char c = code[ii];
		if (!isdigit(c))
		{
			rValue = false;
			break;
		}
	}

	return rValue;
}

CString CJongmukDlg::CompareFuture(CString szCode)
{
	CString rValue = _T("");
	CString sgCode, sgName, G_CODE, G_NAME;

	sgCode = KPI200;
	sgName.Format("%s�ڽ���200", STR_KOSPI);
	G_CODE = CODE_DOW;
	G_NAME = _T("�ٿ�����");

	rValue.Format("%s\t%s\t%s\t%s", sgCode, sgName, G_CODE, G_NAME);

	return rValue;
}

//CString CJongmukDlg::CompareQuote(CString szCode)
//{
//	CString	rValue;
//	int kosd{}, size{}, ucdm{}, jjug{}, ucds{};
//
//	const auto ft = _mapCODE.find(szCode);
//	if (ft != _mapCODE.end())
//	{
//		kosd = (int)ft->second->kosd;
//		size = (int)ft->second->size;
//		jjug = (int)ft->second->jjug;
//
////		ucds = (int)ft->second->ucds;
////		ucdm = (int)ft->second->ucdm;
//
//	}
//	else
//		return _T("");
//	//if (pApp->m_hname.Lookup(szCode, (void *&)hjc))
//	//{
//	//	kosd = (int)hjc->kosd;
//	//	size = (int)hjc->size;
//	//	ucdm = (int)hjc->ucdm;
//	//	jjug = (int)hjc->jjug;
//	//	ucds = (int)hjc->ucds;
//	//}
//	//else
//	//	return _T("");
//
//	int	comp = -1, upgub = -1;
//	CString F_UPCODE, G_NAME, G_CODE, G_STR;
//
//	if (kosd == jmKOSPI)
//	{
//		upgub    = upKOSPI;
//		F_UPCODE = UPCODE_KOSPI;
//		G_CODE   = KPI_TOTAL;
//		G_STR    = STR_KOSPI;
//		G_NAME.Format("%s�����ְ�����", STR_KOSPI);
//
//		// ������(27)�� ����
//		if (ucds < 99)				comp = ucds;
//		else if (jjug < 99 && jjug != 27)	comp = jjug;
//		else if (ucdm < 99)			comp = ucdm;
//		else if (size < 99)			comp = size;
//	}
//	else if (kosd == jmKOSDAQ)
//	{
//		upgub    = upKOSDAQ;
//		F_UPCODE = UPCODE_KOSDAQ;
//		G_CODE   = KDQ_TOTAL;
//		G_STR    = STR_KOSDAQ;
//		G_NAME.Format("%s��������", STR_KOSDAQ);
//
//		if (ucds < 99)			comp = ucds;
//		else if (ucdm < 99)		comp = ucdm;
//		else if (size < 99)		comp = size;
//		else if (jjug < 99)		comp = jjug;
//	}
//
//	CString	sgCode;
//	struct	upcode* upc{};
//
//	rValue.Empty();
//	for (int ii = 0 ; ii < m_arUPCODE.GetSize() ; ii++)
//	{
//		upc = (struct upcode *)m_arUPCODE.GetAt(ii);
//		if (upc->ucod == comp && upc->jgub == upgub)
//		{
//			sgCode.Format("%s%02d", F_UPCODE, comp);
//			rValue.Format("%s\t%s%s\t%s\t%s", sgCode, G_STR, CString(upc->hnam, UNameLen), G_CODE, G_NAME);
//			break;
//		}
//	}
//
//	return rValue;
//}

bool CJongmukDlg::IsCodeType(CString code, int type)
{
	const auto ft = _mapCODE.find(code);
	if (ft == _mapCODE.end())
		return false;

	if ((type == 1 && ft->second->ecng == 1)		// ECN
		|| (type == 2 && ft->second->ssgb == jmETF))	// ETF
			return true;
	return false;
}

void CJongmukDlg::Set_KS_HanMode()
{
	HIMC himc = ImmGetContext(GetSafeHwnd());
	ImmSetConversionStatus(himc, IME_CMODE_NATIVE, IME_SMODE_CONVERSATION);
	ImmReleaseContext(GetSafeHwnd(), himc);
}

//int CALLBACK ListViewCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
//{
//	const _listItem *pItem1 = (_listItem *) lParam1;
//	const _listItem *pItem2 = (_listItem *) lParam2;
//
//	int	iCmp{};
//
//	switch ((int)lParamSort)
//	{
//	case 0:
//		iCmp = lstrcmpi((char*)pItem1->Code, (char*)pItem2->Code);
//		break;
//	case 1:
//		{
//			CString s1((char*)pItem1->Name), s2((char*)pItem2->Name);
//			s1.TrimLeft(); s2.TrimLeft();
//			iCmp = lstrcmpi(s1, s2);
//			//iCmp = lstrcmpi((char*)pItem1->Name, (char*)pItem2->Name);
//		}
//		break;
//	}
//
//	if (m_gListsort)
//	{
//		if (iCmp > 0)
//			iCmp = -1;
//		else if (iCmp < 0)
//			iCmp = 1;
//	}
//	return iCmp;
//}

//void CJongmukDlg::OnButtonElw()
//{
//	CPtrArray arCODE;
//	CString	string;
//	struct	hjcode*	hjc;
//
//	m_gListsort = FALSE;
//	clearEditSearch();
//	Hideall(0);
//
//	InitBtn();
//	m_ctlButtonELW.SetBold(TRUE);
//	m_ctlButtonELW.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
//	m_ctlButtonELW.SetSelected(TRUE);
//
//	m_select = OTHERTYPE;
//	m_ComboGroup.ShowWindow(SW_SHOW);
//	MoveGrid();
//	HideButton();
//
//	arCODE.RemoveAll();
//	for (int ii = 0; ii < m_arHCODE.GetSize(); ii++)
//	{
//		hjc = (struct hjcode *)m_arHCODE.GetAt(ii);
//		if (hjc->ssgb == jmELW)
//			arCODE.Add(hjc);
//	}
//
//	CodeListSome(arCODE);
//	m_ListCtrl.Selected(0);
//	m_szCode = m_ListCtrl.GetItemText(0, 0);
//	m_szName = m_ListCtrl.GetItemText(0, 1);
//	m_szName.TrimLeft();
//	ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc, 0);
//}

CString getMidString(char *pStr, int nFirst, int nLen)
{
	if ((int)strlen(pStr + nFirst) < nLen)
		nLen = strlen(pStr + nFirst);
	return CString(pStr + nFirst, nLen);
}

CString CJongmukDlg::LoadCodeList(int nGroup)
{
// updateX_20051224
	struct _inters	inter;

	CString dir, sRtn;
	dir.Format("%s\\user\\%s\\portfolio.i%02d", m_root, m_user, nGroup);
	TRY
	{
		CFile file(dir, CFile::modeRead);
		UINT nBytesRead;
		do
		{
			nBytesRead = file.Read(&inter, sizeof(_inters));
			if (nBytesRead == sizeof(inter))
			{
				CString sCode = getMidString(inter.code, 0, 12);
				sCode.TrimLeft();
				if (!sCode.IsEmpty())
				{
					sRtn += sCode;
					sRtn += '\t';
				}
			}
		}while((int)nBytesRead);
		file.Close();
		return sRtn;
	}CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
		return "";
	}
	END_CATCH
		return "";
}

void CJongmukDlg::LoadFolder(CString path, HTREEITEM hItem, CString sSection, int nKey)
{
	char	wb[2048]{};
	CString	tmps;
	std::unique_ptr<CStringArray> pStrArr;

	for (int ii = 0; ; ii++)
	{
		HTREEITEM	hItem1{};
		tmps.Format("%02d", ii);
		GetPrivateProfileString("FOLDER", tmps, "", wb, sizeof(wb), path);
		tmps = wb;

		if (tmps.IsEmpty())
			break;

		hItem1 = m_Tree.InsertItem(Parser(tmps, ";"), 0, 1, hItem);
		pStrArr = std::make_unique<CStringArray>();

		//tmps.Format("%02d", ii);
		GetPrivateProfileString(sSection, tmps, "", wb, sizeof(wb), path);
		tmps = wb;

		if (tmps.IsEmpty())
		{
			m_Tree.DeleteItem(hItem1);
			pStrArr.reset(); 
			continue;
		}


		while (!tmps.IsEmpty())
		{
			CString sCode = Parser(tmps, ";");

			if (!sCode.IsEmpty())
				pStrArr->Add(sCode);

			if (tmps.IsEmpty())
			{
				m_codeArr.emplace_back(std::move(pStrArr));
				m_Tree.SetItemData(hItem1, MAKELONG(m_codeArr.size() - 1, nKey));
				break;
			}
		}
	}
}

// updateX_20060218
void CJongmukDlg::LoadGroup(CString path, HTREEITEM hItem)
{
	int readL = 0; char readB[256];
	CString	string, gno, gname, data;

	readL = GetPrivateProfileString(_T("GROUPORDER"), "00", "", readB, sizeof(readB), path);
	if (readL <= 0)	return;

	string = CString(readB, readL);

	while (!string.IsEmpty())
	{
		gno = Parser(string, ";");
		if (gno.IsEmpty() || gno.GetLength() != 2)
			break;

		readL = GetPrivateProfileString(_T("GROUPNAME"), gno, "", readB, sizeof(readB), path);
		gname = CString(readB, readL);

		if (!gname.IsEmpty())
		{
			HTREEITEM hItem1;
			hItem1 =m_Tree.InsertItem(gname, 0, 1, hItem);
			m_Tree.SetItemData(hItem1, MAKELONG(atoi(gno), tree_ks));
		}
	}
}

/*
void CJongmukDlg::LoadGroup(CString path, HTREEITEM hItem)
{
	char twb[1024];
	CString sTmp;

	for (int ii = 0; ii < 100; ii++)
	{
		sTmp.Format("%02d", ii);
		GetPrivateProfileString("GROUPNAME", sTmp, "", twb, sizeof(twb), path);
		sTmp = twb;

		if (!sTmp.IsEmpty())
		{
			HTREEITEM hItem1;
			hItem1 =m_Tree.InsertItem(sTmp, 0, 1, hItem);
			m_Tree.SetItemData(hItem1, MAKELONG(ii, tree_ks));
		}
	}
}
*/

void CJongmukDlg::baseCX_Category()
{
	CString dir;
	HTREEITEM hItem1{}, hItem2{}, hItem3{};
	CString code, name;

	dir.Format("%s\\tab\\", m_root);

	hItem1 = m_Tree.InsertItem("������",0,1);

	m_Tree.SetItemData(hItem1, MAKELONG(0, tree_jjm));
	//�ŷ��� ������
	hItem1 = m_Tree.InsertItem("�ŷ���",0,1);
	m_Tree.SetItemData(hItem1, MAKELONG(0, tree_kspjjm));

	//�ڽ��� ������
	hItem1 = m_Tree.InsertItem("�ڽ���",0,1);
	m_Tree.SetItemData(hItem1, MAKELONG(0, tree_ksdjjm));

	//2013.06.05 KSJ �ڳؽ� �߰�
	hItem1 = m_Tree.InsertItem("�ڳؽ�",0,1);
	m_Tree.SetItemData(hItem1, MAKELONG(0, tree_konex));
	//2013.06.05 KSJ �ڳؽ� �߰� END

	LoadUPCode();

	hItem1 = m_Tree.InsertItem("�ŷ���", 0, 1, TVI_ROOT);
	m_Tree.SetItemData(hItem1, MAKELONG(0, -1));
	hItem2 = m_Tree.InsertItem("�ڽ���", 0, 1, TVI_ROOT);
	m_Tree.SetItemData(hItem2, MAKELONG(0, -1));
	for (int ii = 0; ii< m_arUPCODE.GetSize(); ii++)
	{
		CString		name, dta;

		const upcode *pUpCode = (upcode*)m_arUPCODE.GetAt(ii);

		if( !pUpCode)
			break;

		if(pUpCode->jgub == upKOSDAQ50 && (pUpCode->ucod > 5 && pUpCode->ucod < 18))	continue;	//2016.08.25 KSJ ���������� ���� ������ ����Ʈ���� ���ش�.

		name = CString(pUpCode->hnam, UNameLen);
		const int res = name.Find('\0');
		if (res != -1)
			name = name.Left(res);

		name.TrimRight();

		dta.Format("%d%02d", (int)pUpCode->jgub, (int)pUpCode->ucod);

		if (pUpCode->jgub == upKOSPI || pUpCode->jgub == upINTEGRATE ||
			(pUpCode->jgub == upKOSPI200 && pUpCode->ucod == 1) ||
			pUpCode->jgub == upKOSPIx)
			m_Tree.SetItemData(m_Tree.InsertItem(name, 0, 1, hItem1), MAKELONG(atoi(dta), tree_kspup));
		else if (pUpCode->jgub == upKOSDAQ ||
			pUpCode->jgub == upKOSDAQ50)
		{
			m_Tree.SetItemData(m_Tree.InsertItem(name, 0, 1, hItem2), MAKELONG(atoi(dta), tree_ksdup));
		}
	}

	//KOSPI200
	hItem1 = m_Tree.InsertItem("KOSPI200",0,1);
	m_Tree.SetItemData(hItem1, MAKELONG(0, tree_ksp200));
	/*
	//KOSDAQ50
	hItem1 = m_Tree.InsertItem("KOSDAQ50",0,1);
	m_Tree.SetItemData(hItem1, MAKELONG(0, tree_ksd50));
	*/
	//KRX100
	hItem1 = m_Tree.InsertItem("KRX100",0,1);
	m_Tree.SetItemData(hItem1, MAKELONG(0, tree_krx100));

	//��������
	hItem1 = m_Tree.InsertItem("K-OTC",0,1);
	m_Tree.SetItemData(hItem1, MAKELONG(0, tree_frbd));

// 	if (m_kind == allCODE || m_kind == kospiCODE)
// 	{
// 		//ETF
// 		hItem1 = m_Tree.InsertItem("ETF",0,1);
// 		m_Tree.SetItemData(hItem1, MAKELONG(0, tree_etf));
// 	}

	//ETF
	hItem1 = m_Tree.InsertItem("ETF",0,1);
	m_Tree.SetItemData(hItem1, MAKELONG(0, 10000));

	InitialETFTree(hItem1);			//	 ETF Ʈ�� ����

	//ETN 2014.10.27 KSJ ETN �߰�
	hItem1 = m_Tree.InsertItem("ETN",0,1);
	m_Tree.SetItemData(hItem1, MAKELONG(0, tree_etn));

	//ELW
 	hItem1 = m_Tree.InsertItem("ELW",0,1);
 	m_Tree.SetItemData(hItem1, MAKELONG(0, 1000));

	hItem2 = m_Tree.InsertItem("������", 0, 1, hItem1);
	m_Tree.SetItemData(hItem2, MAKELONG(0, 1000));

	hItem3 = m_Tree.InsertItem("�Ϲ�ELW", 0, 1, hItem2);
	m_Tree.SetItemData(hItem3, MAKELONG(0, tree_elw_gencode));

	hItem3 = m_Tree.InsertItem("��������ELW", 0, 1, hItem2);
	m_Tree.SetItemData(hItem3, MAKELONG(0, tree_elw_jogicode));

	hItem2 = m_Tree.InsertItem("�����ڻ�����", 0, 1, hItem1);
	m_Tree.SetItemData(hItem2, MAKELONG(0, 1000));

	hItem3 = m_Tree.InsertItem("KOSPI200", 0, 1, hItem2);
	m_Tree.SetItemData(hItem3,  MAKELONG(0, tree_elw_base));

	hItem3 = m_Tree.InsertItem("KOSDAQ150", 0, 1, hItem2);	//2016.12.26 KSJ �ڽ���150 �߰�
	m_Tree.SetItemData(hItem3,  MAKELONG(0, tree_elw_base));

	hItem3 = m_Tree.InsertItem("BASKET", 0, 1, hItem2);
	m_Tree.SetItemData(hItem3, MAKELONG(0, tree_elw_base));


	const int nbasecode = m_arrayBASECode.size();

	for(int ii = 0; ii < nbasecode; ii++)
	{
		_basecode basecode = m_arrayBASECode.at(ii);

		code = basecode.code;
		name = basecode.name;
		name.TrimLeft(); name.TrimRight();
		hItem3 = m_Tree.InsertItem(name, 0, 1, hItem2);
		m_Tree.SetItemData(hItem3, MAKELONG(0, tree_elw_base));
	}

	hItem2 = m_Tree.InsertItem("�����", 0, 1, hItem1);
	m_Tree.SetItemData(hItem2, MAKELONG(0, 1000));

	const int nPbcode = m_arrayPBCode.GetSize();

	for(int ii = 0; ii < nPbcode; ii++)
	{
		_pbcode pbcode = m_arrayPBCode.GetAt(ii);

		code = pbcode.code;
		name = pbcode.name;

		name.TrimLeft("+"); name.TrimRight();
		hItem3 = m_Tree.InsertItem(name, 0, 1, hItem2);
		m_Tree.SetItemData(hItem3, MAKELONG(0, tree_elw));
	}

	//ETN 2015.11.04 KSJ spac �߰�
	hItem1 = m_Tree.InsertItem("����",0,1);
	m_Tree.SetItemData(hItem1, MAKELONG(0, tree_spac));

	//�ܱ���
	hItem1 = m_Tree.InsertItem("�ܱ���",0,1);
	m_Tree.SetItemData(hItem1, MAKELONG(0, tree_for));



	//�������� ���� ������ ���̻� �ҷ��ü� ����...
//	hItem1 = m_Tree.InsertItem("��������", 0, 1);
//	m_Tree.SetItemData(hItem1, MAKELONG(0, -1));
//	LoadGroup(m_root + "\\user\\" + m_user + "\\portFolio.ini", hItem1);

	//�׸�����
	hItem1 = m_Tree.InsertItem("��������", 0, 1);
	m_Tree.SetItemData(hItem1, MAKELONG(0, -1));
	LoadFolder(dir + "themcd.ini", hItem1, "TMCODE", tree_tm);

	//�׷�纰
	hItem1 = m_Tree.InsertItem("�׷�纰", 0, 1);
	m_Tree.SetItemData(hItem1, MAKELONG(0, -1));
	LoadFolder(dir + "gropcd.ini", hItem1, "GRCODE", tree_group);

	//��������
	hItem1 = m_Tree.InsertItem("��������", 0, 1);
	m_Tree.SetItemData(hItem1, MAKELONG(0, tree_reits));

	if (m_kind == reitsCODE)
		m_Tree.SelectItem(hItem1);
}

void CJongmukDlg::ProcPart(int ntype,int nSubType)
{
	CStringArray *pArr = nullptr;
	CString	string ="";
	CString code = "";

	std::vector<HCODEX*> vData;
	if (ntype == tree_group)//�׷��
	{
		m_gListsort = FALSE;
		clearEditSearch();

		m_select = 1;
		string = "��  ��%������%������%������%���ķ��%����, �Ǻ�%����, ����%ȭ  ��%�Ǿ�ǰ%��ݼӱ���%ö��, �ݼ�%��  ��%����, ����%�Ƿ�����%������%�����%���Ⱑ����%�Ǽ���%���â��%��ž�%������%��  ��%���� ������%��  ��%��  ��%���񽺾�%������%KOSPI 200%KOSPI 100%KOSPI 50%KOSPI IT%";
		AddCombo(string);
		m_ComboGroup.ShowWindow(SW_SHOW);
		MoveGrid();
		HideButton();

		if( pArr = m_codeArr.at(nSubType).get(); pArr == nullptr)
			return;

		for(int i = 0 ; i < pArr->GetSize(); i++)
		{
			code = pArr->GetAt(i);
			code.Trim();

			const auto ft = _mapCODE.find(code);
			if (ft != _mapCODE.end())
				vData.emplace_back(ft->second);
		}
		CodeListSome(vData);
	}
	else if ( ntype == tree_tm ) //�׸�����
	{
		m_gListsort = FALSE;
		clearEditSearch();

		m_select = 1;
		string = "��  ��%������%������%������%���ķ��%����, �Ǻ�%����, ����%ȭ  ��%�Ǿ�ǰ%��ݼӱ���%ö��, �ݼ�%��  ��%����, ����%�Ƿ�����%������%�����%���Ⱑ����%�Ǽ���%���â��%��ž�%������%��  ��%���� ������%��  ��%��  ��%���񽺾�%������%KOSPI 200%KOSPI 100%KOSPI 50%KOSPI IT%";
		AddCombo(string);
		m_ComboGroup.ShowWindow(SW_SHOW);
		MoveGrid();
		HideButton();
		if( pArr = m_codeArr.at(nSubType).get(); pArr == nullptr)
			return;

		for(int i = 0 ; i < pArr->GetSize(); i++)
		{
			code = pArr->GetAt(i);
			code.Trim();

			const auto ft = _mapCODE.find(code);
			if (ft != _mapCODE.end())
				vData.emplace_back(ft->second);
		}
		CodeListSome(vData);
	}else if( ntype == tree_kspup ) //�ڽ��� ����
	{
		m_gListsort = FALSE;
		clearEditSearch();

		m_select = 1;
		string = "��  ��%������%������%������%���ķ��%����, �Ǻ�%����, ����%ȭ  ��%�Ǿ�ǰ%��ݼӱ���%ö��, �ݼ�%��  ��%����, ����%�Ƿ�����%������%�����%���Ⱑ����%�Ǽ���%���â��%��ž�%������%��  ��%���� ������%��  ��%��  ��%���񽺾�%������%KOSPI 200%KOSPI 100%KOSPI 50%KOSPI IT%";
		AddCombo(string);
		m_ComboGroup.ShowWindow(SW_SHOW);
		MoveGrid();
		HideButton();
		ListKospiSort(nSubType);
	}
	else if ( ntype == tree_ks)	//��������
	{
		const int nGroup = nSubType;
		CString tmp = LoadCodeList(nGroup);
		m_gListsort = FALSE;
		clearEditSearch();

		m_select = -1;
		string = "��  ��%��  ó%��Ÿ����%IT����%IT��ó%��  ��%��  ��%��  ��%����, ����%��  ��%��  ��%��Ź��%IT S/W%IT H/W%���ķ�, ���%����, �Ƿ�%����, ����%����, ��ü%ȭ  ��%��  ��%��ݼ�%��  ��%������ %��������%�Ƿ�, ����%���, ��ǰ%��Ÿ ����%��ż���%��ۼ���%���ͳ�%������%����Ʈ����%��ǻ�� ����%������%�������%�ݵ�ü%IT��ǰ%KOSDAQ 100%KOSDAQ MID 300%KOSDAQ SMALL%����, ��ȭ%KOSDAQ 50%KOSDAQ IT%";//2016.05.16 KSJ KOSTAR ���� ����
		AddCombo(string);
		MoveGrid();
		m_ComboGroup.ShowWindow(SW_SHOW);
		HideButton();

		vector<CString> vInter;
		AxStd::SplitSub(vInter, tmp, "\t");

		for_each(vInter.begin(), vInter.end(), [&](auto code){
			code.Trim();
			if (code.IsEmpty() || code.GetLength() != 6)
				return;

//			switch (code.GetAt(0))
//			{
//			case '1': case '2': case '3': case '4':  //�ּ�
//				return; 
//			}

			const auto ft = _mapCODE.find(code);
			if (ft != _mapCODE.end())
				vData.emplace_back(ft->second);
		});
		CodeListSome(vData);
	}
	else if( ntype == tree_jjm ) //������
	{
		clearEditSearch();

		m_select = 0;
		m_ComboGroup.ShowWindow(SW_SHOW);
		CodeListAll();
		MoveGrid();
		HideButton();
	}
	else if( ntype == tree_kspjjm )	//�ڽ��� ������
	{

		m_gListsort = FALSE;
		clearEditSearch();

		m_select = 1;
		string = "��  ��%������%������%������%���ķ��%����, �Ǻ�%����, ����%ȭ  ��%�Ǿ�ǰ%��ݼӱ���%ö��, �ݼ�%��  ��%����, ����%�Ƿ�����%������%�����%���Ⱑ����%�Ǽ���%���â��%��ž�%������%��  ��%���� ������%��  ��%��  ��%���񽺾�%������%KOSPI 200%KOSPI 100%KOSPI 50%KOSPI IT%";
		AddCombo(string);
		m_ComboGroup.ShowWindow(SW_SHOW);
		MoveGrid();
		HideButton();

		for_each(_mapCODE.begin(), _mapCODE.end(), [&vData](auto item){
			if (item.second->kosd == jmKOSPI &&
				item.second->ssgb != jmSINJU &&
				item.second->ssgb != jmSINJS &&
				item.second->ssgb != jmHYFND &&
				item.second->ssgb != jmELW &&
				item.second->ssgb != jmFOREIGN)
				vData.emplace_back(item.second);
		});
		CodeListSome(vData);
	}
	else if( ntype == tree_ksdjjm )	//�ڽ��� ������
	{
		m_gListsort = FALSE;
		clearEditSearch();

		m_select = -1;
		string = "��  ��%��  ó%��Ÿ����%IT����%IT��ó%��  ��%��  ��%��  ��%����, ����%��  ��%��  ��%��Ź��%IT S/W%IT H/W%���ķ�, ���%����, �Ƿ�%����, ����%����, ��ü%ȭ  ��%��  ��%��ݼ�%��  ��%������ %��������%�Ƿ�, ����%���, ��ǰ%��Ÿ ����%��ż���%��ۼ���%���ͳ�%������%����Ʈ����%��ǻ�� ����%������%�������%�ݵ�ü%IT��ǰ%KOSDAQ 100%KOSDAQ MID 300%KOSDAQ SMALL%����, ��ȭ%KOSDAQ 50%KOSDAQ IT%";//2016.05.16 KSJ KOSTAR ���� ����
		AddCombo(string);
		MoveGrid();
		m_ComboGroup.ShowWindow(SW_SHOW);
		HideButton();

		for_each(_mapCODE.begin(), _mapCODE.end(), [&vData](auto item){
			if (item.second->kosd == jmKOSDAQ)
				vData.emplace_back(item.second);
		});
		CodeListSome(vData);
	}
	else if( ntype == tree_ksp200 )	//KOSPI200
	{
		m_gListsort = FALSE;
		clearEditSearch();

		m_select = 1;	//2014.12.18 KSJ -1���� 1�� ������.
		string = "��  ��%������%������%������%���ķ��%����, �Ǻ�%����, ����%ȭ  ��%�Ǿ�ǰ%��ݼӱ���%ö��, �ݼ�%��  ��%����, ����%�Ƿ�����%������%�����%���Ⱑ����%�Ǽ���%���â��%��ž�%������%��  ��%���� ������%��  ��%��  ��%���񽺾�%������%KOSPI 200%KOSPI 100%KOSPI 50%KOSPI IT%";
		AddCombo(string);
		MoveGrid();
		m_ComboGroup.ShowWindow(SW_SHOW);
		HideButton();

		ListKospiSort( atoi(KPI200));
	}
	else if( ntype == tree_ksd50 )	//KOSDAQ50
	{
		m_gListsort = FALSE;
		clearEditSearch();

		m_select = -1;
		string = "��  ��%��  ó%��Ÿ����%IT����%IT��ó%��  ��%��  ��%��  ��%����, ����%��  ��%��  ��%��Ź��%IT S/W%IT H/W%���ķ�, ���%����, �Ƿ�%����, ����%����, ��ü%ȭ  ��%��  ��%��ݼ�%��  ��%������ %��������%�Ƿ�, ����%���, ��ǰ%��Ÿ ����%��ż���%��ۼ���%���ͳ�%������%����Ʈ����%��ǻ�� ����%������%�������%�ݵ�ü%IT��ǰ%KOSDAQ 100%KOSDAQ MID 300%KOSDAQ SMALL%����, ��ȭ%KOSDAQ 50%KOSDAQ IT%";//2016.05.16 KSJ KOSTAR ���� ����
		AddCombo(string);
		MoveGrid();
		m_ComboGroup.ShowWindow(SW_SHOW);
		HideButton();


		for_each(_mapCODE.begin(), _mapCODE.end(), [&vData](auto item){
			if( item.second->kosd == jmKOSDAQ && item.second->kpgb == 1 )
				vData.emplace_back(item.second);
		});
		CodeListSome(vData);

		}
	else if( ntype == tree_krx100 )	//KRX100
	{
		m_gListsort = FALSE;
		clearEditSearch();

		m_select = -1;
		string = "��  ��%��  ó%��Ÿ����%IT����%IT��ó%��  ��%��  ��%��  ��%����, ����%��  ��%��  ��%��Ź��%IT S/W%IT H/W%���ķ�, ���%����, �Ƿ�%����, ����%����, ��ü%ȭ  ��%��  ��%��ݼ�%��  ��%������ %��������%�Ƿ�, ����%���, ��ǰ%��Ÿ ����%��ż���%��ۼ���%���ͳ�%������%����Ʈ����%��ǻ�� ����%������%�������%�ݵ�ü%IT��ǰ%KOSDAQ 100%KOSDAQ MID 300%KOSDAQ SMALL%����, ��ȭ%KOSDAQ 50%KOSDAQ IT%";//2016.05.16 KSJ KOSTAR ���� ����
		AddCombo(string);
		MoveGrid();
		m_ComboGroup.ShowWindow(SW_SHOW);
		HideButton();

		for_each(_mapCODE.begin(), _mapCODE.end(), [&vData](auto item){
			if( item.second->unio == 1)
				vData.emplace_back(item.second);
		});

		CodeListSome(vData);
	}
	else if( ntype == tree_frbd )	//��������
	{
		m_gListsort = TRUE;
		clearEditSearch();

		if (m_kind == all2CODE)
		{
			m_bOption = FALSE;
			Hideall(2);
			HideButton();
			MoveGrid(2);

			Invalidate();
			ListRemoveAll();
			LoadUSCode();
			_vCODE.clear();

			for_each(m_arUSCODE.begin(), m_arUSCODE.end(), [&](const auto item) {
				AddItem(0, CString(item->symb, sizeof(item->symb)), CString(item->knam, sizeof(item->knam)));
			});
			m_ListCtrl.SetItemCountEx(_vCODE.size());
			listsort(0);
			m_ListCtrl.Invalidate();
		}
		else
		{

			m_select = OTHERTYPE;
			m_ComboGroup.ShowWindow(SW_HIDE);
			MoveGrid(0);
			HideButton();

			for_each(_mapCODE.begin(), _mapCODE.end(), [&vData](auto item){
				if (item.second->kosd == jm3RD)
					vData.emplace_back(item.second);
			});
			CodeListSome(vData);

		}
	}
	else if( ntype == tree_etf )	//ETF
	{
		m_gListsort = FALSE;
		clearEditSearch();

		m_select = OTHERTYPE;
		m_ComboGroup.ShowWindow(SW_HIDE);
		MoveGrid(0);
		HideButton();

		for_each(_mapCODE.begin(), _mapCODE.end(), [&vData](auto item){
			if (item.second->ssgb == jmETF)
				vData.emplace_back(item.second);
		});
		CodeListSome(vData);
	}
	else if( ntype == tree_etf_item)			// ETF ����ȸ�纰
	{
		HTREEITEM hItem = m_Tree.GetSelectedItem();
		CString company = m_Tree.GetItemText(hItem);
		CString code = FindETFItem(company);

		ETFLoadItem(code);
	}
	else if( ntype == tree_etf_tema)			// ETF �׸���
	{
		HTREEITEM hItem = m_Tree.GetSelectedItem();
		CString tema = m_Tree.GetItemText(hItem);
		CString tmcd = FindETFTema(tema);

		ETFLoadTema(tmcd);
	}
	else if( ntype == tree_etf_fore)
	{
		HTREEITEM hItem = m_Tree.GetSelectedItem();
		CString forenm = m_Tree.GetItemText(hItem);
		CString frcd = FindETFFore(forenm);

		ETFLoadFore(frcd);

	}
	else if( ntype == tree_elw )			//ELW ����ȸ�纰
	{
		HTREEITEM hItem = m_Tree.GetSelectedItem();
		CString company = m_Tree.GetItemText(hItem);
		CString code = FindPBcompany(company);

		ApplyELW(code);
	}
	else if( ntype == tree_elw_base)	//ELW�����ڻ�
	{
		HTREEITEM hItem = m_Tree.GetSelectedItem();
		CString company = m_Tree.GetItemText(hItem);
		CString code;
		if(company.Compare("KOSPI200") == 0)
		{
			code = "KOSPI200";
			ApplyELWBase(code);
		}
		else if(company.Compare("KOSDAQ150") == 0)	//2016.12.26 KSJ KOSDAQ150 �߰�
		{
			code = "KOSDAQ150";
			ApplyELWBase(code);
		}
		else if(company.Compare("BASKET") == 0)
		{
			code = "BASKET";
			ApplyELWBase(code);
		}
		else
		{
			code = 	FindBaseCode(company);
			ApplyELWBase(code);
		}
	}
	else if( ntype == tree_elw_gencode)	// �Ϲ� ELW All
	{
		ElwLoadGen();
	}
	else if( ntype == tree_elw_jogicode)	// �������� ELW All
	{
		ElwLoadJogi();
	}
	else if( ntype == tree_for )	// �ܱ���
	{
		m_gListsort = FALSE;
		clearEditSearch();

		m_select = OTHERTYPE;
		m_ComboGroup.ShowWindow(SW_HIDE);
		MoveGrid(0);
		HideButton();

		for_each(_mapCODE.begin(), _mapCODE.end(), [&vData](auto item){
			if (item.second->ssgb == jmFOREIGN)
				vData.emplace_back(item.second);
		});

		CodeListSome(vData);
	}
	else if( ntype == tree_ksdup )	//�ڽ��� ����
	{
		m_gListsort = FALSE;
		clearEditSearch();

		m_select = -1;
		string = "��  ��%��  ó%��Ÿ����%IT����%IT��ó%��  ��%��  ��%��  ��%����, ����%��  ��%��  ��%��Ź��%IT S/W%IT H/W%���ķ�, ���%����, �Ƿ�%����, ����%����, ��ü%ȭ  ��%��  ��%��ݼ�%��  ��%������ %��������%�Ƿ�, ����%���, ��ǰ%��Ÿ ����%��ż���%��ۼ���%���ͳ�%������%����Ʈ����%��ǻ�� ����%������%�������%�ݵ�ü%IT��ǰ%KOSDAQ 100%KOSDAQ MID 300%KOSDAQ SMALL%����, ��ȭ%KOSDAQ 50%KOSDAQ IT%";//2016.05.16 KSJ KOSTAR ���� ����
		AddCombo(string);
		MoveGrid();
		m_ComboGroup.ShowWindow(SW_SHOW);
		HideButton();

		ListKosDaqSort( nSubType);
	}
	else if (ntype == tree_reits)
	{
		m_gListsort = FALSE;
		clearEditSearch();

		m_select = REITSTYPE;
		string = "������%��������%����ȸ��/��������%��������/����ȸ��%";
		AddCombo(string);
		MoveGrid();
		m_ComboGroup.ShowWindow(SW_SHOW);

		HideButton();

		if (m_kind == reitsCODE)
		{
			switch (m_Type)
			{
			case reitsallType:
				m_ComboGroup.SetCurSel(0);
				break;
			case reitsType:
				m_ComboGroup.SetCurSel(1);
				break;
			case shipType:
				m_ComboGroup.SetCurSel(2);
				break;
			case infraType:
				m_ComboGroup.SetCurSel(3);
				break;
			}

			for_each(_mapCODE.begin(), _mapCODE.end(), [&](auto item){
				if (m_Type == reitsallType && (item.second->ssgb == jmREITS || item.second->ssgb == jmSHIP || item.second->ssgb == jmINFRA))
					vData.emplace_back(item.second);
				else if ((m_Type == reitsType && item.second->ssgb == jmREITS) ||
					(m_Type == shipType && item.second->ssgb == jmSHIP) ||
					(m_Type == infraType && item.second->ssgb == jmINFRA))
						vData.emplace_back(item.second);
			});

		}
		else
		{

			for_each(_mapCODE.begin(), _mapCODE.end(), [&vData](auto item){
				if (item.second->ssgb == jmREITS || item.second->ssgb == jmSHIP || item.second->ssgb == jmINFRA)
					vData.emplace_back(item.second);
			});

		}

		CodeListSome(vData);
	}
	else if( ntype == tree_konex )	// 2013.06.05 KSJ �ڳؽ�
	{
		m_gListsort = FALSE;
		clearEditSearch();

		m_select = KONEXTYPE;
		m_ComboGroup.ShowWindow(SW_HIDE);
		MoveGrid(0);
		HideButton();

		for_each(_mapCODE.begin(), _mapCODE.end(), [&vData](auto item){
			if (item.second->ssgb == jmKONEX)
				vData.emplace_back(item.second);
		});
		CodeListSome(vData);
	}
	else if( ntype == tree_etn )	//ETN 2014.10.28 KSJ ETN �߰�
	{
		m_gListsort = FALSE;
		clearEditSearch();

		m_select = OTHERTYPE;
		m_ComboGroup.ShowWindow(SW_HIDE);
		MoveGrid(0);
		HideButton();

		for_each(_mapCODE.begin(), _mapCODE.end(), [&vData](auto item){
			if (item.second->ssgb == jmETN)
				vData.emplace_back(item.second);
		});
		CodeListSome(vData);
	}
	else if( ntype == tree_etfetn )	//ETN 2014.10.28 KSJ ETF/ETN ������ �߰�
	{
		m_gListsort = FALSE;
		clearEditSearch();

		m_select = OTHERTYPE;
		m_ComboGroup.ShowWindow(SW_HIDE);
		MoveGrid(0);
		HideButton();

		for_each(_mapCODE.begin(), _mapCODE.end(), [&vData](auto item){
			if (item.second->ssgb == jmETF || item.second->ssgb == jmETN)
				vData.emplace_back(item.second);
		});
		CodeListSome(vData);

	}
	else if( ntype == tree_spac )	//2015.11.04 KSJ spac �߰�
	{
		m_gListsort = FALSE;
		clearEditSearch();

		m_select = OTHERTYPE;
		m_ComboGroup.ShowWindow(SW_HIDE);
		MoveGrid(0);
		HideButton();

		for_each(_mapCODE.begin(), _mapCODE.end(), [&vData](auto item){
			if (item.second->ssgb == jmSPAC)
				vData.emplace_back(item.second);
		});
		CodeListSome(vData);

	}

	m_ListCtrl.Selected(0);
	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);
	m_szName.TrimLeft();
}

void CJongmukDlg::OnDblclkTreeBunryu(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
}

void CJongmukDlg::OnSelchangedTreeBunryu(NMHDR* pNMHDR, LRESULT* pResult)
{
	const NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hItem = m_Tree.GetSelectedItem();
	// �з��� �ε�
	if( hItem == nullptr)
		return;

	const int ntype =HIWORD(m_Tree.GetItemData(hItem));
	const int nSubType = LOWORD(m_Tree.GetItemData(hItem));
	if( ntype != 0xffff)
		ProcPart(ntype,nSubType);
	else
		ListRemoveAll();

	*pResult = 0;
}

void CJongmukDlg::OnButtonSector()
{
	CString	string;
	struct	upcode* upc{};
	CRect rect(100 + GAPPLUS, 20, LISTCTRL_WIDTH + GAPPLUS, 340);

	m_gListsort = TRUE;
	clearEditSearch();
	Hideall(0);

	InitBtn();
	m_CtlButtonSector.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
	m_CtlButtonSector.SetSelected(TRUE);

	m_ListCtrl.MoveWindow(rect);
        m_ComboGroup.ShowWindow(SW_HIDE);
	HideButton(TRUE);

	ListRemoveAll();
	LoadUPCode();

	if (m_kind == allCODE || (m_kind == indexCODE && m_Type % 100 == 99))
	{
		InitBtn();
		m_CtlButtonSector.SetBold(TRUE);
		m_CtlButtonSector.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
		m_CtlButtonSector.SetSelected(TRUE);
	}

	CString szCode,szName;

	_vCODE.clear();
	for (int ii = 0, count = 0; ii < m_arUPCODE.GetSize(); ii++)
	{
		upc = (struct upcode *)m_arUPCODE.GetAt(ii);
		if (m_Type == 10 || m_Type == 199)	// 40���� ���� ����
		{
			if (upc->jgub != 0 || upc->ucod >= 40)
				continue;
		}
		if (upc->jgub == 6)
		{
			string.Format("%d%02d", upc->jgub, upc->ucod);
			AddItem(count++, string, CString(upc->hnam, UNameLen));
		}
		else if(upc->jgub == 9)
		{
			string.Format("%d%02d", upc->jgub, upc->ucod);

			szCode = string;
			szName = CString(upc->hnam, UNameLen);
			AddItem(count++,szCode,szName);
		}
	}

	m_ListCtrl.SetItemCountEx(_vCODE.size());
	listsort(0);
	m_ListCtrl.Invalidate();
	m_ListCtrl.Selected(0);

	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);
	m_szName.TrimLeft();
}


void CJongmukDlg::OnButtonThirdjs()
{
	CString	string;
	struct	upcode* upc{};
	CRect rect(100 + GAPPLUS, 20, LISTCTRL_WIDTH + GAPPLUS, 340);

	m_gListsort = TRUE;
	clearEditSearch();
	Hideall(0);

	InitBtn();
	m_CtlButtonThirdJS.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
	m_CtlButtonThirdJS.SetSelected(TRUE);

	m_ListCtrl.MoveWindow(rect);
        m_ComboGroup.ShowWindow(SW_HIDE);
	HideButton(TRUE);

	ListRemoveAll();
	LoadUPCode();

	if (m_kind == allCODE || (m_kind == indexCODE && m_Type % 100 == 99))
	{
		InitBtn();
		m_CtlButtonThirdJS.SetBold(TRUE);
		m_CtlButtonThirdJS.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
		m_CtlButtonThirdJS.SetSelected(TRUE);
	}

	_vCODE.clear();
	for (int ii = 0, count = 0; ii < m_arUPCODE.GetSize(); ii++)
	{
		upc = (struct upcode *)m_arUPCODE.GetAt(ii);
		if (m_Type == 10 || m_Type == 199)	// 40���� ���� ����
		{
			if (upc->jgub != 0 || upc->ucod >= 40)
				continue;
		}
		if (upc->jgub == 7)
		{
			string.Format("%d%02d", upc->jgub, upc->ucod);
			AddItem(count++, string, CString(upc->hnam, UNameLen));
		}
	}

	m_ListCtrl.SetItemCountEx(_vCODE.size());
	listsort(0);
	//qsort(static_cast<void*>(&m_arList[0]), m_arList.GetSize(), sizeof( ITEMS), compareSortCode2);
	m_ListCtrl.Invalidate();
	m_ListCtrl.Selected(0);

	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);
	m_szName.TrimLeft();

}

void CJongmukDlg::searchAction(int column)
{
	CString	string;
	m_EditSearch.GetWindowText(string);
	if (column == 0)	// number search
	{
		if (!m_bAddAll)
		{

			vector<HCODEX*> vData;
			for_each(_vSCODE.begin(), _vSCODE.end(), [&vData](const auto item) {
				vData.emplace_back(std::move(item));
			});
			ListRemoveAll();
			CodeListSome(vData);
			m_bAddAll = true;
		}

		if (m_ListCtrl.GetItemCount() > 0)
		{
			int	idx = -1;
			if (string.GetLength() > 0)
			{
				m_gListsort = FALSE;

				idx = findMatchIndex(string, column);
				if (idx >= 0)
				{
					m_ListCtrl.Selected(idx);
					m_szCode = m_ListCtrl.GetItemText(idx, 0);
					m_szName = m_ListCtrl.GetItemText(idx, 1);
					m_szName.TrimLeft();
				}
			}
			else
			{
				m_ListCtrl.Selected(0);
				m_szCode = m_ListCtrl.GetItemText(0, 0);
				m_szName = m_ListCtrl.GetItemText(0, 1);
				m_szName.TrimLeft();
			}
		}
	}
	else			// name search
	{
		m_bAddAll = SearchData(m_ComboSearchKind.GetCurSel(), string, m_bAddAll);
		if (m_ListCtrl.GetItemCount() > 0)
		{
			m_gListsort = FALSE;
			listsort(column);   
			listsort(column); 
			m_ListCtrl.Selected(0);
			m_szCode = m_ListCtrl.GetItemText(0, 0);
			m_szName = m_ListCtrl.GetItemText(0, 1);
			m_szName.TrimLeft();
		}
	}
}

int CJongmukDlg::findMatchIndex(CString text, int column)
{
	CString	string;
	const int	textL = text.GetLength();
	for (int ii = 0; ii < m_ListCtrl.GetItemCount(); ii++)
	{
		string = m_ListCtrl.GetItemText(ii, column);
		if (column == 1) string = string.Mid(1);

		if (!strncmp(text, string, textL))
			return ii;
	}
	return -1;
}

bool CJongmukDlg::SearchData(int kind, CString sName, bool bAddAll)
{
	if (kind == SK_WORD)
		return SearchWord(sName, bAddAll);

	return SearchJongmok(sName, bAddAll);
}

bool CJongmukDlg::SearchJongmok(CString sName, bool bAddAll)
{
	CString	code, name;
	if (sName.IsEmpty())
	{	// add all
		if (!bAddAll)
		{
			vector<HCODEX*> vData;
			std::copy(_vSCODE.begin(), _vSCODE.end(), std::back_inserter(vData));
			ListRemoveAll();
			CodeListSome(vData);
		}
		return true;
	}

	ListRemoveAll(false);

	const int	idx = 0;
	char	cName{};
	const int	sLen = sName.GetLength();
	if (sLen == 1)
	{	// �Ϲݰ˻�����(���̸�ŭ �´°͸� ����)
		const int sLen = sName.GetLength();
		//m_arList.RemoveAll();
		_vCODE.clear();
		for_each(_vSCODE.begin(), _vSCODE.end(), [&](const auto hjc){
			name = CString(hjc->hnam, sizeof(hjc->hnam)).Mid(1);
			CString strUpperS, strUpperO;
			strUpperS = sName;
			strUpperO = name;
			strUpperS.MakeUpper();
			strUpperO.MakeUpper();

			if (strncmp(strUpperS, strUpperO, sLen) != 0)
				return;

			code = CString(hjc->code, HCodeLen).Mid(1);
			name.TrimRight();
			_vCODE.push_back(std::move(std::make_pair(code, name)));
		});

		m_ListCtrl.SetItemCountEx(_vCODE.size());
		listsort(1);
		m_ListCtrl.Invalidate();
	}
	else
	{	// �ѱ� �˻�
		WORD	wHangul{}, wHangul2{}, wStart{}, wEnd{};
		std::vector<HCODEX*> vSearch;

		_vCODE.clear();
		int ii = 0;

		cName = sName.GetAt(ii);
		if (cName & HANGULMSK)	// �ѱ��϶� 2Byte
		{
			wHangul = MakeHangul(sName.GetAt(ii), sName.GetAt(ii+1));
			if (IsChosung(wHangul, wStart, wEnd))
			{
				// �ʼ� ��
				for_each(_vSCODE.begin(), _vSCODE.end(), [&](const auto hjc){
					name = CString(hjc->hnam, sizeof(hjc->hnam)).Mid(1);
					if (name.GetLength() < 2)	return;
					wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
					if (wStart <= wHangul2 && wHangul2 <= wEnd)
						vSearch.push_back(hjc);
				});
			}
			else
			{	// �ѱ� ��
				for_each(_vSCODE.begin(), _vSCODE.end(), [&](const auto hjc){
					name = CString(hjc->hnam, sizeof(hjc->hnam)).Mid(1);
					if (name.GetLength() < 2)	return;
					wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii + 1));
					if (wHangul == wHangul2)
						vSearch.push_back(hjc);
				});
			}
			ii += 2;
		}
		else	// �׿ܴ� 1Byte
		{
			for_each(_vSCODE.begin(), _vSCODE.end(), [&](const auto hjc){
				name = CString(hjc->hnam, sizeof(hjc->hnam)).Mid(1);
				if (name.GetLength() < 1)	return;
				CString strUpperS, strUpperO;
				strUpperS = cName;
				strUpperO = name.GetAt(ii);
				strUpperS.MakeUpper();
				//strUpperO.MakeUpper();

				if (strUpperS.CompareNoCase(strUpperO) == 0)	vSearch.push_back(hjc);
			});

			ii += 1;
		}


		for (; ii < sLen && vSearch.size();)
		{
			cName = sName.GetAt(ii);
			if (cName & HANGULMSK)	// �ѱ��϶� 2Byte
			{
				wHangul = MakeHangul(sName.GetAt(ii), sName.GetAt(ii+1));
				if (IsChosung(wHangul, wStart, wEnd))
				{
					// �ʼ� ��
					auto it = std::remove_if(vSearch.begin(), vSearch.end(), [&](const auto hjc)->bool{
						name = CString(hjc->hnam, sizeof(hjc->hnam)).Mid(1);
						if (name.GetLength() < ii+2)
							return true;
						wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
						if (wStart > wHangul2 || wHangul2 > wEnd)
							return true;
						return false;
					});
					if (it != vSearch.end())
						vSearch.erase(it, vSearch.end());
				}
				else
				{	// �ѱ� ��
					auto it = std::remove_if(vSearch.begin(), vSearch.end(), [&](const auto hjc)->bool{
						name = CString(hjc->hnam, sizeof(hjc->hnam)).Mid(1);
						if (name.GetLength() < ii+2)
							return true;
						wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
						if (wHangul != wHangul2)
							return true;
						return false;
					});
					if (it != vSearch.end())
						vSearch.erase(it, vSearch.end());
				}
				ii += 2;
			}
			else			// �׿ܴ� 1Byte
			{
				auto it = std::remove_if(vSearch.begin(), vSearch.end(), [&](const auto hjc)->bool{
					name = CString(hjc->hnam, sizeof(hjc->hnam)).Mid(1);
					name.Trim();
					if (name.GetLength() < ii)
						return true;
					CString strUpperS, strUpperO;
					strUpperS = cName;


					strUpperO = name.GetAt(ii);
					strUpperS.MakeUpper();	
					
					if (strUpperO.GetAt(0) > 0 && 127 >= strUpperO.GetAt(0))
					{
						if (isalpha(strUpperO.GetAt(0)))
							strUpperO.MakeUpper();
					}
					if (strUpperS != strUpperO)
						return true;
					return false;
				});
				if (it != vSearch.end())
					vSearch.erase(it, vSearch.end());

				ii += 1;
			}
		}
		CodeListSome(vSearch, false);
	}

	return false;
}

bool CJongmukDlg::SearchWord(CString sName, bool bAddAll)
{
	std::vector<HCODEX*> vSearch;
	if (sName.IsEmpty())
	{	// add all
		if (!bAddAll)
		{
			ListRemoveAll(false);
			for_each(_vSCODE.begin(), _vSCODE.end(), [&vSearch](const auto hjc){
				vSearch.emplace_back(hjc);
			});
			CodeListSome(vSearch);
		}
		return true;
	}
	ListRemoveAll(false);

	for_each(_vSCODE.begin(), _vSCODE.end(), [&vSearch, &sName](const auto hjc){
		CString name = CString(hjc->hnam, sizeof(hjc->hnam)).Mid(1);
		if (name.Find(sName) >= 0)
			vSearch.emplace_back(hjc);
	});

	CodeListSome(vSearch, false);
	return false;
}

bool CJongmukDlg::IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd)
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

WORD CJongmukDlg::MakeHangul(char cChar1, char cChar2)
{	// 2Byte���ڷκ��� �ѱ��� �ͱ۾��ֶ�~~
	const WORD	wHangul = (cChar1<<8) | (0x00FF & cChar2);
	return wHangul;
}

void CJongmukDlg::OnSelchangeStaticSearch()
{
	CString	string;
	m_EditSearch.GetWindowText(string);

	if (string.IsEmpty())
		searchAction(0);
	else
		searchAction(1);

	m_EditSearch.SetFocus();
	m_EditSearch.SetSel(0, -1);
}



void CJongmukDlg::OnButtonPremier()
{
	// TODO: Add your control notification handler code here
	CString	string;
	struct	upcode* upc{};
	CRect rect(100 + GAPPLUS, 20, LISTCTRL_WIDTH + GAPPLUS, 340);

	m_gListsort = TRUE;
	clearEditSearch();
	Hideall(0);

	InitBtn();
	m_ctlBtnPremier.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
	m_ctlBtnPremier.SetSelected(TRUE);

	m_ListCtrl.MoveWindow(rect);
        m_ComboGroup.ShowWindow(SW_HIDE);
	HideButton(TRUE);

	ListRemoveAll();
	LoadUPCode();

	if (m_kind == allCODE || (m_kind == indexCODE && m_Type % 100 == 99))
	{
		InitBtn();
		m_ctlBtnPremier.SetBold(TRUE);
		m_ctlBtnPremier.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
		m_ctlBtnPremier.SetSelected(TRUE);
	}

	_vCODE.clear();
	for (int ii = 0, count = 0; ii < m_arUPCODE.GetSize(); ii++)
	{
		upc = (struct upcode *)m_arUPCODE.GetAt(ii);
		if (upc->jgub == 8 )
		{
			string.Format("%d%02d", upc->jgub, upc->ucod);
			AddItem(count++, string, CString(upc->hnam, UNameLen));
		}
	}

	m_ListCtrl.SetItemCountEx(_vCODE.size());
	listsort(0);
	m_ListCtrl.Invalidate();
	m_ListCtrl.Selected(0);

	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);
	m_szName.TrimLeft();

}

void CJongmukDlg::ElwLoadGen()
{
	m_gListsort = FALSE;
	clearEditSearch();

	m_select = OTHERTYPE;
	m_ComboGroup.ShowWindow(SW_HIDE);

	MoveGrid(0);
	HideButton();

 	_elwcode elwcode{};
  	struct	hjcodex* hjc = nullptr;
	std::vector<HCODEX*> vData;
	const int ncnt = m_arrayELWGenCode.GetSize();

	for(int ii = 0; ii < ncnt; ii++)
	{
		elwcode = m_arrayELWGenCode.GetAt(ii);
		hjc = FindOnlyELW(elwcode.code);
		if(hjc)
			vData.emplace_back(hjc);
	}

	LockWindowUpdate();
	CodeListSome(vData);
	UnlockWindowUpdate();

	m_ListCtrl.Selected(0);
	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);
	m_szName.TrimLeft();
}

void CJongmukDlg::ElwLoadJogi()
{
	m_gListsort = FALSE;
	clearEditSearch();

	m_select = OTHERTYPE;
	m_ComboGroup.ShowWindow(SW_HIDE);

	MoveGrid(0);
	HideButton();

	_elwcode elwcode;
	struct	hjcodex* hjc{};
	std::vector<HCODEX*> vData;


	const int ncnt = m_arrayELWJogiCode.GetSize();

	for(int ii = 0; ii < ncnt; ii++)
	{
		elwcode = m_arrayELWJogiCode.GetAt(ii);
		hjc = FindOnlyELW(elwcode.code);
		if(hjc)
			vData.emplace_back(hjc);
	}

	LockWindowUpdate();
	CodeListSome(vData);
	UnlockWindowUpdate();

	m_ListCtrl.Selected(0);
	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);
	m_szName.Trim();
}

CString CJongmukDlg::FindBaseCode(CString company)
{
	for(auto basecode : m_arrayBASECode)
	{	
		if(basecode.name.Compare(company) == 0)
			return basecode.code;
	}
	return "";
}

CString CJongmukDlg::FindPBcompany(CString company)
{
	PBCODE pbcode;
	CString name;

	for(int ii = 0; ii < m_arrayPBCode.GetSize(); ii++)
	{
		pbcode = m_arrayPBCode.GetAt(ii);
		name = pbcode.name;
		name.TrimLeft("+"); name.TrimRight();

		if(name.Compare(company) == 0)
			return pbcode.code;
	}
	return "";
}

//void CJongmukDlg::ELWKospi200()
//{
//	m_gListsort = FALSE;
//	clearEditSearch();
//
//	m_select = OTHERTYPE;
//	m_ComboGroup.ShowWindow(SW_HIDE);
//
//	MoveGrid(0);
//	HideButton();
//
//	_elwcode elwcode;
//	CString base;
//	struct	hjcode*	hjc;
//	CPtrArray	arCODE;
//
//	arCODE.RemoveAll();
//
//	int ncnt = m_arrayELWCode.GetSize();
//
//	for(int ii = 0; ii < ncnt; ii++)
//	{
//		elwcode = m_arrayELWCode.GetAt(ii);
//		base = elwcode.bcode[0];
//		base.TrimLeft(); base.TrimRight();
//
//		if(base.Compare("KOSPI200") == 0)
//		{
//			hjc = FindOnlyELW(elwcode.code);
//
//			if(hjc)
//				arCODE.Add(hjc);
//		}
//	}
//
//	LockWindowUpdate();
//
//	CodeListSome(arCODE);
//
//	UnlockWindowUpdate();
//
//	m_ListCtrl.Selected(0);
//	m_szCode = m_ListCtrl.GetItemText(0, 0);
//	m_szName = m_ListCtrl.GetItemText(0, 1);
//	m_szName.TrimLeft();
//	ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc, 0);
//
//}

void CJongmukDlg::InitialETFTree(HTREEITEM hRoot)
{

	CString data, line;
	CString type, opcd, hnam;
	CString tmcd, frcd, frnm, code, etfm, etfl;

	HTREEITEM hItem2;

	hItem2 = m_Tree.InsertItem("��ü����",0,1,hRoot);
	m_Tree.SetItemData(hItem2, MAKELONG(0, tree_etf));

	hItem2 = m_Tree.InsertItem("���纰",0,1,hRoot);
	m_Tree.SetItemData(hItem2, MAKELONG(0, 10000));

	const auto etfItem = m_pApp->_vETFitem;
	for_each(etfItem.begin(), etfItem.end(), [&](const auto item) {
		m_Tree.SetItemData(m_Tree.InsertItem(std::get<2>(item), 0, 1, hItem2), MAKELONG(0, tree_etf_item));
	});


	hItem2 = m_Tree.InsertItem("�����ڻ꺰",0,1,hRoot);	//2015.11.04 KSJ �׸��� --> �����ڻ꺰�� ����
	m_Tree.SetItemData(hItem2, MAKELONG(0, 10000));

	const auto etfTema = m_pApp->_vETFtema;
	for_each(etfTema.begin(), etfTema.end(), [&](const auto item) {
		m_Tree.SetItemData(m_Tree.InsertItem(std::get<2>(item), 0, 1, hItem2), MAKELONG(0, tree_etf_tema));
	});


	hItem2 = m_Tree.InsertItem("����/��",0,1,hRoot);
	m_Tree.SetItemData(hItem2, MAKELONG(0, 10000));

	const auto etffore = m_pApp->_vETFfore;
	for_each(etffore.begin(), etffore.end(), [&](const auto item) {
		m_Tree.SetItemData(m_Tree.InsertItem(std::get<2>(item), 0, 1, hItem2), MAKELONG(0, tree_etf_fore));
	});
	GetDlgItem(IDC_EDIT_SEARCH)->SetFocus();
}

CString CJongmukDlg::FindETFItem(CString company)
{
	const auto etfItem = m_pApp->_vETFitem;
	const auto it = std::find_if(etfItem.begin(), etfItem.end(), [&company](auto item)-> bool {
		if (company.CompareNoCase(std::get<2>(item)) == 0)
			return true;
		return false;
	});

	if (it == etfItem.end())
		return "";

	return std::get<1>(*it);

}

CString CJongmukDlg::FindETFTema(CString tema)
{
	const auto etfTema = m_pApp->_vETFtema;
	const auto it = std::find_if(etfTema.begin(), etfTema.end(), [&tema](const auto item)->bool{
		if (tema.CompareNoCase(std::get<2>(item)) == 0)
			return true;
		return false;
	});

	if (it == etfTema.end())
		return "";

	return std::get<1>(*it);

}

CString CJongmukDlg::FindETFFore(CString fore)
{
	const auto etffore = m_pApp->_vETFfore;
	const auto it = std::find_if(etffore.begin(), etffore.end(), [&fore](const auto item)->bool{
		if (fore.CompareNoCase(std::get<2>(item)) == 0)
			return true;
		return false;
	});

	if (it == etffore.end())
		return "";

	return std::get<1>(*it);
}

void CJongmukDlg::ETFLoadItem(CString code)
{
	m_gListsort = FALSE;
	clearEditSearch();

	m_select = OTHERTYPE;
	m_ComboGroup.ShowWindow(SW_HIDE);

	MoveGrid(0);
	HideButton();

	std::vector<HCODEX*> vData;

	const auto etfcode = m_pApp->_vETFcode;
	for_each(etfcode.begin(), etfcode.end(), [&](const auto item) {
		if (code.Compare(std::get<3>(item)) == 0)
		{
			auto hjc = FindHJCode(std::get<1>(item));
			if (hjc)
				vData.emplace_back(hjc);
		}
	});
	CodeListSome(vData);
}

void CJongmukDlg::ETFLoadTema(CString tmcd)
{
	m_gListsort = FALSE;
	clearEditSearch();

	m_select = OTHERTYPE;
	m_ComboGroup.ShowWindow(SW_HIDE);

	MoveGrid(0);
	HideButton();

	std::vector<HCODEX*> vData;


	const auto etfcode = m_pApp->_vETFcode;
	for_each(etfcode.begin(), etfcode.end(), [&](const auto item) {
		if (tmcd[0] == std::get<4>(item))
		{
			auto hjc = FindHJCode(std::get<1>(item));
			if (hjc)
				vData.emplace_back(hjc);
		}
	});
	CodeListSome(vData);
}

void CJongmukDlg::ETFLoadFore(CString frcd)
{
	m_gListsort = FALSE;
	clearEditSearch();

	m_select = OTHERTYPE;
	m_ComboGroup.ShowWindow(SW_HIDE);

	MoveGrid(0);
	HideButton();

	std::vector<HCODEX*> vData;


	const auto etfcode = m_pApp->_vETFcode;
	for_each(etfcode.begin(), etfcode.end(), [&](const auto item) {
		if (frcd[0] == std::get<5>(item))
		{
			auto hjc = FindHJCode(std::get<1>(item));
			if (hjc)
				vData.emplace_back(hjc);
		}
	});
	CodeListSome(vData);
}

void CJongmukDlg::OnGetdispinfoListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem= &(pDispInfo)->item;

	if (_vCODE.empty())
		return;

	const auto &[code, name] = _vCODE.at(pItem->iItem);
	if (pItem->mask & LVIF_TEXT) //valid text buffer?
	{
		switch(pItem->iSubItem)
		{
		case 0:
			lstrcpy(pItem->pszText, code);
			break;
		case 1:
			lstrcpy(pItem->pszText, name);
			break;
		}
	}
	*pResult = 0;
}
