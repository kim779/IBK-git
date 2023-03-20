// SiteCustomAddin.cpp: implementation of the CSiteCustomAddin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./SiteCustomAddin.h"

#include "../Include_Chart/BlockBaseEnum.h"				// for CToolOptionInfo
#include "./Include_AddIn/I139092/_IPacketList.h"		// for CIPacketList
#include "./Include_AddIn/I139092/_IPacket.h"			// for IPacket
#include "./Include_AddIn/I139092/_IPacketManager.h"	// for IPacketManager

//#include "./Include_AddIn/I429091/_IPacketManager.h"	// for IPacketManager
//#include "./Include_AddIn/I429091/_IPacket.h"			// for IPacket
#include "../Common_ST/IDataItem.h"
#include "../Common_ST/STConfigdef.h"
#include "./WndAssistIndexManager.h"

#include "../../inc/IMainExInfoMng.h"				// for IMainExInfoManager2
#include "../Include_ST/STOrderDef.h"
#include "../Include_ST/SiteDefine.h"					// for g_strChartProgID

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CString Parser(LPCTSTR lpInputData, int *pNShifter );
extern HINSTANCE g_hInstance;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSiteCustomAddin::CSiteCustomAddin( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2008/4/29 - Seung-Won, Bae) to use the ProgID in ChartMetaTable.dat
	if( g_strChartProgID.IsEmpty())
	{
		ILPCSTR szProgID = p_pIChartOCX->GetProgID();
		g_strChartProgID = szProgID;
	}

	// (2006/2/22 - Seung-Won, Bae) Create Manager Directly in constructor.
	//	m_assistIndexManager.Create(NULL,"Assist", WS_CHILD,CRect(0,0,0,0), (CWnd*)m_pChartCWnd, 0x001, m_pIChartManager, m_pIChartOCX, m_pIPacketManager, m_pIGraphInfoManager);
	//	m_assistIndexManager.SetSiteCUstomAddin((LONG)this);

	m_lpszRQ = _T("DEFAULT");	// ���� RQ : �������� - ojtaso (20070305)

	// RQ�� ���� : �������� - ojtaso (20070305)
	CWndAssistIndexManager* pAssistIndexManager = new CWndAssistIndexManager;
	pAssistIndexManager->Create(NULL,"Assist", WS_CHILD,CRect(0,0,0,0), (CWnd*)m_pChartCWnd, 0x001, m_pIChartManager, m_pIChartOCX, m_pIPacketManager, m_pIGraphInfoManager);
	pAssistIndexManager->SetSiteCUstomAddin((LONG)this);

	m_mapWndAssistIndexManager.insert(MAP_WNDASSISTINDEXMANAGER::value_type(m_lpszRQ, pAssistIndexManager));
}

CSiteCustomAddin::~CSiteCustomAddin()
{
	//	m_assistIndexManager.DestroyWindow();

	// RQ�� ���� : �������� - ojtaso (20070305)
	if(!m_mapWndAssistIndexManager.empty())
	{
		MAP_WNDASSISTINDEXMANAGER::iterator it = m_mapWndAssistIndexManager.begin();
		while(it != m_mapWndAssistIndexManager.end())
		{
			(*it).second->DestroyWindow();
			delete (*it++).second;
		}
	}
}


//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CSiteCustomAddin)
	ONADDINEVENT( OnDrawBeforeGraph)
// vntsorl:[0003431] [7111]��Ʈȭ�鿡���� ��ȸ�ӵ��� ����Ʈ�� ���� �ʾ� ���� �䱸
// 	ONADDINEVENT( OnPacketData)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( OnUpdatePacketData)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_char)
	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_CObject)
	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType_long)
	ONADDINEVENT( OnLButtonDblClk)
	// �������� ������ ���� �߰� : �������� - ojtaso (20070125)
	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_char_CObject)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long_char_char_char)
	// �������� Real�ݿ� : �������� - ojtaso (20070131)
	ONADDINEVENT( OnUpdatePacketDataMultiItem)
	ONADDINEVENT( OnPacketDataMultiItem)
	// �������� �߰�/���� : �������� - ojtaso (20070302)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char)
END_ADDIN_EVENT_MAP()


