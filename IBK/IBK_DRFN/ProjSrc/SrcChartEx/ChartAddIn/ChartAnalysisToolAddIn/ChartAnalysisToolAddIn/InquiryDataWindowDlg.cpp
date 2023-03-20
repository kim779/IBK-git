// InquiryDataWindowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InquiryDataWindowDlg.h"

#include <math.h>
#include "../Include_Chart/Dll_Load/IMetaTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT RMSG_INQUIRY_DATA_EXPORT = ::RegisterWindowMessage( "RMSG_INQUIRY_DATA_EXPORT");

/////////////////////////////////////////////////////////////////////////////
// CInquiryDataWindowDlg dialog

CInquiryDataWindowDlg::CInquiryDataWindowDlg( IChartOCX *p_pIChartOCX, CWnd* pParent)
	: CNumericalInquiryDlg(CInquiryDataWindowDlg::IDD, pParent),
	  m_niDataQueryer( p_pIChartOCX)
{
	// (2008/10/30 - Seung-Won, Bae) for Inquiry Data Export.
	m_hExportWnd = NULL;

	//{{AFX_DATA_INIT(CInquiryDataWindowDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bCalculateSize = false;

	// (2006/6/27 - Seung-Won, Bae) Manage Interfaces
	m_pIChartOCX = p_pIChartOCX;
	m_pIChartManager = NULL;
	if( m_pIChartOCX)
	{
		m_pIChartOCX->AddRef();
		m_pIChartManager = m_pIChartOCX->GetIChartManager();
	}

	m_bUseNumericalInquiryDlg = false;
	m_bInitDialogMove = false;
	m_bNumericalInquiryDlgPosLeft = true;
	m_bCalcSizeInquiryDlg = false;	// ��ġ��ȸâ����� ����ߴ��� �ľ��ϴ� ����.
	m_nMoveNumericalInquiryDlg = 0;		// �̵�.
	m_clrInquiryDlg = RGB(255, 255, 255);

	// (2006/7/2 - Seung-Won, Bae) Using Virtual Draw. 0:Title, 1:Data
	m_nListDataSize = 0;
	
	m_hOcxWnd = ( m_pIChartOCX ? m_pIChartOCX->GetOcxHwnd() : NULL);
}

CInquiryDataWindowDlg::~CInquiryDataWindowDlg()
{
	// (2008/10/30 - Seung-Won, Bae) for Inquiry Data Export. (2: Do not use m_saListData.)
	if( m_hExportWnd)
		::SendMessage( m_hExportWnd, RMSG_INQUIRY_DATA_EXPORT, 2, ( LPARAM)m_saListData);

	m_listData = NULL;

	// (2006/6/27 - Seung-Won, Bae) Manage Interfaces
	if( m_pIChartOCX) m_pIChartOCX->Release();
	if( m_pIChartManager) m_pIChartManager->Release();
}

// (2006/2/15 - Seung-Won, Bae) for checking Runtime Class ~ Not used yet in 2006/2/15
IMPLEMENT_DYNAMIC( CInquiryDataWindowDlg, CNumericalInquiryDlg)

void CInquiryDataWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInquiryDataWindowDlg)
	DDX_Control(pDX, IDC_LIST_INQUIRYDATA, m_listctrlInquiryData);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInquiryDataWindowDlg, CNumericalInquiryDlg)
	//{{AFX_MSG_MAP(CInquiryDataWindowDlg)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_INQUIRYDATA, OnGetdispinfoListInquirydata)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInquiryDataWindowDlg message handlers

HBRUSH CInquiryDataWindowDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CNumericalInquiryDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkMode(TRANSPARENT);

	return m_dlgBrush;
}

