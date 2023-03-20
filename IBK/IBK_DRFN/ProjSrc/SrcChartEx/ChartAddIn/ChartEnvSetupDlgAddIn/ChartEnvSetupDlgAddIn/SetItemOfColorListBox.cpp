// SetItemOfColorListBox.cpp: implementation of the CSetItemOfColorListBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "SetItemOfColorListBox.h"

#include "../Include_Chart/DLL_Load/IMetaTable.h"		// for _MTEXT()

//dll�� enum���� ����.
#include "BaseData.h"
#include "FindType.h"

#include "GraphData.h"
#include "ListBoxColorPickerST.h"

#include "Conversion.h"
#include "BlockBaseData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CSetItemOfColorListBox::SetItem( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	ILPCSTR szIndicatorName = p_pIGraphInfo->GetIndicatorNameString();
	CBaseData::COLORLISTITEMTYPE texttype = CFindType::FindColorItemText( szIndicatorName);
	switch(texttype) {
		case CBaseData::PRICE://����
			SetItem_PRICE(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::VOLUME://�ŷ���
			SetItem_VOLUME(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::UP_DOWN:// ���, �϶�
			SetItem_UPDOWN( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::DAEGIMEMUL:// ���Ź�
			SetItem_DAEGIMEMUL( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::COLOR:// ����
			SetItem_COLOR( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::MACD: //macd
			SetItem_MACD(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::PIVOT: //pivot
			SetItem_PIVOT(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::SUBGRAPHNAME://SubGraph�̸�
			SetItem_SUBGRAPHNAME(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::RAINBOW://���ۻ���, �������
			SetItem_RAINBOW(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::SUB_UPFILL_DOWNFILL://SubGraph�̸�, ���ä��, �϶�ä��
			SetItem_SUB_UPFILL_DOWNFILL( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::GLANCEBLANCE: //��ȯ, ����, ����, ����, ������ä��, �϶�ä��.
			SetItem_GLANCEBLANCE(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::INVERSELINE: //���ð�
			SetItem_INVERSELINE(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CBaseData::NONE:
			SetItembyStyleandType(true, p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
	}
}

int CSetItemOfColorListBox::GetVolumeType(IGraphInfo* p_pIGraphInfo)
{
	//test -
	IDoubleList *pIConditionList = p_pIGraphInfo->GetDrawConditionList();
	if( !pIConditionList) return -1;
	double dType = -1;
	pIConditionList->GetHead( dType);
	pIConditionList->Release();
	return ( int)dType;
}

bool CSetItemOfColorListBox::IsLineChart( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	if( p_nSubGraphIndex < 0) return false;
	if( !p_pIGraphInfo) return false;

	return ( p_pIGraphInfo->GetSubGraphStyle( p_nSubGraphIndex) == 2); //���ν�
}

//������Ʈ
void CSetItemOfColorListBox::SetItem_PRICE(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	//test -
	if(IsLineChart( p_pIGraphInfo, p_nSubGraphIndex))
		SetItem_COLOR( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
	else { 
		SetItem_UPDOWN( p_pIGraphInfo, p_nSubGraphIndex, pListBox);

		IDoubleList *pIConditionList = p_pIGraphInfo->GetCommonCalculateConditionList();
		if( pIConditionList)
		{
			double dType = -1;
			pIConditionList->GetHead( dType);
			pIConditionList->Release();
			if( ( int)dType != 2) SetItem_EQUAL( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
		}
	}
}
//�ŷ���
void CSetItemOfColorListBox::SetItem_VOLUME(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	//test -
	if( p_nSubGraphIndex < 0) return;
	if( p_pIGraphInfo == NULL) return;

// 2008.01.25 by LYH >>
	CString strColor;
	strColor.LoadString(IDS_STRING_COLOR);
// 2008.01.25 by LYH <<
	//�Ϲ�
	if(GetVolumeType(p_pIGraphInfo) == 1)
		//sy 2004.05.11. -> �Ϲݰŷ��� ���� 1��°�� �Ѵ�.
// 2008.01.25 by LYH >>
		//pListBox->AddString( _T("����"), p_pIGraphInfo->GetSubGraphColor( 0, 1));
		pListBox->AddString( strColor, p_pIGraphInfo->GetSubGraphColor( 0, 1));
// 2008.01.25 by LYH <<
	else if(GetVolumeType(p_pIGraphInfo) == 2) //���, �϶�
		SetItem_UPDOWN(	p_pIGraphInfo, p_nSubGraphIndex, pListBox);
	else if(GetVolumeType(p_pIGraphInfo) == 3) { //ĵ����� ����
		SetItem_UPDOWN(	p_pIGraphInfo, p_nSubGraphIndex, pListBox);
		SetItem_EQUAL(	p_pIGraphInfo, p_nSubGraphIndex, pListBox);
	}
}

//���, �϶� �� ����
void CSetItemOfColorListBox::SetItem_UPDOWN( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
// 2008.01.25 by LYH >>
	//pListBox->AddString(_T("���"), p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
	//pListBox->AddString(_T("�϶�"), p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 3));
	CString strUp, strDown;
	strUp.LoadString(IDS_STRING_UP);
	strUp.LoadString(IDS_STRING_DOWN);
	pListBox->AddString(strUp, p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
	pListBox->AddString(strDown, p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 3));
// 2008.01.25 by LYH <<
}

//���ջ� ����
void CSetItemOfColorListBox::SetItem_EQUAL( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
// 2008.01.25 by LYH >>
	//pListBox->AddString(_T("����"), p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 5));
	CString strSteady;
	strSteady.LoadString(IDS_STRING_STEADY);
	pListBox->AddString(strSteady, p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 5));
// 2008.01.25 by LYH <<
}

//���Ź�
void CSetItemOfColorListBox::SetItem_DAEGIMEMUL( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	if( p_nSubGraphIndex < 0 || p_pIGraphInfo->GetSubGraphCount() <= p_nSubGraphIndex) return;

	ILPCSTR szGraphName = p_pIGraphInfo->GetSubGraphNameString( p_nSubGraphIndex);
	if( !strcmp( szGraphName, _MTEXT( C3_OVERHANGING_SUPPLY))) 
	{
		//sy 2004.05.13. -> ���Ź��� ���� Text�� ���� �����ϰ� ó����.
		// ���� : �⺻�׵θ�, �⺻��, �ְ�ŷ� �׵θ�, ���簡 �׵θ�, ���簡 ��
		//2008.01.31 by LYH ��� ����
		pListBox->AddString("�⺻�Ź�",		p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
		pListBox->AddString("�⺻�Ź�Text",	p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 2));
		pListBox->AddString("����Ź�",		p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 3));
		pListBox->AddString("����Ź�Text", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 4));
		pListBox->AddString("�ִ�Ź�",		p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 5));
		pListBox->AddString("�ִ�Ź�Text", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 6));
	}
	//sy 2004.05.12. : ������ Ư����Ʈ�� ������ <������Ʈ>�� ÷�εǴ� ��Ʈ�� ������.
	/*else if(p_nSubGraphIndex->GetName().CompareNoCase("����") == 0 ||
		    p_nSubGraphIndex->GetName().CompareNoCase("ü�ᰡ") == 0)  {
		int pSub = p_pIGraphInfo->GetAt(1);
		if(pSub == NULL)
			return;

		if(IsLineChart(p_nSubGraphIndex))
			pListBox->AddString(_T("����"), pSub->GetColor().GetColor1());
		else {
			pListBox->AddString(_T("���"), pSub->GetColor().GetColor1());
			pListBox->AddString(_T("�϶�"), pSub->GetColor().GetColor3());
		}
	}*/
}
//������ String���� �� �� ����
void CSetItemOfColorListBox::SetItem_COLOR( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
// 2008.01.25 by LYH >>
	CString strColor;
	strColor.LoadString(IDS_STRING_COLOR);
	//pListBox->AddString("����", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
	pListBox->AddString(strColor, p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
// 2008.01.25 by LYH <<
}

//��ǥ���� �� ����
void CSetItemOfColorListBox::SetItem_SUBGRAPHNAME(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	ILPCSTR szGraphName = NULL;
	for(int i = 0 ; i< p_pIGraphInfo->GetSubGraphCount() ; i++)
	{
		szGraphName = p_pIGraphInfo->GetSubGraphNameString( i);
		pListBox->AddString( szGraphName, p_pIGraphInfo->GetSubGraphColor( i, 1));
	}
}

//���κ���
void CSetItemOfColorListBox::SetItem_RAINBOW( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	pListBox->AddString("���ۻ���", p_pIGraphInfo->GetSubGraphColor( 0, 1));
	pListBox->AddString("�������", p_pIGraphInfo->GetSubGraphColor( 0, 2));
}
//��ǥ, ���ä��, �϶�ä��.
void CSetItemOfColorListBox::SetItem_SUB_UPFILL_DOWNFILL( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	ILPCSTR szGraphName = p_pIGraphInfo->GetSubGraphNameString( p_nSubGraphIndex);
	pListBox->AddString( szGraphName,	p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 5));
	pListBox->AddString( "���ä��",	p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 2));
	pListBox->AddString( "�϶�ä��",	p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 4));
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetItem_SUB_UPFILL_DOWNFILL
 - Created at  :  2004-10-04   14:30
 - Author      :  ������
 - Description :  ����;ä���;�� �Լ� (����׷������ UP/DOWNä�������� �����Ѵ�.)
 -----------------------------------------------------------------------------------------*/
void CSetItemOfColorListBox::SetItem_SUB_UPFILL_DOWNFILL2( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
#ifdef _DEBUG
	AfxMessageBox( "What is this?");
#endif
/*
	for(int i = 0; i < p_pIGraphInfo->GetSubGraphCount(); i++)
	{
		// �Լ��Ķ���ͷ� ���� SubGraphData�� ���� ���̸� ����׷���, ���ä��, �϶�ä����
		// ListBox�� �ִ´�.
		if(pTmp_nSubGraphIndex->GetName() == p_nSubGraphIndex->GetName())
		{	
			pListBox->AddString(p_nSubGraphIndex->GetName(), p_nSubGraphIndex->GetColor().GetColor5());
			pListBox->AddString(_T("���ä��"), p_nSubGraphIndex->GetColor().GetColor2());
			pListBox->AddString(_T("�϶�ä��"), p_nSubGraphIndex->GetColor().GetColor4());
		}
		else
		{
			pListBox->AddString((p_pIGraphInfo->GetSubGraphData(i))->GetName(), (p_pIGraphInfo->GetSubGraphData(i))->GetColor().GetColor1());
		}
	}
*/
}


//��ȯ,����, ����, ����, ��±�����, �϶�������.
void CSetItemOfColorListBox::SetItem_GLANCEBLANCE(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	ILPCSTR szSubGraphName = NULL;
	for( int i = 0 ; i <  p_pIGraphInfo->GetSubGraphCount() ; i++)
	{
		szSubGraphName = p_pIGraphInfo->GetSubGraphNameString( i);
		if( !strcmp( szSubGraphName, "���ེ��"))
		{
			pListBox->AddString( "��±�����", p_pIGraphInfo->GetSubGraphColor( i, 2));
			pListBox->AddString( "�϶�������", p_pIGraphInfo->GetSubGraphColor( i, 4));
		}
		else pListBox->AddString( szSubGraphName, p_pIGraphInfo->GetSubGraphColor( i, 1));
	}
/*
		if(subName.CompareNoCase("��ȯ��") == 0)
			pListBox->AddString("��ȯ��", pSub->GetColor().GetColor1());
		else if(subName.CompareNoCase("���ؼ�") == 0)
			pListBox->AddString("���ؼ�", pSub->GetColor().GetColor1());
		else if(subName.CompareNoCase("���ེ��") == 0)
			pListBox->AddString("���ེ��", pSub->GetColor().GetColor1());

*/
/*
	int pSub = p_pIGraphInfo->GetAt(0);
	if(pSub != NULL)
		pListBox->AddString("��ȯ��", pSub->GetColor().GetColor1());
	
	pSub = p_pIGraphInfo->GetAt(1);
	if(pSub != NULL) 
		pListBox->AddString("���ؼ�", pSub->GetColor().GetColor1());

	pSub = p_pIGraphInfo->GetAt(2);
	if(pSub != NULL) 
		pListBox->AddString("���ེ��", pSub->GetColor().GetColor1());

	pSub = p_pIGraphInfo->GetAt(3);
	if(pSub != NULL) {
		pListBox->AddString("��±�����", pSub->GetColor().GetColor2());
		pListBox->AddString("�϶�������", pSub->GetColor().GetColor4());
	}
*/
}

//InverseLine
void CSetItemOfColorListBox::SetItem_INVERSELINE(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	pListBox->AddString("���ð�1", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 1));
	pListBox->AddString("���ð�2", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 2));
	pListBox->AddString("���ð�3", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 3));
	pListBox->AddString("���ð�4", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 4));
	pListBox->AddString("���ð�5", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 5));
	pListBox->AddString("���ð�6", p_pIGraphInfo->GetSubGraphColor( p_nSubGraphIndex, 6));
}

void CSetItemOfColorListBox::OnApply(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	//sy 2004.05.12.
	if( p_nSubGraphIndex < 0) return;
	if(pListBox == NULL || p_pIGraphInfo == NULL) return;

	ILPCSTR szIndicatorName = p_pIGraphInfo->GetIndicatorNameString();
	CBaseData::COLORLISTITEMTYPE texttype = CFindType::FindColorItemText( szIndicatorName);
	switch(texttype) {
		case CBaseData::PRICE: //����
			OnApply_PRICE(pListBox, p_pIGraphInfo, p_nSubGraphIndex);
			break;
		case CBaseData::VOLUME:  //�ŷ���
			OnApply_VOLUME(pListBox, p_pIGraphInfo, p_nSubGraphIndex);
			break;
		case CBaseData::UP_DOWN: // ���, �϶�
			OnApply_UPDOWN( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
		case CBaseData::SUBGRAPHNAME://SubGraph�̸�
		case CBaseData::PIVOT: //pivot
			OnApply_SUBGRAPHNAME(pListBox, p_pIGraphInfo, p_nSubGraphIndex);
			break;
		case CBaseData::MACD: //macd
			OnApply_MACD(pListBox, p_pIGraphInfo, p_nSubGraphIndex);
			break;
		case CBaseData::COLOR: // ����
			OnApply_COLOR( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
		case CBaseData::DAEGIMEMUL: // ���Ź�
			OnApply_DAEGIMEMUL(pListBox, p_pIGraphInfo, p_nSubGraphIndex);
			break;
		case CBaseData::RAINBOW: //���ۻ���, �������
			OnApply_RAINBOW(pListBox, p_pIGraphInfo, p_nSubGraphIndex);
			break;
		case CBaseData::SUB_UPFILL_DOWNFILL: //SubGraph�̸�, ���ä��, �϶�ä��
			OnApply_SUB_UPFILL_DOWNFILL( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
		case CBaseData::GLANCEBLANCE: //��ȯ, ����, ����, ����, ������ä��, �϶�ä��.
			OnApply_GLANCEBLANCE(pListBox, p_pIGraphInfo, p_nSubGraphIndex);
			break;
		case CBaseData::INVERSELINE: //���ð�
			OnApply_INVERSELINE(pListBox, p_pIGraphInfo, p_nSubGraphIndex);
			break;
		case CBaseData::NONE:
			SetItembyStyleandType(false, p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
	}
}

void CSetItemOfColorListBox::OnApply_PRICE(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	OnApply_upcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(0));
	if(!IsLineChart( p_pIGraphInfo, p_nSubGraphIndex)) { 
		OnApply_downcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(1));

		IDoubleList *pIConditionList = p_pIGraphInfo->GetDrawConditionList();
		if( pIConditionList)
		{
			double dType = -1;
			pIConditionList->GetHead( dType);
			pIConditionList->Release();
			if( ( int)dType == 1) OnApply_equalcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor( 2));
		}
	}
}

void CSetItemOfColorListBox::OnApply_VOLUME(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	//�Ϲݰŷ���
	if(GetVolumeType(p_pIGraphInfo) == 1) { 
		//sy 2004.05.11. -> �Ϲݰŷ��� ���� 1��°�� �Ѵ�.
		OnApply_upcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(0));
		//p_nSubGraphIndex->GetColor().SetColor5(pListBox->GetColor(0));
		//p_nSubGraphIndex->GetColor().SetColor6(pListBox->GetColor(0));
		//sy end
	}
	else if(GetVolumeType(p_pIGraphInfo) == 2) { //���, �϶�
		OnApply_upcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(0));
		OnApply_downcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(1));
	}
	else if(GetVolumeType(p_pIGraphInfo) == 3) { //ĵ����� ����
		OnApply_upcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(0));
		OnApply_downcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(1));
		OnApply_equalcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(2));
	}
}

