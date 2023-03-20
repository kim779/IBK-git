// CodeDataFromInputBar.cpp: implementation of the CCodeDataFromInputBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CodeDataFromInputBar.h"
#include "./define/ChartMsgDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCodeDataFromInputBar::CCodeDataFromInputBar()
{
	m_lEndTime = 0;
	m_chType = NULL;
	m_lTypeNum = 1;	
	m_lDataCnt = 0;
	m_lPtrST = 0;

	m_lDispCnt = 120;
	m_lRequestCnt = 400;

	m_nLineType = 0;
	m_bTodayRequest = FALSE;

}

CCodeDataFromInputBar::~CCodeDataFromInputBar()
{

}

CShareInputData::CShareInputData()
{
	m_DataInputBar.m_chType = '1';
	m_DataInputBar.m_lTypeNum = 1;
	m_DataInputBar.m_cChartShape = 'A';
	m_DataInputBar.m_cDataGubun = '0';

	m_DataInputBar.m_lEndTime;//
	m_bCanDel = FALSE;
}

void CShareInputData::ReInit()
{
	m_DataInputBar.m_chType = '1';	//��
	m_DataInputBar.m_lTypeNum = 1;	
	m_DataInputBar.m_lDispCnt = 120;
	m_DataInputBar.m_lRequestCnt = 400;

	m_DataInputBar.m_nLineType = 0;
	m_DataInputBar.m_bTodayRequest = FALSE;
	m_DataInputBar.m_cChartShape = 'A';
	m_DataInputBar.m_nUnitType = WON_TYPE;
	m_DataInputBar.m_cDataGubun = '0';
	m_DataInputBar.m_bTodayRequest = FALSE;

	m_bCanDel = FALSE;
}

//-->@.080617-001 Defaultó��
void CDlgCompDataHandle::DoCheckFirst()
{
	CString szSect("Fluctuation");
	CString szVersion;
	szVersion.GetBuffer(MAX_PATH);
	::GetPrivateProfileString(szSect, "version", "", (LPSTR)(LPCSTR)szVersion, MAX_PATH, m_szDataFile);
	szVersion.ReleaseBuffer();

	if(szVersion.CompareNoCase("0806171")<0)
	{
		WritePrivateProfileString(szSect, "nGroupCnt"	, "1", m_szDataFile);
		WritePrivateProfileString(szSect, "SelectGroup"	, "�⺻�׷�", m_szDataFile);
		WritePrivateProfileString(szSect, "bShowAll"	, "1", m_szDataFile);

		szSect = "FGroup00";
		WritePrivateProfileString(szSect, "nItemCnt"	, "2"		, m_szDataFile);
		WritePrivateProfileString(szSect, "szGroupName"	, "�⺻�׷�", m_szDataFile);

		WritePrivateProfileString(szSect, "szCode00"	, "001"		, m_szDataFile);
		WritePrivateProfileString(szSect, "szCodeName00", "����"	, m_szDataFile);
		WritePrivateProfileString(szSect, "clr00"		, "255"		, m_szDataFile);

		WritePrivateProfileString(szSect, "szCode01"	, "301"			, m_szDataFile);
		WritePrivateProfileString(szSect, "szCodeName01", "�ڽ�������"	, m_szDataFile);
		WritePrivateProfileString(szSect, "clr01"		, "16711680"	, m_szDataFile);
	}
}
//<--

//[Fluctuation]
//nBase = 0
//nTimer = 5
//bCurDisp = 1
//nTypeUnit = 1
//nGroupCnt = 0

