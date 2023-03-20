#include "../../control/fx_misc/fxStatic.h"
#include <afxtempl.h>
#include <afxcoll.h>
#include <afxmt.h>

#pragma once
// MapWnd.h : header file
//

struct  _mid {
	char	mymd[6];	// �����
	char	hvil[3];	// ������������ �ϼ�
	char	jjil[1];	// �����ϼ� 0: �޷� 1: ����
};
#define	L_MID	sizeof (struct _mid)

struct  _gmod {
	char	ccod[8];	// ��RTS CODE
	char	cdif[8];	// �ݴ��
	char	ccur[8];	// �����簡
	char	cmdga[8];	// �ŵ�ȣ��
	char	cmsga[8];	// �ż�ȣ��
	char	cgvol[8];	// �ŷ���
	char	cmgjv[8];	// �̰���
	char	cmgjg[8];	// �̰�������
    char    csiga[8];   // CALL �ð�
    char    ckoga[8];   // CALL ��
    char    cjega[8];   // CALL ����
	
	char	price[8];	// ��簡

	char	pcod[8];	// ǲRTS CODE
	char	pdif[8];	// ǲ���
	char	pcur[8];	// ǲ���簡
	char	pmdga[8];	// �ŵ�ȣ��
	char	pmsga[8];	// �ż�ȣ��
	char	pgvol[8];	// �ŷ���
	char	pmgjv[8];	// �̰���
	char	pmgjg[8];	// �̰�������
	char    psiga[8];   // PUT �ð�
    char    pkoga[8];   // PUT ��
    char    pjega[8];   // PUT ����
	

};
#define L_GMOD	sizeof (struct _gmod)

struct  _mod {
	char	date[8];	// ����������
	char	mgym[11][6];	// ���� ���	KSJ ���� 11���� ����
	char	frts[8];	// �ֱٿ��� ����
	char	fnam[10];
	char	fcur[8];
	char	fdif[8];
	char	frat[8];
	char	krts[8];	// KOSPI 200
	char	kcur[8];
	char	kdif[8];
	char	krat[8];
	char	jjil[3];	// �����ϼ�
	char	hval[8];	// H.V
	//char	rciv[8];	// ��ǥ �� IV
	//char	rpiv[8];	// ��ǥ ǲ IV
	char	cdgl[8];	// CD �ݸ�
	char	bdjs[8];	// �������
	char	atmg[8];	// ATM(��簡)
	char	grec[4];	// �ڵ尳��( count of gmod )
	struct  _gmod    gmod[92];  // MAX_R = 92
};
#define	L_MOD	sizeof (struct _mod)


struct _data
{
	CString		sym;		// Rts Symbol
	//CString	display;	// display string
	CString		val;		// value
	//COLORREF	fgcolor;	// foreground
	//COLORREF	bgcolor;	// background
};
#define	L_DATA	sizeof (struct _data);

#define CNT_STATIC	14

const int datBasicPrice = 0, datBasicRate = 1, datJjil = 2;
const int datFuCode = 3, datFuCur = 4, datFuDif = 5, datFuRate = 6, datFuSell = 7, datFuBuy = 8, datFuGVol = 9, datFuMigeul = 10, datFuMDaebi = 11;
const int datK200Code = 12, datK200Cur = 13, datK200Dif = 14, datK200Rate = 15, datK200Time = 16, datKpCode = 17, datKpCur = 18, datEndCnt = 19;

struct _strike
{
	char	ccod[8 + 1];	// ��RTS CODE
	double	price;		// ��簡
	char	pcod[8 + 1];	// ǲRTS CODE
} typedef STRIKE;



/////////////////////////////////////////////////////////////////////////////
// CMapWnd window

class CMapWnd : public CWnd
{
// Construction
public:
	CMapWnd();

// Attributes
public:
private:
	CWnd				*m_pParent;
	CString				m_sRoot, m_sUser;
	COLORREF			m_clrBack, m_clrBackIn, m_blinkColor;
	CFont				*m_font, *m_Bfont;

	std::unique_ptr <class CfxStatic> m_pBorder, m_pContent;
	std::unique_ptr <CComboBox	> m_pMonthCbo, m_pGridCbo;	//2013.10.16 KSJ ȣ��/�ð��� �����޺��ڽ�
	std::unique_ptr <class CfxImgButton > m_pGreekBtn, m_pMingamBtn, m_pUpBtn, m_pDownBtn;
	std::unique_ptr <class CfxStatic>	m_pCallTitle, m_pPutTitle;
	std::unique_ptr <class CfxGrid> m_pCallGrid, m_pPutGrid;

	std::unique_ptr <class CCfg> m_cfg{};

