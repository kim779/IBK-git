// DlgMainTabIndicatorSetUp.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "DlgMainTabIndicatorSetUp.h"

#include <MMSystem.h>						// for timeGetTime()

#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTOL0()	// 2008.01.25 by LYH >>
#include "../Include_Chart/OutsideCommonInterfaceFunction.h"	// for COutsideCommonInterfaceFunction
#include "../Include_Chart/GraphData.h"

#include "BaseDialog.h"
#include "BaseData.h"
#include "DlgEnvSetUpAll.h"
#include "IndicatorNameListCtrl.h"
#include "IndicatorGroupButton.h"			// for CIndicatorGroupButton


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMainTabIndicatorSetUp dialog

IMPLEMENT_DYNAMIC( CDlgMainTabIndicatorSetUp, CBaseTabDlg)

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 11
// Return void  : 
// Param  IChartOCX *p_pIChartOCX : 
// Param  CMainBlock *pMainBlock : 
// Param  CIndicatorList *pIndicatorList : 
// Param  CWnd *pParent : 
// Comments		: 
//-----------------------------------------------------------------------------
CDlgMainTabIndicatorSetUp::CDlgMainTabIndicatorSetUp( IChartOCX *p_pIChartOCX, CWnd *pParent )
	: CBaseTabDlg(CDlgMainTabIndicatorSetUp::IDD, pParent), 
	m_bIsOnlyViewData(false)
//	: CDialog(CDlgMainTabIndicatorSetUp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMainTabIndicatorSetUp)
	m_editIndicatorExp = _T("");
	m_bSaveDefault = FALSE;
	//}}AFX_DATA_INIT

	// 1. Member �������� �ʱ�ȭ �Ѵ�.
	m_pIChartOCX			= p_pIChartOCX;
	m_pIChartManager		= NULL;
	m_pIGraphInfoManager	= NULL;
	if( m_pIChartOCX)
	{
		m_pIChartOCX->AddRef();
		m_pIChartManager		= m_pIChartOCX->GetIChartManager();
		m_pIGraphInfoManager	= m_pIChartOCX->GetIGraphInfoManager();
	}

	m_pChildDlg = NULL;
	m_nInitChartIndex = -1;

	m_pftOldStaticFont = NULL;

	m_bPreview = FALSE;
	m_bSave = FALSE;

	m_bDlgMode = TRUE;

	m_pIndicatorNameList	= NULL;
	m_pOrgIndicatorNameList	= NULL;
	m_pIPacketManager = NULL;

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	m_dwLastClickedTime = 0;

// 2008.01.25 by LYH >>
	m_hOcxWnd = NULL;
// 2008.01.25 by LYH <<
}
CDlgMainTabIndicatorSetUp::~CDlgMainTabIndicatorSetUp()   // standard destructor
{
	if( m_pIChartOCX)
	{
		m_pIChartOCX->Release();
		m_pIChartOCX = NULL;
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
	if( m_pIndicatorNameList)
	{
		m_pIndicatorNameList->Release();
		m_pIndicatorNameList = NULL;
	}
	if( m_pOrgIndicatorNameList)
	{
		m_pOrgIndicatorNameList->Release();
		m_pOrgIndicatorNameList = NULL;
	}
	if( m_pIPacketManager)
	{
		m_pIPacketManager->Release();
		m_pIPacketManager = NULL;
	}
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 11
// Return void : 
// Param  CDataExchange* pDX : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMainTabIndicatorSetUp)
	DDX_Control(pDX, IDC_SUBTAB_ALL, m_tabIndicatorSetUpAll);
	DDX_Check(pDX, IDC_CHK_WITH_DEFAULT, m_bSaveDefault);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMainTabIndicatorSetUp, CDialog)
	//{{AFX_MSG_MAP(CDlgMainTabIndicatorSetUp)
	ON_BN_CLICKED(IDC_INITILIZE_ALLINDICATOR, OnInitilizeAllindicator)
	ON_BN_CLICKED(IDC_DEFAULT_INDICATORDATA, OnDefaultIndicatordata)
	ON_BN_CLICKED(IDC_CHK_WITH_DEFAULT, OnChkWithDefault)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_BTN_RECOVER, OnBtnRecover)
	ON_BN_CLICKED(IDC_BTN_PREVIEW, OnBtnPreview)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_ADD_ALL, OnBtnAddAll)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDC_BTN_DELETE_ALL, OnBtnDeleteAll)
	ON_BN_CLICKED(IDC_BTN_WITH_DEFAULT, OnBtnWithDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMainTabIndicatorSetUp message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 9
// Modifier		: 
// Comments		: ��ǥ ���� TabPage�� �ʱ�ȭ �Ѵ�.
//-----------------------------------------------------------------------------
BOOL CDlgMainTabIndicatorSetUp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 1. Dialog�� ����ư���� ��Ŀ���� �ֱ� ���� ���
	mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
	mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );

	if(m_pIChartOCX == NULL)
		return FALSE;

	// 2. ���� Dialog���� ��ǥ�� �ٸ� font�� ����ϱ� ���� ��Ʈ�� �ٲپ��ش�.
	CFont *pFont = NULL;
	pFont = this->GetFont();
	if( pFont )
	{
		LOGFONT lfIndicatorName;
		pFont->GetLogFont( &lfIndicatorName );

		lfIndicatorName.lfWeight = FW_BOLD;
		lfIndicatorName.lfHeight = lfIndicatorName.lfHeight - 1;
		m_pftGroupNameFont.CreateFontIndirect( &lfIndicatorName );
	}

	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_STATIC_INDICATOR_NAME);
	m_pftOldStaticFont = pStatic->GetFont();
	pStatic->SetFont( &m_pftGroupNameFont );

	// 3. ���� ��Ʈ ȭ���� �����ϰ� �ʱ��ε����� ������ �ִ´�.
	// ���ϵ� Index�� '���'�۾����Ҷ� �ʿ��� Index�� ��ǥ����â �������� ��ǥ���ð����� ���ư��� ����.
	m_nInitChartIndex = m_pIChartOCX->SaveChartCFG_SaveNow();

	// 4. ���� ��Ʈ�� �������� ��ǥ�� �̸� ��������.
	if( m_pIndicatorNameList)
	{
		m_pIndicatorNameList->Release();
		m_pIndicatorNameList = NULL;
	}
	if( m_pIChartManager) m_pIndicatorNameList		= m_pIChartManager->GetAllIndicatorNameStringList();
	if( m_pOrgIndicatorNameList)
	{
		m_pOrgIndicatorNameList->Release();
		m_pOrgIndicatorNameList = NULL;
	}
	if( m_pIChartManager) m_pOrgIndicatorNameList	= m_pIChartManager->GetAllIndicatorNameStringList();

	// 5. �ʱ� IndicatorInfo�� �����صα�(���߿� ��ҽÿ� �ʿ���)
	SetOrgViewData();
	
	// 6. SubTab�� ���� ��Ų��.
	m_tabIndicatorSetUpAll.Init( m_pIChartOCX, this);

	// 7. ListCtrl�� GroupButton������ ��ġ����.
	SetIndicatorSize();
	m_ctrlGroupButton.SetData( this );

	// ( 2007 / 01 / 08 - Sang-Woo, Cho ) 
	m_ctrlUseGroupButton.SetData( this );

	if( m_pIPacketManager) m_pIPacketManager->Release();
	m_pIPacketManager = m_pIChartOCX->GetIDefaultPacketManager();
	
	// ( 2007 / 01 / 08 - Sang-Woo, Cho ) - �����ǥ ������ �����Լ��� �и�
	m_ctrlGroupButton.CreateGeneralCtrl( m_pIndicatorNameList, m_pIChartOCX );
	m_ctrlGroupButton.SetGroupButtonCtrlType( GENERAL_GROUP );
	m_ctrlGroupButton.SetCheck( m_pIndicatorNameList );
	m_ctrlUseGroupButton.SetGroupButtonCtrlType( USE_GROUP );
	IStringList *pIGraphNameList = m_pIChartManager->GetAllGraphNameStringList();
	if( pIGraphNameList)
	{
		m_ctrlUseGroupButton.CreateUseCtrl( pIGraphNameList, m_pIChartOCX );
		pIGraphNameList->Release();
	}
	m_ctrlUseGroupButton.SetCheck( m_pIndicatorNameList );

	// Sang-Woo, Cho		2006 / 11 / 06
	// ���� �̵���հ� �ŷ��� �̵������ ���� Dialog�� control���� ���Ƽ� �������� �����ϸ� 
	// ���������� �� ��ǥ�� ���� �����صд�.
	// Begin
