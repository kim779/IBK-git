// TabTrade.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TabTrade.h"
//#include "InputLeft.h"
#include "DlgAnalysis.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabTrade dialog


CTabTrade::CTabTrade(CWnd* pParent /*=NULL*/)
	: CDlgInterface(CTabTrade::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabTrade)
	//}}AFX_DATA_INIT
	m_pFont = NULL;
}


void CTabTrade::DoDataExchange(CDataExchange* pDX)
{
	CDlgInterface::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabTrade)
	//}}AFX_DATA_MAP
//	DDX_Control(pDX, IDC_EXPLORER1, m_Browser);
}


BEGIN_MESSAGE_MAP(CTabTrade, CDlgInterface)
	//{{AFX_MSG_MAP(CTabTrade)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabTrade message handlers

BOOL CTabTrade::OnInitDialog() 
{
	CDlgInterface::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitGrid();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabTrade::InitGrid()
{
	if(!m_TradeGridCtrl.GetSafeHwnd())
	{
		CRect rect1;
		GetClientRect(rect1);
		rect1.top =1;
		rect1.left +=1;
		rect1.right-=10;
//		rect1.bottom -=5;
		HINSTANCE hInst = AfxGetResourceHandle();
		AfxSetResourceHandle(m_pDlgAnalysis->m_hRsc);
		if(!m_TradeGridCtrl.Create(rect1,this,IDC_STATIC_RESULT,WS_CHILD | WS_TABSTOP | WS_VISIBLE ))
		{
			AfxMessageBox("No");
			return;
		}
		m_TradeGridCtrl.SetEditable(FALSE);
		m_TradeGridCtrl.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(255, 255, 255));
		m_TradeGridCtrl.SetGridLineColor(RGB(204,210,224));
		m_TradeGridCtrl.SetColumnCount(10);
		m_TradeGridCtrl.SetRowCount(1);
		m_TradeGridCtrl.SetBkColor(CLR_BACKGROUND);
		m_TradeGridCtrl.ShowScrollBar(SB_VERT );
		m_TradeGridCtrl.SetRowHeight(0, 23);
		m_TradeGridCtrl.SetFont(m_pFont);

		char* GridIndexName[] = {"NO","������","�ŸŽ���","����","����","����/û��ݾ�","�Ǻ�����","��������","�Ǻ����ͷ�","�������ͷ�"};
		m_TradeGridCtrl.SetColumnWidth(0,40);
		m_TradeGridCtrl.SetColumnWidth(1,70);
		m_TradeGridCtrl.SetColumnWidth(2,80);
		m_TradeGridCtrl.SetColumnWidth(3,85);
		m_TradeGridCtrl.SetColumnWidth(4,40);
		m_TradeGridCtrl.SetColumnWidth(5,100);
		m_TradeGridCtrl.SetColumnWidth(6,80);
		m_TradeGridCtrl.SetColumnWidth(7,80);
		m_TradeGridCtrl.SetColumnWidth(8,80);
		m_TradeGridCtrl.SetColumnWidth(9,80);

		for(int col =0 ; col <10; col++)
		{
			m_TradeGridCtrl.SetItemBkColour(0,col,RGB(242,248,254));
			m_TradeGridCtrl.SetItemText(0,col,GridIndexName[col]);
		}
		//m_TradeGridCtrl.ExpandColumnsToFit();
		AfxSetResourceHandle(hInst);
	}
}