void CSetItemOfColorListBox::OnApply_UPDOWN( IGraphInfo* p_pIGraphInfo, CListBoxColorPickerST *pListBox, int p_nSubGraphIndex)
{
	OnApply_upcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(0));
	OnApply_downcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(1));
}

void CSetItemOfColorListBox::OnApply_SUBGRAPHNAME(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	for(int i = 0 ; i< p_pIGraphInfo->GetSubGraphCount(); i++) OnApply_upcolor( p_pIGraphInfo, i, pListBox->GetColor(i));
}

void CSetItemOfColorListBox::OnApply_COLOR( IGraphInfo* p_pIGraphInfo, CListBoxColorPickerST *pListBox, int p_nSubGraphIndex)
{
	OnApply_upcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(0));
}

void CSetItemOfColorListBox::OnApply_RAINBOW(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 1, pListBox->GetColor( 0));
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 2, pListBox->GetColor( 1));
}

void CSetItemOfColorListBox::OnApply_SUB_UPFILL_DOWNFILL( IGraphInfo* p_pIGraphInfo, CListBoxColorPickerST *pListBox, int p_nSubGraphIndex)
{
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 5, pListBox->GetColor( 0));
	OnApply_upcolor(	p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor( 1));
	OnApply_downcolor(	p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor( 2));
}

