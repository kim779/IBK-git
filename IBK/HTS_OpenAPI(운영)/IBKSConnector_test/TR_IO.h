#pragma once

#include "TR_IN.h"
#include "PTS_API_OUT.h"

struct SONBQ504_mid
{
	char In[5];                        /* ���ڵ尹��             */
	char zAcntNo[20];                  /* ���¹�ȣ               */
	char zInptPwd[8];                  /* �Էº�й�ȣ           */
	char zOrdDt[8];                    /* �ֹ���                 */
	char zBalEvalTp[1];                /* �ܰ��򰡱���           */
	char zFutsPrcEvalTp[1];            /* ���������򰡱���       */
	char zLqdtQtyQryTp[1];             /* û�������ȸ����       */
};
#define L_SONBQ504_mid		sizeof(struct SONBQ504_mid)

struct SONBQ504_grid
{
	char zFnoIsuNo[32];                /* �����ɼ������ȣ       */
	char zIsuNm[40];                   /* �����                 */
	char zBnsTp[1];                    /* �Ÿű���               */
	char zBnsTpNm[10];                 /* �Ÿű���               */
	char lUnsttQty[16];                /* �̰�������             */
	char dFnoAvrPrc[19];               /* ��հ�                 */
	char dNowPrc[13];                  /* ���簡                 */
	char dCmpPrc[13];                  /* ���                 */
	char lEvalPnl[16];                 /* �򰡼���               */
	char dPnlRat[12];                  /* ���ͷ�                 */
	char lEvalAmt[16];                 /* �򰡱ݾ�               */
	char dEvalRat[7];                  /* �򰡺���               */
	char lLqdtAbleQty[16];             /* û�갡�ɼ���           */
};
#define L_SONBQ504_grid		sizeof(struct SONBQ504_grid)

struct SONBQ504_mod
{
	char Out[5];                       /* ���ڵ尹��             */
	char zAcntNm[40];                  /* ���¸�                 */
	char lEvalDpsamtTotamt[15];        /* �򰡿�Ź���Ѿ�         */
	char lMnyEvalDpstgAmt[15];         /* �����򰡿�Ź�ݾ�       */
	char lDpsamtTotamt[16];            /* ��Ź���Ѿ�             */
	char lDpstgMny[16];                /* ��Ź����               */
	char lDpstgSubst[16];              /* ��Ź���               */
	char lFcurrSubstAmt[16];           /* ��ȭ���ݾ�           */
	char lPsnOutAbleTotAmt[15];        /* ���Ⱑ���ѱݾ�         */
	char lPsnOutAbleCurAmt[16];        /* ���Ⱑ�����ݾ�         */
	char lPsnOutAbleSubstAmt[16];      /* ���Ⱑ�ɴ��ݾ�       */
	char lOrdAbleTotAmt[15];           /* �ֹ������ѱݾ�         */
	char lMnyOrdAbleAmt[16];           /* �����ֹ����ɱݾ�       */
	char lCsgnMgnTotamt[16];           /* ��Ź���ű��Ѿ�         */
	char lMnyCsgnMgn[16];              /* ������Ź���űݾ�       */
	char lMtmgnTotamt[15];             /* �������ű��Ѿ�         */
	char lMnyMaintMgn[16];             /* �����������űݾ�       */
	char lAddMgnTotamt[15];            /* �߰����ű��Ѿ�         */
	char lMnyAddMgn[16];               /* �����߰����űݾ�       */
	char lCmsnAmt[16];                 /* ������                 */
	char lRcvblAmt[16];                /* �̼��ݾ�               */
	char lRcvblOdpnt[16];              /* �̼���ü��             */
	char lFutsEvalPnlAmt[16];          /* �����򰡼��ͱݾ�       */
	char lOptEvalPnlAmt[16];           /* �ɼ��򰡼��ͱݾ�       */
	char lOptEvalAmt[16];              /* �ɼ��򰡱ݾ�           */
	char lOptBnsplAmt[16];             /* �ɼǸŸż��ͱݾ�       */
	char lFutsAdjstDfamt[16];          /* ������������           */
	char lTotPnlAmt[16];               /* �Ѽ��ͱݾ�             */
	char lNetPnlAmt[16];               /* �����ͱݾ�             */
	char Out2[5];                      /* ���ڵ尹��             */
	struct SONBQ504_grid grid[1];
};
#define L_SONBQ504_mod		sizeof(struct SONBQ504_mod)

//////////////////////////////////////////////////////////////////////////
//������ü ����
/******************************************************************************
* NAME : TR4501
* DESC : ������ü���(����->����)
 ******************************************************************************/
