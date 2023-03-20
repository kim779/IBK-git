// FloatingHogaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FloatingHogaDlg.h"
//#include "../CommonTR/DrdsX11.h"
#include "../chart_common/Grid/MainDefine.h"
#include "../chart_common/Grid/ColorTable.h"
#include "../../inc/RealUpdate.h"
#include "../../inc/IMainInfoMng.h"
//#include "../../../../../include/CommDefine.h"

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
//	m_hBitmap = NULL;
	m_dlgOriRect = CRect(0,0,0,0);
	// end

	m_pDrdsLib = NULL;
}


void CFloatingHogaDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFloatingHogaDlg)
	DDX_Control(pDX, IDC_STATIC_FLOTING_GRID, m_staticGridFrame);
	DDX_Control(pDX, IDC_STATIC_SYMBOL, m_staticSymbol);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_staitcTilte);	
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_ctrlButtonClose);	
//@solomon	20091113	-->>
//	DDX_Control(pDX, IDC_BUTTON_SELL, m_ctrlButtonSell);	
//	DDX_Control(pDX, IDC_BUTTON_BUY, m_ctrlButtonBuy);	
//@solomon	20091113	<<--
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
//@solomon	20091113	-->>
//	ON_BN_CLICKED(IDC_BUTTON_SELL, OnButtonSell)
//	ON_BN_CLICKED(IDC_BUTTON_BUY, OnButtonBuy)
//@solomon	20091113	<<--
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()
//@Solomon090819 ON_MESSAGE( WM_BC_DATA_RECEIVED, OnGetBroad)

/////////////////////////////////////////////////////////////////////////////
// CFloatingHogaDlg message handlers



/*
void CFloatingHogaDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CString sTemp;
	m_btnToggle.GetWindowText(sTemp);	
	CRect rect;	
	m_btnToggle.GetWindowRect(rect);
	ScreenToClient(rect);
	MoveWindow(rect);
}*/

//void CFloatingHogaDlg::OnPictureSymbol() 
//{
//	CRect rect(0,0,0,0);
//	
//	if(m_bIconState)	
//		m_picSymbol.GetWindowRect(rect);
//	else
//		rect.EqualRect(&m_dlgOriRect);
//
//	ScreenToClient(rect);
//	MoveWindow(rect);
//
//
//	m_bIconState = !m_bIconState;
//}

void CFloatingHogaDlg::OnSize(UINT nType, int cx, int cy) 
{	
	CDialog::OnSize(nType, cx, cy);

	if(cy>100 && m_dlgOriRect.bottom>0)
	{
		if(m_pGridCtrl == NULL)
			InitGrid();
	}
	//if(m_hBitmap)
	//{
	//	DeleteObject(m_hBitmap);
	//	m_hBitmap = NULL;
	//}
}

#define WS_EX_LAYERED       0x00080000
#define LWA_ALPHA           0x00000002
BOOL CFloatingHogaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();


	//m_staticSymbol.SetBkColor(RGB(255,0,0));
	//m_staticSymbol.SetBkColor(RGB(90,167,115));
	m_staticSymbol.SetBkColor(RGB(185,205,230));
	m_staticSymbol.SetBkColor(RGB(92,135,183));
	//m_staticSymbol.SetBkColor(RGB(205,175,224));
	m_staticSymbol.SetTextColor(CLR_FONT_DEFAULT);
	m_staticSymbol.SetTextColor(RGB(255,255,255));

	m_staitcTilte.SetBkColor(RGB(255,255,255));
	m_staitcTilte.SetTextColor(RGB(61,57,57));
	//m_staitcTilte.SetTextColor(RGB(0,0,255));
	m_staitcTilte.SetText("5ȣ�� â");
	
	
	// TODO: Add extra initialization here
	if(GetSafeHwnd())
	{
		GetWindowRect(m_dlgOriRect);
	}
	
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
	//@solomon	20091113	-->>
	/*
	if(pGateMng != NULL)
	{
		IMainInfoManager* pManager = (IMainInfoManager*)pGateMng->GetInterface(nID);
		CString strImageDir;
		//solomon-090825 strImageDir.Format("%s\\Image\\Chart\\W38_h21.bmp", pManager->GetRootDir());
		strImageDir.Format("%s\\Image\\Chart\\W38_h21.bmp", pManager->GetDataDir());

		CBitmap bitmap;
		bitmap.Attach((HBITMAP)LoadImage(NULL, (LPSTR)(LPCTSTR)strImageDir, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
		m_ImageListOrder.Create(38, 21, ILC_MASK | ILC_COLORDDB, 1, 1);
		m_ImageListOrder.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();

		m_ctrlButtonSell.FP_vSetImage(&m_ImageListOrder, 0, 1);
		m_ctrlButtonBuy.FP_vSetImage(&m_ImageListOrder, 3, 4);
	}
	*/
	//@solomon	20091113	<<--

	//�̵�
	//InitGrid();
	
	 

//    // Ȯ�� ��Ÿ�Ͽ� WS_EX_LAYERED�� �߰��Ѵ�
//
//    ModifyStyleEx(0, WS_EX_LAYERED);
//
// 
//
//    // user32.dll�� �ִ� SetLayeredWindowAttributes()�� �����͸� ��´�
//
//    HMODULE hUserDll = ::LoadLibrary(_T("USER32.dll"));
//
//    typedef BOOL (WINAPI* LPSETTRANSPARENT)(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
//
// 
//
//    LPSETTRANSPARENT lpSetTransparent = NULL;
//
//    lpSetTransparent  = (LPSETTRANSPARENT)::GetProcAddress(hUserDll, "SetLayeredWindowAttributes");
//
// 
//
//    if(lpSetTransparent)
//
//        // SetLayeredWindowAttributes() �Լ��� ȣ���Ѵ�.
//
//        // ����° ������ 0 ~ 255������ ���� �����Ѵ�
//
//        lpSetTransparent(m_hWnd, NULL, 200, LWA_ALPHA);
//
// 
//
//    // user32.dll�� �ݴ´�
//
//    ::FreeLibrary(hUserDll);

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

				CString* strRecvCode = (CString*)wParam;
				
				stReceiveFromServer* pStR = (stReceiveFromServer*)lParam;				
				m_strOldCode = m_strCode;	//�������� ����.
				m_strCode = *strRecvCode;
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
		
	
		if(m_bIconState && m_strCode.GetLength()>0)
			OnCodeUnAdvise(m_strCode);	// Old Code����
	}
	
	CRscDlg::OnLButtonDblClk(nFlags, point);
}



void CFloatingHogaDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	m_bCapTure = TRUE;
//	SetCapture();
//	OutputDebugString("LB___dn");

	CRscDlg::OnLButtonDown(nFlags, point);
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
}

void CFloatingHogaDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
/*
	if(m_bCapTure)
	{
		ReleaseCapture();
		m_bCapTure = FALSE;
//		OutputDebugString("LB___up");
		RedrawWindow();
	}
*/


	CRscDlg::OnLButtonUp(nFlags, point);
}

void CFloatingHogaDlg::OnMouseMove(UINT nFlags, CPoint point) 
{	
/*
	if(m_bCapTure)
	{
//		OutputDebugString("Mouse___Capmove");
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	
		
		//if(m_pGridCtrl)
		//	m_pGridCtrl->Invalidate();
		//Invalidate();
	}
*/
	CRscDlg::OnMouseMove(nFlags, point);
}


