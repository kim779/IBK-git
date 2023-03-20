// BaseNetworkModule.cpp : implementation file
//

#include "stdafx.h"
#include "networkdefine.h"
#include "BaseNetworkModule.h"
#include "NetworkModule.h"
#include "FONetworkModule.h"
#include "global.h"
#include "CommonFrameWorkWnd.h"
#include "exportfunction.h"
#include <math.h>
#include "ClosingNoticeDlg.h"
#include "../../h/axisfire.h"
#include "../../control/fx_misc/miscType.h"

//#include "SetupSheet.h"
#include "../../h/jmcode.h"	// LJP 20080130

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseNetworkModule
#define	TIMER		60000

#define	TEST_TIMER			1
#define	TEST_TIMER_INTERVAL	30000

#define WM_MANAGE	(WM_USER + 1001)
#define DLG_SET_DATA	99999

#define	OPCODE_LEN	8

CBaseNetworkModule::CBaseNetworkModule(CWnd* pParent)
{
	RegisterWndClass(AfxGetApp()->m_hInstance);

	m_pNetwork = NULL;

	m_bReceiveBaseData = FALSE;

	m_bEnableNetwork = TRUE;

	m_nTradeTimeType = 1;//������ð� ����.(1-����, 2-������)

	m_pCommonFrameWork = NULL;

	m_pClosingDlg = NULL;

	m_pHistoricalWnd = NULL;

	m_pFrameWnd = pParent;

	m_strATM = "";

	m_pPastDataOfUnderlyingAssets = NULL;

	m_nBizDayCount = 0;

	m_hVolateSelDlg = NULL;

	m_strKP200Time  = "";//�������� �����Ͱ� �ȵ����� ���� Ȯ��

	//20080314 �̹��� >>
	m_dTimeLoss      = 0.;
	//20080314 �̹��� <<
}

CBaseNetworkModule::~CBaseNetworkModule()
{
}


BEGIN_MESSAGE_MAP(CBaseNetworkModule, CWnd)
	//{{AFX_MSG_MAP(CBaseNetworkModule)
	ON_WM_CREATE()
	ON_WM_COPYDATA()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DIALOGDESTORYED,	OnDialogDestroyed)	
	ON_MESSAGE(WM_CLOSE_BASENETWORKMODULE,	OnCloseBaseNetworkModule)	
	ON_MESSAGE(WM_SHOW_CLOSE_DLG,	OnShowCloseDlg)
	ON_MESSAGE(WMM_GET_THIS_WND_PTR,OnGetBaseNetworkModule)	
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()

//	ON_MESSAGE(WMU_RTM_PACKET,		OnReceiveRTM)

long CBaseNetworkModule::OnGetBaseNetworkModule(WPARAM wParam, LPARAM lParam)
{
	return (long)this;
}

/////////////////////////////////////////////////////////////////////////////
// CBaseNetworkModule message handlers
BOOL CBaseNetworkModule::RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.lpszClassName	= "HADT_BASE_NETWORK_MODULE"; // �ϳ����� ���̽� ��Ʈ�� ���
	wc.hInstance		= hInstance;
	wc.lpfnWndProc		= ::DefWindowProc;
	wc.hCursor			= ::LoadCursor(NULL,IDC_ARROW);
	wc.hIcon			= 0;
	wc.lpszMenuName		= NULL;	
	wc.hbrBackground	= (HBRUSH)::GetStockObject(WHITE_BRUSH);	
	wc.style			= CS_GLOBALCLASS | CS_DBLCLKS;
	wc.cbClsExtra		= 0;
	////////////////////////////////////////
	wc.cbWndExtra		= sizeof(HWND);

	return (::RegisterClass(&wc) != 0);
}

BOOL CBaseNetworkModule::CreateBaseNetworkWnd(int nID, CWnd * pParentWnd)
{
	return Create("HADT_BASE_NETWORK_MODULE","HADT_FO_NETWORK_WND", WS_CAPTION|WS_CHILD |WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS ,
		CRect(0,0,0,0), pParentWnd, nID);
}

BOOL CBaseNetworkModule::RequestBaseData()
{
	if(!m_pNetwork) return FALSE;

	m_strRqCodeArray.RemoveAll();

	m_pNetwork->RequestBaseData();

	return TRUE;
}

int CBaseNetworkModule::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	::SetFactorialArray();

	// LJP 20080130 Start -->
	m_strHomeDir = Variant(homeCC);					// client root ���丮
	m_strUser	 = Variant(nameCC);	
	m_strId		 = Variant(userCC);

	ReadFutCode();
	ReadOptCode();
	ReadSOptCode();

	SortSOptItem();	

	m_BaseDataSet.m_pRTDataArray = &m_RTDataArray;

	m_pNetwork = new CNetworkModule(this, m_pFrameWnd);
	m_pNetwork->SetMode(BASE_NETWORK_MODULE_MODE);

	m_pCommonFrameWork = new CCommonFrameWorkWnd;
	m_pCommonFrameWork->SetAppPath(m_strHomeDir, m_strUser);
	m_pCommonFrameWork->CreateWnd(10000, this);

	RequestBaseData();


	//TEST
#ifdef __TEST__
	SetTimer(TEST_TIMER, TEST_TIMER_INTERVAL, NULL);
#endif
	return 0;
}

//20080316 �̹��� >>
BOOL CBaseNetworkModule::RequestData(CString strCode, HWND hRqWnd)
{
	if(!m_pNetwork || !m_pNetwork->m_pKeyManager) return FALSE;


	{
		m_pNetwork->m_bAutoMakeKey = FALSE;
		m_pNetwork->m_nKey = m_pNetwork->m_pKeyManager->GetKey(strCode);
		
		for(POSITION pos = m_KeyToWndList.GetHeadPosition(); pos ; )
		{
			CKeyToWndInfo Info = m_KeyToWndList.GetNext(pos);
			if(Info.m_nKey == m_pNetwork->m_nKey)
			{
				for(int i = 0 ; i < Info.m_hWndArray.GetSize(); i++ )
				{
					HWND hWnd = (HWND)Info.m_hWndArray.GetAt(i);
					if(hWnd == hRqWnd) //���� ��û�� ���� ������ ������ �Ǵ��ϰ� ����.
						return FALSE;
				}	

				//Ű �ߺ��� ������ �������ڵ� �ߺ��� ���� ���.
				Info.m_hWndArray.Add((UINT)hRqWnd);
				break;
			}
		}

		//Ű �ߺ��� ���� ���.
		if(!pos)
		{		
			CKeyToWndInfo Info(m_pNetwork->m_nKey, hRqWnd);
			m_KeyToWndList.AddTail(Info);
		}
	}

	int nResult = m_pNetwork->RequestData(strCode);	
	m_pNetwork->m_bAutoMakeKey = TRUE;
	return nResult;
}
//20080316 �̹��� <<

BOOL CBaseNetworkModule::RequestData()
{
	if(!m_pNetwork) return FALSE;	
	if(m_strRqCodeArray.GetSize() <= 0) return FALSE;

	CString strCode = m_strRqCodeArray.GetAt(0);

	int nCodeType = m_pNetwork->GetCodeType(strCode); // �����ڵ����� ����
	
	//20080316 �̹��� >>
	/*
	if(nCodeType != KP200_CODE && nCodeType != KOSPI_CODE && m_pNetwork->m_pKeyManager)
	{
		m_pNetwork->m_bAutoMakeKey = FALSE;
		m_pNetwork->m_nKey = m_pNetwork->m_pKeyManager->GetKey(strCode);
		
		for(POSITION pos = m_KeyToWndList.GetHeadPosition(); pos ; )
		{
			CKeyToWndInfo Info = m_KeyToWndList.GetNext(pos);
			if(Info.m_nKey == m_pNetwork->m_nKey)
			{
				for(int i = 0 ; i < Info.m_hWndArray.GetSize(); i++ )
				{
					HWND hWnd = (HWND)Info.m_hWndArray.GetAt(i);
					if(hWnd == GetSafeHwnd()) //���� ��û�� ���� ������ ������ �Ǵ��ϰ� ����.
						return FALSE;
				}	

				//Ű �ߺ��� ������ �������ڵ� �ߺ��� ���� ���.
				Info.m_hWndArray.Add((UINT)GetSafeHwnd());
				break;
			}
		}

		//Ű �ߺ��� ���� ���.
		if(!pos)
		{		
			CKeyToWndInfo Info(m_pNetwork->m_nKey, GetSafeHwnd());
			m_KeyToWndList.AddTail(Info);
		}
	}
	int nResult = m_pNetwork->RequestData(strCode);	
	m_pNetwork->m_bAutoMakeKey = TRUE;
	return nResult;
	*/
	return m_pNetwork->RequestData(strCode);	
	//20080316 �̹��� <<
}