/*-----------------------------------------------------------------------------------------
 - Function    :  OnApply_SUB_UPFILL_DOWNFILL
 - Created at  :  2004-10-04   14:42
 - Author      :  ������
 - Description :  ����;ä���; �϶��� ��ǥ������ �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CSetItemOfColorListBox::OnApply_SUB_UPFILL_DOWNFILL2( IGraphInfo* p_pIGraphInfo, CListBoxColorPickerST *pListBox, int p_nSubGraphIndex)
{
#ifdef _DEBUG
	AfxMessageBox( "What is it?");
#endif
/*
	for(int i = 0; i < p_pIGraphInfo->GetSubGraphCount(); i++)
	{
		int pTmp_nSubGraphIndex = p_pIGraphInfo->GetSubGraphData(i);

		if(pTmp_nSubGraphIndex->GetName() == p_nSubGraphIndex->GetName())
		{
			p_nSubGraphIndex->GetColor().SetColor5(pListBox->GetColor(0));
			OnApply_upcolor(p_nSubGraphIndex, pListBox->GetColor(1));
			OnApply_downcolor(p_nSubGraphIndex, pListBox->GetColor(2));
		}
		else
			OnApply_upcolor(p_pIGraphInfo->GetSubGraphData(i), pListBox->GetColor(i+2));
	}
*/
}

