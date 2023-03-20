#if !defined(AFX_SAMSUNG_ORDERDEF_H_73649_BY_98B1A__INCLUDED_)
#define AFX_SAMSUNG_ORDERDEF_H_73649_BY_98B1A__INCLUDED_

// #include "../src/SSSTOrder/SS_OrderDef.h"
// �� ������ �Ｚ�� �ֹ����� ��Ŷ�� ������ ������.
// �ٸ� ����Ʈ���� ��������.
#define		STORDER_TITLE_0		"�ý���Ʈ���̵� �ż�����"
#define		STORDER_TITLE_1		"�ý���Ʈ���̵� �ż�û��"
#define		STORDER_TITLE_2		"�ý���Ʈ���̵� �ŵ�û��"
#define		STORDER_TITLE_3		"�ý���Ʈ���̵� �ŵ�����"
#define		STORDER_TITLE_4		"�ý���Ʈ���̵� ����û��"

////////////////////////////////////////////////////////////////////////////////////////
// Input structure                                                                   
//*ǥ�� ������ �ǹ�
////////////////////////////////////////////////////////////////////////////////////////

// -------------------------------------------------------------------------------------
//			��������(HFIBOA0F2000)
// -------------------------------------------------------------------------------------
// HFIBOA0F2000InRec: �����ɼ������ֹ�                                               
/*
typedef struct
{
    char    InItem1[1];                           // #1(1)              1            
    char    InItem2[1];                           // �ŷ�����(1)        1            
    char    InItem3[10];                          // ���¹�ȣ(10)       5001448503  
    char    InItem4[8];                           // ��й�ȣ(8)        aa1122**     
    char    InItem5[8];                           // �����ڵ�(8)        101A9***     
    char    InItem6[1];                           // �ֹ�����(1)        1    '1'������, '2'���尡
    char    InItem7[1];                           // #3(1)              3            
    char    InItem8[9];                           // �ֹ�����(9)        000000001    
    char    InItem9[13];                          // �ֹ�����(13)       173.00*******
    char    InItem10[2];                          // #10(2)             10           
}fn_HFIBOA0F2000_InRec1;


////////////////////////////////////////////////////////////////////////////////////////
// Output structure                                                                  
////////////////////////////////////////////////////////////////////////////////////////

// HFIBOA0F2000OutRec1: �����ɼ������ֹ�                                             
typedef struct
{
    char    OutItem1[1];                          // ó������OUT(1)        1         
    char    OutItem2[1];                          // �Ÿű���OUT(1)        2         
    char    OutItem3[10];                         // ���¹�ȣOUT(10)       5001448503          
    char    OutItem4[8];                          // ���º�й�ȣ(8)       
    char    OutItem5[8];                          // �����ڵ�OUT(8)        101A9               
    char    OutItem6[1];                          // �ֹ�����OUT(1)        1         
    char    OutItem7[1];                          // �ŷ�����OUT_F200(1)   3         
    char    OutItem8[9];                          // �ֹ�����OUT(9)        000000001              
    char    OutItem9[13];                         // �ֹ�����OUT(13)       173.00          
    char    OutItem10[2];                         // ��ű���OUT(2)        10        
    char    OutItem11[7];                         // �ֹ���ȣ(7)           0000001   
    char    OutItem12[20];                        // ���¸�(20)            �׽�Ʈ��    
}fn_HFIBOA0F2000_OutRec1;
*/

// -------------------------------------------------------------------------------------
//			���� ������ �����ֹ�(HFIJOA0F1100)
// -------------------------------------------------------------------------------------
/*
typedef struct
{
	char	InItem1[11];							// ���¹�ȣ
    char    InItem2[ 8];							// ��й�ȣ
    char    InItem3[ 1];							// �ֹ�����
    char    InItem4[12];							// �����ȣ
    char    InItem5[ 7];							// �Ÿż���
    char    InItem6[ 6];							// �ŸŴܰ�
    char    InItem7[ 1];							// �Ÿű���
	char    InItem8[ 2];							// ����ֹ����� ?
}fn_HFIJOAOF1100_InRec1;

typedef struct
{
    char    OutItem1[11];							// ���¹�ȣOUT
    char    OutItem2[10];							// ������OUT
	char    OutItem3[20];							// ���¸�OUT
	char    OutItem4[12];							// �����ȣOUT
	char    OutItem5[20];							// �����OUT
	char    OutItem6[ 7];							// �Ÿż���OUT
	char    OutItem7[ 6];							// �ŸŰ���OUT
	char    OutItem8[ 5];							// �ֹ���ȣOUT
	char    OutItem9[ 1];							// ����ֹ�����OUT
	char    OutItem10[ 8];							// ����ֹ����и�OUT
	char    OutItem11[ 1];							// �Ÿű���OUT
	char    OutItem12[12];							// �Ÿű��и�OUT
}fn_HFIJOAOF1100_OutRec1;
*/