BOOL CBaseNetworkModule::RequestKP200Data()
{
	if(!m_pNetwork) return FALSE;
	return m_pNetwork->RequestKP200Data();
}

BOOL CBaseNetworkModule::ReceiveAverageIV(WPARAM wParam, LPARAM lParam)
{
// 	CAverageIV AveIV;
// 	if(m_pNetwork->ReceiveData(wParam, lParam, AveIV))
// 	{
// 		m_AverageIV = AveIV;				
// 		m_bEnableNetwork = TRUE;
// 		
// 		if(atoi(m_AverageIV.m_strType) != 2) 
// 			SetTimer(0, TIMER, NULL);
// 	}
// 	else
// 	{
// 		m_bEnableNetwork = TRUE;
// 		SetTimer(0, TIMER, NULL);
// 	}
	return TRUE;
}

BOOL CBaseNetworkModule::ReceiveData(WPARAM wParam, LPARAM lParam)
{
	if(!m_pNetwork) return FALSE;
	if(m_strRqCodeArray.GetSize() <= 0) return FALSE;
	CString strCode = m_strRqCodeArray.GetAt(0);

	CDataSet * pDataSet = new CDataSet;
	pDataSet->m_strCode = strCode;
	if(!m_pNetwork->ReceiveData(wParam, lParam, pDataSet))
	{
		delete pDataSet;
		return FALSE;
	}
	m_RTDataArray.Add(pDataSet);
	m_strRqCodeArray.RemoveAt(0);

	return TRUE;
}

BOOL CBaseNetworkModule::RqItem(BOOL bStart)
{
	if(bStart) // Kospi, Kp200, ���� 4����, �ֽı����ڻ� ���� �ü� ��û
	{
		int i;
		
		CString strKP    = KOSPICODE;//kospi
		m_strRqCodeArray.Add(strKP);

		CString strKP200 = KP200CODE; //kp200
		m_strRqCodeArray.Add(strKP200);

		for(i = 0 ; i < min(4, m_FutureMonthInfoArray.GetSize()); i++) //���� 4����
		{
			CItemDataInfo * pInfo = m_FutureMonthInfoArray.GetAt(i);
			m_strRqCodeArray.Add(pInfo->m_strCode);
		}
		for(i = 0 ; i < m_UnderlyingAssetsInfoArray.GetSize(); i++) //�ֽ� �����.
		{
			CUnderlyingAssetsInfo * pUnder = m_UnderlyingAssetsInfoArray.GetAt(i);
			if(pUnder->m_strUnderlyingAssetsCode != KP200CODE)
				m_strRqCodeArray.Add(pUnder->m_strUnderlyingAssetsCode);
		}

		return RequestData();		
	}
	else
	{	
		if(m_strRqCodeArray.GetSize() > 0)
		{
			return RequestData();
		}
		else			
			return  FALSE;
	}
}

void CBaseNetworkModule::AddRTData()
{
	if(!m_pNetwork) return;
	m_pNetwork->AddRTData(m_RTDataArray);	
}

void CBaseNetworkModule::SetWnd(CWnd * pWnd)
{
	if(m_bReceiveBaseData)
	{	
		pWnd->PostMessage(WM_RECEIVE_BASEDATA, 0, (LPARAM) &m_BaseDataSet);
		pWnd->PostMessage(WM_DISPLAY_TODAY, 0, (LPARAM)&m_BaseDataSet);
	}
	else
		m_WndArray.Add(pWnd);
}

BOOL CBaseNetworkModule::ReadFutCode()
{
	if (m_FutureMonthInfoArray.GetSize() > 0)
		return true;

	char buf[1024];
	int	res;
	DWORD	dw;
	int	bufsize;
	struct	fjcode	*pFjCode;
	
	CString path;
	path.Format("%s\\%s\\%s", m_strHomeDir, "tab", "fjcode.dat");

	HANDLE fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		::MessageBox(NULL, "���� ȭ���� ���� �� �����ϴ�!", "", MB_OK|MB_ICONERROR);
		CloseHandle(fh);
		return false;
	}
	
	bufsize = sizeof(struct fjcode);
	
	for (int ii = 0; ; ii++)
	{
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;

		pFjCode = (struct fjcode *)buf;

		CItemDataInfo * pFutureInfo = new CItemDataInfo;
		pFutureInfo->m_strCode = CString(pFjCode->cod2, FCodeLen);
		pFutureInfo->m_strCode.TrimRight();
		pFutureInfo->m_strName = CString(pFjCode->hnam, FNameLen);
		pFutureInfo->m_strName.TrimRight();

		m_FutureMonthInfoArray.Add(pFutureInfo);
	}

	CloseHandle(fh);

	return TRUE;
}


BOOL CBaseNetworkModule::ReadETFCode()
{
	CStdioFile file;
	CString strFile1= m_strHomeDir + "\\table\\etfstcdk.dat";
	if(!file.Open(strFile1, CFile::modeRead))
		return FALSE;

	int nPos;
	CString strLine;
	CString strMonthInfo;
	CString strCode, strName, strStrike;

	while(file.ReadString(strLine))
	{
		if((nPos = strLine.Find("=")) == -1) continue;
		strCode = strLine.Left(nPos - 1);
		strLine = strLine.Mid(nPos + 1);
		
		if((nPos = strLine.Find("=")) == -1) continue;
		strName = strLine.Left(nPos - 1);
		
		strName.TrimLeft(); strName.TrimRight();
		strCode.TrimLeft(); strCode.TrimRight();

		CItemDataInfo * pItem= new CItemDataInfo;
		pItem->m_strCode = strCode;
		pItem->m_strName = strName;

		m_ETFItemInfoArray.Add(pItem);
	}
	file.Close();

	return TRUE;
}


