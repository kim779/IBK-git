#if !defined(AFX_GLOBALSHAREDDATA_H__DDDE14A1_C5F7_11D4_B319_0048543A0D2E__INCLUDED_)
#define AFX_GLOBALSHAREDDATA_H__DDDE14A1_C5F7_11D4_B319_0048543A0D2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GlobalSharedData.h : header file
//

// #include "../include/GlobalSharedData.h"
#include <AfxTempl.h>

class CGlobalSharedClient;
class CDeskSharedServer;
class CGlobalSharedManager;

/////////////////////////////////////////////////////////////////////////////
// CGlobalSharedClient

class CGlobalSharedClient
{
public:
	// CDeskSharedServer ���� ȣ���ϴ� �Լ�
	virtual BOOL ChangedData(LPCSTR _szName, LPCSTR _szValue)  { ASSERT(FALSE); return FALSE; }
};


/////////////////////////////////////////////////////////////////////////////
// CDeskSharedServer

// ��Ʈ�ѸŴ���(DESK)���� ������ �����޴���
class CDeskSharedServer
{
public:
	// CGlobalSharedClient ���� ȣ���ϴ� �Լ�
	virtual BOOL GetData(LPCSTR _szName, CString& szValue) { ASSERT(FALSE); return FALSE; }
	virtual BOOL SetData(LPCSTR _szName, LPCSTR _szValue, CGlobalSharedClient* _pCli)  { ASSERT(FALSE); return FALSE; }
	virtual BOOL Advise(LPCSTR _szName, CGlobalSharedClient* _pClient) { ASSERT(FALSE); return FALSE; }
	virtual BOOL UnAdvise(LPCSTR _szName, CGlobalSharedClient* _pClient) { ASSERT(FALSE); return FALSE; }

	// CGlobalSharedManager ���� ȣ���ϴ� �Լ�
	virtual BOOL ChangedData(LPCSTR _szName, LPCSTR _szValue)  { ASSERT(FALSE); return FALSE; }

	// * �߰� 2000. 12. 27 Junok Lee
	// * ���۷����� �ѱ�� ��Ʈ�ѿ��� ���������� ��û�� Release/Debug�� ���� �״�
	// * ������ �߻��Ͽ� LPCSTR�� �����ϴ� �Լ��� �߰���.
	virtual LPCSTR GetData2(LPCSTR _szName){ ASSERT(FALSE); return ""; }

	// * �߰� 2001. 03. 07 Junok Lee
	// * CDeskSharedServer�� �����͸� �Ѱ��ش�.
	virtual void SetDeskSharedServer(long* pList, int nListCnt) {}

};

/////////////////////////////////////////////////////////////////////////////
// CGlobalSharedManager window

class CGlobalSharedManager
{
public:
	virtual BOOL GetData(LPCSTR _szName, CString& szValue) { ASSERT(FALSE); return FALSE; }
	virtual BOOL SetData(LPCSTR _szName, LPCSTR _szValue, CDeskSharedServer* _pSvr)  { ASSERT(FALSE); return FALSE; }
	virtual BOOL Advise(CDeskSharedServer* _pServer) { ASSERT(FALSE); return FALSE; }
	virtual BOOL UnAdvise(CDeskSharedServer* _pServer) { ASSERT(FALSE); return FALSE; }

	virtual LPCSTR GetData2(LPCSTR _szName) { ASSERT(FALSE); return ""; }
};

class CListGlobalCliient
{
public:
	CListGlobalCliient() : m_pCli(NULL) {};
	CString m_szShareName;
	CGlobalSharedClient*	m_pCli;
};

class CListGlobalServer
{
public:
	CListGlobalServer() : m_pSvr(NULL) {};
	CString m_szShareName;
	CDeskSharedServer*	m_pSvr;
};

class CShareData
{
public:
	CString m_szKey, m_szValue;
	CShareData(LPCSTR _szKey, LPCSTR _szValue) { m_szKey=_szKey; m_szValue=_szValue; }
	CShareData(){}
};

typedef struct {
	CString	m_szSharedName;
	CString m_szData;
	BOOL	m_bStart;
	BOOL	m_bSet;
	BOOL	m_bGet;
} ST_GSHAREDFORMAT, *LPST_GSHAREDFORMAT;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLOBALSHAREDDATA_H__DDDE14A1_C5F7_11D4_B319_0048543A0D2E__INCLUDED_)
