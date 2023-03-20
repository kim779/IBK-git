// ChartItem.cpp: implementation of the CChartItem_Stock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartItem_Stock.h"
#include "ChartDef_Stock.h"
#include "../MainFrmWnd.h"
#include "../../../DR_Include/PropertyMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

LPCTSTR szSetAllForSignalnew[] = 
{
	"EN=1",
	"HI=0",
	"ST=",
	"GS=",
	"LS=",
	"XCM=��ȣ��",
	"XID=",
	"CFT=����ü;12;0;0;0;0;",
	"TKS=1",
	"UDF=1",
	"OCN=NULL"
};

LPCTSTR szSetAll_Stock[] = 
{
// 	"CM=Name,SfnChart0",
// 	"AN=SfnChart0",
	"ID=3,4,0",
	"RC=2,24,397,248",
	"WS=WS_CHILD|WS_VISIBLE",
	"WSE=",
	"BT=0",
	"BC=0",
	"ROWC=1",
	"CC=1",
	"BG=-1",
	"BBT=1",
	"BDT=0",
	"SLT=1",							// ����(graph ���ý�)
	"WV=0",
	"PDC=100",
	"BAC=10",			// �ִ� ����
	"GDD=0",
	"LM=0",	// left ����
	"RM=0",	// right ����
	"TM=0",	// top ����
	"BM=0",	// bottom ����
	"AGM=1",												// ���ؽ�Ʈ �޴� 
	"EFN=",
	"IFN=",
	"SFN=",
	"HI=0",
	"IR=0",
	//"TO=����\r\n�����ͺ���;Graph������ ����;+/-�� �������� ǥ�þ���;\r\n��ġ��ȸâ;�˾� ������;��ġDialog ���;\r\n���ڼ�;��ġ��ȸâ ������;;\r\n�߼���;��ġ��ȸâ ������;;\r\n��������;;;\r\n��ũ�ѹ�;;;\r\nDisplay�� ������;;;\r\nȮ��,���;����;10��;\r\n"
	//"�ܺ�\r\n"
	//"���ڼ�;��ġ��ȸâ ������;;\r\n"
	//"RButton\r\n",
	"CL=BlockColumn;\r\n"
	"Block;\r\n"
	"�ڷ�����;1;1;/;:;0;0;0;1;\r\n"  // �ڷ�����;4;1 �� �ϸ� x�ེ������ �ȳ��´�.
	"VertScaleGroup;\r\n"
	"��ǥ Data;1;1;0;0;0;\r\n"
	"Graph;\r\n"
	";������Ʈ;G:������Ʈ;�ð�,��,����,����,;0:0;0;0;0;0;1;\r\n"
	"0;0;0;����;����;0;208,45,45;208,45,45;45,45,208;45,45,208;208,45,45;208,45,45;1;1;1;1;1;0;\r\n"
	"GraphEnd;\r\n"
	"Graph;\r\n"
	";���� �̵����;G:���� �̵����;����,;0:0;0;0;0;0;0;\r\n"
	"1;0;0;_PMA1_;5;0;202,7,2;202,7,2;202,7,2;202,7,2;202,7,2;202,7,2;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA2_;10;0;70,70,255;70,70,255;70,70,255;70,70,255;70,70,255;70,70,255;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA3_;20;0;222,149,8;222,149,8;222,149,8;222,149,8;222,149,8;222,149,8;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA4_;60;1;24,130,58;24,130,58;24,130,58;24,130,58;24,130,58;24,130,58;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA5_;120;1;149,151,148;149,151,148;149,151,148;149,151,148;149,151,148;149,151,148;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA6_;240;1;255,70,70;255,70,70;255,70,70;255,70,70;255,70,70;255,70,70;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA7_;250;1;156,48,47;156,48,47;156,48,47;156,48,47;156,48,47;156,48,47;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA8_;260;1;64,224,208;64,224,208;64,224,208;64,224,208;64,224,208;64,224,208;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA9_;280;1;145,91,255;145,91,255;145,91,255;145,91,255;145,91,255;145,91,255;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA10_;290;1;100,85,255;100,85,255;100,85,255;100,85,255;100,85,255;100,85,255;14;14;1;1;1;0;0;1;0;\r\n"

// 	"1;0;0;����1;5;0;0,0,255;0,0,255;0,0,255;0,0,255;0,0,255;0,0,255;1;1;0,0,255;0,0,255;0,0,255;0,0,255;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����2;10;1;77,149,249;77,149,249;77,149,249;77,149,249;77,149,249;77,149,249;1;1;77,149,249;77,149,249;77,149,249;77,149,249;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����3;20;0;255,0,255;255,0,255;255,0,255;255,0,255;255,0,255;255,0,255;1;1;255,0,255;255,0,255;255,0,255;255,0,255;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����4;30;1;255,128,128;255,128,128;255,128,128;255,128,128;255,128,128;255,128,128;1;1;255,128,128;255,128,128;255,128,128;255,128,128;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����5;40;1;232,197,55;232,197,55;232,197,55;232,197,55;232,197,55;232,197,55;1;1;232,197,55;232,197,55;232,197,55;232,197,55;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����6;60;1;0,0,128;0,0,128;0,0,128;0,0,128;0,0,128;0,0,128;1;1;0,0,128;0,0,128;0,0,128;0,0,128;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����7;80;1;115,115,0;115,115,0;115,115,0;115,115,0;115,115,0;115,115,0;1;1;115,115,0;115,115,0;115,115,0;115,115,0;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����8;90;1;155,106,255;155,106,255;155,106,255;155,106,255;155,106,255;155,106,255;1;1;155,106,255;155,106,255;155,106,255;155,106,255;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����9;100;1;67,154,8;67,154,8;67,154,8;67,154,8;67,154,8;67,154,8;1;1;67,154,8;67,154,8;67,154,8;67,154,8;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����10;120;1;255,0,0;255,0,0;255,0,0;255,0,0;255,0,0;255,0,0;1;1;255,0,0;255,0,0;255,0,0;255,0,0;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����11;150;1;0,0,128;0,0,128;0,0,128;0,0,128;0,0,128;0,0,128;1;1;0,0,128;0,0,128;0,0,128;0,0,128;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����12;180;1;255,0,255;255,0,255;255,0,255;255,0,255;255,0,255;255,0,255;1;1;255,0,255;255,0,255;255,0,255;255,0,255;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����13;200;1;0,128,64;0,128,64;0,128,64;0,128,64;0,128,64;0,128,64;1;1;0,128,64;0,128,64;0,128,64;0,128,64;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����14;210;1;255,153,0;255,153,0;255,153,0;255,153,0;255,153,0;255,153,0;1;1;255,153,0;255,153,0;255,153,0;255,153,0;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����15;240;1;255,133,11;255,133,11;255,133,11;255,133,11;255,133,11;255,133,11;1;1;255,133,11;255,133,11;255,133,11;255,133,11;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����16;250;1;0,136,0;0,136,0;0,136,0;0,136,0;0,136,0;0,136,0;1;1;0,136,0;0,136,0;0,136,0;0,136,0;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����17;270;1;24,62,188;24,62,188;24,62,188;24,62,188;24,62,188;24,62,188;1;1;24,62,188;24,62,188;24,62,188;24,62,188;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����18;300;1;138,0,255;138,0,255;138,0,255;138,0,255;138,0,255;138,0,255;1;1;138,0,255;138,0,255;138,0,255;138,0,255;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����19;330;1;67,154,8;67,154,8;67,154,8;67,154,8;67,154,8;67,154,8;1;1;67,154,8;67,154,8;67,154,8;67,154,8;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;����20;350;1;132,132,132;132,132,132;132,132,132;132,132,132;132,132,132;132,132,132;1;1;132,132,132;132,132,132;132,132,132;132,132,132;1;1;0;1;1;1;0;1;1;0;\r\n"
	"GraphEnd;\r\n"
	"VertScaleGroupEnd;\r\n"
	"BlockEnd;\r\n"
// 	"Block;\r\n"
// 	"�ڷ�����;1;2;/;:;0;0;0;0;\r\n"
// 	"VertScaleGroup;\r\n"
// 	"��ǥ Data;1;2;0;\r\n"
// 	"Graph;\r\n"
// 	";�ŷ�����Ʈ;�⺻�ŷ���,;0:0;0;0;0;0;0;\r\n"
// 	"2;0;0;�ŷ���;�ŷ���;0;0,128,64;0,128,64;0,128,64;0,128,64;0,128,64;0,128,64;1;1;1;1;1;0;\r\n"
// 	"GraphEnd;\r\n"
	
	/*
	"Graph;\r\n"
	";�ŷ��� �̵����;�⺻�ŷ���,;0:0;0;0;0;0;0;\r\n"
	"1;0;0;�ŷ�1;5;0;255,0,0;255,0,0;255,0,0;255,0,0;255,0,0;255,0,0;0;0;1;1;1;0;\r\n"
	"1;0;0;�ŷ�2;20;0;0,0,255;0,0,255;0,0,255;0,0,255;0,0,255;0,0,255;0;0;1;1;1;0;\r\n"
	"1;0;0;�ŷ�3;60;0;0,0,102;0,0,102;0,0,102;0,0,102;0,0,102;0,0,102;0;0;1;1;1;0;\r\n"
	"1;0;0;�ŷ�4;80;1;255,0,255;255,0,255;255,0,255;255,0,255;255,0,255;255,0,255;1;1;1;1;1;0;\r\n"
	"1;0;0;�ŷ�5;120;1;153,0,204;153,0,204;153,0,204;153,0,204;153,0,204;153,0,204;1;1;1;1;1;0;\r\n"
	"1;0;0;�ŷ�6;160;1;255,153,0;255,153,0;255,153,0;255,153,0;255,153,0;255,153,0;1;1;1;1;1;0;\r\n"
	"GraphEnd;\r\n"
	*/
//	"VertScaleGroupEnd;\r\n"
//	"BlockEnd;\r\n"
	"BlockColumnEnd;\r\n",
	"PO=1;�ڷ�����;;8;YYYYMMDD;��;\r\n"
	"1;�ð�;;10;�� 1;��;\r\n"
	"1;��;;10;�� 1;��;\r\n"
	"1;����;;10;�� 1;��;\r\n"
	"1;����;;10;�� 1;��;\r\n"
	"1;�⺻�ŷ���;;10;�� 1;��;\r\n"
	"1;������;;1;�� 1;��;\r\n"
	"1;������;;2;�� 1;��;\r\n"
	"1;������;;10;�� 0.01;��;\r\n"
	"1;������ Sign;;1;�� 1;��;\r\n",
	"RL=",
	"URD=1",
	"IRL=�ڷ�����.S31.time.�ڵ�;����.S31.price.�ڵ�;�⺻�ŷ���.S31.volume.�ڵ�;\r\n",
	"MBT=3",
	"USESKIN=1",
	"CBKG=244,244,244",
	"CBKB=122,122,122",
	"CB=255,255,255",
	"CBB=122,122,122",
	"CSL=192,192,192",
	"CST=0,0,0",
	"CGB1=8,189,246",
	"CGB2=128,182,0",
	"CGB3=253,122,4",
	"CGB4=150,19,147",
	"CGB5=255,133,11",
	"CGB6=128,0,255",
	"CAT=82,103,146",
	"BR=row;0;216;\r\n"
	"column;0;387;",
	"CSL2=192,192,192",
	"RDB=0",
	"UIDWCL=0",
	"CDB=0",
	"CIW=255,255,255",
	"CFT=����ü;12;0;0;0;0;",
	"USK=0",
	"UCTT=1",
	"UDTT=0",
	"NRBMT=0",
	"BSWMB=0",
	"BCBS=1",
	"BBMD=1",
	"BST=1",
	"BSG=1",
	"BUSRBM=1",
	"NHG=0",
	"NVG=0",
	"CLTI=255,255,255",
	"NGRBT=1",
	"NHPS=0",
	"NMDCOOP=5",
	"!MapSize=0,0,400,250",
	"BRAUAP=0",
	"NIO=6",//"NIO=4",//"NIO=7",//��ġ��ȸ �⺻ (6), Ȯ��⺻(4) ??(7)
//	"GSL=�ð�-�׸���4;�ð�-��Ʈ2;"
	"PGCNTTR=1"		// JSR 20060221 ADD : From Doori
};


