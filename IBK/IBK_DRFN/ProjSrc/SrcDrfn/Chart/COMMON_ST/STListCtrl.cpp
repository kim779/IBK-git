// STListCtrl.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "STListCtrl.h"
#include "../../inc/ISTManager.h"
#include ".\stlistctrl.h"
#include "../../inc/IAUDataMonitorMng.h"	// UUID_IAUDataMonitorManager : �ݹ��Լ� �޴µ� ���

const UINT RMSG_OB_ITEMCLICK = ::RegisterWindowMessage("RMSG_OB_ITEMCLICK");
const UINT RMSG_OB_ITEMDBLCLK = ::RegisterWindowMessage("RMSG_OB_ITEMDBLCLK");
const UINT RMSG_ISSTRUNSTATE = ::RegisterWindowMessage(_T("RMSG_ISSTRUNSTATE"));

IMPLEMENT_DYNAMIC(CSTListCtrl, CListCtrl)
CSTListCtrl::CSTListCtrl()
{
	m_pDC = NULL;
	m_pOldFont = NULL;
}

CSTListCtrl::~CSTListCtrl()
{
	if (m_fontBold.GetSafeHandle())
		m_fontBold.DeleteObject();
}


BEGIN_MESSAGE_MAP(CSTListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClk)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
END_MESSAGE_MAP()


void CSTListCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem<0) return;

	CString szText = GetItemText(pNMListView->iItem, pNMListView->iSubItem);
	int nType = (int)GetItemData(pNMListView->iItem);

	if(!szText.IsEmpty()) {
		STMNG_ITEMINFO Info;
		Info.szSTPath = m_szFullPath;
		switch(nType)
		{
//			case 1:
//				Info.szSTName.Format(_T("P_%s%c%d"), szText, gSTGubunKey, nType);
//				break;
			case 4:
			case 5:
//				Info.szSTName.Format(_T("P_%s%c%d"), szText, gSTGubunKey, nType);
				if(m_nFolderIndx == 3)
					Info.szSTName.Format(_T("P_%s%c%d"), szText, gSTGubunKey, nType);	// ������
				else
					Info.szSTName.Format(_T("%s%c%d"), szText, gSTGubunKey, nType);	// ����ڿ�
				break;
			case 9999:
				Info.szSTName = szText;
				break;
			default:
				Info.szSTName.Format(_T("%s%c%d"), szText, gSTGubunKey, nType);
				break;
		}

		SetUserSetting(0, pNMListView->iItem, m_bUseBoldFont);

		Info.nSTType = m_nSTFolderType;
		Info.szSTRData = m_szFolderName;
		Info.nFolderIndx = m_nFolderIndx;
		Info.nItemIndx = pNMListView->iItem;

		Invalidate();
		if(GetItemState(pNMListView->iItem, LVIS_SELECTED))
		GetParent()->GetOwner()->SendMessage(RMSG_OB_ITEMDBLCLK, (WPARAM)0, (LPARAM)&Info);
	}

	*pResult = 0;
}

void CSTListCtrl::OnNMClk(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem<0) return;

	CString szText = GetItemText(pNMListView->iItem, pNMListView->iSubItem);
	int nType = (int)GetItemData(pNMListView->iItem);

	if(!szText.IsEmpty()) {
		STMNG_ITEMINFO Info;
		Info.szSTPath = m_szFullPath;
		if(nType == 9999)
			Info.szSTName = szText;
		else
			Info.szSTName.Format(_T("%s%c%d"), szText, gSTGubunKey, nType);

		Info.nSTType = m_nSTFolderType;
		Info.szSTRData = m_szFolderName;
		Info.nFolderIndx = m_nFolderIndx;
		Info.nItemIndx = pNMListView->iItem;
		GetParent()->GetOwner()->SendMessage(RMSG_OB_ITEMCLICK, (WPARAM)0, (LPARAM)&Info);
	}

	*pResult = 0;
}

void CSTListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

	switch(lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;          // ask for item notifications.
		break;

	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_DODEFAULT;

//		m_pDC = CDC::FromHandle(lplvcd->nmcd.hdc);
//		if(GetUserSetting(0, lplvcd->nmcd.dwItemSpec)==TRUE)
//		{
//			m_pOldFont = m_pDC->SelectObject(&m_fontBold);
//		}
//		else {
//			if(m_pOldFont)
//			{
//				m_pDC->SelectObject(m_pOldFont);
//				m_pOldFont = NULL;
//			}
//		}

		*pResult = CDRF_NEWFONT;
		break;

	default:
		*pResult = CDRF_DODEFAULT;
	}
}

