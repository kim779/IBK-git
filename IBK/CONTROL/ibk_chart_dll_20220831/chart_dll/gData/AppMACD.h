#if !defined(AFX_APPMACD_H__A65754D5_7E36_484A_A264_D227A3C31CE2__INCLUDED_)
#define AFX_APPMACD_H__A65754D5_7E36_484A_A264_D227A3C31CE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"
//	MACD

//	11 MACD	= �ܱ���������(n1) - �����������(n2)
//	12 Signal	= MACD�� ��������(n3)
//	20   Oscillator	= MACD - Signal

//	��������(EMV)	= [ a**(n-1)*P��+ a**(n-2)*P��+ ... + 1*Pn ] /
//			[ a**(n-1) + a**(n-2) + ... + 1 ]
//	a(��Ȱ���)	= 2.0 / (n + 1)
//	�⺻��		: n1 = 12(a=2/13),n2 = 26(a=2/27),n3 = 9(a=0.2)
//	EMVt	= EMV(t-1) + a*( Pt - EMV(t-1) ) 

class CAppMACD : public CLineData  
{
public:
	CAppMACD(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppMACD();	

protected:
	double	m_adPrevVal[6];

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);	
	
};

#endif // !defined(AFX_APPMACD_H__A65754D5_7E36_484A_A264_D227A3C31CE2__INCLUDED_)
