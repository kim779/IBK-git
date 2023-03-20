 // TrConVolView.cpp : implementation file
//

#include "stdafx.h"
//#include "TrConVolView.h"
#include "../../itgen/interface/ICtlBase.h"
#include "../../itgen/interface/IFormMngr.h"
#include "../../itgen/CommonSrc/CtlColor.h"
#include "../../ChartDll/Include_Chart/Dll_Load/IMetaTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_FORM			999

#define WM_RP_RECEIVED				WM_USER + 1000
#define UMSG_SENDSHAREDCODE			WM_USER+30			// Main�����޸𸮿����� �ڵ带 ������.
#define UMSG_RECEIVE_FIND0301		WM_USER+166			// 05.07. 30 ���ǰ˻� ������� �޴´�.


//extern void*   dll_GetCodeMaster( );
//extern BOOL    dll_GetMasterData(CString szType, CStringArray &arrCode, CStringArray &arrName);
//extern BOOL    dll_GetMasterDataInUp(CString szType, CStringArray &arrCode, CStringArray &arrName, CString strUp);
//extern LPCTSTR dll_GetJongmokName( CString szType, LPCTSTR szCode, BOOL bFullName=TRUE);
//extern CWnd* CREATE_TRADING(int iTradingID, CWnd* pParent);

typedef	CWnd * (CreateTradingAup)(UINT nDldresId, CWnd* pParent, LPCTSTR strMapFileName);

extern WORD GetCurLangeCode ( );

#define SafeDelete(xpVal) if(xpVal) delete xpVal; xpVal=NULL;
#define MakeNull(xpVal) xpVal=NULL;
#define		MAX_ROWCNT	500

extern HINSTANCE g_hInstance;
/////////////////////////////////////////////////////////////////////////////
// CTrConVolView

IMPLEMENT_DYNCREATE(CTrConVolView, CScrFormView)

CTrConVolView::CTrConVolView()
	: CScrFormView(CTrConVolView::IDD)
{
	m_pwndMainFrm = NULL;
	m_nScreenType = 0;

	//{{AFX_DATA_INIT(CTrConVolView)
	m_nInitFlag     = 0;

//	m_pDllWindow	= NULL;

//	m_hInstance		= NULL;
//	m_pDllWindow	= NULL;
	m_strScreenNo	= "";
	m_strMapFileName= "";
	m_hChildFrame   = NULL;
	//}}AFX_DATA_INIT

//	m_hDLLIns = NULL;
//	m_pCrInfo = NULL;
//	m_pTRConVolSite = NULL;
	m_nRowTotalSize = 0;
	m_bFirst = TRUE;
	m_nOld_InsertType = 0;

	m_bWatching		= FALSE;
}

CTrConVolView::~CTrConVolView()
{
//	if(m_pDllWindow != NULL)
//	{
//		m_pDllWindow->DestroyWindow();
//		delete m_pDllWindow;
//		m_pDllWindow = NULL;
//	}
	
//	if(m_hInstance)
//		FreeLibrary(m_hInstance);
}

void CTrConVolView::DoDataExchange(CDataExchange* pDX)
{
	CScrFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrConVolView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrConVolView, CScrFormView)
	//{{AFX_MSG_MAP(CTrConVolView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_GET_SKINCOLOR, OnGetSkinColor)
	ON_REGISTERED_MESSAGE(RMSG_KSMOVECTRL, OnRmsgKSMoveCtrl)
END_MESSAGE_MAP()

