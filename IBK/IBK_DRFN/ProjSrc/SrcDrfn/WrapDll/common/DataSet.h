// DataSet.h: interface for the CFutureDataSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUTUREDATASET_H__004D79E3_305C_4245_8682_90D380181782__INCLUDED_)
#define AFX_FUTUREDATASET_H__004D79E3_305C_4245_8682_90D380181782__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _EXT_DATA_SET
	#define	DATASET_DECLSPEC __declspec(dllimport)
#else
	#define	DATASET_DECLSPEC __declspec(dllexport)
#endif

#include <afxtempl.h>
#include "networkdefine.h"

//20080324 �̹��� >>
#define	FILE_INI					"TheorySetup.ini"

#define	SECTION_CONFIGURATION		"Config"

#define	SETUP_FILE_INI				"StrategyView.ini"

#define		THEORY_KEY_01			"�����ڻ갡������"
#define		THEORY_KEY_01_1			"KOSPI200����"
#define		THEORY_KEY_01_2			"��������"
#define		THEORY_KEY_02			"����������"
#define		THEORY_KEY_02_1			"�ݺ�����"
#define		THEORY_KEY_02_2			"ǲ������"
#define		THEORY_KEY_03			"�����ϼ�"
#define		THEORY_KEY_04			"���߽ð���ġ���ҹݿ�"
#define		THEORY_KEY_04_1			"���۽ð�"
#define		THEORY_KEY_04_2			"���ð�"

#define		ORDER_KEY_01			"�ŵ��ֹ��ð��ݱ���"	
#define		ORDER_KEY_02			"�ŵ��ֹ��ôܰ�����"
#define		ORDER_KEY_03			"�ŵ�ȣ������"
#define		ORDER_KEY_04			"�ż��ֹ��ð��ݱ���"
#define		ORDER_KEY_05			"�ż��ֹ��ôܰ�����"
#define		ORDER_KEY_06			"�ż�ȣ������"

#define		SLIPPAGE_ORDER_KEY_01	"�켱�ŷ�������"
#define		SLIPPAGE_ORDER_KEY_02	"�����켱�ŷ��ֹ�����"
#define		SLIPPAGE_ORDER_KEY_03	"�켱�ŷ��ŵ��ֹ���"
#define		SLIPPAGE_ORDER_KEY_04	"�켱�ŷ��ż��ֹ���"
#define		SLIPPAGE_ORDER_KEY_05	"�����ŷ��ŵ��ֹ���"
#define		SLIPPAGE_ORDER_KEY_06	"�����ŷ��ż��ֹ���"
#define		SLIPPAGE_ORDER_KEY_07	"���簡���Ǳ���"
#define		SLIPPAGE_ORDER_KEY_07_1 "�켱�ŷ��ڵ��ֹ����"

#define		DISPLAY_KEY_01			"��հ�����"
#define		DISPLAY_KEY_02			"GREEK��ġǥ��"
#define		DISPLAY_KEY_03			"���������߰���"
#define		DISPLAY_KEY_04			"���Ĺ��"
#define		DISPLAY_KEY_04_1		"���Ĺ������"
#define		DISPLAY_KEY_04_2		"����Ÿ��"
#define		DISPLAY_KEY_05			"�����ڻ��ּҰ�"
#define		DISPLAY_KEY_06			"�����ڻ��ִ밪"
#define		DISPLAY_KEY_07			"����ǥ�ù���"
#define		DISPLAY_KEY_07_1		"����ǥ���ִ�"
#define		DISPLAY_KEY_07_2		"����ǥ���ּ�"
#define		DISPLAY_KEY_08			"�ܰ����ǥ��"
#define		DISPLAY_KEY_09			"�򰡼��Ͱ����"

#define		ETC_KEY_01				"�����ἳ��"
#define		ETC_KEY_01_1			"������������"
#define		ETC_KEY_01_2			"�ɼǼ�������"
#define		ETC_KEY_01_3			"�ֽļ�������"
#define		ETC_KEY_02				"���űݰ��ü���"
#define		ETC_KEY_03				"�ż��ŵ�������"
#define		ETC_KEY_04				"���簡����"
#define		ETC_KEY_04_1			"û����ؿ켱ȣ��"
#define		ETC_KEY_05				"����ü�ᰡ����"
#define		ETC_KEY_06				"���纯��������"
//20080324 �̹��� <<

class CTheoryForMargin;
class CStrategyCalc;

class CAccount
{
public:
	CString m_strAccount;
	CString m_strAccountName;
	CString m_strGubun1;
	CString m_strGubun2;

public:
	CAccount & operator=(CAccount & account)
	{
		m_strAccount = account.m_strAccount;
		m_strAccountName = account.m_strAccountName;
		m_strGubun1 = account.m_strGubun1;
		m_strGubun2 = account.m_strGubun2;

		return *this;
	}
};
typedef CArray < CAccount *, CAccount *> CAccountArray;


class DATASET_DECLSPEC CHogaSet
{
public:
	
	long	m_lCount;//�Ǽ�
	CString m_strPrice;//ȣ��
	CString m_strVolume;//�ܷ�
	
public:
	CHogaSet & operator=(CHogaSet & hoga)
	{
		m_lCount	= hoga.m_lCount;
		m_strPrice	= hoga.m_strPrice;
		m_strVolume	= hoga.m_strVolume;		
		return *this;
	}
};
#define	HOGA_CNT	5

class DATASET_DECLSPEC CContractSet
{
public:
	CString	m_strContractTime;
	CString m_strContractPrice;
	CString	m_strOffer;
	CString m_strBid;
	CString m_strContractVolume;
	CString m_strContractAttr;//ü���� �ŵ�ȣ���� �Ǿ��°�?, �ż�ȣ���� �Ǿ��°�?

public:

	CContractSet & operator=(CContractSet & cont)
	{
		m_strContractTime	= cont.m_strContractTime;
		m_strContractPrice	= cont.m_strContractPrice;
		m_strOffer			= cont.m_strOffer;
		m_strBid			= cont.m_strBid;
		m_strContractVolume = cont.m_strContractVolume;
		m_strContractAttr	= cont.m_strContractAttr;
		return *this;
	}
	
};
typedef CArray < CContractSet * , CContractSet *> CContractArray;

class CRTDataBaseType
{
public:
	UINT m_uDataType;

};
typedef CArray < CRTDataBaseType *, CRTDataBaseType *> CRTDataBaseTypeArray;

class DATASET_DECLSPEC CDataSet : public CRTDataBaseType
{
public:
	CDataSet()
	{
		m_nRTDataType = RT_NO_DATA_TYPE;
		m_bSelect = TRUE;
		m_bRemainItem = FALSE;
		m_pTheoryForMargin = NULL;
//		m_pNDataSet = NULL;
		m_uDataType = RT_DATA_TYPE;

		m_bHedge = FALSE;
	}

	~CDataSet()
	{
		int i;
		for(i = 0 ; i < m_ContractArray.GetSize(); i++) delete m_ContractArray.GetAt(i);
		m_ContractArray.RemoveAll();
		
// 		if(m_pNDataSet) 
// 		{
// 		
// 			delete m_pNDataSet;			
// 			m_pNDataSet = NULL;
// 		}
	}

public:

	int			m_nRTDataType;					//N�� ���������� �ƴ��� ���� (���߿� �� Ȯ���� �� ���� ���̴�.)
												//�Ϲݵ����� RT_DATA_TYPE	
												//N�� ������ RTN_DATA_TYPE
												//����ü��	 RT_EXP_DATA_TYPE
	
	CUIntArray	m_nIDArray;						//�ش������� 

	CString	m_strTime;					//�ð�.

	CString	m_strCount;					//�������
	CString	m_strTradeType;				//�Ÿű���("�ż�", "�ŵ�")
	CString m_strAvePrice;				//�ŸŰ�(�Ǵ� ��հ�)
	CString m_strAveAmount;				//�Ÿűݾ�(����ȣ �߰� 20080407)

	CString m_strCode;					//�����ڵ� 
	CString m_strName;					//�����
	CString m_strStrike;				//��簡��
	
	CString m_strCurPrice;				//���簡
	CString m_strBid;					//�ż�ȣ��
	CString m_strOffer;					//�ŵ�ȣ��
	CString m_strVolume;				//�ŷ���
	CString	m_strMoney;					//�ŷ��ݾ�
	CString m_strLastVolume;			//���ϰŷ���
	CString m_strBidVol;				//�ż�ȣ�� �ܷ�
	CString m_strOfferVol;				//�ŵ�ȣ�� �ܷ�
	CString m_strLastPrice;				//��������
	CString	m_strNotTradeVolume;		//�̰�����
	CString	m_strUnitVol;				//����ü�ᷮ (�ǽð����� ���簡 ü�ᵥ���Ͱ� ���� ��츸 ������ ����ִ�)

	CString m_strRemainDay;				//�ܿ���
	CString m_strLastTradeDate;			//������
	CString m_strHistoricalVolate;		//������������(90��)
	CString m_strCDRate;				//������������	
	CString m_strIndex;					//��������

	CString m_strPredictPrice;			//�ֽ� ����ü�ᰡ (20080416����ȣ�߰�)

//	CString m_strFlag;					//���ϴ�񱸺�(��쿡������ ������)

	CHogaSet	m_BidArray[HOGA_CNT];
	CHogaSet	m_OfferArray[HOGA_CNT];	

	CHogaSet	m_TotalBid;
	CHogaSet	m_TotalOffer;

	CContractArray	m_ContractArray;//ü��ƽ ������ ����.

	CString		m_strCoveredCallCount;		//����ڰ� �����ϰų� �ܰ��������� ������ Ŀ������ ����.
	BOOL		m_bSelect;					//���õǾ����� ����.
	BOOL		m_bRemainItem;				//�ܰ���.
	
	//���� 
	CString		m_strReserved;				//�ʿ��� ��� �����ϰ� ���.
	//(8531���� �����͸� �ϴ� �� ������� ���޽� �׸��� �ٹ�ȣ�� ���)
	//n�� �����Ϳ����� ���纯�������� ����Ǿ� �ִ�.(201999999, 301999999, 401999999)

	CTheoryForMargin * m_pTheoryForMargin;	//���űݿ� �̷а� ���޿�.

	CString				m_strTPrice;//�̷а� ���ű� ���ÿ� �̷а��� ������ �� ���.(ETF�� ��� �ֱٿ��������簡)

	CString				m_strIVForMargin;//���űݿ� ���纯����


	//kp200�� ���� ��...
	CString		m_strCurPrice2;				//���簡
	CString		m_strStartPrice2;			//�ð�
	CString		m_strHighPrice2;			//��
	CString		m_strLowPrice2;				//����
	CString		m_strVolume2;				//�ŷ���
	CString		m_strMoney2;				//�ŷ��ݾ�
	CString		m_strPrevPrice2;			//��������	
	CString		m_strPrevVolume2;			//���ϰŷ���
	CString		m_strUnderlyingAssetsCode;	//�����ڻ� �ڵ�.
	
	///////////////////////////////////////////
	//���迡�� ���� �κ��̶� ������ ����� ����ϴ� 
	//���躯���� �ð��� �����Ƿ� �̰��� N�� ������ �����͸� �߰��Ѵ�.
//	CDataSet *  m_pNDataSet;


	//���� ��հ� �Է°�.
	CString		m_strSaveAvePrice;

	CString		m_strOrderEnableCount;
	CString		m_strRemainItemCount;

	CString		m_strCoveredCallRemainItemCount;
	CString		m_strCoveredCallEnableCount;

	
	///////////////////////////////////////////////////////////////////
	//
	//	�������� ���� �̷а�, ���纯����, ��Ÿ, ����, ��Ÿ, ������ ����
	//
	////////////////////////////////////////////////////////////////////
	CString		m_strTPriceFromServer;
	CString		m_strIVFromServer;
	CString		m_strDeltaFromServer;
	CString		m_strGammaFromServer;
	CString		m_strThetaFromServer;
	CString		m_strVegaFromServer;

	BOOL		m_bHedge;					// �������񿩺�
public:

	void AddID(int nID)
	{
		m_nIDArray.Add(nID);
	}

	void AddIDArray(CDataSet * pDataSet)
	{
		m_nIDArray.Append(pDataSet->m_nIDArray);
	}

	void DeleteAllID()
	{
		m_nIDArray.RemoveAll();
	}

	void DeleteID(UINT nID)
	{
		int i,nSize = m_nIDArray.GetSize();
		for(i = nSize - 1 ; i >= 0; i--)
			if(m_nIDArray.GetAt(i) == nID)
			{
				m_nIDArray.RemoveAt(i);
			}
	}

