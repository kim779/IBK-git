// ChartItemStock.h: interface for the CChartItemStock class.
//
// ��Ʈ �ֽ�, ����ü <-> FID ������ Ŭ����
// 2009.10.06 �ַθ� Ưȭ, ���н� �÷���
// by Jeoyoho
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTITEMSTOCK_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
#define AFX_CHARTITEMSTOCK_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../chart/commontr/KB_p0602.h"
//@Solomon:091123SM068
#include "../../inc/IChartBozoMsgMng.h"

class CChartItemStock  
{
public:
	CChartItemStock();
	virtual ~CChartItemStock();
	void SetInData(KB_p0602_InRec1* pData);

	int		m_nTickCnt, m_nSecCnt, m_nMinCnt, m_nDayCnt, m_nWeekCnt, m_nYearCnt, m_nGridCnt;
	KB_p0602_InRec1 m_InData;
//@Solomon:091123SM068	-->
	CChartBozoInput m_bojoInput;
	CChartBozoInput	GetBozoInputData()	{ return m_bojoInput;	};
//@Solomon:091123SM068	<--

	CString Convert();
	char* Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen);
	
private:
	bool m_bSet;
};

#endif // !defined(AFX_CHARTITEMSTOCK_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
