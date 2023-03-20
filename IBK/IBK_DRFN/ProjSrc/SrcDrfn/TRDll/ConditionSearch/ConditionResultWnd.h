#if !defined(AFX_CONDITIONRESULTWND_H__A2B5E2DE_C2D8_4111_9A91_2F218C543DFA__INCLUDED_)
#define AFX_CONDITIONRESULTWND_H__A2B5E2DE_C2D8_4111_9A91_2F218C543DFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConditionResultWnd.h : header file
//

#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
#include "DlgEditSubject.h"
#include "DlgSearchRng.h"
#include "RequestMsg.h"

/////////////////////////////////////////////////////////////////////////////
// CConditionResultWnd window
class CGrid_Result;
class CFieldEditDlg;
class CConditionResultWnd : public CWnd
{
	// Construction
public:
	CConditionResultWnd();

	CMapStringToPtr		m_MapUpCode;
	int Modual_SendTr(CString TrNo, int nTrLen, BYTE *TrData);
	HRESULT ReceiveData(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen , int nKey );
	COLORREF GetAxColor(int nColor);
	
#ifdef _DEBUG
	int m_nTickCnt, m_nAfterTickCnt;
#endif
	
private:
	
	CBrush		m_brushBk;
	
public:

	CWnd*			m_pwndMain;
	COLORREF		m_clrMainSkin;
	CFont			m_fontDefault, m_fontBold;
	BOOL			m_bInit;
	
	CButton			m_btnResultGroup;
	CfxImgButton	m_btnSearch;
	
	CStatic			m_ctrlTime;
	CComboBox		m_comboSearchTime;
	CfxImgButton	m_btnRollSearch;
	CfxImgButton	m_btnRollSearchStop;
	CfxImgButton	m_ctrlRealSearchBtn;
	CfxImgButton	m_btnPrint;

	CfxImgButton	m_btnFieldEdit;
	CfxImgButton	m_btnResultCtrl;
	CfxImgButton	m_btnExpand;

	CfxImgButton	m_btnLanguage;
	
	CGrid_Result*	m_pGridCtrl;
	CStatic			m_ctrlConditionTitle;
	
	CStatic			m_ctrlSearchTime;
	CStatic			m_ctrlPass;
	CProgressCtrl	m_progressPass;
	CStatic			m_ctrlPassResult;
	
	CStatic			m_ctrlCodeOKCnt_Text;
	CStatic			m_ctrlCodeOKCnt;
	
	CStatic			m_ctrlCodeTargetCnt_Text;
	CStatic			m_ctrlCodeTargetCnt;
	
	CStatic			m_ctrlMsg;

	CStatic			m_stcStatusBar;
	
	CFieldEditDlg*	m_pdlgFieldEdit;
	CDlgSearchRng*	m_pDlgSearchRng;
	
	// 20110628 ������ ��󺯰� ���� �κ� ���͸� ���� >>
	// KQ100, KQMid300, KQSmall �� ���ο��� �����ڵ尡 �Ѿ���� ����
	CStringArray		m_saKQ100;
	CStringArray		m_saKQMid300;
	CStringArray		m_saKQSmall;
	// 20110628 ������ <<

	int					m_nSeparateForm;

//	20130321 �̻�� : �ʿ���� ������ ---------->>
//	CStringArray		m_saFilterItemCode;
//	CStringArray		m_saMarket;				// ���� Ʈ�� ���� ����
//	CStringArray		m_saPortfolio;			// ��Ʈ������ Ʈ�� ���� ����
//	CStringArray		m_saRecommendation;		// ��õ Ʈ�� ���� ����
//	------------<<
	CStringArray		m_arTargetResult;
	CStringArray		m_arTargetMText;
	CStringArray		m_saDelItemCode;		// �׸����� ������ư���� ���ܵ� ����
	CMapStringToString	m_saPortfolioFilterMap;
	CStringArray		m_saMonth[12];

