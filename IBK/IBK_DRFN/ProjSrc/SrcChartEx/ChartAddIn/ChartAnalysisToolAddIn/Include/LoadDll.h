#ifndef _NEOPLATFORM__INCLUDE_LOADDLL__HEADER__
#define _NEOPLATFORM__INCLUDE_LOADDLL__HEADER__

#define		MakeNull(xxVal)			xxVal=NULL
#define		SafeDelete(xxVal)		if(xxVal) delete xxVal; xxVal=NULL

// LoadDll State => LDS_
#define		LDS_OK				TRUE
#define		LDS_SUCCESS			TRUE
#define		LDS_ERROR			FALSE
#define		LDS_FAIL			FALSE

// 0�����϶��� ����,������ ���°�
// 0�̻��϶� ����,������ �ƴ� ���°�
// LoadDLL Result => LDR_
#define		LDR_READY				0		// �غ����
#define		LDR_LOADERROR			-1		// DLL Load Error
#define		LDR_UNLOADERROR			-2		// DLL Unload Error
#define		LDR_GETPROCADDRESS		-3		// GetProcAddress Error
#define		LDR_INPUTERROR			-4		// �Է�(�Ķ����)���� ����
#define		LDR_OUTPUTERROR			-5		// ��»��� ����

#define		MAKESTATE(xxState, xxResult)		m_bState=xxState; m_nResult=xxResult;
class CLoadDllLib
{
public:
	CLoadDllLib() {
		MAKESTATE(LDS_OK, LDR_READY);
		m_hLib = NULL;		
	}
	~CLoadDllLib()
	{
		UnLoadLib();
	}

	HMODULE LoadLib()
	{
		CString szDllName = m_szDllName;
		HMODULE		hLib = NULL;
		hLib = AfxLoadLibrary(szDllName);
		if(hLib==NULL) {
			MAKESTATE(LDS_FAIL, LDR_LOADERROR);

			CString msg;
			msg.Format("DLL[%s]�� Load�� �� �����ϴ�.", szDllName);
			TRACE("%s\n", msg);
			// AfxMessageBox(msg);
			return NULL;
		}

		MAKESTATE(LDS_OK, LDR_READY);
		TRACE("Loaded %s\n", m_szDllName);

		m_hLib = hLib;
		return hLib;
	}
	BOOL UnLoadLib()
	{
		if(!m_hLib) {
			MAKESTATE(LDS_FAIL, LDR_UNLOADERROR);
			return TRUE;
		}

		BOOL bResult = AfxFreeLibrary(m_hLib);
		m_hLib = NULL;

		MAKESTATE(LDS_OK, LDR_READY);
		TRACE("UnLoaded %s\n", m_szDllName);

		return bResult;
	}
	BOOL GetState()   { return m_bState; }
	BOOL GetResult()  { return m_nResult; }

// Attibutes
public:
	HMODULE		m_hLib;
	CString		m_szDllName;
	int			m_bState,		//  ���°� : ����.����(0), ����(1)
				m_nResult;		//	����, ���� ���е��� ��Ÿ���� ��

};

class CResourceHelp
{
public:
	HINSTANCE m_hOld;
	CResourceHelp(HINSTANCE hOld, HINSTANCE hNew)
	{
		m_hOld = hOld;
		AfxSetResourceHandle(hNew);
	}

	~CResourceHelp()
	{
		AfxSetResourceHandle(m_hOld);
	}
};


#endif //_NEOPLATFORM__INCLUDE_LOADDLL__HEADER__
