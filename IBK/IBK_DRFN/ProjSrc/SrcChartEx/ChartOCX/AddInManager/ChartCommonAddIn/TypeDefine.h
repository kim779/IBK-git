/* FX ü��,��ü�� ���� ����ü */
typedef struct  
{
	char	szKeyType[5];				/*  [FX�ֹ�ü��] ��ü�� - OR01, ü�� CH01		*/
	char	szAccNo[11];				/*	[���¹�ȣ]	 �ֹ��� ȸ����+���¹�ȣ			*/
	char	szMemberNo[3];				/*  [ȸ����ID]	 ����Ʈ 000 (����)				*/
	char	szOrdType[5];				/*  [�ֹ�����]	 �ι�° �ڸ��� ����		  
														 OM - ���尡,
														 OE - ������,
														 OS - ��������					*/
	char	SHCODE[12];					/*  [�����]	 ���� : CCY1/CCY2				*/
	char	szOrderID[20];				/*  [�ŷ��� ������ȣ]							*/
	char	szRate[10];					/*  [��������]	 ��ü��� �ֹ���, ü��� ü�ᰡ	*/
	char	szQuote[10];				/*  [���簡]	 ���� ü��� �������� �ݴ�Ÿ� ���簡 */
	char	szStop[10];					/*  [Stop Price]								*/
	char	szLimit[10];				/*	[Limit Price								*/	
	char	szOldStop[10];				/*  [Old Stop]									*/
	char	szOldLimit[10];				/*  [Old Limit]									*/
	char	szOpenTime[30];				/*  [�ֹ�, ���� �ð�]							*/
	char	szSLOpenTime[30];			/*  [SL �ֹ�, ���� �ð�]						*/
	char	szExeTime[30];				/*  [�����ð�]	 ü��ð�						*/
	char	szOrgCustItem1[11];			/*  [��ȸ��ó���׸�1]							*/
	char	szOrgCustItem2[14];			/*  [��ȸ��ó���׸�2] ���ֹ���ȣ				*/
	char	szOrgCustItem3[55];			/*  [��ȸ��ó���׸�3]							*/
	char	szCustItem1[11];			/*  [ȸ��ó���׸� 1]							*/
	char	szCustItem2[14];			/*  [ȸ��ó���׸� 2]  �ֹ���ȣ					*/
	char	szCustItem3[55];			/*  [ȸ��ó���׸� 3]							*/
	char	szStopItem[80];				/*  [Stop ȸ��ó���׸�]	Stop �ֹ���ȣ (��ǥ��)	*/
	char	szLimitItem[80];			/*  [Limit ȸ��ó���׸�] Limit �ֹ���ȣ	(��ǥ��)*/
	char	szOldStopItem[80];			/*	[������ Stop ȸ��ó���׸�]					*/
	char	szOldLimitItem[80];			/*  [������ Limit ȸ��ó���׸�]					*/
	char	szTSCustItem[80];			/*  [TrailingStop ȸ��ó���׸�]					*/
	char	szSide[3];					/*  [�Ÿű���] 
											079 - �ż�, 081 - �ŵ�, 
											080 - ȯ��, 082 - ����						*/
	char	szAccName[20];				/*  [���¸�]									*/
	char	szUserID[20];				/*  [��ID]									*/
	char	szFXCMPosID[20];			/*  [TicketNo]	ü���ȣ						*/
	char	szStatus[1];				/*  [��ü�� ���� ���� 0:������, 1:����]			*/
	char	szSign[1];					/*  [�����ȣ]	2 - ���, 3 - ����, 4 - �϶�	*/
	long	nCommition;					/*  [������]									*/
	long	nTrailingStop;				/*  [Trailing Stop Pip]							*/
	long	nPipLowest;					/*	[�Ҽ��� ��ȿ�ڸ���]							*/
	double	fAmount;					/*  [ü�����]									*/
	double	fOrderAmt;					/*  [��ü�����]								*/
	double	fTotalPL;					/*  [�̼���]									*/
	double	fGrossPL;					/*  [���ϼ���(����ȯ��)]						*/
	double	fRevRate;					/*  [ü��� �ݴ�����]							*/
	double	fOpenMarkup;				/*  [OM (��ȯ�Ž�)]								*/
	double	fInterest;					/*  [����]										*/
	double	fAddInterest;				/*  [������ ����]								*/
	double	fCloseMarkup;				/*  [CM (��ȯ�Ž� ���)]						*/
	double	fOutBalance;				/*  [�̰����ݾ�(û�����)]						*/
	double	fPipCost;					/*  [ü���PipCost]								*/
	double	fPredictMRate;				/*  ���� ������ ȯ��							*/
} FX_CHE_STRUCT;

