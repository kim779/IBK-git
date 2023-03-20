#include "stdafx.h"
#include "CustomChartInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CAxChartInfo* CreateChartInfo(CWnd* pView, LPCTSTR dllName) 
{
	CString map(dllName);

	switch (atoi(map.Mid(2, 4))) {
		case 7000: return new CI7000(pView, dllName);
		case 7100: return new CI7100(pView, dllName);
		case 7200: return new CI7200(pView, dllName);
		case 7300: return new CI7300(pView, dllName);
		default:   return NULL;
	}
}
