#include "Open_API_OUT.h"

#define TRANSFER_URL	(1)	//��ü
#define CONTRACT_URL	(2)	//������û
#define CME_URL			(3)	//CME �߰����� �ŷ���û/����
#define QNA_URL			(4)	//QnA

#pragma once

#pragma	pack(1)

//////////////////////////////////////////////////////////////////////////////
//PTS �߰�
//////////////////////////////////////////////////////////////////////////////

/******************************************************************************
* NAME : TR4224
* DESC : �����ɼ� - �����ɼ� �������Ͽ������ű���ȸ2
 ******************************************************************************/
struct TR4224_mid
{
	char In[5];							/* ���ڵ尹��             */
	char zAcntNo[20];					/* ���¹�ȣ               */
	char zInptPwd[8];					/* �Էº�й�ȣ           */
	char zKpi200Idx[13];				/* �ڽ���200����          */
};
#define L_TR4224_mid	sizeof(struct TR4224_mid)

struct TR4224_grid
{
	char zFnoIsuNo[32];					/* �����ɼ������ȣ			*/
	char zBnsTp[1];						/* �Ÿű���					*/
	char zBnsTpNm[10];					/* �Ÿű���					*/
	char zUnsttQty[16];					/* �̰�������				*/
	char zCurPrc[13];					/* ���簡					*/
	char zIvlatRat[7];					/* ���纯��������			*/
	char zFcastThrprc[19];				/* �����̷а�				*/
	char zRemnDays[6];					/* �����ϼ�					*/
};
#define L_TR4224_grid		sizeof(struct TR4224_grid)

struct TR4224_mod
{
	struct TR4224_mid mid;
	char Out[5];						/* ���ڵ尹��				*/
	char zAcntNm[40];					/* ���¸�					*/
	char zFnoAcntAfmgnNm[16];			/* �����ɼǰ��»������űݸ�	*/
	char zDpsTotamt[16];				/* �������Ѿ�				*/
	char zDps[16];						/* ������					*/
	char zSubstAmt[16];					/* ���ݾ�					*/
	char zCsgnMgn[16];					/* ��Ź���űݾ�				*/
	char zMnyCsgnMgn[16];				/* ������Ź���űݾ�			*/
	char zAddMgn[16];					/* �߰����űݾ�				*/
	char zMnyAddMgn[16];				/* �����߰����űݾ�			*/
	char zPsnOutAbleAmt[16];			/* ���Ⱑ�ɱݾ�				*/
	char zPsnOutAbleCurAmt[16];			/* ���Ⱑ�����ݾ�			*/
	char zOrdAbleTotAmt[16];			/* �ֹ������ѱݾ�			*/
	char zOrdAbleAmt[16];				/* �ֹ����ɱݾ�				*/
	char zOut2[5];						/* ���ڵ尹��				*/
	char zDpsTotamt2[16];				/* �������Ѿ�				*/
	char zDps2[16];						/* ������					*/
	char zSubstAmt2[16];					/* ���ݾ�					*/
	char zCsgnMgn2[16];					/* ��Ź���űݾ�				*/
	char zMnyCsgnMgn2[16];				/* ������Ź���űݾ�			*/
	char zAddMgn2[16];					/* �߰����űݾ�				*/
	char zMnyAddMgn2[16];				/* �����߰����űݾ�			*/
	char zPsnOutAbleAmt2[16];			/* ���Ⱑ�ɱݾ�				*/
	char zPsnOutAbleCurAmt2[16];			/* ���Ⱑ�����ݾ�			*/
	char zOrdAbleTotAmt2[16];			/* �ֹ������ѱݾ�			*/
	char zOrdAbleAmt2[16];				/* �ֹ����ɱݾ�				*/
	char zOut3[5];						/* ���ڵ尹��				*/
	char zDpstgTotamtLackAmt[16];		/* ��Ź�Ѿ׺����ݾ�(��Ź���űݱ���)				*/
	char zDpstgMnyLackAmt[16];			/* ��Ź���ݺ����ݾ�(��Ź���űݱ���)				*/
	char zDpstgTotamtLackAmt1[16];		/* ��Ź�Ѿ׺����ݾ�(�������űݱ���)				*/
	char zDpstgMnyLackAmt1[16];			/* ��Ź���ݺ����ݾ�(�������űݱ���)				*/
	char zFutsThdayDfamt[16];			/* ������������				*/
	char zFutsUdamt[16];				/* ������������				*/
	char zFutsLastSettDfamt[16];		/* ����������������			*/
	char zAdjstAmt[16];					/* ����ݾ�					*/
	char zOptBuyAmt[16];				/* �ɼǸż��ݾ�				*/
	char zOptSellAmt[16];				/* �ɼǸŵ��ݾ�				*/
	char zOptRgtexeDfamt[16];			/* �ɼǱǸ��������			*/
	char zOptRgtAsgnDfamt[16];			/* �ɼǱǸ���������			*/
	char zFutsCmsnAmt[16];				/* ����������				*/
	char zOptCmsnAmt[16];				/* �ɼǼ�����				*/
	char zFnoPrdaySubstSellAmt[16];		/* �����ɼ����ϴ��ŵ��ݾ�				*/
	char zFnoCrdaySubstSellAmt[16];		/* �����ɼǱ��ϴ��ŵ��ݾ�				*/
	char zOptEvalPnlAmt[16];			/* �ɼ��򰡼��ͱݾ�			*/
	char zOut4[16];						/* ���ڵ尹��				*/
	struct TR4224_grid grid[1];
};
#define L_TR4224_mod		sizeof(struct TR4224_mod)

