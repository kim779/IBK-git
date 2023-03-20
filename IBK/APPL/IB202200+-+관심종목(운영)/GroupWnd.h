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
	CGroupWnd(CWnd *pMainWnd);

	// Attributes
public:
	CString m_strWaveFile;
	BOOL m_bWaveApply;

	COLORREF m_clrKospi;
	COLORREF m_clrKosdaq;
	BOOL m_bMKClrApply;
	BOOL m_bPopupDlg;
	BOOL m_bDispCode;
	COLORREF m_clrbookMark;	   //�ϸ�ũ ��
	COLORREF m_clrbookMarkPen; //�ϸ�ũ�� ��

	int m_RecomGroup[30]; //��õ���� ���̴� ��ġ
	int m_iDragIn;	      // �巡�������� ������ ī��Ʈ Ƚ�� ����

	bool m_bActiveArrange; //���� ���� ����
	bool m_bNewsNChart;    // 2011.12.02	KSJ
	bool m_bMargin;	       // 2011.12.05	KSJ
	bool m_bInfo;	       // 2011.12.05	KSJ
	bool m_bTicker;	       // 2011.12.12	KSJ

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
	int GetDragInCount() { return m_iDragIn; }
	void SetDragInCount(int data);
	void AddDragInCount();
	void SendUini();

	int GetGroupCount();
	void GetMinMaxWidth(int &nMinWidth, int &nMaxWidth);
	BOOL IsValidCode(CString strCode);
	int GetRowCount();
	int GetFontSize() { return m_fontsize; }
	BOOL GetCurrBold() { return m_bCurr; }
	BOOL GetNameBold() { return m_bName; }
	BOOL GetAllBold() { return m_bAllBold; }
	CString GetFontName() { return m_fontname; }
	CString GetLinkmap() { return m_linkMap; }

	char GetBkUse() { return m_chUse; }
	COLORREF GetBkColor1() { return m_bkColor1; }
	COLORREF GetBkColor2() { return m_bkColor2; }
	COLORREF GetRTMColor() { return m_rtmColor; }
	COLORREF GetBookMarkColor() { return m_clrbookMark; }
	COLORREF GetBookMarkPenColor() { return m_clrbookMarkPen; }
	COLORREF GetFgColor(int nIndex) { return m_fgColor[nIndex]; }
	int GetRtmAction() { return m_rtmAction; }
	int GetDblAction() { return m_dblAction; }
	int GetSelAction() { return m_selAction; }
	int GetOverLapAction() { return m_overlap; }
	int GetOverLapAlert() { return m_overlapAlert; }

	int GetLine() { return m_line; }
	int GetRowHeight() { return m_rowH; }
	void SetGridBkColor(COLORREF color);
	void AutoExpectOper(int param);
	void saveWhenGroupClosed(int index);

	int sumEachGroupCount();
	void testSaveFile();
	void testSaveFile(CString dt);
	void InitSetGroup(); //�ʱ⿡ �׷� �����ϰ� �ϴ� �Լ�
	void RefreshGrid();
	std::pair<int, std::vector<std::shared_ptr<struct _intersx>>> WriteFileSumtoEachGroup(UINT curruntGroup);
	CString MakePacket(CString &code, CString amount = _T(""), CString price = _T(""), CString name = _T(""), CString bookmark = _T(""));
	void selectFirst();
	void saveGroupIndex(int index);
	void ReSetupGrid();
	void saveOverLap(int overlap);

	void SetEqualizerField(int param);

	CGridWnd *GetGridWndUpjn();
	void setIsRecommand(bool bFlag);
	bool GetIsRecommand();

	void saveRecommandGroup();

	int getRecommandCursel(int index) { return m_RecomGroup[index]; }
	void arrangeGroup(int type);
	int GetmnGroupCount() { return m_nGroup; }

	void checktheRecommand(); //���� �ǽð� ������ �׸��������쿡 ������Ʈ �ǽ�

	bool isModified(int kind);

protected:
	//{{AFX_MSG(CGroupWnd)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	// virtual
	void OperInit();
	void OperDraw(CDC *pDC);
	void OperDestory();
	void OperResize(int cx, int cy);
	void RecvOper(int kind, CRecvData *rdata);
	void RecvRTS(CRecvData *rdata);

