// StgPage.cpp : implementation file
//

#include "stdafx.h"
#include "IB410100.h"
#include "StgPage.h"
#include "StgBoard.h"
#include "StgInfo.h"
#include "MGrpWnd.h"
#include "message.h"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../control/fx_grid/fxgrid.h"
#include  "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STG_FILE	"Strategy.ini"

#define BOARD_WIDTH	255
#define GRID_WIDTH	261
#define	GRID_HEIGHT	76

#define GRID_ROWH	19

struct	_stggridHdr {
	char*	text;		// fixed header
	UINT	width;		// fixed width
	UINT	attr;		// fixde attr

	char	symbol[8];	// nonfixed symbol
	UINT	format;		// nonfixed format
	UINT	attrx;		// nonfixed attr
	char*	defaultVal;
} stggridHdr[] = {
	{ "�����ڵ�",	0,	0,	"",		0,		0},			// 0	
	{ "�����",	150,	0,	"",		GVFM_LEFT,	0},			// 1
	{ "����",	45,	0,	"",		GVFM_CENTER,	GVAT_CONDITIONx},	// 2
	{ "����",	49,	0,	"",		GVFM_RIGHT,	GVAT_COMMA},		// 3
	//{ "���簡",	40,	0,	"",		GVFM_RIGHT,	GVAT_CONDITIONx},	// 4
};

const int gCode = 0, gName = 1, gGB = 2, gQty = 3;//, gPrc = 4;

/////////////////////////////////////////////////////////////////////////////
// CStgPage property page
[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CStgPage, CPropertyPage)

CStgPage::CStgPage(CWnd* pView, CWnd* pParent) : CPropertyPage(CStgPage::IDD)
{
	//{{AFX_DATA_INIT(CStgPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pApp = (CIB410100App *)AfxGetApp();
	m_pParent = pParent;
	m_pView = pView;
	m_pStgInfo = NULL;
	m_nStgCode = -1;
	m_pPFGrid = NULL;
	m_pGrpWnd = NULL;
}

CStgPage::~CStgPage()
{
}

void CStgPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStgPage)
	DDX_Control(pDX, IDC_BTN_ADDSTG, m_btnAddStg);
	DDX_Control(pDX, IDC_BTN_SETSTG, m_btnSetStg);
	DDX_Control(pDX, IDC_EDT_STGINFO, m_edtStgInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStgPage, CPropertyPage)
	//{{AFX_MSG_MAP(CStgPage)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_ADDSTG, OnBtnAddstg)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_SETSTG, OnClickStgBtn)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+1, OnUser)
	ON_MESSAGE(WM_STGInitData, OnInitData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStgPage message handlers

int CStgPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CStgPage::OnDestroy() 
{
	//DELWND(m_pPFGrid)
	//DELWND(m_pStgInfo);
	//DELWND(m_pGrpWnd);

	CPropertyPage::OnDestroy();
}


long CStgPage::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case MSG_SELECTSTG:  // CStgBoard������ ��������
		m_nStgCode = lParam;
		initData();
		makeCode(m_nStgCode);
		setCodeInfo(m_nStgCode);
		setGrid();
		//makeChartInfo(m_nStgCode);
		m_pGrpWnd->m_nStgCode = m_nStgCode;
		m_pGrpWnd->Redraw();
		break;
	}

	return 0;
}

BOOL CStgPage::OnSetActive() 
{
	//::SetWindowPos( m_hWnd, HWND_TOP, 1, 1, 0, 0, SWP_NOSIZE); 
	CRect rcParent;
	GetParent()->GetClientRect(&rcParent);
	::SetWindowPos( m_hWnd, HWND_TOP, rcParent.left, rcParent.top, rcParent.right, rcParent.bottom, 0); 

	if (m_nStgCode == -1)
	{
		OnUser(MSG_SELECTSTG, 1);
	}

	return CPropertyPage::OnSetActive();
}