	CDataSet & operator=(CDataSet & data)
	{
		m_strTime			= data.m_strTime;
		m_strCount			= data.m_strCount;
		m_strTradeType		= data.m_strTradeType;
		m_strAvePrice		= data.m_strAvePrice;
		m_strAveAmount		= data.m_strAveAmount; // ����ȣ 20080407
		m_strCoveredCallCount=data.m_strCoveredCallCount;

		m_nRTDataType		= data.m_nRTDataType;
		m_strCode			= data.m_strCode;
		m_strName			= data.m_strName;
		m_strStrike			= data.m_strStrike;
		m_strCurPrice		= data.m_strCurPrice;
		m_strBid			= data.m_strBid;
		m_strOffer			= data.m_strOffer;
		m_strVolume			= data.m_strVolume;
		m_strMoney			= data.m_strMoney;
		m_strUnitVol		= data.m_strUnitVol;
		m_strBidVol			= data.m_strBidVol;
		m_strOfferVol		= data.m_strOfferVol;
		m_strLastPrice		= data.m_strLastPrice;
		m_strRemainDay		= data.m_strRemainDay;
		m_strLastTradeDate	= data.m_strLastTradeDate;
		m_strHistoricalVolate=data.m_strHistoricalVolate;
		m_strCDRate			= data.m_strCDRate;
		m_strIndex			= data.m_strIndex;
		m_strReserved		= data.m_strReserved;
		m_bSelect			= data.m_bSelect;
		m_bRemainItem		= data.m_bRemainItem;
		m_strIVForMargin	= data.m_strIVForMargin;
//		m_strFlag			= data.m_strFlag;
		m_strNotTradeVolume = data.m_strNotTradeVolume;
		m_strTPriceFromServer=data.m_strTPriceFromServer;		//�������� ���� �̷а�
		m_strIVFromServer	=data.m_strIVFromServer;			//�������� ���� ���纯����
		m_strDeltaFromServer=data.m_strDeltaFromServer;			//�������� ���� ��Ÿ
		m_strGammaFromServer=data.m_strGammaFromServer;			//�������� ���� ����
		m_strThetaFromServer=data.m_strThetaFromServer;			//�������� ���� ��Ÿ
		m_strVegaFromServer	=data.m_strVegaFromServer;			//�������� ���� ����

// 		m_strNAV			= data.m_strNAV;					//NAV
// 		m_strDiffNAV		= data.m_strDiffNAV;				//NAV����ġ(���簡 ���)
// 		m_strDiffRateNAV	= data.m_strDiffRateNAV;			//NAV������(���簡 ���)
// 		m_strDiffKP200		= data.m_strDiffKP200;				//KP200����ġ(���簡 ���)
// 		m_strDiffRateKP200	= data.m_strDiffRateKP200;			//KP200������(���簡 ���)
// 		m_strDiffKP2001		= data.m_strDiffKP2001;				//KP200����ġ(NAV ���)
// 		m_strDiffRateKP2001	= data.m_strDiffRateKP2001;			//KP200������(NAV ���)

		m_bHedge			= data.m_bHedge;					//�������񿩺�

		m_nIDArray.RemoveAll();		
		m_nIDArray.Append(data.m_nIDArray);

		for(int i = 0 ; i < HOGA_CNT ; i++)
		{
			m_BidArray[i] = data.m_BidArray[i];
			m_OfferArray[i]=data.m_OfferArray[i];
		}
		m_TotalBid = data.m_TotalBid;
		m_TotalOffer = data.m_TotalOffer;

		for(i = 0 ; i < m_ContractArray.GetSize(); i++) delete m_ContractArray.GetAt(i);
		m_ContractArray.RemoveAll();

		for(i = 0 ; i < data.m_ContractArray.GetSize(); i++) 
		{
			CContractSet * pNewSet = new CContractSet;
			*pNewSet = *data.m_ContractArray.GetAt(i);
			m_ContractArray.Add(pNewSet);
		}

		m_strCurPrice2		= data.m_strCurPrice2;				//���簡
		m_strStartPrice2	= data.m_strStartPrice2;			//�ð�
		m_strHighPrice2		= data.m_strHighPrice2;			//��
		m_strLowPrice2		= data.m_strLowPrice2;				//����
		m_strVolume2		= data.m_strVolume2;				//�ŷ���
		m_strMoney2			= data.m_strMoney2;
		m_strPrevPrice2		= data.m_strPrevPrice2;			//��������	
		m_strPrevVolume2	= data.m_strPrevVolume2;			//���ϰŷ���
		m_strUnderlyingAssetsCode = data.m_strUnderlyingAssetsCode;	//�����ڻ� �ڵ�.
		
	
		m_pTheoryForMargin = data.m_pTheoryForMargin;		
		m_strTPrice = data.m_strTPrice;

		
		m_strSaveAvePrice = data.m_strSaveAvePrice;
		m_strOrderEnableCount	= data.m_strOrderEnableCount;
		m_strRemainItemCount	= data.m_strRemainItemCount;
		m_strCoveredCallRemainItemCount = data.m_strCoveredCallRemainItemCount;
		m_strCoveredCallEnableCount = data.m_strCoveredCallEnableCount;

		return *this;
	}

	//�ü������͸� �����ϴ� �뵵.
	void CopySiseData(CDataSet & data)
	{
		m_strCode			= data.m_strCode;
		m_strName			= data.m_strName;
		m_strStrike			= data.m_strStrike;
		m_strCurPrice		= data.m_strCurPrice;
		m_strBid			= data.m_strBid;
		m_strOffer			= data.m_strOffer;
		m_strVolume			= data.m_strVolume;
		m_strMoney			= data.m_strMoney;
		m_strBidVol			= data.m_strBidVol;
		m_strOfferVol		= data.m_strOfferVol;
		m_strUnitVol		= data.m_strUnitVol;
		m_strLastPrice		= data.m_strLastPrice;
		m_strRemainDay		= data.m_strRemainDay;
		m_strLastTradeDate	= data.m_strLastTradeDate;
		m_strHistoricalVolate=data.m_strHistoricalVolate;
		m_strCDRate			= data.m_strCDRate;
		m_strIndex			= data.m_strIndex;

		m_strCurPrice2		= data.m_strCurPrice2;				//���簡
		m_strStartPrice2	= data.m_strStartPrice2;			//�ð�
		m_strHighPrice2		= data.m_strHighPrice2;			//��
		m_strLowPrice2		= data.m_strLowPrice2;				//����
		m_strVolume2		= data.m_strVolume2;				//�ŷ���
		m_strMoney2			= data.m_strMoney2;
		m_strPrevPrice2		= data.m_strPrevPrice2;			//��������	
		m_strPrevVolume2	= data.m_strPrevVolume2;			//���ϰŷ���

//		m_strFlag			= data.m_strFlag;
		m_strNotTradeVolume = data.m_strNotTradeVolume;
		m_strTPriceFromServer=data.m_strTPriceFromServer;		//�������� ���� �̷а�
		m_strIVFromServer	=data.m_strIVFromServer;			//�������� ���� ���纯����
		m_strDeltaFromServer=data.m_strDeltaFromServer;			//�������� ���� ��Ÿ
		m_strGammaFromServer=data.m_strGammaFromServer;			//�������� ���� ����
		m_strThetaFromServer=data.m_strThetaFromServer;			//�������� ���� ��Ÿ
		m_strVegaFromServer	=data.m_strVegaFromServer;			//�������� ���� ����

		
		for(int i = 0 ; i < HOGA_CNT ; i++)
		{
			m_BidArray[i] = data.m_BidArray[i];
			m_OfferArray[i]=data.m_OfferArray[i];
		}
		m_TotalBid		= data.m_TotalBid;
		m_TotalOffer	= data.m_TotalOffer;


		for(i = 0 ; i < m_ContractArray.GetSize(); i++) delete m_ContractArray.GetAt(i);
		m_ContractArray.RemoveAll();

		for(i = 0 ; i < data.m_ContractArray.GetSize(); i++) 
		{
			CContractSet * pNewSet = new CContractSet;
			*pNewSet = *data.m_ContractArray.GetAt(i);
			m_ContractArray.Add(pNewSet);
		}
	}
};

typedef CArray < CDataSet *, CDataSet *> CDataArray;
typedef CList  < CDataSet *, CDataSet *> CDataList;

class CCodeDataSet
{
public:
	CString m_strLastTradeDate;		//�����ŷ���
	CString m_strCodeInfo;			//�ڵ�����(��,��)
	CString m_strRemainDay;			//������
	CString m_strWorkRemainDay;		//����������
	CString m_strIndex;				//��������

public:

	CCodeDataSet & operator=(CCodeDataSet & data)
	{
		m_strLastTradeDate	= data.m_strLastTradeDate;
		m_strCodeInfo		= data.m_strCodeInfo;			//�ڵ�����(��,��)
		m_strRemainDay		= data.m_strRemainDay;			//������
		m_strIndex			= data.m_strIndex;				//��������
		return *this;
	}
};
typedef CArray <CCodeDataSet *, CCodeDataSet *> CCodeDataArray;



class CTradeDateInfo
{
public:
	CString m_strDate;		//��������
	int		m_nTradeDate;	//�����ϼ�
};
typedef CArray < CTradeDateInfo * ,CTradeDateInfo *> CTradeDateInfoArray;



//��츸�� Ư���� ���̽�....
class COptIVInfo
{
public:
	CString m_strUnderlyingAssetsCode;
	CString m_strIV;//��ճ��纯����->�̷а�,�ΰ��� ���Ҷ� �⺻������ ���Ǵ� �������̴�.(�������������� ������� �ʴ´�)
};
typedef CArray < COptIVInfo *, COptIVInfo *> COptIVInfoArray;

class DATASET_DECLSPEC CVolateInfo
{
public:
	CString m_strUnderlyingAssetsShortCode;
    CString m_strIV;
	CString m_strHisVolate30;
	CString m_strHisVolate60;
	CString m_strHisVolate90;    
	
	CString m_strTradeUnit;

public:

	CVolateInfo & operator=(CVolateInfo & volate)
	{
		m_strUnderlyingAssetsShortCode  = volate.m_strUnderlyingAssetsShortCode;
		m_strIV							= volate.m_strIV;
		m_strHisVolate30				= volate.m_strHisVolate30;
		m_strHisVolate60				= volate.m_strHisVolate60;
		m_strHisVolate90				= volate.m_strHisVolate90;
		m_strTradeUnit					= volate.m_strTradeUnit;
		return *this;
	}
};
typedef CArray < CVolateInfo * , CVolateInfo *> CVolateInfoArray;


class CBaseDataSet
{
public:
	CBaseDataSet(){m_pRTDataArray = NULL;}

	~CBaseDataSet()
	{
		int i, nSize = m_TradeDateInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_TradeDateInfoArray.GetAt(i);
		m_TradeDateInfoArray.RemoveAll();

		nSize = m_CodeDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_CodeDataArray.GetAt(i);
		m_CodeDataArray.RemoveAll();

		nSize = m_VolateInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_VolateInfoArray.GetAt(i);
		m_VolateInfoArray.RemoveAll();		
	}

	double GetOptIV(CString strCode)
	{
		if(strCode.GetLength() > 6 && (strCode.Left(1) == "2" || strCode.Left(1) == "3"))
		{
			for(int i = 0 ; i < m_VolateInfoArray.GetSize() ; i++)
			{
				CVolateInfo * pOpt = m_VolateInfoArray.GetAt(i);
				if(strCode.Mid(1,2) == pOpt->m_strUnderlyingAssetsShortCode)
					return atof(pOpt->m_strIV);
			}
		}
		return 0.;
	}


	CBaseDataSet & operator=(CBaseDataSet & set)
	{
		m_strCDRate					= set.m_strCDRate;

		m_strHistoricalVolate90		= set.m_strHistoricalVolate90;

		m_strHistoricalVolate60		= set.m_strHistoricalVolate60;

		m_strHistoricalVolate30		= set.m_strHistoricalVolate30;

		m_strTodayDate				= set.m_strTodayDate;	


		int i;
		for(i = 0 ; i < m_VolateInfoArray.GetSize(); i++) delete m_VolateInfoArray.GetAt(i);
		m_VolateInfoArray.RemoveAll();
		for(i = 0 ; i < set.m_VolateInfoArray.GetSize(); i++)
		{
			CVolateInfo * pVol    = set.m_VolateInfoArray.GetAt(i);
			CVolateInfo * pNewVol = new CVolateInfo;
			*pNewVol = *pVol;
			m_VolateInfoArray.Add(pNewVol);
		}
		
		for(i = 0 ; i < m_CodeDataArray.GetSize(); i++) delete m_CodeDataArray.GetAt(i);
		m_CodeDataArray.RemoveAll();
		for(i = 0 ; i < set.m_CodeDataArray.GetSize(); i++)
		{
			CCodeDataSet * pSet = set.m_CodeDataArray.GetAt(i);
			CCodeDataSet * pNewSet = new CCodeDataSet;
			*pNewSet = *pSet;
			m_CodeDataArray.Add(pNewSet);
		}


		for(i = 0 ; i < m_TradeDateInfoArray.GetSize(); i++) delete m_TradeDateInfoArray.GetAt(i);
		m_TradeDateInfoArray.RemoveAll();
		for(i = 0 ; i < set.m_TradeDateInfoArray.GetSize(); i++)
		{
			CTradeDateInfo * pSet = set.m_TradeDateInfoArray.GetAt(i);
			CTradeDateInfo * pNewSet = new CTradeDateInfo;
			*pNewSet = *pSet;
			m_TradeDateInfoArray.Add(pNewSet);
		}		

		for(i = 0 ; i < m_TradeDateInfoArray.GetSize(); i++) delete m_TradeDateInfoArray.GetAt(i);
		m_TradeDateInfoArray.RemoveAll();
		for(i = 0 ; i < set.m_TradeDateInfoArray.GetSize(); i++)
		{
			CTradeDateInfo * pSet = set.m_TradeDateInfoArray.GetAt(i);
			CTradeDateInfo * pNewSet = new CTradeDateInfo;
			*pNewSet = *pSet;
			m_TradeDateInfoArray.Add(pNewSet);
		}		

		if(m_pRTDataArray)
		{		
			for(i = 0 ; i < m_pRTDataArray->GetSize(); i++) delete m_pRTDataArray->GetAt(i);
			delete m_pRTDataArray;
			m_pRTDataArray = NULL;
		}

		if(set.m_pRTDataArray)
		{		
			m_pRTDataArray = new CDataArray;
			for(i = 0 ; i < set.m_pRTDataArray->GetSize(); i++)
			{
				CDataSet * pSet = set.m_pRTDataArray->GetAt(i);
				CDataSet* pNewSet = new CDataSet;
				*pNewSet = *pSet;
				m_pRTDataArray->Add(pNewSet);
			}	
		}
		return *this;
	}

public:

	CString				m_strCDRate;

	CString				m_strHistoricalVolate90;

	CString				m_strHistoricalVolate60;

	CString				m_strHistoricalVolate30;

	CString				m_strATM;

	CVolateInfoArray	m_VolateInfoArray;
	
	CCodeDataArray		m_CodeDataArray;

	CTradeDateInfoArray	m_TradeDateInfoArray;

	CString				m_strTodayDate;

	CDataArray	*		m_pRTDataArray;
};