struct TR4501_mid
{
	char In[5];					/* ���ڵ� ����      */
	char zCardFirmNo[3];		/* ī��ȸ���ȣ*/
	char zCardAcntNo[20];		/* ī����¹�ȣ*/
	char zCardPtnCode[2];		/* ī�������ڵ�*/
	char zCardIssCnt[10];		/* ī��߱ްǼ� Default +000000000 */
	char zMgrCardFirmNo[3];		/* å����ī��ȸ���ȣ*/
	char zMgrCardAcntNo[20];	/* å����ī����¹�ȣ*/
	char zMgrCardPtnCode[2];	/* å����ī�������ڵ�*/
	char zMgrCardIssCnt[10];	/* å����ī��߱�Ƚ�� Default +000000000 */
	char zAgrmReqstDt[8];		/* ���ο�û��*/
	char zAgrmTrxId[8];			/* ����ó��ID*/
	char zLastAgrmUserId[16];	/* ����������ID*/
	char zAgrmUserNm[40];		/* ���λ���ڸ�*/
	char zAgrmAddMsg[100];		/* �������߰��޽���*/
	char zRlyBnkCode[3];		/* �߰������ڵ�*/
	char zMnyoutAcntNo[20];		/* ��ݰ��¹�ȣ*/
	char zPwd[8];				/* ��й�ȣ*/
	char zMnyoutAmt[16];		/* ��ݱݾ�*/
	char zMnyinBnkCode[3];		/* �Ա������ڵ�*/
	char zBnkAcntNo[20];		/* ������¹�ȣ*/
	char zRcvrNm[40];			/* �����θ�*/
	char zBnkTrsfPwd[8];		/* ������ü��й�ȣ*/
	char zMnyoutPtnTp[1];		/* �������	Default 1 */
	char zIvstfndCode[12];		/* �����ݵ��ڵ�*/
	char zAskpsnNm[40];			/* �Ƿ��θ�*/
	char zTelExno[4];			/* ��ȭ����ȣ*/
	char zTelRgno[4];			/* ��ȭ������ȣ*/
	char zTelSeqno[4];			/* ��ȭ�Ϸù�ȣ*/
};
#define L_TR4501_mid		sizeof(struct TR4501_mid)

struct TR4501_mod
{
	struct TR4501_mid mid;
	char Out[5];				/* ���ڵ尹�� */
	char zAcntNm[40];			/* ���¸�		*/
	char zMtrsfTrdNo[10];		/* ��ü�ŷ���ȣ*/
	char zRlyBnkNm[40];			/* �߰������*/
	char zMnyinBnkNm[40];		/* �Ա������*/
	char zRcvrNm[40];			/* �����θ�*/
	char zOrdNo[10];			/* �ֹ���ȣ*/
	char zBncertSellUnit[16];	/* �������Ǹŵ��¼�*/
	char zInouno[10];			/* �ⳳ��ȣ*/
	char zMnyoutAmt[16];		/* ��ݱݾ�*/
	char zOutTrsfCmsnAmt[16];	/* ��ü��ݼ�����*/
	char zMnyoutTotAmt[16];		/* ����ѱݾ�*/
	char zDpsBfbalAmt[16];		/* ���������ܱݾ�*/
	char zDpsCrbalAmt[16];		/* �����ݱ��ܱݾ�*/
	char zFundNm[40];			/* �ݵ��*/
	char zBnkRspnsCode[4];		/* ���������ڵ�*/
	char zBnkMsgCnts[100];		/* ����޽�������*/
	char zAgrmTrxStep[10];		/* ����ó���ܰ�*/
	char zAgrmMthdCode[2];		/* ���ι���ڵ�*/
};
#define L_TR4501_mod		sizeof(struct TR4501_mod)


/******************************************************************************
* NAME : TR4502
* DESC : ���°��ڱݴ�ü(���� -> ����)
 ******************************************************************************/
