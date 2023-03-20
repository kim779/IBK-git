// GraphSetting.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "GraphSetting.h"

#include "BaseDialog.h"
// 2008.01.25 by LYH >>
//#include "DefineData.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"	// for _MTOL0()
// 2008.01.25 by LYH <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphSetting dialog

IMPLEMENT_DYNAMIC( CGraphSetting, CDialog)

/////////////////////////////////////////////////////////////////////////////
// CGraphSetting dialog
CGraphSetting::CGraphSetting( IChartOCX *p_pIChartOCX, BOOL p_bEnableWithDefaultInfo, CWnd* pParent /*=NULL*/)
	: CDialog(CGraphSetting::IDD, pParent),
	m_bHaveToDeleteMAGraph(false),
	m_bEnableDeleteAll(true)
{
	//{{AFX_DATA_INIT(CGraphSetting)
	m_bWithDefaultInfo = FALSE;
	//}}AFX_DATA_INIT

	// (2004.10.15, ��¿�) OCX Interface�� NULL�� ��츦 ����Ͽ�, Member�� �̸� NULL �ʱ�ȭ�� ó���Ѵ�.
	m_pIPacketManager		= NULL;
	m_pIChartManager		= NULL;
	m_pIGraphInfoManager	= NULL;
	m_pIAddInToolMgr		= NULL;
	m_pIChartOCX			= p_pIChartOCX;
	if( m_pIChartOCX)
	{
		m_pIChartOCX->AddRef();
		m_pIPacketManager		= m_pIChartOCX->GetIDefaultPacketManager();
		m_pIChartManager		= m_pIChartOCX->GetIChartManager();
		m_pIGraphInfoManager	= m_pIChartOCX->GetIGraphInfoManager();
		m_pIAddInToolMgr		= m_pIChartOCX->GetIAddInToolMgr();
	}

	m_eReturn = IDCANCEL;
	m_pIGraphInfo = NULL;
	
	//sy 2005.09.28.
	m_bVertLogScale = false;
	m_bVertInvertScale = false;
	m_bDoCalculate = false;
	
	// (2004.09.06, ��¿�) ���õ� Graph�� ��ǥ ������ �޴µ��� �ϴ� ��ɿ���,
	//		������ Check Button���� Default (User Default) Indicator Info�� �����ϵ��� ������ ��������
	//		�����޾� ó����� �Ѵ�.
	m_bEnableWithDefaultInfo = p_bEnableWithDefaultInfo;

	m_hOcxWnd = m_pIChartOCX->GetOcxHwnd();
}

CGraphSetting::~CGraphSetting()
{
	DeleteChildDlg();

	// (2004.10.08, ��¿�) Chart OCX Interface�� �����Ͽ�, ���� Ÿ Object�� ��ȸ�� �� �ֵ��� �Ѵ�.
	if( m_pIChartOCX) m_pIChartOCX->Release();

	// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
	if( m_pIPacketManager)
	{
		m_pIPacketManager->Release();
		m_pIPacketManager = NULL;
	}
	if( m_pIChartManager)
	{
		m_pIChartManager->Release();
		m_pIChartManager = NULL;
	}
	if( m_pIGraphInfoManager)
	{
		m_pIGraphInfoManager->Release();
		m_pIGraphInfoManager = NULL;
	}
	if( m_pIAddInToolMgr)
	{
		m_pIAddInToolMgr->Release();
		m_pIAddInToolMgr = NULL;
	}
	if( m_pIGraphInfo)
	{
		m_pIGraphInfo->Release();
		m_pIGraphInfo = NULL;
	}
}

void CGraphSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGraphSetting)
	DDX_Control(pDX, IDC_SUBTAB_ALL, m_tabIndicatorSetUpAll);
	DDX_Check(pDX, IDC_CHK_WITH_DEFAULT, m_bWithDefaultInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGraphSetting, CDialog)
	//{{AFX_MSG_MAP(CGraphSetting)
	ON_BN_CLICKED(IDC_ALL_DELETE, OnAllDelete)
	ON_BN_CLICKED(IDC_CHK_WITH_DEFAULT, OnChkWithDefault)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UDM_INDICATOR_GRAPH_INSERT, OnInsertOscillator)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphSetting message handlers

