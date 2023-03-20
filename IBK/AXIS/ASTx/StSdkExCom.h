#ifndef __STSDKEXCOM_H_
#define __STSDKEXCOM_H_



//////////////////////////////////////////////////////////////
// Env Option

#define STSDKEX_ENV_OPTION_ASYNC					0x00000001
#define STSDKEX_ENV_OPTION_FUNCTIMEOUT				0x00000002
#define STSDKEX_ENV_OPTION_CUSTOMERID				0x00000003
#define STSDKEX_ENV_OPTION_STANDALONE				0x00000004
#define STSDKEX_ENV_OPTION_USEBASEPID				0x00000005
#define STSDKEX_ENV_OPTION_USEMMTIMER				0x00000006
#define STSDKEX_ENV_OPTION_PROCESSSCAN				0x00000007
#define STSDKEX_ENV_OPTION_ASCRP					0x00000008



//////////////////////////////////////////////////////////////
// InstallEx Mode Option

#define STSDKEX_IEX_MODE_SYNC						0x00000001
#define STSDKEX_IEX_MODE_ASYNC						0x00000002

//////////////////////////////////////////////////////////////
// UI Mode Option

#define STSDKEX_UI_MODE_SHOW						0x00000001
#define STSDKEX_UI_MODE_HIDE						0x00000002


//////////////////////////////////////////////////////////////
// Func Code Option

#define STSDKEX_FUNC_CODE_PB						0x00000004	// ASTx Protect Browser
#define STSDKEX_FUNC_CODE_FW						0x00000080	// ASTx Firewall
#define STSDKEX_FUNC_CODE_PCSCAN					0x00008000	// ASTx PCScan


//////////////////////////////////////////////////////////////
// Activate Option

#define STSDKEX_PB_ACTIVATE_PROTECT_MEMORY			0x00000001L
#define STSDKEX_PB_ACTIVATE_ANTIDEBUG				0x00000002L
#define	STSDKEX_PB_ACTIVATE_BLOCK_REMOTE			0x00000008L
	
//////////////////////////////////////////////////////////////
// SDK Extension Option

#define	STSDKEX_PB_EXOPTION_EXCEPTPROCESS			0x00000001L
#define	STSDKEX_PB_EXOPTION_PROTECTPROCESS_INS		0x00000002L
#define	STSDKEX_PB_EXOPTION_PROTECTPROCESS_DEL		0x00000003L
#define	STSDKEX_PB_EXOPTION_EXCEPTIONPROCESS_INS	0x00000004L
#define	STSDKEX_PB_EXOPTION_EXCEPTIONPROCESS_DEL	0x00000005L


//////////////////////////////////////////////////////////////
// CallBack Code			 
// Driver has failed to run
#define STSDKEX_PB_CALLBACK_DRIVERFAILED			0x010301			
// Debugger has been detected
#define STSDKEX_PB_CALLBACK_DETECT_DEBUG			0x010302
// In memory of attempted unauthorized access
#define STSDKEX_PB_CALLBACK_ABNORMAL_MEMORY_ACCESS	0x010601
// Attempting remote access has been detected
#define STSDKEX_PB_CALLBACK_REMOTE_DETECT			0x020007
// Attempting remote access has been blocked
#define STSDKEX_PB_CALLBACK_REMOTE_BLOCKD			0x020008


#define STSDKEX_PB_CALLBACK_START					0x030000
#define STSDKEX_PB_CALLBACK_STOP					0x030001
#define STSDKEX_PB_CALLBACK_EXCEPTION_PROCESS		0x030002
#define STSDKEX_PB_CALLBACK_PROTECT_PROCESS_INS		0x030003
#define STSDKEX_PB_CALLBACK_PROTECT_PROCESS_DEL		0x030004
#define STSDKEX_PB_CALLBACK_EXCEPTION_PROCESS_INS	0x030005
#define STSDKEX_PB_CALLBACK_EXCEPTION_PROCESS_DEL	0x030006
#define STSDKEX_PB_CALLBACK_GET_PLUG_STATUS			0x030007
#define STSDKEX_FW_CALLBACK_START					0x030008
#define STSDKEX_FW_CALLBACK_STOP					0x030009
#define STSDKEX_UP_CALLBACK_INSTALL					0x040000

