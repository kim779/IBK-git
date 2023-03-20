#pragma once

#define MAX_R		20

#define title_len	132

#define MAX_NEWS 8			
#define MAX_NEWS_SUB 20
#define MAX_NEWS_COLOR 8

#define WM_COMBO_ENTERED WM_USER + 1007
//#define NEWS_SIZEGUBN 13				// ������ �߰��� �������� ����
#define NEWS_SIZEGUBN 20				// ������ �߰��� �������� ����

typedef struct {
	char fday[8];			// ��ȸ����
	char vgbn[2];			// ��ȸ����  "00"
	char gubn[12];			// �������� 
	char subg[3];			// �з��ڵ� "999"
	char sdir[1];			// ����
	char cpag[4];			// ��û ������ 
} req_news_count;

typedef struct {
	char emsg[50];
	char cpag[4];
	char tpag[4];
	char nrec[4];
} res_news_count_header;

typedef struct {
	char titl[80];
	char flag[1];
	char time[6];
	char vcnt[6];
	char date[8];
	char gubn[2];
	char subg[4];
	char seqn[10];
} res_news_count_content;

typedef struct {
	int iCurPage;
	int iMaxPage;
	int iRecCnt;
} news_count_page_header;

// �з�		  TR		  
//-----------------------------------
//���ս�Ȳ	 PIBO5501
//����		 BO550110
//�Ӵ�������	BO550120
//�Ѱ洺��	 BO550140
//�̵��ϸ�	 BO550150

//#pragma pack(push,1)
typedef struct  {                       /* GRID form input format       */
        char    vrow[2];                /* visible rows                 */
        char    nrow[4];                /* # of rows                    */
        char    vflg[1];                /* if '1' variable row          */
        char    gdir[1];                /* add to top or bottom         */
                                        /* '1':top,     '2':bottom      */
        char    sdir[1];                /* sort direction               */
                                        /* '1':asc,     '2':desc        */
        char    scol[16];               /* sorting column symbol        */
        char    ikey[1];                /* key action                   */
                                        /* '0':Enter or Other           */
                                        /* '1':PgUp     '2':PgDn        */
                                        /* '3':ScrollUp '4':ScrollDn    */
                                        /* '5':First or End             */
                                        /* '6':Please sort              */
        char    page[4];                /* current page number          */
        char    save[80];               /* grid inout save field        */
} GRID_I;

typedef struct  {           	/* GRID form output format  */
    char    aflg[1];        	/* add action to top or bottom  */
                    		/* '0':replace '1':top, '2':bottom */
    char    sdir[1];        	/* sort direction       */
                    		/* '1':asc, '2':desc    */
    char    scol[16];       	/* sorting column symbol    */
    char    xpos[1];        	/* continuous status        */
                    		/* 0x40: default(normal)    */
                    		/* 0x01: PgUp, ScrUp enable */
                    		/* 0x02: PgDn, ScrDn enable */
                    		/* 0x04: no local sorting   */
    char    page[4];        	/* # of page            */
    char    save[80];       	/* grid inout save field    */
} GRID_O;               	/* grid header          */


enum sendtr_type{
	type_gongsi = 0,			// ����
	type_etoday,				// �������� 
	type_infostock,				// ������Ź 
	type_eletcnews,				// ���ڽŹ�
	type_edaily,				// �̵��ϸ�
	type_worldindex,			// �����ε��� 
	type_yunhab = 7,			// ���մ���
	type_moneytoday,			// �Ӵ������� 
	type_hk,					// �Ѱ洺�� 
	type_mk,					// �Ű洺�� 
	type_asia,
	type_total,					// ��ü 
	type_content,				// �������� 
	type_category,
	type_subtotal,			// ī�װ��� ��ȸ
	type_symbol = 20,
	type_news_count_day,		// �Ϻ�
	type_news_count_week,		// �ֺ� 
	type_news_count_month,		// ���� 
	type_scrap
};

//�Ѱ� �߰� ���� �κ�
struct mid_title{
	
