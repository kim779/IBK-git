// TabCodeSearchCommodity.cpp : implementation file
//

#include "stdafx.h"
#include "multichart.h"
#include "TabCodeSearchCommodity.h"
#include "../chart_common/Grid/ColorTable.h"
#include ".\tabcodesearchcommodity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef CArray <int, int>	CIntArray;

#define ROW_HEIGHT 15
#define ROW_WIDTH  48
#define COL_CENTER 1
#define TITLE_ROW 1
enum		{EXERCISE, FIRST, SECOND, THIRD, FOURTH};	// �� �ɼ� ���� 

CTabCodeSearchCommodity::CTabCodeSearchCommodity(CWnd* pParent /*=NULL*/)
	: CTabDlg(CTabCodeSearchCommodity::IDD, pParent)
	, m_bSpread(FALSE)
	, m_nOptionMarket(-1)
{
	//{{AFX_DATA_INIT(CTabCodeSearchCommodity)
	m_pGridCtrl = NULL;
	m_nIndex = 1;
	//}}AFX_DATA_INIT
}

void CTabCodeSearchCommodity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabCodeSearchCommodity)
	DDX_Control(pDX, IDC_COMBO_MONTH, m_ctrlComboMonth);
	DDX_Control(pDX, IDC_LIST_FUTURE, m_listFuture);
	DDX_Control(pDX, IDC_LIST_FUTURE2, m_listFuture2);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_RDO_FUTURE, m_bSpread);
}

BEGIN_MESSAGE_MAP(CTabCodeSearchCommodity, CDialog)
	//{{AFX_MSG_MAP(CTabCodeSearchCommodity)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_CBN_EDITCHANGE(IDC_COMBO_MONTH, OnEditchangeComboMonth)
	ON_CBN_SELCHANGE(IDC_COMBO_MONTH, OnSelchangeComboMonth)
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_LIST_FUTURE, OnSelchangeListFuture)
	ON_LBN_SELCHANGE(IDC_LIST_FUTURE2, OnSelchangeListFuture2)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RDO_FUTURE, OnBnClickedRdoFuture)
	ON_BN_CLICKED(IDC_RDO_SPREAD, OnBnClickedRdoSpread)
END_MESSAGE_MAP()

void CTabCodeSearchCommodity::OnSize(UINT nType, int cx, int cy) 
{
	CTabDlg::OnSize(nType, cx, cy);	
	// TODO: Add your message handler code here
	RecalcLayout();	
}

