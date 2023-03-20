// ChartItemFOHoga.h: interface for the CChartItemFOHoga class.
//
// ��Ʈ �ֽ�, ����ü <-> FID ������ Ŭ����
// 2009.10.06 �ַθ� Ưȭ, ���н� �÷���
// by Jeoyoho
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTITEMFOHOGA_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
#define AFX_CHARTITEMFOHOGA_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../chart/commontr/KB_f0201.h"

class CChartItemFOHoga  
{
public:
	CChartItemFOHoga();
	virtual ~CChartItemFOHoga();
	void SetInData(KB_f0201_InRec1* pData);

	int		m_nCnt;
	KB_f0201_InRec1 m_InData;
	CString Convert();
	char* Convert2Struct(LPBYTE pData, int nLen, CString szTRCode, int &nDataLen);
	//KHD : IBK�� �����ϰ� �ɼ��ϰ� ���� �����ϱ⿡ �и���.
	CString ConvertFuture();
	CString ConvertOption();
	CString ConvertCommodity();//��ǰ����
	int m_nCodeType;
//END
private:
	bool m_bSet;
};

#endif // !defined(AFX_CHARTITEMFOHOGA_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
