#ifndef __STORDER_DEF__BYALZIO__060914__HEADER__
#define __STORDER_DEF__BYALZIO__060914__HEADER__

// #include "../Include_ST/STOrderDef.h"

#define		LANGUAGE_MAX			180000

typedef struct _ST_ORDERSIGN
{
	char In[5];
	char AcntNo[20];                      // ���¹�ȣ
	char InptPwd[8];					  // �Էº�й�ȣ
	char IsuNo[12];						  // �����ȣ
	long OrdQty;						  // �ֹ�����
	double OrdPrc;						  // �ֹ���
	char BnsTp[1];						  // �Ÿű���
	char OrdprcPtnCode[2];				  // ȣ�������ڵ�
	char PrgmOrdprcPtnCode[2];			  // ���α׷�ȣ�������ڵ�
	char StslAbleYn[1];					  // ���ŵ����ɿ���
	char StslOrdprcTp[1];				  // ���ŵ�ȣ������
	char CommdaCode[2];					  // ��Ÿ�ü�ڵ�
	char MgntrnCode[3];					  // �ſ�ŷ��ڵ�
	char LoanDt[8];						  // ������
	char MbrNo[3];						  // ȸ����ȣ
	char OrdCndiTp[1];					  // �ֹ����Ǳ���
	char StrtgCode[6];					  // �����ڵ�
	char GrpId[20];						  // �׷�ID
	long OrdSeqNo;						  // �ֹ�ȸ��
	long PtflNo;						  // ��Ʈ��������ȣ
	long BskNo;							  // �ٽ��Ϲ�ȣ
	long TrchNo;						  // Ʈ��ġ��ȣ
	long ItemNo;						  // �����۹�ȣ
	char OpDrtnNo[12];					  // ������ù�ȣ
	char Out[5];
//	long OrdNo;							  // �ֹ���ȣ
} ST_ORDERSIGN;

typedef struct {
	int		nMarket;				// 0:�ֽ�, 1:����/�ɼ�
	int		nOrderType;				// VALENTRYSELL �ż�����, VALEXITLONG �ż�û��, VALEXITSHORT �ŵ�û��, VALENTRYBUY �ŵ�����
	char	aAcctNo[20+1];			// ���¹�ȣ
	char	aAcctName[30+1];		// ���¸�
	char	aCode[12+1];			// �����ڵ�
	char	aCodeName[30+1];		// �����ڵ��
	int		nOrderGubun;			// �ֹ�����, 0:������, MARKETENTRY : ���Խ��尡, MARKETCLEAR : û����尡.
	char	aOrderAmt[16+1];		// �ֹ�����
	// char	aOrderPrice[20+1];		// �ֹ�����
	short	nOrderPriceTypeEnt;		// �ֹ�����Ÿ��. �����޺��� �ε���
	short	nOrderPriceTypeClr;		// �ֹ�����Ÿ��. û���޺��� �ε���
	//-----------------------
	int		nSTType;				// ������ ����, ��������(0), ����� ����(1)
	char	szSTName[MAX_PATH];		// ������
	short	nAutoOrder;				// �ڵ��ֹ�? 0 : �ֹ����� 1/���ڵ� 2/�ڵ�
} STORDER_INDATA;

#define		SZSTORDER_INDATA	sizeof(STORDER_INDATA)

typedef  int (WINAPI* FpSTOrder_DoModal)(long pSTORDER_INDATA);
typedef  int (WINAPI* FpSTOrder_Sign)(long, float);

class CStrategyOrderLoader
{
public:
	HINSTANCE m_hLib;
	CString m_szLibName;
	FpSTOrder_DoModal	m_fpSTOrder_DoModal;
	FpSTOrder_Sign	m_fpSTOrder_Sign;

public:
	CStrategyOrderLoader()
	{
		m_hLib = NULL;
		m_fpSTOrder_DoModal = NULL;
		m_szLibName = "StrategyOrder.dll";
	}

	~CStrategyOrderLoader()
	{
		UnLoadLib();
	}


