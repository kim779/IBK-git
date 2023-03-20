#if !defined(AFX_LEFTBARDLG_H__B80386F0_5717_4A42_B2BD_CE3773D3AB9B__INCLUDED_)
#define AFX_LEFTBARDLG_H__B80386F0_5717_4A42_B2BD_CE3773D3AB9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeftBarDlg.h : header file
//
#include "RscDlg.h"

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CLeftBarDlg dialog
#include "./control/XListCtrl.h"

#define	MAXPACKET			7
#define	ID_LIB_EZFINDER		3000

#include "XTabCtrl.h"
class CStdDialog;
class CTabDlg;
class CChartMng;
class CRegFavoriteDlg;
class CFavoriteThingsMng;
class CLeftBarDlg : public CRscDlg
{
// Construction
public:
	CLeftBarDlg(CWnd* pParent = NULL);   // standard constructor

	int		m_nMarketType;
	void	SetMarketType(int nMarketType) { m_nMarketType = nMarketType; m_tabctrl.m_nMarketType = nMarketType; }
	
	BOOL	m_bIsRunningSB;
	int		m_nSideBarIncrease;
	void	StartSideBar();
	void	EndSideBar();

	//-00----------------------------------------
	HWND	m_hMainFrame;
	void	SetMainFrame(HWND* pMainFrame);
	void	GetIndicatorNameListIntoComboBox(CComboBox& rComboBox,LPCTSTR p_strGroup);
	//===========================================

	UINT m_nID;
// Dialog Data
	//{{AFX_DATA(CLeftBarDlg)
	enum { IDD = IDD_LEFTBAR };	
	CXTabCtrl	m_tabctrl;
	//}}AFX_DATA
	CString m_strRootPath;
	CString m_szImageDir;
	CString m_strConfig;
	CBrush	m_brBkColor;
	COLORREF m_clrBk;

	void ChangeSkinColor();


	void	OnCodeAdvise	(CString strCode	);
	void	OnCodeUnAdvise	(CString strCode	);
	BOOL	IsAdvisingCodeOf(LPCTSTR lpCode		);
	void	SetRealPriceData(void *pData);

	long	GetCodeMasterArray(long lGubun, CStringArray& arCode, CStringArray& arName);
	void	GetIndicatorNameListIntoListCtrl(CXListCtrl& rxlistCtrl,LPCTSTR p_strGroup);
	void	GetIndicatorNameListIntoTreeCtrl(CTreeCtrl& rxtreeCtrl);

	HRESULT ReceiveData(HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID aTRData, long dwTRDateLen);
	void	SetStdDialog(CStdDialog* pStdDialog) { m_pStdDialog = pStdDialog; }
	void	SetpChartMng(CChartMng* pChartMng) {m_pChartMng = pChartMng;}
	
	void	SetPacketFromOutSide(CString strPacketFromOutSide);
	CString GetPacketFromOutSide();
	BOOL	GetCheckIgnoreShareData() { return m_bIgnoreShareData;}
	void	SetCheckIgnoreShareData(BOOL bIgnore) { m_bIgnoreShareData = bIgnore;}

	int		EzFinderLibGetFindDefine_TR(char *pFullFilePath, int nSize, int &nTrIndex, LPSTR &lpData);
	int		EzFinderLibGetConditionList(int nType, BOOL bGetAlert, LPSTR &lpData, LPSTR &lpPath);
	BOOL	EnableWindow(BOOL bEnable = TRUE);
	BOOL	GetEnableWindow();
	void	SetpFavoriteThingsMng(CFavoriteThingsMng* pFavoriteMng) { m_pFavoriteMng = pFavoriteMng;}
	void	SetConfig(BOOL bLoad = TRUE);

	CString GetUserPath();
private:
	CFavoriteThingsMng* m_pFavoriteMng;
	CStringList			m_Codes;										// DRDS �ڵ� ���� ����
	CStdDialog*			m_pStdDialog;
	CChartMng*			m_pChartMng;
	long				m_lJMaster;
	long				m_lKMaster;

	CString m_strPacketFromOutSide;
	BOOL m_bEnable;
	BOOL m_bIgnoreShareData;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftBarDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLeftBarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBthidechart();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTBARDLG_H__B80386F0_5717_4A42_B2BD_CE3773D3AB9B__INCLUDED_)
