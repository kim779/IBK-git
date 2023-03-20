#if !defined(AFX_DLGOPTIMIZER_H__7322843D_469D_45B4_8BEB_6DC808FE3CA7__INCLUDED_)
#define AFX_DLGOPTIMIZER_H__7322843D_469D_45B4_8BEB_6DC808FE3CA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptimizer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptimizer dialog
#include "resource.h"
#define G_MINUTE	1
#define G_DATE		2
#define G_TICK		3

//Dlg
#include "DlgResult.h"
//#include "DlgSonikChart.h"
//#include "DlgSooikChart.h"
#include "TabEarnChart.h"
#include "TabSonikChart.h"

#include "DlgTrade.h"
//Control
#include "GridCtrl_src/GridCtrl.h"
//#include "Control/XTabCtrl.h"
#include "Control/xSkinButton.h"
//#include "../../COMMON_BASE/TrComm/TrComm.h"
#include "../../inc/ExOCXDLL.h"
//KHD : ��Ʈ�� ���� ����ü

//KHD : �ƿ���� �ø��� : 
#include"../Common_ST/STGfxOutBarCtrl.h"
#include "../../inc/ISTLoadManager.h"
#include "afxwin.h"
#include "../../inc/ISTManager.h"				// �ý���Ʈ���̵� ���õ� ������ �������̽�
#include "../Common_ST/DataItem.h"
#include "../Common_ST/SDataItem.h"
//////////////////////////////////////////
//����//
#define TIME_OPTIMIZER 1003 //�ð�
#define INDEX_OPTIMIZER 1001 //��ǥ
#define VAL_OPTIMIZER 1002//����

#include "DlgTimeSetup.h"	// 06.09.06 [alzioyes]�׽�Ʈ �����.

#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExDrawButton.h"
#include "../../inc/ExStatic.h"
#include "../../inc/ExComboBox.h"

#include "../Common_ST/STTreeCtrl.h"		//@@@06.12.07
typedef struct ST_VALUE {
	CStatic *stStatic;
	CSpinButtonCtrl *stSpinBtn;
	CExEdit	*stEdit;
	CRect   rt;//Edit ��Ʈ���� ���� ��ġ..
} ST_VALUESET;


typedef struct ST_RUNVALUE {
	int start;
	int end;
	BOOL bNumber;
} ST_RUNVALUESET;

typedef struct ST_GRIDDATA{
	CString ByunSu;
	/////////////////////
	CString RateAccm; //���ͷ� 7:0
	CString AAerProf;//1
	CString CurPosRate;//2
	CString RateCurrent; //�Ѽ���1:3
	CString ProfitAll;//������3:4
	CString LossAll ;//�Ѽս� 4:5
	CString TotalPL;//6
	CString MaxProfit;//�ִ�����5:7
	CString MaxLoss;//�ִ�ս�6:8
	CString LossSavg;//�Ǻ���ռ���2:9
	CString AverProf;//10
	CString AverLoss;//11
	CString TotalEnt;//12
	CString NoCleanTotal;//13
	CString ProfitFactor;//??8??
	CString CntAll;//�ŷ��� 10:15
	CString ProfTrace;//16
	CString LossTrade;//17
	CString RateProfitLoss;//�·�9:18
	CString PTRate;//19
	CString LTRate;//20
	CString PriodSign;//21
	CString MaxConProfN;//22
	CString MaxConLossN;//23
	CString SOverRate;//24
	CString MaxConProf;//25
	CString MaxConLoss;//26

	CString TotalStick;//27
	CString PTStick;//28
	CString LTStick;//29
	CString PriodParty;//30
	CString EntryStick;//31
	CString CleanStick;//32
	CString AtStick;//33
	CString PTAStick;//34
	CString PLTAStick;//35
	///////////////////////

} ST_GRIDVALUEDATA;

typedef struct ST_STRGRIDDATA{
	CString  Byunsu;
	CString  strGridData;
} ST_STRDATA;
//KHD : 2006.09.15//////////////
//��������ȭ�� �� �������� �����ϱ� ���� ���� ����ü
typedef struct ST_VALUEOPTIMAZE{
	CString nTitle ; //Ÿ��Ʋ��
	double nStart; // ���۰�
	double nEnd;   // ��������
	double nAdv;   // ������
	int nDataType; // ������ Ÿ�� 
	//���� �߰� :�ִ� , �ּ�
	double nMax;
	double nMin;
} ST_VALOTM;
//////////////////////////////

