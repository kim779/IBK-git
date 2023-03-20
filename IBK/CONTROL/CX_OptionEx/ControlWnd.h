#pragma once
// ControlWnd.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

#include "../../appl/IhUtils.h"
#include "../../h/jmcode.h"
#include "../../h/axisvar.h"

class CParam
{
public:
	int		key=0;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point=0;		// font point
	int		style=0;		// font style
	DWORD		tRGB=0;			// text color index
	DWORD		pRGB=0;			// paint color index
	CString		options;		// properties
};

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd(CWnd* pParent, _param* pParam, BOOL bMini = FALSE);
	virtual		~CControlWnd();

	enum { OBJ_C = 0, OBJ_P, OBJ_U, OBJ_D, OBJ_O};	//call, put, spin up, spin down, off
	enum { OF_O = 0, OF_F, OF_OFF};

// Attributes
protected:
	CWnd*		m_pParent;
	CParam		m_Param;

	CArray <ojcode, ojcode>		m_ojcode;		// �ɼ�����
	CArray <ojcode, ojcode>		m_wojcode;		// ��Ŭ�� �ɼ�����
	CArray <ojcode, ojcode>		m_mojcode;		// �ɼ�����

	bool		m_capture;
	BOOL		m_bTracking;
	BOOL		m_bHover[5];
	CRect		m_rcAll, m_rcBtn[5];
	HBITMAP		m_hBitmap[5], m_hBitmap_dn[5], m_hBitmap_hv[5], m_hBitmap_off[5];

	CRect		m_rcOFBtn[3];
	HBITMAP		m_hOFBitmap[3], m_hOFBitmap_dn[3], m_hOFBitmap_hv[3], m_hOFBitmap_off[3];
	CString		m_sOFImagePath[3];
	BOOL		m_bOFHover[3];
	int		m_nOFIndex;		//���纸�̴� �̹��� �ε��� ����
	bool		m_bOption;		//��ư�� ǥ�õǴ�

	CString		m_sRoot;
	COLORREF	m_backColor;
	CString		m_sImagePath[5];
	CFont		m_font;

	bool		m_bPut;			//2013.01.16 KSJ ��ư�� P�� ǥ���ؾߵǸ� true(�����ڵ尡 �ݿɼ��̸�)
	CString		m_strCurCode;		//2013.01.16 KSJ �����ڵ� ����
	CString		m_sMapHandle;		//���� �ڵ��� ����
	int		m_nIndex;		//���纸�̴� �̹��� �ε��� ����

	std::unique_ptr<CToolTipCtrl>	m_pTip;

	bool		m_bShowOFBtn;		//2013.10.22 KSJ �����ɼ���ȯ��ư�� ǥ�������� ����(����Ʈ ǥ�þ���).
	BOOL		m_bMini;
	bool		m_bDll;

private:
	//2013.01.17 KSJ
	void	loadingOJcode();				//��ǲ�ɼ� �����б�
	void	loadingMOJcode();				//��ǲ�ɼ� �����б�
	void	loadingWOJcode();				//��Ŭ���ɼ� �����б�
	void	SendTrigger();					//Ʈ���ź�����
	void	ChangeCode();					//���� ǲ���� ǲ�� �ݷ� ����
	void	CPButtonAction();				//��ǲ ��ư ������ ��
	void	SetEnable(bool bEnable);			//�����϶��� Disable ���ش�.
	void	UpDownButtonAction(bool bUp);			//UpDown��ư ������ ��
	void	UpDownButtonAction_WOpCode(bool bUp);
	void	UpDownButtonAction_MOpCode(bool bUp);
	void	GetOFCode();
	void	OFButtonAction();
	void	Draw(CDC* pDC);
	void	DrawButton(CDC* pDC, int index);	//2013.10.23 bButton�� false�� ��ǲ��ư�̰�, true�� ���ɹ�ư��
	void	DrawStretchByMask(CDC* pDC, CBitmap* pBitmap, CRect bRc, COLORREF maskcolor);
	CBitmap* getBitmap(CString path);
	void	SetButtonImages();
	CString	Variant(int comm, CString data = _T(""));
	void	SetParam(struct _param* pParam);
	void	setFont(CString fName, int fsize, int style, LOGFONT* lf);
	COLORREF GetIndexColor(int index);

// Operations
public:
	bool	Initialize(bool bDll);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg	void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg BSTR GetCurCode();
	afx_msg void SetCurCode(LPCTSTR lpszNewValue);
	afx_msg BSTR GetMapHandle();
	afx_msg void SetMapHandle(LPCTSTR lpszNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};