// ��ȭ�� ��Ʈ�ѷκ��� �ް� ���� �̺�Ʈ�� ����ϼ���.
// ��: ON_EVENT(CConditionSearch_OneView, 3780,	CfxImgButton::eidClick,	OnClickBtn, VTS_NONE)
BEGIN_EVENTSINK_MAP(CTrConVolView, CScrFormView)
	ON_EVENT(CTrConVolView, CTLID_FORM,	CWrpForm::eventidOnFormAfterInit,		OnFormAfterInit,	VTS_NONE)
	ON_EVENT(CTrConVolView, CTLID_FORM,	CWrpForm::eventidOnFormBeforeInit,		OnFormBeforeInit,	VTS_NONE)
	ON_EVENT(CTrConVolView, CTLID_TRANDM,	CWrpTranDM::eventidReceiveData,			OnReceiveTranData,	VTS_BSTR)
	ON_EVENT(CTrConVolView, CTLID_TRANDM,	CWrpTranDM::eventidReceiveComplete,	OnReceiveTranCompleteData, VTS_BSTR)
	ON_EVENT(CTrConVolView, CTLID_REALDM,	CWrpRealDM::eventidReceive,			OnReceiveRealData,	VTS_BSTR)
	ON_EVENT(CTrConVolView, CTLID_REALDM,	CWrpRealDM::eventidReceiveComplete,	OnReceiveRealCompleteData, VTS_BSTR)
	ON_EVENT(CTrConVolView, 3775, CfxImgButton::eventidOnClick, OnClickbtnErase, VTS_NONE)
	ON_EVENT(CTrConVolView, 3785, CfxImgButton::eventidOnClick, OnClickbtnFieldSet, VTS_NONE)
	ON_EVENT(CTrConVolView, 3790, CfxImgButton::eventidOnClick, OnClickbntConditionSet, VTS_NONE)
	ON_EVENT(CTrConVolView, 3795, CfxImgButton::eventidOnClick, OnClickbtnTargetItem, VTS_NONE)
	ON_EVENT(CTrConVolView, 3780, CfxImgButton::eventidOnClick, OnClickbtnWatchSet, VTS_NONE)
	ON_EVENT(CTrConVolView, CTLID_FORM,	CWrpForm::eventidOnChangeColorTbl,		OnChangeColorTbl,		VTS_I2)	
	
	ON_EVENT(CTrConVolView, 3800, CCodeCtrl::eventidEditFull, EditFullSelectMarketCode, VTS_NONE)
	ON_EVENT(CTrConVolView, 3800, CCodeCtrl::eventidEditEnter, EditEnterSelectMarketCode, VTS_NONE)
	ON_EVENT(CTrConVolView, 3805, CCodeCtrl::eventidEditFull, EditFullExceptItemCode, VTS_NONE)
	ON_EVENT(CTrConVolView, 3805, CCodeCtrl::eventidEditEnter, EditEnterExceptItemCode, VTS_NONE)
	
	ON_EVENT(CTrConVolView, 3810, CfxImgButton::eventidOnClick, OnClickBtnAdd, VTS_NONE)
	ON_EVENT(CTrConVolView, 3815, CfxImgButton::eventidOnClick, OnClickBtnRemove, VTS_NONE)
	ON_EVENT(CTrConVolView, 3820, CfxImgButton::eventidOnClick, OnClickBtnRemoveAll, VTS_NONE)

END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrConVolView diagnostics

#ifdef _DEBUG
void CTrConVolView::AssertValid() const
{
	CScrFormView::AssertValid();
}

void CTrConVolView::Dump(CDumpContext& dc) const
{
	CScrFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTrConVolView message handlers
// ��Ʈ�ѿ� ���� WrapŬ���� ������ �����Ѵ�.
void CTrConVolView::OnInitialControl() 
{
	AttachCtrl(&m_ctrlEraseBtn, "btnErase");
	AttachCtrl(&m_ctrlFieldSetBtn, "btnFieldSet");
	AttachCtrl(&m_ctrlConditionSetBtn, "bntConditionSet");
	AttachCtrl(&m_ctrlTargetItemBtn, "btnTargetItem");
	AttachCtrl(&m_btnWatchSet, "btnWatchSet");
	AttachCtrl(&m_ctrlSelectUJCombo, "ItemCode1");
	AttachCtrl(&m_ctrlExceptJMCombo, "ItemCode2");
	AttachCtrl(&m_btnAdd, "btnAddEx");
	AttachCtrl(&m_btnRemove, "btnRemoveEx");
	AttachCtrl(&m_btnRemoveAll, "btnRemoveAllEx");

	m_ctrlTargetItemBtn.SetVisibleForecastPriceCol(FALSE);
}

// ȭ�鿡 ���� �ʱ�ȭ�� �Ͼ�� ���� ���´�.
// DLL�� ����/Ŭ���� ���� �ʱⰪ �� ��ȭ���� ������ 
// �����νð� �ƴ� ���ǿ� ���� �ʱ� �ε� ������ ������ �Ѵ�.
void CTrConVolView::OnFormBeforeInit() 
{
	CWrpForm* pForm = GetFormCtl();
	pForm->LockFontResize(TRUE);
}

// ȭ�鿡 ���� ��� �ʱ�ȭ �� ������������, ��ġ�ϸ� �������� �ε�, �ʱ� �̺�Ʈ TR ȣ���� ������ ��� �´�. 
void CTrConVolView::OnFormAfterInit() 
{
	DoDllCreate();
}

// ��ȸ �����Ϳ� ���� ������ ���� ������ ȣ��ȴ�.
// �����Ϳ� ����� ��Ʈ�ѿ� ������Ʈ �ϱ� ���� ���´�.
void CTrConVolView::OnReceiveTranData(LPCTSTR szTranID)
{
}

// ��ȸ �����Ϳ� ���� ������ ���� ������ ȣ��ȴ�.
// �����Ϳ� ����� ��Ʈ�ѿ� ������Ʈ �� �Ŀ� ���´�.
void CTrConVolView::OnReceiveTranCompleteData(LPCTSTR szTranID)
{
}

// ����� �ǽð� �����Ͱ� ���������� ȣ��ȴ�.
// �����Ϳ� ����� ��Ʈ�ѿ� ������Ʈ �ϱ� ���� ���´�.
void CTrConVolView::OnReceiveRealData(LPCTSTR szTranID)
{
}

// ����� �ǽð� �����Ͱ� ���������� ȣ��ȴ�.
// �����Ϳ� ����� ��Ʈ�ѿ� ������Ʈ �� �Ŀ� ���´�.
void CTrConVolView::OnReceiveRealCompleteData(LPCTSTR szTranID)
{
}

int CTrConVolView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	OnColorChange();
	m_crBackColor = RGB(255, 255, 255);

	return 0;
}
	
