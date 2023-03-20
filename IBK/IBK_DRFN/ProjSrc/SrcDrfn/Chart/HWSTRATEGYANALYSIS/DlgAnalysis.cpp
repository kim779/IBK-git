// DlgAnalysis.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAnalysis.h"
#include "DlgInterface.h"
#include "DlgWait.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ID_TIME_RUNST 10223
#define CRBACKGROUND	RGB(177,199,234)

//#define WMU_GET_AUP_SIZE	WM_USER + 2243			// Aup�� �⺻ Size�� View�� ��û�Ѵ�.
const UINT RMSG_GETDLLSIZE = ::RegisterWindowMessage("RMSG_GETDLLSIZE");
const UINT RMSG_SETDLLSIZE = ::RegisterWindowMessage("RMSG_SETDLLSIZE");
const UINT RMSG_STDATAMANAGER_RESULTDATA = ::RegisterWindowMessage("RMSG_STDATAMANAGER_RESULTDATA");


/////////////////////////////////////////////////////////////////////////////
// CDlgAnalysis dialog


CDlgAnalysis::CDlgAnalysis(CWnd* pParent /*=NULL*/)
	: CEscDialog(CDlgAnalysis::IDD, pParent)
{
	EnableAutomation();
 
	//{{AFX_DATA_INIT(CDlgAnalysis)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pStrategyItem = NULL;
	m_pdlgInfo = NULL;
	m_hSocket = NULL;
	m_pChartInterface = NULL;
	m_pCtlList = NULL;

	m_pChartItem = NULL;
	m_nStartType = 0;	// 0:�����м�, 1:�������

	m_pDlgWait = NULL;
	m_hTabChartEx = NULL;
	m_hRsc = AfxGetResourceHandle();
	m_bReceiveData = FALSE;
	m_crBk = CRBACKGROUND;
}


void CDlgAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAnalysis)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAnalysis, CDialog)
	//{{AFX_MSG_MAP(CDlgAnalysis)
	ON_WM_DESTROY()	
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_GETDLLSIZE,   OnGetDllSize)
	ON_REGISTERED_MESSAGE(RMSG_STDATAMANAGER_RESULTDATA, OnSTDataManagerMessagge)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//	ON_MESSAGE( WMU_GET_AUP_SIZE, OnGetAupSize )

/////////////////////////////////////////////////////////////////////////////
// CDlgAnalysis message handlers
void	CDlgAnalysis::FormInit()
{
}

void CDlgAnalysis::FormLoad()
{
}

//�����м��� ���⼭ �Ѵ�. ������ InputLeft���� �߾��µ� ������..
void CDlgAnalysis::InitSTLoad() 
{

	if(m_stCode=="" || m_stSTName=="") 
	{
		AfxMessageBox("������̳� �������� ���õ��� �ʾҽ��ϴ�.");
		return;
	}
	//m_stSTName.Delete(m_stSTName.GetLength()-5,m_stSTName.GetLength());
	int nEndPos = -1;
	if((nEndPos = m_stSTName.Find(gSTGubunKey)) != -1)
		m_stSTName = m_stSTName.Left(nEndPos);
	if((nEndPos = m_stSTName.Find('.')) != -1)
		m_stSTName = m_stSTName.Left(nEndPos);

	if(m_pdlgInfo)
	{
		if(m_stCode.GetLength()>6)
			m_pdlgInfo->InitCodeGubun(FALSE); // �������Ƣ�E��C 
		else
			m_pdlgInfo->InitCodeGubun(TRUE); // AO��A��E��C 
	}
	/////////////////////////////////
	m_stArrayPath.RemoveAll();

	////////////////////////////////
//@��������
//#ifdef _DEBUG
//	CString strSystemPath = m_strRootPath +  "\\" + STSD_D;
//	CString strUserPath = m_strRootPath + "\\" + STUSER_D;
//#else
//	CString strSystemPath = m_strRootPath + "\\" + STSD;
//	CString strUserPath = m_strRootPath + "\\" + STUSER;
//#endif
//@��������
//@�����߰�
//@�����߰�
//>> [QC No]090806HMC094
//	CString strSystemPath = m_strRootPath + "\\" + PATH_DATA_STDATA_SD;
//	CString strUserPath = m_strRootPath + "\\" + PATH_USER_STDATA_SD;
	CString strSystemPath = m_strDataDir + "\\STData\\SD";
	CString strUserPath = m_strUserDir + "\\STData\\SD";
//<< [QC No]090806HMC094
//@�����߰�

	m_stArrayPath.Add(strSystemPath);//Data
	m_stArrayPath.Add(strUserPath);//User

	if(!m_pStrategyItem)
	{
		m_pStrategyItem = m_strategyLoader.GetStrategyItem();// �޸� ���� 
		if(m_pStrategyItem == NULL) return;
		int nRetValue = m_pStrategyItem->Create(this, (long)m_pStrategyItem,m_stArrayPath,m_pChartInterface);
		if(nRetValue==-1) 
		{
			m_pStrategyItem->DestoryWindow();
			delete m_pStrategyItem;
			m_pStrategyItem = NULL;
		}
	}

	//LoadForRun�����(String), �����Ͱ���(int),��¥(String)
	m_bReceiveData = FALSE;//�����͸� ���� ���� ������ ����
	BOOL bRetValue = m_pStrategyItem->LoadST(m_stSTPath,m_stCode,m_nCount,m_stEndDate); //
	if(bRetValue)
	{
		m_pStrategyItem->Run();
	}
		//�ʱ�ȭ �ϱ� 

	//m_pdlgInfo->InitWebView();
	//if(m_pdlgInfo && m_nStartType==0)
	//	m_pdlgInfo->ApplyST();

}