void CTabTrade::SetDataSpreadGrid()
{
	CString strResultData;
	CStringArray ResultData;
	CStrategyResult* Result;
	CStringArray stNujukArray;//�������ͷ�
	CArray<double,double> dGunSu;
	CArray<double,double> dSonIk;//�������ͷ�
	BOOL bNujuk = FALSE;

	DWORD nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;//�׸��� ����
	m_TradeGridCtrl.SetRowCount(1);

	char* szParam1 = "%.2f";
	if(m_pDlgAnalysis->m_nCodeType == 0)	szParam1 = "%2.f";
	CIStrategyItem* Strate = m_pDlgAnalysis->GetStrategyItem();
	int nSize = Strate->GetCntStrategyResult();
	
	for(int i =0 ;i < nSize; i++)
	{	
		Result = Strate->GetStrategyResult(i);
		if(Result==NULL) break;//���� ó�� :KHD 

		//���� �Ľ� �ؼ� �����͸� �ʱ�ȭ �ؼ� �ִ´�.
		ResultData.RemoveAll();
		strResultData.Format("%d",i+1);
		ResultData.Add(strResultData);//NO

		switch(Result->m_DealType) {
		case CStrategyResult::NONE:strResultData = "NONE"; bNujuk = FALSE;
			break;
		case CStrategyResult::BUY:strResultData  = "�ż�����";bNujuk = FALSE;
			break;
		case CStrategyResult::EXITSHORT:strResultData  = "�ŵ�û��";bNujuk = FALSE;
			break;
		case CStrategyResult::SELL:strResultData  = "�ŵ�����";bNujuk = TRUE;
			break;
		case CStrategyResult::EXITLONG: strResultData = "�ż�û��";bNujuk = TRUE;
			break;
		}

		ResultData.Add(strResultData);//Position
		//Time
		strResultData.Format("%2.f",Result->m_dTime);
		m_pDlgAnalysis->InsertGubun(strResultData);
		ResultData.Add(strResultData);//�ŸŽ���

		//strResultData.Format("%2.f",Result->m_dPrice);
		strResultData.Format(szParam1, Result->m_dPrice);
		strResultData = GetComma(strResultData);
		ResultData.Add(strResultData);//����

		strResultData.Format("%ld",Result->m_lQty);
		strResultData = GetComma(strResultData);
		ResultData.Add(strResultData);//����

		//strResultData.Format("%2.f",Result->m_dAmount);
		strResultData.Format(szParam1, Result->m_dAmount);
		strResultData = GetComma(strResultData);
		ResultData.Add(strResultData);//���Աݾ�

		//strResultData.Format("%2.f",Result->m_dProfit);
		strResultData.Format(szParam1, Result->m_dProfit);
		strResultData = GetComma(strResultData);
		ResultData.Add(strResultData);//�Ǻ�����

		//strResultData.Format("%2.f",Result->m_dProfitAccmulated);
		strResultData.Format(szParam1,Result->m_dProfitAccmulated);
		strResultData = GetComma(strResultData);			
		ResultData.Add(strResultData);//��������


		strResultData.Format("%.2f%%",Result->m_dProfitRate);
		if(bNujuk)//�Ǻ����ͷ�
			dGunSu.Add(Result->m_dProfitRate);
		strResultData = GetComma(strResultData);
		ResultData.Add(strResultData);//�Ǻ����ͷ�

		if(bNujuk)
			dSonIk.Add(atof(strResultData));

		strResultData.Format("%.2f%%",Result->m_dProfitRateAccmulated);//�������ͷ�
		strResultData = GetComma(strResultData);

		if(bNujuk)
			stNujukArray.Add(strResultData);
		
		ResultData.Add(strResultData);

		int nRow = m_TradeGridCtrl.InsertRow("");
		m_TradeGridCtrl.SetRowHeight(nRow, 23);
		for(int col = 0 ; col < ResultData.GetSize(); col++)
		{
			strResultData = ResultData.GetAt(col);
			m_TradeGridCtrl.SetItemText(m_TradeGridCtrl.GetRowCount()-1, col, strResultData,nFormat);
			if(strResultData== "�ż�û��"||strResultData == "�ŵ�û��")
			{
				m_TradeGridCtrl.SetItemFgColour(m_TradeGridCtrl.GetRowCount()-1,col,RGB(0,0,255));
				m_TradeGridCtrl.SetItemFgColour(m_TradeGridCtrl.GetRowCount()-1,col+1,RGB(0,0,255));
			}
			else if(strResultData == "�ż�����"||strResultData == "�ŵ�����" )
			{
				m_TradeGridCtrl.SetItemFgColour(m_TradeGridCtrl.GetRowCount()-1,col,RGB(255,0,0));
				m_TradeGridCtrl.SetItemFgColour(m_TradeGridCtrl.GetRowCount()-1,col+1,RGB(255,0,0));
			}

			if(strResultData.Find('-')>=0)
				m_TradeGridCtrl.SetItemFgColour(m_TradeGridCtrl.GetRowCount()-1,col,RGB(0,0,255));
		}
	}

	if(m_pDlgAnalysis && m_pDlgAnalysis->m_pdlgInfo)
	{
		if(m_pDlgAnalysis->m_pdlgInfo->m_EarnChart->GetSafeHwnd()!=NULL)
		{
			m_pDlgAnalysis->m_pdlgInfo->m_EarnChart->PrepareShow(stNujukArray.GetSize());
			m_pDlgAnalysis->m_pdlgInfo->m_EarnChart->ShowData2Chart(stNujukArray,stNujukArray.GetSize());
			m_pDlgAnalysis->m_pdlgInfo->m_EarnChart->ShowData1Chart((long)&dGunSu);
		}
		if(m_pDlgAnalysis->m_pdlgInfo->m_SonikChart->GetSafeHwnd()!=NULL)
		{
			m_pDlgAnalysis->m_pdlgInfo->m_SonikChart->ShowData1Chart((long)&dSonIk);//�������ͷ� : ����íƮ ������
		}
	}	
}
void CTabTrade::OpenWebView(CString FileName)
{
	//CString strPath = m_strFilePath+FileName;
	//m_Browser.Navigate(strPath,NULL,NULL,NULL,NULL);
}

