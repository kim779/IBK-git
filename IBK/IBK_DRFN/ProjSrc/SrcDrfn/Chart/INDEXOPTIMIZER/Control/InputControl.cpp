// InPlaceEdit.cpp : implementation file
//

#include "stdafx.h"
#include "InputControl.h"
#include "LocalUtil.h"
#include "../StrategyCenterDef.h"
/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

#define CODE_LENGTH 6
#define FUTURE_LENGTH	8
/////////////////////////////////////////////////////////////////////////////
// CInputControl
CInputControl::CInputControl()
{
	m_bESC = false;
	mp_bHangulMode = FALSE;
	m_nType = 0;
	//m_bShowSearchList = TRUE;
	m_strPath = _T("");
}

CInputControl::~CInputControl()
{
	if(m_SearchList.GetSafeHwnd())
		m_SearchList.DestroyWindow();

	if(m_ctrlCodeList.GetSafeHwnd())
		m_ctrlCodeList.DestroyWindow();
}


BEGIN_MESSAGE_MAP(CInputControl, CEdit)
	//{{AFX_MSG_MAP(CInputControl)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputControl message handlers
//BOOL CInputControl::PreTranslateMessage(MSG* pMsg)
//{
////	if(pMsg->message == WM_KEYDOWN)   {
////		if(pMsg->wParam == VK_DELETE
////			|| pMsg->wParam == VK_ESCAPE
////			|| GetKeyState(VK_CONTROL))   
////		{
////			::TranslateMessage(pMsg);
////			::DispatchMessage(pMsg);
////			return true;
////		}
////	}
//	return CEdit::PreTranslateMessage(pMsg);
//}

void CInputControl::OnKillFocus(CWnd *pNewWnd)
{
	SetSel(-1, 0);
	CEdit::OnKillFocus(pNewWnd);
}

void CInputControl::OnNcDestroy()
{ 
	CEdit::OnNcDestroy();
}

#include <Imm.h>
int CInputControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	CFont *font = GetParent()->GetFont();
	SetFont(font);
//#ifdef _DEBUG
//CString strTEST;
//strTEST.Format("[gm0604]SetFocus() In int CInputControl::OnCreate(...");
//::OutputDebugString(strTEST);
//#endif
	SetFocus();
	SetSel(0, -1);

//	// �� �� �� : ������ (2002��1��7��   16��05��)
//	// �������� : CodeInput �� �㶧 �ѱ��Է¸�尡 ����Ʈ�� �Ǳ� ���� ��ƾ
//	HIMC hIMC = ::ImmGetContext(m_hWnd); //m_hWnd == Edit Control  Handle
//	if (hIMC != NULL)
//	{
//		DWORD dwConvert, dwWord;
//		::ImmGetConversionStatus(hIMC, &dwConvert, &dwWord);
//		dwConvert = (FALSE) ? dwConvert & ~IME_CMODE_NATIVE : dwConvert | IME_CMODE_NATIVE;
//		::ImmSetConversionStatus(hIMC, dwConvert, dwWord);
//		::ImmReleaseContext(m_hWnd, hIMC);
//	}	
//	// �����Ϸ� :(2002��1��7��   16��05��)	
//
//	//�ѱ�����ġ �������ϰ� �����ʹ� ���� �ʴ´�.(�ӵ������� FormInitialize �޽��� �Ŀ� �������Է�)
//	if(m_SearchList.GetSafeHwnd() == NULL)
//	{
//		CRect clRc(0,0,0,0);
//		GetClientRect(&clRc);	
//		m_SearchList.Create(clRc,this);
//	}
//
//	ReadMasterFile();

	return 0;
}

void CInputControl::OnLButtonUp(UINT nFlags, CPoint point)
{
//	int nStartChar=0, nEndChar=0;
//	GetSel( nStartChar, nEndChar );
//	if(nStartChar == nEndChar)
//	{	
//		SetFocus();
//		SetSel(0, -1);
//	}
//	else
//	{
//		SetSel(-1, -1);
//		CEdit::OnLButtonUp(nFlags, point);
//	}
	
	CEdit::OnLButtonUp(nFlags, point);
	SetSel(0, -1);
}