//�ϴ� ����ϴ� ��ƾ : �����͸� �������� ��ƾ������ KHD 2006.07.27
LRESULT CDlgAnalysis::OnSTDataManagerMessagge( WPARAM wParam, LPARAM lParam)
{
	if(m_bReceiveData) return 0;//�����͸� �޾����� ����..
	CString strDataItemName, szTData;
	CString strDiv, strRep;
	CStringArray strDate; //Date ������ ����
	ST_DATAINDEX Index;
	BOOL nIsDate = -1;
	int i = 0;
	m_stDataIndex.RemoveAll();

	if(wParam==(WPARAM)m_pStrategyItem||m_DivideState ==1||m_DivideState==2)
	{
		m_pStrategyItem = (CIStrategyItem*)wParam;	
		m_pStrategyItem->Stop();
		ST_DRAWINFO_EX* pInfo = (ST_DRAWINFO_EX*)lParam;
		CPtrArray* pPtrArrayPlot = pInfo->pPtrArraySDataItem;
		m_pDataItem = NULL;
		int nSize = pPtrArrayPlot->GetSize();

		if(nSize ==0) return 0;

		/////////////List ä��� ///////////////////////////////////////
		m_pDataItem = (CISDataItem *)pPtrArrayPlot->GetAt(0);

		long nTotalData	= m_pDataItem->GetSize();
		//Data ��û //////////////////////////
		/////////////////////
		strDate.SetSize( nTotalData );

		//��ü�Ⱓ ��Ƽ��Ʈ ����Ʈ�� �ֱ�(KHD:2006.08.08)
		m_pDataItem->GetAt(0,&strDataItemName);//�� ó�� ���� ó��
		InsertGubun(strDataItemName);
		Index.nStart = 0;
		Index.strStartDate = strDataItemName;
		Index.strGubun ="��ü";
		
		m_pCtlList->DeleteAllItems();
		m_pCtlList->InsertItem(0,"*");		// �������� ���̱� ���ؼ� ������.

		nIsDate = strDataItemName.Find('/');
		if(nIsDate<0 ||m_DivideState==1)
		{
			m_pCtlList->SetItemText(0,1,strDataItemName);
		}
		else 
		{
			szTData = strDataItemName.Mid(2);
			m_pCtlList->SetItemText(0,1,szTData);
		}

		m_pDataItem->GetAt(nTotalData-1,&strDataItemName);//�� ������ ���̳� 
		InsertGubun(strDataItemName);

		if(nIsDate<0||m_DivideState==1)
		{
			m_pCtlList->SetItemText(0,2,strDataItemName);
		}
		else
		{
			szTData = strDataItemName.Mid(2);
			m_pCtlList->SetItemText(0,2,szTData);
		}

		Index.nEnd = nTotalData-1;
		Index.strEndDate = strDataItemName;

		m_stDataIndex.Add(Index);//��ü����..�ֱ�
		//if(m_nDivideCnt==1)
		//{
		//	m_stDataIndex.Add(Index);
		//}
	//�迭�� ��¥�� ������ �ͼ� �����Ѵ�. ������ ������ �ϴ� ������� ���� 
		for( i = 0;i<nTotalData;i++)
		{
			m_pDataItem->GetAt(i,&strDataItemName);
			strDate.SetAt(i,strDataItemName);
		}

	//	//��ø�� ���ϱ� :  ���� �۾�
	//	//num - ((Div-1)*Repnum/Div)
		int Div = m_nDivideCnt; //���� ���� ���� ����.
		int num = strDate.GetSize();//��ü �����Ͱ���
		int Divnum =0;
		int Repnum;
		long double Repfloat;

		if(m_dfDivideRate > 0)
		{
			Repnum		= ((num/10) * (int)m_dfDivideRate/10);//atoi(strRep) *;//�� ������� ��ø��
			int tot		= num - ((Div-1)*Repnum/Div);//��ü�� ����
			Repfloat	= long double(num/long double(tot));//Repfloat * num-Repnum = 400
		}
		else
		{
			Repnum = 0;
			Repfloat =1;
		}

		//��ø�� �־������~
		for(i =0; i<Div; i++)//��������..
		{
			//�������� �Ͽ� ������ ���� : 0.5�� ���� -1�� ������ �Ǿ� �������� ��Ȯ������.
			int a = int(((Divnum-(i*(Repnum/Div)))*Repfloat)-0.5);
			if(a>0)
			{
				strDataItemName = strDate.GetAt(a);
				Index.nStart = a;
			}
			else//ù ������������ 0 ��ü���� ���� �����;� �ϹǷ�
			{
				strDataItemName = strDate.GetAt(Divnum );
				Index.nStart = int(Divnum);
			}
			
			InsertGubun(strDataItemName);
			Index.strStartDate = strDataItemName;
			//Index.strGubun.Format("[%d����]",i+1);
			Index.strGubun.Format("%d",i+1);		// �������� ���̱� ���ؼ� ������.
			//m_pCtlList->InsertItem(i+1,"");
			m_pCtlList->InsertItem(i+1,Index.strGubun);

			if(nIsDate<0||m_DivideState==1)
			{
				m_pCtlList->SetItemText(i+1,1,strDataItemName);
			}
			else
			{
				szTData = strDataItemName.Mid(2);
				m_pCtlList->SetItemText(i+1,1,szTData);
			}

			//�迭�� �������� 0�̹Ƿ� ù ȭ�鿡���� �״�� �������� 1���� ���� �ؾ��Ѵ�.
			// 0~99 , 100~199, 200~299 : ���������� ���ڴ� 99������ �迭���� �ּҴ� 100���� �ȴ�.
			if(i==0)
				Divnum+=(num/Div);
			else
				Divnum += ((num/Div)-1);//1������ �����. ������ ��ŭ ���Ѵ�.
			
			if(Divnum > nTotalData-1)//���� 400���� ������ ���� ���� ��� ���������� �����Ѵ�.
				Divnum = nTotalData-1;

	//		//��� ���� 
			int b = int(((Divnum - (i*(Repnum/Div)))*Repfloat)-0.5);//int((Divnum+(Repnum/Div))*Repfloat);

			if(b<num && i!=Div)// �����Ͱ����� ���� ���� ó�� : �ִ밹���� ���� �ʱ� ����..
			{
				strDataItemName = strDate.GetAt(b);
				Index.nEnd = b;
			}

			if(m_nDivideCnt==1 || i == Div-1)//������ ���� �����´�. 
			{
				strDataItemName = strDate.GetAt(num-1);
				Index.nEnd = num-1;

			}
			InsertGubun(strDataItemName);
			Index.strEndDate = strDataItemName;
			/////////������ �����Ѵ�. KHD : 2006/9/4/////////////
			m_pStrategyItem->CalculateStatisticalInfo(Index.nStart, Index.nEnd);
			// �·� = 18 , �������ͷ� =1 , �ѸŸż� = 15, �Ѽ��ͷ� = 3
			
			CString stData1;
			stData1.Format("%.2f",m_pStrategyItem->GetStatisticalInfo(0));
			stData1 = CDlgInterface::GetComma(stData1);

			CString stData3;
			stData3.Format("%.0f",m_pStrategyItem->GetStatisticalInfo(15));
			stData3 = CDlgInterface::GetComma(stData3);
			CString stData4;
			stData4.Format("%.2f",m_pStrategyItem->GetStatisticalInfo(19));
			stData4 = CDlgInterface::GetComma(stData4);
			Index.strInfo1.Format("����:%s ",stData1);
			Index.strInfo2.Format("�·�:%s �Ÿż�:%s",stData4,stData3);
			Index.STName = m_stSTName;
			/////////////////////////////////////////////////////
			m_stDataIndex.Add(Index);//�ߺ� ���⿡ ����� �ɷ� ��Ʈ���� ����.
			
			if(nIsDate<0||m_DivideState==1)
			{	
				m_pCtlList->SetItemText(i+1,2,strDataItemName);
			}
			else 
			{
				szTData = strDataItemName.Mid(2);
				m_pCtlList->SetItemText(i+1,2,szTData);
			}	
			Divnum++;
		}	
		if(m_pdlgInfo && m_nStartType!=1)
		{
			Result();
			//m_pdlgInfo->AllUpdateDlg(); // UpdateDlg
			//	////////////////////////////////////////////////////
			//m_pdlgInfo->m_CtlResult.EnableWindow(TRUE);
		}
	}


	m_pCtlList->Invalidate();
	if(m_pdlgInfo)
		m_pdlgInfo->DrawGraph();

	if(m_nStartType==1)
		PostMessage(WM_DESTROY, 0, 0);
	m_bReceiveData = TRUE;//�����͸� �޾Ҵ�..
	return 0;
}

