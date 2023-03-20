// MyButtonListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "NsyButtonListCtrl.h"
#include "../StdDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNsyButtonListCtrl

CNsyButtonListCtrl::CNsyButtonListCtrl()
{	
	m_szImageDir = _T("");
	m_FontOfFont.CreatePointFont(9, "����");

	m_bDirectRemove = FALSE;
}

CNsyButtonListCtrl::~CNsyButtonListCtrl()
{
}


BEGIN_MESSAGE_MAP(CNsyButtonListCtrl, CXListCtrl)
	//{{AFX_MSG_MAP(CNsyButtonListCtrl)	
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	//ON_WM_MEASUREITEM()
	//ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNsyButtonListCtrl message handlers
void CNsyButtonListCtrl::SetItemButton(int nItem, CString strJongName, int nJongCode)
{
	CRect subRect;	
//	GetSubItemRect(nItem, 1,LVIR_BOUNDS, subRect);
	CString strImageUp, strImageDown, strImageOver;

// 	if(m_bShowBlank)
// 	{
// 		strImageUp		= m_szImageDir + "\\DEL_BTN_BLANK.bmp";
// 		strImageDown	= strImageUp;
// 		strImageOver	= strImageUp;
// 	}
// 	else
	{
		strImageUp		= m_szImageDir + "\\del_btn_up.bmp";
		strImageDown	= m_szImageDir + "\\del_btn_down.bmp";
		strImageOver	= m_szImageDir + "\\btn_select.bmp";
	}

//	CxSkinButton* pButton = NULL;
//	pButton = new CxSkinButton;
//
//	m_ptrArrayBtnWnd.Add(pButton);
//	pButton->Create(_T(""), WS_VISIBLE | WS_CHILD,  subRect, this, nJongCode + nItem);
//	pButton->SetSkin(strImageUp,strImageDown,strImageOver,"","","",0,0,0);
//	pButton->SetWindowText(strJongName);
	

	GetSubItemRect(nItem, 1,LVIR_BOUNDS, subRect);	

	BTNINFO* pBtnInfo = NULL;
	pBtnInfo = new BTNINFO;
	if(pBtnInfo != NULL)
	{
		pBtnInfo->nIndex = nItem;		
		pBtnInfo->xSkinButton.Create(_T(""), WS_VISIBLE | WS_CHILD,  subRect, this, nJongCode + nItem);
		pBtnInfo->xSkinButton.SetSkin(strImageUp,strImageDown,strImageOver,"","","",0,0,0);
		//pBtnInfo->xSkinButton.SetWindowText(strJongName);
		m_ptrArrayBtnWnd.Add(pBtnInfo);		
	}
}








/*
void CNsyButtonListCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{	
	lpMeasureItemStruct->itemHeight = 50; // �䷸�� ������ �����Ϲ� ��
	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}*/

void CNsyButtonListCtrl::Init()
{
	{
		CFont* pFont = GetParent()->GetFont();
		SetFont(pFont);
	}

	GetHeaderCtrl()->EnableWindow(FALSE);
	SetExtendedStyle(LVS_EX_FULLROWSELECT);	
	SetHilightColor(RGB(0,0,0),RGB(208,221,240));

	CRect rect;
	LV_COLUMN lvcolumn;
//	HDITEM hditem;	
	
	GetWindowRect(rect);
	int nColWd =  rect.Width()	- 20;	

//	int colwidthsType[3] = {20, nColWd - 20 , 20};
//	for(int i = 0; i < 3; i++)
//	{
//		memset(&lvcolumn, 0, sizeof(lvcolumn));
//		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
//		lvcolumn.fmt =  LVCFMT_CENTER;
//		
//		if(i == 1 )lvcolumn.pszText = _T("�����");
//		else lvcolumn.pszText = _T("");
//
//		lvcolumn.iSubItem = i;
//		lvcolumn.cx = colwidthsType[i];		
//		InsertColumn(i, &lvcolumn);
//	}	

	int colwidthsType[2] = {nColWd - 20 , 20};
	for(int i = 0; i < 2; i++)
	{
		memset(&lvcolumn, 0, sizeof(lvcolumn));
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt =  LVCFMT_CENTER;
		
		if(i == 0 )lvcolumn.pszText = _T("�����");
		else lvcolumn.pszText = _T("");

		lvcolumn.iSubItem = i;
		lvcolumn.cx = colwidthsType[i];		
		InsertColumn(i, &lvcolumn);
	}	

	CImageList imageList; 	
	long nheight; 	
	nheight = 13; 	

	imageList.Create(1,nheight,ILC_COLOR4,1,1); 
	SetImageList(&imageList,LVSIL_SMALL); 
}

void CNsyButtonListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CXListCtrl::OnSize(nType, cx, cy);	
	if(GetSafeHwnd())
	{
		SetColumnWidth(0, cx - 20);	
		for(int i = 0; i < m_ptrArrayBtnWnd.GetSize(); i++)
		{
			BTNINFO *pBtnInfo = NULL;
			pBtnInfo = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(i);
			if(pBtnInfo != NULL)
			{
				CRect subRect;
				if(i < GetItemCount())
				{
					GetSubItemRect(i, 1,LVIR_BOUNDS, subRect);
					pBtnInfo->xSkinButton.MoveWindow(subRect);
				}				
			}
			/*
			CxSkinButton *pButton = (CxSkinButton*)m_ptrArrayBtnWnd.GetAt(i);
			if(pButton != NULL)
			{
				//BSTR bstrStatus = ::SysAllocString( L"Some text" ); 
				CRect subRect;
				if(i < GetItemCount())
				{
					GetSubItemRect(i, 2,LVIR_BOUNDS, subRect);
					pButton->MoveWindow(subRect);
				}
			}
			*/
		}
	}
}

void CNsyButtonListCtrl::OnDestroy() 
{
	ClearArrayBtnWnd();

	
	CListCtrl::OnDestroy();
}

LRESULT CNsyButtonListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CString sTemp;
	switch(message)
	{	
//		case WM_COMMAND:
//			{
//				if( BN_CLICKED == (int)HIWORD(wParam) )				
//				{
//					for(int i = 0; i < m_ptrArrayBtnWnd.GetSize(); i++)
//					{
//						BTNINFO *pBtnInfo = NULL;
//						pBtnInfo = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(i);
//						if(pBtnInfo != NULL)
//						{
//							if( pBtnInfo->xSkinButton.GetSafeHwnd() == (HWND)lParam)
//							{
//								DeleteChartData(i);
//								
//								pBtnInfo->xSkinButton.DestroyWindow();
//								delete pBtnInfo;
//								m_ptrArrayBtnWnd.RemoveAt(i);
//								DeleteItem(i);
//								Invalidate();
//
//								// �� �Ʒ��� ��ġ�ϴ� �׸���� Index�� Button���� ��ġ�� �� �Űܾ��Ѵ�.
//								for(int j = i; j < m_ptrArrayBtnWnd.GetSize(); j++)
//								{
//									BTNINFO *pBtnInfo2 = NULL;
//									pBtnInfo2 = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(j);
//									if(pBtnInfo2 != NULL)
//									{
//										if( pBtnInfo2->xSkinButton.GetSafeHwnd())
//										{											
//											pBtnInfo2->nIndex -= 1;
//											CRect subRect;
//											GetSubItemRect(pBtnInfo2->nIndex, 1,LVIR_BOUNDS, subRect);
//											pBtnInfo2->xSkinButton.MoveWindow(subRect);
//										}
//									}
//								}
//
//								Invalidate();
//								return S_OK;
//							}
//						}
//					}
//				}
//			}
//			break;
		case WM_COMMAND:
			{
				if( BN_CLICKED == (int)HIWORD(wParam) )				
				{
					int nCount = m_ptrArrayBtnWnd.GetSize();
					if(nCount==1)
					{
						Init_CompareChart();
					}
					else
					{
						for(int i = 0; i < nCount; i++)
						{
							BTNINFO *pBtnInfo = NULL;
							pBtnInfo = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(i);
							if(pBtnInfo != NULL)
							{
								if( pBtnInfo->xSkinButton.GetSafeHwnd() == (HWND)lParam)
								{
									SendToChartRemoveGraph(i);
									return S_OK;
								}
							}
						}
					}
				}
			}
			break;
	}
	return CListCtrl::WindowProc(message, wParam, lParam);
}

