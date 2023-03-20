// Define.h : header file
//
#pragma once
// Control Resource ID
#define IDC_TAB				1001
#define IDC_GUIDE			1002
#define IDC_BUY				1003
#define IDC_SELL			1004
#define IDC_MODIFY			1005
#define IDC_CANCEL			1006
#define IDC_EXPAND			1007
#define IDC_EXPANDBTN		1008
#define IDC_GROUPSTATIC		1009
#define IDC_GROUP			1010
#define IDC_ADDACCOUNT		1011
#define IDC_CLEAR			1012
#define IDC_REMAIN			1013
#define IDC_ORDER			1014
#define IDC_CONTRACT		1015
#define IDC_CODECTRL		1016
#define IDC_CODESEARCH		1017
#define IDC_TYPESTATIC		1018
#define IDC_TYPE			1019
#define IDC_CONDITION		1020
#define IDC_QUANTITYSTATIC	1021
#define IDC_QUANTITY		1022
#define IDC_COSTSTATIC		1023
#define IDC_COSTTYPE		1024
#define IDC_COST			1025
#define IDC_SELLGRID		1026
#define IDC_MAX				1027
#define IDC_BUYGRID			1028
#define IDC_CANCELGRID		1029
#define IDC_MODIFYGRID		1030
#define IDC_REMAINQTY		1031
#define IDC_PARTQTY			1032
#define IDC_NONCONTRACT		1033
#define IDC_CONTRACTGRID	1034
#define IDC_ORDERTYPE		1035
#define IDC_ORDERTYPESTATIC	1036
#define IDC_WARNING			1037
#define IDC_PASSSET			1038
#define IDC_QTYTYPESTATIC	1036
#define IDC_GROUPID			1037
#define IDC_ABLEQTY			1039

// Size Define
#define SZ_MAP_WIDTH		994
#define	SZ_MAP_HEIGHT		523
#define	SZ_FORM_MARGIN		3
#define SZ_TAB_HMARGIN		2
#define SZ_CTRL_VGAP		2
#define SZ_CTRL_HGAP		5
#define SZ_CTRL_HEIGHT		20

#define SZ_TAB_HEIGHT		28
#define	SZ_CTRL_ACCN		140
#define SZ_CTRL_ACCNM		64
#define SZ_CTRL_1STATIC		14
#define SZ_CTRL_2STATIC		26
#define SZ_CTRL_1BUTTON		20
#define SZ_CTRL_2BUTTON		32
#define SZ_CTRL_3BUTTON		44
#define SZ_CTRL_4BUTTON		56
#define SZ_CTRL_8BUTTON		104
#define SZ_CTRL_4STATIC		50
#define SZ_CTRL_2EDIT		32
#define SZ_CTRL_3EDIT		44
#define SZ_CTRL_4EDIT		56
#define SZ_CTRL_6EDIT		80
#define SZ_CTRL_4BUTTON		56
#define SZ_CTRL_6STATIC		74
#define SZ_CHECK_5WIDTH		80
#define SZ_EXPAND_WIDTH		320
#define SZ_RADIO_1WIDTH		32
#define SZ_RADIO_2WIDTH		44
#define SZ_RADIO_3WIDTH		56
#define SZ_COMBO_2WIDTH		48
#define SZ_COMBO_3WIDTH		60
#define SZ_COMBO_4WIDTH		72
#define SZ_COMBO_5WIDTH		84
#define SZ_COMBO_6WIDTH		96
#define SZ_COMBO_7WIDTH		108
#define SZ_ROWHEIGHT		18
#define SZ_SYMBOL			110
#define SZ_CONGRID_WIDTH	135


// Color Index Define
#define CLR_FORM_BACK		64
#define CLR_PANNEL_BACK		66
#define CLR_PANNEL_LINE		67
#define	CLR_STATIC_BACK		78
#define	CLR_STATIC_FORE		69
#define	CLR_STATIC_LINE		65
#define CLR_TAB_FORE		70
#define CLR_TAB_SELFORE		71
#define CLR_GRID_TEXT		69
#define CLR_GRID_BACK		68
#define CLR_GRID_BACK2		77
#define CLR_GRID_LINE		75
#define CLR_GRID_HBACK		74
#define CLR_GRID_HTEXT		76
#define CLR_GRID_SBACK		78
#define CLR_GRID_STEXT		69
#define CLR_DATA_UP			94
#define CLR_DATA_DOWN		95
#define CLR_SYMBOL_BACK		160
#define CLR_SYMBOL_FORE		69
#define CLR_SYMBOL_LINE		89
#define CLR_GUIDE_BACK		34
#define CLR_GUIDE_FORE		130
#define CLR_GUIDE_LINE		75


