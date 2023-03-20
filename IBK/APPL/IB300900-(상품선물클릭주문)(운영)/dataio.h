#pragma once
typedef enum { 
	TK_PIBO3002      =   1,
	TK_PIBOFCHG      =   2,
	TK_PIBOFJGO      =   3,
	TK_PIBOFODR      =   4,
	TK_SDLDQ003      =   5,
	TK_SACMT238      =   6,
	TK_SONBQ101      =   7,
	TK_CODE_CTRL     = 100,
	TK_NOTIFY_CTRL   = 200,
	TK_ACCOUNT_CTRL1 = 254,
	TK_ACCOUNT_CTRL2 = 255,
} TR_KEY;

typedef struct _pibo3002_mid {
	char	code[8];	        // �����ڵ�
} pibo3002_mid;

#define MAX_FC	30

typedef struct _pibo3002_mod {
	char    curr[9];                // ���簡       
	char    diff[9];                // ���ϴ��     
	char    gvol[12];               // �ŷ���  
	char 	gamt[12];		// �ŷ����  
	char    rate[9];		// �����
	char    shga[9];                // ���Ѱ�       
	char    hhga[9];                // ���Ѱ� 
	char    gjga[9];                // ���ذ�       
	char    siga[9];                // �ð�         
	char    koga[9];                // ��         
	char    jega[9];                // ����         
	struct  {
		char     mdga[9];	// �ŵ�ȣ��            (999V99) 
		char     msga[9];       // �ż�ȣ��            (999V99)
		char     dvol[9];       // �ŵ�ȣ������        (���)  
		char     svol[9];       // �ż�ȣ������        (���)  
		char     dcnt[9];       // �ŵ�ȣ���Ǽ�
		char     scnt[9];       // �ż�ȣ���Ǽ�
	} hoga[5];		        // ����/�ɼ� ȣ��
	char    dvol[9];                // �ŵ�ȣ���Ѽ���
	char    svol[9];                // �ż�ȣ���Ѽ���
	char    dcnt[9];                // �ŵ�ȣ���ѰǼ�
	char    scnt[9];                // �ż�ȣ���ѰǼ�
	char 	sdch[9];		// �ܷ���
	char	mgjv[9];		// �̰�������
	struct  {
		char time[6];		// ü��ð� HHMMSS 
		char curr[9];		// ü�ᰡ��
		char cvol[6];		// ü�����
	} cheg[MAX_FC];
} pibo3002_mod;

typedef struct _pibofchg_mid {
    char accn[11];
    char pswd[8];
    char dlgb[1];    /* 0.All 1.ü�� 2.��ü��    */
    char juno[6];
    char sort[1];    /* 0.X   1.���� 2.����      */
    char date[8];
    char cod2[8];    /* �����ڵ�                 */
    char cpgb[1];    /* 0.All F.���� C.�ݿɼ� P.ǲ�ɼ� S.�������� X.Ư������ */
    char dsgb[1];    /* 0.All 1.�ŵ� 2.�ż�      */
    char dllf[1];    /* 0.MAP 1.DLL      */
} pibofchg_mid;

typedef struct _pibofchg_grid {
    char mono[6];    /* ���ֹ���ȣ       */
    char juno[6];    /* �ֹ�  ��ȣ       */
    char ojno[6];    /* ���ֹ� ��ȣ      */
    char cod2[8];    /* �����ڵ�         */
    char hnam[30];   /* �����           */
    char odgb[8];    /* �ֹ�����         */
    char hogb[20];   /* �ֹ�����         */
    char oprc[11];   /* �ֹ�����         */
    char oqty[7];    /* �ֹ�����         */
    char dlgb[4];    /* ü�ᱸ��         */
    char dprc[11];   /* ü�ᰡ��         */
    char dqty[7];    /* ü�����         */
    char dtim[6];    /* ü��ð�         */
    char wqty[7];    /* ��ü�����       */
    char hqty[7];    /* ����/��Ҽ���    */
    char stat[8];    /* ��������         */
    char time[6];    /* �����ð�         */
    char jseq[6];    /* ������ȣ         */
    char yseq[7];    /* ������ȣ         */
    char ecod[4];    /* �ź��ڵ�         */
    char dseq[4];    /* ü��Ƚ��         */
} pibofchg_grid;

typedef struct _pibofchg_mod {
    char    accn[11];
    char    nrec[4];
    pibofchg_grid grid[1];
} pibofchg_mod;

