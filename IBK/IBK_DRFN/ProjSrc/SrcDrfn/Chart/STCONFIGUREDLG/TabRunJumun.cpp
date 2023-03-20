// TabRunJumun.cpp : implementation file
//

#include "stdafx.h"
#include "TabRunJumun.h"
#include "../Common_ST/STControllerDef.h"
#include "../Include_ST/ISTControllerD.h"
#include "../Common_ST/STConfigdef.h"
#include "../../inc/ICompanyMng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define CTRL_DROP 1000 // KHD : ���� ��ǲ.... 
//#define ACC_QRYCOMPLETED	WM_USER + 9003

// Msg Define
const UINT RMSG_GET_CONTROLHANDLE = ::RegisterWindowMessage(_T("RMSG_GET_CONTROLHANDLE"));
const UINT RMSG_GET_DISPATCHER_ACCOUNTCTRL = ::RegisterWindowMessage(_T("RMSG_GET_DISPATCHER_ACCOUNTCTRL"));
const UINT RMSG_GET_INTERFACE_ACCOUNTCTRL = ::RegisterWindowMessage(_T("RMSG_GET_INTERFACE_ACCOUNTCTRL"));
const UINT RMSG_GET_ACCOUNTINFO = ::RegisterWindowMessage(_T("RMSG_GET_ACCOUNTINFO"));
const UINT RMSG_GET_AVAILABLEFOR_STORDER = ::RegisterWindowMessage(_T("RMSG_GET_AVAILABLEFOR_STORDER"));

/////////////////////////////////////////////////////////////////////////////
// CTabRunJumun dialog
CTabRunJumun::CTabRunJumun(CWnd* pParent /*=NULL*/)
	: CRscDlg(CTabRunJumun::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabRunJumun)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMainMng = NULL;
//	m_StAccInfo = NULL;
	m_pWndOrgAccountParent = NULL;
}


void CTabRunJumun::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabRunJumun)
	DDX_Control(pDX, IDC_CB_ACCT, m_cbAcct);
	DDX_Control(pDX, IDC_CB_BUY, m_cbBuy);
	DDX_Control(pDX, IDC_CB_SELL, m_cbSell);
	DDX_Control(pDX, IDC_CB_JUMUNMETHOD, m_cbJMethod);
	DDX_Control(pDX, IDC_CB_JUMUNSTARTSIGNAL, m_cbJumunSS);
	DDX_Control(pDX, IDC_BTN_FILEBUY, m_btnFileBuy);
	DDX_Control(pDX, IDC_BTN_PLAYBUY, m_btnPlayBuy);
	DDX_Control(pDX, IDC_BTN_FILESELL, m_btnFileSell);
	DDX_Control(pDX, IDC_BTN_PLAYSELL, m_btnPlaySell);
	DDX_Control(pDX, IDC_CB_JUMUNTIME, m_cbJTime);
	DDX_Control(pDX, IDC_STT_ACCTNAME, m_sttAccountName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabRunJumun, CRscDlg)
	//{{AFX_MSG_MAP(CTabRunJumun)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_FILEBUY, OnBtnFileBuy)
	ON_BN_CLICKED(IDC_BTN_FILESELL, OnBtnFileSell)
	ON_BN_CLICKED(IDC_BTN_PLAYBUY, OnBtnPlayBuy)
	ON_BN_CLICKED(IDC_BTN_PLAYSELL, OnBtnPlaySell)
	ON_CBN_SELCHANGE(IDC_CB_ACCT, OnSelchangeCmbAcct)
	ON_CBN_SELCHANGE(IDC_CB_JUMUNMETHOD, OnSelchangeCmbMethod)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHK_MARKETPRICECLEAR, OnBnClickedChkMarketpriceclear)
	ON_BN_CLICKED(IDC_CHK_MARKETPRICEENTRY, OnBnClickedChkMarketpriceentry)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabRunJumun message handlers

