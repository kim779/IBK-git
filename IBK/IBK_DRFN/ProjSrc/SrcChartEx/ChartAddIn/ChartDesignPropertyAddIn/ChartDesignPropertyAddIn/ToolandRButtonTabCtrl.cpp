// ToolandRButtonTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "ToolandRButtonTabCtrl.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "ToolandRButtonDlgwithGrid_ListCtrl.h"
#include "Conversion.h"

// ADD: ����(04/03/17)��ġ��ȸ����
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolandRButtonTabCtrl

CToolandRButtonTabCtrl::CToolandRButtonTabCtrl()
{
	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = NULL;

	// MODIFY: ����(04/03/17) ��ġ��ȸ����. ���� RButton Menu ���� ����� �������� �ʰ� �ִ�.
	m_nNumberOfPages=1;

	UINT nID = 0x200;
	for(int i = 0 ; i< m_nNumberOfPages ; i++)
		m_tabPages[i] = new CToolandRButtonDlgwithGrid_ListCtrl(nID * (i+1));
}

CToolandRButtonTabCtrl::~CToolandRButtonTabCtrl()
{
	for(int nCount=0; nCount < m_nNumberOfPages; nCount++){
		delete m_tabPages[nCount];
	}
}


BEGIN_MESSAGE_MAP(CToolandRButtonTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CToolandRButtonTabCtrl)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolandRButtonTabCtrl message handlers
void CToolandRButtonTabCtrl::InitTab()
{
	// MODIFY: ����(04/03/17) ��ġ��ȸ����
	// (2009/9/23 - Seung-Won, Bae) Use '{}' for ML_SET_LANGUAGE_RES()'s re-using.
	{
		ML_SET_LANGUAGE_RES();
		CString strColumnName;
		strColumnName.LoadString( IDS_TOOLBAR);
		InsertItem(0, strColumnName);
		strColumnName.LoadString( IDS_NUMERICAL_INQUIRY);
		InsertItem(1, strColumnName);
	}

	// �������� ���������� ��ġ��ȸ���������� �ɼǰ� ������ ���ؼ�
	ML_SET_LANGUAGE_RES();
	for(int i = 0 ; i< m_nNumberOfPages ; i++) 
	{
		m_tabPages[i]->Create(IDD_DLG_TOOL_RBUTTON, this);

		TCITEM tcItem;
		tcItem.mask = TCIF_TEXT;
		char szBuf[16];
		tcItem.pszText = szBuf;
		tcItem.cchTextMax = sizeof(szBuf);
		GetItem(i, &tcItem);
   
		((CToolandRButtonDlgwithGrid_ListCtrl* )m_tabPages[i])->SetListItemInfoMgrObject(&m_ItemInfoManager, tcItem.pszText, &m_DlgNumericalInquiryOption);
	}

	// (2009/9/23 - Seung-Won, Bae) Use '{}' for ML_SET_LANGUAGE_RES()'s re-using.
	{
		ML_SET_LANGUAGE_RES();
		m_DlgNumericalInquiryOption.Create(IDD_DIALOG_NUMERICALINQUIRYOPTION, this);
	}
	// MODIFY: 04/03/17
}

void CToolandRButtonTabCtrl::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	for(int nCount=0; nCount < m_nNumberOfPages; nCount++){
		m_tabPages[nCount]->MoveWindow(nX, nY, nXc - nX, nYc - nY);
	}

	// ADD: ����(04/03/17) ��ġ��ȸ����
	m_DlgNumericalInquiryOption.MoveWindow(nX, nY, nXc-nX, nYc-nY);
}

void CToolandRButtonTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	// MODIFY: ����(04/03/17) ��ġ��ȸ���� �ɼ� �߰�
	int nIndex = GetCurFocus();
	if(m_tabCurrent != nIndex){
		m_tabCurrent = nIndex;
		if(m_tabCurrent != 1)	// MODIFY: ����(04/03/17) RButton Menu page ����.
		{
			for(int i = 0; i< m_nNumberOfPages; i++)
			{
				if(m_tabCurrent != i)
					m_tabPages[i]->ShowWindow(SW_HIDE);
				else 
				{
					m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
					m_tabPages[m_tabCurrent]->SetFocus();
				}
			}//end of for
			m_DlgNumericalInquiryOption.ShowWindow(SW_HIDE);
		}
		else
		{
			for(int i = 0; i < m_nNumberOfPages; i++)
				m_tabPages[i]->ShowWindow(SW_HIDE);

			m_DlgNumericalInquiryOption.ShowWindow(SW_SHOW);
			m_DlgNumericalInquiryOption.SetFocus();
		}
	}//end of if(m_tab..
	// MODIFY: 04/03/17
}

void CToolandRButtonTabCtrl::InitDropTarget()
{
	for(int nCount = 0; nCount < m_nNumberOfPages; nCount++) {
		((CToolandRButtonDlgwithGrid_ListCtrl* )m_tabPages[nCount])->InitDropTarget();
	}
}

