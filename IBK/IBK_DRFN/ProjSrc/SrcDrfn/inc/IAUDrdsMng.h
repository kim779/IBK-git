#if !defined(AFX_HANARO_IAUDRDSMANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_HANARO_IAUDRDSMANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_

// #include "../../_Include/IAUDrdsMng.h"
#include "IBaseDefine.h"
// ������ ����ϴ� CDrdsLoadLib�� �״�� Matching ��Ų��.
//
// ������  : ���ؿ� 2005.11.14(��)
// History : 2005.11.14 �������̽�
//
//	int nID = 19;
// 	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUDrdsManager* pMng = (IAUDrdsManager*)AfxGetPctrInterface(nID);
//
//  _Include/ExDialog.h ���Ͽ� 
//	AFX_INLINE IAUDrdsManager*	GetDrdsManager();
//	// �ǽð� ó�� DRDS�� m_pIDrdsLib
//	AFX_INLINE IDrdsLib* GetDrdsLib();
//  ���� 3���� ���ǵǾ� �����Ƿ� ���� �������̽��� ������ ����
//  GetDrdsLib()�� ȣ���ؼ� ����ϱ� �ٶ��ϴ�.
//  ��
//  GetDrdsLib()->DrdsInit(..);
//  ����ÿ�  GetDrdsLib()->DrdsTerm(..);
//
//
// ��ü������ �����ؼ� �����ϸ� �Ʒ��� ����.
//  Step1 : ������ ��
//	IDrdsLib* pDrds = pDrdsMng->AdviseDrdsLib();
//	pDrds->DrdsInit(..);
//  
//  Step6 : ������ ��
//	pDrds->DrdsTerm(..);
//	pDrdsMng->UnadviseDrdsLib(pDrds);
//
//  Step2 : �ǽð������ ��
//	pDrds->DrdsConnect(..);

//  Step3 : Advise
//	pDrds->DrdsAdvise(..);
//
//  Step4 : Unadvise
//	pDrds->DrdsUnadvise(..);
//
//	Step5 : �ǽð������� ��
//	pDrds->DrdsClose(..);
//
//  [��밭�±ǰ����]
//  Step3�� Step4�� Step2�� �θ��� Step5�� �θ��������� �ݺ��Ѵ�.
//  ���̻� ������� ���� ��쿡�� Step5�� �θ��� ����������.
//
//  ���� CDrdsInfo��� Ŭ���������� Connect�� Advise
//  Unadivse�� DisClose�� �θ��� ���·� ����ߴµ� ��ȿ������ ����̹Ƿ�
//  [��밭�±ǰ����]�� �����ֱ� �ٶ��ϴ�.
//
//	��Ÿ
//	pDrds->DrdsSetGetBrodHwnd(..);
//	pDrds->DrdsSetSocketHandle(..);
//	pDrds->DrdsSendCode(..);

#ifndef __DRDSLOADLIB__DEFINE__
	#define __DRDSLOADLIB__DEFINE__
	#define WM_DRDS_DATA				WM_USER + 200
	#define WM_DRDS_DATA_PACKET_END		WM_USER + 202
	#define MAX_DRDS_PACKET_LEN			4096

	typedef struct  
	{
	 char Name[32];
	 char Field[32];
	 char Key[32];
	 UINT Id;
	 int  Len;
	 char Data[MAX_DRDS_PACKET_LEN];
	} DrdsData;
	#define		SIZE_DrdsData		sizeof(DrdsData)
#endif // __DRDSLOADLIB__DEFINE__



// IDrdsLib
// ������ ����ϴ� DRDS��ƾ�� �״�� �Ѵ�. (����DRDS������ ȣȯ�� ���ؼ�..)
// ���ÿ� IAUDrdsManager�������̽��� ���� AdviseDrdsLib/UnadviseDrdsLib�� ����� �Ѵ�.
// �� ����Ŀ��� IAUDrdsManager�������̽��� UnadviseDrdsLib�� ȣ������� �Ѵ�.
// ������ Library�� Load/UnLoad�ϴ� ���̶�� ����ȴ�.


