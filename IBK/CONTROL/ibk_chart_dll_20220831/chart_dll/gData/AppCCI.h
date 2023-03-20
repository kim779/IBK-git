// CCIData.h: interface for the CAppCCI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPCCI_H__E69C8417_66F9_4718_AD8E_78714265F3E5__INCLUDED_)
#define AFX_APPCCI_H__E69C8417_66F9_4718_AD8E_78714265F3E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

//////////////////////////////////////////////////////////////////////////
//	CCI
//////////////////////////////////////////////////////////////////////////
//	Ÿ�� : ����
//	���� : 1 �ñ׳�(��)
//	��� : 
//////////////////////////////////////////////////////////////////////////
//
//	����)
//	n : �Ⱓ
//
//	����)
//	MPt	: (�� + ���� + ����) / 3
//	MPMAt	: MPt's Moving Average
//	MDt	: ������� = ��[abs(MPi - MPMAt)] / n (i = t���� t-n+1����)
//
//////////////////////////////////////////////////////////////////////////

class CAppCCI : public CLineData  
{
public:
	CAppCCI(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppCCI();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);	
	double	m_prevVal[2];

};

#endif // !defined(AFX_APPCCI_H__E69C8417_66F9_4718_AD8E_78714265F3E5__INCLUDED_)