// ������ �����ɼ������ֹ� Input structure
// #3001 ȭ������.
/*
typedef struct
{
    char    InItem1[1];                           // ó������(1)  
    char    InItem2[1];                           // �ŷ�����(1)  
    char    InItem3[10];                          // ���¹�ȣ(10) 
    char    InItem4[8];                           // ��й�ȣ(8)  
    char    InItem5[8];                           // �����ڵ�(8)  
    char    InItem6[1];                           // �ֹ�����(1)  
    char    InItem7[1];                           // #3(1)        
    char    InItem8[9];                           // �ֹ�����(9)  
    char    InItem9[13];                          // �ֹ�����(13) 
    char    InItem10[2];                          // #00(2)       
}fn_HFIJOA0F0011_InRec1;

// ������ �����ɼ������ֹ� Output structure
typedef struct
{
    char    OutItem1[7];                          // �ֹ���ȣ(7)
    char    OutItem2[20];                         // ���¸�(20) 
}fn_HFIJOA0F0011_OutRec1;

// -------------------------------------------------------------------------------------
//			�ֽİ��� ���ݸż�
// -------------------------------------------------------------------------------------
typedef struct
{
    char    InItem1[10];							// ���¹�ȣ(10)
    char    InItem2[ 8];							// ��й�ȣ(08)
    char    InItem3[12];							// �����ڵ�(12) : ù�ڸ��� 'A'
    char    InItem4[ 9];							// �Ÿż���(09)
    char    InItem5[ 9];							// �ŸŴܰ�(09)
    char    InItem6[ 2];							// �Ÿű���(02)
    char    InItem7[ 2];							// �ֹ�����(02)
    char    InItem8[15];							// ��ȭ��ȣ(15)
    char    InItem9[ 2];							// ����ֹ�����(02)
}fn_HFIBOA0Y3200_InRec1;

typedef struct
{
    char    OutItem1 [ 8];							// ī���ȣ(08)
    char    OutItem2 [ 2];							// ���±���(02)
    char    OutItem3 [20];							// ���¸�(20)
    char    OutItem4 [12];							// �����ȣ(12)
    char    OutItem5 [20];							// �����_��(20)
    char    OutItem6 [ 9];							// �Ÿż���2(09)
    char    OutItem7 [ 9];							// �ŸŰ���2(09)
    char    OutItem8 [ 5];							// �ֹ���ȣ2(15)
    char    OutItem9 [ 2];							// �Ÿű���2(02)
	char    OutItem10[20];							// �Ÿű��и�(20)
	char    OutItem11[ 2];							// �ֹ�����OUT(02)
	char    OutItem12[ 2];							// ����ֹ�����2(2)
	char    OutItem13[10];							// ����ֹ����и�(10)
}fn_HFIBOA0Y3200_OutRec1;

#define		fn_HFIBOA0Y3100_InRec1			fn_HFIBOA0Y3200_InRec1
#define		fn_HFIBOA0Y3100_OutRec1			fn_HFIBOA0Y3200_OutRec1

// -------------------------------------------------------------------------------------
//			������ ���ݸż�
//			hfij20620 ȭ������. �ֽ�ȣ���ֹ�
// -------------------------------------------------------------------------------------
typedef struct
{
    char    InItem1[ 1];							// ���μ���Ÿ��, !2
    char    InItem2[10];							// ���¹�ȣ(10)
    char    InItem3[ 8];							// ��й�ȣ(08)
    char    InItem4[12];							// �����ڵ�(12) : ù�ڸ��� 'A'
    char    InItem5[ 9];							// �Ÿż���(09)
    char    InItem6[ 9];							// �ŸŴܰ�(09)
    char    InItem7[ 2];							// �Ÿű���(02)
    char    InItem8[ 2];							// �ֹ�����(02)
    char    InItem9[15];							// ��ȭ��ȣ(15)
	char    InItem10[2];							// ����ֹ�����(02), !00
}fn_HFIJOA0T1131_InRec1;

typedef struct
{
    char    OutItem1 [ 5];							// �ֹ���ȣ(�ż�)
    char    OutItem2 [13];							// ��������ݾ�
    char    OutItem3 [13];							// ��������ݾ�
    char    OutItem4 [13];							// ��������(�ż�)
    char    OutItem5 [13];							// ���Ŵ��(�ż�)
    char    OutItem6 [13];							// ����㺸�ݾ�
    char    OutItem7 [13];							// �������밡�ɱݾ�
    char    OutItem8 [13];							// �������밡�ɱݾ�
    char    OutItem9 [13];							// ���ݾ�
	char    OutItem10[13];							// �������űݾ�(�ż�)
	char    OutItem11[13];							// ���ϴ㺸(�ż�)
	char    OutItem12[20];							// ���¸�(�ż�)
}fn_HFIJOA0T1131_OutRec1;

// -------------------------------------------------------------------------------------
//			������ ���ݸż�(HFIJOA0Y6100)
// -------------------------------------------------------------------------------------
// ���ݸż� In

typedef struct
{
	char    InItem1[ 1];							// #1(1)
    char    InItem2[10];							// ī���ȣ(10)
    char    InItem3[ 8];							// ��й�ȣ(08)
    char    InItem4[ 1];							// #A(1)
    char    InItem5[ 5];							// �����ڵ�(05)
    char    InItem6[ 6];							// #(06)
	char    InItem7[ 9];							// �Ÿż���(9)
	char    InItem8[ 9];							// �ŸŴܰ�(9)
    char    InItem9[ 2];							// �Ÿű���(02)
	char    InItem10[15];							// ��ȭ��ȣ(15)
	char    InItem11[ 2];							// #00(2)
}fn_HFIJOA0Y6100_InRec1;

// ���ݸż� Out
typedef struct
{
    char    OutItem1 [ 3];							// #3
    char    OutItem2 [ 2];							// �ֹ���ȣ2
    char    OutItem3 [138];							// #138
    char    OutItem4 [20];							// �����
    char    OutItem5 [20];							// ������
    char    OutItem6 [ 1];							// #1
    char    OutItem7 [ 8];							// ī���ȣ2
    char    OutItem8 [ 2];							// ���±���2
    char    OutItem9 [ 8];							// #8
	char    OutItem10[12];							// �����ȣ
	char    OutItem11[ 9];							// �Ÿż���2
	char    OutItem12[ 9];							// �ŸŰ���2
	char    OutItem13[ 2];							// �Ÿű���2
	char    OutItem14[15];							// #15
	char    OutItem15[ 2];							// ����ֹ�����2
}fn_HFIJOA0Y6100_OutRec1;
*/