void CSetItemOfColorListBox::OnApply_GLANCEBLANCE(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	ILPCSTR szSubGraphName = NULL;
	for(int i = 0 ; i< p_pIGraphInfo->GetSubGraphCount() ; i++)
	{
		szSubGraphName = p_pIGraphInfo->GetSubGraphNameString( i);
		OnApply_upcolor( p_pIGraphInfo, i, pListBox->GetColor(i));
		if( !strcmp( szSubGraphName, "���ེ��")) OnApply_downcolor( p_pIGraphInfo, i, pListBox->GetColor(i+1));
	}
/*
	int pSub = p_pIGraphInfo->GetAt(0);
	if(pSub != NULL)
		OnApply_upcolor(pSub, pListBox->GetColor(0));

	pSub = p_pIGraphInfo->GetAt(1);
	if(pSub != NULL)
		OnApply_upcolor(pSub, pListBox->GetColor(1));

	pSub = p_pIGraphInfo->GetAt(2);
	if(pSub != NULL)
		OnApply_upcolor(pSub, pListBox->GetColor(2));

	pSub = p_pIGraphInfo->GetAt(3);
	if(pSub != NULL) {
		OnApply_upcolor(pSub, pListBox->GetColor(3));
		OnApply_downcolor(pSub, pListBox->GetColor(4));
	}
*/
}