void CFloatingHogaDlg::InitGrid()
{
	if (m_pGridCtrl == NULL)
	{
		//AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CClientDC dc(this);
		m_logGridFont.lfWidth = 0; 
		m_logGridFont.lfEscapement = 0; 
		m_logGridFont.lfOrientation = 0; 
		m_logGridFont.lfWeight = FW_NORMAL; 
		m_logGridFont.lfItalic = 0; 
		m_logGridFont.lfUnderline = 0; 
		m_logGridFont.lfStrikeOut = 0; 
		m_logGridFont.lfCharSet = DEFAULT_CHARSET; 
		m_logGridFont.lfOutPrecision = OUT_DEFAULT_PRECIS; 
		m_logGridFont.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
		m_logGridFont.lfQuality = DEFAULT_QUALITY;
		m_logGridFont.lfPitchAndFamily = FF_ROMAN; 
		strcpy(m_logGridFont.lfFaceName, "����ü");
		m_logGridFont.lfHeight = -12;

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
		m_pGridCtrl->SetHighlightColor(COLOR_SELECTED_CELL_HOGA);

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
	int col = 0;
	// column width "0" initialize (��ũ�� ����) ------------->>
	for ( col = 0; col < nCol; col++)
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
	{
		m_pGridCtrl->SetColumnWidth(col, nColWdth[col]);
//		m_pGridCtrl->SetColumnWidth_NoReset(col, nColWdth[col]);
	}


	// ȣ��, ��� ������� �̹���
	if( m_bOpenCol == TRUE)
	{
		m_pGridCtrl->SetItemImage(TENHOGA_ROW_FIRST, HOGA_COL, 0);
		m_pGridCtrl->SetItemImage(TENHOGA_ROW_FIRST, DAEBI_COL, 2);
	}
	else
		m_pGridCtrl->SetItemImage(TENHOGA_ROW_FIRST, HOGA_COL, 1);


	// ���׷��� Hide, Show ------------------------------------------->>>
//	if(m_bExpandRight)
//	{
//		m_pGridCtrl->SetBongGraphEnable(TRUE);
//		m_pGridCtrl->SetColumnWidth(BONG_COL, BONG_COL_WIDTH);
//	}
//	else
//	{
//		m_pGridCtrl->SetBongGraphEnable(FALSE);
//		m_pGridCtrl->SetColumnWidth(BONG_COL, 0);
//	}
	// <<<---------------------------------------------------------------

//	m_pGridCtrl->Call_ResetScrollBars();	

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
				if( col == HOGA_COL )					
				{
					Item.nFormat |= DT_CENTER;

					// ��Ÿȣ���� ������ ���� �����ϰ�(10�ܰ�ȣ��)___JSJ_ADD_050126
//					if( row == ETC_HOGAUP_ROW || row == ETC_HOGADOWN_ROW)
//						Item.nState |= GVIS_CELLEDITABLE;
				}
				else if (col == DAEBI_COL)
				{
					Item.nFormat |= DT_RIGHT;
				}
				else if( col == MAEDO_JAN_COL || col == MAESU_JAN_COL )
				{
					Item.nFormat |= DT_RIGHT;
				}
				else if( col == LEFT_CAN_COL || col == RIGHT_CAN_COL )	
				{
//					if( m_pGridCtrl->InsideStaticLeft(row, col) )
//					{
//						Item.nState = GVIS_MODIFIED ;
//					}
//					else if( m_pGridCtrl->InsideStaticRight(row, col) )
//					{
//						Item.nState = GVIS_MODIFIED ;
//					}
//					
//					Item.nFormat |= DT_CENTER;
				}
				else
				{
					if( col == MAEDO_COL || col == MAESU_COL || col == MAEDO_RESERVE_COL || col == MAESU_RESERVE_COL )
						Item.nState = GVIS_MODIFIED ;
					
					Item.nFormat |= DT_CENTER;
				}

				// Bold Font ------------------------------------>>
//				if ( row <= m_nLowLimitRow && col == HOGA_COL)
//					Item.nState |= GVIS_BOLDFONT;
				// <<----------------------------------------------
			}

			
			// Color
			if(row == TENHOGA_ROW_FIRST)
			{
				Item.crBkClr = COLOR_GRIDHEADER;
				if (col == BONG_COL)
					Item.crBkClr = COLOR_WHITE;

				Item.crFgClr = COLOR_BLACK;
			}
//			else if(row == MARKETPRICE_ROW)
//			{
//				if (col > MAEDO_COL && col < MAESU_COL)
//				{
//					Item.crBkClr = COLOR_MARKET_PRICE;
//					Item.crFgClr = COLOR_BLACK;
//				}
//				else if (col == MAEDO_COL)
//				{
//					Item.crBkClr = COLOR_STATIC_MAEDO_BG;
//					Item.crFgClr = COLOR_BLACK;
//				}
//				else if (col == MAESU_COL)
//				{
//					Item.crBkClr = COLOR_STATIC_MAESU_BG;
//					Item.crFgClr = COLOR_BLACK;
//				}
//				else if (col == MAEDO_RESERVE_COL)
//				{
//					Item.crBkClr = COLOR_MAEDO_RESERVE_COL;
//					Item.crFgClr = COLOR_BLACK;
//				}
//				else if (col == MAESU_RESERVE_COL)
//				{
//					Item.crBkClr = COLOR_MAESU_RESERVE_COL;
//					Item.crFgClr = COLOR_BLACK;
//				}
//				else
//				{
//					Item.crBkClr = COLOR_WHITE;
//					Item.crFgClr = COLOR_BLACK;
//				}
//			}
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
	//m_TRComm.SetTR("HWS10210");		// JSJ_Modify_040426
	
	//int nRet = m_TRComm.Send2Server(m_strCode, CODE_STOCK_LENGTH, FALSE);
}

void CFloatingHogaDlg::RecvHogaData(int nLength , char* pData)
{
	int nLen = nLength;
	
	DisplayClear_AllHogaData();					// ȣ�������͸� �׸��忡�� ����

	if (m_bIsTenHoga == TRUE)
	{
		SetHogaData(pData, nLen);				// ȣ�������͸� m_HogaData�� ����		
		
		//sy, nam
		// ���׸����� ��쿡�� ȣ������� ���
		//if (m_nThreeStateDaebiType != e_PROFIT_PER)
		//	SetDaebiOrProfit(FALSE);
	}
/*	else
	{
		if (m_bUseLimitHoga == TRUE)
		{
			SetHogaData(pData, nLen, TRUE);			// ���Ѱ� ���Ѱ��� �⺻������ ����
			MakeFullHogaGrid();
		}
		m_bUseLimitHoga = TRUE;

		SetFullHogaDataToGrid();					// ��ü ȣ���� �׸��忡 ����					
		SetHogaData(pData, nLen);				// ȣ�������͸� m_HogaData�� ����
		SetRsvdOdrInfoAtGrid_FullHoga();			// �׸��� ���� ���� ǥ��

		// ���׸����� ��쿡�� ȣ������� ���
		if (m_nThreeStateDaebiType != e_PROFIT_PER)
			SetDaebiOrProfit(FALSE);

		// ��Ʈ���� ���� ������ �ش�ȣ�� �� -------->
		SetChartPriceRow();		
		if(m_bMoveChartPriceRow)
		{
			ResortHogaRow(m_nChartPriceRow);	// ��Ʈ���� Scroll
			m_bMoveChartPriceRow = FALSE;
		}
		else
		{
			ResortHogaRow(m_nCurPriceRow);		// ���簡 Scroll
		}
		// <----------------------------------------
	}*/
}	

//void CFloatingHogaDlg::DisplayClear_AllHogaData()
//{
//	int i;
//	int nState;
//
//	//for( i=MARKETPRICE_ROW; i <= m_nLowLimitRow; i++ )
//	for( i=HIGH_ROW; i <= m_nLowLimitRow; i++ )
//	{	
//		m_pGridCtrl->SetItemText(i, HOGA_COL, "");
//
//		nState = m_pGridCtrl->GetItemState(i, HOGA_COL);
//		nState &= ~GVIS_STATIC;
////		nState &= ~GVIS_CHARTPRICE;
//		m_pGridCtrl->SetItemState(i, HOGA_COL, nState);
//
//		m_pGridCtrl->RedrawCell(i, HOGA_COL);
//	}
//}

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
	CKB_01230 kb_01230;
	kb_01230.Convert((LPk01230Out2)pData);

	CString		sTemp, sTemp1, sTemp2, strText;
	int			nTemp1, n5HogaMaesuJan, n5HogaMaedoJan;
	int			nRow;
	int			nMask;
	COLORREF	crFgClr;
	

	//-----------------------------------------------------------------------------
	// Author		: Dae-Sung, Byun	Date :2005/5/12
	// Reason		: InfoStatic �߰� 
	//			-. ��������(��,��,��,�ŷ��� ��)
	//-----------------------------------------------------------------------------
	// start
	CString strExpRate , strSHigh , strSLow , strSPreVol , strSVolPer;	
	double	fPreVol=0., fCurVol=0., fPercent=0.;
	// end


	nTemp1 = n5HogaMaesuJan = n5HogaMaedoJan = 0;

//	m_strOldCode = m_strCode;	//�������� ����.

	//���� ���� ����
