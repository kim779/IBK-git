#ifndef __DRDSLOADLIB__HEADER__
#define __DRDSLOADLIB__HEADER__

// ��Ʈ üũ CAlertInfo Ŭ������ mp_nComponentType ������ ������ ������
#define SignalNone						0x00	// � ������Ʈ�� ���� �ʴ´�.
#define SignalNormal					0x01	// �Ϲ� ������Ʈ
#define SignalAlarm						0x02	// �˶� ������Ʈ
#define SignalTicker					0x04	// ƼĿ
#define SignalOrder						0x08	// �ֹ� ����

typedef  void* (WINAPI* DRDS_INIT)(HWND);
typedef  BOOL  (WINAPI* Drds_Term)(void*, HWND);
typedef  BOOL  (WINAPI* Drds_Connect)(void*, HWND, LPSTR);
typedef  BOOL  (WINAPI* Drds_Close)(void*, HWND, LPSTR);
typedef  BOOL  (WINAPI* Drds_Advise)(void*, HWND,UINT,LPSTR,LPSTR,LPSTR);
typedef  BOOL  (WINAPI* Drds_AdviseEx)(int, HWND, UINT, LPSTR, LPSTR, LPSTR, LPSTR);
typedef  BOOL  (WINAPI* Drds_Unadvise)(void*, HWND, UINT, LPSTR, LPSTR, LPSTR);
typedef  BOOL  (WINAPI* Drds_UnadviseEx)(int, HWND, UINT, LPSTR, LPSTR, LPSTR, LPSTR);
typedef  BOOL  (WINAPI* Drds_SetSocketHandle)(HWND);
typedef  void  (WINAPI*	Drds_SendCode)();
typedef  HWND  (WINAPI* Drds_SetGetBrodHwnd)();
typedef  void  (WINAPI*	Drds_SetFlag)(BOOL);
typedef  int   (WINAPI*	Drds_UDPCreate)();
typedef  int   (WINAPI*	Drds_UDPCreate2)(LPSTR, LPSTR);
typedef  void  (WINAPI*	Drds_UdpCreate3)(LPSTR);
typedef  void  (WINAPI*	Drds_UDPDestroy)();
typedef  int   (WINAPI*	Drds_SetUserDefData)(LPSTR, LPSTR);
typedef  BOOL  (WINAPI*	Drds_AdviseRealCodeData)(BOOL, int, LPSTR, LPSTR);


#define WM_DRDS_DATA			WM_USER + 200
#define MAX_DRDS_PACKET_LEN		4096
typedef struct  
{
	char	Name[32];
	char	Field[32];
	char	Key[32];
	UINT	Id;
	int		Len;
	char	Data[MAX_DRDS_PACKET_LEN];
} DrdsData;


class CDrdsLoadLib
{
public:
	CDrdsLoadLib()
	{
		m_hLib = NULL;
		m_szLibName = "Drds.dll";
	}


	~CDrdsLoadLib()
	{
		UnLoadLib();
	}

public:
	HINSTANCE m_hLib;
	CString m_szLibName;

	void SetLibName(LPCSTR _szLibName)
	{
		if(_szLibName==NULL)
			m_szLibName = "Drds.dll";
		else
			m_szLibName = _szLibName;
	}


	HINSTANCE LoadLib()
	{
		m_hLib  = (HINSTANCE) ::LoadLibrary((LPCTSTR)m_szLibName);
		if( m_hLib == NULL)
		{
			AfxMessageBox("Drds DLL�� Load�� �� �����ϴ�.");
			return NULL;
		}
		return m_hLib;
	}

	BOOL UnLoadLib()
	{
		if(m_hLib == NULL)  return TRUE;

		BOOL bResult = ::FreeLibrary(m_hLib);
		m_hLib = NULL;

		return bResult;
	}

	void* DrdsInit(HWND hWnd)
	{
		if(!m_hLib)	return NULL;

		DRDS_INIT fnDrdsInit;
		fnDrdsInit = (DRDS_INIT)::GetProcAddress(m_hLib,"DrdsInit");

		if(fnDrdsInit == NULL)
		{
			AfxMessageBox("DrdsInit�Լ��� ã�� �� �����ϴ�.");
			return NULL;
		}

		void* pResult;
		pResult = fnDrdsInit(hWnd);
		return pResult;
	}

	HWND  DrdsGetBrodHwnd()
	{
		if(!m_hLib)	return NULL;

		Drds_SetGetBrodHwnd fnDrdsGetBrodHwnd;
		fnDrdsGetBrodHwnd = (Drds_SetGetBrodHwnd)::GetProcAddress(m_hLib,"DrdsGetBrodHwnd");

		if(fnDrdsGetBrodHwnd == NULL)
		{
			AfxMessageBox("DrdsGetBrodHwnd�Լ��� ã�� �� �����ϴ�.");
			return NULL;
		}

		HWND pResult;
		pResult = fnDrdsGetBrodHwnd();
		return pResult;
	}