void CInputControl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnChar(nChar, nRepCnt, nFlags);

	CString strCode;
	GetWindowText(strCode);

	if( strCode.GetLength() >= CODE_LENGTH && m_nType ==0)
	{
//		if( strCode.GetLength() >= CODE_LENGTH && !mp_bHangulMode)
//		{
//			GetParent()->SendMessage(UDM_CODESHAREFROMEDITBOX, (WPARAM)0L, (LPARAM)0L);
//		}
		if(FP_bCheckCodeByList(strCode))
		{
			strCode.MakeUpper();
			SetWindowText(strCode);
			GetParent()->SendMessage(UDM_CODESHAREFROMEDITBOX, (WPARAM)0L, (LPARAM)0L);
		}
		else if(strCode.GetLength() >= CODE_LENGTH && mp_bHangulMode == FALSE)
		{			
			MessageBox("���� ��ȣ�� Ʋ���ϴ�.\n");
//			GetParent()->GetParent()->SendMessage(UMSG_SENDTOMSGBOX,(WPARAM)&"���� ��ȣ�� Ʋ���ϴ�.\n");
//			SetWindowText("");
//			SetSel(0, -1);
//#ifdef _DEBUG
//CString strTEST;
//strTEST.Format("[gm0604]SetFocus() In void CInputBarDlg::ChangeEditCode(...");
//::OutputDebugString(strTEST);
//#endif
//			SetFocus();//
		}
	}
	else if( strCode.GetLength() == FUTURE_LENGTH && m_nType==2)
	{
//		GetParent()->SendMessage(UMSG_INPUTFUTURECODE);
	}

}

/****************************************************************************
	�� �� ��: ������(Sung-Jin Do)
	��    ¥: 2002��3��28��   10��44��
	��    ��: FP_vEventSeachList(CString csData)
	��    ��: �ѱ������ �˻��� ���ؼ� �˻������� �߻��ϴ� �̺�Ʈ
****************************************************************************/
void CInputControl::FP_vEventSeachList(CString csData)
{
	SetWindowText(csData);
	if(FP_bCheckCodeByList(csData))
	{
		csData.MakeUpper();
		SetWindowText(csData);
		GetParent()->SendMessage(UDM_CODESHAREFROMEDITBOX, (WPARAM)0L, (LPARAM)0L);
	}
	else if(csData.GetLength() >= CODE_LENGTH)
	{
		SetSel(0,-1);
		GetParent()->SendMessage(UDM_CODESHAREFROMEDITBOX, (WPARAM)0L, (LPARAM)0L);
	}
}


/****************************************************************************
	�� �� ��: ������(Sung-Jin Do)
	��    ¥: 2002��3��28��   10��50��
	��    ��: FP_vShowComboList(BOOL bShow)
	��    ��: �ѱ������ �˻� �����츦 Show�Ѵ�.
****************************************************************************/
void CInputControl::FP_vShowComboList(BOOL bShow)
{
	if(m_SearchList.GetSafeHwnd())
	{
		m_SearchList.FP_vShowList(bShow);
	}
}

BOOL CInputControl::FP_bEnableHangulSearch()
{
/*
	//5,6,12,13,15
//	return FALSE;
	if(m_szCodeChoice[3] == '1' ||
		m_szCodeChoice[4] == '1' ||
		m_szCodeChoice[10] == '1' ||
		m_szCodeChoice[15] == '1' ||
		m_szCodeChoice[16] == '1' ||
		m_szCodeChoice[17] == '1'||
//		m_szCodeChoice[5] == '1'||
//		m_szCodeChoice[6] == '1'||
//		m_szCodeChoice[12] == '1'||
//		m_szCodeChoice[13] == '1'||
//		m_szCodeChoice[15] == '1'||
		m_szCodeChoice[20] == '1')
	{
		return FALSE;
	}
*/
	return TRUE;
}

