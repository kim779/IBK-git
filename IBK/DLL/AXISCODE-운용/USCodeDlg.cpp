// USCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axiscode.h"
#include "USCodeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	US_TOPCODE_FILE	"uscode_top.dat"

#define NAME_KOR	0
#define NAME_ENG	1

#define BTN_KOR		_T("�ѱ�")
#define BTN_ENG		_T("����")

#define I_JISU		'0'	// ����
#define I_ITEM		'1'	// ����
#define I_FUTURE	'2'	// ����
#define I_RATE		'3'	// �ݸ�
#define I_DEBT		'4'	// ��ä
#define I_EXCHANGE	'5'	// ȯ��
#define I_ETC		'6'	// ��Ÿ

#define I_JISU_USA	'1'	// �̱�
#define I_JISU_ASIA	'2'	// �ƽþ�
#define I_JISU_EUR	'3'	// ����
#define I_JISU_MSCI	'4'	// MSCI
#define I_JISU_ETC	'5'	// ��Ÿ

#define I_ITEM_USA	'1'	// �̱�
#define I_ITEM_JAPAN	'2'	// �Ϻ�
#define I_ITEM_HONGKONG	'3'	// ȫ��
#define I_ITEM_TAIWAN	'4'	// �븸
#define I_ITEM_UK	'5'	// ����
#define I_ITEM_FRANCE	'6'	// ������
#define I_ITEM_GERMAN	'7'	// ����
#define I_ITEM_ADR	'8'	// ADR
#define I_ITEM_GDR	'9'	// GDR

#define I_FUTURE_CME	'1'	// CME
#define I_FUTURE_ENERGY	'2'	// ������
#define I_FUTURE_GOODS	'3'	// ��ǰ����

#define I_RATE_LIBO	'1'	// �����ݸ�
#define I_RATE_USA	'2'	// USA

#define I_DEBT_USA	'1'	// �̱���ä
#define I_DEBT_JAPAN	'2'	// �Ϻ�
#define I_DEBT_UK	'3'	// ����
#define I_DEBT_GERMAN	'4'	// ����

#define I_EXCHANGE_INT	'1'	// ����ȯ��

/////////////////////////////////////////////////////////////////////////////
// CUSCodeDlg dialog

bool	m_listsort;

CUSCodeDlg::CUSCodeDlg(CPoint pt, CWnd* pParent /*=nullptr*/)
	: CDialog(CUSCodeDlg::IDD, pParent)
{
	m_point = pt;

	m_listsort = true;
	m_nameFlag = NAME_KOR;
	m_CodeKind = ckTop;

	m_root = ((CAxisCodeApp*)AfxGetApp())->m_root;

	m_rtnData.code = _T("");
	m_rtnData.name = _T("");

	//{{AFX_DATA_INIT(CUSCodeDlg)
	//}}AFX_DATA_INIT
}


void CUSCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUSCodeDlg)
	DDX_Control(pDX, IDC_BTN_TOP, m_btn_top);
	DDX_Control(pDX, IDC_BTN_RATE, m_btn_rate);
	DDX_Control(pDX, IDC_BTN_JISU, m_btn_jisu);
	DDX_Control(pDX, IDC_BTN_ITEM, m_btn_item);
	DDX_Control(pDX, IDC_BTN_FUTURE, m_btn_future);
	DDX_Control(pDX, IDC_BTN_EXCHANGE, m_btn_exchange);
	DDX_Control(pDX, IDC_BTN_ENG, m_btn_eng);
	DDX_Control(pDX, IDC_BTN_DEBT, m_btn_debt);
	DDX_Control(pDX, IDC_CBO_SLIST, m_cbo_slist);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_edit_search);
	DDX_Control(pDX, IDC_BTN_SORT14, m_btn_sort14);
	DDX_Control(pDX, IDC_BTN_SORT13, m_btn_sort13);
	DDX_Control(pDX, IDC_BTN_SORT12, m_btn_sort12);
	DDX_Control(pDX, IDC_BTN_SORT11, m_btn_sort11);
	DDX_Control(pDX, IDC_BTN_SORT10, m_btn_sort10);
	DDX_Control(pDX, IDC_BTN_SORT09, m_btn_sort09);
	DDX_Control(pDX, IDC_BTN_SORT08, m_btn_sort08);
	DDX_Control(pDX, IDC_BTN_SORT07, m_btn_sort07);
	DDX_Control(pDX, IDC_BTN_SORT06, m_btn_sort06);
	DDX_Control(pDX, IDC_BTN_SORT05, m_btn_sort05);
	DDX_Control(pDX, IDC_BTN_SORT04, m_btn_sort04);
	DDX_Control(pDX, IDC_BTN_SORT03, m_btn_sort03);
	DDX_Control(pDX, IDC_BTN_SORT02, m_btn_sort02);
	DDX_Control(pDX, IDC_BTN_SORT01, m_btn_sort01);
	DDX_Control(pDX, IDC_BTN_ALL, m_btn_all);
	DDX_Control(pDX, IDC_LIST_CODE, m_ListCtlCode);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUSCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CUSCodeDlg)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_SEARCH, OnChangeEditSearch)
	ON_CBN_SELCHANGE(IDC_CBO_SLIST, OnSelchangeCboSlist)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CODE, OnDblclkListCode)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_BTN_ALL, IDC_BTN_TOP, OnClickBtnKind)
	ON_COMMAND_RANGE(IDC_BTN_SORT01, IDC_BTN_ENG, OnClickBtnSort)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_CODE, &CUSCodeDlg::OnGetdispinfoListCode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUSCodeDlg message handlers

void CUSCodeDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	m_CtrlFont.DeleteObject();
	_sBuffer.Empty();
}

BOOL CUSCodeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_CtrlFont.DeleteObject();
	m_CtrlFont.CreatePointFontIndirect(&SetFont(9));
	
	LoadTopCode();
	LoadUSCode();
	InitCtrl();

	SetBtn();
	OnClickBtnKind(IDC_BTN_TOP);

	SetDlgPos(m_point.x, m_point.y);

	return TRUE;
}

void CUSCodeDlg::SetDlgPos(int x, int y)
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

CString CUSCodeDlg::GetCountry(CString ncod)
{
	const struct _ncodInfo {
		char*	code;
		char*	name;
	} ncodInfo[] = {
		{"100", "�ѱ�"}, {"101", "�Ϻ�"}, {"102", "ȫ��"}, {"103", "�븸"},
		{"104", "�߱�"}, {"105", "�̰���"}, {"106", "ȣ��"}, {"107", "�ε��׽þ�"},
		{"108", "�±�"}, {"109", "�ʸ���"}, {"110", "����������"}, {"111", "�ε�"},
		{"112", "������ī"}, {"113", "��Ű��ź"}, {"202", "��������"}, {"301", "����"},
		{"302", "������"}, {"303", "����"}, {"304", "��Ż����"}, {"305", "�׵�����"},
		{"306", "������"}, {"307", "������"}, {"308", "��������"}, {"309", "���⿡"},
		{"310", "����ũ"}, {"311", "�׸���"}, {"312", "������"}, {"313", "��Ű"},
		{"314", "�ʶ���"}, {"315", "���þ�"}, {"316", "������"}, {"317", "����Ʈ����"},
		{"318", "ũ�ξ�Ƽ��"}, {"319", "�밡��"}, {"320", "�����ƴϾ�"}, {"321", "�븣����"},
		{"322", "�縶�Ͼ�"}, {"501", "�̱�"}, {"502", "ĳ����"}, {"503", "�߽���"},
		{"504", "�����"}, {"505", "�Ƹ���Ƽ��"}, {"506", "ĥ��"}, {"507", "���"},
		{"609", "���ٺ��"}
	};

	CString rValue = _T("");
	for (int ii = 0 ; ii < sizeof(ncodInfo) / sizeof(_ncodInfo) ; ii++)
	{
		if (ncodInfo[ii].code == ncod)
		{
			rValue = ncodInfo[ii].name;
			break;
		}
	}

	return rValue;
}

