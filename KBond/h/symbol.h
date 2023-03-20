//*******************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2003
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic,
// mechanical, or otherwise without the written permission of Winix Information.
//
//  Components   : symbol.h	define symbol
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2003-05	Initial version
//		 02.00  2008-02 Upgrade version (for EU2S)
//		 02.10	2009-06 Customize version (for KATS)
//*****************************************************************************
#ifndef	_SYMBOL
#define	_SYMBOL

const char gnCHAR	= 0x7F;
const char tabCHAR	= 0x09;
const char lfCHAR	= 0x0A;
const char crCHAR	= 0x0D;

#define	SNOT	"000"

//****************************************************************
// major SYMBOL	        
//****************************************************************
#define	MSST	"001"		// ����
#define	MSFU	"020"		// ����
#define	MSOP	"030"		// �ɼ�
#define	MSHO	"040"		// �ֽĿɼ�
#define	MSSF	"060"		// �ֽļ���
#define	MSUP	"010"		// ����
#define MSPT	"070"		// ��ǰ����

#define	MGST	"002"		// ����	ü�� GRID
#define	MGFU	"021"		// ����	ü�� GRID
#define	MGOP	"031"		// �ɼ�	ü�� GRID
#define	MGHO	"041"		// �ֽĿɼ� ü�� GRID
#define	MGSF	"061"		// �ֽļ��� ü�� GRID

#define MSPS	"071"		// ��ǰ���� �ü�
#define MSPH	"072"		// ��ǰ���� ȣ��
#define	MGPT	"074"		// ��ǰ���� ü�� GRID




//****************************************************************
// �Է��ڵ�	
//****************************************************************
#define	ICOD	"301"		// ���� : MSST + ICOD
				// ���� : MSFU + ICOD
				// �ɼ� : NSOP + ICOD

#define	UDCD	"301"		// ������з�
				//	00:KOSPI
				//	10:KOSPI200
				//	11:REAL 200
				//	20:KOSPI100
				//	30:KOSPI IT
				//	90:KOSDAQ
				//	91:KOSDAQ50
				//	92:REAL50
				//	40:�ѱ���ǥ����(Korea Blue Chip Index)
#define	USCD	"302"		// �����Һз�
#define	UCCD	"303"		// �����ڵ�(4�ڸ�)
				//	00XX:KOSPI
				//	10XX:KOSPI200
				//	11XX:REAL 200
				//	20XX:KOSPI100
				//	30XX:KOSPI IT
				//	90XX:KOSDAQ
				//	91XX:KOSDAQ50
				//	92XX:REAL50
				//	40XX:�ѱ���ǥ����(Korea Blue Chip Index)
//****************************************************************
// �ü� ���� SYMBOL	        
//****************************************************************
#define	CODX	"021"		// �����ڵ�(�����ڵ�5)
#define	HNAM	"022"		// �ѱ������
#define	CURR	"023"		// ���簡
#define	DIFF	"024"		// ���ϴ��
#define	MDGA	"025"		// �ŵ�ȣ��
#define	MSGA	"026"		// �ż�ȣ��
#define	GVOL	"027"		// �����ŷ���(����:���)
#define	GAMT	"028"		// �����ŷ����(����:���)
#define	SIGA	"029"		// �ð�
#define	KOGA	"030"		// ��
#define	JEGA	"031"		// ����
#define	CVOL	"032"		// ü��ŷ���(���)
#define	UDYL	"033"		// �����
#define	CTIM	"034"		// ü��ð�HHMMSS

//****************************************************************
// �켱ȣ�� ���� SYMBOL		 
//****************************************************************
#define	HTIM	"040"		// ȣ�� �ð�

#define	DHJ1	"041"		// 1���켱ȣ���ܷ�(�ŵ�)
#define	DHJ2	"042"		// 2���켱ȣ���ܷ�(�ŵ�)
#define	DHJ3	"043"		// 3���켱ȣ���ܷ�(�ŵ�)
#define	DHJ4	"044"		// 4���켱ȣ���ܷ�(�ŵ�)
#define	DHJ5	"045"		// 5���켱ȣ���ܷ�(�ŵ�)
#define	DHJ6	"046"		// 6���켱ȣ���ܷ�(�ŵ�)
#define	DHJ7	"047"		// 7���켱ȣ���ܷ�(�ŵ�)
#define	DHJ8	"048"		// 8���켱ȣ���ܷ�(�ŵ�)
#define	DHJ9	"049"		// 9���켱ȣ���ܷ�(�ŵ�)
#define	DHJA	"050"		// 10���켱ȣ���ܷ�(�ŵ�)

#define	DHG1	"051"		// 1���켱ȣ��(�ŵ�)
#define	DHG2	"052"		// 2���켱ȣ��(�ŵ�)
#define	DHG3	"053"		// 3���켱ȣ��(�ŵ�)
#define	DHG4	"054"		// 4���켱ȣ��(�ŵ�)
#define	DHG5	"055"		// 5���켱ȣ��(�ŵ�)
#define	DHG6	"056"		// 6���켱ȣ��(�ŵ�)
#define	DHG7	"057"		// 7���켱ȣ��(�ŵ�)
#define	DHG8	"058"		// 8���켱ȣ��(�ŵ�)
#define	DHG9	"059"		// 9���켱ȣ��(�ŵ�)
#define	DHGA	"060"		// 10���켱ȣ��(�ŵ�)

#define	SHJ1	"061"		// 1���켱ȣ���ܷ�(�ż�)
#define	SHJ2	"062"		// 2���켱ȣ���ܷ�(�ż�)
#define	SHJ3	"063"		// 3���켱ȣ���ܷ�(�ż�)
#define	SHJ4	"064"		// 4���켱ȣ���ܷ�(�ż�)
#define	SHJ5	"065"		// 5���켱ȣ���ܷ�(�ż�)
#define	SHJ6	"066"		// 6���켱ȣ���ܷ�(�ż�)
#define	SHJ7	"067"		// 7���켱ȣ���ܷ�(�ż�)
#define	SHJ8	"068"		// 8���켱ȣ���ܷ�(�ż�)
#define	SHJ9	"069"		// 9���켱ȣ���ܷ�(�ż�)
#define	SHJA	"070"		// 10���켱ȣ���ܷ�(�ż�)

