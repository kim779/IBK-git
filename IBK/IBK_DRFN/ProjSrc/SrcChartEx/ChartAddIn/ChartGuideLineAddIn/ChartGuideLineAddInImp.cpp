// ChartStockOpinionAddInImp.cpp: implementation of the CChartGuideLineAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartGuideLineAddIn.h"
#include "ChartGuideLineAddInImp.h"

#include "./Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
#include "./Include_AddIn.h"							// for Ixxxxxx.h
#include "./Include/IStringList.h"						// for IStringList2

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define IDC_HAND MAKEINTRESOURCE(32649)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CConfigSet		g_clConfigSet;
extern CChartManager	g_clChartMngr;

// (2008/9/5 - Seung-Won, Bae) Support All Chart Sync for MA.
CTypedPtrMap<CMapPtrToPtr, CChartGuideLineAddInImp *, HWND> CChartGuideLineAddInImp::m_mapGuideLines;

extern HINSTANCE g_hInstance;
typedef LPCSTR (* FpGetUnitPrice)(LPSTR, LPSTR);
FpGetUnitPrice pGetUnitPrice = NULL;

CChartGuideLineAddInImp::CChartGuideLineAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2004.09.30, ��¿�) �ð������� ������ ����ϴ� Flag�� �ξ� ������ �� �ֵ��� �Ѵ�.
	//		�߽��� �������� �ʴ´�.
	//m_bEnableReceivingTimeShare = TRUE;
	//m_strUsrPath = _T("");
	//	m_strUsrPath = (LPSTR)(LPCTSTR)p_pIChartOCX->GetHtsPathString();

	ILPCSTR szUsrPath = p_pIChartOCX->GetUserFilePathString();
	// (2008/2/25 - Seung-Won, Bae) In koscom, there is no ID. so the Path String is end with '\' already.
	//	m_strUsrPath.Format("%s\\", szUsrPath);
	m_strUsrPath = szUsrPath;

	// (2009/9/12 - Seung-Won, Bae) for OHLC title
	m_hOcxWnd = NULL;
	ML_SET_LANGUAGE_RES();

//	m_pMAvgInfo = NULL;
	LoadGlobalCfg();
	//m_pmPatternMaker.SetIChartOCX( m_pIChartManager, m_pIChartOCX, m_pIDefaultPacketManager);
	m_lpszRQ = _T("DEFAULT");	// ���� RQ : �������� - ojtaso (20070305)
	CPatternMaker* pPatternMaker = new CPatternMaker;
	pPatternMaker->SetIChartOCX( m_pIChartManager, m_pIChartOCX, m_pIDefaultPacketManager);
	pPatternMaker->SetRQ(m_lpszRQ);
	m_mapPatternMaker.insert(MAP_PATTERNMAKER::value_type(m_lpszRQ, pPatternMaker));

	// (2008/9/5 - Seung-Won, Bae) Support All Chart Sync for MA.
	m_mapGuideLines.SetAt( this, m_pIChartOCX->GetOcxHwnd());
}

CChartGuideLineAddInImp::~CChartGuideLineAddInImp()
{
	// RQ�� ���� : �������� - ojtaso (20070305)
	if(!m_mapPatternMaker.empty())
	{
		MAP_PATTERNMAKER::iterator it = m_mapPatternMaker.begin();
		while(it != m_mapPatternMaker.end())
		{
			delete (*it++).second;
		}
	}

	// (2008/9/5 - Seung-Won, Bae) Support All Chart Sync for MA.
	m_mapGuideLines.RemoveKey( this);
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CChartGuideLineAddInImp::m_strAddInItemName = "GUIDE_LINE";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CChartGuideLineAddInImp)
	ONADDINEVENT(OnDrawAfterGraph)	
	ONADDINEVENT(OnDrawBeforeGraph)
	ONADDINEVENT(OnMouseMove)
	ONADDINEVENT(OnLButtonDown)
	ONADDINEVENT(OnLButtonUp)
	ONADDINEVENT(OnRButtonDown)
	ONADDINEVENT(OnRButtonUp)
	ONADDINEVENT(OnResetChart)
	// �������� Real�ݿ� : �������� - ojtaso (20070131)
	ONADDINEVENT( OnUpdatePacketDataMultiItem)
	ONADDINEVENT( OnPacketDataMultiItem)
	// �������� �߰�/���� : �������� - ojtaso (20070302)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char)
	// (2008/10/26 - Seung-Won, Bae) Draw DO/DH/DL/DC
	ONADDINEVENT( OnAddInToolCommand_EAICommandType)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////


BOOL CChartGuideLineAddInImp::OnMouseMove(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	ML_SET_LANGUAGE_RES();
	return FALSE;
}

BOOL CChartGuideLineAddInImp::OnLButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	ML_SET_LANGUAGE_RES();
	return FALSE;
}

BOOL CChartGuideLineAddInImp::OnLButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	ML_SET_LANGUAGE_RES();
	return FALSE;
}

BOOL CChartGuideLineAddInImp::OnRButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	ML_SET_LANGUAGE_RES();
	return FALSE;
}

BOOL CChartGuideLineAddInImp::OnRButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	ML_SET_LANGUAGE_RES();
	return FALSE;
}


BOOL CChartGuideLineAddInImp::OnLButtonDblClk(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	ML_SET_LANGUAGE_RES();
	return FALSE;
}

void CChartGuideLineAddInImp::OnDrawBeforeGraph(HDC p_hDC)
{
	ML_SET_LANGUAGE_RES();
}

