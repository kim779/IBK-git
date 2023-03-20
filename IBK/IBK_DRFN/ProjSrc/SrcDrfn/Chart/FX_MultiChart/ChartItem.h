// ChartItem.h: interface for the CChartItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTITEM_H__11999AEF_8200_4DA5_B205_33C1402F54D5__INCLUDED_)
#define AFX_CHARTITEM_H__11999AEF_8200_4DA5_B205_33C1402F54D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>


#include "ChartItemData.h"
#include "../../DrCommon/DRChartOcx.h"
#include "../../inc/ExOCXDLL.h"

#define HEIGHTOFTITLEBUTTON		(15)
#define WIDTHOFTITLEBUTTON		(15)
#define POSXOFMAXMIZE			(18)
#define POSYOFMAXMIZE			(1)
#define POSXOFINIT				(2)
#define POSYOFINIT				(1)
#define SIZEOFSIGNAL			(18)

typedef struct __STHIGHLIGHTCHARTINFO {
	CString			m_strJongmokCode;
	CChartItem*		m_pChartItem;
}stHighlightChartInfo;
typedef CList<stHighlightChartInfo,stHighlightChartInfo> CHighlightChartInfoList;

// �������� ���� : �������� - ojtaso
typedef struct _ST_CHARTITEM_REALDATA
{
	CString	m_strItemName;
	UINT m_nRefCount;

	char m_chSign;
	CString m_strPrice;
	CString m_strChange;
	CString m_strChrate;
	CString m_strVolume;

	CString m_strOpen;
	CString m_strHigh;
	CString m_strLow;

	CString m_strPipLowest;		// �Ҽ��� �ڸ���

	CString m_strTime;
}ST_CHARTITEM_REALDATA;

//=============================================================================================================
//08.17 Real ó��....
//=============================================================================================================
#include "../chart_common/ILButton.h"
//====================================================================
#define CODE_LEN	6
struct stChartSign
{
	int  nSignal;
	char RepresentCode[CODE_LEN];
};


typedef BOOL ( WINAPI *FnGetRealRecordInfo)( long p_lRealDataAddress, WORD &p_wRealRecordID, CString &p_strRealKeyCode);
typedef BOOL ( WINAPI *FnGetData)( long p_lRealDataAddress, WORD p_wRealItemID, double &p_dData);

interface IRealReceiver;
class CChartMng;
//=============================================================================================================
class CChartItem : public CDRChartOcx//, public ISharedDataMgr, public IRealTableMgr, public CSharedBase
{
//################################################################################
//2006. 01. 12 ����� 
//�ű��߰� by sy, nam
//################################################################################
public:
	CString m_strCurPrcForReplay; //���� ���簡 
	CString m_strVolumeForReplay; //���� �ŷ���
	CString m_strPrePrcForReplay; //���� ���� ����.
	CString m_strChrateForReplay; //���� ���ϴ�� ��·�
	CString m_strSignForReplay;	  //���� ���� ��ȣ.
	
	
	void	InitString_ForReplay();
	void	SetReplay_PrePrice(CString  strBojoMsg); //���⸦ ���� �������� ����
	CString m_strOrderType;
	CString m_strNextKey;			//	�ؽ�Ʈ Ű�� �����ϴ� ����

public:	
	void	SetNew_BNormalizedMinuteTimeRule(BOOL bFakeCandle);

	CDC* m_pDCForReplay; // ����� �ؽ�Ʈ�� �Ѹ� DC;

	const char* GetNextKey(BOOL bMarket=FALSE);  //���� �޴� �������� ���� ���� ��ȸ�� �ϱ� ���� ����Ű�� �����Ѵ�.

	CArray <CChartItemData*,CChartItemData*> m_arrayMarketTRData; //��û�� TR Array;
	CPtrArray m_ptArrRecvMarketTRData; //���� TR�� �����ϴ� Array;
	CStringArray m_strArrRecvMarketTRData;
	
	BOOL BuildMaketChart(CChartItemData* pItemData);
	