void CSetItemOfColorListBox::OnApply_MACD( CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	ILPCSTR szSubGraphName = NULL;
	for(int i = 0 ; i< p_pIGraphInfo->GetSubGraphCount(); i++)
	{
		szSubGraphName = p_pIGraphInfo->GetSubGraphNameString( i);
		if( !strcmp( szSubGraphName, "Oscillator"))
		{
			p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 5, pListBox->GetColor( i));
			OnApply_upcolor(	p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(i + 1));
			OnApply_downcolor(	p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(i + 2));
		}
		else OnApply_upcolor(	p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(i));
	}
}

void CSetItemOfColorListBox::OnApply_upcolor( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, const COLORREF& color)
{
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 1, color);
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 2, color);
}

void CSetItemOfColorListBox::OnApply_downcolor( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, const COLORREF& color)
{
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 3, color);
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 4, color);
}

void CSetItemOfColorListBox::OnApply_equalcolor( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, const COLORREF& color)
{
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 5, color);
	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, 6, color);
}

void CSetItemOfColorListBox::OnApply_DAEGIMEMUL(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	ILPCSTR szSubGraphName = p_pIGraphInfo->GetSubGraphNameString( p_nSubGraphIndex);
	if( !strcmp( szSubGraphName, _MTEXT( C3_OVERHANGING_SUPPLY)))
		p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, pListBox->GetColor(0), pListBox->GetColor(1), pListBox->GetColor(2), pListBox->GetColor(3),
			pListBox->GetColor(4), pListBox->GetColor(5));
}