void CChartGuideLineAddInImp::OnDrawAfterGraph(HDC p_hDC)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	CDC * pDC = CDC::FromHandle(p_hDC);
	//m_pmPatternMaker.OnDraw( pDC );
	CPatternMaker* pPatternMaker;
	MAP_PATTERNMAKER::iterator it = m_mapPatternMaker.begin();
	while(it != m_mapPatternMaker.end())
	{
		pPatternMaker = (*it).second;
		// 20081106 ����ǥ >>
		BOOL bDrawOHLC = FALSE;
		COLORREF clrUp = RGB( 255, 0, 0);
		COLORREF clrDown = RGB( 0, 0, 255);
		GetShowOHLC( bDrawOHLC, clrUp, clrDown, pPatternMaker->GetRQ());		
		pPatternMaker->SetDrawOHLC( bDrawOHLC, clrUp, clrDown);
		// 20081106 ����ǥ <<
		pPatternMaker->OnDraw( pDC);
		it++;
	}
}

BOOL CChartGuideLineAddInImp::InvokeAddIn(int p_nCommandType, long p_lData)
{
 	CString strFullAcct, sTemp, sTemp2;

	CPatternMaker* pPatternMaker;
	GetPatternMaker(pPatternMaker, m_lpszRQ);
	if(!pPatternMaker)
	{
		pPatternMaker = new CPatternMaker;
		pPatternMaker->SetIChartOCX( m_pIChartManager, m_pIChartOCX,  m_pIDefaultPacketManager);
		pPatternMaker->SetRQ(m_lpszRQ);
		m_mapPatternMaker.insert(MAP_PATTERNMAKER::value_type(m_lpszRQ, pPatternMaker));

		// (2008/11/6 - Seung-Won, Bae) Set Draw Flag at First.
		BOOL bDrawOHLC = FALSE;
		COLORREF clrUp = RGB( 255, 0, 0);
		COLORREF clrDown = RGB( 0, 0, 255);
		GetShowOHLC( bDrawOHLC, clrUp, clrDown, "DEFAULT");
		pPatternMaker->SetDrawOHLC( bDrawOHLC, clrUp, clrDown);
	}
	if(p_nCommandType == 101)
	{
		//m_pmPatternMaker.SetHwndSelChatItem((HWND)p_lData);
		pPatternMaker->SetHwndSelChatItem((HWND)p_lData);
	}
	else if(p_nCommandType == 0)
	{
		//====================================================================
		IStringList2* pIStringList = ( IStringList2 *)p_lData;
		if(pIStringList == NULL) return FALSE;
		
		POSITION pos1 = NULL;
		LPCSTR szTemp = NULL;
		for( pos1 = pIStringList->GetHeadPosition(); pos1;)
		{	
			pIStringList->GetNext( pos1, szTemp);
			strFullAcct += sTemp;
		}
		//====================================================================
		
		//	sTemp = GetString(*p_pvPram);
		// (2004.10.19, ��¿�) Resource�� ������ �ߵǵ��� Module State�� �����Ѵ�.
		int		nOldDay2Pyung1		= g_clConfigSet.m_nDay2Pyung1;
		int		nOldDay2Pyung2		= g_clConfigSet.m_nDay2Pyung2;
		int		nOldDay2Pyung3		= g_clConfigSet.m_nDay2Pyung3;
		int		nOldDay2Pyung4		= g_clConfigSet.m_nDay2Pyung4;
		BOOL	bOldbChk2Pyung1		= g_clConfigSet.m_bChk2Pyung1;
		BOOL	bOldbChk2Pyung2		= g_clConfigSet.m_bChk2Pyung2;
		BOOL	bOldbChk2Pyung3		= g_clConfigSet.m_bChk2Pyung3;
		BOOL	bOldbChk2Pyung4		= g_clConfigSet.m_bChk2Pyung4;

		ML_SET_LANGUAGE_RES();

		CDlgSetGuideLine dlgSetGuideLine;
		
		//���� ���� ǥ��.
		//dlgSetGuideLIne.SetPatternEnvData(&m_PatternEnvData);
		dlgSetGuideLine.m_strCfgFile  = m_strUsrPath + INI_FILENAME;
		dlgSetGuideLine.m_strFullAcct = strFullAcct;
		if( dlgSetGuideLine.DoModal() != IDOK )
		{
			//m_pmPatternMaker.SetHwndSelChatItem( NULL );
			pPatternMaker->SetHwndSelChatItem(NULL);
			//AfxSetResourceHandle(hInstResourceClient);
			return FALSE;
		}

		// (2008/9/5 - Seung-Won, Bae) Support All Chart Sync for MA.
		BOOL bIsMaUpdated = nOldDay2Pyung1		!= g_clConfigSet.m_nDay2Pyung1
							|| nOldDay2Pyung2	!= g_clConfigSet.m_nDay2Pyung2
							|| nOldDay2Pyung3	!= g_clConfigSet.m_nDay2Pyung3
							|| nOldDay2Pyung4	!= g_clConfigSet.m_nDay2Pyung4;
		CChartGuideLineAddInImp *pGuideLineAddIn = NULL;
		HWND hOcxWnd = NULL;
		POSITION psnGuideLines = m_mapGuideLines.GetStartPosition();
		while( psnGuideLines)
		{
			m_mapGuideLines.GetNextAssoc( psnGuideLines, pGuideLineAddIn, hOcxWnd);
			if( pGuideLineAddIn) pGuideLineAddIn->OnChangeGuideLineSetting( bIsMaUpdated);
			::InvalidateRect( hOcxWnd, NULL, FALSE);
		}
	}
	else if(p_nCommandType == 1 )
	{
		CStringList* pIStringList = NULL;
		pIStringList = (CStringList*)p_lData;
		if(pIStringList == NULL) return FALSE;
		
		//m_pmPatternMaker.Init_GuideLineValue();
		pPatternMaker->Init_GuideLineValue();
		POSITION pos1 = NULL;
		int nCnt = 0;
		for( pos1 = pIStringList->GetHeadPosition(); pos1;)
		{	
			if( nCnt == 0)
			{
				//���� ����
				sTemp = pIStringList->GetNext( pos1 );
				strFullAcct += sTemp;
			}
			else if(nCnt == 1)
			{
				OutputDebugString("���򰻽�");
				//�̵� ��� ��  ����
				sTemp = pIStringList->GetNext( pos1 );
				D10418_O* pMAvgInfo = NULL;
				pMAvgInfo = (D10418_O*)(LPTSTR)(LPCTSTR)sTemp;
				//m_pmPatternMaker.SetMAInfo(pMAvgInfo);
				pPatternMaker->SetMAInfo(pMAvgInfo);
				//strFullAcct += sTemp;					
			}
			else
				break;
			nCnt++;
		}		
	}
	else if(p_nCommandType == 2 )
	{
		CUIntArray* pArrayAvg = NULL;
		pArrayAvg = (CUIntArray*)p_lData;
		pArrayAvg->SetSize(4);

		// (2008/9/5 - Seung-Won, Bae) Support All Chart Sync for MA.
		pArrayAvg->SetAt( 0, ( UINT)g_clConfigSet.m_nDay2Pyung1);
		pArrayAvg->SetAt( 1, ( UINT)g_clConfigSet.m_nDay2Pyung2);
		pArrayAvg->SetAt( 2, ( UINT)g_clConfigSet.m_nDay2Pyung3);
		pArrayAvg->SetAt( 3, ( UINT)g_clConfigSet.m_nDay2Pyung4);
	}
	else if(p_nCommandType == INVOKEMSG_INIT)	//�����͸� ��ȸ�Ҽ� �ִ� HWND�� �� ��Ʈ����(?)�� �����͸� �޴´�. 
	{
		//Server Input Structure
			typedef struct  {
					HWND		m_hChartMng;
					CWnd*		m_pChartItem;
					void *		m_pBuffer;		
					DWORD		m_dwLength;
				}stMSGToInvoke;
		
		stMSGToInvoke* pMSGToInvoke  =  NULL;
		pMSGToInvoke = (stMSGToInvoke*)p_lData;
		if(pMSGToInvoke)
		{
			//m_pmPatternMaker.SetHwndCurChatItem(pMSGToInvoke->m_pChartItem->GetSafeHwnd());
			pPatternMaker->SetHwndCurChatItem(pMSGToInvoke->m_pChartItem->GetSafeHwnd());
		}

		//�޼����� ���� ü�� ����Ʈ�� �޵��� ��û�Ѵ�.
		//Server Input Structure
		//	typedef struct  {
		//			HWND		m_hChartMng;
		//			CWnd*		m_pChartItem;
		//			void *		m_pBuffer;		
		//			DWORD		m_dwLength;
		//		}stMSGToInvoke;
	}
	else if(p_nCommandType == 	INVOKEMSG_CHANGECODE) //���� �ڵ尡 �ٲ����.
	{
		//		CString strTemp, strHwnd, strThisChart;
		//		sTemp.Format("%s", (char*)p_lData);
		//		int nPos		= sTemp.Find(",");
		//		strHwnd			= sTemp.Left(nPos);
		//		strThisChart	= sTemp.Right(sTemp.GetLength() - nPos - 1);
		//		HWND	hWnd = (HWND)atoi(strHwnd);
		//		CWnd*	pThisWnd = (CWnd*)atoi(strThisChart);
		//		
		
		//	strTemp.Format("102������  : %x", hWnd);
		//		OutputDebugString(strTemp);
		//		
		//		strTemp.Format("102�ڱⲨ : %x", pThisWnd->GetSafeHwnd());
		//		OutputDebugString(strTemp);		
		
		//Server Input Structure
		//	typedef struct  {
		//			HWND		m_hChartMng;
		//			CWnd*		m_pChartItem;
		//			void *		m_pBuffer;		
		//			DWORD		m_dwLength;
		//		}stMSGToInvoke;
		stMSGToInvoke* pMSGToInvoke = NULL;
		pMSGToInvoke = (stMSGToInvoke*)p_lData;
		if(pMSGToInvoke)
		{			
			CString strCode;
			strCode.Format("%s", pMSGToInvoke->m_pBuffer);
			if(strCode != pPatternMaker->GetCode())			// ������ �����ڵ尡 �ٸ� ��쿣 
			{
				m_strCode = strCode;
				pPatternMaker->DeleteCheChartData();
				pPatternMaker->SetCode(m_strCode);
			}

			OutputDebugString("Changed Code == " + m_strCode);
			m_pIChartOCX->InvalidateControl();
		}	
		
	}
	else if(p_nCommandType == INVOKEMSG_SHOWORDEREDINFO) //����ڰ� �׸���� ��ư�� ������ ȣ��ȴ�.
	{
/*
		stMSGToInvoke* pMSGToInvoke  =  NULL;
		pMSGToInvoke = (stMSGToInvoke*)p_lData;
		if(pMSGToInvoke)
		{	
			//AQges.ima3A5U == ��� 
			//typedef struct _D48112_I
			//{
			//    char        sBuffer     [12];
			//    D48112_ISUB iData;
			//} D48112_I;

			D48112_I ChegyulIn;		
			memset(&ChegyulIn, 0x20, sizeof(D48112_I));

			CString strThisChart;
			strThisChart.Format("%d", pMSGToInvoke->m_pChartItem);
			
			memcpy(ChegyulIn.sBuffer, strThisChart, strThisChart.GetLength());			
			memcpy(ChegyulIn.iData.nxtGb	,	"0"				,	1);
			memcpy(ChegyulIn.iData.ordseq	,	"0"				,	1);
			memcpy(ChegyulIn.iData.querygb	,	"4"				,	1);		// ��ȸ ����   0:��ü, 1:�ŵ�, 2:�ż�, 3:��ü��, 4:ü��
			memcpy(ChegyulIn.iData.gejanum	,	"70101002312"	,	11);	//�� ���¹�ȣ.
			memcpy(ChegyulIn.iData.passwd	,	"AQges.ima3A5U"	,	13);	//���� ���

			CString strCode;
			strCode = "A" + m_strCode;
			memcpy(ChegyulIn.iData.jmcode, (LPTSTR)(LPCTSTR)strCode, strCode.GetLength());
			
			stSend2Server stRequest;

			stRequest.m_strTRCode	= "48112";
			stRequest.m_pBuffer		= (void*)&ChegyulIn;
			stRequest.m_lLength		= sizeof(D48112_I);	
			stRequest.m_pWnd		= pMSGToInvoke->m_pChartItem;
			stRequest.m_nType		= 1;
			
			::SendMessage(pMSGToInvoke->m_hChartMng, UMSG_SEND2SERVERFROMINVOKE, 0, (LPARAM)&stRequest);
		}		
*/
	}
	else if(p_nCommandType == INVOKEMSG_RECEIVEDATA) //��û�� ����Ÿ�� �޾Ƽ� ó���Ѵ�.
	{
		stMSGToInvoke * pMSGToInvoke = (stMSGToInvoke *)p_lData;	
		if(pMSGToInvoke)
		{
			stReceiveFromServer* pRcvFromSvr = (stReceiveFromServer*)pMSGToInvoke->m_pBuffer;
			if(pRcvFromSvr)
			{
				D48112_O*		pChegyulOut	= (D48112_O*)pRcvFromSvr->m_pBuffer;
				D48112_OSUB*	pChegyulSub	= (D48112_OSUB*)&(pChegyulOut->oData);

				//m_pmPatternMaker.SetChegyulList(pChegyulSub);
				pPatternMaker->SetChegyulList(pChegyulSub);
			}			
		}
	}	
	else if(p_nCommandType == -1 )	//Refresh
	{
		m_pIChartOCX->InvalidateControl();
		return TRUE;
	}
	else if(p_nCommandType == 104 )	//�� <-> % <-> $
	{	
		//m_pmPatternMaker.SetBWonChart(p_lData > 0 ? FALSE : TRUE);
		pPatternMaker->SetBWonChart(p_lData > 0 ? FALSE : TRUE);
		m_pIChartOCX->InvalidateControl();

		return TRUE;
	}
	//2007.02.06 by LYH ��ü�ʱ�ȭ�ø� ���� ����
	else if(p_nCommandType == 200 )	//��ü�ʱ�ȭ
	{	
		CString strFileName = m_strUsrPath + INI_FILENAME;
		CFileFind	ff;
		if(ff.FindFile(strFileName,0))
			CFile::Remove(strFileName);
		g_clConfigSet.InitMember();
		return TRUE;
	}
	
	m_pIChartOCX->InvalidateControl();
	return TRUE;
}

