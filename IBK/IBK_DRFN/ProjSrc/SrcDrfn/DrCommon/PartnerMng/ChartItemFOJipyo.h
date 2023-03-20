// ChartItemFOJipyo.h: interface for the ChartItemFOJipyo class.
//
// ��Ʈ �ֽ�, ����ü <-> FID ������ Ŭ����
// 2009.10.06 �ַθ� Ưȭ, ���н� �÷���
// by Jeoyoho
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTITEMFOJIPYO_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
#define AFX_CHARTITEMFOJIPYO_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../chart/commontr/KB_p0515.h"
//@Solomon:091123SM068
#include "../../inc/IChartBozoMsgMng.h"

class CChartItemFOJipyo
{
public:
	CChartItemFOJipyo();
	virtual ~CChartItemFOJipyo();
	void SetInData(KB_p0515_InRec* pData);

	int				m_nRowCnt;
	KB_p0515_InRec	m_InData;
	CString Convert();
	char* Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen);
	//@Solomon:091123SM068	-->
	CChartBozoInput m_bojoInput;
	CChartBozoInput	GetBozoInputData()	{ return m_bojoInput;	};
	//@Solomon:091123SM068	<--

private:
	bool m_bSet;
};

#endif // !defined(AFX_CHARTITEMFOJIPYO_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
