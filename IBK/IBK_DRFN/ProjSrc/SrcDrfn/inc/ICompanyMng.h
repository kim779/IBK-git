#if !defined(AFX_HANARO_ICOMPANYMAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_HANARO_ICOMPANYMAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_

/* ------------------------------------------------------------------------
����Ʈ���� �ٸ� �κ��� ó���ϱ� ���� ��ƾ
����Ʈ�� �°� �� �����ϱ� �ٶ��ϴ�.
�ʿ��� ��� PcTrMng���� 43������ �����ؼ� ����մϴ�.
------------------------------------------------------------------------ */

// #include "../../inc/ICompanyMng.h"
#include "IBaseDefine.h"

#define		UUID_ICompanyManager		43
//  int nID = UUID_ICompanyManager;
// 	ICompanyManager* pGateMng = (ICompanyManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	ICompanyManager* pMng = (ICompanyManager*)AfxGetPctrInterface(nID);

enum 
{
	nPCTRComID_DAISHIN=1,	//���
	nPCTRComID_DAEWOO	,	//���
	nPCTRComID_HYUNDAI	,	//����
	nPCTRComID_WOORI	,	//�츮
	nPCTRComID_HANWHA	,	//��ȭ
	nPCTRComID_DONGBU	,	//����
	nPCTRComID_SK		,	//SK	
	nPCTRComID_KYOBO	,	//����
	nPCTRComID_DONGWON	,	//����
	nPCTRComID_KIWOOM	,	//Ű��
	nPCTRComID_SEOUL	,	//����
	nPCTRComID_SAMSUNG	,	//�Ｚ
	nPCTRComID_BOOKOOK	,	//�α�
	nPCTRComID_HANTU	,	//����
	nPCTRComID_KOSCOM	,	//����
	nPCTRComID_EUGENE	,	//����
	nPCTRComID_KB			//KB
};

// ����Ʈ ȸ�纰 ó���� ���� ��ƾ
// ���� �̷�ƾ�� �Ｚ��.
// �ٸ� ����Ʈ�� ������ ���ٶ�.
DECLARE_INTERFACE(ICompanyManager)
{
	//���� ���ǵ� ����.
	STDMETHOD_(int, GetID)() PURE;

	// ���� ���ǵ� ������ 'nPCTRComID_'�� �� �κ�.
	// ex nPCTRComID_SAMSUNG -> SAMSUNG
	STDMETHOD_(LPCSTR, GetName)(int nID) PURE;
};

typedef void (CALLBACK* SysEduCallbackProc)(long pThisPoint, LPCSTR dwMainKey, long dwSubKey, long dwNotifyData);


typedef struct 
{
	long pThisPoint;
	CString szMainKey;
	long dwSubKey;
	long dwNotifyData;
} STSysCallProcItem;

#define		ACC_SERIAL			0		// ������ǰ�Ϸù�ȣ(3)
#define		ACC_PWD				1		// ���º�й�ȣ  Passwd(8)
#define		ACC_CODE_NAME		2		// ���¸�(20)
#define		ACC_PRODUCTCODE		3		// ������ǰ�ڵ�(3)
#define		ACC_PRODUCNAME		4		// ������ǰ������(30)
#define		ACC_TYPECODE		5		// ���°��������ڵ�(3)
#define		ACC_LAPTYPECODE		6		// �������ڵ�(1)(�ֹ��� �������� ����, ���� ��ȸ�� ����)
#define		ACC_OLDACCTYPECODE	7		// �����±��л��ڵ�(2)
#define     ACC_STIPYN          8		// ���վ��Ȯ�ο���(1)
#define		ACC_ISALL			9		// ���տ���(1)
#define		ACC_ISCLOSE			10		// ��⿩��(1) 
#define		ACC_HTS				11		// HTS����ޱ����ڵ�(1)
#define		ACC_ISPWD			12		// ���

typedef struct
{
	CString szAcct;					// ���¹�ȣ
	CString	InData[ACC_ISPWD+1];	// ���°�������.
} STACC_SSDATA;	// �Ｚ��������.

typedef CList<STACC_SSDATA*, STACC_SSDATA*> LIST_STACC_SSDATA;

#define LIST_STACC_SSDATA_DELETEALL(xList) { for(POSITION pos=xList.GetHeadPosition(); pos;) delete xList.GetNext(pos); }
#define LIST_STACC_SSDATA_DELETEALLP(xpList) { for(POSITION pos=xpList->GetHeadPosition(); pos;) delete xpList->GetNext(pos); }

