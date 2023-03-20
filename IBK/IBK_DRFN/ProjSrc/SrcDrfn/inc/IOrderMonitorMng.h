#if !defined(AFX_INTERFACE_OF_ORDERMONITORMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_ORDERMONITORMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_

// #include "../../_Include/IAUOrderMonitorMng.h"
#include "IBaseDefine.h"

//  int nID = 8;
// 	IAUOrderMonitorManager* pMng = (IAUOrderMonitorManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUOrderMonitorManager* pMng = (IAUOrderMonitorManager*)AfxGetPctrInterface(nID);

// ���� : �ֹ�����͸�! �ֹ��� ����͸��ϴ� �Ŵ���
//
// ������  : ������ �ִ� ��� ���ؿ� 2005.12.03(��)
// ������ �� : ���� �׻� �� �տ� �ִ�. ���� �� ���� �̷�� ���� ���õ� ��ȹ�� ���� �ൿ�Ѵ�.
// History : 2005.12.03 �������̽� ����
//

//typedef struct : public LOADCONTENTINFO
//{
//	char		LogType[1+1];		// �α׳���� Ÿ��
//									// 0 : ����� �� ����. 
//									// 1 : �α׹߻��� �������� �α�����
//									// 2 : ����ÿ��� ����.
//	char		reserved[15];		// �������� �̸� �Ҵ��� ����.
//} LOADCONTENTINFOEX;
//#define		LOADCONTENTINFOEX_SIZE	sizeof(LOADCONTENTINFOEX)


typedef struct {
	char  screen_cd     [  4+1];	// ȭ��ID
	char  screen_nm		[ 40+1];	// ȭ���̸�
	char  screen_hwnd	[  8+1];	// ȭ�� �������ڵ�
	char  worktime		[  8+1];	// ���۽ð�
	// ----------------------------------------
	char  flag			[  1+1];	// 'S' : ȭ�����
									// 'E' : ȭ������
									// 'C' : �������� Ŭ����
	//------------------�Ʒ��κ��� �������� ������---------
	char  logDate		[ 10+1];	// Log����
	char  logTime		[  8+1];	// Log�ð�
} DSBM_RUNSTATE;
#define		DSBM_RUNSTATE_SIZE		sizeof(DSBM_RUNSTATE)


typedef struct {
	char  acct_code		[ 13+1];	// ���¹�ȣ
	char  acct_nm		[ 20+1];	// ���¸�
	char  screen_cd     [  4+1];	// ȭ��ID
	char  screen_nm		[ 40+1];	// ȭ���̸�
	char  screen_hwnd	[  8+1];	// ȭ�� �������ڵ�
	char  item_cd		[ 12+1];	// �����ڵ�
	char  item_nm		[ 40+1];	// �����
	char  worktime		[  8+1];	// ���۽ð�
	char  msg			[100+1];	// ���� �� ����
	DWORD dwKey			       ;	// KEY��
	// ----------------------------------------
} DSBM_BASECOMMON;
#define		DSBM_BASECOMMON_SIZE		sizeof(DSBM_BASECOMMON)

typedef struct : public DSBM_BASECOMMON {
//	DSBM_BASECOMMON ����ü����..................................
//  ............................................................
	char  trade_sect	[  1+1];	// �Ÿű��� 1:�ŵ�, 2:�ż�, 3:����, 4:���
	char  qty			[  9+1];	// �ֹ�����
	char  prc			[ 13+1];	// �ֹ�����
	char  auto_sect		[  1+1];	// 1:�ڵ�, 2:���ڵ�
	char  flag			[  1+1];	// �����ڵ� ó���� ���� ���� �÷���
									// 'N' : New(���ο� ����)
									// 'D' : Delete(����)
									// 'U' : Update
	//------------------�Ʒ��κ��� �������� ������---------
	char  logDate		[ 10+1];	// Log����
	char  logTime		[  8+1];	// Log�ð�
} DSBM_RUN;
#define		DSBM_RUN_SIZE		sizeof(DSBM_RUN)


