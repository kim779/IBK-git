/********************************************************************/
/*																	                                */
/* ���� ���α׷����� ����ϴ� STRUCT/DEFINE/TYPEDEF ����			    */
/*																	                                */
/*										2003. 10. 28		TEXEN		                      */
/********************************************************************/
#ifndef SERVER_TYPEDEF
#define SERVER_TYPEDEF

#define TRCODE_FINDER					"80000"

#define		MAX_IDXCNT				20	/* �ִ� ��ǥ ����(����)		*/

#define		REAL_IDXCNT				18	/* �ִ� ��ǥ ����(�ܺ�)		*/
#define		MAX_PARAM					3		/* param �� �ִ� ���� 		*/
#define		MAX_CONDI					6		/* condition�� �ִ� ����	*/
#define		EXPLEN						80	/* ���ǻ��� �ִ� ���� 		*/

#define		RESULT_MAX				2000	/* �ִ� ���� ��         */
#define		RESULT_FIELD_MAX	20		/* ����ʵ� �ִ� ���� 	*/

/********************************************************************/
/*  ���ǰ˻� INPUT DATA                                             */
/********************************************************************/
typedef struct
{
	long    IndexNo; 					/* ��ǥ index						*/
	long    GroupNo; 					/* �Ҽ� �׷��ȣ				*/
	long    GroupIdxCnt;			/* �׷쳻 ��ǥ ����			*/
	long    TargetBase;				/* 0:�ϰ�, 1:�ְ�, 2:����, 3:1��, 4:3��	*/
														/* 5:5��,  6:10��, 7:15��, 8:30��, 9:60��*/
	long    Type; 						/* ��ǥ�� ����					*/
														/* 00XX	: ����� ��ǥ		*/
														/* 10XX	: ĵ��м�			*/
														/* 20XX	: �ü���ǥ			*/
														/* 30XX	: �繫��ǥ			*/
														/* 40XX	: �⺻�� �м�		*/
														/* 50XX	: �ñ׳� ��ǥ 	*/
														/* 60XX	: ����					*/
														/* 70XX : ���� (new)    */
														/* 80XX : ���� (new)    */
	long    Type1; 						/* ���� ����										  */
														/* 0: ���ذ� ����(����/����)      */
														/* 1: ���ؼ� ����(����/����)      */
														/* 2: ���ذ� ��(��) 				  */
														/* 3: ���ؼ� ��(��) 				  */
														/* 4: �� ��(��,<,<=,>,>=,==)  */
														/* 5: ���� ��(c1�̻�~c2����)    */
														/* 6: �߼�(���/�϶�)             */
														/* 7: ����(���/�϶�)             */
														/* 8: �����˻�									  */
														/* 9: �迭(���迭/���迭)         */
														/* 10: n% ���� �����˻�1	        */
														/* 11: n% ���� �����˻�2 	        */
														/* 20: ���� üũ (new)            */
														/* 30: ���� üũ (new)            */
														/* 90: ���ذ� ����(����/����)		##����##*/ 
														/* 91: ���ؼ� ����(����/����)		##����##*/
														/* 92: ���ذ� ��(��)    		##����##*/
														/* 93: ���ؼ� ��(��)    		##����##*/
														/* 94: ����(��,<,<=,>,>=,==)##����##*/
														/* 95: ���� ��(c1�̻�~c2����) ##����##*/
														/* 96: �߼�(���/�϶�)          ##����##*/
														/* 97: ����(���/�϶�)          ##����##*/
														/* 99:��Ÿ1                     ##����##*/
														/* 98:��Ÿ2                     ##����##*/
	long    Type2; 						/* �� ����						          */
														/* 0: ����/�̻�/���/���迭/�ż�  */
														/* 1: ����/����/�϶�/���迭/�ŵ�  */
														/* 2: ���ϱ���(������ǥ)        ##����##*/
														/* 3: ���ϱ���(������ǥ)        ##����##*/
														/* 99:��Ÿ                        */
	long    STerm;            /* n���� ����               		  */
	long    ETerm;            /* n���̳�                 			  */
	long    Rank;             /* �����˻� ���� ��               */
	long    Trend;            /* �߼�Ƚ��											  */
	long		SignalCnt1;				/* �߻�ȸ��	(new)	X������ X�� ����*/
	long		SignalCnt2;				/* �߻�ȸ��	(new)									*/
	long    ResultViewNum;		/* ��������� �ʵ� ����(0:������)	*/    

	char    IndexType;        /* �˻����� 0:�Ϲ� 1:���� 2:����  */
	char    SearchType;       /* �˻����� 0:�Ϲ� 1:���������	2:���� 3:����   (new) */

	char    Operator;         /* ������'0':OR '1':AND '2':NEXT	*/
	char    Reverse;          /* ������� '0':A, '1':Not A		  */
	char    IdxContiGubun;    /* ��ǥ�� ���ӹ߻� ����
															'0':���Ϲ߻�, 1:���ӹ߻���ǥ		*/
	char    Element;          /* ���ǽ� ��빮��(A) (new)     */
	char		Filler[2];

	double  Param[MAX_PARAM];      	/* ��ǥ���� ������ (ex.12,26,9)	*/
	double  Condition[MAX_CONDI];   /* ��Ÿ ���� ������					    */
} User_Define;
#define SZ_User_Define sizeof(User_Define)