DECLARE_INTERFACE_(ICompanyManager_SS, ICompanyManager)
{
	STDMETHOD_(int, GetAcctCode)(CStringList& listAcct, CStringList& listAcctName) PURE;
	STDMETHOD_(CString, GetPwd)() PURE;

//	STDMETHOD_(long, OnGetData)(WPARAM wParam, LPARAM lParam) PURE;
//
//	// �����ֹ�.
//	// 1:HTS  0:FRONT(����)
//	STDMETHOD_(int, RqNewOrder)(HWND hWnd, long/*COrderSave**/ pSave, int p_nUserType) PURE;	//COrderSave* pSave
//	
//	// �ֽĸż��ֹ�.
//	// 1:HTS  0:FRONT(����)
//	STDMETHOD_(int, RqNewOrderStockBuy)(HWND hWnd, long _pSave, int p_nUserType) PURE;
//
//	// �ֽĸŵ��ֹ�.
//	// 1:HTS  0:FRONT(����)
//	STDMETHOD_(int, RqNewOrderStockSell)(HWND hWnd, long _pSave, int p_nUserType) PURE;
//
//	// �ֹ��� �������� ���� ��
//	STDMETHOD_(BOOL, GetLastMessage)(CString& szMsgCode, CString& szMsg) PURE;


	// ���ϰ� : 0(�̱���), 1(�����Ϸ�), 0���ϰ�(��ȸ������)
	STDMETHOD_(int, GetSysEdu)(LPCSTR szKey, long pThisPoint, long dwKey, SysEduCallbackProc pCallFunc) PURE;

	STDMETHOD_(void, RemoveSysEdu)(LPCSTR szKey, long pThisPoint, long dwKey, SysEduCallbackProc pCallFunc) PURE;

	// listAcct : ������ ���¸�.
	// listAcctName : ���¸�.
	// listAcctType : ����Ÿ�� ������.
	// listAcctTypeName : ����Ÿ�Կ� ���� �̸�.
	STDMETHOD_(int, GetAcctCodeEx)(LIST_STACC_SSDATA &ListAcctInfo, int nReqType) PURE;

	// �������� ���ϱ�
	// nAcctSize �����ڸ���, ������ 11�ڸ�. �����ִ� 8�ڸ������� ������ ���� �� �ִ�.
	//			8 or 11 �������ϰ� �Ǿ�����.
	// nInfoType
	//				0		// ������ǰ�Ϸù�ȣ(3)
	//				1		// ���º�й�ȣ  Passwd(8)
	//				2		// ���¸�(20)
	//				3		// ������ǰ�ڵ�(3)
	//				4		// ������ǰ������(30)
	//				5		// ���°��������ڵ�(3)
	//				6		// �������ڵ�(1)(�ֹ��� �������� ����, ���� ��ȸ�� ����)
	//				7		// �����±��л��ڵ�(2)
	//				8		// ���վ��Ȯ�ο���(1)
	//				9		// ���տ���(1)
	//				10		// ��⿩��(1) 
	//				11		// HTS����ޱ����ڵ�(1)
	//				12		// ���
	STDMETHOD_(LPCSTR, GetAcctInfo)(LPCSTR szAcct, int nAcctSize, int nInfoType) PURE;
};

//#define	gCURRENT_COMPANY_ID	nPCTRComID_SAMSUNG	// �Ｚ
#define	gCURRENT_COMPANY_ID	nPCTRComID_EUGENE	// ����

#define SZFNPROMAX_SYSEDU		"FNPROMAX_SYSEDU"
#define SZEUGENE_SYSEDU			"EUGENE_SYSEDU"

#define gGetSamsungSysEdu(xSysEdu) { xSysEdu=-1; char aBuf[10+1]={0,};		\
	if(::GetEnvironmentVariable(SZFNPROMAX_SYSEDU, aBuf, 10)>0)	{		\
	if(strcmp(aBuf, "1")==0 || strcmp(aBuf, "0")==0) xSysEdu = atoi(aBuf);	}}

#define gGetEugeneSysEdu(xSysEdu) { xSysEdu=-1; char aBuf[10+1]={0,};		\
	if(::GetEnvironmentVariable(SZEUGENE_SYSEDU, aBuf, 10)>0)	{		\
	if(strcmp(aBuf, "1")==0 || strcmp(aBuf, "0")==0) xSysEdu = atoi(aBuf);	}}

//#define	SAMSUNG_VERSION		200703	// 2007.03������ ����� Max
#define	SAMSUNG_VERSION		200709	// 2007.09���¿� �Žý��۹���.

#endif //AFX_HANARO_ICOMPANYMAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_
