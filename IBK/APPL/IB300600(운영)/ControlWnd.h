#pragma once

#include <set>
#include <string>
using namespace std;

#include "AxisExt.h"
#include "ControlWnd.h"
#include "SiseWnd.h"
#include "ScreenWnd.h"

#include "SpEdit.h"

#define	spNOT		0x00000000
#define	spFUTURE	0x00000002		// 선물
#define	spOPTION	0x00000004		// 옵션

#define	attrNUMERIC	0x00000001
#define	attrCOMMA	0x00000002
#define	attrFLOAT	0x00000004

#define	alCENTER	0
#define	alLEFT		1
#define	alRIGHT		2

#define	ssTEXT		0x00000001
#define	ssCOLOR		0x00000002
#define	ssIMAGE		0x00000004
#define	ssCONTRAST	0x00000008
#define	ssBORDER	0x00000010
#define ssGRADIENT	0x00000020

#include "../../h/jmcode.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxSpin.h"
#include "../../control/fx_misc/fxEdit.h"
#include "../../control/fx_misc/fxTab.h"
#include "grid/GridCtrl.h"
#include "dataio.h"

#define IDC_BTN_CHEG	(1001)
#define IDC_BTN_CHART	(1002)
#define IDC_BTN_JANGO	(1003)
#define IDC_GRID_JCNT	(1004)
#define IDC_RD_NONE		(1005)
#define IDC_RD_IOC		(1006)
#define IDC_RD_FOK		(1007)
#define IDC_CHK_MULTI	(1008)
#define IDC_BTN_FUT1	(1010)
#define IDC_BTN_FUT2	(1011)
#define IDC_BTN_FUT3	(1012)
#define IDC_BTN_FUT4	(1013)
#define IDC_BTN_FUT5	(1014)
#define IDC_BTN_OPT1	(1020)
#define IDC_BTN_OPT2	(1021)
#define IDC_BTN_OPT3	(1022)
#define IDC_BTN_OPT4	(1023)
#define IDC_BTN_OPT5	(1024)
#define IDC_BTN_FOLD    (1025)
#define IDC_CHK_OCLICK  (1026)
#define IDC_CHK_AUTOJVOL (1027)
#define IDC_GRID_JVOL   (1028)
#define IDC_BTN_JVOL    (1029)
#define IDC_CTL_TAB1	(1030)
#define IDC_GRID_CHEG	(1031)
#define IDC_BTN_CHEGLINK	(1032)
#define IDC_BTN_SIZE	(1033)
#define IDC_CHK_AUTO	(1034)
#define IDC_CHK_LOSS	(1035)
#define IDC_CHK_MARKET	(1036)
#define IDC_MAPVIEW		(1037)
#define IDC_GRID_OPT	(1038)
#define IDC_BTN_GEN		(1039)
#define IDC_BTN_MINI	(1040)
#define IDC_COMBO_CHAIK	(1041)
#define IDC_BTN_WEEK	(1042)  //week 
#define IDC_SPIN_MULTITRADE	(1043)  //week 
#define IDC_EDIT_JMVAL  (1044)

#define SIZE_GENERAL	0
#define SIZE_JANGO		1

#define NORMALBTN	0
#define MINIBTN		1
#define WEEKBTN		2

#include <math.h>
#include <vector>
#include <map>
using namespace std;

typedef struct tagOptionRow {
	bool atmg;
	char call_code[12];
	char hsga[12];
	char put_code[12];
} OptionRow;
typedef vector<OptionRow> OptionRows;
typedef map<int, OptionRows> OptionData;
typedef pair<int,int> OptionIdxRow;
typedef map<string, OptionIdxRow> OptionIdxMap;

class CControlWnd : public CWnd, public CAxisExt
{
public:
	CControlWnd(CWnd *pParent, AccountMap *pAccount, SiseData *pSiseData);
	int GetJqty();
	int GetCdgb();
	int GetMulti();
	int GetRsvd();
	void Clear();
	void ChangeTheme();
	void SetJvolPrice(int jprc);
	void SetJcnt( int jcnt, BOOL bPower = FALSE );	//2017.01.16 KSJ 그냥 단순히 수량만 바꾸고 싶을때 true
	void SetRealData(RealData *rp);
	void SetRealData( DWORD* data );
	void SetData(SiseData* data);
	void ClearChegGrid();
	void req_SONBQ101();
	bool GetMarket() { return m_chkMarket.GetCheck(); };
	void SetChegView(bool bView);
	void SetShowCurrTab(bool bCurr);
	void SetTabItems();
	void SetControlWindow();
	void SetControlWindowWEEK(bool bInit = false);
	void LoadMaster(bool bFlag = TRUE);
	void LoadMaster(int igubn = 0);
	void SetFOType(bool bFlag = TRUE);
	void SetFOType(int igubn);
	int  GetChaik();
	void SetWeelyBtnSHOW();
		