	BOOL 				m_bIssuesForAdministration;			// ��������
	BOOL 				m_bDealStop;						// �ŷ�����
	BOOL 				m_bTidyDeal;						// �����Ÿ�
	BOOL 				m_bInsincerityNotice;				// �Ҽ��ǰ���
	BOOL 				m_bAttentionInvestment;				// ��������
	BOOL 				m_bNoticeAttentionInvestment;		// �������迹��
	BOOL 				m_bPreferenceStock;					// �켱��
	BOOL 				m_bDeposit;							// ���ű� 100% ����
	BOOL 				m_bDangerInvestment;				// ��������
	BOOL 				m_bWarningInvestment;				// ���ڰ��
	BOOL				m_bETF;								// ETF
	BOOL				m_bETN;								// ETN

	BOOL				m_bOver;							// �̻�޵�

	BOOL				m_bInvestAttention;					// ��������ȯ��			

	BOOL				m_bEquityCapital;					// �ں���
	BOOL				m_bPriceRange;						// ���ݴ�
	BOOL				m_bAggregateValueOfListedStocks;	// �ð��Ѿ�
	BOOL				m_bTradingVolume;					// �ŷ���
	BOOL				m_bParValue;						// �׸鰡

	int					m_nEquityCapitalAbove;				// �ں��� �̻� ����
	int					m_nEquityCapitalBelow;				// �ں��� ���� ����
	int					m_nPriceRangeAbove;					// ���ݴ� �̻� ����
	int					m_nPriceRangeBelow;					// ���ݴ� ���� ����
	int					m_nAggregateValueOfListedStocksAbove;	// �ð��Ѿ� �̻� ����
	int					m_nAggregateValueOfListedStocksBelow;	// �ð��Ѿ�	���� ����
	int					m_nTradingVolumeAbove;				// �ŷ��� �̻� ����
	int					m_nTradingVolumeBelow;				// �ŷ��� ���� ����
	int					m_nParValueAbove;					// �׸鰡 �̻� ����
	int					m_nParValueBelow;					// �׸鰡 ���� ����

	int					m_nSelRadioMonth;					// ���� ���� ��ư ���� 0 = ��ü 1 = 3��...
	int					m_nIndexMonthCombo;					// ���� �޺����� ����
	CString				m_szTargetEditIndicate;				// ��󺯰� �ؽ�Ʈ

	CString				m_strJangGubun;

	CString				m_strEditSubjectSelectParentItemText;
	CString				m_strEditSubjectSelectItemText;

	CRequestMsg*		m_pCommMsg;

private:
	BOOL m_bIndustry;
	
	
	BOOL m_bCreateEnd;
	
	int m_nTrIndex;
	int m_nLastTrIndex;
	
	int m_nRollTime;
	User_Finder m_stUserFinder;
	CStringArray	m_saLanguageName;
	
	BOOL					m_bCodeViewFlag;
	
	int							m_nGridMenuCnt;
	CStringArray		m_saGridMenu;
	
	CString					m_strFixFieldVersion;		//�����ʵ� ����� XML file Version
	CString					m_strSection;
	
	XMLTreeInfoFile	*m_pstLoadedFile;
	
	int m_nTotCodeCnt;

	// ��󺯰��� ��Ʈ������, ��������
	CStringArray		m_saAccount;
	int					m_nRegJangoCount;

	int					m_nPreReceiveLen;

public:
	int m_nSearchType;		//# -1 : Init, 0 : Search, 1 : Run, 2 : Stop
	
	BOOL m_bSortUp;
	int m_nSortBaseCol;
	CString m_strSearchResult;				//#<= 1816 ��� ����
	
	CList <HeadInfoType, HeadInfoType&>		m_listFixHeadSys;
	CList <HeadInfoType, HeadInfoType&>		m_listSelectedHead;
	
	int		m_nFixCol;
	CList <HeadFixInfo, HeadFixInfo&>		m_listFixHeadUser;
	CList <HeadMoveInfo, HeadMoveInfo&>		m_listHeadMoveInfo;
	CList <HeadInfoType, HeadInfoType&>		m_listField;
	CList <HeadFixInfo, HeadFixInfo&>		m_listGridHeadUser;
	
	CMap <int, int, CString, LPCTSTR>		m_mapCandle;
	CMap <int, int, CString, LPCTSTR>		m_mapMemberComp;

	CList <ST_HAVEITEM, ST_HAVEITEM&>		m_listHaveItem;

	CMapStringToString						m_mapExeptItem;

