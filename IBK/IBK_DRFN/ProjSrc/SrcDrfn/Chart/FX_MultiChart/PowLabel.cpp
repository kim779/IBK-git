// PowLabel.cpp : implementation file
//

#include "stdafx.h"
#include "PowLabel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Note *********************************************************************
// This PowLabel Overrides  NT ALMOND's Label Class
// **************************************************************************
/////////////////////////////////////////////////////////////////////////////
// CPowLabel Version 1.2
//
// From now on I'll try to keep a log of fixes and enhancements...
// 
// The new feature were added due to the response of people.
// All I ask is to all you programmers out there, is if you add, fix or
// enhance this code, sent me a copy and I'll send the copy on to www.codeproject.com
//
// Happy Software Engineer :)
// 
// New features include:
//
// A. Support for 3D Fonts
// B. Support for background transparency
// C. More comments provided
// D. If alignment is 'centered' and the window text is seperated by '\r\n'
//	  the will be centered accordingly - requested by someone @ nasa ;)
// E. Support for font rotation.
// F. Respond to System Color Change
// G. OnPaint improved performance - using Double Buffering Technique
//
// Thanks to:
// Mark McDowell	- For suggestion on 'Increasing the flexibility of "hypertext" setting...'
// Erich Ruth		- For suggestion on 'Font Rotation'
//

/////////////////////////////////////////////////////////////////////////////
// CPowLabel Version 1.3
//
// A. Added SS_LEFTNOWORDWRAP to include wordwrap
// B. Fix repainting problem 
// C. Fix SetBkColor
// D. Added SS_CENTER

