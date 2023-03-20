#pragma once
// MapDlg.h : header file
//
#include "resource.h"
#include "category.h"

#include "../../h/interst.h"

#include "../../control/fx_misc/fxCodeCtrl.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_grid/fxGrid.h"
#include "NewMenu.h"	// ADD PSH 20070912


const	int maxVIEW =	4;

struct	_gridHdr {
	UINT	stid;		// string table id
	UINT	width;		// fixed width
	UINT	attr;		// fixed attribute

	char	symbol[8];	// nonfixed symbol
	UINT	format;		// nonfixed format
	UINT	attrx;		// nonfixed attr

	UINT	fidx;		// field index
};

/////////////////////////////////////////////////////////////////////////////
// CMapWnd dialog
#define ROUND_PANEL		1
#define ROUND_CONTENTS	2
#define P_DELI	0x7f
#define P_TAB	0x09
#define P_NEW	0x0a
#define P_PER	0x25
#define MAXROW	200
class CMapWnd : public CWnd
{
	// Construction
public:
	CMapWnd(CWnd* pParent);   // standard constructor


public:
	CWnd* m_parent{};
	CString		m_root;
	CString		m_name;

	CFont*		m_font{};
	CFont*		m_fontB{};

	CComboBox	m_internm[maxVIEW];
	bool		m_waiting[maxVIEW]{};
	std::unique_ptr <CCategory>	m_category[maxVIEW]{};
	bool		m_isTimer[maxVIEW]{};

	CfxImgButton	m_hawk;
	CfxImgButton	m_expect;
	CfxImgButton	m_config;
	CfxImgButton	m_minus;
	CfxImgButton	m_plus;

	int		m_gridW{};
	int		m_rowC{}, m_colC{};
	std::unique_ptr<class CfxGrid>	m_grid[maxVIEW]{};

	CArray < _gridHdr, _gridHdr > m_gridHdrX;
	CArray <std::shared_ptr<_inters>, std::shared_ptr<_inters>> m_inters[maxVIEW];

	enum _viewType {viewTWO = 2, viewTHREE = 3, viewFOUR = 4 } m_viewType;

	CBrush		m_bkBrush;
	int			m_fontsize{};
	COLORREF	m_blinkColor{};
	COLORREF	m_bookmarkColor{};

	HBITMAP		m_bitmap2_nm{}, m_bitmap2_dn{}, m_bitmap2_hv{};
	HBITMAP		m_bitmapM_nm{}, m_bitmapM_dn{}, m_bitmapM_hv{};
	HBITMAP		m_bitmapP_nm{}, m_bitmapP_dn{}, m_bitmapP_hv{};
	HBITMAP		m_bitmapH_nm{}, m_bitmapH_dn{}, m_bitmapH_hv{};

	CCriticalSection m_sync;

	bool		m_blink{};

	MINMAXINFO	m_MMI{};

	int			m_irowCode[maxVIEW][MAXROW]{};		//코드가 존재하는 라인
protected:
	BOOL	create(CWnd* parent);

public:
	int		GetCodeType(CString code);

	BOOL	Create();
	void	plusminusClick(UINT id);

	void	sendTransaction(int vidx);
	void	parsingOubs(char* datB, int datL, int vidx);
	void	parsingAlert(CString datB);
	void	parsingAlertx(LPARAM lParam);	//2012.01.19 KSJ Alertx 추가
	void	parsingTrigger(CString datB);

	void	initControl();
	void	initialGrid(int size = true);
	
	void	loadGridinfo();
	int     _httoi(const TCHAR *value);

	int		loadInterName(bool bcursel = true);
	void	loadFieldInfo();
	void	toggleAction(bool toggle);
	int		loadInterest(int vidx, int gno);
	int		loadInterest2(int vidx, int gno);
	int		loadInterest(int vidx, CString codes);
	void	saveInterest(int vidx, int gno);
	void	clearInterest(int vidx);
	bool	existFile(int gno);
	int		getCodeType(CString code);
	void	writeVtCode(int cx, int cy);

	int		checkRealTimeCode(CString code, char* xrow);
	int		getFlashIndex();

	void	repositionView();
	void	reloadscreen(CString fname = _T(""), int fsize = 0);

	void	saveMF();
	void	restoreMF();

	void	loadGroupInfo();
	void	saveGroupInfo();

	void	hawkeyeAction(bool toggle);
	void	hawkeyeAction(int vidx);
	void	hawkeyeClear(int vidx);
	void	hawkEyeColor(int vidx, int row);

	bool	IsBlink();


	void	Request_GroupList();
	void	Request_GroupCode(int iseq, int vidx=0);
	void	receiveOub(int key, CString data, int vidx=0);

	void	ChangeInterNM(int vidx);
protected:
	CString parseX(CString &srcstr, CString substr);
	CString	Variant(int comm, CString data);
	int	openView(int type, CString data);
	void	sendTR(CString trCode, char* datB, int datL, int key = 0);
	CFont*	getAxFont(CString fName = "굴림체", int point = 9, int style = 0);
	CBitmap* getAxBitmap(CString path);
	COLORREF GetColor(int color);
	double	TOfabs(CString string);
	double	TOf(CString string);
	CString	idTOstring(UINT id);
	CString	charTOstring(char* datB, int datL);
	void SendMinMaxInfo();

public:		// RTM
	CString m_strPal;
	int		m_nOldRow{};
	CArray<char *, char *>	m_arBytesRTM;
	CArray<LPARAM , LPARAM >	m_arBytesRTMx;	// 2012.02.13 KSJ
	CCriticalSection	m_syncRTM;

	BOOL	m_bRemove = false;		// 2011.12.29 KDK
	BOOL    m_isClose = false;

private:	// RTM
	HANDLE	m_hEventRTM;

	void Draw(CDC *pDC);
	void DrawBitmap(CDC *pDC, CRect rc, CString strImage);
	void DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);
	CString GetPalette();

	BOOL OnInit();
	void RbuttonAction(CfxGrid *grid,int row);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL
public:
//	int scrollW;
	virtual ~CMapWnd();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeInterNM0();
	afx_msg void OnSelchangeInterNM1();
	afx_msg void OnSelchangeInterNM2();
	afx_msg void OnSelchangeInterNM3();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg LONG OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnParent (WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