void CUSCodeDlg::ResetCboItem(enum _codeKind codeKind)
{
	m_cbo_slist.ResetContent();
	switch (codeKind)
	{
	case ckAll:
		m_cbo_slist.AddString("������");
		break;
	case ckJisu:
		m_cbo_slist.AddString("��ü");
		m_cbo_slist.AddString("�̱�");
		m_cbo_slist.AddString("�ƽþ�");
		m_cbo_slist.AddString("����");
		m_cbo_slist.AddString("MSCI");
		m_cbo_slist.AddString("��Ÿ");
		break;
	case ckItem:
		m_cbo_slist.AddString("��ü");
		m_cbo_slist.AddString("�̱�");
		m_cbo_slist.AddString("�Ϻ�");
		m_cbo_slist.AddString("ȫ��");
		m_cbo_slist.AddString("�븸");
		m_cbo_slist.AddString("����");
		m_cbo_slist.AddString("������");
		m_cbo_slist.AddString("����");
		m_cbo_slist.AddString("ADR");
		m_cbo_slist.AddString("GDR");
		break;
	case ckFuture:
		m_cbo_slist.AddString("��ü");
		m_cbo_slist.AddString("CME��������");
		m_cbo_slist.AddString("������");
		m_cbo_slist.AddString("��ǰ����");
		break;
	case ckRate:
		m_cbo_slist.AddString("��ü");
		m_cbo_slist.AddString("�����ݸ�");
		m_cbo_slist.AddString("�̱��ݸ�");
		break;
	case ckDebt:
		m_cbo_slist.AddString("��ü");
		m_cbo_slist.AddString("�̱�");
		m_cbo_slist.AddString("�Ϻ�");
		m_cbo_slist.AddString("����");
		m_cbo_slist.AddString("����");
		break;
	case ckExchange:
		m_cbo_slist.AddString("��ü"); // ����ȯ��
		break;
	case ckTop:
		m_cbo_slist.AddString("�ֿ�����"); // ����ȯ��
		break;
	//case ckEtc:
	//	break;
	}
	m_cbo_slist.SetCurSel(0);
}

void CUSCodeDlg::SelectCode(enum _codeKind codeKind, short subKind)
{
	_vSelect.clear();
	if (codeKind == ckAll)
	{
		for_each(_vUSCode.begin(), _vUSCode.end(), [&](const auto usCode){
			if (usCode->dat1 == I_JISU || usCode->dat1 == I_ITEM
			  || usCode->dat1 == I_FUTURE || usCode->dat1 == I_RATE
			  || usCode->dat1 == I_DEBT || usCode->dat1 == I_EXCHANGE
			  /*|| m_aUSCode[ii]->dat1 == I_ETC*/)
				_vSelect.emplace_back(std::move(std::make_tuple(
					CString(usCode->symb, sizeof(usCode->symb)).Trim(),
					CString(usCode->knam, sizeof(usCode->knam)).Trim(),
					CString(usCode->enam, sizeof(usCode->enam)).Trim(),
					GetCountry(CString(usCode->ncod, sizeof(usCode->ncod)).Trim())
				)));
		});
	}
	else if (codeKind == ckTop)
	{

		for_each(_vUSCode.begin(), _vUSCode.end(), [&](const auto usCode){
			if (usCode->dat1 == I_JISU || usCode->dat1 == I_ITEM
				|| usCode->dat1 == I_FUTURE || usCode->dat1 == I_RATE
				|| usCode->dat1 == I_DEBT || usCode->dat1 == I_EXCHANGE
				/*|| m_aUSCode[ii]->dat1 == I_ETC*/)
			{
				CString symb = CString(usCode->symb, sizeof(usCode->symb)).Trim();
				const auto ft = _mTop.find(symb);
				if (ft != _mTop.end())
				{
					_vSelect.emplace_back(std::move(std::make_tuple(
						CString(usCode->symb, sizeof(usCode->symb)).Trim(),
						CString(usCode->knam, sizeof(usCode->knam)).Trim(),
						CString(usCode->enam, sizeof(usCode->enam)).Trim(),
						GetCountry(CString(usCode->ncod, sizeof(usCode->ncod)).Trim())
					)));
				}
			}
		});
	}
	else
	{
		char dat1{}, dat2{};
		dat1 = '0' + (int)codeKind;
		dat2 = '0' + (int)subKind;
		for_each(_vUSCode.begin(), _vUSCode.end(), [&](const auto usCode) {
			if (usCode->dat1 == dat1 && (subKind == 0 || usCode->dat2 == dat2))
				_vSelect.emplace_back(std::move(std::make_tuple(
					CString(usCode->symb, sizeof(usCode->symb)).Trim(),
					CString(usCode->knam, sizeof(usCode->knam)).Trim(),
					CString(usCode->enam, sizeof(usCode->enam)).Trim(),
					GetCountry(CString(usCode->ncod, sizeof(usCode->ncod)).Trim())
				)));
			});
	}
	
	_vData.clear();
	std::copy(_vSelect.begin(), _vSelect.end(), std::back_inserter(_vData));

}

