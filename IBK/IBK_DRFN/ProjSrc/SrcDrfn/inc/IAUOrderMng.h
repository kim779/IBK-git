#if !defined(AFX_ORDER_INTERFACE__BY_LYH_968A_755DC4FC4516__INCLUDED_)
#define AFX_ORDER_INTERFACE__BY_LYH_968A_755DC4FC4516__INCLUDED_

// #include "../../_include/IAUOrderMng.h"
//  int nID = 11;
// 	IAUOrderManager* pMng = (IAUOrderManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUOrderManager* pMng = (IAUOrderManager*)AfxGetPctrInterface(nID);
//  IOrder* pOrder = pMng->AdviseIOrder(hWnd, pITrCommSite);
//  pMng->UnAdviseIOrder(hWnd);

// ���� : ������ �����Ϳ� �������� ������ ���� ID�� �ο��Ѵ�.
// IOrderSite �� ������ ���� �����͸� �޴� Client�κ��̸�
// IAUOrderManager �� ������ ���� �����͸� �����ϴ� Server�κ��̴�.
//
// ������  : ������ 2005.11.09(ȭ)
// History : 2005.11.09 �������̽� ����
//

// PacketType
#define PT_ST	0	// �ֽ�			: COrder_PT_ST (CommonTR/Define_Order_PT_ST.h)
#define PT_FO	1	// ����&�ɼ�	: COrder_PT_FO (CommonTR/Define_Order_PT_FO.h)
#define PT_SO	2	// �ֽĿɼ�		: COrder_PT_SO (CommonTR/Define_Order_PT_SO.h)

// return value of SendOrder
#define RV_SUCCESS		1	// �ֹ� ó�� ��� : �ֹ� ����
#define RV_CONF			-1	// �ֹ� ó�� ��� : �ֹ�Ȯ��â���� ���
#define RV_ERROR_PARAM	-2	// �ֹ� ó�� ��� : �ֹ� �Ķ���� ����
#define RV_ERROR_DATA	-3	// �ֹ� ó�� ��� : �ֹ� ����Ÿ ����

// �ֹ� Ȯ��â
#define CONF_NONE	0	// �ȶ��
#define CONF_SHOW	1	// ����
#define CONF_MAIN	2	// �ȶ��(������)

// return value of RequestTR45162
#define RV_ERROR_45162_ACCT		-54	// ����
#define RV_ERROR_45162_PASS		-55	// ��й�ȣ
#define RV_ERROR_45162_CODE		-56	// �����ڵ�


/*
DECLARE_INTERFACE(IOrderSite)
{
};
*/

DECLARE_INTERFACE(IOrder)
{
	// -----------------------------------------------------------------------
	//	�ֹ� ����
	// I-PARAM : �ֽ�/����/�ֿ� Ÿ��, TR��ȣ, �ֹ�Cnt, Map��ȣ, ����Ÿ��Ʈ����, �ֹ�Ȯ��â ��뿩��, ����Ÿ���� ��뿩��
	//		LPVOID pData : ����Ÿ��Ʈ���� ���� ����
	//			#include "../CommonTR/Define_Order_PT_FO.h" --> �����ɼ��ֹ� ��Ŷ
	//			#include "../CommonTR/Define_Order_PT_ST.h" --> �ֽ��ֹ� ��Ŷ
	//			#include "../CommonTR/Define_Order_PT_SO.h" --> �ֽĿɼ��ֹ� ��Ŷ
	// O-PARAM : �ֹ� ó�� ���
	STDMETHOD_(int, SendOrder)(int nType, LPCSTR szTrNo, int nCount, LPCSTR szMapNo, LPVOID pData, int nFlag=CONF_MAIN, int nVerifyFlag=FALSE) PURE;
	
	// -----------------------------------------------------------------------
	//	�ֹ� ���ɼ��� TR ��ȸ
	// I-PARAM : CTR_45162_I (���� : CommonTR/Tr45162.h)
	// O-PARAM : �ֹ� ó�� ���
	STDMETHOD_(int, RequestTR45162)(LPVOID pData) PURE;

	// -----------------------------------------------------------------------
	//	�ֹ� ������
	// I-PARAM :
	// O-PARAM :
	STDMETHOD_(void, SetDebug)(LPVOID pST_Cond, LPCSTR szFileName, BOOL bOnOff=FALSE) PURE;

	// -----------------------------------------------------------------------
	//	����
	// I-PARAM :
	// O-PARAM :
	STDMETHOD_(void, ExtraAction)(int nType, LPCSTR szData, LPVOID pVoid) PURE;

	// -----------------------------------------------------------------------
	//	����Ÿ ��ȣȭ ����
	// I-PARAM : �⺻�� - ��ȣȭ�Ѵ�. 
	// O-PARAM : 
	STDMETHOD_(void, SetEnc)(BOOL bEnc = TRUE) PURE;
	//	�������� ��뿩��
	// I-PARAM : �⺻�� - ���������� ����Ѵ�. (�α�� ���� ���ð� ���յǴ� ��츸 �����)
	// O-PARAM : 
	STDMETHOD_(void, SetCert)(BOOL bCert = TRUE) PURE;
	
	// -----------------------------------------------------------------------
	//	�ֹ�Ȯ��â ���
//	STDMETHOD_(void, SetBaseDesk)(LPVOID pBaseDesk) PURE;
//	STDMETHOD_(void, SetBaseDeskIUnknown)(LPUNKNOWN pUnKnown) PURE;
	// I-PARAM :	szConfMapNo		- �ֹ�Ȯ��â ȭ���ȣ (�ʿ��� ����ϴ� ��ȣ�� ������ ��ȣ)
	//										* �ֽı⺻ :"10012"  
	//										* �ֽ��ϰ� :"10201"
	//										* ���ɱ⺻ :"14012"
	//										* �����ϰ� :"14148"
	//				bMarketPrice	- �ϰ��ֹ��� ���尡�ֹ����� ex) [1315]�ϰ����尡�ŵ�, [1365]���尡�ϰ�û��
	//				bSettleOrder	- �ϰ��ֹ��� û���ֹ� ����	ex) [1365]�ϰ�û��, ���尡�ϰ�û��
	//				nMultiOrdType	- �ϰ��ֹ��� ���� (1:�ϰ��ŵ�, 2:�ϰ��ż�, 3:�ϰ�����, 4:�ϰ���� (0:�⺻-�ϰ�û���ֹ��� ���))
	STDMETHOD_(BOOL, SetOrderConfMapNo)(LPCSTR szConfMapNo, BOOL bMarketPrice=FALSE, BOOL bSettleOrder=FALSE, int nMultiOrdType=0) PURE;

};


DECLARE_INTERFACE(IAUOrderManager)
{
	STDMETHOD_(LONG, AdviseIOrder) (HWND hWnd, LONG pITrCommSite) PURE;
	STDMETHOD_(LONG, UnAdviseIOrder)  (HWND hWnd)  PURE;
};



#endif // AFX_ORDER_INTERFACE__BY_LYH_968A_755DC4FC4516__INCLUDED_
