// TabRunSignal.cpp : implementation file
//

#include "stdafx.h"
#include "TabRunSignal.h"
#include "../Common_ST/STControllerDef.h"
#include "../Include_ST/ISTControllerD.h"
#include "../Common_ST/STConfigDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		TRS_ST_BUY				"BUY"
#define		TRS_ST_EXITLONG			"ExitLong"
#define		TRS_ST_SELL				"SELL"
#define		TRS_ST_EXITSHORT		"ExitShort"


/////////////////////////////////////////////////////////////////////////////
// CTabRunSignal dialog

CTabRunSignal::CTabRunSignal(CWnd* pParent /*=NULL*/)
	: CRscDlg(CTabRunSignal::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabRunSignal)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabRunSignal::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabRunSignal)
	DDX_Control(pDX, IDC_CB_SELL, m_cbSell);
	DDX_Control(pDX, IDC_CB_EXITSHORT, m_cbExitshort);
	DDX_Control(pDX, IDC_CB_EXITLONG, m_cbExitlong);
	DDX_Control(pDX, IDC_CB_BUY, m_cbBuy);
	DDX_Control(pDX, IDC_BT_CLR_SELL, m_btnClrSell);
	DDX_Control(pDX, IDC_BT_CLR_EXITSHORT, m_btnClrExitshort);
	DDX_Control(pDX, IDC_BT_CLR_EXITLONG, m_btnClrExitlong);
	DDX_Control(pDX, IDC_BT_CLR_BUY, m_btnClrBuy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabRunSignal, CRscDlg)
	//{{AFX_MSG_MAP(CTabRunSignal)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BT_CLR_BUY, OnBtClrBuy)
	ON_BN_CLICKED(IDC_BT_CLR_EXITLONG, OnBtClrExitlong)
	ON_BN_CLICKED(IDC_BT_CLR_EXITSHORT, OnBtClrExitshort)
	ON_BN_CLICKED(IDC_BT_CLR_SELL, OnBtClrSell)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabRunSignal message handlers

BOOL CTabRunSignal::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	CConfigHelper::cmSignal(&m_cbBuy, 0);
	CConfigHelper::cmSignal(&m_cbExitlong, 1);
	CConfigHelper::cmSignal(&m_cbSell, 1);
	CConfigHelper::cmSignal(&m_cbExitshort, 0);

	LoadInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


// ST���Ͽ��� ������ �д´�.
void CTabRunSignal::LoadInfo()
{
	CSTConfigEnv envHelper;
	envHelper.LoadInfo(m_szCFGFile,  m_szDefCFGFile, STConfigEnv_Signal);

	//CString szKey, szTmp;
	//COLORREF clrSignal = RGB(0,0,0);
	//szTmp.GetBufferSetLength(MAX_PATH);

	// ��ȣ������ �д´�. Start...
	// �ż�
	m_cbBuy.SetCurSel(envHelper.m_InfoSignal.m_ShapeBuy);

	// �ż�û��
	m_cbExitlong.SetCurSel(envHelper.m_InfoSignal.m_ShapeExitlong);

	// �ŵ�
	m_cbSell.SetCurSel(envHelper.m_InfoSignal.m_ShapeSell);

	// �ŵ�û��
	m_cbExitshort.SetCurSel(envHelper.m_InfoSignal.m_ShapeExitshort);
	// ��ȣ������ �д´�. End


	// ���� ������ �о�´�. Start...
	// �ż�
	m_btnClrBuy.SetColor(atol(envHelper.m_InfoSignal.m_ClrBuy));

	// �ż�û��
	m_btnClrExitlong.SetColor(atol(envHelper.m_InfoSignal.m_ClrExitlong));

	// �ŵ�
	m_btnClrSell.SetColor(atol(envHelper.m_InfoSignal.m_ClrSell));

	// �ŵ�û��;
	m_btnClrExitshort.SetColor(atol(envHelper.m_InfoSignal.m_ClrExitshort));
	// ���� ������ �о�´�. End

	// ���� : $SRC/STController/STController.cpp
//	dwSizeRead = GetPrivateProfileStringInST(SIGNAL,SHOWSIGNALNAME,"0",szBuffer,sizeof(szBuffer));
//	dwSizeRead = GetPrivateProfileStringInST(SIGNAL,SHOWSIGNALQTY,"0",szBuffer,sizeof(szBuffer));

//	szTmp.ReleaseBuffer();
}

