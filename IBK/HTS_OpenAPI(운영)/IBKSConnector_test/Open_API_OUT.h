#pragma once

#pragma	pack(1)

/******************************************************************************
* NAME : tr1001
* DESC : ���� �ü� ��ȸ
 ******************************************************************************/
struct tr1001_mid
{
	char cod2[6];
};
#define L_tr1001_mid		sizeof(struct tr1001_mid)
struct tr1001_mod
{
    char    cod2[12];       /* RTS �����ڵ�     */
    char    jmgb[1];        /* ���񱸺�(+KP, -KQ)*/
    char    curr[9];        /* ���簡           */
    char    diff[9];        /* ���ϴ��         */
    char    gvol[12];       /* �ŷ���           */
    char    gamt[12];       /* �ŷ����         */
    char    jvol[12];       /* ���� �ŷ���      */
    char    rate[9];        /* �����           */
    char    shga[9];        /* ���Ѱ�           */
    char    hhga[9];        /* ���Ѱ�           */
    char    gjga[9];        /* ���ذ�           */
    char    siga[9];        /* �ð�             */
    char    koga[9];        /* ��             */
    char    jega[9];        /* ����             */
    char    jgbn[3];        /* ���ű���         */
    struct  {
        char    mdga[9];    /* �ŵ�ȣ��         */
        char    msga[9];    /* �ż�ȣ��         */
        char    dvol[9];    /* �ŵ�ȣ������     */
        char    svol[9];    /* �ż�ȣ������     */
        char    dcha[9];    /* �����ŵ�ȣ������ */
        char    scha[9];    /* �����ż�ȣ������ */
        char    dcnt[9];    /* �ŵ�ȣ���Ǽ�     */
        char    scnt[9];    /* �ż�ȣ���Ǽ�     */
    } hoga[10];             /* ����/�ɼ� ȣ��   */
    char    dvol[9];        /* ȣ���Ѽ��� : �ŵ�*/
    char    svol[9];        /*            : �ż�*/
    char    dcha[9];        /* ��������ѷ�:�ŵ�*/
    char    scha[9];        /*             :�ż�*/
    char    sdch[9];        /* �ܷ���(svol-dvol)*/
    char    sum4[9];        /* �����հ� : 5��   */
    char    sum9[9];        /* �����հ� : 9��   */
    char    jggy[9];        /* ���ű���         */
    char    jqty[9];        /* �ֹ�����         */
};
#define L_tr1001_mod		sizeof(struct tr1001_mod)


/******************************************************************************
* NAME : tr1201
* DESC : ���� �ֹ�
 ******************************************************************************/
struct tr1201_mid
{
	char gubn[2];	/* ^C : ���α׷��Ÿ�	2014.04.18 �߰�				*/
	char nrec[4];	/* �ݺ�Ƚ��                                         */
	char odgb[1];	/* �ֹ����� : 1:�Ϲ� 2:���� 3:����                  */
	char mkgb[1];	/* Filler                                           */
	char mmgb[1];	/* �Ÿű��� : 1:�ŵ� 2:�ż� 3:���� 4: ���          */
	char accn[11];	/* ���¹�ȣ                                         */
	char pswd[8];	/* ��й�ȣ                                         */
	char ojno[12];	/* ���ֹ���ȣ                                       */
	char fcod[12];	/* �����ڵ�                                         */
	char jqty[8];	/* �ֹ�����                                         */
	char jprc[10];	/* �ֹ��ܰ�                                         */
	char hogb[2];	/* ȣ������                                         */
	char cdgb[1];	/* �������� : 0:X   1:IOC 2: FOK                    */
	char mdgb[1];	/* ������� : 1:�Ϻ� 2:����                         */
	char prgb[1];	/* ó������ : 'A,B,C,D' : ���� ���� 'X' : ������  */
	char usrf[40];	
	char pggb[2];	/* ���α׷�(00:�Ϲ� 01:���� 03:������) 2014.04.18 �߰� */
};
#define L_tr1201_mid		sizeof(struct tr1201_mid)
struct tr1201_mod
{
	char nrec[4];
	char jmno[6];	/* �ֹ���ȣ   */
	char ojno[6];	/* ���ֹ���ȣ */
	char emsg[80];	/* �����޼��� */
};
#define L_tr1201_mod		sizeof(struct tr1201_mod)


/******************************************************************************
* NAME : tr1211
* DESC : ���� ü�᳻�� ��ȸ
 ******************************************************************************/
