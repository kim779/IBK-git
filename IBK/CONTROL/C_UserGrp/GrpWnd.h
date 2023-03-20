#pragma once

#include <afxtempl.h>
#include <afxmt.h>

struct _tick	// Left/Right/Bottom Tick Info
{
	bool	bDraw=false;
	int	lcolor=0;
	int	lstyle=0;
	int	lwidth=0;
	CRect	tkRect;
};

struct _RgnInfo	// Region Info
{
	int	tkHeight=0;

	_tick	tick[3];	// left/right/bottom
	CRect	gpRect;		// graph rect
};

class CGrpWnd : public CWnd
{
public:
	CGrpWnd(CWnd *pView, CWnd *pParent, struct _param *pInfo);
	virtual ~CGrpWnd();

	//{{AFX_VIRTUAL(CGrpWnd)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CGrpWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT	OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnGrp(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	void	DrawGraph(CDC *pDC);
	void	ClearGrp();
	void	Resize();
	struct _NMinMaxChk*	GetMinMaxChk(int rKey);
	int	GetGrpKindFromConfig(int index);
	void	DrawEmpty(CDC *pDC, bool bInit);
	void	DrawFrameRect(CDC *pDC, CRect rc, COLORREF rgb);
	void	TipMouseMove(CPoint point);
	void	MouseMove(bool bIsTipEvent, CPoint point, bool bCrossOnly = false);
	void	SetShowLine(int nLine,bool bflag);
	class CCrossLine* GetCrossLine() { return m_pCrossLine.get(); }
	void	MessageToParent(int message, int wParam, int lParam)	{ m_pParent->SendMessage(message, wParam, lParam); }
public:
	std::array<std::unique_ptr<struct _RgnInfo>, MAX_REGION>	m_pRgnInfo;		// ���� ����	
	std::array <std::unique_ptr<class CGrp_Base>, MAX_GRAPH>       m_pGrpInfo;		// �׷��� ����
	std::unique_ptr<class CGrp_Data>	m_pDataInfo;			// �׷��� ������ ����

	bool		m_bShow[MAX_GRAPH];
	CArray<_GrpLine, _GrpLine>	m_arGrpLine;		// �׷��� ��������

	CString			m_CfgRgnInfo;		// ������ ���� ���� ���� ����
	int			m_CfgRgnCnt;		// ������ ���� ����
	int			m_RgnCnt;		// ���� ����
	int			m_GrpCnt;		// �׷��� ����

	std::vector<std::unique_ptr<struct _NMinMaxChk>>	m_pMinMaxChk;
	int			m_MinMaxCnt;
	int			m_totalDay;
	int			m_dispPos;
	int			m_dispEnd;
	int			m_dispDay;
	int			m_ShowCnt;	// param option, ��°���
	int			m_ReqCnt;	// param option, ��û����

	CFont			*m_pFont;	// Font
	CFont			*m_pBoldFont;	// Bold Font
	COLORREF		m_TkTextColor;	// Tick Text color
	COLORREF		m_TkLineColor;	// Tick Internal Line line

	CRect			m_ObjRect;	// Entire Rect
	CRect			m_GrpRect;	// Graph Rect
	int			m_fPoint;	// Font point
	CString			m_fName;	// Font name
	
	int			m_dIndex;	// ���ֿ���ƽ

	CString			m_sTipName;	// Tooltip Head String
	CString			m_sExtraName;	// Extraname
	bool			m_bStringIndex;	// �ϴ� ���������� ���ڿ�����������
	CString			m_sTickUnit[3];	// ��/��/�� ����
	bool			m_bHDraw;	// ���� �׸���
	bool			m_bVDraw;	// ������ �׸���
	bool			m_bCtrl;	// contrl ��뿩��
	bool			m_bMinMax;	// �ִ��ּ� ǥ��
	bool			m_bPreview;	// Preview
	bool			m_bRevDraw;	// �������� �׸���
	bool			m_bTickVertic;	// �ϴ�ƽ ��������
	bool			m_bItemRts;	// �� �׸� ���� �ǽð�
	bool			m_b3NChart;	// IGK_2D3NBAR

private:
	int	DispatchData(WPARAM wParam, LPARAM lParam);
	void	RealTimeData(CString sRTS);
	void	RealTimeData(struct _alertR* alert);

	bool	RealTimeBasic(CString sCode, DWORD* data);
	bool	RealTimeItems(CString sCode, DWORD* data);
	bool	AssignRegion(int nRgn);
	bool	AssignGraph(int nGrp);
	
	void	ParseParam(char *param);
	int	RequestHead();
	void	RemoveComponent();
	int	GetPalette(int index);
	int	GetTickColor(int color);

	BOOL	IsPtInGrp();
	CString	GetTipStr(CPoint point);

	bool	ChangeDisplayScale(int scale);
	bool	ChangeDisplay(int dispDay);
	bool	ChangeDisplayShift(int shift);
	void	ChangePalette();
	void	ExcelProc();
	void	RecalculateMinMax();
	void	ReviseTick();
	int	CetBottomVerticHeight(CDC *pDC);
	void	SetError(CString sErr) { m_sError = sErr; }

private:
	CWnd			*m_pView;
	CWnd			*m_pParent;
	std::unique_ptr<class CToolTip>		m_pToolTip;
	std::unique_ptr<class CCrossLine>	m_pCrossLine;
	char			m_pHeader[sz_UPHEADER+1];
	
	int			m_TickView;
	CString			m_PosData;
	UINT			m_timerID;	
	COLORREF		m_tRGB;		// foreground color
	COLORREF		m_pRGB;		// background color
	int			m_tOrgRGB;	// param
	int			m_pOrgRGB;	// param
	CC_UserGrpApp*		m_pApp;	
	CString			m_sError;
	int			m_nShowLine;
};
