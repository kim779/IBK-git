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
#pragma once

//
//	���� �����ڵ�
//

#define FENameLen	20
#define	ENameLen	40
#define HNameLen	40
#define	HCodeLen	12
#define	HSymbolLen	7
#pragma pack(1)

struct	hjcodex {			/* ���������ڵ�	����			*/
	char	code[12];		/* �ڵ�(7) : A######			*/
							/* �ڵ�(9) : �����μ���, ����	*/
	char	hnam[40];		/* �ѱ۸�						*/
	char	enam[40];		/* ������						*/
	char	ecng;			/* ECN (1:�ŷ�����)				*/
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
							/* 10: ���� 11:ELW 12:������ 	*/
							/* 13: �ؿ�ETF 14:�ؿܿ���		*/
	char	ucmd;			/* �켱�ֱ��� 0:������ 5:�켱��	*/
	char	itgb;			/* KOSPI-IT/KQ-IT50����(1:ä��)	*/
	char	wsgb;			/* 0:�Ϲ� 1:���豸��������	*/
	char	jqty;			/* �⺻�ֹ�����					*/
	char	star;			/* Star ���񱸺�				*/
	char	unio;			/* ������������ ����			*/
							/* 0: �Ϲ�, 1:��������			*/
							/* 2006.01.12 htsadd			*/
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
	char	prmr;			/* �����̾�����(1:�����̾�����)	*/
							/* 20160523 �����̾� ���� ���� �ߴ� */
							/* 2:��������ELW				*/

	char uc10;				/* �Ǽ����", 0x00
						  �������", 0x01 ,
						  ö������", 0x02
						  ������ȭ��", 0x03
						  �������", 0x04
						  ����", 0x05
						  �ʼ��Һ���", 0x06
						  �����Һ���", 0x07 					*/
	char	uc20;				/*�췮���", 0x00
						   ��ó���", 0x01 ,
						   �߰߱��", 0x02 ,
						   �ż�����", 0x03 					*/

