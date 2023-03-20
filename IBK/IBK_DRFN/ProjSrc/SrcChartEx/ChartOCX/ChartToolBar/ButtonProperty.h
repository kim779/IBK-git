// ButtonProperty.h: interface for the CButtonProperty class.
// ���ٿ��� ����ϴ� ��ư���� State������ ���� ��å�� ������ Ŭ�����̴�.
// ��, Ư�� ��ư�� �������� ����ǰ� �ִ� �ٸ� ����ư�� �����Ұ������� �����ϴ� Ŭ����.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUTTONPROPERTY_H__C62A5FC8_2FEF_417D_988B_B9440F30CD1B__INCLUDED_)
#define AFX_BUTTONPROPERTY_H__C62A5FC8_2FEF_417D_988B_B9440F30CD1B__INCLUDED_

#include <AFXTEMPL.H>
#include "ToolOption.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CButtonEx;
class CToolBarDlg;

typedef CList<CButtonEx*, CButtonEx*> CButtonList; // ToolBarDlg.h���� ���ǵǾ� ����.

class CButtonProperty  
{
public:
	CButtonProperty(CWnd* pParent, CButtonList* pButtonList);
	virtual ~CButtonProperty();
	int ReleaseOtherButtonUI( CToolOptionInfo::TOOLOPTION enumToolOption, const CString& strParam = _T(""));	// ������ ��ư Ǯ�� ������.
private:
	void ReleaseCaseZoom(CToolOptionInfo::TOOLOPTION enumToolOption, const CString& strParam);
	void ReleaseCaseSpecialChart(CToolOptionInfo::TOOLOPTION enumToolOption);
	void ReleaseCaseDefault(CToolOptionInfo::TOOLOPTION enumToolOption);
	void ReleaseCaseCrossLine();
	
	void ReleaseToolOption(CToolOptionInfo::TOOLOPTION enumToolOption);

	bool IsModalessDialog(CToolOptionInfo::TOOLOPTION enumToolOption);

private:
	CButtonList* m_pButtonList;
	CToolBarDlg* m_pParent;
};

#endif // !defined(AFX_BUTTONPROPERTY_H__C62A5FC8_2FEF_417D_988B_B9440F30CD1B__INCLUDED_)
