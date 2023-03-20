// TabResult.cpp : implementation file
//

#include "stdafx.h"

#include "TabResult.h"
#include "Resource.h"
#include "DlgAnalysis.h"
#include ".\tabresult.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabResult dialog

CTabResult::CTabResult(CWnd* pParent /*=NULL*/)
	: CDlgInterface(CTabResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabResult)
	//}}AFX_DATA_INIT
	m_pInput = NULL;
//	m_pWebView = NULL;
	m_iDateIndex =0 ;
	m_pFont = NULL;
}


void CTabResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabResult)
	//}}AFX_DATA_MAP
//	DDX_Control(pDX, IDC_EXPLORER, m_Browser);
}


BEGIN_MESSAGE_MAP(CTabResult, CDialog)
	//{{AFX_MSG_MAP(CTabResult)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabResult message handlers

BOOL CTabResult::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_brBackground.CreateSolidBrush( CLR_BACKGROUND);
	InitGrid();
	return TRUE;  // return TRUE unless you set the focus to a control	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CTabResult::InitPasingData()
{
//	m_nViewPointIndex = {1,3,};
	
}
void CTabResult::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}
void CTabResult::OpenWebView(CString FileName)
{
	CString strPath = m_strFilePath+FileName;
	COleVariant varEmpty;
	COleVariant varURL(m_strFilePath + FileName);
//	m_Browser.Navigate2(varURL,varEmpty,varEmpty,varEmpty,varEmpty);
	return;
}

