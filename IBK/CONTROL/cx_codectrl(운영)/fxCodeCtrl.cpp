// fxCodeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "fxCodeCtrl.h"
#include "../../h/jmcode.h"
#include "../../h/axisvar.h"
#include <imm.h>
#include "controlwnd.h"

#define IDC_EDIT		0x01
#define IDC_CTRL_UP		0x02
#define IDC_CTRL_DOWN		0x03
#define	IDC_CODE		0x04
#define IDC_LIST		0x05

#define MARGIN			2
#define BUTTON_HEIGHT		20
#define GAP			2
#define STANDARDNUM		11

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CLR_BACKCOLOR	RGB(255,255,255)
#define CLR_NOBACKCOLOR	RGB(204,204,204)


/////////////////////////////////////////////////////////////////////////////
// CCodeCombo

CCodeCombo::CCodeCombo()
{
	m_DataMode = DM_NONE;
	m_bVisible = false;
	m_nType = 0;
	m_Width = 0;
	m_Height = 0;

	m_pWizard = nullptr;
	m_pParent = nullptr;
	m_pCodeList = nullptr;
	m_bTracking = false;

	m_Unit = 0;
}

CCodeCombo::~CCodeCombo()
{
}


BEGIN_MESSAGE_MAP(CCodeCombo, CWnd)
	//{{AFX_MSG_MAP(CCodeCombo)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCodeCombo message handlers

int CCodeCombo::GetCurSel()
{
	return m_pCodeList->GetCurSel();
}

void CCodeCombo::SetTopIndex(int index)
{
	m_pCodeList->SetTopIndex(index);
}

void CCodeCombo::SetCurSel(int cur)
{
	m_pCodeList->SetCurSel(cur);
}

int CCodeCombo::GetTopIndex()
{
	return m_pCodeList->GetTopIndex();
}

int CCodeCombo::GetCount()
{
	return m_pCodeList->GetCount();
}

void CCodeCombo::AddString(CString str)
{
	m_pCodeList->AddItem(str);
}

void CCodeCombo::AddSFString(CString str)
{
	m_pCodeList->AddSFItem(str);
}

void CCodeCombo::ResetContent()
{
	m_pCodeList->Update();
}

void CCodeCombo::OnSelchange() 
{

}

int CCodeCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}

