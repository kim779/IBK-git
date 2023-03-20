#ifndef SIGNAL_CLIENTPACKET
#define SIGNAL_CLIENTPACKET

/****************************************************************/
/*                                                              */
/*						 ��ȣ Packet (Signal_PacketDef.h)		*/
/*																*/
/*						Client���� ����ϴ� ��ȣ Packet!		*/
/*						SVR -> SignalMng -> Client				*/
/*							(User)								*/
/*																*/
/*                   2006. 10.		 		Doori	co.    		*/
/*                      �ۼ��� : �쵿��							*/
/****************************************************************/

//# ��ȣ ��û TYPE!
#define TYPE_SIGNAL_SYS     145  //���� 170->142   //# �ý��� ��ȣ//
#define TYPE_SIGNAL_ALERT   142  //���� 171->145     //# ���� �ý��� or ����� ���� ��ȣ
#define TYPE_SIGNAL_NEWS    161  //���� 172->161    //# ��û ����ó ���� ��ȣ  [����] Only Client!
#define TYPE_REAL_NEWS      162		//(�Ｚ)169    //# ��û ����ó ����
//#define TYPE_REAL_ALERT		142

#define	WM_GET_BROD			WM_USER + 703

//# �Է� Values
//#  BOOL bAdvise : ��û Flag -> TRUE :��û, FASE : ����
//#  CWnd* pwndMain : ��û �����͸� ���� ���� 
//#  CString strType : ������ ��û Type
//#  CString strKey  : ��û ������ Key -> "*" : ALL, [����] : ����ó, [��ȣ] : ���ǹ�ȣ(XXXX)
//extern "C" __declspec(dllexport) int _DLL_SetSiganl(BOOL bAdvise, CWnd* pwndMain, CString strType, CString strKey = "*");


//# ��ȣ Key Table ��û Type!
#define TYPE_TABLE_SYSTEM   "�ý�������"
#define TYPE_TABLE_NEWS     "��������ó"
#define TYPE_TABLE_PORT     "��������"
//# ��ȣ ���� ���̺�
//#  CString strType                Data ����
//   - TYPE_TABLE_SYSTEM    ----->  (Key | ���Ǹ�)
//   - TYPE_TABLE_NEWS      ----->  (Key | ����ó��)
//   - TYPE_TABLE_PORT      ----->  (Key | ���Ǹ�)
//# CStringArray &saSignalTable
//   : Type�� ���� DATA! 
//# Return Values => Table Cnt!
//extern "C" __declspec(dllexport) BOOL _DLL_GetSiganlTable(CString strType, CStringArray &saSignalTable);


//# ���� ���� ���� Type => SignalDegree
#define NEWS_CODETYPE_RPN       99    //# ��ǥ����
#define NEWS_CODETYPE_RLT       98    //# ��������
#define NEWS_CODETYPE_C_INS     97    //# ��������(����)
#define NEWS_CODETYPE_T_INS     96    //# ��������(����)


#define CODE_LEN						6
#define CODE_NAME_LEN					40	//@���� 20 -> 40
#define TIME_LEN						6
#define DATE_LEN						8
#define SIGNAL_NAME_LEN					40
#define SIGNAL_DEGREE_LEN				2
#define IMG_INDEX_LEN					2


#define STR_CONVERT(pBuff, cData, nSize, strData)	\
	pBuff = new char[nSize + 1];					\
	memset(pBuff, 0x00, nSize + 1);					\
	memcpy(pBuff, cData, nSize);					\
	strData = (CString)pBuff;						\
	strData.TrimRight();							\
delete [] pBuff;

#define INT_CONVERT(pBuff, cData, nSize, nData)		\
	pBuff = new char[nSize + 1];					\
	memset(pBuff, 0x00, nSize + 1);					\
	memcpy(pBuff, cData, nSize);					\
	nData = atoi(pBuff);							\
delete [] pBuff;


#define HAVE_SPACE_CODE     1


typedef struct tagSIGNAL_HEAD
{
	char		sSize[4];
	char    sType[20];
} SIGNAL_HEAD;
#define SZ_SIGNAL_HEAD  sizeof(SIGNAL_HEAD)

typedef struct tagSIGNAL_COMMON
{
	char    sMarket[1];
	char	sCode[CODE_LEN];		// �����ڵ�
	
	char    sPrice[8];          // ���簡 (����:��)
	char    sSign;              // ���ϴ���ȣ          
	char    sChage[8];          // ���ϴ��          
	char    sChgRate[6];        // �����   Type(S999.99)          
	char    sVolume[9];         // �ŷ��� : Unit(1,000)          
	char    sVolumeRect[6];     // �ŷ��� �����: Unit(1,000) 
	char	sResultField[1040];	// ���� ����ʵ�
} SIGNAL_COMMON;