// etc
#define MAPNO_SHOGA			"IB100610"
#define MAX_SELLGRIDCOL		11
#define DEF_SELLGRIDROW		20
#define MAX_BUYGRIDCOL		11
#define DEF_BUYGRIDROW		20
#define MAX_CANCELGRIDCOL		10
#define DEF_CANCELGRIDROW		20
#define MAX_MODIFYGRIDCOL		11
#define DEF_MODIFYGRIDROW		20
#define MAX_CONTRACTGRIDCOL		4
#define DEF_CONTRACTGRIDROW		20
#define TT_GUIDE			100

// TR KEY
#define KEY_SYMBOL				101
#define KEY_REMAIN				102
#define KEY_GROUPLIST			103
#define KEY_SISE				104
#define KEY_MODIFYNCDETAIL		105
#define KEY_CANCELNCREMAIN		106
#define KEY_SELLORDER			107
#define KEY_GROUPACCOUNT		108
#define KEY_BUYORDER			109
#define KEY_MODIFYNONCONTRACT	110
#define KEY_MODIFYORDER			111
#define KEY_CANCELNONCONTRACT	112
#define KEY_CANCELNCDETAIL		113
#define KEY_CANCELORDER			114
#define KEY_BUYABLEQTY			115

#define UG_CONT_CELLCHANGE	WM_USER + 100

struct i_frem
{
	char In[5];				// ���ڵ尹��  
	char zUserTp[1];		// ����ڱ���  
	char zUserId[16];		// �����ID    
	char zFundNm[40];		// �ݵ��      
	char zFundKindCode[2];	// �ݵ������ڵ�
	char zIsuNo[12];		// �����ȣ    
};

struct o_frec
{
	char zAcntNo[20];		// ���¹�ȣ    
	char zAcntNm[40];		// ���¸�      
	char zIsuNo[12];		// �����ȣ    
	char zIsuNm[40];		// �����      
	char lOrdAbleQty[16];	// �ֹ����ɼ���
	char lBalQty[16];		// �ܰ����    
	char lUnercQty[16];		// ��ü�����  
};

struct o_frem
{
	char Out1[5];			// ���ڵ尹��  
	struct o_frec rec[100];
};

#define L_i_frem sizeof(i_frem)
#define L_o_frec sizeof(o_frec)
#define L_o_frem sizeof(o_frem)


/*******************************************************************************
 * �ݵ���ȸ IO FORMAT	(�׷츮��Ʈ ��ȸ)
 ******************************************************************************/

struct gqry_mid
{
	char In[5];					// ���ڵ尹��   - F12 : ����, F7 : ����
	char zUserTp[1];			// ����ڱ���	- 1 : ����
	char zCreUserId[16];		// ���������ID
	char zAcntPwd[8];			// ��й�ȣ
};

struct gqry_rec
{
	char zFundNm[40];			// �ݵ��
	char zFundKindTp[1];		// �ݵ���������
	char zCampDescr[100];		// ķ���μ���
};

struct gqry_mod
{
	struct gqry_mid mid;		// �Էµ�����
	char Out[5];				// ���ڵ尹��
	struct gqry_rec rec[50];
};

#define L_gqry_mid	sizeof(gqry_mid)
#define L_gqry_mod	sizeof(gqry_mod)
#define L_gqry_rec	sizeof(gqry_rec)


/*******************************************************************************
 * �ݵ�������ȸ IO FORMAT	(�������� ��ȸ)
 ******************************************************************************/

struct aqry_mid
{
	char In[5];					// ���ڵ尹��  	- F12 : ����, F7 : ���� 
	char zUserTp[1];			// ����ڱ���   - 1 : ����, 2 : ��  
	char zCreUserId[16];		// ���������ID
	char zPwd[8];				// ��й�ȣ    
	char zFundNm[40];			// �ݵ��      
	char zFundKindTp[1];		// �ݵ���������	- 1 : 1 vs N, 2 : N vs 1, 3 : N vs N
};

