// ButtonEx.cpp : implementation file
//

#include "stdafx.h"
#include "ButtonEx.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "Definition.h"
#include "ToolBarDlg.h"
#include "ObjTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonEx

CButtonEx::CButtonEx()
{
	m_pParent = NULL;
	m_strOption1 = _T("");
	m_strOption2 = _T("");
	m_strParam = _T("");
	m_strToolTipText = _T("");
	m_bState = false;
}

CButtonEx::~CButtonEx()
{
}


BEGIN_MESSAGE_MAP(CButtonEx, CButton)
	//{{AFX_MSG_MAP(CButtonEx)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonEx message handlers


/*-----------------------------------------------------------------------------------------
 - Function    :  DrawItem
 - Created at  :  2004-11-08   13:59
 - Author      :  ������
 - Description :  ��Ʈ���̹������� �ش� ��ư���� �׸���.
				  (IMPORTANT: CToolOptionInfo�� Enum���� ��ġ�� �ٲ�� ����! �ȵ�. ��ư�� ��ġ��
					�ȵɰ���.)
 -----------------------------------------------------------------------------------------*/
void CButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	HDC hdcMem = CreateCompatibleDC(lpDrawItemStruct->hDC); 
	SelectObject(hdcMem, CToolBarDlg::m_bitmapButton); 

	int nX = 0;	// ��Ʈ�ʿ��� ��ư�� ������ X.
	int nY = 0;	// ��Ʈ�ʿ��� ��ư�� ������ Y.

	if(m_enumToolOption == CToolOptionInfo::T_INFLATE)	// Ȯ��/���
	{
		if(m_strParam == _MTEXT( C4_ZOOM_IN))	// Ȯ��
			nX = 0;	// �̹������Ͽ��� (0, y) ��ư.
		else if(m_strParam = _MTEXT( C4_ZOOM_OUT))	// ���
			nX = TB_BUTTON_WIDTH;	// �̹������Ͽ��� (1, y)��ư
	}
	else
		nX = (m_enumToolOption + 1 ) * TB_BUTTON_WIDTH;

	if(m_enumToolOption == CToolOptionInfo::T_AUTOSCROLL)
	{ // �ڵ���ũ���� ��ư�� ���� �����ȴ�.
		if(m_strParam == _MTEXT( C4_STOP_AUTO_SCROLL))
			nX += TB_BUTTON_WIDTH;
		if(m_strParam == _MTEXT( C4_RIGHT_AUTO_SCROLL))
			nX += (TB_BUTTON_WIDTH * 2);
	}
	else if(m_enumToolOption > CToolOptionInfo::T_AUTOSCROLL)	
	{ // �ڵ���ũ�ѹ�ư���� �����Ǵ� ��ư���� �ڵ���ũ�ѹ�ư ����(3��)�� ��ŭ ���� �����Ѵ�.
		nX += (TB_BUTTON_WIDTH * 2);
	}

	if( m_enumToolOption == CToolOptionInfo::T_WHOLEVIEW)
	{
		TRACE( "%d\r\n", m_bState);
	}

	if(m_bState) // || lpDrawItemStruct->itemState & ODS_SELECTED)	// if Selected
	{
		nY = TB_BUTTON_HEIGHT;	// (x, 1) ��ư
	}

	// 50��°��ư���ʹ� �̹������Ͽ��� ����° �ٿ� �ִ� ��ư���� �����Ѵ�.
	// (2006/11/24 - Seung-Won, Bae) for Third Tool Button Image Line
	int nLineIndex = m_enumToolOption / 50;
	nX -= nLineIndex * 50 * TB_BUTTON_WIDTH;
	nY += TB_BUTTON_HEIGHT * 2 * nLineIndex;

	// Destination 
	StretchBlt( 
		lpDrawItemStruct->hDC,         // destination DC 
		lpDrawItemStruct->rcItem.left, // x upper left 
		lpDrawItemStruct->rcItem.top,  // y upper left 

		// The next two lines specify the width and 
		// height. 
		lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left, 
		lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top, 
		hdcMem,    // source device context 
		nX, nY,      // x and y upper left 
		TB_BUTTON_WIDTH,
		TB_BUTTON_HEIGHT,
		SRCCOPY);  // raster operation 

	DeleteDC(hdcMem);	
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetToolOptionInfo
 - Created at  :  2004-11-09   15:56
 - Author      :  ������
 - Description :  ���ɼ������� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
