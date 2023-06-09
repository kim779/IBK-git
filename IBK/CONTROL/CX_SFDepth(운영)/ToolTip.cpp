// ToolTip.cpp : implementation file
//

#include "stdafx.h"
#include "ToolTip.h"
#include "depth.h"
#include "memDC.h"
//#include "GrpWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolTip

CToolTip::CToolTip(CWnd *pView, CWnd *pGrpWnd)
{
	WNDCLASS wndcls{};
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, JUGA_TOOLTIP_CLASS, &wndcls)))
	{
		wndcls.style		= CS_SAVEBITS;
		wndcls.lpfnWndProc	= ::DefWindowProc;
		wndcls.cbClsExtra	= wndcls.cbWndExtra = 0;
		wndcls.hInstance	= hInst;
		wndcls.hIcon		= NULL;
		wndcls.hCursor		= LoadCursor(hInst, IDC_ARROW);
		wndcls.hbrBackground	= (HBRUSH)(COLOR_INFOBK + 1); 
		wndcls.lpszMenuName	= NULL;
		wndcls.lpszClassName	= JUGA_TOOLTIP_CLASS;

		if (!AfxRegisterClass(&wndcls))	AfxThrowResourceException();
	}

	m_nCellHeight = 0;

	m_pView = pView;
	m_pWnd = (Cdepth *)pGrpWnd;
	m_font = m_pWnd->getFont(9, "����");
	m_clrBack = RGB(255,255,255);
}

CToolTip::~CToolTip()
{
}


BEGIN_MESSAGE_MAP(CToolTip, CWnd)
	//{{AFX_MSG_MAP(CToolTip)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CToolTip message handlers

int CToolTip::Create()
{
	return CreateEx(WS_EX_TOOLWINDOW, JUGA_TOOLTIP_CLASS, NULL, WS_BORDER | WS_POPUP,
                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
		            NULL, NULL, NULL);
}

int CToolTip::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_clrBack = GetSysColor(COLOR_INFOBK);
	
	return 0;
}

void CToolTip::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
#ifdef _DEBUG
	DrawTip(&dc);
#else
	xxx::CMemDC	mdc(&dc);
	DrawTip(&mdc);
#endif
}

void CToolTip::DrawTip(CDC *pDC)
{
	CRect		rect;
	CString		tmpstr;
	const	int	nCount = m_arTitle.GetSize();

	GetClientRect(rect);
	pDC->FillSolidRect(rect, m_clrBack);	
	CFont *sFont = pDC->SelectObject(m_font);
	rect = m_rcDraw;
	
	for (int ii = 0 ; ii < nCount ; ii++)
	{
		rect.bottom = rect.top + m_nCellHeight;
		tmpstr = m_arTitle.GetAt(ii);
		pDC->DrawText(tmpstr, rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
		tmpstr = m_arContent.GetAt(ii);
		pDC->DrawText(tmpstr, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
		rect.top = rect.bottom;
	}
	
	pDC->SelectObject(sFont);
}

LRESULT CToolTip::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_ERASEBKGND)
		return TRUE;
	
	return CWnd::WindowProc(message, wParam, lParam);
}


//title + \t + content + \n + title + \t + content + \n...
CRect CToolTip::SetData(CString sData)
{	
	int	index = 0;
	CString	title, content;

	m_arTitle.RemoveAll();
	m_arContent.RemoveAll();
	for (;!sData.IsEmpty(); )
	{
		index = sData.Find('\t');
		if (index < 0)
			break;
		title = sData.Left(index++);
		sData = sData.Mid(index);

		index = sData.Find('\n');
		if (index < 0)
			index = sData.GetLength();
		content = sData.Left(index++);

		m_arTitle.Add(title);
		m_arContent.Add(content);

		if (sData.GetLength() <= index)
			break;
		sData = sData.Mid(index);
	}

	return MakeSizeWnd();
}

CRect CToolTip::MakeSizeWnd() 
{
	CRect	rcWnd;
	CRect	rcDraw;
	CString	tmpstr;
	int	nMaxWidth = 0;
	int	nHeight = 0;
	CSize	size = CSize(0, 0);
	const	int	nGab = 2;
	const	int	nCount = m_arTitle.GetSize();

	GetWindowRect(rcWnd);
	rcDraw = rcWnd;
	rcDraw.DeflateRect(nGab, nGab, nGab * 2, nGab);
	
	for (int ii = 0 ; ii < nCount ; ii++)
	{
		tmpstr.Format("%s %s", m_arTitle.GetAt(ii), m_arContent.GetAt(ii));
		size = CalWndSize(tmpstr);
		size.cx += 4;

		if (size.cx > nMaxWidth)
			nMaxWidth = size.cx;
	}

	m_nCellHeight = nHeight = size.cy + 2;
	rcDraw.right = rcDraw.left + nMaxWidth;
	rcDraw.bottom = rcDraw.top + (nHeight * nCount);

	rcWnd = rcDraw;
	rcWnd.InflateRect(nGab, nGab, nGab * 2, nGab);	
	m_rcDraw = rcDraw;
	ScreenToClient(m_rcDraw);	
	m_rcBase = rcWnd;	

	return rcWnd;
}

CSize CToolTip::CalWndSize(CString strExam)
{
	CSize	csSize;
	CFont	*oldfont = NULL;
	CDC*	pDC = GetDC();

	oldfont = pDC->SelectObject(m_font);
	csSize = pDC->GetTextExtent(strExam);
	pDC->SelectObject(oldfont);
	ReleaseDC(pDC);		
	return csSize;
}

void CToolTip::OnMouseMove(UINT nFlags, CPoint point) 
{
//	ClientToScreen(&point);
//	m_pWnd->TipMouseMove(point);

	CWnd::OnMouseMove(nFlags, point);
}
