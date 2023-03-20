// InputLeft.cpp : implementation file
//

#include "stdafx.h"
//#include "hwstrategyanalysis.h"
#include "InputLeft.h"
#include "../Common_ST/STControllerDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputLeft dialog
const UINT RMSG_STDATAMANAGER_RESULTDATA = ::RegisterWindowMessage("RMSG_STDATAMANAGER_RESULTDATA");
#define CRBACKGROUND	RGB(211,223,241)

#define SAVINGSTRATEGY		"SavingStrategy"


CInputLeft::CInputLeft(CWnd* pParent /*=NULL*/)
	: CDialog(CInputLeft::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputLeft)
	//}}AFX_DATA_INIT
	m_bgColor = CRBACKGROUND;

	m_bIsView = FALSE;
	m_pStrategyItem = NULL;
	m_hSocket = NULL;
	m_bInitShareBase = FALSE;

	m_pChartInterface = NULL;
}


void CInputLeft::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputLeft)
	DDX_Control(pDX, IDC_COMBO_DATANUM, m_CtlComNum);
	DDX_Control(pDX, IDC_DATETIME,		m_CtlDateTime);
	DDX_Control(pDX, IDC_COMBO_REP,		m_CtlComboRep);
	DDX_Control(pDX, IDC_COMBO_DIV,		m_CtlComboDiv);
	DDX_Control(pDX, IDC_BTN_APPLY,		m_ctlApply);
	DDX_Control(pDX, IDC_CB_STRATEGY,	m_CtlStategy);
	DDX_Control(pDX, IDC_LIST_DAY,		m_CtlList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputLeft, CDialog)
	//{{AFX_MSG_MAP(CInputLeft)
//	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_RADIO_STOCK, OnRadioStock)
	ON_BN_CLICKED(IDC_RADIO_FUTURE, OnRadioFuture)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_DIV, OnSelchangeComboDiv)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DAY, OnClickListDay)
	//}}AFX_MSG_MAP
		ON_REGISTERED_MESSAGE(RMSG_STDATAMANAGER_RESULTDATA, OnSTDataManagerMessagge)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputLeft message handlers

CInputLeft* CInputLeft::GetInputLeft()
{
	return this;
}
void CInputLeft::SetRightInfo(CInfoRight *pdlg)
{
	m_pInfoRight = pdlg;
}
void CInputLeft::InitInputLeft()
{
//	m_BaseInfo.InitInfo((void*)m_IDrMLib_CDBMgr.GetDBLong());
//	AdviseSharedInfo(SAVINGSTRATEGY,DE_SHARDSET|DE_SHARDGET,DE_AREA_ALL);
	InitColumn();
	InitStrategy();
	InitCodeInfo(0);
	((CButton*)GetDlgItem(IDC_RADIO_FUTURE))->SetCheck(TRUE);
}
void CInputLeft::InitCodeInfo(long lType)
{
//	m_BaseInfo.InitInfo((void*)m_IDrMLib_CDBMgr.GetDBLong());
	int nKind = NULL;
	CRect rc;
	rc.left = 53;//30;
	rc.top  = 30;//26;
	rc.right = rc.left + 135;
	rc.bottom = 328;//250;

	if(lType==0)
	{
//		m_BaseInfo.CreateControl_CodeInput(GetSafeHwnd(), rc, NID_CODEINPUT,CODE_JONGMOK|CODE_KJONGMOK, 6, 7);
		nKind = CODE_JONGMOK | CODE_KJONGMOK;
	}
	else
	{
//		m_BaseInfo.CreateControl_CodeInput(GetSafeHwnd(), rc, NID_CODEINPUT, CODE_FUTURE|CODE_OPTION, 8, 7);
		nKind =	CODE_FUTURE|CODE_OPTION;	
	}
	CString strValue;
	char	szBuf[20];	
	if( ::SendMessage(m_hMainFrame, WMU_GET_KEYCODE, (WPARAM)nKind, (LPARAM)szBuf) == FALSE)
		return;


	if( szBuf[0] == 0x00)
		return;

	// ���� �ڵ� ����
	strValue.Format("%s",szBuf);
	strValue.TrimLeft();	
	strValue.TrimRight();

//	m_BaseInfo.SetTextToCodeInput(strValue);
//	m_strCodeFromHanwha = strValue;
//	ChangeEditCode();
}

