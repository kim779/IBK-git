#if !defined(AFX_MAINWND_H__012DD35A_3186_4393_8080_BDC091E08DA2__INCLUDED_)
#define AFX_MAINWND_H__012DD35A_3186_4393_8080_BDC091E08DA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
struct	_uinfo {
	char	gubn[2];	// ��������
				// "MY" : ��������
				// "UL" : ����� ������ ��� 
				// "UD" : ����� ������
	char	dirt[1];	// ���۹��� 'U' : pc==>host, 'D' : host==>pc, 'X' : host==>pc (conversion), 'R' : ����ȸ
	char	cont[1];	// ���ӱ��� 'F':First, 'M':Middle, 'L':First&Last, 'C':Cancel
	char	name[80];	// file name
	char	nblc[5];	// ������, ����ũ��(max:64k)
	char	retc[1];	// return value 'O':���� 'E':����
	char	emsg[40];	// error message
};

struct	_ginfo {
	char	gnox[2];	// group number
	char	gnam[20];	// group name
	char	jrec[4];	// _jinfo count
	//	char	jptr[1];	// _jinfo pointer
};

struct	_jinfo {
	char	gubn[1];	// ���񱸺�	0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
	char	code[12];	// �����ڵ�
	char	xprc[10];	// �����ܰ�
	char	xnum[10];	// ��������
};

struct	_updn {
	_uinfo	uinfo;
	_ginfo	ginfo;
};

const int sz_uinfo   = sizeof(struct _uinfo);
const int sz_ginfo   = sizeof(struct _ginfo);
const int sz_jinfo   = sizeof(struct _jinfo);
const int sz_updn    = sizeof(struct _updn);


class CParam
{
public:
	int			key;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int			point;			// font point
	int			style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* pWnd, _param* pInfo);
	CMainWnd(CWnd* pWnd, void* pInfo);

	void Sendtest();
	void sendTR(CString name, CString data, BYTE type, int key, CString keyName, int uniqueKey);
	BOOL SendTR(CString strName, BYTE type, CString strData, BYTE key);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainWnd();

	CStringArray m_arrGname;		//2020 10 �׷������(2�ڸ�) �׷�� ���� 
	CStringArray m_arrTmpGname;
	void	SearchGroupList(bool bInt = false);		//2020 10 ���ɱ׷츮��Ʈ��ȸ
	void	SetParam(_param *pParam);
	void    sendTR_1(CString name, CString data, BYTE type, int key, CString keyName, int uniqueKey);
		CString Variant(int comm, CString data = "");
	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnCtrl(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnGrp(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnPreview(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	BOOL m_show;
	afx_msg void OnShowChanged();
	afx_msg BOOL GetVisible();
	afx_msg void SetVisible(BOOL bNewValue);
	afx_msg void SetData(LPCTSTR str);
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	std::unique_ptr<class CGrpWnd> m_pGrpWnd;
	std::unique_ptr<class CCtrlWnd> m_pCtrlWnd;

	CWnd* m_pParent;
	struct _param	m_param;
	bool		m_bCtrl;

	void	Resize();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
protected:
	void OnshowChanged();


	enum
	{
		dispidSetShowLine = 6L,
		dispidClear = 5L,
		dispidSetProperties = 4L,
		dispidGetProperties = 3L,
		dispidvisible = 2,
		dispidshow = 1
	};
	void OnvisibleChanged();
	BOOL m_visible;
	BSTR GetProperties();
	void SetProperties(BSTR str);
	void Clear();
	void SetShowLine(SHORT nLine, BOOL bShow);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINWND_H__012DD35A_3186_4393_8080_BDC091E08DA2__INCLUDED_)