BOOL CBaseNetworkModule::ReadSOptCode()
{
	CFile	file;
	struct  pjcode  PJCode;
	CString path;
	path.Format("%s\\%s\\%s", m_strHomeDir, "tab", "pjcode.dat");

	CUnderlyingAssetsInfo * pUnderlyingAssetsInfo = NULL;

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{	
		AfxMessageBox("�ֽĿɼ� ������ ���� �� �����ϴ�.");
		return false;
	}

// struct pjcode {
// 	char	gubn;		// 1:KOSPI, 2:KOSDAQ
// 	char	tjgb[2];	// �ŷ�������񱸺�(�����ڻ��ڵ�)
// 	char	snam[20];	// �ŷ���������
// 
// 	char	codx[8];	// �����ڵ�
// 	char	hnam[30];	// �ѱ۸�
// 	char	enam[30];	// ������
// 	char	gcod[12];	// �����ڻ������ڵ�
// 	char	mont[4];	// ����
// 	char	atmx;		// ATM���񿩺� 1:ATM����
// 	char	hsga[8];	// ��簡
// 	char	mchk;		// �Ÿ��������	// 0x01:���尡���	// 0x02:���Ǻ����������// 0x04:���������������
// };

	CString strUnderlyingCode, strCurAsset;
	CUnderlyingAssetsInfo * pCurUnderlyingInfo;

	CString strName, strCode, strStrike, strMonthInfo;
	int nCnt = file.GetLength() / sizeof(struct pjcode);
	for(int ii = 0; ii < nCnt; ii++)
	{
		file.Read(&PJCode, sizeof(struct pjcode));

		strUnderlyingCode = CString(PJCode.tjgb, sizeof(PJCode.tjgb));// �����ڻ�Short�ڵ�

		if(strCurAsset.Compare(strUnderlyingCode) != 0) // �����ڻ��� ����� ��츸
		{
			strName = CString(PJCode.snam, sizeof(PJCode.snam)); // �����ڻ��̸�(�ֽĸ�)
			strCode = CString(PJCode.gcod, sizeof(PJCode.gcod)); // �����ڻ��ڵ�
			strName.TrimLeft(); strName.TrimRight();
			strCode.TrimLeft(); strCode.TrimRight();
			strCode = strCode.Mid(3, 6);

			CUnderlyingAssetsInfo * pUnderlyingInfo = new CUnderlyingAssetsInfo;
			pUnderlyingInfo->m_strShortCode = strUnderlyingCode;
			pUnderlyingInfo->m_strUnderlyingAssetsCode = strCode;
			pUnderlyingInfo->m_strUnderlyingAssetsName = strName;

			m_UnderlyingAssetsInfoArray.Add(pUnderlyingInfo);

			strCurAsset = strUnderlyingCode; // ���� �����ڻ��ڵ� ����
			pCurUnderlyingInfo = pUnderlyingInfo; // ��������ڻ��� ������ ����
		}

		strCode = CString(PJCode.codx, sizeof(PJCode.codx)); //�����ڵ�
		strName = CString(PJCode.hnam, sizeof(PJCode.hnam)); //�����
		strName.TrimLeft(); strName.TrimRight();
		strStrike = CString(PJCode.hsga, sizeof(PJCode.hsga)); //��簡
		strStrike.TrimLeft();strStrike.TrimRight();
// 			strTradeUnit = strLine.Right(3);
// 			strTradeUnit.TrimLeft(); strTradeUnit.TrimRight();
		strMonthInfo = CString(PJCode.mont, sizeof(PJCode.mont)); //����

		CItemDataInfo * pOptionInfo = new CItemDataInfo;
		pOptionInfo->m_strCode = strCode;
		pOptionInfo->m_strName = strName;
		pOptionInfo->m_strStrike=strStrike;
		
		if(pCurUnderlyingInfo->m_strShortCode == strUnderlyingCode)
		{
			//�ŷ�����...=> �ֽ����簡 ��ȸ�� �޾Ƽ� ������ ���̴�.
			//pCurUnderlyingInfo->m_strTradeUnit = strTradeUnit;

			for(int j = 0 ; j < pCurUnderlyingInfo->m_MonthInfoArray.GetSize(); j++)
			{
				COptionMonthInfo * pMonthInfo = pCurUnderlyingInfo->m_MonthInfoArray.GetAt(j);
				if(pMonthInfo->m_strYearMonth == strMonthInfo)
				{
					if(strCode.Left(1) == "2")
						pMonthInfo->m_CallOptionInfoArray.Add(pOptionInfo);
					else
						pMonthInfo->m_PutOptionInfoArray.Add(pOptionInfo);	
					break;
				}
			}

			// ��ã���� ��� ó��
			if(j == pCurUnderlyingInfo->m_MonthInfoArray.GetSize())
			{
				COptionMonthInfo * pMonthInfo = new COptionMonthInfo;
				pMonthInfo->m_strYearMonth = strMonthInfo;
				if(strCode.Left(1) == "2")
					pMonthInfo->m_CallOptionInfoArray.Add(pOptionInfo);
				else
					pMonthInfo->m_PutOptionInfoArray.Add(pOptionInfo);

				pCurUnderlyingInfo->m_MonthInfoArray.Add(pMonthInfo);
			}
		}
		else
			delete pOptionInfo;
	}

	return TRUE;
}

BOOL CBaseNetworkModule::ReadOptCode()
{
	int	codeN, len;
	float fPrice;
	CFile	file;
	struct  ojcodh  OJCodh;
	struct  ojcode  OJCode;
	CString path;
	path.Format("%s\\%s\\%s", m_strHomeDir, "tab", "opcode.dat");

	CUnderlyingAssetsInfo * pUnderlyingAssetsInfo = NULL;
	COptionMonthInfo * pOptionMonth[4];

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{	
		AfxMessageBox("����/�ɼ� ������ ���� �� �����ϴ�.");
		return false;
	}

	len = file.Read(&OJCodh, sizeof(struct ojcodh));
	codeN = (file.GetLength() - len) / sizeof(struct ojcode);

	for (int ii = 0; ii < 4; ii++)
	{
		pOptionMonth[ii] = new COptionMonthInfo;
		pOptionMonth[ii]->m_strYearMonth = CString(OJCodh.pjym[ii], sizeof(OJCodh.pjym[ii]));
	}

	CString	sPrice, strCode, strATM;
	BOOL bFull = FALSE;
	
	for (ii = 0; ii < codeN; ii++)
	{
		file.Read(&OJCode, sizeof(struct ojcode));
		sPrice = CString(OJCode.price, sizeof(OJCode.price));
		fPrice = float(atof(sPrice) / 100.0);
		sPrice.Format("%.1f", fPrice);
		strATM = OJCode.atmg;
		
		int nCnt = 0;

		for(int i = 0; i < 4; i++)
		{	
			strCode = CString(OJCode.call[3-i].cod2, OPCODE_LEN);
			if(strCode == "")
				continue;

			if(!bFull) 
			{
				nCnt++;
				pOptionMonth[i]->m_strShortYearMonth = strCode.Mid(3,2);
				if(nCnt == 4)
					bFull = TRUE;
			}

			CItemDataInfo * pOptionInfo = new CItemDataInfo;
			
			pOptionInfo->m_strCode = strCode;
			pOptionInfo->m_strName = CString(OJCode.call[3-i].hnam, ONameLen);
			pOptionInfo->m_strStrike = sPrice;
			pOptionMonth[i]->m_CallOptionInfoArray.Add(pOptionInfo);

			CItemDataInfo * pOptionInfo2 = new CItemDataInfo;
			pOptionInfo2->m_strCode = CString(OJCode.put[i].cod2, OPCODE_LEN);
			pOptionInfo2->m_strName = CString(OJCode.put[i].hnam, ONameLen);
			pOptionInfo2->m_strStrike = sPrice;
			pOptionMonth[i]->m_PutOptionInfoArray.Add(pOptionInfo2);
		}
	}
	
	if(!pUnderlyingAssetsInfo)
	{
		pUnderlyingAssetsInfo = new CUnderlyingAssetsInfo;
		pUnderlyingAssetsInfo->m_strShortCode = "01";
		pUnderlyingAssetsInfo->m_strUnderlyingAssetsCode = "101";//KP200CODE;
		pUnderlyingAssetsInfo->m_strUnderlyingAssetsName = "KP200";
		m_UnderlyingAssetsInfoArray.Add(pUnderlyingAssetsInfo);
	}			
	pUnderlyingAssetsInfo->m_MonthInfoArray.Add(pOptionMonth[0]);
	pUnderlyingAssetsInfo->m_MonthInfoArray.Add(pOptionMonth[1]);
	pUnderlyingAssetsInfo->m_MonthInfoArray.Add(pOptionMonth[2]);
	pUnderlyingAssetsInfo->m_MonthInfoArray.Add(pOptionMonth[3]);

	file.Close();
	
	return TRUE;
}

void CBaseNetworkModule::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if(m_pNetwork)
	{
		m_pNetwork->DelAllData();	
		delete m_pNetwork;
		m_pNetwork = NULL;
	}

	if(m_pCommonFrameWork)
	{
		if(IsWindow(m_pCommonFrameWork->GetSafeHwnd()))
			m_pCommonFrameWork->DestroyWindow();
		delete m_pCommonFrameWork;
		m_pCommonFrameWork = NULL;
	}

	if(m_pHistoricalWnd)
	{
		if(IsWindow(m_pHistoricalWnd->GetSafeHwnd()))
			m_pHistoricalWnd->DestroyWindow();
		delete m_pHistoricalWnd;
		m_pHistoricalWnd = NULL;
	}

	if(m_pPastDataOfUnderlyingAssets) delete m_pPastDataOfUnderlyingAssets;
	
	int i;
	for(i = 0 ; i < m_UnderlyingAssetsInfoArray.GetSize() ; i++)	delete m_UnderlyingAssetsInfoArray.GetAt(i);
	for(i = 0 ; i < m_FutureMonthInfoArray.GetSize(); i++)			delete m_FutureMonthInfoArray.GetAt(i);	
	for(i = 0 ; i < m_RTDataArray.GetSize(); i++)					delete m_RTDataArray.GetAt(i);	
	for(i = 0 ; i < m_ETFItemInfoArray.GetSize(); i++)				delete m_ETFItemInfoArray.GetAt(i);

	KillTimer(0);
}

