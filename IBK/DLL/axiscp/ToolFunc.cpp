// ToolFunc.cpp : implementation file
//

#include "stdafx.h"
#include "ToolFunc.h"
#include "../../h/axisvar.h"
#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//**  �̸��տ� '@' ���� ���� ��� �������� ����.
CString name[] = {
		"ȯ�漳��",
		"�μ�",	
		"ȭ�����",
		"���� ��й�ȣ ����",	
		"����� WorkSpace",		
		"���ǰ���",			
		"�ؿ����� �ǽð�ƼĿ",	
		"�̹�������",			
		"�����ü�����",			
};

CString desc[] = {
		"����ȯ�漳�� ȭ���� �����մϴ�.",			
		"����ȭ���� �μ��մϴ�.",					
		"HTS�� �ּ�ȭ ��Ű�� ��й�ȣ�� �Է��ؾ� �ٽ� �������� �մϴ�.",
		"���º�й�ȣ�� �����Ͽ� �ٽ� �Է����� �ʵ��� �����մϴ�.",		
		"����� ����ȭ���� �����մϴ�.",		
		"���ǰ��⸦ �����մϴ�.",				
		"�ؿ����� �ǽð�ƼĿ�� �����մϴ�.",	
		"����ȭ���� �̹����� �����մϴ�.",		
		"�����ü����� DDE ���񽺸� �����մϴ�.",
};

/////////////////////////////////////////////////////////////////////////////
// CToolFunc dialog


CToolFunc::CToolFunc(CWnd* pParent /*=NULL*/)
	: CSetupDialog(CToolFunc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolFunc)
	//}}AFX_DATA_INIT

	m_caption = "���� ���� ����";
}


void CToolFunc::DoDataExchange(CDataExchange* pDX)
{
	CSetupDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolFunc)
	DDX_Control(pDX, IDC_LIST_TOOL, m_toollist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolFunc, CSetupDialog)
	//{{AFX_MSG_MAP(CToolFunc)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
	ON_BN_CLICKED(IDC_MOVEDOWN, OnMovedown)
	ON_BN_CLICKED(IDC_MOVETOP, OnMovetop)
	ON_BN_CLICKED(IDC_MOVEBOTTOM, OnMovebottom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolFunc message handlers

BOOL CToolFunc::OnInitDialog() 
{
	CSetupDialog::OnInitDialog();

	init();
	loadSetup();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CToolFunc::init()
{
	CString file, value;
	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "UserTool.ini");
	char	buf[256];	FillMemory(buf, sizeof(buf), ' ');
	int	ii = 0;
	CArray <int, int>	arOrder, arTemp;
	CArray <int, int>	arFunc;
	arOrder.RemoveAll();	arFunc.RemoveAll();	arTemp.RemoveAll();

	for (ii = 0; ii < MAX_FUNC + 2; ii++)
		arTemp.Add(ii);

	GetPrivateProfileString("ToolFunc", "Order", "0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10", buf, sizeof(buf), file);
	value = buf;
	for (ii = 0; !value.IsEmpty(); ii++)
	{
		CString order = Parser(value, ",");	
		order.TrimLeft();	order.TrimRight();
		if (order.IsEmpty())	continue;
		
		arOrder.Add(atoi(order));
		arTemp.SetAt(atoi(order), -1);
	}
	if (arOrder.GetSize() != MAX_FUNC)
	{
		for (ii = 0; ii < arTemp.GetSize(); ii++)
		{
			const int order = arTemp.GetAt(ii);
			if (order != -1)
				arOrder.Add(order);
		}
	}

	GetPrivateProfileString("ToolFunc", "Func", "0, 1, 2, 3, 5, 6", buf, sizeof(buf), file);
	value = buf;
	for (ii = 0; !value.IsEmpty(); ii++)
	{
		CString func = Parser(value, ",");	
		func.TrimLeft();	func.TrimRight();
		if (func.IsEmpty())	continue;

		arFunc.Add(atoi(func));
	}

	CBitmap bmp;
	m_imageList.Create(20, 20, ILC_COLORDDB | ILC_MASK, 11, 1);
	
	ASSERT(m_imageList.m_hImageList);
	
	bmp.LoadBitmap(IDB_TOOL_IMGLIST);
	m_imageList.Add(&bmp, Axis::maskColor);
	m_toollist.SetImageList(&m_imageList);
	bmp.DeleteObject();
	
	m_toollist.InsertColumn(0, "");
	m_toollist.InsertColumn(1, "�̸�");
	m_toollist.InsertColumn(2, "����");
	m_toollist.SetCheckboxeStyle(RC_CHKBOX_NORMAL);	// Enable checkboxes

	for (ii = 0; ii < MAX_FUNC; ii++)
	{
		const int order = arOrder.GetAt(ii);

		m_toollist.InsertItem(ii, "");
		m_toollist.SetItemText(ii, 1, name[order]);	
		m_toollist.SetItemImage(ii, 1, order);
		m_toollist.SetItemText(ii, 2, desc[order]);
		m_toollist.SetItemData(ii, order);
	}

	for (ii = 0; ii < arFunc.GetSize(); ii++)
	{
		const int func = arFunc.GetAt(ii);

		for (int jj = 0; jj < MAX_FUNC; jj++)
		{
			const int data = m_toollist.GetItemData(jj);
			if (data == func)
				m_toollist.SetItemStates(jj, RC_ITEM_CHECKED);
		}
	}

	m_toollist.SetColumnWidth(0, 24);
	m_toollist.SetColumnWidth(1, 163);
	m_toollist.SetColumnWidth(2, 380);
}

void CToolFunc::loadSetup()
{
	CString file;
	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "UserTool.ini");

	const int type = GetPrivateProfileInt("ToolFunc", "Type", 1, file);
	if (type == 2)	
		((CButton *)GetDlgItem(IDC_CHECK_TYPE))->SetCheck(1);
}

