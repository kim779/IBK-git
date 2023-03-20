// ButtonProperty.cpp: implementation of the CButtonProperty class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ButtonProperty.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "ButtonEx.h"
#include "Definition.h"
#include "ToolBarDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CButtonProperty::CButtonProperty(CWnd* pParent, CButtonList* pButtonList)
{
	m_pButtonList = pButtonList;
	m_pParent = (CToolBarDlg*) pParent;
}

CButtonProperty::~CButtonProperty()
{
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseOtherButtonUI
 - Created at  :  2004-11-19   15:50
 - Author      :  ������
 - Parameters  :  enumToolOption - ���Ӱ� ������ ���ɼ�.(�� ���ɼ��� ������ �������͵鿡�� Release�� ���Ѿ��Ѵ�.)
 - Return Value:  int - 1: ����, else: ����
 - Description :  ���� ������ ��ư �̿��� ��ư�� �Ӽ�(?)�� �°� Release��Ų��. (�� Ŭ������ ������)
 -----------------------------------------------------------------------------------------*/
int CButtonProperty::ReleaseOtherButtonUI(CToolOptionInfo::TOOLOPTION enumToolOption, const CString& strParam /*= _T("")*/)
{
	switch(enumToolOption) 
	{
		// (2006/3/4 - Seung-Won, Bae) Do not release Numerical Inquiry DLG and Cross Line.
		//		Cross Line of Inner Toolbar control NumericalInquiryOption Value.
		case CToolOptionInfo::T_NIDLG_DRAG:
		case CToolOptionInfo::T_CROSSHAIRS_DRAG:
		// (2006/6/29 - Seung-Won, Bae) Over Tools
		case CToolOptionInfo::T_NIDLG_OVER:
		case CToolOptionInfo::T_CROSSHAIRS_OVER:
													break;

		case CToolOptionInfo::T_INFLATE:				// Ȯ��/���
			// ��޸���Dlg, ���̴�, ��ü�ִ��ּҺ���, �ܺ����� ���ܽ�Ŵ
			// (2006/6/23 - Seung-Won, Bae) Except the T_CROSSHAIRS_DRAG, T_NIDLG_DRAG
			ReleaseCaseZoom(enumToolOption, strParam);
			break;
		case CToolOptionInfo::T_CHART_PANDF:			// P&F
		case CToolOptionInfo::T_CHART_REVERSE:			// ���ð�
		case CToolOptionInfo::T_CHART_THREELINEBREAK:	// �Ｑ��ȯ��
		case CToolOptionInfo::T_CHART_BARFORSALE:		// ���Ź�
			// �ܺ����� ���ܽ�Ŵ
			ReleaseCaseSpecialChart(enumToolOption);
			break;
		default:
			// ��ü����, ���̴���Ʈ, ��ü�ִ��ּҺ���, �ܺ����� ���ܽ�Ŵ.
			ReleaseCaseDefault(enumToolOption);
			break;
	}

	return 1;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseCaseDefault
 - Created at  :  2004-11-22   16:21
 - Author      :  ������
 - Description :  �⺻���� ��ư���� ���. ��ü����, ���̴���Ʈ, ��ü�ִ��ּҺ���, �ܺ����ٸ� ���ܽ�Ŵ.
 - UpDaate	   :  (04/11/30) �ϸ����ǥ�� �߰������� ����
 -----------------------------------------------------------------------------------------*/
void CButtonProperty::ReleaseCaseDefault(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	CToolOptionInfo::TOOLOPTION enumButtonToolOption = (CToolOptionInfo::TOOLOPTION)-1;

	if(m_pButtonList == NULL)
		return;
	
	POSITION pos = m_pButtonList->GetHeadPosition();
	while(pos)
	{
		CButtonEx* pButton = m_pButtonList->GetNext(pos);
		if( pButton == NULL) continue;
		// ��ư�� �������ְ�
		if( !pButton->GetState()) continue;

		enumButtonToolOption = pButton->GetToolOptionInfo();
		// �ڱ��ڽ��� �����ϰ�
		if( enumButtonToolOption == enumToolOption) continue;

		switch( enumButtonToolOption)
		{
			case CToolOptionInfo::T_SHOW_OUTERTOOLBAR:	// �ܺ����� ����
			case CToolOptionInfo::T_WHOLEVIEW:			// ��ü���� ����
			case CToolOptionInfo::T_RADAR:				// ���̴���Ʈ ����
			case CToolOptionInfo::T_CHART_BALANCE:		// �ϸ����ǥ ����
			case CToolOptionInfo::T_MINMAX_FULLDATA:	// ��ü�ִ��ּҺ��� ������ ���ɼ� ����.
			// (2006/3/4 - Seung-Won, Bae) Do not release Numerical Inquiry DLG and Cross Line.
			//		Cross Line of Inner Toolbar control NumericalInquiryOption Value.
			case CToolOptionInfo::T_CROSSHAIRS_DRAG:
			case CToolOptionInfo::T_NIDLG_DRAG:
			// (2006/6/29 - Seung-Won, Bae) Over Tools
			case CToolOptionInfo::T_NIDLG_OVER:
			case CToolOptionInfo::T_CROSSHAIRS_OVER:
														break;
			default:	ReleaseToolOption( enumButtonToolOption);
						pButton->SetState(false);				// ��ư���� �ٽ� Invalidate�� ���ְ�...
						pButton->Invalidate();
						break;
		}
	}//end of while(
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseCaseSpecialChart
 - Created at  :  2004-11-22   16:15
 - Author      :  ������
 - Description :  Ư����Ʈ��ư�� Ŭ������ ���. �ܺ����ٸ� ������ ��� ���ɼǸ� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CButtonProperty::ReleaseCaseSpecialChart(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	CButtonEx* pButton = NULL;
	CToolOptionInfo::TOOLOPTION enumButtonToolOption = (CToolOptionInfo::TOOLOPTION)-1;

	if(m_pButtonList == NULL)
		return;

	POSITION pos = m_pButtonList->GetHeadPosition();
	while(pos)
	{
		pButton = m_pButtonList->GetNext(pos);

		enumButtonToolOption = pButton->GetToolOptionInfo();

		if(pButton->GetState() 
			&& enumButtonToolOption != enumToolOption 
			&& enumButtonToolOption != CToolOptionInfo::T_SHOW_OUTERTOOLBAR)
		{
			ReleaseToolOption(enumButtonToolOption);
			pButton->SetState(false);				// ��ư���� �ٽ� Invalidate�� ���ְ�...
			pButton->Invalidate();
		}//end of if(..
	}//end of while(
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseCaseZoom
 - Created at  :  2004-11-22   15:24
 - Author      :  ������
 - Description :  Ȯ��/��ҹ�ư�� �������� ��ưǮ��. 
				  Ȯ��- ��ȣ, ���ǸŸ�, �ְ�����, �����߰�, ���Ϻм�, ������, ���̴���Ʈ
						��ü�ִ��ּҺ���, �ܺ����ٸ� ������ ��� ���ɼ�Ǯ��.
				  ���- ���� �� + ��ü������� ������ ��� ���ɼ� Ǯ��.
 - UpDaate	   :  (04/11/30) �ϸ����ǥ�� �߰������� ����
-----------------------------------------------------------------------------------------*/
void CButtonProperty::ReleaseCaseZoom(CToolOptionInfo::TOOLOPTION enumToolOption, const CString& strParam)
{
	CButtonEx* pButton = NULL;
	CToolOptionInfo::TOOLOPTION enumButtonToolOption = (CToolOptionInfo::TOOLOPTION)-1;

	if(m_pButtonList == NULL)
		return;

	POSITION pos = m_pButtonList->GetHeadPosition();
	while(pos)
	{
		pButton = m_pButtonList->GetNext(pos);
		
		enumButtonToolOption = pButton->GetToolOptionInfo();

		if( pButton->GetState()							// �������ִ� ��ư����	
			&& !IsModalessDialog( enumButtonToolOption) // ��޸���Dlg �����ϰ�
			&& enumButtonToolOption != enumToolOption)	// �ڱ��ڽ� ����,
			switch( enumButtonToolOption)
		{
			case CToolOptionInfo::T_RADAR:						// ���̴���Ʈ ����,
			case CToolOptionInfo::T_MINMAX_FULLDATA:			// ��ü�ִ��ּҺ��� ����,
			case CToolOptionInfo::T_SHOW_OUTERTOOLBAR:			// �ܺ����� ����,
			case CToolOptionInfo::T_CHART_BALANCE:				// �ϸ����ǥ ����
			// (2006/6/23 - Seung-Won, Bae) Except the T_CROSSHAIRS_DRAG, T_NIDLG_DRAG
			case CToolOptionInfo::T_CROSSHAIRS_DRAG:
			case CToolOptionInfo::T_NIDLG_DRAG:
			// (2006/6/29 - Seung-Won, Bae) Over Tools
			case CToolOptionInfo::T_NIDLG_OVER:
			case CToolOptionInfo::T_CROSSHAIRS_OVER:
														break;

			default:	if(strParam == _MTEXT( C4_ZOOM_OUT))	// ��ҹ�ư�̸�
						{
							if(enumButtonToolOption != CToolOptionInfo::T_WHOLEVIEW)	// ��ü���⵵ ������ 
								ReleaseToolOption(enumButtonToolOption);				// ����� ��ư�� Ǭ��.
						}
						else									// ���� Ȯ���ư�϶���
						{
							if(enumButtonToolOption != CToolOptionInfo::T_WHOLEVIEW)
								ReleaseToolOption(enumButtonToolOption);
							else								// ��ü������ ���� ���¸� �����Ų��.
								m_pParent->SetStateWholeView(0);
						}
						pButton->SetState(false);				// ��ư���� �ٽ� Invalidate�� ���ְ�...
						pButton->Invalidate();
		}//end of if(.......) swtich
	}//end of while(
}


/*-----------------------------------------------------------------------------------------
 - Function    :  IsModalessDialog
 - Created at  :  2004-11-22   15:34
 - Author      :  ������
 - Description :  ��ȣDlg, ���ǸŸ�, �ְ�����, �����߰�, ���Ϻм�, ������, ���̴���Ʈ(���⼱ ����),
				  ��ǥġ�м� �� ��޸��� ���̾�α����� Ȯ���Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CButtonProperty::IsModalessDialog(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	switch(enumToolOption)
	{
	case CToolOptionInfo::T_SYMBOL_BULET:			// ��ȣDlg
	case CToolOptionInfo::T_TRADER_REPORT:			// ���ǸŸ�
	case CToolOptionInfo::T_PRICE_FORCAST:			// �ְ�����
	case CToolOptionInfo::T_MAKE_FORMULA:			// �����߰�
	case CToolOptionInfo::T_PATTERN:				// �����Ϻм�
	case CToolOptionInfo::T_MAGNIFYINGGLASS:		// ������
	case CToolOptionInfo::T_RADAR:					// ���̴���Ʈ
	case CToolOptionInfo::T_TARGET_ANALYSIS:		// ��ǥġ�м�
		return true;
	default:
		return false;
	}
}



/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseToolOption
 - Created at  :  2004-11-22   15:45
 - Author      :  ������
 - Description :  �������� ToolOption�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CButtonProperty::ReleaseToolOption(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	m_pParent->ReleaseToolFunction(enumToolOption);
}