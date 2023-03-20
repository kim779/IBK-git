#pragma once
// MainWnd.h : header file
//
#include "ExcelGrid.h"
#include "../../control/fx_misc/fxCodeCtrl.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_grid/fxGrid.h"



struct	_gridHdr {
	UINT	stid;		// string table id
	UINT	width;		// fixed width
	UINT	attr;		// fixed attribute
	
	char	symbol[8];	// nonfixed symbol
	UINT	format;		// nonfixed format
	UINT	attrx;		// nonfixed attr
	
	UINT	fidx;		// field index
};

struct _mid {
    char    fcod    [ 2][ 8];   /* ���� �����ڵ� ����Ʈ(AAAAAAAA�̸� default)   */
    char    scod    [40][ 6];   /* �ֽ� �����ڵ� ����Ʈ */
};

#define MAX_CNT     2000
#define MAX_ST_R    40
#define MAX_FU_R    2


static  struct  futitem {
    char    fcod        [ 8];   /* ���� �����ڵ�    */
    char    fnam        [30];   /* ���� �����      */
    char    fprc        [ 8];   /* ���� ���簡      */
    char    fdif        [ 8];   /* ���� ���ϴ��    */
    char    fmba        [ 7];   /* ���� ���̽ý�    */
	char    fopv        [ 9];   /* ���� �̰�������  */         
} __futitem;

static  struct  griditem {
    char    code        [ 6];   /* �����ڵ�             */
    char    hnam        [20];   /* �����               */
    char    curr        [ 9];   /* ���簡               */
    char    diff        [ 8];   /* ���ϴ��             */
    char    drat        [ 7];   /* �����               */
    char    ask1        [ 9];   /* �ŵ�ȣ��1            */
    char    bid1        [ 9];   /* �ż�ȣ��1            */
    char    bvol        [10];   /* ���ϰŷ���           */
    char    gvol        [10];   /* �����ŷ���           */
    char    higp        [ 9];   /* ��                 */
    char    lowp        [ 9];   /* ����                 */
    char    stdp        [ 9];   /* ���ذ�               */
} __griditem;


static  struct _mod {
    struct  futitem f1item  ;   /* ���� �ֱٿ��� ����Ÿ         */
    char    f1av        [ 8];   /* ���� �ֱٿ��� �ŵ����ܷ�     */
    char    f1bv        [ 8];   /* ���� �ֱٿ��� �ż����ܷ�     */
    char    f1ac        [ 8];   /* ���� �ֱٿ��� �ŵ��ѰǼ�     */
    char    f1bc        [ 8];   /* ���� �ֱٿ��� �ż��ѰǼ�     */
    char    f1pv        [ 8];   /* ���� �ֱٿ��� ���ż����ܷ�   */
    char    f1pa        [ 8];   /* ���� �ֱٿ��� ���ż��ѰǼ�   */
	
    struct  futitem f2item  ;   /* ���� ������� ����Ÿ     */
	
    char    spr1        [ 8];   /* ���� ��������            */
	
    char    nrec        [ 4];   /* ���ڵ� ī��Ʈ            */
    struct  griditem    grid[MAX_ST_R];
} __mod;

#define USE_DLL_ALERTx
/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
class ChartPoint
{
public:
	int idx{};
	int xpos{};
	double data{};
	CString sdata;
	CString stime;
	bool	bhoga{};
};

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* pParent);

// Attributes
public:
	CFont* m_pFont;
	CPen* m_pLine1Pen;
	CPen* m_pLine2Pen;
	CPen* m_pDeliPen;
	CPen* m_pGridLine;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString m_choi_code;	//�ֱٿ��� �ڵ�
	CString m_cha_code;		//������ �ڵ�
	CString m_choi_curr;	//�ֱٿ� ���簡
	CString m_choi_diff;	//�ֱٿ� ���ϴ��
	CString m_medo_jan;		//�ŵ����ܷ�
	CString m_mesu_jan;		//�ż����ܷ�
	CString	m_medo_geon;	//�ŵ��ѰǼ�
	CString m_mesu_geon;	//�ż��ѰǼ�
	CString m_meme_jan;		//�Ÿ� ���ܷ�
	CString m_meme_geon;	//�Ÿ� ���Ǽ�
	CString m_cha_curr;		//���ٿ� ���簡
	CString m_cha_diff;		//���ٿ� ���ϴ��
	CString m_basis;		//���̽ý�
	CString m_migyul;		//�̰���
	CString m_sum_up1;		//���ݻ�¼�
	CString m_sum_up2;		//�ŷ�����¼�
	
	CRect m_rc_choi_curr;	//�ֱٿ� ���簡
	CRect m_rc_choi_diff;	//�ֱٿ� ���ϴ��
	CRect m_rc_medo_jan;	//�ŵ����ܷ�
	CRect m_rc_mesu_jan;	//�ż����ܷ�
	CRect m_rc_medo_geon;	//�ŵ��ѰǼ�
	CRect m_rc_mesu_geon;	//�ż��ѰǼ�
	CRect m_rc_meme_jan;	//�Ÿ� ���ܷ�
	CRect m_rc_meme_geon;	//�Ÿ� ���Ǽ�
	CRect m_rc_cha_curr;	//���ٿ� ���簡
	CRect m_rc_cha_diff;	//���ٿ� ���ϴ��
	CRect m_rc_cha_migyul;	//�̰�������
	CRect m_rc_basis;		//���̽ý�
	CRect m_rc_sum_up1;		//���ݻ�¼�
	CRect m_rc_sum_up2;		//�ŷ�����¼�
	CRect m_rc_dataRect;    //������ ����
	CRect m_rc_title;		//���񿵿�

	double m_maxyval, m_minyval;
	double m_maxy2val, m_miny2val;
	CRect chartRect, drawChartRect;
	CRect m_rc_Check1, m_rc_Check2;
	bool  m_bCheck1, m_bCheck2;
	double m_lastIChg;
	int	   m_lastIVol;

