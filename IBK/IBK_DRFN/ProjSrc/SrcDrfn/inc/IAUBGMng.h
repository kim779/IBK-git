#if !defined(AFX_INTERFACE_OF_BACKGROUNDMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_BACKGROUNDMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_

// #include "../../_Include/IAUBGMng.h"
#include "IBaseDefine.h"

//  int nID = 7;
// 	IAUBGManager* pMng = (IAUBGManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUBGManager* pMng = (IAUBGManager*)AfxGetPctrInterface(nID);

// ���� : ��׶���� ������ �ʿ��� ����� �����ϴ� �Ŵ���
//
// ������  : ���ؿ� 2005.12.03(��)
//           ������ 3�ܰ��Ģ
//           1�ܰ� : Ȯ���� �ϰ�
//           2�ܰ� : �̹� �̷���� ���� �ٶ󺸰�
//           3�ܰ� : ���� �����϶�
// History : 2005.12.03 �������̽� ����
//

/* ----------------------------------------------------------------------
	// {{ �⺻����
	// HINSTANCE : LoadLib�� ���� �� ���Ϲ޴°�.

	// ��׶����  ����� DLL�� �⺻��
	#1 ���� ��⿡ ���ؼ��� �ϳ��� ��׶��常 ����.
	#2	Export�Լ�.
		1. long GetInterface(int nKey);
		2. 
	// }} �⺻����...
---------------------------------------------------------------------- */
DECLARE_INTERFACE(IAUBGManager)
{
	// ȭ���� ���� ��׶���� ���������� ���θ� �����Ѵ�. 
	// I-PARAM : szModuleName ����Ű, DLL�� �̸��� �־��ָ� ����Ű�� �ɵ�...
	// ��׶���� ����ǰ� ������ LoadLib���� ���� ���� �������ش�.
	STDMETHOD_(HINSTANCE, IsBackGround)(LPCSTR szModuleName) PURE;

	STDMETHOD_(HINSTANCE, AddBackGround)(LPCSTR szModuleName) PURE;

	// hLib ���ϰ��� ����.
	STDMETHOD_(HINSTANCE, AddBackGround)(HINSTANCE hLib) PURE;

	// Reference Count�� 0�϶��� NULL �׷��� ������ hLib����.
	STDMETHOD_(HINSTANCE, DelBackGround)(LPCSTR szModuleName) PURE;

	// Reference Count�� 0�϶��� NULL �׷��� ������ hLib����.
	STDMETHOD_(HINSTANCE, DelBackGround)(HINSTANCE hLib) PURE;
	
	// DLL�� ����� �������̽��� �Ѱ��ش�.
	// DLL�� �ε尡 �ȵǾ� ������ DLL�� �ε��Ѵ�.
	// nKey�� �ش��ϴ� �������̽��� pInterface�� �����Ѵ�.
	STDMETHOD_(HINSTANCE, GetInterface)(LPCSTR szModuleName, int nKey, long &pInterface) PURE;

	// hLib�� �˰� ���� ��..
	// nKey�� �ش��ϴ� ������ �޴´�.
	STDMETHOD_(long, GetInterface)(HINSTANCE hLib, int nKey) PURE;

	// hLib�� �˰� ���� ��..
	// nKey�� pInterface �������� �����͸� �Ѱ��ش�.
	STDMETHOD_(BOOL, SetInterface)(HINSTANCE hLib, int nKey, long pInterface) PURE;

};

typedef  long  (WINAPI* BKMNG_GetInterface)(int);
typedef  BOOL  (WINAPI* BKMNG_SetInterface)(int, long  pInterface);
typedef  BOOL  (WINAPI* BKMNG_Terminate)();

#endif // AFX_INTERFACE_OF_BACKGROUNDMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_


/*
	// ȭ���� ���� ��׶���� ���������� ���θ� �����Ѵ�. TRUE�̸� ������.
	// I-PARAM : szKey ����Ű, DLL�� �̸��� �־��ָ� ����Ű�� �ɵ�...
	// O-PARAM : TRUE/FALSE
	STDMETHOD_(BOOL, IsBackGround)(LPCSTR szKey) PURE;

	// BackGround�� �����Ѵ�.
	// I-PARAM : szKey ����Ű
	//           szModuleName DLL��
	// O-PARAM : 
	STDMETHOD_(void, SetBackGround)(LPCSTR szKey, LPCSTR szDoduleName) PURE;

	// BackGround �������� ����Ѵ�.
	// I-PARAM : szKey ����Ű
	//           szModuleName DLL��
	//           hModule DLL�ڵ�
	//           pDllKey  : �ε�� DLL�� DllKey(������ ����Ʈ)
	// O-PARAM : 
	STDMETHOD_(void, AddBackGround)(LPCSTR szKey, LPCSTR szDoduleName, HINSTANCE hModule, void* pDllKey) PURE;

	// BackGround ���ฮ��Ʈ���� �����Ѵ�.
	// I-PARAM : szKey ����Ű
	//           szModuleName DLL��
	//           hModule DLL�ڵ�
	//           pDllKey  : �ε�� DLL�� DllKey(������ ����Ʈ)
	// O-PARAM :
	STDMETHOD_(void, RemoveBackGround)(LPCSTR szKey, LPCSTR szDoduleName, HINSTANCE hModule, void* pDllKey) PURE;
*/


