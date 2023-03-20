#if !defined(AFX_STDDIALOG_H__CF66AE4F_AD1A_4057_B00E_F9FAA4C3A30F__INCLUDED_)
#define AFX_STDDIALOG_H__CF66AE4F_AD1A_4057_B00E_F9FAA4C3A30F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------------------
//Comment	: �ּ�ó��. �ּ�ó�� ������ ��Ȯ���� ���� �κ���
//			  �θ�������� ���ٹ� �븮���� ��Ȯ�� Ȯ���Ѵ�.	
//			  gm0604@orgio.net
//
//Date		: 2005. 04. 20
//Author	: Sang Yun, Nam
//-----------------------------------------------------------------------------------

// StdDialog.h : header file
//
//#include "./control/ParentChartDlg.h"
#include "../chart_common/OutsideCommonInterface.h"
//#include "../chart_common/DataList/RealChartData.h"

#ifndef __HANHWA_CHART_REPLAY_INFO_H
	#include "../chart_common/ReplayChartInfoDef.h"
#endif

#include "Resource.h"
#include "xySplitterWnd.h"
#include "ChartMng.h"
#include "LeftBarDlg.h"
#include "InputBarDlg.h"
#include "ExtInputBarDlg.h"
#include "../../inc/PcTrMngHelper.h"
#include "./control/RGBButton.h"


#include "../../inc/IAUPartnerMng.h"		//@090922 For IPartnerSite

//#include "../../ToolKit/MainDll_Interface/Platform.h"	// �÷��� ���� �κ� ����.
//#include "../../ToolKit/MainDll_Interface/WndObj.h"
//#include "../../../COMMON_BASE/TrComm.h"
//#include "../../INCLUDE_BASE/DllLoadDef.h"
//#include "../../common_base/trComm/TRComm.h"
//#include "../../ToolKit/SocketModule/TrComm.h"
//Han Hwa #include "../../include_contents/LoadDrds.h"



//-----------------------------------------------------------------------------
// Author		: Sang-Yun, Nam	Date :2005/4/18/MonDay
// Reason		: 
//-----------------------------------------------------------------------------
//start
// {{ ������ ���̺귯�� ��ũ ( Ȯ�� dll )
//#include "../../../DataMgr/DBMgr.h" //05.09.27
//#include "../../COMMON_BASE/baseInfo/BaseInfo.h" //2005.09.27 add by sy, nam
//#include "../../../DrMLib/DrMiddleLib.h"//05.09.27

//#include "../../../include/EnvDir.h"
//#include "../../../Include/CommonStruct.h"
//#include "../../../include/UserMsg.h"				//KJI 20030305 Add
//#include "../../../include/CommonDef.h"			//KJI 20030305 Add
//#include "../../../CodeFind/JMCodeFindDlg.h"
//#include "../../../CommonLib/ServerMsg.h"
//#include "../../common_base/drds/DataUtil.h"

//#include "../../COMMON_BASE/Tree/CodeMnager.h"
//end

#include "../../inc/ExOCXDLL.h"
#include "../../inc/IDebugMng.h"

const UINT RMSG_SELCHARTCODE = ::RegisterWindowMessage( "RMSG_SELCHARTCODE" );
/////////////////////////////////////////////////////////////////////////////
// CStdDialog dialog
#define BOUND_GAP 0//10
#define	DEFAULT_GRAPH					3			// �ʱ� graph count	

	//DIV_OVERLAP : �������Ʈ
enum	{MULTI_CHART, SINGLE_CHART, MINI_MULTI_CHART, MINI_SINGLE_CHART, DIV_OVERLAP};
//#define IMPLCTRLCLASS CStdDialog

//#define MYMETHOD_PROLOGUE(theClass, localClass) \
//	theClass* pThis = \
//	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
//	pThis;

typedef struct {
	CString		m_strTrCode;
	CWnd		*m_pWndRequested;	
	int			m_nType;
	void		*m_pRqstChartItem; //������ǥ TR�� ������.. ���� ��Ʈ TR�� ��ũ�� �� �´� �� ������ ���⿡ ��û�� OCX�� Pointer�� ���� 
} STTRINFO;

typedef struct {
	CWnd* m_pWndRequested;
	double m_dPrevPrice;
	int m_nChartCount;
} STTRINFO2;

// �������� ������ : �������� - ojtaso (20070208)
class CRealTRCounter
{
public:
	CRealTRCounter() 
	{
	}

	~CRealTRCounter() 
	{
//		m_mapTrCounter.erase(m_mapTrCounter.begin(), m_mapTrCounter.end());
		m_mapTrCounter.RemoveAll();
	}

	UINT AddTr(CString strKey)
	{
//		map<string, UINT>::iterator it = m_mapTrCounter.find(lpszTR);
//		if(it == m_mapTrCounter.end())
//		{
//			m_mapTrCounter.insert(map<string, UINT>::value_type(lpszTR, 1));
//			return 1;
//		}
//
//		return ++(*it).second;
		UINT nCount;
		if(!m_mapTrCounter.Lookup(strKey, (LPVOID&)nCount))
		{
			m_mapTrCounter.SetAt(strKey, (LPVOID)1);
			return 1;
		}
		else
		{
			++nCount;
			m_mapTrCounter.SetAt(strKey, (LPVOID)nCount);
		}

		return nCount;
	}

	UINT RemoveTr(CString strKey)
	{
//		map<string, UINT>::iterator it = m_mapTrCounter.find(lpszTR);
//		if(it != m_mapTrCounter.end())
//			return --(*it).second;
//
//		return 0;
		UINT nCount;
		if(m_mapTrCounter.Lookup(strKey, (LPVOID&)nCount))
		{
			if(--nCount)
			{
				m_mapTrCounter.SetAt(strKey, (LPVOID)nCount);
				return nCount;
			}
			else
			{
				m_mapTrCounter.RemoveKey(strKey);
				return nCount;
			}
		}

		return -1;
	}