void CBaseNetworkModule::AddFONetwork(CFONetworkModule * pFONetwork)
{
	m_FONetworkArray.Add(pFONetwork);
	if(m_bReceiveBaseData && pFONetwork && pFONetwork->m_pParentWnd)
	{
		pFONetwork->m_pParentWnd->PostMessage(WM_RECEIVE_BASEDATA, 0, (LPARAM) &m_BaseDataSet);
		pFONetwork->m_pParentWnd->PostMessage(WM_DISPLAY_TODAY, 0, (LPARAM) &m_BaseDataSet);
	}
}

void CBaseNetworkModule::DelFONetwork(CFONetworkModule * pFONetwork)
{
	int i, nSize = m_FONetworkArray.GetSize();

	for(i = 0 ; i < nSize ; i++)
	{
		CFONetworkModule * pNetwork = m_FONetworkArray.GetAt(i);
		if(pNetwork == pFONetwork)
		{
			m_FONetworkArray.RemoveAt(i);
			return;
		}
	}
}

CString CBaseNetworkModule::GetUnderlyingAssetsPrice(CString strUnderlyingAssetsCode)
{
//20080316 �̹��� >>	
	CString strUnderlyingPrice, strUnderlyingPrice1;
	int i, nSize = m_RTDataArray.GetSize();
	
	if(strUnderlyingAssetsCode == KP200CODE)
	{
		int nFindCount = 0;
		CString strFutureCode = GetRecentFuture();
		for(i = 0 ; i < nSize ; i++)
		{
			CDataSet * pDataSet = m_RTDataArray.GetAt(i);
			if(pDataSet->m_strCode == strUnderlyingAssetsCode)
			{//kp200
				nFindCount++;
				strUnderlyingPrice = pDataSet->m_strCurPrice;
			}
			else if(pDataSet->m_strCode == strFutureCode)
			{//�ֱٿ�����
				nFindCount++;
				strUnderlyingPrice1 = pDataSet->m_strCurPrice;
			}
			if(nFindCount >= 2) break;
		}
		
		//�ֱٿ��ɼ��� ���������� ������ �������
		CCodeDataSet * pCodeInfo = NULL;
		if(m_BaseDataSet.m_CodeDataArray.GetSize() >= 5)
		{
			pCodeInfo = m_BaseDataSet.m_CodeDataArray.GetAt(4);
		}
		if(!pCodeInfo)
			return "";

		CCommonFrameWorkWnd * pCommon = (CCommonFrameWorkWnd *)GetCommonFrameWork();
		switch(pCommon->m_pTheorySet->m_nBasicPrice)
		{
		case 0: //kp200 - ��������
			{
				strUnderlyingPrice.Format("%.2f", atof(strUnderlyingPrice) / 100 - atof(pCodeInfo->m_strIndex));				
			}
			break;
		case 1: //�����ֱٿ�
			{
				strUnderlyingPrice = ::ConvertIntToPoint(strUnderlyingPrice1, 2);
			}
			break;
		case 2: //�ڵ����
			{
				strUnderlyingPrice.Format( "%.2f", atof(strUnderlyingPrice1) / 100 * 
					exp(-1 * (atof(m_BaseDataSet.m_strCDRate) / 100) * (atof(pCodeInfo->m_strRemainDay) / 365)) );				
			}
			break;
		default: //����
			{	strUnderlyingPrice.Format("%.2f",( atof(strUnderlyingPrice) * (pCommon->m_pTheorySet->m_nKOSPI200 / 100.) + 
					atof(strUnderlyingPrice1) * (pCommon->m_pTheorySet->m_nFuture / 100.) ) / 100.);				
			}
			break;
		}
	}
	else
	{
		for(i = 0 ; i < nSize ; i++)
		{
			CDataSet * pDataSet = m_RTDataArray.GetAt(i);
			if(pDataSet->m_strCode == strUnderlyingAssetsCode)
			{
				CCommonFrameWorkWnd* pWnd = (CCommonFrameWorkWnd*)GetCommonFrameWork();
				if(pWnd && pWnd->m_pEtcSet->m_bApplyPredictPrice && !pDataSet->m_strPredictPrice.IsEmpty())
					strUnderlyingPrice.Format("%d", atoi(pDataSet->m_strPredictPrice));
				else if(pDataSet->m_nRTDataType != RT_EXP_DATA_TYPE)
					strUnderlyingPrice.Format("%d", atoi(pDataSet->m_strCurPrice));
				break;
			}
		}
	}
	return strUnderlyingPrice;	
//20080316 �̹��� <<
}

CString CBaseNetworkModule::GetUnderlyingAssetsPriceForMargin(CString strUnderlyingAssetsCode)
{
	CString strUnderlyingPrice, strUnderlyingPrice1;
	int i, nSize = m_RTDataArray.GetSize();
	
	for(i = 0 ; i < nSize ; i++)
	{
		CDataSet * pDataSet = m_RTDataArray.GetAt(i);
		if(pDataSet->m_strCode == strUnderlyingAssetsCode)
		{
			if(strUnderlyingAssetsCode == KP200CODE) 			
				strUnderlyingPrice = ::ConvertIntToPoint(pDataSet->m_strCurPrice2, 2);
			else 
				strUnderlyingPrice.Format("%d", atoi(pDataSet->m_strCurPrice));
			break;
		}
	}	
	return strUnderlyingPrice;	
}

CString CBaseNetworkModule::GetUnderlyingAssetsShortCode(CString strUnderlyingAssetsCode)
{
	int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CUnderlyingAssetsInfo * pUnderlyingInfo = m_UnderlyingAssetsInfoArray.GetAt(i);
		if(pUnderlyingInfo->m_strUnderlyingAssetsCode == strUnderlyingAssetsCode)
			return pUnderlyingInfo->m_strShortCode;
	}
	return "";
}

//�Էµ� ������ �����ڻ� ������ ������ �ش�.
CUnderlyingAssetsInfo * CBaseNetworkModule::GetUnderlyingAssetsInfo(CString strCode)
{
	int nLength = strCode.GetLength();
	if(nLength < 3) return NULL;
	int nItemType = GetItemType(strCode);
	
	if(nItemType == EXP_ETF_TYPE || nItemType == EXP_STOCK_TYPE)
	{//�ֽ�
		int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CUnderlyingAssetsInfo * pUnderlyingInfo = m_UnderlyingAssetsInfoArray.GetAt(i);
			if(pUnderlyingInfo->m_strUnderlyingAssetsCode == strCode)
				return pUnderlyingInfo;
		}
	}
	else
	{
		int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CUnderlyingAssetsInfo * pUnderlyingInfo = m_UnderlyingAssetsInfoArray.GetAt(i);
			if(pUnderlyingInfo->m_strShortCode == strCode.Mid(1,2))
				return pUnderlyingInfo;
		}
	}
	return NULL;
}

CString CBaseNetworkModule::GetUnderlyingAssetsLastPrice(CString strUnderlyingAssetsCode)
{
	CString strUnderlyingLastPrice;

	int i, nSize = m_RTDataArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CDataSet * pDataSet = m_RTDataArray.GetAt(i);
		if(pDataSet->m_strCode == strUnderlyingAssetsCode)
		{
			if(pDataSet->m_strCode == KP200CODE)
			{
				strUnderlyingLastPrice = pDataSet->m_strPrevPrice2;
				strUnderlyingLastPrice = ::ConvertIntToPoint(strUnderlyingLastPrice, 2);
			}
			else//�ֽ��� ���.
				strUnderlyingLastPrice.Format("%d", atoi(pDataSet->m_strLastPrice));

			break;
		}
	}
	return strUnderlyingLastPrice;
}

CDataSet * CBaseNetworkModule::GetDataSetOfUnderlyingAssets(CString strUnderlyingAssetsCode)
{	
	int i, nSize = m_RTDataArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CDataSet * pDataSet = m_RTDataArray.GetAt(i);
		if(pDataSet->m_strCode == strUnderlyingAssetsCode)
		{
			return pDataSet;
		}
	}
	return NULL;;
}