void CCodeCombo::OnDestroy() 
{
	m_pJCode.RemoveAll();
	m_pHCode.RemoveAll();
	CWnd::OnDestroy();
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
	const	int	len = code.GetLength();

	if (IsCodeListMode())
	{
		//int	size = m_pJCode.GetSize();
		CString	strItem;
		CString	strCode, strName;
		const	int	size = GetCount();

		for (int ii = 0, nidx = 0; ii < size; ii++)
		{
			//jCode = m_pJCode.GetAt(ii);

			strItem = m_pCodeList->GetRawItem(ii);
			nidx = strItem.Find('\t');

			if (nidx < 0)
				continue;

			strCode = strItem.Left(nidx);
			strName = strItem.Mid(nidx+1);
			if (strName.GetLength() < len)
				continue;
	
			if (m_nType == 0)
			{
				if (strName.Left(len) == code)
				{
					SetTopIndex(ii);
					SetCurSel(ii);
					return;
				}
			}
			else
			{
				if (strName.Find(code) > -1)
				{
					SetTopIndex(ii);
					SetCurSel(ii);
					return;
				}
			}
// 			if (jCode.Name.GetLength() < len)
// 				continue;

// 			if (m_Unit == GU_CODE)
// 			{
// 				if (jCode.Code.GetAt(0) != 'A')
// 				{
// 					continue;
// 				}
// 			}
			
// 			if (m_nType == 0)
// 			{
// 				if (jCode.Name.Left(len) == code)
// 				{
// 					SetTopIndex(ii);
// 					SetCurSel(ii);
// 					return;
// 				}
// 			}
// 			else
// 			{
// 				if (jCode.Name.Find(code) > -1)
// 				{
// 					SetTopIndex(ii);
// 					SetCurSel(ii);
// 					return;
// 				}
// 			}
		}
	}
	else if (IsHistoryMode())
	{
		const	int	size = m_pHCode.GetSize();
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
	const	int	index = GetCurSel();

	if (index < 0)
		return "";

	if (IsCodeListMode())
	{
// 		_JCode	jCode = m_pJCode.GetAt(index);
// 		m_sCode = jCode.Code.Mid(1);
		CString strItem = m_pCodeList->GetRawItem(index);
		const	int	nidx = strItem.Find('\t');
		if (nidx < 0)
			return "";
		
		m_sCode = strItem.Left(nidx);
	}
	else if (IsHistoryMode())
	{
		_JCode	jCode = m_pHCode.GetAt(index);
		m_sCode = jCode.Code.Mid(1);
	}

	return m_sCode;
}

void CCodeCombo::Key(int key)
{
	const	int	count = 9;
	int	topIndex = GetTopIndex();
	int	index = GetCurSel();
	const	int	endIndex = topIndex+count-1;
	bool	bSetTopIndex = false;

	switch (key)
	{
	case VK_UP:	// up arrow
		index = max(0, index-1);
		if (index < topIndex)	topIndex = index;
		bSetTopIndex = true;
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

void CCodeCombo::InitCodeList(int unit,CString sFind)
{
	m_pCodeList->RemoveAll();
	_JCode	jCode;
	CString	Str;
	CStringArray arJCode;

	for (int ii = 0; ii < m_pJCode.GetSize(); ii++)
	{
		jCode = m_pJCode.GetAt(ii);

		if (!jCode.Code.GetLength())
			continue;
		Str.Format("%s\t%s", jCode.Code.Mid(1), jCode.Name);

		if (m_nType == 1)
		{
			if (Str.Find(sFind) > -1)
			{
				if (unit==GU_CODE)
				{
					if (jCode.Code.GetAt(0) == 'J')
					{
						arJCode.Add(Str);
					}
					else
					{
						AddString(Str);
					}
				}
				else
					AddString(Str);
			}
		}
		else
			AddString(Str);
	}

	for (int jj=0;jj < arJCode.GetSize();jj++)
	{
		AddString(arJCode.GetAt(jj));
	}
	
	m_Unit = unit;
	m_DataMode = DM_CODELIST;
	m_pCodeList->Update();
	
}

void CCodeCombo::InitHistory(CString history, int unit)
{
	ResetContent();

	m_pCodeList->RemoveAll();
	m_pHCode.RemoveAll();
	_JCode	jCode;
	CString	Str;
	int	index = 0;
	for ( ;; )
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

			if (unit==GU_ELWCODE && jCode.Code[0]=='9') continue;
			if (unit==GU_ELWCODE && jCode.Code.GetLength()!=6) continue;
			if (unit==GU_CODE && jCode.Code.GetLength()!=6) continue;

			m_pHCode.Add(jCode);
		}
	}

	for (int ii = 0; ii < m_pHCode.GetSize(); ii++)
	{
		jCode = m_pHCode.GetAt(ii);
		Str.Format("%s\t%s", jCode.Code, jCode.Name);
		
		AddString(Str);
	}
	m_Unit = unit;
	m_DataMode = DM_HISTORY;
	m_pCodeList->Update();
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
	CString path;
	CFile	file;

	m_pJCode.RemoveAll();

	path.Format("%s\\%s\\%s", tabPath, TABDIR, HJCODEx);
	if (!file.Open(path, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		CString	msg;
		msg.Format("[%s] file open error!!", path);
		AfxMessageBox(msg);
		return false;
	}

	int	size = (int)file.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(size + 1);

	const	int	ival = file.Read(pWb.get(), size);
	file.Close();
	if (ival != size)
	{
		CString	msg;
		msg.Format("[%s] file read error!!", path);
		AfxMessageBox(msg);
		pWb.reset();
		return false;
	}

	struct _JCode jCode {};
	struct  hjcodex* pcodex = (struct hjcodex*)pWb.get();
	size /= sizeof(struct hjcodex);
	for (int ii = 0; ii < size; ii++, pcodex++)
	{
		switch (pcodex->ssgb)
		{
		case jmSINJS:
		case jmSINJU:
		case jmHYFND:
			continue;
		default:break;
		}

		jCode.Code = CString(pcodex->code, HCodeLen);
		jCode.Code.TrimRight();
		if (jCode.Code.IsEmpty())
			continue;

		jCode.Name = CString(pcodex->hnam, HNameLen);
		jCode.Name.MakeUpper();
		jCode.Name.TrimRight();
		if (pcodex->ssgb != jmELW)
			jCode.Name = jCode.Name.Mid(1);
		jCode.Ssgb = pcodex->ssgb;

		m_pJCode.Add(jCode);
	}
	m_pJCode.QuickSort();
	pWb.reset();

	return true;
}

bool CCodeCombo::ELWCodeLoad(CString tabPath)
{
	CString path;
	CFile	file;

	m_pJCode.RemoveAll();

	path.Format("%s\\%s\\%s", tabPath, TABDIR, HJCODEx);
	if (!file.Open(path, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		CString	msg;
		msg.Format("[%s] file open error!!", path);
		AfxMessageBox(msg);
		return false;
	}

	int	size = (int)file.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(size + 1);

	const	int	ival = file.Read(pWb.get(), size);
	file.Close();
	if (ival != size)
	{
		CString	msg;
		msg.Format("[%s] file read error!!", path);
		AfxMessageBox(msg);
		pWb.reset();
		return false;
	}

	struct _JCode jCode {};
	struct  hjcodex* pcodex = (struct hjcodex*)pWb.get();
	size /= sizeof(struct hjcodex);
	for (int ii = 0; ii < size; ii++, pcodex++)
	{
		if (pcodex->ssgb != jmELW)
			continue;

		jCode.Code = CString(pcodex->code, HCodeLen);
		jCode.Code = jCode.Code.Mid(1);
		jCode.Code.TrimRight();
		if (jCode.Code.IsEmpty())
			continue;

		jCode.Name = CString(pcodex->hnam, HNameLen);
		jCode.Name.MakeUpper();
		jCode.Name.TrimRight();
		jCode.Ssgb = pcodex->ssgb;

		m_pJCode.Add(jCode);
	}
	m_pJCode.QuickSort();
	pWb.reset();
	return true;
}

BOOL CCodeCombo::loadSfCode(CString tabpath)
{	
	CFile	file;
	CString path;

	m_pJCode.RemoveAll();

	path.Format("%s\\%s\\%s", tabpath, TABDIR, SFCODEDAT);
	if (!file.Open(path, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary))
	{	
 		MessageBox("������ �������� �ʽ��ϴ�.");
		return FALSE;
	}

	int	size = (int)file.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(size + 1);

	const	int	ival = file.Read(pWb.get(), size);
	file.Close();
	if (ival != size)
		return FALSE;

	struct	_JCode	jCode {};
	struct  sfcode* pcode = (struct sfcode*)pWb.get();
	size /= sizeof(struct sfcode);
	for (int ii = 0; ii < size; ii++, pcode++)
	{
		jCode.Code = CString(pcode->codx, 8);
		jCode.Code.TrimRight();
		if (jCode.Code.IsEmpty())
			continue;

		jCode.Name = CString(pcode->hnam, 50);
		jCode.Name.MakeUpper();
		jCode.Name.TrimRight();

		m_pJCode.Add(jCode);
	}

	pWb.reset();
	return TRUE;
}

void CCodeCombo::WriteHistory(CString rootPath)
{
	CString usr;
	CString strPath;
	CString line;
	CFile	file;

	usr = ((CControlWnd*)GetParent())->Variant(nameCC, "");
	strPath.Format("%s/%s/%s/%s", rootPath, USRDIR, usr,SFCODE_HISTORY);
	
	if (file.Open(strPath, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone))
	{
		for (int ii = 0; ii < m_pCodeList->GetCount(); ii++)
		{
			line = m_pCodeList->GetRawItem(ii);

			line.Replace("\t", " ");
			line += "\t";

			file.Write(line, line.GetLength());
		}
		file.Close();
	}
}

bool CCodeCombo::FCodeLoad(CString tabPath)
{
	CString path;
	CFile	file;

	path.Format("%s\\%s\\%s", tabPath, TABDIR, FJCODE);
	if (!file.Open(path, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		CString	msg;
		msg.Format("[%s] file open error!!", path);
		AfxMessageBox(msg);
		return false;
	}

	int	size = (int)file.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(size + 1);

	const	int	ival = file.Read(pWb.get(), size);
	file.Close();
	if (ival != size)
	{
		CString	msg;
		msg.Format("[%s] file read error!!", path);
		AfxMessageBox(msg);
		pWb.reset();
		return false;
	}

	struct	_JCode	fCode {};
	struct	fjcode* pcode = (struct fjcode*)pWb.get();

	size /= sizeof(struct fjcode);
	for (int ii = 0; ii < size; ii++, pcode++)
	{
		fCode.Code = CString(pcode->cod2, FCodeLen);
		fCode.Code.TrimRight();
		fCode.Name = CString(pcode->hnam, FNameLen);
		fCode.Name.TrimRight();

		m_pJCode.Add(fCode);
	}

	m_pJCode.QuickSort();
	pWb.reset();
	return true;
}

bool CCodeCombo::OCodeLoad(CString tabPath)
{
	CString	path;
	CFile	file;

	path.Format("%s\\%s\\%s", tabPath, TABDIR, OJCODE);
	if (!file.Open(path, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		CString	msg;
		msg.Format("[%s] file open error!!", path);
		AfxMessageBox(msg);
		return false;
	}

	int	size = (int)file.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(size + 1);
	
	int	ival = file.Read(pWb.get(), size);
	file.Close();
	if (ival != size)
	{
		CString	msg;
		msg.Format("[%s] file read error!!", path);
		AfxMessageBox(msg);
		pWb.reset();
		return false;
	}

	ival = sizeof(struct ojcodh);
	size -= ival;		// skip option header
	size /= sizeof(struct ojcode);

	struct	_JCode	oCode {};
	struct	ojcode* pcode = (struct ojcode*)&pWb[ival];

	for (int ii = 0; ii < size; ii++, pcode++)
	{
		for (int jj = 0; jj < STANDARDNUM; jj++)
		{
			if (pcode->call[STANDARDNUM - jj - 1].yorn == '1')
			{
				oCode.Code = CString(pcode->call[STANDARDNUM - jj - 1].cod2, OCodeLen);
				oCode.Name = CString(pcode->call[STANDARDNUM - jj - 1].hnam, ONameLen);
				ival = oCode.Name.Find('\0');
				if (ival != -1)
					oCode.Name = oCode.Name.Left(ival);
				oCode.Name.TrimRight();
				m_pJCode.InsertAt(0, oCode);
			}

			if (pcode->put[jj].yorn == '1')
			{
				oCode.Code = CString(pcode->put[jj].cod2, OCodeLen);
				oCode.Name = CString(pcode->put[jj].hnam, ONameLen);
				ival = oCode.Name.Find('\0');
				if (ival != -1)
					oCode.Name = oCode.Name.Left(ival);
				oCode.Name.TrimRight();
				m_pJCode.InsertAt(0, oCode);
			}
		}
	}

	m_pJCode.QuickSort();
	pWb.reset();
	return true;
}

bool CCodeCombo::IsValidCode(CString sCode)
{
	CString str = ((CControlWnd*)m_pParent)->GetHNam(sCode);

	return !str.IsEmpty();
}

void CCodeCombo::RemoveCode()
{
	m_pJCode.RemoveAll();
}

bool CCodeCombo::GetDroppedState()
{
	return m_bVisible;
}

void CCodeCombo::SetDroppedState(bool bflag)
{
	m_bVisible = bflag;
}

void CCodeCombo::ShowDropDown(bool bflag, bool bshow)
{
	CRect	rc;
	const	int	defaultWidth = (m_pCodeList->m_RowHeight - 2) * 17;

	GetWindowRect(rc);
	m_pCodeList->SetWindowPos(NULL, rc.left, rc.bottom, defaultWidth, m_pCodeList->m_RowHeight*21+4, SWP_HIDEWINDOW|SWP_NOACTIVATE);
	m_pCodeList->m_pCtlBtnDeleteAll->SetWindowPos(NULL, 
					 GAP, 
					 m_pCodeList->m_RowHeight*21 - m_pCodeList->m_RowHeight, 
					 defaultWidth- 2*GAP - 16, 
					 m_pCodeList->m_RowHeight+4 - GAP, SWP_SHOWWINDOW);
	
	if (bflag)
	{
		m_pCodeList->SetWindowPos(NULL, rc.left, rc.bottom, m_Width, m_Height, SWP_NOSIZE|SWP_SHOWWINDOW);
		m_pCodeList->ShowWindow(bshow == true? SW_SHOW:SW_HIDE);	
	}
	else 
	{
		m_pCodeList->SetWindowPos(NULL, rc.left, rc.bottom, m_Width, m_Height, SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
		m_pCodeList->ShowWindow(bshow == true? SW_SHOWNA:SW_HIDE);
	}
	m_pCodeList->SetTopIndex(0);
	m_bVisible = bshow;
}

/////////////////////////////////////////////////////////////////////////////
// CCodeEdit
#define TID_CHANGEHANGLE 1000
#define TID_RETURNTIDLE 1001
CCodeEdit::CCodeEdit(CfxCodeCtrl *pCombo, CWnd* pTarget)
{
	m_pParent = pCombo;
	m_pTarget = pTarget;

	m_Unit        = GU_NONE;
	m_preCode     = _T("");
	m_bKeydown    = false;
	m_bValidCheck = true;
	m_backColor.CreateSolidBrush(CLR_BACKCOLOR);
	m_NObackColor.CreateSolidBrush(CLR_NOBACKCOLOR);

}

CCodeEdit::~CCodeEdit()
{
	m_backColor.DeleteObject();
	m_NObackColor.DeleteObject();
}

BEGIN_MESSAGE_MAP(CCodeEdit, CDDEdit)
	//{{AFX_MSG_MAP(CCodeEdit)
	ON_WM_CHAR()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_ENABLE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCodeEdit message handlers

void CCodeEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CString tmpS;
	int	len = 0;

	GetWindowText(tmpS);
	if (tmpS.IsEmpty() && nChar >= '0' && nChar <= '9')
		ChangeHangulMode(this->m_hWnd, false);

	switch (nChar)
	{
	case VK_RETURN:
		//TRACE("OnChar: VK_RETURN\n");
		len = tmpS.GetLength();
		if (len <= 0)
		{
			if (len == 0)
			{
				//SetTimer(TID_RETURNTIDLE, 3000, NULL);
				
				m_pParent->m_pParent->PostMessage(WM_KEYDOWN, VK_RETURN, 0);
				m_pParent->FireChangeEvent();
				return;
			}
		}
		
		if (m_Unit == GU_INDEX)
		{
			if (isNumeric(tmpS) && len == sz_INDEX)
			{
				CString	string;
				
				string.Format("%03d", atoi(tmpS)); 
				m_pParent->SetEditData(string);
				SetSel(0, -1);
				m_pParent->m_pParent->SendMessage(WM_COMMAND, MAKEWPARAM(m_pParent->GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
			}
		}
		else if (m_Unit == GU_CODE || m_Unit == GU_ELWCODE)
		{
			//TRACE("OnChar: GU_CODE\n");
			if (!isHexNumeric2(tmpS))
			{	// false
				OnKeyUp(0, 0, 0); //����Ű�� ������ �ѱ��Է��� �Բ� ������ keyup�̺�Ʈ�� �� ��Ÿ�� ���׶����� �ٽ��ѹ� ������ ����ó��
				
				const	int	curIndex = m_pParent->GetCurSel();
				//TRACE("OnChar: GU_CODE-IN: %s[%d]\n", tmpS, curIndex);
				
				if (curIndex == 0)
					m_pParent->SearchCode(tmpS);
				tmpS = m_pParent->GetSelectCode();

				if (m_pParent->GetDroppedState())
					m_pParent->ShowDropDown(false, false);
				m_pParent->SetEditData(tmpS);
			}
			else
			{
				m_pParent->m_DataMode = 0;

				//pyt
				CString str, name;
				switch (len)
				{
				case 2:
//					str.Format("0000%02d", atoi(tmpS));
					str.Format("0000%s", tmpS);
					break;
				case 3:
//					str.Format("000%03d", atoi(tmpS));
					str.Format("000%s", tmpS);
					break;
				case 4:
//					str.Format("00%04d", atoi(tmpS)); 
					str.Format("00%s", tmpS);
					break;
				case 5:
//					str.Format("0%05d", atoi(tmpS)); 
					str.Format("0%s", tmpS);
					break;
				case 6:
//					str.Format("%06d", atoi(tmpS)); 
					str.Format("%s", tmpS);
					break;
				}
				
				name = ((CControlWnd*)m_pParent->m_pParent)->GetHNam(str);
				if (!name.IsEmpty())
				{
					CString sRtnStr = ((CControlWnd*)m_pParent->m_pParent)->GetRtnStr();
					if (sRtnStr != str)
					{
						m_pParent->SetNormal();
						m_pParent->SetEditData(str);
					}
					else
					{
						m_pParent->OnBtnDown();
					}
					
					SetSel(0, -1);
				}
				else 
				{
					str = ((CControlWnd*)m_pParent->m_pParent)->GetRtnStr();
					m_pParent->SetEditData(str);
					SetWindowText(str);
					SetSel(0, -1);
				}
			}
			m_pParent->m_pParent->PostMessage(WM_KEYDOWN, VK_RETURN, 0);
			return;
		}
		else if (m_Unit == GU_FUTURE || m_Unit == GU_OPTION || m_Unit == GU_FOCODE || m_Unit == GU_POPTION || m_Unit == GU_FCODE || m_Unit == GU_FOSTOCK)
		{
			//if (m_Unit == GU_FUTURE && (len == 5 && tmpS.GetAt(0) == '1'))
			// 202300211 �Ļ���ǰ �ڵ� ����  '1', 'A' : ����
			if (m_Unit == GU_FUTURE && len == 5 && (tmpS.GetAt(0) == '1' || tmpS.GetAt(0) == 'A'))	
			{
				tmpS += "000";
				m_pParent->SetEditData(tmpS);
				SetSel(0, -1);
				break;
			}

			GetWindowText(tmpS);
			if (!tmpS.IsEmpty())
			{
				if (!isHexNumeric(tmpS))
				{
					CString	tmpS = m_pParent->GetSelectCode();
					if (m_pParent->GetDroppedState())
						m_pParent->ShowDropDown(false, false);
					m_pParent->SetEditData(tmpS);
				}
				SetSel(0, -1);
			}
		}
		else
			m_pParent->SetEditData(tmpS);	

		m_pParent->m_pParent->PostMessage(WM_KEYDOWN, VK_RETURN, 0);
		break;
	case VK_TAB:
		{
			m_pParent->m_pParent->PostMessage(WM_KEYDOWN, VK_TAB, 0);			
			return;	
		}
	}
	CDDEdit::OnChar(nChar, nRepCnt, nFlags);
	
}

void CCodeEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_bKeydown = true;
	CString DownHis_Code;
	switch (nChar)
	{
	case VK_UP :
		if (!m_pParent->GetDroppedState())
		{
			DownHis_Code = m_pParent->GetHistCode(-1);
			if (DownHis_Code.IsEmpty()) return;
			m_pParent->SetEditData(DownHis_Code);
		
			SetSel(0, -1);
			
			return;
		}
		break;
	case VK_DOWN :
		if (!m_pParent->GetDroppedState())
		{
			DownHis_Code = m_pParent->GetHistCode(1);
			if (DownHis_Code.IsEmpty()) return;
			m_pParent->SetEditData(DownHis_Code);
			//SetWindowText(DownHis_Code);
			
			SetSel(0, -1);
			return;
		}
		break;
	}
	CDDEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCodeEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CDDEdit::OnKeyUp(nChar, nRepCnt, nFlags);
	
	if (nFlags == 0x8000) 		
		return;
	/*
	if ((nChar == VK_UP)||(nChar == VK_DOWN))
		return;
	*/
	
	if ((nChar != VK_RETURN)||(nChar == VK_DOWN))
	{
		CString	sTmp;
		int	len = 0;

		GetWindowText(sTmp);
		if (sTmp.IsEmpty())
		{
			((CControlWnd*)m_pParent->GetParent())->clean();
			return;
		}

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
		case GU_INDEX:
			if (len == sz_INDEX && isHexNumeric(sTmp)) 
			{	
				m_pParent->SetEditData(sTmp);
				m_pParent->m_pParent->SendMessage(WM_COMMAND, MAKEWPARAM(m_pParent->GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
			}
			break;
		case GU_CODE:
		case GU_ELWCODE:
			if (isNumeric(sTmp) && sTmp.GetLength() > 6)
			{
				((CControlWnd*)m_pParent->GetParent())->clean();
				return;
			}
			//TRACE("KeyUp: %s\n", sTmp);
			//if (!isHexNumeric(sTmp) && m_Unit == GU_CODE)
			if (!isHexNumeric2(sTmp) && m_Unit == GU_CODE)
			{
				m_pParent->CodeListMode(sTmp);
				if (!m_pParent->GetDroppedState())
					m_pParent->ShowDropDown(false);
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
				
				m_pParent->SearchCode(sTmp);
				return;
			}
			if ((len == sz_JCODE && m_bValidCheck 
				&& (m_pParent->IsValidCode(sTmp)) || (m_bValidCheck && len >= sz_JCODE)))
			{	
				m_pParent->SetNormal();
				GetParent()->SetWindowText(sTmp);
				m_pParent->SetEditData(sTmp);
				SetSel(0, -1);
				if (m_pParent->GetDroppedState())
					m_pParent->ShowDropDown(false, false);
			}
			else if (len >= sz_OPTION)
			{
				GetParent()->SetWindowText(sTmp);
				m_pParent->SetEditData(sTmp);
				SetSel(0, -1);
				if (m_pParent->GetDroppedState())
					m_pParent->ShowDropDown(false, false);
			}
			break;
		case GU_FUTURE: 
			if (!isHexNumeric(sTmp))
			{
//				m_pParent->CodeListMode();
//				if (!m_pParent->GetDroppedState())
//					m_pParent->ShowDropDown(TRUE);
//				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
//				m_pParent->SearchCode(sTmp);
//				
//				return;
			}
			//if (len == 5 && sTmp.GetAt(0) == '1')
			if (len == 5 && (sTmp.GetAt(0) == '1' || sTmp.GetAt(0) == 'A'))	// 20230211 �Ļ���ǰ �ڵ� ����  '1', 'A' : ����
			{
				sTmp += "000";
				SetWindowText(sTmp);
				SetSel(0, -1);
				len = 8;
			}
			if (len == sz_FCODE)
			{
				GetParent()->SetWindowText(sTmp);
				m_pParent->SetEditData(sTmp);
//				m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
				SetSel(0, -1);
//				if (m_pParent->GetDroppedState())
//					m_pParent->ShowDropDown(FALSE);
			}
			break;
		case GU_FCODE:
			if (len == sz_FOCODE)
			{
// 				GetParent()->SetWindowText(sTmp);
// 				m_pParent->SetEditData(sTmp);
// 				SetSel(0, -1);

				GetParent()->SetWindowText(sTmp);
				m_pParent->SetEditData(sTmp);
				SetSel(0, -1);
				if (m_pParent->GetDroppedState())
					m_pParent->ShowDropDown(false, false);
			}
			break;
		case GU_FOSTOCK:
			if (len == sz_FOCODE)
			{

			}
		case GU_OPTION:
			if (!isHexNumeric(sTmp))
			{
//				m_pParent->CodeListMode();
//				if (!m_pParent->GetDroppedState())
//					m_pParent->ShowDropDown(TRUE);
//				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
//				m_pParent->SearchCode(sTmp);
//				
//				return;
			}
		case GU_POPTION:
			if (len == sz_OPTION)
			{
				GetParent()->SetWindowText(sTmp);
				m_pParent->SetEditData(sTmp);
//				m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
				SetSel(0, -1);
//				if (m_pParent->GetDroppedState() || !sTmp.IsEmpty())
//					m_pParent->ShowDropDown(FALSE);
			}
			break;
		case GU_FOCODE:
			if (!isHexNumeric(sTmp))
			{
//				m_pParent->CodeListMode();
//				if (!m_pParent->GetDroppedState())
//					m_pParent->ShowDropDown(TRUE);
//				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
//				m_pParent->SearchCode(sTmp);
//				
//				return;
			}
			if (len == sz_FOCODE)
			{
				GetParent()->SetWindowText(sTmp);
				m_pParent->SetEditData(sTmp);
//				m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
				SetSel(0, -1);
				if (m_pParent->GetDroppedState() || !sTmp.IsEmpty())
					m_pParent->ShowDropDown(false, false);
			}
			break;
		}
	}

	m_bValidCheck = true;
}

void CCodeEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDDEdit::OnLButtonDown(nFlags, point);
	//m_bFocusInit = TRUE;
	m_pParent->SetAxFocus(true);
	SetSel(0, -1);
}

BOOL CCodeEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (m_pParent->GetDroppedState() && 
		(m_Unit == GU_CODE || m_Unit == GU_FUTURE || m_Unit == GU_OPTION || m_Unit == GU_FOCODE || m_Unit == GU_POPTION || m_Unit == GU_ELWCODE))
	{	
	}
	
	return CDDEdit::OnMouseWheel(nFlags, zDelta, pt);
}

void CCodeEdit::OnSetFocus(CWnd* pOldWnd) 
{

	KillTimer(TID_CHANGEHANGLE);
	SetTimer(TID_CHANGEHANGLE, 10, NULL);	
	SetSel(0, -1);	
	CDDEdit::OnSetFocus(pOldWnd);
}


LRESULT CCodeEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_UP:
		case VK_PRIOR:
//			if (m_pParent->OnBtnUp())
//				return TRUE;
			m_pParent->GoUp();
			break;
		case VK_DOWN:
		case VK_NEXT:
//			if (m_pParent->OnBtnDown())
//				return TRUE;
			m_pParent->GoDown();
			break;
		case VK_HOME:
		case VK_END:
		case VK_RETURN:
			if (m_pParent->GetDroppedState() &&
				(m_Unit == GU_CODE || m_Unit == GU_FUTURE || m_Unit == GU_OPTION || m_Unit == GU_FOCODE || m_Unit == GU_ELWCODE))
			{
				m_pParent->Key(wParam);
				return TRUE;
			}
			break;
		}
	} // Wheel �޽����� ���Ͽ� �״� ���� ���� : 2008.09.29 ����ȣ
	else if (message == WM_MOUSEWHEEL)
	{
		return m_pParent->m_pParent->GetParent()->SendMessage(message, wParam, lParam);	
	}
	
	return CDDEdit::WindowProc(message, wParam, lParam);
}

void CCodeEdit::ListCode(CString code)
{
	SetWindowText("");
	SetWindowText(code);
	GetParent()->SetWindowText(code);
}

void CCodeEdit::ChangeHangulMode(HWND hWnd, bool bHangle)
{
	DWORD	dwConversion = 0, dwSentence = 0;
	
	HIMC hImc = ImmGetContext(this->m_hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		
#if 1 // for korean
		if (dwConversion & IME_CMODE_HANGEUL)
			return;
//		keybd_event(VK_HANGEUL,0,0,0);		
//		keybd_event(VK_HANGEUL,0,KEYEVENTF_KEYUP,0);
		
		dwConversion |= IME_CMODE_HANGEUL;
#else // for english
		if (dwConversion & IME_CMODE_HANGEUL)
			dwConversion -= IME_CMODE_HANGEUL;
#endif
		ImmSetConversionStatus(hImc, dwConversion, dwSentence);
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
	//if (!str.IsEmpty())  //test 20230211 �� �̷��ڵ带
	//	return true;

	if (m_Unit == GU_FUTURE || m_Unit == GU_OPTION || m_Unit == GU_FOCODE || m_Unit == GU_POPTION || m_Unit == GU_FCODE || m_Unit == GU_FOSTOCK)
	{
		// 20230125 �Ļ���ǰ �ڵ� ����  '1', 'A' : ����
		//                              '2', 'B' : Call option
		//			        '3', 'C' : Put option
		//			        '4', 'D' : ��������
		if ((str.GetAt(0) < '0' || str.GetAt(0) > '9') && (str.GetAt(0) < 'A' || str.GetAt(0) > 'G'))
			return false;
	}
	else if (str.GetAt(0) < '0' || str.GetAt(0) > '9')
		return false;

	for (int ii = 1; ii < str.GetLength(); ii++)
		if ((str.GetAt(ii) < '0' || str.GetAt(ii) > '9') &&
			(str.GetAt(ii) < 'a' || (str.GetAt(ii) > 'z' && str.GetAt(ii) != 's')) &&
			(str.GetAt(ii) < 'A' || (str.GetAt(ii) > 'Z' && str.GetAt(ii) != 'S')))
			return false;
	return true;

	/*if (!str.IsEmpty() && (str.GetAt(0) < '0' || str.GetAt(0) > '9'))
		return false;

	for (int ii = 1; ii < str.GetLength(); ii++)
		if ((str.GetAt(ii) < '0' || str.GetAt(ii) > '9') &&
			(str.GetAt(ii) < 'a' || (str.GetAt(ii) > 'z' && str.GetAt(ii) != 's')) &&
			(str.GetAt(ii) < 'A' || (str.GetAt(ii) > 'Z' && str.GetAt(ii) != 'S')))
			return false;
	return true;*/
}

bool CCodeEdit::isHexNumeric2(CString str)
{
	if (!str.IsEmpty() && (str.GetAt(0) < '0' || str.GetAt(0) > '9'))
		return false;
/*
	�� �ڵ�� ii = 1���� ������ return�Ǵ� �����̹Ƿ� 
	vs2019 ����� ����
	for (int ii = 1; ii < str.GetLength(); ii++)
	{
 		if (str.GetAt(ii) < '0' || str.GetAt(ii) > '9')
		{
			if (ii != 3)			//ELW �� 4��°�� ������ �� �� ����
			{
				return false;
			}
		}

		return true;
	}
	return false;
*/
	for (int ii = 0; ii < str.GetLength(); ii++)
	{
		//ELW �� 3��°���� ������ �� �� ����
		if (ii < 2 && (str.GetAt(ii) < '0' || str.GetAt(ii) > '9'))
			return false;
	}

	return true;
}

void CCodeEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	TRACE("CCodeEdit::TRACE\n");
	CDDEdit::OnMouseMove(nFlags, point);
}

void CCodeEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CDDEdit::OnKillFocus(pNewWnd);	
	
	//if (m_bFocusInit == TRUE)	
	//{
	//	if (pNewWnd->GetSafeHwnd() != this->GetSafeHwnd())
	//	{
	//		SetFocus();
	//		SetSel(0,-1);
	//
	//		m_pParent->SetFocus();
	//	}
	//	
	//	m_bFocusInit = FALSE;
	//}
}

/////////////////////////////////////////////////////////////////////////////
// CfxCodeCtrl

CfxCodeCtrl::CfxCodeCtrl(CWnd* parent, CString rootPath)
{
	m_pWizard   = parent;
	m_pParent   = nullptr;
	m_sPath	    = rootPath;
	m_bVisible  = false;
	m_bTracking = false;

	m_pEdit     = nullptr;
	m_pCodeList = std::make_unique<CCodeList>(this, parent, m_sPath);

	m_btnDown = 0;
	m_btnList = 0;
	m_updnIndex = 1;

	m_sHistory = "";
	m_bValidCheck = true;

	m_Unit = GU_NONE;

	m_sbmpCode = m_sbmpCodepush = m_sbmpList = m_sbmpListpush = _T("");
	m_arrList.RemoveAll();

	m_bHistory = true;
	m_pCtlUpBtn = NULL;
	m_pCtlDownBtn = NULL;
	m_nIndex = 0;

	m_pBtnCode = NULL;
	m_pBtnList = NULL;
	m_Name.Empty();

	m_bKONEX = FALSE;	//KDK 2013.05.14 KONEX

	//m_bFocusInit = FALSE;
}

CfxCodeCtrl::~CfxCodeCtrl()
{
}

BEGIN_MESSAGE_MAP(CfxCodeCtrl, 	CCodeCombo)
	//{{AFX_MSG_MAP(CfxCodeCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_WM_MOVE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CTRL_UP, OnBtnUp)
	ON_BN_CLICKED(IDC_CTRL_DOWN, OnBtnDown)
	ON_BN_CLICKED(IDC_CODE, OnCodeDown)
	ON_BN_CLICKED(IDC_LIST, OnListDown)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CfxCodeCtrl message handlers
BOOL CfxCodeCtrl::Create(CWnd* parent, CRect rect, UINT id)
{
	m_pParent = parent;
	m_bFirst = false;
	m_backColor.CreateSolidBrush(CLR_BACKCOLOR);
	m_NObackColor.CreateSolidBrush(CLR_NOBACKCOLOR);

	return CWnd::Create(NULL, _T(""), WS_CHILD | WS_VISIBLE, rect, parent, id);
}

int CfxCodeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pEdit = std::make_unique<CCodeEdit>(this, m_pWizard);
	m_pEdit->Create(ES_AUTOHSCROLL|WS_VISIBLE|WS_CHILD|ES_UPPERCASE, CRect(0,0,10,10), this, IDC_EDIT);
	m_pEdit->m_bValidCheck = m_bValidCheck;
	
	m_pCtlUpBtn   = std::make_unique<CfxImgButton>();
	m_pCtlDownBtn = std::make_unique<CfxImgButton>();
	m_pCtlUpBtn->Create("up", CRect(0, 0, 0, 0), this, IDC_CTRL_UP);
	m_pCtlDownBtn->Create("down", CRect(0, 0, 0, 0), this, IDC_CTRL_DOWN);

	CString path;
	path.Format("%s\\%s\\", m_sPath, IMAGEDIR);
	m_pCtlUpBtn->SetImgBitmap(((CControlWnd*)m_pParent)->GetBitmap(path + "axspin1.bmp"),
				  ((CControlWnd*)m_pParent)->GetBitmap(path + "axspin1_en.bmp"),
				  ((CControlWnd*)m_pParent)->GetBitmap(path + "axspin1_dn.bmp"));

	m_pCtlDownBtn->SetImgBitmap(((CControlWnd*)m_pParent)->GetBitmap(path + "axspin2.bmp"),
				    ((CControlWnd*)m_pParent)->GetBitmap(path + "axspin2_en.bmp"),
				    ((CControlWnd*)m_pParent)->GetBitmap(path + "axspin2_dn.bmp"));

	m_pCtlUpBtn->ShowWindow(SW_HIDE);
	m_pCtlDownBtn->ShowWindow(SW_HIDE);

	int	width = 200;
	if (((CControlWnd*)m_pParent)->m_nGubn ==  GU_FCODE || ((CControlWnd*)m_pParent)->m_nGubn == GU_FOSTOCK)
		width = 300;

	m_pCodeList->Create(CRect(0, 0, width, 200), this);
	SetUnit(m_Unit);

	m_pCodeList->ShowWindow(SW_HIDE);
	m_pCodeList->Update();

	m_pBtnCode = std::make_unique<CfxImgButton>();
	m_pBtnList = std::make_unique<CfxImgButton>();
	m_pBtnCode->Create("", CRect(0, 0, 0, 0), this, IDC_CODE, FALSE, TRUE);
	m_pBtnList->Create("", CRect(0, 0, 0, 0), this, IDC_LIST, FALSE, TRUE);

	m_pBtnCode->SetCursor(GetCursor(10));
	m_pBtnList->SetCursor(GetCursor(10));
	if (((CControlWnd*)m_pParent)->m_bSearch)
		m_pBtnCode->ShowWindow(SW_SHOW);
	else
		m_pBtnCode->ShowWindow(SW_HIDE);

	return 0;
}