	void RemoveAll()
	{
		m_mapTrCounter.RemoveAll();
	}

	BOOL GetAllKey(CStringList& strList) const
	{
		strList.RemoveAll();

		int nPos = -1;
		CString strKey, strTemp;
		UINT nCount;
		POSITION pos = m_mapTrCounter.GetStartPosition();
		while(pos)
		{
			m_mapTrCounter.GetNextAssoc(pos, strTemp, (LPVOID&)nCount);

			strKey.Format("%s,%d", strTemp, nCount);
			nPos = strKey.Find(":");
			if(strKey.IsEmpty() == FALSE && nPos > 0)
				strList.AddTail(strKey);
		}
		return TRUE;
	}

	BOOL HasKey(CString strCode)
	{
		if(strCode.IsEmpty())
			return FALSE;

		int nPos = -1;
		CString strKey;
		LPVOID	rValue;
		POSITION pos = m_mapTrCounter.GetStartPosition();
		while(pos)
		{
			m_mapTrCounter.GetNextAssoc(pos, strKey, rValue);

			nPos = strKey.Find(":");
			if(strKey.IsEmpty() == FALSE && nPos > 0)
				if(strCode == strKey.Mid(nPos + 1))
					return TRUE;
		}

		return FALSE;
	}

private:
//	map<string, UINT> m_mapTrCounter;
	CMapStringToPtr m_mapTrCounter;
};

typedef struct {
	COLORREF clrTabSelText;
	COLORREF clrTabNorText;
	COLORREF clrTabBkgrnd;
	COLORREF clrTabSelGDStt;
	COLORREF clrTabSelGDEnd;
	COLORREF clrTabNorGDStt;
	COLORREF clrTabNorGDEnd;
} STTabColor;


//## Han Hwa
LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam);
static int ConvertToChartSign(char* pBuff);

class CLeftBarDlg;
class CInputBarDlg;
class CExtInputBarDlg;
class CMiniBarDlg;
class CChartListDlg;	//2005. 04. 25 add by sang yun, nam
class CFloatingHogaDlg;
class CDataWindowDlg;
class CCodeMnager;		//2005. 07. 26 add by sang yun, nam
class CStdDialog :public CExOCXDLL, public CMChartInterface061115//, public CSharedBase // CSharedBase to use shaered memory is Added By gm0604 2005/7/21
{
public:
	BOOL GetTimeSyncFlag();				//CExtInputBar���� �ð� ���� ���� ��ȸ
	void SetTimeSyncFlag(BOOL bCheck);  //CExtInputBar���� �ð� ���� ���� ������ 

public:
	// ��� �ֹ��� ȣ���� �����ڵ� ���� = "�����ڵ�?ȣ��"
	void SendHogaNCodeToSpeedJumun(CString strCode, CString strPrice);
	//===================================================================================

	// ���� ���õ� ��Ʈ�� �����ϰ� �������� �����´�.
	void GetStartEndDateAtStdDialog(double& dStartDate, double& dEndDate);
	//===================================================================================


	//======================================================================================
	static CImageList   m_ImgList_W15H15; //�� 15�� ���� 15�� ��ư �̹�������Ʈ	
	static CImageList   m_ImgList_W20H20; //�� 20�� ���� 19�� ��ư �̹�������Ʈ
	static CImageList   m_ImgList_W20H20_Tool; //�� 20�� ���� 19�� ��ư �̹�������Ʈ
	static CImageList   m_ImgList_W46H20; //�� 46�� ���� 20�� ��ư �̹�������Ʈ	
	static CImageList   m_ImgList_W50H20; //�� 50�� ���� 19�� ��ư �̹�������Ʈ
	static CImageList   m_ImgList_W31H20; //�� 31�� ���� 20�� ��ư �̹�������Ʈ
	static CImageList   m_ImgList_W151H21; //�� 151�� ���� 21�� ��ư �̹�������Ʈ	
	static CImageList   m_ImgList_W28H22; //�� 28�� ���� 22�� ��ư �̹�������Ʈ
	static CImageList   m_ImgList_W17H20; //�� 17�� ���� 20�� ��ư �̹�������Ʈ
	static CImageList   m_ImgList_W43H20; //�� 43�� ���� 20�� ��ư �̹�������Ʈ
	static CImageList   m_ImgList_W58H20; //�� 58�� ���� 20�� ��ư �̹�������Ʈ
//	static CImageList   m_ImgList_W72H20; //�� 72�� ���� 20�� ��ư �̹�������Ʈ
//	static CImageList   m_ImgList_W46H20_Gwan; //�� 52�� ���� 19�� ��ư �̹�������Ʈ
//	static CImageList   m_ImgList_W8H25;  //�� 8�� ���� 25�� ��ư �̹�������Ʈ
	static CImageList   m_ImgList_W60H20; //�� 60�� ���� 20�� ��ư �̹�������Ʈ	

	static int m_static_nChartCnt4ImgList;
	void InitImageList();
	void DeleteAllImageList();
	//====================================================================================

	//ȭ��ID. fnLoad���� ���õ�.
	UINT	m_nScreenID;

	//ȭ���ȣ. fnLoad���� ���õ�.
	HWND m_hPlatform;
	CString m_strScreenNo; 

	//���� DLL���� Type�� ���� �޸� ������ �ϱ����ؼ� ����Ű��.fnLoad���� ���õ�.
	CString m_szScreenType; 