	long ReceiveMarketChartData(void* szData, long nLength); //���� ��ǥ TR �� �޾Ƽ� ó���Ѵ�.

	CString GetMarketQueryData(CString strJipyo, int nCntRqst, BOOL bIsNew); //���� ��ǥ TR ������ ���� ���ڿ� Get
	CChartItemData* GetChartItemMarketDataNew(CString strTRCode,LPCTSTR lpCodeForQuery); //���� ��ǥ �����͸� �Ź� ���� �����.

	void SetNew_OnePageDataCount(int nPCnt);
	void RemoveAllMarketData(); //������ǥ ���� Array �����͸� ���� �����Ѵ�.
	BOOL RemoveChartItem_OneData(LPCTSTR lpCode, CString strQueryType); //���� TR�� �ִ� ��� ���� �����Ѵ�.
//==============================================================================================================
// 2005.11. 16 pm 04:56
// ����
private:
	BOOL m_bReplaychartMode;
	CString m_strCurReplayDate;
public:
	void	SetReplay_CurDate(CString strBojoMsg);
	CString GetReplay_CurDate() {return m_strCurReplayDate;}
	void SetReplayChart_RealData(CString strCnts, CString strPrice = _T(""), CString strVolume = _T("")); // ���� ���� ǥ��.
	BOOL GetBReplayChartMode() { return m_bReplaychartMode; }
	void SetBReplayChartMode(BOOL bReplayMode);

	void	DrawReplayChartTitle(CRect rectIn,CDC* pDC,CFont *pFt, CFont *pFtNormal, COLORREF crBackIn);
	CString m_strReplayTime;
	
//==============================================================================================================
	
public:
	BOOL m_bReceivedDataChart; //��Ʈ�� �����͸� �ѹ��� �� �޴� ��� ���� FALSE;


	//===============================================================================================
	// �ֽ� ��ž������ �����ϱ� ����...
	void* m_pStdDialog;

	//===============================================================================================	
	//09.09
	void OpenSignalMapScr();

	//===============================================================================================	
	//09.07
private:
	CString m_strOldCode;
public:
	void	SetOldChartCode(CString strCode);
	CString GetOldChartCode() { return m_strOldCode; }
	//===============================================================================================


public:
	CChartItem();
	void SetKorName(CString strKorName)		{ m_strKorName = strKorName;	}
	CString GetKorName()					{ return m_strKorName;			}
	void SetAutoSaveToOutside(const bool bAutoSave);		// �ڵ� ���� ����
	BOOL SetPacketData(LPCTSTR szPacketName, long szData, long nLength, BOOL bReset); // added by gm0604 2005.10.10

	//===============================================================================================
	// ���� ��¥ Flag �� ���� ���� 
private:
	CString m_strStartDate, m_strEndDate;

public:
	//09.06	
	void SetEndDate(LPCTSTR endDate)     ;

	//08.25	
	void SetStartDate(LPCTSTR startDate)   { m_strStartDate = startDate; }	
	void SetStartDateFlag(BOOL bStartDate) { m_CfgInfo.bStartDate = bStartDate ? true : false;	}
	//===============================================================================================
	

	int m_nMarketType;
	BOOL	m_bNowInitialState;

	CString				m_strImagePath;
	void				SetImagePath(CString strPath) { m_strImagePath.Format("%s\\Image", strPath); }
	void				InitShareManager();

	CImageList*			m_pIL_Symbol;			// Symbol Image List
	enum				e_SIGTYPE{ e_NKS = 1 , e_SAS = 2 , e_UAS = 3 };
	int					GetSignalImageIndex_FromSignalDegree(int nType, int nSignalDegree);
	void				SetSignalData(CString sShareName, BYTE* pData, int nLen);


	//==================================================================
	// 2005. 08. 23 add by sy, nam
	// ��Ʈ ��� ĵ�� ��, �Ｑ, ��� 
	CString m_strChartLineType;
	void	SetChartLineType(CString strLineType) {	m_strChartLineType = strLineType ;}

