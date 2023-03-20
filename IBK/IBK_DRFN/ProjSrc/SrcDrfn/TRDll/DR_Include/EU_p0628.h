/*************************************************************************************/
/*  1. ���α׷�ID : EU_p0628                                                         */
/*  2. ���α׷��� : ���ǰ˻�_�׸�����Ʈ��ȸ                                          */
/*  3. ���񽺸�   : EU_p0628                                                         */
/*  4. ������̺� :                                                                  */
/*  5. �ۼ���     : drfn                                                             */
/*  6. �Է��׸�   :                                                                  */
/*  7. ����׸�   :                                                                  */
/*************************************************************************************/
#ifndef __EU_TRAN_EU_p0628_H__
#define __EU_TRAN_EU_p0628_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
#define EU_TRAN0628                       "EU_p0628"
#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec1:                                                                           */
typedef struct
{
    char    giCallIndex[4];                       /* giCallIndex(4)                  */
    char    gsCode[6];                            /* ��ȸ�������(6)                 */
    char    gsFilterGubun[1];                     /* ���ͱ���(1)                     */
}EU_p0628_InRec1;

/* InRec2:                                                                           */
typedef struct
{
    char    giCallGubun[4];                       /* giCallGubun(4)                  */
    char    giTargetGubun[2];                     /* Ÿ�ٱ���(2)                     */
    char    giTargetIdxCnt[2];                    /* �����ǥ����(2)                 */
    char    giTargetResultViewCnt[2];             /* �������ʵ尹��(2)               */
    char    gsTargetExp[80];                      /* �����������ǥ����(80)          */
    char    giComplexIdxCnt[2];                   /* ������ǥ����(2)                 */
    char    giComplexResultViewCnt[2];            /* ���հ����ⰹ��(2)               */
    char    gsComplexExp[80];                     /* ������������ǥ����(80)          */
    char    giResultIdxCnt[4];                    /* ������񱳰���(4)               */
    char    gsResultExp[80];                      /* �����������(80)              */
    char    giFieldEditerCnt[2];                  /* �����ʵ���������(2)             */
    char    giFieldList[80];                      /* �ʵ�������ȣ����Ʈ(80)          */
    char    gsTargetBitChk[1];                    /* TargetBit(1)                    */
    char    gsJongmokIndex[4000];           	  /* �����ڵ�Bitset(4000)            */	//3000 -> 4000
    char    gsSort_flag[1];                       /* ���Ĺ���(1)                     */
    char    giSort_Pos[4];                        /* ���Ĺ�ȣ(4)                     */
    char    giBackDate[4];                        /* backtest����(4)                 */
    char    giBackTime[4];                        /* backtest�ð�(4)                 */
}EU_p0628_InRec2;

/* InRec3:                                                                           */
char EU_p0628_InRec3_count[4];
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
    char    gsSearchType[1];                      /* �˻�����(1)                     */
    char    gsOperator[1];                        /* ������0:OR1:AND2:NEXT(1)        */
    char    gsReverse[1];                         /* �������0:A1:NotA(1)            */
    char    gsIdxContiGubun[1];                   /* 0:���Ϲ߻�1:���ӹ߻���ǥ(1)     */
    char    gsElement[1];                         /* ���ǽĻ�빮��(A)(1)            */
    char    gsScode[6];                           /* shortcode(6)                    */
    char    gdParam[48];                          /* ��ǥ���ú�����(48)              */
    char    gdCondition[96];                      /* ��Ÿ����������(96)              */
}EU_p0628_InRec3;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
typedef struct
{
    char    giCallIndex[4];                       /* giCallIndex(4)                  */
    char    giThemeCnt[4];                        /* giThemeCnt(4)                   */
}EU_p0628_OutRec1;

/* OutRec2:                                                                          */
char EU_p0628_OutRec2_count[4];
typedef struct
{
    char    gsThemeCode[4];                       /* �׸��ڵ�(4)                     */
    char    gsThemeName[20];                      /* �׸���(20)                      */
}EU_p0628_OutRec2;

/*************************************************************************************/

#endif

