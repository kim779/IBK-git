// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB300600.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog


CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/, CString profile, CAxisExt *pAxisExt)
	: CDialog(CConfigDlg::IDD, pParent)
{
	m_pAxisExt = pAxisExt;
	m_profile = profile;
	//{{AFX_DATA_INIT(CConfigDlg)
	m_rsvd_width = 0;
	m_jumn_width = 0;
	m_size_width = 0;
	m_remain_width = 0;
	m_hoga_width = 0;
	m_rate_width = 0;
	m_row_height = 0;
	//}}AFX_DATA_INIT
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Text(pDX, IDC_ED_RSVD, m_rsvd_width);
	DDV_MinMaxUInt(pDX, m_rsvd_width, 40, 80);
	DDX_Text(pDX, IDC_ED_JUMN, m_jumn_width);
	DDV_MinMaxUInt(pDX, m_jumn_width, 45, 80);
	DDX_Text(pDX, IDC_ED_SIZE, m_size_width);
	DDV_MinMaxUInt(pDX, m_size_width, 30, 50);
	DDX_Text(pDX, IDC_ED_REMAIN, m_remain_width);
	DDV_MinMaxUInt(pDX, m_remain_width, 35, 55);
	DDX_Text(pDX, IDC_ED_HOGA, m_hoga_width);
	DDV_MinMaxUInt(pDX, m_hoga_width, 40, 70);
	DDX_Text(pDX, IDC_ED_RATE, m_rate_width);
	DDV_MinMaxUInt(pDX, m_rate_width, 30, 50);
	DDX_Text(pDX, IDC_ED_ROWHEIGHT, m_row_height);
	DDV_MinMaxUInt(pDX, m_row_height, 12, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_BN_CLICKED(IDC_ODR_CONFIRM, OnOdrConfirm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

void CConfigDlg::OnOK() 
{
	if (!UpdateData()) return;

	CButton *btn;

	btn = (CButton*)GetDlgItem(IDC_ODR_CONFIRM);
	if (btn) WritePrivateProfileString("Config", "ShowConfirm", btn->GetCheck() ? "TRUE" : "FALSE", m_profile);

	btn = (CButton*)GetDlgItem(IDC_SHOW_RSVD);
	if (btn) WritePrivateProfileString("Config", "ShowRsvd", btn->GetCheck() ? "TRUE" : "FALSE", m_profile);

	btn = (CButton*)GetDlgItem(IDC_SHOW_RATE);
	if (btn) WritePrivateProfileString("Config", "ShowRate", btn->GetCheck() ? "TRUE" : "FALSE", m_profile);

	btn = (CButton*)GetDlgItem(IDC_SHOW_BEFORE);
	if (btn) WritePrivateProfileString("Config", "ShowBefore", btn->GetCheck() ? "TRUE" : "FALSE", m_profile);

	btn = (CButton*)GetDlgItem(IDC_SHOW_CHG);
	if (btn) WritePrivateProfileString("Config", "ShowCheg", btn->GetCheck() ? "TRUE" : "FALSE", m_profile);

	btn = (CButton*)GetDlgItem(IDC_SHOW_CURR);
	if (btn) WritePrivateProfileString("Config", "ShowCurr", btn->GetCheck() ? "TRUE" : "FALSE", m_profile);

	btn = (CButton*)GetDlgItem(IDC_SPACEORDER);
	if (btn) WritePrivateProfileString("Config", "SpaceKeyOrder", btn->GetCheck() ? "TRUE" : "FALSE", m_profile);

	btn = (CButton*)GetDlgItem(IDC_EXTENTORDER);
	if (btn) WritePrivateProfileString("Config", "ExtOrderColumn", btn->GetCheck() ? "TRUE" : "FALSE", m_profile);

	btn = (CButton*)GetDlgItem(IDC_RCLICKCANCEL);
	if (btn) WritePrivateProfileString("Config", "RClickCancel", btn->GetCheck() ? "TRUE" : "FALSE", m_profile);

	btn = (CButton*)GetDlgItem(IDC_FOCUSCHEG);
	if (btn) WritePrivateProfileString("Config", "FocusCheg", btn->GetCheck() ? "TRUE" : "FALSE", m_profile);

	//2017.01.16 KSJ ���� �߰�
	btn = (CButton*)GetDlgItem(IDC_SIJANGDBCLICK);
	if (btn) WritePrivateProfileString("Config", "SiJangDBClick", btn->GetCheck() ? "TRUE" : "FALSE", m_profile);

	btn = (CButton*)GetDlgItem(IDC_JUMUNRESET);
	if (btn) WritePrivateProfileString("Config", "JumunReset", btn->GetCheck() ? "TRUE" : "FALSE", m_profile);

	CDialog::OnOK();
}

void CConfigDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	char buff[128];
	CButton *btn;
	
	btn = (CButton*)GetDlgItem(IDC_ODR_CONFIRM);
	GetPrivateProfileString("Config", "ShowConfirm", "TRUE", buff, sizeof(buff), m_profile);
	btn->SetCheck( (strcmp(buff, "FALSE")==0) ? FALSE : TRUE );
	
	btn = (CButton*)GetDlgItem(IDC_SHOW_RSVD);
	GetPrivateProfileString("Config", "ShowRsvd", "TRUE", buff, sizeof(buff), m_profile);
	btn->SetCheck( (strcmp(buff, "FALSE")==0) ? FALSE : TRUE );

	btn = (CButton*)GetDlgItem(IDC_SHOW_RATE);
	GetPrivateProfileString("Config", "ShowRate", "TRUE", buff, sizeof(buff), m_profile);
	btn->SetCheck( (strcmp(buff, "FALSE")==0) ? FALSE : TRUE );

	btn = (CButton*)GetDlgItem(IDC_SHOW_BEFORE);
	GetPrivateProfileString("Config", "ShowBefore", "TRUE", buff, sizeof(buff), m_profile);
	btn->SetCheck( (strcmp(buff, "FALSE")==0) ? FALSE : TRUE );

	btn = (CButton*)GetDlgItem(IDC_SHOW_CHG);
	GetPrivateProfileString("Config", "ShowCheg", "FALSE", buff, sizeof(buff), m_profile);
	btn->SetCheck( (strcmp(buff, "FALSE")==0) ? FALSE : TRUE );

	btn = (CButton*)GetDlgItem(IDC_SHOW_CURR);
	GetPrivateProfileString("Config", "ShowCurr", "TRUE", buff, sizeof(buff), m_profile);
	btn->SetCheck( (strcmp(buff, "FALSE")==0) ? FALSE : TRUE );

	btn = (CButton*)GetDlgItem(IDC_SPACEORDER);
	GetPrivateProfileString("Config", "SpaceKeyOrder", "FALSE", buff, sizeof(buff), m_profile);
	btn->SetCheck( (strcmp(buff, "FALSE")==0) ? FALSE : TRUE );

	btn = (CButton*)GetDlgItem(IDC_EXTENTORDER);
	GetPrivateProfileString("Config", "ExtOrderColumn", "FALSE", buff, sizeof(buff), m_profile);
	btn->SetCheck( (strcmp(buff, "FALSE")==0) ? FALSE : TRUE );

	btn = (CButton*)GetDlgItem(IDC_RCLICKCANCEL);
	GetPrivateProfileString("Config", "RClickCancel", "FALSE", buff, sizeof(buff), m_profile);
	btn->SetCheck( (strcmp(buff, "FALSE")==0) ? FALSE : TRUE );
	btn->ShowWindow(SW_HIDE);

	btn = (CButton*)GetDlgItem(IDC_FOCUSCHEG);
	GetPrivateProfileString("Config", "FocusCheg", "TRUE", buff, sizeof(buff), m_profile);
	btn->SetCheck( (strcmp(buff, "FALSE")==0) ? FALSE : TRUE );

	//2017.01.16 KSJ ���� �߰�
	btn = (CButton*)GetDlgItem(IDC_SIJANGDBCLICK);
	GetPrivateProfileString("Config", "SiJangDBClick", "FALSE", buff, sizeof(buff), m_profile);
	btn->SetCheck( (strcmp(buff, "FALSE")==0) ? FALSE : TRUE );

	btn = (CButton*)GetDlgItem(IDC_JUMUNRESET);
	GetPrivateProfileString("Config", "JumunReset", "FALSE", buff, sizeof(buff), m_profile);
	btn->SetCheck( (strcmp(buff, "FALSE")==0) ? FALSE : TRUE );

	CenterWindow();

	// ��Ŭ���ֹ���ҽ� Context�޴� ���� ����Ҽ� �ִ� ����� �Ϻ� ����ڿ��Ը� �߰��� 
	// ��û�� : �������� �̼��� ����
	// ���� : (����)�̼���(101201, sdybbc), (����)����ö, (����)õ����, (����)�̰��, (����)��̶�
	static char *allow_list[] = {"101201", "sdybbc",  "071006", "091120", "081394", "081001", "mira99", "ja6224", "warship", NULL };
	CString usid = m_pAxisExt->Variant(userCC, _T(""));
	for(int n=0; allow_list[n]!=NULL; ++n)
	{
		if (usid==allow_list[n])
		{
			btn->ShowWindow(SW_SHOW);
			break;
		}
	}

	return TRUE;
}

void CConfigDlg::OnOdrConfirm() 
{
	CButton *btn = (CButton*)GetDlgItem(IDC_ODR_CONFIRM);
	if (!btn->GetCheck())
	{
		const char *msg = 
			"Ŭ���ֹ��� Ư���� ���콺�� (����)Ŭ�������� �ֹ��� ����ǰ� ü��� �� ������,\r\n"
			"�ֹ��Է� Ȯ��â�� Ȯ���������� �ֹ��� ����Ǿ� �߻��ϴ� ������,\r\n"
			"�� ���񽺸� �����ϴ� ȸ�簡 å���� ���������� �˷��帳�ϴ�.";
		MessageBox(msg, "IBK��������", MB_OK|MB_ICONWARNING);
	}
}