void CfxCodeCtrl::OnDestroy() 
{
	m_pCodeList.reset();
	m_pEdit.reset();
	m_pBtnCode.reset();
	m_pBtnList.reset();
	m_pCtlUpBtn.reset();
	m_pCtlDownBtn.reset();
	m_backColor.DeleteObject();
	m_NObackColor.DeleteObject();

	CWnd::OnDestroy();
}

void CfxCodeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//m_bFocusInit = TRUE;

	CWnd::OnLButtonDown(nFlags, point);
}

void CfxCodeCtrl::LButtonUp()
{
	m_btnList = false;
	InvalidateRect(&m_btnListRect);
}

void CfxCodeCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect	WinRC;
	GetClientRect(&WinRC);
	//dc.FillSolidRect(WinRC, RGB(140,140,140));
	CRect	btnRect;
	m_pBtnCode->GetClientRect(&btnRect);
	if (((CControlWnd*)m_pParent)->m_bSearch)
		WinRC.right -= (19+btnRect.Width());  
	else
		WinRC.right -= btnRect.Width(); 

	CPen pn; pn.CreatePen(PS_SOLID, 1, RGB(149,159,172));
	dc.SelectObject(&pn);
	
	if (IsWindowEnabled())
	{
		dc.SelectObject(&m_backColor);
		//dc.FillSolidRect(WinRC, CLR_BACKCOLOR);
	}
	else
	{
		dc.SelectObject(&m_NObackColor);
		//dc.FillSolidRect(WinRC, CLR_NOBACKCOLOR);
	}
	
	dc.Rectangle(WinRC);