void CTrConVolView::OnSize(UINT nType, int cx, int cy) 
{
	Invalidate();
	
	CScrFormView::OnSize(nType, cx, cy);

	OnSize(cx, cy);
}

void CTrConVolView::OnSize(int cx, int cy)
{
	if (m_pwndMainFrm != NULL)
		m_pwndMainFrm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE|SWP_NOZORDER);	
}

void CTrConVolView::OnInitialUpdate() 
{	
	CScrFormView::OnInitialUpdate();

	if( m_nInitFlag == 0 )
	{
		HINSTANCE hInstSave = AfxGetResourceHandle();
		AfxSetResourceHandle(g_hInstance);
		
		g_iMetaTable.LoadMetaTable();
		m_LinkDataManager.Initialize();
		g_iMetaTable.SetHTSID(atoi(m_LinkDataManager.GetMediaType()));

		SetInit_MainWnd();	
		
//		IKSLibManager*	pKSMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager);
//		if(pKSMng)
		{
			CString strReg="", strRight="";
			strReg = AfxGetApp()->GetProfileString("STBasic", "UserL", "");

			if(strReg.CompareNoCase("A") == 0 || strReg.CompareNoCase("S") == 0)
				strRight = "1";
			
//			if(strRight.GetLength() == 0)
//				strRight = pKSMng->Lib_KSLink_GetLoginInfo(CHEGYEL_ARG);

		//	CString strRight = pKSMng->Lib_KSLink_GetLoginInfo(CHEGYEL_ARG);
			m_pwndMainFrm->SetUserRight(atoi(strRight));		
		}
		
		m_bWatching = TRUE;
		m_btnWatchSet.SetForeColor(RGB(255, 0, 0));
		m_btnWatchSet.SetCaption("����");

		Modual_SendReal(TRUE, "0000");

		AfxSetResourceHandle(hInstSave);		
	}

	m_nInitFlag++;
}

void CTrConVolView::DoDllCreate()
{
	if (m_pwndMainFrm != NULL)
	{
		m_pwndMainFrm->m_strScreenNo = m_strScreenNo;
		if(m_strSaveKey.GetLength()>0)
			m_pwndMainFrm->m_strSaveKey	 = m_strSaveKey;
		else
			m_pwndMainFrm->m_strSaveKey	 = m_strScreenNo;

		BOOL bSiseOnly = m_LinkDataManager.GetSiseOnly();		
		
		m_pwndMainFrm->SetInit(bSiseOnly);
		OnChangeColorTbl(0);

// 		ICtlBase* pCtrlBase = (ICtlBase*)m_pFormMngr->GetControlByName("cmbItem");
// 		HWND hControl = pCtrlBase->GetHwnd();
// 		m_pwndMainFrm->SetCodeInfo(&m_ctrCombo, hControl);
	}

	// �ε尡 ���� �Ŀ��� ����Ű�� ȣ���ȣ�� �ٽ� �����Ѵ�.
	if(m_pwndMainFrm) m_pwndMainFrm->m_strSaveKey	 = m_strScreenNo;
}

