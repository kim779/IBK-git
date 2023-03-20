#ifndef __AXCHARTINFO_H__
#define __AXCHARTINFO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../h/axisgenv.h"

class AFX_EXT_CLASS CAxChartInfo
{
public:
	CAxChartInfo(CWnd* pView, LPCTSTR lpszMapName);
	virtual ~CAxChartInfo();

	void	Initialize();
protected:
	CWnd*	m_view;
	CString	m_mapName;
	CString m_gexDir;

	WORD	m_default_rgnCount;
	WORD	m_default_graphCount;

	BOOL	m_noRestoreGraph;

	virtual void	RegionSetting(LPSTR buffer);
	virtual CString MakeCaption() 
	{
		CString sTitle;
		sTitle.Format("[%s] %s", m_mapName.Mid(2, 4), GetTitle());
		return sTitle;
	}
protected:
	virtual CString GetTitle() = 0;
	virtual void CustomEnvironment(struct _envInfo* envInfo) {}
	virtual void CustomGraphInfo(LPSTR buffer) {}

	void	CheckData(LPCTSTR lspzRoot, LPCTSTR lpszName);

	virtual void	MakeData(LPCTSTR lpszFileName);
	virtual void	MakeEnvironment(LPCTSTR lpszFileName);

	virtual void	DefaultCountInfo();
	virtual void	MakeGraphInfo(LPCTSTR lpszFileName);
	virtual void	AdditionalGraphInfo(struct _graph* graph){}

	virtual void	InitInfo(struct _cntInfo *cntInfo, LPSTR pData);

	BOOL	MakeFile();
protected:
	_basicInfo* GetChartBasicInfo(WORD graphKind);

	void	SetChartOption(_graph* graph, int index, int value, COLORREF color = DEFCR1, int width = 1, UINT style = PS_SOLID);
	void	SetTickOption(_graph* graph, int index, float value, int fillOverStyle = 0, COLORREF color = DEFCR1, int width = 1, UINT style = PS_SOLID);
};


#endif // __AXCHARTINFO_H__