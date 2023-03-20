#if !defined(AFX_PATTERNMAKER_H__C8385516_C663_49EB_BC25_79CAEC4882FA__INCLUDED_)
#define AFX_PATTERNMAKER_H__C8385516_C663_49EB_BC25_79CAEC4882FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatternMaker.h : header file
//

#include "./Include_AddIn.h"

#include "HighLowPtData.h"								// �ֿ����, �ֿ����� ������ ���� Ŭ����
#ifndef __COMMON_DEF_H
#include "./Client/commondef.h"
#endif

#include "./Include_PCTR/IPCTRMng.h"
#include "./Include_PCTR/IGateMng.h"
#include "./Include_PCTR/IAuTrCommMng.h"

/////////////////////////////////////////////////////////////////////////////
// CPatternMaker Object

extern CConfigSet g_clConfigSet;
class CPatternMaker : public CObject
{
// Construction
public:
	CPatternMaker();
	virtual ~CPatternMaker();


public:
	CStringArray m_ArrayFileChart;//�Ｚ ����� 
	void ReadOneLineForSamgungDemo(); //�Ｚ �����....
	void SetChegyulList(D48112_OSUB* pD48112_OSub);
	void SetCode(CString strCode);	
	void GetPMMDanga();
	void SetMAInfo(D10418_O* pD10418);
	void RecvRealJango(CString sMMDanga);
	void RecvRealCurPrice(CString sEnd);	
	void RecvRealHigh(CString sHigh);
	void RecvRealOpen(CString sOpen);
	void RecvRealLow(CString sLow);




	
	void Init_GuideLineValue();
	void DeleteLinePen();
	void CreateLinePen();
	void	OnDraw( CDC *p_pDC);

	//int		GetXPositoin(int nDataOffset, double dCandleWidth, CRect rctGraphRegion);	// X��ǥ�� ���Ѵ�
	int		GetXPositoin(double dDataOffset, double dCandleWidth, CRect rctGraphRegion);	// X��ǥ�� ���Ѵ�
		
	double	GetGradient(double dXPos1, double dYPos1, double dXPos2, double dYPos2);	// ������ ������ ���⸦ ���Ѵ�

	void	SetPatternEnvData(CPatternEnvData *pPatternEnvData);						// ���� ȯ�漳�� ������


	CArray <CHighLowPtData*, CHighLowPtData*>			m_aryHighLowPtData;			// �ֿ����, �ֿ����� ������
	CArray <CMAData*, CMAData*>							m_aryMAData;				// �̵���� ������
	CArray <CRegistNSupportData*, CRegistNSupportData*>	m_aryRegistNSupportData;	// ����, ������ ������
	CArray <CCheChartData*, CCheChartData*>				m_aryCheChartData;			// ��Ʈ�� �׷��� ü�� ������


	void	DeleteHighLowPtData();													// �ֿ����, �ֿ����� ������ ���� 
	void	DeleteMAData();															// �̵���� ������ ����
	void	DeleteRegistNSupportData();												// ����, ������ ������ ����
	void	DeleteCheChartData();													// ��Ʈ�� �׷��� ü�� ������ ����								

	BOOL	m_bSpecialChart;		// Special ��Ʈ �϶� �ǽð� ó�� ���Ѵ�.
	BOOL	m_bDrawChegyulLine;		// ü�� ���� �� ü�� ���� ǥ��.

	void SetHwndCurChatItem(HWND hWnd) { m_hwndChartItem = hWnd; }
	void SetHwndSelChatItem(HWND hWnd) { m_hSelChartItem = hWnd; }
	
	HWND GetHwndCurChatItem() { return m_hwndChartItem; }
	HWND GetHwndSelChatItem() { return m_hSelChartItem; }


	void SetBWonChart(BOOL bWonChart) { m_bWonChart = bWonChart; }
	BOOL GetBWonChart() { return m_bWonChart; }
	CString GetCode(){return m_strCode;};


protected:
	CString		m_strCode;				// �����ڵ�
	CPatternEnvData	*m_pPatternEnvData;	// ���� ȯ�漳�� ������	
	D10418_O	m_MAvgInfo;				// �̵���� ����
	CPen		m_pnHighLowLinePen;		// �ֿ���� ���� ������� ����
	CPen		m_pnMaPen;				// �̵���� ����
	CPen		m_pnHighLinePen;		// �ֿ���� ���� ����
	CPen		m_pnLowLinePen;			// �ֿ����� ���� ����
	