// Thanks to:
// Marius						- Added styling problem.
// Azing Vondeling & Broker		- Spotting painting Problem.
// Mel Stober					- Back Color & SS_CENTER
// 
/////////////////////////////////////////////////////////////////////////////
// CPowLabel Version 1.4
//
// A. Fix to transparency mode
// B. Added new SetText3DHiliteColor to change the 3D Font face color - default is white.
// 
// Thanks to:
// michael.groeger				- Spotting Transparency with other controls bug.
//
//
/////////////////////////////////////////////////////////////////////////////
// CPowLabel Version 1.5
//
// A. Sanity handle check
// B. Support Interface Charset
// C. Check compilition with _UNICODE
// D. Fix hyperlink feature
// E. Support default Dialog Font
// F. Inclusion of SS_OWNERDRAW via control creation and subclassing
// G. Modification to Text aligmnent code
// H. New background gradient fill function
// 
// Thanks to:
// Steve Kowald				- Using null handles 
// Alan Chan				- Supporting International Windows
// Dieter Fauth				- Request for default Dialog font
// Herb Illfelder			- Text Alignment code
// 
//
/////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::CPowLabel
// Description:		Default contructor
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
CPowLabel::CPowLabel()
{
	m_crText		= GetSysColor(COLOR_WINDOWTEXT);

	m_hBackBrush	= NULL;		// 1.1

	m_crHiColor		= 0;
	m_crLoColor		= 0;

	m_bTimer		= FALSE;
	m_bState		= FALSE;
	m_bTransparent	= FALSE;
	m_bLink			= TRUE;
	m_hCursor		= NULL;
	m_Type			= None;
	m_bFont3d		= FALSE;
	m_bNotifyParent = FALSE;
	m_bToolTips		= FALSE;
	m_bRotation		= FALSE;
	m_bBorderColor	= FALSE;
	m_fillmode		= Normal;
	m_cr3DHiliteColor =	RGB(255,255,255);

	m_hwndBrush		= ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	m_bSignCheck	= FALSE;

	m_strText		= _T("");
	m_bFirstDraw	= TRUE;

	m_bUseHandCursor= FALSE;	// Hand Cursor Use(default false)
	m_hHandCursor	=  NULL;	// Hand Cursor Handle
	LoadLibHandCursor();
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::~CPowLabel
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
// NT ALMOND				15092000	1.5		Handle Check
//////////////////////////////////////////////////////////////////////////
CPowLabel::~CPowLabel()
{
	// Clean up
	m_font.DeleteObject();
	::DeleteObject(m_hwndBrush);

	// Stop Checking complaining
	if (m_hBackBrush)
		::DeleteObject(m_hBackBrush);

	if(m_hHandCursor)
	{
		DestroyCursor(m_hHandCursor);//KHD
		m_hHandCursor = NULL;
	}	
}

BEGIN_MESSAGE_MAP(CPowLabel, CStatic)
	//{{AFX_MSG_MAP(CPowLabel)
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CPowLabel message handlers


// ***********************************************************************
// Modification this function because frequently blink state.
// Then, Use Different Refresh Option. Opaque State and Transparent State. 
//	- SeoJeong - 2005/07/03
// ************************************************************************
void CPowLabel::UpdateSurface()
{
	// Opaque State
	if ( !m_bTransparent)
	{
		//RedrawWindow();
		//GetParent()->UpdateWindow();
		RedrawLabel();
	}
	// Transparent State
	else
	{
		CRect (rc);
		GetWindowRect(rc);
		GetParent()->ScreenToClient(rc);
		GetParent()->InvalidateRect(rc,TRUE);
		GetParent()->UpdateWindow();

		Invalidate();
	}
}

//*******************************************
//	RedrawLabel - SeoJeong - 2005/07/03
// ******************************************
void CPowLabel::RedrawLabel(CDC* pDC/*=NULL*/)
{
   BOOL bMustReleaseDC = FALSE;

    CRect rect;
    GetClientRect(rect);

    if (!pDC)
    {
        pDC = GetDC();
        if (pDC)
            bMustReleaseDC = TRUE;
    }

    if (pDC)
    {
		OnDraw(pDC);
    } 
	else
        InvalidateRect(rect, TRUE);     // Could not get a DC - invalidate it anyway
									    // and hope that OnPaint manages to get one

    if (bMustReleaseDC)
        ReleaseDC(pDC);
}


//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::ReconstructFont
// Description:		Helper function to build font after it was changed
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
void CPowLabel::ReconstructFont()
{
	m_font.DeleteObject();
	BOOL bCreated = m_font.CreateFontIndirect(&m_lf);

	ASSERT(bCreated);
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::OnPaint
// Description:		Handles all the drawing code for the label
// NOTES:			Called by Windows... not by USER
//					Probably needs tiding up a some point.
//					Different states will require this code to be reworked.
// MODIFICATIONS:
// Name                     Date        Version Comments
// NT ALMOND				22/10/98    1.0     Origin
// NT ALMOND				15092000	1.5		Handle Check
// NT ALMOND				15092000	1.5		Alignment mods
// NT ALMOND				15092000	1.5		Gradient Fill Mode
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::SetBorderColor(COLORREF crBorderParm, int nWidth)
{
	m_crBorder = crBorderParm;
	m_bBorderColor = TRUE;
	m_iBorderWidth = nWidth;

	return *this;
}

void CPowLabel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	OnDraw( &dc);

}

//*******************************************
// 	OnDraw - SeoJeong - 2005/07/02
// ******************************************
void CPowLabel::OnDraw(CDC* pDC)
{
	if ( m_bFirstDraw && m_strText == _T(""))	// Get Default Caption Data - SeoJeong - 2005/07/10
		GetWindowText(m_strText);

	m_bFirstDraw = FALSE;


	CRect rc, tmpRC;
	GetClientRect(rc);
	CString strText;
	//GetWindowText(strText);	// Do Not Use this Method - SeoJeong - 2005/07/10
	strText = m_strText;

	if(m_bSignCheck) 
	{
		SignDraw(m_pWndSign, m_nSign);
		return;
	}

	// ***********************************************
	// Set up for double buffering...
	// ***********************************************
	CBitmap bmp;
	CDC* pDCMem=NULL;
	CPen pen, *oldpen;

	if (!m_bTransparent)
	{
		pDCMem = new CDC;
		pDCMem->CreateCompatibleDC(pDC);
		bmp.CreateCompatibleBitmap(pDC,rc.Width(),rc.Height());
		pDCMem->SelectObject(&bmp);
	}
	else
	{
		pDCMem = pDC;
	}

	UINT nMode = pDCMem->SetBkMode(TRANSPARENT);


	COLORREF crText = pDCMem->SetTextColor(m_crText);
	CFont *pOldFont = pDCMem->SelectObject(&m_font);


	// Fill in backgound if not transparent
	if (!m_bTransparent)
	{
		if (m_fillmode == Normal)
		{
			CBrush br;
			
			if (m_hBackBrush != NULL)
				br.Attach(m_hBackBrush);
			else
				br.Attach(m_hwndBrush);
					
			pDCMem->FillRect(rc,&br);

			br.Detach();
		}
		else // Gradient Fill
		{
			DrawGradientFill(pDCMem, &rc, m_crLoColor, m_crHiColor, 100);
		}
	}
	
	if(m_bBorderColor) 
	{
		pen.CreatePen(PS_SOLID,m_iBorderWidth,m_crBorder);
		oldpen = pDCMem->SelectObject(&pen);

		tmpRC.top = rc.top;
		tmpRC.left = rc.left;
		tmpRC.bottom = rc.bottom - 1;
		tmpRC.right = rc.right - 1;

		pDCMem->MoveTo(tmpRC.left, tmpRC.top);
		pDCMem->LineTo(tmpRC.right, tmpRC.top );
		pDCMem->LineTo(tmpRC.right, tmpRC.bottom);
		pDCMem->LineTo(tmpRC.left, tmpRC.bottom);
		pDCMem->LineTo(tmpRC.left, tmpRC.top);

		pDCMem->SelectObject(oldpen);
		pen.DeleteObject();
	}

	// If the text is flashing turn the text color on
	// then to the color of the window background.

	LOGBRUSH lb;
	ZeroMemory(&lb,sizeof(lb));

	// Stop Checking complaining
	if (m_hBackBrush)
		::GetObject(m_hBackBrush,sizeof(lb),&lb);


	// Something to do with flashing
	if (!m_bState && m_Type == Text)
		pDCMem->SetTextColor(lb.lbColor);

	DWORD dwFlags = 0;
	DWORD style = GetStyle();
	
	switch (style & SS_TYPEMASK)
	{
		case SS_RIGHT: 
			dwFlags = DT_RIGHT | DT_WORDBREAK; 
			break; 
		
		case SS_CENTER: 
			dwFlags = DT_CENTER | DT_WORDBREAK;
			break;

		case SS_LEFTNOWORDWRAP: 
			dwFlags = DT_LEFT; 
			break;

		default: // treat other types as left
			case SS_LEFT: 
				dwFlags = DT_LEFT | DT_WORDBREAK; 
				break;
	}	

	
	// If the text centered make an assumtion that
	// the will want to center verticly as well
	if (style & SS_CENTERIMAGE)
	{
		//dwFlags = DT_CENTER;

		// Apply 
		if (strText.Find(_T("\r\n")) == -1)
		{
			dwFlags |= DT_VCENTER;

			// And because DT_VCENTER only works with single lines
			dwFlags |= DT_SINGLELINE; 
		}
	}

	// ���� dwFlags ���������� �ٲ�����Ƿ� ����! - SeoJeong - 
	//dwFlags = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	

	// ***********************************************
	// 3333   DDDDD
	//     3  D    D
	//   33   D     D    E F X 
	//     3  D    D
	// 3333   DDDDD
	// ***********************************************
	if (m_bRotation)
	{
		tmpRC = rc;
		if(m_bBorderColor) {
			tmpRC.left = (tmpRC.left + m_iBorderWidth + 1) / 2;
			tmpRC.top  = (tmpRC.top  + (m_iBorderWidth + 3)) / 2;
		}

		int nAlign = pDCMem->SetTextAlign (TA_BASELINE);

		CPoint pt;
		GetViewportOrgEx (pDCMem->m_hDC,&pt) ;
		SetViewportOrgEx (pDCMem->m_hDC,rc.Width() / 2, rc.Height() / 2, NULL) ;
		pDCMem->TextOut (0, 0, strText) ;
		SetViewportOrgEx (pDCMem->m_hDC,pt.x / 2, pt.y / 2, NULL) ;
		pDCMem->SetTextAlign (nAlign);
	}
	else
	{
		tmpRC = rc;
		if(m_bBorderColor) {
			tmpRC.left	 = (tmpRC.left	+ m_iBorderWidth + 1) / 2;
			tmpRC.top	 = (tmpRC.top	+ (m_iBorderWidth + 3)) / 2;
			tmpRC.right	 = (tmpRC.right	- m_iBorderWidth - 1);
			tmpRC.bottom = (tmpRC.bottom - (m_iBorderWidth - 3));
		}

		// ���������� ���
		if (dwFlags & SS_RIGHT)
		{
			rc.right -=3;		
			pDCMem->DrawText(strText,rc,dwFlags);
			rc.right +=3;

			if (m_bFont3d)
			{
				pDCMem->SetTextColor(m_cr3DHiliteColor);
				
				if (m_3dType == Raised)
					rc.OffsetRect(-1-3,-1);
				else
					rc.OffsetRect(1-3,1);
				
				pDCMem->DrawText(strText,rc,dwFlags);
			}

		}
		// ���������� ���
		else if(dwFlags & SS_LEFT)
		{
			rc.left +=3;		
			pDCMem->DrawText(strText,rc,dwFlags);
			rc.left -=3;

			if (m_bFont3d)
			{
				pDCMem->SetTextColor(m_cr3DHiliteColor);
				
				if (m_3dType == Raised)
					rc.OffsetRect(-1+3,-1);
				else
					rc.OffsetRect(1+3,1);
				
				pDCMem->DrawText(strText,rc,dwFlags);
			}
		}
		else
		{
			pDCMem->DrawText(strText,rc,dwFlags);

			if (m_bFont3d)
			{
				pDCMem->SetTextColor(m_cr3DHiliteColor);
				
				if (m_3dType == Raised)
					rc.OffsetRect(-1,-1);
				else
					rc.OffsetRect(1,1);
				
				pDCMem->DrawText(strText,rc,dwFlags);
			}
		}
	}

	// Restore DC's State
	pDCMem->SetBkMode(nMode);
	pDCMem->SelectObject(pOldFont);
	pDCMem->SetTextColor(crText);

	// BitBlt
	if (!m_bTransparent)
	{
		pDC->BitBlt(0,0,rc.Width(),rc.Height(),pDCMem,0,0,SRCCOPY);
		pDCMem->DeleteDC();
		bmp.DeleteObject();
		delete pDCMem;
	}
}

BOOL CPowLabel::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;    // Don't erase the background.
}



