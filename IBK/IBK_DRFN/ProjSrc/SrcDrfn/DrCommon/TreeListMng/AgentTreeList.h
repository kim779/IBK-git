#if !defined(AFX_AGENTTREELISTMANAGER_IMPL_DLDLDEE_H__AF_AD91_7EE99FC62AAF__INCLUDED_)
#define AFX_AGENTTREELISTMANAGER_IMPL_DLDLDEE_H__AF_AD91_7EE99FC62AAF__INCLUDED_

// #include "AgentTreeList.h"
// AgentTreeList.h : header file
//
#include <AFXTEMPL.H>
#include "../../Inc/IGateMng.h"
#include "../../Inc/IMainInfoMng.h"
#include "../../Inc/ITreeListMng.h"
#include "../../Inc/IAUTrCommMng.h"
//#include "../../Dlls/CommonTR/TreeItem.h"
#include "../../Inc/IWinixPlatform.h"

#include "LoadConditionMng.h"		// ��Ŭ���߰��� ���ؼ�..
#include "TreeList_Thema.h"

#include "Account.h"
const UINT RMSG_SENDMESSAGE = ::RegisterWindowMessage(_T("RMSG_SENDMESSAGE"));

class CPacketInfo
{
public:
	ITLMCallback* m_pCallback;
	char		m_aDataType;
	long		m_dwReserved;
};
typedef CList<CPacketInfo, CPacketInfo> LIST_CPacketInfo;

//class CTreeResItemEx
//{
//public:
//	ITrComm*		m_pTrComm;
//	LIST_CItemLongData*		m_pItemList;
//	HTREEITEM		m_hGwansim;
//	HTREEITEM		m_hImsi;
//
//public:
//	CTreeResItemEx()
//	{
//		m_pTrComm = NULL;
//		m_pItemList = NULL;
//		m_hGwansim = NULL;
//		m_hImsi = NULL;
//	}
//};
//typedef CList<CTreeResItemEx*, CTreeResItemEx*> LIST_CTreeResItemEx;
// LoadConditionMng.h�� �̵�.

class CTreeListManager : public ITreeListManager2
{
public:
	STDMETHOD_(void, SetKeyData)(LPCSTR szKey, LPCSTR szData) ;
	STDMETHOD_(void, SetKeyData)(LPCSTR szKey, long   dwData) ;

	STDMETHOD_(LONG, LoadTreeList)(CInputCodeListEx *pInData, LPCSTR szSection="TREELIST");
	STDMETHOD_(void, UnLoadTreeList)(LONG dwKey);
//	STDMETHOD_(BOOL, LoadFile)(CInputCodeListEx *pInData, LPCSTR szSection="LIST");
	STDMETHOD_(void, RequestData)(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved=0);
	STDMETHOD_(void, ChangeConfigFile)(LPCSTR szFileName);

	STDMETHOD_(void*, GetITrComm)(LONG dwKey);

	// pITrComm : GetITrComm���� �Ѱܹ�����.
	STDMETHOD_(void, RequestCodeData)(void* pITrComm, LIST_TR_CODEINFO* pCodeList, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved=0) ;

	// ���ɱ׷� Ʈ���� �ٽ� �ε��ϵ��� ��û.
	STDMETHOD_(void, ReloadGwansimGroup)(LONG dwKey, CTreeCtrl* pTreeCtrl);

	// ���ɱ׷��� ������ TREE�� HITEM
	STDMETHOD_(HTREEITEM, GetGwansimItem)(CTreeCtrl* pTreeCtrl);

	// Ʈ������ hTreeRoot�ؿ��� ������ �̸��� ���´�. ����������.
	// ���� ���ɱ׷�, �ӽñ׷��..
	// hTreeRoot�̸� �������� �˻�, 
	// ������������ �˻��� ���� ������. ���� ���������� ã�´�.
	// hRootItem�� NULL�ƴѰ��� ���� �������� ����. ���� �ʿ䰡 ���...
	STDMETHOD_(HTREEITEM, GetTreeItem)(CTreeCtrl* pTreeCtrl, LPCSTR szFolder, HTREEITEM hTreeRoot=NULL);

