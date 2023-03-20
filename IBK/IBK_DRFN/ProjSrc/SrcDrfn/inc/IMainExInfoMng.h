#if !defined(AFX_INTERFACE_OF_MAINEXINFO_1464_45B_BY_JUNOK_LEE_INCLUDED_)
#define AFX_INTERFACE_OF_MAINEXINFO_1464_45B_BY_JUNOK_LEE_INCLUDED_

// #include "../../_include/IMainExInfoMng.h"
#include "IBaseDefine.h"
//  int nID = 22;
// 	IMainExInfoManager* pMainExMng = (IMainExInfoManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IMainExInfoManager* pMng = (IMainExInfoManager*)AfxGetPctrInterface(nID);
//	#define		UUID_IMainExInfoManager				22
//	#define		UUID_IMainExInfoManager2			22

//#define UUID_IMainExInfoManager		22
#define UUID_IMainExInfoManager2		22
#define UUID_IMainExInfoManager3		22
#define IMainExInfoManagerLast			IMainExInfoManager3

#define MAX_IMainExInfoManager			41

//  0 - 30 : ����� ����.
//	0	�����ڵ�	long	O	O	����
//	1	CodeOcx������ ������	long	O	O	����
//	2	Signal.dll Lib�ڵ�(HMODULE)	long	O	O	DRDS
//	3	���������� �ڵ�	long	O	O	����
//	4	���� AFX_MODULE_STATE*	long	O	O	����
//	5		long	O	O	����
//	6		long	O	O	����
//	7					���ο���
//	8					���ο���
//	9					���ο���
//	10					���ο���
//	11					���ο���
//	12					���ο���
//	13					���ο���
//	14					���ο���
//	15					���ο���
//  16		long		���ɿ���
//  17		long		�ӽñ׷쿹��

#define IMainExInfoManager_Socket		1
#define IMainExInfoManager_CodeOcx		2
#define IMainExInfoManager_Gwansim		16
#define IMainExInfoManager_ImsiGroup	17


// ���� �� ������ ����ϰ� �ɰ�� ������ �÷��� ����ϼ���^^
// ������ �̷��� ���� �̸� ������ ��Ƴ���.
// #define		MAX_MAINEXDISP				30
#define		MAX_MAINEXDISP				1

DECLARE_INTERFACE_(IMainExInfoManager, IBaseDataDB)
{
	// {{ DISPID�� ������ ��ƾ. �ӵ� ������ ���ؼ� ����.
	// ��𿡼��ΰ� �� �� �� ��ƾ�� �ݵǾ����� ó���ǵ��� �ϱ� ���� ��ƾ��.
	// �� ��ƾ�� ���� ���������� ����. ���� �ӵ������Ͽ� ������ ��� �����ϱ� �ٶ�.
	STDMETHOD_(LONG, GetDispid)(int nID) PURE;
	STDMETHOD_(void, SetDispid)(INT nID) PURE;
	// }}
};

// ����ڰ� �ʿ��� �����͸� �����ϰ� �����ϰ� �����ϴ� ����� ���ؼ� �����ϴ� �������̽�
// ���ÿ� �ߺ����� �ʵ��� IsGetUserExData(..)�� ���ؼ� FALSE�϶� ����ϼ���.
// ����Ŀ��� ������ ����� �޸𸮿��� ������ϴ�.
DECLARE_INTERFACE_(IMainExInfoManager2, IMainExInfoManager)
{
	STDMETHOD_(LONG, GetUserExData)(LONG nID) PURE;
	STDMETHOD_(BOOL, SetUserExData)(LONG nID, LONG dwValue) PURE;
	STDMETHOD_(BOOL, DeleteUserExData)(LONG nID) PURE;
	STDMETHOD_(BOOL, IsGetUserExData)(LONG nID) PURE;
};

DECLARE_INTERFACE_(IMainExInfoManager3, IMainExInfoManager2)
{
	STDMETHOD_(BOOL, GetUserStringData)(LPCSTR szKey, CString& rValue) PURE;
	STDMETHOD_(BOOL, SetUserStringData)(LPCSTR szKey, LPSTR szData) PURE;

	STDMETHOD_(POSITION, GetUserData)(LPCSTR szKey, LPBYTE *pData, int &nDataLen) PURE;
	STDMETHOD_(BOOL, SetUserData)(LPCSTR szKey, long pData, int nDataLen) PURE;
};

#define		IMRIM_ROOTDIR			"A12000"
#define		IMRIM_DATADIR			"A12002"
#define		IMRIM_ENCUSERDIR		"A12007"

#define		gszCacheDir				"drcache"
#define		gszDataKey_WorkDate		"WorkDate"
#define		gszDataKey_USDollar		"USDollar"
#define		gszDataKey_MstCode		"MstCode"

#endif //AFX_INTERFACE_OF_MAINEXINFO_1464_45B_BY_JUNOK_LEE_INCLUDED_