void CSetItemOfColorListBox::OnApply_INVERSELINE(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex)
{
	if( p_nSubGraphIndex < 0) return;
	if( pListBox == NULL) return;

	p_pIGraphInfo->SetSubGraphColor( p_nSubGraphIndex, pListBox->GetColor(0), pListBox->GetColor(1), pListBox->GetColor(2), pListBox->GetColor(3), 
		pListBox->GetColor(4), pListBox->GetColor(5));
}

void CSetItemOfColorListBox::SetItem_PIVOT(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST *pListBox)
{
	ILPCSTR szSubGraphName = NULL;
	for(int i = 0 ; i< p_pIGraphInfo->GetSubGraphCount() ; i++)
	{
		szSubGraphName = p_pIGraphInfo->GetSubGraphNameString( i);
		if( !strcmp( szSubGraphName, "R1"))			pListBox->AddString("1�� ������",	p_pIGraphInfo->GetSubGraphColor( i, 1));
		else if( !strcmp( szSubGraphName, "R2"))	pListBox->AddString("2�� ������",	p_pIGraphInfo->GetSubGraphColor( i, 1));
		else if( !strcmp( szSubGraphName, "S1"))		pListBox->AddString("1�� ���׼�",	p_pIGraphInfo->GetSubGraphColor( i, 1));
		else if( !strcmp( szSubGraphName, "S2"))	pListBox->AddString("2�� ���׼�",	p_pIGraphInfo->GetSubGraphColor( i, 1));
		else if( !strcmp( szSubGraphName, "PIVOT"))	pListBox->AddString("�Ǻ�",			p_pIGraphInfo->GetSubGraphColor( i, 1));
	}
}

void CSetItemOfColorListBox::SetItem_MACD(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST *pListBox)
{
	ILPCSTR szSubGraphName = NULL;
	for(int i = 0 ; i< p_pIGraphInfo->GetSubGraphCount(); i++)
	{
		szSubGraphName = p_pIGraphInfo->GetSubGraphNameString( i);
		if( strcmp( szSubGraphName, "Oscillator")) pListBox->AddString( szSubGraphName, p_pIGraphInfo->GetSubGraphColor( i, 1));
		else SetItem_SUB_UPFILL_DOWNFILL( p_pIGraphInfo, i, pListBox);
	}
}