struct aqry_rec
{
	char zAcntNo[20];			// ���¹�ȣ        
	char zIsuNo[12];			// �����ȣ        
	char lLastFundOrdNo[10];	// �����ݵ��ֹ���ȣ
	char zLastFundOrdDt[8];		// �����ݵ��ֹ���  
	char zRegDate[8];			// �������        
	char zRegTime[9];			// ��Ͻð�        
	char lOrdAbleQty[16];		// �ֹ����ɼ���    
	char dRatVal[19];			// ������          
	char dWghtval[19];			// ����ġ          
	char zPrcPtnTp[1];			// ������������    
	char zOrdprcPtnCode[2];		// ȣ�������ڵ�    
	char zOrdMthdTp[1];			// �ֹ���ı���    
	char lOrdQty[16];			// �ֹ�����        
	char dOrdPrc[13];			// �ֹ���          
	char zBnsTp[1];				// �Ÿű���        
	char zIsuNm[40];			// �����          
	char zAcntNm[40];			// ���¸�          
	char zPrcPtnNm[40];			// ����������      
	char zOrdMthdNm[20];		// �ֹ���ĸ�      
	char zBnsTpNm[10];			// �Ÿű���        
	char zFundDescr40[40];		// �ݵ弳��40      
	char zMkTp[1];				// ���屸��        
	char lMtiordSeqno[10];		// �����ֹ��Ϸù�ȣ
};

struct aqry_mod
{	
	struct aqry_mid mid;		// �Էµ�����
	char ST_SONAQ039[5];		// ���ڵ尹��
	struct aqry_rec rec[30];
};

#define L_aqry_mid	sizeof(aqry_mid)
#define L_aqry_rec	sizeof(aqry_rec)
#define L_aqry_mod	sizeof(aqry_mod)


/*******************************************************************************
* �ݵ��ֹ�����ȸ
*******************************************************************************/
struct goqry_mid
{
	char In[5];				// ���ڵ尹��
	char zUserTp[1];		// ����ڱ���
	char zCreUserId[16];	// ���������ID
	char zFundNm[40];		// �ݵ��
	char zFundKindTp[1];	// �ݵ���������
	char lFundOrdNo[10];	// �ݵ��ֹ���ȣ
	char zOrdDt[8];			// �ֹ���
};

struct goqry_rec
{
	char zMgmtBrnNo[3];		// ����������ȣ
	char zAcntNo[20];		// ���¹�ȣ
	char zAcntNm[40];		// ���¸�
	char zIsuNo[12];		// �����ȣ
	char zIsuNm[40];		// �����
	char lOrdNo[10];		// �ֹ���ȣ
	char zBnsTp[1];			// �Ÿű���
	char zBnsTpNm[10];		// �Ÿű���
	char zMrcTp[1];			// ������ұ���
	char zMrcTpNm[10];		// ������ұ��и�
	char lMrcQty[16];		// ������Ҽ���
	char lOrdQty[16];		// �ֹ�����
	char lExecQty[16];		// ü�����
	char dOrdPrc[13];		// �ֹ���
	char dAvrExecPrc[13];	// ���ü�ᰡ
	char lExecAmt[16];		// ü��ݾ�
	char zOrdTime[9];		// �ֹ��ð�
	char lOrdTrxPtnCode[9];	// �ֹ�ó�������ڵ�
};

struct goqry_mod
{
	char Out[5];			// ���ڵ尹��
	struct goqry_rec rec[30];
};

#define L_goqry_mid	sizeof(goqry_mid)
#define L_goqry_mod	sizeof(goqry_mod)
#define L_goqry_rec	sizeof(goqry_rec)