	//==================================================================
	// 2005. 08. 12 add by sy, nam
	// ��Ʈ ȯ�� ���� ����
	CHART_CONFIG_INFO	m_CfgInfo;
	void				Init_ChartCfgInfo();
	void				SetChartConfigInfo(CHART_CONFIG_INFO &ChartCfgInfo);
	CHART_CONFIG_INFO*	GetChartConfigInfo(){ return &m_CfgInfo;}
	//==================================================================


	//==================================================================
	// 2005. 08. 11 add by sy, nam
	// ���� ������� ���ο� ���� ����� �ش� ���� �ð��� ���ؼ�
	// ��Ʈ�� �ѱ��.
	CString GetReplayFlag()	;
	CString GetReplayDayBefore();
	CString GetReplayStartTime();
	//==================================================================
	
	void SetChartRequestType(char chType);

	//==================================================================
	// 2005. 06. 22 add by sy, nam
	// ��Ʈ�� ��, %, $�� ���а��� 
	int  m_nChartNumericValueType;
	void SetChart_NumericValueType(int nValue);
	int	 GetChart_NumericValueType(){return m_nChartNumericValueType;}
	void SetChartNumericValue(int nNumericValue);
	//==================================================================

	//==================================================================
	// 2005. 05. 27 add by sy, nam
	void	SetUseLogValue(BOOL bUse);
	//==================================================================

	BOOL Create(CWnd *pWnd,LPRECT lpRect,int nID,long lRow,long lCol, LPCTSTR lpImagePath, BOOL bUseInnerControl);
	void SetInit(long p_lDllWnd, long lKey, LPCTSTR strHtsPath, BOOL bNoToolBar =FALSE);
	long ReceiveInitalChartData(void* szData, long nLength);
	// �������� �ҷ����� : �������� - ojtaso (20070514)
	long ReceiveLoadingChartData(void* szData, long nLength);
	void ResetOCX(BOOL bNotCodeInputInit = TRUE); //bNotCodeInputInit �����ڵ� �κ��� �ʱ�ȭ ���� �ʴ°� Default
	void DrawTitle(CRect rectIn,CDC* pDC,CFont *pFt,CFont *pFtNormal, COLORREF crBackIn);
	void SetButton(const LPRECT lpRect);

	//==========================================================================================
	//void SetCodeForQuery(LPCTSTR lpCode, UINT nQueryType, BOOL bIsPrimaryData);
	//2005. 05. 06 ������ TRCode�� �� ����.
	void SetCodeForQuery(LPCTSTR lpCode, CString strQueryType, BOOL bIsPrimaryData);
	//==========================================================================================
	
	BOOL IsEmpty() { return	(!m_bInitalChart);}
	BOOL AddChartBlock(LPCTSTR lpChartName);
	BOOL DestroyWindow();
	void SetSmallChartMode(INT nSmallChartMode) { m_nSmallChartMode = nSmallChartMode;}
	
	void SetType(char chType);
	char GetTypeRequested();
	char GetType();

	void	SetTickMinDayNumber(UINT nNumber);
	CString GetTickMinDayNumberRequested() {return m_strTickMinDayForRequest;}
	

	//=======================================================================
	//2005. 05. 24 add by sy, nam
	//������ ƽ���� ���� ������ �ϹǷ� ������ �߰�����.
	//CString GetTickMinDayNumber() {return m_strTickMinDay;} //�� �Ⱓ�� ���� �����ؾ� �ϹǷ� 
	//������ �ϳ��� ���� ������ �Լ��� �ּ�ó���ϰ� ������ ���� �Ѵ�.
	CString m_strTick,m_strSec,m_strMin,m_strDay, m_strHour;

	CString GetTickMinDayNumber(int nChartType = -1);

	CString GetTickNumber()	;
	CString GetSecNumber()	;
	CString GetMinNumber()	;	
	CString GetDayNumber()	;	
	CString GetHourNumber()	;
	
	void SetTickNumber	(CString strValue);
	void SetSecNumber	(CString strValue);
	void SetMinNumber	(CString strValue);
	void SetDayNumber	(CString strValue);
	void SetHourNumber	(CString strValue);