	char    cod2[6];	/* �����ڵ�			*/
	char    fday[8];	/* ��ȸ����			*/
	char	gubn[NEWS_SIZEGUBN];	/* ����			*/
				/* gubn[0] : ��Ȳ			*/
				/* gubn[1] : ����			*/
				/* gubn[2] : ��������		*/
				/* gubn[3] : ��Ȳ�Ӻ�		*/
				/* gubn[4] : ���ڽŹ�		*/
				/* gubn[5] : �̵��ϸ�		*/
				/* gubn[6] : �����ε���		*/
				/* gubn[7] : ���մ���		*/
				/* gubn[8] : �Ӵ�������		*/
				/* gubn[9] : �Ѱ����		*/
				/* gubn[10] : �Ű�			*/				
				/* gubn[12] : �ƽþư���	*/
	char    skey[20];       /* �˻���			*/
	
	char	sugb[3];	// �з� ����
	GRID_I  grid_i;
	char	type[1];	/* ����(1:��ü �׿�:��ü)*/
};

struct  grid    {
        char    date[8];        /* ��¥                         */
        char    time[4];        /* �ð�                         */
        char    titl[title_len];/* ����                         */
        char    gubn[12];       /* ��������                     */
        char    subg[20];       /* �з��ڵ�								 */
	char	code[12];			/* �����ڵ�			*/
        char    hnam[20];       /* �����                       */
        char    gisn[10];       /* CISAM RECORD NO.(GRID)       */
        char    sisn[10];       /* CISAM RECORD NO.(������ȸ)   */
	char    keyv[24];			/* ������ȸ�� KEY VALUE     �������� �߰��� ���� +2   */		
        char    flag[1];        /* TEXT: 0, HTML: 1             */
};



//-------------------
//output
//-------------------
// ����

struct  mod_title{
//        char    isrn[10];       /* CISAM RECORD NO.             */
        GRID_O  grid_o;
        char    nrec[4];
        struct  grid    grid[MAX_R];
};



//--------------	
//������ȸ
//--------------
//pnewsrc
struct  pnewsrc_mid {
  //  char    date[8];        // �ڷ�����     
 //   char    gubn[2];        // ��������     
  //  char    subg[4];        // �з��ڵ�     
  //  char    seqn[10];       // �Ϸù�ȣ   
	char	keyv[24];
    char    titl[title_len];     // ���� ����        
};

struct  pnewsrc_mod {
	char	cflag[1];				//2017.02.10 KSJ ������ ����
	char    titl[title_len];     // ���� ����        
    char    subcod[10][12]; // ���� �ڵ� 
    char    size[7];        // size of data     	//2015.11.20 KSJ size 5 -> 7
    char    data[1];
};

//PIBONEWS
struct  mid_content
{
        //char    isrn[10];             /* CISAM RECORD NO.     */
/*	char	date[8];
	char	gubn;
	char	subgubn;
	char	seqn[10];*/
	char	keyv[24];		/* Key Value */
    char    titl[title_len];        /* ���� ����            */
};

struct  mod_content
{
        char    titl[title_len];        /* ���� ����            */
		char	subcod[10][12];
        char    size[5];                /* size of data         */
        char    data[1];
};

struct  news_info_ex {
        char    gisn[10];       /* CISAM RECORD NO.(GRID)       */
        char    sisn[10];       /* CISAM RECORD NO.(������ȸ)   */
		char    keyv[24];		/* ������ȸ�� KEY VALUE         */
		char	code[12];		/* �ڵ�							*/
        char    flag[1];        /* TEXT: 0, HTML: 1             */
};


//#pragma pack(pop)
struct news_info{
	CString    date;        /* ��¥                         */
	CString    time;        /* �ð�                         */
	char       titl[title_len];		/* ����                 */
	CString	   gubn;		// �������� 
	CString    subg;       /* �з��ڵ�                     */
	CString    hnam;       /* �����                       */
	CString    gisn;       /* CISAM RECORD NO.(GRID)       */
	CString    sisn;       /* CISAM RECORD NO.(������ȸ)   */
	CString    keyv;       /* ������ȸ�� KEY VALUE         */
	CString    flag;        /* TEXT: 0, HTML: 1             */