void CNsyButtonListCtrl::DeleteOneItem()
{
	POSITION pos = GetFirstSelectedItemPosition();
	//���õ� �׸��� ���� ��� ������ ù��° �׸����� ������ ��Ų��.
	if(pos == NULL)
	{
		MessageBox("������ �׸��� �����Ͻʽÿ�.", "�˸�", MB_OK | MB_ICONERROR);
		return;
	}

	if(pos)
	{			
		//���� ���콺�� ���õǾ��� �׸��� Zero Base ��� Index�� �����´�.
		int i = GetNextSelectedItem(pos);
		DeleteChartData(i);


		BTNINFO *pBtnInfo = NULL;
		pBtnInfo = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(i);
		if(pBtnInfo != NULL)
		{
			if( pBtnInfo->xSkinButton.GetSafeHwnd())
			{
//				CString sTemp;
//				sTemp.Format("pBtnInfo->nIndex : %d", pBtnInfo->nIndex);
//				AfxMessageBox(sTemp);
				
				
				pBtnInfo->xSkinButton.DestroyWindow();
				delete pBtnInfo;
				m_ptrArrayBtnWnd.RemoveAt(i);
				DeleteItem(i);
				Invalidate();

				// �� �Ʒ��� ��ġ�ϴ� �׸���� Index�� Button���� ��ġ�� �� �Űܾ��Ѵ�.
				for(int j = i; j < m_ptrArrayBtnWnd.GetSize(); j++)
				{
					BTNINFO *pBtnInfo2 = NULL;
					pBtnInfo2 = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(j);
					if(pBtnInfo2 != NULL)
					{
						if( pBtnInfo2->xSkinButton.GetSafeHwnd())
						{											
							pBtnInfo2->nIndex -= 1;
							CRect subRect;
							GetSubItemRect(pBtnInfo2->nIndex, 1,LVIR_BOUNDS, subRect);
							pBtnInfo2->xSkinButton.MoveWindow(subRect);
						}
					}
				}

				Invalidate();											
			}
		}
	}
}

//����Ʈ������ ����.
void CNsyButtonListCtrl::RemoveOnlyList(int nItem)
{
	if(m_bDirectRemove==FALSE)
		DeleteSelectItem(nItem);
}

void CNsyButtonListCtrl::DeleteSelectItem(int nItem)
{
//	DeleteChartData(nItem);

	BTNINFO *pBtnInfo = NULL;
	pBtnInfo = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(nItem);
	if(pBtnInfo != NULL)
	{
		if( pBtnInfo->xSkinButton.GetSafeHwnd())
		{
			pBtnInfo->xSkinButton.DestroyWindow();
			delete pBtnInfo;
			m_ptrArrayBtnWnd.RemoveAt(nItem);
			DeleteItem(nItem);
			Invalidate();

			// �� �Ʒ��� ��ġ�ϴ� �׸���� Index�� Button���� ��ġ�� �� �Űܾ��Ѵ�.
			for(int j = nItem; j < m_ptrArrayBtnWnd.GetSize(); j++)
			{
				BTNINFO *pBtnInfo2 = NULL;
				pBtnInfo2 = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(j);
				if(pBtnInfo2 != NULL)
				{
					if( pBtnInfo2->xSkinButton.GetSafeHwnd())
					{											
						pBtnInfo2->nIndex -= 1;
						CRect subRect;
						GetSubItemRect(pBtnInfo2->nIndex, 1,LVIR_BOUNDS, subRect);
						pBtnInfo2->xSkinButton.MoveWindow(subRect);
					}
				}
			}
			Invalidate();											
		}
	}
}

