///////////////////////////////////////��������
//�������
struct st_comheader
{
	char msgid[4];   //msgid
	char uvalue[16];   //unique value
	char filler[8];    
};

#define SZ_COMHEADER sizeof(struct st_comheader)


//����������
struct st_tagREG_PRE
{
	char rqst_CHNL_DCD[21];				/* ��û��ü�����ڵ�          (�ʼ�)    API�� ȣ���� ��ü�����ڵ�               */
	char tran_LGN_YN[2];				/* �α��ΰŷ�����            (�ʼ�)    Y:�α���, N:�����ŷ�                    */
	char tran_DTL_DCD[4];				/* �ŷ��󼼱����ڵ�          (    )    �������� ������ ���� ȭ���� �ŷ��󼼱����ڵ�*/
	char cprs_pubkey[1025];				/* ��༭����������Ű      (    )    ��༭����������Ű                    */
	char tran_elsg_ontt_con[1001];		/* �ŷ����ڼ����������      (    )    �ŷ����ڼ����������                    */
	char custno[21];					/* ����ȣ                  (    )    ����ȣ                                */
};

struct st_OUT_tagREG_PRE
{
	char ERR_CD[7];								/* �����ڵ�             (�ʼ�       )  �����ڵ�                                */
	char REL_TRAN_NO[21];						/* ����ŷ���ȣ         (������ �ʼ�)  ����ŷ���ȣ                            */
    char REL_TRAN_EASY_NO[11];					/* ���谣��������ȣ     (������ �ʼ�)  ���谣��������ȣ                        */
};

#define SZ_TAGREGPRE sizeof(struct st_tagREG_PRE)


//��������ȸ
typedef struct tagINQUIRY_REG_TRAN_BY_RELAY_REQ  //��������ȸ
{
    char        RQST_CHNL_DCD[20 + 1];			/* ��û��ü�����ڵ�          (�ʼ�)    API�� ȣ���� ��ü�����ڵ�               */
    char        REL_TRAN_NO[20 + 1];			/* ����ŷ���ȣ              (�ʼ�)    ����ŷ���ȣ                            */
    char        REL_TRAN_EASY_NO[10 + 1];		/* ���谣��������ȣ          (�ʼ�)    ���谣��������ȣ                        */
} INQUIRY_REG_TRAN_BY_RELAY_REQ;
#define SZ_INQUIRY_REG_TRAN_BY_RELAY_REQ    sizeof(INQUIRY_REG_TRAN_BY_RELAY_REQ)  //��������ȸ

typedef struct tagINQUIRY_REG_TRAN_BY_RELAY_RES  //��������ȸ
{
    char        ERR_CD[6 + 1];					/* �����ڵ�             (�ʼ�       )  �����ڵ�                                */
    char        REG_TRAN_YN[1 + 1];				/* ����޽�����Ͽ���   (������ �ʼ�)  ����޽�����Ͽ���(Y:���, N:�̵��)    */
    char        CRTC_PTRN_DCD[2 + 1];			/* �������������ڵ�     (REG_TRAN_YN='Y'�� ��� �ʼ�)  �������������ڵ�(10:PIN, 20:BIO, 30:PTN)*/
    char        BIO_CRTC_DCD[2 + 1];			/* BIO���������ڵ�      (REG_TRAN_YN='Y'�� ��� �ʼ�)  BIO���������ڵ�(00:������, 01:����, 02:�ȸ�) */
    char        BIO_CRTC_SUCC_YN[1 + 1];		/* ���̿�������������   (REG_TRAN_YN='Y'�� ��� �ʼ�)  ���̿�������������(Y:����, N:����)      */
    char        LAST_ERR_CD[6 + 1];				/* �����ֱٿ����ڵ�     (           )  �����ֱٿ����ڵ�                        */
    char        LAST_ERR_TS[17 + 1];			/* �����ֱٿ����Ͻ�     (           )  �����ֱٿ����Ͻ�                        */
} INQUIRY_REG_TRAN_BY_RELAY_RES;
#define SZ_INQUIRY_REG_TRAN_BY_RELAY_RES    sizeof(INQUIRY_REG_TRAN_BY_RELAY_RES)  //��������ȸ


