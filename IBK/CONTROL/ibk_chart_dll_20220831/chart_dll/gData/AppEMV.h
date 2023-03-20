#if !defined(AFX_CAPPEMV_H__A60D21FF_C28D_453E_9FF2_A5091AD78028__INCLUDED_)
#define AFX_CAPPEMV_H__A60D21FF_C28D_453E_9FF2_A5091AD78028__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

/*
Arm's Ease of Movement
Val1 = (( ���ϰ� + �������� ) / 2 - (���ϰ� + ��������) / 2) / ((�ŷ���) / (���ϰ� - ��������))
EMV = Val1 �� n�� ���� ����
*/

class CAppEMV : public CLineData  
{
public:
	CAppEMV(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppEMV();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);

};

#endif // !defined(AFX_CAPPEMV_H__A60D21FF_C28D_453E_9FF2_A5091AD78028__INCLUDED_)
