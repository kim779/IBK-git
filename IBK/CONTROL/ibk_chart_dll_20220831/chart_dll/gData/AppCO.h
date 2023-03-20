#if !defined(AFX_APPCO_H__A1603A16_8A37_4D69_97B5_AE2D2C1124E9__INCLUDED_)
#define AFX_APPCO_H__A1603A16_8A37_4D69_97B5_AE2D2C1124E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

//////////////////////////////////////////////////////////////////////////
//	Chaikin's Oscillator
//////////////////////////////////////////////////////////////////////////
//	Ÿ�� : ����
//	���� : 1
//	��� : 
//////////////////////////////////////////////////////////////////////////
//
//	����)
//	n1 : �ܱ�Ⱓ
//	n2 : ���Ⱓ
//
//	����)
//		(����- ����)-(��-����)
//	CAD = ���������������������������� �� �ŷ����� ����ġ
//		        ��-����
//	CO = �ܱ� ���� - �������
//
//////////////////////////////////////////////////////////////////////////

class CAppCO : public CLineData  
{
public:
	CAppCO(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppCO();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);	
	double	m_prevVal[6];
};

#endif // !defined(AFX_APPCO_H__A1603A16_8A37_4D69_97B5_AE2D2C1124E9__INCLUDED_)
