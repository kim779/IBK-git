#if !defined(AFX_IBKSCONNECTORCTL_H__A5FEE1A3_98BC_4E96_B29C_2C7954439131__INCLUDED_)
#define AFX_IBKSCONNECTORCTL_H__A5FEE1A3_98BC_4E96_B29C_2C7954439131__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "AxisWizard.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

#include <set>
#include <functional>

#define	DELWND(w)	{ if(w) {w->DestroyWindow(); delete w; w=NULL;} }

#define WM_ACCOUNT			(WM_USER+10000)
#define WM_JANGO_CHANGE		(WM_USER+10001)			//�ɼ��� �ܰ����� �ֽ��� �ܰ����� �˾ƾ���

#define S_NOTIFY			(0x00000000)	//�ֽ�
#define F_NOTIFY			(0x00000001)	//����

#define WP_F_JNGO_CHANGE		(0x00000002)		//�����ɼ�
#define WP_S_JNGO_CHANGE		(0x00000003)		//�ֽ�

#define PI		3.14159265358979 
#define CALL	2
#define PUT		3
#define THETA_VAL	0.002739726

#define GAMMA		0.2316419
#define CNORMAL_1	0.31938153
#define CNORMAL_2	-0.356563782
#define CNORMAL_3	1.781477937
#define CNORMAL_4	-1.821255978
#define CNORMAL_5	1.330274429

typedef struct {
	char	code[8];	// �����ڵ�
	double	remainDays;	// �����ϼ�
	double	strike;		// ��簡
	double	cd;		// cd�ݸ�
	double	baedangJisu;	// �������
	double	currAmt;	// ���簡
	double	underlying;	// �����ڻ�(Kospi200)
	double	volatility;	// ������������
} OPTION, *POPTION;

typedef enum { 
	eNone=0,	// dummy
	eTR1001,	// �ֽ� �ü���ȸ
	eTR1002,	// �ֽ� �ɺ���ȸ(OOP)
	eTR1003,	// �ֽ� �к���ȸ(OOP)
	eTR1201,	// �ֽ� �ֹ�
	eTR1201_01, // �ֽ� �ֹ�(�ð���,�����ð��� ���� �ֹ�����)
	eTR1211,	// �ֽ� ü����ȸ
	eTR1221,	// �ֽ� �ܰ���ȸ
	eTR1222,	// �ֽ� �ܰ���ȸ(�ǽð�)
	eTR1231,	// �ֽ� �ֹ����ɼ�����ȸ
	eTR3001,	// �����ɼ� �ü���ȸ
	eTR3002,	// �����ɼ� �ɺ���ȸ(OOP)
	eTR3003,	// �����ɼ� �к���ȸ(OOP)
	eTR3201,	// �����ɼ� �ֹ�
	eTR3211,	// �����ɼ� ü����ȸ
	eTR3221,	// �����ɼ� �ܰ���ȸ
	eTR3222,	// �����ɼ� �ܰ���ȸ(�ǽð�)
	eTR3231,	// �����ɼ� �ֹ����ɼ�����ȸ
	eTR3232,	// �����ɼ� ���ϼ�����ȸ
	eTR3411,	// �ɼ� ������ ������ �ü�
	eTR1801,	// ������ - �������� �����ü�
	eTR1802,	// ������ - �������� ���ɽü�
	eTR1803,	// ������ - �������� �����Ȳ
	eTR1804,	// ������ - �������� 
	eTR4223,	// �����ɼ� - �����ɼ� �߰����űݹ߻���Ȳ
	eTR4224,	// �����ɼ� - �����ɼ� �������Ͽ������ű���ȸ2
	eTR4303,	// �����ɼ� - �����ɼ� �Ϻ� ���¼��ͳ���
	eTR4201,	// �����ɼ� - �����ɼ� �����ܰ���ȸ
	eTR4101,	// ���º�й�ȣ����
	eTR4221,	// �����ɼ� - �����ɼ� ���¿�Ź�����ű���ȸ
	eTR4401,	// �����ɼ� - �����ɼ� ���� �ŷ�����
	eTR4202,	// �����ɼ� - �����ɼ� �����ܰ� �� ����Ȳ2
	eTR4003,	// �����ɼ� - �����ɼ� ����û���ֹ�ü�Ό�����ű�
	eTR4004,	// �����ɼ� - �����ɼ� ���������ֹ�
	eTR4403,	// �����ɼ� - �����ɼ� ���º� ��ü�����׼����� ����
	eTR4501,	// ������ü���(����->����)
	eTR4502,	// ���°��ڱݴ�ü(���� -> ����)
	eTR4503,	// ������ü���(���� -> ����)
	eTR4511,	// ���º�������ü�ŷ�����
	eTR4512,	// ���´�ü�ŷ�����
	eTR4521,	// ���ฮ��Ʈ��ȸ
	eTR4522,	// ������¹�ȣ��ȸ
	eTR4523,	// HTS�α��ΰ��¸��
	eTR4524,	// �����⺻������ȸ
	eTR4525,	// ��ݰ��ɱݾ� �� �Ǹ��ȣ��ȸ
	eTR4526,	// ���� ������Ȳ
	eTR4527,	// ���¸���ȸ
	eTR8001 = 100,	// ����� ��ȸ
	eTR2001	// ������ü�Ϻ�
} TR_KEY;

