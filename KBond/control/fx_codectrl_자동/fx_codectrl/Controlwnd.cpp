// Controlwnd.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "fx_codectrl.h"
#include "Controlwnd.h"


// CControlwnd
#define TM_TYPEBAL 9898    //2�ڸ� �����ڵ�
#define TM_TYPELIST 9897   //6�ڸ� ȸ���ڵ�

IMPLEMENT_DYNAMIC(CControlwnd, CWnd)

CControlwnd::CControlwnd()
{

	m_pEdit = NULL;
	EnableAutomation();
	m_pBitmap[MsNo] = NULL;
	m_pBitmap[MsOver] = NULL;
	m_pBitmap[MsDown] = NULL;

	m_bDown = FALSE;
	m_bHover = FALSE;
	m_bTracking = FALSE;
	m_MouseState = MsNo;
	m_sSymbol = _T("vcod");
	m_bonlyBal = false;
	m_bShowCodeList = false;
	m_mapCodeAg.RemoveAll();
}

CControlwnd::~CControlwnd()
{
}

void CControlwnd::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ�
	// OnFinalRelease�� ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CWnd::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CControlwnd, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlwnd, CWnd)
	DISP_FUNCTION_ID(CControlwnd, "SetName", dispidSetdata, SetName, VT_EMPTY, VTS_BSTR)
	DISP_PROPERTY_EX_ID(CControlwnd, "name", dispidname, Getname, Setname, VT_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "GetCode", dispidGetCode, GetCode, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CControlwnd, "SetType", dispidSetType, SetType, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "SetHsymbol", dispidSetHsymbol, SetHsymbol, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "SetVisible", dispidSetVisible, SetVisible, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CControlwnd, "SetEnable", dispidSetEnable, SetEnable, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CControlwnd, "SetBalListType", dispidSetBalListType, SetBalListType, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "ResetCondition", dispidResetCondition, ResetCondition, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// ����: IID_IControlwnd�� ���� ������ �߰��Ͽ�
//  VBA���� ���� ���� ���ε��� �����մϴ�. 
//  �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {3EBBD33B-2F57-4ACA-8C33-D82219DCE4F4}
static const IID IID_IControlwnd =
{ 0x3EBBD33B, 0x2F57, 0x4ACA, { 0x8C, 0x33, 0xD8, 0x22, 0x19, 0xDC, 0xE4, 0xF4 } };

BEGIN_INTERFACE_MAP(CControlwnd, CWnd)
	INTERFACE_PART(CControlwnd, IID_IControlwnd, Dispatch)
END_INTERFACE_MAP()


// CControlwnd �޽��� ó�����Դϴ�.




LRESULT CControlwnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_DOMINO:
		{

		}
		break;
	case DLL_TRIGGER:
		{
			CString strData = (LPCTSTR)lParam;
			AfxMessageBox(strData);
		}
		break;
	case DLL_SETFONT:
	case DLL_SETFONTx:
		{
			CString tmp( (LPCTSTR)lParam );
			int ipoint = HIWORD(wParam);
			Invalidate();
		}
		break;
	}
	
	return 0;
}

void CControlwnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (cx > 0 && cy > 0)
		ResizeControls();
}

void CControlwnd::ResizeControls()
{
	CRect	rect, editRC, btnRC;
	GetClientRect(&rect);

	rect.DeflateRect(1,1);
	editRC = btnRC = rect;
	btnRC.left = btnRC.right - btnRC.Height() + 1;

	editRC.right = btnRC.left;
	editRC.left += 1;
	editRC.top += 1;

	CSize	sz = CSize(0, 0);

	CDC	*pDC = GetDC();
	CFont	*sFont = pDC->SelectObject(&m_Font);
	sz = pDC->GetOutputTextExtent("��");
	pDC->SelectObject(sFont);
	ReleaseDC(pDC);

	if(m_pEdit != NULL)
	{
		m_pEdit->MoveWindow(editRC.left, editRC.top, editRC.Width(), editRC.Height());
		m_pEdit->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}

	if (sz.cy > 0 && m_pEdit != NULL)
	{
		if (editRC.Height() > sz.cy)
		{
			int	y = (editRC.Height() - sz.cy)/2;
			editRC.DeflateRect(y, y);
			m_pEdit->MoveWindow(editRC.left, editRC.top, editRC.Width(), editRC.Height());
			m_pEdit->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		}
	}

	m_btnRect = btnRC;
}

void CControlwnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect	WinRC;
	GetClientRect(&WinRC);
	if (m_pBitmap[m_MouseState])
	{
		dc.FillSolidRect(WinRC, RGB(179, 179, 179));
		WinRC.DeflateRect(1, 1, 1, 1);
		dc.FillSolidRect(WinRC, RGB(255, 255, 255));

		if (m_bDown)
			DrawBitmap(&dc, m_btnRect, m_pBitmap[MsDown]);
		else if (m_bHover)
			DrawBitmap(&dc, m_btnRect, m_pBitmap[MsOver]);
		else
			DrawBitmap(&dc, m_btnRect, m_pBitmap[m_MouseState]);
	}
	else
	{
		dc.FillSolidRect(WinRC, GetSysColor(COLOR_WINDOW));
		dc.DrawEdge(&WinRC, EDGE_SUNKEN, BF_RECT);   

		if (m_bDown)
			dc.DrawFrameControl(&m_btnRect, DFC_SCROLL, DFCS_SCROLLCOMBOBOX|DFCS_PUSHED);
		else
			dc.DrawFrameControl(&m_btnRect, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
	}
}

void CControlwnd::DrawBitmap(CDC *pDC, CRect rc, CBitmap *pBitmap)
{
	if (pBitmap)
	{
		CDC		memDC;
		CBitmap		*pSBitmap;

		BITMAP stBitmapInfo;
		pBitmap->GetBitmap(&stBitmapInfo);

		memDC.CreateCompatibleDC(pDC);	
		if (memDC.m_hDC != NULL) 
			pSBitmap = (CBitmap*)memDC.SelectObject(pBitmap);

		::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, stBitmapInfo.bmWidth, stBitmapInfo.bmHeight, 
			RGB(255, 0, 255));
		//pDC->StretchBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, stBitmapInfo.bmWidth, stBitmapInfo.bmHeight, SRCCOPY);

		memDC.SelectObject(pSBitmap);
		memDC.DeleteDC();
	}
	else
	{
		pDC->FillSolidRect(rc, RGB(196,196,196));
	}
}

CString CControlwnd::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CControlwnd::SetParam(_param *pParam)
{
	m_Param.key   = pParam->key;
	m_Param.name  = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect  = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB  = pParam->tRGB;
	m_Param.pRGB  = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	CString tmps, strdata;
	tmps = m_Param.options;
	strdata = Parser(tmps, _T("/g"));
	m_Unit = atoi(tmps);
}


void CControlwnd::ResetAuthAg()
{
	CString tmpS, temp;

	CString strUserPath(_T(""));
	CString strFilePath(_T(""));


	strUserPath.Format(_T("%s\\user\\%s"), m_strHome, m_sUser);

	strFilePath.Format(_T("%s\\%s"), strUserPath, _T("authcompany.ini"));

	WritePrivateProfileString(_T("auth"), _T("company"), "", (LPCTSTR)strFilePath);
	WritePrivateProfileString(_T("auth"), _T("chechYN"), "", (LPCTSTR)strFilePath);

	WritePrivateProfileString(_T("auth"), _T("listcompany"), "", (LPCTSTR)strFilePath);
	WritePrivateProfileString(_T("auth"), _T("balchechYN"), "", (LPCTSTR)strFilePath);
	
	tmpS.ReleaseBuffer();
}


int CControlwnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	m_pEdit = new CCodeEdit(this, m_parent);
	m_pEdit->Create(ES_AUTOHSCROLL|WS_VISIBLE|WS_CHILD|ES_LEFT, CRect(0,0,10,10), this, 1001);
	m_pEdit->m_bValidCheck = m_bValidCheck;

	m_pCombo = new CCodeCombo(this);
	m_pCombo->Create(CBS_OWNERDRAWVARIABLE|CBS_DROPDOWNLIST|WS_CHILD|WS_VISIBLE|WS_VSCROLL, CRect(0, 0, 0, 212), this, 0);
	m_pCombo->SetItemHeight(-1, 0);
	
	if(m_Unit == 0)
		m_pCombo->SetDroppedWidth(180);
	else
		m_pCombo->SetDroppedWidth(250);

	m_pCombo->m_pWizard = m_pWizard;
	m_pCombo->m_Unit = m_Unit;

	CClientDC dc(this);

	m_Font.CreatePointFont(90, "����ü", &dc);
	
	SetFont(&m_Font, TRUE);

	m_pEdit->SetFont(&m_Font, true);
	m_pCombo->SetFont(&m_Font, true);
	Invalidate();

	m_strHome = (char*)m_pWizard ->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), NULL);
	m_sUser = (char*)m_pWizard ->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), NULL);
	ResetAuthAg();

	if (m_pBitmap[MsNo] == NULL)
		m_pBitmap[MsNo] = GetBitmap(m_strHome + _T("\\image\\AXEDIT.bmp"));
	if (m_pBitmap[MsOver] == NULL)
		m_pBitmap[MsOver] = GetBitmap(m_strHome + _T("\\image\\AXEDIT_En.bmp"));
	if (m_pBitmap[MsDown] == NULL)
		m_pBitmap[MsDown] = GetBitmap(m_strHome + _T("\\image\\AXEDIT_Dn.bmp"));

	LoadAuthAg();
	SetUnit(m_Unit);
	return 0;
}

void CControlwnd::SetUnit(int unit)
{
	m_Unit = unit;
	m_pEdit->SetWindowText("");
	
	m_pEdit->SetUnit(m_Unit);

	m_pCombo->ResetContent();
	m_pCombo->ResetCode();
	m_pCombo->InitHistory("", "");
	m_pCombo->SetUnit(m_Unit);

	switch(m_Unit)
	{
		case 0:
			{
				m_pCombo->CodeLoad(m_strHome);
				m_sHistory = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, historyCC), (long)m_sSymbol.operator LPCTSTR());
			}	
			break;
		case 1:
			{
		
			}
			break;
	}
}

CBitmap* CControlwnd::GetBitmap(CString path)
{
	return (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

void CControlwnd::HistoryMode()
{
	switch (m_Unit)
	{
		case 1:
			m_sSymbol = _T("1301");
			break;
	}

	m_sHistory = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, historyCC), (long)m_sSymbol.operator LPCTSTR());

	m_pCombo->InitHistory(m_sSymbol, m_sHistory);
}

void CControlwnd::LButtonUp()
{
	m_bDown = FALSE;
	Invalidate();
	InvalidateRect(&m_btnRect);
}

void CControlwnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
	if (m_btnRect.PtInRect(point))
	{
		CWnd *pWnd = GetFocus();
	//	m_parent->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
		m_bDown = TRUE;
		InvalidateRect(&m_btnRect);
		RedrawWindow();

		HistoryMode();
		
		m_pCombo->ShowDropDown(TRUE);
		if (m_pCombo->GetCount() > 0)
		{
			m_pCombo->SetCurSel(0);
			m_pCombo->SetFocus();
		}

	}

	Invalidate();
	UpdateWindow();
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CControlwnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	m_bDown = FALSE;
	Invalidate();
	CWnd::OnLButtonUp(nFlags, point);
}

LRESULT CControlwnd::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bHover = true;
	InvalidateRect(&m_btnRect);
	return 0;
}

LRESULT CControlwnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = false;
	m_bHover = false;
	InvalidateRect(&m_btnRect);
	return 0;
}

void CControlwnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if(!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime =1;
		m_bTracking = _TrackMouseEvent(&tme);

	}
	CWnd::OnMouseMove(nFlags, point);
}

