#if !defined(AFX_DLLLOADSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)
#define AFX_DLLLOADSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_

// #include "../include/DllLoadDef.h"

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
} StDllLoadData, *LPStDllLoadData;


typedef CDialog*	(WINAPI * FpDllLoad) (CView*);
typedef void		(WINAPI * FpDllUnload) (CDialog*);
typedef void		(WINAPI * FpSetData) (CDialog*, void*);
typedef void		(WINAPI * FpFormLoad) (CDialog*);

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_DLLLOADSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)

/*#if !defined(AFX_DLLLOADSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)
#define AFX_DLLLOADSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_

// #include "../include/DllLoadDef.h"

// * DLL ȭ�� �ε�� �ʿ��� ����Ÿ ����ü 
typedef struct {
	HWND		m_hSocket;				// ���� �ڵ�
	HWND		m_hParent;				// �θ� �ڵ�
	CString		m_strRootPath;			// ��Ʈ ���	
	CString		m_strUserID;				// ���� id	
	CString		m_strMapID;				// ȭ�� ��ȣ
	CString		m_strEnvFilePath;		// ȯ������ ���
	long		m_dwInterface;			// ���� ������ ������
	long		m_dwAcctManager;		// ���� ������ ������ 
	CString		m_strExtra1;				// �ӽ�
	CString		m_strExtra2;				// �ӽ�
	long		m_lExtra1;				// �ӽ�
	long		m_lExtra2;				// �ӽ� 
} StDllLoadData, *LPStDllLoadData;


typedef CDialog*	(WINAPI * FpDllLoad) (CWnd*, LPCTSTR);
typedef void		(WINAPI * FpDllUnload) (CDialog*);
typedef void		(WINAPI * FpSetData) (CDialog*, void*);
typedef void		(WINAPI * FpFormLoad) (CDialog*);

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_DLLLOADSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)
*/