//���� ���� ������ �����ŷ���ȣ�� ��ȸ�Ѵ�.
typedef struct tagINQUIRY_AUTH_TRAN_NO_BY_RELAY_REQ  //���� ���� ������ �����ŷ���ȣ�� ��ȸ�Ѵ�.
{  
    char        RQST_CHNL_DCD             [20 + 1];    /* ��û��ü�����ڵ�          (�ʼ�)    API�� ȣ���� ��ü�����ڵ�               */
    char        REL_TRAN_NO               [20 + 1];    /* ����ŷ���ȣ              (�ʼ�)    ����ŷ���ȣ                            */
    char        REL_TRAN_EASY_NO          [10 + 1];    /* ���谣��������ȣ          (�ʼ�)    ���谣��������ȣ                        */
} INQUIRY_AUTH_TRAN_NO_BY_RELAY_REQ;  //���� ���� ������ �����ŷ���ȣ�� ��ȸ�Ѵ�.
#define SZ_INQUIRY_AUTH_TRAN_NO_BY_RELAY_REQ    sizeof(INQUIRY_AUTH_TRAN_NO_BY_RELAY_REQ)  //���� ���� ������ �����ŷ���ȣ�� ��ȸ�Ѵ�.


typedef struct tagINQUIRY_AUTH_TRAN_NO_BY_RELAY_RES
{  
    char        ERR_CD                    [6 + 1];    /* �����ڵ�             (�ʼ�       )  �����ڵ�                                */
    char        AUTH_TRAN_NO              [20 + 1];   /* �����ŷ���ȣ         (������ �ʼ�)  �����ŷ���ȣ                            */
} INQUIRY_AUTH_TRAN_NO_BY_RELAY_RES;   //���� ���� ������ �����ŷ���ȣ�� ��ȸ�Ѵ�.
#define SZ_INQUIRY_AUTH_TRAN_NO_BY_RELAY_RES    sizeof(INQUIRY_AUTH_TRAN_NO_BY_RELAY_RES)  //���� ���� ������ �����ŷ���ȣ�� ��ȸ�Ѵ�.




//�α��� �ŷ� ���� ����ü -> ����޽����� �м��Ͽ�, ������� ��ġ���θ� �Ǵ��Ѵ�. 
typedef struct tagVERIFY_LOGIN_TRAN_BY_RELAY_REQ
{   
    char        RQST_CHNL_DCD             [20 + 1];    /* ��û��ü�����ڵ�          (�ʼ�)    API�� ȣ���� ��ü�����ڵ�               */
    char        REL_TRAN_NO               [20 + 1];    /* ����ŷ���ȣ              (�ʼ�)    ����ŷ���ȣ                            */
    char        REL_TRAN_EASY_NO          [10 + 1];    /* ���谣��������ȣ          (�ʼ�)    ���谣��������ȣ                        */
    char        AUTH_TRAN_NO              [20 + 1];    /* �����ŷ���ȣ              (�ʼ�)    �����ŷ���ȣ                            */
} VERIFY_LOGIN_TRAN_BY_RELAY_REQ;
#define SZ_VERIFY_LOGIN_TRAN_BY_RELAY_REQ   sizeof(VERIFY_LOGIN_TRAN_BY_RELAY_REQ)


typedef struct tagVERIFY_LOGIN_TRAN_BY_RELAY_RES
{      
    char        ERR_CD                    [6 + 1];    /* �����ڵ�             (�ʼ�       )  �����ڵ�                                */
    char        CUST_NO                   [20 + 1];    /* ����ȣ             (������ �ʼ�)  ����ȣ                                */
    char        VRFC_SUCC_YN              [1 + 1];    /* �ŷ�������������     (������ �ʼ�)  �ŷ�������������                        */
    char        CPRS_PUB_KEY              [1024 + 1];    /* ��༭����������Ű (������ �ʼ�)  ��༭����������Ű                    */
} VERIFY_LOGIN_TRAN_BY_RELAY_RES;
#define SZ_VERIFY_LOGIN_TRAN_BY_RELAY_RES   sizeof(VERIFY_LOGIN_TRAN_BY_RELAY_RES)



