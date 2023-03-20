#if !defined(AFX_APPABRATIO_H__58E2FA75_EA43_4E1E_961E_2EE21B4CE055__INCLUDED_)
#define AFX_APPABRATIO_H__58E2FA75_EA43_4E1E_961E_2EE21B4CE055__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

//////////////////////////////////////////////////////////////////////////
//	AB Ratio
//////////////////////////////////////////////////////////////////////////
//	Ÿ�� : ����
//	���� : 2
//	��� : 
//////////////////////////////////////////////////////////////////////////
//	����)
//	n1  : A-Ratio ������
//	n2  : B-Ratio ������
//
//	����)
//
//		     sum((hpr - fpr), n1)
//	A RATIO = �������������������������� ��  100
//		     sum((fpr - hpr), n1)
//
//		    sum((hpr - epr(1)), n2)
//	B RATIO = �������������������������� ��  100
//		    sum((epr(1) - lpr), n2)
//
//////////////////////////////////////////////////////////////////////////
class CAppABRatio : public CLineData  
{
public:
	CAppABRatio(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppABRatio();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);

};

#endif // !defined(AFX_APPABRATIO_H__58E2FA75_EA43_4E1E_961E_2EE21B4CE055__INCLUDED_)
