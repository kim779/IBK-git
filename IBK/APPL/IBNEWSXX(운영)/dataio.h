#pragma once

#define TK_PIBONEWS		(1)

#define LINE_SIZE	130		/* LINE size		*/
#define TITL_SIZE   132     /* TITLE size       */
#define SCODE_CNT		10
#define SCODE_SIZE		12

struct  pibonews_mid {
    char    date[8];        /* �ڷ�����     */
    char    gubn[2];        /* ��������     */
    char    subg[4];        /* �з��ڵ�     */
    char    seqn[10];       /* �Ϸù�ȣ     */
    char    titl[TITL_SIZE];     /* ���� ����        */
};

struct  pibonews_mod {
	char	cflag[1];				//2017.02.10 KSJ ������ ����
	char    titl[TITL_SIZE];     /* ���� ����        */
    char    subcod[SCODE_CNT][SCODE_SIZE]; /* ���� �ڵ� */
    char    size[7];        /* size of data     */	//2015.11.20 KSJ size 5 -> 7
    char    data[1];
};

struct hjcode {				/* ���������ڵ� ����            */
    char    codx[12];       /* �ڵ�(7) : A######            */
							/* �ڵ�(9) : �����μ���, ����   */
    char    hnam[40];       /* �ѱ۸�                       */
    char    enam[40];       /* ������                       */
    char    ecnf;           /* ECN (1:�ŷ�����)             */
    char    jsiz;           /* �����ں��� ���߼�            */
							/* �峻(��: 38, ��: 39, ��: 40) */
							/* ���(1: KOSDAQ 100)          */
							/* ���(2: KOSDAQ MID 300)      */
							/* ���(3: KOSDAQ SMALL)        */
							/* ############################ */
							/* ������� : (jsiz += 41)      */
							/* �����μ������� FULL�ڵ� ��   */
    char    symb[7];        /* SYMBOL                       */
    char    size;           /* �ð��ѾױԸ�(2, 3, 4)        */
    char    ucdm;           /* �����ߺз�       99:�̺з�   */
    char    ucds;           /* �����Һз�       99:�̺з�   */
							/* KOSDAQ 7:����                */
    char    jjug;           /* ������ ����(27: ������)      */
							/* KOSDAQ ���� ����(=2)         */
    char    kpgb;           /* KOSPI200/KOSDAQ ����         */
							/* 0:������ 1:K200 2:K100 3:K50 */
							/* 0:������ 1:KQ50              */
    char    kosd;           /* ���񱸺�                     */
							/* 0:�峻  10:���  30:��3����  */
    char    ssgb;           /* �Ҽӱ���                     */
							/* 0:�Ϲ�                       */
							/* 3:�ܱ��ο�Ź����             */
							/* 4:���߾��ݵ�                 */
							/* 5:�����μ���(�߱�)           */
							/* 6:����                       */
							/* 7:�����μ���(����)           */
							/* 8:ETF                        */
							/* 9:�����ϵ��ݵ�(��������)     */
							/* 10: ���� 11:ELW 12:������    */
							/* 13: �ؿ�ETF 14:�ؿܿ���      */
    char    ucmd;           /* �켱�ֱ��� 0:������ 5:�켱�� */
    char    itgb;           /* KOSPI-IT/KQ-IT50����(1:ä��) */
    char    wsgb;           /* 0:�Ϲ� 1:���豸��������    */
    char    jqty;           /* �⺻�ֹ�����                 */
    char    star;           /* Star ���񱸺�                */
    char    unio;           /* ������������ ����            */
							/* 0: �Ϲ�, 1:��������          */
							/* 2006.01.12 htsadd            */
    char    jchk;           /* ����˻��� ����              */
							/* 0x01     �Ҽ��ǰ�������      */
							/* 0x02     �űԻ�������        */
							/* 0x04     �Ǹ�������          */
							/* 0x08     �켱������          */
							/* 0x10     ��������            */
							/* 0x20     ��������            */
							/* 0x40     �ŷ���������        */
							/* 0x80     ������������        */
    char    elwf;           /* 0: �Ϲ�, 1: �����ڻ�         */
    char    sjjs[10];       /* �����ֽļ�(����:��)          */
    char    kqpg;           /* �����̾�����(1:�����̾�����) */
							/* 2:��������ELW                */
    char    fill[ 9];       /* reserved                     */
};