CString CInputLeft::GetJMNameAndGubunFromCode(CString csCode)
{
	return "FALSE";
/*
	csCode.TrimLeft();		csCode.TrimRight();
	
	CString csName;

	// {{ ETF �����̸� }}
//	if(m_IDrMLib_CDBMgr.IsETFCode(csCode))
	{
		m_IDrMLib_CDBMgr.GetCodeName(CODE_UPJONGSTOCK, csCode, csName);
	}
	// {{ �ŷ��� ���� �̸� }}
	else if(m_IDrMLib_CDBMgr.GetCodeName(CODE_JONGMOK, csCode, csName))
	{
		return csName;		
	}
	// {{ �ڽ��� �����̸� }}
	else if(m_IDrMLib_CDBMgr.GetCodeName(CODE_KJONGMOK, csCode , csName))
	{
		csName	= "��"+csName;
		return csName;
	}
	else if(m_IDrMLib_CDBMgr.GetCodeName(CODE_FUTURE, csCode, csName))
	{
		return csName;
	}
	else if(m_IDrMLib_CDBMgr.GetCodeName(CODE_OPTION, csCode, csName))
	{
		return csName;
	}
	else if(m_IDrMLib_CDBMgr.GetCodeName(CODE_QFUTURE, csCode, csName))
	{
		return csName;
	}
	else if(m_IDrMLib_CDBMgr.GetCodeName(CODE_QFUTURES, csCode, csName))
	{
		return csName;
	}
	else if(m_IDrMLib_CDBMgr.GetCodeName(CODE_FSPREAD, csCode, csName))
	{
		return csName;
	}
	else if(m_IDrMLib_CDBMgr.GetCodeName(CODE_QOPTION, csCode, csName))
	{
		return csName;
	}
	else if(m_IDrMLib_CDBMgr.GetCodeName(CODE_QOPTIONS, csCode, csName))
	{
		return csName;
	}
	else if(m_IDrMLib_CDBMgr.GetCodeName(CODE_JOPTION, csCode, csName))
	{
		return csName;
	}
*/


	return "";
}

void CInputLeft::InitColumn()
{
	int colwidthsType0[3] = { 36, 80, 80};	// sixty-fourths

	TCHAR *	lpszHeadersType0[] = { _T("����"), _T("����"), _T("��"),NULL };
	m_CtlList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	int total_cx = 0;
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	// add columns
	for (int i = 0; ; i++)
	{
		if (lpszHeadersType0[i] == NULL)
			break;
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		//lvcolumn.fmt = i>1 ? LVCFMT_RIGHT:LVCFMT_LEFT;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = lpszHeadersType0[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = colwidthsType0[i];
		total_cx += lvcolumn.cx;
		m_CtlList.InsertColumn(i, &lvcolumn);
	}
	
	m_CtlList.InsertItem(0,"��ü",RGB(0,0,0),RGB(80,185,200));
	m_CtlList.SetHilightColor(RGB(255,255,255),RGB(101,86,229));
}

//���� �� �ֽ� �� �ε��ϴ� ��Ÿ�� ���� �׽�Ʈ 
//void CInputLeft::OnButton1() 
//{
	// TODO: Add your control notification handler code here
//	if(m_IDrMLib_CDBMgr)
//		m_BaseInfo.~CDrMLib_CBaseInfo();
//	m_IDrMLib_CDBMgr = NULL;
//}

