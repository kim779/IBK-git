// CToolTip.cpp: 구현 파일
//

#include "StdAfx.h"
#include "testctrl.h"
#include "CToolTip.h"
#include "CGrpWnd.h"
#include "../../H/memDc.h"

// CToolTip

IMPLEMENT_DYNAMIC(CToolTip, CWnd)

CToolTip::CToolTip(CWnd* pView, CWnd* pGrpWnd)
{
	WNDCLASS wndcls{};
	HINSTANCE hInst = AfxGetInstanceHandle();
	if (!(::GetClassInfo(hInst, JUGA_TOOLTIP_CLASS, &wndcls)))
	{
		wndcls.style = CS_SAVEBITS;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInst;
		wndcls.hIcon = nullptr;
		wndcls.hCursor = LoadCursor(hInst, IDC_ARROW);
		wndcls.hbrBackground = (HBRUSH)(COLOR_INFOBK + 1);
		wndcls.lpszMenuName = nullptr;
		wndcls.lpszClassName = JUGA_TOOLTIP_CLASS;

		if (!AfxRegisterClass(&wndcls))	AfxThrowResourceException();
	}

	m_nCellHeight = 0;
	CTestctrlApp* pApp = (CTestctrlApp*)AfxGetApp();
	m_font = pApp->GetFont(pView, 9, "굴림");
	m_pGrpWnd = (CGrpWnd*)pGrpWnd;
	m_clrBack = GetSysColor(COLOR_INFOBK);
}

CToolTip::~CToolTip()
{
}


BEGIN_MESSAGE_MAP(CToolTip, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CToolTip 메시지 처리기

int CToolTip::Create()
{
	return CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, JUGA_TOOLTIP_CLASS, nullptr, WS_BORDER | WS_POPUP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, nullptr);
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
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	xxx::CMemDC	mdc(&dc);
	DrawTip(&mdc);
}

void CToolTip::DrawTip(CDC* pDC)
{
	CRect		rect;
	CString		tmpstr;
	const	int	nCount = m_arTitle.GetSize();

	GetClientRect(rect);
	pDC->FillSolidRect(rect, m_clrBack);
	CFont* sFont = pDC->SelectObject(m_font);
	rect = m_rcDraw;

	for (int ii = 0; ii < nCount; ii++)
	{
		rect.bottom = rect.top + m_nCellHeight;
		tmpstr = m_arTitle.GetAt(ii);
		pDC->DrawText(tmpstr, rect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
		tmpstr = m_arContent.GetAt(ii);
		pDC->DrawText(tmpstr, rect, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
		rect.top = rect.bottom;
	}

	pDC->SelectObject(sFont);
}

CRect CToolTip::SetData(CString sData)
{
	int	index = 0;
	CString	title, content;

	m_arTitle.RemoveAll();
	m_arContent.RemoveAll();
	for (; !sData.IsEmpty(); )
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

	for (int ii = 0; ii < nCount; ii++)
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
	CFont* oldfont = nullptr;
	CDC* pDC = GetDC();

	oldfont = pDC->SelectObject(m_font);
	csSize = pDC->GetTextExtent(strExam);
	pDC->SelectObject(oldfont);
	ReleaseDC(pDC);
	return csSize;
}

void CToolTip::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ClientToScreen(&point);
	m_pGrpWnd->TipMouseMove(point);
	CWnd::OnMouseMove(nFlags, point);
}
