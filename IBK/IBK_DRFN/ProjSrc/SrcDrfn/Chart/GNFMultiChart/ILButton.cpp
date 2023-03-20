// ILButton.cpp : implementation file
//

#include "stdafx.h"
#include "ILButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CILButton

#define UMSG_SIGNAL_CLICK		WM_USER+182 //05.09.09  ��ȣ�� Click

CILButton::CILButton()
{
	m_pImageList = NULL;
	m_bPushLike  = FALSE;
	m_nNormal    = -1;
	m_nDown		 = -1;
	m_nDisable	 = -1;
	m_nOver		 = -1;
	m_bButtonDown= FALSE;
	m_bCheck	 = FALSE;
	m_bLbuttonDown= FALSE;
	m_bUseToolTip = TRUE;
	m_bUseDrawTitle = FALSE;
	
}

CILButton::~CILButton()
{
	if (m_bUseToolTip)
	{
		if (m_Tooltip.GetSafeHwnd())
		{
			//m_Tooltip.DestroyToolTipCtrl();	
			m_Tooltip.DestroyWindow();
		}
	}
}


BEGIN_MESSAGE_MAP(CILButton, CButton)
	//{{AFX_MSG_MAP(CILButton)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_NCLBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CILButton message handlers

void CILButton::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}

/*==========================================================================================
	Author		 : �� �� ��   Date : 2002-09-24 Time : ���� 11:16:00
 ==========================================================================================
	Function	 : CILButton::FP_vSetImage
	Return		 : void - 
	Parameters	 : 
			CImageList* pImageList - �̹�������Ʈ�� ������
			int nNormal - Normal ������ �̹��� �ε���
			int nDown - Down ������ �̹��� �ε���
			int nDisable - Disable ������ �̹��� �ε���
			int nOver - Over ������ �̹��� �ε���
			BOOL bPushLike - PushLike �ι�ư�� ������� ����
	Note		 : �̹�������Ʈ�� �̹��� ����Ʈ���� ��ư�� �̹����� ����� �̹����� �ε�����
					�޾Ƽ� ��������� ������ ���´�.
 ==========================================================================================*/
void CILButton::FP_vSetImage(CImageList* pImageList	,							 
							 int nNormal			,
							 int nDown				,
							 BOOL bDrawTitle		,							 
							 CString strTitle		,
							 BOOL bPushLike			,
							 int nDisable			,
							 int nOver											 
							 )
{
	m_pImageList = pImageList;
	m_bPushLike  = bPushLike;
	m_bUseDrawTitle = bDrawTitle;

	if(m_bUseDrawTitle)
		m_szDrawTitle = strTitle;

	m_nNormal = nNormal;					// Normal ������ �̹��� �ε���
	if(nDown == -1)							// Down ������ �̹��� �ε���
	{m_nDown = nNormal;}
	else
	{m_nDown = nDown;}

	if(nDisable == -1)						// Disable ������ �̹��� �ε���
	{m_nDisable = nNormal;}
	else
	{m_nDisable = nDisable;}

	if(nOver == -1)							// Over ������ �̹��� �ε���
	{m_nOver = nNormal;}
	else
	{m_nOver = nOver;}
}

void CILButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
   POINT mouse_position;
    if ((m_bButtonDown) && (::GetCapture() == m_hWnd) && (::GetCursorPos(&mouse_position)))
	{
		if (::WindowFromPoint(mouse_position) == m_hWnd)
		{
			if ((GetState() & BST_PUSHED) != BST_PUSHED) 
			{
				SetState(TRUE);
				return;
			}
		} else {
			if ((GetState() & BST_PUSHED) == BST_PUSHED) {
				SetState(FALSE);
				return;
			}
		}
	}	
	
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);	// DC �� ���´�.


	RECT r=lpDrawItemStruct->rcItem;					// Rect �� ���´�.
	CRect rc = r;

	pDC->SetBkMode(TRANSPARENT);						// �ؽ�Ʈ�� ������ �����ϰ�

	if(m_pImageList == NULL)
		return;
	
	if (lpDrawItemStruct->itemState & ODS_DISABLED)		// ��Ȱ�� ����
	{
		m_pImageList->Draw(pDC,m_nDisable, CPoint(0,0), ILD_TRANSPARENT);
	}
	else
	{
		if ((lpDrawItemStruct->itemState & ODS_SELECTED) || (m_bPushLike && m_bCheck))// ��ư�ٿ����
		{
			m_pImageList->Draw(pDC,m_nDown, CPoint(0,0), ILD_TRANSPARENT);
		}
		else																// �������
		{
			m_pImageList->Draw(pDC,m_nNormal, CPoint(0,0), ILD_TRANSPARENT);
		}
	}

	if (m_bUseDrawTitle)
	{
		CFont *pFont = CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
		CFont* pOldFont=pDC->SelectObject(pFont);			// get text font

		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		CRect TextRect = lpDrawItemStruct->rcItem;
		if (lpDrawItemStruct->itemState & ODS_DISABLED)		// ��Ȱ�� ����
			pDC->SetTextColor(RGB(172,168,153));
		else
			//<<20100422_JS.Kim �ַθ� ��ư ���ڻ��� ����
			//pDC->SetTextColor(RGB(0,0,0));
			pDC->SetTextColor(RGB(255,255,255));
			//>>
		pDC->DrawText(m_szDrawTitle,TextRect,DT_CENTER|DT_SINGLELINE|DT_VCENTER);

		pDC->SelectObject(pOldFont);
	}
}


