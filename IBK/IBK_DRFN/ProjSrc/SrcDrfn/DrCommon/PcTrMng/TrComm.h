#ifndef __ALZIO__TRCOMM__HEADER__
#define	__ALZIO__TRCOMM__HEADER__

#include <afxmt.h>
#include "../../inc/IAUTrCommMng.h"
#include "../../inc/Packet.h"
#include "../../inc/IAUPartnerMng.h"

// ****************************************************************************
// ITRCommSite
// 
// 0 : Ready State
// 1 : �������� Send�ϱ� �ٷ� ��
// 2 : �������� Send�� �ٷ� ��
// 3 : �������κ��� ������
// 4 : ���Ͽ��� ������ ����
// ****************************************************************************
#define		ICBS_READY				0
#define		ICBS_SSENDBEFORE		1
#define		ICBS_SSENDAFTER			2
#define		ICBS_SREADY				3
#define		ICBS_SRECEIVE			4
#define		ICBS_TIMEOUT			-10
#define		ICBS_CREATEERR			-1
#define		ICBS_PARAMERR			-2

#define		WRET_READY				0
#define		WRET_RECEIVE			1
#define		WRET_TIMEOUT			2
#define		WRET_SENDERR			3
#define		WRET_QUIT				4

#define		RQ_FID					0
#define		RQ_TR					1

class CFIDOutputInfo
{
public:
	CFIDOutputInfo() {}

	CDWordArray	pItemLength;		// �����ۺ� ����
	CString		strOutputFID;		// ��ü Output FID : 3���� ������ ���� ����.
};

// ****************************************************************************
// CTRComm
// ****************************************************************************
class CTRComm : public CWnd
{
public:
	CTRComm()
	{
		m_nTRTimer=-1;
		m_hSocket = NULL;
		m_pCallback = NULL;
		m_nTRTimeout = 90;
		m_bEnc = FALSE;
		m_dwTextStyle = DT_EXPANDTABS|DT_EXTERNALLEADING|DT_NOPREFIX|DT_WORDBREAK;
		m_nWaitResult = WRET_READY;
		m_bCert = FALSE;
		m_nQueryCount = 0;
		m_nTRType = RQ_FID;	//0:FID 1:TR
		m_cDestination = ' ';
		m_nUniqueID = 1;
		m_nSendDebugID = 0;
		m_nRceiDebugID = 0;
		m_nMsgDebugID = 0;

		m_pTRData = NULL;
		m_nMaxTrSize = 0;
		m_pPtManager = NULL;
		m_pPartnerSite = NULL;
	}
	~CTRComm()
	{
		if(m_pTRData) free(m_pTRData);
		m_pTRData = NULL;

		DestroyWindow();
	}

	// Data Send
public:
	BOOL	Send2Server(void* szData, int nLen, int nKey);
	void	SetTR(LPCSTR szTR, BOOL bCert = FALSE);
	void	ChangeCallback(void*	pCallback) { m_pCallback=(ITrCommSite*)pCallback;	}

	BOOL	IsWaitTRRequest();

	//@solomon �߰� 20090928����ȣ
	BOOL	Send2ServerWithKey(void* szData, int nLen, int nKey); 

protected:
	int		m_nWaitResult;		// ������ ó������
	ITrCommSite*	m_pCallback;	// �ݹ�ó���� �������̽�
	HWND	m_hSocket;		// �����ڵ�
	int		m_nErrorCode;	// �����ڵ�
	CString	m_szTR;			// TR��ȣ
	int		m_nTRTimer;		// TimerID
	int		m_nTRTimeout;	// TimeOut����
	BOOL	m_bEnc;			// Data Encrytion
	BOOL	m_bCert;		// �������� ����
	
	CWnd	*m_pParent;
	DWORD	m_dwTextStyle;		// Extended Style

	void	StartTRTimer();
	void	StopTRTimer();

	BOOL	NotifyParent(LPCSTR szTRName, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen );
	int		WaitResult();

public:
	// �����κ��� ����Ÿ�� �޾�����쿡 Memory�� ��ȣ�ϱ����ؼ� Critical Session���� ��ȣ�Ѵ�.
	CCriticalSection	m_CommCritical;

	CString m_strFidOutList;		// ��¿� FID����Ʈ ������
	void	OutPutFid(CString strFidList);

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTRComm)
	public:
	virtual BOOL Create(CWnd* pParentWnd=NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CTRComm)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg long OnGetMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
//@Solomon090819	afx_msg long OnGetData(WPARAM wParam, LPARAM lParam);

public:

protected:
	int	m_nQueryCount;
	int m_nTRType;
	char	m_cDestination;
	CMapStringToString	m_DataTable;
	CMapStringToString	m_PacketID2TrCode;

public:
	void SetDestination(char cDestination){m_cDestination=cDestination;};
	void SetBaseData(LPCSTR szKey, LPCSTR szValue);
	void GetBaseData(LPCSTR szKey, CString &rValue);
	void SetBaseDataLong(LPCSTR szKey, LONG dwValue);

	long Send_MessageData(LPCSTR szTRCode, LPCSTR szMsgCode, LPCSTR szMsg);
	long Send_PacketData(BYTE* pPackData, int nSize, LPCSTR szTRCode, LPCSTR szMsgCode, LPCSTR szMsg);
	
	int	m_nSendDebugID, m_nRceiDebugID,m_nMsgDebugID, m_nUniqueID;
	int GetPacketUniqueID(BOOL bInCrease=TRUE); 

protected:
	UINT m_nMaxTrSize;
	BYTE* m_pTRData;

	LONG m_pPtManager;
	IPartnerSite	*m_pPartnerSite;

//	CCriticalSection	m_InSec;;

	int _DoPreCheck(LPBYTE szData, long nLen, int nKey, BOOL &bChk);
	int _DoPreCheck_WorkDate(LPBYTE szData, long nLen, LPCSTR szInputKey, BOOL &bChk);
	int _DoPreCheck_Dollar(LPBYTE szData, long nLen, LPCSTR szInputKey, BOOL &bChk);
	int _DoPreCheck_StockHoga(LPBYTE szData, long nLen, LPCSTR szInputKey, BOOL &bChk);
	int _DoPreCheck_MstCode(LPBYTE szData, long nLen, LPCSTR szInputKey, BOOL &bChk);
};

#endif // __ALZIO__TRCOMM__HEADER__