//2013.04.01 KSJ ����ȣ������ ��û�� ���ؼ� ��ȸ 4 -> 6���� ����
//2015.12.14 KSJ �������϶����� ��ȸ�Ǽ� 6 -> 1�� ����
struct LIMIT_ITEM
{
	TR_KEY	trkey;
	DWORD	check;	// üũ�����ð� (ms)
	DWORD	stick;	// üũ���� Tick
	DWORD	limit;	// ����Ƚ��
	DWORD	count;	// ����Ƚ��
} limit[] = {
	{ eTR1001, 1000, 0,  1, 0},
	{ eTR1002, 1000, 0,  1, 0},
	{ eTR1002, 1000, 0,  1, 0},
	{ eTR1201, 1000, 0, 10, 0},
	{ eTR1201_01, 1000, 0, 10, 0},
	{ eTR1211, 1000, 0,  1, 0},
	{ eTR1221, 1000, 0,  1, 0},	//2015.12.09 KSJ �ܰ���ȸ xscale������ ���� 6 -> 1
	{ eTR1222, 1000, 0,  1, 0},
	{ eTR1231, 1000, 0,  1, 0},
	{ eTR3001, 1000, 0,  1, 0},
	{ eTR3002, 1000, 0,  1, 0},
	{ eTR3002, 1000, 0,  1, 0},
	{ eTR3201, 1000, 0, 10, 0},
	{ eTR3211, 1000, 0,  1, 0},
	{ eTR3221, 1000, 0,  1, 0},
	{ eTR3222, 1000, 0,  1, 0},
	{ eTR3231, 1000, 0,  1, 0},
	{ eTR3232, 1000, 0,  1, 0},
	{ eTR3411, 1000, 0,  1, 0},
	{ eTR1801, 1000, 0,  1, 0},
	{ eTR1802, 1000, 0,  1, 0}, 
	{ eTR1803, 1000, 0,  1, 0}, 
	{ eTR1804, 1000, 0,  1, 0}, 
	{ eTR2001, 1000, 0,  1, 0}, 
	{ eTR4223, 1000, 0,  1, 0}, 
	{ eTR4224, 1000, 0,  1, 0}, 
	{ eTR4303, 1000, 0,  1, 0}, 
	{ eTR4201, 1000, 0,  1, 0}, 
	{ eTR4101, 1000, 0,  1, 0}, 
	{ eTR4221, 1000, 0,  1, 0}, 
	{ eTR4401, 1000, 0,  1, 0}, 
	{ eTR4202, 1000, 0,  1, 0}, 
	{ eTR4003, 1000, 0,  1, 0}, 
	{ eTR4004, 1000, 0,  1, 0}, 
	{ eTR4403, 1000, 0,  1, 0}, 
	{ eTR4501, 1000, 0,  1, 0}, 
	{ eTR4502, 1000, 0,  1, 0}, 
	{ eTR4503, 1000, 0,  1, 0}, 
	{ eTR4511, 1000, 0,  1, 0}, 
	{ eTR4512, 1000, 0,  1, 0}, 
	{ eTR4521, 1000, 0,  1, 0}, 
	{ eTR4522, 1000, 0,  1, 0}, 
	{ eTR4523, 1000, 0,  1, 0}, 
	{ eTR4526, 1000, 0,  1, 0}, 
	{ eTR4525, 1000, 0,  1, 0}, 
	{ eTR4526, 1000, 0,  1, 0}, 
	{ eTR4527, 1000, 0,  1, 0}, 
	{ eTR8001, 1000, 0,  1, 0}, 
	{ eNone,      0, 0,  0, 0}
};

// ��밡���� ȣ������ ����
int pibosodr_hogb[] = { 0, 3, 5, 6, 7, 10, 13, 16, 20, 23, 26, 81, 61 };
int pibofodr_hogb[] = { 0, 3, 5, 6, 10, 20 };

class CIBKSConnectorCtrl : public COleControl
{
	typedef void (CIBKSConnectorCtrl::*TR_CALLBACK)(WPARAM wParam, LPARAM lParam);

	DECLARE_DYNCREATE(CIBKSConnectorCtrl)

public:
	CIBKSConnectorCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIBKSConnectorCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CIBKSConnectorCtrl();

