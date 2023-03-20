// TabCodeSearchFO.cpp : implementation file
//

#include "stdafx.h"
//#include "multichart.h"
#include "TabCodeSearchFO.h"
#include "../chart_common/Grid/ColorTable.h"
#include "./define/ChartMsgDef.h"
#include "StdDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearchFO dialog
#define ROW_HEIGHT 15
#define ROW_WIDTH  46
#define COL_CENTER 1
#define TITLE_ROW 1
enum		{EXERCISE, FIRST, SECOND, THIRD, FOURTH};	// �� �ɼ� ���� 

CTabCodeSearchFO::CTabCodeSearchFO(CWnd* pParent /*=NULL*/)
	: CTabDlg(CTabCodeSearchFO::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabCodeSearchFO)
		// NOTE: the ClassWizard will add member initialization here
	m_pGridCtrl = NULL;
	m_nIndex = 1;
	m_strCodeNName = "";
	m_strSelCodeNName = "";

	m_nRadioSel = 1;
	//}}AFX_DATA_INIT
}


void CTabCodeSearchFO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabCodeSearchFO)
	DDX_Control(pDX, IDC_COMBO_MONTH, m_ctrlComboMonth);
	DDX_Control(pDX, IDC_LIST_FUTURE, m_listFuture);
	DDX_Control(pDX, IDC_RADIO_KOSPI200, m_ctrlRadioKospi200);
	DDX_Control(pDX, IDC_RADIO_MINIKOSPI, m_ctrlRadioMiniKospi);
	DDX_Control(pDX, IDC_RADIO_WEEKLYKOSPI, m_ctrlRadioWeeklyKospi);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabCodeSearchFO, CDialog)
	//{{AFX_MSG_MAP(CTabCodeSearchFO)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_CBN_EDITCHANGE(IDC_COMBO_MONTH, OnEditchangeComboMonth)
	ON_CBN_SELCHANGE(IDC_COMBO_MONTH, OnSelchangeComboMonth)
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_LIST_FUTURE, OnSelchangeListFuture)
	ON_LBN_DBLCLK(IDC_LIST_FUTURE, OnDblclkListFuture)
	ON_BN_CLICKED(IDC_RADIO_KOSPI200, OnClickRadioKospi200)
	ON_BN_CLICKED(IDC_RADIO_MINIKOSPI, OnClickRadioMiniKospi)
	ON_BN_CLICKED(IDC_RADIO_WEEKLYKOSPI, OnClickRadioWeeklyKospi)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearchFO message handlers
void CTabCodeSearchFO::OnSize(UINT nType, int cx, int cy) 
{
	CTabDlg::OnSize(nType, cx, cy);	
	// TODO: Add your message handler code here
	RecalcLayout();	
}

BOOL CTabCodeSearchFO::OnInitDialog() 
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

CRect CTabCodeSearchFO::GetRectOfCtrl(INT nID,BOOL bInit/*=FALSE*/)
{
	CRect rect;
	GetClientRect(rect);
	int nWidth = rect.Width();
//	rect.DeflateRect(GAP_TABDLG,GAP_TABDLG,GAP_TABDLG,GAP_TABDLG);
	switch(nID)
	{
	case IDC_LIST_FUTURE:
//		rect.top = rect.top + HEIGHTOFCOMBO + GAP_TABDLG;
		rect.left += 1;
		rect.top = rect.top + HEIGHTOFCOMBO * 2 + GAP_TABDLG * 2 + 4;
		//rect.bottom = rect.bottom - HEIGHTOFJONGSTATUS - GAP_TABDLG - HEIGHTOFBUTTON - GAP_TABDLG;
		rect.bottom = rect.top + 100;
		break;		
	case ID_GRIDCTRL:
		rect.left += 1;
		rect.top = rect.top + HEIGHTOFCOMBO * 2 + GAP_TABDLG + 105;
		//rect.top = rect.bottom - GAP_TABDLG - HEIGHTOFJONGSTATUS - HEIGHTOFBUTTON;
		rect.bottom = rect.bottom;// - GAP_TABDLG;// - HEIGHTOFBUTTON;
		break;
	default:
		break;
	}
	return rect;
}

