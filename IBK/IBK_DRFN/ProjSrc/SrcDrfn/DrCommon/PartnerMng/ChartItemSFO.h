// ChartItemSFO.h: interface for the CChartItemSFO class.
//
// ��Ʈ �ֽļ����ɼ�, ����ü <-> FID ������ Ŭ����
// 2009.10.06 �ַθ� Ưȭ, ���н� �÷���
// by Jeoyoho
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTITEMSFO_H__3C32422C_A8FD_44D4_AE1B_153C8F536919__INCLUDED_)
#define AFX_CHARTITEMSFO_H__3C32422C_A8FD_44D4_AE1B_153C8F536919__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../chart/commontr/KB_d1013.h"
//@Solomon:091123SM068
#include "../../inc/IChartBozoMsgMng.h"

class CChartItemSFO  
{
public:
	CChartItemSFO();
	virtual ~CChartItemSFO();
	void SetInData(KB_d1013_InRec1* pData);

	KB_d1013_InRec1 m_InData;
	CString Convert();
	char* Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen);
	//@Solomon:091123SM068	-->
	CChartBozoInput m_bojoInput;
	CChartBozoInput	GetBozoInputData()	{ return m_bojoInput;	};
	//@Solomon:091123SM068	<--

private:
	bool m_bSet;
};

#endif // !defined(AFX_CHARTITEMSFO_H__3C32422C_A8FD_44D4_AE1B_153C8F536919__INCLUDED_)
