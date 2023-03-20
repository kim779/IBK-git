// SiteCustomAddin.cpp: implementation of the CSiteCustomAddin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartPatternAddin.h"
#include "SiteCustomAddin.h"

// 2008.02.20 by LYH >> 7.0 -> 6.0
//#include "../Chart_common/BlockBaseEnum.h"	// for CToolOptionInfo
#include "BlockBaseEnum.h"	// for CToolOptionInfo
//#include "../../inc/LoadDrds.h"						// for DrdsData
// 2008.02.20 by LYH <<

//#include "DlgPatternEnvSetup.h"
#include "DlgSetup.h"

// 2008.02.20 by LYH >>
#include "./Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
// 2008.02.20 by LYH <<

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern HINSTANCE g_hInstance;
CSiteCustomAddin::CSiteCustomAddin( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase, const BOOL p_bOverExclusive)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase, p_bOverExclusive)
{
	m_bRecvRealData_S31 = FALSE;

	//m_objPatternMaker.SetIChartOCX( m_pIChartManager, m_pIChartOCX,  m_pIPacketManager);
	m_lpszRQ = _T("DEFAULT");	// ���� RQ : �������� - ojtaso (20070305)

	ILPCSTR strRQ = m_pIChartOCX->GetSelectedRQ();
	m_lpszRQ = strRQ;
	//p_pIChartOCX->GetSelectedRQ();
	// RQ�� ���� : �������� - ojtaso (20070305)
	CPatternMaker* pPatternMaker = new CPatternMaker;
//	pPatternMaker->Create(NULL,"Assist", WS_CHILD,CRect(0,0,0,0), (CWnd*)m_pChartCWnd, 0x001, m_pIChartManager, m_pIChartOCX, m_pIPacketManager, m_pIGraphInfoManager);
	pPatternMaker->SetIChartOCX( m_pIChartManager, m_pIChartOCX,  m_pIPacketManager);
	pPatternMaker->SetRQ(m_lpszRQ);

	m_mapPatternMaker.insert(MAP_PATTERNMAKER::value_type(m_lpszRQ, pPatternMaker));

		
	//m_bBlock			= FALSE;
	//m_dCurPrice		= ZERO;
	//m_dOpen			= ZERO;
	//m_dHigh			= ZERO;
 	//m_dLow			= ZERO;

	m_hOcxWnd = NULL;
	if( p_pIChartOCX) m_hOcxWnd = p_pIChartOCX->GetOcxHwnd();
}

CSiteCustomAddin::~CSiteCustomAddin()
{
	//m_objPatternMaker.DestroyAnalysisDialog();
	// RQ�� ���� : �������� - ojtaso (20070305)
	if(!m_mapPatternMaker.empty())
	{
		MAP_PATTERNMAKER::iterator it = m_mapPatternMaker.begin();
		while(it != m_mapPatternMaker.end())
		{
			(*it).second->DestroyAnalysisDialog();
			delete (*it++).second;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CSiteCustomAddin)
	ONADDINEVENT( OnDrdsData)
	ONADDINEVENT( OnDrdsPacketEnd)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonUp)
	ONADDINEVENT( OnResetChart)
	// �������� ������ ���� �߰� : �������� - ojtaso (20070125)
//	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_char_CObject)
//	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long_char_char_char)
	// �������� Real�ݿ� : �������� - ojtaso (20070131)
	ONADDINEVENT( OnUpdatePacketDataMultiItem)
	ONADDINEVENT( OnPacketDataMultiItem)
	// �������� �߰�/���� : �������� - ojtaso (20070302)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// (2004.10.15, ��¿�) Real ���� Event�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// [04/10/14] Real�� ���Ź޾� RELPSA�� ó���Ѵ�.
//		�ӵ� ����� DRDS�� Data�� �״�� Pass�Ѵ�.
void CSiteCustomAddin::OnDrdsData( WPARAM p_wParam, LPARAM p_lParam)
{
	LPCSTR	pData = (LPSTR)p_lParam; //(LPCSTR)lParam;
	int		len	  = (int)p_wParam;
	CString strTemp;

// 2008.02.20 by LYH >> Not Used
//	if(len > 3)   
//	{
//		DrdsData* pmyDData = (DrdsData*)pData;
//		
//// DEBUG------------------------------------------------------------------>>
////		CString	strDbg;
////		strDbg.Format("%s", pmyDData->Name);
////		OutputDebugString("\n[HSpeedJumun-DrdsData] DataName = " + strDbg);
//// <<------------------------------------------------------------------------
//
//		if(memcmp(pmyDData->Name, "S31", 3) == 0)			// ���簡 ��Ŷ
//		{
//			m_bRecvRealData_S31 = TRUE;
//		}
//	}
// 2008.02.20 by LYH <<
}

// [04/10/15] Real Packet�� ������ �������� AddIn DLL���� �˷� ó����� �Ѵ�.
void CSiteCustomAddin::OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam)
{
	// a. OnDrdsData()���� �ǽð����� ���簡 ��Ŷ�� ���� ���Ŀ�
	// b. OnDrdsPacketEnd()���� Real Packet�� ������ �������� �˸��� ��ȣ�� ������ ���� ����
	//if ( m_bRecvRealData_S31 )
	//{
	//	m_bRecvRealData_S31 = FALSE;


	//	m_objPatternMaker.DeleteAllData();					// ���ϰ��� ������ ��� ����
	//	m_objPatternMaker.InitSelectedPatternData(TRUE);	// Init Selected Pattern Data

	//	// Get All WavePattern Data
	//	m_objPatternMaker.GetAllData_AboutPattern();
	//		
	//	// ������ �׸��� ���
	//	if ( !m_objPatternMaker.m_PatternEnvData.m_bNotDrawAll)
	//	{
	//		// �ǽð����� �׷��� ���õ� ������ �ִ��� üũ
	//		m_objPatternMaker.CheckSelectedPatternData_ForReal();
	//	}
	//}
}

