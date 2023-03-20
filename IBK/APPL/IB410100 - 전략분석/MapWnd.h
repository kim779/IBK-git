
#include <Afxmt.h>
#include <afxtempl.h>

#pragma once
// MapWnd.h : header file
//

#define INFO_MAX 35		//2014.04.16 KSJ 33 -> 35�� ����
#define TAB_HEIGHT	26
#define ROUND_PANEL		1
#define ROUND_CONTENTS	2

#define GUIDE_HEIGHT 18
#define GRIDHEADER_HEIGHT 45
#define	GUIDCOLOR_INDEX		34
#define	GUIDTXTCOLOR_INDEX	97

struct _codelist	// option code list
{
	CString code;		// �ڵ�
	CString name;		// �����
	int	mdMargin{};	// �ŵ����ű�
};

struct _strikelist
{
	char	strike[9];	// strike
	char	code[2][9];	// call,put
};


struct _bdat {
	CString		time;		// K200 Time
	CString		frts;		// �ֱٿ��� ����
	CString		fnam;
	double		fcur{};
	CString		krts;		// KOSPI
	double		kcur{};
	CString		k200rts;	// KOSPI 200
	double		k200cur{};
	double		jjil[2]{};	// ������, �޷���
	double		hv[4]{};		// hv (90/60/30/21)
	double		cdgl{};		// CD �ݸ�
	double		bdjs{};		// �������
	double		basicprc{};
};

struct _gdat{
	bool		addnew{};
	CString		ccod;		// �����ڵ� or Sum���ÿ��� Title
	int			jango{};		// �ܰ���� or Sum���� �ɼ� �ŵ�����
	int			gb{};		// �ֹ�����(�ŵ� -1, �ż� 1)  or Sum���� �ɼ� �ż�����
	double		prc{};		// �ֹ��������� or Sum���ÿ��� �ҿ��ڱ�
	double		curcalc{};	// "���簡"�� ǥ�ÿ�
					// [���簡]/[���ȣ��...]/[�����ϰ�����簡]���� ǥ���� �� �ִ�
	double		ordprc{};		// ���Դܰ� or ��ȸ�� ���簡
	double		miga{};		// ���԰�
	double		sonik{};		// �򰡼���
	double		gdan{};		// �ŷ��¼� 2012.05.17 KSJ �ɼ� 6������ ��� 100000�� �Ǵ� 500000���� �ɼ� �ִ�.

	double		delta{};
	double		gamma{};
	double		theta{};
	double		vegga{};
};

// �ΰ�����
struct  _mdat {
	// ��ȸ���� ******************
	CString		ccod;		// �����ڵ�
	double		ccur{};		// ���簡
	//char		cdif;		// ���
	double		shga{};		// ���Ѱ�
	double		hhga{};		// ���Ѱ�
	double		mdga[5]{};	// �ŵ�ȣ��1~5
	double		msga[5]{};	// �ż�ȣ��1~5
	//double	cgvol[8];	// �ŷ���
	double		jjil[2]{};	// �����ϼ� (�ŷ��ϼ�/�޷��ϼ�)

	// ������� ******************
	CString		iv;
	//CString		theoryamt;
	double		delta{};
	double		gamma{};
	double		theta{};
	double		vegga{};
};

/////////////////////////////////////////////////////////////////////////////
// CMapWnd window

class CMapWnd : public CWnd
{
// Construction
public:
	CMapWnd(CWnd *parent);

public:
	CWnd		*m_pView;
	class CIB410100App	*m_pApp;
	CString		m_sRoot;
	COLORREF	m_clrGradient1, m_clrGradient2;
	COLORREF	m_bkColor, m_fColor;
	CFont		*m_pFont, *m_pBFont;
	CBrush		m_brNull;

	//enum GRID_DATAMODE {gdJango, gdPortfolio} m_gridMode;
	enum GRID_PRCMODE {pmBuyPrc, pmCurrPrc} m_prcMode;
	enum GRID_CURRMODE {cmCurr, cmOp1, cmOp2, cmExpire} m_currMode;

	std::unique_ptr<class CCfg			>m_cfg;
	std::unique_ptr<class CNotify		>m_pNotify;
	std::unique_ptr<class COpSensCal	>m_opsenscal;
	std::unique_ptr<class CPrice		>m_price;
	std::unique_ptr<class CAccountCtrl	>m_pAccountCbo;
	std::unique_ptr<class CfxEdit		>m_pPwdEdit;
	std::unique_ptr < CComboBox			>m_pMMCbo, m_pPrcCbo;
	std::unique_ptr<class CfxImgButton	>m_pMingamBtn;
	std::unique_ptr<class CfxStatic		>m_static[12];
	std::unique_ptr<class CfxImgButton	>m_pJangoBtn;
	std::unique_ptr<class CfxImgButton	>m_pOrdBtn;
	std::unique_ptr<class CfxImgButton	>m_pSaveStgBtn, m_pAddBtn, m_pDelBtn, m_pDelAllBtn, m_pExcelBtn;
	std::unique_ptr<class CfxGrid		>m_pPFGrid;
	std::unique_ptr<class CfxTab		>m_pTab1;
	std::unique_ptr<class CSheet1		>m_pSheet1;
	std::unique_ptr<class CfxImgButton	>m_pOrderBtn;	//2012.07.23 KSJ �ֹ���ư �߰�
	std::unique_ptr<class CfxImgButton	>m_pConfigBtn;	//2012.10.05 KSJ ������â ������ư �߰�
	std::unique_ptr<class CConfigDlg	>m_pConfigDlg;		//2012.10.05 KSJ ������â �������̾�α�
	CImageList* m_imageList;