void CControlwnd::CodeListMode()
{
	if (!m_pCombo->IsCodeListMode())
		m_pCombo->InitCodeList();
}

void CControlwnd::SetValidCheck(bool bValid)
{
	m_bValidCheck =bValid;
}

void CControlwnd::MouseDrag(CString strData)
{
	CString strSymbol(_T(""));
	CString strText;
	
	if (!m_bDrag)
		return;

	if (m_Unit != 0)
		return;

//	strText.Format("1301\t%s", strData);
//	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(dragDLL, strText.GetLength()), (LPARAM)strText.operator LPCTSTR());
}

void CControlwnd::SetDrag(BOOL nDrag/* = false*/)
{
	m_bDrag = nDrag;
//	m_pEdit->m_bDrag = nDrag;
}

void CControlwnd::OnDestroy()
{
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
	CWnd::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CControlwnd::SetEditData(CString sData)
{
	sData.TrimLeft();
	sData.TrimRight();
	m_pEdit->SetWindowText(sData);
}

void CControlwnd::SetName(LPCTSTR data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ���⿡ ����ġ ó���⸦ �߰��մϴ�.
	CString tmpS;
	tmpS.Format(_T("%s"), data);
	SetEditData(tmpS);
}

BSTR CControlwnd::Getname(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: ���⿡ ����ġ ó���⸦ �߰��մϴ�.
	m_pEdit->GetWindowText(strResult);
	return strResult.AllocSysString();
}

void CControlwnd::Setname(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString tmpS;
	tmpS.Format(_T("%s"), newVal);
	SetEditData(tmpS);
	// TODO: ���⿡ �Ӽ� ó���� �ڵ带 �߰��մϴ�.
}

BSTR CControlwnd::GetCode(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: ���⿡ ����ġ ó���⸦ �߰��մϴ�.
	//m_pJCode
	strResult = m_pEdit->FindCode();


	return strResult.AllocSysString();
}

void CControlwnd::Trigger(CString strparam)
{
	CString strdata;
	strdata.Format("%s\t%s", m_sSymbol, strparam); 
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, triggerCC), (LPARAM)strparam.operator LPCTSTR());
}

void CControlwnd::SendCtrlMsg(int igubn)
{
	switch(igubn)
	{
		case 0:
				m_pWizard ->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnClick)), (long)m_Param.name.operator LPCTSTR());
			break;
		case 1:
				m_pWizard ->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)), (long)m_Param.name.operator LPCTSTR());
			break;
		case 2:
				m_pWizard ->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange)), (long)m_Param.name.operator LPCTSTR());
			break;
	}
	
}


void CControlwnd::SetBalListType(LPCTSTR list, LPCTSTR type)
{//6�ڸ� ȸ���ڵ�� ���ڵ带 ���������� ����
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CString tmpS, tempS, codelist;
	codelist.Format(_T("%s"), list);
	m_mapCodeAg.RemoveAll();

	tmpS.Format(_T("%s"), type);
	tmpS.MakeUpper();
	if(tmpS == _T("Y"))
		m_bShowCodeList = true;
	else
		m_bShowCodeList = false;

	if(!codelist.IsEmpty())
	{
		while(1)
		{
			tempS = Parser(codelist, _T("\t"));
			m_mapCodeAg.SetAt(tempS, tempS);
OutputDebugString(_T("!!!!!!!! SetBalListType" + tempS + _T("\n")));
			if(codelist.IsEmpty())
				break;
		}
	}

	m_pCombo->ResetContent();
	m_pCombo->ResetCode();
	m_pCombo->ResetString();
	
	SetTimer(TM_TYPELIST,100,NULL);
	//m_pCombo->CodeLoad(m_strHome);
	//m_pCombo->InitCodeList();
}