BOOL CILButton::OnEraseBkgnd(CDC* pDC) 
{
	//return FALSE;	
	//return CButton::OnEraseBkgnd(pDC);
	return TRUE;
}

/*==========================================================================================
	Author		 : �� �� ��   Date : 2002-09-24 Time : ���� 12:40:50
 ==========================================================================================
	Function	 : CILButton::SetCheck
	Return		 : void - 
	Parameters	 : 
			BOOL bCheck - CheckState ����
	Note		 : CheckState �� �޾Ƽ� �����Ѵ�.
 ==========================================================================================*/
void CILButton::SetCheck(BOOL bCheck)
{
	m_bCheck = bCheck;
	Invalidate();
}

/*==========================================================================================
	Author		 : �� �� ��   Date : 2002-09-24 Time : ���� 12:45:43
 ==========================================================================================
	Function	 : CILButton::GetCheck
	Return		 : BOOL - 
	Parameters	 : 
	Note		 : CheckState �� �����Ѵ�.
 ==========================================================================================*/
BOOL CILButton::GetCheck()
{
	return m_bCheck;
}

void CILButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLbuttonDown = TRUE;
//	SetCapture();
	CButton::OnLButtonDown(nFlags, point);
}

void CILButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
//	if (::GetCapture() == m_hWnd)
//		ReleaseCapture();
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point))
	{
		if(m_bLbuttonDown && m_bPushLike)
		{
			m_bCheck = !m_bCheck;
		}
	}
	m_bLbuttonDown = FALSE;
	
	//GetParent()->SendMessage(UMSG_SIGNAL_CLICK);

	CButton::OnLButtonUp(nFlags, point);
}

void CILButton::FP_vSetToolTipText(CString s)
{
	m_bUseToolTip = TRUE;
	if( ! m_Tooltip.GetSafeHwnd() )
	{
		if( m_Tooltip.Create(this))
		{
				
			if(m_Tooltip.AddTool(this, (LPCTSTR)s))
			{
				m_Tooltip.Activate(1);
			}
		}
	}
	else
	{		
		m_Tooltip.UpdateTipText((LPCTSTR)s,this);
	}
	

//	if (m_pTooltip == NULL)
//		m_pTooltip = new CToolTipCtrl;
//	if(m_pTooltip->m_hWnd==NULL)
//	{
//		if(m_pTooltip->Create(this))
//		{
//			if(m_pTooltip->AddTool(this, (LPCTSTR)s))
//			{
//				m_pTooltip->Activate(1);
//			}
//		}
//	}
//	else
//	{
//		m_pTooltip->UpdateTipText((LPCTSTR)s,this);
//	}
}

void CILButton::RelayEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(m_Tooltip.GetSafeHwnd())
	{
		MSG msg;
		msg.hwnd = m_hWnd;
		msg.message = message;
		msg.wParam = wParam;
		msg.lParam = lParam;
		msg.time = 0;
		msg.pt.x = LOWORD(lParam);
		msg.pt.y = HIWORD(lParam);
		m_Tooltip.RelayEvent(&msg);

		m_Tooltip.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOOWNERZORDER);
	}
//	if(m_pTooltip && NULL != m_pTooltip->m_hWnd)
//	{
//		MSG msg;
//		msg.hwnd = m_hWnd;
//		msg.message = message;
//		msg.wParam = wParam;
//		msg.lParam = lParam;
//		msg.time = 0;
//		msg.pt.x = LOWORD(lParam);
//		msg.pt.y = HIWORD(lParam);
//		m_pTooltip->RelayEvent(&msg);
//	}

}

void CILButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bUseToolTip)
		RelayEvent(WM_MOUSEMOVE,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));	
	CButton::OnMouseMove(nFlags, point);
}

void CILButton::OnNcLButtonUp(UINT nFlags, CPoint point)
{
//	if (::GetCapture() == m_hWnd)
//		ReleaseCapture();
//	m_bLbuttonDown = FALSE;
//	m_bLbuttonDown = FALSE;

	CButton::OnNcLButtonUp(nFlags, point);
}

int CILButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CILButton::SetBtnTitle(CString szDrawTitle)
{
	m_szDrawTitle = szDrawTitle;
	Invalidate();
}

CString CILButton::GetBtnTitle()
{
	return m_szDrawTitle;
}
//DEL void CILButton::OnDestroy() 
//DEL {
//DEL 	
//DEL 	CButton::OnDestroy();
//DEL }
