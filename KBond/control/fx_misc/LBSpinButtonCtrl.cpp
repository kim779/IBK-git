// LBSpinButtonCtrl.cpp : implementation file

#include "stdafx.h"
#include "LBSpinButtonCtrl.h"

#pragma warning(disable:4786)
#include <map>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
typedef map <HWND,HWND> HWNDMAP;
static HWNDMAP gHandleMap;

#define COLOR_PAL_BG	PALETTERGB(27, 79, 129);
#define COLOR_PAL_FG	PALETTERGB(255, 145, 2);	

static LRESULT CALLBACK FilterBuddyMsgProc(int code,WPARAM wParam,  LPARAM lParam );
static HHOOK ghHook=NULL;

/////////////////////////////////////////////////////////////////////////////
// CLBSpinButtonCtrl

CLBSpinButtonCtrl::CLBSpinButtonCtrl():m_hWndBuddy(NULL),
				   m_bVertical(true),m_nSpinAlign(Outside),
				   m_bDefaultDirection(true),m_bBuddyIsEdit(false),
				   m_rctIsPressed(0,0,0,0),m_bActiveSpinPressed(true),
				   m_bAutoDisable(true),m_pParent(NULL)
{
	m_frColor = ::GetSysColor(COLOR_BTNFACE);	
}

CLBSpinButtonCtrl::~CLBSpinButtonCtrl()
{
}


BEGIN_MESSAGE_MAP(CLBSpinButtonCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CLBSpinButtonCtrl)
	ON_NOTIFY_REFLECT_EX(UDN_DELTAPOS, OnDeltapos)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLBSpinButtonCtrl message handlers

void CLBSpinButtonCtrl::PreSubclassWindow() 
{
	COLORREF clr=::GetSysColor(COLOR_3DDKSHADOW);
	m_penDarkShadow=::CreatePen(PS_SOLID,0,clr);	

	clr=::GetSysColor(COLOR_BTNSHADOW);
	m_penShadow=::CreatePen(PS_SOLID,0,clr);

	clr=::GetSysColor(COLOR_3DHILIGHT);
	m_penLight=::CreatePen(PS_SOLID,0,clr);

	clr=::GetSysColor(COLOR_3DLIGHT);
	m_penLightShadow=::CreatePen(PS_SOLID,0,clr);

	clr=m_frColor; 
	m_penButtonFace=::CreatePen(PS_SOLID,0,clr);

	Init();
	CSpinButtonCtrl::PreSubclassWindow();
}

void CLBSpinButtonCtrl::DrawUpButton(CDC *pDC, CRect rect)
{
	CRect rc, rcBtn, tRc, rcBg;
	CPoint	pts[3];
	COLORREF bg, fg;

	bg = m_frColor;
	fg = RGB(0, 0, 0);
	
	rcBg = rect;
	pDC->Draw3dRect(rcBg, RGB(250, 250, 250), RGB(100, 100, 100));
	rcBg.DeflateRect(1, 1);
	pDC->Draw3dRect(rcBg, RGB(230, 230, 230), RGB(150, 150, 150));
	rcBg.DeflateRect(1, 1);
	
	rcBtn = rcBg;
	CPen *open, pen(PS_SOLID, 1, bg);
	CBrush *obr, brush(bg);
	open = pDC->SelectObject(&pen);
	obr = pDC->SelectObject(&brush);
	pDC->FillRect(rcBg, &brush);

	pDC->MoveTo(rcBtn.left, rcBtn.top+1);
	pDC->LineTo(rcBtn.left, rcBtn.bottom-1);
	pDC->MoveTo(rcBtn.right-1, rcBtn.top+1);
	pDC->LineTo(rcBtn.right-1, rcBtn.bottom-1);

	CPen pen1(PS_SOLID, 1, fg);
	CBrush brush1(fg);
	pDC->SelectObject(&pen1);
	pDC->SelectObject(&brush1);

	tRc = rcBtn;
	tRc.DeflateRect(3, tRc.Height()*2/5);
	tRc.OffsetRect(1, 0);
	tRc.right = tRc.left + (tRc.Width()/2)*2-1;
	tRc.bottom = tRc.top + tRc.Width()/2;
	tRc.InflateRect(-2, 2);
	tRc.OffsetRect(-1, -1);

	pts[0].y = tRc.bottom;
	pts[1].y = tRc.top;
	pts[2].y = tRc.top + (tRc.Height()/2);
	pts[0].x = tRc.right - 1;
	pts[1].x = tRc.right - 1;
	pts[2].x = tRc.left;
	pDC->Polygon(pts, 3);
	
	pDC->SelectObject(open);
	pDC->SelectObject(obr);

// 2008. 3. 24. Kim Jung-tae GDI Leak Check Release Code
	pen.DeleteObject();
	pen1.DeleteObject();
	brush.DeleteObject();
	brush1.DeleteObject();
}
	
