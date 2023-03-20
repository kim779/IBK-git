// ChartItemDollar.h: interface for the CChartItemDollar class.
//
// ��Ʈ �ֽ�, ����ü <-> FID ������ Ŭ����
// 2009.10.06 �ַθ� Ưȭ, ���н� �÷���
// by Jeoyoho
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTITEMDOLLAR_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
#define AFX_CHARTITEMDOLLAR_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../chart/commontr/KB_d1018.h"

class CChartItemDollar
{
public:
	CChartItemDollar();
	virtual ~CChartItemDollar();
	void SetInData(KB_d1018_InRec* pData);

	int		m_nCnt;
	KB_d1018_InRec m_InData;

	CString Convert();
	char* Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen);
	
private:
	bool m_bSet;
};

#endif // !defined(AFX_CHARTITEMDOLLAR_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