	BOOL		m_bLog;					// ��� Graph�� ��ϵ� Block�� Log ��Ʈ ���� �����̴�.
	BOOL		m_bReverse;				// ��� Graph�� ��ϵ� Block�� ������ ���� �����̴�.
	
	double		m_dViewMin;				// View���� ��� Graph�� View�� Minimun Scale ���̴�.
	double		m_dViewMax;				// View���� ��� Graph�� View�� Maximum Scale ���̴�.
	//=========================================================================================
	//���� ������ �ִ� ������ �������� ������ ���� Ŭ ��� �׸��� �ʴ´�.

	BOOL		m_bDraw2Pyung1;			// �̵���� 1��° �׸��� ����..
	BOOL		m_bDraw2Pyung2;			// �̵���� 2��° �׸��� ����..
	BOOL		m_bDraw2Pyung3;			// �̵���� 3��° �׸��� ����..
	BOOL		m_bDraw2Pyung4;			// �̵���� 4��° �׸��� ����..
	//=========================================================================================
	
	

	double m_dYSprice;	//���ϰ��� �ð�
	double m_dYHprice;	//���ϰ��� ��
	double m_dYLprice;	//���ϰ��� ����
	double m_dYEprice;	//���ϰ��� ����
	double m_dYSHL;		//���ϰ��� (��+��+��)/3
	double m_dTSprice;	//���ϰ��� �ð�
	double m_dTHprice;	//���ϰ��� ��
	double m_dTLprice;	//���ϰ��� ����
	double m_dTEprice;	//���ϰ��� ����
	double m_dTHL;		//���ϰ��� (��+��)/2
	double m_dTSHL;		//���ϰ��� (��+��+��)/3
	double m_dTHLE;		//���ϰ��� (��+��+(��*2))/4
	double m_dHighLimit;//���Ѱ�
	double m_dLowLimit;	//���Ѱ�
	double m_dPivot2Rst;	// �Ǻ� 2�� ����
	double m_dPivot1Rst;	// �Ǻ� 1�� ����
	double m_dPivotGLine;	// �Ǻ� ���ؼ�
	double m_dPivot1GG;	// �Ǻ� 1�� ����
	double m_dPivot2GG;	// �Ǻ� 2�� ����
	double m_dDMKPreHPrice;	// Demark �����
	double m_dDMKGLine;		// Demark ���ؼ�
	double m_dDMKPreLPrice;	// Demark ��������

	double m_dMAPeriod1; //�̵���� 1��° ��
	double m_dMAPeriod2; //�̵���� 2��° ��
	double m_dMAPeriod3; //�̵���� 3��° ��
	double m_dMAPeriod4; //�̵���� 4��° ��

	double m_dMASum1; //������ ������ �̵���� 1��° �հ� 
	double m_dMASum2; //������ ������ �̵���� 2��° �հ�
	double m_dMASum3; //������ ������ �̵���� 3��° �հ�
	double m_dMASum4; //������ ������ �̵���� 4��° �հ�

	double m_dPmmDanga ; //��ո��Դܰ�
	double m_dPmmPDanga; //�ܰ� + n%
	double m_dPmmMDanga; //�ܰ� - n%

	//==========================================================================
	CPen m_penYSprice;			//���ϰ��� �ð�				��
	CPen m_penYHprice;			//���ϰ��� ��				��
	CPen m_penYLprice;			//���ϰ��� ����				��
	CPen m_penYEprice;			//���ϰ��� ����				��
	CPen m_penYSHL;				//���ϰ��� (��+��+��)/3		��
	
	CPen m_penTSprice;			//���ϰ��� �ð�				��
	CPen m_penTHprice;			//���ϰ��� ��				��
	CPen m_penTLprice;			//���ϰ��� ����				��
	CPen m_penTEprice;			//���ϰ��� ����				��
	CPen m_penTHL;				//���ϰ��� (��+��)/2		��
	CPen m_penTSHL;				//���ϰ��� (��+��+��)/3		��
	CPen m_penTHLE;				//���ϰ��� (��+��+(��*2))/4 ��
	CPen m_penHighLimit;		//���Ѱ�					��
	CPen m_penLowLimit;			//���Ѱ�					��
	