	void LoadTickMinDayInfo(CString stCfgFile);
	//=======================================================================

	void	SetEndDateRequested(LPCTSTR lpDate);
	CString GetEndDateRequested();
	CString GetEndDate();
	CString GetStartDate() { return m_strStartDate;	}

	void SetCntForRequest(UINT nCntForRequest);
	long GetCntForRequest();
	long GetAllCnt() { return m_lAllCnt;}
	UINT GetID() { return m_nID;}
	// �������� �߰� : �������� - ojtaso (20070118)
	stSend2Server* GetQueryData(BOOL bChangeType = FALSE);
	CString GetDataCode(int nIndex = 0);
	CString GetDataTRCode(int nIndex = 0);
	BOOL HasGraph(LPCTSTR p_szGraphName);
	void SetRealPriceData(void *pData, long lDataKind);
	BOOL IsOnDrdsAdvise() { return m_bOnAdvise;}
	CString GetTrCodeForQuery();
	
	//2005. 06. 13 sy, nam   
	//		==>	bSetChartTick =���� ��ȸ ������ ��Ʈ���� ���Ͽ��� ���� �ʰ��� �Ҷ� False�� �Ѵ�.
	//2005. 09. 05 sy, nam   
	//		==>	bNotCodeInputInit = �����ڵ� �κ��� ���ڿ��� �ʱ�ȭ ���� �ʰ��� �Ҷ� FALSE�� �Ѵ�.
	BOOL LoadChartInfo(LPCTSTR lpTitle, BOOL bSetChartTick =  TRUE, BOOL bNotCodeInputInit = TRUE);
	
	BOOL SaveChartInfo(LPCTSTR lpTitle, BOOL bOnlyJongmok =FALSE);
	BOOL LoadChartSetting(LPCTSTR lpTitle);
	BOOL SaveChartSetting(LPCTSTR lpTitle);
	BOOL ResetChart();
	BOOL IsPrimaryChartItemData(CChartItemData *pChartItemData);
	//BOOL HasChartItemData(LPCTSTR lpCode, UINT nQueryType, BOOL bFindWithPrimaryData, BOOL bAutoRemoveUselessData = TRUE);
	//BOOL RemoveChartItemData(LPCTSTR lpCode, UINT nQueryType);
	//��ȭ���� TR Code�� ���ڿ��� �ؼ�....
	BOOL HasChartItemData(LPCTSTR lpCode, CString strQueryType, BOOL bFindWithPrimaryData, BOOL bAutoRemoveUselessData = TRUE);
	BOOL RemoveChartItemData(LPCTSTR lpCode, CString strQueryType);
	
	// �������� ���� : �������� - ojtaso (20070118)
	void SaveRQInfo(LPCTSTR lpszAppName, LPCTSTR lpszTitle);
	// �������� �ε� : �������� - ojtaso (20070118)
	void LoadRQInfo(LPCTSTR lpszAppName, LPCTSTR lpszTitle);
	
	CChartItemData *FindChartItemDataForQuery();
	long GetCntOfItemData() { return m_arrayChartItemData.GetSize();}
	BOOL	ClearData();
	BOOL	ClearDataExceptPrimaryData();
	BOOL GetNext() {return m_bNext;}
	void SetNext(BOOL bNext);
	BOOL SaveandOpenFromFile(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption);
	// �м��� ���� ���� - ojtaso (20080728)
	BOOL SaveandOpenFromAnalToolFile(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption);
	CChartItemData *GetPrimaryChartItemData();
	void SetUseRevisedValue(BOOL bUse);	
	BOOL GetUseRevisedValue();
	void SetBNormalizedMinuteTimeRule(BOOL bUse);