//���� �ŷ� ���� -> �����ŷ� ����޽����� �����Ѵ�.
typedef struct tagVERIFY_TRAN_BY_RELAY_REQ
{  
    char        RQST_CHNL_DCD             [20 + 1];    /* ��û��ü�����ڵ�          (�ʼ�)    API�� ȣ���� ��ü�����ڵ�               */
    char        REL_TRAN_NO               [20 + 1];    /* ����ŷ���ȣ              (�ʼ�)    ����ŷ���ȣ                            */
    char        REL_TRAN_EASY_NO          [10 + 1];    /* ���谣��������ȣ          (�ʼ�)    ���谣��������ȣ                        */
    char        AUTH_TRAN_NO              [20 + 1];    /* �����ŷ���ȣ              (�ʼ�)    �����ŷ���ȣ                            */
} VERIFY_TRAN_BY_RELAY_REQ;
#define SZ_VERIFY_TRAN_BY_RELAY_REQ sizeof(VERIFY_TRAN_BY_RELAY_REQ)


typedef struct tagVERIFY_TRAN_BY_RELAY_RES
{      
    char        ERR_CD                    [6 + 1];    /* �����ڵ�            (�ʼ�       )  �����ڵ�                                 */
    char        CUST_NO                   [20 + 1];    /* ����ȣ            (������ �ʼ�)  ����ȣ                                 */
    char        VRFC_SUCC_YN              [1 + 1];    /* �ŷ�������������    (������ �ʼ�)  �ŷ�������������                         */
} VERIFY_TRAN_BY_RELAY_RES;
#define SZ_VERIFY_TRAN_BY_RELAY_RES sizeof(VERIFY_TRAN_BY_RELAY_RES)

//OTP��ȸ
typedef struct tagOTP_SEARCH
{  
    char        RQST_CHNL_DCD             [20 + 1];    /* ��û��ü�����ڵ�          (�ʼ�)    API�� ȣ���� ��ü�����ڵ�               */
    char        CUST_NO                   [20 + 1];    /* ����ȣ					(������ �ʼ�)  ����ȣ - �α��� ������ �޾ƿ� ����Ÿ*/
    char        OTP_SEQ_NO                [12 + 1];    /* OTP�Ϸù�ȣ                             */
} OTP_SEARCH;
#define SZ_tagOTP_SEARCH sizeof(tagOTP_SEARCH)

typedef struct tagOTP_RES_SEARCH
{  
	char        ERR_CD                    [6 + 1];     /* �����ڵ�					 (�ʼ�       )  �����ڵ�                         */
	char        OTP_SEQ_NO		          [12 + 1];    /* OTP�Ϸù�ȣ                (�ʼ�       )                                   */
	char        CUST_NO					  [20 + 1];    /* ����ȣ					 (�ʼ�       )                                   */
	char        OTP_STTS_DCD	          [2 + 1];     /* 10 : �߱�,  30 : ���      (�ʼ�       )  (yyyyMMddHHmmsssss)              */
	char        OTP_ISNC_TS		          [17 + 1];     /* OTP�߱��Ͻ�               (�ʼ�       )  (yyyyMMddHHmmsssss)              */
	char        OTP_LOCK_TS		          [17 + 1];     /* OTP����Ͻ�               (�ʼ�       )  (yyyyMMddHHmmsssss)              */
	char        OTP_DCRD_TS		          [17 + 1];     /* OTP����Ͻ�               (�ʼ�       )  (yyyyMMddHHmmsssss)              */
	char        VLTE_STTG_YMD	          [8  + 1];     /* ��ȿ�Ⱓ���۳����        (�ʼ�       )                */
	char        VLTE_EXPY_YMD	          [8  + 1];     /* ��ȿ�Ⱓ��������        (�ʼ�       )                */
	char        OTP_EXPY_GDNC_YN          [1  + 1];     /* OTP�����������           (�ʼ�       )                */
	char        USR_CCTN_MCTL_ID          [200  + 1];   /* �̿������ӱ��ID          (�ʼ�       )                */
	char        OTP_PIN_MAX_EROR_ALWN_CNT [3  + 1];     /* OTP PIN�ִ�������Ƚ��   (�ʼ�       )                */
	char        OTP_PIN_EROR_CNT          [3  + 1];     /* OTP PIN����Ƚ��           (�ʼ�       )                */
	char        OTP_LOCK_YN               [1  + 1];     /* OTP��迩��               (�ʼ�       )                */
	char        OTP_LAST_VRFC_TS          [17  + 1];     /* OTP���������Ͻ�          (�ʼ�       )                */
	char        VRFC_EROR_CNT             [3  + 1];     /* ��������Ƚ��              (�ʼ�       )                */
} OTP_RES_SEARCH;
#define SZ_tagOTP_RES_SEARCH sizeof(tagOTP_RES_SEARCH)


