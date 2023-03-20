/*************************************************************************************/
/*  1. ���α׷�ID : KB_s0001                                                         */
/*  2. ���α׷��� : �ֽ����簡                                                       */
/*  3. ���񽺸�   : KB_s0001                                                         */
/*  4. ������̺� :                                                                  */
/*  5. �ۼ���     : drfn                                                             */
/*  6. �Է��׸�   :                                                                  */
/*  7. ����׸�   :                                                                  */
/*************************************************************************************/
#ifndef __KB_TRAN_KB_s0001_H__
#define __KB_TRAN_KB_s0001_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/
#include "Data_Common.h"

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
//#define KB_TRAN                       "KB_s0001"
#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec1:                                                                           */
typedef struct
{
    char    shcode[6];                            /* �����ڵ�(6)                     */
    char    mode[1];                              /* ��ȸ���(1)                     */
    char    count[4];                             /* ��ȸ�Ǽ�(4)                     */
}KB_s0001_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
//char KB_s0001_OutRec1_count[4];
//typedef struct
//{
//    char    shcode[6];                            /* �����ڵ�(6)                     */
//    char    hname[40];                            /* �����(40)                      */
//    char    price[8];                             /* ���簡(8)                       */
//    char    sign[1];                              /* ���簡���ϴ�񱸺�(1)           */
//    char    change[8];                            /* ���簡���ϴ��(8)               */
//    char    rate[7];                              /* ���簡���ϴ������(7)         */
//    char    volume[12];                           /* �ŷ���(12)                      */
//    char    volrate[7];                           /* �ŷ������ϴ������(7)         */
//    char    value[15];                            /* �ŷ����(15)                    */
//    char    volro[9];                             /* �ŷ���ȸ����(9)                 */
//    char    jnilvolume[12];                       /* ���ϰŷ���(12)                  */
//    char    jnilvalue[15];                        /* ���ϰŷ����(15)                */
//    char    jnilclose[8];                         /* ��������(8)                     */
//    char    creditgubun[1];                       /* �ſ밡�ɿ���(1)                 */
//    char    dambogubun[1];                        /* �㺸���ɿ���(1)                 */
//    char    jkrate[3];                            /* ���űݺ���(3)                   */
//    char    crdrate[3];                           /* �ſ����ű���(3)                 */
//    char    crddate[6];                           /* �ſ�Ⱓ(6)                     */
//    char    crdgrp[1];                            /* �ſ�׷�(1)                     */
//    char    clmtgubun[1];                         /* �ſ��ѵ��ʰ�����(1)             */
//    char    bu12gubun[2];                         /* �Ҽӱ���(2)                     */
//    char    manufgubun[2];                        /* ����������(2)                   */
//    char    upcodem[3];                           /* �����ߺз�(3)                   */
//    char    upcodes[3];                           /* �����Һз�(3)                   */
//    char    gwangubun[2];                         /* ��������(2)                     */
//    char    invwdgubun[1];                        /* ���ڰ����(1)                 */
//    char    invwgubun[2];                         /* �������ǿ���(2)                 */
//    char    dishonest[1];                         /* �Ҽ��ǰ��ÿ���(1)               */
//    char    bjisugubun[2];                        /* ����������񿩺�(2)             */
//    char    govgubun[2];                          /* ���豸�����������񱸺�(2)     */
//    char    invdgubun[2];                         /* ���迹����(2)                 */
//    char    stopgubun[2];                         /* �ŷ���������(2)                 */
//    char    shusjugubun[1];                       /* �켱�ֱ���(1)                   */
//    char    quotesgubun[1];                       /* �⼼����(1)                     */
//    char    jnilquotesgubun[1];                   /* ���ϱ⼼����(1)                 */
//    char    rightsgubun[2];                       /* �ǹ������(2)                   */
//    char    kospi200[2];                          /* KOSPI200����(2)                 */
//    char    kospi2gubun[1];                       /* KOSPI200����(1)                 */
//    char    jisu50100gubun[1];                    /* KOSPI100/KOSPI50����(1)         */
//    char    krx_100[1];                           /* KRX100����(1)                   */
//    char    pivot[8];                             /* �Ǻ�(8)                         */
//    char    recline[8];                           /* ���ؼ�(8)                       */
//    char    resist1[8];                           /* 1������(8)                      */
//    char    support1[8];                          /* 1������(8)                      */
//    char    resist2[8];                           /* 2������(8)                      */
//    char    support2[8];                          /* 2������(8)                      */
//    char    resist_d[8];                          /* D����(8)                        */
//    char    support_d[8];                         /* D����(8)                        */
//    char    high250[8];                           /* 250���ְ�(8)                  */
//    char    high250_rate[7];                      /* 250���ְ������(7)            */
//    char    high250date[8];                       /* 250���ְ�����(8)              */
//    char    low250[8];                            /* 250��������(8)                  */
//    char    low250_rate[7];                       /* 250�������������(7)            */
//    char    low250date[8];                        /* 250������������(8)              */
//    char    high52w[8];                           /* 52���ְ�(8)                   */
//    char    high52w_rate[7];                      /* 52���ְ������(7)             */
//    char    high52wdate[8];                       /* 52���ְ�����(8)               */
//    char    low52w[8];                            /* 52��������(8)                   */
//    char    low52w_rate[7];                       /* 52�������������(7)             */
//    char    low52wdate[8];                        /* 52������������(8)               */
//    char    recprice[8];                          /* ���ذ�(8)                       */
//    char    uplmtprice[8];                        /* ���Ѱ�(8)                       */
//    char    dnlmtprice[8];                        /* ���Ѱ�(8)                       */
//    char    w_avrg[8];                            /* ��������ְ�(8)                 */
//    char    trade_cost[8];                        /* �ŷ����(8)                     */
//    char    open[8];                              /* �ð�(8)                         */
//    char    opensign[1];                          /* �ð����ϴ�񱸺�(1)             */
//    char    openchange[8];                        /* �ð����ϴ��(8)                 */
//    char    openrate[7];                          /* �ð����������������(7)       */
//    char    opentime[6];                          /* �ð��ð�(6)                     */
//    char    high[8];                              /* ��(8)                         */
//    char    highsign[1];                          /* �����ϴ�񱸺�(1)             */
//    char    highchange[8];                        /* �����ϴ��(8)                 */
//    char    highrate[7];                          /* �����������������(7)       */
//    char    hightime[6];                          /* ���ð�(6)                     */
//    char    low[8];                               /* ����(8)                         */
//    char    lowsign[1];                           /* �������ϴ�񱸺�(1)             */
//    char    lowchange[8];                         /* �������ϴ��(8)                 */
//    char    lowrate[7];                           /* �������������������(7)       */
//    char    lowtime[6];                           /* �����ð�(6)                     */
//    char    target[8];                            /* ��ǥ��(8)                       */
//    char    listdate[8];                          /* ������(8)                       */
//    char    floating[12];                         /* �����ֽļ�(12)                  */
//    char    yjrate[8];                            /* �ſ������ܰ����(8)             */
//    char    yjvolume[6];                          /* �ſ������ܰ��ּ�(6)             */
//    char    yjprice[8];                           /* �ſ������ܰ�ݾ�(8)             */
//    char    subprice[8];                          /* ��밡(8)                       */
//    char    perx[6];                              /* PER(6)                          */
//    char    pbrx[6];                              /* PBR(6)                          */
//    char    psrx[6];                              /* PSR(6)                          */
//    char    epsx[8];                              /* EPS(8)                          */
//    char    parprice[8];                          /* �׸鰡(8)                       */
//    char    listing[12];                          /* �����ֽļ�(12)                  */
//    char    capital[17];                          /* �ں���(17)                      */
//    char    flmtvolume[12];                       /* �ܱ����ֹ��ѵ�����(12)          */
//    char    fremainreal[12];                      /* �ܱ����ֹ����ɼ���(12)          */
//    char    jonglmtrate[6];                       /* �ܱ��������ѵ�����(6)           */
//    char    perlmtrate[6];                        /* �ܱ��ΰ����ѵ�����(6)           */
//    char    fholdvol[12];                         /* �ܱ��κ�������(12)              */
//    char    exhratio[6];                          /* �ܱ��κ�������(6)               */
//    char    sigatvalue[12];                       /* �ð��Ѿ�(12)                    */
//    char    gsmm[2];                              /* ����(2)                       */
//    char    bfname[10];                           /* ���б��(10)                    */
//    char    bfsales[12];                          /* ���б�����(12)                */
//    char    bfoper[12];                           /* ���б⿵������(12)              */
//    char    bford[12];                            /* ���б�������(12)              */
//    char    bfnet[12];                            /* ���б������(12)                */
//    char    bfeps[9];                             /* ���б�EPS(9)                    */
//    char    bfname2[10];                          /* �����б��(10)                  */
//    char    bfsales2[12];                         /* �����б�����(12)              */
//    char    bfoper2[12];                          /* �����б⿵������(12)            */
//    char    bford2[12];                           /* �����б�������(12)            */
//    char    bfnet2[12];                           /* �����б������(12)              */
//    char    bfeps2[9];                            /* �����б�EPS(9)                  */
//    char    sales_rate[9];                        /* �����������(9)                 */
//    char    oper_rate[9];                         /* ��������������(9)               */
//    char    ord_rate[9];                          /* �������������(9)               */
//    char    net_rate[9];                          /* ������������(9)                 */
//    char    eps_rate[9];                          /* EPS������(9)                    */
//    char    hotime[6];                            /* ȣ�����Žð�(6)                 */
//    char    offerho[8];                           /* �켱�ŵ�ȣ��(8)                 */
//    char    bidho[8];                             /* �켱�ż�ȣ��(8)                 */
//    char    offerho1[8];                          /* �ŵ�ȣ��1(8)                    */
//    char    offerhorate1[7];                      /* �ŵ�ȣ�������1(7)              */
//    char    offerrem1[8];                         /* �ŵ�ȣ������1(8)                */
//    char    preoffercha1[8];                      /* �ŵ�����������1(8)            */
//    char    offerho2[8];                          /* �ŵ�ȣ��2(8)                    */
//    char    offerhorate2[7];                      /* �ŵ�ȣ�������2(7)              */
//    char    offerrem2[8];                         /* �ŵ�ȣ������2(8)                */
//    char    preoffercha2[8];                      /* �ŵ�����������2(8)            */
//    char    offerho3[8];                          /* �ŵ�ȣ��3(8)                    */
//    char    offerhorate3[7];                      /* �ŵ�ȣ�������3(7)              */
//    char    offerrem3[8];                         /* �ŵ�ȣ������3(8)                */
//    char    preoffercha3[8];                      /* �ŵ�����������3(8)            */
//    char    offerho4[8];                          /* �ŵ�ȣ��4(8)                    */
//    char    offerhorate4[7];                      /* �ŵ�ȣ�������4(7)              */
//    char    offerrem4[8];                         /* �ŵ�ȣ������4(8)                */
//    char    preoffercha4[8];                      /* �ŵ�����������4(8)            */
//    char    offerho5[8];                          /* �ŵ�ȣ��5(8)                    */
//    char    offerhorate5[7];                      /* �ŵ�ȣ�������5(7)              */
//    char    offerrem5[8];                         /* �ŵ�ȣ������5(8)                */
//    char    preoffercha5[8];                      /* �ŵ�����������5(8)            */
//    char    offerho6[8];                          /* �ŵ�ȣ��6(8)                    */
//    char    offerhorate6[7];                      /* �ŵ�ȣ�������6(7)              */
//    char    offerrem6[8];                         /* �ŵ�ȣ������6(8)                */
//    char    preoffercha6[8];                      /* �ŵ�����������6(8)            */
//    char    offerho7[8];                          /* �ŵ�ȣ��7(8)                    */
//    char    offerhorate7[7];                      /* �ŵ�ȣ�������7(7)              */
//    char    offerrem7[8];                         /* �ŵ�ȣ������7(8)                */
//    char    preoffercha7[8];                      /* �ŵ�����������7(8)            */
//    char    offerho8[8];                          /* �ŵ�ȣ��8(8)                    */
//    char    offerhorate8[7];                      /* �ŵ�ȣ�������8(7)              */
//    char    offerrem8[8];                         /* �ŵ�ȣ������8(8)                */
//    char    preoffercha8[8];                      /* �ŵ�����������8(8)            */
//    char    offerho9[8];                          /* �ŵ�ȣ��9(8)                    */
//    char    offerhorate9[7];                      /* �ŵ�ȣ�������9(7)              */
//    char    offerrem9[8];                         /* �ŵ�ȣ������9(8)                */
//    char    preoffercha9[8];                      /* �ŵ�����������9(8)            */
//    char    offerho10[8];                         /* �ŵ�ȣ��10(8)                   */
//    char    offerhorate10[7];                     /* �ŵ�ȣ�������10(7)             */
//    char    offerrem10[8];                        /* �ŵ�ȣ������10(8)               */
//    char    preoffercha10[8];                     /* �ŵ�����������10(8)           */
//    char    bidho1[8];                            /* �ż�ȣ��1(8)                    */
//    char    bidhorate1[7];                        /* �ż�ȣ�������1(7)              */
//    char    bidrem1[8];                           /* �ż�ȣ������1(8)                */
//    char    prebidcha1[8];                        /* �ż�����������1(8)            */
//    char    bidho2[8];                            /* �ż�ȣ��2(8)                    */
//    char    bidhorate2[7];                        /* �ż�ȣ�������2(7)              */
//    char    bidrem2[8];                           /* �ż�ȣ������2(8)                */
//    char    prebidcha2[8];                        /* �ż�����������2(8)            */
//    char    bidho3[8];                            /* �ż�ȣ��3(8)                    */
//    char    bidhorate3[7];                        /* �ż�ȣ�������3(7)              */
//    char    bidrem3[8];                           /* �ż�ȣ������3(8)                */
//    char    prebidcha3[8];                        /* �ż�����������3(8)            */
//    char    bidho4[8];                            /* �ż�ȣ��4(8)                    */
//    char    bidhorate4[7];                        /* �ż�ȣ�������4(7)              */
//    char    bidrem4[8];                           /* �ż�ȣ������4(8)                */
//    char    prebidcha4[8];                        /* �ż�����������4(8)            */
//    char    bidho5[8];                            /* �ż�ȣ��5(8)                    */
//    char    bidhorate5[7];                        /* �ż�ȣ�������5(7)              */
//    char    bidrem5[8];                           /* �ż�ȣ������5(8)                */
//    char    prebidcha5[8];                        /* �ż�����������5(8)            */
//    char    bidho6[8];                            /* �ż�ȣ��6(8)                    */
//    char    bidhorate6[7];                        /* �ż�ȣ�������6(7)              */
//    char    bidrem6[8];                           /* �ż�ȣ������6(8)                */
//    char    prebidcha6[8];                        /* �ż�����������6(8)            */
//    char    bidho7[8];                            /* �ż�ȣ��7(8)                    */
//    char    bidhorate7[7];                        /* �ż�ȣ�������7(7)              */
//    char    bidrem7[8];                           /* �ż�ȣ������7(8)                */
//    char    prebidcha7[8];                        /* �ż�����������7(8)            */
//    char    bidho8[8];                            /* �ż�ȣ��8(8)                    */
//    char    bidhorate8[7];                        /* �ż�ȣ�������8(7)              */
//    char    bidrem8[8];                           /* �ż�ȣ������8(8)                */
//    char    prebidcha8[8];                        /* �ż�����������8(8)            */
//    char    bidho9[8];                            /* �ż�ȣ��9(8)                    */
//    char    bidhorate9[7];                        /* �ż�ȣ�������9(7)              */
//    char    bidrem9[8];                           /* �ż�ȣ������9(8)                */
//    char    prebidcha9[8];                        /* �ż�����������9(8)            */
//    char    bidho10[7];                           /* �ż�ȣ��10(7)                   */
//    char    bidhorate10[8];                       /* �ż�ȣ�������10(8)             */
//    char    bidrem10[8];                          /* �ż�ȣ������10(8)               */
//    char    prebidcha10[8];                       /* �ż�����������10(8)           */
//    char    totofferrem[8];                       /* �ŵ�ȣ���Ѽ���(8)               */
//    char    preoffercha[8];                       /* �ŵ�ȣ���Ѽ����������(8)       */
//    char    totbidrem[8];                         /* �ż�ȣ���Ѽ���(8)               */
//    char    prebidcha[8];                         /* �ż�ȣ���Ѽ����������(8)       */
//    char    netbidrem[8];                         /* ���ż��Ѽ���(8)                 */
//    char    tmofferrem[8];                        /* �ð��ܸŵ��ܷ�(8)               */
//    char    pretmoffercha[8];                     /* �ð��ܸŵ������������(8)       */
//    char    tmbidrem[8];                          /* �ð��ܸż��ܷ�(8)               */
//    char    pretmbidcha[8];                       /* �ð��ܸż������������(8)       */
//    char    yofferho0[8];                         /* ����ŵ�ȣ��(8)                 */
//    char    yofferrem0[8];                        /* ����ŵ�ȣ������(8)             */
//    char    ypreoffercha0[8];                     /* ���������ŵ�������(8)         */
//    char    yofferrem[8];                         /* ����ŵ�ȣ���ܷ���(8)           */
//    char    ybidho0[8];                           /* ����ż�ȣ��(8)                 */
//    char    ybidrem0[8];                          /* ����ż�ȣ������(8)             */
//    char    yprebidcha0[8];                       /* ���������ż�������(8)         */
//    char    ybidrem[8];                           /* ����ż�ȣ���ܷ���(8)           */
//    char    ychetime[6];                          /* ����ü��ð�(6)                 */
//    char    yeprice[8];                           /* ����ü�ᰡ��(8)                 */
//    char    yevolume[8];                          /* ����ü�����(8)                 */
//    char    preychange[8];                        /* ����ü�ᰡ�������簡���(8)     */
//    char    preysign[1];                          /* ����ü�ᰡ�������簡��񱸺�(1) */
//    char    jnilychange[8];                       /* ����ü�ᰡ�����������(8)       */
//    char    jnilysign[1];                         /* ����ü�ᰡ����������񱸺�(1)   */
//    char    yecgubun[1];                          /* ����ü�ᱸ��(1)                 */
//    char    yecvolume[8];                         /* ����ü�ắ����(8)               */
//    char    ymdvolume[8];                         /* ����ŵ�ü�����(8)             */
//    char    ymsvolume[8];                         /* ����ż�ü�����(8)             */
//    char    dan_jnilclose[8];                     /* ���ϴ��ϰ��Ÿ�����(8)           */
//    char    dan_jnilvolume[12];                   /* ���ϴ��ϰ��ŸŰŷ���(12)        */
//    char    dan_jnilvalue[15];                    /* ���ϴ��ϰ��ŸŰŷ����(15)      */
//    char    dan_uplmtprice[8];                    /* �ÿܻ��Ѱ���(8)                 */
//    char    dan_dnlmtprice[8];                    /* �ÿ����Ѱ���(8)                 */
//    char    dan_preclose[8];                      /* �ÿܴ�������(8)                 */
//    char    dan_quotesgubun[1];                   /* �ÿܱ⼼����(1)                 */
//    char    dan_chetime[6];                       /* �ÿ�ü��ð�(6)                 */
//    char    dan_price[8];                         /* �ÿ����簡(8)                   */
//    char    dan_opentime[6];                      /* �ÿܽð��ð�(6)                 */
//    char    dan_hightime[6];                      /* �ÿܰ��ð�(6)                 */
//    char    dan_lowtime[6];                       /* �ÿ������ð�(6)                 */
//    char    dan_open[8];                          /* �ÿܽð�(8)                     */
//    char    dan_high[8];                          /* �ÿܰ�(8)                     */
//    char    dan_low[8];                           /* �ÿ�����(8)                     */
//    char    dan_sign[1];                          /* �ÿܻ�/���Ѻ�ȣ(1)              */
//    char    dan_change[8];                        /* �ÿܴ����������(8)             */
//    char    dan_volume[12];                       /* �ÿܸŸŴ����ŷ���(12)          */
//    char    dan_value[15];                        /* �ÿܸŸŴ����ŷ����(15)        */
//    char    dan_cvolume[8];                       /* �ÿ�ü�����(8)                 */
//    char    dan_cgubun[1];                        /* �ÿ�ü�ᱸ��(1)                 */
//    char    dan_mdvolume[8];                      /* �ÿܸŵ�����ü�ᷮ(8)           */
//    char    dan_msvolume[8];                      /* �ÿܸż�����ü�ᷮ(8)           */
//    char    dan_mdchecnt[8];                      /* �ÿܸŵ�ü��Ǽ�(8)             */
//    char    dan_mschecnt[8];                      /* �ÿܸż�ü��Ǽ�(8)             */
//    char    dan_prevolume[8];                     /* �ÿ������ŷ���(8)               */
//    char    dan_precvolume[8];                    /* �ÿ�����ü�����(8)             */
//    char    dan_hotime[6];                        /* �ÿܼ��Žð�(6)                 */
//    char    dan_offerho1[8];                      /* �ÿܸŵ�ȣ��1(8)                */
//    char    dan_offerrem1[8];                     /* �ÿܸŵ�ȣ������1(8)            */
//    char    dan_preoffercha1[8];                  /* �ÿܸŵ�����������1(8)        */
//    char    dan_offerho2[8];                      /* �ÿܸŵ�ȣ��2(8)                */
//    char    dan_offerrem2[8];                     /* �ÿܸŵ�ȣ������2(8)            */
//    char    dan_preoffercha2[8];                  /* �ÿܸŵ�����������2(8)        */
//    char    dan_offerho3[8];                      /* �ÿܸŵ�ȣ��3(8)                */
//    char    dan_offerrem3[8];                     /* �ÿܸŵ�ȣ������3(8)            */
//    char    dan_preoffercha3[8];                  /* �ÿܸŵ�����������3(8)        */
//    char    dan_offerho4[8];                      /* �ÿܸŵ�ȣ��4(8)                */
//    char    dan_offerrem4[8];                     /* �ÿܸŵ�ȣ������4(8)            */
//    char    dan_preoffercha4[8];                  /* �ÿܸŵ�����������4(8)        */
//    char    dan_offerho5[8];                      /* �ÿܸŵ�ȣ��5(8)                */
//    char    dan_offerrem5[8];                     /* �ÿܸŵ�ȣ������5(8)            */
//    char    dan_preoffercha5[8];                  /* �ÿܸŵ�����������5(8)        */
//    char    dan_bidho1[8];                        /* �ÿܸż�ȣ��1(8)                */
//    char    dan_bidrem1[8];                       /* �ÿܸż�ȣ������1(8)            */
//    char    dan_prebidcha1[8];                    /* �ÿܸż�����������1(8)        */
//    char    dan_bidho2[8];                        /* �ÿܸż�ȣ��2(8)                */
//    char    dan_bidrem2[8];                       /* �ÿܸż�ȣ������2(8)            */
//    char    dan_prebidcha2[8];                    /* �ÿܸż�����������2(8)        */
//    char    dan_bidho3[8];                        /* �ÿܸż�ȣ��3(8)                */
//    char    dan_bidrem3[8];                       /* �ÿܸż�ȣ������3(8)            */
//    char    dan_prebidcha3[8];                    /* �ÿܸż�����������3(8)        */
//    char    dan_bidho4[8];                        /* �ÿܸż�ȣ��4(8)                */
//    char    dan_bidrem4[8];                       /* �ÿܸż�ȣ������4(8)            */
//    char    dan_prebidcha4[8];                    /* �ÿܸż�����������4(8)        */
//    char    dan_bidho5[8];                        /* �ÿܸż�ȣ��5(8)                */
//    char    dan_bidrem5[8];                       /* �ÿܸż�ȣ������5(8)            */
//    char    dan_prebidcha5[8];                    /* �ÿܸż�����������5(8)        */
//    char    dan_totofferrem[8];                   /* �ÿܸŵ�ȣ���Ѽ���(8)           */
//    char    dan_preoffercha[8];                   /* �ÿܸŵ�ȣ�����������(8)       */
//    char    dan_totbidrem[8];                     /* �ÿܸż�ȣ���Ѽ���(8)           */
//    char    dan_prebidcha[8];                     /* �ÿܸż�ȣ�����������(8)       */
//    char    dan_ychetime[6];                      /* �ÿܿ���ü��ð�(6)             */
//    char    dan_yeprice[8];                       /* �ÿܿ���ü�ᰡ��(8)             */
//    char    dan_yevolume[8];                      /* �ÿܿ���ü�����(8)             */
//    char    dan_preychange[8];                    /* �ÿܿ����������簡���(8)     */
//    char    dan_preysign[1];                      /* �ÿܿ����������簡��񱸺�(1) */
//    char    dan_jnilychange[8];                   /* �ÿܿ��󰡴����������(8)       */
//    char    dan_jnilysign[1];                     /* �ÿܿ��󰡴���������񱸺�(1)   */
//    char    offerno1[3];                          /* �ŵ����ǻ��ڵ�1(3)              */
//    char    bidno1[3];                            /* �ż����ǻ��ڵ�1(3)              */
//    char    tradmdvol1[8];                        /* �Ѹŵ�����1(8)                  */
//    char    tradmsvol1[8];                        /* �Ѹż�����1(8)                  */
//    char    tradmdcha1[8];                        /* �ŵ��ŷ����������1(8)          */
//    char    tradmscha1[8];                        /* �ż��ŷ����������1(8)          */
//    char    offerno2[3];                          /* �ŵ����ǻ��ڵ�2(3)              */
//    char    bidno2[3];                            /* �ż����ǻ��ڵ�2(3)              */
//    char    tradmdvol2[8];                        /* �Ѹŵ�����2(8)                  */
//    char    tradmsvol2[8];                        /* �Ѹż�����2(8)                  */
//    char    tradmdcha2[8];                        /* �ŵ��ŷ����������2(8)          */
//    char    tradmscha2[8];                        /* �ż��ŷ����������2(8)          */
//    char    offerno3[3];                          /* �ŵ����ǻ��ڵ�3(3)              */
//    char    bidno3[3];                            /* �ż����ǻ��ڵ�3(3)              */
//    char    tradmdvol3[8];                        /* �Ѹŵ�����3(8)                  */
//    char    tradmsvol3[8];                        /* �Ѹż�����3(8)                  */
//    char    tradmdcha3[8];                        /* �ŵ��ŷ����������3(8)          */
//    char    tradmscha3[8];                        /* �ż��ŷ����������3(8)          */
//    char    offerno4[3];                          /* �ŵ����ǻ��ڵ�4(3)              */
//    char    bidno4[3];                            /* �ż����ǻ��ڵ�4(3)              */
//    char    tradmdvol4[8];                        /* �Ѹŵ�����4(8)                  */
//    char    tradmsvol4[8];                        /* �Ѹż�����4(8)                  */
//    char    tradmdcha4[8];                        /* �ŵ��ŷ����������4(8)          */
//    char    tradmscha4[8];                        /* �ż��ŷ����������4(8)          */
//    char    offerno5[3];                          /* �ŵ����ǻ��ڵ�5(3)              */
//    char    bidno5[3];                            /* �ż����ǻ��ڵ�5(3)              */
//    char    tradmdvol5[8];                        /* �Ѹŵ�����5(8)                  */
//    char    tradmsvol5[8];                        /* �Ѹż�����5(8)                  */
//    char    tradmdcha5[8];                        /* �ŵ��ŷ����������5(8)          */
//    char    tradmscha5[8];                        /* �ż��ŷ����������5(8)          */
//    char    ftradmdvol[8];                        /* �ܱ������ǻ�ŵ��հ�(8)         */
//    char    ftradmsvol[8];                        /* �ܱ������ǻ�ż��հ�(8)         */
//    char    ftradmdcha[8];                        /* �ܱ������ǻ�ŵ��������(8)     */
//    char    ftradmscha[8];                        /* �ܱ������ǻ�ż��������(8)     */
//    char    ftradnetvol[8];                       /* �ܱ������ǻ���ż��հ�(8)       */
//    char    mmsdate[8];                           /* �����ŸŰ�����(8)               */
//    char    mmedate[8];                           /* �����Ÿ�������(8)               */
//    char    memedan[5];                           /* �Ÿż�������(5)                 */
//    char    etfgu[2];                             /* ETF�з�(2)                      */
//    char    etfupl[2];                            /* ETF��������������(2)            */
//    char    etfupm[3];                            /* ETF��������������(3)            */
//    char    etfups[3];                            /* ETF��������������(3)            */
//    char    etfcu[8];                             /* ETFCU����(8)                    */
//    char    etfnum[4];                            /* ETF���������(4)                */
//    char    etftotcap[15];                        /* ETF���ڻ��Ѿ�(15)               */
//    char    etfdiv[2];                            /* ETF����ֱ�(2)                  */
//    char    etfratio[9];                          /* ETF��������������(9)          */
//    char    prenav[9];                            /* ETF����NAV(9)                   */
//    char    etfcurstokcnt[10];                    /* ETF�����ֽļ�(10)               */
//    char    etftotcap_f[15];                      /* ETF���ڻ��Ѿ�(15)               */
//    char    etfcurtotcap_f[15];                   /* ETF������ڻ��Ѿ�(15)           */
//    char    prenav_f[9];                          /* ETF����NAV(9)                   */
//    char    gmprice[6];                           /* �ֱٿ������簡(6)               */
//    char    gmsign[1];                            /* �ֱٿ������ϴ�񱸺�(1)         */
//    char    gmchange[6];                          /* �ֱٿ������ϴ��(6)             */
//    char    gmrate[6];                            /* �ֱٿ��������(6)               */
//    char    gmgrdegree[7];                        /* �ֱٿ���������(7)               */
//    char    gmgrrate[7];                          /* �ֱٿ���������(7)               */
//    char    gmgrratesign[1];                      /* �ֱٿ�����������ȣ(1)           */
//    char    navprice[7];                          /* NAV���簡(7)                    */
//    char    navsign[1];                           /* NAV���ϴ�񱸺�(1)              */
//    char    navchange[6];                         /* NAV���ϴ��(6)                  */
//    char    navrate[6];                           /* NAV�����(6)                    */
//    char    navgrdegree[7];                       /* NAV������(7)                    */
//    char    navgrrate[7];                         /* NAV������(7)                    */
//    char    navgrratesign[1];                     /* NAV��������ȣ(1)                */
//    char    kospijisu[6];                         /* KOSPI200����(6)                 */
//    char    kospisign[1];                         /* KOSPI200���ϴ�񱸺�(1)         */
//    char    kospichange[6];                       /* KOSPI200���ϴ��(6)             */
//    char    kospirate[6];                         /* KOSPI200�����(6)               */
//    char    kospigrdegree[7];                     /* KOSPI200������(7)               */
//    char    kospigrrate[7];                       /* KOSPI200������(7)               */
//    char    kospigrratesign[1];                   /* KOSPI200��������ȣ(1)           */
//    char    issueprice[8];                        /* ���డ(8)                       */
//    char    memnum1[3];                           /* ELWLPȸ����ȣ1(3)               */
//    char    memnum2[3];                           /* ELWLPȸ����ȣ2(3)               */
//    char    memnum3[3];                           /* ELWLPȸ����ȣ3(3)               */
//    char    memnum4[3];                           /* ELWLPȸ����ȣ4(3)               */
//    char    memnum5[3];                           /* ELWLPȸ����ȣ5(3)               */
//    char    memnum6[3];                           /* ELWLPȸ����ȣ6(3)               */
//    char    memnum7[3];                           /* ELWLPȸ����ȣ7(3)               */
//    char    memnum8[3];                           /* ELWLPȸ����ȣ8(3)               */
//    char    memnum9[3];                           /* ELWLPȸ����ȣ9(3)               */
//    char    memnum10[3];                          /* ELWLPȸ����ȣ10(3)              */
//    char    espread[5];                           /* ELW��������(5)                  */
//    char    issuernmk[20];                        /* ELW�ѱ۹�������(20)           */
//    char    issuernme[20];                        /* ELW������������(20)           */
//    char    issuercd[12];                         /* ELW�������ڵ�(12)             */
//    char    item1[12];                            /* ELW�����ڻ�1(12)                */
//    char    item2[12];                            /* ELW�����ڻ�2(12)                */
//    char    item3[12];                            /* ELW�����ڻ�3(12)                */
//    char    item4[12];                            /* ELW�����ڻ�4(12)                */
//    char    item5[12];                            /* ELW�����ڻ�5(12)                */
//    char    comprate1[5];                         /* ELW�����ڻ걸������1(5)         */
//    char    comprate2[5];                         /* ELW�����ڻ걸������2(5)         */
//    char    comprate3[5];                         /* ELW�����ڻ걸������3(5)         */
//    char    comprate4[5];                         /* ELW�����ڻ걸������4(5)         */
//    char    comprate5[5];                         /* ELW�����ڻ걸������5(5)         */
//    char    elwbase[10];                          /* ELW���ʰ���(10)                 */
//    char    elwexec[10];                          /* ELW��簡��(10)                 */
//    char    elwopt[2];                            /* ELW�Ǹ�����(2)                  */
//    char    elwtype[2];                           /* ELW�Ǹ������(2)              */
//    char    elwdetail[100];                       /* ELW�Ǹ�����(100)                */
//    char    settletype[2];                        /* ELW�������(2)                  */
//    char    lastdate[8];                          /* ELW�����ŷ���(8)                */
//    char    convrate[12];                         /* ELW��ȯ����(12)                 */
//    char    riserate[5];                          /* ELW���ݻ��������(5)            */
//    char    compensate[5];                        /* ELW������(5)                    */
//    char    payday[8];                            /* ELW������(8)                    */
//    char    rept[20];                             /* ELW���޴븮��(20)               */
//    char    valuation[100];                       /* ELW�����򰡰��ݹ��(100)        */
//    char    invidx[2];                            /* ELW�Ǹ�����(2)                  */
//    char    lp_holdvol[13];                       /* ELWLP��������(13)               */
//    char    lpord[2];                             /* LP�ֹ����ɿ���(2)               */
//    char    lp_multi[3];                          /* ȣ���������(3)                 */
//    char    krx_autos[1];                         /* KRX_Autos(1)                    */
//    char    krx_semicon[1];                       /* KRX_Semicon(1)                  */
//    char    krx_health[1];                        /* KRX_HealthCare(1)               */
//    char    krx_banks[1];                         /* KRX_Banks(1)                    */
//    char    krx_it[1];                            /* KRX_IT(1)                       */
//    char    krx_energychem[1];                    /* KRX_Energy&Chemicals(1)         */
//    char    krx_steels[1];                        /* KRX_Steels(1)                   */
//    char    krx_consumer[1];                      /* KRX_Consumer_Staples(1)         */
//    char    krx_mediatel[1];                      /* KRX_Media&Telecom(1)            */
//    char    krx_construct[1];                     /* KRX_Constructions(1)            */
//    char    krx_finance[1];                       /* KRX_Financisals(1)              */
//    char    cvolume[8];                           /* ü�����(8)                     */
//    char    cgubun[1];                            /* ü�ᱸ��(1)                     */
//    char    cmmgubun[2];                          /* �Ÿű���(2)                     */
//    char    mdvolume[8];                          /* �ŵ�����ü�ᷮ(8)               */
//    char    msvolume[8];                          /* �ż�����ü�ᷮ(8)               */
//    char    mdchecnt[8];                          /* �ŵ�����ü��Ǽ�(8)             */
//    char    mschecnt[8];                          /* �ż�����ü��Ǽ�(8)             */
//    char    prevolume[8];                         /* �����ŷ���(8)                   */
//    char    precvolume[8];                        /* ����ü�����(8)                 */
//    char    detour_gu[2];                         /* ��ȸ���屸��(2)                 */
//    char    det_reason[2];                        /* ��ȸ�������(2)                 */
//    char    det_st_date[8];                       /* ��ȸ���尳����(8)               */
//    char    det_end_date[8];                      /* ��ȸ����������(8)               */
//    char    stop_reason[2];                       /* �ŷ���������(2)                 */
//    char    divdate1[4];                          /* ��������1(4)                  */
//    char    divdate2[4];                          /* ��������2(4)                  */
//    char    divdate3[4];                          /* ��������3(4)                  */
//    char    divdate4[4];                          /* ��������4(4)                  */
//    char    confirmpay[8];                        /* Ȯ�����޾�(8)                   */
//    char    assetbasemoney[3];                    /* �ڻ������ȭ(3)                 */
//    char    high1[8];                             /* �����ְ�(8)                   */
//    char    low1[8];                              /* ����������(8)                   */
//    char    high5date[8];                         /* 5���ְ�����(8)                */
//    char    high5[8];                             /* 5���ְ�(8)                    */
//    char    low5date[8];                          /* 5������������(8)                */
//    char    low5[8];                              /* 5��������(8)                    */
//    char    high10date[8];                        /* 10���ְ�����(8)               */
//    char    high10[8];                            /* 10���ְ�(8)                   */
//    char    low10date[8];                         /* 10������������(8)               */
//    char    low10[8];                             /* 10��������(8)                   */
//    char    high20date[8];                        /* 20���ְ�����(8)               */
//    char    high20[8];                            /* 20���ְ�(8)                   */
//    char    low20date[8];                         /* 20������������(8)               */
//    char    low20[8];                             /* 20��������(8)                   */
//    char    high60date[8];                        /* 60���ְ�����(8)               */
//    char    high60[8];                            /* 60���ְ�(8)                   */
//    char    low60date[8];                         /* 60������������(8)               */
//    char    low60[8];                             /* 60��������(8)                   */
//    char    high90date[8];                        /* 90���ְ�����(8)               */
//    char    high90[8];                            /* 90���ְ�(8)                   */
//    char    low90date[8];                         /* 90������������(8)               */
//    char    low90[8];                             /* 90��������(8)                   */
//    char    highyeardate[8];                      /* �����ְ�����(8)               */
//    char    highyear[8];                          /* �����ְ�(8)                   */
//    char    highyear_rate[7];                     /* �����ְ������(7)             */
//    char    lowyeardate[8];                       /* ��������������(8)               */
//    char    lowyear[8];                           /* ����������(8)                   */
//    char    lowyear_rate[7];                      /* ���������������(7)             */
//    char    hvolyeardate[8];                      /* �����ְ�ŷ�������(8)           */
//    char    hvolyear[12];                         /* �����ְ�ŷ���(12)              */
//    char    lvolyeardate[8];                      /* ���������ŷ�������(8)           */
//    char    lvolyear[12];                         /* ���������ŷ���(12)              */
//    char    uplmtdaycnt[8];                       /* ���ӻ����ϼ�(8)                 */
//    char    updaycnt[8];                          /* ���ӻ���ϼ�(8)                 */
//    char    dnlmtdaycnt[8];                       /* ���������ϼ�(8)                 */
//    char    dndaycnt[8];                          /* �����϶��ϼ�(8)                 */
//    char    bhdaycnt[8];                          /* ���Ӻ����ϼ�(8)                 */
//    char    upday10cnt[8];                        /* �ֱ�10�ϰ�����ϼ�(8)           */
//    char    upday20cnt[8];                        /* �ֱ�20�ϰ�����ϼ�(8)           */
//    char    uplmtitime[6];                        /* ���Ѱ��������Խð�(6)           */
//    char    uplmtgubun[1];                        /* ������������(1)                 */
//    char    uplmtstime[6];                        /* ���Ѱ��������Խð�(6)           */
//    char    uplmtetime[6];                        /* ���Ѱ�������Ż�ð�(6)           */
//    char    uplmtlow[8];                          /* ���Ѱ�����������(8)             */
//    char    dnlmtitime[6];                        /* ���Ѱ��������Խð�(6)           */
//    char    dnlmtgubun[1];                        /* ������������(1)                 */
//    char    dnlmtstime[6];                        /* ���Ѱ��������Խð�(6)           */
//    char    dnlmtetime[6];                        /* ���Ѱ�������Ż�ð�(6)           */
//    char    dnlmthigh[8];                         /* ���Ѱ������ְ�(8)             */
//    char    upneargubun[1];                       /* ���ѱ�����������(1)             */
//    char    upnearstime[6];                       /* ���Ѱ��������Խð�(6)           */
//    char    upnearetime[6];                       /* ���Ѱ�������Ż�ð�(6)           */
//    char    dnneargubun[1];                       /* ���ѱ�����������(1)             */
//    char    dnnearstime[6];                       /* ���Ѱ��������Խð�(6)           */
//    char    dnnearetime[6];                       /* ���Ѱ�������Ż�ð�(6)           */
//    char    mdvolume_c[8];                        /* �ŵ�ü�����(8)                 */
//    char    msvolume_c[8];                        /* �ż�ü�����(8)                 */
//    char    mdvolume_h[8];                        /* �ŵ�ü�����(8)                 */
//    char    msvolume_h[8];                        /* �ż�ü�����(8)                 */
//    char    msvolume_d[8];                        /* �뷮�ż�ü�����(8)             */
//    char    mdvolume_d[8];                        /* �뷮�ŵ�ü�����(8)             */
//    char    mschecnt_d[8];                        /* �뷮�ż�ü��Ǽ�(8)             */
//    char    mdchecnt_d[8];                        /* �뷮�ŵ�ü��Ǽ�(8)             */
//    char    status[2];                            /* �屸��(2)                       */
//    char    yeuplmt[8];                           /* ���Ͽ�����Ѱ�(8)               */
//    char    yednlmt[8];                           /* ���Ͽ������Ѱ�(8)               */
//}KB_s0001_OutRec1;

