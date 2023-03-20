#if !defined(AFX_INTERFACE_OF_TRCOMM_ACC8_9_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_TRCOMM_ACC8_9_BY_JUNOK_LEE_8B1A__INCLUDED_

// #include "../../_Include/IAUTrCommMng.h"
#include "IBaseDefine.h"

//  int nID = 2;
// 	IAUTrCommManager* pTrCommMng = (IAUTrCommManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUTrCommManager* pMng = (IAUTrCommManager*)AfxGetPctrInterface(nID);

// 개념 : 
// ITrCommSite를 구현할 곳은 데이터를 받는 Client부분이며
// ITrCommManager를 구현할 곳은 데이터를 제공하는 Server부분이다.
//
// 만든이  : 이준옥 2005.11.02(수)
// History : 2005.11.02 인터페이스 정의
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

// IAUTrCommManager를 통해 ITrComm 인터페이스를 구하고
// ITrComm를 통해서 기존에 사용하던 CTrComm의 기능을 사용한다.
// ITrComm인터페이스는 기존 CTrComm을 감싼 클래스임.
DECLARE_INTERFACE(ITrComm)
{
	// 콜백함수를 바꾸려고 할 때 사용
	// I-PARAM : 새로운 ITrCommSite*
	// O-PARAM : 기존에 등록된 ITrCommSite*.
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
	// TR타입이 TYPE400이 아닐 경우도 가능.
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(BOOL, RealSend)(HWND hWnd, LPVOID pData, int nLen, DWORD dwType) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, ResetRealTR)(LPCSTR szTR) PURE;
	STDMETHOD_(void, SetTR)(LPCSTR szTR, BOOL bCert = FALSE) PURE;
	STDMETHOD_(void, SetTRCondition)(LPCSTR szServiceCode, LPCSTR szTR, WORD wKind = 3, WORD wPattern =0, BOOL bCert = FALSE) PURE;

	// 메인의 소켓핸들을 세팅한다.
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
	// 생성된 개별 TRComm 과 관련된 부분. end
	// ---------------------------------------------------------------

	// TR형태 AP조회
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
	// 상품선물옵션차트 - ojtaso (20071001)
	STDMETHOD_(BOOL, GetFidInfo)(WORD wFid, WORD wMarket, int &iType, int &iLength,int &iExp) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	// 다국어 적용 - ojtaso (20080128)
	STDMETHOD_(CString, GetLanguage)() PURE;

	// (2008/9/9 - Seung-Won, Bae) Use pointer without CString.
	STDMETHOD_( const char *, ApplyByteOrder2)(WORD wDataType, char *pBuf, int nLen,  int *pIdx, int nDotPos=0, BOOL bFirstZero=0) PURE;
};

DECLARE_INTERFACE(IAUTrCommManager)
{
	// ITrComm인터페이스 구하기(Advise)
	STDMETHOD_(ITrComm*, AdviseTrComm)() PURE;

	// ITrComm인터페이스 삭제(UnAdvise)
	STDMETHOD_(void, UnAdviseTrComm)(ITrComm*) PURE;

	// 메인이 디폴트로 생성한 소켓의 핸들.
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
 메인이 초기에 소켓의 핸들을 세팅해줘야 한다.
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
