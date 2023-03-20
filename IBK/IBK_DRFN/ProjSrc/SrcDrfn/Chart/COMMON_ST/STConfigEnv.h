// STConfigEnv.h: interface for the CSTDataItem class.
// ����ȯ�漳������ �����ϴ� ������ �����ϴ� ����ü��.
//
// ����� ���� ���� STConfigrueBasic, ChartSTIndexAddin, MultiChart, �ֹ����õȰ�.
// �������� ����ϱ� ���� class�� ����.
// �� ������ �����ϸ� �������� ����� �ϴ� ������Ʈ
//		- STConfigDlg
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_STCONFIGENV__4D45_4BD3_44DC_9BC1_2F5D345896A4__INCLUDED_)
#define AFX_STCONFIGENV__4D45_4BD3_44DC_9BC1_2F5D345896A4__INCLUDED_

// #include "../Common_ST/STConfigEnv.h"
#include "STConfigDef.h"
#include "../../inc/ICompanyMng.h"


class CSTConfigEnv_Jumun
{
public:
	char m_szAcctInfo[50+1];		// ���¹�ȣ+���¸�
	int	m_nJAmount;					// �ֹ�����
	int m_nBuyPrice;				// ���԰���
	int m_nSellPrice;				// û�갡�� 0:���簡,1~3�ŵ�ȣ��
	int m_nJMethod;					// �ֹ���� 0:����, 1:, 2:
	int m_nJTime;					// �ֹ�����
	int m_nJStartSignal;			// �ֹ����۽�ȣ 
	BOOL m_bSound_Buy_Use;			// ��ȣ�߻���_���Խ�ȣ ��뿩��
	CString  m_szSound_Buy_File;	// ��ȣ�߻���_���Խ�ȣ �Ҹ�
	BOOL m_bSound_Sell_Use;			// ��ȣ�߻���_û���ȣ ��뿩��
	CString  m_szSound_Sell_File;	// ��ȣ�߻���_û���ȣ �Ҹ�

	//���� : KHD 
	int			m_nMkprc_Ent; // ���԰� ���尡 ����
	int			m_nMkprc_Clr;// û�갡 ���尡 ����
	int			m_nAcct;//�������
	CString  m_szAcctName;//���¸�
	CString  m_szAcctNumber;//���¹�ȣ
	CSTConfigEnv_Jumun*	m_pDefInfo;
public:
	CSTConfigEnv_Jumun()
	{
		memset(m_szAcctInfo, 0, 51);
		m_nJAmount			= 0;
		m_nBuyPrice			= 0;
		m_nSellPrice		= 0;
		m_nJMethod			= 0;
		m_nJTime			= 0;
		m_nJStartSignal		= 0;
		m_bSound_Buy_Use	= FALSE;
		m_bSound_Sell_Use	= FALSE;
		m_pDefInfo = NULL;
	}
	~CSTConfigEnv_Jumun()
	{
		if(m_pDefInfo) delete m_pDefInfo; m_pDefInfo=NULL;
	}

	void Copy(CSTConfigEnv_Jumun* pTarget)
	{
		sprintf(m_szAcctInfo, "%s", pTarget->m_szAcctInfo);
		m_nJAmount			= pTarget->m_nJAmount;
		m_nBuyPrice			= pTarget->m_nBuyPrice;
		m_nSellPrice		= pTarget->m_nSellPrice;
		m_nJMethod			= pTarget->m_nJMethod;
		m_nJTime			= pTarget->m_nJTime;
		m_nJStartSignal		= pTarget->m_nJStartSignal;
		m_bSound_Buy_Use	= pTarget->m_bSound_Buy_Use;
		m_szSound_Buy_File	= pTarget->m_szSound_Buy_File;
		m_bSound_Sell_Use	= pTarget->m_bSound_Sell_Use;
		m_szSound_Sell_File	= pTarget->m_szSound_Sell_File;
	}

	void LoadInfo(LPCSTR szFileName);
};

class CSTConfigEnv_Signal
{
public:
	int		m_ShapeBuy;
	int		m_ShapeExitlong;
	int		m_ShapeSell;
	int		m_ShapeExitshort;
	CString m_ClrBuy;
	CString m_ClrExitlong;
	CString m_ClrSell;
	CString m_ClrExitshort;

	CSTConfigEnv_Signal* m_pDefInfo;

	CSTConfigEnv_Signal() : m_ShapeBuy(0), m_ShapeExitlong(0), m_ShapeSell(0), m_ShapeExitshort(0)
		,m_ClrBuy("255"), m_ClrExitlong("16711680"), m_ClrSell("16711680"), m_ClrExitshort("255")
	{
		m_pDefInfo = NULL;
	}

	~CSTConfigEnv_Signal()
	{
		if(m_pDefInfo) delete m_pDefInfo; m_pDefInfo=NULL;
	}

	void Copy(CSTConfigEnv_Signal* pTarget)
	{
		m_ShapeExitlong		= pTarget->m_ShapeExitlong;
		m_ShapeSell			= pTarget->m_ShapeSell;
		m_ShapeExitshort	= pTarget->m_ShapeExitshort;
		m_ClrBuy			= pTarget->m_ClrBuy;
		m_ClrExitlong		= pTarget->m_ClrExitlong;
		m_ClrSell			= pTarget->m_ClrSell;
		m_ClrExitshort		= pTarget->m_ClrExitshort;
	}

	void LoadInfo(LPCSTR szFileName);
};


class CSTConfigEnv_Setup
{
public:
	int		m_ClearPoint;			// ����û�� ����
	CString m_Chk1;					// ������(�սǷ�)
	CString m_ED1;
	int		m_P1;					// ������(�սǷ�) : P
	int		m_S1;					// ������(�սǷ�) : S
	CString m_CLR_1;				// ������(�սǷ�) : C

	CString m_Chk2;					// ��ǥ����
	CString m_ED2;
	int		m_P2;					// ��ǥ���� : P
	int		m_S2;					// ��ǥ���� : S
	CString m_CLR_2;				// ��ǥ���� : C

	CString m_Chk3;					// �ְ����
	CString m_ED3;
	int		m_P3;					// �ְ���� : P
	int		m_S3;					// �ְ���� : S
	CString m_CLR_3;				// �ְ���� : C