	std::unique_ptr<class COpSensCal> m_opsensCal;
	std::unique_ptr<class CfxStatic []> m_static;

	HBITMAP				m_bmp2BTN, m_bmp2BTN_DN, m_bmp2BTN_EN;
	HBITMAP				m_bmp3BTN, m_bmp3BTN_DN, m_bmp3BTN_EN;
	HBITMAP				m_bmpUp, m_bmpUp_DN, m_bmpUp_EN;
	HBITMAP				m_bmpDown, m_bmpDown_DN, m_bmpDown_EN;

	char				m_pCode[11][7]{};			// ���� ���	 KSJ ���� 11���� ����
	struct _data		m_pdataX[datEndCnt];		// �⺻����
	//struct _strike* m_pStrike;			// ��簡
	std::unique_ptr<STRIKE[]> m_pStrike;


	//class CfxCodeCtrl	*m_pCodeCtrl;

	HBITMAP				m_bmpSelL, m_bmpSelM, m_bmpSelR, m_bmpL, m_bmpM, m_bmpR;
	int					m_DataCount;
	CString				m_date, m_jjilOrigin, m_hv, m_cdgl, m_bdgs, m_atmg;
	
	BOOL				m_bShowMingam, m_bShowSiKoJe;

	BOOL				m_bResult1, m_bResult2, m_bResult3;
	CString				m_mouseDat;
	bool				m_bBlink{};
	
	BOOL				m_bCreated, m_bChangeFont;
	int					m_iCurFont, m_iOldSize[2];
	double				m_lSizeRate[2];
	CRect				m_pClientRc;
	int					m_iGridColwidths;
	HBITMAP				m_hRoundBitmap[8]{};
	int					m_iScrollw;

// Operations
public:
	void		CreateMap(CWnd* pParent);
	CString		Variant(int comm, CString data);
	CBitmap*	getBitmap(CString path);
	HBITMAP		GetAxHBitmap(CString stPath);

	COLORREF	GetIndexColor(int index);
	CBrush*		getAxBrush(COLORREF clr);
	CFont*		getAxFont(CString fName = "����ü", int point = 9, int style = 0);
	void		SetPalette();

	void		sendTR(CString trCode, char* datB, int datL, int nKey, int nStat);
	void		sendSubTr();

	void		initSymbol();
	void		initData();
	void		initGrid(CfxGrid* grid, bool header);
	void		insertRow(int row = 9999);
	void		sendDataTr();
	void		parseData(char* pData, int datL);
//	void		parseAlert(char* pData);
	void		parseAlert(LPARAM lParam);
	COLORREF	ParseSymbol(char *pData, CString& symbolS, CString& signS);
	void		generateFData();
	void		generateODatas();
	void		generateOData(int nRow, bool callput);
	void		moveGrid(int step);
	void		syncGridTopRow();
	void		showMingamField(BOOL showMingam);
	void		showSiKoJeField(int showSiKoJe);	//2013.10.16 KSJ �ð��� �����ֱ�
	double		getOptionStrike(CString opCode);
	double		getAtmStrike(double k200);
	int		GetMinTime(CString time);
	CString		Parser(CString &srcstr, CString substr = "\t");
	bool		GetBlinkInfo();
//	void		CheckDupCodeData(char * dat);		// ������ �ڵ� ó��(ȣ���̿��� ������)
	BOOL		IsHogaData(char * data);


	void		ChangeFont(int ifontsize, LPCTSTR lszfont);
	void		ResizeControl(int cx, int cy);
	void		SetGridColumnWidth();
	void		DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap);
	void		DrawCornerRoundBitmap(CDC* pDC, CRect rc, int iType = 0);
	void		LoadRoundBitmap();




// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapWnd();

	// Generated message map functions
protected:
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg	void OnClickGreek();
	afx_msg void OnClickMingam();
	afx_msg void OnClickUp();
	afx_msg void OnClickDown();
	afx_msg void OnCallGridRMouseDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPutGridRMouseDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGridLMouseUp(NMHDR* pNMHDR, LRESULT* pResult);
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnPaint();
	afx_msg void OnSelChangeMonth();
	afx_msg void OnSelChangeGrid();
	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:		// RTM
	CArray <char *, char *>	m_arBytesRTM;

	CCriticalSection	m_syncRTM;
	CWinThread*		m_pThreadRTM;
	bool			m_bThreadRTM;
	bool			m_bRTMReady;

	void			doRTM(char *pBytes);
	bool			WaitRTM();

private:	// RTM
	HANDLE			m_hEventRTM;
	
	void			initRTM();
	void			stopRTM();
	void			removeRTM();
};

