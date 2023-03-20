#pragma once
// ToolWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolWnd window
#include "BaseWnd.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "fxSpin.h"
#include "btntab.h"


class CToolWnd : public CBaseWnd
{
	// Construction
public:
	CToolWnd(CWnd* pMainWnd);
	virtual ~CToolWnd();
	// Attributes
public:

	// Operations
public:

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
	

	// Generated message map functions
public:
	//{{AFX_MSG(CToolWnd)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeBtntab(NMHDR* pNMHDR, LRESULT* pResult);
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
	int	loadActiveField();

	void	savecfg();
	void	SetSel(int nIndex);
	void	SelectGroup(UINT kind);
	void	SendTree(int nIndex);
	int	Reload();
	void	SelectMode(int nmode);
	// ADD PSH 20079017
	void	RemoveAllMenu();
	// END ADD
	void testSaveFile();

	void dominoSelectGroup(int index);
	void saveGroupIndex(int index);		//���ɱ׷� index ����
	void SetGroupDataView();			//�׷��� ��� ǥ���Ұ��� ���� ��û(�� �׷쿡 ������, �ѱ����)
	void SetGroupWndActiveChk();
public:	
	void addGroup(std::vector<std::pair<CString, CString>>& vGroup);


	// window members
	std::unique_ptr<CfxImgButton>	m_btPREV;	// Ʈ�� ��ġ��
	std::unique_ptr<CfxImgButton>	m_btNEXT;	// Ʈ�� �����
	std::unique_ptr<CfxImgButton>	m_btUP;		// ���� ���̱�
	std::unique_ptr<CfxImgButton>	m_btDN;		// ���� ���̱�	
//	std::unique_ptr<CfxImgButton>	m_btBASKET;	// �ٱ��� 
	std::unique_ptr<CfxImgButton>	m_btTEXT[2];	// ����/����/����
//	std::unique_ptr<CfxImgButton>	m_btHAWKEYE;	// ȣũ����
	std::unique_ptr<CfxImgButton>	m_btMINUS;	// ���̳ʽ�
	std::unique_ptr<CfxImgButton>	m_btPLUS;	// �÷���
	std::unique_ptr<CfxImgButton>	m_btTool;	// ����
	std::unique_ptr<CfxImgButton>   m_btTURN;	// ��������
	std::unique_ptr<CfxImgButton>	m_btCHART;	// ��Ʈ ��Ʈ
	std::unique_ptr<CfxImgButton>	m_btAddSelect;	// ���̴� ȭ�鿡�� �߰�
	std::unique_ptr<CfxImgButton>	m_btAddVisible;	// ���̴� ȭ�鿡�� �߰�
	std::unique_ptr<CfxImgButton>	m_btAddFull;	// �ִ��� ��� ���� ȭ������ �߰� 

	std::unique_ptr <CfxSpin>		m_spinTIME;		// �������� �ð�
	std::unique_ptr<class CIHCheck>		m_chkAUTO;		// Auto����
	std::unique_ptr<class CIHCheck>		m_chkEXPAND;		// ���Ҹ�� 1 : Ȯ���ϸ鼭 �׷��߰� 0 : �����ϸ鼭 �߰�
	std::unique_ptr <CBtnTab>		m_btntab;


	CArray<CRect, CRect>	m_arDIVIDE;
	CArray<CWnd*, CWnd*>	m_arCONTROL;

	CRect		m_rcSECOND;
	CRect		m_rcFIELD;

	std::unique_ptr<CComboBox>	m_cbFIELD;
	std::unique_ptr<CComboBox>	m_cbGROUP;
	std::unique_ptr<CComboBox>	m_cbArrange;

	// ADD PSH 20070911
	std::unique_ptr<CfxImgButton>	m_btField;		// �ʵ� 
	std::unique_ptr<CfxImgButton>	m_btFieldDrop;
	CMenu		m_menuField;
	// END ADD

// data memebers
	CString		m_fileIMG;
	CString		m_fileFIELD;
	CBrush*		m_pBrush{};
	CString		m_activefield;
	BOOL		m_bBig{};

	// ADD PSH 20070907
	COLORREF	m_clrOutLine{};
	BOOL		m_bMoveCfg{};
	int		m_nMoveSave{};
	int		m_selectedTab{};
	// END ADD
};