/******************************************************************************
* NAME : TR4223
* DESC : �����ɼ� - �����ɼ� �߰����űݹ߻���Ȳ
 ******************************************************************************/
struct TR4223_mid
{
	char In[5];							/* ���ڵ尹��           */
	char zAcntNo[20];					/* ���¹�ȣ				*/
	char zPwd[8];						/* ��й�ȣ				*/
	char zSrtDt[8];						/* ������				*/
	char zEndDt[8];						/* ������				*/
};
#define L_TR4223_mid	sizeof(struct TR4223_mid)

struct TR4223_grid
{
	char zOcrDt[8];						/* �߻���					*/
	char zEvalDpsamtTotamt[15];			/* �򰡿�Ź���Ѿ�			*/
	char zOpnmkAddMgn[16];				/* �����߰����űݾ�			*/
	char zOpnmkMnyAddMgn[16];			/* ���������߰����űݾ�		*/
	char zOpnmkFcurrAddMgn[16];			/* �����ȭ�߰����ű�		*/
	char zOpnmkAddMgnTpNm[12];			/* �߰����űݻ���			*/
	char zAddMgnTpNm[12];				/* �߰����űݹ߻�			*/
	char zIsuNo[12];					/* �����ȣ					*/
	char zBnsTp[1];						/* �Ÿű���					*/
	char zBnsTpNm[10];					/* �Ÿű���					*/
	char zCvrgQty[16];					/* �ݴ�Ÿż���				*/
	char zOrdPrc[13];					/* �ֹ���					*/
	char zCvrgAmt[16];					/* �ݴ�Ÿűݾ�				*/
	char zLnkAcntNo[20];				/* ������¹�ȣ				*/
	char zAddMgnTrxTp[1];				/* �߰����ű�ó������		*/
	char zTrxStatNm[20];				/* ó�����¸�				*/
};
#define L_TR4223_grid	sizeof(struct TR4223_grid)

struct TR4223_mod
{
	struct TR4223_mid mid;
	char Out[5];						/* ���ڵ尹��				*/
	struct TR4223_grid grid[1];
};
#define L_TR4223_mod		sizeof(struct TR4223_mod)

/******************************************************************************
* NAME : TR4303
* DESC : �����ɼ� - �����ɼ� �Ϻ� ���¼��ͳ���
 ******************************************************************************/
struct TR4303_mid
{
	char In[5];							/* ���ڵ尹��           */
	char zAcntNo[20];					/* ���¹�ȣ				*/
	char zPwd[8];						/* ��й�ȣ				*/
	char zQrySrtDt[8];					/* ��ȸ������			*/
	char zQryEndDt[8];					/* ��ȸ������			*/
	char zQryTp[1];						/* ��ȸ����			1.���ں�, 2.����, 3.�ְ���	*/
	char zStnlnSeqTp[1];				/* ���ļ�������		1.��, 2.����*/
	char zBalEvalTp[1];					/* �ܰ��򰡱���		0.�⺻, 1.�̵���չ�, 2.���Լ����	*/
};
#define L_TR4303_mid	sizeof(struct TR4303_mid)