struct tr1211_mid
{
	char	gubn[1];	/* '>' */						//2015.05.28  �����߰�
    char    acno[11];
    char    pswd[8];
    char    date[8];
    char    cod2[12];
    char    fill[5];    /* �����ֹ���ȣ             */	//2015.05.28  filler�߰�
    char    dsgb[1];    /* 0.All 1.�ŵ� 2.�ż�      */
    char    otim[1];    /* 0.All 1.���� 2.�ð���    */
    char    sygb[1];    /* 0.All 1.�ſ� 2.�㺸      */
    char    dllf[1];    /* 0.MAP 1.DLL              */
    char    dlgb[1];    /* 0.All 1.ü�� 2.��ü��    */
    char    sort[1];    /* 1.���� 2.����            */
	char    juno[10];    /* �����ֹ���ȣ             */	//2015.05.28 5 -> 10�� ����
};
#define L_tr1211_mid		sizeof(struct tr1211_mid)
struct tr1211_grid
{
    char    juno[10];    /* �ֹ�   ��ȣ      */	//2015.05.28 5 -> 10�� ����
    char    ojno[10];    /* ���ֹ� ��ȣ      */	//2015.05.28 5 -> 10�� ����
    char    cod2[12];   /* �����ڵ�         */
    char    hnam[40];   /* �����           */
    char    odgb[20];   /* �ֹ�����         */
    char    mcgb[20];   /* ���뱸��         */
    char    hogb[20];   /* ȣ������         */
    char    oprc[12];   /* �ֹ�����         */
    char    oqty[12];   /* �ֹ�����         */
    char    dprc[12];   /* ü�ᰡ��         */
    char    dqty[12];   /* ü�����         */
    char    tqty[12];   /* ü�������       */
    char    wqty[12];   /* ��ü�����       */
    char    stat[20];   /* ��������         */
    char    time[8];    /* �ֹ��ð�         */
};
#define L_tr1211_grid		sizeof(struct tr1211_grid)
struct tr1211_mod
{
    char    acno[11];                 /* ���¹�ȣ         */
    char    nrec[4];                  /* �ݺ�Ƚ��         */
    struct  tr1211_grid grid[1];
};
#define L_tr1211_mod		sizeof(struct tr1211_mod)


/******************************************************************************
* NAME : tr1221
* DESC : ���� �ܰ��� ��ȸ
 ******************************************************************************/
struct  tr1221_mid {   
    char    acno[11];
    char    pswd[8];
    char    cod2[12];
    char    allf[1];    /* 0.�ſ����� 1.All 2.�ſ� */
    char    fill[10];
};
#define L_tr1221_mid		sizeof(struct tr1221_mid)
struct  tr1221_grid {   
    char    cod2[12];   /* �����ڵ�         */
    char    hnam[40];   /* �����           */
    char    jgyh[2];    /* �ܰ�����         */
    char    jqty[10];   /* �ܰ����         */
    char    xqty[10];   /* �ŵ�����         */
    char    pamt[10];   /* ������հ�       */
    char    mamt[15];   /* ���Աݾ�         */
    char    curr[10];   /* ���簡     (*)   */
	char    rate[10];   /* �����           */		
	char    diff[10];   /* ���             */    
    char    camt[15];   /* �򰡱ݾ�         */
    char    tamt[15];   /* �򰡼���   (*)   */
    char    srat[10];   /* �򰡼��ͷ� (*)   */
    char    sycd[2];    /* �ſ��ڵ�         */
    char    sydt[8];    /* ������           */
    char    samt[15];   /* �ſ�ݾ�         */
};
#define L_tr1221_grid		sizeof(struct tr1221_grid)
struct  tr1221_mod   {   
    char    acno[11];                 /* ���¹�ȣ         */
    char    nrec[4];                  /* �ݺ�Ƚ��         */
    struct  tr1221_grid  grid[1];
};
#define L_tr1221_mod		sizeof(struct tr1221_mod)


/******************************************************************************
* NAME : tr1231
* DESC : ���� �ֹ����ɼ�����ȸ
 ******************************************************************************/