	CString m_Chk4;					// ��ǥ������
	CString m_ED4;
	int		m_P4;					// ��ǥ������ : P
	int		m_S4;					// ��ǥ������ : S
	CString m_CLR_4;				// ��ǥ������ : C

	CString m_Chk5;					// �ּҰ���
	CString m_ED5;
	int		m_P5;					// �ּҰ��� : P
	CString m_Bong;					// ��

	CString m_Chk6;					// ����û��
	int		m_TimeA;				// ����/����
	CString m_ED_Time;				// �ð�

	CString m_Chk7;					// ���ͺ���
	CString m_ED6;
	CString m_ED7;
	int		m_P6;					// ���ͺ���(��·�) : P
	int		m_P7;					// ���ͺ���(�϶���) : P
	int		m_S5;					// ���ͺ��� : S
	CString m_CLR_5;				// ���ͺ��� : C

	CSTConfigEnv_Setup* m_pDefInfo;

public:
	CSTConfigEnv_Setup()
	{
		m_ClearPoint = 0;
		//m_Chk1;		
		//m_ED1;
		m_P1		=0;	
		m_S1		=0;	
		m_CLR_1 = _T("16711680");

		//m_Chk2;		
		//m_ED2;
		m_P2		=0;	
		m_S2		=0;	
		m_CLR_2 = _T("16711680");

		//m_Chk3;		
		//m_ED3;
		m_P3		=0;	
		m_S3		=0;	
		m_CLR_3 = _T("16711680");

		//m_Chk4;		
		//m_ED4;
		m_P4		=0;	
		m_S4		=0;	
		m_CLR_4 = _T("16711680");

		//m_Chk5;		
		//m_ED5;
		m_P5		=0;	
		//m_Bong;		

		//m_Chk6;
		m_TimeA		=0;
		//m_ED_Time;

		//m_Chk7;		
		//m_ED6;
		//m_ED7;
		m_P6		=0;	
		m_P7		=0;	
		m_S5		=0;	
		m_CLR_5 = _T("16711680");

		m_pDefInfo = NULL;
	}

	~CSTConfigEnv_Setup()
	{
		if(m_pDefInfo) delete m_pDefInfo; m_pDefInfo=NULL;
	}

	void Copy(CSTConfigEnv_Setup* pTarget)
	{
		m_ClearPoint= pTarget->m_ClearPoint;
		m_Chk1		= pTarget->m_Chk1;		
		m_ED1		= pTarget->m_ED1;
		m_P1		= pTarget->m_P1;		
		m_S1		= pTarget->m_S1;		
		m_CLR_1		= pTarget->m_CLR_1;	

		m_Chk2		= pTarget->m_Chk2;		
		m_ED2		= pTarget->m_ED2;
		m_P2		= pTarget->m_P2;		
		m_S2		= pTarget->m_S2;		
		m_CLR_2		= pTarget->m_CLR_2;	

		m_Chk3		= pTarget->m_Chk3;		
		m_ED3		= pTarget->m_ED3;
		m_P3		= pTarget->m_P3;		
		m_S3		= pTarget->m_S3;		
		m_CLR_3		= pTarget->m_CLR_3;	

		m_Chk4		= pTarget->m_Chk4;		
		m_ED4		= pTarget->m_ED4;
		m_P4		= pTarget->m_P4;		
		m_S4		= pTarget->m_S4;		
		m_CLR_4		= pTarget->m_CLR_4;	

		m_Chk5		= pTarget->m_Chk5;		
		m_ED5		= pTarget->m_ED5;
		m_P5		= pTarget->m_P5;		
		m_Bong		= pTarget->m_Bong;		

		m_Chk6		= pTarget->m_Chk6;		
		m_TimeA		= pTarget->m_TimeA;	
		m_ED_Time	= pTarget->m_ED_Time;

		m_Chk7		= pTarget->m_Chk7;		
		m_ED6		= pTarget->m_ED6;
		m_ED7		= pTarget->m_ED7;
		m_P6		= pTarget->m_P6;		
		m_P7		= pTarget->m_P7;		
		m_S5		= pTarget->m_S5;		
		m_CLR_5		= pTarget->m_CLR_5;	
	}

	void LoadInfo(LPCSTR szFileName);
};

class CSTConfigEnv_Clear
{
public:	
	int	m_Allow;					// ���������
	int m_Repeat;					// �ߺ����� ����
	int m_JCount;					// �ߺ����� ���ȸ��
	CString m_TaxBuy;				//�ŷ����_������+����_����
	CString m_TaxSell;				//�ŷ����_������+����_û��
	int m_cbTax;					//�ŷ����_������+����_POINT
	CString m_SlipigyBuy;			//�ŷ����_��������_����
	CString m_SlipigySell;			//�ŷ����_��������_û��
	int m_cbSplipigy;				//�ŷ����_��������_POINT

	CSTConfigEnv_Clear* m_pDefInfo;

	CSTConfigEnv_Clear() : m_Allow(0), m_Repeat(1), m_JCount(0)
		,m_TaxBuy("0.1"), m_TaxSell("0.4"), m_cbTax(0)
		,m_SlipigyBuy(""), m_SlipigySell(""), m_cbSplipigy(0) 
	{
		m_pDefInfo = NULL;
	}

	~CSTConfigEnv_Clear()
	{
		if(m_pDefInfo) delete m_pDefInfo; m_pDefInfo=NULL;
	}

	void Copy(CSTConfigEnv_Clear* pTarget)
	{
		m_Allow			=pTarget->m_Allow;
		m_Repeat		=pTarget->m_Repeat;
		m_JCount		=pTarget->m_JCount;	
		m_TaxBuy		=pTarget->m_TaxBuy;	
		m_TaxSell		=pTarget->m_TaxSell;	
		m_cbTax			=pTarget->m_cbTax;	
		m_SlipigyBuy	=pTarget->m_SlipigyBuy;	
		m_SlipigySell	=pTarget->m_SlipigySell;	
		m_cbSplipigy	=pTarget->m_cbSplipigy;
	}

	void LoadInfo(LPCSTR szFileName);
};