BOOL CTabCodeSearchCommodity::OnInitDialog() 
{
	CTabDlg::OnInitDialog();

	// TODO: Add extra initialization here
	int	nID = 12; // Data ���丮
	m_strDataDirPath = (LPCTSTR)(LPCSTR)AfxGetPctrData(nID);
	InitMasterData();
	InitGrid();
	InsertMasterData(m_nIndex);			// master file data�� grid edit����/color�� setting
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

CRect CTabCodeSearchCommodity::GetRectOfCtrl(INT nID,BOOL bInit/*=FALSE*/)
{
	CRect rect;
	GetClientRect(rect);
	int nWidth = rect.Width();
	rect.DeflateRect(GAP_TABDLG,GAP_TABDLG,GAP_TABDLG,GAP_TABDLG);
	switch(nID)
	{
	case IDC_LIST_FUTURE:
		rect.top = rect.top + (HEIGHTOFCOMBO * 2);
		//rect.bottom = rect.bottom - HEIGHTOFJONGSTATUS - GAP_TABDLG - HEIGHTOFBUTTON - GAP_TABDLG;
		rect.bottom = rect.top + 110;
		break;		
	case IDC_LIST_FUTURE2:
		rect.top = rect.top + (HEIGHTOFCOMBO * 2) + 110;
		//rect.bottom = rect.bottom - HEIGHTOFJONGSTATUS - GAP_TABDLG - HEIGHTOFBUTTON - GAP_TABDLG;
		rect.bottom = rect.top + 100;
		break;		
	case ID_GRIDCTRL:
		rect.top = rect.top + (HEIGHTOFCOMBO * 2) + 215;
		//rect.top = rect.bottom - GAP_TABDLG - HEIGHTOFJONGSTATUS - HEIGHTOFBUTTON;
		rect.bottom = rect.bottom - GAP_TABDLG;// - HEIGHTOFBUTTON;
		break;
	default:
		break;
	}
	return rect;
}

void CTabCodeSearchCommodity::RecalcLayout()
{	
	RecalcLayoutCtrl(m_listFuture);
	RecalcLayoutCtrl(m_listFuture2);
	if(m_pGridCtrl)
		RecalcLayoutCtrl(*m_pGridCtrl,ID_GRIDCTRL);	
}

void CTabCodeSearchCommodity::OnDestroy() 
{
	CTabDlg::OnDestroy();

	// TODO: Add your message handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if(m_pGridCtrl)
	{
		delete m_pGridCtrl;
		m_pGridCtrl = NULL;
	} 
}

// �����ڵ� grid init
void CTabCodeSearchCommodity::InitGrid()
{
	if (m_pGridCtrl == NULL)
	{
		// Create the Gridctrl object
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		m_pGridCtrl = new CExGridCtrl;
		if (!m_pGridCtrl) return;

		CRect rcRect;
		GetDlgItem(IDC_FRAME_OPTION)->GetWindowRect(rcRect);		
		ScreenToClient(&rcRect);		

		m_pGridCtrl->Create(rcRect, this, ID_GRIDCTRL);

		///////////////////////////////////////////////////
		// ���� ����
		//m_pGridCtrl->SetEditable(TRUE);				// CInplaceEdit ����
		//		m_pGridCtrl->SetFont(GetParent()->GetFont());
		m_pGridCtrl->SetGridColor(COLOR_GRIDHEADER);		// Grid Color ����
		m_pGridCtrl->SetBkColor(COLOR_WHITE);			// Grid Back Color ����
		m_pGridCtrl->EnableColumnHide();				// Column ���� �� �ְ� ����
		m_pGridCtrl->EnableRowHide();					// Row ���� �� �ְ� ����
		m_pGridCtrl->EnableTitleTips(FALSE);			// ToolTip ����
		m_pGridCtrl->SetGridLineColor(COLOR_GRIDLINE);	// Grid Line color

		///////////////////////////////////////////////////
		// �׸��� ����� Default�� �Ѵ�
		InitGridRowCol();

		// �׸����� ����� �ٲ۴�
		ChangeGridRowCol();

		// �׸��忡 �����͸� �߰��Ѵ�.
		InsertGridData();
	}
}

void CTabCodeSearchCommodity::InitGridRowCol()
{	
	try {
		int nRow, nCol;

		nRow = m_listMaster.GetCount() + TITLE_ROW;
		//nCol = DEFAULT_COLUMN_COUNT;
		nCol = 3;

		m_pGridCtrl->SetRowCount(nRow);				// Row ����A��
		m_pGridCtrl->SetColumnCount(nCol);			// Column ����A��
		//		m_pGridCtrl->SetFixedRowCount(1);	// Fixed Row ����A��
		m_pGridCtrl->SetFixedColumnCount(0);		// Fixed Column ����A��

		//		m_pGridCtrl->SetColCenter(COL_CENTER);		// Column A����O ����A��
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
}

void CTabCodeSearchCommodity::ChangeGridRowCol()
{
	int nCol = m_pGridCtrl->GetColumnCount();		// Column Count ���ϱ�
	int nRow = m_pGridCtrl->GetRowCount();			// Row Count ���ϱ�

	// fill rows/cols with text
	for (int row = 0; row < nRow; row++)
		m_pGridCtrl->SetRowHeight(row, ROW_HEIGHT);

	for (int col = 0; col < nCol; col++)
		m_pGridCtrl->SetColumnWidth(col, ROW_WIDTH);

	m_pGridCtrl->SetColumnWidth(COL_CENTER, ROW_WIDTH + 13);		

	m_pGridCtrl->Refresh();
}

// �� ������ ���� �� �߰�
// ���߿� SetItemText(CString) ������ �����͸� �߰��� �� �ְ� �̸� ����
void CTabCodeSearchCommodity::InsertGridData()
{
	int nCol = m_pGridCtrl->GetColumnCount();
	int nRow = m_pGridCtrl->GetRowCount();

	// fill rows/cols with text
	int row, col;	
	for (row = 0; row < nRow; row++)
	{
		for (col = 0; col < nCol; col++)
		{ 

			GV_ITEM Item;
			Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_FGCLR|GVIF_BKCLR|GVIF_STATE|GVIF_PARAM;
			Item.nState = GVIS_READONLY | GVIS_MODIFIED ;//| GVIS_DROPHILITED;
			Item.row = row;
			Item.col = col;

			Item.nFormat = DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX|DT_CENTER;

			Item.strText = GetInsertGridDataText(row, col);
			//			Item.crBkClr = GetColorTable(row, col);
			Item.crBkClr = RGB(238,239,243);
			if(row == 0 && col < COL_CENTER)			Item.crBkClr = RGB(244, 180, 180);
			else if(row == 0 && col > COL_CENTER)		Item.crBkClr = RGB(180, 207, 244);
			Item.crFgClr = COLOR_BLACK;
			//Item.crSelectedBkClr = COLOR_GRAY_LIGHT;

			//Item.lParam = NOEDIT;

			m_pGridCtrl->SetItem(&Item);
		}
	}	

}

CString CTabCodeSearchCommodity::GetInsertGridDataText(int row, int col)
{

	if (row != 0 )		return "";

	if (col == COL_CENTER)	return "��簡";
	else if (col < COL_CENTER)	return "��";
	else if (col > COL_CENTER)	return "ǲ";

	//	int nIndex;
	//	if (col < COL_CENTER)	nIndex = COL_CENTER	- col;
	//	else					nIndex = col - COL_CENTER;

	CString strText;
	strText.Format("%d��", m_nMonth.nInfo[m_nIndex] % 100);
	return strText;
}

// master file data�� grid edit����/color�� setting
void CTabCodeSearchCommodity::InsertMasterData(int index)
{
	int nRow, nCol, nRowTotal, nExercise, nATM = 0;

	CString		strPrice, strTmp;
	COLORREF	clrText;

	// ATM
	CString strPath, strATM;
	strPath.Format("%s\\%s",  m_strDataDirPath, "master\\atm.dat");
	char szAtm[100]; memset(szAtm,0x00,sizeof(szAtm));
	::GetPrivateProfileString("ATM", "PRICE", "", szAtm, 100, strPath);
	strATM = (LPCSTR)(LPSTR)szAtm;
	//m_pDBMgr->GetOptionATM(strATM);
	if (strlen(strATM)> 0)	nATM = (int)atof(strATM);

	nRowTotal = m_pGridCtrl->GetRowCount();
	nRow = nRowTotal;
	for (POSITION pos = m_listMaster.GetHeadPosition(); pos; )
	{
		codeinfo& info = m_listMaster.GetNext(pos);

		nExercise = info.nInfo[EXERCISE];
		nRow--;

		// �� check
		if (nRow < TITLE_ROW || nRow >= nRowTotal)		continue;

		clrText = (nATM == nExercise) ? COLOR_RED : COLOR_BLACK;
		UpdateHogaData(nRow, COL_CENTER, GetRealExercise(nExercise) , clrText, 1);
		//		for (int i=FIRST; i <= FOURTH; i++)
		//		{
		if (info.nInfo[index] == 0)	
		{
			for (int j=0; j < 2; j++)
			{
				nCol = (j == 0)	? (COL_CENTER - 1) : (COL_CENTER + 1);
				m_pGridCtrl->SetItemText(nRow, nCol, "X");
			}
			continue;
		}

		for (int j=0; j < 2; j++)
		{

			nCol = (j == 0)	? (COL_CENTER - 1) : (COL_CENTER + 1);

			m_pGridCtrl->SetItemText(nRow, nCol, " ");
			m_pGridCtrl->SetItemBkColour(nRow, nCol, COLOR_WHITE);
		}
		//		}
	}
	m_pGridCtrl->Refresh();

}	

// �ɼ� ���� ��簡�������� �� -> ǲ �� �������� ���� �������� ���� 
int OptEvtPriceItemFuncCommodity(const void* p1,const void *p2)
{
	COptionItem *q1 = *(COptionItem**)p1;
	COptionItem *q2 = *(COptionItem**)p2;

	if(q1->m_nCallPut > q2->m_nCallPut)
	{
		return 1; 
	}
	else if(q1->m_nCallPut < q2->m_nCallPut)
	{
		return -1; 
	}
	else
	{
		if(q1->m_nYear > q2->m_nYear)
		{
			return 1;
		}
		else if(q1->m_nYear < q2->m_nYear)
		{
			return -1; 
		}
		else
		{
			if(q1->m_nMonth > q2->m_nMonth)
			{
				return 1; 
			}
			else if(q1->m_nMonth < q2->m_nMonth)
			{
				return -1; 
			}
			else
			{
				if(q1->m_nEvtPrice > q2->m_nEvtPrice)
				{
					return 1; 
				}
				else if(q1->m_nEvtPrice < q2->m_nEvtPrice)
				{
					return -1; 
				}
				else
					return 0;
			}
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
// ������            : ������(YouJeong, Lee)
// Email             : lotte222@dooriic.co.kr
// ���� ��¥         : 2004/2/4
// �Լ� �̸�         : CDlgSetup::InitMasterData
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : 
// �Ķ���� ����     : 
// �Լ� ����         : ���ο��� master file list pointer�� ��� codeinfo list ����
// ����				 : //003 2006.06.30 {{ Koscom �������� ������ �������� ���� }} 
///////////////////////////////////////////////////////////////////////////////// 
BOOL CTabCodeSearchCommodity::InitMasterData()
{
	int nID = 5;
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	m_pDataManager = (IMasterDataManager*)pGateMng->GetInterface(nID);

	if (m_pDataManager == NULL)		return FALSE;


	//////////////////////////////////////////////////////////////////////////
	// ����
	BOOL bUSD = FALSE, bGOLD = FALSE, bCD = FALSE, bKTB = FALSE, b5TB = FALSE, bMSB = FALSE, bYEN = FALSE, bSYR = FALSE;
	int iIdx = 0;

//@��������
//	CByteArray* parByteMarket = new CByteArray;
//	if(m_pDataManager->GetMarketArray(parByteMarket))
//	{
//		for(int nIndex=0; nIndex < parByteMarket->GetSize(); nIndex++)
//		{
//			BYTE nMarket = parByteMarket->GetAt(nIndex);
//			switch(nMarket)
//			{
//			case 13:	// 13 = KOFEX ������̵�-USD
//				if(!bUSD)
//				{
//					iIdx = m_listFuture.AddString(_T("USD"));
//					m_listFuture.SetItemData(iIdx, nMarket);
//					bUSD = TRUE;
//				}
//				break;
//
//			case 15:	// 15 = KOFEX ������̵�-GOLD
//				if(!bGOLD)
//				{
//					iIdx = m_listFuture.AddString(_T("GOLD"));
//					m_listFuture.SetItemData(iIdx, nMarket);
//					bGOLD = TRUE;
//				}
//				break;
//
//			case 16:	// 16 = KOFEX ������̵�-CD -> LKTB
//				if(!bCD)
//				{
//					// ����� ���� - ojtaso (20080222)
//					//iIdx = m_listFuture.AddString(_T("CD"));
//					iIdx = m_listFuture.AddString(_T("LKTB"));
//					m_listFuture.SetItemData(iIdx, nMarket);
//					bCD = TRUE;
//				}
//				break;
//
//			case 17:	// 17 = KOFEX ������̵�-KTB
//				if(!bKTB)
//				{
//					iIdx = m_listFuture.AddString(_T("KTB"));
//					m_listFuture.SetItemData(iIdx, nMarket);
//					bKTB = TRUE;
//				}
//				break;
//
//			case 62:	// 62 = KOFEX ������̵�-5TB
//				if(!b5TB)
//				{
//					iIdx = m_listFuture.AddString(_T("5TB"));
//					m_listFuture.SetItemData(iIdx, nMarket);
//					b5TB = TRUE;
//				}
//				break;
//
//			case 61:	// 61 = KOFEX ������̵�-MSB
//				if(!bMSB)
//				{
//					iIdx = m_listFuture.AddString(_T("MSB"));
//					m_listFuture.SetItemData(iIdx, nMarket);
//					bMSB = TRUE;
//				}
//				break; 
//
//			case 18:	// 18 = KOFEX ������̵�-YEN
//				if(!bYEN)
//				{
//					iIdx = m_listFuture.AddString(_T("YEN"));
//					m_listFuture.SetItemData(iIdx, nMarket);
//					bYEN = TRUE;
//				}
//				break;
//
//			case 19:	// 19 = KOFEX ������̵�-SYR
//				if(!bSYR)
//				{
//					iIdx = m_listFuture.AddString(_T("SYR"));
//					m_listFuture.SetItemData(iIdx, nMarket);
//					bSYR = TRUE;
//				}
//				break;
//			}
//		}
//
//	}
//@��������

	// }}

	return TRUE;
}

// ��簡�� �ش��ϴ� row ã��
int  CTabCodeSearchCommodity::GetGridRow(int nExercise)
{
	// int(���ڸ�) -> long(�ټ��ڸ�) (��:92 ==> 9250 : 92.50)
	long lPrice = GetRealExercise(nExercise);

	return TITLE_ROW + ((m_lMaxLimit - lPrice)); 
}

// row�� �ش��ϴ� ��簡 ã��
int  CTabCodeSearchCommodity::GetGridExercise(int row)
{
	long lPrice = m_lMaxLimit - (row - TITLE_ROW)* 250; 

	return (int)(lPrice / 100);
}

// double -> string data ���� & redraw
void CTabCodeSearchCommodity::UpdateHogaData(int nRow, int nCol, double dPrice, COLORREF crFgClr, int nPoint/*=2*/)
{

	CString strText;
	strText.Format("%.*f", nPoint, dPrice);

	// �ؽ�Ʈ ����
	m_pGridCtrl->SetItemText(nRow, nCol, strText);

	// ���� ����ŷ �Ǿ� ������ ���� ����
	m_pGridCtrl->SetItemFgColour(nRow, nCol, crFgClr);

	m_pGridCtrl->RedrawCell(nRow, nCol);
}

// ��簡�� set
long CTabCodeSearchCommodity::GetRealExercise(int nPrice)
{ 
	long lPrice = nPrice / 10.; 
	return lPrice;
}

void CTabCodeSearchCommodity::OnEditchangeComboMonth() 
{
	// TODO: Add your control notification handler code here
	int nSel = m_ctrlComboMonth.GetCurSel();
	InsertMasterData(nSel);
}

void CTabCodeSearchCommodity::OnSelchangeComboMonth() 
{
	// TODO: Add your control notification handler code here
	m_nIndex = m_ctrlComboMonth.GetCurSel() +1;
	InsertGridData();
	InsertMasterData(m_nIndex);
}

LRESULT CTabCodeSearchCommodity::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(message == WM_NOTIFY)
	{
		NM_GRIDVIEW *pNmgv = (NM_GRIDVIEW *)lParam;
		int nCode = pNmgv->hdr.code;

		if(nCode == GVN_SELCHANGING)			// Left button click
		{
			if(pNmgv->hdr.hwndFrom == m_pGridCtrl->m_hWnd) // �ɼ� �ܰ� �׸��忡�� ���콺 Ŭ���� �����ڵ� ó��
			{
				BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
				if(!bInsertMode)
				{
					CString str;
					str = m_pGridCtrl->GetItemText(pNmgv->iClickRow,pNmgv->iClickCol);

					// �ŷ���������� �ƴѰ�� return
					if (str.Find("X") >= 0)						
						return TRUE;

					CString strCodeSelected;
					GetCodeInfo(1, pNmgv->iClickRow, pNmgv->iClickCol, strCodeSelected);
					if(!strCodeSelected.IsEmpty())
					{
						STINPUTCODEDATA stInputCodeData;
						stInputCodeData.m_strCode = strCodeSelected;
						m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG,NULL,(LPARAM)&stInputCodeData);
					}
					return TRUE;
				}	
				// Nonzero if message was handled; otherwise 0.
				//				return fnWhenClickGridJango(FALSE, pNmgv->iClickRow, pNmgv->iClickCol);
			}
		}
		else if(nCode == NM_DBLCLK)			// Left button click
		{
			if(pNmgv->hdr.hwndFrom == m_pGridCtrl->m_hWnd) // �ɼ� �ܰ� �׸��忡�� ���콺 Ŭ���� �����ڵ� ó��
			{
				BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
				if(bInsertMode)
				{
					CString str;
					str = m_pGridCtrl->GetItemText(pNmgv->iClickRow,pNmgv->iClickCol);

					// �ŷ���������� �ƴѰ�� return
					if (str.Find("X") >= 0)						
						return TRUE;

					CString strCodeSelected;
					GetCodeInfo(1, pNmgv->iClickRow, pNmgv->iClickCol, strCodeSelected);
					if(!strCodeSelected.IsEmpty())
					{
						STINPUTCODEDATA stInputCodeData;
						stInputCodeData.m_strCode = strCodeSelected;
						m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG,NULL,(LPARAM)&stInputCodeData);
					}
					return TRUE;
				}	
				// Nonzero if message was handled; otherwise 0.
				//				return fnWhenClickGridJango(FALSE, pNmgv->iClickRow, pNmgv->iClickCol);
			}
		}
	}	
	return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CTabCodeSearchCommodity::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CTabDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		{
			pDC->SetBkColor(m_crBk);
			return m_brBkColor;
		}		
		break;
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

///////////////////////////////////////////////////////////////////////////////
// ������            : ������(YouJeong, Lee)
// Email             : lotte222@dooriic.co.kr
// ���� ��¥         : 2004/2/6
// �Լ� �̸�         : CDlgSetup::GetCodeInfo
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : 
// �Ķ����
//		int nType : ��û�ϴ� ����(ROWCOL : ��/�� ����, CODE:�����ڵ�)
//		int& row :  
//		int& col : 
//		st_Code& code  : 
// �Լ� ����         : ��/������ <-> �����ڵ� ��ȣ ��ȯ
///////////////////////////////////////////////////////////////////////////////
BOOL CTabCodeSearchCommodity::GetCodeInfo(int nType, int& row, int& col, CString& strCode)
{
	int nRowTotal = m_pGridCtrl->GetRowCount();
	if (row < TITLE_ROW || row > nRowTotal || col == COL_CENTER)		return FALSE;

	int nGubun;
	if (col < COL_CENTER)
		nGubun = 2;
	else
		nGubun = 3;

	int nYear = (int)(m_nMonth.nInfo[m_nIndex] / 100);
	int nMonth = m_nMonth.nInfo[m_nIndex] % 100; 

	double dEvtPrice = atof(m_pGridCtrl->GetItemText(row, COL_CENTER));
//@��������
//	return m_pDataManager->GetCommodityOptionItemCode(m_nOptionMarket, nGubun, nYear, nMonth, dEvtPrice, strCode);
//@��������
	return TRUE;
}

void CTabCodeSearchCommodity::OnSelchangeListFuture() 
{
	// TODO: Add your control notification handler code here
	m_listFuture2.ResetContent();

	int nSel = m_listFuture.GetCurSel();
	if(nSel == LB_ERR)
		return;

	UpdateData();

	CStringArray* pastrJongName;	// �ɼ� ���� �� 
	BYTE nMarket = m_listFuture.GetItemData(nSel);

	pastrJongName = new CStringArray;
//@��������
//	m_pDataManager->GetMarketItemName(nMarket, pastrJongName, m_bSpread);
//@��������


	for(int i = 0; i < pastrJongName->GetSize(); i++)
	{
		m_listFuture2.AddString(pastrJongName->GetAt(i));
		m_listFuture2.SetItemData(i, nMarket);
	}

	delete pastrJongName;	// �ɼ� ���� �� 

	//003 2006.06.30 
	// �ɼ�
	m_listMaster.RemoveAll();
	m_ctrlComboMonth.ResetContent();
	for (i=0; i < 5; i++)
		m_nMonth.nInfo[i] = 0;

	CStringArray* pastrJongCode;	// �ɼ� ���� �ڵ� 
	CIntArray* panCallPut;
	CIntArray* panYear;
	CIntArray* panMonth;
	CIntArray* panEvtPrice;

	pastrJongCode = new CStringArray;
	panCallPut = new CIntArray;
	panYear = new CIntArray;
	panMonth = new CIntArray;
	panEvtPrice = new CIntArray;

	int nIndex = 0;
	if(nMarket == 13)
		m_nOptionMarket = 14;
	else
		m_nOptionMarket = -1;

//@��������
//	m_pDataManager->GetCommodityOptionItemCode(m_nOptionMarket, pastrJongCode, panCallPut, panYear, panMonth, panEvtPrice);
//@��������

	// �� �� �ɼ��� ������ ���� �������� ���� 
	int nCount = pastrJongCode->GetSize();
	// �� �� �ɼ��� ������ ���� �������� ���� 
	if(nCount)
	{
		COptionItem **pData = new COptionItem * [nCount];
		for(int i=0; i<pastrJongCode->GetSize(); i++)
		{
			pData[i] = new COptionItem;
			pData[i]->SetItem(pastrJongCode->GetAt(i), panCallPut->GetAt(i), panYear->GetAt(i), panMonth->GetAt(i), panEvtPrice->GetAt(i));
		}

		::qsort(pData, nCount, sizeof(void*), OptEvtPriceItemFuncCommodity);

		for(i=0; i<nCount; i++)
		{
			pastrJongCode->SetAt(i, pData[i]->m_strCode);
			panCallPut->SetAt(i, pData[i]->m_nCallPut);
			panYear->SetAt(i, pData[i]->m_nYear);
			panMonth->SetAt(i, pData[i]->m_nMonth);
			panEvtPrice->SetAt(i, pData[i]->m_nEvtPrice);
			delete pData[i];
			//m_OptEvtPriveList.m_lstEvtPriveItem.Add(pData[i]);
		}

		delete pData;
	}

	//	if (pListP == NULL)		return FALSE;
	CString  strCode, strMonth;
	int		 nGubun, nMonth, nExercise, nMonthNow; 
	int		 nIndexMonth = EXERCISE;
	BOOL	 bFind;

	nMonthNow = 0;
	int MaxExercise = 0;
	for(nIndex=0; nIndex < pastrJongCode->GetSize(); nIndex++)
	{
		strCode = pastrJongCode->GetAt(nIndex); 	
		strCode.TrimLeft();		strCode.TrimRight();

		if (strCode.GetLength() >= 8)// && nGubun != 3)
		{
			//			nGubun = atoi(strCode.Left(1));
			CString strTemp;
			strTemp = strCode.Left(1);
			nGubun = atoi(strTemp);

			if (nGubun == 3)	 break;		// �� �����͸� read

			nMonth = (panYear->GetAt(nIndex) * 100) + panMonth->GetAt(nIndex);
			nExercise = panEvtPrice->GetAt(nIndex);

			if (MaxExercise < nExercise)		MaxExercise = nExercise;

			if (nMonthNow != nMonth)	
			{	
				nIndexMonth++;
				m_nMonth.nInfo[nIndexMonth] = nMonthNow = nMonth; 
			}

			bFind = FALSE;
			for(POSITION pos=m_listMaster.GetHeadPosition(); pos; )
			{
				codeinfo&	info  = m_listMaster.GetNext(pos);
				if (info.nInfo[EXERCISE] == nExercise)
				{
					info.nInfo[nIndexMonth] = 1;
					bFind = TRUE;
					break;
				}
			}

			if (!bFind)
			{
				codeinfo	info;
				info.nInfo[EXERCISE] = nExercise;
				info.nInfo[nIndexMonth] = 1;

				m_listMaster.AddTail(info);
			}
		}
	}

	for(i=1; i<5; i++)
	{
		if(m_nMonth.nInfo[i])
		{
			strCode.Format("%d��", m_nMonth.nInfo[i] % 100);
			m_ctrlComboMonth.AddString(strCode);
		}
	}
	m_ctrlComboMonth.SetCurSel(0);

	// �ְ� ��簡 ����
	if (MaxExercise > 0)	m_lMaxLimit = GetRealExercise(MaxExercise);

	delete pastrJongCode;
	delete panCallPut;
	delete panYear;
	delete panMonth;

	InitGridRowCol();

	// �׸����� ����� �ٲ۴�
	ChangeGridRowCol();

	// �׸��忡 �����͸� �߰��Ѵ�.
	InsertGridData();
	InsertMasterData(m_nIndex);			// master file data�� grid edit����/color�� setting
}

void CTabCodeSearchCommodity::OnSelchangeListFuture2() 
{
	// TODO: Add your control notification handler code here
	BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
	if(!bInsertMode)
	{
		int nSel = m_listFuture2.GetCurSel();
		if(nSel == LB_ERR)
			return;

		UpdateData();

		BYTE nMarket = m_listFuture2.GetItemData(nSel);
		CString strItemName;
		m_listFuture2.GetText(nSel, strItemName);

		CString strCodeSelected = _T("");
//@��������
//		m_pDataManager->GetMarketItemCode(nMarket, strItemName, strCodeSelected, m_bSpread);
//@��������

		if(!strCodeSelected.IsEmpty())
		{
			STINPUTCODEDATA stInputCodeData;
			stInputCodeData.m_strCode = strCodeSelected;
			m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG,NULL,(LPARAM)&stInputCodeData);
		}
	}	
}

void CTabCodeSearchCommodity::OnBnClickedRdoFuture()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!m_bSpread)
		return;

	OnSelchangeListFuture();
}

void CTabCodeSearchCommodity::OnBnClickedRdoSpread()
{
	if(m_bSpread)
		return;

	OnSelchangeListFuture();
}
