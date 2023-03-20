// ScrTest002View.cpp : implementation file
//

#include "stdafx.h"
#include "PortAnalystView.h"
#include "AnalystMainFrmWnd.h"
#include "../../ITGEN/CommonSrc/CtlColor.h"
#include "../../ChartDll/Include_Chart/Dll_Load/IMetaTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CPortAnalystView

IMPLEMENT_DYNCREATE(CPortAnalystView, CScrFormView)

CPortAnalystView::CPortAnalystView()
	: CScrFormView(CPortAnalystView::IDD)
{
	//{{AFX_DATA_INIT(CPortAnalystView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pwndMainFrm = NULL;
}

CPortAnalystView::~CPortAnalystView()
{
}	

void CPortAnalystView::DoDataExchange(CDataExchange* pDX)
{
	CScrFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortAnalystView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPortAnalystView, CScrFormView)
	//{{AFX_MSG_MAP(CPortAnalystView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_MOVECTRL, OnRmsgMoveCtrl)
	ON_REGISTERED_MESSAGE(RMSG_PF_CONTROL_EVENT, OnPlatformControlEvent)
END_MESSAGE_MAP()

// ��ȭ�� ��Ʈ�ѷκ��� �ް� ���� �̺�Ʈ�� ����ϼ���.
// ��: ON_EVENT(CPortAnalystView, 3780,	CWrpButton::eidClick,	OnClickBtn, VTS_NONE)
BEGIN_EVENTSINK_MAP(CPortAnalystView, CScrFormView)
	ON_EVENT(CPortAnalystView, CTLID_FORM,	CWrpForm::eventidOnFormAfterInit,		OnFormAfterInit,	VTS_NONE)
	ON_EVENT(CPortAnalystView, CTLID_FORM,	CWrpForm::eventidOnFormBeforeInit,		OnFormBeforeInit,	VTS_NONE)
	ON_EVENT(CPortAnalystView, CTLID_TRANDM,	CWrpTranDM::eventidReceiveData,		OnReceiveTranData,	VTS_BSTR)
	ON_EVENT(CPortAnalystView, CTLID_TRANDM,	CWrpTranDM::eventidReceiveComplete,	OnReceiveTranCompleteData, VTS_BSTR)
	ON_EVENT(CPortAnalystView, CTLID_REALDM,	CWrpRealDM::eventidReceive,			OnReceiveRealData,	VTS_BSTR)
	ON_EVENT(CPortAnalystView, CTLID_REALDM,	CWrpRealDM::eventidReceiveComplete,	OnReceiveRealCompleteData, VTS_BSTR)
	ON_EVENT(CPortAnalystView, 3775, CWrpButton::eventidOnClick, OnClickbtnTotal, VTS_NONE)
	ON_EVENT(CPortAnalystView, 3780, CWrpButton::eventidOnClick, OnClickbtnKospi, VTS_NONE)
	ON_EVENT(CPortAnalystView, 3785, CWrpButton::eventidOnClick, OnClickbtnKosdaq, VTS_NONE)
	ON_EVENT(CPortAnalystView, 3790, CWrpButton::eventidOnClick, OnClickbtnSearch, VTS_NONE)
	ON_EVENT(CPortAnalystView, 3795, CWrpButton::eventidOnClick, OnClickbtnSend, VTS_NONE)
	ON_EVENT(CPortAnalystView, 3800, CWrpCombo::eventidOnListSelChanged, OnSelChangebtnComboTime, VTS_I2)
	ON_EVENT(CPortAnalystView, 3805, CWrpCalendar::eventidOnSelchanged , OnSelChangebtnCalendar, VTS_BSTR)	
	ON_EVENT(CPortAnalystView, CTLID_FORM,	CWrpForm::eventidOnChangeColorTbl,		OnChangeColorTbl,		VTS_I2)	
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortAnalystView diagnostics

#ifdef _DEBUG
void CPortAnalystView::AssertValid() const
{
	CScrFormView::AssertValid();
}

void CPortAnalystView::Dump(CDumpContext& dc) const
{
	CScrFormView::Dump(dc);
}
#endif //_DEBUG