struct TR4303_grid
{
	char zQryDt[8];			/*��ȸ��*/
	char zDpstgTotamt[16];	/*��Ź�Ѿ�*/
	char zDpstgMny[16];		/*��Ź����*/
	char zFnoMgn[16];		/*�����ɼ����űݾ�*/
	char zFutsPnlAmt[16];	/*�������ͱݾ�*/
	char zOptBsnPnlAmt[16];	/*�ɼǸŸż��ͱݾ�*/
	char zOptEvalPnlAmt[16];/*�ɼ��򰡼��ͱݾ�*/
	char zCmsnAmt[16];		/*������*/
	char zSumAmt1[16];		/*�հ�ݾ�1*/
	char zSumAmt2[16];		/*�հ�ݾ�*/
	char zPnlSumAmt[16];	/*�����հ�ݾ�*/
	char zFutsBuyAmt[16];	/*�����ż��ݾ�*/
	char zFutsSellAmt[16];	/*�����ŵ��ݾ�*/
	char zOptBuyAmt[16];	/*�ɼǸż��ݾ�*/
	char zOptSellAmt[16];	/*�ɼǸŵ��ݾ�*/
	char zInAmt[16];		/*�Աݾ�*/
	char zOutAmt[16];		/*��ݾ�*/
	char zEvalAmt[16];		/*�򰡱ݾ�*/
	char zAddupEvalAmt[16];	/*�ջ��򰡱ݾ�*/
	char zAmt2[16];			/*�ݾ�2*/
};
#define L_TR4303_grid		sizeof(struct TR4303_grid)

struct TR4303_mod
{
	struct TR4303_mid mid;
	char Out[5];						/* ���ڵ尹��				*/
	char zFutsAdjstDfamt[16];			/*������������*/
	char zOptBnsplAmt[16];				/*�ɼǸŸż��ͱݾ�*/
	char zFnoCmsnAmt[16];				/*�����ɼǼ�����*/
	char zPnlSumAmt[16];				/*�����հ�ݾ�*/
	char zMnyinAsmAmt[16];				/*�Աݴ���ݾ�*/
	char zMnyoutAsmAmt[16];				/*��ݴ���ݾ�*/
	char zAcntNm[40];					/*���¸� */
	char Out2[5];                       /* ���ڵ尹��             */
    struct TR4303_grid grid[1];
};
#define L_TR4303_mod		sizeof(struct TR4303_mod)

/******************************************************************************
* NAME : TR4201
* DESC : �����ɼ� - �����ɼ� �����ܰ���ȸ
 ******************************************************************************/
struct TR4201_mid
{
	char In[5];							/* ���ڵ尹��           */
	char zAcntNo[20];					/* ���¹�ȣ				*/
	char zPwd[8];						/* ��й�ȣ				*/
	char zFnoClssCode[2];				/* �����ɼǺз��ڵ�		00.��ü, 11.����, 12.�ɼ�*/
	char zPdGrpCode[2];					/* ��ǰ���ڵ�			default 00 */
};
#define L_TR4201_mid	sizeof(struct TR4201_mid)

struct TR4201_grid
{
	char zFnoIsuNo[32];		/* �����ɼ������ȣ */
	char zBnsTp[1];			/* �Ÿű��� */
	char zBnsTpNm[10];		/* �Ÿű��� */
	char zBgnDt[8];			/* ������ */
	char zUnsttQty[16];		/* �̰������� */
	char zFnoAvrPrc[19];	/* ��հ� */
	char zCurprc[13];		/* ���簡 */
	char zUnsttAmt[16];		/* �̰����ݾ� */
	char zEvalPnlAmt[16];	/* �򰡼��ͱݾ� */
};
#define L_TR4201_grid		sizeof(struct TR4201_grid)

