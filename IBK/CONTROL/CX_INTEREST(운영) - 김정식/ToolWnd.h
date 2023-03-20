#pragma once
// ToolWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolWnd window
#include "BaseWnd.h"

#include "../../control/fx_misc/fxImgButton.h"
//#include "../../control/fx_misc/fxSpin.h"
#include "fxSpin.h"

#include "btntab.h"
#include "NewMenu.h"	// ADD PSH 20070912


class CToolWnd : public CBaseWnd
{
// Construction
public:
	CToolWnd(CWnd* pMainWnd);
	CString m_slog;
// Attributes
public:

// Operations
public:
	void ReloadList();
	void ReloadList2();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	void OperInit();
	void OperDraw(CDC* pDC);
	void OperDestory();
	void OperResize(int cx, int cy);
	void OperPallette();
	
// Implementation
public:
	virtual ~CToolWnd();

	// Generated message map functions
public:
	//{{AFX_MSG(CToolWnd)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	
	void	saveGroupwhenClose(int index);
	
	void	clickBtn();	
	HBITMAP getBitmap(CString path);
	CRect	AddDivide(CRect rect);	
	CRect	GetNextRect(CRect rcPrev, CSize size, CSize gap);
	CRect	MoveButton(CRect rcPrev, CSize size, CSize gap, CfxImgButton* pButton);
	void	DrawSEP(CDC* pDC, CRect rect);
	void	HideControl(CRect rcBasket);
	void	loadcfg();
	CString	loadFieldnm(int fno = -1);
	void	loadTabItem();
	int		loadActiveField();

	void	savecfg();
	void	SetSel(int nIndex);
	void	SelectGroup(UINT kind);
	void	SendTree(int nIndex);
	int		Reload();
	void	SelectMode(int nmode);
	// ADD PSH 20079017
	void	RemoveAllMenu();
	// END ADD
	void testSaveFile();
	
	void dominoSelectGroup(int index);
	void saveGroupIndex(int index);		//���ɱ׷� index ����
	void SetGroupDataView();			//�׷��� ��� ǥ���Ұ��� ���� ��û(�� �׷쿡 ������, �ѱ����)
	
	CString GetFieldChangeBtn();
	void	clickmovePageBtn(int type);
	int countInterest(int group);
	void SetCbPageView();
	void CheckPrevNextBtn();
	void SetViewType();

// window members
	CfxImgButton	m_btPREV;	// Ʈ�� ��ġ��
	CfxImgButton	m_btNEXT;	// Ʈ�� �����
	CfxImgButton	m_btUP;		// ���� ���̱�
	CfxImgButton	m_btDN;		// ���� ���̱�	
	CfxImgButton	m_btBASKET;	// �ٱ��� 
	CfxImgButton	m_btTEXT[2];	// ����/����/����
	CfxImgButton	m_btHAWKEYE;	// ȣũ����
	CfxImgButton	m_btMINUS;	// ���̳ʽ�
	CfxImgButton	m_btPLUS;	// �÷���
	CfxImgButton	m_btTool;	// ����
	CfxImgButton	m_btTURN;	// ��������
	CfxImgButton	m_btCHART;	// ��Ʈ ��Ʈ
	CfxImgButton	m_btAddSelect;	// ���̴� ȭ�鿡�� �߰�
	CfxImgButton	m_btAddVisible;	// ���̴� ȭ�鿡�� �߰�
	CfxImgButton	m_btAddFull;	// �ִ��� ��� ���� ȭ������ �߰� 
	CfxImgButton	m_btEqualizer;	// ���������� 
	
	CfxImgButton	m_btChangeField;	
	CfxImgButton	m_btPrevView;	
	CfxImgButton	m_btNextView;	
	CComboBox	m_cbArrange;
	CComboBox	m_cbViewType;
	CComboBox	m_cbPageView;
	
	CfxSpin*	m_spinTIME;		// �������� �ð�
	std::unique_ptr<class CIHCheck>		m_chkAUTO;		// Auto����
	class CIHCheck*	m_chkEXPAND;		// ���Ҹ�� 1 : Ȯ���ϸ鼭 �׷��߰� 0 : �����ϸ鼭 �߰�
	CBtnTab		m_btntab;
	
	
	CArray<CRect, CRect>	m_arDIVIDE;
	CArray<CWnd*, CWnd*>	m_arCONTROL;

	CRect		m_rcSECOND;
	CRect		m_rcFIELD;
	
	CComboBox	m_cbFIELD;
	CComboBox	m_cbGROUP;

	// ADD PSH 20070911
	CfxImgButton	m_btField;		// �ʵ� 
	CfxImgButton	m_btFieldDrop;
	CNewMenu		m_menuField;
	// END ADD

// data memebers
	CString		m_fileIMG;
	CString		m_fileFIELD;
	CString		m_filePPOLIO;
	CBrush*		m_pBrush;
	CString		m_activefield;	
	BOOL		m_bBig;	

	// ADD PSH 20070907
	COLORREF    m_clrOutLine;
	BOOL		m_bMoveCfg;
	int			m_nMoveSave;
	
	int			m_selectedTab;
	CCriticalSection	m_sync;
	// END ADD
};