#pragma once

// depth.h : header file
//

#include <afxmt.h>
#include <afxtempl.h>
#include "afxole.h"

#define CeilEX(x,dig) (floor((x)*pow(10,dig)+0.5)/pow(10,dig))

class Cdepth;

/////////////////////////////////////////////////////////////////////////////
// COleEditDropTarget
class CDropTarget : public COleDropTarget
{
// Construction
public:
	CDropTarget();
	virtual ~CDropTarget();

	Cdepth*	m_pDepth;

// Implementation
public:
	CString Parser(CString &srcstr, CString substr);
	void	GetDepth(Cdepth* cWnd);
    
	//
	// These members MUST be overridden for an OLE drop target
	// See DRAG and DROP section of OLE classes reference
	//
	DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD 
		dwKeyState, CPoint point );
	DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD 
		dwKeyState, CPoint point );
	void OnDragLeave(CWnd* pWnd);               
	
	BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT 
		dropEffect, CPoint point );    
};

class Citem
{
public:
	CString	m_data;
	bool	m_center = false;
	int	m_attr = 0;

	CRect	m_fRc;
	CRect	m_bRc;

	COLORREF m_fRGB = 0;	// pen
	COLORREF m_pRGB = 0;	// bground
};

struct _config
{
	unsigned char	info;		// 1�� �ΰ�����(1��и� : �ð�����/�Ǻ�)
	unsigned char	mbong;		// 2�� �ΰ�����(3��и� : ��Ʈ/�ŷ���)
	unsigned char	curr;		// ���簡 Ʈ���̽�
	unsigned char	sgrp;		// ü�ᰭ�� �׷���
	unsigned char	hdyul;		// �����
	unsigned char	hbold;		// ȣ�� ����
	unsigned char   sbong;		// ���� ����Ʈ
	unsigned char	pre;		// ����ü��
	unsigned char	currset;	// ���簡 ����	
	unsigned char	shl;		// �ð���ǥ��
	unsigned char	hrbold;		// ȣ���ܷ� ����
	unsigned char	hcbold;		// ȣ���Ǽ� ����
	char		reserved[27];
};
#define	sz_CONFIG	sizeof(struct _config)

// attr
#define atNormal	0		// �Ϲ����� ���� (0 ����)
#define atNoColor	1		// �Ϲ����� ���� (0 ����, �����ǥ��)
#define atSignSup	2		// Normal���� ���α�ȣ ����
#define atArrow		3		// ���� �� (+ ��� ����ǥ��)
#define atCoRgb		4		// + - �� ���� ������ ǥ��
#define atCoRgbSup	5		// ���� ������ ���α�ȣ ����
#define at2SignZero	6		// 2Sign, 0ǥ��
#define atPercent	7		// % ��ȣ �ڿ� ���̱�, ���� �� (+ ��� ����ǥ��)
#define atTime		8		// �ð�
#define	atComma		9		// ������ ������ ��ġǥ��

/////////////////////////////////////////////////////////////////////////////
// Cdepth window

class Cdepth : public CWnd
{
// Construction
public:
	Cdepth();
	virtual ~Cdepth();

protected:
	CString	m_class;
	CWnd*	m_parent;
	std::unique_ptr<class CToolTip> m_pToolTip;

	CString m_szName;
	int	m_key;
	
	CRect	m_rect;
	std::unique_ptr<CRgn>	m_pRgn;
	CString	m_options;

	CFont*	m_font;
	CString	m_fonts;
	int	m_point;
	int	m_style;

	int	m_fontH;

	// 2012.08.24 ����� - LP MODE ���� ����
	bool	m_bLPMode;

	CString	m_code;
//	CPtrArray m_items;
//	CArray	<struct _sisememo*, struct _sisememo*>	m_sisememo;
	CArray	<std::shared_ptr<Citem>, std::shared_ptr<Citem>> m_items;
	CArray	<std::shared_ptr<struct _sisememo>, std::shared_ptr<struct _sisememo>>	m_sisememo;
	CMap	< int, int, int, int& > m_indices;

	int	m_columns[3];		// column width
	double	m_rowH;			// row height
	double	m_OneMoreRowH;		// row height + 1 // 5ȣ������ ü�ᰭ�� �߰��� ����
	int	m_topH;			// header height

	CCriticalSection m_sync;
	BOOL	m_tracking;		// mouse tracking
	CRect	m_prect;		// price & percent rect
	CRect	m_rcConfig;		// button rect for Config
	CRect	m_rcConfigL;		// button rect for Config for Left
	CRect	m_rcConfigR;		// button rect for Config for Right
	CRect	m_rcTime;		// �ð��� / ����ü��
	CRect	m_rcInfo;		// �ΰ����� rect
	CRect	m_rcCurr;		// ���簡 Rect
	CRect	m_rcPriceTip;		// ���� Tip Rect
	BOOL	m_bLButtonDown;		// if lbutton down

	int	m_focus;		// focus rect index
	double	m_maxsize;		// max volumn for bong graph

	double	m_curr;			// last price
	double	m_clos;			// previous close price
	double	m_ma5;
	double	m_ma10;

