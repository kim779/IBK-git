// FloatingHogaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FloatingHogaDlg.h"
//#include "../CommonTR/DrdsX11.h"
#include "../chart_common/Grid/MainDefine.h"
#include "../chart_common/Grid/ColorTable.h"
#include "../../inc/RealUpdate.h"
#include "../../inc/IMainInfoMng.h"

#include "../CommonTR/TranStruct_GNF.h"
#include	<math.h>
#include "StdDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFloatingHogaDlg dialog

#define DLG_WIDTH	222
//#define DLG_HEIGHT	265
#define DLG_HEIGHT	243

CFloatingHogaDlg::CFloatingHogaDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CFloatingHogaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFloatingHogaDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bIconState = TRUE;
	m_bCapTure	 = FALSE;
	m_pMainDlg	 = NULL;

	m_strCode				= _T("");
	m_strOldCode			= _T("");
	
	// ������ Ȯ����� ------------>
	m_bExpandRight			= FALSE;
	m_bExpandBottom			= FALSE;
	m_bOpenCol				= FALSE;
	m_bIsFirstLoad			= FALSE;
	m_bIsShowReserveCol		= FALSE;					// �ֹ�����Į�� ���̱�/���߱�
	// <----------------------------
		
	m_pGridCtrl		= NULL;	
	m_rectGrid		= CRect(0, 0, 0, 0);	// �׸��� 	
		
	m_strCurPrice		= _T("");		// ���簡
	m_strPreClosePrice	= _T("");		// ��������
	m_nHighLimit		=  0;			// ���Ѱ�
	m_nLowLimit			=  0;			// ���Ѱ�
	m_strPreVol			= _T("");		// ���ϰŷ���				JSJ_ITCastle_Add_040502
	m_strSEqualPer		= _T("");		// ���ð���� ���ϰŷ���	JSJ_ITCastle_Add_040706
	m_nMaedoPosJango	=  0;			// ���� �ŵ��ֹ� ���ɼ���
	m_nHogaRow			= -1;
	m_nStaticRow		= -1;
	m_nChartPriceRow	= -1;			// ��Ʈ���� ���� ������ �ش�ȣ����
	m_strChartPrice		= _T("");		// ��Ʈ���� ���� ����
	m_bMoveChartPriceRow= FALSE;		// ��Ʈ���� �ش翭�� ��ũ���̵�
	m_nHogaRangeForColor= -1;			// ȣ������ ����

	
	// Full Hoga ����-------------------->
	m_bIsTenHoga		= TRUE;					// 10�ܰ�ȣ�� <-> Fullȣ�� 
	m_bUseLimitHoga		= TRUE;					// ������ ��ȸ
	m_nTotalRowCnt		= -1;					// ��ü ������
	m_nCurPriceRow		= PRICE_ROW;			// ���簡��
	m_nLowLimitRow		= LOW_ROW;				// ���Ѱ� ��
	m_n5HogaSumRow		= TENHOGA_ROW_5HOSUM;	// 5�ܰ��� ǥ�ÿ�
	m_nTotalSumRow		= TENHOGA_ROW_TOTSUM;	// ��ü�� ǥ�ÿ�
	m_nExtraVolRow		= TENHOGA_ROW_EXTRAVOL;	// �ð����ܷ���
	// <----------------------------------

	m_nThreeStateDaebiType = e_DAEBI;	// ������� �ʱ� ���� 
	m_dlgOriRect = CRect(0,0,0,0);
	// end

	m_pDrdsLib = NULL;

// --> [Edit]  ������ 2008/10/23
	m_nLog			= 1;
	m_nLogDispSize	= 0;
	m_nPrecision	= 0;
	m_nTickSize		= 1;
	m_nValidPoint	= 0;
	m_nMask			= 1;

	m_nSelectRow	= -1;
	m_nSelectCol	= -1;
// <-- [Edit]  ������ 2008/10/23
}


void CFloatingHogaDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFloatingHogaDlg)
	DDX_Control(pDX, IDC_STATIC_FLOTING_GRID, m_staticGridFrame);
	DDX_Control(pDX, IDC_STATIC_SYMBOL, m_staticSymbol);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_staitcTilte);	
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_ctrlButtonClose);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFloatingHogaDlg, CRscDlg)
	//{{AFX_MSG_MAP(CFloatingHogaDlg)
	ON_WM_LBUTTONDOWN()	
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFloatingHogaDlg message handlers
void CFloatingHogaDlg::OnSize(UINT nType, int cx, int cy) 
{	
	CDialog::OnSize(nType, cx, cy);

	if(cy>100 && m_dlgOriRect.bottom>0)
	{
		if(m_pGridCtrl == NULL)
			InitGrid();
	}
}

#define WS_EX_LAYERED       0x00080000
#define LWA_ALPHA           0x00000002
BOOL CFloatingHogaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_staticSymbol.SetBkColor(RGB(185,205,230));
	m_staticSymbol.SetBkColor(RGB(92,135,183));
	m_staticSymbol.SetTextColor(CLR_FONT_DEFAULT);
	m_staticSymbol.SetTextColor(RGB(255,255,255));

	m_staitcTilte.SetBkColor(RGB(255,255,255));
	m_staitcTilte.SetTextColor(RGB(61,57,57));
	m_staitcTilte.SetText("5ȣ�� â");

	if(GetSafeHwnd())
		GetWindowRect(m_dlgOriRect);
	
	CRect rect(0,0,0,0), trect(0,0,0,0), dlgRect(0,0,0,0);

	m_staticSymbol.GetWindowRect(rect);//������ ��ǥ...
	GetWindowRect(m_dlgOriRect); //������ ��ǥ...
	
	trect.left		= rect.left	;
	trect.top		= rect.top	;
	trect.right		= rect.right;	
	trect.bottom	= rect.bottom;
	
	ScreenToClient(trect);
	if(m_bIconState)	
	{	
		CWnd* pWnd = GetParent();
		m_dlgOriRect.right = m_dlgOriRect.left + rect.Width();
		m_dlgOriRect.bottom = m_dlgOriRect.top + rect.Height();											
	}
	else
	{	
		m_dlgOriRect.right	= m_dlgOriRect.left	+ DLG_WIDTH;
		m_dlgOriRect.bottom	= m_dlgOriRect.top	+ DLG_HEIGHT;			
	}
	
	if(m_pMainDlg)
		m_pMainDlg->SendMessage(USMG_PLEASE_RESIZE, m_bIconState ? 1:0, (LPARAM)&m_dlgOriRect);

	int nID = 21;
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);

	MakeDrdsConnectAdvise();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CFloatingHogaDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
		case UMSG_RECEIVEFROMSERVER:
			{
				if(m_strCode.GetLength()>0) OnCodeUnAdvise(m_strCode);	// Old Code����
				stReceiveFromServer* pStR = (stReceiveFromServer*)lParam;				
				SetHogaData((char*)pStR->m_pBuffer,pStR->m_dwLength);
				OnCodeAdvise(m_strCode);	// New Code���
			}
		case WM_CLICKLABEL:
			{
				m_bCapTure = FALSE;
			}
			break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CFloatingHogaDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{	
	CRect rect(0,0,0,0), trect(0,0,0,0), dlgRect(0,0,0,0);

	m_staticSymbol.GetWindowRect(rect);//������ ��ǥ...
	GetWindowRect(dlgRect); //������ ��ǥ...
	
	trect.left		= rect.left	;
	trect.top		= rect.top	;
	trect.right		= rect.right;	
	trect.bottom	= rect.bottom;
	
	ScreenToClient(trect);
	if(PtInRect(trect, point))
	{
		if(m_bIconState)	
		{	
			dlgRect.right	= dlgRect.left	+ DLG_WIDTH;
			dlgRect.bottom	= dlgRect.top	+ DLG_HEIGHT;														
		}
		else
		{	
			CWnd* pWnd = GetParent();
			dlgRect.right = dlgRect.left + rect.Width();
			dlgRect.bottom = dlgRect.top + rect.Height();			
		}

		m_bIconState = !m_bIconState;
		
		if(m_pMainDlg)
			m_pMainDlg->SendMessage(USMG_PLEASE_RESIZE, m_bIconState ? 1 : 0, (LPARAM)&dlgRect);
		
		
	}
	
	CRscDlg::OnLButtonDblClk(nFlags, point);
}



void CFloatingHogaDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRscDlg::OnLButtonDown(nFlags, point);
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
}

void CFloatingHogaDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRscDlg::OnLButtonUp(nFlags, point);
}

void CFloatingHogaDlg::OnMouseMove(UINT nFlags, CPoint point) 
{	
	CRscDlg::OnMouseMove(nFlags, point);
}