void CUSCodeDlg::SetCodeList(int kind)
{
	ReSizeCodeList(kind);	

	switch (kind)
	{
	case IDC_BTN_ALL:
		m_CodeKind = ckAll;
		break;
	case IDC_BTN_JISU: // ����
		m_CodeKind = ckJisu;
		break;
	case IDC_BTN_ITEM: // ����
		m_CodeKind = ckItem;
		break;
	case IDC_BTN_FUTURE: // ����
		m_CodeKind = ckFuture;
		break;
	case IDC_BTN_RATE: // �ݸ�
		m_CodeKind = ckRate;
		break;
	case IDC_BTN_DEBT: // ��ä
		m_CodeKind = ckDebt;
		break;
	case IDC_BTN_EXCHANGE: // ȯ��
		m_CodeKind = ckExchange;
		break;
	case IDC_BTN_TOP: // �ֿ�����
		m_CodeKind = ckTop;
		break;
	}
	ResetCboItem(m_CodeKind);
	SelectCode(m_CodeKind, 0);


	m_listsort = false;
	listsort(0);

	m_ListCtlCode.SetItemCountEx(_vData.size());
	m_ListCtlCode.Invalidate();
}

void CUSCodeDlg::OnSelchangeCboSlist() 
{
	SelectCode(m_CodeKind, m_cbo_slist.GetCurSel());
	m_ListCtlCode.SetItemCountEx(_vData.size());
	m_ListCtlCode.Invalidate();
	
}

void CUSCodeDlg::ReSizeCodeList(int kind)
{
	switch (kind)
	{
	case IDC_BTN_ALL:
	case IDC_BTN_TOP:
		{
			CRect rcList;
			m_ListCtlCode.GetWindowRect(rcList); ScreenToClient(rcList);
			rcList.top = 21;
			
			m_ListCtlCode.MoveWindow(rcList);
			m_cbo_slist.ShowWindow(SW_HIDE);
		}
		break;
	default:
		{
			CRect rcList;
			m_ListCtlCode.GetWindowRect(rcList); ScreenToClient(rcList);
			rcList.top = 41;
			
			m_ListCtlCode.MoveWindow(rcList);
			m_cbo_slist.ShowWindow(SW_SHOW);
		}
		break;
	}
}


void CUSCodeDlg::InitCtrl()
{
	m_ListCtlCode.SetFont(&m_CtrlFont);
	m_ListCtlCode.SetBkColor(COR_WHITE);
	m_ListCtlCode.SetTextBkColor(COR_WHITE);
	m_ListCtlCode.SetTextColor(RGB(0,0,0));
	m_ListCtlCode.InsertColumn(1, "�ڵ�", LVCFMT_LEFT, 80);
	m_ListCtlCode.InsertColumn(2, "�ѱ۸�", LVCFMT_LEFT, 117);
	m_ListCtlCode.InsertColumn(3, "������", LVCFMT_LEFT, 110);

	m_cbo_slist.AddString("������");
	m_cbo_slist.SetCurSel(0);
}

BOOL CUSCodeDlg::LoadUSCode()
{
	if (_sBuffer.GetLength() > 0)
		return TRUE;

	TRY
	{
		CString		path = m_root + "/tab/" + USCODE_FILE;
		CFile		file(path, CFile::modeRead | CFile::shareDenyNone);

		const UINT  size = gsl::narrow_cast<UINT>(file.GetLength());		
		const UINT  nBytesRead = file.Read(_sBuffer.GetBuffer(size + 1), size);
		file.Close();
		
		if (size > nBytesRead)
	 		return FALSE;

		const int count = size / sizeof(struct _USCode); 
		_USCode *pCode = (struct _USCode*)_sBuffer.GetBuffer();
		for (int ii = 0; ii < count; ii++, pCode++)
			_vUSCode.emplace_back(std::move(pCode));

	}CATCH(CFileException, e)
	{
		#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
		#endif
	}
	END_CATCH
		
	return TRUE;
}

