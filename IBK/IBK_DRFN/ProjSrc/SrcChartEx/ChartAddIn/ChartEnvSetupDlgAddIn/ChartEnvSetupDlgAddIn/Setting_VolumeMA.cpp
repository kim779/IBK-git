// Setting_VolumeMA.cpp : implementation file
//

#include "stdafx.h"
#include "chartenvsetupdlgaddin.h"
#include "Setting_VolumeMA.h"

#include <assert.h>

#include "../Include_Chart/Dll_Load/IMetaTable.h"	// for _MTOL0()
#include "../Include_Chart/GraphData.h"
#include "../Include_Chart/Conversion.h"

#include "BaseData.h"
#include "GraphSetting.h"
#include "DlgEnvSetUpAll.h"
#include "TabMainChartEnvSetUp.h"
#include "Setting_VolumeMA_Condition.h"

#define IDC_CALCULATEMETHOD_ALL		19998

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetting_VolumeMA dialog


CSetting_VolumeMA::CSetting_VolumeMA( IChartOCX *p_pIChartOCX, CWnd* pParent /*=NULL*/, CWnd *pTopWnd)
	: CBaseDialog(CSetting_VolumeMA::IDD, pParent, p_pIChartOCX, pTopWnd)
{
	//{{AFX_DATA_INIT(CSetting_VolumeMA)
	m_bShowArrow = FALSE;
	//}}AFX_DATA_INIT
		
	m_bInitialize = false;
	m_pParent = pParent;

	m_nSubGraphCount = 0;
	m_pDlgMACondition = NULL;
}


CSetting_VolumeMA::~CSetting_VolumeMA()
{
	if( m_pDlgMACondition )
		delete m_pDlgMACondition;
}