void CLBSpinButtonCtrl::DrawDownButton(CDC *pDC, CRect rect)
{
	CRect rc, rcBtn, tRc, rcBg;
	CPoint	pts[3];
	COLORREF bg, fg;

	bg = m_frColor;
	fg = RGB(0, 0, 0);
	
	rcBg = rect;
	pDC->Draw3dRect(rcBg, RGB(250, 250, 250), RGB(100, 100, 100));
	rcBg.DeflateRect(1, 1);
	pDC->Draw3dRect(rcBg, RGB(230, 230, 230), RGB(150, 150, 150));
	rcBg.DeflateRect(1, 1);

	rcBtn = rcBg;
	CPen *open, pen(PS_SOLID, 1, bg);
	CBrush *obr, brush(bg);
	open = pDC->SelectObject(&pen);
	obr = pDC->SelectObject(&brush);
	pDC->FillRect(rcBg, &brush);

	pDC->MoveTo(rcBtn.left, rcBtn.top+1);
	pDC->LineTo(rcBtn.left, rcBtn.bottom-1);
	pDC->MoveTo(rcBtn.right-1, rcBtn.top+1);
	pDC->LineTo(rcBtn.right-1, rcBtn.bottom-1);

	CPen pen1(PS_SOLID, 1, fg);
	CBrush brush1(fg);
	pDC->SelectObject(&pen1);
	pDC->SelectObject(&brush1);

	tRc = rcBtn;
	tRc.DeflateRect(3, tRc.Height()*2/5);
	tRc.OffsetRect(1, 0);
	tRc.right = tRc.left + (tRc.Width()/2)*2-1;
	tRc.bottom = tRc.top + tRc.Width()/2;
	tRc.InflateRect(-2, 2);
	tRc.OffsetRect(0, -1);

	pts[0].y = tRc.bottom;
	pts[1].y = tRc.top + (tRc.Height()/2);
	pts[2].y = tRc.top;
	pts[0].x = tRc.left;
	pts[1].x = tRc.right - 1;
	pts[2].x = tRc.left;
	pDC->Polygon(pts, 3);

	pDC->SelectObject(open);
	pDC->SelectObject(obr);

// 2008. 3. 24. Kim Jung-tae GDI Leak check Release Code
	pen.DeleteObject();
	pen1.DeleteObject();
	brush.DeleteObject();
	brush1.DeleteObject();
}

