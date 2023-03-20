#pragma once

#include "Remain.h"
#include "RemainFuture.h"
#include "../../h/ledger.h"
#include "../../h/Grid.h"
#include <afxmt.h>
#include "ShMemory.h"

//==============================================================================
class CAccn
{
public:
  bool      m_bFuture;
  CString   m_accn;
  CString   m_pswd;
  CString   m_errmsg;
  bool      m_bCredit;        // true:�ſ�����, false:�ſ�����
  int       m_dFee;           // ������������ ���� 1 ������ 0
  double    m_dMass;          // ��ü���� 0:hts 1:������/�� 2.ARS
  double    m_dSave;          // ��ü���� 0:hts 1:������/�� 2.ARS
  int       m_dCalcType;      // 0:���ڱ�����, 1:���ڱ�����
  int		m_iMcgb;		  // 2015.04.22 KSJ ��ü���� 0:HTS/MTS/Ȩ������, 1:������/�ݼ���, 2:ARS
  bool		m_bReceive;			//2015.03.04 KSJ true:Recevie����, false:Recevie���� ����
  CMapStringToPtr m_CodeMap;  // Ptr: F->class CRemainFuture, C->class CRemain

public:
  CAccn()
  {
    m_bFuture = false;
    m_bCredit = false;
    m_accn = "";
    m_pswd = m_errmsg = "";
    m_dFee = 0; //������������ ���� 1 ������ 0
    m_dMass = 0;
    m_dSave = 0;
	m_dCalcType = 0;
	m_iMcgb = 0;

    m_CodeMap.RemoveAll();
  }
};
//==============================================================================
class CMapWnd : public CWnd
{
public:
  CMapWnd();
  virtual ~CMapWnd();

  //{{AFX_VIRTUAL(CMapWnd)
  //}}AFX_VIRTUAL

protected:
  //{{AFX_MSG(CMapWnd)
  afx_msg void OnPaint();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnDestroy();
  //}}AFX_MSG
  afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnRequestOfCtrl(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnRequestLawCheg(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnCloseOfCtrl(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnReceiveRemainData(WPARAM wParam, LPARAM lParam);
  DECLARE_MESSAGE_MAP()

public:
	BOOL  CreateMap(CWnd *pParent);

private:
	void  sendRemainTR(CString sAccn, CString sPswd, bool bFuture, bool bCredit, int dFee, double dMass ,double dSave, int dCalcType, int iMcgb,HWND sHwnd);	//2015.04.22 KSJ ��ü���� �߰�
	void  sendLawCheg(CString acc_gb, CString mng_no, CString mk_cd, CString grp_no, HWND sHwnd);
	CString parsingRemainData(char* pData, int len, CString sErrmsg, int trkey);
	void  sendToControl(CString sAccn);
	void  sendToControl(CString sAccn, CAccn *pAccn);
	void  attachCB(HWND hWnd, CString accn);
	void  detachCB(CString accn, CString sData, CString emsg = _T(""));

	void  sendNContTR(CString sLedger = "");
	void  parsingNContData(char* pData);

	void  parsingRTS(CString sRtsData);
	void  parsingRTSx(struct _alertR* alertR);
	CString parsingNotice(CString str);

	BOOL  getRemainFee();
	CString getKey(bool bFuture, CString codeS, CString dateS, CString sygbS, CString sJggb, int trkey=-1);

	CString ledgerTR(CString sTran, CString sSvcn, CString sGubn);
	int openView(int type, CString data);

	CString parser(CString &srcstr, CString substr);

	CString getTime();
	CString variant(int comm, CString data);	
	void  removeAccn(CString accn);
	void  removeRemain(CAccn *pAccn);

public:
	CMapStringToPtr m_hname;
	BOOL LoadCode();
	bool Set3RDCode();					//2014.10.06 KSJ K-OTC ���� ����
	bool Is3RDCode(CString sCode);		//2014.10.06 KSJ K-OTC ���� ã��
	bool IsKONEXCode(CString sCode);	//KONEX ����ã��
	CPtrArray m_3RDcode;				//2014.10.06 KSJ K-OTC �����ڵ�
	CPtrArray m_KONEXcode;			    //KONEX
	CPtrArray m_hcode;

	CString GetRoot();
	CString GetUser();
	CString GetName();
	BOOL  sendTR(CString sTR, CString sData, int nKey, int nStat, CString Acnt);

	CString GetHandleByKey(CString key);
	int GetNewAccHandleKey();
	int GetNewLawHandleKey();
	CWnd  *m_pParent;
	CString m_slog;
private:
	std::unique_ptr<char []>m_hcB;

	void RemoveCB(CString accn);
	HWND GetRightHwnd(CString key);
	void SendLawNotice(CString sData);
	CString GetEncPassword(CString sPswd);	//2014.09.18 KSJ �Ϲ����ȣȭ

	HINSTANCE	m_hiSha256;	//2014.09.18 KSJ �Ϲ����ȣȭ

	int m_hcnt;
	std::unique_ptr<class CShMemory> m_ShMemory;
	CCriticalSection  m_sync;
	char      m_system{};   // ����, �ϳ� ����

	CMapStringToPtr   m_AccnMap;    // Ptr : class CAccn

	CMapStringToString  m_multicb;
	CMapStringToString  m_AccnKey;    // ���º� Ű����
	CMapStringToString  m_LawKey;   // ���ο��� Ű����
	CStringArray    m_arKey;

	CString     m_sCurrAccn;    // ��ȸ���� accn
	CString     m_sCurrPswd;    // ��ȸ���� pswd
	CString     m_sCurrKey;   // ��ȸ���� key
	bool      m_bAllAccn{};   // ����������ȸ

	std::vector<std::unique_ptr<CAccn>> _vList;
};