// �������� Real�ݿ� : �������� - ojtaso (20070131)
void CSiteCustomAddin::OnUpdatePacketDataMultiItem(LPCTSTR lpszRQ, BOOL p_bTrUpdate)
{
	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	CPatternMaker* pPatternMaker;
	GetPatternMaker(pPatternMaker, lpszRQ);

	if(!pPatternMaker)
		return;

	if(p_bTrUpdate==0&&pPatternMaker)
	{
		if ( m_bRecvRealData_S31 )
		{
			m_bRecvRealData_S31 = FALSE;


			pPatternMaker->DeleteAllData();					// ���ϰ��� ������ ��� ����
			pPatternMaker->InitSelectedPatternData(TRUE);	// Init Selected Pattern Data

			// Get All WavePattern Data
			pPatternMaker->GetAllData_AboutPattern();
				
			// ������ �׸��� ���
			if ( !pPatternMaker->m_PatternEnvData.m_bNotDrawAll)
			{
				// �ǽð����� �׷��� ���õ� ������ �ִ��� üũ
				pPatternMaker->CheckSelectedPatternData_ForReal();
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) �⺻ Event�� �����Ѵ�. (�ʿ� ���� ��� �������� �ʴ´�. Base���� �����ش�.)
//////////////////////////////////////////////////////////////////////

// [04/12/25] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
// �߼� ������ ���׷����� �������⶧���� DrawAfter ���
void CSiteCustomAddin::OnDrawAfterGraph( CDC *p_pDC)
{
	//AFX_MANAGE_STATE( AfxGetStaticModuleState());

	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	CPatternMaker* pPatternMaker;
	MAP_PATTERNMAKER::iterator it = m_mapPatternMaker.begin();
	while(it != m_mapPatternMaker.end())
	{
		pPatternMaker = (*it).second;

		pPatternMaker->OnDraw( p_pDC);
		it++;
	}

	//m_objPatternMaker.OnDraw( p_pDC);
}

// �������� Real�ݿ� : �������� - ojtaso (20070131)
void CSiteCustomAddin::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
// 2008.01.20 by LYH >>	
	//if( !( BOOL)strstr( p_szPacketNames, "����;") /*|| !p_bWithReset*/) return;
	CString strClose;
	strClose.Format("%s;", _MTEXT(C0_CLOSE));
	if( !( BOOL)strstr( p_szPacketNames, strClose) /*|| !p_bWithReset*/) return;
// 2008.01.20 by LYH <<	
	// (2006/2/22 - Seung-Won, Bae) Init I/O Packet.
	// (2006/2/24 - Seung-Won, Bae) Do not SetRawData() in CalculateIndicator.
	//	if( m_assistIndexManager.GetSafeHwnd()) m_assistIndexManager.SetRawData(lpszRQ);

	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	CPatternMaker* pPatternMaker;
	GetPatternMaker(pPatternMaker, lpszRQ);

	if(!pPatternMaker)
		return;
	

	pPatternMaker->DeleteAllData();					// ���ϰ��� ������ ��� ����
	pPatternMaker->InitSelectedPatternData();		// Init Selected Pattern Data

	// Get All WavePattern Data
	pPatternMaker->GetAllData_AboutPattern();
	// Get All CandlePattern Data
	pPatternMaker->GetAllData_AboutCandlePattern();
}


//////////////////////////////////////////////////////////////////////
// (2004.10.29, ��¿�) AddIn DLL���� �׹��� Windows Event�� �����Ѵ�.
//////////////////////////////////////////////////////////////////////

// JSJ_ADD_50119__��Ʈ���� InvokeAddIn�Լ� ȣ����
BOOL CSiteCustomAddin::InvokeAddIn( int p_nCommandType, long p_lData)
{
	// (2004.10.19, ��¿�) Resource�� ������ �ߵǵ��� Module State�� �����Ѵ�.
	//AFX_MANAGE_STATE( AfxGetStaticModuleState());

	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	CPatternMaker* pPatternMaker;

	// Create Or Destroy Analysis Dialog (p_lData  0:Destroy, 1:�ڵ��߼� 2:�߼����� 3:���Ϻм� 4:���ϼ���)
	if ( p_lData > 0)
	{
		//2007.02.26 by LYH ��ü�ʱ�ȭ�ø� ���� ����
		if(p_lData == 200 )	//��ü�ʱ�ȭ
		{
			// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
			CPatternMaker* pPatternMaker;
			GetPatternMaker(pPatternMaker);

			pPatternMaker->DestroyAnalysisDialog();
			pPatternMaker->m_bDrawAutoTrendLine = FALSE;
			pPatternMaker->m_bDrawAutoPattern = FALSE;
			pPatternMaker->m_bDrawCandlePattern = FALSE;
			return TRUE;
		}
		//}}
		//if(p_lData == 2)
		//	m_objPatternMaker.CreateDlgAnalysis( m_pIChartOCX, p_lData, TRUE);
		//else
		//m_objPatternMaker.CreateDlgAnalysis( m_pIChartOCX, p_lData, FALSE);
		GetPatternMaker(pPatternMaker, m_lpszRQ);
		if(!pPatternMaker)
		{
			pPatternMaker = new CPatternMaker;
			pPatternMaker->SetIChartOCX( m_pIChartManager, m_pIChartOCX,  m_pIPacketManager);
			pPatternMaker->SetRQ(m_lpszRQ);
			//pPatternMaker->Create(NULL,"Assist", WS_CHILD,CRect(0,0,0,0), (CWnd*)m_pChartCWnd, 0x001, m_pIChartManager, m_pIChartOCX, m_pIPacketManager, m_pIGraphInfoManager);
			m_mapPatternMaker.insert(MAP_PATTERNMAKER::value_type(m_lpszRQ, pPatternMaker));
		}
		pPatternMaker->CreateDlgAnalysis( m_pIChartOCX, p_lData, FALSE);

		pPatternMaker->DeleteAllData();					// ���ϰ��� ������ ��� ����
		pPatternMaker->InitSelectedPatternData();		// Init Selected Pattern Data

		// Get All WavePattern Data
		pPatternMaker->GetAllData_AboutPattern();
		// Get All CandlePattern Data
		pPatternMaker->GetAllData_AboutCandlePattern();

		//m_objPatternMaker.DeleteAllData();					// ���ϰ��� ������ ��� ����
		//m_objPatternMaker.InitSelectedPatternData();		// Init Selected Pattern Data

		//// Get All WavePattern Data
		//m_objPatternMaker.GetAllData_AboutPattern();
		//// Get All CandlePattern Data
		//m_objPatternMaker.GetAllData_AboutCandlePattern();

		if(p_lData == 8)
		{
			ML_SET_LANGUAGE_RES();

			CDlgSetup dlgSetup;
			//dlgSetup.SetPatternEnvData(&(m_objPatternMaker.m_PatternEnvData));
			dlgSetup.SetPatternEnvData(&(pPatternMaker->m_PatternEnvData));

			if( dlgSetup.DoModal() == IDOK )
			{
				//m_objPatternMaker.DeleteAllData();				// ���ϰ��� ������ ��� ����
				//m_objPatternMaker.InitSelectedPatternData();		// Init Selected Pattern Data

				//// Get All WavePattern Data
				//m_objPatternMaker.GetAllData_AboutPattern();
				//// Get All CandlePattern Data
				//m_objPatternMaker.GetAllData_AboutCandlePattern();
				pPatternMaker->DeleteAllData();				// ���ϰ��� ������ ��� ����
				pPatternMaker->InitSelectedPatternData();		// Init Selected Pattern Data

				// Get All WavePattern Data
				pPatternMaker->GetAllData_AboutPattern();
				// Get All CandlePattern Data
				pPatternMaker->GetAllData_AboutCandlePattern();
			}	
		}
	}
	else if ( p_lData == 0)
	{
		//m_objPatternMaker.DestroyAnalysisDialog();
		GetPatternMaker(pPatternMaker, m_lpszRQ);
		if(pPatternMaker)
			pPatternMaker->DestroyAnalysisDialog();
		return FALSE;
	}


	m_pIChartOCX->InvalidateControl();
	

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////
BOOL CSiteCustomAddin::OnLButtonDown(CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)
{
	ML_SET_LANGUAGE_RES();

	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	CPatternMaker* pPatternMaker;
	GetPatternMaker(pPatternMaker);

	CPoint pt(point);

	if(pPatternMaker)
		return pPatternMaker->OnLButtonDown(nFlags,point);
	else
		return FALSE;
	//return m_objPatternMaker.OnLButtonDown( nFlags, point);
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////
BOOL CSiteCustomAddin::OnMouseMove(CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)
{
	ML_SET_LANGUAGE_RES();

	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	CPatternMaker* pPatternMaker;
	GetPatternMaker(pPatternMaker);

	CPoint pt(point);
	if(pPatternMaker)
		return pPatternMaker->OnMouseMove( nFlags, point, pDC);
	else
		return FALSE;
	//return m_objPatternMaker.OnMouseMove( nFlags, point, pDC);
}

BOOL CSiteCustomAddin::OnLButtonUp(CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)
{
	//AFX_MANAGE_STATE( AfxGetStaticModuleState());
	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	CPatternMaker* pPatternMaker;
	GetPatternMaker(pPatternMaker);

	CPoint pt(point);
	if(pPatternMaker)
		return pPatternMaker->OnLButtonUp( nFlags, point);
	else 
		return FALSE;
	//return m_objPatternMaker.OnLButtonUp( nFlags, point);
}

//{{ ��Ʈ �ʱ�ȭ ó�� by LYH 2006.12.22
void CSiteCustomAddin::OnResetChart()
{
	//m_objPatternMaker.DestroyAnalysisDialog();
	//m_objPatternMaker.m_bDrawAutoTrendLine = FALSE;
	//m_objPatternMaker.m_bDrawAutoPattern = FALSE;
	//m_objPatternMaker.m_bDrawCandlePattern = FALSE;
}
//}}

// �ش� RQ�� PatternMaker : �������� - ojtaso (20070305)
void CSiteCustomAddin::GetPatternMaker(CPatternMaker*& pPatternMaker, LPCTSTR lpszRQ/* = NULL*/)
{
	BOOL bRetunNull = TRUE;

	if(!lpszRQ)
	{
		lpszRQ = m_lpszRQ;
		bRetunNull = FALSE;
	}

	MAP_PATTERNMAKER::iterator it = m_mapPatternMaker.find(lpszRQ);
	if(it == m_mapPatternMaker.end())
	{
		// RQ�� ����������� �⺻ RQ�� ���� �������� �ʴ´�
		if(bRetunNull)
		{
			pPatternMaker = NULL;
			return;
		}
		else
		{
			it = m_mapPatternMaker.find(_T("DEFAULT"));
			if(it == m_mapPatternMaker.end())
			{
				pPatternMaker = NULL;
				return;
			}
		}
	}

	pPatternMaker = (*it).second;
}

// �������� �߰�/���� : �������� - ojtaso (20070302)
BOOL CSiteCustomAddin::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam)
{
	if(p_eCommandID == EAI_PACKETLISTMANAGER_NEW_RQ)
	{
		CPatternMaker* pPatternMaker;
		GetPatternMaker(pPatternMaker, p_szParam);
		if(pPatternMaker != NULL)
			return FALSE;

		pPatternMaker = new CPatternMaker;
		pPatternMaker->SetIChartOCX( m_pIChartManager, m_pIChartOCX,  m_pIPacketManager);
		pPatternMaker->SetRQ(p_szParam);

		m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50031,FALSE);
		m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50032,FALSE);
		m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50033,FALSE);

		m_mapPatternMaker.insert(MAP_PATTERNMAKER::value_type(p_szParam, pPatternMaker));
		m_lpszRQ = p_szParam;
	}
	else if(p_eCommandID == EAI_PACKETLISTMANAGER_CHANGE_RQ)
	{
		m_lpszRQ = p_szParam;

		CPatternMaker* pPatternMaker;
		GetPatternMaker(pPatternMaker, m_lpszRQ);
		if(pPatternMaker != NULL)
		{
			m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50031,pPatternMaker->m_bDrawAutoTrendLine);
			m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50032,pPatternMaker->m_bDrawAutoPattern );
			m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50033,pPatternMaker->m_bDrawCandlePattern);
		}
		else
		{
			m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50031,FALSE);
			m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50032,FALSE);
			m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50033,FALSE);
		}
	}
	else if(p_eCommandID == EAI_PACKETLISTMANAGER_DELETE_RQ)
	{
		CPatternMaker* pPatternMaker;
		GetPatternMaker(pPatternMaker, p_szParam);

		if(pPatternMaker)
		{
			if(pPatternMaker)
				pPatternMaker->DestroyAnalysisDialog();
			delete pPatternMaker;
		}

		m_mapPatternMaker.erase(p_szParam);
	}

	return FALSE;
}
