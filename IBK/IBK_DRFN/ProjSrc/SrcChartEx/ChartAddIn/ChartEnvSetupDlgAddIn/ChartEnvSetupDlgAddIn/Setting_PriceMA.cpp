// Setting_PriceMA.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "Setting_PriceMA.h"

#include <assert.h>

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
#include "../Include_Chart/GraphData.h"
#include "../Include_Chart/Conversion.h"

#include "BaseData.h"
#include "GraphSetting.h"
#include "DlgEnvSetUpAll.h"
#include "TabMainChartEnvSetUp.h"
#include "Setting_PriceMA_Condition.h"
#include "resource.h"									// for IDC_STATIC_PACKET_ALL

#define IDC_PACKETSETTING_ALL		19999
#define IDC_CALCULATEMETHOD_ALL		19998

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetting_PriceMA dialog

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void	: 
// Param  IGraphInfo* p_pIGraphInfo : 
// Param  CWnd* pParent /*=NULL*/ : 
// Comments		: UI�� ������ ���� ������ �ʰ� �����ϱ� ���ؼ� ����IndicatorInfo��
//				: �����.
//-----------------------------------------------------------------------------
CSetting_PriceMA::CSetting_PriceMA( IChartOCX *p_pIChartOCX, CWnd* pParent /*=NULL*/, CWnd *pTopWnd)
	: CBaseDialog(CSetting_PriceMA::IDD, pParent, p_pIChartOCX, pTopWnd)
{
	//{{AFX_DATA_INIT(CSetting_PriceMA)
	m_bShowArrow = FALSE;
	//}}AFX_DATA_INIT

	m_bInitialize = false;

	m_pParent = pParent;

	m_nSubGraphCount = 0;
	
	m_pDlgMACondition = NULL;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
CSetting_PriceMA::~CSetting_PriceMA()
{
	if( m_pDlgMACondition)
	{
		delete m_pDlgMACondition;
		m_pDlgMACondition = NULL;
	}
}


void CSetting_PriceMA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting_PriceMA)
	DDX_Control(pDX, IDC_THICKNESS_ALL, m_cmbThicknessGraphAll);
	DDX_Control(pDX, IDC_DRAWSTYLE_ALL, m_cmbDrawStyleGraphAll);
	DDX_Control(pDX, IDC_CHECK_ALL, m_btnShowGraphAll);
	DDX_Check(pDX, IDC_CHECK_SHOWARROW, m_bShowArrow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetting_PriceMA, CDialog)
	//{{AFX_MSG_MAP(CSetting_PriceMA)
	ON_BN_CLICKED(IDC_CHECK_ALL, OnCheckAll)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE_ALL, OnSelchangeDrawstyleAll)
	ON_CBN_SELCHANGE(IDC_THICKNESS_ALL, OnSelchangeThicknessAll)
	ON_MESSAGE(CM_SELECTION_CHANGED, OnSelectionChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting_PriceMA message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: Dialog �ʱ�ȭ
//-----------------------------------------------------------------------------
BOOL CSetting_PriceMA::OnInitDialog() 
{
	// 1. BaseDialog�� OnInitDialog�� ȣ���Ѵ�.
	CBaseDialog::OnInitDialog();

	// 3. MovingAverage Condition Dialog�� �����Ѵ�.

	ASSERT( !m_pDlgMACondition);
	m_pDlgMACondition = new CSetting_PriceMA_Condition( this );
	m_pDlgMACondition->Create( IDD_PRICEMA_CONDITION, this );

	// 1. Packet Combo�� �ʱ�ȭ�Ѵ�.
	CString strStandard;
	strStandard.LoadString(IDS_STRING_STANDARD);
	if( CreateDropButton( m_btnPacketAll, strStandard, IDC_STATIC_PACKET_ALL, IDC_PACKETSETTING_ALL, false ) )
	{
		m_btnPacketAll.SetListWidth(80);

		CDropList *pList = m_btnPacketAll.GetList();
		pList->ResetContent();
		pList->AddString(_LTEXT0(C0_OPEN));
		pList->AddString(_LTEXT0(C0_HIGH));
		pList->AddString(_LTEXT0(C0_LOW));
		pList->AddString(_LTEXT0(C0_CLOSE));
		CString strStdHL, strStdHLC;
		strStdHL.LoadString(IDS_STANDART_HL);
		strStdHLC.LoadString(IDS_STANDART_HLC);
		pList->AddString(strStdHL);
		pList->AddString(strStdHLC);
	}

	// 2. CalcMethod Combo�� �ʱ�ȭ�Ѵ�.
// 2008.01.25 by LYH >>
	//if( CreateDropButton( m_btnCalcMethodAll, "���", IDC_STATIC_CALCMETHOD_ALL, IDC_CALCULATEMETHOD_ALL, false ) )
	CString strCalc;
	strCalc.LoadString(IDS_STRING_CALC);	
	if( CreateDropButton( m_btnCalcMethodAll, strCalc, IDC_STATIC_CALCMETHOD_ALL, IDC_CALCULATEMETHOD_ALL, false ) )
	{
		CDropList *pList = m_btnCalcMethodAll.GetList();
		CString strSimple, strIndex, strWeighted, strFitness;
		strSimple.LoadString(IDS_STRING_SIMPLE);
		strIndex.LoadString(IDS_STRING_INDEX);
		strWeighted.LoadString(IDS_STRING_WEIGHTED);
		strFitness.LoadString(IDS_STRING_FITNESS);
		pList->AddString(strSimple);
		pList->AddString(strIndex);
		pList->AddString(strWeighted);
		pList->AddString(strFitness);
	}
// 2008.01.25 by LYH <<
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
BOOL CSetting_PriceMA::CreateDropButton( CDropButton &pBtn, CString strTitle, UINT uID, UINT uID2, bool bChangeTitleWithSelection )
{
	CRect rcBase;
	CWnd * pWnd = GetDlgItem( uID);
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
void CSetting_PriceMA::Initial_Control( void )
{
	// 3. DrawStyleAll Combo�� �ʱ�ȭ�Ѵ�.
	Initial_DrawStyleComboBox( 0, m_cmbDrawStyleGraphAll );

	// 4. ThicknessAll Combo�� �ʱ�ȭ�Ѵ�.
	Initial_ThicknessComboBox( 0, 0, m_cmbThicknessGraphAll );

	// 5. SubGraph�� ������ �����Ѵ�.
	if( m_pIGraphInfo )
		m_nSubGraphCount = m_pIGraphInfo->GetSubGraphCount();

	if( m_pDlgMACondition) m_pDlgMACondition->SetSubGraphCount( m_nSubGraphCount );

	// 6. CSetting_PriceMA_Contidio Dialog�� Control���� �ʱ�ȭ�Ѵ�.
	int i;
	CBitmapItemComboBox *pDrawStyleComboBox = NULL;
	CBitmapItemComboBox *pThicknessCombo = NULL;
	for( i = 0 ; i < m_nSubGraphCount ; i++ )
	{
		// 6.1 CSetting_PriceMA_Contidio Dialog�� DrawStyle�� �����Ѵ�.
		pDrawStyleComboBox = GetDrawStyleCombo( i);
		if( pDrawStyleComboBox) Initial_DrawStyleComboBox( i, *pDrawStyleComboBox);			// Draw Style �ʱ�ȭ.
		
		// 6.2 CSetting_PriceMA_Contidio Dialog�� Line Thickness�� �����Ѵ�.
		pThicknessCombo = GetThicknessCombo( i);
		if( pThicknessCombo) Initial_ThicknessComboBox( i, 0, *pThicknessCombo);		// Line Thickness �ʱ�ȭ.

		ShowHideSubGraph(i, SW_SHOW);
	}

	// 7. CSetting_PriceMA_Contidio Dialog���� SubGraph�� ����
	for( ; i < PRICE_MA_CONDITION_COUNT ; i++ )
		ShowHideSubGraph(i, SW_HIDE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 10 / 18
// Return void	: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_PriceMA::SetMAConditionDlgPosition()
{
	// 1. PriceMA Condition Dialog�� �����Ǿ� ���� ������ Return�Ѵ�.
	if( m_pDlgMACondition == NULL )		return;

	// 2. MovingAverage Condition Dialog�� ������ PictureBox�� pointer�� ���´�.
	CDialog *pDlgItem = (CDialog *)GetDlgItem(IDC_PRICEMA_CONDITION);
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
void CSetting_PriceMA::Initial(IGraphInfo* p_pIGraphInfo)
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

	// 4. �ʱ�ȭ�� �ʿ��� Control���� �ʱ�ȭ �Ѵ�.
	Initial_Control();

	// 2. �ʱ�ȭ�� �Ϸ�ɶ� m_bInitialize�� true�� �ٲ۴�.
	m_bInitialize = false;

	// 3. SubGraph�� ������ ���´�.
	m_nSubGraphCount = m_pIGraphInfo->GetSubGraphCount();
	if( m_pDlgMACondition) m_pDlgMACondition->SetSubGraphCount( m_nSubGraphCount );

	// 6. MA_Condition Dialog�� ��ġ�� ���Ѵ�.
	SetMAConditionDlgPosition();
	
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
	
		// 4.3 SubGraph�� Packet�� �����Ѵ�.
		SetPacketName(i);

		// 4.4 SubGraph�� CalcMethod�� �����Ѵ�.
		SetCalMethod(i);

		// 4.5 SubGraph�� color�� �����Ѵ�.
		SetLineColor(i);

		// 4.6 Draw Style �ʱ�ȭ.
		SetDrawStyle(i);	
		
		// 4.7 Line Thickness �ʱ�ȭ.
		SetLineThickness(i);			
	}

	// 5. �Ⱓ�� ȭ��ǥ ���� �ʱ�ȭ
	SetShowArrow();
	
	m_bInitialize = true;		// �ʱ�ȭ �Ϸ�
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
BOOL CSetting_PriceMA::OnApply(IGraphInfo* p_pIGraphInfo)
//void CSetting_PriceMA::OnApply(IGraphInfo* p_pIGraphInfo)
// 20081014 �ڵ��� <<
{
	if(p_pIGraphInfo == NULL)
		return FALSE;

	UpdateData(TRUE);

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

		ILPCSTR szPacketName = m_pIGraphInfo->GetSubGraphPacketNameString( i);
		CString strPacketNames( szPacketName);
		BOOL bEqual2 = DoesEqualPacketNames_SubGraphPart( p_pIGraphInfo, i, strPacketNames);

		// (2006/12/4 - Seung-Won, Bae) Check Show-Hide Flag Changing.
		BOOL bShowOrg = FALSE;
		p_pIGraphInfo->GetSubGraphShowFlag( i, bShowOrg);
		BOOL bShowNew = FALSE;
		m_pIGraphInfo->GetSubGraphShowFlag( i, bShowNew);
		BOOL bEqual3 = ( ( bShowOrg ? 1 : 0) + ( bShowNew ? 1 : 0) != 1);

		if( bEqual1 && bEqual2 && bEqual3) continue;

		CBaseDialog::m_bDoCalculate = true;
		break;
	}

	p_pIGraphInfo->GetCopyDataFrom(m_pIGraphInfo);

	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void : 
// Comments		: ���� ������ �������� �����ϱ�.(���� ����� Info���� ������)
//-----------------------------------------------------------------------------
void CSetting_PriceMA::ApplyAllGraphData()
{
	if(m_pIGraphInfo == NULL) return;

	IDoubleList *pICalCnd = NULL;
	CString strCondition;
	double dCalCnd;
	POSITION psn;
	CNumEdit *pNumEdit = NULL;
	CBitmapItemComboBox *pThicknessCombo = NULL;
	CBitmapItemComboBox *pDrawStyleComboBox = NULL;
	CComboBox *pCalcMethodCombo = NULL;
	CButton *pCheckButton = NULL;
	for( int i = 0 ; i < m_nSubGraphCount ; i++ )
	{
		// Show/Hide �����ϱ�
		pCheckButton = GetCheckButton( i);
		if( pCheckButton) m_pIGraphInfo->SetSubGraphShowFlag( i, pCheckButton->GetCheck());

		pICalCnd = m_pIGraphInfo->GetCalculateConditionList( i);
		pNumEdit = GetEditCtrl( i);
		if( pICalCnd )
		{
			psn = pICalCnd->GetHeadPosition();
			if( psn)
			{
				// ���� �����ϱ�.
				strCondition = "";
				if( pNumEdit) pNumEdit->GetWindowText( strCondition);
				pICalCnd->SetAt( psn, atof(strCondition));

				dCalCnd = -1;
				pICalCnd->GetNext( psn, dCalCnd);
			}
			pCalcMethodCombo = GetCalcMethodCombo(i);
			if( psn && pCalcMethodCombo) 
				pICalCnd->SetAt( psn, pCalcMethodCombo->GetCurSel() + 1);
			pICalCnd->Release();
		}

		strCondition = "";
		if( pNumEdit) pNumEdit->GetWindowText(strCondition);
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

		// ��Ŷ�� �����ϱ�
		CString strPacketNames;
		CComboBox *pComboBox = GetPacketCombo( i);
		int nSelect = 0;
		if( pComboBox) nSelect = pComboBox->GetCurSel();
		if( nSelect < 0) nSelect = 0;
		// (��+����)/2
		if( nSelect == 4)		strPacketNames = CString( "(") + _MTEXT( C0_HIGH) + "+" + _MTEXT( C0_LOW) + ")/2";
		// (��+����+����)/3
		else if( nSelect == 5)	strPacketNames = CString( "(") + _MTEXT( C0_HIGH) + "+" + _MTEXT( C0_LOW) + "+" + _MTEXT( C0_CLOSE) + ")/3";
		else
		{
			if( pComboBox) pComboBox->GetLBText( nSelect, strPacketNames);
			strPacketNames = _LTOM0(strPacketNames);
		}
		if( !strPacketNames.IsEmpty()) strPacketNames += ",";
		m_pIGraphInfo->SetSubGraphPacketName( i, strPacketNames);

		CColorButton * pColorButton = GetColorButton(i);
		COLORREF crColor = pColorButton->GetColor();
		m_pIGraphInfo->SetSubGraphColor( i, 1, crColor);
		m_pIGraphInfo->SetSubGraphColor( i, 2, crColor);

		// ������ ����.
		pThicknessCombo = GetThicknessCombo( i);
		if( pThicknessCombo) OnApply_Thickness( i, *pThicknessCombo);

		// ���� ����.
		pDrawStyleComboBox = GetDrawStyleCombo( i);
		if( pDrawStyleComboBox) OnApply_DrawStyle(i, *pDrawStyleComboBox);
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void : 
// Param  const int nIndex : 
// Param  const int nConditionValue : 
// Comments		: ������ �����Ѵ�.
//-----------------------------------------------------------------------------
void CSetting_PriceMA::SetConditionValue(const int nSubGraphIndex, const int nConditionValue)
{
	// ���� ����.
	CNumSpinCtrl *pNumSpinCtrl = GetSpinCtrl( nSubGraphIndex);
	CNumEdit *pNumEdit = GetEditCtrl( nSubGraphIndex);
	if( pNumSpinCtrl && pNumEdit) Initial_Spin( *pNumSpinCtrl, *pNumEdit, nConditionValue);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return CNumSpinCtrl& : 
// Param  const int nIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
CNumSpinCtrl *CSetting_PriceMA::GetSpinCtrl(const int nSubGraphIndex)
{
	if( !m_pDlgMACondition) return NULL;
	return m_pDlgMACondition->GetSpinCtrl( nSubGraphIndex);
}


CNumEdit *CSetting_PriceMA::GetEditCtrl(const int nSubGraphIndex)
{
	if( !m_pDlgMACondition) return NULL;
	return m_pDlgMACondition->GetEditCtrl( nSubGraphIndex);
}


void CSetting_PriceMA::SetShowHide(const int nSubGraphIndex, const int nCheck)
{
	if( !m_pDlgMACondition) return;
	CButton *pButton = m_pDlgMACondition->GetCheckButton( nSubGraphIndex);
	if( pButton) pButton->SetCheck( nCheck);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void : 
// Comments		: �Ⱓ�� ȭ��ǥ ���� �� ����.
//-----------------------------------------------------------------------------
void CSetting_PriceMA::SetShowArrow( void )
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
// Comments		: SubGraph�� Index�� �޾Ƽ� CSetting_PriceMA_Condition Dialog�� 
//				: �ش�Ǵ� DrawStyle Combo�� ���� �����Ѵ�.
//-----------------------------------------------------------------------------
void CSetting_PriceMA::SetDrawStyle( int nSubGraphIndex )
{
	if(m_pDlgMACondition != NULL)
		m_pDlgMACondition->SetDrawStyle(nSubGraphIndex, m_pIGraphInfo->GetSubGraphDrawStyle(nSubGraphIndex));
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 01
// Return void	: 
// Param  int nSubGraphIndex : SubGraph Index
// Comments		: SubGraph�� Index�� �޾Ƽ� CSetting_PriceMA_Condition Dialog�� 
//				: �ش�Ǵ� Line Thickness Combo�� ���� �����Ѵ�.
//-----------------------------------------------------------------------------
void CSetting_PriceMA::SetLineThickness( int nSubGraphIndex )
{
	if(m_pDlgMACondition != NULL)
		m_pDlgMACondition->SetLineThickness(nSubGraphIndex, m_pIGraphInfo->GetSubGraphPenThickness(nSubGraphIndex, 1) - 1);
}


CBitmapItemComboBox *CSetting_PriceMA::GetDrawStyleCombo( int nSubGraphIndex )
{
	if( !m_pDlgMACondition) return NULL;
	return m_pDlgMACondition->GetDrawStyleCombo(nSubGraphIndex);
}


CBitmapItemComboBox *CSetting_PriceMA::GetThicknessCombo( int nSubGraphIndex )
{
	if( !m_pDlgMACondition) return NULL;
	return m_pDlgMACondition->GetThicknessCombo( nSubGraphIndex );
}


void CSetting_PriceMA::SetLineColor( int nSubGraphIndex )
{
	if( nSubGraphIndex < 0 || !m_pDlgMACondition) return;

	m_pDlgMACondition->SetLineColor( nSubGraphIndex, m_pIGraphInfo->GetSubGraphColor( nSubGraphIndex, 1) );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void : 
// Param  const int nIndex : 
// Comments		: ��Ŷ���� �޺��ڽ��� �����Ѵ�.
//-----------------------------------------------------------------------------
void CSetting_PriceMA::SetPacketName(const int nSubGraphIndex)
{
	if(m_pIGraphInfo == NULL || !m_pDlgMACondition) return;

	ILPCSTR szPacketName = m_pIGraphInfo->GetSubGraphPacketNameString( nSubGraphIndex);
	CString strPacketNames( szPacketName);

	int nPacketCount = CDataConversion::GetStringDataCount(strPacketNames, _T(","));
	strPacketNames = CDataConversion::GetStringData(strPacketNames, _T(","));

	CComboBox *pPacketCombo = m_pDlgMACondition->GetPacketCombo(nSubGraphIndex);
	int nCount = 0;
	if( pPacketCombo) nCount = pPacketCombo->GetCount();
	if( nPacketCount != 1)
	{
		pPacketCombo->SetCurSel(nCount -1);
		return;
	}

	pPacketCombo->SetCurSel(4);
	if( 0 <= strPacketNames.Find( '('))
	{
		if( strPacketNames == CString( "(") + _MTEXT( C0_HIGH) + "+" + _MTEXT( C0_LOW) + ")/2")
			pPacketCombo->SetCurSel(4);
		else if( strPacketNames == CString( "(") + _MTEXT( C0_HIGH) + "+" + _MTEXT( C0_LOW) + "+" + _MTEXT( C0_CLOSE) + ")/3")
			pPacketCombo->SetCurSel(5);
	}
	else
	{
		strPacketNames = _MTOLS( strPacketNames, ",", 0);
		CString data;
		for(int i = 0 ; i < pPacketCombo->GetCount() ; i++)
		{
			pPacketCombo->GetLBText( i, data);	
			if( strPacketNames.Compare(data) == 0)
			{
				pPacketCombo->SetCurSel(i);
				break;
			}
		}
	}
}




//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void : 
// Param  const int nIndex : 
// Comments		: ����� ���� �޺��ڽ��� �����Ѵ�.
//-----------------------------------------------------------------------------
void CSetting_PriceMA::SetCalMethod(int nSubGraphIndex)
{
	if(m_pIGraphInfo == NULL || !m_pDlgMACondition) return;

	IDoubleList *pIDataList = m_pIGraphInfo->GetCalculateConditionList( nSubGraphIndex);
	if( !pIDataList) return;

	double dCondition = -1;
	pIDataList->GetTail( dCondition);
	pIDataList->Release();

	// ������������� GetTail()�� �ִ�. GetHead()�� ����.
	CComboBox *pCalcMethodCombo = m_pDlgMACondition->GetCalcMethodCombo( nSubGraphIndex);
	if( pCalcMethodCombo) pCalcMethodCombo->SetCurSel( ( int)dCondition - 1);
}


void CSetting_PriceMA::ShowHideSubGraph( int nSubGraphIndex, int nCmdShow )
{
	if( m_pDlgMACondition) m_pDlgMACondition->ShowHideSubGraph( nSubGraphIndex, nCmdShow );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 01
// Modifier		: 
// Comments		: ��ü Check Button�� ���������� �����Ѵ�.
//-----------------------------------------------------------------------------
void CSetting_PriceMA::OnCheckAll() 
{
	// TODO: Add your control notification handler code here
	if( m_pDlgMACondition == NULL )
		return;

	int nCheck = m_btnShowGraphAll.GetCheck();

	CButton *pButton = NULL;
	for( int i = 0 ; i < m_nSubGraphCount ; i++ )
	{
		pButton = GetCheckButton( i);
		if( pButton) pButton->SetCheck( nCheck);
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
// Comments		: CSetting_PriceMA_Condition Dialog���� CheckButton
//-----------------------------------------------------------------------------
CButton* CSetting_PriceMA::GetCheckButton(int nSubGraphIndex)
{
	if( m_pDlgMACondition == NULL )
		return &m_btnShowGraphAll;

	return m_pDlgMACondition->GetCheckButton(nSubGraphIndex);
}


CColorButton *CSetting_PriceMA::GetColorButton(int nSubGraphIndex )
{
	if( !m_pDlgMACondition) return NULL;
	return m_pDlgMACondition->GetColorButton( nSubGraphIndex );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 01
// Modifier		: 
// Comments		: DrawStyle�� �ϰ� �����Ѵ�.
//-----------------------------------------------------------------------------
void CSetting_PriceMA::OnSelchangeDrawstyleAll() 
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
void CSetting_PriceMA::OnSelchangeThicknessAll() 
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
		if( pThicknessCombo) pThicknessCombo->SetCurSel( nThicknessGraph );

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


CComboBox* CSetting_PriceMA::GetCalcMethodCombo( int nSubGraphIndex )
{
	if( !m_pDlgMACondition) return NULL;
	return m_pDlgMACondition->GetCalcMethodCombo( nSubGraphIndex );
}


CComboBox* CSetting_PriceMA::GetPacketCombo( int nSubGraphIndex )
{
	if( !m_pDlgMACondition) return NULL;
	return m_pDlgMACondition->GetPacketCombo( nSubGraphIndex );
}


BOOL CSetting_PriceMA::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
			OK();
		
		if( pMsg->wParam == VK_ESCAPE )
			Cancel();
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CSetting_PriceMA::Cancel()
{
	if( m_pParentWnd != NULL )
	{
		if( m_pTopWnd != NULL )
			( (CDlgEnvSetUpAll *)m_pTopWnd)->Cancel();

		else
			( (CGraphSetting *)m_pParentWnd)->Cancel();
	}
}

void CSetting_PriceMA::OK()
{
	if( m_pParentWnd != NULL )
	{
		if( m_pTopWnd != NULL )
			( (CDlgEnvSetUpAll *)m_pTopWnd)->OK();

		else
			( (CGraphSetting *)m_pParentWnd)->OK();
	}
}


LONG CSetting_PriceMA::OnSelectionChanged(UINT wParam, LONG lParam)
{
    // What control sent the message
    CDropList *pList = NULL;

    if( m_pDlgMACondition == NULL )
		return 0;

	int nCurSel;
	switch(wParam)
    {
        case IDC_PACKETSETTING_ALL :
		{
			pList = m_btnPacketAll.GetList();
			nCurSel = pList->GetCurSel();
        	for( int i = 0 ; i < m_nSubGraphCount ; i++ )
				GetPacketCombo(i)->SetCurSel( nCurSel );

			break;
		}

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

/*
//sy 2005.11.08.
void CSetting_PriceMA::Initial_SubGraphName(const int nIDIndex)
{
	// ����׷����̸��� �����Ѵ�.
	//CBaseDialog::Initial_SubGraphName(m_nSubGraphIndex, m_SubGraphName_Bt);
	if(!IsEditIndex(nIDIndex))
		return;

	double dTitle = GetEditCtrl(nIDIndex).GetValue();
	CString strText;
	strText.Format("[ %.0f�� ����(%d��°) ]", dTitle, (nIDIndex+1));	
	m_SubGraphName_Bt.SetWindowText(strText);
}
//sy end

*/