CString CToolandRButtonTabCtrl::GetListItemInfo()
{
	CString strValue = "";
	// (2006/3/3 - Seung-Won, Bae) Get Data with Outer ToolBar
	for(int nCount = 0; nCount < m_nNumberOfPages + 1; nCount++)
	{
		if(nCount == 0)
		{
			strValue += CString( _MTEXT( C4_INNER_TOOL_OPTION)) + "\r\n";
			strValue += ((CToolandRButtonDlgwithGrid_ListCtrl* )m_tabPages[nCount])->GetListItemInfo();
		}
		else if(nCount == 1)
		{
			strValue += CString( _MTEXT( C4_OUTER_TOOL_OPTION)) + "\r\n";
			if( ( ( CButton *)( ( (CToolandRButtonDlgwithGrid_ListCtrl* )m_tabPages[ 0])->GetDlgItem( IDC_CHK_USE_OUTER)))
				->GetCheck()) strValue += CString( _MTEXT( C4_USED)) + "\r\n";
		}
	}
	return strValue;
}

/*-------------------------------------------------------------------------------
 - Function    :  GetNumericalInquiryOption
 - Created at  :  2004-03-17   11:20
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Description :  ��ġ��ȸ���� �ɼǰ��� ��ġ��ȸ������������Dlg���� �����´�.
 -------------------------------------------------------------------------------*/
int CToolandRButtonTabCtrl::GetNumericalInquiryOption()
{
	return m_DlgNumericalInquiryOption.GetNumericalInquiryOption();
}

/*-------------------------------------------------------------------------------
 - Function    :  SetSavedDataFromParent
 - Created at  :  2004-03-09   10:39
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Description :  ��ġ��ȸ���� �ɼǰ�(int Ÿ��)�� �ش� Dlg�� ���� �����Ѵ�.
 -------------------------------------------------------------------------------*/
void CToolandRButtonTabCtrl::SetSavedDataFromParent(const int nOption)
{
	m_DlgNumericalInquiryOption.AddNumericalInquiryOption(nOption);
}

void CToolandRButtonTabCtrl::SetSavedDataFromParent(const CString &strSavedData)
{
	m_strSavedDataFromParent = strSavedData;
}

void CToolandRButtonTabCtrl::SetSavedDataToDlgwithListCtrl()
{
	CString _szData = m_strSavedDataFromParent;

	if(_szData.Right(2) != "End\r\n")
		_szData += "End\r\n";

	CDataConversion::GetStringData(_szData, CString( _MTEXT( C4_INNER_TOOL_OPTION)) + "\r\n");

	// MODIFY: ����(04/03/17) ��ġ��ȸ����
	CString strItemInfo = CDataConversion::GetStringData(_szData, CString( _MTEXT( C4_OUTER_TOOL_OPTION)) + "\r\n");
	((CToolandRButtonDlgwithGrid_ListCtrl* )m_tabPages[0])->SetListItemInfoFromParent(strItemInfo);

	// (2006/3/3 - Seung-Won, Bae) Use CheckButton for Outer Tool
//	((CToolandRButtonDlgwithGrid_ListCtrl* )m_tabPages[1])->SetListItemInfoFromParent(CDataConversion::GetStringData(_szData, "End\r\n"));
	if( !CDataConversion::GetStringData(_szData, "End\r\n").IsEmpty())
		( ( CButton *)( ( (CToolandRButtonDlgwithGrid_ListCtrl* )m_tabPages[ 0])->GetDlgItem( IDC_CHK_USE_OUTER)))->SetCheck( 1);
	// MODIFY: 04/03/17
}


void CToolandRButtonTabCtrl::SetTabFocus()
{
	SetRectangle();
	m_tabCurrent = 0;
	SetCurFocus(0);
	for(int nCount = 0; nCount < m_nNumberOfPages; nCount++){
		if(m_tabCurrent != nCount)
			m_tabPages[nCount]->ShowWindow(SW_HIDE);
		else {
			m_tabPages[nCount]->ShowWindow(SW_SHOW);
			m_tabPages[nCount]->SetFocus();
		}
	}

	// ADD: ����(04/03/17) ��ġ��ȸ���� �ɼ� �߰�
	m_DlgNumericalInquiryOption.ShowWindow(SW_HIDE);

}

/*-------------------------------------------------------------------------------
 - Function    :  GetMiniToolBarItem
 - Created at  :  2004-03-16   18:41
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Description :  �������� ������ ���� ���ڿ��� �����´�.
 -------------------------------------------------------------------------------*/
CString CToolandRButtonTabCtrl::GetMiniToolBarItem()
{
	return ((CToolandRButtonDlgwithGrid_ListCtrl* )m_tabPages[0])->GetListItemInfo();
}

void CToolandRButtonTabCtrl::SetOcxHwnd( HWND p_hOcxWnd)
{
	m_hOcxWnd = p_hOcxWnd;
	for( int i = 0 ; i< m_nNumberOfPages ; i++)
		m_tabPages[i]->SetOcxHwnd( p_hOcxWnd);
	m_ItemInfoManager.SetOcxHwnd( p_hOcxWnd);

	m_ItemInfoManager.Init();
}
