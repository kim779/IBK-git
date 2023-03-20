#if !defined(AFX_APPMAO_H__0B011CD4_FA2A_4DF3_AA96_52F6AA5DA86D__INCLUDED_)
#define AFX_APPMAO_H__0B011CD4_FA2A_4DF3_AA96_52F6AA5DA86D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"
// MAO
// PMAO : (�ܱ��������� - �����������)/�ܱ���������
// VMAO : (�ܱ�ŷ������� - ���ŷ�������)/�ܱ�ŷ�����
//	n: �⺻�� 10, 20

class CAppMAO : public CLineData  
{
public:
	CAppMAO(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppMAO();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);	
	double	m_prevVal[6];
};

#endif // !defined(AFX_APPMAO_H__0B011CD4_FA2A_4DF3_AA96_52F6AA5DA86D__INCLUDED_)