void CInputControl::FT_vHangulSearch()
{
	CString csInputData = "";
	GetWindowText(csInputData);
	
	//�ѱ�����ġ
	BOOL bRet = FP_bEnableHangulSearch();
	if(!FP_bCheckNumeric(csInputData) && bRet)
	{
		//�ѱ�����ġ
		if(m_SearchList.m_ListCtrl.GetItemCount() <= 0 )
		{
			m_SearchList.FP_vAttachList(&m_ctrlCodeList);
		}
		
		mp_bHangulMode = TRUE;
		if(GetLimitText() != 0 )
			SetLimitText(0);
		FP_vShowComboList(TRUE);
		m_SearchList.SetSelection(csInputData,0);
		return;
	}
	else
	{
		mp_bHangulMode = FALSE;
		if((int)GetLimitText() != CODE_LENGTH )
			SetLimitText(CODE_LENGTH);
		FP_vShowComboList(FALSE);
	}
}

/********************************************************************
	�� �� ��: �� �� ��
	�� �� ��: 2001��12��10��   16��24��
	�� �� ��: FP_bCheckNumeric
	����Ÿ��: BOOL
	��    ��: CString csData = �Էµ� ������
	��    ��: ���� �Էµ� �����Ͱ� ���ڸ� �ִ����� üũ�Ѵ�.
*********************************************************************/
BOOL CInputControl::FP_bCheckNumeric(CString csData)
{
	for(int i=0; i< csData.GetLength() ; i++)
	{
		if(csData[i] < 48 || csData[i] > 57)
			return FALSE;
	}
	if(csData.IsEmpty())
		return TRUE;
	if(csData.GetLength() <= 0 )
		return TRUE;
	return TRUE;
}

LRESULT CInputControl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
//	CNeoInputExCtrl *pParent = (CNeoInputExCtrl *)GetParent();
//	TRACE("message = %d : wParam = %d : lParam = %d\n",message,wParam,lParam);
	if(m_nType==0)
	{
		if(message == 257 && wParam == 229)
		{	
			FP_vShowComboList(FALSE);
		}
		else if(message == 257 && wParam != 229)
		{		
			FT_vHangulSearch();
		}	
	}
	return CEdit::WindowProc(message, wParam, lParam);
}

void CInputControl::ReadMasterFile()
{
	FillCode_JMaster();
	FillCode_KMaster();
}

void CInputControl::FillCode_JMaster()
{
	CString szPath;
	GetFile("JMaster", szPath);

	CStringList szList;
	CCodeUtil util;
	util.Read_File(szPath, szList);

	int item = 0;
	for(POSITION pos = szList.GetHeadPosition(); pos; )
	{
		CString strData = szList.GetNext(pos);		
		CJMaster code;
		if(code.ReadData(strData)) 
		{
			item = m_ctrlCodeList.InsertItem(0, code.m_szCode);
			m_ctrlCodeList.SetItemText(item, 1, code.m_szName);
		}
	}	
}

void CInputControl::FillCode_KMaster()
{
	CString szPath;
	GetFile("KMaster", szPath);
	CStringList szList;
	CCodeUtil util;
	util.Read_File(szPath, szList);
	int item = 0;
	CString strData;
	CKMaster code;
	for(POSITION pos = szList.GetHeadPosition(); pos; )
	{
		strData = szList.GetNext(pos);		
		if(code.ReadData(strData)) 
		{
			item = m_ctrlCodeList.InsertItem(0, code.m_szCode);
			m_ctrlCodeList.SetItemText(item, 1, code.m_szName);
		}
	}
}

void CInputControl::GetFile(LPCSTR szKey, CString &szPath)
{
	//szPath.Format("%s%s%s%s", m_strPath, "\\Data\\", szKey, ".dat");
	szPath.Format("%s%s%s%s", m_strPath, "\\Icss\\", szKey, ".dat");	//@Solomon
}