//	dc.DrawEdge(&WinRC, EDGE_SUNKEN, BF_RECT);   
	/*
	dc.Draw3dRect(WinRC, ((CControlWnd*)GetParent())->GetIndexColor(110),
			     ((CControlWnd*)GetParent())->GetIndexColor(111));
	*/
}

void CfxCodeCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	if (m_pEdit)
	{
		m_pEdit->SetFocus();
		m_pEdit->SetSel(0, -1);
	}
}

void CfxCodeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	CRect	rect, editRC, btnRC;
	GetClientRect(&rect);

	//rect.DeflateRect(1,1);
	editRC = btnRC = rect;
	
	
	btnRC.left = btnRC.right - editRC.Height()-3+1;/*19*/;
	//btnRC.bottom = 21;
	//if ((btnRC.Width()/rect.Width()) > 18/65) 
	if (m_Unit == GU_FCODE || m_Unit == GU_FUTURE || m_Unit == GU_OPTION || m_Unit == GU_FOCODE)
		btnRC.left = rect.Width()-rect.Width()*18/80-1;
	else
		btnRC.left = rect.Width()-rect.Width()*18/65-1;
	
	m_btnCodeRect.CopyRect(btnRC);
	
	btnRC.OffsetRect(-17/*-16*/, 0);
	btnRC.right -= 2;
	m_btnListRect.CopyRect(btnRC);
	

	editRC.right = btnRC.left;
	//editRC.left += 2;
	//editRC.top += 6;

	if (m_pEdit) 
	{
		/*
		
		if (((CControlWnd*)m_pParent)->m_bSearch)
			m_pEdit->MoveWindow(editRC.left, editRC.top , editRC.Width()-1, editRC.Height() - 2);
		else
			m_pEdit->MoveWindow(editRC.left, editRC.top , editRC.Width()-1+19, editRC.Height() - 2);
		*/
		//ResizeEdit(editRC.Width()+editRC.Height(), editRC.Height());
		ResizeEdit(m_btnCodeRect.right, editRC.Height());

	}
	

	if (m_pBtnList && m_pBtnCode) 
	{
		if (((CControlWnd*)m_pParent)->m_bSearch)
		{
			m_pBtnList->MoveWindow(m_btnListRect);
			m_pBtnCode->MoveWindow(m_btnCodeRect);
			
		}
		else
		{
			m_btnCodeRect.left += 2;
			m_pBtnList->MoveWindow(m_btnCodeRect);
			m_pBtnCode->MoveWindow(m_btnCodeRect);
		}
	}
}