class DATASET_DECLSPEC CItemDataInfo
{
public:
	
	CString m_strCode;
	CString m_strName;
	CString m_strStrike;
/////////////////////////////////
	CString m_strCurPrice;	//���簡
	CString m_strBid;		//�ż�ȣ��
	CString m_strOffer;		//�ŵ�ȣ��
	CString m_strVolume;	//�ŷ���
	CString m_strLastPrice; //��������
	CString m_strOfferVol;
	CString m_strBidVol;
//	CString m_strFlag;

	DWORD m_dwReserved;//�ַ� �����͸� ������ �� ����Ϸ� �Ѵ�.(�� : CFONetworkModule)

public:

	CItemDataInfo()
	{
		m_dwReserved = 0;
	}

	CItemDataInfo & operator=(CItemDataInfo & info)
	{
		m_strCode = info.m_strCode;
		m_strName = info.m_strName;
		m_strStrike=info.m_strStrike;

		m_strCurPrice	= info.m_strCurPrice;
		m_strBid		= info.m_strBid;
		m_strOffer		= info.m_strOffer;
		m_strVolume		= info.m_strVolume;
		m_strLastPrice  = info.m_strLastPrice;
		m_strOfferVol	= info.m_strOfferVol;
		m_strBidVol		= info.m_strBidVol;
//		m_strFlag		= info.m_strFlag;
		m_dwReserved	= info.m_dwReserved;	

		return *this;
	}
};
typedef CArray < CItemDataInfo *, CItemDataInfo *> CItemInfoArray;

class DATASET_DECLSPEC COptionMonthInfo
{
public:
	CString m_strYearMonth;
	CString m_strShortYearMonth;
	CItemInfoArray	m_CallOptionInfoArray;
	CItemInfoArray	m_PutOptionInfoArray;

public:
	COptionMonthInfo(){;}

	~COptionMonthInfo()
	{
		int i, nSize = m_CallOptionInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_CallOptionInfoArray.GetAt(i);
		m_CallOptionInfoArray.RemoveAll();

		nSize = m_PutOptionInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_PutOptionInfoArray.GetAt(i);
		m_PutOptionInfoArray.RemoveAll();
	}

	COptionMonthInfo & operator=(COptionMonthInfo & month)
	{
		m_strYearMonth = month.m_strYearMonth;
		int i,nSize = m_CallOptionInfoArray.GetSize();
		for(i = 0 ; i < nSize ;i ++) delete m_CallOptionInfoArray.GetAt(i);
		m_CallOptionInfoArray.RemoveAll();
		
		nSize = m_PutOptionInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++) delete m_PutOptionInfoArray.GetAt(i);
		m_PutOptionInfoArray.RemoveAll();

		nSize = month.m_CallOptionInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CItemDataInfo * pOption = month.m_CallOptionInfoArray.GetAt(i);
			CItemDataInfo * pNewOption = new CItemDataInfo;
			*pNewOption = *pOption;
			m_CallOptionInfoArray.Add(pNewOption);
		}

		nSize = month.m_PutOptionInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CItemDataInfo * pOption = month.m_PutOptionInfoArray.GetAt(i);
			CItemDataInfo * pNewOption = new CItemDataInfo;
			*pNewOption = *pOption;
			m_PutOptionInfoArray.Add(pNewOption);
		}
		return *this;
	}
};
typedef CArray < COptionMonthInfo *, COptionMonthInfo *> COptionMonthInfoArray;




class DATASET_DECLSPEC CUnderlyingAssetsInfo
{
public:
	
	CString m_strShortCode; //��������->10, �Ｚ����->11, .....
	CString m_strUnderlyingAssetsCode;
	CString m_strUnderlyingAssetsName;
	CString m_strTradeUnit;	

	COptionMonthInfoArray	m_MonthInfoArray;

	DWORD	m_dwReserved;
	
	CItemInfoArray			m_ItemArray; //�����ڻ��� kp200�� ���� ����4����������
	//�ֽ��� ���� �ֽ��� ����...

public:

	CUnderlyingAssetsInfo()
	{
		m_dwReserved = 0;
	}

	~CUnderlyingAssetsInfo()
	{
		int i, nSize = m_MonthInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_MonthInfoArray.GetAt(i);
		m_MonthInfoArray.RemoveAll();
		
		nSize = m_ItemArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_ItemArray.GetAt(i);
		m_ItemArray.RemoveAll();
	}

	CUnderlyingAssetsInfo & operator=(CUnderlyingAssetsInfo & under)
	{
		m_strShortCode = under.m_strShortCode;
		m_strUnderlyingAssetsCode = under.m_strUnderlyingAssetsCode;
		m_strUnderlyingAssetsName = under.m_strUnderlyingAssetsName;
		m_strTradeUnit			  = under.m_strTradeUnit;

		int i,nSize = m_MonthInfoArray.GetSize();
		for(i = 0 ; i < nSize ;i ++)		
			delete m_MonthInfoArray.GetAt(i);
		m_MonthInfoArray.RemoveAll();

		nSize = m_ItemArray.GetSize();
		for(i = 0 ; i < nSize ;i ++)		
			delete m_ItemArray.GetAt(i);
		m_ItemArray.RemoveAll();

		nSize = under.m_MonthInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			COptionMonthInfo * pMonth = under.m_MonthInfoArray.GetAt(i);
			COptionMonthInfo * pNewMonth = new COptionMonthInfo;
			*pNewMonth = *pMonth;
			m_MonthInfoArray.Add(pNewMonth);
		}

		nSize = under.m_ItemArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CItemDataInfo * pItem = under.m_ItemArray.GetAt(i);
			CItemDataInfo * pNewItem = new CItemDataInfo;
			*pNewItem = *pItem ;
			m_ItemArray.Add(pNewItem);
		}

		m_dwReserved = under.m_dwReserved;
		return *this;
	}
};
typedef CArray < CUnderlyingAssetsInfo *, CUnderlyingAssetsInfo *> CUnderlyingAssetsInfoArray;


//����� ���� �����
class DATASET_DECLSPEC CStrategyData
{
public:
	
	CString m_strATMPos;		// ATM ������ �Ÿ�
	CString m_strMonth;			// �ٿ� 0, ���ٿ� 1, �����ٿ� 2, �������ٿ� 3
	CString m_strCallPut;		// Call 0, Put 1
	CString m_strTradeType;		// �ŵ� 0, �ż� 1
	CString m_strQty;			// ����
	CString m_strCovedCall;		// Ŀ���� �� ����
	CString m_strBase;			// �������̵忡�� ��� �⺻ ���� �������� ������� �Ǻ�..
	DWORD	m_dwReserved;//�ַ� �����͸� ������ �� ����Ϸ� �Ѵ�.(�� : CFONetworkModule)

public:

	CStrategyData()
	{
		m_dwReserved = 0;
	}

	CStrategyData & operator=(CStrategyData & info)
	{
		m_strATMPos		= info.m_strATMPos;
		m_strMonth		= info.m_strMonth;
		m_strCallPut	= info.m_strCallPut;
		m_strTradeType	= info.m_strTradeType;
		m_strQty		= info.m_strQty;
		m_strCovedCall	= info.m_strCovedCall;
		m_strBase		= info.m_strBase;

		m_dwReserved	= info.m_dwReserved;	

		return *this;
	}
};
typedef CArray < CStrategyData *, CStrategyData *> CStrategyDataArray;

class DATASET_DECLSPEC CStrategyItemInfo
{
public:
	CString	m_strStrategyName;				// ������
	CStrategyDataArray m_StrategyDataArray;	// ���� ����Ÿ
	CString m_strStrategyMsg;				// ���� ����

public:
	CStrategyItemInfo(){;}

	~CStrategyItemInfo()
	{
		int i, nSize = m_StrategyDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_StrategyDataArray.GetAt(i);
		m_StrategyDataArray.RemoveAll();
	}

	CStrategyItemInfo & operator=(CStrategyItemInfo & StrategyName)
	{
		m_strStrategyName = StrategyName.m_strStrategyName;
		m_strStrategyMsg = StrategyName.m_strStrategyMsg;

		int i,nSize = m_StrategyDataArray.GetSize();
		for(i = 0 ; i < nSize ;i ++) delete m_StrategyDataArray.GetAt(i);
		m_StrategyDataArray.RemoveAll();
		
		nSize = StrategyName.m_StrategyDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CStrategyData * pData = StrategyName.m_StrategyDataArray.GetAt(i);
			CStrategyData * pNewData = new CStrategyData;
			*pNewData = *pData;
			m_StrategyDataArray.Add(pNewData);
		}
		return *this;
	}
};
typedef CArray < CStrategyItemInfo *, CStrategyItemInfo *> CStrategyItemInfoArray;

class DATASET_DECLSPEC CStrategyGroup
{
public:
	CString m_strGroupName;						// �׷��
	CStrategyItemInfoArray m_StrategyItemArray;	// �׷��� ����

public:
	CStrategyGroup(){;}

	~CStrategyGroup()
	{
		int i, nSize = m_StrategyItemArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_StrategyItemArray.GetAt(i);
		m_StrategyItemArray.RemoveAll();
	}

	CStrategyGroup & operator=(CStrategyGroup & GroupName)
	{
		m_strGroupName = GroupName.m_strGroupName;

		int i,nSize = m_StrategyItemArray.GetSize();
		for(i = 0 ; i < nSize ;i ++) delete m_StrategyItemArray.GetAt(i);
		m_StrategyItemArray.RemoveAll();
		
		nSize = GroupName.m_StrategyItemArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CStrategyItemInfo * pStrategy = GroupName.m_StrategyItemArray.GetAt(i);
			CStrategyItemInfo * pNewStrategy = new CStrategyItemInfo;
			*pNewStrategy = *pStrategy;
			m_StrategyItemArray.Add(pNewStrategy);
		}
		return *this;
	}
};
typedef CArray < CStrategyGroup *, CStrategyGroup *> CStrategyGroupArray;

////////////////////////////////////////////////////////
// �ܰ�..
/////////////////////////////////////////////////////////
//�����ɼǰ��� ����.
class DATASET_DECLSPEC CRemainDataSet 
{
public:
	CString m_strCode;					//�����ڵ�	
	CString m_strName;					//�����
	CString m_strTradeType;				//�ŵ� (1), �ż�(2);
	CString m_strRemainCount;			//�ܰ����
	CString m_strOrderEnableCount;		//û�갡�ɼ���.
	CString m_strAvePrice;				//��մܰ�
	CString m_strCreditType;			//�ſ뱸�� (�Ϲ�-0, ����-1, ����-2)
	CString m_strCoveredRemainCount;	//Ŀ���� �ܰ����
	CString m_strCoveredOrderEnableCount;//Ŀ���� û�갡�ɼ���
	CString m_strUnderlyingAssets;		//�����ڻ�
	CString m_strAveAmount;				//���Աݾ�(����ȣ �߰� 20080407)

	CDataSet m_DataSet;

public:
	CRemainDataSet & operator=(CRemainDataSet & data)
	{
		m_strCode				= data.m_strCode;
		m_strName				= data.m_strName;
		m_strTradeType			= data.m_strTradeType;
		m_strRemainCount		= data.m_strRemainCount;
		m_strOrderEnableCount	= data.m_strOrderEnableCount;
		m_strAvePrice			= data.m_strAvePrice;
		m_strCreditType			= data.m_strCreditType;
		m_strCoveredRemainCount = data.m_strCoveredRemainCount;
		m_strCoveredOrderEnableCount = data.m_strCoveredOrderEnableCount;
		m_strUnderlyingAssets   = data.m_strUnderlyingAssets;
		m_strAveAmount			= data.m_strAveAmount;//���Աݾ�(����ȣ �߰� 20080407)
		m_DataSet				= data.m_DataSet;
		return *this;
	}
};
typedef CArray< CRemainDataSet *, CRemainDataSet *> CRemainDataArray;

class DATASET_DECLSPEC CAccountInfo
{
public:	

	//����/�ɼ�...
	CString				m_strFOAccountName;		//���¸�
	CString				m_strFOOrderEnableTotal;	//�ֹ������Ѿ�
	CString				m_strFOOrderEnableMoney;	//�ֹ���������
	CString				m_strFOOrderRealBuyMoney;	//���ż��ݾ�
	CRemainDataArray	m_FORemainDataArray;

	//�ֽ�..
	CString				m_strSAccountName;
	CRemainDataArray	m_SRemainDataArray;

public:
	CAccountInfo(){;}
	~CAccountInfo()
	{
		DeleteArray();
	}

	void DeleteArray()
	{
		int i, nSize = m_FORemainDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)		
			delete m_FORemainDataArray.GetAt(i);
		m_FORemainDataArray.RemoveAll();

		nSize = m_SRemainDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_SRemainDataArray.GetAt(i);
		m_SRemainDataArray.RemoveAll();
	}

	CAccountInfo & operator=(CAccountInfo & account)
	{
		m_strFOAccountName = account.m_strFOAccountName;
		m_strFOOrderEnableTotal = account.m_strFOOrderEnableTotal;
		m_strFOOrderEnableMoney = account.m_strFOOrderEnableMoney;
		m_strFOOrderRealBuyMoney= account.m_strFOOrderRealBuyMoney;

		m_strSAccountName       = account.m_strSAccountName;
		
		int i,nSize = m_FORemainDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)		
			delete m_FORemainDataArray.GetAt(i);
		m_FORemainDataArray.RemoveAll();

		nSize = account.m_FORemainDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CRemainDataSet * pDataSet = account.m_FORemainDataArray.GetAt(i);
			CRemainDataSet * pNewDataSet = new CRemainDataSet;
			*pNewDataSet = *pDataSet;
			m_FORemainDataArray.Add(pNewDataSet);
		}
		
		nSize = m_SRemainDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_SRemainDataArray.GetAt(i);
		m_SRemainDataArray.RemoveAll();

		nSize = account.m_SRemainDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CRemainDataSet * pDataSet = account.m_SRemainDataArray.GetAt(i);
			CRemainDataSet * pNewDataSet = new CRemainDataSet;
			*pNewDataSet = *pDataSet;
			m_SRemainDataArray.Add(pNewDataSet);
		}

		return *this;
	}
};