#define STConfigEnv_Jumun		0x0001
#define STConfigEnv_Signal		0x0002
#define STConfigEnv_Setup		0x0004
#define STConfigEnv_Clear		0x0008
#define STConfigEnv_All			(STConfigEnv_Jumun|STConfigEnv_Signal|STConfigEnv_Setup|STConfigEnv_Clear)
class CSTConfigEnv
{
public:
	CSTConfigEnv_Jumun	m_InfoJumun;
	CSTConfigEnv_Signal	m_InfoSignal;
	CSTConfigEnv_Setup	m_InfoSetup;
	CSTConfigEnv_Clear	m_InfoClear;

// ST���Ͽ��� ���� �� ���� ������
	CString m_szSTFileName, m_szSTName;
	CString m_szCode, m_szCodeName;
	int		m_nCodeType;	//MultiChart�� CTabSystemTradingEx::CSTAnalysisInput::GetCodeType()���� �Ѱܹ޴� ��.
							// 0:�ֽ�, 1:����, 2:����
	int		m_nOrderType;				// VALENTRYSELL �ż�����, VALEXITLONG �ż�û��, VALEXITSHORT �ŵ�û��, VALENTRYBUY �ŵ�����

public:
// Implementation
	CSTConfigEnv()
	{
		m_nCodeType		= 0;
		m_nOrderType	= -1;
	}

	void LoadInfo(LPCSTR szFileName, int nKey=STConfigEnv_Jumun);
	void LoadInfo(LPCSTR szFileName, LPCSTR szDefFileName, int nKey=STConfigEnv_Jumun);

	void Copy(CSTConfigEnv* pTarget)	
	{
		m_szSTFileName	= pTarget->m_szSTFileName;
		m_szSTName		= pTarget->m_szSTName;
		m_szCode		= pTarget->m_szCode;
		m_szCodeName	= pTarget->m_szCodeName;	
		m_nCodeType		= pTarget->m_nCodeType;
		m_nOrderType	= pTarget->m_nOrderType;
		m_InfoJumun.Copy(&pTarget->m_InfoJumun);
	}

	BOOL GetAcctCode(CString& rString);
	BOOL GetAcctName(CString& rString);


//	CSTConfigEnv& operator=(const CSTConfigEnv& src) const
//	{
//	}
};

//*****************************************************************************
// ��  ��  ��  : inline void CSTConfigEnv::LoadInfo(LPCSTR szFileName, LPCSTR szKey)
// ��      ��  : ���������� ���� �ε����� �ʴ´�.
// ��      ��  : 
// ȣ��  �Լ�  : 
// ��  ��  ��  : ���ؿ�     (��)Ʃ����ó��
// Date        : 2006-12-15 ���� 9:48:39
//*****************************************************************************
inline void CSTConfigEnv::LoadInfo(LPCSTR szFileName, int nKey)
{
	if(nKey&STConfigEnv_Jumun)		m_InfoJumun.LoadInfo(szFileName);
	if(nKey&STConfigEnv_Signal)		m_InfoSignal.LoadInfo(szFileName);
	if(nKey&STConfigEnv_Setup)		m_InfoSetup.LoadInfo(szFileName);
	if(nKey&STConfigEnv_Clear)		m_InfoClear.LoadInfo(szFileName);
}

inline void CSTConfigEnv::LoadInfo(LPCSTR szFileName, LPCSTR szDefFileName, int nKey)
{
	if(nKey&STConfigEnv_Jumun)
	{
		m_InfoJumun.m_pDefInfo = new CSTConfigEnv_Jumun;
		m_InfoJumun.m_pDefInfo->LoadInfo(szDefFileName);
		m_InfoJumun.LoadInfo(szFileName);
		delete m_InfoJumun.m_pDefInfo;
		m_InfoJumun.m_pDefInfo = NULL;
	}

	if(nKey&STConfigEnv_Signal)
	{
		m_InfoSignal.m_pDefInfo = new CSTConfigEnv_Signal;
		m_InfoSignal.m_pDefInfo->LoadInfo(szDefFileName);
		m_InfoSignal.LoadInfo(szFileName);
		delete m_InfoSignal.m_pDefInfo;
		m_InfoSignal.m_pDefInfo = NULL;
	}

	if(nKey&STConfigEnv_Setup)
	{
		m_InfoSetup.m_pDefInfo = new CSTConfigEnv_Setup;
		m_InfoSetup.m_pDefInfo->LoadInfo(szDefFileName);
		m_InfoSetup.LoadInfo(szFileName);
		delete m_InfoSetup.m_pDefInfo;
		m_InfoSetup.m_pDefInfo = NULL;
	}

	if(nKey&STConfigEnv_Clear)
	{
		m_InfoClear.m_pDefInfo = new CSTConfigEnv_Clear;
		m_InfoClear.m_pDefInfo->LoadInfo(szDefFileName);
		m_InfoClear.LoadInfo(szFileName);
		delete m_InfoClear.m_pDefInfo;
		m_InfoClear.m_pDefInfo = NULL;
	}
}

inline BOOL CSTConfigEnv::GetAcctCode(CString& rString)
{
	if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG)
	{
		rString = m_InfoJumun.m_szAcctInfo;
	}
	else if(gCURRENT_COMPANY_ID==nPCTRComID_EUGENE)
	{
		rString.Format("%d", m_InfoJumun.m_nAcct);
	}

	if(rString.GetLength()>10)
	{
		rString = rString.Left(11);
		return TRUE;
	}

	rString.Empty();
	return FALSE;
}

inline BOOL CSTConfigEnv::GetAcctName(CString& rString)
{
	rString = m_InfoJumun.m_szAcctInfo;
	if(rString.GetLength()>10)
	{
		rString = rString.Mid(12);
		rString.TrimLeft();
		rString.TrimRight();
		return TRUE;
	}

	rString.Empty();
	return FALSE;
}