	char m_sTargetBit[RESULT_MAX - RESULT_MAX_OFFSET];
	BOOL m_bJongFillterChk;
	
private:
	void OnSize_FunBar(CRect reClient);
	void OnSize_ResultBar(CRect reClient);
	int GetBtnWidth(int nTextLen) { return (int)BTN_WIDTH_BASE + (int)BTN_WIDTH_STEP * nTextLen; }
	
	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);

	BOOL FilterEditSubject(D1021OUT2* pstOutListPacket);
	BOOL FilterSearchRng(D1021OUT2* pstOutListPacket);

	int GetFillString(CString& _szGetStr, int nStart, int nEnd, int nOneWordSize);
	void OnSearchRngInit();
	
//	void SetEditSubjectTarGetBit();
	void GetItemCodeFromThemeGroup(CStringArray &arrCode, CString strThemeIndex);

private:
	int m_nLanSendCnt;
	int m_nToTalLanSendCnt;
	char* m_pScriptBody;
	char* m_pScriptIndex;
	void SetLanguage_SendTr();
	void SetLanguage_ReceiveSendTr();

	CDlgEditSubject* m_pDlgEditSubject;
	
public:
	void SetEditSubjectTarGetBit(BOOL bRecvTR = FALSE);
	BOOL	WriteTargetInfo(CString strSavePath);
	BOOL	ReadTargetInfo(CString strSavePath);
	int		GetProfileInt(CString strGKey, CString strKey, CString strSavePath);
	void SetInit();
	void SetTargetEditIndicate();

	void SetBtnExpandImage();
	
	void SetPopupMenuResultCtrl(CPoint ptMenu);
	
	void SetFieldEdit();
	void SetResultSave();
	void SetResultPrint();	// ����� ����Ʈ
	void SetSaveGridToExcel(CString strFileName);
	void SetSaveGridToText(CString strFileName);
	
	void SetLoadFixField();
	HeadInfoType GetMoveFieldPosInfo();
	CString GetVersion_FieldFile(MSXML::IXMLDOMNodePtr pNIndex);
	
	BOOL GetLoadFixField_Def(CStringArray	&saFieldId);
	BOOL GetLoadFixSysFieldData(MSXML::IXMLDOMNodePtr pNIndex,
		CList<HeadInfoType, HeadInfoType&> &listFixList);
	BOOL GetLoadFixUserFieldData(MSXML::IXMLDOMNodePtr pNIndex,
		CList<HeadFixInfo, HeadFixInfo&> &listFixList);
	BOOL GetHave_FixUserField(CString strField);
	void SetUpdateUserFieldData(CList<HeadInfoType, HeadInfoType&> *plistFixSys, 
		CList<HeadFixInfo, HeadFixInfo&> *plistFixUser);
	BOOL SetSaveFixUserFieldData(CList<HeadFixInfo, HeadFixInfo&> *plistFixUser,
		CString strVersion = (CString)XML_VERSION_USERFILE);
	
	void SetLoadCandle();
	BOOL GetLoadCandleData(MSXML::IXMLDOMNodePtr pNIndex, CMap <int, int, CString, LPCTSTR> &mapCandle);
	
	BOOL GetLoadXml(CString strFileRoot, MSXML::IXMLDOMDocumentPtr &pXmlDom);
	
	void SetLoadMemberComp();
	
	void SetControlEnable(BOOL bState, BOOL bRollSearch = FALSE);
	void SetSendMainMsg(CString strMsg);
	
	void SetAdd_MoveHead(HeadMoveInfo stHeadMoveInfo);
	void SetAllDelete_MoveHead();
	
	int GetCount_MoveHead();
	int GetCount_FixHead(BOOL &bHaveCodeFiled, CUIntArray &iaFixHeadId);
	
	void GetTrIndex(long &nTrIndex);

	CString GetSelectGridItemCode();
	
	void SetSearch_MSTCode();
	void SetSearch_WorkDate();
	BOOL LookUpSearchCenterModify(CString& strType, CString& strUserFolder, CString& strUserFileName);
	void SearchUserItem(CString& strUserFolder, CString& strFullName);
	BOOL LoadCondionItem(NODE pParent, XMLTreeInfoFile& Item);
	
	BOOL SetSearch(BOOL bFirstSearch = TRUE);
	void SetByteOrdering_In(User_Finder *pstUserFinder);
	void SetByteOrderRevDouble(LPSTR pDouble);
	
	BOOL SetReceiveData(LPTSTR pData, DWORD dwDataLen);
	void SetByteOrdering_Out(int nFieldCnt, OutListPacket *pstOutListPacket);
	