#define	SHG1	"071"		// 1���켱ȣ��(�ż�)
#define	SHG2	"072"		// 2���켱ȣ��(�ż�)
#define	SHG3	"073"		// 3���켱ȣ��(�ż�)
#define	SHG4	"074"		// 4���켱ȣ��(�ż�)
#define	SHG5	"075"		// 5���켱ȣ��(�ż�)
#define	SHG6	"076"		// 6���켱ȣ��(�ż�)
#define	SHG7	"077"		// 7���켱ȣ��(�ż�)
#define	SHG8	"078"		// 8���켱ȣ��(�ż�)
#define	SHG9	"079"		// 9���켱ȣ��(�ż�)
#define	SHGA	"080"		// 10���켱ȣ��(�ż�)

//****************************************************************
// ���� ȣ���ܷ�		
//****************************************************************
#define	DHX1	"081"		// 1���켱ȣ���ܷ����(�ŵ�)
#define	DHX2	"082"		// 2���켱ȣ���ܷ����(�ŵ�)
#define	DHX3	"083"		// 3���켱ȣ���ܷ����(�ŵ�)
#define	DHX4	"084"		// 4���켱ȣ���ܷ����(�ŵ�)
#define	DHX5	"085"		// 5���켱ȣ���ܷ����(�ŵ�)
#define	DHX6	"086"		// 6���켱ȣ���ܷ����(�ŵ�)
#define	DHX7	"087"		// 7���켱ȣ���ܷ����(�ŵ�)
#define	DHX8	"088"		// 8���켱ȣ���ܷ����(�ŵ�)
#define	DHX9	"089"		// 9���켱ȣ���ܷ����(�ŵ�)
#define	DHXA	"090"		// 10���켱ȣ���ܷ����(�ŵ�)

#define	SHX1	"091"		// 1���켱ȣ���ܷ����(�ż�)
#define	SHX2	"092"		// 2���켱ȣ���ܷ����(�ż�)
#define	SHX3	"093"		// 3���켱ȣ���ܷ����(�ż�)
#define	SHX4	"094"		// 4���켱ȣ���ܷ����(�ż�)
#define	SHX5	"095"		// 5���켱ȣ���ܷ����(�ż�)
#define	SHX6	"096"		// 6���켱ȣ���ܷ����(�ż�)
#define	SHX7	"097"		// 7���켱ȣ���ܷ����(�ż�)
#define	SHX8	"098"		// 8���켱ȣ���ܷ����(�ż�)
#define	SHX9	"099"		// 9���켱ȣ���ܷ����(�ż�)
#define	SHXA	"100"		// 10���켱ȣ���ܷ����(�ż�)

#define	DHCJ	"101"		// �켱ȣ�����ܷ�(�ŵ�)
#define	DHTJ	"102"		// �ð��ܸŵ�����
#define	DHCC	"103"		// �켱ȣ�����ܷ��Ǽ�(�ŵ�)
#define	DHXX	"104"		// ���ܷ��������(�ŵ�)
#define	DHTX	"105"		// �ŵ��ð��ܴ��

#define	SHCJ	"106"		// �켱ȣ�����ܷ�(�ż�)
#define	SHTJ	"107"		// �ð��ܸż�����
#define	SHCC	"108"		// �켱ȣ�����ܷ��Ǽ�(�ż�)
#define	SHXX	"109"		// ���ܷ��������(�ż�)
#define	SHTX	"110"		// �ż��ð��ܴ��

#define	SYCP	"111"		// ����ü�ᰡ
#define	SYCS	"112"		// ����ü�����

#define	YSCA	"111"		/* ����ü�ᰡ			*/
#define	YSCV	"112"		/* �����ֹ�ü�����		*/
#define	YSDB	"113"		/* ������   (���簡����)	*/
#define	YSRT	"114"		/* �������� (���簡����)	*/
#define	YSD2	"115"		/* ������   (������������) 	*/
#define	YSR2	"116"		/* �������� (������������)	*/
#define	MDVL	"118"		/* �����ŵ�ü�ᷮ		*/
#define	SDVL	"119"		/* �����ż�ü�ᷮ		*/
#define	DVOL	"20"		/* �ŵ�ü�����			*/
#define	SVOL	"19"		/* �ż�ü�����			*/


//****************************************************************
// elw �ΰ��� ���� SYMBOL		 
//****************************************************************
#define	SNJBS	"271"		// ���纯����(I.V)
#define	SDLTA	"272"		// ��Ÿ (���̷а�)
#define	SBEGA	"273"		// ����	(���̷а�)
#define	SGRYC	"274"		// �̷а�
#define	SLPPO	"275"		// LP��������
#define	STHTA	"276"		// ��Ÿ (���̷а�)
#define	SGAMA	"277"		// ���� (���̷а�)
#define	SRHOO	"278"		// ��	(���̷а�)
#define SDLT2	"279"		// ��Ÿ2(�����ϱ��� ���̷а�)

//****************************************************************
// piar �Ÿż���SYMBOL		 
//****************************************************************
#define	SACCN	"271"		// ���¹�ȣ
#define	SARBT	"272"		// ARBT_IDX
#define	SJQTY	"273"		// �ܰ�
#define	SPAMT	"274"		// ���Աݾ�
#define	STAMT	"275"		// �Ÿż���


#define	DTN1	"121"		// �ŵ��ŷ�����1
#define	DTN2	"122"		// �ŵ��ŷ�����2
#define	DTN3	"123"		// �ŵ��ŷ�����3
#define	DTN4	"124"		// �ŵ��ŷ�����4
#define	DTN5	"125"		// �ŵ��ŷ�����5


//****************************************************************
// �ŷ��� ���� SYMBOL		 
//****************************************************************
#define	DTN1	"121"		// �ŵ��ŷ�����1
#define	DTN2	"122"		// �ŵ��ŷ�����2
#define	DTN3	"123"		// �ŵ��ŷ�����3
#define	DTN4	"124"		// �ŵ��ŷ�����4
#define	DTN5	"125"		// �ŵ��ŷ�����5

