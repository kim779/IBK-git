#ifndef __DOORI__INTERESTDEF__HEADER__
#define	__DOORI__INTERESTDEF__HEADER__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TYPE_USER_ITEMS		 "��������"
#define TYPE_HIGH_ITEMS		 "���Ѱ�"		
#define TYPE_HIGHING_ITEMS	 "���"		
#define TYPE_EQUAL_ITEMS	 "����"		
#define TYPE_LOW_ITEMS		 "���Ѱ�"		
#define TYPE_LOWING_ITEMS	 "�϶�"		
#define TYPE_NEWHIGH_ITEMS   "�Ű�"      
#define TYPE_NEWLOW_ITEMS    "����"     

#define TYPE_USER_ITEM		 "00"		//: �������� 
#define TYPE_HIGH_ITEM		 1		//: ���Ѱ� ����
#define TYPE_HIGHING_ITEM	 2		//: ��� ����
#define TYPE_EQUAL_ITEM		 3		//: ���� ����
#define TYPE_LOW_ITEM		 4		//: ���Ѱ� ����
#define TYPE_LOWING_ITEM	 5		//: �϶� ����
#define TYPE_NEWHIGH_ITEM    6       //: �Ű�
#define TYPE_NEWLOW_ITEM     7       //: ���� 


//2004.12.09 by LYH Ư������ ����(20����) 
#define TYPE_TODAY		1
#define TYPE_YESTERDAY	2

#define TYPE_KOSPI_HIGH_ITEMS		"���Ѱ�(�ŷ���)"
#define TYPE_KOSDAQ_HIGH_ITEMS		"���Ѱ�(�ڽ���)"
#define TYPE_KOSPI_HIGHING_ITEMS	"���(�ŷ���)"
#define TYPE_KOSDAQ_HIGHING_ITEMS	"���(�ڽ���)"
#define TYPE_ALL_EQUAL_ITEMS		"����"		
#define TYPE_KOSPI_LOW_ITEMS		"���Ѱ�(�ŷ���)"
#define TYPE_KOSDAQ_LOW_ITEMS		"���Ѱ�(�ڽ���)"
#define TYPE_KOSPI_LOWING_ITEMS		"�϶�(�ŷ���)"
#define TYPE_KOSDAQ_LOWING_ITEMS	"�϶�(�ڽ���)"
#define TYPE_ALL_NEWHIGH_ITEMS			"�Ű�"
#define TYPE_ALL_NEWLOW_ITEMS			"����"

#define TYPE_PREV_KOSPI_HIGH_ITEMS		"���ϻ��Ѱ�(�ŷ���)"
#define TYPE_PREV_KOSDAQ_HIGH_ITEMS		"���ϻ��Ѱ�(�ڽ���)"
#define TYPE_PREV_KOSPI_HIGHING_ITEMS	"���ϻ��(�ŷ���)"
#define TYPE_PREV_KOSDAQ_HIGHING_ITEMS	"���ϻ��(�ڽ���)"
#define TYPE_PREV_EQUAL_ITEMS			"���Ϻ���"		
#define TYPE_PREV_KOSPI_LOW_ITEMS		"�������Ѱ�(�ŷ���)"
#define TYPE_PREV_KOSDAQ_LOW_ITEMS		"�������Ѱ�(�ڽ���)"
#define TYPE_PREV_KOSPI_LOWING_ITEMS	"�����϶�(�ŷ���)"
#define TYPE_PREV_KOSDAQ_LOWING_ITEMS	"�����϶�(�ڽ���)"

enum ETC_TYPE
{
TYPE_KOSPI_HIGH_ITEM=1,
TYPE_KOSDAQ_HIGH_ITEM,
TYPE_KOSPI_HIGHING_ITEM,
TYPE_KOSDAQ_HIGHING_ITEM,
TYPE_ALL_EQUAL_ITEM,
TYPE_KOSPI_LOWING_ITEM,
TYPE_KOSDAQ_LOWING_ITEM,
TYPE_KOSPI_LOW_ITEM,
TYPE_KOSDAQ_LOW_ITEM,
TYPE_ALL_NEWHIGH_ITEM,
TYPE_ALL_NEWLOW_ITEM,
TYPE_PREV_KOSPI_HIGH_ITEM,
TYPE_PREV_KOSDAQ_HIGH_ITEM,
TYPE_PREV_KOSPI_HIGHING_ITEM,
TYPE_PREV_KOSDAQ_HIGHING_ITEM,
TYPE_PREV_EQUAL_ITEM,
TYPE_PREV_KOSPI_LOWING_ITEM,
TYPE_PREV_KOSDAQ_LOWING_ITEM,
TYPE_PREV_KOSPI_LOW_ITEM,
TYPE_PREV_KOSDAQ_LOW_ITEM
};