void CfxCodeCtrl::SetFont(CFont* pFont, bool redraw)
{
	m_pFont = pFont;
	if (m_pEdit)
		m_pEdit->SetFont(m_pFont); 
}

CString CfxCodeCtrl::GetEditData()
{
	CString	code;

	if (m_pEdit)
		m_pEdit->GetWindowText(code);

	return code;
}

CString CfxCodeCtrl::GetHistCode(int idx)
{
	//HistoryMode();
	if (m_pHCode.GetSize() <= 0)
	{
		return "";
	}

	if (idx==1)
	{
		if (m_pHCode.GetSize()==m_updnIndex) return "";
		if (m_pHCode.GetSize()<=m_updnIndex)//(m_pHCode.GetSize()>idx) && (idx>=0))
			 m_updnIndex = 1;
		_JCode	jCode = m_pHCode.GetAt(m_updnIndex);
		m_sCode = jCode.Code;//.Mid(1);
		m_updnIndex++;
		if (m_updnIndex>=m_pHCode.GetSize()) m_updnIndex = 1;
		return m_sCode;
	}else if (idx==-1)
	{
		HistoryMode();
		m_updnIndex = 1;
		_JCode	jCode = m_pHCode.GetAt(m_pHCode.GetSize()-1);
		m_sCode = jCode.Code;//.Mid(1);
		
		return m_sCode;
	}else
	{
		_JCode	jCode = m_pHCode.GetAt(idx);
		m_sCode = jCode.Code;//.Mid(1);
		
		return m_sCode;
	}


}