int CBaseNetworkModule::GetTradeUnit(CString strCode)
{
	int nItemType = GetItemType(strCode);

	if(nItemType == EXP_ETF_TYPE || nItemType == EXP_STOCK_TYPE)
		return 1;
	else
	{
		if(strCode.Left(1) == "1")
		{
			return 500000;
		}
		else if(strCode.Left(3) == "201" || strCode.Left(3) == "301")
		{
			return 100000;
		}
		else if(strCode.Left(1) == "2" || strCode.Left(1) == "3")
		{
			//20080327 �̹��� >>
			int i,nSize = m_BaseDataSet.m_VolateInfoArray.GetSize();
			for(i = 0 ; i < nSize ; i++)
			{
				CVolateInfo * pVolateInfo = 
					m_BaseDataSet.m_VolateInfoArray.GetAt(i);
				if(pVolateInfo->m_strUnderlyingAssetsShortCode == strCode.Mid(1,2))
				{
					return atoi(pVolateInfo->m_strTradeUnit);
				}
			}
			//20080327 �̹��� <<
			return 1;
		}
	}
	
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////
//���� , �ɼ�, �ֽ��� �̰����� ���� �� ����.
//�ֽĿɼ��� �����ڻ� ���� �ŷ������� �������� �� ��츸 ���...
//////////////////////////////////////////////////////////////////////////////////////////
int CBaseNetworkModule::GetTradeUnitByUnderlyingAssetsCode(CString strUnderlyingAssetsCode)
{
	int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CUnderlyingAssetsInfo * pUnderlyInfo = 
			m_UnderlyingAssetsInfoArray.GetAt(i);
		if(pUnderlyInfo->m_strUnderlyingAssetsCode == strUnderlyingAssetsCode)
		{
			return atoi(pUnderlyInfo->m_strTradeUnit);
		}
	}		
	return 1;
}

CString CBaseNetworkModule::GetATM(CString strUnderlyingAssetsCode, CString strYM)
{
	CString strATM = "", strTemp;
	int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CUnderlyingAssetsInfo * pUnderlyingAssetsInfo = 
			m_UnderlyingAssetsInfoArray.GetAt(i);

		if(pUnderlyingAssetsInfo->m_strUnderlyingAssetsCode == strUnderlyingAssetsCode)
		{
			int j,nSize1 = m_RTDataArray.GetSize();
			for(j = 0 ; j  < nSize1; j++)
			{
				CDataSet * pDataSet = m_RTDataArray.GetAt(j);
				if(pDataSet->m_strCode == strUnderlyingAssetsCode)
				{
					double dUnderlyingAssetsPrice;
					double dTerm;
					//
					if(strUnderlyingAssetsCode == KP200CODE)
					{
						CString strCurPrice = ::ConvertIntToPoint(pDataSet->m_strCurPrice2, 2);
						dUnderlyingAssetsPrice = atof(strCurPrice);
					}
					else
						dUnderlyingAssetsPrice = atof(pDataSet->m_strCurPrice);

					int k, nSize2 = pUnderlyingAssetsInfo->m_MonthInfoArray.GetSize();
					for(k = 0 ; k < nSize2 ; k++)
					{
						COptionMonthInfo * pMonthInfo = pUnderlyingAssetsInfo->m_MonthInfoArray.GetAt(k);
						if(pMonthInfo->m_strYearMonth.GetLength() > 4)
							strTemp = pMonthInfo->m_strYearMonth.Mid(3);
						else
							strTemp = pMonthInfo->m_strYearMonth.Mid(1);		// �ֽĿɼ� �� ��
//						if(pMonthInfo->m_strYearMonth.Mid(3) == strYM)
						if(strTemp == strYM)
						{
							int l, nSize3 = pMonthInfo->m_CallOptionInfoArray.GetSize();
							for(l = 0 ; l < nSize3 ; l++)
							{
								CItemDataInfo * pInfo = pMonthInfo->m_CallOptionInfoArray.GetAt(l);
								double dDifference = fabs(atof(pInfo->m_strStrike) - dUnderlyingAssetsPrice);
								if(l == 0)
									dTerm = dDifference;
								else
								{
									if(dTerm > dDifference)
										dTerm = dDifference;
									else
									{
										if(l > 0) l--;
										CItemDataInfo * pData = pMonthInfo->m_CallOptionInfoArray.GetAt(l);
										return pData->m_strStrike;
									}
								}
							}
							if(l == nSize3)
							{
								CItemDataInfo * pData = 
									pMonthInfo->m_CallOptionInfoArray.GetAt(pMonthInfo->m_CallOptionInfoArray.GetSize() - 1);
								return pData->m_strStrike;
							}							
						}
					}
					return strATM;
				}
			}
			return strATM;
		}
	}
	return strATM;
}

void CBaseNetworkModule::SortSOptItem()
{
	int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CUnderlyingAssetsInfo * pUnderlyingInfo = 
			m_UnderlyingAssetsInfoArray.GetAt(i);

		int j, nSize1 = pUnderlyingInfo->m_MonthInfoArray.GetSize();
		for(j = 0 ; j < nSize1; j++)
		{
			COptionMonthInfo * pMonthInfo = pUnderlyingInfo->m_MonthInfoArray.GetAt(j);			
			Sort(pMonthInfo->m_CallOptionInfoArray);
			Sort(pMonthInfo->m_PutOptionInfoArray);
		}
	}
}

void CBaseNetworkModule::Sort(CItemInfoArray & ItemInfoArray)
{
	int i,j,nSize = ItemInfoArray.GetSize();
	for(i = nSize - 1 ; i >= 0 ; i--)
	{
		for(j = 0; j < i ; j++)
		{
			CItemDataInfo * pItem = ItemInfoArray.GetAt(j);
			CItemDataInfo * pNext = ItemInfoArray.GetAt(j+1);
			if(atof(pItem->m_strStrike) > atof(pNext->m_strStrike))
			{
				CItemDataInfo Temp;
				Temp		= *pItem;
				*pItem		= *pNext;
				*pNext		= Temp;
			}
		}
	}
}

//�������� ���.
CString CBaseNetworkModule::GetIndex(CString strCode)
{
	int nType = GetItemType(strCode);
	if(nType != EXP_FUTURE_TYPE && nType != EXP_KP200_CALL_OPT_TYPE && nType != EXP_KP200_PUT_OPT_TYPE &&
		nType != EXP_STOCK_CALL_OPT_TYPE && nType != EXP_STOCK_PUT_OPT_TYPE)
		return "";

	int i,nSize = m_BaseDataSet.m_CodeDataArray.GetSize();
	for(i = 0 ; i < nSize; i++)
	{	
		CCodeDataSet * pCode = m_BaseDataSet.m_CodeDataArray.GetAt(i);
	
		if(nType == EXP_FUTURE_TYPE)
		{
			if(pCode->m_strCodeInfo == strCode.Left(5))			
				return pCode->m_strIndex;			
		}
		else
		{
			if(pCode->m_strCodeInfo.GetLength() == 5 &&					// 5�ڸ�
					pCode->m_strCodeInfo.Left(1) != "1" &&				// ������ �ƴϰ�
					pCode->m_strCodeInfo.Mid(1,4) == strCode.Mid(1,4)	// 201C6, 301C6 -> 01C6 �����
				)
				return pCode->m_strIndex;
		}
	}

	return "";
}

