#pragma once

#define MAX_R			20      /* max grid row     */
#define TIT_LEN			132     /* length of title  */
#define DAT_LEN         1536
#define MAX_CODES       10	
#define SIHWANG_GUBN    1
#define M_LINE			15      /* max. # of lines  */
#define M_COL			132     /* ����             */
#define X_COL			80      /* ����, ��Ȳ       */
#define MAX_PIDOTUJA	64

#define TK_PIBO7710		0
#define TK_PIBONEWS		1
#define TK_PIDOSIHW		2
#define TK_PIDOTUJA		4

/* GRID_T.gdir */
#define GD_TOP      1       /* top ending           */
#define GD_BOTTOM   2       /* bottom ending        */

/* GRID_T.aflg */
#define AD_NOT      0       /* reset                */
#define AD_TOP      1       /* add to top           */
#define AD_BOTTOM   2       /* add to bottom        */

/* GRID_T.xpos */
#define PS_END      0x40               /* no continuoue          */
#define PS_PREV     0x01               /* enable previous        */
#define PS_NEXT     0x02               /* enable next            */
#define PS_PNXT     (PS_PREV|PS_NEXT)  /* enable next & previous */
#define PS_SORT     0x04               /* no local sorting       */

/* GRID_T.ikey */
#define KY_ENTER    0       /* enter : default          */
#define KY_PGUP     1       /* PageUp                   */
#define KY_PGDN     2       /* PageDn                   */
#define KY_SCUP     3       /* ScrollUp                 */
#define KY_SCDN     4       /* ScrollDn                 */
#define KY_FIRST    5       /* First or End             */
#define KY_SORT     6       /* click sorting column     */

typedef struct {            /* GRID form input format       */
    char    vrow[2];        /* visible rows                 */
    char    nrow[4];        /* # of rows                    */
    char    vflg[1];        /* if '1' variable row          */
    char    gdir[1];        /* add to top or bottom         */
							/* '1':top, '2':bottom          */
    char    sdir[1];        /* sort direction               */
							/* '1':asc, '2':desc            */
    char    scol[16];       /* sorting column symbol        */
    char    ikey[1];        /* key action                   */
							/* '0':Enter or Other           */
							/* '1':PgUp '2':PgDn            */
							/* '3':ScrollUp '4':ScrollDn    */
							/* '5':First or End             */
							/* '6':Please sort              */
    char    page[4];        /* current page number          */
    char    save[80];       /* grid inout save field        */
} GRID_I;

typedef struct {            /* GRID form output format         */
    char    aflg[1];        /* add action to top or bottom     */
							/* '0':replace '1':top, '2':bottom */
    char    sdir[1];        /* sort direction                  */
							/* '1':asc, '2':desc               */
    char    scol[16];       /* sorting column symbol           */
    char    xpos[1];        /* continuous status               */
							/* 0x40: default(normal)           */
							/* 0x01: PgUp, ScrUp enable        */
							/* 0x02: PgDn, ScrDn enable        */
							/* 0x04: no local sorting          */
    char    page[4];        /* # of page                       */
    char    save[80];       /* grid inout save field           */
} GRID_O;

typedef struct {      /* NEXT KEY       */
    char    cod2[6];        /* �����ڵ�       */
    char    fday[8];        /* ��ȸ����       */
//  char    gubn[13];       /* ����           */
	char    gubn[20];       /* ����           */  //test  13->20
    char    skey[20];       /* �˻���         */
    char    isrn[10];       /* ISAM KEY       */
} pibo7710_nkey;

typedef struct {
    char    cod2[6];        /* �����ڵ�                         */
    char    fday[8];        /* ��ȸ����                         */
//	char    gubn[13];       /* ����                             */
	char    gubn[20];       /* ����                             */ //test  13->20
							/* gubn[0 ] : ��Ȳ                  */
							/* gubn[1 ] : ����                  */
							/* gubn[2 ] : ��������              */
							/* gubn[3 ] : ��Ȳ�Ӻ�  -������Ź   */
							/* gubn[4 ] : ���ڽŹ�              */
							/* gubn[5 ] : �̵��ϸ�              */
							/* gubn[6 ] : �����ε���-������Ź   */
							/* gubn[7 ] : ���մ���              */
							/* gubn[8 ] : �Ӵ�������            */
							/* gubn[9 ] : �Ѱ洺��              */
							/* gubn[10] : DUMMY                 */
							/* gubn[11] : DUMMY                 */
    char    skey[20];       /* �˻���                           */
    char    subg[3];        /* �з��ڵ�(��ü:'999')             */
    GRID_I  grid_i;
    char    type[ 1];       /* Ÿ��('1':����, �׿�:��ü);       */
} pibo7710_mid;