	DECLARE_OLECREATE_EX(CIBKSConnectorCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CIBKSConnectorCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CIBKSConnectorCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CIBKSConnectorCtrl)		// Type name and misc status
	DECLARE_EVENTSINK_MAP()
// Message maps
	//{{AFX_MSG(CIBKSConnectorCtrl)
	//}}AFX_MSG
	afx_msg long OnUser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CIBKSConnectorCtrl)
	afx_msg BOOL Initialize();
	afx_msg void Uninitialize();
	afx_msg BOOL Login(LPCTSTR user_id, LPCTSTR user_pw, LPCTSTR cert_pw, LPCTSTR svr_ip, long svr_port);
	afx_msg BSTR GetLastErrMsg();
	afx_msg BSTR GetUserName();
	afx_msg BSTR GetHome();
	afx_msg BSTR GetUserID();
	afx_msg void Logout();
	afx_msg BOOL TR1001(long key, LPCTSTR code);
	afx_msg BOOL TR1201(long key, long mmgb, LPCTSTR acno, LPCTSTR pswd, long ojno, LPCTSTR code, long jqty, long jprc, long hogb, long mdgb);
	afx_msg BOOL TR1211(long key, LPCTSTR acno, LPCTSTR pswd, LPCTSTR code, long dsgb, long sygb, long dlgb, long sort, LPCTSTR nkey);
	afx_msg BOOL TR1221(long key, LPCTSTR acno, LPCTSTR pswd, long allf, LPCTSTR nkey);
	afx_msg BOOL TR1231(long key, LPCTSTR acno, LPCTSTR pswd, LPCTSTR code, long mmgb, double jprc);
	afx_msg BOOL TR3001(long key, LPCTSTR code);
	afx_msg BOOL TR3201(long key, long mmgb, LPCTSTR acno, LPCTSTR pswd, long ojno, LPCTSTR code, long jqty, long jprc, long hogb, long mdgb);
	afx_msg BOOL TR3211(long key, LPCTSTR acno, LPCTSTR pswd, long dlgb, long sort, LPCTSTR code, long cpgb, long dsgb, LPCTSTR nkey);
	afx_msg BOOL TR3221(long key, LPCTSTR acno, LPCTSTR pswd, LPCTSTR nkey);
	afx_msg BOOL TR3231(long key, LPCTSTR acno, LPCTSTR pswd, LPCTSTR code, long mmgb, double jprc, long hogb, long qtpy, double jamt, double jrat);
	afx_msg BOOL TR1801(long key, LPCTSTR code);
	afx_msg BOOL TR1802(long key, LPCTSTR code);
	afx_msg BOOL TR1803(long key, LPCTSTR code);
	afx_msg BOOL TR1804(long key, LPCTSTR jidx);
	afx_msg BSTR GetAccounts();
	afx_msg BOOL TR1002(long key, LPCTSTR code, LPCTSTR symb);
	afx_msg BOOL TR1003(long key, LPCTSTR code, long type, LPCTSTR symb, LPCTSTR nkey);
	afx_msg BOOL TR3002(long key, LPCTSTR code, LPCTSTR symb);
	afx_msg BOOL TR3003(long key, LPCTSTR code, long type, LPCTSTR symb, LPCTSTR nkey);
	afx_msg BOOL GetCode(long key, LPCTSTR reqtype);
	afx_msg BOOL TR1202(long key, long mmgb, LPCTSTR acno, LPCTSTR pswd, long ojno, LPCTSTR code, long jqty, long jprc, long hogb, long mdgb, long id);
	afx_msg BOOL TR3202(long key, long mmgb, LPCTSTR acno, LPCTSTR pswd, long ojno, LPCTSTR code, long jqty, long jprc, long hogb, long mdgb, long id);
	afx_msg BOOL TR4224(long key, LPCTSTR acno, LPCTSTR pswd, double zKpi200Idx);
	afx_msg BOOL TR4223(long key, LPCTSTR acno, LPCTSTR pswd, LPCTSTR zSrtDt, LPCTSTR zEndDt, LPCTSTR nkey);
	afx_msg BOOL TR4303(long key, LPCTSTR acno, LPCTSTR pswd, LPCTSTR zQrySrtDt, LPCTSTR zQryEndDt, long zQryTp, long zStnlnSeqTp, long zBalEvalTp, LPCTSTR nkey);
	afx_msg BOOL TR4201(long key, LPCTSTR acno, LPCTSTR pswd, long zFnoClssCode, LPCTSTR nkey);
	afx_msg BOOL TR4101(long key, LPCTSTR acno, LPCTSTR pswd);
	afx_msg BOOL TR4221(long key, LPCTSTR acno, LPCTSTR pswd);
	afx_msg BOOL TR4401(long key, LPCTSTR acno, LPCTSTR pswd, long zQryTp, LPCTSTR zQrySrtDt, LPCTSTR zQryEndDt, LPCTSTR zIsuNo, LPCTSTR nkey);
	afx_msg BOOL TR4202(long key, LPCTSTR acno, LPCTSTR pswd, long zBalEvalTp, long zPrcEvalTp, LPCTSTR nkey);
	afx_msg BOOL TR4003(long key, LPCTSTR acno, LPCTSTR pswd, long zMprcOrdTp, LPCTSTR zRoopData);
	afx_msg BOOL TR4004(long key, LPCTSTR acno, LPCTSTR pswd, LPCTSTR zRoopData);
	afx_msg void SetPTS();
	afx_msg BOOL TR4501(long key, LPCTSTR zMnyoutAcntno, LPCTSTR zMnyoutPswd, long zlMnyoutAmt, LPCTSTR zMnyinBnkCode, LPCTSTR zMnyinBnkAcntNo, LPCTSTR zAskpsnNm, LPCTSTR zTelExno, LPCTSTR zTelRgno, LPCTSTR zTelSeqno);
	afx_msg BOOL TR4502(long key, LPCTSTR acno, LPCTSTR pswd, long lBkeepAmt, LPCTSTR zMnyinAcntNo);
	afx_msg BOOL TR4503(long key, LPCTSTR zMnyinAcntNo, LPCTSTR zBnkCode, LPCTSTR zBnkAcntNo, LPCTSTR zBnkPwd, long zMnyinAmt);
	afx_msg BOOL TR4511(long key, LPCTSTR acno, LPCTSTR pswd, LPCTSTR zQrySrtDt, LPCTSTR zQryEndDt, LPCTSTR nkey);
	afx_msg BOOL TR4512(long key, LPCTSTR acno, LPCTSTR pswd, LPCTSTR zQrySrtDt, LPCTSTR zQryEndDt, LPCTSTR nkey);
	afx_msg BOOL TR4521(long key);
	afx_msg BOOL TR4522(long key, LPCTSTR acno, LPCTSTR pswd, LPCTSTR zBankCode);
	afx_msg BOOL TR4523(long key, LPCTSTR nkey);
	afx_msg BOOL TR4524(long key);
	afx_msg BOOL TR4525(long key, LPCTSTR zMnyoutAcntno);
	afx_msg BOOL TR4526(long key, LPCTSTR acno, LPCTSTR pswd, LPCTSTR nkey);
	afx_msg BOOL TR4527(long key, LPCTSTR acno);
	afx_msg void SaveConfig(LPCTSTR sSendName, LPCTSTR sKey, LPCTSTR sValue);
	afx_msg BOOL TR8001(long key, LPCTSTR acno);
	afx_msg void GoURL(long nType);
	afx_msg BOOL TR1222(long key, LPCTSTR acno, LPCTSTR pswd, long allf);
	afx_msg BOOL TR3222(long key, LPCTSTR acno, LPCTSTR pswd);
	afx_msg BOOL TR4403(long key, LPCTSTR acno, LPCTSTR pswd, long zFnoClssCode, LPCTSTR zCmsnAmtAddupCode, LPCTSTR zStrDt, LPCTSTR zEndDt, LPCTSTR nkey);
	afx_msg BOOL TR3411(long key, LPCTSTR mymd);
	afx_msg BSTR GetGreeksData(LPCTSTR sData);
	afx_msg BOOL TR3232(long key, LPCTSTR acno, LPCTSTR pswd, long fstp, long ertp);
	afx_msg BOOL TR2001(long key, LPCTSTR upcd, long dtgb);
	afx_msg void SetPrograms(long pggb);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CIBKSConnectorCtrl)
	void FireOnLogin(BOOL bLogin, LPCTSTR msg)
		{FireEvent(eventidOnLogin,EVENT_PARAM(VTS_BOOL  VTS_BSTR), bLogin, msg);}
	void FireOnContract(LPCTSTR data)
		{FireEvent(eventidOnContract,EVENT_PARAM(VTS_BSTR), data);}
	void FireOnRealData(LPCTSTR data)
		{FireEvent(eventidOnRealData,EVENT_PARAM(VTS_BSTR), data);}
	void FireOnRecvData(long key, long dptr, long size, BOOL bNext, LPCTSTR nkey)
		{FireEvent(eventidOnRecvData,EVENT_PARAM(VTS_I4  VTS_I4  VTS_I4  VTS_BOOL  VTS_BSTR), key, dptr, size, bNext, nkey);}
	void FireOnError(LPCTSTR msg)
		{FireEvent(eventidOnError,EVENT_PARAM(VTS_BSTR), msg);}
	void FireOnClose()
		{FireEvent(eventidOnClose,EVENT_PARAM(VTS_NONE));}
	void FireOnAlert(long code, LPCTSTR msg)
		{FireEvent(eventidOnAlert,EVENT_PARAM(VTS_I4  VTS_BSTR), code, msg);}
	void FireOnRecvDataID(long key, long dptr, long size, BOOL bNext, LPCTSTR nkey, long id)
		{FireEvent(eventidOnRecvDataID,EVENT_PARAM(VTS_I4  VTS_I4  VTS_I4  VTS_BOOL  VTS_BSTR  VTS_I4), key, dptr, size, bNext, nkey, id);}
	void FireOnVerUpdate()
		{FireEvent(eventidOnVerUpdate,EVENT_PARAM(VTS_NONE));}
	void FireOnUpdateStart()
		{FireEvent(eventidOnUpdateStart,EVENT_PARAM(VTS_NONE));}
	void FireOnUpdating(LPCTSTR sData, long nProgress)
		{FireEvent(eventidOnUpdating,EVENT_PARAM(VTS_BSTR  VTS_I4), sData, nProgress);}
	void FireOnUpdateEnd()
		{FireEvent(eventidOnUpdateEnd,EVENT_PARAM(VTS_NONE));}
	void FireOnGuideMsg(long key, LPCTSTR sMsg)
		{FireEvent(eventidOnGuideMsg,EVENT_PARAM(VTS_I4  VTS_BSTR), key, sMsg);}
	void FireOnFBalance(LPCTSTR data)
		{FireEvent(eventidOnFBalance,EVENT_PARAM(VTS_BSTR), data);}
	void FireOnSBalance(LPCTSTR data)
		{FireEvent(eventidOnSBalance,EVENT_PARAM(VTS_BSTR), data);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CIBKSConnectorCtrl)
	dispidInitialize = 1L,
	dispidUninitialize = 2L,
	dispidLogin = 3L,
	dispidGetLastErrMsg = 4L,
	dispidGetUserName = 5L,
	dispidGetHome = 6L,
	dispidGetUserID = 7L,
	dispidLogout = 8L,
	dispidTR1001 = 9L,
	dispidTR1201 = 10L,
	dispidTR1211 = 11L,
	dispidTR1221 = 12L,
	dispidTR1231 = 13L,
	dispidTR3001 = 14L,
	dispidTR3201 = 15L,
	dispidTR3211 = 16L,
	dispidTR3221 = 17L,
	dispidTR3231 = 18L,
	dispidTR1801 = 19L,
	dispidTR1802 = 20L,
	dispidTR1803 = 21L,
	dispidTR1804 = 22L,
	dispidGetAccounts = 23L,
	dispidTR1002 = 24L,
	dispidTR1003 = 25L,
	dispidTR3002 = 26L,
	dispidTR3003 = 27L,
	dispidGetCode = 28L,
	dispidTR1202 = 29L,
	dispidTR3202 = 30L,
	dispidTR4224 = 31L,
	dispidTR4223 = 32L,
	dispidTR4303 = 33L,
	dispidTR4201 = 34L,
	dispidTR4101 = 35L,
	dispidTR4221 = 36L,
	dispidTR4401 = 37L,
	dispidTR4202 = 38L,
	dispidTR4003 = 39L,
	dispidTR4004 = 40L,
	dispidSetPTS = 41L,
	dispidTR4501 = 42L,
	dispidTR4502 = 43L,
	dispidTR4503 = 44L,
	dispidTR4511 = 45L,
	dispidTR4512 = 46L,
	dispidTR4521 = 47L,
	dispidTR4522 = 48L,
	dispidTR4523 = 49L,
	dispidTR4524 = 50L,
	dispidTR4525 = 51L,
	dispidTR4526 = 52L,
	dispidTR4527 = 53L,
	dispidSaveConfig = 54L,
	dispidTR8001 = 55L,
	dispidGoURL = 56L,
	dispidTR1222 = 57L,
	dispidTR3222 = 58L,
	dispidTR4403 = 59L,
	dispidTR3411 = 60L,
	dispidGetGreeksData = 61L,
	dispidTR3232 = 62L,
	dispidTR2001 = 63L,
	dispidSetPrograms = 64L,
	eventidOnLogin = 1L,
	eventidOnContract = 2L,
	eventidOnRealData = 3L,
	eventidOnRecvData = 4L,
	eventidOnError = 5L,
	eventidOnClose = 6L,
	eventidOnAlert = 7L,
	eventidOnRecvDataID = 8L,
	eventidOnVerUpdate = 9L,
	eventidOnUpdateStart = 10L,
	eventidOnUpdating = 11L,
	eventidOnUpdateEnd = 12L,
	eventidOnGuideMsg = 13L,
	eventidOnFBalance = 14L,
	eventidOnSBalance = 15L,
	//}}AFX_DISP_ID
	};

