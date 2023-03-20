// AlarmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AlarmDlg.h"
#include "tmenu.h"
#include "../../h/axisvar.h"

// CAlarmDlg dialog

//IMPLEMENT_DYNAMIC(CAlarmDlg, CDialog)


CAlarmDlg::CAlarmDlg(class CTMenu* menu, CString home, CString user, CWnd* pParent /*=NULL*/)
: CSetupDialog(CAlarmDlg::IDD, pParent)
{
	m_caption = _T("�˸�");
	m_home = home;
	m_user = user;
}

CAlarmDlg::CAlarmDlg(CWnd* pParent /*=NULL*/)
	: CSetupDialog(CAlarmDlg::IDD, pParent)
{

}

CAlarmDlg::~CAlarmDlg()
{
}

void CAlarmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAlarmDlg, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_POP, &CAlarmDlg::OnDeltaposSpinPop)
	ON_BN_CLICKED(IDC_CHK_GONG, &CAlarmDlg::OnBnClickedChkGong)
	ON_BN_CLICKED(IDC_CHK_SU, &CAlarmDlg::OnBnClickedChkSu)
	ON_BN_CLICKED(IDC_CHK_BAL, &CAlarmDlg::OnBnClickedChkBal)
END_MESSAGE_MAP()

// CAlarmDlg message handlers
void CAlarmDlg::OnDeltaposSpinPop(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString string, tmpS;
	((CWnd*)GetDlgItem(IDC_EDIT_SECOND))->GetWindowText(string);
	int iTime;
	iTime = atoi(string);
	if (pNMUpDown->iDelta > 0)
		iTime -= 1;
	else	
		iTime += 1;

	if (iTime < 0) 
		iTime = 0;
	if (iTime > 99) 
		iTime = 99;

	string.Format("%d", iTime);
	((CWnd*)GetDlgItem(IDC_EDIT_SECOND))->SetWindowText(string);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}

BOOL CAlarmDlg::OnInitDialog()
{
	CSetupDialog::OnInitDialog();
	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CAlarmDlg::CheckAlarm()
{
	((CButton*)GetDlgItem(IDC_CHK_ALL))->SetCheck(false);

	if(((CButton *)GetDlgItem(IDC_CHK_GONG))->GetCheck()) //��������
		((CButton*)GetDlgItem(IDC_CHK_ALL))->SetCheck(true);

	if(((CButton *)GetDlgItem(IDC_CHK_SU))->GetCheck()) //���俹��
		((CButton*)GetDlgItem(IDC_CHK_ALL))->SetCheck(true);

	if(((CButton *)GetDlgItem(IDC_CHK_BAL))->GetCheck()) //�������
		((CButton*)GetDlgItem(IDC_CHK_ALL))->SetCheck(true);
}

void CAlarmDlg::Init()
{
	char		buffer[128];
	CString		file, value, tmpS;
	file.Format(_T("%s\\%s\\%s\\%s"),m_home, USRDIR, m_user, SETUPFILE);

	//���� ������ ���� �˶�ǥ��
	GetPrivateProfileString(_T("ALARM"), _T("ALL") , _T("1"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);
	if(tmpS == _T("1"))
		((CButton*)GetDlgItem(IDC_CHK_ALL))->SetCheck(true);
	else
		((CButton*)GetDlgItem(IDC_CHK_ALL))->SetCheck(false);

	//��������
	GetPrivateProfileString(_T("ALARM"), _T("GONGJI") , _T("1"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);
	if(tmpS == _T("1"))
		((CButton*)GetDlgItem(IDC_CHK_GONG))->SetCheck(true);
	else
		((CButton*)GetDlgItem(IDC_CHK_GONG))->SetCheck(false);

	//���俹��
	GetPrivateProfileString(_T("ALARM"), _T("SUYO") , _T("1"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);
	if(tmpS == _T("1"))
		((CButton*)GetDlgItem(IDC_CHK_SU))->SetCheck(true);
	else
		((CButton*)GetDlgItem(IDC_CHK_SU))->SetCheck(false);

	//�������
	GetPrivateProfileString(_T("ALARM"), _T("BAL") , _T("1"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);
	if(tmpS == _T("1"))
		((CButton*)GetDlgItem(IDC_CHK_BAL))->SetCheck(true);
	else
		((CButton*)GetDlgItem(IDC_CHK_BAL))->SetCheck(false);

	//�˾�â���ð�
	GetPrivateProfileString(_T("ALARM"), _T("SECOND") , _T("3"), buffer, sizeof(buffer), file);
	tmpS.Format(_T("%s"), buffer);
	((CWnd*)GetDlgItem(IDC_EDIT_SECOND))->SetWindowText(tmpS);

	CheckAlarm();
}

void CAlarmDlg::ApplySetup()
{
	save_info();
}

void CAlarmDlg::save_info()
{
	CString		file, value, tmp;
	file.Format(_T("%s\\%s\\%s\\%s"),m_home, USRDIR,m_user, SETUPFILE);

	//���� ������ ���� �˶�ǥ��
	if(((CButton *)GetDlgItem(IDC_CHK_ALL))->GetCheck())
		WritePrivateProfileString(_T("ALARM"), _T("ALL"), _T("1"), file);
	else
		WritePrivateProfileString(_T("ALARM"), _T("ALL"), _T("0"), file);

	//��������
	if(((CButton *)GetDlgItem(IDC_CHK_GONG))->GetCheck())
		WritePrivateProfileString(_T("ALARM"), _T("GONGJI"), _T("1"), file);
	else
		WritePrivateProfileString(_T("ALARM"), _T("GONGJI"), _T("0"), file);

	//���俹��
	if(((CButton *)GetDlgItem(IDC_CHK_SU))->GetCheck())
		WritePrivateProfileString(_T("ALARM"), _T("SUYO"), _T("1"), file);
	else
		WritePrivateProfileString(_T("ALARM"), _T("SUYO"), _T("0"), file);

	//�������
	if(((CButton *)GetDlgItem(IDC_CHK_BAL))->GetCheck())
		WritePrivateProfileString(_T("ALARM"), _T("BAL"), _T("1"), file);
	else
		WritePrivateProfileString(_T("ALARM"), _T("BAL"), _T("0"), file);

	//�˾�â���ð�
	
	((CWnd*)GetDlgItem(IDC_EDIT_SECOND))->GetWindowText(tmp);
	WritePrivateProfileString(_T("ALARM"), _T("SECOND"), tmp, file);
}

BOOL CAlarmDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_ESCAPE:
				{
					::TranslateMessage(pMsg);
					::DispatchMessage(pMsg);
				}
				return TRUE;
		}
	}
	return CSetupDialog::PreTranslateMessage(pMsg);
}


void CAlarmDlg::OnBnClickedChkGong()
{
	CheckAlarm();
}


void CAlarmDlg::OnBnClickedChkSu()
{
	CheckAlarm();
}


void CAlarmDlg::OnBnClickedChkBal()
{
	CheckAlarm();
}