public:						     // 2012.02.13 KSJ �̺�Ʈ�� ������ �����Ͱ� ������ ���� �޼ҵ� ȣ����.
	void RecvRTSx(LPARAM lParam, int igubn = 0); // 2012.01.19 KSJ Alertx �߰�
	void SelectOper();
protected:
	void loadcfg();
	void loadfield();
	void savecfg();

	// members
	void SetArray(CString rates);
	void MakeGridRect(CRect rcFull);
	void CreateGrid();
	void InitGrid();
	void SendGrid(int nIndex, CGridData *sdata);
	void SendGrid(int nIndex, UINT kind);
	void SendUpjnGrid(CGridData *sdata);
	int OverOper(int nIndex, CGridData *sdata);
	void DestroyGrid();
	void DrawDivide(CDC *pDC);
	void DrawTitle(CDC *pDC);
	void DrawMove(CRect rect);
	int GroupPlus(BOOL bExpand);
	int GroupMinus(BOOL bExpand);
	void IniGroupDel(); // 2011.12.12 KSJ ������ �׷��� ini����
	int GetIndexDivide(CPoint pt);
	CRect GetArea(int nIndex);
	CRect GetMovedRect(int nIndex, int xPos);
	void ExpectOper(int param);

	void DrawHighlight(CDC *pDC, CRect rect, COLORREF clrLight, COLORREF clrShadow);
	void DrawHigV(CDC *pDC, CRect rect, COLORREF clrLight, COLORREF clrShadow);
	int SetMovedRect(int nIndex, int xPos);

	
	int _httoi(const TCHAR *value);
	void CloseGrid(int nIndex);
	// sub utils
	int getTotalSize();
	void InvalidateDivide();

	void addGridWnd();     //�׸��� �޸� �籸��
	void AllGroupSave();   //�׷� �߰� �������Ŀ� �����ϱ�
	void loadGroupCheck(); //üũ�� �׷� ���� �������� 2011.12.13

	// members
	bool m_isRecommand; //���� ��õ������ �����ְ� �ִ°���
	int m_cntGroup; //�׷� ����

	int m_nGroup;
	CBrush *m_pBrush;
	COLORREF m_clrBack;
	CArray<int, int> m_arRATE;
	CArray<CRect, CRect> m_arRECT;
	CArray<CRect, CRect> m_arDIVIDE;
	CArray<CRect, CRect> m_arUpDIV;
	CArray<CRect, CRect> m_arDnDIV;
	BOOL m_bDivide;
	int m_xPos;
	int m_xPosMove;
	int m_nIndex;
	int m_nCurSel;
	CRect m_rcDIVIDE;
	CRect m_rcUpDIV;
	CRect m_rcDnDIV;
	//	CArray <class CGridWnd*, class CGridWnd*> m_GridWnd;
	std::array<std::unique_ptr<class CGridWnd>, MAX_GROUP> m_GridWnd;
	class CGridWnd *m_GridWndUpjn;
	std::unique_ptr<class DlgAutoSave> m_pdlgsave;

	CMapWordToPtr m_mapExpectCol;

	BOOL m_bCurr;
	int m_fontsize;
	CString m_fontname;
	CString m_linkMap;
	char m_chUse;
	COLORREF m_bkColor1;
	COLORREF m_bkColor2;
	COLORREF m_rtmColor;
	COLORREF m_fgColor[4];
	enum _rtmAction m_rtmAction;
	enum _dblAction m_dblAction;
	int m_selAction;

	int m_overlap;	    //�ߺ����
	int m_overlapAlert; //�ߺ���� ���â
	BOOL m_bAutoSave;   // 2016.12.09 KSJ ����� �ڵ����� üũ

	int m_line;
	BOOL m_bMAP;
	CString m_szTITLE;
	CRect m_rcTitle;
	int m_rowH;

	// ADD PSH 20070917
	BOOL m_bName;
	CString m_tempCode;
	// END ADD

	BOOL m_bAllBold; // 2012.11.28 KSJ ��ü ����

	void OnDestroySave();

public:
	BOOL IsNewDrop();
	BOOL m_bAutoCloseSave;
	const std::unordered_map<int, int>& getRSymbol();
	bool isCodeSymbol(CString code);

	std::map<CString, int> _symbol;
	std::map<int, CString> _GroupName;
	int m_iTime;
};