//�Ÿż��� �������� Save ��ư �����ÿ� : KHD 
BOOL CTabRunSignal::SaveInfo()
{
	CString szKey, szTmp;
	
	// ��ȣ����  Start...
	int nSel = 0;
	//szKey.Format("CB_%s", TRS_ST_BUY);			// �ż�
	szKey = SHAPEBUY;
	szTmp.Format("%d", m_cbBuy.GetCurSel());
	::WritePrivateProfileString(STCONFIG_SIGNAL, szKey, szTmp, m_szCFGFile);

	//szKey.Format("CB_%s", TRS_ST_EXITLONG);		// �ż�û��
	szKey = SHAPEEXITLONG;
	szTmp.Format("%d", m_cbExitlong.GetCurSel());
	::WritePrivateProfileString(STCONFIG_SIGNAL, szKey, szTmp, m_szCFGFile);

	//szKey.Format("CB_%s", TRS_ST_SELL);			// �ŵ�
	szKey = SHAPESELL;
	szTmp.Format("%d", m_cbSell.GetCurSel());
	::WritePrivateProfileString(STCONFIG_SIGNAL, szKey, szTmp, m_szCFGFile);

	//szKey.Format("CB_%s", TRS_ST_EXITSHORT);	// �ŵ�û��
	szKey = SHAPEEXITSHORT;
	szTmp.Format("%d", m_cbExitshort.GetCurSel());
	::WritePrivateProfileString(STCONFIG_SIGNAL, szKey, szTmp, m_szCFGFile);
	// ��ȣ������ �д´�. End


	// ���� ������ �о�´�. Start...
	//szKey.Format("CLR_%s", TRS_ST_BUY);			// �ż�
	szKey = COLORBUY;
	szTmp.Format("%ld", m_btnClrBuy.GetColor());
	::WritePrivateProfileString(STCONFIG_SIGNAL, szKey, szTmp, m_szCFGFile);

	//szKey.Format("CLR_%s", TRS_ST_EXITLONG);		// �ż�û��
	szKey = COLOREXITLONG;
	szTmp.Format("%ld", m_btnClrExitlong.GetColor());
	::WritePrivateProfileString(STCONFIG_SIGNAL, szKey, szTmp, m_szCFGFile);

	//szKey.Format("CLR_%s", TRS_ST_SELL);			// �ŵ�
	szKey = COLORSELL;
	szTmp.Format("%ld", m_btnClrSell.GetColor());
	::WritePrivateProfileString(STCONFIG_SIGNAL, szKey, szTmp, m_szCFGFile);

	//szKey.Format("CLR_%s", TRS_ST_EXITSHORT);	// �ŵ�û��
	szKey = COLOREXITSHORT;
	szTmp.Format("%ld", m_btnClrExitshort.GetColor());
	::WritePrivateProfileString(STCONFIG_SIGNAL, szKey, szTmp, m_szCFGFile);
	// ���� ������ �о�´�. End

	// ���� : $SRC/STController/STController.cpp
	//	dwSizeRead = GetPrivateProfileStringInST(STCONFIG_SIGNAL,SHOWSTCONFIG_SIGNALNAME,"0",szBuffer,sizeof(szBuffer));
	//	dwSizeRead = GetPrivateProfileStringInST(STCONFIG_SIGNAL,SHOWSIGNALQTY,"0",szBuffer,sizeof(szBuffer));

	return TRUE;
}


void CTabRunSignal::RemoveInfo()
{
}

void CTabRunSignal::SetDefault()
{
	m_cbSell.SetCurSel(0);
	m_cbBuy.SetCurSel(0);
	m_cbExitshort.SetCurSel(0);
	m_cbExitlong.SetCurSel(0);

	m_btnClrSell.SetColor(16711680);
	m_btnClrBuy.SetColor(255);
	m_btnClrExitshort.SetColor(255);
	m_btnClrExitlong.SetColor(16711680);

	SaveInfo();

	m_btnClrSell.Invalidate();
	m_btnClrBuy.Invalidate();
	m_btnClrExitshort.Invalidate();
	m_btnClrExitlong.Invalidate();
}

HBRUSH CTabRunSignal::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CTabRunSignal::OnBtClrBuy() 
{
	// TODO: Add your control notification handler code here
	OnBtnClr(m_btnClrBuy);
}

void CTabRunSignal::OnBtClrExitlong() 
{
	// TODO: Add your control notification handler code here
	OnBtnClr(m_btnClrExitlong);	
}

void CTabRunSignal::OnBtClrExitshort() 
{
	// TODO: Add your control notification handler code here
	OnBtnClr(m_btnClrExitshort);	
}

void CTabRunSignal::OnBtClrSell() 
{
	// TODO: Add your control notification handler code here
	OnBtnClr(m_btnClrSell);
}
void CTabRunSignal::OnBtnClr(CColorButton &rColorButton)
{
	COLORREF crColor = rColorButton.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		rColorButton.SetColor(color.GetColor());				
		rColorButton.Invalidate();
	}
}
