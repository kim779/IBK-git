#if !defined(AFX_INTERFACE_OF_CHARTUTIL_MANAGER_BY_JUNOK_LEE__INCLUDED_)
#define AFX_INTERFACE_OF_CHARTUTIL_MANAGER_BY_JUNOK_LEE__INCLUDED_

// #include "../../_include/IAUChartUtilMng.h"
// 

#include "IBaseDefine.h"
#include <AFXTEMPL.H>

#define		UUID_IAUChartUtilManager				44

// ��뿹�� �� ������ ���� �ϴ� �κ��� �ڸ�Ʈ�� ����.
//
//
/* -------------------------------------------------------------------------
���� : ��Ʈ���� ����� ���� ��� ����Ʈ�������� ����Ʈ������
������  : ���ؿ� 2007.12.10(��)
History : 2007.12.10 �������̽� ����
----------------------------------------------------------------------------
int nID = UUID_IChartUtilManager;
IChartUtilManager* pManager = (IChartUtilManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
IChartUtilManager* pManager = (IChartUtilManager*)AfxGetPctrInterface(nID);

------------------------------------------------------------------------- */

#define		ID_IAUChartUtilManager_Max				3
#define		ID_IAUChartUtilManager_FloatingDlg		1

// ��ġ��ȸâ Ŭ���̾�Ʈ �������̽�
DECLARE_INTERFACE(IChartFloatClient)
{
	// 0:Hide, 1:Show
	STDMETHOD_(void, StateChanged)(UINT nState) PURE;
};

// ��ġ��ȸâ �������̽�
DECLARE_INTERFACE(IChartFloatDlg)
{
	STDMETHOD_(LONG, AdviseFloatDlg)(IChartFloatClient* pIClient) PURE;
	STDMETHOD_(void, UnAdviseFloatDlg)(IChartFloatClient* pIClient) PURE;

	// Floating�ü�â Show/Hide
	// ������ �ε��Ǿ� ���� ������ �ε��Ѵ�.
	// nShow 0: Hide
	//		 1 : Show
	//		 2 : Toggle
	STDMETHOD_(void, ShowFloatDlg)(IChartFloatClient* pIClient, int nShow=2) PURE;

	// �������� ���
	// 0:Hide, 1:Show
	STDMETHOD_(int, GetFloatSiseState)() PURE;

	// ���콺 �̵��� �ü������͸� �Ѱ��ش�.
	STDMETHOD_(void, SetDataWindowData)(CList<CString, CString&>* pListData) PURE;
};

DECLARE_INTERFACE(IAUChartUtilManager)
{
	STDMETHOD_(IChartFloatDlg*, AdviseChartFloatDlg)() PURE;
	STDMETHOD_(void, UnAdviseChartFloatDlg)(IChartFloatDlg*) PURE;
};

#endif //AFX_INTERFACE_OF_CHARTUTIL_MANAGER_BY_JUNOK_LEE__INCLUDED_
