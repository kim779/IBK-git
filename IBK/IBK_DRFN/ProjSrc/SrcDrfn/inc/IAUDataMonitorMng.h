#if !defined(AFX_INTERFACE_OF_DATAMONITORMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_DATAMONITORMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_

// #include "../../_Include/IAUDataMonitorMng.h"
#include "IBaseDefine.h"

#define		UUID_IAUDataMonitorManager		28

//**************************************************************************************
//�����ͺ���ó���� ���� �������̽� ��뼳��.
//
//#include "../../_Include/IAUDataMonitorMng.h"
//
//���� 1. ����Ű�� : "�ֽ�_�ֹ�����"
//     2. 
//
//CString szKey = "�ֽ�_�ֹ�����";
//CString szSubKey = "A����";
//long    dwSubKey = (long)(LPCSTR)szSubKey;
//long    dwNotifyData = xx; // ���⿡ ����� Sturct���� ������ �� �ִ�.
//
//@@@@@ ������ ����ó���� �ϴ� Server���� ó��������� �κ�.
//int nID = 28;
//IAUDataMonitorManager* pMng = (IAUDataMonitorManager*)AfxGetPctrInterface(nID);
//pMng->NofityData(szKey, dwSubKey, dwNotifyData);
//
//
//@@@@@ ������ ����ó���� �ؾ��ϴ� Client���� ó������� �� �κ�.
//Header���� 
//	Class�� �����������
//	static void CALLBACK DataMonitorCallbackProc(LPCSTR szKey, long dwKey, long dwSubKey, long dwNotifyData);
//
//	long  m_dwDataMonitor;
//
//Cpp����
//	// OnInitialUpdate���
//	int nID = 28;
//	IAUDataMonitorManager* pMng = (IAUDataMonitorManager*)AfxGetPctrInterface(nID);
//	m_dwDataMonitor = pMng->Advise(szKey, dwKey, DataMonitorCallbackProc);
//
//	// Destroy���� 
//	pMng->UnAdvise(m_dwDataMonitor);
//
//	void CMyClass::DataMonitorCallbackProc(LPCSTR szKey, long dwKey, long dwSubKey, long dwNotifyData)
//	{
//		if(szKey=="�ֽ�_�ֹ�����")
//		{
//			// ���⼭ �ʿ��� ó���� �Ѵ�.
//		}
//	}
//**************************************************************************************


//  int nID = 28;
// 	IAUDataMonitorManager* pMng = (IAUDataMonitorManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUDataMonitorManager* pMng = (IAUDataMonitorManager*)AfxGetPctrInterface(nID);

// ���� : ��׶���� ������ �ʿ��� ����� �����ϴ� �Ŵ���
//
// ������  : ���ؿ� 2005.12.03(��)
//           ������ 3�ܰ��Ģ
//           1�ܰ� : Ȯ���� �ϰ�
//           2�ܰ� : �̹� �̷���� ���� �ٶ󺸰�
//           3�ܰ� : ���� �����϶�
// History : 2005.12.03 �������̽� ����
//


#define DataMon_SZGWANSIM		"��������"


typedef void (CALLBACK* DataMonitorCallbackProc)(LPCSTR szKey, long pThisPoint, long dwKey, long dwSubKey, long dwNotifyData);
// static void CALLBACK DataMonitorCallbackProc(LPCSTR szKey, long pThisPoint, long dwKey, long dwSubKey, long dwNotifyData);

DECLARE_INTERFACE(IAUDataMonitorManager)
{
	// I-PARAM : szKey ������Ű��.
	//           dwKey �ݹ���� �� �ǵ��� �޴� ��.. ���������� ����� �� �ֵ��� �����ϴ� ����.
	//                 �ʿ������ NULL
	//           pCallFunc ����� �� �Լ��� �Ҹ���.
	// O-PARAM : AdviseŰ��. �� Ű���� ����ߴٰ� UnAdive��� ���.
	STDMETHOD_(long, Advise)(LPCSTR szKey, long pThisPoint, long dwKey, DataMonitorCallbackProc pCallFunc) PURE;

	// I-PARAM : Advise���� ���Ϲ��� Ű��.
	// O-PARAM : 
	STDMETHOD_(void, UnAdvise)(long dwRet) PURE;

	// I-PARAM : szKey ������Ű��.
	//           szSubKey ������Ű���ܿ� ����Ű��. �ʿ��� ����� �����ؼ� ���..
	//           dwData �����Ϳ� �ش��ϴ� �������̽�
	// O-PARAM : 
	STDMETHOD_(void, NotifyData)(LPCSTR szKey, long dwSubKey, long dwNotifyData) PURE;
};

#endif // AFX_INTERFACE_OF_DATAMONITORMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_