//	m_strCode = m_pInputBarDlg->m_ctrlCodeInput.GetDataText();

	// ���ذ� ����
	sTemp1 = m_Util.GetComma(kb_01230.m_szrecprice);
	m_strPreClosePrice = sTemp1;

	// ���簡 ����
	sTemp1 = m_Util.GetComma(kb_01230.m_szprice);
	m_strCurPrice = sTemp1;
	
	// ���Ѱ�
	nRow = HIGH_ROW;	
	m_nHighLimit = atoi(kb_01230.m_szuplmtprice);
	strText = m_Util.GetComma(kb_01230.m_szuplmtprice);
	crFgClr = COLOR_WHITE;
	nMask = GVIF_FGCLR;	
	CompareNUpdateHogaData(nRow, HOGA_COL, strText, nMask, crFgClr);

	if (m_bIsTenHoga == TRUE )	strText = "����";
	else						strText = "";
	CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);

	// ���Ѱ�
	nRow	= m_nLowLimitRow;
	m_nLowLimit = atoi(kb_01230.m_szdnlmtprice);
	strText = m_Util.GetComma(kb_01230.m_szdnlmtprice);
	crFgClr = COLOR_WHITE;
	nMask = GVIF_FGCLR;	
	CompareNUpdateHogaData(nRow, HOGA_COL, strText, nMask, crFgClr);

	if (m_bIsTenHoga == TRUE )	strText = "����";
	else						strText = "";
	CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);

	// ���Ѱ�, ���Ѱ� �� �⺻������ ������ ����
	if ( bGetBasicData == TRUE)
		return;
	
	//////////////////////////////////////////////////////////////////////////////////
	// �ŵ� ȣ�� ���ؿ�
	//////////////////////////////////////////////////////////////////////////////////
	m_nHogaRow = MAEDO_ROW;
	m_nHogaRangeForColor = 0;


	// �ŵ�, �ż� ȣ�������� �ʱ�ȭ
	memset(&m_10HogaData, 0x00, TEN_HOGADATA_SIZE);	

	m_nHogaRow += 5;	
	// �ŵ� ȣ��/�ܷ� 5��
	m_10HogaData.nOfferho5 = atoi(kb_01230.m_szofferho5);
	n5HogaMaedoJan += atoi(kb_01230.m_szofferrem5);
	SetHogaData_Sub(kb_01230.m_szofferho5, kb_01230.m_szofferrem5, m_strPreClosePrice, TRUE);

	// �ŵ� ȣ��/�ܷ� 4��
	m_10HogaData.nOfferho4 = atoi(kb_01230.m_szofferho4);
	n5HogaMaedoJan += atoi(kb_01230.m_szofferrem4);
	SetHogaData_Sub(kb_01230.m_szofferho4, kb_01230.m_szofferrem4, m_strPreClosePrice, TRUE);

	// �ŵ� ȣ��/�ܷ� 3��
	m_10HogaData.nOfferho3 = atoi(kb_01230.m_szofferho3);
	n5HogaMaedoJan += atoi(kb_01230.m_szofferrem3);
	SetHogaData_Sub(kb_01230.m_szofferho3, kb_01230.m_szofferrem3, m_strPreClosePrice, TRUE);

	// �ŵ� ȣ��/�ܷ� 2��
	m_10HogaData.nOfferho2 = atoi(kb_01230.m_szofferho2);
	n5HogaMaedoJan += atoi(kb_01230.m_szofferrem2);
	SetHogaData_Sub(kb_01230.m_szofferho2, kb_01230.m_szofferrem2, m_strPreClosePrice, TRUE);

	// �ŵ� ȣ��/�ܷ� 1��
	m_10HogaData.nOfferho1 = atoi(kb_01230.m_szofferho1);
	n5HogaMaedoJan += atoi(kb_01230.m_szofferrem1);
	SetHogaData_Sub(kb_01230.m_szofferho1, kb_01230.m_szofferrem1, m_strPreClosePrice, TRUE);

	//////////////////////////////////////////////////////////////////////////////////
	// �ż� ȣ�� ���ؿ�
	//////////////////////////////////////////////////////////////////////////////////   
	m_nHogaRow = MAESU_ROW;
	
	// �ż� ȣ��/�ܷ� 1��
	m_10HogaData.nBidho1 = atoi(kb_01230.m_szbidho1);
	n5HogaMaesuJan += atoi(kb_01230.m_szbidrem1);
	SetHogaData_Sub(kb_01230.m_szbidho1, kb_01230.m_szbidrem1, m_strPreClosePrice, FALSE);

	// �ż� ȣ��/�ܷ� 2��
	m_10HogaData.nBidho2 = atoi(kb_01230.m_szbidho2);
	n5HogaMaesuJan += atoi(kb_01230.m_szbidrem2);
	SetHogaData_Sub(kb_01230.m_szbidho2, kb_01230.m_szbidrem2, m_strPreClosePrice, FALSE);

	// �ż� ȣ��/�ܷ� 3��
	m_10HogaData.nBidho3 = atoi(kb_01230.m_szbidho3);
	n5HogaMaesuJan += atoi(kb_01230.m_szbidrem3);
	SetHogaData_Sub(kb_01230.m_szbidho3, kb_01230.m_szbidrem3, m_strPreClosePrice, FALSE);

	// �ż� ȣ��/�ܷ� 4��
	m_10HogaData.nBidho4 = atoi(kb_01230.m_szbidho4);
	n5HogaMaesuJan += atoi(kb_01230.m_szbidrem4);
	SetHogaData_Sub(kb_01230.m_szbidho4, kb_01230.m_szbidrem4, m_strPreClosePrice, FALSE);

	// �ż� ȣ��/�ܷ� 5��
	m_10HogaData.nBidho5 = atoi(kb_01230.m_szbidho5);
	n5HogaMaesuJan += atoi(kb_01230.m_szbidrem5);
	SetHogaData_Sub(kb_01230.m_szbidho5, kb_01230.m_szbidrem5, m_strPreClosePrice, FALSE);

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
	strText = m_Util.GetComma(kb_01230.m_sztotofferrem);
	CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);
	
	// �ż� �� �ܷ�
	nRow = m_nTotalSumRow;		
	strText = m_Util.GetComma(kb_01230.m_sztotbidrem);
	CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);

	// �ð��� �ŵ��ܷ�
	nRow = m_nExtraVolRow;		
	strText = m_Util.GetComma(kb_01230.m_sztmofferrem);
	CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);
	
	// �ð��� �ż��ܷ�
	nRow = m_nExtraVolRow;		
	strText = m_Util.GetComma(kb_01230.m_sztmbidrem);
	CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);
	// <<---------------------------------------------------------------------------------------------
	
	// ����ü�ᰡ�������簡���
	strExpRate.Format("%s", kb_01230.m_szyerate);

	// ��
	strSHigh = m_Util.GetComma(kb_01230.m_szhigh);
	
	// ����
	strSLow = m_Util.GetComma(kb_01230.m_szlow);	

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
	CString strSign, strChange, strChangeRate;
	CString strSVol, strSPrice;
	COLORREF clrChange;
	
	// �����ȣ
	sTemp1.Format("%1.1s", kb_01230.m_szsign);		sTemp1.TrimLeft();		sTemp1.TrimRight();
	strSign = kb_01230.m_szsign;
	int nSign = atoi(kb_01230.m_szsign);
	
	// �����
	sTemp1 = kb_01230.m_szchange;
	sTemp1.Remove('-');
	strChange = m_Util.GetComma(sTemp1);
	
	sTemp1 = m_Util.GetSignSimbol(nSign);
	strChange = sTemp1 + " " + strChange;
	
	// �����
	sTemp1 = kb_01230.m_szrate;
	sTemp1.Remove('-');
	
	sTemp1.Format("%.2f", atof(sTemp1));
	strChangeRate = sTemp1 + "%";

	// �����ŷ���
	fCurVol = atoi(kb_01230.m_szvolume);
	strSVol = m_Util.GetVolUnit(kb_01230.m_szvolume);
	
	// �����ݾ�
	strSPrice =	m_Util.GetPriceUnit(kb_01230.m_sztrade_cost);

	// ���ϰŷ��� 
/*
	fPreVol = atoi(kb_01230.m_szjnilvolume);
	m_strPreVol = kb_01230.m_szjnilvolume;
	strSPreVol = m_Util.GetVolUnit(kb_01230.m_szjnilvolume);

	// �ŷ��� ��� %
	if (fCurVol == 0 || fPreVol == 0)
		strSVolPer = "";
	else
	{
		fPercent = fCurVol * 100. / fPreVol;
		sTemp1.Format("%.1f", fPercent);
		strSVolPer = m_Util.GetComma(sTemp1);
	}
*/

	// #### ����ü�� ####
	CString		strExpPrice, strExpVol;
	CString		strPrice, strPrePrice;
	long		lChange=0, lPrice=0;
	
	m_chSign = kb_01230.m_szsign[0];
//>> [QC No]090803HMC075
//	int nSign = atoi(kb_01230.m_szsign);
	int nYSign = atoi(kb_01230.m_szjnilysign);
//<< [QC No]090803HMC075
//	if(m_chSign == '9')
	{
		// ���� ü�ᰡ��
		strPrice = kb_01230.m_szyeprice;

		strExpPrice = /*"*" + */m_Util.GetComma(strPrice);
		// ���� ü�����
		strExpVol = m_Util.GetComma(kb_01230.m_szyevolume) + "��";

		long lPrice = atol(strPrice);
		long lExpRate = atol(strExpRate);
//		double dChangeRate = (double)((double)lExpRate/(double)(lPrice-lExpRate)) * 100.;
		//double dChangeRate = (double)((double)atol(kb_01230.m_szyerate));
		//strChangeRate.Format("%.2f", dChangeRate);
		strChangeRate = kb_01230.m_szyerate;
		strChangeRate += "%";

		if ( nYSign < 3 )
			clrChange = COLOR_RED;
		else if ( nYSign > 3 )
			clrChange = COLOR_BLUE;
		else
			clrChange = COLOR_BLACK;

		CompareNUpdateHogaData(9, MAESU_JAN_COL, "����ü�ᰡ", 0, COLOR_BLACK);
		CompareNUpdateHogaData(10, MAESU_JAN_COL, strExpPrice, GVIF_FGCLR, clrChange);
		CompareNUpdateHogaData(11, MAESU_JAN_COL, strChangeRate, GVIF_FGCLR, clrChange);
		CompareNUpdateHogaData(12, MAESU_JAN_COL, "����ü�ᷮ", 0, COLOR_BLACK);
		CompareNUpdateHogaData(13, MAESU_JAN_COL, strExpVol, 0, COLOR_BLACK);
	}
//	else
//	{
//		CompareNUpdateHogaData(9, MAESU_JAN_COL, "����ü�ᰡ", 0, COLOR_BLACK);
//		CompareNUpdateHogaData(10, MAESU_JAN_COL, "*0", GVIF_FGCLR, clrChange);
//		CompareNUpdateHogaData(11, MAESU_JAN_COL, "0.00%", GVIF_FGCLR, clrChange);
//		CompareNUpdateHogaData(12, MAESU_JAN_COL, "����ü�ᷮ", 0, COLOR_BLACK);
//		CompareNUpdateHogaData(13, MAESU_JAN_COL, "0��", 0, COLOR_BLACK);
//	}
	// **********************************************************************


	// ****************************************************************
	// ���簡�� ã��
	// ***********************************************
	// *** ����ü�ᰡ ***
	if (atol(strPrice) > 0)
	{
		// ����ü�ᰡ�� ��쿡�� ������ �׸���.
		// �ð�, ��, ����, ����(����ü��)
		//m_ChartData.nOpen = m_ChartData.nHigh = m_ChartData.nLow = m_ChartData.nClose = atoi(strPrice);
		//m_pGridCtrl->DrawCandleChart(&m_ChartData, &m_MapGridAllData, nHogaData);		// ĵ��
	}
	// *** ���簡 ***
	else
	{
		strPrice = m_Util.GetNumber(m_strCurPrice);
	}

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
	// -------------------------------------------------------------------------->>	
	// *** ��üȣ���� ��� �ش�ȣ���� Row ���� ***