struct TR4502_mid
{
	char In[5];					/* ���ڵ� ����      */
	char zCardFirmNo[3];		/* ī��ȸ���ȣ */
	char zCardAcntNo[20];		/* ī����¹�ȣ */
	char zCardPtnCode[2];		/* ī�������ڵ� */
	char lCardIssCnt[10];		/* ī��߱ްǼ� Default +000000000 */
	char zMgrCardFirmNo[3];		/* å����ī��ȸ���ȣ */
	char zMgrCardAcntNo[20];	/* å����ī����¹�ȣ */
	char zMgrCardPtnCode[2];	/* å����ī�������ڵ� */
	char lMgrCardIssCnt[10];	/* å����ī��߱�Ƚ�� Default +000000000 */
	char zAcntNo[20];			/* ���¹�ȣ */
	char zPwd[8];				/* ��й�ȣ */
	char lBkeepAmt[16];			/* ��ü�ݾ� */
	char zBncertSellIsuNo[6];	/* �ŵ������ȣ */
	char zBncertSellIsuSeqno[6];/* �ŵ������Ϸù�ȣ */
	char zMnyoutPtnTp[1];		/* ������� Default 4 */
	char zMnyinAcntNo[20];		/* �Աݰ��¹�ȣ */
	char zBncertBuyIsuNo[6];	/* �ż������ȣ */
	char zBncertBuyIsuSeqno[6];	/* �ż������Ϸù�ȣ */
	char zAgrmBranNo[3];		/* ����������ȣ */
	char zAgrmTermNo[3];		/* ���δܸ���ȣ */
	char zLastAgrmUserId[16];	/* ����������ID */
	char zAgrmReqstYn[1];		/* ���ο�û���� */
	char zEmpPwd[8];			/* �����й�ȣ */
	char zInptPwd[8];			/* �Էº�й�ȣ */
	char zSmryNo[4];			/* �����ȣ */
};
#define L_TR4502_mid		sizeof(struct TR4502_mid)

struct TR4502_mod
{
	struct TR4502_mid mid;
	char Out[5];						/* ���ڵ尹�� */
	char lMnyoutInouno[8];				/* ����ⳳ��ȣ */
	char lBkeepAmt[16];					/* ��ü�ݾ� */
	char lMnyoutDpsBfbalAmt[16];		/* ��ݿ��������� */
	char lMnyoutDpsCrbalAmt[16];		/* ��ݿ����ݱ��� */
	char lMnyoutNSavBfbalAmt[16];		/* ��ݺ��������� */
	char lMnyoutNSavCrbalAmt[16];		/* ��ݺ�������� */
	char lMnyinInouno[8];				/* �Ա��ⳳ��ȣ */
	char lMnyinDpsBfbalAmt[16];			/* �Աݿ��������� */
	char lMnyinDpsCrbalAmt[16];			/* �Աݿ����ݱ��� */
	char lMnyinNSavBfbalAmt[16];		/* �Աݺ��������� */
	char lMnyinNSavCrbalAmt[16];		/* �Աݺ�������� */
	char lMnyRcvblRepayAmt[16];			/* ���ݹ̼������ݾ� */
	char lMnyOdpntRepayAmt[16];			/* ���ݹ̼���ü�ắ���ݾ� */
	char lIntdltRepayAmt[16];			/* ���ڹ̳������ݾ� */
	char lIntdltOdpntRepayAmt[16];		/* ���ڹ̳���ü�ắ���ݾ� */
	char lEtclndRepayAmt[16];			/* ��Ÿ�뿩�����ݾ� */
	char lEtclndDlinqOdpntRepayAmt[16];	/* ��Ÿ�뿩��ü�ắ���ݾ� */
	char lDpsCrdaySubRepayAmt[16];		/* �������Ϻκ����ݾ� */
	char lRealInAmt[16];				/* ���Աݾ� */
	char lRealOutAmt[16];				/* ����ݾ� */
	char zEmpPwd[8];					/* �����й�ȣ */
	char zMnyoutAcntNm[40];				/* ��ݰ��¸� */
	char zSellIsuNm[40];				/* �ŵ������ */
	char zMnyinAcntNm[40];				/* �Աݰ��¸� */
	char zBuyIsuNm[40];					/* �ż������ */
	char zSmryNo[4];					/* �����ȣ */
	char zSmryNm[40];					/* ����� */
	char lBkeepInouno[10];				/* ��ü�ⳳ��ȣ */
	char lBrnInouno[10];				/* �����ⳳ��ȣ */
	char lReqstInouno[10];				/* ��û�ⳳ��ȣ */
	char Out2[5];						/* ���ڵ尹�� */
	char lOrdNo[10];					/* �ֹ���ȣ */
	char lBuyQty[16];					/* �ż����� */
	char dFundBfbalQty[20];				/* �ݵ����ܼ��� */
	char dBasePrc[13];					/* ���ذ� */
	char zSettDt[8];					/* ������ */
	char lBuyAmt[16];					/* �ż��ݾ� */
	char dFundCrbalQty[20];				/* �ݵ���ܼ��� */
	char dBoaBasePrc[20];				/* ��ǥ���ذ� */
	char lMchndCmsnAmt[16];				/* �Ǹż����� */
	char zHanglIvstfndNm[40];			/* �ѱ������ݵ�� */
	char lIvstfndSeqno[10];				/* �����ݵ��Ϸù�ȣ */
	char zBuyPtnCode[2];				/* �ż������ڵ� */
	char zBuyPtnNm[40];					/* �ż����� */
};
#define L_TR4502_mod		sizeof(struct TR4502_mod)