// [05/07/08] AddIn ���� Interface�� �����Ѵ�.
//		*. ����. �ݵ��� ���� Interface Pointer�� Converting�� IUnknown�� Pointer�� ��ȯ�Ͽ� �����Ѵ�.
//		*. ���� ������ _IWRAPPER_IUNKNOWN_INTERFACE�� �����Ѵ�.
IUnknown *CSiteCustomAddin::GetAddInCustomInterface( void)
{
// (2006/1/23 - Seung-Won, Bae) Do not use it. Use AddIn Command!
/*
	IUnknown *pIUnknown = ( IUnknown *)( IChartAssistIndexAddIn *)this;
	pIUnknown->AddRef();
	return pIUnknown;
*/
	return NULL;
}

// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
void CSiteCustomAddin::GetAssistIndexManager(CWndAssistIndexManager*& pAssistIndexManager, LPCTSTR lpszRQ/* = NULL*/)
{
	BOOL bRetunNull = TRUE;

	if(!lpszRQ)
	{
		lpszRQ = m_lpszRQ;
		bRetunNull = FALSE;
	}

	MAP_WNDASSISTINDEXMANAGER::iterator it = m_mapWndAssistIndexManager.find(lpszRQ);
	if(it == m_mapWndAssistIndexManager.end())
	{
		// RQ�� ����������� �⺻ RQ�� ���� �������� �ʴ´�
		if(bRetunNull)
		{
			pAssistIndexManager = NULL;
			return;
		}
		else
		{
			it = m_mapWndAssistIndexManager.find(_T("DEFAULT"));
			// (2008/10/16 - Seung-Won, Bae) Check NULL.
			if(it == m_mapWndAssistIndexManager.end())
			{
				pAssistIndexManager = NULL;
				return;
			}
		}
	}

	pAssistIndexManager = (*it).second;
}

void CSiteCustomAddin::OnResetChart( void)
{
	//// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	//CWndAssistIndexManager* pAssistIndexManager;
	//GetAssistIndexManager(pAssistIndexManager);

	//pAssistIndexManager->ClearData();
	//m_pIChartOCX->InvalidateControl();
	//pAssistIndexManager->m_hSysTabEx = NULL;
}

