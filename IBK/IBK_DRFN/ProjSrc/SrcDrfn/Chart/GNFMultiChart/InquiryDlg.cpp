// InquiryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InquiryDlg.h"
#include "./define/ChartMsgDef.h"
#include "../chart_common/BlockBaseEnum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define _DPV_CUSTOMFONT						"����ü;10;0;0;0;0;"
/////////////////////////////////////////////////////////////////////////////
// CInquiryDlg dialog


CInquiryDlg::CInquiryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInquiryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInquiryDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = pParent;

	m_dlgBrush.CreateSolidBrush(RGB(230, 230, 230));
//	m_pFont = NULL;

	m_clrInquiryDlg = RGB(255, 255, 255);

	m_nListDataSize = 0;
	m_bCalculateSize	= false;

	m_bDataDispFlag		= FALSE;

	m_saListData	= NULL;
}

CInquiryDlg::~CInquiryDlg()
{
	if( IsWindow( GetSafeHwnd())) DestroyWindow();

	m_dlgBrush.DeleteObject();
	m_pFont.DeleteObject();
}

void CInquiryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInquiryDlg)
	DDX_Control(pDX, IDC_LIST_INQUIRYDATA, m_listctrlInquiryData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInquiryDlg, CDialog)
	//{{AFX_MSG_MAP(CInquiryDlg)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_INQUIRYDATA, OnGetdispinfoListInquirydata)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInquiryDlg message handlers

HBRUSH CInquiryDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
/*
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkMode(TRANSPARENT);

	return m_dlgBrush;
*/
}

