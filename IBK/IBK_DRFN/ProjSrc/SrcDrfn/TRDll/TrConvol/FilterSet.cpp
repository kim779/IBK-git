// FilterSet.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrmWnd.h"
#include "ScreenWnd.h"

#include "FilterSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define		ID_RADIO_CONVOL		18010
#define		ID_RADIO_DEALPRICE	17009
#define		ID_RADIO_BOTH		17015
#define		ID_ADDITEMUP		17032

#define		BETWEEN_CTRL			2
/////////////////////////////////////////////////////////////////////////////
// CFilterSet dialog
const		long		ARR_CONVOL[] = { 5000, 7000, 10000, 20000, 30000};
// const		long		ARR_DEALPRICE[] = { 10000000, 20000000, 30000000, 50000000, 70000000,
// 											100000000, 300000000, 500000000 };
const		long		ARR_DEALPRICE[] = { 50000000, 70000000, 100000000, 300000000, 500000000 };


#define TRIMALL(xString)	xString.TrimLeft(); xString.TrimRight();
//CFilterSet::CFilterSet(CWnd* pParent /*=NULL*/)
//	: CDialog(CFilterSet::IDD, pParent)
//{
//	//{{AFX_DATA_INIT(CFilterSet)
//	//}}AFX_DATA_INIT
//}

CFilterSet::CFilterSet(CWnd* pParent, CString strScreenNo, StConVolSet*	pstSrc, COLORREF clrBk)
	: CDialog(CFilterSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterSet)
	//}}AFX_DATA_INIT
	m_strScreenNo = strScreenNo;
	StConVolSet* pTarget = &m_stConVolSet;
	STCOPY_P_StConVolSet(pstSrc, pTarget);
	m_pMainData = pstSrc;
	m_bRight = FALSE;
	m_clrBk = clrBk;	
	m_brushCtrl.CreateSolidBrush(m_clrBk);
	m_pMainParent = pParent;

	m_pctrlSelectUJCombo = NULL;
	m_pctrlExceptJMCombo = NULL;
}

void CFilterSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterSet)
	DDX_Control(pDX, IDC_CHK_SOUND, m_chkSound);
	DDX_Control(pDX, IDC_EDIT_SOUND, m_editSound);
	DDX_Control(pDX, IDC_EDIT_SETRATEUP, m_editChgRateUp);
	DDX_Control(pDX, IDC_EDIT_SETRATEDOWN, m_editChgRateDown);
	DDX_Control(pDX, IDC_EDIT_SETPRICEDOWN, m_editSetPriceDown);
	DDX_Control(pDX, IDC_EDIT_SETPRICEUP, m_editSetPriceUp);
	DDX_Control(pDX, IDC_EDIT_ACUMDEALVOL, m_editAccumVol);
	DDX_Control(pDX, IDC_CHK_SETUPDOWNRATE, m_chkChgRateSet);
	DDX_Control(pDX, IDC_CHK_SETPRICE, m_chkSetPrice);
	DDX_Control(pDX, IDC_CHK_ACUMDEALVOL, m_chkAccumVol);
	DDX_Control(pDX, IDC_EDIT_DEALPRICE, m_editDealPrice);
	DDX_Control(pDX, IDC_EDIT_CONVOL, m_editConVol);
	DDX_Control(pDX, IDC_BTN_SOUND, m_btnSound);
	DDX_Control(pDX, IDC_BTN_SEARCHSOUND, m_btnSearchSound);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilterSet, CDialog)
	//{{AFX_MSG_MAP(CFilterSet)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHK_ACUMDEALVOL, OnChkAcumdealvol)
	ON_BN_CLICKED(IDC_CHK_SETPRICE, OnChkSetprice)
	ON_BN_CLICKED(IDC_CHK_SETUPDOWNRATE, OnChkSetupdownrate)
	ON_BN_CLICKED(IDC_CHK_SOUND, OnChkSound)
	ON_CONTROL_RANGE( BN_CLICKED, IDC_RADIO_CONVOL1, IDC_RADIO_CONVOL6, OnRadioConvolUser )
	ON_CONTROL_RANGE( BN_CLICKED, IDC_RADIO_DEALPRICE1, IDC_RADIO_DEALPRICE9, OnRadioDealpriceUser)
	ON_BN_CLICKED(IDC_BTN_SOUND, OnBtnSound)
	ON_BN_CLICKED(IDC_BTN_SEARCHSOUND, OnBtnSearchsound)
	ON_BN_CLICKED(IDC_UPJONG1, OnRadioUpjong)
	ON_BN_CLICKED(IDC_UPJONG2, OnRadioUpjong)
	ON_BN_CLICKED(IDC_UPJONG3, OnRadioUpjong)
	ON_BN_CLICKED(IDC_UPJONG4, OnRadioUpjong)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()

	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_REMOVE, OnBtnRemove)
	ON_BN_CLICKED(IDC_BTN_REMOVEALL, OnBtnRemoveAll)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterSet message handlers

BOOL CFilterSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	OnSetNumExceptText();
	return TRUE;  
}

void CFilterSet::SetInit()
{	
	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pMainParent;
	m_strFullPath.Format("%s%s\\%s\\%s", pMainFrmWnd->Variant(homeCC), USRDIR, pMainFrmWnd->Variant(nameCC), CONVOL_FILE);

	GetInfo();

	OnChkAcumdealvol();
	OnChkSetprice();
	OnChkSetupdownrate();
	OnChkSound();	

	GetDlgItem(IDC_STATIC_RIGHT)->SetWindowText("���� ���� ������ �����մϴ�.");
	
	ST_GRIDCOLOR stGridColor;
	stGridColor.clrGridHeader = pMainFrmWnd->GetAxColor(74);
	stGridColor.clrGridHilight = pMainFrmWnd->GetAxColor(78);

	m_gridExcept.SetInit("CODE", stGridColor);
	OnSizeCtrl();
	SetInItExceptGrid();
	OnRadioUpjong();
}

void CFilterSet::OnSizeCtrl()
{
	CRect reClient, reTemp, reBottom, reExceptCtrls;
	GetClientRect(reClient);

	reTemp = reClient;
	reTemp.top += 112;
	reTemp.bottom = reTemp.top + 1;
	reTemp.left += 78;
	reTemp.right = reTemp.left + 348;
	m_TradingValue_line.MoveWindow(reTemp);

	reTemp = reClient;
	reTemp.top = reTemp.bottom - 292;
	reTemp.bottom = reTemp.top + 20;
	reTemp.right = reTemp.right - 9;
	reTemp.left = reTemp.right - 84;
	m_pctrlSelectUJCombo->MoveWindow(reTemp);
 
	reTemp.top = reTemp.bottom + 25;
	reTemp.bottom = reTemp.top + 20;

	reTemp.left -= 138;
	reTemp.right = reTemp.left + 84;
	reTemp.bottom = reTemp.top + 20;
	m_pctrlExceptJMCombo->MoveWindow(reTemp);

	reExceptCtrls = reTemp;

	/*m_btnAdd.ShowWindow(SW_SHOW);*/
	reTemp.left = reTemp.right + BETWEEN_CTRL;
	reTemp.right = reTemp.left + GetBtnWidth(2);
	m_btnAdd.MoveWindow(reTemp);

	/*m_btnRemove.ShowWindow(SW_SHOW);*/
	reTemp.left = reTemp.right + BETWEEN_CTRL;
	reTemp.right = reTemp.left + GetBtnWidth(2);
	m_btnRemove.MoveWindow(reTemp);

	/*m_btnRemoveAll.ShowWindow(SW_SHOW);*/
	reTemp.left = reTemp.right + BETWEEN_CTRL;
	reTemp.right = reTemp.left + GetBtnWidth(4);
	m_btnRemoveAll.MoveWindow(reTemp);
	
	reTemp.left = reExceptCtrls.left;
	reTemp.top = reTemp.bottom + BETWEEN_CTRL;
	reTemp.right = reExceptCtrls.left + 222;
	reTemp.bottom = reTemp.top + 105;
	m_gridExcept.MoveWindow(reTemp);

	//reTemp.left += BETWEEN_CTRL * 3;
	reTemp.top = reTemp.bottom + 1;
	reTemp.bottom = reTemp.top + 20;
	//m_staticNumExcept.MoveWindow(reTemp);

	reBottom = reClient;

	reBottom.top = reBottom.bottom - 24;
	reBottom.bottom = reBottom.top + 20;
	reBottom.right -= 5;
	reBottom.left = reBottom.right - 60;
	m_btnCancel.MoveWindow(reBottom);

	reBottom.right = reBottom.left - BETWEEN_CTRL;
	reBottom.left = reBottom.right - 60;
	m_btnOk.MoveWindow(reBottom);

	Invalidate();
}