struct TR4201_mod
{
	struct TR4201_mid mid;
	char Out[5];				/* ���ڵ尹�� */
	char zAcntNm[40];			/* ���¸� */
	char zDpstgTotamt[16];		/* ��Ź�Ѿ� */
	char zDps[16];				/* ������ */
	char zMgn[16];				/* ���űݾ� */
	char zMnyMgn[16];			/* �������űݾ� */
	char zOrdAbleAmt[16];		/* �ֹ����ɱݾ� */
	char zMnyOrdAbleAmt[16];	/* �����ֹ����ɱݾ� */
	char zPsnOutAbleAmt[16];	/* ���Ⱑ�ɱݾ� */
	char zPsnOutAbleCurAmt[16];	/* ���Ⱑ�����ݾ� */
	char zFutsPnlAmt[16];		/* �������ͱݾ� */
	char zOptBnsplAmt[16];		/* �ɼǸŸż��ͱݾ� */
	char zPnlSumAmt[16];		/* �����հ�ݾ� */
	char Out2[5];               /* ���ڵ尹�� */
    struct TR4201_grid grid[1];
};
#define L_TR4201_mod		sizeof(struct TR4201_mod)

/******************************************************************************
* NAME : TR4101
* DESC : ���� ��й�ȣ ����
 ******************************************************************************/
struct TR4101_mid
{
	char In[5];							/* ���ڵ尹��           */
	char zAcntNo[20];					/* ���¹�ȣ				*/
	char zPwd[8];						/* ��й�ȣ				*/
};
#define L_TR4101_mid	sizeof(struct TR4101_mid)

struct TR4101_mod
{
	struct TR4101_mid mid;
	char Out[5];				/* ���ڵ尹�� */
	char zAvalYn[1];			/* ��ȿ���� */
};
#define L_TR4101_mod		sizeof(struct TR4101_mod)

/******************************************************************************
* NAME : TR4221
* DESC : �����ɼ� ���¿�Ź�����ű���ȸ
 ******************************************************************************/
struct TR4221_mid
{
	char In[5];							/* ���ڵ尹��           */
	char zAcntNo[20];					/* ���¹�ȣ				*/
	char zPwd[8];						/* ��й�ȣ				*/
};
#define L_TR4221_mid	sizeof(struct TR4221_mid)

struct TR4221_grid
{
	char zPdGrpCodeNm[20];				/* ��ǰ���ڵ��*/
	char zNetRiskMgn[16];				/* ���������űݾ�*/
	char zPrcFlctMgn[16];				/* ���ݺ������űݾ�*/
	char zSprdMgn[16];					/* �����������űݾ�*/
	char zMgnRdctAmt[16];				/* ���űݰ�����Ѿ�*/
	char zMinMgn[16];					/* �ּ����űݾ�*/
	char zPrcMgn[16];					/* �������űݾ�*/
	char zOrdMgn[16];					/* �ֹ����űݾ�*/
	char zOptNetBuyAmt[16];				/* �ɼǼ��ż��ݾ�*/
	char zCsgnMgn[16];					/* ��Ź���űݾ�*/
	char zUndMgn[16];					/* �μ������űݾ�*/
	char zMaintMgn[16];					/* �������űݾ�*/
	char zFutsBuyExecAmt[16];			/* �����ż�ü��ݾ�*/
	char zFutsSellExecAmt[16];			/* �����ŵ�ü��ݾ�*/
	char zOptBuyExecAmt[16];			/* �ɼǸż�ü��ݾ�*/
	char zOptSellExecAmt[16];			/* �ɼǸŵ�ü��ݾ�*/
	char zFutsPnlAmt[16];				/* �������ͱݾ�*/
	char zTotRiskCsgnMgn[16];			/* ��������Ź���ű�*/
};
#define L_TR4221_grid		sizeof(struct TR4221_grid)