void CFloatingHogaDlg::InitGrid()
{
	if (m_pGridCtrl == NULL)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CClientDC dc(this);
		int PointSize = 9;
		m_logGridFont.lfHeight = -MulDiv(PointSize, GetDeviceCaps(dc.GetSafeHdc(), LOGPIXELSY), 72);
		m_logGridFont.lfWidth = 0;
		m_logGridFont.lfEscapement = 0;
		m_logGridFont.lfOrientation = 0;
		m_logGridFont.lfWeight = FW_THIN;
		m_logGridFont.lfItalic = FALSE;
		m_logGridFont.lfUnderline = FALSE;
		m_logGridFont.lfStrikeOut = FALSE;
		m_logGridFont.lfCharSet = DEFAULT_CHARSET;
		m_logGridFont.lfOutPrecision = OUT_CHARACTER_PRECIS;
		m_logGridFont.lfClipPrecision = CLIP_CHARACTER_PRECIS;
		m_logGridFont.lfQuality = DEFAULT_QUALITY;
		m_logGridFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
		strcpy(m_logGridFont.lfFaceName, "����ü");

		if( m_GridFont.GetSafeHandle() )
			m_GridFont.DeleteObject();

		m_GridFont.CreateFontIndirect(&m_logGridFont);

		// Create the Gridctrl object
		m_pGridCtrl = new CExGridCtrl;
		if (!m_pGridCtrl) return;

		// Create the Gridctrl window
		GetDlgItem(IDC_STATIC_FLOTING_GRID)->GetWindowRect(m_rectGrid);		
		ScreenToClient(&m_rectGrid);
		//m_pGridCtrl->Create(m_rectGrid, CWnd::FromHandle(m_hWnd), 1754);
		m_pGridCtrl->Create(m_rectGrid, this, IDC_STATIC_FLOTING_GRID);
		m_pGridCtrl->SetWindowPos(NULL, m_rectGrid.left, m_rectGrid.top+1, m_rectGrid.Width(), m_rectGrid.Height()-1, SWP_NOZORDER);


		// fill it up with stuff
		m_pGridCtrl->SetEditable(TRUE);
		m_pGridCtrl->EnableDragAndDrop(FALSE);
		m_pGridCtrl->SetFont(&m_GridFont, FALSE);
		m_pGridCtrl->SetGridColor(COLOR_WHITE);
		m_pGridCtrl->SetGridLineColor(COLOR_GRIDLINE);
		m_pGridCtrl->EnableColumnHide();
		m_pGridCtrl->EnableRowHide();	

// --> [Edit]  ������ 2008/10/27	( ȣ�����̺� ���ü� ���� ���� )
		m_pGridCtrl->SetHighlightColor(RGB(125,125,255));
// <-- [Edit]  ������ 2008/10/27

		// �׸��� ����� Default�� �Ѵ�
		InitGridRowCol();
	
		// �׸����� ����� �ٲ۴�
		ChangeGrid_AllWidth();

		// �׸����� �� ����
		InitGridFormat();
		m_pGridCtrl->ShowWindow(SW_SHOW);
	}
}