void CFilterSet::Set_DefaultInfo()
{
	m_stConVolSet.strVer = "1.0";
	m_stConVolSet.strScreenNo = m_strScreenNo;

	CButton* pButton;
	pButton = (CButton*)GetDlgItem(ID_RADIO_CONVOL);
	pButton->SetCheck(TRUE);

	pButton = (CButton*)GetDlgItem(ID_RADIO_DEALPRICE);
	pButton->SetCheck(TRUE);

	pButton = (CButton*)GetDlgItem(ID_RADIO_BOTH);
	pButton->SetCheck(TRUE);

	pButton = (CButton*)GetDlgItem(ID_ADDITEMUP);
	pButton->SetCheck(TRUE);

	m_stConVolSet.nChkAccumDealVol = 1;
	m_chkAccumVol.SetCheck(TRUE);

	m_stConVolSet.nChkSetPrice = 1;
	m_chkSetPrice.SetCheck(TRUE);

	m_stConVolSet.nChkChgRate = 1;
	m_chkChgRateSet.SetCheck(TRUE);
	
	m_stConVolSet.nChkSound = 1;
	m_chkSound.SetCheck(TRUE);

	m_stConVolSet.nItemPosition = 1;
	
}

BOOL CFilterSet::OnApply()
{
	BOOL bResult = TRUE;
	bResult = ErrorCheck();

	//CMainFrmWnd* pMainFrm = (CMainFrmWnd*)GetParent();
	//pMainFrm->OnFilterSetMessage(0);
	if(bResult)
		ShowWindow(SW_HIDE);

	return bResult;
}

BOOL CFilterSet::SetInfo(StConVolSet &stConVolSet)
{
	BOOL bResult = TRUE;

	CButton *pButton;
	CString strData;
	long lData;

	stConVolSet.strVer = "1.0";
	stConVolSet.strScreenNo = m_strScreenNo;
//#ü�ᷮ
	int nIndex = 0;
	for(nIndex = 0; _countof(ARR_CONVOL) + 1; nIndex++)
	{
		pButton = (CButton*)GetDlgItem(ID_RADIO_CONVOL + nIndex);
		if(pButton->GetCheck() == TRUE)
		{
			if(nIndex == 5)
			{
				m_editConVol.GetWindowText(strData);
				lData = atol(strData);
				break;
			}
			lData = ARR_CONVOL[nIndex];
			break;
		}
	}	

	stConVolSet.nChkConVol = 1;
	stConVolSet.lConVol = lData;
//#�ŷ����
//	for(nIndex = 0; nIndex < 9; nIndex++)
	for(nIndex = 0; nIndex < 6; nIndex++)
	{
		pButton = (CButton*)GetDlgItem(ID_RADIO_DEALPRICE + nIndex);
		if(pButton->GetCheck() == TRUE)
		{
			if(nIndex == 5)
			{
				m_editDealPrice.GetWindowText(strData);
				lData = atol(strData) * 10000;
				break;
			}
			lData = ARR_DEALPRICE[nIndex];
			break;
		}
	}
	stConVolSet.nChkDealPrice = nIndex;
	stConVolSet.lDealPrice = lData;
	
//#ü�ᷮ, �ŷ���� And or OR
	for(nIndex = 0; nIndex < 4; nIndex++)
	{
		pButton = (CButton*)GetDlgItem(ID_RADIO_BOTH + nIndex);
		if(pButton->GetCheck() == TRUE)
			break;		
	}

	stConVolSet.nConditionCheck = nIndex;
//#���� �ŷ���
	stConVolSet.nChkAccumDealVol = m_chkAccumVol.GetCheck();

	m_editAccumVol.GetWindowText(strData);
	stConVolSet.lAccumDealVol = atol(strData);
//#���� ����
	CString strUp, strDown;
	
	stConVolSet.nChkSetPrice = m_chkSetPrice.GetCheck();

	m_editSetPriceUp.GetWindowText(strUp);
	m_editSetPriceDown.GetWindowText(strDown);

	stConVolSet.lSetPriceUp = atol(strUp);
	stConVolSet.lSetPriceDown = atol(strDown);
//#����� ����
	stConVolSet.nChkChgRate = m_chkChgRateSet.GetCheck();

	m_editChgRateUp.GetWindowText(strUp);		//�� �̻�
	m_editChgRateDown.GetWindowText(strDown);	//�� ����

	stConVolSet.dChgRateUp = atof(strUp);		 //%�̻�
	stConVolSet.dChgRateDown = atof(strDown);	 //%����

// �������
	for(nIndex = 0; nIndex < 4; ++nIndex)
	{
		pButton = (CButton*)GetDlgItem(IDC_UPJONG1 + nIndex);
		if(pButton->GetCheck() == TRUE)
			break;
	}
	
	stConVolSet.nChkTargetUpjong = nIndex;
	stConVolSet.strMarketCode = _T("");
	if(stConVolSet.nChkTargetUpjong == 3)
	{
		if((m_pctrlSelectUJCombo->GetCode()).GetLength() == 3)
			stConVolSet.strMarketCode = m_pctrlSelectUJCombo->GetCode();
	}

	if(stConVolSet.strMarketCode == "0001") // �ŷ��� >> ��, ��, �� �з��δ� �ŷ���, �ڽ��� ������ ������ �� ����
		stConVolSet.nChkTargetUpjong = 1;
	else if(stConVolSet.strMarketCode == "1001") // �ڽ���
		stConVolSet.nChkTargetUpjong = 2;

// ��������
	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION1);			// ��������
	stConVolSet.nChkManagement = pButton->GetCheck();		

	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION2);			// �켱��
	stConVolSet.nChkPrefferShare = pButton->GetCheck();

	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION3);			// ��������
	stConVolSet.nChkInvestAttention = pButton->GetCheck();

	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION4);			// ���ڰ��
	stConVolSet.nChkInvestWarning = pButton->GetCheck();

	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION5);			// �������迹��
	stConVolSet.nChkPreInvestRisk = pButton->GetCheck();

	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION6);			// ��������
	stConVolSet.nChkInvestRisk = pButton->GetCheck();

	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION7);			// �����Ÿ�
	stConVolSet.nChkTidyDeal = pButton->GetCheck();

	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION8);			// �ŷ�����
	stConVolSet.nChkDealStop = pButton->GetCheck();

	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION9);			// �Ҽ��ǰ���
	stConVolSet.nChkInsincerityNotice = pButton->GetCheck();

	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION10);		// ���ű� 100%
	stConVolSet.nChkDeposit = pButton->GetCheck();

