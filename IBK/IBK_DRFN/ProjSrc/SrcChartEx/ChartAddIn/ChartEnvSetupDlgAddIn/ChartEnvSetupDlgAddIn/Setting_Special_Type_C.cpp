// Setting_Special_Type_C.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "Setting_Special_Type_C.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
#include "../Include_Chart/Conversion.h"

#include "GraphData.h"
#include "DlgEnvSetUpAll.h"
#include "GraphSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetting_Special_Type_C dialog


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void : 
// Param  CWnd* pParent /*=NULL*/ : 
// Comments		: 
//-----------------------------------------------------------------------------
CSetting_Special_Type_C::CSetting_Special_Type_C( IChartOCX *p_pIChartOCX, CWnd* pParent /*=NULL*/, CWnd *pTopWnd)
	: CBaseDialog(CSetting_Special_Type_C::IDD, pParent, p_pIChartOCX, pTopWnd)
{
	//{{AFX_DATA_INIT(CSetting_Special_Type_C)
	m_strBlockChange = _T("");
	//}}AFX_DATA_INIT

	m_nSubGraphIndex = -1;
}


void CSetting_Special_Type_C::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting_Special_Type_C)
	DDX_Control(pDX, IDC_BTN_UP, m_btnUp);
	DDX_Control(pDX, IDC_BTN_DOWN, m_btnDown);
	DDX_Control(pDX, IDC_SPIN1, m_spnCond1);
	DDX_Control(pDX, IDC_EDIT1, m_edtCond1);
	DDX_Text(pDX, IDC_EDIT_BLOCKCHANGE, m_strBlockChange);
	//}}AFX_DATA_MAP
//	DDX_Control(pDX, IDC_THICKNESS, m_cmbThickness);
}


BEGIN_MESSAGE_MAP(CSetting_Special_Type_C, CBaseDialog)
	//{{AFX_MSG_MAP(CSetting_Special_Type_C)
	ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting_Special_Type_C message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CSetting_Special_Type_C::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( !m_pIGraphInfo) return TRUE;
	Initial( NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void : 
// Param  IGraphInfo *p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_C::Initial(IGraphInfo *p_pIGraphInfo)
{
	if( m_pIGraphInfo)
	{
		OnApply( m_pIGraphInfo);
		m_pIGraphInfo->Release();
	}
	m_pIGraphInfo = p_pIGraphInfo;
	if( m_pIGraphInfo) m_pIGraphInfo->AddRef();
	else return;

	m_nSubGraphIndex = 0;

	m_btnUp.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 1));
	m_btnUp.Invalidate();
	
	m_btnDown.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 3));
	m_btnDown.Invalidate();
	
//	Initial_ThicknessComboBox(m_nSubGraphIndex, 0, m_cmbThickness);

	// 20081028 �ڵ��� ��ŶŸ���� ���´�. >>
	CString strPacketType = GetPacketType();
	// 20081028 �ڵ��� <<

	// ���ǰ� ����� ���� ���� - ojtaso (20070913)
	ILPCSTR szGraphName = m_pIGraphInfo->GetIndicatorNameString();
	if( !strcmp( szGraphName, _MTEXT( C2_RENKO_CHART )))
	{
		GetDlgItem( IDC_STATIC_BLOCKCHANGE)->ShowWindow(SW_SHOW);
		GetDlgItem( IDC_EDIT_BLOCKCHANGE)->ShowWindow(SW_SHOW);

		IDoubleList *pICalCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
		if(pICalCnd)
		{
			POSITION psnCnd = pICalCnd->GetHeadPosition();
			if( psnCnd)
			{
				double dData = 0;
				pICalCnd->GetNext( psnCnd, dData);

				// 20081028 �ڵ��� ��ŶŸ�Կ� ���� ���� �����Ѵ�. >>
				//m_strBlockSize.Format("%.1f",dData);
				if(CDataConversion::IsBaseFormat(strPacketType))
				{
					// ������ȯ�� ���... ������ �������� ��ȯ�Ѵ�.
					CString strData = CDataConversion::GetStringThousandFormat(dData, strPacketType);
					m_strBlockChange = strData;
				}
				else
				{
					// ������ȯ�� �ƴ� ���...

					// ��ŶŸ�Կ��� �Ҽ��� �ڸ����� ���´�.
					CString strValue = strPacketType;
					strValue.Remove('%');

					int nPoint=0;
					int nPos = strValue.Find('.');
					if(nPos >= 0)
					{
						CString strPoint = strValue.Mid(nPos+1);
						nPoint = strPoint.GetLength();
					}

					m_strBlockChange.Format("%.*f", nPoint, dData);
				}
				//m_strBlockChange.Format("%.0f",dData);
				// 20081028 �ڵ��� <<
			}
		}
		pICalCnd->Release();
	}
	else
	{
		GetDlgItem( IDC_STATIC_BLOCKCHANGE)->ShowWindow(	SW_HIDE);
		GetDlgItem( IDC_EDIT_BLOCKCHANGE)->ShowWindow(		SW_HIDE);
	}

	UpdateData(FALSE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_C::OnBtnUp() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0)
		return;
	
	COLORREF crColor = m_btnUp.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() != IDOK) 
		return;

	m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 1, color.GetColor());
	m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 2, color.GetColor());

	m_btnUp.SetColor( color.GetColor() );
	m_btnUp.Invalidate();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_C::OnBtnDown() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnDown.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() != IDOK) 
		return;

	m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 3, color.GetColor());
	m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 4, color.GetColor());

	m_btnDown.SetColor( color.GetColor() );
	m_btnDown.Invalidate();
}




