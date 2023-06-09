#if !defined(AFX_SCREENWND_H__5B5B4733_43A6_4094_A214_EA9ACAA2E271__INCLUDED_)
#define AFX_SCREENWND_H__5B5B4733_43A6_4094_A214_EA9ACAA2E271__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScreenWnd.h : header file
//

#define FONT_TYPE		"����ü"

#include "FnSetScreenWnd.h"
#include "MainFrmWnd.h"
#include "../../../SrcSite/control/fx_misc/fxImgButton.h"

/////////////////////////////////////////////////////////////////////////////
// CScreenWnd view

class CScreenWnd : public CWnd
{
protected:
	//DECLARE_DYNCREATE(CScreenWnd)

// Attributes
public:

private:
	CMainFrmWnd*			m_pwndMainFrm;
	CWnd*					m_pwndModuleMain;

	CFnSetScreenWnd			m_FnSet;

// Operations
public:
	CScreenWnd(CWnd *pParent);

	CWnd*		GetModuleMainWnd();
	void 		ReceiveTR(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen, int nKey = 0);
	void		SetPalete(COLORREF clrPalete, ST_TABCOLOR stTabColor);

public:
	CString		Variant(int nComm, CString strData = "")			{ return m_FnSet.Variant(nComm, strData);		}
	void		SetBtnImg(CfxImgButton *pBtn, CString strFileName)	{ m_FnSet.SetBtnImg(pBtn, strFileName);			}
	COLORREF	GetAxColor(int nColor)								{ return m_FnSet.GetAxColor(nColor);			}
	CBrush*		GetAxBrush(COLORREF clr)							{ return m_FnSet.GetAxBrush(clr);				}
	CPen*		GetAxPen(COLORREF clr, int nWidth, int nStyle)		{ return m_FnSet.GetAxPen(clr, nWidth, nStyle);	}

	CFont*		GetDefFont()										{ return m_FnSet.GetDefFont();					}
	CFont*		GetBoldFont()										{ return m_FnSet.GetBoldFont();					}
	COLORREF	GetPalete()											{ return m_FnSet.GetPalete();					}
	
	void		RegPortItem(CStringArray* pSaItemCode)				{ m_FnSet.RegPortItem(pSaItemCode);				}
	void		ShareCode(CString strCode)							{ m_FnSet.ShareCode(strCode);					}

	CString		GetFormCode()										{ return m_FnSet.GetFormCode();					}

	CString		GetMarketNameToCode(CString strMarketCode)			{ return m_FnSet.GetMarketNameToCode(strMarketCode); }

	void		GetAccNo(CStringArray* psaAccNo)					{ m_FnSet.GetAccNo(psaAccNo);					}

	void		RequestHaveItem(CString strAccNo)					{ m_FnSet.SendTr_AccountInfo(strAccNo);			}
	
	int	SetUserMaster(LPCSTR szKey, int nCount, CStringArray *pArrCode, CStringArray *pArrName)
	{
		return m_FnSet.SetUserMaster(szKey, nCount, pArrCode, pArrName);
	}
	
	BOOL GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub = 0)
	{
		return m_FnSet.GetStockMasterData(arrCode, arrName, Jgub, Upgub);
	}
	
	BOOL GetUpMasterData(CStringArray *&strCode, CStringArray *&strName, int jgub)
	{
		return m_FnSet.GetUpMasterData(strCode, strName, jgub);
	}

	void SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold = FALSE) 
	{ 
		m_FnSet.SetBtnImg(pBtn, nTextLen, bBold); 
	}

	void SendTR(CString strTRCode, char* szSendData, int nDataLen, int nKey, char cStat)
	{
		m_FnSet.SendTR(strTRCode, szSendData, nDataLen, nKey, cStat);
	}

	BOOL GetKSPUpMst(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption)
	{
		return m_FnSet.GetKSPUpMst(pastrJongCode, pastrJongName, pastrJongEngName, nOption);
	}

	BOOL GetKSQUpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
	{
		return m_FnSet.GetKSQUpMst(pastrUpCode, pastrUpName, pastrUpEngName, nOption);
	}

	void GetData_PortGroup(CStringArray& saKey, CStringArray& saGroupName)
	{
		m_FnSet.GetData_PortGroup(saKey, saGroupName);
	}
	
	BOOL GetData_PortItem(CStringArray& saCode, CStringArray& saName, int nKey)
	{
		return m_FnSet.GetData_PortItem(saCode, saName, nKey);
	}

	void OpenScreen(CString strScreenNo, CStringArray* pSaCode = NULL, CStringArray* pSaName = NULL, CString strOtherData = "")
	{
		m_FnSet.OpenScreen(strScreenNo, pSaCode, pSaName, strOtherData);
	}

	void SendHaveItem(CStringArray* psaItemCode)
	{
		m_pwndMainFrm->SendHaveItem(psaItemCode);
	}

	BOOL GetMemberCompany(CStringArray &arrCode, CStringArray &arrName)
	{
		return m_FnSet.GetMemberCompany(arrCode, arrName);
	}
	
private:
	void		OnCreate_MainWnd();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreenWnd)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreenWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CScreenWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREENWND_H__5B5B4733_43A6_4094_A214_EA9ACAA2E271__INCLUDED_)