//Html ������ String�� �ε��Ͽ� �Ľ��Ͽ� ����Ÿ�� ä��� Html������ ���� �Ѵ�. 
void CTabResult::MakeHtmlFile()
{
	//���� �м� ���� �ε�!!
	CStdioFile NewFile;
///String �Ľ̷�ƾ //////////////////////..
	CString strHtml;
	InitHtml();
	strHtml = m_strHtml;
	strHtml = ParsingstrHtml(strHtml);
//////NewFile�� ���� 1. ���� �м�,�����м�,�Ÿų�����ƾ �ʿ�//////////////////////////

	SetFileAttributes (m_strFilePath+"\\�����м�.htm", FILE_ATTRIBUTE_NORMAL);//protect Read Only Error 
	if(!NewFile.Open(m_strFilePath+"\\�����м�.htm",CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox("Do Not Create Html File.");
		return;
	}else
		NewFile.WriteString(strHtml);
	
	NewFile.Close();
	OpenWebView("\\�����м�.htm");
}
CString CTabResult::ParsingstrHtml(CString strHtml)
{
	//try
	//{
	//	//�ӵ� ����� ���� �̸� �Ľ��� ���� �迭�� ���� //
	//	double Data;
	//	CString strData;
	//	CString m_pHtmlCodeNameArray[MAXDATANUM] = 
	//	{"AccumProf","AAverProf","CurPosRate","TotalSon","TotalProf",
	//	"Totalloss","TotalPL","MaxProf","MaxLoss","AverSon","AverProf",
	//	"AverLoss",	"TotalEnt","NoCleanTotal","NoClean","TotalTrade",
	//	"ProfTrade","LossTrade","PnLn","PTRate","LTRate",
	//	"PrdSign","MaxConProfN","MaxConLossN","SOverRate","MaxConProf",
	//	"MaxConLoss","TotalStick","PTStick","LTStick","PrdParty",
	//	"EntryStick","CleanStick","ATStick","PTAStick","PLTAStick"};
	//	int m_nViewPointIndex[11] = {15,16,17,18,22,23,27,28,29,31,32}; 
	//	/////////////////////////////////
	//	
	//	CString strName;
	//	CStrategyResult* Result;
	//	ST_DATAINDEX stData;
	//	CIStrategyItem* Strate = m_pDlgAnalysis->GetStrategyItem();
	//
	//	int nSize = Strate->GetCntStrategyResult();
	//	if(nSize > 0)
	//	{
	//		Result = Strate->GetStrategyResult(nSize-1);
	//		switch(Result->m_DealType) {
	//		case CStrategyResult::NONE:strName = "NONE";
	//			break;
	//		case CStrategyResult::BUY:strName  = "�ż�����";
	//			break;
	//		case CStrategyResult::EXITSHORT:strName  = "�ŵ�û��";
	//			break;
	//		case CStrategyResult::SELL:strName  = "�ŵ�����";
	//			break;
	//		case CStrategyResult::EXITLONG: strName = "�ż�û��";
	//			break;
	//		}
	//	}
	//	else
	//		strName = "NONE";
	//	
	//	m_strCurrentPos = strName;
	//	strHtml.Replace("CurrentPos",strName);
	//	m_pDlgAnalysis->m_pdlgInfo->m_ctlstPosition.SetText(m_strCurrentPos);

	//	strName = m_pDlgAnalysis->m_stSTName;
	//	m_strStrategyName = strName;
	//	strHtml.Replace("StrategyName",strName);
	//	m_pDlgAnalysis->m_pdlgInfo->m_ctlSTFILENAME.SetText(m_strStrategyName);

	//	m_strItemName = m_pDlgAnalysis->m_stCodeName;
	//	strHtml.Replace("ItemName", m_strItemName);
	//	m_pDlgAnalysis->m_pdlgInfo->m_ctlstName.SetText(m_strItemName);
	//	
	//	if(m_iDateIndex < m_pDlgAnalysis->m_stDataIndex.GetCount())
	//	{
	//		stData = m_pDlgAnalysis->m_stDataIndex.GetAt(m_iDateIndex);

	//		strName.Format(" %s %s  ~ %s", stData.strGubun, stData.strStartDate,stData.strEndDate);
	//		m_strPriod = strName;
	//		strHtml.Replace("Priod", strName);
	//		m_pDlgAnalysis->m_pdlgInfo->m_ctlstPeriod.SetText(m_strPriod);
	//	}

	//	//���� ���ͷ� ���� 
	//	Data = Strate->GetStatisticalInfo(0);
	//	strData.Format("%.2f", Data);
	//	m_strACCumProf = strData;
	//	strHtml.Replace("AccumProf",strData);
	//	m_pDlgAnalysis->m_pdlgInfo->m_ctlstNujuk.SetText("dd");//m_strACCumProf);

	//	///////////////////////
	//////�ɼǿ� ���� ��� : 8�ڸ��� ���� 6�ڸ��� �ֽ� �ɼ� //////////////////////////////////////////////////////////////			
	//	for(int i = 1; i<MAXDATANUM;i++)
	//	{
	//		if(i==14) continue;//��û���� ���� ���� �ʽ��ϴ�.
	//		if(i==24)// �����ʰ����ͷ�:�������ͷ� - �Ⱓ����� ����!
	//		{
	//			Data =atof(m_strACCumProf)-Strate->GetStatisticalInfo(21);
	//		}
	//		else
	//			Data = Strate->GetStatisticalInfo(i);
	//		//�޸� ��ƾ �ʿ� Default!! 
	//		if(i==2||i==6||i==18||i==19||i==20||i==21||i==24||i==30||i==33||i==34||i==35)
	//			strData.Format("%.2f", Data);
	//		else
	//		{
	//			strData.Format("%d", (INT)Data);
	//		}
	//		//�ֽ� �ɼ��� ��� .... �Ҽ��� �ڸ� �Ⱥ��϶�...
	//		//�Ѽ���, ������, �Ѽս�, �Ǻ��ִ�����, �Ǻ��ִ�ս�,������, ��û��������û��, 
	//		//�ִ뿬������, �ִ뿬�Ӽս�, 
	//		if(m_bIsJusik)
	//		{
	//			if(i==2||i==6||i==18||i==19||i==20||i==21||i==24||i==30||i==33||i==34||i==35)
	//				strData.Format("%.2f", Data);
	//			else
	//				strData.Format("%d", (INT)Data);
	//		}
	//		strData = GetComma(strData);//Comma
	//		int Index = strHtml.Find(m_pHtmlCodeNameArray[i],0);
	//		//if(strData.Find("-",0)>=0)�ϴ� ����..
	//		//	strHtml.Insert(Index-2, "Color=Red");
	//		strHtml.Replace(m_pHtmlCodeNameArray[i],strData);
	//	}
	//	return strHtml;
	//}
	//catch(...)
	//{
	//	return "";
	//}
return "";
}

void CTabResult::InitGrid()
{

	if(!m_GridResult.GetSafeHwnd())
	{
		CRect rect1;
		GetClientRect(rect1);
		rect1.top =1;
		rect1.left +=1;
		rect1.right-=10;
		HINSTANCE hInst = AfxGetResourceHandle();
		AfxSetResourceHandle(m_pDlgAnalysis->m_hRsc);
		if(!m_GridResult.Create(rect1,this,IDC_STATIC_RESULT,WS_CHILD | WS_TABSTOP | WS_VISIBLE ))
		{
			AfxMessageBox("No");
			return;
		}
		m_GridResult.SetEditable(FALSE);
		m_GridResult.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(255, 255, 255));
		m_GridResult.SetGridLineColor(RGB(204,210,224));
		m_GridResult.SetColumnCount(6);
		m_GridResult.SetRowCount(14);
		m_GridResult.SetBkColor(CLR_BACKGROUND);
		if(m_pFont)
			m_GridResult.SetFont(m_pFont);

		CString str;
		GV_ITEM Item;
		int nWidth = (rect1.right/6) -3;
		char* IndexName[47]={"�������ͷ�","�Ѽ���","������/�Ѽս�","�Ǻ���ռ���","������","","�ѸŸż�","���ͼ�/�սǼ�","�Ⱓ�����","�����ʰ����ͷ�","","�Ѻ���","�Ⱓ������","��ոŸź���",
										"�Ǻ���ռ��ͷ�","������","�Ǻ��ִ�����","�Ǻ��������","��û��������û��","","���͸Ÿż�","���͸Ÿ���(�·�)","�ִ뿬�����ͼ�","�ִ뿬������","","���͸Ÿź���","���Ի��º���","���͸Ÿ���պ���",
										"���������Ǽ��ͷ�","�Ѽս�","�Ǻ��ִ�ս�","�Ǻ���ռս�"," ","","�սǸŸż�","�սǸŸ���(����)","�ִ뿬�ӼսǼ�","�ִ뿬�Ӽս�","","�սǸŸź���","û����º���","�սǸŸ���պ���"};
		int nRowIndex =0;
		for (int col = 0; col < 6; col++)
		{
			Item.mask = GVIF_TEXT;
			m_GridResult.SetColumnWidth(col,nWidth);
			if(!(col%2))
			{
				Item.nFormat = DT_CENTER;
				for(int row = 0; row<14;row++)
				{
					m_GridResult.SetRowHeight(row,23);
					if(IndexName[nRowIndex]=="")
					{
						m_GridResult.SetItemBkColour(row,col,RGB(204,210,224));
						m_GridResult.SetItemBkColour(row,col+1,RGB(204,210,224));
					}
					else
						m_GridResult.SetItemBkColour(row,col,RGB(242,248,254));
					m_GridResult.SetItemText(row,col,IndexName[nRowIndex]);
					++nRowIndex;
				}
			}

		}
		m_GridResult.ExpandColumnsToFit();
		AfxSetResourceHandle(hInst);
	}
}

