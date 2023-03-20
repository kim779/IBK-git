// IndicatorNameListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "IndicatorNameListCtrl.h"

#include "../Include_Chart/OutsideCommonInterface.h"		// for CMChartInterface
#include "../Include_Chart/OutsideCommonInterfaceFunction.h"
#include "GroupButtonCtrl.h"
#include "DlgMainTabIndicatorSetUp.h"
// 2008.01.25 by LYH >>
//#include "DefineData.h"
// 2008.01.25 by LYH <<
#include "IndiConfigurationDefineData.h"

// 2008.01.25 by LYH >>
#include "../Include_Chart/Dll_Load/IMetaTable.h"	// for _MTOL0()
// 2008.01.25 by LYH <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndicatorNameListCtrl

CIndicatorNameListCtrl::CIndicatorNameListCtrl(CGroupButtonCtrl* pParent, LPCTSTR pcszIndicatorGroupName, int nIndex, IPacketManager *p_pIPacketManager)
{
	m_pParent = pParent;
	m_strIndicatorGroupName = pcszIndicatorGroupName;
	m_nIndex = nIndex;

  	m_pIPacketManager = p_pIPacketManager;
	if( m_pIPacketManager) m_pIPacketManager->AddRef();

// 2008.01.25 by LYH >>
	m_hOcxWnd = NULL;
// 2008.01.25 by LYH <<
}

CIndicatorNameListCtrl::~CIndicatorNameListCtrl()
{
	if( m_pIPacketManager)
	{
		m_pIPacketManager->Release();
		m_pIPacketManager = NULL;
	}
}


BEGIN_MESSAGE_MAP(CIndicatorNameListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CIndicatorNameListCtrl)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_DRAWITEM()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndicatorNameListCtrl message handlers