// SJS ���� (2005.09.06)
/******************* ���ű� ������� �̷а� �������� ************************/
// ��Ź ���ű� : 11���� -> 31����
// ���� ���ű� : 11���� -> 21����
/****************************************************************************/
#define DANGER_THEORY_COUNT	31
#define	HAVE_THEORY_COUNT	21

#define	THEORY_COUNT 11

class DATASET_DECLSPEC CTheoryForMargin
{
public:
	int				m_nDataType;			//���ϵ�����-1, ���� - 2
	
	int				m_nID;				//���̵�.

	CString			m_strCode;
	CString			m_strUnit;
	CString			m_strOffSet;	
	CString			m_strOffSet1;
	CString			m_strAdjSect;		//
	CString			m_strProductID;
	CString			m_strProductID1;
	CString			m_strUnderlyingAssetsShortCode;
	CString			m_strStrike;
	CString			m_strAdjPractice;
	

	double m_dDangerTheoryArray[DANGER_THEORY_COUNT];
	double m_dHaveTheoryArray[HAVE_THEORY_COUNT];

	double m_dDangerTheoryArrayNext[DANGER_THEORY_COUNT];
	double m_dHaveTheoryArrayNext[HAVE_THEORY_COUNT];

public:

	CTheoryForMargin(){;}

	CTheoryForMargin & operator=(CTheoryForMargin & theory)
	{
		m_nDataType = theory.m_nDataType;
		m_strCode   = theory.m_strCode;
		m_nID		= theory.m_nID;
		m_strUnit	= theory.m_strUnit;
		m_strOffSet = theory.m_strOffSet;
		m_strOffSet1= theory.m_strOffSet1;
		m_strAdjSect= theory.m_strAdjSect;
		m_strProductID=theory.m_strProductID;
		m_strProductID1=theory.m_strProductID1;
		m_strUnderlyingAssetsShortCode = theory.m_strUnderlyingAssetsShortCode;
		m_strStrike = theory.m_strStrike;
		m_strAdjPractice = theory.m_strAdjPractice;
		
		memcpy(m_dDangerTheoryArray, theory.m_dDangerTheoryArray, sizeof(m_dDangerTheoryArray));
		memcpy(m_dHaveTheoryArray,   theory.m_dHaveTheoryArray,   sizeof(m_dHaveTheoryArray));

		memcpy(m_dDangerTheoryArrayNext, theory.m_dDangerTheoryArrayNext, sizeof(m_dDangerTheoryArrayNext));
		memcpy(m_dHaveTheoryArrayNext, theory.m_dHaveTheoryArrayNext, sizeof(m_dHaveTheoryArrayNext));

		return *this;
	}
};
typedef CArray < CTheoryForMargin *, CTheoryForMargin *> CTheoryForMarginArray;



class CItemSet
{
public:
	CString m_strCode;
	CString m_strName;
	CString m_strCount;
	CString m_strTradeType;
	CString m_strAvePrice;
public:
	CItemSet & operator=(CItemSet & item)
	{
		m_strCode = item.m_strCode;
		m_strName = item.m_strName;
		m_strCount= item.m_strCount;
		m_strTradeType = item.m_strTradeType;
		m_strAvePrice = item.m_strAvePrice;
		return *this;
	}
};
typedef CArray < CItemSet *, CItemSet *> CItemArray;


#define	NEWADD						1	//�߰�
#define	APPEND						2	//����

class CPassDataSet
{
public:

	CString		m_strAccount;

	CString		m_strPassword;

	CString		m_strUnderlyingAssetsCode;

	CString		m_strUnderlyingAssetsName;	

	int			m_nMode;			//�߰� ��� : NEWADD, ������ : APPEND

	LONGLONG	m_lProfit;			//Ȯ������

	CDataArray	m_DataArray;	

    CString     m_strUnderlyingStartPrice;      // KOSPI200 ���� ����
	CString		m_strUnderlyingEndPrice;		// KOSPI200 �� ����
	
public:
	CPassDataSet(){	m_lProfit = 0, m_nMode = NEWADD;}

	~CPassDataSet()
	{
		for(int i = 0 ; i < m_DataArray.GetSize();  i++) delete m_DataArray.GetAt(i);		
	}
	
	CPassDataSet & operator=(CPassDataSet & data)
	{
		m_strAccount				= data.m_strAccount;
		m_strPassword				= data.m_strPassword;
		m_strUnderlyingAssetsCode	= data.m_strUnderlyingAssetsCode;
		m_strUnderlyingAssetsName	= data.m_strUnderlyingAssetsName;
		m_nMode						= data.m_nMode;
		m_lProfit					= data.m_lProfit;
		
		int i ; 
		for(i = 0 ; i < m_DataArray.GetSize(); i++) delete m_DataArray.GetAt(i);
		m_DataArray.RemoveAll();

		for(i = 0 ; i < data.m_DataArray.GetSize(); i++)
		{
			CDataSet * pDataSet = data.m_DataArray.GetAt(i);
			CDataSet * pNewSet  = new CDataSet ;
			*pNewSet = *pDataSet;
			m_DataArray.Add(pNewSet);
		}
		return *this;
	}
};

class CUserFileSaveData
{
public:
	CString m_strSaveName;

	CPassDataSet m_PassDataSet;
public:

	CUserFileSaveData(){;}

	CUserFileSaveData & operator=(CUserFileSaveData & item)
	{
		m_strSaveName	= item.m_strSaveName;
		
		m_PassDataSet	= item.m_PassDataSet;
		return *this;
	}
};
typedef CArray < CUserFileSaveData *, CUserFileSaveData *> CUserFileSaveDataArray;

class DATASET_DECLSPEC CUserDataFile
{
public:
	CString m_strGroupName;						// �׷��
	CUserFileSaveDataArray m_UserFileDataArray;	// �׷��� ����

public:
	CUserDataFile(){;}

	~CUserDataFile()
	{
		int i, nSize = m_UserFileDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_UserFileDataArray.GetAt(i);
		m_UserFileDataArray.RemoveAll();
	}

	CUserDataFile & operator=(CUserDataFile & GroupName)
	{
		m_strGroupName = GroupName.m_strGroupName;

		int i,nSize = m_UserFileDataArray.GetSize();
		for(i = 0 ; i < nSize ;i ++) delete m_UserFileDataArray.GetAt(i);
		m_UserFileDataArray.RemoveAll();
		
		nSize = GroupName.m_UserFileDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CUserFileSaveData * pStrategy = GroupName.m_UserFileDataArray.GetAt(i);
			CUserFileSaveData * pNewStrategy = new CUserFileSaveData;
			*pNewStrategy = *pStrategy;
			m_UserFileDataArray.Add(pNewStrategy);
		}
		return *this;
	}
};
typedef CArray < CUserDataFile *, CUserDataFile *> CUserDataFileArray;

#define	THEORY_SETUP_UNDERLYING_FOCUS	1

class CSetupDataSet
{
public:

	int	m_nAvePrice; // �����Է°� ���� - 0, �ڵ����� - 1
	
	int	m_nGreek;	 //������������ ���� - 0, �Է��� �������� ��� - 1
	
	int m_nProfitType; //���簡 ��� - 0, �̷а� ��� - 1, ���Ⱑ ��� - 2
	
	int m_nAddItem;    //1���� �߰� - 0, �������� �߰� - 1
	
	int m_nAddSameItemType; // ���ο� �ٷ� �߰� - 0, ��հ�, �����ջ� �� �� �ٷ� ǥ��-1
	
	BOOL m_bAddCommission;  //�����Ḧ ���Ϳ� �ݿ�
	
	double m_dFutureCom;	//���� ��������(%����)
	
	double m_dOptionCom;	//�ɼ� ��������(%����)
	
	double	m_dStockCom;    //�ֽ� ��������(%����)

	BOOL	m_bSort;//���Ŀ���.

	int		m_nSort;	//�������Ǻ�/����/��簡�� - 0
						//�������Ǻ�/��簡��/���� - 1
						//����/�������Ǻ�/��簡�� - 2
						//����/��簡��/�������Ǻ� - 3
						//��簡��/�������Ǻ�/���� - 4
						//��簡��/����/�������Ǻ� - 5

	int		m_nSortType; //��������-0, ��������-1

	double	m_dUnderlyingAssetsMin; //�����ڻ� �ּҰ�

	double	m_dUnderlyingAssetsMax; //�����ڻ� �ִ밪

	int		m_nYRange; // �ڵ����� - 0, �������� - 1

	double	m_dYMax;   // �ִ�y��

	double	m_dYMin;	//�ּ�y��

	int		m_nTradeInputChange; //�ŵ� / �ż� �Է� ���  0 - �޺��ڽ� , 1 - ��۹��

public:

	CSetupDataSet()
	{
		m_bSort = FALSE;
	}

	CSetupDataSet & operator=(CSetupDataSet & data)
	{
		m_nAvePrice			= data.m_nAvePrice;		
		m_nGreek			= data.m_nGreek;
		m_nProfitType		= data.m_nProfitType;
		m_nAddItem			= data.m_nAddItem;
		m_nAddSameItemType	= data.m_nAddSameItemType;
		m_bAddCommission	= data.m_bAddCommission;
		m_dFutureCom		= data.m_dFutureCom;
		m_dOptionCom		= data.m_dOptionCom;
		m_dStockCom			= data.m_dStockCom;
		m_bSort				= data.m_bSort;
		m_nSort				= data.m_nSort;
		m_nSortType			= data.m_nSortType;
		m_dUnderlyingAssetsMin=data.m_dUnderlyingAssetsMin;
		m_dUnderlyingAssetsMax=data.m_dUnderlyingAssetsMax;
		m_nYRange			= data.m_nYRange;
		m_dYMax				= data.m_dYMax;
		m_dYMin				= data.m_dYMin;
		m_nTradeInputChange = data.m_nTradeInputChange;
		return *this;
	}
};

// Add By LJP 20080123 Start -->
class CSetupTheorySet
{
public:
	int		m_nBasicPrice;	///< 0 : KOSPI200���� - �������, 1 : �����ֱٿ���, 2: �ڵ����, 3 : ����
	
	int		m_nKOSPI200;	///< ���� KOSPI200 ����
	int		m_nFuture;		///< ���� ����

	int		m_nIVType;		///< 0 : ����� ���纯����, 1 : ������������ 90��, 2 : ���纯����, 3 : ����ڼ���

	double	m_dCallIV;		///< ��������� �� ������	
	double	m_dPutIV;		///< ��������� ǲ ������
	
	
	int		m_nRemainDay;	///< 0 : �ŷ��ϼ� ����, 1 : �޷��ϼ� ����
	BOOL	m_bTimeValue;	///< ���� �ð���ġ ���Ҹ� �ݿ�
	CString	m_strFromTime;	///< �ð���ġ ���ҽð��� ���� �ð�
	CString	m_strToTime;	///< �ð���ġ ���ҽð��� �� �ð�
	int		m_nFocusIndex;//���ذ� ��ư�� Ŭ���� ȯ�漳��â�� ��Ŀ���� �̷а��ǿ� ���ذ��� ������ �Ѵ�.(m_nFocusIndex -> 1)

public:

	CSetupTheorySet() {m_nFocusIndex = 0;}

	CSetupTheorySet & operator=(CSetupTheorySet & data)
	{
		m_nBasicPrice	= data.m_nBasicPrice;
		m_nKOSPI200		= data.m_nKOSPI200;
		m_nFuture		= data.m_nFuture;
		m_nIVType		= data.m_nIVType;
		m_dCallIV		= data.m_dCallIV;
		m_dPutIV		= data.m_dPutIV;
		m_nRemainDay	= data.m_nRemainDay;
		m_bTimeValue	= data.m_bTimeValue;
		m_strFromTime	= data.m_strFromTime;
		m_strToTime		= data.m_strToTime;
		m_nFocusIndex = data.m_nFocusIndex;
		return *this;
	}

	BOOL LoadTheoryInitFile(CString strUserPath)
	{		
		CString sFileName;
		TCHAR Temp[100];		

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_01, "0", Temp, sizeof(Temp), sFileName); 
		m_nBasicPrice = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_01_1, "100", Temp, sizeof(Temp), sFileName); 
		m_nKOSPI200 = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_01_2, "0", Temp, sizeof(Temp), sFileName); 
		m_nFuture = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_02, "0", Temp, sizeof(Temp), sFileName); 
		m_nIVType = atoi(Temp);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_02_1, "0", Temp, sizeof(Temp), sFileName); 
		m_dCallIV = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_02_2, "0", Temp, sizeof(Temp), sFileName); 
		m_dPutIV = atoi(Temp);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_03, "0", Temp, sizeof(Temp), sFileName); 
		m_nRemainDay = atoi(Temp);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_04, "0", Temp, sizeof(Temp), sFileName); 
		m_bTimeValue = atoi(Temp);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_04_1, "0900", Temp, sizeof(Temp), sFileName); 
		m_strFromTime = Temp;
		
		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_04_2, "1515", Temp, sizeof(Temp), sFileName); 
		m_strToTime = Temp;

		return TRUE;
	}

	void SaveTheoryInitFile(CString strUserPath)
	{
		CString sFileName;
		CString sTemp;

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);	

		sTemp.Format("%d", m_nBasicPrice);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_01, sTemp, sFileName);

		sTemp.Format("%d", m_nKOSPI200);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_01_1, sTemp, sFileName);

		sTemp.Format("%d", m_nFuture);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_01_2, sTemp, sFileName);

		sTemp.Format("%d", m_nIVType);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_02, sTemp, sFileName);

		sTemp.Format("%.4f", m_dCallIV);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_02_1, sTemp, sFileName);

		sTemp.Format("%.4f", m_dPutIV);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_02_2, sTemp, sFileName);

		sTemp.Format("%d", m_nRemainDay);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_03, sTemp, sFileName);

		sTemp.Format("%d", m_bTimeValue);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_04, sTemp, sFileName);

		sTemp.Format("%s", m_strFromTime);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_04_1, sTemp, sFileName);

		sTemp.Format("%s", m_strToTime);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_04_2, sTemp, sFileName);	
	}
};