protected:
	CCriticalSection m_cs;
	bool m_bInit, m_bLogin;
	long m_svrport;
	CString m_sLastMsg, m_usid, m_uspw, m_certpw, m_svrip, m_cltip, m_maddr, m_usnm, m_home, m_fcltip, m_strAcno;
	
	set<int> m_shoga, m_fhoga;
	vector<CString> m_acno, m_acnm;
	map<int, CString> m_chk_acno;
	map<TR_KEY, struct LIMIT_ITEM> m_limit;
	map<int, TR_CALLBACK> m_callback;
	map<int, int> m_cbcount;
	map< int, vector<char> > m_codes;
	map< int, CString> m_cname;
	CWnd m_dummyWnd;
	CWnd m_jngoWnd;
	CAxisWizard m_Wizard;	
	CWnd m_Notify;

	int m_nPggb;	//����/������ ����
	HINSTANCE m_hiSha256;	//2014.04.21 KSJ �Ϲ����ȣȭ �߰�

	//�б��ϴ� ���
	//����� �����ȸ, ����� ���а�, ����ó��
	bool m_bPTS;	//2012.08.08 PTS���� üũ�ؼ� �б� KSJ
	CString m_strService;	//IBKSConnector.INI�� PIBOPBXB�� 0�̸� pibfPBxB 1�̸� piboPBxB
	bool m_bUpdate;			//2012.08.27 KSJ UpdateStart�� ȣ��Ǿ����� üũ�ؼ� UpdateEnd�� ȣ���Ѵ�.
	bool m_bSaveLog;		//2012.09.27 KSJ �α����� ������ ����
	void testSaveFile(int tr, LPCSTR datB, int size);
	void testSaveFile(LPCTSTR gubn, LPCSTR datB, int size);
	CString CalcGreeks(CString sData);	//2013.04.04 KSJ �ɼ� �׸��� ���
	double	GetOptionTheoryAmt(POPTION in);
	double	CNormal(double value);
	double	GetND(double value);
	int	CallPut(LPSTR code);