	CString				m_Hts;
	CString				m_AccnName;
	std::unique_ptr<struct _bdat	    >m_bData;
	struct _gdat		m_allSum;
	//CArray<std::unique_ptr<struct _mdat>, std::unique_ptr<struct _mdat>>	m_mData;
	//CArray<std::unique_ptr<struct _gdat>, std::unique_ptr<struct _gdat>>	m_PFCodes;
	CArray<struct _mdat*, struct _mdat*>	m_mData;
	CArray<struct _gdat*, struct _gdat*>	m_PFCodes;
	bool			m_bRts;
	double			m_fjjil[2]; // 0:������ 1:�޷���

	CCriticalSection	m_csSort;

	class CfxGrid		*m_priceGrid;
	int			m_priceRow;
	CString			m_rtnStr;
	CString			m_strPal; // ��Ų ����� ����ϱ� ���ؼ�
	CString			m_PriceData;
	CRect			m_PriceRect;

public:
	CWnd*	m_pMainWnd;

	HBITMAP		m_bmp2BTN, m_bmp2BTN_DN, m_bmp2BTN_EN;
	HBITMAP		m_bmp4BTN, m_bmp4BTN_DN, m_bmp4BTN_EN;
	HBITMAP		m_bmp5BTN, m_bmp5BTN_DN, m_bmp5BTN_EN;
	HBITMAP		m_bmp6BTN, m_bmp6BTN_DN, m_bmp6BTN_EN;
	HBITMAP		m_bmpSearchBTN, m_bmpSearchBTN_DN, m_bmpSearchBTN_EN;	//2012.07.23 KSJ ��ȸ��ư �߰�

	CString		m_StgInfoName[INFO_MAX];	// strategy name
	int		m_jjIdx;		// �����ϼ�: 0-�ŷ��ϼ�, 1-�޷��ϼ�
	int		m_idxVar;		// ������  : 0-HV, 1-�ǽð�IV, 2-��ǥIV
	int		m_timeOpt;		// ���߽ð���ġ���� ���� : 1, ������: 0
	int		m_idxX;			// x��ǥ�� : 0-��������, 1-�����ϼ�, 2-������
	CString		m_timeStart, m_timeEnd;	// ���߽ð���ġ���� ����� ���۽ð��� ���ð� ����
	CString		m_fcode;		// future code, CPropPage01::InitData()���� set
	CString		m_sUser;		// name
	CString		m_sSavePath;		// root/user/name
	CString		m_atmCall;		// atmCall����
	struct _opgubn
	{
		char	name[15];	// �����̸�
		char	gubn;		// �������а�
	};
	_opgubn		m_pOPMonthInfo[11];	// �ɼǿ��� 11��	2014.08.13 KSJ 
	CArray <_codelist *, _codelist *> m_OPCodeList;		// full option codelist
	CArray <_strikelist, _strikelist> m_pOPStrikeList[4];	// �� ������ ��簡����Ʈ
	CStringArray	m_HedgeCodeList;	// �����Ҷ� ���� �ڵ帮��Ʈ(�ɼ� �ֱٿ�)

	bool	m_excelF;
	CString	m_excelS;
	HANDLE	m_excelH;
	CCriticalSection m_syncx;
	int		m_iGuideHeight;
	int		m_iGridHeadrHeight;
	CString m_strGuideMessage;
	COLORREF	m_guidecolor, m_guideTxtcolor, m_panelcolor[2];
	HBITMAP	m_hGuideBitmap, m_hRoundBitmap[4];
	bool m_bOnDialog;	//KSJ 2012.07.18 ���̾�α� ������ �׷��� �ٽñ׸��� �ʰ��ϱ� ����
	bool m_bAdd;		//KSJ 2012.08.28 �޼��� ������ �׷��� �׷��ֱ�
	class CSearchPage * m_pSearchPage;
	bool m_bMulti;		//KSJ 2012.10.08 ��Ƽ����â���� 