/******************************************************************************
* NAME : TR4503
* DESC : ������ü���(����->����)
 ******************************************************************************/
struct TR4503_mid
{
	char In[5];					/* ���ڵ� ����      */
	char zMnyinAcntNo[20];		/* �Աݰ��¹�ȣ */
	char zBnkCode[3];			/* �����ڵ� */
	char zBnkAcntNo[20];		/* ������¹�ȣ */
	char zPwd[8];				/* ��й�ȣ */
	char zMnyinAmt[16];			/* �Աݱݾ� */
};
#define L_TR4503_mid		sizeof(struct TR4503_mid)

struct TR4503_mod
{
	struct TR4503_mid mid;
	char Out[5];					/* ���ڵ尹�� */
	char zSecAcntNm[40];			/* ���ǰ��¸� */
	char zBnkAcntNm[40];			/* ������¸� */
	char zMtrsfTrdNo[10];			/* ��ü�ŷ���ȣ */
	char zMnyInouno[10];			/* �����ⳳ��ȣ */
	char zDpsBfbalAmt[16];			/* ���������ܱݾ� */
	char zDpsCrbalAmt[16];			/* �����ݱ��ܱݾ� */
	char zMnyrclRepayAmt[16];		/* ���ݹ̼������ݾ� */
	char zMnyrclOdpntRepayAmt[16];	/* ���ݹ̼���ü�ắ���ݾ� */
	char zIntdltRepayAmt[16];		/* ���ڹ̳������ݾ� */
	char zIntdltOdpntRepayAmt[16];	/* ���ڹ̳���ü�ắ���ݾ� */
	char zEtclndRepayAmt[16];		/* ��Ÿ�뿩�����ݾ� */
	char zEtclndOdpntRepayAmt[16];	/* ��Ÿ�뿩�ݿ�ü�ắ���ݾ� */
	char zDpsCrdaySubRepayAmt[16];	/* �������Ϻκ����ݾ� */
	char zBnkDps[15];				/* ���࿹���� */
	char zBnkNm[40];				/* ����� */
	char zBnkRspnsCode[4];			/* ���������ڵ� */
	char zBnkMsgCnts[100];			/* ����޽������� */
};
#define L_TR4503_mod		sizeof(struct TR4503_mod)


/******************************************************************************
* NAME : TR4511
* DESC : ���º�������ü�ŷ�����
 ******************************************************************************/
struct TR4511_mid
{
	char In[5];				/* ���ڵ� ����      */
	char zAcntNo[20];		/* ���¹�ȣ*/
	char zPwd[8];			/* ��й�ȣ*/
	char zQrySrtDt[8];		/* ��ȸ������*/
	char zQryEndDt[8];		/* ��ȸ������*/
};
#define L_TR4511_mid		sizeof(struct TR4511_mid)

struct TR4511_grid
{
	char zTrdDt[8];			/* �ŷ��� */
	char zTrxTime[9];		/* ó���ð� */
	char zMtrsfTrdNo[10];	/* ��ü�ŷ���ȣ */
	char zTrdTpNm[20];		/* �ŷ����� */
	char zTrxBrnNm[40];		/* ó�������� */
	char zCancYnNm[10];		/* ��ҿ��θ� */
	char zTrdBnkNm[40];		/* �ŷ������ */
	char zRlyBnkNm[40];		/* �߰������ */
	char zBnkAcntNo[20];	/* ������¹�ȣ */
	char zBnkAcntNm[40];	/* ������¸� */
	char zTrdAmt[16];		/* �ŷ��ݾ� */
	char zCmsnAmt[16];		/* ������ */
	char zMnyAmt[16];		/* ���ݱݾ� */
	char zChckAmt[16];		/* ��ǥ�ݾ� */
	char zColorTp[1];		/* ������ */
	char zTrxStatNm[20];	/* ó�����¸� */
	char zUnTrxRsn[20];		/* ��ó������ */
	char zCommdaCodeNm[40];	/* ��Ÿ�ü�ڵ�� */
	char zIntaccNo[20];		/* ���հ��¹�ȣ */
	char zTelRgno[4];		/* ��ȭ������ȣ */
	char zTelExno[4];		/* ��ȭ����ȣ */
	char zTelSeqno[4];		/* ��ȭ�Ϸù�ȣ */
	char zInouno[10];		/* �ⳳ��ȣ */
	char zOrgInouno[10];	/* ���ⳳ��ȣ */
	char zUablCode[4];		/* �Ҵ��ڵ� */
	char zOcrPtnTp[1];		/* �߻��������� */
	char zTrxtrNm[40];		/* ó���ڸ� */
	char zChckAbndSettTpNm[8];	/* ó������ */
};
#define L_TR4511_grid		sizeof(struct TR4511_grid)