// 2008.01.25 by LYH >>
	//SelChangeChildDlg( "���� �̵����", "");
	//SelChangeChildDlg( "�ŷ��� �̵����", "");
	SelChangeChildDlg( _MTEXT(C2_PRICE_MA), "");
	SelChangeChildDlg( _MTEXT(C2_VOLUME_MA), "");
// 2008.01.25 by LYH <<
	// End

	//SelChangeChildDlg( m_ctrlGroupButton.GetFirstCheckItem(), TRUE);
	//m_ctrlGroupButton.SetFocusItem();
	CString strIndicatorName, strGraphName;
	m_ctrlUseGroupButton.GetFirstCheckItem( strIndicatorName, strGraphName);
	SelChangeChildDlg( strIndicatorName, strGraphName);
	m_ctrlUseGroupButton.SetFocusItem();
	CIndicatorGroupButton *pIndicatorGroupButton = m_ctrlUseGroupButton.GetSelectGroupButton();
	if( pIndicatorGroupButton) pIndicatorGroupButton->SetState( FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void	: 
// Param  const CString &strIndicatorName : 
// Comments		: GroupButton�� �����Ҷ� ȣ���Ѵ�.
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::SelChangeGroupButton( const CString &strIndicatorName, const CString &strGraphName)
{
	// Ư����ǥ���� �Ϲ���ǥ�� ��ȯ�Ұ�쿡 ��ǥ�� üũ���¸� �ٽ� ��ġ���Ѿ��Ѵ�.
	SyncItemCheck();

	SelChangeChildDlg( strIndicatorName, strGraphName);
} 


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void	: 
// Param  const CString &strIndicatorName : 
// Comments		: ��ǥ�� üũ�Ͽ� �߰��Ѵ�.
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::AddGraph( const CString &strIndicatorName, CString &p_strGraphName)
{
	p_strGraphName.Empty();
	// �ش� ��ǥ�� ����Dlg�� �����´�.
	CBaseDialog *pChildDialog = m_BaseDialogContainer.GetBaseDialog( strIndicatorName);

	ILPCSTR szGraphName = m_pIChartOCX->AddChartBlock( strIndicatorName);
	p_strGraphName = szGraphName;
	
	// Info���� ������ UserIndicatorInfo�� �����´�.
	IGraphInfo *pUserIndicatorInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, szGraphName);
	if( pUserIndicatorInfo == NULL) return;
	
	// Dialog ������Ʈ�� ���ٸ� ���� �����Ͽ� Dialog����Ŭ������ Insert�Ѵ�.
	if( pChildDialog == NULL)
	{
		ML_SET_LANGUAGE_RES();
		m_MakeChildDlg.Initial( m_pIChartOCX, pUserIndicatorInfo, this, pChildDialog, m_pParentWnd);
		//m_pChildDlg->GetTail()
		m_BaseDialogContainer.AddBaseDialog( ( LPCTSTR)strIndicatorName, pChildDialog);
	}
	pChildDialog->Initial( pUserIndicatorInfo);
	pChildDialog->RedrawWindow();

	//sy test - bTheBeginning ���� �����ϱ�!!
	// ���� ����Ǿ��� �����ִ�. OrgViewData�� Info�� Insert �Ѵ�. (����ҽÿ� �ʿ��� ���̴�.)
	m_OrgViewData.AddTail(pUserIndicatorInfo);
	pUserIndicatorInfo->Release();

	// chartȭ�鿡 �ٷ� ������ �ǹǷ� �̸����⸦ �ѰͰ� ���� �����̴�.
	m_bPreview = TRUE;

	// OCX�� AddChartȣ��.
	SyncItemCheck();

// (2007/1/24 - Seung-Won, Bae) Why call ? 
//	ApplyGraph( strIndicatorName);
}


