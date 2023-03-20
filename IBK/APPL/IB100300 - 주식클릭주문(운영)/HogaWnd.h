#pragma once
// HogaWnd.h : header file
//

#include <map>
#include <set>
using namespace std;

#include "AxScrollBar.h"
#include "UserDefine.h"
#include <afxtempl.h>
#include <afxmt.h>
#include "../../control/fx_misc/fxButton.h"

#define	D10SHGA		0
#define D10HHGA		21
#define D10CHVL		22
#define D10BBTN		23	//2012.10.30 KSJ �ϰ���� ��ư

#define	D5SHGA		0
#define D5HHGA		11
#define D5CHVL		12
#define D5BBTN		13	//2012.10.30 KSJ �ϰ���� ��ư

#define MAX_COL		7
#define MAX_ROW		24

#define IDC_BTN_MADO_RSVD		(1003)
#define IDC_BTN_MADO_JUMN		(1004)
#define IDC_BTN_MADO_MAKET		(1005)
#define IDC_BTN_MASU_RSVD		(1006)
#define IDC_BTN_MASU_JUMN		(1007)
#define IDC_BTN_MASU_MAKET		(1008)
#define IDC_BTN_HOGA_FIX        (1009)

#define	bsTEXT		0x00000001
#define	bsCOLOR		0x00000002
#define	bsIMAGE		0x00000004	// 1 IMAGE, size DeflateRect(2, 2)
#define	bsGRAD		0x00000008	// gradiation

#define	bsPUSH		0x00000010	// push button
#define	bsFLATE		0x00000020	// flate button
#define	bsTOGGLE	0x00000040	// toggle button
#define	bsRECT		0x00000080	// draw rect only
#define	bsGENERAL	0x00000100	// general button

/////////////////////////////////////////////////////////////////////////////
// CHogaWnd window
class CHoga
{
public:
	CString mdga;		// �ŵ�ȣ��           
	CString msga;		// �ż�ȣ��           
	CString dvol;		// �ŵ�ȣ������       
	CString svol;		// �ż�ȣ������       
	CString pdvl;		// �����ŵ�ȣ������            
	CString psvl;		// �����ż�ȣ������
};

class COrder
{
public:
	CString code;		// �����ڵ�
	CString juno;		// ���ֹ� ��ȣ
	CString mmgb;		// �ŵ�/�� ����		1:�ŵ� 2:�ż�
	CString jprc;		// �ֹ�����
	CString jqty;		// �ֹ�����
	CString mcvl;		// ��ü�� ����
};

class CReserve
{
public:
	CString code;		// �����ڵ�
	CString name;		// �����
	char	mmgb{};		// �ŵ�/�� ����		1:�ŵ� 2:�ż� 
	int	gprc{};		// ���� ���ذ���
	int	jprc{};		// �ֹ�����
	int	ojprc{};		// ���ֹ� �ֹ�����
	int	jqty{};		// �ֹ�����
	int	juno{};		// ���ֹ���ȣ
	int	gubn{};		// ���ذ��� ���� (���簡���� ��/�Ʒ�)
	char sijang{};	//KSJ ���尡 �߰� ���尡�̸� 1 �ƴϸ� 0
};

class CArrow
{
public:
	int	ojno{};		// ���ֹ���ȣ
	int	jflg{};		// �ֹ� ����
	int	mmgb{};		// �ŵ�/�� ����		1:�ŵ� 2:�ż�
	int	oprc{};		// �� ����
	int	mprc{};		// ���� ����
};

class CHogaWnd : public CWnd
{
// Construction
public:
	CHogaWnd(class CTool* pTool, class CControlWnd* pCtrlWnd, CWnd* parent);


public:

protected:

	bool		m_bDragJumun{};	// �ֹ� �巡�� 
	bool		m_bDragResv{};	// ���� �巡��
	BOOL		m_bTracking{};	// ���콺 TRACCK

	int		m_iPosSHGA{};
	int		m_iPosHHGA{};
	int		m_iPosCHVL{};
	int		m_iPosBBTN{};	//2012.10.30 KSJ �ϰ���ҹ�ư
	int		m_iRows{};
	int		m_iMmgb{};		// �ֹ��� �ŵ�/�� ���� 1, 2
	int		m_iFocusMmgb{};	// ��Ŀ�� �ŵ�/�� ���� 1, 2
	int		m_iFocusIndex{};	// ��Ŀ�� �ε���
	int		m_aiCol[MAX_COL]{};
	int		m_iIndexTip{};
	int		m_iHogaStart{};	// ������� ���� ����

	double		m_width{};	// cell width
	double		m_height{};	// cell height
	
	CFont*		m_pfont{};
	CFont*		m_pfontB{};

	CPen*		m_ppenLine{};
	CPen*		m_ppenTable{};