//*****************************************************************************
// ��  ��  ��  : inline void CSTConfigEnv_Jumun::LoadInfo(LPCSTR szFileName)
// ��      ��  : 
// ��      ��  : 
// ȣ��  �Լ�  : 
// ��  ��  ��  : ���ؿ�     (��)Ʃ����ó��
// Date        : 2006-12-14 ���� 8:52:57
//*****************************************************************************
inline void CSTConfigEnv_Jumun::LoadInfo(LPCSTR szFileName)
{
	CString szKey;
	char szTmp[MAX_PATH];

	if(m_pDefInfo)
	{
		//��������
		
		if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG)
		{
			szKey.Format("%s", "AcctInfo");
			GetPrivateProfileString(STCONFIG_ORDER, szKey, m_pDefInfo->m_szAcctInfo, szTmp, MAX_PATH, szFileName);
			sprintf(m_szAcctInfo, "%s", szTmp);	// ����! �ִ� 50�� ������ �ȵ�.

		}

		else if(gCURRENT_COMPANY_ID==nPCTRComID_EUGENE)
		{
		//�������
			szKey.Format("%s", "Acct");
			GetPrivateProfileString(STCONFIG_ORDER, szKey, "", szTmp, MAX_PATH, szFileName);
			m_szAcctNumber = szTmp;
			m_nAcct = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nAcct, szFileName);

		//���¸�
			szKey.Format("%s", "AcctName");
			GetPrivateProfileString(STCONFIG_ORDER, szKey, m_pDefInfo->m_szAcctName, szTmp, MAX_PATH, szFileName);
			m_szAcctName = szTmp;

			// ���԰� ���尡 ����
			szKey.Format("%s", "MKPRC_ENT");
			m_nMkprc_Ent = GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nMkprc_Ent, szFileName);

			// û�갡 ���尡 ����
			szKey.Format("%s", "MKPRC_CLR");
			m_nMkprc_Clr = GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nMkprc_Clr, szFileName);
		}
		//�ֹ�����
		szKey.Format("%s", "JAmount");
		m_nJAmount = GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nJAmount, szFileName);

		//���԰���
		szKey.Format("%s", "BuyPrice");
		m_nBuyPrice = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nBuyPrice, szFileName);
		if(m_nBuyPrice<0) m_nBuyPrice=0;

		//û�갡��
		szKey.Format("%s", "SellPrice");
		m_nSellPrice = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nSellPrice, szFileName);
		if(m_nSellPrice<0) m_nSellPrice=0;

		//�ֹ����
		szKey.Format("%s", "JMethod");
		m_nJMethod = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nJMethod, szFileName);
		if(m_nJMethod<0) m_nJMethod=0;

		//�ֹ�����
		szKey.Format("%s", "JTime");
		m_nJTime = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nJTime, szFileName);
		if(m_nJTime<0) m_nJTime=0;

		//�ֹ����۽�ȣ
		szKey.Format("%s", "JStartSignal");
		m_nJStartSignal = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nJStartSignal, szFileName);
		if(m_nJStartSignal<0) m_nJStartSignal=0;

		//��ȣ�߻���_���Խ�ȣ ��뿩��
		szKey.Format("%s", "Sound_Buy_Use");
		m_bSound_Buy_Use = (BOOL)::GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_bSound_Buy_Use, szFileName);

		//��ȣ�߻���_���Խ�ȣ �Ҹ�
		szKey.Format("%s", "Sound_Buy_File");
		GetPrivateProfileString(STCONFIG_ORDER, szKey, m_pDefInfo->m_szSound_Buy_File, szTmp, MAX_PATH, szFileName);
		m_szSound_Buy_File = szTmp;

		//��ȣ�߻���_û���ȣ ��뿩��
		szKey.Format("%s", "Sound_Sell_Use");
		m_bSound_Sell_Use = (BOOL)::GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_bSound_Sell_Use, szFileName);

		//��ȣ�߻���_û���ȣ �Ҹ�
		szKey.Format("%s", "Sound_Sell_File");
		GetPrivateProfileString(STCONFIG_ORDER, szKey, m_pDefInfo->m_szSound_Sell_File, szTmp, MAX_PATH, szFileName);
		m_szSound_Sell_File = szTmp;


	}
	else
	{
		if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG)
		{
			//��������
			szKey.Format("%s", "AcctInfo");
			GetPrivateProfileString(STCONFIG_ORDER, szKey, "", szTmp, MAX_PATH, szFileName);
			sprintf(m_szAcctInfo, "%s", szTmp);	// ����! �ִ� 50�� ������ �ȵ�.
		}
		else if(gCURRENT_COMPANY_ID==nPCTRComID_EUGENE)
		{
		//�������
			szKey.Format("%s", "Acct");
			m_nAcct = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);
			if(m_nAcct<0) m_nAcct=0;
			
		//���¸�
			szKey.Format("%s", "AcctName");
			GetPrivateProfileString(STCONFIG_ORDER, szKey, "", szTmp, MAX_PATH, szFileName);
			m_szAcctName = szTmp;


			// ���԰� ���尡 ����
			szKey.Format("%s", "MKPRC_ENT");
			m_nMkprc_Ent = GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);

			// û�갡 ���尡 ����
			szKey.Format("%s", "MKPRC_CLR");
			m_nMkprc_Clr = GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);
		}
		//�ֹ�����
		szKey.Format("%s", "JAmount");
		m_nJAmount = GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);

		//���԰���
		szKey.Format("%s", "BuyPrice");
		m_nBuyPrice = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);
		if(m_nBuyPrice<0) m_nBuyPrice=0;

		//û�갡��
		szKey.Format("%s", "SellPrice");
		m_nSellPrice = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);
		if(m_nSellPrice<0) m_nSellPrice=0;

		//�ֹ����
		szKey.Format("%s", "JMethod");
		m_nJMethod = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);
		if(m_nJMethod<0) m_nJMethod=0;

		//�ֹ�����
		szKey.Format("%s", "JTime");
		m_nJTime = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);
		if(m_nJTime<0) m_nJTime=0;

		//�ֹ����۽�ȣ
		szKey.Format("%s", "JStartSignal");
		m_nJStartSignal = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);
		if(m_nJStartSignal<0) m_nJStartSignal=0;
		//KHD : ���� 
		char szWindowsPath[MAX_PATH];
		GetWindowsDirectory(szWindowsPath, MAX_PATH-1);

		CString szMediaPath;
		szMediaPath.Format(_T("%s\\Media\\"), szWindowsPath);

		CString szDefSoundPath;
		//��ȣ�߻���_���Խ�ȣ ��뿩��
		szKey.Format("%s", "Sound_Buy_Use");
		m_bSound_Buy_Use = (BOOL)::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);

		//��ȣ�߻���_���Խ�ȣ �Ҹ�
		szKey.Format("%s", "Sound_Buy_File");
		szDefSoundPath = szMediaPath + "ding.wav";
		GetPrivateProfileString(STCONFIG_ORDER, szKey, szDefSoundPath, szTmp, MAX_PATH, szFileName);
		m_szSound_Buy_File = szTmp;

		//��ȣ�߻���_û���ȣ ��뿩��
		szKey.Format("%s", "Sound_Sell_Use");
		m_bSound_Sell_Use = (BOOL)::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);

		//��ȣ�߻���_û���ȣ �Ҹ�
		szKey.Format("%s", "Sound_Sell_File");
		szDefSoundPath = szMediaPath + "chord.wav";
		GetPrivateProfileString(STCONFIG_ORDER, szKey, szDefSoundPath, szTmp, MAX_PATH, szFileName);
		m_szSound_Sell_File = szTmp;//Modify End

	}
}