// [04/10/11] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
void CSiteCustomAddin::OnDrawBeforeGraph( HDC p_hDC) //  CDC *p_pDC)
{
	//AFX_MANAGE_STATE( AfxGetStaticModuleState());
	HINSTANCE hInstResourceClient = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	CWndAssistIndexManager* pAssistIndexManager;
	//	GetAssistIndexManager(pAssistIndexManager);
	MAP_WNDASSISTINDEXMANAGER::iterator it = m_mapWndAssistIndexManager.begin();
	while(it != m_mapWndAssistIndexManager.end())
	{
		pAssistIndexManager = (*it).second;

		if(pAssistIndexManager->GetSafeHwnd())
		{
			CDC* pDC = CDC::FromHandle(p_hDC);
			pAssistIndexManager->Draw(pDC);// to draw system treading signal
			//pDC->ReleaseOutputDC();
		}

		it++;
	}
	AfxSetResourceHandle(hInstResourceClient);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  CalculateIndicator
 - Created at  :  2005-07-11   09:56
 - Author      :  ������
 - Parameters  :  pcszIndicatorName - ��ǥ��
 - Return Value:  BOOL - TRUE: ����, FALSE: ��ǥ
 - Description :  ��ǥ�� �߰� �� ����Ѵ�. (��ǥ�߰���, ��ǥ������ �����)
 -----------------------------------------------------------------------------------------*/
// (2006/1/9 - Seung-Won, Bae) CObject * <= const CList<CPacket*, CPacket*> *
/*
BOOL CSiteCustomAddin::CalculateIndicator(const char* pcszIndicatorName, CObject *p_plCPacket)
{
	// (2006/1/9 - Seung-Won, Bae) Use IPacket for suitable Version Number
	// IPacket���� ��ȯ�Ͽ� AddIn�� �����ش�. AddIn������ CPacket�� �𸣳�
	// IPacket�������̽��� �˱� �����̴�.
	BOOL bResult = FALSE;
	CList <IPacket *, IPacket *> lIPacketList;
	if( m_pIPacketManager->GetIPacketListFromCPacketList( p_plCPacket, &lIPacketList))
	{
		if(m_assistIndexManager.GetSafeHwnd())
		{
			m_assistIndexManager.AddGraph( pcszIndicatorName, &lIPacketList);
			bResult = TRUE;
		}
		IPacket *pIPacket = NULL;
		while( !lIPacketList.IsEmpty())
		{
			pIPacket = lIPacketList.RemoveHead();
			if( pIPacket) pIPacket->Release();
		}
	}
	return bResult;
}
*/


// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
BOOL CSiteCustomAddin::CalculateIndicator(LPCTSTR lpszRQ, LPCTSTR lpszParamCombine, CObject *p_plCPacket)
{
	// (2006/1/9 - Seung-Won, Bae) Use IPacket for suitable Version Number
	// IPacket���� ��ȯ�Ͽ� AddIn�� �����ش�. AddIn������ CPacket�� �𸣳�
	// IPacket�������̽��� �˱� �����̴�.
	//KHD : ���� 
	BOOL bResult = FALSE;
	CIPacketList lPacketList;

	if( m_pIPacketManager->GetIPacketListFromCPacketList(p_plCPacket, &lPacketList) )
	{
		CWndAssistIndexManager* pAssistIndexManager;
		GetAssistIndexManager(pAssistIndexManager, lpszRQ);

		if(pAssistIndexManager->GetSafeHwnd())
		{
			pAssistIndexManager->AddGraph( lpszParamCombine, &lPacketList);
			bResult = TRUE;
		}
		IPacket *pIPacket = NULL;
		while( !lPacketList.IsEmpty())
		{
			pIPacket = lPacketList.RemoveHead();
			if( pIPacket) pIPacket->Release();
		}
	}
	return bResult;
	//END
}

/*-----------------------------------------------------------------------------------------
 - Function    :  DeleteIndicator
 - Created at  :  2005-07-11   09:56
 - Author      :  ������
 - Parameters  :  pcszIndicatorName - ��ǥ��
 - Return Value:  BOOL - TRUE: ����, FALSE: ����
 - Description :  ��ǥ�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
BOOL CSiteCustomAddin::DeleteIndicator(LPCTSTR lpszRQ, LPCTSTR lpszParamCombine)
{
	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	CWndAssistIndexManager* pAssistIndexManager;
	GetAssistIndexManager(pAssistIndexManager, lpszRQ);

	if(pAssistIndexManager->GetSafeHwnd())
	{
		pAssistIndexManager->RemoveIndexItem(lpszParamCombine);		
		return TRUE;
	}	
	return FALSE;	
}

BOOL CSiteCustomAddin::CalculateAll(const int nDataOption)
{
	return TRUE;
}

// [04/10/13] TR�� ������ AddIn DLL�鿡�� �˷� ������� �Ѵ�.
void CSiteCustomAddin::OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset)
{
	// (2006/2/22 - Seung-Won, Bae) Init I/O Packet.
	// (2006/2/24 - Seung-Won, Bae) Do not SetRawData() in CalculateIndicator.
//	if( m_assistIndexManager.GetSafeHwnd()) m_assistIndexManager.SetRawData();
}

// �������� Real�ݿ� : �������� - ojtaso (20070131)
void CSiteCustomAddin::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	// (2006/2/22 - Seung-Won, Bae) Init I/O Packet.
	// (2006/2/24 - Seung-Won, Bae) Do not SetRawData() in CalculateIndicator.
	//	if( m_assistIndexManager.GetSafeHwnd()) m_assistIndexManager.SetRawData(lpszRQ);

	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	CWndAssistIndexManager* pAssistIndexManager;
	GetAssistIndexManager(pAssistIndexManager, lpszRQ);

	if(!pAssistIndexManager)
		return;

	if( pAssistIndexManager->GetSafeHwnd()) pAssistIndexManager->SetRawData();
}

// [04/12/07] TR�̳� Real�� ���ſ� ���� Packet Data�� Update�Ǿ����� AddIn DLL�鿡 �˷� ��ǥ���� ó����� �Ѵ�.
void CSiteCustomAddin::OnUpdatePacketData( BOOL p_bTrUpdate)
{
//	if(p_bTrUpdate==0&&m_assistIndexManager.GetSafeHwnd())
//	{
//		m_assistIndexManager.UpdateRawData();
//	}
}

// �������� Real�ݿ� : �������� - ojtaso (20070131)
void CSiteCustomAddin::OnUpdatePacketDataMultiItem(LPCTSTR lpszRQ, BOOL p_bTrUpdate)
{
	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	CWndAssistIndexManager* pAssistIndexManager;
	GetAssistIndexManager(pAssistIndexManager, lpszRQ);

	if(!pAssistIndexManager)
		return;

	if(p_bTrUpdate==0&&pAssistIndexManager->GetSafeHwnd())
	{
		//		m_assistIndexManager.UpdateRawData(lpszRQ);
		pAssistIndexManager->UpdateRawData();
	}
}

//////////////////////////////////////////////////////////////////////
// Temp Interface III	: ����Ǵ� Option ����� Interface�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////
// JSJ_ADD_50119__��Ʈ���� InvokeAddIn�Լ� ȣ����
BOOL CSiteCustomAddin::InvokeAddIn( int p_nCommandType, long p_lData)
{
	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	CWndAssistIndexManager* pAssistIndexManager;

	// (2004.10.19, ��¿�) Resource�� ������ �ߵǵ��� Module State�� �����Ѵ�.
	//AFX_MANAGE_STATE( AfxGetStaticModuleState());
	BOOL bRetValue = FALSE;
	switch(p_nCommandType)
	{
	case 1:// add
		{
			GetAssistIndexManager(pAssistIndexManager, m_lpszRQ);
			if(!pAssistIndexManager)
			{
				CWndAssistIndexManager* pAssistIndexManager = new CWndAssistIndexManager;
				pAssistIndexManager->Create(NULL,"Assist", WS_CHILD,CRect(0,0,0,0), (CWnd*)m_pChartCWnd, 0x001, m_pIChartManager, m_pIChartOCX, m_pIPacketManager, m_pIGraphInfoManager);
				pAssistIndexManager->SetSiteCUstomAddin((LONG)this);//?

				m_mapWndAssistIndexManager.insert(MAP_WNDASSISTINDEXMANAGER::value_type(m_lpszRQ, pAssistIndexManager));
			}
			CStringArray *pStrArrayName = (CStringArray *)p_lData;
			bRetValue = pAssistIndexManager->AddST(pStrArrayName->GetAt(0),pStrArrayName->GetAt(1));
			m_pIChartOCX->InvalidateControl();
		}
		break;


	//��Ƽ��Ʈ���� �Ҹ���� CChartMng::OnDestroy()���� �ݵ�.
	//pChartItem->InvokeAddIn("ChartSTIndexAddIn.dll",2,0);
	case 2://Clear
		{
			GetAssistIndexManager(pAssistIndexManager);
			// (2008/10/16 - Seung-Won, Bae) Check NULL.
			if( pAssistIndexManager) pAssistIndexManager->DeleteSTInfo();
			bRetValue = TRUE;
		}
		break;

	case 3: // �ڱ��ڽ� ����
		{
			TRACE("point is %ld\n", p_lData);
			IMainExInfoManager2* pMng = (IMainExInfoManager2*)AfxGetPctrInterface(UUID_IMainExInfoManager2);
			if(pMng)
			{
				LONG pValue = (LONG)&m_xSTAddinData;
				pMng->SetUserExData(p_lData, pValue);
				TRACE("p_lData=%ld, pValue=%ld\n", p_lData, pValue);
			}
		}
		break;
		//KHD by : 2006.08.29
	case 4:// It is For Strategy Analysis View
		{
			//m_assistIndexManager.SetpChartModule(m_pIChartManager, m_pIChartOCX, m_pIPacketManager);
			GetAssistIndexManager(pAssistIndexManager);

			pAssistIndexManager->SetOnlyDealSignal(p_lData);
			m_pIChartOCX->InvalidateControl();
		}
		break;
	case 5:// It is For Strategy Analysis View
		{
			GetAssistIndexManager(pAssistIndexManager);

			pAssistIndexManager->SetpChartModule(m_pIChartManager, m_pIChartOCX, m_pIPacketManager);
			pAssistIndexManager->SetPeriod(p_lData);
			//m_pIChartOCX->InvalidateControl();
		}	
		break;
	case 6:// It is For Strategy Analysis View
		{
			GetAssistIndexManager(pAssistIndexManager);

			pAssistIndexManager->m_hSysTabEx = (HWND)p_lData;	 //KHD 2006.11.22 HWND
			//m_assistIndexManager.SetpChartModule(m_pIChartManager, m_pIChartOCX, m_pIPacketManager);
			//m_assistIndexManager.SetHandleUpdatePeriod(p_lData);
			//m_pIChartOCX->InvalidateControl();
		}	
		break;
	case 7://���� íƮ�� �����Ÿ�ǥ�� �� ���ͷ��׷����� ���ش�. : KHD 
		{
			GetAssistIndexManager(pAssistIndexManager);

			pAssistIndexManager->DestroyStrategyItem();
			pAssistIndexManager->ClearData();
			m_pIChartOCX->InvalidateControl();

		}
		break;
	case 8:// It is For Strategy Analysis View
		{
			GetAssistIndexManager(pAssistIndexManager);

			CString* pstrSTPath = (CString*)p_lData;
			(*pstrSTPath) = pAssistIndexManager->m_szMSTName;
		}
		break;
	case 10:	// ����(ST)�� �����Ѵ�.
		{
			GetAssistIndexManager(pAssistIndexManager, m_lpszRQ);
			if(!pAssistIndexManager)
			{
				pAssistIndexManager = new CWndAssistIndexManager;
				pAssistIndexManager->Create(NULL,"Assist", WS_CHILD,CRect(0,0,0,0), (CWnd*)m_pChartCWnd, 0x001, m_pIChartManager, m_pIChartOCX, m_pIPacketManager, m_pIGraphInfoManager);
				pAssistIndexManager->SetSiteCUstomAddin((LONG)this);

				m_mapWndAssistIndexManager.insert(MAP_WNDASSISTINDEXMANAGER::value_type(m_lpszRQ, pAssistIndexManager));
			}			
			stAddinSTRun* pData = (stAddinSTRun*)p_lData;	
			
			{
				//���һ��� 
				pAssistIndexManager->SetChartDivideInfo((int)m_pIPacketManager->GetBaseTimeDataType(), m_pIPacketManager->GetBaseTimeUnit());
			}

			if(pData->nCount <= 0)
			{
				pAssistIndexManager->ClearData();
				m_pIChartOCX->InvalidateControl();
				pAssistIndexManager->m_hSysTabEx = NULL;			//@@@@@06.11.20 HWND
				return bRetValue;
			}

			int nRet = pAssistIndexManager->LoadST((LPCSTR)pData->szSTFile,
				pData->pChartInterface,
				(LPCSTR)pData->szCode,
				pData->nCount,
				(LPCSTR)pData->szEndData,
				pData->stArrayPath,
				pData->szSTName);

			pAssistIndexManager->m_StDataIndex.RemoveAll();//�����м����� ������ ������ Clear : KHD : 2006.11.06
			pAssistIndexManager->m_hSysTabEx = pData->wndInfo.hSystemTabEx;		//@@@@@06.11.20 HWND

			pAssistIndexManager->m_STConfigInfo.m_szSTFileName = pData->szSTFile;		// ST���ϸ�
			pAssistIndexManager->m_STConfigInfo.m_szSTName = pData->szSTName;			// ������
			pAssistIndexManager->m_STConfigInfo.m_szCode = pData->szCode;	// �����
			pAssistIndexManager->m_STConfigInfo.m_szCodeName = pData->szCodeName;
			pAssistIndexManager->m_STConfigInfo.m_nCodeType = pData->nCodeType;
			if(nRet==0)
			{	// case 7�϶��� ����
				// 0�ΰ��� �������� ������ �����϶���,
				// ���̳ʽ����� ����
				// �÷����� ����
				pAssistIndexManager->ClearData();
				m_pIChartOCX->InvalidateControl();
				pAssistIndexManager->m_hSysTabEx = NULL;			//@@@@@06.11.20 HWND
			}
		}
		break;
	case 11:	// ���� ���������� ������¸� �����Ѵ�.
		{
			GetAssistIndexManager(pAssistIndexManager);
			// (2008/10/16 - Seung-Won, Bae) Check NULL.
			if( !pAssistIndexManager) return FALSE;

			bRetValue = TRUE;
			stAddinSTRunInfo* pInfo = (stAddinSTRunInfo*)p_lData;
			pInfo->bIsRun = FALSE;
			pInfo->szRunSTFile = pAssistIndexManager->m_szMSTName;
			if(pAssistIndexManager->GetSafeHwnd())
			{
				if(pInfo->nSTType == 4)		// �����༼
				{
					CString strSTName(pInfo->szSTFile);
					int nPosExt = strSTName.ReverseFind('.');
					int nPosFileTitle = strSTName.ReverseFind(('\\'));
					strSTName = strSTName.Left(nPosExt);
					strSTName = strSTName.Mid(nPosFileTitle+1);
					if(strSTName.Left(2) == "P_")
						strSTName = strSTName.Mid(2);

					if(strSTName.CompareNoCase(pAssistIndexManager->m_szSTName) == 0)
					{
						// �������� ���� TRUE;
						pInfo->bIsRun = TRUE;
					}
				}
				else if(pAssistIndexManager->m_szMSTName.CompareNoCase(pInfo->szSTFile)==0) 
				{
					// �������� ���� TRUE;
					pInfo->bIsRun = TRUE;
				}
			}
		}
		break;
	// �������� �߰��� CSiteCustomAddin�� ������� �ʴ´� : �������� - ojtaso (20070306)
/*	
	case 20:	// �ֹ���ȣ
		{
			STORDER_INDATA* lpSTOrderData = (STORDER_INDATA*)p_lData;
			m_assistIndexManager.OrderSign(lpSTOrderData);
		}
		break;
*/
	case 200:	//��ü�ʱ�ȭ
		{	
			// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
			CWndAssistIndexManager* pAssistIndexManager = NULL;
			GetAssistIndexManager(pAssistIndexManager);

			// (2008/10/16 - Seung-Won, Bae) Check NULL.
			if( !pAssistIndexManager) return FALSE;

			pAssistIndexManager->ClearData();
			m_pIChartOCX->InvalidateControl();
			pAssistIndexManager->m_hSysTabEx = NULL;
			bRetValue = TRUE;
		}
		break;
	}

	return bRetValue;
}