void CFloatingHogaDlg::InitGridRowCol()
{	
	try {
		int nRow, nCol;

		// ������ Ÿ�Կ� �°� �׸��� ����� �ٽ� ������ �ش�
		nRow = TENHOGA_ROW_COUNT;
		nCol = TOTAL_COLUMN_COUNT;
		
		m_pGridCtrl->SetRowCount(nRow);
		m_pGridCtrl->SetColumnCount(nCol);
		m_pGridCtrl->SetFixedRowCount(FIXED_ROW_CNT_TEN);
		m_pGridCtrl->SetFixedColumnCount(0);
//		m_pGridCtrl->SetBongGraphEnable(TRUE);

		m_pGridCtrl->SetBkColor(COLOR_WHITE);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
}



// �׸��� Row, Column Width
void CFloatingHogaDlg::ChangeGrid_AllWidth()
{
	ChangeGrid_RowHeight();
	ChangeGrid_ColumnWidth();
}


// �׸��� Row Width ����
void CFloatingHogaDlg::ChangeGrid_RowHeight()
{
	int nRow = m_pGridCtrl->GetRowCount();
	// ���� ���� ����
	int row;
	for (row = 0; row < nRow; row++)
	{
		// ******** 10 ��ȣ�� **********
		if ( m_bIsTenHoga == TRUE )		
		{
			if( m_bExpandBottom == TRUE )		// Ȯ��
			{
				m_pGridCtrl->SetRowHeight(row, ROW_HEIGHT);
				//m_pGridCtrl->SetRowHeight_NoReset(row, ROW_HEIGHT);
			}
			else								// ���
			{
				if( MAEDO_ROW <= row && row <= MAEDO_ROW+4 )
				{
					m_pGridCtrl->SetRowHeight(row, 0);
					//m_pGridCtrl->SetRowHeight_NoReset(row, 0);
				}
				else if( MAESU_ROW+5 <= row && row <= MAESU_ROW+9 )
				{
					m_pGridCtrl->SetRowHeight(row, 0);
					//m_pGridCtrl->SetRowHeight_NoReset(row, 0);
				}
				else if (	(ETC_HOGAUP_ROW		== row) || 	// ��Ÿ ȣ�� ���
							(ETC_HOGADOWN_ROW	== row)	||	// ��Ÿ ȣ�� �ϴ�
							(MARKETPRICE_ROW	== row)
				)
				{
					m_pGridCtrl->SetRowHeight(row, 0);
					//m_pGridCtrl->SetRowHeight_NoReset(row, 0);
				}
				else 
				{
					m_pGridCtrl->SetRowHeight(row, ROW_HEIGHT);
					//m_pGridCtrl->SetRowHeight_NoReset(row, ROW_HEIGHT);
				}
			}
		}
		// ******** ��ü ȣ�� **********
		else
		{
			m_pGridCtrl->SetRowHeight(row, ROW_HEIGHT);
			//m_pGridCtrl->SetRowHeight_NoReset(row, ROW_HEIGHT);

		}
	}

//	m_pGridCtrl->Call_ResetScrollBars();

	m_pGridCtrl->Refresh();	
}


// �׸��� Column Width ����
void CFloatingHogaDlg::ChangeGrid_ColumnWidth()
{
	int nCol = m_pGridCtrl->GetColumnCount();
	int nColWdth[TOTAL_COLUMN_COUNT] = {0, };

	// column width "0" initialize (��ũ�� ����) ------------->>
	for (int col = 0; col < nCol; col++)
	{
		m_pGridCtrl->SetColumnWidth(col, 0);
	}
	//m_pGridCtrl->Call_ResetScrollBars();
	// <<-------------------------------------------------------


	// *****************************************************************************
	// nColGubun	0 : 10ȣ��
	//				1 : ��üȣ�� 1) ��ü ���� ������ 10ȣ�� �׸��� Į������ *����*
	//							 2)	�ϴ� Ȯ�� ����							
	//				2 : ��üȣ�� 1) ��ü ���� ������ 10ȣ�� �׸��� Į������ *����*
	//							 2) �ϴ� ��� ����
	//				3 : ��üȣ�� 1) ��ü ���� ������ 10ȣ�� �׸��� Į������ *ū���*
	// ******************************************************************************
	int nColGubun;
	if (m_bIsTenHoga == TRUE)
	{
		nColGubun = 0;
	}
	else
	{
		int nRow = m_pGridCtrl->GetRowCount();

		// ## ��ü ���� ������ 10ȣ�� �׸��� Į������ ������� ##
		if (nRow <= TENHOGA_ROW_COUNT)
		{
			if (m_bExpandBottom == TRUE)
				nColGubun = 1;
			else
				nColGubun = 2;
		}
		else
		{
			nColGubun = 3;
		}
	}


	// ���� ���� ����
	switch (nColGubun)
	{
	case 0:
	case 1:
		if ( m_bExpandRight == TRUE )			// Ȯ��-VScroll No
		{
			nColWdth[MAEDO_RESERVE_COL]	= (m_bIsShowReserveCol) ? RESERVE_COL_WIDTH : 0;
			nColWdth[LEFT_CAN_COL]	= (m_bIsShowReserveCol) ? 0 : CANCEL_COL_WIDTH;
			nColWdth[MAEDO_COL]		= MAEMAE_COL_WIDTH;
			nColWdth[MAEDO_JAN_COL] = JAN_COL_WIDTH;		
			
			//nColWdth[HOGA_COL]		= (m_bOpenCol) ? HOGA_COL_WIDTH  : HOGA_COL_WIDTH+DAEBI_COL_WIDTH;
			nColWdth[HOGA_COL]		= HOGA_COL_WIDTH;
			nColWdth[DAEBI_COL]		= (m_bOpenCol) ? DAEBI_COL_WIDTH : 0;
			
			nColWdth[MAESU_JAN_COL] = JAN_COL_WIDTH;
			nColWdth[MAESU_COL]		= MAEMAE_COL_WIDTH;
			nColWdth[RIGHT_CAN_COL] = (m_bIsShowReserveCol) ? 0 : CANCEL_COL_WIDTH;
			
			nColWdth[MAESU_RESERVE_COL]	= (m_bIsShowReserveCol) ? RESERVE_COL_WIDTH : 0;
			nColWdth[BONG_COL]		= BONG_COL_WIDTH;
		}
		else									// ���-VScroll No
		{
			nColWdth[MAEDO_RESERVE_COL] = 0;
			nColWdth[LEFT_CAN_COL]	= 0;		
			nColWdth[MAEDO_COL]		= 0;
			nColWdth[MAEDO_JAN_COL] = JAN_COL_WIDTH_SH;		
			
			//nColWdth[HOGA_COL]		= (m_bOpenCol) ? HOGA_COL_WIDTH_SH  : HOGA_COL_WIDTH_SH+DAEBI_COL_WIDTH;
			nColWdth[HOGA_COL]		= HOGA_COL_WIDTH;
			nColWdth[DAEBI_COL]		= (m_bOpenCol) ? DAEBI_COL_WIDTH	: 0;
			
			nColWdth[MAESU_JAN_COL] = JAN_COL_WIDTH_SH;
			nColWdth[MAESU_COL]		= 0;
			nColWdth[RIGHT_CAN_COL] = 0;
			
			nColWdth[MAESU_RESERVE_COL]	= 0;
			nColWdth[BONG_COL]		= BONG_COL_WIDTH;
		}
		break;

	case 2:
	case 3:
		if ( m_bExpandRight == TRUE )		// Ȯ��- VScroll Yes
		{
			nColWdth[MAEDO_RESERVE_COL]	= (m_bIsShowReserveCol) ? RESERVE_COL_WIDTH : 0;
			nColWdth[LEFT_CAN_COL]	= (m_bIsShowReserveCol) ? 0 : CANCEL_COL_WIDTH;		
			nColWdth[MAEDO_COL]		= MAEMAE_COL_WIDTH_SB;
			nColWdth[MAEDO_JAN_COL] = JAN_COL_WIDTH_SB;		
			
			//nColWdth[HOGA_COL]		= (m_bOpenCol) ? HOGA_COL_WIDTH_SB  : HOGA_COL_WIDTH_SB+DAEBI_COL_WIDTH;
			nColWdth[HOGA_COL]		= HOGA_COL_WIDTH;
			nColWdth[DAEBI_COL]		= (m_bOpenCol) ? DAEBI_COL_WIDTH : 0;
			
			nColWdth[MAESU_JAN_COL] = JAN_COL_WIDTH_SB;
			nColWdth[MAESU_COL]		= MAEMAE_COL_WIDTH_SB;
			nColWdth[RIGHT_CAN_COL] = (m_bIsShowReserveCol) ? 0 : CANCEL_COL_WIDTH;
			
			nColWdth[MAESU_RESERVE_COL]	= (m_bIsShowReserveCol) ? RESERVE_COL_WIDTH : 0;
			nColWdth[BONG_COL]		= BONG_COL_WIDTH;
		}
		else								// ���- VScroll Yes
		{
			nColWdth[MAEDO_RESERVE_COL]	= 0;
			nColWdth[LEFT_CAN_COL]	= 0;		
			nColWdth[MAEDO_COL]		= 0;
			nColWdth[MAEDO_JAN_COL] = JAN_COL_WIDTH_SH-9;		
			
			//nColWdth[HOGA_COL]		= (m_bOpenCol) ? HOGA_COL_WIDTH_SH_SB  : HOGA_COL_WIDTH_SH_SB+DAEBI_COL_WIDTH;
			nColWdth[HOGA_COL]		= HOGA_COL_WIDTH;
			nColWdth[DAEBI_COL]		= (m_bOpenCol) ? DAEBI_COL_WIDTH	: 0;
			
			nColWdth[MAESU_JAN_COL] = JAN_COL_WIDTH_SH-9;
			nColWdth[MAESU_COL]		= 0;
			nColWdth[RIGHT_CAN_COL] = 0;
			
			nColWdth[MAESU_RESERVE_COL]	= 0;
			nColWdth[BONG_COL]		= BONG_COL_WIDTH;
		}
		break;
	}

	
	// column width
	for (col = 0; col < nCol; col++)
		m_pGridCtrl->SetColumnWidth(col, nColWdth[col]);

	// ȣ��, ��� ������� �̹���
	if( m_bOpenCol == TRUE)
	{
		m_pGridCtrl->SetItemImage(TENHOGA_ROW_FIRST, HOGA_COL, 0);
		m_pGridCtrl->SetItemImage(TENHOGA_ROW_FIRST, DAEBI_COL, 2);
	}
	else
		m_pGridCtrl->SetItemImage(TENHOGA_ROW_FIRST, HOGA_COL, 1);

	m_pGridCtrl->Refresh();
}




// JSJ_040417 Modify
void CFloatingHogaDlg::InitGridFormat()
{
	int nCol = m_pGridCtrl->GetColumnCount();
	int nRow = m_pGridCtrl->GetRowCount();

	char *szTitle[] = {
		"Stop�ŵ�", "��", "�ŵ��ֹ�", "�ŵ��ܷ�", "ȣ ��", " ���", "�ż��ܷ�", "�ż��ֹ�", "��", "Stop�ż�", ""
		//"Stop�ŵ�", "��", "�ŵ��ֹ�", "�ŵ��ܷ�", "�ŵ��ܷ�", "ȣ ��", "���", "�ż��ܷ�", "�ż��ܷ�", "�ż��ֹ�", "��", "Stop�ż�", ""
		};

	if (m_nThreeStateDaebiType == e_DAEBI_PER)
		szTitle[DAEBI_COL] = "�����";
	else if (m_nThreeStateDaebiType == e_PROFIT_PER)
		szTitle[DAEBI_COL] = " ����";


	// �ֹ�����Į�� Show/Hide�� ���Į�� �� �������� ���� Ÿ��Ʋ ����
	if (!m_bIsShowReserveCol)
		szTitle[LEFT_CAN_COL] = szTitle[RIGHT_CAN_COL] = "���";

	// fill rows/cols with text
	int row, col;	
	for (row = 0; row < nRow; row++)
	{
		for (col = 0; col < nCol; col++)		// JSJ_Modify
		{ 

			GV_ITEM Item;
			Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_FGCLR|GVIF_BKCLR|GVIF_STATE;
			Item.nState = GVIS_READONLY | GVIS_MODIFIED ;//| GVIS_DROPHILITED;
			Item.row = row;
			Item.col = col;
			
			Item.nFormat = DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;//|DT_END_ELLIPSIS;

			// Text
			Item.strText = "";

			if( row == TENHOGA_ROW_FIRST )
			{
				Item.strText = szTitle[col];

				if (col == HOGA_COL)
				{
					Item.iImage = 0;
					Item.mask  |= (GVIF_IMAGE);
				}
				
				if (col == DAEBI_COL)
					Item.nFormat |= DT_LEFT;
				else
					Item.nFormat |= DT_CENTER;
			}
			else
			{
				if (col == HOGA_COL)					Item.nFormat |= DT_CENTER;
				else if (col == DAEBI_COL)				Item.nFormat |= DT_RIGHT;
				else if ((col == MAEDO_JAN_COL) ||
						 (col == MAESU_JAN_COL))		Item.nFormat |= DT_RIGHT;
//				else if( col == LEFT_CAN_COL || col == RIGHT_CAN_COL ) {}
				else
				{
					if (col == MAEDO_COL ||
						col == MAESU_COL ||
						col == MAEDO_RESERVE_COL ||
						col == MAESU_RESERVE_COL)		Item.nState = GVIS_MODIFIED;

					Item.nFormat |= DT_CENTER;
				}
			}

			
			// Color
			if(row == TENHOGA_ROW_FIRST)
			{
				Item.crBkClr = COLOR_GRIDHEADER;
				if (col == BONG_COL)
					Item.crBkClr = COLOR_WHITE;

				Item.crFgClr = COLOR_BLACK;
			}
			else if(row == HIGH_ROW)
			{
				if (col == HOGA_COL)
				{
					Item.crBkClr = COLOR_RED;
					Item.crFgClr = COLOR_WHITE;
				}
				else if (col == MAEDO_COL)
				{
					Item.crBkClr = COLOR_STATIC_MAEDO_BG;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAESU_COL)
				{
					Item.crBkClr = COLOR_STATIC_MAESU_BG;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAEDO_RESERVE_COL)
				{
					Item.crBkClr = COLOR_MAEDO_RESERVE_COL;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAESU_RESERVE_COL)
				{
					Item.crBkClr = COLOR_MAESU_RESERVE_COL;
					Item.crFgClr = COLOR_BLACK;
				}
				else
				{
					Item.crBkClr = COLOR_WHITE;
					Item.crFgClr = COLOR_BLACK;
				}
			}
			else if(row == m_nLowLimitRow)
			{
				if (col == HOGA_COL)
				{
					Item.crBkClr = COLOR_BLUE;
					Item.crFgClr = COLOR_WHITE;
				}
				else if (col == MAEDO_COL)
				{
					Item.crBkClr = COLOR_STATIC_MAEDO_BG;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAESU_COL)
				{
					Item.crBkClr = COLOR_STATIC_MAESU_BG;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAEDO_RESERVE_COL)
				{
					Item.crBkClr = COLOR_MAEDO_RESERVE_COL;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAESU_RESERVE_COL)
				{
					Item.crBkClr = COLOR_MAESU_RESERVE_COL;
					Item.crFgClr = COLOR_BLACK;
				}
				else
				{
					Item.crBkClr = COLOR_WHITE;
					Item.crFgClr = COLOR_BLACK;
				}
			}
			else if(row >= m_n5HogaSumRow )
			{
				Item.crBkClr = COLOR_TOTSUM_ROW;
				Item.crFgClr = COLOR_BLACK;
			}
			else
			{
				if (col == MAEDO_COL)
				{
					Item.crBkClr = COLOR_STATIC_MAEDO_BG;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAESU_COL)
				{
					Item.crBkClr = COLOR_STATIC_MAESU_BG;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAEDO_RESERVE_COL)
				{
					Item.crBkClr = COLOR_MAEDO_RESERVE_COL;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAESU_RESERVE_COL)
				{
					Item.crBkClr = COLOR_MAESU_RESERVE_COL;
					Item.crFgClr = COLOR_BLACK;
				}
				else
				{
					//{{ 2007.06.26 by LYH �����ǰ� ����
					if(col == MAESU_JAN_COL && (row == PRICE_ROW-5 || row == PRICE_ROW-2))//����ü�ᰡ, ü�ᷮ Ÿ��Ʋ
						Item.crBkClr = COLOR_GRIDHEADER;
					else
						Item.crBkClr = COLOR_WHITE;
					//}}
					Item.crFgClr = COLOR_BLACK;
				}
			}
			
			// Cancel Column Color
			if ( row > TENHOGA_ROW_FIRST && (col == LEFT_CAN_COL || col == RIGHT_CAN_COL) )
			{
				Item.crBkClr = COLOR_CANCEL_COL;
				Item.crFgClr = COLOR_BLACK;
			}


			// SetItem
			m_pGridCtrl->SetItem(&Item);
		}
	}

	// �ϴ�����(5ȣ���ܷ�, ���ܷ�, �ð����ܷ�)
	m_pGridCtrl->SetItemText(m_n5HogaSumRow, HOGA_COL, "5ȣ���ܷ�");
	m_pGridCtrl->RedrawCell(m_n5HogaSumRow, HOGA_COL);
	m_pGridCtrl->SetItemText(m_nTotalSumRow, HOGA_COL, "���ܷ�");
	m_pGridCtrl->RedrawCell(m_nTotalSumRow, HOGA_COL);
	m_pGridCtrl->SetItemText(m_nExtraVolRow, HOGA_COL, "�ð���");
	m_pGridCtrl->RedrawCell(m_nExtraVolRow, HOGA_COL);
}


void CFloatingHogaDlg::SendTrHoga()
{
	HogaMemoryFree();
}

void CFloatingHogaDlg::RecvHogaData(int nLength , char* pData)
{
	int nLen = nLength;
	
	DisplayClear_AllHogaData();					// ȣ�������͸� �׸��忡�� ����

	if (m_bIsTenHoga == TRUE)
		SetHogaData(pData, nLen);				// ȣ�������͸� m_HogaData�� ����		
}	

void CFloatingHogaDlg::DisplayClear_AllHogaData()
{
	int i;
	int nState;

	//for( i=MARKETPRICE_ROW; i <= m_nLowLimitRow; i++ )
	for( i=HIGH_ROW; i <= m_nLowLimitRow; i++ )
	{	
		m_pGridCtrl->SetItemText(i, HOGA_COL, "");
		m_pGridCtrl->SetItemText(i, MAEDO_JAN_COL, "");
		m_pGridCtrl->SetItemText(i, MAESU_JAN_COL, "");

		nState = m_pGridCtrl->GetItemState(i, HOGA_COL);
		nState &= ~GVIS_STATIC;
//		nState &= ~GVIS_CHARTPRICE;
		m_pGridCtrl->SetItemState(i, HOGA_COL, nState);

		m_pGridCtrl->RedrawCell(i, HOGA_COL);
		m_pGridCtrl->RedrawCell(i, MAEDO_JAN_COL);
		m_pGridCtrl->RedrawCell(i, MAESU_JAN_COL);
	}
	for( i=m_nLowLimitRow; i <= m_nLowLimitRow+3; i++ )
	{
		m_pGridCtrl->SetItemText(i, MAEDO_JAN_COL, "");
		m_pGridCtrl->SetItemText(i, MAESU_JAN_COL, "");
		m_pGridCtrl->RedrawCell(i, MAEDO_JAN_COL);
		m_pGridCtrl->RedrawCell(i, MAESU_JAN_COL);
	}
}

// JSJ_ITCastle_Modify_040429
void CFloatingHogaDlg::SetHogaData(char *pData, int nSize, BOOL bGetBasicData/*=FALSE*/)
{
	if(nSize <1 )
	{
		DisplayClear_AllHogaData();
		return;
	}
	HCQ01112_OUT	hcq01112;
	memcpy(&hcq01112, pData, HCQ01112_OUT_SZ);

	CString		sTemp = "", sTemp1 = "", sTemp2 = "", strText = "";
	int			nTemp1, n5HogaMaesuJan, n5HogaMaedoJan;
	int			nRow;

	//-----------------------------------------------------------------------------
	// Author		: Dae-Sung, Byun	Date :2005/5/12
	// Reason		: InfoStatic �߰� 
	//			-. ��������(��,��,��,�ŷ��� ��)
	//-----------------------------------------------------------------------------
	// start
	CString strExpRate = "", strSHigh = "", strSLow = "", strSPreVol = "", strSVolPer = "";	
	double	fPreVol=0., fCurVol=0., fPercent=0.;
	// end


	nTemp1 = n5HogaMaesuJan = n5HogaMaedoJan = 0;

	m_strOldCode = m_strCode;	//�������� ����.

	//���� ���� ����
	m_strCode = CString(hcq01112.jmcode, sizeof(hcq01112.jmcode));
	m_strCode.TrimRight();

// --> [Edit]  ������ 2008/10/23		( ǰ���� ������ ���´�. )
	m_nLog			= 1;
	m_nLogDispSize	= 0;
	m_nPrecision	= 0;
	m_nTickSize		= 1;
	m_nValidPoint	= 0;
	m_nMask			= 1;

	GetCodeConvInfo( m_strCode, m_nLog, m_nLogDispSize, m_nPrecision, m_nValidPoint, m_nMask, m_nTickSize);
// <-- [Edit]  ������ 2008/10/23

	// ���ذ� ����
	sTemp1 = CString(hcq01112.jsprice, sizeof(hcq01112.jsprice));
	m_strPreClosePrice = m_Util.GetComma(sTemp1);
//	m_strPreClosePrice = sTemp1;

	// ���簡 ����
	sTemp1 = CString(hcq01112.price, sizeof(hcq01112.price));
	m_strCurPrice = m_Util.GetComma(sTemp1);

	//////////////////////////////////////////////////////////////////////////////////
	// �ŵ� ȣ�� ���ؿ�
	//////////////////////////////////////////////////////////////////////////////////
	m_nHogaRow = MAEDO_ROW;
	m_nHogaRangeForColor = 0;


	// �ŵ�, �ż� ȣ�������� �ʱ�ȭ
	memset(&m_10HogaData, 0x00, TEN_HOGADATA_SIZE);	

	m_nHogaRow += 5;	
	// �ŵ� ȣ��/�ܷ� 5��
	sTemp = CString(hcq01112.mdhoga5, sizeof(hcq01112.mdhoga5));
	m_10HogaData.nOfferho5 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mdhorem5, sizeof(hcq01112.mdhorem5));
	n5HogaMaedoJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, TRUE);

	// �ŵ� ȣ��/�ܷ� 4��
	sTemp = CString(hcq01112.mdhoga4, sizeof(hcq01112.mdhoga4));
	m_10HogaData.nOfferho4 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mdhorem4, sizeof(hcq01112.mdhorem4));
	n5HogaMaedoJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, TRUE);

	// �ŵ� ȣ��/�ܷ� 3��
	sTemp = CString(hcq01112.mdhoga3, sizeof(hcq01112.mdhoga3));
	m_10HogaData.nOfferho3 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mdhorem3, sizeof(hcq01112.mdhorem3));
	n5HogaMaedoJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, TRUE);

	// �ŵ� ȣ��/�ܷ� 2��
	sTemp = CString(hcq01112.mdhoga2, sizeof(hcq01112.mdhoga2));
	m_10HogaData.nOfferho2 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mdhorem2, sizeof(hcq01112.mdhorem2));
	n5HogaMaedoJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, TRUE);

	// �ŵ� ȣ��/�ܷ� 1��
	sTemp = CString(hcq01112.mdhoga1, sizeof(hcq01112.mdhoga1));
	m_10HogaData.nOfferho1 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mdhorem1, sizeof(hcq01112.mdhorem1));
	n5HogaMaedoJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, TRUE);

	//////////////////////////////////////////////////////////////////////////////////
	// �ż� ȣ�� ���ؿ�
	//////////////////////////////////////////////////////////////////////////////////   
	m_nHogaRow = MAESU_ROW;
	
	// �ż� ȣ��/�ܷ� 1��
	sTemp = CString(hcq01112.mshoga1, sizeof(hcq01112.mshoga1));
	m_10HogaData.nBidho1 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mshorem1, sizeof(hcq01112.mshorem1));
	n5HogaMaesuJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, FALSE);

	// �ż� ȣ��/�ܷ� 2��
	sTemp = CString(hcq01112.mshoga2, sizeof(hcq01112.mshoga2));
	m_10HogaData.nBidho2 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mshorem2, sizeof(hcq01112.mshorem2));
	n5HogaMaesuJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, FALSE);

	// �ż� ȣ��/�ܷ� 3��
	sTemp = CString(hcq01112.mshoga3, sizeof(hcq01112.mshoga3));
	m_10HogaData.nBidho3 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mshorem3, sizeof(hcq01112.mshorem3));
	n5HogaMaesuJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, FALSE);

	// �ż� ȣ��/�ܷ� 4��
	sTemp = CString(hcq01112.mshoga4, sizeof(hcq01112.mshoga4));
	m_10HogaData.nBidho4 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mshorem4, sizeof(hcq01112.mshorem4));
	n5HogaMaesuJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, FALSE);

	// �ż� ȣ��/�ܷ� 5��
	sTemp = CString(hcq01112.mshoga5, sizeof(hcq01112.mshoga5));
	m_10HogaData.nBidho5 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mshorem5, sizeof(hcq01112.mshorem5));
	n5HogaMaesuJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, FALSE);

	//---------------------------------------------------------------------------------------------->>
	// 5ȣ�� �ŵ� �� �ܷ�
	nRow = m_n5HogaSumRow;		
	sTemp1.Format("%d", n5HogaMaedoJan);
	strText = m_Util.GetComma(sTemp1);
	CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);
	
	// 5ȣ�� �ż� �� �ܷ�
	nRow = m_n5HogaSumRow;		
	sTemp1.Format("%d", n5HogaMaesuJan);
	strText = m_Util.GetComma(sTemp1);
	CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);
	
	// �ŵ� �� �ܷ�
	nRow = m_nTotalSumRow;		
	sTemp = CString(hcq01112.totmdhorem, sizeof(hcq01112.totmdhorem));
	strText = m_Util.GetComma(sTemp);
	CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);
	
	// �ż� �� �ܷ�
	nRow = m_nTotalSumRow;		
	sTemp = CString(hcq01112.totmshorem, sizeof(hcq01112.totmshorem));
	strText = m_Util.GetComma(sTemp);
	CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);

	// ��
	sTemp = CString(hcq01112.high, sizeof(hcq01112.high));
	strSHigh = m_Util.GetComma(sTemp);
	
	// ����
	sTemp = CString(hcq01112.low, sizeof(hcq01112.low));
	strSLow = m_Util.GetComma(sTemp);	

	// ***** 10ȣ�� �׸��� ������ *****
	int nHogaData[TENHOGA_SAVED_CNT] = {0,};
	if (m_bIsTenHoga == TRUE)
	{
		m_10HogaData.nHighLimit = m_nHighLimit;
		
		sTemp1 = m_pGridCtrl->GetItemText(ETC_HOGAUP_ROW, HOGA_COL);	sTemp1.Remove(',');
		m_10HogaData.nExtraUp = atoi(sTemp1);
		
		sTemp1 = m_pGridCtrl->GetItemText(m_nLowLimitRow-1, HOGA_COL);	sTemp1.Remove(',');
		m_10HogaData.nExtraDown = atoi(sTemp1);
		
		m_10HogaData.nLowLimit = m_nLowLimit;

		memcpy(nHogaData, &m_10HogaData, TEN_HOGADATA_SIZE);
	}
	//*********************************


	// ********************************************************************
	//  ���簡 ���� ����ƽ 
	// *********************
	CString strSign = "", strChange = "", strChangeRate = "";
	CString strSVol = "", strSPrice = "";
