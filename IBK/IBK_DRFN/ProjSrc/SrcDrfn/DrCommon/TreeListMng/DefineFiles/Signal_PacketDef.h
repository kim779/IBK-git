#ifndef SIGNAL_CLIENTPACKET
#define SIGNAL_CLIENTPACKET

/****************************************************************/
/*                                                              */
/*						 ��ȣ Packet (Signal_ClientPacket.h)							*/
/*																															*/
/*						Client���� ����ϴ� ��ȣ Packet!									*/
/*						SVR -> DRDS -> SignalMng -> Client								*/
/*																				(User)								*/
/*																															*/
/*                   2005. 12.		 		Doori											*/
/*                      �ۼ��� : �쵿��													*/
/****************************************************************/

#define TYPE_SIGNAL_SYS     "SYS_SIGNAL"
#define TYPE_SIGNAL_USER    "USER_SIGNAL"
#define TYPE_SIGNAL_NEWS    "NEWS_SIGNAL"
#define TYPE_REAL_NEWS      "NEWS_REAL"



//# ���� ���� ���� Type => SignalDegree
#define NEWS_CODETYPE_RPN       99    //# ��ǥ����
#define NEWS_CODETYPE_RLT       98    //# ��������
#define NEWS_CODETYPE_C_INS     97    //# ��������(����)
#define NEWS_CODETYPE_T_INS     96    //# ��������(����)


#define CODE_LEN						6
#define CODE_NAME_LEN				20
#define TIME_LEN						6
#define DATE_LEN						8
#define SIGNAL_NAME_LEN			40
#define SIGNAL_DEGREE_LEN		2
#define IMG_INDEX_LEN       2


#define STR_CONVERT(pBuff, cData, nSize, strData)	\
  pBuff = new char[nSize + 1];							\
  memset(pBuff, 0x00, nSize + 1);						\
  memcpy(pBuff, cData, nSize);							\
  strData = (CString)pBuff;									\
  strData.TrimRight();											\
delete [] pBuff;

#define INT_CONVERT(pBuff, cData, nSize, nData)	\
  pBuff = new char[nSize + 1];						\
  memset(pBuff, 0x00, nSize + 1);					\
  memcpy(pBuff, cData, nSize);						\
  nData = atoi(pBuff);										\
delete [] pBuff;


#define HAVE_SPACE_CODE     1


typedef struct tagSIGNAL_HEAD
{
  char		sSize[4];
  char    sType[4];
} SIGNAL_HEAD;
#define SZ_SIGNAL_HEAD  sizeof(SIGNAL_HEAD)


typedef struct tagSIGNAL_CONDITION
{
  SIGNAL_HEAD	stHEAD;
  
  char	sCode[CODE_LEN];								  // �����ڵ�
  char	sName[CODE_NAME_LEN];						  // �����
  char	sDate[DATE_LEN];								  // �߻� ���� 
  char	sTime[TIME_LEN];								  // �߻� �ð�. HHMMSS
  char	sSignalImageIndex[IMG_INDEX_LEN];	// ��ȣ������ �ش��ϴ� �̹��� �ε��� ���� 
  char	sSignalDegree[SIGNAL_DEGREE_LEN]; // ��ȣ ����. "-3" ~ "3"
  char	sOrderType[1];										// �ֹ� Ÿ��. '0': �ֹ��� ���� ����. '1': ���ڵ�, '2': �ڵ�
  
  char	sSignalName[SIGNAL_NAME_LEN];		  // System Alert �ε����� �ش��ϴ� �̸�
} SIGNAL_CONDITION;
#define SZ_SIGNAL_CONDITION  sizeof(SIGNAL_CONDITION)


class CSIGNAL_CONDITIONDATA
{
public:
  CString	m_strCode;							// �����ڵ�
  CString	m_strName;							// �����
  CString	m_strDate;							// �߻� ���� 
  CString	m_strTime;							// �߻� �ð�. HHMMSS
  int			m_nSignalImageIndex;				// ��ȣ������ �ش��ϴ� �̹��� �ε��� ���� 
  int			m_nSignalDegree;				// ��ȣ ����. "-3" ~ "3"
  CString	m_strOrderType;					// �ֹ� Ÿ��. '0': �ֹ��� ���� ����. '1': ���ڵ�, '2': �ڵ�
  