struct TR4221_mod
{
	struct TR4221_mid mid;
	char Out[5];						/* ���ڵ尹�� */
	char zAcntNm[40];					/* ���¸�	*/
	char zDpsTotamt[16];				/* �������Ѿ�	*/
	char zDps[16];						/* ������		*/
	char zSubstAmt[16];					/* ���ݾ�		*/
	char zFilupDpsamtTotamt[16];		/* ��翹Ź���Ѿ�*/
	char zFilupDps[16];					/* ��翹����*/
	char zFutsPnlAmt[16];				/* �������ͱݾ�*/
	char zPsnOutAbleAmt[16];			/* ���Ⱑ�ɱݾ�*/
	char zPsnOutAbleCurAmt[16];			/* ���Ⱑ�����ݾ�*/
	char zPsnOutAbleSubstAmt[16];		/* ���Ⱑ�ɴ��ݾ�*/
	char zMgn[16];						/* ���űݾ�*/
	char zMnyMgn[16];					/* �������űݾ�*/
	char zOrdAbleAmt[16];				/* �ֹ����ɱݾ�*/
	char zMnyOrdAbleAmt[16];			/* �����ֹ����ɱݾ�*/
	char zAddMgn[16];					/* �߰����űݾ�*/
	char zMnyAddMgn[16];				/* �����߰����űݾ�*/
	char zAmtPrdayChckInAmt[16];		/* �����ϼ�ǥ�Աݾ�*/
	char zFnoPrdaySubstSellAmt[16];		/* �����ɼ����ϴ��ŵ��ݾ�*/
	char zFnoCrdaySubstSellAmt[16];		/* �����ɼǱ��ϴ��ŵ��ݾ�*/
	char zFnoPrdayFdamt[16];			/* �����ɼ����ϰ��Աݾ�*/
	char zFnoCrdayFdamt[16];			/* �����ɼǱ��ϰ��Աݾ�*/
	char zFcurrSubstAmt[16];			/* ��ȭ���ݾ�*/
	char zFnoAcntAfmgnNm[20];			/* �����ɼǰ��»������űݸ�*/
	char Out2[5];						/* ���ڵ尹�� */
    struct TR4221_grid grid[1];
};
#define L_TR4221_mod		sizeof(struct TR4221_mod)

/******************************************************************************
* NAME : TR4401
* DESC : �����ɼ� ���� �ŷ�����
 ******************************************************************************/
struct TR4401_mid
{
	char In[5];							/* ���ڵ尹��           */
	char zQryTp[1];						/* ��ȸ���� 0.��ü, 1.�����, 2.�����, 3.�Ÿ� */
	char zAcntNo[20];					/* ���¹�ȣ*/
	char zPwd[8];						/* ��й�ȣ*/
	char zQrySrtDt[8];					/* ��ȸ������*/
	char zQryEndDt[8];					/* ��ȸ������*/
	char zSrtNo[10];					/* ���۹�ȣ Default +000000000 */
	char zPdptnCode[2];					/* ��ǰ�����ڵ� Default 00 */
	char zIsuNo[12];					/* �����ȣ*/
};
#define L_TR4401_mid	sizeof(struct TR4401_mid)