void CTrConVolView::SetInit_MainWnd()
{
	CString strData;
	strData = "jong";	// m_lbScreenType.GetCaption(); -hidden-
	BOOL bSiseOnly = m_LinkDataManager.GetSiseOnly();
	if (strData.IsEmpty() == FALSE)
	{
		strData.Replace("ScreenType=", "");

		BOOL bIndustry = FALSE;
		if (strData == "Industry")
			bIndustry = TRUE;
		OnCreate_MainWnd(bIndustry);
	}

	if(m_pwndMainFrm)
	{
		ICtlBase* pCtrlSelectBase = (ICtlBase*)m_pFormMngr->GetControlByName("ItemCode1");
		HWND hSelectControl = pCtrlSelectBase->GetHwnd();
		if(bSiseOnly)
			pCtrlSelectBase->ShowControl(!bSiseOnly);

		ICtlBase* pCtrlExceptBase = (ICtlBase*)m_pFormMngr->GetControlByName("ItemCode2");
		HWND hExceptControl = pCtrlExceptBase->GetHwnd();
		m_pwndMainFrm->SetCodeInfo(&m_ctrlSelectUJCombo, hSelectControl, &m_ctrlExceptJMCombo, hExceptControl);
		if(bSiseOnly)
			pCtrlSelectBase->ShowControl(!bSiseOnly);

		ICtlBase* pCtrlBtnAddBase = (ICtlBase*)m_pFormMngr->GetControlByName("btnAddEx");
		HWND hBtnAdd = pCtrlBtnAddBase->GetHwnd();
		if(bSiseOnly)
			pCtrlSelectBase->ShowControl(!bSiseOnly);

		ICtlBase* pCtrlBtnRemoveBase = (ICtlBase*)m_pFormMngr->GetControlByName("btnRemoveEx");
		HWND hBtnRemove = pCtrlBtnRemoveBase->GetHwnd();
		if(bSiseOnly)
			pCtrlSelectBase->ShowControl(!bSiseOnly);

		ICtlBase* pCtrlBtnRemoveAllBase = (ICtlBase*)m_pFormMngr->GetControlByName("btnRemoveAllEx");
		HWND hBtnRemoveAll = pCtrlBtnRemoveAllBase->GetHwnd();
		if(bSiseOnly)
			pCtrlSelectBase->ShowControl(!bSiseOnly);
		
		m_pwndMainFrm->SetBtnInfo(&m_btnAdd, hBtnAdd, &m_btnRemove, hBtnRemove, &m_btnRemoveAll, hBtnRemoveAll);
		
		SetCtrlVisible(FALSE);
	}
}


void CTrConVolView::OnCreate_MainWnd(BOOL bIndustry)
{
	CRect rcFrame;
	CWnd* pFrame = FromHandle(m_hWndFrame);
	pFrame->GetWindowRect(rcFrame);

	int nCXFrame = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXEDGE);
	/*pFrame->SetWindowPos(NULL, 0, 0, rcFrame.Width() + nCXFrame, rcFrame.Height() + nCXFrame,  SWP_NOMOVE);	*/
	
	
	int nWidth, nHeight;
	nWidth = rcFrame.Width() + nCXFrame/*800*/;
	nHeight = rcFrame.Height() + nCXFrame/*545*/;
	
	m_pwndMainFrm = new CMainFrmWnd;
	
	BOOL bResult = m_pwndMainFrm->Create(NULL, _T("Main Frame Window"), WS_CHILD | WS_CLIPSIBLINGS, CRect(0, 0, nWidth, nHeight), this, NULL);
	m_pwndMainFrm->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	
	if (bResult)
	{
		m_pwndMainFrm->ShowWindow(SW_SHOW);
		SetWindowPos(NULL, 0, 0, nWidth, nHeight, SWP_NOZORDER);
	}
}



void CTrConVolView::OnDestroy() 
{
	Modual_SendReal(FALSE, "0000");
	
	if(m_pwndMainFrm)
	{
		if(m_pwndMainFrm->GetSafeHwnd())
			m_pwndMainFrm->SendMessage(WM_DESTROY, 0, 0);
	/*	delete m_pwndMainFrm;*/
		m_pwndMainFrm = NULL;
	}

	CScrFormView::OnDestroy();

}