#define	DTV1	"131"		// �ŵ��ŷ�������1
#define	DTV2	"132"		// �ŵ��ŷ�������2
#define	DTV3	"133"		// �ŵ��ŷ�������3
#define	DTV4	"134"		// �ŵ��ŷ�������4
#define	DTV5	"135"		// �ŵ��ŷ�������5

#define	DTR1	"141"		// �ŵ��ŷ�������1 (����� 999V99)
#define	DTR2	"142"		// �ŵ��ŷ�������2 (����� 999V99)
#define	DTR3	"143"		// �ŵ��ŷ�������3 (����� 999V99)
#define	DTR4	"144"		// �ŵ��ŷ�������4 (����� 999V99)
#define	DTR5	"145"		// �ŵ��ŷ�������5 (����� 999V99)

#define	STN1	"151"		// �ż��ŷ�����1
#define	STN2	"152"		// �ż��ŷ�����2
#define	STN3	"153"		// �ż��ŷ�����3
#define	STN4	"154"		// �ż��ŷ�����4
#define	STN5	"155"		// �ż��ŷ�����5

#define	STV1	"161"		// �ż��ŷ�������1
#define	STV2	"162"		// �ż��ŷ�������2
#define	STV3	"163"		// �ż��ŷ�������3
#define	STV4	"164"		// �ż��ŷ�������4
#define	STV5	"165"		// �ż��ŷ�������5

#define	STR1	"171"		// �ż��ŷ�������1 (����� 999V99)
#define	STR2	"172"		// �ż��ŷ�������2 (����� 999V99)
#define	STR3	"173"		// �ż��ŷ�������3 (����� 999V99)
#define	STR4	"174"		// �ż��ŷ�������4 (����� 999V99)
#define	STR5	"175"		// �ż��ŷ�������5 (����� 999V99)

#define	STVS	"181"		// �ż��ŷ���������
#define	STRS	"183"		// �ż��ŷ��������� (����� 999V99)
#define	DTVS	"186"		// �ŵ��ŷ���������
#define	DTRS	"188"		// �ŵ��ŷ��������� (����� 999V99)

//****************************************************************
// �����ü�����			 
//****************************************************************
#define	MGJY	"201"		// �̰��� ����
#define	GIRG	"202"		// �̷� ����
#define	IBAS	"203"		// �̷� BASIS
#define	SBAS	"204"		// ���� BASIS
#define	MDIF	"206"		// �̰��� ���� ���

#define	DHC1	"211"		// 1���켱ȣ���Ǽ�(�ŵ�)
#define	DHC2	"212"		// 2���켱ȣ���Ǽ�(�ŵ�)
#define	DHC3	"213"		// 3���켱ȣ���Ǽ�(�ŵ�)
#define	DHC4	"214"		// 4���켱ȣ���Ǽ�(�ŵ�)
#define	DHC5	"215"		// 5���켱ȣ���Ǽ�(�ŵ�)
#define	DHC6	"216"		// 6���켱ȣ���Ǽ�(�ŵ�)
#define	DHC7	"217"		// 7���켱ȣ���Ǽ�(�ŵ�)
#define	DHC8	"218"		// 8���켱ȣ���Ǽ�(�ŵ�)
#define	DHC9	"219"		// 9���켱ȣ���Ǽ�(�ŵ�)
#define	DHCA	"220"		// 10���켱ȣ���Ǽ�(�ŵ�)

#define	SHC1	"221"		// 1���켱ȣ���Ǽ�(�ż�)
#define	SHC2	"222"		// 2���켱ȣ���Ǽ�(�ż�)
#define	SHC3	"223"		// 3���켱ȣ���Ǽ�(�ż�)
#define	SHC4	"224"		// 4���켱ȣ���Ǽ�(�ż�)
#define	SHC5	"225"		// 5���켱ȣ���Ǽ�(�ż�)
#define	SHC6	"226"		// 6���켱ȣ���Ǽ�(�ż�)
#define	SHC7	"227"		// 7���켱ȣ���Ǽ�(�ż�)
#define	SHC8	"228"		// 8���켱ȣ���Ǽ�(�ż�)
#define	SHC9	"229"		// 9���켱ȣ���Ǽ�(�ż�)
#define	SHCA	"230"		// 10���켱ȣ���Ǽ�(�ż�)

#define SHSC	"112"		// ü�Ῡ�� "" or ���簡
//******************************* *********************************
// �ɼǽü�����			 
//****************************************************************
#define	OGRYC	"205"		// ������(���)	
#define	ODLTA	"243"		// ��Ÿ (���̷а�)
#define	OGAMA	"244"		// ���� (���̷а�)
#define	OTHTA	"245"		// ��Ÿ (���̷а�)
#define	OBEGA	"246"		// ����	(���̷а�)
#define	ORHOO	"247"		// ��	(���̷а�)
#define ODLT2	"248"		// ��Ÿ2(�����ϱ��� ���̷а�)
#define	ONJBS	"250"		// ���纯����(I.V)
//****************************************************************
// ���� SYMBOL:300�� ����					  
//****************************************************************
#define HSJJC	"117"		// ������ġ
				//	0:����
				//	1:�ŷ�����
				//	2:�ŷ��ߴ�

#define	HCODX	"301"		// �����ڵ�(6):A#####
#define	HCODF	"302"		// �����ڵ�, Full Code
#define	HCODR	"303"		// �����ڵ�, Real Code
#define	HENAM	"304"		// ������

#define	HSYMB	"305"		// �ֱǽɺ�
#define	HSGUB	"306"		// �Ҽӱ���
#define	HLGUB	"307"		// ������
#define	HTJYU	"308"		// ��������
#define	HGGGB	"309"		// ����/��������
#define	HJBGA	"310"		// �ں��ݱԸ�

#define	HUPMG	"320"		// �����ߺз�
#define	HUPSG	"321"		// �����Һз�
#define	HSULG	"322"		// ���������з�
#define	HSUMG	"323"		// ��������ߺз�
#define	HSUSG	"324"		// ��������Һз�

#define	HJJGN	"325"		// ����������
#define	HK2UP	"326"		// K200�����ڵ�	
#define	HK2K5	"327"		// K200,KQ50����
#define	HK1K5	"328"		// K100/K50����
#define	HJTGN	"329"		// ������ű���
#define	HKYCD	"330"		// �迭���ڵ�,000:��迭��
#define	HJMGR	"331"		// ����׷��ȣ(1-10)
#define	HSJGN	"332"		// ���屸��(���񱸺�)
#define	HJBGY	"333"		// �ں���(����:��)
#define	HSJNM	"334"		// �����ֽļ�(����:��)
#define	HGMON	"335"		// ����
#define	HGJJG	"336"		// �����ְ�(����:��)
#define	HYMGA	"337"		// �׸鰡(����:��)
#define	HDYGA	"338"		// ��밡(����:��)
#define	HSYGB	"339"		// �ſ밡�ɱ���
#define	HMMGB	"340"		// �Ÿű���
#define	HSTSY	"341"		// �ŷ���������(�ŷ�������)
#define	HDLJS	"342"		// ����ּ���
#define	HJSTD	"343"		// �����ŸŰ�����
#define	HJEND	"344"		// �����Ÿ�������
#define	HSJYD	"345"		// ����(����)��		YYYYMMDD
#define	HSJCD	"346"		// ����������		YYYYMMDD
#define	HBDGD	"347"		// �������		YYYYMMDD
#define	HMMSD	"348"		// �ŸŰ�����		YYYYMMDD
#define	HCLHG	"349"		// ����ȣ��
#define	HCHHG	"350"		// �ְ�ȣ��
#define	HPGGA	"351"		// �򰡰���
#define	HHSGA	"352"		// ��簡��
#define	HBSGY	"353"		// �Ҽ��ǰ��ÿ���
#define	HKQDG	"354"		// KQ����ϱ���(������)
#define	HKQGG	"355"		// KQ��걸��(������)
#define	HKQJS	"356"		// KQ�������� ���� ����
#define	HKQGN	"357"		// KQ���� �׷��ȣ
#define	HKQSG	"358"		// KQ���ʰ� ���ⱸ��
#define	HSSDD	"359"		// ������(������)
#define	HBTND	"360"		// BATCH���ž��̰�����ϼ�

#define	HJMTS	"370"		// ����Ư�̻���
#define	HJJGB	"371"		// �屸��
#define	HUPGB	"372"		// ��������
#define	HSHSL	"373"		// �Ű�,������
#define	HLJGN	"374"		// ������,�ں��ݱԸ�
#define	HKRUG	"375"		// �ŷ�����,�����з�
#define	HTJYJ	"376"		// ��������,�屸��
#define	HJMSD	"377"		// �ֹ���������

#define	HSHGA	"400"		// ���Ѱ�
#define	HLHGA	"401"		// ���Ѱ�
#define	HJGSJ	"402"		// �屸��,������ġ
#define	HSGCY	"403"		// �ð��Ѿ�
#define	HHGDY	"404"		// ȣ������
#define	HMDCG	"405"		// �ŵ�ü�ᤤ
#define	HMDCS	"406"		// �ŵ�ü�����
#define	HMDCB	"407"		// �ŵ�ü�����
#define	HMSCG	"408"		// �ż�ü��Ǽ�
#define	HMSCS	"409"		// �ż�ü�����
#define	HMSCB	"410"		// �ż�ü�����
#define	HJJGA	"411"		// ��������
#define	HJJVO	"412"		// ���ϰŷ���
#define	HJJDA	"413"		// ���ϰŷ����(�鸸)
#define	HGADB	"414"		// �ŷ���������(���ϵ��ð�����)
#define	HYJGS	"415"		// �ܱ����ֹ����ɼ���
#define	HYHSR	"416"		// �ܱ����ѵ�����
#define	HYJHB	"417"		// �ܱ����ֹ��ѵ�����
#define	HYBYB	"418"		// �ܱ��κ�������
#define	HYBCB	"419"		// �ܱ���������
#define	HYDSS	"420"		// �ܱ��δ��ϼ��ż�
#define	HYDMS	"421"		// �ܱ��δ��ϸż�����
#define	HYDMD	"422"		// �ܱ��δ��ϸŵ�����
#define	HDRPK	"425"		// �����
#define	HJHPK	"426"		// ������

#define	HWCHG	"431"		// 52���ְ�
#define	HWCLG	"432"		// 52��������
#define	HYCHG	"433"		// �����ְ�
#define	HYCLG	"434"		// ����������
#define	HWCHV	"435"		// 52�� �ְ�ŷ���
#define	HWCLV	"436"		// 52�� �����ŷ���
#define	HYCHV	"437"		// �����ְ�ŷ���
#define	HYCLV	"438"		// ���������ŷ���
#define	HWCHGD	"441"		// 52���ְ���
#define	HWCLGD	"442"		// 52����������
#define	HYCHGD	"443"		// �����ְ���
#define	HYCLGD	"444"		// ������������
#define	HWCHVD	"445"		// 52���ְ�ŷ�����
#define	HWCLVD	"446"		// 52�������ŷ�����
#define	HYCHVD	"447"		// �����ְ�ŷ�����
#define	HYCLVD	"448"		// ���������ŷ�����

#define	HYSSH	"450"		// ���ӻ����ϼ�
#define	HYSSS	"451"		// ���ӻ���ϼ�
#define	HYSHH	"452"		// ���������ϼ�
#define	HYSHL	"453"		// �����϶��ϼ�

#define	HBETA	"455"		// ��Ÿ���
#define	HPCR	"456"		// PCR
#define	HPBR	"457"		// PBR
#define	HEPSS	"458"		// EPS_����
#define	HEPSM	"459"		// EPS_�ݱ�
#define	HPERS	"460"		// PER_����
#define	HPERM	"461"		// PER_�ݱ�
#define	HBSYR	"462"		// ��������
#define	HJSYY	"463"		// �ִ������
#define	HJSJS	"464"		// �ִ���ڻ�
#define	HJSCF	"465"		// �ִ�CashFlow
#define	HJSMC	"466"		// �ִ�����
#define	HEVTA	"467"		// EV/EVITA
#define	HSYBY	"468"		// �ſ��ܰ����
#define	HSYJG	"469"		// �ſ��ܰ�ݾ�

//****************************************************************
// ���� SYMBOL:300�� ����					  
//****************************************************************
#define FFFCG	"256"		// �ְ�����Ƚ��
#define FFFCJ	"257"		// ����������Ƚ��

#define	FCODX	"301"		// �����ڵ�(����)
#define	FCODF	"302"		// �����ڵ�FULL
#define	FCODS	"303"		// ���� SEQ��ȣ	
#define	FCOD2	"304"		// �����ڵ�(����)
#define	FENAM	"305"		// ���������
#define	FENMS	"306"		// ���������(Short)
#define	FHANM	"307"		// �����

#define	FSJYJ	"310"		// ������
#define	FSJYS	"311"		// �����ϼ�
#define	FGJYS	"312"		// �����ϼ�(MT DATA)
#define	FJJYS	"313"		// �����ϼ�
#define	FFGRD	"314"		// ���ʰŷ���(MT DATA)
#define	FLGRD	"315"		// �����ŷ���
#define	FLGRY	"316"		// �����ŷ��Ͽ���
#define	FBDYM	"317"		// ���������̷���ġ	999V999999
#define	FGJGB	"318"		// ���ذ��ݱ���
#define	FGJGA	"319"		// ���ذ���		99999V99
#define	FIRGJ	"320"		// �̷а�(������)	99999V99
#define	FIRGB	"321"		// �̷а�(���ذ�)	99999V99
#define	FCDGR	"322"		// CD�ݸ�  		999V99
#define	FJSGB	"323"		// ���갡�ݱ���
#define	FJJSG	"324"		// �������갡��		99999V99
#define	FJJGB	"325"		// ������������
#define	FJCSR	"326"		// ����ü�����(����:���)
#define	FJCAM	"327"		// ����ü����(����:���)
#define	FJMGJ	"328"		// ���Ϲ̰�����������
#define	FSJHG	"329"		// �������ְ�		9999V99
#define	FHGYJ	"330"		// �ְ�����
#define	FSJLG	"331"		// ������������		9999V99
#define	FLGYJ	"332"		// ����������
#define	FYHYJ	"333"		// ��ȸ����
#define	FSJYN	"334"		// ���尡��뱸��

#define	FSJHY	"335"		// ���尡�����
#define	FSJHT	"336"		// ���尡���ð�
#define	FCJGG	"337"		// ������������		9999V99
#define	FCJGB	"338"		// �����������ݱ���
#define	FGRYG	"339"		// ����������(MT DATA)
#define	FGRYD	"340"		// ������(MT DATA)
#define	FCBHG	"341"		// CB������Ѱ�		9999V99
#define	FCBLG	"342"		// CB�������Ѱ�		9999V99
#define	FSSDD	"343"		// ����������
#define	FBTND	"344"		// BATCH���̰�����ϼ�

#define	FK200	"352"		// K200����
#define	FGRYC	"205"		// ������(���)

#define	FSHGA	"354"		// ���Ѱ�		99999V99
#define	FLHGA	"355"		// ���Ѱ�		99999V99
#define	FJJGA	"356"		// ��������		99999V99
#define	FDRPK	"357"		// �����(���Ѱ�-���Ѱ�)

#define	FCGGB	"400"		// ü�ᱸ��
				// �ż� : +
				// �ŵ� : -

#define	FJYGB	"401"		// ���뱸��
#define	FMGCY	"402"		// �̰���ü������
#define	FMGGB	"403"		// �̰�����������
#define	FGSGA	"404"		// ���갡��
#define	FGSGB	"405"		// ���걸��
#define	FCJKG	"406"		// ����������
#define	FCJKB	"407"		// ��������������
#define	FHGDY	"408"		// ȣ������
#define	FGMUL	"411"		// �ŷ��¼�
#define JSTAT	"510"		// ����±���
#define	YCPRC	"511"		// ����ü��
#define	YDIFF	"512"		// ����ü����

//****************************************************************
// �ɼ� SYMBOL:300�� ����					  
//****************************************************************
#define	OCODX	"301"		// �����ڵ�
#define	OCODF	"302"		// �����ڵ�(FULL)
#define	OIBYJ	"303"		// ��ȸ����
#define	OENAM	"305"		// ���������
#define	OHNAM	"306"		// �ѱ������(Short)
#define	OOSGB	"307"		// �ɼǱ���
#define	ODJGB	"308"		// �ŷ�������񱸺�
#define	OMGYM	"309"		// ������
#define	OATMG	"310"		// ��簡�� 9999V99
#define	OCODS	"311"		// ���� SEQ��ȣ	
#define	OYHGB	"312"		// �Ǹ������������
#define	OGJYS	"313"		// �����ϼ�
#define	OJJYS	"314"		// �����ϼ�
#define	OYMGB	"315"		// �ֱٿ�������
#define	OATGB	"316"		// ATM����
#define	OSJYJ	"317"		// ������
#define	OFGRD	"318"		// ���ʰŷ���
#define	OLGRD	"319"		// �����ŷ���
#define	OLGGB	"320"		// �����ŷ�����
#define	ONARG	"321"		// �ű�/�߰�/���� ����
#define	ONAYJ	"322"		// �ű�/�߰� ����
#define	OBDHG	"323"		// �����������簡ġ	999V999999
#define	OIRGJ	"324"		// �̷а�(���簡)	9999V999999
#define	OMMJG	"325"		// �Ÿ����űݱ��ذ�	9999V99
#define	OMMGB	"326"		// �Ÿ����űݱ��ذ�����
#define	OSHGA	"327"		// ���Ѱ�		9999V99
#define	OLHGA	"328"		// ���Ѱ�		9999V99
#define	OGDGJ	"329"		// ���ݴ������ذ�(3.00)
#define	OHGD1	"330"		// ȣ������(0.01)
#define	OHGD5	"331"		// ȣ������(0.05)
#define	OCDGR	"332"		// CD�ݸ�		999V999
#define	OMGCS	"333"		// �̰���ü�����
#define	OJYSG	"334"		// ���Ͻð�
#define	OJYHG	"335"		// ���ϰ�
#define	OJYJG	"336"		// ��������
#define	OJJGB	"337"		// ������������
#define	OJJVO	"338"		// ���ϰŷ���
#define	OJJDA	"339"		// ���ϰŷ����(���)
#define	OYHYJ	"340"		// �����ְ�����
#define	OYHGA	"341"		// �����ְ���
#define	OYLYJ	"342"		// ������������
#define	OYLGA	"343"		// ������������
#define	OSJYN	"344"		// ���尡��뱸��
#define	OSJHY	"345"		// ���尡�����������
#define	OSJHT	"346"		// ���尡�������ð�
#define	OGJGA	"347"		// ���ذ���		9999V99
#define	OGJGB	"348"		// ���ذ��ݱ���
#define	OSJHG	"349"		// �����ְ���		9999V99
#define	OSJHJ	"350"		// �����ְ�����
#define	OSJLG	"351"		// ������������		9999V99
#define	OSJLJ	"352"		// ������������
#define	OUPDT	"353"		// Update Date
#define	ONJBD	"354"		// ���簡ġ������
#define	OAGHD	"355"		// �ڵ��Ǹ�����󱸺�
#define	OJJYB	"356"		// �ֿ����񿩺�
#define	OGGYS	"357"		// ����ϼ�(if-1, deleted)
#define	OJSEQ	"358"		// ��ȸSEQ��ȣ
#define	OJJGA	"359"		// ��������
#define	ODRPK	"370"		// �����

