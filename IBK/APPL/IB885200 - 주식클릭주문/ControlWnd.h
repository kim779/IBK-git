#pragma once

//
//#include "MapWnd.h"

struct _button
{
	CRect	rcChk;
	CRect	rcText;
	CString data;
	bool	chk{};
};
/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd(CWnd* pwndView, class CTool* pTool);

protected:
	CWnd*		m_pwndView;

	int		m_iJmdw;	// �ֹ���������

	COLORREF	m_clrBG;
	COLORREF	m_clrDT;
	COLORREF	m_clrTFG;

	CFont*		m_pfont;
	CFont*		m_pfontB;
	CPen*		m_ppenLine;

	CRect		m_sRc1;
	CRect		m_sRc2;
	CRect		m_sRc3;
	CRect		m_sRc4;
	CRect		m_sRc5;

	CRect		m_tRc1;		// �ŵ����� ����
	CRect		m_tRc2;		// �ż����� ����
	CRect		m_tRc3;		// �ֹ� ����
	CRect		m_rectJangoS;		// �ܰ�
	CRect		m_tRc5;		// ��մܰ�
	CRect		m_tRc6;		// �򰡼���
	CRect		m_tRc7;		// �ֹ�����
	CRect		m_tRc8;		// ����Ÿ� �ֹ�ƽ ����
	CRect		m_tRc9;		// �̼����� ����

	CString		m_text1;
	CString		m_text2;
	CString		m_text3;
	CString		m_strJangoS;
	CString		m_text5;
	CString		m_text6;
	CString		m_text7;
	CString		m_text8;

	CRect		m_rectMdCnt;	
	CRect		m_rectMsCnt;		
	CRect		m_oRc3;		
	CRect		m_rectJango;	
	CRect		m_rectPgdg;		
	CRect		m_rectPgsi;	
	CRect		m_rectMaxMsCnt;
	
	CString		m_strMdCnt;	// �ŵ����ɼ���
	CString		m_strMsCnt;	// �ż����ɼ���
	CString		m_strJango;	// �ܰ�
	CString		m_strPgdg;	// ��մܰ�
	CString		m_strPgsi;	// �򰡼���
	CString		m_strMaxMsCnt;	// �ִ�ż����ɼ���

	CString		m_curr;
	CString		m_cmsg;		// �Ѹ��Աݾ�
	CString		m_ssly;		// ��������
	CString		m_maip;		// �Ѹ��Աݾ�
	CString		m_jango;	// �ܰ�

	std::unique_ptr<class CfxImgButton>	m_pBtnAccount;
	std::unique_ptr<class CfxImgButton>	m_pBtnChart;
	std::unique_ptr<class CfxImgButton>	m_pBtnRevaluate;
	std::unique_ptr<class CfxImgButton>	m_pBtnMDCan;
	std::unique_ptr<class CfxImgButton>	m_pBtnMSCan;
	std::unique_ptr<class CfxImgButton>	m_pBtnMaxMS;
	std::unique_ptr<class CfxImgButton>	m_pBtnJmcn;

	std::unique_ptr<class C3DEdit>		m_pEditJmdg;
	std::unique_ptr<class C3DEdit>		m_pEditJmcn;
	std::unique_ptr<class C3DEdit>		m_pEditMoney;
	std::unique_ptr<class C3DEdit>		m_pEditTick;
	std::unique_ptr<class CSpinCtrl>	m_pSpinJmcn;
	std::unique_ptr<class CSpinCtrl>	m_pSpinTick;
	std::unique_ptr<class CContainerWnd>	m_pcontwndAmount;
	std::unique_ptr<class CCodeReg>		m_pCodeReg;
	std::unique_ptr<class CContainerWnd>	m_pAmount;

	class CTool*		m_pTool;

	CComboBox	m_cbMulti;

	struct _button	m_money;
	struct _button	m_multi;
	struct _button	m_condition[3];

//	CString		m_home;
//	CString		m_user;

	enum { KOSPI = 0, KOSDAQ }	m_market;

	int GetTextWidth(CString sText);

// Operations
public:
	void		ReceiveSise(char* pData, int len);
//	void		DispatchNotice(CString Notice);
	void		NoticeParse(CString dat, CMapStringToString& ary);
	int		GetJumunCount();
	CString		GetJumunCondition();
	int		GetTickCount();
	int		GetPGDG();
	void		SetPrice(int mmgb, int price);
	void		SetJmdg(CString jmdg);
	void		SetMaxJmcn(CString strData, int iMmgb);
	void		SetCurr(CString curr);
	int		GetMultiCount();
	void		ClearMoney();
	void		SetSize(int depth);
	void		ClearJango();
	void		ClearMaip();
//	void		SetJango(WPARAM wParam, LPARAM lParam);
	void		SetJango(CString strCodx, CString strJango, CString strPgdg, CString strPgsi);
	void		JangoClear(CString strJCod);
	void		AddJCod(CString strJCode, CString strJName);
	CString		GetMaip()	{ return m_maip; }
	CString		GetJango()	{ return m_jango; }
	void		Palette();
	BOOL GetWaitMode();
	void SetWaitMode(BOOL bWait);
	void ChangeFont(double* lRate, CFont* pfont, CFont* boldFont=NULL);


protected:
	// Init
	void		Init();
	void		SetButtonBitmaps();
	
	// Draw
	void		DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat = 0, UINT type = 0);
	int		calculateCommission(int money);

	// etc
	void		SendMaxJmcn(CString strJmdg, int iGubn);
	CString		GetCodx();
	void		LoadBuffer();


public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	CString m_strCondition;
	virtual ~CControlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnButtonAccount();
	afx_msg void OnButtonChart();
	afx_msg void OnButtonReval();
	afx_msg void OnButtonMDCan();
	afx_msg void OnButtonMSCan();
	afx_msg void OnButtonJmcn();
	afx_msg void OnButtonMaxMS();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnContainerAmount(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnChangeJmcn();
	DECLARE_MESSAGE_MAP()
};