BOOL CGraphSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_tabIndicatorSetUpAll.Init( m_pIChartOCX, this);

	// 1. �ش� Indicator�� �´� ChildDlg ����.
	ILPCSTR szIndicatorName = m_pIGraphInfo->GetIndicatorNameString();
	if(m_pIGraphInfo->GetSubGraphCount() > 0) 
	{
// 2008.01.25 by LYH >>
		//SetWindowText( szIndicatorName);
		SetWindowText( _MTOL2(szIndicatorName) );
// 2008.01.25 by LYH <<
		
		// GraphSetting�� ���� �����Ǵ� ChildDlg�� MainBlock�����ʹ� NULL�� �����Ѵ�.
		m_MakeChildDlg.Initial( m_pIChartOCX, m_pIGraphInfo, this, m_pChildDlg);
		SetChildDlgPosition();
	}
	// (2007/5/4 - Seung-Won, Bae) Set Indicator Info Copy for Buffer.
	IGraphInfo *pIGraphInfo = m_pIGraphInfo->CreateCopyObject();
	m_pChildDlg->Initial( pIGraphInfo);
	if( pIGraphInfo) pIGraphInfo->Release();

	// 2. ������Ʈ�̰ų� ��ü������ư�� ��ġ ���� ��� ��Ȱ������.
// 2008.01.25 by LYH >>
	//if( !strcmp( szIndicatorName, "������Ʈ") || !m_bEnableDeleteAll)
	if( !strcmp( szIndicatorName, _MTEXT( C2_PRICE_CHART )) || !m_bEnableDeleteAll)
// 2008.01.25 by LYH <<
	{ 
		CButton* pbutton = (CButton* )GetDlgItem(IDC_ALL_DELETE);
		if( pbutton) pbutton->EnableWindow(FALSE);
	}	

	// (2004.09.06, ��¿�) ���õ� Graph�� ��ǥ ������ �޴µ��� �ϴ� ��ɿ���,
	//		������ Check Button���� Default (User Default) Indicator Info�� �����ϵ��� ������ ��������
	//		�����޾� ó����� �Ѵ�.
	if( m_bEnableWithDefaultInfo)
	{
//		GetDlgItem( IDC_CHK_WITH_DEFAULT)->ShowWindow(SW_SHOW);
		GetDlgItem( IDC_ALL_DELETE)->ShowWindow(SW_SHOW);
	}

	// (2006/12/10 - Seung-Won, Bae) Show Help in CGraphSetting Dlg.
	m_tabIndicatorSetUpAll.SetSubGraphName( ( const char *)szIndicatorName);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//InitDialog ���� ���� ����.
void CGraphSetting::SetData( IGraphInfo *p_pIGraphInfo, BOOL p_bLogScale, BOOL p_bReverseScale, BOOL p_bDelAllCheck)
{
	if( m_pIGraphInfo) m_pIGraphInfo->Release();
	m_pIGraphInfo = p_pIGraphInfo;
	if( m_pIGraphInfo) m_pIGraphInfo->AddRef();

	m_bVertLogScale = p_bLogScale;
	m_bVertInvertScale = p_bReverseScale;

	SetDeleteAllButtonState( p_bDelAllCheck);
}

void CGraphSetting::OnOK() 
{
	// 20081014 �ڵ��� >>
	//m_pChildDlg->OnApply(m_pIGraphInfo);
	BOOL bResult = m_pChildDlg->OnApply(m_pIGraphInfo);
	if(!bResult) return ;
	// 20081014 �ڵ��� <<

	UpdateData();
	if( m_bWithDefaultInfo == TRUE )
	{
		ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
		IGraphInfo *pIDefaultInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER_DEFAULT, szGraphName);
		m_pChildDlg->OnApply( pIDefaultInfo);
		m_pChildDlg->m_bOrgSaveDefault = TRUE;
		pIDefaultInfo->Release();
	}
	
	m_bDoCalculate = m_pChildDlg->DoCalculate();
	m_eReturn = IDOK;

	CDialog::OnOK();
}