BOOL CTabRunJumun::OnInitDialog() 
{
	CRscDlg::OnInitDialog();

	// TODO: Add extra initialization here
	int nCodeType = 0;	//�ֽ�(0), ����(1)
	CConfigHelper::cmBuy(&m_cbBuy, nCodeType);
	CConfigHelper::cmSell(&m_cbSell, nCodeType);
	CConfigHelper::cmJumunMethod(&m_cbJMethod);
	CConfigHelper::cmJumunTime(&m_cbJTime);
	CConfigHelper::cmJumunStartSignal(&m_cbJumunSS);

	m_pMainMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
	// ���� �з� �ڵ� �� ( 10 : ������ Client, 11 : ������ HTS, 21 : ���̹� HTS )
//	LPCSTR szHTSKey = m_pMainMng->GetMainData("ISHTS",NULL);

	LoadAcctInfo();
	LoadInfo();

	OnSelchangeCmbMethod();
	
	if (m_cbAcct.GetCount() > 0)
	{
		m_cbAcct.SetCurSel(0);
		OnSelchangeCmbAcct();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// ���°��� ������ �ε��Ѵ�.
void CTabRunJumun::LoadAcctInfo()
{
	CStringArray arrAcc;
	((IMainInfoManager0002*)m_pMainMng)->GetUserAccount(arrAcc);

	CStringArray arrAccType;
	ReadAccType(arrAccType);

	int nSize = arrAcc.GetSize();
	CString strAcct;
	for (int i=0; i<nSize; i++)
	{
		strAcct = arrAcc.GetAt(i);
		if(IsTradableAcct(strAcct, arrAccType))
			m_cbAcct.AddString(strAcct);
	}
}

void CTabRunJumun::ReadAccType(CStringArray& arrAccType)
{
	int	nID = 12;
	CString strDataDir = (LPCTSTR)(LPCSTR)AfxGetPctrData(nID);
	CString strFilePath;
	strFilePath.Format("%s%s", strDataDir, _T("\\PcTrMng.cfg"));

	//ǥ���� ����Ÿ�� �ҷ�����
	char szTmp[MAX_PATH];	
	GetPrivateProfileString(_T("Account"), _T("ShowList"), "",  szTmp, MAX_PATH, strFilePath);
	CString strAcctTypeList = szTmp;
	if(strAcctTypeList.IsEmpty())
		return;

	CString strAccType, strTemp;
	while(TRUE)
	{
		int nFind = strAcctTypeList.Find(',');
		if(nFind > -1)
		{
			strAccType = strAcctTypeList.Left(nFind);
			strTemp = strAcctTypeList.Mid(nFind + 1);
			strAcctTypeList = strTemp;
			arrAccType.Add(strAccType);
			continue;
		}
		else
		{
			if(strAcctTypeList.GetLength() == 2)
				arrAccType.Add(strAcctTypeList);
		}
		break;
	}
}

BOOL CTabRunJumun::IsTradableAcct(CString strAcct, CStringArray& arrAccType)
{
	if(arrAccType.GetSize() == 0) //���� ������ ������ ������ ���� ǥ���ϵ���.
		return TRUE;

	if(strAcct.IsEmpty() && strAcct.GetLength() > 5) //������ ���� ���͵� ���� �ʵ���.
		return FALSE;

	CString strAccType = strAcct.Mid(3, 2);
	int nIndex;
	for(nIndex = 0; nIndex < arrAccType.GetSize(); nIndex++)
	{
		if(strAccType.CompareNoCase(arrAccType.GetAt(nIndex)) == 0)
			return TRUE;	
	}

	return FALSE;
}

void CTabRunJumun::LoadInfo()
{
	CString szKey, szTmp;
	COLORREF clrSignal = RGB(0,0,0);
	int nSel=0;

	szTmp.GetBufferSetLength(MAX_PATH);

	// -------------------- ���� -----------------------
	CSTConfigEnv envHelper;
	envHelper.LoadInfo(m_szCFGFile,  m_szDefCFGFile, STConfigEnv_Jumun);

	{ 
		//�������
		CString AcctNumber;
		AcctNumber = envHelper.m_InfoJumun.m_szAcctNumber;

		if(AcctNumber.GetLength()==11)
		{
			AcctNumber.Insert(3,'-');
			AcctNumber.Insert(6,'-');
//			m_AcctInput.SetText(AcctNumber);
		}
		m_cbAcct.SetCurSel(0);
		
		//���¸�
		SetDlgItemText(IDC_ACCTNAME, envHelper.m_InfoJumun.m_szAcctName);
	
		// ���԰� ���尡 ����
		if(envHelper.m_InfoJumun.m_nMkprc_Ent)
		{
			((CButton*)GetDlgItem(IDC_CHK_MARKETPRICEENTRY))->SetCheck(TRUE);
			GetDlgItem(IDC_CB_BUY)->EnableWindow(FALSE);
		}

		// û�갡 ���尡 ����
		if(envHelper.m_InfoJumun.m_nMkprc_Clr)
		{
			((CButton*)GetDlgItem(IDC_CHK_MARKETPRICECLEAR))->SetCheck(TRUE);
			GetDlgItem(IDC_CB_SELL)->EnableWindow(FALSE);
		}
	}


	//�ֹ�����
	szTmp.Format("%d", envHelper.m_InfoJumun.m_nJAmount);
	SetDlgItemText(IDC_ED_JUMUNAMT, szTmp);

	//���԰���
	m_cbBuy.SetCurSel(envHelper.m_InfoJumun.m_nBuyPrice);

	//û�갡��
	m_cbSell.SetCurSel(envHelper.m_InfoJumun.m_nSellPrice);

	//�ֹ����
	m_cbJMethod.SetCurSel(envHelper.m_InfoJumun.m_nJMethod);

	//�ֹ�����
	m_cbJTime.SetCurSel(envHelper.m_InfoJumun.m_nJTime);

	//�ֹ����۽�ȣ
	m_cbJumunSS.SetCurSel(envHelper.m_InfoJumun.m_nJStartSignal);

	//��ȣ�߻���_���Խ�ȣ ��뿩��
	CheckDlgButton(IDC_CHK_SOUNDBUY , envHelper.m_InfoJumun.m_bSound_Buy_Use);

	//��ȣ�߻���_���Խ�ȣ �Ҹ�
	SetDlgItemText(IDC_ED_FILEBUY, envHelper.m_InfoJumun.m_szSound_Buy_File);

	//��ȣ�߻���_û���ȣ ��뿩��
	CheckDlgButton(IDC_CHK_SOUNDSELL , envHelper.m_InfoJumun.m_bSound_Sell_Use);

	//��ȣ�߻���_û���ȣ �Ҹ�
	SetDlgItemText(IDC_ED_FILESELL, envHelper.m_InfoJumun.m_szSound_Sell_File);

	szTmp.ReleaseBuffer();
}

BOOL CTabRunJumun::SaveInfo()
{
	CString szKey, szTmp;
	int nSel;

	{
		// ��й�ȣ üũ
		CString strAcctInfo;
		int nSel = m_cbJMethod.GetCurSel();
		if (nSel > 0)
		{
			if (!CheckInputPwd())
				return FALSE;

			nSel = m_cbAcct.GetCurSel();
			m_cbAcct.GetLBText(nSel, strAcctInfo);
			if (strAcctInfo.GetLength() == 0)
				return FALSE;
		}

		szKey.Format("%s", "Acct");
		::WritePrivateProfileString(STCONFIG_ORDER, szKey, strAcctInfo, m_szCFGFile);

		//��������
		if(m_cbAcct.GetCount()>0)
		{
			//�������
			szKey.Format("%s", "Acct");
			nSel = m_cbAcct.GetCurSel();		if(nSel==CB_ERR) nSel=0;

			CString strAcctInfo;
			m_cbAcct.GetLBText(nSel, strAcctInfo);

			if(strAcctInfo.GetLength() == 0)
				return FALSE;

			//���¸�
			strAcctInfo.TrimRight();
			strAcctInfo.Remove('-');
			::WritePrivateProfileString(STCONFIG_ORDER, szKey, strAcctInfo, m_szCFGFile);
		}

		//���¸�
		szKey.Format("%s", "AcctName");
		GetDlgItemText(IDC_ACCTNAME, szTmp);
		::WritePrivateProfileString(STCONFIG_ORDER, szKey, szTmp, m_szCFGFile);
	
		// ���԰� ���尡 ����
		BOOL bCheck = ((CButton*)GetDlgItem(IDC_CHK_MARKETPRICEENTRY))->GetCheck();
		szKey.Format("%s", "MKPRC_ENT");
		szTmp.Format("%d", bCheck);
		::WritePrivateProfileString(STCONFIG_ORDER, szKey, szTmp, m_szCFGFile);

		// û�갡 ���尡 ����
		bCheck = ((CButton*)GetDlgItem(IDC_CHK_MARKETPRICECLEAR))->GetCheck();
		szKey.Format("%s", "MKPRC_CLR");
		szTmp.Format("%d", bCheck);
		::WritePrivateProfileString(STCONFIG_ORDER, szKey, szTmp, m_szCFGFile);
	}

	//�ֹ�����
	szKey.Format("%s", "JAmount");
	GetDlgItemText(IDC_ED_JUMUNAMT, szTmp);
	::WritePrivateProfileString(STCONFIG_ORDER, szKey, szTmp, m_szCFGFile);

	//���԰���
	szKey.Format("%s", "BuyPrice");
	szTmp = "";
	nSel = m_cbBuy.GetCurSel();		if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_ORDER, szKey, szTmp, m_szCFGFile);

	//û�갡��
	szKey.Format("%s", "SellPrice");
	szTmp = "";
	nSel = m_cbSell.GetCurSel();	if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_ORDER, szKey, szTmp, m_szCFGFile);

	//�ֹ����
	szKey.Format("%s", "JMethod");
	szTmp = "";
	nSel = m_cbJMethod.GetCurSel();		if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_ORDER, szKey, szTmp, m_szCFGFile);

	//�ֹ�����
	szKey.Format("%s", "JTime");
	szTmp = "";
	nSel = m_cbJTime.GetCurSel();		if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_ORDER, szKey, szTmp, m_szCFGFile);

	//�ֹ����۽�ȣ
	szKey.Format("%s", "JStartSignal");
	szTmp = "";
	nSel = m_cbJumunSS.GetCurSel();		if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_ORDER, szKey, szTmp, m_szCFGFile);

	//��ȣ�߻���_���Խ�ȣ ��뿩��
	szKey.Format("%s", "Sound_Buy_Use");
	szTmp.Format("%d", IsDlgButtonChecked(IDC_CHK_SOUNDBUY));
	::WritePrivateProfileString(STCONFIG_ORDER, szKey, szTmp, m_szCFGFile);

	//��ȣ�߻���_���Խ�ȣ �Ҹ�
	szKey.Format("%s", "Sound_Buy_File");
	GetDlgItemText(IDC_ED_FILEBUY, szTmp);
	::WritePrivateProfileString(STCONFIG_ORDER, szKey, szTmp, m_szCFGFile);

	//��ȣ�߻���_û���ȣ ��뿩��
	szKey.Format("%s", "Sound_Sell_Use");
	szTmp.Format("%d", IsDlgButtonChecked(IDC_CHK_SOUNDSELL));
	::WritePrivateProfileString(STCONFIG_ORDER, szKey, szTmp, m_szCFGFile);

	//��ȣ�߻���_û���ȣ �Ҹ�
	szKey.Format("%s", "Sound_Sell_File");
	GetDlgItemText(IDC_ED_FILESELL, szTmp);
	::WritePrivateProfileString(STCONFIG_ORDER, szKey, szTmp, m_szCFGFile);

	return TRUE;
}

