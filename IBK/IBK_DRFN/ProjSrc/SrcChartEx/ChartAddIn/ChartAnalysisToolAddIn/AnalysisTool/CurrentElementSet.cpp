// CurrentElementSet.cpp: implementation of the CCurrentElementSet class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CurrentElementSet.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "Element.h"
#include "DlgAnalysisToolSet.h"
#include "DlgFiboSet.h"								// 2008.08.20 ������ �Ǻ���ġ Ȯ��
#include "DlgBalanceBasicSet.h"						// 2014.09.24 �ڼ��� : 0010862: [7111] �Ｚ���� ��Ʈ ������ [�ϸ��ġ] ��� IBK��Ʈ���� �����ϰ� �߰� ��û

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CCurrentElementSet

// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
//CCurrentElementSet::CCurrentElementSet(const CString& strUserPath,
//									   const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
//									   CWnd* pParent, CElement* pElement):
CCurrentElementSet::CCurrentElementSet(IChartOCX *p_pIChartOCX, 
									   const CBlockIndex& blockIndex,
									   const CString & strFormatX,
									   const CString& strUserPath,
									   const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
									   CWnd* pParent, 
									   CElement* pElement,
									   LONG pInterface):
// 2011.02.08 by SYS <<
	m_strUserPath(strUserPath),
	m_nDataPeriod(nDataPeriod),
	m_pParent(pParent),
	m_pElement(pElement),
	m_pInterface(pInterface)
{
	m_hOcxWnd = ( pParent ? pParent->GetSafeHwnd() : NULL);

	// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	m_pIChartOCX	= p_pIChartOCX;
	if(blockIndex.IsAllValueBiggerThanZero())
	{
		m_blockIndex.SetRow(blockIndex.GetRow());
		m_blockIndex.SetColumn(blockIndex.GetColumn());
	}
	m_strFormatX	= strFormatX;
	// 2011.02.08 by SYS <<
}

// public =====================================================================
bool CCurrentElementSet::OnMenu(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	if(m_pElement == NULL)
		return false;

	switch(menuType){
	case CMenuType::MENU_SET:	// ���� Dlg
		return OnMenu_Set( wParam ? true : false);
	default:	
		return OnMenu_Pen(menuType, wParam);
	};

	return false;
}

// private ====================================================================
bool CCurrentElementSet::OnMenu_Set(const bool bIsAction)
{
	if(!bIsAction)
		return false;

	// 2008.08.20 ������ �Ǻ���ġ Ȯ��<<
	CToolEnumData::ElementType nElementType = m_pElement->GetElementType();
	if( nElementType == CToolEnumData::Fibo_Semicircle ||	// �Ǻ� �ݿ�
			nElementType == CToolEnumData::Fibo_Circle ||		// �Ǻ� ��
			nElementType == CToolEnumData::Fibo_Fan_Line ||		// �Ǻ� �����
			nElementType == CToolEnumData::Fibo_Retrace ||		// �Ǻ� �ǵ���(������)
			nElementType == CToolEnumData::Fibo_Time )			// �Ǻ� �ð���
		return FiboSetDialogLoad();
	// 2014.09.24 �ڼ��� : 0010862: [7111] �Ｚ���� ��Ʈ ������ [�ϸ��ġ] ��� IBK��Ʈ���� �����ϰ� �߰� ��û >>
	else if( nElementType == CToolEnumData::Balance_Basic )	// �ϸ��ġ
		return BalanceBasicSetDialogLoad();
	// 2014.09.24 �ڼ��� : 0010862: [7111] �Ｚ���� ��Ʈ ������ [�ϸ��ġ] ��� IBK��Ʈ���� �����ϰ� �߰� ��û <<
	else
	//>>
		return AnalysisToolDialogLoad();
}

bool CCurrentElementSet::OnMenu_Pen(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	switch(menuType){
	case CMenuType::MENU_COLOR:		// ��
		OnMenu_Color((COLORREF) wParam);
		break;
	case CMenuType::MENU_THICKNESS: // thickness
		OnMenu_Width((int) wParam);
		break;
	case CMenuType::MENU_DRAWSTYLE: // draw style
		OnMenu_Style((CToolEnumData::PenStyle) wParam);
		break;
	default:
		return false;
	}

	if(m_pParent != NULL)
		m_pParent->Invalidate();

	return true;
}

void CCurrentElementSet::OnMenu_Color(const COLORREF& color)
{
	m_pElement->GetCommonEnvironment().GetPen().SetColor(color);
}

void CCurrentElementSet::OnMenu_Width(const int nWidth)
{
	m_pElement->GetCommonEnvironment().GetPen().SetWidth(nWidth);
}

void CCurrentElementSet::OnMenu_Style(const CToolEnumData::PenStyle eStyle)
{
	m_pElement->GetCommonEnvironment().GetPen().SetStyle(eStyle);
}

