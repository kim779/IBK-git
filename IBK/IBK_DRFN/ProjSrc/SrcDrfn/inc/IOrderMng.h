// IOrderMng.h


// ���� : ������ �����Ϳ� �������� ������ ���� ID�� �ο��Ѵ�.
// IOrderSite �� ������ ���� �����͸� �޴� Client�κ��̸�
// IOrderManager �� ������ ���� �����͸� �����ϴ� Server�κ��̴�.
//
// ������  : ������ 2005.11.09(ȭ)
// History : 2005.11.09 �������̽� ����
//

// PacketType
#define PT_ST	0	// �ֽ�
#define PT_FO	1	// ����&�ɼ�
#define PT_SO	2	// �ֽĿɼ�

// return value
#define RV_SUCCESS		1	// �ֹ� ó�� ��� : �ֹ� ����
#define RV_CONF			-1	// �ֹ� ó�� ��� : �ֹ�Ȯ��â���� ���
#define RV_ERROR_DATA	-2	// �ֹ� ó�� ��� : �ֹ� ����Ÿ ����
#define RV_ERROR_PARAM	-3	// �ֹ� ó�� ��� : �ֹ� �Ķ���� ����

// �ֹ� Ȯ��â
#define CONF_NONE	0	// �ȶ��
#define CONF_SHOW	1	// ����
#define CONF_MAIN	2	// ������ ȯ�漳���� ������

//DECLARE_INTERFACE(IOrderSite)
//{
//};


DECLARE_INTERFACE(IOrderManager)
{
	// -----------------------------------------------------------------------
	//	�ֹ� ����
	// I-PARAM : TR��ȣ, ȭ���ڵ�, ����Ÿ��Ʈ����, ����, �ֹ�Ȯ��â ��뿩��
	// O-PARAM : �ֹ� ó�� ���
	STDMETHOD_(int, Send2Server)(int nType, LPCSTR szTrNo, HWND hWnd, LPVOID pData, int nFlag=CONF_MAIN) PURE;

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
};
