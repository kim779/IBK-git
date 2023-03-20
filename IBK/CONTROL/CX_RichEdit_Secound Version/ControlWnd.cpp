// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_RichEdit.h"
#include "ControlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd()
{
	EnableAutomation();

	m_pParent = nullptr;
	m_Edit = nullptr;
}

CControlWnd::~CControlWnd()
{
}

void CControlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_FUNCTION(CControlWnd, "SetData", SetData, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {67CD94FE-623E-4EA9-9FE4-4E92523BB623}
static const IID IID_IControlWnd =
{ 0x67cd94fe, 0x623e, 0x4ea9, { 0x9f, 0xe4, 0x4e, 0x92, 0x52, 0x3b, 0xb6, 0x23 } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

void CControlWnd::SetParam(struct _param* pParam)
{
	m_param.key     = pParam->key;
	m_param.name    = CString(pParam->name, pParam->name.GetLength());
	m_param.rect    = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_param.fonts   = CString(pParam->fonts, pParam->fonts.GetLength());
	m_param.point   = pParam->point;
	m_param.style   = pParam->style;
	m_param.tRGB    = pParam->tRGB;
	m_param.pRGB    = pParam->pRGB;
	m_param.options = CString(pParam->options, pParam->options.GetLength());
}

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	if (m_Edit == nullptr)
	{
		CRect	rect;

		GetClientRect(rect);
		m_Edit = new CRichEditCtrl;
		m_Edit->Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE, rect, this, 245);
		m_Edit->ShowScrollBar(TRUE);
		m_Edit->SetFocus();

	}
	return 0;
}

void CControlWnd::SetData(LPCTSTR strData) 
{
	if (m_Edit)
	{
		CString sData;

		sData = CString(strData);
		RsDataAddString(sData, RGB(255,0,0), sData.GetLength());
		m_Edit->SetFocus();
		m_Edit->Invalidate();
	}
}

void CControlWnd::RsDataAddString(CString strWriteData, COLORREF crTextColor, int sSize)
{
	const	int	nLineCnt = m_Edit->GetLineCount();
	const	int	nTextLen = m_Edit->GetWindowTextLength();
	const	int	iStartPos = nTextLen;
	CHARFORMAT cf{};

	m_Edit->SetSel(nTextLen, nTextLen);
	m_Edit->ReplaceSel(strWriteData.GetString());

	cf.cbSize = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_FACE | CFM_SIZE;
	cf.dwEffects = (unsigned long)~(CFE_AUTOBACKCOLOR | CFE_UNDERLINE | CFE_BOLD);
	cf.crTextColor = crTextColor;
	cf.yHeight = sSize * 20;

//	const	int	iEndPos = m_Edit->GetWindowTextLength();
//	if (nLineCnt > 1)
//		iStartPos -= nLineCnt;
//	m_Edit->SetSel(iStartPos, iEndPos);
//	m_Edit->SetSelectionCharFormat(cf);

	m_Edit->HideSelection(TRUE, FALSE);
	m_Edit->SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
		{
			CString sdata;
			struct	_extTHx* exth = (struct _extTHx*)lParam;
			sdata = CString(exth->data, exth->size);
			RsDataAddString(sdata, RGB(255, 0, 0), sdata.GetLength());
		}
		break;
	case DLL_OUB:	// NOT USED. Use DLL_OUBx Only !!!
		break;
	default:break;
	}
	return 0;
}