void CDlgCompDataHandle::LoadData(long pDataKey)
{
	STDlgCompData* pSTData = (STDlgCompData*)pDataKey;

	CString szSect("Fluctuation");
	char szTemp[MAX_PATH]={0,};

	//�� ���ذ�. 0:��������,1:���Ͻð�,1:���簡
	pSTData->m_nBase = ::GetPrivateProfileInt(szSect, "nBase", 0, m_szDataFile);

	// ��������(��������)
	pSTData->m_nTimer = ::GetPrivateProfileInt(szSect, "nTimer", 5, m_szDataFile);
	
	//���簡(����)ǥ�� ����
	pSTData->m_bCurDisp = ::GetPrivateProfileInt(szSect, "bCurDisp", 1, m_szDataFile);

	// ��к�����.
	pSTData->m_nTypeUnit = ::GetPrivateProfileInt(szSect, "nTypeUnit", 1, m_szDataFile);

	// SelectGroup
	::GetPrivateProfileString(szSect, "SelectGroup", "", szTemp, MAX_PATH, m_szDataFile);
	pSTData->m_szSelectGroup = szTemp;

	// SelectCode, �������񺸱��̰ų� ���������϶� ��ȿ
	::GetPrivateProfileString(szSect, "szSelCode", "", szTemp, MAX_PATH, m_szDataFile);
	pSTData->m_szSelCode = szTemp;

	// �������� �⺻�׷�(1)/��������(0)
	// �������⸦ ����ϸ� �������񺸱�� ���õ�.
	// �޴����� ���ý� ���������� ������ ����Ǿ�� ��.
	pSTData->m_bShowAll = ::GetPrivateProfileInt(szSect, "bShowAll", 0, m_szDataFile);

	//�������� ���
	// �޴����� ���ý� ���������� ������ ����Ǿ�� ��.
	pSTData->m_bTimerUse = ::GetPrivateProfileInt(szSect, "bTimerUse", 0, m_szDataFile);
	if(pSTData->m_nTimer<=0)
		pSTData->m_bTimerUse = 0; //�������� ���̻��ϸ� �������� 0���� ������.

	// GroupCount
	pSTData->m_nGroupCnt = ::GetPrivateProfileInt(szSect, "nGroupCnt", 0, m_szDataFile);

	CString szKey;
	int nItemCnt=0;
	for(int i=0; i<pSTData->m_nGroupCnt; i++)
	{
		CDlgCompGroupData* pGroupData = new CDlgCompGroupData;

		szKey.Format("FGroup%02d", i);
		szSect = szKey;
		nItemCnt = ::GetPrivateProfileInt(szSect, "nItemCnt", 0, m_szDataFile);

		for(int j=0; j<nItemCnt; j++)
		{
			::GetPrivateProfileString(szSect, "szGroupName", "", szTemp, MAX_PATH, m_szDataFile);
			pGroupData->m_szGroupName = szTemp;

			CDlgCompSubItem* pSubItem = new CDlgCompSubItem;

			szKey.Format("szCode%02d", j);
			::GetPrivateProfileString(szSect, szKey, "", szTemp, MAX_PATH, m_szDataFile);
			pSubItem->m_szCode = szTemp;

			szKey.Format("szCodeName%02d", j);
			::GetPrivateProfileString(szSect, szKey, "", szTemp, MAX_PATH, m_szDataFile);
			pSubItem->m_szCodeName = szTemp;

			szKey.Format("clr%02d", j);
			::GetPrivateProfileString(szSect, szKey, "", szTemp, MAX_PATH, m_szDataFile);
			pSubItem->m_clr = atol(szTemp);

			szKey.Format("nDrawStyle%02d", j);
			pSubItem->m_nDrawStyle = ::GetPrivateProfileInt(szSect, szKey, 0, m_szDataFile);

			szKey.Format("nThickness%02d", j);
			pSubItem->m_nThickness = ::GetPrivateProfileInt(szSect, szKey, 0, m_szDataFile);

			pGroupData->m_ItemList.AddTail(pSubItem);
		}
		pSTData->m_GroupList.AddTail(pGroupData);
	}
}

