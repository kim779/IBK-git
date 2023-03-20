// BasketWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB191000.h"
#include "BasketWnd.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBasketWnd

CBasketWnd::CBasketWnd(CWnd* pMainWnd) : CBaseWnd(pMainWnd)
{
}

CBasketWnd::~CBasketWnd()
{
}


BEGIN_MESSAGE_MAP(CBasketWnd, CBaseWnd)
	//{{AFX_MSG_MAP(CBasketWnd)

	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBasketWnd message handlers


void CBasketWnd::OperDraw(CDC* pDC)
{
	CFont	font;
	COLORREF	clr = pDC->SetTextColor(RGB(0, 0, 255));
	int	nmode = pDC->SetBkMode(TRANSPARENT);
	font.CreatePointFont(25*10, _T("����ü"));
	CFont*	oldfont = pDC->SelectObject(&font);
	CRect	rect;

	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));
	pDC->DrawText("������ �������Դϴ�. ���� �ǰ� ��Ź�帮�ڽ��ϴ�.", rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_WORDBREAK);
	pDC->SetBkMode(nmode);
	pDC->SetTextColor(clr);
	pDC->SelectObject(oldfont);
	font.DeleteObject();	
}