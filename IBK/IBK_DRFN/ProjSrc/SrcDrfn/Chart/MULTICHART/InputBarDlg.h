//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_INPUTBARDLG_H__88AA28E0_917F_485D_BE10_0171E8C2E60B__INCLUDED_)
#define AFX_INPUTBARDLG_H__88AA28E0_917F_485D_BE10_0171E8C2E60B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputBarDlg.h : header file
//
#include "RscDlg.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CInputBarDlg dialog

#include "PopupList.h"
#include "../../inc/ExOCXDLL.h"
//#include "../../inc/ExCodeInput.h"
#include "./control/EditUsr.h"
#include "../chart_common/ILButton.h"
#include "../../../SrcSite/include/CodeCtrl.h"
#include "../../../SrcSite/include/FOHistoryBtnCtrl.h"

#ifndef __HANHWA_CHART_REPLAY_INFO_H
	#include "../chart_common/ReplayChartInfoDef.h"
#endif

#include "./Control/TextProgressCtrl.h"
#include "./define/ChartMsgDef.h"				// �������� - ojtaso (20070223)

#define CNTOFMAXBUTTONS (6)
#define CNTOFMAXCBITEMS (12)

#define STOCK_STROPTION		"KZZ"		//�ֽ� Option
#define KOSPI_STRFUTOPT		"FUT&OPT"	//Kospi Fut/Option
#define KOSDAQ_STRFUTOPT	"KFX&KFO"	//Kosdaq Fut/Option

typedef struct BEFORE_INFO
{
	char chOldType; //���� <->���� �ƴѰ� ���� ����� ����
	int  nInterval; //��ȸ ����
	BOOL bTimeSync; //�ð�(ƽ/��/����....) ���� ����
	BOOL bShowExtToolBar; //�ϴ� ���� �������� ����.

} BEFORE_INFO;


class CStdDialog;
class CChartMng;
class CInputBarDlg : public CRscDlg
{
public:
	CStdDialog* m_pStdDialog;
	//>>>>>>>------------------------------------------------------------------------
	//Author	:	Sang Yun Nam
	//Date		:	2006.01.09
	//Reason	:   ���� ���α׷��� �� ǥ�� 
	//				
private:
	int	 m_nPosOfPrgBar;
	int	 m_nTotalSec;
public:
	void Add_nPosOfPrgBar()			{ m_nPosOfPrgBar++;			}
	int  Get_nPosOfPrgBar()			{ return m_nPosOfPrgBar;	}
	void Init_nPosOfPrgBar()		{ m_nTotalSec = 0; m_nPosOfPrgBar = 0;	}

	//>>>>>>>------------------------------------------------------------------------
	//Author	:	Sang Yun Nam
	//Date		:	2005.09.08
	//Reason	:   ����ڰ� �Է��� ���� �����Ͽ� ��ȸ�� ���� �ƴϸ� 
	//				������ ����� ���� �̿��Ͽ� ��ȸ�� ���� ���� �ϱ� ���� 
private:
	BOOL m_bUseUserInputPeriod;
public:
	void SetUseUserInputPeriod(BOOL bUse)	{ m_bUseUserInputPeriod = bUse;	}
	BOOL GetUseUserInputPeriod()			{ return m_bUseUserInputPeriod;	}
	//<<<<<<<<<----------------------------------------------------------------------

public:	
	//���� �̹��� ����Ʈ
	CImageList* m_pIL_W20H20;
	CImageList* m_pIL_W50H20;
//@�������� CImageList* m_pIL_W15H20;
	CImageList* m_pIL_W17H20;
	CImageList* m_pIL_W46H20;
//@��������	CImageList* m_pIL_W43H19;

	//>>>>>>>------------------------------------------------------------------------
	//Author	:	Sang Yun Nam
	//Date		:	2005.08.30
	//Reason	:   ���� ���õ� ��Ʈ�� ����� Set & Get
	CString m_strJonMokName;  // 05. 08. 30 added by sy, nam
	void	SetCurCodeNameToControl(CString strName);
	CString	GetCurCodeNameFromControl() { return m_strJonMokName; }