protected:
	void DebugFile(LPCSTR path, LPCSTR mode, LPCSTR data, int dlen);
	inline bool IsInit();										// �ʱ�ȭ ���� Ȯ��
	inline bool IsLogin();										// �α��� ���� Ȯ��
	inline bool IsPTS();										// PTS ���� Ȯ�� KSJ
	inline bool IsValidAccount(LPCSTR acno);					// �������� ���� Ȯ��
	inline bool IsValidSendAccount(LPCSTR acno);				// ��ü���°� ���ǿ� �´��� Ȯ��
	inline bool IsValidAcnoPswd(LPCSTR pswd);					// ���º�й�ȣ ����
	inline bool IsValidState();									// �ʱ�ȭ/�α��ο��� ���� Ȯ��
	inline bool RegCallBack(int key, TR_CALLBACK cb);			// TR�� Unique Key �Ҵ�
	inline void UnregCallBack(int key);							// �Ҵ�� TR Key ����
	inline bool AccessTr(TR_KEY key);							// TR ȣ�����ѿ��� Ȯ��
	inline bool IsValidPTS();									// 2012.09.21 KSJ PTS����ڸ� ȣ���Ҽ��ִ� TR ����
	void CheckValidUser();										// IBKSConnector ����㰡 �������� Ȯ��
	inline int  atoi2(LPCSTR data, int size);					// atoi �ڸ������� ����
	CString ToKoscomIP(CString addr);
	void LoadIni();												// INI���� �б� KSJ
	BOOL IsNumber(CString str);									// ������ ���� �ƴ����� �Ǵ��ϰ��� �ϴ� �Լ�
	CString GetAuthParam();										//2012.09.17 KSJ ��üURL�ڿ� �ٴ� PARAM
	CString Parser(CString &srcstr, CString substr = "\t");

	void InitJanGo();											// 2012.09.13 KSJ �ܰ� ��Ʈ�� ����
	CString URLEncode(const char* lpszURL);						// 2012.09.13 KSJ
	CString HTSEncode(const char* lpszSource, const char* key);	// 2012.09.13 KSJ
	CString GetEncPassword(CString sPswd);						// 2014.04.21 KSJ �Ϲ����ȣȭ ���

	// AxisWizard Event Handler
	afx_msg int OnAxisFire(int type, WPARAM wParam, LPARAM lParam);

	// AxisEvent Callback Functions
	int OnFevOpen(WPARAM wParam, LPARAM lParam);
	int OnFevAxis(WPARAM wParam, LPARAM lParam);
	int OnFevFmx (WPARAM wParam, LPARAM lParam);
	int OnFevAnm (WPARAM wParam, LPARAM lParam);

	// Transaction Functions
	void GetLedger(void *param);
	void* Variant(int cmd, LPARAM lParam);
	int SendTR(LPCSTR trnm, int tkey, int stat, LPCSTR data, int size, TR_CALLBACK cb);
	char* GetFileSHA256( CString sPath, HMODULE hModule);
	char* GetSHA256(char* data, int dataL,HMODULE hModule,bool isfile);