BOOL CUSCodeDlg::LoadTopCode()
{
	vector<CString> ss;
	CString sBuffer;
	CString	path = m_root + "/tab/" + US_TOPCODE_FILE;
	CFile file(path, CFile::modeRead|CFile::shareDenyNone);
	
	const int size = gsl::narrow_cast<int>(file.GetLength());
	const int nRead = file.Read(sBuffer.GetBuffer(size + 1), size);
	sBuffer.ReleaseBuffer();
	file.Close();
	if (nRead < size) return FALSE;

	AxStd::SplitSub(ss, sBuffer, "\r\n");

	for_each(ss.begin(), ss.end(), [&](const auto item){
		_mTop.emplace(std::move(std::make_pair(item, "")));
	});

	return TRUE;
}


LOGFONT CUSCodeDlg::SetFont(int Size)
{
	LOGFONT		Font;

	ZeroMemory(&Font, sizeof(LOGFONT));
	Font.lfHeight		= Size * 10;
	Font.lfCharSet		= DEFAULT_CHARSET;
	Font.lfWeight		= FW_NORMAL;
	Font.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	Font.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	Font.lfQuality		= DEFAULT_QUALITY;
	Font.lfPitchAndFamily	= DEFAULT_PITCH; 
	Font.lfItalic		= false;
	strcpy_s(Font.lfFaceName, "����");

	return Font;
}