void CNsyButtonListCtrl::Init_CompareChart()
{
	//2005. 07. 14=====================================================
	int nTot = GetItemCount();
	for(int i=nTot-1; i >= 0; i--)
	{
		CString strCode;
		strCode = GetItemText(i, 1);
		// 1. Real ���� �� ��Ʈ ���� ����.
		//DeleteChartData(i);		
		SendToChartRemoveGraph(i);
	}
	//2. List ���� ����
	DeleteAllItems();

	//3. BtnArray ���� ����.
	ClearArrayBtnWnd();
	
	Invalidate();
	//4. ��Ʈ Reset	
	((CStdDialog*)(GetParent()->GetParent()->GetParent()))->SendMessage(UMSG_RESETCHART, 0, 0);
	((CStdDialog*)(GetParent()->GetParent()->GetParent()))->SendMessage(UMSG_INIT_COMPARECHART, 0, 0);

}

void CNsyButtonListCtrl::ClearArrayBtnWnd()
{
	for(int i = 0; i < m_ptrArrayBtnWnd.GetSize(); i++)
	{
		BTNINFO *pBtnInfo = NULL;
		pBtnInfo = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(i);
		if(pBtnInfo)
		{
			pBtnInfo->xSkinButton.DestroyWindow();
			delete pBtnInfo;
			pBtnInfo = NULL;
		}		
	}
	m_ptrArrayBtnWnd.RemoveAll();
}

void CNsyButtonListCtrl::DeleteChartData(int nItem)
{
//080604�� RemoveGraph ȣ���� �ϴ� SendToChartRemoveGraph(..)��ƾ�� ����鼭 ������.
// ���Լ������� �ƹ�ó���� 
return;

	if(nItem<0 || nItem>=GetItemCount()) return;

	//2005. 07. 14=====================================================
	CString strCode, strName;
	strName = GetItemText(nItem, 0);
	strCode = GetItemText(nItem, 1);
	// 1. Real ����
	((CStdDialog*)(GetParent()->GetParent()->GetParent()))->OnCodeUnAdvise(strCode, TRUE);

	// 2. ��Ʈ���� ����.
	// ��������� ��Ʈ�� ���̳� �����͸� �����Ѵ�.
	// �������̳� ����/�ɼ��� ������� �ұ����ϱ� ������
	// ,�� ������� �����Ͽ� �߰��ϹǷ� 
	// CChartItemData�� SetKorName(CString strName)�� �̸� �߰� ����� 
	// ���� ������� �̸��� �����Ѵ�.
	strName.TrimLeft(); strName.TrimRight();
	strName.Replace(" ", "");
	strName.Replace(",", "");
	//((CStdDialog*)(GetParent()->GetParent()->GetParent()))->RemoveGraph_Title(-1, -1, strCode, strName, FALSE);
	((CStdDialog*)(GetParent()->GetParent()->GetParent()))->RemoveGraph(0, 0, strCode, strName);
	//=================================================================

// 	// DEFAULT RQ �� ������ ó��
// 	if(GetItemCount()==0)
// 		Init_CompareChart();
}

void CNsyButtonListCtrl::SendToChartRemoveGraph(int nItem)
{
	if(nItem<0 || nItem>=GetItemCount()) return;
	
	m_bDirectRemove = TRUE;
	CString strCode, strName;
	strName = GetItemText(nItem, 0);
	strCode = GetItemText(nItem, 1);
	
	strName.TrimLeft(); strName.TrimRight();
	strName.Replace(" ", "");
	strName.Replace(",", "");
	((CStdDialog*)(GetParent()->GetParent()->GetParent()))->RemoveGraph(0, 0, strCode, strName);

	// DEFAULT RQ �� ������ ó��
//	if(GetItemCount()==0)	Init_CompareChart();

	// DEFAULT RQ ���ﶧ�� ���� ��ƾ...
//	if(nItem==0)
	{
		DeleteSelectItem(nItem);
		//DeleteItem(nItem);
	}
	m_bDirectRemove = FALSE;
}

void CNsyButtonListCtrl::SetItemButtonVisible(int nItem, BOOL bVisible)
{
	for(int i = 0; i < m_ptrArrayBtnWnd.GetSize(); i++)
	{
		BTNINFO *pBtnInfo = NULL;
		pBtnInfo = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(i);
		if(pBtnInfo != NULL)
		{
			if(bVisible)
				pBtnInfo->xSkinButton.ShowWindow(SW_SHOW);
			else
				pBtnInfo->xSkinButton.ShowWindow(SW_HIDE);
		}
	}
}