void CTabResult::SetDataSpreadGrid()
{
	CString strName;
	CStrategyResult* Result;
	ST_DATAINDEX stData;
	double Data;
	CString strData;
	DWORD nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;//�׸��� ����

	CIStrategyItem* Strate = m_pDlgAnalysis->GetStrategyItem();
	int nSize = Strate->GetCntStrategyResult();
	if(nSize > 0)
	{
		Result = Strate->GetStrategyResult(nSize-1);
		switch(Result->m_DealType) {
			case CStrategyResult::NONE:strName = "NONE";
				break;
			case CStrategyResult::BUY:strName  = "�ż�����";
				break;
			case CStrategyResult::EXITSHORT:strName  = "�ŵ�û��";
				break;
			case CStrategyResult::SELL:strName  = "�ŵ�����";
				break;
			case CStrategyResult::EXITLONG: strName = "�ż�û��";
				break;
		}
	}
	else
		strName = "NONE";

	m_strCurrentPos = strName;
	m_pDlgAnalysis->m_pdlgInfo->m_ctlstPosition.SetText(m_strCurrentPos);

	strName = m_pDlgAnalysis->m_stSTName;
	m_strStrategyName = strName;
	m_pDlgAnalysis->m_pdlgInfo->m_ctlSTFILENAME.SetText(m_strStrategyName);

	m_strItemName = m_pDlgAnalysis->m_stCodeName;
	m_pDlgAnalysis->m_pdlgInfo->m_ctlstName.SetText(m_strItemName);

	if(m_iDateIndex < m_pDlgAnalysis->m_stDataIndex.GetSize())
	{
		stData = m_pDlgAnalysis->m_stDataIndex.GetAt(m_iDateIndex);
		strName.Format(" [%s] %s  ~ %s", stData.strGubun, stData.strStartDate,stData.strEndDate);
		m_strPriod = strName;
		m_pDlgAnalysis->m_pdlgInfo->m_ctlstPeriod.SetText(m_strPriod);
	}

	//���� ���ͷ� ���� 
	Data = Strate->GetStatisticalInfo(0);
	strData.Format(" %.2f", Data);
	m_strACCumProf = strData;
	m_GridResult.SetItemText(0,1,m_strACCumProf+"%",nFormat);//Grid
	m_pDlgAnalysis->m_pdlgInfo->m_ctlstNujuk.SetText(m_strACCumProf+"%");
	
	///////////////////////
	////�ɼǿ� ���� ��� : 8�ڸ��� ���� 6�ڸ��� �ֽ� �ɼ� //////////////////////////////////////////////////////////////	
	
	for(int i = 1; i<MAXDATANUM;i++)
	{
		if(i==14) continue;//��û���� ���� ���� �ʽ��ϴ�.
		if(i==24)// �����ʰ����ͷ�:�������ͷ� - �Ⱓ����� ����!
		{
			Data =atof(m_strACCumProf)-Strate->GetStatisticalInfo(21);
		}
		else
			Data = Strate->GetStatisticalInfo(i);
		//�޸� ��ƾ �ʿ� Default!! 
		if(i==1||i==2||i==6||i==18||i==19||i==20||i==21||i==24||i==30||i==33||i==34||i==35)
			strData.Format("%.2f", Data);
		else
		{
			strData.Format("%d", (INT)Data);
		}
		strData = GetComma(strData);//Comma

		switch(i) 
		{
		case PROFITRATEAVG:
				m_GridResult.SetItemText(0,3,strData+"%",nFormat);//Grid
			break;
		case PROFITRATECURRENT:
				m_GridResult.SetItemText(0,5,strData+"%",nFormat);//Grid		
			break;
		case PROFITLOSSALL:
				m_GridResult.SetItemText(1,1,strData,nFormat);//Grid
			break;
		case PROFITALL:
				m_GridResult.SetItemText(1,3,strData,nFormat);//Grid
			break;
		case LOSSALL:
				m_GridResult.SetItemText(1,5,strData,nFormat);//Grid
			break;
		case PROFITLOSSRATEALL:
				m_GridResult.SetItemText(2,1,strData,nFormat);//Grid
			break;
		case MAXPROFIT:
				m_GridResult.SetItemText(2,3,strData,nFormat);//Gridi
			break;
		case MAXLOSS:
				m_GridResult.SetItemText(2,5,strData,nFormat);//Grid
			break;
		case PROFITLOSSAVG:
				m_GridResult.SetItemText(3,1,strData,nFormat);//Grid
			break;
		case PROFITAVG:
				m_GridResult.SetItemText(3,3,strData,nFormat);//Grid
			break;
		case LOSSAVG:
				m_GridResult.SetItemText(3,5,strData,nFormat);//Grid
			break;
		case AMOUNTENTEREDALL:
				m_GridResult.SetItemText(4,1,strData,nFormat);//Grid
			break;
		case AMOUNTEXITALL:
				m_GridResult.SetItemText(4,3,strData,nFormat);//Grid
			break;
		//�׸��� ��ĭ 
		case CNTALL:
				m_GridResult.SetItemText(6,1,strData,nFormat);//Grid
			break;
		case CNTPROFIT:
				m_GridResult.SetItemText(6,3,strData,nFormat);//Grid
			break;
		case CNTLOSS:
				m_GridResult.SetItemText(6,5,strData,nFormat);//Grid
			break;
		case RATEPROFITLOSS:
				m_GridResult.SetItemText(7,1,strData,nFormat);//Grid
			break;
		case RATEPROFIT:
				m_GridResult.SetItemText(7,3,strData+"%",nFormat);//Grid
			break;
		case RATELOSS:
				m_GridResult.SetItemText(7,5,strData+"%",nFormat);//Grid
			break;
		case CHANGERATE:
				m_GridResult.SetItemText(8,1,strData+"%",nFormat);//Grid
			break;
		case MAXPROFITSTRAIGHT:
				m_GridResult.SetItemText(8,3,strData,nFormat);//Grid
			break;
		case MAXLOSSSTRAIGHT:
				m_GridResult.SetItemText(8,5,strData,nFormat);//Grid
			break;
		case RATEPROFITEXCEED:///���� �ʰ� ���ͷ� : �������ͷ�(0) - �Ⱓ�����(21)
				m_GridResult.SetItemText(9,1,strData+"%",nFormat);//Grid
			break;
		case PROFITSTRAIGHT:
				m_GridResult.SetItemText(9,3,strData,nFormat);//Grid
			break;
		case LOSSSTRAIGHT:
				m_GridResult.SetItemText(9,5,strData,nFormat);//Grid
			break;
			//��ĭ
		case CNTCANDLE:
				m_GridResult.SetItemText(11,1,strData,nFormat);//Grid
			break;
		case CNTCANDLEPROFIT:
				m_GridResult.SetItemText(11,3,strData,nFormat);//Grid
			break;
		case CNTCANDLELOSS:
				m_GridResult.SetItemText(11,5,strData,nFormat);//Grid
			break;
		case RATEACCESS:
				m_GridResult.SetItemText(12,1,strData+"%",nFormat);//Grid
			break;
		case CNTCANDLEACCESS:
				m_GridResult.SetItemText(12,3,strData,nFormat);//Grid
			break;
		case CNTCANDLEEXIT:
				m_GridResult.SetItemText(12,5,strData,nFormat);//Grid
			break;
		case CNTCANDLEAVGDEAL:
				m_GridResult.SetItemText(13,1,strData,nFormat);//Grid
			break;
		case CNTCANDLEAVGPROFIT:
				m_GridResult.SetItemText(13,3,strData,nFormat);//Grid
			break;
		case CNTCANDLEAVGLOSS:
				m_GridResult.SetItemText(13,5,strData,nFormat);//Grid
			break;
		}
	}
	m_GridResult.Refresh();
}