	void SetRowNCol(long lRowIndex, long lColIndex);
	void HideScale(BOOL bHide);
	void HideInnerToolBar(BOOL bHide);
	void InvalidateType();
	BOOL PtInRectJongmokTitle(POINT point) const;
	void ResetChartCfgWith(short p_nIndex);
	long GetTempOnePageDataCount();
	CString GetQueryDataForGuideLine(const CUIntArray* pAryAvgForChartGuideLine);
	void ShowLock(BOOL bShow);
	BOOL SendMessageToInvoke(short nCommandType, long lData, long lSize);

	//// System Trading
	BOOL ReceiveFromSTDataManager();
	void SetPtrST(long lPtr, LPCTSTR lpSTName);
	long GetPtrST() { return m_lPtrST;}
	void ClearPtrST();
	void SetSystemTradingIndexByTab(CStringArray *pstrArraySTName);
	void UpdateSystemTradingIndexByTab();

	CChartItemData*		GetItemDataAt( int index );

private:	
	//==================================================================
	// 2005. 05. 18 add by sy, nam
	int		m_nCntOverLap;  // ��ø�� ���� Cnt;
	BOOL	m_bOverLapping; // ��ø ��� Flag
	BOOL	m_bLogPrice;	// LOG ���
	//==================================================================
	
private:
	/// System Trading gm0604	2005.7.21
	long m_lPtrST;
	CString m_strName;	// System Trading Module Name
	CStringArray m_strArrayNameBySystemTradingTab;
	/// System Trading gm0604	2005.7.21
	BOOL DestoryItemData();
	BOOL BuildChart();
	BOOL SetAllProperties(BOOL bNoToolbar =FALSE);
	BOOL SetDrdsAdvise(long lOnAdvise);
	void ResetData();
	void InitialStartDate();
	CChartItemData *GetChartItemDataNew(CString strTRCode,LPCTSTR lpCodeForQuery, BOOL bCreateIfNoExist=TRUE,BOOL bIsPrimaryData = FALSE);
	long GetPosChartItemData(CString strTRCode,LPCTSTR lpCodeForQuery);
	
	CArray <CChartItemData*,CChartItemData*> m_arrayChartItemData;
	CArray <CChartItemData*,CChartItemData*> m_arrayChartItemDataBackup;
	BOOL	m_bUseInnerControl;
	BOOL	m_bBuild;	// ��ȸ �� ��Ʈ�� �����Ǿ������� �ǹ� (ResetOCX �ϸ� False)
	BOOL	m_bOnAdvise;
	BOOL	m_bInitalChart;
	BOOL	m_nSmallChartMode;	// 0:NONE 1:No Volume Graph&No Scroll Graph 2:No Scroll Graph
	BOOL	m_bLoadChartInfo;
	long	m_lNOnePageDataCount;

	char	m_chTypeForRequest;	// ��������ȸ�ϱ��� ƽ(0)/��(1)/��(2)/��(3)/��(4)
	char	m_chType;			// ƽ(0)/��(1)/��(2)/��(3)/��(4)
	CString m_strDateForRequest;
	long	m_lCntForRequest;
	long	m_lAllCnt;

	CString m_strTickMinDayForRequest;	// 2005. 05. 23 add by nam ƽ/��/���� ��û ���� �� �����ϰ� �ִ�.
	CString m_strTickMinDay;			// 2005. 05. 23 add by nam ƽ/��/���� ��û�Ǿ� ���� ���� �� �����ϰ� �ִ�.2Byte Tick Min Number
	UINT	m_nNum;			// �˻��� ������ ����
	BOOL	m_bRevisedPrice;

	CString m_strKorName;		// �����
	CString m_strPrice;			// ���簡
	CString m_strChange;		// ���
	CString m_strChrate;		// �����
	CString m_strVolume;		// �ŷ���
	//2006.10.09 by LYH �ð�, ��, ���� �߰�
	CString m_strOpen;			// �ð�
	CString m_strHigh;			// ��
	CString m_strLow;			// ����
	char	m_chSign;
	long m_lColIndex;
	long m_lRowIndex;
	CILButton m_btMaxmize;
// (2008/6/23 - Seung-Won, Bae) Remove Init Button.
	CILButton m_btInit;
	
//@��������	CButton	m_Check;
	UINT	m_nID;	// ID For Chart Component
	BOOL	m_bNext;
	CRect	m_rectTitleJongmok;
	CRect	m_rectTitleType;
	CRect	m_rectTitlePos;
	CRect	m_rectTitleJongmokPast;
	CRect	m_rectTitleTypePast;
	CRect	m_rectTitlePosPast;
	BOOL	m_bShowLock;

