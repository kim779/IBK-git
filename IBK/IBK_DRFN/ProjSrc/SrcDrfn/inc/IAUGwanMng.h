#if !defined(AFX_GWAN_INTERFACE__BY_JUNOKLEE_20060208__INCLUDED_)
#define AFX_GWAN_INTERFACE__BY_JUNOKLEE_20060208__INCLUDED_

//#include "IBaseDefine.h"

#include "./Define_Gwansim.h"
#include "./IAUDataMonitorMng.h"

// #include "../../_include/IAUGwanMng.h"
//  int nID = 9;
// 	IAUGwanManager* pMng = (IAUGwanManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUGwanManager* pMng = (IAUGwanManager*)AfxGetPctrInterface(nID);

// ���� : ���ɵ�� ���� �Ŵ���.
//
// ������  : ���ؿ� 2006.02.08(��)
// History : 2006.02.08(��) �������̽� ����
//

/*
// Receive����Ÿ ó��....

LRESULT CYourClassName::DataMonitorCallbackProc(LPCSTR szKey, long pThisPoint, long dwKey, long dwSubKey, long dwNotifyData)
{
	CYourClassName *pThis = (CYourClassName*)pThisPoint;
	GWANSIM_IntRequest* pNotifyData = (GWANSIM_IntRequest*)dwNotifyData;
	WPARAM wParam = pNotifyData->wparam;
	LPARAM lParam = pNotifyData->lparam;


	INT_TOTAL* pData = (INT_TOTAL*)lParam;
	CINT_ITEM_ST  OneItem;

	CPtrArray* pArGroupAll = (CPtrArray*)pData->Idata;
	if(pArGroupAll) {
		int nSize = pArGroupAll->GetSize();
		char szNo[3] = {0, };
		INT_GROUP_ST* pIntGroup = NULL;
		CINT_GROUP_ST itemGwan;
		for(int nIndx = 0; nIndx < nSize; ++nIndx)
		{
			pIntGroup = (INT_GROUP_ST*)pArGroupAll->GetAt(nIndx);
			itemGwan.Convert(pIntGroup);

			itemGwan.LocalTRACE("CGwanWnd::OnIntRequest");

			CPtrArray* pGroupItem = (CPtrArray*)itemGwan.m_pIdata;
			int nSize = pGroupItem->GetSize();

			INT_ITEM_ST* pItem = NULL;
			for(int nIndx = 0; nIndx < nSize; ++nIndx)
			{
				if(pItem = (INT_ITEM_ST*)pGroupItem->GetAt(nIndx)) {
					OneItem.Convert(pItem);
					OneItem.LocalTRACE("CGwanWnd::FindGroup");
				}
			}
		}
	}

	return 0L;
}
*/


typedef struct  {
	WPARAM wparam;
	LPARAM lparam;
} GWANSIM_IntRequest;

DECLARE_INTERFACE(IAUGwanManager)
{
	STDMETHOD_(long, Advise)(LPCSTR szKey) PURE;

	// I-PARAM : Advise���� ���Ϲ��� Ű��.
	// O-PARAM : 
	STDMETHOD_(void, UnAdvise)(long dwRet) PURE;


	// nType : 0 ������ü. 
	//           ���ο��� RMSG_INTREQUEST �޽����� �޾��� ���� ������ ���� �״����.
	STDMETHOD_(long, GetGwansimDataAll)(long dwRet, int nType, LPCSTR szGroup=NULL) PURE;
	STDMETHOD_(void, GetGwansimGroupName)(long dwRet, CStringArray &arrKey, CStringArray &arrName) PURE;
	STDMETHOD_(void, GetGwansimItem)(long dwRet, CString strKey, CStringArray& arrCode, CStringArray& arrName) PURE;
	STDMETHOD_(int, ShowGwansimDlg)(long dwRet, CWnd* pCaller, CString strCodeList="") PURE;
	STDMETHOD_(BOOL,ADDGwasimItem)(long dwRet, CString strKey, CString strItem, BOOL bFirst=FALSE) PURE;
//	STDMETHOD_(BOOL,ADDGwasimItems)(long dwRet, CString strKey, CStringArray& arrItem) PURE;
	STDMETHOD_(int,SetRegGwansim)(long dwRet, CWnd* pComboWnd, int nType) PURE;
	STDMETHOD_(int,  RegiestToImsiGroup)(long dwRet, CStringArray& arrCode, CWnd* pWnd) PURE;
	
	//�׷��� Ű������
	STDMETHOD_(LPCSTR, GetGwanKeyFromName)(long dwRet, LPCSTR szGwanGroupName) PURE;
	STDMETHOD_(int, ShowGwansimDlgWithList)(long dwRet, CWnd* pWnd, CStringArray& arrItems) PURE;

	//���н����� ���� ���� ������ Parsing�ؼ� �ʿ��� ������ �ѱ��
	STDMETHOD_(int, ParseGwanList)(WPARAM wParam, LPARAM lParam, CString &szGroup, CStringList &szItemList) PURE;

	//������ ���ɱ׷� ����Ʈ ��û.
	STDMETHOD_(void, RequestGwansimGroup)(long dwRet) PURE;
	STDMETHOD_(int, ParseGroup)(LPARAM lParam, CStringArray & arrCode, CStringArray & arrName) PURE;//tour2k
};