//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

//{{JS.Kim_20100908 ����Ÿ�Ǽ� �ʰ��� �����Ҷ� ��ǥ �ݿ��ȵǴ� ���� ����
BOOL CSiteCustomAddin::OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam)
{
	if( EAI_ST_REMOVED_DATA_COUNT == p_eCommandID )
	{
		CWndAssistIndexManager* pAssistIndexManager = NULL;
		GetAssistIndexManager(pAssistIndexManager);
		if(pAssistIndexManager && pAssistIndexManager->GetSafeHwnd())
		{
			pAssistIndexManager->UpdateRawData();
		}
		return TRUE;
	}
	return FALSE;
}
//}}

BOOL CSiteCustomAddin::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2)
{
	/* BAE, 2��, �̵����, #3333, =>  InedicatorName, GraphName */
	//	if( EAI_ALL_ON_DELETE_INDICATOR == p_eCommandID)
	//	{
	//		CString szParamCombine = p_szParam1;
	//		szParamCombine += ";";
	//		szParamCombine += p_szParam2;
	//		//		DeleteIndicator( p_szParam);
	//		DeleteIndicator( szParamCombine);
	//	}
	return TRUE;
}

BOOL CSiteCustomAddin::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, CObject *p_pParam)
{
	/* BAE, IndicatorName, GraphName, CObjectg!!*/
	//	if( EAI_ST_CALCULATE_INDICATOR == p_eCommandID)
	//	{
	//		CString szParamCombine = p_szParam1;
	//		szParamCombine += ";";
	//		szParamCombine += p_szParam2;
	//		//		p_lResult = CalculateIndicator( p_szParam1, p_pParam);
	//		p_lResult = CalculateIndicator( szParamCombine, p_pParam);
	//		return TRUE;
	//	}
	return FALSE;
}