//# �Ҹ����
	stConVolSet.nChkSound = m_chkSound.GetCheck(); 
	CString strSoundPath;
	m_editSound.GetWindowText(strSoundPath);
	stConVolSet.strPathSound = strSoundPath;
	
//# ���� �߰� ��ġ
	for(nIndex = 0; nIndex < 2; nIndex++)
	{
		pButton = (CButton*)GetDlgItem(ID_ADDITEMUP + nIndex);
		if(pButton->GetCheck() == TRUE)
			break;
	}

	stConVolSet.nItemPosition = nIndex;

	return bResult;
}


BOOL CFilterSet::GetInfo()
{
	BOOL bResult = TRUE;	

	CButton *pButton;
	CString strData;
//#ü�ᷮ
	int nIndex = 0;
	for(nIndex = 0; nIndex < _countof(ARR_CONVOL) + 1; nIndex++)
	{
		if(ARR_CONVOL[nIndex] == m_stConVolSet.lConVol)
		{			
			pButton = (CButton*)GetDlgItem(ID_RADIO_CONVOL + nIndex);
			pButton->SetCheck(TRUE);			
			break;
		}
		else if(nIndex == _countof(ARR_CONVOL))
		{
			pButton = (CButton*)GetDlgItem(ID_RADIO_CONVOL + nIndex);
			pButton->SetCheck(TRUE);
			
			CString strConVol;
			strConVol.Format("%d", m_stConVolSet.lConVol);
			m_editConVol.EnableWindow(TRUE);
			m_editConVol.SetWindowText(strConVol);
		}
	}
//#�ŷ����
	for(nIndex = 0; nIndex < _countof(ARR_DEALPRICE) + 1; nIndex++)
	{
		if(ARR_DEALPRICE[nIndex] == m_stConVolSet.lDealPrice)
		{
			pButton = (CButton*)GetDlgItem(ID_RADIO_DEALPRICE + nIndex);
			pButton->SetCheck(TRUE);
			break;
		}
		else if(nIndex == _countof(ARR_DEALPRICE))
		{
			pButton = (CButton*)GetDlgItem(ID_RADIO_DEALPRICE + nIndex);
			pButton->SetCheck(TRUE);
			
			CString strDealPrice;
			strDealPrice.Format("%d", m_stConVolSet.lDealPrice / 10000);
			m_editDealPrice.EnableWindow(TRUE);
			m_editDealPrice.SetWindowText(strDealPrice);
		}
	}
//���� üũ
	nIndex = m_stConVolSet.nConditionCheck;
	pButton = (CButton*)GetDlgItem(ID_RADIO_BOTH + nIndex);
	pButton->SetCheck(TRUE);

//�����ŷ���
	int nCheck;

	nCheck = m_stConVolSet.nChkAccumDealVol;
	m_chkAccumVol.SetCheck(nCheck);

	if(nCheck == 0)
		strData = "";
	else
		strData.Format("%ld", m_stConVolSet.lAccumDealVol);

	m_editAccumVol.SetWindowText(strData);
//#���� ����
	CString strUp, strDown;

	nCheck = m_stConVolSet.nChkSetPrice;
	m_chkSetPrice.SetCheck(nCheck);

	if(nCheck == 0)
	{
		strUp = "";
		strDown = "";
	}
	else
	{
		strUp.Format("%ld", m_stConVolSet.lSetPriceUp);
		strDown.Format("%ld", m_stConVolSet.lSetPriceDown);
	}

	m_editSetPriceUp.SetWindowText(strUp);
	m_editSetPriceDown.SetWindowText(strDown);
//#����� ����
	nCheck = m_stConVolSet.nChkChgRate;
	m_chkChgRateSet.SetCheck(nCheck);

	if(nCheck == 0)
	{
		strUp = "";
		strDown = "";
	}
	else
	{
		strUp.Format("%.2f", m_stConVolSet.dChgRateUp);
		strDown.Format("%.2f", m_stConVolSet.dChgRateDown);
	}

	m_editChgRateUp.SetWindowText(strUp);
	m_editChgRateDown.SetWindowText(strDown);

// �������
	nIndex = m_stConVolSet.nChkTargetUpjong;
	pButton = (CButton*)GetDlgItem(IDC_UPJONG1 + nIndex);
	pButton->SetCheck(TRUE);
	
// ��������
	nCheck = m_stConVolSet.nChkManagement;		
	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION1);			// ��������
	pButton->SetCheck(nCheck);

	nCheck = m_stConVolSet.nChkPrefferShare;
	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION2);			// �켱��
	pButton->SetCheck(nCheck);
	
	nCheck = m_stConVolSet.nChkInvestAttention;
	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION3);			// ��������
	pButton->SetCheck(nCheck);

	nCheck = m_stConVolSet.nChkInvestWarning;	
	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION4);			// ���ڰ��
	pButton->SetCheck(nCheck);

	nCheck = m_stConVolSet.nChkPreInvestRisk;
	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION5);			// �������迹��
	pButton->SetCheck(nCheck);

	nCheck = m_stConVolSet.nChkInvestRisk;
	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION6);			// ��������
	pButton->SetCheck(nCheck);

	nCheck = m_stConVolSet.nChkTidyDeal;					// �����Ÿ�
	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION7);
	pButton->SetCheck(nCheck);

	nCheck = m_stConVolSet.nChkDealStop;					// �ŷ�����
	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION8);
	pButton->SetCheck(nCheck);

	nCheck = m_stConVolSet.nChkInsincerityNotice;			// �Ҽ��ǰ���
	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION9);
	pButton->SetCheck(nCheck);

	nCheck = m_stConVolSet.nChkDeposit;						// ���ű� 100%
	pButton = (CButton*)GetDlgItem(IDC_EXCEPTION10);
	pButton->SetCheck(nCheck);

//#�Ҹ����
	nCheck = m_stConVolSet.nChkSound;
	m_chkSound.SetCheck(nCheck);

	strData = m_stConVolSet.strPathSound;
	
	if(strData.GetLength() > 0)
		m_editSound.SetWindowText(strData);
//#���� �߰� ��ġ
	nIndex = m_stConVolSet.nItemPosition;
	pButton = (CButton*)GetDlgItem(ID_ADDITEMUP + nIndex);
	pButton->SetCheck(TRUE);

	return bResult;
}