void CfxCodeCtrl::SetEditData(CString sData, bool bflag, bool bfocus)
{
	//AfxMessageBox("SetListCode");
	CString	str, symbol;
	if (m_pEdit == NULL)
	{
		return;
	}

	if (sData.Find("FOCUS") > -1)
	{
		if (IsWindowEnabled())
			SetEditFocus();
		return;
	}

	Msg("%s - %s", GetSymbol(), sData);
	if (sData.CompareNoCase(" ") != 0)
		sData.TrimRight();
	
	// ���� �ڵ�� ��...================================================================================
	str = ((CControlWnd*)m_pParent)->GetRtnStr();
//	if (!str.CompareNoCase(sData))
//		return;
	//====================================================================================================
	
	// �ڵ� ���缺 Ȯ�� ===================================================================================
	if (m_Unit == GU_CODE && !IsValidCode(sData) && !sData.IsEmpty())
		sData = str;
	else if (m_Unit == GU_ELWCODE && !sData.IsEmpty())
	{
		if (sData[0] < '5')
			sData = str;
		else if (sData[0] > '8')
			sData = str;
		else if (!IsValidCode(sData))
			sData = str;
	}
	else if (m_Unit == GU_FCODE || m_Unit == GU_FOSTOCK) 
	{
		if (sData.IsEmpty())
			sData = str;
	}
	else if (m_Unit == GU_BOND)
	{
		CString tmp = sData.Mid(0,2);

		if (tmp != "16" && tmp != "17" && tmp != "46" && tmp != "47" && tmp != "A6" && tmp != "A7" && tmp != "D6" && tmp != "D7") //�Ļ��ǰ �ڵ尳��
			sData = str;
	}
	if (m_Unit == GU_CODE) sData = sData.Mid(0,6);
	//=====================================================================================================
	m_pEdit->SetWindowText(sData);	
	((CControlWnd*)m_pParent)->SetRtnStr(sData);
	
	// �ʱ⿡ �̺�Ʈ �߻� �ѹ��� ==========================================================================
	if (!m_bFirst && ((m_Unit == GU_CODE) || (m_Unit == GU_ELWCODE)|| (m_Unit == GU_FOCODE) 
		       || (m_Unit == GU_FUTURE) || (m_Unit == GU_OPTION) || (m_Unit == GU_FCODE) || (m_Unit == GU_FOSTOCK))) 
	{
		m_bFirst = true;
		/*
		if (m_Unit == GU_CODE)
			((CControlWnd*)m_pParent)->Send(sData);

		str = ((CControlWnd*)m_pParent)->Variant(historyCC, GetSymbol());
		if (!str.IsEmpty())
			return;
		*/
	}
	//=====================================================================================================

	// Ʈ���Ź� �����丮 ó�� ==============================================================================
	symbol = GetSymbol();				
	if (!symbol.IsEmpty() && !sData.IsEmpty())
	{
		if (m_Unit == GU_ELWCODE)
		{
			if (sData[0] < '5')
				return;
			else if (sData[0] > '8')
				return;
			else
			{
				if (sData.CompareNoCase(str) != 0)
				{
					str.Format("%s\t%s", symbol, sData);
					((CControlWnd*)m_pParent)->Variant(codeCC, str);
					((CControlWnd*)m_pParent)->Variant(triggerCC, str);
				}
			}
		}
		else if (m_Unit == GU_CODE)
		{
//			if (sData[0] >= '5' && sData[0] <= '9')
			if (sData.CompareNoCase(str) != 0)
			{
				str.Format("1301\t%s", sData);
				((CControlWnd*)m_pParent)->Variant(codeCC, str);
				((CControlWnd*)m_pParent)->Variant(triggerCC, str);
				//KDK 2013.05.14 KONEX
				struct _JCode jCode;

				for (int i=0;i<m_pJCode.GetSize();i++)
				{
					jCode = m_pJCode.GetAt(i);
					
					if (jCode.Code.Mid(1) == sData)
					{
						if (jCode.Ssgb == jmKONEX)	
						{
							m_bKONEX = TRUE;
						}
						else
						{
							m_bKONEX = FALSE;
						}
						break;
					}
				}
				//KDK 2013.05.14 KONEX END
			}
		}
		else if (m_Unit == GU_FCODE)
		{
			//			if (sData[0] >= '5' && sData[0] <= '9')
			if (sData.CompareNoCase(str) != 0)
			{
				str.Format("%s\t%s", symbol, sData);

				struct _JCode jCode;

				jCode.Code = sData;
				//jCode.Name = ((CControlWnd*)m_pParent)->GetHNam(sData);
				jCode.Name = "";

				for (int i=0;i<m_pJCode.GetSize();i++)
				{
					jCode = m_pJCode.GetAt(i);

					if (jCode.Code == sData)
					{
						jCode.Name.TrimRight();
						break;
					}

					jCode.Code = "";
					jCode.Name = "";
				}

				if (jCode.Name == "")
				{
					HistoryMode();

					CString strCode = GetHistCode(0);

					m_pEdit->SetWindowText(strCode);
					((CControlWnd*)m_pParent)->SetRtnStr(strCode);
					return;
				}

				m_pHCode.Add(jCode);

				CString strHis;
				strHis.Format("%s\t%s", jCode.Code, jCode.Name);
				strHis.TrimRight();
				
				AddSFString(strHis);
				((CControlWnd*)m_pParent)->Variant(triggerCC, str);

				WriteHistory(m_sPath);
			}
		}
		else if (m_Unit == GU_FOREIGN)
		{
			if (!strlen(sData) || !strlen(m_Name))
				return;
			
			HistoryMode();
			
			CString history = m_sHistory;
			CString fileName;
			fileName.Format("%s/%s/%s/%s", m_sPath, USRDIR, ((CControlWnd*)GetParent())->Variant(nameCC, ""),FOREIGN_HISTORY);
			
			if (!history.IsEmpty()) 
			{
				TRY 
				{
					CFile::Remove(fileName);
				}
				CATCH(CFileException, e) 
				{
				}
				END_CATCH;
			}
			
			_JCode	jCode;
			CArray<_JCode, _JCode> arr;
			CString	str;
			int	find = 0;
			
			jCode.Code = sData;
			jCode.Name = m_Name;
			
			arr.Add(jCode);
			
			while (TRUE)
			{
				find = history.Find('\t');
				if (find < 1) 
					break;
				
				str = history.Left(find);
				history.Delete(0, find + 1);
				
				find = str.Find(' ');
				if (find > 0) {
					jCode.Code = str.Left(find);
					jCode.Name = str.Mid(find + 1);
					
					if (jCode.Code != sData)
						arr.Add(jCode);
				}
			}
			
			if (arr.GetSize() > 25) 
			{
				arr.SetSize(25);
			}
			
			history.Empty();
			
			for (int i = 0; i < arr.GetSize(); i++) {
				str.Format("%s %s\t", arr[i].Code, arr[i].Name);
				history += str;
			}
			
			CFile* pFile;
			
			TRY 
			{
				pFile = new CFile(fileName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone);
			}
			CATCH (CFileException, e) 
			{
				return;
			}
			END_CATCH;
			
			pFile->Write((LPCTSTR)history, history.GetLength());
			
			delete pFile;
		}
		else
		{
			str.Format("%s\t%s", symbol, sData);
			((CControlWnd*)m_pParent)->Variant(codeCC, str);
			((CControlWnd*)m_pParent)->Variant(triggerCC, str);
		}

	}
	// ======================================================================================================
	if (bfocus)
		((CControlWnd*)m_pParent)->m_bFocus = true;
	else
		((CControlWnd*)m_pParent)->m_bFocus = false;

	if (!bflag)	
		FireChangeEvent();

	if (m_Unit == GU_CODE)		
		((CControlWnd*)m_pParent)->Send(sData);
	else				
		m_pParent->SetTimer(100, 200, NULL);

}

