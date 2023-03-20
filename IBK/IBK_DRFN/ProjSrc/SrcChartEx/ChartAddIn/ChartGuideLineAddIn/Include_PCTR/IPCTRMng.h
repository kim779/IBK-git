#if !defined(AFX_PCTRMANAGER_OF_GDI_H__320E_14_BY_JUNOK_LEE_B1A__INCLUDED_)
#define AFX_PCTRMANAGER_OF_GDI_H__320E_14_BY_JUNOK_LEE_B1A__INCLUDED_

// #include "../../_Include/IPCTRMng.h"
#include "IBaseDefine.h"

//  int nID = 1;
// 	IPCTRManager* pPcTrMng = (IPCTRManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IPCTRManager* pMng = (IPCTRManager*)AfxGetPctrInterface(nID);


/*
	PCTrData�� ���ڿ������͸��� ó���Ѵ�.
*/

//const UINT RMSG_GETPCTRDATA = ::RegisterWindowMessage("RMSG_GETPCTRDATA");
//const UINT RMSG_SETPCTRDATA = ::RegisterWindowMessage("RMSG_SETPCTRDATA");
//IBaseDefine.h�� ���ǵǾ�����.
// 
// ���� : ������ �����Ϳ� �������� ������ ���� ID�� �ο��Ѵ�.
// IPCTRSite�� ������ ���� �����͸� �޴� Client�κ��̸�
// IPCTRManager�� ������ ���� �����͸� �����ϴ� Server�κ��̴�.
//
// const UINT RMSG_PCTRGETDATA = ::RegisterWindowMessage("RMSG_PCTRGETDATA");
// ���ϰ��� �޾Ƽ� �ٷ� ó���ؾ� �� ���
//
// const UINT RMSG_PCTRSETDATA = ::RegisterWindowMessage("RMSG_PCTRSETDATA");
// ����� IPCTRSite���� ó���ϰ��� �� �� ȣ��
//
// ������  : ���ؿ� 2005.11.02(��)
// History : 2005.11.02 �������̽� ����
//

//	RMSG_GETPCTRDATA 
// WPARAM wp;
// LPARAM lp;
// wp : 'A':�⺻Ÿ��, 'E':����TR���� Ȯ��Ÿ
//	1 : 'A'�ϰ�� IMainInfoManager���� �����ϴ� ������ ����.
//		 lp = int nID; ��.
//	2 : 'E'
//		 lp = STPCTR_GETDATAEX*
//		return���� STPCTR_GETDATAEX*

//	RMSG_SETPCTRDATA 
// WPARAM wp;
// LPARAM lp;
// wp : 'A':�⺻Ÿ��, 'E':����TR���� Ȯ��Ÿ
//	1 : 'A'�ϰ�� IMainInfoManager���� �����ϴ� ������ ����.
//		 lp = int nID; ��.
//	2 : 'E' �������� ����.


// ID�� �ش��ϴ� ����Ÿ�� �ٲ���� ��� �� �ٲ������� �ް��� �� ��츦 ���� ����
DECLARE_INTERFACE(IPCTRSite)
{
	STDMETHOD_(void, DataReceive)(long dwKey, long dwData)PURE;
};

DECLARE_INTERFACE(IPCTRManager)
{
	// ---------------------------------------------------------------------
	// I-PARAM : 
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(LPCSTR, GetData)(int nID)PURE;

	// ---------------------------------------------------------------------
	// I-PARAM : 
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(BOOL, SetData)(int nID, LPCSTR szNewData)PURE;

	// ---------------------------------------------------------------------
	// PCTR
	// I-PARAM : 
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(COleVariant*, GetDataEx)(int nID)PURE;

	// ---------------------------------------------------------------------
	// I-PARAM : 
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(BOOL, SetDataEx)(int nID, COleVariant* pNewData)PURE;

	// ---------------------------------------------------------------------
	// PCTR
	// I-PARAM : 
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(COleVariant*, GetDataEx)(LPCSTR szKey)PURE;

	// ---------------------------------------------------------------------
	// I-PARAM : 
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(BOOL, SetDataEx)(LPCSTR szKey, COleVariant* pNewData)PURE;

	// ---------------------------------------------------------------------
	// PCTR
	// I-PARAM : 
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(LONG, GetTrData)(char szTR, int nCount, LPCSTR szData, long dwKeyPURE, LPCSTR szKey=NULL)PURE;

	// ---------------------------------------------------------------------
	// I-PARAM : nKey, IPCTRSite*
	// O-PARAM : Advise�� Key��. Key���� Unadvise�� �� �ʿ��� ����.
	// ---------------------------------------------------------------------
	STDMETHOD_(LONG, Advise)(int nKey, IPCTRSite* pSite)PURE;

	// ---------------------------------------------------------------------
	// I-PARAM : pKey : Advise�ÿ� �Ѱܹ��� Ű
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(void, UnAdvise)(LONG pKey)PURE;

	// ---------------------------------------------------------------------
	// I-PARAM : nKey, IPCTRSite*
	// O-PARAM : Advise�� Key��. Key���� Unadvise�� �� �ʿ��� ����.
	// ---------------------------------------------------------------------
	STDMETHOD_(LONG, AdviseEx)(int nKey, IPCTRSite* pSite)PURE;

	// ---------------------------------------------------------------------
	// I-PARAM : pKey : Advise�ÿ� �Ѱܹ��� Ű
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(void, UnAdviseEx)(LONG pKey)PURE;

};


#endif //AFX_PCTRMANAGER_OF_GDI_H__320E_14_BY_JUNOK_LEE_B1A__INCLUDED_