BOOL CFilterSet::GetLoad_ConVolInfo(CString strFullPath, StConVolSet	*pstConVolSet, BOOL bRight)
{
	BOOL bLoad = TRUE;
	TCHAR buffer[256];
	CString strData;
	char* pBuff;
	TRY
	{
		::GetPrivateProfileString("FilterSet", "strVer", "1.0", buffer, 256, strFullPath);
		STR_CONVERT(pBuff, buffer, sizeof(buffer), strData);
		pstConVolSet->strVer = strData;
		::GetPrivateProfileString("FilterSet", "strScreenNo", "9999", buffer, 256, strFullPath);
		STR_CONVERT(pBuff, buffer, sizeof(buffer), strData);
		pstConVolSet->strScreenNo = strData;
			
		pstConVolSet->nChkConVol = ::GetPrivateProfileInt("FilterSet", "nChkConVol", 1, strFullPath);
		pstConVolSet->lConVol = ::GetPrivateProfileInt("FilterSet", "lConVol", 10000, strFullPath);
		if(!bRight) pstConVolSet->lConVol = 10000;
 
		pstConVolSet->nChkDealPrice = ::GetPrivateProfileInt("FilterSet", "nChkDealPrice", 1, strFullPath);
		pstConVolSet->lDealPrice = ::GetPrivateProfileInt("FilterSet", "lDealPrice", 50000000, strFullPath);
		if(!bRight) pstConVolSet->lDealPrice = 50000000;
//���� �ϳ��̻� ������ Default�� ����
		pstConVolSet->nConditionCheck = ::GetPrivateProfileInt("FilterSet", "nConditionCheck", 1, strFullPath);
		if(!bRight) pstConVolSet->nConditionCheck = 1;

		pstConVolSet->nChkAccumDealVol = ::GetPrivateProfileInt("FilterSet", "nChkAccumDealVol", 1, strFullPath);
		pstConVolSet->lAccumDealVol = ::GetPrivateProfileInt("FilterSet", "lAccumDealVol", 10000, strFullPath);
		if(!bRight)
		{
			pstConVolSet->nChkAccumDealVol = 1;
			pstConVolSet->lAccumDealVol = 10000;
		}

		pstConVolSet->nChkSetPrice = ::GetPrivateProfileInt("FilterSet", "nChkSetPrice", 0, strFullPath);
		pstConVolSet->lSetPriceUp = ::GetPrivateProfileInt("FilterSet", "lSetPriceUp", 0, strFullPath);
		pstConVolSet->lSetPriceDown = ::GetPrivateProfileInt("FilterSet", "lSetPriceDown", 999999999, strFullPath);
		if(!bRight)
		{
			pstConVolSet->nChkSetPrice = 0;
			pstConVolSet->lSetPriceUp = 0;
			pstConVolSet->lSetPriceDown = 999999999;				
		}

		CString strRateUp, strRateDown;
		pstConVolSet->nChkChgRate = ::GetPrivateProfileInt("FilterSet", "nChkChgRate", 0, strFullPath);
		::GetPrivateProfileString("FilterSet", "dChgRateUp", "0", buffer, 256, strFullPath);
		STR_CONVERT(pBuff, buffer, sizeof(buffer), strRateUp);
		::GetPrivateProfileString("FilterSet", "dChgRateDown", "100", buffer, 256, strFullPath);
		STR_CONVERT(pBuff, buffer, sizeof(buffer), strRateDown);
		
		pstConVolSet->dChgRateUp = atof(strRateUp);
		pstConVolSet->dChgRateDown = atof(strRateDown);
		if(!bRight)
		{
			pstConVolSet->nChkChgRate = 0;
			pstConVolSet->dChgRateUp = 0.0;
			pstConVolSet->dChgRateDown = 100.0;
		}

		pstConVolSet->nChkSound = ::GetPrivateProfileInt("FilterSet", "nChkSound", 0, strFullPath);
		::GetPrivateProfileString("FilterSet", "strPathSound", "", buffer, 256, strFullPath);
		STR_CONVERT(pBuff, buffer, sizeof(buffer), strData);
		pstConVolSet->strPathSound = strData;

		if(!bRight)
		{
			GETPATH_APP(strRootPath);
			strRootPath += "\\image\\alarm.wav";
			strData = strRootPath;

			pstConVolSet->nChkSound = 0;
			pstConVolSet->strPathSound = "";
		}

		pstConVolSet->nItemPosition = ::GetPrivateProfileInt("FilterSet", "nItemPosition", 1, strFullPath);
		if(!bRight)	pstConVolSet->nItemPosition = 1;

		pstConVolSet->nChkTargetUpjong = ::GetPrivateProfileInt("FilterSet", "nChkTargetUpjong", 0, strFullPath);

		pstConVolSet->nChkManagement = ::GetPrivateProfileInt("FilterSet", "nChkManagement", 0, strFullPath);						// ��������
		pstConVolSet->nChkPrefferShare = ::GetPrivateProfileInt("FilterSet", "nChkPrefferShare", 0, strFullPath);					// �켱��
		pstConVolSet->nChkInvestAttention = ::GetPrivateProfileInt("FilterSet", "nChkInvestAttention", 0, strFullPath);				// ��������
		pstConVolSet->nChkInvestWarning = ::GetPrivateProfileInt("FilterSet", "nChkInvestWarning", 0, strFullPath);					// ���ڰ��
		pstConVolSet->nChkPreInvestRisk = ::GetPrivateProfileInt("FilterSet", "nChkPreInvestRisk", 0, strFullPath);					// �������迹��
		pstConVolSet->nChkInvestRisk = ::GetPrivateProfileInt("FilterSet", "nChkInvestRisk", 0, strFullPath);						// ��������

		pstConVolSet->nChkTidyDeal = ::GetPrivateProfileInt("FilterSet", "nChkTidyDeal", 0, strFullPath);							// �����Ÿ�
		pstConVolSet->nChkDealStop = ::GetPrivateProfileInt("FilterSet", "nChkDealStop", 0, strFullPath);							// �ŷ�����
		pstConVolSet->nChkInsincerityNotice = ::GetPrivateProfileInt("FilterSet", "nChkInsincerityNotice", 0, strFullPath);			// �Ҽ��ǰ���
		pstConVolSet->nChkDeposit = ::GetPrivateProfileInt("FilterSet", "nChkDeposit", 0, strFullPath);								// ���ű�

	}
	CATCH(CFileException, pEx)
	{
		bLoad = FALSE;
	}
	END_CATCH
/*
	CFile clsFile;
	if (clsFile.Open(strFullPath, CFile::typeBinary | CFile::modeRead))
	{
		LPSTR lpBuff = NULL;
		DWORD nLen = clsFile.GetLength();
		if(nLen == 0) return FALSE;
		
		lpBuff = new char[nLen];
		memset(lpBuff, 0, nLen);
		clsFile.Read(lpBuff, nLen);
		clsFile.Close();
		
		CMemFile memFile = CMemFile(nLen);
		memFile.Attach((unsigned char*)lpBuff, nLen);
		
		CArchive ar(&memFile, CArchive::load);

		ar >> pstConVolSet->strVer;		
		ar >> pstConVolSet->strScreenNo;
		
		ar >> pstConVolSet->nChkConVol;
		ar >> pstConVolSet->lConVol;

		ar >> pstConVolSet->nChkDealPrice;
		ar >> pstConVolSet->lDealPrice;

		ar >> pstConVolSet->nConditionCheck;

		ar >> pstConVolSet->nChkAccumDealVol;
		ar >> pstConVolSet->lAccumDealVol;

		ar >> pstConVolSet->nChkSetPrice;
		ar >> pstConVolSet->lSetPriceUp;
		ar >> pstConVolSet->lSetPriceDown;

		ar >> pstConVolSet->nChkChgRate;
		ar >> pstConVolSet->dChgRateUp;
		ar >> pstConVolSet->dChgRateDown;

		ar >> pstConVolSet->nChkSound;
		ar >> pstConVolSet->strPathSound;

		ar >> pstConVolSet->nItemPosition;

		ar.Close();
		if (lpBuff) 
			delete lpBuff;

		return TRUE;
	}
*/
	
	return bLoad;
}