typedef struct tagRealTimeOrder
{
	char	szAccountNum[14];	/* ���� : 0 - �ű� ������ (�߰�)
								          1 - û�� ������ (����)
										  2 - �ű� �ֹ�   (�߰�)
										  3 - ���� �ֹ�	  (�̵�)
										  4 - ��� �ֹ�   (����)*/
	char	szCustItem[80];		/* ȸ��ó���׸� (���°� 0, 2�϶� KEY) */
	char	szCurrency[12];		/* ����� */
	char	szSide[4];			/* �Ÿű��� */
	char	szOrdType[5];		/* �ֹ����� */
	double	dAmount;			/* ���� */
	double	dPrice;				/* ���� */
	char	szOrderNo[20];		/* ü���ȣ Ȥ�� �ֹ���ȣ */
	char	szOrgCustItem[80];	/* ��ȸ��ó���׸� (���°� 1, 3, 4�� �� KEY) */
	char	szTranTime[30];		/* ó���ð� */
}REALTIME_ORDER;  //���� �뺸



#define CHAR_TO_STRING(xStr, oStr, nSize, VStr) 	memset(xStr, 0x00, sizeof(xStr));	memcpy(xStr, oStr, nSize);\
	VStr.Format("%s", xStr);\



// ü��
class COpenPosition
{
public:
	CString	m_strAccountNum;		/* ü���ȣ	*/
	CString	m_strTicketNo;			/* �����	*/
	CString	m_strRate;				// ü�ᰡ
	CString	m_strSide;				/* �Ÿű���	*/
	double	m_dAmount;				/* ���� */		
	double	m_dOpenPrice;				/* ����ȯ�� */
	double	m_dClosePrice;			/* ����ȯ�� */
	double	m_dCloseStop;				/* ������ */
	double	m_dCloseLimit;			/* ���ͽ��� */
	double  m_dTotalPL;				/* Pip ���� */
	double	m_dGrossPL;				/* ����($) */
	double	m_dInterest;				/* ���� */	
	double	m_dPipCost;				/* Pip Cost */
	CString	m_strExecutedTime;		/* ���Խð� */
	CString	m_strCustItem1;			/* ��ȸ��ó���׸�1 */
	CString	m_strCustItem2;			/* ��ȸ��ó���׸�2 ���ֹ���ȣ */
	CString	m_strCustItem3;			/* ��ȸ��ó���׸�3 */
	CString	m_strOrgCustItem1;		/* ȸ��ó���׸� 1 */
	CString	m_strOrgCustItem2;		/* ȸ��ó���׸� 2  �ֹ���ȣ */
	CString	m_strOrgCustItem3;		/* ȸ��ó���׸� 3 */
	CString	m_strStopItemp;		/* ������ ȸ��ó���׸� */
	CString	m_strLimitItem;		/* ���ͽ��� ȸ��ó���׸� */