void CTabCodeSearchFO::RecalcLayout()
{	
	RecalcLayoutCtrl(m_listFuture);
	if(m_pGridCtrl)
		RecalcLayoutCtrl(*m_pGridCtrl,ID_GRIDCTRL);	
}

void CTabCodeSearchFO::OnDestroy() 
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
void CTabCodeSearchFO::InitGrid()
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

void CTabCodeSearchFO::InitGridRowCol()
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

void CTabCodeSearchFO::ChangeGridRowCol()
{
	int nCol = m_pGridCtrl->GetColumnCount();		// Column Count ���ϱ�
	int nRow = m_pGridCtrl->GetRowCount();			// Row Count ���ϱ�

	// fill rows/cols with text
	for (int row = 0; row < nRow; row++)
		m_pGridCtrl->SetRowHeight(row, ROW_HEIGHT);
	
	for (int col = 0; col < nCol; col++)
		m_pGridCtrl->SetColumnWidth(col, ROW_WIDTH);

	m_pGridCtrl->SetColumnWidth(COL_CENTER, ROW_WIDTH + 12);		

	m_pGridCtrl->Refresh();
}

// �� ������ ���� �� �߰�
// ���߿� SetItemText(CString) ������ �����͸� �߰��� �� �ְ� �̸� ����
void CTabCodeSearchFO::InsertGridData()
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

CString CTabCodeSearchFO::GetInsertGridDataText(int row, int col)
{
	
	if (row != 0 )		return "";

	if (col == COL_CENTER)	return "��簡";
	else if (col < COL_CENTER)	return "��";
	else if (col > COL_CENTER)	return "ǲ";

//	int nIndex;
//	if (col < COL_CENTER)	nIndex = COL_CENTER	- col;
//	else					nIndex = col - COL_CENTER;

	CString strText;
	strText.Format("%d��", m_nMonth.nInfo[m_nIndex]);
	return strText;
}

