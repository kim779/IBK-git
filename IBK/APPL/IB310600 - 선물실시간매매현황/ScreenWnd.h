#pragma once
// MiddleWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMiddleWnd window

class CScreenWnd : public CWnd
{
// Construction
public:
	CScreenWnd(class CMapWnd *pParent);

// Attributes
public:
	
protected:
	class CMapWnd*	m_pMapWnd=nullptr;
	std::unique_ptr <class CDllCtrl>	m_pOrderDll;
	std::unique_ptr <class CDllCtrl2>	m_pSignedDll;
	std::unique_ptr <class CDllCtrl>	m_pBalanceDll;

	std::unique_ptr <class SplitterCtrl> m_pSplitter;
	
	int m_nHeight[3]{};
	int m_nTotalHeight{};			//Mapȭ���� �� ����

	BOOL m_bBtnState[3]{};		//���̱�, ���߱� ��ư flag
	BOOL m_bClick{};				//onsizeȣ���� ��ưŬ������ ������ �������� ����

// Operations
public:
	CMapWnd* GetMapWnd();
	void SetScreenAccount(CString strParam, CString strAccount);	//���¹�ȣ�� load�� ȭ������ ����
	void GetBtnState(BOOL *btn);			
	void SetBtnState();
	void SetPal();
	int GetScreenHeight();
	void SetClickBtn(BOOL bClick);

protected:
	void CreateControl();
	void BtnClickReSize(int cx, int cy);		//��ư Ŭ���� onSize
	void ScreenReSize(int cx, int cy);			//splitter ������ ���������� �׸��� ���� ���
	
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreenWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScreenWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CScreenWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