//���� �ʱ�ȭ 
void CInputLeft::InitStrategy()
{

//#ifdef _DEBUG
//	CString strUserPath = m_strUserPath + "\\STDllD\\*.ST";
//#else
//	CString strUserPath = m_strUserPath + "\\STDll\\*.ST";
//#endif



//
#ifdef _DEBUG
 CString strUserPath = m_strRootPath + STUSERFILE_D;
#else
 CString strUserPath = m_strRootPath + STUSERFILE;
#endif
//
	CFileFind finder;
	m_CtlStategy.ResetContent();
	BOOL bWorking = finder.FindFile(strUserPath);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		m_CtlStategy.AddString(finder.GetFileTitle());
	}
	m_CtlComNum.SetCurSel(3);
	m_CtlStategy.SetCurSel(0);
	m_CtlComboDiv.SetCurSel(3);
	m_CtlComboRep.SetCurSel(1);
	OnSelchangeComboDiv() ;
}


void CInputLeft::OnRadioStock() 
{
	// TODO: Add your control notification handler code here
	InitCodeInfo(1);
	CString Code;
	
//    Code = m_BaseInfo.GetTextToCodeInput();
	if(Code.GetLength()>6)
		m_pInfoRight->InitCodeGubun(FALSE); // �����ɼ� 
	else
		m_pInfoRight->InitCodeGubun(TRUE); // �ֽĿɼ�
}

void CInputLeft::OnRadioFuture() 
{
	// TODO: Add your control notification handler code here
	InitCodeInfo(0);
	CString Code;
	
//    Code = m_BaseInfo.GetTextToCodeInput();
	if(Code.GetLength()>6)
		m_pInfoRight->InitCodeGubun(FALSE); // �����ɼ� 
	else
		m_pInfoRight->InitCodeGubun(TRUE); // �ֽĿɼ�
}

