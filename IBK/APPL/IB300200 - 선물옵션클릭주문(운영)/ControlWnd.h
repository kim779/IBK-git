#pragma once

#include "MapWnd.h"

struct _button
{
	CRect	rcChk;
	CRect	rcText;
	CString data;
	bool	chk{};
	bool	bRadio{};
};

const int rdCnt = 2;
enum { RD_NOT = 0, RD_IOC, RD_FOK };
/////////////////////////////////////////////////////////////////////////////
class CControlWnd : public CWnd
{
public:
	CControlWnd(CWnd* pParent, CWnd* pView);
	virtual ~CControlWnd();

	//{{AFX_VIRTUAL(CControlWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnButtonAccount();
	afx_msg void OnButtonChart();
	afx_msg void OnButtonReval();
	afx_msg void OnButtonCan();
	afx_msg void OnButtonJmcn();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnContainerAmount(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	CMapWnd* m_pParent{};
	CWnd* m_pView{};
	std::unique_ptr<class CfxImgButton> m_pBtnAccount{};
	std::unique_ptr<class CfxImgButton> m_pBtnChart{};
	std::unique_ptr<class CfxImgButton> m_pBtnRevaluate{};
	std::unique_ptr<class CfxImgButton> m_pBtnCan{};
	std::unique_ptr<class CfxImgButton> m_pBtnJmcn{};
	std::unique_ptr<class CfxImgButton> m_pBtnRsCancel{};
	std::unique_ptr<class CfxImgButton> m_pBtnJmCancel{};

	std::unique_ptr<class CfxEdit> m_pEditJmdg{};
	std::unique_ptr<class C3DEdit> m_pEditJmcn{};
	std::unique_ptr<class C3DEdit> m_pEditTick{};
	std::unique_ptr<class CSpinCtrl> m_pSpinJmcn{};
	std::unique_ptr<class CSpinCtrl> m_pSpinTick{};
	std::unique_ptr<class CContainerWnd> m_pcontwndAmount{};
public:
	std::unique_ptr<class CJangoWnd> m_pJangoWnd{};
	bool	m_bMarketConfirm{};
protected:
	std::unique_ptr<class CContainerWnd> m_pAmount{};

	CRect		m_sRc1, m_sRc2, m_sRc3, m_sRc4;		// ������ : �ֹ�����, �����ֹ�, ��Ŭ��, �ܰ�
	
	struct	_jmsr	{
		CString		text;
		CRect		rect;
		COLORREF	color{};
	} m_jmsr[3][3]{};						// �ֹ����ɼ��� 0:row 1:col

	enum { R_TITLE = 0, R_MD, R_MS };			// row
	enum { C_TITLE = 0, C_NEW, C_CLEAR };			// col
	CRect		m_jmsrRc;				// �ֹ����� ���� invalidate����
	struct _button	m_chkJmsr;				// �ֹ����ɼ����ڵ����� check

	CRect		m_tRc3;		// �ֹ� ����
	CRect		m_tRc4;		// �ֹ�����
	CRect		m_tRc5;		// ����Ÿ� �ֹ�ƽ ����

	CString		m_text3;	// �ֹ�����
	CString		m_text4;	// �ֹ�����
	CString		m_text5;	// �����ֹ�ƽ

	// �ܰ� title
	CString		m_tgubn;	// ����
	CString		m_tjgsu;	// �ܰ����
	CString		m_tjmga;	// �ֹ�����
	CString		m_tpgga;	// ��մܰ�
	CString		m_tpgsk;	// �򰡼���

	// �ܰ� title rect
	CRect		m_rgubn;	// ����
	CRect		m_rjgsu;	// �ܰ����
	CRect		m_rjmga;	// �ֹ�����
	CRect		m_rpgga;	// ��մܰ�
	CRect		m_rpgsk;	// �򰡼���

	// �ܰ� data
	CString		m_ogubn;	// ����
	CString		m_ojgsu;	// �ܰ����
	CString		m_ojmga;	// �ֹ�����
	CString		m_opgga;	// ��մܰ�
	CString		m_opgsk;	// �򰡼���

	// �ܰ� data rect
	CRect		m_rogubn;	// ����
	CRect		m_rojgsu;	// �ܰ����
	CRect		m_rojmga;	// �ֹ�����
	CRect		m_ropgga;	// ��մܰ�
	CRect		m_ropgsk;	// �򰡼���

	CRect		m_oRc3;		
	CString		m_output3;
	CString		m_output4;	// ��������
	CString		m_output5;	// ��մܰ�
	CString		m_output6;	// �򰡼���

	CString		m_curr;
	CString		m_cmsg;		// �Ѹ��Աݾ�
	CString		m_ssly;		// ��������
	CString		m_maip;		// �Ѹ��Աݾ�
	CString		m_jango;	// �ܰ�
	CString		m_mouseDLL;

	struct _button	m_multi;
	struct _button	m_oneclick;
	struct _button	m_clickmod;
	struct _button	m_trackmod;
	struct _button  m_market;
	CComboBox	m_cbMulti;

	struct	_button	m_rdNOT {};
	struct	_button	m_rdIOC {};
	struct	_button	m_rdFOK {};
	int		m_rdSel{};

	struct _button	m_chkHold {};

	CString		m_home;
	CString		m_user;

	CFont*		m_pFont;
	CFont*		m_pBFont;

	int		m_mmgb{};
	bool		m_bInit{};

	std::unique_ptr <CToolTipCtrl> m_pTipCtrl{};

private:
	HBITMAP		m_hBitmap2{}, m_hBitmap2_dn{}, m_hBitmap2_hv{};
	HBITMAP		m_hBitmap1{}, m_hBitmap1_dn{}, m_hBitmap1_hv{};
	HBITMAP		m_hBitmapJMCN{}, m_hBitmapJMCN_dn{}, m_hBitmapJMCN_hv{};

public:
	void		Init();
	void		DispatchData(char* pData, int len);
	void		DispatchMicheg(char *pData, int len, bool bErase=true);
	void		NoticeParse(CString dat, CMapStringToString& ary);
	int		GetJumunCount();
	int		GetTickCount();
	int		GetPGDG();
	void		SetMicheg(CString code, int cnt);
	void		SetPrice(int mmgb, int price);
	void		SetJmdg(CString jmdg);
	void		SetJmcn(CString code, CString jmcn);
//	void		SetMaxJmcn(CString oldj, CString newj, int mmgb);
	void		SetCanClearS(CString str);
	void		SetCanClearB(CString str);
	int		GetMultiCount();
	void		SetSize(int depth);
	void		SetJango(CString code, CString jgvl, CString pgdg, CString pgga, CString jmgr, CString pgsk, bool bAll);
	void		DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat = 0, UINT type = 0, COLORREF tcolor=0);
	void		WriteFile(char* pBytes, int nBytes);
	void		ClearJango();
	void		ClearMaip();
	bool		GetTrackMode()  { return m_trackmod.chk; }
	bool		GetOneClick()	{ return m_oneclick.chk; }
	bool		GetClickMod()	{ return m_clickmod.chk; }
	bool		GetChkHold()	{ return m_chkHold.chk; }
	bool		GetMarket()	{ return m_market.chk; }
	CString		GetMaip()	{ return m_maip; }
	CString		GetJango()	{ return m_jango; }
	CString		GetJmgb();
	bool		GetChkJmsr()	{ return m_chkJmsr.chk; }
	CString		Parser(CString &srcstr, CString substr);
	void		SearchCan();
	BOOL GetWaitMode();
	void SetWaitMode(BOOL bWait);
	void		ResizeControl(int cx, int cy);
	void		ChangeFont(double* lRate, CFont* pfont, CFont* boldFont);



protected:
	void		setButtonBitmaps();
	int		calculateCommission(int money);
};