typedef struct tagSIGNAL_CONDITION
{
//	SIGNAL_HEAD	stHEAD;
	
	char	sCode[CODE_LEN];					// �����ڵ�
	char	sName[20];//[CODE_NAME_LEN];			// �����
	char	sJobFlag[1];						  // �ű�:'N', ������:'R', Ż��:'O'
	char	sEntryCnt[4];             // ���� Ƚ��
	char	sDate[DATE_LEN];					// �߻� ���� 
	char	sTime[TIME_LEN];					// �߻� �ð�. HHMMSS
	
	char	sSignalImageIndex[IMG_INDEX_LEN];	// ��ȣ������ �ش��ϴ� �̹��� �ε��� ���� 
	char	sSignalDegree[SIGNAL_DEGREE_LEN]; // ��ȣ ����. "-3" ~ "3"
	char	sOrderType[1];										// �ֹ� Ÿ��. '0': �ֹ��� ���� ����. '1': ���ڵ�, '2': �ڵ�
	
	char	sSignalKey[4];		                // System Alert �ε���
	char	sSignalName[SIGNAL_NAME_LEN];		  // System Alert �ε����� �ش��ϴ� �̸�
	SIGNAL_COMMON   stCommon;
} SIGNAL_CONDITION;
#define SZ_SIGNAL_CONDITION  sizeof(SIGNAL_CONDITION)


class CSIGNAL_CONDITIONDATA
{
public:
//	CString m_strType;
	CString m_strSignalKey;
	
	int     m_nMarket;
	CString m_strPrice;
	CString	m_strCode;							// �����ڵ�
	CString	m_strName;							// �����
	
	CString	m_strDate;							// �߻� ���� 
	CString	m_strTime;							// �߻� �ð�. HHMMS6S
	int			m_nSignalImageIndex;			// ��ȣ������ �ش��ϴ� �̹��� �ε��� ���� 
	int			m_nSignalDegree;				// ��ȣ ����. "-3" ~ "3"
	CString	m_strOrderType;						// �ֹ� Ÿ��. '0': �ֹ��� ���� ����. '1': ���ڵ�, '2': �ڵ�
	
	CString	m_strSignalName;					// System Alert �ε����� �ش��ϴ� �̸�
	
	virtual void  Convert(SIGNAL_CONDITION* pInput)
	{
		char *pBuff;
//		STR_CONVERT(pBuff, pInput->stHEAD.sType, sizeof(pInput->stHEAD.sType), m_strType);
//		m_strType.TrimRight(" ");
		
		STR_CONVERT(pBuff, pInput->sSignalKey, sizeof(pInput->sSignalKey), m_strSignalKey);
		
		INT_CONVERT(pBuff, pInput->stCommon.sMarket, sizeof(pInput->stCommon.sMarket), m_nMarket);
		STR_CONVERT(pBuff, pInput->stCommon.sPrice, sizeof(pInput->stCommon.sPrice), m_strPrice);
		STR_CONVERT(pBuff, pInput->sCode, sizeof(pInput->sCode), m_strCode);
		STR_CONVERT(pBuff, pInput->sName, sizeof(pInput->sName), m_strName);
		m_strName.TrimRight(" ");
		
		STR_CONVERT(pBuff, pInput->sDate, sizeof(pInput->sDate), m_strDate);
		STR_CONVERT(pBuff, pInput->sTime, sizeof(pInput->sTime), m_strTime);
		INT_CONVERT(pBuff, pInput->sSignalImageIndex, sizeof(pInput->sSignalImageIndex), m_nSignalImageIndex);
		INT_CONVERT(pBuff, pInput->sSignalDegree, sizeof(pInput->sSignalDegree), m_nSignalDegree);
		STR_CONVERT(pBuff, pInput->sOrderType, sizeof(pInput->sOrderType), m_strOrderType);
		
		STR_CONVERT(pBuff, pInput->sSignalName, sizeof(pInput->sSignalName), m_strSignalName);
	}
};

typedef struct tagNEWS_KEY
{
	char    sDate       [ 8];
	char    sTime       [ 6];
	char    sSeqno      [ 12];
	char    sGbn_l      [ 2];
	char    sGbn_m      [ 2];
//	char    sGbn_s      [ 2];
//	char    sFiller		[ 2];
} DR_NEWS_KEY;
#define		SZ_DR_NEWS_KEY		sizeof(DR_NEWS_KEY)	

#define MAX_REAL_PACKET_SIZE 1024 * 16
typedef struct tagRealItem
{
	char	szPacketKey[4];
	char	szDataLength[4];
	char	szBcData[MAX_REAL_PACKET_SIZE + 1];

} ST_REALITEM;
#define  SZ_REALITEM sizeof(ST_REALITEM)