	//���� �ڵ�� ����� ��������.
	CString GetJongMokNameByCode(CString strCode, LPCTSTR lpszTRCode = NULL);
	int GetMarketTypeFromTRCode(LPCTSTR lpszTRCode);
	//<<<<<<<<<----------------------------------------------------------------------


	//>>>>>>>------------------------------------------------------------------------
	//Author	:	Sang Yun Nam
	//Date		:	2005.08.29
	//ȯ�浿��ȭ�� ��Ʈ���� ���������� Selelct�� �Ǹ鼭 
	//UMSG_SETINITIALCHARTDATA �޼����� �߻����Ѽ� 
	//InputBarDlg Class���� ȣ�� TR�� �����Ƿ� 
	//�̶��� ������ ȣ�� TR�� ������ ���ϵ��� �Ѵ�.
	BOOL m_bCanRequestHogaTR; //ȣ�� TR�� ���� �� �ִ��� ������ ���� 
	void SetStopRequestHogaTR(BOOL bCanRequest) { m_bCanRequestHogaTR = bCanRequest; }
	//<<<<<<<<<----------------------------------------------------------------------

	//------------------------------------------------------------------------
	//Author	:	Sang Yun Nam
	//Date		:	2005.08.11
	//���� ��Ʈ �� �ƴ� ��Ʈ ���� ���������� �����Ѵ�.
	BEFORE_INFO m_ExChangeBeforeInfo;
	//========================================================================

	//���� ��Ʈ ��� ���� �ѱ��.
	long GetReplaySpeed();

	//���� ���� ���� �޼��� 
	void ReplayFail();
	//���� ��
	void ReplayEnd();

	//------------------------------------------------------------------------
	//Author	:	Sang Yun Nam
	//Date		:	2005.08.11
	//���� ��Ʈ Start �ð��� �ѱ��.
	long GetReplayStartTime();

	//���� ��Ʈ ���� n������ n���� �ѱ��.
	long GetReplayDayBefore();
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	//Author	:	Sang Yun Nam
	//Date		:	2005.08.10
	//Color Table ����� ȣ�� - ��Ʈ�� ColorTable �� ����.
	void ChangeButtonColor();
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	//Author	:	Sang Yun Nam
	//Date		:	2005.08.09
//@��������
//	int		m_nDataAddMode; //��ȯ/�߰�/��ø/���� 
//@��������
	REPLAY_INFO* m_pReplayInfo;
	BOOL	m_bRunningReplay; //���� ���� ����
	BOOL	m_bPauseReplay; //���� �Ͻ����� ����
	BOOL	m_bStop;
	//------------------------------------------------------------------------
	

	//------------------------------------------------------------------------
	//Author	:	Sang Yun Nam
	//Date		:	2005.07.08
	//Comment	:	�����ɼ� ��Ʈ������ �����Ѵ�.
	//				������ ������ �ȵǾ� �ε��� ���� ���� ���е� ���·� �����ϵ��� flag���� ���´�.
	//------------------------------------------------------------------------		
	enum	{KOSPI_FUTOPT, KOSDAQ_FUTOPT, STOCK_FUTOPT,  STAR_FUTURE	};
	int		m_nFutOptType	; // 0 - �Ϲ� �����ɼ� , 1 - �ڽ��� �ɼ� , 2 - �ֽĿɼ� 
	DWORD	m_dwDataKind	;
	//CString m_strFutOptType	; // KOSPI_STRFUTOPT - �Ϲ� �����ɼ� , KOSDAQ_STRFUTOPT - �ڽ��� �ɼ� , STOCK_STROPTION - �ֽĿɼ� 
	