/*	if (m_bIsTenHoga == FALSE)
	{
		m_nHogaRangeForColor++;
		m_nHogaRow = FindRowInFullHoga(strHoga);

		// ���Ѱ� �Ǵ� ���Ѱ��� �ƴѰ�쿡 ���� ����
		if ( !(m_nHogaRow == -1 || m_nHogaRow == HIGH_ROW || m_nHogaRow == m_nLowLimitRow) )
		{
			// ��� ����
			COLORREF clrHoga;
//			if ( m_nHogaRangeForColor >= 1 && m_nHogaRangeForColor <= 5 )
//				clrHoga = COLOR_MAEDO_10HOGA;
//			else if ( m_nHogaRangeForColor >= 6 && m_nHogaRangeForColor <= 10 )
//				clrHoga = COLOR_MAEDO_5HOGA;
//			else if ( m_nHogaRangeForColor >= 11 && m_nHogaRangeForColor <= 15 )
//				clrHoga = COLOR_MAESU_5HOGA;
//			else if ( m_nHogaRangeForColor >= 16 && m_nHogaRangeForColor <= 20 )
//				clrHoga = COLOR_MAESU_10HOGA;
			
			clrHoga = GetHogaGridBkColor(m_nHogaRangeForColor);


			//if (clrHoga == COLOR_MAEDO_10HOGA || clrHoga == COLOR_MAEDO_5HOGA)
			if (m_nHogaRangeForColor >= 1 && m_nHogaRangeForColor <= 10)
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
	}
	else*/
	{
		m_nHogaRangeForColor++;

		// ��� ����
		COLORREF clrHoga;
		clrHoga = GetHogaGridBkColor(m_nHogaRangeForColor);


		//if (clrHoga == COLOR_MAEDO_10HOGA || clrHoga == COLOR_MAEDO_5HOGA)
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
	pHogaData->m_strText = m_Util.GetComma(strHoga);


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


	//-----------------------------------------------------------------------------
	// Author		: Dae-Sung, Byun	Date :2005/5/12
	// Reason		: ü�� ����Ʈ �߰� �Կ� ���� �����ܷ� ǥ������ ����
	//			-. ���Ŀ� �߰� �Ҽ� ������ �߰��Ѵ� ( �÷��� ���� ������ )
	//-----------------------------------------------------------------------------
	// start
	// �����ܷ�
	//nCol = (bMaeDo == TRUE) ? MAESU_JAN_COL : MAEDO_JAN_COL;
	//strText = "";
	//CompareNUpdateHogaData(m_nHogaRow, nCol, strText, GVIF_FGCLR, COLOR_BLACK);
	// end

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
	//if ( nHogaRange == 1 )
	//	clrHoga = COLOR_MAEDO_10HOGA_1;
	//else if ( nHogaRange == 2 )
	//	clrHoga = COLOR_MAEDO_10HOGA_2;
	//else if ( nHogaRange == 3 )
	//	clrHoga = COLOR_MAEDO_10HOGA_3;
	//else if ( nHogaRange == 4 )
	//	clrHoga = COLOR_MAEDO_10HOGA_4;
	//else if ( nHogaRange == 5 )
	//	clrHoga = COLOR_MAEDO_10HOGA_5;
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
	//else if ( nHogaRange == 16 )
	//	clrHoga = COLOR_MAESU_10HOGA_6;
	//else if ( nHogaRange == 17 )
	//	clrHoga = COLOR_MAESU_10HOGA_7;
	//else if ( nHogaRange == 18 )
	//	clrHoga = COLOR_MAESU_10HOGA_8;
	//else if ( nHogaRange == 19 )
	//	clrHoga = COLOR_MAESU_10HOGA_9;
	//else if ( nHogaRange == 20 )
	//	clrHoga = COLOR_MAESU_10HOGA_10;


	return clrHoga;
}