struct TR4401_grid
{
	char zTrdDate[8];					/* �ŷ�����*/
	char zTrdNo[10];					/* �ŷ���ȣ*/
	char zTrdTpNm[20];					/* �ŷ�����*/
	char zSmryNo[4];					/* �����ȣ*/
	char zSmryNm[40];					/* �����*/
	char zCancTpNm[20];					/* ��ұ���*/
	char zTrdQty[16];					/* �ŷ�����*/
	char zTrtax[16];					/* �ŷ���*/
	char zAdjstAmt[16];					/* ����ݾ�*/
	char zOvdSum[16];					/* ��ü��*/
	char zDpsBfbalAmt[16];				/* ���������ܱݾ�*/
	char zSellPldgRfundAmt[16];			/* �ŵ��㺸��ȯ��*/
	char zDpspdgLoanBfbalAmt[16];		/* ��Ź�㺸�������ܱݾ�*/
	char zTrdmdaNm[40];					/* �ŷ���ü��*/
	char zOrgTrdNo[10];					/* ���ŷ���ȣ*/
	char zIsuNm[40];					/* �����*/
	char zTrdUprc[13];					/* �ŷ��ܰ�*/
	char zCmsnAmt[16];					/* ������*/
	char zRfundDiffAmt[16];				/* ��ȯ���̱ݾ�*/
	char zRepayAmtSum[16];				/* �������հ�*/
	char zSecCrbalQty[16];				/* �������Ǳ��ܼ���*/
	char zCslLoanRfundIntrstAmt[16];	/* �ŵ���ݴ㺸�����ȯ���ڱݾ�*/
	char zDpspdgLoanCrbalAmt[16];		/* ��Ź�㺸������ܱݾ�*/
	char zTrxTime[9];					/* ó���ð�*/
	char zInouno[10];					/* �ⳳ��ȣ*/
	char zIsuNo[12];					/* �����ȣ*/
	char zTrdAmt[16];					/* �ŷ��ݾ�*/
	char zTaxSumAmt[16];				/* �����հ�ݾ�*/
	char zIntrstUtlfee[16];				/* �����̿��*/
	char zDvdAmt[16];					/* ���ݾ�*/
	char zRcvblOcrAmt[16];				/* �̼��߻��ݾ�*/
	char zTrxBrnNo[3];					/* ó��������ȣ*/
	char zTrxBrnNm[40];					/* ó��������*/
	char zDpspdgLoanAmt[16];			/* ��Ź�㺸����ݾ�*/
	char zDpspdgLoanRfundAmt[16];		/* ��Ź�㺸�����ȯ�ݾ�*/
	char zBasePrc[13];					/* ���ذ�*/
	char zDpsCrbalAmt[16];				/* �����ݱ��ܱݾ�*/
	char zBoa[16];						/* ��ǥ*/
	char zMnyoutAbleAmt[16];			/* ��ݰ��ɱݾ�*/
	char zBcrLoanOcrAmt[16];			/* �������Ǵ㺸����߻���*/
	char zBcrLoanBfbalAmt[16];			/* �������Ǵ㺸�������ܱ�*/
	char zBnsBasePrc[20];				/* �Ÿű��ذ�*/
	char zTaxchrBasePrc[20];			/* �������ذ�    */
	char zTrdUnit[16];					/* �ŷ��¼�      */
	char zBalUnit[16];					/* �ܰ��¼�      */
	char zEvrTax[16];					/* ������      */  
	char zEvalAmt[16];					/* �򰡱ݾ�      */
	char zBcrLoanRfundAmt[16];			/* �������Ǵ㺸�����ȯ��*/
	char zBcrLoanCrbalAmt[16];			/* �������Ǵ㺸������ܱ�*/
	char zAddMgnOcrTotamt[16];			/* �߰����űݹ߻��Ѿ�*/
	char zAddMnyMgnOcrAmt[16];			/* �߰��������űݹ߻��ݾ�*/
	char zAddMgnDfryTotamt[16];			/* �߰����űݳ����Ѿ�*/
	char zAddMnyMgnDfryAmt[16];			/* �߰��������űݳ��αݾ�*/
	char zBnsplAmt[16];					/* �Ÿż��ͱݾ�  */
	char zIctax[16];					/* �ҵ漼        */
	char zIhtax[16];					/* �ֹμ�      */  
	char zLoanDt[8];					/* ������    */    
};
#define L_TR4401_grid		sizeof(struct TR4401_grid)

struct TR4401_mod
{
	struct TR4401_mid mid;
	char Out[5];						/* ���ڵ尹�� */
	char zAcntNm[40];					/* ���¸�	*/
	char Out2[5];						/* ���ڵ尹�� */
    struct TR4401_grid grid[1];
	char Out3[5];						/* ���ڵ尹�� */
	char zPnlSumAmt[16];				/* �����հ�ݾ�  */
	char zCtrctAsm[16];					/* ��������      */
	char zCmsnAmtSumAmt[16];			/* �������հ�ݾ�*/
};
#define L_TR4401_mod		sizeof(struct TR4401_mod)

/******************************************************************************
* NAME : TR4202
* DESC : �����ɼ� �����ܰ� �� ����Ȳ2
 ******************************************************************************/
struct TR4202_mid
{
	char In[5];                        /* ���ڵ尹��             */
	char zAcntNo[20];                  /* ���¹�ȣ               */
	char zInptPwd[8];                  /* �Էº�й�ȣ           */
	char zOrdDt[8];                    /* �ֹ���                 */
	char zBalEvalTp[1];                /* �ܰ��򰡱���           1.�̵���չ�, 2.���Լ����*/
	char zFutsPrcEvalTp[1];            /* ���������򰡱���       1.���ʰ�, 2.��������*/
	char zLqdtQtyQryTp[1];             /* û�������ȸ����       */
};
#define L_TR4202_mid		sizeof(struct TR4202_mid)