//Log ��� ��ƾ !! KHD
void CDlgAnalysis::WriteDebug(CString szDebug)
{
	SYSTEMTIME sTime;
	GetSystemTime(&sTime);

	//	FILE *fp = fopen("M:\\Logs\\GridDebug.txt", "a+t");
	FILE *fp = fopen("c:\\Logs\\�����м�.txt", "a+t");

	if(fp != NULL) 
	{
		fprintf(fp, "[%02d:%02d:%02d]", sTime.wHour+9, sTime.wMinute, sTime.wSecond);
		fprintf(fp, "[%s]\n", szDebug);
		fclose(fp);
	}

}

void CDlgAnalysis::InsertMonthGubun(CString &str)
{
	if(str.GetLength()<4) return;
	str.Insert(4,'/');
}

void CDlgAnalysis::InsertDateGubun(CString &str)
{
	if(str.GetLength()<4) return;
	str.Insert(4,'/');
	str.Insert(7,'/');
}

//���� Tick�� ��� ������ ��ü�� �� �� ������ �ֽ��ϴ�. : KHD : 2006.09.29
void CDlgAnalysis::InsertTimeGubun(CString &str)
{
	int nSize = str.GetLength();
	if(nSize == 7)
	{
		str.Insert(0,'0');
		nSize++;
	}
	if(nSize<7) return;

	str.Insert(2,'/');
	str.Insert(5,':');
	str.Insert(8,':');
}

