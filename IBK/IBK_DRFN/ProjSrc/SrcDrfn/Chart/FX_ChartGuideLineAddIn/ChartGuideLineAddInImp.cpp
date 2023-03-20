// ChartStockOpinionAddInImp.cpp: implementation of the CChartGuideLineAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "ChartGuideLineAddIn.h"
#include "ChartGuideLineAddInImp.h"
#include "./Include_AddIn.h"							// for Ixxxxxx.h
#include "../CommonTR/Data_Common.h"

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

extern HINSTANCE g_hInstance;
typedef LPCSTR (* FpGetUnitPrice)(LPSTR, LPSTR);
FpGetUnitPrice pGetUnitPrice = NULL;

CChartGuideLineAddInImp::CChartGuideLineAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	//<<< 20090827 by kmlee
	/*
	ILPCSTR szUsrPath = p_pIChartOCX->GetUserFilePathString();
	m_strUsrPath.Format("%s\\", szUsrPath);
	*/
//<<20100326 �ַθ�	
//	m_strUsrPath = (LPSTR)(LPCTSTR)p_pIChartOCX->GetHtsPathString();
	ILPCSTR szUsrPath = p_pIChartOCX->GetUserFilePathString();
	m_strUsrPath.Format("%s\\", szUsrPath);
//>>	

	LoadGlobalCfg();

	m_lpszRQ = _T("DEFAULT");	// ���� RQ : �������� - ojtaso (20070305)
	CPatternMaker* pPatternMaker = new CPatternMaker;
	pPatternMaker->SetIChartOCX( m_pIChartManager, m_pIChartOCX, m_pIDefaultPacketManager);
	pPatternMaker->SetRQ(m_lpszRQ);
	m_mapPatternMaker.SetAt(m_lpszRQ, pPatternMaker);
}

CChartGuideLineAddInImp::~CChartGuideLineAddInImp()
{
	CString rKey;
	void* rValue;
	for(POSITION pos=m_mapPatternMaker.GetStartPosition(); pos; )
	{
		m_mapPatternMaker.GetNextAssoc(pos, rKey, rValue);
		CPatternMaker* pData = (CPatternMaker*)rValue;
		delete pData;
	}
	m_mapPatternMaker.RemoveAll();
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CChartGuideLineAddInImp::m_strAddInItemName = "GUIDE_LINE";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CChartGuideLineAddInImp)
	ONADDINEVENT(OnDrawAfterGraph)	
	ONADDINEVENT(OnDrawBeforeGraph)
	ONADDINEVENT(OnPacketData)
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
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////


BOOL CChartGuideLineAddInImp::OnMouseMove(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	return FALSE;
}

BOOL CChartGuideLineAddInImp::OnLButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	return FALSE;
}

BOOL CChartGuideLineAddInImp::OnLButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	return FALSE;
}

BOOL CChartGuideLineAddInImp::OnRButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
		
	return FALSE;
}

BOOL CChartGuideLineAddInImp::OnRButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	return FALSE;
}

/*
BOOL CChartGuideLineAddInImp::OnLButtonDblClk(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());


	return FALSE;
}
*/
void CChartGuideLineAddInImp::OnDrawBeforeGraph(HDC p_hDC)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
}

void CChartGuideLineAddInImp::OnDrawAfterGraph(HDC p_hDC)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	CDC * pDC = CDC::FromHandle(p_hDC);

	CString rKey;
	void* rValue;
	for(POSITION pos=m_mapPatternMaker.GetStartPosition(); pos; )
	{
		m_mapPatternMaker.GetNextAssoc(pos, rKey, rValue);
		CPatternMaker* pData = (CPatternMaker*)rValue;
		pData->OnDraw( pDC);
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
		m_mapPatternMaker.SetAt(m_lpszRQ, pPatternMaker);
	}