struct TR4202_grid
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
#define L_TR4202_grid		sizeof(struct TR4202_grid)

struct TR4202_mod
{
	struct TR4202_mid mid;
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
	struct TR4202_grid grid[1];
};
#define L_TR4202_mod		sizeof(struct TR4202_mod)

/******************************************************************************
* NAME : TR4003
* DESC : �����ɼ� ����û���ֹ�ü�Ό�����ű�
 ******************************************************************************/
struct TR4003_grid_mid
{
	char zFnoIsuNo[32];			/* �����ɼ������ȣ*/
	char zBnsTp[1];				/* �Ÿű���        1.�ŵ�, 2.�ż�*/
	char zBnsTpNm[10];			/* �Ÿű���        1.�ŵ�, 2.�ż�*/
	char zUnsttQty[16];			/* �̰�������      */
	char zLqdtQty[16];			/* û�����        */
	char zExecPrc[13];			/* ü�ᰡ          */
};
#define L_TR4003_grid_mid		sizeof(struct TR4003_grid_mid)

struct TR4003_mid
{
	char In[5];            /* ���ڵ� ����      */
	char zAcntNo[20];      /* ���¹�ȣ         */
	char zInptPwd[8];      /* �Էº�й�ȣ     */
	char zMprcOrdTp[1];    /* ���尡�ֹ�����   0.���簡, 1.���尡 */
};
#define L_TR4003_mid		sizeof(struct TR4003_mid)

struct TR4003_mid2
{
	char In2[5];           /* ���ڵ� ����      */
	struct TR4003_grid_mid grid[100];
};
#define L_TR4003_mid2		sizeof(struct TR4003_mid2)

struct TR4003_mod
{
	struct TR4003_mid mid;
	char In[5];
	struct TR4003_grid_mid grid_mid[1];
	char Out[5];                /* ���ڵ尹��             */
	char zAcntNm[40];			/* ���¸�                  */
	char zCsgnMgn[16];			/* ��Ź���űݾ�			   */
	char zMnyCsgnMgn[16];		/* ������Ź���űݾ�        */
	char zExecAfCsgnMgn[16];	/* ü������Ź���űݾ�      */
	char zExecAfMnyCsgnMgn[16];	/* ü����������Ź���űݾ�  */
};
#define L_TR4003_mod		sizeof(struct TR4003_mod)


/******************************************************************************
* NAME : TR4004
* DESC : �����ɼ� ���������ֹ�
 ******************************************************************************/
struct TR4004_grid_mid
{
	char zFnoIsuNo[32];			/* �����ɼ������ȣ*/
	char zBnsTp[1];				/* �Ÿű���        */
	char zFnoOrdprcPtnCode[2];	/* �����ɼ�ȣ�������ڵ�	03.���尡, 00.���簡*/
	char zOrdQty[16];			/* �ֹ�����*/
	char zOrdPrc[13];			/* �ֹ���*/
	char zCurPrc[13];			/* ���簡*/
	char zOrdNo[10];			/* �ֹ���ȣ Default +000000000 */
	char zMsgCode[4];			/* �޽����ڵ� Default �� */
};
#define L_TR4004_grid_mid		sizeof(struct TR4004_grid_mid)

struct TR4004_mid
{
	char In[5];				/* ���ڵ� ����      */
	char zAcntNo[20];		/* ���¹�ȣ         */
	char zInptPwd[8];		/* �Էº�й�ȣ     */
	char zTrxTp[1];			/* ó������     Default 2*/
	char zFnoTrdPtnCode[2];	/* �����ɼǰŷ������ڵ�   Default 03  */
	char zCommdaCode[2];	/* ��Ÿ�ü�ڵ�     Default 01*/
	char zIp[12];			/* IP�ּ�			*/
};
#define L_TR4004_mid		sizeof(struct TR4004_mid)

struct TR4004_mid2
{
	char In2[5];           /* ���ڵ� ����      */
	struct TR4004_grid_mid grid[100];
};
#define L_TR4004_mid2		sizeof(struct TR4004_mid2)