// �������� Real�ݿ� : �������� - ojtaso (20070131)
void CChartGuideLineAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	// (2004.10.13, ��¿�) �켱 �����Ѱ� ���� Message�� ������ �������� Ȯ���Ѵ�.
	// (2004.10.22, ��¿�) ����/���Ѱ� ǥ�� ���� ���� ���Ϻ� �ʱ�ȭ �����
	//			��� ���� ��Ʈ�� �ִ� ������ �ʱ�ȭ TR�� ���Ž����� �ش��Ѵ�.
	//		�� ��ɵ��� ���� ������Ʈ�� ����/������ ����/������ ����Ʈ�� �����ϸ鼭,
	//			��/���Ѱ��� ǥ���ϰų� ���� ���� ǥ�ø� Reset ��Ű�� ����� ó���ϴ� ���̴�.
	//		Ư�� �� ��ɵ��� ��Ʈ�� 1���� Graph���� ������ �� �ִ� �������
	//			(����� Data ������ AddIn �� 1���� ���ѵ� ������� �����̴�.)
	//			���� Packet�� �̿��ϴ� ������Ʈ���� �����Ǵ� ������� �����Ѵ�.
	//		*. ���� �ڼ��� ������ �� Class Header ����
	//if( !( BOOL)strstr( p_szPacketNames, "����;") || !p_bWithReset) return;	// (2006/2/22 - Seung-Won, Bae) Init I/O Packet.
	CString strClose;
	strClose.Format("%s;", _MTEXT(C0_CLOSE));
	if( !( BOOL)strstr( p_szPacketNames, strClose) || !p_bWithReset) return;	// (2006/2/22 - Seung-Won, Bae) Init I/O Packet.

	//m_pIChartOCX->InvalidateControl();
	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	CPatternMaker* pPatternMaker;
	GetPatternMaker(pPatternMaker, lpszRQ);

	if(!pPatternMaker)
		return;


	// �ֿ����, �ֿ���� ������ Ŭ����	JSJ_ADD_041223
	pPatternMaker->DeleteHighLowPtData();

	// �̵���� ������ ����	JSJ_ADD_041227
	pPatternMaker->DeleteMAData();

	// ����, ������ ������ ���� JSJ_ADD_050103
	pPatternMaker->DeleteRegistNSupportData();	

	pPatternMaker->Init_GuideLineValue();

	// (2008/9/5 - Seung-Won, Bae) Temporary for Daily MA.
	pPatternMaker->RequestSiseData();

	m_pIChartOCX->InvalidateControl();
}

