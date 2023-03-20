// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2000
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic, 
// mechanical, or otherwise without the written permission of Winix Information.
//
//  Components   : jmcode.h
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2000-07	Initial version
// *****************************************************************************

#ifndef _JMCODE
#define _JMCODE

//
//	���� �����ڵ�
//

#define FENameLen	20
#define	ENameLen	40
#define HNameLen	40
#define	HCodeLen	12
#define	HSymbolLen	7

struct	hjcode {
	char	code[HCodeLen];		// �ڵ�	(12)
					// �ڵ�(7) : A######
					// �ڵ�(9) : �����μ���, ����
	char	hnam[HNameLen];		// �ѱ۸�(32)
	char	enam[ENameLen];		/* ������			*/
	char	ecng;			// ECN ����
					// �ŷ�����:1
	char	jsiz;			// �����ں��� ���߼�
					// �峻 (��:38  ��:39  ��:40)
					// ��� (1:KOSDAQ 100)
					// ��� (2:KOSDAQ MID 300)
					// ��� (3:KOSDAQ SMALL)
					// �������(jsiz += 41)
					// �����μ������� FULL�ڵ��
	char	symb[HSymbolLen];	// SYMBOL
	char	size;			// �ں��� �Ը�
					// KOSPI  (��:2  ��:3  ��:4)
					// KOSDAQ (��Ÿ����:3  �ڽ���IT����:4)
	char	ucdm;			// �����ߺз�
					// KOSDAQ:5~14  �̺з�:99
	char	ucds;			// �����Һз�
					// KOSDAQ:15~37  ����:7  �̺з�:99
	char	jjug;			// ������ ����
					// KOSPI  (������:27)
					// KOSDAQ (��ó����:2)
	char	kpgb;			// KOSPI, KOSDAQ ����
					// KOSPI  (0:������ 1:K200 2:K100 3:K50)
					// KOSDAQ (0:������ 1:KQ50)
	char	kosd;			// ���񱸺�
					// 0:�峻  10:���  30:��3����
	char    ssgb;	// �Ҽӱ���
					// 0:�Ϲ�
                    // 3:�ܱ��ο�Ź����
                    // 4:���߾��ݵ�
                    // 5:�����μ���(�߱�)
                    // 6:����
                    // 7:�����μ���(����)
                    // 8:ETF
                    // 9:�����ϵ��ݵ�(��������)
                    // 10: ���� 12:������
                    // 13: �ؿ�ETF 14:�ؿܿ���
/*	char	ssgb;			// �Ҽӱ���
					// 0:default
					// 3:�ܱ���
					// 4:���߾��ݵ�
					// 5:�����μ���
					// 6:����
					// 7:�����μ�������
					// 8:ETF
					// 9:�����ϵ��ݵ�
*/
	char	ucmd;			// �켱�ֱ��� (0:������  5:�켱��)
	char	itgb;			// KOSPI-IT/KQ-IT50����(1:ä��)
	char	wsgb;			// KOSPI ���豸������:42 (0:������ 1:���)
	char	jqty;			// �⺻�ֹ�����
	char	star;			// KOSDAQ ��Ÿ����
	char	unio;			// ��������
	char    jchk;			// ����˻��� ����
					// 0x01     �Ҽ��ǰ�������
					// 0x02     �űԻ�������
					// 0x04     �Ǹ�������
					// 0x08     �켱������
					// 0x10     ��������
					// 0x20     ��������
					// 0x40     �ŷ���������
					// 0x80     ������������
	char    elwf;			// 0: �Ϲ�, 1: �����ڻ� 20070716 �߰�
	char	sjjs[10];		// �����ֽļ�(���� ��)
	char	fill[10];		// reserved
};	
	
//
//	hjcode.kosd : ���񱸺�
//
#define	jmKOSPI		0		// KOSPI ����
#define jmKOSDAQ	10		// �ڽ���
#define	jm3RD		30		// ��3����

//
//	hjcode.ssgb : �Ҽӱ���
//
#define	jmDEF		0		// default
#define jmMUFND		4		// ���߾��ݵ�
#define jmSINJU		5		// �����μ���
#define jmREITS		6		// ����
#define jmSINJS		7		// �����μ�������
#define jmETF		8		// ETF
#define	jmHYFND		9		// �����ϵ��ݵ�
#define	jmELW		11		// ELW
#define	jmFOREIGN	3		// �ܱ��� 20070327
#define jmSHIP		10		// ����
#define jmINFRA		12		// ������

//
//	hjcode.ucmd : ������, �켱��
//
#define stockCom	0		// ������
#define stockPre	5		// �켱��



//
//	���������ڵ�
//

#define FNameLen	20
#define FCodeLen	8

