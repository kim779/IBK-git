// TabDlgCreator.cpp: implementation of the CTabDlgCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabDlgCreator.h"
//#include "TabChart.h"
#include "./define/ChartMsgDef.h"
#include "TabIndices.h"
//#include "TabFavoriteGrp.h"
#include "TabCodeSearch.h"
//@��������
//#include "TabCodeSearchJisu.h"
//#include "TabCodeSearchFO.h"
//#include "TabCodeSearchFR.h"
//#include "TabCodeSearchELW.h"
//@��������
//#include "TabMyFavorite.h"
//#include "TabOrder.h"
//#include "TabSystemTrading.h"		// �ý���Ʈ���̵� �� ����
#include "TabSystemTradingEx.h"
//#include "TabAnalTool.h"
// ��ǰ�����ɼ� 2�� - ojtaso (20071016)
//@��������
//#include "TabCodeSearchCommodity.h"
//@��������

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define CREATETAB(_pTab,_class) {\
		_pTab = new _class;\
		if(_pTab) {\
		_pTab->m_pParentDlg = m_pParentDlg;\
		_pTab->m_pMultiChartWnd = m_pMultiChartWnd;\
		_pTab->m_pChartMng = m_pChartMng;\
		_pTab->Create(_class::IDD, m_pTabCtrl);} }


CTabDlgCreator::CTabDlgCreator()
{
	m_pParentDlg = NULL;
	m_pTabCtrl = NULL;
	m_pMultiChartWnd = NULL;
	m_pChartMng = NULL;
}

CTabDlgCreator::~CTabDlgCreator()
{

}

void CTabDlgCreator::Init(CWnd* pMultiChartWnd, CLeftBarDlg *pParentDlg,CTabCtrl *pTabCtrl,CWnd *pChartMng)
{
	m_pMultiChartWnd = pMultiChartWnd;
	m_pParentDlg = pParentDlg;
	m_pTabCtrl = pTabCtrl;
	m_pChartMng = pChartMng;
}

int CTabDlgCreator::GetCntOfTabDlg()
{	
	return STOCK_TABTOTALCNT;
}


void CTabDlgCreator::GetCaptionOfTabDlg(UINT nIndex, char *pszCaption)
{
	switch(nIndex)
	{
//	case TABNO_CHART:
//		strcpy(pszCaption,TAB_CHART);
//		break;
	case TABNO_ASSISTINDEX:
		strcpy(pszCaption,TAB_ASSISTINDEX);
		break;
//	case TABNO_FAVORITEGRP:
//		strcpy(pszCaption,TAB_FAVORITEGRP);
//		break;
	case TABNO_CODESEARCH:
		strcpy(pszCaption,TAB_CODESEARCH);
		break;
//	case TABNO_MYFAVORITE:
//		strcpy(pszCaption,TAB_MYFAVORITE);
//		break;
//	case TABNO_ORDER:
//		strcpy(pszCaption,TAB_ORDER);
//		break;	
	case TABNO_SYSTEMTRADING:
		strcpy(pszCaption,TAB_SYSTEMTRADING);
		break;	
//@��������
//	case TABNO_SYSTEM: //2005. 05. 16�� �߰�.===================================
//		strcpy(pszCaption,TAB_SYSTEM);
//		break;
//@��������
		//2005. 05. 16�� �߰�.===================================

	}
}

CTabDlg* CTabDlgCreator::CreateTabDlg(UINT nIndex)
{
	CTabDlg* _pTab = NULL;
	switch(nIndex)
	{
	//case TABNO_CHART:		CREATETAB(_pTab,CTabChart)			break;
//	case TABNO_CHART:		CREATETAB(_pTab,CTabAnalTool)		break;
	case TABNO_ASSISTINDEX:	CREATETAB(_pTab,CTabIndices)		break;
//	case TABNO_FAVORITEGRP:	CREATETAB(_pTab,CTabFavoriteGrp)	break;
	case TABNO_CODESEARCH:	CREATETAB(_pTab,CTabCodeSearch)		break;
//@��������
//	case TABNO_CODESEARCHFO:	CREATETAB(_pTab,CTabCodeSearchFO)		break;
//	case TABNO_CODESEARCHUP:	CREATETAB(_pTab,CTabCodeSearchJisu)		break;
//	case TABNO_CODESEARCHFR:	CREATETAB(_pTab,CTabCodeSearchFR)		break;	//101 2006.9.14, �ؿ�
//	case TABNO_CODESEARCHELW:	CREATETAB(_pTab,CTabCodeSearchELW)		break;	//101 2006.9.14, ELW
//@��������
//	case TABNO_MYFAVORITE:	CREATETAB(_pTab,CTabMyFavorite)		break;
//	case TABNO_ORDER:		CREATETAB(_pTab,CTabOrder)			break; //2005. 08. 25 �ֹ� ���� �Ͻ������� ���´�.

//	case TABNO_SYSTEMTRADING:	CREATETAB(_pTab,CTabSystemTrading)		break;		// �ý���Ʈ���̵��Ǻ���. 060718
	//solomon. �ؿܼ����� �ý���Ʈ���̵� ����
	//case TABNO_SYSTEMTRADING:	CREATETAB(_pTab,CTabSystemTradingEx)		break;
		// ��ǰ�����ɼ� 2�� - ojtaso (20071016)
//@��������
//	case TABNO_CODESEARCHCMM:	CREATETAB(_pTab,CTabCodeSearchCommodity)		break;
//@��������
	}
	return _pTab;
}