//@��������
//void CFloatingHogaDlg::SetRealHogaData(long pRealData)
//{
//	if(!m_pGridCtrl)
//		return;
//
//	// ȣ���������� ���� �� ������ ���ش�------------------->
//	int nCount = m_HogaData.GetSize();
//	CHogaData *pHogaData = NULL;
//	for(int i=0; i<nCount; i++)
//	{
//		pHogaData = m_HogaData.GetAt(i);
//		if( pHogaData == NULL )
//			continue;
//
//		pHogaData->m_nRow = -1;
//	}
//	// <----------------------------------------------------
//
////	DRDS_X11_DATA* Op = (DRDS_X11_DATA*) pData;
//	//CMap<WORD, WORD, CString, CString> * pReal = (CMap<WORD, WORD, CString, CString>*) pRealData;
//	CRealUpdateData* pReal = (CRealUpdateData*)pRealData;
//
//	BOOL bRet, bOfferRem, bBidRem;
//	CString sTemp, sTemp1, sTemp2, sTemp3, strText;
//	int		nRow;
//	int		nTemp1, n5HogaMaesuJan, n5HogaMaedoJan;
//	nTemp1 = n5HogaMaesuJan = n5HogaMaedoJan = 0;
//	bRet = bOfferRem = bBidRem = FALSE;
//
//	// �����ڵ� üũ
//	bRet = pReal->GetDataString(DI_CODE,sTemp);
//	if(bRet)
//	{
//		sTemp1.Format("%7.7s" , sTemp);		sTemp1.TrimLeft();		sTemp1.TrimRight();
//		//	sTemp1.Format("%7.7s" , Op->shcode);		sTemp1.TrimLeft();		sTemp1.TrimRight();
//		m_strCode.TrimLeft();
//		if( m_strCode != sTemp1 )
//			return;
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	// �ǽð� �ŵ� ȣ�� ���ؿ�
//	////////////////////////////////////////////////////////////////////////// 
//	m_nHogaRow = MAEDO_ROW;
//	m_nHogaRangeForColor = 0;
//
//
//	// �ŵ� ȣ��/�ܷ� 5��
//	m_nHogaRow += 5;
//	//sTemp1.Format("%9.9s", Op->offerho5);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//	//sTemp2.Format("%10.10s", Op->offerrem5);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//	//sTemp3.Format("%10.10s", Op->oremain5);			sTemp3.TrimLeft();		sTemp3.TrimRight();
//	bRet = pReal->GetDataString(DI_OFFER_HO1+4,sTemp);
//	if(bRet)
//	{
//		sTemp1.Format("%9.9s", sTemp);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//		m_10HogaData.nOfferho5 = atoi(sTemp1);
//		//n5HogaMaedoJan += atoi(sTemp2);
//		SetRealHogaData_Sub(sTemp1, /*sTemp2, sTemp3,*/ m_strPreClosePrice, TRUE);
//	}
//	bRet = pReal->GetDataString(DI_OFFER_REM1+4,sTemp);
//	if(bRet)
//	{
//		bOfferRem = TRUE;
//		sTemp2.Format("%10.10s", sTemp);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//		SetRealHogaData_Rem(sTemp2, TRUE);
//	}
//	
//
//	// �ŵ� ȣ��/�ܷ� 4��
//	//sTemp1.Format("%9.9s", Op->offerho4);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//	//sTemp2.Format("%10.10s", Op->offerrem4);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//	//sTemp3.Format("%10.10s", Op->oremain4);			sTemp3.TrimLeft();		sTemp3.TrimRight();
//
//	m_nHogaRow++;
//	bRet = pReal->GetDataString(DI_OFFER_HO1+3,sTemp);
//	if(bRet)
//	{
//		sTemp1.Format("%9.9s", sTemp);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//		m_10HogaData.nOfferho4 = atoi(sTemp1);
//		//n5HogaMaedoJan += atoi(sTemp2);
//		SetRealHogaData_Sub(sTemp1, /*sTemp2, sTemp3,*/ m_strPreClosePrice, TRUE);
//	}
//	bRet = pReal->GetDataString(DI_OFFER_REM1+3,sTemp);
//	if(bRet)
//	{
//		bOfferRem = TRUE;
//		sTemp2.Format("%10.10s", sTemp);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//		SetRealHogaData_Rem(sTemp2, TRUE);
//	}
//
//	// �ŵ� ȣ��/�ܷ� 3��
//	//sTemp1.Format("%9.9s", Op->offerho3);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//	//sTemp2.Format("%10.10s", Op->offerrem3);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//	//sTemp3.Format("%10.10s", Op->oremain3);			sTemp3.TrimLeft();		sTemp3.TrimRight();
//	m_nHogaRow++;
//	bRet = pReal->GetDataString(DI_OFFER_HO1+2,sTemp);
//	if(bRet)
//	{
//		sTemp1.Format("%9.9s", sTemp);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//		m_10HogaData.nOfferho3 = atoi(sTemp1);
//		//n5HogaMaedoJan += atoi(sTemp2);
//		SetRealHogaData_Sub(sTemp1, /*sTemp2, sTemp3,*/ m_strPreClosePrice, TRUE);
//	}	
//	bRet = pReal->GetDataString(DI_OFFER_REM1+2,sTemp);
//	if(bRet)
//	{
//		bOfferRem = TRUE;
//		sTemp2.Format("%10.10s", sTemp);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//		SetRealHogaData_Rem(sTemp2, TRUE);
//	}	
//	
//	// �ŵ� ȣ��/�ܷ� 2��
//	//sTemp1.Format("%9.9s", Op->offerho2);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//	//sTemp2.Format("%10.10s", Op->offerrem2);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//	//sTemp3.Format("%10.10s", Op->oremain2);			sTemp3.TrimLeft();		sTemp3.TrimRight();
//	m_nHogaRow++;
//	bRet = pReal->GetDataString(DI_OFFER_HO1+1,sTemp);
//	if(bRet)
//	{
//		sTemp1.Format("%9.9s", sTemp);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//		m_10HogaData.nOfferho2 = atoi(sTemp1);
//		//n5HogaMaedoJan += atoi(sTemp2);
//		SetRealHogaData_Sub(sTemp1, /*sTemp2, sTemp3,*/ m_strPreClosePrice, TRUE);
//	}
//	bRet = pReal->GetDataString(DI_OFFER_REM1+1,sTemp);
//	if(bRet)
//	{
//		bOfferRem = TRUE;
//		sTemp2.Format("%10.10s", sTemp);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//		SetRealHogaData_Rem(sTemp2, TRUE);
//	}	
//
//	// �ŵ� ȣ��/�ܷ� 1��
//	//sTemp1.Format("%9.9s", Op->offerho1);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//	//sTemp2.Format("%10.10s", Op->offerrem1);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//	//sTemp3.Format("%10.10s", Op->oremain1);			sTemp3.TrimLeft();		sTemp3.TrimRight();
//	m_nHogaRow++;
//	bRet = pReal->GetDataString(DI_OFFER_HO1,sTemp);
//	if(bRet)
//	{
//		sTemp1.Format("%9.9s", sTemp);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//		m_10HogaData.nOfferho1 = atoi(sTemp1);
//		//n5HogaMaedoJan += atoi(sTemp2);
//		SetRealHogaData_Sub(sTemp1, /*sTemp2, sTemp3,*/ m_strPreClosePrice, TRUE);
//	}	
//	bRet = pReal->GetDataString(DI_OFFER_REM1,sTemp);
//	if(bRet)
//	{
//		bOfferRem = TRUE;
//		sTemp2.Format("%10.10s", sTemp);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//		SetRealHogaData_Rem(sTemp2, TRUE);
//	}	
//	
//	//////////////////////////////////////////////////////////////////////////////////
//	// �ǽð� �ż� ȣ�� ���ؿ�
//	//////////////////////////////////////////////////////////////////////////////////
//	m_nHogaRow = MAESU_ROW;  
//	
//	// �ż� ȣ��/�ܷ� 1��
//	//sTemp1.Format("%9.9s", Op->bidho1);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//	//sTemp2.Format("%10.10s", Op->bidrem1);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//	//sTemp3.Format("%10.10s", Op->bremain1);		sTemp3.TrimLeft();		sTemp3.TrimRight();
//	bRet = pReal->GetDataString(DI_BID_HO1,sTemp);
//	if(bRet)
//	{
//		sTemp1.Format("%9.9s", sTemp);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//		m_10HogaData.nBidho1 = atoi(sTemp1);
//		n5HogaMaesuJan += atoi(sTemp2);
//		SetRealHogaData_Sub(sTemp1,/*sTemp2, sTemp3,*/ m_strPreClosePrice, FALSE);
//	}
//	bRet = pReal->GetDataString(DI_BID_REM1,sTemp);
//	if (bRet)
//	{
//		bBidRem = TRUE;
//		sTemp2.Format("%10.10s", sTemp);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//		SetRealHogaData_Rem(sTemp2, FALSE);
//	}
//	
//	// �ż� ȣ��/�ܷ� 2��
//	//sTemp1.Format("%9.9s", Op->bidho2);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//	//sTemp2.Format("%10.10s", Op->bidrem2);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//	//sTemp3.Format("%10.10s", Op->bremain2);		sTemp3.TrimLeft();		sTemp3.TrimRight();
//	m_nHogaRow++;
//	bRet = pReal->GetDataString(DI_BID_HO1+1,sTemp);
//	if(bRet)
//	{
//		sTemp1.Format("%9.9s", sTemp);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//		m_10HogaData.nBidho2 = atoi(sTemp1);
//		n5HogaMaesuJan += atoi(sTemp2);
//		SetRealHogaData_Sub(sTemp1,/*sTemp2, sTemp3,*/ m_strPreClosePrice, FALSE);
//	}
//	bRet = pReal->GetDataString(DI_BID_REM1+1,sTemp);
//	if (bRet)
//	{
//		bBidRem = TRUE;
//		sTemp2.Format("%10.10s", sTemp);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//		SetRealHogaData_Rem(sTemp2, FALSE);
//	}
//
//	// �ż� ȣ��/�ܷ� 3��
//	//sTemp1.Format("%9.9s", Op->bidho3);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//	//sTemp2.Format("%10.10s", Op->bidrem3);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//	//sTemp3.Format("%10.10s", Op->bremain3);		sTemp3.TrimLeft();		sTemp3.TrimRight();
//	m_nHogaRow++;
//	bRet = pReal->GetDataString(DI_BID_HO1+2,sTemp);
//	if(bRet)
//	{
//		sTemp1.Format("%9.9s", sTemp);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//		m_10HogaData.nBidho3 = atoi(sTemp1);
//		n5HogaMaesuJan += atoi(sTemp2);
//		SetRealHogaData_Sub(sTemp1,/*sTemp2, sTemp3,*/ m_strPreClosePrice, FALSE);
//	}
//	bRet = pReal->GetDataString(DI_BID_REM1+2,sTemp);
//	if (bRet)
//	{
//		bBidRem = TRUE;
//		sTemp2.Format("%10.10s", sTemp);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//		SetRealHogaData_Rem(sTemp2, FALSE);
//	}
//
//	// �ż� ȣ��/�ܷ� 4��
//	//sTemp1.Format("%9.9s", Op->bidho4);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//	//sTemp2.Format("%10.10s", Op->bidrem4);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//	//sTemp3.Format("%10.10s", Op->bremain4);		sTemp3.TrimLeft();		sTemp3.TrimRight();
//	m_nHogaRow++;
//	bRet = pReal->GetDataString(DI_BID_HO1+3,sTemp);
//	if(bRet)
//	{
//		sTemp1.Format("%9.9s", sTemp);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//		m_10HogaData.nBidho4 = atoi(sTemp1);
//		n5HogaMaesuJan += atoi(sTemp2);
//		SetRealHogaData_Sub(sTemp1,/*sTemp2, sTemp3,*/ m_strPreClosePrice, FALSE);
//	}	
//	bRet = pReal->GetDataString(DI_BID_REM1+3,sTemp);
//	if (bRet)
//	{
//		bBidRem = TRUE;
//		sTemp2.Format("%10.10s", sTemp);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//		SetRealHogaData_Rem(sTemp2, FALSE);
//	}
//
//	// �ż� ȣ��/�ܷ� 5��
//	//sTemp1.Format("%9.9s", Op->bidho5);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//	//sTemp2.Format("%10.10s", Op->bidrem5);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//	//sTemp3.Format("%10.10s", Op->bremain5);		sTemp3.TrimLeft();		sTemp3.TrimRight();
//	m_nHogaRow++;
//	bRet = pReal->GetDataString(DI_BID_HO1+4,sTemp);
//	if (bRet)
//	{
//		sTemp1.Format("%9.9s", sTemp);			sTemp1.TrimLeft();		sTemp1.TrimRight();
//		m_10HogaData.nBidho5 = atoi(sTemp1);
//		n5HogaMaesuJan += atoi(sTemp2);
//		SetRealHogaData_Sub(sTemp1,/*sTemp2, sTemp3,*/ m_strPreClosePrice, FALSE);
//	}	
//	bRet = pReal->GetDataString(DI_BID_REM1+4,sTemp);
//	if (bRet)
//	{
//		bBidRem = TRUE;
//		sTemp2.Format("%10.10s", sTemp);		sTemp2.TrimLeft();		sTemp2.TrimRight();
//		SetRealHogaData_Rem(sTemp2, FALSE);
//	}
//
//
//
//	// ���ܷ� ---------------------------------------------------------------------------->>
//	// 5ȣ�� �ŵ� �� �ܷ�
//	if(bOfferRem)
//	{
//		for(int row=MAEDO_ROW+5; row < MAEDO_ROW+10; row++)
//		{
//			sTemp = m_pGridCtrl->GetItemText(row, MAEDO_JAN_COL);
//			sTemp.Remove(',');
//			n5HogaMaedoJan += atoi(sTemp);
//		}
//		sTemp1.Format("%d", n5HogaMaedoJan);
//		strText = m_Util.GetComma(sTemp1);
//		nRow = m_n5HogaSumRow;
//		CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);
//	}	
//	
//	// 5ȣ�� �ż� �� �ܷ�
//	if(bBidRem)
//	{
//		for(int row=MAESU_ROW; row < MAESU_ROW+5; row++)
//		{
//			sTemp = m_pGridCtrl->GetItemText(row, MAESU_JAN_COL);
//			sTemp.Remove(',');
//			n5HogaMaesuJan += atoi(sTemp);
//		}
//		sTemp1.Format("%d", n5HogaMaesuJan);
//		strText = m_Util.GetComma(sTemp1);
//		nRow = m_n5HogaSumRow;
//		CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);
//	}	
//
//	// �ŵ� �� �ܷ�
//	nRow = m_nTotalSumRow;
//	//sTemp1.Format("%10.10s", Op->offertotrem);		sTemp1.TrimLeft();		sTemp1.TrimRight();
//	//bRet = pReal->GetDataString(DI_OFFER_TOTREM,sTemp);
//	bRet = pReal->GetDataString(14565,sTemp);
//	if(bRet)
//	{
//		sTemp1.Format("%10.10s", sTemp);		sTemp1.TrimLeft();		sTemp1.TrimRight();
//		strText = m_Util.GetComma(sTemp1);
//		CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);
//	}	
//
//	// �ż� �� �ܷ�
//	nRow = m_nTotalSumRow;
//	//sTemp1.Format("%10.10s", Op->bidtotrem);		sTemp1.TrimLeft();		sTemp1.TrimRight();
//	//bRet = pReal->GetDataString(DI_BID_TOTREM,sTemp);
//	bRet = pReal->GetDataString(14567,sTemp);
//	if (bRet)
//	{
//		sTemp1.Format("%10.10s", sTemp);		sTemp1.TrimLeft();		sTemp1.TrimRight();
//		strText = m_Util.GetComma(sTemp1);
//		CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);
//	}	
//
//	//{{ 2007.06.26 by LYH ���� ���� ����
//
//	CString strSign;
//	bRet = pReal->GetDataString(15317, strSign);// �����ȣ
//	if(bRet)
//	{
//		if(strSign == "69")
//			m_chSign = '9';
//		else
//			m_chSign = strSign.GetAt(0);
//	}
//	if(m_chSign == '9')
//	{
//		double dExpRate = 0;
//
//		//������
//		bRet = pReal->GetDataString(15326,sTemp);
//		COLORREF clrChange = COLOR_BLACK;
//		if (bRet)
//		{
//			//sTemp1.Format("%10.10s", sTemp);		sTemp1.TrimLeft();		sTemp1.TrimRight();
//			//strText = m_Util.GetComma(sTemp1);
//			dExpRate = atof(sTemp)/100.;
//			sTemp1.Format("%.2f", dExpRate);
//			strText = sTemp1 + "%";
//
//			if ( dExpRate > 0)
//				clrChange = COLOR_RED;
//			else if ( dExpRate < 0)
//				clrChange = COLOR_BLUE;
//
//			CompareNUpdateHogaData(11, MAESU_JAN_COL, strText, GVIF_FGCLR, clrChange);
//		}	
//
//		//���󰡰�
//		bRet = pReal->GetDataString(15313,sTemp);
//		if (bRet)
//		{
//			sTemp1.Format("%10.10s", sTemp);		sTemp1.TrimLeft();		sTemp1.TrimRight();
//			strText = m_Util.GetComma(sTemp1);
//			CompareNUpdateHogaData(10, MAESU_JAN_COL, strText, GVIF_FGCLR, clrChange);
//		}	
//
//		//����ŷ���
//		bRet = pReal->GetDataString(15315,sTemp);
//		if (bRet)
//		{
//			sTemp1.Format("%10.10s", sTemp);		sTemp1.TrimLeft();		sTemp1.TrimRight();
//			strText = m_Util.GetComma(sTemp1) + "��";
//			CompareNUpdateHogaData(13, MAESU_JAN_COL, strText, 0, COLOR_BLACK);
//		}	
//	}
//	else
//	{
//		CompareNUpdateHogaData(10, MAESU_JAN_COL, "*0", GVIF_FGCLR, COLOR_BLACK);
//		CompareNUpdateHogaData(11, MAESU_JAN_COL, "0.00%", GVIF_FGCLR, COLOR_BLACK);
//		CompareNUpdateHogaData(13, MAESU_JAN_COL, "0��", 0, COLOR_BLACK);
//	}
//	//}}
//	// �ð��� �ŵ��ܷ�
////	nRow = m_nExtraVolRow;		
////	sTemp1.Format("%10.10s", Op->offerrem_ot);		sTemp1.TrimLeft();		sTemp1.TrimRight();
////	strText = m_Util.GetComma(sTemp1);
////	CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);
////	
////	// �ð��� �ż��ܷ�
////	nRow = m_nExtraVolRow;		
////	sTemp1.Format("%10.10s", Op->bidrem_ot);		sTemp1.TrimLeft();		sTemp1.TrimRight();
////	strText = m_Util.GetComma(sTemp1);
////	CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);
//	
//	// 10�ܰ� ȣ������ ȣ���� �ٲ� ���簡���� ã�´�. ------------------------>>
//	// ***** 10ȣ�� �׸��� ������ *****
//	int nHogaData[TENHOGA_SAVED_CNT] = {0,};
//	if (m_bIsTenHoga == TRUE)
//	{
//		m_10HogaData.nHighLimit = m_nHighLimit;
//		
//		sTemp1 = m_pGridCtrl->GetItemText(ETC_HOGAUP_ROW, HOGA_COL);	sTemp1.Remove(',');
//		m_10HogaData.nExtraUp = atoi(sTemp1);
//		
//		sTemp1 = m_pGridCtrl->GetItemText(m_nLowLimitRow-1, HOGA_COL);	sTemp1.Remove(',');
//		m_10HogaData.nExtraDown = atoi(sTemp1);
//		
//		m_10HogaData.nLowLimit = m_nLowLimit;
//
//		memcpy(nHogaData, &m_10HogaData, TEN_HOGADATA_SIZE);
//
//		// ���簡��(���Ѱ��� ���Ѱ� ���� üũ ü��) ----------------->>>
//		BOOL bFindRow = FALSE;
//		int nPriceTemp = atoi(m_Util.GetNumber(m_strCurPrice));
//
//		for(int nCnt = 1; nCnt < TENHOGA_SAVED_CNT-1; nCnt++)
//		{
//			if (nPriceTemp ==  nHogaData[nCnt])
//			{
//				bFindRow = TRUE;
//				m_nCurPriceRow = nCnt+HIGH_ROW;
//				break;
//			}
//		}
//		if (bFindRow == FALSE)
//		{
//			m_nCurPriceRow = -1;
//		}
//
////		m_pGridCtrl->SetCurPriceRow(m_nCurPriceRow);
//		// <<<---------------------------------------------------------	
//		
//	}
//	//<<---------------------------------------------------------------------------
//
//}
//@��������

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
	strText = m_Util.GetComma(strHoga);

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

		if( pHogaData->m_strText == strData )
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
	//if(m_hBitmap!=NULL)	// gm0604 added..2005.9.9
	//{
	//	DeleteObject(m_hBitmap);
	//	m_hBitmap = NULL;
	//}	
