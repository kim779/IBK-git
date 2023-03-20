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
	STDMETHOD_(BOOL, Send2ServerCondition)(LPCSTR szData, int nLen, BOOL bWait=FALSE) PURE;

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
	STDMETHOD_(void, SetTR)(LPCSTR szTR, BOOL bCert = FALSE) PURE;
	STDMETHOD_(void, SetTRCondition)(LPCSTR szTR, BOOL bCert = FALSE) PURE;

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
	STDMETHOD_(void, AddMainKey)(WORD wFID, LPCSTR strData, WORD wType, WORD wLen) PURE;

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
	STDMETHOD_(LPVOID, GetFidIndexData)	(int nIndex) PURE;
	STDMETHOD_(void, FidQueryKeyPartMake) ( CString &strQuery, char* pFidNum , char* pValue) PURE;
	
	STDMETHOD_(int,  FidArrayQueryMake)    ( CString &strQuery, CString strKeyList, CString strSaveBuf, CStringArray& arFidList ,  int nQueryIndex = 0, int nRequetCount = 20, int nRequestType = 0  ) PURE;
	STDMETHOD_(int,  FidSimpleQueryMake)   ( CString &strQuery, CString strKeyList, CStringArray& arFidList , int nQueryIndex = 0 ) PURE;
	STDMETHOD_(int,  FidPortPolioQueryMake)( CString &strQuery, int nType, int nGidNum, CStringArray& arKeyList, CStringArray& arFidList ,int nQueryIndex = 0 ) PURE;
	STDMETHOD_(void,  SetDestination)( char cDestination ) PURE;

	// ---------------------------------------------------------------
	// ������ ���� TRComm �� ���õ� �κ�. end
	// ---------------------------------------------------------------
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

