#include "stdafx.h"
#include "fxSpinEx.h"
#include "misctype.h"

#include <math.h>
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	IDC_INPLACE_EDIT	1001
#define	IDC_INPLACE_SPIN	1002


CfxSpinEx::CfxSpinEx(CFont* pFont/*=NULL*/)
{
	m_pFont = pFont;

	m_spType = spNOT;
	m_attr = 0;
	m_pEdit = NULL;
	m_pSpin = NULL;
}

CfxSpinEx::~CfxSpinEx()
{
	if (m_pEdit)
	{
		//m_pEdit->DestroyWindow();
		delete m_pEdit; m_pEdit = NULL;
	}
	if (m_pSpin)
	{
		//m_pSpin->DestroyWindow();
		delete m_pSpin; m_pSpin = NULL;
	}
}


BEGIN_MESSAGE_MAP(CfxSpinEx, CStatic)
	//{{AFX_MSG_MAP(CfxSpinEx)
	ON_WM_ENABLE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(SPNM_ONECHANGE, IDC_INPLACE_EDIT, OnEditChange)
	ON_NOTIFY(SPNM_ONSCHANGE, IDC_INPLACE_SPIN, OnSpinChange)
END_MESSAGE_MAP()


BOOL CfxSpinEx::Create(DWORD attr, DWORD spType, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_spType = spType;
	m_attr = attr;
	m_parent = pParentWnd;
	DWORD style = WS_CHILD|WS_VISIBLE|WS_TABSTOP;
	CString lpszText = _T("");
	if (CStatic::Create(lpszText, style, rect, pParentWnd, nID) < 0)
		return FALSE;

	createControls();
	return TRUE;
}

void CfxSpinEx::Init(DWORD attr, DWORD spType, CWnd* pParentWnd)
{
	m_spType = spType;
	m_attr = attr;
	m_parent = pParentWnd;
	createControls();
}

void CfxSpinEx::SetAttribute(DWORD attr)
{
	if (attr & attrCOMMA)
		attr &= ~attrNUMERIC;
	m_attr = attr;	
	if (m_pSpin)	m_pSpin->SetAttribute(m_attr);
	if (m_pEdit)	m_pEdit->SetAtrribute(m_attr);
}

void CfxSpinEx::SetSpinType(DWORD remove, DWORD add)
{
	m_spType &= ~remove;
	m_spType |= add;

	
	if (!m_pSpin)	return;
	if (m_spType & (spFUTURE | spOPTION | spINDEX))
	{
		m_attr |= attrFLOAT;
		m_attr &= ~attrCOMMA;
		m_attr &= ~attrNUMERIC;
		SetDecimalPlaces(2);
		m_pSpin->SetAttribute(m_attr);
		m_pEdit->SetAtrribute(m_attr);
	}
	else if (m_spType & (spKOSPI | spSOPT | spKOSDAQ | spETFKOSPI | spETFKOSDAQ | spNUMBER | spELW))
	{
		m_attr |= attrCOMMA;
		m_attr &= ~attrFLOAT;
		m_attr &= ~attrNUMERIC;
		SetDecimalPlaces(0);
		m_pSpin->SetAttribute(m_attr);
		m_pEdit->SetAtrribute(m_attr);
	}
	
	m_pSpin->SetSpinType(m_spType);
}

void CfxSpinEx::SetColors(COLORREF crBack, COLORREF crFont, bool redraw)
{
	if (m_pEdit)
	{
		m_pEdit->SetColors(crBack, crFont, redraw);
	}
}

void CfxSpinEx::SetDecimalPlaces(int digit, bool redraw)
{
	if (m_pSpin)	m_pSpin->SetDecimalPlaces(digit);
	if (m_pEdit)	m_pEdit->SetDecimalPlaces(digit);
	if (redraw)	Invalidate();
}

