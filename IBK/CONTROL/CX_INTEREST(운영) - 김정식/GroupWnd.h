#pragma once
////////////////////////////////////////////////////////////////////////////
// CGroupWnd window

#include "BaseWnd.h"
#include <AFXTEMPL.H>
#include "sharemsg.h"
#include "../../h/interst.h"

class CGridWnd;

class CGroupWnd : public CBaseWnd
{
// Construction
public:
	CGroupWnd(CWnd* pMainWnd);
	CString m_slog;
// Attributes
public:
	std::vector<std::shared_ptr<_intersx>> m_Inters;

	CString m_strWaveFile;
	BOOL	m_bWaveApply{};
	int		m_nSortCurIndex{};

	COLORREF	m_clrKospi{};
	COLORREF	m_clrKosdaq{};
	BOOL		m_bMKClrApply{};
	BOOL		m_bPopupDlg{};
	BOOL		m_bDispCode{};
	COLORREF    	m_clrbookMark{};		//�ϸ�ũ ��
	COLORREF    	m_clrbookMarkPen{};		//�ϸ�ũ�� ��

	CRect		m_globalRect;

	CString		m_strCode;

	int		m_iDragIn{};		// �巡�������� ������ ī��Ʈ Ƚ�� ����

	_trkey* 	m_pTrkey{};
	CString		m_commInfo[2][3];	// Send/Receive�� TRŰ�� �� ȭ�� �ε��� ��ȣ ����
	int		m_commIndex{};
	double	m_xRate{};
	double	m_yRate{};

	int			m_iInterCnt{};
// Operations
public:
// Overrides

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGroupWnd();	
	// Generated message map functions
	
public:
	void	saveWhenChangeViewMode(int index);
	int	GetDragInCount() { return m_iDragIn; }	
	void	SetDragInCount(int data);
	void	AddDragInCount();

	int	GetGroupCount();
	void	GetMinMaxWidth(int &nMinWidth, int &nMaxWidth);
	BOOL	IsValidCode(CString strCode);
	int	GetRowCount();
	int	GetFontSize() { return m_fontsize; }
	BOOL	GetCurrBold() { return m_bCurr; }
	BOOL	GetNameBold() { return m_bName; }
	BOOL	GetAllBold() { return m_bAllBold; }
	CString	GetFontName() { return m_fontname; }
	CString	GetLinkmap() { return m_linkMap; }

	char	 GetBkUse() { return m_chUse; }
	COLORREF GetBkColor1() { return m_bkColor1; }
	COLORREF GetBkColor2() { return m_bkColor2; }
	COLORREF GetRTMColor() { return m_rtmColor; }
	COLORREF GetBookMarkColor() { return m_clrbookMark; }
	COLORREF GetBookMarkPenColor() { return m_clrbookMarkPen; }
	COLORREF GetFgColor(int nIndex) { return m_fgColor[nIndex]; }
	int	GetRtmAction() { return m_rtmAction; }
	int	GetDblAction() { return m_dblAction; }
	int	GetSelAction() { return m_selAction; }
	int	GetOverLapAction() { return m_overlap; }
	int	GetOverLapAlert() { return m_overlapAlert; }

	int	GetLine() { return m_line; }
	int	GetRowHeight() { return m_rowH; }
	void	SetGridBkColor(COLORREF color);
	void	AutoExpectOper(int param);
	void	saveWhenGroupClosed(int index);
	
	int	sumEachGroupCount();
	void	InitSetGroup();			//�ʱ⿡ �׷� �����ϰ� �ϴ� �Լ�
	void	RefreshGrid();
	int		WriteFileSumtoEachGroup(UINT curruntGroup);
	CString	MakePacket(CString& code, CString amount = _T(""), CString price = _T(""), CString name = _T(""), CString bookmark = _T(""));
	void	selectFirst();
	void	saveGroupIndex(int index);
	void	ReSetupGrid();
	void	saveOverLap(int overlap);

	void	SetEqualizerField(int param);

	
	void	ChangeFieldOper(int param);
	void	arrangeGroup(int type);
	void	SetKillFocus(int view);
	void	DeleteOrg();

