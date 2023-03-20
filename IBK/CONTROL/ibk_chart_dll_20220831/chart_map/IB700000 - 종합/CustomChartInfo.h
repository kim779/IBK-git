#ifndef __CHARTINFO_H__
#define __CHARTINFO_H__

#include "../../h/axisgenv.h"
#include "../../chart_dll/AxMInfo/AxChartInfo.h" 

class CI7000: public CAxChartInfo
{
public:
	CI7000(CWnd* pView, LPCTSTR lpszFileName):CAxChartInfo(pView, lpszFileName) {}
protected:
	virtual CString GetTitle() {return _T("주식종합차트");	}
};

class CI7100: public CAxChartInfo
{
public:
	CI7100(CWnd* pView, LPCTSTR lpszFileName): CAxChartInfo(pView, lpszFileName) {}
protected:
	virtual CString GetTitle() {return _T("업종종합차트");	}
	virtual void CustomEnvironment(struct _envInfo* envinfo) {envinfo->datainfo.btUnit = GU_INDEX;}
};

class CI7200: public CAxChartInfo
{
public:
	CI7200(CWnd* pView, LPCTSTR lpszFileName): CAxChartInfo(pView, lpszFileName) {}
protected:
	virtual CString GetTitle() {return _T("선물옵션종합차트");	}
	virtual void CustomEnvironment(struct _envInfo* envinfo) {	envinfo->datainfo.btUnit = GU_FUTURE;}
};

class CI7300: public CAxChartInfo
{
public:
	CI7300(CWnd* pView, LPCTSTR lpszFileName): CAxChartInfo(pView, lpszFileName) {}
protected:
	virtual CString GetTitle() {return _T("해외지수차트");	}
	virtual void CustomEnvironment(struct _envInfo* envinfo) 
	{
		envinfo->datainfo.btUnit = GU_FOREIGN; 
		envinfo->datainfo.wUpdate = DU_DUMMY;
	}
};

CAxChartInfo* CreateChartInfo(CWnd* pView, LPCTSTR mapName);


#endif // __CHARTINFO_H__