	CPen m_penPivot2Rst;		// �Ǻ� 2�� ���� �� 
	CPen m_penPivot1Rst;		// �Ǻ� 1�� ���� ��
	CPen m_penPivotGLine;		// �Ǻ� ���ؼ�  ��
	CPen m_penPivot1GG;			// �Ǻ� 1�� ���� ��
	CPen m_penPivot2GG;			// �Ǻ� 2�� ���� ��
	
	CPen m_penDMKPreHPrice;		// Demark �����  ��
	CPen m_penDMKGLine;			// Demark ���ؼ�	��
	CPen m_penDMKPreLPrice;		// Demark ��������  ��
	
	CPen m_pen2Pyung1;			// �̵���� ù��° �Է°� ��
	CPen m_pen2Pyung2;			// �̵���� �ι�° �Է°� ��
	CPen m_pen2Pyung3;			// �̵���� ����° �Է°� ��
	CPen m_pen2Pyung4;			// �̵���� �׹�° �Է°� ��
	
	CPen m_penPmmDanga;			// ��ո��� �ܰ� ��
	CPen m_penPmmPDanga;		// ��ո��� �ܰ� +n% ��
	CPen m_penPmmMDanga;		// ��ո��� �ܰ� -n% ��

	CPen m_penChegyul;			// ü�� �ð�ǥ��	�� 
	CPen m_penSell;				// �ŵ� ü�� ǥ��	��
	CPen m_penBuy;				// �ż� ü�� ǥ��	��
	//==========================================================================
	
	HWND m_hwndChartItem;
	HWND m_hSelChartItem;


	BOOL m_bWonChart;

	IChartManager *m_pIChartManager;
	IChartOCX *m_pIChartOCX;
	IPacketManager *m_pIPacketManager;
	void RefreshChart();
public:
	void			SetIChartOCX( IChartManager *p_pIChartManager, IChartOCX *p_pIChartOCX, IPacketManager *p_pIPacketManager);
	//{{ ���(����) ó�� ���
	IAUTrCommManager*		m_pITrCommManager;
	ITrComm*		m_TRComm;		
	
	class CTrCommSite : public ITrCommSite
	{
	public:
		STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDataLen);
		STDMETHOD(ReceiveRealData) (WORD wFID, LPCSTR szMainKey, void* pRealData);
		STDMETHOD(StatusChange) (int nState);

	public:
		CPatternMaker* m_pTLMParent;
	} m_xTrCommSite;
	friend class CPatternMaker;
	//}}

	void RequestSiseData();
	void ReceiveSiseData(LPVOID	aTRData, long dwTRDataLen);

	CString	m_strRQ;		// ���ǰ� �ִ� RQ :�������� - ojtaso (20070307)
	void	SetRQ(LPCTSTR lpszRQ) {m_strRQ = lpszRQ; }
	LPCTSTR GetRQ() { return m_strRQ; };

// (2008/9/9 - Seung-Won, Bae) Request MA Data
protected:
	BYTE	m_nCodeGubun;
public:
	void RequestMaData( void);
	void ReceiveMaData( LPVOID aTRData, long dwTRDataLen);

// (2008/10/26 - Seung-Won, Bae) Draw DO/DH/DL/DC
protected:
	BOOL	m_bDrawOHLC;
	COLORREF	m_clrO;
	COLORREF	m_clrH;
	COLORREF	m_clrL;
	COLORREF	m_clrC;
public:
	void	SetDrawOHLC( BOOL p_bDrawOHLC, COLORREF p_clrUp, COLORREF p_clrDown);

// (2009/9/12 - Seung-Won, Bae) for OHLC title
protected:
	static CString	m_strTitleO;
	static CString	m_strTitleH;
	static CString	m_strTitleL;
	static CString	m_strTitleC;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATTERNMAKER_H__C8385516_C663_49EB_BC25_79CAEC4882FA__INCLUDED_)