//	COLORREF clrChange;
	
	// �����ȣ
	strSign = CString(hcq01112.sign, sizeof(hcq01112.sign));
//	sTemp1.Format("%1.1s", sTemp);		sTemp1.TrimLeft();		sTemp1.TrimRight();
//	strSign = hcq01112.m_szsign;
	int nSign = 5; //atoi(hcq01112.m_szsign);
	if ( strSign == "+" )
		nSign = 6;
	else if ( strSign == "-" )
		nSign = 4;
	else
		nSign = 5;
	
	// �����
	sTemp1 = CString(hcq01112.change, sizeof(hcq01112.change));
	sTemp1.Remove('-');
	strChange = m_Util.GetComma(sTemp1);

	sTemp1 = m_Util.GetSignSimbol(nSign);
	strChange = sTemp1 + " " + strChange;
	
	// �����		
	sTemp1 = CString(hcq01112.drate, sizeof(hcq01112.drate));
	sTemp1.Remove('-');
//	sTemp1.Format("%.2f", atof(sTemp1));
	strChangeRate = sTemp1 + "%";

	// �����ŷ���
	sTemp1 = CString(hcq01112.volume, sizeof(hcq01112.volume));
	fCurVol = atoi(sTemp1);
	strSVol = m_Util.GetVolUnit(sTemp1);

	CString strPrice = m_Util.GetNumber(m_strCurPrice);
