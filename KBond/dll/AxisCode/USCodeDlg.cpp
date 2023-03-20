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

#define cTAB		'\t'

#define NAME_KOR	0
#define NAME_ENG	1

#define BTN_KOR		_T("�ѱ�")
#define BTN_ENG		_T("����")

#define I_USI	'P'	// �̱�����
#define I_WRI	'W'	// ��������
#define I_WMQ	'H'	// �����ֿ�����
#define I_CME	'F'	// CME����
#define I_ADR	'D'	// ADR
#define I_GDR	'G'	// GDR
#define I_SCO	'M'	// �ݵ�ü
#define I_MAT	'S'	// ������
#define I_GOF	'B'	// ����ä
#define I_LBI	'L'	// �����ݸ�
#define I_AMI	'R'	// �̱��ݸ�
#define I_KOI	'N'	// �����ݸ�
#define I_EXR	'X'	// ȯ��

#define I_DOW	0x01	// �ٿ�30
#define I_NAS	0x02	// ������100
#define I_SNP	0x04	// S&P500

#define NO_ALL	0	//��ü
#define NO_SNP	1	//S&P500
#define NO_NAS	2	//������100
#define NO_DOW	3	//�ٿ�30

#define EXCHANGE_NYSE	21	//�ŷ��� ����
#define EXCHANGE_NASDAQ	22
#define EXCHANGE_AMEX	23
#define EXCHANGE_DOW	35
#define EXCHANGE_SNP	36
#define EXCHANGE_MSCI	37
#define EXCHANGE_GOLDMAN	38
#define EXCHANGE_PHIL		39

#define GOLDMAN_1	"USI@GHA"
#define GOLDMAN_2	"USI@GIN"
#define GOLDMAN_3	"USI@GIP"
#define GOLDMAN_4	"USI@GSM"
#define GOLDMAN_5	"USI@GSO"
#define GOLDMAN_6	"USI@GSV"

#define PHIL_1		"USI@SOXX"
#define PHIL_2		"USI@XAU"
#define PHIL_3		"USI@DOT"
#define PHIL_4		"AMS@BKX"
#define PHIL_5		"AMS@OSX"
#define PHIL_6		"AMS@UTY"

#define DEFAULT_INDEX_US	"��ü:AMEX:NASDAQ:NYSE:DOW:S&P:��常�轺:�ʶ��Ǿ�:"
#define DEFAULT_INDEX_GLOBAL	"��ü:�Ϻ�:�븸:ȫ��:�߱�:��Ÿ:MSCI:"
#define DEFAULT_ITEM_US		"��ü:S&P500:������100:�ٿ�30:"
#define DEFAULT_ITEM_GLOBAL	"��ü:�Ϻ�:�븸:ȫ��:�߱�:����:������:����:"

/////////////////////////////////////////////////////////////////////////////
// CUSCodeDlg dialog

bool	m_listsort;

CUSCodeDlg::CUSCodeDlg(CPoint pt, CWnd* pParent /*=NULL*/)
	: CDialog(CUSCodeDlg::IDD, pParent)
{
	m_Point = pt;

	m_listsort = true;
	m_nameFlag = NAME_KOR;

	m_sRoot = ((CAxisCodeApp*)AfxGetApp())->m_RootPath;

	m_rtnData.code = _T("");
	m_rtnData.name = _T("");

	m_nCurPos = 0;
	//{{AFX_DATA_INIT(CUSCodeDlg)
	//}}AFX_DATA_INIT
}


void CUSCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUSCodeDlg)
	DDX_Control(pDX, IDC_BTN_MAT, m_btn_mat);
	DDX_Control(pDX, IDC_BTN_LIBOINTEREST, m_btn_lbi);
	DDX_Control(pDX, IDC_BTN_KAINTEREST, m_btn_kai);
	DDX_Control(pDX, IDC_BTN_GOFUND, m_btn_gof);
	DDX_Control(pDX, IDC_BTN_EXRATE, m_btn_exr);
	DDX_Control(pDX, IDC_CBO_SLIST, m_cbo_slist);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_edit_search);
	DDX_Control(pDX, IDC_BTN_ENG, m_btn_eng);
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
	DDX_Control(pDX, IDC_BTN_USI, m_btn_usi);
	DDX_Control(pDX, IDC_BTN_WRI, m_btn_wri);
	DDX_Control(pDX, IDC_BTN_AMQ, m_btn_amq);
	DDX_Control(pDX, IDC_BTN_WMQ, m_btn_wmq);
	DDX_Control(pDX, IDC_BTN_CME, m_btn_cme);
	DDX_Control(pDX, IDC_BTN_ADR, m_btn_adr);
	DDX_Control(pDX, IDC_LIST_CODE, m_ListCtlCode);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUSCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CUSCodeDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_SEARCH, OnChangeEditSearch)
	ON_CBN_SELCHANGE(IDC_CBO_SLIST, OnSelchangeCboSlist)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CODE, OnDblclkListCode)
