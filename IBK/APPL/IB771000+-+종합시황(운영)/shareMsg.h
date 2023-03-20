
#pragma once

#include "../../h/axisvar.h"

#define WM_MANAGE			(WM_USER + 1000)
#define WM_SEND				(WM_USER + 1020)
#define	EVT_SENDCONTENT		1
#define	EVT_SENDNAVIGATE	2

#define MSG_TREE_CLICK		1
#define MSG_COLOR_CHANGE	2
#define MSG_OPTION_CHECK	3
#define MSG_LOG				4
#define MSG_LOG2			5
#define MSG_TREECHK_CLICK	6



//////////////////////// ��ü ���ڿ�
#define STR_TOTAL_ROOT			"��ü"			// Root ��ü
#define STR_NOTICE_ROOT			"����"			// Root ����
#define STR_NEWS_ROOT			"��Ȳ"			// Root ��Ȳ

#define STR_ETODAY_ROOT			"��������"		// Root ��������
#define STR_ETODAY_SUB1			"����"
#define STR_ETODAY_SUB2			"����"

#define STR_HANENWS_ROOT		"�Ѱ洺��"		// Root �Ѱ洺�� 
#define STR_HANNEWS_SUB1		"����"
#define STR_HANNEWS_SUB2		"����"
#define STR_HANNEWS_SUB3		"�ε���"
#define STR_HANNEWS_SUB4		"����"
#define STR_HANNEWS_SUB5		"���/���"
#define STR_HANNEWS_SUB6		"��ó/�߱�"
#define STR_HANNEWS_SUB7		"����/����"
#define STR_HANNEWS_SUB8		"��ġ"
#define STR_HANNEWS_SUB9		"����"
#define STR_HANNEWS_SUB10		"��ȸ"
#define STR_HANNEWS_SUB11		"��ȭ/����"
#define STR_HANNEWS_SUB12		"����/������"
#define STR_HANNEWS_SUB13		"�ι�/����"
#define STR_HANNEWS_SUB14		"��Ÿ"
#define STR_HANNEWS_SUB15		"����"

//#define STR_HANENWS_ROOT		"�Ѱ洺��"		// Root �Ѱ洺�� 

#define STR_EDAILY_ROOT			"�̵��ϸ�"		// Root �̵��ϸ�
#define STR_EDAILY_SUB1			"�Ϲ�"
#define STR_EDAILY_SUB2			"ä��"
#define STR_EDAILY_SUB3			"����"
#define STR_EDAILY_SUB4			"��ȯ"
#define STR_EDAILY_SUB5			"����"
#define STR_EDAILY_SUB6			"IPO"
#define STR_EDAILY_SUB7			"����"
#define STR_EDAILY_SUB8			"����� ��ó"
#define STR_EDAILY_SUB9			"�ֽ�"

#define STR_INFOSTOCK_ROOT		"������Ź"		// Root ������Ź
#define STR_INFOSTOCK_SUB1		"�ŷ�������"
#define STR_INFOSTOCK_SUB2		"�ڽ�������"
#define STR_INFOSTOCK_SUB3		"�ؿ�����"
#define STR_INFOSTOCK_SUB4		"��������"
#define STR_INFOSTOCK_SUB5		"��Ÿ"

#define STR_MONEYTODAY_ROOT		"�Ӵ�������"		// Root �Ӵ�������
#define STR_MONEYTODAY_SUB1		"�ֽ�"
#define STR_MONEYTODAY_SUB2		"����/�ɼ�"
#define STR_MONEYTODAY_SUB3		"�ؿ�����"
#define STR_MONEYTODAY_SUB4		"��ȯ"
#define STR_MONEYTODAY_SUB5		"ä��"
#define STR_MONEYTODAY_SUB6		"����"
#define STR_MONEYTODAY_SUB7		"���"
#define STR_MONEYTODAY_SUB8		"��������"
#define STR_MONEYTODAY_SUB9		"����"
#define STR_MONEYTODAY_SUB10	"����/����"
#define STR_MONEYTODAY_SUB11	"�ε���"