struct TR4511_mod
{
	struct TR4511_mid mid;
	char Out[5];				/* ���ڵ尹�� */
	char zAcntNm[40];			/* ���¸�		*/
	char Out2[5];				/* ���ڵ尹�� */
	struct TR4511_grid grid[1];
};
#define L_TR4511_mod		sizeof(struct TR4511_mod)


/******************************************************************************
* NAME : TR4512
* DESC : ���´�ü�ŷ�����
 ******************************************************************************/
struct TR4512_mid
{
	char In[5];				/* ���ڵ� ����      */
	char zBrnNo[3];			/* ������ȣ Default 000 */
	char zQrySrtDt[8];		/* ��ȸ������ */
	char zQryEndDt[8];		/* ��ȸ������ */
	char zSrtTrdNo[6];		/* ���۰ŷ���ȣ Default +00000 */
	char zBkeepTp[1];		/* ��ü���� Default 1 */
	char zAcntNo[20];		/* ���¹�ȣ */
	char zPwd[8];			/* ��й�ȣ */
	char zQryCnt[10];		/* ��ȸ�Ǽ� Default +000000020 */
};
#define L_TR4512_mid		sizeof(struct TR4512_mid)

struct TR4512_grid
{
	char zBkeepNo[10];			/* ��ü��ȣ */
	char zInouno[10];			/* �ⳳ��ȣ */
	char zBkeepTpNm[20];		/* ��ü���� */
	char zCancTpNm[20];			/* ��ұ��� */
	char zMnyoutAcntNo[20];		/* ��ݰ��¹�ȣ */
	char zMnyoutAcntNm[40];		/* ��ݰ��¸� */
	char zSecoutIsuNo1[12];		/* ��������ȣ1 */
	char zSeqno1[10];			/* �Ϸù�ȣ */
	char zSecoutIsuNm1[40];		/* ��������1 */
	char zMnyinAcntNo[20];		/* �Աݰ��¹�ȣ */
	char zMnyinAcntNm[40];		/* �Աݰ��¸� */
	char zSecinIsuNo1[12];		/* �԰������ȣ1 */
	char zSeqno2[10];			/* �Ϸù�ȣ */
	char zSecinIsuNm1[40];		/* �԰������1 */
	char zTrdAmt[16];			/* �ŷ��ݾ� */
	char zMnyoutSeqno[10];		/* ����Ϸù�ȣ */
	char zMnyinSeqno[10];		/* �Ա��Ϸù�ȣ */
	char zIsuNo[12];			/* �����ȣ */
	char zIsuNm[40];			/* ����� */
	char zTrxTermNo[3];			/* ó���ܸ���ȣ */
	char zTrxTime[9];			/* ó���ð� */
	char zBuyDate[8];			/* �ż����� */
	char zSecPtnCode[4];		/* �������������ڵ� */
	char zTrdDate[8];			/* �ŷ����� */
};
#define L_TR4512_grid		sizeof(struct TR4512_grid)

struct TR4512_mod
{
	struct TR4512_mid mid;
	char Out[5];				/* ���ڵ尹�� */
	char zBrnNo[3];				/* ������ȣ */
	char zBrnNm[40];			/* ������ */
	char zQrySrtDt[8];			/* ��ȸ������ */
	char zQryEndDt[8];			/* ��ȸ������ */
	char zSrtTrdNo[6];			/* ���۰ŷ���ȣ */
	char zBkeepTp[1];			/* ��ü���� */
	char Out2[5];				/* ���ڵ尹�� */
	struct TR4512_grid grid[1];
};
#define L_TR4512_mod		sizeof(struct TR4512_mod)


/******************************************************************************
* NAME : TR4521
* DESC : ���ฮ��Ʈ��ȸ
 ******************************************************************************/
struct TR4521_grid
{
	char zBnkNo[3];				/* �����ȣ */
	char zBnkNm[40];			/* ����� */
	char zFltYn[1];				/* ��ֿ��� */
	char zColortp[1];			/* ���󱸺� */
};
#define L_TR4521_grid		sizeof(struct TR4521_grid)