struct tr1231_mid
{
	char In[5];                             /* ���ڵ尹��                     */     
	char zBnsTp[1];                         /* �Ÿű���                       */
	char zAcntNo[20];                       /* ���¹�ȣ                       */
	char zPwd[8];                           /* ��й�ȣ                       */
	char zIsuNo[12];                        /* �����ȣ                       */
	char dOrdPrc[13];                       /* �ֹ���                         */
	char zCommdaCode[2];                    /* ��Ÿ�ü�ڵ�                   */
	char zLoanPtnCode[2];                   /* ���������ڵ�                   */
};
#define L_tr1231_mid		sizeof(struct tr1231_mid)
struct tr1231_mod
{
	struct tr1231_mid mid;
	char Out[5];                            /* ���ڵ尹��                     */
	char zAcntNm[40];                       /* ���¸�                         */
	char zIsuNm[40];                        /* �����                         */
	char zBnsTpNm[10];                      /* �Ÿű���                       */
	char lDps[16];                          /* ������                         */
	char lSubstAmt[16];                     /* ���ݾ�                       */
	char lMnyMgn[16];                       /* �������űݾ�                   */
	char lSubstMgn[16];                     /* ������űݾ�                   */
	char lRcvblSumAmt[16];                  /* �̼��հ�ݾ�                   */
	char lRuseOrdAbleAmt[16];               /* �����ֹ����ɱݾ�             */
	char lMnyRuseObjAmt[16];                /* ����������ݾ�             */
	char lSubstRuseObjAmt[16];              /* ���������ݾ�             */
	char lPrdayRuseOrdAbleAmt[16];          /* ���������ֹ����ɱݾ�         */
	char lCrdayRuseOrdAbleAmt[16];          /* ���������ֹ����ɱݾ�         */
	char lMnyOrdAbleAmt[16];                /* �����ֹ����ɱݾ�               */
	char lSubstOrdAbleAmt[16];              /* ����ֹ����ɱݾ�               */
	char lCrdtPldgRuseAmt[16];              /* �ſ�㺸����ݾ�             */
	char lSeOrdAbleAmt[16];                 /* �ŷ��ұݾ�                     */
	char lKdqOrdAbleAmt[16];                /* �ڽ��ڱݾ�                     */
	char lAbleAmt[16];                      /* ���ɱݾ�                       */
	char lMgnRat100pctOrdAbleAmt[16];       /* ���űݷ�100�ۼ�Ʈ�ֹ����ɱݾ�  */
	char lMgnRat100OrdAbleQty[16];          /* ���űݷ�100�ۼ�Ʈ�ֹ����ɼ���??*/
	char dLoanPldgRat[9];                   /* ����㺸��                     */
	char dPldgMaintRat[7];                  /* �㺸��������                   */
	char lOrdAbleQty[16];                   /* �ֹ����ɼ���                   */
	char lUnercBuyOrdAmt[16];               /* ��ü��ż��ֹ��ݾ�             */
	char dCmsnAmtRat[11];                   /* ��������                       */
	char lAddCmsnAmtLevyAmt[16];            /* �߰�������¡���ݾ�             */
	char lBalQty[16];                       /* �ܰ����                       */
	char lHtsOrdAbleAmt[16];                /* HTS�ֹ����ɱݾ�                */
	char lD1Dps[16];                        /* D+1������                      */
	char lD2Dps[16];                        /* D2������                       */
	char lPrdayRuseOrdAbleQty[16];          /* ���������ֹ����ɼ���         */
	char lCrdayRuseOrdAbleQty[16];          /* ���������ֹ����ɼ���         */
	char lCrdtOrdAbleAmt[16];               /* �ſ��ֹ����ɱݾ�               */
};
#define L_tr1231_mod		sizeof(struct tr1231_mod)


/******************************************************************************
* NAME : tr3001
* DESC : �����ɼ� �ü� ��ȸ
 ******************************************************************************/
struct tr3001_mid
{
	char cod2[8];
};
#define L_tr3001_mid		sizeof(struct tr3001_mid)
struct  tr3001_mod {   
    char    curr[9];        /* ���簡           */
    char    diff[9];        /* ���ϴ��         */
    char    gvol[12];       /* �ŷ���           */
    char    gamt[12];       /* �ŷ����         */
    char    rate[9];        /* �����           */
    char    shga[9];        /* ���Ѱ�           */
    char    hhga[9];        /* ���Ѱ�           */
    char    gjga[9];        /* ���ذ�           */
    char    siga[9];        /* �ð�             */
    char    koga[9];        /* ��             */
    char    jega[9];        /* ����             */
    struct  {
        char    mdga[9];    /* �ŵ�ȣ��         */
        char    msga[9];    /* �ż�ȣ��         */
        char    dvol[9];    /* �ŵ�ȣ������     */ 
        char    svol[9];    /* �ż�ȣ������     */ 
        char    dcnt[9];    /* �ŵ�ȣ���Ǽ�     */
        char    scnt[9];    /* �ż�ȣ���Ǽ�     */
    } hoga[5];  
    char    dvol[9];        /* ȣ���Ѽ��� : �ŵ�    */
    char    svol[9];        /*            : �ż�    */
    char    dcnt[9];        /* �ŵ�ȣ���Ǽ�         */
    char    scnt[9];        /* �ż�ȣ���Ǽ�         */
    char    sdch[9];        /* �ܷ���(svol-dvol)    */
    char    mgjv[9];        /* �̰�����������       */
};


/******************************************************************************
* NAME : tr3201
* DESC : �����ɼ� �ֹ�
 ******************************************************************************/