//*****************************************************************************
// ��  ��  ��  : inline void CSTConfigEnv_Signal::LoadInfo(LPCSTR szFileName)

// ��      ��  : 
// ��      ��  : 
// ȣ��  �Լ�  : 
// ��  ��  ��  : ���ؿ�     (��)Ʃ����ó��
// Date        : 2006-12-14 ���� 8:52:59
//*****************************************************************************
inline void CSTConfigEnv_Signal::LoadInfo(LPCSTR szFileName)
{
	CString szKey;
	COLORREF clrSignal = RGB(0,0,0);

	char szTmp[MAX_PATH];

	if(m_pDefInfo)
	{
		// ��ȣ������ �д´�. Start...
		int nSel = 0;
		// szKey.Format("CB_%s", TRS_ST_BUY);			// �ż�
		szKey = SHAPEBUY;
		m_ShapeBuy = ::GetPrivateProfileInt(STCONFIG_SIGNAL, szKey, m_pDefInfo->m_ShapeBuy, szFileName);
		if(m_ShapeBuy<0) m_ShapeBuy=0;

		//szKey.Format("CB_%s", TRS_ST_EXITLONG);		// �ż�û��
		szKey = SHAPEEXITLONG;
		m_ShapeExitlong = ::GetPrivateProfileInt(STCONFIG_SIGNAL, szKey, m_pDefInfo->m_ShapeExitlong, szFileName);
		if(m_ShapeExitlong<0) m_ShapeExitlong=0;

		//szKey.Format("CB_%s", TRS_ST_SELL);			// �ŵ�
		szKey = SHAPESELL;
		m_ShapeSell = ::GetPrivateProfileInt(STCONFIG_SIGNAL, szKey, m_pDefInfo->m_ShapeSell, szFileName);
		if(m_ShapeSell<0) m_ShapeSell=0;

		//szKey.Format("CB_%s", TRS_ST_EXITSHORT);	// �ŵ�û��
		szKey = SHAPEEXITSHORT;
		m_ShapeExitshort = ::GetPrivateProfileInt(STCONFIG_SIGNAL, szKey, m_pDefInfo->m_ShapeExitshort, szFileName);
		if(m_ShapeExitshort<0) m_ShapeExitshort=0;
		// ��ȣ������ �д´�. End


		// ���� ������ �о�´�. Start...
		//szKey.Format("CLR_%s", TRS_ST_BUY);			// �ż�
		szKey = COLORBUY;
		GetPrivateProfileString(STCONFIG_SIGNAL, szKey, m_pDefInfo->m_ClrBuy, szTmp, MAX_PATH, szFileName);
		m_ClrBuy = szTmp;

		//szKey.Format("CLR_%s", TRS_ST_EXITLONG);		// �ż�û��
		szKey = COLOREXITLONG;
		GetPrivateProfileString(STCONFIG_SIGNAL, szKey, m_pDefInfo->m_ClrExitlong, szTmp, MAX_PATH, szFileName);
		m_ClrExitlong = szTmp;

		//szKey.Format("CLR_%s", TRS_ST_SELL);			// �ŵ�
		szKey = COLORSELL;
		GetPrivateProfileString(STCONFIG_SIGNAL, szKey, m_pDefInfo->m_ClrSell, szTmp, MAX_PATH, szFileName);
		m_ClrSell = szTmp;

		// szKey.Format("CLR_%s", TRS_ST_EXITSHORT);	// �ŵ�û��
		szKey = COLOREXITSHORT;
		GetPrivateProfileString(STCONFIG_SIGNAL, szKey, m_pDefInfo->m_ClrExitshort, szTmp, MAX_PATH, szFileName);
		m_ClrExitshort = szTmp;
		// ���� ������ �о�´�. End
	}
	else
	{
		// ��ȣ������ �д´�. Start...
		int nSel = 0;
		// szKey.Format("CB_%s", TRS_ST_BUY);			// �ż�
		szKey = SHAPEBUY;
		m_ShapeBuy = ::GetPrivateProfileInt(STCONFIG_SIGNAL, szKey, 0, szFileName);
		if(m_ShapeBuy<0) m_ShapeBuy=0;

		//szKey.Format("CB_%s", TRS_ST_EXITLONG);		// �ż�û��
		szKey = SHAPEEXITLONG;
		m_ShapeExitlong = ::GetPrivateProfileInt(STCONFIG_SIGNAL, szKey, 0, szFileName);
		if(m_ShapeExitlong<0) m_ShapeExitlong=0;

		//szKey.Format("CB_%s", TRS_ST_SELL);			// �ŵ�
		szKey = SHAPESELL;
		m_ShapeSell = ::GetPrivateProfileInt(STCONFIG_SIGNAL, szKey, 0, szFileName);
		if(m_ShapeSell<0) m_ShapeSell=0;

		//szKey.Format("CB_%s", TRS_ST_EXITSHORT);	// �ŵ�û��
		szKey = SHAPEEXITSHORT;
		m_ShapeExitshort = ::GetPrivateProfileInt(STCONFIG_SIGNAL, szKey, 0, szFileName);
		if(m_ShapeExitshort<0) m_ShapeExitshort=0;
		// ��ȣ������ �д´�. End


		// ���� ������ �о�´�. Start...
		//szKey.Format("CLR_%s", TRS_ST_BUY);			// �ż�
		szKey = COLORBUY;
		GetPrivateProfileString(STCONFIG_SIGNAL, szKey, "255", szTmp, MAX_PATH, szFileName);
		m_ClrBuy = szTmp;

		//szKey.Format("CLR_%s", TRS_ST_EXITLONG);		// �ż�û��
		szKey = COLOREXITLONG;
		GetPrivateProfileString(STCONFIG_SIGNAL, szKey, "16711680", szTmp, MAX_PATH, szFileName);
		m_ClrExitlong = szTmp;

		//szKey.Format("CLR_%s", TRS_ST_SELL);			// �ŵ�
		szKey = COLORSELL;
		GetPrivateProfileString(STCONFIG_SIGNAL, szKey, "16711680", szTmp, MAX_PATH, szFileName);
		m_ClrSell = szTmp;

		// szKey.Format("CLR_%s", TRS_ST_EXITSHORT);	// �ŵ�û��
		szKey = COLOREXITSHORT;
		GetPrivateProfileString(STCONFIG_SIGNAL, szKey, "255", szTmp, MAX_PATH, szFileName);
		m_ClrExitshort = szTmp;
		// ���� ������ �о�´�. End
	}
}


