#pragma once
// MainWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
#include "../../control/fx_misc/fxImgButton.h"
#include "sharemsg.h"
#include "GridWnd.h"
#include <afxmt.h>


class CParam
{
public:
	int		key{};			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point{};			// font point
	int		style{};			// font style
	DWORD		tRGB{};			// text color index
	DWORD		pRGB{};			// paint color index
	CString		options;		// properties
};



class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* pWnd, _param* pInfo);
	CString m_slog;
// Attributes
public:
	CParam		m_param{};
	CString		m_strTotalCode;
	CString		m_strAccount;
	BOOL		m_bOthers{};
	CString		m_mapName;
	CString		m_Smallfields;
	CString		m_SmallDefWidth;
	CString		m_Largefields;
	CString		m_LargeDefWidth;
	CString		m_Pass;
	BOOL		m_bRemain{};
	CString		m_sMapHandle;

	BOOL		m_bChangeGroup{};
	int			m_iSendTr{};
// Operations
public:
	BOOL GetRTSVisible() { return m_bRTS; };
	void SetRTSVisible(BOOL bRTS) { m_bRTS = bRTS; };
	COLORREF getAxColor(int color);
	CFont*	 GetAxFont(CString fName = _T("����ü"), int point = 9, bool bBold = false, bool bItalic = false);

	void testSaveFile(CString dt);
	class CTreeWnd* GetTreeWnd();

	void	SetParam(_param *pParam);
	int	GetParamKey() { return m_param.key; }
	CString GetParamName() { return CString(m_param.name, m_param.name.GetLength());}
	CRect	GetParamRect() { return m_param.rect; }
	BOOL	GetViewType() { return m_bAllType; }
	void    doRTMx(LPARAM lParam);
	CString GetMarketTime() { return m_strMarketTime; };	//KSJ 2012.11.26 ���ð� 
	void	uploadBackup();			//2016.07.12 KSJ �������� ������ ���� 'b' ������
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
		virtual void OnFinalRelease();
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
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
	CString m_cbGroupData;
	afx_msg void OnCbGroupDataChanged();
	afx_msg void SetProperties(LPCTSTR str);
	afx_msg BSTR GetProperties();
	afx_msg void Reload();
	afx_msg void SetUseToolBox(BOOL bUseTool);
	afx_msg void SetViewType(BOOL bType);
	afx_msg void ChangeGroup(short nIndex);
	afx_msg void ChangeArrange(short nIndex);
	afx_msg void ChangeView(short nIndex, short ncbIndex);
	afx_msg void SetAutoCheck(BOOL bAuto);
	afx_msg BSTR GetCode();
	afx_msg void OnPortfolio(LPCTSTR result);
	afx_msg void RefreshCX(short key, short nIndex);
	afx_msg void SetGroup(short nIndex);
	afx_msg void Save();
	afx_msg BOOL CheckSaveStatus();
	afx_msg void Recover();
	afx_msg void DeleteOrgFile();
	afx_msg void ClosingSave(short nID);
	afx_msg BOOL IsWindow();
	afx_msg void ClearGrid();
	afx_msg void SetExpect(BOOL bExpect);
	afx_msg void SetAccount(LPCTSTR strAccount);
	afx_msg void SetOthersMap(BOOL bFlag);
	afx_msg void SetFieldData(LPCTSTR mapname, LPCTSTR smallFields, LPCTSTR smallWidth, LPCTSTR largeFields, LPCTSTR largeWidth);
	afx_msg void SetPass(LPCTSTR sPass);
	afx_msg BOOL CanClose();
	afx_msg void SetMapHandle(LPCTSTR sHandle);
	afx_msg BSTR GetMapHandle();
	afx_msg BSTR GetRemainCode();
	afx_msg void SearchGroupList();
	afx_msg void SearchGroupCode(short index);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	
private:
// MEMBERS
// window member


	CWnd*	m_pWnd{};
	std::unique_ptr<class CTreeWnd>		m_pTreeWnd{};
	std::unique_ptr<class CToolWnd>		m_pToolWnd{};
	std::unique_ptr<class CBaseWnd>		m_pBasketWnd{};
	std::unique_ptr<class CGroupWnd>	m_pGroupWnd{};
	bool			m_bProc{};
	int			m_nOption{};
	
	
