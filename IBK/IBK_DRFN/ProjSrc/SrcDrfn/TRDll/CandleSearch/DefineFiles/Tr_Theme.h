#if !defined(AFX_TR_THEME_DLDLDEE_H__AF_AD91_7EE99FC62AAF__INCLUDED_)
#define AFX_TR_THEME_DLDLDEE_H__AF_AD91_7EE99FC62AAF__INCLUDED_

//#include "TreeList_Thema.h"


//[[�Ｚ�׸����� 
/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec: �׸���ȸ                                                                   */
typedef struct
{
    char    Sect2[1];                             /* ����2(1)                        */
    char    WMonthSect[1];                        /* �ֿ�������(1)                   */
    char    RefrWord2[50];                        /* ��ȸ�ܾ�2(50)                   */
}fn_HFIBOA076220_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1: �׸���ȸ                                                                 */
typedef struct
{
    char    PrntCnt3[4];                          /* ��°Ǽ�3(4)                    */
    char    filler;                               /*                                 */
}fn_HFIBOA076220_OutRec1;

/* OutRec2: �׸���ȸ                                                                 */
// fn_HFIBOA076220_OutRec2�� �迭 ���� = 'fn_HFIBOA076220_OutRec1.PrntCnt3'
typedef struct
{
    char    ThemaGrpCd[5];                        /* �׸��׷��ڵ�(5)                 */
    char    ThemaGrpNm[50];                       /* �׸��׷��(50)                  */
}fn_HFIBOA076220_OutRec2;

/*************************************************************************************/



/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec: ������ȸ                                                                   */
typedef struct
{
    char    InItem1[4];                           /* #30(4)                          */
    char    InItem2[1];                           /* #(1)                            */
    char    ContinueKey4[6];                      /* ����Ű4(6)                      */
    char    ThemaCd[5];                           /* �׸��ڵ�(5)                     */
}fn_HFIBOA076210_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1: ������ȸ                                                                 */
typedef struct
{
    char    PrntCnt3[4];                          /* ��°Ǽ�3(4)                    */
    char    EndingYN[1];                          /* ������(1)                       */
    byte    OutItem3[3];                             /* #3(3)                           */
    byte    OutItem4;                             /* #1(1)                           */
    char    ContinueKey4[6];                      /* ����Ű4(6)                      */
    byte    OutItem6;                             /* #1(1)                           */
}fn_HFIBOA076210_OutRec1;

/* OutRec2: ������ȸ                                                                 */
// fn_HFIBOA076210_OutRec2�� �迭 ���� = 'fn_HFIBOA076210_OutRec1.PrntCnt3'
typedef struct
{
    byte    OutItem1;                             /* #1(1)                           */
    char    ItemCd13[6];                          /* �����ڵ�13(6)                   */
    byte    OutItem3;                             /* #1(1)                           */
    char    ItemNm1[20];                          /* �����1(20)                     */
    long    CurPrc;                               /* ���簡(4)                       */
    char    Symbl[1];                             /* ��ȣ(1)                         */
    byte    OutItem7[3];                          /* #3(3)                           */
    long    UpNDn1;                               /* ���1(4)                        */
    long    UpNDnRt3;                             /* �����(4)                       */
    long    TrdVol1;                              /* �ŷ���1(4)                      */
}fn_HFIBOA076210_OutRec2;
/*************************************************************************************/

#endif // !defined(AFX_TR_THEME_DLDLDEE_H__AF_AD91_7EE99FC62AAF__INCLUDED_)