void CStgPage::addPCode(CString Code, int Num, int mmgb)
{
	_ProjectCode	ProjectCode;

	ProjectCode.Code = Code;
	ProjectCode.Num = Num;
	ProjectCode.mmgb = mmgb;
	ProjectCode.jprc = 0;

// 	CString strTemp;
// 	strTemp.Format("[KSJ] addPCode code[%s] num[%d] mmgb[%d]", Code, Num, mmgb);
// 	OutputDebugString(strTemp);

	m_projectCode.Add(ProjectCode);
}
/*
void CStgPage::addCCode(int ctype, int atmgap, int Num, int mmgb)
{
	_MChartCode chartCode;

	chartCode.ctype = ctype;	// ���� 1, �� 2, ǲ 3
	chartCode.atmgap= atmgap;	// atm���� ����
	chartCode.Num	= Num;	// ����
	chartCode.mmgb	= mmgb;	// �Ÿű��� (-1. �ŵ�, +1. �ż�)

	m_pGrpWnd->m_chartCode.Add(chartCode);
}
*/
void CStgPage::initData()
{
	m_MakeCodeData.hsga.RemoveAll();
		
	CString atmCallCode;
	m_MakeCodeData.fcode.Format("%s",
		(char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(getCODEfuture, 0)), 0));
	atmCallCode.Format("%s",
		(char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(getCODEcall, 0)), 0));

// 	OutputDebugString("[KSJ] m_MakeCodeData[" + m_MakeCodeData.fcode + "] atmCallCode[" + atmCallCode + "]");

	m_MakeCodeData.year  = m_MakeCodeData.fcode.Mid(3, 1);
	m_MakeCodeData.month = atmCallCode.Mid(4,1);
	m_MakeCodeData.atmg  = ((CMapWnd*)m_pParent)->m_atmCall;// atmCallCode;
	((CMapWnd*)m_pParent)->GetHedgeCode(m_MakeCodeData.month.GetAt(0));
	
	CString opcode;
	for (int ii = 0 ; ii < ((CMapWnd*)m_pParent)->m_OPCodeList.GetSize() ; ii++)
	{
		_codelist *pcodelist = ((CMapWnd*)m_pParent)->m_OPCodeList.GetAt(ii);
		opcode.Format("%s", pcodelist->code); opcode.TrimRight();
		if (opcode.Mid(0, 1) == "2" && opcode.Mid(4, 1) == m_MakeCodeData.month)
			m_MakeCodeData.hsga.Add(opcode);
	}
}

