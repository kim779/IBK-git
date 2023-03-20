#if !defined(AFX_DLLLOADDEF_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)
#define AFX_DLLLOADDEF_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_

//#include "../../common_base/DataMng.h"
//#include "../../common_base/AcctUtil.h"
//
#include "baseGShareMng.h"
#include "OcxDLLDef.h"
//#include "baseGShareMng.h"
//#include "Account.h"
//#include "Platform.h"
//#include "../../include_base/ReserveShareName.h"


// * DLLȭ���� ����� ���� ��ӹ޾ƾ� �ϴ� Ŭ���� 
class CPrimDialog
{
public:
	long		m_lExtra1;
	long		m_lExtra2;
//	CAcctHelper*	m_pAccHelp;

	HWND		m_hSocket;			// ���� �ڵ�
	HWND		m_hParent;			// �θ� ������ �ڵ� 

	CString		m_strExtra1;		// Ticker���� ��ﶧ �ּҷ� �ϱ� ���ؼ� -> �������� ��Ź
	CString		m_strRootPath;		// ��Ʈ ���
	CString		m_strUserDir;		// User ���丮
	CString		m_strDataDir;		// Date ���丮
	CString		m_strUserID;		// ����� id

	CString		m_strMapID;			// ȭ���ȣ
	CString		m_strEnvFilePath;	// ȯ������ ���

	CStringList	m_strEnvVariables;	// ȯ�溯�� ����Ʈ (key,value)

	long		m_dwAcctManager;	// ���� �Ŵ��� 
	long		m_pShareMgr;		// ���� �Ŵ���
	
	//void FormLoad()
	void SetData(void* _pData)
	{
		if(_pData == NULL)
			return;

		LPStDllLoadData pData = (LPStDllLoadData)_pData;
		m_hSocket = pData->m_hSocket;
		m_hParent = pData->m_hParent;
		m_strRootPath = pData->m_szRootPath;
		m_strUserDir = m_strRootPath + "\\User";
		m_strDataDir = m_strRootPath + "\\Data";
		m_strUserID = pData->m_szUserID;
		m_strMapID = pData->m_szMapID;
		m_strEnvFilePath = pData->m_szEnvFilePath;
//		m_lExtra1 = pData->m_lExtra1;			// DefaultColorTable*
//		m_strExtra1 = pData->m_szExtra1;			// �������� ��Ź
		m_lExtra2 = pData->m_lExtra2;		// BaseDesl	- ojtaso

		fnSetSharedInterface(pData->m_dwInterface, pData->m_dwAcctManager);
	}

	void ClearData()
	{
		ClearSharedInterface();
	}

	void GetUserInfo()
	{
/*		typedef CString	(WINAPI * fnGetEnvVariables) (CString, CString);

		HINSTANCE hInst = LoadLibrary("EnvSetup.dll");

		if(hInst==NULL)
		{
			TRACE("EnvSetup.dll�� �ε��� �� �����ϴ�.");
			return;
		}
				
		fnGetEnvVariables fpGetEnvVar = (fnGetEnvVariables)::GetProcAddress(hInst, "fnGetEnvVariables");
		if(fpGetEnvVar)
		{
			CString szUnit;
			CString szRet = fpGetEnvVar(m_szMapID, m_szEnvFilePath);

			// �� �������� ������(;)�� ������ �����Ѵ�. 
			if(szRet.Right(1) == ";")
				szRet.Mid(0, szRet.GetLength()-1);

			int nIndex = szRet.Find(";");
			while(nIndex>0)
			{
				szUnit = szRet.Left(nIndex);
				szRet = szRet.Mid(nIndex + 1);
				m_szEnvVariables.AddTail(szUnit);
				nIndex = szRet.Find(";");
				if(nIndex<0)
				{
					m_szEnvVariables.AddTail(szRet);
					break;
				}
			}
		}
		else
		{
			TRACE("fnGetEnvVariables �Լ� �����͸� ���� �� �����ϴ�.");			
		}
*/
	}

	// * ���� ó�� ���� Nasted Class
	class CGSharedSvr  : public CDeskSharedServer
	{
	public:
		~CGSharedSvr() {};
		CList <CListGlobalCliient*, CListGlobalCliient*> m_CliList;
		CBaseChildShareManager*	m_pGSharedMain;
		CString		m_szShareValue;
		
		void RemoveAll();
		BOOL NotifyToClient(LPCSTR _szName, LPCSTR _szValue, CGlobalSharedClient* _pCli);
		
		// Virtual
		// CGlobalSharedClient ���� ȣ���ϴ� �Լ�
		virtual BOOL GetData(LPCSTR _szName, CString& szValue)
		{
			return TRUE;
		}
		virtual BOOL SetData(LPCSTR _szName, LPCSTR _szValue, CGlobalSharedClient* _pCli)
		{
			return TRUE;
		}
		virtual BOOL Advise(LPCSTR _szName, CGlobalSharedClient* _pClient)
		{
			return TRUE;
		}
		virtual BOOL UnAdvise(LPCSTR _szName, CGlobalSharedClient* _pClient)
		{
			return TRUE;
		}
		
		// CBaseChildShareManager ���� ȣ���ϴ� �Լ�
		virtual BOOL ChangedData(LPCSTR _szName, LPCSTR _szValue)
		{
			MYMETHOD_PROLOGUE(CPrimDialog, GSharedSvr)
		
			return pThis->ChangedGlobalData(_szName, _szValue);
		}
		
		virtual LPCSTR GetData2(LPCSTR _szName)
		{
			return NULL;
		}
		
		CList<CDeskSharedServer*, CDeskSharedServer*> m_DeskSSList;
		virtual void SetDeskSharedServer(long* pList, int nListCnt)
		{
		}

		void NotifyDeskSharedServer(LPCSTR _szName, LPCSTR _szValue);		
	} m_xGSharedSvr;
	friend class CGSharedSvr;

	void fnSetSharedInterface(long dwInterface, long dwAcctManager)
	{
		if(dwInterface && dwAcctManager)
		{
			m_xGSharedSvr.m_pGSharedMain = (CBaseChildShareManager*)dwInterface;

			BOOL ret = m_xGSharedSvr.m_pGSharedMain->Advise(&m_xGSharedSvr);
//			m_dwAcctManager = dwAcctManager;
			m_pShareMgr = dwInterface;

//			m_pAccHelp = (CAcctHelper *) m_dwAcctManager;
		}
	}
	void ClearSharedInterface()
	{
		m_xGSharedSvr.m_pGSharedMain->UnAdvise(&m_xGSharedSvr);
	}
	virtual BOOL ChangedGlobalData(CString szName, CString szValue)
	{
		return TRUE;
	}
};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_DLLLOADDEF_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)