// �������� Real�ݿ� : �������� - ojtaso (20070131)
void CChartGuideLineAddInImp::OnUpdatePacketDataMultiItem(LPCTSTR lpszRQ, BOOL p_bTrUpdate)
{
	//// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	//CPatternMaker* pPatternMaker;
	//GetPatternMaker(pPatternMaker, lpszRQ);

	//if(!pPatternMaker)
	//	return;

	//if(p_bTrUpdate==0&&pPatternMaker)
	//{
	//	if ( m_bRecvRealData_S31 )
	//	{
	//		m_bRecvRealData_S31 = FALSE;


	//		pPatternMaker->DeleteAllData();					// ���ϰ��� ������ ��� ����
	//		pPatternMaker->InitSelectedPatternData(TRUE);	// Init Selected Pattern Data

	//		// Get All WavePattern Data
	//		pPatternMaker->GetAllData_AboutPattern();

	//		// ������ �׸��� ���
	//		if ( !pPatternMaker->m_PatternEnvData.m_bNotDrawAll)
	//		{
	//			// �ǽð����� �׷��� ���õ� ������ �ִ��� üũ
	//			pPatternMaker->CheckSelectedPatternData_ForReal();
	//		}
	//	}
	//}
}

void CChartGuideLineAddInImp::LoadGlobalCfg()
{
	// ���� �о���� ***************************************************************************
	CFile		pfile;
	CString		strData, strTemp;
	char		szData[2096];
	int			nCount;
	int			nPos, nPos2;
/*	for(int nNum = 0; nNum < NUMBER_COUNT; nNum++)
	{
		m_ArrayNumberCount[nNum] = 0;
		m_ArrayPriceCount[nNum]	 = 0;
	}
*/	
	if( pfile.Open(m_strUsrPath + INI_FILENAME, CFile::modeRead) )
	{
		pfile.Read(szData, 2096);
		pfile.Close();

		strData = LPCTSTR(szData);
		strData.TrimLeft();		strData.TrimRight();

		// ȯ������ ������ �ε� ----------------------------------------------------------------->
		char*	szDataName[] = {	"CHK00","CHK01","CHK02","CHK03","CHK04",   
									"CHK05","CHK06","CHK07","CHK08","CHK09",   //10
									"CHK10","CHK11","CHK12","CHK13","CHK14",   
									"CHK15","CHK16","CHK17","CHK18","CHK19",   //20
									"CHK20","CHK21","CHK22","CHK23","CHK24",   
									"CHK25","CHK26","CHK27","CHK28","CLR00",   //30
									"CLR01","CLR02","CLR03","CLR04", "CLR05",  
									"CLR06","CLR07","CLR08","CLR09", "CLR10",  //40
									"CLR11","CLR12","CLR13","CLR14", "CLR15",  
									"CLR16","CLR17","CLR18","CLR19", "CLR20",  //50
									"CLR21","CLR22","CLR23","CLR24", "CLR25",  
									"CLR26","CLR27","CLR28","LN00" , "LN01" ,  //60
									"LN02" ,"LN03" ,"LN04" ,"LN05" , "LN06" ,
									"LN07" ,"LN08" ,"LN09" ,"LN10" , "LN11" ,  //70
									"LN12" ,"LN13" ,"LN14" ,"LN15" , "LN16" ,
									"LN17" ,"LN18" ,"LN19" ,"LN20" , "LN21" ,  //80	
									"LN22" ,"LN23" ,"LN24" ,"LN25" , "LN26" ,
									"LN27" ,"LN28" ,"YP00" ,"YP01" , "YP02" ,  //90
									"YP03" ,"DAN00","DAN01","ACCT00","NOT_DRAW"			   //95
							};
		
		nCount = sizeof(szDataName)/sizeof(char*);		
		CString sTTT;
		for(int i = 0; i < nCount; i++)
		{
			nPos  = strData.Find(szDataName[i]);
			if(nPos >= 0)
			{
				nPos  = strData.Find("=", nPos);
				nPos2 = strData.Find(";", nPos);
				if (nPos2 > nPos+1)
					strTemp = strData.Mid(nPos+1, nPos2-nPos-1);
				else
					strTemp = "";
			}
			else
			{
				strTemp = "";
			}

//			sTTT.Format("Cnt :%d - %s", i, strTemp);
//			OutputDebugString(sTTT);

			int nValue = atoi(strTemp);		
			switch(i)
			{				
				case  0: { g_clConfigSet.m_bChkYSprice		= nValue; } break;//���ϰ��� �ð�				üũ ����							
				case  1: { g_clConfigSet.m_bChkYHprice		= nValue; } break;//���ϰ��� ��				üũ ����
				case  2: { g_clConfigSet.m_bChkYLprice		= nValue; } break;//���ϰ��� ����				üũ ����
				case  3: { g_clConfigSet.m_bChkYEprice		= nValue; } break;//���ϰ��� ����				üũ ����
				case  4: { g_clConfigSet.m_bChkYSHL			= nValue; } break;//���ϰ��� (��+��+��)/3		üũ ����
				case  5: { g_clConfigSet.m_bChkTSprice		= nValue; } break;//���ϰ��� �ð�				üũ ����
				case  6: { g_clConfigSet.m_bChkTHprice		= nValue; } break;//���ϰ��� ��				üũ ����
				case  7: { g_clConfigSet.m_bChkTLprice		= nValue; } break;//���ϰ��� ����				üũ ����
				case  8: { g_clConfigSet.m_bChkTEprice		= nValue; } break;//���ϰ��� ����				üũ ����
				case  9: { g_clConfigSet.m_bChkTHL			= nValue; } break;//���ϰ��� (��+��)/2			üũ ����
				case 10: { g_clConfigSet.m_bChkTSHL			= nValue; } break;//���ϰ��� (��+��+��)/3		üũ ����
				case 11: { g_clConfigSet.m_bChkTHLE			= nValue; } break;//���ϰ��� (��+��+(��*2))/4	üũ ����					
				case 12: { g_clConfigSet.m_bChkHighLimit	= nValue; } break;//���Ѱ�						üũ ����
				case 13: { g_clConfigSet.m_bChkHighLimit	= nValue; } break;//���Ѱ�						üũ ����				
				case 14: { g_clConfigSet.m_bChkPivot2Rst	= nValue; } break;// �Ǻ� 2�� ����				üũ ���� 
				case 15: { g_clConfigSet.m_bChkPivot1Rst	= nValue; } break;// �Ǻ� 1�� ����				üũ ����
				case 16: { g_clConfigSet.m_bChkPivotGLine	= nValue; } break;// �Ǻ� ���ؼ�				üũ ����
				case 17: { g_clConfigSet.m_bChkPivot1GG		= nValue; } break;// �Ǻ� 1�� ����				üũ ����
				case 18: { g_clConfigSet.m_bChkPivot2GG		= nValue; } break;// �Ǻ� 2�� ����				üũ ����				
				case 19: { g_clConfigSet.m_bChkDMKPreHPrice = nValue;} break;// Demark �����			üũ ����
				case 20: { g_clConfigSet.m_bChkDMKGLine		= nValue;} break;// Demark ���ؼ�				üũ ����
				case 21: { g_clConfigSet.m_bChkDMKPreLPrice = nValue;} break;// Demark ��������			üũ ����				
				case 22: { g_clConfigSet.m_bChk2Pyung1		= nValue; } break;// �̵���� ù��° �Է°�		üũ ����
				case 23: { g_clConfigSet.m_bChk2Pyung2		= nValue; } break;// �̵���� �ι�° �Է°�		üũ ����
				case 24: { g_clConfigSet.m_bChk2Pyung3		= nValue; } break;// �̵���� ����° �Է°�		üũ ����
				case 25: { g_clConfigSet.m_bChk2Pyung4		= nValue; } break;// �̵���� �׹�° �Է°�		üũ ����									
				case 26: { g_clConfigSet.m_bChkPmmDanga		= nValue; } break;// ��ո��� �ܰ�				üũ ����
				case 27: { g_clConfigSet.m_bChkPmmPDanga	= nValue; } break;// ��ո��� �ܰ� +n%			üũ ����
				case 28: { g_clConfigSet.m_bChkPmmMDanga	= nValue; } break;// ��ո��� �ܰ� -n%			üũ ����				

				case 29: { g_clConfigSet.m_clrYSprice		= nValue; } break;	//���ϰ��� �ð�				�� ������
				case 30: { g_clConfigSet.m_clrYHprice		= nValue; } break;	//���ϰ��� ��				�� ������
				case 31: { g_clConfigSet.m_clrYLprice		= nValue; } break;	//���ϰ��� ����				�� ������
				case 32: { g_clConfigSet.m_clrYEprice		= nValue; } break;	//���ϰ��� ����				�� ������
				case 33: { g_clConfigSet.m_clrYSHL			= nValue; } break;	//���ϰ��� (��+��+��)/3		�� ������
				case 34: { g_clConfigSet.m_clrTSprice		= nValue; } break;	//���ϰ��� �ð�				�� ������
				case 35: { g_clConfigSet.m_clrTHprice		= nValue; } break;	//���ϰ��� ��				�� ������
				case 36: { g_clConfigSet.m_clrTLprice		= nValue; } break;	//���ϰ��� ����				�� ������
				case 37: { g_clConfigSet.m_clrTEprice		= nValue; } break;	//���ϰ��� ����				�� ������
				case 38: { g_clConfigSet.m_clrTHL			= nValue; } break;	//���ϰ��� (��+��)/2		�� ������
				case 39: { g_clConfigSet.m_clrTSHL			= nValue; } break;	//���ϰ��� (��+��+��)/3		�� ������
				case 40: { g_clConfigSet.m_clrTHLE			= nValue; } break;	//���ϰ��� (��+��+(��*2))/4 �� ������
				case 41: { g_clConfigSet.m_clrHighLimit		= nValue; } break;	//���Ѱ�					�� ������
				case 42: { g_clConfigSet.m_clrLowLimit		= nValue; } break;	//���Ѱ�					�� ������
				case 43: { g_clConfigSet.m_clrPivot2Rst		= nValue; } break;	// �Ǻ� 2�� ���� �� ������
				case 44: { g_clConfigSet.m_clrPivot1Rst		= nValue; } break;	// �Ǻ� 1�� ���� �� ������
				case 45: { g_clConfigSet.m_clrPivotGLine	= nValue; } break;	// �Ǻ� ���ؼ�	 �� ������
				case 46: { g_clConfigSet.m_clrPivot1GG		= nValue; } break;	// �Ǻ� 1�� ���� �� ������
				case 47: { g_clConfigSet.m_clrPivot2GG		= nValue; } break;	// �Ǻ� 2�� ���� �� ������		
				case 48: { g_clConfigSet.m_clrDMKPreHPrice	= nValue; } break;	// Demark �����  �� ������
				case 49: { g_clConfigSet.m_clrDMKGLine		= nValue; } break;	// Demark ���ؼ�	�� ������
				case 50: { g_clConfigSet.m_clrDMKPreLPrice	= nValue; } break;	// Demark ��������  �� ������	
				case 51: { g_clConfigSet.m_clr2Pyung1		= nValue; } break;	// �̵���� ù��° �Է°� �� ������
				case 52: { g_clConfigSet.m_clr2Pyung2		= nValue; } break;	// �̵���� �ι�° �Է°� �� ������
				case 53: { g_clConfigSet.m_clr2Pyung3		= nValue; } break;	// �̵���� ����° �Է°� �� ������
				case 54: { g_clConfigSet.m_clr2Pyung4		= nValue; } break;	// �̵���� �׹�° �Է°� �� ������							
				case 55: { g_clConfigSet.m_clrPmmDanga		= nValue; } break;	// ��ո��� �ܰ� �� ������
				case 56: { g_clConfigSet.m_clrPmmPDanga		= nValue; } break;	// ��ո��� �ܰ� +n% �� ������
				case 57: { g_clConfigSet.m_clrPmmMDanga		= nValue; } break;	// ��ո��� �ܰ� -n% �� ������

				case 58: { g_clConfigSet.m_nLineYSprice		= nValue;  } break;	//���ϰ��� �ð�				���� ũ��
				case 59: { g_clConfigSet.m_nLineYHprice		= nValue;  } break;	//���ϰ��� ��				���� ũ��
				case 60: { g_clConfigSet.m_nLineYLprice		= nValue;  } break;	//���ϰ��� ����				���� ũ��
				case 61: { g_clConfigSet.m_nLineYEprice		= nValue;  } break;	//���ϰ��� ����				���� ũ��
				case 62: { g_clConfigSet.m_nLineYSHL		= nValue;  } break;	//���ϰ��� (��+��+��)/3		���� ũ��
				case 63: { g_clConfigSet.m_nLineTSprice		= nValue;  } break;	//���ϰ��� �ð�				���� ũ��
				case 64: { g_clConfigSet.m_nLineYSprice		= nValue;  } break;	//���ϰ��� ��				���� ũ��
				case 65: { g_clConfigSet.m_nLineYHprice		= nValue;  } break;	//���ϰ��� ����				���� ũ��
				case 66: { g_clConfigSet.m_nLineYLprice		= nValue;  } break;	//���ϰ��� ����				���� ũ��
				case 67: { g_clConfigSet.m_nLineTHL			= nValue;  } break;	//���ϰ��� (��+��)/2		���� ũ��
				case 68: { g_clConfigSet.m_nLineTSHL		= nValue;  } break;	//���ϰ��� (��+��+��)/3		���� ũ��
				case 69: { g_clConfigSet.m_nLineTHLE		= nValue;  } break;	//���ϰ��� (��+��+(��*2))/4 ���� ũ��
				case 70: { g_clConfigSet.m_nLineHighLimit	= nValue;  } break;	//���Ѱ�					���� ũ��
				case 71: { g_clConfigSet.m_nLineLowLimit	= nValue;  } break;	//���Ѱ�					 ���� ũ��		
				case 72: { g_clConfigSet.m_nLinePivot2Rst	= nValue;  } break;	// �Ǻ� 2�� ���� ���� ũ��
				case 73: { g_clConfigSet.m_nLinePivot1Rst	= nValue;  } break;	// �Ǻ� 1�� ���� ���� ũ��
				case 74: { g_clConfigSet.m_nLinePivotGLine	= nValue;  } break;	// �Ǻ� ���ؼ�	 ���� ũ��
				case 75: { g_clConfigSet.m_nLinePivot1GG	= nValue;  } break;	// �Ǻ� 1�� ���� ���� ũ��
				case 76: { g_clConfigSet.m_nLinePivot2GG	= nValue;  } break;	// �Ǻ� 2�� ���� ���� ũ��		
				case 77: { g_clConfigSet.m_nLineDMKPreHPrice= nValue;  } break;	// Demark �����  ���� ũ��
				case 78: { g_clConfigSet.m_nLineDMKGLine	= nValue;  } break;	// Demark ���ؼ�	���� ũ��
				case 79: { g_clConfigSet.m_nLineDMKPreLPrice= nValue;  } break;	// Demark ��������  ���� ũ��	
				case 80: { g_clConfigSet.m_nLine2Pyung1		= nValue;  } break;	// �̵���� ù��° �Է°� ���� ũ��
				case 81: { g_clConfigSet.m_nLine2Pyung2		= nValue;  } break;	// �̵���� �ι�° �Է°� ���� ũ��
				case 82: { g_clConfigSet.m_nLine2Pyung3		= nValue;  } break;	// �̵���� ����° �Է°� ���� ũ��
				case 83: { g_clConfigSet.m_nLine2Pyung4		= nValue;  } break;	// �̵���� �׹�° �Է°� ���� ũ��				
				case 84: { g_clConfigSet.m_nLinePmmDanga	= nValue;  } break;	// ��ո��� �ܰ� ���� ũ��
				case 85: { g_clConfigSet.m_nLinePmmPDanga	= nValue;  } break;	// ��ո��� �ܰ� +n% ���� ũ��
				case 86: { g_clConfigSet.m_nLinePmmMDanga	= nValue;  } break;	// ��ո��� �ܰ� -n% ���� ũ��			

				case 87: { g_clConfigSet.m_nDay2Pyung1		= nValue; } break;	//�̵���� 1��° 		��ġ(�Ⱓ) ��
				case 88: { g_clConfigSet.m_nDay2Pyung2		= nValue; } break;	//�̵���� 2��° 		��ġ(�Ⱓ) ��
				case 89: { g_clConfigSet.m_nDay2Pyung3		= nValue; } break;	//�̵���� 3��° 		��ġ(�Ⱓ) ��
				case 90: { g_clConfigSet.m_nDay2Pyung4		= nValue; } break;	//�̵���� 4��° 		��ġ(�Ⱓ) ��
				case 91: { g_clConfigSet.m_nPmmPDanga		= nValue; } break;	// ��ո��� �ܰ� +n%	��
				case 92: { g_clConfigSet.m_nPmmMDanga		= nValue; } break;	// ��ո��� �ܰ� -n%	��	

				case 93: { g_clConfigSet.m_strAcct			= strTemp; } break;	// ����	
				case 94: { g_clConfigSet.m_bNotDrawLine		= nValue ; } break; // ���̵� ���� �׸��� 	üũ ����				
			}		
		}

		//UpdateData(FALSE);
		//g_clConfigSet.SetConfigValue(&m_clConfigSet);
		// <---------------------------------------------------------------------------------
		
	}

	
	//*****************************************************************************************
}

