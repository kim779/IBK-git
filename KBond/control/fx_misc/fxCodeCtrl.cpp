// fxCodeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "fxCodeCtrl.h"
#include "../../h/jmcode.h"
#include "../../h/axisvar.h"
#include <imm.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define COLOR_WHITE	RGB(255, 255, 255)
#define COLOR_PAL_BG	PALETTERGB(27, 79, 129)
#define COLOR_PAL_FG	PALETTERGB(255, 145, 2)


/////////////////////////////////////////////////////////////////////////////
// CListWnd

CSubList::CSubList(class CfxCodeCtrl *pCodeCtrl) 
{ 
	m_pCodeCtrl = pCodeCtrl; 
}

CSubList::~CSubList() 
{ 
}

LRESULT CSubList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_LBUTTONUP)
	{
		m_pCodeCtrl->LButtonUp();
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CCodeCombo

CCodeCombo::CCodeCombo(class CfxCodeCtrl *pParent)
{
	m_pParent = pParent;
	m_DataMode = DM_NONE;

	m_hList = NULL;
}

CCodeCombo::~CCodeCombo()
{
}


BEGIN_MESSAGE_MAP(CCodeCombo, CComboBox)
	//{{AFX_MSG_MAP(CCodeCombo)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCodeCombo message handlers

void CCodeCombo::OnSelchange() 
{
	int	index = GetCurSel();
	_JCode	jCode;
	if (index >= 0)
	{
		if (IsCodeListMode())
		{
			jCode = m_pJCode.GetAt(index);

			if (m_nUnit == GU_CODE || m_nUnit == GU_ELWCODE)
				m_pParent->m_pEdit->ListCode(jCode.Code.Mid(1));
			else
				m_pParent->m_pEdit->ListCode(jCode.Code);			
			//m_pParent->m_pEdit->SetFocus();
		}
		else if (IsHistoryMode())
		{
			jCode = m_pHCode.GetAt(index);
			m_pParent->m_pEdit->ListCode(jCode.Code);
			//m_pParent->m_pEdit->SetFocus();
		}
	}
}

void CCodeCombo::OnDropdown() 
{

}

int CCodeCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetHorizontalExtent(140);
	SetDroppedWidth(140);

	m_pSubList = new CSubList(m_pParent);

	return 0;
}


void CCodeCombo::OnDestroy() 
{
	if (m_pSubList->m_hWnd)
		m_pSubList->UnsubclassWindow();
	delete m_pSubList;

	CComboBox::OnDestroy();
}

HBRUSH CCodeCombo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_LISTBOX:
		{
			if (m_hList == NULL)
			{
				m_hList = pWnd->m_hWnd;
				m_pSubList->SubclassWindow(m_hList);
			}
		}
		break;
	}

	return hbr;
}

void CCodeCombo::SearchCode(CString code)
{
	if (code.IsEmpty())
	{
		SetTopIndex(0);
		SetCurSel(0);
		return;
	}

	code.MakeUpper();

	_JCode	jCode;
	int	len = code.GetLength();

	if (IsCodeListMode())
	{
		int	size = (int)m_pJCode.GetSize();

		if (m_nUnit == GU_CODE || m_nUnit == GU_ELWCODE)
		{
			for (int ii = 0; ii < size; ii++)
			{
				jCode = m_pJCode.GetAt(ii);
				if (jCode.Name.GetLength() < len)
					continue;
				
				if (jCode.Name.Left(len) == code)
				{
					SetTopIndex(ii);
					SetCurSel(ii);
					return;
				}
			}
		}
		else
		{
			for (int ii = 0; ii < size; ii++)
			{
				jCode = m_pJCode.GetAt(ii);
				if (jCode.Code.GetLength() < len)
					continue;
				
				if (jCode.Code.Left(len) == code)
				{
					SetTopIndex(ii);
					SetCurSel(ii);
					return;
				}
			}
		}
	}
	else if (IsHistoryMode())
	{
		int	size = (int)m_pHCode.GetSize();
		for (int ii = 0; ii < size; ii++)
		{
			jCode = m_pHCode.GetAt(ii);
			if (jCode.Code.GetLength() < len)
				continue;
			
			if (jCode.Code.Left(len) == code)
			{
				SetTopIndex(ii);
				SetCurSel(ii);
				return;
			}
		}
	}

	if (GetCurSel() < 0)
	{
		SetTopIndex(0);
		SetCurSel(0);
	}
}

CString CCodeCombo::GetSelectCode()
{
	int	index = GetCurSel();
	if (index < 0)
		return "";

	if (IsCodeListMode())
	{
		_JCode	jCode = m_pJCode.GetAt(index);
		
		if (m_nUnit == GU_CODE || m_nUnit == GU_ELWCODE)
			m_sCode = jCode.Code.Mid(1);
		else
			m_sCode = jCode.Code;
	}
	else if (IsHistoryMode())
	{
		_JCode	jCode = m_pHCode.GetAt(index);

		if (m_nUnit == GU_CODE || m_nUnit == GU_ELWCODE)
			m_sCode = jCode.Code.Mid(1);
		else
			m_sCode = jCode.Code;
	}

	return m_sCode;
}