class CSetupFOOrderSet
{
public:
	int		m_nPriceTypeSell;	///< �ŵ��ֹ� ���ݱ���(������-0, ���尡-1, ���Ǻ�������-2, ������������-3)
	int		m_nCPriceTypeSell;	///< �ŵ��ֹ� �ܰ�����(���簡-0, �ŵ�ȣ��-1, �ż�ȣ��-2, ���Ѱ�-3, ���Ѱ�-4)
	int		m_nTickSell;		///< �ŵ�ȣ������

	
	int		m_nPriceTypeBuy;	///< �ż��ֹ� ���ݱ���(������-0, ���尡-1, ���Ǻ�������-2, ������������-3)
	int		m_nCPriceTypeBuy;	///< �ż��ֹ� �ܰ�����(���簡-0, �ŵ�ȣ��-1, �ż�ȣ��-2, ���Ѱ�-3, ���Ѱ�-4)
	int		m_nTickBuy;			///< �ż�ȣ������ 

public:
	CSetupFOOrderSet(){}

	CSetupFOOrderSet & operator=(CSetupFOOrderSet & data)
	{
		m_nPriceTypeSell	= data.m_nPriceTypeSell;
		m_nCPriceTypeSell	= data.m_nCPriceTypeSell;
		m_nTickSell			= data.m_nTickSell;
		m_nPriceTypeBuy		= data.m_nPriceTypeBuy;
		m_nCPriceTypeBuy	= data.m_nCPriceTypeBuy;
		m_nTickBuy			= data.m_nTickBuy;
		return *this;
	}

	BOOL LoadOrderInitFile(CString strUserPath)
	{		
		CString sFileName;
		TCHAR Temp[100];

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_01, "0", Temp, sizeof(Temp), sFileName); 
		m_nPriceTypeSell = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_02, "1", Temp, sizeof(Temp), sFileName); 
		m_nCPriceTypeSell = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_03, "0", Temp, sizeof(Temp), sFileName); 
		m_nTickSell = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_04, "0", Temp, sizeof(Temp), sFileName); 
		m_nPriceTypeBuy = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_05, "1", Temp, sizeof(Temp), sFileName); 
		m_nCPriceTypeBuy = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_06, "0", Temp, sizeof(Temp), sFileName); 
		m_nTickBuy = atoi(Temp);

		return TRUE;
	}

	void SaveOrderInitFile(CString strUserPath)
	{
		CString sFileName;
		CString sTemp;		

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);	

		sTemp.Format("%d", m_nPriceTypeSell);
		WritePrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_01, sTemp, sFileName);

		sTemp.Format("%d", m_nCPriceTypeSell);
		WritePrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_02, sTemp, sFileName);

		sTemp.Format("%d", m_nTickSell);
		WritePrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_03, sTemp, sFileName);

		sTemp.Format("%d", m_nPriceTypeBuy);
		WritePrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_04, sTemp, sFileName);

		sTemp.Format("%d", m_nCPriceTypeBuy);
		WritePrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_05, sTemp, sFileName);
		
		sTemp.Format("%d", m_nTickBuy);
		WritePrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_06, sTemp, sFileName);
	}
};

class CSetupSlippageZeroOrderSet
{
public:
	int		m_nFirstTrade;		///< 0 : ȣ������ ū ����, 1 : ���簡 ���� ����
	
	int		m_nFirstTradeSeq;	///< 0 : ��� �켱�ŷ� �����ֹ�, 1 : #1 Set�� �켱�ŷ����� �����ֹ�
	
	int		m_nFirstTradeSell;	///< 0 : 1���ŵ�ȣ��, 1 : 2���ŵ�ȣ��, 2 : 3���ŵ�ȣ��, 3 : 4���ŵ�ȣ��, 4 : 5���ŵ�ȣ��, 5 : �����������, 6 : ������������, 7 : ���簡, 8 : 1���ż�ȣ��, 9 : 2���ż�ȣ��, 10 : 3���ż�ȣ��, 11 : 4���ż�ȣ��, 12 : 5���ż�ȣ��, 13 : ���尡
	int		m_nFirstTradeBuy;	///< 0 : 1���ż�ȣ��, 1 : 2���ż�ȣ��, 2 : 3���ż�ȣ��, 3 : 4���ż�ȣ��, 4 : 5���ż�ȣ��, 5 : �����������, 6 : ������������, 7 : ���簡, 8 : 1���ŵ�ȣ��, 9 : 2���ŵ�ȣ��, 10 : 3���ŵ�ȣ��, 11 : 4���ŵ�ȣ��, 12 : 5���ŵ�ȣ��, 13 : ���尡
	
	int		m_nSecondTradeSell;	///< 0 : 1���ż�ȣ��, 1 : 2���ż�ȣ��, 2 : 3���ż�ȣ��, 3 : 4���ż�ȣ��, 4 : 5���ż�ȣ��, 5 : ���尡, 6 : ������������, 7 : �����������, 8 : ���簡, 9 : 1���ŵ�ȣ��, 10 : 2���ŵ�ȣ��, 11 : 3���ŵ�ȣ��, 12 : 4���ŵ�ȣ��, 13 : 5���ŵ�ȣ��
	int		m_nSecondTradeBuy;	///< 0 : 1���ŵ�ȣ��, 1 : 2���ŵ�ȣ��, 2 : 3���ŵ�ȣ��, 3 : 4���ŵ�ȣ��, 4 : 5���ŵ�ȣ��, 5 : ���尡, 6 : ������������, 7 : �����������, 8 : ���簡, 9 : 1���ż�ȣ��, 10 : 2���ż�ȣ��, 11 : 3���ż�ȣ��, 12 : 4���ż�ȣ��, 13 : 5���ż�ȣ��

	int		m_nTick;			///< ������� ����
	BOOL	m_bCancelOrder;		///< �켱�ŷ� �ڵ� �ֹ���� ����

public:
	CSetupSlippageZeroOrderSet(){}
	CSetupSlippageZeroOrderSet & operator=(CSetupSlippageZeroOrderSet & data)
	{
		m_nFirstTrade		= data.m_nFirstTrade;
		m_nFirstTradeSeq	= data.m_nFirstTradeSeq;
		m_nFirstTradeSell	= data.m_nFirstTradeSell;
		m_nFirstTradeBuy	= data.m_nFirstTradeBuy;
		m_nSecondTradeSell	= data.m_nSecondTradeSell;
		m_nSecondTradeBuy	= data.m_nSecondTradeBuy;
		m_nTick				= data.m_nTick;
		m_bCancelOrder		= data.m_bCancelOrder;
		return *this;
	}

	
	BOOL LoadSlippageInitFile(CString strUserPath)
	{
		CString sFileName;
		TCHAR Temp[100];

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_01, "0", Temp, sizeof(Temp), sFileName); 
		m_nFirstTrade = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_02, "0", Temp, sizeof(Temp), sFileName); 
		m_nFirstTradeSeq = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_03, "0", Temp, sizeof(Temp), sFileName); 
		m_nFirstTradeSell = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_04, "0", Temp, sizeof(Temp), sFileName); 
		m_nFirstTradeBuy = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_05, "0", Temp, sizeof(Temp), sFileName); 
		m_nSecondTradeSell = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_06, "0", Temp, sizeof(Temp), sFileName); 
		m_nSecondTradeBuy = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_07, "0", Temp, sizeof(Temp), sFileName); 
		m_nTick = atoi(Temp);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_07_1, "0", Temp, sizeof(Temp), sFileName); 
		m_bCancelOrder = atoi(Temp);

		return TRUE;
	}

	void SaveSlippageInitFile(CString strUserPath)
	{
		CString sFileName;
		CString sTemp;
		
		sFileName.Format("%s\\%s", strUserPath, FILE_INI);	

		sTemp.Format("%d", m_nFirstTrade);
		WritePrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_01, sTemp, sFileName);

		sTemp.Format("%d", m_nFirstTradeSeq);
		WritePrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_02, sTemp, sFileName);

		sTemp.Format("%d", m_nFirstTradeSell);
		WritePrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_03, sTemp, sFileName);

		sTemp.Format("%d", m_nFirstTradeBuy);
		WritePrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_04, sTemp, sFileName);

		sTemp.Format("%d", m_nSecondTradeSell);
		WritePrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_05, sTemp, sFileName);

		sTemp.Format("%d", m_nSecondTradeBuy);
		WritePrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_06, sTemp, sFileName);

		sTemp.Format("%d", m_nTick);
		WritePrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_07, sTemp, sFileName);

		sTemp.Format("%d", m_bCancelOrder);
		WritePrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_07_1, sTemp, sFileName);
	}

	
};

class CSetupDisplaySet
{
public:
	int		m_nAvgPrice;		///< 0 : �����Է°� ����, 1 : �ڵ�����
	int		m_nGreekDisplay;	///< 0 : ���� ���� ����(1������� ���), 1: �Է��� �������� ���
	
	int		m_nAddSameCode;		///< 0 : ���ο� �ٷ� �߰�, 1 : ��հ�, ���� �ջ� �� �� �ٷ� ǥ��
	
	BOOL	m_bSort;			///< ���Ŀ���
	int		m_nSort;			///< 0 : �������Ǻ�/����/��簡��
								///< 1 : �������Ǻ�/��簡��/����
								///< 2 : ����/�������Ǻ�/��簡��
								///< 3 : ����/��簡��/�������Ǻ�
								///< 4 : ��簡��/�������Ǻ�/����
								///< 5 : ��簡��/����/�������Ǻ�
	int		m_nSortType;		///< 0 : ��������, 1 : ��������
	
	double	m_dXAxisMin;		///< �����ڻ� �ּҰ�
	double	m_dXAxisMax;		///< �����ڻ� �ִ밪
	int		m_nYRange;			///< 0: �ڵ�����, 1 : ��������
	double	m_dYMax;			///< �ִ� y��
	double	m_dYMin;			///< �ּ� y��
	
	int		m_nRemainDisplay;	///< 0 : ���� �ܰ� ����, 1 : û�� ���� ����
	int		m_nProfitType;		///< 0 : ���밡, 1 : �̷а�, 2 : ���Ⱑ
public:
	CSetupDisplaySet()
	{
		m_nAvgPrice		= 0;
		m_nGreekDisplay = 0;
		m_nAddSameCode	= 0;
		m_bSort			= FALSE;
		m_nSort			= 0;	
		m_nSortType		= 0;
		m_dXAxisMin		= 0.0;
		m_dXAxisMax		= 0.0;
		m_nYRange		= 0;
		m_dYMax			= 0.0;
		m_dYMin			= 0.0;
		m_nRemainDisplay= 0;
		m_nProfitType	= 0;
	}
	CSetupDisplaySet & operator=(CSetupDisplaySet & data)
	{
		m_nAvgPrice			= data.m_nAvgPrice;
		m_nGreekDisplay		= data.m_nGreekDisplay;
		m_nAddSameCode		= data.m_nAddSameCode;
		m_bSort				= data.m_bSort;
		m_nSort				= data.m_nSort;
		m_nSortType			= data.m_nSortType;

		m_dXAxisMin			= data.m_dXAxisMin;
		m_dXAxisMax			= data.m_dXAxisMax;
		m_nYRange			= data.m_nYRange;
		m_dYMax				= data.m_dYMax;
		m_dYMin				= data.m_dYMin;
		m_nRemainDisplay	= data.m_nRemainDisplay;
		m_nProfitType		= data.m_nProfitType;
		return *this;
	}

	BOOL LoadDisplayInitFile(CString strUserPath)
	{		
		CString sFileName;
		TCHAR Temp[100];

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_01, "0", Temp, sizeof(Temp), sFileName); 
		m_nAvgPrice = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_02, "0", Temp, sizeof(Temp), sFileName); 
		m_nGreekDisplay = atoi(Temp);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_03, "0", Temp, sizeof(Temp), sFileName); 
		m_nAddSameCode = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_04, "0", Temp, sizeof(Temp), sFileName); 
		m_bSort = atoi(Temp);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_04_1, "0", Temp, sizeof(Temp), sFileName); 
		m_nSort = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_04_2, "0", Temp, sizeof(Temp), sFileName); 
		m_nSortType = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_05, "0", Temp, sizeof(Temp), sFileName); 
		m_dXAxisMin = atof(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_06, "0", Temp, sizeof(Temp), sFileName); 
		m_dXAxisMax = atof(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_07, "0", Temp, sizeof(Temp), sFileName); 
		m_nYRange = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_07_1, "0", Temp, sizeof(Temp), sFileName); 
		m_dYMax = atof(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_07_2, "0", Temp, sizeof(Temp), sFileName); 
		m_dYMin = atof(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_08, "0", Temp, sizeof(Temp), sFileName); 
		m_nRemainDisplay = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_09, "0", Temp, sizeof(Temp), sFileName); 
		m_nProfitType = atoi(Temp);

		return TRUE;
	}

	void SaveDisplayInitFile(CString strUserPath)
	{	
		CString sFileName;
		CString sTemp;

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);	

		sTemp.Format("%d", m_nAvgPrice);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_01, sTemp, sFileName);

		sTemp.Format("%d", m_nGreekDisplay);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_02, sTemp, sFileName);

		sTemp.Format("%d", m_nAddSameCode);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_03, sTemp, sFileName);

		sTemp.Format("%d", m_bSort);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_04, sTemp, sFileName);

		sTemp.Format("%d", m_nSort);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_04_1, sTemp, sFileName);

		sTemp.Format("%d", m_nSortType);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_04_2, sTemp, sFileName);

		sTemp.Format("%.2f", m_dXAxisMin);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_05, sTemp, sFileName);
		
		sTemp.Format("%.2f", m_dXAxisMax);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_06, sTemp, sFileName);

		sTemp.Format("%d", m_nYRange);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_07, sTemp, sFileName);

		sTemp.Format("%.2f", m_dYMax);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_07_1, sTemp, sFileName);

		sTemp.Format("%.2f", m_dYMin);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_07_2, sTemp, sFileName);

		sTemp.Format("%d", m_nRemainDisplay);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_08, sTemp, sFileName);

		sTemp.Format("%d", m_nProfitType);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_09, sTemp, sFileName);
	}
};