	//------------------------------------------------------------------------
	//Author	:	Sang Yun Nam
	//Date		:	2005.06.01
	//Comment	:
	//------------------------------------------------------------------------
private :
public:
		CString		m_strCode;

public:
//@��������
//	void	SetItemCode(CString strCode){m_strCode = strCode;};
//	void	SetCurChartMainCode(CString strCode);
//	CString GetCurChartMainCode();
//@��������
//@�����߰�
	void	SetCurrentCode(CString strCode);
	CString GetCurrentCode();
//@�����߰�

	CString		m_strCodeFromOutside;
	CString		m_strOldCode;
	// bRequestAP Paramater is inserted by gm0604 2005/9/7
	void		InitBaseInfo(CBaseDesk2* pBaseDesk, BOOL bRequestAP = TRUE); //CodeInput

	void		SetKeyCodeChange(WPARAM wParam, LPARAM lParam);	
	//008 {{ KOSCOM �ڵ��޺� �ڽ��Ǿ���/�ڽ��ھ������� ���е�
	void		InitUpjongCodeCombo(UINT nUpjongType);		// �ڽ���/�ڽ��ڱ��п� ���� �ڵ��޺� ����
	//008 }}

	//------------------------------------------------------------------------
	//Author	:	Sang Yun Nam
	//Date		:	2005.05.24
	//Comment	:
	//------------------------------------------------------------------------
	int m_nCurSelCol;
	int m_nCurSelRow;
	int m_nTick;
	int m_nSec;
	int m_nDay;
	int m_nMin;
	int m_nHour;
	//CString m_strCfgFile;

	//------------------------------------------------------------------------
	//Author	:	Sang Yun Nam
	//Date		:	2005.04.20
	//Comment	:
	//------------------------------------------------------------------------
	// ��ȭ���ǿ�����	m_cbMarket�� �޺��ڽ��� ���ֹ��� �����
	// ��ü������		m_nChartType ������ �߰��Ͽ� ����Ѵ�.
//@��������
//	enum	{STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, ELW_CHART, FOREIGN_CHART, COMMODITY_CHART};	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
	//	int		m_nChartType;  // 0 - �ֽ���Ʈ, 1 - ���� ��Ʈ , 2 - �����ɼ���Ʈ
//	void	SetChartGubun(int nGubun) {m_nChartType = nGubun;} 
//	BOOL	SetMarket(UINT nMarketType,BOOL bUpdateCbMarket, BOOL bRequestToServer);
//	void	SetMarKetToolType(int nMarketType);
//@��������
//@�����߰�
	int		m_nMarketType;  // 0 - �ֽ���Ʈ, 1 - ���� ��Ʈ , 2 - �����ɼ���Ʈ
//	int		GetMarketType(CString strCode);
	void	SetMarketType(int nMarketType) { m_nMarketType = nMarketType; } 
	BOOL	ChangeMarket(UINT nMarketType, BOOL bUpdateCbMarket = TRUE, BOOL bRequestToServer = FALSE);
//@�����߰�
	//------------------------------------------------------------------------