void CCodeCombo::Key(int key)
{
	int	curIndex = GetCurSel();

	if (curIndex < 0)	return;

	CRect	cRC;
	int	count = 9;
	if (m_pSubList)
	{
		m_pSubList->GetClientRect(&cRC);
		int	iHeight = GetItemHeight(curIndex);
		count = cRC.Height() / iHeight;
	}

	int	topIndex = GetTopIndex();
	int	endIndex = topIndex+count-1;
	int	index = curIndex;
	bool	bSetTopIndex = false;
	switch (key)
	{
	case VK_UP:	// up arrow
		index = max(0, index-1);
		if (index < topIndex)	topIndex = index;
		break;
	case VK_DOWN:	// down arrow
		index = min(GetCount()-1, index+1);
		if (index > endIndex)	topIndex += 1;
		bSetTopIndex = true;
		break;
	case VK_PRIOR:	// pgdn
		index = max(0, index-count);
		topIndex = index;
		break;
	case VK_NEXT:	// pgup
		index = min(GetCount()-1, index+count);
		topIndex = min(GetCount()-1 - (count-1), index - (count-1));
		bSetTopIndex = true;
		break;
	case VK_HOME:	// home
		index = 0;
		topIndex = index;
		break;
	case VK_END:	// end
		index = GetCount()-1;
		topIndex = min(GetCount()-1 - (count-1), index - (count-1));
		break;
	default:
		return;
	}

	if (index >= 0 && index < GetCount())
	{
		SetCurSel(index);
		if (bSetTopIndex)	SetTopIndex(topIndex);
	}
}

void CCodeCombo::InitCodeList()
{
	ResetContent();

	_JCode	jCode;
	CString	Str;
	for (int ii = 0; ii < m_pJCode.GetSize(); ii++)
	{
		jCode = m_pJCode.GetAt(ii);
		if (!jCode.Code.GetLength())
			continue;
		
		if (m_nUnit == GU_CODE || m_nUnit == GU_ELWCODE)
			Str.Format("%s  %s", jCode.Code.Mid(1), jCode.Name);
		else
			Str.Format("%s  %s", jCode.Code, jCode.Name);

		AddString(Str);
	}
	m_DataMode = DM_CODELIST;
}

void CCodeCombo::InitHistory(CString history)
{
	ResetContent();

	m_pHCode.RemoveAll();
	_JCode	jCode;
	CString	Str;
	int	index;
	for (;;)
	{
		index = history.Find("\t");
		if (index <= 0)
			break;

		Str = history.Left(index);
		history = history.Mid(index + 1);

		index = Str.Find(" ");
		if (index > 0)
		{
			jCode.Code = Str.Left(index);
			jCode.Name = Str.Mid(index + 1);
			m_pHCode.Add(jCode);
		}
	}
	m_pHCode.QuickSort(TRUE);

	for (int ii = 0; ii < m_pHCode.GetSize(); ii++)
	{
		jCode = m_pHCode.GetAt(ii);
		Str.Format("%s  %s", jCode.Code, jCode.Name);
		AddString(Str);
	}
	m_DataMode = DM_HISTORY;
}

bool CCodeCombo::IsHistoryMode() 
{ 
	return (m_DataMode == DM_HISTORY)? true:false; 
}

bool CCodeCombo::IsCodeListMode()
{
	return (m_DataMode == DM_CODELIST)? true:false; 
}

bool CCodeCombo::JCodeLoad(CString tabPath)
{
// 	if (m_pJCode.GetSize() > 0)
// 		return true;

	char	buf[1024];
	int	bufsize;
	DWORD	dw;
	int	res;
	struct  hjcode  *pHJCode;

	CString path = tabPath + "\\" + HJCODE;
  	HANDLE fh = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		CString	msg; msg.Format("[%s] file open error!!", path);
		AfxMessageBox(msg); CloseHandle(fh);
		return false;
	}

	struct	_JCode	jCode;
	bufsize = sizeof(struct hjcode);
	for (int ii = 0; ;ii++)
	{
		ZeroMemory(buf, sizeof(buf));
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;

		pHJCode = (struct hjcode *)buf;

		jCode.Code.Empty();

		jCode.Name = CString(pHJCode->hnam, HNameLen);
		jCode.Name.MakeUpper();
		jCode.Name.TrimRight();

		//fname = jCode.Name.GetAt(0);
		//jCode.Name = jCode.Name.Mid(1);

		switch ((char)pHJCode->kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
		default:
// updateX_20060209
			jCode.Code = CString(pHJCode->code, HCodeLen);
			jCode.Code.MakeUpper();
			jCode.Code.TrimRight();
			break;
		}
		if (!jCode.Code.IsEmpty())
			m_pJCode.Add(jCode);
	}
  	CloseHandle(fh);

	m_pJCode.QuickSort();
	return true;
}