void CDlgCompDataHandle::SaveData(long pDataKey)
{
	STDlgCompData* pSTData = (STDlgCompData*)pDataKey;
	CString szSect("Fluctuation");
	CString szData;

	//����Save
	::WritePrivateProfileString(szSect, "version", "0806171", m_szDataFile);

	//�� ���ذ�. 0:��������,1:���Ͻð�,1:���簡
	szData.Format("%d", pSTData->m_nBase);
	::WritePrivateProfileString(szSect, "nBase", szData, m_szDataFile);

	// ��������(��������)
	if(pSTData->m_nTimer<=0) 
	{
		pSTData->m_nTimer=0;
		pSTData->m_bTimerUse = 0;
	}
	szData.Format("%d", pSTData->m_nTimer);
	::WritePrivateProfileString(szSect, "nTimer", szData, m_szDataFile);
	
	//���簡(����)ǥ�� ����
	szData.Format("%d", pSTData->m_bCurDisp);
	::WritePrivateProfileString(szSect, "bCurDisp", szData, m_szDataFile);

	// ��к�����.
	szData.Format("%d", pSTData->m_nTypeUnit);
	::WritePrivateProfileString(szSect, "nTypeUnit", szData, m_szDataFile);

	// SelectGroup
	// �Լ����� : SaveSelGroup)(long pDataKey)
	::WritePrivateProfileString(szSect, "SelectGroup", pSTData->m_szSelectGroup, m_szDataFile);


	// �������� �⺻�׷�(1)/��������(0)
	// �������⸦ ����ϸ� �������񺸱�� ���õ�.
	// �޴����� ���ý� ���������� ������ ����Ǿ�� ��.
	// �Լ����� : SaveShowGroupInfo)(long pDataKey)
	szData.Format("%d", pSTData->m_bShowAll);
	::WritePrivateProfileString(szSect, "bShowAll", szData, m_szDataFile);

	// SelectCode, �������񺸱��̰ų� ���������϶� ��ȿ
	::WritePrivateProfileString(szSect, "szSelCode", pSTData->m_szSelCode, m_szDataFile);

	//�������� ���
	// �޴����� ���ý� ���������� ������ ����Ǿ�� ��.
	// �Լ����� : SaveTimeUse)(long pDataKey)
	szData.Format("%d", pSTData->m_bTimerUse);
	::WritePrivateProfileString(szSect, "bTimerUse", szData, m_szDataFile);

	// GroupCount
	int nGroupCnt = pSTData->m_GroupList.GetCount();
	szData.Format("%d", nGroupCnt);
	::WritePrivateProfileString(szSect, "nGroupCnt", szData, m_szDataFile);

	CString szKey;
	int i=0;
	for(POSITION iPos=pSTData->m_GroupList.GetHeadPosition(); iPos; i++)
	{
		szSect.Format("FGroup%02d", i);

		CDlgCompGroupData* pGroupData = pSTData->m_GroupList.GetNext(iPos);

		int nItemCnt=0;
		szData.Format("%d", pGroupData->m_ItemList.GetCount());
		::WritePrivateProfileString(szSect, "nItemCnt", szData, m_szDataFile);

		int j=0;
		for(POSITION jPos=pGroupData->m_ItemList.GetHeadPosition(); jPos; j++)
		{
			//szSect = szKey;

			szData = pGroupData->m_szGroupName;
			::WritePrivateProfileString(szSect, "szGroupName", szData, m_szDataFile);

			CDlgCompSubItem* pSubItem = pGroupData->m_ItemList.GetNext(jPos);

			szKey.Format("szCode%02d", j);
			::WritePrivateProfileString(szSect, szKey, pSubItem->m_szCode, m_szDataFile);

			szKey.Format("szCodeName%02d", j);
			::WritePrivateProfileString(szSect, szKey, pSubItem->m_szCodeName, m_szDataFile);

			szKey.Format("clr%02d", j);
			szData.Format("%ld", pSubItem->m_clr);
			::WritePrivateProfileString(szSect, szKey, szData, m_szDataFile);

			szKey.Format("nDrawStyle%02d", j);
			szData.Format("%d", pSubItem->m_nDrawStyle);
			::WritePrivateProfileString(szSect, szKey, szData, m_szDataFile);

			szKey.Format("nThickness%02d", j);
			szData.Format("%d", pSubItem->m_nThickness);
			::WritePrivateProfileString(szSect, szKey, szData, m_szDataFile);
		}
	}
}

void CDlgCompDataHandle::SaveSelGroup(long pDataKey)
{
	STDlgCompData* pSTData = (STDlgCompData*)pDataKey;
	if(!pSTData) return;

	CString szSect("Fluctuation");

	// SelectGroup
	::WritePrivateProfileString(szSect, "SelectGroup", pSTData->m_szSelectGroup, m_szDataFile);
}

