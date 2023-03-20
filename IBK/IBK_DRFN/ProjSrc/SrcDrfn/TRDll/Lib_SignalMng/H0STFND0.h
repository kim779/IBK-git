
typedef struct
{
	char    gsAlertNum[4];      /* ��ȣ Ű����(���� 4�ڸ� ä��, ���� space) */
	char    sKey[4];
	char    gsAlertGubun[1];    /* System('S'), User('U')                   */
	char    gsDate[8];          /* ��ȣ �߻�����                            */
	char    gsTime[6];          /* ��ȣ �߻��ð�                            */
	char    gsTrueCnt[4];       /* ���縸�� �����(����+�ű�)               */
	char    gsDataCnt[4];       /* ���������                               */
	char    gsStatus[1];        /* ��ȣ�߻�:'S',�����̻�:'E',���� ����:'W'  */
	/* ���� �ʱ�ȭ: 'I', ���� ��������: 'K'     */
} ALERT_OUT_HEAD;
#define SZ_ALERT_OUT_HEAD  sizeof(ALERT_OUT_HEAD)

typedef struct
{
	char    gsJobFlag;                      /* �ű�:'N',������:'R',Ż��:'O' */
	char    gsCode[6];                      /* �����ڵ�                     */
	char    gsHname[20];                    /* �����                       */
	char    gsEntryCnt[4];                  /* ���� Ƚ��                    */

	char    gsManagement[1];                /* ��������                     */
	char    gsControl[1];                   /* �̻�޵� ����                */
	char    gsTradeStop[1];                 /* �ŷ����� ����                */
	char    gsInvestAttention[1];           /* �������� ����                */
	char    gsInsincerityNotice[1];         /* �Ҽ��ǰ��� ����              */
	char    gsConfidenceLimit[1];           /* �ſ����� ����                */
	char    gsCodeNotice[1];                /* �������                     */
	char    gsJungriMeme[1];                /* �����Ÿ� ����                */
	char    gsMarket[1];                    /* ���屸��                     */
	char    gsDepositMoney[3];              /* ���ű� ����                  */

	char    gsPrice[8];                     /* ���簡                       */
	char    gsSign[1];                      /* ���ϴ������ȣ             */
	char    gsChange[8];                    /* ���ϴ��                     */
	char    gsChgRate[6];                   /* ���ϴ������               */
	char    gsVolume[9];                    /* �ŷ���                       */
	char    gsVolumeRate[6];                /* �ŷ��� ���ϴ����            */
	char    gsAmount[9];                    /* �ŷ����                     */
} ALERT_OUT_LIST;
#define SZ_ALERT_OUT_LIST  sizeof(ALERT_OUT_LIST)