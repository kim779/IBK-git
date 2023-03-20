#pragma once

#include "TabDlg.h"

// CTabSystemTradingEx ��ȭ �����Դϴ�.

#include "../Common_ST/DllSTLoader.h"
#include "../Include_ST/ISTControllerD.h"
#include "./control/XListCtrl.h"

#include "../../inc/ISTManager_GNF.h"				// �ý���Ʈ���̵� ���õ� ������ �������̽�
#include "../../inc/ISTRunManager_GNF.h"			// ����������� �������̽� ����.

#include "DlgSTExOption.h"						// �����м� �ɼǰ���.
//#include "./control/GfxOutBarCtrl.h"
#include "../../inc/ISTLoadManager.h"			// �����ε���� �������̽� ����.
#include "../../inc/IAUDataMonitorMng.h"	// UUID_IAUDataMonitorManager : �ݹ��Լ� �޴µ� ���
#include "../../inc/ISysTabExChartItem.h"
#include "../Common_ST/IGfxOutBarCtrl.h"
#include "../Common_ST/STTreeCtrl.h"//KHD 
#include "STDlgConfirm.h"				// �ֹ�â.
extern HINSTANCE g_hInstance;

#include "./ChartItem.h"
#include "afxwin.h"
class CSysTabExChartItem : public ISysTabExChartItem
{
public:
	CSysTabExChartItem();

	CChartItem* m_pChartItem;

public:
	STDMETHOD_(long, GetChartItem)() ;
	STDMETHOD_(void, SetChartItem)(long dwNewChartItem) ;
	STDMETHOD_(BOOL, AddPacketAndGraph) () ;
	STDMETHOD_(BOOL, SetPacketData)(LPCTSTR szPacketName, long szData, long nLength, BOOL bReset) ;
	STDMETHOD_(void, SetNOnePageDataCount)(long propVal) ;
	STDMETHOD_(void, ShowInnerToolBar)(BOOL p_bShow) ;
	STDMETHOD_(BOOL, InvokeAddIn) (LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData) ;
	STDMETHOD_(BOOL, AddChartBlock) (LPCTSTR lpChartName) ;
	STDMETHOD_(CString, GetPacketName)();
};

