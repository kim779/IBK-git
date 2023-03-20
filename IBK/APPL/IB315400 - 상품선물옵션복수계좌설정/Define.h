// Define.h : header file
//

// Control Resource ID
#define IDC_ACCTITLE			1001
#define IDC_GROUPTITLE			1002
#define IDC_SELTITLE			1003
#define IDC_ACCGRID				1004
#define IDC_LTOR				1005
#define IDC_GROUPGRID			1006
#define IDC_SELGRID				1007
#define IDC_IDSTATIC			1008
#define IDC_ID					1009
#define IDC_NAMESTATIC			1010
#define IDC_NAME				1011
#define IDC_ADDMODIFY			1012
#define IDC_GROUPDELETE			1013
#define IDC_GROUPUP				1014
#define IDC_GROUPDOWN			1015
#define IDC_ACCSTATIC			1016
#define IDC_ACCOUNT				1017
#define IDC_RATESTATIC			1018
#define IDC_RATE				1019
#define IDC_RATESPIN			1020
#define IDC_SELMODIFY			1021
#define IDC_SELDELETE			1022
#define IDC_SELUP				1023
#define IDC_SELDOWN				1024

// Size Define
#define SZ_MAP_WIDTH			642
#define	SZ_MAP_HEIGHT			450
#define	SZ_FORM_MARGIN			6
#define SZ_CTRL_ACCWIDTH		250
#define SZ_CTRL_RIGHTWIDTH		310
#define SZ_CTRL_GROUPHEIGHT		90
#define SZ_CTRL_SELHEIGHT		210
#define SZ_CTRL_VGAP			2
#define SZ_CTRL_HGAP			5
#define SZ_CTRL_HEIGHT			20
#define SZ_CTRL_1BUTTON			20
#define SZ_CTRL_4BUTTON			60
#define SZ_CTRL_1STATIC			24
#define SZ_CTRL_2STATIC			36
#define SZ_CTRL_3STATIC			48
#define SZ_CTRL_2EDIT			28
#define SZ_CTRL_4EDIT			52
#define SZ_CTRL_5EDIT			64
#define SZ_CTRL_7EDIT			88
#define SZ_CTRL_2BUTTON			36
#define SZ_CTRL_3BUTTON			48
#define SZ_CTRL_5BUTTON			72
#define SZ_CTRL_7BUTTON			96
#define SZ_CTRL_8STATIC			92
#define SZ_COL_ACCOUNT			116
#define SZ_COL_ACCNAME			116
#define SZ_COL_CODE				64
#define	SZ_ROWHEIGHT			18
#define SZ_COL_GROUPID			48
#define SZ_COL_GROUPNAME		245
#define SZ_COL_TOTRATE			64

// Color Index Define
#define CLR_FORM_BACK			64
#define CLR_GUIDE_BACK			66
#define CLR_GUIDE_FORE			69
#define CLR_GUIDE_LINE			75
#define	CLR_STATIC_FORE			69
#define CLR_GRID_TEXT			69
#define CLR_GRID_BACK			68
#define CLR_GRID_LINE			75
#define CLR_GRID_HBACK			74
#define CLR_GRID_HTEXT			76
#define CLR_GRID_SBACK			78
#define CLR_GRID_STEXT			69
#define CLR_PANEL_BORD			165

// TR Key
#define KEY_GROUPLIST			100
#define KEY_GROUPAMD			101
#define KEY_GROUPACCOUNT		102


/*******************************************************************************
 * �����ɼ� �ݵ���
 ******************************************************************************/

struct fofamd_rec
{
	char zAcntNo[20];			// ���¹�ȣ
	char zFnoIsuNo[32];			// �����ɼ������ȣ
};

struct fofamd_mid
{
	char In[5];					// ���ڵ尹��
	char zTrxPtnTp[1];			// ó����������
	char zUserTp[1];			// ����ڱ���
	char zFundOrdUserId[16];	// �ݵ��ֹ������ID
	char zEmpPwd[8];			// �����й�ȣ
	char zFundId[12];			// �ݵ�ID
	char zFundPtnTp[1];			// �ݵ���������
	char zAcntNo[20];			// ���¹�ȣ
	char zPwd[8];				// ��й�ȣ
	char zFnoIsuNo[32];			// �����ɼ������ȣ
	char lOrdAbleQty[16];		// �ֹ����ɼ���
	char dRatVal[19];			// ������
	char dWghtval[19];			// ����ġ
	char zPrcPtnTp[1];			// ������������
	char zOrdMthdTp[1];			// �ֹ���ı���
	char lOrdQty[16];			// �ֹ�����
	char dOrdPrc[13];			// �ֹ���
	char zBnsTp[1];				// �Ÿű���
	char zFundDescr40[40];		// �ݵ弳��40
	char In2[5];				// ���ڵ尹��
	struct fofamd_rec rec[30];	//
};

struct fofamd_mod
{
	char Out[5];				// ���ڵ尹��
	char zRegDt[8];				// �����
	char zRegTime[9];			// ��Ͻð�
};


/*******************************************************************************
 * �����ɼ� �ݵ���ȸ
 ******************************************************************************/


struct fofqry_mid
{
	char In[5];					// ���ڵ尹��
	char zUserTp[1];			// ����ڱ���
	char zFundOrdUserId[16];	// �ݵ��ֹ������ID
	char zEmpPwd[8];			// �����й�ȣ
};