void CGraphSetting::OnAllDelete() 
{
	m_eReturn = IDABORT;
	CDialog::OnOK();
}


void CGraphSetting::OnCancel() 
{
	m_eReturn = IDCANCEL;	
	
	CDialog::OnCancel();
}


//Ok, Cancel, Delete �� ����ڰ� ������ ���� ��´�.
int CGraphSetting::GetReturnValue() const
{
	return m_eReturn;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  DoCalculate
 - Created at  :  2005-01-26   15:09
 - Author      :  ������
 - Description :  ��ǥ�� ��꿩�θ� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
BOOL CGraphSetting::DoCalculate() const
{
	return m_bDoCalculate;
}

//�θ� �ȿ� �� ChildDlg�� ��ġ ���.
void CGraphSetting::SetChildDlgPosition()
{
	CRect rcCommon;
	CButton* pCommonBtn = (CButton* )m_tabIndicatorSetUpAll.GetSubTab(0);

	if( pCommonBtn == NULL )
		return;
	
	pCommonBtn->GetWindowRect(rcCommon);
	ScreenToClient(rcCommon);

	pCommonBtn->ShowWindow(SW_HIDE);

	// ( 2006 / 10 / 31 - Sang-Woo, Cho ) - ������ ���� Tab�� ������ ���ؼ� ����.
	if( m_tabIndicatorSetUpAll.GetCurSel() == 0 )
		m_pChildDlg->SetWindowPos( &wndTop, rcCommon.left, rcCommon.top, rcCommon.Width(), rcCommon.Height(), SWP_SHOWWINDOW);
	else
		m_pChildDlg->SetWindowPos( &wndTop, rcCommon.left, rcCommon.top, rcCommon.Width(), rcCommon.Height(), SWP_HIDEWINDOW);

}

void CGraphSetting::DeleteChildDlg()
{
	if( m_pChildDlg != NULL)
	{
		m_pChildDlg->DestroyWindow();
		delete m_pChildDlg;
		m_pChildDlg = NULL;
	}
}


// NOTE: ����(05/01/18) �̺κ��� ��� ó���ؾ����� ����Ұ�.
LRESULT CGraphSetting::OnInsertOscillator(WPARAM wParam, LPARAM lParam)
{
//	m_pGraphData->AddSubGraph("Oscillator", m_pIGraphInfoManager);
	return 0L;
}



/*-----------------------------------------------------------------------------------------
 - Function    :  SetDeleteAllButtonState
 - Created at  :  2005-01-26   09:23
 - Analysis    :  (������) '��ü����'��ư ���¸� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CGraphSetting::SetDeleteAllButtonState(const BOOL enableDeleteAll)
{
	m_bEnableDeleteAll = enableDeleteAll;
}


void CGraphSetting::SetLogState(BOOL bLogState)
{
	//sy 2005.09.28.
	m_bVertLogScale = bLogState;
}

BOOL CGraphSetting::GetLogState() const
{
	//sy 2005.09.28.
	return m_bVertLogScale;
}

void CGraphSetting::SetInvertScaleType(BOOL bInvertScaleType)
{
	//sy 2005.09.28.
	m_bVertInvertScale = bInvertScaleType;
}

BOOL CGraphSetting::GetInvertScaleType() const
{
	//sy 2005.09.28.
	return m_bVertInvertScale;
}

void CGraphSetting::OnChkWithDefault() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CGraphSetting::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			OnOK();
			return TRUE;
		}	
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CGraphSetting::ShowChildDialog(BOOL bShow)
{
	// 1. ChildDialog�� List�� ������ ����������. 
	if( !m_pChildDlg) return;
	
	// 2. m_plstChildDlg�� loop�� ���鼭 Child Dialog�� Show / Hide�� �Ѵ�.
	m_pChildDlg->ShowWindow( bShow ? SW_SHOW : SW_HIDE);
}