class CTabSystemTradingEx : public CTabDlg
{
	//DECLARE_DYNAMIC(CTabSystemTradingEx)

public:
	CTabSystemTradingEx(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTabSystemTradingEx();

	enum	// �ʱ�/����� ������������ ���� ���� ���ǵǾ�����.
	{
		Basic_STChange_Delete=0x10,		//�ʱ������� ����
		Basic_STChange_Add=0x11,		//�ʱ������� �߰�
		Ext_STChange_SDDelete=0x20,		//��������� ����ڴ�����������
		Ext_STChange_SDAdd=0x21,		//��������� ����ڴ��������߰�
		Ext_STChange_Delete=0x40,		//��������� �����������������
		Ext_STChange_Add=0x41,			//��������� ��������������߰�
	};

public:
	void InitControl();

	void RecalcLayout();
	CRect GetRectOfCtrl(INT nID,BOOL bInit=FALSE);

	void LoadInfo();

	BOOL OpenAUP();
	CWnd *LoadTradeAup(CString strFile, CWnd *pParent);
	void UnLoadSTList();

	// ���̾�α� ����ó���� ���ؼ� ...
	HBRUSH GetBkColorBrush() { return (HBRUSH)m_brBkColor.GetSafeHandle(); }
	COLORREF GetBkColor() { return m_crBk; }

	virtual void ChangeSkinColor(COLORREF clrBkgrnd);
	
// ��ȭ ���� �������Դϴ�.
	//{{AFX_DATA(CTabCodeSearch)
	enum { IDD = IDD_TABSYSTEMTRADINGEX };
	CILButton	m_BtnStwizardA;
	CILButton	m_BtnStoptimaA;
	CILButton	m_BtnStDelete;
	CILButton	m_BtnStanal;
	CILButton	m_BtnOption;
	//}}AFX_DATA
//	CILButton	m_BtnStwizardA;
//	CILButton	m_BtnStoptimaA;
//	CILButton	m_BtnStanal;

	long m_nHeightOfSearch;

	BOOL m_bShowOption;
	CDlgSTExOption*	m_pDlgOption;
	CImageList		m_ilSTIcon;
	CImageList		m_ImageOutbarIcon;
	CImageList		m_ImageListIcon;
	CImageList		m_ilW151H21;
	CStringArray	m_arSTPath;
	ISTLoadManager2* m_pSTLoadMnger;
	HINSTANCE		m_hSTConfigureWizardInst;

	CSysTabExChartItem	m_STExChartItem;	// CChartItem* ������ �Ѱ��ش�.
	BOOL			m_bEmptyWizard;			// �����ۼ� ���࿩��.
	STMNG_ITEMINFO	m_RunSTInfo;			// �������� ��������
//	CString			m_szRunSTPath;			// ���� ��Ʈ�� ����� ���������� ���.
	BOOL			m_bSTBtn_Click;			// ��ư �׸� ���̱����� �߰� KHD
	// �ƿ���� ���� �ø���.
//	CGfxOutBarCtrl	m_wndOutBar;
	// IGfxOutBarCtrl* m_pImpl;
	IGfxOutBarCtrl*	m_pWndOutBar;
	//CListBox		m_listSTList;
	void LoadOutlookBar();
	void LoadSTList();
	void AddSTFile(CListBox& rSTList, CString szPath);

	CString		m_szRootDir;
	CString		m_szDataDir;
	CString		m_szEncUserDir;
	int			m_nUserType;	// "1":HTS  "0":FRONT(����)

	CSTBasicTree m_STBasicTree;
	CSTExtTree	m_STTree;
	void LoadSTList_SSStyle();		// �Ｚ��Ÿ��:�ʱ�/��� ȥ����.
	//
	BOOL IsSTSelected();
	void RunWizard(BOOL bSelectedDefault);			// �����ۼ� �ε�
	BOOL CanSTModule();								// �ֽ�,��/�ɿ����� ��������.
	BOOL IsSTRun(LPCSTR p_szSTPath, LPCSTR p_szSTName, int p_nSTType);		// ������ ���������� �˷��ش�.

	BOOL CheckOcx();	// TeeChart, MSChart���
	BOOL CheckRegister(LPCSTR szProgID, LPCSTR szFileName);	// �����м� �� ����ȭ�� ���� MSChart�� TeeChart�� ��Ͽ��θ� Ȯ���Ѵ�.
	void CheckSTBasic();					//�ʱ������� ��밡���� �̸����� Ȯ��

	//[[�ֹ�����
	void TestJumun();

	//]]�ֹ�����

	//..{ �����ͺ��濡 ���� �뺸�� �ޱ����ؼ�.
	// �ý������� ����� KEY���� "STLIST_MODIFIED"
	IAUDataMonitorManager*	m_pIAUDataMonitorManager; // 28 : �����ͺ���ó���� ���� �������̽�
	long					m_dwIAUDataMonitorManager[2];
	static void CALLBACK DataMonitorCallbackProc(LPCSTR szKey, long pThisPoint, long dwKey, long dwSubKey, long dwNotifyData);
	//..}

private:
	void MakeSTDefaultDirectory();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CTabSystemTradingEx)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnStwizardA();
	afx_msg void OnBtnStoptimaA();
	afx_msg void OnBtnStanal();
	afx_msg void OnBtnStDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	LONG OnRmsgObItemClk(WPARAM wp, LPARAM lp);
	LONG OnRmsgObItemDBClk(WPARAM wp, LPARAM lp);
	LONG OnRmsgMultiST(WPARAM wp, LPARAM lp);
	LONG OnRmsgIndexAddinNotifyRawData(WPARAM wp, LPARAM lp);
	LONG OnRmsgIndexAddinNotifyBuySell(WPARAM wp, LPARAM lp);
	LONG OnRmsgIndexAddinNotifySignal(WPARAM wp, LPARAM lp);
	LONG OnRmsgDestroyConfirmWnd( WPARAM wp, LPARAM lp);
	LONG OnRmsgIsSTRunState( WPARAM wp, LPARAM lp);


public:
	void ShowHideStOptimaA(BOOL bChange, BOOL bShopOption, int nOffset=0);
	void HideStOptimaA();
	void STClear();

	BOOL Run_STDLL(LPCSTR szDll);
	void DirectRunST(LPCSTR szSTFile);	// FullPath�� ST����.
	void RunMultiST();		// ���������� �����Ѵ�. sd(sdd)���丮�� �����ϴ� ST����.
	CString m_szImsiRunSTFile;

	// �����༼, ��ǥ ����
	LRESULT RunIndexSD(int p_nSTRunType, STMNG_ITEMINFO* p_pArg);

	// �����༼, ��ǥ���� ����â�� ����.
	int DoDlgSysVarOption(CStringList* p_dwInput, LONG dwOutput);


public:
	class CSTInterface : public ISTInterface
	{
	public:
		STDMETHOD_(void*, GetInterface)(int nID);
		STDMETHOD_(LONG, GetOutBarColor) (COLORREF& clrOLBkgrnd, COLORREF& clrOLOutLine);
	}m_xSTInterface;
	friend class CSTInterface;

	class CSTAnalysisInput : public ISTAnalysisInput
	{
	public:
		STDMETHOD_(void, SetUserData)(BOOL bUse, ISTUserData* pInterfaceUserData) ;	// TRUE �ϰ�쿡�� ISTUserData* ���� ��������� �Ѵ�.
		STDMETHOD_(BOOL, IsUseUserData)() ;					// ����ڰ��� ������ ��뿩��
		STDMETHOD_(ISTUserData*, GetUserDataInterface)() ;	// �ð�����ȭ�� ���� ISTAnalysisInput*���� �����ϴ� GetChartData ��ſ�
															// ISTUserData*���� �����ϴ� GetChartData����Ѵ�.
															// �ֳĸ� ������ �����͸� ����ϱ⶧����

		// ID�� ������
		STDMETHOD_(COleVariant, GetData)(int nID)  ;

		// 0: �����ڵ� , GetCode(0)
		STDMETHOD_(CString, GetCode)() ;

		// �����ڵ��, GetCode(1)
		STDMETHOD_(CString, GetCodeName)() ;

		// �����ڵ� ���� : 0:�ֽ�, 1:����, GetCode(2)
		STDMETHOD_(int, GetCodeType)() ;

		// ������, GetCode(3)
		STDMETHOD_(BOOL, GetSTInfo)(STMNG_ITEMINFO* pInfo) ;

		// ����, GetCode(4)
		STDMETHOD_(int, GetCount)() ;

		// ������, GetCode(5)
		STDMETHOD_(CString, GetEndDay)() ;

		// ���Ҽ�, GetCode(6)
		STDMETHOD_(int, GetDivideCnt)() ;

		// ���� ����, GetCode(7)
		// ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5)
		STDMETHOD_(int, GetDivideState)() ;

		// ���� ������ ���� ���� ���� 1, 5, 10, 15 ��ư���� ǥ�õ� �κ�.
		// ���ϰ� 1(0),5(1),10(2),15(3)
		STDMETHOD_(int, GetUnitPos)();

		// ��ø��, GetCode(8)
		STDMETHOD_(double, GetDeivideRate)() ;

		// �����Ͱ��� ����.
		// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
		STDMETHOD_(int, GetChartData)(LONG& dwDataTime, LONG& dwDataOpen, LONG& dwDataHigh, LONG& dwDataLow, LONG& dwCLose,LONG& dwVolume, LONG& dwDataExt) ;

		// ���ҿ���
		STDMETHOD_(BOOL, IsDivide)() ;

		// ��Ʈ���� ���´�.
		// nID 0(ChartOcx), 1(CListCtrl)
		STDMETHOD_(LONG, GetControl)(int nID);

		// 20081016 �ڽ·� - 8���� ��ȯó��
		STDMETHOD_(CString, GetPacketType)();

		//>> 20081203 �ڽ·� - 30�� TimeFormat ó��
		STDMETHOD_(BOOL, Is30SecUnit)() ;
		//<< 20081203 �ڽ·� - 30�� TimeFormat ó��
	public:
		STMNG_ITEMINFO	m_Info;
		STMNG_ITEMINFO	m_InfoForAnalysis;

		BOOL	m_bUseUserData;				// TRUE �ϰ�쿡�� ISTUserData* ���� ��������� �Ѵ�.

		ISTUserData* m_pInterfaceUserData;	// �ð�����ȭ�� ���� ISTAnalysisInput*���� �����ϴ� GetChartData ��ſ�
		// ISTUserData*���� �����ϴ� GetChartData����Ѵ�.
		// �ֳĸ� ������ �����͸� ����ϱ⶧����


		CSTAnalysisInput();

	}m_xSTAnalysisInput;
	friend class CSTAnalysisInput;
//	STMNG_ITEMINFO	m_LastRunInfo;		// ����,��ǥ,�����༼ �� ���� ����Ǿ��� ���� ����,070215

	class CSTRunManager : public ISTRunManager
	{
	public:
		STDMETHOD_(int, RunST)(BOOL bImsiRun, CString &szRunFile, LONG pInData);
		STDMETHOD_(LONG, GetSelectedSTInfo) () ;
		STDMETHOD_(LONG, GetViewHandle) () ;
		STDMETHOD_(LONG, GetOutBarColor) (COLORREF& clrOLBkgrnd, COLORREF& clrOLOutLine);
	} m_xSTRunManager;
	friend class CSTRunManager;

//	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnBnClickedStchkdiv();

	afx_msg void OnStshowoption();

	// �ֹ���ȣ �߻��� �Ҹ�
	void PlayOrderSound(int nOrderType, BOOL bBuySound, LPCSTR szBuyFile, BOOL bSellSound, LPCSTR szSellFile);
	BOOL IsAllowJumun(CSTOrderRData_EG *pInData, BOOL bShowMsg=FALSE);
	BOOL IsAllowJumunBefore(LPCSTR szSTFile, CString& szMsg);	//������������ �ֹ����ɿ��� üũ
	void CallJunmunDlg(CSTOrderRData_EG *pInData);
	CMap<CConfirmWnd*, CConfirmWnd*, long, long> m_mapConfirmWnd;
	
	class CSTDlgConfirmSendJumun : public ISTDlgConfirmSendJumun
	{
		STDMETHOD_(CWnd*, GetParent)();
		STDMETHOD_(HWND, GetHwndScreenView)();
//		STDMETHOD_(LRESULT, SendJumun)(WPARAM wp, LPARAM lp);
	}m_xSTDlgConfirmSendJumun;
	friend class CSTDlgConfirmSendJumun;

};