#define STR_YOUNHAP_ROOT		"���մ���"		// Root ���մ���
#define STR_YOUNHAP_SUB1		"��ġ"
#define STR_YOUNHAP_SUB2		"����"
#define STR_YOUNHAP_SUB3		"��ȸ"
#define STR_YOUNHAP_SUB4		"��ȭ"
#define STR_YOUNHAP_SUB5		"ü��"
#define STR_YOUNHAP_SUB6		"�ܽ�"
#define STR_YOUNHAP_SUB7		"����"
#define STR_YOUNHAP_SUB8		"��Ÿ"

#define STR_MAEKYONG_ROOT		"���ϰ���"			// Root �Ű洺��
#define STR_MAEKYONG_SUB1		"����"
#define STR_MAEKYONG_SUB2		"����"
#define STR_MAEKYONG_SUB3		"���/���"
#define STR_MAEKYONG_SUB4		"�߱�/����/����"
#define STR_MAEKYONG_SUB5		"����"
#define STR_MAEKYONG_SUB6		"�ε���"
#define STR_MAEKYONG_SUB7		"��ġ"
#define STR_MAEKYONG_SUB8		"��ȸ"
#define STR_MAEKYONG_SUB9		"�ι�/����"
#define STR_MAEKYONG_SUB10		"����"
#define STR_MAEKYONG_SUB11		"��ȭ"
#define STR_MAEKYONG_SUB12		"����/������"
#define STR_MAEKYONG_SUB13		"�缳/�÷�"
#define STR_MAEKYONG_SUB15		"����"
#define STR_MAEKYONG_SUB16		"English News"
#define STR_MAEKYONG_SUB18		"mbn"


#define STR_ETODAY_SUB4			"���"
#define STR_ETODAY_SUB5			"IT/�������"
#define STR_ETODAY_SUB6			"��Ȱ����"
#define STR_ETODAY_SUB7			"�ι�"

#define STR_NEWSP_ROOT			"������"		// Root ������
#define STR_NEWSP_SUB1			"�ֽ�"
#define STR_NEWSP_SUB2			"ä��"
#define STR_NEWSP_SUB3			"��ȯ"
#define STR_NEWSP_SUB4			"����"
#define STR_NEWSP_SUB5			"����/����ũ"
#define STR_NEWSP_SUB6			"���"
#define STR_NEWSP_SUB7			"����"
#define STR_NEWSP_SUB8			"����"
//#define STR_NEWSP_SUB9			"���������"

enum tid  {	xTotalRoot = 0,				// Ʈ�� INDEX
		xNoticeRoot,
		xNewsRoot,
		xHannewsRoot, 
		xHannewsSub1,
		xHannewsSub2,
		xHannewsSub3,
		xHannewsSub4,
		xHannewsSub5,
		xHannewsSub6,
		xHannewsSub7,
		xHannewsSub8,
		xHannewsSub9,
		xHannewsSub10,
		xHannewsSub11,
		xHannewsSub12,
		xHannewsSub13,
		xHannewsSub14,
		xHannewsSub15,
		xEDailyRoot,
		xEDailySub1,
		xEDailySub2,
		xEDailySub3,
		xEDailySub4,
		xEDailySub5,
		xEDailySub6,
		xEDailySub7,
		xEDailySub8,
		xEDailySub9,
		xInfoStockRoot,
		xInfoStockSub1,
		xInfoStockSub2,
		xInfoStockSub3,
		xInfoStockSub4,
		xInfoStockSub5,

		xMoneytodayRoot,
		xMoneytodaySub1,
		xMoneytodaySub2,
		xMoneytodaySub3,
		xMoneytodaySub4,
		xMoneytodaySub5,
		xMoneytodaySub6,
		xMoneytodaySub7,
		xMoneytodaySub8,
		xMoneytodaySub9,
		xMoneytodaySub10,	
		xMoneytodaySub11,

		xYounhapRoot,
		xYounhapSub1,
		xYounhapSub2,
		xYounhapSub3,
		xYounhapSub4,
		xYounhapSub5,
		xYounhapSub6,
		xYounhapSub7,
		xYounhapSub8,

