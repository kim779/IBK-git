#if !defined(AFX_INTERFACE_OF_TRAGENT_INCLUDED)
#define AFX_INTERFACE_OF_TRAGENT_INCLUDED

#include "AutoUpdateIO.h"
// ���� : 
// CTRAgentDll ���� ����Ÿ�� ������ CTAupBaseDlg�� ��ӹ޴� ȭ�� DLL�� �Լ��� ȣ���ϱ� ���� ����
// CTAupBaseDlg �� �Լ��� virtual �� ����Ǿ� �ڽ� Class���� ���� �ڵ��� �����ϴ�.
//

DECLARE_INTERFACE(ITrAgentSite)
{
	STDMETHOD(ReceiveQueryData) (WORD wQAIndex, LPVOID lpHederInfo, LPVOID lpTRData, long dwTRDateLen) PURE;
	STDMETHOD(ReceiveRealData) (WORD wQAIndex, CAutoToTR * lpAutoToTr) PURE;
};
typedef  ITrAgentSite FAR * LPITrAgentSite;

#endif // AFX_INTERFACE_OF_TRAGENT_INCLUDED