void CTrConVolView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	m_crBackColor = RGB(255, 255, 255);
	CBrush BackBrush(m_crBackColor);

	CRect rect;
	dc.GetClipBox(&rect);
	dc.FillRect(rect, &BackBrush);	


	BackBrush.DeleteObject();
	// Do not call CScrFormView::OnPaint() for painting messages
}

void CTrConVolView::OnBtnPopup()
{
/* -hidden-
	CPoint ptMenu;
	GetCursorPos(&ptMenu);

	HWND hWnd = (HWND)m_lRunForm.GetControlHwnd("btnPopup");
	if(hWnd)
	{
		CRect rect;
		CWnd* pWnd = CWnd::FromHandle(hWnd);
		pWnd->GetWindowRect(&rect);
		ptMenu.x = rect.left;
		ptMenu.y = rect.bottom;
	}

	CMenu	menu;
	menu.CreatePopupMenu();

	int nID=100;
	menu.AppendMenu(MF_STRING, nID++, "����ü�ᷮ ����"); 
	menu.AppendMenu(MF_STRING, nID++, "�˻����� ����"); 

	UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	int nRet = menu.TrackPopupMenu(flags, ptMenu.x, ptMenu.y, this);
	menu.DestroyMenu();


	switch(nRet) {
		case 100:
			{
				m_nOld_InsertType = m_pwndMainFrm->m_stConVolSet.nItemPosition;
				m_pwndMainFrm->FilterSet();
				int m_nCur_InsertType = m_pwndMainFrm->m_stConVolSet.nItemPosition;
				if(m_nOld_InsertType != m_nCur_InsertType) //�׸��� ���� ��� �����
					Change_GridInsertType(m_nCur_InsertType, m_nGridRow - 2);
				break;
			}
		case 101:
			m_pwndMainFrm->TargetSet();
			break;
	}
*/	
}

LRESULT CTrConVolView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
		case WM_INITMENUPOPUP:
		case WM_COMMAND:
			if(m_hChildFrame)
			{
				::PostMessage( m_hChildFrame, WM_COMMAND, wParam, 0);
				return TRUE;
			}
			else 
			{
			}
			//break;
			return TRUE;
/* -hidden-
		case WM_USER+105:	//TR����
			m_pFormWnd->SendMessage(WM_BEGIN_WAIT_CURSOR, 0, 0);
			//break;
			return TRUE;
		case WM_USER+106:	//TR����
			m_pFormWnd->SendMessage(WM_END_WAIT_CURSOR, 0, 0);
			//break;
			return TRUE;
*/
	}	
	return CScrFormView::WindowProc(message, wParam, lParam);
}

BOOL CTrConVolView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient, COLOR_MAIN_CANVAS);

	return TRUE;
}

void CTrConVolView::SetFrameMsgText(CString strMsg) 
{
//	m_lRunForm.SetFrameMsgText(strMsg);		-hidden-
}

// ������ ������ �ڵ��� ���Ѵ�.
CWnd* CTrConVolView::GetFrameWnd(CWnd*	pWnd)
{
	CWnd* pParentWnd = pWnd->GetParent();  // start with one parent up
	while (pParentWnd != NULL)
	{
		DWORD dwExStyle = GetWindowLong(pParentWnd->GetSafeHwnd(), GWL_STYLE);
		if ( dwExStyle & 0x8000)
		{
			return pParentWnd;
		}
		pParentWnd = pParentWnd->GetParent();
	}
	return NULL;
}

void CTrConVolView::SetSendMainMsg(CString strMsg)
{
//	m_lRunForm.SetFrameMsgText(strMsg);		-hidden-
}

void CTrConVolView::GetSignInfo(int& nSign, CString& strSign)//, int nSign, COLORREF &clrSignText)
{
	switch (nSign)
	{
	case 1:		// ����(ȭ��ǥ) : RED ������ WHITE TEXT
		strSign = "+";
		nSign = 88;
	//	clrSignText = RED;
		break;
	case 2:		// ���(�ﰢ��) : WHITE ������ RED TEXT
		strSign = "+";
		nSign = 88;
	//	clrSignText = RED;
		break;
	case 4:		// ����(ȭ��ǥ) : BLUE ������ WHITE TEXT
		strSign = "-";
		nSign = 89;
	//	clrSignText = BLUE;
		break;
	case 5:		// �϶�(�ﰢ��) : WHITE ������ BLUE TEXT
		strSign = "-";
		nSign = 89;
	//	clrSignText = BLUE;
		break;
	default:
		strSign = "";
		nSign = 112;
		break;
	}
}