// -------------------------------------------------------------------------------------
//			���� ���ݸż�(HFIJOA0Y6100)
// -------------------------------------------------------------------------------------
// ���ݸż� In
/*
typedef struct
{
	char    InItem1[ 1];							// #1(1)
    char    InItem2[10];							// ī���ȣ(10)
    char    InItem3[ 8];							// ��й�ȣ(08)
    char    InItem4[ 1];							// #A(1)
    char    InItem5[ 5];							// �����ڵ�(05)
    char    InItem6[ 6];							// #(06)
	char    InItem7[ 9];							// �Ÿż���(9)
	char    InItem8[ 9];							// �ŸŴܰ�(9)
    char    InItem9[ 2];							// �Ÿű���(02)
	char    InItem10[15];							// ��ȭ��ȣ(15)
	char    InItem11[ 2];							// #00(2)
}fn_HFIJOA0Y6100_InRec1;

// ���ݸż� Out
typedef struct
{
    char    OutItem1 [ 3];							// #3
    char    OutItem2 [ 2];							// �ֹ���ȣ2
    char    OutItem3 [138];							// #138
    char    OutItem4 [20];							// �����
    char    OutItem5 [20];							// ������
    char    OutItem6 [ 1];							// #1
    char    OutItem7 [ 8];							// ī���ȣ2
    char    OutItem8 [ 2];							// ���±���2
    char    OutItem9 [ 8];							// #8
	char    OutItem10[12];							// �����ȣ
	char    OutItem11[ 9];							// �Ÿż���2
	char    OutItem12[ 9];							// �ŸŰ���2
	char    OutItem13[ 2];							// �Ÿű���2
	char    OutItem14[15];							// #15
	char    OutItem15[ 2];							// ����ֹ�����2
}fn_HFIJOA0Y6100_OutRec1;

// -------------------------------------------------------------------------------------
//			������ ���ݸŵ�(HFIJOA0T1111)
//			hfij80620�ֽĸż� ȭ������.
// -------------------------------------------------------------------------------------
typedef struct
{
    char    InItem1[ 1];							// ���μ���Ÿ��, !2
    char    InItem2[10];							// ���¹�ȣ(10)
    char    InItem3[ 8];							// ��й�ȣ(08)
    char    InItem4[12];							// �����ڵ�(12) : ù�ڸ��� 'A'
    char    InItem5[ 9];							// �Ÿż���(09)
    char    InItem6[ 9];							// �ŸŴܰ�(09)
    char    InItem7[ 2];							// �Ÿű���(02)
    char    InItem8[ 2];							// �ֹ�����(02)
    char    InItem9[15];							// ��ȭ��ȣ(15)
	char    InItem10[2];							// ����ֹ�����(02), !00
}fn_HFIJOA0T1111_InRec1;

typedef struct
{
    char    OutItem1 [ 5];							// �ֹ���ȣ(�ŵ�)
    char    OutItem2 [ 9];							// �����������(�ŵ�)
    char    OutItem3 [ 9];							// �����������(�ŵ�)
    char    OutItem4 [ 9];							// �ǹ��ֹ�����(�ŵ�)
    char    OutItem5 [ 9];							// �������밡�ɼ���(�ŵ�)
    char    OutItem6 [ 9];							// �������밡�ɼ���(�ŵ�)
    char    OutItem7 [ 9];							// �ܰ����(�ŵ�)
    char    OutItem8 [20];							// ���¸�(�ŵ�)
}fn_HFIJOA0T1111_OutRec1;

/*
#define COrderArray_FO CArray<COrder_FO, COrder_FO&> 
class COrder_FO
{
public:
	CString		clikey;		//   [ 8] �ֹ� key 
	char		gb;			//   [ 1] 1:�ŵ�, 2:�ż�, 3:����, 4:���
	CString		geja;		//   [11] ���¹�ȣ
	CString		passwd;		//   [ 8] ��й�ȣ
	CString		code;		//   [15] �����ڵ�
	CString		codeName;	//	 �����ڵ��
	CString		wonno;		//   [ 7] ���ֹ���ȣ
	char		jjgb;		//   [ 1] �Ϲ� �ֹ��� �����ֹ��� ��� : '1'�������� '2'�ֹ���������
							//	 [ 1] ���̽� �ϰ��ֹ� TR : 1:�Ϻ�����,2:��������,3:�Ϻ����,4:�������
	char		jutype;		//   [ 1] �ֹ����� 1:������ 2:���尡 3:���Ǻ����� 4:������������
	int			juprc;		//   [10] �ŸŰ���
	int			qty;		//   [ 7] �ֹ�����
	
	// [NOTE] : �ֹ�Ȯ��â
	CString		gejaname;	//   ���¸�

	// [NOTE] : ����/��� �ֹ� Ȯ��â ���� ���ֹ� ������ ä���.
	char		org_gb;		//   ���ֹ����� - [ 1] 1:�ŵ�, 2:�ż� (�ŵ�/�� ���и� ����Ѵ�.)
	char		org_jutype;	//   ���ֹ����� - [ 1] �ֹ����� 1:������ 2:���尡 3:���Ǻ����� 4:������������
	int			org_juprc;	//	 ���ֹ����� 

	int			nMITPrice;	//   mit �����ֹ����ݼ���

public:
	COrder_FO()
	{
		fnInitData();
	}

	void fnInitData()
	{
		clikey		= "";
		gb			= ' ';
		geja		= "";
		passwd		= "";
		code		= "";
		codeName	= "";
		wonno		= "";
		jjgb		= ' ';
		jutype		= ' ';
		juprc		= 0;
		qty			= 0;

		// [NOTE] : �ֹ�Ȯ��â
		gejaname	= "";

		// [NOTE] : �ֹ� Ȯ��â ���� ���ֹ� ������ ä���.
		org_gb		= ' ';
		org_jutype	= ' ';
		org_juprc	= 0;
		nMITPrice	= 0;
	}
};


class COrderSave
{
public:
	COrderSave() { bIsMITOrder = FALSE; }
public:
	int		nIndex;			// ��ǥ index
	int		nEventType;		// �ֹ��׼��� �߻��� type(0:ȣ����, 1:popup, 2:MIT-��������, 3:MIT-back 4:�����̴�, 5:��ü���, 6:��ü�Ḯ��Ʈ)
	int		nCount;			// �ֹ� �Ǽ�
	int		nRowID;			// ��ְ� ���� ��춧����
	int		nColID;			// ��ְ� ���� ��춧����, ����/��� ������
	BOOL	bIsMITOrder;	// MIT �ֹ� ����
	COrderArray_FO	m_arrOrdSave;
};
*/