//File Pasing!!
void CTabTrade::MakeHtmlFile()
{

	CStdioFile NewFile;
	CString str,HtmlData;
	
	///String �Ľ̷�ƾ //////////////////////..
	InitHtml();
	HtmlData = m_strHtml;
	HtmlData = ParsingstrHtml(HtmlData);
	
	//////NewFile�� ���� 1. ���� �м�,�����м�,�Ÿų�����ƾ �ʿ�//////////////////////////
	SetFileAttributes (m_strFilePath+"\\�Ÿų���.htm", FILE_ATTRIBUTE_NORMAL);//protect Read Only Error
	if(!NewFile.Open(m_strFilePath+"\\�Ÿų���.htm",CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox("Do Not Create Html File.");
		return;
	}else
		NewFile.WriteString(HtmlData);

		
	NewFile.Close();
	OpenWebView("\\�Ÿų���.htm");
}

CString CTabTrade::ParsingstrHtml(CString strHtml)
{

	CString TableCode;
	CString str2;
	CString strResultData;
	CStringArray ResultData;
	CStrategyResult* Result;
	CString str;

	CString m_pHtmlCodeNameArray[TRAMAXDATANUM] =
	{"Num","Position","Date","Price","Amount","Entry","ProfitAccum","NuAccum","GunRate","NuJukRate"};

	str= _T("<tr Align=\"Right\"><td width=\"39\" height=\"18\" ><font face=\"����\" color=\"black\"><span style=\"font-size:10pt;\">Num</span></font></td>\n")
      _T("<td width=\"71\" height=\"18\"><font face=\"����\" color=\"FC1\"><span style=\"font-size:9pt;\">Position</span></font></td>\n")
      _T("<td width=\"70\" height=\"18\"><font face=\"����\" color=\"FC1\"><span style=\"font-size:9pt;\">Date</span></font></td>\n")
      _T("<td width=\"70\" height=\"18\"><font face=\"����\" color=\"black\"><span style=\"font-size:9pt;\">Price</span></font></td>\n")
      _T("<td width=\"35\" height=\"18\"><font face=\"����\" color=\"black\"><span style=\"font-size:9pt;\">Amount</span></font></td>\n")
      _T("<td width=\"73\" height=\"18\"><font face=\"����\" color=\"black\"><span style=\"font-size:9pt;\">Entry</span></font></td>\n")
      _T("<td width=\"73\" height=\"18\"><font face=\"����\" color=\"FC6\"><span style=\"font-size:9pt;\">ProfitAccum</span></font></td>\n")
      _T("<td width=\"78\" height=\"18\"><font face=\"����\" color=\"FC7\"><span style=\"font-size:9pt;\">NuAccum</span></font></td>\n")
      _T("<td width=\"73\" height=\"18\"><font face=\"����\" color=\"FC8\"><span style=\"font-size:9pt;\">GunRate</span></font></td>\n")
      _T("<td width=\"78\" height=\"18\"><font face=\"����\" color=\"FC9\"><span style=\"font-size:9pt;\">NuJukRate</span></font></td>\n")
	  _T("</tr>\n");
  

	CIStrategyItem* Strate = m_pDlgAnalysis->GetStrategyItem();
	int nSize = Strate->GetCntStrategyResult();
	CStringArray stNujukArray;//�������ͷ�
	CArray<double,double> dGunSu;
	CArray<double,double> dSonIk;//�������ͷ�
	BOOL bNujuk = FALSE;
	for(int i =0 ;i < nSize; i++)
	{	
		Result = Strate->GetStrategyResult(i);
		if(Result==NULL) break;//���� ó�� :KHD 
		str2 = str;

		//���� �Ľ� �ؼ� �����͸� �ʱ�ȭ �ؼ� �ִ´�.
		ResultData.RemoveAll();
		strResultData.Format("%d",i+1);
		ResultData.Add(strResultData);
		
		switch(Result->m_DealType) {
		case CStrategyResult::NONE:strResultData = "NONE"; bNujuk = FALSE;
			break;
		case CStrategyResult::BUY:strResultData  = "�ż�����";bNujuk = FALSE;
			break;
		case CStrategyResult::EXITSHORT:strResultData  = "�ŵ�û��";bNujuk = FALSE;
			break;
		case CStrategyResult::SELL:strResultData  = "�ŵ�����";bNujuk = TRUE;
			break;
		case CStrategyResult::EXITLONG: strResultData = "�ż�û��";bNujuk = TRUE;
			break;
		}
		
		ResultData.Add(strResultData);
		//Time
		strResultData.Format("%2.f",Result->m_dTime);
 		m_pDlgAnalysis->InsertGubun(strResultData);
		ResultData.Add(strResultData);
		
		if(m_pDlgAnalysis->m_nCodeType == 0)//�ֽ��̸�
		{
			strResultData.Format("%2.f",Result->m_dPrice);
			strResultData = GetComma(strResultData);
			ResultData.Add(strResultData);

			strResultData.Format("%ld",Result->m_lQty);
			strResultData = GetComma(strResultData);
			ResultData.Add(strResultData);

			strResultData.Format("%2.f",Result->m_dAmount);
			strResultData = GetComma(strResultData);
			ResultData.Add(strResultData);
			
			strResultData.Format("%2.f",Result->m_dProfit);
			strResultData = GetComma(strResultData);
			ResultData.Add(strResultData);

			strResultData.Format("%2.f",Result->m_dProfitAccmulated);
			strResultData = GetComma(strResultData);			
			ResultData.Add(strResultData);
		}
		else
		{
			strResultData.Format("%.2f",Result->m_dPrice);
			strResultData = GetComma(strResultData);
			ResultData.Add(strResultData);

			strResultData.Format("%ld",Result->m_lQty);
			strResultData = GetComma(strResultData);
			ResultData.Add(strResultData);

			strResultData.Format("%.2f",Result->m_dAmount);
			strResultData = GetComma(strResultData);
			ResultData.Add(strResultData);

			strResultData.Format("%.2f",Result->m_dProfit);
			strResultData = GetComma(strResultData);
			ResultData.Add(strResultData);

			strResultData.Format("%.2f",Result->m_dProfitAccmulated);
			strResultData = GetComma(strResultData);
			ResultData.Add(strResultData);
		}
		
		strResultData.Format("%.2f%%",Result->m_dProfitRate);
		if(bNujuk)//�Ǻ����ͷ�
			dGunSu.Add(Result->m_dProfitRate);
		strResultData = GetComma(strResultData);
		ResultData.Add(strResultData);

		//�Ǻ����ͷ��� �����ش�. �Ǻ����ͷ��� int���·� ó���Ѵ�.
		// strResultData.Format("%2.f",Result->m_dProfitRate);
			
		double m_dProfitRate = 0;			// �Ǻ����ͷ�
		if(Result->m_dProfitRate > 0)
		{
			strResultData.Format("%.f",Result->m_dProfitRate +0.5);
		}
			
		else if(Result->m_dProfitRate < 0)
		{
			strResultData.Format("%.f",Result->m_dProfitRate -0.5);
		}

		//else if(Result->m_dProfitRate == 0)
		//{
		//	strResultData.Format("%.f",Result->m_dProfitRate);
		//}
		//strResultData.Format("%.f",Result->m_dProfitRate);   
		if(bNujuk) {
			dSonIk.Add(atof(strResultData));
		//dSonIk.Add(Result->m_dProfitAccmulated);
		}

		strResultData.Format("%.2f%%",Result->m_dProfitRateAccmulated);//�������ͷ�
		strResultData = GetComma(strResultData);
		if(bNujuk)
			stNujukArray.Add(strResultData);
		ResultData.Add(strResultData);
		
		for(int j =0;j < TRAMAXDATANUM;j++)
		{
			if(j==1)
			{
				if(ResultData.GetAt(j) == "�ż�����"||ResultData.GetAt(j) == "�ŵ�����" )
				{
					for(int i = 7; i<TRAMAXDATANUM;i++)
						ResultData.SetAt(i," ");	
				}

				if(ResultData.GetAt(j) == "�ż�û��"||ResultData.GetAt(j) == "�ŵ�û��" )
					str2.Replace("\"FC1\"","\"blue\"");
				else if(ResultData.GetAt(j) == "�ż�����"||ResultData.GetAt(j) == "�ŵ�����" )
					str2.Replace("\"FC1\"","\"red\"");
		
			}
			if(j > 5)
			{
				CString Data;
				CString Index;
				Index.Format("\"FC%d\"",j);
				Data =ResultData.GetAt(j);
				if(Data.Find("-",0)<0)
					str2.Replace(Index,"\"black\"");//�߰ߵ��� ���� 
				else
					str2.Replace(Index,"\"blue\"");
			}
		
			str2.Replace(m_pHtmlCodeNameArray[j], ResultData.GetAt(j));
		}
	
		TableCode +=str2;
	}
	strHtml.Replace("<!--ADD-->",TableCode);
	//KHD : �ּ�ó�� 
	m_pDlgAnalysis->m_pdlgInfo->m_EarnChart->PrepareShow(stNujukArray.GetSize());
	m_pDlgAnalysis->m_pdlgInfo->m_EarnChart->ShowData2Chart(stNujukArray,stNujukArray.GetSize());
	m_pDlgAnalysis->m_pdlgInfo->m_EarnChart->ShowData1Chart((long)&dGunSu);
	m_pDlgAnalysis->m_pdlgInfo->m_SonikChart->ShowData1Chart((long)&dSonIk);//�������ͷ� : ����íƮ ������
	stNujukArray.RemoveAll();
	dGunSu.RemoveAll();
	dSonIk.RemoveAll();

	return strHtml;
}

void CTabTrade::OnDestroy() 
{
	CDlgInterface::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_TradeGridCtrl.GetSafeHwnd())
		m_TradeGridCtrl.DestroyWindow();
}