//IAUGwanManager���� long dwRet�� ������..
DECLARE_INTERFACE(IWinixGwanManager)
{
	STDMETHOD_(long, Advise)(LPCSTR szKey) PURE;
	STDMETHOD_(void, UnAdvise)() PURE;
	
	STDMETHOD_(long, GetGwansimDataAll)(int nType, LPCSTR szGroup=NULL) PURE;
	STDMETHOD_(void, GetGwansimGroupName)(CStringArray &arrKey, CStringArray &arrName) PURE;
	STDMETHOD_(void, GetGwansimItem)(CString strKey, CStringArray& arrCode, CStringArray& arrName) PURE;
	STDMETHOD_(int, ShowGwansimDlg)(CWnd* pCaller, CString strCodeList="") PURE;
	STDMETHOD_(BOOL,ADDGwasimItem)(CString strKey, CString strItem, BOOL bFirst=FALSE) PURE;
	//	STDMETHOD_(BOOL,ADDGwasimItems)(CString strKey, CStringArray& arrItem) PURE;
	STDMETHOD_(int,SetRegGwansim)(CWnd* pComboWnd, int nType) PURE;
	STDMETHOD_(int,  RegiestToImsiGroup)(CStringArray& arrCode, CWnd* pWnd) PURE;
	STDMETHOD_(LPCSTR, GetGwanKeyFromName)(LPCSTR szGwanGroupName) PURE;
	STDMETHOD_(int, ShowGwansimDlgWithList)(CWnd* pWnd, CStringArray& arrItems) PURE;

	//���н����� ���� ���� ������ Parsing�ؼ� �ʿ��� ������ �ѱ��
	STDMETHOD_(int, ParseGwanList)(WPARAM wParam, LPARAM lParam, CString &szGroup, CStringList &szItemList) PURE;

	//������ ���ɱ׷� ����Ʈ ��û.
	STDMETHOD_(void, RequestGwansimGroup)() PURE;
	STDMETHOD_(int, ParseGroup)(LPARAM lParam, CStringArray & arrCode, CStringArray & arrName) PURE;//tour2k
};

#define DECLARE_IWinixGwanManager_MAP()		public:	\
	IAUGwanManager*	m_pGwanMng;	\
	long m_pGwandwRet;	\
	STDMETHOD_(long, Advise)(LPCSTR szKey);	\
	STDMETHOD_(void, UnAdvise)();	\
	STDMETHOD_(long, GetGwansimDataAll)(int nType, LPCSTR szGroup=NULL);	\
	STDMETHOD_(void, GetGwansimGroupName)(CStringArray &arrKey, CStringArray &arrName);	\
	STDMETHOD_(void, GetGwansimItem)(CString strKey, CStringArray& arrCode, CStringArray& arrName);	\
	STDMETHOD_(int, ShowGwansimDlg)(CWnd* pCaller, CString strCodeList="");	\
	STDMETHOD_(BOOL,ADDGwasimItem)(CString strKey, CString strItem, BOOL bFirst=FALSE);	\
	STDMETHOD_(int,SetRegGwansim)(CWnd* pComboWnd, int nType);	\
	STDMETHOD_(int,  RegiestToImsiGroup)(CStringArray& arrCode, CWnd* pWnd);	\
	STDMETHOD_(LPCSTR, GetGwanKeyFromName)(LPCSTR szGwanGroupName);	\
	STDMETHOD_(int, ShowGwansimDlgWithList)(CWnd* pWnd, CStringArray& arrItems);	\
	STDMETHOD_(int, ParseGwanList)(WPARAM wParam, LPARAM lParam, CString &szGroup, CStringList &szItemList) ;	\
	STDMETHOD_(void, RequestGwansimGroup)();\
	STDMETHOD_(int, ParseGroup)(LPARAM lParam, CStringArray & arrCode, CStringArray & arrName);