						/* ��������					*/
	char	sec1;		/*	�ڵ���/�ݵ�ü/�ǰ�/����					*/
	char	sec2;		/*	�������/ȭ��E/ö��/�ʼ�				*/
	char	sec3;		/*	�̵��/�Ǽ�/����/����					*/
	char	sec4;		/*	����/��ȸ����/����/���					*/
	char	sec5;		/*	�Һ���/����/KRX100					*/
	char    acdl[6];       	/* ������з�                      <--- �߰�			*/
	char    acdm[6];       	/* �����ߺз�                      <--- �߰�, AS-IS�� ucdm	*/
	char    acds[6];       	/* �����Һз�                     <--- �߰�, AS-IS�� ucds	*/
	char	fill[20];		/* reserved	7						*/
};
/*
TO-BE �����з� ù ����Ʈ�� '0' ���ϴ� �̺з�(���� '000   ' Ȥ�� '      '���� ����. ���� ������ Ȯ�� �� ���� �ϳ��� ���� ����)



***** ucdm <-> acdm, ucds <-> acds  ����
           AS-IS   TO-BE  DESCRIPTION
�ڽ������� 001     KGG01P �ڽ���                        
�ڽ������� 002     KGZ01P �ڽ��� ������                 
�ڽ������� 003     KGZ02P �ڽ��� ������                 
�ڽ������� 004     KGZ03P �ڽ��� ������                 
�ڽ������� 005     KGS01P ���ķ�ǰ                      
�ڽ������� 006     KGS02P ����,�Ǻ�                     
�ڽ������� 007     KGS03P ����,����                     
�ڽ������� 008     KGS04P ȭ��                          
�ڽ������� 009     KGS05P �Ǿ�ǰ                        
�ڽ������� 010     KGS06P ��ݼӱ���                    
�ڽ������� 011     KGS07P ö���ױݼ�                    
�ڽ������� 012     KGS08P ���                          
�ڽ������� 013     KGS09P ����,����                     
�ڽ������� 014     KGS19P �Ƿ�����                      
�ڽ������� 015     KGS10P ������                      
�ڽ������� 016     KGS11P �����                        
�ڽ������� 017     KGS20P ���Ⱑ����                    
�ڽ������� 018     KGS12P �Ǽ���                        
�ڽ������� 019     KGS13P ���â��                      
�ڽ������� 020     KGS21P ��ž�                        
�ڽ������� 021     KGS14P ������                        
�ڽ������� 022     KGS15P ����                          
�ڽ������� 024     KGS16P ����                          
�ڽ������� 025     KGS17P ����                          
�ڽ������� 026     KGS22P ���񽺾�                      
�ڽ������� 027     KGS18P ������                        
�ڽ������� 028     K2G14P �ѱ��ְ����� 200(Ư����������)
�ڽ������� 029     K2G01P �ڽ��� 200                    
�ڽ������� 035     K2G02P �ڽ��� 100                    
�ڽ������� 036     K2G03P �ڽ��� 50                     
�ڽ������� 150     K2S11P KOSPI 200 Ŀ�´����̼Ǽ���  
�ڽ������� 151     K2S02P �ڽ��� 200 �Ǽ�               
�ڽ������� 152     K2S08P �ڽ��� 200 �߰���             
�ڽ������� 153     K2S09P �ڽ��� 200 ö��/����          
�ڽ������� 154     K2S05P �ڽ��� 200 ������/ȭ��        
�ڽ������� 155     K2S07P �ڽ��� 200 �������           
�ڽ������� 156     K2S03P �ڽ��� 200 ����               
�ڽ������� 157     K2S10P �ڽ��� 200 ��Ȱ�Һ���         
�ڽ������� 158     K2S06P �ڽ��� 200 ���Һ���         
�ڽ������� 159     K2S04P �ڽ��� 200 �����             
�ڽ������� 160     K2S01P �ڽ��� 200 �ｺ�ɾ�           

�ڽ������� 001     QGG01P �ڽ���               
�ڽ������� 002     QGZ01P �ڽ��� ������        
�ڽ������� 003     QGZ02P �ڽ��� ������        
�ڽ������� 004     QGZ03P �ڽ��� ������   

�ڽ������� 007     QGS35P �߼ұ������         
�ڽ������� 008     QGS36P ����        

�ڽ������� 009     QGS37P ���뼭��           
�ڽ������� 010     QGS38P �Ǽ�        **         
�ڽ������� 011     QGS39P ����                 
�ڽ������� 012     QGS01P ��Ÿ����           
�ڽ������� 015     QGS32P �ڽ��� IT            
�ڽ������� 017     QGS42P ��������ȭ     **       
�ڽ������� 024     QGS02P ����         **        
�ڽ������� 026     QGS03P �Ǽ�                 
�ڽ������� 027     QGS04P ����        **         
�ڽ������� 029     QGS31P ���        **         
�ڽ������� 031     QGS05P ����        **            
�ڽ������� 033     QGS49P �������           
�ڽ������� 037     QGS34P ��������ȭ           
�ڽ������� 041     QGS06P ��Ź�ۼ���          **
�ڽ������� 042     QGS07P IT S/W & SVC           **
�ڽ������� 043     QGS08P IT H/W                 **



�ڽ������� 056     QGS09P ���ķᡤ���         
�ڽ������� 058     QGS10P �������Ƿ�           
�ڽ������� 062     QGS11P ���̡�����           
�ڽ������� 063     QGS12P ���ǡ���ü����       
�ڽ������� 065     QGS13P ȭ��                 
�ڽ������� 066     QGS33P ����                 
�ڽ������� 067     QGS14P ��ݼ�               
�ڽ������� 068     QGS15P �ݼ�                 
�ڽ������� 070     QGS16P ��衤���           
�ڽ������� 072     QGS17P �Ϲ���������         
�ڽ������� 074     QGS18P �Ƿᡤ���б��       
�ڽ������� 075     QGS19P �����񡤺�ǰ       
�ڽ������� 077     QGS20P ��Ÿ ����            
�ڽ������� 081     QGS65P ���հǼ�             
�ڽ������� 082     QGS66P �����Ǽ�             
�ڽ������� 084     QGS68P ����                 
�ڽ������� 092     QGS75P ����                 
�ڽ������� 099     QGS81P �������             
�ڽ������� 106     QGS87P ��������ȭ           
�ڽ������� 151     QGS21P ��ż���           
�ڽ������� 152     QGS22P ��ۼ���           
�ڽ������� 153     QGS23P ���ͳ�               
�ڽ������� 154     QGS24P ������������         
�ڽ������� 155     QGS25P ����Ʈ����           
�ڽ������� 156     QGS26P ��ǻ�ͼ���         
�ڽ������� 157     QGS27P ������             
�ڽ������� 158     QGS28P �������             
�ڽ������� 159     QGS29P �ݵ�ü               
�ڽ������� 160     QGS30P IT��ǰ               

�ڽ������� 181     QGG03P �ڽ��� �췮�����    
�ڽ������� 182     QGG04P �ڽ��� ��ó�����    
�ڽ������� 183     QGG05P �ڽ��� �߰߱����    
�ڽ������� 184     QGG06P �ڽ��� �����������
*/







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
					// 15 : �ڳؽ� 
