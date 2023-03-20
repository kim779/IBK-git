// UsrAddListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "comparechart.h"
#include "UsrAddListDlg.h"
#include "StdDialog.h"
#include "DlgItemMaxCount.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CUsrAddListDlg dialog


CUsrAddListDlg::CUsrAddListDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CUsrAddListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUsrAddListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_szImageDir= _T("");
	m_strMainPath = _T("");

	//m_pIL_W20H13 = NULL;
	m_pIL_W60H20 = NULL;
}


void CUsrAddListDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUsrAddListDlg)
	DDX_Control(pDX, IDC_UALD_BTN_INIT, m_btnInitAll);
	DDX_Control(pDX, IDC_UALD_BTN_DEL, m_btnDelItem);
	DDX_Control(pDX, IDC_UALD_BTN_ADD, m_btnAddItem);
	DDX_Control(pDX, IDC_UALD_LISTCODE, m_listUsrAdd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUsrAddListDlg, CRscDlg)
	//{{AFX_MSG_MAP(CUsrAddListDlg)
	ON_BN_CLICKED(IDC_UALD_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_UALD_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_UALD_BTN_INIT, OnBtnInit)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_CHECKBOX_CLICKED, OnRecvMsg_CheckBoxStates_From_XListCtrl) //2005. 07. 28 added by sy,nam			
	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_DBCLICK, OnRecvMsg_DBClick_From_XListCtrl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUsrAddListDlg message handlers

void CUsrAddListDlg::OnBtnAdd() 
{	
	GetParent()->SendMessage(UMSG_ADDITEM_BTNCLICK, 0, 0);
}

void CUsrAddListDlg::OnBtnDel() 
{
	// TODO: Add your control notification handler code here
	m_listUsrAdd.DeleteOneItem();
}

void CUsrAddListDlg::OnBtnInit() 
{
	m_listUsrAdd.Init_CompareChart();
}

void CUsrAddListDlg::OnBtnDelItem(LPCSTR szDelCode)
{
	DelItem(szDelCode);
// 	int nTot = m_listUsrAdd.GetItemCount();
// 
// 	CString strCode;
// 	for(int i = 0; i < nTot ; i++)	
// 	{
// 		strCode = m_listUsrAdd.GetItemText(i, 1); strCode.TrimLeft(); strCode.TrimRight();
// 		if(strCode.CompareNoCase(szDelCode)==0)
// 		{
// 			//m_listUsrAdd.DeleteSelectItem(i);
// 			m_listUsrAdd.RemoveOnlyList(i);
// 
// 			// DEFAULT RQ �� ������ ó��
// 			if(m_listUsrAdd.GetItemCount()==0)
// 				OnBtnInit();
// 			return;
// 		}
// 	}
}

void CUsrAddListDlg::DelItem(LPCSTR szDelCode)
{
	int nTot = m_listUsrAdd.GetItemCount();
	
	CString strCode;
	for(int i = 0; i < nTot ; i++)	
	{
		strCode = m_listUsrAdd.GetItemText(i, 1); strCode.TrimLeft(); strCode.TrimRight();
		if(strCode.CompareNoCase(szDelCode)==0)
		{
			//m_listUsrAdd.DeleteSelectItem(i);
			m_listUsrAdd.RemoveOnlyList(i);
			
			// DEFAULT RQ �� ������ ó��
			if(m_listUsrAdd.GetItemCount()==0)
				OnBtnInit();
			return;
		}
	}
}
BOOL CUsrAddListDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();	

	m_pIL_W60H20 = &(((CStdDialog*)GetParent()->GetParent())->m_ImgList_W60H20);
	if(m_pIL_W60H20 == NULL) return TRUE;

	m_btnAddItem	.FP_vSetImage(m_pIL_W60H20,  12, 14);
	m_btnDelItem	.FP_vSetImage(m_pIL_W60H20, 24, 26);
	m_btnInitAll	.FP_vSetImage(m_pIL_W60H20, 16, 18);

