#pragma once
// HogaWnd.h : header file
//
#include "MapWnd.h"
#include "AxScrollBar.h"
#include "ControlWnd.h"
//#include <afxtempl.h>
//#include <afxmt.h>

#include <map>
#include <vector>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
using namespace std;

/*
#define	D10SHGA		0
#define D10HHGA		21
#define D10CHVL		22

#define	D5SHGA		0
#define D5HHGA		11
#define D5CHVL		12

#define MAX_COL		7
#define MAX_ROW		23
*/
#define	D10SHGA		0
#define D10HHGA		21
#define D10CHVL		22
#define D10BTN		23

#define	D5SHGA		0
#define D5HHGA		11
#define D5CHVL		12
#define D5BTN		13

#define MAX_COL		7
#define MAX_ROW		24


/////////////////////////////////////////////////////////////////////////////
// CHogaWnd window
class CHoga
{
public:
	CString mdga;		// �ŵ�ȣ��           
	CString msga;		// �ż�ȣ��           
	CString dvol;		// �ŵ�ȣ������       
	CString svol;		// �ż�ȣ������       
	CString dcnt;		// �����ŵ�ȣ������            
	CString scnt;		// �����ż�ȣ������
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
	int	mmgb{};		// �ŵ�/�� ����		1:�ŵ� 2:�ż�
	double	gprc{};		// ���� ���ذ���
	double	jprc{};		// �ֹ�����
	double  ojprc{};		// ���ֹ� �ֹ�����
	int	juno{};		// ���ֹ���ȣ
	int	jqty{};		// �ֹ�����
	int	gubn{};		// ���ذ��� ���� (���簡���� ��/�Ʒ�)
	int	tick{};		// ƽ
	int index{};      // �ε���
	bool market{};	//���尡�ֹ� ����
};

class CArrow
{
public:
	int	ojno{};		// ���ֹ���ȣ
	int	jflg{};		// �ֹ� ����
	int	mmgb{};		// �ŵ�/�� ����		1:�ŵ� 2:�ż�
	double	oprc{};		// �� ����
	double	mprc{};		// ���� ����
};

class CHogaWnd : public CWnd
{
// Construction
public:
	CHogaWnd(CWnd* pParent, CWnd* pView);

public:
	void DrawScreen();

public:
	CMapWnd* m_pParent{};
	CWnd* m_pView{};
	CControlWnd* m_pCtrlWnd{};

	std::unique_ptr <CExButton> m_mdStopBtn{};
	std::unique_ptr <CExButton> m_mdJmBtn{};
	std::unique_ptr <CExButton> m_msJmBtn{};
	std::unique_ptr <CExButton> m_msStopBtn{};

	std::unique_ptr <class CRectWnd> m_pTrackingWnd{};

	CFont* m_pFont{};
	CFont* m_pBFont{};

	CString		m_home;
	CString		m_user;
	CString		m_file;
	CString		m_code;
	CString		m_mouseDLL;

	struct _config	m_config {};

	enum { FUTURE = 0, OPTION }		m_market;	// ����/�ɼ� ����
	enum { hgDEPTH10 = 0, hgDEPTH5 }	m_depth;	// 5/10ȣ�� ����
	enum { hgFOLD = 0, hgUNFOLD  }		m_fdgb;		// �Ϲ�/��Ÿ� ����


	int		m_posSHGA{};
	int		m_posHHGA{};
	int		m_posCHVL{};
	int		m_posBTN{};
	int		m_rows{};
	double		m_width{};	// cell width
	double		m_height{};	// cell height

	int		m_col[MAX_COL]{};

	CHoga		m_hoga[5]{};

	CString		m_shga;		// ���Ѱ�
	CString		m_hhga;		// ���Ѱ�

        CString		m_curr;	        // ���簡
	CString		m_jijg;		// ��������
	CString		m_pivot;	// ���ذ�
	CString		m_siga;		// �ð�
	CString		m_koga;		// ��
	CString		m_jega;		// ���� 

   	CString		m_gsm4;		// 4�� ������
	CString		m_gsm9;		// 9�� ������

	CString		m_diff;		// ���ϴ��

	CString		m_jggy;		// ���ű���
	CString		m_res2;		// 2������
	CString		m_res1;		// 1������
	CString		m_pivt;		// �Ǻ���
	CString		m_spp1;		// 1������
	CString		m_spp2;		// 2������ 
	CString		m_resd;		// D����
	CString		m_sppd;		// D����   

	CString		m_dvol;         // �ŵ�ȣ���Ѽ���
	CString		m_svol;         // �ż�ȣ���Ѽ���
    CString		m_dcnt;         // �ŵ�ȣ���ѰǼ�
    CString		m_scnt;         // �ż�ȣ���ѰǼ�
    CString		m_dscha;	// �ܷ���
        
	double		m_hogaStart{};	// ������� ���� ����
	CPoint		m_ptStart{};	// LButton down Point
	int		m_mmgb{};		// �ֹ��� �ŵ�/�� ���� 1, 2
	int		m_focusMmgb{};	// ��Ŀ�� �ŵ�/�� ���� 1, 2
	int		m_focusIndex{};	// ��Ŀ�� �ε���
	bool		m_bDragJumun{};	// �ֹ� �巡�� 
	bool		m_bDragResv{};	// ���� �巡��
	bool		m_bOneClick{};	// ��Ŭ�� �ֹ�
	bool		m_bClickMod{};	// ��Ŭ�� ����
	BOOL		m_bTracking{};	// ���콺 TRACCK

	CStringArray	m_arHoga;
	CMap<int, int, class COrder *, class COrder *>	m_mapOrder;
	CArray<class CReserve *, class CReserve *>	m_arReserve;
	CArray<class CArrow *, class CArrow *>		m_arArrow;

	CCriticalSection	m_cs;		// synchronize m_mapOrder
//	CCriticalSection	m_csResv;	// synchronize Reserve Order

	CAxScrollBar	m_vbar;
	std::unique_ptr <CToolTipCtrl> m_tooltip{};
	int		m_indexTip{};
/*
	CRect		m_rcHoga[23];	// ȣ�� ���� 0:���� 21:���� 22:���ܷ�
	CRect		m_rcDvol[23];	// �ŵ��ܷ�
	CRect		m_rcSvol[23];	// �ż��ܷ�
	CRect		m_rcMDJumun[23];// �ŵ��ֹ�
	CRect		m_rcMSJumun[23];// �ż��ֹ�
	CRect		m_rcMDResv[23];	// �ŵ�����
	CRect		m_rcMSResv[23];	// �ż�����
*/
	CRect		m_rcHoga[24];	// ȣ�� ���� 0:���� 21:���� 22:���ܷ�
	CRect		m_rcDvol[24];	// �ŵ��ܷ�
	CRect		m_rcSvol[24];	// �ż��ܷ�
	CRect		m_rcMDJumun[24];// �ŵ��ֹ�
	CRect		m_rcMSJumun[24];// �ż��ֹ�
	CRect		m_rcMDResv[24];	// �ŵ�����
	CRect		m_rcMSResv[24];	// �ż�����
	CRect		m_rcHeadHoga;	// ��� ȣ��
	CRect		m_rcFocus;	// ��Ŀ�� 

	bool		m_bReal{};

	CRect		m_rcWnd;
	/*
	struct	_reserve	{
		CString	code;
		CString	curr;
	};
	CArray <_reserve, _reserve>	m_rtsReserve;
	*/

	
public:
	/* callback-function Timer�� �̿��ϱ� ����. event->function */
	CString		m_rsvCode;
	CString		m_rsvCurr;

public:
	bool		m_bHold;
public:
	bool bAlertClose;
	CCriticalSection m_alertcs;
	HANDLE m_alertevt, m_alertth;
	vector<int> m_alertmsg;

public:
	void		drawHead(CDC* pDC);
	void		drawHogaFold(CDC* pDC);
	void		drawHogaUnfold(CDC* pDC);
	void		drawReserve(CDC* pDC);
	void		drawArrow(CDC *pDC);
	void		drawArrowLine(CDC *pDC, CPoint StartPoint, CPoint EndPoint, bool bHolder = true);
	void		drawArrowLine2(CDC *pDC, CPoint StartPoint, CPoint ViaPoint, CPoint EndPoint, bool bHolder);
	void		drawArrowLine3(CDC *pDC, CPoint StartPoint, CPoint EndPoint, int mmgb = 3);
	void		DrawStar(CDC *pDC, CPoint StartPoint, CPoint EndPoint, int mmgb = 3);
	void		DrawColorBox(CDC* pDC, CRect rc, BOOL l = TRUE, BOOL t = TRUE, BOOL r = TRUE, BOOL b = TRUE);

