#if !defined(AFX_REPLAYCHARTWND_H__FE2A2AD6_B7A8_4A1A_BB18_B32EB2048441__INCLUDED_)
#define AFX_REPLAYCHARTWND_H__FE2A2AD6_B7A8_4A1A_BB18_B32EB2048441__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReplayChartWnd.h : header file
//

#include <afxtempl.h>

#include "../chart_common/ReplayChartInfoDef.h"
#include "../../Inc/IAuTrCommMng.h"
#include "../../Inc/IAuPartnerMng.h"
#include "../../Inc/IGateMng.h"

#include "../commontr/TRDefine.h"

#include "ReceivedTRData.h"

class CDlgRecvTRProgress;

/////////////////////////////////////////////////////////////////////////////
// CReplayChartWnd window
#define IMPLCTRLCLASS CReplayChartWnd

#define MYMETHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = \
	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
	pThis;

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 10 / 18
// Explain		: �ð�����
// Using way	: 
//-----------------------------------------------------------------------------
typedef struct _TIME_INFO
{
	int m_nDD;	// ��¥ ����
	int m_nHH;	// �ð� ����
	int m_nMM;	// �� ����
	int m_nSS;	// �� ����
} TIME_INFO;

typedef struct {
	BOOL m_bRequested;
	int m_nChartCount;
} STTRINFO;

class CReceivedTRData;
class CReceivedEachTR;

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 17
// Comments		: 
// Using way	: 
// See			: ����� Dll
//-----------------------------------------------------------------------------
class CReceivedTRData;
class CReplayChartWnd : public CWnd
{
// Construction
public:
	CReplayChartWnd();

// Attributes
public:
	HWND		m_hMainFrame;
	HWND		m_hParent;

	CDlgRecvTRProgress *m_pDlgRecvTRProgress;
	CReceivedEachTR		m_rcvTrData;

	int			m_nRemainTRCount;			// Receive�ؾ� �Ǵ� TR�� ����
	int			m_nReplaySpeed;				// ���⸦ �� �ӵ�
	int			m_nReplayDayCount;			// ���⸦ �� ��¥��.

	BOOL		m_bSendCurTime;			// MultiChart�� �ð��� �����ߴ��� üũ
	TIME_INFO	m_StartTime;			// ���� �ð� ����
	TIME_INFO	m_CurTime;			// ���� �������� �ð� ����
	TIME_INFO	m_MarketStartTime[3];	// ���� �����ϴ� �ð� ����
	TIME_INFO	m_MarketEndTime[3];	// ���� �����ϴ� �ð� ����

	float		m_fTimeInterval;			// ��ӿ� ���� �����ð� ����

	UINT		m_unTimerHandle;			// Timer Handle
	UINT		m_unTRWaitTimerHandle;		// Server���� TR�� �������� ��ٸ��� Timer Handle
	CString		m_szMarketID;			// 0 : �ֽ�(s) , 2 : ����(f)
	
	BOOL		m_bReceiveTR;				// Server�� TR�� �����ϰ� ���� FALSE, ������ TRUE;
	int			m_nRcvDate;
	TIME_INFO	m_sendTime;
	int			m_nTotalTime;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReplayChartWnd)
	//}}AFX_VIRTUAL

	
// Implementation
public:
	virtual		~CReplayChartWnd();

	// About Create
	void		Init(void *pReplayInfo);
	void		GetReplayInfo(REPLAY_INFO *pRecvReplayInfo);			// MultiChart�� ���� ���� ���⿡ �ʿ��� ������ �����Ѵ�.

	// ������ TR������ ��û�Ѵ�.
	void		SendTRList2Server(CString strTime);

	// TR ��ȸ ����� �޴´�.
	HRESULT		ReceiveData(LPVOID	aTRData, long dwTRDateLen);
	void		SendTickData2MultiChart();

	BOOL		Run(int nMode);				// 0 : Stop, 1 : Play, 2 : Pause
	void		ChangeSpeed(int nSpeed);

	// ���� ��ü �ð��� ����Ѵ�.
	int			GetTotalTime();

	void		IncreaseTime();			// �ð��� ������Ų��.
	void		SendCurTime();			// MultiChart�� ���� �ð��� �����Ѵ�.
	void		MoveTickIdx();			// ���� �����Ͽ� �°� TickIndex�� �����Ѵ�.

	void		SendToChart();

	int			CalcCurTime(int nEndTime);
	int			CalcTotalTime();

	enum	{STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, ELW_CHART, FOREIGN_CHART};
	
// Generated message map functions
protected:
	//{{AFX_MSG(CReplayChartWnd)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	LONG OnRmsgReceiveReplayData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPLAYCHARTWND_H__FE2A2AD6_B7A8_4A1A_BB18_B32EB2048441__INCLUDED_)