/*******************************************************************************
* �����ݵ��ֹ�
*******************************************************************************/
struct godr_mid
{
	char In[5];				// ���ڵ尹��
	char zUserTp[1];		// ����ڱ���
	char zUserId[16];		// �����ID
	char zFundNm[40];		// �ݵ��
	char zIvstfndPtnTp[1];	// �����ݵ���������
	char lSpotOrdCnt[6];	// �����ֹ�Ƚ��
	char zAcntNo[20];		// ���¹�ȣ
	char zIsuNo[12];		// �����ȣ
	char lOrdQty[16];		// �ֹ�����
	char dOrdPrc[13];		// �ֹ���
	char zBnsTp[1];			// �Ÿű���
	char zOrdprcPtnCode[2];	// ȣ�������ڵ�
	char zOrdCndiTp[1];		// �ֹ����Ǳ���
	char zCommdaCode[2];	// ��Ÿ�ü�ڵ�
	char zOpDrtnNo[12];		// ������ù�ȣ
	char zGrpPtnTp[1];		// �׷���������
};

struct godr_mod
{
	char Out[5];			// ���ڵ尹��
	char lFundOrdNo[10];	// �ݵ��ֹ���ȣ
	char lOrdNo[10];		// �ֹ���ȣ
};

#define L_godr_mid	sizeof(godr_mid)
#define L_godr_mod	sizeof(godr_mod)


/*******************************************************************************
* �����ݵ������ֹ�
*******************************************************************************/
struct gmod_mid
{
	char In[5];				// ���ڵ尹��
	char zUserTp[1];		// ����ڱ���
	char zUserId[16];		// �����ID
	char zFundNm[40];		// �ݵ��
	char zIvstfndPtnTp[1];	// �����ݵ���������
	char lFundOrgOrdNo[10];	// �ݵ���ֹ���ȣ
	char lSpotOrdCnt[6];	// �����ֹ�Ƚ��
	char lOrgOrdNo[10];		// ���ֹ���ȣ
	char zAcntNo[20];		// ���¹�ȣ
	char zIsuNo[12];		// �����ȣ
	char lOrdQty[16];		// �ֹ�����
	char zOrdprcPtnCode[2];	// ȣ�������ڵ�
	char zOrdCndiTp[1];		// �ֹ����Ǳ���
	char dOrdPrc[13];		// �ֹ���
	char zCommdaCode[2];	// ��Ÿ�ü�ڵ�
	char zGrpPtnTp[1];		// �׷���������
};

struct gmod_mod
{
	char Out[5];			// ���ڵ尹��
	char lFundOrdNo[10];	// �ݵ��ֹ���ȣ
	char lOrdNo[10];		// �ֹ���ȣ
};

#define L_gmod_mid	sizeof(gmod_mid)
#define L_gmod_mod	sizeof(gmod_mod)


/*******************************************************************************
* �����ݵ�����ֹ�
*******************************************************************************/
struct gcan_mid
{
	char In[5];				// ���ڵ尹��
	char zUserTp[1];		// ����ڱ���
	char zUserId[16];		// �����ID
	char zFundNm[40];		// �ݵ��
	char zIvstfndPtnTp[1];	// �����ݵ���������
	char lFundOrgOrdNo[10];	// �ݵ���ֹ���ȣ
	char lSpotOrdCnt[6];	// �����ֹ�Ƚ��
	char lOrgOrdNo[10];		// ���ֹ���ȣ
	char zAcntNo[20];		// ���¹�ȣ
	char zIsuNo[12];		// �����ȣ
	char lOrdQty[16];		// �ֹ�����
	char zCommdaCode[2];	// ��Ÿ�ü�ڵ�
	char zGrpPtnTp[1];		// �׷���������
};

struct gcan_mod
{
	char Out[5];			// ���ڵ尹��
	char lFundOrdNo[10];	// �ݵ��ֹ���ȣ
	char lOrdNo[10];		// �ֹ���ȣ
};

#define L_gcan_mid	sizeof(gcan_mid)
#define L_gcan_mod	sizeof(gcan_mod)


/*******************************************************************************
* �ݵ��ֹ���ȣ���ֹ�ü����ȸ
*******************************************************************************/
struct gntn_mid
{
	char In[5];					// ���ڵ尹��
	char zUserTp[1];			// ����ڱ���
	char zCreUserId[16];		// ���������ID
	char zFundNm[40];			// �ݵ��
	char zFundKindTp[2];		// �ݵ���������
	char lFundOrdNo[10];		// �ݵ��ֹ���ȣ
	char zOrdDt[8];				// �ֹ���
};