//@solomon	20091113
//	m_ImageListOrder.DeleteImageList();
	//AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
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

void CFloatingHogaDlg::SetMarkRealCurPrice(char *pData, int nSize)
{
//	STPriceData* Op = (STPriceData*) pData;
//
//	CString strPrice, strCode, sTemp1;	
//	strCode.Format("%*.*s",  sizeof(Op->shcode)	,sizeof(Op->shcode)	,Op->shcode); strCode.TrimLeft() ; strCode.TrimRight() ;
//
//	//������ �ٸ��� Return;
//	if(strCode.CompareNoCase(m_strCode) != 0) return;
//
//	// ���簡
//	sTemp1.Format("%9.9s", Op->price);	sTemp1.TrimLeft();		sTemp1.TrimRight();	
//	strPrice = m_Util.GetComma(sTemp1);
//	m_strCurPrice = strPrice;
//	//strPrice.Format("%*.*s", sizeof(Op->price)	,sizeof(Op->price)	,Op->price)	; strPrice.TrimLeft(); strPrice.TrimRight();
//
////	// ***** 10ȣ�� �׸��� ������ *****
////	int nHogaData[TENHOGA_SAVED_CNT] = {0,};
////	if (m_bIsTenHoga == TRUE)
////	{
////		m_10HogaData.nHighLimit = m_nHighLimit;
////		
////		sTemp1 = m_pGridCtrl->GetItemText(ETC_HOGAUP_ROW, HOGA_COL);	sTemp1.Remove(',');
////		m_10HogaData.nExtraUp = atoi(sTemp1);
////		
////		sTemp1 = m_pGridCtrl->GetItemText(m_nLowLimitRow-1, HOGA_COL);	sTemp1.Remove(',');
////		m_10HogaData.nExtraDown = atoi(sTemp1);
////		
////		m_10HogaData.nLowLimit = m_nLowLimit;
////
////		memcpy(nHogaData, &m_10HogaData, TEN_HOGADATA_SIZE);
////	}
////	//*********************************
//
//
//
//	if (m_bIsTenHoga == TRUE)
//	{
//		int nHogaData[TENHOGA_SAVED_CNT] = {0,};	
//		m_10HogaData.nHighLimit = m_nHighLimit;
//		
//		sTemp1 = m_pGridCtrl->GetItemText(ETC_HOGAUP_ROW, HOGA_COL);	sTemp1.Remove(',');
//		m_10HogaData.nExtraUp = atoi(sTemp1);
//		
//		sTemp1 = m_pGridCtrl->GetItemText(m_nLowLimitRow-1, HOGA_COL);	sTemp1.Remove(',');
//		m_10HogaData.nExtraDown = atoi(sTemp1);
//		
//		m_10HogaData.nLowLimit = m_nLowLimit;
//
//		memcpy(nHogaData, &m_10HogaData, TEN_HOGADATA_SIZE);
//
//		// ���簡��(���Ѱ��� ���Ѱ� ���� üũ ü��) ----------------->>>
//		BOOL bFindRow = FALSE;
//		int nPriceTemp = atoi(m_Util.GetNumber(m_strCurPrice));//atoi(strPrice);
//
//		for(int nCnt = 1; nCnt < TENHOGA_SAVED_CNT-1; nCnt++)
//		{
//			if (nPriceTemp ==  nHogaData[nCnt])
//			{
//				bFindRow = TRUE;
//				m_nCurPriceRow = nCnt+HIGH_ROW;
//				break;
//			}
//		}
//		if (bFindRow == FALSE)
//		{
//			m_nCurPriceRow = -1;
//		}
//
//		m_pGridCtrl->SetCurPriceRow(m_nCurPriceRow);
//		// <<<---------------------------------------------------------
//
//
///*		// ���簡��(���Ѱ��� ���Ѱ� ���� üũ ü��) ----------------->>>
//		BOOL bFindRow = FALSE;
//		int nPriceTemp = atoi(m_Util.GetNumber(m_strCurPrice));
//
//		for(int nCnt = 1; nCnt < TENHOGA_SAVED_CNT-1; nCnt++)
//		{
//			if (nPriceTemp ==  nHogaData[nCnt])
//			{
//				bFindRow = TRUE;
//				m_nCurPriceRow = nCnt+HIGH_ROW;
//				break;
//			}
//		}
//		if (bFindRow == FALSE)
//		{
//			m_nCurPriceRow = -1;
//		}
//
//		m_pGridCtrl->SetCurPriceRow(m_nCurPriceRow);
//		// <<<---------------------------------------------------------*/
//	}
}