void CfxSpinEx::OnEnable(BOOL bEnable) 
{
	if(m_pEdit)
		m_pEdit->EnableWindow(bEnable);
	if(m_pSpin)
		m_pSpin->EnableWindow(bEnable);
	this->InvalidateRect(NULL);
}

void CfxSpinEx::OnSetFocus(CWnd* pOldWnd) 
{
	if (m_pEdit)
	{
		m_pEdit->SetFocus();
		m_pEdit->SetSel(0, -1);
	}
}

void CfxSpinEx::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	if (cx == 0 && cy == 0)		return;
	if (!IsWindow(GetSafeHwnd()))	return;
	int swid = GetSystemMetrics(SM_CYHSCROLL);
	int iWidth = cx - swid;
	if (m_pEdit) m_pEdit->MoveWindow(0, 0, iWidth, cy);
	if (m_pSpin) m_pSpin->MoveWindow(iWidth, 0, cx-iWidth, cy);	
	
}

void CfxSpinEx::OnEditChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (!IsWindow(GetSafeHwnd()))
		return;
	if (pNMHDR->code == SPNM_ONECHANGE)
	{
		pNMHDR->code = SPNM_ONCHANGE;
		CWnd* pOwner = GetOwner();
		if (pOwner) pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)pNMHDR);
	}
}

void CfxSpinEx::OnSpinChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (!IsWindow(GetSafeHwnd()))
		return;
	if (pNMHDR->code == SPNM_ONSCHANGE)
	{
		pNMHDR->code = SPNM_ONCHANGE;
		CWnd* pOwner = GetOwner();
		if (pOwner) pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)pNMHDR);
	}
}


void CfxSpinEx::createControls()
{
	CRect rect(0,0,0,0), erect(0,0,0,0), srect(0,0,0,0);
	GetClientRect(rect);
	int nCtrlID = this->GetDlgCtrlID();
	erect = srect = rect;

	int swid = GetSystemMetrics(SM_CYHSCROLL);
	erect.right = rect.right - swid;
	srect.left = rect.right - swid;
	if (!m_pEdit) 
	{
		m_pEdit = new CSpinEditEx(this, m_attr);
		m_pEdit->CreateEx(WS_EX_CLIENTEDGE ,_T("EDIT"), "", 
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL |ES_RIGHT ,  //| WS_BORDER
			erect, this, IDC_INPLACE_EDIT); 
		if (m_pFont != NULL) m_pEdit->SetFont(m_pFont);
	}
	else
		return;

	if (!m_pSpin) 
	{
		srect.InflateRect(-1, 1);
		m_pSpin = new CSpinCtrlEx(this, m_attr, m_spType);
		m_pSpin->Create(WS_CHILD|WS_VISIBLE|UDS_AUTOBUDDY|UDS_ARROWKEYS, srect, this, IDC_INPLACE_SPIN);
	}
	else 
		return;	
	
	if (m_spType & spCOLOR)
		m_pEdit->SetUseColor(true);
	
	SetAttribute(m_attr);
	SetSpinType(0, m_spType);

	m_pSpin->SetBuddy(m_pEdit);	
	if (m_pFont != NULL) m_pEdit->SetFont(m_pFont);
	m_pSpin->SetRange32((int)INT_MIN, (int)INT_MAX);
	m_pSpin->SetPos(0);
}

void CfxSpinEx::SetPos(double nPos)
{
	if (m_pSpin)
		m_pSpin->SetPos(nPos);	
}

double CfxSpinEx::GetPos()
{
	if (!m_pSpin)
		return 0.0;
	return m_pSpin->GetPos();
}

void CfxSpinEx::SetDirection(BOOL nFlag)
{
	if (nFlag)
		m_pSpin->SetDirection(1);
	else
		m_pSpin->SetDirection(-1);
}

