// IWGraphInfoManager050116.cpp: implementation of the CIWGraphInfoManager050116 class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// *. ����!	050116 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� Wrapper�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		������ �ʴ� ������ 050116 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050116 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			��¿�
//			ChartPatternAddIn			GZone			������
//			ChartGuideLine				GZone			������
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWGraphInfoManager050116.h"

#include "IndicatorList.h"				// for CIndicatorList
#include "GraphData.h"					// for CSubGraphData
#include "IndicatorInfo.h"				// for CIndicatorInfo

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIWGraphInfoManager050116::CIWGraphInfoManager050116( CIndicatorList *p_pIndicatorList)
{
	// (2004.10.11, ��¿�) MainBlock Object�� Pointer�� �����Ѵ�.
	m_pIndicatorList = p_pIndicatorList;
	if( !m_pIndicatorList)
	{
		AfxMessageBox( "CIWGraphInfoManager050116 Null Wrapping Error!");
		return;
	}
}

CIWGraphInfoManager050116::~CIWGraphInfoManager050116()
{

}

// [04/10/14] Graph�� 6������ ��ȸ�ϴ� Interface�� �����Ѵ�.
BOOL CIWGraphInfoManager050116::GetGraph6Color( BOOL p_bDefaultInfo, const char *p_szGraphName, int p_nSubIndex, COLORREF p_aclrGraphColor6[ 6])
{
	// 1. Indicator Info�� ���Ѵ�.
	CIndicatorInfo *pIndicatorInfo = NULL;
	if( p_bDefaultInfo) pIndicatorInfo = m_pIndicatorList->GetDefaultIndicatorInfo( p_szGraphName);
	else pIndicatorInfo = m_pIndicatorList->GetUserIndicatorInfo( p_szGraphName);
	if( !pIndicatorInfo) return FALSE;

	// 2. SubGraph�� ���Ѵ�.
	CSubGraphData *pSubGraphData = pIndicatorInfo->GetSubGraphData( p_nSubIndex);
	if( !pSubGraphData) return FALSE;

	// 3. Graph Color�� ���Ѵ�.
	CGraphColor *pGraphColor = pSubGraphData->GetColor_PointerType();
	if( !pGraphColor) return FALSE;
	p_aclrGraphColor6[ 0] = pGraphColor->GetColor1();
	p_aclrGraphColor6[ 1] = pGraphColor->GetColor2();
	p_aclrGraphColor6[ 2] = pGraphColor->GetColor3();
	p_aclrGraphColor6[ 3] = pGraphColor->GetColor4();
	p_aclrGraphColor6[ 4] = pGraphColor->GetColor5();
	p_aclrGraphColor6[ 5] = pGraphColor->GetColor6();
	return TRUE;
}