void CStgPage::makeCode(int Index)
{
	// 1 : ����, 2 : ��, 3 : ǲ
	// +1 = atm + 2.5, -1 = atm - 2.5
	// -1 : �ŵ�, +1 : �ż�

	m_projectCode.RemoveAll();

	switch (Index)
	{
	case 1: // �ݸż�
		addPCode(getCode(2, getHsga(0)), 1, +1);
		break;
	case 2: // �ݹ齺������
		addPCode(getCode(2, getHsga(+1)), 2, +1);
		addPCode(getCode(2, getHsga(0)), 1, -1);
		break;
	case 3: // ��Ʈ���ż�
		addPCode(getCode(3, getHsga(0)), 1, +1);
		addPCode(getCode(2, getHsga(0)), 2, +1);
		break;
	case 4: // �����ż�
		addPCode(getCode(1, getHsga(0)), 1, +1);
		break;
	case 5: // �����ݽ�������
		addPCode(getCode(2, getHsga(0)), 1, +1);
		addPCode(getCode(2, getHsga(+2)), 1, -1);
		break;
	case 6: // ����ǲ��������
		addPCode(getCode(3, getHsga(-2)), 1, +1);
		addPCode(getCode(3, getHsga(0)), 1, -1);
		break;
	case 7: // ǲ�ŵ�
		addPCode(getCode(3, getHsga(0)), 1, -1);
		break;
	case 8: // ��Ʈ����ż�
		addPCode(getCode(3, getHsga(0)), 1, +1);
		addPCode(getCode(2, getHsga(0)), 1, +1);
		break;
	case 9: // ��Ʈ���۸ż�
		addPCode(getCode(3, getHsga(-2)), 1, +1);
		addPCode(getCode(2, getHsga(+2)), 1, +1);
		break;
	case 10: // ��Ʈ�ż�
		addPCode(getCode(2, getHsga(-1)), 1, +1);
		addPCode(getCode(3, getHsga(+1)), 1, +1);
		break;
	case 11: // �ܵ��ŵ�
		addPCode(getCode(3, getHsga(-1)), 1, +1);
		addPCode(getCode(2, getHsga(+1)), 1, +1);
		addPCode(getCode(3, getHsga(-2)), 1, -1);
		addPCode(getCode(2, getHsga(+2)), 1, -1);
		break;
	case 12: // �����ö��̸ŵ�
		addPCode(getCode(3, getHsga(0)), 1, +1);
		addPCode(getCode(2, getHsga(0)), 1, +1);
		addPCode(getCode(3, getHsga(-2)), 1, -1);
		addPCode(getCode(2, getHsga(+2)), 1, -1);
		break;
	case 13: // �����ö��̸ż�
		addPCode(getCode(3, getHsga(-2)), 1, +1);
		addPCode(getCode(2, getHsga(+2)), 1, +1);
		addPCode(getCode(3, getHsga(0)), 1, -1);
		addPCode(getCode(2, getHsga(0)), 1, -1);
		break;
	case 14: // ��Ʈ����ŵ�
		addPCode(getCode(3, getHsga(0)), 1, -1);
		addPCode(getCode(2, getHsga(0)), 1, -1);
		break;
	case 15: // ��Ʈ���۸ŵ�
		addPCode(getCode(3, getHsga(-2)), 1, -1);
		addPCode(getCode(2, getHsga(+2)), 1, -1);
		break;
	case 16: // �ݷ��̼������
		addPCode(getCode(2, getHsga(0)), 1, +1);
		addPCode(getCode(2, getHsga(+1)), 2, -1);
		break;
	case 17: // ǲ���̼������
		addPCode(getCode(3, getHsga(0)), 1, +1);
		addPCode(getCode(3, getHsga(-1)), 2, -1);
		break;
	case 18: // �ܵ��ż�
		addPCode(getCode(3, getHsga(-2)), 1, +1);
		addPCode(getCode(2, getHsga(+2)), 1, +1);
		addPCode(getCode(3, getHsga(-1)), 1, -1);
		addPCode(getCode(2, getHsga(+1)), 1, -1);
		break;
	case 19: // ��Ʈ�ŵ�
		addPCode(getCode(3, getHsga(+1)), 1, -1);
		addPCode(getCode(2, getHsga(-1)), 1, -1);
		break;
	case 20: // ǲ�ż�
		addPCode(getCode(3, getHsga(0)), 1, +1);
		break;
	case 21: // ǲ�齺������
		//addPCode(getCode(3, getHsga(-1)), 2, +1);
		//addPCode(getCode(3, getHsga(0)), 1, -1);
		addPCode(getCode(3, getHsga(-2)), 2, +1);
		addPCode(getCode(3, getHsga(0)), 1, -1);
		break;
	case 22: // ��Ʈ���ż�
		addPCode(getCode(3, getHsga(0)), 2, +1);
		addPCode(getCode(2, getHsga(0)), 1, +1);
		break;
	case 23: // �����ŵ�
		addPCode(getCode(1, getHsga(0)), 1, -1);
		break;
	case 24: // �༼�ݽ�������
		addPCode(getCode(2, getHsga(+2)), 1, +1);
		addPCode(getCode(2, getHsga(0)), 1, -1);
		break;
	case 25: // �༼ǲ��������
		addPCode(getCode(3, getHsga(0)), 1, +1);
		addPCode(getCode(3, getHsga(-2)), 1, -1);
		break;
	case 26: // �ݸŵ�
		addPCode(getCode(2, getHsga(0)), 1, -1);
		break;
	case 27: // ��ǲ���̼��ռ�
		addPCode(getCode(3, getHsga(-1)), 1, +1);
		addPCode(getCode(2, getHsga(+1)), 1, +1);
		addPCode(getCode(3, getHsga(-2)), 2, -1);
		addPCode(getCode(2, getHsga(+2)), 2, -1);
		break;
	case 28: // ������
		addPCode(getCode(3, getHsga(0)), 5, +1);
		addPCode(getCode(2, getHsga(0)), 5, -1);
		addPCode(getCode(1, getHsga(0)), 1, +1);
		break;
	case 29: // ������
		addPCode(getCode(2, getHsga(0)), 5, +1);
		addPCode(getCode(3, getHsga(0)), 5, -1);
		addPCode(getCode(1, getHsga(0)), 1, -1);
		break;
	case 30: // �����ڽ�
		addPCode(getCode(2, getHsga(0)), 1, -1);
		addPCode(getCode(3, getHsga(+1)), 1, -1);
		addPCode(getCode(3, getHsga(0)), 1, +1);
		addPCode(getCode(2, getHsga(+1)), 1, +1);
		break;
	case 31: // Ŀ�����ݸŵ�
		addPCode(getCode(2, getHsga(0)), 5, -1);
		addPCode(getCode(1, getHsga(0)), 1, +1);
		break;
	case 32: // �������ǲ�ż�
		addPCode(getCode(3, getHsga(0)), 5, +1);
		addPCode(getCode(1, getHsga(0)), 1, +1);
		break;
	case 33: // ũ�����ڽ�
		addPCode(getCode(2, getHsga(0)), 1, +1);
		addPCode(getCode(3, getHsga(+1)), 1, +1);
		addPCode(getCode(3, getHsga(0)), 1, -1);
		addPCode(getCode(2, getHsga(+1)), 1, -1);
		break;
	case 34: // �ݹ����ö��̸ż�	//2014.04.16 KSJ �߰�
		addPCode(getCode(2, getHsga(-2)), 1, +1);
		addPCode(getCode(2, getHsga(+2)), 1, +1);
		addPCode(getCode(2, getHsga(0)), 2, -1);
		break;
	case 35: // ǲ�����ö��̸ż�	//2014.04.16 KSJ �߰�
		addPCode(getCode(3, getHsga(-2)), 1, +1);
		addPCode(getCode(3, getHsga(+2)), 1, +1);
		addPCode(getCode(3, getHsga(0)), 2, -1);
		break;
	}
}
/*
void CStgPage::makeChartInfo(int Index)
{
	// 1 : ����, 2 : ��, 3 : ǲ
	// +1 = atm + 2.5, -1 = atm - 2.5
	// -1 : �ŵ�, +1 : �ż�
	
	m_pGrpWnd->m_chartCode.RemoveAll();

	switch (Index)
	{
	case 1: // �ݸż�
		addCCode(2, 0, 1, +1);
		break;
	case 2: // �ݹ齺������
		addCCode(2, +1, 2, +1);
		addCCode(2, 0, 1, -1);
		break;
	case 3: // ��Ʈ���ż�
		addCCode(3, 0, 1, +1);
		addCCode(2, 0, 2, +1);
		break;
	case 4: // �����ż�
		addCCode(1, 0, 1, +1);
		break;
	case 5: // �����ݽ�������
		addCCode(2, 0, 1, +1);
		addCCode(2, +2, 1, -1);
		break;
	case 6: // ����ǲ��������
		addCCode(3, -2, 1, +1);
		addCCode(3, 0, 1, -1);
		break;
	case 7: // ǲ�ŵ�
		addCCode(3, 0, 1, -1);
		break;
	case 8: // ��Ʈ����ż�
		addCCode(3, 0, 1, +1);
		addCCode(2, 0, 1, +1);
		break;
	case 9: // ��Ʈ���۸ż�
		addCCode(3, -2, 1, +1);
		addCCode(2, +2, 1, +1);
		break;
	case 10: // ��Ʈ�ż�
		addCCode(2, -1, 1, +1);
		addCCode(3, +1, 1, +1);
		break;
	case 11: // �ܵ��ŵ�
		addCCode(3, -1, 1, +1);
		addCCode(2, +1, 1, +1);
		addCCode(3, -2, 1, -1);
		addCCode(2, +2, 1, -1);
		break;
	case 12: // �����ö��̸ŵ�
		addCCode(3, 0, 1, +1);
		addCCode(2, 0, 1, +1);
		addCCode(3, -2, 1, -1);
		addCCode(2, +2, 1, -1);
		break;
	case 13: // �����ö��̸ż�
		addCCode(3, -2, 1, +1);
		addCCode(2, +2, 1, +1);
		addCCode(3, 0, 1, -1);
		addCCode(2, 0, 1, -1);
		break;
	case 14: // ��Ʈ����ŵ�
		addCCode(3, 0, 1, -1);
		addCCode(2, 0, 1, -1);
		break;
	case 15: // ��Ʈ���۸ŵ�
		addCCode(3, -2, 1, -1);
		addCCode(2, +2, 1, -1);
		break;
	case 16: // �ݷ��̼������
		addCCode(2, 0, 1, +1);
		addCCode(2, +1, 2, -1);
		break;
	case 17: // ǲ���̼������
		addCCode(3, 0, 1, +1);
		addCCode(3, -1, 2, -1);
		break;
	case 18: // �ܵ��ż�
		addCCode(3, -2, 1, +1);
		addCCode(2, +2, 1, +1);
		addCCode(3, -1, 1, -1);
		addCCode(2, +1, 1, -1);
		break;
	case 19: // ��Ʈ�ŵ�
		addCCode(3, +1, 1, -1);
		addCCode(2, -1, 1, -1);
		break;
	case 20: // ǲ�ż�
		addCCode(3, 0, 1, +1);
		break;
	case 21: // ǲ�齺������
		addCCode(3, -1, 2, +1);
		addCCode(3, 0, 1, -1);
		break;
	case 22: // ��Ʈ���ż�
		addCCode(3, 0, 2, +1);
		addCCode(2, 0, 1, +1);
		break;
	case 23: // �����ŵ�
		addCCode(1, 0, 1, -1);
		break;
	case 24: // �༼�ݽ�������
		addCCode(2, +2, 1, +1);
		addCCode(2, 0, 1, -1);
		break;
	case 25: // �༼ǲ��������
		addCCode(3, 0, 1, +1);
		addCCode(3, -2, 1, -1);
		break;
	case 26: // �ݸŵ�
		addCCode(2, 0, 1, -1);
		break;
	case 27: // ��ǲ���̼��ռ�
		addCCode(3, -1, 1, +1);
		addCCode(2, +1, 1, +1);
		addCCode(3, -2, 2, -1);
		addCCode(2, +2, 2, -1);
		break;
	case 28: // ������
		addCCode(3, 0, 5, +1);
		addCCode(2, 0, 5, -1);
		addCCode(1, 0, 1, +1);
		break;
	case 29: // ������
		addCCode(2, 0, 5, +1);
		addCCode(3, 0, 5, -1);
		addCCode(1, 0, 1, -1);
		break;
	case 30: // �����ڽ�
		addCCode(2, 0, 1, -1);
		addCCode(3, +1, 1, -1);
		addCCode(3, 0, 1, +1);
		addCCode(2, +1, 1, +1);
		break;
	case 31: // Ŀ�����ݸŵ�
		addCCode(2, 0, 5, -1);
		addCCode(1, 0, 1, +1);
		break;
	case 32: // �������ǲ�ż�
		addCCode(3, 0, 5, +1);
		addCCode(1, 0, 1, +1);
		break;
	case 33: // ũ�����ڽ�
		addCCode(2, 0, 1, +1);
		addCCode(3, +1, 1, +1);
		addCCode(3, 0, 1, -1);
		addCCode(2, +1, 1, -1);
		break;
	}
}
*/
void CStgPage::setCodeInfo(int Index)
{
	if (Index <= 0)
	{
		m_edtStgInfo.SetWindowText("");
		return;
	}

	CString sfile, ssection, sname, scase, sinfo, stext;
	CFileFind find;
	sfile.Format("%s\\%s\\%s", ((CMapWnd*)m_pParent)->m_sRoot, TABDIR, STG_FILE);
	ssection.Format("STG%d", Index);

	CRect rcEdit;
	m_edtStgInfo.GetClientRect(&rcEdit); rcEdit.DeflateRect(10, 10);
	m_edtStgInfo.SetRect(rcEdit);

	char buf[1024];
	GetPrivateProfileString(ssection, "Name", "", buf, 1024, sfile); sname.Format("%s", buf);
	GetPrivateProfileString(ssection, "Case", "", buf, 1024, sfile); scase.Format("%s", buf);
	GetPrivateProfileString(ssection, "Info", "", buf, 1024, sfile); sinfo.Format("%s", buf);
	stext.Format("[%s]\r\n\r\n1.��������ñ�:\r\n\r\n%s\r\n\r\n\r\n2.��������:\r\n\r\n%s", sname, scase, sinfo);
	m_edtStgInfo.SetWindowText(stext);
}