void CTrConVolView::SetAddRow_CurrentPrice(StConVolClient	stConVolClient)
{
/* -hidden-
	char* pBuff;
	int nRow_Prev;

	int nLastRow = m_gridConVol.GetRowCount();

	
	if(m_pwndMainFrm->m_stConVolSet.nItemPosition == 0)
	{
		if(m_nGridRow == nLastRow)
			m_gridConVol.InsertRows(++nLastRow, 1);		

		if(m_listStConVol.GetCount() == m_nCurPriceRowCount - 1)
		{
			m_listStConVol.RemoveHead();
		}
		
		if(m_nGridRow == 102)//102
		{
			m_gridConVol.RemoveRows(m_nGridRow - 1, m_nGridRow -1 );
			m_gridConVol.InsertRows(2, 1);

			m_listStConVol.RemoveHead();
			
			nRow_Prev = m_nGridRow - 1;  //���� ȣ��� �ٽ� ������ ������ ����Ű�� ����
			m_nGridRow = 2;				//ù�ٿ� Insert
		}
		else
		{
			m_gridConVol.InsertRows(2, 1);
			nRow_Prev = m_nGridRow;
			m_nGridRow = 2;
			m_gridConVol.RemoveRows(nLastRow , nLastRow);
		}
		
	}
	else if(m_pwndMainFrm->m_stConVolSet.nItemPosition == 1)
	{
		if(m_nGridRow  ==  m_nCurPriceRowCount + 1) 
		{
			m_nGridRow = 2;
			m_gridConVol.ClearAllData(1, NULL);	
	
			m_gridConVol.SetRowCount(m_nCurPriceRowCount);
		}	
		nRow_Prev = m_nGridRow;
	}	


	int nData;
	CString strSign = "";
	INT_CONVERT(pBuff, stConVolClient.cSign, sizeof(stConVolClient.cSign), nData);
	GetSignInfo(nData, strSign);
	int nIndex_Color = nData;

	CString strData;
//	�ð�
	STR_CONVERT(pBuff, stConVolClient.sTime, sizeof(stConVolClient.sTime), strData);
	m_gridConVol.SetCellString(m_nGridRow, 1, strData, 0);
//	�����
	STR_CONVERT(pBuff, stConVolClient.sItemName, sizeof(stConVolClient.sItemName), strData);
	m_gridConVol.SetCellString(m_nGridRow, 2, strData, 0);
//	���簡
	STR_CONVERT(pBuff, stConVolClient.sPriceNow, sizeof(stConVolClient.sPriceNow), strData);
	strData.TrimLeft();
	m_gridConVol.SetCellString(m_nGridRow, 3, strSign + strData, 0);
	m_gridConVol.SetCellTextColor(m_nGridRow, 3, nIndex_Color, nIndex_Color, FALSE);
//	�����
	STR_CONVERT(pBuff, stConVolClient.sChgRate, sizeof(stConVolClient.sChgRate), strData);
	strData.TrimLeft();
	m_gridConVol.SetCellString(m_nGridRow, 4, strSign + strData, 0);
//�ż�, �ŵ� ����   +:�ż�   -:�ŵ�
	CString strPlusMinus;
	STR_CONVERT(pBuff, stConVolClient.chGubun, sizeof(stConVolClient.chGubun), strPlusMinus);
//ü�ᷮ
	STR_CONVERT(pBuff, stConVolClient.sConVol, sizeof(stConVolClient.sConVol), strData);
	strData.TrimLeft();
	strData = strPlusMinus + strData;
	m_gridConVol.SetCellString(m_nGridRow, 5, strData, 0);
//�����ڵ�
	STR_CONVERT(pBuff, stConVolClient.sCode, sizeof(stConVolClient.sCode), strData);
	m_gridConVol.SetCellString(m_nGridRow, 6, strData, 0);

	m_nGridRow = nRow_Prev + 1;

	if(m_pwndMainFrm)
		m_pwndMainFrm->PlaySound();
*/
}