void CfxSpinEx::SetRangeEx(double lower, double upper)
{
	if (!m_pSpin || !m_pEdit)
		return;
	if (lower == UNUSEDVAL)
		lower = 0;
	if (upper ==UNUSEDVAL)
		upper = INT_MAX;
	m_pSpin->SetRange32((int)lower, (int)upper);
	m_pEdit->SetRange(lower, upper);

}

void CfxSpinEx::GetRangeEx(double &lower, double &upper)
{
	if (!m_pSpin)	return;
	int low, upp;
	m_pSpin->GetRange32(low, upp);
	lower = (double)low;
	upper = (double)upp;
}

void CfxSpinEx::SetDelta(double delta, bool redraw)
{
	if (!m_pSpin)	return;
	m_pSpin->SetDelta(delta);
	if (redraw)
		Invalidate();
}

double CfxSpinEx::GetDelta()
{
	if (!m_pSpin)	return 0.0;
	return m_pSpin->GetDelta();
}

void CfxSpinEx::SetText(CString text)
{
	if (m_pEdit)
		m_pEdit->SetText(text);
}

CString CfxSpinEx::GetText()
{
	CString text = _T("");
	if (m_pEdit)
		text = m_pEdit->GetText();
	return text;
}

void CfxSpinEx::SetLimitText(int len)
{
	if (m_pEdit)	m_pEdit->SetLimitText(len);
}

/////////////////////////////////////////////////////////////////////
// Spin Button
CSpinCtrlEx::CSpinCtrlEx(CWnd* pWnd, DWORD attr, DWORD spType)
{
	m_pWnd = pWnd;
	m_spType = spType;
	m_attr = attr;
	m_direction = 1;	
	m_digit = 0;
	m_delta = 1;
}

CSpinCtrlEx::~CSpinCtrlEx()
{
}

BEGIN_MESSAGE_MAP(CSpinCtrlEx, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CSpinCtrlEx)
	ON_NOTIFY_REFLECT(UDN_DELTAPOS, OnDeltapos)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CSpinCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSpinButtonCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetAttribute(m_attr);
	
	return 0;
}

void CSpinCtrlEx::SetSpinType(DWORD spType, bool redraw)
{
	m_spType = spType;
	if (redraw)
		Invalidate();
}

void CSpinCtrlEx::SetAttribute(DWORD attr, bool redraw)
{
	m_attr = attr;

	if (m_attr & attrCOMMA)
		ModifyStyle(0, UDS_NOTHOUSANDS);
	else
		ModifyStyle(UDS_NOTHOUSANDS, 0);

	if (m_attr & attrFLOAT)
	{
		ModifyStyle(UDS_NOTHOUSANDS, 0);
	}

	if (redraw)
		Invalidate();
}

void CSpinCtrlEx::SetDirection(int direction)
{
	m_direction = direction;
}

void CSpinCtrlEx::SetDecimalPlaces(int digit)
{
	m_digit = digit;
}

void CSpinCtrlEx::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	int lower, upper;
	GetRange32(lower, upper);
	CWnd *pWnd = GetBuddy();
	double val = 0;
	double pos = GetPos();
	if (pNMUpDown->iDelta < 0) 
	{
		checkDelta(FALSE);	//down
		val = pos - (m_delta * m_direction);
		if (val > upper || val < lower)
			return;
	}
	else 
	{
		checkDelta(TRUE);	//up
		val = pos + (m_delta * m_direction);		
		if (val > upper || val < lower)
			return;
	}
	SetPos(val);	

	NMHDR nmhdr;
	nmhdr.hwndFrom = GetSafeHwnd();
	nmhdr.idFrom   = GetDlgCtrlID();
	nmhdr.code     = SPNM_ONSCHANGE;
	if (m_pWnd) m_pWnd->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmhdr);	
	*pResult = 0;
}

void CSpinCtrlEx::SetPos(double nPos)
{
	CWnd* pWnd = GetBuddy();
	if (!pWnd)	return;
	CString tmpS;
	tmpS.Format("%.*f", m_digit, nPos);
	if (m_attr & attrCOMMA)
		formatComma(tmpS);
	
	pWnd->SetWindowText(tmpS);
}

