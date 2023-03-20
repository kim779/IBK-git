#include "stdafx.h"
#include "IB700000.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CIB700000App* pMapApp = (CIB700000App *)AfxGetApp();
	return pMapApp->Create(parent);
}