#define TYPE_ALL			 "0"		//�ڽ���, �ڽ��� ��� 
#define TYPE_KOSPI			 "1"
#define TYPE_KOSDAQ			 "2"

#define DEF_INTTREE_UPJONG		0
#define DEF_INTTREE_USER		1
#define DEF_INTTREE_ETC         2

#define DEF_MASTER_ZZIM		0
#define DEF_MASTER_IF		1
#define DEF_MASTER_SIGNAL	2
#define DEF_MASTER_NEWS		3

#define DEF_INTTREES_UPJONG		"����"
#define DEF_INTTREES_USER		"��������"
#define DEF_INTTREES_ETC		"Ư������"
#define DEF_INTTREES_FUTURE		"����"
#define DEF_INTTREES_OPTION		"�ɼ�"
#define DEF_INTTREES_CALL		"�ݿɼ�"
#define DEF_INTTREES_PUT		"ǲ�ɼ�"
#define DEF_INTTREES_ZZIM		"��"
#define DEF_INTTREES_THEME		"�׸�"
#define DEF_INTTREES_IF			"�ӽñ׷�"
#define DEF_INTTREES_SIGNAL      "��ȣó��"
#define DEF_INTTREES_HAVE		"��������"
#define DEF_INTTRESS_NEWS		"��������"
#define DEF_INTTREES_HISTORY	"�ֱ�����"

#define DEF_INTEREES_CURRENT    "�ü�����"
#define DEF_INTEREES_INVEST		"�����ڵ���"
#define DEF_INTEREES_STSIGN		"��ǥ��ȣ"
#define DEF_INTEREES_STRATEGY   "��õ����"
#define DEF_INTEREES_USERMYNET	"����� ��������"

#define	DEF_STR_KOSPI		"KOSPI"				// �ŷ���
#define	DEF_STR_KOSDAQ		"KOSDAQ"			// �ڽ���
#define	DEF_STR_GROUP		"�׷��"			// �׷�
#define	DEF_STR_UALL		"����"				// ����
#define DEF_STR_IFLIST      "Ư¡��"

#define D_TITLE			  0
#define D_UPJONG		100
#define D_KOSIP			101
#define D_KOSDOQ		102
#define D_GROUP			103 
#define D_FUTURE        104
#define D_OPTION	    105
#define D_ZZIM          106 
#define D_THEME			107
#define D_IF            108
#define D_SIGNAL        109
#define D_USERHAVE		110
#define D_UALL			111
#define D_STOCKIF		112
#define D_NEWS			113

#define D_USER			200
#define D_USERSEL		201
#define D_USERHAVESEL   202

#define D_ETC           300
#define D_ETCSEL		301 

#define D_OPTIONCALL    401
#define D_OPTIONPUT		402

#define D_IFMYNET		501
#define D_IFMYNETP		502
#define D_IFMYNETITEM	503

#define D_INVEST		211
#define D_STSIGN		212
#define D_STRATEGY		213
#define D_IFCRRENT		214

#define D_THEMEITEM     701

#define D_HISTORY       711 

#define MAX_IFLIST_CONDITION 4
#define MAX_ITEM_COUNT 50

#define USER_GROUP			0
#define UPJONG_KUGROUP		1
#define UPJONG_JGROUP		2 
#define TR_GROUP			3 
#define DEFINE_GROUP		4
#define ZZIM_GROUP          5
#define UPJONG_GROUP		6
#define THEME_GROUP			7
#define LOOK_GROUP			8
#define TEMP_GROUP			9

//#define MAX_GROUP_COUNT 60
#define MAX_GROUP_COUNT 50

#define FILEPATH_USERLIST "IntList.Dat"
#define FILEPATH_USERLIST01 "IntList01.Dat"
#define FILEPATH_USERIF   "IntIF.Dat"

#define MAX_CONNECT_DRDS 6

#define REAL_NS31 0 // �������簡
#define REAL_NX13 1 // ����ȣ��

#define REAL_NSC0 2 //���� ���簡 
#define REAL_NSH2 3 //���� ȣ��
//#define REAL_NSM0 4 //���� �̰��� ���� 

#define REAL_NOC0 4 //���� ���簡 
#define REAL_NOH2 5 //���� ȣ��
//#define REAL_NOM0 7 //���� �̰��� ���� 



#define REAL_S31 "S31" // �������簡
#define REAL_X13 "X13" // ����ȣ��

#define REAL_SC0 "SC0" //���� ���簡 
#define REAL_SH2 "SH2" //���� ȣ��

#define REAL_OC0 "OC0" //���� ���簡 
#define REAL_OH2 "OH2" //���� ȣ��

#define REG_MOVE_UP     1
#define REG_MOVE_DOWN   2
#define REG_MOVE_TOP    3
#define REG_MOVE_BOTTOM 4
#endif // __DOORI__INTERESTDEF__HEADER__