double CSpinCtrlEx::GetPos()
{
	CWnd* pWnd = GetBuddy();
	if (!pWnd)	return 0.0;
	
	CString text;
	pWnd->GetWindowText(text);
	text.Remove(',');
	return atof(text);
}

void CSpinCtrlEx::formatComma(CString& text)
{
	if (m_attr & attrCOMMA && !text.IsEmpty())
	{
		CString tmpS = _T("");
		text.Remove(',');
		text.MakeReverse();
		for (int ii = 0; ii < text.GetLength(); ii++)
		{
			if (ii && !(ii % 3))
				tmpS += ',';
			tmpS += text[ii];
		}
		tmpS.MakeReverse();
		text = tmpS;
	}
}


void CSpinCtrlEx::checkDelta(BOOL nFlag)
{
	CWnd *pWnd = GetBuddy();
	if (!pWnd)	return;

	CString tmpS;
	pWnd->GetWindowText(tmpS);
	tmpS.Remove(',');
	double	spVal = atof(tmpS);
	if (m_spType & spKOSPI)
	{
		if(nFlag)
		{
			if (spVal < 1000)
				m_delta = 1;
			else if (spVal >= 1000 && spVal < 5000)
				m_delta = 5;
			else if (spVal >= 5000 && spVal < 10000)
				m_delta = 10;
			else if (spVal >= 10000 && spVal < 50000)
				m_delta = 50;
			else if (spVal >= 50000 && spVal < 100000)
				m_delta = 100;
			else if (spVal >= 100000 && spVal < 500000)
				m_delta = 500;
			else
				m_delta = 1000;
		}
		else
		{
			if (spVal <= 1000)
				m_delta = 1;
			else if (spVal > 1000 && spVal <= 5000)
				m_delta = 5;
			else if (spVal > 5000 && spVal <= 10000)
				m_delta = 10;
			else if (spVal > 10000 && spVal <= 50000)
				m_delta = 50;
			else if (spVal > 50000 && spVal <= 100000)
				m_delta = 100;
			else if (spVal > 100000 && spVal <= 500000)
				m_delta = 500;
			else
				m_delta = 1000;
		}

	}
	else if (m_spType & spKOSDAQ)
	{
		if(nFlag)
		{
			if (spVal < 1000)
				m_delta = 1;
			else if (spVal >= 1000 && spVal < 5000)
				m_delta = 5;
			else if (spVal >= 5000 && spVal < 10000)
				m_delta = 10;
			else if (spVal >= 10000 && spVal < 50000)
				m_delta = 50;
			else
				m_delta = 100;
		}
		else
		{
			if (spVal <= 1000)
				m_delta = 1;
			else if (spVal > 1000 && spVal <= 5000)
				m_delta = 5;
			else if (spVal > 5000 && spVal <= 10000)
				m_delta = 10;
			else if (spVal > 10000 && spVal <= 50000)
				m_delta = 50;
			else
				m_delta = 100;
		}
	}
	else if (m_spType & spETFKOSPI)
	{
		if(nFlag)
		{
			if (spVal < 5000)
				m_delta = 5;
			else if (spVal >= 5000 && spVal < 50000)
				m_delta = 10;
			else if (spVal >= 50000 && spVal < 100000)
				m_delta = 100;
			else if (spVal >= 100000 && spVal < 500000)
				m_delta = 500;
			else
				m_delta = 1000;
		}
		else
		{
			if (spVal <= 5000)
				m_delta = 5;
			else if (spVal > 5000 && spVal <= 50000)
				m_delta = 10;
			else if (spVal > 50000 && spVal <= 100000)
				m_delta = 100;
			else if (spVal > 100000 && spVal <= 500000)
				m_delta = 500;
			else
				m_delta = 1000;
		}
	}
	else if (m_spType & spETFKOSDAQ)
	{
		if(nFlag)
		{
			if (spVal < 5000)
				m_delta = 5;
			else if (spVal >= 5000 && spVal < 10000)
				m_delta = 10;
			else if (spVal >= 10000 && spVal < 50000)
				m_delta = 50;
			else
				m_delta = 100;
		}
		else
		{
			if (spVal <= 5000)
				m_delta = 5;
			else if (spVal > 5000 && spVal <= 10000)
				m_delta = 10;
			else if (spVal > 10000 && spVal <= 50000)
				m_delta = 50;
			else
				m_delta = 100;
		}
	}
	else if (m_spType & spFUTURE)
	{
		m_delta = 0.05;
	}
	else if (m_spType & spOPTION)
	{
		if(nFlag) 
		{
			if (spVal < 3)
				m_delta = 0.01;		
			else
				m_delta = 0.05;
		} 
		else 
		{
			if (spVal <= 3)
				m_delta = 0.01;		
			else
				m_delta = 0.05;
		}
	}
	else if (m_spType & spSOPT)
	{
		if (spVal < 1000)
			m_delta = 10;
		else if (spVal >= 1000 && spVal < 2000)
			m_delta = 20;
		else if (spVal >= 2000 && spVal < 5000)
			m_delta = 50;
		else if (spVal >= 5000 && spVal < 10000)
			m_delta = 100;
		else
			m_delta = 200;
	}
	else if (m_spType & spINDEX)
	{
		m_delta = 0.01;
	}
	else if (m_spType & spNUMBER)
	{
		m_delta = 1;
	}
	else if (m_spType & spKFUTURE)
	{
		m_delta = 0.5;
	}
	else if (m_spType & spSFUTURE)
	{
		if(nFlag) 
		{
			if (spVal < 5000)
				m_delta = 2;
			else if (spVal >= 5000 && spVal < 10000)
				m_delta = 5;
			else if (spVal >= 10000 && spVal < 50000)
				m_delta = 25;
			else if (spVal >= 50000 && spVal < 100000)
				m_delta = 50;
			else if (spVal >= 100000 && spVal < 500000)
				m_delta = 250;
			else
				m_delta = 500;
		}
		else
		{
			if (spVal <= 5000)
				m_delta = 2;
			else if (spVal > 5000 && spVal <= 10000)
				m_delta = 5;
			else if (spVal > 10000 && spVal <= 50000)
				m_delta = 25;
			else if (spVal > 50000 && spVal <= 100000)
				m_delta = 50;
			else if (spVal > 100000 && spVal <= 500000)
				m_delta = 250;
			else
				m_delta = 500;
		}
	}
	else if (m_spType & spELW)
	{
		if(nFlag)
		{
			if (spVal < 5000)
				m_delta = 5;
			else if (spVal >= 5000 && spVal < 50000)
				m_delta = 10;
			else if (spVal >= 50000 && spVal < 100000)
				m_delta = 100;
			else if (spVal >= 100000 && spVal < 500000)
				m_delta = 500;
			else
				m_delta = 1000;
		}
		else
		{
			if (spVal <= 5000)
				m_delta = 5;
			else if (spVal > 5000 && spVal <= 50000)
				m_delta = 10;
			else if (spVal > 50000 && spVal <= 100000)
				m_delta = 100;
			else if (spVal > 100000 && spVal <= 500000)
				m_delta = 500;
			else
				m_delta = 1000;
		}
	}
}