DECLARE_INTERFACE(IDrdsLib)
{
	// 
	// I-PARAM :
	// O-PARAM : 
	// Def 1-5
	STDMETHOD_(void*, DrdsInit)(HWND hWnd) PURE;
	STDMETHOD_(BOOL, DrdsTerm)() PURE;
	STDMETHOD_(BOOL, DrdsConnect)(LPSTR szDataName) PURE;
	STDMETHOD_(BOOL, DrdsClose)(LPSTR szDataName) PURE;
	STDMETHOD_(BOOL, DrdsAdvise)(UINT id, LPSTR szDataName, LPSTR FieldName, LPSTR KeyName) PURE;

	STDMETHOD_(BOOL, DrdsUnadvise)(UINT id, LPSTR szDataName, LPSTR FieldName, LPSTR KeyName) PURE;
	STDMETHOD_(BOOL, DrdsSetSocketHandle)(HWND hWnd) PURE;
	STDMETHOD_(void, DrdsSendCode)() PURE;
	
	STDMETHOD_(BOOL, AdviseRealCodeData)(BOOL bIsAdd, int nCount, LPSTR sPacketName, LPSTR sCodeList) PURE;
	STDMETHOD_(int,  SetNewRealData)(LPSTR pPacket, int nPacketLen, int nComponentType, int nBrocastType) PURE;
	STDMETHOD_(BOOL, DrdsAdviseEx)(int nComponentType, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName, LPSTR ObjectName) PURE;
	STDMETHOD_(BOOL, DrdsUnadviseEx)(int nComponentType, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName, LPSTR ObjectName) PURE;
	STDMETHOD_(BOOL, DrdsRequest)(void *Link, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName) PURE;
	STDMETHOD_(void, DrdsPropertySet)() PURE;
	STDMETHOD_(void, DrdsTraceOn)() PURE;
	STDMETHOD_(void, DrdsTraceOff)() PURE;
	STDMETHOD_(void, DrdsSetFlag)(bool bTrace) PURE;
	STDMETHOD_(void, UDPCreate)() PURE;
	STDMETHOD_(void, UDPCreate2)(LPSTR strIP, LPSTR strPort) PURE;
	STDMETHOD_(void, UDPDestroy)() PURE;
	STDMETHOD_(void, UdpCreate3)(LPSTR sCfgFullPathName) PURE;
	STDMETHOD_(int , SetUserDefData)(LPSTR sKeyName, LPSTR sData) PURE;


	STDMETHOD_(BOOL, Drds_OpenSignalMng)(LPSTR sCfgFullPathName,long lMainHwnd) PURE;
	STDMETHOD_(int,  Drds_SignalMng_SetNewRealData)(LPSTR pPacket, int nPacketLen, int nComponentType, int nBrocastType) PURE;
	STDMETHOD_(long, Drds_GetInterface)(UINT nID) PURE;
	STDMETHOD_(BOOL, Drds_SetInterface)(UINT nID, long dwData) PURE;
	STDMETHOD_(int, ConvertToRealStruct)(WPARAM wParam, LPARAM lParam, char* pStruct, int nRealType=-1) PURE;

	STDMETHOD_(BOOL, GetRealStructData)(WPARAM wParam, LPARAM lParam, char*& prValue, int &nRealTpe) PURE;
};


// ���ο��� �����ϴ� ���θ��ҽ��Ŵ���.
// �����Ӹ��� �޸� �����ؾ� �ϴ� ��찡 �ֱ⶧���� ����.
DECLARE_INTERFACE(IAUDrdsManager)
{
	// AdviseDrdsManager, UnAdviseDrdsManager �� �� Call.
	STDMETHOD_(IDrdsLib*, AdviseDrdsLib)() PURE;
	STDMETHOD_(void, UnadviseDrdsLib)(IDrdsLib*) PURE;
};


#endif //AFX_HANARO_IAUDRDSMANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_