	int	m_idxMDS;
	int	m_idxMDE;
	int	m_idxMSS;
	int	m_idxMSE;

	int	m_nHighPercent;
	int	m_nLowPercent;
	BOOL	m_bPercent;

	int	m_depth;						// 5 or 10 depth
	bool	m_alwaysTEN;
	enum	_ctype	{ ctNo=0, ctCode, ctIndex, ctFuture, ctOption, ctECN, ctKOption, ctKOFEX } m_type;
	bool	m_showBeforeDiff;					// �������
	enum	_graph	{ grNo=0, grBar, grBarText } m_bar;		// bar graph
	bool	m_showPreValue;						// ���� ü�ᰡ, ���� ü���
	bool	m_showTop;						// ��� �÷���
	enum	_bottom	{ btNo=0, btSize, btTime, btSizeTime } m_bottom;// �ϴ� ���ܷ� ǥ��
	bool	m_showLine;						// Line ǥ��
	enum	_act	{ acNo=0, acPopup, acMsg } m_action;		// LButton Down Action
//	enum	_showInfo { infoNo = 0, infoSiga, infoPivot, infoCurr } m_showOHLC;		// �ð����� ǥ��
	enum	_showInfo { infoNo = 0, infoSiga, infoPivot, infoCurr,  infoVI} m_showOHLC;	// �ð����� ǥ��
	bool	m_bGradient;						// Gradient
	bool	m_bSiseMemo;						// �ü��޸�
	enum	_percent { pcNo = 0, pcSiga, pcKoga, pcJega, pcJgga, pcUser } m_percent;
	enum	_bong { bongNo = 0, bongBong, bongChgl } m_bong;	// MBong ǥ��
	int	m_nUDPercent;						// �������� ���Դܰ�(����� ���ذ�)
	bool	m_bBong;						// ���Ϻ� ǥ��
	bool	m_bCurr;						// ���簡 ���� ǥ��
	bool	m_bBoldPrice;						// ȣ�� BOLD
	bool	m_bBoldRemain;						// ȣ���ܷ� BOLD
	bool	m_bBoldCnt;						// ȣ���Ǽ� BOLD
	bool	m_bPredict;						// ����ü�� / �ð���
	bool	m_bConfig;						// ���簡ȭ�� (������ư ����)
	CString	m_file;
	CString m_configFile;
	bool	m_bSiga;
	CString	m_refsym;						// reference symbol
	CString m_sDebug;

	struct _config	m_config;
	CString	m_path;
	CBitmap	*m_pBitmapC1, *m_pBitmapC2, *m_pBitmapP1, *m_pBitmapP2;

	COLORREF	m_clrWhite;
	COLORREF	m_clrHeadFg, m_clrHeadBk, m_clrDataFg;
	COLORREF	m_clrAskGs, m_clrAskGe, m_clrBidGs, m_clrBidGe;
	COLORREF	m_clrAskSzFg, m_clrBidSzFg;
	COLORREF	m_clrLine, m_clrDash, m_clrBox, m_clrBottom;
	COLORREF	m_clrUp, m_clrDown;
	COLORREF	m_clrFocus, m_clrCurr;
	COLORREF	m_clrBarAsk, m_clrBarBid;
	bool	m_bBigDigit;
	bool	m_bAble;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cdepth)
	public:
	virtual void OnFinalRelease();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	CDropTarget	m_dropTarget;

	BOOL	m_b1801;
	LPCTSTR m_mapName;
	BOOL	m_bDragMode;
	double	m_nPriceH;
	double	m_nPriceL;
	
	CWnd*	GetParent() { return m_parent; }
	CString calculatePercentByIndex(int idx);
	CString AddTipStr(int tp, CString str1, CString str2);
	CString GetHogaTip(CString hog);
	int hogaTypeToInt(CString hog);
//	CString hogaTypeToString(CString hog);
	boolean bELW;
	BOOL	Create(CWnd* parent, void* ptr);

	COLORREF getColor(int color);
	CPen*	 getPen(int color, int width = 1);
	CBrush*  getBrush(COLORREF color);
	CFont*	 getFont(int point, CString name, int bold = 0, bool italic = false);
	CBitmap* getBitmap(CString path);

	void	TipMouseMove(CPoint point);
	void	MouseMove(bool bIsTipEvent, CPoint point, bool bCrossOnly = false);
	CString	Variant(int comm, CString data);
	void	trigger(CString param);