// master file data�� grid edit����/color�� setting
void CTabCodeSearchFO::InsertMasterData(int index)
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
	for (POSITION pos = m_listMaster.GetHeadPosition(); pos; )
	{
		codeinfo& info = m_listMaster.GetNext(pos);
		
		nExercise = info.nInfo[EXERCISE];
		nRow = GetGridRow(nExercise);
		
		// �� check
		if (nRow < TITLE_ROW || nRow >= nRowTotal)		continue;
		
		clrText = (nATM == nExercise) ? COLOR_RED : COLOR_BLACK;
		UpdateHogaData(nRow, COL_CENTER, GetRealExercise(nExercise)/100. , clrText, 1);
//		for (int i=FIRST; i <= FOURTH; i++)
//		{
			if (info.nInfo[index] == 0)	
				continue;
			
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

int CompareFileName(const void* p1, const void* p2)
{
    TCHAR *sz1 = ((CString*)(p1))->GetBuffer(0);
    TCHAR *sz2 = ((CString*)(p2))->GetBuffer(0);
    int nPos1 = -1;
    int nPos2 = -1;
    int nEndPos1;
    int nEndPos2;
    int nResult;
    while (1)
    {
        nPos1++;
        nPos2++;
        // Make sure we haven't hit the end of either of the strings
        if (sz1[nPos1] == 0 && sz2[nPos2] == 0)
            return 0;
        if (sz1[nPos1] == 0)
            return -1;
        if (sz2[nPos2] == 0)
            return 1;
        // See if this part of both strings is a number
        if (sz1[nPos1] >= _T('0') && sz1[nPos1] <= _T('9') &&
            sz2[nPos2] >= _T('0') && sz2[nPos2] <= _T('9'))
        {
            // Find the end of each number
            nEndPos1 = nPos1;
            do
            {
                nEndPos1++;
            } while (sz1[nEndPos1] >= _T('0') && sz1[nEndPos1] <= _T('9'));
            nEndPos2 = nPos2;
            do
            {
                nEndPos2++;
            } while (sz2[nEndPos2] >= _T('0') && sz2[nEndPos2] <= _T('9'));
            while (true)
            {
                if (nEndPos1 - nPos1 == nEndPos2 - nPos2)
                {
                    // Both numbers are the same length, just compare them
                    nResult = _tcsnicmp(sz1 + nPos1, sz2 + nPos2, nEndPos1 - nPos1);
                    if (nResult == 0)
                    {
                        nPos1 = nEndPos1 - 1;
                        nPos2 = nEndPos2 - 1;
                        break;
                    }
                    else
                    {
                        return nResult;
                    }
                }
                else if (nEndPos1 - nPos1 > nEndPos2 - nPos2)
                {
                    // First number is longer, so if it's not zero padded, it's bigger
                    if (sz1[nPos1] == _T('0'))
                        nPos1 ++;
                    else
                        return 1;
                }
                else
                {
                    // Second number is longer, so if it's not zero padded, it's bigger
                    if (sz2[nPos2] == _T('0'))
                        nPos2 ++;
                    else
                        return -1;
                }
            }
        }
        else
        {
            // One or both characters is not a number, so just compare them as a string
            nResult = _tcsnicmp(sz1 + nPos1, sz2 + nPos2, 1);
            if (nResult != 0)
            {
                return nResult;
            }
        }
    }
}

void SortFileName(CStringArray& arrStr)
{
    qsort(arrStr.GetData(), arrStr.GetSize(), sizeof(CString*), CompareFileName);
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
BOOL CTabCodeSearchFO::InitMasterData()
{
	int nID = 5;
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	m_pDataManager = (IMasterDataManagerLast*)pGateMng->GetInterface(nID);

	if (m_pDataManager == NULL)		return FALSE;

	m_listMaster.RemoveAll();
	m_listFuture.ResetContent();
	m_ctrlComboMonth.ResetContent();

	for (int i=1; i<12; i++)
		m_nMonth.nYear[i] = -1;

//003 2006.06.30 
	// �ɼ�
	CStringArray* pastrJongCode;	// �ɼ� ���� �ڵ� 
	CStringArray* pastrJongName;	// �ɼ� ���� �� 
	CStringArray* pastrJongEngName;	// �ɼ� ���� ���� ��
	CStringArray* pastrEventPrice;	// ��� ����
	CStringArray* pastrFinalYM;		// ���� ���
	CStringArray* pastrValueGubun;	// ���� ����

	pastrJongCode = new CStringArray;
	pastrJongName = new CStringArray;
	pastrJongEngName = new CStringArray;
	pastrEventPrice = new CStringArray;
	pastrFinalYM = new CStringArray;
	pastrValueGubun = new CStringArray;
	int nIndex = 0;

	if( m_nRadioSel == 1 )
	{
		m_ctrlRadioKospi200.SetCheck(TRUE);
		m_pDataManager->GetOptionJongMst(pastrJongCode, pastrJongName, pastrJongEngName, pastrEventPrice, pastrFinalYM, pastrValueGubun);
	}
	else if( m_nRadioSel == 2 )
	{
		m_ctrlRadioWeeklyKospi.SetCheck(TRUE);
		m_pDataManager->GetWeeklyOptionJongMst(pastrJongCode, pastrJongName);
	}
	else
	{
		m_ctrlRadioMiniKospi.SetCheck(TRUE);
		m_pDataManager->GetMiniOptionJongMst(pastrJongCode, pastrJongName);
	}

	CStringArray arrCode;
	CStringArray arrName;
	
	arrName.Copy(*pastrJongName);
	
	SortFileName(arrName);
	
	for(int index = 0; index < arrName.GetSize(); index++)
	{
		for(int j = 0; j < pastrJongName->GetSize(); j++)
		{
			if(arrName.GetAt(index) == pastrJongName->GetAt(j))
				arrCode.Add(pastrJongCode->GetAt(j));
		}
	}

	pastrJongCode->RemoveAll();
	pastrJongName->RemoveAll();
	
	pastrJongCode->Copy(arrCode);
	pastrJongName->Copy(arrName);

//	if (pListP == NULL)		return FALSE;
	CString  strCode, strMonth, strCodeName;
	int		 nGubun, nMonth, nExercise, nMonthNow; 
	int		 nIndexMonth = EXERCISE;
	BOOL	 bFind;

	nMonthNow = 0;
	int MaxExercise = 0;
//	codeinfo oldInfo; oldInfo.nInfo[EXERCISE]=0;
	for(nIndex=0; nIndex < pastrJongCode->GetSize(); nIndex++)
//	for(nIndex=pastrJongCode->GetSize()-1; nIndex >=0 ; nIndex--)
	{
		strCode = pastrJongCode->GetAt(nIndex);
		//strCode.TrimLeft();		strCode.TrimRight();
		strCodeName = pastrJongName->GetAt(nIndex);

		if (strCode.GetLength() >= 8)// && nGubun != 3)
		{
//			nGubun = atoi(strCode.Left(1));
			CString strTemp;
			strTemp = strCode.Left(1);
			nGubun = atoi(strTemp);

			if (nGubun == 3)	 continue;		// �� �����͸� read
			//@Solomon if (nGubun == 3)	 break;		// �� �����͸� read
			//if (nGubun == 3)	 continue;		// �� �����͸� read

			if( m_nRadioSel == 2 )
			{
				nMonth = atoi( strCodeName.Mid( 4, 2) );
			}
			else
			{
				strMonth = strCode.Mid(4, 1);		strMonth.MakeUpper();
				if (strMonth >= "A" && strMonth <= "C")		nMonth = (strMonth[0] - 'A')+10;
				else										nMonth = atoi(strMonth);
			}
			nExercise = atoi(strCode.Mid(5, 3));

			if (MaxExercise < nExercise)		MaxExercise = nExercise;

			if (nMonthNow != nMonth)	
			{	
				nIndexMonth++;
				if(nIndexMonth==12) break;

				m_nMonth.nInfo[nIndexMonth] = nMonthNow = nMonth;

				if( m_nRadioSel == 2 )
				{
					m_nMonth.nYear[nIndexMonth] = atoi( strCodeName.Mid( 2, 2) );
					m_cYear[nIndexMonth] = strCodeName.GetAt(7);
					m_nWeeklyNo[nIndexMonth] = atoi( strCode.Mid( 3, 2) );
				}
				else
				{
					m_nMonth.nYear[nIndexMonth] = atoi( strCodeName.Mid( 2, 4) );
					m_cYear[nIndexMonth] = strCode.GetAt(3); 
				}
				//m_cYear = strCode.GetAt(3);
			}

			bFind = FALSE;
//			if(oldInfo.nInfo[EXERCISE] == nExercise)
//				bFind = TRUE;
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

//			oldInfo.nInfo[EXERCISE] = nExercise;
		}
	}

	if( m_nRadioSel == 1 )
	{
		for(int i=1; i<12; i++)
		{
			if (m_nMonth.nYear[i] < 0)
				continue;
			strCode.Format("%d%02d", m_nMonth.nYear[i], m_nMonth.nInfo[i]);
			m_ctrlComboMonth.AddString(strCode);
		}
	}
	else if( m_nRadioSel == 2 )
	{
		for(int i=1; i<12; i++)
		{			
			if (m_nMonth.nYear[i] < 0)
				continue;
			strCode.Format("%02d��%c��", m_nMonth.nInfo[i], m_cYear[i]);
			m_ctrlComboMonth.AddString(strCode);
		}
	}
	else
	{
		for(int i=1; i<6; i++)
		{			
			if (m_nMonth.nYear[i] < 0)
				continue;
			strCode.Format("%d%02d", m_nMonth.nYear[i], m_nMonth.nInfo[i]);
			m_ctrlComboMonth.AddString(strCode);
		}
	}
	m_ctrlComboMonth.SetCurSel(0);

	// �ְ� ��簡 ����
	if (MaxExercise > 0)	m_lMaxLimit = GetRealExercise(MaxExercise);

	delete pastrEventPrice;
	delete pastrFinalYM;
	delete pastrValueGubun;

	//////////////////////////////////////////////////////////////////////////
	// ����
	pastrJongCode->RemoveAll();
	pastrJongName->RemoveAll();
	pastrJongEngName->RemoveAll();

	if( m_nRadioSel == 2 )
		return TRUE;

	if( m_nRadioSel )
		m_pDataManager->GetFutureJongMst(pastrJongCode, pastrJongName, pastrJongEngName);
	else
		m_pDataManager->GetMiniFutureJongMst(pastrJongCode, pastrJongName, pastrJongEngName);

	int iIdx = 0;
	CString strName;
	for(nIndex=0; nIndex < pastrJongCode->GetSize(); nIndex++)
	{
		strCode = pastrJongCode->GetAt(nIndex); 	
		strCode.TrimLeft();		strCode.TrimRight();
		strName = pastrJongName->GetAt(nIndex);
		iIdx = m_listFuture.AddString(strName);
	}
	delete pastrJongCode;
	delete pastrJongName;
	delete pastrJongEngName;
// }}

	return TRUE;
}

// ��簡�� �ش��ϴ� row ã��
int  CTabCodeSearchFO::GetGridRow(int nExercise)
{
	// int(���ڸ�) -> long(�ټ��ڸ�) (��:92 ==> 9250 : 92.50)
	long lPrice = GetRealExercise(nExercise);

    return TITLE_ROW + ((m_lMaxLimit - lPrice) / 250); 
}

// row�� �ش��ϴ� ��簡 ã��
int  CTabCodeSearchFO::GetGridExercise(int row)
{
	long lPrice = m_lMaxLimit - (row - TITLE_ROW)* 250; 

	return (int)(lPrice / 100);
}

// double -> string data ���� & redraw
void CTabCodeSearchFO::UpdateHogaData(int nRow, int nCol, double dPrice, COLORREF crFgClr, int nPoint/*=2*/)
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
long CTabCodeSearchFO::GetRealExercise(int nPrice)
{ 
	long lPrice = nPrice * 100; 
	if (lPrice % 500) lPrice += 50;	
	return lPrice;
}

void CTabCodeSearchFO::OnEditchangeComboMonth() 
{
	// TODO: Add your control notification handler code here
	int nSel = m_ctrlComboMonth.GetCurSel();
	InsertMasterData(nSel);
}

void CTabCodeSearchFO::OnSelchangeComboMonth() 
{
	// TODO: Add your control notification handler code here
	m_nIndex = m_ctrlComboMonth.GetCurSel() +1;
	InsertGridData();
	InsertMasterData(m_nIndex);
}

LRESULT CTabCodeSearchFO::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
		case UMSG_ADDITEM_BTNCLICK:
			if(!m_strSelCodeNName.GetLength()) return 0l;
			GetParent()->GetParent()->SendMessage(UMSG_ADDITEM, 0, (LPARAM)&(m_strSelCodeNName));
			break;				
	}	
	if(message == WM_NOTIFY)
	{
		NM_GRIDVIEW *pNmgv = (NM_GRIDVIEW *)lParam;
		int nCode = pNmgv->hdr.code;
		
		if(nCode == GVN_SELCHANGED)			// Left button click
		{
			if(pNmgv->hdr.hwndFrom == m_pGridCtrl->m_hWnd) // �ɼ� �ܰ� �׸��忡�� ���콺 Ŭ���� �����ڵ� ó��
			{
				CString strCodeSelected;
				GetCodeInfo(1, pNmgv->iClickRow, pNmgv->iClickCol, strCodeSelected);
				if(!strCodeSelected.IsEmpty())
				{
					CString strCode = strCodeSelected;
					CString strName;

					CStringArray* pastrJongCode = new CStringArray;
					CStringArray* pastrJongName = new CStringArray;
					CStringArray* pastrJongEngName = new CStringArray;
					CStringArray* pastrEventPrice = new CStringArray;
					CStringArray* pastrFinalYM = new CStringArray;
					CStringArray* pastrValueGubun = new CStringArray;

					if( m_nRadioSel == 1 )
						m_pDataManager->GetOptionJongMst(pastrJongCode, pastrJongName, pastrJongEngName, pastrEventPrice, pastrFinalYM, pastrValueGubun);
					else if( m_nRadioSel == 2 )
						m_pDataManager->GetWeeklyOptionJongMst(pastrJongCode, pastrJongName);
					else
						m_pDataManager->GetMiniOptionJongMst(pastrJongCode, pastrJongName);

					for(int i = 0; i < pastrJongCode->GetSize(); i++)
					{
						strCodeSelected = pastrJongCode->GetAt(i);
						if(pastrJongCode->GetAt(i).Find(strCode) >= 0)
						{
							strCode = pastrJongCode->GetAt(i);
							strName = pastrJongName->GetAt(i);
							break;
						}
					}

					delete pastrJongCode;
					delete pastrJongName;
					delete pastrJongEngName;
					delete pastrEventPrice;
					delete pastrFinalYM;
					delete pastrValueGubun;

					strName.TrimLeft();			

					if(strName.GetLength()<=0)
						return TRUE;
					CString strData;
					strData.Format("%s|%s", strName, strCode);
					if(strData != m_strCodeNName)
					{
						AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
						m_strCodeNName = strData;
						GetParent()->GetParent()->SendMessage(UMSG_ADDITEM, 0, (LPARAM)&(m_strCodeNName));
					}
				}
			}
		}
	}	
	return CTabDlg::WindowProc(message, wParam, lParam);
}