struct tr3201_mid
{
	char gubn[2];	/* ^C : ���α׷��Ÿ�	2014.04.18 �߰�				*/
	char nrec[4];	/* �ݺ�Ƚ��                                         */
	char odgb[1];	/* �ֹ����� : 1:�Ϲ� 2:���� 3:����                  */
	char mkgb[1];	/* Filler                                           */
	char mmgb[1];	/* �Ÿű��� : 1:�ŵ� 2:�ż� 3:���� 4: ���          */
	char accn[11];	/* ���¹�ȣ                                         */
	char pswd[8];	/* ��й�ȣ                                         */
	char ojno[12];	/* ���ֹ���ȣ                                       */
	char fcod[12];	/* �����ڵ�                                         */
	char jqty[8];	/* �ֹ�����                                         */
	char jprc[10];	/* �ֹ��ܰ�                                         */
	char hogb[2];	/* ȣ������                                         */
	char cdgb[1];	/* �������� : 0:X   1:IOC 2: FOK                    */
	char mdgb[1];	/* ������� : 1:�Ϻ� 2:����                         */
	char prgb[1];	/* ó������ : 'A,B,C,D' : ���� ���� 'X' : ������  */
	char usrf[40];	
	char pggb[2];	/* ���α׷�(00:�Ϲ� 01:���� 03:������) 2014.04.18 �߰� */
};
#define L_tr3201_mid		sizeof(struct tr3201_mid)
struct tr3201_mod
{
	char nrec[4];
	char jmno[6];	/* �ֹ���ȣ   */
	char ojno[6];	/* ���ֹ���ȣ */
	char emsg[80];	/* �����޼��� */
};
#define L_tr3201_mod		sizeof(struct tr3201_mod)


/******************************************************************************
* NAME : tr3211
* DESC : �����ɼ� ü�᳻�� ��ȸ
 ******************************************************************************/
struct tr3211_mid
{
	char acno[11];
	char pswd[8];
	char dlgb[1];		/* 0.All 1.ü�� 2.��ü��                                */
	char juno[6];
	char sort[1];		/* 0.X   1.���� 2.����                                  */
	char date[8];
	char cod2[8];		/* �����ڵ�                                             */
	char cpgb[1];		/* 0.All F.���� C.�ݿɼ� P.ǲ�ɼ� S.�������� X.Ư������ */
	char dsgb[1];		/* 0.All 1.�ŵ� 2.�ż�                                  */
	char dllf[1];		/* 0.MAP 1.DLL                                          */	
};
#define L_tr3211_mid		sizeof(struct tr3211_mid)
struct tr3211_grid {
	char    mono[6];    /* ���ֹ���ȣ       */
	char    juno[6];    /* �ֹ�  ��ȣ       */
	char    ojno[6];    /* ���ֹ� ��ȣ      */
	char    cod2[8];    /* �����ڵ�         */
	char    hnam[30];   /* �����           */
	char    odgb[8];    /* �ֹ�����         */
	char    hogb[20];   /* �ֹ�����         */
	char    oprc[11];   /* �ֹ�����         */
	char    oqty[7];    /* �ֹ�����         */
	char    dlgb[4];    /* ü�ᱸ��         */
	char    dprc[11];   /* ü�ᰡ��         */
	char    dqty[7];    /* ü�����         */
	char    dtim[6];    /* ü��ð�         */
	char    wqty[7];    /* ��ü�����       */
	char    hqty[7];    /* ����/��Ҽ���    */
	char    stat[8];    /* ó������         */
	char    time[6];    /* ó���ð�         */
	char    jseq[6];    /* ������ȣ         */
	char    yseq[7];    /* ������ȣ         */
	char    ecod[4];    /* �ź��ڵ�         */
	char    dseq[4];    /* ü��Ƚ��         */
};
#define L_tr3211_grid		sizeof(struct tr3211_grid)
struct tr3211_mod {
	char    acno[11];                 /* ���¹�ȣ         */
	char    nrec[4];                  /* �ݺ�Ƚ��         */
	struct  tr3211_grid grid[1];
};
#define L_tr3211_mod		sizeof(struct tr3211_mod)


/******************************************************************************
* NAME : tr3221
* DESC : �����ɼ� �ܰ��� ��ȸ
 ******************************************************************************/