void CFloatingHogaDlg::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	CRect dlgRect(0,0,0,0);

	m_staticSymbol.GetWindowRect(dlgRect);//������ ��ǥ...
	m_bIconState = !m_bIconState;
		
	if(m_pMainDlg)
		m_pMainDlg->SendMessage(USMG_PLEASE_RESIZE, m_bIconState ? 1 : 0, (LPARAM)&dlgRect);

	if(m_bIconState && m_strCode.GetLength()>0)
		OnCodeUnAdvise(m_strCode);	// Old Code����
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

//@solomon	20091113	-->>
/*
void CFloatingHogaDlg::OnButtonBuy() 
{
	AfxMessageBox("�ż�ȭ�� ���");
//	//1:HTS  0:FRONT(����)
//	int nUserType = 1;
//	IMainInfoManager* pMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
//	if(pMng) nUserType = atoi(pMng->GetLoginInfo("ISHTS"));
//	CString szScreen = "hfib21020";
//	if(nUserType==0)	szScreen = "hfij20020";
//	
//	GetParent()->GetParent()->GetParent()->SendMessage(WM_USER+102, (WPARAM)(LPSTR)(LPCSTR)szScreen, 0);
}

void CFloatingHogaDlg::OnButtonSell() 
{
	AfxMessageBox("�ŵ�ȭ�� ���");
//	//1:HTS  0:FRONT(����)
//	int nUserType = 1;
//	IMainInfoManager* pMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
//	if(pMng) nUserType = atoi(pMng->GetLoginInfo("ISHTS"));
//	CString szScreen = "hfib21010";
//	if(nUserType==0)	szScreen = "hfij20010";
//
//	GetParent()->GetParent()->GetParent()->SendMessage(WM_USER+102, (WPARAM)(LPSTR)(LPCSTR)szScreen, 0);
}
*/
//@solomon	20091113	<<--

BOOL CFloatingHogaDlg::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

//-->
BOOL CFloatingHogaDlg::MakeDrdsConnectAdvise()
{
	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
	if(!pDrdsMng) return FALSE;

	m_pDrdsLib = pDrdsMng->AdviseDrdsLib();
	if(!m_pDrdsLib) return FALSE;

	m_pDrdsLib->DrdsInit(m_hWnd);

	return TRUE;
}

BOOL CFloatingHogaDlg::MakeDrdsUnConnectUnAdvise()
{
	if(!m_pDrdsLib) return FALSE;

	if(m_strCode.GetLength()>0)
		OnCodeUnAdvise(m_strCode);	// Old Code����

	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
	if(!pDrdsMng) return FALSE;

	pDrdsMng->UnadviseDrdsLib(m_pDrdsLib);
	m_pDrdsLib = NULL;

	return TRUE;
}

void CFloatingHogaDlg::OnCodeAdvise(LPCSTR strCode)
{
	if(!m_pDrdsLib) return;

	int nKXH1RealType = 1;	// 1:ȣ��
	m_pDrdsLib->DrdsAdvise(nKXH1RealType, "KXH1", "*", (LPSTR)strCode);
	int nKXYSRealType = 23;	// 1:����
	m_pDrdsLib->DrdsAdvise(nKXYSRealType, "KXYS", "*", (LPSTR)strCode);
}

void CFloatingHogaDlg::OnCodeUnAdvise(LPCSTR strCode)
{
	if(!m_pDrdsLib) return;

	int nKXH1RealType = 1;	// 1:ȣ��
	m_pDrdsLib->DrdsUnadvise(nKXH1RealType, "KXH1", "*", (LPSTR)strCode);
	int nKXYSRealType = 23;	// 1:����
	m_pDrdsLib->DrdsUnadvise(nKXYSRealType, "KXYS", "*", (LPSTR)strCode);
}

//-->@Solomon090819
// long CFloatingHogaDlg::OnGetBroad(WPARAM wParam, LPARAM lParam)
// {
// 	if(!m_pDrdsLib) return 0;
// 
// 	BC_ITEM_INFO *pRealItem = (BC_ITEM_INFO*)lParam;
// 	if(strncmp(pRealItem->szBcCode, NAME_KXH1, strlen(NAME_KXH1)) == 0)			// ȣ��
// 	{
// 		SetRealHogaData(pRealItem->szBcData);
// 		return 1L;
// 	}
// 
// 	if(strncmp(pRealItem->szBcCode, NAME_KXYS, strlen(NAME_KXYS)) == 0)			// ����
// 	{
// 		SetRealYeShangData(pRealItem->szBcData);
// 		return 1L;
// 	}
// 
// 	return 0;
// }
//<--@Solomon090819

void CFloatingHogaDlg::SetRealYeShangData(LPCSTR pRealStruct)
{
	if(!m_pGridCtrl)
		return;
	
	CKBREAL_KXYS kbReal_KXYS;
	kbReal_KXYS.Convert((LPKXYS_OutBlock)pRealStruct);

	CString	strPrice, strExpPrice, strExpVol, strExpRate, strChangeRate;
//>> [QC No]090803HMC075
//	strExpRate.Format("%s", kbReal_KXYS.m_szpreychange);
	strExpRate.Format("%s", kbReal_KXYS.m_szjnilychange);
//<< [QC No]090803HMC075

	{
		// ���� ü�ᰡ��
		strPrice = kbReal_KXYS.m_szyeprice;
		strExpPrice = /*"*" + */m_Util.GetComma(kbReal_KXYS.m_szyeprice);
		// ���� ü�����
		strExpVol = m_Util.GetComma(kbReal_KXYS.m_szyevolume) + "��";
		
		long lPrice = atol(strPrice);
		long lExpRate = atol(strExpRate);
//		double dChangeRate = (double)((double)lExpRate/(double)(lPrice-lExpRate)) * 100.;
		double dChangeRate = (double)((double)atol(kbReal_KXYS.m_szjnilydrate)/100.);
		strChangeRate.Format("%.2f", dChangeRate);
		strChangeRate += "%";
		
		COLORREF clrChange;
//>> [QC No]090803HMC075
//		int nSign = atoi(kbReal_KXYS.m_szpreysign);
		int nYSign = atoi(kbReal_KXYS.m_szjnilysign);
//<< [QC No]090803HMC075
		if ( nYSign < 3 )
			clrChange = COLOR_RED;
		else if ( nYSign > 3 )
			clrChange = COLOR_BLUE;
		else
			clrChange = COLOR_BLACK;
		
		CompareNUpdateHogaData(9, MAESU_JAN_COL, "����ü�ᰡ", 0, COLOR_BLACK);
		CompareNUpdateHogaData(10, MAESU_JAN_COL, strExpPrice, GVIF_FGCLR, clrChange);
		CompareNUpdateHogaData(11, MAESU_JAN_COL, strChangeRate, GVIF_FGCLR, clrChange);
		CompareNUpdateHogaData(12, MAESU_JAN_COL, "����ü�ᷮ", 0, COLOR_BLACK);
		CompareNUpdateHogaData(13, MAESU_JAN_COL, strExpVol, 0, COLOR_BLACK);
	}
}