CString CBaseNetworkModule::GetRemainDay(CString strCode)
{
	int nType = GetItemType(strCode);
	if(nType != EXP_FUTURE_TYPE && nType != EXP_KP200_CALL_OPT_TYPE && nType != EXP_KP200_PUT_OPT_TYPE && 
		nType != EXP_STOCK_CALL_OPT_TYPE && nType != EXP_STOCK_PUT_OPT_TYPE)
		return "";

	CString strRemainDay;
	int i,nSize = m_BaseDataSet.m_CodeDataArray.GetSize();
	for(i = 0 ; i < nSize; i++)
	{	
		CCodeDataSet * pCode = m_BaseDataSet.m_CodeDataArray.GetAt(i);
	
		if(nType == EXP_FUTURE_TYPE)
		{
			if(pCode->m_strCodeInfo == strCode.Left(5))			
			{
				//20080314 �̹��� >>
				// 0 : �ŷ��� , 1:������
				if(m_pCommonFrameWork->m_pTheorySet->m_nRemainDay == 0)
				{
					strRemainDay = pCode->m_strWorkRemainDay;
					break;
				}
				else
				{
					strRemainDay = pCode->m_strRemainDay;
					break;
				}
			}
		}
		else
		{
			if(pCode->m_strCodeInfo.Mid(1,4) == strCode.Mid(1,4))
			{
				if(m_pCommonFrameWork->m_pTheorySet->m_nRemainDay == 0)
				{
					strRemainDay = pCode->m_strWorkRemainDay;
					break;
				}
				else
				{
					strRemainDay = pCode->m_strRemainDay;
					break;
				}
			}
			//20080314 �̹��� <<
		}
	}

	if(m_pCommonFrameWork->m_pTheorySet->m_bTimeValue)
	{
		double dTotalRemainDay = atof(strRemainDay) - m_dTimeLoss;
		strRemainDay.Format("%.2f", dTotalRemainDay);
	}
	return strRemainDay;	
}

//20080325 �̹��� >>
CString CBaseNetworkModule::GetRemainDayForMargin(CString strCode)
{
	int nType = GetItemType(strCode);
	if(nType != EXP_FUTURE_TYPE && nType != EXP_KP200_CALL_OPT_TYPE && nType != EXP_KP200_PUT_OPT_TYPE && 
		nType != EXP_STOCK_CALL_OPT_TYPE && nType != EXP_STOCK_PUT_OPT_TYPE)
		return "";

	CString strRemainDay;
	int i,nSize = m_BaseDataSet.m_CodeDataArray.GetSize();
	for(i = 0 ; i < nSize; i++)
	{	
		CCodeDataSet * pCode = m_BaseDataSet.m_CodeDataArray.GetAt(i);
		
		if(pCode->m_strCodeInfo == strCode.Left(5))			
		{
			//�޷��ϼ�
			strRemainDay = pCode->m_strRemainDay;
			return strRemainDay;
		}
	}
	return strRemainDay;	
}
//20080325 �̹��� <<

//20080314 �̹��� >>
double  CBaseNetworkModule::CalcTimeLoss()
{
	if(m_pCommonFrameWork->m_pTheorySet->m_bTimeValue)
	{
		CTime t1, t2, tCurTime;
		CTimeSpan timeSpan1, timeSpan2;
		CString strFromTime , strToTime;
		if(m_pCommonFrameWork->m_pTheorySet->m_strFromTime.GetLength() < 4)
			t1 = CTime(2000, 1, 1, 9, 0, 0);
		else
			t1 = CTime(2000, 1, 1, 
					atoi(m_pCommonFrameWork->m_pTheorySet->m_strFromTime.Left(2)),
					atoi(m_pCommonFrameWork->m_pTheorySet->m_strFromTime.Mid(2,2)), 0);


		if(m_pCommonFrameWork->m_pTheorySet->m_strToTime.GetLength() < 4)
			t2 = CTime(2000, 1, 1, 15, 15, 0);
		else
			t2 = CTime(2000, 1, 1, 
				atoi(m_pCommonFrameWork->m_pTheorySet->m_strToTime.Left(2)),
				atoi(m_pCommonFrameWork->m_pTheorySet->m_strToTime.Mid(2,2)), 0);

		tCurTime = CTime(2000, 1, 1, 0, 0, 0);

		//20080324 �̹��� >>
		if(m_strKP200Time == "�帶��")
			tCurTime = t2;
		else
			tCurTime = CTime(2000, 1, 1, atoi(m_strKP200Time.Left(2)), atoi(m_strKP200Time.Mid(2,2)), 0);
		//20080324 �̹��� <<

		if(t1 >= t2 || t1 >= tCurTime)
			return 0.;	
		
		timeSpan1 = t2			- t1;
		timeSpan2 = tCurTime	- t1;

		m_dTimeLoss = (timeSpan2.GetTotalSeconds() / (double)timeSpan1.GetTotalSeconds());		
	}
	else
		m_dTimeLoss = 0;

	return m_dTimeLoss;
}
//20080314 �̹��� <<

CString CBaseNetworkModule::GetRecentFuture()
{
	if(m_FutureMonthInfoArray.GetSize() > 0)
	{
		CItemDataInfo * pItem = m_FutureMonthInfoArray.GetAt(0);
		return pItem->m_strCode;
	}
	return "";
}

void CBaseNetworkModule::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 0 && m_bEnableNetwork && m_FONetworkArray.GetSize() > 0)
	{
		RequestAverageIV();
		KillTimer(0);
	}
	
#ifdef __TEST__
	else if(nIDEvent == TEST_TIMER)
	{
		for(int i = 0 ; i < 20000 ; i++)
		{		
			for(int j = 0 ; j < 25; j++)m_pNetwork->RequestAverageIV("99");
			Sleep(1000);
		}
	}
#endif

	CWnd::OnTimer(nIDEvent);
}

void CBaseNetworkModule::RequestAverageIV()
{
	m_bEnableNetwork = FALSE;
	m_pNetwork->RequestAverageIV("99");//��ü�����ڻ꿡 ���ؼ� ��û...
}

//�ɼ� �����ڵ带 �־��ش�.
CString CBaseNetworkModule::GetTotalAverageIV(CString strCode)
{
	int nItemType = GetItemType(strCode);

	if(nItemType == EXP_ETF_TYPE || nItemType == EXP_STOCK_TYPE) return "";

	int i;
	char cType = strCode.GetAt(0);
	for(i = 0 ; i < m_AverageIV.m_AverageIVArray.GetSize(); i++)
	{
		CAverageIVSet * pAveIV = m_AverageIV.m_AverageIVArray.GetAt(i);
		if(pAveIV->m_strUnderlyingAssetsCode == strCode.Mid(1,2))
		{		
			if(cType == '2')		return pAveIV->m_strCallAverageIV;
			else if(cType == '3')	return pAveIV->m_strPutAverageIV;
			return "";
		}
	}	
	return "";
}

CString CBaseNetworkModule::GetKOSPIPrice()
{
	for(int i = 0 ; i < m_RTDataArray.GetSize(); i++)
	{
		CDataSet * pDataSet = m_RTDataArray.GetAt(i);
		if(pDataSet->m_strCode == KOSPICODE)
		{
			return pDataSet->m_strCurPrice;
		}
	}
	return "";
}
// [4/10/2003] m_FONetworkArray�� ��ϵ� ��� ������ �ݰ�, �ڽŵ� ����
void CBaseNetworkModule::DestroyWndSelf()
{
	int nSize = m_FONetworkArray.GetSize();

	// ��ϵ� �����츦 ��� ����
	CWnd* pMainWnd = AfxGetMainWnd();
	for(int i = nSize - 1 ; i >= 0; i--)
	{
		CFONetworkModule* pFModule = m_FONetworkArray.GetAt(i);
		CFrameWnd *pWnd = pFModule->m_pParentWnd->GetParentFrame();

		HWND hA = pWnd->GetSafeHwnd();
		HWND hB = pMainWnd->GetSafeHwnd();

		if(pWnd && pMainWnd && hA != hB)
		{
			pWnd->PostMessage(WM_CLOSE);
		}
	}
}

// [4/8/2003] > by jeoyoho, �ڵ� ���� �˸� ��ȭ���� ����
// WM_DIALOGDESTORYED�޽��� ó��(from CCloseNoticeDlg)
long CBaseNetworkModule::OnDialogDestroyed(WPARAM wParam, LPARAM lParam)
{
	DestroyWndSelf();

	return 0L;
} 
// [4/10/2003] > by jeoyoho, ��� â�� ������ �ڵ� ����
// WM_CLOSE_BASENETWORKMODULE�޽��� ó��(from CCommonFrameWorkWnd)
long CBaseNetworkModule::OnCloseBaseNetworkModule(WPARAM wParam, LPARAM lParam)
{
	DestroyWndSelf();

	// �ڽ��� �����ϵ��� 
	PostMessage(WM_CLOSE);

	return 0L;
}