CString CStgPage::getCode(int type, CString hsga /* = _T("") */)
{
	CString rValue;

	switch (type)
	{
	case 1:
		rValue = m_MakeCodeData.fcode;
		break;
	case 2:	
		rValue = CString("201" + m_MakeCodeData.year + m_MakeCodeData.month + hsga);
		break;
	case 3:	
		rValue = CString("301" + m_MakeCodeData.year + m_MakeCodeData.month + hsga);
		break;
	}

// 	CString sTest;
// 	sTest.Format("[KSJ] IC getCode[%s] year[%s] month[%s] haga[%s]\n", rValue, m_MakeCodeData.year, m_MakeCodeData.month, hsga);
// 	OutputDebugString(sTest);
	
	return rValue;
}

CString CStgPage::getHsga(int nItem)
{
	int NoHsga = 0;
	const int nhsga = m_MakeCodeData.hsga.GetSize();
// CString sTest;sTest.Format("[KSJ] IC ====================(%s) \n", m_MakeCodeData.atmg);OutputDebugString(sTest);
	for (int ii = 0 ; ii < nhsga ; ii++)
	{
// sTest.Format("[KSJ] IC m_MakeCodeData.hsga %d (%s) \n", ii, m_MakeCodeData.hsga.GetAt(ii));OutputDebugString(sTest);
		if (m_MakeCodeData.atmg == m_MakeCodeData.hsga.GetAt(ii))
		{
			NoHsga = ii + nItem;
			break;
		}
	}

	if (nItem == 0 && m_MakeCodeData.atmg.GetLength() > 0)
	{
		return m_MakeCodeData.atmg.Mid(5, 3);
	}
	else if (NoHsga > 0 && NoHsga < nhsga)
	{
		return m_MakeCodeData.hsga.GetAt(NoHsga).Mid(5, 3);
	}

	return "";
}