// <-- [Edit]  ������ 2008/10/23
	if (m_bIsTenHoga == TRUE)
	{
		// ���簡��(���Ѱ��� ���Ѱ� ���� üũ ü��) ----------------->>>
		BOOL bFindRow = FALSE;
		int nPriceTemp = atoi(strPrice);

		for(int nCnt = 1; nCnt < TENHOGA_SAVED_CNT-1; nCnt++)
		{
			if (nPriceTemp ==  nHogaData[nCnt])
			{
				bFindRow = TRUE;
				m_nCurPriceRow = nCnt+HIGH_ROW;
				break;
			}
		}
		if (bFindRow == FALSE)
		{
			m_nCurPriceRow = -1;
		}
	}
	// *******************************************************************
}

void CFloatingHogaDlg::SetHogaData_Sub(CString strHoga, CString strNumber, CString strPreClosePrice, BOOL bMaeDo)
{
	{
		m_nHogaRangeForColor++;

		// ��� ����
		COLORREF clrHoga;
		clrHoga = GetHogaGridBkColor(m_nHogaRangeForColor);

		if (m_nHogaRangeForColor >= 1 && m_nHogaRangeForColor <= 5)
		{
			m_pGridCtrl->SetItemBkColour(m_nHogaRow, HOGA_COL, clrHoga);
			m_pGridCtrl->RedrawCell(m_nHogaRow, HOGA_COL);
			m_pGridCtrl->SetItemBkColour(m_nHogaRow, DAEBI_COL, clrHoga);
			m_pGridCtrl->RedrawCell(m_nHogaRow, DAEBI_COL);
			m_pGridCtrl->SetItemBkColour(m_nHogaRow, MAEDO_JAN_COL, clrHoga);
			m_pGridCtrl->RedrawCell(m_nHogaRow, MAEDO_JAN_COL);
		}
		else
		{
			m_pGridCtrl->SetItemBkColour(m_nHogaRow, HOGA_COL, clrHoga);
			m_pGridCtrl->RedrawCell(m_nHogaRow, HOGA_COL);
			m_pGridCtrl->SetItemBkColour(m_nHogaRow, DAEBI_COL, clrHoga);
			m_pGridCtrl->RedrawCell(m_nHogaRow, DAEBI_COL);
			m_pGridCtrl->SetItemBkColour(m_nHogaRow, MAESU_JAN_COL, clrHoga);
			m_pGridCtrl->RedrawCell(m_nHogaRow, MAESU_JAN_COL);
		}
	}
	// <<-----------------------------------------------------------------------------

	CString  sTemp1;
	CHogaData *pHogaData;

	// ȣ�� �ؽ�Ʈ
	pHogaData = new CHogaData;
	pHogaData->m_nRow	 = m_nHogaRow;		
	pHogaData->m_nCol	 = HOGA_COL;

	if ( m_nLog == 8 || m_nLog == 32 )
	{
		pHogaData->m_strText = Gl_Format32String(atol(strHoga), m_nLog, m_nLogDispSize, m_nPrecision, m_nMask);
	}
	else
	{
		strHoga.Format("%0.*f", m_nPrecision, atof(strHoga) / pow(10, m_nPrecision));
		pHogaData->m_strText = m_Util.GetComma(strHoga);
	}


	// ȣ�� ����
	if (m_nHogaRow == HIGH_ROW || m_nHogaRow == m_nLowLimitRow)
	{
		pHogaData->m_crFgClr = COLOR_WHITE;
	}
	else 
	{
		int lTemp1, lTemp2;
		lTemp1 = atol(strHoga);
		CString sTemp;
		sTemp = m_Util.GetNumber(strPreClosePrice);
		lTemp2 = atol(sTemp);
		
		if( lTemp1 > lTemp2 )			
			pHogaData->m_crFgClr = COLOR_RED;		// ����
		else if( lTemp1 < lTemp2 )		
			pHogaData->m_crFgClr = COLOR_BLUE;		// �Ķ�
		else
			pHogaData->m_crFgClr = COLOR_BLACK;
	}

	pHogaData->m_nMask = GVIF_FGCLR;	
	m_HogaData.Add(pHogaData);				// ȣ�� �߰�	
	CompareNUpdateHogaData(pHogaData->m_nRow, HOGA_COL, pHogaData->m_strText, GVIF_FGCLR, pHogaData->m_crFgClr);
	
	// �ܷ�
	CString strText;
	int nCol = 0;
	nCol = (bMaeDo == TRUE) ? MAEDO_JAN_COL : MAESU_JAN_COL;
	sTemp1 = m_Util.GetComma(strNumber);
	strText = sTemp1;
	CompareNUpdateHogaData(m_nHogaRow, nCol, strText, GVIF_FGCLR, COLOR_BLACK);

	m_nHogaRow++;
}