double CSpinCtrlEx::GetDelta()
{
	return m_delta;
}

void CSpinCtrlEx::SetDelta(double delta)
{
	m_delta = delta;
}

/////////////////////////////////////////////////////////////////////
// Spin Edit
CSpinEditEx::CSpinEditEx(CWnd* pWnd, DWORD attr)
{
	m_bColor = false;
	m_pWnd = pWnd;
	m_attr = attr;
	m_clrBACK   = RGB(255, 255, 255);
	m_clrFORE   = RGB(  0,   0,   0);
	m_brBACK.CreateSolidBrush(m_clrBACK);
	m_lower = INT_MIN;
	m_upper = INT_MAX;
	m_digit = 0;
	m_bFocus = false;
}

CSpinEditEx::~CSpinEditEx()
{
	m_brBACK.DeleteObject();
}


BEGIN_MESSAGE_MAP(CSpinEditEx, CEdit)
	//{{AFX_MSG_MAP(CSpinEditEx)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CSpinEditEx::OnNcPaint()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CEdit::OnNcPaint()을(를) 호출하지 마십시오.
	CPaintDC dc(this);
	CRect rc;
	
	GetWindowRect(&rc);
	ScreenToClient(&rc);
	dc.FillSolidRect(&rc, m_clrBACK);
	dc.Draw3dRect(rc, RGB(116,116,116), RGB(116,116,116));
	Invalidate();
}