	long	m_plDllWnd;
	CStringList m_listAddIn;


// Operations
public:

// Overrides
// Implementation
public:
	
	void Add_OverLappedCnt();
	void Init_OverLapped();
	virtual ~CChartItem();	

//	//{{AFX_MSG(CTOcx)
//	afx_msg void OnOnLButtonDownTocxctrl1(short p_n, short p_sz);
//	DECLARE_EVENTSINK_MAP()
//	//}}AFX_MSG


//===================================================================================================================
// 2005. 08. 17 added by ��¿� 
// ��Ʈ ���� �ű� ó�� 
// >>>>>>>>> Start ==================================================================================================
	// * CBaseDesk�� ����
	class CXIBaseDesk : public CBaseDesk2
	{
	public:
		LPDISPATCH m_pDisp;
		CXIBaseDesk() : m_pDisp(NULL) {}
		~CXIBaseDesk() { if(m_pDisp) m_pDisp->Release(); }

		// * CBaseDesk Virtual ����
		virtual LPCSTR GetBasePath() ;
		virtual HWND  GetDeskHwnd() ;
		virtual LPDISPATCH GetUnknown();
		virtual LPUNKNOWN GetCtrlUnknown(LPCTSTR szName) ;
		virtual HWND GetCtrlHwnd(LPCTSTR szName);
		virtual LPVOID GetSharedInterface();
		virtual BOOL InvokeEvent(HWND _hWnd, LPCSTR _szEventName, LPCSTR _szParam);
		virtual BOOL InvokeEvent2(LPCSTR _szCtrlName, LPCSTR _szEventName, LPCSTR _szParam);
		virtual BOOL SendKey(HWND _hWnd, UINT nChar, BOOL _bIsShift, BOOL _bIsCtrl);
		virtual LPVOID GetLSharedData();
		virtual void CallCommand(LPCSTR _szCmd);

		// CBaseDesk2���� �߰��Ǵ� �����
		virtual OLE_COLOR GetUserColor(short nIndex, LPCTSTR szName, short nFlag);
		virtual long GetDeskInfo(LPCSTR szKey);
		virtual long GetUserInfo(LPCSTR szKey);
		virtual long GetFrameInfo(LPCSTR szKey);
		virtual long GetMainInfo(LPCSTR szKey);

//* ���� ���� (2002.8.22 001) ����͸� �� �߰�
		virtual BOOL FP_vAdviseDebug(LPCSTR sName, UINT pMonitoringCommand);
		virtual void FP_vUnadviseDebug(LPCSTR sName, UINT pMonitoringCommand);
		virtual LPCSTR FP_sGetMyVBName(HWND hWnd);
		virtual void FP_vSendDebug(LPCSTR csSection, LPCSTR csKey, LPCSTR csValue);
//* ���� �� (2002.8.22 001) 

		// CControlPos�� m_awndControls �ɹ������� �����͸� �����Ѵ�.
		// �� ������ �������� �� ������Ʈ�� ������ ��� �ִ�.
		virtual long FP_nGetCtrlPos();

		// ������Ʈ�� �������� ��Ų��.
		virtual void FP_vResizeCtrl();

		// {{ OCX���� DESK�� �޼��� ���� �� �ֵ��� ��û(����) 2003.05.19
		virtual void SetMapMessage(long lType, LPCSTR lpData);
		// }}

		// ����ũ���� ������ ���ϰ��� ���ϴ� ����� ������ �Լ�
		// nCommand
		// 100 : Socket ����� ��û�Ѵ�.
		virtual int FP_nGetDataInt(int nCommand);