	void COpenPosition::Setdata(FX_CHE_STRUCT stOpenPosition)
	{
		char pBuff[128] = {0, };

		CHAR_TO_STRING(pBuff, stOpenPosition.szAccNo,		sizeof(stOpenPosition.szAccNo),	m_strAccountNum);
		CHAR_TO_STRING(pBuff, stOpenPosition.szFXCMPosID,	sizeof(stOpenPosition.szFXCMPosID),		m_strTicketNo);
		CHAR_TO_STRING(pBuff, stOpenPosition.szRate,		sizeof(stOpenPosition.szRate),			m_strRate);
		CHAR_TO_STRING(pBuff, stOpenPosition.szSide,		sizeof(stOpenPosition.szSide),			m_strSide);
		CHAR_TO_STRING(pBuff, stOpenPosition.szExeTime,		sizeof(stOpenPosition.szExeTime),  m_strExecutedTime);
		CHAR_TO_STRING(pBuff, stOpenPosition.szOrgCustItem1,	sizeof(stOpenPosition.szOrgCustItem1),		m_strCustItem1);
		CHAR_TO_STRING(pBuff, stOpenPosition.szOrgCustItem2,	sizeof(stOpenPosition.szOrgCustItem2),		m_strCustItem2);
		CHAR_TO_STRING(pBuff, stOpenPosition.szOrgCustItem3,	sizeof(stOpenPosition.szOrgCustItem3),		m_strCustItem3);
		CHAR_TO_STRING(pBuff, stOpenPosition.szOrgCustItem1, sizeof(stOpenPosition.szOrgCustItem1),   m_strOrgCustItem1);
		CHAR_TO_STRING(pBuff, stOpenPosition.szOrgCustItem2, sizeof(stOpenPosition.szOrgCustItem2),   m_strOrgCustItem2);
		CHAR_TO_STRING(pBuff, stOpenPosition.szOrgCustItem3, sizeof(stOpenPosition.szOrgCustItem3),   m_strOrgCustItem3);
		CHAR_TO_STRING(pBuff, stOpenPosition.szStopItem,	sizeof(stOpenPosition.szStopItem),		m_strStopItemp);
		CHAR_TO_STRING(pBuff, stOpenPosition.szLimitItem,	sizeof(stOpenPosition.szLimitItem),		m_strLimitItem);

		m_dAmount        = stOpenPosition.fAmount;        	
		m_dOpenPrice	 = stOpenPosition.fOpenMarkup;	 	
		m_dClosePrice	 = stOpenPosition.fCloseMarkup;	 
//		m_dCloseStop	 = stOpenPosition.dCloseStop;	 
//		m_dCloseLimit	 = stOpenPosition.dCloseLimit;	 	
		m_dTotalPL		 = stOpenPosition.fGrossPL;		 	
		m_dGrossPL		 = stOpenPosition.fTotalPL;		 		
		m_dInterest		 = stOpenPosition.fInterest;		 
		m_dPipCost		 = stOpenPosition.fPipCost;		
	}
}; //������ ����Ʈ


// ��ü��
class CPendingOrder
{
public:
	CString	m_strAccountNum;	/* �ֹ���ȣ */
	CString	m_strOrderNo;		/* �ֹ� ȸ��ó���׸� */
	CString	m_strRate;			// ��ü�ᰡ
	CString	m_strCustItem;		/* ����� */
	CString	m_strSide;			/* �Ÿű��� */
	double	m_dAmount;			/* ���� */
	double	m_dOrderPrice;		/* �ֹ�ȯ�� */
	double	m_dCloseStop;		/* ������ */
	double	m_dCloseLimit;		/* ���ͽ��� */
	CString	m_strOrdType;		/* �ֹ����� */
	CString	m_strStatus;		/* �ֹ����� */
	CString	m_strOrderTime;		/* �ֹ��ð� */

	void CPendingOrder::Setdata(FX_CHE_STRUCT stPendingOrder)
	{
		char pBuff[128] = {0, };

		CHAR_TO_STRING(pBuff, stPendingOrder.szAccNo,		sizeof(stPendingOrder.szAccNo),m_strAccountNum);
		CHAR_TO_STRING(pBuff, stPendingOrder.szOrderID,		sizeof(stPendingOrder.szOrderID),	m_strOrderNo);
		CHAR_TO_STRING(pBuff, stPendingOrder.szRate,		sizeof(stPendingOrder.szRate),			m_strRate);
		CHAR_TO_STRING(pBuff, stPendingOrder.SHCODE,		sizeof(stPendingOrder.SHCODE),	m_strCustItem);
		CHAR_TO_STRING(pBuff, stPendingOrder.szSide,		sizeof(stPendingOrder.szSide),		m_strSide);
		CHAR_TO_STRING(pBuff, stPendingOrder.szOrdType,		sizeof(stPendingOrder.szOrdType),	m_strOrdType);
		CHAR_TO_STRING(pBuff, stPendingOrder.szStatus,		sizeof(stPendingOrder.szStatus),	m_strStatus);
		CHAR_TO_STRING(pBuff, stPendingOrder.szOpenTime,	sizeof(stPendingOrder.szOpenTime), m_strOrderTime);
		
		m_dAmount	        = stPendingOrder.fOrderAmt;    
		m_dOrderPrice		= stPendingOrder.fOpenMarkup;   
//		m_dCloseStop		= stPendingOrder.dCloseStop;   
//		m_dCloseLimit		= stPendingOrder.dCloseLimit;
	}   //�ֹ� ����Ʈ
};