//subgraph�� �ϳ��� ��츸 ����.
//�������� ���� ȭ�鿡..��� subgraph�� color�� ���;� �ϱ⶧����
//�̷��Դ� �Ұ���.
void CSetItemOfColorListBox::SetItembyStyleandType(const bool bSetUp, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	if( p_nSubGraphIndex < 0) return;
	
	switch( p_pIGraphInfo->GetSubGraphType( p_nSubGraphIndex))
	{
		case CGraphBaseData::Bong_Type:
			InCaseOfStylsIsBong( p_pIGraphInfo, bSetUp, p_nSubGraphIndex, pListBox);
			break;
		case CGraphBaseData::Line_Type:
			// ����(04/10/04) ����;ä����; ����
			InCaseOfStylsIsLine(bSetUp, p_nSubGraphIndex, pListBox, p_pIGraphInfo);
			break;
		case CGraphBaseData::Bar_Type:
			InCaseOfStylsIsBar(bSetUp, p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		default:
			InCaseOfStylsIsLine(bSetUp, p_nSubGraphIndex, pListBox, p_pIGraphInfo);
			break;
	}
}

//����;�Ϻ��� 
void CSetItemOfColorListBox::InCaseOfStylsIsBong( IGraphInfo* p_pIGraphInfo, const bool bSetUp, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	if( p_nSubGraphIndex < 0) return;

	switch( p_pIGraphInfo->GetSubGraphStyle( p_nSubGraphIndex))
	{
		case CGraphBaseData::CandlestickBong:
// (2006.10.30 �Ƚ�ȯ) ����Ʈ(��'��'��)�߰�		case CGraphBaseData::HighLowCloseBong:
		case CGraphBaseData::OpenHighLowCloseBong:
			InCaseOfTypeIsCandlestickandHighLowCloseBong( p_pIGraphInfo, bSetUp, p_nSubGraphIndex, pListBox);
			break;
		case CGraphBaseData::LineBong:
			InCaseOfTypeIsLineBong( p_pIGraphInfo, bSetUp, p_nSubGraphIndex, pListBox);
			break;
	}
}

void CSetItemOfColorListBox::InCaseOfTypeIsCandlestickandHighLowCloseBong( IGraphInfo* p_pIGraphInfo, const bool bSetUp, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	if( bSetUp) SetItem_UPDOWN( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
	else
	{
		OnApply_upcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(0));
		OnApply_downcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(1));
	}
}