//	void SetGrid_AddData(int nRow, OutListPacket *pstOutListPacket);
	void SetGrid_AddData(int nRow, D1021OUT2* pstOutListPacket);

	CString GetFormatStr(long lData, int nUnitNum, BOOL bFormatFlag, CString strFormat);
	void SetSearchResultState(LONG lCnt = 0, LONG lTime = -1);
	
	void SetGrid_Head(BOOL bView = FALSE);
	void SetGrid_ClearAll();
	void SetClearAll();
	
	void SetLoadedFile(XMLTreeInfoFile	*pstLoadedFile);
	void SetLoadedFile(CString strFileName);

	void ChangeSkinColor(COLORREF clrSkin);
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);

	void OnClickSearchBtn();
	void OnClickPrintBtn();
	void OnClickEditBtn();
	void OnClickSendBtn();
	void OnClickRealSearchBtn();
	void OnChangeSubject();
	void OnChangeSubjectInit();
	void OnSearchRng();
	void GetHaveItem();
	void SetFieldEditApply();
	void MakeUpCodeList(CStringArray* arrCode, CStringArray* arrName);
	ST_UPCODE* GetUpCode(CString strItemCode);
	void OnDBClickResultGrid(CString strCode); // 20110118 ������ �˻� ��� Double Click �� ���� ���� ���� ���
//	void ParsingCommaUpJongData(int nMarketType, CString stUpJong);// KHD : �����ͷε� �Ľ̷�ƾ

	CString Variant(int nComm, CString strData = "");
	void GetData_PortGroup(CStringArray& saKey, CStringArray& saGroupName);
	BOOL GetData_PortItem(CStringArray& saCode, CStringArray& saName, int nKey);
	BOOL GetUpMasterData(CStringArray *&psaCode, CStringArray *&psaName, int jgub);
	BOOL GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub);
	void SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold = FALSE);
	void SetBtnImg(CfxImgButton *pBtn, CString strFileName);
	CWnd* GetModuleMainWnd();

	int SetResultTemp();

	void GetAccNo(CStringArray* psaAccNo);
	void RequestHaveItem(CString strAccNo);
	void SendHaveItem(CStringArray* psaItemCode);
	void SetTargetBit();
	void SetTargetBit(CStringArray& saExceptCode);

	void GetMonthSettlement(CStringArray* psaItemCode, int nMonth);
	void RemoveCharFromString(CString &parm_string, char parm_remove_char);

	void SetSaveDefalutChangeSubject();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConditionResultWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CConditionResultWnd();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CConditionResultWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG

	afx_msg void OnBtnPrint();	
	afx_msg void OnBtnSearch();
	afx_msg void OnBtnRollSearch();
	afx_msg void OnBtnFieldEdit();
	afx_msg void OnBtnResultCtrl();
	afx_msg void OnMenuResultCtrl(UINT nIndex);
	afx_msg void OnBtnExpand();
	afx_msg void OnBtnRealSearch();	
	//afx_msg void OnNotify_GridEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotify_GridEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReqJangoData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBtnLanguage();
		
	//# �Ｚ����
	afx_msg long OnGetData(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	
	DECLARE_MESSAGE_MAP()

public:
	void InMakeFid_P0621(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos);
	int InMakeTr_P0621(BYTE *pTrData, BYTE* pTrOutData);

	void InMakeFid_Mstcode(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos);
	int InMakeTr_Mstcode(BYTE *pTrData, BYTE* pTrOutData);

	void InMakeFid_Workdata(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos);
	int InMakeTr_Workdate(BYTE *pTrData, CString &szInputData);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	void _DoGridWidthSet(int nOption);		//0000207
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONDITIONRESULTWND_H__A2B5E2DE_C2D8_4111_9A91_2F218C543DFA__INCLUDED_)
