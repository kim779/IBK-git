#pragma once
// ControlWnd.h : header file
//
#include "MapWnd.h"

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
	CRect		m_tRc4;		// 2015.03.16 KSJ �Ÿűݾ�
	CRect		m_rectJangoS;		// �ܰ�
	CRect		m_tRc5;		// ��մܰ�
	CRect		m_tRc6;		// �򰡼���
	CRect		m_tRc7;		// �ֹ�����
	CRect		m_tRc8;		// ����Ÿ� �ֹ�ƽ ����
	CRect		m_tRc9;		// �̼����� ����


	CString		m_text1;
	CString		m_text2;
	CString		m_text3;
	CString		m_text4;	// 2015.03.16 KSJ �Ÿűݾ�
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
	CRect		m_rectConAmt;	//2015.03.16 KSJ �Ÿűݾ�

	CString		m_strMdCnt;	// �ŵ����ɼ���
	CString		m_strMsCnt;	// �ż����ɼ���
	CString		m_strJango;	// �ܰ�
	CString		m_strPgdg;	// ��մܰ�
	CString		m_strPgsi;	// �򰡼���
	CString		m_strMaxMsCnt;	// �ִ�ż����ɼ���
	CString		m_strConAmt;	//2015.03.16 KSJ �Ÿűݾ�

	CString		m_curr;
	CString		m_cmsg;		// �Ѹ��Աݾ�
	CString		m_ssly;		// ��������
	CString		m_maip;		// �Ѹ��Աݾ�
	CString		m_jango;	// �ܰ�

	std::unique_ptr <class CfxImgButton>	m_pBtnAccount;
	std::unique_ptr <class CfxImgButton>	m_pBtnChart;
	std::unique_ptr <class CfxImgButton>	m_pBtnRevaluate;
	std::unique_ptr <class CfxImgButton>	m_pBtnMDCan;
	std::unique_ptr <class CfxImgButton>	m_pBtnMSCan;
	std::unique_ptr <class CfxImgButton>	m_pBtnMaxMS;
	std::unique_ptr <class CfxImgButton>	m_pBtnJmcn;
	std::unique_ptr <class CfxImgButton>	m_pBtnConAmt;	//2015.03.16 KSJ �Ÿűݾ�


	std::unique_ptr <class C3DEdit>		m_pEditJmdg;
	std::unique_ptr <class C3DEdit>		m_pEditJmcn;
	std::unique_ptr <class C3DEdit>		m_pEditMoney;
	std::unique_ptr <class C3DEdit>		m_pEditTick;

	std::unique_ptr <class CSpinCtrl>	m_pSpinJmcn;
	std::unique_ptr <class CSpinCtrl>	m_pSpinTick;

	class CCodeReg* m_pCodeReg;

	class CContainerWnd*	m_pcontwndAmount;
	class CContainerWnd*	m_pAmount;

	class CTool*		m_pTool;

	CComboBox	m_cbMulti;
	CComboBox	m_cbCrType;

	struct _button	m_money;
	struct _button	m_multi;
	struct _button  m_click;
	struct _button  m_sijang;	//2012.10.29 KSJ ���尡 �߰�
	struct _button	m_condition[3];
	struct _button  m_chaik;

	CString m_strHtsFee;		//2015.03.16 KSJ	HTS������
	CString m_strHtsSaveFee;	//2015.03.16 KSJ	SAVE HTS ������
	bool	m_bStaff;			//2015.03.16 KSJ	��������
//	CString		m_home;
//	CString		m_user;

	enum { KOSPI = 0, KOSDAQ }	m_market;

	int GetTextWidth(CString sText);

	CString m_sDept;

	CRect	m_rcAlert;
	HBITMAP m_hAlertBitmap;
	void		DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap);

public:
	void		SetPrgbRadio(int index, CString sgubn = "");
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
	void		SetFee(CString strData, int iMmgb);	//2015.03.16 KSJ ��ü������
	void		CalcFee(int iMmgb);	//2015.03.16 KSJ ��ü������ ���
	void		SetCurr(CString curr);
	int		GetMultiCount();
	void		ClearMoney();
	void		SetSize(int depth);
	void		ClearJango();
	void		ClearMaip();
	void		SetJango(CString strCodx, CString strJango, CString strPgdg, CString strPgsi);
	void		JangoClear(CString strJCod);
	void		AddJCod(CString strJCode, CString strJName);
	CString		GetMaip()	{ return m_maip; }
	CString		GetJango()	{ return m_jango; }
	void		Palette();
	BOOL GetWaitMode();
	void SetWaitMode(BOOL bWait);
	void ChangeFont(double* lRate, CFont* pfont, CFont* boldFont=NULL);
	BOOL		GetOneClickJumun();
	BOOL		GetCheckMarket();
	void		SetAmountRemain(bool flag);

	//
	void		ClearChegGrid();
	void		SetRealData(CString time, CString curr, CString cvol);
	bool		GetSijang() { return m_sijang.chk; };
	bool		GetChaik() { return m_chaik.chk; };
	
	bool	m_bMarketConfirm;

	int     GetCondition();     //0���� 1�ſ� 2����
	CString GetCrdType();		//03 �ڱ����� 01 ��������

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
	CString		GetJcod();	//2013.01.10 KSJ �����ڵ� ��������
	void		SendFee(CString strJmdg, int iGubn);

	CToolTipCtrl*	m_pTooltip;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
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
	afx_msg void OnButtonConAmt();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnContainerAmount(WPARAM wParam, LPARAM lParam);
	afx_msg void OnChangeJmcn();
	DECLARE_MESSAGE_MAP()
};