	COLORREF	m_clrBG{};	// BackGround
	COLORREF	m_clrTH{};	// Table Head
	COLORREF	m_clrGH{};	// Grid Head
	COLORREF	m_clrSBG{};	// Sell BackGround
	COLORREF	m_clrBBG{};	// Buy BackGround
	COLORREF	m_clrLGFG{};	// Light Gray ForeGround
	COLORREF	m_clrUp{};	// Up, SangHan
	COLORREF	m_clrDown{};	// Down, HaHan
	COLORREF	m_clrGridHdrTxt{};	// 
	COLORREF	m_clrYellowBG{};	//�����
	COLORREF	m_clrNomalBG{};	//���


//	CString		m_strCODX;	// RTS �����ڵ�
public:
	CString		m_strShga;	// ���Ѱ�
	CString		m_strHhga;	// ���Ѱ�
    CString		m_strCurr;	// ���簡
	CString		m_strJijg;	// ��������
	CString		m_strGijun;	// ���ذ�
	CString		m_strSiga;	// �ð�
	CString		m_strKoga;	// ��
	CString		m_strJega;	// ���� 
	CString		m_strDiff;	// ���ϴ��
	CString		m_strKrgb;   //
	CString		m_strMkms;   //������ġ
protected:
	CString		m_strDvol;	// �ŵ�ȣ���Ѽ���
	CString		m_strSvol;	// �ż�ȣ���Ѽ���
    CString		m_strDcnt;	// �ŵ�ȣ���ѰǼ�
    CString		m_strScnt;	// �ż�ȣ���ѰǼ�
    CString		m_strDSCha;	// �ܷ���

	CRect		m_rcHoga[MAX_ROW];	// ȣ�� ���� 0:���� 21:���� 22:���ܷ� ADD 23:�ϰ���ҹ�ư
	CRect		m_rcDvol[MAX_ROW];	// �ŵ��ܷ�
	CRect		m_rcSvol[MAX_ROW];	// �ż��ܷ�
	CRect		m_rcMDJumun[MAX_ROW];// �ŵ��ֹ�
	CRect		m_rcMSJumun[MAX_ROW];// �ż��ֹ�
	CRect		m_rcMDResv[MAX_ROW];	// �ŵ�����
	CRect		m_rcMSResv[MAX_ROW];	// �ż�����
	CRect		m_rcHeadHoga;	// ��� ȣ��
	CRect		m_rcFocus;	// new ��Ŀ�� 


	CPoint		m_ptStart{};	// LButton down Point

	struct _config	m_config {};

	CHoga		m_hoga[10];
        
	CAxScrollBar	m_vbar;
	CToolTipCtrl*	m_pTooltip;
	std::unique_ptr<class CRectWnd>	m_pTrackingWnd;
	class CControlWnd*	m_pCtrlWnd;
	class CTool*		m_pTool;

	enum { KOSPI = 0, KOSDAQ, ELW, ETF }		m_eMarket;	// ���� ���� 
	enum { hgDEPTH10 = 0, hgDEPTH5 }	m_eDepth;	// 5/10ȣ�� ����
	enum { hgFOLD = 0, hgUNFOLD  }		m_eFdgb;		// �Ϲ�/���� ����


	CStringArray	m_arHoga;
	CMap<int, int, class COrder*, class COrder*>	m_mapOrder;
	std::map<int, std::unique_ptr<COrder>> m_Order;

	CArray<class CReserve*, class CReserve*>	m_arReserve;
//	CArray<class CArrow*, class CArrow*>		m_arArrow;

	CArray<std::shared_ptr<class CArrow>, std::shared_ptr<class CArrow>> m_arArrow;

	std::vector<std::unique_ptr<COrder>> _vOrder;

	CCriticalSection	m_cs;	// Synchronize m_mapOrder

	CString			m_sendS;
	CWnd*			m_pParent;

	bool		m_bNewHOGA = false; // 2023 ȣ�� ���� ���� ���а�
	std::unique_ptr<CfxButton> m_pMadoRsvd, m_pMadoJumn, m_pMadoMarket, m_pMasuRsvd, m_pMasuJumn, m_pMasuMarket, m_pHogaFix;
	set<HWND> m_HandCursorWnd;
	bool m_bHogaFix;
	bool m_bClearAll;

public:
	void		ReceiveSise(char* pcData, int iLen);
	void		ReceiveMiche(char* pcData, int iLen);
	void		ReceiveAlert(char* pcData);
	void		ReceiveAlert(struct _alertR* alertR);
	void		ReceiveNotice(char* pcData);