		xMaekyoungRoot,
		xMaekyoungSub1,
		xMaekyoungSub2,
		xMaekyoungSub3,
		xMaekyoungSub4,
		xMaekyoungSub5,
		xMaekyoungSub6,
		xMaekyoungSub7,
		xMaekyoungSub8,
		xMaekyoungSub9,
		xMaekyoungSub10,
		xMaekyoungSub11,
		xMaekyoungSub12,
		xMaekyoungSub13,
//		xMaekyoungSub14,
		xMaekyoungSub15,
		xMaekyoungSub16,
//		xMaekyoungSub17,
		xMaekyoungSub18,

		xETodayRoot,
		xETodaySub1,
		xETodaySub2,
//		xETodaySub3,
		xETodaySub4,
		xETodaySub5,
		xETodaySub6,
		xETodaySub7,
//		xETodaySub8,
//		xETodaySub9,

		xNewspRoot,
		xNewspSub1,
		xNewspSub2,
		xNewspSub3,
		xNewspSub4,
		xNewspSub5,
		xNewspSub6,
		xNewspSub7,
		xNewspSub8,
//		xNewspSub9,


};

// select index
enum { 
	SEL_SS = 0,	// 0:��Ȳ
	SEL_GS,		// 1:����
	SEL_MT,		// 2:�Ӵ�������
	SEL_IS,		// 3:������Ź
	SEL_HK,		// 4:�Ѱ�
	SEL_ED,		// 5:�̵��ϸ�
	
	SEL_YH,		// 6:���մ���
	SEL_MK,		// 7:���ϰ���
	SEL_ET,		// 8:��������
	SEL_NP,		// 9:������
};

// detailSelect Index
enum { 
	DS_MT = 0,	// 0:�Ű�
	DS_IS,		// 1:������Ź
	DS_HK,		// 2:�Ѱ�
	DS_ED,		// 3:�̵��ϸ�
	DS_YH,		// 4:����
	DS_MK,		// 5:���ϰ���
	DS_ET,		// 6:��������
	DS_NP		// 7:������
};


#define SIZE_BUTTONS			18
#define COMBO_HEIGHT			20
#define OPTION_WND_HEIGHT		200

#define NEWS_CHECK_SECTION		"Ʈ��üũ ����"
#define NEWS_CHECK_FILE			"newsinfo.ini"
#define NEWS_CONFIG_FILE		"newstab"
#define NEWS_SCRAP_FILE			"news_scrap"
#define NEW_LINE				0x0a
#define	TABCHAR					0x09

enum	idPicker {idNotice = 0, 
				  idNews, 
				  idHanNews, 
				  idEDaily, 
				  idInfoStock, 
				  idMoneyToday, 
				  idYounhap, 
				  idMaekyoung, 
				  idEToday, 
				  idNewsp};

// ��������(����, ������ȸ)
#define COLOR_BLACK		PALETTERGB(0, 0, 0)
#define DEFAULT_COLOR	"0, 0, 0"
#define DEFAULT_CHECK	"FALSE"
#define SIGN_SEPER		","
#define GAP3			3
#define GAP5			1
#define GAP10			10
#define PICKER_WIDTH	60
#define PICKER_HEIGHT	20
#define STATIC_WIDTH1	70
#define STATIC_WIDTH2	100
#define STATIC_HEIGHT	21
#define NEWS_WIDTH		700
#define TREE_WIDTH		160
#define TOOLHEIGHT		45
#define GAPW			3
#define GAPMINW			2
#define ADDITIONALGAP	60

#define SECTION_OPTION	"����/������ȸ ����"
#define KEY_NOTICE		"����"
#define KEY_NEWS		"��Ȳ"
#define KEY_HANNEWS		"�Ѱ洺��"
#define KEY_EDAILY		"�̵��ϸ�"
#define KEY_INFOSTOCK	"������Ź"
#define KEY_MONEYTODAY	"�Ӵ�������"
#define KEY_CONTEXT		"������ȸ"
#define KEY_YOUNHAP		"���մ���"
#define KEY_MAEKYOUNG	"���ϰ���"
#define KEY_ETODAY		"��������"
#define KEY_NEWSP		"������"



#define SECTION_VIEW	"ȭ����ħ ����"
#define KEY_VIEW		"���̱�"

