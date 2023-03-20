#if !defined(AFX_INTERFACE_OF_SYSTEMUTIL_1464_45BB_BY_JUNOK_LEE__INCLUDED_)
#define AFX_INTERFACE_OF_SYSTEMUTIL_1464_45BB_BY_JUNOK_LEE__INCLUDED_

// #include "../../_include/ISystemUtilMng.h"
#include "IBaseDefine.h"

#define		UUID_ISystemUtilManager			42

//  int nID = 24;
// 	ISystemUtilManager* pManager = (ISystemUtilManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	ISystemUtilManager* pManager = (ISystemUtilManager*)AfxGetPctrInterface(nID);

DECLARE_INTERFACE(ISystemUtilManager)
{
	// OCX�� ��ϵ� ����(TRUE/FALSE)�� �����Ѵ�.
	// ��ϵ� OCX�� DLL�� ��� clsid ���� ä���� �������ش�.
	// ��.	MS��Ʈ�� szProgID = "MSChart20Lib.MSChart.2";
	//		TeeChart�� szProgID = "TeeChart.ChartPageNavigator.7";
	STDMETHOD_(BOOL, IsRegistered)(LPCSTR szProgID, CLSID &clsid) PURE;

	// OCX�� DLL�� ����Ѵ�.
	// ���ϰ�	0 : ����
	//			1 : DLLRegister OleInitialize ����
	//			2 : LoadLibraryEx ����
	//			3 : GetProcAddress(DllRegisterServer) ����
	//			4 : DllRegisterServer ����
	STDMETHOD_(int, DLLRegister)(LPCSTR szFileName) PURE;
};

DECLARE_INTERFACE_(ISystemUtilManager2, ISystemUtilManager)
{
	// ������ ���翩�ε� üũ�Ѵ�.
	STDMETHOD_(BOOL, IsRegisteredEx)(LPCSTR szProgID, CLSID &clsid, LPCSTR szOcxName, LPCSTR szFullPath=NULL) PURE;
};


#endif //AFX_INTERFACE_OF_SYSTEMUTIL_1464_45BB_BY_JUNOK_LEE__INCLUDED_