protected:
	void	initialColor();
	void	initialize();
	void	setIndices();

	void	dispatch(char* datB, int datL);
	void	dispatchTEN(char* datB, int datL);
	void	dispatchFIVE(char* datB, int datL);
	void	dispatchMEMO(char* datB, int datL);
	void	alert(CString data);
	void	alert(struct _alertR* alertR);
	void	freeItems();
	void	freeSiseMemo();
	void	clear();
	void	WriteLog(CString Data);

	void	draw(CDC* dc);
	void	drawItems(CDC* dc);
	void	drawInfo(CDC* dc);
	void	drawGraph(CDC* dc, int index);
	void	drawHead(CDC* dc);
	void	drawBottom(CDC* dc);
	void	drawLine(CDC* dc);
	void	drawView(CDC* dc);
	void	drawBground(CDC* dc);
	void	drawSiseMemo(CDC* dc);
	void	drawBong(CDC* dc);
	void	drawButton(CDC *pDC);
	void	drawButton(CDC *pDC, CRect rc, CBitmap *pBitmap);
	void	drawBox(CDC *pDC);
	void	drawBongX(CDC* pDC);

	void	calculateColumnWidth();
	void	calculateRowHeight();
	COLORREF SetItemBGColor(int index);

	void	getMaxSize();
	void	setItemRect();
	void	setItemRect3();
	void	setItemRect5();
	void	setItemRect10();
	void	getPriceRect();
	CRect	getPercentRect();
	void	setFocus(CPoint point);
	int	getIndexByPoint(CPoint point);
	CString	getOrderPrice(CPoint point);
	void	calculatePercent();
	void	calculatePreValue();

	void	popupMenu(CPoint point, CString Price);
	void	sendMessage(CPoint point, CString Price);
	int	openView(int type, CString param);


	CString	format(CString data, int index);
	void	addComma(CString& data);
	CString	getSign(CString& data);
	void	eliminateZero(CString& data);
	double	str2double(CString string);
	bool	IsELW(CString code);

	BOOL	createFont();
	void	parseOptions();

	void	SendSiseMemo();
	void	SendTR(CString name, CString data, BYTE type, int key);
	void	SendTR(CString name, char* datb, int datl, BYTE type, int key);
	
	CString	GetDataString(CString keys, CString init);
	int	GetDataInt(CString keys, int init);
	CString	GetTooltipStr(int item);
	CString	GetBongStr();

	void	InitEnv();
	void	ConfigDlg();
	void	ConfigDlgL();
	void	ConfigDlgR();
	bool	ReadCondition(CString path, struct _config *pConfig);
	void	SaveCondition(CString path, struct _config *pConfig);
	bool	readFile(CString path, char *pBytes, int nBytes);
	bool	writeFile(CString path, char *pBytes, int nBytes);
	void	clickAction(CRect rect, CPoint point);

	///////////////////////////////////////////////////
	// Extra Window
	bool			m_bMBPrice;
	bool			m_bMBVolume;
	std::unique_ptr<class CContractWnd>	m_pContract;
	std::unique_ptr<class CMBongWnd>	m_pMBong;

	void	EW_Init();
	void	EW_Clear();
	void	EW_Destroy();
	bool	EW_Create();
	void	EW_Move();
	void	EW_Show();
	void	EW_Font();
	void	EW_Palette();
	void	EW_Dispatch(char *dataB, bool bTEN);
	void	EW_Alert(CString data);
	void	EW_Alert(struct _alertR* alertR);
	///////////////////////////////////////////////////

	//{{AFX_MSG(Cdepth)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMBong(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(Cdepth)
	afx_msg long _getType();
	afx_msg void _setType(long nNewValue);
	afx_msg long _getHogaType();
	afx_msg void _setHogaType(long nNewValue);
	afx_msg long _getBeforeCompare();
	afx_msg void _setBeforeCompare(long nNewValue);
	afx_msg long _getPercent();
	afx_msg void _setPercent(long nNewValue);
	afx_msg long _getGraph();
	afx_msg void _setGraph(long nNewValue);
	afx_msg long _getTopDisplay();
	afx_msg void _setTopDisplay(long nNewValue);
	afx_msg long _getBottomDisplay();
	afx_msg void _setBottomDisplay(long nNewValue);
	afx_msg long _getLine();
	afx_msg void _setLine(long nNewValue);
	afx_msg long _getLButtonDown();
	afx_msg void _setLButtonDown(long nNewValue);
	afx_msg long _getOHLC();
	afx_msg void _setOHLC(long nNewValue);
	afx_msg BOOL _GetVisible();
	afx_msg void _SetVisible(BOOL bNewValue);
	afx_msg short _GetBongStatus();
	afx_msg void _SetBongStatus(short nNewValue);
	afx_msg short _GetCurrStatus();
	afx_msg void _SetCurrStatus(short nNewValue);
	afx_msg short _GetInfo();
	afx_msg void _SetInfo(short nNewValue);
	afx_msg BOOL GetBoldRemain();
	afx_msg void SetBoldRemain(BOOL bNewValue);
	afx_msg BOOL GetBoldRemainCount();
	afx_msg void SetBoldRemainCount(BOOL bNewValue);
	afx_msg void _SetProperties(LPCTSTR options);
	afx_msg BSTR _GetProperties();
	afx_msg void _Refresh();
	afx_msg void InitAmtSet();
	afx_msg void InitSiseMemoSet();
	afx_msg short _ChangeInfo();
	afx_msg short ChangeBong();
	afx_msg void SetLPMode(BOOL flag);
	afx_msg void SetPriceRange(long high, long low);
	afx_msg void SetPercent(short high, short low);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
	CString Parser(CString &srcstr, CString substr);
	void	SearchChegang();
};