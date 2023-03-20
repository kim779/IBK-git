#if !defined(AFX_APPBOL_H__69D7D08D_92B9_4B7B_9F43_4114AFACC039__INCLUDED_)
#define AFX_APPBOL_H__69D7D08D_92B9_4B7B_9F43_4114AFACC039__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

//////////////////////////////////////////////////////////////////////////
//	Bollinger Band
//////////////////////////////////////////////////////////////////////////
//	Ÿ�� : ����
//	���� : 3
//	��� : 
//////////////////////////////////////////////////////////////////////////
//	����)
//	n : �Ⱓ
//	Multi : �¼�
//
//	����)
//	HighLine = MidLine + Multi * std(MidLine, n)	// MidLine�� n�ϰ��� ǥ�������� ���Ͽ� 
//							// �¼����Ͽ� ���Ѵ�
//	MidLine = MA(epr, n)				// n�ϰ��� ���� ����
//	LowLine = MidLine - Multi * std(MidLine, n)	// MidLine�� n�ϰ��� ǥ�������� ���Ͽ� 
//							// �¼����Ͽ� ����
//      
//      
//
//////////////////////////////////////////////////////////////////////////

class CAppBol : public CLineData  
{
public:
	CAppBol(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppBol();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);	
};

#endif // !defined(AFX_APPBOL_H__69D7D08D_92B9_4B7B_9F43_4114AFACC039__INCLUDED_)
