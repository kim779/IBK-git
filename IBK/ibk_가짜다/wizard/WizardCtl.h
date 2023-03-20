#if !defined(AFX_WIZARDCTL_H__769257DA_90F8_46AF_B175_988EA23591CB__INCLUDED_)
#define AFX_WIZARDCTL_H__769257DA_90F8_46AF_B175_988EA23591CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// WizardCtl.h : Declaration of the CWizardCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CWizardCtrl : See WizardCtl.cpp for implementation.

class CWizardCtrl : public COleControl
{
	DECLARE_DYNCREATE(CWizardCtrl)

// Constructor
public:
	CWizardCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizardCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	class CGuard*	m_guard;
	enum	{ mtNO, mtCON, mtSIGN, mtCA, mtXEC, mtMENU, mtRUN }	m_mode;
	enum	{ xtWS, xtRM, xtXEC}	m_xtype;

private:
	int	m_axisL;
	char*	m_axisB;

private:
	void	OnAlert(char* pBytes, int nBytes);
	void	OnAlert(int type, char* pBytes, int nBytes);
	void	OnPush(char* pBytes, int nBytes);
	void	OnRead(char* pBytes, int nBytes);
	BOOL	OnAxis(int type, char* pBytes, int variant);

	void	OnAxis(struct _axisH* axisH, char *pBytes, int nBytes);
	void	OnStream(struct _axisH* axisH, char *pBytes, int nBytes);
	bool	OnAux(struct _axisH* axisH, char* &pBytes, int& nBytes);
	void	OnXecure(int encK, char *pBytes, int nBytes);
	void	OnSign(int signK, char *pBytes, int nBytes);
	void	OnCertify(char* pBytes, int nBytes);

	void	Xecure();
	int	Xecure(char* pBytes, int nBytes);
	void	Run();

protected:
	~CWizardCtrl();

	DECLARE_OLECREATE_EX(CWizardCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CWizardCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CWizardCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CWizardCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CWizardCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LONG OnFireAxis(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnGuide(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnVersion(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnFireAlert(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	afx_msg	void OnFireEvent(int type, char* pBytes, int nBytes);

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CWizardCtrl)
	afx_msg BOOL RunAxis(long mode, long pBytes, long nBytes);
	afx_msg BOOL axTR(long pBytes, long nBytes);
	afx_msg long axAttach(long view, long type, long key);
	afx_msg void axDetach(long key);
	afx_msg BOOL axWorkShop(long key, LPCTSTR maps, long size, BOOL only);
	afx_msg long axWorkName(long key);
	afx_msg long axWorkView(LPCTSTR maps, long size);
	afx_msg long axWizard(long kind, long variant);
	afx_msg long axInvoke(long wParam, long lParam);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CWizardCtrl)
	void OnFire(long type, long wParam, long lParam)
		{FireEvent(eventidOnFire,EVENT_PARAM(VTS_I4  VTS_I4  VTS_I4), type, wParam, lParam);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CWizardCtrl)
	dispidRunAxis = 1L,
	dispidAxTR = 2L,
	dispidAxAttach = 3L,
	dispidAxDetach = 4L,
	dispidAxWorkShop = 5L,
	dispidAxWorkName = 6L,
	dispidAxWorkView = 7L,
	dispidAxWizard = 8L,
	dispidAxInvoke = 9L,
	eventidOnFire = 1L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDCTL_H__769257DA_90F8_46AF_B175_988EA23591CB__INCLUDED)