struct fofqry_rec
{
	char zFundId[12];			// �ݵ�ID
	char zFundPtnTp[1];			// �ݵ���������
	char zFundDescr40[40];		// �ݵ弳��40
};

struct fofqry_mod
{
	char Out[5];					//	���ڵ尹��
	struct fofqry_rec rec[999];
};

#define L_fofqry_mid	sizeof(fofqry_mid)
#define L_fofqry_rec	sizeof(fofqry_rec)
#define L_fofqry_mod	sizeof(fofqry_mod)


/*******************************************************************************
 * �����ɼ� �ݵ庰������ȸ
 ******************************************************************************/

struct fofaqry_mid
{
	char In[5];					// ���ڵ尹��
	char zUserTp[1];			// ����ڱ���
	char zFundOrdUserId[16];	// �ݵ��ֹ������ID
	char zEmpPwd[8];			// �����й�ȣ
	char zFundId[12];			// �ݵ�ID
	char zFundPtnTp[1];			// �ݵ���������
};

struct fofaqry_rec
{
	char zAcntNo[20];			// ���¹�ȣ
	char zFnoIsuNo[32];			// �����ɼ������ȣ
	char lLastFundOrdNo[10];	// �����ݵ��ֹ���ȣ
	char zLastOrdDt[8];		// �����ֹ���
	char zRegDt[8];				// �����
	char zRegTime[9];			// ��Ͻð�
	char lOrdAbleQty[16];		// �ֹ����ɼ���
	char dRatVal[19];			// ������
	char dWghtval[19];			// ����ġ
	char zPrcPtnTp[1];			// ������������
	char zOrdMthdTp[1];			// �ֹ���ı���
	char lOrdQty[16];			// �ֹ�����
	char dOrdPrc[13];			// �ֹ���
	char zBnsTp[1];				// �Ÿű���
	char zIsuNm[40];			// �����
	char zAcntNm[40];			// ���¸�
	char zPrcPtnNm[40];			// ����������
	char zOrdMthdCodeNm[40];	// �ֹ�����ڵ��
	char zBnsTpNm[10];			// �Ÿű���
	char zFundDescr40[40];		// �ݵ弳��40
	char lFundRegSeq[10];		// �ݵ��ϼ���
};

struct fofaqry_mod
{
	char Out[5];				// ���ڵ尹��
	struct fofaqry_rec rec[999];
};

#define L_fofaqry_mid	sizeof(fofaqry_mid)
#define L_fofaqry_rec	sizeof(fofaqry_rec)
#define L_fofaqry_mod	sizeof(fofaqry_mod)


























/*******************************************************************************
 * �ݵ���/����/���� IO FORMAT	(�׷� ���/����/����)
 ******************************************************************************/

struct gamd_mid
{
	char In[5];					// ���ڵ尹��   - F12 : ����
	char zTrxTp[1];				// ó������     - 1 : �ݵ���ȸ, 2 : �ݵ��Է�, 3 : �ݵ�����, 4 : �ݵ����, 6 : �ݵ���ü����
	char zUserTp[1];			// ����ڱ���   - 1 : ����, 2 : ��
	char zUserId[16];			// �����ID
	char zEmpPwd[8];			// �����й�ȣ
	char zFundNm[40];			// �ݵ��
	char zFundPtnCode[2];		// �ݵ������ڵ� - 1 : ����:����(1:N), 2 : ����:����(N:1), 3 : ����:����(N:N)
	char zAcntNo[20];			// ���¹�ȣ
	char zPwd[8];				// ��й�ȣ
	char zIsuNo[12];			// �����ȣ
	char lOrdAbleQty[16];		// �ֹ����ɼ���
	char dRatVal[19];			// ������
	char dWghtval[19];			// ����ġ
	char zPrcPtnTp[1];			// ������������ - 1 : 1��ȣ��, 2 : 2��ȣ�� , 3 : 3��ȣ�� , 4 : 4��ȣ�� , 5 : 5��ȣ�� , C : ���簡  , H : ���Ѱ�  , L : ���Ѱ�  , Z : ������
	char zOrdMthdTp[1];			// �ֹ���ı��� - 1 : ����, 2 : ����, 3 : ����ġ
	char lOrdQty[16];			// �ֹ�����
	char dOrdPrc[13];			// �ֹ���
	char zBnsTp[1];				// �Ÿű���     - 1 : �ŵ�, 2 : �ż�
	char zFundDescr40[40];		// �ݵ弳��40
	char zMktTp[1];				// ���屸��
};

struct gamd_mod
{
	struct gamd_mid mid;		// �Էµ�����
	char Out[5];				// ���ڵ尹��
	char zRegDt[8];				// �����
	char zRegTime[9];			// ��Ͻð�
	char lOrdAbleQty[16];		// �ֹ����ɼ���
	char dRatVal[19];			// ������
	char dWghtval[19];			// ����ġ
	char zPrcPtnTp[1];			// ������������
	char zOrdMthdTp[1];			// �ֹ���ı���
	char lOrdQty[16];			// �ֹ�����
	char dOrdPrc[13];			// �ֹ���
	char zBnsTp[1];				// �Ÿű���
	char zPrcPtnNm[40];			// ����������
	char zFundPtnNm[40];		// �ݵ�������
	char zBnsTpNm[10];			// �Ÿű���
	char zFundDescr40[40];		// �ݵ弳��40
	char zMktTp[1];				// ���屸��
};

#define L_gamd_mid	sizeof(gamd_mid)
#define L_gamd_mod	sizeof(gamd_mod)


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