//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::OnTimer
// Description:		Used in conjunction with 'FLASH' functions
// INPUTS:          Windows API
// RETURNS:         Windows API
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
void CPowLabel::OnTimer(UINT nIDEvent) 
{
	m_bState = !m_bState;

	UpdateSurface();
	
	CStatic::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::OnSetCursor
// Description:		Used in conjunction with 'LINK' function
// INPUTS:          Windows API
// RETURNS:         Windows API
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
BOOL CPowLabel::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{

	if (m_hCursor)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}

	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::OnLButtonUp
// Description:		Called when a link is click on
// INPUTS:          Windows API
// RETURNS:         Windows API
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
void CPowLabel::OnLButtonUp(UINT nFlags, CPoint point) 
{

/*
	if (!m_bNotifyParent) // Fix
	{
		CString strLink;

		GetWindowText(strLink);
		ShellExecute(NULL,_T("open"),strLink,NULL,NULL,SW_SHOWNORMAL);
	}
	else
	{
		// To use notification in parent window
		// Respond to a OnNotify in parent and disassemble the message
		//
		NMHDR nm;

		nm.hwndFrom = GetSafeHwnd();
		nm.idFrom  = GetDlgCtrlID();
		nm.code = NM_LINKCLICK;
		::SendMessage(GetParent()->m_hWnd, WM_NOTIFY, nm.idFrom, (LPARAM) &nm);
	}	*/

	
	NMHDR nm;
	
	nm.hwndFrom = GetSafeHwnd();
	nm.idFrom  = GetDlgCtrlID();
	nm.code = NM_LINKCLICK;
	::SendMessage(GetParent()->m_hWnd, WM_CLICKLABEL, nm.idFrom, (LPARAM) &nm);
		
	CStatic::OnLButtonUp(nFlags, point);
}


void CPowLabel::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bUseHandCursor && m_hHandCursor)
		::SetCursor(m_hHandCursor);

	CStatic::OnMouseMove(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// THE FUNCTIONS START HERE :----
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::SetText
// Description:		Short cut to set window text - caption - label
// INPUTS:          Text to use
// RETURNS:         Reference to this
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::SetText(const CString& strText)
{
	//SetWindowText(strText);		// Do Not Use this Method - SeoJeong - 2005/07/10
	m_strText = strText;

	UpdateSurface();

	return *this;
}

void CPowLabel::GetText(CString &strText) const
{ 
	strText = m_strText; 
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::SetTextColor
// Description:		Sets the text color 
// INPUTS:          True or false
// RETURNS:         Reference to 'this' object
// Name                     Date        Version Comments
// NT ALMOND				22/10/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::SetTextColor(COLORREF crText)
{
	if ( crText == m_crText)	return *this;

	m_crText = crText;
	UpdateSurface();

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::SetFontBold
// Description:		Sets the font ot bold 
// INPUTS:          True or false
// RETURNS:         Reference to 'this' object
// NOTES:			
// MODIFICATIONS:
// Name                     Date        Version Comments
// NT ALMOND				22/10/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::SetFontBold(BOOL bBold)
{	
	if ( (bBold ? FW_BOLD : FW_NORMAL) == m_lf.lfWeight)	return *this;

	m_lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
	ReconstructFont();
	UpdateSurface();

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::SetFontUnderline
// Description:		Sets font underline attribue
// INPUTS:          True of false
// RETURNS:         Reference to 'this' object
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::SetFontUnderline(BOOL bSet)
{	
	if ( bSet == m_lf.lfUnderline)	return *this;

	m_lf.lfUnderline = bSet;
	ReconstructFont();
	UpdateSurface();

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::SetFontItalic
// Description:		Sets font italic attribute
// INPUTS:          True of false
// RETURNS:         Reference to 'this' object
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::SetFontItalic(BOOL bSet)
{
	if ( bSet == m_lf.lfItalic)	return *this;

	m_lf.lfItalic = bSet;
	ReconstructFont();
	UpdateSurface();

	return *this;	
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::SetSunken
// Description:		Sets sunken effect on border
// INPUTS:          True of false
// RETURNS:         Reference to 'this' object
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::SetSunken(BOOL bSet)
{

	if (!bSet)
		ModifyStyleEx(WS_EX_STATICEDGE,0,SWP_DRAWFRAME);
	else
		ModifyStyleEx(0,WS_EX_STATICEDGE,SWP_DRAWFRAME);
		
	return *this;	
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::SetBorder
// Description:		Toggles the border on/off
// INPUTS:          True of false
// RETURNS:         Reference to 'this' object
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::SetBorder(BOOL bSet)
{

	if (!bSet)
		ModifyStyle(WS_BORDER,0,SWP_DRAWFRAME);
	else
		ModifyStyle(0,WS_BORDER,SWP_DRAWFRAME);
		
	return *this;	
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::SetFontSize
// Description:		Sets the font size
// INPUTS:          True of false
// RETURNS:         Reference to 'this' object
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::SetFontSize(int nSize)
{

	CFont cf;
	LOGFONT lf;

	cf.CreatePointFont(nSize * 10, m_lf.lfFaceName);
	cf.GetLogFont(&lf);

	m_lf.lfHeight = lf.lfHeight;
	m_lf.lfWidth  = lf.lfWidth;

//	nSize*=-1;
//	m_lf.lfHeight = nSize;
	ReconstructFont();
	UpdateSurface();
	cf.DeleteObject();

	return *this;
}


//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::SetBkColor
// Description:		Sets background color
// INPUTS:          Colorref of background color
// RETURNS:         Reference to 'this' object
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::SetBkColor(COLORREF crBkgnd, COLORREF crBkgndHigh , BackFillMode mode)
{
	if ( crBkgnd == m_crLoColor	&& crBkgndHigh == m_crHiColor && mode == m_fillmode)	return *this;

	m_crLoColor = crBkgnd;
	m_crHiColor = crBkgndHigh;

	m_fillmode = mode;

	if (m_hBackBrush)
		::DeleteObject(m_hBackBrush);

	
	if (m_fillmode == Normal)
	{	
		m_hBackBrush = ::CreateSolidBrush(crBkgnd);
		if(m_hwndBrush)
			::DeleteObject(m_hwndBrush);
		m_hwndBrush = m_hBackBrush;
	}

	UpdateSurface();

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::SetFontName
// Description:		Sets the fonts face name
// INPUTS:          String containing font name
// RETURNS:         Reference to 'this' object
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
// NT ALMOND				15092000	1.5		Support internation windows
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::SetFontName(const CString& strFont, BYTE byCharSet /* Default = ANSI_CHARSET */)
{	
	if ( byCharSet == m_lf.lfCharSet)	return *this;

	m_lf.lfCharSet = byCharSet;

	_tcscpy(m_lf.lfFaceName,strFont);
	ReconstructFont();
	UpdateSurface();

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::FlashText
// Description:		As the function states
// INPUTS:          True or false
// RETURNS:         Reference to 'this' object
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::FlashText(BOOL bActivate)
{

	if (m_bTimer)
		KillTimer(1);

	if (bActivate)
	{
		m_bState = FALSE;
		
		m_bTimer = TRUE;
		
		SetTimer(1,500,NULL);

		m_Type = Text;
	}
	else
		m_Type = None; // Fix

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::FlashBackground
// Description:		As the function states
// INPUTS:          True or false
// RETURNS:         Reference to 'this' object
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::FlashBackground(BOOL bActivate)
{

	if (m_bTimer)
		KillTimer(1);

	if (bActivate)
	{
		m_bState = FALSE;

		m_bTimer = TRUE;
		SetTimer(1,500,NULL);

		m_Type = Background;
	}

	return *this;
}


//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::SetLink
// Description:		Indicates the string is a link
// INPUTS:          True or false
// RETURNS:         Reference to 'this' object
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
// NT ALMOND				26/08/99	1.2		Added flexbility of
//												Sending Click meessage to parent
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::SetLink(BOOL bLink,BOOL bNotifyParent)
{

	m_bLink = bLink;
	m_bNotifyParent = bNotifyParent;

	if (bLink)
		ModifyStyle(0,SS_NOTIFY);
	else
		ModifyStyle(SS_NOTIFY,0);

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::SetLinkCursor
// Description:		Sets the internet browers link
// INPUTS:          Handle of cursor
// RETURNS:         Reference to 'this' object
// Name                     Date        Version Comments
// NT ALMOND				26/08/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::SetLinkCursor(HCURSOR hCursor)
{

	m_hCursor = hCursor;
	return *this;
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::SetTransparent
// Description:		Sets the Label window to be transpaent
// INPUTS:          True or false
// RETURNS:         Reference to 'this' object
// Name                     Date        Version Comments
// NT ALMOND				22/10/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::SetTransparent(BOOL bSet)
{

	m_bTransparent = bSet;
	ModifyStyleEx(0,WS_EX_TRANSPARENT); // Fix for transparency
	UpdateSurface();

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::SetFont3D
// Description:		Sets the 3D attribute of the font.
// INPUTS:          True or false, Raised or Sunken
// RETURNS:         Reference to 'this' object
// Name                     Date        Version Comments
// NT ALMOND				22/10/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::SetFont3D(BOOL bSet,Type3D type)
{

	m_bFont3d = bSet;
	m_3dType = type;
	UpdateSurface();

	return *this;
}

void CPowLabel::OnSysColorChange() 
{

	if (m_hwndBrush)
		::DeleteObject(m_hwndBrush);

	m_hwndBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	
	UpdateSurface();
}



//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::SetRotationAngle
// Description:		Sets the rotation angle for the current font.
// INPUTS:          Angle in Degress
// RETURNS:         Reference to 'this' object
// Name                     Date        Version Comments
// NT ALMOND				22/10/98    1.0     Origin
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::SetRotationAngle(UINT nAngle,BOOL bRotation)
{
	// Arrrrh...
	// Your looking in here why the font is rotating, aren't you?
	// Well try setting the font name to 'Arial' or 'Times New Roman'
	// Make the Angle 180 and set bRotation to true.
	//
	// Font rotation _ONLY_ works with TrueType fonts...
	//
	// 
	m_lf.lfEscapement = m_lf.lfOrientation = (nAngle * 10);
	m_bRotation = bRotation;
	
	ReconstructFont();
	UpdateSurface();

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::SetText3DHiliteColor
// Description:		Sets the 3D font hilite color
// INPUTS:          Color 
// RETURNS:         Reference to 'this' object
// Name                     Date        Version Comments
// NT ALMOND				17/07/00    1.0     Origin
//////////////////////////////////////////////////////////////////////////
CPowLabel& CPowLabel::SetText3DHiliteColor(COLORREF cr3DHiliteColor)
{
	m_cr3DHiliteColor = cr3DHiliteColor;
	UpdateSurface();

	return *this;
}


//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::PreSubclassWindow
// Description:		Assigns default dialog font
// Name                     Date        Version Comments
// NT ALMOND				15092000    1.5     Origin
//////////////////////////////////////////////////////////////////////////
void CPowLabel::PreSubclassWindow() 
{

	CStatic::PreSubclassWindow();

	GetFont()->GetObject(sizeof(m_lf),&m_lf);
	ReconstructFont();

	
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::PreCreateWindow
// Name                     Date        Version Comments
// NT ALMOND				15092000    1.5     Origin
//////////////////////////////////////////////////////////////////////////
BOOL CPowLabel::PreCreateWindow(CREATESTRUCT& cs) 
{	
	return CStatic::PreCreateWindow(cs);
}

//////////////////////////////////////////////////////////////////////////
// Function:		CPowLabel::DrawGradientFill
// Description:		Internal help function to gradient fill background
// Name                     Date        Version Comments
// NT ALMOND				15092000    1.5     Origin
//////////////////////////////////////////////////////////////////////////
void CPowLabel::DrawGradientFill(CDC* pDC, CRect* pRect, COLORREF crStart, COLORREF crEnd, int nSegments)
{
	// Get the starting RGB values and calculate the incremental
	// changes to be applied.

	COLORREF cr;
	int nR = GetRValue(crStart);
	int nG = GetGValue(crStart);
	int nB = GetBValue(crStart);

	int neB = GetBValue(crEnd);
	int neG = GetGValue(crEnd);
	int neR = GetRValue(crEnd);

	if(nSegments > pRect->Width())
		nSegments = pRect->Width();

	int nDiffR = (neR - nR);
	int nDiffG = (neG - nG);
	int nDiffB = (neB - nB);

	int ndR = 256 * (nDiffR) / (max(nSegments,1));
	int ndG = 256 * (nDiffG) / (max(nSegments,1));
	int ndB = 256 * (nDiffB) / (max(nSegments,1));

	nR *= 256;
	nG *= 256;
	nB *= 256;

	neR *= 256;
	neG *= 256;
	neB *= 256;

	int nCX = pRect->Width() / max(nSegments,1), nLeft = pRect->left, nRight;
	pDC->SelectStockObject(NULL_PEN);

	for (int i = 0; i < nSegments; i++, nR += ndR, nG += ndG, nB += ndB)
	{
		// Use special code for the last segment to avoid any problems
		// with integer division.

		if (i == (nSegments - 1))
			nRight = pRect->right;
		else
			nRight = nLeft + nCX;

		cr = RGB(nR / 256, nG / 256, nB / 256);
		
		{
			CBrush br(cr);
			CBrush* pbrOld = pDC->SelectObject(&br);
			pDC->Rectangle(nLeft, pRect->top, nRight + 1, pRect->bottom);
			pDC->SelectObject(pbrOld);
		}

		// Reset the left side of the drawing rectangle.

		nLeft = nRight;
	}
}

BOOL CPowLabel::SignDraw(CWnd *pWnd, int nStyle)
{
	m_nStyle = nStyle;
	CRect rect;

	CStatic *m_static = (CStatic *)pWnd;
	
	m_static->GetClientRect(rect);

	CClientDC dc(m_static);

	if (nStyle == 1)//����
	{
		CPoint polygon[7];
		polygon[0].x = rect.left + rect.Width() / 2 +1;
		polygon[0].y = rect.top;
		
		polygon[1].x = rect.right;
		polygon[1].y = rect.top + rect.Height() / 2;
		
		polygon[2].x = polygon[0].x + 2;
		polygon[2].y = polygon[1].y;
		
		polygon[3].x = polygon[2].x;
		polygon[3].y = rect.bottom - 1;
		
		polygon[4].x = polygon[0].x - 2; 
		polygon[4].y = polygon[3].y;
		
		polygon[5].x = polygon[4].x;
		polygon[5].y = polygon[1].y;
		
		polygon[6].x = rect.left;
		polygon[6].y = polygon[1].y;
		
		CRgn UpLimit;
		UpLimit.CreatePolygonRgn(polygon, 7, WINDING);
		CBrush br;
		br.CreateSolidBrush(RGB(255, 25, 25));
		dc.FillRgn(&UpLimit, &br);
		br.DeleteObject();
		UpLimit.DeleteObject();
	}
	else if (nStyle == 2)
	{
		rect.DeflateRect(1, 2, 1, 2);
		CPoint polygon[3];
		polygon[0].x = rect.left + rect.Width() / 2+1;
		polygon[0].y = rect.top;
		polygon[1].x = rect.right;
		polygon[1].y = rect.bottom;
		polygon[2].x = rect.left;
		polygon[2].y = rect.bottom;
		CRgn UpLimit;
		UpLimit.CreatePolygonRgn(polygon, 3, ALTERNATE);
		CBrush br;
		br.CreateSolidBrush(RGB(255, 25, 25));
		dc.FillRgn(&UpLimit, &br);
		br.DeleteObject();
		UpLimit.DeleteObject();
		
		
	}
	else if (nStyle == 3)//���� 
	{
	}
	else if (nStyle == 5) 
	{
		rect.DeflateRect(1, 2, 1, 2);
		CPoint polygon[3];
		polygon[0].x = rect.left;
		polygon[0].y = rect.top;
		polygon[1].x = rect.right;
		polygon[1].y = rect.top;
		polygon[2].x = rect.left + rect.Width() / 2+1;
		polygon[2].y = rect.bottom;
		CRgn UpLimit;
		UpLimit.CreatePolygonRgn(polygon, 3, ALTERNATE);
		CBrush br;
		br.CreateSolidBrush(RGB(25, 25, 255));
		dc.FillRgn(&UpLimit, &br);
		
		br.DeleteObject();
		UpLimit.DeleteObject();
		
	}
	else if (nStyle == 4)//����
	{
		CPoint polygon[7];
		polygon[0].x = rect.left + rect.Width() / 2 +1;
		polygon[0].y = rect.bottom -1;
		
		polygon[1].x = rect.left;
		polygon[1].y = rect.top + rect.Height() / 2-1;
		
		polygon[2].x = polygon[0].x - 2;
		polygon[2].y = polygon[1].y;
		
		polygon[3].x = polygon[2].x;
		polygon[3].y = rect.top + 1;
		
		polygon[4].x = polygon[0].x + 2; 
		polygon[4].y = polygon[3].y;
		
		polygon[5].x = polygon[4].x;
		polygon[5].y = polygon[1].y;
		
		polygon[6].x = rect.right;
		polygon[6].y = polygon[1].y;
		
		CRgn UpLimit;
		UpLimit.CreatePolygonRgn(polygon, 7, WINDING);
		CBrush br;
		br.CreateSolidBrush(RGB(25, 25, 255));
		dc.FillRgn(&UpLimit, &br);
		br.DeleteObject();
		UpLimit.DeleteObject();			
	}

	return TRUE;
}


//*******************************************
// 	Hand Cursor Load - SeoJeong - 2005/07/25
// ******************************************
void CPowLabel::LoadLibHandCursor()
{
    CString strWndDir;
    GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
    strWndDir.ReleaseBuffer();

    strWndDir += _T("\\winhlp32.exe");
    // This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
    HMODULE hModule = ::LoadLibrary(strWndDir);
    if (hModule) 
	{
        HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
        if (hHandCursor)
            m_hHandCursor = CopyCursor(hHandCursor);
		DestroyCursor(hHandCursor);//KHD
    }
	
    ::FreeLibrary(hModule);
}

