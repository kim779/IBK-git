// AppBandWidth.h: interface for the CAppBandWidth class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPBANDWIDTH_H__95638802_71F7_4CE3_831E_678546B15B7E__INCLUDED_)
#define AFX_APPBANDWIDTH_H__95638802_71F7_4CE3_831E_678546B15B7E__INCLUDED_

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
//      Band Width = (��������� ���Ѽ�-��������� ���Ѽ�) / ��������� �߽ɼ�
//      
//
//////////////////////////////////////////////////////////////////////////

class CAppBandWidth : public CLineData  
{
public:
	CAppBandWidth(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppBandWidth();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);

	double	*m_pBollinger[3];

};

#endif // !defined(AFX_APPBANDWIDTH_H__95638802_71F7_4CE3_831E_678546B15B7E__INCLUDED_)