void CBaseNetworkModule::SetHistoricalWnd(CWnd * pHistoricalWnd)
{
	m_pHistoricalWnd = pHistoricalWnd;
}

double CBaseNetworkModule::GetOptIVByUnderlyingAssetsCode(CString strUnderlyingAssetsCode)
{
	CString strShortCode;
	int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CUnderlyingAssetsInfo * pUnderlyingAssetsInfo = m_UnderlyingAssetsInfoArray.GetAt(i);
		if(pUnderlyingAssetsInfo->m_strUnderlyingAssetsCode == strUnderlyingAssetsCode)
		{
			strShortCode = pUnderlyingAssetsInfo->m_strShortCode;
			break;
		}
	}
	if(strShortCode.IsEmpty()) return 0;
	
	for(i = 0 ; i < m_BaseDataSet.m_VolateInfoArray.GetSize() ; i++)
	{
		CVolateInfo * pOpt = m_BaseDataSet.m_VolateInfoArray.GetAt(i);
		if(strShortCode == pOpt->m_strUnderlyingAssetsShortCode)
			return atof(pOpt->m_strIV);
	}	
	return 0.;
}

CVolateInfo * CBaseNetworkModule::GetVolateInfoByUnderlyingAssetsCode(CString strUnderlyingAssetsCode)
{
	CString strShortCode;
	int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize; i++)
	{
		CUnderlyingAssetsInfo * pUnderlyingAssetsInfo = m_UnderlyingAssetsInfoArray.GetAt(i);
		if(pUnderlyingAssetsInfo->m_strUnderlyingAssetsCode == strUnderlyingAssetsCode)
		{
			strShortCode = pUnderlyingAssetsInfo->m_strShortCode;
			break;
		}
	}
	if(strShortCode.IsEmpty()) return 0;

	for(i = 0 ; i < m_BaseDataSet.m_VolateInfoArray.GetSize(); i++)
	{
		CVolateInfo * pVol = m_BaseDataSet.m_VolateInfoArray.GetAt(i);
		if(strShortCode == pVol->m_strUnderlyingAssetsShortCode)
			return pVol;			
	}
	return NULL;
}

long CBaseNetworkModule::OnShowCloseDlg(WPARAM wParam, LPARAM lParam)
{
	if(m_pClosingDlg && IsWindow(m_pClosingDlg->GetSafeHwnd()))
	{
		m_pClosingDlg->ShowWindow(SW_SHOW);
	}
	return 0L;
}

//�����ڵ带 �Է��ؼ� ������ ���� ����ü�� ���´�.
CVolateInfo * CBaseNetworkModule::GetVolateInfoByItemCode(CString strCode)
{
	int nLength = strCode.GetLength();
	if(nLength < 3) return NULL;

	CUnderlyingAssetsInfo * pUnderlyingInfo = GetUnderlyingAssetsInfo(strCode);
	if(!pUnderlyingInfo) return NULL;

	CString strShortCode = pUnderlyingInfo->m_strShortCode;

	int i,nSize = m_BaseDataSet.m_VolateInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CVolateInfo * pVolate = m_BaseDataSet.m_VolateInfoArray.GetAt(i);
		if(pVolate->m_strUnderlyingAssetsShortCode == strShortCode)
		{
			return pVolate;
		}
	}	
	return NULL;
}


void CBaseNetworkModule::GetOptionYearMonthArray(CString strAsset, CStringArray *pstrYearMonthArray)
{
	int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++) 
	{		
		CUnderlyingAssetsInfo * pUnderlying = m_UnderlyingAssetsInfoArray.GetAt(i);
		if(pUnderlying) 
		{
			int nMonthCnt = pUnderlying->m_MonthInfoArray.GetSize();
			if(strAsset == pUnderlying->m_strUnderlyingAssetsCode && nMonthCnt > 0) 
			{
				for(int j=0; j<nMonthCnt; j++) {
					COptionMonthInfo* pOptMonthInfo = pUnderlying->m_MonthInfoArray.GetAt(j);
					if(pOptMonthInfo && pOptMonthInfo->m_CallOptionInfoArray.GetSize() > 0) 
					{
						CItemDataInfo* pItemInfo1 = pOptMonthInfo->m_CallOptionInfoArray.GetAt(0);
						if(pItemInfo1->m_strCode.GetLength() >= 9) 
						{
							CString strMonth = "0" + pItemInfo1->m_strCode.Mid(3,3);
							pstrYearMonthArray->Add(strMonth);
						}
					}
				}
			}
		}
	}
}

LONG CBaseNetworkModule::OnUser(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	BYTE bt = LOBYTE(LOWORD(wParam));
	switch(bt)
	{
	case DLL_OUB:					// Tr����
		ReceiveDataPIDO(wParam, lParam);
		break;

	case DLL_ALERT:					// ���� ����
		{
			int i,j;
			char* pData = (char*)lParam;

			if(!m_pNetwork)	return 0L;
			CDataArray	ReceiveArray;
			
			// �Ľ��ؼ� ReceiveArray �� ä���� ����
			m_pNetwork->ReceiveRTData(wParam, lParam, m_RTDataArray, ReceiveArray);

			if(ReceiveArray.GetSize() > 0)
			{
				for(j= 0 ; j < ReceiveArray.GetSize(); j++)
				{
					CDataSet * pDataSet = ReceiveArray.GetAt(j);

					//20080314 �̹��� >>
					//�ð���ġ �ݿ�
					if(pDataSet->m_strCode == KP200CODE)
						m_strKP200Time = pDataSet->m_strTime;
					//20080314 �̹��� <<

					//����ü�ᰡ�� ���� ���� ���� �ʴ� ������ ���õǾ� �ִ� ���.
					if(pDataSet->m_nRTDataType == RT_EXP_DATA_TYPE && !m_pCommonFrameWork->m_pEtcSet->m_bApplyPredictPrice)
						continue;

					int nSize = m_FONetworkArray.GetSize();
					for(i = 0 ; i < nSize ; i++)
					{
						CFONetworkModule * pNetwork = m_FONetworkArray.GetAt(i);
						if(pNetwork->m_pParentWnd && IsWindow(pNetwork->m_pParentWnd->GetSafeHwnd()))
							pNetwork->m_pParentWnd->SendMessage(WM_RECEIVE_RT_DATA, (WPARAM)BASEDATA_TYPE, (LPARAM)pDataSet);
					}	
				}
			}

			break;
		}
	case DLL_SETPAL:
		break;
	case DLL_TRIGGER:		// ����(ȭ�� ������ ��)
		break;
	case DLL_DOMINO:		// ȭ�� �����鼭 ������ ���� ���� ��
		break;
	case DLL_GUIDE:
		return 1;
	case DLL_SETFONTx:
		break;
	}

	return ret;
}