	// Ʈ���� ���ɰ��� ������ �Ѵ�.
	// ������ ���ɿ��� IMainExInfoManager�������̽��� ���ؼ�. IMainExInfoManager_Gwansim���� Key�� ����ϴ� �κ��� 
	// (����ȿ����) Ʈ���������̽����� �߰��� ����.
	// ���� 1.���ɱ׷��� �����ϱ� ���� �� �����͸� �����ϴ� ���ɿ����� ���������͸� ���������.
	//      2.���� ������ �� �� �����͵� ���������.
	STDMETHOD_(void, GwansimRegist)(long dwData);

	// Ʈ���� �ӽñ׷쿡 ����ϴ� ���. �������� ����� ���� �����.
	//  szSubKey�� ������� �����Ƿ� NULL, ���� �ӽñ׷� �׿� �׷���� �� ���� ��� ���.
	STDMETHOD_(int, ImsiRegist)(LPCSTR szSubKey, long dwData);

	// Ʈ���� �ӽñ׷��� ������ ����� �Լ�
	//  szSubKey�� ������� �����Ƿ� NULL, ���� �ӽñ׷� �׿� �׷���� �� ���� ��� ���.
	STDMETHOD_(int, ImsiDelete)(LPCSTR szSubKey);

	// Ʈ���� �ӽñ׷쿡 �߰��ϴ� �Լ�
	//  szSubKey�� ������� �����Ƿ� NULL, ���� �ӽñ׷� �׿� �׷���� �� ���� ��� ���.
	// ���� �ӽñ׷��� �����͸� �������ʰ� �߰��ϴ� ���
	// STDMETHOD_(int, ImsiAppend)(LPCSTR szSubKey, long dwData);

	STDMETHOD_(void, SetBaseData) (LPCSTR szKey, LPCSTR  szValue) ;
	STDMETHOD_(void, GetBaseData) (LPCSTR szKey, CString &rValue) ;

//->@Solomon���� �ش�.
	STDMETHOD_(void, SetSendKey) (HWND hWinix, HWND hDrfnWrap);
	STDMETHOD_(void, SetGwanInterface) (long pInterface);
//<--@Solomon���� �ش�.

	STDMETHOD_(int, LoadAccountAfter)( CTreeCtrl* pTreeCtrl,HTREEITEM hParentTree );

public:
	CTreeListManager();
	~CTreeListManager();
	void ClearList(LIST_CBaseLongData* pList);
	void ClearAll();
	
	BOOL LoadPCMASTER(LIST_CBaseLongData *pList, HTREEITEM hTree, LPCSTR szKey);
	BOOL LoadTreeConfig(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szSection, LPCSTR szFile, CListCtrl* pListCtrl=NULL);

	BOOL GetFileName(LPCSTR szKey, CString& szFile);
	int  LoadTreeConfig_Master(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, CString szMasterKey ,LPCSTR szTr ,LPCSTR szInputData);
	int	 LoadGwansimGroup     (CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR szInputData);
	int	 LoadGwansimGroup	  (CTreeResItemEx* pList, CListCtrl* pListCtrl, LPCSTR szTr ,LPCSTR szInputData);
	BOOL LoadOnlnyGwansimGroup(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR szInputData);
	