void CLBSpinButtonCtrl::OnPaint() 
{
	if(m_bAutoDisable)
	{
		CPaintDC RealDC(this); 
		
		CRect rctPaint= m_rctClient;
		CRect rcPaintUp,rcPaintDown;

		CDC dc;
		CBitmap bmpMem,*pOldMemBmp;
		dc.CreateCompatibleDC(&RealDC);
		bmpMem.CreateCompatibleBitmap(&RealDC,rctPaint.Width(),rctPaint.Height());
		pOldMemBmp=dc.SelectObject(&bmpMem);

		dc.FillSolidRect(&rctPaint,m_frColor);
		

		if(::IsWindow(m_hWndBuddy) && m_nSpinAlign!=Outside )
		{
			rctPaint.top+=2;
			rctPaint.bottom-=2;
			if(m_nSpinAlign == OnRightInside)
				rctPaint.right-=2;
			else
				rctPaint.left+=2;
		}

		rcPaintUp= rctPaint;

		if(m_bVertical)
		{
			rcPaintUp.bottom=rcPaintUp.top+rcPaintUp.Height()/2;
			rcPaintDown=rctPaint;
			rcPaintDown.top=rcPaintDown.bottom-rcPaintUp.Height();

			DrawUpButton(&dc, rcPaintUp);
			DrawDownButton(&dc, rcPaintDown);
		}
		else
		{
			rcPaintUp.right=rcPaintUp.left+rcPaintUp.Width()/2;
			rcPaintDown=rctPaint;
			rcPaintDown.left=rcPaintDown.right-rcPaintUp.Width();

			DrawUpButton(&dc, rcPaintUp);
			DrawDownButton(&dc, rcPaintDown);
		}
		
		if(::IsWindow(m_hWndBuddy) && m_nSpinAlign!=Outside )
		{
			if(m_nSpinAlign == OnRightInside)
			{
				::SelectObject(dc.m_hDC,m_penShadow);
				dc.MoveTo(m_rctClient.left,m_rctClient.top);
				dc.LineTo(m_rctClient.right,m_rctClient.top);

				::SelectObject(dc.m_hDC,m_penDarkShadow);
				dc.MoveTo(m_rctClient.left,m_rctClient.top+1);
				dc.LineTo(m_rctClient.right-1,m_rctClient.top+1);

				::SelectObject(dc.m_hDC,m_penLight);
				dc.LineTo(m_rctClient.right-1,m_rctClient.bottom-1);
				dc.LineTo(m_rctClient.left-1,m_rctClient.bottom-1);
			}
			else
			{
				::SelectObject(dc.m_hDC,m_penShadow);
				dc.MoveTo(m_rctClient.right,m_rctClient.top);
				dc.LineTo(m_rctClient.left,m_rctClient.top);
				dc.LineTo(m_rctClient.left,m_rctClient.bottom);

				::SelectObject(dc.m_hDC,m_penDarkShadow);
				dc.MoveTo(m_rctClient.right,m_rctClient.top+1);
				dc.LineTo(m_rctClient.left+1,m_rctClient.top+1);
				dc.LineTo(m_rctClient.left+1,m_rctClient.top+1);
				dc.LineTo(m_rctClient.left+1,m_rctClient.bottom-1);

				::SelectObject(dc.m_hDC,m_penLight);
				dc.LineTo(m_rctClient.right+1,m_rctClient.bottom-1);
			}

		}
		switch (m_nSpinState)
		{
		case DisableRight:
			if(m_bDefaultDirection)
			{
				if(m_bVertical)
					DisableRect(dc,rcPaintDown);
				else
					DisableRect(dc,rcPaintUp);
			}
			else
			{
				if(m_bVertical)
					DisableRect(dc,rcPaintUp);
				else
					DisableRect(dc,rcPaintDown);
			}

			break;
		case DisableLeft:
			if(m_bDefaultDirection)
			{
				if(m_bVertical)
					DisableRect(dc,rcPaintUp);
				else
					DisableRect(dc,rcPaintDown);
			
			}
			else
			{
				if(m_bVertical)
					DisableRect(dc,rcPaintDown); 
				else
					DisableRect(dc,rcPaintUp);

			}
			break;
		}

		if (m_bActiveSpinPressed)
		{
			if (m_rctIsPressed.IsRectEmpty())
			{
				CPoint pt=::GetMessagePos();
				ScreenToClient(&pt);
				if(rcPaintUp.PtInRect(pt))
				{
					m_rctIsPressed = rcPaintUp;
				}
				else
				{
					if(rcPaintDown.PtInRect(pt))
					{
						m_rctIsPressed = rcPaintDown; 
					}
				}

			}
			DrawPressedRect(dc,m_rctIsPressed);
			m_bActiveSpinPressed=false;

		}
		RealDC.BitBlt(m_rctClient.left,m_rctClient.top,m_rctClient.Width(),
			m_rctClient.Height(),&dc,m_rctClient.left,m_rctClient.top,SRCCOPY);

		dc.SelectObject(pOldMemBmp);
// 2008. 3. 24. Kim Jung-tae GDI Leack Check Release Code
		bmpMem.DeleteObject();		
	}
	else
	{
		Default();
		return;
	}

}
void CLBSpinButtonCtrl::DisableRect(CDC &dc, const CRect &rectDisable) const
{
	CBitmap bmpMask,*pBmpBeforeMask;
	CBrush	brushMask,*pOldBrush;
	COLORREF clrOldBack,clrOldText;
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	
	CRect rctClient;
	CRect rectToDisable=rectDisable;
	GetClientRect(&rctClient);

	bmpMask.CreateBitmap(rctClient.Width(),rctClient.Height(),1,1,NULL);
	pBmpBeforeMask=memDC.SelectObject(&bmpMask);

	CDC dcSrc;
	dcSrc.CreateCompatibleDC(&dc);
	CBitmap bmpSrc,*pBmpBeforeSrc;
	
	bmpSrc.CreateCompatibleBitmap(&dc,rctClient.Width(),rctClient.Height());
	pBmpBeforeSrc=dcSrc.SelectObject(&bmpSrc);

	clrOldBack=dc.SetBkColor(RGB(0,0,0)); 

	memDC.BitBlt(rectToDisable.left,rectToDisable.top,
		rectToDisable.Width(),rectToDisable.Height(),&dc,
		rectToDisable.left,rectToDisable.top,SRCCOPY);

	dcSrc.BitBlt(rectToDisable.left,rectToDisable.top,
		rectToDisable.Width(),rectToDisable.Height(),&dc,
		rectToDisable.left,rectToDisable.top,SRCCOPY);

	CBrush brushSrc;
	rectToDisable.DeflateRect(1,1);

	brushSrc.CreateSolidBrush(::GetSysColor(COLOR_3DSHADOW));
	dcSrc.SelectObject(&brushSrc);
	dcSrc.SetBkColor(RGB(255,255,255));
	dcSrc.SetTextColor(RGB(0,0,0));
	dcSrc.BitBlt(rectToDisable.left,rectToDisable.top,
		rectToDisable.Width(),rectToDisable.Height(),&memDC,
		rectToDisable.left,rectToDisable.top,0x00E20746L);

	brushMask.CreateSolidBrush(::GetSysColor(COLOR_3DHILIGHT));
	pOldBrush = dc.SelectObject(&brushMask);
	dc.SetBkColor(RGB(255,255,255));
	clrOldText=dc.SetTextColor(RGB(0,0,0));
	dc.BitBlt(rectToDisable.left+1,rectToDisable.top+1,
		rectToDisable.Width()-1,rectToDisable.Height()-1,&memDC,
		rectToDisable.left,rectToDisable.top,0x00E20746L);

	memDC.BitBlt(rectToDisable.left,rectToDisable.top,
		rectToDisable.Width(),rectToDisable.Height(),&dcSrc,
		rectToDisable.left,rectToDisable.top,SRCCOPY);

	dc.BitBlt(rectToDisable.left,rectToDisable.top,
		rectToDisable.Width(),rectToDisable.Height(),&dcSrc,
		rectToDisable.left,rectToDisable.top,SRCINVERT);

	dc.BitBlt(rectToDisable.left,rectToDisable.top,
		rectToDisable.Width(),rectToDisable.Height(),&memDC,
		rectToDisable.left,rectToDisable.top,SRCAND);
	dc.BitBlt(rectToDisable.left,rectToDisable.top,
		rectToDisable.Width(),rectToDisable.Height(),&dcSrc,
		rectToDisable.left,rectToDisable.top,SRCINVERT);

	memDC.SelectObject(pBmpBeforeMask);
	dcSrc.SelectObject(pBmpBeforeSrc);

	dc.SetBkColor(clrOldBack);
	dc.SetTextColor(clrOldText);
	dc.SelectObject(pOldBrush);
// 2008. 3. 24. Kim Jung-tae GDI Leack Check Release Code
	bmpMask.DeleteObject();
	bmpSrc.DeleteObject();
	brushSrc.DeleteObject();
	brushMask.DeleteObject();
}