//@Solomon:091123SM068	-->
/*	INVOKEMSG_RECEIVEDATA �� ��ġ�� ����. ��������ʴ� �� ����.
	if(p_nCommandType == 101)
	{
		pPatternMaker->SetHwndSelChatItem((HWND)p_lData);
	}
	else 
*/
//@Solomon:091123SM068	<--
	if(p_nCommandType == 0)
	{		
		//====================================================================
		CStringList* pCStringList = NULL;
		pCStringList = (CStringList*)p_lData;
		if(pCStringList == NULL) return FALSE;
		
		POSITION pos1 = NULL;
		for( pos1 = pCStringList->GetHeadPosition(); pos1;)
		{	
			sTemp = pCStringList->GetNext( pos1 );
			strFullAcct += sTemp;
		}
		//====================================================================
		
		// (2004.10.19, ��¿�) Resource�� ������ �ߵǵ��� Module State�� �����Ѵ�.
		int		nOldDay2Pyung1		= g_clConfigSet.m_nDay2Pyung1;
		int		nOldDay2Pyung2		= g_clConfigSet.m_nDay2Pyung2;
		int		nOldDay2Pyung3		= g_clConfigSet.m_nDay2Pyung3;
		int		nOldDay2Pyung4		= g_clConfigSet.m_nDay2Pyung4;
		BOOL	bOldbChk2Pyung1		= g_clConfigSet.m_bChk2Pyung1;
		BOOL	bOldbChk2Pyung2		= g_clConfigSet.m_bChk2Pyung2;
		BOOL	bOldbChk2Pyung3		= g_clConfigSet.m_bChk2Pyung3;
		BOOL	bOldbChk2Pyung4		= g_clConfigSet.m_bChk2Pyung4;

		HINSTANCE hInstResourceClient = AfxGetResourceHandle();
		AfxSetResourceHandle(g_hInstance);

		CDlgSetGuideLine dlgSetGuideLine;
		
		//���� ���� ǥ��.
		dlgSetGuideLine.m_strCfgFile  = m_strUsrPath + INI_FILENAME;
		dlgSetGuideLine.m_strFullAcct = strFullAcct;
		if( dlgSetGuideLine.DoModal() != IDOK )
		{
			pPatternMaker->SetHwndSelChatItem(NULL);
			AfxSetResourceHandle(hInstResourceClient);
			return FALSE;
		}
		AfxSetResourceHandle(hInstResourceClient);
		//CStringList�� �̵���� �Ⱓ���� ����Ǿ� �ִ��� ���θ� ������. 0. 1�� ����.
		if( ( nOldDay2Pyung1	!= g_clConfigSet.m_nDay2Pyung1) ||
			( nOldDay2Pyung2	!= g_clConfigSet.m_nDay2Pyung2) ||
			( nOldDay2Pyung3	!= g_clConfigSet.m_nDay2Pyung3) ||
			( nOldDay2Pyung4	!= g_clConfigSet.m_nDay2Pyung4) ||
			( bOldbChk2Pyung1	!= g_clConfigSet.m_bChk2Pyung1) ||
			( bOldbChk2Pyung2	!= g_clConfigSet.m_bChk2Pyung2) ||
			( bOldbChk2Pyung3	!= g_clConfigSet.m_bChk2Pyung3) ||
			( bOldbChk2Pyung4	!= g_clConfigSet.m_bChk2Pyung4) 
		)
		{	
			//m_pmPatternMaker.MakeMAData(m_pIChartManager, m_pIPacketManager, m_pIChartOCX);
			pPatternMaker->MakeMAData(m_pIChartManager, m_pIDefaultPacketManager, m_pIChartOCX);
			pos1 = pCStringList->GetHeadPosition();
			pCStringList->SetAt(pos1, "1"); // �̵� ��� ���� ���� �ϳ��� �� ����Ǿ����� "1"�� ����
		}
		else
		{
			pos1 = pCStringList->GetHeadPosition();
			pCStringList->SetAt(pos1, "0"); // �̵� ��� ���� ���� �ϳ��� �� ����Ǿ����� "0"�� ����
		}

		pPatternMaker->RequestSiseData();
		pPatternMaker->DeleteLinePen();
		pPatternMaker->CreateLinePen();

		//��Ʈ ��ü�� �ݿ����� �ʴ´�. 
		g_clChartMngr.Refresh_AllChart();

	}
	else if(p_nCommandType == 1 )
	{
		CStringList* pCStringList = NULL;
		pCStringList = (CStringList*)p_lData;
		if(pCStringList == NULL) return FALSE;
		
		pPatternMaker->Init_GuideLineValue();
		POSITION pos1 = NULL;
		int nCnt = 0;
		for( pos1 = pCStringList->GetHeadPosition(); pos1;)
		{	
			if( nCnt == 0)
			{
				//���� ����
				sTemp = pCStringList->GetNext( pos1 );
				strFullAcct += sTemp;
			}
			else if(nCnt == 1)
			{
				OutputDebugString("���򰻽�");
				//�̵� ��� ��  ����
				sTemp = pCStringList->GetNext( pos1 );
				D10418_O* pMAvgInfo = NULL;
				pMAvgInfo = (D10418_O*)(LPTSTR)(LPCTSTR)sTemp;
				pPatternMaker->SetMAInfo(pMAvgInfo);
			}
			else
				break;
			nCnt++;
		}		
		
		pPatternMaker->GetBasicData_ForGuideLine( m_pIChartManager, m_pIDefaultPacketManager, m_pIChartOCX);
	}
	else if(p_nCommandType == 2 )
	{
		CUIntArray* pArrayAvg = NULL;
		pArrayAvg = (CUIntArray*)p_lData;
		pArrayAvg->SetSize(4);
		pArrayAvg->SetAt(0, g_clConfigSet.m_bChk2Pyung1 ? (UINT)g_clConfigSet.m_nDay2Pyung1 : 0);
		pArrayAvg->SetAt(1, g_clConfigSet.m_bChk2Pyung2 ? (UINT)g_clConfigSet.m_nDay2Pyung2 : 0);
		pArrayAvg->SetAt(2, g_clConfigSet.m_bChk2Pyung3 ? (UINT)g_clConfigSet.m_nDay2Pyung3 : 0);
		pArrayAvg->SetAt(3, g_clConfigSet.m_bChk2Pyung4 ? (UINT)g_clConfigSet.m_nDay2Pyung4 : 0);
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
			pPatternMaker->SetHwndCurChatItem(pMSGToInvoke->m_pChartItem->GetSafeHwnd());
		}
	
		
	}
	else if(p_nCommandType == 	INVOKEMSG_CHANGECODE) //���� �ڵ尡 �ٲ����.
	{
		stMSGToInvoke* pMSGToInvoke = NULL;
		pMSGToInvoke = (stMSGToInvoke*)p_lData;
		if(pMSGToInvoke)
		{			
			CString strThisChart;
			strThisChart.Format("%d", pMSGToInvoke->m_pChartItem);
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
		stMSGToInvoke* pMSGToInvoke  =  NULL;
		pMSGToInvoke = (stMSGToInvoke*)p_lData;
		if(pMSGToInvoke)
		{	
			D48112_I ChegyulIn;		
			memset(&ChegyulIn, 0x20, sizeof(D48112_I));

			CString strThisChart;
			strThisChart.Format("%d", pMSGToInvoke->m_pChartItem);

			
			
			//############################################################
			//���� ���� ��ȸ - �Ｚ ��������� ���Ͽ��� �о�´�.
			//############################################################
			
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
	}
	else if(p_nCommandType == INVOKEMSG_RECEIVEDATA) //��û�� ����Ÿ�� �޾Ƽ� ó���Ѵ�.
	{
		stMSGToInvoke * pMSGToInvoke = (stMSGToInvoke *)p_lData;	
		if(pMSGToInvoke)
		{
			stReceiveFromServer* pRcvFromSvr = (stReceiveFromServer*)pMSGToInvoke->m_pBuffer;
			if(pRcvFromSvr)
			{
//@Solomon	-->
				if (pRcvFromSvr->m_strTRCode.CompareNoCase(_T("d1017")) == 0)
					pPatternMaker->ReceiveSiseData(pRcvFromSvr->m_pBuffer, pRcvFromSvr->m_dwLength);
				else
				{
					D48112_O*		pChegyulOut	= (D48112_O*)pRcvFromSvr->m_pBuffer;
					D48112_OSUB*	pChegyulSub	= (D48112_OSUB*)&(pChegyulOut->oData);
					
					pPatternMaker->SetChegyulList(pChegyulSub);
				}
//@Solomon	<--
			}			
		}
	}	
	else if(p_nCommandType == -1 )	//Refresh
	{
		pPatternMaker->GetBasicData_ForGuideLine( m_pIChartManager, m_pIDefaultPacketManager, m_pIChartOCX);
		m_pIChartOCX->InvalidateControl();
		return TRUE;
	}
	else if(p_nCommandType == 104 )	//�� <-> % <-> $
	{	
		pPatternMaker->SetBWonChart(p_lData > 0 ? FALSE : TRUE);
		m_pIChartOCX->InvalidateControl();

		return TRUE;
	}
	//2007.02.06 by LYH ��ü�ʱ�ȭ�ø� ���� ����
	else if(p_nCommandType == 200 )	//��ü�ʱ�ȭ
	{	
		CString strFileName = m_strUsrPath + INI_FILENAME;
		CFile		pfile;
		CFileFind ff;
		if(ff.FindFile(strFileName,0))
		{
			pfile.Remove(strFileName);
		}
		g_clConfigSet.InitMember();
		return TRUE;
	}
	//}}
	else if(p_nCommandType == INVOKEMSG_RECEIVEREALDATA) // RealData����
	{
		STREALTYPEDATA *prData = (STREALTYPEDATA *)p_lData;
		InvokeRealData(prData->nDataType, prData->szCode, prData->oPrice, prData->hPrice, prData->lPrice, prData->cPrice);
		return TRUE;
	}
	
	m_pIChartOCX->InvalidateControl();
	return TRUE;
}

void CChartGuideLineAddInImp::OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset)
{
}

