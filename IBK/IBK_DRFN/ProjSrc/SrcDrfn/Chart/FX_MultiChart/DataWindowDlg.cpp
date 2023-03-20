// DataWindowDlg.cpp : implementation file
//
#include "stdafx.h"
#include "multichart.h"

#include "DataWindowDlg.h"
//#include "../chart_common/DataList/PacketList.h"
//#include "../chart_common/DataList/MainBlock.h"
//#include "../chart_common/DataList/OutsideCommonInterface.h"
//#include "../chart_common/DataList/ViewData.h"
#include "../chart_common/DataList/Conversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataWindowDlg dialog


CDataWindowDlg::CDataWindowDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CDataWindowDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataWindowDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	m_pPacketList = NULL;
//	m_pMainBlock = NULL;
//	m_pChartOcxInterface = NULL;
//	m_nDataIndex = -1;
//	m_nPrevDataCount = 0;
//	m_ptPrevMousePoint = CPoint(-1, -1);
//	m_pChartOcxInterface = NULL;
	m_nDataIndex = -1;
	m_nPrevDataCount = 0;
	m_ptPrevMousePoint = CPoint(-1, -1);
}


void CDataWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataWindowDlg)
	DDX_Control(pDX, IDC_LIST_DATA, m_listctrlData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataWindowDlg, CRscDlg)
	//{{AFX_MSG_MAP(CDataWindowDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataWindowDlg message handlers



/*-----------------------------------------------------------------------------------------
 - Function    :  OnOK, OnCancel
 - Created at  :  2005-07-01   15:36
 - Author      :  ������
 - Description :  EnterŰ�� EscŰ�� ���� Dlg�� �����°��� ���´�.
 -----------------------------------------------------------------------------------------*/