	int	 LoadAccountGroup	  (CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR szInputData);
	void GetAccountGroupCode  (char* buf);
	void GetFOAccountGroupCode(char* buf);
	void SendTr_AccountInfo   (CString strGroupKey);
	void RevData_AccountInfo  (char* buf, long len);
	
//	int  LoadJMASTER(CTreeCtrl* pTree, HTREEITEM hParentTree);
//	int  LoadKMASTER(CTreeCtrl* pTree, HTREEITEM hParentTree);
	int  LoadTree_CUMaster (CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int  LoadTree_CKUMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int  LoadTree_CKRXUMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR _szInputData, long dwKey1);
	int  LoadTree_CKNXUMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR _szInputData, long dwKey1);
	int  LoadTree_CTHMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int  LoadTree_CGMaster (CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int  LoadTree_CELWIndexMaster (CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int  LoadTree_CELWIssueMaster (CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int  LoadTree_CUserMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR _szInputData, long dwKey1);
	int	 LoadTree_JFMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int	 LoadTree_JPMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int  LoadTree_PMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int  LoadTree_FolderMaster(int nKind, CString strKind, CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr, LPCSTR szInputData, long dwKey1);

	void RequestDataOfOneClick(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved=0);
	void RequestData_A_MASTER(CItemLongData* pILData, ITLMCallback* pCallback, char aDataType, long dwReserved);
	void RequestData_B_MASTER(CItemLongData* pILData, ITLMCallback* pCallback, char aDataType, long dwReserved);
	void RequestData_A_Tr(CItemLongData* pILData, ITLMCallback* pCallback, char aDataType, long dwReserved);
	void RequestData_B_Tr(CItemLongData* pILData, ITLMCallback* pCallback, char aDataType, long dwReserved);

	// �����ڵ� ó�� �� ������ ������ �����ϴ� ���
	void RequestData_A_SUBMASTER(CItemLongData* pILData, ITLMCallback* pCallback, char aDataType, long dwReserved);
	void RequestData_B_SUBMASTER(CItemLongData* pILData, ITLMCallback* pCallback, char aDataType, long dwReserved);

	void RequestData_TR       (CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved);
	void RequestData_TR_Group (CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved);
	void RequestData_HistoryTR(CItemLongData* pILData, ITLMCallback* pCallback, char aDataType, char cSort, long dwReserved);
//	void RequestData_Thema_Group(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved);
//	void RequestData_Thema_Item(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved);

	// Type='A'�̸鼭 ���������Ͽ��� �о�� �ϴ� ��츦    ���� ��ƾ...
//	void Callback_Master_J_K(LIST_CCodeTypeA* pListTypeA);
	void Callback_KSMaster(LPCSTR szMasterKey, LIST_CCodeTypeA* pListTypeA, CString &szInputData);	
	void Callback_CJMaster(LIST_CCodeTypeA* pListTypeA);
	void Callback_CKMaster(LIST_CCodeTypeA* pListTypeA);
	void Callback_CUMaster(LIST_CCodeTypeA* pListTypeA);
	void Callback_CTMaster(LIST_CCodeTypeA* pListTypeA);
	void Callback_CKUMaster(LIST_CCodeTypeA* pListTypeA);
	void Callback_CETFMaster(LIST_CCodeTypeA* pListTypeA);
	void Callback_CELWMaster(LIST_CCodeTypeA* pListTypeA);
	void Callback_NONE(LIST_CCodeTypeA* pListTypeA);
	
	void CallHistory(LIST_CCodeTypeA* pListTypeA);
	void GetGwanGroupCode(CString szGroupName, LIST_TR_CODEINFO* pCodeList);
	LIST_TR_CODEINFO* GetImsiGroupCode(CString szGroupName);

	long GetBitFromCodeInfo(CString& szWonjoCodeChoice);
	int  FP_strlistHistory(CString strHistory, CStringList &strList);

	int RequestData_GroupMASTER(CItemLongData* pILData, ITLMCallback* pCallback, char aDataType, long dwReserved, char cSort);

public:
	class CTrCommSite : public ITrCommSite
	{
	public:
		STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
		STDMETHOD(ReceiveRealData) (WORD wFID, LPCSTR szMainKey, void* pRealData);
		STDMETHOD(StatusChange) (int nState);
		STDMETHOD(ReceiveMessage) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage);

	public:
		CTreeListManager* m_pTLMParent;
	} m_xTrCommSite;
	friend class CTreeListManager;


protected:
	BOOL CheckAllListt(POSITION comPos);
	CItemLongData m_clItem;//account ȯ������ ���� tour2k
public:
	CString m_szConfigFile;
	IGateManager*		m_pGeteMng;
	IMainInfoManager*	m_pMainInfoMng;
	//IMasterDataManager* m_pMDMng;
	IMasterDataManagerLast* m_pMDMng;
	IAUTrCommManager*	m_pTrCommMng;
//	ITrComm*			m_pTrComm;

	CTreeCtrl*			m_pTreeCtrl;
	CListCtrl*			m_pListCtrl;
	// LIST_LIST_CItemLongData	m_AllList;
	LIST_CTreeResItemEx	m_AllList;
//	LIST_CItemLongData	m_List;
	LIST_CPacketInfo	m_PacketList;


	void SetIGateManager(long);

	// ���ɱ׷��� ����Ǿ��� �� �Ҹ��� �Լ�.
	HTREEITEM hGwanTree;		// ���ɱ׷��� HTREEITEM���� �����Ѵ�.
	// void ReLoadGwanGroup(long dwKey, long dwSubKey, long dwNotifyData);
	void DeleteTreeChildInfo(CTreeCtrl* pTree, HTREEITEM hParentTree);

	CString m_strReserved;
	//�׸� ��ȸ
	//void	Send076220Rq();
	//void	Send076210Rq(CString strKey);
//	int		GetThemaGroup();
//	HRESULT ReceiveDataThemaGroup(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
//	HRESULT ReceiveDataThemaItem(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
//	CMapStringToPtr m_ThemaTrIndex;

	//{{��Ŭ���߰�..
	CCondHelper m_LibCondNng;
	void LoadConditonMng(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree = NULL);
	void LoadConditonMng2(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree = NULL);	
	void LoadConditonMngUser(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree = NULL);	

	HRESULT ReceiveDataOneClick_Change(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
	HRESULT ReceiveDataOneClick(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);

	HRESULT SetReceiveData_NULL();
	HRESULT SetReceiveData_Result(LPTSTR pData, DWORD dwDataLen);
	BOOL m_bLoadCondMng;
	//}}

//2007.10.05�߰� Junok Lee
public:
	CMapStringToString	m_DataTable;
	HWND m_hWinixWnd, m_hWrapper;	//For Solomon
	long m_pWinixGwanInterface;

	//���Ͽ��� �о������ �ϴ� ��ƾ.
	void _DoAlzioSerialize(CArchive& ar,
		CStringArray* parCode, CStringArray* parName, CStringArray* parEngName);

	CPacketInfo m_CurPackInfo;

	ACCOUNT			m_accn;			// �� ���¸�� hold

	static void CALLBACK fnWinixCommCallbackProc(int nKey, long pThisPoint,long dwUserKey, int nLen, LPBYTE pData);
};

/*******************************************************************************
* �ֽ� �ܰ��� ��ȸ IO FORMAT
******************************************************************************/
struct  s_mid   {
    char    accn[11];
    char    pswd[8];
    char    cod2[12];
    char    allf[1];    /* 0.�ſ����� 1.All */
    char    fill[10];
};

struct  s_rec   {
    char    cod2[12];   /* �����ڵ�         */
    char    hnam[40];   /* �����           */
    char    jggb[2];    /* �ܰ��ڵ�         */
    char    jqty[10];   /* �ܰ����         */
    char    xqty[10];   /* �ŵ�����         */
    char    pamt[10];   /* ������հ�       */
    char    mamt[15];   /* ���Աݾ�         */
	
    char    curr[10];   /* ���簡     (*)   */
	char    diff[10];   /* ���ϴ��         */
    char    rate[10];   /* �����           */
    char    camt[15];   /* �򰡱ݾ�         */
    char    tamt[15];   /* �򰡼���   (*)   */
    char    srat[10];   /* �򰡼��ͷ� (*)   */
    char    sycd[2];    /* �ſ��ڵ�         */
    char    sydt[8];    /* ������           */
    char    samt[15];   /* �ſ�ݾ�         */
}; 
struct  s_mod   {
    char    accn[11];
    char    nrec[4];
    struct  s_rec   rec[100];
}; 

#define L_smid	sizeof(struct s_mid)				// Length of stock input

/*******************************************************************************
* ����/�ɼ� �ܰ��� ��ȸ IO FORMAT
******************************************************************************/
struct	f_mid	{		// ���� Input
	char	accn[11];
	char	pswd[8];
	char	date[8];
	char	cod2[8];
};

struct	f_rec	{
	char	cod2[8];	/* �����ڵ�		*/
	char	hnam[30];	/* �����		*/
	char	dsgb[6];	/* ����			*/
	
	char	bqty[10];	/* ����			*/
	char	cqty[10];	/* �ŵ����ɼ���		*/
	char	pprc[10];	/* ��հ�/���갡	*/
	char	curr[10];	/* ���簡		*/
	char	diff[10];	/* ��հ� ���		*/
	char	pamt[15];	/* �򰡱ݾ�		*/
	char	psam[15];	/* �򰡼���		*/
	char	prat[10];	/* ���ͷ�		*/
	char	tamt[15];	/* ���Աݾ�		*/
};

struct	f_mod	{
	char	accn[11];
	char	nrec[4];
	struct	f_rec	rec[100];
};

#define L_fmid	sizeof(struct f_mid)				// Length of Future input

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_AGENTTREELISTMANAGER_IMPL_DLDLDEE_H__AF_AD91_7EE99FC62AAF__INCLUDED_)