// �������� Real�ݿ� : �������� - ojtaso (20070131)
void CChartGuideLineAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	//@Solomon:091123SM068	-->
	//<<20100327_JS.Kim. ������ ���ָ� ���񺯰�ÿ� �̵������ ���� ���ϹǷ� ���� ������ �־��ش�
	// �ش� RQ�� AssistIndexManager : �������� - ojtaso (20070305)
	CPatternMaker* pPatternMaker;
	GetPatternMaker(pPatternMaker, lpszRQ);

	if(!pPatternMaker)
		return;

//	pPatternMaker->MakeMAData(m_pIChartManager, m_pIDefaultPacketManager, m_pIChartOCX);

	pPatternMaker->GetBasicData_ForGuideLine( m_pIChartManager, m_pIDefaultPacketManager, m_pIChartOCX);

	m_pIChartOCX->InvalidateControl();
	//>>
/*
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
	pPatternMaker->GetBasicData_ForGuideLine( m_pIChartManager, m_pIDefaultPacketManager, m_pIChartOCX);

	m_pIChartOCX->InvalidateControl();
*/
//@Solomon:091123SM068	<--
}

// �������� Real�ݿ� : �������� - ojtaso (20070131)
void CChartGuideLineAddInImp::OnUpdatePacketDataMultiItem(LPCTSTR lpszRQ, BOOL p_bTrUpdate)
{
}