void CLBSpinButtonCtrl::DrawPressedRect(CDC &dc, const CRect &rctToDown) const
{

	CRect 	rctDown=rctToDown;
	HPEN hOldPen;
	
	dc.BitBlt(rctDown.left+1,rctDown.top+1,
	rctDown.Width()-1,rctDown.Height()-1,&dc,
	rctDown.left,rctDown.top,SRCCOPY);

	rctDown.bottom-=1;

	hOldPen = (HPEN)SelectObject(dc.m_hDC,m_penShadow);
	dc.MoveTo(rctDown.left,rctDown.bottom-1);
	dc.LineTo(rctDown.left,rctDown.top);
	dc.LineTo(rctDown.right-1,rctDown.top);
		
	SelectObject(dc.m_hDC,m_penLight);
	dc.LineTo(rctDown.right-1,rctDown.bottom);
	dc.LineTo(rctDown.left-1,rctDown.bottom);

	SelectObject(dc.m_hDC,m_penDarkShadow);
	dc.MoveTo(rctDown.left+1,rctDown.bottom-2);
	dc.LineTo(rctDown.left+1,rctDown.top+1);
	dc.LineTo(rctDown.right-2,rctDown.top+1);

	SelectObject(dc.m_hDC,m_penLightShadow);
	dc.LineTo(rctDown.right-2,rctDown.bottom-1);
	dc.LineTo(rctDown.left,rctDown.bottom-1);

	SelectObject(dc.m_hDC,m_penButtonFace);
	dc.MoveTo(rctDown.left+2,rctDown.bottom-2);
	dc.LineTo(rctDown.left+2,rctDown.top+2);
	dc.LineTo(rctDown.right-2,rctDown.top+2);
		
	SelectObject(dc.m_hDC,hOldPen);
}