typedef struct _KB_s0001_OutRec1 {
    char    shcode[6];                            /* �����ڵ�(6)                     */
    char    recprice[8];                          /* ���ذ�(8)                       */
    char    yeprice[8];                           /* ����ü�ᰡ��(8)                 */
    char    yevolume[8];                          /* ����ü�����(8)                 */
    char    hname[40];                            /* �����(40)                      */
    char    price[8];                             /* ���簡(8)                       */
    char    sign[1];                              /* ���簡���ϴ�񱸺�(1)           */
    char    change[8];                            /* ���簡���ϴ��(8)               */
    char    rate[7];                              /* ���簡���ϴ������(7)         */
    char    volume[12];                           /* �ŷ���(12)                      */
    char    value[15];                            /* �ŷ����(15)                    */
    char    offerho1[8];                          /* �ŵ�ȣ��1(8)                    */
    char    offerho2[8];                          /* �ŵ�ȣ��2(8)                    */
    char    offerho3[8];                          /* �ŵ�ȣ��3(8)                    */
    char    offerho4[8];                          /* �ŵ�ȣ��4(8)                    */
    char    offerho5[8];                          /* �ŵ�ȣ��5(8)                    */
    char    offerrem1[8];                         /* �ŵ�ȣ������1(8)                */
    char    offerrem2[8];                         /* �ŵ�ȣ������2(8)                */
    char    offerrem3[8];                         /* �ŵ�ȣ������3(8)                */
    char    offerrem4[8];                         /* �ŵ�ȣ������4(8)                */
    char    offerrem5[8];                         /* �ŵ�ȣ������5(8)                */
    char    bidho1[8];                            /* �ż�ȣ��1(8)                    */
    char    bidho2[8];                            /* �ż�ȣ��2(8)                    */
    char    bidho3[8];                            /* �ż�ȣ��3(8)                    */
    char    bidho4[8];                            /* �ż�ȣ��4(8)                    */
    char    bidho5[8];                            /* �ż�ȣ��5(8)                    */
    char    bidrem1[8];                           /* �ż�ȣ������1(8)                */
    char    bidrem2[8];                           /* �ż�ȣ������2(8)                */
    char    bidrem3[8];                           /* �ż�ȣ������3(8)                */
    char    bidrem4[8];                           /* �ż�ȣ������4(8)                */
    char    bidrem5[8];                           /* �ż�ȣ������5(8)                */
    char    totofferrem[8];                       /* �ŵ�ȣ���Ѽ���(8)               */
    char    totbidrem[8];                         /* �ż�ȣ���Ѽ���(8)               */
    char    tmofferrem[8];                        /* �ð��ܸŵ��ܷ�(8)               */
    char    tmbidrem[8];                          /* �ð��ܸż��ܷ�(8)               */
    char    uplmtprice[8];                        /* ���Ѱ�(8)                       */
    char    dnlmtprice[8];                        /* ���Ѱ�(8)                       */
    char    preychange[8];                        /* ����ü�ᰡ�������簡���(8)     */
    char    high[8];                              /* ��(8)                         */
    char    low[8];                               /* ����(8)                         */
    char    jnilvolume[12];                       /* ���ϰŷ���(12)                  */
    char    listing[12];                          /* �����ֽļ�(12)                  */
} KB_s0001_OutRec1;

