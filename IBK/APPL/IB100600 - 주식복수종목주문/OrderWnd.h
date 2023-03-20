#pragma once
// OrderWnd.h : header file
//

#include "OrderGrid.h"
#include "AccountCtrl.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxEdit.h"
#include "../../control/fx_misc/fxImgButton.h"


struct	_jinfo {
	char    codx[12];		// �����ڵ�
	char    hnam[32];		// �����
	char    jgub[1];		// �屸�� : '1':�ŷ��� '2':�ڽ���, '3' :��3����, '5':ETF
	char    size[12];		// ����Ը�
	char    k2gb[20];		// �ұ��� ����,����
	char    usun[10];		// ������/�켱��
	char    ksmm[2];		// ����
	char    amga[20];		// �׸鰡 8->20 ��������
	char    jbkm[20];		// �ں���(����:��) 12->20 ��������
	char    siga[12];		// ���Ͻð��Ѿ�(����:��)
	char    sjjs[12];		// �����ֽļ�(����:õ��)
	char	jrat[3];		// ���ű���
	char	nrat[3];		// �ſ����ű��� 20070718 
	char    krgb[12];		// "�ŷ�����", "�ŷ��ߴ�", "��������", "�̻�޵�", "��������", "����"
	char    jggb[12];		// �� ����
	char    lock[12];		// ������
	char    dyga[8];		// ��밡
	char    ujgb[12];		// ��������
	char    ujcd[6];		// RTS �����ڵ�
	char    jisu[8];		// ��������
	char    diff[8];		// �������ϴ��
	char    rate[8];		// �������
	char    gvol[12];		// �����ŷ���(����:õ��)
	char    unio[10];		// KRX 100 (0:�Ϲ� 1:��������)
	
};
#define L_jinfo		sizeof(struct _jinfo)
/////////////////////////////////////////////////////////////////////////////
// COrderWnd window

class CMapWnd;

class COrderWnd : public CWnd
{
// Construction
public:
	COrderWnd(CWnd* m_pWizard);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderWnd)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	std::unique_ptr<CAccountCtrl> m_pAccount{};
	std::unique_ptr <CfxEdit> m_pPassword{};
	BOOL m_bInit{};
	int		m_iCmdH{};
	BOOL IsQuantityPriceType();
	void SetPallete();
	void SetInputFieldFocus();
	virtual ~COrderWnd();
	void Init();
	void SetHistory(CString strData);
	void ClearOrderGrid();
	CString GetHistory(CString strSymbol);
	CString GetOrderState();
	CString GetCreditType();
	CString GetOrderType();
	CString GetCondition();
	CString GetQuantity();
	CString GetOrderPrice();
	void ParsingSRemain(CString strData, int nLen, CString strMsg);
	void ParsingSCHoga(CString strData, BOOL bHoga, BOOL bStdPrc = FALSE);	//2015.06.12 KSJ ���ذ� ���ϱ� �߰�
	void ParsingSOrder(CString strData, int nLen, CString strMsg);
	void QuerySCHoga(BOOL bHoga, CString strCode = _T(""), BOOL bStdPrc = FALSE);	//2015.06.12 KSJ  ���ذ� ���ϱ� �߰�
	void SetAccount(CString strAccount);
	CString Parser(CString &srcstr, CString substr);
	inline CWnd* GetAccountCtrl() { return (CWnd*)m_pAccount.get(); };
	inline CWnd* GetWizard() { return m_pWizard; };
	inline BOOL IsBlocking() { return m_bBlock; };
	inline CMapWnd* GetMapWnd() { return m_pParent; };
	void ChangeSize(double* lRate, int ipoint, LPCTSTR lszname);
	void OrderSend();


	// Generated message map functions
protected:
	//{{AFX_MSG(COrderWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnClear();
	afx_msg void OnRemain();
	afx_msg void OnInterest();
	afx_msg void OnOrderSend();
	afx_msg void OnContrect();
	afx_msg void OnSelchangeOrderType();
	afx_msg void OnSelchangeCondition();
	afx_msg void OnSelchangePriceUnit();
	afx_msg void OnBuy();
	afx_msg void OnSell();
	afx_msg void OnQuantityValue();
	afx_msg void OnPercent();
	afx_msg long OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyPassword(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	bool IsNumber(CString str);
	CWnd* m_pWizard{};
	CMapWnd* m_pParent {};
	std::unique_ptr <COrderGrid> m_pOrderGrid;


	std::unique_ptr <CfxStatic> m_pAccName;
	std::unique_ptr <CfxStatic> m_pPassStatic;

	std::unique_ptr <CButton> m_pOrderConfirm;

	std::unique_ptr <CfxImgButton> m_pClear;
	std::unique_ptr <CfxImgButton> m_pRemain;
	std::unique_ptr <CfxImgButton> m_pInterest;
	std::unique_ptr <CfxImgButton> m_pOrderSend;
	std::unique_ptr <CfxImgButton> m_pContrect;
	
	std::unique_ptr <CButton> m_pBuy;
	std::unique_ptr <CButton> m_pSell;

	std::unique_ptr <CfxStatic> m_pOrderTypeStatic;
	std::unique_ptr <CComboBox> m_pOrderType;

	std::unique_ptr <CfxStatic> m_pConditionStatic;
	std::unique_ptr <CComboBox> m_pCondition;

	std::unique_ptr <CfxStatic> m_pQuantityStatic;
	std::unique_ptr <CButton> m_pQuantityShare;
	std::unique_ptr <CButton> m_pQuantityPrice;
	std::unique_ptr <CfxEdit> m_pQuantityValue;

	std::unique_ptr <CfxStatic> m_pPriceUnitStatic;
	std::unique_ptr <CComboBox> m_pPriceUnit;

	std::unique_ptr <CfxEdit> m_pPercent;
	std::unique_ptr <CfxStatic> m_pPercentStatic;

	BOOL m_bBlock{}, m_bFirst{};

	LPCTSTR				m_lszFontName{};
	int					m_iCurFont{};

};

