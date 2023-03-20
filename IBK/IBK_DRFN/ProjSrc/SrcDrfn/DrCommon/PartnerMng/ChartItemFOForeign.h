// ChartItemFOForeign.h: interface for the CChartItemFOForeign class.
//
// ��Ʈ �����ɼ�, ����ü <-> FID ������ Ŭ����
// 2009.10.06 �ַθ� Ưȭ, ���н� �÷���
// by Jeoyoho
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTITEMFOFOREIGN_H__66BCF18B_01D5_4D30_AAB2_E9551759F59D__INCLUDED_)
#define AFX_CHARTITEMFOFOREIGN_H__66BCF18B_01D5_4D30_AAB2_E9551759F59D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../inc/IChartBozoMsgMng.h"
#include "../../chart/commontr/EU_p0602.h"

class CChartItemFOForeign
{
public:
	CChartItemFOForeign();
	virtual ~CChartItemFOForeign();
	void SetInData(HCQ03010_IN* pData);

	int		m_nTickCnt, m_nSecCnt, m_nMinCnt, m_nDayCnt;
	HCQ03010_IN m_InData;
	CString Convert();
	char* Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen);

	//@Solomon:091123SM068	-->
	CChartBozoInput m_bojoInput;
	CChartBozoInput	GetBozoInputData()	{ return m_bojoInput;	};
	//@Solomon:091123SM068	<--

private:
	bool m_bSet;
};

#endif // !defined(AFX_CHARTITEMFOFOREIGN_H__66BCF18B_01D5_4D30_AAB2_E9551759F59D__INCLUDED_)