//////////////////////////////////////////////////////////////
// Verify Environment Type
/*
	// EnvType string
	LogVer			// �α׹���						1.2.2.1
	LogDate			// �α׼�������					20150616
	LogTime			// �α׼����ð�					113147955
	PrivIP1			// �缳 IP 1					172.20.4.15
	PrivIP2			// �缳 IP 2					172.20.4.16
	PrivIP3			// �缳 IP 3					172.20.4.17
	McAdr1			// MAC��巹�� 1				08-9E-01-B9-1A-6B
	McAdr2			// MAC��巹�� 2				08-9E-01-B9-1A-6C
	McAdr3			// MAC��巹�� 3				08-9E-01-B9-1A-6D
	PrxyUseYn		// ���Ͻû�뿩��				Y
	PrxyIP			// ���Ͻ�IP						10.20.30.40
	OsTypeCd		// �ü��Ÿ���ڵ�				12
	OsVerCd			// �ü�������ڵ�				6.1.7601
	OsSpCd			// �ü�������Ѹ���������	01
	OsLangCd		// �ü��������ڵ�			0412
	OsRemoteYn		// �ü������������뿩��		N
	OsFwStupCd		// �ü����ȭ�������ڵ�		02
	HdSerial		// �ϵ��ũ �ø���			Y34AC050T
	HdModel			// �ϵ��ũ ��				TOSHIBA MQ01ABD100
	MbSerial		// ��������ø���				QCQG0KF34900A3
	MbManufacturer	// ��������������				ASUS
	MbProductNo		// ���������ǰ��ȣ				GTX8282
	RemoteEnv		// ����ȯ�濡�� ����			NO(�Ϲ�ȯ��) RD(���ݵ���ũž) RS(�˽���Ʈ) ���� ȯ�� �������� ����(������: '|')  ex) RD|RS
*/


//////////////////////////////////////////////////////////////
// Callback Function 

typedef struct 
{
	ULONG		ulPID;
	ULONG		uPadding;
	CHAR		szProcessName[1024];
} ST_STSDKEX_PB_PROCESS_PATH, *PST_STSDKEX_PB_PROCESS_PATH;

typedef int (__stdcall* PFN_STSDKEX_EventCallback)(long lCode, void* pParam, long lParamSize);

//////////////////////////////////////////////////////////////
// This class is exported from the stsdkex.dll

// Protect Browser
class IStPb
{
public:
	virtual DWORD _stdcall Initialize() = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall StartA(LPCSTR szCustomerID, LPCSTR szCustomPolicyURL) = 0;
	virtual DWORD _stdcall StartW(LPCWSTR szCustomerID, LPCWSTR szCustomPolicyURL) = 0;
	virtual DWORD _stdcall StopA(LPCSTR szCustomerID) = 0;
	virtual DWORD _stdcall StopW(LPCWSTR szCustomerID) = 0;
	virtual DWORD _stdcall SetActivateSubFunc(DWORD dwSubFunc) = 0;
	virtual DWORD _stdcall SetEventCallback(PFN_STSDKEX_EventCallback fnCallback) = 0;
	virtual DWORD _stdcall SetExOptionA(DWORD dwOption, LPCSTR lpszValue) = 0;
	virtual DWORD _stdcall SetExOptionW(DWORD dwOption, LPCWSTR lpszValue) = 0;
};
typedef IStPb* (_stdcall *PFN_STSDKEX_PB_GetObject)();

// Firewall
class IStFw
{
public:
	virtual DWORD _stdcall Initialize() = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall StartA(LPCSTR szCustomerID, LPCSTR szCustomPolicyURL) = 0;
	virtual DWORD _stdcall StartW(LPCWSTR szCustomerID, LPCWSTR szCustomPolicyURL) = 0;
	virtual DWORD _stdcall StopA(LPCSTR szCustomerID) = 0;
	virtual DWORD _stdcall StopW(LPCWSTR szCustomerID) = 0;
};
typedef IStFw* (_stdcall *PFN_FWSDK_GetObject)();

// Verify
class IStVerify
{
public:
	virtual DWORD _stdcall Initialize() = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall IsRunningFuncA(LPCSTR szCustomerID, DWORD dwFuncCode) = 0;
	virtual DWORD _stdcall IsRunningFuncW(LPCWSTR szCustomerID, DWORD dwFuncCode) = 0;
};

// PcLog
class IStPcLog
{
public:
	virtual DWORD _stdcall Initialize() = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall GetPcLogA(LPCSTR szEnvType, LPCSTR szOutputValue, DWORD* pdwSize, BOOL bReload) = 0;
	virtual DWORD _stdcall GetPcLogW(LPCWSTR szEnvType, LPCWSTR szOutputValue, DWORD* pdwSize, BOOL bReload) = 0;
};


// PCScan
class IStPCScan
{
public:
	virtual DWORD _stdcall Initialize() = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall StartA(LPCSTR szCustomerID, LPCSTR szCustomPolicyURL) = 0;
	virtual DWORD _stdcall StartW(LPCWSTR szCustomerID, LPCWSTR szCustomPolicyURL) = 0;
	virtual DWORD _stdcall StopA(LPCSTR szCustomerID) = 0;
	virtual DWORD _stdcall StopW(LPCWSTR szCustomerID) = 0;
	
};


#endif // __STSDKEXCOM_H_