void CTabTrade::InitHtml()
{
	if(m_strHtml!="")return;
	CStdioFile DefaultFile;
	CString str;
	
	if(!DefaultFile.Open(m_strFilePath+"\\����-�Ÿų���.htm",CFile::modeRead))
	{
		AfxMessageBox("Do Not Open Html File.");
		return;
	}

	for(;;){
		if(DefaultFile.ReadString(str))
		{
			str += "\r\n";
			m_strHtml+=str;
		}
		else
			break;
	}	
	m_strHtml.Replace("white", "Black");
	DefaultFile.Close();
}

CString CTabTrade::GetStrValue(HKEY Hkey,CString strkey, CString strsubkey,CString strRValue)
{
   LONG err;
   HKEY openKey;
   BYTE Data[128];
   DWORD type;
   DWORD size=sizeof(Data);

   err=RegOpenKey(Hkey,strkey,&openKey);

   if(ERROR ==err)
	   err=RegQueryValueEx(openKey,strsubkey,0,&type,Data,&size);
	 
   if(err==ERROR_SUCCESS)
	{
		RegCloseKey(openKey);
		CString str(Data);
		return str;
	}
	
   RegCloseKey(openKey);
	return strRValue;

}

void CTabTrade::SetDataToExcel()
{
	SYSTEMTIME sTime;
	GetSystemTime(&sTime);

	CString strTempPath("C:\\Temp");
	CString strFilePath;
	strFilePath.Format("%s\\�Ÿų���_%s_%s_%04d%02d%02d_%02d%02d%02d.nc2", strTempPath, m_pDlgAnalysis->m_stCodeName, m_pDlgAnalysis->m_stSTName,
				sTime.wYear, sTime.wMonth, sTime.wDay, sTime.wHour+9, sTime.wMinute, sTime.wSecond);

	CString strExcel = "NO" ;
	CString strKey = "Excel.csv\\shell\\open\\command";
	int Ver = 60 ;
	HKEY hkey = HKEY_CLASSES_ROOT ;
	strExcel = GetStrValue(hkey, strKey, "", "NO") ;
	strExcel.MakeLower() ;
	int start = strExcel.Find("excel.exe") ;
	strExcel = strExcel.Left(start + 9) ;
	if(strExcel.Left(1).Compare("\"") == 0)
		strExcel = strExcel.Right(strExcel.GetLength() - 1) ;

	CreateDirectory(strTempPath, NULL); 
	CFile file;
	CFileException e;
	if( !file.Open( strFilePath, CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
		#endif
    }

	CString strColumnHeader("NO\t������\t�ŸŽ���\t����\t����\t����/û��ݾ�\t�Ǻ�����\t��������\t�Ǻ����ͷ�\t�������ͷ�\r\n");
	file.Write(strColumnHeader, strColumnHeader.GetLength());

	CString strAllData;
	CString strResultData;
	char* szParam1 = "%.2f";
	CStrategyResult* Result = NULL;
	CIStrategyItem* Strate = m_pDlgAnalysis->GetStrategyItem();
	int nSize = Strate->GetCntStrategyResult();
	for(int i =0 ;i < nSize; i++)
	{	
		Result = Strate->GetStrategyResult(i);
		if(Result==NULL) break;//���� ó�� :KHD 

		// �ʱ�ȭ
		strAllData.Empty();

		// NO
		strResultData.Format("%d",i+1);
		strAllData += strResultData;
		strAllData += _T("\t");

		switch(Result->m_DealType) {
		case CStrategyResult::NONE:strResultData = "NONE";
			break;
		case CStrategyResult::BUY:strResultData  = "�ż�����";
			break;
		case CStrategyResult::EXITSHORT:strResultData  = "�ŵ�û��";
			break;
		case CStrategyResult::SELL:strResultData  = "�ŵ�����";
			break;
		case CStrategyResult::EXITLONG: strResultData = "�ż�û��";
			break;
		}

		// ������
		strAllData += strResultData;
		strAllData += _T("\t");

		// �ŸŽ���
		strResultData.Format("%2.f",Result->m_dTime);
		m_pDlgAnalysis->InsertGubun(strResultData);
		strAllData += strResultData;
		strAllData += _T("\t");

		// ����
		strResultData.Format(szParam1, Result->m_dPrice);
		strResultData = GetComma(strResultData);
		strAllData += strResultData;
		strAllData += _T("\t");

		// ����
		strResultData.Format("%ld",Result->m_lQty);
		strResultData = GetComma(strResultData);
		strAllData += strResultData;
		strAllData += _T("\t");

		// ���Աݾ�
		strResultData.Format(szParam1, Result->m_dAmount);
		strResultData = GetComma(strResultData);
		strAllData += strResultData;
		strAllData += _T("\t");

		// �Ǻ�����
		strResultData.Format(szParam1, Result->m_dProfit);
		strResultData = GetComma(strResultData);
		strAllData += strResultData;
		strAllData += _T("\t");

		// ��������
		strResultData.Format(szParam1,Result->m_dProfitAccmulated);
		strResultData = GetComma(strResultData);			
		strAllData += strResultData;
		strAllData += _T("\t");

		// �Ǻ����ͷ�
		strResultData.Format("%.2f%%",Result->m_dProfitRate);
		strResultData = GetComma(strResultData);
		strAllData += strResultData;
		strAllData += _T("\t");

		// �������ͷ�
		strResultData.Format("%.2f%%",Result->m_dProfitRateAccmulated);
		strResultData = GetComma(strResultData);
		strAllData += strResultData;
		strAllData += _T("\t");

		strAllData +=_T("\r\n");
		file.Write(strAllData, strAllData.GetLength());
	}

	file.Close();

	// 04.17.2002 ���ϸ� �յڿ� ����ǥ(" ")�� �־�� �Ѵ�
	strExcel = strExcel + " \"" + strFilePath + "\"";	

	if(::WinExec((LPCTSTR)strExcel, SW_SHOW) > 31)
		return;
	else	
	{
		CString szMsg;
		szMsg.Format("�� ������ \"%s\"��� ���ϸ����� ����Ǿ� �ֽ��ϴ�.",strFilePath);
		AfxMessageBox(szMsg); //,"nfxChart") ;
		return;
	}
	return;
}