HBRUSH CSpinEditEx::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	if (m_bColor)
	{
		pDC->SetTextColor(m_clrFORE);
		pDC->SetBkColor(m_clrBACK);
		return m_brBACK;
	}
	return NULL;
}


void CSpinEditEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	bool isHan = false, bDot = false, bMinus = false;
	CString tmpS = _T("");
	
	int jj = 0;
	int sel = -1;

	CString text;
	GetWindowText(text);
	char* tmp = (LPSTR)text.operator LPCTSTR();

	switch (m_attr)
	{
	case attrCOMMA:
		for (jj = 0; jj < text.GetLength(); jj++)
		{
			if (tmp[jj] & 0x80)
			{
				if (sel == -1)
					sel = jj;
				isHan = true;
			}
			else if (tmp[jj] == '-')
			{
				sel = jj;
				bMinus = true;
			}
			else
			{
				tmpS += text.GetAt(jj);
			}
		}
		if ((nChar >= 0x30 && nChar <= 0x39) || nChar == VK_BACK)
		;
		else if (nChar == '-')
		{
			if (bMinus)
				return;
		}
		else if (isHan)
		{
			text = tmpS;
			SetWindowText(text);
			SetSel(sel, sel);
			return;
		}
		else
			return;	
		break;
	case attrFLOAT:
	case attrNUMERIC:
		for (jj = 0; jj < text.GetLength(); jj++)
		{
			if (tmp[jj] & 0x80)
			{
				if (sel == -1)
					sel = jj;
				isHan = true;
			}
			else if (tmp[jj] == '-')
			{
				sel = jj;
				bMinus = true;
			}
			else if (tmp[jj] == '.')
			{
				tmpS += text.GetAt(jj);
			 	bDot = true;
			}
			else
			{
				tmpS += text.GetAt(jj);
			}
		}

		if (bDot && nChar == '.')
			return;
		else if ((nChar >= 0x30 && nChar <= 0x39) 
			|| nChar == VK_BACK 
			|| (!bDot && nChar == '.'))
		;
		else if (nChar == '-')
		{
			if (bMinus)	return;
		}
		else if (isHan)
		{
			text = tmpS;
			SetWindowText(text);
			SetSel(sel, sel);
			return;
		}
		else
			return;	
		break;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
	CString realS;
	GetWindowText(realS);
	
	int pos = realS.Find('-');
	if (pos > 0)
	{
		realS.Remove('-');
		SetWindowText(realS);
		SetSel(realS.GetLength(), realS.GetLength());
	}
		
	if (m_attr & attrCOMMA)
		commaText();
	
	tmpS = GetText();
	
	double val = atof(tmpS);
	if (val < m_lower)
	{
		tmpS.Format("%.f", m_lower);
		SetText(tmpS);
		SetSel(realS.GetLength(), realS.GetLength());
	}
	else if (val > m_upper)
	{
		tmpS.Format("%.f", m_upper);
		SetText(tmpS);
		SetSel(realS.GetLength(), realS.GetLength());
	}
	NMHDR nmhdr;
	nmhdr.hwndFrom = GetSafeHwnd();
	nmhdr.idFrom   = GetDlgCtrlID();
	nmhdr.code     = SPNM_ONECHANGE;
	if (m_pWnd) m_pWnd->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmhdr);
}

void CSpinEditEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	if (m_attr & attrCOMMA)
	{
		switch (nChar)
		{
		case VK_DELETE:
			{
				commaText();
				NMHDR nmhdr;
				nmhdr.hwndFrom = GetSafeHwnd();
				nmhdr.idFrom   = GetDlgCtrlID();
				nmhdr.code     = SPNM_ONECHANGE;
				if (m_pWnd) m_pWnd->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmhdr);
			}
			break;
		}
	}
}



void CSpinEditEx::SetUseColor(bool bColor)
{
	m_bColor = bColor;
}

void CSpinEditEx::SetColors(COLORREF clrBACK, COLORREF clrFORE, bool redraw)
{
	m_clrFORE = clrFORE;
	m_clrBACK = clrBACK;
	m_brBACK.DeleteObject();
	m_brBACK.CreateSolidBrush(m_clrBACK);

	if (redraw)
		Invalidate();
}

void CSpinEditEx::SetRange(double lower, double upper)
{
	m_upper = upper;
	m_lower = lower;
}

void CSpinEditEx::SetText(CString text, bool redraw)
{
	if (m_attr & attrCOMMA)
		formatComma(text);

	SetWindowText(text);
	if (redraw)
		Invalidate();
}

CString CSpinEditEx::GetText()
{
	CString text;
	GetWindowText(text);
	if (m_attr & attrCOMMA)
		text.Remove(',');

	if (m_digit > 0)
	{
		double val = atof(text);
		text.Format("%.*f", m_digit, val);
	}
	return text;
}

void CSpinEditEx::SetAtrribute(DWORD attr, bool redraw)
{
	m_attr = attr;
	if (redraw)
		Invalidate();
}

void CSpinEditEx::formatComma(CString& text)
{
	if (m_attr & attrCOMMA && !text.IsEmpty())
	{
		bool bSign = false;
		CString tmpS = _T("");
		int pos = text.Find('-');
		if (pos >= 0)
		{
			bSign = true;
			text.Remove('-');
		}
		text.Remove(',');
		text.MakeReverse();
		for (int ii = 0; ii < text.GetLength(); ii++)
		{
			if (ii && !(ii % 3))
				tmpS += ',';
			tmpS += text[ii];
		}
		tmpS.MakeReverse();
		text = tmpS;
		if (bSign)
			text = "-" + text;
	}
}

void CSpinEditEx::commaText()
{
	CString tmpS = GetText();
	SetText(tmpS);
	GetWindowText(tmpS);
	SetSel(tmpS.GetLength(), tmpS.GetLength());
}

void CSpinEditEx::OnChange() 
{
	return;
	CString tmpS;
	tmpS = GetText();
	double val = atof(tmpS);
	if (val < m_lower)
	{
		tmpS.Format("%.f", m_lower);
		SetText(tmpS);
	}
	else if (val > m_upper)
	{
		tmpS.Format("%.f", m_upper);
		SetText(tmpS);
	}
}
void CSpinEditEx::SetDecimalPlaces(int digit)
{
	m_digit = digit;
}

void CSpinEditEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	CEdit::OnLButtonDown(nFlags, point);
	
}

void CSpinEditEx::OnKillFocus(CWnd* pNewWnd) 
{
	if (m_bFocus)
		m_bFocus = false;
	CEdit::OnKillFocus(pNewWnd);
}

void CSpinEditEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CEdit::OnLButtonUp(nFlags, point);
	if (!m_bFocus)
	{
		SetSel(0, -1);
		m_bFocus = true;
	}
}