// resource memebers
	CSize			m_size;
	int			m_nTree{};	// Ʈ�� ������
	int			m_nBasket{};	// �ٱ��� ������
	int			m_nGroup{};	// �׷������� ������
// for resizing	
	int			m_nResize{};
	CPoint			m_xPos;
	CPoint			m_xPosMove;
	CRect			m_rcDIVIDE;
// data members
	CString			m_user;
	CString			m_id;
	CString			m_home;
	BOOL			m_bTree{};
	BOOL			m_bRTS{};
	BOOL			m_bBasket{};
	CCriticalSection	m_sync;
	CRect			m_rcDivide[3];
	CRect			m_rcPanel[3];//for rounding

	CfxImgButton		m_btOption;
	std::unique_ptr<class COptDlg>		m_pOptDlg;
	CFont*			m_pFont{};
	bool			m_bDestroy{};
	CBrush*			m_pBrush{};
	int			m_nOptHeight{};
	CString			m_szRET;
	CString			m_strWaveFile;
	BOOL			m_bWaveApply{};

	COLORREF	m_clrKospi{};
	COLORREF	m_clrKosdaq{};
	BOOL		m_bMKClrApply{};
	BOOL		m_bPopupDlg{};
	BOOL		m_bDispCode{};

	//ToolWnd�� ������� ���� ����
	BOOL		m_bUseToolWnd{};
	//ViewType ����
	BOOL		m_bAllType{};

	int			m_nFileType{};
	int			m_nMapKey{};
	int			m_nID{};
	int			m_nSendCount{};

	
	BOOL		m_bMainClose{};

	CString		m_strMarketTime;	//���屸�� (����ȣ��) KSJ 2012.11.26 

// etc...
	void DrawBitmap(CDC *pDC, CRect rc, CString strImage);
	void DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);
	
// OPERATION
// windows operation
	CBrush*	GetAxBrush(COLORREF clr);
	void	DrawMove(CRect rect);
	void	SendOper(class CSendData* sdata);
	void	SelectGroup(int kind);
	void	CreateChild();
	void	DestroyChild();
	void	ResizeOper(int cx, int cy);
	LONG	OperTreeVisible(int opt);
	void	OperSize(int opt, CSize size);
	void	OpenScreen(int type, CString data);
	LONG	OperDLLOUB(WPARAM wParam, LPARAM lParam);
	void	parsingTrigger(CString datB);
	HBITMAP getBitmap(CString path);
	DWORD	SearchOption(WORD kind, DWORD option);
	
	void	parsingDomino(CString datB);
// resource operation
	void	DrawDivide(CDC* pDC);
	void	SetPallette();
	void	InvalidateDivide();
	
// data operation
	void	init();
	void	loadinfo();
	void	saveinfo();
	void	sendTR(CString name, CString data, BYTE type, int key, CString keyName, int uniqueKey);
	int	searchTR(CString name, CString data, BYTE type, int key, CString keyName, int uniqueKey);

	CString Variant(int comm, CString data = "");
	void SendMode(bool bSend = true);
	void Enable(int kind, bool bEnable = true);
// etc...
	void GuideMessage(CString msg);
	void SplitString(CString &strData, CString strToken,CStringArray &ar);
	
public:		// RTM

	BOOL IsValidCode(CString strCode);
	CString GetDataTitle(int nKind);
	BOOL		m_bRemove;		// 2011.12.29 KDK

	int	GetFileType();
private:	// RTM
	HANDLE			m_hEventRTM{};
	
private:
	MINMAXINFO m_MMI;

	CString m_strPal;
	CString GetPalette();


	void	SetMarketTime(char* datB);	//2012.11.26 KSJ ������ ��ȸ
	void	RequestMarketTime();	//2012.11.26 KSJ ������ ��ȸ


	//�������� ������ȸ 
	CStringArray m_arrGname;				//2020 10 �׷������(2�ڸ�) �׷�� ���� 
	CStringArray m_arrTmpGname;
	void	SearchGroupList(bool bInt = false);		//2020 10 ���ɱ׷츮��Ʈ��ȸ
	
	char*	m_pcodeArr{};
	CMap<int, int, char*, char*>	m_mapGroupCode;
	void RemoveGroupCodeMap(int key = -1);

	int m_iTrkey{};


public:

	CString _groupName;
	int     _groupKey = 0;
	void Request_GroupCode(int iseq);
};