protected:
	void OnSetClientSite();

public:
	// default callback function
	void DEF_CALLBACK(WPARAM wParam, LPARAM lParam);
	void DEF_KOSCOM_CALLBACK(WPARAM wParam, LPARAM lParam);

	// pidouini - IBKSConnector ����㰡 �������� Ȯ��.
	BOOL S_PIDOUINI();
	void C_PIDOUINI(WPARAM wParam, LPARAM lParam);
	void C_PIDOVERS(WPARAM wParam, LPARAM lParam);

	// C_SACAQ504 - �ý��� Ʈ���̵� ���� Ȯ��
	void C_SACAQ504(WPARAM wParam, LPARAM lParam);
	void C_SACEQ320(WPARAM wParam, LPARAM lParam);

	// �ֽ�
	BOOL S_PIBO1003(int key, LPCSTR jcod);	// �ü���ȸ
	BOOL S_PIBOSODR(int key, int mmgb, LPCSTR acno, LPCSTR pswd, int ojno, LPCSTR fcod, int jqty, int jprc, int hogb, int mdgb ); // �ֹ�
	BOOL S_PIBOSODR_ID(int key, int mmgb, LPCSTR acno, LPCSTR pswd, int ojno, LPCSTR fcod, int jqty, int jprc, int hogb, int mdgb, int id ); // 2012.06.04 KSJ �ֹ� ���������ID �߰�
	BOOL S_PIBOSCHG(int key, LPCSTR acno, LPCSTR pswd, LPCSTR code, int dsgb, int sygb, int dlgb, int sort, LPCSTR nkey); // �ֽ�ü����ȸ
	BOOL S_PIBOSJGO(int key, LPCSTR acno, LPCSTR pswd, int allf, LPCSTR nkey); // �ֽ��ܰ���ȸ
	BOOL S_SONAQ200(int key, LPCSTR acno, LPCSTR pswd, LPCSTR code, int zBnsTp, double dOrdPrc); // �ֹ����ɼ�����ȸ

	// �����ɼ�
	BOOL S_PIBO3002(int key, LPCSTR fcod);	// �ü���ȸ
	BOOL S_PIBOFODR(int key, int mmgb, LPCSTR acno, LPCSTR pswd, int ojno, LPCSTR fcod, int jqty, int jprc, int hogb, int mdgb ); // �ֹ�
	BOOL S_PIBOFODR_ID(int key, int mmgb, LPCSTR acno, LPCSTR pswd, int ojno, LPCSTR fcod, int jqty, int jprc, int hogb, int mdgb, int id ); // 2012.06.04 KSJ �ֹ� ���������ID �߰�
	BOOL S_PIBOFCHG(int key, LPCSTR acno, LPCSTR pswd, int dlgb, int sort, LPCSTR code, int cpgb, int dsgb, LPCSTR nkey); // ü�᳻����ȸ
	BOOL S_PIBOFJGO(int key, LPCSTR acno, LPCSTR pswd, LPCSTR nkey); // �ܰ�����ȸ
	BOOL S_SONBQ504(int key, LPCSTR acno, LPCSTR pswd, int zBalEvalTp, int zPrcEvalTp, LPCSTR nkey); // ���������ܰ�����ȸ
	BOOL S_SONBQ101(int key, LPCSTR acno, LPCSTR pswd, LPCSTR code, int zBnsTp, double dOrdPrc, int hogb, int zQryTp, double lOrdAmt, double dRatVal);
	BOOL S_PIBO4013(int key, LPCSTR mymd);	//�ɼ� ������ ������ �ü� ��ȸ
	BOOL S_SSLBQ033(int key, LPCTSTR acno, LPCTSTR pswd, int fstp, int ertp); //�ɼ� ���ϼ��� ��ȸ KSJ 2013.04.08

	//PTS�߰� //KSJ
	BOOL S_SONBQ123(int key, LPCSTR acno, LPCSTR pswd, double zKpi200Idx); // �����ɼ� �������Ͽ������ű���ȸ2
	BOOL S_SONDQ401(int key, LPCSTR acno, LPCSTR pswd, LPCSTR zSrtDt, LPCSTR zEndDt, LPCSTR nkey); // �����ɼ� �߰����űݹ߻���Ȳ
	BOOL S_SSLAQ826(int key, LPCSTR acno, LPCSTR pswd, LPCSTR zQrySrtDt, LPCSTR zQryEndDt, int zQryTp, int zStnlnSeqTp, int zBalEvalTp, LPCSTR nkey); // �����ɼ� �Ϻ� ���¼��ͳ���
	BOOL S_SONBQ740(int key, LPCSTR acno, LPCSTR pswd, int zFnoClssCode, LPCTSTR nkey); // �����ɼ� �����ܰ���ȸ
	BOOL S_SACMT238(int key, LPCSTR acno, LPCSTR pswd); // ���º�й�ȣ ����
	BOOL S_SONBQ105(int key, LPCSTR acno, LPCSTR pswd); // �����ɼ� ���¿�Ź�����ű���ȸ
	BOOL S_SDPBQ001(int key, LPCSTR acno, LPCSTR pswd, int zQryTp, LPCSTR zQrySrtDt, LPCSTR zQryEndDt, LPCSTR zIsuNo, LPCSTR nkey); // �����ɼ� ���� �ŷ�����
	BOOL S_SONBQ114(int key, LPCSTR acno, LPCSTR pswd, int zMprcOrdTp, LPCSTR zRoopData); // �����ɼ� ����û���ֹ�ü�Ό�����ű�
	BOOL S_SONBT709(int key, LPCSTR acno, LPCSTR pswd, LPCSTR zRoopData); // �����ɼ� ���������ֹ�
	BOOL S_SONDQ304(int key, LPCSTR acno, LPCSTR pswd, int zFnoClssCode, LPCSTR zCmsnAmtAddupCode, LPCSTR zSrtDt, LPCSTR zEndDt, LPCSTR nkey); // �����ɼ� ���º� ��ü�����׼����� ����

	// ��ݰ���, ��ݰ��º�й�ȣ, ��ݱݾ�, �Ա������ڵ�, �Աݰ��¹�ȣ, �Ƿ��θ�
	BOOL S_SDPKT360(int key, LPCSTR zMnyoutAcntno, LPCSTR MnyoutPswd, int zlMnyoutAmt, LPCSTR zMnyinBnkCode, LPCSTR zMnyinBnkAcntNo, LPCSTR zAskpsnNm, LPCSTR zTelExno, LPCSTR zTelRgno, LPCSTR zTelSeqno); // ������ü���(����->����)
	BOOL S_SDPDT901(int key, LPCSTR acno, LPCSTR pswd, int lBkeepAmt, LPCSTR zMnyinAcntNo); // ���°��ڱݴ�ü(���� -> ����)
	BOOL S_SDPKT300(int key, LPCSTR zMnyinAcntNo, LPCSTR zBnkCode, LPCSTR zBnkAcntNo, LPCSTR zBnkPwd, int zMnyinAmt); // ������ü���(���� -> ����)
	BOOL S_SDPNQ002(int key, LPCSTR acno, LPCSTR pswd, LPCSTR zQrySrtDt, LPCSTR zQryEndDt, LPCSTR nkey); // ���º�������ü�ŷ�����
	BOOL S_SDPBQ905(int key, LPCSTR acno, LPCSTR pswd, LPCSTR zQrySrtDt, LPCSTR zQryEndDt, LPCSTR nkey); // ���´�ü�ŷ�����
	BOOL S_SDPNQ060(int key); // ���ฮ��Ʈ��ȸ
	BOOL S_SDPNQ031(int key, LPCSTR acno, LPCSTR pswd, LPCSTR zBankCode); // ������¹�ȣ��ȸ
	BOOL S_SACMQ913(int key, LPCSTR nkey); // HTS�α��ΰ��¸��
	BOOL S_SBPGT339(int key); // �����⺻������ȸ
	BOOL S_SDPNQ180(int key, LPCSTR zMnyoutAcntno); // ��ݰ��ɱݾ� �� �Ǹ��ȣ��ȸ
	BOOL S_SACAQ519(int key, LPCSTR acno, LPCSTR pswd, LPCSTR nkey); // ���� ������Ȳ
	BOOL S_SACMQ229(int key, LPCSTR acno); // ���¸���ȸ
	BOOL S_PIHOCUST(int key, LPCSTR anco); // ����� ��ȸ
	BOOL S_SBPGT315(int key); // ����� ��ȸ
	
	//PTS �߰� End

	// ������
	BOOL S_PIBO5101(LONG key, LPCSTR code);
	BOOL S_PIBO5102(LONG key, LPCSTR code);
	BOOL S_PIBO5103(LONG key, LPCSTR code);
	BOOL S_PIBO5104(LONG key, LPCSTR code);

	// Master files
	BOOL S_PIBOTRAN(LONG key, LPCSTR reqtype, int seqn=1, int pos=0);
	void C_PIBOTRAN(WPARAM wParam, LPARAM lParam);
	
	// POOPPOOP --------------------------------------------------------------------------
	// �ֽ� ������ȸ
	BOOL S_TR1002(int key, LPCSTR code, LPCSTR data);
	void C_TR1002(WPARAM wParam, LPARAM lParam);
	// �ֽ� �к���ȸ
	BOOL S_TR1003(int key, LPCSTR code, int type, LPCSTR columns, LPCSTR nkey);
	void C_TR1003(WPARAM wParam, LPARAM lParam);
	// ���� ������ȸ
	BOOL S_TR3002(int key, LPCSTR code, LPCSTR data);
	void C_TR3002(WPARAM wParam, LPARAM lParam);
	// ���� �к���ȸ
	BOOL S_TR3003(int key, LPCSTR code, int type, LPCSTR columns, LPCSTR nkey);
	void C_TR3003(WPARAM wParam, LPARAM lParam);
	
	// �����ں� ���ں� �Ÿŵ���	//2014.02.03 KSJ
	BOOL S_TR2001(int key, LPCTSTR upcd, int dtgb);
	void C_TR2001(WPARAM wParam, LPARAM lParam);

	// ����üũ
	BOOL S_PIDOVERS();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IBKSCONNECTORCTL_H__A5FEE1A3_98BC_4E96_B29C_2C7954439131__INCLUDED)