void CChartGuideLineAddInImp::LoadGlobalCfg()
{
	// ���� �о���� ***************************************************************************
	CFile		pfile;
	CString		strData, strTemp;
	char		szData[2096];
	int			nCount;
	int			nPos, nPos2;
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
				case 64: { g_clConfigSet.m_nLineTHprice		= nValue;  } break;	//���ϰ��� ��				���� ũ��//20100329_JS.Kim ���׼���
				case 65: { g_clConfigSet.m_nLineTLprice		= nValue;  } break;	//���ϰ��� ����				���� ũ��//20100329_JS.Kim ���׼���
				case 66: { g_clConfigSet.m_nLineTEprice		= nValue;  } break;	//���ϰ��� ����				���� ũ��//20100329_JS.Kim ���׼���
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

		// <---------------------------------------------------------------------------------
		
	}

	
	//*****************************************************************************************
}

//{{ ��Ʈ �ʱ�ȭ ó�� by LYH 2006.12.22
void CChartGuideLineAddInImp::OnResetChart()
{
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

	void* rValue=NULL;
	m_mapPatternMaker.Lookup(lpszRQ, rValue);
	if(rValue==NULL)
	{
		if(bRetunNull)
		{
			pPatternMaker = NULL;
			return;
		}
		else
		{
			m_mapPatternMaker.Lookup(_T("DEFAULT"), rValue);
			if(rValue==NULL)
			{
				pPatternMaker = NULL;
				return;
			}
		}
	}
	pPatternMaker = (CPatternMaker*)rValue;
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

		pPatternMaker = new CPatternMaker;
		pPatternMaker->SetIChartOCX( m_pIChartManager, m_pIChartOCX,  m_pIDefaultPacketManager);
		pPatternMaker->SetRQ(p_szParam);

		m_mapPatternMaker.SetAt(p_szParam, pPatternMaker);
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

		m_mapPatternMaker.RemoveKey(p_szParam);
	}

	return FALSE;
}

void CChartGuideLineAddInImp::InvokeRealData(int nMarketGubun, LPSTR szCode, LPSTR oPrice, LPSTR hPrice, LPSTR lPrice, LPSTR cPrice)
{
	CString rKey;
	void* rValue;
	for(POSITION pos=m_mapPatternMaker.GetStartPosition(); pos; )
	{
		m_mapPatternMaker.GetNextAssoc(pos, rKey, rValue);
		CPatternMaker* pPatternMaker = (CPatternMaker*)rValue;
		if(pPatternMaker && pPatternMaker->GetCode() == szCode)
		{
			pPatternMaker->UpdateRecvRealOpen(oPrice);
			pPatternMaker->UpdateRecvRealHigh(hPrice);
			pPatternMaker->UpdateRecvRealLow(lPrice);
			pPatternMaker->UpdateRecvRealCurPrice(cPrice);

			pPatternMaker->RefreshChart();
		}
	}
}