void CTabRunJumun::SetDefault()
{
	m_cbAcct.SetCurSel(0);
	SetDlgItemText(IDC_ACCTNAME, "");
	SetDlgItemText(IDC_ED_JUMUNAMT, "");
	m_cbBuy.SetCurSel(0);
	m_cbSell.SetCurSel(0);
	m_cbJMethod.SetCurSel(0);
	m_cbJumunSS.SetCurSel(0);
	CheckDlgButton(IDC_CHK_SOUNDBUY , 0);
	SetDlgItemText(IDC_ED_FILEBUY, "");
	CheckDlgButton(IDC_CHK_SOUNDSELL , 0);
	SetDlgItemText(IDC_ED_FILESELL, "");

	((CButton*)GetDlgItem(IDC_CHK_MARKETPRICEENTRY))->SetCheck(FALSE);
	GetDlgItem(IDC_CB_BUY)->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_CHK_MARKETPRICECLEAR))->SetCheck(FALSE);
	GetDlgItem(IDC_CB_SELL)->EnableWindow(TRUE);

	SaveInfo();
}

HBRUSH CTabRunJumun::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CRscDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		pDC->SetBkColor(m_crBk);
		return *m_pBrBkColor;		
		break;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CTabRunJumun::OnBtnFileBuy()
{
	fnBtnFileSearch(IDC_ED_FILEBUY);
}
void CTabRunJumun::OnBtnFileSell()
{
	fnBtnFileSearch(IDC_ED_FILESELL);
}