	CInputBarDlg(CWnd* pParent = NULL);   // standard constructor
	//void	SetpDBMgr(CDrMLib_CDBMgr* pDBMgr) { m_pDBMgr = pDBMgr;	} //2005. 05. 12
	//void	SetpCtrlCode(CAny2Code* pctrlCode) {m_pctrlCode = pctrlCode;}
	void	SetpChartMng(CWnd *pWnd) { m_pChartMng = pWnd; }
	void	SetBaseDesk(long lKey) { m_lBaseDesk = lKey;} 
	void	HideLeftBarBtn(int nCmdShow);
	BOOL	IsMiniBar() { return m_bIsMiniBar;} 
// Dialog Data
	//{{AFX_DATA(CInputBarDlg)
	enum { IDD = IDD_INPUTBAR };
//@��������	CILButton	m_btChangeUpjong;
//@��������	CILButton   m_btEtcMenu;	
	CILButton	m_btSaveChart;
	CComboBox	m_cmbBong;
	CILButton	m_btPeriod;
//@��������	CILButton	m_btSetEnvir;
	CTextProgressCtrl	m_prgBarReplay;
	CStatic	m_staticReplayTime;
	CExComboBox	m_cbMarket;
	CButton		m_btnStop;
	CExComboBox	m_cmbDayBefore;
	CILButton	m_chkFirst;
	CILButton	m_chkSecond;
	CILButton	m_chkThird;
	CILButton	m_chkFourth;
//	CILButton	m_chkFifth;
//	CILButton	m_chkSixth;
//	CILButton	m_chkSec30;
//	CILButton	m_chkSec10;
	CILButton	m_btnStopReplay;
	CILButton	m_btnHandleReplay;
	CExComboBox	m_cmbReplayHour;
	CExComboBox	m_cmbReplayMin;
	CExComboBox	m_cmbReplaySpeed;
	CExComboBox	m_cmbChartViewType;
	CILButton	m_btnForSendTR;
//	CILButton	m_btnFutOptFlag;
//	CILButton	m_btnFutOptCodeDlgShow;
//	CILButton	m_chkDow;
//	CILButton	m_chkTWD;
//	CILButton	m_chkSP500;
//	CILButton	m_chkNiKei;
//	CILButton	m_chkNasDaq;
	CILButton	m_chkDataListWnd;
	CComboBox	m_cmbChartType;
	CExStatic	m_stJongName;
	CILButton	m_chkDay;
	CILButton	m_chkWeek;
	CILButton	m_chkMonth;
	CILButton	m_chkYear;
	CILButton	m_chkMinute;
	CILButton	m_chkSec;
	CILButton	m_chkTick;
//@��������	CILButton	m_chkHour;
	CExEdit		m_editPeriod;
	CEditUsr	m_editCntOfDatAll;
	CStatic		m_stText;
	BOOL	m_bChkRevised;
	BOOL	m_bChkLog;
	CStatic		m_stcDate;
	CILButton	m_btJumun;
	CILButton	m_btnGwanSim;
	//}}AFX_DATA
//	CExCodeInput	m_ctrlCodeInput;
	CCodeCtrl*	m_pCodeInput;
	CRect m_rectCmb;
	void _DoSetPosCodeCodeName(CRect& rcCode);	//@Solomon

	CFOHistoryBtnCtrl*	m_pFOHisBtn;
	afx_msg void OnSelendokFOHistoryBtn();

//	HWND	m_hKSJongName;
	CRect	m_rectJongName;
//	CCodeNameCtrl*	m_pCodeNameCtrl;	//@Solomon

//@��������
//	CExCodeInput*	m_pCtrlCodeInput;
//	CExCodeInput*	m_pCtrlCodeInputStock;
//	CExCodeInput*	m_pCtrlCodeInputFuture;
//	CExCodeInput*	m_pCtrlCodeInputUpjong;
//	CExCodeInput*	m_pCtrlCodeInputELW;
//	CExCodeInput*	m_pCtrlCodeInputForeign;
//	CExCodeInput*	m_pCtrlCodeInputCommodity;		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//@��������

private:
	
//	CILButton	m_chkExtBar2;
	CILButton	m_chkExtBar1;
//	CILButton	m_btResetPeriod;
//	CButton			m_chkFixPeriod;
	CSpinButtonCtrl	m_spZoomValue;
//	CILButton	m_btFixPeriod;
	CILButton	m_btRequestMore;
//	CILButton	m_chkRight;
	CILButton	m_chkLeft;
	CILButton	m_btInc;
//	CILButton	m_chkExt2;
//	CILButton	m_chkExt1;
//	CILButton	m_chkExt6;
//	CILButton	m_chkFirst;
//	CILButton	m_chkFourth;
//	CILButton	m_chkThird;
//	CILButton	m_chkSecond;
	CStatic			m_stDiv;
	CEditUsr		m_editZoomValue;
//	CDateTimeCtrl	m_datePicker;
	CButton		m_chkQueryOpt;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBarDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//CDrMLib_CDBMgr* m_pDBMgr;
	long m_lCntInView[8]; // 6->8�� ���� 10�� 30�� �߰� 05.09.28
	long m_nCntInViewHanHwa;
	CBrush	m_brBkColor;
	COLORREF m_crBk;
	UINT *m_pnLeftBarType;
	CWnd *m_pChartMng;
//	CAny2Code* m_pctrlCode;
	char m_chType;		// ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5)
	char m_chOldType;	// 2005.05.20 - ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5)
	int  m_nUnitPos;  // 2005.05.20 - ������� �ʴ´�.
	BOOL m_bSettingInitData;
	CString m_strImageChartPath;
	CString m_strImagePath;
	BOOL m_bIsMiniBar;
	UINT m_nPosCbMarketOld;
	void LoadValue();
	void SetExt(int nAct);
	void RemoveHistoryInfo();
	BOOL ChangeEditCode(BOOL bFocus, BOOL bDontPutInShared =FALSE);
	CString GetCodeNameAndGubun(CString csCode);
	BOOL AddHistoryInfoListUpjong(CString szKey, CString szName, BOOL bShareCodeList=TRUE);
	BOOL AddHistoryInfoListJongmok(CString szKey, CString szName, BOOL bShareCodeList=TRUE);
	BOOL AddHistoryInfoListForeign(CString szKey, CString szName, BOOL bShareCodeList=TRUE);