void CfxCodeCtrl::FireChangeEvent()
{
	const	int     key = ((CControlWnd*)m_pParent)->m_Param.key;
	CString str = ((CControlWnd*)m_pParent)->m_Param.name;
	
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(key, evOnChange)), 
				(LPARAM)str.GetString());

	if (((CControlWnd*)m_pParent)->m_bFocus)
		m_pEdit->SetSel(0, -1);
}

void CfxCodeCtrl::SetUnit(int unit)
{
	if (m_Unit == unit) return;
	
	m_Unit = unit;

	if (m_pEdit == NULL)
		return;	
//	CString code = GetEditData();
	
	m_pEdit->SetWindowText("");
	m_pEdit->SetUnit(m_Unit);

	switch (m_Unit)
	{
	case GU_FCODE:
		loadSfCode(m_sPath);
		break;
	case GU_FUTURE:
	case GU_OPTION:
	case GU_POPTION:
	case GU_FOCODE:
		break;
	case GU_ELWCODE:
		ELWCodeLoad(m_sPath);
		break;
	case GU_CODE:
	default:
		JCodeLoad(m_sPath);
		break;
	}
}

void CfxCodeCtrl::SetHistory(CString sHistory)
{
	m_sHistory = sHistory; 
}

void CfxCodeCtrl::HistoryMode()
{
	CString usr;
	usr = ((CControlWnd*)GetParent())->Variant(nameCC, "");
	
	if (m_Unit == GU_FCODE)
	{
		m_sHistory = "";

		CStdioFile sFile;
		CString strPath;
		CString strValue;

		strPath.Format("%s/%s/%s/%s", m_sPath, USRDIR, usr,SFCODE_HISTORY);
		if (sFile.Open(strPath,CFile::modeRead | CFile::typeText | CFile::shareDenyNone))
		{
			while (sFile.ReadString(strValue))
			{
				m_sHistory += strValue;
			}
			sFile.Close();
		}
	}
	else if (m_Unit == GU_FOREIGN)
	{
		m_sHistory = "";
		
		CStdioFile sFile;
		CString strPath;
		CString strValue;

		strPath.Format("%s/%s/%s/%s", m_sPath, USRDIR, usr,FOREIGN_HISTORY);
		if (sFile.Open(strPath,CFile::modeRead | CFile::typeText | CFile::shareDenyNone))
		{
			while (sFile.ReadString(strValue))
			{
				m_sHistory += strValue;
			}
			sFile.Close();
		}
	}
	else
	{
		m_sHistory = ((CControlWnd*)GetParent())->Variant(historyCC, GetSymbol());
	}

	InitHistory(m_sHistory, m_Unit);
}

void CfxCodeCtrl::CodeListMode(CString sFind)
{
	InitCodeList(m_Unit,sFind);
}

void CfxCodeCtrl::SetValidCheck(bool bValid)
{
	m_pEdit->m_bValidCheck	= bValid;
	//	m_bValidCheck =bValid;
}

void CfxCodeCtrl::SetComboBtnBmp(CString sCode,CString sCodedown,CString sList,CString sListdown)
{
	m_sbmpCode = sCode;	m_sbmpCodepush = sCodedown;
	m_sbmpList = sList;	m_sbmpListpush = sListdown;

	m_sbmpCodeEn = m_sbmpCodepush;
	m_sbmpListEn = m_sbmpListpush;
	m_sbmpCodeEn.Replace("dn", "en");
	m_sbmpListEn.Replace("dn", "en");

	CString root;

	root.Format("%s\\%s\\", m_sPath, IMAGEDIR);
	if (m_pBtnCode && m_pBtnList)
	{
		m_pBtnCode->SetImgBitmap(((CControlWnd*)m_pParent)->GetBitmap(root + m_sbmpCode),
					((CControlWnd*)m_pParent)->GetBitmap(root + m_sbmpCodepush),
					((CControlWnd*)m_pParent)->GetBitmap(root + m_sbmpCodeEn));

		m_pBtnList->SetImgBitmap(((CControlWnd*)m_pParent)->GetBitmap(root + m_sbmpList),
					((CControlWnd*)m_pParent)->GetBitmap(root + m_sbmpListpush),
					((CControlWnd*)m_pParent)->GetBitmap(root + m_sbmpListEn));
	}
}

void CfxCodeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_btnDown = false;
	InvalidateRect(&m_btnCodeRect);

	m_btnList = false;
	InvalidateRect(&m_btnListRect);

	CWnd::OnLButtonUp(nFlags, point);
}

void CfxCodeCtrl::GetRect(CRect &rc) 
{
	if (m_pCodeList)
		m_pCodeList->GetWindowRect(rc);
}

void CfxCodeCtrl::OnMove(int x, int y) 
{
	CCodeCombo::OnMove(x, y);
	// TODO: Add your message handler code here
}

void CfxCodeCtrl::SetEditFocus()
{
	if (m_pEdit)
		m_pEdit->SetFocus();
}

void CfxCodeCtrl::SetAxFocus(bool bflag)
{
	((CControlWnd*)m_pParent)->SetAxFocus(bflag);
}

CString CfxCodeCtrl::GetSymbol()
{
	CString symbol;

	switch (m_Unit) {
	case GU_CODE:		symbol = "1301";	break;	
	case GU_FUTURE:		symbol = "30301";	break;
	case GU_OPTION:		symbol = "40301";	break;
	case GU_INDEX:		symbol = "20301";	break;
	case GU_POPTION:	symbol = "70301";	break;
	case GU_FOCODE:		symbol = "ed_focod";	break;
	case GU_ELWCODE:	symbol = "1301";	break;
	case GU_SCODE:		symbol = "scode";	break;
	case GU_SBOND:		symbol = "smcd";	break;
	case GU_BOND:		symbol = "ed_fcod";        break;
	case GU_FCODE:		symbol = "100301";	break;
	case GU_FOSTOCK:	symbol = "ed_fpcod";	break; 
	case GU_FOREIGN:	symbol = "gscode";	break;
	default:		symbol = "";		break;
	}
	return symbol;
}


void CfxCodeCtrl::SetListCode(CString sData)
{
	m_arrList.RemoveAll();
	m_nIndex = 0;

	m_pBtnCode->ShowWindow(SW_HIDE);
	m_pBtnList->ShowWindow(SW_HIDE);
	

	while (!sData.IsEmpty())
		m_arrList.Add(((CControlWnd*)m_pParent)->Parser(sData, "\n"));
	
	const	int	cx = m_btnCodeRect.Width();
	const	int	cy = m_btnCodeRect.Height()/2;

	m_pCtlUpBtn->SetWindowPos(NULL, m_btnCodeRect.left + 1, 1, cx, cy, SWP_SHOWWINDOW);
	m_pCtlDownBtn->SetWindowPos(NULL, m_btnCodeRect.left + 1, cy, cx, cy , SWP_SHOWWINDOW);

	if (m_arrList.GetSize())
	{
		SetEditData(m_arrList.GetAt(0));
		m_pEdit->SetSel(0, -1);
		m_pEdit->SetFocus();
	}

	m_bHistory = false;
	Invalidate();
}

void CfxCodeCtrl::SetNormal()
{
	if (((CControlWnd*)m_pParent)->m_bSearch)
		m_pBtnCode->ShowWindow(SW_SHOW);
	else
		m_pBtnCode->ShowWindow(SW_HIDE);
	//m_pBtnCode->ShowWindow(SW_SHOW);
	m_pBtnList->ShowWindow(SW_SHOW);

	m_pCtlUpBtn->ShowWindow(SW_HIDE);
	m_pCtlDownBtn->ShowWindow(SW_HIDE);

	m_bHistory = true;
	m_arrList.RemoveAll();
}

void CfxCodeCtrl::OnBtnDown()
{
	if (m_arrList.GetSize() <= 0)
		return;
	SetValidCheck(false);
	if (m_nIndex < m_arrList.GetSize() - 1)	m_nIndex++;
	SetEditData(m_arrList.GetAt(m_nIndex));
	m_pEdit->SetSel(0, -1);
}

void CfxCodeCtrl::OnBtnUp()
{
	if (m_arrList.GetSize() <= 0)
		return;

	SetValidCheck(false);
	if (m_nIndex > 0)	m_nIndex--;
	SetEditData(m_arrList.GetAt(m_nIndex));
	m_pEdit->SetSel(0, -1);
}

