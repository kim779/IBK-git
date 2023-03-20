#pragma once
#include "Resource.h"
#include "AddControl/CheckTreeCtrl.h"
#include "../../DrCommon/Dr_Control/DrBtnCtrl.h"
#include "AddControl/Grid_Result.h"

#include "../../../SrcSite/control/fx_misc/fxImgButton.h"

#define MONTH_ALL				0
#define MONTH_MARCH				1
#define MONTH_JUNE				2
#define MONTH_SEPTEMBER			3
#define MONTH_DECEMBER			4
#define MONTH_ETC				5


// CDlgEditSubject ��ȭ �����Դϴ�.

class CDlgEditSubject : public CDialog
{
	DECLARE_DYNAMIC(CDlgEditSubject)

public:
	CDlgEditSubject(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgEditSubject();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_EDITSUBJECT };
	
public:
	CFont	m_fontDefault;
	CBrush	m_brushBk;
	CBrush  m_brushCtrl;
	CGrid_Result* m_gridResult;

protected:
	CfxImgButton	m_btnOk;
	CfxImgButton	m_btnCancel;

	CCheckTreeCtrl	m_treeCtrlMarket;

	CStringArray	m_saTargetItemCode;
	CStringArray	m_saTargetItemName;
	CMapStringToString	m_mapExeptItem;

	CButton m_btnExceptStockGroup;					
	CButton	m_btnCheckIssuesForAdministration;		// ��������
	CButton m_btnCheckDealStop;						// �ŷ�����
	CButton	m_btnCheckTidyDeal;						// �����Ÿ�
	CButton	m_btnCheckInsincerityNotice;			// �Ҽ��ǰ���
	CButton	m_btnCheckAttentionInvestment;			// ��������
	CButton	m_btnCheckNoticeAttentionInvestment;	// �������迹��
	CButton m_btnCheckPreferenceStock;				// �켱��
	CButton m_btnCheckDeposit;						// ���ű� 100% ����
	CButton m_btnCheckDangerInvestment;				// ��������
	CButton	m_btnCheckWarningInvestment;			// ���ڰ��

	CButton	m_btnCheckETF;							// ETF
//	CButton	m_btnCheckETN;							// ETN

	CButton		m_btnMonthSettlementGroup;
	CButton 	m_btnRadioAll;
	CButton		m_btnRadioMarch;
	CButton		m_btnRadioJune;
	CButton		m_btnRadioSeptember;
	CButton		m_btnRadioDecember;
	CButton		m_btnRadioEstival;
	CComboBox	m_comboMonths;
	CStatic		m_staticGuide;

	CStatic		m_stcTarget;
	CStatic		m_stcTotCntTargetItem;

	int m_nRadioMonth;
	CWnd* m_pReWnd;

	CString m_stSavePath;//KHD : 20110127
	CString m_strTreeName;

	CString m_strJangGubun;

public:
	BOOL GetUpMasterData(CStringArray *&psaCode, CStringArray *&psaName, int jgub);
	void GetStockMasterData(CString strJangGubun, CString strSelectItem, CString strSelectItemParent);
	void GetItemCodeFromThemeGroup(CString strThemeIndex);
	void RequestHaveItem(CString strAccNo, CString strText);
	void SendHaveItem(CStringArray* psaItemCode);
	void AllClear();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	void SetInit();
	void SetInitCtrl();
	
	void OnSize();
	void OnSizeTopRadio(CRect rect);
	void OnSizeTreeCtrl(CRect rect);
	void OnSizeExceptStock(CRect rect);
	void OnSizeMonthSettlement(CRect rect);

	void GetMonthSettlement(CStringArray* psaItemCode);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnRadioClick();
	afx_msg void OnBnClickedOk();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnNcDestroy();
	afx_msg void OnBtnExcept(UINT nID);
	afx_msg void OnChangeCombo();

//public:
//	static int getPortCodeList(LPCSTR stSavePath, CStringArray *pArrTargetResult);
};
