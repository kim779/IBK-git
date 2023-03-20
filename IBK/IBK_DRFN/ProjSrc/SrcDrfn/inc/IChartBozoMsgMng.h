#if !defined(AFX_INTERFACE_OF_DRFN_CHARTBOZOMESSAGE_BY_JUNOK_LEE__INCLUDED_)
#define AFX_INTERFACE_OF_DRFN_CHARTBOZOMESSAGE_BY_JUNOK_LEE__INCLUDED_

#include "IBaseDefine.h"
#include "MasterDataSub.h"

#define		UUID_IChartBozoMsgManager		(51)

// �������� �������̽� ��ȣ
	// �������� ������ PartnerMng.dll���� �����Ǿ� ����. ���⿡���� ���� Interface��ȣ
#define		LUID_IChartBozoMsgManager		1

class CChartBozoItem
{
public:
	enum{
		cbi_chanage=0,
		cbi_add,
		cbi_delete,
	};

	int		 m_nOption; // enum�� �ϳ�.
	CString  m_szKey;
	CString  m_szValue;

	CChartBozoItem(int nOption, LPCSTR szKey, LPCSTR szValue)
	{
		m_nOption = nOption;
		m_szKey = szKey;
		m_szValue = szValue;
	}
};
typedef CList<CChartBozoItem*, CChartBozoItem*> List_CChartBozoItem;

class CChartBozoInput
{
public:
//@Solomon:091123SM068 -->
	CChartBozoInput()	{ InitData();	};
	CChartBozoInput(const CChartBozoInput& obj)	{ *this = obj;	};

	void InitData()
	{
		ZeroMemory(sShcode, sizeof(sShcode));
//		ZeroMemory(sTick, sizeof(sTick));
		lTick = 0;
		ZeroMemory(sTerm, sizeof(sTerm));
		ZeroMemory(sNxtFlg, sizeof(sNxtFlg));
		ZeroMemory(sDataType, sizeof(sDataType));
		lPrice = 0;
		lVolume = 0;
		lUpLmtPrice = 0;
		lDnLmtPrice = 0;
		lOpen;
		lHigh;
		lLow;
		lPreprice;	
		lPreVolume;	
		lPreOpen;
		lPreHigh;
		lPreLow;
		//ZeroMemory(sStartTime, sizeof(sStartTime));
		lFirstClose = 0;		
		nMarket = 0;
		lWonDollar = 0;

//vntsorl_20110420:[A00000410] ����Ʈ���� ù���� ���ο� ���� ����� ��������
		nPacketType = 0;
	};

	virtual void	Copy(const CChartBozoInput& right)
	{
		m_sMarket		= right.m_sMarket;
		m_sShcode		= right.m_sShcode;
		m_sTick			= right.m_sTick;
		m_sTerm			= right.m_sTerm;
		m_sNxtFlg		= right.m_sNxtFlg;
		m_sDataType		= right.m_sDataType;
		m_sVolume		= right.m_sVolume;
		m_sUpLmtPrice	= right.m_sUpLmtPrice;
		m_sDnLmtPrice	= right.m_sDnLmtPrice;
		m_sOpen			= right.m_sOpen;
		m_sHigh			= right.m_sHigh;
		m_sLow			= right.m_sLow;
		m_sPrice		= right.m_sPrice;		// 20100326_JS.Kim.  ���� ������ϴ� ���� ����
		m_sPreprice		= right.m_sPreprice;
		m_sPreVolume	= right.m_sPreVolume;
		m_sStartTime	= right.m_sStartTime;
		m_sFirstClose	= right.m_sFirstClose;
		m_sPreOpen		= right.m_sPreOpen;
		m_sPreHigh		= right.m_sPreHigh;
		m_sPreLow		= right.m_sPreLow;
		m_sWonDollor	= right.m_sWonDollor;
		memcpy(sShcode, right.sShcode, sizeof(sShcode));
		lTick			= right.lTick;
		memcpy(sTerm, right.sTerm, sizeof(sTerm));
		memcpy(sNxtFlg, right.sNxtFlg, sizeof(sNxtFlg));
		memcpy(sDataType, right.sDataType, sizeof(sDataType));
		lPrice			= right.lPrice;
		lVolume			= right.lVolume;
		lUpLmtPrice		= right.lUpLmtPrice;
		lDnLmtPrice		= right.lDnLmtPrice;
		lOpen			= right.lOpen;
		lHigh			= right.lHigh;
		lLow			= right.lLow;
		lPreprice		= right.lPreprice;
		lPreVolume		= right.lPreVolume;
		lFirstClose		= right.lFirstClose;
		lPreOpen		= right.lPreOpen;
		lPreHigh		= right.lPreHigh;
		lPreLow			= right.lPreHigh;
		nMarket			= right.nMarket;
		lWonDollar		= right.lWonDollar;
//vntsorl_20110420:[A00000410] ����Ʈ���� ù���� ���ο� ���� ����� ��������
		nPacketType		= right.nPacketType;
		m_sLastDataTime	= right.m_sLastDataTime;
		m_sVolAmt		= right.m_sVolAmt;		//[A00000651][A00000652]�����ŷ����.

		m_szMarketTime	= right.m_szMarketTime;;
		m_szBaseTime	= right.m_szBaseTime;;

	};