	enum{Screen_Compchart=0, Screen_ELWChart, Screen_ForeignChart, Screen_fluctuation};	//fluctuation
		// 0:����Ʈ, 1:���ϵ������Ʈ
	int		m_nScreenType;	
	
	//==========================================================================================
	//Chart ȯ�� ���� ���� ����...
	CHART_CONFIG_INFO m_ChartCfgInfo;

	bool	SetImaginaryCandleToOutside		(const bool bImaginaryCandle)	;	// ���������.
	bool	SetShowHighLowBoundToOutside	(const bool bShow			)	;	// ��/���Ѱ� ǥ��
	bool	SetShowExRightDividendToOutside	(const bool bShow			)	;	// �Ǹ���/���� ǥ��
	bool	SetTimeLinkageToOutside			(const bool bTimeLinkage	)	;	// �ð����� ����
	bool	SetAutoSaveToOutside			(const bool bAutoSave		)	;	// ��Ʈ����� �ڵ� ���� ����
	//bool	SetShowCurrentPriceToOutside	(const bool bShow			)	;	// ���ݴ������� ���簡ǥ��
	bool	SetPriceLog						(const bool bLog			)	;	// ������Ʈ log ����.

	bool	GetImaginaryCandleToOutside		(bool& bImaginaryCandle		)	;	// ���������.
	bool	GetShowHighLowBoundToOutside	(bool& bShow				)	;	// ��/���Ѱ� ǥ��
	bool	GetShowExRightDividendToOutside	(bool& bShow				)	;	// �Ǹ���/���� ǥ��
	bool	GetTimeLinkageToOutside			(bool& bTimeLinkage			)	;	// �ð����� ����
	bool	GetAutoSaveToOutside			(bool& bAutoSave			)	;	// ��Ʈ����� �ڵ� ���� ����	
	//bool	GetShowCurrentPriceToOutside	(bool& bShow				)	;	// ���ݴ������� ���簡ǥ��
	
	bool	OnChartEnvSetupOk()										;	// ��Ʈ ȯ�漳�� â�� ������ ���� �Ҹ���.
	//==========================================================================================

	//==========================================================================================
	//OCX�� ��û �ϴ� �κп� �߰����� �Լ� �Լ�
	//2005. 08. 08 added by sy, nam 
	// Start >>>> ==============================================================================
	// ��� �� UserID
	LPCTSTR GetMainPath();
	LPCTSTR GetUserID();		
	//End ==================================================================================<<<<
	bool m_bActiveDataWindow;
	CDataWindowDlg*	m_pDataWindow;

	CRect m_ViewRect;	
	void ShowDataListView(BOOL bShowDataListView, CRect ViewRect);
	void CreateDataWindow();
	void DestroyDataWindow();
	void SetDataWindowData(CList<CString, CString&>* pListData);
	bool IsActiveDataWindow();

	//============================================================================================
	//�ϴ� ����ǥ�� �ٿ� �޼����� ������.
	void CStdDialog::SendMessage2StatusBar(int nMsgType,  const char* pMsg = _T(""), int nNum =0);

	// ���� ���� �˻� ���� ============================================================================================
	void Send2HanHwa_IFCond(void* pData, int nLen, CWnd* pRequestWnd, CString strTRCode);

	// KOSPI, KOSDAQ ��Ÿ ����� ���� ���� �˻� ���� ============================================================================================
	CWnd* m_pRqWndFromTab;
	void Send2HanHwa_09501(void* pData, int nLen, CWnd* pRequestWnd, CString strTRCode);
	
	//���� ��ȸ�� ������ �ڵ带 ������ �ִ´�. 2005. 07. 26============================================================
	CString			m_strMainCode;

	//CCodeMnager Pointer 2005. 07. 26============================================================
	CCodeMnager*	m_pMainCodeManager;
	void			SetpMainCodeManager(CCodeMnager* pCCodeMnager)	{ m_pMainCodeManager = pCCodeMnager;}
	CCodeMnager*	GetpMainCodeManager()							{ return m_pMainCodeManager;		}	

	

	// 2005. 07. 18 add by sy, nam ===============================================	
	//�������� ��ƮOCX�� ������ �� DataWindow�� ������ ������ �ٸ��� �ϱ� ���� ȣ���Ѵ�.
	BOOL  ChangedChartItem(); 
	//=============================================================================
	
public:

	// 2005. 07. 15 add by sy, nam =================================================
	void AddDataKey_Hoga();									//003 ȣ��TR_AddDataKey						
	void Request_StockHogaData(CString strRecvCode = _T(""));
	void Request_StockRealHogaData(CString strCode);
	void Request_Disconnect_StockRealHogaData(CString strCode);
	//==============================================================================




	void    SetRealInfo		(
								void* pDrds,		// Drds�������ε� NULL�� ����.
								HWND hWnd,			// Chart OCX�� ������ �ڵ�
								UINT id, 			// CPacket* 
								LPSTR DataName,		//  ex: "S31"
								LPSTR FieldName, 	//  ex: "time"
								LPSTR KeyName);		//  ex: "000660"


	void    UnSetRealInfo	(
								void* pDrds,		// Drds�������ε� NULL�� ����.
								HWND hWnd,			// Chart OCX�� ������ �ڵ�
								UINT id, 			// CPacket* 
								LPSTR DataName,		//  ex: "S31"
								LPSTR FieldName, 	//  ex: "time"
								LPSTR KeyName);		//  ex: "000660"

	void	OnChartMouseMove(UINT	nFlags, CPoint	pt);

	CPtrList				m_lstRegRealChart;
	
	void					SendStockReal(char* pData);
	void					SendUpjongReal(char* pData);
	void					SendFutureReal(char* pData);
	void					SendOptionReal(char* pData);