HBRUSH CTabCodeSearchFO::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
BOOL CTabCodeSearchFO::GetCodeInfo(int nType, int& row, int& col, CString& strCode)
{
	BOOL bRet = FALSE;
	CString strMonth;
	int nMonth, nExercise, nGubun, nIndex;
//	switch (nType)
//	{
//		case ROWCOL:
//			{
//				if (strCode.GetLength() < 8)		return FALSE;
//				
//				nGubun = atoi(strCode.Left(1));
//				strMonth = strCode.Mid(4, 1);				strMonth.MakeUpper();
//				if (strMonth >= "A" && strMonth <= "C")		nMonth = (strMonth[0] - 'A')+10;
//				else										nMonth = atoi(strMonth);
//				
//				nExercise = atoi(strCode.Mid(5, 3));
//
//				for (int i=FIRST; i <= FOURTH; i++)	
//				{	if (nMonth == m_nMonth.nInfo[i])		break;		}
//
//				if (i > FOURTH)		return FALSE;
//
//				row = GetGridRow(nExercise);
//				col = (nGubun == 2) ? (COL_CENTER - i) : (COL_CENTER + i);
//				bRet = TRUE;
//			}
//			break;
//
//		case CODE:
//			{
				int nRowTotal = m_pGridCtrl->GetRowCount();
				if (row < TITLE_ROW || row > nRowTotal || col == COL_CENTER)		return FALSE;

				if (col < COL_CENTER)	{ nGubun = 2; nIndex = COL_CENTER - col; }
				else					{ nGubun = 3; nIndex = col - COL_CENTER; }

				nMonth = m_nMonth.nInfo[m_nIndex]; 
				//nYear = m_cYear[m_nIndex];
				nExercise = GetGridExercise(row);

				if( m_nRadioSel == 1 )
					strCode.Format("%1d%02d%1c%1X%03d", nGubun, 1, m_cYear[m_nIndex], nMonth, nExercise);
				else if( m_nRadioSel == 2 )
					strCode.Format("%1d%02d%02d%03d", nGubun, 9, m_nWeeklyNo[m_nIndex], nExercise);
				else
					strCode.Format("%1d%02d%1c%1X%03d", nGubun, 5, m_cYear[m_nIndex], nMonth, nExercise);

				bRet = TRUE;

//			}
//			break;
//	}

	return bRet;

}