	CString    snam;		//�з���
	CString    gnam;		//���и�
	CString	   key;			//key
	CString	   code;		//�����ڵ�
	int		   rCnt;		// ��ȸ�� 
		
		news_info()
		{
			date = "";        /* ��¥                         */
			time = "";        /* �ð�                         */
			memset(titl,0x00,title_len);
			//   titl = "";		/* ����                         */
			gubn = "";		// �������� 
			subg = "";       /* �з��ڵ�                     */
			hnam = "";       /* �����                       */
			gisn = "";       /* CISAM RECORD NO.(GRID)       */
			sisn = "";       /* CISAM RECORD NO.(������ȸ)   */
			keyv = "";       /* ������ȸ�� KEY VALUE         */
			flag = "";        /* TEXT: 0, HTML: 1             */
			snam = "";		//�з���
			gnam = "";		//���и�
			key = "";			//key
			code = "";		//�����ڵ�
			rCnt = 0;
		}
};

struct group {
	int gubn;
	int subg;
};

struct  subg_group {         /* ?�з��庰 gubn, subg ��� */
	int subg;
	struct group sgroup[100];
};

#include <afxtempl.h>
#define		MAXCNT			130
struct NewsList
{
	CString		sGubnName;
	CString		sGubn;
	CString		sKey;

	BOOL		bCategory;
	int			ibulruCnt;
	CString		sBulruGubnList[MAXCNT];
	CString		sBulruNameList[MAXCNT];
	int			iCheck[MAXCNT];

	int			ikey;
	int			iGubn;
	HTREEITEM	hTreeItem;
	int			iAllCheck;
	int			iFirst;
	subg_group  stype;

	NewsList(CString sData, CString sData2, int iCnt, CString* sList, CString* sList2 )
	{
		sGubnName = sData;
		sGubn = sData2;
		iGubn = atoi(sGubn);
		ibulruCnt = iCnt;
		for(int i=0; i<iCnt; i++)
		{
			sBulruGubnList[i] = sList[i];
			sBulruNameList[i] = sList2[i];
			iCheck[i] = 1;
		}

		iAllCheck = 1;
		iFirst = 1;
		bCategory = FALSE;
	};

	NewsList(CString sData, CString sData2, int idata, CString key, BOOL bFlag, subg_group type, HTREEITEM hitem)
	{
		sGubnName = sData;
		sGubn = sData2;
		iGubn = atoi(sGubn);
		sKey = key;
		bCategory = bFlag;
		ikey = idata;
		hTreeItem = hitem;
		stype = type;

		iAllCheck = 1;
		iFirst = 1;
	};

	NewsList(CString sData, CString sData2, int iCnt, CString* sList, CString* sList2, int idata,  HTREEITEM hitem)
	{
		sGubnName = sData;
		sGubn = sData2;
		iGubn = atoi(sGubn);
		ibulruCnt = iCnt;
		for(int i=0; i<iCnt; i++)
		{
			sBulruGubnList[i] = sList[i];
			sBulruNameList[i] = sList2[i];
			iCheck[i] = 1;
		}

		ikey = idata;
		hTreeItem = hitem;
		iAllCheck = 1;
		iFirst = 1;
		bCategory = FALSE;
	};

	NewsList()
	{
		iGubn = ibulruCnt = 0;
		sGubnName = _T("");
		iFirst = 1;
	};
};
typedef CArray<NewsList, NewsList> CNewsListArray;

struct AList
{
	CString		sList[MAXCNT];
	int			iDataCnt = 0;
	int			iList[MAXCNT]{};


	CString		sTxt;
	int			iVal=0;
	AList(int icnt, CString* sData)
	{
		iDataCnt = icnt;
		for(int i=0; i<icnt; i++)
			sList[i] = sData[i];
	};

	AList(int icnt, int* iData)
	{
		iDataCnt = icnt;
		for(int i=0; i<icnt; i++)
			iList[i] = iData[i];
	};

	AList(CString sData, int iData)
	{
		sTxt = sData;
		iVal = iData;
	};

	AList()
	{
		iDataCnt = 0;
	};

};
typedef CArray<AList, AList> CAListArray;
