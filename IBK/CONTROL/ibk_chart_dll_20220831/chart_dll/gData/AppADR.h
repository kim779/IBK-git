// AppADR.h: interface for the CAppADR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPADR_H__68A06F34_E509_4888_8AB8_EF9858F7F83E__INCLUDED_)
#define AFX_APPADR_H__68A06F34_E509_4888_8AB8_EF9858F7F83E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

//////////////////////////////////////////////////////////////////////////
//	ADR
//////////////////////////////////////////////////////////////////////////
//	Ÿ�� : ����
//	���� : 1
//	��� : 
//////////////////////////////////////////////////////////////////////////
//	����)
//	n : ������
//
//	����)
//
//	UpCount�� DownCount�� �������� �޴´�
//	UpSum = sum(UpCount, n) / sum(DownCount, n) * 100
//	DownSum = sum(DownCount, n)
//      P = UpSum / DownSum * 100
//      ADR = MA(P, n)
//
//////////////////////////////////////////////////////////////////////////

class CAppADR : public CLineData  
{
public:
	CAppADR(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppADR();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);

};

#endif // !defined(AFX_APPADR_H__68A06F34_E509_4888_8AB8_EF9858F7F83E__INCLUDED_)