void CTabCodeSearchFO::OnSelchangeListFuture() 
{
	CString strCodeSelected;
	int nSel = m_listFuture.GetCurSel();

	CStringArray* pastrJongCode;
	CStringArray* pastrJongName;	// �ɼ� ���� �� 
	CStringArray* pastrJongEngName;	// �ɼ� ���� ���� ��

	pastrJongCode = new CStringArray;
	pastrJongName = new CStringArray;
	pastrJongEngName = new CStringArray;

	if( m_nRadioSel )
		m_pDataManager->GetFutureJongMst(pastrJongCode, pastrJongName, pastrJongEngName);
	else
		m_pDataManager->GetMiniFutureJongMst(pastrJongCode, pastrJongName, pastrJongEngName);

	strCodeSelected = pastrJongCode->GetAt(nSel);

	delete pastrJongCode;
	delete pastrJongName;	// �ɼ� ���� �� 
	delete pastrJongEngName;	// �ɼ� ���� ���� ��

	if(!strCodeSelected.IsEmpty())
	{
		CString strCode = strCodeSelected;
		//@Solomon CString strName = m_pDataManager->GetCodeInfo("FMASTER", 0, strCode);	
		CString strName = m_pDataManager->GetCodeName(strCode);
		strName.TrimLeft();			

		if(strName.GetLength()<=0)
			return;
		CString strData;
		strData.Format("%s|%s", strName, strCode);
		if(strData != m_strSelCodeNName)
		{
			m_strSelCodeNName = strData;
		}
	}
}