void CInputControl::SetPath(CString strPath)
{
	m_strPath = strPath;

	// �� �� �� : ������ (2002��1��7��   16��05��)
	// �������� : CodeInput �� �㶧 �ѱ��Է¸�尡 ����Ʈ�� �Ǳ� ���� ��ƾ
	HIMC hIMC = ::ImmGetContext(m_hWnd); //m_hWnd == Edit Control  Handle
	if (hIMC != NULL)
	{
		DWORD dwConvert, dwWord;
		::ImmGetConversionStatus(hIMC, &dwConvert, &dwWord);
		dwConvert = (FALSE) ? dwConvert & ~IME_CMODE_NATIVE : dwConvert | IME_CMODE_NATIVE;
		::ImmSetConversionStatus(hIMC, dwConvert, dwWord);
		::ImmReleaseContext(m_hWnd, hIMC);
	}	
	// �����Ϸ� :(2002��1��7��   16��05��)	
	
	//�ѱ�����ġ �������ϰ� �����ʹ� ���� �ʴ´�.(�ӵ������� FormInitialize �޽��� �Ŀ� �������Է�)
	if(m_SearchList.GetSafeHwnd() == NULL)
	{
		CRect clRc(0,0,0,0);
		GetClientRect(&clRc);	
		m_SearchList.Create(clRc,this);
	}

	int nEnd = 0;
	m_ctrlCodeList.Create (WS_CHILD|LVS_NOSORTHEADER|LVS_REPORT|LVS_SINGLESEL|
		LVS_SHOWSELALWAYS|LVS_ALIGNTOP|LVS_SORTASCENDING, 
		CRect (0, 0, nEnd*2+100, 150), this, 2);
	
	m_ctrlCodeList.SetExtendedStyle(LVS_EX_TRACKSELECT | LVS_EX_FULLROWSELECT);
	m_ctrlCodeList.InsertColumn(0, "�ڵ�", LVCFMT_LEFT, nEnd, 1);
	m_ctrlCodeList.InsertColumn(1, "�����", LVCFMT_LEFT, nEnd+80, 1);
	
	ReadMasterFile();
	m_SearchList.FP_vAttachList(&m_ctrlCodeList);

	SetSel(0, -1);
//#ifdef _DEBUG
//CString strTEST;
//strTEST.Format("[gm0604]SetFocus() In void CInputBarDlg::ChangeEditCode(...");
//::OutputDebugString(strTEST);
//#endif
	SetFocus();
}

void CInputControl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(!mp_bHangulMode)
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	if(nChar == VK_UP)// && Parent->IsSimpleType())
	{
		m_SearchList.FP_vEventUpDown(VK_UP);
	}
	
	if(nChar == VK_DOWN)// && Parent->IsSimpleType())
	{
		m_SearchList.FP_vEventUpDown(VK_DOWN);
	}
	
	if(nChar == 34) // PageDown
	{
		m_SearchList.FP_vEventUpDown(34);
	}
	if(nChar == 33) // PageUp
	{
		m_SearchList.FP_vEventUpDown(33);
	}

	if(nChar == 36) // Home
	{
		m_SearchList.FP_vEventUpDown(36);
	}

	if(nChar == 35) // End
	{
		m_SearchList.FP_vEventUpDown(35);
	}

	if(nChar == VK_LEFT  || nChar == VK_RIGHT) 
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CInputControl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch(nChar)
	{
	case VK_RETURN:
		{
			if(mp_bHangulMode)	// �ѱ�����ġ������ ����
			{					// SelChanged �̺�Ʈ �߻�
				CString csData = m_SearchList.FP_csEditEnter();					
				FP_vEventSeachList(csData);
				mp_bHangulMode = FALSE;
				SetSel(0,-1);				
			}
			else
			{
				CString csData="";
				CString strCode;
				GetWindowText(strCode);
				if(strCode.GetLength()==FUTURE_LENGTH)
				{
//					GetParent()->SendMessage(UMSG_INPUTFUTURECODE);
				}
				else
				{
					FP_vEventSeachList(strCode);
				}				
			}
		break;
		}
	case VK_TAB:
		{
			FP_vShowComboList(FALSE);
			mp_bHangulMode = FALSE;
		}
	default:{}
	}
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}


