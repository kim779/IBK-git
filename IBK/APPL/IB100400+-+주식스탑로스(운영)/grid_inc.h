
#pragma once

#define				JCOLCOUNT			59
#define				GRIDROWHEIGHT		20

enum _colINDEX
{
	colSET=0,//0
	colCHECK,//1
	colCDTYPE,//2
	colCODE,//3
	colRCOD,//4
	colNAME, //5

	colCRETYPE,//6

	colJANTYPE,//7 �ܰ�����
	colVOL,//�ܰ�8
	colPOSSVOL,//����9
	colCOST,//10 ���԰�
	colAMOUNT,//11
	colCURR,//12
	colBEFORERATE,
	colUPDOWNRATE,
	colPROFITPRC,//13
	colPROFIT,//14
	colRATE,//15

	colCRTCD,//16
	colCRTDATE,//17
	colCRTPRC,//�ſ�ݾ� 18
	

	colTMP1,//19

	colLOSS,//�սǸ�ǥ20
	colLSCFG, //21
	colMIT,//���͸�ǥ  22
	colMTCFG,//23
	colTS,//���ͺ��� 24
	colTSCFG,

	colTSDIF, //��������϶�
	colTSDIFCFG,
	colTSMAX,//����

//	colCRTPRC2,
	colHOGATYPE,
	colSTATE,
	colEXEC,
	colJPRICE,
	colJVOL,
	colJVOLCASE,
	colCVOL,
	colJNO,
	colJMSG,

	//���Ѱ�
	colVolUNIT,
	colSISE,
	colPCUR,

};