void CSTListCtrl::InitFont(BOOL bUseBoldFont)
{
	CClientDC dc(this);

//	dc.GetCurrentFont()->GetLogFont(&m_logFont);
	::memset(&m_logFont, 0x00, sizeof(LOGFONT));
	m_logFont.lfWidth = 0; 
	m_logFont.lfEscapement = 0; 
	m_logFont.lfOrientation = 0; 
	m_logFont.lfWeight = FW_NORMAL; 
	m_logFont.lfItalic = 0; 
	m_logFont.lfUnderline = 0; 
	m_logFont.lfStrikeOut = 0; 
	m_logFont.lfCharSet = DEFAULT_CHARSET; 
	m_logFont.lfOutPrecision = OUT_DEFAULT_PRECIS; 
	m_logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	m_logFont.lfQuality = DEFAULT_QUALITY;
	m_logFont.lfPitchAndFamily = FF_ROMAN; 
	strcpy(m_logFont.lfFaceName, "����ü");
	m_logFont.lfHeight = 12;
//	m_logFont.lfWeight = FW_BOLD;

	if(m_fontBold.GetSafeHandle() == NULL)
	{
		m_fontBold.CreateFontIndirect(&m_logFont);
		SetFont(&m_fontBold);
	}

	m_bUseBoldFont = bUseBoldFont;
}

void CSTListCtrl::SetUserSetting(int nType, int nIndex, BOOL bValue)
{
	if(bValue)
	{
		int nCount = GetItemCount();
		if(nCount<=nIndex) return;

		m_UserSelState.SetAt(nIndex, (DWORD)m_UserSelState.GetAt(nIndex) ? FALSE : TRUE);
	}
}

BOOL CSTListCtrl::GetUserSetting(int nType, int nIndex)
{
	int nCount = GetItemCount();
	if(nCount<=nIndex) 
		return FALSE;
	else if(nCount >= m_UserSelState.GetSize())
		m_UserSelState.SetSize(nCount);

	return (BOOL)m_UserSelState.GetAt(nIndex);	
}

void CSTListCtrl::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem<0) return;

	if(m_nSTFolderType == 0)
	{
		// AfxMessageBox("������ ������ ������ �� �����ϴ�.");
		*pResult = 0;
		return;
	}

	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, 50001, "����");

	CPoint pt;
	::GetCursorPos(&pt);

	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	int nReturn = menu.TrackPopupMenu(nFlagsForMenu, pt.x, pt.y, this);
	menu.DestroyMenu();

	if(nReturn >0 && m_nSTFolderType == 0)
	{
		AfxMessageBox("������ ������ ������ �� �����ϴ�.");
		*pResult = 0;
	}

	if(nReturn == 50001)
	{
		CString szText = GetItemText(pNMListView->iItem, pNMListView->iSubItem);
		int nType = (int)GetItemData(pNMListView->iItem);
		if(!szText.IsEmpty()) {
			STMNG_ITEMINFO Info;
			Info.szSTPath = m_szFullPath;
			if(nType == 9999)
				Info.szSTName = szText;
			else
				Info.szSTName.Format(_T("%s%c%d"), szText, gSTGubunKey, nType);

			Info.nSTType = m_nSTFolderType;
			Info.szSTRData = m_szFolderName;
			Info.nFolderIndx = m_nFolderIndx;
			Info.nItemIndx = pNMListView->iItem;

			// ��������Ʈ�� ����Ǿ��ٴ� ������ �˸���.
			int nID = UUID_IAUDataMonitorManager;
			IAUDataMonitorManager* pMng = (IAUDataMonitorManager*)AfxGetPctrInterface(nID);

			//Ext_STChange_SDDelete=0x20,		//��������� ����ڴ�����������
			//Ext_STChange_SDAdd=0x21,			//��������� ����ڴ��������߰�
			//Ext_STChange_Delete=0x40,			//��������� �����������������
			//Ext_STChange_Add=0x41,			//��������� ��������������߰�
			int nState = 0;
			if(m_szFolderName.Compare("����ڴ�������")==0)
				nState = 0x20;
			else if(m_szFolderName.Compare("�������ǥ")==0)
				nState = 0x20;
			else if(m_szFolderName.Compare("����ڰ����༼")==0)
				nState = 0x20;
			else if(m_szFolderName.Compare("�������������")==0)
				nState = 0x40;

			if(nState > 0 && AfxMessageBox("������ �����Ͻðڽ��ϱ�?", MB_YESNO)==IDYES)
			{
				CString strSTPath;
				strSTPath.Format("%s\\%s.ST", Info.szSTPath, Info.szSTName);
				CString strSTName(Info.szSTName);
				BOOL bRunState = GetParent()->GetOwner()->SendMessage(RMSG_ISSTRUNSTATE, (WPARAM)(LPCTSTR)strSTPath, (LPARAM)(LPCTSTR)strSTName);
				if(bRunState)
				{
					AfxMessageBox("��Ʈ�� ������ ������ �Ǿ� �ִ��� Ȯ���Ͻñ� �ٶ��ϴ�.");
				}
				else
				{
					::DeleteFile(strSTPath);
					pMng->NotifyData("STLIST_MODIFIED", nState, (LONG)(LPCSTR)strSTName);
				}
			}
		}
	}

	*pResult = 0;
}