	IMasterDataManagerLast* m_pDataManager;
	CString GetCodeNameAndGubun(int nChartType, CString csCode);
	


	// ### ��Ʈ ������ 2005. 05. 16 add by sy, nam -----------------------------------------------
//@��������
//	enum	{STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, ELW_CHART, FOREIGN_CHART, COMMODITY_CHART};	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
//	int m_nChartGubun; // STOCK_CHART - �ֽ���Ʈ, UPJONG_CHART - ������Ʈ, FUTOPT_CHART - �����ɼ� ��Ʈ
//@��������
//@�����߰�
	int m_nMarketType; // STOCK_CHART - �ֽ���Ʈ, UPJONG_CHART - ������Ʈ, FUTOPT_CHART - �����ɼ� ��Ʈ
	void ChangeCode(LPCTSTR szCode, int nDefault = MODE_DEFAULT, int nMode = CHANGE_ITEM);
	void ChangeMarket(UINT nMarketType, BOOL bUpdateCbMarket = TRUE , BOOL bRequestToServer = FALSE);

	BOOL IsCompareChartMode() { return m_nChartType == MULTI_CHART; }
	void EnableQueryOpt(BOOL bEnable);
//@�����߰�	

	int GetMarketTypeFromCode(CString strItemCode, int& nCodeType);
	CString GetItemNameFromItemCode(CString strItemCode, BOOL bPreFix=TRUE);

public:
	//-----------------------------------------------------------------------------
	// Author		: Sang-Yun, Nam	Date :2005/4/12
	// Reason		: ��ȭ�� ������ Member�߰�	
	//				- �������̽� ������ ���� ���� ����  �� �߰� 
	//-----------------------------------------------------------------------------
	HWND		m_hMainFrame;							// CMainFrame�� Handle	


	CString		m_strUserID;							// ����� ID
	void		SetInitInfo();							// ȭ�� Create�� ȣ��Ǹ� 
														// ����� ID, RootPath, Image Path���� ���´�.
//	void		SendTR_TEST(CString strRecvCode);		// TR���� �׽�Ʈ

	// ��Ʈ ����Ÿ ����Ʈ â�� �������� ���θ� �����ϰ� �ִ� ���� - 2005. 04. 25
	BOOL m_bShowDataListWnd;

	// gm0604
	void InitChartBySystemTrading(LPCTSTR lpMessage);
	void SetChartBySystemTrading(BOOL bIsFirst);
	HWND m_hWndSystemTrading;
	CString m_strInputCode;
	CString m_strEndDate;
	CStringList m_stringListTimeType;
	CList<long,long> m_longListDataCount;
	CList<long,long> m_ptrListSystemTrading;
	CStringList m_stringListSTName;
	//CRealTRCounter	m_RealTrCounter;	// �������� ������ : �������� - ojtaso (20070208)
											// �񱳵�������� �����ϹǷ� �����غ���.
	
// Construction
public:
	CStdDialog(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CStdDialog)
	enum { IDD = IDD_STD_DIALOG };
	CILButton	m_btnExtTool;
//	CRGBButton	m_btnSideBar;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

protected:
	// �÷��� ���� �������̽��� ������ �Լ� ó���� ������ �� �ִ�.
	// ���� ����� �Լ� �̿ܿ� �� �ʿ��� �Լ��� �ִٸ� ���� ����������. ExOCXDLL.h ����
	virtual void OnGSharedDataChange(LPCTSTR lpSharedName, LPCTSTR lpGSharedData, BOOL bWithStart);

	virtual void Platform_Initialize(long dwMainDllObj, long dwBaseDesk, long dwRscMng, CStringList* pAllProperties);
	virtual LRESULT Platform_WindowProc(UINT message, WPARAM wParam, LPARAM lParam, int &nContinue);
	virtual LRESULT Platform_PreTranslateMessage(MSG* pMsg);
	
	virtual void Platform_OnSize(UINT nType, int cx, int cy);

	BOOL	TrackPopupMenu();	// �÷��� �˾�

public:
	HRESULT ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);	
	HRESULT ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData);
	HRESULT	ReceiveMessage(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage);
	HRESULT	StatusChange(int nState);

	// Demo�����غ��.
	HRESULT DemoReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);

	// ���(����) ó�� ���
	ITrComm2*		m_TRComm;

	//�޼��� â ����.
	HRESULT		RecieveError(int nError, LPCSTR szMessage);
//	CWnd		*m_pWndRequested;
	
/*	class CMyCallback : public ICallbackStatus	
	{
		STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * lplpObj) {return S_OK;}
		STDMETHOD_(ULONG,AddRef) (THIS)  {return S_OK;}
		STDMETHOD_(ULONG,Release) (THIS) {return S_OK;}
		
		STDMETHOD(RecieveData)(HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
		{
			MYMETHOD_PROLOGUE(CStdDialog, MyCallback)
			return pThis->ReceiveData(hTrComm, szTR, szMessage, aTRData, dwTRDateLen);			
//			return S_OK;		
		}
		STDMETHOD(StatusChange) (int nState)
		{
			MYMETHOD_PROLOGUE(CStdDialog, MyCallback)
			return S_OK;			
		}
		STDMETHOD(RecieveError) (int nError, LPCSTR szMessage)
		{
			MYMETHOD_PROLOGUE(CStdDialog, MyCallback)
			return pThis->RecieveError(nError, szMessage);
			return S_OK;			
		}
	} m_xMyCallback;
	friend class CMyCallback;

	
	// * Tr ���� �޼����� �޴´�. 
	HRESULT ReceiveData(HWND hTrComm, 
						LPCSTR szTR, 
						LPCSTR szMessage, 
						LPVOID aTRData, 
						DWORD dwTRDateLen);	*/


	