void CFloatingHogaDlg::CompareNUpdateHogaData(int nRow, int nCol, CString strText, int nMask, COLORREF crFgClr)
{
	CGridCellBase	*pCell = NULL;
	CString			strCellText;
	COLORREF		crFgColorTemp;
	
	// ���� ���� ������ �����´�.
	pCell = m_pGridCtrl->GetCell(nRow, nCol);
	if( pCell == NULL )
		return;

	// ���� ���� ���� ���� ���ؼ� �����Ͱ� ������ �ǵ��ư���.
	strCellText		= pCell->GetText();
	crFgColorTemp	= pCell->GetTextClr();
	if( strCellText == strText && crFgColorTemp == crFgClr)	// �� ����� ������ �����ϸ� ���� JSJ_Modify
		return;
	
	if( strText == "0" )
	{
		strText = "";
	}
	
	// �ؽ�Ʈ ����
	m_pGridCtrl->SetItemText(nRow, nCol, strText);

	// ���� ����ŷ �Ǿ� ������ ���� ����
	if( nMask & GVIF_FGCLR )
		m_pGridCtrl->SetItemFgColour(nRow, nCol, crFgClr);

	m_pGridCtrl->RedrawCell(nRow, nCol);		
}

void CFloatingHogaDlg::HogaMemoryFree()
{
	int i=0; 
	int nCount = m_HogaData.GetSize();
	CObject *pObj=NULL;
	for(i=0; i<nCount; i++)
	{
		pObj = (CObject *) m_HogaData.GetAt(i);
		delete pObj;
	}

	m_HogaData.RemoveAll();
}

COLORREF CFloatingHogaDlg::GetHogaGridBkColor(int nHogaRange)
{
	COLORREF clrHoga;
	if ( nHogaRange == 1 )
		clrHoga = COLOR_MAEDO_10HOGA_6;
	else if ( nHogaRange == 2 )
		clrHoga = COLOR_MAEDO_10HOGA_7;
	else if ( nHogaRange == 3 )
		clrHoga = COLOR_MAEDO_10HOGA_8;
	else if ( nHogaRange == 4 )
		clrHoga = COLOR_MAEDO_10HOGA_9;
	else if ( nHogaRange == 5 )
		clrHoga = COLOR_MAEDO_10HOGA_10;
	else if ( nHogaRange == 6 )
		clrHoga = COLOR_MAESU_10HOGA_1;
	else if ( nHogaRange == 7)
		clrHoga = COLOR_MAESU_10HOGA_2;
	else if ( nHogaRange == 8 )
		clrHoga = COLOR_MAESU_10HOGA_3;
	else if ( nHogaRange == 9 )
		clrHoga = COLOR_MAESU_10HOGA_4;
	else if ( nHogaRange == 10 )
		clrHoga = COLOR_MAESU_10HOGA_5;

	return clrHoga;
}

void CFloatingHogaDlg::SetRealHogaData_Sub(CString strHoga, /*CString strNumber, CString strChangeVol,*/ CString strPreClosePrice, BOOL bMaeDo)
{	
	CString		sTemp1, strText;
	COLORREF	crFgClr;
	int			nMask;
	int			nCol;

	// �ܷ�
	//nCol = (bMaeDo == TRUE) ? MAEDO_JAN_COL : MAESU_JAN_COL;
	//sTemp1 = m_Util.GetComma(strNumber);
	//CompareNUpdateHogaData(m_nHogaRow, nCol, sTemp1, GVIF_FGCLR, COLOR_BLACK);
	
	// ȣ�� �ؽ�Ʈ
	nCol	= HOGA_COL;

	if ( m_nLog == 8 || m_nLog == 32 )
	{
		strText = Gl_Format32String(atol(strHoga), m_nLog, m_nLogDispSize, m_nPrecision, m_nMask);
	}
	else
	{
		strHoga.Format("%0.*f",m_nPrecision, atof(strHoga) / pow(10, m_nPrecision));
		strText = m_Util.GetComma(strHoga);
	}

	// ȣ�� ����
	if (m_nHogaRow == HIGH_ROW || m_nHogaRow == m_nLowLimitRow)
	{
		crFgClr = COLOR_WHITE;
	}
	else
	{
		int lTemp1, lTemp2;
		lTemp1 = atol(strHoga);
		CString sTemp;
		sTemp = m_Util.GetNumber(strPreClosePrice);
		lTemp2 = atol(sTemp);
		
		if( lTemp1 > lTemp2 )			
			crFgClr = COLOR_RED;		// ����
		else if( lTemp1 < lTemp2 )		
			crFgClr = COLOR_BLUE;		// �Ķ�
		else
			crFgClr = COLOR_BLACK;
	}

	nMask = GVIF_FGCLR;	
	CompareNUpdateHogaData(m_nHogaRow, nCol, strText, nMask, crFgClr);
	
	// �����͸� ���� ���θ� üũ �� �Ŀ� ����Ǿ����� ȣ�� �����͸� ���ؼ� 
	// m_HogaDataList�� ȣ�� �����͸� �߰��Ѵ�.
	if( nCol == HOGA_COL && atol(strHoga) != 0 )
	{
		if( strText == "" )
		{
			m_nHogaRow++;
			return;
		}
		
		int nIndex = -1;
		int nRet = CheckRealHogaDataWithinHogaData(strText, &nIndex);

		if( nRet == BIG_HOGA || nRet == SMALL_HOGA )
		{
			// ȣ�� �ؽ�Ʈ
			CHogaData *pHogaData;
			pHogaData = new CHogaData;
			pHogaData->m_nRow = m_nHogaRow;		
			pHogaData->m_nCol = HOGA_COL;
			pHogaData->m_strText = strText;
			
			// ȣ�� ����
			pHogaData->m_crFgClr = crFgClr;
			pHogaData->m_nMask = GVIF_FGCLR;
			
			// ȣ�� �߰�
			if( nRet == BIG_HOGA )
				m_HogaData.InsertAt(0, pHogaData);
			else if( nRet == SMALL_HOGA )
				m_HogaData.Add(pHogaData);
		}	
		else if( nRet == WITHIN_HOGA && nIndex != -1 )
		{
			// ȣ�� �ؽ�Ʈ
			CHogaData *pHogaData;
			pHogaData = new CHogaData;
			pHogaData->m_nRow = m_nHogaRow;		
			pHogaData->m_nCol = HOGA_COL;
			pHogaData->m_strText = strText;
			
			// ȣ�� ����
			pHogaData->m_crFgClr = crFgClr;
			pHogaData->m_nMask = GVIF_FGCLR;
			
			// ȣ�� �߰�
			m_HogaData.InsertAt(nIndex, pHogaData);	
		}
	}
	
//	m_nHogaRow++;
}

int CFloatingHogaDlg::CheckRealHogaDataWithinHogaData(CString strData, int *nIndex)
{
	int nCount = m_HogaData.GetSize();
	int nHogaTop, nHogaBottom, nHogaPrice;

	CHogaData *pHogaDataTop = NULL;
	CHogaData *pHogaDataBottom = NULL;

	if( m_HogaData.GetSize() <= 0 )
		return NOT_FOUND;

	// ���� ����� ȣ���� ���Ѵ�.
	pHogaDataTop = m_HogaData.GetAt(0);
	if( pHogaDataTop == NULL )
		return NOT_FOUND;

	// ���� �ϴ��� ȣ���� ���Ѵ�
	pHogaDataBottom = m_HogaData.GetAt(nCount-1);
	if( pHogaDataBottom == NULL )
		return NOT_FOUND;
		
	nHogaTop = atoi(m_Util.GetNumber(pHogaDataTop->m_strText));
	if( nHogaTop == 0 )
		nHogaTop = atoi(m_Util.GetNumber(m_pGridCtrl->GetItemText(HIGH_ROW, HOGA_COL)));
	
	nHogaBottom = atoi(m_Util.GetNumber(pHogaDataBottom->m_strText));
	if( nHogaBottom == 0 )
		nHogaBottom = atoi(m_Util.GetNumber(m_pGridCtrl->GetItemText(m_nLowLimitRow, HOGA_COL)));

	nHogaPrice = atoi(m_Util.GetNumber(strData));

	// ȣ�� �����Ϳ� ��
	if( nHogaTop < nHogaPrice )				// �ŵ� 10�� ȣ���� ��
	{
		return BIG_HOGA;
	}
	else if( nHogaBottom > nHogaPrice )		// �ż� 10�� ȣ���� ��
	{
		return SMALL_HOGA;
	}
	
	int i;	
	CHogaData *pHogaData = NULL;
	BOOL bFind = FALSE;
	for(i=0; i<nCount; i++)
	{
		pHogaData = m_HogaData.GetAt(i);
		if( pHogaData == NULL )
			continue;

		if( strData.CompareNoCase(pHogaData->m_strText) == 0 )
		{
			pHogaData->m_nRow = m_nHogaRow;
			return EQUAL_HOGA;			
		}				
		else 
		{
			if( bFind == FALSE )
			{
				if( atol(m_Util.GetOnlyNumber(pHogaData->m_strText)) < atol(m_Util.GetOnlyNumber(strData)))
				{
					*nIndex = i;
					bFind = TRUE;
				}
			}
		}
	}

	return WITHIN_HOGA;
}