//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void	: 
// Param  IGraphInfo* p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------
// 20081014 �ڵ��� >>
BOOL CSetting_Special_Type_C::OnApply( IGraphInfo* p_pIGraphInfo )
//void CSetting_Special_Type_C::OnApply( IGraphInfo* p_pIGraphInfo )
// 20081014 �ڵ��� <<
{
	if(p_pIGraphInfo == NULL)
		return FALSE;
	
//	OnApply_Thickness( m_nSubGraphIndex, m_cmbThickness);

	// 20081028 �ڵ��� ��ŶŸ���� ���´�. >>
	CString strPacketType = GetPacketType();
	// 20081028 �ڵ��� <<

	// ���ǰ� ����� ���� ���� - ojtaso (20070913)
	ILPCSTR szGraphName = m_pIGraphInfo->GetIndicatorNameString();
	if( !strcmp( szGraphName, _MTEXT( C2_RENKO_CHART )))
	{
		IDoubleList *pICalCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
		if( pICalCnd)
		{
			POSITION psnCnd = pICalCnd->GetHeadPosition();
			if( psnCnd)
			{
				UpdateData();

				// 20081028 �ڵ��� >>
				//pICalCnd->SetAt( psnCnd, atof( m_strBlockChange));
				if(CDataConversion::IsBaseFormat(strPacketType))
				{
					// �Էµ� ���� ������ȯ�� ��ȿ���� �˻��Ѵ�. >>
					BOOL bResult = CDataConversion::IsValid8_32(m_strBlockChange);
					if(m_strBlockChange == "0") bResult = FALSE;

					if(!bResult)
					{
						AfxMessageBox("�Էµ� ���ڿ��� �˸��� ������ �ƴմϴ�.");
						
						CWnd *pWnd = GetDlgItem(IDC_EDIT_BLOCKCHANGE);
						if(pWnd && IsWindow(pWnd->GetSafeHwnd()))
						{
							pWnd->SetWindowText("");
							pWnd->SetFocus();
						}
						return FALSE;
					}

					// ������ȯ�� ���... ������ ������ ��ȯ�Ѵ�.
					CString strOutput;
					bResult = CDataConversion::GetDataToDec(m_strBlockChange, strPacketType, strOutput);

					if(bResult)
						pICalCnd->SetAt(psnCnd, atof(strOutput));
					else
						pICalCnd->SetAt( psnCnd, atof( m_strBlockChange));
				}
				else
				{
					// ������ȯ�� �ƴҰ��...
					pICalCnd->SetAt( psnCnd, atof( m_strBlockChange));
				}
				// 20081028 �ڵ��� <<

				double d;
				pICalCnd->GetHead(d);
				
			}
		}

		// (2008/10/27 - Seung-Won, Bae) for Auto Condition.
		IDoubleList *pIDrawCalCnd = m_pIGraphInfo->GetDrawConditionList();
		if( pIDrawCalCnd)
		{
			double dData = 0;
			POSITION psnCnd = pIDrawCalCnd->GetHeadPosition();
			if( psnCnd) pIDrawCalCnd->SetAt( psnCnd, atof( "1"));
		}
		pIDrawCalCnd->Release();
		
		if( !CBaseDialog::m_bDoCalculate)
		{
			IDoubleList *pICalCndOrg = p_pIGraphInfo->GetCommonCalculateConditionList();
			if( pICalCndOrg)
			{
				CBaseDialog::m_bDoCalculate = !DoesEqualDoubleList( pICalCndOrg, pICalCnd);
				pICalCndOrg->Release();
			}
		}
		
		pICalCnd->Release();

		p_pIGraphInfo->GetCopyDataFrom(m_pIGraphInfo);

		m_pIChartOCX->ReCalculateIndicator();
	}
	else
		p_pIGraphInfo->GetCopyDataFrom(m_pIGraphInfo);

	return TRUE;
}

BOOL CSetting_Special_Type_C::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if( m_pParentWnd != NULL )
			{
				if( m_pTopWnd != NULL )
					( (CDlgEnvSetUpAll *)m_pTopWnd)->OK();

				else
					( (CGraphSetting *)m_pParentWnd)->OK();
			}
		}

		
		if( pMsg->wParam == VK_ESCAPE )
		{
			if( m_pParentWnd != NULL )
			{
				if( m_pTopWnd != NULL )
					( (CDlgEnvSetUpAll *)m_pTopWnd)->Cancel();

				else
					( (CGraphSetting *)m_pParentWnd)->Cancel();
			}
		}
	}
	
	return CBaseDialog::PreTranslateMessage(pMsg);
}