void CSetting_VolumeMA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting_VolumeMA)
	DDX_Control(pDX, IDC_THICKNESS_ALL, m_cmbThicknessGraphAll);
	DDX_Control(pDX, IDC_DRAWSTYLE_ALL, m_cmbDrawStyleGraphAll);
	DDX_Control(pDX, IDC_CHECK_ALL, m_btnShowGraphAll);
	DDX_Check(pDX, IDC_CHECK_SHOWARROW, m_bShowArrow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetting_VolumeMA, CDialog)
	//{{AFX_MSG_MAP(CSetting_VolumeMA)
	ON_BN_CLICKED(IDC_CHECK_ALL, OnCheckAll)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE_ALL, OnSelchangeDrawstyleAll)
	ON_CBN_SELCHANGE(IDC_THICKNESS_ALL, OnSelchangeThicknessAll)
	ON_MESSAGE(CM_SELECTION_CHANGED, OnSelectionChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting_VolumeMA message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 10 / 18
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CSetting_VolumeMA::OnInitDialog() 
{
	// 1. BaseDialog�� OnInitDialog�� ȣ���Ѵ�.
	CBaseDialog::OnInitDialog();

	// 3. MovingAverage Condition Dialog�� �����Ѵ�.
	if( !m_pDlgMACondition)
	{
		m_pDlgMACondition = new CSetting_VolumeMA_Condition( this );
		m_pDlgMACondition->Create( IDD_VOLUMEMA_CONDITION, this );
		// 6. MA_Condition Dialog�� ��ġ�� ���Ѵ�.
		SetMAConditionDlgPosition();
	}

	// 2. CalcMethod Combo�� �ʱ�ȭ�Ѵ�.
// 2008.01.25 by LYH >>
	//if( CreateDropButton( m_btnCalcMethodAll, "���", IDC_STATIC_CALCMETHOD_ALL, IDC_CALCULATEMETHOD_ALL, false ) )
	CString strCalc;
	strCalc.LoadString(IDS_STRING_CALC);
	if( CreateDropButton( m_btnCalcMethodAll, strCalc, IDC_STATIC_CALCMETHOD_ALL, IDC_CALCULATEMETHOD_ALL, false ) )
// 2008.01.25 by LYH <<
	{
		CDropList *pList = m_btnCalcMethodAll.GetList();
// 2008.01.25 by LYH >>
//		pList->AddString("�ܼ�");
//		pList->AddString("����");
//		pList->AddString("����");
//		pList->AddString("����");
		CString strSimple, strIndex, strWeighted, strFitness;
		strSimple.LoadString(IDS_STRING_SIMPLE);
		strIndex.LoadString(IDS_STRING_INDEX);
		strWeighted.LoadString(IDS_STRING_WEIGHTED);
		strFitness.LoadString(IDS_STRING_FITNESS);
		pList->AddString(strSimple);
		pList->AddString(strIndex);
		pList->AddString(strWeighted);
		pList->AddString(strFitness);
// 2008.01.25 by LYH <<
	}
	
	if( !m_pIGraphInfo) return TRUE;

	// 5. �������� �ʱ�ȭ�Ѵ�.
	Initial( NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 03
// Return BOOL	: 
// Param  CDropButton *pBtn : 
// Param  CString strTitle : 
// Param  UINT uID : 
// Param  bool bChangeTitleWithSelection : 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CSetting_VolumeMA::CreateDropButton( CDropButton &pBtn, CString strTitle, UINT uID, UINT uID2, bool bChangeTitleWithSelection )
{
	CRect rcBase;
	CWnd *pWnd = GetDlgItem( uID);
	if( pWnd) pWnd->GetWindowRect(rcBase);
	ScreenToClient(rcBase);

	if( pWnd) pWnd->ShowWindow(SW_HIDE);
	
	if( !pBtn.Create( strTitle, rcBase, this, uID2 ) )
		return FALSE;

	pBtn.ChangeTitleWithSelection( bChangeTitleWithSelection );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 01
// Return void	: 
// Param  void : 
// Comments		: �ʱ�ȭ�� �ʿ��� Control���� �ʱ�ȭ ��Ų��.
//-----------------------------------------------------------------------------
void CSetting_VolumeMA::Initial_Control( void )
{
	// 4. DrawStyle Combo�� �ʱ�ȭ�Ѵ�.
	Initial_DrawStyleComboBox( 0, m_cmbDrawStyleGraphAll );

	// 5. Thickness Combo�� �ʱ�ȭ�Ѵ�.
	Initial_ThicknessComboBox( 0, 0, m_cmbThicknessGraphAll );

	if( m_pIGraphInfo )
		m_nSubGraphCount = m_pIGraphInfo->GetSubGraphCount();
	
	// 6. CSetting_VolumeMA_Contidio Dialog�� Control���� �ʱ�ȭ�Ѵ�.
	int i;
	CBitmapItemComboBox *pDrawStyleCombo = NULL;
	CBitmapItemComboBox *pThicknessCombo = NULL;
	for( i = 0 ; i < m_nSubGraphCount ; i++ )
	{
		// 4.6 SubGraph�� DrawStyle�� �����Ѵ�.
		pDrawStyleCombo = GetDrawStyleCombo( i);
		if( pDrawStyleCombo) Initial_DrawStyleComboBox( i, *pDrawStyleCombo);			// Draw Style �ʱ�ȭ.
		
		// 4.7 SubGraph�� Line Thickness�� �����Ѵ�.
		pThicknessCombo = GetThicknessCombo( i);
		if( pThicknessCombo) Initial_ThicknessComboBox( i, 0, *pThicknessCombo);		// Line Thickness �ʱ�ȭ.
	}

	for( ; i < VOLUME_MA_CONDITION_COUNT ; i++ )
	{
		ShowHideSubGraph(i, SW_HIDE);
	}
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 10 / 18
// Return void	: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_VolumeMA::SetMAConditionDlgPosition()
{
	// 1. PriceMA Condition Dialog�� �����Ǿ� ���� ������ Return�Ѵ�.
	if( m_pDlgMACondition == NULL )		return;

	// 2. MovingAverage Condition Dialog�� ������ PictureBox�� pointer�� ���´�.
	CDialog *pDlgItem = (CDialog *)GetDlgItem(IDC_VOLUMEMA_CONDITION);
	if( pDlgItem == NULL )		return;
	
	// 3. MA Condition Dialog�� ��ġ�ñ�� ���� PictureBox�� ũ�⸦ ���´�.
	CRect rcDlgItem;
	pDlgItem->GetWindowRect(rcDlgItem);
	ScreenToClient(rcDlgItem);

	// 4. PictureBox�� �����ش�.
	pDlgItem->ShowWindow(SW_SHOW);

	rcDlgItem.DeflateRect(1,1);

	m_pDlgMACondition->MoveWindow( rcDlgItem );

	// 5. �������ѵ״� MA_Condition Dialog�� ��ġ��Ų��.
	m_pDlgMACondition->SetWindowPos( &wndTop, rcDlgItem.left, rcDlgItem.top,
		rcDlgItem.Width(), rcDlgItem.Height(), SWP_SHOWWINDOW );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void  : 
// Param  IGraphInfo* p_pIGraphInfo : 
// Comments		: �ʱ�ȭ �Լ�
//-----------------------------------------------------------------------------
void CSetting_VolumeMA::Initial(IGraphInfo* p_pIGraphInfo)
{
	if( m_pDlgMACondition) m_pDlgMACondition->Initial( p_pIGraphInfo);
	if( m_pIGraphInfo)
	{
		OnApply( m_pIGraphInfo);
		m_pIGraphInfo->Release();
	}
	m_pIGraphInfo = p_pIGraphInfo;
	if( m_pIGraphInfo) m_pIGraphInfo->AddRef();
	else return;

	// 4. 
	Initial_Control();

	// 2. �ʱ�ȭ�� �Ϸ�ɶ� m_bInitialize�� true�� �ٲ۴�.
	m_bInitialize = false;

	// 3. SubGraph�� ������ ���´�.
	m_nSubGraphCount = m_pIGraphInfo->GetSubGraphCount();
	if( m_pDlgMACondition )
		m_pDlgMACondition->SetSubGraphCount( m_nSubGraphCount );

	// 4. SubGraph�� ������ŭ UI�� ���� setting�Ѵ�.
	BOOL bShow;
	IDoubleList *pIDataList = NULL;
	double dCondition1;
	for( int i = 0 ; i < m_nSubGraphCount ; i++ )
	{
		// 4.1 SubGraph�� Show/Hide ���� ǥ���Ѵ�.
		bShow = FALSE;
		m_pIGraphInfo->GetSubGraphShowFlag(i, bShow);
		SetShowHide( i, bShow );
		
		// 4.2 ���������� CList::GetHead()���� ���´�.
		pIDataList = m_pIGraphInfo->GetCalculateConditionList(i);
		if( !pIDataList) continue;

		dCondition1 = -1;
		pIDataList->GetHead( dCondition1);
		SetConditionValue(i, (int)dCondition1);
		pIDataList->Release();
	
		// 4.4 SubGraph�� CalcMethod�� �����Ѵ�.
		SetCalMethod(i);

		// 4.5 SubGraph�� color�� �����Ѵ�.
		SetLineColor(i);

		// 4.8 Draw Style �ʱ�ȭ.
		SetDrawStyle(i);	
		
		// Line Thickness �ʱ�ȭ.
		SetLineThickness(i);			
	}

	// �Ⱓ�� ȭ��ǥ ���� �ʱ�ȭ
	SetShowArrow();
	
	m_bInitialize = true;				// �ʱ�ȭ �Ϸ�

	UpdateData(FALSE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
//sy 2005.09.28. ���ǰ��� �ٲ�������� ȣ�� �Ǿ�� �Ѵ�.
// Return void : 
// Param  IGraphInfo* p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------
// 20081014 �ڵ��� >>
BOOL CSetting_VolumeMA::OnApply(IGraphInfo* p_pIGraphInfo)
//void CSetting_VolumeMA::OnApply(IGraphInfo* p_pIGraphInfo)
// 20081014 �ڵ��� <<
{
	if( p_pIGraphInfo== NULL) return FALSE;
	
	UpdateData( TRUE);

	IDoubleList *pIDrawCnd = m_pIGraphInfo->GetDrawConditionList();
	if( !pIDrawCnd) return FALSE;

	double dDrawType = -1;
	POSITION psn = pIDrawCnd->GetHeadPosition( );
	if( psn ) pIDrawCnd->SetAt( psn, m_bShowArrow);
	pIDrawCnd->Release();

	ApplyAllGraphData();

	int nCount = p_pIGraphInfo->GetSubGraphCount();
	for(int i = 0; i < nCount; i++)
	{
		IDoubleList *pIDataList1 = p_pIGraphInfo->GetCalculateConditionList( i);
		if( !pIDataList1) continue;
		IDoubleList *pIDataList2 = m_pIGraphInfo->GetCalculateConditionList( i);
		if( !pIDataList2)
		{
			pIDataList1->Release();
			continue;
		}
		BOOL bEqual1 = DoesEqualDoubleList( pIDataList1, pIDataList2);
		pIDataList1->Release();
		pIDataList2->Release();

		// (2006/12/4 - Seung-Won, Bae) Check Show-Hide Flag Changing.
		BOOL bShowOrg = FALSE;
		p_pIGraphInfo->GetSubGraphShowFlag( i, bShowOrg);
		BOOL bShowNew = FALSE;
		m_pIGraphInfo->GetSubGraphShowFlag( i, bShowNew);
		BOOL bEqual2 = ( ( bShowOrg ? 1 : 0) + ( bShowNew ? 1 : 0) != 1);

		if( bEqual1 && bEqual2) continue;

		CBaseDialog::m_bDoCalculate = true;
		break;
	}

	p_pIGraphInfo->GetCopyDataFrom(m_pIGraphInfo);

	return TRUE;
}

void CSetting_VolumeMA::ApplyAllGraphData()
{
	// �ٸ� ������ ����Dlg�� ��ȯ�ϱ����� �� �Լ��� ȣ��ǹǷ�
	// CSubGraphData�� �ٽ� ������ �ʿ䰡 ����. �ٷ� ��� m_nSubGraphIndex�� �����Ű�� �ȴ�.
	if( m_pIGraphInfo== NULL ) return;
	
	IDoubleList *pICalCnd = NULL;
	CString strCondition;
	double dCalCnd;
	POSITION psn;
	CComboBox *pCalcMethodCombo = NULL;
	CButton *pCheckButton = NULL;
	CBitmapItemComboBox *pDrawStyleCombo = NULL;
	CNumEdit *pEditCtrl = NULL;
	CBitmapItemComboBox *pThicknessCombo = NULL;
	for( int i = 0 ; i < m_nSubGraphCount ; i++ )
	{
		// Show/Hide �����ϱ�
		pCheckButton = GetCheckButton( i);
		if( pCheckButton) m_pIGraphInfo->SetSubGraphShowFlag( i, pCheckButton->GetCheck());
		
		pICalCnd = m_pIGraphInfo->GetCalculateConditionList( i);
		pEditCtrl = GetEditCtrl( i);
		if( pICalCnd )
		{
			psn = pICalCnd->GetHeadPosition();
			if( psn )
			{
				// ���� �����ϱ�.
				strCondition = "";
				if( pEditCtrl) pEditCtrl->GetWindowText( strCondition);
				pICalCnd->SetAt( psn, atof(strCondition));

				dCalCnd = -1;
				pICalCnd->GetNext( psn, dCalCnd);
			}

			if(psn)
			{
				pCalcMethodCombo = GetCalcMethodCombo( i);
				if( pCalcMethodCombo) pICalCnd->SetAt( psn, pCalcMethodCombo->GetCurSel() + 1);
			}
			pICalCnd->Release();
		}

		strCondition = "";
		if( pEditCtrl) pEditCtrl->GetWindowText(strCondition);
		// (2007/2/20 - Seung-Won, Bae) Set with Header and Footer
		ILPCSTR szOrgTitle = m_pIGraphInfo->GetSubGraphTitleString( i);
		CString strOrgTitle = _MTOL1_N( szOrgTitle);
		if( strOrgTitle.IsEmpty())	strOrgTitle = szOrgTitle;
		const char *szTitle = strOrgTitle;
		int nStartIndex = 0;
		while( szTitle[ nStartIndex] && ( '9' < szTitle[ nStartIndex] || szTitle[ nStartIndex] < '0')) nStartIndex++;
		int nEndIndex = nStartIndex;
		while( szTitle[ nEndIndex] && '0' <= szTitle[ nEndIndex] && szTitle[ nEndIndex] <= '9') nEndIndex++;
		if( nStartIndex != nEndIndex)
		{
			CString strNewCondition;
			strNewCondition.Format( "%s%d%s",strOrgTitle.Left( nStartIndex), atoi( strCondition), strOrgTitle.Mid( nEndIndex));
			m_pIGraphInfo->SetSubGraphTitle( i, strNewCondition);
		}

		CColorButton * pColorButton = GetColorButton(i);
		COLORREF crColor = pColorButton->GetColor();
		m_pIGraphInfo->SetSubGraphColor( i, 1, crColor);
		m_pIGraphInfo->SetSubGraphColor( i, 2, crColor);

		// ������ ����.
		pThicknessCombo = GetThicknessCombo( i);
		if( pThicknessCombo) OnApply_Thickness( i, *pThicknessCombo);

		// ���� ����.
		pDrawStyleCombo = GetDrawStyleCombo( i);
		if( pDrawStyleCombo) OnApply_DrawStyle( i, *pDrawStyleCombo);
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void : 
// Param  const int nIndex : 
// Param  const int nConditionValue : 
// Comments		: ������ �����Ѵ�.
//-----------------------------------------------------------------------------
void CSetting_VolumeMA::SetConditionValue(const int nSubGraphIndex, const int nConditionValue)
{
	// ���� ����.
	CNumEdit *pEditCtrl = GetEditCtrl( nSubGraphIndex);
	CNumSpinCtrl *pSpinCtrl = GetSpinCtrl( nSubGraphIndex);
	if( pEditCtrl && pSpinCtrl) Initial_Spin( *pSpinCtrl, *pEditCtrl, nConditionValue);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return CNumSpinCtrl& : 
// Param  const int nIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
CNumSpinCtrl *CSetting_VolumeMA::GetSpinCtrl(const int nSubGraphIndex)
{
	if( !m_pDlgMACondition ) return NULL;
	return m_pDlgMACondition->GetSpinCtrl(nSubGraphIndex);
}


CNumEdit *CSetting_VolumeMA::GetEditCtrl(const int nSubGraphIndex)
{
	if( !m_pDlgMACondition ) return NULL;
	return m_pDlgMACondition->GetEditCtrl(nSubGraphIndex);
}


void CSetting_VolumeMA::SetShowHide(const int nSubGraphIndex, const int nCheck)
{
	if( !m_pDlgMACondition ) return;
	CButton *pCheckButton = m_pDlgMACondition->GetCheckButton( nSubGraphIndex);
	if( pCheckButton) pCheckButton->SetCheck( nCheck);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 01
// Return void	: 
// Param  void : 
// Comments		: �Ⱓ�� ȭ��ǥ���� ���� ����
//-----------------------------------------------------------------------------
void CSetting_VolumeMA::SetShowArrow( void )
{
	if(m_pIGraphInfo == NULL)
		return;

	IDoubleList *pIDrawCnd = m_pIGraphInfo->GetDrawConditionList();
	if( !pIDrawCnd) return;

	double dCondition = -1;
	if( pIDrawCnd->GetHead( dCondition)) m_bShowArrow = ( int)dCondition;
	pIDrawCnd->Release();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 10 / 30
// Return void	: 
// Param  int nSubGraphIndex : SubGraph Index
// Comments		: SubGraph�� Index�� �޾Ƽ� CSetting_VolumeMA_Condition Dialog�� 
//				: �ش�Ǵ� DrawStyle Combo�� ���� �����Ѵ�.
//-----------------------------------------------------------------------------
void CSetting_VolumeMA::SetDrawStyle( int nSubGraphIndex )
{
	if(m_pDlgMACondition != NULL)
		m_pDlgMACondition->SetDrawStyle(nSubGraphIndex, m_pIGraphInfo->GetSubGraphDrawStyle(nSubGraphIndex));
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 01
// Return void	: 
// Param  int nSubGraphIndex : SubGraph Index
// Comments		: SubGraph�� Index�� �޾Ƽ� CSetting_VolumeMA_Condition Dialog�� 
//				: �ش�Ǵ� Line Thickness Combo�� ���� �����Ѵ�.
//-----------------------------------------------------------------------------
void CSetting_VolumeMA::SetLineThickness( int nSubGraphIndex )
{
	if(m_pDlgMACondition != NULL)
		m_pDlgMACondition->SetLineThickness(nSubGraphIndex, m_pIGraphInfo->GetSubGraphPenThickness(nSubGraphIndex, 1) - 1);
}


CBitmapItemComboBox *CSetting_VolumeMA::GetDrawStyleCombo( int nSubGraphIndex )
{
	if( !m_pDlgMACondition ) return NULL;
	return m_pDlgMACondition->GetDrawStyleCombo(nSubGraphIndex);
}

CBitmapItemComboBox *CSetting_VolumeMA::GetThicknessCombo( int nSubGraphIndex )
{
	if( !m_pDlgMACondition ) return NULL;
	return m_pDlgMACondition->GetThicknessCombo( nSubGraphIndex );
}


void CSetting_VolumeMA::SetLineColor( int nSubGraphIndex )
{
	if( nSubGraphIndex < 0 || !m_pDlgMACondition) return;
	m_pDlgMACondition->SetLineColor( nSubGraphIndex, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 1) );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void : 
// Param  const int nIndex : 
// Comments		: ����� ���� �޺��ڽ��� �����Ѵ�.
//-----------------------------------------------------------------------------
void CSetting_VolumeMA::SetCalMethod(int nSubGraphIndex)
{
	if(m_pIGraphInfo == NULL)
		return;

	IDoubleList *pIDataList = m_pIGraphInfo->GetCalculateConditionList( nSubGraphIndex);
	if( !pIDataList) return;

	double dCondition = -1;
	pIDataList->GetTail( dCondition);
	pIDataList->Release();

	// ������������� GetTail()�� �ִ�. GetHead()�� ����.
	if( !m_pDlgMACondition) return;
	CComboBox *pCalcMethodCombo = m_pDlgMACondition->GetCalcMethodCombo( nSubGraphIndex);
	if( pCalcMethodCombo) pCalcMethodCombo->SetCurSel( ( int)dCondition - 1);
}


void CSetting_VolumeMA::ShowHideSubGraph( int nSubGraphIndex, int nCmdShow )
{
	if( !m_pDlgMACondition) return;
	m_pDlgMACondition->ShowHideSubGraph( nSubGraphIndex, nCmdShow );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 01
// Modifier		: 
// Comments		: ��ü Check Button�� ���������� �����Ѵ�.
//-----------------------------------------------------------------------------
void CSetting_VolumeMA::OnCheckAll() 
{
	// TODO: Add your control notification handler code here
	if( m_pDlgMACondition == NULL )
		return;

	int nCheck = m_btnShowGraphAll.GetCheck();

	CButton *pCheckButton = NULL;
	for( int i = 0 ; i < m_nSubGraphCount ; i++ )
	{
		pCheckButton = GetCheckButton( i);
		if( pCheckButton) pCheckButton->SetCheck( nCheck );
	}

	if( nCheck == 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("�ּ� �ϳ� �̻��� ���������� �����ؾ� �˴ϴ�.");
		AfxMessageBox(m_strMsgExistMin);
// 2008.01.25 by LYH <<
		GetCheckButton(0)->SetCheck(1);
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 01
// Return CButton*	: 
// Param  int nSubGraphIndex : SubGraph�� Index
// Comments		: CSetting_VolumeMA_Condition Dialog���� CheckButton
//-----------------------------------------------------------------------------
CButton* CSetting_VolumeMA::GetCheckButton(int nSubGraphIndex)
{
	if( m_pDlgMACondition == NULL )
		return &m_btnShowGraphAll;

	return m_pDlgMACondition->GetCheckButton(nSubGraphIndex);
}



CColorButton *CSetting_VolumeMA::GetColorButton(int nSubGraphIndex )
{
	if( !m_pDlgMACondition) return NULL;
	return m_pDlgMACondition->GetColorButton( nSubGraphIndex );
}




//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 01
// Modifier		: 
// Comments		: DrawStyle�� �ϰ� �����Ѵ�.
//-----------------------------------------------------------------------------
void CSetting_VolumeMA::OnSelchangeDrawstyleAll() 
{
	// TODO: Add your control notification handler code here
	// 1. CSetting_PriceMA_Condition dialog�� �������� �ʾ����� return�Ѵ�.
	if( m_pDlgMACondition == NULL )
		return;

	// 2. ���õ� ���� DrawStyle�� ���´�.
	int nDrawStyleGraph = m_cmbDrawStyleGraphAll.GetCurSel();

	// 3. DrawStyle�� �����Ⱑ 1�϶����� �����Ѵ�.
	BOOL bResetThickness = FALSE;
	if( nDrawStyleGraph != 0 && m_cmbThicknessGraphAll.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("������� \"�Ǽ�\"�� ��츸 ���⸦ ������ �� �ֽ��ϴ�.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraphAll.SetCurSel(0);
		bResetThickness = TRUE;
	}

	// 4. SubGraph�� ������ ���� DrawStyle�� Thickness�� �ϰ� �����Ѵ�.
	CBitmapItemComboBox *pDrawStyleCombo = NULL;
	CBitmapItemComboBox *pThicknessCombo = NULL;
	for( int i = 0 ; i < m_nSubGraphCount ; i++ )
	{
		pDrawStyleCombo = GetDrawStyleCombo( i);
		if( pDrawStyleCombo) pDrawStyleCombo->SetCurSel( nDrawStyleGraph );

		// DrawStyle�� �Ǽ� �̿��� ������ �����ϸ� �����⸦ 1�� �����Ѵ�.
		if( bResetThickness)
		{
			pThicknessCombo = GetThicknessCombo( i);
			if( pThicknessCombo) pThicknessCombo->SetCurSel( 0);
		}
	}

	// 5. ThicknessGraphAll�� ���õ� �����⸦ �����Ѵ�.
	m_cmbDrawStyleGraphAll.SetCurSel(nDrawStyleGraph);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 01
// Modifier		: 
// Comments		: Thickness�� �ϰ� �����Ѵ�.
//-----------------------------------------------------------------------------
void CSetting_VolumeMA::OnSelchangeThicknessAll() 
{
	// TODO: Add your control notification handler code here
	
	// 1. CSetting_PriceMA_Condition dialog�� �������� �ʾ����� return�Ѵ�.
	if( m_pDlgMACondition == NULL )
		return;

	// 2. ���õ� ���� ���⸦ ���´�.
	int nThicknessGraph = m_cmbThicknessGraphAll.GetCurSel();

	// 3. �������� �����Ⱑ 1�϶����� �����Ѵ�.
	BOOL bResetDrawStyle = FALSE;
	if( nThicknessGraph != 0 && m_cmbDrawStyleGraphAll.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("������� \"�Ǽ�\"�� ��츸 ���⸦ ������ �� �ֽ��ϴ�.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<

		m_cmbDrawStyleGraphAll.SetCurSel(0);
		bResetDrawStyle = TRUE;
	}

	// 4. SubGraph�� ������ ���� DrawStyle�� Thickness�� �ϰ� �����Ѵ�.
	CBitmapItemComboBox *pDrawStyleCombo = NULL;
	CBitmapItemComboBox *pThicknessCombo = NULL;
	for( int i = 0 ; i < m_nSubGraphCount ; i++ )
	{
		pThicknessCombo = GetThicknessCombo( i);
		if( pThicknessCombo) pThicknessCombo->SetCurSel( nThicknessGraph);

		// �����⸦ 1 �̻����� �����ϸ� DrawStyle�� �Ǽ����� �����Ѵ�.
		if( bResetDrawStyle)
		{
			pDrawStyleCombo = GetDrawStyleCombo( i);
			if( pDrawStyleCombo) pDrawStyleCombo->SetCurSel( 0);
		}
	}
	
	// 5. ThicknessGraphAll�� ���õ� �����⸦ �����Ѵ�.
	m_cmbThicknessGraphAll.SetCurSel(nThicknessGraph);
}



CComboBox* CSetting_VolumeMA::GetCalcMethodCombo( int nSubGraphIndex )
{
	if( !m_pDlgMACondition) return NULL;
	return m_pDlgMACondition->GetCalcMethodCombo( nSubGraphIndex );
}


BOOL CSetting_VolumeMA::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			OK();
		}
		
		if( pMsg->wParam == VK_ESCAPE )
		{
			Cancel();
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CSetting_VolumeMA::Cancel()
{
	if( m_pParentWnd != NULL )
	{
		if( m_pTopWnd != NULL )
			( (CDlgEnvSetUpAll *)m_pTopWnd)->Cancel();

		else
			( (CGraphSetting *)m_pParentWnd)->Cancel();
	}
}


void CSetting_VolumeMA::OK()
{
	if( m_pParentWnd != NULL )
	{
		if( m_pTopWnd != NULL )
			( (CDlgEnvSetUpAll *)m_pTopWnd)->OK();

		else
			( (CGraphSetting *)m_pParentWnd)->OK();
	}
}


LONG CSetting_VolumeMA::OnSelectionChanged(UINT wParam, LONG lParam)
{
    // What control sent the message
    CDropList *pList = NULL;

    if( m_pDlgMACondition == NULL )
		return 0;

	int nCurSel;
	switch(wParam)
    {
         case IDC_CALCULATEMETHOD_ALL :
		{
            pList = m_btnCalcMethodAll.GetList();
			nCurSel = pList->GetCurSel();
			CComboBox *pCalcMethodCombo = NULL;
			for( int i = 0 ; i < m_nSubGraphCount ; i++ )
			{
				pCalcMethodCombo = GetCalcMethodCombo( i);
				if( pCalcMethodCombo) pCalcMethodCombo->SetCurSel( nCurSel);
			}
			break;
		}
    }
       

    return 1;
}