class CSetupEtcSet
{
public:
	BOOL	m_bAddCommission;		///< �����Ḧ ���Ϳ� �ݿ�	
	double	m_dFutureCom;			///< ���� ��������(%����)	
	double	m_dOptionCom;			///< �ɼ� ��������(%����)	
	double	m_dStockCom;			///< �ֽ� ��������(%����)

	BOOL	m_bApplyRealBuy;		///< ���ż������ ���űݰ�꿡 ����.
	
	int		m_nTradeInputChange;	///< �ż�/�ŵ� �Է� ���  0 : �޺��ڽ� , 1 : ��۹��
	
	int		m_nCPrice;				///< 0 : ���簡, 1 : ���ȣ��
//	BOOL	m_bOrderEnableHoga;		///< û����� �켱ȣ�� 

	BOOL	m_bApplyPredictPrice;	///< ����ü�ᰡ ���� ����

	int		m_nIV;					///< 0 : �����ϼ� ���� �ݿ�, 1 : �����ϼ� ���� �̹ݿ�
public:
	CSetupEtcSet(){}

	CSetupEtcSet & operator=(CSetupEtcSet & data)
	{
		m_bAddCommission		= data.m_bAddCommission;
		m_dFutureCom			= data.m_dFutureCom;
		m_dOptionCom			= data.m_dOptionCom;
		m_dStockCom				= data.m_dStockCom;
		m_bApplyRealBuy			= data.m_bApplyRealBuy;
		m_nTradeInputChange		= data.m_nTradeInputChange;
		m_nCPrice				= data.m_nCPrice;
//		m_bOrderEnableHoga		= data.m_bOrderEnableHoga;
		m_bApplyPredictPrice	= data.m_bApplyPredictPrice;
		m_nIV					= data.m_nIV;
		return *this;
	}

	BOOL LoadEtcInitFile(CString strUserPath)
	{
		CString sFileName;
		TCHAR Temp[100];

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_01, "0", Temp, sizeof(Temp), sFileName); 
		m_bAddCommission = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_01_1, "0", Temp, sizeof(Temp), sFileName); 
		m_dFutureCom = atof(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_01_2, "0", Temp, sizeof(Temp), sFileName); 
		m_dOptionCom = atof(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_01_3, "0", Temp, sizeof(Temp), sFileName); 
		m_dStockCom = atof(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_02, "0", Temp, sizeof(Temp), sFileName); 
		m_bApplyRealBuy = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_03, "0", Temp, sizeof(Temp), sFileName); 
		m_nTradeInputChange = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_04, "0", Temp, sizeof(Temp), sFileName); 
		m_nCPrice = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_05, "0", Temp, sizeof(Temp), sFileName); 
		m_bApplyPredictPrice = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_06, "0", Temp, sizeof(Temp), sFileName); 
		m_nIV = atoi(Temp);

		return TRUE;
	}

	void SaveEtcInitFile(CString strUserPath)
	{	
		CString sFileName;
		CString sTemp;

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);	

		sTemp.Format("%d", m_bAddCommission);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_01, sTemp, sFileName);

		sTemp.Format("%.2f", m_dFutureCom);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_01_1, sTemp, sFileName);

		sTemp.Format("%.2f", m_dOptionCom);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_01_2, sTemp, sFileName);

		sTemp.Format("%.2f", m_dStockCom);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_01_3, sTemp, sFileName);

		sTemp.Format("%d", m_bApplyRealBuy);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_02, sTemp, sFileName);

		sTemp.Format("%d", m_nTradeInputChange);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_03, sTemp, sFileName);

		sTemp.Format("%d", m_nCPrice);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_04, sTemp, sFileName);

		sTemp.Format("%d", m_bApplyPredictPrice);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_05, sTemp, sFileName);

		sTemp.Format("%d", m_nIV);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_06, sTemp, sFileName);
	}
};
// <-- End

class CSetupSet
{
public:
	CSetupTheorySet*			m_pTheorySet;
	CSetupFOOrderSet*			m_pFOOrderSet;
	CSetupSlippageZeroOrderSet*	m_pSlippageZeroSet;
	CSetupDisplaySet*			m_pDisplaySet;
	CSetupEtcSet*				m_pEtcSet;

public:

	CSetupSet()
	{
		m_pTheorySet		= new CSetupTheorySet;
		m_pFOOrderSet		= new CSetupFOOrderSet;
		m_pSlippageZeroSet	= new CSetupSlippageZeroOrderSet;
		m_pDisplaySet		= new CSetupDisplaySet;
		m_pEtcSet			= new CSetupEtcSet;
	}

	void SetSetupTheorySet(CSetupTheorySet * pSet)
	{
		*m_pTheorySet = *pSet;
	}

	void SetSetupFOOrderSet(CSetupFOOrderSet * pSet)
	{
		*m_pFOOrderSet = *pSet;
	}

	void SetSetupSlippageZeroSet(CSetupSlippageZeroOrderSet * pSet)
	{
		*m_pSlippageZeroSet = *pSet;
	}

	void SetSetupDisplaySet(CSetupDisplaySet * pSet)
	{
		*m_pDisplaySet = *pSet;
	}

	void SetSetupEtcSet(CSetupEtcSet * pSet)
	{
		*m_pEtcSet = *pSet;
	}	

	~CSetupSet()
	{
		if(m_pTheorySet)		delete m_pTheorySet;
		if(m_pFOOrderSet)		delete m_pFOOrderSet;
		if(m_pSlippageZeroSet)	delete m_pSlippageZeroSet;
		if(m_pDisplaySet)		delete m_pDisplaySet;
		if(m_pEtcSet)			delete m_pEtcSet;
	}

	CSetupSet & operator=(CSetupSet & setup)
	{
		*m_pTheorySet		= *setup.m_pTheorySet;
		*m_pFOOrderSet		= *setup.m_pFOOrderSet;
		*m_pSlippageZeroSet	= *setup.m_pSlippageZeroSet;
		*m_pDisplaySet		= *setup.m_pDisplaySet;
		*m_pEtcSet			= *setup.m_pEtcSet;

		return *this;
	}
};


class CDlgUserData
{
public:
	int CurTab;
	int CurMonth;
	int CurATM;
	int CurQty;
	CString StrategyGroup;
	CString StrategyName;
	
	int OptMonth;
	CString OptQty;
	CString FutQty;
	int CurView;
public:

	CDlgUserData()
	{
		CurTab = -1, CurMonth = -1, CurATM = -1, CurQty = 1, OptMonth = -1,
		CurView = -1, StrategyGroup = "", StrategyName = "", OptQty = "", FutQty = "";
	}

	CDlgUserData & operator=(CDlgUserData & item)
	{
		CurTab		  = item.CurTab;
		CurMonth	  = item.CurMonth;
		CurATM		  = item.CurATM;
		CurQty		  = item.CurQty;
		StrategyGroup = item.StrategyGroup;
		StrategyName  = item.StrategyName;

		OptMonth	  = item.OptMonth;
		OptQty		  = item.OptQty;
		FutQty		  = item.FutQty;
		CurView		  = item.CurView;

		return *this;
	}
};

///////////////////////////////////////////////////////////////////////
//
//
//
//
//
///////////////////////////////////////////////////////////////////////
class CInfoData
{
public:
	
	CString m_strFixedProfit;				//Ȯ������ + ������.
	CString m_strOnlyFixedProfit;			//Ȯ�����͸�..
	CString m_strUnderlyingAssetsCode;		//���� ���õ� �����ڻ�..
	CString	m_strUnderlyingAssetsName;		//���� ���õ� �����ڻ��..

//	CSetupDataSet *	m_pSetupDataSet;
	CSetupTheorySet*	m_pSetupTheorySet;
	CSetupFOOrderSet*	m_pSetupFOOrderSet;
	CSetupSlippageZeroOrderSet*	m_pSetupSlippageZeroOrderSet;
	CSetupDisplaySet*	m_pSetupDisplaySet;
	CSetupEtcSet*		m_pSetupEtcSet;
	BOOL	m_bSelectCalc;//TRUE: ���ű� ������ ������ LButtonDown�� Ŭ���ؼ� �������űݰ������ ���..
	
public:

	CInfoData()
	{
		m_bSelectCalc = FALSE;
	}
};


class CChartDataSet
{
public:
	CDataArray	*	m_pChartAddItemArray;
	CDataArray	*	m_pChartDelItemArray;

public:
	CChartDataSet()
	{
		m_pChartAddItemArray = NULL;
		m_pChartDelItemArray = NULL;
	}
};
////////////////////////////////////////////////////////////////////////////

class CRemainDaySet
{
public:
	CString m_strYM;
	CString m_strRemainDayNextDay;

public:

	CRemainDaySet & operator=(CRemainDaySet & set)
	{
		m_strYM = set.m_strYM;
		m_strRemainDayNextDay = set.m_strRemainDayNextDay;
		return *this;
	}

};
typedef CArray < CRemainDaySet * , CRemainDaySet *> CRemainDayArray;

class CMarginInfo
{
public:

	//20080328 �̹��� >>
	//�ϳ��������� ��Ź�� ������ ���������űݿ� ����
	CString	m_strHanaPremiumRatioFlag;	//�� ���� 2�̸� ��������
	CString	m_strDangerPremiumRatio;	//��Ź���������ű�������(%�ƴ� ������ ����)
	CString m_strHavePremiumRatio;		//�������������ű�������(%�ƴ� ������ ����)
	//20080328 �̹��� <<

	CString m_strBranchNo;				/* ������ȣ                 */
	CString m_strGoodsJobSect;			/* ��ǰ��������             */
	CString m_strAccountSeq;/* �����Ϸù�ȣ             */
	
	CString m_strSect;/* ����                     */
	/* �Ϲ��ؼҽ� ���ű� ���� ����  0 - ���� , 1 - ������           */


	CString m_strDangerPercent;/* �������ű���             */
	CString m_strHavePercent;/* �������ű���             */
	CString m_strDangerMinPercent;/* �����ּ����ű���         */
	CString m_strHaveMinPercent; /* �����ּ����ű���         */
	CString m_strFutureMulti;/* �����¼�                 */
	CString m_strOptionMulti;/* �ɼǽ¼�                 */
	CString m_strFutureMinMargin;/* �ּ����ű�               */
             /* ���������ּ����ű� - �Ϲ�                                  */


	CString m_strOptionMinMargin;/* �ּ����ű�1              */
             /* �ɼǰ����ּ����ű� - �Ϲ�                                  */


//	CString m_strFutureMinMargin1; /* �ּ����ű�2              */
             /* ���������ּ����ű� - �Ϲ��ؼ����ű� ������                 */

//	CString m_strOptionMinMargin1; /* �ּ����ű�3              */
             /* �ɼǰ����ּ����ű� - �Ϲ��ؼ����ű� ������                 */

	CString m_strSpreadDangerMargin;/* ���űݾ�                 */
             /* ����������Ź���ű�                                           */

	CString m_strSpreadTradeMargin;/* ���űݾ�1                */
             /* ��������Ÿ����ű�                                           */

//	CString m_strDayPerYearForTheory;/* �����ϼ�                 */
             /* �����̷а� ���� ���������ϼ�                               */


	CString m_strWrMoneySect;
	CString m_strSide_Opg_wrmoney_rate;
	CString m_strSide_Maintn_wrmoney_rate;

	CString m_strCashRate;

    
	CRemainDayArray	m_RemainDayArray;//�ܿ��� �����迭.

//    CString m_strOrdDate;  // �����ֹ�������(���ű� ���� ������)
public:

	CMarginInfo(){ m_strCashRate = "0.05";}

	~CMarginInfo()
	{
		int i,nSize = m_RemainDayArray.GetSize();
		for(i = 0 ; i < nSize ; i++) delete m_RemainDayArray.GetAt(i);
	}

