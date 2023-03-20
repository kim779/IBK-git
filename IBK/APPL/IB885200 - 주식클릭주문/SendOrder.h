#pragma once
// SendOrder.h : header file
//

//#include "../../h/ledger_Hana.h"
//#include "../../h/ledger_Deatoo.h"
#include "../../h/ledger.h"

struct _sendData {
	char	Odgb[1];
	char	Mkgb[1];
	char	Mmgb[1];
	char	Acno[10];
	char	Pswd[8];
	char	Ogno[12];
	char	Code[12];
	char	Jqty[8];
	char	Jprc[10];
	char	Hogb[2];
	char	Jmgb[2];
	char	Mdgb[1];
	char	Prgb[1];
	char	Filler[30];
};

struct _orderResult {
	char	Jmno[6];
	char	Mono[6];
	char	Idat[80];
};

struct f_contmid {
	char	accn[8];
	char	cogb[2];
	char	pswd[8];
	char	gubn[1];
	char	juno[6];
	char	sort[1];
	char	date[8];
	char	cod2[8];
};

struct f_contgrid {
	char	mono[6];	/* ���ֹ���ȣ		*/
	char	juno[6];	/* �ֹ�  ��ȣ		*/
	char	ojno[6];	/* ���ֹ� ��ȣ		*/
	char	cod2[8];	/* �����ڵ�		*/
	char	hnam[30];	/* �����		*/
	char	odgb[8];	/* �ֹ�����		*/
	char	hogb[1];	/* �ֹ�����		*/
	char	jprc[11];	/* �ֹ�����		*/
	char	jqty[7];	/* �ֹ�����		*/
	char	dlgb[4];	/* ü�ᱸ��		*/
	char	dprc[11];	/* ü�ᰡ��		*/
	char	dqty[7];	/* ü�����		*/
	char	wqty[7];	/* ��ü�����		*/
	char	stat[8];	/* ��������		*/
};

struct	f_cont {
	char	nrec[4];
	struct	f_contgrid rec[1];
};

#define L_senddata	sizeof(struct _sendData)

#define L_fContmid	sizeof(struct f_contmid)
#define L_fCont		sizeof(struct f_cont) - sizeof(struct f_contgrid)
#define L_fContGrid	sizeof(struct f_contgrid) 

/////////////////////////////////////////////////////////////////////////////
// CSendOrder window

class CSendOrder : public CWnd
{
// Construction
public:
	CSendOrder();
	CWnd* m_pParent{};

// Attributes
public:
	CString OrderResult(char *pData);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendOrder)
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetErrmsg(LPARAM lParam);
	int	GetRecordCnt(LPARAM lParam);
	CString	GetNextkey(LPARAM lParam);
	long	GetLedgerSize();
	CString MakeFContData(CString sAccn, CString sPswd, CString sGubn, CString  sDate);
	CString LedgerTR(CString sTran, CString sGubn, CString sMaxRow = "999", CString sSvcn = "");
	CString MakeFSendData(CString sMmgb, CString sAccn, CString sPswd, CString sOgno, CString sCode, CString sJqty, CString sJprc, CString sHogb, CString sMdgb);
	virtual ~CSendOrder();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSendOrder)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