BOOL CFilterSet::SetSave_ConVolInfo(CString strFullPath, StConVolSet	*pstConVolSet)
{	
	CString strIntData;
	CString strLongData;
	
	BOOL bSave = TRUE;

	CString strData;

	TRY
	{
		::WritePrivateProfileString("FilterSet", "strVer", pstConVolSet->strVer, strFullPath);
		::WritePrivateProfileString("FilterSet", "strScreenNo", pstConVolSet->strScreenNo, strFullPath);

		strIntData.Format("%d", pstConVolSet->nChkConVol);
		strLongData.Format("%ld", pstConVolSet->lConVol);
		::WritePrivateProfileString("FilterSet", "nChkConVol", strIntData, strFullPath);
		::WritePrivateProfileString("FilterSet", "lConVol", strLongData, strFullPath);

		strIntData.Format("%d", pstConVolSet->nChkDealPrice);
		strLongData.Format("%ld", pstConVolSet->lDealPrice);
		::WritePrivateProfileString("FilterSet", "nChkDealPrice", strIntData, strFullPath);
		::WritePrivateProfileString("FilterSet", "lDealPrice", strLongData, strFullPath);

		strIntData.Format("%d", pstConVolSet->nConditionCheck);
		::WritePrivateProfileString("FilterSet", "nConditionCheck", strIntData, strFullPath);

		strIntData.Format("%d", pstConVolSet->nChkAccumDealVol);
		strLongData.Format("%ld", pstConVolSet->lAccumDealVol);
		::WritePrivateProfileString("FilterSet", "nChkAccumDealVol", strIntData, strFullPath);
		::WritePrivateProfileString("FilterSet", "lAccumDealVol", strLongData, strFullPath);

		CString strLongData2;
		strIntData.Format("%d", pstConVolSet->nChkSetPrice);
		strLongData.Format("%ld", pstConVolSet->lSetPriceUp);
		strLongData2.Format("%ld", pstConVolSet->lSetPriceDown);
		::WritePrivateProfileString("FilterSet", "nChkSetPrice", strIntData, strFullPath);
		::WritePrivateProfileString("FilterSet", "lSetPriceUp", strLongData, strFullPath);	
		::WritePrivateProfileString("FilterSet", "lSetPriceDown", strLongData2, strFullPath);

		CString strDoubleData, strDoubleData2;
		strIntData.Format("%d", pstConVolSet->nChkChgRate);
		strDoubleData.Format("%lf", pstConVolSet->dChgRateUp);
		strDoubleData2.Format("%lf", pstConVolSet->dChgRateDown);
		::WritePrivateProfileString("FilterSet", "nChkChgRate", strIntData, strFullPath);
		::WritePrivateProfileString("FilterSet", "dChgRateUp",strDoubleData, strFullPath);
		::WritePrivateProfileString("FilterSet", "dChgRateDown", strDoubleData2, strFullPath);

		strIntData.Format("%d", pstConVolSet->nChkSound);
		::WritePrivateProfileString("FilterSet", "nChkSound", strIntData, strFullPath);
		::WritePrivateProfileString("FilterSet", "strPathSound", pstConVolSet->strPathSound, strFullPath);

		strIntData.Format("%d", pstConVolSet->nItemPosition);
		::WritePrivateProfileString("FilterSet", "nItemPosition", strIntData, strFullPath);

		strIntData.Format("%d", pstConVolSet->nChkTargetUpjong);
		::WritePrivateProfileString("FilterSet", "nChkTargetUpjong", strIntData, strFullPath);		// �������

		strIntData.Format("%d", pstConVolSet->nChkManagement);
		::WritePrivateProfileString("FilterSet", "nChkManagement", strIntData, strFullPath);		// ��������

		strIntData.Format("%d", pstConVolSet->nChkPrefferShare);
		::WritePrivateProfileString("FilterSet", "nChkPrefferShare", strIntData, strFullPath);		// �켱��

		strIntData.Format("%d", pstConVolSet->nChkInvestAttention);
		::WritePrivateProfileString("FilterSet", "nChkInvestAttention", strIntData, strFullPath);	// ��������

		strIntData.Format("%d", pstConVolSet->nChkInvestWarning);
		::WritePrivateProfileString("FilterSet", "nChkInvestWarning", strIntData, strFullPath);		// ���ڰ��

		strIntData.Format("%d", pstConVolSet->nChkPreInvestRisk);
		::WritePrivateProfileString("FilterSet", "nChkPreInvestRisk", strIntData, strFullPath);		// �������迹��

		strIntData.Format("%d", pstConVolSet->nChkTidyDeal);
		::WritePrivateProfileString("FilterSet", "nChkTidyDeal", strIntData, strFullPath);			// �����Ÿ�

		strIntData.Format("%d", pstConVolSet->nChkDealStop);
		::WritePrivateProfileString("FilterSet", "nChkDealStop", strIntData, strFullPath);		// �ŷ�����

		strIntData.Format("%d", pstConVolSet->nChkInsincerityNotice);
		::WritePrivateProfileString("FilterSet", "nChkInsincerityNotice", strIntData, strFullPath);		// �Ҽ��ǰ���

		strIntData.Format("%d", pstConVolSet->nChkDeposit);
		::WritePrivateProfileString("FilterSet", "nChkDeposit", strIntData, strFullPath);		// ���ű�

		strIntData.Format("%d", pstConVolSet->nChkInvestRisk);
		::WritePrivateProfileString("FilterSet", "nChkInvestRisk", strIntData, strFullPath);		// ��������
	}
	CATCH(CFileException, pEx)
	{
		bSave = FALSE;
	}
	END_CATCH

/*
	CFile clsFile;
	if(!clsFile.Open(strFullPath, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite)) 
		return FALSE;
	CArchive ar(&clsFile, CArchive::store);

	ar << pstConVolSet->strVer;
	ar << pstConVolSet->strScreenNo;

	ar << pstConVolSet->nChkConVol;
	ar << pstConVolSet->lConVol;

	ar << pstConVolSet->nChkDealPrice;
	ar << pstConVolSet->lDealPrice;

	ar << pstConVolSet->nConditionCheck;

	ar << pstConVolSet->nChkAccumDealVol;
	ar << pstConVolSet->lAccumDealVol;

	ar << pstConVolSet->nChkSetPrice;
	ar << pstConVolSet->lSetPriceUp;
	ar << pstConVolSet->lSetPriceDown;

	ar << pstConVolSet->nChkChgRate;
	ar << pstConVolSet->dChgRateUp;
	ar << pstConVolSet->dChgRateDown;

	ar << pstConVolSet->nChkSound;
	ar << pstConVolSet->strPathSound;

	ar << pstConVolSet->nItemPosition;

	ar.Close();
	clsFile.Close();
*/
	return bSave;

}

void CFilterSet::OnDestroy() 
{
	if(m_pctrlSelectUJCombo)
	{
		m_pctrlSelectUJCombo->DestroyWindow();
		delete m_pctrlSelectUJCombo;
		m_pctrlSelectUJCombo = NULL;
	}
	
	if(m_pctrlExceptJMCombo)
	{
		m_pctrlExceptJMCombo->DestroyWindow();
		delete m_pctrlExceptJMCombo;
		m_pctrlExceptJMCombo = NULL;
	}

	m_fontDefault.DeleteObject();

	m_gridExcept.DestroyWindow();
	m_brushCtrl.DeleteObject();

	CDialog::OnDestroy();	
}

void CFilterSet::OnChkAcumdealvol() 
{
	BOOL bEnable = m_chkAccumVol.GetCheck();
	m_editAccumVol.EnableWindow(bEnable);
}

void CFilterSet::OnChkSetprice() 
{
	BOOL bEnable = m_chkSetPrice.GetCheck();
	m_editSetPriceUp.EnableWindow(bEnable);
	m_editSetPriceDown.EnableWindow(bEnable);	
}

void CFilterSet::OnChkSetupdownrate() 
{
	BOOL bEnable = m_chkChgRateSet.GetCheck();
	m_editChgRateUp.EnableWindow(bEnable);
	m_editChgRateDown.EnableWindow(bEnable);	
}

void CFilterSet::OnChkSound() 
{
	BOOL bEnable = m_chkSound.GetCheck();
	m_editSound.EnableWindow(bEnable);	
	m_btnSound.EnableWindow(bEnable);
	m_btnSearchSound.EnableWindow(bEnable);
}