struct TR4004_mod
{
	struct TR4004_mid mid;
	char In[5];
	struct TR4004_grid_mid grid_mid[1];
	char Out[5];                /* ���ڵ尹��              */
	char zAcntNm[40];			/* ���¸�                  */
	char zMsgCode[4];			/* �޽����ڵ�              */
	char zCsgnMgn[16];			/* ��Ź���űݾ�			   */
	char zMnyCsgnMgn[16];		/* ������Ź���űݾ�        */
	char zExecAfCsgnMgn[16];	/* ü������Ź���űݾ�      */
	char zExecAfMnyCsgnMgn[16];	/* ü����������Ź���űݾ�  */
};
#define L_TR4004_mod		sizeof(struct TR4004_mod)

/******************************************************************************
* NAME : TR4403
* DESC : �����ɼ� ���º� ��ü�����׼����� ����
 ******************************************************************************/

struct TR4403_mid
{
	char In[5];					/* ���ڵ� ����      */
	char zChoicInptTp[1];		/* �����Է±��� Default 3 ��ǥ���¹�ȣ*/
	char zAcntNo[20];			/* ��ǥ���¹�ȣ */
	char zPwd[8];				/* ��й�ȣ */
	char zFnoClssCode[2];		/* �����ɼǺз��ڵ� 00.��ü, 11.����, 12.�ɼ�*/
	char zPdGrpCode[2];			/* ��ǰ���ڵ� Default 00 ��ü*/
	char zCmsnAmtAddupCode[2];	/* �������ջ��ڵ� 
									99.��ü, 00.������, 01.HTS, 02.WTS, 06.�ݼ���, 07.ARS, 
									81.MTS(iphone �ž���), 82.MTS(�ȵ���̵� �ž���),
									85.MTS(�����е�), 86.MTS(��������), 87.MTS(iphone ������),
									88.MTS(�ȵ���̵� ������), 83.MTS(����������)*/
	char zSrtDt[8];				/* ������ */
	char zEndDt[8];				/* ������ */
};
#define L_TR4403_mid		sizeof(struct TR4403_mid)

struct TR4403_grid
{
	char zAcntNo[20];			/* ���¹�ȣ */
	char zAcntNm[40];			/* ���¸� */
	char zRmnno[13];			/* �Ǹ�Ȯ�ι�ȣ */
	char zFnoIsuNo[32];			/* �����ɼ������ȣ */
	char zIsuNm[40];			/* ����� */
	char zCommdaCode[2];		/* ��Ÿ�ü�ڵ� */
	char zCommdaNm[40];			/* ��Ÿ�ü�� */
	char zFnoCmsnAmtGrdCode[20];/* �����ɼǼ��������ڵ� */
	char zCmsnAmtAppCode[2];	/* �����������ڵ� */
	char zCmsnAmtAppUpCodeNm[8];/* �����������ڵ�� */
	char zBuyQty[16];			/* �ż����� */
	char zSellQty[16];			/* �ŵ����� */
	char zQty1[16];				/* ����1 */
	char zBuyctrAmt[16];		/* �ż������ݾ� */
	char zSlctrAmt[16];			/* �ŵ������ݾ� */
	char zBuyCmsnAmt[16];		/* �ż������� */
	char zSellCmsnAmt[16];		/* �ŵ������� */
	char zBnsCmsnAmt[16];		/* �Ÿż����� */
	char zCtrctAmt[16];			/* �����ݾ� */
};
#define L_TR4403_grid		sizeof(struct TR4403_grid)

struct TR4403_mod
{
	struct TR4403_mid mid;
	char Out[5];                /* ���ڵ尹��              */
	struct TR4403_grid grid[1];
};
#define L_TR4403_mod		sizeof(struct TR4403_mod)

/******************************************************************************
* NAME : TR8001
* DESC : �������ȸ
 ******************************************************************************/
struct TR8001_mid {
    char usid[16];
	char accn[16];
};
#define L_TR8001_mid		sizeof(struct TR8001_mid)

struct TR8001_mod {
    char gubn[1];
};
#define L_TR8001_mod		sizeof(struct TR8001_mod)


#pragma	pack()