	BOOL  DrdsSetSocketHandle(HWND hWnd)
	{
		if(!m_hLib) return FALSE;
		Drds_SetSocketHandle fnSetSocketHandle;

		fnSetSocketHandle = (Drds_SetSocketHandle) ::GetProcAddress(m_hLib,"DrdsSetSocketHandle");

		if(fnSetSocketHandle == NULL)
		{
			AfxMessageBox("DrdsSetSocketHandle �Լ��� ã�� �� �����ϴ�.");
			return FALSE;	
		}

		BOOL bResult;
		bResult = fnSetSocketHandle(hWnd);
		return bResult;

	}

	BOOL DrdsTerm(void* pDrds, HWND hWnd)
	{
		if(!m_hLib) return FALSE;
		Drds_Term fnDrdsTerm;

		fnDrdsTerm = (Drds_Term)::GetProcAddress(m_hLib,"DrdsTerm");

		if(fnDrdsTerm == NULL)
		{
			AfxMessageBox("DrdsTerm�Լ��� ã�� �� �����ϴ�.");
			return FALSE;
		}

		BOOL bResult; 
		bResult = fnDrdsTerm(pDrds,hWnd);

		return bResult;
	}

	BOOL DrdsConnect(void* pDrds, HWND hWnd,LPSTR DataName)
	{

		if(!m_hLib) return FALSE;
		Drds_Connect fnDrdsConnect;

		fnDrdsConnect = (Drds_Connect)::GetProcAddress(m_hLib,"DrdsConnect");

		if(fnDrdsConnect == NULL)
		{
			AfxMessageBox("DrdsConnect�Լ��� ã�� �� �����ϴ�.");
			return FALSE;
		}
		BOOL bResult; 
		bResult = fnDrdsConnect(pDrds,hWnd,DataName);
		return bResult;

	}

	BOOL DrdsClose(void* pDrds, HWND hWnd,LPSTR DataName)
	{
		if(!m_hLib) return FALSE;
		Drds_Close fnDrdsClose;
		fnDrdsClose = (Drds_Close)::GetProcAddress(m_hLib,"DrdsClose");

		if(fnDrdsClose == NULL)
		{
			AfxMessageBox("DrdsClose�Լ��� ã�� �� �����ϴ�.");
			return FALSE;
		}
		BOOL bResult; 
		bResult = fnDrdsClose(pDrds,hWnd,DataName);
		return bResult;
	}

	BOOL DrdsAdvise(void* pDrds, HWND hWnd,UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName)
	{
		if(!m_hLib) return FALSE;
		Drds_Advise fnDrdsAdvise;
		fnDrdsAdvise = (Drds_Advise)::GetProcAddress(m_hLib,"DrdsAdvise");

		if(fnDrdsAdvise == NULL)
		{
			AfxMessageBox("DrdsAdvise�Լ��� ã�� �� �����ϴ�.");
			return FALSE;
		}
		BOOL bResult; 
		bResult = fnDrdsAdvise(pDrds,hWnd,id,DataName,FieldName, KeyName);
		return bResult;
	}


	BOOL DrdsAdviseEx(int nComponentType, HWND hWnd, UINT id, LPSTR DataName, 
						 LPSTR FieldName, LPSTR KeyName, LPSTR ObjectName)
	{
		if(!m_hLib) return FALSE;
		Drds_AdviseEx fnDrdsAdviseEx;
		fnDrdsAdviseEx = (Drds_AdviseEx)::GetProcAddress(m_hLib,"DrdsAdviseEx");

		if(fnDrdsAdviseEx == NULL)
		{
			AfxMessageBox("DrdsAdviseEx�Լ��� ã�� �� �����ϴ�.");
			return FALSE;
		}
		BOOL bResult; 
		bResult = fnDrdsAdviseEx(nComponentType, hWnd, id, DataName, FieldName, KeyName, ObjectName);
		return bResult;
	}


	BOOL DrdsUnadvise(void* pDrds, HWND hWnd,UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName)
	{
		if(!m_hLib) return FALSE;
		Drds_Unadvise fnDrdsUnadvise;

		fnDrdsUnadvise = (Drds_Unadvise)::GetProcAddress(m_hLib,"DrdsUnadvise");


		if(fnDrdsUnadvise == NULL)
		{
			AfxMessageBox("DrdsUnadvise �Լ��� ã�� �� �����ϴ�.");
			return FALSE;
		}

		BOOL bResult; 
		bResult = fnDrdsUnadvise(pDrds,hWnd,id,DataName,FieldName, KeyName);
		return bResult;
	}