		friend class CChartItem;

	} m_xIBaseDesk;
	friend class CXIBaseDesk;
	CXIBaseDesk *m_pBaseDesk2;

// (2005/8/10 - Seung-Won, Bae) Shared Data Manager Routine for SkcChart OCX
protected:
	// Flag, if on Shared Data Transfering
	BOOL			m_bSharedOn;
//protected:
//	virtual long	ChangeSharedData(CString sSharedName, BYTE* pData, int nLen, int nStock);
//protected:
//	virtual BOOL	AdviseSharedData( const char *p_szShareName, int p_nShareType, int p_nShareArea);
//	virtual BOOL	UnadviseSharedData( const char *p_szShareName);
//	virtual BOOL	GetSharedData( const char *p_szShareName, CString &p_strSharedData);
//	virtual BOOL	FireSharedData( const char *p_szShareName, const char *p_szSharedData, long p_lCodeFilter);

// (2005/8/13 - Seung-Won, Bae) Support Real Table Manager Interface
// 1. Init Real Table Tree Control
//protected:
//	virtual BOOL	InitRealTableTreeControl( CTreeCtrl *p_pTreeCtrl);
// 2. Retreive the Record and Item Info
//protected:
//	virtual BOOL	GetRealIDs( const char *p_szRecordName, const char *p_szItemName, WORD &p_wRecordID, WORD &p_wItemID);
//	virtual BOOL	GetRealNames( WORD p_wRecordID, WORD p_wItemID, const char *&p_szRecordName, const char *&p_szItemName);
// 3. Advise & Unadvise Real Update
//public:
//	virtual BOOL	AddRealUpdate( WORD p_wRealRecordID, const char *p_szRealKeyCode);
//	virtual BOOL	ResetRealUpdate( WORD p_wRealRecordID, const char *p_szRealKeyCode);
// 4. Retreive the Real Data
protected:
	// To make a Library DLL for Real Data Fatch Code
	HMODULE				m_hmCtlDrChart;
	FnGetRealRecordInfo	m_fnGetRealRecordInfo;
	FnGetData			m_fnGetData;

	CString				m_strRealItemCode;		// �ǽð� ���Ź��� �����ڵ� : �������� - ojtaso (20070108)
	CString				m_strSelectedItemCode;	// ���� ���õ� �����ڵ� : �������� - ojtaso (20070108)
public:	
	void InitChartItem_FromExtToolBar();
//	DWORD GetCodeType_By_DBMgr(CString strCode);
	BOOL			AddInEvent_FromChart(LPCSTR _szParam);
//	virtual BOOL	GetRealRecordInfo( long p_lRealDataAddress, WORD &p_wRealRecordID, CString &p_strRealKeyCode);
//	virtual BOOL	GetData( long p_lRealDataAddress, WORD p_wRealItemID, double &p_dData);
//<<<<<<<<<<<<<< End =======================================================================================================
	//2006.01.23 �߰� by LYH
	void PrintChart();
	void SetExcelData();

//2006.04.04 �߰� by LYH
	int m_nSaveAllCount;
	void SetSaveAllCount(int nAllCnt);

//2006.12.11 LYH �ε� �� �������ϸ� �о����
	// �м��� ���� ���� - ojtaso (20080728)
	CString GetOpenAndSaveFileName(BOOL bWithTypeValue = FALSE);
//2006.12.12 LYH DLL�ε� �ʿ��ѽ����� �ε�

	// 2011.01.31 by SYS >> ����Ʈ YYYY(��) �߰� �� �м��� ���� ��� �߰�
	// �м����� ��������
	CString GetOpenAndSaveAnalFileName(BOOL bWithTypeValue = FALSE);
	// 2011.01.31 by SYS <<

	BOOL m_bAddInLoadPattern;
	BOOL InvokeAddInPattern(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData);
	BOOL m_bAddInLoadPeriod;
	BOOL InvokeAddInPeriod(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData);
	BOOL m_bAddInLoadSTIndex;
	BOOL InvokeAddInSTIndex(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData);
	BOOL m_bAddInLoadNews;
	BOOL InvokeAddInNews(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData);