/* ### Han Hwa
	/////////////// drds
	CDrdsLoadLib	m_DrdsDLL;
	void			*m_pDrds;			
	STDRDSINFO		m_DrdsInfo[MAXPACKET];
	int				m_nDrdsInfoCnt;
	void	InitDrds();
	BOOL	MakeDrdsConnectAdviseFromCode(int nI);
	BOOL	MakeDrdsUnConnectUnAdviseFromCode(int nI);
//## Han HWa*/
	typedef struct {
		CString DataName;
		CString FieldName;
		CString KeyName;
		UINT	nID;
		BOOL	bAdvise;
	} STDRDSINFO;
//@�������� : m_DrdsInfo������ ����ϴ��� Ȯ���ʿ���.
	STDRDSINFO		m_DrdsInfo[MAXPACKET];
	int				m_nDrdsInfoCnt;
	IDrdsLib*		m_pDrdsLib;

	void	InitDrds();
	// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
	BOOL	MakeDrdsConnectAdviseFromCode(int nI, BOOL bAddMod);
	// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
	BOOL	MakeDrdsUnConnectUnAdviseFromCode(int nI, BOOL bAddMod);

	// ���� ��Ʈ���� �������� �ִ� �ڵ� ���� List
	// �������� ������ : �������� - ojtaso (20070208)
	CStringList		m_Codes;

	// Real Data ��û	- 2005. 04. 19 by Sang-Yun, Nam
	// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
	void	OnCodeAdvise(CString strCode, BOOL bAddMode);
	
	// Real Data ���� ��û	- 2005. 04. 19 by Sang-Yun, Nam
	// �������� �ǽð� ó�� : �������� - ojtaso (20070109)
	void	OnCodeUnAdvise(CString strCode, BOOL bAddMode);

	// m_strMapID�� ����� ���� ȭ���ȣ(MapID)�� Return�Ѵ�.	- 2005. 04. 19 by Sang-Yun, Nam
	CString	GetMapID();

	//	m_bEnable (ȭ�� ��ü�� Enable / Disable ���� ��) Return		- 2005. 04. 19 by Sang-Yun, Nam
	BOOL	GetStatusEnable();

	//  ���� ���õ� ���� ����Ʈ �� �������� ���õ� Index ���� ��ġ���� ���� ���õ� ���� ���� ��ŭ �� �Ʒ��� �̵�
	//	- 2005. 04. 19 by Sang-Yun, Nam
	void	SetJongListPrev();

	//  ���� ���õ� ���� ����Ʈ �� �������� ���õ� Index ���� ��ġ���� ���� ���õ� ���� ���� ��ŭ �� ���� �̵�
	//	- 2005. 04. 19 by Sang-Yun, Nam
	void	SetJongListNext();

	//	��ü ���� ����Ʈ�� �� CString ���·� Return �Ѵ�.
	//	- 2005. 04. 19 by Sang-Yun, Nam
	CString GetAccountInfo();

	//	ȯ�� ���� ���� ��ΰ� ����Ǵ� ���� - 2005. 04. 19 by Sang-Yun, Nam
	CString			m_strConfig;

	//	2005. 04. 19 ���� Han Hwa���� ������� ����. - 2005. 04. 19 by Sang-Yun, Nam
	CString			m_strConfigOfMain;

	//	"Data"���� ��ΰ� ����Ǵ� ���� - 2005. 04. 19 by Sang-Yun, Nam
	CString m_szDataDir;

	//	"Image"���� ��ΰ� ����Ǵ� ���� - 2005. 04. 19 by Sang-Yun, Nam
	CString m_szImageDir;

	//	"Bin"����( ���� ����) ��ΰ� ����Ǵ� ���� - 2005. 04. 19 by Sang-Yun, Nam
	CString m_strRootPath;

	//	����� ���� ��ΰ� ����Ǵ� ���� - 2005. 04. 19 by Sang-Yun, Nam
	CString m_strUserDir;

	//	���� �ڵ�
	HWND	m_hSocket;

	//	���� ȭ���ȣ(MapID)�� �����Ѵ�. - 2005. 04. 19 by Sang-Yun, Nam
	CString			m_strMapID;

	//	���ø� ��ȣ (2���� ���۵�)�� �����Ѵ�.
	//	����ȭ�鿡���� ȭ����� ������ ������ �־  
	//	ȭ���ȣ�� �ϳ��ε� ����ȭ����� ���� ���� �ִµ�.
	//	���⼭ ������ �����ڵ带 �� ȯ�漳�� ������ �ҷ��� �޾ƿ� ID
	//  ����ڰ� ������ 6810 ����� ���� ȭ�鿡 �� ��Ƽ��Ʈ�� 3���� �� ������쿡..
	//	������ ��ġ�� ���� �ٸ� ID�� �����µ� �̸� �ǹ��Ѵ�. - 2005. 04. 19 by Sang-Yun, Nam
	UINT			m_nSpliteID;

	//	ȭ�� ���۽� ���̴� Tab Index ( Zero Base ) - 2005. 04. 19 by Sang-Yun, Nam
	int		m_nTabID;

	//	ȭ���� �뵵�� ���� ��Ʈ�� ���̴� ���°� �޶����� �� �̸� Index�� �����Ͽ� ����
	//	0:Normal(ǥ��), 1:Mini, 2:OrderSrc&Chart - 2005. 04. 19 by Sang-Yun, Nam
	UINT	m_nChartType; 

	//	�� Dll�� �Բ� ���Ե� �ٸ� ������ ������ ID�� �����ϰ� �ִ�. - 2005. 04. 19 by Sang-Yun, Nam
	UINT	m_nImportedMapID;

	//	Dialog �� - 2005. 04. 19 by Sang-Yun, Nam
	UINT	m_nWidthOfDlg;

	//	Dialog ���� - 2005. 04. 19 by Sang-Yun, Nam
	UINT	m_nHeightOfDlg;

	//	�� Dlg�� ���ΰ� �ִ� View�� Pointer - 2005. 04. 19 by Sang-Yun, Nam
	HWND	m_hViewInMain;
	HWND	m_hKSDllView;	// �Ҹ���ġ DLL�� ����ó���� View Handle

	BOOL			m_bIsSystemTradingMode;// gm0604 added 2005.7.25

	// 2�� ������ ��� ���ٸ� ǥ���ϴ� Dlg
	CInputBarDlg	*m_pInputBarDlg;
