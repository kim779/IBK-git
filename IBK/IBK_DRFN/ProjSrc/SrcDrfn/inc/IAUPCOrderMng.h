#if !defined(AFX_INTERFACE_OF_ORDER_BY_PC_MANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_ORDER_BY_PC_MANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_

// #include "../../_Include/IAUPCOrderMng.h"
#include "IBaseDefine.h"

//  int nID = 25;
// 	IAUPCOrderManager* pMng = (IAUPCOrderManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUPCOrderManager* pMng = (IAUPCOrderManager*)AfxGetPctrInterface(nID);

// ���� : PC�ֹ�(�����ֹ�/�ڵ��ֹ�) �Ŵ���
//
// ������  : ������ �ִ� ��� ���ؿ� 2005.12.03(��)
// ������ �� : ���� �׻� �� �տ� �ִ�. ���� �� ���� �̷�� ���� ���õ� ��ȹ�� ���� �ൿ�Ѵ�.
// History : 2005.12.03 �������̽� ����
//

DECLARE_INTERFACE(IAUPCOrderManager)
{
	// ��   �� : 
	// I-PARAM : szKey ����Ű
	// O-PARAM : TRUE/FALSE
	// STDMETHOD_(BOOL, IsBackGround)(LPCSTR szKey) PURE;
};


#endif // AFX_INTERFACE_OF_ORDER_BY_PC_MANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_