void CInputLeft::OnBtnApply() 
{
	// TODO: Add your control notification handler code here
//	CString path;
//	path = (LPCTSTR)(LPCSTR)AfxGetPctrData(10);	
//	AfxMessageBox(path);

    CString Code;
	
  //  Code = m_BaseInfo.GetTextToCodeInput();
	m_JMName = GetJMNameAndGubunFromCode(Code);
	Code ="000660"; // TESt
	m_CtlStategy.GetLBText(m_CtlStategy.GetCurSel(), m_strStName);
	if(Code=="" || m_strStName=="") 
	{
		AfxMessageBox("������̳� �������� ���õ��� �ʾҽ��ϴ�.");
		return;
	}

	if(Code.GetLength()>6)
		m_pInfoRight->InitCodeGubun(FALSE); // �����ɼ� 
	else
		m_pInfoRight->InitCodeGubun(TRUE); // �ֽĿɼ� 
/////////////////////////////////
	  m_stArrayPath.RemoveAll();
 
////////////////////////////////
//@��������
//#ifdef _DEBUG
// CString strSystemPath = m_strRootPath +  "\\" + STSYSTEM_D;
// CString strUserPath = m_strRootPath + "\\" + STUSER_D;
//#else
// CString strSystemPath = m_strRootPath + "\\" + STSYSTEM;
// CString strUserPath = m_strRootPath + "\\" + STUSER;
//#endif
//@��������
//@�����߰�
	CString strSystemPath = m_strRootPath + "\\" + PATH_DATA_STDATA_STSYSTEM;
	CString strUserPath = m_strRootPath + "\\" + PATH_USER_STDATA_SD;
//@�����߰�

	m_stArrayPath.Add(strSystemPath);
	m_stArrayPath.Add(strUserPath);

	if(!m_pStrategyItem)
	{
		m_pStrategyItem = m_strategyLoader.GetStrategyItem();// �޸� ���� 
		if(m_pStrategyItem == NULL) return;
		//����, DBMGr, Ű������ ���� 
		//int nRetValue = m_pStrategyItem->Create(this,m_hSocket,/*(long)&m_IDrMLib_CDBMg,*/(long)m_pStrategyItem,m_stArrayPath);
		int nRetValue = m_pStrategyItem->Create(this, (long)m_pStrategyItem,m_stArrayPath, m_pChartInterface);
		if(nRetValue==-1) 
		{
			m_pStrategyItem->DestoryWindow();
			delete m_pStrategyItem;
			m_pStrategyItem = NULL;
		}
	}

///
 
	CString strdll;
	m_CtlStategy.GetLBText(m_CtlStategy.GetCurSel(),strdll);
	strdll+=".ST";
//
//@��������
//#ifdef _DEBUG
// CString strPath = m_strRootPath + "\\" + STUSER_D +"\\" + strdll;
//#else
// CString strPath = m_strRootPath + "\\" + STUSER +"\\" + strdll;
//#endif
//@��������
//@�����߰�
 CString strPath = m_strRootPath + "\\" + PATH_USER_STDATA_SD +"\\" + strdll;
//@�����߰�
	//LoadForRun�����(String), �����Ͱ���(int),��¥(String)
	//   AfxMessageBox(strPath);
	CString Num;
	int nCurSel = m_CtlComNum.GetCurSel();
	m_CtlComNum.GetLBText(nCurSel,Num);
	//////////////
	CTime timeTime;

	DWORD dwResult = m_CtlDateTime.GetTime(timeTime);
	CString Time;

	if (dwResult == GDT_VALID)
	{
		int mon,day,year;
		year = timeTime.GetYear();
		mon=timeTime.GetMonth();
		day=timeTime.GetDay();

		if(mon<10 && day <10)
			Time.Format("%d0%d0%d",year,mon,day);
		else if(mon<10 && day > 9)
			Time.Format("%d0%d%d",year,mon,day);
		else if(mon>9 && day <10)
			Time.Format("%d%d0%d",year,mon,day);
		else 
			Time.Format("%d%d%d",year,mon,day);
	}
	/////////////
	
	BOOL bRetValue = m_pStrategyItem->LoadST(strPath,Code,atoi(Num),Time); //
	if(bRetValue)
	{
		m_pStrategyItem->Run();
	}
	
	//�ʱ�ȭ �ϱ� 
	
	m_pInfoRight->InitWebView();

	/* Server2�� �̿��Ͽ� AP ������ �ޱ� 
	ST_SEND2SERVER st;
	int Num= 200;
	st.m_pBuffer = (int*)&Num;
	st.m_lLength= sizeof(Num);
	st.m_strTRCode = "FIND0303";
	st.m_pRequester = this;
	HWND hWnd =GetDlgItem(IDD_DLG_ANALYSIS)->GetSafeHwnd();
	::SendMessage(m_hWnd,UMSG_SEND2SERVER,(WPARAM)&st,NULL);*/

}