void CPortAnalystView::OnCreate_MainWnd()
{
	int nWidth, nHeight;
	nWidth = 800;
	nHeight = 485;
	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	g_iMetaTable.LoadMetaTable();
	m_LinkDataManager.Initialize();
	g_iMetaTable.SetHTSID(atoi(m_LinkDataManager.GetMediaType()));

	m_pwndMainFrm = new CAnalystMainFrmWnd;
	BOOL bResult = m_pwndMainFrm->Create(NULL, _T("Main Frame Window"), WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CRect(0, 0, nWidth, nHeight), this, NULL);
	m_pwndMainFrm->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	
	AfxSetResourceHandle(hInstSave);
	
	if (bResult)
	{
		m_pwndMainFrm->ShowWindow(SW_SHOW);
		SetWindowPos(NULL, 0, 0, nWidth, nHeight, SWP_NOZORDER);
	}
}

void CPortAnalystView::SetInit_MainWnd()
{
	OnCreate_MainWnd();

	if (m_pwndMainFrm != NULL)
	{
		CString strOpenMode;
		CWrpForm* pForm = GetFormCtl();
		if(pForm)
			strOpenMode = pForm->GetOpenMapData();
		
		if(strOpenMode == "")
			strOpenMode = "0";

		m_pwndMainFrm->SetInit(strOpenMode);
		OnChangeColorTbl(0);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CPortAnalystView message handlers

// ��Ʈ�ѿ� ���� WrapŬ���� ������ �����Ѵ�.
void CPortAnalystView::OnInitialControl() 
{
	AttachCtrl(&m_ctrlTotalBtn, "btnTotal");
	AttachCtrl(&m_ctrlKospiBtn, "btnKospi");
	AttachCtrl(&m_ctrlKosdaqBtn, "btnKosdaq");
	AttachCtrl(&m_ctrlSearchBtn, "btnSearch");
	AttachCtrl(&m_ctrlCalendar, "btnCalendar");
	AttachCtrl(&m_ctrlComboTime, "btnComboTime");
	AttachCtrl(&m_ctrlSendBtn,    "btnSend");
	

	m_MapControlName["btnTotal"]  = (CObject*)&m_ctrlTotalBtn;
	m_MapControlName["btnKospi"]  = (CObject*)&m_ctrlKospiBtn;
	m_MapControlName["btnKosdaq"] = (CObject*)&m_ctrlKosdaqBtn;
	m_MapControlName["btnSearch"] = (CObject*)&m_ctrlSearchBtn;
	m_MapControlName["btnCalendar"] = (CObject*)&m_ctrlCalendar;
	m_MapControlName["btnComboTime"] = (CObject*)&m_ctrlComboTime;
	m_MapControlName["btnSend"] =	(CObject*)&m_ctrlSendBtn;
}

// ȭ�鿡 ���� �ʱ�ȭ�� �Ͼ�� ���� ���´�.
// DLL�� ����/Ŭ���� ���� �ʱⰪ �� ��ȭ���� ������ 
// �����νð� �ƴ� ���ǿ� ���� �ʱ� �ε� ������ ������ �Ѵ�.
void CPortAnalystView::OnFormBeforeInit() 
{
	CWrpForm* pForm = GetFormCtl();
	if(pForm)
		pForm->LockFontResize(TRUE);
}

// ��Ʈ�ѿ� �ʱ�Ӽ��� ���� ������, �������� �����Ǳ� ��, ��ġȭ�� �������� �ε�Ǳ� ���� �̰��� ��� �´�.
// ȭ��� ��Ʈ�ѿ� ���� ���� �ʱ�ȭ �� �ʱⰪ ������ �Ѵ�.
void CPortAnalystView::OnInitialUpdate() 
{
	CScrFormView::OnInitialUpdate();

	m_crBackColor = RGB(255, 255, 255);

	SetInit_MainWnd();
}

// ȭ�鿡 ���� ��� �ʱ�ȭ �� ������������, ��ġ�ϸ� �������� �ε�, �ʱ� �̺�Ʈ TR ȣ���� ������ ��� �´�. 
void CPortAnalystView::OnFormAfterInit() 
{
}

// ��ȸ �����Ϳ� ���� ������ ���� ������ ȣ��ȴ�.
// �����Ϳ� ����� ��Ʈ�ѿ� ������Ʈ �ϱ� ���� ���´�.
void CPortAnalystView::OnReceiveTranData(LPCTSTR szTranID)
{
}

// ��ȸ �����Ϳ� ���� ������ ���� ������ ȣ��ȴ�.
// �����Ϳ� ����� ��Ʈ�ѿ� ������Ʈ �� �Ŀ� ���´�.
void CPortAnalystView::OnReceiveTranCompleteData(LPCTSTR szTranID)
{
}

// ����� �ǽð� �����Ͱ� ���������� ȣ��ȴ�.
// �����Ϳ� ����� ��Ʈ�ѿ� ������Ʈ �ϱ� ���� ���´�.
void CPortAnalystView::OnReceiveRealData(LPCTSTR szTranID)
{
}

// ����� �ǽð� �����Ͱ� ���������� ȣ��ȴ�.
// �����Ϳ� ����� ��Ʈ�ѿ� ������Ʈ �� �Ŀ� ���´�.
void CPortAnalystView::OnReceiveRealCompleteData(LPCTSTR szTranID)
{
}

void CPortAnalystView::OnSize(UINT nType, int cx, int cy) 
{
	CScrFormView::OnSize(nType, cx, cy);
	
	if (m_pwndMainFrm != NULL)
	{
		CRect reClient;
		GetClientRect(&reClient);
		m_pwndMainFrm->MoveWindow(&reClient);
	}
}


void CPortAnalystView::DoControlMove(LPCSTR szCtrlName, CWnd* pWnd)
{
	if(!pWnd || !pWnd->GetSafeHwnd()) return;
	
	CRect ctlrect;
	pWnd->GetWindowRect(&ctlrect);	
	ScreenToClient(ctlrect);
	pWnd->ShowWindow(SW_HIDE);
	
	CWrpButton* pBtnObject;
	if( m_MapControlName.Lookup(szCtrlName, (CObject*&)pBtnObject))
	{
		pBtnObject->SetLeft(ctlrect.left);
		pBtnObject->SetTop(ctlrect.top);
		pBtnObject->SetWidth(ctlrect.Width());
		pBtnObject->SetHeight(ctlrect.Height());
	}
}

LRESULT CPortAnalystView::OnRmsgMoveCtrl(WPARAM wParam, LPARAM lParam)
{
	
	ST_MOVEITEM* pItem= (ST_MOVEITEM*)lParam;
	
	if(pItem == NULL) return 0L;
	
	switch(pItem->nType)
	{
	case PF_ITEM_BTN:
		OnPlatFormBtn(pItem, (int)wParam);
		DoPlatFormBtnMove(pItem->szCtrlName, pItem->pWnd);
		break;
	case PF_ITEM_CHECKBTN:
		OnPlatFormCheckBtn(pItem, (int)wParam);
		DoPlatFormCheckBtnMove(pItem->szCtrlName, pItem->pWnd);
		break;
	case PF_ITEM_CALENDAR:
		OnPlatFormCalendar(pItem, (int)wParam);
		DoPlatFormCalendarMove(pItem->szCtrlName, pItem->pWnd);
		break;
	case PF_ITEM_COMBOBOX:
		OnPlatFormComboBtn(pItem, (int)wParam);
		DoPlatFormComboBtnMove(pItem->szCtrlName, pItem->pWnd);
		break;
	default:
		OnPlatFormBtn(pItem, (int)wParam);
		DoPlatFormBtnMove(pItem->szCtrlName, pItem->pWnd);
		break;
	}
	
	return 1L;
}

//�÷��� ��ư Show/Hide
void CPortAnalystView::OnPlatFormBtn(ST_MOVEITEM* pItem, int nFlag)
{
	BOOL bShow = (nFlag == PF_CONTROL_HIDE ? FALSE : TRUE);  //PF_CONTROL_SHOW = 1 , PF_CONTROL_HIDE = 2
	CWnd* pWnd = pItem->pWnd;
	
	if(!pWnd || !pWnd->GetSafeHwnd()) return ;
	pItem->bEnable = pWnd->IsWindowEnabled();
	BOOL bEnable = pItem->bEnable;
	
	CWrpButton* pBtnObject;
	if( m_MapControlName.Lookup(pItem->szCtrlName, (CObject*&)pBtnObject) )
	{
		pBtnObject->SetVisible(bShow);
		pBtnObject->SetEnabled(bEnable);
	}	
	
}

//�÷��� üũ�ڽ���ư Show/Hide
void CPortAnalystView::OnPlatFormCheckBtn(ST_MOVEITEM* pItem, int nFlag)
{
	BOOL bShow = (nFlag == PF_CONTROL_HIDE ? FALSE : TRUE);  //PF_CONTROL_SHOW = 1 , PF_CONTROL_HIDE = 2
	CWnd* pWnd = pItem->pWnd;
	
	if(!pWnd || !pWnd->GetSafeHwnd()) return ;
	pItem->bEnable = pWnd->IsWindowEnabled();
	BOOL bEnable = pItem->bEnable;
	
	CWrpCheck* pBtnObject;
	if( m_MapControlName.Lookup(pItem->szCtrlName, (CObject*&)pBtnObject) )
	{
		pBtnObject->SetVisible(bShow);	
		pBtnObject->SetEnabled(bEnable);
	}	
}

//�÷��� ��ư Show/Hide
void CPortAnalystView::OnPlatFormCalendar(ST_MOVEITEM* pItem, int nFlag)
{
	BOOL bShow = (nFlag == PF_CONTROL_HIDE ? FALSE : TRUE);  //PF_CONTROL_SHOW = 1 , PF_CONTROL_HIDE = 2
	CWnd* pWnd = pItem->pWnd;
	
	if(!pWnd || !pWnd->GetSafeHwnd()) return ;
	pItem->bEnable = pWnd->IsWindowEnabled();
	BOOL bEnable = pItem->bEnable;
	
	CWrpCalendar* pBtnObject;
	if( m_MapControlName.Lookup(pItem->szCtrlName, (CObject*&)pBtnObject) )
	{
		pBtnObject->SetVisible(bShow);
		pBtnObject->SetEnabled(bEnable);
	}	
}

//�÷��� ��ư Show/Hide
void CPortAnalystView::OnPlatFormComboBtn(ST_MOVEITEM* pItem, int nFlag)
{
	BOOL bShow = (nFlag == PF_CONTROL_HIDE ? FALSE : TRUE);  //PF_CONTROL_SHOW = 1 , PF_CONTROL_HIDE = 2
	CWnd* pWnd = pItem->pWnd;
	
	if(!pWnd || !pWnd->GetSafeHwnd()) return ;
	pItem->bEnable = pWnd->IsWindowEnabled();
	BOOL bEnable = pItem->bEnable;
	
	CWrpCombo* pBtnObject;
	if( m_MapControlName.Lookup(pItem->szCtrlName, (CObject*&)pBtnObject) )
	{
		pBtnObject->SetVisible(bShow);
		pBtnObject->SetEnabled(bEnable);
	}	
}


void CPortAnalystView::DoPlatFormBtnMove(LPCSTR szCtrlName, CWnd* pWnd)
{	
	CRect ctlrect;
	pWnd->GetWindowRect(&ctlrect);	
	ScreenToClient(ctlrect);
	pWnd->ShowWindow(SW_HIDE);
	
	CWrpButton* pBtnObject;
	if( m_MapControlName.Lookup(szCtrlName, (CObject*&)pBtnObject))
	{
		pBtnObject->SetLeft(ctlrect.left);
		pBtnObject->SetTop(ctlrect.top);
		pBtnObject->SetWidth(ctlrect.Width());
		pBtnObject->SetHeight(ctlrect.Height());
	}
}

//�÷��� üũ�ڽ���ư��Ʈ�� �̵� 
void CPortAnalystView::DoPlatFormCheckBtnMove(LPCSTR szCtrlName, CWnd* pWnd)
{	
	CRect ctlrect;
	pWnd->GetWindowRect(&ctlrect);	
	ScreenToClient(ctlrect);
	pWnd->ShowWindow(SW_HIDE);
	
	CWrpCheck* pBtnObject;
	if( m_MapControlName.Lookup(szCtrlName, (CObject*&)pBtnObject))
	{
		pBtnObject->SetLeft(ctlrect.left);
		pBtnObject->SetTop(ctlrect.top);
		pBtnObject->SetWidth(ctlrect.Width());
		pBtnObject->SetHeight(ctlrect.Height());
	}
}

//�÷��� üũ�ڽ���ư��Ʈ�� �̵� 
void CPortAnalystView::DoPlatFormCalendarMove(LPCSTR szCtrlName, CWnd* pWnd)
{	
	CRect ctlrect;
	pWnd->GetWindowRect(&ctlrect);	
	ScreenToClient(ctlrect);
	pWnd->ShowWindow(SW_HIDE);
	
	CWrpCalendar* pBtnObject;
	if( m_MapControlName.Lookup(szCtrlName, (CObject*&)pBtnObject))
	{
		pBtnObject->SetLeft(ctlrect.left);
		pBtnObject->SetTop(ctlrect.top);
		pBtnObject->SetWidth(ctlrect.Width());
		pBtnObject->SetHeight(ctlrect.Height());
	}
}

//�÷��� üũ�ڽ���ư��Ʈ�� �̵� 
void CPortAnalystView::DoPlatFormComboBtnMove(LPCSTR szCtrlName, CWnd* pWnd)
{	
	CRect ctlrect;
	pWnd->GetWindowRect(&ctlrect);	
	ScreenToClient(ctlrect);
	pWnd->ShowWindow(SW_HIDE);
	
	CWrpCombo* pBtnObject;
	if( m_MapControlName.Lookup(szCtrlName, (CObject*&)pBtnObject))
	{
		pBtnObject->SetLeft(ctlrect.left);
		pBtnObject->SetTop(ctlrect.top);
		pBtnObject->SetWidth(ctlrect.Width());
		pBtnObject->SetHeight(ctlrect.Height());
	}
}

//DLL -> �÷��� ��Ʈ�� �̺�Ʈ
LRESULT CPortAnalystView::OnPlatformControlEvent(WPARAM wParam, LPARAM lParam)
{
	ST_PF_CONTROL_EVENT* pfEvent= (ST_PF_CONTROL_EVENT*)lParam;
	
	if(pfEvent == NULL)
		return 0L;	
	
	switch(pfEvent->nControlType)
	{
	case PF_ITEM_BTN:
		PlatForm_BtnEvent(pfEvent);
		break;
	case PF_ITEM_RADIOBTN:
		break;
	case PF_ITEM_CHECKBTN:
		PlatForm_CheckBtnEvent(pfEvent);
		break;
	case PF_ITEM_COMBOBOX:
		PlatForm_ComboBtnEvent(pfEvent);
		break;		
	case PF_ITEM_CALENDAR:
		PlatForm_CalendarEvent(pfEvent);
		break;
	default:
		break;
	}
	return 1L;	
}

//DLL -> �÷��� ��ư �̺�Ʈ 
void CPortAnalystView::PlatForm_BtnEvent(ST_PF_CONTROL_EVENT* pfEvent)
{
	CWrpButton* pBtnObject;
	BOOL bState = pfEvent->bState;
	CString strText;
	if( m_MapControlName.Lookup(pfEvent->szCtrlName, (CObject*&)pBtnObject))
	{
		switch(pfEvent->nEventID)
		{
		case PF_EVENT_BTN_SETWINDOWTEXT:
			{
				strText = pfEvent->szData;	
				pBtnObject->SetCaption((LPCTSTR)strText);
				break;
			}	
		case PF_EVENT_BTN_SETTEXTCOLOR:
			{
				COLORREF clr = (COLORREF)pfEvent->nData;
				strText = pfEvent->szData;	
				pBtnObject->SetForeColor(clr);
				break;
			}
		default:
			break;	
		}			
	}	
}

//DLL -> �÷��� üũ�ڽ���ư �̺�Ʈ 
void CPortAnalystView::PlatForm_CheckBtnEvent(ST_PF_CONTROL_EVENT* pfEvent)
{
	CWrpCheck* pBtnObject;
	BOOL bState = pfEvent->bState;
	if( m_MapControlName.Lookup(pfEvent->szCtrlName, (CObject*&)pBtnObject))
	{
		pBtnObject->SetCheck(bState);	
	}	
}

//DLL -> �÷��� �޺��ڽ���ư �̺�Ʈ 
void CPortAnalystView::PlatForm_ComboBtnEvent(ST_PF_CONTROL_EVENT* pfEvent)
{
	CWrpCombo* pBtnObject;
	BOOL bState = pfEvent->bState;
	CString strComboText = "";
	if( m_MapControlName.Lookup(pfEvent->szCtrlName, (CObject*&)pBtnObject))
	{
		if(pfEvent->nEventID == PF_EVENT_COMBO_ADDLIST)
		{
			for(int nIndex = 0; nIndex < pfEvent->arrData.GetSize(); nIndex++)
				pBtnObject->AddRow(pfEvent->arrData.GetAt(nIndex));		
			//		pBtnObject->SetCaption("");
		}
		else if(pfEvent->nEventID == PF_EVENT_COMBO_ADDSTRING)
		{		
			pBtnObject->AddRow(pfEvent->szData);
		}
		else if(pfEvent->nEventID == PF_EVENT_COMBO_GETWINDOWTEXT)
		{		
			strComboText = pBtnObject->GetCaption();
			pfEvent->stReturn.strWindowText = strComboText;
			pBtnObject->InsertRow(strComboText, 0);
		}
		else if(pfEvent->nEventID == PF_EVENT_COMBO_SETCURSEL)
		{
			pBtnObject->SetCurSel(pfEvent->nData);
		}
		else if(pfEvent->nEventID == PF_EVENT_COMBO_GETCURSEL)
		{
			pfEvent->stReturn.nCurSel = pBtnObject->GetCurSel();			
		}
		else if(pfEvent->nEventID == PF_EVENT_COMBO_RESETCONTENTS)
		{
			pBtnObject->ResetContent();
			pBtnObject->SetCurSel(0);
		}	
	}	
}

//DLL -> �÷��� �޷¹�ư �̺�Ʈ 
void CPortAnalystView::PlatForm_CalendarEvent(ST_PF_CONTROL_EVENT* pfEvent)
{
	CWrpCalendar* pBtnObject;
	BOOL bState = pfEvent->bState;
	CString strText = "";
	CString strYear = "";CString strMon = "";CString strDay = "";
	if( m_MapControlName.Lookup(pfEvent->szCtrlName, (CObject*&)pBtnObject))
	{
		strText = pfEvent->szData;
		strYear = strText.Mid(0, 4);
		strMon  = strText.Mid(4, 2);
		strDay  = strText.Mid(6, 2);
		if(pfEvent->nEventID == PF_EVENT_CALENDAR_SETTIME)
		{
			pBtnObject->SetDate(strYear, strMon, strDay);
		}		
	}	
}

void CPortAnalystView::OnClickbtnTotal()
{
	if(m_pwndMainFrm) m_pwndMainFrm->PlatFormBtnTotal();
}

void CPortAnalystView::OnClickbtnKospi()
{
	if(m_pwndMainFrm) m_pwndMainFrm->PlatFormBtnKospi();
}

void CPortAnalystView::OnClickbtnKosdaq()
{	
	if(m_pwndMainFrm) m_pwndMainFrm->PlatFormBtnKosdaq();
}

void CPortAnalystView::OnClickbtnSearch()
{
	if(m_pwndMainFrm) m_pwndMainFrm->PlatFormBtnSearch();
}

void CPortAnalystView::OnClickbtnSend()
{
	if(m_pwndMainFrm) m_pwndMainFrm->PlatFormBtnSend();
}

void CPortAnalystView::OnSelChangebtnComboTime(int nIndex)
{
	if(m_pwndMainFrm) m_pwndMainFrm->PlatFormBtnComboTime(nIndex);
}

void CPortAnalystView::OnSelChangebtnCalendar(LPCTSTR szData)
{
	if(m_pwndMainFrm) m_pwndMainFrm->PlatFormBtnCalendar(szData);
}

void CPortAnalystView::OnChangeColorTbl(int nTableIndex)
{
	COLORREF clrBkgnd =			CCtlColor::GetColor(2); 
	
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

void CPortAnalystView::SetOpenScreen(CString strScreen, CString strCode)
{
	CWrpForm* pForm = GetFormCtl();
	
	if(pForm && strScreen != "")
	{
	//	PutLinkData("&STOCK_CODE", strCode);
		pForm->OpenScreenWithScreenNum(strScreen);
	}
}

void CPortAnalystView::OpenScreenData(CString strScreen, CString strCode)
{
	CWrpForm* pForm = GetFormCtl();
	if(pForm)
		pForm->OpenScreenData((LPCTSTR)strScreen, (LPCTSTR)strCode, (short)strCode.GetLength());
}

void CPortAnalystView::OnDBClick(LPCSTR strCode)
{
	CWrpForm* pForm = GetFormCtl();
	if(pForm) pForm->SendMsgToMainDblLinkScr("ST_DBL_LNK", strCode);
}