struct tr3221_mid {
	char acno[11];
	char pswd[8];
	char date[8];
	char cod2[8];
};
#define L_tr3221_mid		sizeof(struct tr3221_mid)
struct tr3221_grid {
	char    cod2[8];    /* �����ڵ�         */
	char    hnam[30];   /* �����           */
	char    dsgb[6];    /* ����             */
	char    jqty[10];   /* ��������         */
	char    xqty[10];   /* û�갡�ɼ���     */
	char    pamt[10];   /* ��հ�/���갡    */
	char    curr[10];   /* ���簡           */
	char    diff[10];   /* ���ϴ��         */
	char    camt[15];   /* �򰡱ݾ�         */
	char    tamt[15];   /* �򰡼���         */
	char    srat[10];   /* ���ͷ�           */
	char    mamt[15];   /* ���Աݾ�         */
};
#define L_tr3221_grid		sizeof(struct tr3221_grid)
struct tr3221_mod {
	char    acno[11];                 /* ���¹�ȣ         */
	char    nrec[4];                  /* �ݺ�Ƚ��         */
	struct  tr3221_grid grid[1];
};
#define L_tr3221_mod		sizeof(struct tr3221_mod)


/******************************************************************************
* NAME : tr3231
* DESC : �����ɼ� �ֹ����ɼ��� ��ȸ
 ******************************************************************************/
struct tr3231_mid
{
	char In[5];                    /* ���ڵ尹��            */
	char zAcntNo[20];              /* ���¹�ȣ              */
	char zPwd[8];                  /* ��й�ȣ              */
	char zQryTp[1];                /* ��ȸ����              */
	char lOrdAmt[16];              /* �ֹ��ݾ�              */
	char dRatVal[19];              /* ������                */
	char zFnoIsuNo[32];            /* �����ɼ������ȣ      */
	char zBnsTp[1];                /* �Ÿű���              */
	char dOrdPrc[13];              /* �ֹ���                */
	char zFnoOrdprcPtnCode[2];     /* �����ɼ�ȣ�������ڵ�  */
};
#define L_tr3231_mid		sizeof(struct tr3231_mid)
struct tr3231_mod
{
	struct tr3231_mid mid;
	char Out[5];                   /* ���ڵ尹��            */
	char zAcntNm[40];              /* ���¸�                */
	char zQryDt[8];                /* ��ȸ��                */
	char dCurPrc[13];              /* ���簡                */
	char lOrdAbleQty[16];          /* �ֹ����ɼ���          */
	char lNewOrdAbleQty[16];       /* �ű��ֹ����ɼ���      */
	char lLqdtOrdAbleQty[16];      /* û���ֹ����ɼ���      */
	char lUsePreargMgn[16];        /* ��뿹�����űݾ�      */
	char lUsePreargMnyMgn[16];     /* ��뿹���������űݾ�  */
	char lOrdAbleAmt[16];          /* �ֹ����ɱݾ�          */
	char lMnyOrdAbleAmt[16];       /* �����ֹ����ɱݾ�      */
};
#define L_tr3231_mod		sizeof(struct tr3231_mod)

/******************************************************************************
* NAME : tr3232
* DESC : �����ɼ� ���ϼ��� ��ȸ
 ******************************************************************************/
struct tr3232_mid
{
	char zIn[5];		/* ���ڵ尹�� */
	char zAcntNo[20];	/* ���¹�ȣ */
	char zPwd[8];		/* ��й�ȣ */
	char zSrtDt[8];		/* ������ */
	char zEndDt[8];		/* ������ */
	char zFtsubtTp[1];	/* ������뱸�� */
	char zErnratTp[1];	/* ���ͷ����� */
};
#define L_tr3232_mid		sizeof(struct tr3232_mid)
struct tr3232_mod
{
	struct tr3232_mid mid;
	char zOut[5];					/* ���ڵ尹�� */
	char zAcntNm[40];				/* ���¸� */
	char zInvstPramtAvrbalAmt[16];	/* ���ڿ������ܱݾ� */
	char zPnlAmt[16];				/* ���ͱݾ� */
	char zErnRat[12];				/* ���ͷ� */
	char zRat01[19];				/* ������ */
	char zFdDpsastAmt[16];			/* ���ʿ�Ź�ڻ�ݾ� */
	char zEotDpsastAmt[16];			/* �⸻��Ź�ڻ�ݾ� */
	char zInAmt[16];				/* �Աݾ� */
	char zOutAmt[16];				/* ��ݾ� */
	char zBnsctrAmt[16];			/* �Ÿž����ݾ� */
	char zFutsAdjstDfamt[16];		/* ������������ */
	char zBnsCmsnAmt[16];			/* �Ÿż����� */
	char zOptBnsplAmt[16];			/* �ɼǸŸż��ͱݾ� */
	char zDpsamtUtlfee[16];			/* ��Ź���̿�� */
	char zOptEvalPnl[16];			/* �ɼ��򰡼��� */
	char zAcntOpnDt[8];				/* ���°����� */
	char zAcntCloseDt[8];			/* ��������� */
};
#define L_tr3232_mod		sizeof(struct tr3232_mod)