//{{ ��Ʈ �ʱ�ȭ ó�� by LYH 2006.12.22
void CChartGuideLineAddInImp::OnResetChart()
{
//	CString strFileName = m_strUsrPath + INI_FILENAME;
//	CFile		pfile;
//	CFileFind ff;
//	if(ff.FindFile(strFileName,0))
//	{
//		pfile.Remove(strFileName);
//		pfile.Close();
//	}
//	g_clConfigSet.InitMember();
}
//}}

// �ش� RQ�� PatternMaker : �������� - ojtaso (20070305)
void CChartGuideLineAddInImp::GetPatternMaker(CPatternMaker*& pPatternMaker, LPCTSTR lpszRQ/* = NULL*/)
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
BOOL CChartGuideLineAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam)
{
	if(p_eCommandID == EAI_PACKETLISTMANAGER_NEW_RQ)
	{
		CPatternMaker* pPatternMaker;
		GetPatternMaker(pPatternMaker, p_szParam);
		if(pPatternMaker != NULL)
			return FALSE;

		ML_SET_LANGUAGE_RES();
		pPatternMaker = new CPatternMaker;
		pPatternMaker->SetIChartOCX( m_pIChartManager, m_pIChartOCX,  m_pIDefaultPacketManager);
		pPatternMaker->SetRQ(p_szParam);

		m_mapPatternMaker.insert(MAP_PATTERNMAKER::value_type(p_szParam, pPatternMaker));
		m_lpszRQ = p_szParam;
	}
	else if(p_eCommandID == EAI_PACKETLISTMANAGER_CHANGE_RQ)
	{
		m_lpszRQ = p_szParam;
	}
	else if(p_eCommandID == EAI_PACKETLISTMANAGER_DELETE_RQ)
	{
		CPatternMaker* pPatternMaker;
		GetPatternMaker(pPatternMaker, p_szParam);

		if(pPatternMaker)
		{
			delete pPatternMaker;
		}

		m_mapPatternMaker.erase(p_szParam);
	}

	return FALSE;
}