	BOOL LoadInfoFutureList();
	virtual void SetType(BOOL bSendChartMng = TRUE); // UpdateBy m_chType(ƽ(0)/��(1)/��(2)/��(3)/��(4)) and m_nUnitPos
	CPopupList*		m_dlgPopupListForHistory;
	CHistoryList	m_lstHistoryInfo;
	CHistoryList	m_lstHistoryInfoUpjong;
	CHistoryList	m_lstInfoFuture;
	CHistoryList	m_lstHistoryInfoForeign;

//	// ����, �ؿ� �ڵ� ���� - ojtaso (20080115)
//	CString			m_strForeignCode;
//	CString			m_strUpjongCode;

	long m_lBaseDesk;
	long GetBitFromCodeInfoUpjong();
	long GetBitFromCodeInfoJongmok();
	
	virtual void Init();
	virtual void LoadIniValue();
	virtual void SaveIniValue();
	virtual void SetCodeName(int nMarketType, LPCSTR szCode, LPCSTR szName){};

public:	
	int	 GetEndDate();
	void SetReplayEachSec(CString sTime);
	void SetReplayTotalSec(int nTotalSec);
	
	void ChangeToolBarStatusForReplay(int nReplayMode);
	void SetEditZoomValue(CString strValue, BOOL bSetInput=FALSE, BOOL bSetToOCX=TRUE);
	void ChangeChartCode(CString strCode);
	void ChangeToolBarForReplayChart(BOOL bReplay);
	void Replace_FutOptControl();
	void Replace_ForeignChartControl();

	void SetFocusCodeInput();
	void SaveChartType_IniFile(int nType);

	void SetCheckRevised(BOOL bRevised);
	void Enable_PeriodControl(BOOL bEnable);
	void SetLeftBarType(UINT nLeftBarType);
	void SetShowExtInputBarDlg(BOOL bShowExtInputBarDlg);
	void UpdateBtNCbValue();
	void SaveValue();
	char GetType() { return m_chType;}
	virtual long GetTypeNumber();
	void SetMode(int nMode);
	int  GetUnitPos() { return m_nUnitPos;	}

	LONG m_lValueOfButton[CNTOFMAXBUTTONS];			// �� �ֱ�
	LONG m_lValueOfMinButton[CNTOFMAXBUTTONS];		// �� �ֱ�
	LONG m_lValueOfSecButton[CNTOFMAXBUTTONS];		// �� �ֱ�
	LONG m_lValueOfTickButton[CNTOFMAXBUTTONS];		// ƽ �ֱ�
	LONG m_lValueOfHourButton[3];			// �� �ֱ�
	long m_lValueOfCombo[CNTOFMAXCBITEMS];

