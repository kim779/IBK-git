#if !defined(AFX_STDLGCONFIRM_H__DF0EEA73_3992_4279_9F24_928A46825D43__INCLUDED_)
#define AFX_STDLGCONFIRM_H__DF0EEA73_3992_4279_9F24_928A46825D43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// STDlgConfirm.h : header file
//
#include "Resource.h"
#include "../../inc/IAuTrCommMng.h"
#include "../CommonTR/SS_OrderDef.h"

//#include "../chart_common/Grid/TR_drds/D49890.h"
#include "../../inc/RealUpdate.h"
#include "Define/ChartMsgDef.h"
#include "../Common_ST/STControllerDef.h"


///////////////////////////////////////////////////////////////////////////////
// CConfirmWnd

DECLARE_INTERFACE(ISTDlgConfirmSendJumun)
{
	STDMETHOD_(CWnd*, GetParent)() PURE;
//	STDMETHOD_(LRESULT, SendJumun)(WPARAM wp, LPARAM lp) PURE;
};

class CConfirmWnd : public CWnd
{
// Construction
public:
	CConfirmWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfirmWnd)
	//}}AFX_VIRTUAL

	CSTOrderRData_EG		m_InData;
	IAUTrCommManager*		m_pITrCommManager;
	ITrComm2*				m_pITrComm;

	CString m_szHogaData[10];	//���簡,�ż�1~3,�ŵ�1~3,���尡,���Ѱ�,���Ѱ�.
	enum {
		hogaIdx_current=0,
		hogaIdx_buy1=1,
		hogaIdx_sell1=4,
		hogaIdx_market=7,
		hogaIdx_hprice=8,
		hogaIdx_lprice=9,
	};
	
	ISTDlgConfirmSendJumun* m_pSTDlgConfirmSendJumun;
	
// Implementation
public:
	virtual ~CConfirmWnd();
	BOOL	OpenWindow();
	BOOL	InitTrComm();
	void RequestHogaData();

	HRESULT ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);	
	HRESULT ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData);

	int		SendJumun_Stock();	// �ֽ��ֹ�
	int		SendJumun_FO();		// �����ֹ�

	void ConvertStockHoga(LPVOID aTRData, long dwTRDateLen);
	void ConvertFutOptHoga(LPVOID aTRData, long dwTRDateLen);
	void ConvertStockFutHoga(LPVOID aTRData, long dwTRDateLen);
	void ConvertStockOptHoga(LPVOID aTRData, long dwTRDateLen);


	CString m_strOut;
	CString m_strCurPrice;			// ���簡
	CString m_strUpLimitPrice;		// ���Ѱ�
	CString m_strDownLimitPrice;	// ���Ѱ�
	CString m_strBuy1Hoga;			// �ż�1ȣ��
	CString m_strBuy2Hoga;			// �ż�2ȣ��
	CString m_strBuy3Hoga;			// �ż�3ȣ��
	CString m_strSell1Hoga;			// �ŵ�1ȣ��
	CString m_strSell2Hoga;			// �ŵ�2ȣ��
	CString m_strSell3Hoga;			// �ŵ�3ȣ��

	class CMyITrCommSite : public ITrCommSite
	{
	public:
//<<20100308_JS.Kim ����
// --> [Edit]  ������ 2008/07/25		( �����͸� �ѱ涧 ���Ͱ� �ƴ� �׳� Char�� �ѱ� )
		STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
		{
			METHOD_PROLOGUE(CConfirmWnd, MyITrCommSite)
				return pThis->ReceiveData(dwKey, szTR, szMsgCode, szMessage, aTRData, dwTRDateLen);
		}
//		STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, char* pData, long dwTRDateLen)
//		{
//			METHOD_PROLOGUE(CConfirmWnd, MyITrCommSite)
//				return pThis->ReceiveData(dwKey, szTR, szMsgCode, szMessage, pData, dwTRDateLen);
//		}
// <-- [Edit]  ������ 2008/07/25
//>>
		STDMETHOD(ReceiveRealData) (WORD wFID, LPCSTR szMainKey, void* pRealData)
		{
			METHOD_PROLOGUE(CConfirmWnd, MyITrCommSite)
				return pThis->ReceiveRealData(wFID, szMainKey, pRealData);
		}

		STDMETHOD(StatusChange) (int nState)
		{
			MYMETHOD_PROLOGUE(CConfirmWnd, MyITrCommSite)
			return S_OK;
		}

		STDMETHOD(ReceiveMessage) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage)
		{
			MYMETHOD_PROLOGUE(CConfirmWnd, MyITrCommSite)
			return S_OK;
		}
	} m_xMyITrCommSite;
	friend class CMyITrCommSite;

	// Generated message map functions
protected:
	//{{AFX_MSG(CConfirmWnd)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDLGCONFIRM_H__DF0EEA73_3992_4279_9F24_928A46825D43__INCLUDED_)
