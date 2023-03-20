#if !defined(AFX_HANARO_IRUNSTATEMANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_HANARO_IRUNSTATEMANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_

// #include "../../_Include/IRunStateMng.h"
#include "IBaseDefine.h"

//  int nID = 38;
// 	IRunStateManager* pMng = (IRunStateManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IRunStateManager* pMng = (IRunStateManager*)AfxGetPctrInterface(nID);

// ������  : ���ؿ� 2005.11.14(��)
// History : 2005.11.14 �������̽�
// PCTR�� ������¾˷��ֱ� ���

// ex... ���� �����͸� ���� �� �ִ� ������ Call...
// #include "../../_Include/IRunStateMng.h"
// IRunStateManager* pMng = (IRunStateManager*)AfxGetPctrInterface(38);
// pMng->NotifyState("GWANSIM", 1, NULL);

#define  MaxRunState			5

enum
{
	RunState_End=0,
	RunState_Start,
	RunState_Run,
};

enum 
{
	RunModule_GwanLib=0,
	RunModule_AutoTrade,
};

#define		RunState_Gwansim		"GWANSIM"
#define     RunState_AUTOTRADE		"AUTOTRADEMONITOR"

DECLARE_INTERFACE(IRunStateManager)
{
	// szKey ���ɸ��
	// "MAIN"
	// "GWANSIM"
	// dwState  1 : Start
	//          0 : End
	// dwStateData �ʿ��� �߰������� �ѱ���.
	// Return : �ش� ó���� �ϸ�  TRUE, �ƴϸ� FALSE
	STDMETHOD_(BOOL, NotifyState)(LPCSTR szKey, long dwState, long dwStateData=NULL) PURE;
};

#endif // AFX_HANARO_IRUNSTATEMANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_