//	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_BTN_ALL, IDC_BTN_EXRATE, OnClickBtnKind)
	ON_COMMAND_RANGE(IDC_BTN_SORT01, IDC_BTN_ENG, OnClickBtnSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUSCodeDlg message handlers
int CALLBACK ListViewCompareFunc3( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{ 
	struct _ListSort { 
		char*	Code[SORT_LEN_CODE];
		char*	Name[SORT_LEN_NAME];
		char*	CName[SORT_LEN_CNAME];
	};

	_ListSort *pItem1 = (_ListSort *) lParam1; 
	_ListSort *pItem2 = (_ListSort *) lParam2; 

	int	iCmp = 0;

	switch ((int)lParamSort)
	{
	case 0:
		iCmp = lstrcmpi((char*)pItem1->Code, (char*)pItem2->Code); 
		break;
	case 1:
		iCmp = lstrcmpi((char*)pItem1->Name, (char*)pItem2->Name);
		break;
	case 2:
		iCmp = lstrcmpi((char*)pItem1->CName, (char*)pItem2->CName);
		break;
	}

	if (m_listsort)
	{
		if (iCmp > 0)
			iCmp = -1;
		else if (iCmp < 0)
			iCmp = 1;
	}

	return iCmp;
} 

void CUSCodeDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	ClearListCtlCode();

	m_CtrlFont.DeleteObject();
}

BOOL CUSCodeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	m_CtrlFont.DeleteObject();
	m_CtrlFont.CreatePointFontIndirect(&SetFont(9));
	
	InitCtrl();

	SetBtn();
	OnClickBtnKind(IDC_BTN_ALL);

//	if (m_Point.x > 0 || m_Point.y > 0)
//		SetWindowPos(NULL, m_Point.x, m_Point.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	SetDlgPos(m_Point.x, m_Point.y);

	CString msg;
	msg.Format("%d", pApp->m_arrayUSCode.GetSize());
//	MessageBox(msg);

	return TRUE;
}

