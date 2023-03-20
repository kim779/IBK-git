#if !defined(AFX_INTERFACE_OF_SIGNALMNG_H__320E_1464_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_SIGNALMNG_H__320E_1464_BY_JUNOK_LEE_8B1A__INCLUDED_

// #include "../../_Include/ISignalMng.h"
#include <AFXTEMPL.H>
#include "IBaseDefine.h"

//	int nID = 10;
//	ISignalManager* pSigMng = (ISignalManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	ISignalManager* pSigMng = (ISignalManager*)AfxGetPctrInterface(nID);

// ������  : ���ؿ� 2006.01.02(��)
// History : 2006.01.02 �������̽� ����
//

typedef  int  (WINAPI* FpSetLibOpen)(CWnd*);
typedef  void (WINAPI* FpSetLibClose)();
typedef  void (WINAPI* FpGetSignalData)(CString strType, CMapStringToPtr &mapSignalSet) ;
typedef  void (WINAPI* FpSetSignalData)(CString strType, CMapStringToPtr *pmapSignalSet) ;
typedef  void (WINAPI* FpGetFilterData)(long& stFilterInfo) ;
typedef  void (WINAPI* FpSetFilterData)(long* pstFilterInfo) ;
typedef  int  (WINAPI* FpConvertRealData)(int nPacketName, LPSTR &sRealData) ;

DECLARE_INTERFACE(ISignalManager)
{
	//# DRDS���� Signal DLL���ε��� �� �����ϴ� ���
	STDMETHOD_(BOOL, SetLibHandle)(HMODULE) PURE;

	//# Open / Close! => �ݵ�� Open �� Colse!
	STDMETHOD_(int, _DLL_SetLibOpen)(CWnd* pwndMain) PURE;
	STDMETHOD_(void, _DLL_SetLibClose)() PURE;

	//# ��ȣ ���� ���� �Լ�!
	STDMETHOD_(void, _DLL_GetSignalData)(CString strType, CMapStringToPtr &mapSignalSet) PURE;
	STDMETHOD_(void, _DLL_SetSignalData)(CString strType, CMapStringToPtr *pmapSignalSet) PURE;

	STDMETHOD_(void, _DLL_GetFilterData)(long& stFilterInfo) PURE;
	STDMETHOD_(void, _DLL_SetFilterData)(long* pstFilterInfo) PURE;

	//# DRDS �ܺ� �Լ�
	STDMETHOD_(int, ConvertRealData)(int nPacketName, LPSTR &sRealData) PURE;

};


#endif //AFX_INTERFACE_OF_SIGNALMNG_H__320E_1464_BY_JUNOK_LEE_8B1A__INCLUDED_