	virtual CChartBozoInput& operator = (const CChartBozoInput& item)	{ Copy(item);	return *this; };
//@Solomon:091123SM068 <--

//	#include "inc/MasterDataSub.h"�� ���ǵ� ������ ��ü �ٶ��ϴ�.
// 	enum {
// 		ctnone=0,
// 		ctstk=1,
// 		ctup,
// 		ctsfo,
// 		ctfo,
// 		ctfrn,
// 	};
	
public:
	//--> IChartBozoMsgManager->GetChartBozoMsg(..)�� ȣ���ϱ����� ����������ϴ� Data
	//��ȸ�Է°��߿��� �ʿ��� �κ�.
	CString m_sMarket;			// ���屸��
								//_ctstk : 1(KOSPI), 2(KOSDAQ), 3(ELW)
								//_ctsfo : 1(����), 2(�ɼ�)
								//_ctfo  : 1(����), 2(�ɼ�)

	CString m_sShcode,			// �����ڵ�
			m_sTick,			// N��
			m_sTerm,			// ���� : ����(0:ƽ/1:��/2:��/3:��/4:��/6:��)
			m_sNxtFlg,			// ������ȸ����:1(������ȸ), 0(ó����ȸ)
			m_sDataType;		//	��Ʈ : 0-������Ʈ, 1-�����Ʈ, 2-�޷���Ʈ, 3-���簡, 4-�ð���Ʈ

	//��ȸ����� �ʿ��� �κ�.
	CString m_sPrice;		// ���簡
	CString m_sVolume;		// �ŷ���
	CString m_sUpLmtPrice;	// ���Ѱ�
	CString m_sDnLmtPrice;	// ���Ѱ�
	CString m_sOpen;		// �ð�
	CString m_sHigh;		// ��
	CString m_sLow;			// ����
	CString m_sPreprice;	// ��������
	CString m_sPreVolume;	// ���ϰŷ���
	CString m_sStartTime;	// �ŷ����۽ð�(HHMMSS)
	CString m_sFirstClose;	// ��ȸ�� ù���� ����.
	CString m_sPreOpen;		// ���Ͻð�
	CString m_sPreHigh;		// ���ϰ�
	CString m_sPreLow;		// ��������
	CString	m_sWonDollor;	// ���޷� ȯ��
	//<-- IChartBozoMsgManager->GetChartBozoMsg(..)�� ȣ���ϱ����� ����������ϴ� Data
	CString	m_sVolAmt;
	CString m_sBaseValue; // ���ذ� 
	CString m_szMarketTime;
	CString m_szBaseTime;

public:
	inline void SetItemData(CString& rKey, char* pData, int nSize)
	{
		rKey.Format("%*.*s", nSize, nSize, pData);
		rKey.TrimRight();
	}
	

// ����Ʒ� �κ��� ���������� �����ϰų� Call���� ������.
public:	
	inline void DoConvert()
	{
		memcpy(sShcode, m_sShcode, m_sShcode.GetLength());
//		memcpy(sTick, m_sTick, m_sTick.GetLength());
		lTick		= atol(m_sTick);
		memcpy(sTerm, m_sTerm, m_sTerm.GetLength());
		memcpy(sNxtFlg, m_sNxtFlg, m_sNxtFlg.GetLength());
		memcpy(sDataType, m_sDataType, m_sDataType.GetLength());
		lPrice		= atol(m_sPrice);
		lVolume		= atol(m_sVolume);
		lUpLmtPrice = atol(m_sUpLmtPrice);
		lDnLmtPrice = atol(m_sDnLmtPrice);
		lOpen		= atol(m_sOpen);
		lHigh		= atol(m_sHigh);
		lLow		= atol(m_sLow);
		lPreprice	= atol(m_sPreprice);	
		lPreVolume	= atol(m_sPreVolume);	
		//memcpy(sStartTime, m_sStartTime, m_sStartTime.GetLength());
		lFirstClose = atol(m_sFirstClose);
		nMarket		= atoi(m_sMarket);
		lPreOpen	= atoi(m_sPreOpen);
		lPreHigh	= atoi(m_sPreHigh);
		lPreLow		= atoi(m_sPreLow);
		lWonDollar	= atol(m_sWonDollor);
		lVolAmt		= atol(m_sVolAmt);
	}