void CUSCodeDlg::SetDlgPos(int x, int y)
{
	if (x < 0 || y < 0)
		return;

	int	scWidth, scHeight;
	scWidth	= GetSystemMetrics(SM_CXFULLSCREEN);
	scHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	CRect	rc;
	GetClientRect(&rc);

	int	gabX = scWidth - (x + rc.Width());
	int	gabY = scHeight - (y + rc.Height());
	if (gabX < 0)
		x += gabX - 7;
	if (gabY < 0)
		y += gabY - 7;

	SetWindowPos(NULL, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
}

CString CUSCodeDlg::GetCountry(CString ncod)
{
	struct _ncodInfo {
		char*	code;
		char*	name;
	} ncodInfo[] = {
		{"AR", "�Ƹ���Ƽ��"}, {"AU", "����Ʈ���ϸ���"}, {"AT", "����Ʈ����"}, {"BH", "�ٷ���"},
		{"BE", "���⿡"}, {"BR", "�����"}, {"CA", "ĳ����"}, {"CL", "ĥ��"},
		{"CN", "�߱�"}, {"CO", "�ݷҺ��"}, {"HR", "ũ�ξ�Ƽ��"}, {"CY", "Ű���ν�"},
		{"CZ", "ü��"}, {"DK", "����ũ"}, {"EG", "��¤Ʈ"}, {"EE", "������Ͼ�"},
		{"FI", "�ɶ���"}, {"FR", "������"}, {"DE", "����"}, {"GR", "�׸���"},
		{"HK", "ȫ��"}, {"HU", "�밡��"}, {"IN", "�ε�"}, {"ID", "�ε����̻�"},
		{"IE", "���Ϸ���"}, {"IL", "�̽���"}, {"IT", "��Ż����"}, {"JP", "�Ϻ�"},
		{"JO", "�丣��"}, {"KR", "�ѱ�"}, {"KW", "�����Ʈ"}, {"LV", "��Ʈ���"},
		{"LB", "���ٳ�"}, {"LT", "�����ƴϾ�"}, {"LU", "����θ�ũ"}, {"MY", "�����̽þ�"},
		{"MT", "��Ÿ"}, {"MX", "�߽���"}, {"MA", "�����"}, {"NA", "���̺��"},
		{"NZ", "��������"}, {"NO", "�븣����"}, {"OM", "����"}, {"PK", "��Ű��ź"},
		{"PE", "���"}, {"PH", "�ʸ���"}, {"PL", "������"}, {"PT", "������Į"}, 
		{"QA", "īŸ��"}, {"RU", "���þ�"}, {"SA", "����ƶ���"}, {"SG", "�̰�����"}, 
		{"SK", "���ι�Ű��"}, {"SI", "���κ��Ͼ�"}, {"ZA", "��������ī"}, {"ES", "������"}, 
		{"SE", "������"}, {"CH", "������"}, {"TW", "�븸"}, {"TH", "Ÿ��"}, 
		{"NL", "�״�����"}, {"TN", "Ƣ����"}, {"TR", "��Ű"}, {"AE", "�ƶ����̸�Ʈ"}, 
		{"GB", "����"}, {"US", "�̱�"}, {"DJ", "�̱�"}, {"VE", "���׼�����"}
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

CString CUSCodeDlg::GetCountryCode(CString sCountry)
{
	struct _ncodInfo {
		char*	code;
		char*	name;
	} ncodInfo[] = {
		{"AR", "�Ƹ���Ƽ��"}, {"AU", "����Ʈ���ϸ���"}, {"AT", "����Ʈ����"}, {"BH", "�ٷ���"},
		{"BE", "���⿡"}, {"BR", "�����"}, {"CA", "ĳ����"}, {"CL", "ĥ��"},
		{"CN", "�߱�"}, {"CO", "�ݷҺ��"}, {"HR", "ũ�ξ�Ƽ��"}, {"CY", "Ű���ν�"},
		{"CZ", "ü��"}, {"DK", "����ũ"}, {"EG", "��¤Ʈ"}, {"EE", "������Ͼ�"},
		{"FI", "�ɶ���"}, {"FR", "������"}, {"DE", "����"}, {"GR", "�׸���"},
		{"HK", "ȫ��"}, {"HU", "�밡��"}, {"IN", "�ε�"}, {"ID", "�ε����̻�"},
		{"IE", "���Ϸ���"}, {"IL", "�̽���"}, {"IT", "��Ż����"}, {"JP", "�Ϻ�"},
		{"JO", "�丣��"}, {"KR", "�ѱ�"}, {"KW", "�����Ʈ"}, {"LV", "��Ʈ���"},
		{"LB", "���ٳ�"}, {"LT", "�����ƴϾ�"}, {"LU", "����θ�ũ"}, {"MY", "�����̽þ�"},
		{"MT", "��Ÿ"}, {"MX", "�߽���"}, {"MA", "�����"}, {"NA", "���̺��"},
		{"NZ", "��������"}, {"NO", "�븣����"}, {"OM", "����"}, {"PK", "��Ű��ź"},
		{"PE", "���"}, {"PH", "�ʸ���"}, {"PL", "������"}, {"PT", "������Į"}, 
		{"QA", "īŸ��"}, {"RU", "���þ�"}, {"SA", "����ƶ���"}, {"SG", "�̰�����"}, 
		{"SK", "���ι�Ű��"}, {"SI", "���κ��Ͼ�"}, {"ZA", "��������ī"}, {"ES", "������"}, 
		{"SE", "������"}, {"CH", "������"}, {"TW", "�븸"}, {"TH", "Ÿ��"}, 
		{"NL", "�״�����"}, {"TN", "Ƣ����"}, {"TR", "��Ű"}, {"AE", "�ƶ����̸�Ʈ"}, 
		{"GB", "����"}, {"US", "�̱�"}, {"DJ", "�̱�"}, {"VE", "���׼�����"}, {"00", "��ü"},
		{"01", "��Ÿ"}, {"02", "MSCI"}
	};

	CString rValue = _T("");
	for (int ii = 0 ; ii < sizeof(ncodInfo) / sizeof(_ncodInfo) ; ii++)
	{
		if (ncodInfo[ii].name == sCountry)
		{
			rValue = ncodInfo[ii].code;
			break;
		}
	}

	return rValue;
}

void CUSCodeDlg::OnPaint() 
{
	CPaintDC dc(this);
}

void CUSCodeDlg::SetCodeList(int kind)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	m_CodeNameArray.RemoveAll();
	ReSizeCodeList(kind);

	switch (kind)
	{
	case IDC_BTN_ALL:
		{
			for (int ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if (pApp->m_arrayUSCode[ii]->datg == I_USI || pApp->m_arrayUSCode[ii]->datg == I_WRI
					|| pApp->m_arrayUSCode[ii]->datg == I_WMQ || pApp->m_arrayUSCode[ii]->datg == I_CME
					|| pApp->m_arrayUSCode[ii]->datg == I_ADR || pApp->m_arrayUSCode[ii]->datg == I_GDR
					|| pApp->m_arrayUSCode[ii]->jsgb & I_DOW || pApp->m_arrayUSCode[ii]->jsgb & I_NAS
					|| pApp->m_arrayUSCode[ii]->jsgb & I_SNP || pApp->m_arrayUSCode[ii]->datg == I_SCO
					|| pApp->m_arrayUSCode[ii]->datg == I_MAT || pApp->m_arrayUSCode[ii]->datg == I_GOF
					|| pApp->m_arrayUSCode[ii]->datg == I_LBI || pApp->m_arrayUSCode[ii]->datg == I_AMI
					|| pApp->m_arrayUSCode[ii]->datg == I_KOI || pApp->m_arrayUSCode[ii]->datg == I_EXR)
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
		}
		break;
	case IDC_BTN_USI: // �̱�����
		{
			for (int ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if ((pApp->m_arrayUSCode[ii]->datg == I_USI) || (atoi(CString(pApp->m_arrayUSCode[ii]->excd, 3)) == EXCHANGE_SNP))
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
		}
		break;
	case IDC_BTN_WRI: // ��������
		{
			for (int ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if ((pApp->m_arrayUSCode[ii]->datg == I_WRI) && (atoi(CString(pApp->m_arrayUSCode[ii]->excd, 3)) != EXCHANGE_SNP))
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
		}
		break;
	case IDC_BTN_AMQ: // �̱��ֿ�����
		{
			OnSelchangeCboSlist();
		}
		break;
	case IDC_BTN_WMQ: // �����ֿ�����
		{
			for (int ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if (pApp->m_arrayUSCode[ii]->datg == I_WMQ)
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
		}
		break;
	case IDC_BTN_CME: // CME����
		{
			for (int ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if (pApp->m_arrayUSCode[ii]->datg == I_CME)
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
		}
		break;
	case IDC_BTN_ADR: // �ؿ� ADR
		{
			for (int ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if (pApp->m_arrayUSCode[ii]->datg == I_ADR || pApp->m_arrayUSCode[ii]->datg == I_GDR)
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
		}
		break;
	case IDC_BTN_MAT: // ������
		{
			for (int ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if ((pApp->m_arrayUSCode[ii]->datg == I_SCO) || (pApp->m_arrayUSCode[ii]->datg == I_MAT))
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
		}
		break;
	case IDC_BTN_GOFUND: // ����ä
		{
			for (int ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if (pApp->m_arrayUSCode[ii]->datg == I_GOF)
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
		}
		break;
	case IDC_BTN_LIBOINTEREST: // �����ݸ�
		{
			for (int ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if (pApp->m_arrayUSCode[ii]->datg == I_LBI)
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
		}
		break;
	case IDC_BTN_KAINTEREST: // �ѹ̱ݸ�
		{
			for (int ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if ((pApp->m_arrayUSCode[ii]->datg == I_AMI) || (pApp->m_arrayUSCode[ii]->datg == I_KOI))
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
		}
		break;
	case IDC_BTN_EXRATE: // ȯ��
		{
			for (int ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{

				if (pApp->m_arrayUSCode[ii]->datg == I_EXR)
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
		}
		break;
	}

	if (kind != IDC_BTN_AMQ)
		ToggleName(m_nameFlag);
}

int CUSCodeDlg::GetItemUS(CString sItem)
{
	int rtnValue = NO_ALL;

	if (sItem == "��ü")
		rtnValue = NO_ALL;
	else if (sItem == "S&P500")
		rtnValue = NO_SNP;
	else if (sItem == "������100")
		rtnValue = NO_NAS;
	else if (sItem == "�ٿ�30")
		rtnValue = NO_DOW;

	return rtnValue;
}

int CUSCodeDlg::GetIndexUS(CString sIndex)
{
	int rtnValue = 0;

	if (sIndex == "��ü")
		rtnValue = NO_ALL;
	else if (sIndex == "AMEX")
		rtnValue = EXCHANGE_AMEX;
	else if (sIndex == "NASDAQ")
		rtnValue = EXCHANGE_NASDAQ;
	else if (sIndex == "NYSE")
		rtnValue = EXCHANGE_NYSE;
	else if (sIndex == "DOW")
		rtnValue = EXCHANGE_DOW;
	else if (sIndex == "S&P")
		rtnValue = EXCHANGE_SNP;
	else if (sIndex == "��常�轺")
		rtnValue = EXCHANGE_GOLDMAN;
	else if (sIndex == "�ʶ��Ǿ�")
		rtnValue = EXCHANGE_PHIL;

	return rtnValue;
}

void CUSCodeDlg::OnSelchangeCboSlist() 
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int ii = 0, jj = 0;
	int nCurSel = 0;

	nCurSel = m_cbo_slist.GetCurSel();

	m_CodeNameArray.RemoveAll();

	switch (m_nCurPos)
	{
	case 1 :	//�̱�����
		switch (GetIndexUS(m_ListIndexUS.GetAt(nCurSel)))
		{
		case NO_ALL: // ��ü
			for (ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if ((pApp->m_arrayUSCode[ii]->datg == I_USI) || (atoi(CString(pApp->m_arrayUSCode[ii]->excd, 3)) == EXCHANGE_SNP))
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
			break;
		case EXCHANGE_NYSE : 
			for (ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if (pApp->m_arrayUSCode[ii]->datg == I_USI && (atoi(CString(pApp->m_arrayUSCode[ii]->excd, 3)) == EXCHANGE_NYSE))
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
			break;
		case EXCHANGE_NASDAQ : 
			for (ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if (pApp->m_arrayUSCode[ii]->datg == I_USI && (atoi(CString(pApp->m_arrayUSCode[ii]->excd, 3)) == EXCHANGE_NASDAQ))
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
			break;
		case EXCHANGE_AMEX : 
			for (ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if (pApp->m_arrayUSCode[ii]->datg == I_USI)
				{
					if (atoi(CString(pApp->m_arrayUSCode[ii]->excd, 3)) == EXCHANGE_AMEX)
						AddCodeNameArray(pApp->m_arrayUSCode[ii]);
				}
			}
			break;
		case EXCHANGE_DOW : 
			for (ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if (pApp->m_arrayUSCode[ii]->datg == I_USI && (atoi(CString(pApp->m_arrayUSCode[ii]->excd, 3)) == EXCHANGE_DOW))
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
			break;
		case EXCHANGE_SNP : 
			for (ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if (atoi(CString(pApp->m_arrayUSCode[ii]->excd, 3)) == EXCHANGE_SNP)
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
			break;
		case EXCHANGE_GOLDMAN : 
			for (ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if (pApp->m_arrayUSCode[ii]->datg == I_USI && (CString(pApp->m_arrayUSCode[ii]->symb, 16) == GOLDMAN_1 || CString(pApp->m_arrayUSCode[ii]->symb, 16) == GOLDMAN_2 || CString(pApp->m_arrayUSCode[ii]->symb, 16) == GOLDMAN_3 || CString(pApp->m_arrayUSCode[ii]->symb, 16) == GOLDMAN_4 || CString(pApp->m_arrayUSCode[ii]->symb, 16) == GOLDMAN_5 || CString(pApp->m_arrayUSCode[ii]->symb, 16) == GOLDMAN_6))
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
			break;
		case EXCHANGE_PHIL : 
			for (ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if (pApp->m_arrayUSCode[ii]->datg == I_USI && (CString(pApp->m_arrayUSCode[ii]->symb, 16) == PHIL_1 || CString(pApp->m_arrayUSCode[ii]->symb, 16) == PHIL_2 || CString(pApp->m_arrayUSCode[ii]->symb, 16) == PHIL_3 || CString(pApp->m_arrayUSCode[ii]->symb, 16) == PHIL_4 || CString(pApp->m_arrayUSCode[ii]->symb, 16) == PHIL_5 || CString(pApp->m_arrayUSCode[ii]->symb, 16) == PHIL_6))
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
			break;
		}
		ReSizeCodeList(IDC_BTN_USI);
		break;
	case 2 :	//��������
		if (GetCountryCode(m_ListIndexGlobal.GetAt(nCurSel)) == "00")	//��ü
		{
			for (ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if ((pApp->m_arrayUSCode[ii]->datg == I_WRI) && (atoi(CString(pApp->m_arrayUSCode[ii]->excd, 3)) != EXCHANGE_SNP))
				{
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
				}
			}

		}
		//��Ÿ = ��ü - (ComboBox�� ���� ���� + MSCI)
		else if (GetCountryCode(m_ListIndexGlobal.GetAt(nCurSel)) == "01")	//��Ÿ
		{
			for (ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if ((pApp->m_arrayUSCode[ii]->datg == I_WRI) && (atoi(CString(pApp->m_arrayUSCode[ii]->excd, 3)) != EXCHANGE_SNP))
				{
					for (jj = 0; jj < m_ListIndexGlobal.GetSize(); jj++)
					{
						if (GetCountryCode(m_ListIndexGlobal.GetAt(jj)) != "00" && GetCountryCode(m_ListIndexGlobal.GetAt(jj)) != "01" && GetCountryCode(m_ListIndexGlobal.GetAt(jj)) != "02")
						{
							if ((CString(pApp->m_arrayUSCode[ii]->ncod, 2) == GetCountryCode(m_ListIndexGlobal.GetAt(jj))) || (atoi(CString(pApp->m_arrayUSCode[ii]->excd, 3)) == EXCHANGE_MSCI))
								break;
						}
					}
					if (jj >= m_ListIndexGlobal.GetSize())
						AddCodeNameArray(pApp->m_arrayUSCode[ii]);
				}
			}
		}
		else if (GetCountryCode(m_ListIndexGlobal.GetAt(nCurSel)) == "02")	//MSCI
		{
			for (ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if ((pApp->m_arrayUSCode[ii]->datg == I_WRI) && (atoi(CString(pApp->m_arrayUSCode[ii]->excd, 3)) == EXCHANGE_MSCI) && (atoi(CString(pApp->m_arrayUSCode[ii]->excd, 3)) != EXCHANGE_SNP))
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}
		}
		else
		{
			for (ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if ((pApp->m_arrayUSCode[ii]->datg == I_WRI) && (atoi(CString(pApp->m_arrayUSCode[ii]->excd, 3)) != EXCHANGE_MSCI) && (atoi(CString(pApp->m_arrayUSCode[ii]->excd, 3)) != EXCHANGE_SNP))
				{
					if (CString(pApp->m_arrayUSCode[ii]->ncod, 2) == GetCountryCode(m_ListIndexGlobal.GetAt(nCurSel)))
						AddCodeNameArray(pApp->m_arrayUSCode[ii]);
				}
			}
		}
		ReSizeCodeList(IDC_BTN_WRI);
		break;		
	case 3 :	//�̱�����
		switch (GetItemUS(m_ListItemUS.GetAt(nCurSel)))
		{
		case NO_ALL: // ��ü
			{
				for (int ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
				{
					if (pApp->m_arrayUSCode[ii]->jsgb & I_SNP || pApp->m_arrayUSCode[ii]->jsgb & I_NAS
						|| pApp->m_arrayUSCode[ii]->jsgb & I_DOW)
						AddCodeNameArray(pApp->m_arrayUSCode[ii]);
				}
			}
			break;
		case NO_SNP: // S&P500
			{
				for (int ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
				{
					if (pApp->m_arrayUSCode[ii]->jsgb & I_SNP)
						AddCodeNameArray(pApp->m_arrayUSCode[ii]);
				}
			}
			break;
		case NO_NAS: // ������100
			{
				for (int ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
				{
					if (pApp->m_arrayUSCode[ii]->jsgb & I_NAS)
						AddCodeNameArray(pApp->m_arrayUSCode[ii]);
				}
			}
			break;
		case NO_DOW: // �ٿ�30
			{
				for (int ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
				{
					if (pApp->m_arrayUSCode[ii]->jsgb & I_DOW)
						AddCodeNameArray(pApp->m_arrayUSCode[ii]);
				}
			}
			break;
		}
		ReSizeCodeList(IDC_BTN_AMQ);
		break;
	case 4 :	//��������
		if (GetCountryCode(m_ListItemGlobal.GetAt(nCurSel)) == "00")	//��ü
		{
			for (ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if (pApp->m_arrayUSCode[ii]->datg == I_WMQ)
					AddCodeNameArray(pApp->m_arrayUSCode[ii]);
			}

		}
		else
		{
			for (ii = 0 ; ii < pApp->m_arrayUSCode.GetSize() ; ii++)
			{
				if (pApp->m_arrayUSCode[ii]->datg == I_WMQ)
				{
					if (CString(pApp->m_arrayUSCode[ii]->ncod, 2) == GetCountryCode(m_ListItemGlobal.GetAt(nCurSel)))
						AddCodeNameArray(pApp->m_arrayUSCode[ii]);
				}
			}
		}
		ReSizeCodeList(IDC_BTN_WMQ);
		break;
	}
	
	ToggleName(m_nameFlag);
}

void CUSCodeDlg::ReSizeCodeList(int kind)
{
	switch (kind)
	{
	case IDC_BTN_USI:
	case IDC_BTN_AMQ:
	case IDC_BTN_WRI:
	case IDC_BTN_WMQ:
		{
			int	left = 109, top = 41;
			CRect	rc = CRect(0 + left, 0 + top, 240 + left, 246 + top);
			
			m_ListCtlCode.MoveWindow(rc);
			m_cbo_slist.ShowWindow(SW_SHOW);
		}
		break;
	default:
		{
			int	left = 109, top = 21;
			CRect	rc = CRect(0 + left, 0 + top, 240 + left, 266 + top);

			m_ListCtlCode.MoveWindow(rc);
			m_cbo_slist.ShowWindow(SW_HIDE);
		}
		break;
	}
}

void CUSCodeDlg::AddCodeNameArray(_USCode* usCode)
{
	_CodeName	CodeName;
	
	CodeName.symb = CString(usCode->symb, sizeof(usCode->symb));
	CodeName.knam = CString(usCode->knam, sizeof(usCode->knam));
	CodeName.enam = CString(usCode->enam, sizeof(usCode->enam));
	CString ncod = CString(usCode->ncod, sizeof(usCode->ncod));
	CodeName.cnam = GetCountry(ncod);

	CodeName.symb.TrimRight(); CodeName.knam.TrimRight();
	CodeName.enam.TrimRight(); CodeName.cnam.TrimRight();

	switch(m_nCurPos)
	{
	case 11: // ȯ��
		// ȯ���� ��/�޷��� �켱ǥ��
		if (CodeName.symb == "USDKRWSMBS") 
			m_CodeNameArray.InsertAt(0, CodeName);
		else
			m_CodeNameArray.Add(CodeName);
		break;
	default:
		//���� ���ɵǴ� ���� �켱ǥ��(�ٿ�����_���,���ͳ�,���,��ƿ��Ƽ, ������100����)
		if (CodeName.symb == "DJI@DJI" || CodeName.symb == "DJI@DJINET" || CodeName.symb == "DJI@DJT"
			|| CodeName.symb == "DJI@DJU" || CodeName.symb == "CME$ND")
			m_CodeNameArray.InsertAt(0, CodeName);
		else
			m_CodeNameArray.Add(CodeName);
		break;
	}
}

void CUSCodeDlg::ToggleName(int kind)
{
	ClearListCtlCode();

	switch (kind)
	{
	case NAME_KOR:
		{
			for (int ii = 0 ; ii < m_CodeNameArray.GetSize() ; ii++)
				AddListCtlCode(ii, m_CodeNameArray[ii].symb
				, m_CodeNameArray[ii].knam, m_CodeNameArray[ii].cnam);
		}
		break;
	case NAME_ENG:
		{
			for (int ii = 0 ; ii < m_CodeNameArray.GetSize() ; ii++)
				AddListCtlCode(ii, m_CodeNameArray[ii].symb
				, m_CodeNameArray[ii].enam, m_CodeNameArray[ii].cnam);
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
	m_ListCtlCode.InsertColumn(1, "�ڵ�", LVCFMT_LEFT, 66);
	m_ListCtlCode.InsertColumn(2, "�����", LVCFMT_LEFT, 120);
	m_ListCtlCode.InsertColumn(3, "������", LVCFMT_LEFT, 50);

	InitList();
}

void CUSCodeDlg::InitList()
{
	//Current Button State
	//������	= 0
	//�̱�����	= 1		
	//��������	= 2
	//�̱��ֿ�����	= 3
	//�����ֿ�����	= 4
	//CME����	= 5
	//�ؿ�		= 6
	int ii = 0;

	m_cbo_slist.ResetContent();
	
	LoadList();

	switch(m_nCurPos)	
	{
	case 1 :	//�̱�����
		for (ii = 0; ii < m_ListIndexUS.GetSize(); ii++)
			m_cbo_slist.AddString(m_ListIndexUS.GetAt(ii));
		break;
	case 2 :	//��������
		for (ii = 0; ii < m_ListIndexGlobal.GetSize(); ii++)
			m_cbo_slist.AddString(m_ListIndexGlobal.GetAt(ii));
		break;
	case 3 :	//�̱��ֿ�����
		for (ii = 0; ii < m_ListItemUS.GetSize(); ii++)
			m_cbo_slist.AddString(m_ListItemUS.GetAt(ii));
		break;
	case 4 :	//�����ֿ�����
		for (ii = 0; ii < m_ListItemGlobal.GetSize(); ii++)
			m_cbo_slist.AddString(m_ListItemGlobal.GetAt(ii));
		break;
	}
	m_cbo_slist.SetCurSel(0);
}

void CUSCodeDlg::LoadList()
{
	CFile inputFile;
	char twb[2048];
	CString sDir, sTmp, sData;
	DWORD dwRead;
	BYTE buffer[4096];

	sDir.Format("%s\\tab\\%s", m_sRoot, USLIST_FILE);

	m_ListIndexUS.RemoveAll();
	m_ListIndexGlobal.RemoveAll();
	m_ListItemUS.RemoveAll();
	m_ListItemGlobal.RemoveAll();

	//������ �������� �ʰų� ���� ������ ���� ��� Default �� �Է�
	if (inputFile.Open(sDir, CFile::modeRead) != 0)
	{
		dwRead = inputFile.Read(buffer, 4096);
		if (dwRead <= 0)
			SaveList(sDir);	//Insert Default List
		inputFile.Close();
	}
	else
		SaveList(sDir);	//Insert Default List

	//Session => [INDEX_US] [INDEX_GLOBAL] [ITEM_US] [ITEM_GLOBAL]
	memset(twb, NULL, sizeof(twb));
	//GetPrivateProfileString("INDEX_US", "PARAM", DEFAULT_INDEX_US, twb, sizeof(twb), sDir);
	sTmp = DEFAULT_INDEX_US;//sTmp = twb;
	sData = Parser(sTmp, ":");
	while(sData != "")
	{
		m_ListIndexUS.Add(sData);
		sData = Parser(sTmp, ":");
	}
	
	memset(twb, NULL, sizeof(twb));
	//GetPrivateProfileString("INDEX_GLOBAL", "PARAM", DEFAULT_INDEX_GLOBAL, twb, sizeof(twb), sDir);
	sTmp = DEFAULT_INDEX_GLOBAL;//sTmp = twb;
	sData = Parser(sTmp, ":");
	while(sData != "")
	{
		m_ListIndexGlobal.Add(sData);
		sData = Parser(sTmp, ":");
	}

	memset(twb, NULL, sizeof(twb));
	//GetPrivateProfileString("ITEM_US", "PARAM", DEFAULT_ITEM_US, twb, sizeof(twb), sDir);
	sTmp = DEFAULT_ITEM_US;//sTmp = twb;
	sData = Parser(sTmp, ":");
	while(sData != "")
	{
		m_ListItemUS.Add(sData);
		sData = Parser(sTmp, ":");
	}

	memset(twb, NULL, sizeof(twb));
	//GetPrivateProfileString("ITEM_GLOBAL", "PARAM", DEFAULT_ITEM_GLOBAL, twb, sizeof(twb), sDir);
	sTmp = DEFAULT_ITEM_GLOBAL;//sTmp = twb;
	sData = Parser(sTmp, ":");
	while(sData != "")
	{
		m_ListItemGlobal.Add(sData);
		sData = Parser(sTmp, ":");
	}
}

void CUSCodeDlg::SaveList(CString sDir)
{
	/*
	WritePrivateProfileString("INDEX_US", "PARAM", DEFAULT_INDEX_US, sDir);
	WritePrivateProfileString("INDEX_GLOBAL", "PARAM", DEFAULT_INDEX_GLOBAL, sDir);
	WritePrivateProfileString("ITEM_US", "PARAM", DEFAULT_ITEM_US, sDir);
	WritePrivateProfileString("ITEM_GLOBAL", "PARAM", DEFAULT_ITEM_GLOBAL, sDir);
	*/
}



CString CUSCodeDlg::Parser(CString &srcstr, CString substr)
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
	strcpy(Font.lfFaceName, "����");

	return Font;
}

void CUSCodeDlg::AddListCtlCode(int nItem, CString szCode, CString szName, CString CName)
{
	szCode.TrimLeft(); szCode.TrimRight(); szName.TrimLeft(); szName.TrimRight();

	_ListSort* pListSort = new _ListSort;
	ZeroMemory(pListSort, sizeof(_ListSort));

	strcpy((char*)pListSort->Code, szCode);
	strcpy((char*)pListSort->Name, szName);
	strcpy((char*)pListSort->CName, CName);
	m_listSort.Add(pListSort);

	int rItem;
	rItem = m_ListCtlCode.InsertItem(LVIF_TEXT|LVIF_PARAM, nItem, szCode
		, NULL, NULL, 0, (LPARAM) pListSort);

	m_ListCtlCode.SetItemText(rItem, 1, szName);
	m_ListCtlCode.SetItemText(rItem, 2, CName);
}

void CUSCodeDlg::ClearListCtlCode(int nItem /* = -1 */)
{
	_ListSort*	pListSort = NULL;
	if (nItem < 0)
	{
		int nCount = m_ListCtlCode.GetItemCount();
		for (int ii = 0 ; ii < nCount ; ii++)
		{
			pListSort = (_ListSort*) m_ListCtlCode.GetItemData(ii);
			SAFE_DELETE(pListSort);
		}
		m_ListCtlCode.DeleteAllItems();
	}
	else
	{
		pListSort = (_ListSort*) m_ListCtlCode.GetItemData(nItem);
		SAFE_DELETE(pListSort);
		m_ListCtlCode.DeleteItem(nItem);
	}
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
				switch ((LPARAM)pnm->iSubItem)
				{
				case 0:
					m_listsort = !m_listsort;
					break;
				case 1:
					m_listsort = !m_listsort;
					break;
				case 2:
					m_listsort = !m_listsort;
					break;
				}
				ListView_SortItems(pnm->hdr.hwndFrom, ListViewCompareFunc3, (LPARAM)pnm->iSubItem);
				#undef pnm
				break;
			}			
		}
		break;
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CUSCodeDlg::SetBtn()
{
	short	shBtnColor = 245;

	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	CString strImg("");
	strImg.Format("%s/%s/", pApp->m_RootPath, "image");

	for (int ii = IDC_BTN_SORT01 ; ii <= IDC_BTN_EXRATE ; ii++)
	{
		//((CButtonST*)GetDlgItem(ii))->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		//((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
		//((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, USER_FOCUS_COR);
		//((CButtonST*)GetDlgItem(ii))->SetWnd(this);
		((CfxButton*)GetDlgItem(ii))->SetBtnCursor(IDC_CURSOR_HAND);
		((CfxButton*)GetDlgItem(ii))->LoadImg(strImg);
	}
}

void CUSCodeDlg::OnClickBtnKind(UINT nID)
{
	m_nCurPos = nID - IDC_BTN_ALL;

	InitList();

	for (int ii = IDC_BTN_ALL ; ii <= IDC_BTN_EXRATE ; ii++)
	{
		if ((int)nID == ii)
		{
			((CfxButton*)GetDlgItem(ii))->SetBold(TRUE);
			((CfxButton*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
			((CfxButton*)GetDlgItem(ii))->SetSelected(TRUE);
		}
		else
		{
			((CfxButton*)GetDlgItem(ii))->SetBold(FALSE);
			((CfxButton*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
			((CfxButton*)GetDlgItem(ii))->SetSelected(FALSE);
		}
	}
	
	SetCodeList(nID);
}

void CUSCodeDlg::OnClickBtnSort(UINT nID)
{
/*
	for (int ii = IDC_BTN_SORT01 ; ii <= IDC_BTN_ENG ; ii++)
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
*/
	switch (nID)
	{
	case IDC_BTN_ENG:
		{
			m_nameFlag = !m_nameFlag;
			ToggleName(m_nameFlag);
			if (m_nameFlag)
				((CfxButton*)GetDlgItem(nID))->SetWindowText(BTN_KOR);
			else
				((CfxButton*)GetDlgItem(nID))->SetWindowText(BTN_ENG);
		}
		break;
	default:
		{
			if (!m_nameFlag)
			{
				struct _sortInfo {
					char*	str;
					int	len;
				} sortInfo[] = {
					{"��", 2}, {"��", 2}, {"��", 2}, {"��", 2}, {"��", 2},
					{"��", 2}, {"��", 2}, {"��", 2}, {"��", 2}, {"��", 2},
					{"ī", 2}, {"Ÿ", 2}, {"��", 2}, {"��", 2}, {"A", 1}, {"1", 1}
				};

				int bSel = nID - IDC_BTN_SORT01;

				m_listsort = FALSE;
				ListView_SortItems(m_ListCtlCode, ListViewCompareFunc3,1);
				setSelected(&m_ListCtlCode, findIndex(sortInfo[bSel].str, sortInfo[bSel].len));
			}
		}
		break;
	}
}

int CUSCodeDlg::findIndex(CString text, int len, int column /* = 1 */)
{
	CString	strTmp;
	CRect	rc;
	CSize	size;

	for (int ii = 0 ; ii < m_ListCtlCode.GetItemCount() ; ii++)
	{
		strTmp = m_ListCtlCode.GetItemText(ii, column);
		if (strncmp(text, strTmp, len) == 0)
		{
			killSelected(&m_ListCtlCode);
			return ii;
		}
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
	if (pos != NULL)
	{
		while (pos)
		{
			int	nItem;
			nItem = ListCtrl->GetNextSelectedItem(pos);
			ListCtrl->SetItemState(nItem, NULL, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

void CUSCodeDlg::OnChangeEditSearch() 
{
	CString str;
	m_edit_search.GetWindowText(str);

	m_listsort = FALSE;
	ListView_SortItems(m_ListCtlCode, ListViewCompareFunc3,1);
	setSelected(&m_ListCtlCode, findIndex(str, str.GetLength()));	
}

void CUSCodeDlg::OnDblclkListCode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int	nItem;
	CString	code, name;
	POSITION pos = m_ListCtlCode.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
///		while (pos)
//		{
			nItem = m_ListCtlCode.GetNextSelectedItem(pos);

			code = m_ListCtlCode.GetItemText(nItem, 0);
			name = m_ListCtlCode.GetItemText(nItem, 1);

			m_rtnData.code = code;
			m_rtnData.name = name;
//		}
	}


	CDialog::OnOK();
	*pResult = 0;
}

CString CUSCodeDlg::GetRtnData(bool dialog /* = true */, CString code /* = _T("") */)
{
	CString rValue = _T("");
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	if (dialog)
	{
		if (!m_rtnData.code.IsEmpty() && !m_rtnData.name.IsEmpty())
			rValue.Format("%s%c%s", m_rtnData.code, cTAB, m_rtnData.name);

	}
	else
	{
		CString	symb, name;
		int	nSize = (int)pApp->m_arrayUSCode.GetSize();
		for (int ii = 0 ; ii < nSize ; ii++)
		{
			symb = CString(pApp->m_arrayUSCode[ii]->symb, sizeof(pApp->m_arrayUSCode[ii]->symb));
			if (code.Compare(symb) == 0)
			{
				name = CString(pApp->m_arrayUSCode[ii]->knam, sizeof(pApp->m_arrayUSCode[ii]->knam));
				rValue.Format("%s%c%s", code, cTAB, name);
			}
		}
	}
	
	return rValue;
}

/*
HBRUSH CUSCodeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here	
	DeleteObject(m_hBrush);

	// TODO: Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		m_hBrush = CreateSolidBrush(USER_BACKCOLOR);
		
		return m_hBrush;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
*/