bool CCodeCombo::ELWCodeLoad(CString tabPath)
{
// 	if (m_pJCode.GetSize() > 0)
// 		return true;

	char	buf[1024];
	int	bufsize;
	DWORD	dw;
	int	res;
	struct  hjcode  *pHJCode;

	CString path = tabPath + "\\" + HJCODE;
  	HANDLE fh = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		CString	msg; msg.Format("[%s] file open error!!", path);
		AfxMessageBox(msg); CloseHandle(fh);
		return false;
	}

	struct	_JCode	jCode;
	bufsize = sizeof(struct hjcode);
	for (int ii = 0; ;ii++)
	{
		ZeroMemory(buf, sizeof(buf));
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;

		pHJCode = (struct hjcode *)buf;

			continue;

		jCode.Name = CString(pHJCode->hnam, HNameLen);
		jCode.Name.MakeUpper();
		jCode.Name.TrimRight();

		//fname = jCode.Name.GetAt(0);
		//jCode.Name = jCode.Name.Mid(1);

		if (!jCode.Code.IsEmpty())
		{
			jCode.Code = jCode.Code.Mid(1);
			m_pJCode.Add(jCode);
		}
	}
  	CloseHandle(fh);

	m_pJCode.QuickSort();
	return true;
}

bool CCodeCombo::FCodeLoad(CString tabPath)
{
// 	if (m_pJCode.GetSize() > 0)
// 		return true;

	char	buf[1024];
	int	res;
	DWORD	dw;
	int	bufsize;
	struct	fjcode	*pFjCode;
	
	CString path = tabPath + "\\" + FJCODE;
	HANDLE fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		CString	msg; msg.Format("[%s] file open error!!", path);
		AfxMessageBox(msg); CloseHandle(fh);
		return false;
	}
	
	struct	_JCode	fCode;
	bufsize = sizeof(struct fjcode);
	for (int ii = 0; ; ii++)
	{
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;

		pFjCode = (struct fjcode *)buf;

		fCode.Code = CString(pFjCode->cod2, FCodeLen);
		fCode.Code.TrimRight();
		fCode.Name = CString(pFjCode->hnam, FNameLen);
		fCode.Name.TrimRight();
		m_pJCode.Add(fCode);
	}

	CloseHandle(fh);

	m_pJCode.QuickSort();
	return true;
}

bool CCodeCombo::SFCodeLoad(CString tabPath)
{
// 	if (m_pJCode.GetSize() > 0)
// 		return true;

	char	buf[1024];
	int	res;
	DWORD	dw;
	int	bufsize;
	struct	sfcode	*pFjCode;
	
	CString path = tabPath + "\\" + "sfcode.dat";
	HANDLE fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		CString	msg; msg.Format("[%s] file open error!!", path);
		AfxMessageBox(msg); CloseHandle(fh);
		return false;
	}
	
	struct	_JCode	fCode;
	bufsize = sizeof(struct sfcode);
	for (int ii = 0; ; ii++)
	{
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;

		pFjCode = (struct sfcode *)buf;

		fCode.Code = CString(pFjCode->codx, 8);
		fCode.Code.TrimRight();
		fCode.Name = CString(pFjCode->hnam, 50);
		fCode.Name.TrimRight();
		m_pJCode.Add(fCode);
	}

	CloseHandle(fh);

	m_pJCode.QuickSort();
	return true;
}