class CDlgResult;
class CDlgTrade;
class CDlgSonikChart;
class CTabSonikChart;
class CDlgTimeSetup;
class CDlgOptimizer : public CEscDialog
{
// Construction
public:
	CDlgOptimizer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOptimizer)
	enum { IDD = IDD_DLG_OPTIMIZER };
	CExDrawButton	m_CtlAddAll;
	CExDrawButton	m_CtlAddOne;
	CExDrawButton	m_CtlDelAll;
	CExDrawButton	m_CtlDelOne;
	CExDrawButton	m_BtnStop;
	CExDrawButton	m_BtnOpt;
	CXTabCtrl	m_CtlTab;
	/*CxSkinButton	m_BtnDef;
	CProgressCtrl	m_CtlProgress;
	CStatic	m_stMesu;
	CStatic	m_stMedo;
	CEdit	m_CtlEditMesu;
	CEdit	m_CtlEditMedo;
	CSpinButtonCtrl	m_SpinMesu;
	CSpinButtonCtrl	m_SpinMedo;

	CExComboBox		m_CtlComMesu;
	CExComboBox		m_CtlComMedo;
	CSpinButtonCtrl	m_CtlBong;
	CSpinButtonCtrl	m_CtlGijun;
	CEdit	m_CtlEditGijun;
	CEdit	m_CtlEditBong;
	CComboBox	m_CtlComIndex;

	CExStatic		m_CtlStatic6;
	CExStatic		m_CtlStatic5;
	CExStatic		m_CtlStatic4;
	CExStatic		m_CtlStatic3;
	CExStatic		m_CtlStatic2;
	CExStatic		m_CtlStatic1;
	CSpinButtonCtrl	m_CtlSpin6;
	CSpinButtonCtrl	m_CtlSpin5;
	CSpinButtonCtrl	m_CtlSpin4;
	CSpinButtonCtrl	m_CtlSpin3;
	CSpinButtonCtrl	m_CtlSpin2;
	CSpinButtonCtrl	m_CtlSpin1;
	CEdit	m_CtlEdit6;
	CEdit	m_CtlEdit5;
	CEdit	m_CtlEdit3;
	CEdit	m_CtlEdit4;
	CEdit	m_CtlEdit2;
	CEdit	m_CtlEdit1;
	int		m_EditGijun;*/
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptimizer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOptimizer)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//afx_msg void OnButton2();
	//afx_msg void OnSelchangeComIndex();
	//afx_msg void OnBtnDefault();
	afx_msg void OnBtnOpt();
	//afx_msg void OnRadioDay();
	//afx_msg void OnRadioMinute();
	afx_msg void OnTimer(UINT nIDEvent);
	//afx_msg void OnSelchangeComMedo();
	//afx_msg void OnSelchangeComMesu();
	afx_msg void OnBtnStop();
	//}}AFX_MSG
	LRESULT OnSTDataManagerMessagge( WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnSetKeyCodeChange(WPARAM , LPARAM );
	LRESULT OnRmsgObItemDBClk(WPARAM wp, LPARAM lp);//�ƿ���� �޼����ޱ�
	LRESULT OnRmsgGetDllSize(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	//AP ���� //////////////////////////////
	CIStrategyItem *m_pStrategyItem;
	CStrategyLoader m_strategyLoader;
	CISDataItem *m_pDataItem;
	
	////////////////////////////////////////
private: short m_STMessageType;//0:IndexOptimizer 1: Dlg-Result 2: Dlg-Trade
public: //Dll Load 
	// TESTING.....start 06.09.06[alzioyes]
	CDlgTimeSetup*	m_pDlgTimeSetup;
	void CreateTImeSetupDlg();
	void DestoryTImeSetupDlg();
	// TESTING.....end

	HWND			m_hSocket;					// �����ڵ�
	HWND			m_hMainFrame;				// CMainFrame�� Handle
//	CDBMgr*			m_pDBMgr;
//	CDrMLib_CDBMgr  m_IDrMLib_CDBMgr;
//	CBaseInfo	m_BaseInfo;
//	CDrMLib_CBaseInfo m_BaseInfo;
	CString			m_strTrCode;				// ȭ���ȣ
	CString			m_strUserID;
	CString			m_strRootPath;
	CString			m_strUserDir;
	CString			m_strDataDir;
	CString			m_strImageDir;

	void	FormInit();
	void	FormLoad();
	void	SetSTMessageType(short Smt);
//////////////////////////////////////////////////////////////////////////

	CBrush	m_brBkColor;
	COLORREF m_crBk;

	CGridCtrl m_GridStatus;
	CGridCtrl m_GridTap;
	void InitGrid();
	void InitCodeInfo();
	void ReSize();
	void AutoSizeColumns();

//TabDlg
	CDlgResult		*m_DlgResult;
	CTabSoonikChart  *m_DlgSooik;
	CTabSonikChart   *m_DlgSonik;
	CDlgTrade		*m_DlgTrade;

	CArray<CDialog*,CDialog*> m_arrayTab;
	CArray<ST_VALUESET*,ST_VALUESET*> m_arrayValue;
	void InitTab();
	void InitControls();

//ControlSet
	void RemoveAllControlSet();
	CIStrategyItem* GetStrategyItem(){return m_pStrategyItem;}

	void StartOpt();  // ������ ���¿��� ����ȭ����
public:
	
		long m_lSendNum;
		BOOL m_bReceive;
		int m_nTest;
		int m_nTimer;
		int m_nTimer2;
		CStringArray m_stArrayPath;
		CStringArray m_stArraySDFileName;
		CString m_Code; 
		CString m_Name;
		CString m_StragyName;

		void InitStrategy();
		BOOL CalculateSendData();
		CString GetJMNameAndGubunFromCode(CString csCode);

		int m_nTimeState;//��,��,��,��
		//�� �� ���ؼ� ���� 
		CString m_strGijun;
		short m_nGijun;//Min:1 Day: 2 Tick:3
		BOOL IsCurrentChartData(CString strValue);
		BOOL IsNumber(CString str,int Index);
		void SpreadGridData();
		CArray<ST_RUNVALUESET,ST_RUNVALUESET> m_ArRunvalue;
		CArray<ST_GRIDVALUEDATA,ST_GRIDVALUEDATA> m_ArGridData;
		CArray<ST_STRGRIDDATA, ST_STRGRIDDATA> m_ArstrGridData;
		CStringArray m_strNum;// x,y,z ������ �Ľ̵Ǳ�
//		CStringArray m_strArSendData;
		
		//Medo Mesu
		void MedoMesuComboDataParsing();
		CString SearchFileToData(CString strIndex, CString strPath);
		void SetControlState(BOOL State);
		//Skin
		void InitSkin();
public:
	//KHD : �ƿ���� 2006.08.30
	CSTGfxOutBarCtrl	m_wndOutBar;
	ISTLoadManager2* m_pSTLoadMnger;
//	CImageList		m_ilSTIcon;     
//	CImageList		m_ilW190H21;
	CImageList		m_ImageListIcon;
	CImageList		m_ImageOutbarIcon;
	CStringArray	m_arSTPath;
	
	void LoadOutlookBar();
	void LoadSTList();
	void UnLoadSTList();
//OutlookBar END
	afx_msg void OnBnClickedBtnAddall();
	afx_msg void OnBnClickedBtnAddone();
	afx_msg void OnBnClickedBtnDelall();
	afx_msg void OnBnClickedBtnDelone();
	CListBox m_listST;//���� ������ �����ϰ� �ִ� ����Ʈ
	//KHD : ���� 
	int m_nOptimizerDiv;
	long m_pChartInterface;
	CString m_stCode;
	int m_nCount;
	CString m_stEndDate;
	CString m_stCodeName;
	int m_nCodeType;
	BOOL m_bGridClick;
	void HideControl();//����ȭ �ɼǿ� ���� �ٸ��� ����

	class CSTUserData : public ISTUserData
	{
	public:
		// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
		STDMETHOD_(int, GetChartData)(LONG& dwDataTime, LONG& dwDataOpen, LONG& dwDataHigh, LONG& dwDataLow, LONG& dwCLose,LONG& dwVolume, LONG& dwDataExt, int& nBong) ;
	}m_xSTUserData;
	friend class CSTUserData;	
	int m_nBong;
	//��������ȭ
	CArray<ST_VALOTM,ST_VALOTM>   m_ValueOptimizerArray;
	CString m_stPath;//��������ȭ���� ���Ǹ� ������ �ٸ� ����ȭ�� ��ƾ�� �ٸ��⶧���� ����!!
	int m_STValNum;// ��������ȭ�� �������� 
	afx_msg void OnLbnDblclkListSt();
public:
	//@@@06.12.07[[
	HINSTANCE			m_hSTConfigureWizardInst;
	IGfxOutBarCtrl2*		m_pWndOutBar;
	CSTBasicTree		m_STBasicTree;
	CSTExtTree			m_STTree;

	void fnBnClickedBtnAddone(STMNG_ITEMINFO* pArg);
	void LoadSTList_SSStyle();
	//LONG OnRmsgSTTreeItemClk(WPARAM wParam, LPARAM lParam);
	LONG OnRmsgSTTreeItemDBClk(WPARAM wParam, LPARAM lParam);
	//@@@06.12.07]]

	CFont	m_Font;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTIMIZER_H__7322843D_469D_45B4_8BEB_6DC808FE3CA7__INCLUDED_)
