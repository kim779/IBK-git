#if !defined(AFX_APPDISPARITY_H__1E137340_40EC_4245_B0EC_B4AE0D13FED6__INCLUDED_)
#define AFX_APPDISPARITY_H__1E137340_40EC_4245_B0EC_B4AE0D13FED6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

// �̰ݷ��� ������ �ʹ� ���� ���� ���ؼ� 100�� ���Ƿ� ���ϰ� �ٽ� ������ �����ش�.

class CAppDisparity : public CLineData  
{
public:
	CAppDisparity(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppDisparity();	

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);	
	double	m_prevVal[2];
};

#endif // !defined(AFX_APPDISPARITY_H__1E137340_40EC_4245_B0EC_B4AE0D13FED6__INCLUDED_)
