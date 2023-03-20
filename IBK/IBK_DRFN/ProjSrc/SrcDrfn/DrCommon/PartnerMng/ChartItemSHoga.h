// ChartItemSHoga.h: interface for the CChartItemSHoga class.
//
// ��Ʈ �ֽ�, ����ü <-> FID ������ Ŭ����
// 2009.10.06 �ַθ� Ưȭ, ���н� �÷���
// by Jeoyoho
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTITEMSHOGA_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
#define AFX_CHARTITEMSHOGA_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../chart/commontr/KB_01230.h"

class CChartItemSHoga  
{
public:
	CChartItemSHoga();
	virtual ~CChartItemSHoga();
	void SetInData(LPk01230In1 pData);

	int		m_nCnt;
	k01230In1 m_InData;
	CString Convert();
	char* Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen);

private:
	bool m_bSet;
};

#endif // !defined(AFX_CHARTITEMSHOGA_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