void CTabCodeSearchFO::OnDblclkListFuture() 
{
	// TODO: Add your control notification handler code here
		CString strCodeSelected;
		CString strNameSelected;
		int nSel = m_listFuture.GetCurSel();
		
		CStringArray* pastrJongCode;
		CStringArray* pastrJongName;	// �ɼ� ���� �� 
		CStringArray* pastrJongEngName;	// �ɼ� ���� ���� ��

		pastrJongCode = new CStringArray;
		pastrJongName = new CStringArray;
		pastrJongEngName = new CStringArray;
		if( m_nRadioSel )
			m_pDataManager->GetFutureJongMst(pastrJongCode, pastrJongName, pastrJongEngName);
		else
			m_pDataManager->GetMiniFutureJongMst(pastrJongCode, pastrJongName, pastrJongEngName);

		strCodeSelected = pastrJongCode->GetAt(nSel);
		strNameSelected = pastrJongName->GetAt(nSel);

		delete pastrJongCode;
		delete pastrJongName;	// �ɼ� ���� �� 
		delete pastrJongEngName;	// �ɼ� ���� ���� ��

		if(!strCodeSelected.IsEmpty())
		{
			CString strCode = strCodeSelected;
			CString strName = strNameSelected;	
			strName.TrimLeft();			

			if(strName.GetLength()<=0)
				return;
			CString strData;
			strData.Format("%s|%s", strName, strCode);

			//������ ������.
			//if(strData != m_strCodeNName)
			{
				AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
				m_strCodeNName = strData;
				GetParent()->GetParent()->SendMessage(UMSG_ADDITEM, 0, (LPARAM)&(m_strCodeNName));
			}
		}
//	}	
}

void CTabCodeSearchFO::OnClickRadioKospi200()
{
	m_nRadioSel = 1;
	InitMasterData();
	OnSelchangeComboMonth();
}

void CTabCodeSearchFO::OnClickRadioMiniKospi()
{
	m_nRadioSel = 0;
	InitMasterData();
	OnSelchangeComboMonth();
}

void CTabCodeSearchFO::OnClickRadioWeeklyKospi()
{
	m_nRadioSel = 2;
	InitMasterData();
	OnSelchangeComboMonth();
}
