#include "stdafx.h"
#include "C_Total.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* pParent, void* cInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LOG_OUTP(2, "c_total", __FUNCTION__);
	CC_TotalApp* pMapApp = (CC_TotalApp *)AfxGetApp();

	struct _param	*pParam = (struct _param *)cInfo;
	return pMapApp->Create(pParent, pParam);
}