#define	OCGGB	"401"		// ü�ᱸ��
				// �ż� : +
				// �ŵ� : -
#define	OJYGB	"402"		// ���뱸��
#define	OMGCY	"403"		// �̰���ü������
#define	OMGGB	"404"		// �̰�����������
#define	OGSGA	"405"		// ���갡��
#define	OGSGB	"406"		// ���갡����
#define	OCJGK	"407"		// ����������
#define	OCJGB	"408"		// ��������������
#define	OGMUL	"411"		// �ŷ��¼�
#define	OK200	"426"		// K200����
#define	OFUMN	"427"		// �����ֱٿ�
#define	OHGDY	"433"		// ȣ������


//****************************************************************
// ���� SYMBOL : MSUP				  
//****************************************************************
#define	UDHG1	"041"		// �ŵ� 1ȣ�� ����
#define	UDHG2	"042"		// �ŵ� 2ȣ�� ����
#define	USHG1	"046"		// �ż� 1ȣ�� ����
#define	USHG2	"047"		// �ż� 2ȣ�� ����

#define	UFHBS	"050"		// ���� ���簡 ���̽ý�
#define	UHDB1	"051"		// �ŵ� 1ȣ�� ���� ���簡 ���̽ý�
#define	UHDB2	"052"		// �ŵ� 2ȣ�� ���� ���簡 ���̽ý�
#define	UHSB1	"056"		// �ż� 1ȣ�� ���� ���簡 ���̽ý�
#define	UHSB2	"057"		// �ż� 2ȣ�� ���� ���簡 ���̽ý�
#define	UFJBS	"060"		// ���� ����ȣ�� ���̽ý�
#define	UJDB1	"061"		// �ŵ� 1ȣ�� ���� ����ȣ�� ���̽ý�
#define	UJDB2	"062"		// �ŵ� 2ȣ�� ���� ����ȣ�� ���̽ý�
#define	UJSB1	"066"		// �ż� 1ȣ�� ���� ����ȣ�� ���̽ý�
#define	UJSB2	"067"		// �ż� 2ȣ�� ���� ����ȣ�� ���̽ý�
#define	UFSBS	"070"		// ���� ���ȣ�� ���̽ý�
#define	USDB1	"071"		// �ŵ� 1ȣ�� ���� ���ȣ�� ���̽ý�
#define	USDB2	"072"		// �ż� 2ȣ�� ���� ���ȣ�� ���̽ý�
#define	USSB1	"076"		// �ŵ� 1ȣ�� ���� ���ȣ�� ���̽ý�
#define	USSB2	"077"		// �ż� 2ȣ�� ���� ���ȣ�� ���̽ý�

#define	UCODX	"301"		// �����ڵ�(2�ڸ�)
#define	UUPGB	"302"		// ���屸��
#define	UUPCD	"303"		// ���屸��+�����ڵ�
#define	UHANM	"304"		// �ѱ۸�
#define	UENAM	"305"		// ������
#define	UJYJS	"306"		// ��������

//****************************************************************
// �ֽĿɼ� SYMBOL:300�� ����					  
//****************************************************************
#define	HOMUL	"411"		// �ŷ��¼�
#define	HOCDX	"499"		// �����ڻ��ֽ��ڵ�
#define	HOKDY	"500"		// �ŷ��¼�
#define OSJBS	"210"		// �ֽĿɼ� ���纯����

//****************************************************************
// �ֽļ��� SYMBOL:300�� ����					  
//****************************************************************
#define SFGJM   "409"		// ������
#define SFSJJ	"410"		// ��������
#define	SFMUL	"411"		// �ŷ��¼�
#define	SFCDX	"499"		// �����ڻ��ֽ��ڵ�
#define	SFKDY	"500"		// �ŷ��¼�


//****************************************************************
// ���� ü�� ���� GRID						  
// �ߺз� : MGST���						  
//****************************************************************
// GRID SYMBOL
#define	HGRCH	"310"		// �Ϲ�ü�᳻��
#define	HGRGN	"410"		// ���ݺ���ü�᳻��

// GRID COLUMN SYMBOL
#define	HGRCG	"023"		// ü�ᰡ
#define	HGRDB	"024"		// ���ϴ��
#define	HGRMD	"025"		// �ŵ�ȣ��
#define	HGRMS	"026"		// �ż�ȣ��
#define	HGRCV	"032"		// ü�ᷮ
#define	HGRVL	"027"		// �����ŷ���
#define	HGRDY	"033"		// �����
#define	HGRCT	"034"		// ü��ð�

// �Ϲ� OUTPUT SYMBOL
#define	HGRBA	"305"		// ����/����

//****************************************************************
// ���� ü�� ���� GRID						  
// �ߺз� : MGFU���						  
//****************************************************************
// GRID SYMBOL
#define	FGRCH	"310"		// ����ü�᳻��

// GRID COLUMN SYMBOL
#define	FGRCG	"023"		// ü�ᰡ
#define	FGRDB	"024"		// ���ϴ��
#define	FGRMD	"025"		// �ŵ�ȣ��
#define	FGRMS	"026"		// �ż�ȣ��
#define	FGRVL	"027"		// �����ŷ���(����:���)
#define	FGRAM	"028"		// �����ŷ����(����:�鸸��)
#define	FGRCV	"032"		// ü�ᷮ
#define	FGRDY	"033"		// �����
#define	FGRTM	"034"		// �ð�
#define	FGRGB	"035"		// ü�ᱸ��

// �Ϲ� OUTPUT SYMBOL
#define	FGRBA	"305"		// ����/����

//****************************************************************
// �ɼ� ü�� ���� GRID						  
// �ߺз� : MGOP���						  
//****************************************************************
// GRID SYMBOL
#define	OGRCH	"310"		// ����ü�᳻��

// GRID COLUMN SYMBOL
#define	OGRCG	"023"		// ü�ᰡ
#define	OGRDB	"024"		// ���ϴ��
#define	OGRMD	"025"		// �ŵ�ȣ��
#define	OGRMS	"026"		// �ż�ȣ��
#define	OGRVL	"027"		// �����ŷ���(����:���)
#define	OGRAM	"028"		// �����ŷ����(����:�鸸��)
#define	OGRCV	"032"		// ü�ᷮ
#define	OGRDY	"033"		// �����
#define	OGRTM	"034"		// �ð�
#define	OGRGB	"035"		// ü�ᱸ��
#define	OGRMG	"201"		// �̰�����������(����:���)

// �Ϲ� OUTPUT SYMBOL
#define	OGRBA	"305"		// ����/����

//****************************************************************
// ü��+�ٽ���
//****************************************************************
#define	CCODX	"021"		// �����ڵ�
#define	CHNAM	"022"		// �����
#define	CCURR	"023"		// ���簡
#define	CMDGA	"051"		// �ŵ�ȣ��
#define	CMDHJ	"041"		// �ŵ��ܷ�
#define	CMSGA	"071"		// �ż�ȣ��
#define	CMSHJ	"061"		// �ż��ܷ�

#define	CGUBN	"900"		// RTS ����
				//	A:�ٽ��� ����
				//	B:�ٽ��� �ֹ���������
				//	C:�ٽ��� �ֹ�ü��
				//	D:���� �ֹ� ü��
				//	K:���º� ���񱸺к� FEE�۽�


#define	CUSER	"901"		// �Ƿ��� �� ������ID
#define	CBNUM	"902"		// �ٽ��Ϲ�ȣ

#define	CJMBN	"903"		// �ֹ���ȣ
				//	�ֽ�	:5
				//	�����ɼ�:6

#define	CFDID	"904"		// �ݵ�ID(usid+fdid)
#define	CJUID	"905"		// �ֹ���ID
#define	CJMGB	"906"		// �ֹ� ����
				//	1:���ֹ�
				//	2:�����ֹ�
				//	3:����ֹ�

#define	CACCN	"907"		// ���¹�ȣ(�Ϲ�)
#define	CANAM	"908"		// ���¸�
#define	CMDMS	"909"		// �ŵ�/�ż� ����
				//	�ŵ�:1
				//	�ż�:2

#define	CJGSU	"910"		// �ܰ����
#define	CMYDG	"911"		// ������մܰ�
#define	CCHJS	"912"		// û���ֹ����ɼ���
#define	CFJNO	"913"		// �ݵ��ֹ���ȣ
#define	CJMGA	"914"		// �ֹ�����
#define	CJMSU	"915"		// �ֹ�����
#define	CCHGA	"916"		// ü��ܰ�
#define	CCHSU	"917"		// ü�����
#define	CMCHS	"918"		// ��ü�����
#define	CFJCS	"919"		// �����ֹ�ü�����
				//	01:�ֹ�����
				//	02:ü����
				//	03:ü��Ϸ�
				//	04:��������
				//	05:�����Ϸ�
				//	06:�������
				//	07:��ҿϷ�
				//	08:�ź�

#define	COJMN	"920"		// ���ֹ���ȣ
#define	CJMYH	"921"		// �ֹ�����
				//	01:������
				//	02:���尡
				//	03:���Ǻ�������
				//	04:������������

#define	CJJSR	"922"		// ��������
#define	CCSSR	"923"		// ��Ҽ���
#define	CGBSR	"924"		// �źμ���
#define	CMDMC	"925"		// �ŵ���ü�����
#define	CMSMC	"926"		// �ż���ü�����
#define	CRAT1	"927"		// ü���(ü�����/�ֹ�����)
#define	CRAT2	"928"		// ü���(100 - (��ü�����/�ֹ�����*100) => [999V9]
				// #��ü��ݾ� : ���尡 �ֹ��ΰ�� �ż�(���Ѱ�), �ŵ�(���Ѱ�)
#define	CKRDY	"930"		// �ŷ�����(�ܰ� �����ɼ�)
#define	CCTIM	"931"		// ü��ð�(HHMMSS)
#define	CENAM	"932"		// ����� ����(Short)
#define	CDMSI	"933"		// ���� �Ÿ�(û��)����
#define	CCNUM	"934"		// ü���Ϸù�ȣ
#define	CMYKY	"935"		// ���Աݾ�
#define	CJGBN	"936"		// �ֹ�����
				//	1:�ٽ���
				//	3:�Ϲ��ֹ�, 
				//	4:�ݵ��ֹ�
				//	5:�ý���
				//	6:�����Ÿ�����

#define CTCOD	"937"		// ����ڵ�

////////////////////////////////////////////////////////////////
#define CMDCQ	"938"		// �ŵ�ü�᰹��(tfdchqt)
#define CMSCQ	"938"		// �ż�ü�᰹��(tfdchqt)
#define CMDSN	"939"		// i_tfdchqt->msqt - i_tfdchqt->msqt
//****************************************************************
// V2 ȣȯ�� ����( EU2S ������)
//#define	CMDCQ	CMDCQ		// �ŵ�ü��Ǽ�
//#define	CMSCQ	CMDSN		// �ż�ü��Ǽ�
//****************************************************************


