#pragma once


// CDlgSearchDlg ��ȭ �����Դϴ�.
#include "resource.h"
#include "../../../SrcSite/control/fx_misc/fxImgButton.h"

class CDlgSearchRng : public CDialog
{
	DECLARE_DYNAMIC(CDlgSearchRng)

public:
	CDlgSearchRng(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgSearchRng();

	void SetInit();
	CString m_stSavePath;// ȭ���ȣ : KHD
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SEARCHRNG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	
	CWnd*	m_pParentWnd;
	
	CfxImgButton	m_btnOk;
	CfxImgButton	m_btnCancel;

	CStatic	m_ctrlTopRect;
	CStatic	m_ctrlBottomRect;
	CButton	m_btnTopGroup;
	CButton	m_btnBottonGroup;
	
	CStatic m_ctrlNotice;

	CButton	m_ctrlIssuesForAdministration;		// ��������
	CButton	m_ctrlInvestAttention;				// ��������
	CButton m_ctrlInvestWarning;				// ���ڰ��
	CButton	m_ctrlPreInvestRisk;				// �������迹��
	
	CButton	m_ctrlInvestRisk;					// ��������
	CButton	m_ctrlDealStop;						// �ŷ�����
	CButton	m_ctrlTidyDeal;						// �����Ÿ�
	CButton m_ctrlAttentionInvestment;			// �������� -> ��������ȯ��

	CButton	m_ctrlInsincerityNotice;			// �Ҽ��ǰ���
	CButton m_ctrlPreferenceStock;				// �켱��

// 	CButton m_ctrlConfidencelimit;				// �ſ��������� - new
	CButton m_ctrlOver;							// �̻�޵�

	char    sUnder[1];				/* ��������                     */
	char	sInvestAttention[1];	/* ��������						*/
	char	sInvestWarning[1];		/* ���ڰ��						*/
	char	sPreInvestRisk[1];		/* �������迹��					*/

	char	sInvestRisk[1];			/* ��������						*/
	char    sStop[1];               /* �ŷ���������                 */
	char    sJrmm[1];               /* �����Ÿ�����                 */
	char	sAttentionInvestment[1];/* ��������ȯ��					*/
	
	char    sDishonest[1];          /* �Ҽ��ǰ�������               */
	char    sUsjg[1];				/* �켱��   (0:�ش����,1:�ش�) */
    
    char    sDrat[3];               /* ���űݺ���                   */
    char    sMrktalrm[3];           /* ����溸
                                       1:��������, 2:���ڰ��
                                       3:��������, 4:�������迹��	*/

	CButton	m_ctrlEquityCapital;				// �ں���
	CButton m_ctrlPriceRange;					// ���ݴ�
	CButton	m_ctrlAggregateValueOfListedStocks;	// �ð��Ѿ�
	CButton m_ctrlTradingVolume;				// �ŷ���
	CButton	m_ctrlParValue;						// �׸鰡

	CEdit	m_ctrlEquityCapitalAbove;					// �ں��� �̻� ����
	CEdit	m_ctrlEquityCapitalBelow;					// �ں��� ���� ����
	CEdit	m_ctrlPriceRangeAbove;						// ���ݴ� �̻� ����
	CEdit	m_ctrlPriceRangeBelow;						// ���ݴ� ���� ����
	CEdit	m_ctrlAggregateValueOfListedStocksAbove;	// �ð��Ѿ� �̻� ����
	CEdit	m_ctrlAggregateValueOfListedStocksBelow;	// �ð��Ѿ�	���� ����
	CEdit	m_ctrlTradingVolumeAbove;					// �ŷ��� �̻� ����
	CEdit	m_ctrlTradingVolumeBelow;					// �ŷ��� ���� ����
	CEdit	m_ctrlParValueAbove;						// �׸鰡 �̻� ����
	CEdit	m_ctrlParValueBelow;						// �׸鰡 ���� ����

	CStatic	m_ctrlEquityCapitalAboveText;
	CStatic	m_ctrlEquityCapitalBelowText;
	CStatic	m_ctrlPriceRangeAboveText;					
	CStatic	m_ctrlPriceRangeBelowText;					
	CStatic	m_ctrlAggregateValueOfListedStocksAboveText;
	CStatic	m_ctrlAggregateValueOfListedStocksBelowText;
	CStatic	m_ctrlTradingVolumeAboveText;				
	CStatic	m_ctrlTradingVolumeBelowText;				
	CStatic	m_ctrlParValueAboveText;					
	CStatic	m_ctrlParValueBelowText;					

	CBrush	m_brush;
	CFont*	m_pFont;
	CFont*	m_pFontBold;

	void SetInit_Data();
	void SaveValue();
	BOOL VerificationValues();
	BOOL VerificationInput(UINT ID);
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	afx_msg void OnCheckEquityCapital();				
	afx_msg void OnCheckPriceRange();					
	afx_msg void OnCheckAggregateValueOfListedStocks();	
	afx_msg void OnCheckTradingVolume();				
	afx_msg void OnCheckParValue();
	
	afx_msg void OnChangeEquityCapitalAbove();
	afx_msg void OnChangeEquityCapitalBelow();
	afx_msg void OnChangePriceRangeAbove();
	afx_msg void OnChangePriceRangeBelow();
	afx_msg void OnChangeOnCheckAggregateValueOfListedStocksAbove();
	afx_msg void OnChangeOnCheckAggregateValueOfListedStocksBelow();
	afx_msg void OnChangeTradingVolumeAbove();
	afx_msg void OnChangeTradingVolumeBelow();
	afx_msg void OnChangeParValueAbove();
	afx_msg void OnChangeParValueBelow();
};