/*************************************************************************************/
class CKB_s0001
{
public: //Variable
    CString		m_szshcode;							/* �����ڵ�(6)                     */
    CString		m_szrecprice;						/* ���ذ�(8)                       */
    CString		m_szyeprice;						/* ����ü�ᰡ��(8)                 */
    CString		m_szyevolume;						/* ����ü�����(8)                 */
    CString		m_szhname;							/* �����(40)                      */
    CString		m_szprice;							/* ���簡(8)                       */
    CString		m_szsign;							/* ���簡���ϴ�񱸺�(1)           */
    CString		m_szchange;							/* ���簡���ϴ��(8)               */
    CString		m_szrate;							/* ���簡���ϴ������(7)         */
    CString		m_szvolume;							/* �ŷ���(12)                      */
    CString		m_szvalue;							/* �ŷ����(15)                    */
    CString		m_szofferho1;						/* �ŵ�ȣ��1(8)                    */
    CString		m_szofferho2;						/* �ŵ�ȣ��2(8)                    */
    CString		m_szofferho3;						/* �ŵ�ȣ��3(8)                    */
    CString		m_szofferho4;						/* �ŵ�ȣ��4(8)                    */
    CString		m_szofferho5;						/* �ŵ�ȣ��5(8)                    */
    CString		m_szofferrem1;						/* �ŵ�ȣ������1(8)                */
    CString		m_szofferrem2;						/* �ŵ�ȣ������2(8)                */
    CString		m_szofferrem3;						/* �ŵ�ȣ������3(8)                */
    CString		m_szofferrem4;						/* �ŵ�ȣ������4(8)                */
    CString		m_szofferrem5;						/* �ŵ�ȣ������5(8)                */
    CString		m_szbidho1;							/* �ż�ȣ��1(8)                    */
    CString		m_szbidho2;							/* �ż�ȣ��2(8)                    */
    CString		m_szbidho3;							/* �ż�ȣ��3(8)                    */
    CString		m_szbidho4;							/* �ż�ȣ��4(8)                    */
    CString		m_szbidho5;							/* �ż�ȣ��5(8)                    */
    CString		m_szbidrem1;						/* �ż�ȣ������1(8)                */
    CString		m_szbidrem2;						/* �ż�ȣ������2(8)                */
    CString		m_szbidrem3;						/* �ż�ȣ������3(8)                */
    CString		m_szbidrem4;						/* �ż�ȣ������4(8)                */
    CString		m_szbidrem5;						/* �ż�ȣ������5(8)                */
    CString		m_sztotofferrem;					/* �ŵ�ȣ���Ѽ���(8)               */
    CString		m_sztotbidrem;						/* �ż�ȣ���Ѽ���(8)               */
    CString		m_sztmofferrem;						/* �ð��ܸŵ��ܷ�(8)               */
    CString		m_sztmbidrem;						/* �ð��ܸż��ܷ�(8)               */
    CString		m_szuplmtprice;						/* ���Ѱ�(8)                       */
    CString		m_szdnlmtprice;						/* ���Ѱ�(8)                       */
    CString		m_szpreychange;						/* ����ü�ᰡ�������簡���(8)     */
    CString		m_szhigh;							/* ��(8)                         */
    CString		m_szlow;							/* ����(8)                         */
    CString		m_szjnilvolume;						/* ���ϰŷ���(12)                  */
    CString		m_szlisting;						/* �����ֽļ�(12)                  */

public:
	CKB_s0001()
	{
	}

