#if !defined(AFX_INTERFACE_OF_MAINRUNINFO_1464_45BB_ACC8_93B923698B1A__INCLUDED_)
#define AFX_INTERFACE_OF_MAINRUNINFO_1464_45BB_ACC8_93B923698B1A__INCLUDED_

// #include "../../_include/IMainRunInfoMng.h"
#include "IBaseDefine.h"

DECLARE_INTERFACE_(IMainRunInfoManager, IBaseDataDB)
{
	// Root���丮 (bin���丮�� ���ܰ� Dir)
	// I-PARAM : -
	// O-PARAM : Root���丮
	STDMETHOD_(CString, GetRootDir)() PURE;

	// Data���丮
	STDMETHOD_(CString, GetDataDir)() PURE;

	// UserID�� �Ҵ�� �������丮
	// I-PARAM : -
	// O-PARAM : UserID�� �Ҵ�� �������丮
	STDMETHOD_(CString, GetEncUserDir)() PURE;
};

#define		IMRIM_ROOTDIR			"A12000"
#define		IMRIM_DATADIR			"A12002"
#define		IMRIM_ENCUSERDIR		"A12007"

#endif //AFX_INTERFACE_OF_MAINRUNINFO_1464_45BB_ACC8_93B923698B1A__INCLUDED_