typedef struct _pibofodr_mid {
	char nrec[4];
	char odgb[1];    /* �ֹ����� : 1:�Ϲ� 2:���� 3:����                  */
	char mkgb[1];    /* Filler                                           */
	char mmgb[1];    /* �Ÿű��� : 1:�ŵ� 2:�ż� 3:���� 4: ���          */
	char accn[11];   /* ���¹�ȣ                                         */
	char pswd[8];    /* ��й�ȣ                                         */
	char ojno[12];   /* ���ֹ���ȣ                                       */
	char cod2[12];   /* �����ڵ�                                         */
	char jqty[8];    /* �ֹ�����                                         */
	char jprc[10];   /* �ֹ��ܰ�                                         */
	char hogb[2];    /* ȣ������                                         */
	char cdgb[1];    /* �������� : 0:X   1:IOC 2: FOK                    */
	char mdgb[1];    /* ������� : 1:�Ϻ� 2:����                         */
	char prgb[1];    /* ó������ : 'A,B,C,D' : ���� ���� 'X' : ������  */
	char usrf[40];
} pibofodr_mid;

typedef struct _pibofodr_grid {
	char    jmno[6];    /* �ֹ� ��ȣ        */
	char    ojno[6];    /* ���ֹ���ȣ       */
	char    emsg[80];   /* ���� �޼���      */
} pibofodr_grid;

typedef struct _pibofodr_mod {
	char    nrec[4];        /* ����        */
	pibofodr_grid grid[1];   /* ����        */
} pibofodr_mod;

typedef struct _SDLDQ003_mid {
	char In[5];				/* ���ڵ尹��                                 */
	char QryTp[1];			/* ��ȸ����       1:�׷���ȸ 2:������ȸ       */
	char GrpTp[2];			/* �׷������ڵ�   10:�ֽ�, 11:����, 12:ȥ��   */
	char GrpSeqn[20];		/* �׷��ȣ                                   */
	char GrpName[40];		/* �׷��                                     */
} SDLDQ003_mid;

typedef struct _SDLDQ003_grid {
	char GrpSeqn[20];		/* �׷��ȣ       */
	char GrpName[40];		/* �׷��         */
	char Acno[20];			/* �׷���¹�ȣ   */
	char Acnm[40];			/* ���¸�         */
	char Pswd[8];			/* ��й�ȣ       */
	char Rate[17];			/* ��           */
	char Mult[17];			/* �¼�����       */
	char Alias[40];			/* �׷���ºα�� */
} SDLDQ003_grid;

typedef struct _SDLDQ003_mod {
	SDLDQ003_mid mid;
	char In[5];				/* ���ڵ尹��     */
	SDLDQ003_grid grid[1];
} SDLDQ003_mod;

typedef struct _SACMT207_mid {
	char in[5];
	char acctNo[20];
	char password[8];
	char zTrxTp[1];
} SACMT207_mid;

typedef struct _SACMT207_mod {
	SACMT207_mid in;
	char out[5];
	char lPwdContErrCnt[9];
	char lAtmPwdContErrCnt[9];
	char lMtrsfPwdContErrCnt[9];
	char zAcntNm[40];
} SACMT207_mod;

typedef struct _SACMT238_mid {
	char In[5];
	char zAcntNo[20];
	char zPwd[8];
} SACMT238_mid;

typedef struct _SACMT238_mod {
	struct _SACMT238_mid mid;
	char Out[5];
	char zAvalYn[1];
} SACMT238_mod;

typedef struct _SONBQ101_mid {
	char In[5];                        /* ���ڵ尹��            */
	char zAcntNo[20];                  /* ���¹�ȣ              */
	char zPwd[8];                      /* ��й�ȣ              */
	char zQryTp[1];                    /* ��ȸ����              */
	char lOrdAmt[16];                  /* �ֹ��ݾ�              */
	char dRatVal[19];                  /* ������                */
	char zFnoIsuNo[32];                /* �����ɼ������ȣ      */
	char zBnsTp[1];                    /* �Ÿű���              */
	char dOrdPrc[13];                  /* �ֹ���                */
	char zFnoOrdprcPtnCode[2];         /* �����ɼ�ȣ�������ڵ�  */
} SONBQ101_mid;

typedef struct _SONBQ101_mod {
	SONBQ101_mid mid;
	char Out[5];                       /* ���ڵ尹��            */
	char zAcntNm[40];                  /* ���¸�                */
	char zQryDt[8];                    /* ��ȸ��                */
	char dCurPrc[13];                  /* ���簡                */
	char lOrdAbleQty[16];              /* �ֹ����ɼ���          */
	char lNewOrdAbleQty[16];           /* �ű��ֹ����ɼ���      */
	char lLqdtOrdAbleQty[16];          /* û���ֹ����ɼ���      */
	char lUsePreargMgn[16];            /* ��뿹�����űݾ�      */
	char lUsePreargMnyMgn[16];         /* ��뿹���������űݾ�  */
	char lOrdAbleAmt[16];              /* �ֹ����ɱݾ�          */
	char lMnyOrdAbleAmt[16];           /* �����ֹ����ɱݾ�      */
} SONBQ101_mod;