/*	CString		strImageUp		;
	CString		strImageDown	;

	strImageUp		= m_szImageDir + "\\�߰�_up.bmp";
	strImageDown	= m_szImageDir + "\\�߰�_down.bmp";
	m_btnAddItem	.SetSkin(strImageUp,strImageDown,"","","","",0,0,0);
	m_btnAddItem	.SetToolTipText("�ʱ�ȭ");

	strImageUp		= m_szImageDir + "\\��Ʈ_����_up.bmp";
	strImageDown	= m_szImageDir + "\\��Ʈ_����_down.bmp";
	m_btnDelItem	.SetSkin(strImageUp,strImageDown,"","","","",0,0,0);
	m_btnDelItem	.SetToolTipText("�ʱ�ȭ");

	strImageUp		= m_szImageDir + "\\�ʱ�ȭ_up.bmp";
	strImageDown	= m_szImageDir + "\\�ʱ�ȭ_down.bmp";
	m_btnInitAll	.SetSkin(strImageUp,strImageDown,"","","","",0,0,0);
	m_btnInitAll	.SetToolTipText("�ʱ�ȭ");
*/
	
	m_listUsrAdd.Init();
	m_listUsrAdd.SetRowHeight(13);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CUsrAddListDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CString sTemp;
	int nPos = 0;
	switch(message)
	{
		case UMSG_ADDITEM:			
			sTemp = *((CString*)lParam);	//�Ѿ�� lparam���� ���� CString ������ �ִ´�.
			nPos = sTemp.Find('|', 0);
			m_strOpCodeName = sTemp.Left(nPos); m_strOpCodeName.TrimLeft(); m_strOpCodeName.TrimRight();
			m_strOpCodeName.Replace(_T("&"), _T("&&"));
			m_strOpCode = sTemp.Mid(nPos+1);	m_strOpCode.TrimLeft(); m_strOpCode.TrimRight();
			{
				if(m_strOpCode.Compare("5001")==0)
					m_strOpCodeName = "�ڽ�������";
				else if(m_strOpCode.Compare("2001")==0)
					m_strOpCodeName = "�ڽ�������";
			}
			if(!m_strOpCode.GetLength())
			{
				m_strOpCodeName = _T("");
				m_strOpCode		= _T("");
				return 0;
			}

			if (m_strOpCode.GetLength() == 4)
			{
				IMasterDataManagerLast* pDataManager = NULL;
				pDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
				if (pDataManager != NULL)
				{
					CString strRet;
					pDataManager->GetSiteUpKode(m_strOpCode, strRet);
					m_strOpCode = strRet;
				}
			}

			AddItem();
			break;
		case UMSG_RE_REQUEST_TRDATA:
			Re_Request_TRData();
			break;
		case UMSG_INITIAL_BTNCLICK:
			OnBtnInit();
			break;
		case UMSG_DELITEM_BTNCLICK:
			OnBtnDelItem((LPCSTR)lParam);
			break;
	}
	if(message==RMSG_GETINFO)
	{
		if(wParam==10)
		{
			int nTot = m_listUsrAdd.GetItemCount();
			if(nTot<2) return (LRESULT)"";

			CString strCode;
			strCode = m_listUsrAdd.GetItemText(1, 1);
			strCode.TrimLeft(); strCode.TrimRight();
			return (LRESULT)(LPCSTR)strCode;
		}
	}
	return CRscDlg::WindowProc(message, wParam, lParam);
}