void CDlgAnalysis::InsertHourGubun(CString &str)
{
	int nSize = str.GetLength();
	if(nSize == 7)
	{
		str.Insert(0,'0');
		nSize++;
	}
	if(nSize<7) return;

	str.Insert(2,'/');
	str.Insert(5,'/');
	str.Insert(8,':');
}

//���� ����,  ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5)
BOOL CDlgAnalysis::InsertGubun(CString &stTime,BOOL IsHtml)
{
	switch(m_DivideState)
	{
	case 0://ƽ
		InsertTimeGubun(stTime);	
		break;
// 	case 4://��
// 		InsertHourGubun(stTime);
// 		break;
	case 1://��
		InsertMinuteGubun(stTime,IsHtml);
		break;
	case 2://��
	case 3://��
		InsertDateGubun(stTime);
		break;
	case 4://��
		InsertMonthGubun(stTime);
		break;
	}
	return TRUE;
}

void CDlgAnalysis::InsertMinuteGubun(CString &str, BOOL IsHtml)
{
	int nSize = str.GetLength();
	if(nSize == 7)
	{
		str.Insert(0,'0');
		nSize++;
	}
	if(nSize<7) return;
	if(IsHtml)//When HTML File
	{
		str.Insert(2,'/');
		str.Insert(5,"&nbsp;");
		str.Insert(nSize+5,":");
	}
	else
	{
		str.Insert(2,'/');
		str.Insert(5,'/');
		str.Insert(8,':');
	}
}
//KHD : ���� ��ư ������ �߻��ϴ� �̺�Ʈ!!
void CDlgAnalysis::SetCalculateData(int nIndexnum)
{
	ST_DATAINDEX Index;
	int nSize = m_stDataIndex.GetSize();
	if(nIndexnum>=nSize)
		return;

	Index = m_stDataIndex.GetAt(nIndexnum);
	//m_pdlgInfo->m_Trade->m_nDiv = nIndexnum;
	m_pStrategyItem->Stop();
	m_pStrategyItem->CalculateStatisticalInfo(Index.nStart, Index.nEnd);

}