BOOL CSiteCustomAddin::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam)
{
	if( EAI_ST_CALCULATE_ALL == p_eCommandID)
	{
		p_lResult = CalculateAll( p_lParam);
		return TRUE;
	}
	return FALSE;
}

// �������� ������ ���� �߰� : �������� - ojtaso (20070125)
BOOL CSiteCustomAddin::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3, CObject *p_pParam)
{
	if( EAI_ST_CALCULATE_INDICATOR == p_eCommandID)
	{
		if( EAI_ST_CALCULATE_INDICATOR == p_eCommandID)
		{
			CString szParamCombine = p_szParam2;	// Indicator Name
			szParamCombine += ";";
			szParamCombine += p_szParam3;			// Graph Name (Unique)
			//		p_lResult = CalculateIndicator( p_szParam1, p_pParam);
			// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
			p_lResult = CalculateIndicator(p_szParam1, szParamCombine, p_pParam);
			return TRUE;
		}
		return TRUE;
	}

	return FALSE;
}

// �������� ������ ���� �߰� : �������� - ojtaso (20070125)
BOOL CSiteCustomAddin::OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3)
{
	/* BAE, 2��, �̵����, #3333, =>  InedicatorName, GraphName */
	if( EAI_ALL_ON_DELETE_INDICATOR == p_eCommandID)
	{
		CString szParamCombine = p_szParam3;		// Indicator Name
		szParamCombine += ";";
		szParamCombine += p_szParam2;				// Graph Name (Unique)
		//		DeleteIndicator( p_szParam);
		// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
		DeleteIndicator(p_szParam1, szParamCombine);
	}
	return TRUE;
}