typedef struct tagSIGANL_NEWS
{
	SIGNAL_HEAD	stHEAD;
	
	DR_NEWS_KEY stNewsKey;
	char	sSignalImageIndex[IMG_INDEX_LEN];	// ��ȣ������ �ش��ϴ� �̹��� �ε��� ���� 
	char	sSignalDegree[SIGNAL_DEGREE_LEN];	// ��ȣ ����. "-3" ~ "3"
	
	char	sGoodPoint[2];   		// ȣ�� ���� ����. + ��Ȳ�� �߻�
	char	sBadPoint[2];		 	// ���� ���� ����. - ��Ȳ�� �߻�.
	
	char	sCode[CODE_LEN];		/* ���� �ڵ� 				*/
	char	sName[CODE_NAME_LEN];	/* �����    				*/
	char	sProfferID[2];			/* ���� ����ó ID		*/
	char	sProffer[20];			/* ���� ����ó 			*/
	char	sProfferTypeID[3];	//@��������sProfferTypeID[2];    /* ����ó �з� ID		*/
	//char	sProfferTypeID[2];		/* ����ó �з� ID		*/
	char	sProfferType[50];	    /* ����ó �з� 			*/
	char	sTitle[255];			/* �� �� 						*/
	
	char	sNsDt[8];					/* @�����߰�:�������� */
	char	sNsTm[6];					/* @�����߰�:�����ð� */

	SIGNAL_COMMON   stCommon;
} SIGANL_NEWS;
#define SZ_SIGANL_NEWS  sizeof(SIGANL_NEWS)

class CSIGANL_NEWSDATA
{
public:
	DR_NEWS_KEY stNewsKey;
	
	CString m_strDate;
	CString m_strTime;
	CString m_strSqNo;
	
	CString m_strType;
	CString m_strRealKey;
	
	int     m_nMarket;
	CString m_strCode;
	CString m_strName;
	
	int		m_nSignalImageIndex;		// ��ȣ������ �ش��ϴ� �̹��� �ε��� ���� 
	int		m_nSignalDegree;			// ��ȣ ����. "-3" ~ "3"
	
	int			m_nProfferID;			// ����ó ���̵�
	CString	m_strProffer;				// ����ó ��
	CString	m_strProfferTypeID;			// ����ó�� �з� ���̵�
	CString	m_strProfferType;			// ����ó�� �з���
	CString	m_strTitle;					// ����
	
	int m_nGoodPoint;   				// ȣ�� ���� ����. + ��Ȳ�� �߻�
	int	m_nBadPoint;		 			// ���� ���� ����. - ��Ȳ�� �߻�.
	
	virtual void  Convert(SIGANL_NEWS* pInput)
	{
		char *pBuff;
		STR_CONVERT(pBuff, pInput->stNewsKey.sDate, sizeof(pInput->stNewsKey.sDate), m_strDate);
		STR_CONVERT(pBuff, pInput->stNewsKey.sTime, sizeof(pInput->stNewsKey.sTime), m_strTime);
		STR_CONVERT(pBuff, pInput->stNewsKey.sSeqno, sizeof(pInput->stNewsKey.sSeqno), m_strSqNo);
		
		STR_CONVERT(pBuff, pInput->stHEAD.sType, sizeof(pInput->stHEAD.sType), m_strType);
		m_strType.TrimRight(" ");
		
		STR_CONVERT(pBuff, pInput->sProfferID, sizeof(pInput->sProfferID), m_strRealKey);
		
		INT_CONVERT(pBuff, pInput->stCommon.sMarket, sizeof(pInput->stCommon.sMarket), m_nMarket);
		STR_CONVERT(pBuff, pInput->sCode, sizeof(pInput->sCode), m_strCode);
		STR_CONVERT(pBuff, pInput->sName, sizeof(pInput->sName), m_strName);
		m_strName.TrimRight(" ");
		
		INT_CONVERT(pBuff, pInput->sSignalImageIndex, sizeof(pInput->sSignalImageIndex), m_nSignalImageIndex);
		INT_CONVERT(pBuff, pInput->sSignalDegree, sizeof(pInput->sSignalDegree), m_nSignalDegree);
		//STR_CONVERT(pBuff, pInput->sOrderType, sizeof(pInput->sOrderType), m_strOrderType);
		
		INT_CONVERT(pBuff, pInput->sProfferID, sizeof(pInput->sProfferID), m_nProfferID);
		STR_CONVERT(pBuff, pInput->sProffer, sizeof(pInput->sProffer), m_strProffer);
		STR_CONVERT(pBuff, pInput->sProfferTypeID, sizeof(pInput->sProfferTypeID), m_strProfferTypeID);
		STR_CONVERT(pBuff, pInput->sProfferType, sizeof(pInput->sProfferType), m_strProffer);
		STR_CONVERT(pBuff, pInput->sTitle, sizeof(pInput->sTitle), m_strTitle);
		
		INT_CONVERT(pBuff, pInput->sGoodPoint, sizeof(pInput->sGoodPoint), m_nGoodPoint);
		INT_CONVERT(pBuff, pInput->sBadPoint, sizeof(pInput->sBadPoint), m_nBadPoint);
	}
};



#endif