void CfxCodeCtrl::OnCodeDown()
{
	CString	code, str;
	UINT	flags = allCODE;
	int	type = 0;
	CPoint pt(m_btnCodeRect.left, m_btnCodeRect.bottom);

	ClientToScreen(&pt);
	m_updnIndex = 1;

	/*
	if (m_Unit == GU_ELWCODE)
	{
		str.Format("hdselwdg/S/s/t?/P%d", MAKELONG(pt.x,pt.y));
		((CControlWnd*)m_pParent)->OpenView(str, typePOPUP);
	}
	else
	*/
	{
		switch (m_Unit)
		{
		case GU_CODE:		flags = allCODE;		   break;	// �ֽ�
		case GU_FUTURE:		flags = futureCODE;		   break;	// ���� 
		case GU_OPTION:		flags = optionCODE;		   break;	// �ɼ� 
		case GU_POPTION:	flags = koptionCODE;		   break;	// �ֽ� �ɼ� 
		case GU_FOCODE:		flags = foptionCODE;	type = 2;  break;	// ���� + �ɼ� 
		case GU_INDEX:		flags = indexCODE;		   break;	// ���� 
		case GU_SCODE:		flags = sinjuCODE;	type = 4;  break;	// �����μ��� 
		case GU_BOND:		flags = goodFOCODE;		   break;	// ä�� 
		case GU_SBOND:		flags = smbondCODE;		   break;	// �Ҹ�ä��
		case GU_ELWCODE:	flags = elwCODE;		   break;	// elwcode
		case GU_FCODE:		flags = sfCODE;		type = 20; break;	// �ֽļ���
		case GU_FOSTOCK:	flags = sfCODE;		type = 52; break;	// �ֽļ��� + �ɼ� 
		case GU_FOREIGN:	flags = usCODE;		break;
		default:		break;
		}
		
		code = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(flags, type)), 
							      MAKELPARAM(pt.x, pt.y));
		SetValidCheck(false);
		if (!code.IsEmpty())
			SetEditData(((CControlWnd*)m_pParent)->Parser(code, "\t"));
		m_Name = code;
	}
}

void CfxCodeCtrl::OnListDown()
{
	m_updnIndex = 1;
	if (!m_bHistory)
		return;	

	HistoryMode();
	m_bValidCheck = false;
	ShowDropDown(true);
	SetAxFocus(true);
	if (GetCount() > 0)
		SetCurSel(0);
}

HCURSOR CfxCodeCtrl::GetCursor(int kind)
{
	_ASSERT(m_pWizard);
	return (HCURSOR)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(getCURSOR, 0), (LPARAM)kind);
}

void CfxCodeCtrl::GoDown()
{
	if (m_pCodeList->IsWindowVisible())
	{
		m_pCodeList->SetCurSel(m_pCodeList->GetCurSel() + 1);

		if (m_pCodeList->GetCurSel() - m_pCodeList->GetTopIndex() + 1 > 12)
			m_pCodeList->SetTopIndex(m_pCodeList->GetCurSel() - 11);
	}
}

void CfxCodeCtrl::GoUp()
{
	if (m_pCodeList->IsWindowVisible())
	{
		m_pCodeList->SetCurSel(m_pCodeList->GetCurSel() - 1);

		if (m_pCodeList->GetCurSel() - m_pCodeList->GetTopIndex() < 0)
			m_pCodeList->SetTopIndex(m_pCodeList->GetCurSel());
	}
}

void CCodeEdit::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TID_CHANGEHANGLE)
	{		
		KillTimer(TID_CHANGEHANGLE);
		ChangeHangulMode(this->m_hWnd, false);
	} else if (nIDEvent == TID_RETURNTIDLE)
	{
		KillTimer(TID_RETURNTIDLE);
		/*
		m_pParent->m_pParent->PostMessage(WM_KEYDOWN, VK_RETURN, 0);
		m_pParent->FireChangeEvent();
		*/
	}

	CDDEdit::OnTimer(nIDEvent);
}

void CCodeEdit::OnDestroy() 
{
	CDDEdit::OnDestroy();
	
	KillTimer(TID_CHANGEHANGLE);	
}

void CfxCodeCtrl::ResizeEdit(int cx, int cy)
{
	if (!m_pEdit)
		return;
	CRect rc; GetClientRect(&rc);
	//cx = rc.Width();
	cy = rc.Height();
	CRect rcEdit;
	int nFtHeight = 0;
	
	int sz = 0;

	
	sz = (int)((cy/2)-1)<(cx/8 - MARGIN)?((cy/2)-1):(cx/8 - MARGIN) ;
	//((CControlWnd*)m_pParent)->m_Param.point = sz;
	CFont* pFont = ((CControlWnd*)m_pParent)->GetAxFont(((CControlWnd*)m_pParent)->m_Param.fonts, ((CControlWnd*)m_pParent)->m_Param.point, ((CControlWnd*)m_pParent)->m_Param.style);
	m_pEdit->SetFont(pFont);
	m_pCodeList->SetItemFont(pFont);

	if (pFont)
	{
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		
		nFtHeight = abs(lf.lfHeight);
	}
	/*
	else
		nFtHeight = cy;
	*/
	
	int yMargin = 0;
	yMargin = (cy-nFtHeight)/2;
	//TRACE("nFtHeight: %d, yMargin: %d\n", nFtHeight, yMargin);
	
	//rcEdit.SetRect(MARGIN, cy / 2 - nFtHeight / 2, cx - MARGIN, cy / 2 + nFtHeight / 2);
	//CRect rc; GetClientRect(&rc);
	if (yMargin<2) yMargin = 2;
	CRect bRc;
	m_pBtnCode->GetClientRect(&bRc);
	
	rcEdit.SetRect(MARGIN, yMargin, rc.Width() - bRc.Width() - MARGIN*2, cy-yMargin);
	
	m_pEdit->MoveWindow(rcEdit);
	
}

void CfxCodeCtrl::ResizeCtrl()
{
	CRect	rect, editRC, btnRC;
	GetClientRect(&rect);

	//rect.DeflateRect(1,1);
	editRC = btnRC = rect;
	
	
	btnRC.left = btnRC.right - editRC.Height()+1;/*19*/;
	//btnRC.bottom = 21;
	//if ((btnRC.Width()/rect.Width()) > 18/65) 
	if (m_Unit == GU_FCODE || m_Unit == GU_FUTURE || m_Unit == GU_OPTION || m_Unit == GU_FOCODE)
		btnRC.left = rect.Width()-rect.Width()*18/80-1;
	else
		btnRC.left = rect.Width()-rect.Width()*18/65-1;
	
	m_btnCodeRect.CopyRect(btnRC);
	
	
	
	btnRC.OffsetRect(-17/*-16*/, 0);
	btnRC.right -= 2;
	m_btnListRect.CopyRect(btnRC);
	

	editRC.right = btnRC.left;
	//editRC.left += 2;
	//editRC.top += 6;


	

	if (m_pBtnList && m_pBtnCode) 
	{
		if (((CControlWnd*)m_pParent)->m_bSearch)
		{
			m_pBtnList->MoveWindow(m_btnListRect);
			m_pBtnCode->MoveWindow(m_btnCodeRect);
			
		}
		else
		{
			m_btnCodeRect.left += 2;
			m_pBtnList->MoveWindow(m_btnCodeRect);
			m_pBtnCode->MoveWindow(m_btnCodeRect);
		}
	}
	if (m_pEdit) 
	{
		/*
		
		if (((CControlWnd*)m_pParent)->m_bSearch)
			m_pEdit->MoveWindow(editRC.left, editRC.top , editRC.Width()-1, editRC.Height() - 2);
		else
			m_pEdit->MoveWindow(editRC.left, editRC.top , editRC.Width()-1+19, editRC.Height() - 2);
		*/
		ResizeEdit(editRC.Width()-2+editRC.Height(), editRC.Height());
	}
}

void CCodeEdit::OnEnable(BOOL bEnable) 
{
	CDDEdit::OnEnable(bEnable);
	// TODO: Add your message handler code here
	CRect rc; GetClientRect(&rc);

}

HBRUSH CCodeEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDDEdit::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/*
	if (IsWindowEnabled())
	{
		pDC->SetBkColor(CLR_BACKCOLOR);
		return m_backColor;
	}
	else
	{
		pDC->SetBkColor(CLR_NOBACKCOLOR);
		return m_NObackColor;
	}
	*/

	return hbr;
	//return m_NObackColor;
	
	// TODO: Return a different brush if the default is not desired
	
}

HBRUSH CfxCodeCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CCodeCombo::OnCtlColor(pDC, pWnd, nCtlColor);
	const	int	nID = pWnd->GetDlgCtrlID();

	if (nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SetBkColor(CLR_BACKCOLOR);
		return m_backColor;
	}   
	else if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (nID != IDC_STATIC)
		{
			pDC->SetBkColor(CLR_NOBACKCOLOR);
			return m_NObackColor;
		}
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CfxCodeCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CCodeCombo::OnKillFocus(pNewWnd);
}

BOOL CCodeEdit::PreTranslateMessage(MSG* pMsg) 
{
	return CDDEdit::PreTranslateMessage(pMsg);
}