// (2008/9/5 - Seung-Won, Bae) Support All Chart Sync for MA.
void CChartGuideLineAddInImp::OnChangeGuideLineSetting( BOOL p_bWithMAPeriodChange)
{
	CPatternMaker* pPatternMaker;
	MAP_PATTERNMAKER::iterator it = m_mapPatternMaker.begin();
	while(it != m_mapPatternMaker.end())
	{
		pPatternMaker = (*it).second;

		if( p_bWithMAPeriodChange) pPatternMaker->RequestMaData();
		pPatternMaker->DeleteLinePen();
		pPatternMaker->CreateLinePen();
		it++;
	}
}

// (2008/10/26 - Seung-Won, Bae) for O/H/L/C (not LH/LC/HC)
void CChartGuideLineAddInImp::GetShowOHLC( BOOL &bDrawOHLC, COLORREF &clrUp, COLORREF &clrDown, const char *p_szRQ)
{
	// (2008/11/3 - Seung-Won, Bae) for multiple indicator.
	IChartManager *pIChartManager = m_pIChartOCX->GetIChartManager();
	if( !pIChartManager) return;
	int nR = -1, nC = -1, nV = -1, nG = -1;
	ILPCSTR szGraphName = pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, p_szRQ);
	pIChartManager->Release();

	IGraphInfoManager *pIGraphInfoManager = m_pIChartOCX->GetIGraphInfoManager();
	if( !pIGraphInfoManager) return;
	IGraphInfo *pIPriceChart = pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, szGraphName);
	if( !pIPriceChart) pIPriceChart = pIGraphInfoManager->GetIndicatorInfo( IGraphInfoManager::IEILT_SITE_DEFAULT, _MTEXT( C2_PRICE_CHART), IGraphInfoManager::IEIIGT_WITHOUT_DEFAULT);
	pIGraphInfoManager->Release();
	if( !pIPriceChart) return;

	BOOL bLowHigh = FALSE, bLowCur = FALSE, bHighCur = FALSE, bPrevCur = FALSE;
	int nCurData = 0;
	// (2009/9/12 - Seung-Won, Bae) for None Draw.
	pIPriceChart->GetGraphDataDrawingData( bLowHigh, bLowCur, bHighCur, bPrevCur, nCurData, bDrawOHLC);
	clrUp = pIPriceChart->GetSubGraphColor( 0, 1);
	clrDown = pIPriceChart->GetSubGraphColor( 0, 4);
	pIPriceChart->Release();
}
BOOL CChartGuideLineAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID)
{
	if( EAI_ENVSETUP_LHLCHC_CHANGED != p_eCommandID
		&& EAI_ALL_ENVIRONMENT_CHANGED != p_eCommandID) return FALSE;

	MAP_PATTERNMAKER::iterator it = m_mapPatternMaker.begin();
	while(it != m_mapPatternMaker.end())
	{
		BOOL bDrawOHLC = FALSE;
		COLORREF clrUp = RGB( 255, 0, 0);
		COLORREF clrDown = RGB( 0, 0, 255);
		GetShowOHLC( bDrawOHLC, clrUp, clrDown, (*it).second->GetRQ());

		(*it).second->SetDrawOHLC( bDrawOHLC, clrUp, clrDown);
		it++;
	}
	
	return FALSE;
}