/********************************************************************/
/*  ����� ���� ���ǰ˻� ��Ŷ                                       */
/********************************************************************/
typedef struct
{
	long		TrIndex;
	long    TargetIdxCnt;                		/* ��� ��ǥ ����       */
	long    TargetResultViewCnt;            /* ��� �������ʵ� ���� */
	char    TargetExp[EXPLEN];              /* ������� ���� ǥ�� ��*/
	
	long    ComplexIdxCnt;                  /* ���� ��ǥ ����       */
	long    ComplexResultViewCnt;           /* ���� �������ʵ� ���� */
	char    ComplexExp[EXPLEN];             /* �������� ���� ǥ�� ��*/
	
	long    FieldEditerCnt;                 /* �ʵ����� ����        */
	long    FieldList[REAL_IDXCNT];					/* �ʵ����� ��ȣ����Ʈ  */

	long		ScriptCnt;											/* ��ũ��Ʈ ���� (new)	*/
	
	/*
	char    TargetBitChk;                   // TargetBit ��� ���� üũ
	char		Jongmoklist[RESULT_MAX][6];			// ���ɱ׷� �����ڵ�
	char    Filler;                      		// Filler
	*/
	
	//# 2004. 04 => ����!
	char    TargetBitChk;                   	/* TargetBit ��� ���� üũ */
  char    TargetBitlist[RESULT_MAX];
  char    Sort_flag;
    
  long    BackDay;				/* backtest ����(�̻��:-1) */	
  long    BackTime;				/* backtest �ð�(�̻��:-1) */	
} User_Finder;
#define SZ_User_Finder  sizeof(User_Finder)

/********************************************************************/
/*  Output Packet Struct                                            */
/********************************************************************/
typedef struct {
	long		TrIndex;
	long    TotalJongCnt;			  /* �˻��� ���� ���� ��      */
	long    OutFieldCnt;			  /* Out Field Count          */
	long    OutListPacketSize;  /* ��� List ������(������) */
	long	  FindTime;					  /* �˻��ð�									*/

	char    ResultBit[RESULT_MAX];	    /* ������� bit list        */
} OutPacket;
#define SZ_OutPacket sizeof(OutPacket)

/************************************************************************/
/*  Output List Packet Struct                                           */
/************************************************************************/
typedef struct {
	char	Daebi;					/* ���ϴ�� ��ȣ 	*/ // 1:���� 2:��� 3:���� 4:���� 5:�϶�
	char	Code[6];				/* ���� �ڵ�	 		*/
	char	KoreanName[20];	/* �����		 			*/ 
	char	Filler[5];
	long	OrderRank;			/* �����˻� ���� 	*/	
	long	BasePrice;
	long	ResultField[RESULT_FIELD_MAX]; 
} OutListPacket;
#define SZ_OutListPacket sizeof(OutListPacket)

/************************************************************************/
/*  ����� �Լ� ��Ŷ list(����� �Լ� ������ ����� ����) (new)         */
/************************************************************************/
typedef struct
{
    char    FuncName[20];		/* ����� �Լ� ��		*/
    long    IndexNo;        /* �ش� ��ǥ ��ȣ   */
    long    ScriptLen;      /* ��ũ��Ʈ�� ����  */
} User_Script;








//# [������] ##########################3
#define JOBDATE_MAX  9999

#define TRCODE_JOBDATE				"92820"

typedef struct tagJobDate_In
{
  char    date[8];
  char    count[4];
  char	  chNull;
} JobDate_In;
#define SZ_JobDate_In sizeof(JobDate_In)

typedef struct tagJobDate_Out
{
	char        count[4];
	char				date[JOBDATE_MAX][8];
} JobDate_Out;
#define SZ_JobDate_Out sizeof(JobDate_Out)

#endif // SERVER_TYPEDEF

