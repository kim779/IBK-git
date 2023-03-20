#define TRNO_DRAW		"50011"
#define RESULT_FIELD_MAX	20	/* ����ʵ� �ִ� ���� 	*/

/* 
** DRAW �˻� Packet
*/
typedef struct
{
    long        iAccuVal    ;               /* �ֱ� ����ġ ���뿩�� : 1~ 10, ������� 0����.. (0.005)*/
                                              
    long        iDraw_Cnt   ;               /* �׸��� �Ⱓ����(20~120)  */
    long        iErrRate    ;               /* ������				*/
    long        iGapRate    ;               /* ���� ����			*/
    long        iDraw_Value [120];          /* �׸��� ��ǥ��(0~100)	: �׸��� ������� 999�� �Է�*/
                                              
} DRAW_DATA;
#define SZ_DRAW_DATA	sizeof(DRAW_DATA)

typedef struct
{
    long		nTrIndex;
    
    long        iS_Time     ;               /* ���ð�	: ��/��/�� ��.. ���ǰ˻��� ����		*/
                                              
    long        iTarget     ;               /* ������� => 4 ����	   : ��,��,��,��,�ŷ���(1,2,3,4,5)		*/
                                            
    long        iMoveAvg   	;           /* ����Ⱓ => 1����: ���� ������� 1 �Է�				*/
                                              
    long        iS_Pos      ;               /* �˻� ������ġ		*/
    long        iS_Range    ;              /* �˻� ����			*/
    long        iSimilar    ;            /* ���絵				*/
    char		sBoxFlag;		/* Ⱦ�� üũ ����	   : Ⱦ�� üũ�ÿ� 1�� üũ	*/
	                                
	                                          
    long	    FieldEditerCnt;             /* �ʵ����� ����   [Fix] */
    long	 	FieldList[RESULT_FIELD_MAX];	/* �ʵ����� ��ȣ����Ʈ [Just Fix] */
	                                          
    DRAW_DATA   tDraw_Cond;
} DRAW_IN;
#define SZ_DRAW_IN		sizeof(DRAW_IN)

typedef struct
{
    long nTrIndex;
    
    long  lTotalJongCnt;		/* �˻��� ���� ���� ��      */
    long  lOutFieldCnt;			/* Out Field Count          */
    long  lOutListPacketSize;		/* ��� List ������(������) */
    long  lFindTime;			/* �˻��ð�					*/
} DRAW_OUT_HEAD;
#define SZ_DRAW_OUT_HEAD	sizeof(DRAW_OUT_HEAD)

typedef struct
{
    char        sJmcode     [6];
    char        sHname      [20];
    char        sSign       [1];
    char        sMarket     [1];

    char        sSimilar    [6];

    long        iTarget;	//# ���ֿ���
    char        sDateTime_Start [14];
    char        sDateTime_End   [14];
    
    double	dFieldData[RESULT_FIELD_MAX];	//# 5
       
    /*
    char        sPrice      [9];	//���簡
    char        sChange     [8];	// ���
    char        sChgRate    [6];	//�����
    char        sVolume     [12];	//�ŷ���
    char        sVolRate    [8];	//���Ϻ�
    */
} DRAW_OUT_LIST;
#define SZ_DRAW_OUT_LIST	sizeof(DRAW_OUT_LIST)