CString CStgPage::monthCode(int month)
{
	CString rValue;
	rValue.Format("%d", month);

	if (month >= 10)
	{
		switch (month)
		{
		case 10:
			rValue = "A";
			break;
		case 11:
			rValue = "B";
			break;
		case 12:
			rValue = "C";
			break;
		}
	}

	return rValue;
}

void CStgPage::OnClickStgBtn()
{
	if (m_projectCode.GetSize() <= 0)
		return;

	_userMsg	userMsg;
	userMsg.kind = TR_StgApply;
	userMsg.nSize = m_projectCode.GetSize();

	m_pParent->SendMessage(WM_MAPStgApply, (WPARAM) &m_projectCode[0], (LPARAM) (char*) &userMsg);	
}

void CStgPage::OnBtnAddstg() 
{
	_userMsg	userMsg;
	userMsg.kind = TR_StgAdd;
	userMsg.nSize = m_projectCode.GetSize();

	m_pParent->SendMessage(WM_MAPStgAdd, (WPARAM) &m_projectCode[0], (LPARAM) (char*) &userMsg);	
}

void CStgPage::initGrid()
{
	const int count = sizeof(stggridHdr) / sizeof(_stggridHdr);
	m_pPFGrid->Initial(5, count, 1, 0);

	LOGFONT lf; m_pFont->GetLogFont(&lf); lf.lfHeight = 90;
	GVITEM	gvitem;
	CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

 	// column header setting
	for (int ii = 0; ii < count; ii++)
	{
		gvitem.row   = 0;
		gvitem.col   = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_MISC;
		gvitem.format= GVFM_HEADER;
		gvitem.fgcol = m_pApp->GetIndexColor(m_pView, 76);
		gvitem.bkcol = m_pApp->GetIndexColor(m_pView, 74);

		gvitem.text  = stggridHdr[ii].text;
		gvitem.attr  = stggridHdr[ii].attr;
		m_pPFGrid->SetItem(&gvitem);
	};

	for (int ii = 0; ii < count; ii++)
		m_pPFGrid->SetColumnWidth(ii, stggridHdr[ii].width);

	for (int ii = 0; ii < count; ii++)
	{
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;

		gvitem.format= stggridHdr[ii].format;
		gvitem.symbol= stggridHdr[ii].symbol; 
		gvitem.attr  = stggridHdr[ii].attrx;
		gvitem.fgcol = m_pApp->GetIndexColor(m_pView, 69);
		gvitem.bkcol = m_pApp->GetIndexColor(m_pView, 68);

		m_pPFGrid->SetColumnItems(ii, &gvitem);
	}

	for (int ii = 0; ii < m_pPFGrid->GetRowCount(); ii++)
		m_pPFGrid->SetRowHeight(ii, GRID_ROWH);

	//m_pPFGrid->Setm_pPFGridDIScroll(false);
	//m_pPFGrid->Setm_pPFGridScroll(false);
	m_pPFGrid->SetFocusCellSensitive(false);
	m_pPFGrid->SetStepColor(1, m_pApp->GetIndexColor(m_pView, 68), m_pApp->GetIndexColor(m_pView, 77));

	m_pPFGrid->SetGridLine(GVLN_BOTH);
	m_pPFGrid->SetGridScroll(GVSC_VERT);
	m_pPFGrid->SetRowColResize(FALSE, FALSE);
	m_pPFGrid->SetGridFocusLine(FALSE, TRUE);
	m_pPFGrid->SetGridColor(m_pApp->GetIndexColor(m_pView, 65));
	m_pPFGrid->SetBKSelColor(m_pApp->GetIndexColor(m_pView, 78));
	//m_pPFGrid->SetFGSelColor(m_pApp->GetIndexColor(m_pView, 65));
	m_pPFGrid->Adjust();
}

