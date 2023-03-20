#pragma once
// ControlWnd.h : header file
//
#include "IMAXApi.h"
#include "../../h/param.h"

/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd(CWnd *pParent, struct _param *pParam);

// Attributes
public:
	CWnd*		m_pMainWnd;
	CWnd*		m_pParent;
	CString		m_sHome;
	CString		m_sUsID;
	class CParam	m_param;

	CIMAXApi	m_IMAXApi;
	CString		m_sViewNo;
	CSize		m_sizeTot;

	BOOL		m_bLoinError;
	HANDLE		m_hView;
	int		m_nGroupNo;
	bool		m_bStaf;

	CString		m_sExid;
	CString		m_sCode;
	CString		m_sCodeName;
	CString		m_sBidUnit;
	CString		m_sAskUnit;
	CString		m_sSelPrice;
	CString		m_sBidPrice;
	CString		m_sAskPrice;
	CString		m_sCurrency;
	CString		m_sOrderQty;

	int		m_iSkin;    //1 gray,  2 blue,  4 brown

// Operations
public:
	void	Init();
	CString	Variant(int comm, CString data = "");
	void	SetParam(struct _param *pParam);
	void	parseIMAXMessageData(int nKind, CString sIMAXInfo);
	void	IMAXToMapTrigger(CString sValue);
	void	DllEvent(int nKind);

	void	LoginIMAX();
	bool	LoadIMAXView(CString ViewName, CRect rcCli);
	int	openView(int type, CString data);

// for OutputDebugString
	CString	m_tmpX;
	void	dprint();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

protected:

// Implementation
public:
	virtual ~CControlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long IMAXOnMessageOPEN(WPARAM wParam, LPARAM lParam);
	afx_msg long IMAXOnMessageCODEChange(WPARAM wParam, LPARAM lParam);
	afx_msg long IMAXOnMessageORDERINFO(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg void _SetIMAXGroup(short nGroup);
	afx_msg void _SetIMAXCIFId(LPCTSTR CIFId);
	afx_msg void _SetIMAXSkin(short nSkin);
	afx_msg void _SetIMAXData(short nKind, LPCTSTR pData);
	afx_msg BSTR _GetIMAXValue(LPCTSTR sValueText);
	afx_msg short _InitIMAX();
	afx_msg void _SetVisible(BOOL bVisible);
	afx_msg void _SetEnable(BOOL bEnable);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};