LPCTSTR szSetAllNoToolbar[] = 
{
	"CM=Name,Any2Chart0",
	"AN=Any2Chart0",
	"ID=3,4,0",
	"RC=2,24,397,248",
	"WS=WS_CHILD|WS_VISIBLE",
	"WSE=",
	"BT=0",
	"BC=0",
//	"RC=1",
	"ROWC=1",
	"CC=1",
	"BG=-1",
	"BBT=1",

	"BDT=0",
//	"SLT=0",							// �Ǽ�(graph ���ý�)
	"SLT=2",							// ����(graph ���ý�)
	"WV=0",
//	"WV=0",							==> Big Bang Original Source���� ���
	"PDC=140",
	"BAC=10",			// �ִ� ����
	"GDD=0",
	"LM=0",	// left ����
	"RM=0",	// right ����
	"TM=0",	// top ����
	"BM=0",	// bottom ����
	"AGM=1",												// ���ؽ�Ʈ �޴� 
	"EFN=",
	"IFN=",
	"SFN=",
	"HI=0",
	"IR=0",
	"TO=����\r\n����;;;\r\n��ũ�ѹ�;;;\r\nȮ��,���;����;10��;\r\nDisplay�� ������;;;\r\n����;;;\r\n"
//	"TO=����\n��ũ�ѹ�;;;\nGraph����;;;\n������;;;\n������;;;\n��ġ��ȸâ;�˾� ���;��ġDialog ���;\n���ڼ�;��ġ��ȸâ ������;;\n����;;;\n����;;;\nȮ��,���;����;10��;\n������;;;\r\n"
	"�ܺ�\r\n"
//	"���ڼ�;��ġ��ȸâ ������;;\r\n"
	"RButton\r\n",

	"CL=BlockColumn;\r\n"
	"Block;\r\n"
//	"�ڷ�����;4;2;/;:;0;0;0;\r\n"
	"�ڷ�����;4;1;/;:;0;0;0;\r\n"
	"VertScaleGroup;\r\n"
	"��ǥ Data;1;2;0;\r\n"
//	"��ǥ Data;1;3;0;\r\n"

 
//=========================================================================================
//From_LTH (2003.7.1 ) 
// �Ʒ����� AddGraph�� ���� �����ϹǷ� �ּ�ó����
//	"Graph;\r\n"
//	"����;;�ǽð� ����;0;0;0;0;0;0;\r\n"
//	"1;0;0;Graph1;�ǽð�����;0;17,0,0;17,0,0;0,0,255;0,0,255;255,0,255;255,0,255;1;1;1;1;1;0; \r\n"
//	"GraphEnd;\r\n"
//=========================================================================================

/*	"Graph;\r\n",
	"����;;�ŵ�����;0;0;0;0;0;0;\r\n"
	"1;0;0;Graph1;�ŵ�����;0;0,0,255;0,0,255;0,0,255;0,0,255;255,0,255;255,0,255;1;1;1;1;1;0; \r\n"
	"GraphEnd;\r\n"
	"Graph;\r\n"
	"����;;�ż�����;0;0;0;0;0;0;\r\n"
	"1;0;0;Graph1;�ż�����;0;204,0,0;204,0,0;0,0,255;0,0,255;255,0,255;255,0,255;0;1;1;1;1;0; \r\n"
	"GraphEnd;\r\n"
	"Graph;\r\n"
	"����;;�ŷ�������;0;0;0;0;0;0;\r\n"
	"1;0;0;Graph1;�ŷ�������;0;0,102,0;0,119,0;0,0,255;0,0,255;255,0,255;255,0,255;1;1;1;1;1;0; \r\n"
	"GraphEnd;\r\n"*/
	"VertScaleGroupEnd;\r\n"
	"BlockEnd;\r\n"
	"BlockColumnEnd;\r\n",

	"PO=\r\n",//1;�ڷ�����;;6;HHMMSS;��;\r\n",
//	"1;�ǽð� ����;;10;�� 0.01;��;\r\n"
//	"1;�ŵ�����;;10;�� 0.01;��;\r\n"
//	"1;�ż�����;;10;�� 0.01;��;\r\n"
//	"1;�ŷ�������;;10;�� 0.01;��;",
//	"PO=1;�ڷ�����;;6;HHMM  ;��;\r\n",		==> Big Bang Original Source���� ���
//	"1;�ǽð� ����;;10;�� 0.01;��;\r\n",

//	"LSL=�����ڵ�5;",

//	"RL=KP2.time.�ڵ�-�ǽð�����;"
//	"KP2.time.�ڵ�-�ǽð�����;"
//	"KP2.medojisu.�ڵ�-�ǽð�����;"
//	"KP2.mesujisu.�ڵ�-�ǽð�����;"
//	"KP2.upjisu.�ڵ�-�ǽð�����;",
	"RL=",
//	"RL=SC0.time.�����ڵ�;"
//	"SC0.price.�����ڵ�;",

	"URD=1",
	"MBT=3",
//	"CBKG=208,209,213",
	"CBKG=255,255,255",
	"CBKB=188,198,223",
//	"CBKB=0,0,0",
//	"CBKB=255,255,255",
//	"CB=236,236,236",
	"CB=255,255,255",
//	"CBB=231,233,230",
	"CBB=0,0,0",
//	"CSL=255,255,255",
	"CSL=204,204,204",
	"CST=0,0,0",
	"CGB1=8,189,246",
	"CGB2=128,182,0",
	"CGB3=253,122,4",
	"CGB4=150,19,147",
	"CGB5=255,133,11",
	"CGB6=128,0,255",
	"CAT=82,103,146",


	"BR=row;0;216;\r\n"
	"column;0;387;\r\n",// 2005. 07. 22 ����Ʈ���� ����ϱ� ���� �����Ѵ�... ���� ��� "column;0;387;"

	"CSL2=204,204,204",
	"RDB=-1", //2005. 07. 01 ���� �������� ����ȭ 
	"UIDWCL=0",
	"CDB=-1", //2005. 07. 01 ���� �������� ����ȭ 
	"CIW=255,255,255",
	"CFT=����ü;12;0;0;0;0;",
	"USK=0",
	"UCTT=0",	
	"UDTT=0",
	"NRBMT=0",//05.11.17 1->0 R��ư �� �޴� �ܼ������� �ٿ쵵�� 
	"BSWMB=1",
	"BCBS=1",
	"BBMD=1",
	"BST=1",
	"BSG=1",
	"BUSRBM=1",
	"NHG=0",
	"NVG=0",
	"CLTI=255,255,255",
	"NGRBT=1",
	"NHPS=0",
	"NMDCOOP=5",
	"!MapSize=0,0,400,250",
	"BRAUAP=0",
	"NIO=4",//"NIO=7",
//	"GSL=�ð�-�׸���4;�ð�-��Ʈ2;"
//	"SLHW=�ð�_��Ʈ,1,0,0x00000000;�ð�_�׸���,2,0,0x00000000;", //05. 08. 29 ��ȭ���� ���� �������.
//	"PAGECOUNTTR=0" //05.11.28 �߰�
	"PGCNTTR=0"	// JSR 20060221 ADD : From Doori
/*
	"CBKG=255,255,255",
	"CBKB=0,0,0",
	"CB=255,255,255",
	"CBB=0,0,0",
	"CSL=128,128,128",
	"CST=0,0,0",
	"CGB1=0,136,0",
	"CGB2=255,55,255",
	"CGB3=255,51,51",
	"CGB4=0,109,219",
	"CGB5=255,133,11",
	"CGB6=128,0,255",
	"CAT=0,0,0",

	"BR=row;0;216;\r\n"
	"column;0;387;",

	"CSL2=128,128,128",
	"RDB=0",
	"UIDWCL=0",
	"CDB=0",
	"CIW=203,228,203",
	"CFT=����ü;12;0;0;0;0;",
	"USK=0",
	"UCTT=0",
	"UDTT=0",
	"NRBMT=0",
	"BSWMB=1",
	"BCBS=1",
	"BBMD=0",
	"BST=0",
	"BSG=1",
	"BUSRBM=1",
	"NHG=0",
	"NVG=0",
	"CLTI=255,255,255",
	"NGRBT=1",
	"NHPS=2048",
	"NMDCOOP=5",
	"!MapSize=0,0,400,250",
	"BRAUAP=0",
*/
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartItem_Stock::CChartItem_Stock()
{
	m_bInitalChart = FALSE;
	m_lKey = 0;
}

CChartItem_Stock::~CChartItem_Stock()
{
}

BOOL CChartItem_Stock::Create(CWnd *pWnd,LPRECT lpRect,int nID)
{
	CString strPath;
	GetDir_DrfnData(strPath);
	return CKISChart::Create(strPath, NULL,WS_VISIBLE|WS_CHILD,*lpRect,pWnd,nID);
}

void CChartItem_Stock::SetpBaseDesk(long lKey,HWND hWndDll)
{
	m_lKey = lKey;
	CKISChart::SetBaseDesk(lKey);
	CKISChart::SetContainerDllInfo((long)hWndDll,GetDlgCtrlID());
//	SetAllProperties();

}

BOOL CChartItem_Stock::SetAllProperties(BOOL bNoToolbar /*=FALSE*/)
{
	CPropertyMap mapProp;
	CString strData, strValue;
	int nPos;

	if(bNoToolbar)
	{
		for(int i=0;i<sizeof(szSetAllNoToolbar)/sizeof(LPCTSTR);i++)
		{
			strData = szSetAllNoToolbar[i];
			nPos = strData.Find(_T('='));
			if(nPos >= 0)
			{
				strValue = strData.Mid(nPos + 1);
				strData = strData.Left(nPos);
				mapProp.SetAt(strData, strValue);
			}
		}
	}
	else
	{
		for(int i=0;i<sizeof(szSetAll_Stock)/sizeof(LPCTSTR);i++)
		{
			strData = szSetAll_Stock[i];
			nPos = strData.Find(_T('='));
			if(nPos >= 0)
			{
				strValue = strData.Mid(nPos + 1);
				strData = strData.Left(nPos);
				mapProp.SetAt(strData, strValue);
			}
		}
		//for(int i=0;i<sizeof(szSetAll_Stock)/sizeof(LPCTSTR);i++)
		//	strlistSetAll.AddTail(szSetAll_Stock[i]);	
	}

	CKISChart::SetAllProperties3((long)(IPropertyMap*)&mapProp);

	return TRUE;
}

CString CChartItem_Stock::MakePacketOption(LPCTSTR lpPacketName,long lPacketSize,LPCTSTR lpPacketType, BOOL bUseInReal)
{
	char szTemp[64];
	CString strPacket;
	strPacket = "1;";
	strPacket += lpPacketName; // ����� Packet��
	strPacket += ";;";
	sprintf(szTemp,"%d",lPacketSize);
	strPacket += szTemp;
	strPacket += ";";
	strPacket += lpPacketType;
	strPacket += ";";
	if(bUseInReal)
	{
		strPacket += "��;\r\n";
	}
	else
	{
		strPacket += "��;\r\n";
	}
	return strPacket;
}


CString CChartItem_Stock::MakeGraphOption(LPCTSTR lpChartType,LPCTSTR lpChartName, CStringArray& rArrayPacketList, LPCTSTR lpChartTitle, long lChartStyle /*=0*/,
									COLORREF clrGrp1 /*= RGB(218,46,46)*/, COLORREF clrGrp2 /*= RGB(218,46,46)*/,
									COLORREF clrGrp3 /*= RGB(46,46,218)*/, COLORREF clrGrp4 /*= RGB(46,46,218)*/,
									COLORREF clrGrp5 /*= RGB(218,46,46)*/, COLORREF clrGrp6 /*= RGB(218,46,46)*/,
									BOOL bUpFilled /*= 0*/, BOOL bDownFilled /*= 1*/,
									long lUpLineWidth /*= 1*/, long lDownLineWidth /*= 1*/, BOOL bHideGrp /*= 0*/)
{
	CString strChartType = lpChartType;
	CString tempStr;
	CString strGraphOption("Graph;\r\n");
	strGraphOption += lpChartType;
	strGraphOption += ";";
	strGraphOption +=lpChartName;
	strGraphOption += ";";
	LONG lSize = rArrayPacketList.GetSize();
	for(long lIndex=0;lIndex<lSize;lIndex++)
	{
		if(lIndex)	strGraphOption += ",";
		strGraphOption += rArrayPacketList.GetAt(lIndex);		
	}
	if(strChartType==g_strChartType_Stock[0])
	{
		strGraphOption += ";0;0;0;0;1;0";
	}
	strGraphOption += ";\r\n";

	// "�׷���Ÿ��;�׷�����Ÿ��;�׸��½�Ÿ��;�̸�;title;����;��1;��2;��3;��4;��5;��6;
	//  ���ä��;�϶�ä��;������1;������2;������3;����;\r\n"
	
	if(strChartType==g_strChartType_Stock[0])
	{
		strGraphOption += "0;";
	}
	else if(strChartType==g_strChartType_Stock[1])
	{
		strGraphOption += "1;";
	}
	else if(strChartType==g_strChartType_Stock[2])
	{
		strGraphOption += "2;";
	}
	else
	{
		ASSERT(0); // Error !! 
	}
	tempStr.Format("%d;",lChartStyle);
	strGraphOption += tempStr;
	strGraphOption += "0;"; 
	strGraphOption += lpChartName;
	strGraphOption += ";";
	strGraphOption += lpChartTitle;
	tempStr.Format(";0;%d,%d,%d;%d,%d,%d;%d,%d,%d;%d,%d,%d;%d,%d,%d;%d,%d,%d;%d;%d;%d;%d;%d;%d;\r\n",
		GetRValue(clrGrp1),GetGValue(clrGrp1), GetBValue(clrGrp1),
		GetRValue(clrGrp2),GetGValue(clrGrp2), GetBValue(clrGrp2),
		GetRValue(clrGrp3),GetGValue(clrGrp3), GetBValue(clrGrp3),
		GetRValue(clrGrp4),GetGValue(clrGrp4), GetBValue(clrGrp4),
		GetRValue(clrGrp5),GetGValue(clrGrp5), GetBValue(clrGrp5),
		GetRValue(clrGrp6),GetGValue(clrGrp6), GetBValue(clrGrp6),
		bUpFilled,bDownFilled,
		lUpLineWidth,lDownLineWidth,bHideGrp);
	strGraphOption += tempStr;
	
	strGraphOption += "GraphEnd;\r\n";	
	return strGraphOption;
}

CString CChartItem_Stock::GetPacketName()
{
	CString strPacketName;
	long lSize = m_strArrayPacketName.GetSize();
	for(long lPos=0;lPos<lSize;lPos++)
	{
		strPacketName += m_strArrayPacketName.GetAt(lPos);
		strPacketName += ";";
	}
	return strPacketName;
}



long CChartItem_Stock::GetDataSize() 
{ 
	return 0;
}

void CChartItem_Stock::AddRealItem(CString strCode)
{
	CString strRealItem;	
	// �ڷ�����
	strRealItem = g_strPacketName_Stock[0];
	strRealItem += '.';
	strRealItem += "S31.time.";
	strRealItem += strCode;
	strRealItem += ';';
	CKISChart::AddRealItem(strRealItem);
	m_strArrayRealItemRegistered.Add(strRealItem);
	// ����
	strRealItem = g_strPacketName_Stock[4];
	strRealItem += '.';
	strRealItem += "S31.price.";
	strRealItem += strCode;
	strRealItem += ';';
	CKISChart::AddRealItem(strRealItem);
	m_strArrayRealItemRegistered.Add(strRealItem);
	// �ŷ���
	strRealItem = g_strPacketName_Stock[5];
	strRealItem += '.';
	strRealItem += "S31.volume.";
	strRealItem += strCode;
	strRealItem += ';';
	CKISChart::AddRealItem(strRealItem);		
	m_strArrayRealItemRegistered.Add(strRealItem);

//	// �ð�
//	strRealItem = g_strPacketName[1];
//	strRealItem += '.';
//	strRealItem += "S31.open.";
//	strRealItem += strCode;
//	strRealItem += ';';
//	CKISChart::AddRealItem(strRealItem);
//	m_strArrayRealItemRegistered.Add(strRealItem);
//	// �� 
//	strRealItem = g_strPacketName[2];
//	strRealItem += '.';
//	strRealItem += "S31.high.";
//	strRealItem += strCode;
//	strRealItem += ';';
//	CKISChart::AddRealItem(strRealItem);
//	m_strArrayRealItemRegistered.Add(strRealItem);
//	// ����
//	strRealItem = g_strPacketName[3];
//	strRealItem += '.';
//	strRealItem += "S31.low.";
//	strRealItem += strCode;
//	strRealItem += ';';
//	CKISChart::AddRealItem(strRealItem);
//	m_strArrayRealItemRegistered.Add(strRealItem);

}

void CChartItem_Stock::RemoveRealItem()
{
	CString strRealItemRegistered;
	int nSize = m_strArrayRealItemRegistered.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		strRealItemRegistered = m_strArrayRealItemRegistered.GetAt(nPos);
		CKISChart::RemoveRealItem(strRealItemRegistered);
	}
	m_strArrayRealItemRegistered.RemoveAll();
}