void CLBSpinButtonCtrl::Init()
{ 
	m_hWndBuddy=(HWND)::SendMessage(m_hWnd, UDM_GETBUDDY, 0, 0l);

	DWORD dwStyle=::GetWindowLong(m_hWnd,GWL_STYLE);
	if(dwStyle & UDS_WRAP)
		m_bAutoDisable=false; 

	if(m_bAutoDisable) 
	{
		if(dwStyle & UDS_ALIGNRIGHT )
			m_nSpinAlign=OnRightInside;
		else
		{
			if(dwStyle & UDS_ALIGNLEFT)
			{
				m_nSpinAlign=OnLeftInside;
			
			}
		}
		if(dwStyle & UDS_HORZ)
			m_bVertical=false;

		GetRange32(m_nMinPos,m_nMaxPos);	
		if( m_nMinPos < m_nMaxPos)
			m_bDefaultDirection =false;
		else
		{
			int nTemp = m_nMinPos;
			m_nMinPos = m_nMaxPos;
			m_nMaxPos=nTemp;
		}

		GetClientRect(&m_rctClient);

		m_nPrevPos = GetPos();

		if(m_nPrevPos < m_nMinPos || m_nPrevPos > m_nMaxPos)
			m_nPrevPos=m_nMinPos;

		m_nSpinState = BothEnable;
		if(m_nPrevPos == m_nMinPos)
				m_nSpinState = DisableLeft;
		else
		{
			if(m_nPrevPos == m_nMaxPos)
				m_nSpinState = DisableRight;

		}

		if(::IsWindow(m_hWndBuddy))
		{
			char buf[5];
			::GetClassName(m_hWndBuddy,buf,sizeof(buf)/sizeof(buf[0]));
			
			if(!strcmp(buf,"Edit"))
			{
				m_bBuddyIsEdit=true;

				if(ghHook==NULL)
					ghHook=SetWindowsHookEx(WH_CALLWNDPROC,FilterBuddyMsgProc,NULL, 
											GetCurrentThreadId());

				HWNDMAP::iterator iterHwnd=gHandleMap.find(m_hWndBuddy);
				if(iterHwnd != gHandleMap.end())
				{
					if((*iterHwnd).second != m_hWnd)
					{
						gHandleMap.erase(iterHwnd);	
						gHandleMap.insert(HWNDMAP::value_type(m_hWndBuddy,m_hWnd));
					}
				}
				else
					gHandleMap.insert(HWNDMAP::value_type(m_hWndBuddy,m_hWnd));
			}
		}
		Invalidate(FALSE);
	}
}