/*-----------------------------------------------------------------------------------------
 - Function    :  InitColumn
 - Created at  :  2004-03-30   14:08
 - Author      :  ������
 - Description :  ListCtrl�� Į���� �ʱ�ȭ�Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CIndicatorNameListCtrl::InitColumn()
{
	// ( 2007 / 01 / 08 - Sang-Woo, Cho ) - CheckBox ����
	//SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
	SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	CRect rc;
	GetClientRect(rc);

	LV_COLUMN   lvColumn;
	//initialize the columns
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;// | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = rc.Width() - 17;
	lvColumn.pszText = _T("��ǥ");
	InsertColumn(0, &lvColumn);	

	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;// | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 0;
	lvColumn.pszText = _T("GraphName");
	InsertColumn(1, &lvColumn);	
}


/*-----------------------------------------------------------------------------------------
 - Function    :  Initialize
 - Created at  :  2004-03-30   14:09
 - Author      :  ������
 - Description :  ListCtrl�� Į���� �ʱ�ȭ��Ű�� IndicatorName�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CIndicatorNameListCtrl::Initialize( const char *p_szGroupName, IStringList * pGraphNameList, IStringList * pIndicatorNameList)	// ListCtrl �ʱ�ȭ�Ѵ�.
{
	InitColumn();
	
	if( pIndicatorNameList->IsEmpty()) return false;

	if(IsInitialize_Market( p_szGroupName))					return Initialize_Market( pIndicatorNameList);
	else if(IsInitialize_PacketTimeType( p_szGroupName))	return Initialize_PacketTimeType( pIndicatorNameList);

	return Initialize( pGraphNameList, pIndicatorNameList);
}

bool CIndicatorNameListCtrl::Initialize( IStringList *pGraphNameList, IStringList *pIndicatorNameList)
{
	if( pIndicatorNameList == NULL || pIndicatorNameList->IsEmpty()) return false;

	int i = 0;
	ILPCSTR szGraphName = NULL, szIndicatorName = NULL;
	POSITION posGraphName		= ( pGraphNameList ? pGraphNameList->GetHeadPosition() : NULL);
	POSITION posIndicatorName	= pIndicatorNameList->GetHeadPosition();
	while( posIndicatorName)
	{
		if( pGraphNameList && posGraphName) pGraphNameList->GetNext( posGraphName, szGraphName);
		pIndicatorNameList->GetNext( posIndicatorName, szIndicatorName);
// 2008.01.25 by LYH >>
		//InsertItem(		i,		szIndicatorName);
		InsertItem(		i,		_MTOL2(szIndicatorName));
// 2008.01.25 by LYH <<
		SetItemText(	i, 1,	szGraphName);
		i++;
		szGraphName = NULL;
	}		
	return (i > 0);
}

// -> [�����ڵ���], [��ü] �� ���.
//    �ֽ�/����/����/�ɼ�, ��/��/��/��/ƽ �� ���� ��ǥ �߰� �׸� ����.
bool CIndicatorNameListCtrl::Initialize_PacketTimeType( IStringList * pList)	// ListCtrl �ʱ�ȭ�Ѵ�.
{
	if(pList == NULL || pList->IsEmpty())
		return false;

	IPacketManager::ICOMMODITY_BOND_NAME eBondName = IPacketManager::INONAME;
	CScaleBaseData::HORZSCALEDRAWERTYPE eTimeType = CScaleBaseData::HORZ_NOTYPE;
	GetDataInPacket( eBondName, eTimeType);

	int i = 0;
	ILPCSTR szGraphName = NULL;
	POSITION pos = pList->GetHeadPosition();
	while(pos)
	{
		pList->GetNext( pos, szGraphName);
		// �ֽ�/����/����/�ɼ�, ��/��/��/��/ƽ �� ���� ��ǥ �߰� �׸� ����.
		if(IsAddIndicatorName( eBondName, eTimeType, szGraphName))
			InsertItem(i++, szGraphName);
	}
	return (i > 0);
}

// -> [������ǥ] �� ���.
// -> ��ȭ�� ��� �ֽ�, �ϰ� �϶��� �߰�
bool CIndicatorNameListCtrl::Initialize_Market( IStringList *pList)
{
	if(pList == NULL || pList->IsEmpty())
		return false;

// (2009/6/22 - Seung-Won, Bae) Check Market Index.
//	Do not check it. It was checked with RMSG_CAN_ADD_MARKET_INDEX.
//	// 1. �ܺ� dll�� ���� Ȯ��.
//	if( !IsAddIndicatorName_DLL())
//		return false;

	// 2. packet type�� Ȯ��.
	if( !IsAddIndicatorName()) return false;

	int i = 0;
	ILPCSTR szGraphName = NULL;
	POSITION pos = pList->GetHeadPosition();
	while(pos)
	{
		pList->GetNext(pos, szGraphName);
		InsertItem(i++, _MTOL2( szGraphName));
	}
	return (i > 0);
}

void CIndicatorNameListCtrl::GetDataInPacket( IPacketManager::ICOMMODITY_BOND_NAME& eBondName, CScaleBaseData::HORZSCALEDRAWERTYPE& eTimeType)
{
	if( m_pIPacketManager == NULL)
		return;

	eBondName = m_pIPacketManager->GetCommodityBondNameInBase();
	eTimeType = m_pIPacketManager->GetBaseTimeDataType();
}

bool CIndicatorNameListCtrl::IsInitialize_Market(const CString& strGroupName) const
{
	//sy 2005.11.17.
	return (strGroupName == CIndiConfigurationDefineData::GROUPNAME_MARKET);
}

bool CIndicatorNameListCtrl::IsInitialize_PacketTimeType(const CString& strGroupName) const
{
	//sy 2005.11.01.
	//return (trGroupName == CIndiConfigurationDefineData::GROUPNAME_INVESTOR || 
	//	strGroupName == CIndiConfigurationDefineData::GROUPNAME_TOTAL);
	return (strGroupName == CIndiConfigurationDefineData::GROUPNAME_INVESTOR);
}

//sy 2005.11.17. �ܺ� dll�� ���� Ȯ��.
bool CIndicatorNameListCtrl::IsAddIndicatorName_DLL()
{
	if(m_pParent == NULL)
		return false;

	CDlgMainTabIndicatorSetUp* pTabDlg = m_pParent->GetParent();
	if(pTabDlg == NULL)
		return false;

	COutsideCommonInterfaceFunction outsideIF;
	// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface
	pTabDlg->GetMChartInterface( outsideIF);
	return outsideIF.Outside_IsLoad_MarketChart();
}

// �ֽ� �� �� �� ���� ���� ��ǥ.
bool CIndicatorNameListCtrl::IsAddIndicatorName( void)
{
	IPacketManager::ICOMMODITY_BOND_NAME eBondName = IPacketManager::INONAME;
	CScaleBaseData::HORZSCALEDRAWERTYPE eTimeType = CScaleBaseData::HORZ_NOTYPE;
	GetDataInPacket( eBondName, eTimeType);
	if(eBondName == IPacketManager::INONAME) return true;

	if(!IsStock(eBondName)) return false;
	
	return IsDaily(eTimeType);
}

// �ֽ�/����/����/�ɼ� �� ��/��/��/��/ƽ �� ���� ���� ��ǥ.
bool CIndicatorNameListCtrl::IsAddIndicatorName( const IPacketManager::ICOMMODITY_BOND_NAME eBondName, 
		const CScaleBaseData::HORZSCALEDRAWERTYPE eTimeType, const char *p_szGraphName)
{
	if( !p_szGraphName) return false;
	if( !*p_szGraphName) return false;

	if(eBondName == IPacketManager::INONAME) return true;

	if(CIndiConfigurationDefineData::IsIndicatorNameInStocksDaily(p_szGraphName))
	{
		if(!IsStock(eBondName))
			return false;
		
		return IsDaily(eTimeType);
	}
	// [����/�ɼ� �̰��� ����] -> ����/�ɼ�, ��/��/ƽ
	else if( !strcmp( p_szGraphName, CIndiConfigurationDefineData::INDINAME_FO_UNAPPROVAL))
	{
		if(!IsFuturesOrOption(eBondName))
			return false;
		
		return IsDailyOrTimeOrTick(eTimeType);
	}
	// [�����ŷ�������] -> ����/�ɼ�, ��
	else if( !strcmp( p_szGraphName, CIndiConfigurationDefineData::INDINAME_FO_PRICE))
	{
		if(!IsFuturesOrOption(eBondName))
			return false;
		
		return IsDaily(eTimeType);
	}

	return true;
}

//-> �ֽ�
bool CIndicatorNameListCtrl::IsStock(const IPacketManager::ICOMMODITY_BOND_NAME eBondName) const
{
	return (eBondName == CPacketType::STOCK);
}

//-> ����/�ɼ�
bool CIndicatorNameListCtrl::IsFuturesOrOption(const IPacketManager::ICOMMODITY_BOND_NAME eBondName) const
{
	return (eBondName == CPacketType::FUTURES || eBondName == CPacketType::OPTION);
}

//-> ��/��/��
bool CIndicatorNameListCtrl::IsDailyOrWeekleyOrMonthly(const CScaleBaseData::HORZSCALEDRAWERTYPE eTimeType) const
{
	return (eTimeType == CScaleBaseData::HORZ_DAILY || 
			eTimeType == CScaleBaseData::HORZ_WEEKLEY || 
			eTimeType == CScaleBaseData::HORZ_MONTHLY);
}

//-> ��/��/ƽ
bool CIndicatorNameListCtrl::IsDailyOrTimeOrTick(const CScaleBaseData::HORZSCALEDRAWERTYPE eTimeType) const
{
	return (eTimeType == CScaleBaseData::HORZ_DAILY || 
			eTimeType == CScaleBaseData::HORZ_TIME_ONE || 
			eTimeType == CScaleBaseData::HORZ_TIME_TWO || 
			eTimeType == CScaleBaseData::HORZ_TICK);
}

//-> ��
bool CIndicatorNameListCtrl::IsDaily(const CScaleBaseData::HORZSCALEDRAWERTYPE eTimeType) const
{
	return (eTimeType == CScaleBaseData::HORZ_DAILY);
}
//sy end


/*-----------------------------------------------------------------------------------------
 - Function    :  SetCheckSeletecItem
 - Created at  :  2004-03-31   09:20
 - Author      :  ������
 - Description :  ���� ��Ʈ�� ǥ�õ� ��ǥ���� ListCtrl�� �ִ� �������̸� üũǥ�ø� �Ѵ�.(�����ҽ��ο�)
 -----------------------------------------------------------------------------------------*/