void CDlgAnalysis::Result()
{ 
	if(m_pdlgInfo)
	{
		int CurSel = 0;//m_pdlgInfo->m_Trade->m_nDiv;
		SetCalculateData(CurSel);
		m_pdlgInfo->SetRefresh(CurSel);
	}
}
void CDlgAnalysis::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
	m_brBkColor.DeleteObject();
	
	if(m_pdlgInfo) 
	{
		m_pdlgInfo->DestroyWindow();
		delete m_pdlgInfo;
		m_pdlgInfo = NULL;
	}

	if(m_pStrategyItem)
	{
		m_pStrategyItem->DestoryWindow();
		delete m_pStrategyItem;
		m_pStrategyItem = NULL;
	}

	if(m_pDlgWait)
	{
		delete m_pDlgWait;
		m_pDlgWait = NULL;
	}
	
//	if(m_pCtlList)
//		m_pCtlList->DeleteAllItems();
}

BOOL CDlgAnalysis::OnInitDialog() 
{
	CDialog::OnInitDialog();

	AfxEnableControlContainer();

	// TODO: Add extra initialization here
	CRect rt(0,0,0,0);
	m_pdlgInfo = new CInfoRight;
	if(m_pdlgInfo)
	{
		m_pdlgInfo->Create(CInfoRight::IDD,this);
		m_pdlgInfo->ShowWindow(SW_SHOW);
		m_pdlgInfo->m_pDlgAnalysis = this;
		m_pdlgInfo->SetParentsRect(rt);
		m_pdlgInfo->m_strFilePath = m_strDataDir + "\\STDATA";
		if(m_nStartType==0)
			m_pdlgInfo->InitTab();
		m_pdlgInfo->m_pChartItem = m_pChartItem;
	}

	//�����м� ���� :KHD 2006.07.27
	if(m_nStartType==1)
	{
		InitSTLoad();
		return TRUE;
	}

	if(!m_pDlgWait)
	{
		m_pDlgWait = new CDlgWait;
		if(!m_pDlgWait->Create(CDlgWait::IDD, this))
		{
			delete m_pDlgWait;
			m_pDlgWait = NULL;
		}
		else
		{
			m_pDlgWait->CenterWindow(&m_pdlgInfo->m_CtlTab);
			m_pDlgWait->ShowWindow(SW_SHOW);
		}
	}

	m_crBk = DEFAULT_BKGRND_COLOR;
	m_brBkColor.CreateSolidBrush(m_crBk);

	SetTimer(ID_TIME_RUNST, 1, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



CRect CDlgAnalysis::GetRect(int nID)
{

	CRect rect;	
	switch(nID)
	{
	case IDD_DLG_ANALYSIS:
		{
			GetClientRect(rect);
		}
		break;
	}
	return rect;
}

// 060721_001 JLO �����м��� ���̾�α� ���·� �ٷ� ���� �׽�Ʈ�ϱ����� �߰�.
void CDlgAnalysis::OnCancel()
{
	CDialog::OnCancel();
}
// ~060721_001 JLO


//ȭ��ε��� ȭ��ũ�⸦ �˱����� ȣ��ǹǷ� ȭ��ũ�⸦ �Ѱ���.
//LONG CDlgAnalysis::OnGetAupSize(WPARAM wParam, LPARAM lParam)
// ��Ÿ�ӽÿ� DLL�� ����� �����ϰ� ���� ��쿡�� RMSG_SETDLLSIZE �޽����� ������. �θ�(���̾�α�������)����.
LONG CDlgAnalysis::OnGetDllSize(WPARAM wParam, LPARAM lParam)
{

	CPoint* pSize = (CPoint*)wParam;

	pSize->x = 764;
	pSize->y = 482;

	return 0L;
}

void CDlgAnalysis::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
}

void CDlgAnalysis::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == ID_TIME_RUNST)
	{
		KillTimer(ID_TIME_RUNST);

		InitSTLoad();
		if(m_pDlgWait) 
		{
			delete m_pDlgWait;
			m_pDlgWait = NULL;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

HBRUSH CDlgAnalysis::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
	default:
		pDC->SetBkColor(m_crBk);
		return m_brBkColor;		
	}
	
	return hbr;
}