	virtual void  Convert(KB_s0001_OutRec1* pInput)
	{
		CString sTemp1 ;
		char sTmp[50+1];

		DATA_MEMCOPY(sTmp, pInput, shcode);			/* �����ڵ�(6)                     */
		DATA_MEMCOPY(sTmp, pInput, recprice);		/* ���ذ�(8)                       */
		DATA_MEMCOPY(sTmp, pInput, yeprice);		/* ����ü�ᰡ��(8)                 */
		DATA_MEMCOPY(sTmp, pInput, yevolume);		/* ����ü�����(8)                 */
		DATA_MEMCOPY(sTmp, pInput, hname);			/* �����(40)                      */
		DATA_MEMCOPY(sTmp, pInput, price);			/* ���簡(8)                       */
		DATA_MEMCOPY(sTmp, pInput, sign);			/* ���簡���ϴ�񱸺�(1)           */
		DATA_MEMCOPY(sTmp, pInput, change);			/* ���簡���ϴ��(8)               */
		DATA_MEMCOPY(sTmp, pInput, rate);			/* ���簡���ϴ������(7)         */
		DATA_MEMCOPY(sTmp, pInput, volume);			/* �ŷ���(12)                      */
		DATA_MEMCOPY(sTmp, pInput, value);			/* �ŷ����(15)                    */
		DATA_MEMCOPY(sTmp, pInput, offerho1);		/* �ŵ�ȣ��1(8)                    */
		DATA_MEMCOPY(sTmp, pInput, offerho2);		/* �ŵ�ȣ��2(8)                    */
		DATA_MEMCOPY(sTmp, pInput, offerho3);		/* �ŵ�ȣ��3(8)                    */
		DATA_MEMCOPY(sTmp, pInput, offerho4);		/* �ŵ�ȣ��4(8)                    */
		DATA_MEMCOPY(sTmp, pInput, offerho5);		/* �ŵ�ȣ��5(8)                    */
		DATA_MEMCOPY(sTmp, pInput, offerrem1);		/* �ŵ�ȣ������1(8)                */
		DATA_MEMCOPY(sTmp, pInput, offerrem2);		/* �ŵ�ȣ������2(8)                */
		DATA_MEMCOPY(sTmp, pInput, offerrem3);		/* �ŵ�ȣ������3(8)                */
		DATA_MEMCOPY(sTmp, pInput, offerrem4);		/* �ŵ�ȣ������4(8)                */
		DATA_MEMCOPY(sTmp, pInput, offerrem5);		/* �ŵ�ȣ������5(8)                */
		DATA_MEMCOPY(sTmp, pInput, bidho1);			/* �ż�ȣ��1(8)                    */
		DATA_MEMCOPY(sTmp, pInput, bidho2);			/* �ż�ȣ��2(8)                    */
		DATA_MEMCOPY(sTmp, pInput, bidho3);			/* �ż�ȣ��3(8)                    */
		DATA_MEMCOPY(sTmp, pInput, bidho4);			/* �ż�ȣ��4(8)                    */
		DATA_MEMCOPY(sTmp, pInput, bidho5);			/* �ż�ȣ��5(8)                    */
		DATA_MEMCOPY(sTmp, pInput, bidrem1);		/* �ż�ȣ������1(8)                */
		DATA_MEMCOPY(sTmp, pInput, bidrem2);		/* �ż�ȣ������2(8)                */
		DATA_MEMCOPY(sTmp, pInput, bidrem3);		/* �ż�ȣ������3(8)                */
		DATA_MEMCOPY(sTmp, pInput, bidrem4);		/* �ż�ȣ������4(8)                */
		DATA_MEMCOPY(sTmp, pInput, bidrem5);		/* �ż�ȣ������5(8)                */
		DATA_MEMCOPY(sTmp, pInput, totofferrem);	/* �ŵ�ȣ���Ѽ���(8)               */
		DATA_MEMCOPY(sTmp, pInput, totbidrem);		/* �ż�ȣ���Ѽ���(8)               */
		DATA_MEMCOPY(sTmp, pInput, tmofferrem);		/* �ð��ܸŵ��ܷ�(8)               */
		DATA_MEMCOPY(sTmp, pInput, tmbidrem);		/* �ð��ܸż��ܷ�(8)               */
		DATA_MEMCOPY(sTmp, pInput, uplmtprice);		/* ���Ѱ�(8)                       */
		DATA_MEMCOPY(sTmp, pInput, dnlmtprice);		/* ���Ѱ�(8)                       */
		DATA_MEMCOPY(sTmp, pInput, preychange);		/* ����ü�ᰡ�������簡���(8)     */
		DATA_MEMCOPY(sTmp, pInput, high);			/* ��(8)                         */
		DATA_MEMCOPY(sTmp, pInput, low);			/* ����(8)                         */
		DATA_MEMCOPY(sTmp, pInput, jnilvolume);		/* ���ϰŷ���(12)                  */
		DATA_MEMCOPY(sTmp, pInput, listing);		/* �����ֽļ�(12)                  */
	}
};

#endif