void CTabRunJumun::fnBtnFileSearch(UINT nID) 
{
	char szFilter[] = "Wav Files (*.wav)|*.wav|All Files (*.*)|*.*||";
	CString strDefaultName = "";

	char szTargetDllPath[MAX_PATH];
	char szCurrentDirPath[MAX_PATH];
	GetWindowsDirectory(szTargetDllPath, MAX_PATH-1);
	lstrcat(szTargetDllPath,"\\");
	lstrcpyn(szTargetDllPath + lstrlen(szTargetDllPath), "Media\\", MAX_PATH - lstrlen(szTargetDllPath));

	GetCurrentDirectory(MAX_PATH,szCurrentDirPath);
	SetCurrentDirectory(szTargetDllPath);
	CFileDialog dlg(TRUE, "wav", strDefaultName, OFN_HIDEREADONLY, szFilter);
	if (dlg.DoModal() == IDOK)
	{
		CString szFile = dlg.GetPathName();
		SetDlgItemText(nID, szFile);
	}
	SetCurrentDirectory(szCurrentDirPath);	
}

#include <mmsystem.h>
#include ".\tabrunjumun.h"
void CTabRunJumun::OnBtnPlayBuy()
{
	CString szTmp;
	GetDlgItemText(IDC_ED_FILEBUY, szTmp);
	if(szTmp.GetLength()) sndPlaySound (szTmp, SND_ASYNC);
}