//sy 2006.04.24.
void CDlgMainTabIndicatorSetUp::SetOrgViewData()
{
	m_OrgViewData.Initialize();

	IStringList *pIniNameList = m_pIChartOCX->CreateObjectIStringList();
	if(!GetGraphNameList(m_bIsOnlyViewData, pIniNameList))
	{
		pIniNameList->Release();
		return;
	}

	POSITION pos = pIniNameList->GetHeadPosition();
	while(pos)
	{
		ILPCSTR szIndicatorName = NULL;
		pIniNameList->GetNext( pos, szIndicatorName);
		IGraphInfo *pOrgIndicatorInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, szIndicatorName);	
		if(pOrgIndicatorInfo == NULL)
			continue;
 
		m_OrgViewData.AddTail(pOrgIndicatorInfo, true);
		pOrgIndicatorInfo->Release();
	}
	pIniNameList->Release();
}

bool CDlgMainTabIndicatorSetUp::GetGraphNameList(const bool bIsOnlyViewData, IStringList * pIndicatorNameList)
{
	if(m_pIGraphInfoManager == NULL)
		return false;

	pIndicatorNameList->RemoveAll();
	
	// ȭ�鿡 �����ϴ� ��ǥ��
	bool bResult = GetGraphNameList(m_pIndicatorNameList, pIndicatorNameList);
	if(bIsOnlyViewData)
		return bResult;

	// userList �� �����ϴ� ��ǥ��
	IStringList *pIUserIndiNameList = m_pIGraphInfoManager->GetIndicatorNameStringList( IGraphInfoManager::IEILT_USER, IGraphInfoManager::IEIIT_GENERAL);
	if( pIUserIndiNameList) if( GetGraphNameList(pIUserIndiNameList, pIndicatorNameList)) bResult = true;
	pIUserIndiNameList->RemoveAll();
	pIUserIndiNameList->Release();
	pIUserIndiNameList = m_pIGraphInfoManager->GetIndicatorNameStringList( IGraphInfoManager::IEILT_USER, IGraphInfoManager::IEIIT_SPECIAL);
	if( pIUserIndiNameList)
	{
		if(GetGraphNameList(pIUserIndiNameList, pIndicatorNameList))
			bResult = true;
	}
	pIUserIndiNameList->Release();

	return bResult;
}

bool CDlgMainTabIndicatorSetUp::GetGraphNameList(IStringList * pISourceIndiNameList, IStringList * targetIndiNameList)
{
	if( !pISourceIndiNameList) return false;
	if( pISourceIndiNameList->GetCount() <= 0) return false;

	ILPCSTR szGraphName = NULL;
	POSITION pos = pISourceIndiNameList->GetHeadPosition();
	while(pos != NULL)
	{
		pISourceIndiNameList->GetNext( pos, szGraphName);
		if(szGraphName.IsEmpty())
			continue;
		
		if( !IsExistenceIndicatorName( targetIndiNameList, szGraphName)) targetIndiNameList->AddTail(szGraphName);
	}
	
	return (targetIndiNameList->GetCount() > 0);
}

bool CDlgMainTabIndicatorSetUp::IsExistenceIndicatorName(IStringList * pIndicatorNameList, const char *p_szGraphName) const
{
	if( !p_szGraphName) return false;
	if( !*p_szGraphName) return false;

	ILPCSTR szGraphName = NULL;
	POSITION pos = pIndicatorNameList->GetHeadPosition();
	while(pos != NULL)
	{
		pIndicatorNameList->GetNext(pos, szGraphName);
		if( !strcmp( szGraphName, p_szGraphName)) return true;
	}
	return false;
}
//sy end

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 11
// Return void	: 
// Comments		: ����Ʈ��Ʈ���� ��ġ����
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::SetIndicatorSize()
{
	// 1. IDC_BTN_TOP_INDICATOR�� ��ġ�� ���ͼ� ListCtrl�� ��ġ�� ���Ѵ�.
	CButton *pButton = (CButton *)GetDlgItem( IDC_BTN_TOP_INDICATOR );
	CButton *pUseButton = (CButton *)GetDlgItem( IDC_BTN_USE_INDICATOR );
	CButton *pAddButton = (CButton *)GetDlgItem( IDC_BTN_ADD );
	if( pButton == NULL || pUseButton == NULL || pAddButton == NULL )
		return;

	CRect rcBtnRect, rcUseBtnRect, rcAddBtn;
	pButton->GetWindowRect( rcBtnRect );  
	ScreenToClient( rcBtnRect );

	pUseButton->GetWindowRect( rcUseBtnRect );
	ScreenToClient( rcUseBtnRect );

	pAddButton->GetWindowRect( rcAddBtn );
	ScreenToClient( rcAddBtn );

	// ( 2007 / 01 / 08 - Sang-Woo, Cho ) ����ũ�� ����
	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_STATIC_INDICATOR_INFO);
	CRect rcStatic;
	pStatic->GetWindowRect(rcStatic);
	ScreenToClient( rcStatic );

	// 3. ��ǥ�� ǥ���� ������ �����Ѵ�. ���� �������� ���̸� �ݿ��Ѵ�.
	CRect rcIndicatorRect, rcUseIndicatorRect;
	rcIndicatorRect.left = rcBtnRect.left;
	rcIndicatorRect.top = rcBtnRect.bottom + 1;
	rcIndicatorRect.right = rcBtnRect.right;
	rcIndicatorRect.bottom = rcStatic.bottom;

	rcUseIndicatorRect.left = rcUseBtnRect.left;
	rcUseIndicatorRect.top = rcUseBtnRect.bottom + 1;
	rcUseIndicatorRect.right = rcUseBtnRect.right;
	rcUseIndicatorRect.bottom = rcAddBtn.top - 3;	
		
	// 4. GoupButton�� ������ �����Ѵ�.
	m_ctrlGroupButton.SetIndicatorSize(rcIndicatorRect);
	m_ctrlUseGroupButton.SetIndicatorSize(rcUseIndicatorRect);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void	: 
