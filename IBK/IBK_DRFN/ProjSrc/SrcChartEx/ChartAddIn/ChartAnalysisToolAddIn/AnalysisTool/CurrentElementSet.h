// CurrentElementSet.h: interface for the CCurrentElementSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURRENTELEMENTSET_H__E63A59A5_4981_46BD_A886_674B253D486F__INCLUDED_)
#define AFX_CURRENTELEMENTSET_H__E63A59A5_4981_46BD_A886_674B253D486F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/ToolBaseData.h"
#include "BlockBaseData.h"
#include "Coordinate.h"
#include "AnalysisData.h"

class CElement;
class CTextPosition;
class CPenEnvironment;

class CCurrentElementSet  
{
public:

	// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	//CCurrentElementSet(const CString& strUserPath,const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,CWnd* pWnd, CElement* pElement);
	CCurrentElementSet(IChartOCX *p_pIChartOCX, const CBlockIndex& blockIndex, const CString & strFormatX, const CString& strUserPath,const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,CWnd* pWnd, CElement* pElement, LONG pInterface);
	// 2011.02.08 by SYS <<

	bool OnMenu(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);
private:

	// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	IChartOCX	*m_pIChartOCX;
	CBlockIndex m_blockIndex;
	CString		m_strFormatX;
	// 2011.02.08 by SYS <<

	CString m_strUserPath;
	CWnd* m_pParent;
	CElement* m_pElement;

	LONG	m_pInterface;

	CScaleBaseData::HORZSCALEDRAWERTYPE m_nDataPeriod; 

	bool OnMenu_Set(const bool bIsAction);
	bool OnMenu_Pen(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);
	void OnMenu_Color(const COLORREF& color);
	void OnMenu_Width(const int nWidth);
	void OnMenu_Style(const CToolEnumData::PenStyle eStyle);

	bool AnalysisToolDialogLoad();
	bool FiboSetDialogLoad();		// 2008.08.20 ������ �Ǻ���ġ Ȯ��
	bool BalanceBasicSetDialogLoad();	// 2014.09.24 �ڼ��� : 0010862: [7111] �Ｚ���� ��Ʈ ������ [�ϸ��ġ] ��� IBK��Ʈ���� �����ϰ� �߰� ��û

protected:
	HWND	m_hOcxWnd;
	
};

#endif // !defined(AFX_CURRENTELEMENTSET_H__E63A59A5_4981_46BD_A886_674B253D486F__INCLUDED_)