struct	fjcode {
	char	cod2[FCodeLen];		// �����ڵ�
	char	hnam[FNameLen];		// �ѱ۸�
	char	enam[FENameLen];	// ������
	char	mchk;			// �Ÿ��������
					// 0x01:���尡���
					// 0x02:���Ǻ����������
					// 0x04:���������������
};

struct  sfcode {            /* �ֽļ��� �����ڵ� ����       */
	char    codx[8];        /* �����ڵ�(8)                  */
	char    hnam[32];       /* �ѱ۸�                       */
	char    enam[32];       /* ������                       */
	char    mchk;           /* �Ÿ��������                 */
	/* 0x01:���尡���              */
	/* 0x02:���Ǻ����������        */
	/* 0x04:���������������        */
	char    tjgb[2];        /* �ŷ���� ���񱸺�            */
	char    gcod[12];       /* �����ڻ������ڵ�             */
	char    gnam[32];       /* �����ڻ� �����              */
	char    fill[48];       /* reserved                     */
};

//
//	�ɼ��ڵ� 
//

#define OPriceLen	5
#define ONameLen	20
#define OCodeLen	9

//
//	�ɼ������ڵ� HEADER
//
struct	ojcodh {
	char	cjym[4][6];		// CALL �ɼ� ���� Ÿ��Ʋ
	char	pjym[4][6];		// PUT  �ɼ� ���� Ÿ��Ʋ
};

//
//	�ɼ������ڵ�
//
struct	ojcode {
	char	price[OPriceLen];	// ��簡�� (999.99)
	char	atmg;			// ATM ����
	struct	{
		char	yorn;		// ����	�ڵ� ����
		char	cod2[OCodeLen];	// ���������ڵ�
		char	hnam[ONameLen];	// �ѱ������
		char	enam[FENameLen];	// ������
		char	mchk;		// �Ÿ��������
					// 0x01:���尡���
					// 0x02:���Ǻ����������
					// 0x04:���������������
	} call[4]; 
	struct	{
		char	yorn;		// ����	�ڵ� ����
		char	cod2[OCodeLen];	// ���������ڵ�
		char	hnam[ONameLen];	// �ѱ������
		char	enam[FENameLen];	// ������
		char	mchk;		// �Ÿ��������
					// 0x01:���尡���
					// 0x02:���Ǻ����������
					// 0x04:���������������
	} put[4]; 
};


//
//	ä���ڵ�
//

#define	CCodeLen	12
#define	CNameLen	40

struct	cjcode	{
	char	code[CCodeLen];
	char	cgub;			// ä�Ǳ���
					// 1:��������1�� �����
					// 2:��������1�� ������
					// 3:���ﵵ��ö�� �����
					// 4:���ﵵ��ö�� ������
					// 5:��������ä�� �����
					// 6:��������ä�� ������
					// 7:���浵��ö�� �����
					// 8:���浵��ö�� ������
					// 99:��ȯ��ä
	char	hnam[CNameLen];		// ä���̸�
	char    ngub;			// ���� 0: ��ü, 1:�ֱ�����
};

//
//	cjcode : ä�Ǳ���
//
#define	cjSo	1			// �Ҿ�ä��
#define	cjCh	2			// ��ȯ��ä


//
//	�����ڵ�
//

#define	UNameLen	20

struct	upcode	{
	char	jgub;			// �屸��
					// 0:�峻 1:���
					// 1: KOSPI200
					// 3: KOSDAQ50(1), KQ-IT 50(2)
					// 4: K100P(1), K50(2), IT(3
					// 6: SECTOR, 7:�������� 
	char	ucod;			// �����ڵ�
	char	hnam[UNameLen];		// �ѱ۸�
};

//
//	upcode.jgup : �屸�� : ���Ѻ���
//
#define	upKOSPI		0		// 1~41
#define upKOSPI200	1		// 1~6
#define	upKOSDAQ	2		// 1~40
#define upKOSDAQ50	3		// KOSDAQ50   : ucod = 1
					// KOSDAQIT50 : ucod = 2
#define upKOSPIx	4		// KOSPI100   : ucod = 1
					// KOSPI50    : ucod = 2
					// KOSPIIT    : ucod = 3
#define	upSECTOR	6		// ��������
#define upFREEBOARD	7		// ��������   
#define	upINTEGRATE	9		// ��������

#define	upNOT		10		// not
#define	upOTHER		11		// KOSPI IT, KOSPI200
					// KOSPI100, KOSDAQ50


//
//	KOFEX
//

//
//	�̷�����
//

#define KCodeLen        15
#define KNameLen        30