// Param  const CString &strIndicatorName : 
// Param  const int nCheck : 
// Comments		: ��ǥ�� �����Ҷ� Dialog�� �����ͼ�(or �����ؼ�) �����ִ� �Լ�.
//				  ��ǥ�� �����Ҷ� CBaseDialogContainer�� �ش� Dialog������Ʈ�� �ִ��� Ȯ���Ѵ�. 
//				  �ִٸ� Get���� ��������, ���ٸ� �� Dialog������Ʈ�� �����ؼ� 
//				  CBaseDialogContainerŬ������ Insert�Ѵ�.
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::SelChangeChildDlg( const CString &strIndicatorName, const CString &strGraphName)
{
	ML_SET_LANGUAGE_RES();

	// 1. ���õǴ� ��ǥ�� �ٲ�� SubTab�� ����� ��ǥ���� �����Ѵ�.
	m_tabIndicatorSetUpAll.SetSubGraphName( strIndicatorName );

	// 2. ���� SubTab�� "����" Tab �̸� ��ǥ ���� ������ �о���δ�.
	if( m_tabIndicatorSetUpAll.IsWindowVisible() && m_tabIndicatorSetUpAll.GetCurSel() == 1 )
		m_tabIndicatorSetUpAll.ReadIndicatorExpFile();

	m_strIndicatorName = strIndicatorName;

	IGraphInfo* pIGraphInfo = NULL;
	if( strGraphName.IsEmpty())	pIGraphInfo = m_pIGraphInfoManager->GetIndicatorInfo(	IGraphInfoManager::IEILT_USER_DEFAULT,	strIndicatorName, IGraphInfoManager::IEIIGT_WITH_DEFAULT_GET_AND_SET);
	else						pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo(		IGraphInfoManager::IEILT_USER,			strGraphName);
	if( !pIGraphInfo) return;

	// "����" Tab�϶����� ChildDlg�� Hide �Ѵ�.
	if( m_tabIndicatorSetUpAll.GetCurSel() == 0 )
		ShowChildDialog(FALSE);

	// Dialog����Ŭ������ dialog�� �ִٸ� �����´�.
 	m_pChildDlg = m_BaseDialogContainer.GetBaseDialog((LPCTSTR)m_strIndicatorName);

	// Dialog ������Ʈ�� ���ٸ� ���� �����Ͽ� Dialog����Ŭ������ Insert�Ѵ�.
	if(m_pChildDlg == NULL)
	{
		m_MakeChildDlg.Initial( m_pIChartOCX, pIGraphInfo, this, m_pChildDlg, m_pParentWnd);
		//m_pChildDlg->GetTail()
		m_BaseDialogContainer.AddBaseDialog((LPCTSTR)m_strIndicatorName, m_pChildDlg);
	}
	m_pChildDlg->Initial( pIGraphInfo);
	pIGraphInfo->Release();

	if( m_pChildDlg) m_bSaveDefault = m_pChildDlg->m_bSaveDefault;
	UpdateData(FALSE);

	// ChildDlg ��ġ ����
	SetChildDlgPosition();

	// "����" Tab�϶����� ������ ChildDlg�� Show �Ѵ�.
	if( m_tabIndicatorSetUpAll.GetCurSel() == 0 )
		ShowChildDialog(TRUE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void	: 
// Param  BOOL bShow : bShow - TRUE: Show, FALSE: Hide
// Comments		: ChildDialog�� Show / Hide��Ŵ.
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::ShowChildDialog(BOOL bShow)
{
	// 1. ChildDialog�� List�� ������ ����������. 
	if( !m_pChildDlg) return;
	
	// 2. m_pChildDlg�� loop�� ���鼭 Child Dialog�� Show / Hide�� �Ѵ�.
	m_pChildDlg->ShowWindow( bShow ? SW_SHOW : SW_HIDE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void	: 
// Comments		: ChildDlg�� ��ġ�� �����Ѵ�.
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::SetChildDlgPosition()
{
	CRect rcCommon;
	CButton* pCommonBtn = (CButton* )m_tabIndicatorSetUpAll.GetSubTab(0);

	if( pCommonBtn == NULL || !m_pChildDlg) return;

	pCommonBtn->GetWindowRect(rcCommon);
	ScreenToClient(rcCommon);

	pCommonBtn->ShowWindow(SW_HIDE);

	// ( 2006 / 10 / 31 - Sang-Woo, Cho ) - "����"�� "����" Tab�� ������ ���ؼ� ����.
	if( m_tabIndicatorSetUpAll.GetCurSel() == 0 )
		m_pChildDlg->SetWindowPos( &wndTop, rcCommon.left, rcCommon.top, rcCommon.Width(), rcCommon.Height(), SWP_SHOWWINDOW);
	else
		m_pChildDlg->SetWindowPos( &wndTop, rcCommon.left, rcCommon.top, rcCommon.Width(), rcCommon.Height(), SWP_HIDEWINDOW);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 3
// Return void : 
// Param  const int nCheck : 
// Param  const CString &strIndicatorName : 
// Comments		: ListCtrl���� �ٸ� ��ǥ���� �������� ���. (���õ� ��ǥ�� ChildDlg�� �����Ѵ�.)
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::SelChangeGraph( const CString &strIndicatorName, const CString &strGraphName)
{
	SelChangeChildDlg( strIndicatorName, strGraphName);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 23
// Return void : 
// Comments		: ListCtrl�� ������üũ���¸� Chart�� ��ġ��Ų��. (��ǥ �߰��Ҷ� 
//				  �߰��� Block���� �Ѱ��� ��츦 �����)
//				  ���� ������� ��ǥ���� ���� List�� ���� ����� MainBlock���� �ٽ� �������
//				  ��ǥ����Ʈ�� �����´�. ListCtrl�� ��� üũ���¸� �����, ��ǥ��List��
//				  üũ���¸� �ٽ� �����Ѵ�.
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::SyncItemCheck()
{
	if( m_pIndicatorNameList)
	{
		m_pIndicatorNameList->RemoveAll();
		m_pIndicatorNameList->Release();
		m_pIndicatorNameList = NULL;
	}
	m_pIndicatorNameList =  m_pIChartManager->GetAllIndicatorNameStringList();

	// ListCtrl�� �ٽ� üũ�Ѵ�.
	m_ctrlGroupButton.SetAllUnCheck();
	m_ctrlGroupButton.SetCheck(m_pIndicatorNameList);
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return void : 
// Param  void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::InitializeSelectedIndicator( void )
{
	IGraphInfo* pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, m_strIndicatorName);
	if(pIGraphInfo == NULL || !m_pChildDlg) return;
	
	m_pChildDlg->Initial(pIGraphInfo);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 4
// Modifier		: 
// Comments		: ��ü ��ǥ�� �ʱ�ȭ �Ѵ�.
//-----------------------------------------------------------------------------
//sy 2006.04.24. -> ��ü��ǥ �ʱ�ȭ
// -> �ʱ�ȭ �Ǹ� ���� �ɼ��� ���� ������ ȭ�鿡 �ٷ� �����ϱ�
void CDlgMainTabIndicatorSetUp::OnInitilizeAllindicator( void ) 
{
	if(!IsActionInitilizeAllindicator())
		return;

	// ����� ��ǥ�������� ��� ����.
	m_pIGraphInfoManager->RemoveAllIndicatorInfo( IGraphInfoManager::IEILT_USER);
	m_pIChartManager->Initialize_GraphInfo(false);

	POSITION pos = m_BaseDialogContainer.GetStartPosition();
	while(pos != NULL)
	{
		CBaseDialog*pChildDialog = m_BaseDialogContainer.GetNextAssoc(pos);
		if( pChildDialog == NULL) continue;

		Initilizeindicator( pChildDialog);
	}

	m_bPreview = TRUE;

	m_pIChartOCX->InvalidateControl();
}

bool CDlgMainTabIndicatorSetUp::IsActionInitilizeAllindicator()
{
	if(m_pIChartOCX == NULL || m_pIGraphInfoManager == NULL || m_pIChartManager == NULL)
		return false;

	ML_SET_LANGUAGE_RES();
	CString strMsgInit, strMsgInitTitle;
	strMsgInit.LoadString(IDS_MSG_INIT);
	strMsgInitTitle.LoadString(IDS_MSG_INIT_TITLE);
	CString strMessage(strMsgInit);
	int nOk = MessageBox(strMessage, strMsgInitTitle, MB_OKCANCEL | MB_ICONQUESTION);
	return (nOk == IDOK);
}

bool CDlgMainTabIndicatorSetUp::Initilizeindicator(CBaseDialog* pBaseDlg)
{
	if(pBaseDlg == NULL)
		return false;

	// "�⺻������ ����" �ɼ��� system default ������ ������� �ʴ´�.
	m_bSaveDefault = pBaseDlg->m_bSaveDefault = pBaseDlg->m_bOrgSaveDefault;

	// Dlg �ʱ�ȭ
	CString strIndicatorName = pBaseDlg->GetIndicatorName();
	IGraphInfo* pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, strIndicatorName);
	if(pIGraphInfo == NULL)
		return false;

	// subGraph title�� ���ǰ��� �ش�Ǵ� ��쿡 
	pIGraphInfo->ChangeAllSubGraphTitleFromCalConditionData();
	pBaseDlg->Initial(pIGraphInfo);
	pIGraphInfo->Release();

	// defaulte ���� ȭ�鿡 �ٷ� �ݿ������ֱ� (ȭ���, ���ǰ��� ���� ��� �ٽ���)
	m_pIChartManager->ChangeGraphDataFromGraphInfo(strIndicatorName, true);
	return true;
}
//sy end

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 4
// Return void	: 
// Comments		: ���� ������ ������� ���õ� ��ǥ�� ���� ����Ʈ ������ ������
//				  -> �����ɼ��� �ֱ� ������ ȭ�鿡 �ٷ� ��������� �ʴ´�.
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnDefaultIndicatordata() 
{
	//sy 2006.04.24 -> ������ǥ �ʱ�ȭ
	if(m_pChildDlg == NULL)
		return;

	CBaseDialog* pHeadDlg = m_pChildDlg;
	if(pHeadDlg == NULL || pHeadDlg->GetIndicatorName() != m_strIndicatorName)
		return;

	IGraphInfo* pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER_DEFAULT, m_strIndicatorName);
	if(pIGraphInfo == NULL)
		return;

	pHeadDlg->Initial(pIGraphInfo);
	pIGraphInfo->Release();

	// defaulte ���� ȭ�鿡 �ٷ� �ݿ������ֱ� (ȭ���, ���ǰ��� ���� ��� �ٽ���)
	//ApplyGraph(m_strIndicatorName);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 5
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnChkWithDefault() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	//IGraphInfo* pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo(m_strIndicatorName);
	IGraphInfo* pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER_DEFAULT, m_strIndicatorName);
	if( pIGraphInfo == NULL || !m_pChildDlg) return;
	
	m_pChildDlg->m_bSaveDefault = m_bSaveDefault;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 30
// Return void	: 
// Comments		: ��ü ��ǥ ���� ������ �����Ѵ�.
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnBtnSave() 
{
	m_bSave = TRUE;

	POSITION posDialog = m_BaseDialogContainer.GetStartPosition();
	CBaseDialog *pChildDialog = NULL;
	while( posDialog)
	{
		pChildDialog = m_BaseDialogContainer.GetNextAssoc( posDialog);

		// ������ Data�� UserIndicatorInfo�� �����ؾ��Ѵ�.
		IGraphInfo *pIGraphInfo = pChildDialog->GetGraphInfo();
		if( !pIGraphInfo) continue; 

		pChildDialog->OnApply( pIGraphInfo);

		// ��ǥ�� ���
		if( pChildDialog->DoCalculate())
		{
			// �׹���Ʈ�� ��Ŷ������ ������ �� �����Ƿ� �׻� ��Ŷ �ʱ�ȭ�� �����ش�.
			ILPCSTR szIndicatorName	= pIGraphInfo->GetIndicatorNameString();
			if( !strcmp( szIndicatorName, _MTEXT( C2_RAINBOW_CHART )))
				m_pIChartManager->Initialize_AllPackets( _MTEXT( C2_RAINBOW_CHART ), true);
		}

		if( pIGraphInfo) pIGraphInfo->Release();
	}


	// (2007/1/23 - Seung-Won, Bae) Do not update for each graph.
	//		Because, some GraphInfo does not have BaseDialog.
	m_pIChartManager->ChangeAllGraphDataFromGraphInfo( true);

	// �ٽ� �׷���
	m_pIChartOCX->InvalidateControl();
	m_pOrgIndicatorNameList->RemoveAll();

	m_bPreview = FALSE;
	m_bSave = FALSE;
}



// "���" ��ư - ��� ������ ȭ�� �ε� ���·� ���� �� ȭ�� �ݱ�
void CDlgMainTabIndicatorSetUp::OnBtnRecover() 
{
	m_OrgViewData.ChangeAllIndicatorList( m_pIGraphInfoManager);

	// ��Ʈȭ���� �ʱ�ȭ������ �����Ѵ�.
	RestoreChartCfg(m_nInitChartIndex);
}

//sy 2006.04.20.
void CDlgMainTabIndicatorSetUp::RestoreChartCfg(const int nInitChartIndex)
{
	if(m_pIChartOCX == NULL || nInitChartIndex < 0)
		return;

	// ��Ʈȭ���� �ʱ�ȭ������ �����Ѵ�.
	m_pIChartOCX->SaveChartCFG_RestoreChartOfIndex(nInitChartIndex);
}

int CDlgMainTabIndicatorSetUp::GetInitChartIndex() const
{
	return m_nInitChartIndex;
}
//sy end

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 12
// Return void : 
// Comments		: �̸�����
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnBtnPreview() 
{
	// TODO: Add your control notification handler code here
	m_bPreview = TRUE;

	POSITION posDialog = m_BaseDialogContainer.GetStartPosition();
	CBaseDialog *pChildDialog = NULL;
	while( posDialog)
	{
		pChildDialog = m_BaseDialogContainer.GetNextAssoc( posDialog);

		// ������ Data�� UserIndicatorInfo�� �����ؾ��Ѵ�.
		IGraphInfo *pIGraphInfo = pChildDialog->GetGraphInfo();
		if( !pIGraphInfo) continue;

		pChildDialog->OnApply( pIGraphInfo);
		// ��ǥ�� ���
		if( pChildDialog->DoCalculate())
		{
			// �׹���Ʈ�� ��Ŷ������ ������ �� �����Ƿ� �׻� ��Ŷ �ʱ�ȭ�� �����ش�.
			ILPCSTR szIndicatorName	= pIGraphInfo->GetIndicatorNameString();
			if( !strcmp( szIndicatorName, _MTEXT( C2_RAINBOW_CHART )))
				m_pIChartManager->Initialize_AllPackets( _MTEXT( C2_RAINBOW_CHART ), true);
		}

		if( pIGraphInfo) pIGraphInfo->Release();
	}


	// (2007/1/23 - Seung-Won, Bae) Do not update for each graph.
	//		Because, some GraphInfo does not have BaseDialog.
	m_pIChartManager->ChangeAllGraphDataFromGraphInfo( true);

	// �ٽ� �׷���
	m_pIChartOCX->InvalidateControl();
}


BOOL CDlgMainTabIndicatorSetUp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			//OnBtnSave();
			OK();
		}
		
		if( pMsg->wParam == VK_ESCAPE )
		{
			Cancel();
		}
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgMainTabIndicatorSetUp::OK()
{
	if( m_pParentWnd != NULL )
		( (CDlgEnvSetUpAll *)m_pParentWnd)->OK();
}

void CDlgMainTabIndicatorSetUp::Cancel()
{
	if( m_pParentWnd != NULL )
		( (CDlgEnvSetUpAll *)m_pParentWnd)->Cancel();
}



BOOL CDlgMainTabIndicatorSetUp::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_STATIC_INDICATOR_NAME);

	if( m_pftOldStaticFont != NULL )
		pStatic->SetFont(m_pftOldStaticFont);

	return CDialog::DestroyWindow();
}


//-----------------------------------------------------------------------------
// Writer		: Sang-Woo, Cho		Date : 2005 / 10 / 11
// Return  void : 
// Param  CBaseTabDlg *pDlgTabPage   : 
// Explain		: Tab�� �ٲ������ �����ؾ� �Ǵ� ������ ������ ȣ��ȴ�.
//-----------------------------------------------------------------------------
//sy 2005.10.12
bool CDlgMainTabIndicatorSetUp::ChangeTabPage( CBaseTabDlg *pSourceDlgTabPage )
{
	int nDrawingCurPrice = 0;
	if(!GetDrawingCurPrice(pSourceDlgTabPage, nDrawingCurPrice))
		return false;

	IGraphInfo* pIGraphInfo = GetIndicatorInfoInBaseDlg(_MTEXT( C2_PRICE_CHART ));
	if(pIGraphInfo == NULL)
		return false;

	int nCurData = -1;
	BOOL bLowHigh = FALSE, bLowCur = FALSE, bHighCur = FALSE, bPrevCur = FALSE, bShowOHLC = FALSE;
	pIGraphInfo->GetGraphDataDrawingData( bLowHigh, bLowCur, bHighCur, bPrevCur, nCurData, bShowOHLC);
	nCurData = nDrawingCurPrice;
	pIGraphInfo->SetGraphDataDrawingData( bLowHigh, bLowCur, bHighCur, bPrevCur, nCurData, bShowOHLC);
	pIGraphInfo->Release();
	return true;
}

// -------------------------------------------------------------------------------------
CBaseTabDlg::DLG_TYPE CDlgMainTabIndicatorSetUp::GetDialogType() const
{
	return CBaseTabDlg::INDI_SET;
}

IGraphInfo* CDlgMainTabIndicatorSetUp::GetIndicatorInfoInBaseDlg(const CString& strIndicatorName)
{
	if(m_pChildDlg == NULL || strIndicatorName.IsEmpty())
		return NULL;

	IGraphInfo* pIGraphInfo = m_pChildDlg->GetGraphInfo();
	if( pIGraphInfo)
	{
		ILPCSTR szGraphName = pIGraphInfo->GetGraphNameString();
		if( !strcmp( szGraphName, strIndicatorName)) return pIGraphInfo;
		pIGraphInfo->Release();
	}

	return NULL;
}

bool CDlgMainTabIndicatorSetUp::GetDrawingCurPrice(CBaseTabDlg *pSourceDlgTabPage, int& nDrawingCurPrice) const
{
	if(pSourceDlgTabPage == NULL || (CBaseTabDlg*)this == pSourceDlgTabPage)
		return false;

	if(pSourceDlgTabPage->GetDialogType() != CBaseTabDlg::ENV_SET)
		return false;

	pSourceDlgTabPage->UpdateData(TRUE);
	return (pSourceDlgTabPage->GetCurrentPrice(nDrawingCurPrice));
}
//sy end


// (2006/8/15 - Seung-Won, Bae) Support MChart Interface
// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface
void CDlgMainTabIndicatorSetUp::GetMChartInterface( COutsideCommonInterfaceFunction &p_outsideIF)
{
	if( !m_pIChartOCX) return;
	ILPCSTR szDataPath = m_pIChartOCX->GetDefaultPathString();
	p_outsideIF.SetOutsideCommonInterface( m_pIChartOCX->GetMChartInterface(), CString( szDataPath));
}

CString CDlgMainTabIndicatorSetUp::GetDataFolderPath( void )
{
	if( !m_pIChartOCX) return "";

	CString strPath;
	ILPCSTR szPath = m_pIChartOCX->GetDefaultPathString();
	strPath = szPath;

	return strPath;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2007 / 01 / 12
// Return void	: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnBtnAdd() 
{
	ML_SET_LANGUAGE_RES();

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	DWORD dwTime = timeGetTime();
	if( dwTime - m_dwLastClickedTime < 100) return;

	GetDlgItem( IDC_BTN_ADD)->EnableWindow( FALSE);

	// TODO: Add your control notification handler code here
	CIndicatorNameListCtrl *pGeneralNameListCtrl = NULL;
	pGeneralNameListCtrl = m_ctrlGroupButton.GetSelectListCtrl();
	
	if( pGeneralNameListCtrl != NULL )
	{
		int nGeneralSelectItemIndex = pGeneralNameListCtrl->GetSelectItemIndex();
		if(  nGeneralSelectItemIndex > -1 )
		{
			CIndicatorNameListCtrl *pUseNameListCtrl = m_ctrlUseGroupButton.GetSelectListCtrl();

// 2008.01.25 by LYH >>
			//CString strIndicatorName = pGeneralNameListCtrl->GetItemText( pGeneralNameListCtrl->GetSelectItemIndex(), 0 );
			CString strIndicatorName = _LTOM2(pGeneralNameListCtrl->GetItemText( pGeneralNameListCtrl->GetSelectItemIndex(), 0 ));
// 2008.01.25 by LYH >>

			if( strIndicatorName != "" )
			{
				if( strIndicatorName == _MTEXT(C2_PRICE_CHART))
					AfxMessageBox( IDS_MSG_PRICECHART_ADD );
				else
				{
					CString strGraphName;
					AddGraph( strIndicatorName, strGraphName);
					if( !strGraphName.IsEmpty()) pUseNameListCtrl->AddGraph( strIndicatorName, strGraphName);
				}
			}
		}
	}
	GetDlgItem( IDC_BTN_ADD)->EnableWindow( TRUE);

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	m_dwLastClickedTime = timeGetTime();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2007 / 01 / 12
// Return void	: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnBtnAddAll() 
{
	ML_SET_LANGUAGE_RES();

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	DWORD dwTime = timeGetTime();
	if( dwTime - m_dwLastClickedTime < 100) return;

	GetDlgItem( IDC_BTN_ADD_ALL)->EnableWindow( FALSE);

	// TODO: Add your control notification handler code here
	CIndicatorNameListCtrl *pGeneralNameListCtrl = NULL;
	pGeneralNameListCtrl = m_ctrlGroupButton.GetSelectListCtrl();
	
	if( pGeneralNameListCtrl != NULL )
	{
		int nAllCount = pGeneralNameListCtrl->GetItemCount();
		CIndicatorNameListCtrl *pUseNameListCtrl = m_ctrlUseGroupButton.GetSelectListCtrl();
		CString strIndicatorName = "";
		for( int nGeneralItemIndex = 0 ; nGeneralItemIndex < nAllCount ; nGeneralItemIndex++ )
		{
// 2008.01.25 by LYH >>
			//strIndicatorName = pGeneralNameListCtrl->GetItemText( nGeneralItemIndex, 0 );
			strIndicatorName = _LTOM2(pGeneralNameListCtrl->GetItemText( nGeneralItemIndex, 0 ));
// 2008.01.25 by LYH <<
			if( strIndicatorName != "" )
			{
				if( strIndicatorName == _MTEXT(C2_PRICE_CHART))
				{
					AfxMessageBox( IDS_MSG_PRICECHART_ADD );
					continue;
				}

				CString strGraphName;
				AddGraph( strIndicatorName, strGraphName);
				if( !strGraphName.IsEmpty()) pUseNameListCtrl->AddGraph( strIndicatorName, strGraphName);
			}
		}
	}	
	GetDlgItem( IDC_BTN_ADD_ALL)->EnableWindow( TRUE);

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	m_dwLastClickedTime = timeGetTime();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2007 / 01 / 12
// Return void	: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnBtnDelete() 
{
	ML_SET_LANGUAGE_RES();

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	DWORD dwTime = timeGetTime();
	if( dwTime - m_dwLastClickedTime < 100) return;

	GetDlgItem( IDC_BTN_DELETE)->EnableWindow( FALSE);

	// TODO: Add your control notification handler code here
	CIndicatorNameListCtrl *pUseNameListCtrl = NULL;
	pUseNameListCtrl = m_ctrlUseGroupButton.GetSelectListCtrl();
	
	if( pUseNameListCtrl)
	{
		int nUseSelectItemIndex = pUseNameListCtrl->GetSelectItemIndex();
		if( 0 <= nUseSelectItemIndex)
		{
// 2008.01.25 by LYH >>
			//CString strIndicatorName	= pUseNameListCtrl->GetItemText( nUseSelectItemIndex, 0);
			CString strIndicatorName	= _LTOM2(pUseNameListCtrl->GetItemText( nUseSelectItemIndex, 0));
// 2008.01.25 by LYH <<
			CString strGraphName		= pUseNameListCtrl->GetItemText( nUseSelectItemIndex, 1);
			if( !strGraphName.IsEmpty())
			{
				if( strIndicatorName == _MTEXT(C2_PRICE_CHART))
					AfxMessageBox( IDS_MSG_PRICECHART_DEL );
				// OCX�� AddChartȣ��
				else RemoveGraph( strGraphName);
			}
		}
	}

	GetDlgItem( IDC_BTN_DELETE)->EnableWindow( TRUE);

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	m_dwLastClickedTime = timeGetTime();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2007 / 01 / 12
// Return void	: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnBtnDeleteAll() 
{
	ML_SET_LANGUAGE_RES();

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	DWORD dwTime = timeGetTime();
	if( dwTime - m_dwLastClickedTime < 100) return;

	GetDlgItem( IDC_BTN_DELETE_ALL)->EnableWindow( FALSE);

	// TODO: Add your control notification handler code here
	CIndicatorNameListCtrl *pUseNameListCtrl = NULL;
	pUseNameListCtrl = m_ctrlUseGroupButton.GetSelectListCtrl();
	
	if( pUseNameListCtrl != NULL)
	{
		int nAllCount = pUseNameListCtrl->GetItemCount();
		for( int nUseItemIndex = nAllCount - 1; 0 <= nUseItemIndex; nUseItemIndex--)
		{
// 2008.01.25 by LYH >>
			//CString strIndicatorName	= pUseNameListCtrl->GetItemText( nUseItemIndex, 0);
			CString strIndicatorName	= _LTOM2(pUseNameListCtrl->GetItemText( nUseItemIndex, 0));
// 2008.01.25 by LYH <<
			CString strGraphName		= pUseNameListCtrl->GetItemText( nUseItemIndex, 1);

			if( strGraphName != "")
			{
				if( strIndicatorName == _MTEXT(C2_PRICE_CHART))
				{
					AfxMessageBox( IDS_MSG_PRICECHART_DEL );
					continue;
				}

				// OCX�� AddChartȣ��
				RemoveGraph( strGraphName);
			}
		}
	}		
	GetDlgItem( IDC_BTN_DELETE_ALL)->EnableWindow( TRUE);

	// (2007/1/26 - Seung-Won, Bae) Deny the Serial Button Clock
	m_dwLastClickedTime = timeGetTime();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2007 / 01 / 15
// Return void	: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabIndicatorSetUp::OnBtnWithDefault() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	//IGraphInfo* pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo(m_strIndicatorName);
	IGraphInfo* pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER_DEFAULT, m_strIndicatorName);
	if(pIGraphInfo == NULL)
		return;

	ILPCSTR szGraphName = NULL;
	CBaseDialog *pChildDialog = m_BaseDialogContainer.GetBaseDialog(szGraphName);
	if( pChildDialog == NULL )
		return;

	IGraphInfo *pDefaultIndicatorInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER_DEFAULT, szGraphName);
	if( pChildDialog->m_bSaveDefault == TRUE )
	{
		IGraphInfo *pDefaultIndicatorInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER_DEFAULT, szGraphName);
		pChildDialog->OnApply( pDefaultIndicatorInfo );
		pChildDialog->m_bOrgSaveDefault = TRUE;
	}

	pDefaultIndicatorInfo->Release();
}

// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
void CDlgMainTabIndicatorSetUp::OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName)
{
	CIndicatorNameListCtrl *pUseNameListCtrl = NULL;
	pUseNameListCtrl = m_ctrlUseGroupButton.GetSelectListCtrl();
	if( !pUseNameListCtrl) return;

	int nCount = pUseNameListCtrl->GetItemCount();
	int nUseSelectItemIndex = 0;
	for(  nUseSelectItemIndex = 0; nUseSelectItemIndex < nCount; nUseSelectItemIndex++)
		if( pUseNameListCtrl->GetItemText( nUseSelectItemIndex, 1) == p_szGraphName) break;
	if( nCount <= nUseSelectItemIndex) return;

// 2008.01.25 by LYH >>
	//CString strIndicatorName	= pUseNameListCtrl->GetItemText( nUseSelectItemIndex, 0);
	CString strIndicatorName	= _LTOM2(pUseNameListCtrl->GetItemText( nUseSelectItemIndex, 0));
// 2008.01.25 by LYH <<
	CString strGraphName		= pUseNameListCtrl->GetItemText( nUseSelectItemIndex, 1);
	pUseNameListCtrl->RemoveGraph( nUseSelectItemIndex);

	if( strGraphName.IsEmpty()) return;

	// �ش� ��ǥ�� ����Dlg�� �����´�.
	CBaseDialog *pChildDialog = m_BaseDialogContainer.GetBaseDialog( strIndicatorName);
	if( pChildDialog) pChildDialog->Initial( NULL);

	// ��ǥ���� List���� ����
	POSITION pos = m_pIndicatorNameList->Find( strIndicatorName);
	if(pos == NULL) return;
	m_pIndicatorNameList->RemoveAt(pos);

	// (2007/1/25 - Seung-Won, Bae) Show other Dialog
	NM_LISTVIEW nmListView;
	if( nUseSelectItemIndex >= pUseNameListCtrl->GetItemCount() - 1)
			nmListView.iItem =	pUseNameListCtrl->GetItemCount() - 1;
	else	nmListView.iItem =	nUseSelectItemIndex;
	long lResult = 0;
	pUseNameListCtrl->OnClick( ( NMHDR *)&nmListView, &lResult);

	// chartȭ�鿡 �ٷ� ������ �ǹǷ� �̸����⸦ �ѰͰ� ���� �����̴�.
	m_bPreview = TRUE;
}

BOOL CDlgMainTabIndicatorSetUp::RemoveGraph( const char *p_szGraphName)
{
	if( !m_pIChartOCX) return FALSE;
	return m_pIChartOCX->RemoveGraph( p_szGraphName);
}