// ----------------------------------------------------------------------------
bool CCurrentElementSet::AnalysisToolDialogLoad()
{
	CTextPosition* pTextPosition = const_cast <CTextPosition*> (m_pElement->GetTextPosition());
	CElementPointList elementPointList;
	m_pElement->GetPoints(elementPointList);

	ML_SET_LANGUAGE_RES();

	// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
// 	CDlgAnalysisToolSet dlg(m_pParent,m_strUserPath,
// 							m_nDataPeriod,
// 							m_pElement->GetElementType(),
// 							&m_pElement->GetCommonEnvironment().GetEquiDiv(),
// 							&m_pElement->GetCommonEnvironment().GetPriceRangeDiv(),	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
// 							pTextPosition,
// 							&m_pElement->GetCommonEnvironment().GetPen(),
// 							&elementPointList,
// 							&m_pElement->GetCommonEnvironment().GetLineExt(),
// 							&m_pElement->GetCommonEnvironment().GetDataType());

	CDlgAnalysisToolSet dlg(m_pParent,m_strUserPath,
							m_nDataPeriod,
							m_pElement->GetElementType(),
							&m_pElement->GetCommonEnvironment().GetEquiDiv(),
							&m_pElement->GetCommonEnvironment().GetPriceRangeDiv(),	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
							pTextPosition,
							&m_pElement->GetCommonEnvironment().GetPen(),
							&elementPointList,
							&m_pElement->GetCommonEnvironment().GetLineExt(),
							&m_pElement->GetCommonEnvironment().GetDataType(),
							m_pIChartOCX,
							m_blockIndex,
							m_strFormatX,
							m_pInterface);
	// 2011.02.08 by SYS <<


	dlg.DoModal();	
	return true;
}

// 2008.08.20 ������ �Ǻ���ġ Ȯ��<<
bool CCurrentElementSet::FiboSetDialogLoad()
{
	CTextPosition* pTextPosition = const_cast <CTextPosition*> (m_pElement->GetTextPosition());
	CElementPointList elementPointList;
	m_pElement->GetPoints(elementPointList);

	ML_SET_LANGUAGE_RES();

	// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
// 	CDlgFiboSet dlg(m_pParent,m_strUserPath,
// 							m_nDataPeriod,
// 							m_pElement->GetElementType(),
// 							&m_pElement->GetCommonEnvironment().GetEquiDiv(),
// 							&m_pElement->GetCommonEnvironment().GetPriceRangeDiv(),	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
// 							pTextPosition,
// 							&m_pElement->GetCommonEnvironment().GetPen(),
// 							&elementPointList,
// 							&m_pElement->GetCommonEnvironment().GetLineExt(),
// 							&m_pElement->GetCommonEnvironment().GetDataType());

	CDlgFiboSet dlg(m_pParent,m_strUserPath,
							m_nDataPeriod,
							m_pElement->GetElementType(),
							&m_pElement->GetCommonEnvironment().GetEquiDiv(),
							&m_pElement->GetCommonEnvironment().GetPriceRangeDiv(),	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
							pTextPosition,
							&m_pElement->GetCommonEnvironment().GetPen(),
							&elementPointList,
							&m_pElement->GetCommonEnvironment().GetLineExt(),
							&m_pElement->GetCommonEnvironment().GetDataType(),
							m_pIChartOCX,
							m_blockIndex,
							m_strFormatX);
	// 2011.02.08 by SYS <<

	dlg.DoModal();	

	return true;
}
//>>

// 2014.09.24 �ڼ��� : 0010862: [7111] �Ｚ���� ��Ʈ ������ [�ϸ��ġ] ��� IBK��Ʈ���� �����ϰ� �߰� ��û >>
bool CCurrentElementSet::BalanceBasicSetDialogLoad()
{
	CTextPosition* pTextPosition = const_cast <CTextPosition*> (m_pElement->GetTextPosition());
	CElementPointList elementPointList;
	m_pElement->GetPoints(elementPointList);
	
	ML_SET_LANGUAGE_RES();
	CDlgBalanceBasicSet dlg(m_pParent,
		m_strUserPath,
		m_nDataPeriod,
		m_pElement->GetElementType(),
		pTextPosition,
		&m_pElement->GetCommonEnvironment().GetPen(),
		&elementPointList,
		&m_pElement->GetCommonEnvironment().GetLineExt(),
		&m_pElement->GetCommonEnvironment().GetDataType());
	
	dlg.DoModal();
	
	return true;
}
// 2014.09.24 �ڼ��� : 0010862: [7111] �Ｚ���� ��Ʈ ������ [�ϸ��ġ] ��� IBK��Ʈ���� �����ϰ� �߰� ��û <<