// BOOL CChartItem_Stock::AddPacketAndGraph()
// {	
// 	if(!m_bInitalChart)
// 	{
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[0]);
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[1]);
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[2]);
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[3]);
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[4]);
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[5]);
// 	//	m_strArrayPacketName.Add(g_strPacketName_Stock[6]);
// 	//	m_strArrayPacketName.Add(g_strPacketName_Stock[7]);	
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[8]);				
// 		m_bInitalChart = TRUE;
// 	}
// 	return TRUE;
// }

BOOL CChartItem_Stock::AddPacketAndGraph()
{	
	if(!m_bInitalChart)
	{
		AddPacket(MakePacketOption(g_strPacketName_Stock[0],8,g_strPacketTypeDate_Stock[0],TRUE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[0]);
		AddPacket(MakePacketOption(g_strPacketName_Stock[1],10,g_strPacketTypeNumber_Stock[0],FALSE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[1]);
		AddPacket(MakePacketOption(g_strPacketName_Stock[2],10,g_strPacketTypeNumber_Stock[0],FALSE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[2]);
		AddPacket(MakePacketOption(g_strPacketName_Stock[3],10,g_strPacketTypeNumber_Stock[0],FALSE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[3]);
		AddPacket(MakePacketOption(g_strPacketName_Stock[4],10,g_strPacketTypeNumber_Stock[0],TRUE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[4]);
		AddPacket(MakePacketOption(g_strPacketName_Stock[5],10,g_strPacketTypeNumber_Stock[0],TRUE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[5]);
		
		AddPacket(MakePacketOption(g_strPacketName_Stock[6],2,g_strPacketTypeNumber_Stock[0],TRUE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[6]);
		AddPacket(MakePacketOption(g_strPacketName_Stock[7],10,g_strPacketTypeNumber_Stock[4],TRUE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[7]);
		AddPacket(MakePacketOption(g_strPacketName_Stock[8],1,g_strPacketTypeNumber_Stock[0],TRUE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[8]);
// 		AddPacket(MakePacketOption(g_strPacketName_Stock[7],1,g_strPacketTypeEtc_Stock[1],FALSE));
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[7]);	
// 		AddPacket(MakePacketOption(g_strPacketName_Stock[8],6,g_strPacketTypeEtc_Stock[1],FALSE));
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[8]);

		CStringArray arrayPacket;
		arrayPacket.Add(g_strPacketName_Stock[0]);
		arrayPacket.Add(g_strPacketName_Stock[1]);	// �ð�
		arrayPacket.Add(g_strPacketName_Stock[2]);	// ��
		arrayPacket.Add(g_strPacketName_Stock[3]);	// ����
		arrayPacket.Add(g_strPacketName_Stock[4]);	// ����
		arrayPacket.Add(g_strPacketName_Stock[5]);
		arrayPacket.Add(g_strPacketName_Stock[6]);
		arrayPacket.Add(g_strPacketName_Stock[7]);
		arrayPacket.Add(g_strPacketName_Stock[8]);

		//		AddGraph(0,0,0,"������Ʈ",FALSE,FALSE,FALSE);
		ChangeGraphDrawingData("������Ʈ", FALSE, FALSE, FALSE, FALSE, 1);
		//		AddGraph(0,0,0,"���� �̵����",FALSE,FALSE,FALSE);
		//		AddGraph(1,0,0,"�ŷ�����Ʈ",TRUE,TRUE,TRUE);

		m_bInitalChart = TRUE;
		//		ChangeGraphTitle(0, 0, 0,""); // LYH : ������Ʈ�� Nts ��Ʈ�� �ٲٸ鼭, ���� ���� ���´�. �̿������Կ�, �����Ŵ� �ʿ��� ��������
		SetNUseInquiryWithCrossline(0);
		ResetChartCfgWith(0); //
	}
	else
	{
		/*ResetChart();*/
	}

	return TRUE;
}

void CChartItem_Stock::SetInit(HWND p_lDllWnd, long lKey, LPCTSTR strHtsPath, BOOL bNoToolBar /*=FALSE*/)
{	
	CKISChart::SetHtsPath(strHtsPath);
	CKISChart::SetContainerDllInfo((long)p_lDllWnd, GetDlgCtrlID());
	SetAllProperties(bNoToolBar);	
	
//	LoadAddIn("ChartPeriodSummaryAddIn.dll:DRAG_ZOOM_ANAL");
	
}


BOOL CChartItem_Stock::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)  
	{		
		UINT nChar = (UINT)pMsg->wParam;
		switch(nChar)
		{
		case VK_RETURN :
		case VK_ESCAPE :
			{
				return FALSE;
				break;					
			}
		case VK_TAB:
			if(::GetKeyState(VK_CONTROL) < 0) // �Ｚ: ctrl-tab �� �ȸԴ� ���� ������
			{
				GetParent()->GetParent()->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
				return FALSE;
			}
		}
	}
	else if(pMsg->message == WM_SYSKEYDOWN)
	{
		if( ::GetKeyState(VK_MENU) & 0x8000)
			return FALSE;
	}

	if(::IsWindow(m_hWnd))
		return GetParent()->PreTranslateMessage(pMsg);

	return CChartItem_Stock::PreTranslateMessage(pMsg);
}