//*****************************************************************************
// ��  ��  ��  : inline void CSTConfigEnv_Setup::LoadInfo(LPCSTR szFileName)

// ��      ��  : 
// ��      ��  : 
// ȣ��  �Լ�  : 
// ��  ��  ��  : ���ؿ�     (��)Ʃ����ó��
// Date        : 2006-12-14 ���� 8:53:02
//*****************************************************************************
inline void CSTConfigEnv_Setup::LoadInfo(LPCSTR szFileName)
{
	CString szKey;

	char szTmp[MAX_PATH];

	if(m_pDefInfo)
	{
		// ����û�� ����
		szKey.Format("%s", "ClearPoint");
		m_ClearPoint = ::GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ClearPoint, szFileName);
		if(m_ClearPoint<0 || m_ClearPoint>1) m_ClearPoint=0;

		//-------- 1 -----------------------------
		//������(�սǷ�)
		szKey.Format("%s", "Chk1");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_Chk1, szTmp, MAX_PATH, szFileName);
		m_Chk1 = szTmp;

		szKey.Format("%s", "ED1");	
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ED1, szTmp, MAX_PATH, szFileName);
		m_ED1 = szTmp;

		// ������(�սǷ�) : P
		szKey.Format("%s", "P1");
		m_P1 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_P1, szFileName);
		if(m_P1 < 0 ) m_P1 = 0;

		// ������(�սǷ�) : S
		szKey.Format("%s", "S1");
		m_S1 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_S1, szFileName);
		if(m_S1 < 0 ) m_S1 = 0;

		// ������(�սǷ�) : C
		szKey.Format("CLR_%s", "1");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_CLR_1, szTmp, MAX_PATH, szFileName);
		m_CLR_1 = szTmp;

		//------- 2 ------------------------------
		// ��ǥ����	
		szKey.Format("%s", "Chk2");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_Chk2, szTmp, MAX_PATH, szFileName);
		m_Chk2 = szTmp;

		szKey.Format("%s", "ED2");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ED2, szTmp, MAX_PATH, szFileName);
		m_ED2= szTmp;

		// ��ǥ���� : P
		szKey.Format("%s", "P2");
		m_P2 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_P2, szFileName);
		if(m_P2 < 0 ) m_P2 = 0;

		// ��ǥ���� : S
		szKey.Format("%s", "S2");
		m_S2 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_S2, szFileName);
		if(m_S2 < 0 ) m_S2 = 0;

		// ��ǥ���� : C
		szKey.Format("CLR_%s", "2");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_CLR_2, szTmp, MAX_PATH, szFileName);
		m_CLR_2 = szTmp;

		//----------- 3 --------------------------
		// �ְ���� �϶�
		szKey.Format("%s", "Chk3");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_Chk3, szTmp, MAX_PATH, szFileName);
		m_Chk3 = szTmp;

		szKey.Format("%s", "ED3");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ED3, szTmp, MAX_PATH, szFileName);
		m_ED3 = szTmp;

		// �ְ���� �϶� : P
		szKey.Format("%s", "P3");
		m_P3 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_P3, szFileName);
		if(m_P3 < 0 ) m_P3 = 0;

		// �ְ���� �϶� : S
		szKey.Format("%s", "S3");
		m_S3 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_S3, szFileName);
		if(m_S3 < 0 ) m_S3 = 0;

		// �ְ���� �϶� : C
		szKey.Format("CLR_%s", "3");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_CLR_3, szTmp, MAX_PATH, szFileName);
		m_CLR_3 = szTmp;

		//----------- 4 --------------------------
		// ��ǥ������ û��
		szKey.Format("%s", "Chk4");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_Chk4, szTmp, MAX_PATH, szFileName);
		m_Chk4 = szTmp;

		szKey.Format("%s", "ED4");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ED4, szTmp, MAX_PATH, szFileName);
		m_ED4 = szTmp;

		// ��ǥ������ û�� : P
		szKey.Format("%s", "P4");
		m_P4 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_P4, szFileName);
		if(m_P4 < 0 ) m_P4 = 0;

		// ��ǥ������ û�� : S
		szKey.Format("%s", "S4");
		m_S4 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_S4, szFileName);
		if(m_S4 < 0 ) m_S4 = 0;

		// ��ǥ������ û�� : C
		szKey.Format("CLR_%s", "4");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_CLR_4, szTmp, MAX_PATH, szFileName);
		m_CLR_4 = szTmp;

		//----------- 5 --------------------------
		//�ּҰ��ݺ�ȭ
		szKey.Format("%s", "Chk5");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_Chk5, szTmp, MAX_PATH, szFileName);
		m_Chk5 = szTmp;

		szKey.Format("%s", "ED5");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ED5, szTmp, MAX_PATH, szFileName);
		m_ED5 = szTmp;

		// �ּҰ��ݺ�ȭ : P
		szKey.Format("%s", "P5");
		m_P5 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_P5, szFileName);
		if(m_P5 < 0 ) m_P5 = 0;

		// �ּҰ��ݺ�ȭ : S

		// ��
		szKey.Format("%s", "Bong");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_Bong, szTmp, MAX_PATH, szFileName);
		m_Bong = szTmp;


		//----------- 6 --------------------------
		// ����û��
		szKey.Format("%s", "Chk6");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_Chk6, szTmp, MAX_PATH, szFileName);
		m_Chk6 = szTmp;


		// ����/����
		szKey.Format("%s", "TimeA");
		m_TimeA = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_TimeA, szFileName);
		if(m_TimeA < 0 ) m_TimeA = 1;

		// �ð�
		szKey.Format("%s", "ED_Time");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ED_Time, szTmp, MAX_PATH, szFileName);
		m_ED_Time = szTmp;

		//----------- 7 --------------------------
		// ���ͺ��� û��
		szKey.Format("%s", "Chk7");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_Chk7, szTmp, MAX_PATH, szFileName);
		m_Chk7 = szTmp;

		szKey.Format("%s", "ED6");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ED6, szTmp, MAX_PATH, szFileName);
		m_ED6 = szTmp;

		szKey.Format("%s", "ED7");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ED7, szTmp, MAX_PATH, szFileName);
		m_ED7 = szTmp;

		// ���ͺ��� û�� : P
		szKey.Format("%s", "P6");
		m_P6 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_P6, szFileName);
		if(m_P6 < 0 ) m_P6 = 0;

		// ���ͺ��� û�� : P
		szKey.Format("%s", "P7");
		m_P7 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_P7, szFileName);
		if(m_P7 < 0 ) m_P7 = 0;

		// ���ͺ��� û�� : S
		szKey.Format("%s", "S5");
		m_S5 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_S5, szFileName);
		if(m_S5 < 0 ) m_S5 = 0;

		// ���ͺ��� û�� : C
		szKey.Format("CLR_%s", "5");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_CLR_5, szTmp, MAX_PATH, szFileName);
		m_CLR_5 = szTmp;
	}
	else
	{
		// ����û�� ����
		szKey.Format("%s", "ClearPoint");
		m_ClearPoint = ::GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_ClearPoint<0 || m_ClearPoint>1) m_ClearPoint=0;

		//-------- 1 -----------------------------
		// ������(�սǷ�)
		szKey.Format("%s", "Chk1");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "", szTmp, MAX_PATH, szFileName);
		m_Chk1 = szTmp;

		szKey.Format("%s", "ED1");	
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "10", szTmp, MAX_PATH, szFileName);
		m_ED1 = szTmp;

		// ������(�սǷ�) : P
		szKey.Format("%s", "P1");
		m_P1 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_P1 < 0 ) m_P1 = 0;

		// ������(�սǷ�) : S
		szKey.Format("%s", "S1");
		m_S1 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_S1 < 0 ) m_S1 = 0;

		// ������(�սǷ�) : C
		szKey.Format("CLR_%s", "1");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "16711680", szTmp, MAX_PATH, szFileName);
		m_CLR_1 = szTmp;

		//------- 2 ------------------------------
		// ��ǥ����	
		szKey.Format("%s", "Chk2");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "", szTmp, MAX_PATH, szFileName);
		m_Chk2 = szTmp;

		szKey.Format("%s", "ED2");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "20", szTmp, MAX_PATH, szFileName);
		m_ED2= szTmp;

		// ��ǥ���� : P
		szKey.Format("%s", "P2");
		m_P2 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_P2 < 0 ) m_P2 = 0;

		// ��ǥ���� : S
		szKey.Format("%s", "S2");
		m_S2 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 2, szFileName);
		if(m_S2 < 0 ) m_S2 = 0;

		// ��ǥ���� : C
		szKey.Format("CLR_%s", "2");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "16711680", szTmp, MAX_PATH, szFileName);
		m_CLR_2 = szTmp;

		//----------- 3 --------------------------
		// �ְ���� �϶�
		szKey.Format("%s", "Chk3");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "", szTmp, MAX_PATH, szFileName);
		m_Chk3 = szTmp;

		szKey.Format("%s", "ED3");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "5", szTmp, MAX_PATH, szFileName);
		m_ED3 = szTmp;

		// �ְ���� �϶� : P
		szKey.Format("%s", "P3");
		m_P3 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_P3 < 0 ) m_P3 = 0;

		// �ְ���� �϶� : S
		szKey.Format("%s", "S3");
		m_S3 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 4, szFileName);
		if(m_S3 < 0 ) m_S3 = 0;

		// �ְ���� �϶� : C
		szKey.Format("CLR_%s", "3");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "16711680", szTmp, MAX_PATH, szFileName);
		m_CLR_3 = szTmp;

		//----------- 4 --------------------------
		// ��ǥ������ ���
		szKey.Format("%s", "Chk4");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "", szTmp, MAX_PATH, szFileName);
		m_Chk4 = szTmp;

		szKey.Format("%s", "ED4");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "30", szTmp, MAX_PATH, szFileName);
		m_ED4 = szTmp;

		// ��ǥ������ ��� : P
		szKey.Format("%s", "P4");
		m_P4 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_P4 < 0 ) m_P4 = 0;

		// ��ǥ������ ��� : S
		szKey.Format("%s", "S4");
		m_S4 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 5, szFileName);
		if(m_S4 < 0 ) m_S4 = 0;

		// ��ǥ������ ��� : C
		szKey.Format("CLR_%s", "4");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "16711680", szTmp, MAX_PATH, szFileName);
		m_CLR_4 = szTmp;

		//----------- 5 --------------------------
		// �ּҰ��ݺ�ȭ
		szKey.Format("%s", "Chk5");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "", szTmp, MAX_PATH, szFileName);
		m_Chk5 = szTmp;

		szKey.Format("%s", "ED5");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "2", szTmp, MAX_PATH, szFileName);
		m_ED5 = szTmp;

		// �ּҰ��ݺ�ȭ : P
		szKey.Format("%s", "P5");
		m_P5 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_P5 < 0 ) m_P5 = 0;

		// �ּҰ��ݺ�ȭ : S

		// ��
		szKey.Format("%s", "Bong");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "5", szTmp, MAX_PATH, szFileName);
		m_Bong = szTmp;


		//----------- 6 --------------------------
		//����û��
		szKey.Format("%s", "Chk6");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "", szTmp, MAX_PATH, szFileName);
		m_Chk6 = szTmp;


		// ����/����
		szKey.Format("%s", "TimeA");
		m_TimeA = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 1, szFileName);
		if(m_TimeA < 0 ) m_TimeA = 0;

		// �ð�
		szKey.Format("%s", "ED_Time");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "02:00", szTmp, MAX_PATH, szFileName);
		m_ED_Time = szTmp;

		//----------- 7 --------------------------
		// ���ͺ���
		szKey.Format("%s", "Chk7");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "", szTmp, MAX_PATH, szFileName);
		m_Chk7 = szTmp;

		szKey.Format("%s", "ED6");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "20", szTmp, MAX_PATH, szFileName);
		m_ED6 = szTmp;

		szKey.Format("%s", "ED7");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "10", szTmp, MAX_PATH, szFileName);
		m_ED7 = szTmp;

		// ���ͺ��� : P
		szKey.Format("%s", "P6");
		m_P6 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_P6 < 0 ) m_P6 = 0;

		// ���ͺ��� : P
		szKey.Format("%s", "P7");
		m_P7 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_P7 < 0 ) m_P7 = 0;

		// ���ͺ��� : S
		szKey.Format("%s", "S5");
		m_S5 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 6, szFileName);
		if(m_S5 < 0 ) m_S5 = 0;

		// ���ͺ��� : C
		szKey.Format("CLR_%s", "5");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "16711680", szTmp, MAX_PATH, szFileName);
		m_CLR_5 = szTmp;
	}
}