void CFilterSet::OnRadioConvolUser(UINT nID) 
{
	if(nID == IDC_RADIO_CONVOL6)
	{
		CString strConVol;
		int nStringSize;

		m_editConVol.EnableWindow(TRUE);
		m_editConVol.GetWindowText(strConVol);
		nStringSize = strConVol.GetLength();	
		
		m_editConVol.SetSel(0, nStringSize);
		m_editConVol.SetFocus();
	}
	else
		m_editConVol.EnableWindow(FALSE);
}

void CFilterSet::OnRadioDealpriceUser(UINT nID) 
{
	if(nID == IDC_RADIO_DEALPRICE9)
	{
		CString strDealPrice;
		int nStringSize;

		m_editDealPrice.EnableWindow(TRUE);
		m_editDealPrice.GetWindowText(strDealPrice);
		nStringSize = strDealPrice.GetLength();	
		
		m_editDealPrice.SetSel(0, nStringSize);
		m_editDealPrice.SetFocus();
	}
	else
		m_editDealPrice.EnableWindow(FALSE);
}

BOOL CFilterSet::ErrorCheck()
{
	BOOL bNormal = TRUE;

	CString strData;
	CButton *pButton;

	pButton = (CButton*)GetDlgItem(ID_RADIO_CONVOL+5);
	if(pButton->GetCheck() == 1)
	{		
		m_editConVol.GetWindowText(strData);
		
		if(strData == "" || atoi(strData) < 5000)
		{
			ErrorMessage(ERR_STRMSG_CONVOL);
			bNormal = FALSE;
			return bNormal;
		}
	}

	pButton = (CButton*)GetDlgItem(ID_RADIO_DEALPRICE+5);
	if(pButton->GetCheck() == 1)
	{
		m_editDealPrice.GetWindowText(strData);
		
		if(strData == "" || atoi(strData) < 5000)
		{
			ErrorMessage(ERR_STRMSG_DEALPRICE);
			bNormal = FALSE;
			return bNormal;
		}
	}


//�����ŷ���
	BOOL bUseAccumVol = IsDlgButtonChecked(m_chkAccumVol.GetDlgCtrlID());
	long lData;

	if(bUseAccumVol)
	{
		GetDlgItem(IDC_EDIT_ACUMDEALVOL)->GetWindowText(strData);
		lData = atol(strData);

		if(strData == "")
		{
			//ErrorMessage(ERR_STRMSG_EMPTY);
			ErrorMessage(ERR_STRMSG_USER, "�����ŷ����� �Է¹ٶ��ϴ�.");
			bNormal = FALSE;
			return bNormal;
		}
		if(lData < 0 )
		{
			//ErrorMessage(ERR_STRMSG_ACCUM);
			ErrorMessage(ERR_STRMSG_USER, "�����ŷ����� 0�̻� �����մϴ�.");
			bNormal = FALSE;
			return bNormal;
		}
	}

	long lDataUp, lDataDown;
	//��������
	BOOL bUseSetPrice = IsDlgButtonChecked(m_chkSetPrice.GetDlgCtrlID());
	if(bUseSetPrice)
	{		
		GetDlgItem(IDC_EDIT_SETPRICEUP)->GetWindowText(strData);
		lDataUp = atol(strData);

		GetDlgItem(IDC_EDIT_SETPRICEDOWN)->GetWindowText(strData);
		lDataDown = atol(strData);

		if(strData == "")
		{
			//ErrorMessage(ERR_STRMSG_EMPTY);
			ErrorMessage(ERR_STRMSG_USER, "���������� �Է¹ٶ��ϴ�.");
			bNormal = FALSE;
			return bNormal;
		}
		if(lDataUp < 0 || lDataDown <0 || lDataUp > lDataDown)
		{
			//ErrorMessage(ERR_STRMSG_PRICE);
			ErrorMessage(ERR_STRMSG_USER, "���������� 0�̻� �����մϴ�.");
			bNormal = FALSE;
			return bNormal;
		}
	}

	//����� ����
	BOOL bUseChgRateSet = IsDlgButtonChecked(m_chkChgRateSet.GetDlgCtrlID());
	if(bUseChgRateSet)
	{
		GetDlgItem(IDC_EDIT_SETRATEUP)->GetWindowText(strData);
		TRIMALL(strData);
		lDataUp = atol(strData);

		GetDlgItem(IDC_EDIT_SETRATEDOWN)->GetWindowText(strData);
		TRIMALL(strData);
		if(strData.GetLength()==0) strData="15.00";
		lDataDown = atol(strData);

		if(strData == "")
		{
			//ErrorMessage(ERR_STRMSG_EMPTY);
			ErrorMessage(ERR_STRMSG_USER, "����� �Է°��� �Է¹ٶ��ϴ�.");
			bNormal = FALSE;
			return bNormal;
		}
		if(lDataUp > lDataDown)
		{
			//ErrorMessage(ERR_STRMSG_CHGRATE);
			ErrorMessage(ERR_STRMSG_USER, "����� �Է°��� Ȯ�� �ٶ��ϴ�.");
			bNormal = FALSE;
			return bNormal;
		}
	}


	// �Ҹ����
	BOOL bUseSound = IsDlgButtonChecked(m_chkSound.GetDlgCtrlID());
	if(bUseSound)
	{
		GetDlgItem(IDC_EDIT_SOUND)->GetWindowText(strData);
		if(strData == "")
		{
			ErrorMessage(ERR_STRMSG_USER, "�Ҹ������� ���� �ٶ��ϴ�.");
			bNormal = FALSE;
			return bNormal;
		}
	}

	return bNormal;
	
}

void CFilterSet::ErrorMessage(UINT nID, LPCSTR szMsg)
{
	switch(nID) 
	{
		case ERR_STRMSG_USER:
			{
				SetMessageBox(szMsg, (CString)PROGRAM_TITLE, MB_OK);
				break;
			}
		case ERR_STRMSG_CONVOL :
			{
				SetMessageBox(_T("ü�ᷮ�� 5000�� �̻��� ���� �Է��ϼ���"), (CString)PROGRAM_TITLE, MB_OK);
				break;
			}
		
		case ERR_STRMSG_DEALPRICE:
			{
				SetMessageBox(_T("�ŷ������ 5000���� �̻��� ���� �Է��ϼ���"), (CString)PROGRAM_TITLE, MB_OK);
				break;
			}	
		case ERR_STRMSG_ACCUM:
			{
				SetMessageBox(_T("�����ŷ����� 0�̻��� ���� �Է��ϼ���"), (CString)PROGRAM_TITLE, MB_OK);
				break;
			}	
		case ERR_STRMSG_PRICE:
			{
				SetMessageBox(_T("���������� 0�̻��� ���� �Է��ϼ���.(���ϰ��� �� ū�� Ȯ�� �Ͻʽÿ�)"), (CString)PROGRAM_TITLE, MB_OK);
				break;
			}	
		case ERR_STRMSG_CHGRATE:
			{
				SetMessageBox(_T("����������� -100 ~ 100 ������ ���� �Է��ϼ���. (���ϰ��� �� ū�� Ȯ�� �Ͻʽÿ�)"), (CString)PROGRAM_TITLE, MB_OK);
				break;
			}
		case ERR_STRMSG_EMPTY:
			{
				SetMessageBox(_T("�Է����� ���� �׸��� �ֽ��ϴ�. �ٽ� �� �� Ȯ�� ���ּ���"), (CString)PROGRAM_TITLE, MB_OK);
				break;
			}
		default:
			break;
	}	
}