void CTabRunJumun::OnBtnPlaySell()
{
	CString szTmp;
	GetDlgItemText(IDC_ED_FILESELL, szTmp);
	if(szTmp.GetLength()) sndPlaySound (szTmp, SND_ASYNC);
}

void CTabRunJumun::OnSelchangeCmbAcct()
{
	CString strAcct;
	int nSel = m_cbAcct.GetCurSel();
	m_cbAcct.GetLBText(nSel, strAcct);

	CString strName = ((IMainInfoManager0002*)m_pMainMng)->GetAccountName((LPTSTR)(LPCTSTR)strAcct);
	int nFind;
	nFind = strName.Find("|");
	strName = strName.Left(nFind);

	m_sttAccountName.SetWindowText(strName);
	m_sttAccountName.Invalidate();
}

void CTabRunJumun::OnSelchangeCmbMethod()
{
	CString strAcct;
	int nSel = m_cbJMethod.GetCurSel();
	if (nSel > 0)
	{
		m_sttAccountName.EnableWindow(TRUE);
		m_cbAcct.EnableWindow(TRUE);
		GetDlgItem(IDC_ED_ACCTPWD)->EnableWindow(TRUE);
		GetDlgItem(IDC_ED_ACCTPWDCONFIRM)->EnableWindow(TRUE);
	}
	else
	{
		m_sttAccountName.EnableWindow(FALSE);
		m_cbAcct.EnableWindow(FALSE);
		GetDlgItem(IDC_ED_ACCTPWD)->EnableWindow(FALSE);
		GetDlgItem(IDC_ED_ACCTPWDCONFIRM)->EnableWindow(FALSE);
	}
}