// ������ ����ȭ�����.
void CTrConVolView::OnFormGetPageSave(LPCTSTR strFileName)
{
	m_strSaveKey.Format("_%s", strFileName);
	m_strSaveKey.TrimRight();
	if(m_pwndMainFrm)
	{
		CString strUserPath;
		IMainInfoManager* pMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager, 1);
		if(pMng)
		{
			strUserPath = pMng->GetEncUserDir();
		}
		CString strPath;
		strPath = strUserPath + "\\" + strFileName + "_" + CONVOL_FILE;

		// m_pwndMainFrm->m_strSaveKey	 = m_strScreenNo+m_strSaveKey;
		m_pwndMainFrm->DoSaveUserInfo(strPath);
	}
}

// ������ ����ȭ�����. : ȭ��ε��ÿ� �Ҹ�, ����ȭ�� �ε��ÿ��� �Ҹ�
void CTrConVolView::OnFormSetPageSave(LPCTSTR strFileName)
{
	m_strSaveKey.Format("%s", strFileName);
	m_strSaveKey.TrimRight();
	//if(m_pwndMainFrm) m_pwndMainFrm->m_strSaveKey	 = m_strScreenNo+m_strSaveKey;
	if(m_pwndMainFrm) m_pwndMainFrm->m_strSaveKey	 = m_strSaveKey;
}

//������ ��Ų����� �Ҹ���.
void CTrConVolView::OnColorChange()
{
/* -hidden-
	IKSLibManager*	pKSMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager);
	if(pKSMng)
		m_crBackColor = pKSMng->Lib_KSLink_GetColor(1);

	if(m_pwndMainFrm && m_pwndMainFrm->m_hWnd)
		m_pwndMainFrm->ChangeSkinColor();
*/
}

LRESULT CTrConVolView::OnGetSkinColor(WPARAM wParam, LPARAM lParam)
{
	int nColorType = (int)wParam;
	if(nColorType == 0)		// ����
		return m_crBackColor;	

	return RGB(255,255,255);
}

LRESULT CTrConVolView::OnLButtonClickedRowCol(short nRow, short nCol, short nSubPos, short nKeyIndex, LPCTSTR strItem, LPCTSTR strData)
{
/* -hidden-
	if(m_nScreenType!=1)	
	{
		CString strHeaderName = "�ڵ�";
		CString strName="";
		for(int nIndex = 1; nIndex <= m_gridConVol.GetColCount(); nIndex++)
		{
			strName = m_gridConVol.GetCellString(1, nIndex, 0);
			if(strcmp(strHeaderName, strName) == 0)
				break;
		}
		CString strCode="";
		strCode = m_gridConVol.GetCellString(nRow, nIndex, 0); //������� "�ڵ�"�� column�ε����� ã�� code����
		SetLinkCode(strCode);	
	}
*/
	return TRUE;
}

void CTrConVolView::OnClickbtnErase()
{
	m_pwndMainFrm->ClearAll();
}

void CTrConVolView::OnClickbtnFieldSet()
{
	m_pwndMainFrm->FieldSet();
	/*m_pwndMainFrm->OnBtnFieldEdit();*/
}

void CTrConVolView::OnClickbntConditionSet()
{
	if(m_pwndMainFrm)
		m_pwndMainFrm->ChangeConditionSet();
}

void CTrConVolView::OnClickbtnTargetItem()
{
	m_pwndMainFrm->TargetSet();
}

void CTrConVolView::OnClickbtnWatchSet()
{
	m_bWatching = !m_bWatching;

	if(m_bWatching)
	{
		m_btnWatchSet.SetForeColor(RGB(255, 0, 0));
		m_btnWatchSet.SetCaption("����");
	}
	else
	{
		m_btnWatchSet.SetForeColor(RGB(0, 0, 0));
		m_btnWatchSet.SetCaption("����");
	}

	Modual_SendReal(m_bWatching, "0000");
}

void CTrConVolView::Modual_SendReal(BOOL bFlag, CString strKey)
{
	if(m_LinkDataManager.GetSiseOnly())
		return;
	
	if(m_pwndMainFrm)
		m_pwndMainFrm->Modual_SendReal(bFlag, strKey);
}

void CTrConVolView::FrameResize(int nWidth, int nHeight)
{
	CRect rcFrame;
	CWnd* pFrame = FromHandle(m_hWndFrame);
	pFrame->GetWindowRect(rcFrame);
	
	int nCXFrame = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXEDGE);
	int nCYFrame = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYEDGE);

	if(nHeight != 0)
		pFrame->SetWindowPos(NULL, 0, 0, nWidth + nCXFrame, nHeight + nCYFrame,  SWP_NOMOVE);
	else
		pFrame->SetWindowPos(NULL, 0, 0, nWidth + nCXFrame, DEF_FRAMEHEIGHT,  SWP_NOMOVE);
}