  CString	m_strSignalName;				// System Alert �ε����� �ش��ϴ� �̸�
  
  virtual void  Convert(SIGNAL_CONDITION* pInput)
  {
    char *pBuff;
    STR_CONVERT(pBuff, pInput->sCode, sizeof(pInput->sCode), m_strCode);

    if ((int)HAVE_SPACE_CODE == 1)
      m_strCode += " ";
    
    STR_CONVERT(pBuff, pInput->sName, sizeof(pInput->sName), m_strName);
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
    char    sSeqno      [ 6];
    char    sGbn_l      [ 2];
    char    sGbn_m      [ 2];
    char    sGbn_s      [ 2];
    char    sFiller[2];
} DR_NEWS_KEY;
#define		SZ_DR_NEWS_KEY		sizeof(DR_NEWS_KEY)	

typedef struct tagSIGANL_NEWS
{
  SIGNAL_HEAD	stHEAD;
  
  DR_NEWS_KEY stNewsKey;
  char	sSignalImageIndex[IMG_INDEX_LEN];	// ��ȣ������ �ش��ϴ� �̹��� �ε��� ���� 
  char	sSignalDegree[SIGNAL_DEGREE_LEN];	// ��ȣ ����. "-3" ~ "3"
  char	sOrderType[1];										// �ֹ� Ÿ��. '0': �ֹ��� ���� ����. '1': ���ڵ�, '2': �ڵ�

  char	sGoodPoint[2];   	// ȣ�� ���� ����. + ��Ȳ�� �߻�
  char	sBadPoint[2];		 	// ���� ���� ����. - ��Ȳ�� �߻�.

  char	sCode[CODE_LEN];			/* ���� �ڵ� 				*/
  char	sName[CODE_NAME_LEN]; /* �����    				*/
  char	sProfferID[2];			  /* ���� ����ó ID		*/
	char	sProffer[20];				  /* ���� ����ó 			*/
  char	sProfferTypeID[2];    /* ����ó �з� ID		*/
	char	sProfferType[50];	    /* ����ó �з� 			*/
	char	sTitle[255];				  /* �� �� 						*/
} SIGANL_NEWS;
#define SZ_SIGANL_NEWS  sizeof(SIGANL_NEWS)

class CSIGANL_NEWSDATA
{
public:
  DR_NEWS_KEY stNewsKey;

  CString m_strCode;
  CString m_strName;

  int			m_nSignalImageIndex;		// ��ȣ������ �ش��ϴ� �̹��� �ε��� ���� 
  int			m_nSignalDegree;				// ��ȣ ����. "-3" ~ "3"
  CString	m_strOrderType;					// �ֹ� Ÿ��. '0': �ֹ��� ���� ����. '1': ���ڵ�, '2': �ڵ�
    
  int			m_nProfferID;						// ����ó ���̵�
  CString	m_strProffer;						// ����ó ��
  CString	m_strProfferTypeID;				// ����ó�� �з� ���̵�
  CString	m_strProfferType;					// ����ó�� �з���
  CString	m_strTitle;							// ����
  
  int m_nGoodPoint;   						// ȣ�� ���� ����. + ��Ȳ�� �߻�
  int	m_nBadPoint;		 						// ���� ���� ����. - ��Ȳ�� �߻�.
    
  virtual void  Convert(SIGANL_NEWS* pInput)
  {
    char *pBuff;
    STR_CONVERT(pBuff, pInput->sCode, sizeof(pInput->sCode), m_strCode);
    STR_CONVERT(pBuff, pInput->sName, sizeof(pInput->sName), m_strName);
    if ((int)HAVE_SPACE_CODE == 1)
      m_strCode += " ";
    
    INT_CONVERT(pBuff, pInput->sSignalImageIndex, sizeof(pInput->sSignalImageIndex), m_nSignalImageIndex);
    INT_CONVERT(pBuff, pInput->sSignalDegree, sizeof(pInput->sSignalDegree), m_nSignalDegree);
    STR_CONVERT(pBuff, pInput->sOrderType, sizeof(pInput->sOrderType), m_strOrderType);
    
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