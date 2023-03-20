#if !defined(AFX_INTERFACE_OF_EXTCODEWINDOW_MANAGER_BY_JUNOK_LEE__INCLUDED_)
#define AFX_INTERFACE_OF_EXTCODEWINDOW_MANAGER_BY_JUNOK_LEE__INCLUDED_

// #include "../../_include/IExtCodeWndMng.h"
// 

#include "IBaseDefine.h"

/* -------------------------------------------------------------------------
���� : ������ code.ocx���� �����ϴ� �ڵ���ȸâ�� ��ü�ϱ����� DLL �ֽ��ڵ��Է±�
������  : ���ؿ� 2006.01.06(��)
History : 2006.01.06 �������̽� ����
----------------------------------------------------------------------------
int nID = 27;
IExtCodeWndManager* pManager = (IExtCodeWndManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
IExtCodeWndManager* pManager = (IExtCodeWndManager*)AfxGetPctrInterface(nID);

------------------------------------------------------------------------- */
DECLARE_INTERFACE(IExtCodeWndManager)
{
	/* --------------------------------------------------- 
	-. ����[ShowCodeTable]
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(long, ShowCodeTable)(LPCTSTR szCodeKey, const VARIANT& hWnd) PURE;

	/* --------------------------------------------------- 
	-. ����[ShowModeCodeTable]���������ε� �ϴ� ��� ����.
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(long, ShowModeCodeTable)(LPCTSTR szCodeKey, const VARIANT& hWnd) PURE;

	/* --------------------------------------------------- 
	-. ����
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(void, SetPosition)(LPCTSTR szKey, short nX, short nY) PURE;

	/* --------------------------------------------------- 
	-. ����
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(long, ShowCodeTableWithPosition)(LPCTSTR szCodeKey, const VARIANT& hWnd, short nX, short nY) PURE;

	/**********************************************************************************
	-. �� �� : App �� �ִ� m_HistoryList �� �����͸� �߰��Ѵ�.
	-. �� �� 
	short nMaxCount  : History â�� ������ List �� ����
	long nType       : 28byte(CodeInput �� CodeInput �� m_szCodeChoice �� long ��)
	LPCTSTR Code     : Code (�� : 00660 )
	LPCTSTR CodeName : Code Name ( �� : ���̴н� )
	-. ���ϰ� : ������ history �� ������ FALSE, �ƴϸ� TRUE �� �����Ѵ�.
	***********************************************************************************/
	STDMETHOD_(BOOL, InsertHistory)(short nMaxCount, long nType, LPCTSTR Code, LPCTSTR CodeName) PURE;
	
	/**********************************************************************************
	-. �� �� : nType �� �´� History �����͸� �����Ѵ�.
	-. �� �� 
	   short nMaxCount  : History â�� ������ List �� ����
	   long nType       : 28byte(CodeInput �� CodeInput �� m_szCodeChoice �� long ��)
	-. ���ϰ�  : Ÿ�Կ� �´� �ڵ�/�ڵ���� �����͸� �����Ѵ�.
		���: "Code0=CodeName0;Code1=CodeName1;" �� �����Ѵ�.
	***********************************************************************************/
	STDMETHOD_(CString, GetHistoryList)(short nMaxCount, long nType) PURE;

	STDMETHOD_(void, CleanCodeTable)() PURE;

};

#endif //AFX_INTERFACE_OF_EXTCODEWINDOW_MANAGER_BY_JUNOK_LEE__INCLUDED_