BOOL CLBSpinButtonCtrl::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_bAutoDisable)
	{
		NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

		int nNextPos=pNMUpDown->iPos+pNMUpDown->iDelta;

		if(nNextPos > m_nMaxPos)
			nNextPos = m_nMaxPos;
		else
		{
			if(nNextPos < m_nMinPos)
				nNextPos = m_nMinPos;
		}
	
		if(m_nPrevPos!=nNextPos)
		{
			if(pNMUpDown->iDelta)
				m_bActiveSpinPressed=true;

			if(nNextPos  <m_nMaxPos && nNextPos >m_nMinPos)
			{
				m_nSpinState = BothEnable;
			}
			else
			{
				if(nNextPos == m_nMaxPos)
				{
					if(m_nMaxPos != m_nMinPos)
					{
						m_nSpinState=DisableRight;
						m_bActiveSpinPressed=false;
					}
					else
					{
						m_nSpinState = BothDisable;
						m_bActiveSpinPressed=false;
					}
				}
				else
				{
					if(nNextPos == m_nMinPos)
					{
						m_nSpinState=DisableLeft;
						m_bActiveSpinPressed=false;
					}
				}
			}
			m_nPrevPos=nNextPos;
			Invalidate(FALSE);

			*pResult = 0;
			return FALSE;
		}
		else
		{
			*pResult = 1;
			return TRUE;
		}
	}
	else
	{ 
		*pResult = 0;
		return FALSE;
	}
}


LRESULT CLBSpinButtonCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	LRESULT nRet = CSpinButtonCtrl::WindowProc(message, wParam, lParam);

	switch(message)
	{
		case UDM_SETRANGE32:
		case UDM_SETRANGE: 
			Init();
			break;

		case UDM_SETBUDDY :
			CleanUpHook();
			Init();
			if(::IsWindow(m_hWndBuddy))
				::SetWindowPos(m_hWnd,m_hWndBuddy,0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
			break;
	}
	return nRet;

}

void CLBSpinButtonCtrl::CleanUpHook() const
{
	if(m_bBuddyIsEdit)
	{
		HWNDMAP::iterator iterHwnd=gHandleMap.find(m_hWndBuddy);
		if(iterHwnd != gHandleMap.end() && (*iterHwnd).second == m_hWnd)
		{
			iterHwnd = gHandleMap.erase(iterHwnd);
			if(!gHandleMap.size() && ghHook!=NULL)
			{
				UnhookWindowsHookEx( ghHook);
				ghHook=NULL;
			}
		}
	}
}

void CLBSpinButtonCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_rctIsPressed.SetRectEmpty();

	if (m_pParent)
		m_pParent->SendMessage(WM_LBUTTONDOWN, 0, 0);
		
	CSpinButtonCtrl::OnLButtonDown(nFlags, point);
}

BOOL CLBSpinButtonCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE	; 
}

void CLBSpinButtonCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	Invalidate();	
	UpdateWindow();
	m_pParent->PostMessage(WM_LBUTTONDOWN, 0, 0);
	CSpinButtonCtrl::OnLButtonUp(nFlags, point);
}

bool CLBSpinButtonCtrl::SetAutoDisable(bool bSetOn)
{
	bool bPrev = m_bAutoDisable;
	m_bAutoDisable = bSetOn;
	Init();

	return bPrev;
}


LRESULT CALLBACK FilterBuddyMsgProc(  int code,  WPARAM wParam,  LPARAM lParam )
{
	CWPSTRUCT* pData=reinterpret_cast<CWPSTRUCT*>(lParam);
	if(WM_COMMAND==pData->message && EN_UPDATE==HIWORD(pData->wParam))
	{
		HWNDMAP::iterator iterHwnd=gHandleMap.find(reinterpret_cast<HWND>(pData->lParam));
		if(iterHwnd != gHandleMap.end())
		{
			CString strText;
			int nLen=::GetWindowTextLength((*iterHwnd).first);
			::GetWindowText((*iterHwnd).first,strText.GetBufferSetLength(nLen),nLen+1);
			strText.ReleaseBuffer();

			strText.Remove((TCHAR)0xA0);

			NMUPDOWN nmUpDn;
			nmUpDn.iDelta=0;
			nmUpDn.iPos=atoi(strText);
			nmUpDn.hdr.code=UDN_DELTAPOS;
			nmUpDn.hdr.hwndFrom=(*iterHwnd).second;
			nmUpDn.hdr.idFrom=::GetDlgCtrlID((*iterHwnd).second);

			::SendMessage(::GetParent((*iterHwnd).second),
							WM_NOTIFY,(WPARAM)nmUpDn.hdr.idFrom,
							(LPARAM)&nmUpDn);
		}
	}
	return CallNextHookEx(ghHook,code,wParam,lParam);
}

void CLBSpinButtonCtrl::OnDestroy() 
{
	CleanUpHook();

	CSpinButtonCtrl::OnDestroy();
}