BOOL CUSCodeDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	switch(wParam)
	{
	case IDC_LIST_CODE:
		{
			switch(((LPNMHDR) lParam)->code)
			{
			case LVN_COLUMNCLICK:
				#define pnm ((LPNMLISTVIEW)lParam)
				m_listsort = !m_listsort;
				listsort((LPARAM)pnm->iSubItem);
				#undef pnm
				break;
			}			
		}
		break;
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CUSCodeDlg::listsort(int type)
{
	std::sort(_vData.begin(), _vData.end(), [type](const auto item1, const auto item2) {
		switch (type)
		{
		case 1:
			if (m_listsort)
				return (std::get<1>(item1).Compare(std::get<1>(item2)) > 0);
			else 
				return (std::get<1>(item1).Compare(std::get<1>(item2)) < 0);
			break;
		case 2:
			if (m_listsort)
				return (std::get<2>(item1).Compare(std::get<2>(item2)) > 0);
			else 
				return (std::get<2>(item1).Compare(std::get<2>(item2)) < 0);
			break;
		case 0:
		default:
			if (m_listsort)
				return (std::get<0>(item1).Compare(std::get<0>(item2)) > 0);
			else
				return (std::get<0>(item1).Compare(std::get<0>(item2)) < 0);
			break;
		}
	});

	m_ListCtlCode.Invalidate();
}


void CUSCodeDlg::SetBtn()
{
	const short	shBtnColor = 245;
	
	for (int ii = IDC_BTN_SORT01 ; ii <= IDC_BTN_TOP ; ii++)
	{
		((CButtonST*)GetDlgItem(ii))->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
		((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, USER_FOCUS_COR);
		((CButtonST*)GetDlgItem(ii))->SetWnd(this);
		((CButtonST*)GetDlgItem(ii))->SetBtnCursor(IDC_CURSOR_HAND);
	}
}

void CUSCodeDlg::OnClickBtnKind(UINT nID)
{
	for (int ii = IDC_BTN_ALL ; ii <= IDC_BTN_TOP ; ii++)
	{
		if ((int)nID == ii)
		{
			((CButtonST*)GetDlgItem(ii))->SetBold(TRUE);
			((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
			((CButtonST*)GetDlgItem(ii))->SetSelected(TRUE);
		}
		else
		{
			((CButtonST*)GetDlgItem(ii))->SetBold(FALSE);
			((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
			((CButtonST*)GetDlgItem(ii))->SetSelected(FALSE);
		}
	}	
	SetCodeList(nID);
}

void CUSCodeDlg::OnClickBtnSort(UINT nID)
{
	switch (nID)
	{
	case IDC_BTN_ENG:
		{
			m_nameFlag = !m_nameFlag;
			if (m_nameFlag)
				((CButtonST*)GetDlgItem(nID))->SetWindowText(BTN_KOR);
			else
				((CButtonST*)GetDlgItem(nID))->SetWindowText(BTN_ENG);
		}
		break;
	default:
		{
			if (!m_nameFlag)
			{
				const struct _sortInfo {
					char*	str;
					int	len;
				} sortInfo[] = {
					{"��", 2}, {"��", 2}, {"��", 2}, {"��", 2}, {"��", 2},
					{"��", 2}, {"��", 2}, {"��", 2}, {"��", 2}, {"��", 2},
					{"ī", 2}, {"Ÿ", 2}, {"��", 2}, {"��", 2}, {"A", 1}, {"1", 1}
				};

				const int bSel = nID - IDC_BTN_SORT01;

				m_listsort = FALSE;
				listsort(1);				
				setSelected(&m_ListCtlCode, findIndex(sortInfo[bSel].str, sortInfo[bSel].len));
			}
		}
		break;
	}
}

int CUSCodeDlg::findIndex(CString text, int len, int column /* = 1 */)
{
	auto ft = std::find_if(_vData.begin(), _vData.end(), [&text, len](const auto item){
		CString name = std::get<1>(item);
		if (strncmp(text, name, len) <= 0)
			return true;
		return false;
	});
	if (ft != _vData.end())
	{
		killSelected(&m_ListCtlCode);
		return (ft - _vData.begin());
	}
	
	return -1;
}

void CUSCodeDlg::setSelected(CListCtrl* ListCtrl, int nItem, BOOL scroll /* = TRUE */)
{
	if (nItem < 0)
	{
		m_rtnData.code = _T("");
		m_rtnData.name = _T("");
		return;
	}

	CRect	rc;
	CSize	size;

	ListCtrl->SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	if (scroll)
	{
		ListCtrl->GetItemRect(0, rc, LVIR_LABEL);
		size.cx = 0;
		size.cy = rc.Height() * (nItem - ListCtrl->GetTopIndex());
		ListCtrl->Scroll(size);
	}

	CString code = m_ListCtlCode.GetItemText(nItem, 0);
	CString	name = m_ListCtlCode.GetItemText(nItem, 1);

	m_rtnData.code = code;
	m_rtnData.name = name;
}

void CUSCodeDlg::killSelected(CListCtrl* ListCtrl)
{
	POSITION pos = ListCtrl->GetFirstSelectedItemPosition();
	if (pos != nullptr)
	{
		while (pos)
		{
			int	nItem;
			nItem = ListCtrl->GetNextSelectedItem(pos);
			ListCtrl->SetItemState(nItem, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

void CUSCodeDlg::OnChangeEditSearch() 
{
	CString str;
	m_edit_search.GetWindowText(str);

	m_listsort = FALSE;
	listsort(1);
	str.MakeUpper();
	setSelected(&m_ListCtlCode, findIndex(str, str.GetLength()));	
}

void CUSCodeDlg::OnDblclkListCode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int	nItem{};
	CString	code, name;
	POSITION pos = m_ListCtlCode.GetFirstSelectedItemPosition();
	if (pos != nullptr)
	{
		while (pos)
		{
			nItem = m_ListCtlCode.GetNextSelectedItem(pos);

			code = m_ListCtlCode.GetItemText(nItem, 0);
			name = m_ListCtlCode.GetItemText(nItem, 1);

			m_rtnData.code = code;
			m_rtnData.name = name;
		}
	}

	OnOK();
	
	*pResult = 0;
}

CString CUSCodeDlg::GetRtnData(bool dialog /* = true */, CString code /* = _T("") */)
{
	CString rValue = _T("");

	if (dialog)
	{
		if (!m_rtnData.code.IsEmpty() && !m_rtnData.name.IsEmpty())
			rValue.Format("%s\t%s", m_rtnData.code, m_rtnData.name);
	}
	else
	{
		CString	symb, name;
		const int	nSize = m_aUSCode.GetSize();
		for (int ii = 0 ; ii < nSize ; ii++)
		{
			symb = CString(m_aUSCode[ii]->symb, sizeof(m_aUSCode[ii]->symb));
			if (code.Compare(symb) == 0)
			{
				name = CString(m_aUSCode[ii]->knam, sizeof(m_aUSCode[ii]->knam));
				rValue.Format("%s\t%s", code, name);
			}
		}
	}
	
	return rValue;
}

void CUSCodeDlg::OnGetdispinfoListCode(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &(pDispInfo)->item;

	const auto& [code, name, ename, country] = _vData.at(pItem->iItem);
	if (pItem->mask & LVIF_TEXT) //valid text buffer?
	{
		switch (pItem->iSubItem)
		{
		case 0:
			lstrcpy(pItem->pszText, code);
			break;
		case 1:
			lstrcpy(pItem->pszText, name);
			break;
		case 2:
			lstrcpy(pItem->pszText, ename);
			break;
		break;
		}
	}
	*pResult = 0;
}
