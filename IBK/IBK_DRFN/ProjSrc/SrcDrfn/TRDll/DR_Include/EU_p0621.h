/*************************************************************************************/
/*  1. ���α׷�ID : EU_p0621                                                         */
/*  2. ���α׷��� : ���ǰ˻�_1                                                       */
/*  3. ���񽺸�   : EU_p0621                                                         */
/*  4. ������̺� :                                                                  */
/*  5. �ۼ���     : drfn                                                             */
/*  6. �Է��׸�   :                                                                  */
/*  7. ����׸�   :                                                                  */
/*************************************************************************************/
#ifndef __EU_TRAN_EU_p0621_H__
#define __EU_TRAN_EU_p0621_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
#define EU_TRAN0621                       "EU_p0621"
#define LLONG                         long long

#define RESULT_FIELD_MAX_EUGENE     240
/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec1:                                                                           */
typedef struct
{
	char    sJongFillterChk[1];					  /* ������� üũ ����  '0' : No, '1' : Yes */       
	char    sJongFillterIndex[RESULT_MAX];		  /* ��� �����ڵ�Bitset          */

    char    giCallGubun[4];                       /* �ݱ���(4)                       */
    char    giTargetGubun[2];                     /* Ÿ�ٱ���(2)                     */
    char    giTargetIdxCnt[2];                    /* �����ǥ����(2)                 */
    char    giTargetResultViewCnt[2];             /* ��󰪺����ʵ尹��(2)           */
    char    gsTargetExp[80];                      /* �����������ǥ����(80)          */
    char    giComplexIdxCnt[2];                   /* ������ǥ����(2)                 */
    char    giComplexResultViewCnt[2];            /* ���հ������ʵ尳��(2)           */
    char    gsComplexExp[80];                     /* ������������ǥ����(80)          */
    char    giFieldEditerCnt[2];                  /* �����ʵ���������(2)             */
    char    giFieldList[RESULT_FIELD_MAX_EUGENE];                      /* �ʵ�������ȣ����Ʈ(80)          */
    char    giResultIdxCnt[4];                    /* ������� ����         NEW(4)  */
    char    gsResultExp[80];                      /* ����� ������ ǥ�� �� NEW(80) */
    char    gsTargetBitChk[1];                    /* TargetBit(1)                    */
    char    gsJongmokIndex[4000];           		/* �����ڵ�Bitset(4000)            */	//3000 -> 4000
    char    gsSort_flag[1];                       /* ���� ����(0:����, 1:����)(1)    */
    char    giSort_Pos[4];                        /* ���� ��ȣ(�������ʵ�pos)(4)     */
    char    giBackDate[8];                        /* backtest ����(�̻��:-1)(4)     */
    char    giBackTime[6];                        /* backtest �ð�(�̻��:-1)(4)     */
}EU_p0621_InRec1;

/* InRec2:                                                                           */
char EU_p0621_InRec2_count[4];
typedef struct
{
    char    giIndexNo[2];                         /* ��ǥ��ȣ(2)                     */
    char    giGroupNo[2];                         /* �׷��ȣ(2)                     */
    char    giGroupIdxCnt[2];                     /* �׷쳻����ǥ����(2)             */
    char    giTargetBase[2];                      /* Ÿ�ٵ���Ÿ(2)                   */
    char    giType[4];                            /* ��ǥ������(4)                   */
    char    giType1[2];                           /* Type1��������(2)                */
    char    giType2[2];                           /* ������(2)                     */
    char    giSTerm[4];                           /* �׷�˻�������ġ(4)             */
    char    giETerm[4];                           /* �׷�˻�����ġ(4)               */
    char    giRank[4];                            /* �����˻������(4)               */
    char    giTrend[4];                           /* �߼�Ƚ��(4)                     */
    char    giSignalCnt1[4];                      /* ��ȣ�߻�Ƚ��1(4)                */
    char    giSignalCnt2[4];                      /* ��ȣ�߻�Ƚ��2(4)                */
    char    giResultViewNum[2];                   /* ����������ʵ尹��(2)           */
    char    gsIndexType[1];                       /* �˻�����0:�Ϲ�1:����2:����(1)   */
    char    gsSearchType[1];                      /* �˻����� 0:�Ϲ� 1:���������(1) */
    char    gsOperator[1];                        /* ������0:OR1:AND2:NEXT(1)        */
    char    gsReverse[1];                         /* �������0:A1:NotA(1)            */
    char    gsIdxContiGubun[1];                   /* 0:���Ϲ߻�1:���ӹ߻���ǥ(1)     */
    char    gsElement[1];                         /* ���ǽĻ�빮��(A)(1)            */
    char    gsScode[6];                           /* shortcode(6)                    */
//	char    gdParam[48];                          /* ��ǥ���ú�����(48)              */
//	char    gdCondition[96];                      /* ��Ÿ����������(96)              */
    char    gdParam[96];                          /* ��ǥ���ú�����(48)              */
    char    gdCondition[192];                      /* ��Ÿ����������(96)              */
}EU_p0621_InRec2;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
typedef struct
{
    char    giCallGubun[4];                       /* �ݱ���(4)                       */
    char    giTotalJongCnt[4];                    /* �˻������������(4)             */
    char    giOutFieldCnt[2];                     /* OutFieldCount(2)                */
    char    giOutListPacketSize[4];               /* ���List������(������)(4)       */
    char    giFindTime[8];                        /* ���ǰ˻�����ð�(8)             */
}EU_p0621_OutRec1;

/* OutRec2:                                                                          */
char EU_p0621_OutRec2_count[4];
typedef struct
{
    char    gsDaebi[1];                           /* ����ȣ(1)                     */
    char    gsCode[6];                            /* �����ڵ�(6)                     */
    char    gsHname[20];                          /* �����(20)                      */
    char    gsMarket[1];                          /* ���ϱ���(1)                     */
    char    giOrderRank[4];                       /* �����˻�����(4)                 */
    char    gdResultField[320];                   /* ����ʵ�(320)                   */
}EU_p0621_OutRec2;

/*************************************************************************************/

#endif