LONG CBaseNetworkModule::ReceiveDataPIDO(WPARAM wParam, LPARAM lParam)
{
	LONG ret = 0;
	int nKind = (HIBYTE(LOWORD(wParam)));

	if(!m_pNetwork) return FALSE;

	switch(nKind)
	{
		case TR_BASEDATA:			// ���ʵ�����
			m_pNetwork->ReceiveBaseData(wParam, lParam);

			// ��ȸ ������ ����� ���� �迭�� ù ������� ��ȸ
			RqItem(TRUE); // m_strRqCodeArray �迭�� ������ ���ʷ� ��û�Ѵ�.
			break;

		case TR_AVERAGEIV:
			{
				CAverageIV AveIV;

				if(m_pNetwork->ReceiveData(wParam, lParam, AveIV))
				{
					m_AverageIV = AveIV;				
					m_bEnableNetwork = TRUE;
					
					if(atoi(m_AverageIV.m_strType) != 2) 
						SetTimer(0, TIMER, NULL);
				}
				else
				{
					m_bEnableNetwork = TRUE;
					SetTimer(0, TIMER, NULL);
				}
			}
			break;
		case TR_AVERAGETODAYIV:
			{
				m_pNetwork->ReceiveData(wParam, lParam, m_stTodayVI);
				m_bEnableNetwork = TRUE;

				if(m_hVolateSelDlg)
				{
					::SendMessage(m_hVolateSelDlg, WM_AVERAGETODAYIV, 0, 0);
					m_hVolateSelDlg = NULL;
				}
			}
			break;
		case TR_OPTION_MONTH:
			{
				COptionMonthInfo OptionMonthInfo;
			
				int nResult;
				if((nResult = m_pNetwork->ReceiveData(wParam, lParam, &OptionMonthInfo)) == SUCCESS)
				{
					int nSize = m_FONetworkArray.GetSize();
					for(int i = 0; i < nSize; ++i)
					{
						CFONetworkModule* pFoNetWorkModlue = (CFONetworkModule*)m_FONetworkArray.GetAt(i);
						if(pFoNetWorkModlue->IsCommUseBaseModule())
							pFoNetWorkModlue->m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)MONTH_OPT_INFO, (LPARAM)&OptionMonthInfo);
					}	
				}
			}
			break;
		case TR_SOPTION_MONTH:
			{
				COptionMonthInfo OptionMonthInfo;
				
				int nResult;
				if((nResult = m_pNetwork->ReceiveDataSOptionMonth(wParam, lParam, &OptionMonthInfo)) == SUCCESS)
				{
					int nSize = m_FONetworkArray.GetSize();
					for(int i = 0; i < nSize; ++i)
					{
						CFONetworkModule* pFoNetWorkModlue = (CFONetworkModule*)m_FONetworkArray.GetAt(i);
						if(pFoNetWorkModlue->IsCommUseBaseModule())
							pFoNetWorkModlue->m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)MONTH_OPT_INFO, (LPARAM)&OptionMonthInfo);
					}	
				}
			}
			break;
		case TR_DELTA_HEDGE_INFO:
			{
				TR_15204_O stDeltaHedge;

				int nResult;
				if((nResult = m_pNetwork->ReceiveData(wParam, lParam, &stDeltaHedge)) == SUCCESS)
				{
					int nLen = HIWORD(wParam);
					char* szDeltaHedgeInfo = new char[nLen + 1];
					memset(szDeltaHedgeInfo, NULL, nLen + 1);

					memcpy(szDeltaHedgeInfo, (char*)lParam, nLen);

					int nSize = m_FONetworkArray.GetSize();
					for(int i = 0; i < nSize; ++i)
					{
						CFONetworkModule* pFoNetWorkModlue = (CFONetworkModule*)m_FONetworkArray.GetAt(i);
						if(pFoNetWorkModlue->IsCommUseBaseModule())
							pFoNetWorkModlue->m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)DELTA_HEDGE_INFO, (LPARAM)szDeltaHedgeInfo);
					}

					delete szDeltaHedgeInfo;
				}
			}
			break;
		default:
			if(nKind == TR_KOSPI || nKind == TR_KP200 || (nKind >= 50 && nKind <= 255))
			{
				//20080316 �̹��� >>
				if(nKind >= 50 && nKind <= 255 && m_KeyToWndList.GetCount() > 0)
				{				
					BOOL bMyData = FALSE;
					POSITION pos, pos1;
					for(pos = m_KeyToWndList.GetHeadPosition(); (pos1 = pos) ; )
					{
						CKeyToWndInfo Info = m_KeyToWndList.GetNext(pos);
						if(Info.m_nKey == nKind)
						{
							for(int _i = 0; _i < Info.m_hWndArray.GetSize(); _i++ )
							{
								HWND hWnd = (HWND)Info.m_hWndArray.GetAt(_i);
								if(hWnd == GetSafeHwnd())
									bMyData = TRUE;
								else
									::SendMessage(hWnd, WM_USER, wParam, lParam);
							}
							m_KeyToWndList.RemoveAt(pos1);
							break;
						}
					}
					if(!bMyData) return FALSE;
				}
				//20080316 �̹��� <<

				if(m_strRqCodeArray.GetSize() <= 0) return FALSE;

				CString strCode = m_strRqCodeArray.GetAt(0);

				CDataSet * pDataSet = new CDataSet;
				pDataSet->m_strCode = strCode;
				
				// ���� ������ �Ľ�
				m_pNetwork->ReceiveData(wParam, lParam, pDataSet);

				// ���� ��û�� ���� �迭�� ����
				m_RTDataArray.Add(pDataSet);
				// ��ȸ�� ������ �迭���� ����
				m_strRqCodeArray.RemoveAt(0);

				// �ֽ��� ��� �ֹ������� ������ �ش�.(����ȣ �߰�:�ֽĿɼ����Ͽ� �����ڻ��� �ֽ��� �ֹ����������� ������.)
				int nItemType = GetItemType(strCode);	
				if(nItemType == EXP_STOCK_TYPE)
				{
					CUnderlyingAssetsInfo * pAssetInfo = GetUnderlyingAssetsInfo(strCode);
					pAssetInfo->m_strTradeUnit = pDataSet->m_strReserved;
				}

				//20080314 �̹��� >>
				//������ �ð���ġ �ݿ��� ���� 2�ʸ��� ������ kp200�� �ð������͸� ������ ���´�.
				else if(nItemType == EXP_KP200)
				{
					m_strKP200Time = pDataSet->m_strTime;
				}
				//20080314 �̹��� <<
	
				// �ʿ��� ������ �ü��� �� �޾����� 
				if(m_strRqCodeArray.GetSize() == 0)
				{
					m_bReceiveBaseData = TRUE;

					if(m_RTDataArray.GetSize() > 0)	AddRTData();

					m_strRqCodeArray.RemoveAll();
					int i , nSize = m_FONetworkArray.GetSize();
					for(i = 0 ; i < nSize ; i++)
					{
						CFONetworkModule * pFONetwork = m_FONetworkArray.GetAt(i);
						if(pFONetwork->m_pParentWnd && IsWindow(pFONetwork->m_pParentWnd->GetSafeHwnd()))
						{			
							pFONetwork->m_pParentWnd->PostMessage(WM_RECEIVE_BASEDATA, 0, (LPARAM)&m_BaseDataSet);
							pFONetwork->m_pParentWnd->PostMessage(WM_DISPLAY_TODAY, 0, (LPARAM)&m_BaseDataSet);
						}
					}

					//////////////////////////////////////////////////////////////////////////
					RequestAverageIV();
					//////////////////////////////////////////////////////////////////////////

					//20080324 �̹��� >>
					CalcTimeLoss();
					//20080324 �̹��� <<
				}
				else
					RqItem(); // ���� ���� ��û
			}
			break;
	}

	return ret;
}

CString CBaseNetworkModule::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	char* dta = (char*)m_pFrameWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue.Format("%s", dta);
	
	return retvalue;
}

void CBaseNetworkModule::RequestTodayAverageIV(HWND hVolSelWnd, CString strShortCode)
{
	m_hVolateSelDlg = hVolSelWnd;

	int nDayFlag;
	if(m_pCommonFrameWork->m_pTheorySet->m_nRemainDay == 0)
		nDayFlag = (m_pCommonFrameWork->m_pTheorySet->m_bTimeValue) ? 2 : 0;
	else
		nDayFlag = (m_pCommonFrameWork->m_pTheorySet->m_bTimeValue) ? 3 : 1;

	CString strInput;
	strInput.Format("%2.2s%c%3.3s%3.3s%4.4s%4.4s%4.4s",
					strShortCode,										// �����ڻ��ڵ�(2)
					nDayFlag,											// �����ϱ���(1)
					"000",												// �����ϼ�(3)		
					m_nBizDayCount,										// �ѿ����ϼ�(4)	
					m_pCommonFrameWork->m_pTheorySet->m_strFromTime,	// ���۽ð�(4)
					m_pCommonFrameWork->m_pTheorySet->m_strToTime,		// ����ð�(4)
					"    ");											// ����ð�(4)

	m_bEnableNetwork = FALSE;
	m_pNetwork->RequestTodayAverageIV(strInput);
}


BOOL CBaseNetworkModule::IsApplyPredictPrice()
{
	CCommonFrameWorkWnd * pCommon = (CCommonFrameWorkWnd *)GetCommonFrameWork();
	
	if(pCommon != NULL)
		return pCommon->m_pEtcSet->m_bApplyPredictPrice;
		
	return FALSE;
}