//OTP����
typedef struct tagOTP_CERTREG
{  
    char        RQST_CHNL_DCD             [20 + 1];    /* ��û��ü�����ڵ�          (�ʼ�)    API�� ȣ���� ��ü�����ڵ�               */
	char        MSB_VRSN_VL               [32 + 1];    /* ���̺귯�� �������� (Ÿ��ü ����  *****)					              */
    char        CUST_NO                   [20 + 1];    /* ����ȣ            (������ �ʼ�)  ����ȣ                                 */
    char        OTP_SEQ_NO                [12 + 1];    /* OTP�Ϸù�ȣ                             */
	char        OTP_TRAN_NO               [20 + 1];    /* OTP�ŷ���ȣ                             */
    char        PIN_DATA_CON			  [64 + 1];    /* �ɵ����ͳ��� (HexString, 64Byte)                                 */
	char        PRE_TRAN_USE_YN			  [1 + 1];    /* �����ŷ���뿩��                              */
	char        OTP_CLI_VL				  [6 + 1];    /* ���Է�OTP��                              */
} OTP_CERTREG;
#define SZ_tagOTP_CERTREG sizeof(tagOTP_CERTREG)  //OTP����

typedef struct tagOTP_RES_CERTREG
{  
	char        ERR_CD                    [6 + 1];    /* �����ڵ�					(�ʼ�       )  �����ڵ�                                 */
	char        OTP_VRFC_SUCC_YN          [1 + 1];    /* OTP������������            (�ʼ�       )  �����ڵ�                                 */
	char        OTP_VRFC_MAX_EROR_ALWN_CNT[3 + 1];    /* OTP�����ִ�������Ƚ��    (�ʼ�       )  �����ڵ�                                 */
	char        OTP_VRFC_EROR_CNT         [3 + 1];    /* OTP��������Ƚ��            (�ʼ�       )  �����ڵ�                                 */
} OTP_RES_CERTREG;
#define SZ_tagOTP_RES_CERTREG sizeof(tagOTP_RES_CERTREG)

//������ ��ȸ (IBK MBGID 0104)
//(1) ���� �ܸ��� �߱޵� �������� ��ȸ�Ѵ�. (���ID�� APP ID�� �̿��Ͽ� ��ȸ�Ѵ�.) 
//(2) �Ϸù�ȣ �� ����ȣ�� �ʿ信 ���� �����Ͽ� �ش�. 
//��. �������Ϸù�ȣ(CERT_SEQ_NO)�� ������ ��� �ش� �������Ϸù�ȣ�� ��ȸ
//   - ��ȸ ��� ������ �� 030101(��ϵ��� ���� ������ �Ϸù�ȣ) �����ڵ� ����
//   - ��ȸ ��� ���� �� 000000(����) �����ڵ� �� ��ȸ��� ����
//��. ����ȣ(CUST_NO)�� ������ ��� �ش� ����ȣ+�̿������ӱ��ID+����������(����)���� ��ȸ <------------------------------ �̰ɾ��� �ɵ� �ϴ�
//	 - ��ȸ ��� ������ �� 030102(��ϵ��� ���� ����ȣ) �����ڵ� ����
//	 - ��ȸ ��� ���� �� 000000(����) �����ڵ� �� ��ȸ��� ���� 
//��. �������Ϸù�ȣ(CERT_SEQ_NO)+����ȣ(CUST_NO) ������ ��� �ش� �������Ϸù�ȣ�� ���� ��ȸ �� ����ȣ ��ġ ���� Ȯ��
//	 - ��ȸ ��� ������ �� 030101(��ϵ��� ���� ������ �Ϸù�ȣ) �����ڵ� ����
//	 - ��ȸ ��� ���� �� �Է����� ���� ����ȣ(CUST_NO)�� ��ġ ���� üũ
//		. ����ġ�ϴ� ��� 030103(������ �Ϸù�ȣ�� ����ȣ ����ġ) �����ڵ� ����
//		. ��ġ�ϴ� ��� 000000(����) �����ڵ� �� ��ȸ��� ����
//(3) ������ ��ȸ�� �̿������ӱ��ID�� ��ü�����ڵ带 �̿��Ͽ� �����⿡ ��ȿ�� ID�� ��ȸ�Ѵ�.

