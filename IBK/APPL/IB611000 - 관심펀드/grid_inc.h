#pragma once
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
	{"",			55,		GVAT_TEXT,		"",		GVFM_LEFT,	GVAT_TOOLTIP,				1, 1,		"",							0, 0, 0, 0.11},
	{"�ݵ��      ",		260,	GVAT_TEXT,		"",		GVFM_LEFT,		GVAT_TOOLTIP,		0, 1,		"",							0, 0, 3, 0.0559},	
	{"���ذ�",		55,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,			0, 1,		"",							1, 0, 0, 0.f},	

	{"1��", 		41,		GVAT_TEXT,		"",	GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.0527},		
	{"1��", 		41,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.06758},	
	{"1����", 		41,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.06758},		
	{"3����", 		41,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.06758},		
	{"6����", 		41,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.0633},		//15
	{"1��", 		41,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.0633},		//15
	{"3��", 		41,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.0633},		//15
	{"����",		41,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.0633},		//15
	{"����", 		41,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.0633},		//15
//  

 	{"", 			0,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_CONDITIONx,		0, 1,		"��    ��    ��",			1, 1, 0, 0.f}, //19// 	
	{"������������", 80,  	GVAT_TEXT,		"",		GVFM_LEFT,		GVAT_TEXT,	0, 1,		"",							0, 0, 0, 0.0633},		//15	
	{"�Ⱓ(��)",	 55,	GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TEXT,	0, 1,		"",							0, 0, 0, 0.0633},		//15	
	{"������",		 70,	GVAT_TEXT,		"",		GVFM_CENTER,	GVAT_TEXT,	0, 1,		"",							0, 0, 0, 0.0633},		//15	
	{"�ֽ����Ժ���(%)", 97,GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TEXT,	0, 1,		"",							0, 0, 0, 0.0633},		//15	
	{"���ڵ�޸�", 75 ,		GVAT_TEXT,		"",		GVFM_LEFT,		GVAT_TEXT,	0, 1,		"",							0, 0, 0, 0.0633},		//15
//	{"", 0,		GVAT_HIDDEN,		"",		GVFM_RIGHT,		GVAT_CONDITIONx,	1, 1,		"",							1, 1, 0, 0.f},	
};

enum _colINDEX
{
	colSET=0,//0
		colCHECK,//0
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