	char sShcode[15+1];	// �����ڵ�
//	char sTick[3+1];	// N��
	long lTick;			// N��
	char sTerm[1];		// ���� : ����(0:ƽ/1:��/2:��/3:��/4:��/6:��)
	char sNxtFlg[1];	// ������ȸ����:1(������ȸ), 0(ó����ȸ)
	char sDataType[1];	//	��Ʈ : 0-������Ʈ, 1-�����Ʈ, 2-�޷���Ʈ, 3-���簡, 4-�ð���Ʈ

	//��ȸ �������� �ʿ��� ������.
// 		char sPrice[10+1];		// ���簡
// 		char sUpLmtPrice[10+1];	// ���Ѱ�
// 		char sDnLmtPrice[10+1];	// ���Ѱ�
// 		char sOpen[10+1];		// �ð�
// 		char sHigh[10+1];		// ��
// 		char sLow[10+1];		// ����
// 		char sPreprice[10+1];	// ��������
// 		char sPreVolume[10+1];	// ���ϰŷ���
// 		char sStartTime[6+1];	// �ŷ����۽ð�(HHMMSS)
// 		char sFirstClose[10+1];	// ��ȸ�� ù���� ����.
	long lPrice;		// ���簡
	long lVolume;		// �ŷ���
	long lUpLmtPrice;	// ���Ѱ�
	long lDnLmtPrice;	// ���Ѱ�
	long lOpen;		// �ð�
	long lHigh;		// ��
	long lLow;		// ����
	long lPreprice;	// ��������
	long lPreVolume;	// ���ϰŷ���
	//char sStartTime[6+1];	// �ŷ����۽ð�(HHMMSS)
	long lFirstClose;	// ��ȸ�� ù���� ����.
	long lPreOpen;
	long lPreHigh;
	long lPreLow;
	//��Ÿ
//	char cMarket[1];		// ���屸�� : 1(KOSPI), 2(KOSDAQ), 3(ELW)
	int  nMarket;			// ���屸��
							//_ctstk : 1(KOSPI), 2(KOSDAQ), 3(ELW)
							//_ctsfo : 1(����), 2(�ɼ�)
							//_ctfo  : 1(����), 2(�ɼ�)
	long lWonDollar;

//vntsorl_20110420:[A00000410] ����Ʈ���� ù���� ���ο� ���� ����� ��������
	int  nPacketType;		//PacketType�� ������ ������ ���� ���.
	CString m_sLastDataTime;		// ������ �ڷ�����/�ð�
	long lVolAmt;			//�����ŷ����.
};