void CFloatingHogaDlg::SetRealHogaData(DWORD dwMethodKey, CString strCode, LONG _pMapRealData)
{
	CString strData;

	if(dwMethodKey==0)
	{
		CMapStringToString* pMapRealData = (CMapStringToString*)_pMapRealData;
		if (!pMapRealData->Lookup(_T("071"), strData))
			return;
		
		CKBREAL_KXH1 realData;
		realData.m_szkshcode = strCode;

		if( m_strCode != strCode )	return;

		pMapRealData->Lookup(_T("071"), realData.m_szbidho1);
		pMapRealData->Lookup(_T("072"), realData.m_szbidho2);
		pMapRealData->Lookup(_T("073"), realData.m_szbidho3);
		pMapRealData->Lookup(_T("074"), realData.m_szbidho4);
		pMapRealData->Lookup(_T("075"), realData.m_szbidho5);
		pMapRealData->Lookup(_T("051"), realData.m_szofferho1);
		pMapRealData->Lookup(_T("052"), realData.m_szofferho2);
		pMapRealData->Lookup(_T("053"), realData.m_szofferho3);
		pMapRealData->Lookup(_T("054"), realData.m_szofferho4);
		pMapRealData->Lookup(_T("055"), realData.m_szofferho5);
		pMapRealData->Lookup(_T("061"), realData.m_szbidrem1);
		pMapRealData->Lookup(_T("062"), realData.m_szbidrem2);
		pMapRealData->Lookup(_T("063"), realData.m_szbidrem3);
		pMapRealData->Lookup(_T("064"), realData.m_szbidrem4);
		pMapRealData->Lookup(_T("065"), realData.m_szbidrem5);
		pMapRealData->Lookup(_T("041"), realData.m_szofferrem1);
		pMapRealData->Lookup(_T("042"), realData.m_szofferrem2);
		pMapRealData->Lookup(_T("043"), realData.m_szofferrem3);
		pMapRealData->Lookup(_T("044"), realData.m_szofferrem4);
		pMapRealData->Lookup(_T("045"), realData.m_szofferrem5);
		
		pMapRealData->Lookup(_T("101"), realData.m_sztotofferrem);
		pMapRealData->Lookup(_T("106"), realData.m_sztotbidrem);

		SetRealHogaData(realData);
	}
	else if(dwMethodKey==1)
	{
		DWORD* pArrData = (DWORD*)_pMapRealData;

		strData = (char*)pArrData[ 71];
//		if(strData.IsEmpty()==TRUE)	return;
		
		CKBREAL_KXH1 realData;
		CString szFlag = (char*)pArrData[ 0];

		realData.m_szkshcode	= strCode;
		if( m_strCode != strCode )	return;

		realData.m_szbidho1		= (char*)pArrData[ 71];
		realData.m_szbidho2		= (char*)pArrData[ 72];

		if(realData.m_szofferho1.IsEmpty() && realData.m_szbidho1.IsEmpty())
			return;

		realData.m_szbidho3		= (char*)pArrData[ 73];
		realData.m_szbidho4		= (char*)pArrData[ 74];
		realData.m_szbidho5		= (char*)pArrData[ 75];
		realData.m_szofferho1	= (char*)pArrData[ 51];
		realData.m_szofferho2	= (char*)pArrData[ 52];
		realData.m_szofferho3	= (char*)pArrData[ 53];
		realData.m_szofferho4	= (char*)pArrData[ 54];
		realData.m_szofferho5	= (char*)pArrData[ 55];
		realData.m_szbidrem1	= (char*)pArrData[ 61];
		realData.m_szbidrem2	= (char*)pArrData[ 62];
		realData.m_szbidrem3	= (char*)pArrData[ 63];
		realData.m_szbidrem4	= (char*)pArrData[ 64];
		realData.m_szbidrem5	= (char*)pArrData[ 65];
		realData.m_szofferrem1	= (char*)pArrData[ 41];
		realData.m_szofferrem2	= (char*)pArrData[ 42];
		realData.m_szofferrem3	= (char*)pArrData[ 43];
		realData.m_szofferrem4	= (char*)pArrData[ 44];
		realData.m_szofferrem5	= (char*)pArrData[ 45];
		
		realData.m_sztotofferrem = (char*)pArrData[101];
		realData.m_sztotbidrem	 = (char*)pArrData[106];

		SetRealHogaData(realData);
	}	
}

//@SM-100104SM001. ����ü���϶� Minus(-)���� �ͼ� ���� ��Ŵ.
void CFloatingHogaDlg::RemoveExtra(CKBREAL_KXH1 &kbReal_KXH1)
{
	kbReal_KXH1.m_szofferho1.Remove('-');
	kbReal_KXH1.m_szofferho2.Remove('-');
	kbReal_KXH1.m_szofferho3.Remove('-');
	kbReal_KXH1.m_szofferho4.Remove('-');
	kbReal_KXH1.m_szofferho5.Remove('-');

	kbReal_KXH1.m_szbidho1.Remove('-');
	kbReal_KXH1.m_szbidho2.Remove('-');
	kbReal_KXH1.m_szbidho3.Remove('-');
	kbReal_KXH1.m_szbidho4.Remove('-');
	kbReal_KXH1.m_szbidho5.Remove('-');
}

void CFloatingHogaDlg::SetRealHogaData(CKBREAL_KXH1 &kbReal_KXH1)
{
	if(!m_pGridCtrl)	return;

	RemoveExtra(kbReal_KXH1);

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

	CString sTemp, sTemp1, strText;
	int		nRow;
	int		n5HogaMaesuJan = 0, n5HogaMaedoJan = 0;

	// �����ڵ� üũ
	if( m_strCode != kbReal_KXH1.m_szkshcode )
		return;

	//////////////////////////////////////////////////////////////////////////
	// �ǽð� �ŵ� ȣ�� ���ؿ�
	////////////////////////////////////////////////////////////////////////// 
	m_nHogaRow = MAEDO_ROW;
	m_nHogaRangeForColor = 0;

	// �ŵ� ȣ��/�ܷ� 5��
	m_nHogaRow += 5;
	m_10HogaData.nOfferho5 = atoi(kbReal_KXH1.m_szofferho5);
	SetRealHogaData_Sub(kbReal_KXH1.m_szofferho5, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(kbReal_KXH1.m_szofferrem5, TRUE);
	

	// �ŵ� ȣ��/�ܷ� 4��
	m_nHogaRow++;
	m_10HogaData.nOfferho4 = atoi(kbReal_KXH1.m_szofferho4);
	SetRealHogaData_Sub(kbReal_KXH1.m_szofferho4, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(kbReal_KXH1.m_szofferrem4, TRUE);

	// �ŵ� ȣ��/�ܷ� 3��
	m_nHogaRow++;
	m_10HogaData.nOfferho3 = atoi(kbReal_KXH1.m_szofferho3);
	SetRealHogaData_Sub(kbReal_KXH1.m_szofferho3, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(kbReal_KXH1.m_szofferrem3, TRUE);
	
	// �ŵ� ȣ��/�ܷ� 2��
	m_nHogaRow++;
	m_10HogaData.nOfferho2 = atoi(kbReal_KXH1.m_szofferho2);
	SetRealHogaData_Sub(kbReal_KXH1.m_szofferho2, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(kbReal_KXH1.m_szofferrem2, TRUE);

	// �ŵ� ȣ��/�ܷ� 1��
	m_nHogaRow++;
	m_10HogaData.nOfferho1 = atoi(kbReal_KXH1.m_szofferho1);
	SetRealHogaData_Sub(kbReal_KXH1.m_szofferho1, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(kbReal_KXH1.m_szofferrem1, TRUE);
	
	//////////////////////////////////////////////////////////////////////////////////
	// �ǽð� �ż� ȣ�� ���ؿ�
	//////////////////////////////////////////////////////////////////////////////////
	m_nHogaRow = MAESU_ROW;  
	
	// �ż� ȣ��/�ܷ� 1��
	m_10HogaData.nBidho1 = atoi(kbReal_KXH1.m_szbidho1);
	SetRealHogaData_Sub(kbReal_KXH1.m_szbidho1, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(kbReal_KXH1.m_szbidrem1, FALSE);
	
	// �ż� ȣ��/�ܷ� 2��
	m_nHogaRow++;
	m_10HogaData.nBidho2 = atoi(kbReal_KXH1.m_szbidho2);
	SetRealHogaData_Sub(kbReal_KXH1.m_szbidho2, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(kbReal_KXH1.m_szbidrem2, FALSE);

	// �ż� ȣ��/�ܷ� 3��
	m_nHogaRow++;
	m_10HogaData.nBidho3 = atoi(kbReal_KXH1.m_szbidho3);
	SetRealHogaData_Sub(kbReal_KXH1.m_szbidho3, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(kbReal_KXH1.m_szbidrem3, FALSE);

	// �ż� ȣ��/�ܷ� 4��
	m_nHogaRow++;
	m_10HogaData.nBidho4 = atoi(kbReal_KXH1.m_szbidho4);
	SetRealHogaData_Sub(kbReal_KXH1.m_szbidho4, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(kbReal_KXH1.m_szbidrem4, FALSE);

	// �ż� ȣ��/�ܷ� 5��
	m_nHogaRow++;
	m_10HogaData.nBidho5 = atoi(kbReal_KXH1.m_szbidho5);
	SetRealHogaData_Sub(kbReal_KXH1.m_szbidho5, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(kbReal_KXH1.m_szbidrem5, FALSE);

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
	strText = m_Util.GetComma(kbReal_KXH1.m_sztotofferrem);
	CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);

	// �ż� �� �ܷ�
	nRow = m_nTotalSumRow;
	strText = m_Util.GetComma(kbReal_KXH1.m_sztotbidrem);
	CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);
}

LRESULT CFloatingHogaDlg::OnNcHitTest(CPoint point)
{
	return CRscDlg::OnNcHitTest(point);

//	UINT nHitTest = CWnd::OnNcHitTest( point );
//	return (nHitTest == HTCLIENT) ? HTCAPTION : nHitTest;
}
