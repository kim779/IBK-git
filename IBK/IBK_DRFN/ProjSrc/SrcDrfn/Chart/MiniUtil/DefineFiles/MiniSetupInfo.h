#ifndef CLIENT_TYPEDEF
#define CLIENT_TYPEDEF


class CMiniSetupInfo
{
// Constructor / Destructor
public:
	CMiniSetupInfo()
	{
		m_strTitle = "";
		m_nMarketType = 0;
		m_nTimeType = 0;
		m_nPeriod = 0;
		m_nDataOption = 0;
	}
	~CMiniSetupInfo(){}
	
// Member Variables
public:
	CString m_strTitle;	// ��ư �̸�
	UINT	m_nMarketType;	// �ֽ�(0) / ����(1) / ����(2) ...
	UINT	m_nTimeType;	// ��(0) / ��(1) / ��(2) / ��(3) / ��(4) / ƽ(5)
	UINT	m_nPeriod;	// 1 ~ 100
	UINT	m_nDataOption;	// ����(0) / ��ü(1)	

// Member Functions
public:	
	void SetSetupInfo(CString strTitle, UINT nMarketType, UINT nTimeType, UINT nPeriod, UINT nDataOption);
	void ConvertStringToSetupInfo(CString strSetupInfo);
	CString ConvertSetupInfoToString();

}MiniSetupInfo;

#define SZ_CMINISETUPINFO	 sizeof(MiniSetupInfo)

typedef CArray < CMiniSetupInfo *, CMiniSetupInfo * >	CMiniSetupInfoArray;



#endif // CLIENT_TYPEDEF