void CDataWindowDlg::OnOK() 
{
//	CDialog::OnOK();
}
void CDataWindowDlg::OnCancel() 
{
//	CDialog::OnCancel();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  CreateDataWindow
 - Created at  :  2005-07-05   13:22
 - Author      :  ������
 - Description :  DataWindow Dlg�� �����ϰ� ���� ������ ����Ÿ�� �������� 5���� 
				  ListCtrl�� �ѷ��ش�.
 -----------------------------------------------------------------------------------------
bool CDataWindowDlg::CreateDataWindow()
{
	// ����.
	Create(IDD_DIALOG_DATAWINDOW, NULL);
	// TEST: ����(05/07/05) ��� �ӽ�.
	CRect rectDlg;
	GetClientRect(&rectDlg);
	SetWindowPos(&CWnd::wndTopMost, 0, 0, rectDlg.Width(), rectDlg.Height(), SWP_SHOWWINDOW);

	m_listctrlData.SetExtendedStyle(m_listctrlData.GetExtendedStyle()|LVS_EX_GRIDLINES);


	// ����Ÿ�� ������ �ε����� ���ؿ´�.
	int nEndIndex = m_pChartOcxInterface->GetViewEndDataIndex(true);
	// Block�� Column �ε����� ù��°���� �Ѵ�.
	int nBlockColumnIndex = 0;

	SetListCtrlHeader(nBlockColumnIndex);

	// ListCtrl Data�ʱ�ȭ.
	LV_ITEM ltc;
	ltc.mask = LVIF_TEXT;
	ltc.iSubItem = 0;
	ltc.pszText = _T("");
	for(int i = 0; i < 5; i++)
	{
		ltc.iItem = i;
		m_listctrlData.InsertItem(&ltc);
	}

	SetData(nBlockColumnIndex, nEndIndex);

	// ��������Ÿ������ ����صд�.
	m_nPrevDataCount = GetDataCount();

	return true;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  InitDataWindow
 - Created at  :  2005-07-13   11:33
 - Author      :  ������
 - Description :  DataWindow �ʱ�ȭ
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::InitDataWindow(CPacketList* pPacketList, CMainBlock* pMainBlock)
{
	m_pPacketList = pPacketList;
	m_pMainBlock = pMainBlock;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetOutsideCommonInterface
 - Created at  :  2005-07-05   13:35
 - Author      :  ������
 - Parameters  :  pChartOcxInterface - ��ƮOCX�� ������ �������̽�
 - Description :  ��ƮOCX�� ������ �������̽��� ����� �����Ѵ�.
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::SetOutsideCommonInterface(COutsideCommonInterface* pChartOcxInterface)
{
	m_pChartOcxInterface = pChartOcxInterface;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetData
 - Created at  :  2005-07-05   11:02
 - Author      :  ������
 - Parameters  :  nBlockColumnIndex - Block�� Column Index.
 -                nIndex         - 5���� ����Ÿ�� ���(3��°)���� ����Ÿ�ε���.
 - Return Value:  None
 - Description :  Container�� ����Ÿ�� �ִ´�.
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::SetData(const int nBlockColumnIndex, const int nIndex)
{
	CViewData viewData;
	if(!m_pMainBlock->GetViewDataListInAllBlock(viewData))
		return;
	if(viewData.GetColumnViewDataCount() <= 0)
		return;

	// ListCtrl�� ǥ���� ����Ÿ�� ���ε����� ���ؿ´�.
	int nEndIndex = GetEndIndex(nIndex);

	int nInsertColumnIndex = 0;	// ListCtrl�� Column Index.

	int nBlockRowCount = viewData.GetRowViewDataCount(nBlockColumnIndex);
	for(int i = 0; i < 5; i++)
	{
		nInsertColumnIndex = 0;	// Column Index �ʱ�ȭ

		// ���ν����ϵ���Ÿ ���Ͽ� ListCtrl�� �����Ѵ�.
		CRowViewData rowViewDataForHorzScale = viewData.GetAt(nBlockColumnIndex, 0);
		SetHorzScaleData(rowViewDataForHorzScale.GetHorzScaleName(), nEndIndex, i, nInsertColumnIndex);

		for(int j = 0; j < nBlockRowCount; j++)
		{
			CRowViewData rowViewData = viewData.GetAt(nBlockColumnIndex, j);
		
			for(int k = 0; k < rowViewData.GetSubGraphViewDataCount(); k++)
			{
				POSITION pos = rowViewData.GetSubGraphViewDataList().GetSubGraphViewDataList().FindIndex(k);
				if(!pos)
					continue;
				CSubGraphViewData subGraphViewData = rowViewData.GetSubGraphViewDataList().GetSubGraphViewDataList().GetNext(pos);

				SetViewData(subGraphViewData.GetTitle(), subGraphViewData.GetPacketNames(), nEndIndex, i, nInsertColumnIndex);
			}//end of for(k
		}//end of for(j		

		nEndIndex--;
	}//end of for(i
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetHorzScaleData
 - Created at  :  2005-07-05   17:05
 - Author      :  ������
 - Parameters  :  strHorzScaleName - ���ν����ϸ�
 -                nDataIndex       - ����Ÿ �ε���
 -				  nInsertRowIndex  - ListCtrl�� �� �ε���
 -				  nInsertColumnIndex - ListCtrl�� �� Į���ε���
 - Description :  �����ϵ���Ÿ�� ����Ʈ��Ʈ�ѿ� Insert�Ѵ�.
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::SetHorzScaleData(const CString& strHorzScaleName, const int nDataIndex, const int nInsertRowIndex, int& nInsertColumnIndex)
{
	CPacket* pPacket = m_pPacketList->GetBaseData(strHorzScaleName);
	if(pPacket == NULL)
		return;

	CString strScaleData = _T("");	// Scale Data(string type)
	double dScaleData = 0.0;		// Scale Data(double type)
	POSITION pos;

	if(pPacket->GetType() == _T("����"))
	{
		pos = pPacket->GetstrDataList()->FindIndex(nDataIndex);
		if(pos != NULL)
			strScaleData = pPacket->GetstrDataList()->GetNext(pos);
	}
	else
	{
		pos = pPacket->GetnumericDataList()->FindIndex(nDataIndex);
		if(pos != NULL)
			dScaleData = pPacket->GetnumericDataList()->GetNext(pos);
		else
			dScaleData = pPacket->GetDataPastandFuture(nDataIndex);

		if(strHorzScaleName.Compare(_T("�ڷ�����")) != 0)
			strScaleData = CDataConversion::DoubleToString(dScaleData);
		else
			strScaleData = CDataConversion::GetStringFormatTrans(CDataConversion::IntToString((int)dScaleData), pPacket->GetType(), _T(""));
	}

	m_listctrlData.SetItemText(nInsertRowIndex, nInsertColumnIndex, strScaleData);
	
	nInsertColumnIndex++;	// Į���� ����.

	return;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetEndIndex
 - Created at  :  2005-07-05   17:34
 - Author      :  ������
 - Parameters  :  nIndex - ���콺�� ��ġ�Ѱ��� ����Ÿ �ε���
 - Return Value:  int - ListCtrl�� ǥ���� ����Ÿ�� ù��°�� �� ����Ÿ�� �ε���
 - Description :  ListCtrl�� ǥ���� ����Ÿ�� ù��° �ε����� ���Ѵ�.
 -----------------------------------------------------------------------------------------
int CDataWindowDlg::GetEndIndex(const int nIndex) const
{
	// �պκ� ��ġ���� ��� ó��
	if(nIndex - 2 < 0)
		return 4;

	// ���κ� ��ġ���� ��� ó��
	if(nIndex + 2 > m_pChartOcxInterface->GetViewEndDataIndex(true))
		return m_pChartOcxInterface->GetViewEndDataIndex(true);

	return nIndex + 2;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetListCtrlHeader
 - Created at  :  2005-07-06   13:13
 - Author      :  ������
 - Description :  ListCtrl�� ����κ��� �����Ѵ�.
 -----------------------------------------------------------------------------------------
bool CDataWindowDlg::SetListCtrlHeader(const int nBlockColumnIndex)
{
	CViewData viewData;
	if(!m_pMainBlock->GetViewDataListInAllBlock(viewData))
		return false;
	if(viewData.GetColumnViewDataCount() <= 0)
		return false;

	int nHeaderColumnCount = m_listctrlData.GetHeaderCtrl()->GetItemCount();	// Į������.
	int nHeaderColumnIndex = 0;	// Į�� �ʱ�ȭ.
	
	int nBlockRowCount = viewData.GetRowViewDataCount(nBlockColumnIndex);
	for(int i = 0; i < nBlockRowCount; i++)
	{
		CRowViewData rowViewData = viewData.GetAt(nBlockColumnIndex, i);
		
		if(0 == i)	// �ѹ��� �����Ѵ�.
		{		// ù��° ���: �ڷ�����
			CString strHorzTitle;
			CPacket* pPacket = NULL;
			pPacket = m_pPacketList->GetBaseData(rowViewData.GetHorzScaleName());
			if(pPacket == NULL) return false;
			if(pPacket->GetType().Find(_T("HH")) >= 0)
				strHorzTitle = _T("�ð�");
			else
				strHorzTitle = _T("����");

			SetHeaderTitle(nHeaderColumnIndex, nHeaderColumnCount, strHorzTitle);
		}

		// �״��� �����.
		for(int j = 0; j < rowViewData.GetSubGraphViewDataCount(); j++)
		{
			POSITION pos = rowViewData.GetSubGraphViewDataList().GetSubGraphViewDataList().FindIndex(j);
			if(!pos)
				continue;
			CSubGraphViewData subGraphViewData = rowViewData.GetSubGraphViewDataList().GetSubGraphViewDataList().GetNext(pos);

			SetNextHeader(subGraphViewData.GetTitle(), subGraphViewData.GetPacketNames(), nHeaderColumnIndex, nHeaderColumnCount);
		}
	}

	// ���ʿ��� Header Column����.
	for( ; nHeaderColumnIndex < nHeaderColumnCount; nHeaderColumnIndex++)
	{
		m_listctrlData.DeleteColumn(nHeaderColumnIndex);
	}
	return true;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetNextHeader
 - Created at  :  2005-07-07   11:15
 - Author      :  ������
 - Parameters  :  strTitle           - Ÿ��Ʋ
 -                strPacketNames     - ��Ŷ��
 -                nHeaderColumnIndex - ����� Į���ε���
 -                nHeaderColumnCount - ���� ����� Į������.
 - Return Value:  bool - true:����, false:����.
 - Description :  �ڷ�����Į���̿��� Į���� �����Ѵ�.
 -----------------------------------------------------------------------------------------
bool CDataWindowDlg::SetNextHeader(const CString& strTitle, const CString& strPacketNames, int& nHeaderColumnIndex, const int nHeaderColumnCount)
{
	CString strRealPacketNames = GetRealPacketNames(strTitle, strPacketNames);
	
	if(1 ==  CDataConversion::GetStringDataCount(strRealPacketNames, _T(",")))
	{
		SetHeaderTitle(nHeaderColumnIndex, nHeaderColumnCount, strTitle);
	}
	else
	{
		while(strRealPacketNames.IsEmpty() == false)
		{
			CString strHeaderTitle = CDataConversion::GetStringData(strRealPacketNames, _T(","));
			SetHeaderTitle(nHeaderColumnIndex, nHeaderColumnCount, strHeaderTitle);
		}
	}
	return true;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetHeaderTitle
 - Created at  :  2005-07-07   11:10
 - Author      :  ������
 - Parameters  :  nColumnIndex   - Į���ε���
 -                nColumnCount   - �������¿����� ���Į������
 -                strHeaderTitle - ������ ���Ÿ��Ʋ
 - Return Value:  bool - true:����, false:����
 - Description :  ���Ÿ��Ʋ�� �����Ѵ�. ���� Į���������� ������ Insert�ϰ� �� �����͵��� 
				  SetItem�� �����Ѵ�. �� ���� ���Į�������� ������ DeleteColumn�� �Ѵ�.
 -----------------------------------------------------------------------------------------
bool CDataWindowDlg::SetHeaderTitle(int& nColumnIndex, const int nColumnCount, const CString& strHeaderTitle)
{
	if(nColumnCount < nColumnIndex)
	{	// ���� �߰�.
		m_listctrlData.InsertColumn(nColumnIndex, strHeaderTitle, LVCFMT_RIGHT, 80);
	}
	else
	{	// ������ ����
		HDITEM hdi;
		TCHAR lpBuffer[256];
		hdi.mask = HDI_TEXT;
		hdi.pszText = lpBuffer;
		hdi.cchTextMax = 256;

		m_listctrlData.GetHeaderCtrl()->GetItem(nColumnIndex, &hdi);
		strcpy(hdi.pszText, (LPCTSTR)strHeaderTitle);
		m_listctrlData.GetHeaderCtrl()->SetItem(nColumnIndex, &hdi);
	}
	nColumnIndex++;
	return true;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetRealPacketNames
 - Created at  :  2005-07-06   15:39
 - Author      :  ������
 - Description :  ���� ��Ʈ���°� 1ƽ�ϰ��� ���ݵ���Ÿ�� "����"�� ǥ���Ѵ�.
 -----------------------------------------------------------------------------------------
CString CDataWindowDlg::GetRealPacketNames(const CString& strTitle, const CString& strPacketNames) const
{
	CString strRealPacketNames = strPacketNames;
	if(strRealPacketNames.Right(1) != ",")
		strRealPacketNames += ",";

	if(strTitle == _T("����") && strPacketNames.Find(_T("����")) > 0)
	{
		// ���� 5�� "ƽ"�� ��Ʈ������ Enum���̴�.
		if(m_pPacketList->GetDateUnitType() == 5 && m_pPacketList->GetTimeInterval() == 1)
			strRealPacketNames = _T("����,");
	}
	return strRealPacketNames;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetViewData
 - Created at  :  2005-07-06   19:34
 - Author      :  ������
 - Parameters  :  strTitle		 - Ÿ��Ʋ��
 -				  strPacketNames - ��Ŷ��.
 -                nDataIndex     - ����Ÿ �ε���
 -				  nInsertRowIndex   - ListCtrl�� Row Index.(��, ����Ÿ�� Insert�� ��.)
 -				  nInsertColumnIndex- ListCtrl�� Column Index.(��, ����Ÿ�� Insert�� ��.)
 - Description :  nDataIndex�� �ش�Ǵ� ����Ÿ�� ListCtrl�� Insert��Ų��.
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::SetViewData(const CString& strTitle, const CString& strPacketNames, const int nDataIndex, const int nInsertRowIndex, int& nInsertColumnIndex)
{
	CString strRealPacketNames = GetRealPacketNames(strTitle, strPacketNames);

	int nPacketCount = CDataConversion::GetStringDataCount(strRealPacketNames, _T(","));

	CString strData;
	if(nPacketCount == 1)
	{
		CString strPacketName = CDataConversion::GetStringData(strRealPacketNames, _T(","));
		CPacket* pPacket = NULL;
		pPacket = m_pPacketList->GetBaseData(strPacketName);		
		//2005. 07. 18 ====================================
		if(pPacket == NULL) return;
		//=================================================


		if(pPacket->GetType() == _T("����"))
		{
			POSITION pos = pPacket->GetstrDataList()->FindIndex(nDataIndex);
			if(!pos)
				return;
			strData = pPacket->GetstrDataList()->GetNext(pos);
		}
		else
		{
			POSITION pos = pPacket->GetnumericDataList()->FindIndex(nDataIndex);
			if(!pos)
				return;
			double dData = pPacket->GetnumericDataList()->GetNext(pos);
			int nLength = pPacket->GetType().GetLength();
			int nDecimalPt = pPacket->GetType().Find(".") + 1;
			int nDecimal;

			if(nDecimalPt == 0)
				nDecimal = 0;
			else
				nDecimal = nLength - nDecimalPt;

			strData = CDataConversion::DoubleToString(dData, nDecimal);
			strData = CDataConversion::GetStringFormatTrans(strData, pPacket->GetType());
		}
		
		m_listctrlData.SetItemText(nInsertRowIndex, nInsertColumnIndex, strData);
		nInsertColumnIndex++;	// Į���� ����.
	}//end of if(nPacketCount...
	else
	{
		while(strRealPacketNames.IsEmpty() == false)
		{
			CString strPacketName = CDataConversion::GetStringData(strRealPacketNames, _T(","));
			CPacket* pPacket = NULL;
			pPacket = m_pPacketList->GetBaseData(strPacketName);
			if(pPacket == NULL)
				return;
			if(pPacket->GetType() == _T("����"))
			{
				POSITION pos = pPacket->GetstrDataList()->FindIndex(nDataIndex);
				if(!pos)
					return;
				strData = pPacket->GetstrDataList()->GetNext(pos);
			}
			else
			{
				POSITION pos = pPacket->GetnumericDataList()->FindIndex(nDataIndex);
				if(!pos)
					return;
				double dData = pPacket->GetnumericDataList()->GetNext(pos);
				int nLength = pPacket->GetType().GetLength();
				int nDecimalPt = pPacket->GetType().Find(_T(".")) + 1;
				int nDecimal;

				if(nDecimalPt == 0)
					nDecimal = 0;
				else
					nDecimal = nLength - nDecimalPt;

				strData = CDataConversion::DoubleToString(dData, nDecimal);
				strData = CDataConversion::GetStringFormatTrans(strData, pPacket->GetType());
			}

			m_listctrlData.SetItemText(nInsertRowIndex, nInsertColumnIndex, strData);
			nInsertColumnIndex++;	// Į���� ����.
		}//end of while(
	}//end of else
}


/*-----------------------------------------------------------------------------------------
 - Function    :  MoveMouse
 - Created at  :  2005-07-07   08:06
 - Author      :  ������
 - Description :  ���콺 �����϶� point���� �ش��ϴ� Data Index�� ���Ͽ� listctrl�� �ش�
				  ����Ÿ�� �ѷ��ش�.
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::MoveMouse(CPoint point)
{
	// �ش� ���콺 ����Ʈ�� ��ġ�ϴ� ����Ÿ �ε����� ���ؿ´�.
	if(!m_pChartOcxInterface)
		return;
	int nDataIndex = m_pChartOcxInterface->GetViewDataIndex(point);

//	m_listctrlData.DeleteAllItems();
	CBlockIndex blockIndex = m_pMainBlock->FindBlockNumber(point);
	if(blockIndex.GetColumn() == -1)
		return;
	
	SetListCtrlHeader(blockIndex.GetColumn());
	SetData(blockIndex.GetColumn(), nDataIndex);

	// ���콺�� ��ġ�� ����صд�.
	m_ptPrevMousePoint = point;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ChangeTRData
 - Created at  :  2005-07-12   10:31
 - Author      :  ������
 - Description :  TR����Ÿ�� ��ȸ�Ҷ�... ù��° Block Column Index�� ������ ����Ÿ �ε������� �ѷ��ش�.
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::ChangeTRData(void)
{
	// ����Ÿ�� ������ �ε����� ���ؿ´�.
	int nEndIndex = m_pChartOcxInterface->GetViewEndDataIndex(true);
	// Block�� Column �ε����� ù��°���� �Ѵ�.
	int nBlockColumnIndex = 0;

	SetListCtrlHeader(nBlockColumnIndex);
	SetData(nBlockColumnIndex, nEndIndex);
	
	// ���� ����Ÿ�� ����صд�.
	m_nPrevDataCount = GetDataCount();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ChangeRealData
 - Created at  :  2005-07-12   10:31
 - Author      :  ������
 - Description :  ������Ÿ�� �����Ҷ�... ��������Ÿ������ ���Ͽ� ���� ����Ÿ ������
				  ������ ���ο��Ÿ�� ���°��̰�, ��������Ÿ������ ���ٸ� Update��
				  �� �����̴�. ���� ��������Ÿ������ ���絥��Ÿ�������� ũ��
				  ������ RealPacket����Ÿ �Ϻθ� ���� ���¸� �ǹ��Ѵ�.
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::ChangeRealData(void)
{
	this->MoveMouse(m_ptPrevMousePoint);
	
	// ����Ÿ������ ��������Ÿ������ �����صд�.
	m_nPrevDataCount = GetDataCount();
}



/*-----------------------------------------------------------------------------------------
 - Function    :  GetDataCount
 - Created at  :  2005-07-12   13:55
 - Author      :  ������
 - Description :  ����Ÿ ������ ���Ѵ�.
 -----------------------------------------------------------------------------------------
int CDataWindowDlg::GetDataCount(void) const
{
	CViewData viewData;
	if(!m_pMainBlock->GetViewDataListInAllBlock(viewData))
		return -1;
	if(viewData.GetColumnViewDataCount() <= 0)
		return -1;

	// ����Ÿ�� ������ ���صд�.
	CRowViewData tmpRowViewData = viewData.GetAt(0, 0);
	return m_pPacketList->GetCount(tmpRowViewData.GetHorzScaleName());
}
*/
void CDataWindowDlg::OnSize(UINT nType, int cx, int cy) 
{
	CRscDlg::OnSize(nType, cx, cy);
	if(m_listctrlData.GetSafeHwnd())
	{
		CRect listRect;
		m_listctrlData.GetWindowRect(&listRect);

		listRect.right  = listRect.left + cx;
		listRect.bottom = listRect.top  + cy;
		ScreenToClient(listRect);
		m_listctrlData.MoveWindow(listRect, TRUE);
	}
}

BOOL CDataWindowDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(pMsg->message) 
	{
		case WM_KEYDOWN	:
			{
				UINT nChar = (UINT)pMsg->wParam;
				switch(nChar)
				{
					case VK_RETURN :
					case VK_ESCAPE :
					{
						return FALSE;
						break;					
					}
				}
				break;
			}
	}
//	return CRscDlg::PreTranslateMessage(pMsg);
	
	if(::IsWindow(m_hWnd))
		return GetParent()->PreTranslateMessage(pMsg);
	else
		return CRscDlg::PreTranslateMessage(pMsg);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  CreateDataWindow
 - Created at  :  2005-07-05   13:22
 - Author      :  ������
 - Description :  DataWindow Dlg�� �����ϰ� ���� ������ ����Ÿ�� �������� 5���� 
				  ListCtrl�� �ѷ��ش�.
 -----------------------------------------------------------------------------------------*/
bool CDataWindowDlg::CreateDataWindow()
{
	// ����.
	Create(IDD_DIALOG_DATAWINDOW, NULL);
	// TEST: ����(05/07/05) ��� �ӽ�.
	CRect rectDlg;
	GetClientRect(&rectDlg);
	SetWindowPos(&CWnd::wndTopMost, 0, 0, rectDlg.Width(), rectDlg.Height(), SWP_SHOWWINDOW);

	m_listctrlData.SetExtendedStyle(m_listctrlData.GetExtendedStyle()| LVS_EX_FULLROWSELECT| LVS_EX_GRIDLINES);


	// ListCtrl Data�ʱ�ȭ.
	LV_ITEM ltc;
	ltc.mask = LVIF_TEXT;
	ltc.iSubItem = 0;
	ltc.pszText = _T("");
	for(int i = 0; i < 5; i++)
	{
		ltc.iItem = i;
		m_listctrlData.InsertItem(&ltc);
	}

//
//	SetListCtrlHeader(nBlockColumnIndex);
//	SetData(nBlockColumnIndex, nEndIndex);
//
//	// ��������Ÿ������ ����صд�.
//	m_nPrevDataCount = GetDataCount();

	return true;
}



/*-----------------------------------------------------------------------------------------
 - Function    :  SetOutsideCommonInterface
 - Created at  :  2005-07-05   13:35
 - Author      :  ������
 - Parameters  :  pChartOcxInterface - ��ƮOCX�� ������ �������̽�
 - Description :  ��ƮOCX�� ������ �������̽��� ����� �����Ѵ�.
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::SetOutsideCommonInterface(COutsideCommonInterface* pChartOcxInterface)
{
	m_pChartOcxInterface = pChartOcxInterface;
}
 -----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
 - Function    :  SetDataWindowData
 - Created at  :  2005-07-22   11:10
 - Author      :  ������
 - Description :  ��ƮOCX���� List�� ��� �ִ� ����Ÿ�� �����ͼ� ListCtrl�� �Ѹ���.
 -----------------------------------------------------------------------------------------*/
void CDataWindowDlg::SetDataWindowData(CList<CString, CString&>* pListData)
{
	POSITION pos = pListData->GetHeadPosition();
	
	if(!pos)
		return;

	// Header ����.
	CString strHeader;
	strHeader.Format("%s", pListData->GetNext(pos));
	//CString strHeader = pListData->GetNext(pos);
	SetHeaderCtrl(strHeader);
	
	// ����Ÿ ����.
	CString strListCtrlData;
	int nTot = pListData->GetCount();	nTot -= 2;
	for(int nRowIndex = 0; nRowIndex < nTot ; nRowIndex++)
	{
		strListCtrlData.Format("%s", pListData->GetNext(pos));		
		SetListCtrlData(strListCtrlData, nRowIndex);		
	}

	/*
	int nRowIndex = 0;
	while(pos)
	{
		CString strListCtrlData = pListData->GetNext(pos);		
		SetListCtrlData(strListCtrlData, nRowIndex);
		nRowIndex++;
	}*/

	CString strSelIndex;
	strSelIndex.Format("%s", pListData->GetTail());
	int nSelIndex = atoi(strSelIndex) - 1;
	m_listctrlData.SetItemState(nSelIndex, LVNI_SELECTED | LVNI_FOCUSED, LVNI_SELECTED | LVNI_FOCUSED);	
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetListCtrlData
 - Created at  :  2005-07-22   13:37
 - Author      :  ������
 - Description :  ��ȸ����Ÿ�� List��Ʈ�ѿ� Insert�Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CDataWindowDlg::SetListCtrlData(CString& strListCtrlData, const int nRowIndex)
{
	int nColumnIndex = 0;
	while(strListCtrlData.IsEmpty() == false)
	{
		CString strData = CDataConversion::GetStringData(strListCtrlData, _T("|"));
		m_listctrlData.SetItemText(nRowIndex, nColumnIndex, strData);
		nColumnIndex++;
	}
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetHeaderCtrl
 - Created at  :  2005-07-22   13:30
 - Author      :  ������
 - Description :  �����Ʈ���� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CDataWindowDlg::SetHeaderCtrl(CString& strListHeader)
{
	int nHeaderColumnCount = m_listctrlData.GetHeaderCtrl()->GetItemCount();

	int nHeaderColumnIndex = 0;
	while(strListHeader.IsEmpty() == false)
	{
		CString strHeaderTitle = CDataConversion::GetStringData(strListHeader, _T("|"));
		
		if(nHeaderColumnCount <= nHeaderColumnIndex)	// ���� �߰�.
		{
			m_listctrlData.InsertColumn(nHeaderColumnIndex, strHeaderTitle, LVCFMT_RIGHT, 80);
		}
		else	// ������ ����.
		{
			HDITEM hdi;
			TCHAR lpBuffer[256];
			hdi.mask = HDI_TEXT;
			hdi.pszText = lpBuffer;
			hdi.cchTextMax = 256;

			m_listctrlData.GetHeaderCtrl()->GetItem(nHeaderColumnIndex, &hdi);
			strcpy(hdi.pszText, (LPCTSTR)strHeaderTitle);
			m_listctrlData.GetHeaderCtrl()->SetItem(nHeaderColumnIndex, &hdi);
		}

		nHeaderColumnIndex++;
	}

	// ���ʿ��� Header Column����.
	for( ; nHeaderColumnIndex < nHeaderColumnCount; nHeaderColumnIndex++)
		m_listctrlData.DeleteColumn(nHeaderColumnIndex);
}