/******************************************************************************
* NAME : tr3411
* DESC : �ɼ� ������ �ü���ȸ
* DATE : 2013.04.01 
 ******************************************************************************/
struct  tr3411_grid {
    char    c_rtcd[8];  /* ��RTS CODE           */
    char    c_mdif[8];  /* ������(����)         */
    char    c_mgjv[8];  /* �ݹ̰�������         */
    char    c_gvol[8];  /* �ݰŷ���             */
    char    c_msga[8];  /* �ݸż�               */
    char    c_mdga[8];  /* �ݸŵ�               */
    char    c_rate[8];  /* �ݵ����             */
    char    c_diff[7];  /* �ݴ��               */
    char    c_curr[8];  /* �����簡             */
    char    hang[8];    /* ��簡               */
    char    jihs[8];    /* ����ȯ��             */
    char    p_rtcd[8];  /* ǲRTS CODE           */
    char    p_curr[8];  /* ǲ���簡             */
    char    p_diff[7];  /* ǲ���               */
    char    p_rate[8];  /* ǲ�����             */
    char    p_mdga[8];  /* ǲ�ŵ�               */
    char    p_msga[8];  /* ǲ�ż�               */
    char    p_gvol[8];  /* ǲ�ŷ���             */
    char    p_mgjv[8];  /* ǲ�̰�������         */
    char    p_mdif[8];  /* ǲ����(����)         */
};

struct tr3411_mid{
	char ysgb;	//���� ���� default 0
	char func;	// start "F"
	char mymd[6];	// ����� ex. 201304
};

#define	MAX_R		30

struct  tr3411_mod {
    char    atmg[8];    /* ATM  */
    char    nrec[4];
    struct  tr3411_grid    grid[MAX_R];
};


/******************************************************************************
* NAME : tr1801
* DESC : �������� �ü���ȸ
 ******************************************************************************/
struct tr1801_grid
{
    char code[6];	/* RTS Symbol  */
    char hnam[20];  /* �ѱ۸�      */
    char curr[7];   /* ���簡      */
	char giho[1];   /* �����ȣ    1.���Ѱ�, 2.���, 3.����, 4.���Ѱ�, 5.�϶�*/
    char diff[7];   /* ���ϴ��    */
    char mdga[7];	/* �ŵ�ȣ��    */
    char msga[7];   /* �ɼ�ȣ��    */
    char gvol[12];  /* �ŷ���      */
};
#define L_tr1801_grid		sizeof(struct tr1801_grid)
struct tr1801_mod
{
    char nrec[4];
    struct tr1801_grid grid[1];
};
#define L_tr1801_mod		sizeof(struct tr1801_mod)

/******************************************************************************
* NAME : tr1802
* DESC : �������� �ü���ȸ
 ******************************************************************************/
struct tr1802_grid
{
    char code[8];	/* RTS Symbol  */
    char seqn[1];	/* ����SEQN    */
	char giho[1];   /* �����ȣ    1.���Ѱ�, 2.���, 3.����, 4.���Ѱ�, 5.�϶�*/
    char diff[6];	/* ���ϴ��    */
    char curr[6];	/* ���簡      */
    char mdga[6];	/* �ŵ�ȣ��    */
    char msga[6];	/* �ż�ȣ��    */
    char gvol[12];	/* �ŷ���      */
    char mgjv[12];  /* �̰����    */
};
#define L_tr1802_grid		sizeof(struct tr1802_grid)
struct tr1802_mod
{
    char nrec[4];
    struct tr1802_grid grid[1];
};
#define L_tr1802_mod		sizeof(struct tr1802_mod)

/******************************************************************************
* NAME : tr1803
* DESC : �������� �����ȸ
 ******************************************************************************/
struct tr1803_grid
{
    char jggb[1];   /* �屸�� */
    char time[6];   /* �ð�   */
    char ttjs[5];   /* ��ü ����� */
    char trjs[5];   /* �ŷ� ����� */
    char ssjs[5];   /* ��� ����� */
    char shjs[5];   /* ���� ����� */
    char hrjs[5];   /* �϶� ����� */
    char hhjs[5];   /* ���� ����� */
    char bhjs[5];   /* ���� ����� */
    char gsjs[5];   /* �⼼ ���� ����� */
    char gsss[5];   /* �⼼ ��� ����� */
    char ghjs[5];   /* �⼼ �϶� ����� */
    char gvol[10];  /* �ŷ��� (õ��) */
    char gamt[10];  /* �ŷ���� (�鸸) */
};
#define L_tr1803_grid		sizeof(struct tr1803_grid)
struct tr1803_mod
{
    char nrec[4];
    struct tr1803_grid grid[1];
};
#define L_tr1803_mod		sizeof(struct tr1803_mod)

