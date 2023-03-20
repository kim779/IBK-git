#if !defined(AFX_APPRATE_H__4932F483_2F91_4B7B_BB2B_5CEC052AB31E__INCLUDED_)
#define AFX_APPRATE_H__4932F483_2F91_4B7B_BB2B_5CEC052AB31E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

/*
�ŷ���ȸ������ ������ �ŷ��� �Ը� �������� �Ͽ� �����ֽ� �Ѽ��� ���� �� ȸ���ϰ� �ִ°��� 
��Ÿ�� ���̴�. ȸ���� 100%�� �ǹ̴� �����ֽ� �Ѽ��� ���� 1ȸ���ϴ� ���� �ǹ��Ѵ�. 
������ �������� �� �� 50%�� �������� 130% �̻��� �ܱ���, 20% �����̸� �ٴ����� ����
*/

// ȸ����
// ����: ( (���ϰŷ���*��ȸ�ϼ�(300)) / �����ֽ��Ѽ� ) * 100

class CAppRate : public CLineData  
{
public:
	CAppRate(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppRate();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);
};

#endif // !defined(AFX_APPRATE_H__4932F483_2F91_4B7B_BB2B_5CEC052AB31E__INCLUDED_)