void CStgPage::initGridRow(int row)
{
	LOGFONT lf; m_pFont->GetLogFont(&lf); lf.lfHeight = 90;
	GVITEM	gvitem;

	gvitem.row   = row;
	gvitem.mask  = /*GVMK_TEXT|*/GVMK_FORMAT|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR;
	gvitem.mask |= GVMK_ATTR;
	gvitem.font  = lf;
	gvitem.fgcol = m_pApp->GetIndexColor(m_pView, 69);
	gvitem.bkcol = m_pApp->GetIndexColor(m_pView, 68);
	CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

	for (int ii = 0; ii < m_pPFGrid->GetColumnCount(); ii++)
	{
		gvitem.state = 0;
		gvitem.col    = ii;
		//gvitem.text   = "";
		gvitem.format= stggridHdr[ii].format;			

		gvitem.attr  = stggridHdr[ii].attrx;
		m_pPFGrid->SetItem(&gvitem);
	}
	m_pPFGrid->SetRowHeight(row, GRID_ROWH);
}

void CStgPage::setGrid()
{
	CString val;
	_ProjectCode	projCode;

	m_pPFGrid->Clear();
	for (int ii = 0; ii < m_projectCode.GetSize(); ii++)
	{
		projCode = m_projectCode.GetAt(ii);
		val.Format("%s;%d;%d;%f", projCode.Code, projCode.mmgb, projCode.Num, projCode.jprc);
		setGridItem(ii+1, val);
	}
}

