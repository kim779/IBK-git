#if !defined(AFX_INTERFACE_OF_TRCOMM_ACC8_9_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_TRCOMM_ACC8_9_BY_JUNOK_LEE_8B1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #include "../../_Include/ITrCommMng.h"
#include "IBaseDefine.h"

//  int nID = 2;
// 	IAUTrCommManager* pTrCommMng = (IAUTrCommManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);

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
	STDMETHOD(RecieveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, UINT dwTRDateLen) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
    STDMETHOD(StatusChange) (int nState) PURE;
};
typedef  ITrCommSite FAR * LPITrCommSite;

DECLARE_INTERFACE(IAUTrCommManager)
{
	// ������ �����ڵ��� �����Ѵ�.
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, SetSocket)(HWND hSocket) PURE;

	// ������ ����Ʈ�� ������ ������ �ڵ�.
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(HWND, GetSocketHandle)(LPCSTR szType="MAIN") PURE;

	// ---------------------------------------------------------------
	// ������ ���� TRComm �� ���õ� �κ�. start
	// ---------------------------------------------------------------
	// ������ CTRComm�� ���ο��� ó�����ְ� ������, ������ CTRComm���� �������̽�
	// ���� TrComm Create.
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD(Advise)(ITrCommSite* pSite=NULL) PURE;

	// Unadvise�� ���ϸ� ������ ���� TrComm�� Destroy�ȴ�.
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD(Unadvise)() PURE;

	// �ݹ��Լ��� �ٲٷ��� �� �� ���
	// I-PARAM : ���ο� ITrCommSite*
	// O-PARAM : ������ ��ϵ� ITrCommSite*.
	STDMETHOD_(ITrCommSite*, ChangeCallback)(ITrCommSite* pSite) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(BOOL, Send2Server)(LPCSTR szData, int nLen, BOOL bWait=FALSE) PURE;

	// TRŸ���� TYPE400�� �ƴ� ��쵵 ����.
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(BOOL, RealSend)(HWND hWnd, LPVOID pData, int nLen, DWORD dwType) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, SetTR)(LPCSTR szTR, BOOL bCert = FALSE) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, SetTimeout)(int nValue) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, SetEnc)(BOOL bValue) PURE;
	// ---------------------------------------------------------------
	// ������ ���� TRComm �� ���õ� �κ�. end
	// ---------------------------------------------------------------
};

#endif // AFX_INTERFACE_OF_TRCOMM_ACC8_9_BY_JUNOK_LEE_8B1A__INCLUDED_