/*	char	ssgb;	// �Ҽӱ���
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
	char	prmr;			// �����̾�����
	
	//2011.11.22 KSJ
	char	uc10;
						/* �Ǽ����", 0x01   
                          �������", 0x02 , 
                          ö������", 0x03   
                          ������ȭ��", 0x04     
                          �������", 0x05   
                          ����", 0x06   
                          �ʼ��Һ���", 0x07     
                          �����Һ���", 0x08     */

	char	uc20;
						/*�췮���", 0x01   
                           ��ó���", 0x02 ,  
                           �߰߱��", 0x03 ,    
                           �ż�����", 0x04    */      

	char	fill[7];		// reserved
	// KSJ
//	char	fill[9];		// reserved
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
#define jmKONEX		15		// KDK 2013.05.14 KONEX 
#define jmETN		16		// KSJ 2014.10.28 ETN �߰�
#define jmSPAC		17		// KSJ 2015.11.03 spac �߰�
#define jmKOSDAQ150	18		// KSJ 2016.08.22 KOSDAQ 150 �߰�

//
//	hjcode.ucmd : ������, �켱��
//
#define stockCom	0		// ������
#define stockPre	5		// �켱��

/*
 *	��ǰ���� �ڵ� 
 */
struct	ccode	{			/* ��ǰ���� �����ڵ� ����	*/
	char	codx[8]{};		/* �����ڵ�(8)			*/
	char	hnam[50]{};		/* �ѱ۸�			*/
	char	enam[50]{};		/* ������			*/
        char    mchk{};       /* �Ÿ��������                 */
                            /* 0x01:���尡���              */
                            /* 0x02:���Ǻ����������        */
                            /* 0x04:���������������        */
	char	tjgb[2]{};		/* �ŷ���� ���񱸺�		*/
	char	jcod[12]{};		/* �����ڻ� �����ڵ�		*/
	char	jnam[40]{};		/* �����ڻ� �����		*/
	char	fill[20]{};		/* filler			*/
};

//
//	���������ڵ�
//

#define FNameLen	20
#define FCodeLen	8

struct	fjcode {
	char	cod2[FCodeLen]{};		// �����ڵ�
	char	hnam[FNameLen]{};		// �ѱ۸�
	char	enam[FENameLen]{};	// ������
	char	mchk{};			// �Ÿ��������
					// 0x01:���尡���
					// 0x02:���Ǻ����������
					// 0x04:���������������
};

struct  sfcode {            /* �ֽļ��� �����ڵ� ����       */
	char    codx[8]{};        /* �����ڵ�(8)                  */
	char    hnam[50]{};       /* �ѱ۸�                       */
	char    enam[50]{};       /* ������                       */
	char    mchk{};           /* �Ÿ��������                 */
	/* 0x01:���尡���              */
	/* 0x02:���Ǻ����������        */
	/* 0x04:���������������        */
	char    tjgb[2]{};        /* �ŷ���� ���񱸺�            */
	char    gcod[12]{};       /* �����ڻ������ڵ�             */
	char    gnam[32]{};       /* �����ڻ� �����              */
	char	gdan[18]{};		/* �ŷ�����						*/
	char    mgub[1]{};        /* ���ϱ��� �ڽ���[K], �ڽ���[Q]         */
	char    fill[29]{};       /* reserved                     */
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
	char	cjym[11][6]{};		// CALL �ɼ� ���� Ÿ��Ʋ
	char	pjym[11][6]{};		// PUT  �ɼ� ���� Ÿ��Ʋ
};

//
//	�ɼ������ڵ�
//
struct	ojcode {
	char	price[OPriceLen]{};	// ��簡�� (999.99)
	char	atmg{};			// ATM ����
	struct	{
		char	yorn{};		// ����	�ڵ� ����
		char	cod2[OCodeLen]{};	// ���������ڵ�
		char	hnam[ONameLen]{};	// �ѱ������
		char	enam[FENameLen]{};	// ������
		char	mchk{};		// �Ÿ��������
					// 0x01:���尡���
					// 0x02:���Ǻ����������
					// 0x04:���������������
	} call[11]{}; 
	struct	{
		char	yorn{};		// ����	�ڵ� ����
		char	cod2[OCodeLen]{};	// ���������ڵ�
		char	hnam[ONameLen]{};	// �ѱ������
		char	enam[FENameLen]{};	// ������
		char	mchk{};		// �Ÿ��������
					// 0x01:���尡���
					// 0x02:���Ǻ����������
					// 0x04:���������������
	} put[11]{}; 
};


//
//	ä���ڵ�
//

#define	CCodeLen	12
#define	CNameLen	40