typedef struct {
    char    date[8];        /* ��¥                             */
    char    time[4];        /* �ð�                             */
    char    titl[TIT_LEN];  /* ����                             */
    char    gubn[12];       /* �������и�                       */
    char    subg[20];       /* �з��ڵ��                       */
    char    cod2[12];       /* �����ڵ�                         */
    char    hnam[20];       /* �����                           */
    char    gisn[10];       /* CISAM RECORD NO.(GRID)           */
    char    sisn[10];       /* CISAM RECORD NO.(������ȸ)       */
    char    keyv[24];       /* ������ȸ�� KEY VALUE             */
							/* ����(8)+��������(2)+�з��ڵ�(4)+�Ϸù�ȣ(10) */
    char    flag[1];        /* TEXT: 0, HTML: 1                 */
} pibo7710_grid;

typedef struct {
    GRID_O  grid_o;
    char    nrec[4];
    pibo7710_grid grid[MAX_R];
} pibo7710_mod;

typedef struct {
    char    date[8];        /* �ڷ�����     */
    char    gubn[2];        /* ��������     */
    char    subg[4];        /* �з��ڵ�     */
    char    seqn[10];       /* �Ϸù�ȣ     */
    char    titl[TIT_LEN];  /* ���� ����    */
} pibonews_mid;

typedef struct {
    char    titl[TIT_LEN];  /* ���� ����        */
    char    subcod[10][12]; /* ���� �ڵ�        */
    char    size[5];        /* size of data     */
    char    data[1];
} pibonews_mod;


struct  pnewsrc_mid {
    char    date[8];        // �ڷ�����     
    char    gubn[2];        // ��������     
    char    subg[4];        // �з��ڵ�     
    char    seqn[10];       // �Ϸù�ȣ     
    char    titl[TIT_LEN];     // ���� ����        
};

struct  pnewsrc_mod {
	char	cflag[1];				//2017.02.10 KSJ ������ ����
	char    titl[TIT_LEN];     // ���� ����        
    char    subcod[10][12]; // ���� �ڵ� 
    char    size[7];        // size of data     	//2015.11.20 KSJ size 5 -> 7
    char    data[1];
};


typedef struct {
    char code[6];
} pidosihw_jdat;

typedef struct {
    char flag[1];                   /* I,U,D �Է±���    */
	char step[1];                   /* F, M, L           */
    char usid[8];                   /* �Է����� ID(���) */
    char date[8];                   /* �Է���            */
    char time[6];                   /* �Է½ð�          */
    char seqn[8];                   /* �Ϸù�ȣ          */
    char gubn[8];                   /* ��������          */
    char subg[8];                   /* �з��ڵ�          */
    char titl[TIT_LEN];             /* ����              */
    pidosihw_jdat cods[MAX_CODES];  /* �����ڵ�          */
    char dlen[8];                   /* ��������          */
    char data[1];                   /* ����              */
} pidosihw_mid;

typedef struct {
    char seqn[8];   /* �Է�Sequence      */
    char emsg[128]; /* ��� �޼���       */
} pidosihw_mod;

typedef struct {
    char tjcd[3];           /* �������ڵ�      */
    char cod2[12];          /* �����ڵ�        */
    char msgb[1];           /* 1(�ż�),2(�ŵ�) */
    char xvol[8];           /* ����(��)        */
} pidotuja_grid;

typedef struct {
    char flag[1];           /* I(Insert)       */
    char seqn[2];           /* ȸ��            */
    char nrec[8];           /* record #        */
    pidotuja_grid grid[1];    
} pidotuja_mid;

typedef struct mod {
    char emsg[128];         /* ����޼���      */
} pidotuja_mod;      