//�̺�Ʈ�� �Ϸ� ��� �´�. : ���̺� ���� �����͸� ä���� �Ϸ� ���´�.
//�Լ� ���� �ʿ�!!! : KHD 
LRESULT CInputLeft::OnSTDataManagerMessagge( WPARAM wParam, LPARAM lParam)
{
	CString strDataItemName;
	CString strDiv, strRep;
	CStringArray strDate; //Date ������ ����
	ST_DATAINDEX Index;
	
	if(m_stDataIndex.GetSize()>0)
		m_stDataIndex.RemoveAll();

	if(wParam==(WPARAM)m_pStrategyItem)
	{
		m_pStrategyItem = (CIStrategyItem*)wParam;	
		m_pStrategyItem->Stop();
		ST_DRAWINFO* pInfo = (ST_DRAWINFO*)lParam;
		CPtrArray* pPtrArrayPlot = pInfo->pPtrArraySDataItem;
		m_pDataItem = NULL;
		int nSize = pPtrArrayPlot->GetSize();
		
		if(nSize ==0) return 0;
		
		CString strStatic;
		BOOL	bStatic;
		
		strStatic.Format("����: %s",m_pStrategyItem->GetTime());
		GetDlgItem(IDC_EDIT_STATIC)->SetWindowText(strStatic);
		bStatic = IsInsertGubun(m_pStrategyItem->GetTime());
/////////////List ä��� ///////////////////////////////////////
		m_pDataItem = (CISDataItem *)pPtrArrayPlot->GetAt(0);

		long nTotalData	= m_pDataItem->GetSize();
		//Data ��û //////////////////////////
		m_pStrategyItem->CalculateStatisticalInfo(nTotalData-nTotalData, nTotalData-1); //����ϴ� ����
		/////////////////////
		strDate.SetSize( nTotalData );

		m_pDataItem->GetAt(0,&strDataItemName);//�� ó�� ���� ó��
		InsertGubun(strDataItemName, bStatic);
		Index.nStart = 0;
		Index.strStartDate = strDataItemName;
		Index.strGubun ="[��ü]";
		m_CtlList.SetItemText(0,1,strDataItemName);

		m_pDataItem->GetAt(nTotalData-1,&strDataItemName);//�� ������ ���̳� 
		InsertGubun(strDataItemName, bStatic);
		Index.nEnd = nTotalData-1;
		Index.strEndDate = strDataItemName;
		m_CtlList.SetItemText(0,2,strDataItemName);
		m_stDataIndex.Add(Index);

		for(int i = 0;i<nTotalData;i++)
		{
			m_pDataItem->GetAt(i,&strDataItemName);
			strDate.SetAt(i,strDataItemName);
		}
		
		m_CtlComboDiv.GetLBText(m_CtlComboDiv.GetCurSel(),strDiv);
		m_CtlComboRep.GetLBText(m_CtlComboRep.GetCurSel(),strRep);//��Ȱ�� ��������
		int Div = atoi(strDiv); //���� ���� ���� ����.
		int num = strDate.GetSize();

		int Divnum =0;
		int Repnum;
		long double Repfloat;
		//��ø�� ���ϱ�.
		//num - ((Div-1)*Repnum/Div)
		if(strRep!="0")
		{
			Repnum = ((num/10) * m_CtlComboRep.GetCurSel());//atoi(strRep) *;
			int tot =num - ((Div-1)*Repnum/Div);
			Repfloat = long double(num/long double(tot));//Repfloat * num-Repnum = 400
		}
		else
		{
			Repnum = 0;
			Repfloat =1;
		}
		CString str;
		str.Format("%d",Repnum);		
		//��ø�� �־������~
		for(i =0; i<Div; i++)
		{
			//if(Repnum<Divnum  )
	//		if(int((Divnum - (Repnum/Div))*Repfloat)>0 &&int((Divnum - (Repnum/Div))*Repfloat)<num)
	//		{
	//			strDataItemName = strDate.GetAt(int((Divnum - (Repnum/Div))*Repfloat));
	//			Index.nStart = int((Divnum - (Repnum/Div))*Repfloat);
	//		}
	//		else
	//		{
			int a = int(((Divnum-(i*(Repnum/Div)))*Repfloat)-0.5);
			if(a>0)
			{
				strDataItemName = strDate.GetAt(a);
				Index.nStart = a;
			}
			else
			{
				strDataItemName = strDate.GetAt(Divnum );
				Index.nStart = int(Divnum);
				
			}
	//		}
			InsertGubun(strDataItemName, bStatic);
//			Index.nStart = Divnum;// Save
			Index.strStartDate = strDataItemName;
			Index.strGubun.Format("[%d����]",i+1);
			m_CtlList.SetItemText(i+1,1,strDataItemName);

			Divnum += (num/Div);
			if(Divnum > nTotalData-1)
				Divnum = nTotalData-1;

	
			//��� ���� 
			int b = int(((Divnum - (i*(Repnum/Div)))*Repfloat)-0.5);//int((Divnum+(Repnum/Div))*Repfloat);

			if(b<num && i!=Div)
			{
				strDataItemName = strDate.GetAt(b);
				Index.nEnd = b;
			}
			else
			{
				strDataItemName = strDate.GetAt(num-1);
				Index.nEnd = num-1;
			
			}

//			strDataItemName = strDate.GetAt(Divnum);
			InsertGubun(strDataItemName, bStatic);
//			Index.nEnd = Divnum;
			Index.strEndDate = strDataItemName;

			m_stDataIndex.Add(Index);//�ߺ� ���⿡ ����� �ɷ� ��Ʈ���� ����.
			m_CtlList.SetItemText(i+1,2,strDataItemName);

			Divnum++;
		}	
		/////////////////////////////////////////////////
		m_bIsView = TRUE;	
		m_pInfoRight->AllUpdateDlg(); // UpdateDlg
		////////////////////////////////////////////////////
		m_pInfoRight->m_CtlResult.EnableWindow(TRUE);
		return 0;
	}
	m_bIsView = FALSE;
	return 0;
}


