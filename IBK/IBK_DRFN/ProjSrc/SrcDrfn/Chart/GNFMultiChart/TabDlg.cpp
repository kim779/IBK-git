// TabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TabDlg.h"
#include "./define/ChartMsgDef.h"
#include "LeftBarDlg.h"

//#include "../../../DataMgr/DBMgr.h" //05.09.27
//#include "../../../DrMLib/DrMiddleLib.h"//05.09.27

#include "StdDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabDlg dialog


CTabDlg::CTabDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CRscDlg(nIDTemplate , pParent)
{
	//{{AFX_DATA_INIT(CTabDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentDlg = NULL;
	m_pMultiChartWnd = NULL;
	m_pChartMng = NULL;
//	m_crBk = CRLEFTBARBACK;

	//m_pEzFinderLib = NULL;	

	m_pIL_W20H20 = NULL; //�� 20�� ���� 19�� ��ư �̹�������Ʈ
//@�������� m_pIL_W34H20 = NULL; //�� 34�� ���� 19�� ��ư �̹�������Ʈ
//@�������� m_pIL_W46H20 = NULL; //�� 46�� ���� 19�� ��ư �̹�������Ʈ
//@��������	m_pIL_W50H20 = NULL; //�� 50�� ���� 19�� ��ư �̹�������Ʈ
//@��������	m_pIL_W55H19 = NULL; //�� 55�� ���� 19�� ��ư �̹�������Ʈ
//@��������	m_pIL_W60H19 = NULL; //�� 60�� ���� 19�� ��ư �̹�������Ʈ
//@�������� m_pIL_W88H19 = NULL; //�� 60�� ���� 19�� ��ư �̹�������Ʈ
	
	m_pIL_W151H21 = NULL; //�� 151�� ���� 21�� ��ư �̹�������Ʈ
//@�������� m_pIL_W26H23 = NULL; //�� 26�� ���� 23�� ��ư �̹�������Ʈ
//@�������� m_pIL_W58H20 = NULL; //�� 58�� ���� 20�� ��ư �̹�������Ʈ
//@��������	m_pIL_W82H20 = NULL; //�� 82�� ���� 20�� ��ư �̹�������Ʈ
	m_pIL_W43H20 = NULL; //�� 43�� ���� 19�� ��ư �̹�������Ʈ
//@��������	m_pIL_W43H19 = NULL; //�� 43�� ���� 19�� ��ư �̹�������Ʈ
//@�������� m_pIL_W46H19 = NULL; //�� 46�� ���� 19�� ��ư �̹�������Ʈ
//@�������� m_pIL_W58H19 = NULL; //�� 58�� ���� 19�� ��ư �̹�������Ʈ
//@�������� m_pIL_W72H19 = NULL; //�� 72�� ���� 19�� ��ư �̹�������Ʈ
	m_pIL_W58H20 = NULL; //�� 58�� ���� 20�� ��ư �̹�������Ʈ
	m_pIL_W72H20 = NULL; //�� 72�� ���� 20�� ��ư �̹�������Ʈ
//@�������� m_pIL_W52H19 = NULL; //�� 52�� ���� 19�� ��ư �̹�������Ʈ
}

CTabDlg::~CTabDlg()
{
	
}

void CTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabDlg, CRscDlg)
	//{{AFX_MSG_MAP(CTabDlg)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabDlg message handlers

void CTabDlg::RecalcLayoutCtrl(CWnd& rWnd,int nID /*=0*/ )
{
	if(rWnd.GetSafeHwnd())
	{
		if(!nID)
		{
			nID = rWnd.GetDlgCtrlID();
		}
		CRect rect = GetRectOfCtrl(nID);
		rWnd.MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());
	}
}