void CControlwnd::SetType(LPCTSTR type, LPCTSTR btype)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(m_Unit == 1) 
		return;

	m_arrtype.RemoveAll();
	// TODO: ���⿡ ����ġ ó���⸦ �߰��մϴ�.
	CString tmpS, tempS;

	tmpS.Format(_T("%s"), btype);
	tmpS.TrimLeft();
	tmpS.TrimRight();
	tmpS.MakeUpper();
	if(tmpS == _T("Y"))
		m_bonlyBal = true;
	else
		m_bonlyBal = false;

	
	tmpS.Format(_T("%s"), type);
	tmpS.TrimLeft();
	tmpS.TrimRight();


	if(!tmpS.IsEmpty())
	{
		while(1)
		{
			tempS = Parser(tmpS, _T("\t"));
			m_arrtype.Add(tempS);
			if(tmpS.IsEmpty())
				break;
		}
	}

	m_pCombo->ResetContent();
	m_pCombo->ResetCode();
	m_pCombo->ResetString();
	
	SetTimer(TM_TYPEBAL,100,NULL);
}


void CControlwnd::LoadAuthAg()
{
	CString tmpS, temp;
	m_mapAg.RemoveAll();
	
	CString strUserPath(_T(""));
	CString strFilePath(_T(""));

	strUserPath.Format(_T("%s\\user\\%s"), m_strHome, m_sUser);
	strFilePath.Format(_T("%s\\%s"), strUserPath, _T("authcompany.ini"));
	
	GetPrivateProfileString(_T("auth"), _T("company"), _T(""), tmpS.GetBufferSetLength(1024), 1024, (LPCTSTR)strFilePath);
	tmpS.ReleaseBuffer();

	while(1)
	{
		temp = Parser(tmpS, _T("\t"));
		if(temp.IsEmpty())
			break;
		m_mapAg.SetAt(temp, temp);
	}

	/*
	//���࿩�ο� ���� Y,N
	tmpS.Empty();
	GetPrivateProfileString(_T("auth"), _T("chechYN"), _T(""), tmpS.GetBufferSetLength(1024), 1024, (LPCTSTR)strFilePath);
	tmpS.ReleaseBuffer();
	tmpS.TrimRight();
	if(tmpS.IsEmpty())
		m_bonlyBal = false;
	else
	{
		m_bonlyBal = tmpS == "Y" ? true : false;
	}
	*/

	
}

bool CControlwnd::ChecktypeAG(CString type, CString onlytype, CString tmpcode) //2�ڸ��� �Ѿ�� Ÿ��(m_arrtype)�� ��
{
	CString tmps;
	if(m_arrtype.GetCount() == 0)
	{
		if(m_bonlyBal)					  //y�� �� �����ְ�  N �� 2�ڸ� �ڵ� �´°͸� �����ش�
		{
			if(onlytype == _T("Y"))  
				return true;
		}
		else
			return true;
	}
	else
	{
		for(int ii = 0 ; ii < m_arrtype.GetCount() ; ii++)
		{
		//	tmps.Format(_T("%s %s \n"), m_arrtype.GetAt(ii), type);
		//	TRACE(tmps);
			if(atoi(m_arrtype.GetAt(ii)) == atoi(type))    //m_arrtype <-- settype ���� ���� 2�ڸ� ����� �з��ڵ�   ==  itype (01... ���)
			{
				if(m_bonlyBal)
				{
					if(onlytype == _T("Y"))  
						return true;
				}
				else
					return true;
			}
		}
	}
	return false;
}

bool CControlwnd::CheckcodeAG(CString type, CString onlytype, CString tmpcode) //6�ڸ��� �Ѿ�� Ÿ��(m_mapCodeAg)�� ��
{
	CString tmp;
	if(m_mapCodeAg.GetSize() == 0)
		return true;
	if(m_bShowCodeList)  //m_bShowCodeList true �� m_mapCodeAg�� �ִ°͸� �����ְ� false �� m_mapCodeAg�� ���� �����ش�
	{
		if(m_mapCodeAg.Lookup(tmpcode, tmp))   //SetBalListType �� ���� ȸ���ڵ��̴� 6�ڸ� 
				return true;
	}
	else
	{
		if(!m_mapCodeAg.Lookup(tmpcode, tmp))   //SetBalListType �� ���� ȸ���ڵ��̴� 6�ڸ� 
				return true;
	}
	return false;
}