// BOOL CSiteCustomAddin::OnLButtonDblClk(	CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)
BOOL CSiteCustomAddin::OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	CWndAssistIndexManager* pAssistIndexManager;
	GetAssistIndexManager(pAssistIndexManager);

	CPoint pt(point);
	if(pAssistIndexManager->GetSafeHwnd()) return pAssistIndexManager->OnLButtonDblClk(nFlags,pt);
	return FALSE;
}

// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
int CSiteCustomAddin::CSTAddinData::GetChartData(LONG& dwDataTime, LONG& dwDataOpen, LONG& dwDataHigh, LONG& dwDataLow, LONG& dwDataClose,LONG& dwDataVolume, LONG& dwDataExt)
{
	MYMETHOD_PROLOGUE(CSiteCustomAddin, STAddinData)

	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	CWndAssistIndexManager* pAssistIndexManager;
	pThis->GetAssistIndexManager(pAssistIndexManager);

	dwDataTime	= (LONG)&pAssistIndexManager->m_SDataItemDate;
	dwDataOpen	= (LONG)&pAssistIndexManager->m_DataItemOpen;
	dwDataHigh	= (LONG)&pAssistIndexManager->m_DataItemHigh;
	dwDataLow	= (LONG)&pAssistIndexManager->m_DataItemLow;
	dwDataClose	= (LONG)&pAssistIndexManager->m_DataItemClose;
	dwDataVolume= (LONG)&pAssistIndexManager->m_DataItemVolume;
	// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
	dwDataExt	= (LONG)&pAssistIndexManager->m_DataItemExt;

	return pAssistIndexManager->m_DataItemOpen.GetSize();
}