class CSTOrderRData_SS
{
public:
	int			m_nMarket;				//0:�ֽ�, 1:����/�ɼ�, 2:�ֽļ���, 3:�ֽĿɼ�
	int			m_nOrderType;			//VALENTRYSELL �ż�����, VALEXITLONG �ż�û��, VALEXITSHORT �ŵ�û��, VALENTRYBUY �ŵ�����
	int			m_nRealOrderType;		//����Ÿ���� ���� �ֹ����� �� Ÿ�Ը�. �ֹ����� ���õ�
	CString		m_szGeja;				//[11] ���¹�ȣ
	CString		m_szGejaname;			//���¸�
	CString		m_szCode;				//[15] �����ڵ�
	CString		m_szCodeName;			//[12]�����ڵ��
	char		m_jutype[2];			//[02]�Ÿű��� �ֽ� : 00����, 03���尡
										//			   ���� : 
//	int			m_nJuprc;				//[09]�ŸŰ���
	float		m_fJuprc;				//[09]�ŸŰ���
	int			m_nQty;					//[09]�ֹ�����
	char		m_gb;					//[ 1] 1:�ż�, 2:�ŵ�, 3:����, 4:���
										// 1:�ŵ�, 2:�ż�
	CString		m_szPasswd;				//[08] ��й�ȣ
	int			m_nOrderPriceType;		//�ֹ�����Ÿ��. 
										//0���簡,1-3�ż�1~3,4-6�ŵ�1~3,7���尡,8���Ѱ�,9���Ѱ�
	int			m_nSTType;				//������ ����, ��������(0), ����� ����(1)
	int			m_nAutoOrder;			//�ֹ�����:0, ���ڵ�:1
	CString		m_szSTPath;				//������ ���

	long		m_lPtrCSTConfigEnv;		// CSTConfigEnv ����Ʈ

public:
	void Copy(CSTOrderRData_SS* pOrg)
	{
		m_nMarket		= pOrg->m_nMarket;
		m_nOrderType	= pOrg->m_nOrderType;
		m_nRealOrderType= pOrg->m_nRealOrderType;
		m_szGeja		= pOrg->m_szGeja;			
		m_szGejaname	= pOrg->m_szGejaname;		
		m_szCode		= pOrg->m_szCode;			
		m_szCodeName	= pOrg->m_szCodeName;		
		memcpy(m_jutype,  pOrg->m_jutype, 2);
		m_fJuprc		= pOrg->m_fJuprc;			
		m_nQty			= pOrg->m_nQty;			
		m_gb			= pOrg->m_gb;				
		m_szPasswd		= pOrg->m_szPasswd;			
		m_nOrderPriceType= pOrg->m_nOrderPriceType;
		m_nSTType		= pOrg->m_nSTType;		
		m_nAutoOrder	= pOrg->m_nAutoOrder;		
		m_szSTPath		= pOrg->m_szSTPath;	

		m_lPtrCSTConfigEnv = pOrg->m_lPtrCSTConfigEnv;
	}

	CSTOrderRData_SS()
	{
		m_lPtrCSTConfigEnv = NULL;
	}
};

class CSTOrderRData_EG : public CSTOrderRData_SS
{
public:
	int			m_nOrderMarketEntry;
	int			m_nOrderMarketClear;

	BOOL		m_bSound_Buy_Use;
	CString		m_szSound_Buy_File;
	BOOL		m_bSound_Sell_Use;
	CString		m_szSound_Sell_File;

	CSTOrderRData_EG()
	{
	}

	void Copy(CSTOrderRData_EG* pOrg)
	{
		CSTOrderRData_SS::Copy(pOrg);

		m_nOrderMarketEntry = pOrg->m_nOrderMarketEntry;
		m_nOrderMarketClear = pOrg->m_nOrderMarketClear;

		m_bSound_Buy_Use = pOrg->m_bSound_Buy_Use;
		m_szSound_Buy_File = pOrg->m_szSound_Buy_File;
		m_bSound_Sell_Use = pOrg->m_bSound_Sell_Use;
		m_szSound_Sell_File = pOrg->m_szSound_Sell_File;
	}
};


/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
#define FN_TRAN                       "fn_HFIBOA0V8400"
#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* HFIBOA0V8400InRec: �����̼�����                                                   */
typedef struct
{
//    char    InItem1[10];                          /* $�����ID(10)                   */
    char    InItem1[16];                          /* $�����ID(16)                   */
}fn_HFIBOA0V8400_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* HFIBOA0V8400OutRec1: �����̼�����                                                 */
typedef struct
{
    char    OutItem1[1];                          /* Result(1)                       */
}fn_HFIBOA0V8400_OutRec1;

/*************************************************************************************/

#endif //AFX_SAMSUNG_ORDERDEF_H_73649_BY_98B1A__INCLUDED_