	void		Palette();
	
	
//	CString		SetComma(CString src);
//	void		Init(CControlWnd* pCtrlWnd);
	void		SetHogaDepth(int depth);
	void		DoModalConfigDlg();
	int		SetHogaReserve();
	void		ClearJumun();
	char		GetMarket();
	CPoint		GetRect2HogaPoint(CPoint point);
	void		ChangeFont(double* lRate, CFont* pfont, CFont* boldFont=NULL);
	int			GetRowheight(){return (int)m_height;};
	void		Click_Order(CPoint point);


	CString		m_sVIUp;      //����VI���� 
	CString		m_sVIDown;    //����VI����
protected:
	// Init
	int		Initialize();

	// Draw
	//void		DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat = 0, UINT type = 0, int clrIndex = COLOR_TEXT);
	void		DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat = 0, UINT nType = 0, COLORREF clrText = RGB(0, 0, 0));

	// To MapWnd
	CString		GetCODX();
	CString		GetAccount();
	CString		GetCondition();
	CString		GetPassword();
	CString		GetName();
	CString		GetCodeName();
	CString		GetListedQuantity(); //2014.06.23 KSJ �����ֽļ� ��������
	void		SetGuide(CString strMsg);
	void		SendJumun(char cMmgb, int iPrice, int iJuno = 0, int iJqty = 0, int bMarketPrice = 0);	//2012.11.07 KSJ ���尡�϶��� 1�� ��������
	int			GetPrgbType();

	// Calc
	CString		MakeSangHan(int iCurr);	// elw ���Ѱ��� ���� ���
	CString		MakeHaHan(int iCurr);
	CString		CalculateDiffTick(CString hoga);
	CString		CalculatePercent(CString hoga);
	int		GetPriceUnit(int iPrice, bool bPlus);
	int		GetTickUnit(int price, int mmgb, int count = 0);

	// Draw
	void		DrawHead(CDC* pDC);
	void		DrawHogaFold(CDC* pDC);
	void		DrawHogaUnfold(CDC* pDC);
	void		DrawReserve(CDC* pDC);
	void		DrawArrow(CDC *pDC);
	void		drawPgdg(CDC *pDC);	

	void		DrawArrowLine(CDC *pDC, CPoint StartPoint, CPoint EndPoint, bool bHolder = true);
	void		DrawArrowLine2(CDC *pDC, CPoint StartPoint, CPoint ViaPoint, CPoint EndPoint, bool bHolder);
	void		DrawArrowLine3(CDC *pDC, CPoint StartPoint, CPoint EndPoint, int mmgb = 3);
	void		DrawColorBox(CDC* pDC, CRect rc, BOOL l = TRUE, BOOL t = TRUE, BOOL r = TRUE, BOOL b = TRUE);
	void		DrawArrowBox(CDC *pDC, CPoint StartPoint);	//2012.11.15 KSJ ���尡 �϶� �׸���

	//  etc
	void		CreateHogaSet();
	double		Str2double(CString string, bool bABS = true);
	int		Str2int(CString string, bool bABS = true);
	void		SetHogaRect();
	void		CreateScroll();
	void		SetScroll(int nHoga);
	int		GetKey(int value);
	CString		GetHogaData(CString hoga, int dsgb);
	int		GetIndexToHoga(int index);
	CString		GetHogaToJumun(CString hoga, int mmgb, CRect rc, bool bTotal = false);
	CString		GetReserveData(CString hoga, int mmgb, CRect rc, bool bTotal = false);
	int		GetPointToIndex(CPoint point, int cell, int& mmgb);
	void		RemoveJumun(int index);
	void		RemoveAllJumun();
	void		RemoveAllReserve();
	void		RemoveAllArrow();
	bool		GetConfirmJumun();
	bool		CheckJumunAvailable(bool bCheckJmcn = true);
	int		GetJumunCount(CString junos, int mmgb);
	
	
	void		GetPointReserve(class CReserve* pResv, CPoint &ptStart, CPoint &ptEnd);
	void		GetPointArrow(class CArrow* pArrow, CPoint &ptStart, CPoint &ptEnd);
	void		CheckReserveJumun(CString code, CString curr);
	void		SetModArrow(int jflg, int price, int juno);

	// 2012.11.05 KSJ �߰�
	void AllJumnCancel(int nMmgb);	//0 ��������(��������), 1 �ŵ�, 2, �ż�
	void AllRsvdCancel(int nMmgb);	//0 ��������(��������), 1 �ŵ�, 2, �ż�
protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHogaWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString SplitString(CString &strData, CString strToken);
	virtual ~CHogaWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHogaWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnHogaMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAllMadoCancel();
	afx_msg void OnAllMasuCancel();
	afx_msg void OnAllMadoRsvdCancel();
	afx_msg void OnAllMasuRsvdCancel();
	afx_msg void OnHogaFix();
	afx_msg void OnMarketMado();
	afx_msg void OnMarketMasu();
	DECLARE_MESSAGE_MAP()
};

