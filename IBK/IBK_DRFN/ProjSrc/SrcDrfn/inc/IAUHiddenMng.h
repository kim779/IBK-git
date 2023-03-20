#if !defined(AFX_INTERFACE_OF_BACKGOUND_EXCUTE_HIDDEND_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_BACKGOUND_EXCUTE_HIDDEND_BY_JUNOK_LEE_8B1A__INCLUDED_

// #include "../../_Include/IAUHiddenMng.h"
#include "IBaseDefine.h"

//  int nID = 7;
// 	IAUHiddenManager* pTrCommMng = (IAUHiddenManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUHiddenManager* pMng = (IAUHiddenManager*)AfxGetPctrInterface(nID);

// ���� : 
//
// ������  : ���ؿ� 2005.11.02(��)
// History : 2005.11.02 �������̽� ����
//

DECLARE_INTERFACE(IHiddenSite)
{
	//
	//
	// I-PARAM : 
	// O-PARAM : 
    STDMETHOD(StatusChange) (int nState) PURE;
};
typedef  IHiddenSite FAR * LPIHiddenSite;


//DECLARE_INTERFACE(IHiddenItem)
//{
//};

// IAUHiddenManager
DECLARE_INTERFACE(IAUHiddenManager)
{
	//
	// I-PARAM : szKey
	// O-PARAM : BOOL
	STDMETHOD_(BOOL, IsBackGround)(LPCSTR szKey) PURE;
	
	// �־��� ȭ���� BackGround�� �����Ѵ�. 
	// dllName���� DLL�� �ε��Ѵ�. 
	//
	STDMETHOD_(void, SetBackGround)(LPCSTR szKey, LPCSTR szDllName) PURE;

	// szKey : ȭ��KEY
	// szDllName  : DLL�̸�(���⼭�� �������� �ʾƵ� �ȴ�.)
	// hLoadDll : �ε�� DLL�� �ν��Ͻ��ڵ�
	// pDllKey  : �ε�� DLL�� DllKey(������ ����Ʈ)
	STDMETHOD_(void, AddBackGround)(LPCSTR szKey, LPCSTR szDllName, HINSTANCE hLoadDll, void* pDllKey)=0;

	// szKey : ȭ��szKey
	// szDllName  : DLL�̸�(���⼭�� �������� �ʾƵ� �ȴ�.)
	// hLoadDll : �ε�� DLL�� �ν��Ͻ��ڵ�
	// pDllKey  : �ε�� DLL�� DllKey(������ ����Ʈ)
	STDMETHOD_(void, RemoveBackGround)(LPCSTR szKey, LPCSTR szDllName, HINSTANCE hLoadDll, void* pDllKey)=0;
};

// �迭�� ����
#define		ARRAYCOUNT( array )		(sizeof(array) /sizeof(array[0]))


#endif // AFX_INTERFACE_OF_BACKGOUND_EXCUTE_HIDDEND_BY_JUNOK_LEE_8B1A__INCLUDED_
