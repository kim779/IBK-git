#pragma once
// MapWnd.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CMapWnd window

class CMapWnd : public CWnd
{
// Construction
public:
	CMapWnd(CWnd *pParent);

// Attributes
public:
	
private:
	CWnd *m_pWizard;
	std::unique_ptr<class CScreenWnd> m_pScreenWnd;
	std::unique_ptr<class CGuideWnd>  m_pGuideWnd;

	CString m_strGuide;
	int m_nGuideCnt;

	BOOL	m_bBtnState[3];			//���̱�, ���߱⸦ ���� ��ư flag
	BOOL	m_bPopUp;				//�ʿ��� ��ư Ŭ������ ����(0:dll��ȸ��ư, 1:map��ư Ŭ��)

	CRect	m_topRc;				//�ǳ�

	std::unique_ptr <CfxStatic>		m_pPanelCtrl;	//��Ʈ�� �ǳ�
	std::unique_ptr <CfxStatic>		m_pStAccName;	//���� �̸�

	std::unique_ptr <class CAccountCtrl>	m_pAccount;
	std::unique_ptr <CfxEdit>		m_pEditPass;	//��й�ȣ
	std::unique_ptr <CfxImgButton>	m_pBtnInquiry;	//��ȸ��ư

	CString		m_strPass;			//��й�ȣ
	CString		m_strAccount;		//���¹�ȣ
	CString		m_strAccName;		//�����̸�
	CString		m_strUser;			//������̸�
	CString		m_strRoot;			//Ȩ���丮
	CString		m_strPal;			//�ȷ�Ʈ����
	
	CFont*		m_pFont;
	CFont*		m_pBFont;

	HBITMAP		m_hBmpBtnInquiry;	
	HBITMAP		m_hBmpBtnInquiry_dn;
	HBITMAP		m_hBmpBtnInquiry_hv;

	HBITMAP		m_hBmpRoundPanel[4];
	
	int			m_nMapHeight;
	int			m_nOrder;
	int			m_nKey;

// Operations
public:
	CWnd*		GetWizardWnd()	{ return m_pWizard; }
	HBITMAP		GetAxBitmap(CString bmps);
	CPen*		GetAxPen(COLORREF clr, int width, int style);
	COLORREF	GetIndexColor(int color);
	char*		Variant(int key, CString data = "");
	CFont*		GetFont()	{ return m_pFont; }
	void		SetView(int type, CString data);
	void		GetBtnState(BOOL *btn);							//��ư�� ���¸� �д´�. btn[3]
	void		Procedure(CString procs, int major, int minor);
	CString		Parser(CString &srcstr, CString substr);
	void		SendSize(int nWidth, int nHeight);
	void		DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap, bool bTrans = false);
	void		DrawRoundBitmap(CDC *dc, CRect rc, HBITMAP *hBitmap);
	void		DrawOuterSolidRect(CDC *dc, CRect rc, int nLeft, int nTop, int nRight, int nBottom, COLORREF color);
	CString		GetPalette();

protected:
	CFont*		GetAxFont(CString fName = "����", int point = 9, bool bItalic = false, int nBold = 0);
	void		CreateControl();
	void		SetBitmap();
	
	void		SetAccInfo(CString strBuffer);
	void		OnButtonInquiry();
	void		Trigger(CString strTrigger);
	BOOL		CheckPassWord(CString sAcc, CString sPass);
	BOOL		SendTR(CString strTR, CString strData, int nKey, int nStat);
	void		ParsingPass(CString strBuffer);

	
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMessage1(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMsgBtn(WPARAM wParam, LPARAM lParam);
};