void CFilterSet::OnOK() 
{
	BOOL bResult = OnApply();
	if(!bResult)
		return;

	SetInfo(m_stConVolSet);
	SetExceptItem();
	// Lockó�� ������ MainFrame���� ���� �Ѵ�.
	STCOPY_P_StConVolSet((&m_stConVolSet), m_pMainData);

	//CMainFrmWnd* pMainFrm = (CMainFrmWnd*)GetParent();
	((CMainFrmWnd*)m_pMainParent)->OnFilterSetMessage(IDOK);
	ShowWindow(SW_HIDE);
	//CDialog::OnOK();
}

void CFilterSet::SetExceptItem()
{
	CMainFrmWnd* pMainFrm = (CMainFrmWnd*)m_pMainParent;
	
	int nItemCode = m_arItemCode.GetSize();
	int nItemName = m_arItemName.GetSize();

	arItemCode->RemoveAll();
	arItemName->RemoveAll();

	for(int i = 0; i < nItemCode; i++)
	{
		arItemCode->Add(m_arItemCode.GetAt(i));
		arItemName->Add(m_arItemName.GetAt(i));
	}
}

void CFilterSet::OnBtnSound() 
{	
	BOOL bOK = FALSE;
	CString strFilePath;
	m_editSound.GetWindowText(strFilePath);
	

	if(strFilePath.GetLength() > 0)
		bOK = sndPlaySound(strFilePath, SND_ASYNC );
	
}

void CFilterSet::OnBtnSearchsound() 
{
	CString file, fileFullName, fileName, fileExt;
	file = m_stConVolSet.strPathSound;

	if(file == "")
	{
		CMainFrmWnd* pMainFrm = (CMainFrmWnd*)m_pMainParent;
		if(pMainFrm)
		{
			CString strAppPath = pMainFrm->Variant(homeCC);
			file = strAppPath += "\\image\\alarm.wav";
		}
		else
		{
			GETPATH_APP(strAppPath);
			int nPos = strAppPath.ReverseFind('\\');
			file = strAppPath.Left(nPos);
			file = file +  "\\image\\alarm.wav";		
		}		
	}

	if(file != "")
	{		
		int nTot = file.GetLength();
		int nPos = file.ReverseFind('\\');
		fileFullName = file.Right( nTot - (nPos + 1) );
		nPos = fileFullName.ReverseFind('.');
		fileExt = fileFullName.Right(fileFullName.GetLength() - (nPos + 1));
		fileName = fileFullName.Left(nPos);
	}
	
	if(fileFullName.GetLength() <= 3 || fileFullName == "" )
		fileFullName = "alarm.wav";	

	char szFilter[] = "Sound Files(*.wav)|*.wav|All Files(*.*)|*.*||";
	CFileDialog fileDlg(TRUE, "wav", file, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);	
	
	if(IDOK == fileDlg.DoModal())
	{
		file = fileDlg.GetPathName();
		m_editSound.SetWindowText(file);		
		m_stConVolSet.strPathSound = file;		
	}	
}

void CFilterSet::OnCancel() 
{	
	//CMainFrmWnd* pMainFrm = (CMainFrmWnd*)GetParent();
	((CMainFrmWnd*)m_pMainParent)->OnFilterSetMessage(IDCANCEL);

	ShowWindow(SW_HIDE);
//	CDialog::OnCancel();
}

void CFilterSet::SetUserRight(BOOL bRight)
{
	m_bRight = bRight;
}

void CFilterSet::ControlEnable(BOOL bEnable)
{
	CWnd*	pCtrl = GetWindow(GW_CHILD);	// Gets first child

	CRect	rCtrl, rTest;
	CRect	rtDlg;
	GetWindowRect(&rtDlg);
	short nControlID;

	while(pCtrl)
	{		
		if(pCtrl == this)
		{
			pCtrl = pCtrl->GetWindow(GW_HWNDNEXT);
			continue;
		}

		nControlID = (short)pCtrl->GetDlgCtrlID();

		if(nControlID == IDC_STATIC_RIGHT || nControlID == IDOK || nControlID == IDCANCEL)
		{
			pCtrl = pCtrl->GetWindow(GW_HWNDNEXT);
			continue;
		}

		pCtrl->GetWindowRect(&rCtrl);

		if(rTest.IntersectRect(rtDlg, rCtrl))
			pCtrl->EnableWindow(bEnable);

		pCtrl = pCtrl->GetWindow(GW_HWNDNEXT);
	}
}

int CFilterSet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CMainFrmWnd* pMainFrm = (CMainFrmWnd*)m_pMainParent;

	CRect rtNULL(0, 0, 0, 0);

	m_TradingValue_line.Create(_T(""), WS_VISIBLE | WS_CHILD | SS_GRAYFRAME,
		rtNULL, this, IDC_STATIC_LINE);

	m_btnOk.Create(_T("Ȯ��"), rtNULL, this, IDOK);
	pMainFrm->SetBtnImg(&m_btnOk, 4, FALSE);
	
	m_btnCancel.Create(_T("���"), rtNULL, this, IDCANCEL);
	pMainFrm->SetBtnImg(&m_btnCancel, 4, FALSE);

	m_btnAdd.Create(_T("�߰�"), rtNULL, this, IDC_BTN_ADD);
	pMainFrm->SetBtnImg(&m_btnAdd, 2);
	
	m_btnRemove.Create(_T("����"), rtNULL, this, IDC_BTN_REMOVE);
	pMainFrm->SetBtnImg(&m_btnRemove, 2);
	
	m_btnRemoveAll.Create(_T("��ü����"), rtNULL, this, IDC_BTN_REMOVEALL);
	pMainFrm->SetBtnImg(&m_btnRemoveAll, 4);

	CScreenWnd* pScreenWnd = (CScreenWnd*)pMainFrm;

	if (m_pctrlSelectUJCombo == NULL)
	{
		CRect rtNULL(0, 0, 0, 0);
		
		m_pctrlSelectUJCombo = new CCodeCtrl(pMainFrm->GetModuleMain());
		m_pctrlSelectUJCombo->SetRootPath(pMainFrm->Variant(homeCC));
		m_pctrlSelectUJCombo->UseCodeDlg(TRUE);
		m_pctrlSelectUJCombo->CreateCodeCtrl(this, pMainFrm->GetModuleMain(), pScreenWnd, rtNULL, IDC_CODECTRL_ITEM_SELECT);
		m_pctrlSelectUJCombo->SetColor(RGB(0,0,0), RGB(255,255,255));
		m_pctrlSelectUJCombo->SetUnit("����");
	}

	if (m_pctrlExceptJMCombo == NULL)
	{
		CRect rtNULL(0, 0, 0, 0);
		
		m_pctrlExceptJMCombo = new CCodeCtrl(pMainFrm->GetModuleMain());
		m_pctrlExceptJMCombo->SetRootPath(pMainFrm->Variant(homeCC));
		m_pctrlExceptJMCombo->UseCodeDlg(TRUE);
		m_pctrlExceptJMCombo->CreateCodeCtrl(this, pMainFrm->GetModuleMain(), pScreenWnd, rtNULL, IDC_CODECTRL_ITEM_EXCEPT);
		m_pctrlExceptJMCombo->SetColor(RGB(0,0,0), RGB(255,255,255));
		m_pctrlExceptJMCombo->SetUnit("�ֽ�");
	}

	LOGFONT logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);

	m_gridExcept.Create(NULL, _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, NULL);
	m_gridExcept.SetFont(&m_fontDefault);

	m_staticNumExcept.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0),this, NULL);
	m_staticNumExcept.SetFont(&m_fontDefault);

	return 0;
}

