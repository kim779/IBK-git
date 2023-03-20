#if !defined(AFX_INTERFACE_OF_TRCOMM_ACC8_9_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_TRCOMM_ACC8_9_BY_JUNOK_LEE_8B1A__INCLUDED_

// #include "../../_Include/IAUTrCommMng.h"
#include "IBaseDefine.h"

//  int nID = 2;
// 	IAUTrCommManager* pTrCommMng = (IAUTrCommManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUTrCommManager* pMng = (IAUTrCommManager*)AfxGetPctrInterface(nID);

// ���� : 
// ITrCommSite�� ������ ���� �����͸� �޴� Client�κ��̸�
// ITrCommManager�� ������ ���� �����͸� �����ϴ� Server�κ��̴�.
//
// ������  : ���ؿ� 2005.11.02(��)
// History : 2005.11.02 �������̽� ����
//

DECLARE_INTERFACE(ITrCommSite)
{
	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen) PURE;
	STDMETHOD(ReceiveRealData) (WORD wFID, LPCSTR szMainKey, void* pRealData) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
    STDMETHOD(StatusChange) (int nState) PURE;
};
typedef  ITrCommSite FAR * LPITrCommSite;

// IAUTrCommManager�� ���� ITrComm �������̽��� ���ϰ�
// ITrComm�� ���ؼ� ������ ����ϴ� CTrComm�� ����� ����Ѵ�.
// ITrComm�������̽��� ���� CTrComm�� ���� Ŭ������.
DECLARE_INTERFACE(ITrComm)
{
	// �ݹ��Լ��� �ٲٷ��� �� �� ���
	// I-PARAM : ���ο� ITrCommSite*
	// O-PARAM : ������ ��ϵ� ITrCommSite*.
	STDMETHOD_(void, ChangeCallback)(ITrCommSite* pSite) PURE;
//, long dwKey=NULL
	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(BOOL, Send2Server)(LPCSTR szData, int nLen, BOOL bWait=FALSE) PURE;
	STDMETHOD_(BOOL, Send2ServerCondition)(LPCSTR szData = NULL, int nLen=0, BOOL bType = false, BOOL bWait=FALSE) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(BOOL, Send2Server)(void* szData, int nLen, BOOL bWait=FALSE) PURE;

//, long dwKey=NULL
	// TRŸ���� TYPE400�� �ƴ� ��쵵 ����.
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(BOOL, RealSend)(HWND hWnd, LPVOID pData, int nLen, DWORD dwType) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, ResetRealTR)(LPCSTR szTR) PURE;
	STDMETHOD_(void, SetTR)(LPCSTR szTR, BOOL bCert = FALSE) PURE;
	STDMETHOD_(void, SetTRCondition)(LPCSTR szServiceCode, LPCSTR szTR, WORD wKind = 3, WORD wPattern =0, BOOL bCert = FALSE) PURE;

	// ������ �����ڵ��� �����Ѵ�.
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, SetSocket)(HWND hSocket) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, SetTimeout)(int nValue) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, SetEnc)(BOOL bValue) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, AddMainKey)(WORD wFID, LPCSTR strData, WORD wType, WORD wLen, BOOL bClear=true) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, AddOptionKey)(WORD wFID, LPCSTR strData, WORD wType, WORD wLen, BYTE ucOpt) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, AddDataKey)(WORD wFID, LPCSTR strData="", WORD wType=0, WORD wLen=0) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(CString, ApplyByteOrder)(WORD wDataType, char *pBuf, int nLen,  int *pIdx, int nDotPos=0, BOOL bFirstZero=0) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, SetForceMid)(BYTE nFID) PURE;

	//005 {{
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, SetRealUpdateType)(WORD wType) PURE;
	//005 }}

	// ---------------------------------------------------------------
	// ������ ���� TRComm �� ���õ� �κ�. end
	// ---------------------------------------------------------------

	// TR���� AP��ȸ
	STDMETHOD_(int, Send2ServerServiceMgr)(LPCTSTR lpszServiceID, CStringArray* lpOrderData) PURE;
	STDMETHOD_(UINT, GetRealQAIndex)(long dwKey) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, AddSubKey)(WORD wFID, LPCSTR strData, WORD wType, WORD wLen) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, SetJoinCount)(int nCnt) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, SetForceMidArray)(CByteArray *pArrMid) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(HWND, GetHandle)() PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, SetSvcID)(CString strSvcID) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
	STDMETHOD_(BOOL, GetFidInfo)(WORD wFid, WORD wMarket, int &iType, int &iLength,int &iExp) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	// �ٱ��� ���� - ojtaso (20080128)
	STDMETHOD_(CString, GetLanguage)() PURE;

	// (2008/9/9 - Seung-Won, Bae) Use pointer without CString.
	STDMETHOD_( const char *, ApplyByteOrder2)(WORD wDataType, char *pBuf, int nLen,  int *pIdx, int nDotPos=0, BOOL bFirstZero=0) PURE;
};

DECLARE_INTERFACE(IAUTrCommManager)
{
	// ITrComm�������̽� ���ϱ�(Advise)
	STDMETHOD_(ITrComm*, AdviseTrComm)() PURE;

	// ITrComm�������̽� ����(UnAdvise)
	STDMETHOD_(void, UnAdviseTrComm)(ITrComm*) PURE;

	// ������ ����Ʈ�� ������ ������ �ڵ�.
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(HWND, GetSocketHandle)(LPCSTR szType="MAIN") PURE;
	STDMETHOD_(void, SetSocketHandle)(LPCSTR szType, HWND hSocket) PURE;
};

enum{
	C_DT_STRING=1	,
	C_DT_BYTE		,
	C_DT_SHORT	,
	C_DT_USHORT	,
	C_DT_LONG		,
	C_DT_ULONG	,
	C_DT_FLOAT	,
	C_DT_DOUBLE	,
	C_DT_LONGLONG	,
	C_DT_VAR		,
	C_DT_S9		,
	C_DT_U9
};
#endif // AFX_INTERFACE_OF_TRCOMM_ACC8_9_BY_JUNOK_LEE_8B1A__INCLUDED_

/*
 ������ �ʱ⿡ ������ �ڵ��� ��������� �Ѵ�.
#include "../../_Include/IAUTrCommMng.h"

BOOL CMainFrame::OnConnectServer()
{
	m_mapSocket["default"] = theApp.m_pSocketDlg->m_pSocket->m_hWnd;

	int nID = 2;
 	IAUTrCommManager* pMng = (IAUTrCommManager*)SendMessage(RMSG_INTERFACE, nID, 0);
	if(pMng)
	{
		pMng->SetSocketHandle("MAIN", theApp.m_pSocketDlg->m_pSocket->m_hWnd);
	}

	return TRUE;
}
*/