BOOL CInputLeft::IsInsertGubun(CString str)
{
	if(str == "��")
		return TRUE;
	else
		return FALSE;
}

LRESULT CInputLeft::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case UMSG_ST_SEND2SERVER:
		{
			ST_SEND2SERVER *pSt = (ST_SEND2SERVER *)wParam;
			m_listRequested.AddTail(pSt->m_pRequester);
			Send2Server(pSt->m_strTRCode,(char*)pSt->m_pBuffer,pSt->m_lLength);
		}
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

// Ap ����... Server�� ������ �Լ� 
BOOL CInputLeft::Send2Server(LPCTSTR lpTrCode,LPCTSTR lpData,int nSize)
{
//	m_TRComm.SetTR(lpTrCode);
//	CString strTrCode = lpTrCode;
//	m_bRequestingNow = TRUE;
	return TRUE;//m_TRComm.Send2Server(lpData, nSize, FALSE,"N0080000");	
}
/////////////////////////////////////////////////////////////////////////////
// CApInput message handlers

//AP���� �����͸� �޴� �κ� : KHD
//���� ��������� �ʽ��ϴ�.
HRESULT CInputLeft::ReceiveData(HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID aTRData, DWORD dwTRDateLen)
{
//	char    sBuffer [12];           /* out buffer */
				
//	aTRData

	CString Start, End;
	long	lStart, lEnd;
	Start = m_CtlList.GetItemText(0,1);
	End	  = m_CtlList.GetItemText(0,2);
	
	
// �˻� ��ƾ /////////////////////
	if(Start==""||End=="")
	{
		AfxMessageBox("���� ��¥�� �����ּ���~");
		return S_OK;
	}
	Start.Remove('/');
	End.Remove('/');
	lStart = atol(Start);
	lEnd = atol(End);
	if(lStart > lEnd)
	{
		AfxMessageBox("Start��¥�� End��¥�� �ٲ�����ϴ�.");
		return S_OK;
	}
	else if(lStart == lEnd)
	{
		AfxMessageBox("Start��¥�� End��¥�� �����ϴ�.");
		return S_OK;
	}
//////////////////////////////////////

	int *test = (int*)aTRData;
	CString str;
	str.Format("%s | %s | ù��¥ = %d |size = %d", Start, End, test[0],sizeof(test) );
	AfxMessageBox(str);

	return S_OK;
};
//���� ������� �ʴ´�ϴ�. :KHD
HRESULT CInputLeft::RecieveError(int nError, LPCSTR szMessage)
{
	MessageBox(szMessage);
	return S_OK;
}

void CInputLeft::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_pStrategyItem)
	{
		m_pStrategyItem->DestoryWindow();
		delete m_pStrategyItem;
		m_pStrategyItem = NULL;
	}
	
	if(m_brBkColor.GetSafeHandle())
	{
		m_brBkColor.DeleteObject();
	}

	UnAdviseSharedInfo();

//KHD: 	m_TRComm.DestroyWindow();
}