void CTrConVolView::OnChangeColorTbl(int nTableIndex)
{
	COLORREF clrBkgnd =	CCtlColor::GetColor(2); 

	ST_TABCOLOR stTabColor;
	stTabColor.clrTabSelText	= CCtlColor::GetColor(12);   //�� Ȱ�� �ؽ�Ʈ
	stTabColor.clrTabUnSelText	= CCtlColor::GetColor(14);   //�� ��Ȱ�� �ؽ�Ʈ
	stTabColor.clrTabBody		= CCtlColor::GetColor(10);   //�� �ٵ� ���
	stTabColor.clrTabSelGraUp		= CCtlColor::GetColor(10);	//�� Ȱ�� ��� �׶��̼� ����
	stTabColor.clrTabSelGraDown		= CCtlColor::GetColor(10);	//�� Ȱ�� ��� �׶��̼� ��
	stTabColor.clrTabUnSelGraUp		= CCtlColor::GetColor(13);	//�� ��Ȱ�� ��� �׶��̼� ����
	stTabColor.clrTabUnSelGraDown	= CCtlColor::GetColor(13);	//�� ��Ȱ�� ��� �׶��̼� �� 

	m_pwndMainFrm->ChangeSkinColor(clrBkgnd, stTabColor);
	ST_GRIDCOLOR stGridColor;
	stGridColor.clrGridHeader	= CCtlColor::GetColor(21);   //�׸��� ������
	stGridColor.clrGridHText	= CCtlColor::GetColor(143);   //�׸��� ��� �ؽ�Ʈ
	stGridColor.clrGridHilight	= CCtlColor::GetColor(29);   //�׸��� ���̶���Ʈ
	stGridColor.clrGridSBk		= CCtlColor::GetColor(22);   //�׸��� �����
	stGridColor.clrGridSText	= CCtlColor::GetColor(142);   //�׸��� �� �ؽ�Ʈ

	m_pwndMainFrm->ChangeGridColor(stGridColor);
}

void CTrConVolView::EditFullSelectMarketCode()
{

}

void CTrConVolView::EditEnterSelectMarketCode()
{
	
}

void CTrConVolView::EditFullExceptItemCode()
{
	m_arItemData.RemoveAll();
	CWrpForm* pForm = GetFormCtl();

	if(strcmp(m_ctrlExceptJMCombo.GetCaption(), "") == 0) return;
	if(m_ctrlExceptJMCombo.GetVisible() == FALSE)   return;
	
	m_arItemData.Add(m_ctrlExceptJMCombo.GetCaption());
	m_arItemData.Add(m_ctrlExceptJMCombo.GetItemName(m_arItemData.GetAt(0)));
}	

void CTrConVolView::EditEnterExceptItemCode()
{
	
}


LRESULT CTrConVolView::OnRmsgKSMoveCtrl(WPARAM wParam, LPARAM lParam)
{
	BOOL bVisible;
	bVisible = (BOOL)wParam;
	
	SetCtrlVisible(bVisible);

	return TRUE;
}

void CTrConVolView::SetCtrlVisible(BOOL bVisible)
{	
	m_ctrlSelectUJCombo.SetVisibleForecastPriceCol(bVisible);
	m_ctrlExceptJMCombo.SetVisibleForecastPriceCol(bVisible);

	m_btnAdd.SetVisibleForecastPriceCol(bVisible);
	m_btnRemove.SetVisibleForecastPriceCol(bVisible);
	m_btnRemoveAll.SetVisibleForecastPriceCol(bVisible);

	Invalidate();

	if(!bVisible)
		return;
}

void CTrConVolView::OnClickBtnAdd()
{
	if(m_arItemData.GetCount() != 2)
		return;

	if(m_pwndMainFrm)
		m_pwndMainFrm->OnBtnAdd(m_arItemData.GetAt(0), m_arItemData.GetAt(1));
}

void CTrConVolView::OnClickBtnRemove()
{
	if(m_pwndMainFrm)
		m_pwndMainFrm->OnBtnRemove();
}

void CTrConVolView::OnClickBtnRemoveAll()
{
	m_pwndMainFrm->OnBtnRemoveAll();
}