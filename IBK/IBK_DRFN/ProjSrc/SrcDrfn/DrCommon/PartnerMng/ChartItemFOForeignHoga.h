// ChartItemFOForeignHoga.h: interface for the CChartItemFOForeignHoga class.
//
// ��Ʈ �����ɼ�, ����ü <-> FID ������ Ŭ����
// 2009.10.06 �ַθ� Ưȭ, ���н� �÷���
// by Jeoyoho
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTITEMFOFOREIGNHOGA_H__66BCF18B_01D5_4D30_AAB2_E9551759F59D__INCLUDED_)
#define AFX_CHARTITEMFOFOREIGNHOGA_H__66BCF18B_01D5_4D30_AAB2_E9551759F59D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../inc/IChartBozoMsgMng.h"

class CChartItemFOForeignHoga
{
public:
	CChartItemFOForeignHoga();
	virtual ~CChartItemFOForeignHoga();
	void SetInData(HCQ01112_IN* pData);

	HCQ01112_IN m_InData;
	int			m_nCnt;

	CString Convert();
	char* Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen);

	//@Solomon:091123SM068	-->
	CChartBozoInput m_bojoInput;
	CChartBozoInput	GetBozoInputData()	{ return m_bojoInput;	};
	//@Solomon:091123SM068	<--

private:
	bool m_bSet;
};

#endif // !defined(AFX_CHARTITEMFOFOREIGNHOGA_H__66BCF18B_01D5_4D30_AAB2_E9551759F59D__INCLUDED_)