struct TR4521_mod
{
	char Out[5];				/* ���ڵ尹�� */
	struct TR4521_grid grid[1];
};
#define L_TR4521_mod		sizeof(struct TR4521_mod)


/******************************************************************************
* NAME : TR4522
* DESC : ������¹�ȣ��ȸ
 ******************************************************************************/
struct TR4522_mid
{
	char In[5];				/* ���ڵ� ����      */
	char zBankCode[3];		/* �����ڵ�			*/
	char zAcntNo[20];		/* ���¹�ȣ         */
	char zInptPwd[8];		/* �Էº�й�ȣ     */
};
#define L_TR4522_mid		sizeof(struct TR4522_mid)

struct TR4522_grid
{
	char zBankCode[3];			/* �����ڵ� */
	char zBnkNm[40];			/* ����� */
	char zBnkAcntNo[20];		/* ������¹�ȣ */
};
#define L_TR4522_grid		sizeof(struct TR4522_grid)

struct TR4522_mod
{
	struct TR4522_mid mid;
	char Out[5];				/* ���ڵ尹�� */
	struct TR4522_grid grid[1];
};
#define L_TR4522_mod		sizeof(struct TR4522_mod)


/******************************************************************************
* NAME : TR4523
* DESC : HTS�α��ΰ��¸��
 ******************************************************************************/
struct TR4523_mid
{
	char In[5];					/* ���ڵ� ����      */
	char zElecfnUserId[16];		/* ���ڱ��������ID*/
};
#define L_TR4523_mid		sizeof(struct TR4523_mid)

struct TR4523_grid
{
	char zPdptnCode[2];		/* ��ǰ�����ڵ� */
	char zPdptnNm[40];		/* ��ǰ������ */
	char zAcntClssTp[1];	/* ���°������� */
	char zAcntTpNm[20];		/* ���±��� */
	char zAcntStatCode[2];	/* ���»����ڵ� */
	char zAcntStatNm[40];	/* ���»��¸� */
	char zBrnNo[3];			/* ������ȣ */
	char zBrnNm[40];		/* ������ */
	char zAcntNo[20];		/* ���¹�ȣ */
	char zAcntNm[40];		/* ���¸� */
	char zSmpsnYn[1];		/* �����ο��� */
	char zSmpsnYnNm[40];	/* �����ο��� */
	char zDueDt[8];			/* ������ */
};
#define L_TR4523_grid		sizeof(struct TR4523_grid)

struct TR4523_mod
{
	struct TR4523_mid mid;
	char Out[5];			/* ���ڵ尹�� */
	struct TR4523_grid grid[1];
};
#define L_TR4523_mod		sizeof(struct TR4523_mod)


/******************************************************************************
* NAME : TR4524
* DESC : �����⺻������ȸ
 ******************************************************************************/
struct TR4524_mid
{
	char In[5];				/* ���ڵ� ����      */
	char zTrxTp[1];			/* ó������ Default I */
	char zUserId[16];		/* ����� ID */
	char zRmnno[13];		/* �Ǹ�Ȯ�ι�ȣ */
};
#define L_TR4524_mid		sizeof(struct TR4524_mid)

struct TR4524_mod
{
	struct TR4524_mid mid;
	char Out[5];				/* ���ڵ尹�� */
	char zUserId[16];			/* ����� ID */
	char zRmnno[13];			/* �Ǹ�Ȯ�ι�ȣ */
	char zClntTp[1];			/* ������ */
	char zClntNm[40];			/* ���� */
	char zlDigisigErrCnt[2];	/* ���ڼ������Ƚ�� */
	char zCertStatTp[1];		/* ���������±��� */
	char zCertDnm[256];			/* �������ĺ��� */
	char zOnesfCnfTp[1];		/* ����Ȯ�α��� */
};
#define L_TR4524_mod		sizeof(struct TR4524_mod)


/******************************************************************************
* NAME : TR4525
* DESC : ��ݰ��ɱݾ׹׽Ǹ��ȣ��ȸ
 ******************************************************************************/
struct TR4525_mid
{
	char In[5];				/* ���ڵ� ����      */
	char zMnyoutAcntNo[20];	/* ��ݰ��¹�ȣ */
	char zMnyoutPtnTp[1];	/* ������� Default 1 */
	char zIvstfndCode[12];	/* �����ݵ��ڵ� */
};
#define L_TR4525_mid		sizeof(struct TR4525_mid)