void CSetItemOfColorListBox::InCaseOfTypeIsLineBong( IGraphInfo* p_pIGraphInfo, const bool bSetUp, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	if(bSetUp)
		SetItem_COLOR( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
	else
		OnApply_upcolor( p_pIGraphInfo, p_nSubGraphIndex, pListBox->GetColor(0));
}

//����;�Ϲݽ� ����;���κ���� ����;0���񿬼ӽ� ����;����������
//����;��ܽ� ����;�������׽� ����;ä��� ����;�������׽�
void CSetItemOfColorListBox::InCaseOfStylsIsLine(const bool bSetUp, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox, IGraphInfo* p_pIGraphInfo)
{
	if(bSetUp)
		SetItemInCaseOfStylsIsLine(p_nSubGraphIndex, pListBox, p_pIGraphInfo);
	else 
		OnApplyInCaseOfStylsIsLine(p_nSubGraphIndex, pListBox, p_pIGraphInfo);
//		case CGraphBaseData::HorizontalLine:// �����
//		case CGraphBaseData::StairLine:// ��ܽ�
//		case CGraphBaseData::ComparisonLine:	// ����
//		case CGraphBaseData::CloudLine:// ������
//		case CGraphBaseData::DataDotSimpleLine:	// ��ǥ���Ϲ�
//      case CGraphBaseData::RainbowLine:	// ���κ����
}


void CSetItemOfColorListBox::OnApplyInCaseOfStylsIsLine(int p_nSubGraphIndex, CListBoxColorPickerST* pListBox, IGraphInfo* p_pIGraphInfo)
{
	if( p_nSubGraphIndex < 0) return;

	switch( p_pIGraphInfo->GetSubGraphStyle( p_nSubGraphIndex))
	{
		case CGraphBaseData::SimpleLine:// �Ϲݽ�
		case CGraphBaseData::ZigZagLine:	// ������׽�
		case CGraphBaseData::NonZeroLine:// 0���񿬼ӽ� (���� "0"�� ���� �׸��� ����)
			OnApply_COLOR( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
		case CGraphBaseData::FillLine:// ä���
			// ����(04/10/04) ����;ä����ϰ��� ����׷����� �� ������ �ִ�.
			OnApply_SUB_UPFILL_DOWNFILL2( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
		case CGraphBaseData::LineCloudLine:	//���� ������
		case CGraphBaseData::CloudLine:		//������
			OnApply_UPDOWN( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
		default:
			OnApply_COLOR( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
	}
}

void CSetItemOfColorListBox::SetItemInCaseOfStylsIsLine(int p_nSubGraphIndex, CListBoxColorPickerST* pListBox, IGraphInfo* p_pIGraphInfo)
{
	if( p_nSubGraphIndex < 0) return;

	switch( p_pIGraphInfo->GetSubGraphStyle( p_nSubGraphIndex))
	{
		case CGraphBaseData::SimpleLine:// �Ϲݽ�
		case CGraphBaseData::ZigZagLine:	// ������׽�
		case CGraphBaseData::NonZeroLine:// 0���񿬼ӽ� (���� "0"�� ���� �׸��� ����)
			SetItem_COLOR( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CGraphBaseData::FillLine:// ä���
			// ����(04/10/04) ����;ä����ϰ��� ����׷����� �� ������ �ִ�.
			SetItem_SUB_UPFILL_DOWNFILL2( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CGraphBaseData::LineCloudLine:	// ���� ������
		case CGraphBaseData::CloudLine:		//������
			SetItem_UPDOWN( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		default:
			SetItem_COLOR( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
	}
}


//����;�ڽ��� ����;��Ʈ�� 
//����;���ϵ����   
//����� �ȵ����µ�..
void CSetItemOfColorListBox::InCaseOfStylsIsBar(const bool bSetUp, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	if(bSetUp)
		SetItemInCaseOfStylsIsBar(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
	else 
		OnApplyInCaseOfStylsIsBar(p_pIGraphInfo, p_nSubGraphIndex, pListBox);
//      case CGraphBaseData::HorizontalBar:	// �����
//      case CGraphBaseData::UpDownBar:	// ���Ͻ�
//      case CGraphBaseData::CombBar:	// ������
//      case CGraphBaseData::LeftRightBar:	// �¿��
//      case CGraphBaseData::LeftRightCombBar:	// �¿������
//      case CGraphBaseData::GroupBar:	// �׷�ٽ�
//      case CGraphBaseData::RatioBar:	// ������ (ex- �ż�/�ŵ� �ŷ��� ǥ����)
//      case CGraphBaseData::TextHorizontalBar:	// Text�� �ִ� �����
//      case CGraphBaseData::TextGroupBar:	// Text�� �ִ� �׷� �ٽ�
//      case CGraphBaseData::FixedRateLeftRightBar:	// ���������¿��
}

void CSetItemOfColorListBox::OnApplyInCaseOfStylsIsBar(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	if( p_nSubGraphIndex < 0) return;

	switch( p_pIGraphInfo->GetSubGraphStyle( p_nSubGraphIndex))
	{
		case CGraphBaseData::VerticalBar:// ������
			{
				BOOL bApplyed = FALSE;
				IDoubleList *pIConditionList = p_pIGraphInfo->GetCommonCalculateConditionList();
				if( pIConditionList)
				{
					if( pIConditionList->GetCount())
					{
						OnApply_UPDOWN( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
						bApplyed = TRUE;
					}
					pIConditionList->Release();
				}
				if( !bApplyed) OnApply_COLOR( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			}
			break;
		case CGraphBaseData::BoxBar:	// �ڽ���
		case CGraphBaseData::DotBar:	// ��Ʈ��
			OnApply_UPDOWN( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
		case CGraphBaseData::UpDownHighLowBar:	// ���ϵ����
			OnApply_SUB_UPFILL_DOWNFILL( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
		default:
			OnApply_COLOR( p_pIGraphInfo, pListBox, p_nSubGraphIndex);
			break;
	}

}

void CSetItemOfColorListBox::SetItemInCaseOfStylsIsBar(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox)
{
	if( p_nSubGraphIndex < 0) return;

	switch( p_pIGraphInfo->GetSubGraphStyle( p_nSubGraphIndex))
	{
		case CGraphBaseData::VerticalBar:// ������
		{
			//����϶� ǥ��
			BOOL bSetted = FALSE;
			IDoubleList *pIConditionList = p_pIGraphInfo->GetCommonCalculateConditionList();
			if( pIConditionList)
			{
				if( pIConditionList->GetCount())
				{
					SetItem_UPDOWN( p_pIGraphInfo, p_nSubGraphIndex, pListBox);						
					bSetted = TRUE;
				}
				pIConditionList->Release();
			}
			if( !bSetted) SetItem_COLOR( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		}
		case CGraphBaseData::BoxBar:	// �ڽ���
		case CGraphBaseData::DotBar:	// ��Ʈ��
			SetItem_UPDOWN( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		case CGraphBaseData::UpDownHighLowBar:	// ���ϵ����
			SetItem_SUB_UPFILL_DOWNFILL( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
		default:
			SetItem_COLOR( p_pIGraphInfo, p_nSubGraphIndex, pListBox);
			break;
	}
}