void CTabResult::InitHtml()
{
	if(m_strHtml!="")return;
	CStdioFile DefaultFile;
	CString str;
	
	if(!DefaultFile.Open(m_strFilePath+"\\����-�����м�.htm",CFile::modeRead))
	{
		AfxMessageBox("Do Not Create ����-�����м�.htm File.");
		return;
	}

	for(;;){
		if(DefaultFile.ReadString(str))
			m_strHtml+=str;
		else
			break;
	}
	m_strHtml.Replace("color=\"white\"", "");//

	DefaultFile.Close();
}

void CTabResult::InsertData2HtmlString(short nIndex, CString& strHtml, CIStrategyItem* pStrategy)
{
	if(nIndex < 0)
	{
		switch(nIndex)
		{
		case -1:	// CurrentPos
			{
				int nSize = pStrategy->GetCntStrategyResult();
				if(nSize > 0)
				{
					CStrategyResult* Result = pStrategy->GetStrategyResult(nSize-1);
					switch(Result->m_DealType) 
					{
					case CStrategyResult::NONE:
						m_strCurrentPos = "NONE";
						break;
					case CStrategyResult::BUY:
						m_strCurrentPos  = "�ż�����";
						break;
					case CStrategyResult::EXITSHORT:
						m_strCurrentPos  = "�ŵ�û��";
						break;
					case CStrategyResult::SELL:
						m_strCurrentPos  = "�ŵ�����";
						break;
					case CStrategyResult::EXITLONG: 
						m_strCurrentPos = "�ż�û��";
						break;
					}
				}
				else
					m_strCurrentPos = "NONE";

				strHtml += m_strCurrentPos;
			}
			break;
		case -2:	// StrategyName
			{
				m_strStrategyName = m_pDlgAnalysis->m_stSTName;
				strHtml += m_strStrategyName;
			}
			break;
		case -3:	// ItemName
			{
				m_strItemName = m_pDlgAnalysis->m_stCodeName;
				strHtml += m_strItemName;
			}
			break;
		case -4:	// Priod
			{
				if(m_iDateIndex < m_pDlgAnalysis->m_stDataIndex.GetSize())
				{
					ST_DATAINDEX stData = m_pDlgAnalysis->m_stDataIndex.GetAt(m_iDateIndex);

					m_strPriod.Format(" %s %s  ~ %s", stData.strGubun, stData.strStartDate,stData.strEndDate);
					strHtml += m_strPriod;
				}
			}
			break;
		case -5:	// AccumProf
			{
				m_strACCumProf.Format("%.2f%%", pStrategy->GetStatisticalInfo(0));
				strHtml += m_strACCumProf;
			}
		}
	}
	else if(nIndex > 0)
	{
		//		if(nIndex == 14)	// ��û���� ���� ���� �ʽ��ϴ�.
		//			return;

		CString strData;
		double dData = pStrategy->GetStatisticalInfo(nIndex);

		//�ֽ� �ɼ��� ��� .... �Ҽ��� �ڸ� �Ⱥ��϶�...
		//�Ѽ���, ������, �Ѽս�, �Ǻ��ִ�����, �Ǻ��ִ�ս�,������, ��û��������û��, 
		//�ִ뿬������, �ִ뿬�Ӽս�, 
		if(m_bIsJusik && nIndex==1||nIndex==2||nIndex==6||nIndex==9||nIndex==10||nIndex==11||nIndex==19||nIndex==20||nIndex==21||nIndex==30)
			strData.Format("%.2f", dData);
		else
			strData.Format("%d", (INT)dData);

		strHtml += GetComma(strData);//Comma
	}
}