void CStgPage::setGridItem(int row, CString dat)
{
	if (row >= m_pPFGrid->GetRowCount())
		m_pPFGrid->SetRowCount(row + 1);
	initGridRow(row);

	CString val, tmp;
	CString code;
	int num=0, mmgb=0;
	//double prc;
	val = Parse(dat);
	code.Format("%s", val.GetString());
	m_pPFGrid->SetItemText(row, gCode, val);
	
	CString cname;
	cname.Format("%s", (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (long)(LPSTR)(LPCTSTR)val));
	cname.TrimRight(); if (cname.IsEmpty()) return;
	m_pPFGrid->SetItemText(row, gName, cname);


	val = Parse(dat);	mmgb = atoi(val);
	if (atoi(val) == 1)	m_pPFGrid->SetItemText(row, gGB, "+�ż�");
	else			m_pPFGrid->SetItemText(row, gGB, "-�ŵ�");
	val = Parse(dat);	num = atoi(val);
	m_pPFGrid->SetItemText(row, gQty, val);
	//val = Parse(dat);	prc = atof(val);
	//m_pPFGrid->SetItemText(row, gPrc, val);
}

CString CStgPage::Parse(CString &dat, CString delimiter)
{
	int pos;
	CString val;
	pos = dat.Find(delimiter);
	if (pos >= 0)
	{
		val = dat.Left(pos++);
		dat = dat.Mid(pos);
	}
	else
	{
		val = dat;
		dat.Empty();
	}
	return val;
}