	IMasterDataManagerLast* m_pDataManager;
	
	void SetCodeInit();
//@��������	void SetUseReplayChartMode(BOOL bUseReplayChartMode) { m_bUseReplayChartMode = bUseReplayChartMode; }

	//2006.01.20 by LYH
	void SetAllPosition(int nType);

	//�Ⱓ �����϶� ���̴� ����Ÿ�� �������� ����;
	BOOL m_bChkPeriod;
	int  m_nFixZoomValue;

	void SetViewCount(long nViewCount);
	void GetViewCount(long &nViewCount);
	int GetAllCount();
	int m_nUpjongType;

	int m_nUnitValue;
	void ResetUnitValue(BOOL bValueSet);

	BOOL SetCodeDragAndDrop(CString strCode, int nGetMarketType=-1);
//@��������
//	int GetMarketType(CString strCode);
//	CString GetLastKey(int iKeyType);
//@��������
//@�����߰�
	CString GetLastHistoryCode(int nMarketType);
//@�����߰�
	BOOL m_bChkSaveJipyoOneFile;
	//2007.02.08 by LYH
	BOOL m_bChkLoadSaveCode;
	//2007.07.23 by LYH
//@��������
//	BOOL SetRecentCode();
//	CString m_strRecentCode;
//@��������
//@�����߰�
	void SetLastHistoryCode();
	int m_nCurrentMode;
	int GetCurrentMode(int nDefault = MODE_DEFAULT, int nMode = CHANGE_ITEM);
	
	void SetShareCodeChanged(BOOL bShareCodeChanged);
	BOOL GetShareCodeChanged()	{ return m_bIsShareCodeChanged; }
	BOOL m_bIsShareCodeChanged;
	
	BOOL GetChartViewType(CStringList& listViewType);
	void SaveInputBar();
	void RequestReplayMode();
//@�����߰�

	// �������񿡼� ���õ� ��Ʈ ���� ǥ�� : �������� - ojtaso (20070223)
	void InitChartMulitItem(ST_SELCHART* pSelChart);
	BOOL m_bSelectAction;	// ��Ʈ ������ ����Ǿ� �����ڵ尡 ����Ǵ� ��� : �������� - ojtaso (20070323)

	// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
	BOOL GetReplayChartMode();
	// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
	void SetReplayChartMode(BOOL bUseReplayChartMode);

	// ������Ʈ ��뿩�� : �������� - ojtaso (20070530)
	BOOL m_bUseReplayChartMode;
	// �����ڵ带 ������ ������ : �������� - ojtaso (20070612)
	BOOL m_bReceiveDataSetCode;
	// ���������� Total Count�� ��Ƽ��Ʈ�� �˷��� : �������� - ojtaso (20070704)
	void SetMultiTotalCount(int nTotalCount);

	void SetMultiChartItemMode();

	// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)
	void EnableRequestMoreButton(BOOL bEnable);
	void ChangeSkinColor();

	void VerifySettingData(LONG& lData);

	virtual void CreatedChartMng() {};
	virtual void RecievedChartMng() {};
	virtual void MoveCodeInputCtrl() {};

	int		GetMarketStrToIndex(CString strMarket);
	int		GetMarketIdxToCBIdx(int nIndex);
	int		InitMarketCombo();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

protected:
	void _DoChangeCodeCtrlUnit(int nMarketType, LPCSTR szCode=NULL);
	void _DoCheckMarketTypeFromScreen(int &nMarketType);

	BOOL _DoPreCheckCodeCtrlUnit();
	bool m_bSetFixUnit;

	void	_DoReplyResourceCheck();	//���� Resourceüũ.
	void	Init_Size();