// �������� �߰�/���� : �������� - ojtaso (20070302)
BOOL CSiteCustomAddin::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam)
{
	if(p_eCommandID == EAI_PACKETLISTMANAGER_NEW_RQ)
	{
		CWndAssistIndexManager* pAssistIndexManager;
		GetAssistIndexManager(pAssistIndexManager, p_szParam);
		// ���� RQ�� ���ؼ� �߰����� ����
		if(!pAssistIndexManager)
		{
			CWndAssistIndexManager* pAssistIndexManager = new CWndAssistIndexManager;
			pAssistIndexManager->Create(NULL,"Assist", WS_CHILD,CRect(0,0,0,0), (CWnd*)m_pChartCWnd, 0x001, m_pIChartManager, m_pIChartOCX, m_pIPacketManager, m_pIGraphInfoManager);
			pAssistIndexManager->SetSiteCUstomAddin((LONG)this);
			pAssistIndexManager->SetRQ(p_szParam);

			m_mapWndAssistIndexManager.insert(MAP_WNDASSISTINDEXMANAGER::value_type(p_szParam, pAssistIndexManager));
			m_lpszRQ = p_szParam;
		}
	}
	else if(p_eCommandID == EAI_PACKETLISTMANAGER_CHANGE_RQ)
	{
		m_lpszRQ = p_szParam;
	}
	else if(p_eCommandID == EAI_PACKETLISTMANAGER_DELETE_RQ)
	{
		CWndAssistIndexManager* pAssistIndexManager;
		GetAssistIndexManager(pAssistIndexManager, p_szParam);

		if(pAssistIndexManager)
		{
			pAssistIndexManager->DestroyWindow();
			delete pAssistIndexManager;
		}

		m_mapWndAssistIndexManager.erase(p_szParam);
	}

	return FALSE;
}