CToolOptionInfo::TOOLOPTION CButtonEx::GetToolOptionInfo()
{
	return m_enumToolOption;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetToolObject
 - Created at  :  2004-11-08   14:11
 - Author      :  ������
 - Parameters  :  pObjTool - ��ư ������ �ʿ��� ������ ��� �ִ� Object.(Type, Name, Option1, Option2)
 -                strParam - �� �ɼ�����(ex: Ȯ��/���, �ĳ��, ��ǥ������ȸ..)�� �ΰ��̻��� ��ư�� �����ؾ�
							 �ϴ� ��쿡 �� ���а� ������ �ϴ� �Ķ����.
 - Return Value:  None
 - Description :  ToolObject(Type, Name, Option1,2�� �ʿ�)�� �� �ɼǿ� �ΰ��̻��� ��ư�� ����
				  strParam�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CButtonEx::SetToolObject(CObjTool* pObjTool, const CString& strParam /*= _T("")*/)
{
	m_enumToolOption = pObjTool->GetType();
	pObjTool->GetOption(m_strOption1, m_strOption2);
	m_strParam = strParam;

	_MHWND( m_pParent->GetParent()->GetSafeHwnd());
	// �����ؽ�Ʈ ����.
	if(m_enumToolOption == CToolOptionInfo::T_INFLATE || m_enumToolOption == CToolOptionInfo::T_AUTOSCROLL)
		SetToolTipText( _MTOL4( m_strParam));
	else
		SetToolTipText( _MTOL4( pObjTool->GetName()));
}



/*-----------------------------------------------------------------------------------------
 - Function    :  AddToolInTooltip
 - Created at  :  2004-11-08   17:38
 - Description :  ������ �߰��Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CButtonEx::AddToolInTooltip(const CString& strText, const bool bActivate)
{
	// tooltip�� text�� ���� ��쿡 �߰� �Ѵ�.
	if(m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, (LPCTSTR)strText, rectBtn, 1);
	}

	// tooltip text �� set
	m_ToolTip.UpdateTipText((LPCTSTR)strText, this, 1);
	m_ToolTip.Activate(bActivate);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetToolTipText
 - Created at  :  2004-11-08   17:37
 - Description :  �����ؽ�Ʈ�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CButtonEx::SetToolTipText(const CString& strText, const bool bActivate)
{
	if(strText.IsEmpty())
		return;

	if(m_ToolTip.m_hWnd != NULL)
		return;

	m_ToolTip.Create(this);
	m_ToolTip.Activate(FALSE);
	
	AddToolInTooltip(strText, bActivate);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetToolTipText
 - Created at  :  2004-11-08   17:37
 - Description :  ���� �ؽ�Ʈ�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CButtonEx::SetToolTipText(const CString& strText)
{
	m_strToolTipText = strText;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnMouseMove
 - Created at  :  2004-11-08   16:43
 - Author      :  ������
 - Description :  ���콺 �����϶����� ������ �����ش�.
 -----------------------------------------------------------------------------------------*/
void CButtonEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_strToolTipText.IsEmpty())
		return;

	if(m_ToolTip.m_hWnd == NULL)
		SetToolTipText(m_strToolTipText, true);

	MSG msg;
	msg.hwnd = m_hWnd;
	msg.message = WM_MOUSEMOVE;
	msg.wParam = 0;
	msg.lParam = 0;
	m_ToolTip.RelayEvent(&msg);
	
	CButton::OnMouseMove(nFlags, point);
}



/*-----------------------------------------------------------------------------------------
 - Function    :  SetState
 - Created at  :  2004-11-19   11:44
 - Author      :  ������
 - Description :  ��ư���¸� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CButtonEx::SetState(bool bState /* = true */)
{
	m_bState = bState;
	Invalidate( FALSE);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetState
 - Created at  :  2004-11-19   11:25
 - Author      :  ������
 - Description :  ��ư���¸� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CButtonEx::GetState()
{
	return m_bState;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetOption1
 - Created at  :  2004-12-02   14:43
 - Author      :  ������
 - Description :  ��ư�� ù��° �ɼǰ��� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
const CString& CButtonEx::GetOption1()
{
	return m_strOption1;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetOption2
 - Created at  :  2004-12-02   14:44
 - Author      :  ������
 - Description :  ��ư�� �ι�° �ɼǰ��� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
const CString& CButtonEx::GetOption2()
{
	return m_strOption2;
}


/*-------------------------------------------------------------------------------
 - Function    :  GetParam
 - Created at  :  2005-04-15   10:57
 - Author      :  ������
 - Description :  ��ư�� �Ķ���Ͱ��� �����Ѵ�.
 -------------------------------------------------------------------------------*/
const CString& CButtonEx::GetParam()
{
	return m_strParam;
}

// (2006/11/16 - Seung-Won, Bae) Support Auto Repeat and Focus Return.
void CButtonEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CButton::OnLButtonDown(nFlags, point);
	SetCapture();
}
void CButtonEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();
	CButton::OnLButtonUp(nFlags, point);

	CWnd *pParent = GetParent();			// Getting ToolBar DLG
	if( pParent)
	{
		pParent = pParent->GetParent();
		if( pParent) pParent->SetFocus();	// Getting Chart OCX
	}
}