// ����޴��� ���õ� ���������Ѵ�.
// 1. �񱳱׷�(0)/�������񺸱�(1) ���� ��������.
//	�������񺸱��̸� �׶��� �������� ����.
void CDlgCompDataHandle::SaveShowGroupInfo(long pDataKey)
{
	STDlgCompData* pSTData = (STDlgCompData*)pDataKey;
	if(!pSTData) return;

	CString szSect("Fluctuation");
	CString szData;

	// �������� �⺻�׷�(1)/��������(0)
	// �������⸦ ����ϸ� �������񺸱�� ���õ�.
	// �޴����� ���ý� ���������� ������ ����Ǿ�� ��.
	szData.Format("%d", pSTData->m_bShowAll);
	::WritePrivateProfileString(szSect, "bShowAll", szData, m_szDataFile);

	if(pSTData->m_bShowAll==0)	//�������񺸱��̸� ���������� ����.
	{
		::WritePrivateProfileString(szSect, "szSelCode", pSTData->m_szSelCode, m_szDataFile);

		// �ڵ���� �ŷھ���.
		//::WritePrivateProfileString(szSect, "szSelCodeName", pSTData->m_szSelCodeName, m_szDataFile);
	}
}

void CDlgCompDataHandle::SaveSelCodeInfo(long pDataKey)
{
	STDlgCompData* pSTData = (STDlgCompData*)pDataKey;
	if(!pSTData) return;

	CString szSect("Fluctuation");
	CString szData;

	// ���� ���� �����ϱ�.
	::WritePrivateProfileString(szSect, "szSelCode", pSTData->m_szSelCode, m_szDataFile);
	//::WritePrivateProfileString(szSect, "szSelCodeName", pSTData->m_szSelCodeName, m_szDataFile);
}

void CDlgCompDataHandle::SaveTimeUse(long pDataKey)
{
	STDlgCompData* pSTData = (STDlgCompData*)pDataKey;
	if(!pSTData) return;

	CString szSect("Fluctuation");
	CString szData;

	//�������� ���
	// �޴����� ���ý� ���������� ������ ����Ǿ�� ��.
	szData.Format("%d", pSTData->m_bTimerUse);
	::WritePrivateProfileString(szSect, "bTimerUse", szData, m_szDataFile);
}


void CDlgCompDataHandle::ClearAll(long pDataKey)
{
	STDlgCompData* pSTData = (STDlgCompData*)pDataKey;
	ClearGroupList((long)&pSTData->m_GroupList);
}

void CDlgCompDataHandle::ClearGroupList(long _pGroupList)
{
	List_CDlgCompGroupData* pGroupList = (List_CDlgCompGroupData*)_pGroupList;
	for(POSITION pos=pGroupList->GetHeadPosition(); pos; )
	{
		CDlgCompGroupData* pGroupData = pGroupList->GetNext(pos);

		for(POSITION JPos=pGroupData->m_ItemList.GetHeadPosition(); JPos; )
		{
			CDlgCompSubItem* pSubItem = pGroupData->m_ItemList.GetNext(JPos);
			delete pSubItem;
		}
		pGroupData->m_ItemList.RemoveAll();

		delete pGroupData;
	}
	pGroupList->RemoveAll();
}

int CDlgCompDataHandle::CopyGroupList(long _pOrg, long _pTarget)
{
	ClearGroupList(_pTarget);
	List_CDlgCompGroupData* pOrg = (List_CDlgCompGroupData*)_pOrg;
	List_CDlgCompGroupData* pTarget = (List_CDlgCompGroupData*)_pTarget;

	if(pOrg==NULL || pOrg==NULL) return 0;

	for(POSITION pos=pOrg->GetHeadPosition(); pos; )
	{
		CDlgCompGroupData* pGroupData = pOrg->GetNext(pos);
		CDlgCompGroupData* pNewGroup = new CDlgCompGroupData;
		pNewGroup->m_szGroupName = pGroupData->m_szGroupName;

		for(POSITION JPos=pGroupData->m_ItemList.GetHeadPosition(); JPos; )
		{
			CDlgCompSubItem* pSubItem = pGroupData->m_ItemList.GetNext(JPos);
			CDlgCompSubItem* pNewSub = new CDlgCompSubItem;

			pNewSub->m_szCode		= pSubItem->m_szCode;
			pNewSub->m_szCodeName	= pSubItem->m_szCodeName;
			pNewSub->m_clr			= pSubItem->m_clr;
			pNewSub->m_nDrawStyle	= pSubItem->m_nDrawStyle;
			pNewSub->m_nThickness	= pSubItem->m_nThickness;

			pNewGroup->m_ItemList.AddTail(pNewSub);
		}
		pTarget->AddTail(pNewGroup);
	}
	return pTarget->GetCount();
}