struct TR4525_mod
{
	struct TR4525_mid mid;
	char Out[5];				/* ���ڵ尹�� */
	char zRmnno[13];			/* �Ǹ�Ȯ�ι�ȣ */
	char MnyoutAbleAmt[16];		/* ��ݰ��ɱݾ� */
	char zD2Dps[16];			/* D2 ������ */
	char zAcntOnceLmtAmt[16];	/* ������ȸ�ѵ��ݾ� */
	char zAcntDailyLmtAmt[16];	/* ���������ѵ��ݾ� */
	char zLastMtrsfDt[8];		/* ������ü�� */
	char zThdayMtrsfTotamt[16];	/* ������ü�Ѿ� */
	char zCoOnceLmtAmt[16];		/* ȸ����ȸ�ѵ��ݾ� */
	char zCoDailyLmtAmt[16];	/* ȸ�������ѵ��ݾ� */
};
#define L_TR4525_mod		sizeof(struct TR4525_mod)


/******************************************************************************
* NAME : TR4526
* DESC : ���� ������Ȳ
 ******************************************************************************/
struct TR4526_mid
{
	char In[5];				/* ���ڵ� ����      */
	char zAcntTp[1];		/* ���±��� Default 2 */
	char zAcntNo[20];		/* ���¹�ȣ */
	char zInptPwd[8];		/* �Էº�й�ȣ */
	char zSvcKindCode[4];	/* ���������ڵ� Default 1101 */
};
#define L_TR4526_mid		sizeof(struct TR4526_mid)

struct TR4526_grid
{
	char zSvcKindCode[4];		/* ���������ڵ� */
	char zSvcKindNm[40];		/* ���������� */
	char zOppIttCode[3];		/* ������ڵ� */
	char zTrdIttNm[40];			/* �ŷ������ */
	char zOppIttAcntNo[20];		/* ��������¹�ȣ */
	char zOppTrdAcntNm[40];		/* ���ŷ����¸� */
	char zCtrctRegDt[8];		/* ��������� */
	char zCtrctXpirDt[8];		/* ���������� */
	char zIntaccNo[20];			/* ���հ��¹�ȣ */
	char zBnkDlgConnAcntNo[20];	/* �����ǥ������¹�ȣ */
	char zBbkpgPtnNm[40];		/* �ϰ���ü���� */
	char zAbkpDt[8];			/* �ڵ���ü�� */
	char zAmtrBkeepAmt[16];		/* �ڵ���ü��ü�ݾ� */
	char zUndDsgnNm[40];		/* �μ������� */
	char zBuyFundNo[12];		/* �ż��ݵ��ȣ */
	char zSellFundNo[12];		/* �ŵ��ݵ��ȣ */
	char zOtcoNm[40];			/* Ÿ��� */
	char zBkeepTpNm[20];		/* ��ü���� */
	char zOppAcntRrno[13];		/* �������ֹε�Ϲ�ȣ */
	char zFundNm[40];			/* �ݵ�� */
	char zOppFundNm[40];		/* ����ݵ�� */
	char zCtrctSrtDt[8];		/* ���������� */
	char zCtrctAbndDt[8];		/* ���������� */
};
#define L_TR4526_grid		sizeof(struct TR4526_grid)

struct TR4526_mod
{
	struct TR4526_mid mid;
	char Out[5];				/* ���ڵ尹�� */
	char zAcntNm[40];			/* ���¸�		*/
	char Out2[5];				/* ���ڵ尹�� */
	struct TR4526_grid grid[1];
};
#define L_TR4526_mod		sizeof(struct TR4526_mod)


/******************************************************************************
* NAME : TR4527
* DESC : ���¸���ȸ
 ******************************************************************************/
struct TR4527_mid
{
	char In[5];				/* ���ڵ� ����      */
	char zAcntNo[20];		/* ���¹�ȣ */
};
#define L_TR4527_mid		sizeof(struct TR4527_mid)

struct TR4527_mod
{
	struct TR4527_mid mid;
	char Out[5];				/* ���ڵ尹�� */
	char zAcntNm[40];			/* ���¸�		*/
};
#define L_TR4527_mod		sizeof(struct TR4527_mod)

/******************************************************************************
* NAME : TR8011
* DESC : OPT��û SBPGT315
 ******************************************************************************/
struct TR8011_mid {
	char In[5];					/* ���ڵ� ����      */
	char zOnesfCnfTrxSeqno[5];	/* ����Ȯ��ó���Ϸù�ȣ*/
	char zMdaSeqno[16];			/* ��ü�Ϸù�ȣ*/
	char zOtpVrfVal[12];		/* OTP������*/
};
#define L_TR8011_mid		sizeof(struct TR8011_mid)