BOOL CInquiryDataWindowDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// COLUMN �ʱ�ȭ.
	LV_COLUMN lc ;
	lc.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH ;
	lc.fmt = LVCFMT_LEFT; 

	lc.pszText = _T("����");
	lc.cx = 50;
	m_listctrlInquiryData.InsertColumn(0, &lc);
	
	lc.pszText = _T("��");
	lc.cx = 50;
	m_listctrlInquiryData.InsertColumn(1, &lc);

	// ListCtrl ��Ÿ�� �߰�.	
	m_listctrlInquiryData.SetExtendedStyle(m_listctrlInquiryData.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_bCalculateSize = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*-------------------------------------------------------------------------------
 - Function    :  SetInquiryData
 - Created at  :  2005-03-22   15:50
 - Author      :  ������
 - Parameters  :  pInquiryData - ��ȸ����Ÿ�� ����ִ� �����̳�.
				  nMaxDecimal - ��ġ��ȸ �Ҽ��� �ڸ���(�ִ�)
 - Description :  ��ȸ����Ÿ�� ��ȸâ�� �����Ѵ�.
 -------------------------------------------------------------------------------*/
void CInquiryDataWindowDlg::SetInquiryData(CDC* pDC, CInquiryData *pInquiryData,CInquiryData *pInquiryPrevData,int nMaxDecimal /* = 0*/)
{
	if( pInquiryData->IsEmpty()) return;
	if( pInquiryPrevData->IsEmpty()) return;
	// _LISTDATA�� �ϳ��� ����Ѵ�. �׷��Ƿ� �̸� �������°� ����! (Multi Line�� ����������ϴϱ� For��������!)
	m_listData = pInquiryData->GetHead();

	CInquiryItem* listPrevData = pInquiryPrevData->GetHead();
	// ���� List Ctrl�� ������ ������ �����´�. (���� ������ ListCtrl���� ������ ���������� ����� �ǰ�
	int nPrevItemCount = m_listctrlInquiryData.GetItemCount();	

	// ����Ÿ�� ListCtrl�� ����.
	LV_ITEM ltc;
	ltc.mask = LVIF_TEXT;

	// (2006/7/2 - Seung-Won, Bae) Using Virtual Draw. 0:Title, 1:Data
	int nIndex = 0;
	CRowData *pRowData = NULL;
	CRowData *pRowPrevData = NULL;
	CString strItemData;
	double dItemData = 0.0;
	double dPrevItemData = 0.0;
	double dPrevVolumeData = 0.0;	//>>[A00000441] ������ ��ġ��ȸ�� ���ϴ�� ������߰�.
	double dItemRatio = 0.0;
	double dItemContrast = 0.0;
	CString strConSymbol;
	CString strTempData;
	POSITION posRow = m_listData->GetHeadPosition();
	POSITION posPrevRow = listPrevData->GetHeadPosition();
	while( posPrevRow)
	{
		pRowPrevData = listPrevData->GetNext( posPrevRow);
		
		strTempData = pRowPrevData->m_strData;
		strTempData.Replace(",","");
		if(pRowPrevData != NULL && 
			(pRowPrevData->m_strName == _LTEXT0( C0_CLOSE) ||
			 pRowPrevData->m_strName == _LTEXT3( C3_PRICE)))
		{
			strTempData = pRowPrevData->m_strData;
			strTempData.Replace(",","");
			dPrevItemData = atof(strTempData);
		}
		else if(pRowPrevData != NULL &&
			(pRowPrevData->m_strName == _LTEXT3( C0_VOLUME) ))
		{
			strTempData = pRowPrevData->m_strData;
			strTempData.Replace(",","");
			dPrevVolumeData = atof(strTempData);
		}
	}

	double dCurItemData = 0.0;
	CString szPreStrName;
	int	 nMAState=0;	//0:None, 1:Price, 2:Volume
	BOOL bFindPrice = FALSE, bIsNumeric;
	//<<[A00000441]

	while( posRow)
	{
		pRowData = m_listData->GetNext( posRow);
		
		m_saListData[ 0].SetAtGrow( nIndex,ReplaceTitle(pRowData->m_strName));
		strTempData = pRowData->m_strData;
		strTempData.Replace(",","");
		dItemData = atof(strTempData);

		//>>[A00000441] ������ ��ġ��ȸ�� ���ϴ�� ������߰�.
		if(atoi(pRowData->m_strName) > 0) 
		{			
			if(	 szPreStrName == _LTEXT0( C0_OPEN) ||
				szPreStrName == _LTEXT0( C0_HIGH) ||
				szPreStrName == _LTEXT0( C0_LOW) ||
				szPreStrName == _LTEXT0( C0_CLOSE) ||
				szPreStrName == _LTEXT3( C3_PRICE)
				)
				nMAState = 1;
			else if(pRowData->m_strName == _LTEXT0( C0_VOLUME))
				nMAState = 2;
		}
		else 
		{
			szPreStrName = pRowData->m_strName;
			nMAState = 0;
		}
		
		if(pRowData->m_strName == _LTEXT0( C0_CLOSE)) {
			strTempData = pRowData->m_strData;
			strTempData.Replace(",","");
			
			dCurItemData = atof(strTempData);
		}		

		if(atoi(pRowData->m_strName) > 0) bIsNumeric = TRUE;
		else bIsNumeric = FALSE;		
		
		if( pRowData->m_strName ==_LTEXT0( C0_CLOSE) ) {
			strTempData = pRowData->m_strData;
			strTempData.Replace(",","");
			
			dCurItemData = atof(strTempData);
			bFindPrice = TRUE;
		}
		else if(bFindPrice && bIsNumeric)
			;
		else {
			bFindPrice = FALSE;
			}

		if(dItemData != 0.0 && 
			(pRowData->m_strName == _LTEXT0( C0_OPEN) ||
			 pRowData->m_strName == _LTEXT0( C0_HIGH) ||
			 pRowData->m_strName == _LTEXT0( C0_LOW) ||
			 pRowData->m_strName == _LTEXT0( C0_CLOSE) ||
			 pRowData->m_strName == _LTEXT3( C3_PRICE)) ||
			 (bFindPrice && bIsNumeric))
		{
			//dItemRatio = (dItemData-dPrevItemData)/fabs(dPrevItemData)*100;
			//strItemData.Format("%s",ReFormData( pRowData->m_strData,dItemRatio,nMaxDecimal));
			if( dPrevItemData <= 0.0)
			{
				dItemRatio = 0.0;
				dItemContrast = 0.0;
			}
			else
			{
				//dItemRatio = (dItemData-dPrevItemData)/fabs(dPrevItemData)*100;
				if(nMAState==1)
				{
                                        if(dCurItemData != 0)//tour2k
					dItemRatio = (dItemData-dCurItemData)/fabs(dCurItemData)*100;
					dItemContrast = dItemData-dCurItemData;
				}
				else
				{
                                        if (dPrevItemData != 0)//tour2k
					dItemRatio = (dItemData-dPrevItemData)/fabs(dPrevItemData)*100;
					dItemContrast = dItemData-dPrevItemData;
				}
			}

			if( dItemContrast == 0 )
				strConSymbol = _T("    ");
			else if( dItemContrast > 0 )
				strConSymbol = _T("��");
			else
				strConSymbol = _T("��");
			dItemContrast = fabs( dItemContrast );	

			if(nMAState==1)
			{
				if(strTempData.Right(1) == _T("%"))
					strItemData.Format("%s",ReFormData( pRowData->m_strData, nMaxDecimal));
				else
					strItemData.Format("%s",ReFormData( pRowData->m_strData,dItemRatio,nMaxDecimal));
			}
			else
			{
				if( pRowData->m_strData.Find( "." ) > - 1 )
					strItemData.Format("%s %s%.2f (%.2f%%)", pRowData->m_strData, strConSymbol, dItemContrast, dItemRatio);
				else
					strItemData.Format("%s %s%.0f (%.2f%%)", pRowData->m_strData, strConSymbol, dItemContrast, dItemRatio);	
			}				
		}
		else if(dItemData != 0.0 &&
			(pRowData->m_strName == _LTEXT0( C0_VOLUME)) ||
			(nMAState==2) )
		{
			if( dPrevVolumeData <= 0.0)
				dItemRatio = 0.0;
			else
				dItemRatio = (dItemData-dPrevVolumeData)/fabs(dPrevVolumeData)*100 + 100;
			strItemData.Format("%s",ReFormData( pRowData->m_strData,dItemRatio,nMaxDecimal));
		}
		//<<[A00000441]
		else
		{
			strItemData.Format("%s",ReFormData( pRowData->m_strData, nMaxDecimal));
		}
		
		m_saListData[ 1].SetAtGrow( nIndex,strItemData);		
		nIndex++;
	}
	m_nListDataSize = nIndex;
	m_listctrlInquiryData.SetItemCount( m_nListDataSize);

	// (2008/11/6 - Seung-Won, Bae) 
	m_saListData[ 0].SetSize( m_nListDataSize);
	m_saListData[ 1].SetSize( m_nListDataSize);
}


//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/7/2
// Modifier		: Use Virtual Draw.
// Comments		: 
//-----------------------------------------------------------------------------
void CInquiryDataWindowDlg::OnGetdispinfoListInquirydata(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
	
	LV_ITEM *pItem = &( pDispInfo->item);
	if( !( pItem->mask & LVIF_TEXT) || pItem->iSubItem < 0 || 1 < pItem->iSubItem || pItem->iItem < 0
		|| m_nListDataSize <= pItem->iItem) return;
	
	pItem->pszText = ( char *)(const char *)m_saListData[ pItem->iSubItem][ pItem->iItem];
}

/*-------------------------------------------------------------------------------
 - Function    :  MoveWindow
 - Created at  :  2005-03-28   11:19
 - Author      :  ������
 - Description :  ��ġ��ȸâ�� �̵���Ų��. ���Ҿ� ListCtrl�� �̵���Ų��.
 -------------------------------------------------------------------------------*/
void CInquiryDataWindowDlg::MoveWindow(LPCRECT lpRect, BOOL bRepaint /*= TRUE*/, CDC* pDC /*= NULL*/)
{
	// 1. List Ctrl ������ ����.
	CRect rect;
	this->GetClientRect(&rect);
	rect.right = lpRect->right - lpRect->left;
	rect.bottom = lpRect->bottom - lpRect->top;
	m_listctrlInquiryData.MoveWindow(&rect);

	if(!m_bCalculateSize && pDC != NULL)
	{
		// 2. Column Width ����.
		SetColumnWidth(0, pDC);
		SetColumnWidth(1, pDC);
		m_bCalculateSize = true;
	}

	// 3. Window ������ ����.
	CNumericalInquiryDlg::MoveWindow(lpRect, bRepaint);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  MoveWindow
 - Created at  :  2005-04-06   09:40
 - Author      :  ������
 - Description :  ��ġ��ȸâ�� �̵���Ų��.
 -----------------------------------------------------------------------------------------*/
void CInquiryDataWindowDlg::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint /* = TRUE */)
{
	CInquiryDataWindowDlg::MoveWindow(CRect(x, y, x+nWidth, y+nHeight), bRepaint);
}

/*-------------------------------------------------------------------------------
 - Function    :  SetColumnWidth
 - Created at  :  2005-03-23   14:07
 - Author      :  ������
 - Parameters  :  nColIndex - Į���ε���
 -				  pDC		- OCX�� dc.
 - Return Value:  int       - Į��������.
 - Description :  Į���� �ʺ� �������Ѵ�.
 -------------------------------------------------------------------------------*/
int CInquiryDataWindowDlg::SetColumnWidth(int nColIndex, CDC* pDC)
{	
	CString strMaxString = GetMaxColumnString( m_listData, nColIndex);
	
	CFont* pOldFont = (CFont*)pDC->SelectObject(m_pFont);

	CSize size;
	::GetTextExtentPoint32(pDC->m_hDC, _T("0"), 1, &size);

	// Column Width ���.
	int nColumnWidth = 0;
	if(nColIndex == 0)
		nColumnWidth = (strMaxString.GetLength() + 4) * size.cx;
	else
		nColumnWidth = (strMaxString.GetLength() + 4 + 6 + 2)  * size.cx;
	
	if(nColIndex == 1)
		nColumnWidth += 20;	// ���� �����ֱ�.tour2k

	m_listctrlInquiryData.SetColumnWidth(nColIndex, nColumnWidth);

	pDC->SelectObject(pOldFont);

	return nColumnWidth;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetDlgBrush
 - Created at  :  2005-04-08   16:44
 - Author      :  ������
 - Description :  ListCtrl�� ������ ������.
 -----------------------------------------------------------------------------------------*/
void CInquiryDataWindowDlg::SetDlgBrush(COLORREF clrDlgBrush)
{
	m_listctrlInquiryData.SetBkColor(clrDlgBrush);
	m_listctrlInquiryData.SetTextBkColor(clrDlgBrush);

	CNumericalInquiryDlg::SetDlgBrush(clrDlgBrush);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  ReplaceTitle
 - Created at  :  2005-04-11   13:58
 - Author      :  ������
 - Return Value:  CString - �ٲ� ���ڿ�
 - Description :  ��ġ��ȸâ�� ǥ���ϱ⿡ �ʹ� �� Ÿ��Ʋ�� ���ڷ� �ٲپ� ǥ���ϱ� ���� 
				  Ÿ��Ʋ���� �ٲ۴�.
 -----------------------------------------------------------------------------------------*/
CString CInquiryDataWindowDlg::ReplaceTitle(const CString& strTitle)
{
	if(strTitle == _T("MACD Oscillator"))
		return _T("MACD Osc.");
	if(strTitle == _T("MA Oscillator"))
		return _T("MA Osc.");
	
	return strTitle;
}

/*-------------------------------------------------------------------------------
 - Function    :  OnClose
 - Created at  :  2005-04-13   15:31
 - Author      :  ������
 - Description :  ��ġ��ȸâ ��ü�� �������� �θ�Ŭ������ �̺�Ʈ�Լ��� ȣ������ �ʰ�
				  CToolBarManagerImplementation::DestroyNumericalInquiryDlg()�� 
				  ȣ�����ָ� ������ ��ȸâ �ݴ� ������ ������ ���!�ϰ� �ذ��Ѵ�.
 -------------------------------------------------------------------------------*/
void CInquiryDataWindowDlg::OnClose() 
{
	DestroyNumericalInquiryDlg();
//	CDialog::OnClose();
}

/*-------------------------------------------------------------------------------
 - Function    :  OnMouseMove
 - Created at  :  2005-04-13   20:11
 - Author      :  ������
 - Description :  ��ġ��ȸâ�� ListCtrl�� MouseMove�޼����� �߻��� ��쿡�� ��ġ��ȸâ��
				  �̵��� ����ؾ� �Ѵ�.
 -------------------------------------------------------------------------------*/
void CInquiryDataWindowDlg::OnMouseMove(CPoint point)
{
	// (2006/2/8 - Seung-Won, Bae) Resource Handle�� �� DLL�� Resource Handle�� �������ش�.
	ML_SET_LANGUAGE_RES();

	HFONT hFont = m_pIChartOCX->GetChartFont();
	if( !hFont) return;

	HWND hOcxWnd = m_pIChartOCX->GetOcxHwnd();
	HDC hDC = ::GetDC( hOcxWnd);
	if( !hDC) return;

	// (2003.12.23, ��¿�) Resource Manager ������ �����Ѵ�.
	HFONT hOldFont = ( HFONT)::SelectObject( hDC, hFont);

	RECT rc;
	::GetWindowRect( hOcxWnd, &rc);

	CPoint ptMouse;
	ptMouse.x = point.x - rc.left;
	ptMouse.y = point.y - rc.top;

	OnMouseMove( CDC::FromHandle( hDC), ptMouse);

	// (2004.11.04, ��¿�) DC ó���� ������ ���´�.
	::SelectObject( hDC, hOldFont);
	::ReleaseDC( m_pIChartOCX->GetOcxHwnd(), hDC);
}

// (2006/2/7 - Seung-Won, Bae) Show and Hide Numerical Inquiry Dialog
void CInquiryDataWindowDlg::ShowNumericalInquiryDlg( const BOOL p_bShow)
{
	if( p_bShow) CreateNumericalInquiryDlg();
	else DestroyNumericalInquiryDlg();

	// (2008/10/30 - Seung-Won, Bae) for Inquiry Data Export.
	if( m_hExportWnd) ::SendMessage( m_hExportWnd, RMSG_INQUIRY_DATA_EXPORT, 0, p_bShow);
}

void CInquiryDataWindowDlg::ShowNumericalInquiryDlg( CDC *p_pDC, const CPoint &p_ptPoint)
{
	if( IsUsingNumericalInquiryDlg()) OnMouseMove( p_pDC, p_ptPoint);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  DestroyNumericalInquiryDlg
 - Created at  :  2004-12-02   11:03
 - Author      :  ������
 - Description :  �����ִ� ��ġ��ȸâ�� �ݴ´�.
 -----------------------------------------------------------------------------------------*/
bool CInquiryDataWindowDlg::DestroyNumericalInquiryDlg()
{
	// Resource Handle�� �� DLL�� Resource Handle�� �������ش�.
	ML_SET_LANGUAGE_RES();

	m_bInitDialogMove = false;	// ��ġ��ȸâ �ʱ� �̵����� �ʱ�ȭ.
	if( GetSafeHwnd()) DestroyWindow();

	m_bUseNumericalInquiryDlg = false;		// ��ġ��ȸâ ��� ��
	m_bNumericalInquiryDlgPosLeft = true;	// Dlg RePosition.

	m_pIChartOCX->InvalidateControlWithResetBlockRegion();

	m_bCalcSizeInquiryDlg = false;
	return true;
}

/*-------------------------------------------------------------------------------
 - Function    :  OnMouseMove
 - Created at  :  2005-03-15   15:10
 - Author      :  ������
 - Description :  �ش縶�콺 ��ġ�� ����Ÿ�� ��ġ��ȸâ�� �����ϰ�, ��ġ��ȸâ�� �̵���Ų��.
 -------------------------------------------------------------------------------*/
void CInquiryDataWindowDlg::OnMouseMove(CDC* pDC, CPoint point)
{
	if( GetSafeHwnd() == NULL)
		return;

	// (2008/5/13 - Seung-Won, Bae) Do not call MouseMove directly in refresh time. Indicator is not calculated yet.
	CRect rc = m_pIChartOCX->GetOcxRegion();
	CPoint ptCursor( point);
	if( ptCursor.x < 0 || ptCursor.y < 0)
	{
		CPoint ptCenter = rc.CenterPoint();
		if( ptCursor.x < 0) ptCursor.x = ptCenter.x;
		if( ptCursor.y < 0) ptCursor.y = ptCenter.y;
	}

	// Ư����Ʈ(�Ｑ��ȯ��, P&F, ���ð�)�� �ڷ����ڿʹ� ������ ��ǥ���̴�.
	// �׷��Ƿ� ��ġ��ȸâ����� ���ǹ��ϴ�.
	CString strFunctionName;
	if(m_pIChartOCX->IsOnSpecialChart()) 
	{
		// (2008/10/30 - Seung-Won, Bae) for Inquiry Data Export.
		if( !m_hExportWnd)
		{
			DestroyNumericalInquiryDlg();
			return;
		}
	}

	// �ش翵�� ��Ͽ����� �׷��������� ������ ����.
	int nRow, nColumn;
	if( !m_pIChartManager->GetBlockIndex( ptCursor, nRow, nColumn)) return;

	IBlock *pIBlock = m_pIChartManager->GetBlock( nRow,nColumn);
	if( !pIBlock) return;
	CRect rctNoFrame, rctNoFrameAndMargin;
	BOOL bResult = pIBlock->GetGraphRegions( rctNoFrame, rctNoFrameAndMargin);
	pIBlock->Release();
	pIBlock = NULL;
	if( !bResult) return;
	if( CRect(0,0,0,0) == rctNoFrame) return;

	// 1. ���콺����Ʈ�� ��ġ�� ����Ÿ��ȸ �� ��ġ��ȸâ�� ����.
	//CString szRQ = m_pIChartManager->FindGraphRQ(ptCursor);
	if( !m_niDataQueryer.ParsingInquiryData("", ptCursor)) return;

	// (2008/10/30 - Seung-Won, Bae) for Inquiry Data Export. (2: Do not use m_saListData.)
	if( m_hExportWnd)
		::SendMessage( m_hExportWnd, RMSG_INQUIRY_DATA_EXPORT, 2, ( LPARAM)m_saListData);

	SetInquiryData(pDC, &m_niDataQueryer.m_listInquiryData,&m_niDataQueryer.m_listPrevInquiryData, m_niDataQueryer.m_nMaxDecimal);

	// (2008/10/30 - Seung-Won, Bae) for Inquiry Data Export. (1: Use m_saListData.)
	if( m_hExportWnd)
	{
		::SendMessage( m_hExportWnd, RMSG_INQUIRY_DATA_EXPORT, 1, ( LPARAM)m_saListData);
		TRACE( "\r\n[NIDLG_EXPORT] : OK");
		return;
	}

	// 2. ��ġ��ȸâ �̵�.
	// 2.0 ��ġ��ȸâ�� ���ƴٴҼ� �ִ� ������ OCX�������̴�!
	// (2006/3/6 - Seung-Won, Bae) Get Region with Inner Block Moving Option.
	CRect rectOcx;
	if( !m_pIChartOCX->IsOcxAreaNumericalInquiry()) 
		rectOcx = m_pIChartManager->GetGraphRegionSumInColumn( ptCursor);
	
	if( rectOcx.IsRectEmpty()) 
	{
		rectOcx = m_pIChartOCX->GetOcxRegion();
		rectOcx = m_pIChartManager->GetGraphRegionSumInColumn(rectOcx.CenterPoint());
	}

	//Margin ����
	//rectOcx.right += 100; 
	if(m_nMoveNumericalInquiryDlg == 0)
	{ // 2.1. �̵�.
		MoveNumericalInquiryDlg(pDC, rectOcx, ptCursor);
	}
	else if(m_nMoveNumericalInquiryDlg == 1)
	{ // 2.2. ����. (ó�� �����ÿ����� �̵������ش�.)
		if(!m_bInitDialogMove)
		{
			MoveNumericalInquiryDlg(pDC, rectOcx, ptCursor);
			m_bInitDialogMove = true;
		}
	}
}

/*-----------------------------------------------------------------------------------------
 - Function    :  CreateNumericalInquiryDlg
 - Created at  :  2004-12-02   11:02
 - Author      :  ������
 - Description :  ��ġ��ȸâ�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CInquiryDataWindowDlg::CreateNumericalInquiryDlg( void)
{
	// Ư����Ʈ(�Ｑ��ȯ��, P&F, ���ð�)�� �ڷ����ڿʹ� ������ ��ǥ���̴�.
	// �׷��Ƿ� ��ġ��ȸâ����� ���ǹ��ϴ�.
	CString strFunctionName;
	
	if(m_pIChartOCX->IsOnSpecialChart()) return false;

	// (2006/2/9 - Seung-Won, Bae) Do not destroy PrevDlg and not recreate it.
	if( GetSafeHwnd()) return true;

	// (2006/2/8 - Seung-Won, Bae) Resource Handle�� �� DLL�� Resource Handle�� �������ش�.
	ML_SET_LANGUAGE_RES();

	// (2006/2/8 - Seung-Won, Bae) Do not use GetChartWindowPointer()'s result. That does not exist in CWnd Object Map of this Module.
	CWnd* pWnd = CWnd::FromHandle( m_pIChartOCX->GetOcxHwnd());
	Create( IDD_INQUIRYDATAWINDOW, pWnd);
	SetDlgBrush( m_clrInquiryDlg);

//	CWnd *pWnd2 = CWnd::FromHandle( GetSafeHwnd());
//	ASSERT( pWnd2 == this);

	// (2006/3/29 - Seung-Won, Bae) Dynamic Caption Setting for each Site.
	if( m_pIChartOCX->IsOnCaptionFlagOfNIDLG()) ModifyStyle( 0, WS_CAPTION | WS_SYSMENU);

	MoveWindow(-300, -300, 300, 300);

	// Device Context, Resource Font and CallBack Object Setting.
	CClientDC dc( pWnd);
	SetResourceFont( CFont::FromHandle( m_pIChartOCX->GetChartFont()));

	ShowWindow(SW_SHOW);
	m_bUseNumericalInquiryDlg = true;		// ��ġ��ȸâ ������ ǥ��

	m_pIChartOCX->InvalidateControlWithResetBlockRegion();
	return true;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  IsUsingNumericalInquiryDlg
 - Created at  :  2004-12-02   13:20
 - Author      :  ������
 - Description :  ���� ��ġ��ȸâ�� ����������� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CInquiryDataWindowDlg::IsUsingNumericalInquiryDlg()
{
	return m_bUseNumericalInquiryDlg;
}

/*-------------------------------------------------------------------------------
 - Function    :  MoveNumericalInquiryDlg
 - Created at  :  2005-03-15   17:56
 - Author      :  ������
 - Parameters  :  pDC		 - OCX�� DC.
 -				  rectRegion - ��ġ��ȸâ�� �����ϼ� �ִ� ����.
 -                point      - ���콺�� ��ġ.
 - Return Value:  None
 - Description :  ���콺��ġ�� ������ ��ġ��ȸâ�� ��ġ�� �����Ͽ� ��ġ��ȸâ�� �����δ�.
 -------------------------------------------------------------------------------*/
void CInquiryDataWindowDlg::MoveNumericalInquiryDlg(CDC* pDC, CRect rectRegion, CPoint point)
{
	if(!GetSafeHwnd())
		return;

	CRect rectDlgPosition;
	GetWindowRect(&rectDlgPosition);

	// ��ġ��ȸ����Ÿ(���ڿ�)�� ���ڱ��̸�ŭ ��ġ��ȸâ����� ����ؿ´�.
	ResizeNumericalInquiryDlg(pDC, &rectDlgPosition);

	if(m_bNumericalInquiryDlgPosLeft == true)	// ��ġ��ȸâ Left�� ��ġ�� ������.
	{
		if(point.x <= rectRegion.left + rectDlgPosition.Width())	// ���������� �̵����Ѿ��� ���.
			m_bNumericalInquiryDlgPosLeft = false;
		else
			m_bNumericalInquiryDlgPosLeft = true;
	}
	else	// ��ġ��ȸâ Right�� ��ġ�� ������..
	{
		if(point.x >= rectRegion.right - rectDlgPosition.Width())	// �������� �̵����Ѿ��� ���
		{
			if(point.x <= rectRegion.left + rectDlgPosition.Width())
				m_bNumericalInquiryDlgPosLeft = false;
			else
				m_bNumericalInquiryDlgPosLeft = true;
		} 
		else
		{
			m_bNumericalInquiryDlgPosLeft = false;
		}
	}	//end of if(m_bNumer...

	if(point.x == -1 && point.y == -1)
		m_bNumericalInquiryDlgPosLeft = true;

	CRect rectMoveWindow;
	if(m_bNumericalInquiryDlgPosLeft)
		rectMoveWindow = CRect(rectRegion.left, rectRegion.top, rectRegion.left + rectDlgPosition.Width(), rectRegion.top + rectDlgPosition.Height());
	else
		rectMoveWindow = CRect(rectRegion.right - rectDlgPosition.Width(), rectRegion.top, rectRegion.right, rectRegion.top + rectDlgPosition.Height());
	MoveWindow(rectMoveWindow, TRUE, pDC);
}

/*-------------------------------------------------------------------------------
 - Function    :  ResizeNumericalInquiryDlg
 - Created at  :  2005-03-25   14:03
 - Author      :  ������
 - Description :  ��ġ��ȸ����Ÿ���̿� �°� Dialog ����� ����Ѵ�.
 - Update	   :  04/07 ���� ������� ������ �ξ� �˳��ϰ� ���, �ѹ��� ����Ѵ�. ���÷�
					��ȸâũ�Ⱑ ���ϴ°��� �������� �뽺!
 -------------------------------------------------------------------------------*/
void CInquiryDataWindowDlg::ResizeNumericalInquiryDlg(CDC* pDC, CRect* rectDlg)
{
	if(pDC == NULL)
		return;
	
	TEXTMETRIC tm;
	CFont *pOldFont = (CFont*)pDC->SelectObject( CFont::FromHandle( m_pIChartOCX->GetChartFont()));
	SIZE size ;
	::GetTextExtentPoint32(pDC->m_hDC, "0", 1, &size );
	pDC->GetTextMetrics(&tm);

	// 0. ���� ������� �ѹ��� �����Ѵ�. ��ġ��ȸ�ø��� ����Ÿ����� �°� ���÷� ����� 
	//   ����Ǵ°� ���� ���� ���� �ʱ� ����!
	if(!m_bCalcSizeInquiryDlg)
	{
		// 1. Width ���ϱ�.
		// 1.1. Width�� ���ϱ� ���� Title�� Data�� MaxLength�� ���Ѵ�.
		int nTitleMaxLength = 0;
		int nDataMaxLength = 0;

		CRowData *pRowData = NULL;
		POSITION posRow = NULL;
		POSITION posInquiryItem = m_niDataQueryer.m_listInquiryData.GetHeadPosition();
		while( posInquiryItem)
		{
			CInquiryItem *plistData = m_niDataQueryer.m_listInquiryData.GetNext( posInquiryItem);
			
			posRow = plistData->GetHeadPosition();
			while( posRow)
			{
				pRowData = plistData->GetNext( posRow);

				// Title.
				if(nTitleMaxLength < pRowData->m_strName.GetLength())
					nTitleMaxLength = pRowData->m_strName.GetLength();
				// Data.
				if(nDataMaxLength < pRowData->m_strData.GetLength())
					nDataMaxLength = pRowData->m_strData.GetLength();
			}
		}

		// 1.3. Column Width�� ��Ȯ�ϰ�(Ÿ��Ʈ�ϰ�) ����� �����ϸ� ���� �乮�ڿ��� �����ڰ� �߸��� "..."��
		// ǥ�ó��� ���� �ذ��� ���� ������ �ǹ̷� 4 char count���� ������ Į���� �߰����ش�.
		int nDlgWidth = (nTitleMaxLength + 4 + nDataMaxLength + 4 + 6 + 1) * size.cx;	
		// 1.4. Width�� �������� �־� ��ġ��ȸâ ����� �������� �ʰ� ����ϵ��� �Ѵ�.
		nDlgWidth += 20;	// ������

		// 1.5. ������ ����.
		rectDlg->right = rectDlg->left + nDlgWidth;

		// 0.1. Width ������ ����� ������ �÷���ǥ��.
		m_bCalcSizeInquiryDlg = true;
	}

	// 2. Height ���ϱ�.
	// 2.1. Height�� ���ϱ� ���� ��ġ��ȸ����Ÿ�� Row������ ���Ѵ�.
	int nRowCount = 0;
	if( !m_niDataQueryer.m_listInquiryData.IsEmpty()) nRowCount = m_niDataQueryer.m_listInquiryData.GetHead()->GetCount();

	// 2.2 ������ ����.
	rectDlg->bottom = rectDlg->top + nRowCount * (size.cy + nRowCount + 1 );//tour2k

	// 2.3. Ÿ��Ʋ���� ������ ������ �ͼ� ���߿� �����ش�.
	CRect clientRect, windowRect;
	GetClientRect(&clientRect);
	GetWindowRect(&windowRect);

	rectDlg->bottom += (windowRect.Height() - clientRect.Height());

	pDC->SelectObject(pOldFont);
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/3/4
// Modifier		: 
// Comments		: Notify Cursor Mode Change for Drag Cross Line & Numercial Inquiry DLG
//-----------------------------------------------------------------------------
void CInquiryDataWindowDlg::OnNumericalInquiryOptionChanged( const long p_lNumericalInquiryOption)
{
	m_nMoveNumericalInquiryDlg = ( p_lNumericalInquiryOption & NIDLG_MANUALMOVE ? 1 : 0);
}

/*-------------------------------------------------------------------------------
 - Function    :  SetNumericalInquiryDlgBrush
 - Created at  :  2005-03-16   10:21
 - Author      :  ������
 - Description :  ��ġ��ȸâ ������ �����Ѵ�.
 -------------------------------------------------------------------------------*/
void CInquiryDataWindowDlg::SetNumericalInquiryDlgBrush(COLORREF clrDlg)
{
	m_clrInquiryDlg = clrDlg;
	if( GetSafeHwnd()) SetDlgBrush(m_clrInquiryDlg);
}

void CInquiryDataWindowDlg::Refresh( void)
{
	// (2004.11.05, ��¿�) TR�̳� Real ó������ ToolTip�� ���Ͽ� Mouse ��ġ�� ������� �ʰ�,
	//		�׶��׶� Mouse ��ġ�� �ľ��Ͽ� ó���ϴ� ������ �����Ѵ�.
	CPoint ptCursor;
	GetCursorPos( &ptCursor);
	ScreenToClient( &ptCursor);
		
	// (2008/5/13 - Seung-Won, Bae) Do not call MouseMove directly in refresh time. Indicator is not calculated yet.
	//	OnMouseMove(&dc, ptCursor);
	::PostMessage( m_pIChartOCX->GetOcxHwnd(), WM_MOUSEMOVE, 0, MAKELONG( ptCursor.x, ptCursor.y));
}

BOOL CInquiryDataWindowDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class

	// (2008/10/30 - Seung-Won, Bae) for Inquiry Data Export.
	if( m_hExportWnd) cs.style &= ~WS_VISIBLE;
	
	return CNumericalInquiryDlg::PreCreateWindow(cs);
}