	bool	m_bMarketConfirm;
	bool	m_bFocusCheg;
	CGridCtrl *m_pJcntGrid, *m_pOptGrid, *m_pJvolGrid, *m_pChegGrid;
	OptionData m_dtOption;
	OptionData m_dtMOption;
	CScreenWnd* m_pScreenWnd;
	int m_nCurrTab;
	CString m_sAccnDept;

	CStringArray m_sArrTWeel;  //목요일
	CStringArray m_sArrMWeel; //월요일
//	CString m_sATMWeekCode;    
//	CString m_sATMWeekMCode; 
	bool	m_bClickInWeek;   //위클리 버튼 클릭한 상태에서 선물월물 버튼 클릭시... 일반버튼 전환되는거 막기 위해..

protected:
	CfxImgButton *m_pBtnCheg, *m_pBtnChart, *m_pBtnJango, *m_pBtnFold, *m_pBtnFuture[5], *m_pBtnOption[5], *m_pBtnJvol, *m_pBtnSize;
	CfxImgButton *m_pBtnChegLink;
//	CfxImgButton *m_pBtnGen, *m_pBtnMini;  //weekly
	CfxImgButton *m_pBtnGen, *m_pBtnMini, *m_pBtnWeek; 
	CfxStatic *m_pStJumun, *m_pStJcnt, *m_pStRsvd, *m_pStMstr, *m_pStFuture[4], *m_pStJvol;
//	CfxSpin *m_pSpJcnt, *m_pSpMulti, *m_pSpRsvd, *m_pSpAuto, *m_pSpLoss;
	CfxSpin *m_pSpMulti, *m_pSpRsvd, *m_pSpAuto, *m_pSpLoss;
	CSpEdit *m_pSpJcnt;
	CfxEdit *m_pEdJprc;
	CfxTab *m_pTab;
	CBitmap *m_hBtn2[3], *m_hBtn3[3], *m_hBtnLink[3], *m_hBtnSize[3], *m_hBtnLong[3];
	CBitmap m_hBk;
	CButton m_rdNone, m_rdIOC, m_rdFOK, m_chkMulti, m_chkOneClick, m_chkAutoJvol, m_chkAuto, m_chkLoss, m_chkMarket, m_chkChaik;
	HBRUSH m_hBodyBrush;

protected:
	AccountMap *m_pAccount;
	SiseData *m_pSiseData;
	CSiseWnd *m_pSise;
	CString m_profile;
	vector<struct fjcode> m_fjcode;
	struct ojcodh         m_ophead;
	vector<struct ojcode> m_opcode;
	vector<struct fjcode> m_mfcode; //미니선옵
	struct ojcodh         m_mohead; //미니선옵
	vector<struct ojcode> m_mocode; //미니선옵
	int m_dtOptionIdx;
	OptionIdxMap m_OptionMap;
	OptionIdxMap m_MOptionMap;
	BOOL m_bChegUpdate;
	int nSizeMode;
	BOOL	m_bChegView,m_bCurrView;
	CMapStringToString m_mapTab;
	BOOL	m_bKind;
	int  	m_iKind;
	int m_nCurFSelect;	//2015.12.01 KSJ 현재 선택된 선물 월물
	int m_nCurOSelect;	//2015.12.01 KSJ 현재 선택된 옵션 월물
	CString m_dept;
	//CToolTipCtrl* m_pTooltip;

protected:
	CfxImgButton* CreateImgButton(LPCSTR caption, int nID, CBitmap *bmp[]);
	void CalcSizeJango();
	void CalcSize();
	void LoadMiniMaster();
	void LoadConfig();
	void SetFutureData(int idx);	//2015.10.12 KSJ 선물도 선택되면 버튼이 눌려지도록 추가
	void SetOptionData(int idx);
	void SetWOptionData(int idx, bool bWeekclick = false);   //bWeekclick 위클리 버튼 클릭한경우 위클리 코드로 조회하도록 하기 위한 구분자 
	void SetOptionGrid();
	void SetChegGrid();
	void MakeBackground(int width=-1, int height=-1);
	void DisplayJango(Jango *pj);
	void DisplayAllJango();
	void DisplayMicheg(LPCSTR code);
	void Draw(CDC *pdc);
	void SetJvolGrid();
	void res_SONBQ101( LPCSTR data, int dlen );
	void OnBtn_Gen();
	void OnBtn_Mini();
	void OnBtn_Week();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnChkAuto();
	afx_msg void OnChkLoss();
	afx_msg void OnBtnSize();
	afx_msg void OnBtnCheg();
	afx_msg void OnBtnChart();
	afx_msg void OnBtnJango();
	afx_msg void OnBtnFold();
	afx_msg void OnChkMulti();
	afx_msg void OnChkOneClick();
	afx_msg void OnBtnJvol();
	afx_msg void OnFutBtnClick(UINT nID);
	afx_msg void OnPutBtnClick(UINT nID);
	afx_msg void OnTabChange(NMHDR *pHeader, LRESULT *pResult);
	afx_msg void OnChegLink();
	afx_msg void OnChkMarket();
	afx_msg void OnBtnGen();
	afx_msg void OnBtnMini();
	afx_msg void OnBtnWeek();
	afx_msg LRESULT OnRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
};

