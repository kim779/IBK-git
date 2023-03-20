#if !defined(AFX_APPMFI_H__9070B1EE_3F7D_4EC5_896B_849439919F73__INCLUDED_)
#define AFX_APPMFI_H__9070B1EE_3F7D_4EC5_896B_849439919F73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"
///////////////////////////////////////////////////////////////////////////////
// MFI (Money Flow Index)
// �ֽ� �ŷ��� ���� �ڱ��� ���Եǰų� ����Ǵ� ���� ������ �ִ� ��ǥ�μ�
// �߼� ��ȯ�ñ⸦ �����ϰų� �ü��� ���� �� ħü������ �ľ��ϴµ� ����
// ���� : TP(Typical Price)
// TP = (��+����+����) / 3
// Money Flow = TP * �ŷ���
// PositiveMoneyFlow : ���� TP > ���� TP �϶� MoneyFlow�� ��
// NegativeMoneyFlow : ���� TP < ���� TP �϶� MoneyFlow�� ��
// Money Ratio : PositiveMoneyFlow / NegativeMoneyFlow
// Money Flow Index = 100 - 100 / (1 + MoneyRatio)
///////////////////////////////////////////////////////////////////////////////
class CAppMFI : public CLineData  
{
public:
	CAppMFI(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppMFI();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);	
};

#endif // !defined(AFX_APPMFI_H__9070B1EE_3F7D_4EC5_896B_849439919F73__INCLUDED_)