//	CArray<class ChartPoint*, class ChartPoint*> m_arCPoint;
	CArray<std::shared_ptr<ChartPoint>, std::shared_ptr<ChartPoint>&> m_arCPoint;

	CString idTOstring(UINT id);
	int m_rowC;
	int m_colC;
	int		m_gridW;
	CCriticalSection m_sync;
	CFont* m_fontB;
	CFont* m_font;
	enum _viewType {viewTWO = 2, viewTHREE = 3, viewFOUR = 4 } m_viewType;
	CArray < _gridHdr, _gridHdr > m_gridHdrX;
	void initialGrid();
	CString SplitString(CString &strData, CString strToken);
	LRESULT SendTR(CString strCode, CString strData, int iKey, int iStat = NULL);
	CFont* GetAxFont(CString strName, int nPt, bool bItalic, int nBold);
	CString Variant(int nComm, CString strData = _T("") );
	COLORREF GetIndexColor(UINT nIndex);
	void Init();
	virtual ~CMainWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnBtnSel();
	afx_msg void OnBtnSend();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	int m_lastCheg;
	int m_lastHVOL;
	bool m_bViewChart;
	void loadFieldInfo();
	int m_fontsize;
	void ParseAlert(CString pData);
	COLORREF m_BackColor;
	COLORREF m_Line1Color;
	COLORREF m_Line2Color;
	std::unique_ptr<class CfxGrid>	m_grid;
	std::unique_ptr<class CfxImgButton>	m_pBtnSel;
	std::unique_ptr<class CfxImgButton>	m_pBtnSend;
	CString m_stime;
	CRect	m_ChartRect;
	
	
	CWnd* m_pWizard;

public:		// RTM
	
	void DrawUpData(CDC *pdc);
	void CalcRow(int row);
	void CalcRowDiff(int row);
	CString m_strPal;
	int m_nOldRow;
	CArray<char *, char *>	m_arBytesRTM;
	CCriticalSection	m_syncRTM;
	CWinThread*		m_pThreadRTM;
	bool			m_bThreadRTM;
	CArray<struct _alertR *, struct _alertR *>	m_arBytesXRTM;
	
	
	void	doRTM(char *pBytes);
	void	doXRTM(struct _alertR* alertR);
	bool	WaitRTM();
private:	// RTM
	CString AddCommai(int iSrc);
	CString AddComma(CString strSrc);
	CBitmap* RequestBitmap(CString stPath);
	bool m_bChartChange;
	bool m_bDataChange;
	void RemoveCPosAll();
	CPen* RequestPen(COLORREF rgbLine, int iWidth, int iStyle);
	CPen* penLine2;
	CPen* penLine1;
	bool m_bfchg;
	bool m_bfhog;
	void AddChartData(CString time, CString sval, double yval,  int idx, bool bhoga = false);
	int m_maxidx;
	COLORREF m_clrChartBack;
	void DrawChart(CDC *pDC, CRect rc);
	COLORREF clrText;
	void initArea();
	void QueryExcelSise();
	double TOfabs(CString string);
	double TOf(CString string);
	int checkRealTimeCode(CString code, int &xrow);
	void parsingAlert(CString datB);
	void parsingAlert(struct _alertR* alertR);
	void SplitStringArray(CString &strData, CString strToken,CStringArray &ar);
	void DrawGrid();
	void DrawChart();

	HANDLE			m_hEventRTM;
	
	void	initRTM();
	void	stopRTM();
	void	removeRTM();
};