	// �����ְ� Ŭ���� ��ü�� ������Ű�� ���� �߰� by LYH 2007.01.11
	int m_nTotCountForRevised;

	//2007.01.17 add by LYH  ���ٹ�ư ����ȭ
	BOOL m_bDrawAutoTrendLine;
	BOOL m_bDrawAutoPattern;
	BOOL m_bDrawCandlePattern;
	BOOL m_bDrawNews;

	// �������� ���� : �������� - ojtaso
	CMapStringToPtr		m_mapChartItem;
	CMapStringToPtr		m_mapChartItemRealData;

	// �������� �ҷ����� : �������� - ojtaso (20070514)
	BOOL m_bLoadRQ;
	BOOL IsLoadingRQ() { return m_bLoadRQ; }

	CString GetSelectedRQ() { return m_strSelectedRQ; };

	POSITION m_posLoadingRQ;
	CString m_strKey;
	ST_SELCHART* m_lpSelChart;

	CString m_strSelectedRQ;

	BOOL m_bLoadDefaultRQ;

	// �������� �ҷ����� : �������� - ojtaso (20070514)
	stSend2Server* GetLoadedQueryData();

	//2007.01.25 add by LYH  �ڵ�� ���屸�� �о����
//@��������
//	int GetMarketType(CString strCode);
//	CString GetLastKey(int iKeyType);
//@��������
//@�����߰�
	CString GetLastHistoryCode(int nMarketType);
	CString GetQueryOpt();
//@�����߰�

	// �������� �߰� ������� : �������� - ojtaso (20070119)
	BOOL IsAddMode();

	// �������� ���� : �������� - ojtaso (20080215)
	void OnSelectChartItem(ST_SELCHART* pSelChart, BOOL bInitKey = TRUE);
	// �������� ���� : �������� - ojtaso (20080215)
	void OnDeleteChartItem(LPCTSTR lpszRQ, ST_SELCHART*& pSelChart);

	// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
	void AddChartItem (CChartItemData* pChartItemData, LPCTSTR lpszItemCode, LPCTSTR lpszItemName, char cType, LPCTSTR lpszTickMinDayNumber, long lCount, long lChartNumericValueType, BOOL bBuiltChart);
	// ������ RQ�� �������� �ʵ��� ���� - ojtaso (20080509)
	BOOL GetSelChart(LPCTSTR lpszRQ, ST_SELCHART*& pSelChart, BOOL bChangeSelRQ = TRUE);

	// �����ڵ�� RQ�� ã�� ���� : �������� - ojtaso (20070705)
	BOOL FindSelectChartByItemCode(LPCTSTR lpszItemCode, BOOL bSelect = TRUE);

	CString GetSelectedItemCode() { return m_strSelectedItemCode; };
	BOOL IsAvailibleCode(LPCTSTR lpszCodeName);

	void ResetRQ();

	// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)
	void EnableRequestMoreButton(BOOL bEnable);

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	void WriteToStringLog( const char *p_szSection, const char *p_szData, int p_nDataLen);
#endif

// (2008/9/14 - Seung-Won, Bae) for Pair Tick
public:
	void	SetCurrentRQ( CString strRQ);

	void RemoveMapChartItem();

// (2008/9/24 - Seung-Won, Bae) Disable Index Sync for Multi-Item Chart.
//	Support ChartItem Count
public:
	int		GetChartItemCount( void)	{	return m_mapChartItem.GetCount();	}


//<<�ַθ�
// (2008/11/21 - Seung-Won, Bae) for No Ctrl.
protected:
	CChartMng *		m_pChartMng;

public:
	IRealReceiver*	m_pIRealReceiver;


	void			SetChartOrderType(CString	strType);
	ST_CHARTITEM_REALDATA* GetChartItemRealData(CString strRealItemCode);
//>>

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_CHARTITEM_H__11999AEF_8200_4DA5_B205_33C1402F54D5__INCLUDED_)