void CUsrAddListDlg::AddItem()
{
	m_listUsrAdd.m_szImageDir = m_szImageDir;
	int nTot = m_listUsrAdd.GetItemCount();
	
	if(nTot >= MAX_ITEM_CNT)
	{
		//-->@080522-002�ִ밹���ʰ� �޽���
//		CString szMsg;
//		szMsg.Format("�ִ� %d�� ������� �����մϴ�.", MAX_ITEM_CNT);
//		AfxMessageBox(szMsg);

		//<--

		int nReturn = -1;
		//0004000: [7123] ���պ���Ʈ - 10�� ���� ��ϵ� ���¿��� ���� �߰��� ������ �˾�â�� ����߰�
		//HTS ���� �� ó�� ������ ���� �޽���â�� �߰� ������, ���Ŀ��� �޽���â ��ü�� �����ʰ� ����
		HINSTANCE hInstResourceClient = AfxGetResourceHandle();
		AfxSetResourceHandle(g_hInstance);
		CDlgItemMaxCount dlgMaxItem(MAX_ITEM_CNT, nReturn, this);
		dlgMaxItem.DoModal();
		AfxSetResourceHandle(hInstResourceClient);

		if(nReturn == IDOK) // ���� ���� �� �߰�
		{
			m_listUsrAdd.Init_CompareChart();
			AddItem();
			return;

		}
		else if(nReturn == ID_BTN_MODIFY)	// ������ ������ ����
		{
			m_listUsrAdd.SendToChartRemoveGraph(MAX_ITEM_CNT - 1);
			AddItem();
			return;
		}
		else	// ���
			return;
	}
	else
	{
		m_strOpCode.TrimLeft(); m_strOpCode.TrimRight();
		BOOL bCanAddNewCode = TRUE; //���ο� ������ �߰��� �� �ִ�.
		for(int i = 0; i < nTot ; i++)	
		{
			CString strCode;
			strCode = m_listUsrAdd.GetItemText(i, 1); strCode.TrimLeft(); strCode.TrimRight();
			
			if(m_strOpCode.CompareNoCase(strCode) == 0) bCanAddNewCode = FALSE;
			m_listUsrAdd.SetCurSel(i, FALSE);
		}

		if(bCanAddNewCode == FALSE) 
		{
			MessageBox("�̹� �߰��Ǿ� �ִ� �����Դϴ�", "�ȳ�", MB_OK | MB_ICONEXCLAMATION);
			return;
		}
		
//		if(m_strOpCode)
		m_listUsrAdd.InsertItem(nTot, m_strOpCodeName);	
		m_listUsrAdd.SetCurSel(nTot);

		//m_listUsrAdd.SetItemText(nTot, 0,	m_strOpCodeName);
		//@Solomon-100127 ó������ ���� �ȵǰ� ó��.
//		if(nTot==0) m_listUsrAdd.m_bShowBlank=TRUE;
//		else m_listUsrAdd.m_bShowBlank=FALSE;

		m_listUsrAdd.SetItemText(nTot, 1,	m_strOpCode);
		m_listUsrAdd.SetItemButton(nTot,	m_strOpCode, atoi(m_strOpCode));	

		//@Solomon-100127 ó������ ���� �ȵǰ� ó��.
		//if(nTot==0) m_listUsrAdd.SetItemButtonVisible(nTot, FALSE);

		m_listUsrAdd.Invalidate();

		//=================================== TR ��ȸ�� ������ ==============================					
		// ToolBar�� ������ ���� �����ϱ� ���� Toolbar�� ���� TR��û�� �Ѵ�.	
		//GetParent()->GetParent()->SendMessage(UMSG_GET_COMPARECHART_DATA, (WPARAM)&m_strOpCode, (LPARAM)1);		
		//===================================================================================
//-->
//		STINPUTCODEDATA2	stInputCodeData2;
//		stInputCodeData2.m_strCode = m_strOpCode;
//		stInputCodeData2.m_bDontPutInShared = FALSE;
//		//if(m_bAddModeFromTab)
//			stInputCodeData2.m_bRequestWithoutHesitation = TRUE;
//		//else
//		//	stInputCodeData2.m_bRequestWithoutHesitation = FALSE;
//		GetParent()->GetParent()->SendMessage(UMSG_INPUTCODETOINPUTBAR,(WPARAM)&stInputCodeData2);
//<--
		((CStdDialog*)(GetParent()->GetParent()))->RequestTRFormCode(m_strOpCode);
	}
}