//������ ��ȸ
typedef struct tagCERT_INQUIRY
{  
    char        RQST_CHNL_DCD             [20 + 1];    /* ��û��ü�����ڵ�          (�ʼ�)    API�� ȣ���� ��ü�����ڵ�               */
	char        CERT_SEQ_NO               [12 + 1];    /* ������ �Ϸù�ȣ												              */
    char        CUST_NO                   [20 + 1];    /* ����ȣ					                                */
    char        USR_CCTN_MCTL_ID          [200 + 1];    /* �̿������ӱ��ID          (�ʼ�)���� �ϴµ� ����ȣ�θ� ��ȸ�ϸ� �����شٰ� ��        */
	char        NCFR_UNIQ_NO              [32 + 1];    /* ���������ȣ                           */
} CERT_INQUIRY;
#define SZ_tagCERT_INQUIRY sizeof(tagCERT_INQUIRY)

typedef struct tagCERT_RES_INQUIRY
{  
    char        ERR_CD                    [6 + 1];     /* �����ڵ�					 (�ʼ�       )                                   1*/
	char        CERT_SEQ_NO               [12 + 1];    /* �������Ϸù�ȣ             (�ʼ�       )                                   2*/
	char        MSB_CERT_SEQ_NO           [15 + 1];    /* MSB�������Ϸù�ȣ          (�ʼ�       )                                   3*/
	char        CUST_NO                   [20 + 1];    /* ����ȣ                   (�ʼ�       )                                   4*/
	char        RQST_CHNL_DCD             [20 + 1];    /* ��ü�����ڵ�               (�ʼ�       )                                   5*/
	char        CERT_STTS_DCD             [2  + 1];    /* ���������±����ڵ�         (�ʼ�       )                                   6*/
	char        CERT_ISNC_TS              [17 + 1];    /* �������߱��Ͻ�             (�ʼ�       )                                   7*/
	char        CERT_DCRD_TS              [17 + 1];    /* ����������Ͻ�             (�ʼ�       )                                   8*/
	char        VLTE_STTG_YMD             [8 + 1];     /* ��ȿ�Ⱓ���۳����         (�ʼ�       )                                   9*/
	char        VLTE_EXPY_YMD             [8 + 1];     /* ��ȿ�Ⱓ��������         (�ʼ�       )                                   10*/
	char        USR_CCTN_MCTL_ID          [200 + 1];   /* �̿������ӱ��ID           (�ʼ�       )                                   11*/
	char        USR_CCTN_MCTL_OPSS_DCD    [2 + 1];     /* �ƿ������ӱ��OS�����ڵ�   (�ʼ�       )                                   12*/
	char        USR_CCTN_MCTL_OPSS_VRSN_VL[50 + 1];    /* �̿������ӱ��OS������     (�ʼ�       )                                   13*/
	char        USR_CCTN_MCTL_MDL_NM	  [100 + 1];   /* �̿������ӱ��𵨸�       (�ʼ�       )                                   14*/
	char        PIN_LOCK_TS				  [17 + 1];    /* PIN����Ͻ�                (�ʼ�       )                                   15*/
	char        PIN_LOCK_YN				  [1 + 1];     /* PIN��迩��                (�ʼ�       )                                   16*/
	char        PIN_EROR_CNT			  [3 + 1];     /* PIN����Ƚ��                (�ʼ�       )                                   17*/
	char        BIO_LOCK_TS			      [17 + 1];    /* BIO����Ͻ�                (�ʼ�       )                                   18*/
	char        BIO_LOCK_YN			      [1 + 1];     /* BIO��迩��                (�ʼ�       )                                   19*/
	char        BIO_USE_YN			      [1 + 1];     /* BIO��뿩��                (�ʼ�       )                                   20*/
	char        BIO_EROR_CNT     	      [3 + 1];     /* BIO����Ƚ��                (�ʼ�       )                                   21*/
	char        BIO_DISUSE_TS     	      [17 + 1];    /* BIO��������Ͻ�            (�ʼ�       )                                   22*/
	char        BIO_CRTC_DCD     	      [2 + 1];     /* BIO���������ڵ�            (�ʼ�       )                                   23*/
	char        NCFR_UNIQ_NO     	      [32 + 1];    /* ���������ȣ             (�ʼ�       )                                   24*/
	char        PRIV_CTFC_RE_ERLM_OBJT_YN [1 + 1];     /* ����Ȯ�οϷῩ��           (�ʼ�       )                                   25*/
	char        PRIV_CTFC_ERLM_MTHD_CD    [2 + 1];     /* �缳������Ϲ���ڵ�       (�ʼ�       )                                   26*/
	char        PRSF_CFRM_MTHD_CD         [2 + 1];     /* ����Ȯ�ι���ڵ�           (�ʼ�       )                                   27*/
	char        NCFR_UNIQ_NO_REG_TS       [17 + 1];    /* ����Ȯ�ι���ڵ����Ͻ�   (�ʼ�       )                                   28*/
	char        NCFR_STATUS_MDFY_TS       [17 + 1];    /* ��������Ȯ�λ��º����Ͻ� (�ʼ�       )                                   29*/
} CERT_RES_INQUIRY;
#define SZ_tagCERT_RES_INQUIRY sizeof(tagCERT_RES_INQUIRY)