void CTabRunJumun::OnDestroy()
{
	CRscDlg::OnDestroy();
}

void CTabRunJumun::OnBnClickedChkMarketpriceclear()
{
	GetDlgItem(IDC_CB_SELL)->EnableWindow(!((CButton*)GetDlgItem(IDC_CHK_MARKETPRICECLEAR))->GetCheck());
}

void CTabRunJumun::OnBnClickedChkMarketpriceentry()
{
	GetDlgItem(IDC_CB_BUY)->EnableWindow(!((CButton*)GetDlgItem(IDC_CHK_MARKETPRICEENTRY))->GetCheck());
}

BOOL CTabRunJumun::OnCommand(WPARAM wParam, LPARAM lParam)
{
// 	UINT uIid = LOWORD(wParam);
// 	if(uIid == IDC_STATIC_ACCT)
// 	{
// 		switch (HIWORD(wParam))
// 		{
// 		case ACC_QRYCOMPLETED : 
// 			break;
// 		case EN_CHANGE :
// 			break;
// 		}
// 	}
	return CRscDlg::OnCommand(wParam, lParam);
}

BOOL CTabRunJumun::CheckInputPwd()
{
	CString strAcc, strPW, strPWConf, strOrgPW;
	GetDlgItemText(IDC_ED_ACCTPWD, strPW);
	GetDlgItemText(IDC_ED_ACCTPWDCONFIRM, strPWConf);
	strPW.TrimRight();
	strPWConf.TrimRight();

	if (strPW.GetLength() == 0)
	{
		AfxMessageBox("�ֹ������ ���ڵ��ֹ��� ��쿡�� ��й�ȣ�� �Է¹ٶ��ϴ�.");
		GetDlgItem(IDC_ED_ACCTPWD)->SetFocus();
		return FALSE;
	}

	if (strPW.CompareNoCase(strPWConf))
	{
		AfxMessageBox("�ΰ��� ��ȣ�� ��ġ���� �ʽ��ϴ�. �ٽ� �Է¹ٶ��ϴ�.");
		GetDlgItem(IDC_ED_ACCTPWD)->SetFocus();
		return FALSE;
	}

	int nSel = m_cbAcct.GetCurSel();
	m_cbAcct.GetLBText(nSel, strAcc);
//	strOrgPW = ((IMainInfoManager0002*)m_pMainMng)->GetAccountPass((LPTSTR)(LPCTSTR)strAcc);
	strOrgPW = ((IMainInfoManager0002*)m_pMainMng)->GetLoginPass();

	if (strPW.CompareNoCase(strOrgPW))
	{
		AfxMessageBox("��й�ȣ�� �ٸ��ϴ�. �ٽ� �Է¹ٶ��ϴ�.");
		GetDlgItem(IDC_ED_ACCTPWD)->SetFocus();
		return FALSE;
	}

	return TRUE;
}