void CUsrAddListDlg::OnSize(UINT nType, int cx, int cy) 
{
	CRscDlg::OnSize(nType, cx, cy);	
	if(m_listUsrAdd.GetSafeHwnd())
	{
		CRect rect,listRect;
		GetClientRect(rect)	;
		m_listUsrAdd.GetWindowRect(listRect);
		ScreenToClient(listRect);
		listRect.left = rect.left + 1;
//>> [QC No]090806HMC090
		listRect.right = rect.right;
//<< [QC No]090806HMC090
		listRect.bottom = rect.bottom - 3; 
		m_listUsrAdd.MoveWindow(listRect);	
	}
}

void CUsrAddListDlg::OnDestroy() 
{
	CRscDlg::OnDestroy();
	
	m_listUsrAdd.DeleteAllItems();
	m_brBkColor.DeleteObject();
}

//HBRUSH CUsrAddListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
//{
//	HBRUSH hbr = CRscDlg::OnCtlColor(pDC, pWnd, nCtlColor);	
//	switch(nCtlColor) 
//	{	
//		case CTLCOLOR_DLG		:				
//		case CTLCOLOR_BTN       : 				 
//			return m_brBkColor;
//			break;		
//		default:			
//			{
//				pDC->SetBkColor(m_crBk);
//				return m_brBkColor;
//			}		
//			break;
//	}
//	return hbr;
//}


//���ٿ��� ��ȸ �����̳� �� / �� / �� / �� ���� ������ �ٲ� ���
//�߰��� ����鿡 ���� �����͸� �ٽ� ��ȸ �Ѵ�.
void CUsrAddListDlg::Re_Request_TRData()
{
	int nTot = m_listUsrAdd.GetItemCount();
	int i = 0;
	//CStdDialog�� ��ȸ�Ǵ� ������ ����� �˷��� �ش� ������ 
	//������ ��ȸ�� �� �Ǹ� chart�� �׸����� �Ѵ�.
	//=====================================================================================
	int nChkCnt = 0;
	for( i = 0; i < nTot ; i++)	
	{
		if(m_listUsrAdd.GetCheck(i))
			nChkCnt++;
	}

	if(nChkCnt == 0) return;	
	//GetParent()->GetParent()->SendMessage(UMSG_COMPARE_TR_JONMOK_CNT, 0, (LPARAM)nChkCnt);
	//=====================================================================================

	for(i = 0; i < nTot ; i++)	
	{
		//üũ�� ���� ���Ͽ� ��ȸ�Ѵ�. 
		if(m_listUsrAdd.GetCheck(i))
		{	CString strCode, strCodeName;
			strCodeName = m_listUsrAdd.GetItemText(i, 0); strCodeName.TrimLeft(); strCodeName.TrimRight();
			strCode		= m_listUsrAdd.GetItemText(i, 1); strCode.TrimLeft(); strCode.TrimRight();
			
			//=================================== TR ��ȸ�� ������ ==============================					
			// ToolBar�� ������ ���� �����ϱ� ���� Toolbar�� ���� TR��û�� �Ѵ�.	
			GetParent()->GetParent()->SendMessage(UMSG_GET_COMPARECHART_DATA, (WPARAM)&strCode, (LPARAM)nChkCnt/*(LPARAM)&strCodeName*/);
			//===================================================================================
		}
	}	
}

void CUsrAddListDlg::SetRootPath(CString strPath)
{
	//	m_szImageDir = strPath + "\\Image\\Chart";	
}

BOOL CUsrAddListDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
//	switch(pMsg->message)
//	{
//		case WM_KEYDOWN:
//		{
//			switch(pMsg->wParam) 
//			{
//				case VK_ESCAPE:					
//				case VK_RETURN:
//					return TRUE;
//					break;	
//			}			
//		}
//		break;
//	}
	if(::IsWindow(m_hWnd))
		return GetParent()->PreTranslateMessage(pMsg);
	else
		return CRscDlg::PreTranslateMessage(pMsg);	
}