	// Generated message map functions
	//{{AFX_MSG(CInputBarDlg)
	virtual void OnCancel(){}
	virtual void OnOK(){}
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtleft();
	afx_msg void OnBtright();
	afx_msg void OnChkday();
	afx_msg void OnChkweek();
	afx_msg void OnChkmonth();
	afx_msg void OnChkminute();
	afx_msg void OnChksec();
	afx_msg void OnChktick();
	afx_msg void OnDeltaposSpinzoomvalue(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtrequestdatmore();
	afx_msg void OnDestroy();
	afx_msg void OnBtinc();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtresetperiod();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnChkextbar();	
	afx_msg void OnBtinitial2();
	afx_msg void OnChkshowscale2();
	afx_msg void OnChkyear();
//@��������	afx_msg void OnChkDatalist();
	afx_msg void OnDeltaposSpinDatecnt(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChkRevised();
	afx_msg void OnSelendokCmbCharttype();
	afx_msg void OnButton1();
	afx_msg void OnChkForeignMainCode(UINT nID);	
	afx_msg void OnBtnForeignSendtr();
	afx_msg void OnSelchangeCmbType();
	afx_msg void OnBtnPlayPause();
	afx_msg void OnBtnStopReplay();
	afx_msg void OnSelchangeCmbSpeed();
	afx_msg void OnSetfocusEditperiod();
	afx_msg void OnChangeEditperiod();
	afx_msg void OnChkfirst();
	afx_msg void OnChksecond();
	afx_msg void OnChkthird();
	afx_msg void OnChkfourth();
//	afx_msg void OnChkfifth();
//	afx_msg void OnChksixth();
	afx_msg void OnSelchangeCmbMin();
	afx_msg void OnSelchangeCmbHour();
	afx_msg void OnSelchangeCmbDay();	
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeCbmarket();
	afx_msg void OnBtperiod();
	afx_msg void OnBtsetenvir();
	afx_msg void OnBtmultichart();
	afx_msg void OnSelchangeCodeInput();
	afx_msg void OnBtSaveChart();
//@��������	afx_msg void OnChkhour();
//@�������� afx_msg void OnBtetcmenu();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnCheckDow();
	afx_msg void OnCheckNasdaq();
	afx_msg void OnCheckNikei();
	afx_msg void OnCheckSp500();
	afx_msg void OnCheckTwd();
	afx_msg void OnChkQueryOpt();
	afx_msg void OnBtJumun();
	afx_msg void OnBtnGwansim();
	//}}AFX_MSG
//	afx_msg virtual void OnBtperiod();
	LRESULT OnSelBasedlgPopuplistctrl(WPARAM wParam, LPARAM lParam);
	LRESULT OnFromCodeList(WPARAM wParam, LPARAM lParam);
//	LRESULT OnTestTR(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnSetfocusEditFocus();
	afx_msg void OnBnClickedBtchangsypjong();

	//@Solomon-->
	void DoSelchangeCodeInput();
	//@Solomon<--

	//@Sol-100104SM005 ���չ������� �ǽð�ó���ø��� ZoomValue�� ������ �־ �����ϱ� ���� �߰���.
	CString m_szOldZoomValue;

	BOOL GetOldFututureCodeName(LPCSTR szCode, CString& szCodeName);
	BOOL m_bInitChartData;	//ZoomValue�� ��ȸ�� ���� ���������� Ȯ���ϱ����� �߰�.

	int GetComboMarket(); //�޺��� ���õ� market Type.
	void SetChkType(char chartType = DAY_DATA_CHART);
	void EnableQueryOpt(BOOL bEnable);//KHD
	void SetQueryOpt(int nCheck);//KHD
	//Symbol���� : IBK (����ƿ�ǲ)
	HINSTANCE	m_hiSymbol;				//����� �ɹ���
	CWnd*		m_pwndSymbol;
	void		SetInitSymbol();
	void		ParsingSymbol(char* datb);
	void		SendSymbol(CString strCode);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBARDLG_H__88AA28E0_917F_485D_BE10_0171E8C2E60B__INCLUDED_)