private:
	// chart ocx�� ��Ŷ�� �����ϱ� ���� ������ 
	// tr�� ���� ���� ����Ÿ�� m_ChartMng�� ���� ��Ʈ�� ����
	CChartMng m_ChartMng;

	// �����ڵ� OCX
	//CAny2Code m_code;

	// Splitter Bar
    CxSplitterWnd   m_xSplitter1;

	// Splitter ������ Dlg
	CLeftBarDlg		*m_pLeftBarDlg;	

	// 2�� ������ ��� ���ٸ� ǥ���ϴ� Dlg
	//CInputBarDlg	*m_pInputBarDlg;

	// 2�� ������ �ϴ� ���ٸ� ǥ���ϴ� Dlg
	CExtInputBarDlg	*m_pExtInputBarDlg;

	// CLeftBarDlg �� ��ġ�� ���� ������ �ϱ����� ������ ���� �����ϰ� �ִ� ����
	UINT			m_nLeftBarType;	// 0: None, 1:Left, 2:Right 
	
	// CLeftBarDlg�� ��
	int				m_nWidthOfLeftBar;

	// ȭ�鳻 ��Ʈ�� Enable/Disable Toggle
	BOOL			m_bEnable;

	// ���� �����͸� �ѹ� ���� �� FALSE��...??
	BOOL			m_bFirstGShared;

	// 1�� ������ �ϴ� ���ٸ� ǥ�� ���� - gm0604 added For SystemTrading 2005.7.25 
	BOOL			m_bShowInputBarDlg;

	// 2�� ������ �ϴ� ���ٸ� ǥ�� ����
	BOOL			m_bShowExtInputBarDlg;

	// ���� ȭ�� ���� ���� 
	BOOL			m_bSmallScreenMode;

	// ������� �ʴ� ��..
	int				m_nTempLeftBarType;

	// TR�� ��û�� ����(HWND, TR_NO���� ������ STTRINFO �� Array�� ����
	CArray<STTRINFO,STTRINFO> m_arryQuery;

	// ������ǥ���� ��û�� ������ STTRINFO �� Array�� ����
	CArray<STTRINFO,STTRINFO> m_arryQueryInvoke;

	// �⺻ ��
	long			m_lWidthOriginal;

	// �⺻ ����
	long			m_lHeightOriginal;

	// ��Ʈ ���� ���� Toggle
	BOOL			m_bHideChart;

	// �������� �ʱ�ȭ Toggle
	BOOL			m_bInitResize;

	// User �� ���� ������ ������ �ִ´�.
//	CString			m_strAccountInfo;

	// gm0604 added 2005.7.25 Start
//	CStringArray	m_strArrayAdviceInfo;
	// gm0604 added 2005.7.25 End
	
	// �ý��� ��Ʈ ����
	BOOL m_bSystemChart;
	
	// �ܼ���Ʈ ����
	BOOL m_bSimpleChart;

	// �������� ��Ʈ ����
	BOOL m_bMultiItemChart;

//====================================
//	CIndicatorList	m_IndicatorList;
//====================================
// Operations
public:		
	void AllCodeUnAdvise();
	void DestroyFloatingHogaWnd();
	
	// m_bHideChart �� Return
	BOOL	IsChartHide();

	// chart ����
	BOOL	HideChart();

	// ȭ�� Load�ÿ�  ���� �ʱ�ȭ 
	void	FormLoad();

	// Resizing �� ��Ÿ ��Ʈ�� ���ġ
	void	Resize();

	// CLeftBarDlg�� ���¸� ����
	void	SetLeftBarType(UINT nLeftBarType);

	// CLeftBarDlg�� ���� Index Return
	UINT	GetLeftBarType() {return m_nLeftBarType;}

	// �ϴ� ���� Show/Hide ����
	void	SetShowExtInputBarDlg(BOOL bShowExtInputBarDlg);

	// �ϴ� ���� Show/Hide ��(m_bShowExtInputBarDlg) Return
	BOOL	GetShowExtInputBarDlg() {return m_bShowExtInputBarDlg;}
	
	// ��ü HTS ���� ������ ����� ȣ��
	//BOOL	ChangedGlobalData(CString szName, CString szValue);

	// ���� ȭ�� / ū ȭ�鿡 ���� ȭ���ġ
	void	SetSmallView(BOOL bSmall = TRUE);

	// ���ǰ˻��� �ܺο��� ���� ����Ʈ�� �޴´�.
	void	InputCodeFromOutSide(CString strPacketFromOutSide);

	//
	void	*m_pData;
	
	//�ֽ�/����/�����ɼ� ����
	void	ChangeStockMode(int type);
	void	ChangeMiniStockMode(int type);

	//008 {{
	void	ChangeUpjongMode(UINT type);	// �ڽ���/�ڽ��޾��� �����Ͽ� �ڵ��޺� ����
	//008 }}

	BOOL	IsSystemChart(){ return m_bSystemChart; };
	BOOL	IsSimpleChart(){ return m_bSimpleChart; };
	BOOL	IsMultiItemChart() { return m_bMultiItemChart; }

	//20061212 ������ ����
	//GRID Column Ȯ��button �߰�
	void	GridColumnExtend();			//btn click ȣ�� func
	int		m_nExtendState;				//btn ���� ���� 0:���/1:Ȯ��
	//20061212 ���� end