/******************************************************************************
* NAME : tr1804
* DESC : �������� ������ȸ
 ******************************************************************************/
struct tr1804_grid
{
    char symb[16];		/* RTS Symbol */
    char jggb[1];		/* �屸��     */
    char time[6];		/* �ð�       */
    char curr[7];       /* ���簡     */
	char giho[1];		/* �����ȣ    '+', '-'�� ǥ�õ�*/
    char diff[7];       /* ���ϴ��   */
    char gvol[12];      /* �ŷ���     */
    char gamt[12];      /* �ŷ����   */
};
#define L_tr1804_grid		sizeof(struct tr1804_grid)
struct tr1804_mod
{
    char nrec[4];
    struct tr1804_grid grid[1];
};
#define L_tr1804_mod		sizeof(struct tr1804_mod)

/******************************************************************************
* NAME : tr2001
* DESC : �����ں� �Ÿŵ���
 ******************************************************************************/
struct tr2001_grid
{
    char code[10];	/* RTS Symbol  */
    char dvol[12];	/* �ŵ����� 333 */
	char svol[12];  /* �ż����� 334 */
    char rvol[12];	/* ���ż����� 343 */
    char damt[12];	/* �ŵ��ݾ� 339 */
    char samt[12];	/* �ż��ݾ� 340 */
    char ramt[12];	/* ���ż��ݾ� 344 */
};
#define L_tr2001_grid		sizeof(struct tr2001_grid)
struct tr2001_mod
{
    struct tr2001_grid grid[12];	//�ܱ���, ����, �����, ����, ��������, ����, ����, ��Ÿ����, �����, ���, ����, ��Ÿ����
};
#define L_tr2001_mod		sizeof(struct tr2001_mod)


/******************************************************************************
* NAME : GetCode("hjcode")
* DESC : �������񸶽��� [ hjcode * n ]
 ******************************************************************************/
struct hjcode {
	char	codx[12];		/* �ڵ�(7) : A######			*/
							/* �ڵ�(9) : �����μ���, ����	*/
	char	hnam[40];		/* �ѱ۸�						*/
	char	enam[40];		/* ������						*/
	char	ecnf;			/* ECN (1:�ŷ�����)				*/
	char	jsiz;			/* �����ں��� ���߼�			*/
							/* �峻(��: 38, ��: 39, ��: 40)	*/  
							/* ���(1: KOSDAQ 100)		 	*/
							/* ���(2: KOSDAQ MID 300)		*/
							/* ���(3: KOSDAQ SMALL)		*/
							/* ############################ */
							/* ������� : (jsiz += 41)		*/
							/* �����μ������� FULL�ڵ� ��	*/
	char	symb[7];		/* SYMBOL						*/
	char	size;    		/* �ð��ѾױԸ�(2, 3, 4)		*/
	char	ucdm;			/* �����ߺз�       99:�̺з�	*/
	char	ucds;			/* �����Һз�       99:�̺з�	*/
							/* KOSDAQ 7:����				*/
	char	jjug;			/* ������ ����(27: ������)		*/
							/* KOSDAQ ���� ����(=2)			*/
	char	kpgb;			/* KOSPI200/KOSDAQ ����			*/
							/* 0:������ 1:K200 2:K100 3:K50 */
							/* 0:������ 1:KQ50				*/
	char	kosd;			/* ���񱸺�						*/
							/* 0:�峻  10:���  30:��3����	*/
	char	ssgb;			/* �Ҽӱ���						*/
							/* 0:�Ϲ�						*/
							/* 3:�ܱ��ο�Ź����				*/
							/* 4:���߾��ݵ�					*/
							/* 5:�����μ���(�߱�)			*/
							/* 6:����						*/
							/* 7:�����μ���(����)			*/
							/* 8:ETF						*/
							/* 9:�����ϵ��ݵ�(��������)		*/
	char	ucmd;			/* �켱�ֱ��� 0:������ 5:�켱��	*/
	char	itgb;			/* KOSPI-IT/KQ-IT50����(1:ä��)	*/
	char	wsgb;			/* 0:�Ϲ� 1:���豸��������	*/
	char	jqty;			/* �⺻�ֹ�����					*/
	char	star;			/* Star ���񱸺�				*/
	char	unio;			/* ������������ ����			*/
							/* 0: �Ϲ�, 1:��������			*/
	char	jchk;			/* ����˻��� ����				*/
							/* 0x01     �Ҽ��ǰ�������   	*/
							/* 0x02     �űԻ�������     	*/
							/* 0x04     �Ǹ�������       	*/
							/* 0x08     �켱������       	*/
							/* 0x10     ��������         	*/
							/* 0x20     ��������         	*/
							/* 0x40     �ŷ���������     	*/
							/* 0x80     ������������     	*/
	char	elwf;			/* 0: �Ϲ�, 1: �����ڻ� 		*/
	char	sjjs[10];		/* �����ֽļ�(����:��)			*/
	char	fill[10];		/* reserved						*/
};
#define L_hjcode		sizeof(struct hjcode)