void CFloatingHogaDlg::OnDestroy() 
{
	MakeDrdsUnConnectUnAdvise();
	
	GetParent()->SendMessage(UMSG_DISCONNECT_HOGA_REAL, 0, 0);

	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	HogaMemoryFree();	
	if(m_pGridCtrl)
	{
		delete m_pGridCtrl;
		m_pGridCtrl = NULL;
	} 
	CRscDlg::OnDestroy();	
}

BOOL CFloatingHogaDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(pMsg->message) 
	{
		case WM_KEYDOWN	:
			{
				UINT nChar = (UINT)pMsg->wParam;
				switch(nChar)
				{
					case VK_RETURN :
					case VK_ESCAPE :
					{
						return TRUE;
						break;					
					}
				}
				break;
			}
	}
	return CRscDlg::PreTranslateMessage(pMsg);
}

BOOL CFloatingHogaDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{	
	static UINT msg = 512;
    if(msg == 513 && message == 512)
    {
        CPoint p;
        GetCursorPos(&p);
        ScreenToClient(&p);
        SendMessage(WM_LBUTTONUP, 0, MAKELPARAM(p.x, p.y));
    }
    msg = message;

	return CRscDlg::OnSetCursor(pWnd, nHitTest, message);
}

void CFloatingHogaDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	memDC.FillSolidRect(rcClient, GetSysColor(COLOR_WINDOW));
	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDC, 0,0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}

void CFloatingHogaDlg::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	CRect dlgRect(0,0,0,0);

	m_staticSymbol.GetWindowRect(dlgRect);//������ ��ǥ...
	m_bIconState = !m_bIconState;
		
	if(m_pMainDlg)
		m_pMainDlg->SendMessage(USMG_PLEASE_RESIZE, m_bIconState ? 1 : 0, (LPARAM)&dlgRect);

}

void CFloatingHogaDlg::SetRealHogaData_Rem(CString strNumber, BOOL bMaeDo)
{
	CString		sTemp1;
	int			nCol;

	// �ܷ�
	nCol = (bMaeDo == TRUE) ? MAEDO_JAN_COL : MAESU_JAN_COL;
	sTemp1 = m_Util.GetComma(strNumber);
	CompareNUpdateHogaData(m_nHogaRow, nCol, sTemp1, GVIF_FGCLR, COLOR_BLACK);
}

BOOL CFloatingHogaDlg::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

//-->
BOOL CFloatingHogaDlg::MakeDrdsConnectAdvise()
{
	//@SolomonGNF IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager, 3);
	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
	if(!pDrdsMng) return FALSE;

	m_pDrdsLib = pDrdsMng->AdviseDrdsLib();
	if(!m_pDrdsLib) return FALSE;

//@SolomonGNF	int nRealType = STOCK_HOGA1_PID;	// 1:ȣ��
//@SolomonGNF	m_pDrdsLib->DrdsAdvise(nRealType, "CAH1", "", "", m_hWnd);

	return TRUE;
}

BOOL CFloatingHogaDlg::MakeDrdsUnConnectUnAdvise()
{
	if(!m_pDrdsLib) return FALSE;

	//@SolomonGNF IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager, 3);
	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
	if(!pDrdsMng) return FALSE;

//@SolomonGNF	int nRealType = STOCK_HOGA1_PID;	// 1:ȣ��
//@SolomonGNF	m_pDrdsLib->DrdsUnadvise(nRealType, "CAH1", "", "", m_hWnd);
//@SolomonGNF	m_pDrdsLib->DrdsUnadviseAll(m_hWnd);

//@SolomonGNF	pDrdsMng->UnadviseDrdsLib(m_pDrdsLib);
	m_pDrdsLib = NULL;

	return TRUE;
}

void CFloatingHogaDlg::OnCodeAdvise(LPCSTR strCode)
{
	if(!m_pDrdsLib) return;

//@SolomonGNF	int nRealType = STOCK_HOGA1_PID;	// 1:ȣ��
//@SolomonGNF	m_pDrdsLib->DrdsAdvise(nRealType, "CAH1", "", (LPSTR)strCode, m_hWnd);
}

void CFloatingHogaDlg::OnCodeUnAdvise(LPCSTR strCode)
{
	if(!m_pDrdsLib) return;

//@SolomonGNF	int nRealType = STOCK_HOGA1_PID;	// 1:ȣ��
//@SolomonGNF	m_pDrdsLib->DrdsUnadvise(nRealType, "CAH1", "", (LPSTR)strCode, m_hWnd);
}

void CFloatingHogaDlg::SetRealHogaData(char* pstREAL_CAH1)
{
	if(!m_pGridCtrl)
		return;

	// ȣ���������� ���� �� ������ ���ش�------------------->
	int nCount = m_HogaData.GetSize();
	CHogaData *pHogaData = NULL;
	for(int i=0; i<nCount; i++)
	{
		pHogaData = m_HogaData.GetAt(i);
		if( pHogaData == NULL )
			continue;

		pHogaData->m_nRow = -1;
	}
	// <----------------------------------------------------

	CREAL_CAH1 euReal_CAH1;
	euReal_CAH1.Convert((stEUREAL_CAH1*)pstREAL_CAH1);

	CString sTemp, sTemp1, strText;
	int		nRow;
	int		n5HogaMaesuJan = 0, n5HogaMaedoJan = 0;

	// �����ڵ� üũ
	if( m_strCode != euReal_CAH1.m_szkjmcode )
		return;

	//////////////////////////////////////////////////////////////////////////
	// �ǽð� �ŵ� ȣ�� ���ؿ�
	////////////////////////////////////////////////////////////////////////// 
	m_nHogaRow = MAEDO_ROW;
	m_nHogaRangeForColor = 0;

	// �ŵ� ȣ��/�ܷ� 5��
	m_nHogaRow += 5;
	m_10HogaData.nOfferho5 = atoi(euReal_CAH1.m_szmdhoga5);
	SetRealHogaData_Sub(euReal_CAH1.m_szmdhoga5, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmdhorem5, TRUE);
	

	// �ŵ� ȣ��/�ܷ� 4��
	m_nHogaRow++;
	m_10HogaData.nOfferho4 = atoi(euReal_CAH1.m_szmdhoga4);
	SetRealHogaData_Sub(euReal_CAH1.m_szmdhoga4, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmdhorem4, TRUE);

	// �ŵ� ȣ��/�ܷ� 3��
	m_nHogaRow++;
	m_10HogaData.nOfferho3 = atoi(euReal_CAH1.m_szmdhoga3);
	SetRealHogaData_Sub(euReal_CAH1.m_szmdhoga3, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmdhorem3, TRUE);
	
	// �ŵ� ȣ��/�ܷ� 2��
	m_nHogaRow++;
	m_10HogaData.nOfferho2 = atoi(euReal_CAH1.m_szmdhoga2);
	SetRealHogaData_Sub(euReal_CAH1.m_szmdhoga2, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmdhorem2, TRUE);

	// �ŵ� ȣ��/�ܷ� 1��
	m_nHogaRow++;
	m_10HogaData.nOfferho1 = atoi(euReal_CAH1.m_szmdhoga1);
	SetRealHogaData_Sub(euReal_CAH1.m_szmdhoga1, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmdhorem1, TRUE);
	
	//////////////////////////////////////////////////////////////////////////////////
	// �ǽð� �ż� ȣ�� ���ؿ�
	//////////////////////////////////////////////////////////////////////////////////
	m_nHogaRow = MAESU_ROW;  
	
	// �ż� ȣ��/�ܷ� 1��
	m_10HogaData.nBidho1 = atoi(euReal_CAH1.m_szmshoga1);
	SetRealHogaData_Sub(euReal_CAH1.m_szmshoga1, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmshorem1, FALSE);
	
	// �ż� ȣ��/�ܷ� 2��
	m_nHogaRow++;
	m_10HogaData.nBidho2 = atoi(euReal_CAH1.m_szmshoga2);
	SetRealHogaData_Sub(euReal_CAH1.m_szmshoga2, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmshorem2, FALSE);

	// �ż� ȣ��/�ܷ� 3��
	m_nHogaRow++;
	m_10HogaData.nBidho3 = atoi(euReal_CAH1.m_szmshoga3);
	SetRealHogaData_Sub(euReal_CAH1.m_szmshoga3, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmshorem3, FALSE);

	// �ż� ȣ��/�ܷ� 4��
	m_nHogaRow++;
	m_10HogaData.nBidho4 = atoi(euReal_CAH1.m_szmshoga4);
	SetRealHogaData_Sub(euReal_CAH1.m_szmshoga4, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmshorem4, FALSE);

	// �ż� ȣ��/�ܷ� 5��
	m_nHogaRow++;
	m_10HogaData.nBidho5 = atoi(euReal_CAH1.m_szmshoga5);
	SetRealHogaData_Sub(euReal_CAH1.m_szmshoga5, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmshorem5, FALSE);

	// ���ܷ� ---------------------------------------------------------------------------->>
	// 5ȣ�� �ŵ� �� �ܷ�
	{
		for(int row=MAEDO_ROW+5; row < MAEDO_ROW+10; row++)
		{
			sTemp = m_pGridCtrl->GetItemText(row, MAEDO_JAN_COL);
			sTemp.Remove(',');
			n5HogaMaedoJan += atoi(sTemp);
		}
		sTemp1.Format("%d", n5HogaMaedoJan);
		strText = m_Util.GetComma(sTemp1);
		nRow = m_n5HogaSumRow;
		CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);
	}
	
	// 5ȣ�� �ż� �� �ܷ�
	{
		for(int row=MAESU_ROW; row < MAESU_ROW+5; row++)
		{
			sTemp = m_pGridCtrl->GetItemText(row, MAESU_JAN_COL);
			sTemp.Remove(',');
			n5HogaMaesuJan += atoi(sTemp);
		}
		sTemp1.Format("%d", n5HogaMaesuJan);
		strText = m_Util.GetComma(sTemp1);
		nRow = m_n5HogaSumRow;
		CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);
	}

	// �ŵ� �� �ܷ�
	nRow = m_nTotalSumRow;
	strText = m_Util.GetComma(euReal_CAH1.m_sztotmdhorem);
	CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);

	// �ż� �� �ܷ�
	nRow = m_nTotalSumRow;
	strText = m_Util.GetComma(euReal_CAH1.m_sztotmshorem);
	CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);

}