BOOL CTabDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam == VK_HOME)
		return FALSE;
	
	if(::IsWindow(m_hWnd))
		return GetParent()->PreTranslateMessage(pMsg);
	else
		return CRscDlg::PreTranslateMessage(pMsg);
}
HBRUSH CTabDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CRscDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) {
	case CTLCOLOR_BTN:
	case CTLCOLOR_DLG:
		return m_brBkColor;
		break;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CTabDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	if( m_pParentDlg == NULL ) return TRUE;

	// ��Ų ����
	m_crBk = (COLORREF)m_pMultiChartWnd->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
//	m_crBk = ((CStdDialog*)m_pMultiChartWnd)->GetSkinColorManager()->GetColor(1);
	
	m_brBkColor.CreateSolidBrush(m_crBk);
	//m_pDBMgr = m_pParentDlg->GetpDBMgr();
	if(m_pMultiChartWnd)
	{
		CStdDialog* pMainDlg = (CStdDialog*)m_pMultiChartWnd;

		//m_pCommonBaseInfo = pMainDlg->GetpMainBaseInfo();
		//m_pCommonCodeMgr  =	pMainDlg->GetpMainCodeManager();

		m_pIL_W20H20 = &(pMainDlg->m_ImgList_W20H20); //�� 20�� ���� 19�� ��ư �̹�������Ʈ
//@��������	m_pIL_W34H20 = &(pMainDlg->m_ImgList_W34H20); //�� 34�� ���� 19�� ��ư �̹�������Ʈ
//@�������� m_pIL_W46H20 = &(pMainDlg->m_ImgList_W46H20); //�� 46�� ���� 19�� ��ư �̹�������Ʈ
//@��������	m_pIL_W50H20 = &(pMainDlg->m_ImgList_W50H20); //�� 50�� ���� 20�� ��ư �̹�������Ʈ
//@��������	m_pIL_W55H19 = &(pMainDlg->m_ImgList_W55H19); //�� 55�� ���� 19�� ��ư �̹�������Ʈ
//@��������	m_pIL_W60H19 = &(pMainDlg->m_ImgList_W60H19); //�� 60�� ���� 19�� ��ư �̹�������Ʈ
//@�������� m_pIL_W88H19 = &(pMainDlg->m_ImgList_W88H19); //�� 88�� ���� 19�� ��ư �̹�������Ʈ
		m_pIL_W151H21 = &(pMainDlg->m_ImgList_W151H21); //�� 151�� ���� 21�� ��ư �̹�������Ʈ
//@�������� m_pIL_W26H23 = &(pMainDlg->m_ImgList_W26H23); //�� 26�� ���� 23�� ��ư �̹�������Ʈ
//@�������� m_pIL_W58H20 = &(pMainDlg->m_ImgList_W58H20); //�� 58�� ���� 20�� ��ư �̹�������Ʈ
//@��������	m_pIL_W82H20 = &(pMainDlg->m_ImgList_W82H20); //�� 82�� ���� 20�� ��ư �̹�������Ʈ
		m_pIL_W43H20 = &(pMainDlg->m_ImgList_W43H20); //�� 43�� ���� 19�� ��ư �̹�������Ʈ
//@��������	m_pIL_W43H19 = &(pMainDlg->m_ImgList_W43H19); //�� 43�� ���� 19�� ��ư �̹�������Ʈ
//@��������	m_pIL_W46H19 = &(pMainDlg->m_ImgList_W46H19); //�� 46�� ���� 19�� ��ư �̹�������Ʈ
//@��������	m_pIL_W58H19 = &(pMainDlg->m_ImgList_W58H19); //�� 58�� ���� 19�� ��ư �̹�������Ʈ
//@��������	m_pIL_W72H19 = &(pMainDlg->m_ImgList_W72H19); //�� 72�� ���� 19�� ��ư �̹�������Ʈ
		m_pIL_W58H20 = &(pMainDlg->m_ImgList_W58H20); //�� 58�� ���� 20�� ��ư �̹�������Ʈ
		m_pIL_W72H20 = &(pMainDlg->m_ImgList_W72H20); //�� 72�� ���� 20�� ��ư �̹�������Ʈ
//@�������� 	m_pIL_W52H19 = &(pMainDlg->m_ImgList_W52H19); //�� 52�� ���� 19�� ��ư �̹�������Ʈ
	}
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabDlg::OnDestroy() 
{
	m_brBkColor.DeleteObject();
	CRscDlg::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CTabDlg::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CTabDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	memDC.FillSolidRect(rcClient, m_crBk);
	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDC, 0,0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}