/* -------------------------------------------------------------------
�Էµ� ���� �ʿ��� ������.
	sShcode[15+1] �����ڵ�
//	sTick[3+1]	N��
	lTick		N��
	sTerm		���� : ����(0:ƽ/1:��/2:��/3:��/4:��/6:��)
	sNxtFlg		������ȸ����:1(������ȸ), 0(ó����ȸ)

��ȸ �������� �ʿ��� ������.
	sPrice[10+1] : ���簡
	sUpLmtPrice[10+1] ���Ѱ�
	sDnLmtPrice[10+1] ���Ѱ�
	sOpen[10+1] �ð�
	sHigh[10+1] ��
	sLow[10+1]  ����
	sPreprice[10+1]  ��������
	sPreVolume[10+1] ���ϰŷ���
	sStartTime[6+1] �ŷ����۽ð�(HHMMSS)
	sFirstClose[10+1] ��ȸ�� ù���� ����.

��Ÿ
	nMarket			// ���屸��
					//_ctstk : 1(KOSPI), 2(KOSDAQ), 3(ELW)
					//_ctsfo : 1(����), 2(�ɼ�)
					//_ctfo  : 1(����), 2(�ɼ�)
------------------------------------------------------------------- */

DECLARE_INTERFACE(IChartBozoMsgManager)
{
	//szFIDKey : FIDKey
	// pChartBInput : input����.
	// rString : �����޽��� ���.
	// nOption : �������� ó�����¿� ���� �ɼ�
	//			 0 : �����Ϳ� point(.)�� ���� �ö��� ó���ϴ� ����. ������ DRFN����Ʈ�� ó�����.
	//				 �ֽ�=> x1, ����/���� x0.01
	//			 1 : ������ ��ü�� �̹� ������ ������ ����. ���� x1 ���·� ó���Ѵ�.
	//			 default�� 0(���� DRFN���� ����ϴ� ���)
	// return : �����޽��� ����
	//
	// ������ǥ(CMasterTypeInfo::MARKET_CHART)�϶��� Next���θ� �ʿ�.
	STDMETHOD_(int, GetChartBozoMsg)(LPCSTR szFIDKey, CChartBozoInput* pChartBInput, CString &rString, int nOption=0) PURE;

	// return : �����޽��� ����
	STDMETHOD_(int, ChangeChartBozoMsg)(CString &rString, List_CChartBozoItem* pList) PURE;

// 	//STDMETHOD_(void, MakeChtBojo)(LPCTSTR lpszFormat, ...) PURE;	
// 	STDMETHOD_(int, GetFidType)(LPCSTR szFIDMajor) PURE;
// 	STDMETHOD_(int, GetFidType)(int nFIDMajor) PURE;
// 	STDMETHOD_(int, GetTrType)(LPCSTR szTrName) PURE;
// 
// 	STDMETHOD_(int, MakeChtBojo_ctstk)(CChartBozoInput* pChartBInput, CString& rString) PURE;
// 	STDMETHOD_(int, MakeChtBojo_ctup)(CChartBozoInput* pChartBInput, CString& rString) PURE;
// 	STDMETHOD_(int, MakeChtBojo_ctsfo)(CChartBozoInput* pChartBInput, CString& rString) PURE;
// 	STDMETHOD_(int, MakeChtBojo_ctfo)(CChartBozoInput* pChartBInput, CString& rString) PURE;
// 	STDMETHOD_(int, MakeChtBojo_ctfrn)(CChartBozoInput* pChartBInput, CString& rString) PURE;
};

//#include "../Chart/commontr/EU_p0602.h"
// 	static int MakeChtBojo_ctup();
// 	static int MakeChtBojo_ctsfo();
// 	static int MakeChtBojo_ctfo();
// 	static int MakeChtBojo_ctfrn();

// fidxmst.cDataKind = 'X';    /* ������ ���� ������              */
/* P : �̱�����                    */
/* Q : �̱�����(�ٿ�30,������100,     
       S&P 500 ���� �ش�)	        */
/* D : �̱����� �������(ADR)      */
/* C : ��ǰ ����                   */
/* F : CME ����                    */
/* G : �������屹�����(GDR)       */
/* X : ȯ��                        */
/* R : �ݸ�                        */
/* L : �����ݸ�                    */
/* B : �ֿ䱹����ä                */
/* N : �����ݸ�                    */
/* M : �ݵ�ü                      */
/* H : �����ֿ�����                */
/* E : ECN						   */
/* W : �����ְ�����                */

#endif //AFX_INTERFACE_OF_DRFN_CHARTBOZOMESSAGE_BY_JUNOK_LEE__INCLUDED_