	void	setFontSize(int size);
	void	SendGrid(int nIndex, CGridData* sdata);
	void	SetInitSortingValue();		//2011.12.20 KSJ ��Ʈ���� �������� �ʰ� GridWnd�� m_bSorting�� false�� ����� �ش�.
protected:
	//{{AFX_MSG(CGroupWnd)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

// virtual 
	void	OperInit();
	void	OperDraw(CDC* pDC);
	void	OperDestory();
	void	OperResize(int cx, int cy);
	void	RecvOper(int kind, CRecvData* rdata);
	void	RecvRTS(CRecvData* rdata);
public:	//2012.02.13 KSJ �̺�Ʈ�� ������ �����Ͱ� ������ ���� �޼ҵ� ȣ����.
	void	RecvRTSx(LPARAM lParam);	//2012.01.19 KSJ Alertx �߰�

	void	loadcfg();
	void	loadfield();
	void	savecfg();
	
// members
	void	SetArray(CString rates);
	void	MakeGridRect(CRect rcFull);	
	void	CreateGrid();
	void	InitGrid();
	void	SendGrid(int nIndex, UINT kind);
//	void	SendUpjnGrid(CGridData* sdata);
	int		OverOper(int nIndex, CGridData* sdata);
	void	DestroyGrid();
	void	DrawDivide(CDC* pDC);
	void	DrawTitle(CDC* pDC);
	void	DrawMove(CRect rect);
	int		GroupPlus(BOOL bExpand);
	int		GroupMinus(BOOL bExpand);
	int		GetIndexDivide(CPoint pt);
	CRect	GetArea(int nIndex);
	CRect	GetMovedRect(int nIndex, int xPos);
	void	ExpectOper(int param);
	void	Recover();

	void	DrawHighlight(CDC* pDC, CRect rect, COLORREF clrLight, COLORREF clrShadow);
	void	DrawHigV(CDC* pDC, CRect rect, COLORREF clrLight, COLORREF clrShadow);
	int		SetMovedRect(int nIndex, int xPos);
	
	void	setIsRecommand(bool bFlag);
	bool	GetIsRecommand();
	void	SelectOper();
	int	_httoi(const TCHAR *value);
	void	CloseGrid(int nIndex);
// sub utils
	int	getTotalSize();
	void	InvalidateDivide();

	int	loadGroupCount();
	void	addGridWnd();		//�׸��� �޸� �籸��

	void	OperResizeMinus(int cx, int cy);
	
// members
	bool			m_isRecommand{};	//���� ��õ������ �����ְ� �ִ°���
	int			m_cntGroup{};		//�׷� ����

	int			m_nGroup{};
	CBrush*			m_pBrush{};
	COLORREF		m_clrBack{};
	CArray<int, int>	m_arRATE;
	CArray<CRect, CRect>	m_arRECT;
	CArray<CRect, CRect>	m_arDIVIDE;
	CArray<CRect, CRect>	m_arUpDIV;
	CArray<CRect, CRect>	m_arDnDIV;
	BOOL			m_bDivide{};
	int			m_xPos{};
	int			m_xPosMove{};
	int			m_nIndex{};
	int			m_nCurSel{};
	CRect			m_rcDIVIDE;
	CRect			m_rcUpDIV;
	CRect			m_rcDnDIV;
	std::array<std::shared_ptr<class CGridWnd>, MAX_GROUP> m_GridWnd;

	CMapWordToPtr		m_mapExpectCol{};
	
	BOOL			m_bCurr{};
	int			m_fontsize{};
	CString			m_fontname;
	CString			m_linkMap;
	char			m_chUse{};
	COLORREF		m_bkColor1{};
	COLORREF		m_bkColor2{};
	COLORREF		m_rtmColor{};
	COLORREF		m_fgColor[4]{};
	enum _rtmAction		m_rtmAction{};
	enum _dblAction		m_dblAction{};
	int			m_selAction{};

	int			m_overlap{};			//�ߺ���� 
	int			m_overlapAlert{};		//�ߺ���� ���â
	BOOL			m_bAutoSave{};		//2016.12.09 ����� �ڵ����� �߰�

	int			m_line{};
	BOOL			m_bMAP{};
	CString			m_szTITLE;
	CRect			m_rcTitle;
	int			m_rowH{};

	// ADD PSH 20070917
	BOOL			m_bName{};
	// END ADD
	BOOL			m_bAllBold{};
	BOOL IsNewDrop();
	void OnDestroySave();

	void saveServer(int gno);
};








































