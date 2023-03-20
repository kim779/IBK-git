// GraphStyleCombo.cpp : implementation file
//

#include "stdafx.h"
#include "GraphStyleCombo.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for ML_DEFAULT_RES
#include "ToolBarDlg.h"
#include "Definition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphStyleCombo

CGraphStyleCombo::CGraphStyleCombo()
{
	m_pParent = NULL;
}

CGraphStyleCombo::~CGraphStyleCombo()
{
}


BEGIN_MESSAGE_MAP(CGraphStyleCombo, CComboBox)
	//{{AFX_MSG_MAP(CGraphStyleCombo)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphStyleCombo message handlers


/*-----------------------------------------------------------------------------------------
 - Function    :  AddLineType
 - Created at  :  2004-11-01   15:09
 - Author      :  ������
 - Description :  �޺��ڽ��� reset��Ű�� ���ο� �׷���Ÿ�Կ� ���� String�� �ִ´�.
 -----------------------------------------------------------------------------------------*/
void CGraphStyleCombo::AddLineType(int nType)
{
	ResetContent();
	for(int i = 0; i < nType; i++)
		AddString(chType[i]);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetGraphType
 - Created at  :  2004-11-01   15:12
 - Author      :  ������
 - Description :  �޺��ڽ��� �׷���Ÿ���� �����ϰ� �ش�Ÿ�Կ� �´� GraphStyle���ڿ�
					(������Ͽ� ����)�� �ִ´�.
 -----------------------------------------------------------------------------------------*/
void CGraphStyleCombo::SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex)
{
	GetBitmap().DeleteObject();
	
	ML_SET_DEFAULT_RES();
	if(graphType == CGraphBaseData::Bong_Type)
	{
		if(!GetBitmap().LoadBitmap(IDB_BONG_STYLE))
			return;
		AddLineType(3);
	}
	else if(graphType == CGraphBaseData::Line_Type)
	{
		if(!GetBitmap().LoadBitmap(IDB_LINE_STYLE))	
			return;
		AddLineType(5);
	}
	else if(graphType == CGraphBaseData::Bar_Type)
	{
		if(!GetBitmap().LoadBitmap(IDB_BAR_STYLE))	
			return;
		AddLineType(2);
	}
	else if(graphType == CGraphBaseData::Pie_Type)
	{
		if(!GetBitmap().LoadBitmap(IDB_PIE_STYLE))	
			return;
		AddLineType(2);
	}

	if(nIndex >= GetCount())
		nIndex = 0;
	SetCurSel(nIndex);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnSelchange
 - Created at  :  2004-11-25   15:23
 - Author      :  ������
 - Description :  �޺����� Ư�� ������ ����Ʈ��.
 -----------------------------------------------------------------------------------------*/
void CGraphStyleCombo::OnSelchange() 
{
	int nNum = GetCurSel();
	m_pParent->SetToolFunction(CToolOptionInfo::T_GRAPH_STYLE, NULL, (long)nNum);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnMouseMove
 - Created at  :  2004-11-25   15:24
 - Author      :  ������
 - Description :  ������ �����ֱ� ���� �θ�Ŭ���� ȣ��
 -----------------------------------------------------------------------------------------*/
void CGraphStyleCombo::OnMouseMove(UINT nFlags, CPoint point) 
{
	CImageComboBox::OnMouseMove(nFlags, point);
}