void CFilterSet::OnSetNumExceptText()
{
	int nExcept = m_arItemCode.GetSize();
	CString szText;
	szText.Format("���� ���� : %d��", nExcept);

	m_staticNumExcept.SetWindowText(szText);
}

void CFilterSet::AddExceptItem(LPCTSTR szItemCode, LPCTSTR szItemName)
{
	for(int i = 0; i < m_arItemCode.GetSize(); i++)
	{
		if(szItemCode == m_arItemCode.GetAt(i))
			return;
	}

	m_arItemCode.Add(szItemCode);
	m_arItemName.Add(szItemName);
	m_gridExcept.SetAdd_Row(szItemCode, szItemName);

	m_gridExcept.RedrawAll();
	OnSetNumExceptText();
}

void CFilterSet::SetInitExceptItem(CStringArray* _arItemCode, CStringArray* _arItemName)
{
	m_arItemCode.RemoveAll();
	m_arItemName.RemoveAll();

	int nIndexItemCode = 0;
	int nIndexItemName = 0;
	int i = 0;

	arItemCode = _arItemCode;
	arItemName = _arItemName;

	for( i = 0; i < _arItemCode->GetSize(); i++)
	{
		m_arItemCode.Add(_arItemCode->GetAt(i));
	}

	/*nIndexItemCode = arItemCode->GetCount();*/
	/*nIndexItemName = arItemName->GetCount();*/

	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pMainParent;
	char szBuf[16];
	CString strName;

	for(i = 0; i < m_arItemCode.GetSize(); i++)
	{
		sprintf(szBuf, "%s ", m_arItemCode.GetAt(i));
		strName = (char*)pMainFrmWnd->GetModuleMain()->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)szBuf);
		strName.TrimLeft();
		strName.TrimRight();
		m_arItemName.Add(strName);
		arItemName->Add(strName);
	}
	nIndexItemName = m_arItemName.GetSize();

	if(nIndexItemCode == 0 && nIndexItemName == 0)
		return;
// 	else if(nIndexItemCode != nIndexItemName)
// 	{
// 		g_iMetaTable.ShowMessageBox(m_hWnd, "err!");
// 		return;
// 	}

// 	for(int i = 0; i < nIndexItemCode; i++)
// 	{
// 		m_arItemCode.Add(arItemCode->GetAt(i));
// 		m_arItemName.Add(arItemName->GetAt(i));
// 	}
}

void CFilterSet::SetInItExceptGrid()
{
	int nIndexItemCode = 0;
	int nIndexItemName = 0;

	nIndexItemCode = m_arItemCode.GetSize();
	nIndexItemName = m_arItemName.GetSize();

	if(nIndexItemCode == 0 && nIndexItemName == 0)
		return;
	else if(nIndexItemCode != nIndexItemName)
	{
		SetMessageBox(_T("err!"), PROGRAM_TITLE, MB_OK);
		return;
	}

	for(int i = 0; i < nIndexItemCode; i++)
		m_gridExcept.SetAdd_Row(m_arItemCode.GetAt(i), m_arItemName.GetAt(i));

	m_gridExcept.RedrawAll();
}

void CFilterSet::RemoveExceptItem()
{
	CString strCode, strName, strData;
	int nIndex = 0;
	m_gridExcept.GetData_Row(m_gridExcept.m_nLastSelRow, strCode, strName);

	for ( nIndex = 0; nIndex < arItemCode->GetSize(); nIndex++)
	{
		strData = arItemCode->GetAt(nIndex);
		if (strCode == strData.Left((int)CODE_LEN))
		{
			arItemCode->RemoveAt(nIndex);
			arItemName->RemoveAt(nIndex);
			break;
		}
	}

	for(nIndex = 0; nIndex < m_arItemCode.GetSize(); nIndex++)
	{
		if(strCode == m_arItemCode.GetAt(nIndex))
		{
			m_arItemCode.RemoveAt(nIndex);
			m_arItemName.RemoveAt(nIndex);
			break;
		}
	}

	m_gridExcept.SetDelete_Row(m_gridExcept.m_nLastSelRow);
}

void CFilterSet::RemoveAllExceptItem()
{
	m_arItemCode.RemoveAll();
	m_arItemName.RemoveAll();
	m_gridExcept.ClearAll();
}

void CFilterSet::OnRadioUpjong()
{
	CButton* pWndBtn = (CButton*)GetDlgItem(IDC_UPJONG4);
	if(pWndBtn->GetCheck())
		m_pctrlSelectUJCombo->EnableWindow(TRUE);
	else
		m_pctrlSelectUJCombo->EnableWindow(FALSE);
}
void CFilterSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting	

}

HBRUSH CFilterSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	UINT id = pWnd->GetDlgCtrlID();
// 	if(nCtlColor == CTLCOLOR_STATIC)
// 	{
// 		pDC->SetBkMode(TRANSPARENT);
// 		pDC->SetBkColor(m_clrBk);
// 		hbr = m_brushCtrl;
// 	}
// 	else
// 	{	
		switch(id)
		{
			case IDC_EDIT_CONVOL :
			case IDC_EDIT_DEALPRICE :
			case IDC_EDIT_ACUMDEALVOL :
			case IDC_EDIT_SETPRICEUP :
			case IDC_EDIT_SETRATEUP :
			case IDC_EDIT_SETPRICEDOWN :
			case IDC_EDIT_SETRATEDOWN :
			case IDC_EDIT_SOUND : break;
			default :
				pDC->SetBkMode(TRANSPARENT);
				hbr = m_brushCtrl;
		}
//	}

	return hbr;
}

BOOL CFilterSet::OnEraseBkgnd(CDC* pDC)
{
	CRect reClient;
	GetClientRect(reClient);
	pDC->FillSolidRect(reClient, m_clrBk);

	return FALSE;
}

int CFilterSet::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
// 	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pMainParent;
// 	return pMainFrmWnd->SetMessageBox(lpszText, lpszCaption, nType);

	char* szOriginTitle = (char*)AfxGetApp()->m_pszAppName;
	AfxGetApp()->m_pszAppName = (LPCTSTR)lpszCaption;
	AfxMessageBox(lpszText);
	AfxGetApp()->m_pszAppName = szOriginTitle;

	return TRUE;
}

void CFilterSet::OnBtnAdd()
{
	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pMainParent;
	CString strCode, strName;

	strCode = m_pctrlExceptJMCombo->GetCode();

	if(strCode.GetLength() != 6)
		return;

	char szBuf[16];
	sprintf(szBuf, "%s ", strCode);
	strName = (char*)pMainFrmWnd->GetModuleMain()->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)szBuf);
	strName.TrimLeft();
	strName.TrimRight();

	pMainFrmWnd->OnBtnAdd(strCode, strName);

	OnSetNumExceptText();
}

void CFilterSet::OnBtnRemove()
{	
	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pMainParent;
	pMainFrmWnd->OnBtnRemove();

	OnSetNumExceptText();
}

void CFilterSet::OnBtnRemoveAll()
{
	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pMainParent;
	pMainFrmWnd->OnBtnRemoveAll();

	OnSetNumExceptText();
}

int CFilterSet::GetBtnWidth(int nTextLen)
{
	switch(nTextLen)
	{
	case 2 :
		return 36;
	case 4 :
		return 60;
	case 6 :
		return 88;
	}
	return 0;
}

// 2012.4.27 �ڼ���: ���Ǽ���â���� ESC ������ ����ü�� ȭ�鵵 ������ ���� ���� >>
BOOL CFilterSet::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_ESCAPE)
		{
			ShowWindow(SW_HIDE);
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
// 2012.4.27 �ڼ���: ���Ǽ���â���� ESC ������ ����ü�� ȭ�鵵 ������ ���� ���� <<
