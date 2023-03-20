#include "stdafx.h"
#include "LCO.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLCO::CLCO(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
}

CLCO::~CLCO()
{

}

CString CLCO::MakeLegend(int nIndex)
{
	CString	ret;

	if (nIndex == 0)
		ret.Format("CO[%d,%d]", (int)m_pgraph->cInfo.awValue[0], (int)m_pgraph->cInfo.awValue[1]);
	else if (nIndex == 1)
		ret.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[2]);
	else
		ret = _T("");

	return ret;
}

void CLCO::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();
	CString	tmpstr;

	tmpstr.Format("CO[%d,%d]", (int)m_pgraph->cInfo.awValue[0], (int)m_pgraph->cInfo.awValue[1]);
	arTitle.Add(tmpstr);
	tmpstr.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[2]);
	arTitle.Add(tmpstr);
}