struct gntn_rec
{
	char lFundOrdNo[10];		// �ݵ��ֹ���ȣ
	char lFundOrgOrdNo[10];		// �ݵ���ֹ���ȣ	
	char zMrcTp[1];				// ������ұ���
	char lExecQty[16];			// ��üü�����
	char lOrdQty[16];			// ��ü�ֹ�����		
	char zTrxTime[9];			// ó���ð�
	char lMrcAbleQty[16];		// ������Ұ��ɼ���	
};

struct gntn_mod
{
	char Out[5];				// ���ڵ尹��
	char zFundDescr100[100];	// �ݵ弳��100
	char recOut[5];				// ���ڵ尹��
	struct gntn_rec rec[70];		
};

#define L_gntn_mid	sizeof(gntn_mid)
#define L_gntn_rec	sizeof(gntn_rec)
#define L_gntn_mod	sizeof(gntn_mod)


/*******************************************************************************
* �ֹ����ɼ��� ��ȸ SONAQ200
*******************************************************************************/
struct sonaq200_mid
{
	char In[5];
	char zBnsTp[1];
	char zAcntNo[20];
	char zPwd[8];
	char zIsuNo[12];
	char dOrdPrc[13];
	char zCommdaCode[2];
	char zLoanPtnCode[2];
};

struct sonaq200_mod
{
	struct sonaq200_mid mid;
	char Out[5];
	char zAcntNm[40];                          /* ���¸�                          */
	char zIsuNm[40];                           /* �����                          */
	char zBnsTpNm[10];                         /* �Ÿű���                        */
	char lDps[16];                             /* ������                          */
	char lSubstAmt[16];                        /* ���ݾ�                        */
	char lMnyMgn[16];                          /* �������űݾ�                    */
	char lSubstMgn[16];                        /* ������űݾ�                    */
	char lRcvblSumAmt[16];                     /* �̼��հ�ݾ�                    */
	char lRuseOrdAbleAmt[16];                  /* �����ֹ����ɱݾ�              */
	char lMnyRuseObjAmt[16];                   /* ����������ݾ�              */
	char lSubstRuseObjAmt[16];                 /* ���������ݾ�              */
	char lPrdayRuseOrdAbleAmt[16];             /* ���������ֹ����ɱݾ�          */
	char lCrdayRuseOrdAbleAmt[16];             /* ���������ֹ����ɱݾ�          */
	char lMnyOrdAbleAmt[16];                   /* �����ֹ����ɱݾ�                */
	char lSubstOrdAbleAmt[16];                 /* ����ֹ����ɱݾ�                */
	char lCrdtPldgRuseAmt[16];                 /* �ſ�㺸����ݾ�              */
	char lSeOrdAbleAmt[16];                    /* �ŷ��ұݾ�                      */
	char lKdqOrdAbleAmt[16];                   /* �ڽ��ڱݾ�                      */
	char lAbleAmt[16];                         /* ���ɱݾ�                        */
	char lMgnRat100pctOrdAbleAmt[16];          /* ���űݷ�100�ۼ�Ʈ�ֹ����ɱݾ�   */
	char lMgnRat100OrdAbleQty[16];             /* ���űݷ�100�ۼ�Ʈ�ֹ����ɼ���   */
	char dLoanPldgRat[9];                      /* ����㺸��                      */
	char dPldgMaintRat[7];                     /* �㺸��������                    */
	char lOrdAbleQty[16];                      /* �ֹ����ɼ���                    */
	char lUnercBuyOrdAmt[16];                  /* ��ü��ż��ֹ��ݾ�              */
	char dCmsnAmtRat[11];                      /* ��������                        */
	char lAddCmsnAmtLevyAmt[16];               /* �߰�������¡���ݾ�              */
	char lBalQty[16];                          /* �ܰ����                        */
	char lHtsOrdAbleAmt[16];                   /* HTS�ֹ����ɱݾ�                 */
	char lD1Dps[16];                           /* D+1������                       */
	char lD2Dps[16];                           /* D2������                        */
	char lPrdayRuseOrdAbleQty[16];             /* ���������ֹ����ɼ���          */
	char lCrdayRuseOrdAbleQty[16];             /* ���������ֹ����ɼ���          */
	char lCrdtOrdAbleAmt[16];                  /* �ſ��ֹ����ɱݾ�                */
};
