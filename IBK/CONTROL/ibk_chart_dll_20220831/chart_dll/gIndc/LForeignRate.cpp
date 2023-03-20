// LForeignRate.cpp: implementation of the CLForeignRate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LForeignRate.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLForeignRate::CLForeignRate(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_strDspUnit = "";
	m_iValidDigit = 0;
	if (m_iUnit == GU_INDEX)
	{
		switch (m_pgraph->wGKind) 
		{
		case GK_FRGNHAVE:	// �ܱ��κ���(����) 
			break;
		case GK_PSNLTBUY:	// ���δ������ż�(����)
		case GK_FRGNTBUY:	// ���δ������ż�(����)
		case GK_ORGNTBUY:	// ����������ż�(����)
			m_strDspUnit = "õ";
			break;
		case GK_FRGNRATE:	// �ܱ��κ���(����)
			m_strDspUnit = "%";
			m_iValidDigit = 2;
			break;
		default:
			break;
		}
	}
	else // if (m_iUnit == GU_CODE)
	{
		switch (m_pgraph->wGKind) 
		{
		case GK_FRGNHAVE:	// �ܱ��κ���(����) 
			m_strDspUnit = "õ";
			[[fallthrough]];
		case GK_PSNLTBUY:	// ���δ������ż�(����)
		case GK_FRGNTBUY:	// ���δ������ż�(����)
		case GK_ORGNTBUY:	// ����������ż�(����)
			break;
		case GK_FRGNRATE:	// �ܱ��κ���(����)
			m_strDspUnit = "%";
			m_iValidDigit = 2;
			break;
		default:
			break;
		}
	}
}

CLForeignRate::~CLForeignRate()
{

}

CString CLForeignRate::MakeLegend(int nIndex)
{
	CString	ret = "";
	switch (m_pgraph->wGKind) {
		case GK_FRGNHAVE: 
			ret = "�ܱ��κ���(����)";
			break;
		case GK_PSNLTBUY:
			ret = "���δ������ż�(����)";
			break;
		case GK_FRGNTBUY:
			ret = "�ܱ��δ������ż�(����)";
			break;
		case GK_ORGNTBUY:
			ret = "����������ż�(����)";
			break;
		default:
			ret = _T("�ܱ��κ���(����)");
			break;
	}

	return ret;
}

void CLForeignRate::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();

	CString	ret;

	switch (m_pgraph->wGKind) {
		case GK_FRGNHAVE: 
			ret = "�ܱ��κ���(����)";
			break;
		case GK_PSNLTBUY:
			ret = "���δ������ż�(����)";
			break;
		case GK_FRGNTBUY:
			ret = "�ܱ��δ������ż�(����)";
			break;
		case GK_ORGNTBUY:
			ret = "����������ż�(����)";
			break;
		default:
			ret = _T("�ܱ��κ���(����)");
			break;
	}

	arTitle.Add(ret);
}

CString CLForeignRate::GetDisplayPosData(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");

	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData) || pAppData->GetDataCount() <= 0)
		return "";

	int DrawWidth = m_rectGrp.Width();
	int xPosition = pt.x - m_rectGrp.left;

	double szOneDay = double(DrawWidth) / double(m_iDispDtCnt);
	int dataPos = int(double(xPosition) / szOneDay);
	dataPos += m_iDispSPos;

	if (dataPos < 0 || dataPos >= m_iTtlDtCnt)
		return _T("");

	CString	unitStr = _T(""), tmpS, tmpS2;
	double	val = 0.0;
	CStringArray	arTitle;

	arTitle.RemoveAll();
	MakeTitle(arTitle);	

	int	nCount = pAppData->GetDataCount();
	
	char	split = ':';
	if (!bGrid)	split = '\t';
	for ( int ii = 0 ; ii < nCount ; ii++ )
	{
		val = pAppData->GetVal(ii, dataPos);

		if (val == GD_UNUSED)
			continue;

		CIndcBase::FormatVariableComma(tmpS, val, 1, 100);
		tmpS2.Format("%s%c%s", LPCTSTR(arTitle.GetAt(ii)), split, LPCTSTR(tmpS));

		if (!unitStr.IsEmpty())
			unitStr += "\t" + tmpS2;
		else
			unitStr = tmpS2;
	}

	unitStr += m_strDspUnit;

	return unitStr;
}
