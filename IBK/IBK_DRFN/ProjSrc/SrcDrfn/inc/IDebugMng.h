#if !defined(AFX_INTERFACE_OF_DEBUGMANAGER_BY_JUNOK_LEE__INCLUDED_)
#define AFX_INTERFACE_OF_DEBUGMANAGER_BY_JUNOK_LEE__INCLUDED_

#include "./IBaseDefine.h"
#include "./IDebugData.h"
#include "./LoadDRDebuger.h"

#define		UUID_IDebugManager					14		// IBaseDefine.h�� ���ǵǾ� ����..

#define		IDebugManagerLast					IDebugManager3

//#define		UUID_IDebugManager2					14

/* -------------------------------------------------------------------------
���� : ������� �����ϱ� ���� �������̽�
���ù��� : ����_�����.doc
������  : ���ؿ� 2005.12.07(��)
History : 
----------------------------------------------------------------------------

int nID = 14;
IDebugManager* pManager = (IDebugManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
IDebugManager* pManager = (IDebugManager*)AfxGetPctrInterface(nID);

IDebugManager* pIDebugMng = (IDebugManager*)AfxGetPctrInterface(UUID_IDebugManager);
if(pIDebugMng)
{
	CString szMsg;
	szMsg.Format("UP/DOWN:%d, ����:%d, ���:%d\n", m_nUpDown, m_ptATableOffset, m_ptRTableOffset.y);
	pIDebugMng->Trace(szMsg);
}
------------------------------------------------------------------------- */

DECLARE_INTERFACE(IDebugManager)
{
	/* --------------------------------------------------- 
	-. ����[Trace] ListBox�� �α����
	-. I-PARAM : Format(..)����, �ִ���۴� 1024ũ��
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(void, Trace)(LPCTSTR lpszFormat, ...) PURE;
	STDMETHOD_(void, Trace)(int nScrKey, LPCTSTR lpszFormat, ...) PURE;
	
	STDMETHOD_(void, WriteLogFile)(void* sData, int nDataLen, LPCSTR szFileName, BOOL bInsertCR=TRUE, LPCSTR szLogDir="c:\\Logs") PURE;
	STDMETHOD_(void, TraceEx)(BYTE cMessageTag, BYTE cIO, LPCSTR pDataName, DWORD lDataSize, LPBYTE pData) PURE;

	//cIO 'I':Input, 'O' Output
	//pTranName�� �ִ� 30�ڸ�
	STDMETHOD_(void, TraceTran)(BYTE cIO, LPCSTR pTranName, DWORD dwSize, LPBYTE pData) PURE;
	STDMETHOD_(void, TraceReal)(LPCSTR pRealName, DWORD dwSize, LPBYTE pData) PURE;

	//cAU 'A':Advise, 'U':UnAdvise
	//cIO 'I':Input, 'O' Output
	//pTranName�� �ִ� 30�ڸ�
	//hReceiver : WM_COPYDATA���� ������
	//pOnCopyDataKey : WM_COPYDATA������ pCopyDataStruct->dwData
	//QCMonitor�� ���� �����͸� �ް��� �� �� ����ϴ� ���.
	STDMETHOD_(void, TraceSR)(BYTE cAU, BYTE cIO, LPCSTR pTranName, HWND hReceiver, WORD dwOnCopyDataKey) PURE;

	STDMETHOD_(void, ShowOutputDebugString)(LPCTSTR lpszFormat, ...) PURE;
};

//IDebugManager2�� //Data ��Tool�� ���� ���
DECLARE_INTERFACE_(IDebugManager2, IDebugManager)
{
	STDMETHOD_(BOOL, TraceCompData)(BYTE cIO, LPCSTR pTranName, DWORD dwSize, LPBYTE pData) PURE;
};

// DRDebuger.dll ����
DECLARE_INTERFACE_(IDebugManager3, IDebugManager2)
{
	STDMETHOD_(LONG, GetDRDebuger()) PURE;
};

// #define DRMONITOR_CLASSNAME		_T("DrMonitorClass")	//���������� Ŭ������.
// #define	gDebugWindow			"DrMonitor"				//exe�� �������ϸ�. 
// UINT m_uRMSG_HandleToDebug = ::RegisterWindowMessage("RMSG_HandleToDebug");

typedef struct
{
	BYTE	cTag;			
	DWORD	dwDataSize;
	LPBYTE  dwDataBuff;	//�������� ������ ����.
}STDebugMngTraceEx;


#endif //AFX_INTERFACE_OF_DEBUGMANAGER_BY_JUNOK_LEE__INCLUDED_
