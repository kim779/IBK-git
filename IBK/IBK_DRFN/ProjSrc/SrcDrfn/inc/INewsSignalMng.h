#if !defined(AFX_INTERFACE_OF_NEWSSIGNALMNG_H__320E_1464_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_NEWSSIGNALMNG_H__320E_1464_BY_JUNOK_LEE_8B1A__INCLUDED_

// ������  : ���ؿ� 2008.04.18��
// History : 2008.04.18�� �������̽� ����

// #include "../../_Include/INewsSignal.h"
//#include "./ISignalMng.h"

//#include <AFXTEMPL.H>
#include "IBaseDefine.h"
#define UUID_INewsSignalManager			45

//	ISignalManager* pSigMng = (ISignalManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	ISignalManager* pSigMng = (ISignalManager*)AfxGetPctrInterface(UUID_INewsSignalManager);

// ������  : ���ؿ� 2006.01.02(��)
// History : 2006.01.02 �������̽� ����
//

//typedef  int  (WINAPI* FpSetLibOpen)(CWnd*);
//typedef  void (WINAPI* FpSetLibClose)();
//typedef  void (WINAPI* FpGetSignalData)(CString strType, CMapStringToPtr &mapSignalSet) ;
//typedef  void (WINAPI* FpSetSignalData)(CString strType, CMapStringToPtr *pmapSignalSet) ;
//typedef  void (WINAPI* FpGetFilterData)(long& stFilterInfo) ;
//typedef  void (WINAPI* FpSetFilterData)(long* pstFilterInfo) ;
//typedef  int  (WINAPI* FpConvertRealData)(int nPacketName, LPSTR &sRealData) ;
//
//DECLARE_INTERFACE(ISignalManager)
//{
//	//# DRDS���� Signal DLL���ε��� �� �����ϴ� ���
//	STDMETHOD_(BOOL, SetLibHandle)(HMODULE) PURE;
//
//	//# Open / Close! => �ݵ�� Open �� Colse!
//	STDMETHOD_(int, _DLL_SetLibOpen)(CWnd* pwndMain) PURE;
//	STDMETHOD_(void, _DLL_SetLibClose)() PURE;
//
//	//# ��ȣ ���� ���� �Լ�!
//	STDMETHOD_(void, _DLL_GetSignalData)(CString strType, CMapStringToPtr &mapSignalSet) PURE;
//	STDMETHOD_(void, _DLL_SetSignalData)(CString strType, CMapStringToPtr *pmapSignalSet) PURE;
//
//	STDMETHOD_(void, _DLL_GetFilterData)(long& stFilterInfo) PURE;
//	STDMETHOD_(void, _DLL_SetFilterData)(long* pstFilterInfo) PURE;
//
//	//# DRDS �ܺ� �Լ�
//	STDMETHOD_(int, ConvertRealData)(int nPacketName, LPSTR &sRealData) PURE;
//
//};

DECLARE_INTERFACE(INewsSignalManagerSite)
{
};

DECLARE_INTERFACE(INewsSignalManager)
{
//	STDMETHOD_(BOOL, IsNewsSignalLoaded)() PURE;

	// ���� Advise�� ī��Ʈ ����.
	//cType :	A -> �⺻ Ÿ��.
	//			E -> ���� �Ҹ���ġ ����Type;
	STDMETHOD_(LONG, AdviseNewsSignal)(char cType, LONG dwTypeValue) PURE;

	// UnAdvise�� Advise�Ǿ��ִ� ī��Ʈ ����.
	STDMETHOD_(BOOL, UnAdviseNewsSignal)(LONG dwAdviseKey) PURE;

	STDMETHOD_(int, GetAdviseCount)() PURE;

	// bAdvise ��ȣ�� ���� ������(1), ��������(0)
	STDMETHOD_(int, SetNewsSignal)(BOOL bAdvise, CWnd* pScreenWnd, CString strType, CString strKey) PURE;

	//SetSiganl��� SetNewsSignal����Ѵ�.
	//STDMETHOD_(int,  SetSiganl)(BOOL bAdvise, CWnd* pwndMain, CString strType, CString strKey) PURE;

	STDMETHOD_(void, GetSignalData)(CString strType, CMapStringToPtr &mapSignalSet) PURE;

	STDMETHOD_(void, GetFilterData)(long &stFilterInfo) PURE;
	
	STDMETHOD_(void, SetSignalData)(CString strType, CMapStringToPtr *pmapSignalSet) PURE;

	STDMETHOD_(void, SetFilterData)(long *pstFilterInfo) PURE;


	STDMETHOD_(int,  GetSiganlTable)(CString strType, CStringArray &saSignalTable ) PURE;

	STDMETHOD_(HWND, GetDummyHwnd)() PURE;
};

#define WM_GET_BROD				(WM_USER+605)

#endif //AFX_INTERFACE_OF_NEWSSIGNALMNG_H__320E_1464_BY_JUNOK_LEE_8B1A__INCLUDED_