BOOL CInputControl::FP_bCheckCodeByList()
{
	CString strCode;
	GetWindowText(strCode);
	return FP_bCheckCodeByList(strCode);
}

BOOL CInputControl::FP_bCheckCodeByList(CString szCode)
{
	if(!m_ctrlCodeList.GetSafeHwnd())
		return FALSE;
	szCode.MakeUpper();
	for(int i = 0; i < m_ctrlCodeList.GetItemCount() ; i++)
	{
		CString str = m_ctrlCodeList.GetItemText(i,0);
		str.TrimLeft();
		str.TrimRight();
		str.MakeUpper();		
		if(str==szCode)
			return TRUE;
	}
	return FALSE;
}

void CInputControl::OnRButtonUp(UINT nFlags, CPoint point) 
{
//	// TODO: Add your message handler code here and/or call default
//	char* szIndexGroupName[] = { "�����ڵ� ����ȭ"};//, "���ɵ��"};
//	long lAllCnt = sizeof(szIndexGroupName)/sizeof(char*);
//	CString strCode;
//	GetWindowText(strCode);
//
//    CMenu menu;
//    menu.CreatePopupMenu();
//
//	long lPosAll = 0;
//	long lSize = 0;
//	CString strIndicator;
//	for(long lIndex = 0;lIndex<lAllCnt;lIndex++)
//	{
//		menu.AppendMenu(MF_STRING,(UINT)1000+lIndex,szIndexGroupName[lIndex]);
//	}
//
//	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
//	ClientToScreen(&point);
//	int nRetValue = menu.TrackPopupMenu(nFlagsForMenu, point.x, point.y, this);
//	switch(nRetValue)
//	{
//	case 1000:	// �ڵ���� 
//		{
//			GetParent()->GetParent()->SendMessage(UMSG_SENDSYNCCODE,(WPARAM)(LPCTSTR)strCode);
//		}		
//		break;
//	case 1001:	// ���ɵ��
//		{
//			INT_TOTAL		data;
//			memset(&data,0x00,sizeof(INT_TOTAL));
//			strcpy(data.Igubun,REQUEST_REG_ITEMS);
//			strcpy(data.IopenDlg,"0");
//			strcpy(data.Icnt,"1");
//
//			CPtrArray arrGroup;
//			data.Idata = (void*)&arrGroup;
//
//			INT_GROUP_ST Group;
//			memset(&Group,0x00,sizeof(INT_GROUP_ST));
//			sprintf(Group.Icnt,"2");
//			sprintf(Group.Ino,"0");
//			sprintf(Group.Ititle,"JJimTemp");
//			arrGroup.Add(&Group);
//			CPtrArray ArrItem;
//			INT_ITEM_ST* pItem = NULL;
//			Group.Idata = (void*)&ArrItem;
//
//			pItem = new INT_ITEM_ST;
//			memset(pItem,0x20,sizeof(INT_ITEM_ST));
//			memcpy(pItem->Icode,(LPCTSTR)strCode,strCode.GetLength());
//			ArrItem.Add(pItem);
//		
//			GetParent()->GetParent()->GetParent()->GetParent()->GetParent()->SendMessage(RMSG_INTREQUEST,(WPARAM)GetSafeHwnd(),(LPARAM)&data);	
//
//			delete pItem;
//			ArrItem.RemoveAll();
//		}
//		break;
//	default:
//		break;
//	}
//	
//	menu.DestroyMenu();
//	
//	CDialog::OnRButtonUp(nFlags, point);
}


void CInputControl::SetType(int nType)
{
	if(nType==2)
	{
		SetLimitText(8);
	}
	else if(nType==1)
	{
		SetLimitText(4);
	}
	else
	{
		SetLimitText(6);
	}
	SetWindowText("");
	m_nType = nType;
}