void CToolFunc::ApplySetup()
{
	CString file, value, order, func;
	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "UserTool.ini");

	for (int ii = 0; ii < MAX_FUNC; ii++)
	{
		const int data = m_toollist.GetItemData(ii);
		value.Format("%d", data);
		if (order.IsEmpty())	order += value;
		else			order += ", " + value;

		if (m_toollist.GetItemStates(ii) & RC_ITEM_CHECKED)
		{
			if (func.IsEmpty())	func += value;
			else			func += ", " + value;
		}
	}
	const int type = ((CButton *)GetDlgItem(IDC_CHECK_TYPE))->GetCheck() ? 2 : 1;
	value.Format("%d", type);

	WritePrivateProfileString("ToolFunc", "Order", order, file);
	WritePrivateProfileString("ToolFunc", "Func", func, file);
	WritePrivateProfileString("ToolFunc", "type", value, file);
}

void CToolFunc::DefaultSetup()
{
	CString file, value;
	file.Format("%s\\%s\\%s", Axis::home, MTBLDIR, "USERTOOL.INI");
	char	buf[256];	FillMemory(buf, sizeof(buf), ' ');

	int	ii = 0;
	CArray <int, int>	arOrder, arTemp;
	CArray <int, int>	arFunc;
	arOrder.RemoveAll();	arFunc.RemoveAll();	arTemp.RemoveAll();

	for (ii = 0; ii < MAX_FUNC; ii++)
		arTemp.Add(ii);

	GetPrivateProfileString("ToolFunc", "Order", "0, 1, 2, 3, 4, 5, 6, 7, 8", buf, sizeof(buf), file);
	value = buf;
	for (ii = 0; !value.IsEmpty(); ii++)
	{
		CString order = Parser(value, ",");	
		order.TrimLeft();	
		order.TrimRight();

		if (order.IsEmpty())	
			continue;

		if (atoi(order) >= MAX_FUNC)
			continue;

		arOrder.Add(atoi(order));
		arTemp.SetAt(atoi(order), -1);
	}

	if (arOrder.GetSize() != MAX_FUNC)
	{
		for (ii = 0; ii < arTemp.GetSize(); ii++)
		{
			const int order = arTemp.GetAt(ii);
			if (order != -1)
				arOrder.Add(order);
		}
	}

	GetPrivateProfileString("ToolFunc", "Func", "0, 1, 2, 3, 5, 6", buf, sizeof(buf), file);
	value = buf;
	for (ii = 0; !value.IsEmpty(); ii++)
	{
		CString func = Parser(value, ",");	
		func.TrimLeft();	func.TrimRight();
		if (func.IsEmpty())	continue;

		arFunc.Add(atoi(func));
	}

	m_toollist.DeleteAllItems();
	m_toollist.InsertColumn(0, "");
	m_toollist.InsertColumn(1, "�̸�");
	m_toollist.InsertColumn(2, "����");
	m_toollist.SetCheckboxeStyle(RC_CHKBOX_NORMAL);	// Enable checkboxes

	for (ii = 0; ii < MAX_FUNC; ii++)
	{
		const int order = arOrder.GetAt(ii);

		m_toollist.InsertItem(ii, "");
		m_toollist.SetItemText(ii, 1, name[order]);	
		m_toollist.SetItemImage(ii, 1, order);
		m_toollist.SetItemText(ii, 2, desc[order]);
		m_toollist.SetItemData(ii, order);
	}

	for (ii = 0; ii < arFunc.GetSize(); ii++)
	{
		const int func = arFunc.GetAt(ii);

		for (int jj = 0; jj < MAX_FUNC; jj++)
		{
			const int data = m_toollist.GetItemData(jj);
			if (data == func)
				m_toollist.SetItemStates(jj, RC_ITEM_CHECKED);
		}
	}

	m_toollist.SetColumnWidth(0, 24);
	m_toollist.SetColumnWidth(1, 163);
	m_toollist.SetColumnWidth(2, 380);
}

BOOL CToolFunc::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			pMsg->wParam = VK_TAB;
		break;
	}
	
	return CSetupDialog::PreTranslateMessage(pMsg);
}

void CToolFunc::OnMoveup() 
{
	POSITION pos = m_toollist.GetFirstSelectedItemPosition();
	const int nItem = m_toollist.GetNextSelectedItem(pos);

	m_toollist.MoveUp(nItem);
}

void CToolFunc::OnMovedown() 
{
	POSITION pos = m_toollist.GetFirstSelectedItemPosition();
	const int nItem = m_toollist.GetNextSelectedItem(pos);

	m_toollist.MoveDown(nItem);
}

void CToolFunc::OnMovetop() 
{
	POSITION pos = m_toollist.GetFirstSelectedItemPosition();
	const int nItem = m_toollist.GetNextSelectedItem(pos);

	m_toollist.MoveToTop(nItem);
}

void CToolFunc::OnMovebottom() 
{
	POSITION pos = m_toollist.GetFirstSelectedItemPosition();
	const int nItem = m_toollist.GetNextSelectedItem(pos);

	m_toollist.MoveToBottom(nItem);
}

CString CToolFunc::Parser(CString &srcstr, CString substr)
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
