#pragma once
// ControlWnd.h : header file
//
//#include "InfoPopup.h"
//#include "jinfo.h"

#include "axwnd.h"
#include "symbolwnd.h"
#include "fxcodectrl.h"
#include "fximgbutton.h"


// m_nGubn ... �����ڷ�...
#define GU_NONE		-1
#define	GU_CODE		0	// �ֽ� 
#define	GU_FUTURE	1	// ���� 
#define	GU_OPTION	2	// �ɼ� 
#define	GU_INDEX	3	// ���� 
#define	GU_FOCODE	4	// ����  + �ɼ�
#define	GU_POPTION	5	// �ֽĿɼ� 
#define	GU_ELWCODE	6	// ELW 
#define GU_SCODE	7	// �����μ��� 
#define GU_BOND		8	// ä��(��ǰ)
#define GU_SBOND	9	// �Ҹ�ä��
#define GU_FCODE	10	// �ֽļ��� 
#define GU_FOSTOCK	11	// �ֽļ��� + �ɼ� 
#define GU_FOREIGN	12	// �ؿ�


/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CAxWnd
{
// Construction
public:
	CControlWnd(CWnd *pWnd);
	virtual ~CControlWnd();

	struct  _USCode {			/* �ؿ� SYMBOL ����             */
        char    symb[16];               /* ���� SYMBOL                  */
        char    dat1;                   /* ��з�			*/
		/* 0: ����      1: ����		*/
		/* 2: ����      3: �ݸ�		*/
		/* 4: ��ä      5: ȯ��		*/
		/* 7: others			*/
        char    dat2;                   /* �ߺз�(Combo Box)            */
		/* code09.jsgb                  */
		/* [����]                       */
		/* 1. �̱� 2.�ƽþ� 3.���� 4. MSCI 5. ��Ÿ */
		/* [����]                       */
		/* 1. �̱� 2.�Ϻ� 3.ȫ�� 4.�븸 5.���� 6.������ 7.���� 8.ADR 9.GDR */
		/* [����]                       */
		/* 1.CME�������� 2.������ 3.��ǰ����	*/
		/* [�ݸ�]                       */
		/* 1.�����ݸ�   2.�̱��ݸ�	*/
		/* [��ä]                       */
		/* 1.�̱� 2.�Ϻ� 3.���� 4.����	*/
		/* [ȯ��]                       */
		/* ����ȯ��                     */
        char    knam[40];               /* �ѱ� SYMBOL��                */
        char    enam[40];               /* ���� SYMBOL��                */
        char    icod[6];                /* ���� �����ڵ�                */
        char    ncod[3];                /* �����ڵ�                     */
        char    excd[4];                /* �ŷ����ڵ�                   */
	};


// Attributes
public:
	CString		m_sData, m_sRoot;
	COLORREF	m_boxColor, m_boxColorHover, m_bgCol, m_bgColHover;
	int		m_bgColor;
	CString		m_sCode;
	bool		m_ujongsise;	
	int		m_nFirst;	// ó�� �ѹ��� ...
	bool		m_bLastText;	// 4��° �ؽ�Ʈ 
	bool		m_bInit;	// ù��°�� �۵����� �ʰ�...
	bool		m_bCate;	// ī�װ� ��ư 
	bool		m_bSymbol;	// �ɺ��̸� 
	bool		m_bInter;	// ���� 
	bool		m_bText;	// ������ �ؽ�Ʈó��
	bool		m_bTrigger;	// Trigger  
	bool		m_bDomino;	// Domino
	bool		m_bFocus;	// ��Ŀ���� �ٸ� ������ ���� ���� ����.
	bool		m_bNext;	// �ֹ�ȭ��� �ƴ� ȭ�� ���� 
	bool		m_bModify;
	bool		m_bSearch;	// ������ �ؽ�Ʈó��
	int		m_nGubn;

	CRect		m_rcText;
	CRect		m_ClientRect;

protected:
	std::unique_ptr<CSymbolWnd>	m_pSymbol;
	std::unique_ptr<CfxCodeCtrl>	m_pCode;
	std::unique_ptr<CfxImgButton>	m_pBtnCategory;
	std::unique_ptr<CfxImgButton>	m_pBtnInter;
	
public: 
	CString loadCodeList(int nGroup);
	void	Send(CString szCode);
	void	SetEditFocus(BOOL bflag = FALSE);
	CString Variant(int iCommand, CString strData);

private:
	HINSTANCE	m_hCategory;
	CWnd		*m_pCategory;
	
	CFont*		m_pFont;
	BOOL		m_bTabStop;
	static bool	m_bTimer;

	// �̹��� �н�...
	CString		m_szBtnSearch;
	CString		m_szBtnHistory;
	CString		m_szBtnInter;
	CString		m_szBtnCategory;
	CString		m_Text1, m_Text2, m_Text3, m_Text4;

// Operations
private:
	void OnTrigger(CString str);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	void	SetParam(struct _param *pParam);
	void	InitControl(CRect rect);
	void	clean();
	CString	GetHNam(CString code);

protected: 

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBtnCode();
	afx_msg void OnBtnInter();
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	BOOL m_visible;
	afx_msg void OnVisibleChanged();
	afx_msg BSTR GetData();
	afx_msg void SetData(LPCTSTR lpszNewValue);
	afx_msg BOOL GetTabStop();
	afx_msg void SetTabStop(BOOL bNewValue);
	afx_msg short GetUnitData();
	afx_msg void SetUnitData(short nNewValue);
	afx_msg void SetProperties(LPCTSTR sParam);
	afx_msg BSTR GetProperties();
	afx_msg void SetCode(LPCTSTR code);
	afx_msg BSTR GetName();
	afx_msg void Text1(LPCTSTR str);
	afx_msg void Text2(LPCTSTR str);
	afx_msg void Text3(LPCTSTR str);
	afx_msg void Text4(LPCTSTR str);
	afx_msg void SetFocus();
	afx_msg void Trigger(LPCTSTR name, BOOL all);
	afx_msg void Enable(BOOL bflags);
	afx_msg void Clear();
	afx_msg void SetUnit(short unit);
	afx_msg short IsForeign(LPCTSTR code);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
private:
	bool Resize();
};