bool CCodeCombo::OCodeLoad(CString tabPath)
{
// 	if (m_pJCode.GetSize() > 0)
// 		return true;

	char	buf[1024];
	DWORD	dw;
	int	bufsize;
	struct	ojcode	*pOjCode;
	
	CString	path = tabPath + "\\" + OJCODE;
	HANDLE	fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		CString	msg; msg.Format("[%s] file open error!!", path);
		AfxMessageBox(msg); CloseHandle(fh);
		return false;
	}

	bufsize = sizeof(struct ojcodh);
	ZeroMemory(buf, sizeof(buf));
	ReadFile(fh, buf, bufsize, &dw, NULL);
	if (bufsize != (int)dw)
	{
		CloseHandle(fh);
		return false;
	}
	
	bufsize = sizeof(struct ojcode);
	ZeroMemory(buf, sizeof(buf));
	int	AtmIdx = -1;
	CString	tmp;
	int	res;
	
	for (int ii = 0; ; ii++)
	{
		ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;

		pOjCode = (struct ojcode *)buf;
		
		tmp = CString(pOjCode->price, OPriceLen);
		oCode.Price.Format("%.2f", float(atoi(tmp) / 100.0));
		oCode.Atm = pOjCode->atmg;
		if (pOjCode->atmg == 1)
			AtmIdx = ii;
		
		for (int jj = 0; jj < 4; jj++)
		{
			oCode.Call[jj].Exist = pOjCode->call[3-jj].yorn;
			oCode.Call[jj].Code = CString(pOjCode->call[3-jj].cod2, OCodeLen);
			oCode.Call[jj].Name = CString(pOjCode->call[3-jj].hnam, ONameLen);
			res = oCode.Call[jj].Name.Find('\0');
			if (res != -1)
				oCode.Call[jj].Name = oCode.Call[jj].Name.Left(res);
			oCode.Call[jj].Name.TrimRight();
			
			oCode.Put[jj].Exist = pOjCode->put[jj].yorn;
			oCode.Put[jj].Code = CString(pOjCode->put[jj].cod2, OCodeLen);
			oCode.Put[jj].Name = CString(pOjCode->put[jj].hnam, ONameLen);
			res = oCode.Put[jj].Name.Find('\0');
			if (res != -1)
				oCode.Put[jj].Name = oCode.Put[jj].Name.Left(res);
			oCode.Put[jj].Name.TrimRight();

			struct _JCode tOCode;
			if (oCode.Call[jj].Exist == '1')
			{
				tOCode.Code = oCode.Call[jj].Code;
				tOCode.Name = oCode.Call[jj].Name;
				m_pJCode.InsertAt(0, tOCode);
			}
			if (oCode.Put[jj].Exist == '1')
			{
				tOCode.Code = oCode.Put[jj].Code;
				tOCode.Name = oCode.Put[jj].Name;
				m_pJCode.InsertAt(0, tOCode);
			}
		}
	}

	CloseHandle(fh);
	m_pJCode.QuickSort();
	return true;
}

bool CCodeCombo::IsValidCode(CString sCode)
{
	if (sCode.IsEmpty())
		return false;
	
// updateX_20060209
	CString	Acode, Jcode;
	Acode = "A" + sCode; Jcode = "J" + sCode;

	_JCode	jCode;
	for (int ii = 0; ii < m_pJCode.GetSize(); ii++)
	{
		jCode = m_pJCode.GetAt(ii);

		if (!Acode.Compare(jCode.Code) || !Jcode.Compare(jCode.Code))
			return true;
	}

	return false;
}

void CCodeCombo::SetUpData(CString code)
{

	code.MakeUpper();

	_JCode	jCode;
	int	len = code.GetLength();

	int	size = (int)m_pJCode.GetSize();

	for (int ii = 0; ii < size; ii++)
	{
		jCode = m_pJCode.GetAt(ii);
		if (jCode.Code.GetLength() < len)
			continue;
			
		if (jCode.Code.Left(len) == code && ii != size)
		{
			jCode = m_pJCode.GetAt(ii+1);

			if(jCode.Code.Left(5) == code.Left(5))
			{
				m_pParent->m_pEdit->ListCode(jCode.Code);			
				//m_pParent->m_pEdit->SetFocus();
			}

			return;
		}
	}
}

