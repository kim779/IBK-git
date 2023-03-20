#if !defined(AFX_INTERFACE_OF_USERINFO_1464_45BB_ACC8_93B923698B1A__INCLUDED_)
#define AFX_INTERFACE_OF_USERINFO_1464_45BB_ACC8_93B923698B1A__INCLUDED_

// #include "../../_include/IUserInfoMng.h"
#include "IBaseDefine.h"

DECLARE_INTERFACE_(IUserInfoManager, IBaseDataString)
{
	// '����ھ��̵�' ����
	// I-PARAM : -
	// O-PARAM : �������̵�
	STDMETHOD_(CString, GetUserID)() PURE;

	// '��ȣȭ�� ����ھ��̵�' ����
	// I-PARAM : -
	// O-PARAM : ��ȣȭ�� �������̵�
	STDMETHOD_(CString, GetEncUserID)() PURE;
};

#define		USERINFOMNG_USERID			"A10000"
#define		USERINFOMNG_USERENCID		"A10001"

#endif //AFX_INTERFACE_OF_USERINFO_1464_45BB_ACC8_93B923698B1A__INCLUDED_