struct	kfcode	{
	char	kcod[2];		// Commodity Code
					// 01 :	USD Currency Futures
					// 02 : USD Currency Call Option
					// 03 : USD Currency Put Option
					// 04 : GOLD Currency Futures
					// 05 : CD Futures
					// 06 : KTB Futures
					// 07 : KOSDAQ50 Futures
	char	codx[KCodeLen];		// �����ڵ�
	char	hnam[KNameLen];		// �ѱ۸�
};

#define kfTOTAL		0
#define	kfUSDFUTURE	1
#define kfUSDCALL	2
#define kfUSDPUT	3
#define kfGOLD		4
#define kfCD		5
#define kfKTB		6
#define kfKOSDAQ	7


//
//	����
//

#define KFOCodeLen      15
#define KFONameLen      30
#define KFOPriceLen	5
#define KFOMonthLen	4

struct	kfocode	{		
	char	codx[KFOCodeLen];	// �����ڵ�
					// ��������: 6�ڸ�
					// ��������: 8�ڸ�
					// �ɼǺ���: 9�ڸ�
					// �ɼǺ���:11�ڸ�
					// ������ NULL
	char	hnam[KFONameLen];	// �ѱ۸�
	char	flag;			// �����ɼ� ���� FLAG
					// 1:����
					// 2:CALL �ɼ�
					// 3:PUT  �ɼ�
	char	mont[KFOMonthLen];	// 1-12 ����
	char	comb;			// �������ձ���
					// 1:Time  Spread
					// 2:Price Spread
					// 0:���� ���ش�
	char	atmf;			// ATM �ش翩��
					// 1   :ATM
					// else:���ش�
	char	hsga[KFOPriceLen];	// ��簡�� (�ɼǿ��� �ش�)
};


//
//	�����ɼ�
//

#define PCodeLen	8

struct pjcode {
	char	gubn;		// 1:KOSPI, 2:KOSDAQ
	char	tjgb[2];	// �ŷ�������񱸺�
	char	snam[20];	// �ŷ���������

	char	codx[8];	// �����ڵ�
	char	hnam[30];	// �ѱ۸�
	char	enam[30];	// ������
	char	gcod[12];	// �����ڻ������ڵ�
	char	mont[4];	// ����
	char	atmx;		// ATM���񿩺� 1:ATM����
	char	hsga[PCodeLen];	// ��簡
	char	mchk;		// �Ÿ��������
				// 0x01:���尡���
				// 0x02:���Ǻ����������
				// 0x04:���������������
};

#define	L_pjcode	sizeof(struct pjcode)


//
//	ELW
//

#define	ELWCodeLen	12
#define	ELWHNameLen	32
#define	ELWENameLen	20

struct elwcode {
	char	codx[ELWCodeLen];// ELW�����ڵ�(6) J######
	char	hnam[ELWHNameLen];	// �ѱ۸�
	char	enam[ELWENameLen];	// ������
	char	symb[7];	// SYMBOL
	char	lphn[5][3];	// ELW - LP ȸ����ȣ 1~5
	char	lpse[5];	// ELW �������� (999V99)
	char	hbge[ELWENameLen];	// ELW �ѱ۹�������
	char	ebge[ELWENameLen];	// ELW ������������
	char	bgce[12];	// ELW �������ڵ�
	char	gcjs[5][12];	// ELW �����ڻ� 1~5
	char	grat[5][5];	// ELW �����ڻ걸������ 1~5 (999V99)
	char	gcge[10];	// ELW ���ʰ��� (99999999V99)
	char	hsge[10];	// ELW ��簡�� (99999999V99)
	char	krye[2];	// ELW �Ǹ����� 01:CALL, 02:PUT, 03:��Ÿ
	char	khbe[2];	// ELW �Ǹ������ 01:������, 02:�̱���, 03:��Ÿ
	char	mjmt[8];	// ELW ����� (YYYYMMDD)
	char	fill[40];	// reserved
};

#define L_elwcode	sizeof(struct elwcode)


// �Ҹ�ä��(sjcode.dat)
#define	SCodeLen	12
#define	SNameLen	20

struct	sjcode	{
	char	code[SCodeLen];
	char	cgub;			// ä�Ǳ���
					// 1: ��ä
					// 2: ���ä
					// 3: ����ä
					// 4: �����Ư��ä
					// 5: �ֻ�ä
					// 6: ��Ÿ
	char	hnam[SNameLen];		// ä���̸�
	char    ngub;			// ���� 0: ��ü, 1:�ֱ�����
};

//
//	sjcode : �Ҹ�ä�Ǳ���
//
#define	sjKC	1			// ��ä
#define	sjTA	2			// ���ä
#define	sjKY	3			// ����ä
#define	sjNK	4			// �����Ư��ä
#define	sjCS	5			// �ֻ�ä
#define	sjET	6			// ��Ÿ


#endif 
