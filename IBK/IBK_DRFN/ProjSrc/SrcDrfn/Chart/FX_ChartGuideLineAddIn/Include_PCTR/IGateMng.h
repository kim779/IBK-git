#if !defined(AFX_HANARO_IGATENANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_HANARO_IGATENANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_

// #include "../../_Include/IGateMng.h"
#include "IBaseDefine.h"

//  int nID = 0;
// 	IGateManager* pGateMng = (IGateManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IGateManager* pMng = (IGateManager*)AfxGetPctrInterface(nID);

// ������  : ���ؿ� 2005.11.14(��)
// History : 2005.11.14 �������̽�
//

DECLARE_INTERFACE(IGateManager)
{
	// Interface������ ���ϱ�
	// I-PARAM : nID : �������̽� ����ID
	//           dwUnUse : ������� ����.
	// O-PARAM : �ش� Interface* ������
	STDMETHOD_(LONG, GetInterface)(int nID, long dwUnUse=0) PURE;
};

#endif //AFX_HANARO_INTERFACE_H__320E_1464_BY_JUNOK_LEE_98B1A__INCLUDED_