/******************************************************************************
* NAME : GetCode("fjcode")
* DESC : �������񸶽��� [ fjcode * n ]
 ******************************************************************************/
struct fjcode {			/* ���������ڵ� ����			*/
	char	codx[8];		/* �����ڵ� (8) 				*/
	char	hnam[20];		/* �ѱ۸�						*/
	char	enam[20];		/* ������						*/
	char	mchk;			/* �Ÿ��������					*/
							/* 0x01:���尡���				*/
							/* 0x02:���Ǻ����������		*/
							/* 0x04:���������������		*/
};
#define L_fjcode		sizeof(struct fjcode)

/******************************************************************************
* NAME : GetCode("opcode")
* DESC : �ɼ����񸶽��� [opcode_h + (opcode * n)]
 ******************************************************************************/
struct opcode_h {			/* �ɼ������ڵ� HEADER ����		*/
	char	chdr[11][6];		/* CALL OPTION ���� HEADER		*/
	char	phdr[11][6];		/* PUT OPTION ���� HEADER		*/
};

struct opcode {			/* �ɼ������ڵ� ����			*/
	char	hsga[5];		/* ��簡��(999V99)				*/
	char	atmg;			/* ATM����(1:ATM, 2:ITM, 3:OTM)	*/
	struct	{
		char	yorn[1];	/* ���� �ڵ� ����				*/
		char	codx[9];	/* ���������ڵ�					*/
		char	hnam[20];	/* �ѱ������					*/
		char	enam[20];	/* ������						*/
		char	mchk;		/* �Ÿ��������					*/
							/* 0x01:���尡���				*/
							/* 0x02:���Ǻ����������		*/
							/* 0x04:���������������		*/
	} call[11]; 
	struct	{
		char	yorn[1];	/* ���� �ڵ� ����				*/
		char	codx[9];	/* ���������ڵ�					*/
		char	hnam[20];	/* �ѱ������					*/
		char	enam[20];	/* ������						*/
		char	mchk;		/* �Ÿ��������					*/
							/* 0x01:���尡���				*/
							/* 0x02:���Ǻ����������		*/
							/* 0x04:���������������		*/
	} put[11]; 
};
#define L_opcode		sizeof(struct opcode)

#define	N_LP	(5)
struct elwcode {			/* ELW�����ڵ�	����				*/
	char	codx[12];		/* �ڵ�(6) : J######				*/
	char	hnam[32];		/* �ѱ۸�							*/
	char	enam[20];		/* ������							*/
	char	symb[7];		/* SYMBOL							*/
	char	lphn[N_LP][3];	/* ELW - LP ȸ����ȣ 1~5			*/
	char	lpse[5];		/* ELW ��������	   (999V99)			*/	
	char 	hbge[20];		/* ELW �ѱ۹�������				*/
	char	ebge[20]; 		/* ELW ������������				*/
	char	bgce[12];		/* ELW �������ڵ�					*/
	char	gcjs[N_LP][12];	/* ELW �����ڻ� 1~5					*/	
							/* KOSPI100:ǥ���ڵ�,KOSPI200:"KOSPI200    " */
	char	grat[N_LP][5];	/* ELW �����ڻ걸������ 1~5 (999V99)*/
	char	gcge[10];		/* ELW ���ʰ���	     (99999999V99) 	*/
	char	hsge[10];		/* ELW ��簡��	     (99999999V99) 	*/	
	char	krye[2];		/* ELW �Ǹ�����						*/
							/* 01:CALL,02:PUT,03:��Ÿ  		 	*/
	char 	khbe[2];		/* ELW �Ǹ������					*/
							/* 01:������;02:�̱���;03:��Ÿ	 	*/
	char	edte[8];		/* �����ŷ���   YYYYMMDD			*/
	char	fill[40];		/* reserved							*/
};
#define L_elwcode		sizeof(struct elwcode)

struct Jango{
	char flag;		// for Window Message
	char acno[12];	// ���¹�ȣ
	char code[12];	// �����ڵ�
	char name[40];	// �����
	char gubn[10];	// ����
	int  jqty;		// ��������
	int  xqty;		// û�갡�ɼ���
	int  pprc;		// ��հ�
	double camt;	// �򰡱ݾ�
	double tamt;	// �򰡼���
	double srat;	// ���ͷ�
	double mamt;	// ���Աݾ�
};


#pragma	pack()