BOOL CTabResult::ParsingstrHtml2(CString& strHtml)
{
	CFile DefaultFile;
	if(!DefaultFile.Open(m_strFilePath+"\\����-�����м�2.htm",CFile::modeRead))
	{
		AfxMessageBox("Do Not Create ����-�����м�2.htm File.");
		return FALSE;
	}

	CIStrategyItem* Strate = m_pDlgAnalysis->GetStrategyItem();

	char chRead[2] = {NULL, };
	char szIndex[3] = {NULL, };
	while(DefaultFile.Read(chRead, 1))
	{
		if(*chRead == _T('K'))
		{
			if(!DefaultFile.Read(chRead, 1))
				break;

			if(*chRead == _T('H'))
			{
				if(!DefaultFile.Read(chRead, 1))
					break;

				if(*chRead == _T('D'))
				{
					if(!DefaultFile.Read(szIndex, 2))
						break;

					// �� �Է���ġ
					InsertData2HtmlString(atoi(szIndex), strHtml, Strate);						
				}
			}
		}
		else
			strHtml += chRead;

	}

	strHtml.Replace("color=\"white\"", "");//
	DefaultFile.Close();

	return TRUE;
}
HBRUSH CTabResult::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDlgInterface::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	pDC->SetBkColor( RGB(255,255,255));//CLR_BACKGROUND;
	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return m_brBackground;
}