	HINSTANCE		m_hInst;				
	CWnd*			m_pwndFOMultiSel;
	struct	    	_param	FOMultiSelparam;
// Operations
public:
	void		CreateMap(CWnd* pParent);
	CString		Variant(int comm, CString data);
	CBitmap*	getBitmap(CString path);
	COLORREF	getIndexColor(int index);
	CBrush*		getAxBrush(COLORREF clr);
	CFont*		getAxFont(CString fName = "����ü", int point = 9, int style = 0);
	void		sendTR(CString trCode, char* datB, int datL, int nKey, int nStat);
	void		sendLedgerTR(char cJobCd, CString strSvcn, char* strData, int nSize, int nKey, int nStat, CString screen);

	void		SetPalette();
	void		DrawGradientRect(CDC *pDC, CRect rc);
	void		DrawExtraText(CDC *pDC);
	void		initGrid(CfxGrid* grid);
	void		SetGridMode();
	CString		Parser(CString &srcstr, CString substr = "\t");

	void		queryInitValue();
	void		queryMingam();
	void		queryDeposit(BOOL bALL, CString k200Guess);
	void		parseAlert(char* pDat);
	void		parseAlert(struct _alertR* alertR);
	void		parseInitValue(char* dat);
	void		parseUpJong(char* dat);
	void		parseMingam(char* dat);

	void		setJango(CString dat);
	bool		setPF(int nRow, CString code, int gb = 1, int jango = 1, double prc = 0);
	void		deleteGrid(class CfxGrid* pGrid, int idx, bool removeAll);
	int			findGrid(class CfxGrid* pGrid, CString code);
	int			findMData(CString code);
	int			findGData(CArray<struct _gdat*, struct _gdat*> *arr, CString code);
	void		deleteMData(CString code);
	void		readCfg();
	void		generateBasicPrice();
	void		generateFData();
	void		generateMingam(int idx);
	void		generateSum();
	double		getOptionStrike(CString opCode);
	void		setGridVal(class CfxGrid *grid, struct _mdat* mdat, bool init = false);
	void		initGridRow(CfxGrid *pGrid, int row);
	void		addImageList(CString ImageName);
	CString		PriceGubun(CString str);
	void		SetGraph();
	void		SaveINI(int no, CString name);
	void		DrawSum(struct _gdat *sumdat, CRect rc, CDC *pDC, COLORREF bkClr);
	void		DoOrder();

	COLORREF	GetAxColor(int iColor);
	void		ChangeColors();
	void		ChangePrcMode(enum GRID_PRCMODE mode);
	void		ChangeCurrMode(enum GRID_CURRMODE mode);
	void		redrawGraph();
	void		hideWizardBtn();
	void		ReArrangeCodes(CfxGrid *pGrid);
	void		InvalidateSum();
	CString		CodePlusGab(CString code, CString gab);

	void		UpdateSaveList(CString savePath);
	bool		LoadOJCode();
	CString		FindAtmOption(double jisu);
	void		clearOPCodeList();
	void		GetHedgeCode(char month);
	BOOL		IsNumber(CString str);

	CString		GetPalette();
	void		DrawGuideBar(CDC* pDC, CRect rc);
	void		setMessage(char* format, ...);
	CBitmap*	getAxBitmap(CString path);
	void		DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);
	void		DrawBitmap(CDC *pDC, CRect rc, CString strImage);
	void		SetRoundWnd(CDC* pDc, int nSelTab);

	void		RunExcel(bool visible = true);
	bool		RunExcel_();

	void		OpenPriceCtrl(char* data);
	void		initFOMultiSel(CRect rect);

	CString		GetCtrlProperty(LPCSTR property);

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
	afx_msg void OnClickMingam();
	//afx_msg void OnClickJangoMode();
	//afx_msg void OnClickPortfolioMode();
	afx_msg void OnClickJango();
	afx_msg void OnClickOrder();
	afx_msg void OnClickSaveStg();
	afx_msg void OnAddItem();
	afx_msg void OnDelItem();
	afx_msg void OnDelAllItem();
	afx_msg void OnExcel();
	afx_msg void OnEdtEvent(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEditNotify( NMHDR * pHdr, LRESULT * result);
	afx_msg	void OnGridLMouseUp(NMHDR * pHdr, LRESULT * result);
	afx_msg void OnGridLMouseDown(NMHDR * pHdr, LRESULT * result);
	afx_msg void OnGridEndEdit(NMHDR * pHdr, LRESULT * result);
	afx_msg void OnGridEndSort(NMHDR * pHdr, LRESULT * result);
	afx_msg void OnTab1Change(NMHDR * pHdr, LRESULT * result);
	afx_msg void OnClickSearch();	//2012.07.23 KSJ ��ȸ��ư �߰�
	afx_msg void OnClickOrderBtn(); //2012.07.23 KSJ �ֹ���ư �߰�
	afx_msg void OnClickConfigBtn();//2012.10.05 KSJ �ֹ�â���� ��ư �߰�
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMsgPrice(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMsgLineMove(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMsgSave(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMsgAddStrategy(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMsgApplyStrategy(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMapHedgeData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