BOOL CInputLeft::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	m_ctlApply.SetSkin(IDB_BITMAP_GUGAN_NOR,IDB_BITMAP_GUGAN_SEL,IDB_BITMAP_GUGAN_NOR,IDB_BITMAP_GUGAN_DIS);
	m_brBkColor.CreateSolidBrush(m_bgColor);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CInputLeft::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here

	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		pDC->SetBkColor(m_bgColor);
		return m_brBkColor;		
		break;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CInputLeft::OnSelchangeComboDiv() 
{
	// TODO: Add your control notification handler code here
	int Sel = m_CtlComboDiv.GetCurSel();
	int Count =m_CtlList.GetItemCount();
	if(Count>1)
	{
		for(int i=Count-1;i>0;i--)
			m_CtlList.DeleteItem(i);
	}
	
	for(int i =0;i<Sel+1;i++)
	{
		CString str;
		str.Format("%d",i+1);
		m_CtlList.InsertItem(i+1,str,RGB(0,0,0),RGB(242,248,254));
		//DateTime ���� ��ƾ 
	//	m_CtlList.SetDateTime(i+1,1);
	//	m_CtlList.SetDateTime(i+1,2);
	}
	
	m_pInfoRight->m_CtlResult.EnableWindow(FALSE);
	m_pInfoRight->SetEnableButton(FALSE);
	
	
}


void CInputLeft::InsertDateGubun(CString &str)
{
	if(str.GetLength()<4) return;
	str.Insert(4,'/');
	str.Insert(7,'/');
}

void CInputLeft::InsertTimeGubun(CString &str)
{
	int nSize = str.GetLength();
	if(nSize<4) return;

	str.Insert(nSize -2,':');
	str.Insert(nSize -4,"/");
	str.Insert(nSize -6,'/');
}

void CInputLeft::InsertGubun(CString &str, BOOL Gubun)
{
	CString strGubun;
	GetDlgItem(IDC_EDIT_STATIC)->GetWindowText(strGubun);

	if(strGubun.Find("ƽ")!=-1)
	{
		int nSize = str.GetLength();
		if(nSize<4) return;
		str.Insert(nSize -2,':');
		str.Insert(nSize -4,':');
		str.Insert(nSize -6,"/");
	
	}
	else
	{
		if(Gubun)
			InsertDateGubun(str);
		else
			InsertTimeGubun(str);// Time	
	}
}
//KHD : ���� ��ư ������ �߻��ϴ� �̺�Ʈ!!
void CInputLeft::SetCalculateData(int nIndexnum)
{
	ST_DATAINDEX Index;
	int nSize = m_stDataIndex.GetSize();
	if(nIndexnum>nSize)
		return;
	Index = m_stDataIndex.GetAt(nIndexnum);
	m_pStrategyItem->CalculateStatisticalInfo(Index.nStart, Index.nEnd);

}

void CInputLeft::Result()
{
	int CurSel = m_CtlComboDiv.GetCurSel();
	m_pInfoRight->SetRefresh(CurSel);
}
void CInputLeft::OnButton2() 
{
	// TODO: Add your control notification handler code here
	SetListFocus(0);
}

void CInputLeft::AllUpdateDataIndex()
{
	CString strDataItemName;
	int nSize = m_stDataIndex.GetSize();
	ST_DATAINDEX DataIndex;
	
	BOOL bStatic = IsInsertGubun(m_pStrategyItem->GetTime());
	for(int i = 1; i<nSize;i++)
	{
		DataIndex = m_stDataIndex.GetAt(i);
		m_pDataItem->GetAt(DataIndex.nStart,&strDataItemName);
		InsertGubun(strDataItemName,bStatic);
		m_CtlList.SetItemText(i,1,strDataItemName);
		DataIndex.strStartDate = strDataItemName;

		m_pDataItem->GetAt(DataIndex.nEnd,&strDataItemName);
		InsertGubun(strDataItemName,bStatic);
		m_CtlList.SetItemText(i,2,strDataItemName);
		DataIndex.strEndDate = strDataItemName;

		m_stDataIndex.SetAt(i,DataIndex);
	}

}

void CInputLeft::OnClickListDay(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
		HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = 0;
	m_pInfoRight->SetFocusIndex(phdn->iItem);
	*pResult = 0;
}