BOOL CInquiryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// COLUMN �ʱ�ȭ.
	LV_COLUMN lc ;
	lc.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH ;
	lc.fmt = LVCFMT_LEFT; 

	lc.pszText = _T("�׸�");
	lc.cx = 50;
	m_listctrlInquiryData.InsertColumn(0, &lc);
	
	lc.pszText = _T("��");
	lc.cx = 50;
	m_listctrlInquiryData.InsertColumn(1, &lc);

	// ListCtrl ��Ÿ�� �߰�.	
	m_listctrlInquiryData.SetExtendedStyle(m_listctrlInquiryData.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_bCalculateSize = false;

	CreateFontIndirect();

	MoveWindow(CRect(0, 0, 200, 200), FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInquiryDlg::SetInquiryData(CDC* pDC, CStringArray *pInquiryData, CStringArray *pInquiryPrevData,int nMaxDecimal /* = 0*/)
{
	if( pInquiryData->GetSize()) return;
	if( pInquiryPrevData->GetSize()) return;
	// _LISTDATA�� �ϳ��� ����Ѵ�. �׷��Ƿ� �̸� �������°� ����! (Multi Line�� ����������ϴϱ� For��������!)
//	m_listData = pInquiryData->GetHead();

//	CInquiryItem* listPrevData = pInquiryPrevData->GetHead();
	// ���� List Ctrl�� ������ ������ �����´�. (���� ������ ListCtrl���� ������ ���������� ����� �ǰ�
//	int nPrevItemCount = m_listctrlInquiryData.GetItemCount();	
//
//	// ����Ÿ�� ListCtrl�� ����.
//	LV_ITEM ltc;
//	ltc.mask = LVIF_TEXT;
//
//	// (2006/7/2 - Seung-Won, Bae) Using Virtual Draw. 0:Title, 1:Data
	int nIndex = 0;
//	CRowData *pRowData = NULL;
//	CRowData *pRowPrevData = NULL;
//	CString strItemData;
//	double dItemData = 0.0;
//	double dPrevItemData = 0.0;
//	double dItemRatio = 0.0;
//	CString strTempData;
//	POSITION posRow = m_listData->GetHeadPosition();
//	POSITION posPrevRow = listPrevData->GetHeadPosition();
//	while( posPrevRow)
//	{
//		pRowPrevData = listPrevData->GetNext( posPrevRow);
//		
//		strTempData = pRowPrevData->m_strData;
//		strTempData.Replace(",","");
//		if(pRowPrevData != NULL && 
//			(pRowPrevData->m_strName == _MTEXT( C0_CLOSE) ||
//			 pRowPrevData->m_strName == _MTEXT( C3_PRICE)))
//		{
//			strTempData = pRowPrevData->m_strData;
//			strTempData.Replace(",","");
//			dPrevItemData = atof(strTempData);
//		}
//	}
//
//	while( posRow)
//	{
//		pRowData = m_listData->GetNext( posRow);
//		
//		m_saListData[ 0].SetAtGrow( nIndex,ReplaceTitle(pRowData->m_strName));
//		strTempData = pRowData->m_strData;
//		strTempData.Replace(",","");
//		dItemData = atof(strTempData);
//		if(dItemData != 0.0 && 
//			(pRowData->m_strName == _MTEXT( C0_OPEN) ||
//			 pRowData->m_strName == _MTEXT( C0_HIGH) ||
//			 pRowData->m_strName == _MTEXT( C0_LOW) ||
//			 pRowData->m_strName == _MTEXT( C0_CLOSE) ||
//			 pRowData->m_strName == _MTEXT( C3_PRICE)))
//		{
//			dItemRatio = (dItemData-dPrevItemData)/fabs(dPrevItemData)*100;
//			strItemData.Format("%s",ReFormData( pRowData->m_strData,dItemRatio,nMaxDecimal));
//		}
//		else
//		{
//			strItemData.Format("%s",ReFormData( pRowData->m_strData, nMaxDecimal));
//		}
//		
//		m_saListData[ 1].SetAtGrow( nIndex,strItemData);		
//		nIndex++;
//	}
	m_nListDataSize = nIndex;
	m_listctrlInquiryData.SetItemCount( m_nListDataSize);
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/7/2
// Modifier		: Use Virtual Draw.
// Comments		: 
//-----------------------------------------------------------------------------
void CInquiryDlg::OnGetdispinfoListInquirydata(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
	
	LV_ITEM *pItem = &( pDispInfo->item);
	if( !( pItem->mask & LVIF_TEXT) || pItem->iSubItem < 0 || 1 < pItem->iSubItem || pItem->iItem < 0
		|| m_nListDataSize <= pItem->iItem) return;
	
	if ( pItem->iItem >= m_saListData[0].GetSize() )
		return;

	pItem->pszText = ( char *)(const char *)m_saListData[ pItem->iSubItem][ pItem->iItem];
}

/*-------------------------------------------------------------------------------
 - Function    :  MoveWindow
 - Created at  :  2005-03-28   11:19
 - Author      :  ������
 - Description :  ��ġ��ȸâ�� �̵���Ų��. ���Ҿ� ListCtrl�� �̵���Ų��.
 -------------------------------------------------------------------------------*/
void CInquiryDlg::MoveWindow(LPCRECT lpRect, BOOL bRepaint /*= TRUE*/)
{
//	CDC* pDC = GetDC();
	CPaintDC dc(this);

	// 1. List Ctrl ������ ����.
	CRect rect;
	this->GetClientRect(&rect);
	rect.right = lpRect->right - lpRect->left;
	rect.bottom = lpRect->bottom - lpRect->top;
	m_listctrlInquiryData.MoveWindow(&rect);

	if(!m_bCalculateSize)
	{
		// 2. Column Width ����.
		SetColumnWidth(0, &dc);
		SetColumnWidth(1, &dc);
		m_bCalculateSize = true;
	}

	// 3. Window ������ ����.
//	MoveWindow(lpRect, bRepaint);
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
int CInquiryDlg::SetColumnWidth(int nColIndex, CDC* pDC)
{	
	CString strMaxString = GetMaxColumnString( nColIndex );
	
	CFont* pOldFont = (CFont*)pDC->SelectObject(&m_pFont);

	CSize size;
	::GetTextExtentPoint32(pDC->m_hDC, _T("0"), 1, &size);

	// Column Width ���.
	int nColumnWidth = 0;
	if(nColIndex == 0)
		nColumnWidth = (strMaxString.GetLength() + 4) * size.cx;
	else
		nColumnWidth = (strMaxString.GetLength() + 4 + 6) * size.cx;
	
	if(nColIndex == 1)
		nColumnWidth += 5;	// ���� �����ֱ�.

	m_listctrlInquiryData.SetColumnWidth(nColIndex, nColumnWidth);

	pDC->SelectObject(pOldFont);

	return nColumnWidth;
}


void CInquiryDlg::SetDlgBrush(COLORREF clrDlgBrush)
{
	m_listctrlInquiryData.SetBkColor(clrDlgBrush);
	m_listctrlInquiryData.SetTextBkColor(clrDlgBrush);

	if(m_dlgBrush.GetSafeHandle())
		m_dlgBrush.DeleteObject();
	m_dlgBrush.CreateSolidBrush(clrDlgBrush);
}

void CInquiryDlg::Refresh( void)
{
//	// (2004.11.05, ��¿�) TR�̳� Real ó������ ToolTip�� ���Ͽ� Mouse ��ġ�� ������� �ʰ�,
//	//		�׶��׶� Mouse ��ġ�� �ľ��Ͽ� ó���ϴ� ������ �����Ѵ�.
//	CPoint ptCursor;
//	GetCursorPos( &ptCursor);
//	ScreenToClient( &ptCursor);
//		
//	// (2008/5/13 - Seung-Won, Bae) Do not call MouseMove directly in refresh time. Indicator is not calculated yet.
//	//	OnMouseMove(&dc, ptCursor);
//	::PostMessage( m_pIChartOCX->GetOcxHwnd(), WM_MOUSEMOVE, 0, MAKELONG( ptCursor.x, ptCursor.y));
}

BOOL CInquiryDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class

	// (2008/10/30 - Seung-Won, Bae) for Inquiry Data Export.
//	if( m_hExportWnd) cs.style &= ~WS_VISIBLE;
	
	return CDialog::PreCreateWindow(cs);
}

/*-------------------------------------------------------------------------------
 - Function    :  GetMaxColumnString
 - Created at  :  2005-03-23   10:36
 - Author      :  ������
 - Parameters  :  pData     - list.
 -                nColIndex - �÷� �ε���.
 - Description :  Column�� ���а��߿� ���� �� ���ڿ��� �����´�. (�ֳĸ� Į���� ���̸� ���ϱ� ����)
 -------------------------------------------------------------------------------*/
LPCTSTR CInquiryDlg::GetMaxColumnString( int nColIndex )
{
	CString strMaxColumnString = _T("");

	if ( m_saListData == NULL )
		return (LPCTSTR)strMaxColumnString;

	CString strFirst;
	int	nCnt = m_saListData[nColIndex].GetSize();
	for ( int i = 0 ; i < nCnt ; i++ )
	{
		strFirst = m_saListData[nColIndex].GetAt(i);
		if(strMaxColumnString.GetLength() < strFirst.GetLength())
			strMaxColumnString = strFirst;
	}

	return (LPCTSTR)strMaxColumnString;
}


void CInquiryDlg::OnClose() 
{
//	DestroyNumericalInquiryDlg();
	//	��ġ��ȸ ��۹�ư �ʱ�ȭ
	m_bDataDispFlag = FALSE;
//	DestroyWindow();
	CDialog::OnClose();

	m_pParent->SendMessage(UMSG_CHART_INQUIRY_DELETE, CToolOptionInfo::TOOLOPTION::T_NIDLG_OVER , FALSE);
}

void CInquiryDlg::StartDisplayData(CStringArray* saListData) 
{
//	m_bDataDispFlag = TRUE;
	m_saListData = saListData;

	if ( m_bCalculateSize == false )
	{

		CRect rect;
		this->GetClientRect(&rect);

		int nCnt = m_saListData[0].GetSize();
		int nHeight = ( 14 * ( nCnt + 1)) + GetSystemMetrics(SM_CYCAPTION);

		CString strTitle1 = GetMaxColumnString( 0 );
		CString strTitle2 = GetMaxColumnString( 1 );

		strTitle2.TrimRight();

		CPaintDC dc(this);
		CSize size;
		::GetTextExtentPoint32(dc.m_hDC, _T("0"), 1, &size);

		// Column Width ���.
		int nColumnWidth = ((strTitle1.GetLength() + 2) * size.cx) + ((strTitle2.GetLength() + 2) * size.cx);
		int nXBorderSize = GetSystemMetrics(SM_CXDLGFRAME/*SM_CXFRAME*/);
		int nYBorderSize = GetSystemMetrics(SM_CYDLGFRAME/*SM_CYFRAME*/);

		rect.right = rect.left + nColumnWidth + (nXBorderSize*2);
		rect.bottom = rect.top + nHeight + (nYBorderSize*2) + 6;

		// (2008/11/7 - Seung-Won, Bae) Do not activate.
		SetWindowPos( NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOOWNERZORDER);

		MoveWindow(&rect, TRUE);
	}

	m_nListDataSize = m_saListData[0].GetSize();
	m_listctrlInquiryData.SetItemCount( m_nListDataSize );
	m_listctrlInquiryData.Invalidate();
}

void CInquiryDlg::EndDisplayData() 
{
//	m_bDataDispFlag = FALSE;
	m_bCalculateSize = false;
}

void CInquiryDlg::SetShowHide(int nType) 
{
	if ( nType == SW_SHOW )
	{
		m_bDataDispFlag = TRUE;
		m_bCalculateSize = false;
	}
	else
	{
		m_bDataDispFlag = FALSE;
		m_bCalculateSize = true;
	}

	ShowWindow(nType);
}

BOOL CInquiryDlg::GetShowHide()
{
	return m_bDataDispFlag;
}


void CInquiryDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here

	if ( m_bDataDispFlag )
	{
		m_bCalculateSize = false;

		CRect rect;
		this->GetClientRect(&rect);

		MoveWindow(&rect, TRUE);

		m_bCalculateSize = true;
	}
}


void CInquiryDlg::CreateFontIndirect()
{
//	LOGFONT logFont;
//	CString strFont = _DPV_CUSTOMFONT;
//	//��Ʈ�̸�
//	int namePos = strFont.Find(";");
//	CString lfFaceName = strFont.Mid(0, namePos);
//	//��Ʈũ��
//	int HeightPos = strFont.Find(";", namePos + 1);
//	CString lfHeight = strFont.Mid(namePos + 1, HeightPos - (namePos + 1));
//	int nlfHeight = atoi(lfHeight);
//	//����ü��Ʈ
//	int BoldPos = strFont.Find(";", HeightPos + 1);
//	CString strBold = strFont.Mid(HeightPos + 1, BoldPos - (HeightPos + 1));
//	BOOL	blfWeight = atoi(strBold);
//	//��Ҽ�
//	int CCLinePos = strFont.Find(";", BoldPos + 1);
//	CString strCCLine = strFont.Mid(BoldPos + 1, CCLinePos - (BoldPos + 1));
//	BOOL	blfStrikeOut = atoi(strCCLine);
//	//����Ӳ�
//	int italyPos = strFont.Find(";", CCLinePos + 1);
//	CString strItaly = strFont.Mid(CCLinePos + 1, italyPos - (CCLinePos + 1));
//	BOOL	blfItalic = atoi(strItaly);
//	//�������
//	int UnderLinePos = strFont.Find(";", italyPos + 1);
//	CString strUnderLine = strFont.Mid(italyPos + 1, UnderLinePos - (italyPos + 1));
//	BOOL	blfUnderline = atoi(strUnderLine);
//
//	logFont.lfHeight = nlfHeight;
//	logFont.lfWidth = 0;
//	logFont.lfEscapement = 0;
//	logFont.lfOrientation = 0;
//	if(blfWeight)
//		logFont.lfWeight = FW_BOLD;
//	else
//		logFont.lfWeight = FW_THIN;
//	logFont.lfItalic = blfItalic;
//	logFont.lfUnderline = blfUnderline;
//	logFont.lfStrikeOut = blfStrikeOut;
//	logFont.lfCharSet = DEFAULT_CHARSET;
//	logFont.lfOutPrecision = OUT_CHARACTER_PRECIS;
//	logFont.lfClipPrecision = CLIP_CHARACTER_PRECIS;
//	logFont.lfQuality = DEFAULT_QUALITY;
//	logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
//	strcpy(logFont.lfFaceName, lfFaceName);
//
//	m_pFont.CreateFontIndirect( &logFont);

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // Zero out the structure.
	lf.lfHeight = 12;                      // Request a 12-pixel-height font.
/*	lf.lfWeight = 700;					   JSR-20051129 STOP  FW_BOLD(=700) */
	lf.lfWeight = FW_THIN;				   //LYH-20061009 FW_BOLD(=700)
	lstrcpy(lf.lfFaceName, _T("����ü"));   // Request a face name "����".
	lf.lfCharSet = HANGUL_CHARSET;
	m_pFont.CreateFontIndirect(&lf);

}