struct	cjcode	{
	char	code[CCodeLen]{};
	char	cgub{};			// ä�Ǳ���
					// 1:��������1�� �����
					// 2:��������1�� ������
					// 3:���ﵵ��ö�� �����
					// 4:���ﵵ��ö�� ������
					// 5:��������ä�� �����
					// 6:��������ä�� ������
					// 7:���浵��ö�� �����
					// 8:���浵��ö�� ������
					// 99:��ȯ��ä
	char	hnam[CNameLen]{};		// ä���̸�
	char    ngub{};			// ���� 0: ��ü, 1:�ֱ�����
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
	char	jgub{};			// �屸��
					// 0:�峻 1:���
					// 1: KOSPI200
					// 3: KOSDAQ50(1), KQ-IT 50(2)
					// 4: K100P(1), K50(2), IT(3
					// 6: SECTOR, 7:�������� 
	char	ucod{};			// �����ڵ�
	char	hnam[UNameLen]{};	// �ѱ۸�
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
#define	upKONEX		15		// KONEX
#define	upETN		16		// KONEX
#define	upETC		17		// ��Ÿ����

//
//	KOFEX
//

//
//	�̷�����
//

#define KCodeLen        15
#define KNameLen        30

struct	kfcode	{
	char	kcod[2]{};		// Commodity Code
					// 01 :	USD Currency Futures
					// 02 : USD Currency Call Option
					// 03 : USD Currency Put Option
					// 04 : GOLD Currency Futures
					// 05 : CD Futures
					// 06 : KTB Futures
					// 07 : KOSDAQ50 Futures
	char	codx[KCodeLen]{};	// �����ڵ�
	char	hnam[KNameLen]{};	// �ѱ۸�
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
	char	codx[KFOCodeLen]{};	// �����ڵ�
					// ��������: 6�ڸ�
					// ��������: 8�ڸ�
					// �ɼǺ���: 9�ڸ�
					// �ɼǺ���:11�ڸ�
					// ������ NULL
	char	hnam[KFONameLen]{};	// �ѱ۸�
	char	flag{};			// �����ɼ� ���� FLAG
					// 1:����
					// 2:CALL �ɼ�
					// 3:PUT  �ɼ�
	char	mont[KFOMonthLen]{};	// 1-12 ����
	char	comb{};			// �������ձ���
					// 1:Time  Spread
					// 2:Price Spread
					// 0:���� ���ش�
	char	atmf{};			// ATM �ش翩��
					// 1   :ATM
					// else:���ش�
	char	hsga[KFOPriceLen]{};	// ��簡�� (�ɼǿ��� �ش�)
};


//
//	�����ɼ�
//

#define PCodeLen	8

struct pjcode {
	char	gubn{};		// 1:KOSPI, 2:KOSDAQ
	char	tjgb[2]{};	// �ŷ�������񱸺�
	char	snam[20]{};	// �ŷ���������

	char	codx[8]{};	// �����ڵ�
	char	hnam[30]{};	// �ѱ۸�
	char	enam[30]{};	// ������
	char	gcod[12]{};	// �����ڻ������ڵ�
	char	mont[4]{};	// ����
	char	atmx{};		// ATM���񿩺� 1:ATM����
	char	hsga[PCodeLen]{};	// ��簡
	char	mchk{};		// �Ÿ��������
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
	char	codx[ELWCodeLen]{};	// ELW�����ڵ�(6) J######
	char	hnam[ELWHNameLen]{};	// �ѱ۸�
	char	enam[ELWENameLen]{};	// ������
	char	symb[7]{};		// SYMBOL
	char	lphn[5][3]{};		// ELW - LP ȸ����ȣ 1~5
	char	lpse[5]{};		// ELW �������� (999V99)
	char	hbge[ELWENameLen]{};	// ELW �ѱ۹�������
	char	ebge[ELWENameLen]{};	// ELW ������������
	char	bgce[12]{};		// ELW �������ڵ�
	char	gcjs[5][12]{};		// ELW �����ڻ� 1~5
	char	grat[5][5]{};		// ELW �����ڻ걸������ 1~5 (999V99)
	char	gcge[10]{};		// ELW ���ʰ��� (99999999V99)
	char	hsge[10]{};		// ELW ��簡�� (99999999V99)
	char	krye[2]{};		// ELW �Ǹ����� 01:CALL, 02:PUT, 03:��Ÿ
	char	khbe[2]{};		// ELW �Ǹ������ 01:������, 02:�̱���, 03:��Ÿ
	char	mjmt[8]{};		// ELW ����� (YYYYMMDD)
	char	koba[1]{};		// ��������ELW ����
	char	fill[39]{};		// reserved
};

#define L_elwcode	sizeof(struct elwcode)


// �Ҹ�ä��(sjcode.dat)
#define	SCodeLen	12
#define	SNameLen	20

struct	sjcode	{
	char	code[SCodeLen]{};
	char	cgub{};			// ä�Ǳ���
					// 1: ��ä
					// 2: ���ä
					// 3: ����ä
					// 4: �����Ư��ä
					// 5: �ֻ�ä
					// 6: ��Ÿ
	char	hnam[SNameLen]{};	// ä���̸�
	char    ngub{};			// ���� 0: ��ü, 1:�ֱ�����
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

#pragma pack()