static struct	_gridHdr {
	char	szhdr[50];
	UINT	width;		// width
	UINT	attr;		// attribute
	char	symbol[8];	// symbol
	UINT	format;		// format
	UINT	attrx;		// attr
	bool	c_merge;
	bool	r_merge;
	char	szhdr0[50];
	int		c_flag1;
	int		c_flag2;
	int		itogglecol;
	double  lcolrate;
} gridHdr[] = 
{
	{"����",		30,		GVAT_TEXT,		"",		GVFM_CENTER,	GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.0316},	
	{"����",		26,		GVAT_CHECK,		"",		GVFM_RIGHT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.02745},	
	{"T",			0,		GVAT_HIDDEN,		"",		GVFM_LEFT,		0,							0, 1,		"",							0, 0, 0, 0.f},	

	{"�����ڵ�",	0,		GVAT_TEXT,		"",		GVFM_LEFT,		0,							0, 1,		"",							0, 0, 0, 0.f},	
	{"�����ڵ�",	0,		GVAT_HIDDEN,	"1021",	GVFM_LEFT,		0,							0, 1,		"",							0, 0, 0, 0.f},	
	{"�����",		100,	GVAT_TEXT,		"",		GVFM_LEFT,		0,							1, 1,		"",							0, 0, 3, 0.11},	
	{"",			53,		GVAT_TEXT,		"",		GVFM_CENTER,	GVAT_TOOLTIP,							1, 1,		"",							0, 0, 0, 0.0559},	

	{"�ܰ�����",	0,		GVAT_HIDDEN,	"",		GVFM_LEFT,		0,							0, 0,		"",							1, 0, 0, 0.f},	
	{"����",		38,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TOOLTIP|GVAT_COMMA,					0, 0,		"",							0, 0, 9, 0.040126},		
	{"����",		0,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TOOLTIP|GVAT_COMMA,					0, 0,		"",							0, 0, 8, 0.f},		
	{"���԰�",		60,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TOOLTIP|GVAT_COMMA,					0, 0,		"",							0, 0, 0, 0.0633},		
	{"���Աݾ�",	0,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TOOLTIP|GVAT_COMMA,				0, 0,		"",							0, 0, 0, 0.f},		
	{"���簡", 		50,		GVAT_TEXT,		"2023",	GVFM_RIGHT,		GVAT_TOOLTIP|GVAT_COMMA|GVAT_CONDITIONx, 0, 0,		"",							0, 0, 0, 0.0527},		
	{"���ϴ��",	0,		GVAT_HIDDEN,	"",		GVFM_LEFT,		0,							0, 0,		"",							1, 0, 0, 0.f},
	{"�����",	0,		GVAT_HIDDEN,	"",		GVFM_LEFT,		0,							0, 0,		"",							1, 0, 0, 0.f},
	{"�򰡱ݾ�", 	64,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TOOLTIP|GVAT_COMMA,					0, 0,		"",							0, 0, 0, 0.06758},	
	{"�򰡼���", 	64,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TOOLTIP|GVAT_COMMA|GVAT_CONDITION,	0, 0,		"",							0, 0, 0, 0.06758},		
	{"���ͷ�", 		60,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TOOLTIP|GVAT_COMMA|GVAT_CONDITION,	0, 0,		"",							0, 0, 0, 0.0633},		//15

	{"�ڵ�", 		0,		GVAT_TEXT,		"",		GVFM_LEFT,		0,							0, 0,		"",							0, 0, 0, 0.f},	//16	
	{"������", 		66,		GVAT_TEXT,		"",		GVFM_LEFT,		GVAT_TOOLTIP,							0, 0,		"",			0, 0, 0, 0.06969},//17		
	{"�ſ�ݾ�", 	0,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,					0, 0,		"",							0, 0, 0, 0.f},//18

	{"", 			0,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,					0, 1,		"��         ��",			1, 1, 0, 0.f}, //19

	{"�սǸ�ǥ", 	56,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,					0, 0,		"",							0, 0, 23, 0.059134},		
	{"�ս�����", 	0,		GVAT_TEXT,		"",		GVFM_RIGHT,		0,							0, 0,		"",							0, 0, 22, 0.f},		
	{"���͸�ǥ", 	56,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,					0, 0,		"",							0, 0, 25, 0.059134},	
	{"��������", 	0,		GVAT_TEXT,		"",		GVFM_RIGHT,		0,							0, 0,		"",							0, 0, 24, 0.f},	
	{"���ͺ���", 	56,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,					0, 0,		"",							0, 0, 27, 0.059134},		
	{"��������", 	0,		GVAT_TEXT,		"",		GVFM_RIGHT,		0,							0, 0,		"�ֹ��ߵ�����",				0, 1, 26, 0.f},		

	{"�������",	0,		GVAT_HIDDEN,	"",		GVFM_RIGHT,		0,							0, 1,		"",							0, 0, 0, 0.f},
	{"��������",	0,		GVAT_HIDDEN,	"",		GVFM_CENTER,	0,							0, 1,		"",							0, 0, 0, 0.f},
	{"����",		0,		GVAT_HIDDEN,	"",		0,				0,							1, 1,		"",							0, 0, 0, 0.f},

	{"ȣ��",		0,		GVAT_HIDDEN,	"",		0,				0,							0, 0,		"",							1, 0, 0, 0.f},//29
	{"����", 		0,		GVAT_HIDDEN,	"",		GVFM_CENTER,	0,							0, 0,		"",							0, 0, 0, 0.f},//		
	{"����", 		30,		GVAT_TEXT,		"",		GVFM_CENTER,	0,							0, 0,		"",							0, 0, 0, 0.0316},//		
	{"�ֹ���", 		46,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,					0, 0,		"",							0, 0, 0, 0.04857},	
	{"�ֹ���", 		46,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,					0, 0,		"",							0, 0, 36, 0.04857	},		
	{"��������", 	0,		GVAT_TEXT,		"",		GVFM_RIGHT,		0,							0, 0,		"",							0, 0, 35, 0.f},		
	{"����2", 		0,		GVAT_TEXT,		"",		GVFM_RIGHT,		0,							0, 0,		"",							0, 0, 0, 0.f},		
	{"�ֹ�No", 		46,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TOOLTIP,				0, 0,		"��         ��",			0, 0, 0, 0.04857},	//947
	{"ó���޼���", 	0,		GVAT_HIDDEN,	"",		GVFM_LEFT,		0,							0, 0,		"��         ��",			0, 1, 0, 0.f},	

	{"��������",	0,		GVAT_HIDDEN,	"1377",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"���Ѱ�", 		0,		GVAT_HIDDEN,	"2302",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"�ż�5", 	0,		GVAT_HIDDEN,	"3075",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"�ż�4", 	0,		GVAT_HIDDEN,	"3074",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"�ż�3", 	0,		GVAT_HIDDEN,	"3073",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"�ż�2", 	0,		GVAT_HIDDEN,	"3072",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"�ż�1", 	0,		GVAT_HIDDEN,	"3071",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"���簡", 		0,		GVAT_HIDDEN,	"2023",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"�ŵ�1", 	0,		GVAT_HIDDEN,	"3051",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"�ŵ�2", 	0,		GVAT_HIDDEN,	"3052",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"�ŵ�3", 	0,		GVAT_HIDDEN,	"3053",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"�ŵ�4", 	0,		GVAT_HIDDEN,	"3054",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"�ŵ�5", 	0,		GVAT_HIDDEN,	"3055",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"���Ѱ�", 		0,		GVAT_HIDDEN,	"2303",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"��  ��", 		0,		GVAT_HIDDEN,	"2029",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"��  ��", 		0,		GVAT_HIDDEN,	"2030",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"��  ��", 		0,		GVAT_HIDDEN,	"2031",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"�����ֽļ�", 		50,		GVAT_HIDDEN,	"12374",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},
	{"���ذ�", 		50,		GVAT_HIDDEN,	"1336",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},
};




