#if !defined(AFX_CERTIFYCTL_H__A29E1096_43BB_417C_9846_9C68A9C5BC10__INCLUDED_)
#define AFX_CERTIFYCTL_H__A29E1096_43BB_417C_9846_9C68A9C5BC10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CertifyCtl.h : Declaration of the CCertifyCtrl ActiveX Control class.

#include <afxmt.h>
#include "CaLib/interfaceDLL.h"

/////////////////////////////////////////////////////////////////////////////
// CCertifyCtrl : See CertifyCtl.cpp for implementation.

class CCertifyCtrl : public COleControl
{
	DECLARE_DYNCREATE(CCertifyCtrl)

// Constructor
public:
	CCertifyCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCertifyCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Attributes
private:
	CString		m_name;
	APP_CONTEXT	m_context;
	SD_API_CONTEXT_NEW m_contextNew;
	enum	{ caNO, caNOx, caOK, caRUN, caPWD, caPWDa, caOKx } m_ca;

	int		m_nBytes;
	char		m_pBytes[16*1024];
	CCriticalSection m_sync;

	enum	resNO { rspNULL = -1, rspMODAL = 0, rspOK = 1, rspISSUE = 2, rspREISSUE = 3, rspPASSWD = 4, rspOTHER = 5 };
	enum	msgNO { msg0 = 0, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9, msg10, msg11, msg12, msg13, msg14, msg15, msg16, msg17, msg18, msg19 };

	bool		m_auto;
	bool		m_calogon;
	CString		m_user;
	CString		m_certifys;
	CString		m_string;
	char		m_encpass[32+1];	// encrypted passwd

	CString		m_msg;
//	class	CPassInput*	m_passDlg;

	CMapStringToPtr	m_emaps;

// Implementation
private:
	bool	sign();
	int	queryDn(CString dn_name, int* nBytes, bool retry = false);
	void	savePasswd();
	BOOL	checkPasswd(CString pass);
	CString	checkPasswd();
	bool	certify(bool reissue = false);
	void	otherCertificate();
	void	removeCertificate();
	bool	guideMsg(msgNO msgno, CString guide = _T(""), CString title = _T(""));
	BOOL	isMustCertify(CString maps);

protected:
	~CCertifyCtrl();

	DECLARE_OLECREATE_EX(CCertifyCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CCertifyCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCertifyCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CCertifyCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CCertifyCtrl)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	long	OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CCertifyCtrl)
	afx_msg long OnCertify(long pBytes, long nBytes);
	afx_msg BOOL Certify(long pBytes, long nBytes, long infos);
	afx_msg BOOL CertifyErr(long pBytes, long nBytes);
	afx_msg void CertifyId(long pBytes);
	afx_msg BOOL CertifyEx(long pBytes, long nBytes);
	afx_msg long CertifyFull(long pInB, long pInL, long pOutB, long pOutL);
	afx_msg long CertifyName(long pBytes);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CCertifyCtrl)
	void OnFire(long type, long wParam, long lParam)
		{FireEvent(eventidOnFire,EVENT_PARAM(VTS_I4  VTS_I4  VTS_I4), type, lParam, wParam);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CCertifyCtrl)
	dispidOnCertify = 1L,
	dispidCertify = 2L,
	dispidCertifyErr = 3L,
	dispidCertifyId = 4L,
	dispidCertifyEx = 5L,
	dispidCertifyFull = 6L,
	dispidCertifyName = 7L,
	eventidOnFire = 1L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CERTIFYCTL_H__A29E1096_43BB_417C_9846_9C68A9C5BC10__INCLUDED)