void CIndicatorNameListCtrl::SetCheckSeletecItem( IStringList *p_pIGraphNameList)
{
	// ����(04/08/04) �ʱ⼳������ ���� ��ǥ�� üũǥ���Ҷ� 
	// ��ǥ�߰�������ƾ�� ȣ����� �ʰ� ��. T_T �ʽ��ۿ�Ʈ��ĸ��¯ �����ϴ�...��!!
	m_bInitCheckIndicator = TRUE;

	ILPCSTR szGraphName = NULL;
	POSITION pos = p_pIGraphNameList->GetHeadPosition();
	while(pos)
	{
		p_pIGraphNameList->GetNext( pos, szGraphName);
		for(int i = 0; i < GetItemCount(); i++)
		{
			if( !strcmp( szGraphName, GetItemText(i, 0)))
			{
				SetCheck(i);
				break;
			}
		}//end of for
	}//end of while(pos)
	m_bInitCheckIndicator = FALSE;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetAllUnCheck
 - Created at  :  2005-01-24   10:55
 - Author      :  ������
 - Description :  ��ǥ�� ListCtrl�� ��� �������� UnCheck�Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CIndicatorNameListCtrl::SetAllUnCheck()
{
	// Check/UnCheck �ɶ� ��ǥ�� �߰� ���� �Ǵ°��� �����Ѵ�. -> OnItemChanged()�Լ� ����.
	m_bInitCheckIndicator = TRUE;
	for(int i = 0; i < GetItemCount(); i++)
		SetCheck(i, FALSE);
	m_bInitCheckIndicator = FALSE;
}

	
/*-----------------------------------------------------------------------------------------
 - Function    :  SetFocusItem
 - Created at  :  2004-03-31   10:09
 - Author      :  ������
 - Description :  ��Ŀ���� ��.
 -----------------------------------------------------------------------------------------*/
void CIndicatorNameListCtrl::SetFocusItem(const int nIndex)
{
	if(nIndex != -1) 
	{
		SetItemState( m_nSelectItemIndex, 0, LVIF_STATE); 
		m_nSelectItemIndex = nIndex;
		SetItemState( m_nSelectItemIndex, ~LVIS_SELECTED | LVIS_FOCUSED, LVIF_STATE); 
	}
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseFocusItem
 - Created at  :  2004-03-31   11:37
 - Author      :  ������
 - Description :  ��Ŀ���� ǰ.
 -----------------------------------------------------------------------------------------*/
void CIndicatorNameListCtrl::ReleaseFocusItem()
{
	SetItemState(m_nSelectItemIndex, LVIS_SELECTED , LVIF_STATE);	
	
	// ( 2007 / 01 / 09 - Sang-Woo, Cho ) 
	m_nSelectItemIndex = -1;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnItemchanged
 - Created at  :  2004-03-31   14:11
 - Author      :  ������
 - Description :  ������ ���� �����.
 - Update	   :  (05/01/24) ������Ʈ�� UnCheck���� �ʵ��� ��.
 -----------------------------------------------------------------------------------------*/
void CIndicatorNameListCtrl::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	if (pNMListView->uOldState == 0 && pNMListView->uNewState == 0)
		return;	// No change

	// Old check box state
	BOOL bPrevState = (BOOL)(((pNMListView->uOldState & LVIS_STATEIMAGEMASK)>>12)-1);   // Old check box state
	if (bPrevState < 0)	// On startup there's no previous state 
		bPrevState = 0; // so assign as false (unchecked)

	// New check box state
	BOOL bChecked=(BOOL)(((pNMListView->uNewState & LVIS_STATEIMAGEMASK)>>12)-1);   
	if (bChecked < 0) // On non-checkbox notifications assume false
		bChecked = 0; 

	if (bPrevState == bChecked) // No change in check box
		return;

	int iItem = pNMListView->iItem; 

	// ����(04/08/04) �ʱ⼳������ ���� ��ǥ�� üũǥ���Ҷ� 
	// ��ǥ�߰�������ƾ�� ȣ����� �ʰ� ��. T_T �ʽ��ۿ�Ʈ��ĸ��¯ �����ϴ�...��!!
	// ����(05/01/19) ��ǥ �߰�/���� �κ��� ���⼭ ���� ȣ���ϰ� ��.
	if(!m_bInitCheckIndicator)
	{
		if(bChecked)	// �׷��� �߰�
		{
			if(GetItemText(iItem, 0) == _LTEXT2( C2_PRICE_CHART ))	// ������Ʈ�� �ƹ��� ������ ���� �ʴ´�.
				return;

			if( !m_pParent)
			{
				CString strGraphName;
				m_pParent->GetParent()->AddGraph( GetItemText( iItem, 0), strGraphName);
			}
		}
		else			// �׷��� ����
		{
			if(GetItemText(iItem, 0) == _LTEXT2( C2_PRICE_CHART ))
			{	// ������Ʈ�� ������ �ʴ´�. �׻� üũ���� ����.
				SetCheck(iItem);
				return;
			}
			
			if( m_pParent != NULL) m_pParent->GetParent()->RemoveGraph( GetItemText( iItem, 1));
		}//end of if..else..
	}//end of if(!m_bInit...
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnClick
 - Created at  :  2004-03-31   14:11
 - Author      :  ������
 - Description :  ������ Ŭ����
 -----------------------------------------------------------------------------------------*/
void CIndicatorNameListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(m_nSelectItemIndex != pNMListView->iItem) 
	{
		// ( 2007 / 01 / 09 - Sang-Woo, Cho ) 
		ChangeFocusItem();

		if( pNMListView->iItem == -1 )
			return;
		SetItemState(m_nSelectItemIndex, LVIS_SELECTED, LVIF_STATE); 
		SetFocusItem(pNMListView->iItem);
	}
	else 
		SendMessage(WM_KILLFOCUS);
	
	if(m_pParent != NULL) if( m_pParent->GetGroupButtonCtrlType() == USE_GROUP)
	{
		CString szIndicatorName = GetItemText( m_nSelectItemIndex, 0);
// 2008.01.25 by LYH >>
		//m_pParent->GetParent()->SelChangeGraph( szIndicatorName, GetItemText( m_nSelectItemIndex, 1));
		m_pParent->GetParent()->SelChangeGraph( _LTOM2(szIndicatorName), GetItemText( m_nSelectItemIndex, 1));
// 2008.01.25 by LYH <<

		if( szIndicatorName.Find( '&' ) ) szIndicatorName.Replace("&", "&&");
		m_pParent->GetParent()->SetDlgItemText( IDC_STATIC_INDICATOR_NAME, szIndicatorName);
	}

	*pResult = 0;
}



/*-----------------------------------------------------------------------------------------
 - Function    :  GetCheckState
 - Created at  :  2004-03-31   17:11
 - Author      :  ������
 - Description :  �������� üũ���¸� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
BOOL CIndicatorNameListCtrl::GetCheckState(const CString &strIndicatorName, int& nIndex)
{
	for(int i = 0 ; i< GetItemCount() ; i++) {
		if(strIndicatorName.CompareNoCase(GetItemText(i, 0)) == 0) {
			nIndex = i;
			return GetCheck(i);
		}
	}
	nIndex = -1;
	return FALSE;	
}

void CIndicatorNameListCtrl::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	this->RedrawWindow();
	CListCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnDblclk
 - Created at  :  2004-07-08   09:15
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Description :  ���� Ŭ���� ������Ʈ�� UnCheck�Ǵ� ���׸� ���´�.
 -----------------------------------------------------------------------------------------*/
void CIndicatorNameListCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	ML_SET_LANGUAGE_RES();
	// ( 2007 / 01 / 09 - Sang-Woo, Cho ) 
	CIndicatorNameListCtrl *pUseGroupIndicatorNameListCtrl = NULL;

	CString strIndicatorName = _LTOM2(GetItemText(pNMListView->iItem, 0));
	if( m_pParent->GetGroupButtonCtrlType() == GENERAL_GROUP )
	{
		if(strIndicatorName == _MTEXT( C2_PRICE_CHART ))
		{
			//AfxMessageBox( "������Ʈ�� �ߺ� �߰��� �� �����ϴ�.");
			AfxMessageBox( IDS_MSG_PRICECHART_ADD );
			return;
		}

		if(m_pParent != NULL)
		{
			pUseGroupIndicatorNameListCtrl = (m_pParent->GetParent()->m_ctrlUseGroupButton).GetSelectListCtrl();

			if( pUseGroupIndicatorNameListCtrl != NULL )
			{
				if( !strIndicatorName.IsEmpty())
				{
					CString strGraphName;
					m_pParent->GetParent()->AddGraph( strIndicatorName, strGraphName);
					if( !strGraphName.IsEmpty())
					{
						pUseGroupIndicatorNameListCtrl->AddGraph( strIndicatorName, strGraphName);

						// (2007/4/23 - Seung-Won, Bae) Use Correct Names. 
						//		m_pParent->GetParent()->SelChangeGraph( GetItemText(m_nSelectItemIndex, 0), GetItemText(m_nSelectItemIndex, 1));
						m_pParent->GetParent()->SelChangeGraph( strIndicatorName, strGraphName);
					}
				}
			}
			
		}
	}

	else if( m_pParent->GetGroupButtonCtrlType() == USE_GROUP )
	{
		if( strIndicatorName == _MTEXT(C2_PRICE_CHART))
		{
			AfxMessageBox( IDS_MSG_PRICECHART_DEL );
			return;
		}
		
		if(m_pParent != NULL)
		{
			if( pNMListView->iItem > GetItemCount()) m_nSelectItemIndex = GetItemCount();
			else m_nSelectItemIndex = pNMListView->iItem;
// 2008.01.25 by LYH >>
			//if( m_pParent != NULL) m_pParent->GetParent()->RemoveGraph( GetItemText( m_nSelectItemIndex, 1));
			if( m_pParent != NULL) m_pParent->GetParent()->RemoveGraph( _LTOM2(GetItemText( m_nSelectItemIndex, 1)));
// 2008.01.25 by LYH <<
		}
	}

	*pResult = 0;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2007 / 01 / 09
// Return void	: 
// Param  void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CIndicatorNameListCtrl::ChangeFocusItem( void )
{
	CIndicatorNameListCtrl *pOtherIndicatorNameListCtrl = NULL;

	if( m_pParent->GetGroupButtonCtrlType() == GENERAL_GROUP )		// General
	{
		pOtherIndicatorNameListCtrl = (m_pParent->GetParent()->m_ctrlUseGroupButton).GetSelectListCtrl();
		CButton *pBtnWithDefault = (CButton *)(m_pParent->GetParent()->GetDlgItem(IDC_BTN_WITH_DEFAULT));
		pBtnWithDefault->ShowWindow( SW_HIDE );
	}

	else if( m_pParent->GetGroupButtonCtrlType() == USE_GROUP )
	{
		pOtherIndicatorNameListCtrl = (m_pParent->GetParent()->m_ctrlGroupButton).GetSelectListCtrl();
		CButton *pBtnWithDefault = (CButton *)(m_pParent->GetParent()->GetDlgItem(IDC_BTN_WITH_DEFAULT));
		// (2006/1/24 - Seung-Won, Bae) Hide Default Value Setting. 
		pBtnWithDefault->ShowWindow( SW_HIDE);	// SW_SHOW );
	}

	pOtherIndicatorNameListCtrl->ReleaseFocusItem();
	pOtherIndicatorNameListCtrl->SendMessage(WM_KILLFOCUS);
}


BOOL CIndicatorNameListCtrl::AddGraph( const CString &strIndicatorName, const CString &p_strGraphName)
{
	if( !strIndicatorName.IsEmpty() )
	{
		int nIndex = GetItemCount();
// 2008.01.25 by LYH >>
		//InsertItem( nIndex, strIndicatorName);
		InsertItem( nIndex, _MTOL2(strIndicatorName) );
// 2008.01.25 by LYH <<
		SetItemText( nIndex, 1, p_strGraphName);

		NM_LISTVIEW nmListView;
		SetFocusItem( nIndex);
		SetSelectItemIndex(	nIndex);
		nmListView.iItem = nIndex;
		long lResult = 0;
		OnClick( ( NMHDR *)&nmListView, &lResult);

		return TRUE;
	}

	return FALSE;
}

BOOL CIndicatorNameListCtrl::RemoveGraph( int p_nIndex)
{
	if( 0 <= p_nIndex && p_nIndex < GetItemCount())
	{
		DeleteItem( p_nIndex);
		if( p_nIndex <= m_nSelectItemIndex) m_nSelectItemIndex--;
		return TRUE;
	}

	return FALSE;
}