private:
	// ������ TR������ ��û�Ѵ�.
	void	DoCheckMin(int nType, LPCTSTR lpData, CString& szReqTr);
	int		Send2Server(LPCTSTR lpTrCode,LPCTSTR lpData,int nSize, BOOL bWait, int nArrayIndex = -1);

	// CLeftBarDlg�� ũ�⿡ ����  nID�� �־��� Control�� ũ�� ��ġ���� �����Ѵ�.
	CRect	GetRect(UINT nID, BOOL bInit = FALSE);

	// m_pLeftBarDlg,m_pInputBarDlg,m_pExtInputBarDlg,m_ChartMng���� Enable/Disable ��Ų��.
	void	EnableWindowAll(BOOL bEnable = TRUE);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStdDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation

protected:

	// Generated message map functions
	//{{AFX_MSG(CStdDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	LONG	OnWmSplitterMoved( UINT wParam, LONG lParam );	//Splitter Bar Moving Msg
//@Solomon090819 	afx_msg long OnGetBroad(WPARAM wParam, LPARAM lParam);
	LONG	OnByPass_ExportToolOption(UINT wParam, LPARAM lParam); //���� ���õ� ��ƮDll�� �ܺ����� ���� ������ �� �����Ѵ�.	
	long	ChangeSharedData(CString sShareName, BYTE* pData, int nLen, int nStock=0);// ���� ������ 
	LRESULT OnDropData(WPARAM wp, LPARAM lp);
	//-----------------------------------------------------------------------------
	// Author		: {{{ ������ }}} ojtaso 	Date : 2005/2/23
	// Reason		: Added to drag and drop operation.
	//-----------------------------------------------------------------------------
	// BEGIN
	LRESULT OnDragEnd(WPARAM wParam, LPARAM lParam);
	LRESULT OnDragStart(WPARAM wParam, LPARAM lParam);

	//��Ų
	LRESULT OnChangeSkinColor(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetSkinColor(WPARAM wParam, LPARAM lParam);
	
	LRESULT OnUserInfo(WPARAM wp, LPARAM lp); 

	// END
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CStdDialog)
	afx_msg void SetOpenMapData(LPCTSTR szOpenData, short nDataLen);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
	CString m_strOut;
	CString m_strMarketOut;
	void ConvertKoscomData(LPCSTR szTR, LPVOID	aTRData, long dwTRDateLen, LPCSTR szCode);

	CString MakeChartData(LPVOID aTRData, int& nCount, int dwTRDataLen, int nMarketType, int nType, double dPrePrice, int nUnitType=WON_TYPE);
	CString MakeChartData(LPVOID aTRData, int nIdx, int& nCount, int dwTRDataLen, int nMarketType, int nType, double dPrePrice = 0, int nUnitType=WON_TYPE);
	char m_cUM;
	//by LYH 2006/7/7 Drag & drop
	HCURSOR				m_hCursor;
	int					m_nMarketHandle;
	CString				m_strDollar;
	//20070131 by ������
	char m_cMUM;
public:
	void	NofityNewDataReceive();		// �����͸� ���� �޾Ҵٴ� ���� �˸���. For TabSystemTradingEx

	//��Ŭ������� by LYH 2006.10.02
	BOOL m_bOneClickHide;
	void SetOneClickHide(BOOL bOneClickHide);
//	int	m_nMarketTrCode;

	BOOL	GetRevisedPriceFlag( void);		//�����ְ� üũ���� ���
	BOOL	SetRevisedPriceFlag( BOOL p_bRevisedFlag);	//�����ְ� �ٲ���. �����뺸
	BOOL	OnIndicatorAddOrRemoved( const char *p_szGraphName,	const BOOL p_bAdded);	//��ǥ�߰� �� ���� �̺�Ʈ
	BOOL	OnUserZoomChanged( const int p_nDataCountInView);	//�� �̺�Ʈ
	BOOL	OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn);	//��ũ�� �̺�Ʈ

	CString m_strRecentFutureCode;
	CString GetRecentFutureCode(){return m_strRecentFutureCode;};

//	CString m_strMapFileName;
	
	void	AddRealCodes(CString strTrCode, LIST_CODEDATA *pListRealCodes);
	void	RemoveRealCodes(LIST_CODEDATA *pListRealCodes, CString strKey);
	// �������� �ҷ����� : �������� - ojtaso (20070514)
	void	AddRealCodeList(LPCTSTR lpszCode);
	// �������� �ҷ����� : �������� - ojtaso (20070514)
	void	AddRealCodes();

//	LIST_CODEDATA m_ListStockRealCodes;
//	LIST_CODEDATA m_ListUpjongRealCodes;
//	LIST_CODEDATA m_ListFutOptRealCodes;	
//	LIST_CODEDATA m_ListForeignRealCodes;

	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
	USHORT GetDotPosition(CString& strItemCode);

	//���������� ó��
	BOOL m_bPreMarket;

	//ƽ������ ������� ����
	char m_cUT;

	//2007.04.25 by LYH ���� �ڵ�(���� CodeCombo �ڵ�� ���ؼ� �ٸ� ��� ����ȸ)
	CString m_strRecvCode;

	//2007.05.02 by LYH ���� ��� ���� ���� ID �߰�
//	CString m_strSvcID;

	STTRINFO2	m_stTrInfo2;


	int GetWidthOfLeftBar();

	// �������� ���� ���� : �������� - ojtaso (20070209)
	void	OnDeleteChartItem(LPCSTR lpszItemCode);
	// ����� ��� : �������� - ojtaso (20080213)
	CString		GetJongMokNameByCode(LPCSTR lpszItemCode, LPCTSTR lpszTRCode = NULL);
	// �������� ���� : �������� - ojtaso (20080215)
	void OnSelectChartItem(ST_SELCHART* pSelChart);
	// ���� ����� ���õ� ������ ������ : �������� - ojtaso (20070223)
	BOOL IsSameItemCodeName(LPCTSTR lpszItemName);
	// ��Ʈ �ʱ�ȭ�� ��� ���� ���� : �������� - ojtaso (20070621)
	void ResetChartAll();

	// ����Ʈ ������ Ű�� ��� - ojtaso (20071226)
	void EnableRequestMoreButton(BOOL bEnable);

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	void WriteToStringLog( const char *p_szSection, const char *p_szData, int p_nDataLen);
	void CreateLogFile();
	void CloseLogFile();

	HFILE		m_hLogFile;			// Log File Handle
#endif
	
public:
	// ����Ʈ�� �߰��� ������ �����Ѵ�.	
	BOOL	RemoveGraph_Title(int nRow, int nCol, CString strCode, CString strJMName, BOOL bIsOnlyNoTRData);
	BOOL	RemoveGraph(int nRow, int nCol, CString strCode, CString strJMName);

	// strName(�����)�� �ش��ϴ� �����͸� ȭ�鿡�� SHOW/HIDE�Ѵ�.
	void SetGraphHide3(CString strCode, CString strName, BOOL bShowGraph);

	// ���� �Է����ǰ� �־��� �����ڵ忡 ���� ��ȸ���ۻ���.
	BOOL RequestTRFormCode(LPCSTR szRuestCode, BOOL bNext=FALSE);
	BOOL GetInputBarInput();

	//�������Ʈ�� ����â.
	BOOL SetupChartUtil();

	// LeftBar�� �ʱ�ȭ ��ƾ Call
	void InitialBtnClick();

	// ChartMng���� ��ȸ��û.
	void SendToChartMngCode(LPCSTR szCode);

	CShareInputData	*m_pShareInputData;

	// ��Ʈ ���߿��� ��������� ���� ��� ���� Left�ٿ��� �����ϴ� ó���� �Ѵ�.
	void DeleteToLeftBar(LPCSTR strItemCode);
	
	// ���� ����ȸ�� ���� ����
//	CStringList	m_szRCodeList, m_szRCodeNameList;
	int GetRequestList(CStringList* pzRCodeList, CStringList* pszRCodeNameList);
//	void RequestNext();
	void FileDelete();


	//--> �������Ʈ
	STDlgCompData	m_stDlgCompData;//�������Ʈ�� ���� ��������.
	CDlgCompDataHandle m_FnHandle;
	//<--

	BOOL m_bSizeTestMode;

	// ��Ų ����
	COLORREF m_clrBackground;
	// Tab
	COLORREF m_clrTabSelText;
	COLORREF m_clrTabNorText;
	COLORREF m_clrTabBkgrnd;
	COLORREF m_clrTabSelGDStt;
	COLORREF m_clrTabSelGDEnd;
	COLORREF m_clrTabNorGDStt;
	COLORREF m_clrTabNorGDEnd;
	
	COLORREF GetBkgrndColor();	//	{ return m_clrBackground; }
	void ChangeSkinColor(COLORREF clrBackground);

	//-->@.080728-001 ��ȸ������ ��������.
	CMapStringToString	m_mapSCDate;
	void GetSCDDataFromBosomsg(CString szKey, CString strData, LPCSTR szTR);

	void OnCodeScdClear(CString strCode, BOOL bAddMode);
	BOOL GetSCDData(CString szCode, CString &rValue);
	//<--

	void GetELWIndexCode(CString strELWCode, CString& strIndexCode, CString& strIndexName);
	//-->@090922 For IPartnerSite
	IPartnerManager*	m_pPTManager;
	CString				m_szPTManager;
	void DoSetPartnerSite(IPartnerManager* pPtManager);

	class CPartnerSite : public IPartnerSite
	{
	public:
		STDMETHOD(RecieveFidData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
		
		STDMETHOD(RecieveTRData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
		
		STDMETHOD(RecieveRealData) (long dwKey, LONG dwData);

		// nKey 0 : ������������
		//		1 : ���� Dialog���� ������ �ڵ�
		STDMETHOD(RecieveKeyData)(int nKey, LPCSTR szData);
		
		STDMETHOD(RecieveRealDataEx) (long dwKey, LPCTSTR pszCode, LONG dwData);

		STDMETHOD(ReceiveOnUser) (WPARAM wParam, LPARAM lParam);
	} m_xPartnerSite;
	friend class CPartnerSite;

	//<--@090922 For IPartnerSite

	IWinixGwanManager*	m_pGwanMng;

	void _DoCheckDaultDataFile(LPCSTR szScreenNo, LPCSTR szUserFile);
	void _DoCopyDefaultToUser(LPCSTR szSect);
	void _DoCheckRecentFuture(LPCSTR szDataFile);

	//Leftbar���� ù��° ���� ���ϱ�
	void GetFirstCode(CString &rString);
	CWnd *GetpLeftBarDlg() { return m_pLeftBarDlg;}//KHD
	BOOL	m_bOnDestroy;

	BOOL IsContinueFutCode(CString& strCode);		// ���ἱ���� ��� �ֱٿ����� ����
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDDIALOG_H__CF66AE4F_AD1A_4057_B00E_F9FAA4C3A30F__INCLUDED_)