HBRUSH CStgPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	pDC->SetBkMode(TRANSPARENT);
		
	if (pWnd->GetDlgCtrlID() == IDC_EDT_STGINFO)
	{	
		pDC->SetBkColor(m_memoColor);
		hbr = m_edtBr;
	} 
	else if (nCtlColor == CTLCOLOR_BTN || 
		     nCtlColor == CTLCOLOR_STATIC ||
			 pWnd->GetDlgCtrlID() == IDC_STR_GRPWND )
	{
		pDC->SetBkColor(m_bkColor);
		hbr = m_bkBr;
	}

	return CreateSolidBrush(m_bkColor);
}

long CStgPage::OnInitData(WPARAM wParam, LPARAM lParam)
{
	initData();
	return 0;
}

void CStgPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(rcClient, m_bkColor);
}

BOOL CStgPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_pFont = m_pApp->GetFont(m_pView, 9);
//	m_memoColor = RGB(219, 219, 219);
	m_memoColor = RGB(0, 0, 255);
	m_edtBr.CreateSolidBrush(m_memoColor);
	m_bkColor = m_pApp->GetIndexColor(m_pView, 66);
	m_bkBr.CreateSolidBrush(m_bkColor);

	CRect rc, rcClient;
	GetClientRect(&rcClient);
	rcClient.right -= 3;

	m_btnAddStg.SetImgBitmap(((CMapWnd*)m_pParent)->m_bmp4BTN, ((CMapWnd*)m_pParent)->m_bmp4BTN_DN, ((CMapWnd*)m_pParent)->m_bmp4BTN_EN);
	m_btnAddStg.SetFont(m_pFont);
	m_btnSetStg.SetImgBitmap(((CMapWnd*)m_pParent)->m_bmp4BTN, ((CMapWnd*)m_pParent)->m_bmp4BTN_DN, ((CMapWnd*)m_pParent)->m_bmp4BTN_EN);
	m_btnSetStg.SetFont(m_pFont);

	rc = CRect(1, 0, rcClient.right - BOARD_WIDTH - GRID_WIDTH, rcClient.bottom);
	m_pStgInfo = std::make_unique<CStgInfo>(m_pView, m_pFont);
	m_pStgInfo->Create(AfxRegisterWndClass(NULL), "StgInfo", WS_CHILD|WS_VISIBLE|WS_VSCROLL, rc, this, 1);
	m_pStgInfo->LoadImage();

	//rc = CRect(rcClient.right - BOARD_WIDTH - GRID_WIDTH + 1, 25, rcClient.right - GRID_WIDTH, rcClient.bottom);
	//m_pStgBoard = new CStgBoard(m_pView);
	//m_pStgBoard->Create(AfxRegisterWndClass(NULL), "StgBoard", WS_CHILD|WS_VISIBLE|WS_VSCROLL, rc, this, 0);

	rc = CRect(rcClient.right - GRID_WIDTH + 1, 0, rcClient.right, GRID_HEIGHT);
	m_pPFGrid = std::make_unique <CfxGrid>();
	m_pPFGrid->Create(rc, this, ID_JANGOGRID, GVSC_VERT, GVDD_FULL, WS_CHILD|WS_TABSTOP|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
	m_pPFGrid->SetFont(m_pFont);
	initGrid();

	rc = CRect(rcClient.right - GRID_WIDTH + 1, GRID_HEIGHT + 3, rcClient.right, rcClient.bottom);
	m_pGrpWnd = std::make_unique<CMGrpWnd>(m_pView, this);
	m_pGrpWnd->Create(AfxRegisterWndClass(NULL), "MGrpWnd", WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, rc, this, IDC_STR_GRPWND);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