#define	CQSBP	"940"		// �Ǻ� ü��ܰ� (�����ɼ��� �ֹ������� ü��ÿ��� �Ǻ�)
#define CQSBN	"941"		// �Ǻ� ü����� (�����ɼ��� �ֹ������� ü��ÿ��� �Ǻ�)
#define CJMCT	"942"		// �ֹ�����
				//	0:�Ϲ�
				//	1:IOC
				//	2:FOK
//****************************************************************
// V2 ȣȯ�� ���� (EU2S ������)
#define	CNETQ	CQSBP		// NET	
#define CMDCC	CQSBN		// �����ֹ��ŵ�ü��Ǽ�
#define CMSCC	CJMCT		// �����ֹ��ż�ü��Ǽ�
//****************************************************************




#define CJOGB	"943"		// ���񱸺�
				//	1:KOSPI
				//	2:KOSDAQ
				//	3:ETFinKOSPI
				//	4:ETFinKOSDAQ
				//	5:ELW

#define CFDON	"946"		// �ݵ� �ֹ���ȣ
#define	CKIND	"947"		// RTS ������
				// D:���� �ֹ�ü��
				// E:�����ܰ�
				// F:�����ɼ� �ֹ�ü��
				// G:�����ɼ��ܰ�
				// J:�ݵ庰 SUM����
				// H:CD�ֹ��� SUM����
				// I:CD�ֹ��� ����ں���
				// K:���º� ���񱸺к� FEE�۽�
				// L:CD�ֹ� �ú��� ó�� ��������
				// M:OCIMS ����͸�

#define	CBJSS	"950"		// ���߼���
#define	CGBHN	"951"		// (�ѱ�2�ڸ�-4Byte)
				// �ŵ�
				// �ż�
				// ����
				// ���

#define	CGSBJ	"952"		// ��������(999V999)
#define	CBSKN	"953"		// �ٽ��ϸ�
#define	CBSGB	"954"		// �ٽ����ֹ�����(�ű�/û�� + �ŵ�/�ż�)
#define	CBJGA	"955"		// �ٽ��� �ֽ� �ֹ��ݾ�
#define	CBCGA	"956"		// �ٽ��� �ֽ� ü��ݾ�
#define	CBJCH	"957"		// �ٽ��� �ֽ� ü���
#define	CBFOJ	"958"		// �ٽ��� ���� �ֹ�����
#define	CBFOC	"959"		// �ٽ��� ���� ü�����
#define	CBFOR	"960"		// �ٽ��� ���� ü��ݾ�
				
#define	CBJMS	"961"		// �ٽ����ֹ�����
				//	1:���
				//	2:�ֹ�
				//	3:�Ϸ�

#define	CBJSR	"962"		// �ٽ��� �ڵ� �ֹ�����
#define	CBSCR	"963"		// �ٽ��� �ڵ� ü�����
#define	CBSSR	"964"		// �ٽ��� �ڵ� ��Ҽ���
#define	CBKSR	"965"		// �ٽ��� �ڵ� �źμ���
#define	CBCJS	"966"		// �ٽ��� �ݿɼ� �ֹ�����
#define	CBCCS	"967"		// �ٽ��� �ݿɼ� ü�����
#define	CBCCK	"968"		// �ٽ��� �ݿɼ� ü��ݾ�
#define	CBPJS	"969"		// �ٽ��� ǲ�ɼ� �ֹ�����
#define	CBPCS	"970"		// �ٽ��� ǲ�ɼ� ü�����
#define	CBPCK	"971"		// �ٽ��� ǲ�ɼ� ü��ݾ�
#define	CBFCD	"972"		// �ٽ��� �����ڵ�
#define	CBCCD	"973"		// �ٽ��� �ݿɼ� �ڵ�
#define	CBPCD	"974"		// �ٽ��� ǲ�ɼ� �ڵ�
#define	CBMCK	"975"		// �ٽ��� ��ü��ݾ�
#define	CBCKY	"976"		// �ٽ����ڵ� ü��ݾ�
#define	CBCMS	"977"		// �ٽ����ڵ� ��ü�����
#define	CBRAT	"978"		// �ٽ��� �����ɼ� ü���
#define	CBMCS	"979"		// �ٽ��� ��ü�����
#define	CBCSR	"980"		// �ٽ��� ü�����
#define	CBJJS	"981"		// �ٽ��� �ֹ�����
#define	CJMER	"982"		// �ٽ��� �ڵ� �ֹ�������ȣ
#define	COJMG	"983"		// �ٽ��� �����ֹ��ݾ�


//****************************************************************
// CD �ֹ� ����
//****************************************************************
#define CDOID	"985"		// CDID
#define CDSEQ	"986"		// CD SEQ
//****************************************************************
// V2 ȣȯ�� ���� (EU2S ��� ����)
#define	CHTCD	"984"		// ������
#define	CFTCD	CDOID		// V2 ȣȯ�� ���ؼ� ����
#define	COTCD	CDSEQ		// V2 ȣȯ�� ���ؼ� ����

//****************************************************************
// OCIMS ����͸�
//****************************************************************
#define	CRSEQ	"031"		// �������
#define CTEAM	"032"		// ���ڵ�
#define CJUGB	"033"		// ����ŷ� ���� 
				// 0:KOSDAQ 
				// 1:�峻�ŷ� 
				// 2:��ܴ���
				// 3:�������� 
				// 4:��������
				// 8:�����μ���
				// 9:��ܸŸ�
#define	CPMGB	"034"		// ���α׷�ȣ�� ����
				// 1: ���α׷�
				// 0: �Ϲ�
#define MIREA	"035"		// �̷����°��±���
//****************************************************************

#define CDORN	"987"		// CD�ֹ� ȸ��
#define CDCGB	"988"		// ���񱸺�
				//	1:����
				//	2:�ɼ�
				//	3:�����ֽĿɼ�
				//	4:������������
				//	5:�ֽļ���

#define CDFEE	"989"		// FEE ����
#define CDOAL	"990"		// �õ��ֹ� �� �ֹ����� ȸ��
#define CDORR	"991"		// CD�ֹ� ���� ȸ��
#define CDSOC	"992"		// CD�ֹ� �����ֹ� ����


#endif