//. ������ �α��� �����ŷ� ��� (IBK MBGID 0105)
//(1) ������������� ���� �α����� ó���ϱ� ���� �����ŷ��� ó���Ѵ�.
//(2) �α��� ���μ����� ������ ������ �����ϰ�, ���� �����ŷ����� �ش� ������ ������ ������ �����ϴ� ���μ���
//(3) �����ŷ��� ��ü�����ڵ�� �Ϸù�ȣ, �̿������ӱ��ID�� üũ�Ͽ�, ��ȿ�� ���ӱ�� ���ΰ� �Ǵ��� �Ǹ� ������ ������ �ŷ���ȣ�� �Բ� Ŭ���̾�Ʈ�� �����Ѵ�.
//(5) ��ü�ν������� �����Ǿ��ٴ� �޽����� ���� ���, ��ü������ ������Ų��.
//(6) ȯ�漳���� ��ϵǾ� �ִ� ���� �������� ���� �̿��Ͽ�, ������ ������ ������ ��� ���� �������θ� 'Y'�� �����Ͽ� ������ �ش�.

//������ �α��� �����ŷ� ���
typedef struct tagRegLogin_PRETRAN
{  
    char        RQST_CHNL_DCD             [20 + 1];    /* ��û��ü�����ڵ�          (�ʼ�)    API�� ȣ���� ��ü�����ڵ�               */
	char        CERT_SEQ_NO               [12 + 1];    /* ������ �Ϸù�ȣ			(�ʼ�) 									              */
    char        MSB_VRSN_VL               [32 + 1];    /* ���̺귯�� ��������       (�ʼ�) 					              */                              
    char        USR_CCTN_MCTL_ID          [200 + 1];    /* �̿������ӱ��ID         (�ʼ�)���� �ϴµ� ����ȣ�θ� ��ȸ�ϸ� �����شٰ� ��        */
	char        BIO_INFO_MDFC_YN          [1 + 1];    /* BIO�������濩��            (�ʼ�)                 */
} RegLogin_PRETRAN;
#define SZ_tagRegLogin_PRETRAN sizeof(tagRegLogin_PRETRAN)


typedef struct tagRegLogin_RES_PRETRAN
{  
    char        ERR_CD                    [6 + 1];     /* �����ڵ�					 (�ʼ�       )                                   1*/
	char        CERT_TRAN_NO              [20 + 1];    /* �������ŷ���ȣ             (�ʼ�       )                                   2*/
	char        LGN_ELSG_ONTT_CON         [1000 + 1];  /* �α������ڼ����������     (�ʼ�       )                                   3*/
	char        EXPY_GDNC_YN              [1 + 1];     /* �����������               (�ʼ�       )                                   4*/
	char        BIO_INFO_MDFC_YN          [1 + 1];     /* BIO�������濩��            (�ʼ�       )                                   5*/
	char        VLTE_STTG_YMD             [8  + 1];    /* ��ȿ�Ⱓ���۳����         (�ʼ�       )                                   6*/
	char        VLTE_EXPY_YMD             [8 + 1];     /* ��ȿ�Ⱓ��������         (�ʼ�       )                                   7*/
	char        PIN_EROR_CNT              [3 + 1];     /* PIN����Ƚ��                (�ʼ�       )                                   8*/
	char        BIO_USE_YN                [1 + 1];     /* BIO��뿩��                (�ʼ�       )                                   9*/
	char        BIO_CRTC_DCD              [2 + 1];     /* BIO���������ڵ�            (�ʼ�       )                                   10*/
} RegLogin_RES_PRETRAN;
#define SZ_tagRegLogin_RES_PRETRAN sizeof(tagRegLogin_RES_PRETRAN)

/////////////////////////////////////////////////