	BOOL DrdsUnadviseEx(int nComponentType, HWND hWnd,UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName, LPSTR ObjectName)
	{
		if(!m_hLib) return FALSE;
		Drds_UnadviseEx fnDrdsUnadviseEx;

		fnDrdsUnadviseEx = (Drds_UnadviseEx)::GetProcAddress(m_hLib,"DrdsUnadviseEx");


		if(fnDrdsUnadviseEx == NULL)
		{
			AfxMessageBox("DrdsUnadviseEx �Լ��� ã�� �� �����ϴ�.");
			return FALSE;
		}

		BOOL bResult; 
		bResult = fnDrdsUnadviseEx(nComponentType, hWnd, id, DataName, FieldName, KeyName, ObjectName);
		return bResult;
	}


	void	DrdsSendCode()
	{
		if(!m_hLib) return;
		Drds_SendCode	fnDrdsSendCode;

		fnDrdsSendCode = (Drds_SendCode)::GetProcAddress(m_hLib,"DrdsSendCode");

		if(fnDrdsSendCode == NULL)
		{
			AfxMessageBox("DrdsSendCode �Լ��� ã�� �� �����ϴ�.");
			return;
		}
		fnDrdsSendCode();
		return;
	}

	void DrdsSetFlag(BOOL bFlag)
	{
		if(!m_hLib)	return;

		Drds_SetFlag fnDrdsSetFlag;
		fnDrdsSetFlag = (Drds_SetFlag)::GetProcAddress(m_hLib,"DrdsSetFlag");

		if(fnDrdsSetFlag == NULL)
		{
			AfxMessageBox("DrdsSetFlag �Լ��� ã�� �� �����ϴ�.");
			return;
		}

		fnDrdsSetFlag(bFlag);
	}

	int	UDPCreate()
	{
		if(!m_hLib) return 0;

		Drds_UDPCreate	fnDrdsUDPCreate;

		fnDrdsUDPCreate = (Drds_UDPCreate)::GetProcAddress(m_hLib,"UDPCreate");

		if(fnDrdsUDPCreate== NULL)
		{
			AfxMessageBox("UDPCreate �Լ��� ã�� �� �����ϴ�.");
			return 0;
		}
		return fnDrdsUDPCreate();
	}

	int	UDPCreate2(LPSTR strIP, LPSTR strPort)
	{
		if(!m_hLib) return 0;
		Drds_UDPCreate2	fnDrdsUDPCreate2;

		fnDrdsUDPCreate2 = (Drds_UDPCreate2)::GetProcAddress(m_hLib,"UDPCreate2");

		if(fnDrdsUDPCreate2== NULL)
		{
			AfxMessageBox("UDPCreate2 �Լ��� ã�� �� �����ϴ�.");
			return 0;
		}
		return fnDrdsUDPCreate2(strIP, strPort);
	}

	int UdpCreate3(LPSTR sCfgFullPathName)
	{
		if(!m_hLib) return 0;
		Drds_UdpCreate3	fnDrdsUdpCreate3;

		fnDrdsUdpCreate3 = (Drds_UdpCreate3)::GetProcAddress(m_hLib,"UdpCreate3");

		if(fnDrdsUdpCreate3== NULL)
		{
			AfxMessageBox("UdpCreate3 �Լ��� ã�� �� �����ϴ�.");
			return 0;
		}
		fnDrdsUdpCreate3(sCfgFullPathName);
		return 1;
	}

	void	UDPDestroy()
	{
		if(!m_hLib) return;
		Drds_UDPDestroy	fnDrdsUDPDestroy;

		fnDrdsUDPDestroy = (Drds_UDPDestroy)::GetProcAddress(m_hLib,"UDPDestroy");

		if(fnDrdsUDPDestroy== NULL)
		{
			AfxMessageBox("UDPDestroy �Լ��� ã�� �� �����ϴ�.");
			return;
		}
		fnDrdsUDPDestroy();
		return;
	}


	int SetUserDefData(LPSTR sKeyName, LPSTR sData)
	{

		if(!m_hLib) return 0;
		Drds_SetUserDefData	fnSetUserDefData;

		fnSetUserDefData = (Drds_SetUserDefData)::GetProcAddress(m_hLib,"SetUserDefData");

		if(fnSetUserDefData== NULL)
		{
			AfxMessageBox("SetUserDefData �Լ��� ã�� �� �����ϴ�.");
			return 0;
		}
		fnSetUserDefData(sKeyName, sData);
	}




	BOOL AdviseRealCodeData(BOOL bIsAdd, int nCount, LPSTR sPacketName, LPSTR sCodeList)
	{
		if(!m_hLib) return FALSE;
		Drds_AdviseRealCodeData	fnAdviseRealCodeData;

		fnAdviseRealCodeData = (Drds_AdviseRealCodeData)::GetProcAddress(m_hLib,"AdviseRealCodeData");

		if(fnAdviseRealCodeData== NULL)
		{
			AfxMessageBox("AdviseRealCodeData �Լ��� ã�� �� �����ϴ�.");
			return FALSE;
		}
		return fnAdviseRealCodeData(bIsAdd, nCount, sPacketName, sCodeList);
	}


};


#endif //__DRDSLOADLIB__HEADER__