void CCodeCombo::SetDownData(CString code)
{
	code.MakeUpper();

	_JCode	jCode;
	int	len = code.GetLength();

	int	size = (int)m_pJCode.GetUpperBound();

	//for (int ii = 0; ii < size; ii++)
	for (int ii = size; ii >= 0; ii--)
	{
		jCode = m_pJCode.GetAt(ii);
		if (jCode.Code.GetLength() < len)
			continue;
			
		if (jCode.Code.Left(len) == code && ii != 0)
		{
			jCode = m_pJCode.GetAt(ii-1);

			if(jCode.Code.Left(5) == code.Left(5))
			{
				m_pParent->m_pEdit->ListCode(jCode.Code);			
				//m_pParent->m_pEdit->SetFocus();
			}

			return;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CCodeEdit

CCodeEdit::CCodeEdit(CfxCodeCtrl *pCombo, CWnd* pTarget)
{
	m_pParent = pCombo;
	m_pTarget = pTarget;

	m_Unit        = GU_NONE;
	m_preCode     = _T("");
	m_bInitFocus  = true;
	m_bKeydown    = false;
	m_bValidCheck = true;
}

CCodeEdit::~CCodeEdit()
{
}


BEGIN_MESSAGE_MAP(CCodeEdit, CEdit)
	//{{AFX_MSG_MAP(CCodeEdit)
	ON_WM_CHAR()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCodeEdit message handlers

void CCodeEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int	len;
	CString tmpS = _T("");
	GetWindowText(tmpS);

	if (tmpS.IsEmpty() && nChar >= '0' && nChar <= '9')
		ChangeHangulMode(this->m_hWnd, false);

	switch (nChar)
	{
	case VK_RETURN:
		len = tmpS.GetLength();
		if (len <= 0)
		{
			if (len == 0)
				m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
			return;
		}

		if (m_Unit == GU_CODE || m_Unit == GU_ELWCODE)
		{
			if (!isElwNumeric(tmpS))
			{
				CString	tmpS = m_pParent->m_pCombo->GetSelectCode();
				if (m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(FALSE);
				SetWindowText(tmpS);					
			}
			else
			{
				m_pParent->m_pCombo->m_DataMode = 0;
// updateX_20060205
				if (len < 6)
				{
					CString	string; string.Format("%06d", atoi(tmpS)); SetWindowText(string); SetSel(0, -1);
				}
			}
			m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
			SetSel(0, -1);
			return;
		}
		else if (m_Unit == GU_FUTURE || m_Unit == GU_OPTION || m_Unit == GU_FOCODE || m_Unit == GU_POPTION)
		{
			if (m_Unit == GU_FUTURE && (len == 5 && tmpS.GetAt(0) == '1'))
			{
				tmpS += "000";
				SetWindowText(tmpS);
				SetSel(0, -1);
				break;
			}

			GetWindowText(tmpS);
			if (!tmpS.IsEmpty())
			{
				if (isHexNumeric(tmpS))
				{
					CString	tmpS = m_pParent->m_pCombo->GetSelectCode();
					if (m_pParent->m_pCombo->GetDroppedState())
						m_pParent->m_pCombo->ShowDropDown(FALSE);
					SetWindowText(tmpS);					
				}
				m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
				SetSel(0, -1);
			}
		}
		else
		{
			m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
		}
		break;
	case VK_TAB:
		//m_pTarget->SendMessage(GEV_INPUT, MAKEWPARAM(inputChange, ipTab), m_Type);
		break;
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CCodeEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_bKeydown = true;
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCodeEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
	//TRACE("%ld", nFlags);
	/*if (!m_bKeydown)
		return;
	m_bKeydown = false;*/
	if (nFlags == 0x8000)
		return;
	int len;
	if (nChar != VK_RETURN)
	{
		CString	sTmp = _T("");
		GetWindowText(sTmp);
		
		len = sTmp.GetLength();

		if (len <= 0)
		{
			m_preCode = sTmp;
			return;
		}
		
		if (sTmp == m_preCode)
			return;
		
		m_preCode = sTmp;
	
		switch (m_Unit)
		{
		case GU_CODE:
		case GU_ELWCODE:
			if (!isElwNumeric(sTmp))
			{
				m_pParent->CodeListMode();
				if (!m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(TRUE);
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
				m_pParent->m_pCombo->SearchCode(sTmp);
				
				return;
			}
			if ((len == sz_JCODE && m_bValidCheck 
				&& (m_pParent->m_pCombo->IsValidCode(sTmp)) || (!m_bValidCheck && len >= sz_JCODE)))
			{
				GetParent()->SetWindowText(sTmp);
				m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
				SetSel(0, -1);
				if (m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(FALSE);
			}
			else if (len >= sz_JCODE)
			{
				GetParent()->SetWindowText(sTmp);
				m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
				SetSel(0, -1);
				if (m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(FALSE);
			}
			break;
		case GU_FUTURE: 
			if (!isHexNumeric(sTmp))
			{
				m_pParent->CodeListMode();
				if (!m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(TRUE);
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
				m_pParent->m_pCombo->SearchCode(sTmp);
				
				return;
			}
			if (len == 5 && sTmp.GetAt(0) == '1')
			{
				sTmp += "000";
				SetWindowText(sTmp);
				SetSel(0, -1);
				len = 8;
			}
			if (len == sz_FCODE)
			{
				GetParent()->SetWindowText(sTmp);
				m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
				SetSel(0, -1);
				if (m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(FALSE);
			}
			break;
		case GU_OPTION:
			if (!isHexNumeric(sTmp))
			{
				m_pParent->CodeListMode();
				if (!m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(TRUE);
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
				m_pParent->m_pCombo->SearchCode(sTmp);
				
				return;
			}
		case GU_POPTION:
			if (len == sz_OPTION)
			{
				GetParent()->SetWindowText(sTmp);
				m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
				SetSel(0, -1);
				if (m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(FALSE);
			}
			break;
		case GU_FOCODE:
			if (isHexNumeric(sTmp))
			{
				m_pParent->CodeListMode();
				if (!m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(TRUE);
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
				m_pParent->m_pCombo->SearchCode(sTmp);
				
				return;
			}
			if (len == sz_FOCODE)
			{
				GetParent()->SetWindowText(sTmp);
				m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
				SetSel(0, -1);
				if (m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(FALSE);
			}
			break;
		}
	}
}

void CCodeEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CEdit::OnLButtonDown(nFlags, point);
	
	SetSel(0, -1);
}

BOOL CCodeEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	int	key = (zDelta > 0)? VK_UP:VK_DOWN;
	if (m_pParent->m_pCombo->GetDroppedState() && 
		(m_Unit == GU_CODE || m_Unit == GU_FUTURE || m_Unit == GU_OPTION || m_Unit == GU_FOCODE || m_Unit == GU_POPTION || m_Unit == GU_ELWCODE))
	{
		m_pParent->m_pCombo->Key(key);
	}
	
	return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}

void CCodeEdit::OnSetFocus(CWnd* pOldWnd) 
{
	if (m_bInitFocus && (m_Unit == GU_CODE || m_Unit == GU_ELWCODE))
	{
		//m_bInitFocus = false;
		ChangeHangulMode(this->m_hWnd);
	}

	m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SETFOCUS), (LPARAM)GetParent()->GetSafeHwnd());
	
	
	SetSel(0, -1);

	CEdit::OnSetFocus(pOldWnd);
}

LRESULT CCodeEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_UP:
		case VK_DOWN:
		case VK_PRIOR:
		case VK_NEXT:
		case VK_HOME:
		case VK_END:
		case VK_RETURN:
			if (m_pParent->m_pCombo->GetDroppedState() &&
				(m_Unit == GU_CODE || m_Unit == GU_FUTURE || m_Unit == GU_OPTION || m_Unit == GU_FOCODE || m_Unit == GU_ELWCODE))
			{
				m_pParent->m_pCombo->Key((int)wParam);
				return TRUE;
			}
			break;
		}
	}
	
	return CEdit::WindowProc(message, wParam, lParam);
}

void CCodeEdit::ListCode(CString code)
{
	SetWindowText("");
	SetWindowText(code);
	GetParent()->SetWindowText(code);
	m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
}

void CCodeEdit::ChangeHangulMode(HWND hWnd, bool bHangle)
{
	DWORD	dwConversion, dwSentence;

	HIMC hImc = ImmGetContext(hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		if (bHangle)
			ImmSetConversionStatus(hImc, dwConversion | IME_CMODE_HANGEUL, dwSentence);
		else
			ImmSetConversionStatus(hImc, dwConversion & ~IME_CMODE_HANGEUL, dwSentence);
	}
}

bool CCodeEdit::isNumeric(CString str)
{
	for (int ii = 0; ii < str.GetLength(); ii++)
		if (str.GetAt(ii) < '0' || str.GetAt(ii) > '9')
			return false;
	return true;
}

bool CCodeEdit::isHexNumeric(CString str)
{
	if (!str.IsEmpty() && (str.GetAt(0) < '0' || str.GetAt(0) > '9'))
		return false;
	for (int ii = 1; ii < str.GetLength(); ii++)
		if ((str.GetAt(ii) < '0' || str.GetAt(ii) > '9') &&
			(str.GetAt(ii) < 'a' || (str.GetAt(ii) > 'f' && str.GetAt(ii) != 's')) &&
			(str.GetAt(ii) < 'A' || (str.GetAt(ii) > 'F' && str.GetAt(ii) != 'S')))
			return false;
	return true;
}


bool CCodeEdit::isElwNumeric(CString str)
{
	if (!str.IsEmpty() && (str.GetAt(0) < '0' || str.GetAt(0) > '9'))
		return false;
	for (int ii = 1; ii < str.GetLength(); ii++)
		if ( ii != 3 && (str.GetAt(ii) < '0' || str.GetAt(ii) > '9') &&
			(str.GetAt(ii) < 'a' || (str.GetAt(ii) > 'f' && str.GetAt(ii) != 's')) &&
			(str.GetAt(ii) < 'A' || (str.GetAt(ii) > 'F' && str.GetAt(ii) != 'S')))
			return false;
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CfxCodeCtrl

CfxCodeCtrl::CfxCodeCtrl(CWnd* parent, CString tabPath)
{
	m_view    = NULL;
	m_parent  = parent;
	m_tabPath = tabPath;

	m_pEdit   = NULL;
	m_pCombo  = NULL;

	m_btnDown = false;
	m_sHistory = "";
	m_bValidCheck = true;

	m_Unit = GU_NONE;


	CString path = tabPath;

	path.Replace("tab", "");
	path += "image\\";

	CString ss;

	ss.Format("%sAxEdit.BMP", path);
	m_Normal.LoadBitmap(ss);
	ss.Format("%sAxEdit_DN.BMP", path);
	m_Down.LoadBitmap(ss);
	ss.Format("%sAxEdit_EN.BMP", path);
	m_En.LoadBitmap(ss);
	m_bHover = false;
}

CfxCodeCtrl::~CfxCodeCtrl()
{
	m_Normal.DeleteObject();
	m_Down.DeleteObject();
	m_En.DeleteObject();
}


BEGIN_MESSAGE_MAP(CfxCodeCtrl, CWnd)
	//{{AFX_MSG_MAP(CfxCodeCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CfxCodeCtrl message handlers
BOOL CfxCodeCtrl::Create(CWnd* parent, CRect rect, UINT id)
{
	UINT style = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS/*|WS_BORDER*/;
	return CWnd::Create(NULL, _T(""), style, rect, parent, id);
}

int CfxCodeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pEdit = new CCodeEdit(this, m_parent);
	m_pEdit->Create(ES_AUTOHSCROLL|WS_VISIBLE|WS_CHILD|ES_UPPERCASE, CRect(0,0,10,10), this, 1001);
	m_pEdit->m_bValidCheck = m_bValidCheck;
	
	m_pCombo = new CCodeCombo(this);
	m_pCombo->Create(CBS_DROPDOWN|WS_CHILD|WS_VISIBLE|WS_VSCROLL, CRect(0, 0, 0, 140), this, 0);
	m_pCombo->SetItemHeight(-1, 0);
	m_pCombo->SetDroppedWidth(max(lpCreateStruct->cx, 150));

	//SetUnit(GU_CODE);
	
	m_cbRGB = -1;
	m_csRGB = -1;

	return 0;
}

void CfxCodeCtrl::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if (m_pEdit)
	{
		m_pEdit->DestroyWindow();
		delete m_pEdit; m_pEdit = NULL;
	}
	if (m_pCombo)
	{
		m_pCombo->DestroyWindow();
		delete m_pCombo; m_pCombo = NULL;
	}
}

void CfxCodeCtrl::SetPalette(COLORREF clrFg, COLORREF clrBg)
{
	m_cbRGB = clrFg;
	m_csRGB = clrBg;
	Invalidate();
}

void CfxCodeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_btnRect.PtInRect(point))
	{
		m_parent->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
		m_btnDown = true;
		InvalidateRect(&m_btnRect);
		RedrawWindow();

		HistoryMode();
		m_pCombo->ShowDropDown(TRUE);
		if (m_pCombo->GetCount() > 0)
			m_pCombo->SetCurSel(0);
	}

	Invalidate();
	UpdateWindow();
	CWnd::OnLButtonDown(nFlags, point);
}

void CfxCodeCtrl::LButtonUp()
{
	m_btnDown = false;
	Invalidate();
	InvalidateRect(&m_btnRect);
}

void CfxCodeCtrl::OnPaint() 
{
	CPaintDC	dc(this); 
	
	CDC		*pDC = &dc;
	
	COLORREF bg, fg;
	if (m_csRGB != -1 && m_csRGB != -1)
	{
		bg = m_cbRGB;
		fg = m_csRGB;
	}
	else
	{
		bg = COLOR_PAL_BG;
		fg = COLOR_PAL_FG;
	}

	CRect rc, rcBtn, tRc;
	CPoint	pts[3];
	GetClientRect(&rc);
	pDC->FillSolidRect(rc, COLOR_WHITE);
//	pDC->Draw3dRect(rc, RGB(170, 170, 170), COLOR_WHITE);
	pDC->Draw3dRect(rc, RGB(164, 164, 164), RGB(116, 116, 116));

	rcBtn = rc;
//	rcBtn.DeflateRect(0, 3, 2, 3);
	rcBtn.left = rcBtn.right - rcBtn.Height();
//	tRc = rcBtn;
//	tRc.DeflateRect(1, 0);
	rcBtn.DeflateRect(1, 1);
	rcBtn.left += 2;
	//if (m_b)
	m_btnDown ?
	m_Down.Draw(pDC, rcBtn) :
	m_bHover ? m_En.Draw(pDC, rcBtn): m_Normal.Draw(pDC, rcBtn);

	if (m_pEdit && m_pEdit->GetSafeHwnd())
		m_pEdit->Invalidate();


	/*
	CPen *open, pen(PS_SOLID, 1, bg);
	CBrush *obr, brush(bg);
	open = pDC->SelectObject(&pen);
	obr = pDC->SelectObject(&brush);
//	pDC->FillRect(tRc, &brush);

//	pDC->MoveTo(rcBtn.left, rcBtn.top+1);
//	pDC->LineTo(rcBtn.left, rcBtn.bottom-1);
//	pDC->MoveTo(rcBtn.right-1, rcBtn.top+1);
//	pDC->LineTo(rcBtn.right-1, rcBtn.bottom-1);

	CPen pen1(PS_SOLID, 1, fg);
	CBrush brush1(fg);
	pDC->SelectObject(&pen1);
	pDC->SelectObject(&brush1);

	tRc = rcBtn;
	tRc.DeflateRect(3, tRc.Height()*2/5);
	tRc.OffsetRect(1, 0);
	tRc.right = tRc.left + (tRc.Width()/2)*2-1;
	tRc.bottom = tRc.top + tRc.Width()/2;
	pts[0].y = tRc.bottom;
	pts[1].y = tRc.top;
	pts[2].y = tRc.top;
	pts[0].x = tRc.left + (tRc.Width()/2);
	pts[1].x = tRc.right-1;
	pts[2].x = tRc.left;
//	pDC->Polygon(pts, 3);

	pDC->SelectObject(open);
	pDC->SelectObject(obr);
	*/
// 2008. 3. 24. Kim Jung-tae GDI Leak Check Release Code
	//pen.DeleteObject();
	//pen1.DeleteObject();
	//brush.DeleteObject();
	//brush1.DeleteObject();
}

void CfxCodeCtrl::OnSetFocus(CWnd* pOldWnd) 
{	
	CWnd::OnSetFocus(pOldWnd);
	//m_pEdit->SetFocus();
	//m_pEdit->SetSel(0, -1);
}

void CfxCodeCtrl::SetEditSel()
{
	m_pEdit->SetFocus();
	m_pEdit->SetSel(0, -1);
}

void CfxCodeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect	rect, editRC, btnRC;
	GetClientRect(&rect);

	rect.DeflateRect(2,2);
	editRC = btnRC = rect;
	btnRC.left = btnRC.right - btnRC.Height();

	editRC.right = btnRC.left - 1;
	editRC.left += 1;
	editRC.top += 1;

	m_pEdit->MoveWindow(editRC.left, editRC.top, editRC.Width(), editRC.Height());
	m_pEdit->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

	m_btnRect = btnRC;
}

void CfxCodeCtrl::SetFont(CFont* pFont, bool redraw)
{
	m_pFont = pFont;

	m_pEdit->SetFont(m_pFont); m_pCombo->SetFont(m_pFont);
	if (redraw) RedrawWindow();
}

CString CfxCodeCtrl::GetEditData()
{
	CString	code;
	m_pEdit->GetWindowText(code);

	return code;
}

void CfxCodeCtrl::SetEditData(CString sData)
{
	m_pEdit->SetWindowText(sData);
}

void CfxCodeCtrl::SetUnit(int unit)
{
	if (m_Unit == unit) return;

	m_Unit = unit;

	m_pCombo->m_nUnit = unit;

	CString code = GetEditData();
	m_pEdit->SetWindowText("");
	m_pEdit->SetUnit(m_Unit);

	m_pCombo->ResetContent();

	m_pCombo->m_pJCode.RemoveAll();

	switch(m_Unit)
	{
	case GU_FUTURE:
		m_pCombo->FCodeLoad(m_tabPath);
		break;
	case GU_OPTION:
		m_pCombo->OCodeLoad(m_tabPath);
		break;
	case GU_POPTION:
		break;
	case GU_FOCODE:
		m_pCombo->FCodeLoad(m_tabPath);
		m_pCombo->OCodeLoad(m_tabPath);
		m_pCombo->SFCodeLoad(m_tabPath);
		break;
	case GU_ELWCODE:
		m_pCombo->ELWCodeLoad(m_tabPath);
		break;
	case GU_CODE:
	default:
		m_pCombo->JCodeLoad(m_tabPath);
		break;
	}
}

void CfxCodeCtrl::SetHistory(CString sHistory)
{
	m_sHistory = sHistory; 
}

void CfxCodeCtrl::HistoryMode()
{
	CString	symbol;
	switch (m_Unit)
	{
	case GU_CODE:
		symbol = "1301";
		break;
	case GU_FUTURE:
		symbol = "30301";
		break;
	case GU_OPTION:
		symbol = "40301";
		break;
	case GU_POPTION:
		symbol = "70301";
		break;
	case GU_FOCODE:
		symbol = "50301";
		break;
	case GU_ELWCODE:
		symbol = "1301";
		break;
	default:
		symbol = "";
		break;
	}

	m_pCombo->InitHistory(m_sHistory);
}

void CfxCodeCtrl::CodeListMode()
{
	if (!m_pCombo->IsCodeListMode())
		m_pCombo->InitCodeList();
}

void CfxCodeCtrl::SetValidCheck(bool bValid)
{
	m_bValidCheck =bValid;
}

void CfxCodeCtrl::SetUpData(CString code)
{
	if (code.IsEmpty())	return;

	m_pCombo->SetUpData(code);
}

void CfxCodeCtrl::SetDownData(CString code)
{
	if (code.IsEmpty())	return;

	m_pCombo->SetDownData(code);
}


void CfxCodeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rc, rcBtn;
	GetClientRect(&rc);
	rcBtn = rc;
	rcBtn.left = rcBtn.right - rcBtn.Height();

	if (!m_bHover && rcBtn.PtInRect(point)) {
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER/*|TME_NONCLIENT*/;
		tme.dwHoverTime = 1;
		m_bHover = _TrackMouseEvent(&tme) ? 1:0;
	}	
	CWnd::OnMouseMove(nFlags, point);
}


void CfxCodeCtrl::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bHover = false;
	Invalidate();

	CWnd::OnMouseLeave();
}


void CfxCodeCtrl::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	Invalidate();
	CWnd::OnMouseHover(nFlags, point);
}