void CInputLeft::SetListFocus(int nIndex)
{
	if(nIndex == -1)
	{
		int nCurSel = m_CtlList.GetCurSel();
		m_CtlList.SetCurSel(nCurSel,FALSE);
	}
	else
	{
		m_CtlList.SetCurSel(nIndex);
	}	
}

BOOL CInputLeft::UnAdviseSharedInfo(LPCTSTR lpShareName/*=NULL*/)
{
	if(lpShareName==NULL)
	{
		int nSizeAdvice = m_strArrayAdviceInfo.GetSize();
		if(nSizeAdvice)
		{
			for(int nPos=0;nPos<nSizeAdvice;nPos++)
			{
//				m_BaseInfo.UnAdviseSharedInfo(m_strArrayAdviceInfo.GetAt(nPos),(CSharedBase*)this);
			}
		}
		m_strArrayAdviceInfo.RemoveAll();
	}
	else
	{
		int nSizeAdvice = m_strArrayAdviceInfo.GetSize();
		CString strShareNameInArray;
		if(nSizeAdvice)
		{
			for(int nPos=0;nPos<nSizeAdvice;nPos++)
			{
				strShareNameInArray = m_strArrayAdviceInfo.GetAt(nPos);
				if(strShareNameInArray==lpShareName)
				{
//					m_BaseInfo.UnAdviseSharedInfo(m_strArrayAdviceInfo.GetAt(nPos),(CSharedBase*)this);
					m_strArrayAdviceInfo.RemoveAt(nPos);
					break;
				}				
			}
		}		
	}
	return TRUE;
}

BOOL CInputLeft::AdviseSharedInfo(LPCTSTR lpShareName, int nSharedType, int nSharedArea)
{
	if(!m_bInitShareBase)
	{
//		m_hCHild = GetParent()->GetSafeHwnd(); // ���ϵ� �����츦 ���� ������ ����. �������� 
//		m_hCtrl = (HWND)this; // ���� ������ ���ߴ��� ����,�ڵ��� �־ ����.
		m_strArrayAdviceInfo.RemoveAll();
		m_bInitShareBase = TRUE;
	}
//	BOOL bRetValue = m_BaseInfo.AdviseSharedInfo(lpShareName,(CSharedBase*)this,nSharedType,nSharedArea);
	m_strArrayAdviceInfo.Add(lpShareName);
	return FALSE;//bRetValue;
}

BOOL CInputLeft::SetSharedData2(HWND hArea,int nArea, LPCTSTR lpShareName,BYTE* pData, int nLen, int nStock, BOOL bOnlySetData)
{
	if(m_bInitShareBase)
	{
//		return m_BaseInfo.SetSharedData2(hArea,nArea,lpShareName,pData,nLen,nStock,bOnlySetData);
	}
	return FALSE;
}

long CInputLeft::ChangeSharedData(CString sShareName, BYTE* pData, int nLen, int nStock/*=0*/)
{
	if(sShareName==SAVINGSTRATEGY)
	{
#ifdef _DEBUG
		CString strUserPath = m_strRootPath + STUSERFILE_D;
#else
		CString strUserPath = m_strRootPath + STUSERFILE;
#endif
	//
		CFileFind finder;
		BOOL bWorking = finder.FindFile(strUserPath);
		CString strFileTitle;
		int nResultOfFindString;
		int nCurSel = m_CtlStategy.GetCurSel();
		CString strTitleCurSel;
		m_CtlStategy.GetLBText(nCurSel,strTitleCurSel);
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			strFileTitle = finder.GetFileTitle();
			nResultOfFindString = m_CtlStategy.FindStringExact(0,strFileTitle);
			if(nResultOfFindString==CB_ERR)
			{
				m_CtlStategy.AddString(strFileTitle);
			}			
		}
		nResultOfFindString = m_CtlStategy.FindStringExact(0,strTitleCurSel);
		if(nResultOfFindString==CB_ERR)
		{
			m_CtlStategy.SetCurSel(0);
		}
		else
		{
			m_CtlStategy.SetCurSel(nResultOfFindString);
		}
	}
	return 0;
}