	HINSTANCE LoadLib()
	{
		HINSTANCE hLib = NULL;
		hLib  = (HINSTANCE) ::LoadLibrary((LPCTSTR)m_szLibName);
		if( hLib == NULL)
		{
			CString szMsg;
			szMsg.Format("%s�� Load�� �� �����ϴ�.", m_szLibName);
			TRACE("%s\n", szMsg);
			return NULL;
		}
		m_hLib = hLib;
		LoadProcess();

		return hLib;
	}

	BOOL UnLoadLib()
	{
		if(m_hLib == NULL)  return TRUE;

		BOOL bResult = ::FreeLibrary(m_hLib);
		m_hLib = NULL;

		return bResult;
	}


	void LoadProcess()
	{
		if(!m_hLib)	return ;

		m_fpSTOrder_DoModal = (FpSTOrder_DoModal)::GetProcAddress(m_hLib, "STOrder_DoModal");
		m_fpSTOrder_Sign = (FpSTOrder_Sign)::GetProcAddress(m_hLib, "STOrder_Sign");
	}

	int	STOrder_DoModal(long pSTORDER_INDATA)
	{
		if(m_hLib==NULL && LoadLib()==FALSE) return -1;	// DLL�ε忡��
		if(pSTORDER_INDATA==NULL) return -1;			// �Է°�����

		return m_fpSTOrder_DoModal(pSTORDER_INDATA);
	}

	int	STOrder_Sign(long pSTORDER_INDATA, float fHogaData)
	{
		if(m_hLib==NULL && LoadLib()==FALSE) return -1;	// DLL�ε忡��
		if(pSTORDER_INDATA==NULL) return -1;			// �Է°�����

		return m_fpSTOrder_Sign(pSTORDER_INDATA, fHogaData);
	}
};

//#define		STORDER_TITLE_0		"�ý���Ʈ���̵� �ż�����"//VALENTRYBUY
//#define		STORDER_TITLE_1		"�ý���Ʈ���̵� �ż�û��"//VALEXITLONG
//#define		STORDER_TITLE_2		"�ý���Ʈ���̵� �ŵ�û��"//VALEXITSHORT
//#define		STORDER_TITLE_3		"�ý���Ʈ���̵� �ŵ�����"//VALENTRYSELL
//#define		STORDER_TITLE_4		"�ý���Ʈ���̵� �ŵ�/�ż�û��" //VALSELL
//#define		STORDER_TITLE_5		"�ý���Ʈ���̵� �ż�/�ŵ�û��" //VALBUY
typedef struct _PROCEED_ORDER
{
	LPVOID pThis;
	int  nOrderIndex;
	int nOrderQty;				// �ű� �ֹ� ���ɼ���
	int nClearQty;				// û�� �ֹ� ���ɼ���
	float fOrderPriceEnt;
	float fOrderPriceClr;
}PROCEED_ORDER;


