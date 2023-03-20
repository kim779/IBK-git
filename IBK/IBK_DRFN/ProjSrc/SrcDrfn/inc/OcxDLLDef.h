#if !defined(AFX_OCXDLLSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)
#define AFX_OCXDLLSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_

// #include "../include/OcxDLLDef.h"

#include "WndObj.h"

// * DLL ȭ�� �ε�� �ʿ��� ����Ÿ ����ü 
typedef struct {
	HWND		m_hSocket;				// ���� �ڵ�
	HWND		m_hParent;				// �θ� �ڵ�
	CString		m_szRootPath;			// ��Ʈ ���	
	CString		m_szUserID;				// ���� id	
	CString		m_szMapID;				// ȭ�� ��ȣ
	CString		m_szEnvFilePath;		// ȯ������ ���
	long		m_dwInterface;			// ���� ������ ������
	long		m_dwAcctManager;		// ���� ������ ������ 
	CString		m_szExtra1;				// �ӽ�
	CString		m_szExtra2;				// �ӽ�
	long		m_lExtra1;				// �ӽ�
	long		m_lExtra2;				// �ӽ� 
	long		m_lExtra3;				// �ӽ� 
} StDllLoadData, *LPStDllLoadData;

#define		Init_StDllLoadData(pSt)		pSt->m_hSocket=NULL;	\
				pSt->m_hParent=NULL;		\
				pSt->m_dwInterface=NULL;	\
				pSt->m_dwAcctManager=NULL;	\
				pSt->m_lExtra1=NULL;		\
				pSt->m_lExtra2=NULL;

typedef void*		(WINAPI * FpDllLoad) (void*);
typedef void		(WINAPI * FpDllUnload) (void*);
typedef void		(WINAPI * FpSetData) (void*, void*);
typedef void		(WINAPI * FpFormLoad) (void*);
typedef void		(WINAPI * FpSetKeyData) (void*, LPCSTR szKey, void*, void*, void*);
typedef void		(WINAPI * FpFuncCall) (void*, LPCSTR szFuncName, LPCSTR szParam, VARIANT& result);

// --------------------------------------------------------------------------
// OCX_DLL�� ��ũ��Ʈ���� ���� ��� ������ �� �ֵ��� IDispatch���� ��ӹ޾� ó���� �Ѵ�.
// IDispatch ���� DLL_OCX�� ���� �� ���� �Ѱ��༭ �ʿ��� DLL�� ���� �����Ѵ�.
// --------------------------------------------------------------------------
//typedef struct {
//	HRESULT InvokeHelper(LPCSTR szFuncName, LPCSTR szParam, VARIANT& result) { return S_FALSE; }
//	HRESULT GetProperty(LPCSTR szProperty, VARIANT& data) { return S_FALSE; }
//	HRESULT SetProperty(LPCSTR szProperty, VARIANT  data)  { return S_FALSE; }
//} 

// #define		MAXDLLPARAM			3
typedef struct {
//	char	m_aParam[MAXDLLPARAM][1024+1];	// �Ķ����
	long	m_dwReserved1;
} STODINIT_IN;
#define		SZSTODINIT_IN		sizeof(STODINIT_IN)
#define		INIT_STODINIT_IN(pst)		pst->m_dwReserved1=NULL;


typedef struct {
	void		*m_pDllKey;
	IPlatformObj *m_pPlatformObj;
} STODINIT_OUT;
#define		SZSTODINIT_OUT		sizeof(STODINIT_OUT)
#define		INIT_STODINIT_OUT(pst)		pst->m_pDllKey=NULL;	pst->m_pPlatformObj=NULL;
#define		INIT_STODINIT_OUT2(st)		st.m_pDllKey=NULL;	st.m_pPlatformObj=NULL;


typedef struct {
	STODINIT_OUT *m_pInitOut;
	HWND	m_hParent;
	CRect	m_Rect;		// ����(Rect)
	UINT	m_nID;		// ������ ��Ʈ���� ID
	//char	szMapFileName[30+1];
	char	m_aOpenInfo[300+1];
} STODCREATEINFO_IN;
#define		SZSTODCREATEINFO_IN		sizeof(STODCREATEINFO_IN)

#define		INIT_STODCREATEINFO_IN(pstIn)		pstIn->m_hParent=NULL;	\
				pstIn->m_nID=0;		ZeroMemory(pstIn->m_aOpenInfo, sizeof(pstIn->m_aOpenInfo));


typedef struct {
//	char		szDllKey[30+1];		// DLLŰ���� �ִ� 30���� �Ѵ�. NULL��Ʈ��.
//	                                // DLL�� Ű���� �ٽ��̸��� "maindll"�̶�� �̸��� ����Ѵ�.
//									// TR I/O���� GetBase�� �ǵ��� ù��°���� �Ѵ�.
	IDispatch*	pDispatch;			// DLLŰ���� �ش��ϴ� Dispatch
} STDLLDISPATCH;

// #define		MAXDLLDISPATCH			3
typedef struct {
	STODINIT_OUT m_InitOut;
	
	IAWndObj	*m_pWndObj;		// IAWndObj* ����Ʈ
	// STDLLDISPATCH m_stDllDisp;	// 
	IDispatch*	m_pDispatch;	// 
} STODCREATEINFO_OUT;
#define		SZSTODCREATEINFO_OUT		sizeof(STODCREATEINFO_OUT)

#define		INIT_STODCREATEINFO_OUT(pstOut)		pstOut->m_pWndObj = NULL;	\
			pstOut->m_pDispatch = NULL;


	// STODCREATEINFO_IN  : 
	// STODCREATEINFO_OUT : 
typedef HRESULT		(WINAPI * FPInit) (STODINIT_IN*, STODINIT_OUT*);
typedef HRESULT		(WINAPI * FPLoad) (STODCREATEINFO_IN*, STODCREATEINFO_OUT*);
typedef HRESULT		(WINAPI * FPUnLoad) (void* dwDllKey);
//typedef HRESULT		(WINAPI * FPGetState) (void* dwDllKey, LPCSTR szKey, long szState);

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_OCXDLLSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)