	CMarginInfo & operator=(CMarginInfo & margin)
	{
		m_strBranchNo = margin.m_strBranchNo ;				/* ������ȣ                 */
		m_strGoodsJobSect= margin.m_strGoodsJobSect;			/* ��ǰ��������             */
		m_strAccountSeq= margin.m_strAccountSeq;/* �����Ϸù�ȣ             */
		
		m_strSect= margin.m_strSect;/* ����                     */
		/* �Ϲ��ؼҽ� ���ű� ���� ����  0 - ���� , 1 - ������           */


		m_strDangerPercent= margin.m_strDangerPercent;/* �������ű���             */
		m_strHavePercent= margin.m_strHavePercent;/* �������ű���             */
		m_strDangerMinPercent= margin.m_strDangerMinPercent;/* �����ּ����ű���         */
		m_strHaveMinPercent= margin.m_strHaveMinPercent; /* �����ּ����ű���         */
		m_strFutureMulti= margin.m_strFutureMulti;/* �����¼�                 */
		m_strOptionMulti= margin.m_strOptionMulti;/* �ɼǽ¼�                 */
		m_strFutureMinMargin= margin.m_strFutureMinMargin;/* �ּ����ű�               */
				 /* ���������ּ����ű� - �Ϲ�                                  */


		m_strOptionMinMargin= margin.m_strOptionMinMargin;/* �ּ����ű�1              */
				 /* �ɼǰ����ּ����ű� - �Ϲ�                                  */


//		m_strFutureMinMargin1= margin.m_strFutureMinMargin1; /* �ּ����ű�2              */
				 /* ���������ּ����ű� - �Ϲ��ؼ����ű� ������                 */

//		m_strOptionMinMargin1= margin.m_strOptionMinMargin1; /* �ּ����ű�3              */
				 /* �ɼǰ����ּ����ű� - �Ϲ��ؼ����ű� ������                 */

		m_strSpreadDangerMargin= margin.m_strSpreadDangerMargin;/* ���űݾ�                 */
				 /* ����������Ź���ű�                                           */

		m_strSpreadTradeMargin= margin.m_strSpreadTradeMargin;/* ���űݾ�1                */
				 /* ��������Ÿ����ű�                                           */

//		m_strDayPerYearForTheory= margin.m_strDayPerYearForTheory;/* �����ϼ�                 */
				 /* �����̷а� ���� ���������ϼ�                               */

		m_strCashRate = margin.m_strCashRate;

		m_strWrMoneySect = margin.m_strWrMoneySect;
		m_strSide_Opg_wrmoney_rate = margin.m_strSide_Opg_wrmoney_rate;
		m_strSide_Maintn_wrmoney_rate = margin.m_strSide_Maintn_wrmoney_rate;

//		if(m_strOrdDate.GetLength() != 0)
//			m_strOrdDate = margin.m_strOrdDate;
		
		
		//20080328 �̹��� >>
		m_strHanaPremiumRatioFlag	= margin.m_strHanaPremiumRatioFlag;	//�� ���� 2�̸� ��������
		m_strDangerPremiumRatio		= margin.m_strDangerPremiumRatio;	//��Ź���������ű�������(%�ƴ� ������ ����)
		m_strHavePremiumRatio		= margin.m_strHavePremiumRatio;		//�������������ű�������(%�ƴ� ������ ����)
		//20080328 �̹��� <<


		int i,nSize = m_RemainDayArray.GetSize();
		for(i = 0 ; i < nSize ; i++) delete m_RemainDayArray.GetAt(i);
		m_RemainDayArray.RemoveAll();



		nSize = margin.m_RemainDayArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CRemainDaySet * pSet = margin.m_RemainDayArray.GetAt(i);
			CRemainDaySet * pNewSet = new CRemainDaySet;
			*pNewSet = *pSet;
			m_RemainDayArray.Add(pNewSet);
		}		
		return *this;
	}

	CString GetRemainDay(CString strCode)
	{
		for(int i = 0 ;  i < m_RemainDayArray.GetSize(); i++)
		{
			CRemainDaySet * pRemainSet = m_RemainDayArray.GetAt(i);
			if(pRemainSet->m_strYM.GetLength() > 1 && pRemainSet->m_strYM == strCode.Mid(3,2))
			{
				return pRemainSet->m_strRemainDayNextDay;
			}
		}
		return "";
	}
};

class CAverageIVSet
{
public:
	CString m_strUnderlyingAssetsCode;
	CString m_strCallAverageIV;
	CString m_strPutAverageIV;
	CString m_strVolate;

public:
	CAverageIVSet & operator=(CAverageIVSet & set)
	{
		
		m_strUnderlyingAssetsCode	= set.m_strUnderlyingAssetsCode;
		m_strCallAverageIV			= set.m_strCallAverageIV;
		m_strPutAverageIV			= set.m_strPutAverageIV;
		m_strVolate					= set.m_strVolate;

		return *this;
	}

};
typedef CArray < CAverageIVSet * , CAverageIVSet *> CAverageIVArray;

class CAverageIV
{
public:
	char	m_cTradeState;
	CString m_strType;//�����ᱸ��(1 - ����, 2 - ������)

	CAverageIVArray m_AverageIVArray;
public:
	CAverageIV & operator=(CAverageIV & iv)
	{
		m_strType			=iv.m_strType;
		m_cTradeState		=iv.m_cTradeState;

		int i;
		for(i = 0 ; i < m_AverageIVArray.GetSize(); i++) delete m_AverageIVArray.GetAt(i);
		m_AverageIVArray.RemoveAll();

		for(i = 0 ; i < iv.m_AverageIVArray.GetSize(); i++) 
		{
			CAverageIVSet * pSet = iv.m_AverageIVArray.GetAt(i);
			CAverageIVSet * pNewSet = new CAverageIVSet;
			*pNewSet = *pSet;
			m_AverageIVArray.Add(pNewSet);
		}
		return *this;
	}

	~CAverageIV()
	{
		for(int i = 0 ; i < m_AverageIVArray.GetSize(); i++) delete m_AverageIVArray.GetAt(i);
	}
};

////////////////////////////////////////////////////////////////////////////////////
//
//	�ɼ� ������ 20�� ���纯���� ������
//
////////////////////////////////////////////////////////////////////////////////////
class COptionIVSet
{
public:
	CString m_strStrike;
	CStringArray m_strIVArray;

public:

	COptionIVSet & operator=(COptionIVSet & option)
	{
		m_strStrike = option.m_strStrike;
		
		m_strIVArray.RemoveAll();

		m_strIVArray.Append(option.m_strIVArray);

		return *this;
	}
};
typedef CArray < COptionIVSet *, COptionIVSet *> COptionIVArray;

class COptionIVMonthSet
{
public:
	CString m_strEndDate;
	COptionIVArray	m_CallArray;
	COptionIVArray	m_PutArray;

public:
	COptionIVMonthSet(){;}
	~COptionIVMonthSet()
	{
		int i;
		for(i = 0 ; i < m_CallArray.GetSize(); i++) delete m_CallArray.GetAt(i);
		for(i = 0 ; i < m_PutArray.GetSize();  i++) delete m_PutArray.GetAt(i);
	}

	COptionIVMonthSet & operator=(COptionIVMonthSet & month)
	{
		int i;
		m_strEndDate = month.m_strEndDate;
		
		for(i = 0 ; i < m_CallArray.GetSize(); i++) delete m_CallArray.GetAt(i);
		m_CallArray.RemoveAll();
		for(i = 0 ; i < m_PutArray.GetSize();  i++) delete m_PutArray.GetAt(i);
		m_PutArray.RemoveAll();

		for(i = 0 ; i < month.m_CallArray.GetSize(); i++)
		{
			COptionIVSet * pIV = month.m_CallArray.GetAt(i);
			COptionIVSet * pNewIV = new COptionIVSet;
			*pNewIV = *pIV;
			m_CallArray.Add(pNewIV);
		}

		for(i = 0 ; i < month.m_PutArray.GetSize(); i++)
		{
			COptionIVSet * pIV = month.m_PutArray.GetAt(i);
			COptionIVSet * pNewIV = new COptionIVSet;
			*pNewIV = *pIV;
			m_PutArray.Add(pNewIV);
		}		
		return *this;
	}
};

//20080308 �̹��� >>
class CPriceProgressInput
{
public:
	CString m_strCode;
	int		m_nDataCount;
	char	m_nDataBit;
	CString	m_strDate;
	CString	m_strTime;

public:
	CPriceProgressInput(){ m_nDataCount = 0; m_nDataBit = 0;	}
	CPriceProgressInput & operator=(CPriceProgressInput & Input)
	{
		m_strCode	= Input.m_strCode;
		m_nDataCount= Input.m_nDataCount;
		m_nDataBit	= Input.m_nDataBit;
		m_strDate	= Input.m_strDate;
		m_strTime	= Input.m_strTime;
		return *this;
	}
};
typedef CList < CPriceProgressInput * , CPriceProgressInput * > CPriceProgressInputList;
//20080308 �̹��� <<

////////////////////////////////////////////////////////////////////////////////////
// �輺�� �߰� 2003.02.07
class CPriceProgressItem
{
public:
	CString		m_strDate;
	CString		m_strTime;
	CString		m_strLastPrice;					// ����
	CString		m_strDecisionTime;				// Ȯ���ð�

	//////////////////////////////
	//20080306 �̹��� >>
	//�ӽ� ������
	CString		m_strRemainDay;					//������
	CString		m_strRatio;						//��������
	CString		m_strCDRate;					//cd�ݸ�
	//20080306 �̹��� <<
	//////////////////////////////

public:
	CPriceProgressItem& operator =(CPriceProgressItem &PriceProgressItem)
	{
		m_strDate			= PriceProgressItem.m_strDate;
		m_strTime			= PriceProgressItem.m_strTime;
		m_strLastPrice		= PriceProgressItem.m_strLastPrice;

		//20080306 �̹��� >>
		m_strRemainDay		= PriceProgressItem.m_strRemainDay;
		m_strRatio			= PriceProgressItem.m_strRatio;
		m_strCDRate			= PriceProgressItem.m_strCDRate;
		//20080306 �̹��� <<
		
		return *this;
	}
};

typedef CList <CPriceProgressItem *, CPriceProgressItem *> CPriceProgressItemList;
typedef CArray <CPriceProgressItem *, CPriceProgressItem *> CPriceProgressItemArray;

class CPriceProgressSubItem
{
public:
	CString		m_strDate;
	CString		m_strRemainDay;					// �����ϼ�
	CString		m_strAvgIntRate;				// �ݸ����
	CString		m_strRatio;						// ����
	CString		m_strVolate;					// ����պ�����
	CString		m_strHisVolate;					// ������������
public:
	CPriceProgressSubItem& operator =(CPriceProgressSubItem &PriceProgressSubItem)
	{
		m_strDate		= PriceProgressSubItem.m_strDate;
		m_strRemainDay	= PriceProgressSubItem.m_strRemainDay;
		m_strAvgIntRate	= PriceProgressSubItem.m_strAvgIntRate;
		m_strRatio		= PriceProgressSubItem.m_strRatio;
		m_strVolate		= PriceProgressSubItem.m_strVolate;
		m_strHisVolate	= PriceProgressSubItem.m_strHisVolate;

		return *this;
	}
};

typedef CList <CPriceProgressSubItem *, CPriceProgressSubItem *> CPriceProgressSubItemList;
typedef CArray <CPriceProgressSubItem *, CPriceProgressSubItem *> CPriceProgressSubItemArray;

class CPriceProgressInfo
{
public:
	CString		m_strCode;

	CPriceProgressItemList		m_PriceProgressItemList;
	CPriceProgressSubItemList	m_PriceProgressSubItemList;

	int		m_iReferenceCount;

public:
	CPriceProgressInfo()
	{
		m_iReferenceCount = 1;
	}

	~CPriceProgressInfo()
	{
		DeleteAllItem();
	}

	void DeleteAllItem()
	{
		POSITION Pos = m_PriceProgressItemList.GetHeadPosition();

		while(Pos)
		{
			CPriceProgressItem *pPriceProgressItem = m_PriceProgressItemList.GetNext(Pos);

			if(pPriceProgressItem != NULL)
				delete pPriceProgressItem;
		}
		m_PriceProgressItemList.RemoveAll();

		Pos = m_PriceProgressSubItemList.GetHeadPosition();

		while(Pos)
		{
			CPriceProgressSubItem *pPriceProgressSubItem = m_PriceProgressSubItemList.GetNext(Pos);

			if(pPriceProgressSubItem != NULL)
				delete pPriceProgressSubItem;
		}
		m_PriceProgressSubItemList.RemoveAll();
	}

	CPriceProgressInfo& operator =(CPriceProgressInfo &PriceProgressInfo)
	{
		DeleteAllItem();

		m_strCode		= PriceProgressInfo.m_strCode;

		POSITION Pos = PriceProgressInfo.m_PriceProgressItemList.GetHeadPosition();

		while(Pos)
		{
			CPriceProgressItem *pPriceProgressItem = new CPriceProgressItem;
			CPriceProgressItem *pTempPriceProgressItem = PriceProgressInfo.m_PriceProgressItemList.GetNext(Pos);
			
			*pPriceProgressItem = *pTempPriceProgressItem;

			m_PriceProgressItemList.AddTail(pPriceProgressItem);
		}

		Pos = PriceProgressInfo.m_PriceProgressSubItemList.GetHeadPosition();

		while(Pos)
		{
			CPriceProgressSubItem *pPriceProgressSubItem = new CPriceProgressSubItem;
			CPriceProgressSubItem *pTempPriceProgressSubItem = 
				PriceProgressInfo.m_PriceProgressSubItemList.GetNext(Pos);

			*pPriceProgressSubItem = *pTempPriceProgressSubItem;

			m_PriceProgressSubItemList.AddTail(pPriceProgressSubItem);
		}

		return *this;
	}

	CPriceProgressSubItem *FindSubItem(LPCTSTR pszDate)
	{
		POSITION Pos = m_PriceProgressSubItemList.GetHeadPosition();

		while(Pos)
		{
			CPriceProgressSubItem *pPriceProgressSubItem = m_PriceProgressSubItemList.GetNext(Pos);

			if(pPriceProgressSubItem == NULL)
				continue;

			if(pPriceProgressSubItem->m_strDate == pszDate)
				return pPriceProgressSubItem;
		}

		return NULL;
	}
};

typedef CArray<CPriceProgressInfo *, CPriceProgressInfo *> CPriceProgressInfoArray;

class CPriceProgressData
{
public:
	int		m_iReceiveMode;		// ������ ���� ���� ���(1: Middle, 2: End)

	CPriceProgressInfoArray		m_arrPriceProgressInfo;

public:
	~CPriceProgressData()
	{
		DeleteAllItem();
	}

	void DeleteAllItem()
	{
		int iCount = m_arrPriceProgressInfo.GetSize();

		for(int _i=0; _i<iCount; _i++)
		{
			CPriceProgressInfo *pPriceProgressInfo = m_arrPriceProgressInfo.GetAt(_i);

			if(pPriceProgressInfo != NULL)
				delete pPriceProgressInfo;
		}
		m_arrPriceProgressInfo.RemoveAll();
	}

	CPriceProgressData& operator =(CPriceProgressData &PriceProgressData)
	{
		DeleteAllItem();

		m_iReceiveMode		= PriceProgressData.m_iReceiveMode;
		
		int iCount = PriceProgressData.m_arrPriceProgressInfo.GetSize();
		
		for(int _i=0; _i<iCount; _i++)
		{
			CPriceProgressInfo *pPriceProgressInfo = new CPriceProgressInfo;
			CPriceProgressInfo *pTempPriceProgressInfo = PriceProgressData.m_arrPriceProgressInfo.GetAt(_i);
			
			*pPriceProgressInfo = *pTempPriceProgressInfo;

			m_arrPriceProgressInfo.Add(pPriceProgressInfo);
		}

		return *this;
	}
};