	double		getPriceUnit(double price, bool bFuture, bool bPlus);
	double		getTickUnit(double price, bool bFuture, int mmgb, int count = 0);
	void		createHogaSet();
	double		str2double(CString string, bool bABS = true);
	int		str2int(CString string, bool bABS = true);
	void		setHogaRect();
	void		createScroll();
	void		setScroll(int nHoga);
	int		getKey(double value);
	CString		getHogaData(CString hoga, int dsgb);
	double		getIndexToHoga(int index);
	CString		getHogaToJumun(CString hoga, int mmgb, CRect rc, bool bTotal = false);
	CString		getReserveData(CString hoga, int mmgb, CRect rc, bool bTotal = false);
	int		getPointToIndex(CPoint point, int cell, int& mmgb);
	int		getHogaToIndex(double hoga);
	void		removeJumun(int index);
	void		removeAllJumun();
	void		removeAllReserve();
	void		removeAllArrow();
	bool		getConfirmJumun();
	bool		checkJumunAvailable(int mmgb = 1);
	int		getJumunCount(CString junos, int mmgb);
	int		getMichegCount();
	CString		calculatePercent(CString hoga);
	CString		calculateDiffTick(CString hoga);
	void		getPointReserve(class CReserve* pResv, CPoint &ptStart, CPoint &ptEnd);
	void		getPointArrow(class CArrow* pArrow, CPoint &ptStart, CPoint &ptEnd);
public:
	/* callback-function timer �̿��� ���� public�޼ҵ�� ������ */
	void		checkReserveJumun(CString code, CString curr);
public:
	void		SetModArrow(int jflg, double price, int juno);
	void		OneClickJumun(CPoint point);
	void		ClickMod(CPoint point);

	void		invalidateRC();
	void		onBtn(int mmgb, bool bReserve);

public:
	void		DispatchData(char* pData, int len);
	void		DispatchMicheg(char *pData, int len, bool bErase=true);
	void		DispatchAlertX(struct _alertR* alertR);
	void		DispatchNotice(CString Notice);
	void		DispatchJango(CString Jango);
	void		NoticeParse(CString dat, CMapStringToString& ary);
	void		DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat = 0, UINT type = 0, int clrIndex = COLOR_TEXT);
	COLORREF	GetIndexColor(int index);
	CBrush*		GetAxBrush(COLORREF clr);
	CPen*		GetAxPen(COLORREF clr, int width, int style);
	CFont*		GetAxFont(CString fName = "����", int point = 9, bool bItalic = false, int nBold = 0);
	CString		SetComma(CString src);
	void		Init(CControlWnd* pCtrlWnd);
	void		SetHogaDepth(int depth);
	void		DoModalConfigDlg();
	int		SetHogaReserve();
	void		ClearJumun();
	CPoint		GetRect2HogaPoint(CPoint point);

	
	void		SetHold(bool bHold);
	int		GetRowheight(){return (int)m_height;};
	void		ChangeFont(CFont* pFont, CFont* pBFont);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHogaWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHogaWnd();

	// Generated message map functions
public:
	//{{AFX_MSG(CHogaWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonMdStop();
	afx_msg void OnButtonMdJm();
	afx_msg void OnButtonMsJm();
	afx_msg void OnButtonMsStop();
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CPoint trackpoint;
	CString SplitString(CString& strData, CString strToken);
};