bool CControlwnd::IsCheckType(CString type)
{
	CString itype, tmp, tmpcode, onlytype;  //itype 2�ڸ� ȸ���ڵ� onlytype ���࿩��   tmpcode 6�ڸ� ȸ���ڵ� 

	type.TrimLeft();
	type.TrimRight();

	itype= Parser(type, _T("\t"));
	onlytype = Parser(type, _T("\t"));
	tmpcode = Parser(type, _T("\t"));

	if(!ChecktypeAG(itype,onlytype,tmpcode))
		return false;

	if(!CheckcodeAG(itype,onlytype,tmpcode))
		return false;

	/*
	if(m_arrtype.GetCount() > 0)    //2�ڸ� ȸ���ڵ� ���� ����Ʈ�� ������
	{
		for(int ii = 0 ; ii < m_arrtype.GetCount() ; ii++)
		{
			if(atoi(m_arrtype.GetAt(ii)) == atoi(itype))    //m_arrtype <-- settype ���� ���� 2�ڸ� ����� �з��ڵ�   ==  itype (01... ���)
			{
				if(m_bShowCodeList)  //m_bShowCodeList true �̸� m_mapCodeAg �� �ִ°�츸 �����ش�
				{
					if(m_mapCodeAg.Lookup(tmpcode, tmp))   //SetBalListType �� ���� ȸ���ڵ��̴� 6�ڸ� 
					{
						if(m_bonlyBal)
						{
							if(onlytype == _T("Y"))  
								return true;
						}
						else
							return true;
					}
					else 
						return false;
				}
				else  //m_bShowCodeList false �̸� m_mapCodeAg �� ���°�츸 �����ش�
				{
					if(!m_mapCodeAg.Lookup(tmpcode, tmp))
					{
						if(m_bonlyBal)
						{
							if(onlytype == _T("Y"))  
								return true;
						}
						else
							return true;
					}
					else 
						return false;
				}
			}
		}	
	}
	else //2�ڸ� ȸ���ڵ� ���� ����Ʈ�� ������
	{
		if(m_bShowCodeList)  //m_bShowCodeList true �̸� m_mapCodeAg �� �ִ°�츸 �����ش�
		{
			if(m_mapCodeAg.Lookup(tmpcode, tmp))   //SetBalListType �� ���� ȸ���ڵ��̴� 6�ڸ� 
			{
				return true;
			}
			else 
				return false;
		}
		else  //m_bShowCodeList false �̸� m_mapCodeAg �� ���°�츸 �����ش�
		{
			if(!m_mapCodeAg.Lookup(tmpcode, tmp) )
			{
				return true;
			}
			else 
				return false;
		}
	}
	*/
	return true;
}

void CControlwnd::SetHsymbol(LPCTSTR symbol)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString tmpS;
	tmpS.Format(_T("%s"), symbol);
	m_sSymbol= tmpS;
	// TODO: ���⿡ ����ġ ó���⸦ �߰��մϴ�.
}


void CControlwnd::SetVisible(LONG benable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(benable == -1)
		ShowWindow(SW_SHOW);
	else
		ShowWindow(SW_HIDE);
	Invalidate();
}


void CControlwnd::SetEnable(LONG benable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(benable == -1)
		EnableWindow(true);
	else
		EnableWindow(false);
}




void CControlwnd::ResetCondition(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_mapAg.RemoveAll();   

	m_arrtype.RemoveAll();    
	m_bonlyBal = false;   

	m_mapCodeAg.RemoveAll();  
	m_bShowCodeList = false;   
}


void CControlwnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch(nIDEvent)
	{
		case TM_TYPELIST:
		case TM_TYPEBAL:
			KillTimer(nIDEvent);
			m_pCombo->CodeLoad(m_strHome);
			m_pCombo->InitCodeList();
		break;
	}
	CWnd::OnTimer(nIDEvent);
}