//*****************************************************************************
// ��  ��  ��  : inline void CSTConfigEnv_Clear::LoadInfo(LPCSTR szFileName)

// ��      ��  : 
// ��      ��  : 
// ȣ��  �Լ�  : 
// ��  ��  ��  : ���ؿ�     (��)Ʃ����ó��
// Date        : 2006-12-14 ���� 8:53:04
//*****************************************************************************
inline void CSTConfigEnv_Clear::LoadInfo(LPCSTR szFileName)
{
	CString szKey;
	char szTmp[MAX_PATH];

	if(m_pDefInfo)
	{
		// ���������
		szKey.Format("%s", "Allow");
		m_Allow = GetPrivateProfileInt(STCONFIG_SETUP, szKey, m_pDefInfo->m_Allow, szFileName);
		if(m_Allow<0) m_Allow=0;

		//-----------------------------------------------------------------------------------------------------
		// �ߺ����� ����
		szKey.Format("%s", "Repeat");
		m_Repeat = ::GetPrivateProfileInt(STCONFIG_SETUP, szKey, m_pDefInfo->m_Repeat, szFileName);
		if(m_Repeat<0 || m_Repeat>1) m_Repeat=1;

		// �ߺ����� ���ȸ��
		szKey.Format("%s", "JCount");
		m_JCount = GetPrivateProfileInt(STCONFIG_SETUP, szKey, m_pDefInfo->m_JCount, szFileName);
		//-----------------------------------------------------------------------------------------------------

		//�ŷ����_������+����_����
		szKey.Format("%s", "TaxBuy");
		GetPrivateProfileString(STCONFIG_SETUP, szKey, m_pDefInfo->m_TaxBuy, szTmp, MAX_PATH, szFileName);
		m_TaxBuy = szTmp;

		//�ŷ����_������+����_û��
		szKey.Format("%s", "TaxSell");
		GetPrivateProfileString(STCONFIG_SETUP, szKey, m_pDefInfo->m_TaxSell, szTmp, MAX_PATH, szFileName);
		m_TaxSell = szTmp;

		//�ŷ����_������+����_POINT
		szKey.Format("%s", "cbTax");
		m_cbTax = GetPrivateProfileInt(STCONFIG_SETUP, szKey, m_pDefInfo->m_cbTax, szFileName);
		if(m_cbTax<0) m_cbTax=0;

		//�ŷ����_��������_����
		szKey.Format("%s", "SlipigyBuy");
		GetPrivateProfileString(STCONFIG_SETUP, szKey, m_pDefInfo->m_SlipigyBuy, szTmp, MAX_PATH, szFileName);
		m_SlipigyBuy = szTmp;

		//�ŷ����_��������_û��
		szKey.Format("%s", "SlipigySell");
		GetPrivateProfileString(STCONFIG_SETUP, szKey, m_pDefInfo->m_SlipigySell, szTmp, MAX_PATH, szFileName);
		m_SlipigySell = szTmp;

		//�ŷ����_��������_POINT
		szKey.Format("%s", "cbSplipigy");
		m_cbSplipigy = GetPrivateProfileInt(STCONFIG_SETUP, szKey, m_pDefInfo->m_cbSplipigy, szFileName);
		if(m_cbSplipigy<0) m_cbSplipigy=0;
	}
	else
	{
		// ���������
		szKey.Format("%s", "Allow");
		m_Allow = GetPrivateProfileInt(STCONFIG_SETUP, szKey, 0, szFileName);
		if(m_Allow<0) m_Allow=0;

		//-----------------------------------------------------------------------------------------------------
		// �ߺ����� ����
		szKey.Format("%s", "Repeat");
		m_Repeat = ::GetPrivateProfileInt(STCONFIG_SETUP, szKey, 1, szFileName);
		if(m_Repeat<0 || m_Repeat>1) m_Repeat=1;

		// �ߺ����� ���ȸ��
		szKey.Format("%s", "JCount");
		m_JCount = GetPrivateProfileInt(STCONFIG_SETUP, szKey, 0, szFileName);
		//-----------------------------------------------------------------------------------------------------

		//�ŷ����_������+����_����
		szKey.Format("%s", "TaxBuy");
		GetPrivateProfileString(STCONFIG_SETUP, szKey, "0.1", szTmp, MAX_PATH, szFileName);
		m_TaxBuy = szTmp;

		//�ŷ����_������+����_û��
		szKey.Format("%s", "TaxSell");
		GetPrivateProfileString(STCONFIG_SETUP, szKey, "0.4", szTmp, MAX_PATH, szFileName);
		m_TaxSell = szTmp;

		//�ŷ����_������+����_POINT
		szKey.Format("%s", "cbTax");
		m_cbTax = GetPrivateProfileInt(STCONFIG_SETUP, szKey, 0, szFileName);
		if(m_cbTax<0) m_cbTax=0;

		//�ŷ����_��������_����
		szKey.Format("%s", "SlipigyBuy");
		GetPrivateProfileString(STCONFIG_SETUP, szKey, "", szTmp, MAX_PATH, szFileName);
		m_SlipigyBuy = szTmp;

		//�ŷ����_��������_û��
		szKey.Format("%s", "SlipigySell");
		GetPrivateProfileString(STCONFIG_SETUP, szKey, "", szTmp, MAX_PATH, szFileName);
		m_SlipigySell = szTmp;

		//�ŷ����_��������_POINT
		szKey.Format("%s", "cbSplipigy");
		m_cbSplipigy = GetPrivateProfileInt(STCONFIG_SETUP, szKey, 0, szFileName);
		if(m_cbSplipigy<0) m_cbSplipigy=0;
	}	
}

#endif // !defined(AFX_STCONFIGENV__4D45_4BD3_44DC_9BC1_2F5D345896A4__INCLUDED_)