//*******************************************************************/
/*! Function Name : Format32String
/*! Function      : ���������� �̿��Ͽ� ������ ������ Return���ش�. �� �������� ����ŷ�� 
/*!					������������ ũ�Ⱑ Long���� �ѱ⶧���� ����ŷ���� ó���ϴ� ������ ���� �߰�����
/*! Param         : dllexport) CString Format32String(long nTBValue, int scale, int outdec, int indec, UINT uMask
/*! Return        : 
/*! Create        : ������ , 2006/11/16
/*! Comment       : 
//******************************************************************/
CString CFloatingHogaDlg::Gl_Format32String(long nTBValue, int scale, int outdec, int indec, UINT uMask) 
{

	CString strValue = "", strInt = "", strMid = "", strFloat = "", strResult = "";
	long	nValue(0);

	if ( scale < 0 || outdec < 0 || indec < 0 )
	{
		strValue.Format("%d", nTBValue);
		return strValue;
	}

	//	���� 0�ϰ�쿡�� Format�� ���� ������� �ʰ� �����Ѵ�.
	if ( nTBValue == 0 )
	{
		CString strZero = "00000000000000000000";
		if ( scale == 1 )
			return "0";
		else if ( scale == 8 || scale == 32 )
		{
			if ( indec == 0 )
				strValue.Format("0'%s", strZero.Left(outdec));
			else
				strValue.Format("0'%s.%s", strZero.Left(outdec - indec), strZero.Left(indec) );
		}
	}

	int nMask = uMask;
	if( nMask == 0 )
		nMask = 1;

	BOOL	bFlag = FALSE;
	if ( nTBValue < 0 )
	{
		nTBValue = abs(nTBValue);
		bFlag = TRUE;
	}

	double dValue = (double)nTBValue/(double)nMask;

	nValue = FtoTbondPrcL( dValue, scale, outdec );

	//	32�����ϰ�� ����ó��
	if ( scale == 32 || scale == 8 ) 
	{

//		int nMidSize = 2;
		strValue.Format("%d", nTBValue);

		if ( strValue.GetLength() < outdec )
		{
			int nCnt = strValue.GetLength();
			for ( int i = 0 ; i < outdec - nCnt; i++ )
				strValue = "0" + strValue;
		}
			
		//	�Ǽ��κ� ©�󳻱�	(�� : 102'31.5 => 5 )
		strFloat = strValue.Right(indec);
		//	�߰��κ� ©�󳻱�	(�� : 102'31.5 => 31 )
		strMid = strValue.Mid(strValue.GetLength() - outdec, outdec - indec);
		//	�����κ� ©�󳻱�	(�� : 102'31.5 => 102 )
		strInt = strValue.Left(strValue.GetLength() - outdec);

		//	�����κ��� ������� 0���� ��ü�Ѵ�.
		if ( strInt.IsEmpty() )
			strInt = "0";

		//	�Ǽ��κ��� ������� . ���ϴ� ǥ�� ���Ѵ�.
		if ( strFloat.IsEmpty() )
			strValue.Format("%s'%s", strInt, strMid);
		else
			strValue.Format("%s'%s.%s", strInt, strMid, strFloat);

//		return strValue;
	}
	else	//10
	{
		strValue.Format("%d", nTBValue);

		if ( strValue.GetLength() < indec/*outdec*/ )
		{
			int nCnt = strValue.GetLength();
			for ( int i = 0 ; i < indec/*outdec*/ - nCnt; i++ )
				strValue = "0" + strValue;
		}
			
		//	�Ǽ��κ� ©�󳻱�	(�� : 102'31.5 => 5 )
		strFloat = strValue.Right(indec);
		//	�����κ� ©�󳻱�	(�� : 102'31.5 => 102 )
		strInt = strValue.Left(strValue.GetLength() - indec/*outdec*/);

		//	�����κ��� ������� 0���� ��ü�Ѵ�.
		if ( strInt.IsEmpty() )
			strInt = "0";
		//	�Ǽ��κ��� ������� . ���ϴ� ǥ�� ���Ѵ�.
		if ( strFloat.IsEmpty() )
			strValue.Format("%s", strInt);
		else
			strValue.Format("%s.%s", strInt, strFloat/*strMid*/);

//		return strValue;
	}

//	strValue.Format("%d", nTBValue);
	if ( bFlag )
		strResult.Format("-%s", strValue);
	else
		strResult.Format("%s", strValue);

	return strResult;
}

CString CFloatingHogaDlg::GetCodeConvInfo( CString strCode, int &nLog, int &nLogDispSize, int &nPrecision, int &nValidPoint, int &nMask, int &nTickSize)
{
	CString			strTickSize = "1";
	CString			strTickValue, strPriceInfo;

	strCode.TrimLeft();		strCode.TrimRight();
	//@SolomonGNF IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager, 3);
	IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager);
//@SolomonGNF	if ( !pDataManager->GetGJongmokInfo( strCode, nLog, nLogDispSize, nPrecision, strTickSize, strTickValue, strPriceInfo ) )
	if(TRUE)
	{
		nLog			= 1;
		nLogDispSize	= 0;
		nPrecision	= 0;

		nValidPoint	= 0;
		nMask			= 0;

		nTickSize		= 1;
	}
	else
	{
		strTickSize.TrimLeft(); strTickSize.TrimRight();
		CString strTmp = strTickSize.Right(1);

		while( strTmp == "0" )
		{
			strTickSize = strTickSize.Left(strTickSize.GetLength()-1);
			strTmp = strTickSize.Right(1);
		};

		nMask = strTickSize.GetLength() - 2;

		if ( nMask < 0 )
			nMask = 0;

		nMask = pow(10, nMask);
		nValidPoint = nPrecision;

		nTickSize = (long)(atof(strTickSize)*nMask);

	}

	return strTickSize;
}

//*******************************************************************/
/*! Function Name : FtoTbondPrcL
/*! Function      : �̱�ä ǥ�ú�ȯ float decimal -> TB Price long
/*! Param         : double inprice, 
/*!                 int scale, 
/*!                 int outdec
/*! Return        : long  
/*! Create        : �Ȱ�ȯ , 2006/09/14
/*! Comment       : 
//******************************************************************/
long CFloatingHogaDlg::FtoTbondPrcL( double inprice, int scale, int outdec )
{
	int		i_intnum;
	char	s_float[10], s_temp[20];
	
	if( scale == 8 )
	{
		i_intnum = (int)floor( inprice );
		sprintf( s_float, "%3.1f", (inprice-i_intnum)*scale );
	}
	else if ( scale == 32 )
	{
		i_intnum = (int)floor( inprice );
		if ( outdec == 4 )
			sprintf( s_float, "%06.1f", (inprice-i_intnum)*scale*100.0 );
// --> [Edit]  ������ 2009/09/03
// �̱�ä 30�⹰ ƽ������ ���濡 ���� �߰�
		else if ( outdec == 2 )
			sprintf( s_float, "%04.1f", (inprice-i_intnum)*scale );
// <-- [Edit]  ������ 2009/09/03
		else
			sprintf( s_float, "%05.1f", (inprice-i_intnum)*scale*10.0 );
	}
	else
	{
		i_intnum = (int)floor( inprice );
		sprintf( s_float, "%05.1f", (inprice-i_intnum)*scale*10.0 );
	}

	
	sprintf( s_temp, "%d%.*s", i_intnum, outdec, s_float );
	return atol( s_temp );
}