LONG CUsrAddListDlg::OnRecvMsg_CheckBoxStates_From_XListCtrl(WPARAM wp, LPARAM lp)
{		
	int		nItem		= (int)wp;
	int		nSubItem	= (int)lp;
	BOOL	bIsHide		= FALSE;
	bIsHide = m_listUsrAdd.GetCheckbox(nItem, nSubItem) ? FALSE : TRUE;

	
	//2005. 09. 14=============================================================================
	CString strCode, strName;
	strName = m_listUsrAdd.GetItemText(nItem, 0);
	strCode = m_listUsrAdd.GetItemText(nItem, 1);
	
	// ��Ʈ���� SHOW/HIDE
	// ��������� ��Ʈ�� ���̳� �����͸� SHOW/HIDE
	// �������̳� ����/�ɼ��� ������� �ұ����ϱ� ������
	// ,�� ������� �����Ͽ� �߰��ϹǷ� 
	// CChartItemData�� SetKorName(CString strName)�� �̸� �߰� ����� 
	// ���� ������� �̸��� �����Ѵ�.
	strName.TrimLeft(); strName.TrimRight();
	strName.Replace(" ", "");
	strName.Replace(",", "");
	((CStdDialog*)(GetParent()->GetParent()))->SetGraphHide3(strCode, strName, bIsHide);
	//=========================================================================================
	

	return 1;
}

LONG CUsrAddListDlg::OnRecvMsg_DBClick_From_XListCtrl(WPARAM wp, LPARAM lp)
{
	int nItem = (int)wp;
	int nSubItem = (int)lp;

	CString szCode = m_listUsrAdd.GetItemText(nItem, 1);	// col 1�϶� �����ڵ�.

	if(szCode.GetLength()>0)
	GetParent()->GetParent()->GetParent()->SendMessage(WM_USER+107, 0, (LPARAM)(LPCSTR)szCode);

	return 0L;
}

int CUsrAddListDlg::GetRequestList(CStringList& szCodeList, CStringList& szCodeNameList)
{
	int nTot = m_listUsrAdd.GetItemCount();
	for(int i = 0; i < nTot ; i++)	
	{
		CString strCode, strCodeName;
		strCode = m_listUsrAdd.GetItemText(i, 1); strCode.TrimLeft(); strCode.TrimRight();		
		szCodeList.AddTail(strCode);

		strCodeName = m_listUsrAdd.GetItemText(i, 0); strCodeName.TrimLeft(); strCodeName.TrimRight();		
		szCodeNameList.AddTail(strCodeName);
	}
	return nTot;
}

void CUsrAddListDlg::AddELWIndexCode()
{
	if(((CStdDialog*)(GetParent()->GetParent()))->m_nScreenType != CStdDialog::Screen_ELWChart)
		return;

	CString strIndexCode;
	CString strIndexName;

	CStdDialog* pStdDialog = (CStdDialog*)(GetParent()->GetParent());
	pStdDialog->GetELWIndexCode(m_strOpCode, strIndexCode, strIndexName);
	if(strIndexCode.IsEmpty() || strIndexName.IsEmpty())
		return;

	int nTot = m_listUsrAdd.GetItemCount();
	for(int i = 0; i < nTot ; i++)	
	{
		CString strCode;
		strCode = m_listUsrAdd.GetItemText(i, 1); strCode.TrimLeft(); strCode.TrimRight();
		
		if(strIndexCode.CompareNoCase(strCode) == 0)
			return;
	}

	m_strOpCode = strIndexCode;
	m_strOpCodeName =  strIndexName;

	AddItem();
}

void CUsrAddListDlg::ChangeSkinColor(COLORREF clrBackground)
{
	m_clrBkgrnd = clrBackground;
	Invalidate();
}

void CUsrAddListDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect(rcClient);
	dc.FillSolidRect(&rcClient, m_clrBkgrnd);
}