// TRLOG_RUN ����ü�� ������ TR�� ��������� ����ü..
typedef struct  {
	char  acct_code		[ 13+1];	// ���¹�ȣ
	char  acct_nm		[ 20+1];	// ���¸�
	char  screen_cd     [  4+1];	// ȭ��ID
	char  screen_nm		[ 40+1];	// ȭ���̸�
	char  screen_hwnd	[  8+1];	// ȭ�� �������ڵ�
	char  item_cd		[ 12+1];	// �����ڵ�
	char  item_nm		[ 40+1];	// �����
	char  worktime		[  8+1];	// ���۽ð�
	char  msg			[100+1];	// ���� �� ����
	char  aKey			[  8+1];	// KEY��
	// ----------------------------------------
	char  trade_sect	[  1+1];	// �Ÿű��� 1:�ŵ�, 2:�ż�, 3:����, 4:���
	char  qty			[  9+1];	// �ֹ�����
	char  prc			[ 13+1];	// �ֹ�����
	char  auto_sect		[  1+1];	// 1:�ڵ�, 2:���ڵ�
	char  flag			[  1+1];	// �����ڵ� ó���� ���� ���� �÷���
									// 'N' : New(���ο� ����)
									// 'D' : Delete(����)
									// 'U' : Update
	//------------------�Ʒ��κ��� �������� ������---------
	char  logDate		[ 10+1];	// Log����
	char  logTime		[  8+1];	// Log�ð�
} TRLOG_RUN;
#define		TRLOG_RUN_SIZE		sizeof(TRLOG_RUN)


typedef struct  : public DSBM_BASECOMMON {
//	DSBM_BASECOMMON ����ü����..................................
//  ............................................................
	char  ord_no		[ 10+1];	// �ֹ���ȣ
	char  trade_sect	[  1+1];  	// �Ÿű��� 1:�ŵ�, 2:�ż�, 3:����, 4:���
	char  prc			[ 13+1];	// �ֹ�����
	char  current		[ 13+1];	// �ֹ��� ���簡
	char  qty			[  9+1];	// �ֹ�����


	char  ord_sect		[  2+1];	/* �ֹ�����[�ֹ�����]        
										������ ���    
											01:������         
											02:���尡         
											03:���Ǻ�������   
											04:������������   

										������ ���    
											01:������(����)
											05:���尡
											06:���Ǻ�������
											12:������������
											13:�ֿ켱������
											71:�ð�������
											??:����������			*/										
	char conclsn_sect	[  2+1];	/* ü������ (�ֽ� �ż�/�ŵ�/����)
											00:�Ϲ�
											01:IOC(�ܷ�������)
											02:FOK(���� ü��/���)	*/
	char  auto_sect		[  1+1];	// 1:�ڵ�, 2:���ڵ�

	//------------------�Ʒ��κ��� �������� ������---------
	char  logDate		[ 10+1];	// Log����
	char  logTime		[  8+1];	// Log�ð�

} DSBM_SBLOG;
#define		DSBM_TRADELOG		DSBM_SBLOG
#define		DSBM_SBLOG_SIZE		sizeof(DSBM_SBLOG)


// �ʿ��� ������ �� ���ͼ� Ȯ����. 2005.08.31
typedef struct : public DSBM_SBLOG {
	char  logType		[  1+1];	// DSBM_RUN�� flag + ���ֹ�
									// 'N' : ���
									// 'D' : ���
									// 'U' : ����
									// 'R' : ���ֹ�

}DSBM_SBLOGEX;
#define		DSBM_SBLOGEX_SIZE		sizeof(DSBM_SBLOGEX)


// �ֹ����ո���͸�ID(SBMID_)
#define	SBMID_RUNSTATE				0x01	// ���ۻ��¾˸�
#define SBMID_CONDITION				0x02	// �����������
#define SBMID_SBLOG					0x04	// �ֹ��α�
#define SBMID_COND_SBLOG			0x07	// ��������+�ֹ��α� : �ֹ��α��ܿ� �α׸� �����.

#define SBMID_CLEARALLITEM			0x16	// �ش� ȭ���� ����Ʈ �ڵ�Ŭ����.
											// ȭ���� ���� �� �ڵ����� �ش�ȭ���� ������ ���ﶧ

// �迭�� ����
//#define		ARRAYCOUNT( array )		(sizeof(array) /sizeof(array[0]))


DECLARE_INTERFACE(IOrderMonitorManager)
{
	// ȭ���� ���ۻ��¸� ��Ÿ����.
	STDMETHOD_(BOOL, SetState)(DSBM_RUNSTATE* pData, long dwLong=NULL) PURE;

	// ������ ���ۻ��¸� ��Ÿ����.
	STDMETHOD_(BOOL, SetCondition)(DSBM_RUN* pData, long dwLong=NULL) PURE;

	// �ֹ��α׸� �����.
	STDMETHOD_(BOOL, SetTradeLog)(DSBM_SBLOG* pData, long dwLong=NULL) PURE;

};

#endif // AFX_INTERFACE_OF_ORDERMONITORMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_
