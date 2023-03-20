#pragma once
// MapWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
#include "../../control/fx_misc/fxImgButton.h"
#include "sharemsg.h"
#include "GridWnd.h"
#include <afxmt.h>
class CMainWnd : public CWnd
{
	// Construction
public:
	CMainWnd(CWnd *pWnd);

	// Attributes
public:
	CRect m_rectMain;
	// Operations
public:
	BOOL GetRTSVisible() { return m_bRTS; };
	void SetRTSVisible(BOOL bRTS) { m_bRTS = bRTS; };
	COLORREF getAxColor(int color);
	CFont *GetAxFont(CString fName = _T("����ü"), int point = 9, bool bBold = false, bool bItalic = false);

	CString GetMarketTime() { return m_strMarketTime; };  // KSJ 2012.11.26 ���ð�
	void IsChange(BOOL bChange) { m_bChange = bChange; }; // 2016.12.16 KSJ �����Ҷ� ���������� ����Ǿ����� ����
	CString GetMarketEndTime() { return m_strMarketEndTime; };
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CMainWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	CFont* m_pFont{};
	CString _sTrigger;
	bool    _bInit{};

private:
	// MEMBERS
	// window members
	CWnd* m_pWnd{};
	std::unique_ptr<class CTreeWnd> m_pTreeWnd{};
	std::unique_ptr<class CToolWnd> m_pToolWnd{};
	std::unique_ptr<class CGroupWnd> m_pGroupWnd{};
	std::unique_ptr<class COptDlg>  m_pOptDlg{};

	bool m_bProc{};
	int m_nOption{};
	BOOL m_bDLLDATA{};
	BOOL m_bUser9555{}; // 2016.12.13 KSJ ���ο��� ��� ��ٸ���
	BOOL m_bUser9111{}; // 2016.12.13 KSJ ���� ���� ��
	BOOL m_bChange{};	  // 2016.12.16 KSJ ������ �߻� �ߴ��� üũ

	// resource memebers
	CSize m_size;
	int m_nTree{};   // Ʈ�� ������
	int m_nBasket{}; // �ٱ��� ������
	int m_nGroup{};  // �׷������� ������
		       // for resizing
	int m_nResize{};
	CPoint m_xPos;
	CPoint m_xPosMove;
	CRect m_rcDIVIDE;
	// data members
	CString m_user;
	CString m_id;
	CString m_home;
	BOOL m_bTree{};
	BOOL m_bRTS{};
	BOOL m_bBasket{};
	CRect m_rcDivide[3];
	CRect m_rcPanel[3]; // for rounding

	CfxImgButton m_btOption;
	
	bool m_bDestroy{};
	CBrush* m_pBrush{};
	int m_nOptHeight{};
	CString m_szRET;
	CString m_strWaveFile;
	BOOL m_bWaveApply{};

	COLORREF m_clrKospi{};
	COLORREF m_clrKosdaq{};
	BOOL m_bMKClrApply{};
	BOOL m_bPopupDlg{};
	BOOL m_bDispCode{};
	BOOL m_bStartMsg{};
	CString m_strTag;
	BOOL m_bNewsNChart{};    // 2011.12.02	KSJ
	BOOL m_bMargin{};	       // 2011.12.05	KSJ
	BOOL m_bInfo{};	       // 2011.12.05	KSJ
	BOOL m_bTicker{};	       // 2011.12.12	KSJ
	bool m_bDominoToolWnd{}; // 2012.01.13 KSJ ��ȭ�鿡�� ���������� ���� ���ٰ� �������� �ʾҴµ� ���� ���� ����
	int m_nGroupIndex{};     // 2012.01.13 KSJ �׷��ε���

	CString m_strMarketTime;    //���屸�� (����ȣ��) KSJ 2012.11.26
	CString m_strMarketEndTime; //����ȣ��
				    // etc...
	void DrawBitmap(CDC *pDC, CRect rc, CString strImage);
	void DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);

	// OPERATION
	// windows operation
	CBrush *GetAxBrush(COLORREF clr);
	void DrawMove(CRect rect);
	void SendOper(class CSendData *sdata);
	void SelectGroup(int kind);
	void CreateChild();
	void DestroyChild();
	void ResizeOper(int cx, int cy);
	LONG OperTreeVisible(int opt);
	LONG OperBasketVisible(int opt);
	void OperSize(int opt, CSize size);
	void OpenScreen(int type, CString data);
	LONG OperDLLOUB(CRecvData& data);
	void parsingTrigger(CString datB);
	HBITMAP getBitmap(CString path);
	DWORD SearchOption(WORD kind, DWORD option);
	CString SplitString(CString &strData, CString strToken);

	void parsingDomino(CString datB);
	// resource operation
	void DrawDivide(CDC *pDC);
	void SetPallette();
	void InvalidateDivide();

	// data operation
	void init();
	void loadinfo();
	void saveinfo();
	void sendTR(CString trCode, char *datB, int datL, int key);
	CString Variant(int comm, CString data = "");
	void SendMode(bool bSend = true);
	void Enable(int kind, bool bEnable = true);
	void SaveItemInfo(CString strKeyName, CString strValue); // 2011.12.13 KSJ �Ѿ����� ini���Ͽ� �߰���
	// etc...
	void GuideMessage(CString msg);

	// 2013.01.23 KSJ win7 or win8�϶� ���������� ����� x+2, y+1�Ǵ� ������ �ִ�.
	//�������� �׷��� Ȯ�� ������ �ʾ����� �켱 OnSize�Ҷ� x-2, y-1���ش�.
//	bool IsWin7or8();
	bool m_bWin7or8{};

public: // RTM
	BOOL IsValidCode(CString strCode);
	CString GetDataTitle(int nKind);
	BOOL m_bRemove{}; // 2011.12.29 KDK

private:			 

private:
	MINMAXINFO m_MMI{};

	CString m_strPal;
	CString GetPalette();

	BOOL m_bAlertx; // 2012.01.19 KSJ Alertx �߰�

	void SetMarketTime(char *datB); // 2012.11.26 KSJ ������ ��ȸ
	void Wait(DWORD dwMillisecond); // 2015.04.08 KSJ ������ �������� �����Ȱ� ������� �ʾ� ������ ��Ŵ.
public:
	void RequestMarketTime(); // 2012.11.26 KSJ ������ ��ȸ
	void uploadBackup();	  // 2016.07.12 KSJ �������� ������ ���� 'b' ������	
	void RTS_RecvRTSx(LPARAM lParam);

	int m_iTick{};
	BOOL m_bSending{};
	CString CheckIP();
	bool isIPInRange(CString ip, CString network_s, CString network_e);
	unsigned int IPToUInt(CString ip);

	bool m_bSkipRTS{};

	CString m_strBeginTimeEnd = "085959";  //����ȣ�� ����ð� xx:59:59
	CString m_strEndTime      = "152000";  //����ȣ�� ���۽ð� xx:10:00
	CString m_strEndTimeEnd   = "155959";  //����ȣ�� ����ð� xx:59:59
	CString m_strBeginTime    = "083000";  //����ȣ�� ���۽ð� xx:10:00

	bool m_bcustomer{};
	
	std::map<CString, int>    _mRealtime;
	std::map<CString, std::unique_ptr<struct _Ralert>> _mapRealData;
	

	void Request_GroupList();
	void Request_GroupCode(int iseq);
	void receiveOub(int key, CString data);
};