class CThreadStruct
{
public:
	
	CWnd * m_pWnd;
	BOOL * m_pbExeThread;
	CStrategyCalc * m_pStrategyCalc;
	CStrategyCalc * m_pStrategyCalcHedge;
	CDataArray	m_ChartAddItemArray;
	CDataArray	m_ChartDelItemArray;

	
	//20080317 �̹��� >>
	//Ȯ���ǿ� ��꿡 ���
	CString		m_strRemainDay;
	CString		m_strVolate;
	BOOL		m_bProbWeight;	
	//20080317 �̹��� <<

public:
	
	CThreadStruct()
	{
		m_pWnd = NULL;
		m_pStrategyCalc = NULL;
		m_pStrategyCalcHedge = NULL;
		m_pbExeThread = NULL;

		//20080317 �̹��� >>
		m_bProbWeight = FALSE;
		//20080317 �̹��� <<

	}

	~CThreadStruct()
	{
		int i;
		for(i = 0 ; i < m_ChartAddItemArray.GetSize(); i++) delete m_ChartAddItemArray.GetAt(i);
		for(i = 0 ; i < m_ChartDelItemArray.GetSize(); i++) delete m_ChartDelItemArray.GetAt(i);
	}
};

#define	GREEK_UNDERLYING_MODE		0
#define	GREEK_VOLATE_MODE			1
#define	GREEK_REMAIN_DAY_MODE		2

#define	GREEK_DELTA_MODE			0
#define	GREEK_GAMMA_MODE			1
#define	GREEK_THETA_MODE			2
#define	GREEK_VEGA_MODE				3
#define	GREEK_THEORY_PROFIT_MODE	4

class CInputInfoSet
{
public:

	BOOL	m_bSelectItem;

	int		m_nIndex; //�ùķ��̼� �׸����� �� ��ȣ.

	int		m_nXMode; //0 : �����ڻ�, 1:������, 2: ������

	int     m_nYMode; //0: ��Ÿ, 1: ����, 2: ��Ÿ, 3: ���� 4 : �̷м���
	
	double	m_dCallVolate;
	
	double	m_dPutVolate;
	
	double	m_dUnderlyingAssetsPrice;
	
	int		m_nRemainDay;

public:
	CInputInfoSet()
	{
		m_nIndex= 0;
		m_nXMode= 0;
		m_nYMode= 0;
		m_dCallVolate = 0.;
		m_dPutVolate  = 0.;
		m_dUnderlyingAssetsPrice = 0;
		m_nRemainDay = 1;
		m_bSelectItem = TRUE;
	}



	CInputInfoSet & operator=(CInputInfoSet & InfoSet)
	{		
		m_nIndex= InfoSet.m_nIndex;
		m_nXMode= InfoSet.m_nXMode;
		m_nYMode= InfoSet.m_nYMode;
		m_dCallVolate = InfoSet.m_dCallVolate;
		m_dPutVolate  = InfoSet.m_dPutVolate;
		m_dUnderlyingAssetsPrice = InfoSet.m_dUnderlyingAssetsPrice;
		m_nRemainDay = InfoSet.m_nRemainDay;
		m_bSelectItem = InfoSet.m_bSelectItem;
		return *this;
	}
};
typedef CArray <CInputInfoSet *, CInputInfoSet *> CInputInfoArray;

class CInputInfo


{
public:
	int m_nXMode;
	int m_nYMode;

	double	m_dXMin;
	double	m_dXMax;




	CInputInfoArray	m_InputInfoArray;



public:

	CInputInfo(){ m_nXMode = 0, m_nYMode = 0, m_dXMin = 0., m_dXMax = 0;}
	~CInputInfo()
	{	
		for(int i = 0 ; i < m_InputInfoArray.GetSize(); i++) delete m_InputInfoArray.GetAt(i);		




	}
	
	CInputInfo & operator=(CInputInfo & Info)
	{
		m_nXMode	= Info.m_nXMode;
		m_nYMode	= Info.m_nYMode;
		
		m_dXMax		= Info.m_dXMax;
		m_dXMin		= Info.m_dXMin;

		for(int i = 0 ; i < m_InputInfoArray.GetSize(); i++) delete m_InputInfoArray.GetAt(i);
		m_InputInfoArray.RemoveAll();


		for(i = 0 ; i < Info.m_InputInfoArray.GetSize(); i++)
		{
			CInputInfoSet * pInfo = Info.m_InputInfoArray.GetAt(i);
			CInputInfoSet * pNewInfo = new CInputInfoSet;
			*pNewInfo = *pInfo;
			m_InputInfoArray.Add(pNewInfo);


		}

		return *this;
	}
};
typedef CArray <CInputInfo * , CInputInfo *> CInputArray;


class CThreadIO4GreekSimulator
{
public:
	CInputInfo *	m_pInputInfo;
	CWnd *			m_pWnd;
	CStrategyCalc *	m_pStrategyCalc;

public:
	
	CThreadIO4GreekSimulator()
	{
		m_pInputInfo = NULL;
		m_pWnd = NULL;
		m_pStrategyCalc = NULL;
	}

	~CThreadIO4GreekSimulator()
	{
		if(m_pInputInfo)	delete m_pInputInfo;
	}

};

class CGreekSimulSetupItem
{
public:
	int			m_nXMode;
	CString		m_strUnderlyingAssetsCode;

	CString		m_strCallVolateStart;
	CString		m_strPutVolateStart;
	CString		m_strRemainDayStart;
	CString		m_strUnderlyingAssetsPriceStart;

	CString		m_strCallVolateDelta;
	CString		m_strPutVolateDelta;
	CString		m_strRemainDayDelta;
	CString		m_strUnderlyingAssetsPriceDelta;

	int			m_nDataCount;

public:	

	CGreekSimulSetupItem(int nXMode)
	{
		m_nXMode = nXMode;
		m_nDataCount = 3;
		m_strRemainDayStart = "1";
	}


	CGreekSimulSetupItem & operator=(CGreekSimulSetupItem & SetupItem)
	{
		m_nXMode = SetupItem.m_nXMode;
		m_strCallVolateDelta = SetupItem.m_strCallVolateDelta;
		m_strCallVolateStart = SetupItem.m_strCallVolateStart;
		m_strPutVolateDelta = SetupItem.m_strPutVolateDelta;
		m_strPutVolateStart = SetupItem.m_strPutVolateStart;
		m_strRemainDayStart = SetupItem.m_strRemainDayStart;
		m_strRemainDayDelta = SetupItem.m_strRemainDayDelta;
		m_strUnderlyingAssetsPriceDelta = SetupItem.m_strUnderlyingAssetsPriceDelta;
		m_strUnderlyingAssetsPriceStart = SetupItem.m_strUnderlyingAssetsPriceStart;
		m_nDataCount = SetupItem.m_nDataCount;
		m_strUnderlyingAssetsCode = SetupItem.m_strUnderlyingAssetsCode;
		return *this;
	}
};

typedef CArray <CGreekSimulSetupItem *, CGreekSimulSetupItem *> CGreekSimulSetupArray;

class DATASET_DECLSPEC CIVPastInputData
{
public:

	int     m_nOutputCount;

	CString m_strCode1; 
	CString m_strCode2;

	CString m_strRqType;	//��ȸ����
	CString	m_strVolate;	//������

	CStringArray	m_strOutputTypeArray; //��±��к�	

public:

	CIVPastInputData()
	{
		m_nOutputCount = 9;
		for(int i = 0  ;i < m_nOutputCount ; i++) m_strOutputTypeArray.Add("0");
	}

	CIVPastInputData & operator=(CIVPastInputData & input)
	{
		m_strCode1 = input.m_strCode1;
		m_strCode2 = input.m_strCode2;
		m_strRqType= input.m_strRqType;
		m_strVolate= input.m_strVolate;

		m_strOutputTypeArray.RemoveAll();
		m_strOutputTypeArray.Append(input.m_strOutputTypeArray);

		return *this;
	}
};

class DATASET_DECLSPEC CIVPastDataSet
{
public:
	CString m_strDate;			  //��¥
	CString	m_strTime;			  //�ð�
	CStringArray	m_strIVArray; //���纯����

public:

	CIVPastDataSet & operator=(CIVPastDataSet & data)
	{
		m_strDate	= data.m_strDate;
		m_strTime	= data.m_strTime;

		m_strIVArray.RemoveAll();
		m_strIVArray.Append(data.m_strIVArray);
		return *this;
	}
};
typedef CArray < CIVPastDataSet *, CIVPastDataSet *> CIVPastDataArray;


class DATASET_DECLSPEC CIVPastData
{
public:
	char	m_cIntervalType;	//�Ⱓ ����
	char	m_cRealType;		//�ǽð� ����
	char	m_cUseType;			//�������
	CString	m_strCurrent;		//���簡
	CString	m_strPreClose;		//��������
	CString m_strVolume;		//�ŷ���
	CString	m_strNormalCode;	//�����ڵ� �븻 ??
	CString	m_strName;			//�����

	CString	m_strCode1;
	CString	m_strCode2;
	CString m_strIV1;
	CString m_strIV2;
	CString m_strDate;

	CIVPastDataArray	m_IVPastDataArray;
public:

	~CIVPastData()
	{
		for(int i = 0 ; i < m_IVPastDataArray.GetSize(); i++) delete m_IVPastDataArray.GetAt(i);
	}

	CIVPastData & operator=(CIVPastData & data)
	{
		m_cIntervalType = data.m_cIntervalType;
		m_cRealType		= data.m_cRealType;
		m_cUseType		= data.m_cUseType;
		m_strCurrent	= data.m_strCurrent;
		m_strPreClose	= data.m_strPreClose;
		m_strVolume		= data.m_strVolume;
		m_strNormalCode	= data.m_strNormalCode;
		m_strName		= data.m_strName;
		
		m_strCode1		= data.m_strCode1;
		m_strCode2		= data.m_strCode2;
		m_strIV1		= data.m_strIV1;
		m_strIV2		= data.m_strIV2;
		m_strDate		= data.m_strDate;

		int i;
		for(i = 0 ; i < m_IVPastDataArray.GetSize(); i++) delete m_IVPastDataArray.GetAt(i);
		m_IVPastDataArray.RemoveAll();

		for(i = 0 ; i < data.m_IVPastDataArray.GetSize(); i++)
		{
			CIVPastDataSet * pSet = data.m_IVPastDataArray.GetAt(i);
			CIVPastDataSet * pNewSet = new CIVPastDataSet;
			*pNewSet = *pSet;
			m_IVPastDataArray.Add(pNewSet);
		}
		return *this;
	}

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//20080228 �̹��� >>
//�ϳ��������� �߰��� ��Ŷ
//������ ��쿡�� �������̵����ͷ� �޴� ��������Ŷ �� ���񺰷� �ߺ����� ��� �����͸� �и��ؼ� �����ڻ꺰�� 
//��û�ϰ� �޵��� �ߴ�.
//20080228 �̹��� <<
//////////////////////////////////////////////////////////////////////////////////////////////////////////
class CTotalIVDataSet
{
public:
	CString m_strBaseCode;	//01(kp200), 10(�ֽĿɼ�1), ....
	CString m_strCallIV;	//������ճ��纯����
	CString m_strPutIV;		//ǲ����ճ��纯����
	CString m_strTotIV;		//������ճ��纯����
	CString m_strStatus;    //�屸�л��� (1-����, 2-������)

public:
	
	CTotalIVDataSet & operator=(CTotalIVDataSet & Set)
	{
		m_strBaseCode	= Set.m_strBaseCode;
		m_strCallIV		= Set.m_strCallIV;
		m_strPutIV		= Set.m_strPutIV;
		m_strTotIV		= Set.m_strTotIV;
		m_strStatus		= Set.m_strStatus;
		return *this;
	}

};
typedef CArray < CTotalIVDataSet *, CTotalIVDataSet *> CTotalIVDataArray;


//20080304 �̹��� >>
class CIV4DateSet
{
public:
	CString		m_strDate;
	CString		m_strClose;
	CString		m_strCallIV;
	CString		m_strPutIV;
	CString		m_strTotIV;
	CString		m_strHis90IV;

public:
	CIV4DateSet(){;}

	CIV4DateSet(CIV4DateSet & Set)
	{
		Copy(Set);
	}

	CIV4DateSet & operator=(CIV4DateSet & Set)
	{
		Copy(Set);
		return *this;
	}

	void Copy(CIV4DateSet & Set)
	{
		m_strDate  = Set.m_strDate;
		m_strClose = Set.m_strClose;
		m_strCallIV= Set.m_strCallIV;
		m_strPutIV = Set.m_strPutIV;
		m_strTotIV = Set.m_strTotIV;
		m_strHis90IV=Set.m_strHis90IV;
	}
};

typedef CArray < CIV4DateSet * , CIV4DateSet * > CIV4DateArray;

class CIV4ItemSet
{
public:

	CString			m_strCode;
	CIV4DateArray	m_IVDateArray;

public:

	CIV4ItemSet & operator=(CIV4ItemSet & Set)
	{
		int i;
		m_strCode = Set.m_strCode;
		for(i = 0 ; i < m_IVDateArray.GetSize(); i++ )		
			delete m_IVDateArray.GetAt(i);
		
		for(i = 0 ;i < Set.m_IVDateArray.GetSize(); i++)
		{
			CIV4DateSet * pSet = Set.m_IVDateArray.GetAt(i);
			CIV4DateSet * pNewSet = new CIV4DateSet;
			*pNewSet = *pSet;
			m_IVDateArray.Add(pNewSet);
		}

		return *this;
	}
};

typedef CArray < CIV4ItemSet *, CIV4ItemSet * > CIV4ItemArray;
//20080304 �̹��� <<
#endif 