struct TR8011_mod {
	struct TR8011_mid mid;
	char Out[5];			/* ���ڵ尹�� */
	char zRetryAbleCnt[2];	/* ��õ�����Ƚ��*/
	char zErrMsgCode[4];	/* �����޽����ڵ�*/
	char zErrMsgCnts[100];	/* �����޽�������*/
};
#define L_TR8011_mod		sizeof(struct TR8011_mod)

/******************************************************************************
* NAME : TR8012
* DESC : OPT��û���� SBPGT314
 ******************************************************************************/
struct TR8012_mid {
	char In[5];					/* ���ڵ� ����      */
	char zUserId[16];			/* �����ID*/
	char zReqstSvcId[8];		/* ��û����ID*/
};
#define L_TR8012_mid		sizeof(struct TR8012_mid)

struct TR8012_mod {
	struct TR8012_mid mid;
	char Out[5];				/* ���ڵ尹�� */
	char zOnesfCnfTrxSeqno[5];	/* ����Ȯ��ó����*/
	char zOnesfCnfPtnTp[1];		/* ����Ȯ����������*/
	char zRetryAbleCnt[2];		/* ��õ�����Ƚ��*/
	char zMdaSeqnoReqstYn[1];	/* ��ü�Ϸù�ȣ ��û����*/
};
#define L_TR8012_mod		sizeof(struct TR8012_mod)

/******************************************************************************
* NAME : TR8013
* DESC : ����ī���û SBPGT310
 ******************************************************************************/
struct TR8013_mid {
	char In[5];						/* ���ڵ� ����      */
	char zOnesfCnfTrxSeqno[5];		/* ����Ȯ��ó���Ϸù�ȣ */
	char zSecurCardSeqnoInpt[3];	/* ����ī���Ϸù�ȣ�Է� */
	char zSecurCardInpt1[2];		/* ����ī���Է�1 */
	char zSecurCardInpt2[2];		/* ����ī���Է�2 */
};
#define L_TR8013_mid		sizeof(struct TR8013_mid)

struct TR8013_mod {
	struct TR8013_mid mid;
	char Out[5];			/* ���ڵ尹�� */
	char zRetryAbleCnt[2];	/* ��õ�����Ƚ�� */
	char zErrMsgCode[4];	/* �����޽����ڵ� */
	char zErrMsgCnts[100];	/* �����޽������� */
};
#define L_TR8013_mod		sizeof(struct TR8013_mod)

/******************************************************************************
* NAME : TR8014
* DESC : ����ī���û����
 ******************************************************************************/
struct TR8014_mid {
	char In[5];					/* ���ڵ� ����      */
	char zUserId[16];			/* �����ID */
	char zReqstSvcId[8];		/* ��û����ID */
	char zSecurCardAppTp[1];	/* ����ī�����뱸�� */
};
#define L_TR8014_mid		sizeof(struct TR8014_mid)

struct TR8014_mod {
	struct TR8014_mid mid;
	char Out[5];					/* ���ڵ尹�� */
	char zOnesfCnfTrxSeqno[5];		/* ����Ȯ��ó���Ϸù�ȣ */
	char zOnesfCnfPtnTp[1];			/* ����Ȯ���������� */
	char zRetryAbleCnt[2];			/* ��õ�����Ƚ�� */
	char zSecurCardSeqnoLen[2];		/* ����ī���Ϸù�ȣ���� */
	char zSecurCardSeqnoInptPos[3];	/* ����ī���Ϸù�ȣ�Է���ġ */
	char zSecurCardInptNo1[2];		/* ����ī���Է¹�ȣ1 */
	char zSecurCardInptNo2[2];		/* ����ī���Է¹�ȣ2 */
};
#define L_TR8014_mod		sizeof(struct TR8014_mod)

/******************************************************************************
* NAME : TR8015
* DESC : ���Ⱥ�й�ȣ��û
 ******************************************************************************/
struct TR8015_mid {
	char In[5];				/* ���ڵ� ����      */
	char zAcntNo[20];		/* ���¹�ȣ */
	char zInptPwd[8];		/* ��й�ȣ */
};
#define L_TR8015_mid		sizeof(struct TR8015_mid)

struct TR8015_mod {
	struct TR8015_mid mid;
	char Out[5];				/* ���ڵ尹�� */
	char zAcntNm[40];			/* ���¸� */
	char zAvalYn[1];			/* ��ȿ���� */
	char zPdptnCode[2];			/* ��ǰ�����ڵ� */
	char zPrdtDtlCode[2];		/* ��ǰ���ڵ� */
	char zErrCnt[10];			/* �����Ǽ� */
};
#define L_TR8015_mod		sizeof(struct TR8015_mod)
//////////////////////////////////////////////////////////////////////////