//typedef struct	tagSIGNALORDERSINFO
//{
//	long    hDLLWnd;				//	 ȣ���� ��Ʈ�� �ڵ�
//	TCHAR	szIndexNo[2+1];		// - DLL Index��ȣ (��, 11(�ֽĽü�), 12(���ɽü�), 13(�����ü�), 14(�ֽĽ�ž), 15(���ɽ�ž), 16(�ý���Ʈ���̵�)
//	TCHAR	szWindName[8+1];		// - ��ȣ�̸� ("�ֽĽü�", "���ɽü�", "���ɽ�ž", "���ǰ˻�" .....)
//	TCHAR	szSignalgb[2+1];		// - ��ȣ���� ("��", "��" ....., "��")
//	TCHAR	szPopgb[1+1];			// - �˾�Type(0:�̽���, 1:�˸�â, 2:�ֹ�â,  4:�ڵ��ֹ�)
//	TCHAR	szActionGb[2+1];		// - ��ư ó�� ���а� (1:�ֹ�, 2:���, 3:��������) : x
//	TCHAR	szAcctno[11+1];			// - ���¹�ȣ
//	TCHAR	szCode[12+1];			// - �����ڵ�
//	TCHAR	szLoangb[1+1];			// - �ſ뱸��(0:����, 1:�ſ�)
//	TCHAR	szSygb[2+1];		  	// /*  �ſ뱸��                        */
//	TCHAR	szJanGogb[1+1];			// - �ܰ���(0:��ü, 1:�ŵ�, 2:�ż�)
//	TCHAR	szMemegb[1+1];			// - �Ÿű���(1:�ŵ�, 2:�ż�, 3:����, 4:���, 5:�ſ�ŵ�, 6:�ſ�ż� )
//	TCHAR	szOrderNo[5+1];			// - �ֹ���ȣ
//	TCHAR	szOrgNo[5+1];			// - ���ֹ���ȣ
//	TCHAR	szOrdgb[2+1];			// - �ֹ�����(���밡, ���尡,...)
//	TCHAR	szOrdqtygb[1+1];		// - �ֹ���������(0:����, 1:�ݾ�, 2:�ŵ����ɼ���%)
//	TCHAR	szOrdqty[15+1];			// - �ֹ�����
//	TCHAR	szOrdprcgb[1+1];		// - �ֹ��ܰ�����(0:������...(�ε���))
//	TCHAR	szOrdprc[7+1];			// - �ֹ��ܰ�
//	TCHAR	szOrdRow[3+1];			// - ���õ� (��ž �ɸ�) ROW
//	TCHAR	szLoanDate[8+1];		// - ��������
//	TCHAR	szLoanType[1+1];		// - ���ⱸ��	(0:�ڱ�, 1:����)	
//	TCHAR	szbContinue[1+1];		// - ���� ���࿩��
//	TCHAR	szMsg[512+1];			// - ����MSG
//	TCHAR	szOrdMsg[512+1];		// - ����MSG
//	
//// --> [Edit]  ������ 2008/07/07
//	// ��ž �ν� �ֹ� �α� 
//	TCHAR	szMenhceCancel[1+1];		// ��ü�����ۿ��� (0:��ü�����۾���, 1:��ü��������)
//	TCHAR	szMenhceCancelSuccess[1+1];	// ��ü�����ۿ��� (0:��ü�� ����, 1:��ü�� ����)
//// <-- [Edit]  ������ 2008/07/07
//
//// --> [Edit]  �̿��� 2008/05/23
//	TCHAR	szAccName[30+1];		// - ���¸�
//	TCHAR	szJmName[20+1];			// - �����
//	TCHAR	szRealOrdqty[15+1];		// - �����ֹ�����
//	TCHAR	szRealOrdprc[7+1];		// - �����ֹ��ܰ�
//// <-- [Edit]  �̿��� 2008/05/23
//} SIGNALORDERSINFO;


typedef struct	tagSTORDERINFO
{
	TCHAR	szPopgb[1+1];			// - �˾�Type(1 : �ֹ��Է�â��)
	TCHAR	szCode[12+1];			// - �����ڵ�
	TCHAR	szAcctno[11+1];			// - ���¹�ȣ
	TCHAR	szMemegb[1+1];			// - �Ÿű���(0:�ż�, 1:�ŵ�)
	TCHAR	szOrdgb[2+1];			// - �ֹ�����(00:���밡, 03:���尡,...)
	TCHAR	szOrdqty[15+1];			// - �ֹ�����
	TCHAR	szOrdprc[10+1];			// - �ֹ��ܰ�
} STORDERINFO;

#define WMU_SIGNAL_CHART		(WM_USER + 659)	// ��ȣ�˸��� - ��Ʈ
typedef struct _stSTORDERMSGINFO
{
	char szTime[6];			// �߻��ð�
	char szItemCode[12];	// �����ڵ�
	char szItemName[30];	// �����
	char szSTName[40];		// ������
	char szOrderType[1];	// �Ÿű���(0:�ŵ�, 1:�ż�)
	char szAccount[11];		// �ֹ����¹�ȣ
	char szOrdgb[2];		// �ֹ�����(1:���尡)
	char szOrdqty[15];		// �ֹ�����
	char szOrdprc[10];		// �ֹ��ܰ�
} stSTORDERMSGINFO;
#endif //__STORDER_DEF__BYALZIO__060914__HEADER__