#define IMPLEMENT_IWinixGwanManager_MAP()	long CWinixGwanManager::Advise(LPCSTR szKey)	{m_pGwandwRet = m_pGwanMng->Advise(szKey);	return m_pGwandwRet;}	\
	void CWinixGwanManager::UnAdvise()	{m_pGwanMng->UnAdvise(m_pGwandwRet);}		\
	long CWinixGwanManager::GetGwansimDataAll(int nType, LPCSTR szGroup)	{return m_pGwanMng->GetGwansimDataAll(m_pGwandwRet, nType, szGroup);}		\
	void CWinixGwanManager::GetGwansimGroupName(CStringArray &arrKey, CStringArray &arrName)		\
		{m_pGwanMng->GetGwansimGroupName(m_pGwandwRet, arrKey, arrName);}		\
	void CWinixGwanManager::GetGwansimItem(CString strKey, CStringArray& arrCode, CStringArray& arrName)		\
		{m_pGwanMng->GetGwansimItem(m_pGwandwRet, strKey, arrCode, arrName);}		\
	int CWinixGwanManager::ShowGwansimDlg(CWnd* pCaller, CString strCodeList)		\
		{return m_pGwanMng->ShowGwansimDlg(m_pGwandwRet, pCaller, strCodeList);}		\
	BOOL CWinixGwanManager::ADDGwasimItem(CString strKey, CString strItem, BOOL bFirst)		\
		{return m_pGwanMng->ADDGwasimItem(m_pGwandwRet, strKey, strItem, bFirst);}		\
	int CWinixGwanManager::SetRegGwansim(CWnd* pComboWnd, int nType)		\
		{return m_pGwanMng->SetRegGwansim(m_pGwandwRet, pComboWnd, nType);}		\
	int CWinixGwanManager::RegiestToImsiGroup(CStringArray& arrCode, CWnd* pWnd)		\
		{return m_pGwanMng->RegiestToImsiGroup(m_pGwandwRet, arrCode, pWnd);}		\
	LPCSTR CWinixGwanManager::GetGwanKeyFromName(LPCSTR szGwanGroupName)		\
		{return m_pGwanMng->GetGwanKeyFromName(m_pGwandwRet, szGwanGroupName);}		\
	int CWinixGwanManager::ShowGwansimDlgWithList(CWnd* pWnd, CStringArray& arrItems)		\
		{return m_pGwanMng->ShowGwansimDlgWithList(m_pGwandwRet, pWnd, arrItems);}		\
	int CWinixGwanManager::ParseGwanList(WPARAM wParam, LPARAM lParam, CString &szGroup, CStringList &szItemList)			\
		{return m_pGwanMng->ParseGwanList(wParam, lParam, szGroup, szItemList);}		\
	void CWinixGwanManager::RequestGwansimGroup()		\
		{ m_pGwanMng->RequestGwansimGroup(m_pGwandwRet);	}\
	int CWinixGwanManager::ParseGroup(LPARAM lParam, CStringArray & arrCode, CStringArray & arrName)		\
		{return m_pGwanMng->ParseGroup( lParam,   arrCode,   arrName);}		

//����Tree�� �ִ� �θ������쿡�� ������ �޽���
const UINT RMSG_GWANSIMREGISTER		=   ::RegisterWindowMessage("RMSG_GWANSIMREGISTER");

//@Solomon For winix-->
#define WM_NOTIFY_LOAD		WM_USER + 100
#define WM_NOTIFY_SAVE		WM_USER + 101
#define WM_NOTIFY_GROUP		WM_USER + 102
#define WM_NOTIFY_GROUPLIST	WM_USER + 103
#define WM_NOTIFY_SAVECODE	WM_USER + 104
//@Solomon For winix<--

#endif // AFX_GWAN_INTERFACE__BY_JUNOKLEE_20060208__INCLUDED_
